/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wep-1
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.12  2007/10/29 00:30:24  seiki_masashi
  Copyright date

  Revision 1.11  2007/10/27 14:55:52  seiki_masashi
  dataShare-Model の変更をマージ

  Revision 1.10  2006/10/26 06:57:18  kitase_hirotake
  WH_SetIndCallback でコールバックを設定するように変更

  Revision 1.9  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 11:07:19  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.7  2005/06/27 11:10:13  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.6  2005/04/11 04:42:58  seiki_masashi
  コメントの修正

  Revision 1.5  2005/04/07 04:35:05  seiki_masashi
  チャンネル選択の不具合を修正
  アラインメント関係の不具合を修正

  Revision 1.4  2005/04/06 06:41:33  seiki_masashi
  MI_CpuClear32 を 16 に変更

  Revision 1.3  2005/04/05 00:00:11  seiki_masashi
  warning 対策

  Revision 1.2  2005/04/01 12:29:46  seiki_masashi
  WEP Key の計算に HMAC-SHA-1 を使用するように変更

  Revision 1.1  2005/03/25 07:41:37  seiki_masashi
  dataShare-Model に WEP Key の動的設定を加え、wep-1 デモとして新規登録

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
  wep-1 デモ

  dataShare-Model デモに接続時の WEP 使用を追加したデモです。
*/

#include <nitro.h>
#include <nitro/wm.h>
#include <string.h>

#include "main.h"
#include "font.h"
#include "print.h"
#include "key.h"
#include "graphics.h"
#include "wh.h"
#include "menu.h"

/* このデモで使用する GGID */
#define WH_GGID           SDK_MAKEGGID_SYSTEM(0x14)
#define GRAPH_TOTAL_FRAME 60

typedef struct GraphData_
{
    u16     level;
    s16     data;
}
GraphData;

static u16 sForcedChannel = 0;
static u16 sTgid = 0;
static int sSysMode = 0;
static int sSysModeStep = 0;

static s32 sFrame = 0;
static u8 sSendBuf[512] ATTRIBUTE_ALIGN(32);
static u8 sRecvBuf[512] ATTRIBUTE_ALIGN(32);
static BOOL sRecvFlag[WM_NUM_MAX_CHILD + 1];

static WMKeySet sKeySet;

static GraphData sGraphData[1 + WM_NUM_MAX_CHILD][GRAPH_TOTAL_FRAME];
static PRScreen sInfoScreen ATTRIBUTE_ALIGN(32);
static PRScreen sDebugScreen ATTRIBUTE_ALIGN(32);

static int sBeaconCount = 0;
static int sNeedWait = 0;
static BOOL sGraphEnabled = 0;

static WMBssDesc sBssDesc[3];
static Window sRoleMenuWindow;
static Window sSelectChannelWindow;
static Window sSelectParentWindow;
static Window sLobbyWindow;
static Window sErrorWindow;
static Window sBusyWindow;
static Window sOptionWindow;
static Window sWaitWindow;

static MATHRandContext32 sRand;

extern const unsigned char wlicon_image[];
extern const unsigned short wlicon_palette[];

static u16 ParentWEPKeyGenerator(u16 *wepkey, const WMParentParam *parentParam);
static u16 ChildWEPKeyGenerator(u16 *wepkey, const WMBssDesc *bssDesc);

static void TraceWH(const char *fmt, ...)
{
    va_list vlist;
    va_start(vlist, fmt);
    PR_VPrintString(&sDebugScreen, fmt, vlist);
    va_end(vlist);
}

static void printString(const char *fmt, ...)
{
    va_list vlist;
    va_start(vlist, fmt);
    PR_VPrintString(&sInfoScreen, fmt, vlist);
    va_end(vlist);
}

BOOL isDataReceived(int index)
{
    return sRecvFlag[index];
}

ShareData *getRecvData(int index)
{
    return (ShareData *) (&(sRecvBuf[index * sizeof(ShareData)]));
}

ShareData *getSendData(void)
{
    return (ShareData *) sSendBuf;
}

PRScreen *getInfoScreen(void)
{
    return &sInfoScreen;
}

void changeSysMode(int s)
{
    if (sSysMode == s)
    {
        return;
    }

    OS_Printf("sysmode = %d\n", s);
    sSysMode = s;
    sSysModeStep = 0;
}

static void indicateCallback(void *arg)
{
    WMIndCallback *cb;
    cb = (WMIndCallback *)arg;
    if (cb->state == WM_STATECODE_BEACON_RECV)
    {
        sBeaconCount = 2;
    }
}

static void scanCallback(WMBssDesc *bssdesc)
{
    char    buf[ITEM_LENGTH_MAX];
    int     i;

    for (i = 0; i < sSelectParentWindow.itemnum; ++i)
    {
        if (memcmp(sBssDesc[i].bssid, bssdesc->bssid, 12) == 0)
        {
            // bssid が同じ親は同一エントリとして除外する。
            return;
        }
    }

    WH_PrintBssDesc(bssdesc);

    // 情報を保存しておく。
    sBssDesc[sSelectParentWindow.itemnum] = *bssdesc;

    // 親機選択用のメニュー項目を作成。
    (void)snprintf(buf,
                   ITEM_LENGTH_MAX,
                   "[%d]channel%d", sSelectParentWindow.itemnum + 1, bssdesc->channel);
    addItemToWindow(&sSelectParentWindow, buf);
    WH_PrintBssDesc(bssdesc);
}

static void forceSpinWait(void)
{
    // OS_SpinWait を使用して、処理落ち状態を任意に作り出す
    // ための処理。

    static int waitcycle = 0;

    if (getKeyInfo()->cnt & PAD_BUTTON_L)
    {
        waitcycle += 4000;
        // OS_Printf("wait = %d\n", waitcycle);

    }
    else if (getKeyInfo()->cnt & PAD_BUTTON_R)
    {
        waitcycle -= 4000;
        if (waitcycle < 0)
        {
            waitcycle = 0;
        }
        // OS_Printf("wait = %d\n", waitcycle);
    }

    OS_SpinWait((u32)waitcycle);
}

static void ModeSelectRole(void)
{
    static const char *menuitems[] = {
        "Start (Parent mode)",
        "Start (Child mode)",
        "Option",
        NULL
    };

    if (sSysModeStep == 0)
    {
        sRoleMenuWindow.selected = 0;
        setupWindow(&sRoleMenuWindow, 16, 16, WIN_FLAG_SELECTABLE, 24, 24, 16);
        if (sRoleMenuWindow.itemnum == 0)
        {
            int     i;
            for (i = 0; menuitems[i] != NULL; ++i)
            {
                addItemToWindow(&sRoleMenuWindow, menuitems[i]);
            }
        }
        openWindow(&sRoleMenuWindow);
    }

    if (sRoleMenuWindow.state == WIN_STATE_CLOSED)
    {
        if (sRoleMenuWindow.selected < 0)
        {
            openWindow(&sRoleMenuWindow);
            return;
        }

        switch (sRoleMenuWindow.selected)
        {
        case 0:
            if (sForcedChannel == 0)
            {
                // 電波使用率から最適なチャンネルを取得して接続する。
                (void)WH_StartMeasureChannel();

            }
            else
            {
                // 手動で選択したチャンネルを使用して接続開始。
                static u32 wepSeed;
                wepSeed = MATH_Rand32(&sRand, 0);
                // WEP 計算用の Seed 値を UserGameInfo に埋め込む
                WH_SetUserGameInfo((u16 *)&wepSeed, 4);
                sTgid++;
                (void)WH_ParentConnect(WH_CONNECTMODE_DS_PARENT, sTgid, sForcedChannel);
            }
            changeSysMode(SYSMODE_LOBBY);
            break;

        case 1:
            {
                // 親機を検索しに行く。
                static const u8 ANY_PARENT[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
                enum
                { ALL_CHANNEL = 0 };

                initWindow(&sSelectParentWindow);
                setupWindow(&sSelectParentWindow, 16, 16, WIN_FLAG_SELECTABLE, 8, 8, 16);
                (void)WH_StartScan(scanCallback, ANY_PARENT, ALL_CHANNEL);
                changeSysMode(SYSMODE_SCAN_PARENT);
            }
            break;

        case 2:
            // オプション画面へ。
            changeSysMode(SYSMODE_OPTION);
            break;

        default:
            break;
        }
    }
}

static void ModeOption(void)
{
    // オプション画面の処理。

    if (sSysModeStep == 0)
    {
        initWindow(&sOptionWindow);
        setupWindow(&sOptionWindow, 16, 16, WIN_FLAG_SELECTABLE, 8, 8, 16);
        addItemToWindow(&sOptionWindow, "Select channel (parent)");

        if (sGraphEnabled)
        {
            addItemToWindow(&sOptionWindow, "Disable beacon graph");
        }
        else
        {
            addItemToWindow(&sOptionWindow, "Enable beacon graph");
        }

        openWindow(&sOptionWindow);
        return;
    }

    if (sOptionWindow.state == WIN_STATE_CLOSED)
    {
        if (sOptionWindow.selected < 0)
        {
            // キャンセルされた。
            changeSysMode(SYSMODE_SELECT_ROLE);
            return;
        }

        if (sOptionWindow.selected == 0)
        {
            // チャンネル選択画面へ。
            changeSysMode(SYSMODE_SELECT_CHANNEL);

        }
        else if (sOptionWindow.selected == 1)
        {
            sGraphEnabled = sGraphEnabled ? FALSE : TRUE;
            changeSysMode(SYSMODE_SELECT_ROLE);
        }
    }
}

static void ModeLobby(void)
{
    // ロビー画面の処理。

    u16     bmap;
    int     i;

    if (sSysModeStep == 0)
    {
        initWindow(&sLobbyWindow);
        setupWindow(&sLobbyWindow, 16, 16, WIN_FLAG_NONE, 8, 8, 16);

        for (i = 0; i < WH_CHILD_MAX; ++i)
        {
            addItemToWindow(&sLobbyWindow, "");
        }

        addItemToWindow(&sLobbyWindow, "");
        addItemToWindow(&sLobbyWindow, "Push A to start");

        openWindow(&sLobbyWindow);
        return;
    }

    bmap = WH_GetBitmap();
    for (i = 0; i < WH_CHILD_MAX; ++i)
    {
        if (bmap & (1 << i))
        {
            (void)OS_SNPrintf(sLobbyWindow.item[i], ITEM_LENGTH_MAX, "[%02d] - entry", i);
        }
        else
        {
            (void)OS_SNPrintf(sLobbyWindow.item[i], ITEM_LENGTH_MAX, "[%02d] - waiting", i);
        }
    }

    if (sLobbyWindow.state == WIN_STATE_CLOSED)
    {
        getSendData()->command = SHARECMD_NONE;
        if (sLobbyWindow.selected < 0)
        {
            WH_Finalize();
            changeSysMode(SYSMODE_SELECT_ROLE);
            return;
        }

        changeSysMode(SYSMODE_PARENT);
    }
}

static void ModeLobbyWait(void)
{
    // 子機のロビー待機中の処理。
    // 親機が開始の合図を送ってくるまで、このまま待つ。

    if (sSysModeStep == 0)
    {
        initWindow(&sWaitWindow);
        setupWindow(&sWaitWindow, 32, 56, WIN_FLAG_NOCONTROL, 8, 8, 8);
        addItemToWindow(&sWaitWindow, "\\2Accepted.");
        addItemToWindow(&sWaitWindow, "\\2Waiting for parent...");
        openWindow(&sWaitWindow);
        return;
    }

    if (getRecvData(0)->command == SHARECMD_EXITLOBBY)
    {
        closeWindow(&sWaitWindow);
        changeSysMode(SYSMODE_CHILD);
    }
}

static void ModeSelectChannel(void)
{
    static u16 channelList[15];        // Auto select + 最大14ch
    // チャンネル選択画面。
    if (sSysModeStep == 0)
    {
        setupWindow(&sSelectChannelWindow, 16, 16, WIN_FLAG_SELECTABLE, 16, 12, 16);

        if (sSelectChannelWindow.itemnum == 0)
        {
            u16     pattern;
            int     i, j;
            for (i = 0; i < 14; i++)
            {
                channelList[i] = 0;
            }
            pattern = WH_GetAllowedChannel();
            addItemToWindow(&sSelectChannelWindow, "Auto select");
            for (i = 1, j = 1; i <= 14; ++i)
            {
                if (pattern & (1 << (i - 1)))
                {
                    char    buf[ITEM_LENGTH_MAX];
                    (void)OS_SNPrintf(buf, ITEM_LENGTH_MAX, "Channel %d", i);
                    channelList[j] = (u16)i;
                    ++j;
                    addItemToWindow(&sSelectChannelWindow, buf);
                }
            }
        }

        openWindow(&sSelectChannelWindow);
    }

    if (sSelectChannelWindow.state == WIN_STATE_CLOSED)
    {
        if (sSelectChannelWindow.selected >= 0)
        {
            sForcedChannel = channelList[sSelectChannelWindow.selected];
        }

        // 役割選択画面に戻る。
        changeSysMode(SYSMODE_SELECT_ROLE);
    }
}

static void ModeSelectParent(void)
{
    // 親機をリスト表示し選択する。
    // この状態の時は WH が親機をスキャンしており、スキャン中に新しく
    // 発見された親機もすぐメニューに追加、反映される。

    if (WH_GetSystemState() == WH_SYSSTATE_CONNECT_FAIL)
    {
        // WM_StartConnect()に失敗した場合にはWM内部のステートが不正になっている為
        // 一度WM_ResetでIDLEステートにリセットする必要があります。
        WH_Reset();
        return;
    }

    if (sSysModeStep == 0)
    {
        openWindow(&sSelectParentWindow);
    }

    // 親機検索画面をユーザが閉じたか
    if ((sSelectParentWindow.state == WIN_STATE_CLOSED))
    {
        if (WH_GetSystemState() == WH_SYSSTATE_SCANNING)
        {
            // 親機スキャン中であれば一旦スキャンを終了する
            (void)WH_EndScan();
            return;
        }

        if (WH_GetSystemState() == WH_SYSSTATE_IDLE)
        {
            if (sSelectParentWindow.selected < 0)
            {
                WH_Finalize();
                changeSysMode(SYSMODE_SELECT_ROLE);
                return;
            }

            // スキャン中でなく、なおかつ親機をユーザが選択していればデータシェアリング開始
            (void)WH_ChildConnect(WH_CONNECTMODE_DS_CHILD,
                                  &(sBssDesc[sSelectParentWindow.selected]));
            changeSysMode(SYSMODE_LOBBYWAIT);
        }
    }
}

static void ModeError(void)
{
    // エラー状態。
    if (sSysModeStep == 0)
    {
        initWindow(&sErrorWindow);
        setupWindow(&sErrorWindow, 16, 16, WIN_FLAG_NONE, 8, 8, 16);

        addItemToWindow(&sErrorWindow, "\\1Error has occured!");

        if (WH_GetLastError() == WM_ERRCODE_OVER_MAX_ENTRY)
        {
            addItemToWindow(&sErrorWindow, "\\4Rejected\n");
        }

        if (WH_GetLastError() == WH_ERRCODE_DISCONNECTED)
        {
            addItemToWindow(&sErrorWindow, "\\4Disconnected by parent\n");
        }

        if (WH_GetLastError() == WH_ERRCODE_PARENT_NOT_FOUND)
        {
            addItemToWindow(&sErrorWindow, "\\4Parent not found\n");
        }

        if (WH_GetLastError() == WH_ERRCODE_LOST_PARENT)
        {
            addItemToWindow(&sErrorWindow, "\\4Lost parent\n");
        }

        addItemToWindow(&sErrorWindow, "");
        addItemToWindow(&sErrorWindow, "\\fPush A to reset");

        closeAllWindow();
        openWindow(&sErrorWindow);
    }

    if (sErrorWindow.state == WIN_STATE_CLOSED)
    {
        WH_Finalize();
        getRecvData(0)->command = SHARECMD_NONE;
        changeSysMode(SYSMODE_SELECT_ROLE);
    }
}

static void printShareData(void)
{
    s32     i;
    ShareData *sd;

    sd = getSendData();
    printString("\\2Send:     0x%04x 0x%04x\n", sd->key, sd->count & 0xffff);

    printString("\\4Receive:\n");
    for (i = 1; i < (WM_NUM_MAX_CHILD + 1); i++)
    {
        if (sRecvFlag[i])
        {
            sd = getRecvData(i);
            printString("\\4Child%02d:  0x%04x 0x%04x\n", i, sd->key, sd->count & 0xffff);

        }
        else
        {
            printString("No child\n");
        }
    }
}

static void ModeParent(void)
{
    printString("\n  \\fParent mode\n\n");
    printShareData();
}

static void ModeChild(void)
{
    ShareData *sd;

    printString("\n  \\fChild mode\n\n");
    printShareData();

    sd = (ShareData *) getRecvData(0);
    printString("\\4Parent:   0x%04x 0x%04x\n", sd->key, sd->count & 0xffff);
}

static void VBlankIntr(void)
{
    /*
       他のサンプルの中には、ここ（VBlankIntr関数）で StepDataSharing を
       しているものがあります。
       このサンプルでも、以前はここで StepDataSharing をしていました。

       そうなっている（いた）理由は、安定した通信のために WM_StepDataSharing
       （及び WM_GetKeySet）は「そのフレームの MP 通信開始より前に」
       呼ばなければならないからです。
       SDK がデフォルトの MP 通信の準備を行う V カウントは子機が 240、
       親機が 260 ですので、V Blank 開始直後にセットするのが一番問題が
       少なくなります（このドキュメントが書かれた時点では未実装ですが、
       MP 通信開始の V カウントは初期化時に設定可能になる予定です。

       30fps のゲームなどでは、通常は StepDS の呼び出し回数を 2 フレームに
       1 回に制御するコードが必要です。
       その場合、WM_ERRCODE_NO_DATASET が返った時（処理落ちが発生した場合）
       には、2 フレーム待つのではなく、1 フレームだけ待つようにしてください。
       そうしなければ、親子で 1 フレームだけずれている場合に修正できません。
     */

    updateKeys();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

static void initAllocSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

static void drawPowerGraph(void)
{
    static const GXRgb linecolor[4] = {
        GX_RGB(15, 0, 0),              // dark red (dead)
        GX_RGB(31, 31, 0),             // yellow
        GX_RGB(0, 31, 0),              // green
        GX_RGB(20, 31, 20),            // light green
    };

    int     midx, ringidx;

    ringidx = (sFrame % GRAPH_TOTAL_FRAME);

    for (midx = 0; midx < WM_NUM_MAX_CHILD + 1; ++midx)
    {
        sGraphData[midx][ringidx].data = (s16)getRecvData(midx)->data;
        sGraphData[midx][ringidx].level = (u16)getRecvData(midx)->level;
    }

    G3_PolygonAttr(GX_LIGHTMASK_NONE,
                   GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, GX_POLYGON_ATTR_MISC_DISP_1DOT);
    G3_TexImageParam(GX_TEXFMT_NONE,
                     GX_TEXGEN_NONE,
                     GX_TEXSIZE_S16,
                     GX_TEXSIZE_T16, GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);

    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_PushMtx();
    G3_Identity();
    G3_Translate(0, 0, -FX16_ONE * 4);

    G3_Begin(GX_BEGIN_TRIANGLES);

    for (midx = 1; midx < WM_NUM_MAX_CHILD + 1; ++midx)
    {
        int     basey, ys, ye, gi, x, level;
        basey = ((WM_NUM_MAX_CHILD / 2 - midx) * 9 + 6) * FX16_ONE / 64;

        for (gi = 0; gi < GRAPH_TOTAL_FRAME; ++gi)
        {
            int     ri;
            ri = (ringidx - gi);
            if (ri < 0)
            {
                ri += GRAPH_TOTAL_FRAME;
            }

            ys = sGraphData[midx][ri].data;
            level = sGraphData[midx][ri].level;

            ++ri;
            if (ri >= GRAPH_TOTAL_FRAME)
            {
                ri -= GRAPH_TOTAL_FRAME;
            }

            ye = sGraphData[midx][ri].data;

            x = -(gi - GRAPH_TOTAL_FRAME / 2) * 3;
            x *= FX16_ONE / 64;
            ys = ys * FX16_ONE / 64 + basey;
            ye = ye * FX16_ONE / 64 + basey;

            G3_Color(linecolor[level]);

            G3_Vtx((fx16)x, (fx16)ys, 0);
            G3_Vtx((fx16)(x + FX16_ONE / 64 / 2), (fx16)(ys + FX16_ONE / 64), 0);
            G3_Vtx((fx16)(x + 3 * FX16_ONE / 64), (fx16)ye, 0);
        }
    }

    G3_End();
    G3_PopMtx(1);
}

static void drawPowerIcon(void)
{
    // GUIDELINE
    // 電波受信強度アイコンの表示。
    setupPseudo2DCamera();

    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_DECAL, GX_CULL_NONE, 0, 31, 0);
    G3_PushMtx();
    G3_MtxMode(GX_MTXMODE_TEXTURE);
    G3_Identity();

    G3_TexImageParam(GX_TEXFMT_PLTT16,
                     GX_TEXGEN_TEXCOORD,
                     GX_TEXSIZE_S16,
                     GX_TEXSIZE_T16,
                     GX_TEXREPEAT_NONE,
                     GX_TEXFLIP_NONE,
                     GX_TEXPLTTCOLOR0_USE, (u32)(0x2000 + WM_GetLinkLevel() * 16 * 16 / 2));
    G3_TexPlttBase(0x2000, GX_TEXFMT_PLTT16);
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    drawPseudo2DTexQuad(224, 160, 16, 16, 16, 16);
}

static void drawRadioIcon(void)
{
    // GUIDELINE
    // 電波発信アイコンの表示。
    int     i;
    G3_TexPlttBase(0x2000, GX_TEXFMT_PLTT16);
    G3_TexImageParam(GX_TEXFMT_PLTT16,
                     GX_TEXGEN_TEXCOORD,
                     GX_TEXSIZE_S16,
                     GX_TEXSIZE_T16,
                     GX_TEXREPEAT_NONE,
                     GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0x2000 + 4 * 16 * 16 / 2);

    for (i = 0; i < 2; ++i)
    {
        drawPseudo2DTexQuad(16,
                            12 + i * 24 + ((i == sRoleMenuWindow.selected) ? (sFrame / 15 & 1) : 0),
                            16, 16, 16, 16);
    }
}

static void updateShareData(void)
{
    if (WH_GetSystemState() == WH_SYSSTATE_DATASHARING)
    {
        if (WH_StepDS(sSendBuf))
        {
            u16     i;
            for (i = 0; i < WM_NUM_MAX_CHILD + 1; ++i)
            {
                u8     *adr;
                ShareData *sd;

                adr = (u8 *)WH_GetSharedDataAdr(i);
                sd = (ShareData *) & (sRecvBuf[i * sizeof(ShareData)]);

                if (adr != NULL)
                {
                    MI_CpuCopy8(adr, sd, sizeof(ShareData));
                    sRecvFlag[i] = TRUE;

                }
                else
                {
                    sd->level = 0;
                    sd->data = 0;
                    sRecvFlag[i] = FALSE;
                }
            }

            sNeedWait = FALSE;

        }
        else
        {
            u16     i;
            for (i = 0; i < WM_NUM_MAX_CHILD + 1; ++i)
            {
                sRecvFlag[i] = FALSE;
            }

            sNeedWait = TRUE;
        }

    }
    else
    {
        u16     i;
        for (i = 0; i < WM_NUM_MAX_CHILD + 1; ++i)
        {
            sRecvFlag[i] = FALSE;
        }

        sNeedWait = FALSE;
    }
}

static void packSendData(void)
{
    ShareData *senddata;

    if (sNeedWait)
    {
        return;
    }

    senddata = getSendData();
    senddata->command = (sSysMode == SYSMODE_LOBBY) ? SHARECMD_NONE : SHARECMD_EXITLOBBY;

    senddata->level = (u16)WM_GetLinkLevel();

    senddata->data = 0;
    senddata->key = getKeyInfo()->cnt;
    senddata->count = sFrame & 0xffff;

    if (sBeaconCount != 0)
    {
        senddata->data += sBeaconCount * senddata->level;

        if (sBeaconCount > 0)
        {
            sBeaconCount = -sBeaconCount + 1;
        }
        else
        {
            sBeaconCount = -sBeaconCount - 1;
        }
    }
}

static void mainLoop(void)
{
    int retry = 0;
    enum {
        MAX_RETRY = 5
    };

    for (sFrame = 0; TRUE; sFrame++)
    {
        int     whstate;
        int     prevmode;

        whstate = WH_GetSystemState();
        prevmode = sSysMode;

        switch (whstate)
        {
        case WH_SYSSTATE_CONNECT_FAIL:
            // 接続失敗は何度かリトライする。
            if (sSysMode == SYSMODE_LOBBYWAIT && retry < MAX_RETRY)
            {
                changeSysMode(SYSMODE_SELECT_PARENT);
                sSysModeStep = 1; // window が開かないように
                retry++;
                break;
            }
            // リトライ失敗のときはそのままエラー状態へ

        case WH_SYSSTATE_ERROR:
            // エラー発生時は WH 状態が優先。
            changeSysMode(SYSMODE_ERROR);
            break;

        case WH_SYSSTATE_MEASURECHANNEL:
            {
                u16     channel = WH_GetMeasureChannel();
                static u32 wepSeed;
                wepSeed = MATH_Rand32(&sRand, 0);
                // WEP 計算用の Seed 値を UserGameInfo に埋め込む
                WH_SetUserGameInfo((u16 *)&wepSeed, 4);
                sTgid++;
                (void)WH_ParentConnect(WH_CONNECTMODE_DS_PARENT, sTgid, channel);
            }
            break;

        default:
            break;
        }

        PR_ClearScreen(&sInfoScreen);

        // 負荷実験。
        forceSpinWait();

        switch (sSysMode)
        {
        case SYSMODE_SELECT_ROLE:
            // 役割（親機・子機）選択画面。
            ModeSelectRole();
            retry = 0;
            break;

        case SYSMODE_SELECT_CHANNEL:
            // チャンネル選択画面。
            ModeSelectChannel();
            break;

        case SYSMODE_LOBBY:
            // ロビー画面。
            ModeLobby();
            break;

        case SYSMODE_LOBBYWAIT:
            // ロビー待機画面。
            ModeLobbyWait();
            break;

        case SYSMODE_OPTION:
            // オプション画面。
            ModeOption();
            break;

        case SYSMODE_SCAN_PARENT:
        case SYSMODE_SELECT_PARENT:
            // 親機選択画面。
            ModeSelectParent();
            break;

        case SYSMODE_ERROR:
            // エラー発生報告画面。
            ModeError();
            break;

        case SYSMODE_PARENT:
            // 親機モード時画面。
            ModeParent();
            break;

        case SYSMODE_CHILD:
            // 子機モード時画面。
            ModeChild();
            break;

        default:
            break;
        }

        if ((whstate == WH_SYSSTATE_BUSY)
            || ((whstate == WH_SYSSTATE_SCANNING) && (sSelectParentWindow.itemnum == 0)))
        {
            sBusyWindow.state = WIN_STATE_OPENED;

        }
        else
        {
            sBusyWindow.state = WIN_STATE_CLOSED;
        }

        updateAllWindow();
        drawAllWindow();

        drawPowerIcon();

        if ((sSysMode == SYSMODE_PARENT) || (sSysMode == SYSMODE_CHILD))
        {
            if (sGraphEnabled)
            {
                drawPowerGraph();
            }
        }

        if ((sSysMode == SYSMODE_SELECT_ROLE) && (sRoleMenuWindow.state == WIN_STATE_OPENED))
        {
            drawRadioIcon();
        }

        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        if (!sNeedWait && ((sSysMode == SYSMODE_PARENT) || (sSysMode == SYSMODE_CHILD)))
        {
            // ... 実際のゲームではここでキャラクターなどの更新処理を
            // 行う事になります。
        }

        DC_FlushRange(sInfoScreen.screen, sizeof(u16) * PR_SCREEN_SIZE);
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */

        // DC_WaitWriteBufferEmpty();
        GX_LoadBG2Scr(sInfoScreen.screen, 0, sizeof(u16) * PR_SCREEN_SIZE);
        DC_FlushRange(sDebugScreen.screen, sizeof(u16) * PR_SCREEN_SIZE);
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */

        // DC_WaitWriteBufferEmpty();
        GXS_LoadBG2Scr(sDebugScreen.screen, 0, sizeof(u16) * PR_SCREEN_SIZE);

        OS_WaitVBlankIntr();

        packSendData();
        updateShareData();

        if (prevmode == sSysMode)
        {
            ++sSysModeStep;
        }
    }
}

void NitroMain(void)
{
    OS_Init();
    FX_Init();
    RTC_Init();

    initGraphics();

    GX_SetBankForBG(GX_VRAM_BG_256_AB);
    GX_SetBankForBGExtPltt(GX_VRAM_BGEXTPLTT_01_F);
    G2_SetBG2ControlText(GX_BG_SCRSIZE_TEXT_256x256,
                         GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000);

    GX_BeginLoadTex();
    GX_LoadTex(wlicon_image, 0x2000, 16 * 16 * 5);
    GX_EndLoadTex();

    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(wlicon_palette, 0x2000, 32);
    GX_EndLoadTexPltt();

    GX_LoadBG2Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
    GX_SetBankForSubBGExtPltt(GX_VRAM_SUB_BGEXTPLTT_0123_H);
    G2S_SetBG2ControlText(GX_BG_SCRSIZE_TEXT_256x256,
                          GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x00000);

    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG2);
    G2S_SetBG2Priority(0);
    G2S_BG2Mosaic(FALSE);

    GXS_LoadBG2Char(d_CharData, 0, sizeof(d_CharData));
    GXS_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    initAllocSystem();

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    sInfoScreen.scroll = FALSE;
    sDebugScreen.scroll = TRUE;

    // 無線初期化
    // wh 内部で呼ばれる WM_SetIndCallback で指定するコールバックを設定
    WH_SetIndCallback(indicateCallback);
    (void)WH_Initialize();

    // WH 初期設定
    WH_SetGgid(WH_GGID);
    WH_SetDebugOutput(TraceWH);

    // WEP Key Generator 設定
    WH_SetParentWEPKeyGenerator(ParentWEPKeyGenerator);
    WH_SetChildWEPKeyGenerator(ChildWEPKeyGenerator);

    // WEP Key の種用の乱数生成機の初期化
    {
        u64     randSeed = 0;
        RTCDate date;
        RTCTime time;
        if (RTC_GetDateTime(&date, &time) == RTC_RESULT_SUCCESS)
        {
            randSeed =
                (((((((u64)date.year * 16ULL + date.month) * 32ULL) + date.day) * 32ULL +
                   time.hour) * 64ULL + time.minute) * 64ULL + time.second);
        }
        MATH_InitRand32(&sRand, randSeed);
    }

    GX_DispOn();
    GXS_DispOn();

    initWindow(&sRoleMenuWindow);
    initWindow(&sSelectChannelWindow);
    initWindow(&sSelectParentWindow);
    initWindow(&sLobbyWindow);
    initWindow(&sErrorWindow);
    initWindow(&sOptionWindow);
    initWindow(&sWaitWindow);
    initWindow(&sBusyWindow);

    setupWindow(&sBusyWindow, 64, 80, WIN_FLAG_NOCONTROL, 8, 8, 16);
    addItemToWindow(&sBusyWindow, "\\2Working...");

    // IPL での A ボタン押下対策
    updateKeys();

    // initCharacter();
    changeSysMode(SYSMODE_SELECT_ROLE);
    mainLoop();
}

// WEP Key 作成用の共通鍵（親子で共通の鍵を使用する）
// アプリケーションごとに固有のものとすること
// ASCII 文字列である必要はなく、任意の長さのバイナリデータでよい
static char *sSecretKey = "this is a secret key for HMAC";
static u32 sSecretKeyLength = 30;      // 鍵は 20 バイト以上であることが望ましい

// 親機用の WEP Key の計算ルーチン
u16 ParentWEPKeyGenerator(u16 *wepkey, const WMParentParam *parentParam)
{
    u16     data[20 / sizeof(u16)];
    u16     tmpWep[20 / sizeof(u16)];
    u8      macAddress[WM_SIZE_MACADDR];

    OS_GetMacAddress(macAddress);
    MI_CpuClear16(data, sizeof(data));

    // 前もって WH_ParentConnect 関数の直前に WH_SetUserGameInfo 関数で
    // UserGameInfo に設定しておいた Seed 値を WEP Key に反映させる
    MI_CpuCopy16(parentParam->userGameInfo, data, 4);
    // 同じアルゴリズムを異なるアプリケーションで使いまわせるように
    // GGID も WEP Key に反映させる
    MI_CpuCopy16(&parentParam->ggid, data + 2, sizeof(u32));
    *(u16 *)(data + 4) = parentParam->tgid;
    // 親機が異なれば WEP Key も異なるように MAC Address も WEP Key に反映させる
    MI_CpuCopy8(macAddress, (u8 *)(data + 5), WM_SIZE_MACADDR);

    // HMAC-SHA-1 で鍵付ハッシュ値を取り、128bit に切り詰めて WEP Key とする。
    // HMAC で使用する鍵はアプリケーション固有のものとすること。
    // (HMAC に関しては RFC2104 参照)
    MATH_CalcHMACSHA1(tmpWep, data, sizeof(data), sSecretKey, sSecretKeyLength);
    MI_CpuCopy8(tmpWep, wepkey, 16);
    OS_TPrintf("wepkey: %04x%04x %04x%04x %04x%04x %04x%04x\n", wepkey[0], wepkey[1],
               wepkey[2], wepkey[3], wepkey[4], wepkey[5], wepkey[6], wepkey[7]);

    return WM_WEPMODE_128BIT;
}

// 子機用の WEP Key の計算ルーチン
u16 ChildWEPKeyGenerator(u16 *wepkey, const WMBssDesc *bssDesc)
{
    u16     data[20 / sizeof(u16)];
    u16     tmpWep[20 / sizeof(u16)];

    MI_CpuClear16(data, sizeof(data));

    // UserGameInfo 内に埋め込まれていた Seed 値を使用して WEP Key を計算
    MI_CpuCopy16(bssDesc->gameInfo.userGameInfo, data, 4);
    // GGID や TGID や親機の MAC Address  も考慮に入れる
    MI_CpuCopy16(&bssDesc->gameInfo.ggid, data + 2, sizeof(u32));
    *(u16 *)(data + 4) = bssDesc->gameInfo.tgid;
    MI_CpuCopy8(bssDesc->bssid, (u8 *)(data + 5), WM_SIZE_MACADDR);

    // HMAC-SHA-1 で鍵付ハッシュ値を取り、128bit に切り詰めて WEP Key とする。
    // HMAC で使用する鍵はアプリケーション固有のものとすること。
    // (HMAC に関しては RFC2104 参照)
    MATH_CalcHMACSHA1(tmpWep, data, sizeof(data), sSecretKey, sSecretKeyLength);
    MI_CpuCopy8(tmpWep, wepkey, 16);
    OS_TPrintf("wepkey: %04x%04x %04x%04x %04x%04x %04x%04x\n", wepkey[0], wepkey[1],
               wepkey[2], wepkey[3], wepkey[4], wepkey[5], wepkey[6], wepkey[7]);

    return WM_WEPMODE_128BIT;
}

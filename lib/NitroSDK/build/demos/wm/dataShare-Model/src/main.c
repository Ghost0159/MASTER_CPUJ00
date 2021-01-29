/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - simple-1
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.48  2007/10/27 14:56:09  seiki_masashi
  接続に失敗した場合のリトライ処理の追加

  Revision 1.47  2007/10/26 09:44:08  yosizaki
  add OAM rendering (from ball.h)

  Revision 1.46  2007/10/25 01:59:28  seiki_masashi
  子機からの接続時に ssid のユーザ領域に本体ニックネームを設定する機能の修正

  Revision 1.45  2007/10/24 14:18:29  seiki_masashi
  親機の UserGameInfo に本体ニックネームを代入するように修正
  子機の ssid に本体ニックネームを設定するように変更

  Revision 1.44  2007/10/23 13:41:08  seiki_masashi
  親機のニックネーム送信に関する不具合を修正

  Revision 1.43  2007/10/19 10:35:23  yosizaki
  fix timing of PAD_Read().

  Revision 1.42  2007/10/19 09:46:40  yosizaki
  add OBJ view.

  Revision 1.41  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.40  2006/10/26 06:57:08  kitase_hirotake
  WH_SetIndCallback でコールバックを設定するように変更

  Revision 1.39  2006/04/10 04:50:11  kitase_hirotake
  データシェアする子機台数を WM_NUM_MAX_CHILD でなく WH_CHILD_MAX を参照するように変更

  Revision 1.38  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.37  2005/11/25 01:35:20  seiki_masashi
  構造体名を GameInfo から MyGameInfo に変更

  Revision 1.36  2005/11/24 04:27:58  adachi_hiroaki
  コメント追加

  Revision 1.35  2005/11/21 11:07:00  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.34  2005/11/21 00:41:10  adachi_hiroaki
  親機選択画面に参加人数とニックネームを表示するようにした

  Revision 1.33  2005/11/11 07:25:42  yosizaki
  fix sub-BG chrbase setting.

  Revision 1.32  2005/06/27 11:10:13  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.31  2005/04/07 04:35:20  seiki_masashi
  チャンネル選択の不具合を修正

  Revision 1.30  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.29  2005/02/18 14:23:18  seiki_masashi
  Copyright 表記の修正

  Revision 1.28  2005/02/18 12:08:23  seiki_masashi
  warning 対策

  Revision 1.27  2004/12/01 07:24:08  sasakis
  コメントの見直しと、エラー復帰後のロビー待機バグのfix

  Revision 1.26  2004/11/08 02:47:11  takano_makoto
  scanCallbackの返り値変更。WH_EndScanを追加

  Revision 1.25  2004/11/05 09:16:48  sasakis
  ウィンドウの挙動を改良し、コードを整理した。

  Revision 1.24  2004/11/05 04:27:40  sasakis
  ロビー画面、親機選択画面の追加と、それに必要な改造（scan関連など）。

  Revision 1.23  2004/11/02 07:12:55  sasakis
  コード整理。

  Revision 1.22  2004/11/02 01:07:34  sasakis
  意味の無いコードになってしまっていた部分を戻した他、整理等。

  Revision 1.21  2004/11/01 09:47:21  seiki_masashi
  不要な文を削除

  Revision 1.20  2004/11/01 09:42:17  seiki_masashi
  sSendBuf と sRecvBuf を 256 バイトから 512 バイトへ変更

  Revision 1.19  2004/11/01 04:11:44  sasakis
  処理落ち対策コードなどの追加（作業中コードを含む暫定版）。

  Revision 1.18  2004/10/29 02:06:37  takano_makoto
  WH_ChildConnectAutoのインターフェイス変更

  Revision 1.17  2004/10/28 08:10:19  takano_makoto
  WH_Connectの関数名を変更

  Revision 1.16  2004/10/27 03:47:00  takano_makoto
  MeasureChannelを手動で実行するように変更

  Revision 1.15  2004/10/22 07:35:18  sasakis
  共有化した wh に対応。

  Revision 1.14  2004/10/22 06:58:26  sasakis
  電波放射アイコンの追加。

  Revision 1.13  2004/10/21 00:43:34  yosizaki
  add SHARED_WH switch.

  Revision 1.12  2004/10/06 05:11:35  sasakis
  グラフ表示の追加など。

  Revision 1.11  2004/09/30 06:19:57  seiki_masashi
  インデントを変更

  Revision 1.10  2004/09/30 06:16:07  seiki_masashi
  WM_StepDataSharing の呼び出し位置を変更

  Revision 1.9  2004/09/24 09:43:35  sasakis
  細かいバグフィックスをいくつか。

  Revision 1.8  2004/09/24 06:12:35  sasakis
  チャンネル手動選択機能を追加。

  Revision 1.7  2004/09/22 09:39:33  sasakis
  デバグ画面を追加。

  Revision 1.6  2004/09/17 09:13:30  sasakis
  親機時、MeasureChannelを使用して、最も混雑していないチャンネルを
  選択するように改造した（ただし条件がまだ決定していない）。

  Revision 1.5  2004/09/15 10:23:40  sasakis
  電波受信強度アイコンの追加と、データサイズ可変化のための改造など。

  Revision 1.4  2004/09/10 08:44:52  sasakis
  ガイドライン準拠処理の追加、エラー関係の見直しなど。

  Revision 1.3  2004/09/08 08:55:35  sasakis
  切断処理を少々強化した。

  Revision 1.2  2004/09/08 01:52:52  sasakis
  エラー・切断関係改良。

  Revision 1.1  2004/09/07 04:59:42  sasakis
  初版登録。

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
  dataShare-Model

  ゲームの体裁にはなっていませんが、実際のゲームアプリケーションに
  必要な（必要そうな）機能を、積極的に実装したサンプルプログラムです。
  具体的には、
  
  1) ロビー画面
  2) 親機の選択画面
  3) 処理落ち対策（同期）
  4) エラー表示と復帰
  
  などが実装されています。
  操作は、

  十字キー : メニューの選択
  A ボタン : 決定
  B ボタン : キャンセル

  という通常の動作の他に、デバッグ用操作として
  
  L ボタン : 負荷カウントを増加
  R ボタン : 負荷カウントを減少
  
  があります。
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
#include "unicode.h"
#include "ball.h"

/* このデモで使用する GGID */
#define WH_GGID           SDK_MAKEGGID_SYSTEM(0x13)
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
static OSOwnerInfo sMyInfo;
static MyGameInfo sGameInfo;

static WMKeySet sKeySet;

static GraphData sGraphData[1 + WM_NUM_MAX_CHILD][GRAPH_TOTAL_FRAME];
static PRScreen sInfoScreen ATTRIBUTE_ALIGN(32);
static PRScreen sDebugScreen ATTRIBUTE_ALIGN(32);
static GXOamAttr oamBak[BALL_PLAYER_MAX];
static int sBeaconCount = 0;
static int sNeedWait = 0;
static BOOL sGraphEnabled = 0;

static WMBssDesc sBssDesc[ITEM_NUM_MAX];
static Window sRoleMenuWindow;
static Window sSelectChannelWindow;
static Window sSelectParentWindow;
static Window sLobbyWindow;
static Window sErrorWindow;
static Window sBusyWindow;
static Window sOptionWindow;
static Window sWaitWindow;

extern const unsigned char wlicon_image[];
extern const unsigned short wlicon_palette[];

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
    u16     nickName[10 + 1];
    char    sjisNickName[10 * 2 + 1];
    u8      entryCount;
    int     i;

    (void)STD_CopyLString(sjisNickName, "unknown", sizeof(sjisNickName));
    entryCount = 0;

    // userGameInfoが想定しているものか確認
    if (bssdesc->gameInfoLength != 0 && bssdesc->gameInfo.userGameInfoLength == sizeof(MyGameInfo))
    {
        const MyGameInfo *pMyGameInfo = (MyGameInfo *) bssdesc->gameInfo.userGameInfo;
        u32 nickNameLength = pMyGameInfo->nickNameLength;

        // 通信で得られるデータは信用せずに、必ず妥当な値かを確認せねばならない
        if (nickNameLength <= 10)
        {
            MI_CpuCopy8(pMyGameInfo->nickName, nickName, nickNameLength * 2);
            nickName[nickNameLength] = 0;
            MI_CpuFill8(sjisNickName, 0, sizeof(sjisNickName));
            ExUTF16_LEtoSJIS_BE((u8*)sjisNickName, (u16*)nickName, (u16)nickNameLength);

            entryCount = pMyGameInfo->entryCount;
        }
    }

    // すでに発見した親機かを調べる
    for (i = 0; i < sSelectParentWindow.itemnum; ++i)
    {
        if (WM_IsBssidEqual(sBssDesc[i].bssid, bssdesc->bssid))
        {
            break;
        }
    }

    // 親機選択用のメニュー項目を作成。
    (void)OS_SNPrintf(buf, ITEM_LENGTH_MAX,
                      "[%d]channel%d %s %d/%d", i + 1, bssdesc->channel, sjisNickName, entryCount,
                      WH_CHILD_MAX);

    if (i < sSelectParentWindow.itemnum)
    {
        // すでにメニューに登録済みなので上書き
        setItemToWindow(&sSelectParentWindow, buf, i);
    }
    else
    {
        // 新しく発見した親機なので情報を記憶してメニューに追加
        sBssDesc[sSelectParentWindow.itemnum] = *bssdesc;
        addItemToWindow(&sSelectParentWindow, buf);

        WH_PrintBssDesc(bssdesc);
    }
}

static void gameinfoCallback(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
}

static void updateGameInfo(BOOL isEntry)
{
    ++sGameInfo.periodicalCount;
    sGameInfo.entryCount = (u8)(MATH_CountPopulation(WH_GetBitmap()) - 1);

    (void)WM_SetGameInfo(gameinfoCallback, (u16 *)&sGameInfo, sizeof(sGameInfo), WH_GGID, sTgid,
                         (u8)(isEntry ? WM_ATTR_FLAG_ENTRY : 0));
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
                sTgid++;

                // エントリー受付状態にuserGameInfoを更新する
                updateGameInfo(TRUE);

                // キャッシュされた親機情報を破棄
                MI_CpuClear8(sBssDesc, sizeof(sBssDesc));

                // 手動で選択したチャンネルを使用して接続開始。
                (void)WH_ParentConnect(WH_CONNECTMODE_DS_PARENT, sTgid, sForcedChannel);
            }
            InitBall(NULL, 13, 2, 4);
            changeSysMode(SYSMODE_LOBBY);
            break;

        case 1:
            {
                // 親機を検索しに行く。
                static const u8 ANY_PARENT[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
                enum
                { ALL_CHANNEL = 0 };

                initWindow(&sSelectParentWindow);
                setupWindow(&sSelectParentWindow, 16, 16, WIN_FLAG_SELECTABLE, 8 * 2, 8, 16);
                (void)WH_StartScan(scanCallback, ANY_PARENT, ALL_CHANNEL);
                InitBall(NULL, 13, 2, 4);
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

    updateGameInfo(TRUE);

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

            // 親機に接続する際に、自分のニックネームを SSID のユーザ領域に設定して通知する
            WH_SetSsid(sMyInfo.nickName, (u32)(sMyInfo.nickNameLength * 2));

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
            for (i = 0; i < WH_CHILD_MAX + 1; ++i)
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
            for (; i < WM_NUM_MAX_CHILD + 1; ++i)
            {
                ShareData *sd;

                sd = (ShareData *) & (sRecvBuf[i * sizeof(ShareData)]);

                sd->level = 0;
                sd->data = 0;
                sRecvFlag[i] = FALSE;
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
            int     i;
            for (i = 0; i < BALL_PLAYER_MAX; ++i)
            {
                if (sRecvFlag[i])
                {
                    ShareData *sd = getRecvData(i);
                    InputBallKey((int)i, (int)sd->key);
                }
            }

            // 共有できたデータを使用して、ボールの状態を更新します。
            //   WH_StepDS に失敗したフレームで実行してしまうと
            //   親子のゲーム状態の同期が崩れますので注意してください。
            UpdateBalls(WH_GetCurrentAid());

            // 表示データの準備
            for (i = 0; i < BALL_PLAYER_MAX; ++i)
            {
                G2_SetOBJAttr(&oamBak[i],
                              shared->ball[i].x, shared->ball[i].y, 0,
                              GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE,
                              GX_OAM_SHAPE_8x8, GX_OAM_COLOR_16,
                              shared->ball[i].chr, shared->ball[i].plt, 0);
            }
        }

        DC_FlushRange(sInfoScreen.screen, sizeof(u16) * PR_SCREEN_SIZE);
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */

        // DC_WaitWriteBufferEmpty();
        GX_LoadBG2Scr(sInfoScreen.screen, 0, sizeof(u16) * PR_SCREEN_SIZE);
        DC_FlushRange(sDebugScreen.screen, sizeof(u16) * PR_SCREEN_SIZE);
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */

        // DC_WaitWriteBufferEmpty();
        GXS_LoadBG2Scr(sDebugScreen.screen, 0, sizeof(u16) * PR_SCREEN_SIZE);
        DC_FlushRange(&oamBak, sizeof(oamBak));
        if ((sSysMode == SYSMODE_PARENT) || (sSysMode == SYSMODE_CHILD))
        {
            GX_LoadOAM(&oamBak, 0, sizeof(oamBak));
        }
        else
        {
            // 全OAMを非表示
            MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
        }

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
                          GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000);

    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG2);
    G2S_SetBG2Priority(0);
    G2S_BG2Mosaic(FALSE);

    GXS_LoadBG2Char(d_CharData, 0, sizeof(d_CharData));
    GXS_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    GX_SetBankForOBJ(GX_VRAM_OBJ_16_G);
    GX_LoadOBJ(d_CharData, 0, sizeof(d_CharData));
    GX_LoadOBJPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    GX_SetVisiblePlane(GX_GetVisiblePlane() | GX_PLANEMASK_OBJ);

    initAllocSystem();

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    sInfoScreen.scroll = FALSE;
    sDebugScreen.scroll = TRUE;

    // オーナー情報の取り込み
    OS_GetOwnerInfo(&sMyInfo);
    sGameInfo.nickNameLength = (u8)sMyInfo.nickNameLength;
    MI_CpuCopy8(sMyInfo.nickName, sGameInfo.nickName, MATH_MIN(sMyInfo.nickNameLength * 2, sizeof(sGameInfo.nickName)));

    // 無線初期化
    // wh 内部で呼ばれる WM_SetIndCallback で指定するコールバックを設定
    WH_SetIndCallback(indicateCallback);
    (void)WH_Initialize();

    // WH 初期設定
    WH_SetGgid(WH_GGID);
    WH_SetDebugOutput(TraceWH);

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

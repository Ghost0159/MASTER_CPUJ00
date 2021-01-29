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
  dataShare-Model �̕ύX���}�[�W

  Revision 1.10  2006/10/26 06:57:18  kitase_hirotake
  WH_SetIndCallback �ŃR�[���o�b�N��ݒ肷��悤�ɕύX

  Revision 1.9  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 11:07:19  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.7  2005/06/27 11:10:13  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.6  2005/04/11 04:42:58  seiki_masashi
  �R�����g�̏C��

  Revision 1.5  2005/04/07 04:35:05  seiki_masashi
  �`�����l���I���̕s����C��
  �A���C�������g�֌W�̕s����C��

  Revision 1.4  2005/04/06 06:41:33  seiki_masashi
  MI_CpuClear32 �� 16 �ɕύX

  Revision 1.3  2005/04/05 00:00:11  seiki_masashi
  warning �΍�

  Revision 1.2  2005/04/01 12:29:46  seiki_masashi
  WEP Key �̌v�Z�� HMAC-SHA-1 ���g�p����悤�ɕύX

  Revision 1.1  2005/03/25 07:41:37  seiki_masashi
  dataShare-Model �� WEP Key �̓��I�ݒ�������Awep-1 �f���Ƃ��ĐV�K�o�^

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
  wep-1 �f��

  dataShare-Model �f���ɐڑ����� WEP �g�p��ǉ������f���ł��B
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

/* ���̃f���Ŏg�p���� GGID */
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
            // bssid �������e�͓���G���g���Ƃ��ď��O����B
            return;
        }
    }

    WH_PrintBssDesc(bssdesc);

    // ����ۑ����Ă����B
    sBssDesc[sSelectParentWindow.itemnum] = *bssdesc;

    // �e�@�I��p�̃��j���[���ڂ��쐬�B
    (void)snprintf(buf,
                   ITEM_LENGTH_MAX,
                   "[%d]channel%d", sSelectParentWindow.itemnum + 1, bssdesc->channel);
    addItemToWindow(&sSelectParentWindow, buf);
    WH_PrintBssDesc(bssdesc);
}

static void forceSpinWait(void)
{
    // OS_SpinWait ���g�p���āA����������Ԃ�C�ӂɍ��o��
    // ���߂̏����B

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
                // �d�g�g�p������œK�ȃ`�����l�����擾���Đڑ�����B
                (void)WH_StartMeasureChannel();

            }
            else
            {
                // �蓮�őI�������`�����l�����g�p���Đڑ��J�n�B
                static u32 wepSeed;
                wepSeed = MATH_Rand32(&sRand, 0);
                // WEP �v�Z�p�� Seed �l�� UserGameInfo �ɖ��ߍ���
                WH_SetUserGameInfo((u16 *)&wepSeed, 4);
                sTgid++;
                (void)WH_ParentConnect(WH_CONNECTMODE_DS_PARENT, sTgid, sForcedChannel);
            }
            changeSysMode(SYSMODE_LOBBY);
            break;

        case 1:
            {
                // �e�@���������ɍs���B
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
            // �I�v�V������ʂցB
            changeSysMode(SYSMODE_OPTION);
            break;

        default:
            break;
        }
    }
}

static void ModeOption(void)
{
    // �I�v�V������ʂ̏����B

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
            // �L�����Z�����ꂽ�B
            changeSysMode(SYSMODE_SELECT_ROLE);
            return;
        }

        if (sOptionWindow.selected == 0)
        {
            // �`�����l���I����ʂցB
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
    // ���r�[��ʂ̏����B

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
    // �q�@�̃��r�[�ҋ@���̏����B
    // �e�@���J�n�̍��}�𑗂��Ă���܂ŁA���̂܂ܑ҂B

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
    static u16 channelList[15];        // Auto select + �ő�14ch
    // �`�����l���I����ʁB
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

        // �����I����ʂɖ߂�B
        changeSysMode(SYSMODE_SELECT_ROLE);
    }
}

static void ModeSelectParent(void)
{
    // �e�@�����X�g�\�����I������B
    // ���̏�Ԃ̎��� WH ���e�@���X�L�������Ă���A�X�L�������ɐV����
    // �������ꂽ�e�@���������j���[�ɒǉ��A���f�����B

    if (WH_GetSystemState() == WH_SYSSTATE_CONNECT_FAIL)
    {
        // WM_StartConnect()�Ɏ��s�����ꍇ�ɂ�WM�����̃X�e�[�g���s���ɂȂ��Ă����
        // ��xWM_Reset��IDLE�X�e�[�g�Ƀ��Z�b�g����K�v������܂��B
        WH_Reset();
        return;
    }

    if (sSysModeStep == 0)
    {
        openWindow(&sSelectParentWindow);
    }

    // �e�@������ʂ����[�U��������
    if ((sSelectParentWindow.state == WIN_STATE_CLOSED))
    {
        if (WH_GetSystemState() == WH_SYSSTATE_SCANNING)
        {
            // �e�@�X�L�������ł���Έ�U�X�L�������I������
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

            // �X�L�������łȂ��A�Ȃ����e�@�����[�U���I�����Ă���΃f�[�^�V�F�A�����O�J�n
            (void)WH_ChildConnect(WH_CONNECTMODE_DS_CHILD,
                                  &(sBssDesc[sSelectParentWindow.selected]));
            changeSysMode(SYSMODE_LOBBYWAIT);
        }
    }
}

static void ModeError(void)
{
    // �G���[��ԁB
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
       ���̃T���v���̒��ɂ́A�����iVBlankIntr�֐��j�� StepDataSharing ��
       ���Ă�����̂�����܂��B
       ���̃T���v���ł��A�ȑO�͂����� StepDataSharing �����Ă��܂����B

       �����Ȃ��Ă���i�����j���R�́A���肵���ʐM�̂��߂� WM_StepDataSharing
       �i�y�� WM_GetKeySet�j�́u���̃t���[���� MP �ʐM�J�n���O�Ɂv
       �Ă΂Ȃ���΂Ȃ�Ȃ�����ł��B
       SDK ���f�t�H���g�� MP �ʐM�̏������s�� V �J�E���g�͎q�@�� 240�A
       �e�@�� 260 �ł��̂ŁAV Blank �J�n����ɃZ�b�g����̂���Ԗ�肪
       ���Ȃ��Ȃ�܂��i���̃h�L�������g�������ꂽ���_�ł͖������ł����A
       MP �ʐM�J�n�� V �J�E���g�͏��������ɐݒ�\�ɂȂ�\��ł��B

       30fps �̃Q�[���Ȃǂł́A�ʏ�� StepDS �̌Ăяo���񐔂� 2 �t���[����
       1 ��ɐ��䂷��R�[�h���K�v�ł��B
       ���̏ꍇ�AWM_ERRCODE_NO_DATASET ���Ԃ������i�������������������ꍇ�j
       �ɂ́A2 �t���[���҂̂ł͂Ȃ��A1 �t���[�������҂悤�ɂ��Ă��������B
       �������Ȃ���΁A�e�q�� 1 �t���[����������Ă���ꍇ�ɏC���ł��܂���B
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
    // �d�g��M���x�A�C�R���̕\���B
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
    // �d�g���M�A�C�R���̕\���B
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
            // �ڑ����s�͉��x�����g���C����B
            if (sSysMode == SYSMODE_LOBBYWAIT && retry < MAX_RETRY)
            {
                changeSysMode(SYSMODE_SELECT_PARENT);
                sSysModeStep = 1; // window ���J���Ȃ��悤��
                retry++;
                break;
            }
            // ���g���C���s�̂Ƃ��͂��̂܂܃G���[��Ԃ�

        case WH_SYSSTATE_ERROR:
            // �G���[�������� WH ��Ԃ��D��B
            changeSysMode(SYSMODE_ERROR);
            break;

        case WH_SYSSTATE_MEASURECHANNEL:
            {
                u16     channel = WH_GetMeasureChannel();
                static u32 wepSeed;
                wepSeed = MATH_Rand32(&sRand, 0);
                // WEP �v�Z�p�� Seed �l�� UserGameInfo �ɖ��ߍ���
                WH_SetUserGameInfo((u16 *)&wepSeed, 4);
                sTgid++;
                (void)WH_ParentConnect(WH_CONNECTMODE_DS_PARENT, sTgid, channel);
            }
            break;

        default:
            break;
        }

        PR_ClearScreen(&sInfoScreen);

        // ���׎����B
        forceSpinWait();

        switch (sSysMode)
        {
        case SYSMODE_SELECT_ROLE:
            // �����i�e�@�E�q�@�j�I����ʁB
            ModeSelectRole();
            retry = 0;
            break;

        case SYSMODE_SELECT_CHANNEL:
            // �`�����l���I����ʁB
            ModeSelectChannel();
            break;

        case SYSMODE_LOBBY:
            // ���r�[��ʁB
            ModeLobby();
            break;

        case SYSMODE_LOBBYWAIT:
            // ���r�[�ҋ@��ʁB
            ModeLobbyWait();
            break;

        case SYSMODE_OPTION:
            // �I�v�V������ʁB
            ModeOption();
            break;

        case SYSMODE_SCAN_PARENT:
        case SYSMODE_SELECT_PARENT:
            // �e�@�I����ʁB
            ModeSelectParent();
            break;

        case SYSMODE_ERROR:
            // �G���[�����񍐉�ʁB
            ModeError();
            break;

        case SYSMODE_PARENT:
            // �e�@���[�h����ʁB
            ModeParent();
            break;

        case SYSMODE_CHILD:
            // �q�@���[�h����ʁB
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
            // ... ���ۂ̃Q�[���ł͂����ŃL�����N�^�[�Ȃǂ̍X�V������
            // �s�����ɂȂ�܂��B
        }

        DC_FlushRange(sInfoScreen.screen, sizeof(u16) * PR_SCREEN_SIZE);
        /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */

        // DC_WaitWriteBufferEmpty();
        GX_LoadBG2Scr(sInfoScreen.screen, 0, sizeof(u16) * PR_SCREEN_SIZE);
        DC_FlushRange(sDebugScreen.screen, sizeof(u16) * PR_SCREEN_SIZE);
        /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */

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

    // ����������
    // wh �����ŌĂ΂�� WM_SetIndCallback �Ŏw�肷��R�[���o�b�N��ݒ�
    WH_SetIndCallback(indicateCallback);
    (void)WH_Initialize();

    // WH �����ݒ�
    WH_SetGgid(WH_GGID);
    WH_SetDebugOutput(TraceWH);

    // WEP Key Generator �ݒ�
    WH_SetParentWEPKeyGenerator(ParentWEPKeyGenerator);
    WH_SetChildWEPKeyGenerator(ChildWEPKeyGenerator);

    // WEP Key �̎�p�̗��������@�̏�����
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

    // IPL �ł� A �{�^�������΍�
    updateKeys();

    // initCharacter();
    changeSysMode(SYSMODE_SELECT_ROLE);
    mainLoop();
}

// WEP Key �쐬�p�̋��ʌ��i�e�q�ŋ��ʂ̌����g�p����j
// �A�v���P�[�V�������ƂɌŗL�̂��̂Ƃ��邱��
// ASCII ������ł���K�v�͂Ȃ��A�C�ӂ̒����̃o�C�i���f�[�^�ł悢
static char *sSecretKey = "this is a secret key for HMAC";
static u32 sSecretKeyLength = 30;      // ���� 20 �o�C�g�ȏ�ł��邱�Ƃ��]�܂���

// �e�@�p�� WEP Key �̌v�Z���[�`��
u16 ParentWEPKeyGenerator(u16 *wepkey, const WMParentParam *parentParam)
{
    u16     data[20 / sizeof(u16)];
    u16     tmpWep[20 / sizeof(u16)];
    u8      macAddress[WM_SIZE_MACADDR];

    OS_GetMacAddress(macAddress);
    MI_CpuClear16(data, sizeof(data));

    // �O������ WH_ParentConnect �֐��̒��O�� WH_SetUserGameInfo �֐���
    // UserGameInfo �ɐݒ肵�Ă����� Seed �l�� WEP Key �ɔ��f������
    MI_CpuCopy16(parentParam->userGameInfo, data, 4);
    // �����A���S���Y�����قȂ�A�v���P�[�V�����Ŏg���܂킹��悤��
    // GGID �� WEP Key �ɔ��f������
    MI_CpuCopy16(&parentParam->ggid, data + 2, sizeof(u32));
    *(u16 *)(data + 4) = parentParam->tgid;
    // �e�@���قȂ�� WEP Key ���قȂ�悤�� MAC Address �� WEP Key �ɔ��f������
    MI_CpuCopy8(macAddress, (u8 *)(data + 5), WM_SIZE_MACADDR);

    // HMAC-SHA-1 �Ō��t�n�b�V���l�����A128bit �ɐ؂�l�߂� WEP Key �Ƃ���B
    // HMAC �Ŏg�p���錮�̓A�v���P�[�V�����ŗL�̂��̂Ƃ��邱�ƁB
    // (HMAC �Ɋւ��Ă� RFC2104 �Q��)
    MATH_CalcHMACSHA1(tmpWep, data, sizeof(data), sSecretKey, sSecretKeyLength);
    MI_CpuCopy8(tmpWep, wepkey, 16);
    OS_TPrintf("wepkey: %04x%04x %04x%04x %04x%04x %04x%04x\n", wepkey[0], wepkey[1],
               wepkey[2], wepkey[3], wepkey[4], wepkey[5], wepkey[6], wepkey[7]);

    return WM_WEPMODE_128BIT;
}

// �q�@�p�� WEP Key �̌v�Z���[�`��
u16 ChildWEPKeyGenerator(u16 *wepkey, const WMBssDesc *bssDesc)
{
    u16     data[20 / sizeof(u16)];
    u16     tmpWep[20 / sizeof(u16)];

    MI_CpuClear16(data, sizeof(data));

    // UserGameInfo ���ɖ��ߍ��܂�Ă��� Seed �l���g�p���� WEP Key ���v�Z
    MI_CpuCopy16(bssDesc->gameInfo.userGameInfo, data, 4);
    // GGID �� TGID ��e�@�� MAC Address  ���l���ɓ����
    MI_CpuCopy16(&bssDesc->gameInfo.ggid, data + 2, sizeof(u32));
    *(u16 *)(data + 4) = bssDesc->gameInfo.tgid;
    MI_CpuCopy8(bssDesc->bssid, (u8 *)(data + 5), WM_SIZE_MACADDR);

    // HMAC-SHA-1 �Ō��t�n�b�V���l�����A128bit �ɐ؂�l�߂� WEP Key �Ƃ���B
    // HMAC �Ŏg�p���錮�̓A�v���P�[�V�����ŗL�̂��̂Ƃ��邱�ƁB
    // (HMAC �Ɋւ��Ă� RFC2104 �Q��)
    MATH_CalcHMACSHA1(tmpWep, data, sizeof(data), sSecretKey, sSecretKeyLength);
    MI_CpuCopy8(tmpWep, wepkey, 16);
    OS_TPrintf("wepkey: %04x%04x %04x%04x %04x%04x %04x%04x\n", wepkey[0], wepkey[1],
               wepkey[2], wepkey[3], wepkey[4], wepkey[5], wepkey[6], wepkey[7]);

    return WM_WEPMODE_128BIT;
}

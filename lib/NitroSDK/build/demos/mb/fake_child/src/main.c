/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - fake_child
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.17  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.16  2006/03/13 06:39:22  yosizaki
  add test of MB_SetUserVolatData..

  Revision 1.15  2006/01/20 02:05:13  seiki_masashi
  WM_MPParameter �֐��̒ǉ��Ɋւ���f�o�b�O�\���̕ύX

  Revision 1.14  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.13  2005/11/21 10:49:28  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.12  2005/09/02 08:44:03  yosizaki
  fix symbol name.

  Revision 1.11  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.10  2005/02/18 11:06:37  yosizaki
  fix around hidden warnings.

  Revision 1.9  2005/01/11 07:32:58  takano_makoto
  fix copyright header.

  Revision 1.8  2005/01/07 06:00:34  takano_makoto
  �R�����g�C��

  Revision 1.7  2005/01/07 02:54:11  takano_makoto
  WH_SYSSTATE_CONNECT_FAIL���Ƀ��Z�b�g���Ă���Đڑ�����悤�ɏC���B�f�o�b�O�o�͂�ǉ��B

  Revision 1.6  2004/11/26 00:23:04  takano_makoto
  �V�[�P���X�̏C���A�R�����g�ǉ�

  Revision 1.4  2004/11/25 07:39:13  takano_makoto
  ���o�[�W������IPL�p��OwnerInfo�̑�����C��

  Revision 1.2  2004/11/24 13:09:10  takano_makoto
  �G���[�����A�L�����Z��������ǉ�

  Revision 1.1  2004/11/22 13:01:22  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include "common.h"
#include "disp.h"
#include "gmain.h"
#include "wh.h"


#define     DEBUG_PRINT

/*
 * ���̃T���v���ł�demos/mb/multiboot-Model�ɑ΂��Ďq�@�Ƃ��ăG���g���[���A
 * ����DS�_�E�����[�h�v���C�q�@�ƍ��݂��ĒʐM�ɎQ�����܂��B
 * MB_FakeScanParent�֐���GGID�����ɂ���āA�ΏۂƂ���A�v���P�[�V������
 * �ݒ肵�܂��B
 */


enum
{
    MODE_TITLE,
    MODE_START_SCAN,
    MODE_SCANNING,
    MODE_ENTRY,
    MODE_RECONNECT,
    MODE_GMAIN,
    MODE_ERROR,
    MODE_CANCEL
};

typedef struct
{
    MBUserInfo info;
    u16     valid;
    u8      maxPlayerNum;              // �ő�v���C���[��
    u8      nowPlayerNum;              // ���݂̃v���C���[��
    u8      macAddr[6];                // MAC�A�h���X
}
ParentInfo;

#define WH_GGID (0x003FFF00 | (0x21))

static void PrintError(MBFakeScanErrorCallback *arg);

static void ModeTitle(void);
static void ModeStartScan(void);
static void ModeScanning(void);
static void ModeEntry(void);
static void ModeError(void);
static void ModeReconnect(void);
static void ModeGMain(void);
static void ModeCancel(void);

static void NotifyScanParent(u16 type, void *arg);
static void MBStateCallback(u32 status, void *arg);


// �e�@��񃊃X�g
static ParentInfo parentInfo[MB_GAME_INFO_RECV_LIST_NUM];
static ParentInfo parentInfoBuf[MB_GAME_INFO_RECV_LIST_NUM];

static WMBssDesc sParentBssDesc;       // �G���g���[�����e�@��BssDesc

static u8 *mbfBuf = NULL;
static u8 *wmBuf = NULL;


static s32 gFrame;                     // �t���[���J�E���^

static u16 sMode;

enum
{
    WSTATE_STOP,                       // ��~���
    WSTATE_INITIALIZE_BUSY,            // ���C�����X��������
    WSTATE_IDLE,                       // �A�C�h�����
    WSTATE_ERROR,                      // �t�F�C�N�q�@�J�n�O�܂��͏I����̃G���[
    WSTATE_FAKE_SCAN,                  // �X�L�������
    WSTATE_FAKE_END_SCAN,              // �X�L�����I��
    WSTATE_FAKE_ENTRY,                 // �e�@�փG���g���[��
    WSTATE_FAKE_BOOT_READY,            // DS�_�E�����[�h�v���C�ւ̃G���g���[����
    WSTATE_FAKE_BOOT_END_BUSY,         // DS�_�E�����[�h�����I����
    WSTATE_FAKE_BOOT_END,
    WSTATE_FAKE_CANCEL_BUSY,           // DS�_�E�����[�h�L�����Z����
    WSTATE_FAKE_ERROR                  // �G���[���
};
static u16 sWirelessState = WSTATE_STOP;

static u16 sConnectIndex;              // �ڑ�����e�@�C���f�b�N�X

/******************************************************************************/
/* function */

#ifdef DEBUG_PRINT
static void ChangeWirelessState(u16 wstate)
{
    static const char *WSTATE_NAME[] = {
        "WSTATE_STOP",
        "WSTATE_INITIALIZE_BUSY",
        "WSTATE_IDLE",
        "WSTATE_ERROR",
        "WSTATE_FAKE_SCAN",
        "WSTATE_FAKE_END_SCAN",
        "WSTATE_FAKE_ENTRY",
        "WSTATE_FAKE_BOOT_READY",
        "WSTATE_FAKE_BOOT_END_BUSY",
        "WSTATE_FAKE_BOOT_END",
        "WSTATE_FAKE_CANCEL_BUSY",
        "WSTATE_FAKE_ERROR"
    };

    if (sWirelessState != wstate)
    {
        OS_TPrintf("change sWirelessState %s -> %s\n", WSTATE_NAME[sWirelessState],
                   WSTATE_NAME[wstate]);
    }
#else
static inline void ChangeWirelessState(u16 wstate)
{
#endif
    sWirelessState = wstate;
}

/* �u�u�����N���荞�ݏ��� */
static void VBlankIntr(void)
{
    //---- ���荞�݃`�F�b�N�t���O
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}


/* ���C�� */
void NitroMain()
{
    // �V�X�e���̏�����
    CommonInit();
    // ��ʏ�����
    DispInit();
    // �q�[�v�̏�����
    InitAllocateSystem();

    // ���荞�ݗL��
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    // �f�[�^�V�F�A�����O�p�̃o�b�t�@���N���A
    GInitDataShare();

    // �^�C�g����ʂ�
    sMode = MODE_TITLE;

    // LCD�\���J�n
    GX_DispOn();
    GXS_DispOn();

    // �f�o�b�O������o��
    OS_TPrintf("MB fake child: Simple DataSharing demo started.\n");
    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    ReadKey();

    /* ���C�����[�v */
    for (gFrame = 0; TRUE; gFrame++)
    {
        // �L�[���͏��擾
        ReadKey();

        // �X�N���[���N���A
        BgClear();

        switch (sMode)
        {
        case MODE_TITLE:
            // �^�C�g����ʂ̕\��
            ModeTitle();
            break;
        case MODE_START_SCAN:
            // �X�L�����J�n
            ModeStartScan();
            break;
        case MODE_SCANNING:
            // MB�e�@���X�L������
            ModeScanning();
            break;
        case MODE_ENTRY:
            // MB�e�@�փG���g���[
            ModeEntry();
            break;
        case MODE_RECONNECT:
            // MB�e�@�֍Đڑ�
            ModeReconnect();
            break;
        case MODE_GMAIN:
            // �Q�[�����C��
            ModeGMain();
            break;
        case MODE_ERROR:
            // �G���[����
            ModeError();
            break;
        case MODE_CANCEL:
            // �L�����Z����
            ModeCancel();
            break;
        }

        OS_WaitVBlankIntr();
    }
}



/* ----------------------------------------------------------------------
  Name:        ModeTitle
  
  Description: �^�C�g�����
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeTitle(void)
{
    BgSetMessage(PLTT_WHITE, " Press Button A to start   ");

    ChangeWirelessState(WSTATE_STOP);

    // A�{�^���������ꂽ�疳����ON�ɂ���MB�e�@�̌������[�h�ֈڍs����B
    if (IS_PAD_TRIGGER(PAD_BUTTON_A))
    {
        sMode = MODE_START_SCAN;
    }
}

/* ----------------------------------------------------------------------
  Name:        ModeStartScan
  
  Description: �e�@�̃X�L�����J�n����
  
  Arguments:   None.
               
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeStartScan(void)
{
    BgSetMessage(PLTT_WHITE, "Now working....         ");

    switch (sWirelessState)
    {
        //--------------------------
        // ���C�����X�ʐM�̊J�n
    case WSTATE_STOP:
        if (!WH_Initialize())
        {
            ChangeWirelessState(WSTATE_ERROR);
            sMode = MODE_ERROR;
            return;
        }
        ChangeWirelessState(WSTATE_INITIALIZE_BUSY);
        break;

        //--------------------------
        // ���C�����X�̏�������
    case WSTATE_INITIALIZE_BUSY:
        if (WH_GetSystemState() == WH_SYSSTATE_BUSY)
        {
            BgSetMessage(PLTT_WHITE, "Now working....         ");
            return;
        }
        if (WH_GetSystemState() != WH_SYSSTATE_IDLE)
        {
            ChangeWirelessState(WSTATE_ERROR);
            sMode = MODE_ERROR;
            return;
        }
        ChangeWirelessState(WSTATE_IDLE);
        // don't break;

        //--------------------------
        // �e�@�̃X�L�������J�n
    case WSTATE_IDLE:
        {
            // ���[�U�[����ݒ�
            MBUserInfo userInfo;

#if ( SDK_NVRAM_FORMAT >= 100 )
            OSOwnerInfo info;

            OS_GetOwnerInfo(&info);
            userInfo.favoriteColor = info.favoriteColor;
            userInfo.nameLength = (u8)info.nickNameLength;
            MI_CpuCopy8(info.nickName, userInfo.name, (u32)(userInfo.nameLength * 2));
#else  // ��IPL�p
            NVRAMConfig *info = (NVRAMConfig *)OS_GetSystemWork()->nvramUserInfo;

#define DEFAULT_FAVORIT_COLOR   1
            userInfo.favoriteColor = DEFAULT_FAVORIT_COLOR;
            userInfo.nameLength = (u8)info->ncd.owner.nickname.length;
            MI_CpuCopy8(info->ncd.owner.nickname.name, userInfo.name,
                        NVRAM_CONFIG_NICKNAME_LENGTH * 2);
#endif
            // �ʏ��MB�q�@�ł�playerNo��0�ƂȂ邪�A�l��ݒ肷�邱�ƂŐe�@�փp�����[�^�Ƃ��ēn�����Ƃ��\�B
            userInfo.playerNo = 7;

            // �}���`�u�[�g�G���g���[�p�̃��[�N�o�b�t�@���m��
            mbfBuf = (u8 *)OS_Alloc(MB_FakeGetWorkSize());
            MB_FakeInit(mbfBuf, &userInfo);
            // ��ԑJ�ڒʒm�R�[���o�b�N�֐���ݒ�
            MB_FakeSetCStateCallback(MBStateCallback);

            // �e�@���X�g�̃N���A
            MI_CpuClear8(parentInfo, sizeof(parentInfo));

            // ���C�����X��Ԃ��X�L��������
            ChangeWirelessState(WSTATE_FAKE_SCAN);

            // �X�L��������e�@��GGID�ƁA�ʒm�p�R�[���o�b�N�֐���ݒ肵�A�X�L�������J�n
            MB_FakeStartScanParent(NotifyScanParent, WH_GGID);

            // �X�L�������[�h�֑J��
            sMode = MODE_SCANNING;
        }
        break;
    default:
        OS_TPanic("illegal state %d", sWirelessState);
        break;
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeScanning
  
  Description: �e�@�̃X�L������
  
  Arguments:   None.
               
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeScanning(void)
{
    enum
    { STR_X = 4, STR_Y = 4, NUM_STR_X = 24 };
    static u16 cursol_idx = 0;

    u16     i;

    // �G���[���������Ă�����I��
    if (sWirelessState == WSTATE_FAKE_ERROR)
    {
        sMode = MODE_ERROR;
        return;
    }

    // �`�撆�Ɋ��荞�݂ɂ���ĕύX����Ȃ��悤�e�@����
    // ��U�e���|�����o�b�t�@�փR�s�[���Ă���g�p����B
    {
        OSIntrMode enabled = OS_DisableInterrupts();
        MI_CpuCopy8(parentInfo, parentInfoBuf, sizeof(parentInfo));
        (void)OS_RestoreInterrupts(enabled);
    }

    // �J�[�\���̈ړ�
    if (IS_PAD_TRIGGER(PAD_KEY_UP))
    {
        cursol_idx = (u16)((cursol_idx - 1) & 0xF);
    }
    else if (IS_PAD_TRIGGER(PAD_KEY_DOWN))
    {
        cursol_idx = (u16)((cursol_idx + 1) & 0xF);
    }

    // �e�@���̕\��
    BgPutString(10, 1, PLTT_RED, "Select Parent");

    BgPutString(STR_X - 2, STR_Y + cursol_idx, PLTT_WHITE, ">");
    for (i = 0; i < MB_GAME_INFO_RECV_LIST_NUM; i++)
    {
        if (parentInfoBuf[i].valid)
        {
            BgPrintStr(STR_X, STR_Y + i, PLTT_GREEN, "%02x:%02x:%02x:%02x:%02x:%02x",
                       parentInfoBuf[i].macAddr[0], parentInfoBuf[i].macAddr[1],
                       parentInfoBuf[i].macAddr[2], parentInfoBuf[i].macAddr[3],
                       parentInfoBuf[i].macAddr[4], parentInfoBuf[i].macAddr[5]);
            BgPrintStr(NUM_STR_X, STR_Y + i, PLTT_GREEN, "%d/%d",
                       parentInfoBuf[i].nowPlayerNum, parentInfoBuf[i].maxPlayerNum);
        }
        else
        {
            BgPutString(STR_X, STR_Y + i, PLTT_WHITE, "Searching...");
        }
    }

    // �ʐM���L�����Z��
    if (IS_PAD_TRIGGER(PAD_BUTTON_B))
    {
        ChangeWirelessState(WSTATE_FAKE_CANCEL_BUSY);
        sMode = MODE_CANCEL;
        MB_FakeEnd();
        return;
    }

    // �ڑ�����e�@��I��
    if (IS_PAD_TRIGGER(PAD_BUTTON_A))
    {
        if (!parentInfoBuf[cursol_idx].valid)
        {
            return;
        }

        // �ڑ�����e�@�̏����擾
        sConnectIndex = cursol_idx;
        if (!MB_FakeReadParentBssDesc
            (sConnectIndex, &sParentBssDesc, WH_PARENT_MAX_SIZE, WH_CHILD_MAX_SIZE, FALSE, FALSE))
        {
            // �e�@��񂪗L���łȂ�������X�L�������s
            return;
        }
        sMode = MODE_ENTRY;
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeEntry
  
  Description: �e�@�ւ̃G���g���[
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeEntry(void)
{
    BgSetMessage(PLTT_WHITE, "Now Entrying...        ");

    switch (sWirelessState)
    {
        //--------------------------
        // �X�L�������̏ꍇ
    case WSTATE_FAKE_SCAN:
        {
            // �X�L�������I������
            MB_FakeEndScan();
            ChangeWirelessState(WSTATE_FAKE_END_SCAN);
        }
        break;

        //--------------------------
        // �X�L�����I����
    case WSTATE_FAKE_END_SCAN:
        break;

        //--------------------------
        // ���C�����X�ʐM�̊J�n
    case WSTATE_IDLE:
        {
            if (!MB_FakeEntryToParent(sConnectIndex))
            {
                // �w�肳�ꂽIndex���L���Ȃ��̂łȂ���΃G���[
                OS_TPrintf("ERR: Illegal Parent index\n");
                ChangeWirelessState(WSTATE_FAKE_ERROR);
                sMode = MODE_ERROR;
                return;
            }
            ChangeWirelessState(WSTATE_FAKE_ENTRY);
        }
        break;

        //--------------------------
        // �G���g���[��
    case WSTATE_FAKE_ENTRY:
        // �G���g���[���������Ă��Ȃ����B�{�^���ŃL�����Z��
        if (IS_PAD_TRIGGER(PAD_BUTTON_B))
        {
            ChangeWirelessState(WSTATE_FAKE_CANCEL_BUSY);
            sMode = MODE_CANCEL;
            MB_FakeEnd();
            return;
        }
        break;

        //--------------------------
        // �G���g���[��������
    case WSTATE_FAKE_BOOT_READY:
        {
            // �G���g���[�����̏I������
            ChangeWirelessState(WSTATE_FAKE_BOOT_END_BUSY);
            MB_FakeEnd();
        }
        break;

        //--------------------------
        // �����҂�
    case WSTATE_FAKE_BOOT_END_BUSY:
        break;

        //--------------------------
        // �t�F�C�N�q�@�̏�������
    case WSTATE_FAKE_BOOT_END:
        {
            // �e�@�Ƃ��߂������킹��tgid���C���N�������g
            sParentBssDesc.gameInfo.tgid++;
            // �Đڑ��������J�n
            ChangeWirelessState(WSTATE_IDLE);
            sMode = MODE_RECONNECT;
        }
        break;

        //--------------------------
        // �G���[�I��
    case WSTATE_FAKE_ERROR:
        sMode = MODE_ERROR;
        break;

    default:
        OS_TPanic("illegal state %d\n", sWirelessState);
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeReconnect
  
  Description: �^�C�g�����
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeReconnect(void)
{
    BgSetMessage(PLTT_WHITE, "Connecting to Parent...        ");

    switch (WH_GetSystemState())
    {
    case WH_SYSSTATE_CONNECT_FAIL:
        {
            // WM_StartConnect()�Ɏ��s�����ꍇ�ɂ�WM�����̃X�e�[�g���s���ɂȂ��Ă����
            // ��xWM_Reset��IDLE�X�e�[�g�Ƀ��Z�b�g����K�v������܂��B
            WH_Reset();
        }
        break;
    case WH_SYSSTATE_IDLE:
        {
            (void)WH_ChildConnect(WH_CONNECTMODE_DS_CHILD, &sParentBssDesc);
        }
        break;
    case WH_SYSSTATE_ERROR:
        ChangeWirelessState(WSTATE_ERROR);
        sMode = MODE_ERROR;
        break;
    case WH_SYSSTATE_BUSY:
        break;
    case WH_SYSSTATE_CONNECTED:
    case WH_SYSSTATE_KEYSHARING:
    case WH_SYSSTATE_DATASHARING:
        sMode = MODE_GMAIN;
        break;
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeGMain
  
  Description: �^�C�g�����
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeGMain(void)
{
    if (WH_GetSystemState() == WH_SYSSTATE_ERROR)
    {
        ChangeWirelessState(WSTATE_ERROR);
        sMode = MODE_ERROR;
        return;
    }
    GStepDataShare(gFrame);
    GMain();
}


/* ----------------------------------------------------------------------
  Name:        ModeCancel
  
  Description: �L�����Z�������
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeCancel(void)
{
    switch (sWirelessState)
    {
        //-------------------------
        // �A�C�h����Ԃ֑J�ڂ����烏�C�����X�I��
    case WSTATE_IDLE:
        if (WH_GetSystemState() == WH_SYSSTATE_IDLE)
        {
            (void)WH_End();
            return;
        }

        if (WH_GetSystemState() != WH_SYSSTATE_STOP)
        {
            return;
        }
        ChangeWirelessState(WSTATE_STOP);
        sMode = MODE_TITLE;
        break;
        //--------------------------
        // �L�����Z����
    case WSTATE_FAKE_CANCEL_BUSY:
        break;

        //--------------------------
        // �G���[����
    case WSTATE_ERROR:
        ChangeWirelessState(WSTATE_IDLE);
        (void)WH_Finalize();
        break;

        //--------------------------
        // �I������
    default:
        ChangeWirelessState(WSTATE_IDLE);
        (void)WH_Finalize();
        break;
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeError
  
  Description: �G���[���
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeError(void)
{
    BgPutString(4, 10, PLTT_RED, "========== ERROR ==========");

    if (IS_PAD_TRIGGER(PAD_BUTTON_B) || IS_PAD_TRIGGER(PAD_BUTTON_A))
    {
        if (sWirelessState == WSTATE_FAKE_ERROR)
        {
            ChangeWirelessState(WSTATE_FAKE_CANCEL_BUSY);
            MB_FakeEnd();
        }
        sMode = MODE_CANCEL;
    }
}


/* ----------------------------------------------------------------------
  Name:        NotifyScanParent
  
  Description: �e�@�X�L�����󋵒ʒm�R�[���o�b�N
  
  Arguments:   state    �X�L�������.
                        MB_FAKESCAN_PARENT_FOUND, MB_FAKESCAN_PARENT_LOST,
                        MB_FAKESCAN_API_ERROR, MB_FAKESCAN_END_SCAN
                        �̂S��ނ̃��b�Z�[�W���ʒm�����B
                
               arg      MB_FAKESCAN_PARENT_FOUND, MB_FAKESCAN_PARENT_LOST�̏ꍇ�ɂ�
                        �ΏۂƂȂ�e�@�̏�񂪎擾�ł���B
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void NotifyScanParent(u16 type, void *arg)
{
    static const char *MB_FAKESCAN_CALLBACK_TYPE_NAME[] = {
        "MB_FAKESCAN_PARENT_FOUND",
        "MB_FAKESCAN_PARENT_LOST",
        "MB_FAKESCAN_API_ERROR",
        "MB_FAKESCAN_END_SCAN",
        "MB_FAKESCAN_PARENT_BEACON",
        "MB_FAKESCAN_CONNECTED",
        "MB_FAKESCAN_ENTRY",
        "MB_FAKESCAN_DISCONNECTED",
        "MB_FAKESCAN_SUCCESS"
    };

    OS_TPrintf("> %s\n", MB_FAKESCAN_CALLBACK_TYPE_NAME[type]);

    switch (type)
    {
    case MB_FAKESCAN_PARENT_BEACON:
        {
            MBFakeScanCallback *cb = (MBFakeScanCallback *)arg;
            WMBssDesc *bssdesc = cb->bssDesc;
            const u8 *volat_data = (const u8 *)MB_GetUserVolatData(&bssdesc->gameInfo);
            OS_TPrintf(" find MB-beacon MAC=(%04x%08x)\n",
                       *(u16 *)(&bssdesc->bssid[4]), *(u32 *)(&bssdesc->bssid[0]));
            if (volat_data)
            {
                OS_TPrintf("                VOLAT=(%02x %02x %02x %02x %02x %02x %02x %02x)\n",
                           volat_data[0], volat_data[1], volat_data[2], volat_data[3],
                           volat_data[4], volat_data[5], volat_data[6], volat_data[7]);
            }
        }
        break;
    case MB_FAKESCAN_PARENT_FOUND:
        {
            MBFakeScanCallback *cb = (MBFakeScanCallback *)arg;
            WMBssDesc *bssdesc = cb->bssDesc;
            MBGameInfo *gameInfo = cb->gameInfo;

            OS_TPrintf(" find parent %04x%08x %d\n", *(u16 *)(&bssdesc->bssid[4]),
                       *(u32 *)(&bssdesc->bssid[0]), cb->index);
            OS_TPrintf("      parentMaxSize = %d, childMaxSize = %d\n",
                       bssdesc->gameInfo.parentMaxSize, bssdesc->gameInfo.childMaxSize);

            parentInfo[cb->index].valid = 1;
            parentInfo[cb->index].maxPlayerNum = gameInfo->fixed.maxPlayerNum;
            parentInfo[cb->index].nowPlayerNum = gameInfo->volat.nowPlayerNum;
            MI_CpuCopy8(&gameInfo->fixed.parent, &parentInfo[cb->index].info, sizeof(MBUserInfo));
            WM_CopyBssid(bssdesc->bssid, parentInfo[cb->index].macAddr);
        }
        break;
    case MB_FAKESCAN_PARENT_LOST:
        {
            MBFakeScanCallback *cb = (MBFakeScanCallback *)arg;

            OS_TPrintf(" lost parent %d\n", cb->index);

            parentInfo[cb->index].valid = 0;
        }
        break;
    case MB_FAKESCAN_API_ERROR:
        {
            PrintError((MBFakeScanErrorCallback *)arg);
            OS_TPrintf("ERR : API Error occured\n");
            ChangeWirelessState(WSTATE_FAKE_ERROR);
        }
        break;
    case MB_FAKESCAN_END_SCAN:
        {
            ChangeWirelessState(WSTATE_IDLE);
        }
        break;
    default:
        break;
    }
}


/* ----------------------------------------------------------------------
  Name:        MBStateCallback
  
  Description: �t�F�C�N�q�@�̏�ԑJ�ڒʒm�R�[���o�b�N�ł��B
               MB_FakeEntryToParent()�֐����R�[�����ꂽ��́A
               �q�@�u�[�g�������b�Z�[�W����������܂ł̃V�[�P���X��
               �����I�Ɏ��s����܂��̂ŁA���ʂɏ���������K�v�͂���܂���B
               ���ׂẴ}���`�u�[�g��������������ƁAMB_COMM_CSTATE_BOOT_READY
               �R�[���o�b�N���Ԃ��Ă��܂��B
  
  Arguments:   status   MB�q�@�Ƃ��Ă̌��݂̏��
               arg      �R�[���o�b�N����
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void MBStateCallback(u32 status, void *arg)
{
#pragma unused( arg )
    switch (status)
    {
    case MB_COMM_CSTATE_INIT_COMPLETE: // �e�@�ւ̐ڑ������ʒm
    case MB_COMM_CSTATE_CONNECT:      // �e�@�ւ̐ڑ������ʒm
    case MB_COMM_CSTATE_REQ_ENABLE:   // �e�@�Ƃ�MP�m���ʒm
    case MB_COMM_CSTATE_DLINFO_ACCEPTED:       // �e�@�ւ̃G���g���[�����ʒm
    case MB_COMM_CSTATE_RECV_PROCEED: // �e�@����̃f�[�^���M�J�n�ʒm
    case MB_COMM_CSTATE_RECV_COMPLETE: // �e�@�̃f�[�^���M�����ʒm
    case MB_COMM_CSTATE_BOOTREQ_ACCEPTED:      // �e�@����̃u�[�g���N�G�X�g��M�ʒm
        // ���ʂȏ����͕K�v�Ȃ�
        break;

    case MB_COMM_CSTATE_BOOT_READY:
        // �e�@����̐ؒf��������MB�̏����������������̒ʒm
        {
            ChangeWirelessState(WSTATE_FAKE_BOOT_READY);
        }
        break;

    case MB_COMM_CSTATE_CANCELED:
        // �r���Őe�@�ւ̃G���g���[�������L�����Z�������ꍇ�̒ʒm
        break;

    case MB_COMM_CSTATE_CONNECT_FAILED:        // �e�@�ւ̐ڑ����s�ʒm
    case MB_COMM_CSTATE_DISCONNECTED_BY_PARENT:        // �e�@����̐ؒf�ʒm
    case MB_COMM_CSTATE_REQ_REFUSED:  // �e�@�Ƀ��N�G�X�g�����ۂ��ꂽ�ꍇ�̒ʒm
    case MB_COMM_CSTATE_MEMBER_FULL:  // �e�@�̃G���g���[�q�@�����I�[�o�[���Ă����ꍇ�̒ʒm
    case MB_COMM_CSTATE_ERROR:        // �r���ŒʐM��Ԉُ킪���������ꍇ�̏���
        {
            OS_TPrintf("ERR : MB Error occured reason 0x%x\n", status);
            ChangeWirelessState(WSTATE_FAKE_ERROR);
        }
        break;

    case MB_COMM_CSTATE_FAKE_END:
        // MB�֓n�������[�N�������
        OS_Free(mbfBuf);
        mbfBuf = NULL;

        if (sWirelessState == WSTATE_FAKE_BOOT_END_BUSY)
        {
            ChangeWirelessState(WSTATE_FAKE_BOOT_END);
        }
        else
        {
            ChangeWirelessState(WSTATE_IDLE);
        }
    }
}



/* ----------------------------------------------------------------------
  Name:        PrintError
  
  Description: WM��API�G���[�̓��e���v�����g�o�͂��܂��B�B
  
  Arguments:   arg MB_FakeStartScanParent�̃G���[�R�[���o�b�N
  
  Returns:     None.
  ---------------------------------------------------------------------- */
static void PrintError(MBFakeScanErrorCallback *arg)
{
#pragma unused( arg )
    static const char *APIID_NAME[] = {
        "WM_APIID_INITIALIZE",
        "WM_APIID_RESET",
        "WM_APIID_END",
        "WM_APIID_ENABLE",
        "WM_APIID_DISABLE",
        "WM_APIID_POWER_ON",
        "WM_APIID_POWER_OFF",
        "WM_APIID_SET_P_PARAM",
        "WM_APIID_START_PARENT",
        "WM_APIID_END_PARENT",
        "WM_APIID_START_SCAN",
        "WM_APIID_END_SCAN",
        "WM_APIID_START_CONNECT",
        "WM_APIID_DISCONNECT",
        "WM_APIID_START_MP",
        "WM_APIID_SET_MP_DATA",
        "WM_APIID_END_MP",
        "WM_APIID_START_DCF",
        "WM_APIID_SET_DCF_DATA",
        "WM_APIID_END_DCF",
        "WM_APIID_SET_WEPKEY",
        "WM_APIID_START_KS",
        "WM_APIID_END_KS",
        "WM_APIID_GET_KEYSET",
        "WM_APIID_SET_GAMEINFO",
        "WM_APIID_SET_BEACON_IND",
        "WM_APIID_START_TESTMODE",
        "WM_APIID_STOP_TESTMODE",
        "WM_APIID_VALARM_MP",
        "WM_APIID_SET_LIFETIME",
        "WM_APIID_MEASURE_CHANNEL",
        "WM_APIID_INIT_W_COUNTER",
        "WM_APIID_GET_W_COUNTER",
        "WM_APIID_SET_ENTRY",
        "WM_APIID_AUTO_DEAUTH",
        "WM_APIID_SET_MP_PARAMETER",
        "WM_APIID_SET_BEACON_PERIOD",
        "WM_APIID_AUTO_DISCONNECT",
        "WM_APIID_START_SCAN_EX",
        "WM_APIID_KICK_MP_PARENT",
        "WM_APIID_KICK_MP_CHILD",
        "WM_APIID_KICK_MP_RESUME",
        "WM_APIID_ASYNC_KIND_MAX",
        "WM_APIID_INDICATION",
        "WM_APIID_PORT_SEND",
        "WM_APIID_PORT_RECV",
        "WM_APIID_READ_STATUS",
        "WM_APIID_UNKNOWN",
    };

    static const char *ERRCODE_NAME[] = {
        "WM_ERRCODE_SUCCESS",
        "WM_ERRCODE_FAILED",
        "WM_ERRCODE_OPERATING",
        "WM_ERRCODE_ILLEGAL_STATE",
        "WM_ERRCODE_WM_DISABLE",
        "WM_ERRCODE_NO_DATASET",
        "WM_ERRCODE_INVALID_PARAM",
        "WM_ERRCODE_NO_CHILD",
        "WM_ERRCODE_FIFO_ERROR",
        "WM_ERRCODE_TIMEOUT",
        "WM_ERRCODE_SEND_QUEUE_FULL",
        "WM_ERRCODE_NO_ENTRY",
        "WM_ERRCODE_OVER_MAX_ENTRY",
        "WM_ERRCODE_INVALID_POLLBITMAP",
        "WM_ERRCODE_NO_DATA",
        "WM_ERRCODE_SEND_FAILED",
        "WM_ERRCODE_DCF_TEST",
        "WM_ERRCODE_WL_INVALID_PARAM",
        "WM_ERRCODE_WL_LENGTH_ERR",
        "WM_ERRCODE_FLASH_ERROR"
    };

    OS_TPrintf("ERR: %s %s\n", APIID_NAME[arg->apiid], ERRCODE_NAME[arg->errcode]);
}

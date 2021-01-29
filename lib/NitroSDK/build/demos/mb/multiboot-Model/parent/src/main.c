/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-Model
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.33  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.32  2006/03/13 06:39:01  yosizaki
  add test of MB_SetUserVolatData..

  Revision 1.31  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.30  2005/11/21 10:51:51  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.29  2005/04/11 08:50:25  yosizaki
  change to use WM_GetNextTgid().

  Revision 1.28  2005/03/08 10:29:04  yosizaki
  change to initialize TGID at random.

  Revision 1.27  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.26  2005/02/04 11:02:55  yasu
  ����N�̏C��

  Revision 1.25  2005/02/02 09:05:14  seiki_masashi
  sChildInfo �̓Y���������C��

  Revision 1.24  2004/12/28 09:34:44  pair2
  WH_Initialize() �̏d���Ăяo���Ɋւ���t���[�����[�N�̏C��.

  Revision 1.23  2004/12/03 08:12:58  takano_makoto
  MPB_USING_MB_EX -> MBP_USING_MB_EX �ɏC���A MBP_USING_PREVIOUS_DOWNLOAD�X�C�b�`�ɑΉ�

  Revision 1.22  2004/11/12 09:01:20  yosizaki
  change mb_parent to /wireless_shared/mbp

  Revision 1.21  2004/11/10 01:12:28  takano_makoto
  �q�@��14�䂵���\������Ă��Ȃ������̂��C��

  Revision 1.20  2004/11/02 18:08:07  takano_makoto
  fix comments.

  Revision 1.19  2004/10/28 01:31:09  takano_makoto
  wmhigh��wireless_shared��wh�ֈڍs

  Revision 1.18  2004/10/27 02:06:41  takano_makoto
  �ċN�����tgid���C���N�������g����悤�ɏC��

  Revision 1.17  2004/10/25 06:29:56  takano_makoto
  �ő�ڑ��q�@�̐ݒ��ǉ�

  Revision 1.16  2004/10/21 00:42:42  yosizaki
  add SHARED_WH switch.

  Revision 1.15  2004/10/18 11:44:06  yosizaki
  change to use MB_StartParentEx/MB_EndEx (switch)

  Revision 1.14  2004/10/08 01:27:10  takano_makoto
  �\���q�@�̔ԍ��Y���̕s��C��

  Revision 1.13  2004/10/05 09:45:29  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.12  2004/10/05 07:49:36  takano_makoto
  �q�@��Accept��mb_parent.c�̃R�[���o�b�N���ŏ�������悤�ɏC��

  Revision 1.11  2004/10/05 03:58:49  takano_makoto
  �R�����g�̔��C��

  Revision 1.10  2004/10/05 02:15:05  takano_makoto
  �G���g���[��Ԃő҂��āA���ׂĂ̎q�@�ň�ĂɃ_�E�����[�h���J�n����悤�ɕύX�B

  Revision 1.9  2004/09/30 08:28:26  yosizaki
  add SDK_MAKEGGID_SYSTEM macro.

  Revision 1.8  2004/09/27 08:35:49  takano_makoto
  �e�@�ċN�����Ƀ`�����l���ԍ����ω����Ă��܂��Ă����̂��C��

  Revision 1.7  2004/09/21 07:26:06  takano_makoto
  �R�����g�ǉ�

  Revision 1.4  2004/09/17 10:36:11  takano_makoto
  BeaconPeriod��W���֐�srand�o�R�Œ������Ă����̂�ύX

  Revision 1.3  2004/09/17 06:11:59  takano_makoto
  ���[�U����MBP_GetChildInfo()�o�R�Ŏ擾����悤�ɕύX

  Revision 1.2  2004/09/17 02:20:23  sato_masaki
  TODO: �ɑ΂���񓚃R�����g��ǉ��B

  Revision 1.1  2004/09/16 14:06:41  takano_makoto
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/mb.h>

#include "mbp.h"
#include "common.h"
#include "disp.h"
#include "gmain.h"
#include "wh.h"

/*
 * �}���`�u�[�g��Đڑ�����A�v���P�[�V�����̃T���v��.
 *
 * MB���C�u�����̃T���v���̓}���`�u�[�g�@�\���g�����߁A
 * �����ʐM��(�������邢�͗L��)�ǂ����̊J���@�ނ𕡐��K�v�Ƃ��܂��B
 * $NitroSDK/bin/ARM9-TS/Release/ �f�B���N�g���ɂ���
 * mb_child.bin �v���O�������ŏI���@�ɂ�����}���`�u�[�g�q�@�Ɠ�����
 * �@�\��񋟂���T���v���ɂȂ��Ă��܂��̂ŁA
 * ���̃o�C�i�����T���v���v���O�����Ɠ������@�ő��̋@�ނɓǂݍ��݁A
 * �ꏏ�Ɏ��s���Ă��������B
 * 
 */

/******************************************************************************/

static void WaitPressButton(void);
static void GetChannelMain(void);
static BOOL ConnectMain(u16 tgid);
static void PrintChildState(void);
static BOOL JudgeConnectableChild(WMStartParentCallback *cb);
static void OnPreSendMBVolat(u32 ggid);


//============================================================================
//  �萔��`
//============================================================================

/* ���̃f���Ŏg�p���� GGID */
#define WH_GGID                 SDK_MAKEGGID_SYSTEM(0x21)


/* ���̃f�����_�E�����[�h������v���O������� */
const MBGameRegistry mbGameList = {
    "/child.srl",                      // �q�@�o�C�i���R�[�h
    (u16 *)L"DataShareDemo",           // �Q�[����
    (u16 *)L"DataSharing demo",        // �Q�[�����e����
    "/data/icon.char",                 // �A�C�R���L�����N�^�f�[�^
    "/data/icon.plt",                  // �A�C�R���p���b�g�f�[�^
    WH_GGID,                           // GGID
    MBP_CHILD_MAX + 1,                 // �ő�v���C�l���A�e�@�̐����܂߂��l��
};



//============================================================================
//   �ϐ���`
//============================================================================

static s32 gFrame;                     // �t���[���J�E���^

//-----------------------
// �ʐM�o�H�̕ێ��p
//-----------------------
static u16 sChannel = 0;
static const MBPChildInfo *sChildInfo[MBP_CHILD_MAX];


//============================================================================
//   �֐���`
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C�����[�`��

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    u16     tgid = 0;

    // ��ʁAOS�̏�����
    CommonInit();
    // ��ʏ�����
    DispInit();
    // �q�[�v�̏�����
    InitAllocateSystem();

    // WH �ɏ���ݒ�
    WH_SetGgid(WH_GGID);

    // ���荞�ݗL��
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    DispOn();

    /*
     * DS�_�E�����[�h�v���C�̎q�@�́A�e�@��TGID�������ꍇ�A�e�@�̏����X�V���܂���B
     * ���̂���DS�_�E�����[�h�v���C�̐e�@�ł́A�N������TGID��
     * �Ȃ�ׂ��قȂ�l�ɂ΂������悤�ɔz������K�v������܂��B
     * ����͒ʏ�A WM_GetNextTgid() �֐����g�p�����
     * RTC �Ɋ�Â��Ă�����x��ӂȒl���擾���邱�Ƃ��ł��܂��B
     */
    tgid = WM_GetNextTgid();

    while (TRUE)
    {
        // A�{�^�������҂�
        WaitPressButton();

        // �g���t�B�b�N�̏��Ȃ��`�����l���̌�������
        GetChannelMain();

        /*
         * tgid�͐e�@���N���̓x�Ɋ�{�I�ɂ͑O��ƈႤ�l��ݒ肵�܂��B
         * �������}���`�u�[�g�q�@�Ƃ̍Đڑ����ɂ͓���tgid�ŋN�����Ȃ����
         * �ăX�L�������s�Ȃ�Ȃ���ΐڑ��ł��Ȃ��Ȃ邽�ߒ��ӂ��K�v�ł��B
         * ������x�X�L�������s�Ȃ��Ă���Đڑ�������ꍇ�ɂ�tgid��ۑ����Ă���
         * �K�v�͂���܂���B
         */
        // �}���`�u�[�g�z�M����
        if (ConnectMain(++tgid))
        {
            // �}���`�u�[�g�q�@�̋N���ɐ���
            break;
        }
    }

    //--------------
    // �}���`�u�[�g��͎q�@�����Z�b�g����ʐM����U�ؒf����܂��B
    // �܂�����e�@����xMB_End()����ʐM���I������K�v������܂��B
    // �e�q����x���S�ɐؒf���ꂽ��Ԃňꂩ��ʐM���m�����Ă��������B
    // 
    // �܂����̎��q�@��aid���V���b�t������邽�߁A�����K�v�������
    // �}���`�u�[�g�O��aid��MAC�A�h���X�̑g�ݍ��킹��ۑ����Ă����A
    // �Đڑ����ɐV����aid�Ƃ̌��т����s�Ȃ��Ă��������B
    //--------------


    // �f�[�^�V�F�A�����O�ʐM�p�Ƀo�b�t�@��ݒ�
    GInitDataShare();

#if !defined(MBP_USING_MB_EX)
    (void)WH_Initialize();
#endif

    // �ڑ��q�@�̔���p�֐���ݒ�
    WH_SetJudgeAcceptFunc(JudgeConnectableChild);

    /* ���C�����[�`�� */
    for (gFrame = 0; TRUE; gFrame++)
    {
        OS_WaitVBlankIntr();

        ReadKey();

        BgClear();

        switch (WH_GetSystemState())
        {
        case WH_SYSSTATE_IDLE:
            /* ----------------
             * �q�@���ōăX�L�����Ȃ��ɓ����e�@�ɍĐڑ����������ꍇ�ɂ�
             * �q�@����tgid�y��channel�����킹��K�v������܂��B
             * ���̃f���ł́A�}���`�u�[�g���Ɠ���channel�ƃ}���`�u�[�g����tgid+1��
             * �e�q�Ƃ��Ɏg�p���邱�ƂŁA�ăX�L�����Ȃ��ł��ڑ��ł���悤�ɂ��Ă��܂��B
             * 
             * MAC�A�h���X���w�肵�čăX�L����������ꍇ�ɂ͓���tgid, channel�łȂ��Ă�
             * ��肠��܂���B
             * ---------------- */
            (void)WH_ParentConnect(WH_CONNECTMODE_DS_PARENT, (u16)(tgid + 1), sChannel);
            break;

        case WH_SYSSTATE_CONNECTED:
        case WH_SYSSTATE_KEYSHARING:
        case WH_SYSSTATE_DATASHARING:
            {
                BgPutString(8, 1, 0x2, "Parent mode");
                GStepDataShare(gFrame);
                GMain();
            }
            break;
        }
    }
}



/*---------------------------------------------------------------------------*
  Name:         WaitPressButton

  Description:  A�{�^�����������̂����[�v�ő҂֐��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WaitPressButton(void)
{
    while (TRUE)
    {
        OS_WaitVBlankIntr();
        ReadKey();
        BgClear();
        BgSetMessage(PLTT_WHITE, " Push A Button to start   ");
        if (IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            return;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         GetChannelMain

  Description:  �g�p����`�����l����d�g�g�p���𒲂ׂĂ܂��߂ɋ��߂�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void GetChannelMain(void)
{

    /*-----------------------------------------------*
     * �`�����l���̓d�g�g�p����������ƒ��ׂ���ŁA
     * ��Ԏg�p���̒Ⴂ�`�����l����I�����܂��B
     * WM_MeasureChannel()�����s����ɂ�IDLE��ԂɂȂ�K�v������
     * �}���`�u�[�g��Ԃł�IDLE��ԂɎ~�܂鎖���Ȃ��̂Ŏ��s�ł��܂���B
     * ��UWM_Initialize���Ă�œd�g�g�p���𒲂ׂĂ���WM_End�ŏI�����A
     * ���炽�߂�MB_Init�����s����B
     *-----------------------------------------------*/
    (void)WH_Initialize();

    while (TRUE)
    {
        ReadKey();
        BgClear();
        BgSetMessage(PLTT_YELLOW, " Search Channel ");

        switch (WH_GetSystemState())
        {
            //-----------------------------------------
            // ����������
        case WH_SYSSTATE_IDLE:
            (void)WH_StartMeasureChannel();
            break;
            //-----------------------------------------
            // �`�����l����������
        case WH_SYSSTATE_MEASURECHANNEL:
            {
                sChannel = WH_GetMeasureChannel();
#if !defined(MBP_USING_MB_EX)
                (void)WH_End();
#else
                /* IDLE ��Ԃ��ێ������܂܃}���`�u�[�g������ */
                return;
#endif
            }
            break;
            //-----------------------------------------
            // WM�I��
        case WH_SYSSTATE_STOP:
            /* WM_End������������}���`�u�[�g������ */
            return;
            //-----------------------------------------
            // �r�W�[��
        case WH_SYSSTATE_BUSY:
            break;
            //-----------------------------------------
            // �G���[����
        case WH_SYSSTATE_ERROR:
            (void)WH_Reset();
            break;
            //-----------------------------------------
        default:
            OS_Panic("Illegal State\n");
        }
        OS_WaitVBlankIntr();           // V�u�����N�����I���҂�
    }
}


/*---------------------------------------------------------------------------*
  Name:         ConnectMain

  Description:  �}���`�u�[�g�Őڑ�����B

  Arguments:    tgid        �e�@�Ƃ��ċN������ꍇ��tgid���w�肵�܂�.

  Returns:      �q�@�ւ̓]���ɐ��������ꍇ�ɂ� TRUE,
                ���s������L�����Z�����ꂽ�ꍇ�ɂ�  FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL ConnectMain(u16 tgid)
{
    MBP_Init(mbGameList.ggid, tgid);

    while (TRUE)
    {
        ReadKey();

        BgClear();

        BgSetMessage(PLTT_YELLOW, " MB Parent ");

        switch (MBP_GetState())
        {
            //-----------------------------------------
            // �A�C�h�����
        case MBP_STATE_IDLE:
            {
                MBP_Start(&mbGameList, sChannel);
                /* ���[�U��`�f�[�^�̑��M�e�X�g */
                MB_SetSendVolatCallback(OnPreSendMBVolat, MB_SEND_VOLAT_CALLBACK_TIMMING_BEFORE);
            }
            break;

            //-----------------------------------------
            // �q�@����̃G���g���[��t��
        case MBP_STATE_ENTRY:
            {
                BgSetMessage(PLTT_WHITE, " Now Accepting            ");

                if (IS_PAD_TRIGGER(PAD_BUTTON_B))
                {
                    // B�{�^���Ń}���`�u�[�g�L�����Z��
                    MBP_Cancel();
                    break;
                }

                // �G���g���[���̎q�@�����ł����݂���ΊJ�n�\�Ƃ���
                if (MBP_GetChildBmp(MBP_BMPTYPE_ENTRY) ||
                    MBP_GetChildBmp(MBP_BMPTYPE_DOWNLOADING) ||
                    MBP_GetChildBmp(MBP_BMPTYPE_BOOTABLE))
                {
                    BgSetMessage(PLTT_WHITE, " Push START Button to start   ");

                    if (IS_PAD_TRIGGER(PAD_BUTTON_START))
                    {
                        // �_�E�����[�h�J�n
                        MBP_StartDownloadAll();
                    }
                }
            }
            break;

            //-----------------------------------------
            // �v���O�����z�M����
        case MBP_STATE_DATASENDING:
            {

                // �S�����_�E�����[�h�������Ă���Ȃ�΃X�^�[�g�\.
                if (MBP_IsBootableAll())
                {
                    // �u�[�g�J�n
                    MBP_StartRebootAll();
                }
            }
            break;

            //-----------------------------------------
            // ���u�[�g����
        case MBP_STATE_REBOOTING:
            {
                BgSetMessage(PLTT_WHITE, " Rebooting now                ");
            }
            break;

            //-----------------------------------------
            // �Đڑ�����
        case MBP_STATE_COMPLETE:
            {
                // �S�������ɐڑ�����������}���`�u�[�g�����͏I����
                // �ʏ�̐e�@�Ƃ��Ė������ċN������B
                BgSetMessage(PLTT_WHITE, " Reconnecting now             ");

                OS_WaitVBlankIntr();
                return TRUE;
            }
            break;

            //-----------------------------------------
            // �G���[����
        case MBP_STATE_ERROR:
            {
                // �ʐM���L�����Z������
                MBP_Cancel();
            }
            break;

            //-----------------------------------------
            // �ʐM�L�����Z��������
        case MBP_STATE_CANCEL:
            // None
            break;

            //-----------------------------------------
            // �ʐM�ُ�I��
        case MBP_STATE_STOP:
#ifdef MBP_USING_MB_EX
            switch (WH_GetSystemState())
            {
            case WH_SYSSTATE_IDLE:
                (void)WH_End();
                break;
            case WH_SYSSTATE_BUSY:
                break;
            case WH_SYSSTATE_STOP:
                return FALSE;
            default:
                OS_Panic("illegal state\n");
            }
#else
            return FALSE;
#endif
        }

        // �q�@��Ԃ�\������
        PrintChildState();

        OS_WaitVBlankIntr();           // V�u�����N�����I���҂�
    }
}


/*---------------------------------------------------------------------------*
  Name:         PrintChildState

  Description:  �q�@������ʂɕ\������

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintChildState(void)
{
    static const char *STATE_NAME[] = {
        "NONE       ",
        "CONNECTING ",
        "REQUEST    ",
        "ENTRY      ",
        "DOWNLOADING",
        "BOOTABLE   ",
        "BOOTING    ",
    };
    enum
    {
        STATE_DISP_X = 2,
        INFO_DISP_X = 15,
        BASE_DISP_Y = 2
    };

    u16     i;

    /* �q�@���X�g�̕\�� */
    for (i = 1; i <= MBP_CHILD_MAX; i++)
    {
        const MBPChildInfo *childInfo;
        MBPChildState childState = MBP_GetChildState(i);
        const u8 *macAddr;

        SDK_ASSERT(childState < MBP_CHILDSTATE_NUM);

        // ��ԕ\��
        BgPutString(STATE_DISP_X, i + BASE_DISP_Y, PLTT_WHITE, STATE_NAME[childState]);

        // ���[�U�[���\��
        childInfo = MBP_GetChildInfo(i);
        macAddr = MBP_GetChildMacAddress(i);

        if (macAddr != NULL)
        {
            BgPrintStr(INFO_DISP_X, i + BASE_DISP_Y, PLTT_WHITE,
                       "%02x%02x%02x%02x%02x%02x",
                       macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         JudgeConnectableChild

  Description:  �Đڑ����ɐڑ��\�Ȏq�@���ǂ����𔻒肷��֐�

  Arguments:    cb      �ڑ����Ă����q�@�̏��.

  Returns:      �ڑ����󂯕t����ꍇ�� TRUE.
                �󂯕t���Ȃ��ꍇ�� FALSE.
 *---------------------------------------------------------------------------*/
static BOOL JudgeConnectableChild(WMStartParentCallback *cb)
{
    u16     playerNo;

    /*  cb->aid �̎q�@�̃}���`�u�[�g����aid��MAC�A�h���X���猟�����܂� */
    playerNo = MBP_GetPlayerNo(cb->macAddress);

    OS_TPrintf("MB child(%d) -> DS child(%d)\n", playerNo, cb->aid);

    if (playerNo == 0)
    {
        return FALSE;
    }

    sChildInfo[playerNo - 1] = MBP_GetChildInfo(playerNo);
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         OnPreSendMBVolat

  Description:  �e�@��MB�r�[�R���𑗐M����O�ɒʒm�����R�[���o�b�N�֐�

  Arguments:    ggid    ���M����Q�[������GGID.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void OnPreSendMBVolat(u32 ggid)
{
    /*
     * �����̃Q�[����o�^���X�̃Q�[�����Ƃɐݒ�l��؂�ւ���ꍇ��
     * �ȉ��̂悤�Ɉ����� ggid �ŃQ�[�����𔻒肵�܂�.
     */
    if (ggid == mbGameList.ggid)
    {
        /*
         * ���M�\�ȃ��[�U��`�f�[�^�͍ő� 8 BYTE �ł�.
         * ���̗�ł̓C���N�������^���� 64bit �l�𑗐M���Ă��܂�.
         * ���I�ɍX�V���ꂽ�f�[�^���e���A�q�@���ł͕K��������������
         * ��M�ł���킯�ł͂Ȃ��Ƃ����_�ɒ��ӂ��Ă�������.
         */
        static u64 count ATTRIBUTE_ALIGN(8);
        SDK_COMPILER_ASSERT(sizeof(count) <= MB_USER_VOLAT_DATA_SIZE);
        SDK_COMPILER_ASSERT(MB_USER_VOLAT_DATA_SIZE == 8);
        ++count;
        MB_SetUserVolatData(ggid, (u8 *)&count, sizeof(count));
    }
}

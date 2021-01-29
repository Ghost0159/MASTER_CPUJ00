/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - cloneboot
  File:     parent.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: parent.c,v $
  Revision 1.9  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 10:48:43  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.7  2005/08/04 14:18:48  yosizaki
  fix about ParentMain().

  Revision 1.6  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.5  2005/02/04 11:02:15  yasu
  ����N�̏C��

  Revision 1.4  2005/02/02 09:05:14  seiki_masashi
  sChildInfo �̓Y���������C��

  Revision 1.3  2004/12/03 08:48:36  takano_makoto
  MPB_USING_MB_EX -> MBP_USING_MB_EX �ɏC���A MBP_USING_PREVIOUS_DOWNLOAD�X�C�b�`�ɑΉ�

  Revision 1.2  2004/11/24 11:07:38  yosizaki
  add parent_begin/parent_end.

  Revision 1.1  2004/11/15 06:21:18  yosizaki
  initial update.

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


/******************************************************************************/

static void GetChannelMain(void);
static BOOL ConnectMain(u16 tgid);
static void PrintChildState(void);
static BOOL JudgeConnectableChild(WMStartParentCallback *cb);


//============================================================================
//  �萔��`
//============================================================================

/* ���̃f���Ŏg�p���� GGID */
#define WH_GGID                 SDK_MAKEGGID_SYSTEM(0x22)


/* ���̃f�����_�E�����[�h������v���O������� */
const MBGameRegistry mbGameList = {
    /*
     * �N���[���u�[�g�ł̓v���O�����̃p�X���� NULL ���w�肵�܂�.
     * ����������� MBP ���W���[���� MBP_RegistFile() �ɂ�����d�l��,
     * ���ۂ� MB_RegisterFile() �֗^��������Ƃ��Ă͉��ł��\���܂���.
     */
    NULL,
    (u16 *)L"DataShareDemo",           // �Q�[����
    (u16 *)L"DataSharing demo(cloneboot)",      // �Q�[�����e����
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


/*****************************************************************************/
/* �e�@��p�̈� .parent �Z�N�V�����̒�`�͈͂��J�n���܂� */
#include <nitro/parent_begin.h>
/*****************************************************************************/


/*---------------------------------------------------------------------------*
  Name:         ParentMain

  Description:  �e�@���C�����[�`��

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ParentMain(void)
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

    while (TRUE)
    {
        OS_WaitVBlankIntr();

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
        else
        {
            // WH ���W���[�����I�������ČJ��Ԃ�
            WH_Finalize();
            (void)WH_End();
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
            BgSetMessage(PLTT_WHITE, " Push A Button to start   ");
            if (IS_PAD_TRIGGER(PAD_BUTTON_A))
            {
                BgSetMessage(PLTT_YELLOW, "Check Traffic ratio       ");
                (void)WH_StartMeasureChannel();
            }
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
            OS_WaitVBlankIntr();
            return FALSE;
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


/*****************************************************************************/
/* �e�@��p�̈� .parent �Z�N�V�����̒�`�͈͂��I�����܂� */
#include <nitro/parent_end.h>
/*****************************************************************************/

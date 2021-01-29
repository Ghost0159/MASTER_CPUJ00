/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     wh_download.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_download.c,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"


/*****************************************************************************/
/* constant */

/* �_�E�����[�h�v���O������� (�N���[���u�[�g) */
/* *INDENT-OFF* */
const MBGameRegistry mbGameList =
{
    NULL,
    (u16 *)L"wireless-all",
    (u16 *)L"all of the wireless demo(cloneboot, dataSharing, WFS)",
    "/icon.char", "/icon.plt",
    WH_GGID,
    WH_CHILD_MAX + 1,
};
/* *INDENT-ON* */


/*****************************************************************************/
/* function */

/*****************************************************************************/
/* �e�@��p�̈� .parent �Z�N�V�����̒�`�͈͂��J�n���܂� */
#include <nitro/parent_begin.h>
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         ConnectMain

  Description:  �}���`�u�[�g�Őڑ�����B

  Arguments:    None.

  Returns:      �q�@�ւ̓]���ɐ��������ꍇ�ɂ� TRUE,
                ���s������L�����Z�����ꂽ�ꍇ�ɂ�  FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL ConnectMain(void)
{
    for (;;)
    {
        /* �t���[���҂� */
        (void)WaitNextFrame();

        PrintString(4, 22, PLTT_YELLOW, " MB Parent ");

        switch (MBP_GetState())
        {
            //-----------------------------------------
            // �q�@����̃G���g���[��t��
        case MBP_STATE_ENTRY:
            {
                PrintString(4, 22, PLTT_WHITE, " Now Accepting            ");

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
                    PrintString(4, 22, PLTT_WHITE, " Push START Button to start   ");

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
                PrintString(4, 22, PLTT_WHITE, " Rebooting now                ");
            }
            break;

            //-----------------------------------------
            // �Đڑ�����
        case MBP_STATE_COMPLETE:
            {
                // �S�������ɐڑ�����������}���`�u�[�g�����͏I����
                // �ʏ�̐e�@�Ƃ��Ė������ċN������B
                PrintString(4, 22, PLTT_WHITE, " Reconnecting now             ");

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
            for (i = 1; i < WH_PLAYER_MAX; i++)
            {
                const MBPChildInfo *childInfo;
                MBPChildState childState = MBP_GetChildState(i);
                const u8 *macAddr;
                // ��ԕ\��
                PrintString(STATE_DISP_X, i + BASE_DISP_Y, PLTT_WHITE, STATE_NAME[childState]);
                // ���[�U�[���\��
                childInfo = MBP_GetChildInfo(i);
                macAddr = MBP_GetChildMacAddress(i);
                if (macAddr)
                {
                    PrintString(INFO_DISP_X, i + BASE_DISP_Y, PLTT_WHITE,
                                "%02x%02x%02x%02x%02x%02x",
                                macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4],
                                macAddr[5]);
                }
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ExecuteDownloadServer

  Description:  DS�_�E�����[�h�v���C�e�@����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ExecuteDownloadServer(void)
{
    WH_SetGgid(WH_GGID);

    for (;;)
    {
        /* ���C�����X�̏����� */
        (void)WH_Initialize();
        WaitWHState(WH_SYSSTATE_IDLE);
        while (!IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            (void)WaitNextFrame();
            PrintString(4, 22, PLTT_WHITE, " Press A Button to start   ");
        }
        /* �g���t�B�b�N�̏��Ȃ��`�����l���̌������� */
        SetCurrentChannel(MeasureChannel());

        /* DS �_�E�����[�h�v���C�J�n */
#if !defined(MBP_USING_MB_EX)
        (void)WH_End();
        WaitWHState(WH_SYSSTATE_STOP);
#endif
        MBP_Init(WH_GGID, WM_GetNextTgid());
        MBP_Start(&mbGameList, GetCurrentChannel());

        if (ConnectMain())
        {
            /* �}���`�u�[�g�q�@�̋N���ɐ��� */
            break;
        }
        else
        {
            /* WH ���W���[�����I�������ČJ��Ԃ� */
            WH_Finalize();
            (void)WH_End();
        }
    }

}


/*****************************************************************************/
/* �e�@��p�̈� .parent �Z�N�V�����̒�`�͈͂��I�����܂� */
#include <nitro/parent_end.h>
/*****************************************************************************/

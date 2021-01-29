/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     main.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"

/*
 * �ȉ��̃��C�����X�@�\�����ׂĎ�������A�v���P�[�V�����̃T���v��.
 *
 * �EDS�_�E�����[�h�v���C�̃N���[���u�[�g
 * �E�f�[�^�V�F�A�����O
 * �E���C�����X�o�R�̃t�@�C���V�X�e���g�p
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
/* function */

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C�����[�`��

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /* ���ʂ̃V�X�e������������ */
    InitCommon();

    /* �S�̂̏�ԑJ�� */
    for (;;)
    {
        /* �e�@�Ȃ�DS�_�E�����[�h�v���C���� */
        if (!MB_IsMultiBootChild())
        {
            ExecuteDownloadServer();
        }
        /* ���C�����X�̏������Ɛe�q�ʂ̋N������ */
        if (!MB_IsMultiBootChild())
        {
#if !defined(MBP_USING_MB_EX)
            (void)WH_Initialize();
#endif
            WaitWHState(WH_SYSSTATE_IDLE);
            StartWirelessParent();
        }
        else
        {
            (void)WH_Initialize();
            WaitWHState(WH_SYSSTATE_IDLE);
            StartWirelessChild();
        }
        /* �f�[�^�V�F�A�����O�̃��C������ */
        ExecuteDataSharing();
        (void)WH_End();
        WaitWHState(WH_SYSSTATE_STOP);
    }

    /* ����͂����֋y�т܂��� */
    OS_Terminate();
}

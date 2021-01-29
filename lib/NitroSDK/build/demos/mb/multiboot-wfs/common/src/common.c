/*---------------------------------------------------------------------------*
  Project:  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     common.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.c,v $
  Revision 1.2  2005/11/21 10:52:47  kitase_hirotake
  OS_PrintServer �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.1  2005/06/23 09:05:57  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    "common.h"
#include    "wfs.h"
#include    "wc.h"


/*****************************************************************************/
/* constant */

/* �Q�[����� */
const WMParentParam def_parent_param[1] = {
    {
     NULL, 0, 0,
     GGID_WBT_FS,
     0x0000,                           // temporary ID (����+1�����)
     1,                                // �G���g���[���t���O
     15,                               // �ő�ڑ��q�@��
     0,                                // �}���`�u�[�g�t���O
     0,                                // �L�[�V�F�A�����O
     1,                                // �A�����M�t���O
     200,                              // beacon �Ԋu
     {0, 0, 0, 0},                     // ���[�U�[��
     {0, 0, 0, 0, 0, 0, 0, 0},         // �Q�[����
     1,                                // �ڑ��`�����l�� (�N������ MeasureChannel����)
     WC_PARENT_DATA_SIZE_MAX,          // �e�@���M�f�[�^�T�C�Y
     WC_CHILD_DATA_SIZE_MAX / 16       // �q�@���M�f�[�^�T�C�Y
     }
};


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         StateCallbackForWFS

  Description:  WFS �̃R�[���o�b�N�֐�.
                ��Ԃ� WFS_STATE_READY �ɂȂ������_�ŌĂяo����܂�.
                ���̃R�[���o�b�N�Œʒm���󂯂�, �P�ɔC�ӂ̈ʒu��
                WFS_GetStatus() ���g�p���Ĕ��肷�邱�Ƃ��\�ł�.

  Arguments:    arg       �R�[���o�b�N�Ɏw�肵�����[�U��`����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void StateCallbackForWFS(void *arg)
{
    (void)arg;
    switch (WFS_GetStatus())
    {
    case WFS_STATE_READY:
        OS_TPrintf("WFS ready.\n");
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         AllocatorForWFS

  Description:  WFS �Ɏw�肷�郁�����̓��I�m�ۊ֐�.

  Arguments:    arg       �A���P�[�^�Ɏw�肵�����[�U��`����.
                size      �������m�ۗv���Ȃ�ΕK�v�ȃT�C�Y.
                ptr       NULL �̏ꍇ�̓������m��, �����łȂ���Ή���v��.

  Returns:      ptr �� NULL �Ȃ� size ���̃��������m�ۂ��ă|�C���^��Ԃ�����.
                �����łȂ���� ptr �̃�������������邱��.
                ����̏ꍇ, �Ԃ�l�͒P�ɖ��������.
 *---------------------------------------------------------------------------*/
void   *AllocatorForWFS(void *arg, u32 size, void *ptr)
{
    (void)arg;
    if (!ptr)
        return OS_Alloc(size);
    else
    {
        OS_Free(ptr);
        return NULL;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WCCallbackForWFS

  Description:  WC �̖��������쓮�����Ԓʒm���󂯂�R�[���o�b�N.

  Arguments:    arg     - �ʒm�� WM �֐��̃R�[���o�b�N�|�C���^.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WCCallbackForWFS(WMCallback *arg)
{
    WCStatus wcStatus = WcGetStatus();
    switch (arg->apiid)
    {
    case WM_APIID_START_MP:
        {                              /* MP �X�e�[�g�J�n */
            WMStartMPCallback *cb = (WMStartMPCallback *)arg;
            switch (cb->state)
            {
            case WM_STATECODE_MP_START:
                /*
                 * MP_PARENT �܂��� MP_CHILD �X�e�[�g�ւ̈ڍs��ʒm���邽�߂�
                 * WFS_Start() ���Ăяo���Ă�������.
                 * ���̒ʒm���_�@��, WFS �̓u���b�N�]���̂��߂�
                 * ������ WM_SetMPDataToPort() �֐����Ăяo���n�߂܂�.
                 */
                WFS_Start();
                break;
            }
        }
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         InitFrameLoop

  Description:  �Q�[���t���[�����[�v�̏����������B

  Arguments:    p_key           ����������L�[���\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitFrameLoop(KeyInfo * p_key)
{
    ClearLine();
    ClearString();
    KeyRead(p_key);
}

/*---------------------------------------------------------------------------*
  Name:         WaitForNextFrame

  Description:  ���̕`��t���[���܂őҋ@�B

  Arguments:    p_key           �X�V����L�[���\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WaitForNextFrame(KeyInfo * p_key)
{
    FlushLine();
    OS_WaitVBlankIntr();
    ClearString();
    KeyRead(p_key);
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  �G���[�\����ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeError(void)
{
    KeyInfo key[1];

    InitFrameLoop(key);
    while (WcGetStatus() == MODE_ERROR)
    {
        WaitForNextFrame(key);
        PrintString(5, 10, COLOR_RED, "======= ERROR! =======");
        PrintString(5, 13, COLOR_WHITE, " Fatal error occured.");
        PrintString(5, 14, COLOR_WHITE, "Please reboot program.");
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  �r�W�[��ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeWorking(void)
{
    KeyInfo key[1];

    InitFrameLoop(key);
    while (WcGetStatus() == MODE_BUSY)
    {
        WaitForNextFrame(key);
        PrintString(9, 11, COLOR_WHITE, "Now working...");
        if (key->trg & PAD_BUTTON_START)
        {
            WcEnd();
        }
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

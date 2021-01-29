/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - libraries
  File:     card_request.c

  Copyright 2003-2005,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/

#if defined(SDK_ARM9)
/*****************************************************************************/
/* code for ARM9 only */


#include <nitro.h>

#include "../include/card_common.h"
#include "../include/card_spi.h"


/******************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         CARDi_OnFifoRecv

  Description:  PXI FIFO ���[�h��M�R�[���o�b�N.

  Arguments:    tag        PXI �^�O (��� PXI_FIFO_TAG_FS)
                data       ��M�f�[�^
                err        �G���[�r�b�g (���d�l�ɂ�����)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_OnFifoRecv(PXIFifoTag tag, u32 data, BOOL err)
{
#pragma unused(data)
    if ((tag == PXI_FIFO_TAG_FS) && err)
    {
        CARDiCommon *const p = &cardi_common;
        /* ARM7 ����̉�������M���Ċ�����ʒm���� */
        SDK_ASSERT(data == CARD_REQ_ACK);
        p->flag &= ~CARD_STAT_REQ;
        OS_WakeupThreadDirect(p->cur_th);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_TaskThread

  Description:  �^�X�N�X���b�h�̃��C���֐�.

  Arguments:    arg          �s�g�p

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_TaskThread(void *arg)
{
    CARDiCommon *const p = &cardi_common;
    (void)arg;

    for (;;)
    {
        /* ���̏�����҂� */
        OSIntrMode bak_psr = OS_DisableInterrupts();
        while ((p->flag & CARD_STAT_TASK) == 0)
        {
            OS_SleepThread(NULL);
        }
        (void)OS_RestoreInterrupts(bak_psr);
        (*p->task_func) (p);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_Request

  Description:  ARM9 ���� ARM7 �փ��N�G�X�g���M��, �������u���b�L���O.
                ���ʂ� CARD_RESULT_SUCCESS �łȂ��Ȃ�w��񐔂܂ł͍Ď��s����.
                (����̃o�X�̃��b�N����у^�X�N�X���b�h�̔r�������,
                 ���̊֐��̌Ăяo�����ŕۏ؂���)

  Arguments:    p            ���C�u�����̃��[�N�o�b�t�@ (�����̂��߂Ɉ����n��)
                req_type     �R�}���h���N�G�X�g�̎��
                retry_max    ���g���C�ő��

  Returns:      ���ʂ� CARD_RESULT_SUCCESS �ł���� TRUE.
 *---------------------------------------------------------------------------*/
BOOL CARDi_Request(CARDiCommon * p, int req_type, int retry_count)
{
    /* PXI ���������Ȃ炱���Ŏ��s */
    if ((p->flag & CARD_STAT_INIT_CMD) == 0)
    {
        p->flag |= CARD_STAT_INIT_CMD;
        while (!PXI_IsCallbackReady(PXI_FIFO_TAG_FS, PXI_PROC_ARM7))
        {
            OS_SpinWait(100);
        }
        /* �ŏ��̃R�}���h "INIT" �𑗐M (�ċA) */
        (void)CARDi_Request(p, CARD_REQ_INIT, 1);
    }
    /* �ݒ肵�����L���������t���b�V�� */
    DC_FlushRange(p->cmd, sizeof(*p->cmd));
    DC_WaitWriteBufferEmpty();

    do
    {
        /* �R�}���h���N�G�X�g���M */
        p->command = req_type;
        p->flag |= CARD_STAT_REQ;
        CARDi_SendPxi((u32)req_type);
        /* �㑱����������Βǉ����M */
        switch (req_type)
        {
        case CARD_REQ_INIT:
            CARDi_SendPxi((u32)p->cmd);
            break;
        }
        {
            /* �R�}���h������ҋ@ */
            OSIntrMode bak_psr = OS_DisableInterrupts();
            while ((p->flag & CARD_STAT_REQ) != 0)
            {
                OS_SleepThread(NULL);
            }
            (void)OS_RestoreInterrupts(bak_psr);
        }
        DC_InvalidateRange(p->cmd, sizeof(*p->cmd));
        /* �^�C���A�E�g�Ȃ�w��񐔂܂ōĎ��s */
    }
    while ((p->cmd->result == CARD_RESULT_TIMEOUT) && (--retry_count > 0));

    /* ���ۂ�Ԃ� */
    return (p->cmd->result == CARD_RESULT_SUCCESS);
}


/******************************************************************************/
#endif /* defined(SDK_ARM9) */


/*---------------------------------------------------------------------------*
  $Log: card_request.c,v $
  Revision 1.4  2007/07/31 00:53:00  yosizaki
  update copyright

  Revision 1.3  2007/07/30 01:59:16  yosizaki
  fix about asynchronous task thread.

  Revision 1.2  2005/11/28 01:17:26  yosizaki
  fix about cache operation.

  Revision 1.1  2005/05/12 11:15:23  yosizaki
  moved from ../common/src

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*

  [old log (build/libraries/card/common/src/card_request.c)]

  Revision 1.9  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.8  2005/02/18 11:20:41  yosizaki
  fix around hidden warnings.

  Revision 1.7  2004/11/15 00:54:38  yosizaki
  move ARM7 implementations to ../arm7/src/card_command.c

  Revision 1.6  2004/09/02 11:54:42  yosizaki
  fix CARD include directives.

  Revision 1.5  2004/08/23 10:38:10  yosizaki
  add write-command.
  add retry count on writing timeout.

  Revision 1.4  2004/08/18 08:19:02  yosizaki
  change CARDBackupType format.

  Revision 1.3  2004/08/18 03:24:14  yosizaki
  fix around PXI-INIT command.

  Revision 1.2  2004/07/28 13:18:06  yosizaki
  asynchronous mode support.

  Revision 1.1  2004/07/23 08:29:16  yosizaki
  (none)

 *---------------------------------------------------------------------------*/

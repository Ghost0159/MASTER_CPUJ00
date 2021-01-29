/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - libraries
  File:     card_common.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/mb.h>

#include "../include/card_common.h"
#include "../include/card_spi.h"


/*****************************************************************************/
/* variable */

CARDiCommon cardi_common ATTRIBUTE_ALIGN(32);
static CARDiCommandArg cardi_arg ATTRIBUTE_ALIGN(32);

static u8 cardi_thread_stack[0x400] ATTRIBUTE_ALIGN(4);


/*****************************************************************************/
/* function */

static void CARDi_LockResource(CARDiOwner owner, CARDTargetMode target);
static void CARDi_UnlockResource(CARDiOwner owner, CARDTargetMode target);


/*---------------------------------------------------------------------------*
  Name:         CARDi_SetTask

  Description:  �^�X�N�X���b�h�Ƀ^�X�N��ݒ�.
                (���łɃ^�X�N�X���b�h�� CARDi_WaitTask() �ɂ���Ĕr������
                 ����Ă��邱�Ƃ�, ���̊֐��̌Ăяo�����ŕۏ؂���)

  Arguments:    task       �ݒ肷��^�X�N�֐�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_SetTask(void (*task) (CARDiCommon *))
{
    CARDiCommon *const p = &cardi_common;

    /* �܂��X���[�v���̃^�X�N�X���b�h�D��x��ύX */
    (void)OS_SetThreadPriority(p->thread, p->priority);

    /* ����������^�X�N��ݒ肵, �X���b�h���N���� */
    p->cur_th = p->thread;
    p->task_func = task;
    p->flag |= CARD_STAT_TASK;
    OS_WakeupThreadDirect(p->thread);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_LockResource

  Description:  ���\�[�X�r���̃��b�N.

  Arguments:    owner      ���b�N�̏��L�҂����� lock-ID
                target     ���b�N���ׂ��J�[�h�o�X��̃��\�[�X�^�[�Q�b�g

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_LockResource(CARDiOwner owner, CARDTargetMode target)
{
    CARDiCommon *const p = &cardi_common;
    OSIntrMode bak_psr = OS_DisableInterrupts();
    if (p->lock_owner == owner)
    {
        if (p->lock_target != target)
        {
            OS_TPanic("card-lock : can not reuse same ID for locking without unlocking!");
        }
    }
    else
    {
        while (p->lock_owner != OS_LOCK_ID_ERROR)
            OS_SleepThread(p->lock_queue);
        p->lock_owner = owner;
        p->lock_target = target;
    }
    ++p->lock_ref;
    p->cmd->result = CARD_RESULT_SUCCESS;
    (void)OS_RestoreInterrupts(bak_psr);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_UnlockResource

  Description:  ���\�[�X�r���̃A�����b�N.

  Arguments:    owner      ���b�N�̏��L�҂����� lock-ID
                target     �A�����b�N���ׂ��J�[�h�o�X��̃��\�[�X�^�[�Q�b�g

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_UnlockResource(CARDiOwner owner, CARDTargetMode target)
{
    CARDiCommon *p = &cardi_common;
    OSIntrMode bak_psr = OS_DisableInterrupts();
    if ((p->lock_owner != owner) || !p->lock_ref)
    {
        OS_Panic("card-unlock : specified ID for unlocking is not locking one!");
    }
    else
    {
        if (p->lock_target != target)
        {
            OS_Panic("card-unlock : locking target and unlocking one are different!");
        }
        if (!--p->lock_ref)
        {
            p->lock_owner = OS_LOCK_ID_ERROR;
            p->lock_target = CARD_TARGET_NONE;
            OS_WakeupThread(p->lock_queue);
        }
    }
    p->cmd->result = CARD_RESULT_SUCCESS;
    (void)OS_RestoreInterrupts(bak_psr);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_InitCommon

  Description:  CARD ���C�u�������ʕ����̏�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_InitCommon(void)
{
    CARDiCommon *p = &cardi_common;

    /* ���b�N�Ώۂ̏����� */
    p->lock_owner = OS_LOCK_ID_ERROR;
    p->lock_ref = 0;
    p->lock_target = CARD_TARGET_NONE;

    /* ���L�������̐ݒ� */
#if defined(SDK_ARM9)
    p->cmd = &cardi_arg;
    MI_CpuFillFast(&cardi_arg, 0x00, sizeof(cardi_arg));
    DC_FlushRange(&cardi_arg, sizeof(cardi_arg));
#else
    p->cmd = NULL;
    p->recv_step = 0;
#endif

#if defined(SDK_ARM9)
    /* �L���b�V���S�̖�����臒l�̐ݒ�. (�f�t�H���g�ł͑S�̖����������Ȃ�) */
    p->flush_threshold_ic = 0xFFFFFFFF;
    p->flush_threshold_dc = 0xFFFFFFFF;
#endif

#if !defined(SDK_SMALL_BUILD) && defined(SDK_ARM9)
    /* �K�؂� CARD-ROM �w�b�_����ޔ� */
    if (!MB_IsMultiBootChild())
    {
        MI_CpuCopy8((const void *)HW_ROM_HEADER_BUF, (void *)HW_CARD_ROM_HEADER,
                    HW_CARD_ROM_HEADER_SIZE);
    }
#endif /* !defined(SDK_SMALL_BUILD) && defined(SDK_ARM9) */

#if !defined(SDK_NO_THREAD)
    /* �^�X�N�X���b�h���N�� */
    OS_InitThreadQueue(p->lock_queue);
    OS_InitThreadQueue(p->busy_q);
    p->priority = CARD_THREAD_PRIORITY_DEFAULT;
    OS_CreateThread(p->thread,
                    CARDi_TaskThread, NULL,
                    cardi_thread_stack + sizeof(cardi_thread_stack),
                    sizeof(cardi_thread_stack), p->priority);
    OS_WakeupThreadDirect(p->thread);
#endif

    /* �R�[���o�b�N��o�^ */
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_FS, CARDi_OnFifoRecv);

    /* �e�@�Ȃ�J�[�h�A�N�Z�X��L���ɂ��� */
    if (!MB_IsMultiBootChild())
    {
        CARD_Enable(TRUE);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARD_IsEnabled

  Description:  check if CARD is accessable

  Arguments:    None.

  Returns:      Return card access permission.
 *---------------------------------------------------------------------------*/
static BOOL CARDi_EnableFlag = FALSE;

BOOL CARD_IsEnabled(void)
{
    return CARDi_EnableFlag;
}

/*---------------------------------------------------------------------------*
  Name:         CARD_CheckEnabled

  Description:  Terminate program if CARD is not accessable

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARD_CheckEnabled(void)
{
    if (!CARD_IsEnabled())
    {
        OS_Panic("NITRO-CARD permission denied");
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARD_Enable

  Description:  Set card access permission mode.

  Arguments:    enable       permission mode to be set.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARD_Enable(BOOL enable)
{
    CARDi_EnableFlag = enable;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_WaitAsync

  Description:  �񓯊�������҂�.

  Arguments:    None.

  Returns:      �ŐV�̏������ʂ� CARD_RESULT_SUCCESS �ł���� TRUE.
 *---------------------------------------------------------------------------*/
BOOL CARDi_WaitAsync(void)
{
    CARDiCommon *const p = &cardi_common;
    SDK_ASSERT(CARD_IsAvailable());

    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        while ((p->flag & CARD_STAT_BUSY) != 0)
        {
            OS_SleepThread(p->busy_q);
        }
        (void)OS_RestoreInterrupts(bak_psr);
    }
    return (p->cmd->result == CARD_RESULT_SUCCESS);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_TryWaitAsync

  Description:  �񓯊������ҋ@�����s��, ���ۂɊւ�炸�������ɐ����Ԃ�.

  Arguments:    None.

  Returns:      �ŐV�̔񓯊��������������Ă���� TRUE.
 *---------------------------------------------------------------------------*/
BOOL CARDi_TryWaitAsync(void)
{
    CARDiCommon *const p = &cardi_common;
    SDK_ASSERT(CARD_IsAvailable());

    return !(p->flag & CARD_STAT_BUSY);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_IsAvailable

  Description:  check if CARD library is ready

  Arguments:    None.

  Returns:      If CARD library is ready, True.
 *---------------------------------------------------------------------------*/
BOOL CARD_IsAvailable(void)
{
    CARDiCommon *const p = &cardi_common;
    return (p->flag != 0);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_GetResultCode

  Description:  get result code of latest command.

  Arguments:    None.

  Returns:      result code of latest command.
 *---------------------------------------------------------------------------*/
CARDResult CARD_GetResultCode(void)
{
    CARDiCommon *const p = &cardi_common;
    SDK_ASSERT(CARD_IsAvailable());

    return p->cmd->result;
}

/*---------------------------------------------------------------------------*
  Name:         CARD_GetThreadPriority

  Description:  get current priority of internal thread which deal with
                asynchronous CARD-ROM tasks.

  Arguments:    None.

  Returns:      priority of internal thread.
 *---------------------------------------------------------------------------*/
u32 CARD_GetThreadPriority(void)
{
    CARDiCommon *const p = &cardi_common;
    SDK_ASSERT(CARD_IsAvailable());

    return p->priority;
}

/*---------------------------------------------------------------------------*
  Name:         CARD_SetThreadPriority

  Description:  set priority of internal thread which deal with
                asynchronous CARD-ROM tasks.

  Arguments:    None.

  Returns:      priority of internal thread.
 *---------------------------------------------------------------------------*/
u32 CARD_SetThreadPriority(u32 prior)
{
    CARDiCommon *const p = &cardi_common;
    SDK_ASSERT(CARD_IsAvailable());

    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        u32     ret = p->priority;
        SDK_ASSERT((prior >= OS_THREAD_PRIORITY_MIN) && (prior <= OS_THREAD_PRIORITY_MAX));
        p->priority = prior;
        (void)OS_SetThreadPriority(p->thread, p->priority);
        (void)OS_RestoreInterrupts(bak_psr);
        return ret;
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARD_LockRom

  Description:  lock the CARD-bus before ROM operation.

  Arguments:    lock id.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARD_LockRom(u16 lock_id)
{
    SDK_ASSERT(CARD_IsAvailable());

    /* ���\�[�X�����b�N */
    CARDi_LockResource(lock_id, CARD_TARGET_ROM);
    /* �J�[�h�o�X�����b�N */
#if defined(SDK_TEG)
    if (!CARDi_IsTrueRom())
        (void)OS_LockCartridge(lock_id);
    else
#endif
    {
        (void)OS_LockCard(lock_id);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARD_UnlockRom

  Description:  unlock the CARD-bus after ROM operation.

  Arguments:    lock id which is used by CARD_LockRom().

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARD_UnlockRom(u16 lock_id)
{
    SDK_ASSERT(CARD_IsAvailable());
    SDK_ASSERT(cardi_common.lock_target == CARD_TARGET_ROM);

    /* �J�[�h�o�X���A�����b�N */
#if defined(SDK_TEG)
    if (!CARDi_IsTrueRom())
        (void)OS_UnLockCartridge(lock_id);
    else
#endif
    {
        (void)OS_UnlockCard(lock_id);
    }
    /* ���\�[�X���A�����b�N */
    CARDi_UnlockResource(lock_id, CARD_TARGET_ROM);

}

/*---------------------------------------------------------------------------*
  Name:         CARD_LockBackup

  Description:  lock the CARD-bus before backup operation.

  Arguments:    lock id.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARD_LockBackup(u16 lock_id)
{
    SDK_ASSERT(CARD_IsAvailable());

    /* �v���Z�b�T����ROM/�o�b�N�A�b�v�r������� */
    CARDi_LockResource(lock_id, CARD_TARGET_BACKUP);
    /* ��� ARM7 ���A�N�Z�X */
#if defined(SDK_ARM7)
    (void)OS_LockCard(lock_id);
#endif
}

/*---------------------------------------------------------------------------*
  Name:         CARD_UnlockBackup

  Description:  unlock the CARD-bus after backup operation.

  Arguments:    lock id which is used by CARD_LockBackup().

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARD_UnlockBackup(u16 lock_id)
{
    SDK_ASSERT(CARD_IsAvailable());
    SDK_ASSERT(cardi_common.lock_target == CARD_TARGET_BACKUP);

    /* �񓯊������̎��s���Ƀo�X��������悤�Ƃ�����u���b�L���O */
    if (!CARD_TryWaitBackupAsync())
    {
        OS_TWarning("called CARD_UnlockBackup() during backup asynchronous operation. (force to wait)\n");
        (void)CARD_WaitBackupAsync();
    }

    /* ��� ARM7 ���A�N�Z�X */
#if defined(SDK_ARM7)
    (void)OS_UnlockCard(lock_id);
#endif
    /* ���\�[�X���A�����b�N */
    CARDi_UnlockResource(lock_id, CARD_TARGET_BACKUP);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_GetCacheFlushThreshold

  Description:  �L���b�V���������𕔕��I�ɍs�����S�̂֍s������臒l���擾.

  Arguments:    icache            ���߃L���b�V���̖�����臒l���i�[����|�C���^.
                                  NULL�ł���Ζ��������.
                dcache            �f�[�^�L���b�V���̖�����臒l���i�[����|�C���^.
                                  NULL�ł���Ζ��������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARD_GetCacheFlushThreshold(u32 *icache, u32 *dcache)
{
#if defined(SDK_ARM9)
    SDK_ASSERT(CARD_IsAvailable());
    if (icache)
    {
        *icache = cardi_common.flush_threshold_ic;
    }
    if (dcache)
    {
        *dcache = cardi_common.flush_threshold_dc;
    }
#else
    (void)icache;
    (void)dcache;
#endif
}

/*---------------------------------------------------------------------------*
  Name:         CARD_SetCacheFlushThreshold

  Description:  �L���b�V���������𕔕��I�ɍs�����S�̂֍s������臒l��ݒ�.

  Arguments:    icache            ���߃L���b�V���̖�����臒l.
                dcache            �f�[�^�L���b�V���̖�����臒l.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARD_SetCacheFlushThreshold(u32 icache, u32 dcache)
{
#if defined(SDK_ARM9)
    SDK_ASSERT(CARD_IsAvailable());
    cardi_common.flush_threshold_ic = icache;
    cardi_common.flush_threshold_dc = dcache;
#else
    (void)icache;
    (void)dcache;
#endif
}


#if !defined(SDK_SMALL_BUILD)


/*---------------------------------------------------------------------------*
  Name:         CARD_GetRomHeader

  Description:  get data buffer which contains own ROM header information.
                if wireless-downloaded program call this function,
                the data is not equal to HW_ROM_HEADER_BUF's.

  Arguments:    None.

  Returns:      pointer of CARD-ROM header information buffer.
 *---------------------------------------------------------------------------*/
const u8 *CARD_GetRomHeader(void)
{
    return (const u8 *)HW_CARD_ROM_HEADER;
}

#endif /* !defined(SDK_SMALL_BUILD) */


/*---------------------------------------------------------------------------*
  $Log: card_common.c,v $
  Revision 1.25  2007/05/28 04:15:46  yosizaki
  add CARD_SetCacheFlushThreshold.

  Revision 1.24  2007/04/05 06:50:03  yosizaki
  update copyright date.

  Revision 1.23  2007/04/04 04:28:06  yosizaki
  add check about UnlockBackup().

  Revision 1.22  2006/05/18 00:28:30  yosizaki
  add CARD_GetRomHeader for ARM7.

  Revision 1.21  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.20  2005/05/20 05:46:57  yosizaki
  change around HW_CARD_ROM_HEADER.

  Revision 1.19  2005/05/12 11:19:42  yosizaki
  optimize some functions and calling interfaces.
  add comment.

  Revision 1.18  2005/04/14 07:00:48  yosizaki
  fix around CARDi_InitCommon.

  Revision 1.17  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.16  2004/12/15 09:44:45  yasu
  CARD �A�N�Z�X�C�l�[�u���@�\�̒ǉ�

  Revision 1.15  2004/12/07 10:50:56  yosizaki
  change CARDi_CheckValidity to return TRUE.
  add comments.

  Revision 1.14  2004/11/12 05:45:48  yosizaki
  add CARDi_CheckValidity().

  Revision 1.13  2004/11/02 10:03:59  yosizaki
  fix typo.

  Revision 1.12  2004/10/08 14:31:41  yosizaki
  add CARD_GetRomHeader().

  Revision 1.11  2004/09/10 01:05:49  yosizaki
  fix declaration position of variables.

  Revision 1.10  2004/09/10 00:00:16  yosizaki
  add assertion checkings.

  Revision 1.9  2004/09/02 11:54:42  yosizaki
  fix CARD include directives.

  Revision 1.8  2004/08/31 10:59:28  yosizaki
  add errorcode CARD_RESULT_SUCCESS in some functions.

  Revision 1.7  2004/08/18 03:24:14  yosizaki
  fix around PXI-INIT command.

  Revision 1.6  2004/07/28 13:18:06  yosizaki
  asynchronous mode support.

  Revision 1.5  2004/07/23 08:28:46  yosizaki
  change backup-routine to ARM7 only.

  Revision 1.4  2004/07/23 01:04:55  yada
  OS_UnLock* -> OS_Unlock*.
  type of lockID for lock functions becomes u16

  Revision 1.3  2004/07/22 11:31:47  yosizaki
  fix CARD access of ARM7.

  Revision 1.2  2004/07/19 13:14:57  yosizaki
  add task thread.

  Revision 1.1  2004/07/10 10:19:55  yosizaki
  (none)


 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_task.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_task.c,v $
  Revision 1.8  2007/07/31 00:58:42  yosizaki
  update copyright

  Revision 1.7  2007/07/30 03:43:19  yosizaki
  fix about MB_End

  Revision 1.6  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.5  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.4  2005/02/24 07:34:25  yosizaki
  �s�v�� #ifdef __cplusplus ������.

  Revision 1.3  2005/01/27 11:27:32  yosizaki
  remove debug-output...

  Revision 1.2  2004/12/08 09:32:46  yosizaki
  fix around end of task-thread.

  Revision 1.1  2004/11/11 11:45:57  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include "mb_task.h"


/* struct ------------------------------------------------------------------ */

typedef struct
{
    OSThread th[1];                    /* thread context */
    MBiTaskInfo *volatile list;        /* waiting task list */
    MBiTaskInfo end_task;              /* task structure for end-command */
}
MBiTaskWork;

/* variable ---------------------------------------------------------------- */

static MBiTaskWork *mbi_task_work = NULL;


/* function ---------------------------------------------------------------- */

static void MBi_TaskThread(void *arg)
{
    MBiTaskWork *const p = (MBiTaskWork *) arg;
    for (;;)
    {
        MBiTaskInfo *trg = NULL;
        /* ���̃^�X�N���擾 */
        {
            OSIntrMode bak_cpsr = OS_DisableInterrupts();
            /* �A�C�h����ԂȂ�X���[�v */
            while (!p->list)
            {
                (void)OS_SetThreadPriority(p->th, OS_THREAD_PRIORITY_MIN);
                OS_SleepThread(NULL);
            }
            trg = p->list;
            p->list = p->list->next;
            (void)OS_SetThreadPriority(p->th, trg->priority);
            (void)OS_RestoreInterrupts(bak_cpsr);
        }
        /* �^�X�N�����s */
        if (trg->task)
            (*trg->task) (trg);
        /* �^�X�N�����R�[���o�b�N���s */
        {
            OSIntrMode bak_cpsr = OS_DisableInterrupts();
            MB_TASK_FUNC callback = trg->callback;
            /*
             * ������, �X���b�h�D�惌�x���ɂ��Ă͐T�d�ɑ��삷��.
             * 1. ���̃^�X�N�������̂ł���΍ō��Ɏw��. (�҂�sleep)
             * 2. ���̃^�X�N�����茻�݂�荂����΂���ɕύX.
             * 3. ���̃^�X�N�����茻�݂��Ⴏ��΂��̂܂�.
             * �D�惌�x����������Ⴍ�Ȃ邱�Ƃ͖���.
             */
            const u32 cur_priority = OS_GetThreadPriority(p->th);
            u32     new_priority;
            if (!p->list)
                new_priority = OS_THREAD_PRIORITY_MIN;
            else if (cur_priority < p->list->priority)
                new_priority = p->list->priority;
            else
                new_priority = cur_priority;
            if (new_priority != cur_priority)
                (void)OS_SetThreadPriority(p->th, new_priority);
            trg->next = NULL;
            trg->busy = FALSE;
            if (callback)
                (*callback) (trg);
            /*
             * �I���v���Ȃ犄�荞�݋֎~�̂܂܃X���b�h�I��.
             * (���̋֎~�ݒ�̓R���e�L�X�g�؂�ւ��̏u�Ԃ܂ŗL��)
             */
            if (trg == &p->end_task)
                break;
            (void)OS_RestoreInterrupts(bak_cpsr);
        }
    }
    OS_TPrintf("task-thread end.\n");
    OS_ExitThread();
    return;
}

/*---------------------------------------------------------------------------*
  Name:         MBi_InitTaskThread

  Description:  �^�X�N�X���b�h���N������.
                
  Arguments:    p_work     �������[�N�p�̃o�b�t�@.
                           MBi_EndTaskThread() �������܂œ����Ŏg�p�����.
                size       p_work �̃o�C�g�T�C�Y.
                           MB_TASK_WORK_MIN �ȏ�ł���K�v������,
                           size - MB_TASK_WORK_MIN ���X�^�b�N�Ɏg�p�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_InitTaskThread(void *p_work, u32 size)
{
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    if (!mbi_task_work)
    {
        MBiTaskWork *const p = (MBiTaskWork *) p_work;

        SDK_ASSERT(size >= MB_TASK_WORK_MIN);
        SDK_ASSERT(OS_IsThreadAvailable());

        /* ���[�N�\����, �X�^�b�N�o�b�t�@, �^�X�N�X���b�h�̏��� */
        mbi_task_work = p;
        MBi_InitTaskInfo(&p->end_task);
        p->list = NULL;
        size = (u32)((size - sizeof(MBiTaskWork)) & ~3);
        OS_CreateThread(p->th, MBi_TaskThread, p,
                        (u8 *)(p + 1) + size, size, OS_THREAD_PRIORITY_MIN);
        OS_WakeupThreadDirect(p->th);
    }
    (void)OS_RestoreInterrupts(bak_cpsr);
}

/*---------------------------------------------------------------------------*
  Name:         MBi_IsTaskAvailable

  Description:  �^�X�N�X���b�h�����ݎg�p�\������.
                
  Arguments:    None.

  Returns:      ���ݎg�p�\�Ȃ� TRUE, �����łȂ��Ȃ� FALSE.
 *---------------------------------------------------------------------------*/
BOOL MBi_IsTaskAvailable(void)
{
    return (mbi_task_work != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         MBi_InitTaskInfo

  Description:  �^�X�N���\���̂�����������.
                �g�p����O�� 1 �񂾂��Ăяo���K�v������.

  Arguments:    pt         ���������̃^�X�N���\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_InitTaskInfo(MBiTaskInfo * pt)
{
    SDK_ASSERT(pt != NULL);
    MI_CpuClear8(pt, sizeof(*pt));
}

/*---------------------------------------------------------------------------*
  Name:         MBi_IsTaskBusy

  Description:  �^�X�N��񂪌��ݎg�p��������.
                
  Arguments:    pt         �^�X�N���

  Returns:      ���ݎg�p���Ȃ� TRUE, �����łȂ��Ȃ� FALSE.
 *---------------------------------------------------------------------------*/
BOOL MBi_IsTaskBusy(volatile const MBiTaskInfo * pt)
{
    return pt->busy != FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         MBi_SetTask

  Description:  �^�X�N������X���b�h�ɒǉ�����.
                
  Arguments:    pt         ���ݎg�p���łȂ��^�X�N���
                task       �^�X�N�֐�
                callback   �^�X�N�������̃R�[���o�b�N (NULL �Ȃ疳��)
                priority   �^�X�N���s���̃X���b�h�D��x

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_SetTask(MBiTaskInfo * pt, MB_TASK_FUNC task, MB_TASK_FUNC callback, u32 priority)
{
    MBiTaskWork *const p_work = mbi_task_work;

    SDK_ASSERT(pt != NULL);

    /* ���C�u�����I�����܂��̓J�[�h�������A�Â��ɏ����𖳎�����悤�ύX */
    if (!MBi_IsTaskAvailable())
    {
        OS_TWarning("MBi_SetTask() ignored... (task-thread is not available now)");
        return;
    }
    if (pt->busy)
    {
        OS_TWarning("MBi_SetTask() ignored... (specified structure is busy)");
        return;
    }

    /* �D�惌�x���̊g����`�ւ̑Ή� */
    if (priority > OS_THREAD_PRIORITY_MAX)
    {
        const u32 cur_priority = OS_GetThreadPriority(p_work->th);
        if (priority == MB_TASK_PRIORITY_ABOVE)
        {
            /* �Ăяo������� 1 �������D�� */
            priority = (u32)((cur_priority > OS_THREAD_PRIORITY_MIN) ?
                             (cur_priority - 1) : OS_THREAD_PRIORITY_MIN);
        }
        else if (priority == MB_TASK_PRIORITY_BELOW)
        {
            /* �Ăяo������� 1 ������D�� */
            priority = (u32)((cur_priority < OS_THREAD_PRIORITY_MAX) ?
                             (cur_priority + 1) : OS_THREAD_PRIORITY_MAX);
        }
        else if (priority == MB_TASK_PRIORITY_NORMAL)
        {
            /* �Ăяo�����Ɠ����D�惌�x�� */
            priority = cur_priority;
        }
        else
        {
            /* �P�Ȃ�s���w�� */
            priority = OS_THREAD_PRIORITY_MAX;
        }
    }
    /* �^�X�N�ǉ� */
    {
        OSIntrMode bak_cpsr = OS_DisableInterrupts();
        pt->busy = TRUE;
        pt->priority = priority;
        pt->task = task;
        pt->callback = callback;
        /* �A�C�h����Ԃ̐V�K�^�X�N�Ȃ�X���b�h���N�� */
        if (!p_work->list)
        {

            if (pt == &p_work->end_task)
            {
                /* ��������^�X�N�X���b�h�̗��p���֎~���� */
                mbi_task_work = NULL;
            }

            p_work->list = pt;
            OS_WakeupThreadDirect(p_work->th);
        }
        else
        {
            /* ���X�g����łȂ���Α}�� */
            MBiTaskInfo *pos = p_work->list;
            /* �I���R�}���h�Ȃ��ɏI�[�ɒǉ� */
            if (pt == &p_work->end_task)
            {
                while (pos->next)
                    pos = pos->next;
                pos->next = pt;
                /* ��������^�X�N�X���b�h�̗��p���֎~���� */
                mbi_task_work = NULL;
            }
            /* �ʏ�R�}���h�Ȃ�D�惌�x�����ɑ}�� */
            else
            {
                if (pos->priority > priority)
                {
                    p_work->list = pt;
                    pt->next = pos;
                }
                else
                {
                    while (pos->next && (pos->next->priority <= priority))
                        pos = pos->next;
                    pt->next = pos->next;
                    pos->next = pt;
                }
            }
        }
        (void)OS_RestoreInterrupts(bak_cpsr);
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBi_EndTaskThread

  Description:  �^�X�N�X���b�h���I������.
                
  Arguments:    callback   �^�X�N�X���b�h�I�����̃R�[���o�b�N (NULL �Ȃ疳��)
                           ���̃R�[���o�b�N�̓^�X�N�X���b�h�I�����O�̏�Ԃ�
                           ���荞�݂��֎~�����܂܌Ăяo�����.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_EndTaskThread(MB_TASK_FUNC callback)
{
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    if (MBi_IsTaskAvailable())
    {
        MBi_SetTask(&mbi_task_work->end_task, NULL, callback, OS_THREAD_PRIORITY_MIN);
    }
    (void)OS_RestoreInterrupts(bak_cpsr);
}

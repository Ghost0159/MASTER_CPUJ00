/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CTRDG - libraries - ARM9
  File:     ctrdg_task.c

  Copyright 2003,2004,2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ctrdg_task.c,v $
  Revision 1.3  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.2  2006/04/07 03:26:35  okubata_ryoma
  CTRDG_SetTaskThreadPriority�֐������J

  Revision 1.1  2006/04/05 10:48:30  okubata_ryoma
  AGB�o�b�N�A�b�v���C�u������SDK���^�̂��߂̕ύX


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

/*******************************************************

    function's description

********************************************************/
static CTRDGiTaskWork *ctrdgi_task_work = NULL;
static CTRDGTaskInfo ctrdgi_task_list;

static void CTRDGi_TaskThread(void *arg);

u64     ctrdg_task_stack[CTRDG_TASK_STACK_SIZE / sizeof(u64)];
/*---------------------------------------------------------------------------*
  Name:         CTRDGi_InitTaskThread

  Description:  �^�X�N�X���b�h���N������.
                
  Arguments:    p_work     �������[�N�p�̃o�b�t�@.
                           CTRDGi_EndTaskThread() �������܂œ����Ŏg�p�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CTRDGi_InitTaskThread(void *p_work)
{
    // IRQ���荞�݂̋֎~
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    // ���̍\���̂�NULL�Ȃ�X���b�h�쐬
    if (!ctrdgi_task_work)
    {
        CTRDGiTaskWork *const p = (CTRDGiTaskWork *) p_work;

        // �X���b�h������������Ă��Ďg�p�\�����f
        SDK_ASSERT(OS_IsThreadAvailable());

        /* ���[�N�\����, �X�^�b�N�o�b�t�@, �^�X�N�X���b�h�̏��� */
        // ����ł��̍\���̂�NULL�łȂ��Ȃ�̂Ń^�X�N�X���b�h�͐V��������Ȃ� 
        ctrdgi_task_work = p;
        // end_task�̍\���̂�������
        CTRDGi_InitTaskInfo(&p->end_task);
        // ctrdgi_task_list�\���̂�������
        CTRDGi_InitTaskInfo(&ctrdgi_task_list);
        // �҂��Ă���^�X�N���X�g�͂��̎��_�ł͂Ȃ��͂��Ȃ̂�NULL������
        p->list = NULL;

        OS_CreateThread(p->th, CTRDGi_TaskThread, p,
                        ctrdg_task_stack + CTRDG_TASK_STACK_SIZE / sizeof(u64),
                        CTRDG_TASK_STACK_SIZE, CTRDG_TASK_PRIORITY_DEFAULT);
        OS_WakeupThreadDirect(p->th);
    }
    // IRQ���荞�݋���߂�
    (void)OS_RestoreInterrupts(bak_cpsr);
}

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_IsTaskAvailable

  Description:  �^�X�N�X���b�h�����ݎg�p�\������.
                
  Arguments:    None.

  Returns:      ���ݎg�p�\�Ȃ� TRUE, �����łȂ��Ȃ� FALSE.
 *---------------------------------------------------------------------------*/
BOOL CTRDGi_IsTaskAvailable(void)
{
    return (ctrdgi_task_work != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_InitTaskInfo

  Description:  �^�X�N���\���̂�����������.
                �g�p����O�� 1 �񂾂��Ăяo���K�v������.

  Arguments:    pt         ���������̃^�X�N���\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CTRDGi_InitTaskInfo(CTRDGTaskInfo * pt)
{
    SDK_ASSERT(pt != NULL);
    MI_CpuClear8(pt, sizeof(*pt));
}

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_IsTaskBusy

  Description:  �^�X�N��񂪌��ݎg�p��������.
                
  Arguments:    pt         �^�X�N���

  Returns:      ���ݎg�p���Ȃ� TRUE, �����łȂ��Ȃ� FALSE.
 *---------------------------------------------------------------------------*/
BOOL CTRDGi_IsTaskBusy(volatile const CTRDGTaskInfo * pt)
{
    return pt->busy != FALSE;
}

static void CTRDGi_TaskThread(void *arg)
{
    CTRDGiTaskWork *const p = (CTRDGiTaskWork *) arg;
    // �X���b�h���I�����閽�߂�����܂Ń��[�v
    for (;;)
    {
        // �\���̂�������
        CTRDGTaskInfo trg;
        MI_CpuClear8(&trg, sizeof(CTRDGTaskInfo));
        /* ���̃^�X�N���擾 */
        {
            // IRQ���荞�݋֎~
            OSIntrMode bak_cpsr = OS_DisableInterrupts();
            /* �A�C�h����ԂȂ�X���[�v */
            // �҂��Ă���^�X�N���X�g�Ƀ^�X�N������܂Ń��[�v���đ҂�
            while (!p->list)
            {
                OS_SleepThread(NULL);
            }
            // �҂��̃^�X�N���X�g�Ƀ^�X�N�������̂ŁA���̃^�X�N���\���̂�trg�ɃR�s�[
            trg = *p->list;
            // IRQ���荞�݋���߂�
            (void)OS_RestoreInterrupts(bak_cpsr);
        }
        /* �^�X�N�����s */
        if (trg.task)
            // �֐��|�C���^�Ń^�X�N�����s�A������trg
            trg.result = (u32)(*trg.task) (&trg);
        /* �^�X�N�����R�[���o�b�N���s */
        // �����ɂ��Ă���Ă��Ƃ̓^�X�N���I����Ă���̂Ń^�X�N�̃R�[���o�b�N
        {
            // IRQ���荞�݋֎~
            OSIntrMode bak_cpsr = OS_DisableInterrupts();
            // �R�[���o�b�N�֐���ݒ�
            CTRDG_TASK_FUNC callback = trg.callback;

            // �^�X�N�����s���ł͂Ȃ��͂��Ȃ̂�FALSE
            ctrdgi_task_list.busy = FALSE;
            // �R�[���o�b�N�֐��������
            if (callback)
                // �֐��|�C���^�ŃR�[���o�b�N�֐����ĂԁA������trg
                (void)(*callback) (&trg);
            /*
             * �I���v���Ȃ犄�荞�݋֎~�̂܂܃X���b�h�I��.
             * (���̋֎~�ݒ�̓R���e�L�X�g�؂�ւ��̏u�Ԃ܂ŗL��)
             */
            //if (p->list == &p->end_task)
            if (ctrdgi_task_work == NULL)
                break;

            // ���X�g�\���̂̏�����
            p->list = NULL;

            (void)OS_RestoreInterrupts(bak_cpsr);
        }
    }
    OS_TPrintf("task-thread end.\n");
    OS_ExitThread();
    return;
}

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_SetTask

  Description:  �^�X�N������X���b�h�ɒǉ�����.
                
  Arguments:    pt         ���ݎg�p���łȂ��^�X�N���
                task       �^�X�N�֐�
                callback   �^�X�N�������̃R�[���o�b�N (NULL �Ȃ疳��)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CTRDGi_SetTask(CTRDGTaskInfo * pt, CTRDG_TASK_FUNC task, CTRDG_TASK_FUNC callback)
{
    // ���݂̃X���b�h�̃|�C���^��A�҂��^�X�N���X�g�Ȃǂ̓������\���̂�����
    CTRDGiTaskWork *const p_work = ctrdgi_task_work;

    SDK_ASSERT(pt != NULL);
    SDK_ASSERT(CTRDGi_IsTaskAvailable());

    if (!CTRDGi_IsTaskAvailable())
    {
        OS_TPanic("CTRDGi_SetTask() failed! (task-thread is not available now)");
    }

    // �����X���b�h�̒��Ń^�X�N�����s���Ȃ�_��
    if (ctrdgi_task_list.busy)
    {
        OS_TPanic("CTRDGi_SetTask() failed! (specified structure is busy)");
    }

    /* �^�X�N�ǉ� */
    {
        // �\���̂Ƀp�����[�^��ݒ�
        OSIntrMode bak_cpsr = OS_DisableInterrupts();
        pt->busy = TRUE;
        pt->task = task;
        pt->callback = callback;
        /* �A�C�h����Ԃ̐V�K�^�X�N�Ȃ�X���b�h���N�� */

        // ���̃^�X�N���I���R�}���h�Ȃ�
        if (pt == &p_work->end_task)
        {
            /* ��������^�X�N�X���b�h�̗��p���֎~���� */
            ctrdgi_task_work = NULL;
        }
        // �҂��^�X�N���X�g�ɂ��̃^�X�N�̍\���̂����ă^�X�N�X���b�h���N����
        ctrdgi_task_list = *pt;
        // ���̂̃A�h���X���i�[����B
        p_work->list = &ctrdgi_task_list;
        OS_WakeupThreadDirect(p_work->th);

        (void)OS_RestoreInterrupts(bak_cpsr);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_EndTaskThread

  Description:  �^�X�N�X���b�h���I������.
                
  Arguments:    callback   �^�X�N�X���b�h�I�����̃R�[���o�b�N (NULL �Ȃ疳��)
                           ���̃R�[���o�b�N�̓^�X�N�X���b�h�I�����O�̏�Ԃ�
                           ���荞�݂��֎~�����܂܌Ăяo�����.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void CTRDGi_EndTaskThread(CTRDG_TASK_FUNC callback)
{
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    if (CTRDGi_IsTaskAvailable())
    {
        (void)CTRDGi_SetTask(&ctrdgi_task_work->end_task, NULL, callback);
    }
    (void)OS_RestoreInterrupts(bak_cpsr);
}

/*---------------------------------------------------------------------------*
  Name:         CTRDG_SetTaskThreadPriority

  Description:  �^�X�N�X���b�h�̗D��x��ύX����B
                
  Arguments:    priority   �^�X�N�X���b�h�̗D��x
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void CTRDG_SetTaskThreadPriority(u32 priority)
{
    if (ctrdgi_task_work)
    {
        CTRDGiTaskWork *const p = ctrdgi_task_work;
        (void)OS_SetThreadPriority(p->th, priority);
    }
}

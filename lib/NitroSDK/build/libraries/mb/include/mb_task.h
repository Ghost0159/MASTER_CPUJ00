/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_task.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_task.h,v $
  Revision 1.3  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.1  2004/11/11 11:51:41  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#if	!defined(NITRO_MB_TASK_H_)
#define NITRO_MB_TASK_H_


#if	defined(__cplusplus)
extern  "C"
{
#endif


/* constant ---------------------------------------------------------------- */

/* �^�X�N�X���b�h�D�惌�x���̊g����` */
    /* �Ăяo������� 1 ���������D�惌�x�����w�� */
#define MB_TASK_PRIORITY_ABOVE  (OS_THREAD_PRIORITY_MAX + 1)
    /* �Ăяo������� 1 �����Ⴂ�D�惌�x�����w�� */
#define MB_TASK_PRIORITY_BELOW  (OS_THREAD_PRIORITY_MAX + 2)
    /* �Ăяo�����Ɠ����D�惌�x�����w�� */
#define MB_TASK_PRIORITY_NORMAL (OS_THREAD_PRIORITY_MAX + 3)

#define MB_TASK_WORK_MIN	(sizeof(OSThread) + 256)


/* struct ------------------------------------------------------------------ */

    struct MBiTaskInfo_tag;

    typedef void (*MB_TASK_FUNC) (struct MBiTaskInfo_tag *);

/*
 * �^�X�N�X���b�h�ɗv������^�X�N���\����.
 */
    typedef struct MBiTaskInfo_tag
    {
/* private: */
        struct MBiTaskInfo_tag *next;  /* next element as list */
        u32     busy:1;                /* if now processing, set 1 */
        u32     priority:31;           /* thread priority */
        MB_TASK_FUNC task;             /* task function */
        MB_TASK_FUNC callback;         /* callback */
/* public: */
        u32     param[4];              /* user-defined argument and return-value */
    }
    MBiTaskInfo;



/* function ---------------------------------------------------------------- */


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
    void    MBi_InitTaskThread(void *p_work, u32 size);

/*---------------------------------------------------------------------------*
  Name:         MBi_IsTaskAvailable

  Description:  �^�X�N�X���b�h�����ݎg�p�\������.
                
  Arguments:    None.

  Returns:      ���ݎg�p�\�Ȃ� TRUE, �����łȂ��Ȃ� FALSE.
 *---------------------------------------------------------------------------*/
    BOOL    MBi_IsTaskAvailable(void);

/*---------------------------------------------------------------------------*
  Name:         MBi_InitTaskInfo

  Description:  �^�X�N���\���̂�����������.
                �g�p����O�� 1 �񂾂��Ăяo���K�v������.

  Arguments:    pt         ���������̃^�X�N���\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    MBi_InitTaskInfo(MBiTaskInfo * pt);

/*---------------------------------------------------------------------------*
  Name:         MBi_IsTaskBusy

  Description:  �^�X�N��񂪌��ݎg�p��������.
                
  Arguments:    pt         �^�X�N���

  Returns:      ���ݎg�p���Ȃ� TRUE, �����łȂ��Ȃ� FALSE.
 *---------------------------------------------------------------------------*/
    BOOL    MBi_IsTaskBusy(volatile const MBiTaskInfo * pt);

/*---------------------------------------------------------------------------*
  Name:         MBi_SetTask

  Description:  �^�X�N������X���b�h�ɒǉ�����.
                
  Arguments:    pt         ���ݎg�p���łȂ��^�X�N���
                task       �^�X�N�֐�
                callback   �^�X�N�������̃R�[���o�b�N (NULL �Ȃ疳��)
                priority   �^�X�N���s���̃X���b�h�D��x

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    MBi_SetTask(MBiTaskInfo * pt, MB_TASK_FUNC task, MB_TASK_FUNC callback, u32 priority);

/*---------------------------------------------------------------------------*
  Name:         MBi_EndTaskThread

  Description:  �^�X�N�X���b�h���I������.
                
  Arguments:    callback   �^�X�N�X���b�h�I�����̃R�[���o�b�N (NULL �Ȃ疳��)
                           ���̃R�[���o�b�N�̓^�X�N�X���b�h�I�����O�̏�Ԃ�
                           ���荞�݂��֎~�����܂܌Ăяo�����.
  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    MBi_EndTaskThread(MB_TASK_FUNC callback);


#if	defined(__cplusplus)
}
#endif


#endif                                 /* NITRO_MB_TASK_H_ */

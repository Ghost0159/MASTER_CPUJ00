/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CTRDG - include
  File:     ctrdg_task.h

  Copyright 2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ctrdg_task.h,v $
  Revision 1.3  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.2  2006/04/07 03:28:00  okubata_ryoma
  CTRDG_SetTaskThreadPriority�֐������J�A�X���b�h�̗D��x�̕ύX

  Revision 1.1  2006/04/05 10:34:15  okubata_ryoma
  AGB�o�b�N�A�b�v���C�u������SDK���^�̂��߂̕ύX


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	NITRO_CTRDG_TASK_H_
#define	NITRO_CTRDG_TASK_H_

#include <nitro.h>

/* �^�X�N�X���b�h�D�惌�x�� */
#define CTRDG_TASK_PRIORITY_DEFAULT 20 // main�X���b�h�����D��x���Ⴍ�āA�^�X�N�X���b�h��main�X���b�h�̊Ԃɂ������̃X���b�h������悤��

/* �^�X�N�X���b�h�̃X�^�b�N�T�C�Y */
#define CTRDG_TASK_STACK_SIZE 1024

struct CTRDGTaskInfo_tag;

// ������CTRDGTaskInfo_tag�̊֐��|�C���^��錾
typedef u32 (*CTRDG_TASK_FUNC) (struct CTRDGTaskInfo_tag *);

/*
 * �^�X�N�X���b�h�ɗv������^�X�N���\����.
 */
typedef struct CTRDGTaskInfo_tag
{
    // �X���b�h�ő҂��^�X�N���X�g�Ƀ^�X�N������̂�҂��Ă郋�[�v�𔲂��Ă���A�^�X�N�̎��s���I���܂ł̊�TRUE
    CTRDG_TASK_FUNC task;              /* task function */
    CTRDG_TASK_FUNC callback;          /* callback */
    u32     result;                    /* task �֐��̕Ԃ�l */
    u8     *data;                      /* �������݂����f�[�^ program�R�}���h�ȊO�͎g�p���܂��� */
    u8     *adr;                       /* �ǂݏ����������f�[�^�̃A�h���X */
    u32     offset;                    /* �Z�N�^���̃o�C�g�P�ʂ̃I�t�Z�b�g */
    u32     size;                      /* �T�C�Y */
    u8     *dst;                       /* ���[�h�����f�[�^���i�[���郏�[�N�̈�̃A�h���X */
    u16     sec_num;                   /* �Z�N�^No */
    u8      busy;                      /* if now processing */
    u8      param[1];                  /* user-defined argument and return-value */
}
CTRDGTaskInfo;

typedef struct
{
    OSThread th[1];                    /* thread context */
    CTRDGTaskInfo *volatile list;      /* waiting task list */
    CTRDGTaskInfo end_task;            /* task structure for end-command */
}
CTRDGiTaskWork;

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_InitTaskThread

  Description:  �^�X�N�X���b�h���N������.
                
  Arguments:    p_work     �������[�N�p�̃o�b�t�@.
                           CTRDGi_EndTaskThread() �������܂œ����Ŏg�p�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CTRDGi_InitTaskThread(void *p_work);

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_IsTaskAvailable

  Description:  �^�X�N�X���b�h�����ݎg�p�\������.
                
  Arguments:    None.

  Returns:      ���ݎg�p�\�Ȃ� TRUE, �����łȂ��Ȃ� FALSE.
 *---------------------------------------------------------------------------*/
BOOL    CTRDGi_IsTaskAvailable(void);

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_InitTaskInfo

  Description:  �^�X�N���\���̂�����������.
                �g�p����O�� 1 �񂾂��Ăяo���K�v������.

  Arguments:    pt         ���������̃^�X�N���\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CTRDGi_InitTaskInfo(CTRDGTaskInfo * pt);

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_IsTaskBusy

  Description:  �^�X�N��񂪌��ݎg�p��������.
                
  Arguments:    pt         �^�X�N���

  Returns:      ���ݎg�p���Ȃ� TRUE, �����łȂ��Ȃ� FALSE.
 *---------------------------------------------------------------------------*/
BOOL    CTRDGi_IsTaskBusy(volatile const CTRDGTaskInfo * pt);

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_SetTask

  Description:  �^�X�N������X���b�h�ɒǉ�����.
                
  Arguments:    pt         ���ݎg�p���łȂ��^�X�N���
                task       �^�X�N�֐�
                callback   �^�X�N�������̃R�[���o�b�N (NULL �Ȃ疳��)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CTRDGi_SetTask(CTRDGTaskInfo * pt, CTRDG_TASK_FUNC task, CTRDG_TASK_FUNC callback);

/*---------------------------------------------------------------------------*
  Name:         CTRDGi_EndTaskThread

  Description:  �^�X�N�X���b�h���I������.
                
  Arguments:    callback   �^�X�N�X���b�h�I�����̃R�[���o�b�N (NULL �Ȃ疳��)
                           ���̃R�[���o�b�N�̓^�X�N�X���b�h�I�����O�̏�Ԃ�
                           ���荞�݂��֎~�����܂܌Ăяo�����.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CTRDGi_EndTaskThread(CTRDG_TASK_FUNC callback);

/*---------------------------------------------------------------------------*
  Name:         CTRDG_SetTaskThreadPriority

  Description:  �^�X�N�X���b�h�̗D��x��ύX����B
                
  Arguments:    priority   �^�X�N�X���b�h�̗D��x
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CTRDG_SetTaskThreadPriority(u32 priority);

#endif /* NITRO_CTRDG_TASK_H_ */

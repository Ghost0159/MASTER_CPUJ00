/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - libraries
  File:     card_common.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: card_common.h,v $
  Revision 1.30  2007/11/13 04:22:02  yosizaki
  support erase-SUBSECTOR.

  Revision 1.29  2007/10/04 13:11:37  yosizaki
  add test commands.

  Revision 1.28  2007/06/06 01:43:30  yasu
  ����N�x�C��

  Revision 1.27  2007/05/28 23:31:52  yosizaki
  add CARD_SetCacheFlushThreshold.

  Revision 1.26  2006/04/28 07:43:40  yosizaki
  add DeviceCaps.

  Revision 1.25  2006/04/10 04:14:26  yosizaki
  add cmd->spec (erase timeout)

  Revision 1.24  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.23  2005/11/07 01:09:47  okubata_ryoma
  SDK_ASSERT_ON_COMPILE����SDK_COMPILER_ASSERT�ɕύX

  Revision 1.22  2005/11/01 06:14:25  okubata_ryoma
  SDK_ASSERT_ON_COMPILE�����p��

  Revision 1.21  2005/10/13 01:14:40  yosizaki
  add initial_status.

  Revision 1.20  2005/09/02 07:03:35  yosizaki
  add CARD_STAT_CANCEL

  Revision 1.19  2005/07/04 01:27:22  yosizaki
  fix size of padding in SDK_THREAD_INFINITY.

  Revision 1.18  2005/06/30 00:00:58  yosizaki
  fix padding for alignment.

  Revision 1.17  2005/06/28 09:52:53  yada
  applied for SDK_THREAD_INFINITY

  Revision 1.16  2005/05/12 11:19:14  yosizaki
  optimize some functions and calling interfaces.
  add comment.

  Revision 1.15  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.14  2005/01/24 06:35:09  yosizaki
  copyright �N�\�L�C��.

  Revision 1.13  2005/01/19 12:17:55  yosizaki
  delete pragma `pack'.

  Revision 1.12  2004/12/15 09:44:45  yasu
  CARD �A�N�Z�X�C�l�[�u���@�\�̒ǉ�

  Revision 1.11  2004/12/08 12:39:18  yosizaki
  add device-spec members.

  Revision 1.10  2004/11/12 05:44:53  yosizaki
  add CARDi_CheckValidity().

  Revision 1.9  2004/08/23 10:38:23  yosizaki
  add write-command.
  add retry count on writing timeout.

  Revision 1.8  2004/08/18 03:24:03  yosizaki
  fix around PXI-INIT command.

  Revision 1.7  2004/07/28 13:18:14  yosizaki
  asynchronous mode support.

  Revision 1.6  2004/07/27 06:47:31  yosizaki
  small fix (warning "padding inserted")

  Revision 1.5  2004/07/24 08:23:55  yasu
  Change type of card owner ID (with error status) to s32.

  Revision 1.4  2004/07/23 08:28:23  yosizaki
  change backup-routine to ARM7 only.

  Revision 1.3  2004/07/22 11:33:28  yosizaki
  fix CARD access of ARM7.

  Revision 1.2  2004/07/19 13:14:32  yosizaki
  add task thread.

  Revision 1.1  2004/07/10 10:20:05  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __NITRO_CARD_COMMON_H__
#define __NITRO_CARD_COMMON_H__


#include <nitro.h>
#include <nitro/pxi.h>


/*****************************************************************************/
/* macro */

/* �z��̗v�f���擾 */
#define	COUNT_OF_(array)	(sizeof(array) / sizeof(*array))

/* �r�b�g���Z�}�N�� */
#define	BIT_MASK(n)	((1 << (n)) - 1)
#define	ALIGN_MASK(a)	((a) - 1)
#define	ALIGN_BYTE(n, a)	(((u32)(n) + ALIGN_MASK(a)) & ~ALIGN_MASK(a))
#define CARD_ALIGN_HI_BIT(n)     (((u32)(n)) & ~ALIGN_MASK(CARD_ROM_PAGE_SIZE))
#define CARD_ALIGN_LO_BIT(n)     (((u32)(n)) & ALIGN_MASK(CARD_ROM_PAGE_SIZE))


/*****************************************************************************/
/* constant */

/* CARD ������� */
enum
{
    CARD_STAT_INIT = (1 << 0),         /* �������ς� */
    CARD_STAT_INIT_CMD = (1 << 1),     /* �R�}���hPXI�������ς� */
    CARD_STAT_BUSY = (1 << 2),         /* ���[�h�֐����̂̏����� */
    CARD_STAT_TASK = (1 << 3),         /* �^�X�N�X���b�h�ֈڊǒ� */
    CARD_STAT_RECV = (1 << 4),         /* PXI �o�R���N�G�X�g����M�� */
    CARD_STAT_REQ = (1 << 5),          /* �^�X�N�������� PXI �o�R�̈˗��� */
    CARD_STAT_CANCEL = (1 << 6)        /* �L�����Z���v���� */
};

typedef enum
{
    CARD_TARGET_NONE,
    CARD_TARGET_ROM,
    CARD_TARGET_BACKUP
}
CARDTargetMode;

/* �o�b�N�A�b�v�f�o�C�X�@�\ (�����g�p) */
#define CARD_BACKUP_CAPS_AVAILABLE      (u32)(CARD_BACKUP_CAPS_READ - 1)
#define CARD_BACKUP_CAPS_READ           (u32)(1 << CARD_REQ_READ_BACKUP)
#define CARD_BACKUP_CAPS_WRITE          (u32)(1 << CARD_REQ_WRITE_BACKUP)
#define CARD_BACKUP_CAPS_PROGRAM        (u32)(1 << CARD_REQ_PROGRAM_BACKUP)
#define CARD_BACKUP_CAPS_VERIFY         (u32)(1 << CARD_REQ_VERIFY_BACKUP)
#define CARD_BACKUP_CAPS_ERASE_PAGE     (u32)(1 << CARD_REQ_ERASE_PAGE_BACKUP)
#define CARD_BACKUP_CAPS_ERASE_SECTOR   (u32)(1 << CARD_REQ_ERASE_SECTOR_BACKUP)
#define CARD_BACKUP_CAPS_ERASE_CHIP     (u32)(1 << CARD_REQ_ERASE_CHIP_BACKUP)
#define CARD_BACKUP_CAPS_READ_STATUS    (u32)(1 << CARD_REQ_READ_STATUS)
#define CARD_BACKUP_CAPS_WRITE_STATUS   (u32)(1 << CARD_REQ_WRITE_STATUS)
#define CARD_BACKUP_CAPS_ERASE_SUBSECTOR    (u32)(1 << CARD_REQ_ERASE_SUBSECTOR_BACKUP)


/*****************************************************************************/
/* declaration */

typedef s32 CARDiOwner;                // s32 for error status


/*
 * PXI �R�}���h���N�G�X�g����M�p�����[�^.
 */
typedef struct CARDiCommandArg
{
    CARDResult result;                 /* �߂�l */
    CARDBackupType type;               /* �f�o�C�X�^�C�v */
    u32     id;                        /* �J�[�h ID */
    u32     src;                       /* �]���� */
    u32     dst;                       /* �]���� */
    u32     len;                       /* �]���� */

    struct
    {
        /* memory property */
        u32     total_size;            /* memory capacity (BYTE) */
        u32     sect_size;             /* write-sector (BYTE) */
        u32     subsect_size;          /* write-sector (BYTE) */
        u32     page_size;             /* write-page (BYTE) */
        u32     addr_width;            /* address width on command (BYTE) */
        /* maximun wait-time (as LCD-V-COUNT tick) */
        u32     program_page;
        u32     write_page;
        u32     write_page_total;
        u32     erase_chip;
        u32     erase_chip_total;
        u32     erase_sector;
        u32     erase_sector_total;
        u32     erase_subsector;
        u32     erase_subsector_total;
        u32     erase_page;
        /* initial parameter of status register */
        u8      initial_status;
        u8      padding1[3];
        u32     caps;
        u8      padding2[4];
    }
    spec;

}
CARDiCommandArg;


/* CARD ���ʃp�����[�^ */
typedef struct CARDiCommon
{
    /* �R�}���h�����p���L������ */
    CARDiCommandArg *cmd;

    int     command;                   /* ���݂̃R�}���h */
#if	defined(SDK_ARM7)
    u32     recv_step;                 /* ��M�X�e�b�v�� */
#endif

    /*
     * �J�[�h�A�N�Z�X���Ǘ�.
     * �v���Z�b�T���̃J�[�h/�o�b�N�A�b�v�r�������.
     * �����, CARD �A�N�Z�X���g�p���镡���̔񓯊��֐�(Rom&Backup)��
     * ����X���b�h����Ăяo�����ꍇ�����邽�߂ɕK�v.
     *
     * OSMutex ���X���b�h�ɕt���ĉ��̂ɑ΂�, ����� lock-ID �ɕt���ĉ��.
     */
    volatile CARDiOwner lock_owner;    // ==s32 with Error status
    volatile int lock_ref;
#ifndef SDK_THREAD_INFINITY
    OSThreadQueue lock_queue[4 / sizeof(OSThreadQueue)];
#else
    OSThreadQueue lock_queue[1];
#endif
    CARDTargetMode lock_target;

    /* API �^�X�N�p�����[�^ */
    u32     src;                       /* �]���� */
    u32     dst;                       /* �]���� */
    u32     len;                       /* �]���T�C�Y */
    u32     dma;                       /* DMA �`�����l�� */
    CARDRequest req_type;              /* ���N�G�X�g�^�C�v */
    int     req_retry;                 /* ���N�G�X�g�̃��g���C�� */
    CARDRequestMode req_mode;          /* ���N�G�X�g�̓��샂�[�h */
    MIDmaCallback callback;            /* �R�[���o�b�N */
    void   *callback_arg;              /* �R�[���o�b�N���� */
    void    (*task_func) (struct CARDiCommon *);        /* �^�X�N */

    /* �^�X�N�X���b�h */
    OSThread thread[1];                /* �X���b�h�{�� */
    OSThread *cur_th;                  /* �^�X�N�������̃X���b�h.
                                          ���[�U�X���b�h�̂��Ƃ����� */
    u32     priority;
#ifndef SDK_THREAD_INFINITY
    OSThreadQueue busy_q[4 / sizeof(OSThreadQueue)];    /* �^�X�N�����҂��X���b�h */
#else
    OSThreadQueue busy_q[1];           /* �^�X�N�����҂��X���b�h */
#endif

    /* ��ԃt���O */
    volatile u32 flag;

#if	defined(SDK_ARM9)
    u32     flush_threshold_ic;        /* ���߃L���b�V���S�̖�����臒l */
    u32     flush_threshold_dc;        /* �f�[�^�L���b�V���S�̖�����臒l */
#endif

    /* padding */
#ifndef SDK_THREAD_INFINITY
    u8      dummy[20];
#endif

    /* �Ō�ɓ]�������o�b�N�A�b�v�y�[�W�̃L���b�V�� */
    u8      backup_cache_page_buf[256] ATTRIBUTE_ALIGN(32);

}
CARDiCommon;


SDK_COMPILER_ASSERT(sizeof(CARDiCommandArg) % 32 == 0);

SDK_COMPILER_ASSERT(sizeof(CARDiCommon) % 32 == 0);


/*****************************************************************************/
/* variable */

extern CARDiCommon cardi_common;


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         CARDi_SendPxi

  Description:  PXI FIFO ���[�h���M.

  Arguments:    data       ���M�f�[�^

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void CARDi_SendPxi(u32 data)
{
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_FS, data, TRUE) < 0)
        ;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_OnFifoRecv

  Description:  PXI FIFO ���[�h��M�R�[���o�b�N.

  Arguments:    tag        PXI �^�O (��� PXI_FIFO_TAG_FS)
                data       ��M�f�[�^
                err        �G���[�r�b�g (���d�l�ɂ�����)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_OnFifoRecv(PXIFifoTag tag, u32 data, BOOL err);

/*---------------------------------------------------------------------------*
  Name:         CARDi_GetTargetMode

  Description:  CARD �o�X�̌��݂̃��b�N�^�[�Q�b�g���擾.

  Arguments:    None.

  Returns:      CARDTargetMode �Ŏ������ 3 �̏�Ԃ̂����ꂩ.
 *---------------------------------------------------------------------------*/
static inline CARDTargetMode CARDi_GetTargetMode(void)
{
    return cardi_common.lock_target;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_WaitAsync

  Description:  �񓯊�������҂�.

  Arguments:    None.

  Returns:      �ŐV�̏������ʂ� CARD_RESULT_SUCCESS �ł���� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    CARDi_WaitAsync(void);

/*---------------------------------------------------------------------------*
  Name:         CARDi_TryWaitAsync

  Description:  �񓯊������ҋ@�����s��, ���ۂɊւ�炸�������ɐ����Ԃ�.

  Arguments:    None.

  Returns:      �ŐV�̔񓯊��������������Ă���� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    CARDi_TryWaitAsync(void);


/*---------------------------------------------------------------------------*
  Name:         CARDi_WaitTask

  Description:  �^�X�N�X���b�h�̎g�p�������擾�ł���܂ő҂�.
                (����̃o�X�̃��b�N��, ���̊֐��̌Ăяo�����ŕۏ؂���)

  Arguments:    p            ���C�u�����̃��[�N�o�b�t�@ (�����̂��߂Ɉ����n��)
                callback     �A�N�Z�X�I����̃R�[���o�b�N�֐�
                callback_arg �R�[���o�b�N�֐��̈���

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void CARDi_WaitTask(CARDiCommon * p, MIDmaCallback callback, void *callback_arg)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();
    while ((p->flag & CARD_STAT_BUSY) != 0)
    {
        OS_SleepThread(p->busy_q);
    }
    p->flag |= CARD_STAT_BUSY;
    p->callback = callback;
    p->callback_arg = callback_arg;
    (void)OS_RestoreInterrupts(bak_psr);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_SetTask

  Description:  �^�X�N�X���b�h�Ƀ^�X�N��ݒ�.
                (���łɃ^�X�N�X���b�h�� CARDi_WaitTask() �ɂ���Ĕr������
                 ����Ă��邱�Ƃ�, ���̊֐��̌Ăяo�����ŕۏ؂���)

  Arguments:    task       �ݒ肷��^�X�N�֐�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_SetTask(void (*task) (CARDiCommon *));

/*---------------------------------------------------------------------------*
  Name:         CARDi_EndTask

  Description:  �^�X�N�I����ʒm���ă^�X�N�X���b�h�̎g�p���������.

  Arguments:    p            ���C�u�����̃��[�N�o�b�t�@ (�����̂��߂Ɉ����n��)
                is_own_task  ���g�̃v���Z�b�T�ł̃^�X�N�Ȃ� TRUE
                             (ARM7 �ɂ�� ARM9 �^�X�N�����̊����Ȃ� FALSE)

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void CARDi_EndTask(CARDiCommon * p, BOOL is_own_task)
{
    const MIDmaCallback func = p->callback;
    void   *const arg = p->callback_arg;

    {
        OSIntrMode bak_psr = OS_DisableInterrupts();

        p->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK | CARD_STAT_CANCEL);
        OS_WakeupThread(p->busy_q);
        if ((p->flag & CARD_STAT_RECV) != 0)
        {
            OS_WakeupThreadDirect(p->thread);
        }
        (void)OS_RestoreInterrupts(bak_psr);
    }

    if (is_own_task && func)
    {
        (*func) (arg);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_TaskThread

  Description:  �^�X�N�X���b�h�̃��C���֐�.

  Arguments:    arg          �s�g�p

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_TaskThread(void *arg);

/*---------------------------------------------------------------------------*
  Name:         CARDi_InitCommon

  Description:  CARD ���C�u�������ʕ����̏�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_InitCommon(void);


#if	defined(SDK_ARM9)

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
BOOL    CARDi_Request(CARDiCommon * p, int req_type, int retry_max);


#endif /* SDK_ARM9 */


#endif  /*__NITRO_CARD_SPI_H__*/

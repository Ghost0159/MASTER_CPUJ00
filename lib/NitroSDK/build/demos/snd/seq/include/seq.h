/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - seq
  File:     seq.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: seq.h,v $
  Revision 1.2  2006/01/18 02:12:27  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/13 03:29:19  ida
  SND��NITRO-SDK�ֈڐ�

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  ����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef SEQ_H_
#define SEQ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>

/* �N���b�N�C���^�[�o���萔(�ʕb) */
#define SEQ_CLOCK_INTERVAL_NITRO_VBLANK 16715   /* NITRO�̂u�u�����N�Ԋu */

/* �G���[�R�[�h */
#define SEQ_ERROR_WRONG_HEADER         1        /* �s���ȃw�b�_�`�����N */
#define SEQ_ERROR_NOT_FORMAT0          2        /* �t�H�[�}�b�g0 �ł͂Ȃ�SMF */
#define SEQ_ERROR_DIVISION_TIMECODE    3        /* SMF�̃f�B�r�W�������^�C���R�[�h�`�� */
#define SEQ_ERROR_WRONG_TRACK          4        /* �s���ȃg���b�N�`�����N */
#define SEQ_ERROR_WRONG_DELTA_TIME     5        /* �s���ȃf���^�^�C���̉ϒ��l */

/* �T�E���h�n���h�� */
typedef struct tSeqHandle
{
    u8      state;                     /* �X�e�[�g */
    u8      running_status;            /* �����j���O�X�e�[�^�X�o�b�t�@ */
    u8      next_delta_bytesize;       /* ���̃C�x���g�̃f���^�^�C���̃o�C�g�T�C�Y */
    u8      padding1;
    u16     division;                  /* SMF����\ */
    u16     padding2;
    u32     tempo;                     /* �e���| */
    u32     time_control;              /* ���ԃ}�l�[�W�� */
    u32     time_per_callback;         /* �P�R�[���o�b�N�Ői�ގ���(tick*�ʕb/�l������) */
    u32     chunk_size;                /* �f�[�^�`�����N�̃T�C�Y */
    u32     total_tick;                /* �擪���琔�������݈ʒu�̑�tick�� */
    u32     delta_time;                /* ���̃C�x���g�܂ł̃f���^�^�C�� */
    const u8 *current_ptr;             /* ���݈ʒu�̃|�C���^ */
    const u8 *track_begin;             /* �g���b�N�J�n�ʒu�̃|�C���^ */
    const u8 *loop_begin;              /* ���[�v�J�n�ʒu�̃|�C���^ */
}
SeqHandle;

void    SeqInit(u32 clock_interval, void (*callback) (const u8 *));
BOOL    SeqMain(SeqHandle * block);
BOOL    SeqPlay(SeqHandle * block, const u8 *smfdata);
void    SeqStop(SeqHandle * block);
void    SeqPause(SeqHandle * block);
u8      SeqGetErrorCode(void);

#ifdef __cplusplus
}
#endif

#endif // SEQ_H_

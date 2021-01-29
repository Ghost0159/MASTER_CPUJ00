/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - seq
  File:     seq.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: seq.c,v $
  Revision 1.2  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/13 03:29:19  ida
  SND��NITRO-SDK�ֈڐ�

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  ����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include "seq.h"
#include "smfdefine.h"

/* ��Ԓ萔 */
#define SEQ_STATE_PLAY 0x01            /* �Đ��� */
#define SEQ_STATE_END  0x02            /* �Đ��I��(�G���h�I�u�g���b�N) */
#define SEQ_STATE_LOOP 0x04            /* ���[�v�t���O */
#define SEQ_STATE_MUTE 0x10            /* �~���[�g */

/* �f�t�H���g�e���|��120BPM(�ʕb) */
#define SEQ_DEFAULT_TEMPO 500000

/* �V�X�e���G�N�X�N���[�V�u�p�o�b�t�@�T�C�Y(�T�C�Y���I�[�o�[�������b�Z�[�W�͖���) */
#define SEQ_SYSX_BUFFER_SIZE 64


static u8 SeqReadVariableData(const u8 *data, u32 *result);
static u8 SeqReadSMFHeader(SeqHandle * block, const u8 *data);
static u8 SeqExecMetaEvent(SeqHandle * block);

static u16 ReverseEndian16(u16 data);
static u32 ReverseEndian32(u32 data);

typedef struct tSeqSystemBlock
{
    void    (*callback) (const u8 *);
    u32     clock_interval;            /* SeqMain() ���Ă΂�鎞�ԊԊu. �P�ʂ̓ʕb */
}
SeqSystemBlock;

SeqSystemBlock seq_sys;

/* MIDI�C�x���g���̃f�[�^�o�C�g�� */
static const u8 seq_midi_byte_size[8] = {
/*  8x,9x,Ax,Bx,Cx,Dx,Ex,Fx*/
    2, 2, 2, 2, 1, 1, 2, 0
};

/* �V�X�e���G�N�X�N���[�V�u�p�o�b�t�@ */
static u8 seqSysxBuffer[SEQ_SYSX_BUFFER_SIZE];

/* �G���[���̃G���[�R�[�h��ۑ����� */
static u8 seqErrorCode;

/*---------------------------------------------------------------------------*
  Name:         SeqInit

  Description:  �V�[�P���X�V�X�e��������������

  Arguments:    clock_interval - SeqMain() ���Ăяo�����ԊԊu�B�P�ʂ̓ʕb�B
                callback - MIDI�C�x���g�����������Ƃ��ɌĂ΂��֐���ݒ肷��

  Returns:      None
 *---------------------------------------------------------------------------*/
void SeqInit(u32 clock_interval, void (*callback) (const u8 *))
{
    seq_sys.callback = callback;       // �R�[���o�b�N�֐���ۑ�
    seq_sys.clock_interval = clock_interval;    // �Ăяo���Ԋu��ۑ�
}

/*---------------------------------------------------------------------------*
  Name:         SeqMain

  Description:  �V�[�P���T�̃��C�����[�`��

  Arguments:    handle - ��������Ȃ̃n���h��

  Returns:      �G���[������������ TRUE
 *---------------------------------------------------------------------------*/
BOOL SeqMain(SeqHandle * handle)
{
    u8      read_size;
    u8      current_event;
    u8      f_exist_status;            /* MIDI�C�x���g���߂����݂��邩(�����j���O�X�e�[�^�X���ǂ���) */

    /* �Đ���ԂłȂ��ꍇ�͂��̂܂܋A�� */
    if (!(handle->state & SEQ_STATE_PLAY))
    {
        return FALSE;
    }

    /* �P�R�[���o�b�N���̎��Ԃ�i�߂� */
    handle->time_control += handle->time_per_callback;

    while (handle->time_control > handle->tempo)
    {
        /* tick���̏��� */

        /* �f���^�^�C�������Z */
        if (handle->delta_time > 0)
        {
            handle->delta_time--;
        }

        while (handle->delta_time == 0)
        {

            /* �f���^�^�C����0�ɂȂ����̂ŃC�x���g���s */
            handle->current_ptr += handle->next_delta_bytesize;

            /* current_ptr��MIDI���b�Z�[�W�擪 */
            if (*handle->current_ptr >= 0x80)
            {
                /* ���߂���n�܂�̂Ń����j���O�X�e�[�^�X�ݒ� */
                current_event = handle->running_status = *handle->current_ptr;
                f_exist_status = 1;
            }
            else
            {
                /* �l����n�܂�̂Ń����j���O�X�e�[�^�X�g�p */
                if (handle->running_status < 0x80)
                {
                    seqErrorCode = SEQ_ERROR_WRONG_TRACK;
                    return TRUE;       /* �G���[ */
                }
                current_event = handle->running_status;
                f_exist_status = 0;
            }

            if (*handle->current_ptr == SMF_META)
            {

                /* ���^�C�x���g�̓V�[�P���T���ŏ��� */
                read_size = SeqExecMetaEvent(handle);

                if (handle->state & SEQ_STATE_END)
                {
                    /* END OF TRACK: �Ȃ��Ō�܂ŉ��t���� */
                    (void)SeqStop(handle);
                    handle->state &= ~SEQ_STATE_END;
                    return FALSE;
                }
                else if (handle->state & SEQ_STATE_LOOP)
                {
                    /* LOOP END: ���[�v�J�n�ʒu�� */
                    handle->current_ptr = handle->loop_begin;
                    handle->state &= ~SEQ_STATE_LOOP;
                }
                else
                {
                    /* ���̃C�x���g�܂Ői�� */
                    handle->current_ptr += read_size;
                }

            }
            else                       /* ���^�C�x���g�ȊO */
            {
                if (*handle->current_ptr == SMF_SYSX)
                {
                    /* �V�X�e���G�N�X�N���[�V�u */

                    if (SEQ_SYSX_BUFFER_SIZE > handle->current_ptr[1] + 1)
                    {
                        u8      i;

                        /* �o�b�t�@�ɃR�s�[ */
                        seqSysxBuffer[0] = handle->current_ptr[0];
                        for (i = 1; i <= handle->current_ptr[1]; i++)
                        {
                            seqSysxBuffer[i] = handle->current_ptr[i + 1];
                        }

                        /* MIDI�C�x���g���R�[���o�b�N�ɑ��� */
                        seq_sys.callback(seqSysxBuffer);
                    }

                    /* ���̃C�x���g�܂Ői�� */
                    handle->current_ptr += handle->current_ptr[1] + 2;
                }
                else
                {
                    /* �V���[�gMIDI�C�x���g */

                    /* MIDI�C�x���g���R�[���o�b�N�ɑ��� */
                    seq_sys.callback(handle->current_ptr);

                    /* ���̃C�x���g�܂Ői�� */
                    handle->current_ptr +=
                        seq_midi_byte_size[(current_event >> 4) - 8] + f_exist_status;
                }
            }

            /* ���̃C�x���g�̃f���^�^�C����ǂ� */
            handle->next_delta_bytesize =
                SeqReadVariableData(handle->current_ptr, &handle->delta_time);
            if (handle->next_delta_bytesize == 0)
            {
                return TRUE;           /* �G���[ */
            }
        }

        /* tick��i�߂� */
        handle->total_tick++;

        /* ���Ԃ�i�߂� */
        handle->time_control -= handle->tempo;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         SeqExecMetaEvent

  Description:  ���^�C�x���g�̏������[�`��

  Arguments:    handle - ��������Ȃ̃n���h��

  Returns:      ���^�C�x���g�̃o�C�g�T�C�Y
 *---------------------------------------------------------------------------*/
static u8 SeqExecMetaEvent(SeqHandle * handle)
{
    switch (handle->current_ptr[1])
    {
    case SMF_META_MARKER:             /* �}�[�J�[ */
        if (handle->current_ptr[2] != 1)
        {
            break;
        }

        /* NITRO Composer �݊��̃��[�v�@�\ */
        if (handle->current_ptr[3] == '[')
        {
            /* ���[�v�J�n�ʒu */
            handle->loop_begin = handle->current_ptr + handle->current_ptr[2] + 3;
        }
        else if (handle->current_ptr[3] == ']')
        {
            /* ���[�v�I���ʒu */
            if (handle->loop_begin != NULL)
            {
                handle->state |= SEQ_STATE_LOOP;
            }
        }
        break;
    case SMF_META_ENDOFTRACK:         /* �G���h�I�u�g���b�N */
        handle->state |= SEQ_STATE_END;
        break;
    case SMF_META_TEMPO:              /* �Z�b�g�e���| */
        handle->tempo = handle->current_ptr[3];
        handle->tempo <<= 8;
        handle->tempo += handle->current_ptr[4];
        handle->tempo <<= 8;
        handle->tempo += handle->current_ptr[5];
        break;
    default:
        break;
    }

    return (u8)(handle->current_ptr[2] + 3);
}

/*---------------------------------------------------------------------------*
  Name:         SeqPlay

  Description:  SMF�f�[�^�̍Đ����J�n

  Arguments:    handle - ��������Ȃ̃n���h��
                smfdata- SMF�f�[�^

  Returns:      ���^�C�x���g�̃o�C�g�T�C�Y
 *---------------------------------------------------------------------------*/
BOOL SeqPlay(SeqHandle * handle, const u8 *smfdata)
{
    u8      read_size;
    static const u8 mtrk[] = "MTrk";
    u8      i;

    /* SMF�w�b�_�`�����N�ǂݍ��� */
    read_size = SeqReadSMFHeader(handle, smfdata);
    if (read_size == 0)
        return TRUE;                   /* �G���[ */

    handle->current_ptr = smfdata + read_size;

    /* �g���b�N�`�����N�w�b�_�ǂݍ��� */
    /* 'MTrk'�̃`�F�b�N */
    for (i = 0; i < 4; i++)
    {
        if (handle->current_ptr[i] != mtrk[i])
        {
            seqErrorCode = SEQ_ERROR_WRONG_TRACK;       /* �G���[ */
            return TRUE;
        }
    }
    handle->current_ptr += sizeof(u8) * 4;
    /* �`�����N�T�C�Y */
    handle->chunk_size = ReverseEndian32(*(u32 *)handle->current_ptr);
    handle->current_ptr += sizeof(u32);

    /* �g���b�N�̐擪 */
    handle->track_begin = handle->current_ptr;

    /* �����ݒ� */
    handle->tempo = SEQ_DEFAULT_TEMPO;
    handle->time_control = 0;
    handle->time_per_callback = handle->division * seq_sys.clock_interval;
    handle->total_tick = 0;
    handle->running_status = 0x00;
    handle->loop_begin = NULL;

    /* ��ԕϐ��ݒ� */
    handle->state = SEQ_STATE_PLAY;

    /* �ŏ��̃f���^�^�C����ǂ� */
    handle->next_delta_bytesize = SeqReadVariableData(handle->current_ptr, &handle->delta_time);
    if (handle->next_delta_bytesize == 0)
    {
        seqErrorCode = SEQ_ERROR_WRONG_TRACK;
        return TRUE;                   /* �G���[ */
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         SeqStop

  Description:  SMF�f�[�^�̍Đ����~

  Arguments:    handle - ��������Ȃ̃n���h��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SeqStop(SeqHandle * handle)
{
    handle->current_ptr = handle->track_begin;

    /* ��ԕϐ��ݒ� */
    handle->state &= ~SEQ_STATE_PLAY;
}

/*---------------------------------------------------------------------------*
  Name:         SeqPause

  Description:  SMF�f�[�^�̍Đ����ꎞ��~

  Arguments:    handle - ��������Ȃ̃n���h��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SeqPause(SeqHandle * handle)
{
    /* ��ԕϐ��ݒ� */
    handle->state &= ~SEQ_STATE_PLAY;
}

/*---------------------------------------------------------------------------*
  Name:         SeqReadSMFHeader

  Description:  SMF�w�b�_�[�`�����N�ǂݍ���

  Arguments:    handle - ��������Ȃ̃n���h��
                data- SMF�f�[�^��

  Returns:      �ǂݍ��񂾃o�C�g�� �ʏ��14
 *---------------------------------------------------------------------------*/
static u8 SeqReadSMFHeader(SeqHandle * handle, const u8 *data)
{
    static const u8 mthd[] = "MThd";
    u8      i;
    const u8 *ptr = data;

    /* �t�H�[�}�b�g0�ȊO�̒�^�łȂ��f�[�^�͑S�ăG���[�Ƃ��� */

    /* 'MThd'�̃`�F�b�N */
    for (i = 0; i < 4; i++)
    {
        if (ptr[i] != mthd[i])
        {
            seqErrorCode = SEQ_ERROR_WRONG_HEADER;      /* �G���[ */
            return 0;
        }
    }
    ptr += sizeof(u8) * 4;

    /* �w�b�_�`�����N�̃f�[�^���͂U�o�C�g */
    if (ReverseEndian32(*(u32 *)ptr) != 6)
    {
        seqErrorCode = SEQ_ERROR_WRONG_HEADER;  /* �G���[ */
        return 0;
    }
    ptr += sizeof(u32);

    /* �t�H�[�}�b�g0 �݂̂����� */
    if (*(u16 *)ptr != SMF_FORMAT0)
    {
        seqErrorCode = SEQ_ERROR_NOT_FORMAT0;   /* �G���[ */
        return 0;
    }
    ptr += sizeof(u16);

    /* �g���b�N���P�łȂ���΃G���[ */
    if (ReverseEndian16(*(u16 *)ptr) != 1)
    {
        seqErrorCode = SEQ_ERROR_WRONG_HEADER;  /* �G���[ */
        return 0;
    }
    ptr += sizeof(u16);

    /* ����\��ۑ� */
    handle->division = ReverseEndian16(*(u16 *)ptr);
    if (handle->division >= 0x8000)
    {
        seqErrorCode = SEQ_ERROR_DIVISION_TIMECODE;     /* �G���[ */
        return 0;
    }
    ptr += sizeof(u16);

    return (u8)(ptr - data);
}

/*---------------------------------------------------------------------------*
  Name:         SeqReadVariableData

  Description:  �ϒ��f�[�^�ǂݍ��݃��[�`��

  Arguments:    data - �ϒ���SMFMIDI�f�[�^��
                result - �ǂݍ��񂾐��l���i�[����̈�

  Returns:      �ǂݍ��񂾃o�C�g���BSMF�ł͍ő�S�o�C�g�܂łȂ̂ŁA
                ����ȏ�̓G���[�Ƃ��ĂO��Ԃ�
 *---------------------------------------------------------------------------*/
static u8 SeqReadVariableData(const u8 *data, u32 *result)
{
    u8      count = 0;
    *result = 0;

    while (*data & 0x80)
    {
        /* �p���o�C�g */
        *result = (*result << 7) + (*data & 0x7f);
        count++;
        if (count == 4)
        {
            /* �S�o�C�g�ڂ��ŏI�o�C�g�łȂ��̂ŃG���[ */
            seqErrorCode = SEQ_ERROR_WRONG_DELTA_TIME;
            return 0;
        }
        data++;
    }

    /* �ŏI�o�C�g */
    *result = (*result << 7) + *data;
    count++;

    return count;
}

/*---------------------------------------------------------------------------*
  Name:         ReverseEndian16

  Description:  �G���f�B�A���ϊ�(�Q�o�C�g)

  Arguments:    data - �ϊ�����f�[�^

  Returns:      �G���f�B�A����ϊ������f�[�^
 *---------------------------------------------------------------------------*/
static u16 ReverseEndian16(u16 data)
{
    return (u16)(((data & 0x00ff) << 8) | ((data & 0xff00) >> 8));
}

/*---------------------------------------------------------------------------*
  Name:         ReverseEndian32

  Description:  �G���f�B�A���ϊ�(�S�o�C�g)

  Arguments:    data - �ϊ�����f�[�^

  Returns:      �G���f�B�A����ϊ������f�[�^
 *---------------------------------------------------------------------------*/
static u32 ReverseEndian32(u32 data)
{
    return (((data >> 24) & 0x000000ff) |
            ((data >> 8) & 0x0000ff00) | ((data << 8) & 0x00ff0000) | ((data << 24) & 0xff000000));
}

/*---------------------------------------------------------------------------*/
u8 SeqGetErrorCode(void)
{
    return seqErrorCode;
}

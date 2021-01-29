/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - seq
  File:     channel.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: channel.c,v $
  Revision 1.2  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/13 03:29:29  ida
  SND��NITRO-SDK�ֈڐ�

  Revision 1.4  2005/04/13 01:15:26  kyuma_koichi
  �g�`�t�@�C���t�H�[�}�b�g�̕ύX

  Revision 1.3  2005/03/08 08:32:59  kyuma_koichi
  Warning�Ή�

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  ����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "smfdefine.h"
#include "channel.h"
#include "piano.g5.pcm16.h"

#define MAX_CHANNEL 16
#define MAX_MIDI_CHANNEL 16

/* �c�r�̃n�[�h�`�����l���̍\���� */
typedef struct ChannelEx
{
    u8      playing;                   /* �Đ��t���O */
    u8      midi_ch;                   /* �Đ����Ă��鉹��MIDI�`�����l�� */
    u8      key;                       /* �Đ�����L�[ */
    u8      velocity;                  /* �Đ�����x���V�e�B */
}
ChannelEx;

/* MIDI�`�����l���̍\���� */
typedef struct MidiChannel
{
    u8      pan;                       /* �p�� */
    u8      volume;                    /* �{�����[�� */
    u8      expression;                /* �G�N�X�v���b�V���� */
}
MidiChannel;

ChannelEx channel[MAX_CHANNEL];
MidiChannel midi_ch[MAX_MIDI_CHANNEL];

static void NoteOn(const u8 *midi_data);
static void NoteOff(const u8 *midi_data);


/*---------------------------------------------------------------------------*
  Name:         ChInit

  Description:  �`�����l���̏�����

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void ChInit(void)
{
    int     ch_num;

    /* �S�Ẵ`�����l�������b�N */
    SND_LockChannel(0xffff, 0);

    for (ch_num = 0; ch_num < MAX_CHANNEL; ch_num++)
    {
        channel[ch_num].playing = FALSE;
    }

    for (ch_num = 0; ch_num < MAX_MIDI_CHANNEL; ch_num++)
    {
        midi_ch[ch_num].volume = 127;
        midi_ch[ch_num].pan = 64;
        midi_ch[ch_num].expression = 127;
    }
}

/*---------------------------------------------------------------------------*
  Name:         ChSetEvent

  Description:  MIDI�C�x���g�����߂��Ď��s����

  Arguments:    midi_data - MIDI�C�x���g�f�[�^��

  Returns:      None
 *---------------------------------------------------------------------------*/
void ChSetEvent(const u8 *midi_data)
{
    switch (midi_data[0] & 0xf0)
    {
    case MIDI_NOTEOFF:                /* �m�[�g�I�t */
        NoteOff(midi_data);
        break;
    case MIDI_NOTEON:                 /* �m�[�g�I�� */
        NoteOn(midi_data);
        break;
    case MIDI_CONTROLCHANGE:          /* �R���g���[���`�F���W */
        switch (midi_data[1])
        {
        case MIDI_CC_VOLUME:          /* �`�����l���{�����[�� */
            midi_ch[midi_data[0] & 0x0f].volume = midi_data[2];
            break;
        case MIDI_CC_PAN:             /* �`�����l���p�� */
            midi_ch[midi_data[0] & 0x0f].pan = midi_data[2];
            break;
        case MIDI_CC_EXPRESSION:      /* �G�N�X�v���b�V���� */
            midi_ch[midi_data[0] & 0x0f].expression = midi_data[2];
            break;
        default:
            break;
        }
    default:
        /* ���̑��̃��b�Z�[�W�͑Ή����Ȃ� */
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         ChAllNoteOff

  Description:  �S�`�����l�����~

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void ChAllNoteOff(void)
{
    int     ch_num;

    SND_StopTimer(0xffff, 0, 0, 0);

    for (ch_num = 0; ch_num < MAX_CHANNEL; ch_num++)
    {
        channel[ch_num].playing = FALSE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NoteOn

  Description:  MIDI�C�x���g�ɏ]���ăm�[�g�I���������s��

  Arguments:    midi_data - MIDI�C�x���g�f�[�^��

  Returns:      None
 *---------------------------------------------------------------------------*/
static void NoteOn(const u8 *midi_data)
{
    int     ch_num;
    ChannelEx *ch;
    s16     db;
    u16     vol;

    for (ch_num = 0; ch_num < MAX_CHANNEL; ch_num++)
    {
        if (!channel[ch_num].playing)
            break;
    }
    if (ch_num == MAX_CHANNEL)
        return;                        /* �󂫃`�����l����������Ȃ����� */

    ch = &channel[ch_num];
    ch->midi_ch = (u8)(midi_data[0] & 0x0f);
    ch->key = midi_data[1];
    ch->velocity = midi_data[2];

    /* ���ʌv�Z */
    db = SND_CalcDecibel(ch->velocity);
    db += SND_CalcDecibel(midi_ch[ch->midi_ch].volume);
    db += SND_CalcDecibel(midi_ch[ch->midi_ch].expression);
    vol = SND_CalcChannelVolume(db);

    /* �����Đ� */
    SND_SetupChannelPcm(ch_num,
                        PIANO_G5_PCM16_FORMAT,
                        piano_g5_pcm16,
                        PIANO_G5_PCM16_LOOPFLAG ? SND_CHANNEL_LOOP_REPEAT : SND_CHANNEL_LOOP_1SHOT,
                        PIANO_G5_PCM16_LOOPSTART,
                        PIANO_G5_PCM16_LOOPLEN,
                        vol & 0xff,
                        (SNDChannelDataShift)(vol >> 8),
                        SND_CalcTimer(PIANO_G5_PCM16_TIMER, (int)(ch->key - 67) * 64),
                        midi_ch[ch->midi_ch].pan);
    SND_StartTimer(1U << ch_num, 0, 0, 0);

    ch->playing = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NoteOff

  Description:  MIDI�C�x���g�ɏ]���ăm�[�g�I�t�������s��

  Arguments:    midi_data - MIDI�C�x���g�f�[�^��

  Returns:      None
 *---------------------------------------------------------------------------*/
static void NoteOff(const u8 *midi_data)
{
    int     ch_num;
    ChannelEx *ch;

    for (ch_num = 0; ch_num < MAX_CHANNEL; ch_num++)
    {
        ch = &channel[ch_num];

        if (!ch->playing)
            continue;
        if ((ch->midi_ch == (u8)(midi_data[0] & 0x0f)) && (ch->key == midi_data[1]))
        {
            /* MIDI�`�����l���ƃL�[�ԍ�����v�œ��ꉹ�Ƃ݂Ȃ� */
            SND_StopTimer(1U << ch_num, 0, 0, 0);
            ch->playing = FALSE;
        }
    }

}

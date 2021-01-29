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
  SNDをNITRO-SDKへ移設

  Revision 1.4  2005/04/13 01:15:26  kyuma_koichi
  波形ファイルフォーマットの変更

  Revision 1.3  2005/03/08 08:32:59  kyuma_koichi
  Warning対応

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  初版

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "smfdefine.h"
#include "channel.h"
#include "piano.g5.pcm16.h"

#define MAX_CHANNEL 16
#define MAX_MIDI_CHANNEL 16

/* ＤＳのハードチャンネルの構造体 */
typedef struct ChannelEx
{
    u8      playing;                   /* 再生フラグ */
    u8      midi_ch;                   /* 再生している音のMIDIチャンネル */
    u8      key;                       /* 再生するキー */
    u8      velocity;                  /* 再生するベロシティ */
}
ChannelEx;

/* MIDIチャンネルの構造体 */
typedef struct MidiChannel
{
    u8      pan;                       /* パン */
    u8      volume;                    /* ボリューム */
    u8      expression;                /* エクスプレッション */
}
MidiChannel;

ChannelEx channel[MAX_CHANNEL];
MidiChannel midi_ch[MAX_MIDI_CHANNEL];

static void NoteOn(const u8 *midi_data);
static void NoteOff(const u8 *midi_data);


/*---------------------------------------------------------------------------*
  Name:         ChInit

  Description:  チャンネルの初期化

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void ChInit(void)
{
    int     ch_num;

    /* 全てのチャンネルをロック */
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

  Description:  MIDIイベントを解釈して実行する

  Arguments:    midi_data - MIDIイベントデータ列

  Returns:      None
 *---------------------------------------------------------------------------*/
void ChSetEvent(const u8 *midi_data)
{
    switch (midi_data[0] & 0xf0)
    {
    case MIDI_NOTEOFF:                /* ノートオフ */
        NoteOff(midi_data);
        break;
    case MIDI_NOTEON:                 /* ノートオン */
        NoteOn(midi_data);
        break;
    case MIDI_CONTROLCHANGE:          /* コントロールチェンジ */
        switch (midi_data[1])
        {
        case MIDI_CC_VOLUME:          /* チャンネルボリューム */
            midi_ch[midi_data[0] & 0x0f].volume = midi_data[2];
            break;
        case MIDI_CC_PAN:             /* チャンネルパン */
            midi_ch[midi_data[0] & 0x0f].pan = midi_data[2];
            break;
        case MIDI_CC_EXPRESSION:      /* エクスプレッション */
            midi_ch[midi_data[0] & 0x0f].expression = midi_data[2];
            break;
        default:
            break;
        }
    default:
        /* その他のメッセージは対応しない */
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         ChAllNoteOff

  Description:  全チャンネルを停止

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

  Description:  MIDIイベントに従ってノートオン処理を行う

  Arguments:    midi_data - MIDIイベントデータ列

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
        return;                        /* 空きチャンネルが見つからなかった */

    ch = &channel[ch_num];
    ch->midi_ch = (u8)(midi_data[0] & 0x0f);
    ch->key = midi_data[1];
    ch->velocity = midi_data[2];

    /* 音量計算 */
    db = SND_CalcDecibel(ch->velocity);
    db += SND_CalcDecibel(midi_ch[ch->midi_ch].volume);
    db += SND_CalcDecibel(midi_ch[ch->midi_ch].expression);
    vol = SND_CalcChannelVolume(db);

    /* 音を再生 */
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

  Description:  MIDIイベントに従ってノートオフ処理を行う

  Arguments:    midi_data - MIDIイベントデータ列

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
            /* MIDIチャンネルとキー番号が一致で同一音とみなす */
            SND_StopTimer(1U << ch_num, 0, 0, 0);
            ch->playing = FALSE;
        }
    }

}

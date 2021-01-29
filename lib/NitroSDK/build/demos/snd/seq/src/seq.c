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
  SNDをNITRO-SDKへ移設

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  初版

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include "seq.h"
#include "smfdefine.h"

/* 状態定数 */
#define SEQ_STATE_PLAY 0x01            /* 再生中 */
#define SEQ_STATE_END  0x02            /* 再生終了(エンドオブトラック) */
#define SEQ_STATE_LOOP 0x04            /* ループフラグ */
#define SEQ_STATE_MUTE 0x10            /* ミュート */

/* デフォルトテンポ＝120BPM(μ秒) */
#define SEQ_DEFAULT_TEMPO 500000

/* システムエクスクルーシブ用バッファサイズ(サイズをオーバーしたメッセージは無視) */
#define SEQ_SYSX_BUFFER_SIZE 64


static u8 SeqReadVariableData(const u8 *data, u32 *result);
static u8 SeqReadSMFHeader(SeqHandle * block, const u8 *data);
static u8 SeqExecMetaEvent(SeqHandle * block);

static u16 ReverseEndian16(u16 data);
static u32 ReverseEndian32(u32 data);

typedef struct tSeqSystemBlock
{
    void    (*callback) (const u8 *);
    u32     clock_interval;            /* SeqMain() が呼ばれる時間間隔. 単位はμ秒 */
}
SeqSystemBlock;

SeqSystemBlock seq_sys;

/* MIDIイベント毎のデータバイト数 */
static const u8 seq_midi_byte_size[8] = {
/*  8x,9x,Ax,Bx,Cx,Dx,Ex,Fx*/
    2, 2, 2, 2, 1, 1, 2, 0
};

/* システムエクスクルーシブ用バッファ */
static u8 seqSysxBuffer[SEQ_SYSX_BUFFER_SIZE];

/* エラー時のエラーコードを保存する */
static u8 seqErrorCode;

/*---------------------------------------------------------------------------*
  Name:         SeqInit

  Description:  シーケンスシステムを初期化する

  Arguments:    clock_interval - SeqMain() を呼び出す時間間隔。単位はμ秒。
                callback - MIDIイベントが発生したときに呼ばれる関数を設定する

  Returns:      None
 *---------------------------------------------------------------------------*/
void SeqInit(u32 clock_interval, void (*callback) (const u8 *))
{
    seq_sys.callback = callback;       // コールバック関数を保存
    seq_sys.clock_interval = clock_interval;    // 呼び出し間隔を保存
}

/*---------------------------------------------------------------------------*
  Name:         SeqMain

  Description:  シーケンサのメインルーチン

  Arguments:    handle - 処理する曲のハンドル

  Returns:      エラーが発生したら TRUE
 *---------------------------------------------------------------------------*/
BOOL SeqMain(SeqHandle * handle)
{
    u8      read_size;
    u8      current_event;
    u8      f_exist_status;            /* MIDIイベント命令が存在するか(ランニングステータスかどうか) */

    /* 再生状態でない場合はそのまま帰る */
    if (!(handle->state & SEQ_STATE_PLAY))
    {
        return FALSE;
    }

    /* １コールバック分の時間を進める */
    handle->time_control += handle->time_per_callback;

    while (handle->time_control > handle->tempo)
    {
        /* tick毎の処理 */

        /* デルタタイムを減算 */
        if (handle->delta_time > 0)
        {
            handle->delta_time--;
        }

        while (handle->delta_time == 0)
        {

            /* デルタタイムが0になったのでイベント実行 */
            handle->current_ptr += handle->next_delta_bytesize;

            /* current_ptrはMIDIメッセージ先頭 */
            if (*handle->current_ptr >= 0x80)
            {
                /* 命令から始まるのでランニングステータス設定 */
                current_event = handle->running_status = *handle->current_ptr;
                f_exist_status = 1;
            }
            else
            {
                /* 値から始まるのでランニングステータス使用 */
                if (handle->running_status < 0x80)
                {
                    seqErrorCode = SEQ_ERROR_WRONG_TRACK;
                    return TRUE;       /* エラー */
                }
                current_event = handle->running_status;
                f_exist_status = 0;
            }

            if (*handle->current_ptr == SMF_META)
            {

                /* メタイベントはシーケンサ内で処理 */
                read_size = SeqExecMetaEvent(handle);

                if (handle->state & SEQ_STATE_END)
                {
                    /* END OF TRACK: 曲を最後まで演奏完了 */
                    (void)SeqStop(handle);
                    handle->state &= ~SEQ_STATE_END;
                    return FALSE;
                }
                else if (handle->state & SEQ_STATE_LOOP)
                {
                    /* LOOP END: ループ開始位置へ */
                    handle->current_ptr = handle->loop_begin;
                    handle->state &= ~SEQ_STATE_LOOP;
                }
                else
                {
                    /* 次のイベントまで進む */
                    handle->current_ptr += read_size;
                }

            }
            else                       /* メタイベント以外 */
            {
                if (*handle->current_ptr == SMF_SYSX)
                {
                    /* システムエクスクルーシブ */

                    if (SEQ_SYSX_BUFFER_SIZE > handle->current_ptr[1] + 1)
                    {
                        u8      i;

                        /* バッファにコピー */
                        seqSysxBuffer[0] = handle->current_ptr[0];
                        for (i = 1; i <= handle->current_ptr[1]; i++)
                        {
                            seqSysxBuffer[i] = handle->current_ptr[i + 1];
                        }

                        /* MIDIイベントをコールバックに送る */
                        seq_sys.callback(seqSysxBuffer);
                    }

                    /* 次のイベントまで進む */
                    handle->current_ptr += handle->current_ptr[1] + 2;
                }
                else
                {
                    /* ショートMIDIイベント */

                    /* MIDIイベントをコールバックに送る */
                    seq_sys.callback(handle->current_ptr);

                    /* 次のイベントまで進む */
                    handle->current_ptr +=
                        seq_midi_byte_size[(current_event >> 4) - 8] + f_exist_status;
                }
            }

            /* 次のイベントのデルタタイムを読む */
            handle->next_delta_bytesize =
                SeqReadVariableData(handle->current_ptr, &handle->delta_time);
            if (handle->next_delta_bytesize == 0)
            {
                return TRUE;           /* エラー */
            }
        }

        /* tickを進める */
        handle->total_tick++;

        /* 時間を進める */
        handle->time_control -= handle->tempo;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         SeqExecMetaEvent

  Description:  メタイベントの処理ルーチン

  Arguments:    handle - 処理する曲のハンドル

  Returns:      メタイベントのバイトサイズ
 *---------------------------------------------------------------------------*/
static u8 SeqExecMetaEvent(SeqHandle * handle)
{
    switch (handle->current_ptr[1])
    {
    case SMF_META_MARKER:             /* マーカー */
        if (handle->current_ptr[2] != 1)
        {
            break;
        }

        /* NITRO Composer 互換のループ機能 */
        if (handle->current_ptr[3] == '[')
        {
            /* ループ開始位置 */
            handle->loop_begin = handle->current_ptr + handle->current_ptr[2] + 3;
        }
        else if (handle->current_ptr[3] == ']')
        {
            /* ループ終了位置 */
            if (handle->loop_begin != NULL)
            {
                handle->state |= SEQ_STATE_LOOP;
            }
        }
        break;
    case SMF_META_ENDOFTRACK:         /* エンドオブトラック */
        handle->state |= SEQ_STATE_END;
        break;
    case SMF_META_TEMPO:              /* セットテンポ */
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

  Description:  SMFデータの再生を開始

  Arguments:    handle - 処理する曲のハンドル
                smfdata- SMFデータ

  Returns:      メタイベントのバイトサイズ
 *---------------------------------------------------------------------------*/
BOOL SeqPlay(SeqHandle * handle, const u8 *smfdata)
{
    u8      read_size;
    static const u8 mtrk[] = "MTrk";
    u8      i;

    /* SMFヘッダチャンク読み込み */
    read_size = SeqReadSMFHeader(handle, smfdata);
    if (read_size == 0)
        return TRUE;                   /* エラー */

    handle->current_ptr = smfdata + read_size;

    /* トラックチャンクヘッダ読み込み */
    /* 'MTrk'のチェック */
    for (i = 0; i < 4; i++)
    {
        if (handle->current_ptr[i] != mtrk[i])
        {
            seqErrorCode = SEQ_ERROR_WRONG_TRACK;       /* エラー */
            return TRUE;
        }
    }
    handle->current_ptr += sizeof(u8) * 4;
    /* チャンクサイズ */
    handle->chunk_size = ReverseEndian32(*(u32 *)handle->current_ptr);
    handle->current_ptr += sizeof(u32);

    /* トラックの先頭 */
    handle->track_begin = handle->current_ptr;

    /* 初期設定 */
    handle->tempo = SEQ_DEFAULT_TEMPO;
    handle->time_control = 0;
    handle->time_per_callback = handle->division * seq_sys.clock_interval;
    handle->total_tick = 0;
    handle->running_status = 0x00;
    handle->loop_begin = NULL;

    /* 状態変数設定 */
    handle->state = SEQ_STATE_PLAY;

    /* 最初のデルタタイムを読む */
    handle->next_delta_bytesize = SeqReadVariableData(handle->current_ptr, &handle->delta_time);
    if (handle->next_delta_bytesize == 0)
    {
        seqErrorCode = SEQ_ERROR_WRONG_TRACK;
        return TRUE;                   /* エラー */
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         SeqStop

  Description:  SMFデータの再生を停止

  Arguments:    handle - 処理する曲のハンドル

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SeqStop(SeqHandle * handle)
{
    handle->current_ptr = handle->track_begin;

    /* 状態変数設定 */
    handle->state &= ~SEQ_STATE_PLAY;
}

/*---------------------------------------------------------------------------*
  Name:         SeqPause

  Description:  SMFデータの再生を一時停止

  Arguments:    handle - 処理する曲のハンドル

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SeqPause(SeqHandle * handle)
{
    /* 状態変数設定 */
    handle->state &= ~SEQ_STATE_PLAY;
}

/*---------------------------------------------------------------------------*
  Name:         SeqReadSMFHeader

  Description:  SMFヘッダーチャンク読み込み

  Arguments:    handle - 処理する曲のハンドル
                data- SMFデータ列

  Returns:      読み込んだバイト数 通常は14
 *---------------------------------------------------------------------------*/
static u8 SeqReadSMFHeader(SeqHandle * handle, const u8 *data)
{
    static const u8 mthd[] = "MThd";
    u8      i;
    const u8 *ptr = data;

    /* フォーマット0以外の定型でないデータは全てエラーとする */

    /* 'MThd'のチェック */
    for (i = 0; i < 4; i++)
    {
        if (ptr[i] != mthd[i])
        {
            seqErrorCode = SEQ_ERROR_WRONG_HEADER;      /* エラー */
            return 0;
        }
    }
    ptr += sizeof(u8) * 4;

    /* ヘッダチャンクのデータ長は６バイト */
    if (ReverseEndian32(*(u32 *)ptr) != 6)
    {
        seqErrorCode = SEQ_ERROR_WRONG_HEADER;  /* エラー */
        return 0;
    }
    ptr += sizeof(u32);

    /* フォーマット0 のみを扱う */
    if (*(u16 *)ptr != SMF_FORMAT0)
    {
        seqErrorCode = SEQ_ERROR_NOT_FORMAT0;   /* エラー */
        return 0;
    }
    ptr += sizeof(u16);

    /* トラックが１つでなければエラー */
    if (ReverseEndian16(*(u16 *)ptr) != 1)
    {
        seqErrorCode = SEQ_ERROR_WRONG_HEADER;  /* エラー */
        return 0;
    }
    ptr += sizeof(u16);

    /* 分解能を保存 */
    handle->division = ReverseEndian16(*(u16 *)ptr);
    if (handle->division >= 0x8000)
    {
        seqErrorCode = SEQ_ERROR_DIVISION_TIMECODE;     /* エラー */
        return 0;
    }
    ptr += sizeof(u16);

    return (u8)(ptr - data);
}

/*---------------------------------------------------------------------------*
  Name:         SeqReadVariableData

  Description:  可変長データ読み込みルーチン

  Arguments:    data - 可変長のSMFMIDIデータ列
                result - 読み込んだ数値を格納する領域

  Returns:      読み込んだバイト数。SMFでは最大４バイトまでなので、
                それ以上はエラーとして０を返す
 *---------------------------------------------------------------------------*/
static u8 SeqReadVariableData(const u8 *data, u32 *result)
{
    u8      count = 0;
    *result = 0;

    while (*data & 0x80)
    {
        /* 継続バイト */
        *result = (*result << 7) + (*data & 0x7f);
        count++;
        if (count == 4)
        {
            /* ４バイト目が最終バイトでないのでエラー */
            seqErrorCode = SEQ_ERROR_WRONG_DELTA_TIME;
            return 0;
        }
        data++;
    }

    /* 最終バイト */
    *result = (*result << 7) + *data;
    count++;

    return count;
}

/*---------------------------------------------------------------------------*
  Name:         ReverseEndian16

  Description:  エンディアン変換(２バイト)

  Arguments:    data - 変換するデータ

  Returns:      エンディアンを変換したデータ
 *---------------------------------------------------------------------------*/
static u16 ReverseEndian16(u16 data)
{
    return (u16)(((data & 0x00ff) << 8) | ((data & 0xff00) >> 8));
}

/*---------------------------------------------------------------------------*
  Name:         ReverseEndian32

  Description:  エンディアン変換(４バイト)

  Arguments:    data - 変換するデータ

  Returns:      エンディアンを変換したデータ
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

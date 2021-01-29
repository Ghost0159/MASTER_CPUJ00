/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - seq
  File:     smfdefine.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: smfdefine.h,v $
  Revision 1.2  2006/01/18 02:12:27  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/13 03:29:19  ida
  SNDをNITRO-SDKへ移設

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  初版

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*                                                                  */
/* MIDIデータ構造定義                                               */
/*                                                                  */
/*------------------------------------------------------------------*/

#ifndef SMFDEFINE_H_
#define SMFDEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------*/
/* MIDI規格における定義                                               */
/*--------------------------------------------------------------------*/

/* 上位4BIT定義 */
#define MIDI_NOTEOFF            0x80   /* ノートオフ */
#define MIDI_NOTEON             0x90   /* ノートオン */
#define MIDI_POLYPRESS          0xA0   /* キーポリフォニックプレッシャー */
#define MIDI_CONTROLCHANGE      0xB0   /* コントロールチェンジ */
#define MIDI_PROGRAMCHANGE      0xC0   /* プログラムチェンジ */
#define MIDI_CANNELPRESS        0xD0   /* チャンネルプレッシャー */
#define MIDI_PITCHBEND          0xE0   /* ピッチベンド */
#define MIDI_SYSTEMMESSAGE      0xF0   /* システムメッセージ */

/* MIDI_SYSTEMMESSAGE */
#define MIDI_SYSX               0xF0   /* エクスクルーシブメッセージ */
/* COMMON MESSAGE */
#define MIDI_MTC                0xF1   /* MIDIタイムコード */
#define MIDI_SONG_POSITION      0xF2   /* ソングポジションポインタ */
#define MIDI_SONG_SELECT        0xF3   /* ソングセレクト */
#define MIDI_UNDEFINED_F4       0xF4   /* 未定義 */
#define MIDI_UNDEFINED_F5       0xF5   /* 未定義 */
#define MIDI_TUNE_REQUEST       0xF6   /* チューンリクエスト */
#define MIDI_END_OF_SYSX        0xF7   /* エンドオブエクスクルーシブ */
/* REALTIME MESSAGE */
#define MIDI_TIMING_CLOCK       0xF8   /* タイミングクロック */
#define MIDI_UNDEFINED_F9       0xF9   /* 未定義 */
#define MIDI_START              0xFA   /* スタート */
#define MIDI_CONTINUE           0xFB   /* コンティニュー */
#define MIDI_STOP               0xFC   /* ストップ */
#define MIDI_UNDEFINED_FD       0xFD   /* 未定義 */
#define MIDI_ACTIVESENSING      0xFE   /* アクティブセンシング */
#define MIDI_SYSTEMRESET        0xFF   /* システムリセット */

/* Control Change */
/* continuos cc */
#define MIDI_CC_BANKSELECT_MSB  0x00   /*   0: バンクセレクト */
#define MIDI_CC_BANKSELECT_LSB  0x20   /*  32: バンクセレクト */
#define MIDI_CC_MODURATION      0x01   /*   1: モジュレーションホイール */
#define MIDI_CC_BREATH_CONTROL  0x02   /*   2: ブレスコントロール */
#define MIDI_CC_FOOT_CONTROL    0x04   /*   4: フットコントロール */
#define MIDI_CC_PORTAMENT_TIME  0x05   /*   5: ポルタメントタイム */
#define MIDI_CC_VOLUME          0x07   /*   7: ボリューム */
#define MIDI_CC_BALANCE         0x08   /*   8: バランス */
#define MIDI_CC_PAN             0x0A   /*  10: パン */
#define MIDI_CC_EXPRESSION      0x0B   /*  11: エクスプレッション */
#define MIDI_CC_GENERAL1        0x10   /*  16: 汎用１ */
#define MIDI_CC_GENERAL2        0x11   /*  17: 汎用２ */
#define MIDI_CC_GENERAL3        0x12   /*  18: 汎用３ */
#define MIDI_CC_GENERAL4        0x13   /*  19: 汎用４ */
/* switched cc */
#define MIDI_CC_HOLD1           0x40   /*  64: ホールド(ダンパー) */
#define MIDI_CC_PORTAMENT       0x41   /*  65: ポルタメントスイッチ */
#define MIDI_CC_SOSTENUTO       0x42   /*  66: ソステヌート */
#define MIDI_CC_SOFT_PEDAL      0x43   /*  67: ソフトペダル */
#define MIDI_CC_LEGATO          0x44   /*  68: レガートフットスイッチ */
#define MIDI_CC_HOLD2           0x45   /*  69: ホールド(フリーズ) */
/* sound controler cc */
#define MIDI_CC_SOUND_CONTROL1  0x46   /*  70: サウンドコントローラー１ */
#define MIDI_CC_SOUND_CONTROL2  0x47   /*  71: サウンドコントローラー２ */
#define MIDI_CC_SOUND_CONTROL3  0x48   /*  72: サウンドコントローラー３ */
#define MIDI_CC_SOUND_CONTROL4  0x49   /*  73: サウンドコントローラー４ */
#define MIDI_CC_SOUND_CONTROL5  0x4A   /*  74: サウンドコントローラー５ */
#define MIDI_CC_SOUND_CONTROL6  0x4B   /*  75: サウンドコントローラー６ */
#define MIDI_CC_SOUND_CONTROL7  0x4C   /*  76: サウンドコントローラー７ */
#define MIDI_CC_SOUND_CONTROL8  0x4D   /*  77: サウンドコントローラー８ */
#define MIDI_CC_SOUND_CONTROL9  0x4E   /*  78: サウンドコントローラー９ */
#define MIDI_CC_SOUND_CONTROL10 0x4F   /*  79: サウンドコントローラー10 */
#define MIDI_CC_SOUND_VARIATION 0x46   /*  70: サウンドバリエーション */
#define MIDI_CC_RESONANCE       0x47   /*  71: レゾナンス */
#define MIDI_CC_RELEASE_TIME    0x48   /*  72: リリースタイム */
#define MIDI_CC_ATTACK_TIME     0x49   /*  73: アタックタイム */
#define MIDI_CC_BRIGHTNESS      0x4A   /*  74: フィルターカットオフ */
#define MIDI_CC_DECAY_TIME      0x4B   /*  75: ディケイタイム */
#define MIDI_CC_VIBRATO_RATE    0x4C   /*  76: ビブラートレート(スピード) */
#define MIDI_CC_VIBRATO_DEPTH   0x4D   /*  77: ビブラートデプス */
#define MIDI_CC_VIBRATO_DELAY   0x4E   /*  78: ビブラートディレイ */
/* portament_control */
#define MIDI_CC_PORTAMENT_CTRL  0x54   /*  84: ポルタメントコントロール */
/* effect controler cc */
#define MIDI_CC_EFFECT_CONTROL1 0x0C   /*  12: エフェクトコントロール１ */
#define MIDI_CC_EFFECT_CONTROL2 0x0D   /*  13: エフェクトコントロール２ */
#define MIDI_CC_REVERB_SEND     0x5B   /*  91: リバーブセンドレベル */
#define MIDI_CC_TREMOLO_DEPTH   0x5C   /*  92: トレモロデプス */
#define MIDI_CC_CHORUS_SEND     0x5D   /*  93: コーラスセンドレベル */
#define MIDI_CC_DELAY_SEND      0x5E   /*  94: ディレイセンドレベル */
#define MIDI_CC_PHASER_DEPTH    0x5F   /*  95: フェイザーデプス */
/* parameter control */
#define MIDI_CC_DATA_ENTRY_MSB  0x06   /*   6: データエントリー */
#define MIDI_CC_DATA_ENTRY_LSB  0x26   /*  38: データエントリー */
#define MIDI_CC_DATA_INCREMENT  0x60   /*  96: インクリメント */
#define MIDI_CC_DATA_DECREMENT  0x61   /*  97: デクリメント */
#define MIDI_CC_RPN_MSB         0x65   /* 101: RPN MSB */
#define MIDI_CC_RPN_LSB         0x64   /* 100: RPN LSB */
#define MIDI_CC_NRPN_MSB        0x63   /*  99: NRPN MSB */
#define MIDI_CC_NRPN_LSB        0x62   /*  98: NRPN LSB */
/* mode messege */
#define MIDI_CC_ALL_SOUND_OFF   0x78   /* 120: オールサウンドオフ */
#define MIDI_CC_RESET_ALL_CTRL  0x79   /* 121: リセットオールコントローラ */
#define MIDI_CC_ROCAL_CONTROL   0x7A   /* 122: ローカルコントロール */
#define MIDI_CC_ALL_NOTE_OFF    0x7B   /* 123: オールノートオフ */
#define MIDI_CC_OMNI_ON         0x7C   /* 124: オムニモードオン */
#define MIDI_CC_OMNI_OFF        0x7D   /* 125: オムニモードオフ */
#define MIDI_CC_MONO_MODE       0x7E   /* 126: モノモードオン */
#define MIDI_CC_POLY_MODE       0x7F   /* 127: ポリモードオン */
/* RPN */
#define MIDI_RPN_PITCHBEND_SENS 0x0000 /* RPN: ピッチベンドセンシティビティ */
#define MIDI_RPN_FINE_TUNE      0x0001 /* RPN: ファインチューン */
#define MIDI_RPN_COASE_TUNE     0x0002 /* RPN: コースチューン */
#define MIDI_RPN_TUNING_PROGRAM 0x0003 /* RPN: チューニングプログラムセレクト */
#define MIDI_RPN_TUNING_BANK    0x0004 /* RPN: チューニングバンクセレクト */
#define MIDI_RPN_MODURATION_SENS 0x0005 /* RPN: モジュレーションセンシティビティ */



/*--------------------------------------------------------------------*/
/* Standard MIDI File フォーマットにおける定義                        */
/*--------------------------------------------------------------------*/

#define SMF_FORMAT0             0      /* SMF format0 */
#define SMF_FORMAT1             1      /* SMF format1 */
#define SMF_FORMAT2             2      /* SMF format2 */

#define SMF_NOTEOFF             0x80   /* ノートオフ */
#define SMF_NOTEON              0x90   /* ノートオン */
#define SMF_POLYPRESS           0xA0   /* キーポリフォニックプレッシャー */
#define SMF_CONTROLCHANGE       0xB0   /* コントロールチェンジ */
#define SMF_PROGRAMCHANGE       0xC0   /* プログラムチェンジ */
#define SMF_CANNELPRESS         0xD0   /* チャンネルプレッシャー */
#define SMF_PITCHBEND           0xE0   /* ピッチベンド */
#define SMF_SYSX                0xF0   /* エクスクルーシブメッセージ */
#define SMF_MTC                 0xF1   /* MIDIタイムコード */
#define SMF_SONGPOSITION        0xF2   /* ソングポジションポインタ */
#define SMF_SONGSELECT          0xF3   /* ソングセレクト */
#define SMF_UNDEFINED_F4        0xF4   /* 未定義 */
#define SMF_UNDEFINED_F5        0xF5   /* 未定義 */
#define SMF_TUNEREQUEST         0xF6   /* チューンリクエスト */
#define SMF_ENDOFSYSX           0xF7   /* エンドオブエクスクルーシブ */
#define SMF_TIMINGCLOCK         0xF8   /* タイミングクロック */
#define SMF_UNDEFINED_F9        0xF9   /* 未定義 */
#define SMF_START               0xFA   /* スタート */
#define SMF_CONTINUE            0xFB   /* コンティニュー */
#define SMF_STOP                0xFC   /* ストップ */
#define SMF_UNDEFINED_FD        0xFD   /* 未定義 */
#define SMF_ACTIVESENSING       0xFE   /* アクティブセンシング */
#define SMF_META                0xFF   /* メタイベント */

#define SMF_META_SEQ_NUM        0x00   /* シーケンス番号 */
#define SMF_META_TEXT           0x01   /* テキストイベント */
#define SMF_META_COPYRIGHT      0x02   /* 著作権表示 */
#define SMF_META_SEQ_NAME       0x03   /* シーケンス名／トラック名 */
#define SMF_META_INST_NAME      0x04   /* 楽器名 */
#define SMF_META_LYRIC          0x05   /* 歌詞 */
#define SMF_META_MARKER         0x06   /* マーカー */
#define SMF_META_QUE            0x07   /* キューポイント */
#define SMF_META_PREFIX         0x20   /* チャンネルプリフィックス */
#define SMF_META_ENDOFTRACK     0x2F   /* エンドオブトラック */
#define SMF_META_TEMPO          0x51   /* セットテンポ */
#define SMF_META_SMPTE          0x54   /* SMPTEオフセット */
#define SMF_META_BEAT           0x58   /* 拍子／メトロノーム設定 */
#define SMF_META_KEY            0x59   /* 調 */
#define SMF_META_OTHER          0x7F   /* シーケンサ固有メタイベント */


/*------------------------------------------------------------------*/
/* 構造体定義                                                       */
/*------------------------------------------------------------------*/

/* SMF のチャンクタグ構造体 */
typedef struct tSMFChunkTag
{
    char    chunkType[4];              /* チャンクタイプ文字列(MThd, MTrk) */
    unsigned long length;              /* チャンクのデータ長 */
}
SMFChunkTag;

/* ヘッダチャンク構造体 */
typedef struct tSMFHeaderChunk
{
    unsigned short format;             /* ファイルフォーマット */
    unsigned short ntracks;            /* トラック数 */
    unsigned short division;           /* 4分音符分解能 */
}
SMFHeaderChunk;


#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif // SMFDEFINE_H_

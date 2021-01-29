/*
  Project:  NitroSDK - demo
  File:     wh_config.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_config.h,v $
  Revision 1.1  2007/10/27 14:55:13  seiki_masashi
  inital upload

  $NoKeywords$
 */

#ifndef WH_CONFIG_H__
#define WH_CONFIG_H__

// 無線で使用するDMA番号
#define WH_DMA_NO                 2

// 子機最大数（親機を含まない数）
#define WH_CHILD_MAX              15

// シェア出来るデータの最大サイズ
#define WH_DS_DATA_SIZE           8

// 1回の通信で送れるデータの最大サイズ
// データシェアリングに加えて通常の通信をする場合は、その分だけ
// ここの値を増やしてください。その際は、複数パケット送信による追加の
// ヘッダフッタ分を加算する必要があります。
// 詳しくは docs/TechnicalNotes/WirelessManager.doc を参照してください。
// GUIDELINE : ガイドライン準拠ポイント(6.3.2)
// リファレンスのワイヤレスマネージャ(WM)→図表・情報→無線通信時間計算シート
// で計算した MP 通信1回分の所要時間が 5600 μ秒以下となることを推奨しています。
#define WH_PARENT_MAX_SIZE      (WH_DS_DATA_SIZE * (1 + WH_CHILD_MAX) + WM_SIZE_DS_PARENT_HEADER)
#define WH_CHILD_MAX_SIZE       (WH_DS_DATA_SIZE)

// 1ピクチャーフレームあたりのMP通信回数上限
// データシェアリングとブロック転送など複数のプロトコルを並行する場合は
// この値を1より大きく(または無制限を示す0に)設定する必要があります。
// そうでない場合、もっとも優先度の高い1つのプロトコル以外は
// MP通信を一切実行できなくなってしまいます。
#define WH_MP_FREQUENCY           1

// 通常の MP 通信で使用するポート
#define WH_DATA_PORT              14

// 通常の MP 通信で使用する優先度
#define WH_DATA_PRIO              WM_PRIORITY_NORMAL

// データシェアリングで使用するポート
#define WH_DS_PORT                12



#endif // WH_CONFIG_H__

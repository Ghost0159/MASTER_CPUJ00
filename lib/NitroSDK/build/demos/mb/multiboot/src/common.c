/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot
  File:     common.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.c,v $
  Revision 1.15  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.14  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.13  2004/10/05 06:47:54  sato_masaki
  設定メニューを追加。その他色々細かな修正。

  Revision 1.12  2004/09/30 03:05:12  sato_masaki
  - 親機の名前をNVRAMから取得するように変更。
  - 初期化させる度に、チャンネルを巡回変更するように変更。

  Revision 1.11  2004/09/14 23:56:23  yosizaki
  change MBGameregistry's member to Unicode.

  Revision 1.10  2004/09/14 02:54:07  miya
  サイン入り

  Revision 1.9  2004/09/13 06:25:40  yosizaki
  change balldemo -> pol_toon

  Revision 1.8  2004/09/09 14:03:31  sato_masaki
  mbライブラリ更新に伴う変更。

  Revision 1.7  2004/09/08 00:23:27  sato_masaki
  MBGameRegistryの登録数を2つに戻す。

  Revision 1.6  2004/08/25 04:28:20  sato_masaki
  MB_CommStartSending関数を実装

  Revision 1.5  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.4  2004/08/19 14:47:18  yosiokat
  MBGameRegistryのゲーム内容説明に改行を付加してみる。

  Revision 1.3  2004/08/19 02:43:19  yosizaki
  change type of MBUserInfo.

  Revision 1.2  2004/08/12 09:57:34  yosiokat
  アイコンデータをファイルパスで指定するよう変更。

  Revision 1.1  2004/08/10 07:31:53  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>


#include "common.h"


/*
 * このデモ全体で使用する共通機能.
 */


/******************************************************************************/
/* constant */

/* このデモがダウンロードさせるプログラム情報の配列 */
const MBGameRegistry mbGameList[GAME_PROG_MAX] = {
    {
     "/em.srl",                        // マルチブート子機バイナリのファイルパス
     L"edgeDemo",                      // ゲーム名
     L"edgemarking demo\ntesttesttest", // ゲーム内容説明
     "/data/icon.char",                // アイコンキャラクタデータのファイルパス
     "/data/icon.plt",                 // アイコンパレットデータのファイルパス
     0x12123434,                       // GameGroupID(GGID)
     16,                               // 最大プレイ人数
     },
    {
     "/pol_toon.srl",
     L"PolToon",
     L"toon rendering",
     "/data/icon.char",
     "/data/icon.plt",
     0x56567878,
     8,
     },
};


/******************************************************************************/
/* variable */

/* MB ライブラリへ割り当てるワーク領域 */
u32     cwork[MB_SYSTEM_BUF_SIZE / sizeof(u32)];

/* このデモのゲームシーケンス状態 */
u8      prog_state;


/******************************************************************************/
/* function */

/* キートリガ検出 */
u16 ReadKeySetTrigger(u16 keyset)
{
    static u16 cont = 0xffff;          /* IPL 内での A ボタン押下対策 */
    u16     trigger = (u16)(keyset & (keyset ^ cont));
    cont = keyset;
    return trigger;
}

/* min - max の範囲でval値を offset分ローテーション */
BOOL RotateU8(u8 *val, u8 min, u8 max, s8 offset)
{
    int     lVal = (int)*val - min;
    int     div = max - min + 1;

    if (div == 0)
        return FALSE;

    lVal = (lVal + offset + div) % div + min;

    *val = (u8)lVal;
    return TRUE;
}

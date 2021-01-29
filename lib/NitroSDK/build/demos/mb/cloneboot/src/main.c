/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - cloneboot
  File:     main.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.3  2004/11/24 11:07:05  yosizaki
  change ParentIdentifier to ParentMain.

  Revision 1.2  2004/11/24 10:17:32  yasu
  Untabify

  Revision 1.1  2004/11/15 06:21:18  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"

/*
 * クローンブートを実現するアプリケーションのサンプル.
 *
 * MBライブラリのサンプルはマルチブート機能を使うため、
 * 同じ通信環境(無線あるいは有線)どうしの開発機材を複数必要とします。
 * $NitroSDK/bin/ARM9-TS/Release/ ディレクトリにある
 * mb_child.bin プログラムが最終実機におけるマルチブート子機と同等の
 * 機能を提供するサンプルになっていますので、
 * このバイナリをサンプルプログラムと同じ方法で他の機材に読み込み、
 * 一緒に実行してください。
 *
 */

/******************************************************************************/



//============================================================================
//   関数定義
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メインルーチン

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /*
     * このサンプルでは, multiboot-Model サンプルデモをそのまま使用し
     * 自身がDSダウンロードプレイ子機プログラムかどうかを判断して
     * 単純に処理を分けています.
     *
     * クローンブートで親機と子機との間にある主な環境相違点としては
     *  1. 子機は CARD アクセスができない
     *  2. 8kB 以内の親機専用コードを含まなければならない
     *  3. ワイヤレス通信の手順が親機/子機で異なる
     * が挙げられます.
     *
     * これらの点に注意してアプリケーション設計に合うラッパー処理で対応すれば
     * ほとんどの内容を親子で共通化しつつ一人用と対戦プレイをサポートし
     * CARD-ROM 容量も節約された効率的なプログラムを作成することが出来ます.
     * 逆に, DSダウンロードプレイの親機と子機の間に全く共通点が無ければ
     * 上に挙げたような利点は得られないでしょう.
     */
    if (!MB_IsMultiBootChild())
    {
        ParentMain();
    }
    else
    {
        ChildMain();
    }

    /* 制御はここへ及びません */
}

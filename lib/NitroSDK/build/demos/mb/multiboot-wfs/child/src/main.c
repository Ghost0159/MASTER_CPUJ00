/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     main.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.1  2005/06/23 09:05:39  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    WBT を駆動して無線マルチブート時の子機がファイルシステムを
	利用するためのサンプルプログラムです。

    HOWTO:
        1. 子機として通信を開始するためにＢボタンを押して下さい。
           周辺に同じ wbt-fs デモの親機が見つかり次第、自動的にその親機と
           通信を開始します。
        2. 接続中のさまざまな機能については、親機 / 子機の画面表示と
           ソースコード内の注釈を参照ください。
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/wbt.h>
#include <nitro/fs.h>

#include    "wfs.h"
#include    "wc.h"

#include    "util.h"
#include    "common.h"


/*---------------------------------------------------------------------------*
    関数定義
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  初期化及びメインループ。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /* サンプル用描画フレームワーク初期化 */
    UTIL_Init();

    /*
     * ファイルシステム初期化.
     * 親機の場合には有効な DMA チャンネルを指定することも可能です.
     * 子機の場合には使用されないので単に無視されます.
     */
    FS_Init(FS_DMA_NOT_USE);

    /* LCD表示開始 */
    GX_DispOn();
    GXS_DispOn();

    /* メインループ */
    for (;;)
    {
        /* 前回の WFS がすでに起動中であれば一度終了 */
        if (WFS_GetStatus() != WFS_STATE_STOP)
        {
            WFS_End();
            WcFinish();
        }

        /* 子機起動処理を開始 */
        ModeSelect();

        /* 子機起動完了まで待機 */
        ModeWorking();

        /* 起動したら子機画面を表示 */
        ModeChild();

        /* エラーならここで停止 */
        ModeError();
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

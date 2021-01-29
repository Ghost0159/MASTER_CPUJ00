/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     main.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"

/*
 * 以下のワイヤレス機能をすべて実現するアプリケーションのサンプル.
 *
 * ・DSダウンロードプレイのクローンブート
 * ・データシェアリング
 * ・ワイヤレス経由のファイルシステム使用
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
/* function */

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メインルーチン

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /* 共通のシステム初期化処理 */
    InitCommon();

    /* 全体の状態遷移 */
    for (;;)
    {
        /* 親機ならDSダウンロードプレイ処理 */
        if (!MB_IsMultiBootChild())
        {
            ExecuteDownloadServer();
        }
        /* ワイヤレスの初期化と親子別の起動処理 */
        if (!MB_IsMultiBootChild())
        {
#if !defined(MBP_USING_MB_EX)
            (void)WH_Initialize();
#endif
            WaitWHState(WH_SYSSTATE_IDLE);
            StartWirelessParent();
        }
        else
        {
            (void)WH_Initialize();
            WaitWHState(WH_SYSSTATE_IDLE);
            StartWirelessChild();
        }
        /* データシェアリングのメイン処理 */
        ExecuteDataSharing();
        (void)WH_End();
        WaitWHState(WH_SYSSTATE_STOP);
    }

    /* 制御はここへ及びません */
    OS_Terminate();
}

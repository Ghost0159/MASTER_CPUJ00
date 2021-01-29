/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - httpclient
  File:     main.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.6  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.5  2006/01/16 01:40:58  kitase_hirotake
  公開用に sitedefs の修正

  Revision 1.4  2006/01/12 00:10:21  kitase_hirotake
  2005->2006 に修正

  Revision 1.3  2006/01/10 04:18:39  kitase_hirotake
  機能をシンプルなものに変更

  Revision 1.2  2006/01/06 00:44:47  kitase_hirotake
  PAD入力対応版
  
  Revision 1.1  2005/12/22 04:34:06  adachi_hiroaki
  HTTPクライアントのデモをSOCで置き換え

  $NoKeywords$
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    http クライアントとして動作するサンプルプログラムです。
    PADでURLを入力することでサーバへと接続します。

    PADでの文字入力の方法:
        1. 文字入力はPADを用いて行います。
           Lボタンを押すごとにDIRECT_MODEとINDIRECT_MODEが切り替わります。
           文字入力はINDIRECT_MODEで行います。
        2. 上下キーで選択中の文字を変えます。
           Rボタンで文字の種類が数字、大文字、小文字、記号と変わります。
           Aボタンでカレントの文字を決定し、次の文字の入力に移ります。
           左右キーで今までに入力した文字にカレントを移動し、変更ができます。
        3. Bボタンで選択中の文字を消します。
        4. Xボタンで今までに入力した文字列を送信します。
    
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include <string.h>
#include <sitedefs.h>

#include "padkeyboard.h"
#include "output.h"
#include "screen.h"

#include "netconnect.h"
#include "http.h"
#include "ssl.h"

#define SITEDEFS_APCLASS    "WiFi.LAN.1.AP.1"

#define URL_LENGTH          64

static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void NitroMain()
{
    char    url[URL_LENGTH];

    /* 初期化関数,接続関数呼び出し */
    NcGlobalInit();
    NcStart(SITEDEFS_APCLASS);

    /* スクリーンの初期化 */
    InitScreen();

    FlushScreen();
    FlushDebugOutput();

    /* キー入力情報取得の空呼び出し(IPL での A ボタン押下対策) */
    (void)PAD_Read();

    /* キーボードの初期化 */
    InitKeyboardThread();
    KeyboardValid();

    /* HTTP server への接続             *
     * Pad で接続先の URL を入力 */
    DebugOutput("http_server_ip>");

    /* 初期値のURLはルータの管理画面 */
    MI_CpuClear8(url, URL_LENGTH);
    {
        SOCInAddr   addr;
        SOC_GetGateway(NULL, (u8*) &addr);
        (void)OS_SNPrintf(url, sizeof(url), "http://%s/", SOC_InetNtoA(addr));
    }

    while (1)
    {
        PadKeySetString(url);

        /* URL の入力をする */
        PadKeyGetInputString(url, URL_LENGTH);

        {
            static char         pageBuffer[1024 * 20];
            int         socket = -1;
            SOCSslConnection*   sslCon;

            SslInit();
            socket = HttpConnect(url, &sslCon);

            OS_TPrintf("URL: %s\n", url);

            if (socket < 0)
            {
                OS_Panic("failed to connect.");
            }

            if (HttpGet(socket, url, NULL, NULL, 0, pageBuffer, sizeof(pageBuffer)) < 0)
            {
                OS_Panic("failed to download\n");
            }

            OS_PutString(pageBuffer);
            (void)HttpClose(socket, sslCon);
        }
    }

    NcFinish();
    OS_Terminate();
}

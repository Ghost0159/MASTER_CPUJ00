/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WVR - demos
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.6  2005/11/21 11:08:27  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.5  2005/02/28 05:26:23  yosizaki
  do-indent.

  Revision 1.4  2005/02/18 12:08:32  seiki_masashi
  warning 対策

  Revision 1.3  2005/02/16 10:39:42  terui
  wvrのincludeヘッダ表記を簡略化。

  Revision 1.2  2005/02/09 09:23:21  terui
  WVR_StartUp呼び出し時にVRAMのIDを明示的に指定するように修正。

  Revision 1.1  2005/01/14 06:25:50  terui
  tests以下からdemos以下に移動。

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/mb.h>

#include "common.h"
#include "disp.h"
#include "gmain.h"
#include "wh.h"

#include <nitro/wvr.h>

//============================================================================
//  プロトタイプ宣言
//============================================================================

static void ModeConnect(void);         // 親機への接続開始
static void ModeError(void);           // エラー表示画面
static void ModeWorking(void);         // ビジー画面
static void ChildReceiveCallback(WMmpRecvBuf *data);

static void StartUpCallback(void *arg, WVRResult result);


//============================================================================
//  変数定義
//============================================================================

static u32 gFrame;                     // フレームカウンタ

static WMBssDesc gMBParentBssDesc ATTRIBUTE_ALIGN(32);

static volatile u8 startCheck;


//============================================================================
//  関数定義
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  子機メインルーチン

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{

    // 画面、OSの初期化
    CommonInit();

    // 自分がマルチブートから起動した子機であるかどうかをチェックします。
    if (!MB_IsMultiBootChild())
    {
        OS_Panic("not found Multiboot child flag!\n");
    }

    //--------------------------------------------------------------
    // マルチブートで起動した場合、一旦リセットされ通信が切断されます。
    // ブート後もブートした親機のBssDescを保持しているため、この情報を使って
    // 親機へ再接続してください。
    // この時、BssDescからMACアドレスのみを取り出してMACアドレス指定で
    // 親のスキャンから接続を行なう場合は特に問題ありませんが、保持されている
    // BssDescを使って直に親機に接続を行なう場合には、通信サイズや転送モードを
    // あらかじめ親子の間で合わせて設定しておく必要があります。
    //--------------------------------------------------------------

    /* 
     * 親機と再接続するために親機の情報を取得します。                   
     * 接続に利用するWMBssDescは32バイトにアラインされている必要があります。
     * 親機のMACアドレスで再スキャンすることなしに再接続させる場合は、
     * 親機/子機の最大送信サイズ、KS/CSフラグはあらかじめ合わせておいてください。
     * 再スキャンを行なってから接続する場合はこれらの値はすべて0で構いません。
     */
    MB_ReadMultiBootParentBssDesc(&gMBParentBssDesc, WH_PARENT_MAX_SIZE,        // 親機最大送信サイズ
                                  WH_CHILD_MAX_SIZE,    // 子機最大送信サイズ
                                  0,   // キーシェアリングフラグ
                                  0);  // 連続転送モードフラグ

    // 親を再スキャンすること無しに、接続する場合には親機と子機でtgidを合わせる
    // 必要があります。
    // 親機は再起動後に無関係のIPLからの接続されるのを避ける為にtgidを変更し、
    // 子機側もそれに合わせてtgidを変更する必要があります。
    // このデモでは親機、子機ともにtgidを1インクリメントしています。
    gMBParentBssDesc.gameInfo.tgid++;

    // 画面初期化
    DispInit();
    // ヒープの初期化
    InitAllocateSystem();

    // 割り込み有効
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    /*================================================================*/
    // 無線ライブラリ駆動開始
    startCheck = 0;
    if (WVR_RESULT_OPERATING != WVR_StartUpAsync(GX_VRAM_ARM7_128_D, StartUpCallback, NULL))
    {
        OS_TPanic("WVR_StartUpAsync failed. \n");
    }
    while (!startCheck)
    {
    }
    /*================================================================*/

    GInitDataShare();

    //********************************
    // 無線初期化
    (void)WH_Initialize();
    //********************************

    // LCD表示開始
    GX_DispOn();
    GXS_DispOn();

    // デバッグ文字列出力
    OS_TPrintf("MB child: Simple DataSharing demo started.\n");

    // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
    ReadKey();

    // メインループ
    for (gFrame = 0; TRUE; gFrame++)
    {
        // キー入力情報取得
        ReadKey();

        // スクリーンクリア
        BgClear();

        // 通信状態により処理を振り分け
        switch (WH_GetSystemState())
        {
        case WH_SYSSTATE_CONNECT_FAIL:
            {
                // WM_StartConnect()に失敗した場合にはWM内部のステートが不正になっている為
                // 一度WM_ResetでIDLEステートにリセットする必要があります。
                WH_Reset();
            }
            break;
        case WH_SYSSTATE_IDLE:
            {
                static  retry = 0;
                enum
                {
                    MAX_RETRY = 5
                };

                if (retry < MAX_RETRY)
                {
                    ModeConnect();
                    retry++;
                    break;
                }
                // MAX_RETRYで親機に接続できなければERROR表示
            }
        case WH_SYSSTATE_ERROR:
            ModeError();
            break;
        case WH_SYSSTATE_BUSY:
        case WH_SYSSTATE_SCANNING:
            ModeWorking();
            break;

        case WH_SYSSTATE_CONNECTED:
        case WH_SYSSTATE_KEYSHARING:
        case WH_SYSSTATE_DATASHARING:
            {
                BgPutString(8, 1, 0x2, "Child mode");
                GStepDataShare(gFrame);
                GMain();
            }
            break;
        }

        // 電波受信強度の表示
        {
            int     level;
            level = WH_GetLinkLevel();
            BgPrintStr(31, 23, 0xf, "%d", level);
        }

        // Ｖブランク待ち
        OS_WaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  Name:         StartUpCallback

  Description:  無線駆動制御ライブラリの非同期的な処理終了が通知される
                コールバック関数。

  Arguments:    arg     -   WVR_StartUpAsyncコール時に指定した引数。未使用。
                result  -   非同期関数の処理結果。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void StartUpCallback(void *arg, WVRResult result)
{
#pragma unused( arg )

    if (result != WVR_RESULT_SUCCESS)
    {
        OS_TPanic("WVR_StartUpAsync error.[%08xh]\n", result);
    }
    startCheck = 1;
}

/*---------------------------------------------------------------------------*
  Name:         ModeConnect

  Description:  接続開始

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
static void ModeConnect(void)
{
#define USE_DIRECT_CONNECT

    // 親機の再スキャンなしに直接接続する場合。
#ifdef USE_DIRECT_CONNECT
    //********************************
    (void)WH_ChildConnect(WH_CONNECTMODE_DS_CHILD, &gMBParentBssDesc);
    // WH_ChildConnect(WH_CONNECTMODE_MP_CHILD, &gMBParentBssDesc, TRUE);
    // WH_ChildConnect(WH_CONNECTMODE_KS_CHILD, &gMBParentBssDesc, TRUE);
    //********************************
#else
    WH_SetGgid(gMBParentBssDesc.gameInfo.ggid);
    // 親機の再スキャンを実行する場合。
    //********************************
    (void)WH_ChildConnectAuto(WH_CONNECTMODE_DS_CHILD, gMBParentBssDesc.bssid,
                              gMBParentBssDesc.channel);
    // WH_ChildConnect(WH_CONNECTMODE_MP_CHILD, &gMBParentBssDesc, TRUE);
    // WH_ChildConnect(WH_CONNECTMODE_KS_CHILD, &gMBParentBssDesc, TRUE);
    //********************************
#endif
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  エラー通知画面表示

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
static void ModeError(void)
{
    BgPrintStr(5, 10, 0x1, "======= ERROR! =======");

    if (WH_GetLastError() == WM_ERRCODE_OVER_MAX_ENTRY)
    {
        BgPrintStr(5, 13, 0xf, "OVER_MAX_ENTRY");
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  処理中画面を表示

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
static void ModeWorking(void)
{
    BgPrintStr(9, 11, 0xf, "Now working...");

    if (IS_PAD_TRIGGER(PAD_BUTTON_START))
    {
        //********************************
        (void)WH_Finalize();
        //********************************
    }
}

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.20  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.19  2005/11/21 11:06:11  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.18  2005/06/10 06:31:30  yosizaki
  fix around ModeSelect().

  Revision 1.17  2005/05/09 04:49:23  yosizaki
  fix about call of WFS_End().

  Revision 1.16  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.15  2004/10/31 23:53:26  yosizaki
  delete WC_DEFAULT_CHANNEL (abandoned symbol)

  Revision 1.14  2004/10/28 13:10:48  yosizaki
  change to use block-transfer callback of wc.

  Revision 1.13  2004/10/25 10:20:52  yosizaki
  move to parent.c and child.c

  Revision 1.12  2004/10/05 10:22:36  yosizaki
  change packet-size within 5600[us].

  Revision 1.11  2004/09/29 07:27:52  yosizaki
  change WFS_Init.

  Revision 1.10  2004/09/29 07:02:00  yosizaki
  merge from branch.

  Revision 1.9  2004/09/15 09:10:57  yosizaki
  add Seek&Read.
  add some sample test.

  Revision 1.8  2004/09/09 14:09:03  yosizaki
  add sample of WFS_GetCurrentDownloadProgress().

  Revision 1.7  2004/09/09 07:38:31  yosizaki
  fix timing of FS_LoadTable.

  Revision 1.6  2004/09/09 05:05:01  yosizaki
  total reform.

  Revision 1.5  2004/09/06 08:46:12  yosizaki
  add routine to check result of FS_ReadFile().

  Revision 1.4  2004/09/06 06:54:09  yosizaki
  small change. (debug output)

  Revision 1.3  2004/09/04 13:26:58  yosizaki
  add test ofoverlay.

  Revision 1.2  2004/09/04 01:45:41  yosizaki
  add some debug-prints.

  Revision 1.1  2004/09/03 01:11:58  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    WBT を駆動して無線マルチブート時の子機がファイルシステムを
	利用するためのサンプルプログラムです。

    HOWTO:
        1. 親機として通信を開始する場合はＡボタンを押して下さい。
           周辺に同じ wbt-fs デモの子機が見つかり次第、自動的にその子機と
           通信を開始します。同時に通信できる子機は15台までです。
        2. 子機として通信を開始する場合はＢボタンを押して下さい。
           周辺に同じ wbt-fs デモの親機が見つかり次第、自動的にその親機と
           通信を開始します。
        3. 接続中のさまざまな機能については、親機 / 子機の画面表示と
           ソースコード内の注釈を参照ください。
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/wbt.h>
#include <nitro/fs.h>

#include    "wfs.h"

#include    "util.h"
#include    "wc.h"
#include    "func.h"
#include    "common.h"



/*---------------------------------------------------------------------------*
    内部定数定義
 *---------------------------------------------------------------------------*/

/* 無線通信パラメータ */

#define		WC_PARENT_DATA_SIZE_MAX		128
#define		WC_CHILD_DATA_SIZE_MAX		256

#if	!defined(SDK_MAKEGGID_SYSTEM)
#define SDK_MAKEGGID_SYSTEM(num)    (0x003FFF00 | (num))
#endif
#define GGID_WBT_FS                 SDK_MAKEGGID_SYSTEM(0x31)

// ゲーム情報
const WMParentParam def_parent_param[1] = {
    {
     NULL, 0, 0,
     GGID_WBT_FS,
     0x0000,                           // temporary ID (毎回+1される)
     1,                                // エントリー許可フラグ
     15,                               // 最大接続子機数
     0,                                // マルチブートフラグ
     0,                                // キーシェアリング
     1,                                // 連続送信フラグ
     200,                              // beacon 間隔
     {0, 0, 0, 0},                     // ユーザー名
     {0, 0, 0, 0, 0, 0, 0, 0},         // ゲーム名
     1,                                // 接続チャンネル (起動時に MeasureChannelする)
     WC_PARENT_DATA_SIZE_MAX,          // 親機送信データサイズ
     WC_CHILD_DATA_SIZE_MAX / 16       // 子機送信データサイズ
     // Reserves
     }
};


/* 画面遷移モード */
enum
{
    MODE_SELECT,
    MODE_ERROR,
    MODE_BUSY,
    MODE_PARENT,
    MODE_CHILD,
    MODE_MAX
};


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/

/* 一度でも子機側を選択すると FS を置き換えるので親機にはなれません */
static BOOL is_child = FALSE;


/*---------------------------------------------------------------------------*
    内部関数定義
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
  Name:         StateCallbackForWFS

  Description:  WFS のコールバック関数.
                状態が WFS_STATE_READY になった時点で呼び出されます.
                このコールバックで通知を受けず, 単に任意の位置で
                WFS_GetStatus() を使用して判定することも可能です.

  Arguments:    arg       コールバックに指定したユーザ定義引数.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void StateCallbackForWFS(void *arg)
{
    (void)arg;
    switch (WFS_GetStatus())
    {
    case WFS_STATE_READY:
        OS_TPrintf("WFS ready.\n");
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         AllocatorForWFS

  Description:  WFS に指定するメモリの動的確保関数.

  Arguments:    arg       アロケータに指定したユーザ定義引数.
                size      メモリ確保要求ならば必要なサイズ.
                ptr       NULL の場合はメモリ確保, そうでなければ解放要求.

  Returns:      ptr が NULL なら size 分のメモリを確保してポインタを返すこと.
                そうでなければ ptr のメモリを解放すること.
                解放の場合, 返り値は単に無視される.
 *---------------------------------------------------------------------------*/
static void *AllocatorForWFS(void *arg, u32 size, void *ptr)
{
    (void)arg;
    if (!ptr)
        return OS_Alloc(size);
    else
    {
        OS_Free(ptr);
        return NULL;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WCCallbackForWFS

  Description:  WC の無線自動駆動から状態通知を受けるコールバック.

  Arguments:    arg     - 通知元 WM 関数のコールバックポインタ.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WCCallbackForWFS(WMCallback *arg)
{
    WCStatus wcStatus = WcGetStatus();
    switch (arg->apiid)
    {
    case WM_APIID_START_MP:
        {                              /* MP ステート開始 */
            WMStartMPCallback *cb = (WMStartMPCallback *)arg;
            switch (cb->state)
            {
            case WM_STATECODE_MP_START:
                /*
                 * MP_PARENT または MP_CHILD ステートへの移行を通知するために
                 * WFS_Start() を呼び出してください.
                 * この通知を契機に, WFS はブロック転送のために
                 * 内部で WM_SetMPDataToPort() 関数を呼び出し始めます.
                 */
                WFS_Start();
                break;
            }
        }
        break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  初期化及びメインループ。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /* 各種初期化 */
    OS_Init();
    OS_InitTick();

    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

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

    {                                  /* メインループ */

        static KeyInfo key[1];
        int     frame_count;

        int     cur_mode = MODE_MAX;

        KeyRead(key);
        for (frame_count = 0;; ++frame_count)
        {
            KeyRead(key);

            ClearString();
            PrintString(1, 22, COLOR_WHITE, "frame:%d", frame_count);

            {                          /* 画面の遷移判定 */
                int     new_mode = MODE_ERROR;
                switch (WcGetStatus())
                {
                case WC_STATUS_READY:
                    {
                        /* 前回の WFS がすでに起動中であれば一度終了 */
                        if (WFS_GetStatus() != WFS_STATE_STOP)
                        {
                            WFS_End();
                            WcFinish();
                            /* ここで WM, WC, WFS 全て初期化前状態に遷移完了 */
                        }
                    }
                    new_mode = MODE_SELECT;
                    break;
                case WC_STATUS_BUSY:
                    new_mode = MODE_BUSY;
                    break;
                case WC_STATUS_PARENT:
                    new_mode = MODE_PARENT;
                    break;
                case WC_STATUS_CHILD:
                    new_mode = MODE_CHILD;
                    break;
                case WC_STATUS_ERROR:
                    new_mode = MODE_ERROR;
                    break;
                }
                if (cur_mode != new_mode)
                {
                    cur_mode = new_mode;
                    ClearLine();
                }
            }

            {                          /* 画面処理 */
                switch (cur_mode)
                {
                case MODE_SELECT:
                    ModeSelect(key);
                    break;
                case MODE_ERROR:
                    ModeError(key);
                    break;
                case MODE_BUSY:
                    ModeWorking(key);
                    break;
                case MODE_PARENT:
                    ModeParent(key);
                    break;
                case MODE_CHILD:
                    ModeChild(key);
                    break;
                }
                FlushLine();
            }

            OS_WaitVBlankIntr();
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         ModeSelect

  Description:  親機/子機 選択画面での処理。

  Arguments:    key         キー入力.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeSelect(const KeyInfo * p_key)
{
    if (!is_child)
    {
        PrintString(3, 10, COLOR_WHITE, "Press A to connect as PARENT");
    }
    PrintString(3, 12, COLOR_WHITE, "Press B to connect as CHILD");

    /*
     * WFS が WM_SetPortCallback() を利用して駆動するため,
     * WM_Initialize() の後に WFS_Init() を呼び出して下さい.
     */
    if (!is_child && (p_key->trg & PAD_BUTTON_A))
    {
        /*
         * 親機である場合のみ, テーブルロードは効果的です.
         * 子機は WFS の中で動的確保されたメモリ上に
         * 親機側のファイルシステムテーブル情報を保持するため,
         * これをさらにテーブルロードすることに意味はありません.
         */
        static BOOL table_loaded = FALSE;
        if (!table_loaded)
        {
            u32     need_size = FS_GetTableSize();
            void   *p_table = OS_Alloc(need_size);
            SDK_ASSERT(p_table != NULL);
            table_loaded = FS_LoadTable(p_table, need_size);
            if (!table_loaded && p_table)
            {
                OS_Free(p_table);
            }
        }
        WcInit(def_parent_param, FALSE, /* data-sharing is not used */
               TRUE,                   /* block-transfer is used */
               1 /* non care in this demo (data-sharing frame period) */ );
        WcSetBlockTransferCallback(WCCallbackForWFS);
        WcStartParent();
        /*
         * 親機はデフォルトで以下の設定から起動しています.
         * (これを変更したい場合は任意のタイミングで設定可能です)
         */
        WFS_InitParent(port_wbt, StateCallbackForWFS, AllocatorForWFS, NULL,
                       parent_packet_max, NULL, TRUE);
        WFS_EnableSync(0);
    }
    else if (p_key->trg & PAD_BUTTON_B)
    {
        is_child = TRUE;
        WcInit(def_parent_param, FALSE, /* data-sharing is not used */
               TRUE,                   /* block-transfer is used */
               1 /* non care in this demo (data-sharing frame period) */ );
        WcSetBlockTransferCallback(WCCallbackForWFS);
        WcStartChild();
        WFS_InitChild(port_wbt, StateCallbackForWFS, AllocatorForWFS, NULL);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  エラー表示画面での処理。

  Arguments:    key         キー入力.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeError(const KeyInfo * p_key)
{
    PrintString(5, 10, COLOR_RED, "======= ERROR! =======");
    PrintString(5, 13, COLOR_WHITE, " Fatal error occured.");
    PrintString(5, 14, COLOR_WHITE, "Please reboot program.");

    (void)p_key;
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  ビジー画面での処理。

  Arguments:    key         キー入力.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeWorking(const KeyInfo * p_key)
{
    PrintString(9, 11, COLOR_WHITE, "Now working...");

    if (p_key->trg & PAD_BUTTON_START)
    {
        WcEnd();
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

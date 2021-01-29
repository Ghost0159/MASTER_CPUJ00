/*---------------------------------------------------------------------------*
  Project:  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     common.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.c,v $
  Revision 1.2  2005/11/21 10:52:47  kitase_hirotake
  OS_PrintServer の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.1  2005/06/23 09:05:57  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    "common.h"
#include    "wfs.h"
#include    "wc.h"


/*****************************************************************************/
/* constant */

/* ゲーム情報 */
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
     }
};


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         StateCallbackForWFS

  Description:  WFS のコールバック関数.
                状態が WFS_STATE_READY になった時点で呼び出されます.
                このコールバックで通知を受けず, 単に任意の位置で
                WFS_GetStatus() を使用して判定することも可能です.

  Arguments:    arg       コールバックに指定したユーザ定義引数.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void StateCallbackForWFS(void *arg)
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
void   *AllocatorForWFS(void *arg, u32 size, void *ptr)
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
void WCCallbackForWFS(WMCallback *arg)
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
  Name:         InitFrameLoop

  Description:  ゲームフレームループの初期化処理。

  Arguments:    p_key           初期化するキー情報構造体.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitFrameLoop(KeyInfo * p_key)
{
    ClearLine();
    ClearString();
    KeyRead(p_key);
}

/*---------------------------------------------------------------------------*
  Name:         WaitForNextFrame

  Description:  次の描画フレームまで待機。

  Arguments:    p_key           更新するキー情報構造体.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WaitForNextFrame(KeyInfo * p_key)
{
    FlushLine();
    OS_WaitVBlankIntr();
    ClearString();
    KeyRead(p_key);
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  エラー表示画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeError(void)
{
    KeyInfo key[1];

    InitFrameLoop(key);
    while (WcGetStatus() == MODE_ERROR)
    {
        WaitForNextFrame(key);
        PrintString(5, 10, COLOR_RED, "======= ERROR! =======");
        PrintString(5, 13, COLOR_WHITE, " Fatal error occured.");
        PrintString(5, 14, COLOR_WHITE, "Please reboot program.");
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  ビジー画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeWorking(void)
{
    KeyInfo key[1];

    InitFrameLoop(key);
    while (WcGetStatus() == MODE_BUSY)
    {
        WaitForNextFrame(key);
        PrintString(9, 11, COLOR_WHITE, "Now working...");
        if (key->trg & PAD_BUTTON_START)
        {
            WcEnd();
        }
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

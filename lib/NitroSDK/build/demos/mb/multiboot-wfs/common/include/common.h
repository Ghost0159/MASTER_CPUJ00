/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     common.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.h,v $
  Revision 1.4  2005/10/20 07:49:56  yosizaki
  fix port number.

  Revision 1.3  2005/10/03 09:54:20  yosizaki
  fix port number.

  Revision 1.2  2005/08/09 13:55:24  yosizaki
  fix GGID.

  Revision 1.1  2005/06/23 09:05:57  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef __NITROSDK_DEMO_MB_MULTIBOOT_WFS_COMMON_H
#define __NITROSDK_DEMO_MB_MULTIBOOT_WFS_COMMON_H


#include <nitro.h>
#include "util.h"


/******************************************************************************/
/* constant */

/* 無線通信パラメータ */
#define		WC_PARENT_DATA_SIZE_MAX		128
#define		WC_CHILD_DATA_SIZE_MAX		256

#if	!defined(SDK_MAKEGGID_SYSTEM)
#define SDK_MAKEGGID_SYSTEM(num)    (0x003FFF00 | (num))
#endif
#define GGID_WBT_FS                 SDK_MAKEGGID_SYSTEM(0x23)

extern const WMParentParam def_parent_param[1];


/* WFS 用の通信設定 */
#define	port_wbt            5
#define	parent_packet_max   WC_PARENT_DATA_SIZE_MAX


/* 画面遷移モード */
enum
{
    MODE_SELECT,                       /* 起動オプション選択 */
    MODE_ERROR,                        /* エラー発生による停止 */
    MODE_BUSY,                         /* 起動処理中 */
    MODE_PARENT,                       /* 親機処理実行中 */
    MODE_CHILD,                        /* 子機処理実行中 */
    MODE_MAX
};


/******************************************************************************/
/* function */

#if	defined(__cplusplus)
extern  "C"
{
#endif

/*---------------------------------------------------------------------------*
  Name:         StateCallbackForWFS

  Description:  WFS のコールバック関数.
                状態が WFS_STATE_READY になった時点で呼び出されます.
                このコールバックで通知を受けず, 単に任意の位置で
                WFS_GetStatus() を使用して判定することも可能です.

  Arguments:    arg       コールバックに指定したユーザ定義引数.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    StateCallbackForWFS(void *arg);

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
    void   *AllocatorForWFS(void *arg, u32 size, void *ptr);

/*---------------------------------------------------------------------------*
  Name:         WCCallbackForWFS

  Description:  WC の無線自動駆動から状態通知を受けるコールバック.

  Arguments:    arg     - 通知元 WM 関数のコールバックポインタ.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    WCCallbackForWFS(WMCallback *arg);

/*---------------------------------------------------------------------------*
  Name:         InitFrameLoop

  Description:  ゲームフレームループの初期化処理。

  Arguments:    p_key           初期化するキー情報構造体.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    InitFrameLoop(KeyInfo * p_key);

/*---------------------------------------------------------------------------*
  Name:         WaitForNextFrame

  Description:  次の描画フレームまで待機。

  Arguments:    p_key           更新するキー情報構造体.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    WaitForNextFrame(KeyInfo * p_key);

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  エラー表示画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeError(void);

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  ビジー画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeWorking(void);

/*---------------------------------------------------------------------------*
  Name:         ModeSelect

  Description:  親機/子機 選択画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeSelect(void);

/*---------------------------------------------------------------------------*
  Name:         ModeMultiboot

  Description:  DSダウンロードプレイ親機処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeMultiboot(void);

/*---------------------------------------------------------------------------*
  Name:         ModeParent

  Description:  親機 通信画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeParent(void);

/*---------------------------------------------------------------------------*
  Name:         ModeChild

  Description:  子機 通信画面。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeChild(void);


#if	defined(__cplusplus)
}                                      /* extern "C" */
#endif


/******************************************************************************/


#endif                                 /* __NITROSDK_DEMO_WBT_WBT_FS_COMMON_H */

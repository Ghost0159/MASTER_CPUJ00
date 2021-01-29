/*---------------------------------------------------------------------------*
  Project:  NitroSDK - wireless_shared - demos - wfs
  File:     wfs.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wfs.h,v $
  Revision 1.5  2006/05/19 06:42:06  yosizaki
  fix source indent.

  Revision 1.4  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.3  2005/05/09 04:30:58  yosizaki
  add comment of WFS_End().

  Revision 1.2  2005/02/28 05:26:34  yosizaki
  do-indent.

  Revision 1.1  2004/11/16 01:13:04  yosizaki
  moved from /demos/wbt/wbt-fs.


 -[old log at /demos/wbt/wbt-fs/wfs]-

  Revision 1.9  2004/10/25 04:20:14  yosizaki
  add WFS_InitParent and WFS_InitChild.
  add some getter functions.

  Revision 1.8  2004/09/29 07:28:03  yosizaki
  change WFS_Init.

  Revision 1.7  2004/09/29 07:10:11  yosizaki
  merge from branch.

  Revision 1.6  2004/09/15 01:11:12  yosizaki
  add WFS_SetPacketSize().

  Revision 1.5  2004/09/11 04:13:35  yosizaki
  add comments.
  unify MP-size definition.

  Revision 1.4  2004/09/04 13:25:40  yosizaki
  add WFS_GetCurrentDownloadProgress.

  Revision 1.3  2004/09/04 01:45:14  yosizaki
  delete obsolete codes and waste parameters.

  Revision 1.2  2004/09/03 10:10:04  yosizaki
  enable WFS_OBSOLETE.

  Revision 1.1  2004/09/03 01:12:10  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	NITRO_BUILD_DEMOS_WIRELESSSHARED_WFS_INCLUDE_WFS_H_
#define	NITRO_BUILD_DEMOS_WIRELESSSHARED_WFS_INCLUDE_WFS_H_


#include <nitro.h>

#ifdef __cplusplus
extern  "C" {
#endif


/*****************************************************************************/
/* constant */


/*
 * WFS の内部状態を示す列挙型です.
 */
typedef enum
{
    WFS_STATE_STOP,
    WFS_STATE_IDLE,
    WFS_STATE_READY,
    WFS_STATE_ERROR
}
WFSState;


/*****************************************************************************/
/* prototype */

/*
 * WFS の内部状態が変わるごとに呼び出されるコールバックです.
 * WFS_STATE_READY が返った時点で自動的に FS が利用可能となります.
 */
typedef void (*WFSStateCallback) (void *);


/*
 * WFS 内部の動的メモリ割り当てコールバックです.
 * arg はユーザ定義の引数です.
 * ptr が NULL なら size バイトのメモリを返してください.
 * ptr が NULL でないなら ptr を解放してください.
 */
typedef void *(*WFSAllocator) (void *arg, u32 size, void *ptr);


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         WFS_InitParent

  Description:  無線ファイルシステム(WFS)を親機として初期化します.
                WM ライブラリへポートコールバックを設定して自動駆動するので,
                必ず WM の READY ステート以上で呼び出す必要があります.

  Arguments:    port             内部での MP 通信に使用させるポート番号.
                callback         各種状態通知を受けるコールバック.
                allocator        内部でのメモリ確保に使用させるコールバック.
                                 WFS の使用メモリ量はアプリケーションごとに
                                 実行時に決定されます.
                allocator_arg    allocator に与えるユーザ定義の引数.
                parent_packet    初期設定する親機側送信サイズ.
                                 この値は WBT_PACKET_SIZE_MIN 以上かつ
                                 親機最大送信サイズ以下である必要があります.
                                 子機の場合は単に無視されます.
                p_rom            子機に対して提供する FAT/FNT/OVT を含む
                                 プログラムを指すファイルポインタ.
                                 これは通常, ワイヤレスダウンロードで送信した
                                 子機プログラムそのものを指定します.
                                 シーク位置は呼び出し前の位置を維持します.
                                 この引数が NULL の場合, 親機自身の
                                 ファイルシステムを提供します.
                use_parent_fs    TRUE の場合, p_rom で指定したプログラムでなく
                                 親機自身の FAT/FNT を提供します.
                                 この設定により, 独立した親子プログラムで
                                 単一ファイルシステムの共有が可能です.
                                 p_rom が NULL であればこの設定は無視され,
                                 常に共有扱いとなります.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_InitParent(int port, WFSStateCallback callback,
                       WFSAllocator allocator, void *allocator_arg, int parent_packet,
                       FSFile *p_rom, BOOL use_parent_fs);

/*---------------------------------------------------------------------------*
  Name:         WFS_InitChild

  Description:  無線ファイルシステム(WFS)を子機として初期化します.
                WM ライブラリへポートコールバックを設定して自動駆動するので,
                必ず WM の READY ステート以上で呼び出す必要があります.

  Arguments:    port             内部での MP 通信に使用させるポート番号.
                callback         各種状態通知を受けるコールバック.
                allocator        内部でのメモリ確保に使用させるコールバック.
                                 WFS の使用メモリ量はアプリケーションごとに
                                 実行時に決定されます.
                allocator_arg    allocator に与えるユーザ定義の引数.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_InitChild(int port, WFSStateCallback callback,
                      WFSAllocator allocator, void *allocator_arg);

/*---------------------------------------------------------------------------*
  Name:         WFS_Init

  Description:  無線ファイルシステム(WFS)を初期化します.
                親機または子機としての内部状態を初期化した後,
                WM ライブラリへポートコールバックを設定して自動駆動するので,
                必ず WM の READY ステート以上で呼び出す必要があります.

  Arguments:    is_parent        無線親機なら TRUE, 子なら FALSE.
                                 実際のマルチブートではこの引数には
                                 !MB_IsMultiBootChild() を与えます.
                port             内部での MP 通信に使用させるポート番号.
                parent_packet    初期設定する親機側送信サイズ.
                                 この値は WBT_PACKET_SIZE_MIN 以上かつ
                                 親機最大送信サイズ以下である必要があります.
                                 子機の場合は単に無視されます.
                callback         各種状態通知を受けるコールバック.
                allocator        内部でのメモリ確保に使用させるコールバック.
                                 WFS の使用メモリ量はアプリケーションごとに
                                 実行時に決定されます.
                allocator_arg    allocator に与えるユーザ定義の引数.

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void WFS_Init(BOOL is_parent, int port, int parent_packet,
                         WFSStateCallback callback, WFSAllocator allocator, void *allocator_arg)
{
    if (is_parent)
        WFS_InitParent(port, callback, allocator, allocator_arg, parent_packet, NULL, TRUE);
    else
        WFS_InitChild(port, callback, allocator, allocator_arg);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_Start

  Description:  WFS に対し, 無線が MP ステートになったことを通知します.
                WFS はこの通知以降, WM_SetMPDataToPort() を使用して
               「優先度:低 (WM_PRIORITY_LOW) 」で自動駆動します.
                そのため, 必ず MP_PARENT または MP_CHILD ステートで
                呼び出す必要があります.
                通常は WM_StartMP() のコールバックで state の要因種別に
                WM_STATECODE_MP_START が渡された後に呼び出します.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_Start(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_End

  Description:  WFS が不要になった時点で呼び出します.
                内部で確保したメモリを全て解放し, 初期化前の状態に戻ります.
                通常は, すべての無線通信が切断された時点で呼び出します.
                この関数を割り込みハンドラから呼び出すことはできません.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_End(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetStatus

  Description:  WFS の現在の内部状態を WFSState 型で取得します.

  Arguments:    None.

  Returns:      WFSState 型の列挙値のいずれか.
 *---------------------------------------------------------------------------*/
WFSState WFS_GetStatus(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetCurrentBitmap

  Description:  WBT が認識している現在の子機群のビットマップを取得します.
                この関数は親機側のみ呼び出すことができます.

  Arguments:    None.

  Returns:      現在認識されている子機群.
 *---------------------------------------------------------------------------*/
int     WFS_GetCurrentBitmap(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetSyncBitmap

  Description:  アクセス同期を指定された子機群のビットマップを取得します.
                この関数は親機側のみ呼び出すことができます.

  Arguments:    None.

  Returns:      アクセス同期を取るよう指定された子機群.
 *---------------------------------------------------------------------------*/
int     WFS_GetSyncBitmap(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetBusyBitmap

  Description:  現在親機にアクセス中の子機群のビットマップを取得します.
                この関数は親機側のみ呼び出すことができます.

  Arguments:    None.

  Returns:      現在親機にアクセス中の子機群.
 *---------------------------------------------------------------------------*/
int     WFS_GetBusyBitmap(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_IsBusy

  Description:  指定した aid の子機が現在アクセス中か判定します.
                この関数は親機側のみ呼び出すことができます.

  Arguments:    aid              判定する子機の aid.

  Returns:      指定の子機がアクセス中なら TRUE, そうでなければ FALSE.
 *---------------------------------------------------------------------------*/
BOOL    WFS_IsBusy(int aid);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetCurrentDownloadProgress

  Description:  子機側が現在 ReadFile しているファイルの進捗を取得します.
                単位は WBT 内部のシーケンス番号となります.
                この関数は子機側のみ呼び出すことができます.

  Arguments:    current_count    現在の受信済みシーケンス数を格納する
                                 変数のアドレス.
                total_count      受信すべきシーケンス総数を格納する
                                 変数のアドレス.

  Returns:      現在 ReadFile の状態でかつ正しく進捗が取得できれば TRUE,
                そうでなければ FALSE を返します.
 *---------------------------------------------------------------------------*/
BOOL    WFS_GetCurrentDownloadProgress(int *current_count, int *total_count);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetPacketSize

  Description:  WFS に設定された親機の MP 通信パケットサイズを取得します.

  Arguments:    None.

  Returns:      WFS に設定された親機の MP 通信パケットサイズ.
 *---------------------------------------------------------------------------*/
int     WFS_GetPacketSize(void);

/*---------------------------------------------------------------------------*
  Name:         WFS_SetPacketSize

  Description:  親機の MP 通信パケットサイズを設定します.
                この値を大きくすると転送効率が向上し, 小さくした場合は
                一回の MP 通信でデータシェアリング等の別ポート通信と同梱され,
                ポート優先度による一方的な停滞を回避することができます.
                この関数は親機側のみ呼び出すことができます.

  Arguments:    size             設定する親機側送信サイズ.
                                 この値は WBT_PACKET_SIZE_MIN 以上かつ
                                 親機最大送信サイズ以下である必要があります.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_SetPacketSize(int size);

/*---------------------------------------------------------------------------*
  Name:         WFS_EnableSync

  Description:  親機側でアクセス同期を取る子機群のセットを設定します.
                この関数は, 同一のファイル群を同じ順序でアクセスすることが
                明らかに保証されている子機群に対して応答を同期することで
                WBT ライブラリの特性を活かした効率的な転送を実現します.
                ただし, 同期開始のタイミングが論理的に安全でない場合
                子機群への応答がずれてデッドロックする点に注意してください.
                この関数は親機側のみ呼び出すことができます.

  Arguments:    sync_bitmap      アクセス同期を取る子機群の aid ビットマップ.
                                 親機自身を示す最下位ビット 1 は無視されます.
                                 この値に 0 を指定すると同期は行いません.
                                 デフォルトではこの状態です.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_EnableSync(int sync_bitmap);

/*---------------------------------------------------------------------------*
  Name:         WFS_DisableSync

  Description:  親機側で子機群のアクセス同期を解除します.
                この関数は WFS_EnableSync() 0 を指定することと等価です.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void WFS_DisableSync(void)
{
    WFS_EnableSync(0);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_SetDebugMode

  Description:  WFS 内部のデバッグ出力を有効 / 無効に設定します.
                この設定はデフォルトで FALSE になっています.

  Arguments:    enable_debug     デバッグ出力有効なら TRUE, 無効なら FALSE.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WFS_SetDebugMode(BOOL enable_debug);


/*****************************************************************************/


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_BUILD_DEMOS_WIRELESSSHARED_WFS_INCLUDE_WFS_H_ */

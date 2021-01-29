/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless_shared
  File:     wc.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.h,v $
  Revision 1.11  2006/03/13 06:04:46  okubata_ryoma
  マクロの微修正

  Revision 1.10  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.9  2005/04/26 02:35:02  terui
  Fix comment

  Revision 1.8  2005/02/28 05:26:34  yosizaki
  do-indent.

  Revision 1.7  2005/02/02 04:21:20  terui
  TurnOff のつづり間違いを修正。

  Revision 1.6  2005/01/11 07:51:38  terui
  Update copyright.

  Revision 1.5  2005/01/11 04:59:09  terui
  WcFinish関数を追加。

  Revision 1.4  2004/12/22 02:46:48  terui
  ピクトチャットサーチに対応

  Revision 1.3  2004/10/29 01:54:47  terui
  子機のスキャン時のチャンネルをWcInitに渡されたチャンネルを使用するよう修正。

  Revision 1.2  2004/10/28 11:20:16  yosizaki
  merge with wbt-1.

  Revision 1.1  2004/10/22 11:04:14  terui
  wcモジュールを共有領域に切り出し。

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef DEMOS_WIRELESS_SHARED_WC_H_
#define DEMOS_WIRELESS_SHARED_WC_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include    <nitro/types.h>
#include    <nitro/wm.h>


/*---------------------------------------------------------------------------*
    定数定義
 *---------------------------------------------------------------------------*/
#define     WC_DMA_NO                   2       // GX等のDMA番号と重ならないよう注意
#define     WC_DEFAULT_PORT             4       // ポート番号( 4～7 )
#define     WC_DEFAULT_PORT_PRIO        2       // ポートの優先度( 0～3 )
#define     WC_DS_PORT_NUMBER           12      // データシェア用ポート番号( 12～15 )


/*---------------------------------------------------------------------------*
    マクロ定義
 *---------------------------------------------------------------------------*/
// 32バイトアライン用
#define     WC_ROUNDUP_32B( x ) ( ( (x) + 31 ) & 0xffffffe0 )


/*---------------------------------------------------------------------------*
    構造体定義
 *---------------------------------------------------------------------------*/
typedef enum WCStatus
{
    WC_STATUS_READY = 0,               // 初期化状態
    WC_STATUS_ERROR,                   // どうにもならないエラー状態
    WC_STATUS_BUSY,                    // 状態遷移中につきビジー状態
    WC_STATUS_PARENT,                  // 親機として接続済み状態
    WC_STATUS_CHILD,                   // 子機として接続済み状態
    WC_STATUS_MAX
}
WCStatus;

// 単体MPモード用のコールバック関数型定義
typedef void (*WCCallbackFunc) (u16 aid, u16 *data, u16 length);

// ブロック転送モード用のコールバック関数型定義
// 現在は、以下の時点で WMCallback ポインタが渡される
// ・WM_StartMP 完了時
// ・WcSetParentData,WcSetChildData 完了時
// ・WM_Reset 完了時
typedef void (*WCBlockTransferCallbackFunc) (WMCallback *arg);

// ピクトチャットサーチ用のコールバック関数型定義
typedef void (*WCPictoCatchCallbackFunc) (WMBssDesc *pBssDesc);


/*---------------------------------------------------------------------------*
    関数定義
 *---------------------------------------------------------------------------*/
void    WcInit(const WMParentParam *pp, BOOL dsFlag, BOOL btFlag, u16 framePeriod);     // 各種初期化。主にメモリ確保
void    WcFinish(void);                // 初期化前の状態に戻す。主にメモリ解放
void    WcStartParent(void);           // 親機としての接続開始
void    WcStartChild(void);            // 子機としての接続開始
void    WcEnd(void);                   // 初期化状態への移行開始
WCStatus WcGetStatus(void);            // WCStatus型の内部状態取得
u16     WcGetAid(void);                // AID取得

// MP通信のデータ受信時に呼び出すコールバック設定
void    WcSetMpReceiveCallback(WCCallbackFunc cb);
// ブロック転送モード設定時に呼び出すコールバック設定
void    WcSetBlockTransferCallback(WCBlockTransferCallbackFunc cb);
// 親機としての送信データセット
void    WcSetParentData(const void *buf, u16 size);
// 子機としての送信データセット
void    WcSetChildData(const void *buf, u16 size);

// 接続完了後のデータシェア通信を進める。1ゲームフレームに一回コール
BOOL    WcStepDataSharing(const void *send, u16 *bitmap);
// 受信データから各端末のデータへのポインタを取得する。
u8     *WcGetSharedDataAddress(u16 aid);

// ピクトチャットサーチ機能を有効にする
void    WcTurnOnPictoCatch(WCPictoCatchCallbackFunc func);
// ピクトチャットサーチ機能を無効にする
void    WcTurnOffPictoCatch(void);


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* DEMOS_WIRELESS_SHARED_WC_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

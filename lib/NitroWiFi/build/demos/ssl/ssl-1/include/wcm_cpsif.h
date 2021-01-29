/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     wcm_cpsif.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_cpsif.h,v $
  Revision 1.3  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.1  2005/07/21 11:47:03  seiki_masashi
  ssl/ssl-1 に移動

  Revision 1.1  2005/07/18 08:10:26  seiki_masashi
  ssl-1 デモの追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITROWIFI_WCM_CPSIF_H_
#define NITROWIFI_WCM_CPSIF_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include <nitro/types.h>

/*---------------------------------------------------------------------------*
    定数定義
 *---------------------------------------------------------------------------*/

// WCM_SendDCFData 関数の返り値 ( 異常時 )
#define WCM_CPSIF_RESULT_BEFORE_INIT    (-1)    // WCMライブラリが初期化されていない
#define WCM_CPSIF_RESULT_ILLEGAL_PARAM  (-2)    // 指定パラメータが不正
#define WCM_CPSIF_RESULT_IN_IRQ_MODE    (-3)    // IRQ モードでの呼び出し
#define WCM_CPSIF_RESULT_NO_CONNECTION  (-4)    // AP と接続確立していない、もしくは途中で切断された
#define WCM_CPSIF_RESULT_SEND_FAILURE   (-5)    // DCF フレームの送信になんらかの理由で失敗( 要再送 )

/*---------------------------------------------------------------------------*
    構造体定義
 *---------------------------------------------------------------------------*/

// DCF フレーム受信コールバック関数型
typedef void (*WCMRecvInd) (const u8*srcAddr, const u8*dstAddr, const u8*buf, s32 len);

/*---------------------------------------------------------------------------*
    関数定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         WCM_SetRecvDCFCallback

  Description:  CPS の受信コールバック関数を登録する。
                無線ドライバが DCF フレームを受信する度にここで登録した受信
                コールバック関数が呼び出される。
    NOTICE:     ここで登録したコールバック関数がオーバーレイ内にある場合には、
                飛び先がメモリに存在しなくなる前に登録しているコールバック関数を
                NULL で上書きクリアする必要がある点に注意。

  Arguments:    callback    -   DCF 受信コールバック関数を指定する。
                                NULL を指定すると受信通知されなくなる。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WCM_SetRecvDCFCallback(WCMRecvInd callback);

/*---------------------------------------------------------------------------*
  Name:         WCM_GetApMacAddress

  Description:  無線ドライバが接続を確立している AP の MAC アドレスを取得する。
    NOTICE:     この関数によって取得したポインタが示すバッファの内容は、割り込み
                等による状態変化に伴って変更されたり、バッファ自身が解放されたり
                する可能性がある点に注意。割り込み禁止した状態で呼び出し、内容を
                別バッファに退避することを推奨する。

  Arguments:    None.

  Returns:      u8*     -   接続を確立している AP の MAC アドレスへのポインタを
                            返す。接続が確立されていない場合には NULL を返す。
 *---------------------------------------------------------------------------*/
u8*     WCM_GetApMacAddress(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_SendDCFData

  Description:  無線ドライバに DCF フレーム送信を指示する。DCF フレームの送信
                完了、もしくは失敗するまで内部でブロックして待つ。また、無線の
                接続状態が DCF フレームを送信できない状態の場合には失敗する。
    NOTICE:     複数のスレッドから非同期に呼び出されることを想定しているが、
                割り込みサービス内で呼び出されることは想定していない点に注意。

  Arguments:    dstAddr -   DCF フレームのあて先となる MAC アドレスを指定する。
                buf     -   送信するデータへのポインタを指定する。偶数アドレス
                            である必要があり、送信データが存在する領域を含む
                            キャッシュが強制的にストア(メモリへ書き出し)される。
                            また、当関数から戻るまではデータ本体の内容は保持され
                            ていなければならない点にも注意。
                len     -   送信するデータの長さをバイト単位で指定する。

  Returns:      s32     -   送信に成功した場合に送信されたデータ長を返す。
                            失敗した場合には負値を返す。
 *---------------------------------------------------------------------------*/
s32     WCM_SendDCFData(const u8* dstAddr, const u8* buf, s32 len);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* NITROWIFI_WCM_CPSIF_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

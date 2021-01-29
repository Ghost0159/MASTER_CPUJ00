/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - httpclient
  File:     ssl.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ssl.h,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2005/12/22 04:34:06  adachi_hiroaki
  HTTPクライアントのデモをSOCで置き換え

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#ifndef HTTPCLIENT_SSL_H_
#define HTTPCLIENT_SSL_H_

#ifdef __cplusplus

extern "C" {
#endif

#include <nitroWiFi.h>

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  定数定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  構造体定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  関数プロトタイプ定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         SslInit

  Description:  SSL 初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    SslInit(void);

/*---------------------------------------------------------------------------*
  Name:         SslSetDefaultConncetion

  Description:  デフォルトのSOCLSslConnection構造体を登録する.

  Arguments:    psslCon		登録するSOCLSslConnectionへのポインタ.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    SslSetDefaultConncetion(SOCSslConnection* psslCon);

/*---------------------------------------------------------------------------*
  Name:         SslGetDefaultConnection

  Description:  ssl.cに登録されているデフォルトのSOCLSslConnectionへのポインタを返す.

  Arguments:    None.

  Returns:      ssl.cに登録されているSOCLSslConnectionへのポインタ.
                InitSSL前はNULLを返す.
 *---------------------------------------------------------------------------*/
SOCLSslConnection*  SslGetDefaultConnection();

/*---------------------------------------------------------------------------*
  Name:         SslSetDefaultAuthCallback

  Description:  SSL認証用のコールバックを設定する.

  Arguments:    auth_callback	コールバック関数へのポインタ.

  Returns:		None.
 *---------------------------------------------------------------------------*/
void    SslSetDefaultAuthCallback(int (*auth_callback) (int, struct _CPSSslConnection*, int));

/*---------------------------------------------------------------------------*
  Name:         SslAuthCallback

  Description:  SSL サーバ認証コールバック.

  Arguments:    con           SSL接続情報
                level         レベル

  Returns:      None.
 *---------------------------------------------------------------------------*/
int     SslAuthCallback(int result, SOCSslConnection* con, int level);

/*---------------------------------------------------------------------------*
  Name:         SslSetDefaultCA

  Description:  SSLの証明書を設定する.

  Arguments:    ca_list		証明書リスト
                builtins.	証明書の数

  Returns:		None.
 *---------------------------------------------------------------------------*/
void    SslSetDefaultCA(SOCCaInfo ** ca_list, int builtins);

/*---------------------------------------------------------------------------*
  Name:         SslCreateConnection

  Description:  SSLの証明書を設定する.

  Arguments:    servername	CNとのマッチさせる接続先のサーバ名

  Returns:		新しく確保されたSslCreateConnection構造体へのポインタ
 *---------------------------------------------------------------------------*/
SOCLSslConnection*  SslCreateConnection(const char* servername);

/*---------------------------------------------------------------------------*
  Name:         SslDestroyConnection

  Description:  ソケットに関連付けられたSslCreateConnection構造体を破棄する.

  Arguments:    socket		SSLが有効になっているソケットディスクリプタ

  Returns:		新しく確保されたSslCreateConnection構造体へのポインタ
 *---------------------------------------------------------------------------*/
void    SslDestroyConnection(SOCSslConnection* con);

void    SslPrintCA(const SOCSslConnection* con);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif /* __cplusplus */
#endif /* HTTPCLIENT_SSL_H_ */

/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     wcm_control.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_control.h,v $
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
#ifndef WCM_CONTROL_H_
#define WCM_CONTROL_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         InitWcmControl

  Description:  内部変数を初期化し、WCM コントロールスレッドを起動する。
                既にスレッドが起動している場合は何も行わない。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    InitWcmControl(void);

/*---------------------------------------------------------------------------*
  Name:         TerminateWcmControl

  Description:  WCM コントロールスレッドを終了する。
                スレッドが起動されていない場合は何も行わない。
    NOTICE:     当関数はスレッドを終了する為のトリガとなるだけなので、スレッド
                自体が終了するまで待たずに戻る点に注意。
                スレッド自体の終了を待ちたい場合は、OS_SendMessage 呼び出しに
                続いて OS_JoinThread を呼び出す必要がある。
                なお、WCM_GetPhase 関数が WCM_PHASE_NULL を返すようになれば
                スレッドは終了している。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    TerminateWcmControl(void);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* WCM_CONTROL_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

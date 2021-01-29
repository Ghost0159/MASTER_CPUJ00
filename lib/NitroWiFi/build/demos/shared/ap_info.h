/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - shared - demos
  File:     ap_info.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ap_info.h,v $
  Revision 1.3  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/11/02 04:13:42  terui
  Fix comment.

  Revision 1.1  2005/07/21 08:38:16  terui
  サンプルで共有可能なソースファイルとして新規追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef SHARED_AP_INFO_H_
#define SHARED_AP_INFO_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include <nitroWiFi/wcm.h>

// AP 情報構造体 型定義
typedef struct WcmControlApInfo
{
    u8          bssId[WCM_BSSID_SIZE];
    WCMWepDesc  wepDesc;
    u8          essId[WCM_ESSID_SIZE];
    u32         auth_option;

} WcmControlApInfo;

// 固定 AP 情報の参照
extern const WcmControlApInfo   constApInfo;

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* SHARED_AP_INFO_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

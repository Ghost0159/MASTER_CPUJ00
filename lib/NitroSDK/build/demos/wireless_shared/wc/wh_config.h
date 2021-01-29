/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless_shared - wc
  File:     wh_config.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_config.h,v $
  Revision 1.1  2007/10/03 05:38:57  yosizaki
  initial upload.

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#ifndef WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_
#define WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_

#include "wc.h"

#define WH_DMA_NO                 WC_DMA_NO
#define WH_DATA_PORT              WC_DEFAULT_PORT
#define WH_DATA_PRIO              WC_DEFAULT_PORT_PRIO
#define WH_DS_PORT                WC_DS_PORT_NUMBER

// WCでは以下の情報をWMParemtParamの変数として管理しているため、
// とりあえず最大値を指定してWHに充分なバッファサイズを定義させたのち
// wh.c 内でマクロを再定義して変数を参照するよう切り替える。
#define WH_PLAYER_MAX             16
#define WH_CHILD_MAX              (WH_PLAYER_MAX - 1)
#define WH_DS_DATA_SIZE           32
#define WH_PARENT_MAX_SIZE        512
#define WH_CHILD_MAX_SIZE         512

#define WH_MP_FREQUENCY           0


#endif /* WM_DEMO_WIRELESS_SHARED_WC_WH_CONFIG_H_ */

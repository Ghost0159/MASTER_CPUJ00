/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     wh_config.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_config.h,v $
  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#ifndef WM_DEMO_WIRELESS_ALL_WH_CONFIG_H_
#define WM_DEMO_WIRELESS_ALL_WH_CONFIG_H_


/* このデモで使用する GGID */
#define WH_GGID                   SDK_MAKEGGID_SYSTEM(0x15)

// 無線で使用するDMA番号
#define WH_DMA_NO                 2

#define WH_PLAYER_MAX             4

// 子機最大数（親機を含まない数）
#define WH_CHILD_MAX              (WH_PLAYER_MAX - 1)

// シェア出来るデータの最大サイズ
#define WH_DS_DATA_SIZE           32

#define WH_PARENT_MAX_SIZE        512
#define WH_CHILD_MAX_SIZE       (WH_DS_DATA_SIZE)

// 通常の MP 通信で使用するポート
#define WH_DATA_PORT              14

// 通常の MP 通信で使用する優先度
#define WH_DATA_PRIO              WM_PRIORITY_NORMAL

// WFS とデータシェアリングを併用するため連続転送モードに */
#define WH_MP_FREQUENCY           0

// データシェアリングで使用するポート
#define WH_DS_PORT                13

// WFSで使用するポート
#define PORT_WFS                  4

#endif /* WM_DEMO_WIRELESS_ALL_WH_CONFIG_H_ */

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - include
  File:     mb_measure_channel.h

  Copyright 2004-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_measure_channel.h,v $
  Revision 1.7  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.6  2005/03/01 01:57:00  yosizaki
  copyright の年を修正.

  Revision 1.5  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.4  2004/10/22 04:10:30  takano_makoto
  MBM_MeasureChannelExをMBM_MeasureChannelInIdleに改名

  Revision 1.3  2004/10/22 02:54:53  takano_makoto
  ユーザコールバック前に状態をクリアするように修正

  Revision 1.2  2004/10/22 02:39:57  takano_makoto
  MBM_MeasureChannelExを追加

  Revision 1.1  2004/09/28 06:50:04  takano_makoto
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef _MBM_MEASURE_CHANNEL_H_
#define _MBM_MEASURE_CHANNEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>

/* 定数定義 */

typedef enum
{
    MBM_MEASURE_SUCCESS = 0,           // チャンネル取得
    MBM_MEASURE_ERROR_INIT_API = -1,   // 
    MBM_MEASURE_ERROR_INIT_CALLBACK = -2,       // 
    MBM_MEASURE_ERROR_MEASURECHANNEL_API = -3,  // 
    MBM_MEASURE_ERROR_MEASURECHANNEL_CALLBACK = -4,     // 
    MBM_MEASURE_ERROR_RESET_API = -5,  // 
    MBM_MEASURE_ERROR_RESET_CALLBACK = -6,      // 
    MBM_MEASURE_ERROR_END_API = -7,    // 
    MBM_MEASURE_ERROR_END_CALLBACK = -8,        // 
    MBM_MEASURE_ERROR_NO_ALLOWED_CHANNEL = -9,  // 
    MBM_MEASURE_ERROR_ILLEGAL_STATE = -10       // 
}
MBMErrCode;


typedef struct
{
    s16     errcode;                   // エラーコード(MBMErrCode)
    u16     channel;                   // 最も使用率の低いチャンネル
}
MBMCallback;

/* 型定義 */
typedef void (*MBMCallbackFunc) (MBMCallback *);


/* 関数定義 */

// 無線初期化前状態でコールする最適チャンネル調査関数
void    MBM_MeasureChannel(u8 *wm_buffer, MBMCallbackFunc callback);
// IDLEステートでコールする最適チャンネル調査関数
void    MBM_MeasureChannelInIdle(MBMCallbackFunc callback);



#ifdef __cplusplus
}
#endif

#endif /* _MBM_MEASURE_CHANNEL_H_ */

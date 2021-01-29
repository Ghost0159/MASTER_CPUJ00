/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     wh_measure.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_measure.c,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"
#include "wh.h"


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         MeasureChannel

  Description:  使用可能な各チャンネルのワイヤレス使用率を計測.
                WH が WH_SYSSTATE_IDLE ステートの状態で使用可能.

  Arguments:    None.

  Returns:      もっともワイヤレス使用率の小さいチャンネル.
 *---------------------------------------------------------------------------*/
u16 MeasureChannel(void)
{
    SDK_ASSERT(WH_GetSystemState() == WH_SYSSTATE_IDLE);
    /* チャンネル計測開始 */
    (void)WH_StartMeasureChannel();
    WaitWHState(WH_SYSSTATE_MEASURECHANNEL);
    /* 全チャンネル計測完了 */
    return WH_GetMeasureChannel();
}

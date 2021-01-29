/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - seq
  File:     channel.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: channel.h,v $
  Revision 1.2  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/13 03:29:35  ida
  SNDÇNITRO-SDKÇ÷à⁄ê›

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  èâî≈

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef CHANNEL_H_
#define CHANNEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>

void    ChInit(void);
void    ChSetupWave(const u8 *wave_buf);
void    ChSetEvent(const u8 *midi_data);
void    ChAllNoteOff(void);

#ifdef __cplusplus
}
#endif

#endif // CHANNEL_H_

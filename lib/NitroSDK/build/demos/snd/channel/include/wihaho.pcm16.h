/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - channel
  File:     wihaho.pcm16.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wihaho.pcm16.h,v $
  Revision 1.2  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/13 03:29:19  ida
  SNDÇNITRO-SDKÇ÷à⁄ê›

  Revision 1.1  2005/04/13 01:14:54  kyuma_koichi
  èâî≈

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef WIHAHO_PCM16_H_
#define WIHAHO_PCM16_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>

#define WIHAHO_PCM16_FORMAT    SND_WAVE_FORMAT_PCM16
#define WIHAHO_PCM16_LOOPFLAG  0
#define WIHAHO_PCM16_RATE      44100
#define WIHAHO_PCM16_TIMER     379
#define WIHAHO_PCM16_LOOPSTART 0
#define WIHAHO_PCM16_LOOPLEN   27016

const s16 wihaho_pcm16[];

#ifdef __cplusplus
}
#endif

#endif // WIHAHO_PCM16_H_

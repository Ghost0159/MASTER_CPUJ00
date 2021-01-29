/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - seq
  File:     piano.g5.pcm16.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: piano.g5.pcm16.h,v $
  Revision 1.2  2006/01/18 02:12:27  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/13 03:29:19  ida
  SNDÇNITRO-SDKÇ÷à⁄ê›

  Revision 1.1  2005/04/13 01:14:54  kyuma_koichi
  èâî≈

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef PIANO_G5_PCM16_H_
#define PIANO_G5_PCM16_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>

#define PIANO_G5_PCM16_FORMAT    SND_WAVE_FORMAT_PCM16
#define PIANO_G5_PCM16_LOOPFLAG  1
#define PIANO_G5_PCM16_RATE      32009
#define PIANO_G5_PCM16_TIMER     523
#define PIANO_G5_PCM16_LOOPSTART 9007
#define PIANO_G5_PCM16_LOOPLEN   1632

const s16 piano_g5_pcm16[];

#ifdef __cplusplus
}
#endif

#endif // PIANO_G5_PCM16_H_

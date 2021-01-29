/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - fake_child
  File:     disp.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: font.h,v $
  Revision 1.2  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.1  2004/11/22 13:00:19  takano_makoto
  multiboot-Model‚Ì‚à‚Ì‚Æ“¯ˆê‚Ìƒtƒ@ƒCƒ‹‚Æ‚µ‚Ä“o˜^

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef MB_DEMO_FONT_H_
#define MB_DEMO_FONT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>


extern const u16 samplePlttData[16][16];

extern const u32 sampleFontCharData[8 * 0x100];



#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // MB_DEMO_DISP_H_

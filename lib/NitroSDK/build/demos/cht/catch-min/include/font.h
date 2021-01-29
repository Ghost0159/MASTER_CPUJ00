/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CHT - demos
  File:     font.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: font.h,v $
  Revision 1.3  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.2  2005/07/14 23:39:19  kitase_hirotake
  Copyright’¼‚µ

  Revision 1.1  2005/07/14 01:33:38  kitase_hirotake
  Initial Release

  Revision 1.1  2004/12/22 02:42:43  terui
  Initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef FONT_H_
#define FONT_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include    <nitro/types.h>

extern const u32 d_CharData[8 * 256];
extern const u32 d_PaletteData[8 * 16];

/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* FONT_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

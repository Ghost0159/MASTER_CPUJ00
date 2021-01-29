/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     font.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: font.h,v $
  Revision 1.3  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.1  2005/07/21 11:47:03  seiki_masashi
  ssl/ssl-1 ‚ÉˆÚ“®

  Revision 1.1  2005/07/18 08:10:26  seiki_masashi
  ssl-1 ƒfƒ‚‚Ì’Ç‰Á

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef FONT_H_
#define FONT_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include <nitro/types.h>

extern const u32    d_CharData[8 * 256];
extern const u32    d_PaletteData[8 * 16];

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* FONT_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - simple-1
  File:     font.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: font.h,v $
  Revision 1.3  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.2  2004/11/01 04:11:44  sasakis
  処理落ち対策コードなどの追加（作業中コードを含む暫定版）。

  Revision 1.1  2004/09/07 04:59:42  sasakis
  初版登録。

  Revision 1.1  2004/07/23 15:28:28  terui
  整理して新規追加

  Revision 1.2  2004/07/23 14:52:40  terui
  Only fix comment

  Revision 1.1  2004/07/15 12:48:21  terui
  initial upload

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

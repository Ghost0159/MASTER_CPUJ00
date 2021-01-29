/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-Model
  File:     unicode.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: unicode.h,v $
  Revision 1.4  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.3  2005/11/25 02:13:27  adachi_hiroaki
  著作権表記修正

  Revision 1.1  2005/11/21 00:41:10  adachi_hiroaki
  親機選択画面に参加人数とニックネームを表示するようにした

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#ifndef	__UNICODE_H_
#define	__UNICODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>

// define data----------------------------------

// function's prototype declaration-------------
void    ExSJIS_BEtoUTF16_LE(u8 *sjisp, u16 *unip, u16 length);
void    ExUTF16_LEtoSJIS_BE(u8 *sjisp, u16 *unip, u16 length);
void    CheckSJIS_BEtoUTF16_LE(void);


#ifdef __cplusplus
}
#endif

#endif // __UNICODE_H_

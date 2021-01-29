/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Pol_High_Light
  File:     data.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: data.h,v $
  Revision 1.8  2005/02/28 05:26:05  yosizaki
  do-indent.

  Revision 1.7  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.6  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.5  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.4  2004/01/07 09:38:05  nishida_kenji
  revises comments

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef _DATA_H_
#define _DATA_H_

#include <nitro.h>

/* 椅子モデル */
extern const u32 isu1[];
extern const u32 isu2[];

/* 椅子サイズ */
extern const u32 isu1_size;
extern const u32 isu2_size;

/* 椅子テクスチャ */
extern const u16 tex_isu1[];
extern const u16 tex_isu2[];

/* 椅子パレット */
extern const u16 pal_isu1[];
extern const u16 pal_isu2[];

/* 椅子テクスチャ及びパレットのサイズ */
extern const u32 tex_isu1_size;
extern const u32 tex_isu2_size;

extern const u32 pal_isu1_size;
extern const u32 pal_isu2_size;

#endif /* _DATA_H_ */

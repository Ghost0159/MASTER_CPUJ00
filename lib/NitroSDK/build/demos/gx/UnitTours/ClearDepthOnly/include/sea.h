/*---------------------------------------------------------------------------*
  Project:  NitroSDK - tests - GX
  File:     sea.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: sea.h,v $
  Revision 1.2  2005/02/28 05:26:32  yosizaki
  do-indent.

  Revision 1.1  2004/10/04 13:36:33  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef _SEA_H_
#define _SEA_H_

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

/* 海スクリーン */

extern const unsigned int d_natsunoumi_sscDT[32 * 32 / 2];
extern const unsigned int d_natsunoumi_sclDT[8 * 16];
extern const unsigned int d_natsunoumi_schDT[16 * 736];

#endif /* _SEA_H_ */

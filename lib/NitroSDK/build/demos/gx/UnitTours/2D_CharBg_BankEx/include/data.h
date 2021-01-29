/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_CharBg_BankEx
  File:     data.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: data.h,v $
  Revision 1.2  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.1  2004/11/25 11:41:44  takano_makoto
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef BG_DATA_H_
#define BG_DATA_H_

#define SCREEN_SIZE (32 * 24)

/*-------------------------- Data  -------------------------------*/
#define CHAR_SIZE (16 * 128 * sizeof(int))
extern const unsigned int d_64_256_bg_schDT[16 * 128];
extern const unsigned int d_64_256_bg_sclDT[8 * 16];

#endif

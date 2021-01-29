/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_256_16
  File:     data.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: data.h,v $
  Revision 1.4  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.3  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.1  2004/03/01 09:05:42  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
 ******************************************************************************
 *
 *  Project    :
 *               nitroSDKサンプルプログラム
 *  Title      :
 *               アフィン拡張・256色×16拡張パレットＯＢＪのを表示
 *  Explanation:
 *　　　　　　　 256色×16拡張パレットＯＢＪを、アフィン拡張（回転拡大縮小）を
 *             行って表示する。
 *
 ******************************************************************************
 */
#ifndef OBJ_DATA_H
#define OBJ_DATA_H

extern const unsigned char bitmapOBJ_256color_icg[16 * 2048];
extern const unsigned char bitmapOBJ_256color_icl[16 * 512];

#endif

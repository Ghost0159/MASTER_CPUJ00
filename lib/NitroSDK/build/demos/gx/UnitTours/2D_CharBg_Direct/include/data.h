/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_CharBg_Direct
  File:     data.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: data.h,v $
  Revision 1.5  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.4  2004/11/02 18:23:39  takano_makoto
  fix comments.

  Revision 1.3  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.1  2004/03/01 08:59:58  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
 ******************************************************************************
 *
 *  Project    :
 *               nitroSDKサンプルプログラム
 *  Title      :
 *               アフィン拡張・ダイレクトカラービットマップＢＧを表示
 *  Explanation:
 *               ダイレクトカラービットマップＢＧを、アフィン変換（回転拡
 *             大縮小）を行って表示する。
 *
 ******************************************************************************
 */
#ifndef BG_DATA_H_
#define BG_DATA_H_

/*-------------------------- Data  -------------------------------*/
extern const unsigned short bitmapBG_directcolor_Texel[256 * 256];

#define SCREEN_SIZE (256 * 256 * 2)


#endif

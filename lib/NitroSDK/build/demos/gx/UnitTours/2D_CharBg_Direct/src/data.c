/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_CharBg_Direct
  File:     data.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: data.c,v $
  Revision 1.6  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.5  2004/11/02 09:59:24  takano_makoto
  fix typo.

  Revision 1.4  2004/07/29 02:18:23  yasu
  Let a picture exchangeable

  Revision 1.3  2004/04/07 01:22:55  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 08:59:52  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
 ******************************************************************************
 *
 *  Project    :
 *               nitroSDKサンプルプログラム
 *  Title      :
 *               アフィン拡張BGのダイレクトカラービットマップBGを表示のデータ
 *  Explanation:
 *               アフィン拡張BGの256色ダイレクトカラービットマップBGの、回転、
 *             拡大表示を行う。
 *
 ******************************************************************************
 */
// texture information
// type:          GX_TEXFMT_DIRECT
// size_width:    256
// size_height:   256

const unsigned short bitmapBG_directcolor_Texel[256 * 256] = {
#include "../pictures/picture_256_256.autogen.c"
};

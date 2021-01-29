/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_BmpBg_MainRam
  File:     image.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: image.c,v $
  Revision 1.7  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.6  2004/11/02 09:47:01  takano_makoto
  fix typo.

  Revision 1.5  2004/07/28 07:05:54  yasu
  Let pictures exchangeable

  Revision 1.4  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.2  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.1  2004/01/10 02:18:32  nishida_kenji
  directory renamed

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include "image.h"

/* Image on MRAM */
const unsigned short IMAGE_MRAM256x192[4][256 * 192] = {
    {
#include "picture_256_192_a.autogen.c"
     },
    {
#include "picture_256_192_b.autogen.c"
     },
    {
#include "picture_256_192_c.autogen.c"
     },
    {
#include "picture_256_192_d.autogen.c"
     }
};

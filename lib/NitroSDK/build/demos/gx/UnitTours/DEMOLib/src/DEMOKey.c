/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/DEMOLib
  File:     DEMOKey.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: DEMOKey.c,v $
  Revision 1.10  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.9  2005/02/28 05:26:05  yosizaki
  do-indent.

  Revision 1.8  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.7  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.6  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.5  2004/01/07 09:38:05  nishida_kenji
  revises comments

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include "DEMOKey.h"

DEMOKeyWork gKeyWork;

void DEMOReadKey()
{
    u16     readData = PAD_Read();
    gKeyWork.trigger = (u16)(readData & (readData ^ gKeyWork.press));
    gKeyWork.press = readData;
}

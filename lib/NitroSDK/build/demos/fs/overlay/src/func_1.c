/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay
  File:     func_1.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: func_1.c,v $
  Revision 1.3  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.2  2004/04/13 12:08:02  yosizaki
  add ovaerlay to TCM.
  fix header comment.

  Revision 1.1  2004/04/02 12:21:12  yosizaki
  (none)


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include	<nitro.h>

#include	"func.h"

void func_1(void)
{
    static int i = 0;
    ++i;
    OS_Printf("here is func_1. called %d time%s.\n", i, (i == 1) ? "" : "s");
}

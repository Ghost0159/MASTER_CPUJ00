/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - file-1
  File:     woo.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: woo.c,v $
  Revision 1.5  2006/01/18 02:12:40  kitase_hirotake
  do-indent

  Revision 1.4  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.3  2004/04/13 12:08:47  yosizaki
  fix header comment.

  Revision 1.2  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.1  2004/01/15 07:32:17  yasu
  Test for ROM Image construction

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include	<nitro.h>

void    function_Woo(void);
void function_Woo(void)
{
    OS_Printf("Hello, I am Woo.\n");
    return;
}

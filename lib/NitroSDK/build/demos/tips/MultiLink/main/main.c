/*---------------------------------------------------------------------------*
  Project:  NitroSDK - tips - demos - MultiLink
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.3  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:23  yosizaki
  do-indent.

  Revision 1.1  2004/06/25 11:38:55  yasu
  demo for hierarchy makefile to do breakdown module onto small modules

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    Ext_Module(void);
void    Ext_Obj_Module(void);

void NitroMain(void)
{
    OS_Printf("NitroMain Start\n");
    Ext_Module();
    Ext_Obj_Module();
    OS_Printf("NitroMain End\n");
    OS_Terminate();
}

/*====== End of main.c ======*/

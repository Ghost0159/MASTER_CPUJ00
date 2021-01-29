/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - file-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.8  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.7  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.6  2004/04/13 12:08:47  yosizaki
  fix header comment.

  Revision 1.5  2004/04/03 09:02:32  yasu
  merged with 1.00 branch

  Revision 1.4.2.1  2004/04/03 09:00:30  yasu
  add NitroStartUp

  Revision 1.4  2004/02/23 05:01:38  yasu
  axf->nef

  Revision 1.3  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.2  2004/01/15 13:24:19  yasu
  Find more butter way to avoid deadstripping

  Revision 1.1  2004/01/15 07:32:17  yasu
  Test for ROM Image construction

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include	<nitro.h>

/*
  This test is checking:
     if OVERLAY files are generated correctly (area of CW/makelcf)
     if Rom image are constructed             (area of IPL/makerom)
  
  Not checking:
     file system, overlay
  
  File Moves:
  
  [makelcf]
     main.lsf
     include/nitro/ARM9-TEG.lcf.template
                                      =>    main.lcf
  
  [CodeWarrior]
     main.c
      boo.c
      foo.c
      woo.c
     main.lcf                         =>    *.o
                                            main.sbin
                                            main_defs.sbin
                                            main_table.sbin
                                            main.nef
                                            main_overlay_0.sbin
                                            main_overlay_1.sbin
  
  [makerom]
     main.rsf
     main.sbin
     main_defs.sbin
     main_table.sbin
     main.nef
     main_overlay_0.sbin
     main_overlay_1.sbin
     tools/makerom/spMain.sbin
     tools/makerom/spMain_defs.sbin   =>    main.bin  -----> TARGET
                                            main.nlf  -----> for debug info
                                            main_header.sbin
                                            main_files.sbin

 */

void    function_Boo(void);
void    function_Foo(void);
void    function_Woo(void);
void    SDK_MAIN_ARENA_LO(void);
void    SDK_SYS_STACKSIZE(void);

void NitroMain(void)
{
    OS_Printf("Hello, I am main.\n");

    OS_Printf(" This is function_Boo = 0x%08x\n", function_Boo);
    OS_Printf(" This is function_Foo = 0x%08x\n", function_Foo);
    OS_Printf(" This is function_Woo = 0x%08x\n", function_Woo);

    OS_Printf(" SDK_MAIN_ARENA_LO    = 0x%08x\n", SDK_MAIN_ARENA_LO);
    OS_Printf(" SDK_SYS_STACKSIZE    = 0x%08x\n", SDK_SYS_STACKSIZE);

    while (1)
    {
    };
}

void NitroStartUp(void)
{
    OS_Printf("Hello, I am NitroStartUp.\n");
}

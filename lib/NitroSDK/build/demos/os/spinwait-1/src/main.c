/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - spinwait-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.6  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.5  2005/02/28 05:26:20  yosizaki
  do-indent.

  Revision 1.4  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.3  2004/04/06 02:18:05  yada
  OS_Halt()Å®OS_Terminate()

  Revision 1.2  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.1  2004/02/27 01:55:51  yasu
  add demo for OS_SpinWait

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#define TicksToCPUCycles( tick )        ((tick)*64*2)

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    OSTick  begin, end;
    u32     n;

    OS_Init();
    OS_InitTick();

    n = 1000;
    for (n = 1000; n <= 1000000; n *= 2)
    {
        begin = OS_GetTick();
        OS_SpinWait(n);
        end = OS_GetTick();

        OS_Printf("OS_SpinWait(%d)  Wait=%lld cycles\n", n, TicksToCPUCycles(end - begin));
    }

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

/*====== End of main.c ======*/

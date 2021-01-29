/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - functionCost-1
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

  Revision 1.5  2005/04/18 06:31:07  yosizaki
  add OS_EnableIrq().

  Revision 1.4  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.3  2004/05/24 04:47:08  yada
  fix profile pragma

  Revision 1.2  2004/05/20 04:32:47  yada
  add pragma profile off to NitroMain()

  Revision 1.1  2004/05/19 04:11:51  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    test1(void);
void    test2(void);
void    test3(void);

#define MYCOSTBUFFER_SIZE 0x3000
#define MYSTATBUFFER_SIZE 0x300

u32     myCostBuffer[MYCOSTBUFFER_SIZE / sizeof(u32)];
u32     myStatBuffer[MYSTATBUFFER_SIZE / sizeof(u32)];
//================================================================

//----------------------------------------------------------------
// test1
//
void test1(void)
{
    test2();
}

//----------------------------------------------------------------
// test2
//
void test2(void)
{
    test3();
}

//----------------------------------------------------------------
// test3
//
void test3(void)
{
    return;
}

//================================================================================
#pragma profile off
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    OS_Init();
    OS_InitTick();
    (void)OS_EnableIrq();

    //---- init functionCost
    OS_InitFunctionCost(&myCostBuffer, MYCOSTBUFFER_SIZE);
    OS_InitStatistics(&myStatBuffer, MYSTATBUFFER_SIZE);

    test2();
    test1();
    test2();
    test3();

    //---- calculate cost
    OS_CalcStatistics(&myStatBuffer);

    //---- display functionCost
    OS_DumpStatistics(&myStatBuffer);

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

/*====== End of main.c ======*/

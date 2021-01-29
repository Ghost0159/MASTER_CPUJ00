/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - callTrace-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.12  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.11  2005/03/04 11:09:27  yasu
  __declspec(weak) ‚ð SDK_WEAK_SYMBOL ‚É’u‚«Š·‚¦

  Revision 1.10  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.9  2004/05/24 04:47:08  yada
  fix profile pragma

  Revision 1.8  2004/05/20 04:32:47  yada
  add pragma profile off to NitroMain()

  Revision 1.7  2004/05/18 06:09:43  yada
  small fix

  Revision 1.6  2004/05/12 05:17:41  yada
  delete OS_PROFILE_CALL_TRACE ifdef

  Revision 1.5  2004/05/10 12:03:28  yada
  consider for no CALLTRACE case

  Revision 1.4  2004/04/23 01:24:58  yada
  small fix

  Revision 1.3  2004/04/23 01:16:20  yada
  add log mode demo

  Revision 1.2  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.1  2004/04/12 12:28:13  yada
  first release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

int     test1(int a);
int     test2(int a);
int     test3(int a);
void    test4(int a);
void    test5(int a);

#define MYTRACEBUFFER_SIZE 0x300
u32     myTraceBuffer[MYTRACEBUFFER_SIZE / sizeof(u32)];

SDK_WEAK_SYMBOL asm void __PROFILE_ENTRY( void ){ bx lr }
SDK_WEAK_SYMBOL asm void __PROFILE_EXIT( void ){ bx lr }

//================================================================

//----------------------------------------------------------------
// test1
//
int test1(int a)
{
    return test2(a + 1);
}

//----------------------------------------------------------------
// test2
//
int test2(int a)
{
    return test3(a + 2);
}

//----------------------------------------------------------------
// test3
//
int test3(int a)
{
    OS_DumpCallTrace();
    return a + 4;
}

//----------------------------------------------------------------
// test4
//
void test4(int a)
{
    test5(a + 3);
    return;
}

//----------------------------------------------------------------
// test5
//
void test5(int a)
{
#pragma unused( a )
    return;
}

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
#pragma profile off
void NitroMain(void)
{
    int     n;

    OS_Init();

    OS_Printf("---------------- stack mode demo of callTrace.\n");
    OS_InitCallTrace(&myTraceBuffer, MYTRACEBUFFER_SIZE, OS_CALLTRACE_STACK);
    OS_Printf("buffer has %d lines to record callTrace.\n",
              OS_CalcCallTraceLines(MYTRACEBUFFER_SIZE));

    OS_Printf("---- call test3.\n");
    n = test3(1);
    OS_Printf("---- call test1.\n");
    n = test1(0x100);

    OS_Printf("---------------- log mode demo of callTrace.\n");
    OS_InitCallTrace(&myTraceBuffer, MYTRACEBUFFER_SIZE, OS_CALLTRACE_LOG);

    OS_Printf("---- call test4.\n");
    test4(1);
    OS_Printf("---- call test4.\n");
    test4(0x100);

    OS_DumpCallTrace();
    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

/*====== End of main.c ======*/

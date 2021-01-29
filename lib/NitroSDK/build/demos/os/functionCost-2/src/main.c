/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - functionCost-2
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.6  2005/04/18 06:31:13  yosizaki
  add OS_EnableIrq().

  Revision 1.5  2005/02/28 05:26:20  yosizaki
  do-indent.

  Revision 1.4  2004/06/03 04:44:08  yada
  add wait

  Revision 1.3  2004/05/24 04:47:08  yada
  fix profile pragma

  Revision 1.2  2004/05/20 04:32:47  yada
  add pragma profile off to NitroMain()

  Revision 1.1  2004/05/19 04:11:58  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    test1(void);
void    test2(void);
void    test3(void);
void    test_sleep(void);

#define MYCOSTBUFFER_SIZE 0x3000
#define MYSTATBUFFER_SIZE 0x300

u32     myCostBuffer[MYCOSTBUFFER_SIZE / sizeof(u32)];
u32     myStatBuffer[MYSTATBUFFER_SIZE / sizeof(u32)];

u32     myCostBuffer1[MYCOSTBUFFER_SIZE / sizeof(u32)];
u32     myStatBuffer1[MYSTATBUFFER_SIZE / sizeof(u32)];

u32     myCostBuffer2[MYCOSTBUFFER_SIZE / sizeof(u32)];
u32     myStatBuffer2[MYSTATBUFFER_SIZE / sizeof(u32)];

//---- thread
#define STACK_SIZE  1024

void    proc1(void *arg);
void    proc2(void *arg);

OSThread thread1;
OSThread thread2;

u64     stack1[STACK_SIZE / sizeof(u64)];
u64     stack2[STACK_SIZE / sizeof(u64)];

#define     THREAD1_PRIO    10
#define     THREAD2_PRIO    11

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
    int     n;

    OS_Init();
    OS_InitTick();
    (void)OS_EnableIrq();

    //---- init thread
    //OS_InitThread(); <- maybe done in OS_Init() already.
    OS_CreateThread(&thread1, proc1, (void *)0x111, stack1 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD1_PRIO);
    OS_CreateThread(&thread2, proc2, (void *)0x222, stack2 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD2_PRIO);
    OS_WakeupThreadDirect(&thread1);
    OS_WakeupThreadDirect(&thread2);

    //---- init functionCost
    OS_InitFunctionCost(&myCostBuffer, MYCOSTBUFFER_SIZE);
    OS_InitStatistics(&myStatBuffer, MYSTATBUFFER_SIZE);

    for (n = 0; n < 10; n++)
    {
        test3();
        OS_WakeupThreadDirect(&thread2);
    }

    //---- calculate cost
    OS_CalcStatistics(&myStatBuffer);


    //---- display all thread functionCost
    OS_DumpStatistics(&myStatBuffer);
    OS_DumpStatistics(&myStatBuffer1);
    OS_DumpStatistics(&myStatBuffer2);


    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

#pragma profile reset

//--------------------------------------------------------------------------------
//    proc1
//
void proc1(void *arg)
{
#pragma unused( arg )

    //---- init functionCost
    OS_InitFunctionCost(&myCostBuffer1, MYCOSTBUFFER_SIZE);
    OS_InitStatistics(&myStatBuffer1, MYSTATBUFFER_SIZE);

    while (1)
    {
        test1();
        test_sleep();

        //---- calculate cost
        OS_CalcStatistics(&myStatBuffer1);
    }
}

//--------------------------------------------------------------------------------
//    proc2
//
void proc2(void *arg)
{
#pragma unused( arg )

    //---- init functionCost
    OS_InitFunctionCost(&myCostBuffer2, MYCOSTBUFFER_SIZE);
    OS_InitStatistics(&myStatBuffer2, MYSTATBUFFER_SIZE);

    while (1)
    {
        test2();
        OS_SleepThread(NULL);
        OS_WakeupThreadDirect(&thread1);

        //---- calculate cost
        OS_CalcStatistics(&myStatBuffer2);
    }
}

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
    OS_SpinWait(0x10000);
    return;
}

//----------------------------------------------------------------
// test_sleep
//
void test_sleep(void)
{
    OS_SpinWait(0x10000);
    OS_SleepThread(NULL);
}

/*====== End of main.c ======*/

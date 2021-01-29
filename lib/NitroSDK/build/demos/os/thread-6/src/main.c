/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - thread-6
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.3  2005/07/12 00:57:47  yada
  just modifiy comment

  Revision 1.2  2005/06/21 02:49:22  yada
  just fix comment

  Revision 1.1  2005/06/16 09:00:09  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

/* ***************************************************************
   --- Test for OS_YieldThread() ---

   If you let the following line "#define USE_YIELD 1" be comment line
   (it means not use OS_YieldThread()), the result will be:
     Thread 3
     Thread 2
     Thread 3
     Thread 2
     Thread 3
     Thread 2
        :

  If you make it available (means to use OS_YieldThread()),
  the result will be:
     Thread 3
     Thread 2
     Thread 1
     Thread 3
     Thread 2
     Thread 1
        :

  Please try it.
 * ****************************************************************/
#define USE_YIELD  1



//================================================================================
#define	STACK_SIZE	1024

void    proc1(void *arg);
void    proc2(void *arg);
void    proc3(void *arg);

OSThread thread1;
OSThread thread2;
OSThread thread3;

u64     stack1[STACK_SIZE / sizeof(u64)];
u64     stack2[STACK_SIZE / sizeof(u64)];
u64     stack3[STACK_SIZE / sizeof(u64)];

#define		THREAD1_PRIO	10
#define		THREAD2_PRIO	10
#define		THREAD3_PRIO	10

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    OS_Init();
    OS_InitThread();

    OS_CreateThread(&thread1, proc1, (void *)0, stack1 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD1_PRIO);
    OS_CreateThread(&thread2, proc2, (void *)0, stack2 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD2_PRIO);
    OS_CreateThread(&thread3, proc3, (void *)0, stack3 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD3_PRIO);

    OS_DumpThreadList();

    (void)OS_DisableScheduler();
    {
        OS_WakeupThreadDirect(&thread1);
        OS_WakeupThreadDirect(&thread2);
        OS_WakeupThreadDirect(&thread3);
    }
    (void)OS_EnableScheduler();
    OS_RescheduleThread();

    while (1)
    {
        OS_Printf("*** launcher thread\n");     // maybe never reach
    }

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

//--------------------------------------------------------------------------------
//    proc1
//
void proc1(void *arg)
{
#pragma unused( arg )
    while (1)
    {
        OS_Printf("Thread 1\n");

#ifdef USE_YIELD
        OS_YieldThread();
#else
        (void)OS_DisableScheduler();
        {
            OS_WakeupThreadDirect(&thread2);
            OS_WakeupThreadDirect(&thread3);
        }
        (void)OS_EnableScheduler();
        OS_SleepThread(NULL);
#endif
    }
}

//--------------------------------------------------------------------------------
//    proc2
//
void proc2(void *arg)
{
#pragma unused( arg )
    while (1)
    {
        OS_Printf("Thread 2\n");

#ifdef USE_YIELD
        OS_YieldThread();
#else
        (void)OS_DisableScheduler();
        {
            OS_WakeupThreadDirect(&thread1);
            OS_WakeupThreadDirect(&thread3);
        }
        (void)OS_EnableScheduler();
        OS_SleepThread(NULL);
#endif
    }
}

//--------------------------------------------------------------------------------
//    proc3
//
void proc3(void *arg)
{
#pragma unused( arg )
    while (1)
    {
        OS_Printf("Thread 3\n");

#ifdef USE_YIELD
        OS_YieldThread();
#else
        (void)OS_DisableScheduler();
        {
            OS_WakeupThreadDirect(&thread1);
            OS_WakeupThreadDirect(&thread2);
        }
        (void)OS_EnableScheduler();
        OS_SleepThread(NULL);
#endif
    }
}

/*====== End of main.c ======*/

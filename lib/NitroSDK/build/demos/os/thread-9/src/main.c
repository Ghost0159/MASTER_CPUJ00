/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - thread-9
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.2  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.1  2005/07/12 07:54:44  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

//================================================================================
//   demo about OS_KillThread() and OS_KillThreadWithPriority().
//================================================================================


#ifdef SDK_THREAD_INFINITY
void    MY_CreateThread1(void (*func) (void *), u32 priority);
void    MY_ThreadProc1(void *arg);
void    MY_Destructor(void *arg);

#define	MY_STACK_SIZE	1024

OSThread MY_Thread1;

u64     MY_Stack1[MY_STACK_SIZE / sizeof(u64)];
#endif // ifdef SDK_THREAD_INFINITY

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

#ifdef SDK_THREAD_INFINITY
    //================================================================
    // test1
    //     The thread1 priority is lower than main thread.
    //     Killing thread1, main thread will keep running
    //     and thread1 destructor will be called when main thread sleeps.
    //
    //     (Thread1 task will not run because of low priority.)
    //     
    OS_Printf("==== test1\n");

    //---- create thread 1
    MY_CreateThread1(MY_ThreadProc1, 20);

    //---- kill thread1
    OS_Printf("kill thread1\n");
    OS_KillThread(&MY_Thread1, (void *)1);
    OS_Printf("main thread after kill thread1\n");

    //---- wait thread1 finish
    OS_JoinThread(&MY_Thread1);


    //================================================================
    // test2
    //     The thread1 priority is higher than main thread.
    //     Killing thread1, thread1 destructor will be called immediately.
    //     
    OS_Printf("==== test2\n");

    //---- create thread 1
    MY_CreateThread1(MY_ThreadProc1, 10);

    //---- kill thread 1
    OS_Printf("kill thread1\n");
    OS_KillThread(&MY_Thread1, (void *)2);
    OS_Printf("main thread after kill thread1\n");

    //---- wait thread1 finish
    OS_JoinThread(&MY_Thread1);

    //================================================================
    // test3
    //     The thread1 priority is lower than main thread.
    //     Killing thread1 with specifying high priority,
    //     thread 1 destructor will be called immediately.
    //
    //     (Thread1 task will not run because of low priority.)
    //     
    OS_Printf("==== test3\n");

    //---- create thread 1
    MY_CreateThread1(MY_ThreadProc1, 20);

    //---- kill thread 1
    OS_Printf("kill thread1\n");
    OS_KillThreadWithPriority(&MY_Thread1, (void *)3, 5 /*high priority than main */ );
    OS_Printf("main thread after kill thread1\n");

    //---- wait thread1 finish
    OS_JoinThread(&MY_Thread1);


#endif // ifdef SDK_THREAD_INFINITY

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}


#ifdef SDK_THREAD_INFINITY
/*---------------------------------------------------------------------------*
  Name:         MY_CreateThread1

  Description:  create thread1

  Arguments:    func     : thread task function
  				priority : thread priority

  Returns:      None
 *---------------------------------------------------------------------------*/
void MY_CreateThread1(void (*func) (void *), u32 priority)
{
    OS_CreateThread(&MY_Thread1, func, (void *)0, MY_Stack1 + MY_STACK_SIZE / sizeof(u64),
                    MY_STACK_SIZE, priority);
    OS_SetThreadDestructor(&MY_Thread1, MY_Destructor);
    OS_WakeupThreadDirect(&MY_Thread1);
}


/*---------------------------------------------------------------------------*
  Name:         MY_ThreadProc1

  Description:  thread1 task

  Arguments:    arg : not used

  Returns:      None
 *---------------------------------------------------------------------------*/
void MY_ThreadProc1(void *arg)
{
#pragma unused( arg )
    while (1)
    {
        OS_Printf("called thread1.\n");
        OS_SleepThread(NULL);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MY_Destructor

  Description:  thread destructor

  Arguments:    arg : argument

  Returns:      None
 *---------------------------------------------------------------------------*/
void MY_Destructor(void *arg)
{
#ifdef SDK_FINALROM
#pragma unused( arg )
#endif
    OS_Printf("thread1 destructor is called. (test%d) \n", (int)arg);
}

#endif // ifdef SDK_THREAD_INFINITY

/*====== End of main.c ======*/

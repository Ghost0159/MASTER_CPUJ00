/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - thread-7
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

  Revision 1.3  2005/07/12 02:54:39  yada
  NITRO_THREAD_INFINITH -> SDK_THREAD_INFINITY

  Revision 1.2  2005/07/12 00:54:15  yada
  remove test undef line

  Revision 1.1  2005/07/08 12:04:06  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>


//================================================================================
//   thread destructor test.
//================================================================================

#ifdef SDK_THREAD_INFINITY
void    MY_CreateThread1(void (*func) (void *), u32 priority);
void    MY_ThreadProc1(void *arg);
void    MY_ThreadProc2(void *arg);
void    MY_ThreadProc3(void *arg);
void    MY_ThreadProc4(void *arg);
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
    //      Thread1 will be killed by OS_KillThread.
    //      Destructor will be called.
    //
    OS_Printf("==== test1\n");
    MY_CreateThread1(MY_ThreadProc1, 10);
    OS_KillThread(&MY_Thread1, (void *)0x1234);

    //================================================================
    // test2
    //      Thread1 will be killed by OS_DestroyThread.
    //      Destructor won't be called.
    OS_Printf("==== test2\n");
    MY_CreateThread1(MY_ThreadProc1, 10);
    OS_DestroyThread(&MY_Thread1);

    //================================================================
    // test3
    //      Thread1 will reach to end of task.
    //      Destructor will be called.
    OS_Printf("==== test3\n");
    MY_CreateThread1(MY_ThreadProc2, 10);

    //================================================================
    // test4
    //      Thread1 will be dead by kill itself by OS_KillThread.
    //      Destructor will be called.
    OS_Printf("==== test4\n");
    MY_CreateThread1(MY_ThreadProc3, 10);

    //================================================================
    // test5
    //      Thread1 will be dead by kill itself by OS_DestroyThread.
    //      Destructor won't be called.
    OS_Printf("==== test5\n");
    MY_CreateThread1(MY_ThreadProc4, 10);
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
  Name:         MY_ThreadProc1 / 2 / 3 / 4

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

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void MY_ThreadProc2(void *arg)
{
#pragma unused( arg )

    OS_Printf("called thread1.\n");
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void MY_ThreadProc3(void *arg)
{
#pragma unused( arg )

    OS_Printf("called thread1.\n");
    OS_KillThread(OS_GetCurrentThread(), (void *)0x5678);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void MY_ThreadProc4(void *arg)
{
#pragma unused( arg )

    OS_Printf("called thread1.\n");
    OS_DestroyThread(OS_GetCurrentThread());
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
    OS_Printf("called thread1 destructor.  arg=%x\n", arg);
}

#endif // ifdef SDK_THREAD_INFINITY

/*====== End of main.c ======*/

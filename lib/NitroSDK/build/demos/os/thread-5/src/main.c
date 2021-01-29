/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - thread-5
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.9  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.8  2005/03/03 13:01:54  takano_makoto
  exclude any code which is available in specific compiler.

  Revision 1.7  2005/02/28 05:26:20  yosizaki
  do-indent.

  Revision 1.6  2004/09/03 12:07:34  yada
  only fix comment

  Revision 1.5  2004/06/25 04:47:21  yada
  OS_JoinThread() and  OS_IsThreadTerminate() sample

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#define STACK_SIZE       1024
#define THREAD1_PRIO     17
#define MESSAGE_EXIT     (OSMessage)100

void    VBlankIntr(void);
void    proc1(void *p1);

OSMessage mesgBuffer[10];
OSMessageQueue mesgQueue;

OSThread thread1;
u32     stack1[STACK_SIZE / sizeof(u32)];

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void    OS_WaitInterrupt(BOOL clearEnable, OSIrqMask intrFlags);
void NitroMain()
{
    OS_Init();
    OS_InitThread();
#ifdef SDK_ARM9
    GX_Init();
#endif // SDK_ARM9
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr(TRUE);

    OS_InitMessageQueue(&mesgQueue, &mesgBuffer[0], 10);

    //================ test 1
    //---- create thread
    OS_CreateThread(&thread1, proc1, NULL, stack1 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD1_PRIO);
    OS_WakeupThreadDirect(&thread1);

    //---- send message to be die for thread1
    OS_Printf("terminate thread1.\n");
    (void)OS_SendMessage(&mesgQueue, (OSMessage)MESSAGE_EXIT, OS_MESSAGE_BLOCK);
    OS_JoinThread(&thread1);
    OS_Printf("terminated.\n");


    //================ test 2
    //---- create thread
    OS_CreateThread(&thread1, proc1, NULL, stack1 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD1_PRIO);
    OS_WakeupThreadDirect(&thread1);

    //---- send message to be die for thread1
    OS_Printf("terminate thread1.\n");
    (void)OS_SendMessage(&mesgQueue, (OSMessage)MESSAGE_EXIT, OS_MESSAGE_BLOCK);

    //---- wait till thread1 die
    while (!OS_IsThreadTerminated(&thread1))
    {
        OS_WaitIrq(1, OS_IE_V_BLANK);

        // Notice: if you write OS_WaitInterrupt() instead of OS_WaitIrq(),
        //         thread1 will not die because OS_WaitInterrupt() doesn't
        //         cause to switch thread.  OS_WaitIrq() do.
    }

    OS_Printf("terminated.\n");


    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

//--------------------------------------------------------------------------------
//    VBlank interrupt handler
//
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//--------------------------------------------------------------------------------
//    proc1, which is procedure of thread1
//
void proc1(void *p1)
{
#pragma unused(p1)
    OSMessage message;

    while (1)
    {
        (void)OS_ReceiveMessage(&mesgQueue, &message, OS_MESSAGE_BLOCK);

        if (message == MESSAGE_EXIT)
        {
            OS_Printf("thread1 will die...\n");
            OS_ExitThread();
        }
    }
}


/*====== End of main.c ======*/

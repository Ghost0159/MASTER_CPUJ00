/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - thread-3
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.13  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.12  2005/03/03 12:59:09  takano_makoto
  exclude any code which is available in specific compiler.

  Revision 1.11  2005/02/28 05:26:20  yosizaki
  do-indent.

  Revision 1.10  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.9  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.8  2004/02/26 05:51:46  yada
  OS_CreateThread() に起動関数の引数が追加され、
  それに伴う修正

  Revision 1.7  2004/02/12 11:41:07  yada
  ARM9/ARM7 振り分け作業

  Revision 1.6  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.5  2003/12/25 07:32:21  yada
  型ルール統一による変更

  Revision 1.4  2003/11/30 06:34:00  yada
  OS_CreateThread() 引数を変更

  Revision 1.3  2003/11/26 02:33:37  yada
  (none)

  Revision 1.2  2003/11/26 02:08:58  yada
  (none)

  Revision 1.1  2003/11/26 02:04:03  yada
  初版


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#define STACK_SIZE  1024

void    proc1(void *p1);
void    proc2(void *p1);

OSThread thread1;
OSThread thread2;

OSMessage mesgBuffer[10];
OSMessageQueue mesgQueue;


u64     stack1[STACK_SIZE / sizeof(u64)];
u64     stack2[STACK_SIZE / sizeof(u64)];

#define   THREAD1_PRIO  1
#define   THREAD2_PRIO  2

//================================================================================
//  メッセージキューを使用したスレッド切り替えサンプル
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    s32     n;

    OS_InitMessageQueue(&mesgQueue, &mesgBuffer[0], 10);

    OS_InitThread();
    OS_CreateThread(&thread1, proc1, NULL, stack1 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD1_PRIO);
    OS_CreateThread(&thread2, proc2, NULL, stack2 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD2_PRIO);
    OS_WakeupThreadDirect(&thread1);
    OS_WakeupThreadDirect(&thread2);

    for (n = 0; n < 5; n++)
    {
        OSMessage message;

        OS_Printf("Idle\n");

        message = (OSMessage)0x12345;
        OS_Printf("send message = %x\n", message);
        (void)OS_SendMessage(&mesgQueue, message, OS_MESSAGE_BLOCK);
    }

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

//--------------------------------------------------------------------------------
//    proc1
//
void proc1(void *p1)
{
#pragma unused(p1)
    while (1)
    {
        OSMessage message;

        (void)OS_ReceiveMessage(&mesgQueue, &message, OS_MESSAGE_BLOCK);
        OS_Printf("recv message = %x\n", message);

        OS_WakeupThreadDirect(&thread2);
    }
}

//--------------------------------------------------------------------------------
//    proc2
//
void proc2(void *p1)
{
#pragma unused(p1)
    while (1)
    {
        OS_Printf("Thread2\n");
        OS_SleepThread(NULL);
    }
}

/*====== End of main.c ======*/

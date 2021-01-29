/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - waitIrq-2
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

  Revision 1.6  2005/03/03 13:02:59  takano_makoto
  exclude any code which is available in specific compiler.

  Revision 1.5  2005/02/28 05:26:20  yosizaki
  do-indent.

  Revision 1.4  2004/12/08 05:18:04  yada
  use timer interrupt instead of key interrupt

  Revision 1.3  2004/08/06 12:45:46  yada
  change key interrupt logic enum

  Revision 1.2  2004/08/06 05:14:04  yada
  PAD_SetIrq -> PADi_SetIrq, PAD_ClearIrq -> PADi_ClearIrq

  Revision 1.1  2004/05/20 00:54:45  yada
  initial release

  Revision 1.2  2004/04/30 05:02:25  yada
  add comment

  Revision 1.1  2004/04/30 04:37:56  yada
  first release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    TimerIntr(void);
void    proc1(void *p1);
void    proc2(void *p1);

#define    STACK_SIZE     1024
#define    THREAD1_PRIO   17
#define    THREAD2_PRIO   18
OSThread thread1;
OSThread thread2;
u32     stack1[STACK_SIZE / sizeof(u32)];
u32     stack2[STACK_SIZE / sizeof(u32)];

/* ----------------
 * (↓)このdefineをどちらか一方有効にして下さい。
 *     そして、動作の違いを確認して下さい。   */

//#define WAIT_TIMER
#define WAIT_ANY

/*  thread1, thread2 の2つのスレッドが IRQ を待ってとまります。
 *  WAIT_TIMERを有効にした場合は thread1 のみが動き、
 *  WAIT_ANYを有効にした場合は thread1 と thread2 が動きます。
 *  これは、WAIT_TIMER の場合、タイマ割込みによって thread1 がOS_WaitIrq() から復帰した後、
 *  再び thread1 が割込みチェックフラグをクリアしてからOS_WaitIrq() に入るからです。
 *  一旦 proc2 は動作可能状態になるのですが、キー割込みチェックフラグがすでにクリア
 *  されているので OS_WaitIrq() から抜けられません。
 *
 *  WAIT_ANY の場合、割り込みが発生した瞬間に thread1とthread2を
 *  実行可能状態にしますので、thread1が次の OS_WaitAnyIrq()に
 *  来たときに thread2 が動けるのです。
 */

// どちらか一方を有効にしたかのチェック
#if (defined(WAIT_TIMER) && defined(WAIT_ANY)) || (!defined(WAIT_TIMER) && !defined(WAIT_ANY))
*****ERROR.define either from WAIT_TIMER or WAIT_ANY.
#endif
//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    OS_Init();
    OS_StartTimer(OS_TIMER_0, 0xff, OS_TIMER_PRESCALER_1024);

    //---- timer interrupt setting
    (void)OS_SetIrqFunction(OS_IE_TIMER0, TimerIntr);
    (void)OS_EnableIrqMask(OS_IE_TIMER0);
    (void)OS_EnableIrq();

    //---- create and wakeup threads
    OS_InitThread();
    OS_CreateThread(&thread1, proc1, NULL, stack1 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD1_PRIO);
    OS_CreateThread(&thread2, proc2, NULL, stack2 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD2_PRIO);
    OS_WakeupThreadDirect(&thread1);
    OS_WakeupThreadDirect(&thread2);

    OS_Printf("please push any button.\n");

    //---- idle loop
    while (1)
    {
        OS_SleepThread(NULL);
        // never wake up
    }

    // never reach
}

//--------------------------------------------------------------------------------
//    Timer interrupt handler
//
void TimerIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_TIMER0);
}

//--------------------------------------------------------------------------------
//    proc1
//
void proc1(void *p1)
{
#pragma unused(p1)
    int     n = 0;

    while (1)
    {
        OS_Printf("thread 1\n");
#ifdef WAIT_TIMER
        OS_WaitIrq(TRUE, OS_IE_TIMER0);
#endif
#ifdef WAIT_ANY
        OS_WaitAnyIrq();
#endif
    }
}

//--------------------------------------------------------------------------------
//    proc2
//
void proc2(void *p1)
{
#pragma unused(p1)
    int     n = 0;
    while (1)
    {
        OS_Printf("thread 2\n");
#ifdef WAIT_TIMER
        OS_WaitIrq(TRUE, OS_IE_TIMER0);
#endif
#ifdef WAIT_ANY
        OS_WaitAnyIrq();
#endif
    }
}


/*====== End of main.c ======*/

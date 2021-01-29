/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - thread-4
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.14  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.13  2005/11/21 10:56:02  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.12  2005/03/03 13:00:06  takano_makoto
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

  Revision 1.7  2004/02/24 00:05:20  yada
  (none)

  Revision 1.6  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.5  2004/02/02 05:34:12  yada
  OS_SetIrqMask()→OS_EnableIrqMask() へ

  Revision 1.4  2004/01/18 06:49:20  yada
  (none)

  Revision 1.3  2004/01/09 10:53:49  yada
  割込みチェックフラグのセット関数書き換え

  Revision 1.2  2003/12/25 07:32:21  yada
  型ルール統一による変更

  Revision 1.1  2003/12/16 06:30:36  yasu
  thread-4 割り込みによる thread 切り替え処理のサンプル

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    VBlankIntr(void);

#define    STACK_SIZE     1024
#define    THREAD1_PRIO   1
#define    THREAD2_PRIO   2

void    proc1(void *p1);
void    proc2(void *p1);

OSThread thread1;
OSThread thread2;

u32     stack1[STACK_SIZE / sizeof(u32)];
u32     stack2[STACK_SIZE / sizeof(u32)];

//================================================================================
//      割り込みを利用したスレッドの切り替えのサンプル
//      IRQ ハンドラ内でスレッドを寝させたり起こしたり出来る、ただし実際に
//      スレッドの切り替えが起こるのは IRQ ルーチン終了まで延期される．
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    s32     n;

    OS_Init();

    //================ スレッド起動
    OS_InitThread();
    OS_CreateThread(&thread1, proc1, NULL, stack1 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD1_PRIO);
    OS_CreateThread(&thread2, proc2, NULL, stack2 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD2_PRIO);
    OS_WakeupThreadDirect(&thread1);
    OS_WakeupThreadDirect(&thread2);

    //================ 設定
#ifdef SDK_ARM9
    //---- グラフィクス初期化
    GX_Init();
#endif // SDK_ARM9

    //----  Vブランク割込許可
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();

    //---- Vブランク発生設定
    (void)GX_VBlankIntr(TRUE);

    //================ メインループ
    for (n = 0; n < 5; n++)
    {
        //---- Vブランク割込終了待ち
        OS_WaitVBlankIntr();

        OS_Printf("Idle\n");
    }

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

//--------------------------------------------------------------------------------
//    Ｖブランク割り込み処理 (IRQ 内処理)
//
void VBlankIntr(void)
{
    //---- 割り込みチェックフラグ
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);

    //---- スレッド１起動
    OS_WakeupThreadDirect(&thread1);
}

//--------------------------------------------------------------------------------
//    proc1
//
void proc1(void *p1)
{
#pragma unused(p1)
    while (1)
    {
        OS_Printf("------- Thread1 sleep\n");
        OS_SleepThread(NULL);
        OS_Printf("------- Thread1 wakeup\n");

        //---- スレッド２起動
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
        OS_Printf("--------------- Thread2 sleep\n");
        OS_SleepThread(NULL);
        OS_Printf("--------------- Thread2 wakeup\n");
    }
}

/*====== End of main.c ======*/

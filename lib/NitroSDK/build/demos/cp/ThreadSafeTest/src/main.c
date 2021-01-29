/*---------------------------------------------------------------------------*
  Project:  NitroSDK - ThreadSafeTest - CP
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.6  2007/05/22 01:26:32  okubata_ryoma
  GX_BlankScr を GX_DispOn に変更

  Revision 1.5  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.4  2005/11/21 10:20:22  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.3  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.2  2004/09/03 13:45:27  takano_makoto
  OS_Panic -> OS_Terminate

  Revision 1.1  2004/05/17 05:17:38  takano_makoto
  Initial Update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// 除算器、平方根演算器のスレッドセーフテスト
// HOWTO:
// 1. スレッドの切り替え時には、スレッドの切り替えと同時に除算機・平方根演算機の状態
//    も切り替えられるため、特別な処理を記述しなくてもスレッドセーフとなる。
//
// 2．割り込み内で、除算機・平方根演算機を使用する場合は 前後でCP_SaveContext(),
//    CP_RestoreContext()を呼ぶことで、除算機・平方根演算機の状態が復元され安全に
//    使用することができる。
//---------------------------------------------------------------------------

#include <nitro.h>


/*---------------------------------------------------------------------------*
    プロトタイプ宣言
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void);
void    proc1(void *arg);
void    proc2(void *arg);


/*---------------------------------------------------------------------------*
    定数定義
 *---------------------------------------------------------------------------*/
#define A1  0x40F337
#define B1  0x197
#define A2  0x7843276F4561
#define B2  0x3208F1


#define     THREAD1_PRIO    10
#define     THREAD2_PRIO    11

#define STACK_SIZE  1024


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/
u64     stack1[STACK_SIZE / sizeof(u64)];
u64     stack2[STACK_SIZE / sizeof(u64)];

OSThread thread1;
OSThread thread2;


//---------------------------------------------------------------------------
// Main Loop.
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u64     a;
    u32     b;
    u64     c;

    //---------------------------------------------------------------------------
    //  初期化
    //---------------------------------------------------------------------------
    OS_Init();
    OS_InitThread();
    FX_Init();

    GX_Init();
    GX_SetPower(GX_POWER_ALL);

    GX_DispOn();

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_SetIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();

    reg_GX_DISPSTAT = 8;


    OS_Printf("Correct Answer: %x / %x = %x\n", A1, B1, A1 / B1);
    OS_Printf("Correct Answer: %llx / %x = %llx\n\n", A2, B2, A2 / B2);

    a = A2;
    b = B2;


    // 計算途中でVBlankを待つ
    CP_SetDiv64_32(a, b);
    OS_Printf("Main: Start %llx / %x ...\n", a, b);

    CP_WaitDiv();
    OS_WaitVBlankIntr();               // Waiting the end of VBlank interrupt

    c = (u64)CP_GetDivResult64();

    OS_Printf("Main: Get %llx / %x = %llx\n", a, b, c);


    // スレッドの開始
    OS_Printf("----------------------------\n");
    OS_Printf("Create Thread\n");

    OS_CreateThread(&thread1, proc1, (void *)0x111, stack1 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD1_PRIO);
    OS_CreateThread(&thread2, proc2, (void *)0x222, stack2 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD2_PRIO);

    OS_WakeupThreadDirect(&thread1);
    OS_WakeupThreadDirect(&thread2);

    OS_Printf("Idle\n");
    OS_WakeupThreadDirect(&thread2);


    // 終了
    OS_Printf("==== Finish sample.\n");
    OS_Terminate();

    while (1)
    {

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

    }
}


//---------------------------------------------------------------------------
// VBlank interrupt function:
//---------------------------------------------------------------------------
static void VBlankIntr(void)
{
    u32     a, b, c;
    CPContext context;

    // 演算機コンテキストの保存
    CP_SaveContext(&context);

    a = A1;
    b = B1;

    CP_SetDiv32_32(a, b);
    OS_Printf("VBlank: Start %x / %x ...\n", a, b);
    CP_WaitDiv();
    c = (u32)CP_GetDivResult32();

    OS_Printf("VBlank: Get %x / %x = %x\n", a, b, c);

    // 演算機コンテキストの復元
    CP_RestoreContext(&context);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt


}


//--------------------------------------------------------------------------------
//    proc1
//
void proc1(void *arg)
{
#pragma unused(arg)
    u64     a;
    u32     b;
    u64     c;

    while (1)
    {
        OS_Printf("Sleep Thread1\n");
        OS_SleepThread(NULL);
        OS_Printf("Wake Thread1 (Priority %d)\n", THREAD1_PRIO);

        a = A2;
        b = B2;

        CP_SetDiv64_32(a, b);
        OS_Printf("Thread1: Start %llx / %x ...\n", a, b);
        CP_WaitDiv();
        c = (u64)CP_GetDivResult64();

        OS_Printf("Thread1: Get %llx / %x = %x\n", a, b, c);
    }
}

//--------------------------------------------------------------------------------
//    proc2
//
void proc2(void *arg)
{
#pragma unused(arg)
    u32     a, b, c;

    while (1)
    {
        OS_Printf("Sleep Thread2\n");
        OS_SleepThread(NULL);
        OS_Printf("WakeThread2: (Priority %d)\n", THREAD2_PRIO);

        a = A1;
        b = B1;

        CP_SetDiv32_32(a, b);
        OS_Printf("Thread2: Start %x / %x ...\n", a, b);
        CP_WaitDiv();

        // 計算途中でスレッド１の呼び出し
        OS_WakeupThreadDirect(&thread1);
        c = (u32)CP_GetDivResult32();
        OS_Printf("Thread2: Get %x / %x = %x\n", a, b, c);
    }
}

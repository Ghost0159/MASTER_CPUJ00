/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - heap-3
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.8  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.7  2005/02/28 05:26:20  yosizaki
  do-indent.

  Revision 1.6  2004/09/28 12:22:10  yada
  not call OS_InitArena() because OS_Init() called

  Revision 1.5  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.4  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.3  2004/02/12 11:41:07  yada
  ARM9/ARM7 振り分け作業

  Revision 1.2  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.1  2004/01/07 08:06:54  yada
  初版

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

//----------------------------------------------------------------
#define   MAIN_HEAP_SIZE    0x2000

//================================================================================
//        ヒープのサンプル
//        メインメモリのアリーナに対し ヒープを作成。
//        OS_AllocFixed を使い、不連続ヒープを作成する。
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    void   *heapStart[2];
    void   *nstart;
    OSHeapHandle handle[2];
    void   *area[10];

    void   *fixed;
    void   *fixedStart;
    void   *fixedEnd;

    OS_Init();
    //OS_InitArena(); /*called in OS_Init()*/
    OS_Printf("arena lo = %x\n", OS_GetMainArenaLo());
    OS_Printf("arena hi = %x\n", OS_GetMainArenaHi());


    //---- MainRAM アリーナに対して メモリ割り当てシステム初期化
    nstart = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 2);
    OS_SetMainArenaLo(nstart);



    //==== ヒープ０====
    //---- アリーナからヒープ用領域確保
    heapStart[0] = OS_AllocFromMainArenaLo(MAIN_HEAP_SIZE, 32);
    OS_Printf("heapStart (0) %x\n", heapStart[0]);

    //---- ヒープ作成
    handle[0] =
        OS_CreateHeap(OS_ARENA_MAIN, heapStart[0], (void *)((u32)heapStart[0] + MAIN_HEAP_SIZE));
    OS_Printf("heap handle (0) %d\n", handle[0]);



    //==== ヒープ１====
    //---- アリーナからヒープ用領域確保
    heapStart[1] = OS_AllocFromMainArenaLo(MAIN_HEAP_SIZE, 32);
    OS_Printf("heapStart (1) %x\n", heapStart[1]);

    //---- ヒープ作成
    handle[1] =
        OS_CreateHeap(OS_ARENA_MAIN, heapStart[1], (void *)((u32)heapStart[1] + MAIN_HEAP_SIZE));
    OS_Printf("heap handle (1) %d\n", handle[1]);


    //---- ヒープ１のうちの一部の領域を OS_FixedAlloc で取得
    OS_Printf("---------------- now alloc fixed area\n");
    fixedStart = (void *)((u32)heapStart[1] + 0x300);
    fixedEnd = (void *)((u32)heapStart[1] + 0x500);
    fixed = OS_AllocFixed(OS_ARENA_MAIN, &fixedStart, &fixedEnd);
    OS_Printf("fixed area %x  (%x-%x)\n", fixed, fixedStart, fixedEnd);

    //---- ヒープ情報表示
    OS_DumpHeap(OS_ARENA_MAIN, handle[0]);
    OS_DumpHeap(OS_ARENA_MAIN, handle[1]);


    //---- Fixed 領域を ヒープ０に付ける
    OS_Printf("---------------- now add to heap...\n");
    OS_AddToHeap(OS_ARENA_MAIN, handle[0], fixedStart, fixedEnd);

    //---- ヒープ情報表示
    OS_DumpHeap(OS_ARENA_MAIN, handle[0]);
    OS_DumpHeap(OS_ARENA_MAIN, handle[1]);



    //---- カレントヒープ設定
    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, handle[0]);


    //---- 領域確保してみる
    area[0] = OS_Alloc(0x1f00);
    area[1] = OS_Alloc(0x100);
    OS_Printf("area[0] %x\n", area[0]);
    OS_Printf("area[1] %x\n", area[1]);

    //---- ヒープ情報表示
    OS_DumpHeap(OS_ARENA_MAIN, handle[0]);
    OS_DumpHeap(OS_ARENA_MAIN, handle[1]);

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

/*====== End of main.c ======*/

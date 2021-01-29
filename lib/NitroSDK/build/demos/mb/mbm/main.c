/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.8  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.7  2005/12/26 00:24:14  kitase_hirotake
  PXI_InitFifo の削除

  Revision 1.6  2005/11/21 10:50:34  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.5  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.4  2004/11/24 06:15:01  yosizaki
  rename MB_StartParentEx/MB_EndEx.

  Revision 1.3  2004/10/21 06:28:00  takano_makoto
  コメント追加

  Revision 1.2  2004/09/28 09:58:37  takano_makoto
  channel_setting をvalatileに修正

  Revision 1.1  2004/09/28 06:50:04  takano_makoto
  initial update.


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//--------------------------------------------------------------------
// マルチブート時のチャンネル決定サンプル
// 
// 電波使用率の計測は無線IDLE状態でのみ使用可能なため、
// マルチブート時にはMB開始前に前処理としてチャンネルの決定をする必要が
// あります。
//
// このサンプルのMBM_MeasureChannel()は、無線パワーOFF状態でコールすると、
// 内部で最適なチャンネルを調査して無線パワーOFF状態に戻してから
// コールバックで結果を返すという造りになっています。
// 
// (※NITRO-SDK2.0rc2で追加されたMB_StartParentFromIdle,
//    MB_EndToIdleを利用する方法もあります)
//--------------------------------------------------------------------


#include <nitro.h>

#include "mb_measure_channel.h"


static void my_callback(MBMCallback * cb);
static vu16 channel_setting = 0;
static int prog_state;


/******************************************************************************/
/* function */

/* Ｖブランク割り込み処理 */
static void VBlankIntr(void)
{
    //---- 割り込みチェックフラグ
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/* メイン */
void NitroMain()
{
    /* OS 初期化 */
    OS_Init();
    OS_InitThread();
    OS_InitTick();
    OS_InitAlarm();
    FX_Init();

    /* GX 初期化 */
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    {                                  /* メモリ割り当ての初期化 */
        OSHeapHandle heapHandle;       // Heapハンドル
        enum
        { MAIN_HEAP_SIZE = 0x80000 };
        void   *heapStart, *nstart;
        nstart = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 16);
        OS_SetMainArenaLo(nstart);
        heapStart = OS_AllocFromMainArenaLo((u32)MAIN_HEAP_SIZE, 32);
        heapHandle =
            OS_CreateHeap(OS_ARENA_MAIN, heapStart, (void *)((u32)heapStart + MAIN_HEAP_SIZE));
        if (heapHandle < 0)
        {
            OS_Panic("ARM9: Fail to create heap...\n");
        }
        heapHandle = OS_SetCurrentHeap(OS_ARENA_MAIN, heapHandle);
    }

    /* Vブランク割込設定 */
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    /* 表示開始 */
    GX_DispOn();
    GXS_DispOn();
    G2_SetBG0Offset(0, 0);

    channel_setting = 0;

    {
        void   *wm_sys_buf_ptr = OS_Alloc(WM_SYSTEM_BUF_SIZE);
        MBM_MeasureChannel((u8 *)wm_sys_buf_ptr, my_callback);
        while (channel_setting == 0)
        {
            OS_WaitVBlankIntr();
        }
        OS_Free(wm_sys_buf_ptr);
    }

    /* メインループ */
    for (;;)
    {
        OS_WaitVBlankIntr();
    }
}


static void my_callback(MBMCallback * cb)
{
    channel_setting = 1;
    if (cb->errcode == MBM_MEASURE_SUCCESS)
    {
        OS_TPrintf("measure channel = %d\n", cb->channel);
    }
    else
    {
        OS_TPrintf("get measure channel fail\n");
    }
}

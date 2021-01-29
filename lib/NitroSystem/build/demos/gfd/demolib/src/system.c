/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - gfd - demolib
  File:     system.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/
#include "gfd_demolib/system.h"


#define SYSTEM_HEAP_SIZE        64*1024

#define GFDDEMO_ROUND_UP(value, alignment) \
    (((u32)(value) + (alignment-1)) & ~(alignment-1))

#define GFDDEMO_ROUND_DOWN(value, alignment) \
    ((u32)(value) & ~(alignment-1))

static NNSFndHeapHandle gfdDemoAppHeap;     // アプリケーションヒープ（拡張ヒープを使用）


/*---------------------------------------------------------------------------*
    初期化関係。
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         VBlankIntr_

  Description:  Vブランクのコールバックルルーチン。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
VBlankIntr_(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         GFDDemo_CommonInit

  Description:  OS、グラフィックスエンジン、ヒープ、ファイルシステムの
                初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void GFDDemo_CommonInit(void)
{
    OS_Init();
    FX_Init();
    GX_SetPower(GX_POWER_ALL);
    GX_Init();
    OS_InitTick();

    GX_DispOff();
    GXS_DispOff();

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr_ );

    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);
    (void)OS_EnableIrq();

    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request
    
    {
        void* sysHeapMemory = OS_AllocFromMainArenaLo(SYSTEM_HEAP_SIZE, 16);
        u32   arenaLow      = GFDDEMO_ROUND_UP  (OS_GetMainArenaLo(), 16);
        u32   arenaHigh     = GFDDEMO_ROUND_DOWN(OS_GetMainArenaHi(), 16);
        u32   appHeapSize   = arenaHigh - arenaLow;
        void* appHeapMemory = OS_AllocFromMainArenaLo(appHeapSize, 16);
        gfdDemoAppHeap      = NNS_FndCreateExpHeap(appHeapMemory, appHeapSize);
    }
}


void* GfDDemo_Allock( u32 size )
{
    return NNS_FndAllocFromExpHeapEx( gfdDemoAppHeap, size, 16);
}

void GfDDemo_Free( void* pBlk )
{
    NNS_FndFreeToExpHeap( gfdDemoAppHeap, pBlk );
}


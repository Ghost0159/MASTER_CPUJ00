/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WVR - demos - switchover
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.3  2005/11/21 11:08:15  kitase_hirotake
  OS_InitPrintServer の削除

  Revision 1.2  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.1  2005/02/16 12:47:54  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include "common.h"


/*---------------------------------------------------------------------------*
    関数定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         InitializeAllocateSystem

  Description:  メインメモリ上のアリーナにてメモリ割当てシステムを初期化する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    // OS_Initは呼ばれているという前提
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  初期化及びメインループ。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /* ライブラリの初期化 */
    OS_Init();
    FX_Init();
    GX_Init();

    /* メモリ割当 */
    InitializeAllocateSystem();

    {
        BOOL    bSwitch = FALSE;
        for (;; bSwitch = !bSwitch)
        {
            if (bSwitch)
            {
                GraphicMain();
            }
            else
            {
                WirelessMain();
            }
            (void)OS_DisableIrq();
            (void)OS_DisableInterrupts();

            (void)GX_DisableBankForLCDC();
            (void)GX_DisableBankForTex();
            (void)GX_DisableBankForTexPltt();
            (void)GX_DisableBankForBG();
            (void)GX_DisableBankForOBJ();
            (void)GX_DisableBankForSubBG();
            (void)GX_DisableBankForSubOBJ();

        }
    }
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

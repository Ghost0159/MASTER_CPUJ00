/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - fnd - archive2
  File:     sdk_init.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

#include "sdk_init.h"

#define	DEFAULT_DMA_NUMBER		MI_DMA_MAX_NUM


/* -------------------------------------------------------------------------
	V-blank callback
   ------------------------------------------------------------------------- */
void
VBlankIntr(void)
{
	OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}


/*---------------------------------------------------------------------------*
  Name:         InitSystem

  Description:  NITROの初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
InitSystem(void)
{
    OS_Init();
    FX_Init();
	GX_SetPower(GX_POWER_ALL);
    GX_Init();

    GX_DispOff();
    GXS_DispOff();

	OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);

    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);
    (void)OS_EnableIrq();

	FS_Init(DEFAULT_DMA_NUMBER);

    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request
}

/*---------------------------------------------------------------------------*
  Name:         InitVRAM

  Description:  ＶＲＡＭの初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
InitVRAM(void)
{
    // 全てのバンクをLCDCに割り当てる
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);

    // LCDC空間全てをクリア
    MI_CpuClearFast( (void*)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE );
    
    // LCDCに割り当てられたバンクをDisable
    (void)GX_DisableBankForLCDC();

    MI_CpuFillFast ((void*)HW_OAM    , 192, HW_OAM_SIZE   );  // OAM クリア
    MI_CpuFillFast ((void*)HW_DB_OAM , 192, HW_DB_OAM_SIZE);  // clear OAM

    MI_CpuClearFast((void*)HW_PLTT   , HW_PLTT_SIZE   );      // パレット クリア
    MI_CpuClearFast((void*)HW_DB_PLTT, HW_DB_PLTT_SIZE);	  // clear the standard palette
}

/*---------------------------------------------------------------------------*
  Name:         InitDisplay

  Description:  ＢＧ０のみを表示します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
InitDisplay(void)
{
    GX_SetBankForBG (GX_VRAM_BG_128_A );                // バンクＡをＢＧに割り当てる

	GX_SetBGCharOffset(GX_BGCHAROFFSET_0x00000);
	GX_SetBGScrOffset ( GX_BGSCROFFSET_0x10000);

    GX_SetGraphicsMode(
    	GX_DISPMODE_GRAPHICS,       					// グラフィックス表示モードにする
        GX_BGMODE_0,                					// BGMODEを0に設定
        GX_BG0_AS_2D);            						// BG0を2D表示に設定

    GX_SetVisiblePlane(
    	GX_PLANEMASK_BG0								// BG0を表示。
    );

	G2_SetBG0Control(
   		GX_BG_SCRSIZE_TEXT_256x256,           			// 256×256ドット
        GX_BG_COLORMODE_256,                  			// 256色モード。
        GX_BG_SCRBASE_0x0000,                 			// スクリーンベース。
        GX_BG_CHARBASE_0x00000,               			// キャラクタベース。
        GX_BG_EXTPLTT_01                      			// BGExtPlttスロット。
    );

    GX_DispOn();
}

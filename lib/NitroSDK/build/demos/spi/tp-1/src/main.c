/*---------------------------------------------------------------------------*
  Project:  NitroSDK - demos - tp-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.19  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.18  2005/11/21 11:05:06  kitase_hirotake
  PXI_Init の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.17  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.16  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.15  2005/02/22 08:40:30  takano_makoto
  TEGのサポート終了

  Revision 1.14  2005/02/18 08:01:09  terui
  Update copyright

  Revision 1.13  2005/02/18 07:36:13  terui
  NITRO_NVRAM_FORMAT -> SDK_NVRAM_FORMAT

  Revision 1.12  2004/11/02 08:49:05  takano_makoto
  fix comment.

  Revision 1.11  2004/11/02 08:42:39  takano_makoto
  TP_GetUserInfo失敗時の処理をTEG/TSとNITRO_NVRAM_FORMATで切り替えるように修正

  Revision 1.10  2004/09/29 06:07:19  takano_makoto
  TP_RequestSetStabilityをコールしないように修正

  Revision 1.9  2004/06/03 12:59:56  takano_makoto
  メイン画面 <->サブ画面切り替え

  Revision 1.8  2004/05/21 09:56:11  takano_makoto
  NVRAMからキャリブレーション値を取得

  Revision 1.7  2004/05/14 02:01:39  takano_makoto
  Modify Calibrate Parameter format.

  Revision 1.6  2004/04/21 12:14:53  takano_makoto
  Add Sync function.

  Revision 1.5  2004/04/19 12:02:29  takano_makoto
  modify any functions interface.

  Revision 1.4  2004/04/16 02:14:11  takano_makoto
  Modify for STRB warning.

  Revision 1.3  2004/04/15 13:56:07  takano_makoto
  Change any function name.

  Revision 1.2  2004/04/15 11:29:11  takano_makoto
  change header path

  Revision 1.1  2004/04/15 11:16:54  takano_makoto
  Initial Update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/spi/ARM9/tp.h>
#include    "data.h"


/*---------------------------------------------------------------------------*
    Prototype definition
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void);


/*---------------------------------------------------------------------------*
    Static variables definition
 *---------------------------------------------------------------------------*/
static GXOamAttr gOam[128];


/*---------------------------------------------------------------------------*
  Name:         SetPoint16x16

  Description:  Display a 16x16 OBJ on indicated point.

  Arguments:    x - position X.
                y - position Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void SetPoint16x16(u16 pos_x, u16 pos_y)
{
    G2_SetOBJAttr(&gOam[0],            // OAM number
                  pos_x - 8,           // X position
                  pos_y - 8,           // Y position
                  0,                   // Priority
                  GX_OAM_MODE_BITMAPOBJ,        // Bitmap mode
                  FALSE,               // mosaic off
                  GX_OAM_EFFECT_NONE,  // affine off
                  GX_OAM_SHAPE_16x16,  // 16x16 size
                  GX_OAM_COLOR_16,     // 16 color
                  1,                   // charactor
                  15,                  // alpha
                  0);
}


/*---------------------------------------------------------------------------*
  Name:         DisplayInit

  Description:  Graphics Initialization

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DisplayInit()
{
    GX_Init();
    FX_Init();

    GX_DispOff();
    GXS_DispOff();
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request

    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

    (void)GX_DisableBankForLCDC();

    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);   // clear OAM
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);     // clear the standard palette

    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);     // clear OAM
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);       // clear the standard palette

    GX_SetBankForOBJ(GX_VRAM_OBJ_256_AB);       // Set VRAM-A,B for OBJ

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // 2D / 3D Mode
                       GX_BGMODE_0,    // BGMODE 0
                       GX_BG0_AS_2D);  // Set BG0 as 2D

    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);       // make OBJ visible
    GX_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_1D_128K);   // 2D mapping OBJ

    /* Load character bitmap data */
    GX_LoadOBJ((const void *)IMAGE_DATA, 0, IMAGE_DATA_SIZE);   // Transfer OBJ bitmap data to VRAM

    OS_WaitVBlankIntr();               // Waiting the end of VBlank interrupt
    GX_DispOn();

}



/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  Initialization and main loop

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    TPData  raw_point;
    TPData  disp_point;
    TPCalibrateParam calibrate;

    // Initialization
    OS_Init();
    TP_Init();

    // get CalibrateParameter from FlashMemory
    if (!TP_GetUserInfo(&calibrate))
    {
        OS_Panic("FATAL ERROR: can't read UserOwnerInfo\n");
    }
    else
    {
        OS_Printf("Get Calibration Parameter from NVRAM\n");
    }

    TP_SetCalibrateParam(&calibrate);

    DisplayInit();

    // unnecessary called. 
    // be able to use default parameter (retry,range).
#if 0
    // Send parameter of revision noise.
    if (TP_RequestSetStability(3, 15) != 0)
    {
        OS_Panic("SetStability request err!\n");
    }
#endif

    while (TRUE)
    {
        // Draw Marker by calibrated point.
        while (TP_RequestRawSampling(&raw_point) != 0)
        {
        };
        TP_GetCalibratedPoint(&disp_point, &raw_point);

        if (disp_point.touch)
        {
            SetPoint16x16(disp_point.x, disp_point.y);

            switch (disp_point.validity)
            {
            case TP_VALIDITY_VALID:
                OS_Printf("( %d, %d ) -> ( %d, %d )\n", raw_point.x, raw_point.y, disp_point.x,
                          disp_point.y);
                break;
            case TP_VALIDITY_INVALID_X:
                OS_Printf("( *%d, %d ) -> ( *%d, %d )\n", raw_point.x, raw_point.y, disp_point.x,
                          disp_point.y);
                break;
            case TP_VALIDITY_INVALID_Y:
                OS_Printf("( %d, *%d ) -> ( %d, *%d )\n", raw_point.x, raw_point.y, disp_point.x,
                          disp_point.y);
                break;
            case TP_VALIDITY_INVALID_XY:
                OS_Printf("( *%d, *%d ) -> ( *%d, *%d )\n", raw_point.x, raw_point.y, disp_point.x,
                          disp_point.y);
                break;
            }
        }

        /* flush cache of OAM buffers to main memory */
        DC_FlushRange(gOam, sizeof(gOam));
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();
        GX_LoadOAM(gOam, 0, sizeof(gOam));
        MI_DmaFill32(3, gOam, 192, sizeof(gOam));       // Clear OAM buffer

        // Wait V-Blank interrupt
        OS_WaitVBlankIntr();

    }
}


/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V-Blank function

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{

    // Set IRQ check flag
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

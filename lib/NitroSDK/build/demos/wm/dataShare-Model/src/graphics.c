/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-Model
  File:     graphics.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: graphics.c,v $
  Revision 1.3  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.2  2004/11/05 04:27:40  sasakis
  ロビー画面、親機選択画面の追加と、それに必要な改造（scan関連など）。

  Revision 1.1  2004/11/01 04:11:44  sasakis
  処理落ち対策コードなどの追加（作業中コードを含む暫定版）。

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "graphics.h"

void initGraphics(void)
{
    GX_Init();

    GX_DispOff();
    GXS_DispOff();

    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    G3X_Init();
    G3X_InitMtxStack();

    GX_SetBankForTex(GX_VRAM_TEX_0_D);
    GX_SetBankForTexPltt(GX_VRAM_TEXPLTT_0123_E);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_3, GX_BG0_AS_3D);
    G3X_AntiAlias(TRUE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);

    G3_ViewPort(0, 0, 255, 191);

    G2_SetBG0Priority(1);
    G2_SetBG2Priority(0);

    G2_BG0Mosaic(FALSE);
    G2_BG2Mosaic(FALSE);

    GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2);
}

void setupPseudo2DCamera(void)
{
    VecFx32 Eye = { 0, 0, FX32_ONE };
    VecFx32 at = { 0, 0, 0 };
    VecFx32 vUp = { 0, -FX32_ONE, 0 };

    G3_Ortho(96 * PSEUDO2D_ONE, -96 * PSEUDO2D_ONE,
             128 * PSEUDO2D_ONE, -128 * PSEUDO2D_ONE, FX32_ONE, FX32_ONE * 400, NULL);
    G3_StoreMtx(0);
    G3X_Reset();
    G3_LookAt(&Eye, &vUp, &at, NULL);
}

void drawPseudo2DTexQuad(int sx, int sy, int width, int height, int texw, int texh)
{
    sx -= 128;
    sy -= 96;

    G3_Begin(GX_BEGIN_QUADS);

    G3_TexCoord(0, 0);
    G3_Normal(0, 0, FX16_ONE - 1);
    G3_Vtx((fx16)(sx * PSEUDO2D_ONE), (fx16)(sy * PSEUDO2D_ONE), 0);

    G3_TexCoord(FX32_ONE * texw, 0);
    G3_Normal(0, 0, FX16_ONE - 1);
    G3_Vtx((fx16)((sx + width) * PSEUDO2D_ONE), (fx16)(sy * PSEUDO2D_ONE), 0);

    G3_TexCoord(FX32_ONE * texw, FX32_ONE * texh);
    G3_Normal(0, 0, FX16_ONE - 1);
    G3_Vtx((fx16)((sx + width) * PSEUDO2D_ONE), (fx16)((sy + height) * PSEUDO2D_ONE), 0);

    G3_TexCoord(0, FX32_ONE * texh);
    G3_Normal(0, 0, FX16_ONE - 1);
    G3_Vtx((fx16)(sx * PSEUDO2D_ONE), (fx16)((sy + height) * PSEUDO2D_ONE), 0);
    G3_End();
}

void drawPseudo2DColorQuad(int sx, int sy, int width, int height, GXRgb color)
{
    sx -= 128;
    sy -= 96;

    G3_Begin(GX_BEGIN_QUADS);
    G3_Color(color);
    G3_Vtx((fx16)(sx * PSEUDO2D_ONE), (fx16)(sy * PSEUDO2D_ONE), 0);
    G3_Vtx((fx16)((sx + width) * PSEUDO2D_ONE), (fx16)(sy * PSEUDO2D_ONE), 0);
    G3_Vtx((fx16)((sx + width) * PSEUDO2D_ONE), (fx16)((sy + height) * PSEUDO2D_ONE), 0);
    G3_Vtx((fx16)(sx * PSEUDO2D_ONE), (fx16)((sy + height) * PSEUDO2D_ONE), 0);
    G3_End();
}

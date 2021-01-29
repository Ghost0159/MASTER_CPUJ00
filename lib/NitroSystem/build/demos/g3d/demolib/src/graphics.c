/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - demolib
  File:     graphics.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/

//#include "g3d_demolib/system.h"
//#include "g3d_demolib/camera.h"
#include "g3d_demolib/graphics.h"

/*---------------------------------------------------------------------------*
  Name:         InitDisplay

  Description:  ３Ｄ表示します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_InitDisplay(void)
{
//  G3X_Init();                                            // initialize the 3D graphics states
    G3X_InitMtxStack();                                    // initialize the matrix stack
    GX_SetBankForTex(GX_VRAM_TEX_0123_ABCD);               // VRAM-ABCD for texture images
    GX_SetBankForTexPltt(GX_VRAM_TEXPLTT_0123_E);          // VRAM-E for texture palettes
    GX_SetBankForBG(GX_VRAM_BG_16_G);                      // VRAM-C for BGs

	GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,               // graphics mode
	                   GX_BGMODE_0,                        // BGMODE is 4
	                   GX_BG0_AS_3D);                      // BG #0 is for 3D

    GX_SetVisiblePlane( GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 );

	GX_SetBGCharOffset(GX_BGCHAROFFSET_0x00000);
	GX_SetBGScrOffset ( GX_BGSCROFFSET_0x00000);

    G2_SetBG1Control(
        GX_BG_SCRSIZE_TEXT_256x256,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0x3800,
        GX_BG_CHARBASE_0x00000,
        GX_BG_EXTPLTT_01);

	G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);

	G3X_SetShading(GX_SHADING_TOON);                       // shading mode is toon
    G3X_AntiAlias(TRUE);                                   // enable antialias(without additional computing costs)
    G2_BlendNone();                                        // no 2D alpha blending or brightness change

    //---------------------------------------------------------------------------
    // After you start the geometry engine and the rendering one
    // (by GX_SetPower etc.), G3_SwapBuffers must be called once before use
    //---------------------------------------------------------------------------
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);


	// ライト０の設定
//	G3_LightVector(GX_LIGHTID_0, 0, 0, -FX32_ONE + 1);
//	G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));

	G3X_AlphaTest(FALSE, 0);                   // アルファテスト　　オフ
	G3X_AlphaBlend(TRUE);                      // アルファブレンド　オン

    G3_ViewPort(0, 0, 255, 191);

	GX_DispOn();
	GXS_DispOn();
}

/*---------------------------------------------------------------------------*
  Name:         DrawGroundMesh

  Description:  地面のメッシュの描画を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
DrawGroundMesh(void)
{
	const int offset = 2 * FX16_ONE -1;
	s16		  posx1  = FX16_MAX;
	s16		  posx2  = FX16_MAX - offset;
	int		  cnty;

	for (cnty=0; cnty<8; cnty++)
	{
		int cntx;
		s16 posz1 = FX16_MAX;
		s16 posz2 = FX16_MAX - offset;

		G3_Begin(GX_BEGIN_QUAD_STRIP);
			G3_Vtx(posx1, 0, posz1);
			G3_Vtx(posx2, 0, posz1);

			for (cntx=0; cntx<8; cntx++)
			{
				posz1  = posz2;
				posz2 -= offset;

				G3_Vtx(posx1, 0, posz2);
				G3_Vtx(posx2, 0, posz2);
			}
		G3_End();

		posx1  = posx2;
		posx2 -= offset;
	}
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitGround

  Description:  G3DDemoGround構造体を初期化します。

  Arguments:    ground: G3DDemoGround構造体へのポインタ。
				scale : メッシュ地面のスケール。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_InitGround(G3DDemoGround* ground, fx32 scale)
{
	ground->groundEnable = TRUE;
	ground->wireColor	= GX_RGB(31,31,31);
	ground->backColor	= GX_RGB( 0,10,31);
	ground->backAlpha	= 31;
	ground->scale		= scale;
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_DrawGround

  Description:  セレクトボタンが押された時に、地面のメッシュの表示を切り替
  				えます。

  Arguments:    ground: G3DDemoGround構造体へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_DrawGround(G3DDemoGround* ground)
{
	if (ground->groundEnable)
	{
		G3X_SetClearColor(ground->backColor, ground->backAlpha, 0x7fff, 63, FALSE);

		G3_MaterialColorDiffAmb(ground->wireColor, GX_RGB(0,0,0), TRUE);
		G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0,0,0);

		G3_PushMtx();
		G3_Scale(ground->scale, ground->scale, ground->scale);
		DrawGroundMesh();
		G3_PopMtx(1);
	}
}

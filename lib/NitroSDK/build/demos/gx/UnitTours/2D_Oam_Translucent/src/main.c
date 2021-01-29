/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_Translucent
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.8  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.7  2005/11/21 10:32:19  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.6  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.5  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.4  2004/05/27 09:22:22  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 09:04:12  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use OBJ with alpha blending effect.
// 
// This sample displays two spheres on the display.
// One of the sphere is transparent.
// 
// USAGE:
//   UP, DOWN   : Change rate of transparent.
//   
// HOWTO:
// 1. Set GX_OAM_MODE_XLU to OAM attribute to use transparent OBJ.
// 2. Set transparent object by G2_SetBlendAlpha().
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"
//---------------------------------------------------------------------------
//  概説：
//        ＯＡＭバッファ領域
//---------------------------------------------------------------------------
static GXOamAttr s_OAMBuffer[128];

//---------------------------------------------------------------------------
//  概説：
//  　　　Ｖブランク割り込み処理
//  説明：
//  　　　Ｖブランク割り込みが行われたことを確認するためのフラグを立てます。
//
//        共通初期化（DEMOInitCommon()）で、
//        ・ＩＲＱ割り込みの選択(OS_SetIrqMask())。
//        ・ＩＲＱ割り込みを行うこの関数の登録(OS_SetIRQFunction())。
//        ・ＩＲＱ割り込みの許可の設定(OS_EnableIrq())。
//        が行われることで、この関数がＶブランクのときに呼ばれます。
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // VBlank 割り込みの確認フラグを設定
}

//---------------------------------------------------------------------------
//  概説：
//        半透明ＯＢＪの表示
//  説明：
//        半透明のＯＢＪキャラクターを表示する。
//        左側に表示されているOBJは通常表示で、右側に表示されているOBJは半透明
//      OBJ。
//  操作：
//        UP,DOWN: 半透明OBJのブレンディング係数を変更できる。
//---------------------------------------------------------------------------
void NitroMain()
{
    int     eva = 12;

    //---------------------
    // 初期化
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplayOBJOnly();

    /* キャラクターデーターとパレットデータ読み込み */
    GX_LoadOBJ(d_64_256_obj_schDT, 0, sizeof(d_64_256_obj_schDT));
    GX_LoadOBJPltt(d_64_256_obj_sclDT, 0, sizeof(d_64_256_obj_sclDT));

    DEMOStartDisplay();

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        /* パッド情報の読み込み */
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] = { {PAD_KEY_DOWN, 5}, {0, 0} };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            if (++eva > 0x1f)
            {
                eva = 0x1f;
            }
            OS_Printf("eva=%d\n", eva);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            if (--eva < 0x00)
            {
                eva = 0x00;
            }
            OS_Printf("eva=%d\n", eva);
        }

        /* 通常ＯＢＪのＯＡＭアトリビュートを設定 */
        G2_SetOBJAttr(&s_OAMBuffer[0], // １番目のＯＡＭを使用
                      0,               // Ｘ位置
                      0,               // Ｙ位置
                      0,               // 優先順位
                      GX_OAM_MODE_NORMAL,       // 通常OBJ
                      FALSE,           // モザイク無効
                      GX_OAM_EFFECT_NONE,       // 特殊効果無し
                      GX_OAM_SHAPE_64x64,       // ＯＢＪの大きさ
                      GX_OAM_COLOR_256, // ２５６色
                      0,               // キャラ名
                      0, 0);

        /* 半透明ＯＢＪのＯＡＭアトリビュートを設定 */
        G2_SetOBJAttr(&s_OAMBuffer[1], // ２番目のＯＡＭを使用
                      64,              // Ｘ位置
                      0,               // Ｙ位置
                      0,               // 優先順位
                      GX_OAM_MODE_XLU, // 半透明OBJ
                      FALSE,           // モザイク無効
                      GX_OAM_EFFECT_NONE,       // 特殊効果無し
                      GX_OAM_SHAPE_64x64,       // ＯＢＪの大きさ
                      GX_OAM_COLOR_256, // ２５６色
                      0,               // キャラ名
                      0, 0);
        /* αブレンディング設定 */
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE,       // 第一対象面は指定なし
                         GX_BLEND_PLANEMASK_BD, // 第二対象面はバックドロップ
                         eva,          // 第一対象面へのブレンド係数の設定
                         0);           // 

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x80C7021F);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        /* キャッシュをメインメモリーに吐き出す */
        DC_FlushRange(s_OAMBuffer, sizeof(s_OAMBuffer));
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();

        /* ＶＢｌａｎｋ待ち */
        OS_WaitVBlankIntr();

        /* ＯＡＭへ転送 */
        GX_LoadOAM(s_OAMBuffer,        // OAMバッファをOAMへ転送
                   0, sizeof(s_OAMBuffer));
        MI_DmaFill32(3,                // OAMバッファをクリアー
                     s_OAMBuffer, 192, sizeof(s_OAMBuffer));
    }
}

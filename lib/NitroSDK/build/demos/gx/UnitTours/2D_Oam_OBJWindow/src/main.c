/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_OBJWindow
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

  Revision 1.7  2005/11/21 10:32:04  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.6  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.5  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.4  2004/05/27 09:22:11  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 09:03:11  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use OBJ window.
// 
// This sample displays a sea view through a OBJ window.
// 
// USAGE:
//   UP, DOWN, LEFT, RIGHT      : Move the OBJ window
//   
// HOWTO:
// 1. Set up OBJ window by GX_SetVisibleWnd(GX_WNDMASK_OW)
// 2. Set inside and outside of window by G2_SetWndOutsidePlane() and 
//    G2_SetWndOBJInsidePlane.
// 3. Transfer the OBJ data by GX_LoadOBJ().
// 4. Set GX_OAM_MODE_OBJWND to OAM attribute to use the OBJ window
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
//        ＯＢＪウィンドウの表示
//  説明：
//        背景のＢＧを、円形のＯＢＪウィンドウを通して表示する。ＯＢＪウィン
//      ドウの外側は黒で表示される。  
//        パッド操作によって、ＯＢＪウィンドウを移動することが可能。
//  操作：
//        UP, DOWN: ＯＢＪウィンドウの移動。
//---------------------------------------------------------------------------
void NitroMain()
{
    int     pos_x = 0, pos_y = 0;

    //---------------------
    // 初期化
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();

    /* 表示させるＯＡＭとＢＧの選択 */
    GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);

    /* ＯＡＭの設定 */
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);        // VRAM-Aを使用
    // キャラクタOBJのマッピングモードを設定
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);      // ２次元マッピング

    /* ＢＧの設定 */
    GX_SetBankForBG(GX_VRAM_BG_128_B); // VRAM-Bを使用
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // ２Ｄ／３Ｄモード
                       GX_BGMODE_0,    // 
                       GX_BG0_AS_2D);  // ２Ｄ表示
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,        // スクリーンサイズ256×256
                     GX_BG_COLORMODE_256,       // ２５６色
                     GX_BG_SCRBASE_0x0000,      // スクリーンオフセット 0x0000
                     GX_BG_CHARBASE_0x04000,    // キャラクターオフセット0x04000
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);              // ＢＧ０番の表示優先度をトップに
    G2_BG0Mosaic(FALSE);               // モザイク効果を行わない

    /* ウィンドウ設定 */
    GX_SetVisibleWnd(GX_WNDMASK_OW);   // ＯＢＪウィンドウを有効
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE,        // ウィンドウの外側を無効
                          FALSE);
    G2_SetWndOBJInsidePlane(GX_WND_PLANEMASK_BG0,       // ウィンドウの内側をBG0に設定
                            FALSE);

    /* ＯＢＪキャラクターデーターの読み込み */
    GX_LoadOBJ(d_64_256_obj_schDT,     // OBJキャラデータ(窓の形)読み込み
               0, sizeof(d_64_256_obj_schDT));
    /* ＢＧキャラクターデーターとパレットデータ読み込み */
    GX_LoadBG0Char(d_natsunoumi_schDT, // BG0のキャラデータの読み込み
                   0, sizeof(d_natsunoumi_schDT));
    GX_LoadBGPltt(d_natsunoumi_sclDT,  // BGパレットデータの読み込み
                  0, sizeof(d_natsunoumi_sclDT));
    GX_LoadBG0Scr(d_natsunoumi_sscDT,  // BG0のスクリーンデータの読み込み
                  0, sizeof(d_natsunoumi_sscDT));

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
            const EXTKeys keys[8] =
                { {PAD_BUTTON_A, 10}, {PAD_BUTTON_L, 10}, {PAD_KEY_DOWN | PAD_KEY_RIGHT, 20}, {0,
                                                                                               0}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            if (++pos_y > 191)
            {
                pos_y = 191;
            }
        }
        else if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            if (--pos_y < 0)
            {
                pos_y = 0;
            }
        }
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
        {
            if (++pos_x > 255)
            {
                pos_x = 255;
            }
        }
        else if (DEMO_IS_PRESS(PAD_KEY_LEFT))
        {
            if (--pos_x < 0)
            {
                pos_x = 0;
            }
        }

        /* ＯＢＪウィンドウのＯＡＭアトリビュートを設定 */
        G2_SetOBJAttr(&s_OAMBuffer[0], // 使用するＯＡＭのポインター
                      pos_x,           // Ｘ位置
                      pos_y,           // Ｙ位置
                      0,               // 優先順位
                      GX_OAM_MODE_OBJWND,       // ＯＢＪウィンドウモードに設定
                      FALSE,           // モザイク無効
                      GX_OAM_EFFECT_NONE,       // 特殊効果無し
                      GX_OAM_SHAPE_64x64,       // ＯＢＪの大きさ
                      GX_OAM_COLOR_256, // ２５６色
                      0, 0, 0);

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xAE1F20B9);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        /* キャッシュをメインメモリーに吐き出し、無効化する */
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

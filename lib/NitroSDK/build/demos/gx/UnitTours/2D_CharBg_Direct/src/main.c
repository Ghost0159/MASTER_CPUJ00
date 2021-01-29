/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_CharBg_Direct
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.11  2006/03/01 01:56:46  okubata_ryoma
  BG3をload前に明示的にSetするように修正

  Revision 1.10  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 10:29:32  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.8  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.7  2004/11/02 09:59:24  takano_makoto
  fix typo.

  Revision 1.6  2004/07/29 02:18:23  yasu
  Let a picture exchangeable

  Revision 1.5  2004/05/27 09:20:44  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/23 07:08:00  takano_makoto
  Change GX_LoadBGXScr to GX_LoadBGXBmp.

  Revision 1.1  2004/03/01 08:59:52  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use direct color BITMAP BG:
// 
// This sample display picture on the display by direct color BITMAP BG.
// 
// USAGE:
//   UP, DOWN, LEFT, RIGHT      : Slide the BG
//   A, B                       : Scaling the BG
//   L, R                       : Rotate the BG
//   SELECT + [UP, DOWN, LEFT, RIGHT] : Change the center position of rotation
//   SELECT                     : Switch ON/OFF that set BG area over mode, 
//                                loop or transparent.
//   
// HOWTO:
// 1. Transfer the bitmap data by GX_LoadBGxScr().
// 2. Set the direct color BG mode by G2_SetBGxControlDCBmp().
//---------------------------------------------------------------------------


#include <nitro.h>
#include "DEMO.h"
#include "data.h"
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
//        アフィン拡張・ダイレクトカラービットマップＢＧを表示
//  説明：
//        256色ダイレクトカラービットマップＢＧを、アフィン変換（回転拡大縮小）
//      を行って表示する。
//
//        1. BGMODEを3番でBG面3を使用する。
//        2. G2_SetBG3ControlDCBmp()で設定を行い、指定の場所へキャラクタデータ
//         を転送する。
//        3. スクリーンデータを指定の場所へ転送する。
//  操作：
//        PLUS_KEY          : BG面のオフセットを操作する。
//        SELECT +  PLUS_KEY: 回転・拡縮の中心位置を操作する。
//        BUTTON_A,B        : 拡縮する。
//        BUTTON_L,R        : 回転する。
//        SELECT            : エリアオーバー処理のスイッチ
//        START             : 設定値を初期化する。
//---------------------------------------------------------------------------
void NitroMain(void)
{
    int     trg = 0;
    int     offset_H = 0;
    int     offset_V = 30;
    fx32    scale = FX32_ONE;
    s16     x_0 = 0, y_0 = 0;
    s16     x_1 = 0, y_1 = 0;
    u16     rotate = 0;
    
    GXBGAreaOver area_over = GX_BG_AREAOVER_XLU;
    
    //---------------------
    // 初期化
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();

    /* ＢＧの設定 */
    GX_SetBankForBG(GX_VRAM_BG_256_AB); // VRAM-A,BバンクをＢＧに割り当る

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // ２Ｄ／３Ｄモード
                       GX_BGMODE_3,    // BGMODE 3
                       GX_BG0_AS_2D);  // ＢＧ０に２Ｄ表示

    GX_SetBGScrOffset(GX_BGSCROFFSET_0x00000);  // スクリーンオフセット値を設定
    GX_SetBGCharOffset(GX_BGCHAROFFSET_0x10000);        // キャラクタオフセットを設定

    GX_SetVisiblePlane(GX_PLANEMASK_BG3);       // BG3を表示選択
    G2_SetBG3Priority(0);              // BG3の優先順位をトップに設定
    G2_BG3Mosaic(FALSE);               // BG3にモザイク効果を行わない
    
    /* 画面サイズ256x256ドット、ダイレクトカラービットマップにＢＧ３を設定 */
    {
        // ダイレクトカラービットマップのBGにBG3を設定
        // 　画面サイズ              ：画面サイズを256x256ドット
        //   エリアオーバー処理      ：area_overによって決定
        //　 スクリーンベースブロック：0x00000
        G2_SetBG3ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256, area_over, GX_BG_BMPSCRBASE_0x00000);
    }

    /* ＢＧ３にビットマップデータを転送 */
    GX_LoadBG3Bmp(bitmapBG_directcolor_Texel, 0, SCREEN_SIZE);

    DEMOStartDisplay();

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        MtxFx22 mtx;

        /* パッド情報の読み込みと操作 */
        DEMOReadKey();

#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] =
                { {PAD_BUTTON_A, 10}, {PAD_BUTTON_L, 10}, {PAD_KEY_UP | PAD_KEY_RIGHT, 20} };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_BUTTON_SELECT))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP))
                y_0 -= 2;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN))
                y_0 += 2;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                x_0 += 2;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                x_0 -= 2;
        }
        else
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP))
                offset_V += 2;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN))
                offset_V -= 2;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                offset_H -= 2;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                offset_H += 2;
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_A))
            scale += 2 << (FX32_SHIFT - 8);
        if (DEMO_IS_PRESS(PAD_BUTTON_B))
            scale -= 2 << (FX32_SHIFT - 8);
        if (DEMO_IS_PRESS(PAD_BUTTON_L))
            rotate -= 256;
        if (DEMO_IS_PRESS(PAD_BUTTON_R))
            rotate += 256;
        if (DEMO_IS_TRIG(PAD_BUTTON_SELECT))
        {
            trg = (trg + 1) & 0x01;
            OS_Printf("area_over=%d\n", trg);
        }

        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            offset_H = 0;
            offset_V = 0;
            x_0 = 32;
            y_0 = 32;
            scale = 1 << FX32_SHIFT;
            rotate = 0;
        }

        /* 画面サイズ256x256ドット、ダイレクトカラービットマップにＢＧ３を設定 */
        {
            GXBGAreaOver area_over = (trg ? GX_BG_AREAOVER_REPEAT : GX_BG_AREAOVER_XLU);

            // ダイレクトカラービットマップのBGにBG3を設定
            // 　画面サイズ              ：画面サイズを256x256ドット
            //   エリアオーバー処理      ：area_overによって決定
            //　 スクリーンベースブロック：0x00000
            G2_SetBG3ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256, area_over, GX_BG_BMPSCRBASE_0x00000);
        }

        /* アフィン変換行列の生成 */
        {
            fx16    sinVal = FX_SinIdx(rotate);
            fx16    cosVal = FX_CosIdx(rotate);
            fx32    rScale = FX_Inv(scale);

            mtx._00 = (fx32)((cosVal * rScale) >> FX32_SHIFT);
            mtx._01 = (fx32)((sinVal * rScale) >> FX32_SHIFT);
            mtx._10 = -(fx32)((sinVal * rScale) >> FX32_SHIFT);
            mtx._11 = (fx32)((cosVal * rScale) >> FX32_SHIFT);
        }

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x54831E5C);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        /* VBlank待ち */
        OS_WaitVBlankIntr();

        /* BG3平面に適用されるアフィン変換を設定 */
        G2_SetBG3Affine(&mtx,          // 変換行列
                        x_0,           // 回転中心(X)座標
                        y_0,           // 回転中心(Y)座標
                        offset_H,      // 回転前座標(X)
                        offset_V);     // 回転前座標(Y)
    }
}

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_CharBg_256_16
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.6  2005/11/21 10:27:17  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.5  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.4  2004/05/27 09:19:00  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 08:57:46  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use extended affine BG with extended 256x16 color palette:
// 
// This sample draw spheres which stretch and rotate on the display 
// with extended 256x16 color palette.
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
// 1. Allocate VRAM to extended palette by GX_SetBankForBGExtPltt().
// 2. Transfer the palette data by GX_BeginLoadBGExtPltt(), GX_LoadBGExtPltt()
//    and GX_EndLoadBGExtPltt().
// 3. Set the extended palette to BG by G2_SetBGxControl256x16Pltt().
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
//        アフィン拡張・256色×16拡張パレットキャラクタＢＧを表示
//  説明：
//         256色×16の拡張パレットのＢＧを、アフィン変換（回転拡大縮小）を行っ
//       て表示する。
//
//       1. BGMODEを3番でBG面3を使用する。
//       2. G2_SetBG3Control256x16Pltt()にて設定を行い、指定の場所へキャラク
//        タデータを転送する。
//       3. パレットデータの転送にはGX_BeginLoadBGExtPltt(),
//        GX_LoadBGExtPltt(), GX_EndLoadBGExtPltt()を使用して拡張エリアへと転
//        送する。
//       4. スクリーンデータを指定の場所へ転送する。
//  操作：
//       PLUS_KEY         : BG面のオフセットを操作する。
//       SELECT + PLUS_KEY: 回転・拡縮の中心位置を操作する。
//       BUTTON_A,B       : 拡縮する。
//       BUTTON_L,R       : 回転する。
//       SELECT           : エリアオーバー処理のスイッチ
//       START            : 設定値を初期化する。
//---------------------------------------------------------------------------
void NitroMain(void)
{
    int     trg = 0;
    int     offset_H = 0;
    int     offset_V = 0;
    fx32    scale = FX32_ONE;
    s16     x_0 = 0, y_0 = 0;
    s16     x_1 = 0, y_1 = 0;
    u16     rotate = 0;

    //---------------------
    // 初期化
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();

    /* ＢＧの設定 */
    GX_SetBankForBG(GX_VRAM_BG_256_AB); // VRAM-A,BバンクをＢＧに割り当る
    // VRAMバンクをＢＧ拡張パレットに割り当る
    GX_SetBankForBGExtPltt(GX_VRAM_BGEXTPLTT_0123_E);   /* BG拡張パレットの
                                                           スロット0,1,2,3を確保
                                                           VRAM-Eを割り当る */
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // ２Ｄ／３Ｄモード
                       GX_BGMODE_3,    // BGMODE 3
                       GX_BG0_AS_2D);  // ＢＧ０に２Ｄ表示

    GX_SetBGScrOffset(GX_BGSCROFFSET_0x00000);  // スクリーンオフセット値を設定
    GX_SetBGCharOffset(GX_BGCHAROFFSET_0x10000);        // キャラクタオフセットを設定
    GX_SetVisiblePlane(GX_PLANEMASK_BG3);       // BG3を表示選択
    G2_SetBG3Priority(0);              // BG3の優先順位をトップに設定
    G2_BG3Mosaic(FALSE);               // BG3にモザイク効果を行わない

    /* キャラクターデーターとパレットデーターの読み込み
       （BG3のキャラデータとパレットデータをVRAMに転送） */
    GX_LoadBG3Char((const void *)kakucho_8bit_icg,      // キャラデータをVRAMに転送
                   0, CHAR_SIZE);

    {
        GX_BeginLoadBGExtPltt();       // パレットデータの転送準備
        // パレットデータをVRAMに転送
        GX_LoadBGExtPltt((const void *)kakucho_8bit_icl, 0x6000, PALETTE_SIZE);
        GX_EndLoadBGExtPltt();         // パレットデータの転送完了
    }

    /* BG3にスクリーンデータを転送 */
    GX_LoadBG3Scr(kakucho_8bit_isc, 0, SCREEN_SIZE);

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
                { {PAD_BUTTON_A, 10}, {PAD_BUTTON_L, 10}, {PAD_KEY_UP | PAD_KEY_RIGHT, 20}, {0,
                                                                                             0}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_BUTTON_SELECT))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP))
            {
                y_0 -= 2;
            }
            if (DEMO_IS_PRESS(PAD_KEY_DOWN))
            {
                y_0 += 2;
            }
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
            {
                x_0 += 2;
            }
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
            {
                x_0 -= 2;
            }
        }
        else
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP))
            {
                offset_V += 2;
            }
            if (DEMO_IS_PRESS(PAD_KEY_DOWN))
            {
                offset_V -= 2;
            }
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
            {
                offset_H -= 2;
            }
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
            {
                offset_H += 2;
            }
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_A))
        {
            scale += (2 << (FX32_SHIFT - 8));
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_B))
        {
            scale -= (2 << (FX32_SHIFT - 8));
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_L))
        {
            rotate -= 256;
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            rotate += 256;
        }
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

        /* 画面サイズ256x256ドット、256色×16拡張パレットにＢＧ３を設定 */
        {
            GXBGAreaOver area_over = (trg ? GX_BG_AREAOVER_REPEAT : GX_BG_AREAOVER_XLU);

            // 256色×16拡張パレットのBGにBG3を設定
            // 　画面サイズ              ：画面サイズ256x256ドット
            //   エリアオーバー処理      ：area_overによって決定
            //　 スクリーンベースブロック：0x00000
            //   キャラクタベースブロック
            G2_SetBG3Control256x16Pltt(GX_BG_SCRSIZE_256x16PLTT_256x256,
                                       area_over, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x00000);
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
        EXT_TestScreenShot(100, 0x7A94EA54);
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

/*====== End of main.c ======*/

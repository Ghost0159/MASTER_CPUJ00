/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_Char1D
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.9  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 10:31:32  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.7  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.6  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.5  2004/06/02 13:08:38  takano_makoto
  Change EXTPLTT enum name.

  Revision 1.4  2004/05/27 09:23:10  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 09:01:42  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use OBJ in 1D mapped charactor mode.
// 
// This sample displays objects on the display in 1D mapped charactor format.
// 
// USAGE:
//   UP, DOWN, LEFT, RIGHT      : Slide the OBJ
//   A, B                       : Scaling the OBJ
//   L, R                       : Rotate the OBJ
//   SELECT                     : Switch palette Number of the OBJs
//   START                      : Initialize status of the OBJ
//   
// HOWTO:
// 1. Set VRAM size and mapping mode of OBJ by GX_SetOBJVRamModeChar()
// 2. Transfer the bitmap data by GX_LoadOBJ().
// 3. Set GX_OAM_MODE_NORMAL to OAM attribute to use the charactor OBJ.
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
//        １次元マッピング・キャラクタＯＢＪの表示
//  説明：
//        １次元マッピングのキャラクタデータを持つＯＢＪを、アフィン変換（回転
//      拡大縮小）を伴って表示する。
//  操作：
//        UP,DOWN    : 表示位置を操作
//        BUTTON_A,B : 拡縮
//        BUTTON_L,R : 回転
//        SELECT     : パレットの切り替え
//        START      : 設定値を初期化
//---------------------------------------------------------------------------
void NitroMain()
{
    int     base_pos_x = 0, base_pos_y = 0;
    int     base_pal = 0;
    int     base_pat = 0;
    fx32    scale = FX32_ONE;
    u16     rotate = 0;

    //---------------------
    // 初期化
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();

    /* ２Ｄ設定 */
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);        // VRAM-AをOBJに割り当てる

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // ２Ｄ／３Ｄモード
                       GX_BGMODE_0,    // BGMODEを０に設定
                       GX_BG0_AS_2D);  // ＢＧの０を２Ｄとして使用

    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);       // OBJを表示可に設定
    // キャラクターOBJのOBJ-VRAM容量とマッピングモードを設定
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);  // 1次元マッピングモード
    // VRAMバンクをOBJ拡張パレットに割り当てる
    GX_SetBankForOBJExtPltt(GX_VRAM_OBJEXTPLTT_0_F);

    /* キャラクターデーターとパレットデーターの読み込み */
    GX_LoadOBJ(bitmapOBJ_256color_icg, // OBJデータをVRAM-Aに転送
               0, sizeof(bitmapOBJ_256color_icg));
    {
        GX_BeginLoadOBJExtPltt();      // パレットデータの転送準備
        GX_LoadOBJExtPltt(bitmapOBJ_256color_icl,       // パレットデータをVRAMに転送
                          0, sizeof(bitmapOBJ_256color_icl));
        GX_EndLoadOBJExtPltt();        // パレットデータの転送完了
    }

    DEMOStartDisplay();

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        int     i, j;
        int     pos_x = base_pos_x;
        int     pos_y = base_pos_y;
        int     palette = base_pal;

        /* パッド情報の取得と操作設定 */
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

        if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            if (--base_pos_y < 0)
            {
                base_pos_y = 255;
            }
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            if (++base_pos_y > 255)
            {
                base_pos_y = 0;
            }
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
        {
            if (++base_pos_x > 511)
            {
                base_pos_x = 0;
            }
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_LEFT))
        {
            if (--base_pos_x < 0)
            {
                base_pos_x = 511;
            }
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_L))
        {
            rotate -= 512;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            rotate += 512;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_A))
        {
            scale += 128;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_B))
        {
            scale -= 128;
            if (scale < 0)
                scale = 0;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_SELECT))
        {
            if (++base_pal > 15)
                base_pal = 0;
            OS_Printf("base_pal=%d\n", base_pal);
        }

        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            base_pal = 0;
            base_pos_x = 0;
            base_pos_y = 0;
            rotate = 0;
            scale = FX32_ONE;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }

        /* アフィン変換マトリックスの生成と設定 */
        {
            MtxFx22 mtx;
            fx16    sinVal = FX_SinIdx(rotate);
            fx16    cosVal = FX_CosIdx(rotate);
            fx32    rScale = FX_Inv(scale);
            GXOamAffine *oamBuffp = (GXOamAffine *)&s_OAMBuffer[0];

            mtx._00 = (fx32)((cosVal * rScale) >> FX32_SHIFT);
            mtx._01 = (fx32)((sinVal * rScale) >> FX32_SHIFT);
            mtx._10 = -(fx32)((sinVal * rScale) >> FX32_SHIFT);
            mtx._11 = (fx32)((cosVal * rScale) >> FX32_SHIFT);
            // ＯＢＪのアフィン変換を設定
            G2_SetOBJAffine(oamBuffp,  // OAMバッファのポインタ
                            &mtx);     // アフィン変換用2x2行列
        }

        /* ＯＡＭの設定(32個のＯＢＪを設定) */
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 4; j++)
            {
                int     count = (i * 4) + j;
                int     pattern = count * 0x20;

                /* ＯＢＪのＯＡＭアトリビュートを設定 */
                G2_SetOBJAttr(&s_OAMBuffer[count],      // ＯＡＭの場所を指定
                              pos_x,   // Ｘ位置
                              pos_y,   // Ｙ位置
                              0,       // 優先順位
                              GX_OAM_MODE_NORMAL,       // 通常OBJ
                              FALSE,   // モザイク無効
                              GX_OAM_EFFECT_AFFINE,     // アフィン効果
                              GX_OAM_SHAPE_32x32,       // ＯＢＪの大きさ
                              GX_OAM_COLOR_256, // ２５６色
                              pattern, // キャラ名
                              palette, // カラーパレット
                              0);
                pos_x += 32;
                if (++palette > 15)
                {
                    palette = 0;
                }
            }
            pos_x = base_pos_x;
            pos_y += 32;
        }

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xF81231F2);
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

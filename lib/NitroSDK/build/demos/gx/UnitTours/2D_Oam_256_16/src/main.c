/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_256_16
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.11  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.10  2005/11/21 10:30:18  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.9  2005/07/22 12:53:44  kitase_hirotake
  消し忘れていたデクリメント部分を消した

  Revision 1.8  2005/07/22 12:38:33  kitase_hirotake
  FINALROMで十字キーが効かないことの修正

  Revision 1.7  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.6  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.5  2004/06/02 13:08:56  takano_makoto
  Change EXTPLTT enum name.

  Revision 1.4  2004/05/27 09:21:38  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 09:05:36  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use extended 256x16 color palette in OBJ:
// 
// This sample displays some objects on the display with extended 256x16 color 
// palette.
// 
// USAGE:
//   UP, DOWN, LEFT, RIGHT      : Slide the OBJs
//   A, B                       : Scaling the OBJs
//   L, R                       : Rotate the OBJs
//   SELECT                     : Switch palette Number of the OBJs
//   START                      : Initialize status of the OBJs
//   
// HOWTO:
// 1. Allocate VRAM to extended palette by GX_SetBankForOBJExtPltt().
// 2. Transfer the palette data by GX_BeginLoadOBJExtPltt(), GX_LoadOBJExtPltt()
//    and GX_EndLoadOBJExtPltt().
// 3. Set GX_OAM_COLOR_256 to OAM attribute to use the extended palette.
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
//        アフィンＯＢＪの256色×16拡張パレットキャラクタを表示
//  説明：
//　　　　256色×16拡張パレットＯＢＪを、アフィン拡張（回転拡大縮小）を行って
//      表示する。
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
    DEMOInitDisplayOBJOnly();

    /* VRAMバンクをOBJ拡張パレットに割り当てる */
    GX_SetBankForOBJExtPltt(GX_VRAM_OBJEXTPLTT_0_F);

    /* キャラクターデーターとパレットデーターの読み込み */
    GX_LoadOBJ(bitmapOBJ_256color_icg, // OBJデータをVRAMにA転送
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
                { {PAD_BUTTON_A, 10}, {PAD_BUTTON_SELECT, 1}, {PAD_BUTTON_L, 10},
            {PAD_KEY_UP | PAD_KEY_RIGHT, 20}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            --base_pos_y;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            ++base_pos_y;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
        {
            ++base_pos_x;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_LEFT))
        {
            --base_pos_x;
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
            {
                scale = 0;
            }
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_SELECT))
        {
            if (++base_pal > 15)
            {
                base_pal = 0;
            }
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

        /* ＯＡＭの設定(16個の32×32ドットのＯＢＪを設定) */
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                int     count = (i * 4) + j;
                int     pattern = ((count / 4) * 0x80) + ((count % 4) * 0x08);

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

        /* キャッシュをメインメモリーに吐き出す */
        DC_FlushRange(s_OAMBuffer, sizeof(s_OAMBuffer));
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x8B9FC4A0);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        /* ＶＢｌａｎｋ待ち */
        OS_WaitVBlankIntr();

        /* ＯＡＭへ転送 */
        GX_LoadOAM(s_OAMBuffer,        // OAMバッファをOAMへ転送
                   0, sizeof(s_OAMBuffer));
        MI_DmaFill32(3,                // OAMバッファをクリアー
                     s_OAMBuffer, 192, sizeof(s_OAMBuffer));
    }
}

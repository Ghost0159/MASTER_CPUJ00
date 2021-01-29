/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_Direct
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

  Revision 1.7  2005/11/21 10:31:46  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.6  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.5  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.4  2004/05/27 09:21:57  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 09:02:37  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use 2D mapped bitmap OBJ.
// 
// This sample displays objects on the display in 2D mapped bitmap format.
// 
// USAGE:
//   UP, DOWN, LEFT, RIGHT      : Slide the OBJ
//   A, B                       : Scaling the OBJ
//   L, R                       : Rotate the OBJ
//   START                      : Initialize status of the OBJ
//   
// HOWTO:
// 1. Set VRAM size and mapping mode of OBJ by GX_SetOBJVRamModeBmp()
// 2. Transfer the bitmap data by GX_LoadOBJ().
// 3. Set GX_OAM_MODE_BITMAPOBJ to OAM attribute to use the bitmap OBJ.
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
//        ２次元マッピング・ビットマップＯＢＪの表示
//  説明：
//　　　  ２次元マッピングのビットマップデータを持つＯＢＪを、アフィン変換（回
//      転拡大縮小）を伴って表示する。
//  操作：
//        UP,DOWN    : 表示位置を操作
//        BUTTON_A,B : 拡縮
//        BUTTON_L,R : 回転
//        START      : 設定値を初期化
//---------------------------------------------------------------------------
void NitroMain()
{
    int     base_pos_x = 0, base_pos_y = 0;
    int     base_pat = 0;
    fx32    scale = FX32_ONE;
    u16     rotate = 0;

    //---------------------
    // 初期化
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();

    /* ２Ｄ設定 */
    GX_SetBankForOBJ(GX_VRAM_OBJ_256_AB);       // VRAM-A,BをOBJに割り当てる

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // ２Ｄ／３Ｄモード
                       GX_BGMODE_0,    // BGMODEを０に設定
                       GX_BG0_AS_2D);  // ＢＧの０を２Ｄとして使用

    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);       // OBJを表示可に設定
    // ビットマップOBJのOBJ-VRAM容量とマッピングモードを設定
    GX_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_2D_W256_32K);       // ２次元マッピング

    /* キャラクターデーターの読み込み */
    GX_LoadOBJ((const void *)IMAGE_DATA,        // OBJデータをVRAMに転送
               0, IMAGE_DATA_SIZE);

    DEMOStartDisplay();

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        int     i, j;
        int     pos_x = base_pos_x;
        int     pos_y = base_pos_y;

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
            {
                scale = 0;
            }
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }

        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
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

        /* ＯＡＭの設定(16個のＯＢＪを設定) */
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                int     count = (i * 4) + j;
                int     pattern = ((count / 4) * 0x80) + ((count % 4) * 0x08);

                G2_SetOBJAttr(&s_OAMBuffer[count],      // ＯＡＭの場所を指定
                              pos_x,   // Ｘ位置
                              pos_y,   // Ｙ位置
                              0,       // 優先順位
                              GX_OAM_MODE_BITMAPOBJ,    // ビットマップモード
                              FALSE,   // モザイク無効
                              GX_OAM_EFFECT_AFFINE,     // アフィン効果
                              GX_OAM_SHAPE_32x32,       // ＯＢＪの大きさ
                              GX_OAM_COLOR_16,  // １６色
                              pattern, // キャラ名
                              15,      // アルファ値
                              0);
                pos_x += 32;
            }
            pos_x = base_pos_x;
            pos_y += 32;
        }

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xA407A660);
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

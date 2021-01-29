/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_PolAttr_FARClip
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 10:34:33  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.8  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.7  2004/11/02 08:23:24  takano_makoto
  fix comment.

  Revision 1.6  2004/09/14 01:50:40  yasu
  Fix backslashs at end of line.

  Revision 1.5  2004/05/27 09:24:52  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:01:25  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:09:15  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that set GX_POLYGON_ATTR_MISC_FAR_CLIPP in G3_PolygonAttr().
//
// This sample displays two cubes.
// The left one is normal setting and the right one is used far clipping.
// If polygon cross the far face, the right one is clipped.
//
// USAGE:
//   UP, DOWN  : Move z position of the object.
//   START     : Reset position of the object.
//
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"

//---------------------------------------------------------------------------
//  概説：
//  　　　立方体のモデルデータ
//---------------------------------------------------------------------------
/*  */
/* 頂点データ */
static s16 s_Vertex[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};

/* 法線データ */
static VecFx10 s_Normal[6] = {
    GX_VECFX10(0, 0, FX32_ONE - 1),
    GX_VECFX10(0, FX32_ONE - 1, 0),
    GX_VECFX10(FX32_ONE - 1, 0, 0),
    GX_VECFX10(0, 0, -FX32_ONE + 1),
    GX_VECFX10(0, -FX32_ONE + 1, 0),
    GX_VECFX10(-FX32_ONE + 1, 0, 0)
};

/* テクスチャ座標 */
static GXSt s_TextureCoord[] = {
    GX_ST(0, 0),
    GX_ST(0, 64 * FX32_ONE),
    GX_ST(64 * FX32_ONE, 0),
    GX_ST(64 * FX32_ONE, 64 * FX32_ONE)
};

//---------------------------------------------------------------------------
//  概説：
//  　　　頂点座標を設定
//  説明：
//  　　　指定の頂点データで頂点座標を設定する。
//  入力：
//        i_idx : 頂点データのＩＤ
//---------------------------------------------------------------------------
inline void Vertex(int i_idx)
{
    G3_Vtx(s_Vertex[i_idx * 3],        // 頂点座標を設定
           s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　法線の設定
//  説明：
//  　　　
//  入力：
//        i_idx : 法線データのＩＤ
//---------------------------------------------------------------------------
inline void Normal(int i_idx)
{
    G3_Direct1(G3OP_NORMAL, s_Normal[i_idx]);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　テクスチャ座標の設定
//  説明：
//
//  入力：
//        i_idx : テクスチャデータのＩＤ
//---------------------------------------------------------------------------
inline void TextureCoord(int i_idx)
{
    G3_Direct1(G3OP_TEXCOORD, s_TextureCoord[i_idx]);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　立方体の描画
//---------------------------------------------------------------------------
static void DrawCube(void)
{
    G3_Begin(GX_BEGIN_QUADS);          // 頂点リストの開始(四角形ポリゴンで描画)
    {
        TextureCoord(1);
        Normal(0);
        Vertex(2);
        TextureCoord(0);
        Normal(0);
        Vertex(0);
        TextureCoord(2);
        Normal(0);
        Vertex(4);
        TextureCoord(3);
        Normal(0);
        Vertex(6);

        TextureCoord(1);
        Normal(3);
        Vertex(7);
        TextureCoord(0);
        Normal(3);
        Vertex(5);
        TextureCoord(2);
        Normal(3);
        Vertex(1);
        TextureCoord(3);
        Normal(3);
        Vertex(3);

        TextureCoord(1);
        Normal(5);
        Vertex(6);
        TextureCoord(0);
        Normal(5);
        Vertex(4);
        TextureCoord(2);
        Normal(5);
        Vertex(5);
        TextureCoord(3);
        Normal(5);
        Vertex(7);

        TextureCoord(1);
        Normal(2);
        Vertex(3);
        TextureCoord(0);
        Normal(2);
        Vertex(1);
        TextureCoord(2);
        Normal(2);
        Vertex(0);
        TextureCoord(3);
        Normal(2);
        Vertex(2);

        TextureCoord(1);
        Normal(1);
        Vertex(5);
        TextureCoord(0);
        Normal(1);
        Vertex(4);
        TextureCoord(2);
        Normal(1);
        Vertex(0);
        TextureCoord(3);
        Normal(1);
        Vertex(1);

        TextureCoord(1);
        Normal(4);
        Vertex(6);
        TextureCoord(0);
        Normal(4);
        Vertex(7);
        TextureCoord(2);
        Normal(4);
        Vertex(3);
        TextureCoord(3);
        Normal(4);
        Vertex(2);
    }

    G3_End();                          // 頂点リストの終了
}


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
//  　　　G3_PolygonAttr()のFAR面交差ポリゴンのクリッピング
//  説明：
//          左側に表示されている立方体は、FAR面クリッピングが適用されていない
//          表示で、右側に表示されている立方体はFAR面クリッピングが適用される。
//          視体積のfar値が、通常の視体積設定値に比べて非常に小さくなっている。
//          これは、クリッピング処理が行われているのを間近に見るためである。
//  操作：
//        UP,DOWN: オブジェクトのZ座標位置を操作する。
//        START  : 初期位置に戻す。
//---------------------------------------------------------------------------
void NitroMain(void)
{
    int     pos_z = -5;
    u16     rotate = 0;
    u32     texAddr = 0x01000;         // テクスチャイメージ・スロットのアドレス

    //---------------------
    // 初期化(IRQの許可、ＶＲＡＭの初期化、ＢＧ０を３Ｄモードで使用)
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    /* テクスチャイメージをテクスチャイメージ・スロットへの読み込む */
    GX_BeginLoadTex();                 // スロット割り当てのバンクをLCDCメモリにマップ
    {
        GX_LoadTex((void *)&tex_32768_64x64[0], // 読み込み元のポインター
                   texAddr,            // 読み込み先スロットのアドレス
                   8192);              // 読み込みサイズ
    }
    GX_EndLoadTex();                   // LCDCにマップしたスロットを元に戻す

    /* 透視射影の設定 */
    {
        fx32    right = FX32_ONE;
        fx32    top = FX32_ONE * 3 / 4;
        fx32    near = FX32_ONE;
        fx32    far = FX32_ONE * 6;
        // 透視射影の設定
        G3_Perspective(FX32_SIN30,     // Sine   FOVY
                       FX32_COS30,     // Cosine FOVY
                       FX32_ONE * 4 / 3,        // Aspect
                       near,           // Near
                       far,            // Far
                       NULL);

        G3_StoreMtx(0);                // 行列をスタック０番に格納
    }

    DEMOStartDisplay();

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        G3X_Reset();
        rotate += 256;

        /* パッド入力処理 */
        DEMOReadKey();

        if (DEMO_IS_TRIG(PAD_KEY_DOWN))
        {
            if (++pos_z > -3)
            {
                pos_z = -3;
            }
            OS_Printf("Pos_Z=%d\n", pos_z);
        }
        else if (DEMO_IS_TRIG(PAD_KEY_UP))
        {
            if (--pos_z < -7)
            {
                pos_z = -7;
            }
            OS_Printf("Pos_Z=%d\n", pos_z);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            pos_z = -5;
            OS_Printf("Pos_Z=%d\n", pos_z);
        }

        /* カメラの設定 */
        {
            VecFx32 Eye = { 0, 0, FX32_ONE };   // 視点位置
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // 視点設定
        }

        /* ライトの設定(ライトの色と向きを設定) */
        {
            G3_LightVector(GX_LIGHTID_0, 0, 0, -FX32_ONE + 1);
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // 行列をTextureモードに設定
        G3_Identity();                 // 現在の行列を単位行列に初期化
        // 行列をPosition・Vector 同時設定モードに設定
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        G3_PushMtx();                  // 行列をスタックへ入れる

        /* 右表示立方体の回転計算と位置設定 */
        {
            fx16    s = FX_SinIdx(rotate);
            fx16    c = FX_CosIdx(rotate);

            G3_Translate(FX32_ONE * 2, 0, pos_z * FX32_ONE);
            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        /* 右表示立方体の描画設定 */
        // 材質の拡散反射色と環境反射色を設定
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // 拡散反射色
                                GX_RGB(16, 16, 16),     // 環境反射色
                                TRUE); // 頂点カラーを設定
        // 材質の鏡面反射色と放射光色を設定
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // 鏡面反射色
                                GX_RGB(0, 0, 0),        // 放射光色
                                FALSE); // 鏡面反射を未使用

        // テクスチャのパラメータを設定
        G3_TexImageParam(GX_TEXFMT_DIRECT,      // ダイレクトテクスチャを使用
                         GX_TEXGEN_TEXCOORD,    // テクスチャ座標変換モードの設定
                         GX_TEXSIZE_S64,        // 64 テクスチャｓサイズ
                         GX_TEXSIZE_T64,        // 64 テクスチャｔサイズ
                         GX_TEXREPEAT_NONE,     // リピート無し
                         GX_TEXFLIP_NONE,       // フリップ無し
                         GX_TEXPLTTCOLOR0_USE,  // パレットのカラー0設定値が有効
                         texAddr);     // テクスチャのアドレス
        // ポリゴン属性の設定
        G3_PolygonAttr(GX_LIGHTMASK_0, // ライト０をオン
                       GX_POLYGONMODE_MODULATE, // モジュレーションモード
                       GX_CULL_NONE,   // カリングを行わない
                       0,              // ポリゴンＩＤ ０
                       31,             // アルファ値
                       GX_POLYGON_ATTR_MISC_FAR_CLIPPING);

        DrawCube();                    // 立方体の描画

        G3_PopMtx(1);

        G3_PushMtx();
        /* 左表示立方体の回転計算と位置設定 */
        {
            fx16    s = FX_SinIdx(rotate);
            fx16    c = FX_CosIdx(rotate);

            G3_Translate(-FX32_ONE * 2, 0, pos_z * FX32_ONE);

            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // 行列をTextureモードに設定
        G3_Identity();                 // 現在の行列を単位行列に初期化
        // 行列をPosition・Vector 同時設定モードに設定
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        /* 左表示立方体の描画設定 */
        // 材質の拡散反射色と環境反射色を設定
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // 拡散反射色
                                GX_RGB(16, 16, 16),     // 環境反射色
                                TRUE); // 頂点カラーを設定
        // 材質の鏡面反射色と放射光色を設定
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // 鏡面反射色
                                GX_RGB(0, 0, 0),        // 放射光色
                                FALSE); // 鏡面反射を未使用
        // テクスチャのパラメータを設定
        G3_TexImageParam(GX_TEXFMT_DIRECT,      // ダイレクトテクスチャを使用
                         GX_TEXGEN_TEXCOORD,    // テクスチャ座標変換モードの設定
                         GX_TEXSIZE_S64,        // 64 テクスチャｓサイズ
                         GX_TEXSIZE_T64,        // 64 テクスチャｔサイズ
                         GX_TEXREPEAT_NONE,     // リピート無し
                         GX_TEXFLIP_NONE,       // フリップ無し
                         GX_TEXPLTTCOLOR0_USE,  // パレットのカラー0設定値が有効
                         texAddr);     // テクスチャのアドレス
        G3_PolygonAttr(GX_LIGHTMASK_0, // ライト０をオン
                       GX_POLYGONMODE_MODULATE, // モジュレーションモード
                       GX_CULL_NONE,   // カルバックを行わない
                       0,              // ポリゴンＩＤ ０
                       31,             // アルファ値
                       GX_POLYGON_ATTR_MISC_NONE);

        DrawCube();                    // 立方体の描画

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        /* VBlank待ち */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x9F1292DC);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}

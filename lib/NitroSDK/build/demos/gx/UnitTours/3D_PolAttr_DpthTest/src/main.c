/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_PolAttr_DpthTest
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

  Revision 1.7  2005/11/21 10:34:05  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.6  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.5  2004/05/27 09:24:32  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:00:23  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:07:48  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


//---------------------------------------------------------------------------
// A sample that set GX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL in G3_PolygonAttr().
// 
// This sample displays two square.  If switch is ON, the right one is drawn
// only when the polygon's depth value equals the value in depth buffer.
// 
// USAGE:
//   START      : Switch ON/OFF of GX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL
//   
//---------------------------------------------------------------------------


#include <nitro.h>
#include "DEMO.h"
#include "data.h"

//---------------------------------------------------------------------------
//  概説：
//  　　　矩形のモデルデータ
//---------------------------------------------------------------------------
/* 頂点データ */
static s16 s_Vertex[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
};
/* 法線データ */
static VecFx10 s_Normal = GX_VECFX10(0, 0, FX32_ONE - 1);
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
//        i_idx : 頂点データのID
//---------------------------------------------------------------------------
inline void Vertex(int i_idx)
{
    G3_Vtx(s_Vertex[i_idx * 3], s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　法線の設定
//  説明：
//  　　　
//---------------------------------------------------------------------------
inline void Normal(void)
{
    G3_Direct1(G3OP_NORMAL, s_Normal);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　テクスチャ座標の設定
//  説明：
//        
//  入力：
//        i_idx : テクスチャデータのID
//---------------------------------------------------------------------------
inline void TextureCoord(int i_idx)
{
    G3_Direct1(G3OP_TEXCOORD, s_TextureCoord[i_idx]);
}


//---------------------------------------------------------------------------
//  概説：
//  　　　矩形の描画
//  説明：
//        
//---------------------------------------------------------------------------
static void DrawRectangle(void)
{
    G3_Begin(GX_BEGIN_QUADS);          // 頂点リストの開始(四角形ポリゴンで描画)
    {
        TextureCoord(1);
        Normal();
        Vertex(1);
        TextureCoord(3);
        Normal();
        Vertex(3);
        TextureCoord(2);
        Normal();
        Vertex(2);
        TextureCoord(0);
        Normal();
        Vertex(0);
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
//        ・IRQ割り込みの選択(OS_SetIrqMask())。
//        ・IRQ割り込みを行うこの関数の登録(OS_SetIRQFunction())。
//        ・IRQ割り込みの許可の設定(OS_EnableIrq())。
//        が行われることで、この関数がＶブランクのときに呼ばれます。
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // VBlank 割り込みの確認フラグを設定
}

//---------------------------------------------------------------------------
//  概説：
//  　    G3_PolygonAttr()のGX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL設定
//  説明：
//　　　　ポリゴンのデプス値がデプスバッファのデプス値と等しい場合に描画する。
//              
//        左側に表示されている矩形は通常表示で、右側に表示されている矩形がデプ
//      ス対象のポリゴン。効果をＯＮにすると、左矩形とデプス値が同じになる右側
//      矩形の重なった箇所が左矩形の上に表示される。
//  操作：
//        START: 効果のＯＮ/ＯＦＦの切り替えスイッチ
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u32     texAddr = 0x00000;         // テクスチャイメージ・スロットのアドレス
    unsigned int count = 0;
    BOOL    trg = 0;
    u16     rotate = 0;

    //---------------------
    // 初期化(IRQの許可、ＶＲＡＭの初期化、BG0を3Dモードで使用)
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

    DEMOStartDisplay();

    OS_Printf("mssg%d:PolygonAttr_DepthTest=%s\n", count++, (trg ? "ON" : "OFF"));

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        G3X_Reset();
        rotate += 128;

        /* パッド入力 */
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] = { {0, 20}, {PAD_BUTTON_START, 20}, {0, 0} };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            trg = (trg + 1) & 0x01;
            OS_Printf("mssg%d:PolygonAttr_DepthTest=%s\n", count++, (trg ? "ON" : "OFF"));
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
            G3_LightVector(GX_LIGHTID_0, 0, 0, -FX32_ONE + 1);  // ライトの向き
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));    // ライトの色
        }

        /* 左表示矩形の描画設定 */
        G3_MtxMode(GX_MTXMODE_TEXTURE);
        G3_Identity();
        // 行列をPosition・Vector 同時設定モードに設定
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        G3_PushMtx();                  // 行列をスタックへ入れる

        /* 左表示矩形の回転計算と位置設定 */
        {
            fx16    s = FX_SinIdx(rotate);
            fx16    c = FX_CosIdx(rotate);

            G3_Translate(-FX32_ONE / 2, 0, -3 * FX32_ONE);
            G3_RotZ(s, c);
        }

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
        G3_PolygonAttr(GX_LIGHTMASK_0, // ライト0を反映
                       GX_POLYGONMODE_MODULATE, // モジュレーションモード
                       GX_CULL_NONE,   // カルを行わない
                       0,              // ポリゴンID 0
                       31,             // アルファ値
                       GX_POLYGON_ATTR_MISC_NONE);

        DrawRectangle();               // 矩形の描画

        G3_PopMtx(1);

        G3_PushMtx();

        /* 右表示矩形の回転計算と位置設定 */
        {
            fx16    s = FX_SinIdx(rotate);
            fx16    c = FX_CosIdx(rotate);

            G3_Translate(FX32_ONE / 2, 0, -3 * FX32_ONE);
            G3_RotZ(s, c);
        }
        /* 右表示矩形の描画設定 */
        G3_MtxMode(GX_MTXMODE_TEXTURE); // 行列をTextureモードに設定
        G3_Identity();                 // 現在の行列を単位行列に初期化
        // 行列をPosition・Vector 同時設定モードに設定
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
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

        // 右表示矩形ポリゴンのデプス変更設定
        {
            int     attr;

            if (trg)
            {
                attr = GX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL;
            }
            else
            {
                attr = GX_POLYGON_ATTR_MISC_NONE;
            }
            // ポリゴン属性の設定
            G3_PolygonAttr(GX_LIGHTMASK_0,      // ライト0をオン
                           GX_POLYGONMODE_MODULATE,     // モジュレーションモード
                           GX_CULL_NONE,        // カリングを行わない
                           0,          // ポリゴンID 0
                           31,         // アルファ値
                           attr);
        }

        DrawRectangle();               // 矩形の描画

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        OS_WaitVBlankIntr();           // VBlank待ち

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x43CB4676);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

    }
}

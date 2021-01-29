/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Pol_Vertex_Source
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2006/01/18 02:11:26  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 10:39:24  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.8  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.7  2004/11/02 08:23:12  takano_makoto
  fix comment.

  Revision 1.6  2004/05/27 09:24:07  takano_makoto
  Add Autotest code.

  Revision 1.5  2004/04/23 07:23:36  yada
  only fix comment

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:00:09  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:06:20  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that call G3_TexImageParam() with GX_TEXGEN_VERTEX mode.
// 
// This sample displays a cube having a constant viewpoint texture.
//---------------------------------------------------------------------------


#include <nitro.h>
#include "DEMO.h"
#include "data.h"

//---------------------------------------------------------------------------
//  概説：
//  　　　立方体のモデルデータ
//---------------------------------------------------------------------------
/* テクスチャ行列 */
static MtxFx44 s_TextureMtx = {
    32 * FX32_ONE, 0, 0, 0,
    0, -32 * FX32_ONE, 0, 0,
    0, 0, FX32_ONE, 0,
    0, 0, 0, FX32_ONE
};
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
static VecFx10 s_Normal[8] = {
    GX_VECFX10(FX32_SQRT1_3, FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(FX32_SQRT1_3, FX32_SQRT1_3, -FX32_SQRT1_3),
    GX_VECFX10(FX32_SQRT1_3, -FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(FX32_SQRT1_3, -FX32_SQRT1_3, -FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, FX32_SQRT1_3, -FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, -FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, -FX32_SQRT1_3, -FX32_SQRT1_3)
};

//---------------------------------------------------------------------------
//  概説：
//  　　　頂点座標を設定
//  説明：
//  　　　指定の頂点データで頂点座標を設定する。
//  入力：
//        idx : 頂点データのＩＤ
//---------------------------------------------------------------------------
inline void Vertex(int idx)
{
    G3_Vtx(s_Vertex[idx * 3], s_Vertex[idx * 3 + 1], s_Vertex[idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　法線の設定
//  説明：
//  　　　
//  入力：
//        idx : 法線データのＩＤ
//---------------------------------------------------------------------------
inline void Normal(int idx)
{
    G3_Direct1(G3OP_NORMAL, s_Normal[idx]);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　立方体の面生成
//  説明：
//  　　　立方体の１面を生成する。
//  入力：
//        idx0～idx3 : 構成する頂点データのＩＤ
//---------------------------------------------------------------------------
static void Quad(int idx0, int idx1, int idx2, int idx3)
{
    Normal(idx0);
    Vertex(idx0);
    Normal(idx1);
    Vertex(idx1);
    Normal(idx2);
    Vertex(idx2);
    Normal(idx3);
    Vertex(idx3);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　立方体の描画（法線無し）
//  説明：
//  　　　
//---------------------------------------------------------------------------
static void DrawCube(void)
{
    G3_Begin(GX_BEGIN_QUADS);          // 頂点リストの開始(四角形ポリゴンで描画
    {
        Quad(2, 0, 4, 6);
        Quad(7, 5, 1, 3);
        Quad(6, 4, 5, 7);
        Quad(3, 1, 0, 2);
        Quad(5, 4, 0, 1);
        Quad(6, 7, 3, 2);
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
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt
}

//---------------------------------------------------------------------------
//  概説：
//        G3_TexImageParam()のGX_TEXGEN_VERTEX設定（視点固定テクスチャの表現）
//  説明：
//        G3_TexImageParam()のGX_TEXGEN_VERTEX設定を行い、立方体にテクスチャを
//      張る。結果、立方体の回転に関わらず視点に対して固定して見えるテクスチャ
//      が張られる。
//
//        テクスチャ行列に送られた頂点ベクトルが、ローカルにあることに注意。
//        従って、適切な回転行列を前もって増やさなければならない。
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u32     myTexAddr = 0x1000;        // テクスチャイメージ・スロットのアドレス
    u32     myTexPlttAddr = 0x1000;    // テクスチャパレット・スロットのアドレス
    u16     Rotate = 0;

    //---------------------
    // 初期化(IRQの許可、ＶＲＡＭの初期化、ＢＧ０を３Ｄモードで使用)
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    /* テクスチャイメージをテクスチャイメージ・スロットへ読み込む */
    GX_BeginLoadTex();                 // スロット割り当てのバンクをLCDCメモリにマップ
    {
        GX_LoadTex((void *)&tex_4plett64x64[0], // 読み込み元のポインター
                   myTexAddr,          // 読み込み先のスロットのアドレス
                   1024);              // 読み込みサイズ
    }
    GX_EndLoadTex();                   // LCDCにマップしたスロットを元に戻す

    /* テクスチャパレットのテクスチャパレット・スロットへの読み込み */
    GX_BeginLoadTexPltt();             // スロット割り当てのバンクをLCDCメモリにマップ
    {
        GX_LoadTexPltt((void *)&pal_4plett[0],  // 読み込み元のポインター
                       myTexPlttAddr,  // 読み込み先パレット・スロットのアドレス
                       8);             // 読み込みサイズ
    }
    GX_EndLoadTexPltt();               // LCDCにマップしたスロットを元に戻す

    DEMOStartDisplay();

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        MtxFx43 camera;

        G3X_Reset();
        Rotate += 256;

        /* カメラの設定 */
        {
            VecFx32 Eye = { 0, 0, 10 * FX32_ONE };      // Eye position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up
            G3_LookAt(&Eye, &vUp, &at, &camera);
        }

        /* ライトの設定
           (LightVectorコマンドの直後に、ライト行列が現在の行列によって変換さ
           れていることに注意) */
        {
            G3_LightVector(GX_LIGHTID_0, 0, -FX32_ONE + 1, 0);
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
        }

        G3_PushMtx();

        /* 立方体の回転計算と位置設定 */
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_Scale(FX32_ONE * 3, FX32_ONE * 3, FX32_ONE * 3);
            G3_RotZ(s, c);
            G3_RotY(s, c);
            G3_RotX(s, c);
        }

        /* 立方体の描画設定 */
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            /* 立方体への環境マッピングの設定
               現在のテクスチャ行列に設定される前に、引数の行列は(16,16,16)に
               スケールされている。
               G3_MtxMode(GX_MTXMODE_TEXTURE)やG3_LoadMtx44()を使用すれば、行
               列を自分でスケールしなければならない。 */

            // 行列をテクスチャモードにしてテクスチャ行列を設定
            G3_LoadTexMtxEnv(&s_TextureMtx);

            /* ここでカメラ行列を増やせば、同じ方法でテクスチャを見られる。
               通常、ここでの行列は環境光の方向の指定を行う。 */
            // 現在の行列に３×３の行列を乗算する
            G3_MultMtx33((MtxFx33 *)&camera);   // 現行列にカメラ行列を乗算

            G3_RotZ(s, c);
            G3_RotY(s, c);
            G3_RotX(s, c);

            /* 立方体へ張るテクスチャの設定 */
            // 行列をPosition・Vector 同時設定モードに設定
            G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

            // テクスチャ座標の設定
            G3_TexCoord(32 * FX32_ONE, 32 * FX32_ONE);  // 64x64テクスチャの中心を指定
        }

        // テクスチャのパラメータを設定
        G3_TexImageParam(GX_TEXFMT_PLTT4,       // 4色パレットテクスチャ
                         GX_TEXGEN_VERTEX,      // バーテックスソース
                         GX_TEXSIZE_S64,        // 64 テクスチャｓサイズ
                         GX_TEXSIZE_T64,        // 64 テクスチャｔサイズ
                         GX_TEXREPEAT_NONE,     // リピート無し
                         GX_TEXFLIP_NONE,       // フリップ無し
                         GX_TEXPLTTCOLOR0_USE,  // パレットのカラー0設定値が有効
                         myTexAddr);   // テクスチャのアドレス

        // テクスチャパレットのアドレスを設定
        G3_TexPlttBase(myTexPlttAddr,  // テクスチャパレットのアドレス
                       GX_TEXFMT_PLTT4);        // 4色パレットテクスチャに適用

        // 材質の拡散反射色と環境反射色を設定
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // 拡散反射色
                                GX_RGB(16, 16, 16),     // 環境反射色
                                TRUE); // 拡散反射色を頂点カラーとしてセット

        // 材質の鏡面反射色と放射光色を設定
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // 鏡面反射色
                                GX_RGB(0, 0, 0),        // 放射光色
                                FALSE); // 鏡面反射を未使用

        // ポリゴン属性の設定
        G3_PolygonAttr(GX_LIGHTMASK_0, // ライト０を反映
                       GX_POLYGONMODE_MODULATE, // モジュレーションモード
                       GX_CULL_BACK,   // カルバックを行う
                       0,              // ポリゴンＩＤ ０
                       31,             // アルファ値
                       0);

        DrawCube();                    // 立方体の描画

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
        /* Ｖブランク待ち */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xB561C3C8);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}

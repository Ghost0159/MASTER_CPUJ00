/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_PolAttr_1Dot
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

  Revision 1.8  2005/11/21 10:33:52  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.7  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.6  2004/07/27 08:02:40  yasu
  Avoid warning of padding

  Revision 1.5  2004/05/27 09:24:21  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:00:17  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:07:03  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that control the display of 1dot polygon by depth value.
// 
// This sample displays three cubes, and change scale and depth by pad.
// If cubes looks less than 1dot, set visible or unvisible by press A.
// 
// USAGE:
//   UP, DOWN   : Change the limit depth value that 1dot polygon is visible.
//   L, R       : Change the scale of object.
//   A          : Switch ON/OFF that control visible of 1dot polygon.
//   
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"

//---------------------------------------------------------------------------
//  概説：
//  　　　立方体のモデルデータ
//---------------------------------------------------------------------------
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
    G3_Vtx(s_Vertex[i_idx * 3], s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　頂点カラーを設定
//---------------------------------------------------------------------------
inline void Color(void)
{
    G3_Color(GX_RGB(31, 31, 31));
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
//        立方体の描画情報の型
//  説明：
//---------------------------------------------------------------------------
#pragma  warn_padding off
typedef struct
{
    int     trg;
    fx32    scale;
    fx32    depth;
    u32     texAddr;
    u16     rotate;
    // [PADDING 2 BYTE HERE]
}
cubu_c;
#pragma  warn_padding reset


//---------------------------------------------------------------------------
//  概説：
//        立方体の描画
//  説明：
//
//  入力：
//        *p : 立方体情報のポインター
//        x  : Ｘ位置
//        y  : Ｙ位置
//        z  : Ｚ位置
//---------------------------------------------------------------------------
static void Cube(cubu_c * p, fx32 x, fx32 y, fx32 z)
{
    G3_PushMtx();

    /* 立方体の回転と位置設定 */
    {
        fx16    s = FX_SinIdx(p->rotate);
        fx16    c = FX_CosIdx(p->rotate);

        G3_Translate(x, y, z);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);

        G3_Scale(p->scale, p->scale, p->scale);
    }

    /* 立方体の描画設定 */
    // 材質の拡散反射色と環境反射色を設定
    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // 拡散反射色
                            GX_RGB(16, 16, 16), // 環境反射色
                            TRUE);     // 頂点カラーを設定
    // 材質の鏡面反射色と放射光色を設定
    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // 鏡面反射色
                            GX_RGB(0, 0, 0),    // 放射光色
                            FALSE);    // 鏡面反射を未使用
    // ポリゴン属性の設定

    // テクスチャのパラメータを設定
    G3_TexImageParam(GX_TEXFMT_DIRECT, // ダイレクトテクスチャを使用
                     GX_TEXGEN_TEXCOORD,        // テクスチャ座標変換モードの設定
                     GX_TEXSIZE_S64,   // 64 テクスチャｓサイズ
                     GX_TEXSIZE_T64,   // 64 テクスチャｔサイズ
                     GX_TEXREPEAT_NONE, // リピート無し
                     GX_TEXFLIP_NONE,  // フリップ無し
                     GX_TEXPLTTCOLOR0_USE,      // パレットのカラー0設定値が有効
                     p->texAddr);      // テクスチャのアドレス

    {
        int     attr;

        if (p->trg)
        {
            attr = GX_POLYGON_ATTR_MISC_DISP_1DOT;
        }
        else
        {
            attr = GX_POLYGON_ATTR_MISC_NONE;
        }
        // ポリゴン属性の設定
        G3_PolygonAttr(GX_LIGHTMASK_NONE,       // ライトを反映しない
                       GX_POLYGONMODE_MODULATE, // モジュレーションモード
                       GX_CULL_NONE,   // カリング無し
                       0,              // ポリゴンID 0
                       31,             // アルファ値
                       attr);          // 1ドットポリゴンの表示設定
    }
    // 
    G3X_SetDisp1DotDepth(p->depth);    // 1ドットポリゴン表示境界デプス値

    G3_Begin(GX_BEGIN_QUADS);          // 頂点リストの開始(四角形ポリゴンでの描画)
    {
        TextureCoord(1);
        Color();
        Vertex(2);
        TextureCoord(0);
        Color();
        Vertex(0);
        TextureCoord(2);
        Color();
        Vertex(4);
        TextureCoord(3);
        Color();
        Vertex(6);

        TextureCoord(1);
        Color();
        Vertex(7);
        TextureCoord(0);
        Color();
        Vertex(5);
        TextureCoord(2);
        Color();
        Vertex(1);
        TextureCoord(3);
        Color();
        Vertex(3);

        TextureCoord(1);
        Color();
        Vertex(6);
        TextureCoord(0);
        Color();
        Vertex(4);
        TextureCoord(2);
        Color();
        Vertex(5);
        TextureCoord(3);
        Color();
        Vertex(7);

        TextureCoord(1);
        Color();
        Vertex(3);
        TextureCoord(0);
        Color();
        Vertex(1);
        TextureCoord(2);
        Color();
        Vertex(0);
        TextureCoord(3);
        Color();
        Vertex(2);

        TextureCoord(1);
        Color();
        Vertex(5);
        TextureCoord(0);
        Color();
        Vertex(4);
        TextureCoord(2);
        Color();
        Vertex(0);
        TextureCoord(3);
        Color();
        Vertex(1);

        TextureCoord(1);
        Color();
        Vertex(6);
        TextureCoord(0);
        Color();
        Vertex(7);
        TextureCoord(2);
        Color();
        Vertex(3);
        TextureCoord(3);
        Color();
        Vertex(2);
    }
    G3_End();                          // 頂点リストの終了

    G3_PopMtx(1);
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
//        G3_PolygonAttr()のGX_POLYGON_ATTR_MISC_DISP_1DOT設定
//　　　　(デプス値による１ドットポリゴンの表示抑制)
//  説明：
//        デプス値によって１ドットポリゴンの表示抑制を行う。
//
//        1. サンプルが始まったらBUTTON_Lを押してオブジェクトのスケール値を0に
//         する。
//        2. スケール値が0になり、3つのオブジェクトが1ドットで表示されている。
//        3. 十字キーの上下を押してデプス値を増減させることにより奥に表示されて
//         いる左側のオブジェクトから順に点滅表示される。
//  操作：
//        BUTTON_L,R: オブジェクトのスケール値
//        UP,DOWN   : オブジェクトのデプス値の増減
//        START     : 初期値へ戻す
//        SELECT    : ポジションテストで、デプス値を近い値に更新する
//---------------------------------------------------------------------------
void NitroMain(void)
{
    cubu_c  cubu;

    cubu.trg = 1;
    cubu.scale = FX32_ONE;
    cubu.depth = 16384;
    cubu.texAddr = 0x01000;            // テクスチャイメージ・スロットのアドレス
    cubu.rotate = 0;

    /*
     * 初期化(IRQの許可、ＶＲＡＭの初期化、ＢＧ０を３Ｄモードで使用)
     */
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    /* テクスチャイメージをテクスチャイメージ・スロットへの読み込む */
    GX_BeginLoadTex();                 // スロット割り当てのバンクをLCDCメモリにマップ
    {
        GX_LoadTex((void *)&tex_32768_64x64[0], // 読み込み元のポインター
                   cubu.texAddr,       // 読み込み先スロットのアドレス
                   8192);              // 読み込みサイズ
    }
    GX_EndLoadTex();                   // LCDCにマップしたスロットを元に戻す

    DEMOStartDisplay();

    while (1)
    {
        G3X_Reset();
        cubu.rotate += 256;

        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] =
                { {PAD_BUTTON_L, 80}, {PAD_KEY_DOWN, 8}, {PAD_BUTTON_A, 1}, {0, 0} };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            cubu.scale += 64;
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_L))
        {
            cubu.scale -= 64;
            if (cubu.scale < 0)
            {
                cubu.scale = 0;
            }
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            cubu.depth += 256;
            if (cubu.depth > 0xffffff)
            {
                cubu.depth = 0xffffff;
            }
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            cubu.depth -= 256;
            if (cubu.depth < 0)
            {
                cubu.depth = 0;
            }
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_A))
        {
            cubu.trg = (cubu.trg + 1) & 0x01;
            OS_Printf("PolygonAttr_1DotPoly=%s\n", (cubu.trg ? "ON" : "OFF"));
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            cubu.scale = FX32_ONE;
            cubu.depth = 16384;
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_SELECT))
        {
            fx16    s = FX_SinIdx(cubu.rotate);
            fx16    c = FX_CosIdx(cubu.rotate);

            G3_Translate(-FX32_ONE, 0, -4 * FX32_ONE);

            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);

            G3_Scale(cubu.scale, cubu.scale, cubu.scale);

            {
                VecFx32 m;
                fx32    w;

                G3_PositionTest(FX16_ONE, FX16_ONE, FX16_ONE);
                while (G3X_GetPositionTestResult(&m, &w))
                {
                    // ポジションテスト待ち
                }

                OS_Printf("Position Test : Pos(%d, %d, %d) W(%d)\n", m.x, m.y, m.z, w);
                cubu.depth = w;
            }
        }

        /* カメラの設定 */
        {
            VecFx32 Eye = { 0, 0, FX32_ONE };   // Eye position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // 視点設定
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // 行列をTextureモードに設定
        G3_Identity();                 // 現在の行列を単位行列に初期化
        // 行列をPosition・Vector 同時設定モードに設定
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        /* 立方体の描画 */
        Cube(&cubu, FX32_ONE, 0, -2 * FX32_ONE);
        Cube(&cubu, 0, 0, -3 * FX32_ONE);
        Cube(&cubu, -FX32_ONE, 0, -4 * FX32_ONE);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        /* ＶＢｌａｎｋ待ち */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(150, 0xA00DF599);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

    }
}

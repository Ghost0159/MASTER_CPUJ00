/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_PolAttr_DpthUpdate
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

  Revision 1.8  2005/11/21 10:34:19  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.7  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.6  2004/11/02 08:23:17  takano_makoto
  fix comment.

  Revision 1.5  2004/05/27 09:24:41  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.2  2004/03/15 02:00:32  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:08:29  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that set GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE in G3_PolygonAttr().
// The depth buffer is updated when the transparent polygon is drawn.
// 
// This sample displays a transparent cube and pole. If switch is ON, 
// the cube is drawn with GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE parameter.
// 
// USAGE:
//   UP, DOWN   : Change alpha blend parameter.
//   START      : Switch ON/OFF of GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE.
//   
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"

//---------------------------------------------------------------------------
//  概説：
//  　　　立方体のモデルデータ
//---------------------------------------------------------------------------
/* 頂点データ */
static const s16 s_Vertex[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};

//---------------------------------------------------------------------------
//  　　　頂点座標を設定
//  説明：
//  　　　指定の頂点データで頂点座標を設定する。
//  入力：
//        i_idx : 頂点データのＩＤ
//---------------------------------------------------------------------------
static void Vertex(int i_idx)
{
    G3_Vtx(s_Vertex[i_idx * 3], s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  概説：
//  　　　立方体の面生成
//  説明：
//  　　　立方体の１面を生成する。
//  入力：
//        i_idx0～i_id3 : 構成する頂点データのＩＤ
//---------------------------------------------------------------------------
inline void Quad(int i_idx0, int i_idx1, int i_idx2, int i_idx3)
{
    Vertex(i_idx0);
    Vertex(i_idx1);
    Vertex(i_idx2);
    Vertex(i_idx3);
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
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // VBlank 割り込みの確認フラグを設定
}

//---------------------------------------------------------------------------
//  概説：
//        G3_PolygonAttr()のGX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE設定
//       （半透明ポリゴン描画時のデプスバッファの更新）
//  説明：
//        通常表示の矩形の周囲を、デプステスト対象のフォグが掛けられている半透
//      明立方体が周回する。
//        デプス値更新がＯＦＦであれば、半透明立方体にはフォグの影響が反映され
//      ない。デプス値更新をＯＮにすればフォグの影響が反映される。
//  操作：
//  　    START  : デプス値更新のＯＮ/ＯＦＦの切り替え
//   　 　UP,DOWN: オブジェクトのα値の増減
//---------------------------------------------------------------------------
void NitroMain(void)
{
    unsigned int count = 0;
    int     alpha = 0x0a;
    BOOL    trg = 1;
    u16     Rotate = 0;

    //---------------------
    // 初期化(IRQの許可、ＶＲＡＭの初期化、ＢＧ０を３Ｄモードで使用)
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    /* フォグ設定 */
    {
        u32     fog_table[8];
        int     i;

        // クリアカラーの設定
        G3X_SetClearColor(GX_RGB(0, 0, 0),      // クリアカラー
                          0,           // クリアアルファ値
                          0x7fff,      // クリアデプス値
                          63,          // クリアポリゴンID
                          TRUE);       // フォグを行う
        // フォグ属性の設定
        G3X_SetFog(TRUE,               // フォグを有効に
                   GX_FOGBLEND_COLOR_ALPHA,     // カラーとアルファにフォグ
                   GX_FOGSLOPE_0x2000, // フォグ勾配の設定
                   0x5800);            // フォグ計算デプス値
        G3X_SetFogColor(GX_RGB(31, 31, 31), 0); // フォグカラーの設定

        // フォグテーブルの設定(値が大きいほど濃いフォグになる)
        for (i = 0; i < 8; i++)
        {
            fog_table[i] = (u32)(((i * 16) << 0) |
                                 ((i * 16 + 4) << 8) |
                                 ((i * 16 + 8) << 16) | ((i * 16 + 12) << 24));
        }
        G3X_SetFogTable(&fog_table[0]);
    }

    G3X_AlphaBlend(TRUE);              // アルファブレンディングを許可

    /* レンダリングエンジンの参照データ群をスワップ
       (ＺバッファモードとＷバッファモードで、
       デプス値が異なることに注意。) */
    G3_SwapBuffers(GX_SORTMODE_AUTO,   // 自動ソートモード
                   GX_BUFFERMODE_Z);   // Ｚ値によるバッファリングモード

    DEMOStartDisplay();

    OS_Printf("mssg%d:PolygonAttr_DepthUpdate=%s\n", count++, (trg ? "ON" : "OFF"));

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        G3X_Reset();
        Rotate += 256;

        /* パッド情報の取得と操作設定 */
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] = { {PAD_KEY_UP, 10}, {0, 1}, {PAD_KEY_UP, 10}, {0, 1},
            {PAD_KEY_UP, 10}, {0, 1}, {PAD_KEY_UP, 10}, {0, 0}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_TRIG(PAD_KEY_UP))
        {
            if (++alpha > 31)
            {
                alpha = 31;
            }
            OS_Printf("mssg%d:alpha=%d\n", count++, alpha);
        }
        else if (DEMO_IS_TRIG(PAD_KEY_DOWN))
        {
            if (--alpha < 0)
            {
                alpha = 0;
            }
            OS_Printf("mssg%d:alpha=%d\n", count++, alpha);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            trg = (trg + 1) & 0x01;
            OS_Printf("mssg%d:PolygonAttr_DepthUpdate=%s\n", count++, (trg ? "ON" : "OFF"));
        }

        /* カメラの設定 */
        {
            VecFx32 Eye = { 0, 0, FX32_ONE * 8 };       // 視点位置
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // 視点設定
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // 行列をTextureモードに設定
        G3_Identity();                 // 現在の行列を単位行列に初期化
        // 行列をPosition・Vector 同時設定モードに設定
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        G3_PushMtx();                  // 行列をスタックへ入れる

        /* 立方体の回転計算と位置設定 */
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_RotY(s, c);
            G3_Translate(0, 0, 5 * FX32_ONE);
            G3_RotX(s, c);
            G3_RotZ(s, c);
        }

        /* 立方体の描画設定（フォグブレンドを行う） */
        // 質感の拡散反射色と環境反射色を設定
        G3_MaterialColorDiffAmb(GX_RGB(31, 0, 0),       // 拡散反射色
                                GX_RGB(16, 16, 16),     // 環境反射色
                                TRUE); // 頂点カラーを設定
        // 質感の鏡面反射色と放射光色を設定
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // 鏡面反射色
                                GX_RGB(0, 0, 0),        // 放射光色
                                FALSE); // 鏡面反射を未使用

        // 立方体のポリゴン属性（アルファとデプス）の設定
        {
            int     attr;

            if (trg)
            {
                attr = GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE;
            }
            else
            {
                attr = GX_POLYGON_ATTR_MISC_NONE;
            }
            attr |= GX_POLYGON_ATTR_MISC_FOG;
            // ポリゴン属性の設定
            G3_PolygonAttr(GX_LIGHTMASK_NONE,   // ライトの影響なし
                           GX_POLYGONMODE_MODULATE,     // モジュレーションモード
                           GX_CULL_BACK,        // カルバックを行う
                           0,          // ポリゴンＩＤ ０
                           alpha,      // アルファ値の設定
                           attr);
        }

        DrawCube();                    // 立方体の描画

        G3_PopMtx(1);

        /* 矩形の描画設定（フォグブレンドを行わない） */
        G3_PushMtx();
        // 質感の拡散反射色と環境反射色を設定
        G3_MaterialColorDiffAmb(GX_RGB(0, 0, 31),       // 拡散反射色
                                GX_RGB(16, 16, 16),     // 環境反射色
                                TRUE); // 頂点カラーを設定
        // 質感の鏡面反射色と放射光色を設定
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // 鏡面反射色
                                GX_RGB(0, 0, 0),        // 放射光色
                                FALSE); // 鏡面反射を未使用
        // ポリゴン属性の設定
        G3_PolygonAttr(GX_LIGHTMASK_NONE,       // ライトの影響なし
                       GX_POLYGONMODE_MODULATE, // モジュレーションモード
                       GX_CULL_BACK,   // カルバックを行う
                       0,              // ポリゴンＩＤ ０
                       31,             // アルファ値
                       0);

        G3_Begin(GX_BEGIN_QUADS);      // 頂点リストの開始(四角形ポリゴンで描画)
        {
            G3_Vtx(FX32_ONE << 2, -FX32_ONE << 2, 0);
            G3_Vtx(FX32_ONE << 2, FX32_ONE << 2, 0);
            G3_Vtx(-FX32_ONE, FX32_ONE << 2, 0);
            G3_Vtx(-FX32_ONE, -FX32_ONE << 2, 0);
        }
        G3_End();                      // 頂点リストの終了

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);

        /* Ｖブランク待ち */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x2A787000);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}

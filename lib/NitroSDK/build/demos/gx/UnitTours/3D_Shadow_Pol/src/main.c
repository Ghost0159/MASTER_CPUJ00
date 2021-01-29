/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Shadow_Pol
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.9  2006/01/18 02:11:26  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 10:39:35  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.7  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.6  2004/11/02 08:23:30  takano_makoto
  fix comment.

  Revision 1.5  2004/05/27 09:25:01  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:01:34  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:09:37  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


//---------------------------------------------------------------------------
// A sample that use shadow polygon.
// 
// This sample displays a sphere and ground.
// On the ground, the sphere's shadow is drawn.
//   
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
//        シャドウポリゴンを使用して影を表示
//  説明：
//        1. 球が地面に写す影を作るために円柱を用意する、この時、円柱は同じ物を
//         二つ用意する。
//        2. マスク用にPolygonAttrのポリゴン属性がシャドウポリゴンである円柱を
//         用意し、裏面のみ描画するものとする。
//        3. 次に、先程のと同じ座標に、描画用にPolygonAttrのポリゴン属性がシャ
//         ドウポリゴンである円柱を用意し、表面のみ描画するものとする。
//           この時、描画用の円柱には頂点カラーを与えておく。
//        4. この円柱を地面となるポリゴンに交差させると、円柱と地面とが交差して
//         いる部分に影が表示される。
//           この時表示される影の色は描画用のシャドウポリゴンのカラーが使用され
//         る。
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u16     Rotate = 0;                // for rotating sphear(0-65535)

    //---------------------
    // 初期化(IRQの許可、ＶＲＡＭの初期化、ＢＧ０を３Ｄモードで使用)
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    G3X_AlphaBlend(TRUE);              // アルファブレンドを許可

    DEMOStartDisplay();

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        G3X_Reset();
        Rotate += 256;

        /* カメラの設定 */
        {
            VecFx32 Eye = { 0, 15 * FX32_ONE, -15 * FX32_ONE }; // 視点位置
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // 視点設定
        }

        /* ライトの設定(ライトの色と向きを設定) */
        {
            G3_LightVector(GX_LIGHTID_0, 0, -FX32_ONE + 1, 0);
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // 行列をTextureモードに設定
        G3_Identity();                 // 現在の行列を単位行列に初期化
        // 行列をPosition・Vector 同時設定モードに設定
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        /* 地形の描画設定 */
        G3_PushMtx();
        // 材質の拡散反射色と環境反射色を設定
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // 拡散反射色
                                GX_RGB(16, 16, 16),     // 環境反射色
                                FALSE); // 頂点カラーは使用しない

        // 材質の鏡面反射色と放射光色を設定
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // 鏡面反射色
                                GX_RGB(0, 0, 0),        // 放射光色
                                FALSE); // 鏡面反射を未使用
        // テクスチャのパラメータを設定
        G3_TexImageParam(GX_TEXFMT_NONE,        // テクスチャは使用しない
                         GX_TEXGEN_NONE,
                         GX_TEXSIZE_S8,
                         GX_TEXSIZE_T8,
                         GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
        // ポリゴン属性の設定
        G3_PolygonAttr(GX_LIGHTMASK_0, // ライト０をオン
                       GX_POLYGONMODE_MODULATE, // モジュレーションモード
                       GX_CULL_NONE,   // カルバックを行わない
                       2,              // ポリゴンＩＤ ２
                       31,             // アルファ値
                       GX_POLYGON_ATTR_MISC_NONE);

        G3_Scale(10 * FX32_ONE, 0, 10 * FX32_ONE);

        G3_Begin(GX_BEGIN_QUADS);      // 頂点リストの開始(四角形ポリゴンでの描画)
        G3_Normal(0, 1 << 9, 0);
        G3_Vtx(FX16_ONE, 0, FX16_ONE);
        G3_Vtx(FX16_ONE, 0, -FX16_ONE);
        G3_Vtx(-FX16_ONE, 0, -FX16_ONE);
        G3_Vtx(-FX16_ONE, 0, FX16_ONE);
        G3_End();                      // 頂点リストの終了

        G3_PopMtx(1);


        G3_PushMtx();
        /* 球の回転計算と位置設定 */
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_RotY(s, c);
            G3_Translate(0, 5 * FX32_ONE, -5 * FX32_ONE);
            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        /* 球の描画設定 */
        // 材質の拡散反射色と環境反射色を設定
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // 拡散反射色
                                GX_RGB(16, 16, 16),     // 環境反射色
                                FALSE); // 頂点カラーは使用しない

        // 材質の鏡面反射色と放射光色を設定
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // 鏡面反射色
                                GX_RGB(0, 0, 0),        // 放射光色
                                FALSE); // 鏡面反射を未使用
        // テクスチャのパラメータを設定
        G3_TexImageParam(GX_TEXFMT_NONE,        // テクスチャは使用しない
                         GX_TEXGEN_NONE,
                         GX_TEXSIZE_S8,
                         GX_TEXSIZE_T8,
                         GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
        // ポリゴン属性の設定
        G3_PolygonAttr(GX_LIGHTMASK_0, // ライト０をオン
                       GX_POLYGONMODE_MODULATE, // モジュレーションモード
                       GX_CULL_BACK,   // カルバックを行う
                       1,              // ポリゴンＩＤ １
                       31,             // アルファ値
                       GX_POLYGON_ATTR_MISC_NONE);

        G3_Begin(GX_BEGIN_TRIANGLES);  // 頂点リストの開始(三角形ポリゴンで描画)
        {
            MI_SendGXCommand(3, sphere1, sphere_size);
        }
        G3_End();                      // 頂点リストの終了

        G3_PopMtx(1);

        /* マスク用シャドウポリゴンの位置設定 */
        G3_PushMtx();
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_RotY(s, c);
            G3_Translate(0, 0, -5 * FX32_ONE);
        }

        /* マスク用シャドウポリゴンの描画設定 */
        // 材質の拡散反射色と環境反射色を設定
        G3_MaterialColorDiffAmb(GX_RGB(0, 0, 0),        // 拡散反射色
                                GX_RGB(0, 0, 0),        // 環境反射色
                                FALSE); // 頂点カラーを使用しない
        // 材質の鏡面反射色と放射光色を設定
        G3_MaterialColorSpecEmi(GX_RGB(0, 0, 0),        // 鏡面反射色
                                GX_RGB(0, 0, 0),        // 放射光色
                                FALSE); // 鏡面反射を未使用
        // テクスチャのパラメータを設定
        G3_TexImageParam(GX_TEXFMT_NONE,        // テクスチャは使用しない
                         GX_TEXGEN_NONE,
                         GX_TEXSIZE_S8,
                         GX_TEXSIZE_T8,
                         GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
        // ポリゴン属性の設定
        G3_PolygonAttr(GX_LIGHTMASK_0, // ライトを反映しない
                       GX_POLYGONMODE_SHADOW,   // シャドウポリゴンモード
                       GX_CULL_FRONT,  // カルフロントを行う
                       0,              // ポリゴンＩＤ ０
                       0x08,           // アルファ値
                       GX_POLYGON_ATTR_MISC_NONE);

        // マスク（円柱）の描画
        G3_Begin(GX_BEGIN_TRIANGLES);  // 頂点リストの開始(三角形ポリゴンで描画)
        {
            MI_SendGXCommand(3, cylinder1, cylinder_size);
        }
        G3_End();                      // 頂点リストの終了

        /* 描画用シャドウポリゴンの描画設定 */
        // 材質の拡散反射色と環境反射色を設定
        G3_MaterialColorDiffAmb(GX_RGB(0, 0, 0),        // 拡散反射色
                                GX_RGB(0, 0, 0),        // 環境反射色
                                TRUE); // 頂点カラーを使用
        // 材質の鏡面反射色と放射光色を設定
        G3_MaterialColorSpecEmi(GX_RGB(0, 0, 0),        // 鏡面反射色
                                GX_RGB(0, 0, 0),        // 放射光色
                                FALSE); // 鏡面反射を未使用
        // テクスチャのパラメータを設定
        G3_TexImageParam(GX_TEXFMT_NONE,        // テクスチャは使用しない
                         GX_TEXGEN_NONE,
                         GX_TEXSIZE_S8,
                         GX_TEXSIZE_T8,
                         GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
        // ポリゴン属性の設定
        G3_PolygonAttr(GX_LIGHTMASK_0, // ライトを反映しない
                       GX_POLYGONMODE_SHADOW,   // シャドウポリゴンモード
                       GX_CULL_BACK,   // カルバックを行う
                       1,              // ポリゴンＩＤ １
                       0x08,           // アルファ値
                       GX_POLYGON_ATTR_MISC_NONE);      // 

        // シャドウ（円柱）の描画
        G3_Begin(GX_BEGIN_TRIANGLES);  // 頂点リストの開始(三角形ポリゴンで描画)
        {
            MI_SendGXCommand(3, cylinder1, cylinder_size);
        }
        G3_End();                      // 頂点リストの終了

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W);

        /* Ｖブランク待ち */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x38FFDE1A);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}

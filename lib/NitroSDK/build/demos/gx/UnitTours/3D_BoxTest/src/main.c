/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_BoxTest
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

  Revision 1.8  2005/11/21 10:33:02  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.7  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.6  2004/07/29 06:50:14  takano_makoto
  Add PolygonAttr setting before BoxTest.

  Revision 1.5  2004/05/27 09:23:45  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 01:58:41  takano_makoto
  modify comments and programs a little.

  Revision 1.1  2004/03/01 09:04:35  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use Box test.
// 
// This sample displays a cube and a rolling sphere.
// If the sphere contact with the cube, it is displayed normaly.
// The other case it is diplayed by wire frame.
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
//        ボックステストを行う
//  説明：
//        回転するワイヤーフレームの球体が周囲を囲む立方体の面に接触すると、
//      ボックステストの結果通常描画される。
//        
//        1. 正射影で空間中にBoxTestでカリングをする空間を設定する。
//        2. 正射影で設定されたカレントクリップ座標行列に対して、球と同サイズの
//         立方体を使ってBoxTestを行う。
// 　　　 3. その後、透視射影に変換してワイヤーフレームで先ほど設定したカリング
//         空間を描画する。
//        4. そして球の描画の際に先立って行ったBoxTestの結果を元に通常描画と
//　　     ワイヤーフレーム描画を切り替える。
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u16     Rotate = 0;                // for rotating cubes(0-65535)

    //---------------------
    // 初期化
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();

    G3X_Init();                        // ３Ｄグラフィックの初期化
    G3X_InitTable();                   // グラフィックテーブルの初期化
    G3X_InitMtxStack();                // 行列スタックの初期化

    /* ３Ｄグラフィックの設定 */
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);       // ＢＧの０番を使用

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // ２Ｄ／３Ｄモード
                       GX_BGMODE_4,    // BGMODEを４に設定
                       GX_BG0_AS_3D);  // ＢＧの０を３Ｄとして使用
    G2_SetBG0Priority(0);              // ＢＧ０番の表示優先度をトップに

    G3X_SetShading(GX_SHADING_HIGHLIGHT);       // シェーディングモードの設定
    G3X_AntiAlias(TRUE);               // アンチエイリアスを有効

    G3_SwapBuffers(GX_SORTMODE_AUTO,   // スワップバッファの設定
                   GX_BUFFERMODE_W);
    // クリアカラーの設定
    G3X_SetClearColor(GX_RGB(0, 0, 0), // clear color
                      0,               // clear alpha
                      0x7fff,          // clear depth
                      63,              // clear polygon ID
                      FALSE);          // fog

    G3_ViewPort(0, 0, 255, 191);       // ビューポートの設定

    DEMOStartDisplay();

    //---------------------
    //  メインループ
    //---------------------
    while (1)
    {
        int     sphere_alpha;

        G3X_Reset();
        Rotate += 256;

        /* 正射影の設定 */
        {
            fx32    pos1_x = -((5 * FX32_ONE) / 2);
            fx32    pos1_y = ((5 * FX32_ONE) / 2);
            fx32    pos2_x = ((5 * FX32_ONE) / 2);
            fx32    pos2_y = -((5 * FX32_ONE) / 2);
            fx32    near = (5 * FX32_ONE) / 2;
            fx32    far = ((5 * FX32_ONE) / 2) + (5 * FX32_ONE);

            G3_MtxMode(GX_MTXMODE_PROJECTION);  // 行列をProjectionモードに設定
            G3_Identity();             // 現在の行列を単位行列に初期化
            // 正射影の設定
            G3_Ortho(pos1_y,           // up    pos Y
                     pos2_y,           // down  pos Y
                     pos1_x,           // left  pos X
                     pos2_x,           // right pos X
                     near,             // near
                     far,              // far
                     NULL);

            G3_StoreMtx(0);
        }

        /* ボックステスト */
        {
            G3_MtxMode(GX_MTXMODE_TEXTURE);     // 行列をTextureモードに設定
            G3_Identity();             // 現在の行列を単位行列に初期化
            // 行列をPosition・Vector 同時設定モードに設定
            G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
            G3_PushMtx();
            {
                GXBoxTestParam box;
                fx16    s = FX_SinIdx(Rotate);
                fx16    c = FX_CosIdx(Rotate);
                s32     result;

                G3_Translate(0, 0, -11 * FX32_ONE);
                G3_RotY(s, c);
                G3_Translate(0, 0, 6 * FX32_ONE);
                G3_RotX(s, c);
                G3_RotY(s, c);
                G3_RotZ(s, c);

                // BoxTestの前にはFARクリップ及び1ドットポリゴンレンダリングフラグを
                // ONに設定する
                G3_PolygonAttr(GX_LIGHTMASK_0,
                               GX_POLYGONMODE_MODULATE,
                               GX_CULL_NONE,
                               0,
                               0,
                               GX_POLYGON_ATTR_MISC_FAR_CLIPPING | GX_POLYGON_ATTR_MISC_DISP_1DOT);
                G3_Begin(GX_BEGIN_TRIANGLES);
                G3_End();

                box.x = -7200;
                box.y = -7200;
                box.z = -7200;
                box.width = 7200 * 2;
                box.height = 7200 * 2;
                box.depth = 7200 * 2;
                G3_BoxTest(&box);      // ボックステストを行う

                while (G3X_GetBoxTestResult(&result) != 0)
                {
                    // ボックステストの結果待ちループ
                    ;
                }
                sphere_alpha = (result ? 31 : 0);
                OS_Printf("result %d\n", result);
            }
            G3_PopMtx(1);
        }

        /* 透視射影の設定 */
        G3_MtxMode(GX_MTXMODE_PROJECTION);      // 行列をProjectionモードに設定
        G3_Identity();                 // 現在の行列を単位行列に初期化
        {
            fx32    right = FX32_ONE;
            fx32    top = FX32_ONE * 3 / 4;
            fx32    near = FX32_ONE;
            fx32    far = FX32_ONE * 400;
            // 透視射影の設定
            G3_Perspective(FX32_SIN30, // Sine   FOVY
                           FX32_COS30, // Cosine FOVY
                           FX32_ONE * 4 / 3,    // Aspect
                           near,       // Near
                           far,        // Far
                           NULL);

            G3_StoreMtx(0);
        }

        /* カメラの設定 */
        {
            VecFx32 Eye = { 0, 8 * FX32_ONE, FX32_ONE };        // 視点位置
            VecFx32 at = { 0, 0, -5 * FX32_ONE };       // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // 視点設定 
        }

        /* ライトの設定 */
        {
            G3_LightVector(GX_LIGHTID_0, 0, -FX32_ONE + 1, 0);  // ライトの色
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));    // ライトの向き
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // 行列をTextureモードに設定
        G3_Identity();                 // 現在の行列を単位行列に初期化
        // 行列をPosition・Vector 同時設定モードに設定
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        /* ボックスの生成と描画 */
        {
            G3_PushMtx();
            G3_Translate(0, 0, -5 * FX32_ONE);  // ボックスの位置設定
            // 材質の拡散反射色と環境反射色を設定
            G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // 拡散反射色
                                    GX_RGB(16, 16, 16), // 環境反射色
                                    FALSE);     // 頂点カラーをセットしない
            // 材質の鏡面反射色と放射光色を設定
            G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // 鏡面反射色
                                    GX_RGB(0, 0, 0),    // 放射光色
                                    FALSE);     // 鏡面反射テーブルを未使用
            // テクスチャのパラメータを設定
            G3_TexImageParam(GX_TEXFMT_NONE,    // テクスチャなし
                             GX_TEXGEN_NONE,    // テクスチャ座標変換なし
                             GX_TEXSIZE_S8,
                             GX_TEXSIZE_T8,
                             GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
            // ポリゴン属性の設定
            G3_PolygonAttr(GX_LIGHTMASK_0,      // ライト０をオン
                           GX_POLYGONMODE_MODULATE,     // モジュレーションモード
                           GX_CULL_NONE,        // カリング無し
                           1,          // ポリゴンＩＤ １
                           0,          // アルファ値
                           GX_POLYGON_ATTR_MISC_NONE);
            G3_Begin(GX_BEGIN_QUADS);  // 頂点リストの開始
            // (四角形ポリゴンでの描画宣言)
            {
                G3_Normal(0, 1 << 9, 0);
                G3_Vtx(((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));

                G3_Normal(0, -1 << 9, 0);
                G3_Vtx(((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));

                G3_Normal(1 << 9, 0, 0);
                G3_Vtx(((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));

                G3_Normal(-1 << 9, 0, 0);
                G3_Vtx(-((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
            }
            G3_End();                  // 頂点リストの終了
            G3_PopMtx(1);
        }

        /* 球の生成と描画 */
        {
            G3_PushMtx();              // 行列をスタックへ
            {                          // 移動位置の計算
                fx16    s = FX_SinIdx(Rotate);
                fx16    c = FX_CosIdx(Rotate);

                G3_Translate(0, 0, -11 * FX32_ONE);
                G3_RotY(s, c);
                G3_Translate(0, 0, 6 * FX32_ONE);
                G3_RotX(s, c);
                G3_RotY(s, c);
                G3_RotZ(s, c);
            }
            // 材質の拡散反射色と環境反射色を設定
            G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // 拡散反射色
                                    GX_RGB(16, 16, 16), // 環境反射色
                                    FALSE);     // 頂点カラーをセットしない
            // 材質の鏡面反射色と放射光色を設定
            G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // 鏡面反射色
                                    GX_RGB(0, 0, 0),    // 放射光色
                                    FALSE);     // 鏡面反射テーブルを使用せず
            // テクスチャのパラメータを設定
            G3_TexImageParam(GX_TEXFMT_NONE,    // テクスチャなし
                             GX_TEXGEN_NONE,    // テクスチャ座標変換なし
                             GX_TEXSIZE_S8,
                             GX_TEXSIZE_T8,
                             GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
            // ポリゴン関連属性値を設定
            G3_PolygonAttr(GX_LIGHTMASK_0,      // ライト０をオン
                           GX_POLYGONMODE_MODULATE,     // モジュレーションモード
                           GX_CULL_NONE,        // カリング無し
                           1,          // ポリゴンＩＤ １
                           sphere_alpha,        // アルファ値
                           GX_POLYGON_ATTR_MISC_NONE);

            OS_Printf("sphere_alpha=%d\n", sphere_alpha);
            // 球体の描画
            G3_Begin(GX_BEGIN_TRIANGLES);       /* 頂点リストの開始
                                                   (三角形ポリゴンでの描画宣言) */
            {
                MI_SendGXCommand(3, sphere1, sphere_size);
            }
            G3_End();                  // 頂点リストの終了
            G3_PopMtx(1);
        }

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        /* VBlank待ち */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x1959EE87);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

    }
}

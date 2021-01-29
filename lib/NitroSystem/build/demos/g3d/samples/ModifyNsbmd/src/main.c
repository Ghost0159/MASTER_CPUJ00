/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ModifyNsbmd
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.14 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// モデルリソース内のデータを書き換えるサンプル
//
// G3Dのライブラリ内では初期化又は終了時を除いてモデルリソース内のデータを
// 書き換えることはありませんが、ユーザーが明示的にコードを書けば直接書き換える
// ことは可能です。本サンプルでは、G3Dのmodel.hで宣言されている関数を用いて
// マテリアル内の各属性にアクセスしています。
//
// サンプルの動作
// モデルリソース内のマテリアルデータは毎フレーム変更されています。
// B,Xボタンでメニュー(マテリアルの各属性)を上下に移動します。
// AボタンでマテリアルのデータにNNSG3dGlbXXXX関数で設定されたグローバル情報を
// モデルリソース内のマテリアルデータの代わりに使用するかどうか設定します。ボタンを
// 押すことで切り替わります。
//
// HOWTO:
// ・NNS_G3dMdlUseMdlXXXXでモデルリソース内のマテリアルデータを描画に反映させます(デフォルト)。
// ・NNS_G3dMdlUseGlbXXXXでNSG3dGlbXXXX関数で設定されたマテリアルデータを描画に反映させます。
// ・NNS_G3dMdlSetMdlXXXXでモデルリソース内のマテリアルデータを書き換えます。
// ・NNS_G3dMdlGetMdlXXXXでモデルリソース内のマテリアルデータを取得します。
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);

//  モデルのあるマテリアルを時間変化させるためのタイマー
static int mdlTimer = 0;
static void changeMdlMat(NNSG3dResMdl* pMdl);
static GXRgb calcNextColor(GXRgb rgb);

#define	NUM_ITEM	9


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  サンプルのメイン。
                X,Bで項目選択,AでGlb,Mdlの切り替え

  Arguments:    なし。

  Returns:      なし。
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    //  項目関係の変数定義
//  const int NumItem = 9;

    int itemNo = 0;
    const char* ItemName[NUM_ITEM][2] = {
        {"MdlDiff","GlbDiff"},
        {"MdlAmb","GlbAmb"},
        {"MdlSpec","GlbSpec"},
        {"MdlEmi","GlbEmi"},
        {"MdlLightFlag","GlbLightFlag"},
        {"MdlPolyMode","GlbPolyMode"},
        {"MdlCullMode","GlbCullMode"},
        {"MdlPolyID","GlbPolyID"},
        {"MdlAlpha","GlbAlpha"}
    };
    int itemFlag[NUM_ITEM] = {
        0,0,0,0,0,0,0,0,0
    };
    

    NNSG3dRenderObj object;
    NNSG3dResMdl*    model;

    G3DDemo_InitSystem();
    G3DDemo_InitMemory();
    G3DDemo_InitVRAM();

    InitG3DLib();
    G3DDemo_InitDisplay();

    // マネージャがテクスチャイメージスロットを4スロット分管理できるようにして
    // デフォルトマネージャにする。
    NNS_GfdInitFrmTexVramManager(4, TRUE);

    // マネージャがパレットを32KB分管理できるようにして
    // デフォルトマネージャにする。
    NNS_GfdInitFrmPlttVramManager(0x8000, TRUE);

    model = LoadG3DModel("data/human_run_t.nsbmd");
    SDK_ASSERTMSG(model, "load failed");

    //
    // G3D: NNSG3dRenderObjの初期化
    //
    // NNSG3dRenderObjは１つのモデルとそのモデルに適用されるアニメに関する各種情報を
    // 保持する構造体です。
    // 内部のメンバポインタで指定される領域はユーザー自身で取得・解放する必要があります。
    // ここでmdlはNULLであってもかまいません(NNS_G3dDraw時には設定されている必要がある)。
    //
    NNS_G3dRenderObjInit(&object, model);

    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ModifyNsBmd");

    for(;;)
    {
        OSTick time;

        SVC_WaitVBlankIntr();

        G3DDemo_PrintApplyToHW();
        G3DDemo_ReadGamePad();
        
        //  グローバルなパラメータを使うか,モデルのパラメータを使うかを設定する
        //  model.hで宣言されている関数を使用
        if (itemFlag[0] == 0)
            NNS_G3dMdlUseMdlDiff(model);
        else
            NNS_G3dMdlUseGlbDiff(model);

        if (itemFlag[1] == 0)
            NNS_G3dMdlUseMdlAmb(model);
        else
            NNS_G3dMdlUseGlbAmb(model);

        if (itemFlag[2] == 0)
            NNS_G3dMdlUseMdlSpec(model);
        else
            NNS_G3dMdlUseGlbSpec(model);

        if (itemFlag[3] == 0)
            NNS_G3dMdlUseMdlEmi(model);
        else
            NNS_G3dMdlUseGlbEmi(model);

        if (itemFlag[4] == 0)
            NNS_G3dMdlUseMdlLightEnableFlag(model);
        else
            NNS_G3dMdlUseGlbLightEnableFlag(model);

        if (itemFlag[5] == 0)
            NNS_G3dMdlUseMdlPolygonMode(model);
        else
            NNS_G3dMdlUseGlbPolygonMode(model);

        if (itemFlag[6] == 0)
            NNS_G3dMdlUseMdlCullMode(model);
        else
            NNS_G3dMdlUseGlbCullMode(model);

        if (itemFlag[7] == 0)
            NNS_G3dMdlUseMdlPolygonID(model);
        else
            NNS_G3dMdlUseGlbPolygonID(model);

        if (itemFlag[8] == 0)
            NNS_G3dMdlUseMdlAlpha(model);
        else
            NNS_G3dMdlUseGlbAlpha(model);

        //  モデルのあるマテリアルのパラメータを時間変化させる
        changeMdlMat(model);

        G3DDemo_MoveCamera(&gCamera);
        G3DDemo_CalcCamera(&gCamera);

        G3X_Reset();
        G3X_ResetMtxStack();

        SetCamera(&gCamera);

        {
            VecFx32 scale = {FX32_ONE >> 2, FX32_ONE >> 2, FX32_ONE >> 2};
            VecFx32 trans = {FX32_ONE * 4, 0, 0};

            //
            // モデルのベーススケールとベーストランスレーションを設定
            //
            NNS_G3dGlbSetBaseScale(&scale);
            NNS_G3dGlbSetBaseTrans(&trans);

            //
            // G3D:
            // G3dGlbXXXXで設定した状態をまとめて転送します。
            // カメラや射影行列等が設定されています。
            //
            NNS_G3dGlbFlushP();

            //  時間測定開始
            time = OS_GetTick();
            {
                //
                // G3D: NNS_G3dDraw
                // セットアップされたNNSG3dRenderObjを渡すことにより
                // あらゆる場合の描画を行う
                //
                NNS_G3dDraw(&object);
            }
            // 時間測定結果画面表示。
            time = OS_GetTick() - time;
        }

        // スケールを元に戻す
        {
            VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
            NNS_G3dGlbSetBaseScale(&scale);
        }
        //  トランスも元に戻す
        {
            VecFx32 trans = {0, 0, 0};
            NNS_G3dGlbSetBaseTrans(&trans);
        }
        //
        // G3D: 通常のG3_XXXを呼ぶときは、その前にNNS_G3dGeComFlushBuffer()
        //      をコールして同期しなくてはならない
        //
        NNS_G3dGlbFlushP();
        NNS_G3dGeFlushBuffer();
        G3DDemo_DrawGround(&gGround);
        

        /* ジオメトリ＆レンダリングエンジン関連メモリのスワップ */
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
        
        
        //  X,Bで選択項目切り替え,ボタンで変更
        {
            if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_X))
            {
                itemNo --;
                if(itemNo < 0)
                {
                    itemNo = NUM_ITEM - 1;
                }
            }
            if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_B))
            {
                itemNo ++;
                if(itemNo >= NUM_ITEM)
                {
                    itemNo = 0;
                }
            }
            if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
            {
                itemFlag[itemNo] = 1 - itemFlag[itemNo];
            }
        }
        
        
        //  項目の現状態表示
        {
            int item;
            G3DDemo_Printf(0,2, G3DDEMO_COLOR_GREEN, "select:X,B");
            G3DDemo_Printf(0,3, G3DDEMO_COLOR_GREEN, "change:A");
            for(item = 0;item < NUM_ITEM;item ++)
            {
                int color = G3DDEMO_COLOR_WHITE;
                if(item == itemNo)
                {
                    color = G3DDEMO_COLOR_RED;
                }
                G3DDemo_Printf(0,5 + item, color, "%s", ItemName[item][itemFlag[item]]);
            }
        }

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06d usec", OS_TicksToMicroSeconds(time));
    }
}


/* -------------------------------------------------------------------------
  Name:         changeMdlMat

  Description:  モデルのあるマテリアルの設定を時間変化させる

  Arguments:    モデルへのポインタ:NNSG3dResMdl* pMdl

  Returns:      なし。
   ------------------------------------------------------------------------- */
void
changeMdlMat(NNSG3dResMdl* pMdl)
{
    u32 MatID;

    mdlTimer ++;
    if(mdlTimer >= 32 * 32 * 32)
    {
        mdlTimer = 0;
    }

    for (MatID = 0; MatID < pMdl->info.numMat; ++MatID)
    {
        //
        // フレーム毎にモデルのマテリアルカラーを変化させます。
        //
        NNS_G3dMdlSetMdlDiff(pMdl,
                             MatID,
                             calcNextColor(NNS_G3dMdlGetMdlDiff(pMdl, MatID)));

        NNS_G3dMdlSetMdlAmb(pMdl,
                            MatID,
                            calcNextColor(NNS_G3dMdlGetMdlAmb(pMdl, MatID)));

        NNS_G3dMdlSetMdlSpec(pMdl,
                             MatID,
                             calcNextColor(NNS_G3dMdlGetMdlSpec(pMdl, MatID)));

        NNS_G3dMdlSetMdlEmi(pMdl,
                            MatID,
                            calcNextColor(NNS_G3dMdlGetMdlEmi(pMdl, MatID)));

        //
        // モデルにあたるライトのON/OFFを変化させます。
        //
        NNS_G3dMdlSetMdlLightEnableFlag(pMdl,
                                        MatID,
                                        mdlTimer / 55 % 16);

        //
        // モデルのポリゴンモードを変化させます。
        //
        NNS_G3dMdlSetMdlPolygonMode(pMdl,
                                    MatID,
                                    (GXPolygonMode)(mdlTimer / 60 % 2));

        //
        // モデルのカルモードを変化させます。
        //
        NNS_G3dMdlSetMdlCullMode(pMdl,
                                 MatID,
                                 (GXCull)(mdlTimer / 73 % 2 + 1));

        //
        // モデルのポリゴンIDを設定します。
        //
        NNS_G3dMdlSetMdlPolygonID(pMdl,
                                  MatID,
                                  31);

        //
        // モデルのαを変化させます。
        //
        NNS_G3dMdlSetMdlAlpha(pMdl,
                              MatID,
                              mdlTimer / 7 % 32);
    }
}


/* -------------------------------------------------------------------------
  Name:         calcNextColor

  Description:  マテリアルの色を変化させるため,今の色から次の色を計算する

  Arguments:    今の色:GXRgb rgb

  Returns:      次の色:GXRgb
   ------------------------------------------------------------------------- */
GXRgb
calcNextColor(GXRgb rgb) {
    //  各成分の抽出
    u16 R = (u16)((rgb & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT);
    u16 G = (u16)((rgb & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT);
    u16 B = (u16)((rgb & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT);
    
    const int ChangeSpeed = 2;
    
    R += ChangeSpeed;
    if(R > 31)
    {
        R -= 31;
        G += ChangeSpeed;
        if(G > 31)
        {
            G -= 31;
            B += ChangeSpeed;
            if(B > 31)
            {
                B -= ChangeSpeed;
            }
        }
    }

    return GX_RGB(R, G, B);
}


/* -------------------------------------------------------------------------
  Name:         InitG3DLib

  Description:  G3Dライブラリを初期化します。

  Arguments:    なし。

  Returns:      なし。
   ------------------------------------------------------------------------- */
static void
InitG3DLib(void)
{
    //
    // G3D:
    // デフォルトの初期化を行う
    // ------------------------------------------------------------------------
    NNS_G3dInit();

    //
    // G3D:
    // グローバルライトの設定
    // ------------------------------------------------------------------------
    NNS_G3dGlbLightVector(GX_LIGHTID_0, -FX16_SQRT1_3, -FX16_SQRT1_3, -FX16_SQRT1_3);
    NNS_G3dGlbLightVector(GX_LIGHTID_1,  FX16_SQRT1_3,  FX16_SQRT1_3,  FX16_SQRT1_3);
    NNS_G3dGlbLightVector(GX_LIGHTID_2,             0,   -FX16_ONE  ,             0);
    NNS_G3dGlbLightVector(GX_LIGHTID_3,             0,    FX16_ONE-1,             0);

    NNS_G3dGlbLightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
    NNS_G3dGlbLightColor(GX_LIGHTID_1, GX_RGB(10, 10, 10));
    NNS_G3dGlbLightColor(GX_LIGHTID_2, GX_RGB(16,  0,  0));
    NNS_G3dGlbLightColor(GX_LIGHTID_3, GX_RGB(16, 16,  0));

    NNS_G3dGlbMaterialColorDiffAmb(GX_RGB(20, 10, 10), GX_RGB(6, 6, 6), FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(GX_RGB(10, 16, 0), GX_RGB(6, 6, 14), FALSE);
    
    //  グローバルポリゴン属性の設定
    NNS_G3dGlbPolygonAttr(
            0x000f,                                   // ライトは全てON
            GX_POLYGONMODE_MODULATE,
            GX_CULL_BACK,
            0,
            28,
            GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE
    );
}


/* -------------------------------------------------------------------------
  Name:         LoadG3DModel

  Description:  ファイルからモデルデータを読み込みます。

  Arguments:    path: モデルデータのパス名。

  Returns:      モデルへのポインタ。
   ------------------------------------------------------------------------- */
static NNSG3dResMdl*
LoadG3DModel(const char* path)
{
    BOOL status;
    NNSG3dResMdl* model = NULL;
    NNSG3dResTex* texture = NULL;
    NNSG3dResFileHeader* resFile = G3DDemo_LoadFile(path);

    SDK_NULL_ASSERT(resFile);

    // NNS_G3dResDefaultSetupやNNS_G3dDrawの中でDMA転送を使用するため、
    // これらの関数を呼ぶ前にモデルリソース全体をメモリへストアします。
    // リソースのサイズがある程度大きい場合はDC_StoreAllを呼んだ方が高速です。
    // DC_StoreRangeやDC_StoreAllについてはNITRO-SDK関数リファレンスマニュアルを参照して下さい。
    DC_StoreRange(resFile, resFile->fileSize);

    // デフォルトの初期化関数をコールしてセットアップする場合
    status = NNS_G3dResDefaultSetup(resFile);
    NNS_G3D_ASSERTMSG(status, "NNS_G3dResDefaultSetup failed");

    //
    // G3D: モデルの取得
    // nsbmdはモデルを複数含むことができるので、インデックス(モデルが１つの場合は0)
    // を指定して１つのモデルへのポインタを取得する。
    // --------------------------------------------------------------------
    model = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFile), 0);
    return model;
}

/* -------------------------------------------------------------------------
  Name:         SetCamera

  Description:  G3Dのグローバル状態にカメラ行列と射影行列を設定します。

  Arguments:    camera:     G3DDemoCamera構造体へのポインタ。

  Returns:      なし。
   ------------------------------------------------------------------------- */
static void
SetCamera(G3DDemoCamera* camera)
{
    G3DDemoLookAt* lookat = &camera->lookat;
    G3DDemoPersp*  persp  = &camera->persp;

    NNS_G3dGlbPerspective(persp->fovySin, persp->fovyCos, persp->aspect, persp->nearClip, persp->farClip);
    NNS_G3dGlbLookAt(&lookat->camPos, &lookat->camUp, &lookat->target);
}

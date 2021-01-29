/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - 1mat1shp
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// 非常に単純なモデルを高速に表示するサンプル
//
// NNS_G3dDraw1Mat1Shpを使用することにより、指定されたマテリアルIDとシェイプID
// の組を高速に描画します。
//
// サンプルの動作
// Aボタンを押すことにより、NNS_G3dDraw1MatShpによる描画とNNS_G3dDrawによる描画
// を切り替えます。
//
// HOWTO:
// 1: モデルのリソースファイル（.nsbmdファイル）をメインメモリにロードします。
// 2: NNS_G3dResDefaultSetup関数でロードしたリソースに対してデフォルトのセットアップ動作
//    を行います。
// 3: モデルリソースから使用したいモデルを、NNS_G3dGetMdlSet関数とNNS_G3dGetMdlByIdx関数を
//    使用して取得します。
// 4: NNS_G3dGlbXXXX関数を使用して、ライトの向きや色、射影行列、カメラ行列、モデルの
//    ベースSRT等を設定します。
// 5: NNS_G3dGlbFlush関数を使用して5で設定した値をジオメトリエンジンにまとめて送信します。
// 6: NNS_G3dDraw1Mat1Shp関数を使用して、単純なモデルを高速に描画します。
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void           InitG3DLib(void);
static NNSG3dResMdl*  LoadG3DModel(const char* path);
static void           SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  サンプルのメイン。

  Arguments:    なし。

  Returns:      なし。
 ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    NNSG3dRenderObj object;
    NNSG3dResMdl*   pModel;
    void*           pResFile;
    BOOL            is1Mat1Shp = TRUE;

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

    {
        //
        // モデルリソースのロード
        //
        pResFile = G3DDemo_LoadFile("data/cube_n_t.nsbmd");
        SDK_ASSERTMSG(pResFile, "load failed");

        // NNS_G3dResDefaultSetupやNNS_G3dDraw*の中でDMA転送を使用するため、
        // これらの関数を呼ぶ前にモデルリソース全体をメモリへストアします。
        // リソースのサイズがある程度大きい場合はDC_StoreAllを呼んだ方が高速です。
        // DC_StoreRangeやDC_StoreAllについてはNITRO-SDK関数リファレンスマニュアルを参照して下さい。
        DC_StoreRange(pResFile, ((NNSG3dResFileHeader*)pResFile)->fileSize);

        //
        // デフォルトの初期化関数をコールしてセットアップする
        //
        if (!NNS_G3dResDefaultSetup(pResFile))
        {
            NNS_G3D_ASSERTMSG(0, "NNS_G3dResDefaultSetup failed");
        }

        //
        // モデルリソース内のモデルセットからインデックス#0のモデルを取得
        //
        pModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(pResFile), 0);
        SDK_ASSERTMSG(pModel, "model not found");
    }

    //
    // G3D: NNSG3dRenderObjの初期化
    //
    // NNS_G3dDraw1Mat1Shpには必要ありませんが、通常の描画方法と比較するために使います。
    //
    NNS_G3dRenderObjInit(&object, pModel);

    G3DDemo_InitCamera(&gCamera, 0*FX32_ONE, 120*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "1MAT1SHP");

    for(;;)
    {
        OSTick time;

        SVC_WaitVBlankIntr();
        G3DDemo_PrintApplyToHW();
        G3DDemo_ReadGamePad();

        G3DDemo_MoveCamera(&gCamera);
        G3DDemo_CalcCamera(&gCamera);

        G3X_Reset();
        G3X_ResetMtxStack();

        SetCamera(&gCamera);

        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            is1Mat1Shp = is1Mat1Shp ? FALSE : TRUE;
        }
        if (is1Mat1Shp)
        {
            G3DDemo_Printf(0,1, G3DDEMO_COLOR_GREEN, "NNS_G3dDraw1Mat1Shp");
        }
        else
        {
            G3DDemo_Printf(0,1, G3DDEMO_COLOR_GREEN, "NNS_G3dDraw        ");
        }

        {
            //
            // G3D:
            // G3dGlbXXXXで設定した状態をまとめて転送します。
            // カメラや射影行列等が設定されています。
            //
            NNS_G3dGlbFlush();

            time = OS_GetTick();
            {
                //
                // G3D: NNS_G3dDraw1Mat1Shp
                //
                // マテリアルIDとシェイプIDをユーザーが指定して描画します。
                // アニメーションはサポートされていません。
                // 最後の引数をFALSEにするとマテリアルの送信を省略します。
                // (ただし、モデルリソースのマテリアルを操作することによって
                //  マテリアルアニメーションの効果を得ることができます。
                //  model.hとサンプルのModifyNsbmdを参照してください)
                //
                int i, j;
                BOOL sendMaterial = TRUE;
                VecFx32 trans = {0, 0, 0};

                //
                // テクスチャ付ボックスを200個並べる
                //
                for (i = 0; i < 20; ++i)
                {
                    trans.z = (i - 10) * FX32_ONE * 7;
                    for (j = 0; j < 10; ++j)
                    {
                        trans.x = (j - 5) * FX32_ONE * 7;

                        // 単純なオブジェクトなので、NNS_G3dGlbFlushは使わずに、
                        // カレント行列だけをセットしてしまってもよい。
                        NNS_G3dGeLoadMtx43(&NNS_G3dGlb.cameraMtx);
                        NNS_G3dGeTranslateVec(&trans);

                        if (is1Mat1Shp)
                        {
                            // 2個目以降はマテリアル情報をジオメトリエンジンに送信しない。
                            NNS_G3dDraw1Mat1Shp(pModel, 0, 0, sendMaterial);
                            sendMaterial = FALSE;
                        }
                        else
                        {
                            NNS_G3dDraw(&object);
                        }
                    }
                }
            }
            time = OS_GetTick() - time;
        }

        {
            VecFx32 trans = {0, 0, 0};
            NNS_G3dGlbSetBaseTrans(&trans);
            //
            // G3D: 通常のG3_XXXを呼ぶときは、その前にNNS_G3dGeComFlushBuffer()
            //      をコールして同期しなくてはならない
            //
            NNS_G3dGlbFlush();
            NNS_G3dGeFlushBuffer();
            G3DDemo_DrawGround(&gGround);
        }

        /* ジオメトリ＆レンダリングエンジン関連メモリのスワップ */
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06d usec", OS_TicksToMicroSeconds(time));
    }
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

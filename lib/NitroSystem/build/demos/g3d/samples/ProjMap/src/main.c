/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ProjMap
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// 投影マップされたオブジェクトを表示するサンプル
//
// .imdファイルの<material>要素のtex_gen_modeがposに設定されている場合、
// 投影マップを使用してテクスチャをマッピングします。
//
// サンプルの動作
// サンプルでは、テクスチャはxy座標で(-1.0, -1.0)から(1.0, 1.0)の座標に
// 正射影されるようになっています(effect_mtxが単位行列の場合)。
// オブジェクトはX座標で-1.0から1.0までの間を常に移動しています。
//
// Aボタンを押すと --> 描画前の射影行列の設定方法が切り替わります。
//                     どの方法でも同じようにテクスチャがマッピングされます。
// Xボタンを押すと --> X軸(ビュー座標系で)を中心に環境マップが回転します。
// Yボタンを押すと --> Y軸(ビュー座標系で)を中心に環境マップが回転します。
// Bボタンを押すと --> Z軸(ビュー座標系で)を中心に環境マップが回転します。
//
//---------------------------------------------------------------------------


#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void             InitG3DLib(void);
static NNSG3dResMdl*    LoadG3DModel(const char* path);
static void             SetCamera(G3DDemoCamera* camera);


/*-------------------------------------------------------------------------
    rotEnvMap

    環境マップの方向を変える処理を行うコールバック関数
  -------------------------------------------------------------------------*/
static void
rotProjMap(NNSG3dRS* rs)
{
    static MtxFx33 mRot = {
        FX32_ONE, 0, 0,
        0, FX32_ONE, 0,
        0, 0, FX32_ONE
    };
    MtxFx33 m;

    if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_B))
    {
        MTX_RotZ33(&m, FX_SinIdx(256), FX_CosIdx(256));
        MTX_Concat33(&m, &mRot, &mRot);
    }

    if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_Y))
    {
        MTX_RotY33(&m, FX_SinIdx(256), FX_CosIdx(256));
        MTX_Concat33(&m, &mRot, &mRot);
    }

    if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_X))
    {
        MTX_RotX33(&m, FX_SinIdx(256), FX_CosIdx(256));
        MTX_Concat33(&m, &mRot, &mRot);
    }

    // effect_mtxを自分で作成し合成することにより、投影されるテクスチャの方向を変えることができる。
    NNS_G3dGeMultMtx33(&mRot);

    // effect_mtxの作成部分を置き換えるのでフラグを立てておく。
    // そうすると、Timing BからTiming Cまでの間にあるライブラリ内の処理をスキップできる。
    rs->flag |= NNS_G3D_RSFLAG_SKIP;
}


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
    NNSG3dResMdl*    model;
    int state = 0;
    u16 sincos = 0;
    fx32 X = -FX32_ONE;

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

    model = LoadG3DModel("data/sphere_proj.nsbmd");
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

    G3DDemo_InitCamera(&gCamera, 0, 5*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ProjMap");

    //
    // effect_mtxの設定をカスタマイズする場合は、
    // NNS_G3D_SBC_ENVMAPにタイミングＢで
    // コールバック処理を入れる。
    //
    NNS_G3dRenderObjSetCallBack(&object,
                                &rotProjMap,
                                NULL,
                                NNS_G3D_SBC_PRJMAP,
                                NNS_G3D_SBC_CALLBACK_TIMING_B);

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

        {
            //
            // オブジェクトは常に回転している
            //
            MtxFx33 mRot;
            VecFx32 mTrans;
            sincos += 256;

            mTrans.x = X;
            mTrans.y = 0;
            mTrans.z = 0;
            
            MTX_RotX33(&mRot, FX_SinIdx(sincos), FX_CosIdx(sincos));
            NNS_G3dGlbSetBaseRot(&mRot);
            NNS_G3dGlbSetBaseTrans(&mTrans);

            X += FX32_ONE >> 8;
            if (X >= FX32_ONE)
                X = -FX32_ONE;
            
            //
            // G3dGlbXXXXで設定した状態をまとめて転送します。
            // カメラや射影行列等が設定されています。
            //
            if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
            {
                state = (state + 1) % 3;
            }

            //
            // Aボタンを押すことにより射影行列のセットアップ方法が切り替わる
            // (出てくる絵は同じ)。
            //
            switch(state)
            {
            case 0:
                NNS_G3dGlbFlushP();
                break;
            case 1:
                NNS_G3dGlbFlushVP();
                break;
            case 2:
                NNS_G3dGlbFlushWVP();
                break;
            };

            time = OS_GetTick();

            {
                //
                // G3D: NNS_G3dDraw
                // セットアップされたNNSG3dRenderObjを渡すことにより
                // あらゆる場合の描画を行う
                //
                NNS_G3dDraw(&object);
            }

            // 時間測定。
            time = OS_GetTick() - time;
        }

        {
            // 回転を元に戻す
            MtxFx33 mRot;
            VecFx32 mTrans = {0,0,0};
            MTX_Identity33(&mRot);
            NNS_G3dGlbSetBaseRot(&mRot);
            NNS_G3dGlbSetBaseTrans(&mTrans);

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

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06ld usec", OS_TicksToMicroSeconds(time));
        switch(state)
        {
        case 0:
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_YELLOW, "NNS_G3dGlbFlushP  ");
            break;
        case 1:
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_YELLOW, "NNS_G3dGlbFlushVP ");
            break;
        case 2:
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_YELLOW, "NNS_G3dGlbFlushWVP");
            break;
        };
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
  Name:         LoadG3DModel

  Description:  ファイルからモデルデータを読み込みます。

  Arguments:    path: モデルデータのパス名。

  Returns:      モデルへのポインタ。
   ------------------------------------------------------------------------- */
static NNSG3dResMdl*
LoadG3DModel(const char* path)
{
    NNSG3dResMdl*        model   = NULL;
    NNSG3dResTex*        texture = NULL;
    NNSG3dResFileHeader* resFile = G3DDemo_LoadFile(path);
    BOOL status;
    SDK_NULL_ASSERT(resFile);

    // NNS_G3dResDefaultSetupやNNS_G3dDrawの中でDMA転送を使用するため、
    // これらの関数を呼ぶ前にモデルリソース全体をメモリへストアします。
    // リソースのサイズがある程度大きい場合はDC_StoreAllを呼んだ方が高速です。
    // DC_StoreRangeやDC_StoreAllについてはNITRO-SDK関数リファレンスマニュアルを参照して下さい。
    DC_StoreRange(resFile, resFile->fileSize);

    // デフォルトの初期化関数をコールしてセットアップする
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

  Arguments:    camera:		G3DDemoCamera構造体へのポインタ。

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

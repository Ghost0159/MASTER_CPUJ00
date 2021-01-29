/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - MultiModel
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
// 複数のモデルが1つの.nsbmdファイルに入っている場合のためのサンプル
//
// g3dcvtrで複数の.imdファイルを入力として与えると、複数のモデルを1つの.nsbmdファイル
// にまとめることができます。この際、名前・内容共に同一のテクスチャやパレットは1つに
// まとめられメモリの使用領域を削減することができます。
//
// サンプルの動作
// NNS_G3dGetMdlByIdxでリソース内のモデルセットからインデックスを指定して
// 各モデルリソースのデータを取得します。それぞれのモデルリソースはテクスチャを
// 共有しています(g3dcvtr ./data/two_cubes.nsbmdで確認できます)。
//
// HOWTO
// 1: 例えば、g3dcvtr file1.imd file2.imd ... fileN.imd -o files.nsbmdとすると、
//    file1からfileNのモデルがfiles.nsbmdという１つのファイルにまとめられます。
//    この際、同一名で同一内容のテクスチャ／パレットは１つにまとめられます。
// 2: NNS_G3dResDefaultSetupでテクスチャ／パレットのVRAMへのロードと各モデルリソース
//    へのバインドが行われます。
// 3: NNS_G3dGetMdlByIdx関数でインデックスを指定して各モデルリソースを取得できます。
//    また、NNS_G3D_GET_MDLマクロ(util.hで定義)を使用すると.imdファイル名から
//    拡張子を取り除いた部分を文字列リテラルとして指定できます。
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void InitG3DLib(void);
static void LoadG3DModel(const char* path,NNSG3dResMdl** pModel1,NNSG3dResMdl** pModel2);
static void SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  サンプルのメイン。

  Arguments:    なし。

  Returns:      なし。
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    //  二つのモデルがある
    NNSG3dRenderObj object1,object2;
    NNSG3dResMdl*    model1 = NULL;
    NNSG3dResMdl*    model2 = NULL;

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

    //  一つのバイナリから二つのモデルを取り出す
    LoadG3DModel("data/two_cubes.nsbmd", &model1, &model2);
    SDK_ASSERTMSG(model1, "model1 load failed");
    SDK_ASSERTMSG(model2, "model2 load failed");

    //
    // G3D: NNSG3dRenderObjの初期化
    //
    // NNSG3dRenderObjは１つのモデルとそのモデルに適用されるアニメに関する各種情報を
    // 保持する構造体です。
    // 内部のメンバポインタで指定される領域はユーザー自身で取得・解放する必要があります。
    // ここでmdlはNULLであってもかまいません(NNS_G3dDraw時には設定されている必要がある)。
    //
    NNS_G3dRenderObjInit(&object1, model1);
    NNS_G3dRenderObjInit(&object2, model2);

    G3DDemo_InitCamera(&gCamera, 2*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "MULTI MODEL");

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
            // object1の設定,描画
            //
            VecFx32 scale = {FX32_ONE << 2, FX32_ONE << 2, FX32_ONE << 2};
            VecFx32 trans = {FX32_ONE * 4, 0, 0};

            // スケールをかけて大きくする
            NNS_G3dGlbSetBaseScale(&scale);

            //  少し右に移動
            NNS_G3dGlbSetBaseTrans(&trans);

            //
            // G3D:
            // G3dGlbXXXXで設定した状態をまとめて転送します。
            // カメラや射影行列等が設定されています。
            //
            NNS_G3dGlbFlushP();

            // 時間測定開始
            time = OS_GetTick();
            {
                //
                // G3D: NNS_G3dDraw
                // セットアップされたNNSG3dRenderObjを渡すことにより
                // あらゆる場合の描画を行う
                //
                NNS_G3dDraw(&object1);
            }
            // 時間測定結果
            time = OS_GetTick() - time;
        }

        {
            //
            // object2の設定,描画
            //
            VecFx32 trans = {-FX32_ONE * 4, 0, 0};
            // スケールはobject1の描画時の設定と同じにする
            
            //  少し左に移動
            NNS_G3dGlbSetBaseTrans(&trans);

            //
            // G3D:
            // G3dGlbXXXXで設定した状態をまとめて転送します。
            // カメラや射影行列等が設定されています。
            //
            NNS_G3dGlbFlushP();

            //
            // G3D: NNS_G3dDraw
            // セットアップされたNNSG3dRenderObjを渡すことにより
            // あらゆる場合の描画を行う
            //
            NNS_G3dDraw(&object2);
        }

        {
            // 地面の描画
            // まず、スケールとトランスを元に戻す
            VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
            VecFx32 trans = {0, 0, 0};

            NNS_G3dGlbSetBaseScale(&scale);
            NNS_G3dGlbSetBaseTrans(&trans);
            //
            // G3D: 通常のG3_XXXを呼ぶときは、その前にNNS_G3dGeComFlushBuffer()
            //      をコールして同期しなくてはならない
            //
            NNS_G3dGlbFlushP();
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
  Name:         LoadG3DModel

  Description:  ファイルからモデルデータを読み込みます。

  Arguments:    path: モデルデータのパス名。
                    pModel1,pModel2:モデルを指すポインタのアドレス

  Returns:      なし。
   ------------------------------------------------------------------------- */
static void
LoadG3DModel(const char* path, NNSG3dResMdl** pModel1, NNSG3dResMdl** pModel2)
{
    BOOL status;
    NNSG3dResTex*        texture = NULL;
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
    //  一つのバイナリから二つのモデルを取り出す
    //  サイコロの１の青星と赤丸の部分だけテクスチャが違う
    //  他は共有することでデータ量を抑えている
    *pModel1 = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFile), 0);
    *pModel2 = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFile), 1);
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

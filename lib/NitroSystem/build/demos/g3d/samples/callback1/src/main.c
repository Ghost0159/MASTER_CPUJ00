/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - callback1
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.18 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// コールバックを使用してG3Dの描画動作をカスタマイズするサンプル（その１）
//
// NNS_G3dRenderObjSetCallBack関数を用いると、描画時に実行されるコールバック
// 関数を設定することができます。なお、コールバック関数の引数にもなっている
// NNSG3dRS型の構造体は、描画時のレンダラの状態を記録しています。
// この構造体を操作することにより、G3Dの描画動作をカスタマイズすることが
// できます。
// 
// サンプルの動作
// Xボタンを押すと --> マテリアルによって半透明になったりワイヤーフレームになったりします。
// Yボタンを押すと --> ロボットの胸のあたりが見えなくなります。
// Aボタンを押すと --> ロボットの左腕が2倍に膨れ上がります。
//
// HOWTO:
// 1: NNS_G3dRenderObjSetCallBack関数で、NNSG3dRenderObjに設定するコールバック関数,
//    SBC命令の種類/アドレス, 指定されたSBC命令内の呼び出しタイミングを設定します。
// 2: コールバック関数は、NNSG3dRS構造体へのポインタを引数にとり、返り値はありません。
// 3: コールバック関数内で、NNSG3dRS構造体にアクセスすることにより、G3Dによる描画動作
//    をカスタマイズすることができます。
// 3a: NNS_G3D_SBC_NODEのNNS_G3D_SBC_CALLBACK_TIMING_Bでは、NNSG3dVisAnmResult構造体の
//     内容を操作することで、ノードに所属するシェイプが見えるか見えないか(Visibility)を
//     コントロールすることができます。
// 3b: NNS_G3D_SBC_MATのNNS_G3D_SBC_CALLBACK_TIMING_Bでは、NNSG3dMatAnmResult構造体の
//     内容を操作することで、マテリアルの特性を変更することができます。
// 3c: NNS_G3D_SBC_NODEDESCのNNS_G3D_SBC_CALLBACK_TIMING_Bでは、NNSG3dJntAnmResult構造体の
//     内容を操作することで、ジョイントの位置や向きやスケールを変更することができます。
//
//---------------------------------------------------------------------------


#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void                InitG3DLib(void);
static NNSG3dResMdl*    LoadG3DModel(const char* path);
static void                SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
    eraseWaist

    SBCのNODEコマンド(NNSi_G3dFuncSbc_NODE関数)に入るコールバック。
    NODEコマンドで、タイミングA,タイミングBにおいてコールバックを登録すると
    ビジビリティ計算の結果を作成したり変更したりできる。
    NNS_G3dRSGetVisAnmResult(rs)で、ビジビリティの計算結果が格納されている
    NNSG3dVisAnmResult構造体にアクセスする。

    呼ばれたときの描画ノード名がchestならば表示しない
 ------------------------------------------------------------------------- */
static void
eraseWaist(NNSG3dRS* rs)
{
    int jntID;
    NNS_G3D_GET_JNTID(NNS_G3dRenderObjGetResMdl(NNS_G3dRSGetRenderObj(rs)),
                      &jntID,
                      "chest");
    SDK_ASSERT(jntID >= 0);

    if (NNS_G3dRSGetCurrentNodeID(rs) == jntID)
    {
        NNSG3dVisAnmResult* visResult = NNS_G3dRSGetVisAnmResult(rs);
        visResult->isVisible = FALSE;
        //
        // これ以上コールバックを使用する必要がない場合はリセットしておくと
        // パフォーマンスへの悪影響が若干小さくなる可能性がある。
        //
        NNS_G3dRSResetCallBack(rs, NNS_G3D_SBC_NODE);
    }
}


/* -------------------------------------------------------------------------
    getTranslucent

    SBCのMATコマンド(NNSi_G3dFuncSbc_MAT関数)に入るコールバック。
    MATコマンドで、タイミングA,タイミングBにおいてコールバックを登録すると
    マテリアル計算の結果を作成したり変更したりできる。
    NNS_G3dRSGetMatAnmResult(rs)で、マテリアルの計算結果が格納されている
    NNSG3dMatAnmResult構造体にアクセスする。

    呼ばれたときの描画マテリアル名がlambert2ならば半透明に、
    それ以外ならばワイヤーフレーム表示にする。
 ------------------------------------------------------------------------- */
static void
getTranslucent(NNSG3dRS* rs)
{
    int matID;
    NNSG3dMatAnmResult* matResult;
    NNS_G3D_GET_MATID(NNS_G3dRenderObjGetResMdl(NNS_G3dRSGetRenderObj(rs)),
                      &matID,
                      "lambert2");
    SDK_ASSERT(matID >= 0);

    matResult = NNS_G3dRSGetMatAnmResult(rs);

    if (NNS_G3dRSGetCurrentMatID(rs) == matID)
    {
        matResult->prmPolygonAttr =
            (matResult->prmPolygonAttr & ~REG_G3_POLYGON_ATTR_ALPHA_MASK) |
            (16 << REG_G3_POLYGON_ATTR_ALPHA_SHIFT);
    }
    else
    {
        matResult->flag |= NNS_G3D_MATANM_RESULTFLAG_WIREFRAME;
    }
}


/* -------------------------------------------------------------------------
    scaleArm

    SBCのNODEDESCコマンド(NNSi_G3dFuncSbc_NODEDESC関数)に入るコールバック。
    NODEDESCコマンドで、タイミングA, タイミングBにおいてコールバックを登録する
    と、ジョイント計算の結果を作成したり変更したりできる。
    NNS_G3dRSGetJntAnmResult(rs)で、ジョイントの計算結果が格納されている
    NNSG3dJntAnmResult構造体にアクセスする。

    NODEDESCコマンドの最初の引数がNODEDESCコマンドで計算するジョイントIDになる。
    left_upper_armの計算の際にのみ、スケールを2倍にする。
 ------------------------------------------------------------------------- */
static void
scaleArm(NNSG3dRS* rs)
{
    int jntID;
    
    NNS_G3D_GET_JNTID(NNS_G3dRenderObjGetResMdl(NNS_G3dRSGetRenderObj(rs)),
                      &jntID,
                      "left_upper_arm");
    SDK_ASSERT(jntID >= 0);

    // NODEDESCコマンドの最初の引数を取得
    if (NNS_G3dRSGetCurrentNodeDescID(rs) == jntID)
    {
        NNSG3dJntAnmResult* jntResult;
        jntResult = NNS_G3dRSGetJntAnmResult(rs);

        jntResult->flag &= ~NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE;
        jntResult->scale.x = 2 * FX32_ONE;
        jntResult->scale.y = 2 * FX32_ONE;
        jntResult->scale.z = 2 * FX32_ONE;

        //
        // これ以上コールバックを使用する必要がない場合はリセットしておくと
        // パフォーマンスへの悪影響が若干小さくなる可能性がある。
        //
        NNS_G3dRSResetCallBack(rs, NNS_G3D_SBC_NODEDESC);
    }
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

    model = LoadG3DModel("data/robot_t.nsbmd");
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
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "callback1");

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
            VecFx32 scale = {FX32_ONE>>2, FX32_ONE>>2, FX32_ONE>>2};
            NNS_G3dGlbSetBaseScale(&scale);

            //
            // 押すボタンによって設定するコールバック関数,コールバックを入れるSBCコマンド,
            // コマンド内タイミングを設定する。
            //
            if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_Y))
            {
                // Yボタンを押すとwaistノードが見えなくなる
                // NODEコマンドのタイミングBでコールバックを呼ぶ
                NNS_G3dRenderObjSetCallBack(&object,
                                            &eraseWaist,
                                            NULL,
                                            NNS_G3D_SBC_NODE,
                                            NNS_G3D_SBC_CALLBACK_TIMING_B);
            }
            else if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_X))
            {
                // Xボタンを押すと、マテリアルが半透明とワイヤーフレームになる
                // MATコマンドのタイミングBでコールバックを呼ぶ
                NNS_G3dRenderObjSetCallBack(&object,
                                            &getTranslucent,
                                            NULL,
                                            NNS_G3D_SBC_MAT,
                                            NNS_G3D_SBC_CALLBACK_TIMING_B);
            }
            else if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_A))
            {
                // Aボタンを押すと左腕が2倍に膨れ上がる
                // NODEDESCコマンドのタイミングBでコールバックを呼ぶ
                NNS_G3dRenderObjSetCallBack(&object,
                                            &scaleArm,
                                            NULL,
                                            NNS_G3D_SBC_NODEDESC,
                                            NNS_G3D_SBC_CALLBACK_TIMING_B);
            }
            else
            {
                // 何も押さない場合はコールバック設定をリセット
                NNS_G3dRenderObjResetCallBack(&object);
            }

            //
            // G3dGlbXXXXで設定した状態をまとめて転送します。
            // カメラや射影行列等が設定されています。
            //
            NNS_G3dGlbFlushP();


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
            // スケールを元に戻す
            VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
            NNS_G3dGlbSetBaseScale(&scale);

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

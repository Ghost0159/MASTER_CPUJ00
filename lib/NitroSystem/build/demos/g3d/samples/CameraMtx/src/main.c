/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - CameraMtx
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
// Projection行列とPosition/Vector行列のセットアップ方法についてのサンプル
//
// G3Dでは、NNS_G3dDrawでモデルを描画する前に、NNS_G3dGlbFlushP, NNS_G3dGlbFlushVP
// NNS_G3dGlbFlushWVPによって、Projection行列、Position/Vector行列のカレント行列等
// をセットアップするのが普通です。
// 上記３つの関数の違いは、Projection行列、Position/Vector行列に何をセットするかになります。
// NNS_G3dGlbFlushP関数では、Projection行列に射影変換行列が、Position/Vector行列に
// カメラ行列とオブジェクトのBaseSRT行列が合成されたものがセットされます。
// NNS_G3dGlbFlushVP関数では、Projection行列に射影変換行列とカメラ行列が合成されたものが、
// Position/Vector行列にオブジェクトのBaseSRT行列がセットされます。
// NNS_G3dGlbFlushWVP関数では、Projection行列に射影変換行列とカメラ行列と
// オブジェクトのBaseSRT行列が合成されたものが、Position/Vector行列には
// 単位行列がセットされます。
//
// このように複数の行列セットアップ方法をサポートしているのは以下の２つの理由からです。
// 1: Position/Vector行列を取り出して何らかの加工を行いたい場合、カメラ空間
//    での座標が欲しい場合、ワールド座標が欲しい場合、モデル(ジョイント)の
//    ローカル座標が欲しい場合があります。あらかじめ適切な関数でセットアップを
//    行っておけば、行列の乗算なしにこれらの値を得ることが可能です。
// 2: モデル(ジョイント)のローカル座標があらかじめ計算してある場合、
//    Position/Vector行列のスタックに計算済みの行列をセットしておけば、
//    Projection行列を変更するだけで、オブジェクト全体の位置やカメラの
//    位置を変更することができます。
//
// サンプルの動作
// Aボタンを押すことで、オブジェクトの描画前に呼ぶ関数を、
// NNS_G3dGlbFlushP / NNS_G3dGlbFlushVP / NNS_G3dGlbFlushWVP
// の中から選択します。なお、
// P: Projection
// V: View
// W: World
// の略です。
//
// HOWTO:
// オブジェクトの描画前にNNS_G3dGlbFlushP / NNS_G3dGlbFlushVP / NNS_G3dGlbFlushWVP関数
// をコールします。
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  サンプルのメイン。

  Arguments:    なし。

  Returns:      なし。
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    enum
    {
        Enum_NNS_G3dGlbFlushP   = 0,
        Enum_NNS_G3dGlbFlushVP  = 1,
        Enum_NNS_G3dGlbFlushWVP = 2
    };

    //  どっちの関数を使っているか
    int cameraFlag = Enum_NNS_G3dGlbFlushP;

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

    model = LoadG3DModel("data/billboard.nsbmd");
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

    G3DDemo_InitCamera(&gCamera, 4 * FX32_ONE, 16 * FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5 * FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "CameraMtx");
    G3DDemo_Print(0,2, G3DDEMO_COLOR_WHITE, "change:A");

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
            VecFx32 scale = {FX32_ONE << 2, FX32_ONE << 2, FX32_ONE << 2};
            NNS_G3dGlbSetBaseScale(&scale);

            //
            // G3D:
            // G3dGlbXXXXで設定した状態をまとめて転送します。
            // カメラや射影行列等が設定されています。
            //

            // NNS_G3dGlbFlushAltを用いた場合、カレントProjection行列に射影変換行列・カメラ行列・ベースSRT
            // がかけられるようになる。Position/Vector行列(スタック)には、ローカル変換の行列のみが格納されることになる。
            // そのため、カレントProjection行列を変更することで、Position/Vector行列(スタック)を計算しなおさなくても
            // 同じポーズをとったモデルを複数描画することができる(行列スタック内に計算結果が残っている必要はある)。
            switch(cameraFlag) {
            case Enum_NNS_G3dGlbFlushP:
                //  Projection行列にカメラ行列はかけられない
                NNS_G3dGlbFlushP();
                break;
            case Enum_NNS_G3dGlbFlushVP:
                //
                NNS_G3dGlbFlushVP();
                break;
            case Enum_NNS_G3dGlbFlushWVP:
                //  Projection行列にカメラ行列がかけられます
                NNS_G3dGlbFlushWVP();
                break;
            }

            //  時間測定開始
            time = OS_GetTick();
            {
                //
                // G3D: NNS_G3dDraw
                // セットアップされたNNSG3dRenderObjを渡すことにより
                // あらゆる場合の描画を行う
                // NNS_G3dGlbFlush(),NNS_G3dGlbFlushAlt()のどちらを使っても出てくる絵は同じ
                NNS_G3dDraw(&object);
            }
            // 時間測定結果
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

        //  Ａボタンを押すたびにcameraFlagを切り替える
        if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A)) {
            cameraFlag++;
            cameraFlag = cameraFlag % 3;
        }
        
        //   今,どの関数を呼んでいるかを表示する
        switch(cameraFlag)
        {
        case Enum_NNS_G3dGlbFlushP:
            G3DDemo_Printf(0,4, G3DDEMO_COLOR_GREEN, "G3dGlbFlushP  ");
            break;
        case Enum_NNS_G3dGlbFlushVP:
            G3DDemo_Printf(0,4, G3DDEMO_COLOR_GREEN, "G3dGlbFlushVP ");
            break;
        case Enum_NNS_G3dGlbFlushWVP:
            G3DDemo_Printf(0,4, G3DDEMO_COLOR_GREEN, "G3dGlbFlushWVP");
            break;
        }
        
        //   今のカレントProjection行列とカレントPos/Vector行列の中身が何かを表示する
        switch(cameraFlag)
        {
        case Enum_NNS_G3dGlbFlushP:
            G3DDemo_Printf(0,6, G3DDEMO_COLOR_WHITE, "Projection Mtx:");
            G3DDemo_Printf(2,7, G3DDEMO_COLOR_MAGENTA, "Pj                   ");
            G3DDemo_Printf(0,8, G3DDEMO_COLOR_WHITE, "Pos/Vector Mtx:");
            G3DDemo_Printf(2,9, G3DDEMO_COLOR_MAGENTA, "Camera * BaseSRT ");
            break;
        case Enum_NNS_G3dGlbFlushVP:
            G3DDemo_Printf(0,6, G3DDEMO_COLOR_WHITE, "Projection Mtx:");
            G3DDemo_Printf(2,7, G3DDEMO_COLOR_MAGENTA, "Pj * Camera          ");
            G3DDemo_Printf(0,8, G3DDEMO_COLOR_WHITE, "Pos/Vector Mtx:");
            G3DDemo_Printf(2,9, G3DDEMO_COLOR_MAGENTA, "BaseSRT              ");
            break;
        case Enum_NNS_G3dGlbFlushWVP:
            G3DDemo_Printf(0,6, G3DDEMO_COLOR_WHITE, "Projection Mtx:");
            G3DDemo_Printf(2,7, G3DDEMO_COLOR_MAGENTA, "Pj * Camera * BaseSRT");
            G3DDemo_Printf(0,8, G3DDEMO_COLOR_WHITE, "Pos/Vector Mtx:");
            G3DDemo_Printf(2,9, G3DDEMO_COLOR_MAGENTA, "Identity Mtx         ");
            break;
        }
        
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
    NNS_G3D_NULL_ASSERT(resFile);

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

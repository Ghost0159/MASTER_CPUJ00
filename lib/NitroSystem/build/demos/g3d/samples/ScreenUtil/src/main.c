/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ScreenUtil
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// G3Dの座標変換ユーティリティAPIを使ったサンプル
//
// BGスクリーン座標との座標変換を行うことで、ワールド座標がBGスクリーン上の
// どの点に相当するのか、BGスクリーン座標上の点がワールド座標のどの線に相当
// するのかを求めることができます。
//
// サンプルの動作
// ワールド座標系の原点に対応するBGスクリーン上の座標を表示し、
// 走る人の各ジョイントの位置に対応するBGスクリーン上の座標を表示します。
// タッチパネルを触ると、触った位置のBGスクリーン上の座標と、
// それに相当するNearプレーン上の点とFarプレーン上の点を
// ワールド座標系で表示します。
//
// HOWTO:
// 1: NNS_G3dWorldPosToScrPos関数を使用するとワールド座標系の点をBGスクリーン
//    座標系の点に変換することができます。
// 2: NNS_G3dLocalOriginToScrPos関数を使用するとローカル座標系の原点を
//    BGスクリーン座標系の点に変換することができます。
// 3: NNS_G3dScrPosToWorldLine関数を使用するとBGスクリーン座標系の点を
//    ワールド座標系の線に変換することができます。線は2点として返され、
//    それぞれ、NEARクリップ面, FARクリップ面上の点になります。
// 
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


#define NUMJNT 24
struct
{
    BOOL isOnScreen;
    int  x;
    int  y;
}
ScrPosJnt[NUMJNT];

//
// 各ジョイントのBGスクリーン上の位置を取得
//
static void
getScrPos(NNSG3dRS* rs)
{
    int jntID;
    int x, y, r;

    jntID = NNS_G3dRSGetCurrentNodeDescID(rs);
    r = NNS_G3dLocalOriginToScrPos(&x, &y);
    ScrPosJnt[jntID].isOnScreen = (!r);
    ScrPosJnt[jntID].x = x;
    ScrPosJnt[jntID].y = y;
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
    NNSG3dResMdl*   pModel;
    void*           pAnmRes;
    NNSFndAllocator allocator;
    NNSG3dAnmObj*   pAnmObj;

    G3DDemo_InitSystem();
    G3DDemo_InitMemory();
    G3DDemo_InitVRAM();

    InitG3DLib();
    G3DDemo_InitDisplay();

    //
    // このサンプルではタッチパネルを使用するためタッチパネルの初期化を行う
    //
    {
        BOOL result;
        TPCalibrateParam calibrate;

        // タッチパネルAPIのイニシャライズ
        TP_Init();

        // キャリブレーションのパラメータをフラッシュメモリから取ってくる
        result = TP_GetUserInfo( &calibrate );

        if (!result)
        {
            // 有効な値が入っていなかったらユーザーにキャリブレートを
            // お願いしてプログラムを終了
            OS_Printf("Calibration please\n");
            return;
        }

        // キャリブレーションのパラメータをタッチパネルAPIにセット
        TP_SetCalibrateParam( &calibrate );
    }

    //
    // 4バイトアラインメントでアロケータを作成
    //
    NNS_FndInitAllocatorForExpHeap(&allocator, G3DDemo_AppHeap, 4);

    // マネージャがテクスチャイメージスロットを4スロット分管理できるようにして
    // デフォルトマネージャにする。
    NNS_GfdInitFrmTexVramManager(4, TRUE);

    // マネージャがパレットを32KB分管理できるようにして
    // デフォルトマネージャにする。
    NNS_GfdInitFrmPlttVramManager(0x8000, TRUE);

    pModel = LoadG3DModel("data/human_run_t.nsbmd");
    SDK_ASSERTMSG(pModel, "load failed");

    //
    // G3D: NNSG3dRenderObjの初期化
    //
    // NNSG3dRenderObjは１つのモデルとそのモデルに適用されるアニメに関する各種情報を
    // 保持する構造体です。
    // 内部のメンバポインタで指定される領域はユーザー自身で取得・解放する必要があります。
    // ここでmdlはNULLであってもかまいません(NNS_G3dDraw時には設定されている必要がある)。
    //
    NNS_G3dRenderObjInit(&object, pModel);
    
    //
    // ジョイントアニメを読み込んで関連付けしてみる
    //
    {
        const u8* pFile = G3DDemo_LoadFile("data/human_run_t.nsbca");
        SDK_ASSERTMSG( pFile, "anm load failed" );
        {
            // アニメーションの初期化、レンダリングオブジェクトへの追加
            // 
            // 読み込んだアニメーションファイルとNNSG3dRenderObjの仲介をする
            // アニメーションオブジェクト(NNSG3dAnmObj)を確保・初期化する。
            // 全てのアニメーションが同様のコードでセットアップされる。
            //

            //
            // アニメーションオブジェクトに必要なメモリをallocatorから割り当てる。
            // モデルリソースとアニメーションリソースを指定して割り当てる必要がある。
            //

            // インデックス＃０のアニメーションを指定
            pAnmRes = NNS_G3dGetAnmByIdx(pFile, 0);
            SDK_NULL_ASSERT(pAnmRes);

            // 必要量のメモリをアロケートする。イニシャライズは別途必要になる。
            pAnmObj = NNS_G3dAllocAnmObj(&allocator, // 使用するアロケータを指定
                                         pAnmRes,    // アニメーションリソースを指定
                                         pModel);    // モデルリソースを指定
            SDK_NULL_ASSERT(pAnmObj);

            //
            // AnmObj を初期化する。ジョイントアニメーション以外も同様
            //
            NNS_G3dAnmObjInit(pAnmObj, // アニメーションオブジェクトへのポインタ
                              pAnmRes, // アニメリソースへのポインタ
                              pModel,  // NNSG3dResMdlへのポインタ
                              NULL );  // NNSG3dResTexへのポインタ(テクスチャパターンアニメ以外ならばNULLでもよい)

            //
            // RenderObj に AnmObj を追加する。ジョイントアニメーション以外も同様
            //
            NNS_G3dRenderObjAddAnmObj( &object, pAnmObj );

            //
            // コールバックを設定する
            //
            NNS_G3dRenderObjSetCallBack(&object,
                                        &getScrPos,
                                        NULL,
                                        NNS_G3D_SBC_NODEDESC,
                                        NNS_G3D_SBC_CALLBACK_TIMING_C);
        }
    }
    
    //
    // デモ共通の初期化
    //
    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ScreenUtil");

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

        //
        // アニメフレームを更新する
        //
        pAnmObj->frame += FX32_ONE;
        if( pAnmObj->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObj))
        {
            pAnmObj->frame = 0;
        }

        {
            VecFx32 scale = { FX32_ONE>>2, FX32_ONE>>2, FX32_ONE>>2 };
            NNS_G3dGlbSetBaseScale(&scale);
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
            // 時間測定結果
            time = OS_GetTick() - time;
        }

        {
            VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
            NNS_G3dGlbSetBaseScale(&scale);
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

        {
            int x, y, r;
            VecFx32 worldOrig = {0, 0, 0};
            r = NNS_G3dWorldPosToScrPos(&worldOrig, &x, &y);
            if (!r)
            {
                G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "World Origin -> (%3d,%3d)", x, y);
            }
            else
            {
                G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "World Origin -> ---------");
            }
        }

        {
            TPData tp;
            if (!TP_RequestCalibratedSampling(&tp) &&
                tp.touch                           &&
                tp.validity == TP_VALIDITY_VALID)
            {
                VecFx32 near, far;
                int x, y;
                x = tp.x;
                y = tp.y;
                G3DDemo_Printf(0, 5, G3DDEMO_COLOR_GREEN, "Touch = (%3d, %3d)", x, y);

                (void)NNS_G3dScrPosToWorldLine(x, y, &near, &far);
                G3DDemo_Printf(0, 6, G3DDEMO_COLOR_GREEN, "Point on near plane(World)");
                G3DDemo_Printf(0, 7, G3DDEMO_COLOR_GREEN, "  (%7.2f, %7.2f, %7.2f)", near.x / 4096.f, near.y / 4096.f, near.z / 4096.f);
                
                G3DDemo_Printf(0, 8, G3DDEMO_COLOR_GREEN, "Point on far plane(World)");
                G3DDemo_Printf(0, 9, G3DDEMO_COLOR_GREEN, "  (%7.2f, %7.2f, %7.2f)", far.x / 4096.f, far.y / 4096.f, far.z / 4096.f);
            }
            else
            {
                G3DDemo_Printf(0, 5, G3DDEMO_COLOR_GREEN, "No touch          ");
                G3DDemo_Printf(0, 6, G3DDEMO_COLOR_GREEN, "                          ");
                G3DDemo_Printf(0, 7, G3DDEMO_COLOR_GREEN, "                                ");
                G3DDemo_Printf(0, 8, G3DDEMO_COLOR_GREEN, "                          ");
                G3DDemo_Printf(0, 9, G3DDEMO_COLOR_GREEN, "                                ");
            }
        }

        {
            //
            // 各ジョイントのスクリーン上の位置を表示
            //
            int i;
            for (i = 0; i < NUMJNT / 2; i++)
            {
                if (ScrPosJnt[2*i].isOnScreen)
                    G3DDemo_Printf( 0, i + 11, G3DDEMO_COLOR_GREEN, "Jnt%02d (%3d,%3d)", 2*i, ScrPosJnt[2*i].x, ScrPosJnt[2*i].y);
                else
                    G3DDemo_Printf( 0, i + 11, G3DDEMO_COLOR_GREEN, "Jnt%02d ---------", 2*i);

                if (ScrPosJnt[2*i+1].isOnScreen)
                    G3DDemo_Printf(16, i + 11, G3DDEMO_COLOR_GREEN, "Jnt%02d (%3d,%3d)", 2*i+1, ScrPosJnt[2*i+1].x, ScrPosJnt[2*i+1].y);
                else
                    G3DDemo_Printf(16, i + 11, G3DDEMO_COLOR_GREEN, "Jnt%02d ---------", 2*i+1);
            }
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

    SDK_NULL_ASSERT(resFile);

    // NNS_G3dResDefaultSetupやNNS_G3dDrawの中でDMA転送を使用するため、
    // これらの関数を呼ぶ前にモデルリソース全体をメモリへストアします。
    // リソースのサイズがある程度大きい場合はDC_StoreAllを呼んだ方が高速です。
    // DC_StoreRangeやDC_StoreAllについてはNITRO-SDK関数リファレンスマニュアルを参照して下さい。
    DC_StoreRange(resFile, resFile->fileSize);

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

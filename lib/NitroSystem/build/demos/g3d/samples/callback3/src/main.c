/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - callback3
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
// コールバックを使用してG3Dの描画動作をカスタマイズするサンプル（その３）
//
// NNS_G3dRenderObjSetCallBack関数を用いると、描画時に実行されるコールバック
// 関数を設定することができます。なお、コールバック関数の引数にもなっている
// NNSG3dRS型の構造体は、描画時のレンダラの状態を記録しています。
// この構造体を操作することにより、G3Dの描画動作をカスタマイズすることが
// できます。
//
// サンプルの動作
// 走る人の描画と、それとは全く無関係な行列の計算を行います。
// ディスプレイリストをジオメトリエンジンに送信している間に、
// コールバック関数内で、行列を計算を進めています。
// Aボタンでディスプレイリストの送信中に計算を行うか行わないかを
// 切り替えることができます。
//
// HOWTO:
// 1: 走る人のレンダリングオブジェクトにコールバック関数を設定します。
// 2: コールバック関数は、NNS_G3D_SBC_SHPのNNS_G3D_SBC_CALLBACK_TIMING_Cで
//    呼び出されるようにします。この場所でコールした場合、ディスプレイリストの
//    送信直後にコールバック関数に制御が移ります。
// 3: コールバック関数内で、ディスプレイリストの送信中に描画とは関係のない計算を
//    行うことができます。
// 
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);

#define DUMMY_MTX_SIZE 60
static int idxMtx;
static MtxFx43 mtx;
static MtxFx43 mtxArray[DUMMY_MTX_SIZE];

/* -------------------------------------------------------------------------
  dummyCalc

  ディスプレイリストの転送中に何か別の計算をさせる。
 ------------------------------------------------------------------------- */
static void
dummyCalc(NNSG3dRS* rs)
{
    #pragma unused(rs)

    while(NNS_G3dGeIsSendDLBusy() && idxMtx < DUMMY_MTX_SIZE)
    {
        MTX_Concat43(&mtx, &mtxArray[idxMtx], &mtxArray[idxMtx]);
        ++idxMtx;
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
    NNSG3dResMdl*   pModel;
    void*           pAnmRes;
    NNSFndAllocator allocator;
    NNSG3dAnmObj*   pAnmObj;
    BOOL callBackOn;

    G3DDemo_InitSystem();
    G3DDemo_InitMemory();
    G3DDemo_InitVRAM();

    InitG3DLib();
    G3DDemo_InitDisplay();

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
    // NNSG3dRenderObjの初期化
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
            // SBCのSHPコマンドに対してコールバックを設定
            //
            callBackOn = TRUE;
            NNS_G3dRenderObjSetCallBack(&object,
                                        &dummyCalc,
                                        NULL,
                                        NNS_G3D_SBC_SHP,
                                        NNS_G3D_SBC_CALLBACK_TIMING_C);
        }
    }
    
    //
    // デモ共通の初期化
    //
    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "callback3");

    {
        int i;
        MTX_Identity43(&mtx);
        for (i = 0; i < DUMMY_MTX_SIZE; ++i)
        {
            MTX_Identity43(&mtxArray[i]);
        }
    }

    for(;;)
    {
        OSTick time;
        int cntMtxConcat;

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

        //
        // Aボタンでコールバックを使用するかどうかを切り替える
        //
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            if (callBackOn)
            {
                callBackOn = FALSE;
                NNS_G3dRenderObjResetCallBack(&object);
            }
            else
            {
                callBackOn = TRUE;
                NNS_G3dRenderObjSetCallBack(&object,
                                            &dummyCalc,
                                            NULL,
                                            NNS_G3D_SBC_SHP,
                                            NNS_G3D_SBC_CALLBACK_TIMING_C);
            }
        }

        if (callBackOn)
        {
            G3DDemo_Print(0,1, G3DDEMO_COLOR_GREEN, "CallBack ON ");
        }
        else
        {
            G3DDemo_Print(0,1, G3DDEMO_COLOR_GREEN, "CallBack OFF");
        }

        {
            VecFx32 scale = { FX32_ONE>>2, FX32_ONE>>2, FX32_ONE>>2 };
            NNS_G3dGlbSetBaseScale(&scale);
            NNS_G3dGlbFlushP();
            idxMtx = 0;
            
            //  時間測定開始
            time = OS_GetTick();
            {
                NNS_G3dDraw(&object);
            }

            cntMtxConcat = idxMtx;

            while(idxMtx < DUMMY_MTX_SIZE)
            {
                MTX_Concat43(&mtx, &mtxArray[idxMtx], &mtxArray[idxMtx]);
                ++idxMtx;
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

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06ld usec", OS_TicksToMicroSeconds(time));
        G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "MTX_CONCAT count(NNS_G3dDraw)");
        G3DDemo_Printf(0, 3, G3DDEMO_COLOR_GREEN, "= %3d", cntMtxConcat);
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

   Arguments:    camera:        G3DDemoCamera構造体へのポインタ。

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

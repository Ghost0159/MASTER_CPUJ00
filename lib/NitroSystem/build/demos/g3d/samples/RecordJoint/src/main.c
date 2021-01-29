/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - RecordJoint
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
// ジョイントの計算結果を前計算しておくサンプル
//
// ジョイントの計算結果をバッファしておくことで、ジョイントが不変の場合にCPUの
// 計算コストを節約することができます。
//
// サンプルの動作
// ビジビリティアニメーションが繰り返し再生されます。
// Aボタンを押すとバッファされたジョイント計算結果を使用するか、
// 描画毎に計算しなおすかが切り替わります。
//
// HOWTO:
// 1: NNS_G3dAllocRecBufferJntでバッファの領域を確保します。
// 2: NNS_G3dRenderObjSetJntAnmBufferでバッファをレンダリングオブジェクトに
//    アタッチします。
// 3: NNS_G3dRenderObjSetFlagでNNS_G3D_RENDEROBJ_FLAG_RECORDを立てます。
//    このフラグを立ててNNS_G3dDrawをコールした場合、バッファに計算結果が格納されます。
// 4: また、NNS_G3D_RENDEROBJ_FLAG_NOGECMDを立てておくとNNS_G3dDrawの実行時に
//    ジオメトリエンジンにデータが送信されません。
// 5: NNS_G3dDrawを実行して、バッファにデータを格納します。
// 6: NNS_G3dRenderObjResetFlagでNNS_G3D_RENDEROBJ_FLAG_RECORDと(NNS_G3D_RENDEROBJ_FLAG_NOGECMD)
//    をリセットします。
// 7: 次回のNNS_G3dDrawからジョイントデータを計算する代わりにバッファのデータを参照して描画します。
// 8: NNS_G3dRenderObjReleaseJntAnmBufferでバッファを取り外すことができます。
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
    NNSG3dRenderObj object;
    NNSG3dResMdl*   pModel;
    void*           pAnmRes;
    NNSFndAllocator allocator;
    NNSG3dAnmObj*   pAnmObj;
    NNSG3dJntAnmResult* pBufJntAnmResult;
    BOOL useRecord = TRUE;

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

    pModel = LoadG3DModel("data/brother.nsbmd");
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
    // ビジビリティアニメを読み込んで関連付けしてみる
    //
    {
        const u8* pFile = G3DDemo_LoadFile("data/brother.nsbva");
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
            // AnmObj を初期化する。
            //
            NNS_G3dAnmObjInit(pAnmObj, // アニメーションオブジェクトへのポインタ
                              pAnmRes, // アニメリソースへのポインタ
                              pModel,  // NNSG3dResMdlへのポインタ
                              NULL );  // NNSG3dResTexへのポインタ(テクスチャパターンアニメ以外ならばNULLでもよい)

            //
            // RenderObj に AnmObj を追加する。
            //
            NNS_G3dRenderObjAddAnmObj( &object, pAnmObj );
        }

        {
            //
            // ジョイント計算結果のバッファを確保し、レンダリングオブジェクトにつける
            //
            pBufJntAnmResult = NNS_G3dAllocRecBufferJnt(&allocator, pModel);
            NNS_G3dRenderObjSetJntAnmBuffer(&object, pBufJntAnmResult);

            //
            // ジョイントの計算結果をバッファに記録する。
            //
            NNS_G3dRenderObjSetFlag(&object, NNS_G3D_RENDEROBJ_FLAG_NOGECMD); // ジオメトリエンジンに送信しない
            NNS_G3dRenderObjSetFlag(&object, NNS_G3D_RENDEROBJ_FLAG_RECORD);  // pBufJntAnmResultにデータを記録

            NNS_G3dDraw(&object);

            NNS_G3dRenderObjResetFlag(&object, NNS_G3D_RENDEROBJ_FLAG_NOGECMD); // ジオメトリエンジンに送信する
            NNS_G3dRenderObjResetFlag(&object, NNS_G3D_RENDEROBJ_FLAG_RECORD);  // pBufJntAnmResultのデータを使用
        }

    }
    
    //
    // デモ共通の初期化
    //
    G3DDemo_InitCamera(&gCamera, 0, 5*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "RecordJoint");

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

        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            //
            // Aボタンを押すと切り替わる
            //
            if (useRecord)
            {
                useRecord = FALSE;
                (void)NNS_G3dRenderObjReleaseJntAnmBuffer(&object);
            }
            else
            {
                useRecord = TRUE;
                NNS_G3dRenderObjSetJntAnmBuffer(&object, pBufJntAnmResult);
            }
        }

        if (useRecord)
        {
            G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "use recorded joint data    ");
        }
        else
        {
            G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "calc joint data every frame");
        }

        {
            VecFx32 scale = { FX32_ONE>>1, FX32_ONE>>1, FX32_ONE>>1 };
            NNS_G3dGlbSetBaseScale(&scale);
            //
            // G3D:
            // G3dGlbXXXXで設定した状態をまとめて転送します。
            // カメラや射影行列等が設定されています。
            //
            NNS_G3dGlbFlushP();

            //  時間測定開始
            time = OS_GetTick();
            //
            // G3D: NNS_G3dDraw
            // セットアップされたNNSG3dRenderObjを渡すことにより
            // あらゆる場合の描画を行う
            //
            NNS_G3dDraw(&object);

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

        //
        // 時間測定結果画面表示。
        //
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

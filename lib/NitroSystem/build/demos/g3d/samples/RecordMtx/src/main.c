/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - RecordMtx
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// callback2サンプルと同様の動作をコールバックを使用せずに実現するサンプル
//
// g3dcvtrの-sオプションを使用すると、計算した行列を必ず行列スタックに格納する
// ようにすることができます。このようにすると、描画後にスタックに格納されて
// いる行列をカレント行列にリストアすることにより、必要なジョイントの行列を
// 得ることができます。
//
// サンプルの動作
// 人が歩くロボットを左手で持って走っています。callback2サンプルと同様の動作をします。
//
// HOWTO:
// 1: .imdファイルを-sオプションをつけてコンバートします。
// 2: NNS_G3dDrawで走る人を描画します。
// 3: NNS_G3dGetResultMtxで必要なジョイントのjntIDを指定して、
//    行列をカレント行列にリストアします。
// 4: NNS_G3dDraw関数でロボットを描画すると、走る人の左手がロボットを持っている
//    (ロボットがくっついている）ようになります。
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
    NNSG3dRenderObj object2;
    NNSG3dResMdl*   pModel;
    void*           pAnmRes;
    NNSG3dResMdl*   pModel2;
    void*           pAnmRes2;
    NNSFndAllocator allocator;
    NNSG3dAnmObj*   pAnmObj;
    NNSG3dAnmObj*   pAnmObj2;

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

    pModel2 = LoadG3DModel("data/robot_t.nsbmd");
    SDK_ASSERTMSG(pModel2, "load failed");

    //
    // G3D: NNSG3dRenderObjの初期化
    //
    // NNSG3dRenderObjは１つのモデルとそのモデルに適用されるアニメに関する各種情報を
    // 保持する構造体です。
    // 内部のメンバポインタで指定される領域はユーザー自身で取得・解放する必要があります。
    // ここでmdlはNULLであってもかまいません(NNS_G3dDraw時には設定されている必要がある)。
    //
    NNS_G3dRenderObjInit(&object, pModel);
    NNS_G3dRenderObjInit(&object2, pModel2);
    
    //
    // ジョイントアニメを読み込んで関連付けしてみる
    //
    {
        //
        // 走る人
        //
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
        }
    }

    {
        //
        // 歩くロボット
        //
        const u8* pFile = G3DDemo_LoadFile("data/robot_t.nsbca");
        SDK_ASSERTMSG( pFile, "anm load failed" );

        pAnmRes2 = NNS_G3dGetAnmByIdx(pFile, 0);
        SDK_NULL_ASSERT(pAnmRes2);

        pAnmObj2 = NNS_G3dAllocAnmObj(&allocator,
                                      pAnmRes2,
                                      pModel2);
        SDK_NULL_ASSERT(pAnmObj2);

        NNS_G3dAnmObjInit(pAnmObj2,
                          pAnmRes2,
                          pModel2,
                          NULL);

        NNS_G3dRenderObjAddAnmObj(&object2, pAnmObj2);
    }
    
    //
    // デモ共通の初期化
    //
    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "RecordMtx");

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

        // ロボットを見やすくするために人のアニメーションを遅くしている
        pAnmObj->frame += (FX32_ONE >> 1);
        if( pAnmObj->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObj))
        {
            pAnmObj->frame = 0;
        }

        // ロボットが動いているのが分かるようにロボットのアニメーションを速くしている
        pAnmObj2->frame += FX32_ONE * 2;
        if ( pAnmObj2->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObj2))
        {
            pAnmObj2->frame = 0;
        }

        //  時間測定開始
        time = OS_GetTick();

        {
            //
            // 走る人の描画
            //

            VecFx32 scale = { FX32_ONE>>2, FX32_ONE>>2, FX32_ONE>>2 };
            NNS_G3dGlbSetBaseScale(&scale);
            //
            // G3D:
            // G3dGlbXXXXで設定した状態をまとめて転送します。
            // カメラや射影行列等が設定されています。
            //
            NNS_G3dGlbFlushP();

            {
                //
                // G3D: NNS_G3dDraw
                // セットアップされたNNSG3dRenderObjを渡すことにより
                // あらゆる場合の描画を行う
                //
                NNS_G3dDraw(&object);
            }
        }
 
        {
            //
            // 人の左手に持たれて歩くロボットの描画
            //

            // この場合、NNS_G3dGlbFlushPは呼ばなくてもよい

            //
            // 人の.imdファイルを変換する際に-sオプションをつけているので
            // 描画時に計算された行列は全て行列スタックに格納されています
            // (そのためノードの数は30以下でなくてはコンバートできない)。
            // callback2サンプルとは違い、描画中に行列を取得する必要はなく、
            // そのためコールバックを使う必要がありません。
            //
            int jntID;
            BOOL result;

            // 左手のジョイントIDを取得する。
            NNS_G3D_GET_JNTID(object.resMdl, &jntID, "wrist_l_end");

            //
            // レンダリングオブジェクトのjntIDノードに対応する行列をカレント行列に持ってくる
            // ２番目の引数にMtxFx43へのポインタを指定すると位置座標行列が、
            // ３番目の引数にMtxFx33へのポインタを指定すると方向ベクトル行列が得られる。
            //
            result = NNS_G3dGetResultMtx(&object, NULL, NULL, (u32)jntID);

            // jntIDノードに対応する行列がなかった場合はFALSEが返ってくるので念のためチェック
            SDK_ASSERT(result);

            //
            // 描画直前にしかposScaleでのスケーリングはかからないので、
            // 明示的にスケーリングしなくてはならない。
            //
            NNS_G3dGeScale(object.resMdl->info.posScale, object.resMdl->info.posScale, object.resMdl->info.posScale);

            NNS_G3dDraw(&object2);
        }
        
        
        // 時間測定結果
        time = OS_GetTick() - time;

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

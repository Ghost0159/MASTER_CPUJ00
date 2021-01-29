/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ShadowVolume
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
// シャドウボリューム表示サンプル
//
// G3Dを使用してシャドウボリュームを描画する方法を例示します。
//
// サンプルの動作
// ビジビリティアニメーションが繰り返し再生されているオブジェクトに、
// シャドウボリュームを用いて影を映します。
//
// HOWTO:
// 1: G3_SwapBuffersでマニュアルソートモードを指定していることを確認します。
// 2: 同じモデルを、マスク用シャドウポリゴンのマテリアル設定にして描画し、
//    その後、描画用シャドウポリゴンのマテリアル設定にして描画します。
//    詳しくはサンプル内のDrawShadowVolume関数内のコメントを参照してください。
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
    DrawShadowVolume

    シャドウボリュームを描画する。
 ------------------------------------------------------------------------- */
static void
DrawShadowVolume(NNSG3dResMdl* p, BOOL isShadow)
{
    //
    // シャドウボリュームの描画
    //
    // まず、１つでもシャドウボリュームを描画する場合には、必ずマニュアルソートモード
    // にしてください。
    // つまり、
    // G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W)か
    // G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z)を使用
    //
    // １つのモデルを、マスク用シャドウポリゴンのマテリアル設定にして描画し、
    // 次に同じモデルを描画用シャドウポリゴンのマテリアル設定にして描画すると、
    // 物体に影を落とすことができます。
    // なお、シャドウボリューム用のモデルは、影を映す物体を貫通している必要があります。
    //
    {
        // 位置調整
        VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
        VecFx32 trans = {0, 5 * FX32_ONE, 0};
        NNS_G3dGlbSetBaseScale(&scale);
        NNS_G3dGlbSetBaseTrans(&trans);
        NNS_G3dGlbFlushP();
        trans.y = 0;
        NNS_G3dGlbSetBaseTrans(&trans);
    }

    if (isShadow)
    {
        {
            //
            // マスク用シャドウポリゴンを描画します。
            // ポリゴンIDは0, αは1-30，ポリゴンモードはGX_POLYGONMODE_SHADOW
            // に設定して、裏面だけ(GX_CULL_FRONT)を描画します。
            //
            NNS_G3dMdlSetMdlLightEnableFlag( p, 0, 0 );
            NNS_G3dMdlSetMdlPolygonID( p, 0, 0 );                       // ポリゴンIDは0に設定
            NNS_G3dMdlSetMdlCullMode( p, 0, GX_CULL_FRONT );            // 背面のみを描画
            NNS_G3dMdlSetMdlAlpha( p, 0, 10 );
            NNS_G3dMdlSetMdlPolygonMode( p, 0, GX_POLYGONMODE_SHADOW ); // シャドウポリゴン使用

            // matID=0, shpID=0を描画(マテリアルの送信は省略しない)
            NNS_G3dDraw1Mat1Shp(p, 0, 0, TRUE);
        }

        {
            //
            // 同じ場所に描画用シャドウポリゴンを描画します。
            // ポリゴンIDは1-63，αは1-30，ポリゴンモードはGX_POLYGONMODE_SHADOW
            // に設定して、両面(GX_CULL_NONE)を描画します。
            //
            // GX_CULL_BACKの設定だと、マスク用シャドウポリゴンがはみ出すケースが存在し
            // 正しく描画されない場合があります。
            //
            // ポリゴンIDは、影が表示されるオブジェクトとは違うものにしてください。
            // ポリゴンIDが同じ場合は、影は描画されません。影を落とすオブジェクト自身に
            // 影を表示させたくない場合は、影を落とすオブジェクトと描画用シャドウポリゴンの
            // ポリゴンIDを同じにしてください。
            //
            NNS_G3dMdlSetMdlLightEnableFlag( p, 0, 0 );
            NNS_G3dMdlSetMdlPolygonID( p, 0, 1 );                       // ポリゴンIDは1-63に設定
            NNS_G3dMdlSetMdlCullMode( p, 0, GX_CULL_NONE );             // 両面描画
            NNS_G3dMdlSetMdlAlpha( p, 0, 10 );
            NNS_G3dMdlSetMdlPolygonMode( p, 0, GX_POLYGONMODE_SHADOW ); // シャドウポリゴン使用

            // matID=0, shpID=0を描画(マテリアルの送信は省略しない)
            NNS_G3dDraw1Mat1Shp(p, 0, 0, TRUE);
        }
    }
    else
    {
        //
        // シャドウボリュームとしてではなく普通の半透明ポリゴンとして描画します。
        // このサンプルで使用しているモデルは円筒形です。
        // 
        NNS_G3dMdlSetMdlLightEnableFlag( p, 0, 0 );
        NNS_G3dMdlSetMdlPolygonID( p, 0, 0 );
        NNS_G3dMdlSetMdlCullMode( p, 0, GX_CULL_BACK ); 
        NNS_G3dMdlSetMdlAlpha( p, 0, 10 );
        NNS_G3dMdlSetMdlPolygonMode( p, 0, GX_POLYGONMODE_MODULATE );

        // matID=0, shpID=0を描画(マテリアルの送信は省略しない)
        NNS_G3dDraw1Mat1Shp(p, 0, 0, TRUE);
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
    NNSFndAllocator allocator;
    
    NNSG3dRenderObj object;
    NNSG3dResMdl*   pModel;
    void*           pAnmRes;
    NNSG3dAnmObj*   pAnmObj;
    BOOL isShadow = TRUE;

    //
    // シャドウボリュームは普通１マテリアル１シェイプで構成されるので
    // NNS_G3dDraw1Mat1Shpで描画できます。従って、NNSG3dRenderObjは不要なので
    // NNSG3dResMdlだけをセットアップするだけで十分です。
    //
    NNSG3dResMdl*   pShadow;

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

    pShadow = LoadG3DModel("data/shadow.nsbmd");
    SDK_ASSERTMSG(pShadow, "load failed");

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
    // ビジビリティアニメを読み込んで関連付けする。
    //
    {
        const u8* pFile = G3DDemo_LoadFile("data/brother.nsbva");
        SDK_ASSERTMSG( pFile, "anm load failed" );
        {
            //
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
            // AnmObjを初期化する。
            //
            NNS_G3dAnmObjInit(pAnmObj, // アニメーションオブジェクトへのポインタ
                              pAnmRes, // アニメーションリソースへのポインタ
                              pModel,  // NNSG3dResMdlへのポインタ
                              NULL );  // NNSG3dResTexへのポインタ(テクスチャパターンアニメ以外ならばNULLでもよい)

            //
            // RenderObj に AnmObj を追加する。
            //
            NNS_G3dRenderObjAddAnmObj( &object, pAnmObj );
        }
    }
    
    //
    // デモ共通の初期化
    //
    G3DDemo_InitCamera(&gCamera, 2*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ShadowVolume");

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

        // アニメフレームを更新する
        pAnmObj->frame += FX32_ONE;

        if (pAnmObj->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObj))
        {
            pAnmObj->frame = 0;
        }

        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            isShadow = isShadow ? FALSE : TRUE;
        }

        time = OS_GetTick();
        {
            {
                // スケールをかけて大きくする
                VecFx32 scale = {FX32_ONE << 1, FX32_ONE << 1 , FX32_ONE << 1};
                NNS_G3dGlbSetBaseScale(&scale);

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
                NNS_G3dDraw(&object);
            }

            DrawShadowVolume(pShadow, isShadow);
        }
        time = OS_GetTick() - time;

        {
            //
            // G3D: 通常のG3_XXXを呼ぶときは、その前にNNS_G3dGeComFlushBuffer()
            //      をコールして同期しなくてはならない
            //
            NNS_G3dGlbFlushP();
            NNS_G3dGeFlushBuffer();
            G3DDemo_DrawGround(&gGround);
        }

        //
        // NOTICE:
        // シャドウポリゴンを使用する場合は必ずマニュアルソートモードにしてください
        // オートソートにした場合、半透明ポリゴンの描画順序をユーザーが決定することが
        // できないので、シャドウボリュームを正しく描画することができません。
        //
        G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W);

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

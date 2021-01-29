/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - CustomizeSbcOp
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
// SBCコマンドの処理関数をユーザーがカスタマイズしたものに変更するサンプル
//
// サンプルの動作
// 3枚の正方形が表示されていて、それぞれ、ビルボードなし・ビルボードあり・
// Y軸ビルボードありで表示されています。
// Aボタンを押すことにより、ビルボード処理関数をこのサンプル内で定義したものに置き換える
// ことができます。Bボタンを押すことにより、ビルボードにかかるスケールが２倍になります。
// なお、このサンプル内で定義されているビルボード処理関数はライブラリのものより高速ですが、
// スケールがかかっている場合は正確に表示できません。
//
// HOWTO:
// ・NNS_G3dFuncSbcTable配列のカスタマイズしたいコマンドに対応した場所に
//   ユーザーが定義した関数のポインタを代入する
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


static void MySbcBBSimple(NNSG3dRS* rs, u32 opt);
static void MySbcBBYSimple(NNSG3dRS* rs, u32 opt);


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
    NNSG3dResMdl*   model;
    BOOL            useCustomizedBB = FALSE;
    BOOL            isScaled = FALSE;

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

    G3DDemo_InitCamera(&gCamera, 0 * FX32_ONE, 4 * FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5 * FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "CustomizeSbcOp");
    G3DDemo_Print(0,2, G3DDEMO_COLOR_WHITE, "change:A");
    G3DDemo_Print(0,4, G3DDEMO_COLOR_WHITE, "change:B");

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
            VecFx32 scale;

            //
            // Bボタンでモデルにかかるスケールが変更されている
            //
            if (isScaled)
            {
                scale.x = scale.y = scale.z = 2 * FX32_ONE;
            }
            else
            {
                scale.x = scale.y = scale.z = FX32_ONE;
            }

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
            NNS_G3dGlbFlushP();
            NNS_G3dGeFlushBuffer();
            G3DDemo_DrawGround(&gGround);
        }

        /* ジオメトリ＆レンダリングエンジン関連メモリのスワップ */
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        // AボタンでSBCのビルボードコマンドのハンドラを変更
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            if (useCustomizedBB)
            {
                useCustomizedBB = FALSE;
                NNS_G3dFuncSbcTable[NNS_G3D_SBC_BB]  = &NNSi_G3dFuncSbc_BB;
                NNS_G3dFuncSbcTable[NNS_G3D_SBC_BBY] = &NNSi_G3dFuncSbc_BBY;
            }
            else
            {
                useCustomizedBB = TRUE;
                NNS_G3dFuncSbcTable[NNS_G3D_SBC_BB]  = &MySbcBBSimple;
                NNS_G3dFuncSbcTable[NNS_G3D_SBC_BBY] = &MySbcBBYSimple;
            }
        }

        // Bボタンでモデルにかかるスケールを変更
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_B))
        {
            if (isScaled)
                isScaled = FALSE;
            else
                isScaled = TRUE;
        }

        if (useCustomizedBB)
        {
            G3DDemo_Printf(0,3, G3DDEMO_COLOR_GREEN, "Use simplified BB Functions");
        }
        else
        {
            G3DDemo_Printf(0,3, G3DDEMO_COLOR_GREEN, "Use default BB Functions   ");
        }

        if (isScaled)
        {
            G3DDemo_Printf(0,5, G3DDEMO_COLOR_GREEN, "Billboards scaled    ");
        }
        else
        {
            G3DDemo_Printf(0,5, G3DDEMO_COLOR_GREEN, "Billboards not scaled");
        }

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06ld usec", OS_TicksToMicroSeconds(time));
    }
}


/*---------------------------------------------------------------------------*
    MySbcBBSimple
    
    単純化されたビルボード処理関数。以下のような制限がある代わりに高速になっている
    ・コールバックポイントが存在しない。
    ・optはNNS_G3D_SBCFLG_000でなくてはならない
    ・NNS_G3dGlbFlushP以外には対応していない
    ・NNS_G3D_RSFLAG_OPT_NOGECMDには対応していない
    ・スケールがかかっている場合、正しく表示されない
 *---------------------------------------------------------------------------*/
static void
MySbcBBSimple(NNSG3dRS* rs, u32 opt)
{
    //
    // 「カメラの射影平面」に平行なビルボードの表示
    //
    static u32 bbcmd1[] =
    {
        ((G3OP_MTX_POP << 0)       |
         (G3OP_MTX_MODE << 8)      |
         (G3OP_MTX_LOAD_4x3 << 16)),
        1,
        GX_MTXMODE_POSITION_VECTOR,
        FX32_ONE, 0, 0, 
        0, FX32_ONE, 0,
        0, 0, FX32_ONE,
        0, 0, 0   // ここは可変(Trans)
    };
    
    VecFx32* trans = (VecFx32*)&bbcmd1[12];
    MtxFx44 m;

    NNS_G3D_NULL_ASSERT(rs);

    //
    // コールバックポイントは存在しない
    //

    //
    // 行列スタックとの出し入れはできない
    //
#pragma unused(opt)
    NNS_G3D_ASSERT(opt == NNS_G3D_SBCFLG_000);

    //
    // NNS_G3dGlbFlushAltは禁止してしまう
    //
    NNS_G3D_ASSERT(!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_ALT));

    //
    // NNS_G3D_RSFLAG_OPT_NOGECMDも禁止
    //
    NNS_G3D_ASSERT(!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD));

    // バッファのフラッシュ
    NNS_G3dGeFlushBuffer();

    // コマンド転送:
    // PROJモードに変更
    // 射影行列を退避
    // 単位行列をセット
    reg_G3X_GXFIFO = ((G3OP_MTX_MODE << 0) |
                      (G3OP_MTX_PUSH << 8) |
                      (G3OP_MTX_IDENTITY << 16));
    reg_G3X_GXFIFO = (u32)GX_MTXMODE_PROJECTION;

    // 更にジオメトリエンジンの停止を待つ
    // カレント行列の取得
    while (G3X_GetClipMtx(&m))
        ;

    // ビルボード行列の計算
    trans->x = m._30;
    trans->y = m._31;
    trans->z = m._32;

    // 射影行列のPOP
    // POS_VECに復帰
    // カレント行列に格納
    MI_CpuSend32(&bbcmd1[0],
                 &reg_G3X_GXFIFO,
                 15 * sizeof(u32));

    rs->c += 2;
}


/*---------------------------------------------------------------------------*
    MySbcBBYSimple
    
    単純化されたY軸ビルボード処理関数。以下のような制限がある代わりに高速になっている
    ・コールバックポイントが存在しない。
    ・optはNNS_G3D_SBCFLG_000でなくてはならない
    ・NNS_G3dGlbFlushP以外には対応していない
    ・NNS_G3D_RSFLAG_OPT_NOGECMDには対応していない
    ・スケールがかかっている場合、正しく表示されない
 *---------------------------------------------------------------------------*/
static void
MySbcBBYSimple(NNSG3dRS* rs, u32 opt)
{
    MtxFx44 m;
    VecFx32 lz;

    static u32 bbcmd1[] =
    {
        ((G3OP_MTX_POP << 0)       |
         (G3OP_MTX_MODE << 8)      |
         (G3OP_MTX_LOAD_4x3 << 16)),
        1,
        GX_MTXMODE_POSITION_VECTOR,
        FX32_ONE, 0, 0, // ここは可変(4x3Mtx)
        0, FX32_ONE, 0,
        0, 0, FX32_ONE,
        0, 0, 0
    };
    VecFx32* trans = (VecFx32*)&bbcmd1[12];
    MtxFx43* mtx   = (MtxFx43*)&bbcmd1[3];

    NNS_G3D_NULL_ASSERT(rs);
    
    //
    // コールバックポイントは存在しない
    //

    //
    // 行列スタックとの出し入れはできない
    //
#pragma unused(opt)
    NNS_G3D_ASSERT(opt == NNS_G3D_SBCFLG_000);

    //
    // NNS_G3dGlbFlushAltは禁止してしまう
    //
    NNS_G3D_ASSERT(!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_ALT));

    //
    // NNS_G3D_RSFLAG_OPT_NOGECMDも禁止
    //
    NNS_G3D_ASSERT(!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD));

    // バッファのフラッシュ
    NNS_G3dGeFlushBuffer();

    // コマンド転送:
    // PROJモードに変更
    // 射影行列を退避
    // 単位行列をセット
    reg_G3X_GXFIFO = ((G3OP_MTX_MODE << 0) |
                      (G3OP_MTX_PUSH << 8) |
                      (G3OP_MTX_IDENTITY << 16));
    reg_G3X_GXFIFO = (u32)GX_MTXMODE_PROJECTION;

    // 更にジオメトリエンジンの停止を待つ
    // カレント行列(クリップ行列)の取得
    while (G3X_GetClipMtx(&m))
        ;

    // ビルボード行列の計算
    trans->x = m._30;
    trans->y = m._31;
    trans->z = m._32;

    // mtxの_00, _01, _02は不変
    VEC_Normalize((VecFx32*)&m._10, (VecFx32*)&mtx->_10);

    lz.x = 0;
    lz.y = -m._12;
    lz.z = m._11;
    VEC_Normalize(&lz, (VecFx32*)&mtx->_20);

    // 射影行列のPOP
    // POS_VECに復帰
    // カレント行列に格納
    MI_CpuSend32(&bbcmd1[0],
                 &reg_G3X_GXFIFO,
                 15 * sizeof(u32));

    rs->c += 2;
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

/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ManualSetup
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
// NNS_G3dResDefaultSetupを使用せずにモデルリソースをセットアップするサンプル
//
// モデルのバイナリデータをメインメモリ上にロードNNS_G3dResDefaultSetupを使用
// せずにセットアップを行ない、モデルを画面に表示します。
//
// サンプルの動作
// モデルを表示します。
// 
// HOWTO:
// コード上の説明を参照してください。
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void           InitG3DLib(void);
static NNSG3dResMdl*  LoadG3DModel(const char* path);
static void           SetCamera(G3DDemoCamera* camera);


static void
ManualSetup(void* pResFile /* pResFileはロードされた.nsbmdファイルへのポインタ */)
{
    //
    // NNS_G3dResDefaultSetupを使用せずに自分でセットアップする
    // このコードでは、.nsbmd内のインデックス＃０のモデルのみをセットアップしている。
    //
    NNSG3dResMdlSet* mdlSet = NNS_G3dGetMdlSet(pResFile);
    NNSG3dResTex* texture = NNS_G3dGetTex(pResFile); // テクスチャデータがない場合はNULLを返す。

    //
    // モデルデータの初期化には以下のようなステップが必要です。
    // 実際にはテクスチャのないモデルの初期化処理は必要ありません。
    //
    // 1: 必要なテクスチャ・パレット領域のサイズを取得します
    // 2: テクスチャやパレット用に使用するVRAM領域のためのキー(gfdライブラリと同じフォーマット)を作成します
    // 3: キーをモデルデータのテクスチャブロックに設定します
    // 4: テクスチャやパレットをVRAMに転送します
    // 5: モデルリソース内のマテリアルとテクスチャをバインド(関連付け)します
    //
    // 4と5は順番を入れ替えても問題ありません。
    //
    // なお、NNS_G3dResDefaultSetupと同様、テクスチャやパレットの転送には
    // DMAを使用するため、このセットアップ関数を呼ぶ前にこれらのデータを
    // キャッシュからメインメモリにライトバックしておく必要があります。
    //
    if (texture)
    {
        //
        // 1: 必要なテクスチャ・パレット領域のサイズを取得します。
        //
        u32 szTex     = NNS_G3dTexGetRequiredSize(texture);
        u32 szTex4x4  = NNS_G3dTex4x4GetRequiredSize(texture);
        u32 szPltt    = NNS_G3dPlttGetRequiredSize(texture);

        //
        // 2: テクスチャやパレット用に使用するVRAM領域のためのキー
        //    (gfdライブラリと同じフォーマット)を作成します。
        //
        // キーに設定する領域のサイズは、必要なサイズ以上である必要があります。
        // また、正当なキーならば、ユーザーが独自に作成してもかまいません。
        // キーは、NNS_GfdMakeTexKey, NNS_GfdMakePlttKeyを使用することにより
        // 直接作成することもできます。
        //
        u32 keyTex    = NNS_GfdAllocTexVram(szTex, FALSE, 0);
        u32 keyTex4x4 = NNS_GfdAllocTexVram(szTex4x4, TRUE, 0);
        u32 keyPltt   = NNS_GfdAllocPlttVram(szPltt, FALSE, TRUE);

        OS_Printf("keyTex    = 0x%08x addr = 0x%08x size = 0x%08x\n", keyTex   , NNS_GfdGetTexKeyAddr(keyTex   ), NNS_GfdGetTexKeySize(keyTex   ));
        OS_Printf("keyTex4x4 = 0x%08x addr = 0x%08x size = 0x%08x\n", keyTex4x4, NNS_GfdGetTexKeyAddr(keyTex4x4), NNS_GfdGetTexKeySize(keyTex4x4));
        OS_Printf("keyPltt   = 0x%08x addr = 0x%08x size = 0x%08x\n", keyPltt  , NNS_GfdGetPlttKeyAddr(keyPltt ), NNS_GfdGetPlttKeySize(keyPltt ));

        //
        // 3: 作成したキーをモデルデータのテクスチャブロックに設定します。
        // 
        NNS_G3dTexSetTexKey(texture, keyTex, keyTex4x4);
        NNS_G3dPlttSetPlttKey(texture, keyPltt);

        //
        // 4: テクスチャやパレットをVRAMに転送します。
        //
        // 設定したキーの情報に従ってテクスチャとパレットをVRAMに転送します。
        // 2番目の引数にTRUEを設定すると毎回BeginLoadTex, EndLoadTexを内部で行う
        // FALSEだとなにもしません（外部でBeginLoadTex,EndLoadTexをする必要がある）
        //
        NNS_G3dTexLoad(texture, TRUE);
        NNS_G3dPlttLoad(texture, TRUE);

        //
        // 5: モデルリソース内のマテリアルとテクスチャをバインド(関連付け)します
        //
        // NNS_G3dBindMdlSetを使用して、モデルリソースのマテリアル内の
        // テクスチャ関連データ(TexImageParam TexPlttBase)をセットアップします。
        // 
        // 関数はBOOL型の返り値を持ち、返り値がTRUEの場合はモデル内のテクスチャを持つマテリアル
        // 全てが対応するテクスチャにバインドされています。
        // FALSEの場合は、テクスチャにバインドされていないマテリアルがまだ残っています。
        //
        // なお、既にバインドされているマテリアルはReleaseしない限り新しいテクスチャにバインドする
        // ことができません(要するに同じテクスチャ名を持つ複数のテクスチャブロックをバインド
        // した場合、先にバインドされたテクスチャが有効になります)。
        //
        {
            BOOL result;
            result = NNS_G3dBindMdlSet(mdlSet, texture);
            SDK_ASSERT(result);
        }
    }
}


static void
ManualRelease(void* pResFile /* pResFileはロードされた.nsbmdファイルへのポインタ */)
{
    //
    // NNS_G3dResDefaultReleaseを使用せずに自分でリリースする。
    //

    NNSG3dResTex* texture = NNS_G3dGetTex(pResFile); // テクスチャデータがない場合はNULLを返す。

    //
    // モデルデータの解放処理には以下のようなステップが必要です。
    // 実際にはテクスチャのないモデルの解放処理は必要ありません。
    //
    // 1: モデルリソース内に設定されているテクスチャリソースへの関連付けをクリアする。
    // 2: テクスチャリソースからキーをとり外す。
    // 3: (利用している場合は)VRAMマネージャを用いてキーに対応するVRAM領域を解放する。
    //
    if (texture)
    {
        NNSG3dPlttKey plttKey;
        NNSG3dTexKey  texKey, tex4x4Key;
        int status;
        NNSG3dResMdlSet* mdlSet = NNS_G3dGetMdlSet(pResFile);

        //
        // 1: モデルリソース内に設定されているテクスチャリソースへの関連付けをクリアする。
        //
        NNS_G3dReleaseMdlSet(mdlSet);

        // 2: テクスチャリソースからキーをとり外す。
        plttKey = NNS_G3dPlttReleasePlttKey(texture);
        NNS_G3dTexReleaseTexKey(texture, &texKey, &tex4x4Key);

        //
        // 3: (利用している場合は)VRAMマネージャを用いてキーに対応するVRAM領域を解放する。
        //
        // VRAMマネージャを使用している場合は、VRAMマネージャを利用して
        // VRAM領域を解放しておきます。
        // ただし、このサンプルで使用しているフレーム方式のマネージャの場合は、
        // 領域を個別に解放できないので、同じ領域を再利用することはできません。
        // ユーザーが独自に管理する場合は、以下のコードをユーザー独自のものに
        // する必要ががります。
        //
        if (plttKey > 0)
        {
            // テクスチャパレットスロット内に確保されていれば解放
            status = NNS_GfdFreePlttVram(plttKey);
            NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreePlttVram failed");
        }

        if (tex4x4Key > 0)
        {
            // テクスチャイメージスロット内に確保されていれば解放
            status = NNS_GfdFreeTexVram(tex4x4Key);
            NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreeTexVram failed");
        }

        if (texKey > 0)
        {
            // テクスチャイメージスロット内に確保されていれば解放
            status = NNS_GfdFreeTexVram(texKey);
            NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreeTexVram failed");
        }
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
    void*           pResFile;
    BOOL            isSetup = TRUE;

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

    {
        //
        // モデルリソースのロード
        //
        pResFile = G3DDemo_LoadFile("data/human_run_t.nsbmd");
        SDK_ASSERTMSG(pResFile, "load failed");

        // ManualSetupやNNS_G3dDrawの中でDMA転送を使用するため、
        // これらの関数を呼ぶ前にモデルリソース全体をメモリへストアします。
        // リソースのサイズがある程度大きい場合はDC_StoreAllを呼んだ方が高速です。
        // DC_StoreRangeやDC_StoreAllについてはNITRO-SDK関数リファレンスマニュアルを参照して下さい。
        DC_StoreRange(pResFile, ((NNSG3dResFileHeader*)pResFile)->fileSize);

        ManualSetup(pResFile);

        //
        // モデルリソース内のモデルセットからインデックス#0のモデルを取得
        //
        pModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(pResFile), 0);
        SDK_ASSERTMSG(pModel, "model not found");

        //
        // G3D: NNSG3dRenderObjの初期化
        //
        // NNSG3dRenderObjは１つのモデルとそのモデルに適用されるアニメに関する各種情報を
        // 保持する構造体です。
        // 内部のメンバポインタで指定される領域はユーザー自身で取得・解放する必要があります。
        // ここでmdlはNULLであってもかまいません(NNS_G3dDraw時には設定されている必要がある)。
        //
        NNS_G3dRenderObjInit(&object, pModel);
    }

    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ManualSetup");

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

        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            if (isSetup)
            {
                ManualRelease(pResFile);
                pModel = NULL;
                isSetup = FALSE;
            }
            else
            {
                ManualSetup(pResFile);
                pModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(pResFile), 0);
                SDK_ASSERTMSG(pModel, "model not found");
                NNS_G3dRenderObjInit(&object, pModel);

                isSetup = TRUE;
            }
        }

        if (isSetup)
        {
            // スケールをかけて小さくしてみる
            VecFx32 scale = {FX32_ONE>>2, FX32_ONE>>2, FX32_ONE>>2};
            NNS_G3dGlbSetBaseScale(&scale);

            //
            // G3D:
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

        if (isSetup)
        {
            G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06d usec", OS_TicksToMicroSeconds(time));
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_GREEN, "Press A Button to Release");
        }
        else
        {
            G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:           ");
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_GREEN, "Press A Button to Setup  ");
        }
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

/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - DisposeTex
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
//
// メインメモリ上のテクスチャ／パレット領域をVRAM転送後に破棄するサンプル
//
// 主人公キャラ等のモデルのテクスチャ／パレットをVRAM領域に常駐させる場合、
// メインメモリ上にそのモデルのテクスチャ／パレットを保持しておく必要はありません。
// そのような場合は、リソースファイルの最後尾にあるテクスチャ／パレット領域を解放して
// メモリを節約することができます。
//
// サンプルの動作
// サンプル内のCollapseResourceMemory関数において、NNSG3dResTexからテクスチャ／パレット
// の開始アドレスを取得し、そのアドレス以降の領域をヒープに返却しています。
//
// HOWTO
// texをNNSG3dResTex構造体へのポインタとすると、
// (u8*)tex + tex->texInfo.ofsTexがテクスチャ／パレットデータの開始アドレスになります。
// それ以降の番地のメモリは書き換えても構いません。
//
//---------------------------------------------------------------------------


#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void SetCamera(G3DDemoCamera* camera);
static void CollapseResourceMemory(void* memBlock, NNSG3dResTex* texture);
static void TextureZeroClear(NNSG3dResTex* pTex);

/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  サンプルのメイン。
                TextureZeroClearでテクスチャ領域をゼロクリアします。
                Ram転送後にテクスチャ領域をゼロクリアしても問題なく動作することを示す。

  Arguments:    なし。

  Returns:      なし。
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    NNSG3dRenderObj object;
    NNSG3dResMdl*   model;

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

    //
    // LoadG3DModel内部でデータをロード、テクスチャをVRAMに設定する等の
    // 初期化を行った後、テクスチャデータ部分のメモリをゼロクリアし、
    // ヒープを縮小してテクスチャデータ部分のみを解放します。
    //
    model = LoadG3DModel("data/human_run_t.nsbmd");
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

    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "DisposeTex");

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
    NNSG3dResMdl* model   = NULL;
    NNSG3dResTex* texture = NULL;
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
    // テクスチャブロックのテクスチャデータ部分をゼロクリアします。
    //
    TextureZeroClear(NNS_G3dGetTex(resFile));

    //
    // ヒープを縮小して、テクスチャデータ部分を解放します。
    //
    CollapseResourceMemory(resFile, NNS_G3dGetTex(resFile));

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


/* -------------------------------------------------------------------------
  Name:         CollapseResourceMemory

  Description:  テクスチャイメージのサイズ分メモリブロックを縮小します。

                メモリブロックの末尾はテクスチャイメージのみであることを
                前提としています。

  Arguments:    memBlock:  縮小するメモリブロック。
                texture:   テクスチャデータのポインタ。

  Returns:      なし。
   ------------------------------------------------------------------------- */
static void
CollapseResourceMemory(void* memBlock, NNSG3dResTex* texture)
{
    u8* texImgStartAddr;
    u32 newSize;

    // テクスチャ／パレットは、4x4COMP以外のテクスチャ・4x4COMPのテクスチャ・パレット
    // の順番で格納されています。よって4x4COMP以外のテクスチャの開始アドレス以降は
    // 不要になります。なお、4x4COMPフォーマット以外のテクスチャイメージが存在しない
    // 場合でもtexture->texInfo.ofsTexには適切な値が入っています。
    SDK_ASSERT(texture->texInfo.ofsTex != 0);
    texImgStartAddr = (u8*)texture + texture->texInfo.ofsTex;

    // ヒープの先頭からテクスチャイメージまでのサイズ
    newSize = (u32)(texImgStartAddr - (u8*)memBlock);

    // メモリブロックを縮小します。
    // その結果、テクスチャイメージ分のメモリはヒープに返却されます。
    (void)NNS_FndResizeForMBlockExpHeap(G3DDemo_AppHeap, memBlock, newSize);

}


/*-------------------------------------------------------------------------
    TextureZeroClear

    テクスチャブロックのテクスチャデータ部分をゼロクリアします。
    辞書等の情報は残るので、バインドやリリースは可能です。
  ------------------------------------------------------------------------- */
static void
TextureZeroClear(NNSG3dResTex* pTex)
{
    MI_CpuClearFast((u8*)pTex + pTex->texInfo.ofsTex,
                    pTex->header.size - pTex->texInfo.ofsTex);
}

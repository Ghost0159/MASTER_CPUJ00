/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - UnbindTex
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
// テクスチャの取り外し、交換のサンプル
//
// モデルからテクスチャを取り外したり、違うモデルとテクスチャを交換したりします。
//
// サンプルの動作
// 立方体と直方体にテクスチャが貼られています。
// Aボタン->テクスチャを交換します。
// Bボタン->テクスチャをとりはずします。
//
// HOWTO:
// ・NNS_G3dBindMdlTex, NNS_G3dBindMdlPlttでモデルとテクスチャを関連付けます。
// ・NNS_G3dReleaseMdlTex, NNS_G3dReleaseMdlPlttでモデルとテクスチャの関連付けを
//   解除できます。
// ・NNS_G3dBindMdlTexEx, NNS_G3dBindMdlPlttExでモデル内の指定された名前の
//   テクスチャを関連付けます。
// ・NNS_G3dReleaseMdlTexEx, NNS_G3dReleaseMdlPlttExでモデル内の指定された名前の
//   テクスチャへの関連付けを解除できます。
//
//---------------------------------------------------------------------------


#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void          InitG3DLib(void);
static NNSG3dResTex* LoadG3DTexture(NNSG3dResFileHeader* resFile);
static void          SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  サンプルのメイン。
                BでテクスチャをはずしてAでテクスチャを交換します。

  Arguments:    なし。

  Returns:      なし。
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    
    BOOL changeTexture = TRUE;
    
    //二つのモデル用
    NNSG3dResFileHeader* resFileCube = NULL;
    NNSG3dResFileHeader* resFileBox  = NULL;
    
    NNSG3dRenderObj cubeObject;
    NNSG3dResMdl*   cubeModel = NULL;
    NNSG3dResTex*   cubeTexture = NULL;
    
    NNSG3dRenderObj boxObject;
    NNSG3dResMdl*   boxModel = NULL;
    NNSG3dResTex*   boxTexture = NULL;
    
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
        // Box Modelをロードして初期化。
        // 後で使うNNSG3dResMdl*と、NNSG3dResTex*を取得しておく。
        //
        resFileBox = G3DDemo_LoadFile("data/TextureBox.nsbmd");

        // NNS_G3dResDefaultSetupやNNS_G3dDrawの中でDMA転送を使用するため、
        // これらの関数を呼ぶ前にモデルリソース全体をメモリへストアします。
        // リソースのサイズがある程度大きい場合はDC_StoreAllを呼んだ方が高速です。
        // DC_StoreRangeやDC_StoreAllについてはNITRO-SDK関数リファレンスマニュアルを参照して下さい。
        DC_StoreRange(resFileBox, resFileBox->fileSize);

        (void)NNS_G3dResDefaultSetup(resFileBox);

        SDK_ASSERTMSG(resFileBox, "load failed");
        boxModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFileBox), 0);
        SDK_NULL_ASSERT(boxModel);

        boxTexture = NNS_G3dGetTex(resFileBox);
    }

    {
        //
        // Cube Modelをロードして初期化
        // 後で使うNNSG3dResMdl*と、NNSG3dResTex*を取得しておく。
        //
        resFileCube = G3DDemo_LoadFile("data/TextureCube.nsbmd");

        // NNS_G3dResDefaultSetupやNNS_G3dDrawの中でDMA転送を使用するため、
        // これらの関数を呼ぶ前にモデルリソース全体をメモリへストアします。
        // リソースのサイズがある程度大きい場合はDC_StoreAllを呼んだ方が高速です。
        // DC_StoreRangeやDC_StoreAllについてはNITRO-SDK関数リファレンスマニュアルを参照して下さい。
        DC_StoreRange(resFileCube, resFileCube->fileSize);

        (void)NNS_G3dResDefaultSetup(resFileCube);

        SDK_ASSERTMSG(resFileCube, "load failed");
        cubeModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFileCube), 0);
        SDK_NULL_ASSERT(cubeModel);

        cubeTexture = NNS_G3dGetTex(resFileCube);
    }

    // G3D: NNSG3dRenderObjの初期化
    //
    // NNSG3dRenderObjは１つのモデルとそのモデルに適用されるアニメに関する各種情報を
    // 保持する構造体です。
    // 内部のメンバポインタで指定される領域はユーザー自身で取得・解放する必要があります。
    // ここでmdlはNULLであってもかまいません(NNS_G3dDraw時には設定されている必要がある)。
    //
    NNS_G3dRenderObjInit(&cubeObject, cubeModel);
    NNS_G3dRenderObjInit(&boxObject, boxModel);

    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "UNBINDTEX SAMPLE");

    for(;;)
    {
        OSTick time;

        //Aでテクスチャの切り替え
        //release後でないとbindは有効になりません
        if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A)) 
        {
            // same_nameという名前を持つテクスチャを入れ替える。
            // このサンプルで使用しているモデルのテクスチャはダイレクトテクスチャ
            // なのでパレットを入れ替える必要はない
            NNS_G3D_DEFRESNAME(texName, "same_name");
            (void)NNS_G3dReleaseMdlTexEx(boxModel, &texName.resName);
            (void)NNS_G3dReleaseMdlTexEx(cubeModel, &texName.resName);

            if (changeTexture)
            {
                //
                // 相手モデルのテクスチャをバインドする。
                //
                (void)NNS_G3dBindMdlTexEx(boxModel, cubeTexture, &texName.resName);
                (void)NNS_G3dBindMdlTexEx(cubeModel, boxTexture, &texName.resName);
            }
            else
            {
                //
                // 自モデルのテクスチャをバインドする。
                //
                (void)NNS_G3dBindMdlTexEx(boxModel, boxTexture, &texName.resName);
                (void)NNS_G3dBindMdlTexEx(cubeModel, cubeTexture, &texName.resName);
            }
            changeTexture = (changeTexture) ? FALSE : TRUE;
        }                            
        else if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_B)) // Bボタンでテクスチャを外します
        {
            NNS_G3dReleaseMdlTex(boxModel);
            NNS_G3dReleaseMdlPltt(boxModel); // パレットが存在しない場合は何もしない
            
            NNS_G3dReleaseMdlTex(cubeModel);
            NNS_G3dReleaseMdlPltt(cubeModel); // パレットが存在しない場合は何もしない
        }

        SVC_WaitVBlankIntr();
        G3DDemo_PrintApplyToHW();
        G3DDemo_ReadGamePad();

        G3DDemo_MoveCamera(&gCamera);
        G3DDemo_CalcCamera(&gCamera);



        G3X_Reset();
        G3X_ResetMtxStack();

        SetCamera(&gCamera);

        time = OS_GetTick();
        {
            //boxModel描画
            VecFx32 trans = {FX32_ONE<<2, FX32_ONE<<2, 0};
            NNS_G3dGlbSetBaseTrans(&trans);
            NNS_G3dGlbFlushP();
            NNS_G3dDraw(&boxObject);
        }

        {
            //cubeModel描画
            VecFx32 trans = {-FX32_ONE<<2, FX32_ONE<<2, 0};
            NNS_G3dGlbSetBaseTrans(&trans);
            NNS_G3dGlbFlushP();
            NNS_G3dDraw(&cubeObject);
        }
        time = OS_GetTick() - time;

        {
            // グラウンドを描画する前にTranslationを設定しておく
            VecFx32 trans = {0,0,0};
            NNS_G3dGlbSetBaseTrans(&trans);
        }
        
        //
        // G3D: 通常のG3_XXXを呼ぶときは、その前にNNS_G3dGeComFlushBuffer()
        //      をコールして同期しなくてはならない
        //
        NNS_G3dGlbFlushP();
        NNS_G3dGeFlushBuffer();
        G3DDemo_DrawGround(&gGround);

        /* ジオメトリ＆レンダリングエンジン関連メモリのスワップ */
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06d usec", OS_TicksToMicroSeconds(time));
        
        //情報表示
        G3DDemo_Printf(0,2, G3DDEMO_COLOR_GREEN, "change Texture:A");
        G3DDemo_Printf(0,3, G3DDEMO_COLOR_GREEN, "release Texture:B");
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

/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - PartialAnm2
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
// ジョイントアニメーションを部分的に再生するサンプル２
//
// イニシャライズされたアニメーションオブジェクトを適切に操作することによって、
// 例えば腕だけを動かすといった、ジョイントアニメーションの一部再生が可能です。
//
// サンプルの動作
// 人が走っています。ボタンを押すと、四肢のアニメーションを独立にＯＮ／ＯＦＦできます。
// Ａボタンを押すと、左脚のアニメーションがＯＮ／ＯＦＦします。
// Ｂボタンを押すと、右脚のアニメーションがＯＮ／ＯＦＦします。
// Ｘボタンを押すと、左腕のアニメーションがＯＮ／ＯＦＦします。
// Ｙボタンを押すと、右腕のアニメーションがＯＮ／ＯＦＦします。
//
// HOWTO:
// 1: JointAnmサンプルと同様にアニメーションオブジェクトをセットアップします。
// 2: 右腕・左腕・右脚・左脚それぞれだけが動くアニメーションオブジェクトを
//    NNS_G3dAnmObjDisableIDを用いて、作成します。
// 3: それぞれのアニメーションオブジェクトを独立にコントロールすることにより、
//    右腕・左腕・右脚・左脚が動かしたり止めたりできます。
//
//---------------------------------------------------------------------------
#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // カメラ構造体。
G3DDemoGround gGround;        // グラウンド構造体。

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
    anmObjArmL

    アニメーションオブジェクトを操作して左腕だけを動かすように設定する。
   ------------------------------------------------------------------------- */
static void anmObjArmL(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int arm_l1, arm_l2, wrist_l, wrist_l_end;
    int i;

    // 左腕部分のノードIDを取得
    NNS_G3D_GET_JNTID(m, &arm_l1, "arm_l1");
    NNS_G3D_GET_JNTID(m, &arm_l2, "arm_l2");
    NNS_G3D_GET_JNTID(m, &wrist_l, "wrist_l");
    NNS_G3D_GET_JNTID(m, &wrist_l_end, "wrist_l_end");

    SDK_ASSERT(arm_l1 >= 0 && arm_l2 >= 0 && wrist_l >=0 && wrist_l_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != arm_l1  &&
            i != arm_l2  &&
            i != wrist_l &&
            i != wrist_l_end)
        {
            //
            // 左腕以外のアニメーションはDisableにする。
            //
            NNS_G3dAnmObjDisableID(p, i);
        }
    }
}


/* -------------------------------------------------------------------------
    anmObjArmR

    アニメーションオブジェクトを操作して右腕だけを動かすように設定する。
   ------------------------------------------------------------------------- */
static void anmObjArmR(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int arm_r1, arm_r2, wrist_r, wrist_r_end;
    int i;

    // 左腕部分のノードIDを取得
    NNS_G3D_GET_JNTID(m, &arm_r1, "arm_r1");
    NNS_G3D_GET_JNTID(m, &arm_r2, "arm_r2");
    NNS_G3D_GET_JNTID(m, &wrist_r, "wrist_r");
    NNS_G3D_GET_JNTID(m, &wrist_r_end, "wrist_r_end");

    SDK_ASSERT(arm_r1 >= 0 && arm_r2 >= 0 && wrist_r >=0 && wrist_r_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != arm_r1  &&
            i != arm_r2  &&
            i != wrist_r &&
            i != wrist_r_end)
        {
            //
            // 左腕以外のアニメーションはDisableにする。
            //
            NNS_G3dAnmObjDisableID(p, i);
        }
    }
}


/* -------------------------------------------------------------------------
    anmObjLegL

    アニメーションオブジェクトを操作して左脚だけを動かすように設定する。
   ------------------------------------------------------------------------- */
static void anmObjLegL(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int leg_l1, leg_l2, foot_l, foot_l_end;
    int i;

    // 左脚部分のノードIDを取得
    NNS_G3D_GET_JNTID(m, &leg_l1, "leg_l1");
    NNS_G3D_GET_JNTID(m, &leg_l2, "leg_l2");
    NNS_G3D_GET_JNTID(m, &foot_l, "foot_l");
    NNS_G3D_GET_JNTID(m, &foot_l_end, "foot_l_end");

    SDK_ASSERT(leg_l1 >= 0 && leg_l2 >= 0 && foot_l >= 0 && foot_l_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != leg_l1 &&
            i != leg_l2 &&
            i != foot_l &&
            i != foot_l_end)
        {
            //
            // 左腕以外のアニメーションはDisableにする。
            //
            NNS_G3dAnmObjDisableID(p, i);
        }
    }
}


/* -------------------------------------------------------------------------
    anmObjLegR

    アニメーションオブジェクトを操作して右脚だけを動かすように設定する。
   ------------------------------------------------------------------------- */
static void anmObjLegR(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int leg_r1, leg_r2, foot_r, foot_r_end;
    int i;

    // 左脚部分のノードIDを取得
    NNS_G3D_GET_JNTID(m, &leg_r1, "leg_r1");
    NNS_G3D_GET_JNTID(m, &leg_r2, "leg_r2");
    NNS_G3D_GET_JNTID(m, &foot_r, "foot_r");
    NNS_G3D_GET_JNTID(m, &foot_r_end, "foot_r_end");

    SDK_ASSERT(leg_r1 >= 0 && leg_r2 >= 0 && foot_r >= 0 && foot_r_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != leg_r1 &&
            i != leg_r2 &&
            i != foot_r &&
            i != foot_r_end)
        {
            //
            // 左腕以外のアニメーションはDisableにする。
            //
            NNS_G3dAnmObjDisableID(p, i);
        }
    }
}


/* -------------------------------------------------------------------------
    anmObjLegR

    アニメーションオブジェクトを操作して左腕・右腕・左脚・右脚以外の部分だけを
    動かすように設定する。
   ------------------------------------------------------------------------- */
static void anmObjMisc(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int human_all, body_model, skl_root, hip, spin1, spin2, head, head_end;
    int i;

    // 左腕・右腕・左脚・右脚以外の部分のノードIDを取得
    NNS_G3D_GET_JNTID(m, &human_all, "human_all");
    NNS_G3D_GET_JNTID(m, &body_model, "body_model");
    NNS_G3D_GET_JNTID(m, &skl_root, "skl_root");
    NNS_G3D_GET_JNTID(m, &hip, "hip");
    NNS_G3D_GET_JNTID(m, &spin1, "spin1");
    NNS_G3D_GET_JNTID(m, &spin2, "spin2");
    NNS_G3D_GET_JNTID(m, &head, "head");
    NNS_G3D_GET_JNTID(m, &head_end, "head_end");

    SDK_ASSERT(human_all >= 0 && body_model >= 0 && skl_root >= 0 && hip >= 0 &&
               spin1 >= 0 && spin2 >= 0 && head >= 0 && head_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != human_all && i != body_model &&
            i != skl_root && i != hip &&
            i != spin1 && i != spin2 &&
            i != head && i != head_end)
        {
            //
            // 左腕・右腕・左脚・右脚のアニメーションはDisableにする。
            //
            NNS_G3dAnmObjDisableID(p, i);
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
    void*           pAnmRes;
    NNSFndAllocator allocator;
    
    NNSG3dAnmObj*   pAnmObjArmL;
    NNSG3dAnmObj*   pAnmObjArmR;
    NNSG3dAnmObj*   pAnmObjLegL;
    NNSG3dAnmObj*   pAnmObjLegR;
    NNSG3dAnmObj*   pAnmObjMisc;

    BOOL armL = TRUE;
    BOOL armR = TRUE;
    BOOL legL = TRUE;
    BOOL legR = TRUE;

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

            {
                // 使用するアニメーションオブジェクトをアロケート
                pAnmObjArmL = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjArmL);
                pAnmObjArmR = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjArmR);
                pAnmObjLegL = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjLegL);
                pAnmObjLegR = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjLegR);
                pAnmObjMisc = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjMisc);
            }

            {
                // 使用するアニメーションオブジェクトを初期化する
                NNS_G3dAnmObjInit(pAnmObjArmL, pAnmRes, pModel, NULL);
                NNS_G3dAnmObjInit(pAnmObjArmR, pAnmRes, pModel, NULL);
                NNS_G3dAnmObjInit(pAnmObjLegL, pAnmRes, pModel, NULL);
                NNS_G3dAnmObjInit(pAnmObjLegR, pAnmRes, pModel, NULL);
                NNS_G3dAnmObjInit(pAnmObjMisc, pAnmRes, pModel, NULL);
            }

            {
                // アニメーションオブジェクトを設定して体の一部だけを動かすようにする
                anmObjArmL(pAnmObjArmL, pModel);
                anmObjArmR(pAnmObjArmR, pModel);
                anmObjLegL(pAnmObjLegL, pModel);
                anmObjLegR(pAnmObjLegR, pModel);
                anmObjMisc(pAnmObjMisc, pModel);
            }

            {
                // RenderObjに各AnmObjを追加する。
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjArmL );
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjArmR );
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjLegL );
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjLegR );
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjMisc );
            }
        }
    }
    
    //
    // デモ共通の初期化
    //
    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "PartialAnm2");

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
        // アニメーションフレームを更新する。
        // それぞれのオブジェクトの初期フレームがバラバラなので、
        // 体はバラバラの動きをする。
        //
        pAnmObjArmL->frame += FX32_ONE;
        if( pAnmObjArmL->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjArmL))
            pAnmObjArmL->frame = 0;

        pAnmObjArmR->frame += FX32_ONE;
        if( pAnmObjArmR->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjArmR))
            pAnmObjArmR->frame = 0;

        pAnmObjLegL->frame += FX32_ONE;
        if( pAnmObjLegL->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjLegL))
            pAnmObjLegL->frame = 0;

        pAnmObjLegR->frame += FX32_ONE;
        if( pAnmObjLegR->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjLegR))
            pAnmObjLegR->frame = 0;

        pAnmObjMisc->frame += FX32_ONE;
        if( pAnmObjMisc->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjMisc))
            pAnmObjMisc->frame = 0;

        // Ａボタンは左脚のＯＮ／ＯＦＦ
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            if (legL)
            {
                legL = FALSE;
                NNS_G3dRenderObjRemoveAnmObj( &object, pAnmObjLegL );
            }
            else
            {
                legL = TRUE;
                pAnmObjLegL->frame = 0;
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjLegL );
            }
        }

        // Ｂボタンは右脚のＯＮ／ＯＦＦ
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_B))
        {
            if (legR)
            {
                legR = FALSE;
                NNS_G3dRenderObjRemoveAnmObj( &object, pAnmObjLegR );
            }
            else
            {
                legR = TRUE;
                pAnmObjLegR->frame = 0;
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjLegR );
            }
        }

        // Ｘボタンは左腕のＯＮ／ＯＦＦ
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_X))
        {
            if (armL)
            {
                armL = FALSE;
                NNS_G3dRenderObjRemoveAnmObj( &object, pAnmObjArmL );
            }
            else
            {
                armL = TRUE;
                pAnmObjArmL->frame = 0;
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjArmL );
            }
        }

        // Ｙボタンは右腕のＯＮ／ＯＦＦ
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_Y))
        {
            if (armR)
            {
                armR = FALSE;
                NNS_G3dRenderObjRemoveAnmObj( &object, pAnmObjArmR );
            }
            else
            {
                armR = TRUE;
                pAnmObjArmR->frame = 0;
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjArmR );
            }
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

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06ld usec", OS_TicksToMicroSeconds(time));

        // 四肢のアニメーションのＯＮ／ＯＦＦを表示
        if (armL)
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_GREEN, "Left  Arm(X): ON ");
        else
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_GREEN, "Left  Arm(X): OFF");

        if (armR)
            G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "Right Arm(Y): ON ");
        else
            G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "Right Arm(Y): OFF");

        if (legL)
            G3DDemo_Printf(0, 3, G3DDEMO_COLOR_GREEN, "Left  Leg(A): ON ");
        else
            G3DDemo_Printf(0, 3, G3DDEMO_COLOR_GREEN, "Left  Leg(A): OFF");

        if (legR)
            G3DDemo_Printf(0, 4, G3DDEMO_COLOR_GREEN, "Right Leg(B): ON ");
        else
            G3DDemo_Printf(0, 4, G3DDEMO_COLOR_GREEN, "Right Leg(B): OFF");

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

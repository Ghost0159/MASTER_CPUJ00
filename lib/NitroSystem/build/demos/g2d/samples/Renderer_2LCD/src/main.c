/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_2LCD
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.16 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      Renderer モジュールを用いて2画面(上画面とした画面)表示を行うサンプル。
//
//      各画面外のセルの描画を回避するため、可視カリング関数をレンダラに設定しています。
//
//  操作方法
//      START ボタン    Cell の出力方法を OBJ <=> SoftwareSprite と
//                      切り替えます。
//      X ボタン        Cell 及び Surface の位置をリセットします。
//      十字キー        Cell を移動します。
//      Y + 十字キー    メイン Surface を移動します。
//      B + 十字キー    サブ Surface を移動します。
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// 各種定数定義
#define SCREEN_WIDTH                256                     // 画面幅
#define SCREEN_HEIGHT               192                     // 画面高さ


#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // 出力方法の初期値

#define NUM_OF_OAM                  128                 // OAM マネージャに割り当てる OAM の数
#define NUM_OF_AFFINE               (NUM_OF_OAM / 4)    // OAM マネージャに割り当てる Affineパラメータ の数

#define CHARA_BASE                  0x0                 // キャラクタイメージベースアドレス
#define PLTT_BASE                   0x0                 // パレットベースアドレス

#define SURFACE_MAIN_INIT_POS ((NNSG2dFVec2){0 << FX32_SHIFT, 0 << FX32_SHIFT})
#define SURFACE_SUB_INIT_POS ((NNSG2dFVec2){0 << FX32_SHIFT, SCREEN_HEIGHT << FX32_SHIFT})

#define CELL_A_INIT_POS ((NNSG2dFVec2){150 << FX32_SHIFT, 120 << FX32_SHIFT})


static const NNSG2dFVec2 CELL_B_POS = {100 << FX32_SHIFT, 120 << FX32_SHIFT};
static const NNSG2dFVec2 CELL_C_POS = {100 << FX32_SHIFT, 300 << FX32_SHIFT};


//------------------------------------------------------------------------------
// 構造体定義


//------------------------------------------------------------------------------
// グローバル変数

static NNSG2dImageProxy             sImageProxy;    // Cell 用キャラクタ/テクスチャプロキシ
static NNSG2dImagePaletteProxy      sPaletteProxy;  // Cell 用パレットプロキシ

static NNSG2dOamManagerInstance     sMainOamMan;    // メイン画面用 OAM マネージャ
static NNSG2dOamManagerInstance     sSubOamMan;     // サブ画面用 OAM マネージャ

static NNSG2dAnimBankData*          spAnimBank;     // アニメーションバンク
static NNSG2dCellAnimation*         spCellAnim;     // セルアニメーション

//------------------------------------------------------------------------------
// プロトタイプ宣言



/*---------------------------------------------------------------------------*
  Name:         CallBackAddOam*

  Description:  NNS_G2dDraw* 関数内部で OAM を追加するために呼ばれる関数です。
                OAM マネージャに OAM を渡します。

  Arguments:    pOam:           追加すべき OAM へのポインタ。
                affineIndex:    この OAM が使用する Affine インデックス。
                                32 以上の値である可能性があります。
                                Affine を未使用の場合は
                                NNS_G2D_OAM_AFFINE_IDX_NONE を指定します。
                bDoubleAffine:  倍角Affineモードかどうか。

  Returns:      成功した場合は TRUE、そうでなければ FALSE を返します。
 *---------------------------------------------------------------------------*/
// メイン画面用
static BOOL CallBackAddOamMain( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine )
{
#pragma unused( bDoubleAffine )
    SDK_NULL_ASSERT( pOam );

    return NNS_G2dEntryOamManagerOamWithAffineIdx( &sMainOamMan, pOam, affineIndex );
}

// サブ画面用
static BOOL CallBackAddOamSub( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine )
{
#pragma unused( bDoubleAffine )
    SDK_NULL_ASSERT( pOam );

    return NNS_G2dEntryOamManagerOamWithAffineIdx( &sSubOamMan, pOam, affineIndex );
}

/*---------------------------------------------------------------------------*
  Name:         CallBackAddAffine*

  Description:  NNS_G2dDraw* 関数内部で Affine パラメータ を追加するために
                呼ばれる関数です。
                OAM マネージャに Affine パラメータを渡します。

  Arguments:    mtx:        追加すべき Affine 変換行列へのポインタ

  Returns:      追加した Affine パラメータを参照するための AffineIndex を
                返します。。
 *---------------------------------------------------------------------------*/
// メイン画面用
static u16 CallBackAddAffineMain( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( &sMainOamMan, mtx );
}

// サブ画面用
static u16 CallBackAddAffineSub( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( &sSubOamMan, mtx );
}

/*---------------------------------------------------------------------------*
  Name:         CallBackCulling

  Description:  NNS_G2dDraw* 関数内部でセルの表示/非表示を判定するために
                呼ばれる関数です。
                コンバータによってセルデータに付加されたセルの境界球半径を用い
                対象セルが Surface 内部にあるかどうかを判定します。
                
                注意：
                境界球算出のアルゴリズムから、サンプルで使用しているセルの境界球の半径は32程度（直径で64程度）と
                実際の表示サイズよりもかなり大きなサイズとなります。（実際には36となっているようです）。
                そのため、可視カリング処理で可視と判定され、回り込みのため、画面反対側に表示されてしまいます。

                対策はさまざまな方法が考えられます。

                A．素材固有の条件を考慮した特殊なカリング関数を実装する。
                B．セル境界矩形情報を利用してカリングをおこなう（詳細はコンバータマニュアルを参照ください。）。
                C．OBJ単位カリングを行う。

                今回のサンプルでは、
                A．素材固有の条件を考慮した特殊なカリング関数を実装する。
                の方法をとり、
                
                すべてのセルがセル原点からセルY軸正方向にOBJを配置していると仮定し
                本来
                const fx32  minY = py - R;
                const fx32  maxY = py + R;
                とすべき処理を
                を
                const fx32  minY = py - R;
                const fx32  maxY = py;
                としています。


  Arguments:    pCell:      判定対象のセルデータへのポインタ。
                pMtx:       判定対象のセルに適用される座標変換行列。
                pViewRect:  出力先 Surface の表示矩形。

  Returns:      対象セルを表示するのであれば TRUE、そうでなければ FALSE。
 *---------------------------------------------------------------------------*/
static BOOL CallBackCulling(
                const NNSG2dCellData*   pCell,
                const MtxFx32*          pMtx,
                const NNSG2dViewRect*   pViewRect)
{
    // コンバータにより計算された境界球半径を取得する
    const fx32  R = NNSi_G2dGetCellBoundingSphereR( pCell ) * FX32_ONE;

    // セルの表示位置原点を求める
    const fx32  px = pMtx->_20 - pViewRect->posTopLeft.x;
    const fx32  py = pMtx->_21 - pViewRect->posTopLeft.y;

    // セルの境界球を内包する矩形を求める
    // 注意：すべてのセルがセル原点からセルY軸正方向にOBJを配置していると仮定している。
#if 1
    const fx32  minY = py - R;
    const fx32  maxY = py;
#else
    const fx32  minY = py - R;
    const fx32  maxY = py + R;              
#endif 

    const fx32  minX = px - R;
    const fx32  maxX = px + R;


    if( (maxY > 0) && (minY < pViewRect->sizeView.y) )
    {
        if( (maxX > 0) && (minX < pViewRect->sizeView.x) )
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAM マネージャ実体の初期化を行います。

  Arguments:    pOamManager:    初期化する OAM マネージャへのポインタ。
                type:           どのグラフィックスエンジン向けに初期化するかを
                                指定します。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitOamManager(NNSG2dOamManagerInstance* pOamManager, NNSG2dOamType type)
{
    BOOL result;
    SDK_NULL_ASSERT( pOamManager );

    result = NNS_G2dGetNewManagerInstance( pOamManager, 0, NUM_OF_OAM - 1, type );
    SDK_ASSERT( result );
    result = NNS_G2dInitManagerInstanceAffine( pOamManager, 0, NUM_OF_AFFINE - 1 );
    SDK_ASSERT( result );
}

/*---------------------------------------------------------------------------*
  Name:         InitRenderer

  Description:  Renderer と Surface を初期化します。

  Arguments:    pRenderer:      初期化する Renderer へのポインタ。
                pMainSurface:   初期化するメイン画面 Surface へのポインタ。
                pSubSurface:    初期化するサブ画面 Surface へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitRenderer(
                NNSG2dRendererInstance* pRenderer,
                NNSG2dRenderSurface*    pMainSurface,
                NNSG2dRenderSurface*    pSubSurface)
{
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pMainSurface );
    SDK_NULL_ASSERT( pSubSurface );

    NNS_G2dInitRenderSurface( pMainSurface );
    NNS_G2dInitRenderSurface( pSubSurface );

    // メイン画面 Surface を初期化します
    {
        NNSG2dViewRect* pRect = &(pMainSurface->viewRect);

        // 表示矩形
        pRect->posTopLeft = SURFACE_MAIN_INIT_POS;
        pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
        pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

        // コールバック関数
        pMainSurface->pFuncOamRegister          = CallBackAddOamMain;
        pMainSurface->pFuncOamAffineRegister    = CallBackAddAffineMain;
        pMainSurface->pFuncVisibilityCulling    = CallBackCulling;

        // 表示先
        pMainSurface->type                      = NNS_G2D_SURFACETYPE_MAIN2D;
    }

    // サブ画面 Surface を初期化します
    {
        NNSG2dViewRect* pRect = &(pSubSurface->viewRect);

        // 表示矩形
        pRect->posTopLeft = SURFACE_SUB_INIT_POS;
        pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
        pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

        // コールバック関数
        pSubSurface->pFuncOamRegister           = CallBackAddOamSub;
        pSubSurface->pFuncOamAffineRegister     = CallBackAddAffineSub;
        pSubSurface->pFuncVisibilityCulling     = CallBackCulling;

        // 表示先
        pSubSurface->type                       = NNS_G2D_SURFACETYPE_SUB2D;
    }

    // Renderer を初期化します
    {
        NNS_G2dInitRenderer( pRenderer );

        // Surface 登録
        NNS_G2dAddRendererTargetSurface( pRenderer, pMainSurface );
        NNS_G2dAddRendererTargetSurface( pRenderer, pSubSurface );

        // プロキシ登録
        NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );

        // Z オフセット増分を指定
        NNS_G2dSetRendererSpriteZoffset( pRenderer, - FX32_ONE );
    }
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  アニメーションデータ、セルバンク、キャラクタデータ、
                パレットデータをファイルから読み込み、キャラクタデータと
                パレットデータをそれぞれVRAMにロードします。

  Arguments:    なし。

  Returns:      読み込んだセルバンクへのポインタ。
 *---------------------------------------------------------------------------*/
static NNSG2dCellDataBank* LoadResources(void)
{
    NNSG2dCellDataBank* pCellBank;
    void* pBuf;

    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //------------------------------------------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/Renderer_2LCD.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // セルバンクは最後までメインメモリ上で使用するので
        // この pBuf は開放しません。
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // セルアニメーションデータは最後までメインメモリ上で
        // 使用するのでこの pBuf は開放しません。
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/Renderer_2LCD.NANR" );
        SDK_NULL_ASSERT( pBuf );

        //
        // セルアニメーションの実体を初期化します
        //
        {
            spCellAnim = G2DDemo_GetNewCellAnimation(1);
            SDK_NULL_ASSERT( spCellAnim );

            SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx(spAnimBank, 0) );

            NNS_G2dInitCellAnimation(
                spCellAnim,
                NNS_G2dGetAnimSequenceByIdx(spAnimBank, 0),
                pCellBank );
        }
    }

    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Renderer_2LCD.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For Main 2D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            CHARA_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sImageProxy );

        // Loading For Sub 2D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            CHARA_BASE,
            NNS_G2D_VRAM_TYPE_2DSUB,
            &sImageProxy );

        // キャラクタデータを VRAM にコピーしたので
        // この pBuf は開放します。以下同じ。
        G2DDemo_Free( pBuf );
    }

    // load character data for 3D (software sprite)
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/Renderer_2LCD.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            CHARA_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sImageProxy );

        G2DDemo_Free( pBuf );
    }

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/Renderer_2LCD.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For Main 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        // Loading For Sub 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DSUB,
            &sPaletteProxy);

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sPaletteProxy);

        G2DDemo_Free( pBuf );
    }

    return pCellBank;
}


/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  キー入力に応じた処理を行います。

  Arguments:    pCellPos:       セルの位置へのポインタ。
                pOutputType:    出力方法を格納したバッファへのポインタ。
                pMainPos:       メイン画面 Surface の位置へのポインタ。
                pSubPos:        サブ画面 Surface の位置へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ProcessInput(NNSG2dFVec2* pCellPos,
                            NNSG2dSurfaceType* pOutputType,
                            NNSG2dFVec2* pMainPos,
                            NNSG2dFVec2* pSubPos)
{
    int dx = 0;
    int dy = 0;
    SDK_NULL_ASSERT( pCellPos );
    SDK_NULL_ASSERT( pOutputType );

    G2DDemo_ReadGamePad();

    // 十字キーの入力を取得
    {
        if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
        {
            dy--;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
        {
            dy++;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
        {
            dx--;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
        {
            dx++;
        }
    }

    if( G2DDEMO_IS_PRESS(PAD_BUTTON_Y) )
    {
        // メイン画面 Surface 移動
        pMainPos->x += (dx << FX32_SHIFT);
        pMainPos->y += (dy << FX32_SHIFT);
    }
    else if( G2DDEMO_IS_PRESS(PAD_BUTTON_B) )
    {
        // サブ画面 Surface 移動
        pSubPos->x += (dx << FX32_SHIFT);
        pSubPos->y += (dy << FX32_SHIFT);
    }
    else
    {
        // セル移動
        pCellPos->x += (dx << FX32_SHIFT);
        pCellPos->y += (dy << FX32_SHIFT);
    }

    // Reset
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
    {
        *pMainPos = SURFACE_MAIN_INIT_POS;
        *pSubPos  = SURFACE_SUB_INIT_POS;
        *pCellPos = CELL_A_INIT_POS;
    }

    // Toggle Output Type
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_START) )
    {
        *pOutputType = (*pOutputType == NNS_G2D_SURFACETYPE_MAIN2D) ?
                            NNS_G2D_SURFACETYPE_MAIN3D:
                            NNS_G2D_SURFACETYPE_MAIN2D;
    }
}

/*---------------------------------------------------------------------------*
  Name:         PrintSurfaceRect

  Description:  Surface の位置情報を表示します。

  Arguments:    x:          表示位置 x。
                y:          表示位置 y。
                pSurface:   表示対象の Surface へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void PrintSurfaceRect(u8 x, u8 y, const NNSG2dRenderSurface* pSurface)
{
    const NNSG2dViewRect* pRect = &(pSurface->viewRect);

    G2DDemo_PrintOutf(x, y, "(%3d, %3d)-(%3d, %3d)",
        pRect->posTopLeft.x >> FX32_SHIFT,
        pRect->posTopLeft.y >> FX32_SHIFT,
        (pRect->posTopLeft.x + pRect->sizeView.x - FX32_ONE) >> FX32_SHIFT,
        (pRect->posTopLeft.y + pRect->sizeView.y - FX32_ONE) >> FX32_SHIFT);
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    NNSG2dRendererInstance  render;         // 描画用 Renderer
    NNSG2dRenderSurface     mainSurface;    // メイン画面 Surface
    NNSG2dRenderSurface     subSurface;     // サブ画面 Surface
    NNSG2dCellDataBank*     pCellBank;      // Cell データ


    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();

        NNS_G2dInitOamManagerModule();
        InitOamManager( &sMainOamMan, NNS_G2D_OAMTYPE_MAIN );
        InitOamManager( &sSubOamMan, NNS_G2D_OAMTYPE_SUB );
        InitRenderer( &render, &mainSurface, &subSurface );
        pCellBank = LoadResources();
    }

    // start display
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    // Main loop
    while( TRUE )
    {
        static NNSG2dFVec2 cellAPos = CELL_A_INIT_POS;  // Cell A 描画位置


        // 入力を読み取り Cell と Surface の位置、メイン画面の出力方法を更新します
        {
            ProcessInput(
                &cellAPos,
                &(mainSurface.type),
                &(mainSurface.viewRect.posTopLeft),
                &(subSurface.viewRect.posTopLeft));
        }

        // 描画
        //
        // Renderer モジュールを用いて Cell を描画します。
        //
        {
            NNS_G2dBeginRendering( &render );
                // セル A: 初期値ではメイン画面に表示。キー操作により移動可能。
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( cellAPos.x, cellAPos.y, 0 );
                    NNS_G2dDrawCellAnimation( spCellAnim );
                NNS_G2dPopMtx();

                // セル B: 初期値ではメイン画面に表示。位置固定。
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( CELL_B_POS.x, CELL_B_POS.y, 0 );
                    NNS_G2dDrawCellAnimation( spCellAnim );
                NNS_G2dPopMtx();

                // セル C: 初期値ではサブ画面に表示。位置固定。
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( CELL_C_POS.x, CELL_C_POS.y, 0 );
                    NNS_G2dDrawCellAnimation( spCellAnim );
                NNS_G2dPopMtx();
            NNS_G2dEndRendering();
        }

        // 表示情報の出力
        {
            G2DDemo_PrintOutf(0, 0, "cell pos: (%3d, %3d)",
                cellAPos.x >> FX32_SHIFT, cellAPos.y >> FX32_SHIFT);
            G2DDemo_PrintOut(0, 1, "main surface:");
            PrintSurfaceRect(1, 2, &mainSurface);
            G2DDemo_PrintOut(0, 3, "sub surface:");
            PrintSurfaceRect(1, 4, &subSurface);

            G2DDemo_PrintOut(0, 23, (mainSurface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite");
        }

        // V Blank 待ち
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();


        //
        // バッファの内容をHWに書き出します
        //
        {
            // 表示情報
            G2DDemo_PrintApplyToHW();

            // Cell
            NNS_G2dApplyAndResetOamManagerBuffer( &sMainOamMan );
            NNS_G2dApplyAndResetOamManagerBuffer( &sSubOamMan );
        }

        NNS_G2dTickCellAnimation( spCellAnim, FX32_ONE );
    }
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Vブランク割り込みを処理します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );                   // checking VBlank interrupt
}



/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_PerfCheck
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      Renderer モジュールのパフォーマンスを比較します。
//
//      ･Renderer を用いてマルチセルアニメーションを描画し描画にかかった時間を
//       表示します。
//
//      ･コンパイルスイッチ USE_OPZ_API(規定値 0)が定義されています。
//       USE_OPZ_API を 1に定義すると、最適化ヒントをサポートする、
//       NNS_G2dBeginRenderingEx()を使用します。
//       (ただし、NNS_G2dBeginRenderingEx()未サポートのバージョンではビルド不可
//       となります。)
//
//      ･Aボタンを押すともっとも高速に描画が可能な NNS_G2dMakeSimpleMultiCellTo
//       Oams()での描画へ切り替わります。
//       NNS_G2dMakeSimpleMultiCellToOams()描画時にはソフトウェアスプライト描画
//       は行われません。
//
// 
//  操作方法
//      START ボタン    Cell の出力方法を OBJ <=> SoftwareSprite と
//                      切り替えます。
//      X ボタン        Cell の拡大縮小移動回転状態をリセットします。
//      十字キー        Cell を移動します。
//      A ボタン        レンダラを使用した描画と、
//                      NNS_G2dMakeSimpleMultiCellToOams() 関数を利用した描画を
//                      切り替えます。
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// 定数定義
#define SCREEN_WIDTH                256                     // 画面幅
#define SCREEN_HEIGHT               192                     // 画面高さ

#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell の初期位置 X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell の初期位置 Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // 出力方法の初期値

#define NUM_OF_OAM                  128      // OAM マネージャに割り当てる OAM の数

#define CHARA_BASE                  0x0      // キャラクタイメージベースアドレス
#define PLTT_BASE                   0x0      // パレットベースアドレス

//
// 本定義を１にすると2004/11/10版より新しく追加された、最適化APIを使用します。
// （ただし、最適化APIがサポートされないバージョンではコンパイルが不可能になります。
//   また、アフィンパラメータに対する操作も反映されなくなります。）
//
#define USE_OPZ_API 1

//------------------------------------------------------------------------------
// 構造体定義

typedef struct CellInfo
{
    u16          cellIdx;   // 描画 Cell 番号
    u16          rotate;    // 回転角
    NNSG2dFVec2  scale;     // 拡大率
    NNSG2dFVec2  pos;       // Cell 描画位置
} CellInfo;


//------------------------------------------------------------------------------
// グローバル変数

static NNSG2dImageProxy             sImageProxy;    // Cell 用キャラクタ/テクスチャプロキシ
static NNSG2dImagePaletteProxy      sPaletteProxy;  // Cell 用パレットプロキシ

static NNSG2dOamManagerInstance*    spCurrentOam;   // コールバック処理対象OAMマネージャ

static NNSG2dAnimBankData*          spAnimBank;     // アニメーションバンク
static NNSG2dCellAnimation*         spCellAnim;     // セルアニメーション


//------------------------------------------------------------------------------
// プロトタイプ宣言

void VBlankIntr(void);
static void ProcessInput(CellInfo* pCellInfo, NNSG2dSurfaceType* pOutputType, u16 numCells);
static void MakeAffineMatrix(MtxFx22* pMtx, MtxFx22* pMtx2D, const CellInfo* pCellInfo);
static BOOL CallBackAddOam( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine );
static u16 CallBackAddAffine( const MtxFx22* mtx );
static void InitOamManager(NNSG2dOamManagerInstance*   pObjOamManager);
static void InitRenderer(
                NNSG2dRendererInstance* pRenderer,
                NNSG2dRenderSurface*    pSurface);


/*---------------------------------------------------------------------------*
  Name:         CallBackAddOam

  Description:  NNS_G2dDraw* 関数内部で OAM を追加するために呼ばれる関数です。

  Arguments:    pOam:           追加すべき OAM へのポインタ。
                affineIndex:    この OAM が使用する Affine インデックス。
                                32 以上の値である可能性があります。
                                Affine を未使用の場合は
                                NNS_G2D_OAM_AFFINE_IDX_NONE を指定します。
                bDoubleAffine:  倍角Affineモードかどうか。

  Returns:      成功した場合は TRUE、そうでなければ FALSE を返します。
 *---------------------------------------------------------------------------*/
static BOOL CallBackAddOam( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine )
{
#pragma unused( bDoubleAffine )
    SDK_NULL_ASSERT( spCurrentOam );
    SDK_NULL_ASSERT( pOam );

    return NNS_G2dEntryOamManagerOamWithAffineIdx( spCurrentOam, pOam, affineIndex );
}

/*---------------------------------------------------------------------------*
  Name:         CallBackAddAffine

  Description:  NNS_G2dDraw* 関数内部で Affine パラメータ を追加するために
                呼ばれる関数です。

  Arguments:    mtx:        追加すべき Affine 変換行列へのポインタ

  Returns:      追加した Affine パラメータを参照するための AffineIndex を
                返します。。
 *---------------------------------------------------------------------------*/
static u16 CallBackAddAffine( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( spCurrentOam );
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( spCurrentOam, mtx );
}

/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAM マネージャシステムの初期化と、1つのOAMマネージャ
                実体の初期化を行います。

  Arguments:    pOamManager:    初期化する OAM マネージャへのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitOamManager(NNSG2dOamManagerInstance*   pOamManager)
{
    BOOL result;
    SDK_NULL_ASSERT( pOamManager );

    NNS_G2dInitOamManagerModule();

    result = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode(
                pOamManager, 0, NUM_OF_OAM, NNS_G2D_OAMTYPE_MAIN );
    SDK_ASSERT( result );
}

/*---------------------------------------------------------------------------*
  Name:         InitRenderer

  Description:  Renderer と Surface を初期化します。

  Arguments:    pRenderer:  初期化する Renderer へのポインタ。
                pSurface:   初期化する Surface へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitRenderer(
                NNSG2dRendererInstance* pRenderer,
                NNSG2dRenderSurface*    pSurface)
{
    NNSG2dViewRect* pRect = &(pSurface->viewRect);
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pSurface );

    NNS_G2dInitRenderer( pRenderer );
    NNS_G2dInitRenderSurface( pSurface );
    

    pRect->posTopLeft.x = FX32_ONE * 0;
    pRect->posTopLeft.y = FX32_ONE * 0;
    pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
    pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

    pSurface->pFuncOamRegister       = CallBackAddOam;
    pSurface->pFuncOamAffineRegister = CallBackAddAffine;

    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );
    NNS_G2dSetRendererSpriteZoffset( pRenderer, -FX32_ONE >> 2 );

}


/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  キー入力にしたがって、Cell の表示状態を変更します。
                表示 Cell の切り替え、状態リセットも行います。

  Arguments:    pCellInfo:      キー入力を反映させる CellInfo へのポインタ
                pOutputType:    出力方法を格納したバッファへのポインタ
                numCells:       選択可能な Cell の数

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ProcessInput(CellInfo* pCellInfo, NNSG2dSurfaceType* pOutputType, u16 numCells)
{
    SDK_NULL_ASSERT( pCellInfo );
    SDK_NULL_ASSERT( pOutputType );

    G2DDemo_ReadGamePad();

    // Change current Cell
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
    {
       pCellInfo->cellIdx++;
        if( pCellInfo->cellIdx >= numCells )
        {
            pCellInfo->cellIdx = 0;
        }
    }

    // Move Cell
    if( ! G2DDEMO_IS_PRESS(PAD_BUTTON_B) && ! G2DDEMO_IS_PRESS(PAD_BUTTON_Y) )
    {
        if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
        {
            pCellInfo->pos.y -= FX32_ONE;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
        {
            pCellInfo->pos.y += FX32_ONE;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
        {
            pCellInfo->pos.x -= FX32_ONE;
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
        {
            pCellInfo->pos.x += FX32_ONE;
        }
    }

    // Reset
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
    {
        pCellInfo->rotate      = 0;
        pCellInfo->scale       = (NNSG2dFVec2){FX32_ONE, FX32_ONE};
        pCellInfo->pos         = (NNSG2dFVec2){CELL_INIT_POS_X, CELL_INIT_POS_Y};
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
  Name:         LoadResources

  Description:  マルチセルアニメーションの構築とキャラクタデータ、
                パレットデータの読み込みを行います。

  Arguments:    ppMCAnim:   構築したマルチセルアニメーションへのポインタを
                            受け取るポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void LoadResources(NNSG2dMultiCellAnimation** ppMCAnim)
{
    void* pBuf;
    
    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );
    
    //
    // MultiCell 関連初期化
    //
    {
        NNSG2dCellDataBank*             pCellBank = NULL;   // セルデータ
        NNSG2dCellAnimBankData*         pAnimBank = NULL;   // セルアニメーション
        NNSG2dMultiCellDataBank*        pMCBank = NULL;     // マルチセルデータ
        NNSG2dMultiCellAnimBankData*    pMCABank = NULL;    // マルチセルアニメーション

        // セルデータ、セルアニメーション、マルチセルデータ、
        // マルチセルアニメーションをロード。
        // これらは最後までメインメモリ上で使用するので pBuf を解放しません。
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/MultiCellAnimation.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/MultiCellAnimation.NANR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMCR( &pMCBank, "data/MultiCellAnimation.NMCR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMAR( &pMCABank, "data/MultiCellAnimation.NMAR" );
        SDK_NULL_ASSERT( pBuf );


        //
        // マルチセルアニメーションの実体を初期化します
        //
        {
            const NNSG2dMultiCellAnimSequence* pSequence;

            // 再生するシーケンスを取得
            pSequence = NNS_G2dGetAnimSequenceByIdx( pMCABank, 0 );
            SDK_ASSERT( pSequence );

            // マルチセルアニメーションを構築
            *ppMCAnim = G2DDemo_GetNewMultiCellAnimation( pAnimBank,
                                                          pCellBank,
                                                          pMCBank,
                                                          NNS_G2D_MCTYPE_SHARE_CELLANIM );
            SDK_NULL_ASSERT( *ppMCAnim );


            // マルチセルアニメーションに再生するシーケンスをセット
            NNS_G2dSetAnimSequenceToMCAnimation( *ppMCAnim, pSequence);
        }
    }

    //
    // VRAM 関連の初期化
    //
    {
        //------------------------------------------------------------------------------
        // load character data for 2D
        {
            NNSG2dCharacterData* pCharData;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/MultiCellAnimation.NCGR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            NNS_G2dLoadImage2DMapping(
                pCharData,
                CHARA_BASE,
                NNS_G2D_VRAM_TYPE_2DMAIN,
                &sImageProxy );

            // キャラクタデータを VRAM にコピーしたので
            // この pBuf は開放します。以下同じ。
            G2DDemo_Free( pBuf );
        }

        // load character data for 3D (software sprite)
        {
            NNSG2dCharacterData* pCharData;

            pBuf = G2DDemo_LoadNCBR( &pCharData, "data/MultiCellAnimation.NCBR" );
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

            pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/MultiCellAnimation.NCLR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            NNS_G2dLoadPalette(
                pPlttData,
                PLTT_BASE,
                NNS_G2D_VRAM_TYPE_2DMAIN,
                &sPaletteProxy);

            // Loading For 3D Graphics Engine.
            NNS_G2dLoadPalette(
                pPlttData,
                PLTT_BASE,
                NNS_G2D_VRAM_TYPE_3DMAIN,
                &sPaletteProxy);

            G2DDemo_Free( pBuf );
        }
    }
}

//------------------------------------------------------------------------------
// レンダラモジュールのAPIを使用して
// マルチセルを描画します。
static void DrawUsingRendererAPI
(
    NNSG2dRendererInstance*         pRender,       // 描画用 Renderer
    const NNSG2dFVec2*              pPos,          // 位置情報
    const NNSG2dMultiCellAnimation* pMCAnim        // MultiCellAnimation 実体
)
{
    //
    // USE_OPZ_API が定義されていた場合は、
    // 最適化ヒントを与えます。ここでは、
    // アフィン変換を使用しない。
    // パラメータ変更を行わない。を指定しています。
    //            
#if USE_OPZ_API
    NNS_G2dBeginRenderingEx( pRender, NNS_G2D_RDR_OPZHINT_NOT_SR | 
                                      NNS_G2D_RDR_OPZHINT_LOCK_PARAMS );
#else// USE_OPZ_API
    NNS_G2dBeginRendering( pRender );
#endif//USE_OPZ_API

        NNS_G2dPushMtx();

            NNS_G2dTranslate( pPos->x, pPos->y, 0 );
            NNS_G2dDrawMultiCellAnimation( pMCAnim );

        NNS_G2dPopMtx();
    NNS_G2dEndRendering();
}

//------------------------------------------------------------------------------
// NNS_G2dMakeSimpleMultiCellToOams() を使用して
// マルチセルを描画します。
static void DrawUsingSimpleAPI
(
    NNSG2dOamManagerInstance*       pOamManager,   // 2D描画時のOAMマネージャ
    const NNSG2dFVec2*              pPos,          // 位置情報
    const NNSG2dMultiCellAnimation* pMCAnim        // MultiCellAnimation 実体
)
{
    static GXOamAttr    temp[NUM_OF_OAM];   
    u16                 numOamDrawn = 0;           // 描画 OAM 数
    //
    // MultiCell と等価な OBJリストをテンポラリに書き出します
    //
    numOamDrawn = NNS_G2dMakeSimpleMultiCellToOams(
                        temp,                           // 出力先Oamバッファ
                        NUM_OF_OAM,                     // 出力先バッファ長
                        &pMCAnim->multiCellInstance,    // 出力対象 MultiCell
                        NULL,                           // Affine 変換
                        pPos,                           // オフセット位置
                        NULL,                           // Affine Index
                        FALSE );                        // 倍角Affineか？

    SDK_ASSERT( numOamDrawn < NUM_OF_OAM );

    // OAMマネージャへOBJリストを登録します
    (void)NNS_G2dEntryOamManagerOam(
            pOamManager,
            temp,
            numOamDrawn );
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // 描画用 Renderer
    NNSG2dRendererInstance      render;         
    // メイン画面 Surface
    NNSG2dRenderSurface         surface;        
    // 2D描画時のOAMマネージャ
    NNSG2dOamManagerInstance    oamManager;     
    // 描画方法の種類
    NNSG2dSurfaceType           outputType = INIT_OUTPUT_TYPE;
    // マルチセルアニメーション 実体
    NNSG2dMultiCellAnimation*   pMCAnim = NULL; 
    
    //------------------------------------------
    // アプリケーションの初期化
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        InitRenderer( &render, &surface );
        LoadResources( &pMCAnim );
        
        spCurrentOam = &oamManager;
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
        // レンダラを使用して描画をおこなうか？
        static BOOL     bUsingRenderer = TRUE;
        // 描画にかかる時間
        static OSTick   timeDraw       = 0;
        // セルの位置情報など
        static CellInfo cellInfo = {
            0,                                  // 描画 Cell 番号
            0,                                  // 回転角
            {FX32_ONE, FX32_ONE},               // 拡大率
            {CELL_INIT_POS_X, CELL_INIT_POS_Y}  // Cell 描画位置
        };

        //------------------------------------------
        // 入力を読み取り Cell の表示情報と出力方法を更新します
        {
            ProcessInput(&cellInfo, &outputType, 0);
            surface.type = outputType;
            
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
            {
                bUsingRenderer = bUsingRenderer^TRUE;
            }
        }
        

        //------------------------------------------
        // 描画
        // 描画処理にかかった時間を計測しています
        {
            timeDraw = OS_GetTick();
            
            // レンダラを使用するか？
            if( bUsingRenderer )
            {
                DrawUsingRendererAPI( &render, &cellInfo.pos, pMCAnim );
            }else{
                //
                // ソフトウエアスプライト描画ならば処理しない。
                //
                if( outputType != NNS_G2D_SURFACETYPE_MAIN3D )
                {
                    DrawUsingSimpleAPI( &oamManager, &cellInfo.pos, pMCAnim );
                }
            }
                
            timeDraw = OS_GetTick() - timeDraw;
        }
        
        
        //------------------------------------------
        // 表示情報の出力
        {
            // 画面上部
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);
            
            // 画面下部
            G2DDemo_PrintOutf( 0, 21, "TIME-draw  :%06ld usec\n", OS_TicksToMicroSeconds(timeDraw) );
            G2DDemo_PrintOut(0, 22, (outputType == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite");
            G2DDemo_PrintOut(0, 23, (bUsingRenderer) ?
                   "UsingRenderer           ": "UsingMakeCellToOams");
            
            // NNS_G2dMakeSimpleMultiCellToOams()描画時にはソフトウェアスプライト描画は行われません。
            if( outputType != NNS_G2D_SURFACETYPE_MAIN2D && !bUsingRenderer )
            {
                G2DDemo_PrintOutf( 5, 10, "Not supported." );
            }else{
                G2DDemo_PrintOutf( 5, 10, "              " );
            }
        }
        
        //------------------------------------------
        // V Blank 待ち
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();

        //------------------------------------------
        //
        // バッファの内容をHWに書き出します
        //
        {
            // 表示情報
            G2DDemo_PrintApplyToHW();

            // Cell
            NNS_G2dApplyOamManagerToHW( &oamManager );
            NNS_G2dResetOamManagerBuffer( &oamManager );
        }
        
        //------------------------------------------
        //
        // アニメーションを更新します
        //
        NNS_G2dTickMCAnimation( pMCAnim, FX32_ONE );
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



/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - RendererCore2
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      RendererCore モジュールを用いてセルアニメーションを描画するサンプル
//      
//      ･RendererCore1とほぼ同一の内容を持つサンプルデモです。
//
//      ･レンダラモジュールを利用しながら、必要に応じてレンダラコアモジュールAPIを
//       利用するようなケースをデモしています。
//       (レンダラモジュールは内部でレンダラコアモジュールを使用しているため、
//        上記のような運用が可能です）      
//  
//      ･レンダラモジュールAPI
//       レンダラコアモジュールAPI
//       その他描画API のパフォーマンスの比較を行います。
//
//  操作方法
//      START ボタン    Cell の出力方法を OBJ <=> SoftwareSprite と
//                      切り替えます。
//                      SoftwareSprite の場合 MakeCellToOamsでの描画は行われません。
//      R ボタン        表示する Cell を切り替えます。(通常のセルと 単一画像から構成されるセル)
//      L ボタン        描画方法を変更します。
//      十字キー        Cell を移動します。(同時にレンダラのフリップ状態も変更します。
//                                          MakeCellToOams関数 では反映されません。)
//
// 
//  ソフトウェアスプライト描画でレンダラコアは 
//  単一画像から構成されるセルに特化した描画関数を使用して描画をおこないますので、
//  通常のセルを描画した場合、正しく描画されない不具合を確認することができます。
//  その場合は R ボタン で 表示セルを 単一画像から構成されるセル に 変更してみてください。
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


#define SCREEN_WIDTH                256                     // 画面幅
#define SCREEN_HEIGHT               192                     // 画面高さ

#define CELL_INIT_POS_X             (60 << FX32_SHIFT)     // Cell の初期位置 X
#define CELL_INIT_POS_Y             (60 << FX32_SHIFT)     // Cell の初期位置 Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // 出力方法の初期値

#define NUM_OF_OAM                  128                 // OAM マネージャに割り当てる OAM の数

#define CHARA_BASE                  0x0                 // キャラクタイメージベースアドレス
#define PLTT_BASE                   0x0                 // パレットベースアドレス



//------------------------------------------------------------------------------
// 描画セルに関する定数
// NUM_CELL_X * NUM_CELL_Y が 描画セル数ですが、総OBJ数が128を超えた場合は、
// MyDrawMethod == MYDRAWMETHOD_CELLTOOAM 以外の場合、
// 処理が中断せず正確なパフォーマンス比較ができなくなるので注意が必要です。
// 
#define NUM_CELL_X 5        
#define NUM_CELL_Y 5
#define STEP_CELL_X 24
#define STEP_CELL_Y 24
  
//------------------------------------------------------------------------------
// 構造体定義

typedef struct CellInfo
{
    NNSG2dFVec2                 scale;     // 拡大率
    NNSG2dFVec2                 pos;       // Cell 描画位置
    NNSG2dCellAnimation*        pCellAnim; // セルアニメ
} CellInfo;

typedef enum 
{
    MYDRAWMETHOD_RENDERER,
    MYDRAWMETHOD_RENDERERCORE,
    MYDRAWMETHOD_CELLTOOAM,
    MYDRAWMETHOD_MAX

}MyDrawMethod;

const char*         drawTypeStrTbl[MYDRAWMETHOD_MAX]=
{
    "MYDRAWMETHOD_RENDERER    ",
    "MYDRAWMETHOD_RENDERERCORE",
    "MYDRAWMETHOD_CELLTOOAM   ",

};

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
static NNSG2dCellDataBank* LoadResources(void);
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
                返します。
 *---------------------------------------------------------------------------*/
static u16 CallBackAddAffine( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( spCurrentOam );
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( spCurrentOam, mtx );
}

/*---------------------------------------------------------------------------*
  Name:         CallBackAddAffine

  Description:  レンダラコア 描画関数内部で OAM を追加するために
                呼ばれる関数です。
                複数OAMを一括して登録します。
                EndRendering のタイミングで一回だけ呼ばれます。

  Arguments:    pOam:           追加すべき OAM列 へのポインタ。
                num:            追加するOAM個数。

  Returns:      成功した場合は TRUE、そうでなければ FALSE を返します。
 *---------------------------------------------------------------------------*/
static BOOL CallBackAddOamBlock( const GXOamAttr* pOam, u16 num )
{
    return NNS_G2dEntryOamManagerOam( spCurrentOam, pOam, num );
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

//------------------------------------------------------------------------------
static void ResetRendererSurface( NNSG2dRenderSurface*    pSurface )
{
    SDK_NULL_ASSERT( pSurface );
    {
        NNSG2dViewRect* pRect = &(pSurface->viewRect);
        
        pRect->posTopLeft.x = FX32_ONE * 0;
        pRect->posTopLeft.y = FX32_ONE * 0;
        pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
        pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

        pSurface->pFuncOamRegister       = CallBackAddOam;
        pSurface->pFuncOamAffineRegister = CallBackAddAffine;
        
        pSurface->type                   = NNS_G2D_SURFACETYPE_MAIN2D;
    }
    
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
                NNSG2dRenderSurface*    pSurface )
{
    NNSG2dViewRect* pRect = &(pSurface->viewRect);
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pSurface );

    NNS_G2dInitRenderer( pRenderer );
    
    NNS_G2dInitRenderSurface( pSurface );
    
    ResetRendererSurface( pSurface );
    
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );
    
}


/*---------------------------------------------------------------------------*
  Name:         LoadResource

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
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/RendererCore.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // セルバンクは最後までメインメモリ上で使用するので
        // この pBuf は開放しません。
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // セルアニメーションデータは最後までメインメモリ上で
        // 使用するのでこの pBuf は開放しません。
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/RendererCore.NANR" );
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

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/RendererCore.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadImage1DMapping(
            pCharData,
            CHARA_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sImageProxy );

        // キャラクタデータを VRAM にコピーしたので
        // この pBuf は開放します。以下同じ。
        G2DDemo_Free( pBuf );
    }

    // load charactor data for 3D (software sprite)
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/RendererCore.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadImage1DMapping(
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

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/RendererCore.NCLR" );
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

    return pCellBank;
}

//------------------------------------------------------------------------------
static void MtxSetIdentity( MtxFx32* pM )
{
    pM->_00 = FX32_ONE;
    pM->_01 = 0;
    pM->_10 = 0;
    pM->_11 = FX32_ONE;
    pM->_20 = 0;
    pM->_21 = 0;
}

//------------------------------------------------------------------------------
// レンダラモジュールを使用して描画を行います。
static void DrawByRenderer
( 
    NNSG2dRendererInstance*     pRdr, 
    const CellInfo*             pCellInf 
)
{
    int                         i,j;
    //
    // NNS_G2dBeginRendering()の代わりに最適化オプションを指定する
    // NNS_G2dBeginRenderingEx を 使用しています。
    //
    NNS_G2dBeginRenderingEx( pRdr, NNS_G2D_RDR_OPZHINT_NOT_SR | 
                                   NNS_G2D_RDR_OPZHINT_LOCK_PARAMS );
                                   
    for( i = 0; i < NUM_CELL_X; i++ )
    {
        for( j = 0; j < NUM_CELL_Y; j++ )
        {
            //
            // 本デモの場合ならば、Push Pop は不要です。
            // そのような場合は、下記のように記述すると処理効率がよくなります。
            //
            NNS_G2dSetTrans( pCellInf->pos.x + FX32_ONE * STEP_CELL_X*i, 
                             pCellInf->pos.y + FX32_ONE * STEP_CELL_Y*j, 
                                  0 );
            NNS_G2dDrawCellAnimation( pCellInf->pCellAnim );
        }
    }
    NNS_G2dEndRendering();
}          
  


//------------------------------------------------------------------------------
// レンダラコアモジュールのAPIを使用して描画を行います。
// レンダラコアモジュールはレンダラモジュールの描画処理を下請けするモジュールです。
// レンダラコアモジュールを直接使用する場合は、
// レンダラモジュールが行っている、行列スタック管理、パラメータ書き換え、等を自らが行う必要があります。
static void DrawByRndCore
( 
    NNSG2dRendererInstance*     pRdr, 
    const CellInfo*             pCellInf 
)
{
    NNSG2dRndCoreInstance*    pRndCore = &pRdr->rendererCore;
    NNSG2dRenderSurface*      pSurface = pRdr->pTargetSurfaceList;
    MtxFx32                   mtx;
    int                       i,j;
    
    MtxSetIdentity( &mtx );
    
    
    //
    // レンダコアに、サーフェス設定を行う
    //
    NNS_G2dSetRndCoreSurface( pRndCore, &pSurface->coreSurface );
    
    //
    // 2D サーフェスに対する描画ならば...
    //
    if( pSurface->type != NNS_G2D_SURFACETYPE_MAIN3D )
    {
        //
        // 2D 描画用に登録関数の設定
        //
        NNS_G2dSetRndCoreOamRegisterFunc  ( pRndCore, 
                                            CallBackAddOam,
                                            CallBackAddAffine );

        NNS_G2dRndCoreBeginRendering( pRndCore );
        for( i = 0; i < NUM_CELL_X; i++ )
        {
            for( j = 0; j < NUM_CELL_Y; j++ )
            {
                // 描画位置の設定をレンダラコアに設定する行列に対して行います。
                mtx._20 = pCellInf->pos.x + FX32_ONE * STEP_CELL_X*i;
                mtx._21 = pCellInf->pos.y + FX32_ONE * STEP_CELL_Y*j;
                //
                // レンダラにマトリクスキャッシュを設定します。
                // (事前にZ値を設定しておく必要があります。)
                //
                NNS_G2dSetRndCoreCurrentMtx2D( &mtx, NULL );
                //
                // レンダコアAPIを使用して描画処理をおこないます。
                //
                NNS_G2dRndCoreDrawCellFast2D( NNS_G2dGetCellAnimationCurrentCell( pCellInf->pCellAnim ) );
            }
        }
        NNS_G2dRndCoreEndRendering( );
    }else{
        //
        // 3D 用 Z値を設定します。
        //
        NNS_G2dSetRndCore3DSoftSpriteZvalue( pRndCore, -1 );
        NNS_G2dRndCoreBeginRendering( pRndCore );
        NNS_G2dSetRndCoreCellCloneSource3D( NNS_G2dGetCellAnimationCurrentCell( pCellInf->pCellAnim ) );
        for( i = 0; i < NUM_CELL_X; i++ )
        {
            for( j = 0; j < NUM_CELL_Y; j++ )
            {
                // 描画位置の設定をレンダラコアに設定する行列に対して行います。
                mtx._20 = pCellInf->pos.x + FX32_ONE * STEP_CELL_X*i;
                mtx._21 = pCellInf->pos.y + FX32_ONE * STEP_CELL_Y*j;
                //
                // レンダラにマトリクスキャッシュを設定します。
                // (事前にZ値を設定しておく必要があります。)
                //
                NNS_G2dSetRndCoreCurrentMtx3D( &mtx );
                //
                // レンダコアAPIを使用して描画処理をおこないます。
                // 同一のスプライトを高速に描画するためのAPIを使用します。
                //
                NNS_G2dRndCoreDrawCellClone3D( NNS_G2dGetCellAnimationCurrentCell( pCellInf->pCellAnim ) );
            }
        }
        NNS_G2dRndCoreEndRendering( );
    }
}

//------------------------------------------------------------------------------
// NNS_G2dMakeCellToOams を使用して描画を行います。
// もっとも高速に動作しますが、付加機能が少なくなっています。
static void DrawByMakeOamsFunc
( 
    NNSG2dOamManagerInstance*   pOamMgr, 
    const CellInfo*             pCellInf 
)
{   
    static GXOamAttr          temp[NUM_OF_OAM];
    u16                       numOamBuffer = NUM_OF_OAM;                  // 残り OAM 数
    u16                       numDrawn;
    GXOamAttr*                pBuffer = temp;
    NNSG2dFVec2               trans;
    int                       i,j;
    
    
    
    
    for( i = 0; i < NUM_CELL_X; i++ )
    {
        for( j = 0; j < NUM_CELL_Y; j++ )
        {
            trans.x = pCellInf->pos.x + FX32_ONE * STEP_CELL_X*i;
            trans.y = pCellInf->pos.y + FX32_ONE * STEP_CELL_Y*j;
            
            if( numOamBuffer <= 0 )
            {
                return;
            }
            //
            // MultiCell と等価な OBJリストをテンポラリに書き出します
            //
            numDrawn = NNS_G2dMakeCellToOams(
                                pBuffer,           // 出力先Oamバッファ
                                numOamBuffer,   // 出力先バッファ長
                                NNS_G2dGetCellAnimationCurrentCell( pCellInf->pCellAnim ), // 出力対象 Cell
                                NULL,           // Affine 変換
                                &trans,         // オフセット位置
                                NULL,           // Affine Index
                                FALSE );        // 倍角Affineか？
            
            numOamBuffer    -= numDrawn;
            pBuffer         += numDrawn;
            
            SDK_ASSERT(numOamBuffer < NUM_OF_OAM);
        }
    }
    // OAMマネージャへOBJリストを登録します
    (void)NNS_G2dEntryOamManagerOam(
            pOamMgr,
            temp,
            (u16)( NUM_OF_OAM - numOamBuffer ) );
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    NNSG2dRendererInstance      render;         // 描画用 Renderer
    NNSG2dRenderSurface         surface;        // メイン画面 Surface
    NNSG2dCellDataBank*         pCellBank;      // Cell データ
    NNSG2dOamManagerInstance    oamManager;     // 2D描画時のOAMマネージャ
    BOOL                        bDrawByCore = FALSE;
    MyDrawMethod                drawMethod = MYDRAWMETHOD_RENDERERCORE;
    
    OSTick                      time;
    u16                         anmSeqIdx = 0;
    
    // Initilizing App.
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        InitRenderer( &render, &surface );
        pCellBank = LoadResources();
        
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
        static CellInfo cellInfo = {
            {FX32_ONE, FX32_ONE},               // 拡大率
            {CELL_INIT_POS_X, CELL_INIT_POS_Y},  // Cell 描画位置
            NULL
        };

        cellInfo.pCellAnim = spCellAnim;
        //------------------------------------------------------------------------------
        // 入力を読み取り Cell の表示情報と出力方法を更新します
        {
            G2DDemo_ReadGamePad();
            
            // Move Cell
            {
                if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
                {
                    cellInfo.pos.y -= FX32_ONE;
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
                {
                    cellInfo.pos.y += FX32_ONE;
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
                {
                    cellInfo.pos.x -= FX32_ONE;
                    NNS_G2dSetRendererFlipMode( &render, FALSE, FALSE );
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
                {
                    cellInfo.pos.x += FX32_ONE;
                    NNS_G2dSetRendererFlipMode( &render, TRUE, FALSE );
                }
            }

            // Reset
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
            {
                cellInfo.pos = (NNSG2dFVec2){CELL_INIT_POS_X, CELL_INIT_POS_Y};
            }

            // Toggle Output Type
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_START) )
            {
                surface.type = (surface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                                    NNS_G2D_SURFACETYPE_MAIN3D:
                                    NNS_G2D_SURFACETYPE_MAIN2D;
            }
            
        
            // 描画方法の変更
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_L) )
            {
                drawMethod = (MyDrawMethod)(++drawMethod % MYDRAWMETHOD_MAX);
                //
                // 再度サーフェスの初期化を行う
                // レンダラモジュール内部で設定しているレンダコア用コールバック等をリセットするため。
                //
                {
                    if( drawMethod == MYDRAWMETHOD_RENDERER )
                    {
                        NNS_G2dInitRenderSurface( &surface );
                        ResetRendererSurface( &surface );
                        
                    }else if( drawMethod == MYDRAWMETHOD_RENDERERCORE ){
                        
                        NNS_G2dInitRndCoreSurface( &surface.coreSurface );
                        ResetRendererSurface( &surface );
                    }
                }
            }
        
            //
            // アニメ切り替え
            //
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_R) )
            {
                anmSeqIdx++;
                anmSeqIdx = (u16)(anmSeqIdx % NNS_G2dGetNumAnimSequence( spAnimBank ));
                
                NNS_G2dInitCellAnimation( spCellAnim, 
                                          NNS_G2dGetAnimSequenceByIdx(spAnimBank, anmSeqIdx ),
                                          pCellBank );
            }
        }
        
        
        //------------------------------------------------------------------------------
        // 描画
        //
        // 様々な描画APIを用いて Cell を描画します。
        //
        time = OS_GetTick();   
        {
            switch( drawMethod )
            {
            case MYDRAWMETHOD_RENDERER:
                //
                // レンダラによる描画
                //                
                DrawByRenderer( &render, &cellInfo );break;            
            case MYDRAWMETHOD_RENDERERCORE:
                //
                // レンダラコアによる描画
                //
                DrawByRndCore( &render, &cellInfo );break;            
            case MYDRAWMETHOD_CELLTOOAM:
                //
                // NNS_G2dMakeCellToOams() を使用した描画
                //     ソフトウェアスプライト描画の場合は処理しません。
                if( surface.type != NNS_G2D_SURFACETYPE_MAIN3D )
                {
                    DrawByMakeOamsFunc( &oamManager, &cellInfo );break;
                }
            }
        }
        time = OS_GetTick() - time;
        
        //------------------------------------------------------------------------------
        // 表示情報の出力
        {
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);   
            G2DDemo_PrintOut(0, 1, drawTypeStrTbl[drawMethod] );
            G2DDemo_PrintOut(0, 2, (surface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite"); 
            
            G2DDemo_PrintOutf( 0, 22, "OBJ_CNT:%04ld", 
                cellInfo.pCellAnim->pCurrentCell->numOAMAttrs * NUM_CELL_X * NUM_CELL_Y );
            G2DDemo_PrintOutf( 0, 23, "TIME   :%06ld usec\n", OS_TicksToMicroSeconds(time) );
            
            // NNS_G2dMakeSimpleMultiCellToOams()描画時にはソフトウェアスプライト描画は行われません。
            if( surface.type == NNS_G2D_SURFACETYPE_MAIN3D && drawMethod == MYDRAWMETHOD_CELLTOOAM )
            {
                G2DDemo_PrintOutf( 5, 10, "Not supported." );
            }else{
                G2DDemo_PrintOutf( 5, 10, "              " );
            }
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
            NNS_G2dApplyOamManagerToHW( &oamManager );
            NNS_G2dResetOamManagerBuffer( &oamManager );
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

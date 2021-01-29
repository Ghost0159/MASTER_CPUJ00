/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - RendererCore
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      レンダラコア モジュールを単独で用いてセルアニメーションを描画するサンプル
//      ･レンダラコア モジュールを使用して、2D 描画と 3D描画を行います。
//      ･3D描画 は 同一のスプライトを高速に描画するためのAPIを使用して描画を行います。
//       (そのため、複数種類のOBJから構成されるセルでは描画が正しく行われません)

//      ･レンダラコア モジュールの使用方法については、ライブラリコードのレンダラモ
//       ジュール内のコードも参考になります。（特に振る舞いのカスタマイズなど...）
//      
//  操作方法
//      START ボタン    Cell の出力方法を OBJ <=> SoftwareSprite と
//                      切り替えます。
//                      SoftwareSprite の場合 MakeCellToOamsでの描画は行われません。
//      R ボタン        表示する Cell を切り替えます。(通常のセルと 単一画像から構成されるセル)
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

//------------------------------------------------------------------------------
// 定数定義
#define SCREEN_WIDTH                256                     // 画面幅
#define SCREEN_HEIGHT               192                     // 画面高さ

#define CELL_INIT_POS_X             (60 << FX32_SHIFT)     // Cell の初期位置 X
#define CELL_INIT_POS_Y             (60 << FX32_SHIFT)     // Cell の初期位置 Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // 出力方法の初期値

#define NUM_OF_OAM                  128                 // OAM マネージャに割り当てる OAM の数

#define CHARA_BASE                  0x0                 // キャラクタイメージベースアドレス
#define PLTT_BASE                   0x0                 // パレットベースアドレス

// 描画セルに関する定数
// NUM_CELL_X * NUM_CELL_Y が 描画セル数です。
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
static void ResetRendererSurface( NNSG2dRndCoreSurface*    pSurface )
{
    SDK_NULL_ASSERT( pSurface );
    {
        NNSG2dViewRect* pRect = &(pSurface->viewRect);
        
        pRect->posTopLeft.x = FX32_ONE * 0;
        pRect->posTopLeft.y = FX32_ONE * 0;
        pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
        pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;
        
        pSurface->type                   = NNS_G2D_SURFACETYPE_MAIN2D;
    }
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
// レンダラコアモジュールのAPIを使用して描画を行います。
// レンダラコアモジュールはレンダラモジュールの描画処理を下請けするモジュールです。
// レンダラコアモジュールを直接使用する場合は、
// レンダラモジュールが行っている、行列スタック管理、パラメータ書き換え、等を自らが行う必要があります。
static void DrawByRndCore
( 
    NNSG2dRndCoreInstance*     pRndCore, 
    NNSG2dRndCoreSurface*      pSurface,
    const CellInfo*            pCellInf 
)
{
    MtxFx32                   mtx;
    int                       i,j;
    
    MtxSetIdentity( &mtx );
    //
    // レンダコアに、サーフェス設定を行う
    //
    NNS_G2dSetRndCoreSurface( pRndCore, pSurface );
    
    //
    // 2D サーフェスに対する描画ならば...
    //
    if( pSurface->type != NNS_G2D_SURFACETYPE_MAIN3D )
    {
        //
        // 2D 描画用に登録関数の設定
        //
        NNS_G2dSetRndCoreOamRegisterFunc( pRndCore, 
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
                // レンダラに行列を設定します。
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
       
        // 3D 用 Z値を設定します。
        NNS_G2dSetRndCore3DSoftSpriteZvalue( pRndCore, -1 );
        NNS_G2dRndCoreBeginRendering( pRndCore );
        // 同一のスプライトを高速に描画するためのAPIを使用します。
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
                // (事前にZ値を
                // NNS_G2dSetRndCore3DSoftSpriteZvalue() で設定しておく必要があります。)
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




/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    NNSG2dRndCoreInstance       rndCore;        // レンダラコア
    NNSG2dRndCoreSurface        surface;        // レンダラコアサーフェス
    
    NNSG2dCellDataBank*         pCellBank;      // Cell データ
    NNSG2dOamManagerInstance    oamManager;     // 2D描画時のOAMマネージャ
    OSTick                      time;           // パフォーマンス計測用時間
    u16                         anmSeqIdx = 0;  // アニメシーケンス番号
    
    // Initilizing App.
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        
        //
        // レンダラの初期化
        //
        NNS_G2dInitRndCore( &rndCore );
        NNS_G2dInitRndCoreSurface( &surface );
        
        NNS_G2dSetRndCoreOamRegisterFunc( &rndCore, CallBackAddOam, CallBackAddAffine );
        
        ResetRendererSurface( &surface );
        NNS_G2dSetRndCoreImageProxy( &rndCore, &sImageProxy, &sPaletteProxy );
        
        
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
            {CELL_INIT_POS_X, CELL_INIT_POS_Y}, // Cell 描画位置
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
                    NNS_G2dSetRndCoreFlipMode( &rndCore, FALSE, FALSE );
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
                {
                    cellInfo.pos.x += FX32_ONE;
                    NNS_G2dSetRndCoreFlipMode( &rndCore, TRUE, FALSE );
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
        // Renderer モジュールを用いて Cell を描画します。
        //
        time = OS_GetTick();   
        {
            //
            // レンダラコアによる描画
            //
            DrawByRndCore( &rndCore, &surface, &cellInfo );
        }
        time = OS_GetTick() - time;
        
        //------------------------------------------------------------------------------
        // 表示情報の出力
        {
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);   
            G2DDemo_PrintOut(0, 1, (surface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite"); 
            
            G2DDemo_PrintOutf( 0, 22, "OBJ_CNT:%04ld", 
                cellInfo.pCellAnim->pCurrentCell->numOAMAttrs * NUM_CELL_X * NUM_CELL_Y );
            G2DDemo_PrintOutf( 0, 23, "TIME   :%06ld usec\n", OS_TicksToMicroSeconds(time) );
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
        
        // セルアニメーションの更新
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





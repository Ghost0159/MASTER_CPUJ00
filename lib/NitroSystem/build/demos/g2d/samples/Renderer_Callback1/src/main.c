/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_Callback1
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
//      Renderer モジュールのOBJ描画コールバックを用いて
//      各ゲームタイトル固有の特殊処理を行うサンプルです。
//
//      画面に表示されるキャラクタのOBJの表示プライオリティや位置をコールバ
//      ック内で操作します。
//
//  操作方法
//      十字キー        Cell を移動します。
//      B ボタン        押しているとき、コールバックが無効になります。
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// 各種定数定義
#define SCREEN_WIDTH                256                     // 画面幅
#define SCREEN_HEIGHT               192                     // 画面高さ

#define AFFINE_IDX                  3                       // Cell 用 Affine Index
#define CELL_ROTATE_UNIT            0xFF                    // Cell の単位回転量
#define CELL_SCALE_UNIT             ((FX32_ONE >> 7) * 3 )  // Cell の単位拡大縮小率
                                                            // scale 値が 0 付近の値にならないようにしてあります
#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell の初期位置 X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell の初期位置 Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // 出力方法の初期値

#define NUM_OF_OAM                  128                 // OAM マネージャに割り当てる OAM の数

#define CHARA_BASE                  0x0                 // キャラクタイメージベースアドレス
#define PLTT_BASE                   0x0                 // パレットベースアドレス



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

static NNSG2dScreenData*            pScrDataBG1;    // BG用スクリーンデータ
static NNSG2dSVec2                  randomDiff;     // 頭部ランダム移動量

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
                返します。。
 *---------------------------------------------------------------------------*/
static u16 CallBackAddAffine( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( spCurrentOam );
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( spCurrentOam, mtx );
}


/*---------------------------------------------------------------------------*
  Name:         CallBackBeforeOBJ

  Description:  レンダラOBJ描画コールバック：
                レンダラOBJ描画前コールバックとして登録して、各ゲームタイトル固有
                のハッキングを行います。
                ここでは、セルの頭部部分の位置をずらし、下半身部分の描画優先度を変更しています。

  Arguments:    pRend               レンダラ
                pSurface            サーフェス
                pCell               セル
                oamIdx              OAM番号
                pMtx                行列

  Returns:      なし
 *---------------------------------------------------------------------------*/
static void CallBackBeforeOBJ
(
    struct NNSG2dRendererInstance*         pRend,
    struct NNSG2dRenderSurface*            pSurface,
    const NNSG2dCellData*                  pCell,
    u16                                    oamIdx,
    const MtxFx32*                         pMtx
)
{
#pragma unused( pSurface ) 
#pragma unused( pCell ) 
#pragma unused( pMtx ) 
    GXOamAttr*    pTempOam = &pRend->rendererCore.currentOam;
    //
    // パラメータ書き換え
    //
    {
        // 0 -- 3 は 頭部 をあらわす      
        if( oamIdx <= 3 )
        {
            // 位置をずらす
            pTempOam->x += randomDiff.x;                    
            pTempOam->y += randomDiff.y;
        }
        
        // 4と5は下半身 6 は影 をあらわす
        if( oamIdx >= 4 )
        {
            // 描画優先度変更
            pTempOam->priority = 0;
        }
    }
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
    pSurface->type = NNS_G2D_SURFACETYPE_MAIN2D;

    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );
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
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/Rdr_CallBack1.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // セルバンクは最後までメインメモリ上で使用するので
        // この pBuf は開放しません。
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // セルアニメーションデータは最後までメインメモリ上で
        // 使用するのでこの pBuf は開放しません。
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/Rdr_CallBack1.NANR" );
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

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Rdr_CallBack1_OBJ.NCGR" );
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
    
    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/Rdr_CallBack1_OBJ.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        G2DDemo_Free( pBuf );
    }
    
    //------------------------------------------------------------------------------
    // load screen data
    {
        NNSG2dPaletteData*      pPltData;
        NNSG2dCharacterData*    pChrData;
        NNSG2dScreenData*       pScrData;
        void*   pBuf2;
        void*   pBuf3;

        pBuf  = G2DDemo_LoadNSCR( &pScrDataBG1, "data/Rdr_CallBack1_BG0.NSCR" );
        pBuf2 = G2DDemo_LoadNCLR( &pPltData, "data/Rdr_CallBack1_BG.NCLR" );
        pBuf3 = G2DDemo_LoadNCGR( &pChrData, "data/Rdr_CallBack1_BG.NCGR" );
        
        SDK_NULL_ASSERT( pBuf );
        SDK_NULL_ASSERT( pBuf2 );
        SDK_NULL_ASSERT( pBuf3 );

        NNS_G2dBGSetup(
            NNS_G2D_BGSELECT_MAIN1,
            pScrDataBG1,
            pChrData,
            pPltData,
            GX_BG_SCRBASE_0x0000,
            GX_BG_CHARBASE_0x08000
        );
        G2_SetBG1Priority(0);
        
        pBuf  = G2DDemo_LoadNSCR( &pScrData, "data/Rdr_CallBack1_BG1.NSCR" );
        NNS_G2dBGSetup(
            NNS_G2D_BGSELECT_MAIN2,
            pScrData,
            pChrData,
            pPltData,
            GX_BG_SCRBASE_0x1800,
            GX_BG_CHARBASE_0x10000
        );
        G2_SetBG2Priority(1);
        G2DDemo_Free( pBuf2 );
        G2DDemo_Free( pBuf3 );
        
    }
    
    return pCellBank;
}

//------------------------------------------------------------------------------
// 画面位置のスクリーンデータのキャラクタ番号を取得する
static u16 GetBG1ScreenCharNo( int posX, int posY )
{
    const u16*  pData = (const u16*)pScrDataBG1->rawData;
    
    posX %= pScrDataBG1->screenWidth;
    posY %= pScrDataBG1->screenHeight;
    
    pData += (posY >> 3) * (pScrDataBG1->screenWidth >> 3) + (posX >> 3);
    
    // キャラクタ番号を返す
    return (u16)(*pData & 0x3FF);
}

//------------------------------------------------------------------------------
// セル下半身部分が道路上に位置するか？
static BOOL IsOnLoad( int x, int y )
{
    const u16 bgNo = GetBG1ScreenCharNo( x, y );
    
    // 道路をあらわすキャラクタ番号か？
    if( bgNo == 0x20 || bgNo == 0x21 || bgNo == 0x40 || bgNo == 0x41 )
    {
        return TRUE;
    }else{
        return FALSE;
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
    NNSG2dRendererInstance      render;         // 描画用 Renderer
    NNSG2dRenderSurface         surface;        // メイン画面 Surface
    NNSG2dCellDataBank*         pCellBank;      // Cell データ
    NNSG2dOamManagerInstance    oamManager;     // 2D描画時のOAMマネージャ
    NNSG2dSurfaceType           outputType = INIT_OUTPUT_TYPE;


    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        
        GX_SetVisiblePlane( GX_PLANEMASK_BG0 | 
                            GX_PLANEMASK_BG1 | 
                            GX_PLANEMASK_BG2 | 
                            GX_PLANEMASK_OBJ );
        
        InitOamManager( &oamManager );
        InitRenderer( &render, &surface );
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
        static CellInfo cellInfo = {
            0,                                  // 描画 Cell 番号
            0,                                  // 回転角
            {FX32_ONE, FX32_ONE},               // 拡大率
            {CELL_INIT_POS_X, CELL_INIT_POS_Y}  // Cell 描画位置
        };

        //
        // パッド入力を読み取り Cell の表示情報と出力方法を更新します
        // 
        {
            G2DDemo_ReadGamePad();    
            //
            // セルの移動
            //
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
                }
                if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
                {
                    cellInfo.pos.x += FX32_ONE;
                }
            }
            //
            // OBJ単位コールバックの ON/OFF
            //
            if( !G2DDEMO_IS_PRESS(PAD_BUTTON_B) )
            {
                const int x = cellInfo.pos.x >> FX32_SHIFT;
                const int y = cellInfo.pos.y >> FX32_SHIFT;
                const int OBJ_SIZE = 16;
                
                // セルの4隅のうちいづれかが、道路の上ならば...
                if( IsOnLoad( x           , y               )  ||
                    IsOnLoad( x + OBJ_SIZE, y               )  ||
                    IsOnLoad( x           , y - OBJ_SIZE    )  ||
                    IsOnLoad( x + 16      , y - OBJ_SIZE)           )
                {
                    surface.pBeforeDrawOamBackFunc = CallBackBeforeOBJ;
                }else{
                    surface.pBeforeDrawOamBackFunc = NULL;
                }
            }else{
                surface.pBeforeDrawOamBackFunc = NULL;
            }
        }

        // 描画
        //
        // Renderer モジュールを用いて Cell を描画します。
        //
        {
            spCurrentOam = &oamManager;

            NNS_G2dBeginRendering( &render );
                NNS_G2dPushMtx();

                    NNS_G2dTranslate( cellInfo.pos.x, cellInfo.pos.y, 0 );
                    NNS_G2dRotZ( FX_SinIdx( cellInfo.rotate ), FX_CosIdx( cellInfo.rotate ) );
                    NNS_G2dScale( cellInfo.scale.x, cellInfo.scale.y, FX32_ONE );

                    NNS_G2dDrawCellAnimation( spCellAnim );

                NNS_G2dPopMtx();
            NNS_G2dEndRendering();
        }
        
        
        // V Blank 待ち
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();


        //
        // バッファの内容をHWに書き出します
        //
        {
            // Cell
            NNS_G2dApplyOamManagerToHW( &oamManager );
            NNS_G2dResetOamManagerBuffer( &oamManager );
        }
        
        // 更新
        {
            //
            // 頭部の揺らぎ量更新
            //
            {
                randomDiff.x = 0;
                randomDiff.y = 0;
                
                if( OS_GetTick() % 5 == 0 )
                {
                    randomDiff.x = 3;
                }
                
                if( OS_GetTick() % 7 == 0 )
                {
                    randomDiff.y = 3;
                }
            }
            //
            // セルアニメ更新
            //
            NNS_G2dTickCellAnimation( spCellAnim, FX32_ONE );
        }
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



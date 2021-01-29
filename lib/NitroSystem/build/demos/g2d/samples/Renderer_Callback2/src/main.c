/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_Callback2
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
//      Renderer モジュールのOBJ描画前コールバックを使用して、
//      OBJ単位の可視カリングを行うサンプルです。
//      
//      画面に2画面にまたがる大きな鉛筆のセルが表示されます。
//      サンプルデモは、NNS_G2dDrawCellAnimation() の処理にかかる時間をデバック表示します。
//
//  操作方法
//      START ボタン    カリングの ON,OFF
//      X ボタン        Cell 及び Surface の位置をリセットします。
//      十字キー        Cell を移動します。
//      Y + 十字キー    メイン Surface を移動します。
//      A               セルの回転を ON OFFします。
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


//------------------------------------------------------------------------------
// 定数定義
#define SCREEN_WIDTH        256                 // 画面幅
#define SCREEN_HEIGHT       192                 // 画面高さ

#define NUM_OF_OAM          128                 // OAM マネージャに割り当てる OAM の数
#define NUM_OF_AFFINE       (NUM_OF_OAM / 4)    // OAM マネージャに割り当てる Affineパラメータ の数

#define CHARA_BASE          0x0                 // キャラクタイメージベースアドレス
#define PLTT_BASE           0x0                 // パレットベースアドレス

#define INIT_OUTPUT_TYPE    NNS_G2D_SURFACETYPE_MAIN2D   // 出力方法の初期値

// サーフェス初期位置
#define SURFACE_MAIN_INIT_POS ((NNSG2dFVec2){0 << FX32_SHIFT, 0 << FX32_SHIFT})             
// サーフェス初期位置
#define SURFACE_SUB_INIT_POS ((NNSG2dFVec2){0 << FX32_SHIFT, SCREEN_HEIGHT << FX32_SHIFT})  
// セル初期位置
#define CELL_INIT_POS ((NNSG2dFVec2){150 << FX32_SHIFT, 120 << FX32_SHIFT})   



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

//------------------------------------------------------------------------------
// 矩形がView内か判定する。
static BOOL IsRectInView_
( 
    const fx32  px,
    const fx32  py,
    const fx32 _min_x, 
    const fx32 _max_x,
    const fx32 _min_y,
    const fx32 _max_y,
    const MtxFx32*          pMtx,
    const NNSG2dViewRect*   pViewRect
)
{
        s32 max_x, min_x;
    s32 max_y, min_y;
    s32 temp;
    
    // 回転拡大縮小変換後の表示最大最小XY座標を求める
    min_x = FX_Mul(_min_x, pMtx->_00) + FX_Mul(_min_y, pMtx->_10);  // 左上
    max_x = min_x;
    
    temp  = FX_Mul(_min_x, pMtx->_00) + FX_Mul(_max_y, pMtx->_10);  // 左下
    if      (temp < min_x) { min_x = temp; }
    else if (temp > max_x) { max_x = temp; }
    temp  = FX_Mul(_max_x, pMtx->_00) + FX_Mul(_min_y, pMtx->_10);  // 右上
    if      (temp < min_x) { min_x = temp; }
    else if (temp > max_x) { max_x = temp; }
    temp  = FX_Mul(_max_x, pMtx->_00) + FX_Mul(_max_y, pMtx->_10);  // 右下
    if      (temp < min_x) { min_x = temp; }
    else if (temp > max_x) { max_x = temp; }
    
    min_y = FX_Mul(_min_x, pMtx->_01) + FX_Mul(_min_y, pMtx->_11);  // 左上
    max_y = min_y;
    
    temp  = FX_Mul(_min_x, pMtx->_01) + FX_Mul(_max_y, pMtx->_11);  // 左下
    if      (temp < min_y) { min_y = temp; }
    else if (temp > max_y) { max_y = temp; }
    
    temp  = FX_Mul(_max_x, pMtx->_01) + FX_Mul(_min_y, pMtx->_11);  // 右上
    if      (temp < min_y) { min_y = temp; }
    else if (temp > max_y) { max_y = temp; }
    
    temp  = FX_Mul(_max_x, pMtx->_01) + FX_Mul(_max_y, pMtx->_11);  // 右下
    if      (temp < min_y) { min_y = temp; }
    else if (temp > max_y) { max_y = temp; }
    
    // セルの表示位置原点を加算する
    min_x  += px;
    max_x  += px;
    min_y  += py;
    max_y  += py;
    
    // サーフェース内外判定
    if( (max_y > 0) && (min_y < pViewRect->sizeView.y) )
    {
        if( (max_x > 0) && (min_x < pViewRect->sizeView.x) )
        {
            return TRUE;
        }
    }
    return FALSE;
}
//------------------------------------------------------------------------------
// OBJ 描画前コールバックとしてOBJカリング関数を実装する
// OBJはCELL と比較すると 多数なので、カリングによる処理負荷の増加量も
// 多い点に注意する必要があります。
static void OBJCullingFunc_
(
    struct NNSG2dRendererInstance*         pRend,
    struct NNSG2dRenderSurface*            pSurface,
    const  NNSG2dCellData*                 pCell,
    u16                                    oamIdx,
    const MtxFx32*                         pMtx
)
{
#pragma unused( pCell )
#pragma unused( oamIdx )
    {
        const GXOamAttr* pOam = &pRend->rendererCore.currentOam;
        const GXOamShape shape = NNS_G2dGetOAMSize( pOam ); 
        
        const fx32 minX = NNS_G2dRepeatXinCellSpace( (s16)pOam->x ) * FX32_ONE ;
        const fx32 maxX = ( NNS_G2dRepeatXinCellSpace( (s16)pOam->x ) + NNS_G2dGetOamSizeX( &shape ) ) * FX32_ONE;
        const fx32 minY = NNS_G2dRepeatYinCellSpace( (s16)pOam->y ) * FX32_ONE;
        const fx32 maxY = ( NNS_G2dRepeatYinCellSpace( (s16)pOam->y ) + NNS_G2dGetOamSizeY( &shape ) ) * FX32_ONE;
        
        const fx32  px = pMtx->_20 - pSurface->viewRect.posTopLeft.x;
        const fx32  py = pMtx->_21 - pSurface->viewRect.posTopLeft.y;
                    
        if( IsRectInView_( px, py, minX, maxX, minY, maxY, pMtx, &pSurface->viewRect ) )
        {
            pRend->rendererCore.bDrawEnable = TRUE;
        }else{
            pRend->rendererCore.bDrawEnable = FALSE;
        }
    }   
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
        
        // 表示先
        pMainSurface->type                      = NNS_G2D_SURFACETYPE_MAIN2D;
        
        pMainSurface->pBeforeDrawOamBackFunc = OBJCullingFunc_;
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
        
        // 表示先
        pSubSurface->type                       = NNS_G2D_SURFACETYPE_SUB2D;
        
        pSubSurface->pBeforeDrawOamBackFunc = OBJCullingFunc_;
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
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/Rdr_CallBack2.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // セルバンクは最後までメインメモリ上で使用するので
        // この pBuf は開放しません。
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // セルアニメーションデータは最後までメインメモリ上で
        // 使用するのでこの pBuf は開放しません。
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/Rdr_CallBack2.NANR" );
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

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Rdr_CallBack2.NCGR" );
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

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/Rdr_CallBack2.NCLR" );
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
        G2DDemo_Free( pBuf );
    }

    return pCellBank;
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


    // Initilizing App.
    {
        G2DDemo_CommonInit();
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
        static NNSG2dFVec2 cellAPos = CELL_INIT_POS;  // Cell A 描画位置
        static u16         priority = 0;
        static u16         plttNo   = 0;
        static BOOL        bRotate = FALSE;
        static u16         rot = 0x0;
        static OSTick      time;

        //------------------------------------------------------
        // パッド入力を読み取り、データを更新します
        {
            int dx = 0;
            int dy = 0;
            
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
                mainSurface.viewRect.posTopLeft.x += (dx << FX32_SHIFT);
                mainSurface.viewRect.posTopLeft.y += (dy << FX32_SHIFT);
            }
            else
            {
                // セル移動
                cellAPos.x += (dx << FX32_SHIFT);
                cellAPos.y += (dy << FX32_SHIFT);
            }

            // リセット
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
            {
                cellAPos = CELL_INIT_POS;
                mainSurface.viewRect.posTopLeft = SURFACE_MAIN_INIT_POS;
                subSurface.viewRect.posTopLeft  = SURFACE_SUB_INIT_POS;
            }
            
            // 回転 ON/OFF           
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
            {
                bRotate = bRotate^TRUE;
            }
            
            //
            // オブジェ単位の可視カリング関数 ON/OFF。
            //
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_START) )
            {
                if( mainSurface.pBeforeDrawOamBackFunc == NULL )
                {
                    mainSurface.pBeforeDrawOamBackFunc  = OBJCullingFunc_;
                    subSurface.pBeforeDrawOamBackFunc   = OBJCullingFunc_;
                }else{
                    mainSurface.pBeforeDrawOamBackFunc  = NULL;
                    subSurface.pBeforeDrawOamBackFunc   = NULL;
                }
            }
        }
        //------------------------------------------------------
        // 描画
        //
        // Renderer モジュールを用いて Cell を描画します。
        //
        {
            NNS_G2dBeginRendering( &render );
                    
                // セル キー操作により移動可能。
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( cellAPos.x, cellAPos.y, 0 );
                    
                    if( bRotate )
                    {
                        NNS_G2dRotZ( FX_SinIdx( rot ), FX_CosIdx( rot ) );
                    }
                    
                    // 描画にかかる時間を計測
                    time = OS_GetTick();
                        NNS_G2dDrawCellAnimation( spCellAnim );
                    time = OS_GetTick() - time;
                    
                NNS_G2dPopMtx();
                
            NNS_G2dEndRendering();
        }
        //------------------------------------------------------
        // 表示情報の出力
        {
            G2DDemo_PrintOutf( 0, 0, "TimeForDraw  : %06ld usec\n", OS_TicksToMicroSeconds(time) );
            G2DDemo_PrintOutf( 0, 1, "OBJ-Culling  : %s", ( mainSurface.pBeforeDrawOamBackFunc != NULL ) ? "ON ":"OFF");
            G2DDemo_PrintOutf( 0, 2, "Rotate       : %s", ( bRotate ) ? "ON ":"OFF");
        }
        //------------------------------------------------------
        // V Blank 待ち
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();

        //------------------------------------------------------
        //
        // バッファの内容をHWに書き出します
        //
        {
            // Cell
            NNS_G2dApplyAndResetOamManagerBuffer( &sMainOamMan );
            NNS_G2dApplyAndResetOamManagerBuffer( &sSubOamMan );
            
            // 表示情報
            G2DDemo_PrintApplyToHW();
        }
        
        //------------------------------------------------------
        // シーン更新
        rot += 0x100;
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



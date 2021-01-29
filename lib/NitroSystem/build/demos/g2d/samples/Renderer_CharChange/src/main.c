/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Renderer_CharChange
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
//      Renderer モジュールを用いてセルアニメーションを描画するサンプル
//        ･Renderer の使用する 画像プロクシ（2個）を切り替えて描画を行います。
//        ･画像プロクシのうちのひとつは、VRAMに読み込まれた画像データの一部分
//         だけを書き換えます。
//        ･その際に、g2dcvtr.exe(g2dバイナリコンバータ) 
//         の -cr/オプション (キャラクタファイル変換領域指定オプション)を利用して
//         データを作成しています。
//
//      本プロジェクトのメイクファイルに記述されているコンバートオプションや、
//      data/src フォルダ以下のリソースデータもあわせて参照していただくと、
//      処理が理解していただきやすいと思います。        
//
//  操作方法
//                      
//      A ボタン      使用する画像プロクシの切り替え  
//                      
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// 定数定義
#define SCREEN_WIDTH                256                     // 画面幅
#define SCREEN_HEIGHT               192                     // 画面高さ

#define NUM_OF_OAM                  128                     // OAM マネージャに割り当てる OAM の数

#define CHARA_BASE                  0x0                     // キャラクタイメージベースアドレス
#define PLTT_BASE                   0x0                     // パレットベースアドレス

#define APP_NUM_IMG_PROXY           2                       // 画像プロクシの数

//------------------------------------------------------------------------------
// グローバル変数

static NNSG2dImageProxy             sImageProxy[APP_NUM_IMG_PROXY];    // Cell 用キャラクタ/テクスチャプロキシ
static NNSG2dImagePaletteProxy      sPaletteProxy;                     // Cell 用パレットプロキシ

static NNSG2dOamManagerInstance     oamManager;     // レンダラが使用するOAMマネージャ

static NNSG2dAnimBankData*          spAnimBank;     // アニメーションバンク
static NNSG2dCellAnimation*         spCellAnim;     // セルアニメーション
static NNSG2dCellDataBank*          spCellBank;     // セル情報バンク

//------------------------------------------------------------------------------
// プロトタイプ宣言



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
    SDK_NULL_ASSERT( pOam );

    return NNS_G2dEntryOamManagerOamWithAffineIdx( &oamManager, pOam, affineIndex );
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
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( &oamManager, mtx );
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
static void InitRenderer
(
    NNSG2dRendererInstance* pRenderer,
    NNSG2dRenderSurface*    pSurface
)
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
    
    // コールバックの初期化
    pSurface->pFuncOamRegister       = CallBackAddOam;
    pSurface->pFuncOamAffineRegister = CallBackAddAffine;

    pSurface->type = NNS_G2D_SURFACETYPE_MAIN2D;
    
    // 描画対象サーフェスの設定
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    
    // 画像プロクシ、パレットプロクシの設定、
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy[0], &sPaletteProxy );
}


//------------------------------------------------------------------------------
static u32 InitImageProxy
( 
    NNSG2dImageProxy* pImageProxy,
    const char*       pNcgrFileName,
    u32               characterBaseOffset
)
{
    SDK_NULL_ASSERT( pNcgrFileName );
    NNS_G2dInitImageProxy( pImageProxy );
    
    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData* pCharData  = NULL;
        void*                pBuf       = NULL;
        
        pBuf = G2DDemo_LoadNCGR( &pCharData, pNcgrFileName );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            characterBaseOffset,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            pImageProxy );
            
        // キャラクタデータを VRAM にコピーしたので
        // この pBuf は開放します。
        G2DDemo_Free( pBuf );
        
        return pCharData->szByte;
    }
}

/*---------------------------------------------------------------------------*
  Name:         OverWrriteVramImage_

  Description:  キャラクタデータを部分的に上書きします

  Arguments:    なし。

  Returns:      なし
  
 *---------------------------------------------------------------------------*/
static void OverWrriteVramImage_( void* pNcgrFile )
{
    NNSG2dCharacterPosInfo* pCharPosInfo = NULL;
    
    if( NNS_G2dGetUnpackedCharacterPosInfo( pNcgrFile, &pCharPosInfo ) )
    {
    
    }
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  アニメーションデータ、セルバンク、キャラクタデータ、
                パレットデータをファイルから読み込み、キャラクタデータと
                パレットデータをそれぞれVRAMにロードします。

  Arguments:    なし。

  Returns:      なし
  
 *---------------------------------------------------------------------------*/
static void LoadResources(void)
{
    void* pBuf;

    
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //------------------------------------------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &spCellBank, "data/PartialCharCvt.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // セルバンクは最後までメインメモリ上で使用するので
        // この pBuf は開放しません。
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // セルアニメーションデータは最後までメインメモリ上で
        // 使用するのでこの pBuf は開放しません。
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/PartialCharCvt.NANR" );
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
                NNS_G2dGetAnimSequenceByIdx( spAnimBank, 0), spCellBank );
        }
    }

    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        int i = 0;
        u32 szLoaded = 0;
        
        const char* ncgrFileNames [] = 
        {
            "data/PartialCharCvt0.NCGR",
            "data/PartialCharCvt0.NCGR"
        };
        
        for( i = 0 ; i < APP_NUM_IMG_PROXY; i++ )
        { 
            szLoaded += InitImageProxy( &sImageProxy[i], 
                                        ncgrFileNames[i], 
                                        CHARA_BASE + szLoaded );
        }
        
        //
        // sImageProxy[1] が管理しているVRAM領域を部分的に上書きします。
        //
        {
            const u32 addr = NNS_G2dGetImageLocation( &sImageProxy[1], NNS_G2D_VRAM_TYPE_2DMAIN );

            NNSG2dCharacterData*    pCharData       = NULL;
            NNSG2dCharacterPosInfo* pCharPosInfo    = NULL;
            void*                   pBuf            = NULL;
        
            pBuf = G2DDemo_LoadNCGREx( &pCharData, &pCharPosInfo, "data/PartialCharCvt1.NCGR" );
            SDK_NULL_ASSERT( pBuf );
            
            DC_FlushAll();
            {
                const int szChar = ( pCharData->pixelFmt == GX_TEXFMT_PLTT16 ) ? 32 : 64;
                const int offset = ( pCharPosInfo->srcPosY * pCharPosInfo->srcW ) * szChar;
                GX_LoadOBJ( (void*)pCharData->pRawData, addr + offset, pCharData->szByte );
            }
            
            // キャラクタデータを VRAM にコピーしたので
            // この pBuf は開放します。
            G2DDemo_Free( pBuf );
        }
    }
    

    
    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/PartialCharCvt.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        G2DDemo_Free( pBuf );
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
    int                         imgProxyIdx = 0;
    
    // アプリケーションを初期化します
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        InitRenderer( &render, &surface );
        LoadResources();
    }

    // 描画を開始します
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    // Main loop
    while( TRUE )
    {
        // パッド入力処理
        {
            G2DDemo_ReadGamePad();
            
            // A ボタンで、使用する画像プロクシを変更します
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
            {
                imgProxyIdx++;
                imgProxyIdx &= (APP_NUM_IMG_PROXY - 1);
                
                NNS_G2dSetRendererImageProxy( &render, 
                                              &sImageProxy[imgProxyIdx], 
                                              &sPaletteProxy );
            }
        }
        

        // 描画
        //
        // Renderer モジュールを用いて Cell を描画します。
        //
        {
            NNS_G2dBeginRendering( &render );
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( 120 * FX32_ONE, 120 * FX32_ONE, 0 );
                    NNS_G2dDrawCellAnimation( spCellAnim );
                NNS_G2dPopMtx();
            NNS_G2dEndRendering();
        }

        // 表示情報の出力
        {
            G2DDemo_PrintOutf(0, 0, "ImageProxyIdx : %d", imgProxyIdx );
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



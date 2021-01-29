/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Entity
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      エンティティを用いてセルアニメーションを表示する。
//      カラーパレットを切り替えてキャラクタの色を変更する。
//
//  操作方法
//      START ボタン    Cell の出力方法を OBJ <=> SoftwareSprite と
//                      切り替えます。
//      A ボタン        アニメーションシーケンス切り替え
//      X ボタン        カラーパレット切り替え
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>
#include <string.h>

#include "g2d_demolib.h"


#define SCREEN_WIDTH                256     // 画面幅
#define SCREEN_HEIGHT               192     // 画面高さ

#define NUM_OF_OAM                 128      // OAM マネージャが管理する OAM の数
#define NUM_OF_AFFINE               1       // OAM マネージャが管理する Affine パラメータの数
#define NUM_OF_ALT_PLT              4       // パレットの置き換えに使えるパレットの数
#define PLT_SWAP_TARGET             8       // 置き換え元のパレット番号



//------------------------------------------------------------------------------
// グローバル変数

// OAM マネージャ
static NNSG2dOamManagerInstance         sOamManager;

//------------------------------------------------------------------------------
// プロトタイプ宣言

static inline u16 GetNumOfEntitySequence( NNSG2dEntity* pEntity );
static BOOL CallBackAddOam( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine );
static u16 CallBackAddAffine( const MtxFx22* mtx );
static void LoadEntity(
    NNSG2dEntity* pEntity,
    const char* pathbase
);
static void LoadResources(
                NNSG2dEntity*               pEntity,
                NNSG2dImageProxy*           pImgProxy,
                NNSG2dImagePaletteProxy*    pPltProxy);
static void InitializeOamManager(NNSG2dOamManagerInstance* pManager);
static void InitRenderer(
                NNSG2dRendererInstance*     pRenderer,
                NNSG2dRenderSurface*        pSurface,
                NNSG2dImageProxy*           pImgProxy,
                NNSG2dImagePaletteProxy*    pPltProxy);


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
    
    return NNS_G2dEntryOamManagerOamWithAffineIdx( &sOamManager, pOam, affineIndex );
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

    return NNS_G2dEntryOamManagerAffine( &sOamManager, mtx );
}


/*---------------------------------------------------------------------------*
  Name:         LoadEntity

  Description:  リソースファイルを読み込み Entity を構築します。

  Arguments:    pEntity:    読み込んだ Entity を受け取る Entity へのポインタ。
                pathbase:   Entity の各リソースファイル名から拡張子を
                            除いた文字列へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void LoadEntity(
    NNSG2dEntity* pEntity,
    const char* pathbase
){
    size_t length = strlen( pathbase );
    char*                   filepath;
    char*                   extPos;
    NNSG2dCellDataBank*     pCellBank = NULL;
    NNSG2dAnimBankData*     pAnimBank = NULL;
    NNSG2dEntityDataBank*   pEntityBank = NULL;
    SDK_NULL_ASSERT( pEntity );
    SDK_NULL_ASSERT( pathbase );


    filepath = (char*)G2DDemo_Alloc( length + 6 );
    SDK_NULL_ASSERT( filepath );

    extPos = filepath + length;
    (void)strcpy( filepath, pathbase );

    // load entity resource
    {
        void* pBuf;

        (void)strcpy( extPos, ".NCER" );
        pBuf = G2DDemo_LoadNCER( &pCellBank, filepath );
        SDK_NULL_ASSERT( pBuf );

        (void)strcpy( extPos, ".NANR" );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, filepath );
        SDK_NULL_ASSERT( pBuf );

        (void)strcpy( extPos, ".NENR" );
        pBuf = G2DDemo_LoadNENR( &pEntityBank, filepath );
        SDK_NULL_ASSERT( pBuf );
    }

    // construct an entity
    {
        NNSG2dCellAnimation *pCellAnim = G2DDemo_GetNewCellAnimation(1);
        SDK_NULL_ASSERT( pCellAnim );

        NNS_G2dInitCellAnimation(
            pCellAnim,
            NNS_G2dGetAnimSequenceByIdx( pAnimBank, 0 ),
            pCellBank );

        NNS_G2dInitEntity(
            pEntity,
            pCellAnim,
            NNS_G2dGetEntityDataByIdx( pEntityBank, 0 ),
            pAnimBank );

        // 上記処理だけでは Entity のアニメーションシーケンスが正しく初期化されません。
        // NNS_G2dSetEntityCurrentAnimation で初期値を指定する必要があります。
        NNS_G2dSetEntityCurrentAnimation( pEntity, 0 );

        SDK_ASSERT( NNS_G2dIsEntityValid( pEntity ) );
    }

    G2DDemo_Free( filepath );
}



/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  リソースを読み込みます。

  Arguments:    pEntity:    読み込んだ Entity を受け取る Entity へのポインタ。
                pImgProxy:  読み込んだキャラクタの情報を格納する ImageProxy。
                pPltProxy:  読み込んだパレットの情報を格納する PaletteProxy。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void LoadResources(
                NNSG2dEntity*               pEntity,
                NNSG2dImageProxy*           pImgProxy,
                NNSG2dImagePaletteProxy*    pPltProxy)
{
    void* pBuf;

    NNS_G2dInitImageProxy( pImgProxy );
    NNS_G2dInitImagePaletteProxy( pPltProxy );

    // load resources.

    //------------------------------------------------------------------------------
    // load entity
    {
        LoadEntity( pEntity, "data/Entity" );
    }

    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData*        pCharData = NULL;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Entity.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // For 2D Graphics Engine
        NNS_G2dLoadImage2DMapping( pCharData, 0, NNS_G2D_VRAM_TYPE_2DMAIN, pImgProxy );

        G2DDemo_Free( pBuf );
    }

    // load character data for 3D (for software sprite)
    {
        NNSG2dCharacterData*        pCharData = NULL;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/Entity.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // For 3D Graphics Engine
        NNS_G2dLoadImage2DMapping( pCharData, 0, NNS_G2D_VRAM_TYPE_3DMAIN, pImgProxy );

        G2DDemo_Free( pBuf );
    }


    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData*        pPltData = NULL;

        pBuf = G2DDemo_LoadNCLR( &pPltData, "data/Entity.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // For 3D Graphics Engine
        NNS_G2dLoadPalette( pPltData, 0, NNS_G2D_VRAM_TYPE_3DMAIN, pPltProxy );
        // For 2D Graphics Engine
        NNS_G2dLoadPalette( pPltData, 0, NNS_G2D_VRAM_TYPE_2DMAIN, pPltProxy );

        G2DDemo_Free( pBuf );
    }
}



/*---------------------------------------------------------------------------*
  Name:         InitRenderer

  Description:  Renderer と Surface を初期化します。

  Arguments:    pRenderer:  初期化する Renderer へのポインタ。
                pSurface:   初期化し、Renderer に設定する Surface へのポインタ。
                pImgProxy:  Renderer に設定する ImageProxy へのポインタ。
                pPltProxy:  Renderer に設定する PaletteProxyへのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitRenderer(
                NNSG2dRendererInstance*     pRenderer,
                NNSG2dRenderSurface*        pSurface,
                NNSG2dImageProxy*           pImgProxy,
                NNSG2dImagePaletteProxy*    pPltProxy)
{
    NNSG2dViewRect* pRect = &(pSurface->viewRect);
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pSurface );
    SDK_NULL_ASSERT( pImgProxy );
    SDK_NULL_ASSERT( pPltProxy );

    NNS_G2dInitRenderer( pRenderer );
    NNS_G2dInitRenderSurface( pSurface );

	// サーフェイスの範囲を設定します
    pRect->posTopLeft.x = FX32_ONE * 0;
    pRect->posTopLeft.y = FX32_ONE * 0;
    pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
    pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

    pSurface->pFuncOamRegister       = CallBackAddOam;
    pSurface->pFuncOamAffineRegister = CallBackAddAffine;
    pSurface->type = NNS_G2D_SURFACETYPE_MAIN3D;

	// Renderer に設定します
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, pImgProxy, pPltProxy );
}



/*---------------------------------------------------------------------------*
  Name:         GetNumOfEntitySequence

  Description:  Entity に登録されているシーケンスの数を取得する。

  Arguments:    pEntity:    調査対象の Entity。

  Returns:      Entity に登録されているシーケンスの数
*---------------------------------------------------------------------------*/
static inline u16 GetNumOfEntitySequence( NNSG2dEntity* pEntity )
{
    return pEntity->pEntityData->animData.numAnimSequence;
}



/*---------------------------------------------------------------------------*
  Name:         InitializeOamManager

  Description:  OAM マネージャを初期化します。

  Arguments:    pManager:   初期対象の OAM マネージャ

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitializeOamManager(NNSG2dOamManagerInstance* pManager)
{
    BOOL    bSuccess;

    bSuccess = NNS_G2dGetNewManagerInstance( pManager, 0, NUM_OF_OAM - 1, NNS_G2D_OAMTYPE_MAIN );
    SDK_ASSERT( bSuccess );

    bSuccess = NNS_G2dInitManagerInstanceAffine( pManager, 0, NUM_OF_AFFINE - 1 );
    SDK_ASSERT( bSuccess );
}



/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    static NNSG2dEntity                 entity;     // Entity
    static NNSG2dRendererInstance       renderer;   // Renderer
    static NNSG2dRenderSurface          surface;    // Surface
    static NNSG2dImageProxy             imgProxy;   // ImageProxy
    static NNSG2dImagePaletteProxy      pltProxy;   // PaletteProxy
    static NNSG2dPaletteSwapTable       pltSwpTbl;  // パレット置換テーブル

    // Initialize App.
    {
        u16 i;
        u16* pTbl = pltSwpTbl.paletteIndex;

        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();

        LoadResources( &entity, &imgProxy, &pltProxy );
        NNS_G2dInitOamManagerModule();
        InitializeOamManager( &sOamManager );
        InitRenderer( &renderer, &surface, &imgProxy, &pltProxy );

    	// パレット置換テーブルを初期化
        for( i = 0; i < NNS_G2D_NUM_COLOR_PALETTE; ++i )
        {
            pTbl[i] = i;
        }
        NNS_G2dSetEntityPaletteTable( &entity, &pltSwpTbl );
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
        static u16      rotate      = 0;
        static fx32     animSpeed   = FX32_ONE;
        static u16      animIdx    = 0;
        static u16      pltIdx     = 0;

        //
        // Key handling.
        //
        {
            G2DDemo_ReadGamePad();

            // シーケンス切り替え
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_A ) )
            {
                animIdx++;
                if( animIdx >= GetNumOfEntitySequence( &entity ) )
                {
                    animIdx = 0;
                }
                NNS_G2dSetEntityCurrentAnimation( &entity, animIdx );
            }

            // 描画モード切替
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_START ) )
            {
                if( surface.type == NNS_G2D_SURFACETYPE_MAIN2D )
                {
                    surface.type = NNS_G2D_SURFACETYPE_MAIN3D;
                }else{
                    surface.type = NNS_G2D_SURFACETYPE_MAIN2D;
                }
            }

            // パレット切り替え
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_X ) )
            {
                pltIdx++;
                if( pltIdx > NUM_OF_ALT_PLT )
                {
                    pltIdx = 0;
                }

                if( pltIdx == 0 )
                {
                	// 置換を行わない
                    pltSwpTbl.paletteIndex[PLT_SWAP_TARGET] = PLT_SWAP_TARGET;
                }
                else
                {
                    pltSwpTbl.paletteIndex[PLT_SWAP_TARGET] = pltIdx;
                }
            }
        }

        //
        // draw
        //
        {
            NNS_G2dBeginRendering( &renderer );
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( FX32_ONE * 120, FX32_ONE * 100, 0 );
                    NNS_G2dDrawEntity( &entity );
                NNS_G2dPopMtx();
            NNS_G2dEndRendering();
        }

        // 表示状態を出力
        {
            G2DDemo_PrintOutf(0, 0, "sequence no.: %d", animIdx);
            G2DDemo_PrintOutf(0, 1, "palette swap: %d", pltIdx);

            G2DDemo_PrintOut(0, 23, (surface.type == NNS_G2D_SURFACETYPE_MAIN2D) ?
                                "OBJ           ": "SoftwareSprite");
        }

        // SoftwareSprite の出力反映
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);

        // V ブランク待ち
        SVC_WaitVBlankIntr();

        //
        // copy Oam datas from manager's buffer to 2D HW.
        //
        {
            // BG（表示状態文字列）を反映
            G2DDemo_PrintApplyToHW();

            // OBJ を反映
            NNS_G2dApplyOamManagerToHW( &sOamManager );
            NNS_G2dResetOamManagerBuffer( &sOamManager );
        }

        //
        // Tick animation
        //
        {
            NNS_G2dTickEntity( &entity, animSpeed );
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



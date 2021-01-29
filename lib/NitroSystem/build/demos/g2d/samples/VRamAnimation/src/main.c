/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - VRamAnimation
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
//      VRAM 転送マネージャを用いてセルアニメーションを描画、更新するサンプル
//      ･VRAM転送用のバイナリデータを作成するためにコンバータに-vta オプションを
//       指定してデータ出力を行っています。(MakeFileを確認してください。 )
//
//  操作方法
//      START ボタン    Cell の出力方法を OBJ <=> SoftwareSprite と
//                      切り替えます。
//      A ボタン        表示する Cell を切り替えます(切り替えるセルが存在するならば)。
//      X ボタン        Cell の拡大縮小移動回転状態をリセットします。
//      十字キー        Cell を移動します。
//      B ボタン + 十字キー
//                      Cell を回転します。
//      Y ボタン + 十字キー
//                      Cell を拡大縮小します。
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


#define CHARNAME_TO_OFFSET(charname)    (charname * 32)     // キャラクタネームから VRAM 上のオフセットに変換

#define SCREEN_WIDTH                256                     // 画面幅
#define SCREEN_HEIGHT               192                     // 画面高さ

#define AFFINE_IDX                  3                       // Cell 用 Affine Index
#define CELL_ROTATE_UNIT            0xFF                    // Cell の単位回転量
#define CELL_SCALE_UNIT             ((FX32_ONE >> 7) * 3 )  // Cell の単位拡大縮小率
                                                            // scale 値が 0 付近の値にならないようにしてあります
#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell の初期位置 X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell の初期位置 Y

#define INIT_OUTPUT_TYPE            NNS_G2D_SURFACETYPE_MAIN2D   // 出力方法の初期値

#define NUM_OF_OAM                  128                     // OAM マネージャに割り当てる OAM の数
#define NUM_OF_CELLANM              1                       // 表示するセルアニメーションの数
#define NUM_OF_VRAMSTATE            NUM_OF_CELLANM          // セル転送マネージャが扱う状態数
#define NUM_OF_VRAMTASK             NUM_OF_CELLANM          // VRAM 転送マネージャが扱う転送タスクの数

#define PLTT_BASE                   0x0                     // パレットベースアドレス

#define CELL0_CHARNAME_BEGIN_MAIN   6                       // 利用するメイン画面 OAM の開始番号
#define CELL0_CHARNAME_BEGIN_SUB    0                       // 利用するサブ画面 OAM の開始番号
#define CHAR_MAIN_OFFSET            CHARNAME_TO_OFFSET(CELL0_CHARNAME_BEGIN_MAIN)
#define CHAR_SUB_OFFSET             CHARNAME_TO_OFFSET(CELL0_CHARNAME_BEGIN_SUB)

#define TEX_OFFSET                  128                     // 利用するテクスチャ VRAM のオフセット



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

// VRAM 転送マネージャ用のワーク領域
static NNSGfdVramTransferTask       sTransferTaskArray[NUM_OF_VRAMTASK];

// セル転送マネージャ用のワーク領域
static NNSG2dCellTransferState      sVramStateArray[NUM_OF_VRAMSTATE];

static u32                          sCellTransManHandle;    // セル転送状態管理オブジェクトのハンドル


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
  Name:         CallbackRegistTransferTask

  Description:  NNS_G2dDraw* 関数内部でキャラクタデータの転送タスクを
                登録するために呼ばれる関数です。

  Arguments:    type:       転送先 VRAM 領域種別
                dstAddr:    転送先アドレスオフセット
                pSrc:       転送元データへのポインタ。
                szByte:     転送すべきデータサイズ。

  Returns:      成功した場合は TRUE、そうでなければ FALSE を返します。
 *---------------------------------------------------------------------------*/
static BOOL CallbackRegistTransferTask(
                NNS_GFD_DST_TYPE type,
                u32 dstAddr,
                void* pSrc,
                u32 szByte )
{
    // VRAM 転送マネージャにそのまま渡す
    return NNS_GfdRegisterNewVramTransferTask(
                type,
                dstAddr,
                pSrc,
                szByte );
}

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
static void InitOamManager(NNSG2dOamManagerInstance* pOamManager)
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

    // 表示矩形
    pRect->posTopLeft.x = FX32_ONE * 0;
    pRect->posTopLeft.y = FX32_ONE * 0;
    pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
    pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

    // コールバック関数
    pSurface->pFuncOamRegister       = CallBackAddOam;
    pSurface->pFuncOamAffineRegister = CallBackAddAffine;

    // Surface 登録
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );

    // プロキシ登録
    NNS_G2dSetRendererImageProxy( pRenderer, &sImageProxy, &sPaletteProxy );

        // Z オフセット増分を指定
    NNS_G2dSetRendererSpriteZoffset( pRenderer, - FX32_ONE );
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  必要なデータをファイルから読み込みます。
                またセルアニメーションの構築も行います。

  Arguments:    なし。

  Returns:      読み込んだセルバンクへのポインタ。
 *---------------------------------------------------------------------------*/
static NNSG2dCellDataBank* LoadResources(void)
{
    NNSG2dCharacterData* pCharData2D;
    NNSG2dCharacterData* pCharData3D;
    NNSG2dCellDataBank* pCellBank;
    void* pBuf;

    // Proxy 初期化
    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/VRamAnimation.NCLR" );
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

        // パレットデータを VRAM にコピーしたので
        // この pBuf は開放します。
        G2DDemo_Free( pBuf );
    }

    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        pBuf = G2DDemo_LoadNCGR( &pCharData2D, "data/VRamAnimation.NCGR" );
        SDK_NULL_ASSERT( pBuf );
        // このデータは最後までメインメモリ上で使用するので
        // この pBuf は開放しません。以下同じ。

        // 他の NNS_G2dLoad* 関数と異なり NNS_G2dLoadImageVramTransfer は
        // キャラクタデータをVRAMに読み込みません。
        // キャラクタデータの属性を ImageProxy と HW に設定するだけです。
        // そのためキャラクタデータをメインメモリに保持する必要があります。
        NNS_G2dLoadImageVramTransfer(
            pCharData2D,
            CHAR_MAIN_OFFSET,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sImageProxy );
    }

    // load character data for 3D (software sprite)
    {
        pBuf = G2DDemo_LoadNCBR( &pCharData3D, "data/VRamAnimation.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        NNS_G2dLoadImageVramTransfer(
            pCharData3D,
            TEX_OFFSET,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sImageProxy );
    }

    //------------------------------------------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/VRamAnimation.NCER" );
        SDK_NULL_ASSERT( pBuf );
    }


    //------------------------------------------------------------------------------
    // load animation data
    {
        pBuf = G2DDemo_LoadNANR( &spAnimBank, "data/VRamAnimation.NANR" );
        SDK_NULL_ASSERT( pBuf );
    }

    //------------------------------------------------------------------------------
    // セルアニメーションを構築します
    {
        // VRAM 転送のための初期化を行います
        {
            NNS_GfdInitVramTransferManager(
                sTransferTaskArray,
                NUM_OF_VRAMTASK
            );

            NNS_G2dInitCellTransferStateManager(
                sVramStateArray,
                NUM_OF_VRAMSTATE,
                CallbackRegistTransferTask          // <- (1)
            );

            // セル転送状態管理オブジェクトを新しく取得します。
            // オブジェクトが不必要になったら
            // NNS_G2dFreeCellTransferStateHandle( handle ) で解放します。
            sCellTransManHandle = NNS_G2dGetNewCellTransferStateHandle();
            SDK_ASSERT( sCellTransManHandle != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE );
        }

        // セルアニメーションの実体を初期化します
        {
            spCellAnim = G2DDemo_GetNewCellAnimation(1);
            SDK_NULL_ASSERT( spCellAnim );

            SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx(spAnimBank, 0) );
            SDK_ASSERT( NNS_G2dCellDataBankHasVramTransferData(pCellBank) );

            // NNS_G2dInitializeVramTransferedCellAnimation の第 8,9 引数を共に NULL 以外にする場合は、
            // 基本的には2つのデータサイズが等しくなければなりません
            SDK_ASSERT( pCharData2D->szByte == pCharData3D->szByte );

            NNS_G2dInitCellAnimationVramTransfered(
                spCellAnim,
                NNS_G2dGetAnimSequenceByIdx(spAnimBank, 0),
                pCellBank,

                sCellTransManHandle,    // このセルアニメーションのVRAM転送を管理する
                                        // セル転送状態管理オブジェクトのハンドル
                                        // 以下の3つは (1) のCallback関数に dstAddr として渡されます
                                        // 通常は ImageProxy に指定したものと同じ値を指定します
                TEX_OFFSET,             // テクスチャ転送先オフセット
                CHAR_MAIN_OFFSET,       // メイン画面キャラクタ転送先オフセット
                CHAR_SUB_OFFSET,        // サブ画面キャラクタ転送先オフセット
                                        // 以下の2つはどちらかが NULL でも構いません
                pCharData2D->pRawData,  // キャラクタデータ
                pCharData3D->pRawData,  // テクスチャデータ
                pCharData2D->szByte     // テクスチャデータ or キャラクタデータのサイズ
            );
        }
    }

    return pCellBank;
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

    // Rotate Cell
    if( G2DDEMO_IS_PRESS(PAD_BUTTON_B) )
    {
        if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
        {
            pCellInfo->rotate -= CELL_ROTATE_UNIT;      // 反時計回り
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
        {
            pCellInfo->rotate += CELL_ROTATE_UNIT;      // 時計回り
        }
    }

    // Zoom Cell
    if( G2DDEMO_IS_PRESS(PAD_BUTTON_Y) )
    {
        if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
        {
            pCellInfo->scale.y += CELL_SCALE_UNIT;   // Y方向拡大
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
        {
            pCellInfo->scale.y -= CELL_SCALE_UNIT;   // Y方向縮小
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
        {
            pCellInfo->scale.x += CELL_SCALE_UNIT;   // X方向拡大
        }
        if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
        {
            pCellInfo->scale.x -= CELL_SCALE_UNIT;   // X方向縮小
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
        G2DDemo_PrintInit();
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


        // 入力を読み取り Cell の表示情報と出力方法を更新します
        ProcessInput(&cellInfo, &outputType, pCellBank->numCells);

        surface.type = outputType;

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

            // セル転送マネージャへの転送要求を コールバック関数経由で
            // VRAM 転送マネージャに送ります
            NNS_G2dUpdateCellTransferStateManager();
        }

        // 表示情報の出力
        {
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);
            G2DDemo_PrintOutf(0, 1, "scale: x=%7.3f y=%7.3f",
                (double)cellInfo.scale.x / FX32_ONE, (double)cellInfo.scale.y / FX32_ONE);
            G2DDemo_PrintOutf(0, 2, "rot:   %5d (%6.2f)",
                cellInfo.rotate, 360.0 * cellInfo.rotate / 0x10000);
            G2DDemo_PrintOut(0, 23, (outputType == NNS_G2D_SURFACETYPE_MAIN2D) ?
                   "OBJ           ": "SoftwareSprite");
        }

        // V Blank 待ち
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();


        //
        // バッファの内容をHWに書き出します
        //
        {
            // VRAM 転送を行います
            NNS_GfdDoVramTransfer();

            // 表示情報を書き出します
            G2DDemo_PrintApplyToHW();

            // OAM を書き出します
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



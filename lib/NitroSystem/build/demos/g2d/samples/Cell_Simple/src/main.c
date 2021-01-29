/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - Cell_Simple
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.13 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      Cellの単純な描画をおこなうサンプル
//
//  特徴
//      ･OAMマネージャモジュールを利用しています。
//      ･OBJによる描画とソフトウェアスプライトによる描画を切り替えて描画しています。
//      ･セルはアニメーションを行いません。
//
//  操作方法
//      START ボタン    Cell の出力方法を OBJ <=> SoftwareSprite と
//                      切り替えます。
//      A ボタン        表示する Cell を切り替えます(切り替えるセルが存在するなら)。
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


#define AFFINE_IDX                  3                       // Cell 用 Affine Index
#define CELL_ROTATE_UNIT            0xFF                    // Cell の単位回転量
#define CELL_SCALE_UNIT             ((FX32_ONE >> 7) * 3)   // Cell の単位拡大縮小率
                                                            // scale 値が 0 付近の値にならないようにしてあります
#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell の初期位置 X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell の初期位置 Y

#define INIT_OUTPUT_TYPE            NNS_G2D_OAMTYPE_MAIN    // 出力方法の初期値

#define NUM_OF_OAM                  128                     // OAM マネージャに割り当てる OAM の数
#define NUM_OF_AFFINE               (NUM_OF_OAM / 4)        // OAM マネージャに割り当てる Affine パラメータの数

#define TEX_BASE                    0x0                     // テクスチャベースアドレス
#define TEX_PLTT_BASE               0x0                     // テクスチャパレットベースアドレス



//------------------------------------------------------------------------------
// 構造体定義

//------------------------------------------------------------------------------
//
// セルの情報
//
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

static NNSG2dCellDataBank*          spCellBank = NULL;
static NNSG2dOamManagerInstance     objOamManager;  // OBJ 出力用 OAM マネージャ
static NNSG2dOamManagerInstance     ssOamManager;   // SoftwareSprite 出力用 OAM マネージャ

static GXOamAttr                    tempOamBuffer[NUM_OF_OAM];           // テンポラリ OAM バッファ 

static CellInfo cellInfo = {
    0,                                  // 描画 Cell 番号
    0,                                  // 回転角
    {FX32_ONE, FX32_ONE},               // 拡大率
    {CELL_INIT_POS_X, CELL_INIT_POS_Y}  // Cell 描画位置
};


//------------------------------------------------------------------------------
// プロトタイプ宣言
void VBlankIntr(void);

static NNSG2dCellDataBank* LoadResources(void);
static void ProcessInput(CellInfo* pCellInfo, NNSG2dOamType* pOutputType, u16 numCells);
static void MakeAffineMatrix(MtxFx22* pMtx, MtxFx22* pMtx2D, const CellInfo* pCellInfo);
static void InitApp( void );
static void AppDraw
( 
    NNSG2dOamManagerInstance*   pOamMgr, 
    const CellInfo*             pCellInfo,
    const MtxFx22*              pMtxAffineForCell,
    const MtxFx22*              pMtxAffineForOBJ
);
static void ApplayOamMgrToHW( NNSG2dOamType outputType );





/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  セルバンク、キャラクタデータ、パレットデータをファイルから
                読み込み、キャラクタデータとパレットデータを
                それぞれVRAMにロードします。

  Arguments:    なし。

  Returns:      読み込んだセルバンクへのポインタ。
 *---------------------------------------------------------------------------*/
static NNSG2dCellDataBank* LoadResources(void)
{
    NNSG2dCellDataBank* pCellBank;
    void* pBuf;

    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //----------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/Cell_Simple.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // セルバンクは最後までメインメモリ上で使用するので
        // この pBuf は開放しません。
    }

    //----------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Cell_Simple.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            TEX_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sImageProxy );

        // キャラクタデータを VRAM にコピーしたので
        // この pBuf は開放します。以下同じ。
        G2DDemo_Free( pBuf );
    }
    //--------------------------------------------
    // load character data for 3D (software sprite)
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/Cell_Simple.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            TEX_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sImageProxy );

        G2DDemo_Free( pBuf );
    }

    //--------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/Cell_Simple.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            TEX_PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            TEX_PLTT_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sPaletteProxy);

        G2DDemo_Free( pBuf );
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
static void ProcessInput(CellInfo* pCellInfo, NNSG2dOamType* pOutputType, u16 numCells)
{
    SDK_NULL_ASSERT( pCellInfo );

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
        if( *pOutputType == NNS_G2D_OAMTYPE_MAIN )
        {
            // 一旦OAMをリセットする
            NNS_G2dResetOamManagerBuffer( &objOamManager );
            NNS_G2dApplyOamManagerToHW( &objOamManager );
            
            *pOutputType = NNS_G2D_OAMTYPE_SOFTWAREEMULATION;
        }else{
            *pOutputType = NNS_G2D_OAMTYPE_MAIN;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         MakeAffineMatrix

  Description:  Cell の表示情報を元に Affine 行列を作成します。

  Arguments:    pMtx:       通常の Affine 行列を受け取る MtxFx22 へのポインタ。
                pMtx2D:     2Dグラフィックエンジン用の Affine 行列を
                            受け取る MtxFx22 へのポインタ。
                pCellInfo:  Cell の表示情報をあらわす CellInfo。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void MakeAffineMatrix(MtxFx22* pMtx, MtxFx22* pMtx2D, const CellInfo* pCellInfo)
{
    MtxFx22 tmpMtx;

    // 回転は2つの行列で共通です
    G2DDemo_MTX_Rot22( &tmpMtx, FX_SinIdx( pCellInfo->rotate ), FX_CosIdx( pCellInfo->rotate ) );

    // 通常の Affine 行列を作成します
    MTX_ScaleApply22(
        &tmpMtx,
        pMtx,
        pCellInfo->scale.x,
        pCellInfo->scale.y );

    // 2Dグラフィックエンジン用の Affine 行列を作成します
    // スケール成分を逆数として設定する点が特徴的です。
    // これは、2D グラフィックスエンジンの仕様です。
    // 詳しくは、プログラミングマニュアルをご覧ください。
    MTX_ScaleApply22(
        &tmpMtx,
        pMtx2D,
        (FX32_ONE * FX32_ONE) / pCellInfo->scale.x,
        (FX32_ONE * FX32_ONE) / pCellInfo->scale.y );
}

//------------------------------------------------------------------------------
// アプリケーションの初期化を行います
//------------------------------------------------------------------------------
static void InitApp( void )
{
    BOOL result = TRUE;
    
    //
    // デモライブラリの初期化を行います
    //
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
    }
    
    //
    // OAM マネージャの初期化を行います
    //
    // バッファ転送のパフォーマンスは低下しますが、
    // より自由度の高いOAMマネージャの実体の初期化を行うAPI
    // NNS_G2dGetNewOamManagerInstance()も用意されています。
    //
    {
        NNS_G2dInitOamManagerModule();
        
        // OBJ 用
        {
            result = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode
                     ( 
                        &objOamManager, 
                        0, 
                        NUM_OF_OAM, 
                        NNS_G2D_OAMTYPE_MAIN 
                     );
            SDK_ASSERT( result );
        }
        
        // ソフトウェアスプライト用
        {
            result = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode
                     ( 
                        &ssOamManager, 
                        0, 
                        NUM_OF_OAM, 
                        NNS_G2D_OAMTYPE_SOFTWAREEMULATION 
                     );
            SDK_ASSERT( result );
            // ソフトウェアスプライトのZオフセット増分を指定
            NNS_G2dSetOamManagerSpriteZoffsetStep( &ssOamManager, - FX32_ONE );
        }
    }
    
    //
    // リソース類のロードを行います
    //
    spCellBank = LoadResources();
}

//------------------------------------------------------------------------------
// 描画
//
// 低レベルな描画方法 NNS_G2dMakeCellToOams() で Cellを描画します。
// 上位モジュールのG2dRenderer を 利用する描画方法もあります。
//
//
//       NNS_G2dMakeCellToOams() API は セルをセルを表すOBJ群として指定したバッファに
//       書き出します。本サンプルでは、一旦一時バッファに書き出したあと、その一時バッ
//       ファの内容をOAMマネージャモジュールに登録しています。
//    
// 注意：アフィンパラメータを表す行列 として 
//       pMtxAffineForCell と pMtxAffineForOBJ が
//       存在します。
//       pMtxAffineForOBJ で表される行列は OBJ の参照するアフィンパラメータとし
//       て使用されます。 そのため、OAMマネージャの種類が 2D の場合には、スケー
//       ル分の１した、行列を渡す必要があります。
//
//       pMtxAffineForCell で表される行列は、2D グラフィックスエンジンから参照
//       されることはなく、セルの中のOBJの位置を変換する際に使用されます。 
//
static void AppDraw
( 
    NNSG2dOamManagerInstance*   pOamMgr, 
    const CellInfo*             pCellInfo,
    const MtxFx22*              pMtxAffineForCell,
    const MtxFx22*              pMtxAffineForOBJ
)
{
    // 描画されたOAMの数
    u16                     numOamDrawn = 0;  
    // affine パラメータの設定( affine Index )
    const u16               affineIdx 
        = NNS_G2dEntryOamManagerAffine( pOamMgr, pMtxAffineForOBJ );
    // 表示するセル
    const NNSG2dCellData*   pCell
        = NNS_G2dGetCellDataByIdx( spCellBank, pCellInfo->cellIdx );                    
    SDK_NULL_ASSERT( pCell );
    
    //----------------------------------------------------
    // Cell と等価な OBJリストをテンポラリに書き出します
    //
    numOamDrawn  = NNS_G2dMakeCellToOams(
                        tempOamBuffer,      // 出力先Oamバッファ
                        NUM_OF_OAM,         // 出力先バッファ長
                        pCell,              // 出力対象Cell
                        pMtxAffineForCell,  // Affine 変換     <= 通常の Affine 行列を指定します。
                        &pCellInfo->pos,    // セルの位置         この行列は2Dグラフィックエンジンには
                        affineIdx,          // Affine Index       渡されず、CellのAffine変換に利用されます。
                        TRUE );             // 倍角Affineか？

    SDK_ASSERT( numOamDrawn < NUM_OF_OAM );

    //---------------------------------------------------- 
    // テンポラリに書き出したOBJリストをOAM マネージャへ登録します
    // 
    (void)NNS_G2dEntryOamManagerOam( pOamMgr,tempOamBuffer, numOamDrawn );
    
}

//------------------------------------------------------------------------------
//
// マネージャの内容をHWに書き出します
//
// outputType の種類ごとに処理が分岐しています。
//
static void ApplayOamMgrToHW( NNSG2dOamType outputType )
{ 
    // ----- SoftwareSprite で Cell を描画します
    if( outputType == NNS_G2D_OAMTYPE_SOFTWAREEMULATION )
    {
        u32 texBase = NNS_G2dGetImageLocation( &sImageProxy, NNS_G2D_VRAM_TYPE_3DMAIN );
        u32 pltBase = NNS_G2dGetImagePaletteLocation( &sPaletteProxy, NNS_G2D_VRAM_TYPE_3DMAIN );
        //
        // 本サンプルでは、V-Blank中にマネージャの内容をHWに書き出していますが
        // ソフトウェアスプライト描画の場合には、
        // 必ずしも書き出し処理はV-Blank中である必要はありません。
        //
        NNS_G2dApplyOamManagerToHWSprite( &ssOamManager, &sImageProxy.attr, texBase, pltBase );
        NNS_G2dResetOamManagerBuffer( &ssOamManager );
                
    }else{
    // ----- OBJ で Cell を描画します
        NNS_G2dApplyOamManagerToHW( &objOamManager );
        NNS_G2dResetOamManagerBuffer( &objOamManager );
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
    //----------------------------------------------------
    // アプリケーションの初期化
    InitApp();
    // start display
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }


    //----------------------------------------------------
    // メインループ
    while( TRUE )
    {
        static NNSG2dOamType outputType = INIT_OUTPUT_TYPE;  // Cell 出力方法
        MtxFx22              mtxAffine;                      // 通常 Cell affine 行列
        MtxFx22              mtxAffine2D;                    // OBJアフィンパラメータ用 行列
        
        //-------------------------------------------------
        // 更新
        //
        // 入力を読み取り Cell の表示情報と出力方法を更新します
        ProcessInput( &cellInfo, &outputType, spCellBank->numCells);
        // アフィン変換行列の生成
        MakeAffineMatrix( &mtxAffine, &mtxAffine2D, &cellInfo );
        
        
        //-------------------------------------------------
        // 描画
        //
        // OBJ で表示するか SoftwareSprite で表示するかで
        // OAM マネージャと OBJアフィンパラメータ用 行列を切り替えます。
        // 
        if( outputType == NNS_G2D_OAMTYPE_MAIN )
        {
            AppDraw( &objOamManager, &cellInfo, &mtxAffine, &mtxAffine2D );
        }else{               
            AppDraw( &ssOamManager, &cellInfo, &mtxAffine, &mtxAffine );
        }
        
        //-------------------------------------------------
        // 表示情報の出力
        {
            G2DDemo_PrintOutf(0, 0, "pos:   x=%3d     y=%3d",
                cellInfo.pos.x >> FX32_SHIFT, cellInfo.pos.y >> FX32_SHIFT);
            G2DDemo_PrintOutf(0, 1, "scale: x=%7.3f y=%7.3f",
                (double)cellInfo.scale.x / FX32_ONE, (double)cellInfo.scale.y / FX32_ONE);
            G2DDemo_PrintOutf(0, 2, "rot:   %5d (%6.2f)",
                cellInfo.rotate, 360.0 * cellInfo.rotate / 0x10000);
            G2DDemo_PrintOut(0, 23, (outputType == NNS_G2D_OAMTYPE_MAIN ) ?
                   "OBJ           ": "SoftwareSprite");
        }
        
        //-------------------------------------------------
        //
        // V Blank 待ち
        {
            G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
            SVC_WaitVBlankIntr();
        
            // 表示情報の反映（BGデータの操作）
            G2DDemo_PrintApplyToHW();
        
            //-------------------------------------------------
            //
            // マネージャの内容をHWに書き出します
            //
            ApplayOamMgrToHW( outputType );
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


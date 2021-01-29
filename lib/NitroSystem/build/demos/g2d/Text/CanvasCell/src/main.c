/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - CanvasCell
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      1DマッピングOBJ用に構築されたCharCanvasをセルとして表示します
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include <nnsys/g2d/g2d_Renderer.h>
#include "g2d_textdemolib.h"

#define SCREEN_WIDTH        256
#define SCREEN_HEIGHT       192

#define CANVAS_WIDTH        6       // 文字描画域の幅    (キャラクタ単位)
#define CANVAS_HEIGHT       4       // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS_LEFT         13      // 文字描画域の位置X (ピクセル単位)
#define CANVAS_TOP          17      // 文字描画域の位置Y (ピクセル単位)

#define TEXT_HSPACE         1       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE         1       // 文字列描画時の行間   (ピクセル単位)

#define CHARACTER_OFFSET    1       // 使用するキャラクタ列の先頭キャラクタネーム
                                    //

#define MAX_OBJ_NUM         128
#define REPEAT_THRESHOLD    22

#define CHARACTER_WIDTH     8
#define CHARACTER_HEIGHT    8

#define NUM_OF_OAM          128                 // OAM マネージャに割り当てる OAM の数


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dFont                  gFont;          // フォント
NNSG2dCharCanvas            gCanvas;        // CharCanvas
NNSG2dTextCanvas            gTextCanvas;    // TextCanvas

NNSG2dRendererInstance      gRenderer;
NNSG2dRenderSurface         gSurface;
NNSG2dImageProxy            gImageProxy;
NNSG2dImagePaletteProxy     gPaletteProxy;
NNSG2dOamManagerInstance    gOamManager;

NNSG2dCellData*             gpCanvasCell;



//****************************************************************************
// レンダラ関連の処理
//****************************************************************************
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

    return NNS_G2dEntryOamManagerOamWithAffineIdx( &gOamManager, pOam, affineIndex );
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
    return NNS_G2dEntryOamManagerAffine( &gOamManager, mtx );
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

    result = NNS_G2dGetNewOamManagerInstance(
                pOamManager,
                0, NUM_OF_OAM,
                0, NUM_OF_OAM/4,
                NNS_G2D_OAMTYPE_MAIN
    );
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
                NNSG2dRendererInstance*     pRenderer,
                NNSG2dRenderSurface*        pSurface,
                NNSG2dImageProxy*           pImageProxy,
                NNSG2dImagePaletteProxy*    pPaletteProxy
)
{
    NNSG2dViewRect* pRect;

    SDK_NULL_ASSERT( pImageProxy );
    SDK_NULL_ASSERT( pPaletteProxy );
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pSurface );

    //---- ImageProxy をダミーアドレスで初期化
    NNS_G2dInitImageProxy( pImageProxy );
    NNS_G2dSetImageLocation( pImageProxy, NNS_G2D_VRAM_TYPE_2DMAIN, NULL );

    //---- PaletteProxy ダミーアドレスで初期化
    NNS_G2dInitImagePaletteProxy( pPaletteProxy );
    NNS_G2dSetImagePaletteLocation( pPaletteProxy, NNS_G2D_VRAM_TYPE_2DMAIN, NULL );

    //---- サーフェスを初期化
    NNS_G2dInitRenderSurface( pSurface );
    pRect = &(pSurface->viewRect);

    pRect->posTopLeft.x = FX32_ONE * 0;
    pRect->posTopLeft.y = FX32_ONE * 0;
    pRect->sizeView.x   = FX32_ONE * SCREEN_WIDTH;
    pRect->sizeView.y   = FX32_ONE * SCREEN_HEIGHT;

    pSurface->pFuncOamRegister          = CallBackAddOam;
    pSurface->pFuncOamAffineRegister    = CallBackAddAffine;
    pSurface->type                      = NNS_G2D_SURFACETYPE_MAIN2D;

    //---- レンダラを初期化
    NNS_G2dInitRenderer( pRenderer );
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, pImageProxy, pPaletteProxy );
}



//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  OBJ表示の初期化をします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitScreen(void)
{
    // マッピングモード設定 1D 256 KB
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_256K);

    // OBJ を可視に
    CMN_SetPlaneVisible( GX_PLANEMASK_OBJ );

    // カラーパレットを設定
    GX_LoadOBJPltt(TXTColorPalette, 0, sizeof(TXTColorPalette));
}



/*---------------------------------------------------------------------------*
  Name:         InitCanvas

  Description:  文字列描画の初期化をします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitCanvas(void)
{
    GXCharFmt16* const pCharBase = (GXCharFmt16*)G2_GetOBJCharPtr();
    GXOamAttr* const pObjBase = (GXOamAttr*)HW_OAM;
    const int cOffset = CHARACTER_OFFSET;
    const NNSG2dOBJVramMode vramMode = NNS_G2D_OBJVRAMMODE_256K;

    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForOBJ1D(
        &gCanvas,
        pCharBase + (cOffset << vramMode),
            // VRAM参照可能サイズによってキャラクタネーム境界が異なるため補正する

        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // TextCanvasの初期化
    NNS_G2dTextCanvasInit(
        &gTextCanvas,
        &gCanvas,
        &gFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );

    // セルを構築
    {
        //---- 回転中心を CharCanvas の中心にする
        const int x                 = CANVAS_WIDTH  * CHARACTER_WIDTH  / 2;
        const int y                 = CANVAS_HEIGHT * CHARACTER_HEIGHT / 2;
        const int priority          = 0;
        const GXOamMode mode        = GX_OAM_MODE_NORMAL;
        const BOOL mosaic           = FALSE;
        const GXOamEffect effect    = GX_OAM_EFFECT_AFFINE_DOUBLE;
        const GXOamColorMode color  = GX_OAM_COLORMODE_16;
        const int cParam            = TXT_CPALETTE_MAIN;
        const BOOL makeBR           = FALSE;

        const size_t szByte         = NNS_G2dCharCanvasCalcCellDataSize1D(&gCanvas, makeBR);

        //---- セルデータ用メモリ確保
        gpCanvasCell = (NNSG2dCellData*)TXT_Alloc(szByte);
        SDK_NULL_ASSERT( gpCanvasCell );

        NNS_G2dCharCanvasMakeCell1D(
            gpCanvasCell,
            &gCanvas,
            x,
            y,
            priority,
            mode,
            mosaic,
            effect,
            color,
            cOffset,
            cParam,
            vramMode,
            makeBR
        );
    }
}



/*---------------------------------------------------------------------------*
  Name:         SampleDraw

  Description:  サンプルの描画処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleDraw(void)
{
    // Canvasクリア
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_NULL);

    // 文字描画
    NNS_G2dTextCanvasDrawText(&gTextCanvas, 1, 1, TXT_COLOR_BLACK, TXT_DRAWTEXT_FLAG_DEFAULT,
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
    );
}



/*---------------------------------------------------------------------------*
  Name:         PrintSampleInfo

  Description:  サンプルの情報をした画面に出力します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void PrintSampleInfo(void)
{
    DTX_PrintLine(
        "1D mapping OBJ CharCanvas Cell Sample\n"
        "operation\n"
        "  (none)\n"
    );
    DTX_PrintLine("canvas size character:  %3d x %3d", CANVAS_WIDTH, CANVAS_HEIGHT);
    DTX_PrintLine("canvas size pixel:      %3d x %3d", CANVAS_WIDTH * 8, CANVAS_HEIGHT * 8);
    DTX_PrintLine("bounding sphere radius: %3d", NNS_G2dGetCellBoundingSphereR(gpCanvasCell));
}



/*---------------------------------------------------------------------------*
  Name:         SampleInit

  Description:  サンプルの初期化処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleInit(void)
{
    // フォントを読み込みます
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    InitOamManager( &gOamManager );
    InitRenderer( &gRenderer, &gSurface, &gImageProxy, &gPaletteProxy );

    InitScreen();
    InitCanvas();

    SampleDraw();

    PrintSampleInfo();
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  サンプルのメイン処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    static fx32 x       = SCREEN_WIDTH /2 * FX32_ONE;
    static fx32 y       = SCREEN_HEIGHT/2 * FX32_ONE;
    static fx32 scale   = FX32_ONE;
    static u16  rot     = 0;
    static fx32 vx      = FX32_ONE;
    static fx32 vy      = FX32_ONE;
    static fx32 vs      = FX32_ONE/120;
    static u16  vr      = 0x10000/0xFF;

    //---- セルの表示パラメータを更新します
    {
        const int bsr = NNS_G2dGetCellBoundingSphereR( gpCanvasCell );

        x       += vx;
        y       += vy;
//      scale   += vs;
        rot     += vr;

        //---- ディスプレイ端で反射する
        if( (x < bsr * scale) || (SCREEN_WIDTH * FX32_ONE - bsr * scale < x) )
        {
            vx *= -1;
        }
        if( (y < bsr * scale) || (SCREEN_HEIGHT * FX32_ONE - bsr * scale < y) )
        {
            vy *= -1;
        }
        if( (scale < FX32_ONE/2) || (FX32_ONE * 2 < scale) )
        {
            vs *= -1;
        }
    }

    //---- CharCanvas セルを描画します
    NNS_G2dBeginRendering( &gRenderer );
    {
        NNS_G2dPushMtx();
        {
            NNS_G2dTranslate( x, y, 0 );
            NNS_G2dRotZ( FX_SinIdx(rot), FX_CosIdx(rot) );
            NNS_G2dScale( scale, scale, FX32_ONE );

            NNS_G2dDrawCell( gpCanvasCell );
        }
        NNS_G2dPopMtx();
    }
    NNS_G2dEndRendering();
}


//****************************************************************************
// Main
//****************************************************************************


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // Initilizing App.
    {
        // SDK とデモライブラリを初期化
        OS_Init();
        TXT_Init();

        // 背景を設定
        TXT_SetupBackground();

        // サンプルの初期化処理
        SampleInit();
    }

    // start display
    {
        CMN_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    // Main loop
    while( TRUE )
    {
        CMN_ReadGamePad();

        // サンプルのメイン処理
        SampleMain();

        CMN_WaitVBlankIntr();

        // セルデータの書き出し
        NNS_G2dApplyOamManagerToHW( &gOamManager );
        NNS_G2dResetOamManagerBuffer( &gOamManager );

        // 情報出力の表示
        DTX_Reflect();
    }
}


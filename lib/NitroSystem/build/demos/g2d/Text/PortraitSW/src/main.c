/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - PortraitSW
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
//      縦画面のための画面描画を行います。
//      縦画面用に変換したフォントを使用して描画を行います。
//
//  操作方法
//      十字ボタンの押した方向が画面上となるように
//      フォントと表示位置を切り替えます。
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"


#define CHARACTER_WIDTH     8
#define CHARACTER_HEIGHT    8

#define CANVAS_WIDTH        28      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS_HEIGHT       20      // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS_LEFT         2       // 文字描画域の位置X (キャラクタ単位)
#define CANVAS_TOP          2       // 文字描画域の位置Y (キャラクタ単位)

#define BG_WIDTH            32      // 文字列を描画する BG の幅   (キャラクタ単位)
#define BG_HEIGHT           32      // 文字列を描画する BG の高さ (キャラクタ単位)

#define TEXT_HSPACE         1       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE         1       // 文字列描画時の行間   (ピクセル単位)

#define CHARACTER_OFFSET    1       // 使用するキャラクタ列の開始番号


//------------------------------------------------------------------------------
// 型

typedef enum Direction
{
    DIR_H_0,    // 横書き 画面の左から右へ
    DIR_H_90,   // 横書き 画面の上から下
    DIR_H_180,  // 横書き 画面の右から左へ
    DIR_H_270,  // 横書き 画面の下から上へ
    DIR_NULL
}
Direction;

typedef struct FontInfo
{
    const char* name;
    Direction   dir;
    NNSG2dFont  res;
}
FontInfo;


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas

FontInfo gFonts[] =
{
    {"/data/fontu8.NFTR",     DIR_H_0 },
    {"/data/fontu8_90.NFTR",  DIR_H_90 },
    {"/data/fontu8_180.NFTR", DIR_H_180 },
    {"/data/fontu8_270.NFTR", DIR_H_270 },
};

// 表示するテキスト
const char* const sSampleText = "abcdefghijklmnopqrstuvwxyz\n"
                                "The quick brown fox jumps over\n"
                                "the lazy dog\n";



//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  BG 面を設定します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitScreen(void)
{
    // BG 1 を設定
    G2_SetBG1Control(
        GX_BG_SCRSIZE_TEXT_256x256,     // スクリーンサイズ 256x256
        GX_BG_COLORMODE_16,             // カラーモード     16色
        GX_BG_SCRBASE_0x0000,           // スクリーンベース
        GX_BG_CHARBASE_0x00000,         // キャラクタベース
        GX_BG_EXTPLTT_01                // 拡張パレットスロット
    );

    // BG1 を可視に
    CMN_SetPlaneVisible( GX_PLANEMASK_BG1 );

    // カラーパレットを設定
    GX_LoadBGPltt(TXTColorPalette, 0, sizeof(TXTColorPalette));
}



/*---------------------------------------------------------------------------*
  Name:         InitCanvas

  Description:  文字列描画の初期化をします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitCanvas(void)
{
    GXCharFmt16*  pChrBase = (GXCharFmt16*) G2_GetBG1CharPtr();
    GXScrFmtText* pScrBase = (GXScrFmtText*)G2_GetBG1ScrPtr();
    int cOffset = CHARACTER_OFFSET;

    // スクリーンをクリア
    MI_CpuClear16(G2_GetBG1ScrPtr(), sizeof(GXScrFmtText) * BG_WIDTH * BG_HEIGHT);

    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        pChrBase + cOffset,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // TextCanvasの初期化
    NNS_G2dTextCanvasInit(
        &gTextCanvas,
        &gCanvas,
        &gFonts[0].res,
        TEXT_HSPACE,
        TEXT_VSPACE
    );

    // スクリーンを設定
    NNS_G2dMapScrToCharText(
        pScrBase,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        CANVAS_LEFT,
        CANVAS_TOP,
        NNS_G2D_TEXT_BG_WIDTH_256,
        cOffset,
        TXT_CPALETTE_MAIN
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleDraw

  Description:  サンプル文字列を描画します。

  Arguments:    x:      描画する位置 x
                y:      描画する位置 y
                pFont:  描画に使用するフォント

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleDraw(int x, int y, const NNSG2dFont* pFont)
{
    // 背景クリア
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // フォント変更
    NNS_G2dTextCanvasSetFont(&gTextCanvas, pFont);

    // テキスト描画
    NNS_G2dTextCanvasDrawText(&gTextCanvas, x, y, TXT_COLOR_BLACK,
            TXT_DRAWTEXT_FLAG_DEFAULT, sSampleText);
}



/*---------------------------------------------------------------------------*
  Name:         SampleInit

  Description:  サンプルの初期設定処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleInit(void)
{
    int i;

    // フォント読み込み
    for( i = 0; i < ARY_SIZEOF(gFonts); ++i )
    {
        TXT_LoadFont( &gFonts[i].res, gFonts[i].name );
    }


    // スクリーン設定
    InitScreen();

    // キャンバス構築
    InitCanvas();
}



/*---------------------------------------------------------------------------*
  Name:         SearchFont

  Description:  指定された方向用のフォントを探します。

  Arguments:    pfis:   フォント情報の配列
                dir:    対象の方向

  Returns:      見つかったフォントを返します。
 *---------------------------------------------------------------------------*/
static const NNSG2dFont* SearchFont(const FontInfo* pfis, Direction dir)
{
    int i;

    for( i = 0; i < ARY_SIZEOF(gFonts); ++i )
    {
        if( pfis[i].dir == dir )
        {
            return &pfis[i].res;
        }
    }

    return NULL;
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  サンプルのメイン処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    static Direction rotation = DIR_NULL;
    BOOL bUpdated = FALSE;

    //---- 入力に応じて表示の方向を変更します。
    if( CMN_IsTrigger(PAD_KEY_UP) || rotation == DIR_NULL )
    {
        rotation = DIR_H_0;
        bUpdated = TRUE;
    }
    if( CMN_IsTrigger(PAD_KEY_DOWN) )
    {
        rotation = DIR_H_180;
        bUpdated = TRUE;
    }
    if( CMN_IsTrigger(PAD_KEY_LEFT) )
    {
        rotation = DIR_H_270;
        bUpdated = TRUE;
    }
    if( CMN_IsTrigger(PAD_KEY_RIGHT) )
    {
        rotation = DIR_H_90;
        bUpdated = TRUE;
    }

    //---- 方向が変更された場合に変更を反映します。
    if( bUpdated )
    {
        static const int canvas_width  = CANVAS_WIDTH  * CHARACTER_WIDTH;
        static const int canvas_height = CANVAS_HEIGHT * CHARACTER_HEIGHT;
        const NNSG2dFont* pFont = SearchFont(gFonts, rotation);
        int x = 0;
        int y = 0;

        NNS_G2D_POINTER_ASSERT( pFont );

        //---- 方向に応じて始点を調整します。
        switch( rotation )
        {
        case DIR_H_270: y = canvas_height;  break;
        case DIR_H_90:  x = canvas_width;  break;
        case DIR_H_180: x = canvas_width;
                        y = canvas_height;  break;
        }

        //---- 方向別のフォントを用いてサンプル文字列を描画します。
        SampleDraw(x, y, pFont);


        //---- メッセージ表示
        DTX_PrintLine("PortraitSW demo");
        DTX_PrintLine("  operation:");
        DTX_PrintLine("    +: change  direction");
        DTX_PrintLine("");
        DTX_PrintLine("direction:");
        switch( rotation )
        {
        case DIR_H_0:   DTX_PrintLine("  left to right"); break;
        case DIR_H_90:  DTX_PrintLine("  top to bottom"); break;
        case DIR_H_180: DTX_PrintLine("  right to left"); break;
        case DIR_H_270: DTX_PrintLine("  bottom to up");  break;
        }
    }
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

        // デモ用の前処理
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

        CMN_WaitVBlankIntr();
        SampleMain();

        // 情報出力の表示
        DTX_Reflect();
    }
}


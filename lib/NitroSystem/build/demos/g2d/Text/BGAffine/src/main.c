/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - BGAffine
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
//      Affine BG での文字列表示サンプル（256色モード）
//      Affine BG では256キャラクタしか使用できないので、
//      このデモでは全ての文字を表示するのに十分なサイズの CharCanvas を
//      作成できていません。
//
//  操作方法
//      操作なし。
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"


// Affine BG では256キャラクタしか使えない事に注意しなければなりません
// これは CANVAS*_WIDTH * CANVAS*_HEIGHT の和が 256以下でなければならない事を意味します。

#define CANVAS0_WIDTH       10      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS0_HEIGHT      5       // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS0_LEFT        1       // 文字描画域の位置X (キャラクタ単位)
#define CANVAS0_TOP         1       // 文字描画域の位置Y (キャラクタ単位)

#define CANVAS1_WIDTH       20      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS1_HEIGHT      6       // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS1_LEFT        3       // 文字描画域の位置X (キャラクタ単位)
#define CANVAS1_TOP         15      // 文字描画域の位置Y (キャラクタ単位)

#define CANVAS2_WIDTH       10      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS2_HEIGHT      6      // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS2_LEFT        15      // 文字描画域の位置X (キャラクタ単位)
#define CANVAS2_TOP         3       // 文字描画域の位置Y (キャラクタ単位)

#define TEXT_HSPACE0        4       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE0        4       // 文字列描画時の行間   (ピクセル単位)

#define TEXT_HSPACE1        1       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE1        1       // 文字列描画時の行間   (ピクセル単位)

#define CHARACTER_OFFSET    1       // 使用するキャラクタ列の開始番号


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dFont           gFont;             // フォント
NNSG2dCharCanvas     gCanvas[3];        // CharCanvas x3
NNSG2dTextCanvas     gTextCanvas[3];    // TextCanvas x3



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
    // BG 2 を設定
    G2_SetBG2ControlAffine(
        GX_BG_SCRSIZE_AFFINE_256x256,   // スクリーンサイズ 256x256
        GX_BG_AREAOVER_XLU,             // 外領域           透明
        GX_BG_SCRBASE_0x0000,           // スクリーンベース
        GX_BG_CHARBASE_0x00000          // キャラクタベース
    );

    // BG2 を可視に
    CMN_SetPlaneVisible( GX_PLANEMASK_BG2 );

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
    // BGキャラクタベースへのポインタ
    GXCharFmt256* pCharBase = (GXCharFmt256*)G2_GetBG2CharPtr();
    // BGスクリーンベースへのポインタ
    GXScrAffine32x32* pScr = (GXScrAffine32x32*)G2_GetBG1ScrPtr();
    int cOffset = CHARACTER_OFFSET;

    // フォントを読み込みます
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    // CharCanvas の初期化と BG スクリーンの設定
    // CharCanvas 0
    NNS_G2dCharCanvasInitForBG(
        &gCanvas[0],                    // CharCanvasへのポインタ
        pCharBase + cOffset,            // 使用するキャラクタ列先頭へのポインタ
        CANVAS0_WIDTH,                  // CharCanvas幅
        CANVAS0_HEIGHT,                 // CharCanvas高さ
        NNS_G2D_CHARA_COLORMODE_256     // カラーモード
    );
    NNS_G2dMapScrToCharAffine(
        &( pScr->scr[CANVAS0_TOP][CANVAS0_LEFT] ),  // CharCanvas表示位置のスクリーンへのポインタ
        CANVAS0_WIDTH,                              // CharCanvas幅
        CANVAS0_HEIGHT,                             // CharCanvas高さ
        NNS_G2D_AFFINE_BG_WIDTH_256,                // スクリーン幅
        cOffset                                     // 使用するキャラクタ列先頭のキャラクタ番号
    );

    // CharCanvas 0 が使う分のキャラクタ数をオフセットに加算
    cOffset += CANVAS0_WIDTH * CANVAS0_HEIGHT;

    NNS_G2dCharCanvasInitForBG(
        &gCanvas[1],
        pCharBase + cOffset,
        CANVAS1_WIDTH,
        CANVAS1_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_256
    );
    NNS_G2dMapScrToCharAffine(
        &( pScr->scr[CANVAS1_TOP][CANVAS1_LEFT] ),
        CANVAS1_WIDTH,
        CANVAS1_HEIGHT,
        NNS_G2D_AFFINE_BG_WIDTH_256,
        cOffset
    );

    // CharCanvas 1 が使う分のキャラクタ数をオフセットに加算
    cOffset += CANVAS1_WIDTH * CANVAS1_HEIGHT;

    NNS_G2dCharCanvasInitForBG(
        &gCanvas[2],
        pCharBase + cOffset,
        CANVAS2_WIDTH,
        CANVAS2_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_256
    );
    NNS_G2dMapScrToCharAffine(
        &( pScr->scr[CANVAS2_TOP][CANVAS2_LEFT] ),
        CANVAS2_WIDTH,
        CANVAS2_HEIGHT,
        NNS_G2D_AFFINE_BG_WIDTH_256,
        cOffset
    );

    // TextCanvas の初期化
    // CharCanvas 毎に TextCanvas を用意することもできますし、
    // 複数の TextCanvas で1つの CharCanvas を共有する事もできます。
    // 複数の CharCanvas を1つの TextCanvas で使う場合は
    // TextCanvas の CharCanvas を置き換えながら使う事になります。

    // 1) 複数の TextCanvas で1つの CharCanvas を共有
    // CharCanvas 1 を TextCanvas 0, 1 で共有
    {
        NNS_G2dTextCanvasInit(
            &gTextCanvas[0],        // TextCanvasへのポインタ
            &gCanvas[1],            // 描画先のCharCanvasへのポインタ
            &gFont,                 // 描画に用いるフォントへのポインタ
            TEXT_HSPACE0,           // 文字間
            TEXT_VSPACE0            // 行間
        );
        NNS_G2dTextCanvasInit(
            &gTextCanvas[1],
            &gCanvas[1],
            &gFont,
            TEXT_HSPACE1,
            TEXT_VSPACE1
        );
    }

    // 2) CharCanvas 毎に TextCanvas を用意
    // CharCanvas 2 を TextCanvas 2 単独で使用
    {
        NNS_G2dTextCanvasInit(
            &gTextCanvas[2],
            &gCanvas[2],
            &gFont,
            TEXT_HSPACE0,
            TEXT_VSPACE0
        );
    }
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  サンプルのメイン処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    InitScreen();
    InitCanvas();

    // それぞれの CharCanvas を TXT_COLOR_WHITE で塗りつぶし
    NNS_G2dCharCanvasClear(&gCanvas[0], TXT_COLOR_WHITE);
    NNS_G2dCharCanvasClear(&gCanvas[1], TXT_COLOR_WHITE);
    NNS_G2dCharCanvasClear(&gCanvas[2], TXT_COLOR_WHITE);

    // 文字列描画
    // TextCanvas 0 (->CharCanvas 1) に描画
    NNS_G2dTextCanvasDrawText(&gTextCanvas[0], 0, 0, TXT_COLOR_CYAN, TXT_DRAWTEXT_FLAG_DEFAULT,
        "TextCanvas0 CharCanvas1\n"
        "abcdefg"
    );

    // TextCanvas 1 (->CharCanvas 1) に描画
    NNS_G2dTextCanvasDrawText(&gTextCanvas[1], 0, 30, TXT_COLOR_MAGENTA, TXT_DRAWTEXT_FLAG_DEFAULT,
        "TextCanvas1 CharCanvas1\n"
        "hijklmn"
    );

    // TextCanvas 2 (->CharCanvas 2) に描画
    NNS_G2dTextCanvasDrawText(&gTextCanvas[2], 0, 0, TXT_COLOR_GREEN, TXT_DRAWTEXT_FLAG_DEFAULT,
        "TextCanvas2\n"
        "CharCanvas2\n"
        "opqrstu"
    );


    // 3) 複数の CharCanvas を1つの TextCanvas で使う
    {
        // TextCanvas 1 の CharCanvas を入れ替え
        NNS_G2dTextCanvasSetCharCanvas(&gTextCanvas[1], &gCanvas[0]);

        // TextCanvas 1 (->CharCanvas 0) に描画
        NNS_G2dTextCanvasDrawText(&gTextCanvas[1], 0, 0, TXT_COLOR_BLUE, TXT_DRAWTEXT_FLAG_DEFAULT,
            "TextCanvas1\n"
            "CharCanvas0\n"
            "vwxyz"
        );
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

        // サンプルのメイン処理
        SampleMain();
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

        // 情報出力の表示
        DTX_Reflect();
    }
}


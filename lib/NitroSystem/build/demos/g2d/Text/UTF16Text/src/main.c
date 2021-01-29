/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - UTF16Text
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      UTF-16文字列の表示方法サンプル。
//
//  操作方法
//      なし。
// ============================================================================

// 文字列描画関数が扱う文字をUnicodeにする
#define NNS_G2D_UNICODE

#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"


#define CANVAS_WIDTH        22      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS_HEIGHT       16      // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS_LEFT         5       // 文字描画域の位置X (キャラクタ単位)
#define CANVAS_TOP          4       // 文字描画域の位置Y (キャラクタ単位)

#define TEXT_HSPACE         1       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE         1       // 文字列描画時の行間   (ピクセル単位)

#define CHARACTER_OFFSET    1       // 使用するキャラクタ列の開始番号


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dFont              gFont;          // フォント
NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas



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
    GXCharFmt16* pCharBase = (GXCharFmt16*)G2_GetBG1CharPtr();
    int cOffset = CHARACTER_OFFSET;

    // フォントを読み込みます
    {
//      他のデモと同じように TXT_LoadFont を使ってもフォントを読み込めます。
//      このデモでは NNS_G2dFontInitUTF16 を使う場合を示しています。
//        TXT_LoadFont( &gFont, TXT_UTF16_FONTRESOURCE_NAME );

        void* pFontFile;
        u32 size;

        size = TXT_LoadFile( &pFontFile, TXT_UTF16_FONTRESOURCE_NAME );
        NNS_G2D_ASSERT( size > 0 );

        NNS_G2dFontInitUTF16(&gFont, pFontFile);
        NNS_G2dPrintFont(&gFont);
    }

    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        pCharBase + cOffset,
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

    // スクリーンを設定
    NNS_G2dMapScrToCharText(
        G2_GetBG1ScrPtr(),
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        CANVAS_LEFT,
        CANVAS_TOP,
        NNS_G2D_TEXT_BG_WIDTH_256,
        CHARACTER_OFFSET,
        TXT_CPALETTE_MAIN
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  サンプルのメイン処理。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    InitScreen();
    InitCanvas();

    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);
    NNS_G2dTextCanvasDrawText(&gTextCanvas, 0, 0,
        TXT_COLOR_BLACK, TXT_DRAWTEXT_FLAG_DEFAULT,
        L"UTF-16\n"
        L"0123456789\n"                                     // <- 数字
        L"abcdefg ABCDEFG\n"                                // <- アルファベット
        L"\x00E0\x00E1\x00E2\x00E3\x00E4\x00E5\n"           // <- 欧州文字
        L"\x3042\x3044\x3046\x3048\x304A\x6F22\x5B57"       // <- 日本語文字
    );
    // 日本語文字部分は ShiftJIS で記述する事が出来ますが
    // ShiftJIS に対応していない環境のために文字コードで記述しています
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


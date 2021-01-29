/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - DrawLetter
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
//      文字描画関数の使用サンプルです。
//      1文字ずつ色を変えながら階段状に文字を描画します。
//
//  操作方法
//      操作なし。
// ============================================================================


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

NNSG2dFont                  gFont;          // フォント
static NNSG2dCharCanvas     gCanvas;            // CharCanvas



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
    // 他のデモと違い、わかりやすいように左スペースを持つフォントを使用します
    TXT_LoadFont( &gFont, DEBUG_FONTRESOURCE_NAME );

    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        pCharBase + cOffset,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // スクリーンを設定
    NNS_G2dMapScrToCharText(
        G2_GetBG1ScrPtr(),
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        CANVAS_LEFT,
        CANVAS_TOP,
        NNS_G2D_TEXT_BG_WIDTH_256,
        cOffset,
        TXT_CPALETTE_USERCOLOR
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  サンプルのメイン処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
#define NEXT_COLOR(c)   ((u8)(((c) + 1) & 0xF))
static void SampleMain(void)
{
    // 表示する文字列
    const char* const str = "abcdefghijklmnopqrstuvwxyz";
    const char* pos;
    const u8 bgColor = TXT_UCOLOR_GRAY;

    int x = 0;
    int y = 0;
    u8 clr = 0;
    NNSG2dGlyph glyph;

    InitScreen();
    InitCanvas();

    // 背景クリア
    NNS_G2dCharCanvasClear(&gCanvas, bgColor);

    // DrawChar で描画
    for( pos = str; *pos != '\0'; pos++ )
    {
        int width;

        // NNS_G2dCharCanvasDrawChar の返り値は文字幅(=左スペース幅 + グリフ幅 + 右スペース幅) です
        width = NNS_G2dCharCanvasDrawChar(&gCanvas, &gFont, x, y, clr, *pos);

        // 文字幅を加算します
        x += width;

        x += TEXT_HSPACE;
        y += TEXT_VSPACE;

        // 次のカラー
        clr = NEXT_COLOR(clr);
        if( clr == bgColor )
        {
            clr = NEXT_COLOR(clr);
        }
    }

    // DrawGlyph で描画
    x = 0;
    for( pos = str; *pos != '\0'; pos++ )
    {
        NNS_G2dFontGetGlyph(&glyph, &gFont, *pos);
        NNS_G2dCharCanvasDrawGlyph(&gCanvas, &gFont, x, y, clr, &glyph);

        // グリフ幅のみを加算します。
        x += glyph.pWidths->glyphWidth;

        x += TEXT_HSPACE;
        y += TEXT_VSPACE;

        // 次のカラー
        clr = NEXT_COLOR(clr);
        if( clr == bgColor )
        {
            clr = NEXT_COLOR(clr);
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


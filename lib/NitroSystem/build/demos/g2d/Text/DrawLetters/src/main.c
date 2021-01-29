/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - DrawLetters
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
//      文字列描画関数の利用サンプルです。
//      NNS_G2dTextCanvasDrawString
//      NNS_G2dTextCanvasDrawText
//      NNS_G2dTextCanvasDrawTextRect
//      の 3 関数においてフラグによる描画の違いを示します。
//
//  操作方法
//      デモ実行時の下画面参照
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"

#define CHARACTER_WIDTH     8
#define CHARACTER_HEIGHT    8

#define CANVAS_WIDTH        22      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS_HEIGHT       16      // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS_LEFT         5       // 文字描画域の位置X (キャラクタ単位)
#define CANVAS_TOP          4       // 文字描画域の位置Y (キャラクタ単位)

#define TEXT_HSPACE         1       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE         1       // 文字列描画時の行間   (ピクセル単位)

#define CHARACTER_OFFSET    1       // 使用するキャラクタ列の開始番号

#define TRANS_TASK_NUM  1


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dFont              gFont;          // フォント
NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas

// オフスクリーンバッファ
GXCharFmt16             gOffBuffer[CANVAS_HEIGHT][CANVAS_WIDTH];
NNSGfdVramTransferTask  gTrasTask[TRANS_TASK_NUM];

// 表示するテキスト
static const char sSampleText[] =
    "short string\n"
    "It's a long long long long string.\n"
    "\n"
    "after null line";



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
    // フォントを読み込みます
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        &gOffBuffer,
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
  Name:         SampleInit

  Description:  デモ処理の初期化。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleInit(void)
{
    InitScreen();
    InitCanvas();

    // オフスクリーンバッファの転送にVRAM転送マネージャを使います
    NNS_GfdInitVramTransferManager(gTrasTask, TRANS_TASK_NUM);
}



/*---------------------------------------------------------------------------*
  Name:         SampleString

  Description:  NNS_G2dTextCanvasDrawString() のサンプル

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleString(void)
{
    const char* str = sSampleText;
    const int linefeed = NNS_G2dFontGetLineFeed(&gFont) + NNS_G2dTextCanvasGetVSpace(&gTextCanvas);
    const int baselinePos = NNS_G2dFontGetBaselinePos(&gFont);
    int x, y;

    // 情報出力
    {
        DTX_PrintLine("NNS_G2dTextCanvasDrawString Sample");
        DTX_PrintLine("  operation:");
        DTX_PrintLine("    X:     next mode");
        DTX_PrintLine("");
    }

    // 描画位置初期値
    x = 0;
    y = baselinePos;

    // 背景をクリア
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // 文字列描画
    while( str != NULL )
    {
        NNS_G2dTextCanvasDrawString(
            &gTextCanvas,           // 描画するTextCanvasへのポインタ
            x,                      // 描画開始座標 x
            y - baselinePos,        // 描画開始座標 y
            TXT_COLOR_CYAN,         // 文字色
            str,                    // 描画する文字列
            &str                    // 描画終了位置を受け取るバッファへのポインタ
        );

        y += linefeed;
    }
}



/*---------------------------------------------------------------------------*
  Name:         SampleText

  Description:  NNS_G2dTextCanvasDrawText() のサンプル。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleText(void)
{
    u32 flag = 0;

    // 背景をクリア
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // 基準点を通る水平/垂直線を表示
    {
        const int canvas_pw = CANVAS_WIDTH * CHARACTER_WIDTH;
        const int canvas_ph = CANVAS_HEIGHT * CHARACTER_HEIGHT;
        const int origin_x = canvas_pw / 2;
        const int origin_y = canvas_ph / 2;

        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED, 0, origin_y, canvas_pw, 1);
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED, origin_x, 0, 1, canvas_ph);
    }

    // キー入力処理と情報出力
    {
        DTX_PrintLine("NNS_G2dTextCanvasDrawText Sample");
        DTX_PrintLine("  operation:");
        DTX_PrintLine("    up:    origin bottom");
        DTX_PrintLine("    down:  origin top");
        DTX_PrintLine("    left:  origin right");
        DTX_PrintLine("    right: origin left");
        DTX_PrintLine("    Y:     left align");
        DTX_PrintLine("    A:     right align");
        DTX_PrintLine("    X:     next mode");
        DTX_PrintLine("");

        if( CMN_IsPress(PAD_KEY_UP) )
        {
            flag |= NNS_G2D_VERTICALORIGIN_BOTTOM;
            DTX_PrintLine("  NNS_G2D_VERTICALORIGIN_BOTTOM");
        }
        else if( CMN_IsPress(PAD_KEY_DOWN) )
        {
            flag |= NNS_G2D_VERTICALORIGIN_TOP;
            DTX_PrintLine("  NNS_G2D_VERTICALORIGIN_TOP");
        }
        else
        {
            flag |= NNS_G2D_VERTICALORIGIN_MIDDLE;
            DTX_PrintLine("  NNS_G2D_VERTICALORIGIN_MIDDLE");
        }

        if( CMN_IsPress(PAD_KEY_LEFT) )
        {
            flag |= NNS_G2D_HORIZONTALORIGIN_RIGHT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALORIGIN_RIGHT");
        }
        else if( CMN_IsPress(PAD_KEY_RIGHT) )
        {
            flag |= NNS_G2D_HORIZONTALORIGIN_LEFT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALORIGIN_LEFT");
        }
        else
        {
            flag |= NNS_G2D_HORIZONTALORIGIN_CENTER;
            DTX_PrintLine("  NNS_G2D_HORIZONTALORIGIN_CENTER");
        }

        if( CMN_IsPress(PAD_BUTTON_Y) )
        {
            flag |= NNS_G2D_HORIZONTALALIGN_LEFT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_LEFT");
        }
        else if( CMN_IsPress(PAD_BUTTON_A) )
        {
            flag |= NNS_G2D_HORIZONTALALIGN_RIGHT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_RIGHT");
        }
        else
        {
            flag |= NNS_G2D_HORIZONTALALIGN_CENTER;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_CENTER");
        }
    }

    // 文字列描画
    NNS_G2dTextCanvasDrawText(
        &gTextCanvas,                           // 描画するTextCanvasへのポインタ
        CANVAS_WIDTH * CHARACTER_WIDTH / 2,     // 基準点座標   CharCanvas中心
        CANVAS_HEIGHT * CHARACTER_HEIGHT / 2,   // 基準点座標   CharCanvas中心
        TXT_COLOR_MAGENTA,                      // 文字色
        flag,                                   // 描画位置フラグ
        sSampleText                             // 描画する文字列
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleTextRect

  Description:  NNS_G2dTextCanvasDrawTextRect() のサンプル。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleTextRect(void)
{
    const int canvas_pw = CANVAS_WIDTH * CHARACTER_WIDTH;
    const int canvas_ph = CANVAS_HEIGHT * CHARACTER_HEIGHT;
    const int rect_x    = canvas_pw * 1/6;
    const int rect_y    = canvas_ph * 1/6;
    const int rect_w    = canvas_pw * 2/3;
    const int rect_h    = canvas_ph * 2/3;
    u32 flag = 0;

    // 背景をクリア
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // 基準矩形を表示
    {
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED,
            rect_x - 1,      rect_y - 1,      rect_w + 2, 1);
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED,
            rect_x - 1,      rect_y + rect_h, rect_w + 2, 1);
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED,
            rect_x - 1,      rect_y,          1,          rect_h);
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED,
            rect_x + rect_w, rect_y,          1,          rect_h);
    }

    // キー入力処理と情報出力
    {
        DTX_PrintLine("NNS_G2dTextCanvasDrawTextRect Sample");
        DTX_PrintLine("  operation:");
        DTX_PrintLine("    up:    top align");
        DTX_PrintLine("    down:  bottom align");
        DTX_PrintLine("    left:  left align");
        DTX_PrintLine("    right: right align");
        DTX_PrintLine("    X:     next mode");
        DTX_PrintLine("");

        if( CMN_IsPress(PAD_KEY_UP) )
        {
            flag |= NNS_G2D_VERTICALALIGN_TOP;
            DTX_PrintLine("  NNS_G2D_VERTICALALIGN_TOP");
        }
        else if( CMN_IsPress(PAD_KEY_DOWN) )
        {
            flag |= NNS_G2D_VERTICALALIGN_BOTTOM;
            DTX_PrintLine("  NNS_G2D_VERTICALALIGN_BOTTOM");
        }
        else
        {
            flag |= NNS_G2D_VERTICALALIGN_MIDDLE;
            DTX_PrintLine("  NNS_G2D_VERTICALALIGN_MIDDLE");
        }

        if( CMN_IsPress(PAD_KEY_LEFT) )
        {
            flag |= NNS_G2D_HORIZONTALALIGN_LEFT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_LEFT");
        }
        else if( CMN_IsPress(PAD_KEY_RIGHT) )
        {
            flag |= NNS_G2D_HORIZONTALALIGN_RIGHT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_RIGHT");
        }
        else
        {
            flag |= NNS_G2D_HORIZONTALALIGN_CENTER;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_CENTER");
        }
    }

    // 文字列の描画
    NNS_G2dTextCanvasDrawTextRect(
        &gTextCanvas,                       // 描画するTextCanvasへのポインタ
        rect_x,                             // 矩形位置座標
        rect_y,                             // 矩形位置座標
        rect_w,                             // 基準矩形幅
        rect_h,                             // 基準矩形高さ
        TXT_COLOR_GREEN,                    // 文字色
        flag,                               // 描画位置フラグ
        sSampleText                         // 描画する文字列
    );
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
    // 表示モード
    enum
    {
        MODE_STRING,        // NNS_G2dTextCanvasDrawString  のデモ
        MODE_TEXT,          // NNS_G2dTextCanvasDrawText    のデモ
        MODE_TEXT_RECT,     // NNS_G2dTextCanvasDrawTextRectのデモ
        NUM_OF_MODE
    }
    mode = MODE_TEXT_RECT;

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

        // Xボタンでモード変更
        if( CMN_IsTrigger(PAD_BUTTON_X) )
        {
            mode ++;
            mode %= NUM_OF_MODE;
        }

        // モードに応じて処理分岐
        switch( mode )
        {
        case MODE_STRING:       SampleString();     break;
        case MODE_TEXT:         SampleText();       break;
        case MODE_TEXT_RECT:    SampleTextRect();   break;
        }

        // 転送タスクの登録
        (void)NNS_GfdRegisterNewVramTransferTask(
            NNS_GFD_DST_2D_BG1_CHAR_MAIN,
            sizeof(GXCharFmt16) * CHARACTER_OFFSET,
            gOffBuffer,
            sizeof(gOffBuffer)
        );

        CMN_WaitVBlankIntr();

        // 情報出力の表示
        DTX_Reflect();

        // オフスクリーンバッファの転送
        NNS_GfdDoVramTransfer();
    }
}


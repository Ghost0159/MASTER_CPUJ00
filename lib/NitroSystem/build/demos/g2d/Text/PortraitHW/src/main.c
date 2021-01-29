/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - PortraitHW
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
//      横画面用に変換したフォントを用いて描画を行い、
//      BG の回転を使用して縦画面表示を行います。
//
//  操作方法
//      十字ボタンの押した方向が画面上となるように BG を回転します。
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"


#define CHARACTER_WIDTH     8
#define CHARACTER_HEIGHT    8

#define CANVAS_WIDTH        21      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS_HEIGHT       13      // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS_LEFT         2       // 文字描画域の位置X (キャラクタ単位)
#define CANVAS_TOP          7       // 文字描画域の位置Y (キャラクタ単位)

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


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dFont              gFont;          // フォント
NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas


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
    // BG 3 を設定
    // Affine BG か 256x16パレット BG でなければ
    // BG の回転は使用できません
    G2_SetBG3Control256x16Pltt(
        GX_BG_SCRSIZE_256x16PLTT_256x256,   // スクリーンサイズ 256x256
        GX_BG_AREAOVER_XLU,                 // エリアオーバー処理
        GX_BG_SCRBASE_0x0000,               // スクリーンベース
        GX_BG_CHARBASE_0x00000              // キャラクタベース
    );

    // BG 3 を可視に
    CMN_SetPlaneVisible( GX_PLANEMASK_BG3 );

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
    GXCharFmt256* pChrBase = (GXCharFmt256*)G2_GetBG3CharPtr();
    GXScrFmtText* pScrBase = (GXScrFmtText*)G2_GetBG3ScrPtr();
    int cOffset = CHARACTER_OFFSET;

    // スクリーンをクリア
    MI_CpuClear16(G2_GetBG3ScrPtr(), sizeof(GXScrFmtText) * BG_WIDTH * BG_HEIGHT);

    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        pChrBase + cOffset,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_256
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
    NNS_G2dMapScrToChar256x16Pltt(
        pScrBase + CANVAS_TOP * BG_WIDTH
                 + CANVAS_LEFT,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_256x16PLTT_BG_WIDTH_256,
        cOffset,
        0           // 0 番の 256色カラーパレットを指定
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleDraw

  Description:  サンプル文字列を描画します。

  Arguments:    x:      描画する位置 x
                y:      描画する位置 y

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleDraw(int x, int y)
{
    // 背景クリア
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

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
    // フォント読み込み
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    // スクリーン設定
    InitScreen();

    // キャンバス構築
    InitCanvas();

    // テキスト描画
    SampleDraw(0, 0);
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
        int centerX, centerY;
        MtxFx22 m;

        switch( rotation )
        {
        case DIR_H_0:
            {
                MTX_Rot22(&m, FX32_SIN0,   FX32_COS0);
                centerX      = 0;
                centerY      = 0;
            }
            break;

        case DIR_H_90:
            {
                MTX_Rot22(&m, FX32_SIN90, FX32_COS90);
                centerX      = GX_LCD_SIZE_X / 2;
                centerY      = GX_LCD_SIZE_X / 2;
            }
            break;

        case DIR_H_180:
            {
                MTX_Rot22(&m, FX32_SIN180, FX32_COS180);
                centerX      = GX_LCD_SIZE_X / 2;
                centerY      = GX_LCD_SIZE_Y / 2;
            }
            break;

        case DIR_H_270:
            {
                MTX_Rot22(&m, FX32_SIN270, FX32_COS270);
                centerX      = GX_LCD_SIZE_Y / 2;
                centerY      = GX_LCD_SIZE_Y / 2;
            }
            break;

        default:
            return;
        }

        // BG の回転を設定
        G2_SetBG3Affine(&m, centerX, centerY, 0, 0);


        //---- メッセージ表示
        DTX_PrintLine("PortraitHW demo");
        DTX_PrintLine("  operation:");
        DTX_PrintLine("    +: change direction");
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


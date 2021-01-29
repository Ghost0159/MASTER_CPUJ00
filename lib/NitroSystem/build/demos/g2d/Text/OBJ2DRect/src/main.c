/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - OBJ2DRect
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
//      2DマッピングOBJ上での文字描画サンプル。
//      CharCanvasのサイズに応じて必要なOBJ数を表示します。
//
//  操作方法
//      十字ボタン  CharCanvasのサイズを変更する。
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"


#define CANVAS_WIDTH        28      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS_HEIGHT       10      // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS_LEFT         13      // 文字描画域の位置X (ピクセル単位)
#define CANVAS_TOP          17      // 文字描画域の位置Y (ピクセル単位)

#define TEXT_HSPACE         1       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE         1       // 文字列描画時の行間   (ピクセル単位)

#define CHARACTER_OFFSET    32 * 1  // 使用するキャラクタ列の開始番号

#define REPEAT_THRESHOLD    22


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dFont              gFont;          // フォント
NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas

int gCanvasWidth    = CANVAS_WIDTH;
int gCanvasHeight   = CANVAS_HEIGHT;


//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         ResetOAM

  Description:  OAMをリセットします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ResetOAM(void)
{
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
}



/*---------------------------------------------------------------------------*
  Name:         HandleInput

  Description:  入力を読み取ります。

  Arguments:    なし。

  Returns:      入力によって内部状態が変更されたらTRUE。
 *---------------------------------------------------------------------------*/
static BOOL HandleInput(void)
{
    BOOL bChanged = FALSE;

#define REPEAT(key)   ((repeat_count >= REPEAT_THRESHOLD) ? CMN_IsPress(key): CMN_IsTrigger(key))
    {
        static int repeat_count = 0;
        int old_cw = gCanvasWidth;
        int old_ch = gCanvasHeight;

        if( REPEAT(PAD_KEY_UP) )
        {
            gCanvasHeight--;
        }
        if( REPEAT(PAD_KEY_DOWN) )
        {
            gCanvasHeight++;
        }
        if( REPEAT(PAD_KEY_LEFT) )
        {
            gCanvasWidth--;
        }
        if( REPEAT(PAD_KEY_RIGHT) )
        {
            gCanvasWidth++;
        }
        if( gCanvasWidth < 1 )
        {
            gCanvasWidth = 1;
        }
        if( gCanvasHeight < 1 )
        {
            gCanvasHeight = 1;
        }
        if( 32 < gCanvasWidth )
        {
            gCanvasWidth = 32;
        }
        if( 31 < gCanvasHeight )
        {
            gCanvasHeight = 31;
        }

        if( gCanvasWidth != old_cw || gCanvasHeight != old_ch )
        {
            bChanged = TRUE;
        }
        if( CMN_IsPress(PAD_PLUS_KEY_MASK) )
        {
            repeat_count++;
        }
        else
        {
            repeat_count = 0;
        }
    }

    return bChanged;
}



/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  OBJ表示の初期化をします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitScreen(void)
{
    // マッピングモード設定 2D 32 KB
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);

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
    int nObjs;

    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForOBJ2DRect(
        &gCanvas,
        pCharBase + cOffset,
        gCanvasWidth,
        gCanvasHeight,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // OBJを配置
    nObjs = NNS_G2dArrangeOBJ2DRect(
        pObjBase,
        gCanvasWidth,
        gCanvasHeight,
        CANVAS_LEFT,
        CANVAS_WIDTH,
        GX_OAM_COLORMODE_16,
        cOffset
    );

    // OBJを設定
    TXT_SetCharCanvasOBJAttrs(
        pObjBase,               // OAM配列へのポインタ
        nObjs,                  // 対象とするOAMの数
        0,                      // priority
        GX_OAM_MODE_NORMAL,     // OBJモード
        FALSE,                  // モザイク
        GX_OAM_EFFECT_NONE,     // OBJ効果
        TXT_CPALETTE_MAIN,      // パレット番号
        0                       // アフィンパラメータインデックス
    );

    // TextCanvasの初期化
    NNS_G2dTextCanvasInit(
        &gTextCanvas,
        &gCanvas,
        &gFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );

    // 使用中のOBJ数
    DTX_PrintLine("using OBJ:   %3d", nObjs);
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
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // 文字描画
    NNS_G2dTextCanvasDrawText(&gTextCanvas, 1, 1, TXT_COLOR_BLACK, TXT_DRAWTEXT_FLAG_DEFAULT,
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\n"
        "bcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789a\n"
        "cdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ab\n"
        "defghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abc\n"
        "efghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcd\n"
        "fghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcde\n"
        "ghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdef\n"
        "hijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefg\n"
        "ijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefgh\n"
        "jklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghi\n"
        "klmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghij\n"
        "lmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijk\n"
        "mnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijkl\n"
        "nopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklm\n"
        "opqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmn\n"
        "pqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmno\n"
        "qrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnop\n"
        "rstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopq\n"
        "stuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqr\n"
        "tuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrs\n"
        "uvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrst\n"
        "vwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstu\n"
        "wxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuv\n"
        "xyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvw\n"
        "yzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwx\n"
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
        "2D mapping OBJ CharCanvas Sample\n"
        "operation\n"
        "  +  change canvas size\n"
    );
    DTX_PrintLine("canvas size: %2d x %2d", gCanvasWidth, gCanvasHeight);
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

    PrintSampleInfo();

    InitScreen();
    InitCanvas();
    SampleDraw();
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  サンプルのメイン処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    if( HandleInput() )
    {
        PrintSampleInfo();

        ResetOAM();
        InitCanvas();

        SampleDraw();
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

        // 情報出力の表示
        DTX_Reflect();
    }
}


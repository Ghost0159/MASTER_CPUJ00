/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - DoubleBuffering
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
//      ダブルバッファリングONまたはOFFで文字列を表示します。
//      文字列の描画には非常に時間がかかるのでダブルバッファリングを行わないと
//      表示がちらつきます。
//
//  操作方法
//      A : ダブルバッファリングのON/OFFを切り替えます。
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"



#define CANVAS_WIDTH        30      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS_HEIGHT       22      // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS_LEFT         1       // 文字描画域の位置X (キャラクタ単位)
#define CANVAS_TOP          1       // 文字描画域の位置Y (キャラクタ単位)

#define TEXT_HSPACE         1       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE         1       // 文字列描画時の行間   (ピクセル単位)

#define CHARACTER_OFFSET    1       // 使用するキャラクタ列の開始番号


//------------------------------------------------------------------------------
// グローバル変数

// オンスクリーン表示用
NNSG2dCharCanvas        gOnCanvas;
NNSG2dTextCanvas        gOnTextCanvas;

// オフスクリーン表示用
#define TRANS_TASK_NUM  1
NNSG2dCharCanvas        gOffCanvas;
NNSG2dTextCanvas        gOffTextCanvas;

  // オフスクリーンバッファ
  //   CharCanvasと同サイズのキャラクタ配列
GXCharFmt16             gOffBuffer[CANVAS_HEIGHT][CANVAS_WIDTH];
NNSGfdVramTransferTask  gTrasTask[TRANS_TASK_NUM];

// 共通
NNSG2dFont              gFont;




//****************************************************************************
// Initialize etc.
//****************************************************************************


/*---------------------------------------------------------------------------*
  Name:         InitCanvasShare

  Description:  オン/オフ共有リソースを初期化します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitCanvasShare(void)
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

    // フォントをロード
    TXT_LoadFont(&gFont, TXT_FONTRESOURCE_NAME);

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
  Name:         InitOnScreenCanvas

  Description:  オンスクリーン表示のCanvasを初期化します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
// オンスクリーン表示時の初期化
static void InitOnScreenCanvas(void)
{
    GXCharFmt16* const charBase = (GXCharFmt16*)G2_GetBG1CharPtr();

    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForBG(
        &gOnCanvas,
        charBase + CHARACTER_OFFSET,    // VRAM のアドレス
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // TextCanvas の初期化
    NNS_G2dTextCanvasInit(
        &gOnTextCanvas,
        &gOnCanvas,
        &gFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );
}



/*---------------------------------------------------------------------------*
  Name:         InitOffScreenCanvas

  Description:  オフスクリーン表示のCanvasを初期化します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
// オフスクリーン表示時の初期化
static void InitOffScreenCanvas(void)
{
    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForBG(
        &gOffCanvas,
        gOffBuffer,             // VRAMのアドレスではなくオフスクリーンバッファのアドレスを渡す
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // TextCanvas の初期化
    NNS_G2dTextCanvasInit(
        &gOffTextCanvas,
        &gOffCanvas,
        &gFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );

    // オフスクリーンバッファの転送にVRAM転送マネージャを使います
    NNS_GfdInitVramTransferManager(gTrasTask, TRANS_TASK_NUM);
}



/*---------------------------------------------------------------------------*
  Name:         SampleInit

  Description:  サンプル描画の初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleInit(void)
{
    // 共有利ソースの初期化
    InitCanvasShare();

    // オンスクリーン表示用初期化
    InitOnScreenCanvas();

    // オフスクリーン表示用初期化
    InitOffScreenCanvas();
}



/*---------------------------------------------------------------------------*
  Name:         SampleDraw

  Description:  サンプルの描画を行います。

  Arguments:    pTextCanvas:    TextCanvas へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleDraw(NNSG2dTextCanvas *pTextCanvas)
{
    static int y = 0;

    // Canvas クリア
    NNS_G2dCharCanvasClear(NNS_G2dTextCanvasGetCharCanvas(pTextCanvas), TXT_COLOR_NULL);

    // 描画
    NNS_G2dTextCanvasDrawText(
        pTextCanvas,
        0, y,
        TXT_COLOR_BLACK,
        TXT_DRAWTEXT_FLAG_DEFAULT,
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
    );

    // 少しずつ下へ。
    y ++;
    if( y > 100 )
    {
        y = 0;
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
    BOOL bDoubleBuffering = FALSE;

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

        {
            // オンオフ描画を切り替えます。
            if( CMN_IsTrigger(PAD_BUTTON_A) )
            {
                bDoubleBuffering = ! bDoubleBuffering;
            }
            DTX_PrintLine(
                "Double Buffering Sample\n"
                "operation\n"
                "  A    on/off double buffering\n"
            );
            DTX_PrintLine(bDoubleBuffering ? "DoubleBuffering: ON": "DoubleBuffering: OFF");
        }

    //----------------------------------
    // LCD 表示期間中

        if( bDoubleBuffering )
        {
            // オフスクリーン表示では転送中以外はいつでも描画できます。
            SampleDraw( &gOffTextCanvas );

            // Vブランク期間中の転送タスクを登録します。
            (void)NNS_GfdRegisterNewVramTransferTask(
                NNS_GFD_DST_2D_BG1_CHAR_MAIN,
                sizeof(GXCharFmt16) * CHARACTER_OFFSET,
                gOffBuffer,
                sizeof(gOffBuffer)
            );
        }
        else
        {
            // オンスクリーン表示では何もする事はありません
        }

    //
    //----------------------------------

        CMN_WaitVBlankIntr();
        DTX_Reflect();

    //----------------------------------
    // V ブランク期間中

        if( bDoubleBuffering )
        {
            // オフスクリーン表示ではVブランク中に描画済みイメージをVRAMへ転送します。
            NNS_GfdDoVramTransfer();
        }
        else
        {
            // オンスクリーン表示ではVブランク中にVRAMに描画します。
            SampleDraw( &gOnTextCanvas );
        }

    //
    //----------------------------------
    }
}


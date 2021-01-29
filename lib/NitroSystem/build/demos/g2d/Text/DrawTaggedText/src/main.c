/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - DrawTaggedText
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
//      NNS_G2dTextCanvasDrawTaggedText() の使用サンプル。
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

// 文字列埋め込み用色番号
#define WHITE   "\x1"
#define BLACK   "\x2"
#define RED     "\x3"
#define GREEN   "\x4"
#define BLUE    "\x5"
#define CYAN    "\x6"
#define MAGENTA "\x7"
#define YELLOW  "\x8"


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dFont          gFont;          // フォント
NNSG2dCharCanvas    gCanvas;        // CharCanvas
NNSG2dTextCanvas    gTextCanvas;    // TextCanvas




//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         SimpleTagCallback

  Description:  NNSG2dTagCallback のサンプル。

  Arguments:    c:      コールバック発生要因の文字コード。
                pInfo:  描画情報構造体へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SimpleTagCallback(u16 c, NNSG2dTagCallbackInfo* pInfo)
{
    if( c == 0x1B )
    {
        const char* pos = (const char*)pInfo->str;

        switch( *pos++ )
        {
        // 0x1B C 0xXX
        case 'C':   // Color
            {
                pInfo->clr = *(u8*)pos;
                pos++;
            }
            break;

        // 0x1B B
        case 'B':   // callBack
            {
                NNS_G2D_POINTER_ASSERT( pInfo->cbParam );

                pos++;  // alignment for NNS_G2D_UNICODE
                pInfo->str = (const NNSG2dChar*)pos;

                ((NNSG2dTagCallback)pInfo->cbParam)( c, pInfo );
            }
            return;

        // 0x1B H 0xXX
        case 'H':   // Horizontal
            {
                s8 hSpace;

                hSpace = *(s8*)pos;
                pos++;

                NNS_G2dTextCanvasSetHSpace(&(pInfo->txn), hSpace);
            }
            break;

        // 0x1B V 0xXX
        case 'V':   // Vertical
            {
                s8 vSpace;

                vSpace = *(s8*)pos;
                pos++;

                NNS_G2dTextCanvasSetVSpace(&(pInfo->txn), vSpace);
           }
            break;

        // 0x1B X 0xXX
        case 'X':   // X
            {
                pInfo->x += *(s8*)pos;
                pos++;
            }
            break;

        // 0x1B Y 0xXX
        case 'Y':   // Y
            {
                pInfo->y += *(s8*)pos;
                pos++;
            }
            break;

        default:
            OS_Warning("NNS_G2dTextCanvasDrawTaggedText: Unknown Tag: %c (0x%02X)\n", *(u8*)(pos-1), *(u8*)(pos-1));
            pos++;  // alignment for NNS_G2D_UNICODE
            break;
        }

        pInfo->str = (const NNSG2dChar*)pos;
    }
}



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
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        pCharBase + cOffset,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // TextCanvas の初期化
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
        cOffset,
        TXT_CPALETTE_MAIN
    );
}




/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  サンプルのメイン処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
#define TAG_BEGIN       "\x1B"
#define CHG_COLOR(clr)  TAG_BEGIN  "C" clr
#define CHG_LFEED(h)    TAG_BEGIN  "L" #h
#define CHG_HSP(h)      TAG_BEGIN  "H" #h
#define CHG_VSP(v)      TAG_BEGIN  "V" #v
#define ADD_X(x)        TAG_BEGIN  "X" #x
#define ADD_Y(y)        TAG_BEGIN  "Y" #y

static void SampleMain(void)
{
    const static char text[] =
        "change color to " CHG_COLOR(RED) "red " CHG_COLOR(GREEN) "green " CHG_COLOR(CYAN) "cyan" CHG_COLOR(BLACK) "\n"
        CHG_HSP(\x5) "change hspace to 5" CHG_HSP(\x1) "\n"
        ADD_X(\x32) ADD_Y(\x32) "jump x+50 y+50";

    InitScreen();
    InitCanvas();

    // 背景クリア
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // タグつき文字列描画
    NNS_G2dTextCanvasDrawTaggedText(
        &gTextCanvas,           // 描画する TextCanvas
        0,                      // 描画開始座標
        0,                      // 描画開始座標
        TXT_COLOR_BLACK,        // 文字色
        text,                   // 描画する文字列
        SimpleTagCallback,      // タグ処理コールバック
        NULL                    // タグ処理コールバック用パラメータ
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


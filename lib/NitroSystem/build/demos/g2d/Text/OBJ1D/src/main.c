/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - OBJ1D
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      1DマッピングOBJ上での文字描画サンプル。
//      CharCanvasのサイズに応じて必要なOBJ数を表示します。
//      OBJ1D ではカラーモード、VRAMモードとキャンバスの辺の長さの
//      組み合わせによっては正しく文字が表示されないことに注意してください。
//        参照：NNS_G2dCharCanvasInitForOBJ1D() APIリファレンス
//
//  操作方法
//      A ボタン    カラーモードを変更する。
//      B ボタン    VRAMモードを変更する。
//      十字ボタン  CharCanvasのサイズを変更する。
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include <nnsys/misc.h>
#include "g2d_textdemolib.h"


#define CANVAS_WIDTH        28      // 文字描画域の幅    (キャラクタ単位)
#define CANVAS_HEIGHT       10      // 文字描画域の高さ  (キャラクタ単位)
#define CANVAS_LEFT         13      // 文字描画域の位置X (ピクセル単位)
#define CANVAS_TOP          17      // 文字描画域の位置Y (ピクセル単位)

#define TEXT_HSPACE         1       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE         1       // 文字列描画時の行間   (ピクセル単位)

#define CHARACTER_OFFSET    1       // 使用するキャラクタ列の先頭キャラクタネーム

#define MAX_OBJ_NUM         128
#define REPEAT_THRESHOLD    22


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dFont              gFont;          // フォント
NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas

const char* gErrMsg         = NULL;
int gCanvasWidth            = CANVAS_WIDTH;
int gCanvasHeight           = CANVAS_HEIGHT;
GXOamColorMode gColorMode   = GX_OAM_COLORMODE_16;
NNSG2dOBJVramMode gVramMode = NNS_G2D_OBJVRAMMODE_32K;

//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         IsValidCharCanvasSize

  Description:  CharCanvas のサイズが正常に表示可能なものかどうか判定します。
                ただし、サイズ超過は判定しません。

  Arguments:    areaWidth:  CharCanvas の幅（キャラクタ単位）
                areaHeight: CharCanvas の高さ（キャラクタ単位）
                colorMode:  CharCanvas のカラーモード
                vramMode:   OBJ VRAM 容量

  Returns:      正常に表示可能なら TRUE を返します。
 *---------------------------------------------------------------------------*/
static BOOL IsValidCharCanvasSize(
    int                 areaWidth,
    int                 areaHeight,
    GXOamColorMode      colorMode,
    NNSG2dOBJVramMode   vramMode
)
{
    NNS_G2D_ASSERT( (colorMode == GX_OAM_COLORMODE_16)
                 || (colorMode == GX_OAM_COLORMODE_256) );
    NNS_G2D_ASSERT( (vramMode == NNS_G2D_OBJVRAMMODE_32K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_64K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_128K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_256K) );

    // サイズは正でなければならない
    if( areaWidth <= 0 || areaHeight <= 0 )
    {
        return FALSE;
    }

    if(    (colorMode == GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_128K)
        || (colorMode != GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_256K) )
    {
        // 「奇数と「4で割った余りが3の値」の組み合わせは不可」のケース。
        // これは「どちらかが偶数または共に4で割ったあまりが3でないなら可」と同値なので
        // これで判定する。
        return ((areaWidth  % 2) == 0)
            || ((areaHeight % 2) == 0)
            || ((areaWidth  % 4) != 3 && (areaHeight % 4) != 3);
    }
    else if( colorMode == GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_256K )
    {
        // 共に偶数でなければならない。
        return (areaWidth % 2) == 0 && (areaHeight % 2) == 0;
    }
    else
    {
        return TRUE;
    }
}




/*---------------------------------------------------------------------------*
  Name:         OffsetCharName

  Description:  1D マッピングOBJでキャラクタネームが指し示すアドレスを取得します。

  Arguments:    pCharBase:  OBJのキャラクタベース
                charName:   対象のキャラクタネーム
                vramMode:   VRAM モード

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void* OffsetCharName(
    void*               pCharBase,
    int                 charName,
    NNSG2dOBJVramMode   vramMode )
{
    u32 addr = (u32)pCharBase;
    addr += sizeof(GXCharFmt16) * (charName << vramMode);
    return (void*)addr;
}



/*---------------------------------------------------------------------------*
  Name:         SDKToNNSColorMode

  Description:  NITRO-SDK のカラーモード列挙子から NITRO-System の
                カラーモード列挙子に変換します。

  Arguments:    sdk:    NITRO-SDKのカラーモード列挙子

  Returns:      sdk に対応する NITRO-System のカラーモード列挙子を返します。
 *---------------------------------------------------------------------------*/
static inline NNSG2dCharaColorMode SDKToNNSColorMode(GXOamColorMode sdk)
{
    static const NNSG2dCharaColorMode nns[] =
        { NNS_G2D_CHARA_COLORMODE_16, NNS_G2D_CHARA_COLORMODE_256 };

    NNS_MINMAX_ASSERT(sdk, 0, 1);
    return nns[sdk];
}



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

        if( CMN_IsTrigger(PAD_BUTTON_A) )
        {
            gColorMode = (gColorMode != GX_OAM_COLORMODE_16) ?
                            GX_OAM_COLORMODE_16:
                            GX_OAM_COLORMODE_256;
            bChanged = TRUE;
        }
        if( CMN_IsTrigger(PAD_BUTTON_B) )
        {
            gVramMode++;
            if( gVramMode > NNS_G2D_OBJVRAMMODE_256K )
            {
                gVramMode = NNS_G2D_OBJVRAMMODE_32K;
            }
            bChanged = TRUE;
        }
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
        if( MAX_OBJ_NUM < NNS_G2dCalcRequiredOBJ1D(gCanvasWidth, gCanvasHeight) )
        {
            // 使える OBJ の最大数を超えた
            gCanvasWidth = old_cw;
            gCanvasHeight = old_ch;
            gErrMsg = "OBJ OVERFLOW";
            bChanged = TRUE;
        }
        {
            const int maxChr = ((1024 << gVramMode) >> gColorMode) - CHARACTER_OFFSET;

            // 
            if( maxChr < (gCanvasWidth + 1) * (gCanvasHeight + 1) )
            {
                // 使えるキャラクタの最大数を超えた
                gCanvasWidth = old_cw;
                gCanvasHeight = old_ch;
                gErrMsg = "Character OVERFLOW";
                bChanged = TRUE;
            }
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
    void*      const pCharBase = G2_GetOBJCharPtr();
    GXOamAttr* const pObjBase = (GXOamAttr*)HW_OAM;
    const int cOffset = CHARACTER_OFFSET;
    int nObjs;


    // マッピングモード設定
    switch( gVramMode )
    {
    case NNS_G2D_OBJVRAMMODE_32K:  GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);   break;
    case NNS_G2D_OBJVRAMMODE_64K:  GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_64K);   break;
    case NNS_G2D_OBJVRAMMODE_128K: GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_128K);  break;
    case NNS_G2D_OBJVRAMMODE_256K: GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_256K);  break;
    default:                       GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);   break;
    }


    // CharCanvas の初期化
    NNS_G2dCharCanvasInitForOBJ1D(
        &gCanvas,
        OffsetCharName(pCharBase, cOffset, gVramMode),
            // VRAM参照可能サイズによってキャラクタネーム境界が異なるため補正する

        gCanvasWidth,
        gCanvasHeight,
        SDKToNNSColorMode(gColorMode)
    );

    // Canvasクリア
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // OBJを配置
    nObjs = NNS_G2dArrangeOBJ1D(
        pObjBase,
        gCanvasWidth,
        gCanvasHeight,
        CANVAS_LEFT,
        CANVAS_WIDTH,
        gColorMode,
        cOffset,
        gVramMode
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

    if( ! IsValidCharCanvasSize(gCanvasWidth, gCanvasHeight, gColorMode, gVramMode) )
    {
        gErrMsg = "INVALID CharCanvas size";
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
    static const char* colorModeString[] =
    {
        "GX_OAM_COLORMODE_16",
        "GX_OAM_COLORMODE_256"
    };
    static const char* vramModeString[] =
    {
        "NNS_G2D_OBJVRAMMODE_32K",
        "NNS_G2D_OBJVRAMMODE_64K",
        "NNS_G2D_OBJVRAMMODE_128K",
        "NNS_G2D_OBJVRAMMODE_256K"
    };

    DTX_PrintLine(
        "1D mapping OBJ CharCanvas Sample\n"
        "operation\n"
        "  A  change color mode\n"
        "  B  change vram mode\n"
        "  +  change canvas size\n"
    );
    DTX_PrintLine("color mode:  %s", colorModeString[gColorMode]);
    DTX_PrintLine("vram mode:   %s", vramModeString[gVramMode]);
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
    gErrMsg = NULL;

    if( HandleInput() )
    {
        PrintSampleInfo();

        ResetOAM();
        InitCanvas();

        SampleDraw();
        DC_StoreAll();

        if( gErrMsg != NULL )
        {
            DTX_PrintLine("\n\n\n\n     !!! %s !!!", gErrMsg);
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


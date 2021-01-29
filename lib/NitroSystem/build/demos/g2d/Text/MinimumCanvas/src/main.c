/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - MinimumCanvas
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      表示する文字列の大きさに応じて、その文字列を表示する事が出来る
//      最小の CharCanvas を構築し、描画を行います。
//      NNS_G2dFontGetTextRect によって得られた文字列のサイズが
//      CharCanvas のサイズとして無効である場合は、それ以上かつ最小の
//      有効サイズを計算して CharCanvas を構築します。
//
//  操作方法
//      A ボタン    カラーモードを変更する。
//      B ボタン    VRAMモードを変更する。
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include <nnsys/misc.h>
#include "g2d_textdemolib.h"


#define ROUNDUP(x, div) (((x) + (div) - 1) & ~((div) - 1))

#define CHARACTER_WIDTH     8       // キャラクタのピクセル単位での幅
#define CHARACTER_HEIGHT    8       // キャラクタのピクセル単位での高さ
#define SCREEN_WIDTH        256     // スクリーン幅
#define SCREEN_HEIGHT       192     // スクリーン高さ

#define TEXT_HSPACE         1       // 文字列描画時の文字間 (ピクセル単位)
#define TEXT_VSPACE         1       // 文字列描画時の行間   (ピクセル単位)

#define CHARACTER_OFFSET    1       // 使用するキャラクタ列の開始番号

#define MAX_OBJ_NUM         128     // OBJ の最大数
#define MAX_CHARA_NAME      1024    // キャラクタネームの最大値


//------------------------------------------------------------------------------
// グローバル変数

NNSG2dFont              gFont;          // フォント

GXOamColorMode gColorMode       = GX_OAM_COLORMODE_16;
NNSG2dOBJVramMode gVramMode     = NNS_G2D_OBJVRAMMODE_32K;

int gUsingCharas = 0;
int gUsingOam    = 0;

const static NNSG2dChar* sampleText[] =
{
    NNS_G2D_TRANSCODE("This sample program make temporary minimum\n"
                      "CharCanvas to draw a text and draw the\n"
                      "text to it."),
    NNS_G2D_TRANSCODE("sample text"),
    NNS_G2D_TRANSCODE("this text require\n11x3 size."),
    NNS_G2D_TRANSCODE("line 1\n" "line 2\n" "line 3\n" "line 4\n"),
    NNS_G2D_TRANSCODE("A"),
};

//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         GetMinValidCanvasSize

  Description:  与えられたサイズ以上で CharCanvas のサイズとして有効な
                最小のサイズを取得します。

  Arguments:    areaWidth:  CharCanvas の幅（キャラクタ単位）
                areaHeight: CharCanvas の高さ（キャラクタ単位）
                colorMode:  CharCanvas のカラーモード
                vramMode:   OBJ VRAM 容量

  Returns:      与えられたサイズ以上で CharCanvas のサイズとして有効な
                最小のサイズを返します。
 *---------------------------------------------------------------------------*/
static NNSG2dTextRect GetMinValidCanvasSize(
    int                 areaWidth,
    int                 areaHeight,
    GXOamColorMode      colorMode,
    NNSG2dOBJVramMode   vramMode
)
{
    NNSG2dTextRect rect;
    NNS_G2D_MIN_ASSERT( areaWidth,  1 );
    NNS_G2D_MIN_ASSERT( areaHeight, 1 );
    NNS_G2D_ASSERT( (colorMode == GX_OAM_COLORMODE_16)
                 || (colorMode == GX_OAM_COLORMODE_256) );
    NNS_G2D_ASSERT( (vramMode == NNS_G2D_OBJVRAMMODE_32K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_64K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_128K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_256K) );

    rect.width  = areaWidth;
    rect.height = areaHeight;

    if(    (colorMode == GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_128K)
        || (colorMode != GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_256K) )
    {
        // 「奇数と「4で割った余りが3の値」の組み合わせは不可」のケース。
        // 条件に当てはまる場合は適切などちらか片方を +1 すればよい
        if( (rect.width % 2) != 0 && (rect.height % 2) != 0 )
        {
            if( (rect.width % 4) != 1 )
            {
                // width % 4 == 3 && height % 4 == 1,3
                rect.width++;
            }
            else if( (rect.height % 4) != 1 )
            {
                // width % 4 == 1 && height % 4 == 3
                rect.height++;
            }
        }
    }
    else if( colorMode == GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_256K )
    {
        // 共に偶数でなければならない。
        rect.width  = ROUNDUP(rect.width,  2);
        rect.height = ROUNDUP(rect.height, 2);
    }

    return rect;
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

  Description:  OAMを全て解放しリセットします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ResetOAM(void)
{
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    gUsingOam = 0;
}



/*---------------------------------------------------------------------------*
  Name:         ResetOBJChr

  Description:  OBJキャラクタを全て解放します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ResetOBJChr(void)
{
    gUsingCharas = 0;
}



/*---------------------------------------------------------------------------*
  Name:         AllocCharacters

  Description:  簡易キャラクタマネージャ。
                指定した数のキャラクタを確保し、確保したキャラクタ列の
                先頭キャラクタネームを返します。

  Arguments:    num:    確保するキャラクタの数。

  Returns:      確保に成功した場合は確保したキャラクタ列の
                先頭キャラクタネームを返します。
                失敗した場合は -1 を返します。
 *---------------------------------------------------------------------------*/
static int AllocCharacters(
    int num,
    GXOamColorMode colorMode,
    NNSG2dOBJVramMode vramMode )
{
    int ret;
    int realNum = ROUNDUP_DIV(num, (1 << vramMode));

    if( colorMode == GX_OAM_COLORMODE_256 )
    {
        realNum *= 2;
    }

    if( gUsingCharas + realNum > MAX_CHARA_NAME )
    {
        return -1;
    }

    ret = gUsingCharas;
    gUsingCharas += realNum;

    return ret;
}



/*---------------------------------------------------------------------------*
  Name:         AllocOAM

  Description:  簡易 OAM マネージャ。
                指定した数の OAM を確保し、確保した OAM 列の
                先頭へのポインタを返します

  Arguments:    num:    確保する OAM の数。

  Returns:      確保に成功した場合は確保したキャラクタ列の
                先頭へのポインタを返します。
                失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
static GXOamAttr* AllocOAM(int num)
{
    GXOamAttr* const pObjBase = (GXOamAttr*)HW_OAM;
    GXOamAttr* ret;

    if( gUsingOam + num > MAX_OBJ_NUM )
    {
        return NULL;
    }

    ret = pObjBase + gUsingOam;
    gUsingOam += num;

    return ret;
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
  Name:         PutText

  Description:  指定された場所に指定されたテキストを表示します。

  Arguments:    x:          テキストを表示する位置
                y:          テキストを表示する位置
                pFont:      テキストを描画するためのフォント
                colorMode:  メインOBJのカラーモードを指定します。
                vramMode:   メインOBJのVRAMモードを指定します。
                color:      テキストの色番号を指定します。
                txt:        表示するテキストを指定します。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void PutText(
    int                 x,
    int                 y,
    const NNSG2dFont*   pFont,
    GXOamColorMode      colorMode,
    NNSG2dOBJVramMode   vramMode,
    u8                  color,
    const NNSG2dChar*   txt
)
{
    void* const pCharBase = G2_GetOBJCharPtr();
    NNSG2dCharCanvas cc;
    NNSG2dTextCanvas tc;
    NNSG2dTextRect textRect;
    NNSG2dTextRect canvasSize;
    int areaWidth, areaHeight;
    GXOamAttr* pOam;
    int numOBJ;
    int charNo;

    // テキスト領域サイズを求めます
    textRect   = NNS_G2dFontGetTextRect(pFont, TEXT_HSPACE, TEXT_VSPACE, txt);
    areaWidth  = ROUNDUP_DIV(textRect.width,  CHARACTER_WIDTH);
    areaHeight = ROUNDUP_DIV(textRect.height, CHARACTER_HEIGHT);
    canvasSize = GetMinValidCanvasSize(areaWidth, areaHeight, colorMode, vramMode);

    // OAM と キャラクタを確保します
    numOBJ = NNS_G2dCalcRequiredOBJ1D( canvasSize.width, canvasSize.height );
    charNo = AllocCharacters( canvasSize.width * canvasSize.height, colorMode, vramMode );
    pOam   = AllocOAM( numOBJ );
    NNS_G2D_ASSERT( charNo >= 0 );
    NNS_G2D_POINTER_ASSERT( pOam );

    // CharCanvas を 1D OBJ 用に テキスト領域サイズで初期化
    NNS_G2dCharCanvasInitForOBJ1D(
        &cc,
        OffsetCharName(pCharBase, charNo, vramMode),
        canvasSize.width,
        canvasSize.height,
        SDKToNNSColorMode(colorMode)
    );

    // CharCanvas 用に OBJ を配置
    (void)NNS_G2dArrangeOBJ1D(
        pOam,
        canvasSize.width,
        canvasSize.height,
        x, y,
        colorMode,
        charNo,
        vramMode
    );

    // OBJを設定
    TXT_SetCharCanvasOBJAttrs(
        pOam,                  // OAM配列へのポインタ
        numOBJ,                 // 対象とするOAMの数
        0,                      // priority
        GX_OAM_MODE_NORMAL,     // OBJモード
        FALSE,                  // モザイク
        GX_OAM_EFFECT_NONE,     // OBJ効果
        TXT_CPALETTE_MAIN,      // パレット番号
        0                       // アフィンパラメータインデックス
    );

    // TextCanvas を構築
    NNS_G2dTextCanvasInit(
        &tc,
        &cc,
        pFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );

    // Canvas をクリアして文字列描画
    NNS_G2dCharCanvasClear(
        &cc,
        TXT_COLOR_WHITE
    );
    NNS_G2dTextCanvasDrawText(
        &tc,
        0, 0,
        color,
        TXT_DRAWTEXT_FLAG_DEFAULT,
        txt
    );


    DTX_PrintLine("%3dx%3d  %2dx%2d  %2dx%2d  %4d  %3d",
        textRect.width, textRect.height,
        areaWidth, areaHeight,
        canvasSize.width, canvasSize.height,
        canvasSize.width * canvasSize.height, numOBJ
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
        "Text size dependents Canvas Sample\n"
        "operation\n"
        "  A  change color mode\n"
        "  B  change vram mode\n"
    );
    DTX_PrintLine("color mode:  %s", colorModeString[gColorMode]);
    DTX_PrintLine("vram mode:   %s", vramModeString[gVramMode]);
    DTX_PrintLine(
        "\n"
        "Size                    Using\n"
        "text     canvas adjust  char  OBJ"
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleDraw

  Description:  サンプルの描画処理です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SampleDraw(void)
{
    int i;

    PrintSampleInfo();

    switch( gVramMode )
    {
    case NNS_G2D_OBJVRAMMODE_32K:  GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);   break;
    case NNS_G2D_OBJVRAMMODE_64K:  GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_64K);   break;
    case NNS_G2D_OBJVRAMMODE_128K: GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_128K);  break;
    case NNS_G2D_OBJVRAMMODE_256K: GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_256K);  break;
    default:                       GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);   break;
    }

    ResetOAM();
    ResetOBJChr();

    for( i = 0; i < ARY_SIZEOF(sampleText); ++i )
    {
        int x = (int)(SCREEN_WIDTH / ARY_SIZEOF(sampleText)) * i;
        int y = (int)(SCREEN_HEIGHT / ARY_SIZEOF(sampleText)) * i;
        u8 color = (u8)(TXT_COLOR_RED + i);

        PutText(x, y, &gFont, gColorMode, gVramMode, color, sampleText[i]);
    }
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

    InitScreen();
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
        SampleDraw();
        DC_StoreAll();
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


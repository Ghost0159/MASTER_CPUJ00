/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib - include - g2d_textdemolib
  File:     txt.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/
#ifndef TEXT_H_
#define TEXT_H_

#include <nnsys/g2d/g2d_Font.h>

#ifdef __cplusplus
extern "C" {
#endif

// DrawText での左上寄せ
#define TXT_DRAWTEXT_FLAG_DEFAULT   (NNS_G2D_VERTICALORIGIN_TOP | NNS_G2D_HORIZONTALORIGIN_LEFT | NNS_G2D_HORIZONTALALIGN_LEFT)

// デモ用フォントリソース名
#define TXT_FONTRESOURCE_NAME               "/data/fonts.NFTR"
#define TXT_SJIS_FONTRESOURCE_NAME          "/data/fonts.NFTR"
#define TXT_UTF8_FONTRESOURCE_NAME          "/data/fontu8.NFTR"
#define TXT_UTF16_FONTRESOURCE_NAME         "/data/fontu16.NFTR"
#define TXT_CP1252_FONTRESOURCE_NAME        "/data/font1252.NFTR"



// TXTColorPalette の色名 16色パレットへのロードを想定
enum
{
    // パレット0 TXT_CPALETTE_MAIN
    TXT_COLOR_NULL=0,
    TXT_COLOR_WHITE,
    TXT_COLOR_BLACK,
    TXT_COLOR_RED,
    TXT_COLOR_GREEN,
    TXT_COLOR_BLUE,
    TXT_COLOR_CYAN,
    TXT_COLOR_MAGENTA,
    TXT_COLOR_YELLOW,

    // パレット1 TXT_CPALETTE_USERCOLOR
    TXT_UCOLOR_NULL=0,
    TXT_UCOLOR_GRAY,
    TXT_UCOLOR_BROWN,
    TXT_UCOLOR_RED,
    TXT_UCOLOR_PINK,
    TXT_UCOLOR_ORANGE,
    TXT_UCOLOR_YELLOW,
    TXT_UCOLOR_LIMEGREEN,
    TXT_UCOLOR_DARKGREEN,
    TXT_UCOLOR_SEAGREEN,
    TXT_UCOLOR_TURQUOISE,
    TXT_UCOLOR_BLUE,
    TXT_UCOLOR_DARKBLUE,
    TXT_UCOLOR_PURPLE,
    TXT_UCOLOR_VIOLET,
    TXT_UCOLOR_MAGENTA,

    // パレット TXT_CPALETTE_4BPP
    TXT_COLOR_4BPP_NULL=0,
    TXT_COLOR_4BPP_BG=1,
    TXT_COLOR_4BPP_TEXT=1
};

// TXTColorPalette のパレット名 16色パレットへのロードを想定
enum
{
    TXT_CPALETTE_MAIN,
    TXT_CPALETTE_USERCOLOR,
    TXT_CPALETTE_4BPP,
    TXT_NUM_CPALEETE
};

// 共通カラーパレットデータ
extern GXRgb TXTColorPalette[TXT_NUM_CPALEETE * 16];



//****************************************************************************
//
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         TXT_Init

  Description:  サンプル共通の初期化。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void TXT_Init(void);



/*---------------------------------------------------------------------------*
  Name:         TXT_SetupBackground

  Description:  メイン画面BG0に背景画像をロード＆表示します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void TXT_SetupBackground( void );



/*---------------------------------------------------------------------------*
  Name:         TXT_Alloc

  Description:  メモリを確保します。

  Arguments:    size:   確保するメモリのサイズ

  Returns:      確保したメモリ領域へのポインタ。
 *---------------------------------------------------------------------------*/
void* TXT_Alloc(u32 size);



/*---------------------------------------------------------------------------*
  Name:         TXT_Free

  Description:  TXT_Alloc() で確保したメモリを解放します。

  Arguments:    ptr:    開放するメモリ領域へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void TXT_Free(void* ptr);



/*---------------------------------------------------------------------------*
  Name:         TXT_SetCharCanvasOBJAttrs

  Description:  連続するOAMのNNS_G2dArrangeOBJ* で設定されないパラメータを
                まとめて設定します。

  Arguments:    oam:        対象の OAM 列の先頭
                num:        対象の OAM の個数
                priority:   表示優先順位(0～3)
                mode:       OBJモード
                mosaic:     モザイクの有無
                effect:     エフェクトの種類
                cParam:     カラーパラメータ
                rsParam:    アフィン変換パラメータインデックス

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void TXT_SetCharCanvasOBJAttrs(
    GXOamAttr * oam,
    int         num,
    int         priority,
    GXOamMode   mode,
    BOOL        mosaic,
    GXOamEffect effect,
    int         cParam,
    int         rsParam
);



/*---------------------------------------------------------------------------*
  Name:         TXT_LoadFont

  Description:  フォントをファイルからメモリにロードします。

  Arguments:    pFname: フォントリソースのパス。

  Returns:      ロードしたフォントへのポインタ。
 *---------------------------------------------------------------------------*/
void TXT_LoadFont( NNSG2dFont* pFont, const char* pFname );



/*---------------------------------------------------------------------------*
  Name:         TXT_LoadFile

  Description:  ファイルをメモリにロードします。

  Arguments:    ppFile: ファイルをロードしたメモリアドレスを受け取る
                        バッファへのポインタ。
                fpath:  ロードするファイルのパス

  Returns:      ロードしたファイルのファイルサイズを返します。
                0 の場合はファイルロードに失敗した事を表します。
                この場合 *ppFile の値は無効です。
 *---------------------------------------------------------------------------*/
u32 TXT_LoadFile(void** ppFile, const char* fpath);





#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // TEXT_H_


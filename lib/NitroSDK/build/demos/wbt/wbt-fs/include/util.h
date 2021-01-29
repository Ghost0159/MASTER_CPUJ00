/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs
  File:     util.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: util.h,v $
  Revision 1.4  2006/01/18 02:12:38  kitase_hirotake
  do-indent

  Revision 1.3  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.2  2004/10/25 10:18:42  yosizaki
  add color definitions.

  Revision 1.1  2004/09/03 01:11:01  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#if	!defined(NITRO_DEMOS_WBT_WBT_FS_UTIL_H_)
#define	NITRO_DEMOS_WBT_WBT_FS_UTIL_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include	<nitro/types.h>

/*
 * サンプルのフレームワーク全般を処理するルーチンです.
 * サンプルが提示するデモンストレーションとは直接関係ありません.
 */


/*---------------------------------------------------------------------------*
    構造体定義
 *---------------------------------------------------------------------------*/

/* ASCII フォントデータ */
extern const u32 d_CharData[8 * 256];
extern const u32 d_PaletteData[8 * 16];

/* カラーパレット */
#define	COLOR_BLACK                0x00
#define	COLOR_RED                  0x01
#define	COLOR_GREEN                0x02
#define	COLOR_BLUE                 0x03
#define	COLOR_YELLOW               0x04
#define	COLOR_PURPLE               0x05
#define	COLOR_LIGHT_BLUE           0x06
#define	COLOR_DARK_RED             0x07
#define	COLOR_DARK_GREEN           0x08
#define	COLOR_DARK_BLUE            0x09
#define	COLOR_DARK_YELLOW          0x0A
#define	COLOR_DARK_PURPLE          0x0B
#define	COLOR_DARK_LIGHT_BLUE      0x0C
#define	COLOR_GRAY                 0x0D
#define	COLOR_DARK_GRAY            0x0E
#define	COLOR_WHITE                0x0F


/*---------------------------------------------------------------------------*
    構造体定義
 *---------------------------------------------------------------------------*/

/* キー入力情報 */
typedef struct KeyInfo
{
    u16     cnt;                       /* 未加工入力値 */
    u16     trg;                       /* 押しトリガ入力 */
    u16     up;                        /* 離しトリガ入力 */
    u16     rep;                       /* 押し維持リピート入力 */

}
KeyInfo;


/*---------------------------------------------------------------------------*
    関数宣言
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         UTIL_Init

  Description:  サンプルのフレームワーク初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    UTIL_Init(void);


/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  キーデータの取得.

  Arguments:    pKey : キー情報を格納するアドレス

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    KeyRead(KeyInfo * pKey);


/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  表示用仮想スクリーンの初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ClearString(void);


/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  仮想スクリーンに32文字以内の文字列を表示.

  Arguments:    x       - 文字列の先頭を配置する x 座標 / 8 dot
                y       - 文字列の先頭を配置する y 座標 / 8 dot
                palette - 文字色を指定するパレット番号
                text    - 表示文字列
                ...     - 後続の可変引数

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PrintString(int x, int y, int palette, const char *text, ...);


/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  仮想スクリーンに表示されている文字列の色を変更.

  Arguments:    x       - 色変更を開始する x 座標 / 8 dot
                y       - 色変更を開始する y 座標 / 8 dot
                length  - 色変更する文字数
                palette - 文字色を指定するパレット番号

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ColorString(int x, int y, int length, int palette);


/*---------------------------------------------------------------------------*
  Name:         ClearLine

  Description:  文字列描画を初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ClearLine(void);


/*---------------------------------------------------------------------------*
  Name:         PrintLine

  Description:  指定ラインに 1 フレーム以上残る不揮発な文字列を描画.

  Arguments:    x          表示位置 x グリッド. (8 pixel 単位)
                y          表示位置 y グリッド. (8 pixel 単位)
                text       後続の可変引数を受ける書式文字列.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PrintLine(int x, int y, const char *text, ...);


/*---------------------------------------------------------------------------*
  Name:         FlushLine

  Description:  通常の文字列描画に PrintLine() の不揮発文字列を上書き.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FlushLine(void);


/*===========================================================================*/

#ifdef	__cplusplus
}          /* extern "C" */
#endif

#endif /* NITRO_DEMOS_WBT_WBT_FS_UTIL_H_ */


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

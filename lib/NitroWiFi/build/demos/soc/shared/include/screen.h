/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - shared
  File:     screen.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: screen.h,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/06 00:23:35  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if !defined(NITROWIFI_SOC_SHARED_SCREEN_H_)
#define NITROWIFI_SOC_SHARED_SCREEN_H_

#include <nitro.h>

#ifdef __cplusplus

extern "C"
{
#endif

/*****************************************************************************/

/* constant */
extern const u32    d_CharData[8 * 256];
extern const u32    d_PaletteData[8 * 16];

#define COLOR_WHITE             0
#define COLOR_RED               1
#define COLOR_GREEN             2
#define COLOR_BLUE              3
#define COLOR_YELLOW            4
#define COLOR_PURPLE            5
#define COLOR_LIGHT_BLUE        6
#define COLOR_DARK_RED          7
#define COLOR_DARK_GREEN        8
#define COLOR_DARK_BLUE         9
#define COLOR_DARK_YELLOW       10
#define COLOR_DARK_PURPLE       11
#define COLOR_DARK_LIGHT_BLUE   12
#define COLOR_GRAY              13
#define COLOR_DARK_GRAY         14
#define COLOR_DARK_BLACK        15

/*****************************************************************************/

/* function */

/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  表示処理の初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    InitScreen(void);

/*---------------------------------------------------------------------------*
  Name:         ClearScreen

  Description:  テキストスクリーンの消去.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ClearScreen(void);

/*---------------------------------------------------------------------------*
  Name:         PadkeyClearScreen

  Description:  Padkeyが使用する部分のテキストスクリーンの消去.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PadkeyClearScreen(void);

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  テキストスクリーンに文字列を表示.

  Arguments:    x             グリッド単位の X 座標
                y             グリッド単位の Y 座標
                palette       パレットインデックス
                format        書式文字列

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PrintString(int x, int y, int palette, const char* format, ...);

/*---------------------------------------------------------------------------*
  Name:         PrintDebugString

  Description:  サブ画面テキストスクリーンにデバッグ文字列を表示.

  Arguments:    s             出力文字列.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PrintDebugString(const char* s, ...);

/*---------------------------------------------------------------------------*
  Name:         FlushScreen

  Description:  テキストスクリーンをこの場でVRAMへ反映.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FlushScreen(void);

/*---------------------------------------------------------------------------*
  Name:         GetFlickerColor

  Description:  点滅用の文字列色を取得する。

  Arguments:    None.

  Returns:      int     -   パレット番号
 *---------------------------------------------------------------------------*/
int     GetFlickerColor(void);

/*---------------------------------------------------------------------------*
  Name:         ShowNavigater

  Description:  テキストスクリーンの最下行に、オペレータを誘導するメッセージ
                を表示する。毎フレーム呼び出すことにより、点滅表示される。

  Arguments:    text    -   表示するメッセージテキスト。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ShowNavigater(const char* text);

/*---------------------------------------------------------------------------*
  Name:         PrintStep

  Description:  テキストスクリーンの指定位置に、手順文字列を表示。
                文字数と行幅に応じて適宜改行する。

  Arguments:    x       -   表示原点 x 座標。
                y       -   表示原点 y 座標。
                palette -   表示パレット色。 (COLOR_*)
                step    -   現在のステップ番号。
                comment -   テスト手順を示すコメント文字列。

  Returns:      表示に要した行数.
 *---------------------------------------------------------------------------*/
int     PrintStep(int x, int y, int palette, int step, const char* comment);

#ifdef __cplusplus

} /* extern "C" */
#endif

#endif /* NITROWIFI_SOC_SHARED_SCREEN_H_ */

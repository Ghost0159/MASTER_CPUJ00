/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - shared
  File:     padkeyboard.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: padkeyboard.h,v $
  Revision 1.3  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2006/01/10 04:21:17  kitase_hirotake
  PadKeySetString, PadKeyGetInputString の追加

  Revision 1.1  2006/01/06 00:23:35  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if !defined(NITROWIFI_SOC_SHARED_PAD_KEYBOARD_H_)
#define NITROWIFI_SOC_SHARED_PAD_KEYBOARD_H_

#include <nitro.h>

#include <stdlib.h>

#include "output.h"
#include "screen.h"

#ifdef __cplusplus

extern "C"
{
#endif

/*****************************************************************************/

/* constant */
#define INPUT_MAX   256

/* キーボードで使用している定数 */
#define KEY_UP              (-1)
#define KEY_DOWN            (-2)
#define KEY_LEFT            (-3)
#define KEY_RIGHT           (-4)
#define KEY_BACKSPACE       (-5)
#define KEY_SPACE           (-6)
#define KEY_RETURN          (-7)
#define KEY_DONT_GET        (-128)

#define DIRECT_MODE         1
#define INDIRECT_MODE       2

#define ARABIC_NUMERALS     1
#define CAPITAL_LETTER      2
#define SMALL_LETTER        3
#define CHARACTOR_SYMBOL    4
#define MAX_CHARACTOR_MODE  4

/* PadKeyGet* で得られたデータが Indirect Mode であるかのチェック用マクロ */
#define IndirectCheck(charactor)    (charactor > KEY_UP ? 1 : 0)

/*---------------------------------------------------------------------------*
  Name:         KeyboardValid, KeyboardInvalid

  Description:  キーボード入力の有効、無効を切り替える.

  Arguments:    None.

  Returns:      なし.
 *---------------------------------------------------------------------------*/
void    KeyboardValid();
void    KeyboardInvalid();

/*---------------------------------------------------------------------------*
  Name:         PadKeySetString

  Description:  指定文字列を現在の入力にセットする.

  Arguments:    string - セットする文字列.

  Returns:      なし.
 *---------------------------------------------------------------------------*/
void    PadKeySetString(char string[]);

/*---------------------------------------------------------------------------*
  Name:         PadKeyGetInputString

  Description:  文字列を入力させて、それを受け取る.

  Arguments:    string - セットする文字列.

  Returns:      なし.
 *---------------------------------------------------------------------------*/
void    PadKeyGetInputString(char string[], int size);

/*---------------------------------------------------------------------------*
  Name:         PadKeyGetcharAsync

  Description:  パッド入力データを一文字得る(非同期).

  Arguments:    None.

  Returns:      文字データ.
 *---------------------------------------------------------------------------*/
char    PadKeyGetcharAsync(void);

/*---------------------------------------------------------------------------*
  Name:         PadKeyGetchar

  Description:  パッド入力データを一文字得る(同期).

  Arguments:    None.

  Returns:      文字データ.
 *---------------------------------------------------------------------------*/
char    PadKeyGetchar(void);

/*---------------------------------------------------------------------------*
  Name:         WakeupKeyboardThread

  Description:  キーボードスレッドを起こす.

  Arguments:    None.

  Returns:      なし.
 *---------------------------------------------------------------------------*/
void    WakeupKeyboardThread(void);

/*---------------------------------------------------------------------------*
  Name:         InitKeyboardThread

  Description:  キーボードスレッドを作成.

  Arguments:    None.

  Returns:      なし.
 *---------------------------------------------------------------------------*/
void    InitKeyboardThread(void);

#ifdef __cplusplus

} /* extern "C" */
#endif

#endif /* NITROWIFI_SOC_SHARED_PAD_KEYBOARD_H_ */

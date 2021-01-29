/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - demolib
  File:     print.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

#include "g3d_demolib/print.h"
#include "g3d_demolib/font.h"

#define SCREEN_HEIGHT       192     // 画面高さ
#define SCREEN_WIDTH        256     // 画面幅

#define SCREEN_CHARA_SIZE   8       // BGキャラクタサイズ

#define SCR_PLTT_SHIFT      12      // スクリーンデータ内のパレットNo用シフト幅
#define FONT_COLOR          4       // 文字色=青

// キャラクタ単位の画面サイズ
#define SCREEN_HEIGHT_CHARA (SCREEN_HEIGHT / SCREEN_CHARA_SIZE)
#define SCREEN_WIDTH_CHARA  (SCREEN_WIDTH / SCREEN_CHARA_SIZE)

// スクリーンバッファ
static u16 sScreenBuf[SCREEN_HEIGHT_CHARA][SCREEN_WIDTH_CHARA];


/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitConsole

  Description:  メイン画面のBG1を文字表示用にセットアップします。
                system.c の Init3DStuff_ の処理に依存しています。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G3DDemo_InitConsole(void)
{
    // リソースのロード
    GX_LoadBG1Char(fontCharData, 0, sizeof(fontCharData));
	GX_LoadBGPltt (fontPlttData, 0, 0x00200);

    // 内部バッファのクリア
    G3DDemo_ClearConsole();
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_ClearConsole

  Description:  内部スクリーンバッファをクリアします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G3DDemo_ClearConsole(void)
{
    MI_CpuClearFast( sScreenBuf, sizeof(sScreenBuf) );
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_Print

  Description:  メイン画面のBG1を利用して文字列を表示します。
                1行32文字 24行の表示ができます。

  Arguments:    x:      1文字目を表示する列を指定します。0から始まります。
                y:      表示する行を指定します。0から始まります。
                string: 出力する文字列です。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G3DDemo_Print(int x, int y, int color, const char* string)
{
    const char* pos;
    SDK_NULL_ASSERT( string );
    SDK_ASSERT( 0 <= x && x < SCREEN_WIDTH_CHARA );
    SDK_ASSERT( 0 <= y && y < SCREEN_HEIGHT_CHARA );

    for( pos = string; *pos != '\0'; ++pos )
    {
        sScreenBuf[y][x] = (u16)((u16)(color << SCR_PLTT_SHIFT) | (u16)(*pos));
        x++;

        if( x >= SCREEN_WIDTH )
        {
            break;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_Printf

  Description:  メイン画面のBG1を利用して文字列を表示します。
                1行32文字 24行の表示ができます。

  Arguments:    x:      1文字目を表示する列を指定します。0から始まります。
                y:      表示する行を指定します。0から始まります。
                format: printf形式のフォーマット文字列です。
                ...:    フォーマットに対応するパラメータです。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G3DDemo_Printf(int x, int y, int color, const char* format, ...)
{
    va_list vlist;
    char buf[SCREEN_WIDTH_CHARA + 1];
	SDK_NULL_ASSERT( format );
    
    va_start( vlist, format );
    (void)vsnprintf( buf, sizeof(buf), format, vlist );
    va_end( vlist );
    
    G3DDemo_Print(x, y, color, buf);
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_PrintApplyToHW

  Description:  内部スクリーンバッファをハードウェアに転送します。
                通常は V ブランクごとに呼び出します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G3DDemo_PrintApplyToHW(void)
{
    DC_FlushRange( sScreenBuf, sizeof(sScreenBuf) );
    GX_LoadBG1Scr( sScreenBuf, 0, sizeof(sScreenBuf) );
}


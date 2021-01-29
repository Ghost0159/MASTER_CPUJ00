/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - demolib
  File:     print.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

#include "g2d_demolib.h"
#include "g2d_demolib/print.h"
#include "g2d_demolib/fontData.h"

#define SCREEN_HEIGHT       192     // 画面高さ
#define SCREEN_WIDTH        256     // 画面幅

#define SCREEN_CHARA_SIZE   8       // BGキャラクタサイズ

#define SCR_PLTT_SHIFT      12      // スクリーンデータ内のパレットNo用シフト幅
#define FONT_COLOR          3       // 文字色=青

// キャラクタ単位の画面サイズ
#define SCREEN_HEIGHT_CHARA (SCREEN_HEIGHT / SCREEN_CHARA_SIZE)
#define SCREEN_WIDTH_CHARA  (SCREEN_WIDTH / SCREEN_CHARA_SIZE)

// スクリーンバッファ
static u16 sScreenBuf[SCREEN_HEIGHT_CHARA][SCREEN_WIDTH_CHARA];


/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintInit

  Description:  メイン画面のBG1を文字表示用にセットアップします。
                system.c の Init3DStuff_ の処理に依存しています。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintInit(void)
{
    // BG1 のセットアップ
    GX_SetBankForBGExtPltt(GX_VRAM_BGEXTPLTT_01_F);
    GX_SetVisiblePlane( GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_OBJ );
    G2_SetBG1Control(
        GX_BG_SCRSIZE_TEXT_256x256,
        GX_BG_COLORMODE_256,
        GX_BG_SCRBASE_0x0000,
        GX_BG_CHARBASE_0x04000,
        GX_BG_EXTPLTT_01);
    G2_SetBG1Priority(3);

    // リソースのロード
    GX_LoadBG1Char(fontCharData, 0, sizeof(fontCharData));
    
    GX_BeginLoadBGExtPltt();
    GX_LoadBGExtPltt(fontPlttData, 0x2000, sizeof(fontPlttData));
    GX_EndLoadBGExtPltt();

    // 内部バッファのクリア
    G2DDemo_PrintClear();
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintClear

  Description:  内部スクリーンバッファをクリアします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintClear(void)
{
    MI_CpuClearFast( sScreenBuf, sizeof(sScreenBuf) );
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintOut

  Description:  メイン画面のBG1を利用して文字列を表示します。
                1行32文字 24行の表示ができます。

  Arguments:    x:      1文字目を表示する列を指定します。0から始まります。
                y:      表示する行を指定します。0から始まります。
                string: 出力する文字列です。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintOut(int x, int y, const char* string)
{
    const char* pos;
    SDK_NULL_ASSERT( string );
    SDK_ASSERT( 0 <= x && x < SCREEN_WIDTH_CHARA );
    SDK_ASSERT( 0 <= y && y < SCREEN_HEIGHT_CHARA );

    for( pos = string; *pos != '\0'; ++pos )
    {
        sScreenBuf[y][x] = (u16)((u16)(FONT_COLOR << SCR_PLTT_SHIFT) | (u16)(*pos));
        x++;

        if( x >= SCREEN_WIDTH )
        {
            break;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintOutf

  Description:  メイン画面のBG1を利用して文字列を表示します。
                1行32文字 24行の表示ができます。

  Arguments:    x:      1文字目を表示する列を指定します。0から始まります。
                y:      表示する行を指定します。0から始まります。
                format: printf形式のフォーマット文字列です。
                ...:    フォーマットに対応するパラメータです。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintOutf(int x, int y, const char* format, ...)
{
    va_list vlist;
    char buf[SCREEN_WIDTH_CHARA + 1];
	SDK_NULL_ASSERT( format );
    
    va_start( vlist, format );
    (void)vsnprintf( buf, sizeof(buf), format, vlist );
    va_end( vlist );
    
    G2DDemo_PrintOut(x, y, buf);
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintApplyToHW

  Description:  内部スクリーンバッファをハードウェアに転送します。
                通常は V ブランクごとに呼び出します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintApplyToHW(void)
{
    DC_FlushRange( sScreenBuf, sizeof(sScreenBuf) );
    GX_LoadBG1Scr( sScreenBuf, 0, sizeof(sScreenBuf) );
}


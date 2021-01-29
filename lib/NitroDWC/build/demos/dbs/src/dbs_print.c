/*---------------------------------------------------------------------------*

  画面へのデバッグ用表示

  - BGスクリーンに文字列を表示します
  - 16色、256x256のスクリーン専用です
  
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include "dbs_print.h"
#include "dbs_print_data.h"




static BOOL          s_initialized;                                 // 初期化済みかどうかのフラグ
static tDbsInitParam s_initparam;                                   // 初期化パラメータ保存用
static GXScrFmtText  s_screen_data[24][32];                         // 文字列用スクリーンデータ
static void*         s_screen_loadaddr;                             // スクリーンロード先のアドレス
static int           s_charname_top;                                // フォントデータの先頭のキャラクタネーム
static int           s_pltt_base;                                   // パレット番号の先頭
static int           s_pltt_num;                                    // 文字列用に使うパレットの数
static u8            s_converted_font[ sizeof( s_font_chardata ) ]; // ピクセル色を変更後のフォント
static char          s_format_buffer[ 256 ];                        // 文字列フォーマット用バッファ。
static GXRgb         s_fgcolor[] = { GX_RGB(  0,  0,  0 ),          // black
                                    GX_RGB( 31,  0,  0 ),           // red
                                    GX_RGB(  0, 31,  0 ),           // green
                                    GX_RGB( 31, 25,  0 ),           // yellow
                                    GX_RGB(  0,  0, 31 ),           // blue
                                    GX_RGB( 31,  0, 31 ),           // magenta
                                    GX_RGB(  0, 31, 31 ),           // cyan
                                    GX_RGB( 31, 31, 31 )            // white
};
static GXRgb         s_bgcolor = GX_RGB( 31, 31, 31 );



// static functions
static int  GetEscapeCode( const char* p, int * advance );
static void DummyVBlankIntr(void);
static void PutString             ( int x, int y, const char* str, u32 strbufsize );
static void ConvertFontPixelcolor ( const u8* font_in, u8* font_out, int fontsize, int fg_color, int bg_color );





/*---------------------------------------------------------------------------*
  
  文字列描画を初期化します。何度呼び出してもかまいません。呼び出すごと
  にグラフィックスエンジンを設定します。

  param             : 文字列用に使うBG面はどこか。dbs_print.hで定義されたenum値。
  fontchar_location : フォント用キャラデータを配置する場所。BG面のキャラクタベースからのバイト数
  fg_color          : 文字列の文字色に割り当てるピクセルカラー
  bg_color          : 文字列の背景色に割り当てるピクセルカラー
  pltt_base         : 文字列用に使うパレットの始まり
  pltt_num          : 文字色を何色まで使いたいか。使った分だけパレットを占有する。

  初期化設定の例 その１

  fg_color  = 8
  bg_color  = 15
  pltt_base = 12
  pltt_num  = 4  の場合は

  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□●□□□□□□●
  □□□□□□□□●□□□□□□●
  □□□□□□□□●□□□□□□●
  □□□□□□□□●□□□□□□●

  上図の●の部分が文字列用として使われます。

  
  初期化設定の例 その２
  
  fg_color  = 15
  bg_color  = 0
  pltt_base = 0
  pltt_num  = 2  の場合は

  □□□□□□□□□□□□□□□●
  □□□□□□□□□□□□□□□●
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□
  □□□□□□□□□□□□□□□□

  上図の●の部分が文字列用として使われます。この場合は背景色が透明になります。
  
 *---------------------------------------------------------------------------*/
void dbs_Init       ( tDbsInitParam param, int fontchar_location, int fg_color, int bg_color, int pltt_base, int pltt_num )
{
    GXCharBGText16* bgchar;


    // 設定値を保存する
    s_initparam    = param;
	s_charname_top = fontchar_location / (int)sizeof( GXCharFmt16 );
    s_pltt_base    = pltt_base;
    s_pltt_num     = pltt_num;

    switch( param )
    {
    case eDBS_INIT_MAIN_BG0:
        bgchar = (GXCharBGText16*)G2_GetBG0CharPtr();
        s_screen_loadaddr = G2_GetBG0ScrPtr();
        break;
    case eDBS_INIT_MAIN_BG1:
        bgchar = (GXCharBGText16*)G2_GetBG1CharPtr();
        s_screen_loadaddr = G2_GetBG1ScrPtr();
        break;
    case eDBS_INIT_MAIN_BG2:
        bgchar = (GXCharBGText16*)G2_GetBG2CharPtr();
        s_screen_loadaddr = G2_GetBG2ScrPtr();
        break;
    case eDBS_INIT_MAIN_BG3:
        bgchar = (GXCharBGText16*)G2_GetBG3CharPtr();
        s_screen_loadaddr = G2_GetBG3ScrPtr();
        break;
    case eDBS_INIT_SUB_BG0:
        bgchar = (GXCharBGText16*)G2S_GetBG0CharPtr();
        s_screen_loadaddr = G2S_GetBG0ScrPtr();
        break;
    case eDBS_INIT_SUB_BG1:
        bgchar = (GXCharBGText16*)G2S_GetBG1CharPtr();
        s_screen_loadaddr = G2S_GetBG1ScrPtr();
        break;
    case eDBS_INIT_SUB_BG2:
        bgchar = (GXCharBGText16*)G2S_GetBG2CharPtr();
        s_screen_loadaddr = G2S_GetBG2ScrPtr();
        break;
    case eDBS_INIT_SUB_BG3:
        bgchar = (GXCharBGText16*)G2S_GetBG3CharPtr();
        s_screen_loadaddr = G2S_GetBG3ScrPtr();
        break;
    default:
        OS_Panic( "dbs_init : invalid parameter\n" );
        break;
    }


    // フォントを加工する
    if ( ! s_initialized ) // 加工は最初の一回だけ
    {
        ConvertFontPixelcolor( s_font_chardata, s_converted_font, sizeof(s_font_chardata), fg_color, bg_color );
    }

    // フォントをロードする
    MI_DmaCopy32( GX_GetDefaultDMA(), s_converted_font, & bgchar->ch[ s_charname_top ], sizeof( s_font_chardata ) );

    // パレットをロードする
    if( eDBS_INIT_MAIN_BG0 <= param && param <= eDBS_INIT_MAIN_BG3 ) // 上画面
    {
        int i;

        for( i = 0 ; i < s_pltt_num; i++ )
        {
            GX_LoadBGPltt ( & s_fgcolor[ i ], sizeof(GXBGPltt16) * ( s_pltt_base + i ) + sizeof(GXRgb) * fg_color,  sizeof(GXRgb) );

            if( bg_color != 0 ) // 背景色は透明でない場合のみロードする
            {
                GX_LoadBGPltt ( & s_bgcolor,  sizeof(GXBGPltt16) * ( s_pltt_base + i ) + sizeof(GXRgb) * bg_color,  sizeof(GXRgb) );
            }
        }
        
    }
    else // 下画面
    {
        int i;

        for( i = 0 ; i < s_pltt_num; i++ )
        {
            GXS_LoadBGPltt ( & s_fgcolor[ i ], sizeof(GXBGPltt16) * ( s_pltt_base + i ) + sizeof(GXRgb) * fg_color,  sizeof(GXRgb) );

            if( bg_color != 0 ) // 背景色は透明でない場合のみロードする
            {
                GXS_LoadBGPltt ( & s_bgcolor,      sizeof(GXBGPltt16) * ( s_pltt_base + i ) + sizeof(GXRgb) * bg_color,  sizeof(GXRgb) );
            }
        }
        
    }

    // スクリーンバッファをクリアする
    MI_CpuClear8( s_screen_data, sizeof( s_screen_data ) );

    s_initialized = TRUE;
    
}




/*---------------------------------------------------------------------------*
  
  グラフィックエンジンにスクリーンデータのロードを行います。通常のロー
  ド関数同様、VBLANK 期間中に呼び出す必要があります。
  
 *---------------------------------------------------------------------------*/
void dbs_Load       ( void )
{
    if ( ! s_initialized ) return; // 未初期化ならなにもしない

    DC_StoreRange( s_screen_data, sizeof( s_screen_data ) );
    MI_DmaCopy32( GX_GetDefaultDMA(), s_screen_data, s_screen_loadaddr, sizeof( s_screen_data ) );

    // ロード後はクリアする
    MI_CpuClear8( s_screen_data, sizeof( s_screen_data ) );

}




/*---------------------------------------------------------------------------*
  
  スクリーン座標 ( x, y ) の場所を基点として文字列を描画します。１フレー
  ムだけの表示になりますので、通常は毎ループ呼ぶ必要があります。
  
 *---------------------------------------------------------------------------*/
void dbs_Print      ( int x, int y, const char* fmt, ... )
{
    va_list va;
    va_start(va, fmt);
    (void)vsnprintf( s_format_buffer, sizeof(s_format_buffer), fmt, va );
    va_end(va);

    PutString( x, y, s_format_buffer, sizeof(s_format_buffer) );
}


void dbs_VPrint           ( int x, int y, const char *fmt, va_list vlist )
{
    (void)vsnprintf( s_format_buffer, sizeof(s_format_buffer), fmt, vlist );

    PutString( x, y, s_format_buffer, sizeof(s_format_buffer) );
    
}



/*---------------------------------------------------------------------------*
  
  コンテキスト情報を表示します。例外発生時のユーザコールバックとして使
  うことを想定した関数です。

 *---------------------------------------------------------------------------*/
void dbs_DisplayException ( u32 context_addr, void* /* arg */ )
{
    static int bg_offset = 8 * 8; // 例外情報は画面外に描いて、ずらして表示する。
    OSContext* context = (OSContext*) context_addr;
    int i;

    if ( ! s_initialized ) return; // 未初期化ならなにもしない

    // 例外情報をプリントする
    for( i=0; i<13; i++ )
    {
        dbs_Print( ( i % 2) * 16, i / 2, "R%02d=%08X ", i, context->r[i] );
    }

    dbs_Print( ( i % 2 ) * 16, i / 2, "SP =%08X ", context->sp );
    i++;
    
    dbs_Print( ( i % 2 ) * 16, i / 2, "LR =%08X ",  context->lr );
    i++;
    
    dbs_Print( ( i % 2 ) * 16, i / 2, "PC =%08X " , context->pc_plus4 );
    i++;


    DC_StoreRange( s_screen_data, sizeof( s_screen_data ) );

    // なにもしないVBLANK関数をセットして、割り込み許可状態にする
    OS_SetIrqFunction( OS_IE_V_BLANK, DummyVBlankIntr );
    (void)OS_EnableIrqMask( OS_IE_V_BLANK );
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr(TRUE);

    // VBLANK期間中にロードする
    OS_WaitIrq( 1, OS_IE_V_BLANK );
    MI_DmaCopy32( GX_GetDefaultDMA(), s_screen_data,
                  ((GXScrText32x32*)s_screen_loadaddr)->scr[24], // 画面外の領域にロードする
                  sizeof(GXScrFmtText) * 32 * 8 );

    // コントローラで例外情報表示位置を動かすためのメインループ
    while ( 1 )
    {
        u16 pad;

        pad = PAD_Read();

        if ( pad & PAD_KEY_UP )
        {
            bg_offset ++;
        }
        else if ( pad & PAD_KEY_DOWN )
        {
            bg_offset --;
        }

        if( bg_offset > 8 * 8 )
        {
            bg_offset = 8 * 8;
        }
        else if ( bg_offset < 0 )
        {
            bg_offset = 0;
        }
        
        switch( s_initparam )
        {
        case eDBS_INIT_MAIN_BG0:
            G2_SetBG0Offset( 0, bg_offset );
            break;
        case eDBS_INIT_MAIN_BG1:
            G2_SetBG1Offset( 0, bg_offset );
            break;
        case eDBS_INIT_MAIN_BG2:
            G2_SetBG2Offset( 0, bg_offset );
            break;
        case eDBS_INIT_MAIN_BG3:
            G2_SetBG3Offset( 0, bg_offset );
            break;
        case eDBS_INIT_SUB_BG0:
            G2S_SetBG0Offset( 0, bg_offset );
            break;
        case eDBS_INIT_SUB_BG1:
            G2S_SetBG1Offset( 0, bg_offset );
            break;
        case eDBS_INIT_SUB_BG2:
            G2S_SetBG2Offset( 0, bg_offset );
            break;
        case eDBS_INIT_SUB_BG3:
            G2S_SetBG3Offset( 0, bg_offset );
            break;
        }
        
        OS_WaitIrq( 1, OS_IE_V_BLANK );
    }

}





/*---------------------------------------------------------------------------*
  
  フォントのキャラデータを書き換えます。もとになるキャラデータは、4bit１ピクセルで
  透明 ＝０、文字色＝１、背景色＝２、という構成です。
  
 *---------------------------------------------------------------------------*/
static void ConvertFontPixelcolor( const u8* font_in, u8* font_out, int fontsize, int fg_color, int bg_color )
{
    int i;
    int table[3]; //

    SDK_NULL_ASSERT( font_in );
    SDK_NULL_ASSERT( font_out );
    SDK_ASSERT( fontsize > 0 );
    SDK_ASSERT( fg_color < 16 );
    SDK_ASSERT( bg_color < 16 );

    table[0] = 0;
    table[1] = fg_color;
    table[2] = bg_color;
    
    for( i = 0; i < fontsize; i++ )
    {
        int high, low;

        high = table[ ( font_in[ i ] & 0xF0 ) >> 4 ];
        low  = table[ ( font_in[ i ] &  0xF ) >> 0 ];
        
        font_out[ i ] = (u8) ( ( high << 4 ) | low );

    }

}





/*---------------------------------------------------------------------------*
  
  スクリーンに文字列をセットします
  
 *---------------------------------------------------------------------------*/
static void PutString( int x, int y, const char* str, u32 strbufsize )
{
    int i;
    int start_x = x;
    int color_index = 0; // s_fgcolorのインデクス

    for( i = 0 ; str[i] != '\0' && i < strbufsize; i++ )
    {
        if ( x < 0 )             continue; // 範囲外ならなにもしない
        if ( y < 0 || y >= 24 )  continue; // 範囲外ならなにもしない

        if ( str[i] == '\e' ) // エスケープ文字があったら色を変える
        {
            int code, advance;
            
            code = GetEscapeCode( & str[i], & advance );

            if ( code >= 0 )
            {
                color_index = code - DBS_BLACK;
            }

            i += advance - 1; // for文でi++されるので、1引いておく
            
            continue;
            
        }

        // 改行文字なら改行する
        if ( str[i] == '\n' )
        {
            x = start_x;
            y ++;
            continue;
        }
        

        s_screen_data[ y ][ x ] = GX_SCRFMT_TEXT( s_pltt_base + color_index % s_pltt_num, 0, 0, s_charname_top + str[i] - ' ' );

        x ++;
        // 右端についたら自動的に改行する
        if ( x >= 32 )
        {
            x = start_x;
            y ++;
        }

    }

}




/*---------------------------------------------------------------------------*
  
  例外表示時に設定する、なにもしないVBLANK関数。これがないと、VBLANK関
  数でdbs_Loadを呼んでいるときに困る。
  
 *---------------------------------------------------------------------------*/
static void DummyVBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}




/*---------------------------------------------------------------------------*

  文字色変更コードを取得

  p : エスケープシーケンス先頭
  advance : 何バイト使ったかを返すための変数

  returns:
  -1 失敗（知らないエスケープシーケンスがきた）
  30 "\e[30m" だった、つまり黒にセット
  ：
  ：
  37 "\e[37m" だった、つまり白にセット
  
 *---------------------------------------------------------------------------*/
static int  GetEscapeCode( const char* p, int * advance )
{
    int code = -1; // エラー値をいれておく
    
    SDK_ASSERT( *p == '\e' ); // エスケープシーケンスじゃないとこで呼ばれるはずはない

    // エラーチェック
    if ( p[1] != '[' )
    {
        *advance = 1;
        return code;
    }

    // エラーチェックその２
    if ( p[2] != '3' )
    {
        *advance = 2;
        return code;
    }

    // エラーチェックその３
    if ( p[3] < '0' || p[3] > '7' )
    {
        *advance = 3;
        return code;
    }

    // エラーチェックその4
    if ( p[4] != 'm' )
    {
        *advance = 4;
        return code;
    }

    code = DBS_BLACK + p[3] - '0';
    *advance = 5;

    return code;
        
}

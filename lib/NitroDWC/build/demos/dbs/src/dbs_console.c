#include <nitro.h>
#include "dbs_console.h"
#include "dbs_print.h"


typedef struct tLine
{
    u8 color;
    char str[1];
} tLine;


static tDbsConsole* sConsoleList; // コンソールのリンクリスト
static tDbsConsole* sDefaultConsole; // 引数にNULLを指定したときに使われる、デフォルトのコンソール



// static functions
static void CorrectOutOfRangeDisplay( tDbsConsole* con );
static const char* GetString( const tDbsConsole* con, u32 line );
static int GetColor( const tDbsConsole* con, u32 line );
static BOOL IsInsideBuffer( const tDbsConsole* con, u32 line );
static void Print( tDbsConsole* console, int color, const char* fmt, va_list vlist );
static tLine* GetLine( tDbsConsole* con, u32 line );
static void Draw( tDbsConsole* console );
static tDbsConsole* GetTarget( tDbsConsole* arg );
static void AppendConsole( tDbsConsole* appending );





/*---------------------------------------------------------------------------*

 コンソールを作成します
  
 *---------------------------------------------------------------------------*/
void dbs_CreateConsole( tDbsConsole* console, void* linebuf, u32 linebuf_size, int x, int y, int width, int height )
{
    SDK_NULL_ASSERT( console );

    console->linenum         = linebuf_size / ( width + 1 ) ; // 色保存用に１バイト使うので、width + 1で割る
    console->currentline     = 0;
    console->nowcolumn       = 0;
    console->displayline     = 0;
    MI_CpuClear8( linebuf, linebuf_size );
    console->linebuf         = (u8*)linebuf;
    AppendConsole( console );
    console->x               = (u8) x;
    console->y               = (u8) y;
    console->width           = (u8) width;
    console->height          = (u8) height;
    console->visible         = (u8) TRUE;
    console->linenumber_width = (u8) 0;
    console->stop_bufferfull = (u8) FALSE;
    console->default_color   = DBS_BLACK;

    // 一つ目の場合は自動でデフォルトに
    if ( sDefaultConsole == NULL )
    {
        sDefaultConsole = console;
    }
    
}




/*---------------------------------------------------------------------------*
  
  コンソールを削除します
  
 *---------------------------------------------------------------------------*/
void dbs_DestroyConsole( tDbsConsole* removing )
{
    tDbsConsole* con; // tDbsConsoleのnextフィールドのアドレスをどんどんいれていく変数

    if ( removing == NULL ) return;

    for( con = sConsoleList; ; con = con->next )
    {
        if ( !con ) break;

        if ( con == removing )
        {
            // 最初の要素がremoving.
            sConsoleList = removing->next;

            break;
        }
        else if ( con->next == removing )
        {
            // それ以降。
            con->next = removing->next;

            break;
        }
    }

    if ( removing == sDefaultConsole )
    {
        sDefaultConsole = sConsoleList;
    }
}





/*---------------------------------------------------------------------------*
  
  管理しているコンソールをすべて破棄します
  
 *---------------------------------------------------------------------------*/
void dbs_CleanupConsole( void )
{
    sConsoleList    = NULL;
    sDefaultConsole = NULL;
}



/*---------------------------------------------------------------------------*
  
  コンソールを描画します。グラフィックエンジンには一切さわらず、
  dbs_print関数を呼び出すだけです。
  
 *---------------------------------------------------------------------------*/
void dbs_DrawConsole( void )
{
    tDbsConsole* con;

    if ( ! sConsoleList ) return ; // コンソールがひとつも作られてなかったらなにもしない

    for ( con = sConsoleList; con != NULL; con = con->next )
    {
        if ( con->visible )
        {
            Draw( con );
        }
    }

}




/*---------------------------------------------------------------------------*
  
  デフォルトのコンソールに対して文字を表示します
  
 *---------------------------------------------------------------------------*/
void dbs_Puts( const char* fmt, ... )
{
    va_list vlist;
    
    if ( ! sDefaultConsole ) return;

    va_start( vlist, fmt );
    Print( sDefaultConsole, sDefaultConsole->default_color, fmt, vlist );
    va_end( vlist );
    
}




/*---------------------------------------------------------------------------*
  
  デフォルトのコンソールに対して色つきで文字を表示します
  
 *---------------------------------------------------------------------------*/
void dbs_PutsColor( int color, const char* fmt, ... )
{
    va_list vlist;
    
    if ( ! sDefaultConsole ) return;

    va_start( vlist, fmt );
    Print( sDefaultConsole, color, fmt, vlist );
    va_end( vlist );

}




/*---------------------------------------------------------------------------*
  
  コンソールを指定して文字を表示します。コンソールにNULLを指定するとデフォルトに表示します。
  
 *---------------------------------------------------------------------------*/
void dbs_CPrintf( tDbsConsole* console, const char* fmt, ... )
{
    va_list vlist;

    va_start( vlist, fmt );
    dbs_CVPrintf( console, fmt, vlist );
    va_end( vlist );
}

void dbs_CVPrintf( tDbsConsole* console, const char* fmt, va_list vlist )
{
    tDbsConsole* target;
    
    if ( ! sConsoleList ) return;

    target = GetTarget( console );

    Print( target, target->default_color, fmt, vlist );
}




/*---------------------------------------------------------------------------*
  
  コンソールと色を指定して文字を表示します。コンソールにNULLを指定するとデフォルトに表示します。
  
 *---------------------------------------------------------------------------*/
void dbs_PrintColorConsole( tDbsConsole* console, int color, const char* fmt, ... )
{
    tDbsConsole* target;
    va_list vlist;
    
    if ( ! sConsoleList ) return;

    target = GetTarget( console );


    va_start( vlist, fmt );
    Print( target, color, fmt, vlist );
    va_end( vlist );
    
}





/*---------------------------------------------------------------------------*
  
  コンソールの内容を、バッファも含めてすべて消去します。
  
 *---------------------------------------------------------------------------*/
void dbs_ClearConsole( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );
    
    con->currentline     = 0;
    con->displayline     = 0;
    MI_CpuClear8( con->linebuf, sizeof(u8) * ( con->width + 1 ) * ( con->linenum ) );
    
}





/*---------------------------------------------------------------------------*

  見える範囲を移動します。linesを正の値にすると進み、linesを負の値にすると戻ります。
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsole( tDbsConsole* console, int lines )
{
    tDbsConsole* con = GetTarget( console );

    // displaylineをゼロ未満にしてしまいそうな場合、ゼロでとめる
    if ( lines < 0 && con->displayline < -lines )
    {
        con->displayline = 0;
        return ;
    }

    con->displayline += lines;

    CorrectOutOfRangeDisplay( con );

}




/*---------------------------------------------------------------------------*

 バッファに残っている限りの一番古い行が見えるようにします
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsoleToFirstLine( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );

    con->displayline = con->currentline - con->linenum;

}



/*---------------------------------------------------------------------------*
  
  一番最後の一行だけが見えるような位置にスクロールします
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsoleToLastLine( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );

    con->displayline = con->currentline - 1;

}




/*---------------------------------------------------------------------------*
  
  1ページ前にもどります
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsoleToPrevPage( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );
    
    dbs_ScrollConsole( con, - con->height );
}




/*---------------------------------------------------------------------------*
  
  1ページ先に進みます
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsoleToNextPage( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );

    dbs_ScrollConsole( con, con->height );
}




/*---------------------------------------------------------------------------*
  
  デフォルトコンソールを変更します
  
 *---------------------------------------------------------------------------*/
void dbs_SetDefaultConsole( tDbsConsole* console )
{
    SDK_ASSERT( console );

    sDefaultConsole = console;

}



/*---------------------------------------------------------------------------*
  
  デフォルトコンソールを取得します
  
 *---------------------------------------------------------------------------*/
tDbsConsole* dbs_GetDefaultConsole( void )
{
    return sDefaultConsole;
}






/*---------------------------------------------------------------------------*
  
  コンソールの表示を切り替えます
  
 *---------------------------------------------------------------------------*/
void dbs_SetConsoleVisibility( tDbsConsole* console, BOOL show )
{
    GetTarget( console )->visible = (u8)show;
}



/*---------------------------------------------------------------------------*
  
  コンソールの可視状態を取得します
  
 *---------------------------------------------------------------------------*/
BOOL dbs_GetConsoleVisibility( tDbsConsole* console )
{
    return GetTarget( console )->visible;
}



/*---------------------------------------------------------------------------*
  
  コンソールの行番号表示に何桁使うかを設定します。行番号は、Print時に
  文字列先頭に挿入されますので、動的につけたり消したりすることはできま
  せん。
  
 *---------------------------------------------------------------------------*/
void dbs_SetConsoleLineNumberWidth( tDbsConsole* console, int width )
{
    SDK_ASSERT( width >= 0 );

    GetTarget( console )->linenumber_width = (u8)width;
    
}




/*---------------------------------------------------------------------------*
  
  バッファがまんたんになったときの挙動を指定します
  
 *---------------------------------------------------------------------------*/
void dbs_SetConsoleStopOnBufferFull( tDbsConsole* console,  BOOL stop )
{
    GetTarget( console )->stop_bufferfull = (u8)stop;
}




/*---------------------------------------------------------------------------*
  
  バッファがまんたんになったときの挙動を取得します
  
 *---------------------------------------------------------------------------*/
BOOL dbs_GetConsoleStopOnBufferFull( const tDbsConsole* console )
{
    return GetTarget( (tDbsConsole*)console )->stop_bufferfull;
}



/*---------------------------------------------------------------------------*
  
  リストに追加
  
 *---------------------------------------------------------------------------*/
static void AppendConsole( tDbsConsole* appending )
{
    appending->next = sConsoleList;
    sConsoleList = appending;
}





static void Print( tDbsConsole* console, int color, const char* fmt, va_list vlist )
{
    int i;
    tLine* linebuf = GetLine( console, console->currentline );
    char str[128];
    
    (void) OS_VSNPrintf( str, sizeof(str), fmt, vlist );

    // 色は行単位。
    linebuf->color = (u8)color;

    i = 0;
    while ( str[i] )
    {
        if ( console->nowcolumn == 0 )
        {
            linebuf = GetLine( console, console->currentline );
            linebuf->color = (u8)color;

            // これから書く行のバッファを消しておく。
            MI_CpuFill8( linebuf->str, ' ', console->width );
        }

        // 文字の描画
        if ( str[i] == 0x0d || str[i] == 0x0a )
        {
            // 改行
            console->nowcolumn = 0;
            console->currentline ++;

            // DOS改行対応
            if ( str[i] == 0x0d && str[i+1] == 0x0a )
            {
                i+=2;

                continue;
            }

            // UNIX/MAC改行
            i++;
            continue;
        }
        else if ( console->nowcolumn < console->width )
        {
            linebuf->str[console->nowcolumn] = str[i];

            console->nowcolumn++;

            if ( console->nowcolumn == console->width )
            {
                // 回り込み処理。
                console->nowcolumn = 0;
                console->currentline ++;
            }
        }

        i++;
    }

    // コンソール最下端を見ている場合は、自動的にスクロールさせる
    if( console->currentline > console->displayline + console->height )
    {
        // 数行飛ぶことがあるので、対策。
        console->displayline = console->currentline - console->height + 1;
    }
}


static void Draw( tDbsConsole* console )
{
    int i;

    for ( i = 0; i < console->height; i++ )
    {
        char tmpstr[64]; // コンソールの文字列バッファにはヌル文字のぶんが無いのでこれで作る
        
        MI_CpuCopy8( GetString( console, console->displayline + i ), tmpstr, console->width );
        tmpstr[ console->width ] = '\0';
        
        dbs_Print( console->x, console->y + i, tmpstr );
    }
    
}


static tDbsConsole* GetTarget( tDbsConsole* arg )
{
    if ( arg )
    {
        return arg;
    }
    else
    {
        return sDefaultConsole;
    }
    
}


static tLine* GetLine( tDbsConsole* con, u32 line )
{
    int offset; // 行の先頭が何バイト目にあたるか

	offset = (int)(( line % con->linenum ) * ( con->width + 1 ));

    return (tLine*)( & con->linebuf[ offset ] ) ;
    
}



/*---------------------------------------------------------------------------*
  
  コンソールのバッファから文字列取得。バッファからはみ出た範囲を指定し
  た場合、空白が32文字並んだものへのポインタが返る。
  
 *---------------------------------------------------------------------------*/
static const char* GetString( const tDbsConsole* con, u32 line )
{
                                  // 12345678901234567890123456789012
    static const char* whitespace = "                                ";

    if ( IsInsideBuffer( con, line ) )
    {
        return GetLine( (tDbsConsole*)con, line )->str;
    }
    else
    {
        return whitespace;
    }
}


static int GetColor( const tDbsConsole* con, u32 line )
{
    if ( IsInsideBuffer( con, line ) )
    {
        return GetLine( (tDbsConsole*)con, line )->color;
    }
    else
    {
        return DBS_BLACK;
    }
}


static BOOL IsInsideBuffer( const tDbsConsole* con, u32 line )
{
    // オーバーフローも考慮する、u32値同士の比較。「 > 」のかわりにつかう。
    // 差が0xFFFFFFFF/2(=2147483647)をこえると、不正な結果を返します。
#define GREATER( x, y )  ( 0 < (u32)((x) - (y)) && (u32)((x) - (y)) <= 0xFFFFFFFF/2 )

    u32 bufstart;

    bufstart = con->currentline - con->linenum - 1;
    
    if ( GREATER( line, bufstart ) &&
         GREATER( con->currentline, line ) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}




static void CorrectOutOfRangeDisplay( tDbsConsole* con )
{

    // 戻りすぎの場合、一番古い行が見えるようにする
    if ( con->displayline < con->currentline - con->linenum )
    {
        dbs_ScrollConsoleToFirstLine( con );
    }

}

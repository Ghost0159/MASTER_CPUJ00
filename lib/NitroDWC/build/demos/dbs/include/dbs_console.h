#ifndef DBS_CONSOLE_H_
#define DBS_CONSOLE_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct tDbsConsole tDbsConsole;
struct tDbsConsole
{
    u32 linenum;              // コンソールが保持できる文字の総行数。
    u32 currentline;          // 今何行目まで文字を保存したか。延々と増え続けます。
    u32 nowcolumn;            // 行のどこに文字を書いているか。
    u32 displayline;          // 表示箇所を制御するためのカウンタ。コンソール上端が何行目を表示しているか。
    u8* linebuf;              // 文字保存バッファ。
    tDbsConsole* next;        // 複数コンソールを管理するためのリスト用リンク
    u8 x;                     // コンソールX座標。単位はキャラクタ。
    u8 y;                     // コンソールY座標。単位はキャラクタ。
    u8 width;                 // コンソール幅。単位はキャラクタ。
    u8 height;                // コンソール高さ。単位はキャラクタ。
    u8 visible;               // 見えてるかどうか。Create直後はTRUE。
    u8 linenumber_width;      // 行番号に何桁使うか。Create直後は0．
    u8 stop_bufferfull;       // バッファが満タンになったとき止まるかどうか。Create直後はFALSE。
    u8 default_color;         // デフォルト色
    
};


// 作成・削除
void dbs_CreateConsole( tDbsConsole* console, void* linebuf, u32 linebuf_size, int x, int y, int width, int height );
void dbs_DestroyConsole( tDbsConsole* console );
void dbs_CleanupConsole( void );

// プリント
void dbs_Puts( const char* fmt, ... );
void dbs_PutsColor( int color, const char* fmt, ... );
void dbs_CPrintf ( tDbsConsole* console, const char* fmt, ... );
void dbs_CVPrintf( tDbsConsole* console, const char* fmt, va_list vlist );
void dbs_PrintColorConsole( tDbsConsole* console, int color, const char* fmt, ... );
void dbs_ClearConsole( tDbsConsole* console );
void dbs_DrawConsole( void );
int  dbs_VSNPrintf( char *dst, size_t len, const char *fmt, va_list vlist );
int  dbs_VNPrintf( char *dst, size_t len, const char *fmt, va_list vlist );

// スクロール
void dbs_ScrollConsole( tDbsConsole* console, int lines );
void dbs_ScrollConsoleToFirstLine( tDbsConsole* console );
void dbs_ScrollConsoleToLastLine( tDbsConsole* console );

void dbs_ScrollConsoleToPrevPage( tDbsConsole* console );
void dbs_ScrollConsoleToNextPage( tDbsConsole* console );

// 設定変更・取得
void dbs_SetDefaultConsole( tDbsConsole* console );
tDbsConsole* dbs_GetDefaultConsole( void );

void dbs_SetConsoleVisibility( tDbsConsole* console, BOOL show );
BOOL dbs_GetConsoleVisibility( tDbsConsole* console );

void dbs_SetConsoleStopOnBufferFull( tDbsConsole* console,  BOOL stop );
BOOL dbs_GetConsoleStopOnBufferFull( const tDbsConsole* console );

void dbs_SetConsoleLineNumberWidth( tDbsConsole* console, int width );


#ifdef __cplusplus
}/* extern "C" */
#endif
    

#endif // DBS_CONSOLE_H_

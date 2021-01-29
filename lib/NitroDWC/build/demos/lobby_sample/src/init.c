#include <nitro.h>
#include <dwc.h>
#include "init.h"
#include "dbs.h"

/** --------------------------------------------------------------------
  statics
  ----------------------------------------------------------------------*/

static char common_buffer[256];				// thread unsafe, but less use of stack
static BOOL sPrintOverride;					// デバッグ出力のオーバーライド
static BOOL sEnable_HeapDebug = FALSE;		// 
static u32  sHeapMaxAllocSize = 0;			// 最大allocサイズ

static OSHeapHandle sHandle;

#define ROUND(n, a)		(((u32)(n)+(a)-1) & ~((a)-1))

/** --------------------------------------------------------------------
  ヒープの初期化
  ----------------------------------------------------------------------*/
void Heap_Init(void)
{
	void*    arenaLo;
	void*    arenaHi;
	
	// [nakata] ヒープ領域の準備
    arenaLo = OS_GetArenaLo(OS_ARENA_MAIN);
    arenaHi = OS_GetArenaHi(OS_ARENA_MAIN);

    arenaLo = OS_InitAlloc(OS_ARENA_MAIN, arenaLo, arenaHi, 1);
    OS_SetArenaLo(OS_ARENA_MAIN, arenaLo);

    // [nakata] ポインタを32bit境界にアラインする
    arenaLo = (void*)MATH_ROUNDUP32((u32)arenaLo);
    arenaHi = (void*)MATH_ROUNDUP32((u32)arenaHi);

    // [nakata] ヒープ領域の作成
    sHandle = OS_CreateHeap(OS_ARENA_MAIN, arenaLo, arenaHi);
    OS_SetCurrentHeap(OS_ARENA_MAIN, sHandle );

    // From here on out, OS_Alloc and OS_Free behave like malloc and free respectively
    OS_SetArenaLo(OS_ARENA_MAIN, arenaHi);
}

/** --------------------------------------------------------------------
  毎フレーム呼ぶヒープのデバッグチェック
  ----------------------------------------------------------------------*/
void Heap_Debug( void )
{
    static u32 presize = 0;
    u32 size;

    if ( sEnable_HeapDebug )
    {
        size = OS_GetTotalAllocSize( OS_ARENA_MAIN, sHandle );

        if ( presize != size )
        {
            OS_TPrintf("***Heap:%d->%d\n", presize, size);

            presize = size;
        }
    }
}

void Heap_Print( void )
{
    u32 size;
    size = OS_GetTotalAllocSize( OS_ARENA_MAIN, sHandle );
    OS_TPrintf("***Heap:%d: max(%d)\n", size, sHeapMaxAllocSize);
}

void Heap_SetDebug( BOOL flag )
{
    sEnable_HeapDebug = flag;
}

void Heap_Dump( void )
{
    OS_DumpHeap( OS_ARENA_MAIN, sHandle );
}

void Heap_Update( void )
{
    u32 size;
    size = OS_GetTotalAllocSize( OS_ARENA_MAIN, sHandle );

    if ( sHeapMaxAllocSize < size )
    {
        sHeapMaxAllocSize = size;
    }
}

void Heap_Clear( void )
{
    sHeapMaxAllocSize = 0;
}

/** --------------------------------------------------------------------
  メモリ確保関数
  ----------------------------------------------------------------------*/
static void*
Heap_AllocFunc( DWCAllocType name, u32   size, int align )
{
    void * ptr;
    OSIntrMode old;
    (void)name;
    (void)align;

    old = OS_DisableInterrupts();

    ptr = OS_AllocFromMain( size );
    if(!ptr)
    {
        OS_Printf("+++ Alloc failed +++\n");
    }

    Heap_Update();

    OS_RestoreInterrupts( old );

    return ptr;
}

/** --------------------------------------------------------------------
  メモリ開放関数
  ----------------------------------------------------------------------*/
static void
Heap_FreeFunc( DWCAllocType name, void* ptr,  u32 size  )
{
    OSIntrMode old;
    (void)name;
    (void)size;

    if ( !ptr ) return;

    old = OS_DisableInterrupts();

    OS_FreeToMain( ptr );

    OS_RestoreInterrupts( old );
}

DWCAllocEx DTDB_GetAllocFunc( void )
{
    return Heap_AllocFunc;
}

DWCFreeEx DTDB_GetFreeFunc( void )
{
    return Heap_FreeFunc;
}

/** --------------------------------------------------------------------
  Print系のoverride
  ----------------------------------------------------------------------*/
void DTDB_SetPrintOverride( BOOL flag )
{
    sPrintOverride = flag;
}
/** --------------------------------------------------------------------
  OS_TVPrintfの実装
  ----------------------------------------------------------------------*/
void OS_TVPrintf(const char *fmt, va_list vlist )
{
    (void)OS_VSNPrintf(common_buffer, sizeof(common_buffer), fmt, vlist);
#ifndef SDK_FINALROM
    OS_PutString(common_buffer);
#endif

    if ( sPrintOverride )
    {
        dbs_CVPrintf( NULL, fmt, vlist );
    }
}
/** --------------------------------------------------------------------
  OS_TPrintfの実装
  ----------------------------------------------------------------------*/
#ifdef SDK_FINALROM
void OS_TPrintf(const char *fmt, ...)
{
    va_list vlist;

    va_start(vlist, fmt);
    OS_TVPrintf(fmt, vlist);
    va_end(vlist);
}
#endif

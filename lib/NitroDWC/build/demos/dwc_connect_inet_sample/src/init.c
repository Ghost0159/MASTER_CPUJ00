#include <nitro.h>
#include "init.h"


#define ROUND(n, a)		(((u32)(n)+(a)-1) & ~((a)-1))

static OSHeapHandle sHandle;

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
    arenaLo = (void*)ROUND(arenaLo, 32);
    arenaHi = (void*)ROUND(arenaHi, 32);

    // [nakata] ヒープ領域の作成
    sHandle = OS_CreateHeap(OS_ARENA_MAIN, arenaLo, arenaHi);
    OS_SetCurrentHeap(OS_ARENA_MAIN, sHandle );

    // From here on out, OS_Alloc and OS_Free behave like malloc and free respectively
    OS_SetArenaLo(OS_ARENA_MAIN, arenaHi);
}

static BOOL sEnableDebug = FALSE;
void Heap_Debug( void )
{
    static u32 presize = 0;
    u32 size;

    if ( sEnableDebug )
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
    OS_TPrintf("***Heap:%d\n", size);
}

void Heap_SetDebug( BOOL flag )
{
    sEnableDebug = flag;
}

void Heap_Dump( void )
{
    OS_DumpHeap( OS_ARENA_MAIN, sHandle );
}

void Alarm_Init(void)
{
	// [nakata] インフラストラクチャモードを使うために必要な初期化
	OS_InitTick();
	OS_InitAlarm();
	PXI_InitFifo();
}

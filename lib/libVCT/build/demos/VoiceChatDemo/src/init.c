#include <nitro.h>
#include "init.h"
#include "vct.h"

#define ROUND(n, a)		(((u32)(n)+(a)-1) & ~((a)-1))

static OSHeapHandle sHandle;

u8 gAudioBuffer[VCT_AUDIO_BUFFER_SIZE * VCT_DEFAULT_AUDIO_BUFFER_COUNT *
                VCT_MAX_AUDIO_STREAM] ATTRIBUTE_ALIGN(32);

void Heap_Init(void)
{
	void*    arenaLo;
	void*    arenaHi;
	
	// [nakata] �q�[�v�̈�̏���
    arenaLo = OS_GetArenaLo(OS_ARENA_MAIN);
    arenaHi = OS_GetArenaHi(OS_ARENA_MAIN);

    arenaLo = OS_InitAlloc(OS_ARENA_MAIN, arenaLo, arenaHi, 1);
    OS_SetArenaLo(OS_ARENA_MAIN, arenaLo);

    // [nakata] �|�C���^��32bit���E�ɃA���C������
    arenaLo = (void*)ROUND(arenaLo, 32);
    arenaHi = (void*)ROUND(arenaHi, 32);

    // [nakata] �q�[�v�̈�̍쐬
    sHandle = OS_CreateHeap(OS_ARENA_MAIN, arenaLo, arenaHi);
    OS_SetCurrentHeap(OS_ARENA_MAIN, sHandle );

    // From here on out, OS_Alloc and OS_Free behave like malloc and free respectively
    OS_SetArenaLo(OS_ARENA_MAIN, arenaHi);
}

void Heap_Debug( void )
{
    static u32 presize = 0;
    u32 size;

    size = OS_GetTotalAllocSize( OS_ARENA_MAIN, sHandle );

    if ( presize != size )
    {
        OS_TPrintf("***Heap:%d->%d\n", presize, size);

        presize = size;
    }
}

void Heap_Dump( void )
{
    OS_DumpHeap( OS_ARENA_MAIN, sHandle );
}

void Alarm_Init(void)
{
	// [nakata] �C���t���X�g���N�`�����[�h���g�����߂ɕK�v�ȏ�����
	OS_InitTick();
	OS_InitAlarm();
	PXI_InitFifo();
}
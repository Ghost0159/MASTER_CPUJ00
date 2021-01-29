/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - snd
  File:     heap.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.24 $
 *---------------------------------------------------------------------------*/
#include <nnsys/snd/heap.h>

#include <nnsys/misc.h>
#include <nnsys/fnd.h>

/******************************************************************************
	macro definition
 ******************************************************************************/

#define HEAP_ALIGN 32

#define ROUNDUP( value, align ) ( ( (u32)(value) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

/******************************************************************************
	structure definition
 ******************************************************************************/

typedef struct NNSSndHeap
{
    NNSFndHeapHandle handle;
    NNSFndList sectionList;
} NNSSndHeap;

typedef struct NNSSndHeapBlock
{
    NNSFndLink link;
    u32 size;
    NNSSndHeapDisposeCallback callback;
    u32 data1;
    u32 data2;
    u8 padding[ 0x20 - ( ( sizeof( NNSFndLink ) + sizeof( NNSSndHeapDisposeCallback ) + sizeof( u32 ) * 3 ) & 0x1f ) ];
    u32 buffer[ 0 ];
} NNSSndHeapBlock; // NOTE: must be 32 byte boundary

typedef struct NNSSndHeapSection
{
    NNSFndList blockList;
    NNSFndLink link;
} NNSSndHeapSection;

/******************************************************************************
	static function declarations
 ******************************************************************************/

static void InitHeapSection( NNSSndHeapSection* section );
static BOOL InitHeap( NNSSndHeap* heap, NNSFndHeapHandle handle );
static BOOL NewSection( NNSSndHeap* heap );
static void EraseSync( void );

/******************************************************************************
	public functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHeapCreate

  Description:  �q�[�v���쐬

  Arguments:    startAddress - �J�n�A�h���X
                size         - �������T�C�Y

  Returns:      �q�[�v�n���h��
 *---------------------------------------------------------------------------*/
NNSSndHeapHandle NNS_SndHeapCreate( void* startAddress, u32 size )
{
    NNSSndHeap* heap;
    void* endAddress;
    NNSFndHeapHandle handle;
    
    NNS_NULL_ASSERT( startAddress );
    
    endAddress   = (u8*)startAddress + size;
    startAddress = (void*)ROUNDUP( startAddress, 4 ); // NNSSndHeap align
    
    if ( startAddress > endAddress ) return NNS_SND_HEAP_INVALID_HANDLE;
    
    size = (u32)( (u8*)endAddress - (u8*)startAddress );
    if ( size < sizeof( NNSSndHeap ) ) {
        return NNS_SND_HEAP_INVALID_HANDLE;
    }
    
    size -= sizeof( NNSSndHeap );
    
    heap = (NNSSndHeap*)startAddress;
    startAddress = heap + 1;
    
    handle = NNS_FndCreateFrmHeap( startAddress, size );
    if ( handle == NNS_FND_HEAP_INVALID_HANDLE ) {
        return NULL;
    }
    
    if ( ! InitHeap( heap, handle ) ) {
        NNS_FndDestroyFrmHeap( handle );
        return NNS_SND_HEAP_INVALID_HANDLE;
    }
    
    return heap;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHeapDestroy

  Description:  �q�[�v��j�����܂�

  Arguments:    heap - �T�E���h�q�[�v

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndHeapDestroy( NNSSndHeapHandle heap )
{
    NNS_ASSERT( heap != NNS_SND_HEAP_INVALID_HANDLE );
    
    NNS_SndHeapClear( heap );
    
    NNS_FndDestroyFrmHeap( heap->handle );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHeapClear

  Description:  �q�[�v���쐬���̏�Ԃɖ߂�

  Arguments:    heap - .�T�E���h�q�[�v

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndHeapClear( NNSSndHeapHandle heap )
{
    NNSSndHeapSection* section=NULL;
    void* object;
    BOOL result;
    BOOL doCallback = FALSE;
    
    NNS_ASSERT( heap != NNS_SND_HEAP_INVALID_HANDLE );
    
    // �Z�N�V�����̔j��
    while ( ( section = (NNSSndHeapSection*)NNS_FndGetPrevListObject( & heap->sectionList, NULL ) ) != NULL )
    {
        // �R�[���o�b�N�̌Ăяo��
        object = NULL;
        while ( ( object = NNS_FndGetPrevListObject( & section->blockList, object ) ) != NULL )
        {
            NNSSndHeapBlock* block = (NNSSndHeapBlock*)object;
            if ( block->callback != NULL ) {
                block->callback( block->buffer, block->size, block->data1, block->data2 );
                doCallback = TRUE;
            }
        }
        
        // �Z�N�V�������X�g����̍폜
        NNS_FndRemoveListObject( & heap->sectionList, section );
    }
    
    // �q�[�v�̃N���A
    NNS_FndFreeToFrmHeap( heap->handle, NNS_FND_FRMHEAP_FREE_ALL );
    
    // �T�E���h�f�[�^�̎g�p��~�̓���
    if ( doCallback ) EraseSync();
    
    // �x�[�X�Z�N�V�����̍쐬
    result = NewSection( heap );
    NNS_ASSERTMSG( result, "NNS_SndHeapClear(): NewSection is Failed");
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHeapAlloc

  Description:  �q�[�v���烁�������m��

  Arguments:    heap     - �T�E���h�q�[�v
                size     - �������T�C�Y
                callback - ���������j�����ꂽ�Ƃ��ɌĂт������R�[���o�b�N�֐�
                data1    - �R�[���o�b�N�f�[�^1
                data2    - �R�[���o�b�N�f�[�^2

  Returns:      �m�ۂ����������̃|�C���^
 *---------------------------------------------------------------------------*/
void* NNS_SndHeapAlloc( NNSSndHeapHandle heap, u32 size, NNSSndHeapDisposeCallback callback, u32 data1, u32 data2 )
{
    NNSSndHeapSection* section;
    NNSSndHeapBlock* block;
    
    NNS_ASSERT( heap != NNS_SND_HEAP_INVALID_HANDLE );
    
    block = (NNSSndHeapBlock*)NNS_FndAllocFromFrmHeapEx(
        heap->handle, sizeof( NNSSndHeapBlock ) + ROUNDUP( size, HEAP_ALIGN ), HEAP_ALIGN );
    if ( block == NULL ) return NULL;
    
    section = (NNSSndHeapSection*)NNS_FndGetPrevListObject( & heap->sectionList, NULL );
    
    block->size = size;
    block->callback = callback;
    block->data1 = data1;
    block->data2 = data2;
    NNS_FndAppendListObject( & section->blockList, block );
    
    NNS_ASSERTMSG( ( (u32)( block->buffer ) & 0x1f ) == 0, "NNS_SndHeapAlloc: Internal Error" );
    
    return block->buffer;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHeapSaveState

  Description:  �q�[�v�̏�Ԃ�ۑ�

  Arguments:    heap - �T�E���h�q�[�v.

  Returns:      �ۑ������K�w���x����Ԃ�
                ���s���ɂ́A-1
 *---------------------------------------------------------------------------*/
int NNS_SndHeapSaveState( NNSSndHeapHandle heap )
{
    BOOL result;
    
    NNS_ASSERT( heap != NNS_SND_HEAP_INVALID_HANDLE );
    
    if ( ! NNS_FndRecordStateForFrmHeap( heap->handle, heap->sectionList.numObjects ) ) {
        return -1;
    }
    
    if ( ! NewSection( heap ) ) {
        result = NNS_FndFreeByStateToFrmHeap( heap->handle, 0 );
        NNS_ASSERTMSG( result, "NNS_SndHeapSaveState(): NNS_FndFreeByStateToFrmHeap is Failed");
        return -1;
    }
    
    return heap->sectionList.numObjects - 1;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHeapLoadState

  Description:  �q�[�v�̏�Ԃ�߂�

  Arguments:    heap  - �T�E���h�q�[�v
                level - �K�w���x��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndHeapLoadState( NNSSndHeapHandle heap, int level )
{
    NNSSndHeapSection* section;
    void* object = NULL;
    BOOL result;
    BOOL doCallback = FALSE;
    
    NNS_ASSERT( heap != NNS_SND_HEAP_INVALID_HANDLE );
    NNS_MINMAX_ASSERT( level, 0, heap->sectionList.numObjects-1 );
    
    if ( level == 0 ) {
        NNS_SndHeapClear( heap );
        return;
    }
    while( level < heap->sectionList.numObjects )
    {
        // get latest section
        section = (NNSSndHeapSection*)NNS_FndGetPrevListObject( & heap->sectionList, NULL );
        
        // call dispose callback
        while ( ( object = NNS_FndGetPrevListObject( & section->blockList, object ) ) != NULL )
        {
            NNSSndHeapBlock* block = (NNSSndHeapBlock*)object;
            if ( block->callback != NULL ) {
                block->callback( block->buffer, block->size, block->data1, block->data2 );
                doCallback = TRUE;
            }
        }
        
        // �Z�N�V�������X�g����̍폜
        NNS_FndRemoveListObject( & heap->sectionList, section );
    }
    
    // �q�[�v��Ԃ𕜌�
    result = NNS_FndFreeByStateToFrmHeap( heap->handle, (u32)level );
    NNS_ASSERTMSG( result, "NNS_SndHeapLoadState(): NNS_FndFreeByStateToFrmHeap is Failed");   

    // �T�E���h�f�[�^�̎g�p��~�̓���
    if ( doCallback ) EraseSync();
    
    // �ēx�L�^
    result =  NNS_FndRecordStateForFrmHeap( heap->handle, heap->sectionList.numObjects );
    NNS_ASSERTMSG( result, "NNS_SndHeapLoadState(): NNS_FndRecordStateForFrmHeap is Failed");
    
    // �Z�N�V�����̍쐬
    result = NewSection( heap );
    NNS_ASSERTMSG( result, "NNS_SndHeapLoadState(): NewSection is Failed");
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHeapGetCurrentLevel

  Description:  �q�[�v�̌��݂̊K�w���x�����擾

  Arguments:    heap - �T�E���h�q�[�v

  Returns:      ���݂̊K�w���x��
 *---------------------------------------------------------------------------*/
int NNS_SndHeapGetCurrentLevel( NNSSndHeapHandle heap )
{
    NNS_ASSERT( heap != NNS_SND_HEAP_INVALID_HANDLE );
    
    return heap->sectionList.numObjects - 1;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHeapGetSize

  Description:  �q�[�v�̗e�ʂ��擾

  Arguments:    heap - �T�E���h�q�[�v

  Returns:      �q�[�v�̗e��
 *---------------------------------------------------------------------------*/
u32 NNS_SndHeapGetSize( NNSSndHeapHandle heap )
{
    NNS_ASSERT( heap != NNS_SND_HEAP_INVALID_HANDLE );
    
    return
        (u32)( (u8*)NNS_FndGetHeapEndAddress( heap->handle ) -
               (u8*)NNS_FndGetHeapStartAddress( heap->handle ) )
        ;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHeapGetFreeSize

  Description:  �q�[�v�̋󂫗e�ʂ��擾

  Arguments:    heap - �T�E���h�q�[�v

  Returns:      �󂫗e��
 *---------------------------------------------------------------------------*/
u32 NNS_SndHeapGetFreeSize( NNSSndHeapHandle heap )
{
    u32 size;
    
    NNS_ASSERT( heap != NNS_SND_HEAP_INVALID_HANDLE );
    
    size = NNS_FndGetAllocatableSizeForFrmHeapEx( heap->handle, HEAP_ALIGN );
    
    if ( size < sizeof( NNSSndHeapBlock ) ) return 0;
    size -= sizeof( NNSSndHeapBlock );
    
    size &= ~0x1f;
    
    return size;
}

/******************************************************************************
	static functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         InitHeapSection

  Description:  NNSSndHeapSection �\���̂�������

  Arguments:    section - �q�[�v�Z�N�V����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitHeapSection( NNSSndHeapSection* section )
{
    NNS_FND_INIT_LIST( & section->blockList, NNSSndHeapBlock, link );    
}

/*---------------------------------------------------------------------------*
  Name:         InitHeap

  Description:  NNSSndHeap �\���̂�������

  Arguments:    heap   - �T�E���h�q�[�v
                handle - �t���[���q�[�v�n���h��

  Returns:      �����������ǂ���
 *---------------------------------------------------------------------------*/
static BOOL InitHeap( NNSSndHeap* heap, NNSFndHeapHandle handle )
{
    NNS_FND_INIT_LIST( & heap->sectionList, NNSSndHeapSection, link );
    heap->handle = handle;
    
    // �x�[�X�Z�N�V�����̍쐬
    if ( ! NewSection( heap ) ) {
        return FALSE;
    }
    
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NewSection

  Description:  �V�����Z�N�V�������쐬

  Arguments:    heap - �T�E���h�q�[�v

  Returns:      �����������ǂ���
 *---------------------------------------------------------------------------*/
static BOOL NewSection( NNSSndHeap* heap )
{
    NNSSndHeapSection* section;
    
    // new NNSSndHeapSection
    section = (NNSSndHeapSection*)NNS_FndAllocFromFrmHeap( heap->handle, sizeof( NNSSndHeapSection ) );
    if ( section == NULL ) {
        return FALSE;
    }
    InitHeapSection( section );
    
    NNS_FndAppendListObject( & heap->sectionList, section );
    
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         EraseSync

  Description:  �q�[�v�̍폜���ɁA�f�[�^�̎g�p�����S�ɒ�~���邽�߂̓���

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void EraseSync( void )
{
    u32 commandTag;
    
    commandTag = SND_GetCurrentCommandTag();
    (void)SND_FlushCommand( SND_COMMAND_BLOCK );
    SND_WaitForCommandProc( commandTag );
}

/*====== End of heap.c ======*/


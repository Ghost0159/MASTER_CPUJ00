/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - fnd
  File:     frameheap.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.20 $
 *---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <nitro.h>
#include <nnsys/misc.h>
#include <nnsys/fnd/frameheap.h>
#include <nnsys/fnd/config.h>
#include "heapcommoni.h"

/* ========================================================================
    �}�N���萔
   ======================================================================== */

// �A���C�����g�̍ŏ��l
#define MIN_ALIGNMENT           4


/* ========================================================================
    static�֐�
   ======================================================================== */

static NNS_FND_INLINE BOOL
IsValidFrmHeapHandle(NNSFndHeapHandle handle)
{
    if(handle == NNS_FND_HEAP_INVALID_HANDLE)
    {
        return FALSE;
    }

    {
        NNSiFndHeapHead* pHeapHd = handle;
        return pHeapHd->signature == NNSI_FRMHEAP_SIGNATURE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         GetFrmHeapHeadPtrFromHeapHead

  Description:  �q�[�v�w�b�_�ւ̃|�C���^����A�t���[���q�[�v�w�b�_�ւ̃|�C���^���擾���܂��B

  Arguments:    pHHead:  �q�[�v�w�b�_�ւ̃|�C���^�B

  Returns:      �t���[���q�[�v�w�b�_�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE NNSiFndFrmHeapHead*
GetFrmHeapHeadPtrFromHeapHead(NNSiFndHeapHead* pHHead)
{
    return AddU32ToPtr(pHHead, sizeof(NNSiFndHeapHead));
}

/*---------------------------------------------------------------------------*
  Name:         GetHeapHeadPtrFromFrmHeapHead

  Description:  �t���[���q�[�v�w�b�_�ւ̃|�C���^����A�q�[�v�w�b�_�ւ̃|�C���^���擾���܂��B

  Arguments:    pFrmHeapHd:  �t���[���q�[�v�w�b�_�ւ̃|�C���^�B

  Returns:      �q�[�v�w�b�_�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE NNSiFndHeapHead*
GetHeapHeadPtrFromFrmHeapHead(NNSiFndFrmHeapHead* pFrmHeapHd)
{
    return SubU32ToPtr(pFrmHeapHd, sizeof(NNSiFndHeapHead));
}

/*---------------------------------------------------------------------------*
  Name:         InitFrameHeap

  Description:  �t���[���q�[�v�̏��������s���܂��B

  Arguments:    startAddress:  �t���[���q�[�v�Ƃ��郁�����̊J�n�A�h���X�B
                endAddress:    �t���[���q�[�v�Ƃ��郁�����̏I���A�h���X +1�B
                optFlag:       �I�v�V�����t���O�B

  Returns:      �q�[�v�w�b�_�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNSiFndHeapHead*
InitFrameHeap(
    void*   startAddress,
    void*   endAddress,
    u16     optFlag
)
{
    NNSiFndHeapHead* pHeapHd = startAddress;
    NNSiFndFrmHeapHead* pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(pHeapHd);

    NNSi_FndInitHeapHead(       // �q�[�v���ʏ�����
        pHeapHd,
        NNSI_FRMHEAP_SIGNATURE,
        AddU32ToPtr(pFrmHeapHd, sizeof(NNSiFndFrmHeapHead)),    // heapStart
        endAddress,                                             // heapEnd
        optFlag);

    pFrmHeapHd->headAllocator = pHeapHd->heapStart;
    pFrmHeapHd->tailAllocator = pHeapHd->heapEnd;

    pFrmHeapHd->pState = NULL;   // ��ԕۑ��X�e�[�g�ʒu

    return pHeapHd;
}


/*---------------------------------------------------------------------------*
  Name:         AllocFromHead

  Description:  �q�[�v�̐擪���烁�����u���b�N���m�ۂ��܂��B
                �A���C�������g�̎w�肪����܂��B

  Arguments:    pHHead:  �q�[�v�w�b�_�ւ̃|�C���^�B
                size:    �m�ۂ��郁�����u���b�N�̃T�C�Y�B
                alignment:  �A���C�����g�l�B

  Returns:      �������u���b�N�̊m�ۂ����������ꍇ�A�m�ۂ����������u���b�N�ւ�
                �|�C���^���Ԃ�܂��B
                ���s�����ꍇ�ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
static void*
AllocFromHead(
    NNSiFndFrmHeapHead* pFrmHeapHd,
    u32                 size,
    int                 alignment
)
{
    void* newBlock = NNSi_FndRoundUpPtr(pFrmHeapHd->headAllocator, alignment);
    void* endAddress = AddU32ToPtr(newBlock, size);

    if(NNSiGetUIntPtr(endAddress) > NNSiGetUIntPtr(pFrmHeapHd->tailAllocator))
    {
        return NULL;
    }

    FillAllocMemory(  // �������[�U
        GetHeapHeadPtrFromFrmHeapHead(pFrmHeapHd),
        pFrmHeapHd->headAllocator,
        GetOffsetFromPtr(pFrmHeapHd->headAllocator, endAddress));

    pFrmHeapHd->headAllocator = endAddress;

    return newBlock;
}

/*---------------------------------------------------------------------------*
  Name:         AllocFromTail

  Description:  �q�[�v�̖������烁�����u���b�N���m�ۂ��܂��B
                �A���C�������g�̎w�肪����܂��B

  Arguments:    pHHead:     �q�[�v�w�b�_�ւ̃|�C���^�B
                size:       �m�ۂ��郁�����u���b�N�̃T�C�Y�B
                alignment:  �A���C�����g�l�B

  Returns:      �������u���b�N�̊m�ۂ����������ꍇ�A�m�ۂ����������u���b�N�ւ�
                �|�C���^���Ԃ�܂��B
                ���s�����ꍇ�ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
static void*
AllocFromTail(
    NNSiFndFrmHeapHead* pFrmHeapHd,
    u32                 size,
    int                 alignment
)
{
    void* newBlock = NNSi_FndRoundDownPtr(SubU32ToPtr(pFrmHeapHd->tailAllocator, size), alignment);

    if(NNSiGetUIntPtr(newBlock) < NNSiGetUIntPtr(pFrmHeapHd->headAllocator))
    {
        return NULL;
    }

    FillAllocMemory(  // �������[�U
        GetHeapHeadPtrFromFrmHeapHead(pFrmHeapHd),
        newBlock,
        GetOffsetFromPtr(newBlock, pFrmHeapHd->tailAllocator));

    pFrmHeapHd->tailAllocator = newBlock;

    return newBlock;
}

/*---------------------------------------------------------------------------*
  Name:         FreeHead

  Description:  �q�[�v�̈�̐擪����m�ۂ����������u���b�N���ꊇ���ĊJ�����܂��B

  Arguments:    pHeapHd:  �q�[�v�̃w�b�_�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
FreeHead(NNSiFndHeapHead* pHeapHd)
{
    NNSiFndFrmHeapHead* pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(pHeapHd);

    FillFreeMemory(
        pHeapHd,
        pHeapHd->heapStart,
        GetOffsetFromPtr(pHeapHd->heapStart, pFrmHeapHd->headAllocator));

    pFrmHeapHd->headAllocator = pHeapHd->heapStart;
    pFrmHeapHd->pState = NULL;
}

/*---------------------------------------------------------------------------*
  Name:         FreeTail

  Description:  �q�[�v����m�ۂ����S�Ẵ������u���b�N���ꊇ���ĊJ�����܂��B

  Arguments:    pHeapHd:  �q�[�v�̃w�b�_�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
FreeTail(NNSiFndHeapHead* pHeapHd)
{
    NNSiFndFrmHeapHead* pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(pHeapHd);

    FillFreeMemory(
        pHeapHd,
        pFrmHeapHd->tailAllocator,
        GetOffsetFromPtr(pFrmHeapHd->tailAllocator, pHeapHd->heapEnd));

    /*
        �q�[�v�̊��蓖�ď�Ԃ̕��A�ɂ���������������u���b�N���������Ă��܂�
        �Ȃ��悤�ɁA�ۑ����̌�����蓖�ă|�C���^���Đݒ肵�Ă����B
     */
    {
        NNSiFndFrmHeapState* pState;
        for (pState = pFrmHeapHd->pState; pState; pState = pState->pPrevState)
        {
            pState->tailAllocator = pHeapHd->heapEnd;
        }
    }

    pFrmHeapHd->tailAllocator = pHeapHd->heapEnd;
}

/*---------------------------------------------------------------------------*
  Name:         PrintSize

  Description:  �T�C�Y�ƃp�[�Z���e�[�W���o�͂��܂��B

  Arguments:    size:       �ΏۂƂȂ�T�C�Y�B
                wholeSize:  �S�̂̃T�C�Y�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    static void
    PrintSize(
        u32     size,
        u32     wholeSize
    )
    {
        OS_Printf("%9d (%6.2f%%)", size, 100.0 * size / wholeSize);
    }

// #if ! defined(NNS_FINALROM)
#endif


/* ========================================================================
    �O���֐�(����J)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndGetFreeStartForFrmHeap

  Description:  �t���[���q�[�v�̃t���[�G���A�̐擪�A�h���X���擾���܂��B

  Arguments:    heap: �t���[���q�[�v�̃n���h���B

  Returns:      �t���[���q�[�v�̃t���[�G���A�̐擪�A�h���X��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void*
NNSi_FndGetFreeStartForFrmHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidFrmHeapHandle(heap));

    return GetFrmHeapHeadPtrFromHeapHead(heap)->headAllocator;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndGetFreeEndForFrmHeap

  Description:  �t���[���q�[�v�̃t���[�G���A�̖����A�h���X���擾���܂��B

  Arguments:    heap: �t���[���q�[�v�̃n���h���B

  Returns:      �t���[���q�[�v�̃t���[�G���A�̖����A�h���X +1 ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void*
NNSi_FndGetFreeEndForFrmHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidFrmHeapHandle(heap));

    return GetFrmHeapHeadPtrFromHeapHead(heap)->tailAllocator;
}


/*---------------------------------------------------------------------------*
  Name:         NNSi_FndDumpFrmHeap

  Description:  �t���[���q�[�v�����̏���\�����܂��B
                ����̓f�o�b�O�p�̊֐��ł��B

  Arguments:    heap:    �t���[���q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    void
    NNSi_FndDumpFrmHeap(NNSFndHeapHandle heap)
    {
        NNS_ASSERT(IsValidFrmHeapHandle(heap));

        {
            NNSiFndHeapHead *const pHeapHd = heap;
            NNSiFndFrmHeapHead *const pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(pHeapHd);
            const u32 heapSize = GetOffsetFromPtr(pHeapHd->heapStart, pHeapHd->heapEnd);

            NNSi_FndDumpHeapHead(pHeapHd);

            OS_Printf(  "     head [%p - %p) ", pHeapHd->heapStart, pFrmHeapHd->headAllocator);
            PrintSize(GetOffsetFromPtr(pHeapHd->heapStart, pFrmHeapHd->headAllocator), heapSize);
            OS_Printf("\n     free                           ");
            PrintSize(GetOffsetFromPtr(pFrmHeapHd->headAllocator, pFrmHeapHd->tailAllocator), heapSize);
            OS_Printf("\n     tail [%p - %p) ", pFrmHeapHd->tailAllocator, pHeapHd->heapEnd);
            PrintSize(GetOffsetFromPtr(pFrmHeapHd->tailAllocator, pHeapHd->heapEnd), heapSize);
            OS_Printf("\n");

            if (pFrmHeapHd->pState)
            {
                NNSiFndFrmHeapState* pState;

                OS_Printf("    state : [tag]   [head]      [tail]\n");

                for (pState = pFrmHeapHd->pState; pState; pState = pState->pPrevState)
                {
                    OS_Printf("            '%c%c%c%c' : %p %p\n", pState->tagName >>24, (pState->tagName >>16) & 0xFF, (pState->tagName >>8) & 0xFF, pState->tagName & 0xFF,
                                            pState->headAllocator, pState->tailAllocator);
                }
            }

            OS_Printf("\n");
        }
    }

// #if ! defined(NNS_FINALROM)
#endif


/* ========================================================================
    �O���֐�(���J)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNS_FndCreateFrmHeapEx

  Description:  �t���[���q�[�v���쐬���܂��B

  Arguments:    startAddress: �q�[�v�̈�̐擪�A�h���X�B
                size:         �q�[�v�̈�̃T�C�Y�B
                optFlag:      �I�v�V�����t���O�B

  Returns:      �֐������������ꍇ�A�쐬���ꂽ�t���[���q�[�v�̃n���h�����Ԃ�܂��B
                �֐������s����ƁANNS_FND_INVALID_HEAP_HANDLE ���Ԃ�܂��B

  Memo:         ��{�̓X���b�h�Z�[�t�ł͂Ȃ��B
                �X���b�h�Z�[�t�ɂ���ꍇ�A�q�[�v�̑������w�肷�������ǉ�����悤�ɂ��邩�A
                ���邢�́A�������Z�b�g����֐��Ő��䂵�Ă��炤���B
 *---------------------------------------------------------------------------*/
NNSFndHeapHandle
NNS_FndCreateFrmHeapEx(
    void*   startAddress,
    u32     size,
    u16     optFlag
)
{
    void* endAddress;

    SDK_NULL_ASSERT(startAddress);

    endAddress   = NNSi_FndRoundDownPtr(AddU32ToPtr(startAddress, size), MIN_ALIGNMENT);
    startAddress = NNSi_FndRoundUpPtr(startAddress, MIN_ALIGNMENT);

    if ( NNSiGetUIntPtr(startAddress) > NNSiGetUIntPtr(endAddress)
     ||  GetOffsetFromPtr(startAddress, endAddress) < sizeof(NNSiFndHeapHead) + sizeof(NNSiFndFrmHeapHead)
    )
    {
        return NNS_FND_HEAP_INVALID_HANDLE;
    }

    {   // Frame �q�[�v����������
        NNSiFndHeapHead* pHHead = InitFrameHeap(startAddress, endAddress, optFlag);
        return pHHead;  // �q�[�v�w�b�_�ւ̃|�C���^�����̂܂܃n���h���l�Ƃ���
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNS_FndDestroyFrmHeap

  Description:  �t���[���q�[�v��j�����܂��B

  Arguments:    heap: �t���[���q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndDestroyFrmHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidFrmHeapHandle(heap));

    NNSi_FndFinalizeHeap(heap);
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndAllocFromFrmHeapEx

  Description:  �t���[���q�[�v���烁�����u���b�N���m�ۂ��܂��B
                �������u���b�N�̃A���C�����g���w��ł��܂��B
                �A���C�����g�l�𕉂̒l�Ŏw�肷��ƁA�q�[�v�̋󂫗̈���������T���܂��B

  Arguments:    heap:      �t���[���q�[�v�̃n���h���B
                size:      �m�ۂ��郁�����u���b�N�̃T�C�Y(�o�C�g�P��)�B
                alignment: �m�ۂ��郁�����u���b�N�̃A���C�����g�B
                           4,8,16,32,-4,-8,-16,-32�̂����ꂩ�̒l���w��ł��܂��B

  Returns:      �������u���b�N�̊m�ۂ����������ꍇ�A�m�ۂ����������u���b�N�ւ�
                �|�C���^���Ԃ�܂��B
                ���s�����ꍇ�ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
void*
NNS_FndAllocFromFrmHeapEx(
    NNSFndHeapHandle    heap,
    u32                 size,
    int                 alignment
)
{
    void* memory = NULL;
    NNSiFndFrmHeapHead* pFrmHeapHd;

    NNS_ASSERT(IsValidFrmHeapHandle(heap));

    // alignment �̃`�F�b�N
    NNS_ASSERT(alignment % MIN_ALIGNMENT == 0);
    NNS_ASSERT(MIN_ALIGNMENT <= abs(alignment) && abs(alignment) <= 32);

    pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(heap);

    if (size == 0)
    {
        size = 1;
    }

    size = NNSi_FndRoundUp(size, MIN_ALIGNMENT);

    if (alignment >= 0)   // �q�[�v�O����m��
    {
        memory = AllocFromHead(pFrmHeapHd, size, alignment);
    }
    else                    // �q�[�v��납��m��
    {
        memory = AllocFromTail(pFrmHeapHd, size, -alignment);
    }

    return memory;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndFreeToFrmHeap

  Description:  �t���[���q�[�v�փ������u���b�N��ԋp���܂��B

  Arguments:    heap: �t���[���q�[�v�̃n���h���B
                mode: �������u���b�N�̕ԋp���@�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndFreeToFrmHeap(
    NNSFndHeapHandle    heap,
    int                 mode
)
{
    NNS_ASSERT(IsValidFrmHeapHandle(heap));

    if (mode & NNS_FND_FRMHEAP_FREE_HEAD)
    {
        FreeHead(heap);
    }

    if (mode & NNS_FND_FRMHEAP_FREE_TAIL)
    {
        FreeTail(heap);
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetAllocatableSizeForFrmHeapEx

  Description:  �t���[���q�[�v���̊��蓖�ĉ\�ȍő�T�C�Y���擾���܂��B
                �������u���b�N�̃A���C�����g���w��ł��܂��B

  Arguments:    heap:      �t���[���q�[�v�̃n���h���B
                alignment: �m�ۂ��郁�����u���b�N�̃A���C�����g�B
                           4,8,16,32�̂����ꂩ�̒l���w��ł��܂��B

  Returns:      �t���[���q�[�v���̊��蓖�ĉ\�ȍő�T�C�Y��Ԃ��܂�(�o�C�g�P��)�B
 *---------------------------------------------------------------------------*/
u32
NNS_FndGetAllocatableSizeForFrmHeapEx(
    NNSFndHeapHandle    heap,
    int                 alignment
)
{
    NNS_ASSERT(IsValidFrmHeapHandle(heap));

    // alignment �̃`�F�b�N
    NNS_ASSERT(alignment % MIN_ALIGNMENT == 0);
    NNS_ASSERT(MIN_ALIGNMENT <= abs(alignment) && abs(alignment) <= 32);

    alignment = abs(alignment); // �O�̂��ߐ�����

    {
        const NNSiFndFrmHeapHead* pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(heap);
        const void* block = NNSi_FndRoundUpPtr(pFrmHeapHd->headAllocator, alignment);

        if (NNSiGetUIntPtr(block) > NNSiGetUIntPtr(pFrmHeapHd->tailAllocator))
        {
            return 0;
        }

        return GetOffsetFromPtr(block, pFrmHeapHd->tailAllocator);
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNS_FndRecordStateForFrmHeap

  Description:  �t���[���q�[�v�̌��݂̃������g�p��Ԃ��L�^���܂��B
                ��ŋL�^�����������g�p�󋵂ɖ߂����Ƃ��ł��܂��B
                ��Ԃ��L�^����̂�20�o�C�g�g�p���܂��B

  Arguments:    heap:     �t���[���q�[�v�̃n���h���B
                tagName:  ��ԋL�^�ɕt����^�O���B

  Returns:      �֐������������ꍇ�ATRUE���Ԃ�܂��B
                ���s������AFALSE���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_FndRecordStateForFrmHeap(
    NNSFndHeapHandle    heap,
    u32                 tagName
)
{
    NNS_ASSERT(IsValidFrmHeapHandle(heap));

    {
        NNSiFndFrmHeapHead* pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(heap);
        void* oldHeadAllocator = pFrmHeapHd->headAllocator;

        // ���ۑ��̈�m��
        NNSiFndFrmHeapState* pState = AllocFromHead(pFrmHeapHd, sizeof(NNSiFndFrmHeapState), MIN_ALIGNMENT);
        if (! pState)
        {
            return FALSE;
        }

        // ���݂̏�Ԃ��i�[
        pState->tagName       = tagName;
        pState->headAllocator = oldHeadAllocator;
        pState->tailAllocator = pFrmHeapHd->tailAllocator;
        pState->pPrevState    = pFrmHeapHd->pState;

        pFrmHeapHd->pState = pState;

        return TRUE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndFreeByStateToFrmHeap

  Description:  �t���[���q�[�v�̃������u���b�N���L�^���ꂽ��Ԃɏ]���ĕԋp���܂��B
                �w�肵���^�O����p����NNS_FndRecordStateForFrmHeap()���Ăяo�����O
                �̃������̎g�p�󋵂ɖ߂�܂��B
                �^�O����0���w�肷��ƍŌ��NNS_FndRecordStateForFrmHeap()���Ăяo��
                ���O�̏�ԂɂȂ�܂��B

                �^�O�����w�肵�ĕԋp�����ꍇ�A����Ȍ�ɌĂяo���ꂽ
                NNS_FndRecordStateForFrmHeap()�ɂ���ċL�^���ꂽ����
                �����Ȃ�܂��B

  Arguments:    heap:     �t���[���q�[�v�̃n���h���B
                tagName:  ��ԋL�^�ɕt����^�O���B

  Returns:      �֐������������ꍇ�ATRUE���Ԃ�܂��B
                ���s������AFALSE���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_FndFreeByStateToFrmHeap(
    NNSFndHeapHandle    heap,
    u32                 tagName
)
{
    NNS_ASSERT(IsValidFrmHeapHandle(heap));

    {
        NNSiFndFrmHeapHead* pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(heap);
        NNSiFndFrmHeapState* pState = pFrmHeapHd->pState;

        if (tagName != 0)   // �^�O���̎w�肠��
        {
            for(; pState; pState = pState->pPrevState)
            {
                if(pState->tagName == tagName)
                    break;
            }
        }

        if (! pState)
        {
            return FALSE;
        }

        pFrmHeapHd->headAllocator = pState->headAllocator;
        pFrmHeapHd->tailAllocator = pState->tailAllocator;

        pFrmHeapHd->pState = pState->pPrevState;

        return TRUE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndAdjustFrmHeap

  Description:  �t���[���q�[�v�̋󂫗̈���q�[�v�̈悩��J�����A���̕��q�[�v�̈��
                �k�����܂��B
                �q�[�v�������̌�납��m�ۂ��ꂽ�������u���b�N�����݂��Ă��Ă�
                �����܂���B

  Arguments:    heap:     �t���[���q�[�v�̃n���h���B

  Returns:      �֐������������ꍇ�A�k����̃t���[���q�[�v�̃T�C�Y��Ԃ��܂�
                (�o�C�g�P��)�B
                ���s�����ꍇ�A0��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u32
NNS_FndAdjustFrmHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidFrmHeapHandle(heap));

    {
        NNSiFndHeapHead* pHeapHd = heap;
        NNSiFndFrmHeapHead* pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(pHeapHd);

        // ����Ɋm�ۂ��ꂽ�������u���b�N�����݂���ꍇ�͎��s����
        if(0 < GetOffsetFromPtr(pFrmHeapHd->tailAllocator, pHeapHd->heapEnd))
        {
            return 0;
        }

        pFrmHeapHd->tailAllocator = pHeapHd->heapEnd = pFrmHeapHd->headAllocator;

        return GetOffsetFromPtr(heap, pHeapHd->heapEnd);
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndResizeForMBlockFrmHeap

  Description:  �t���[���q�[�v����m�ۂ��ꂽ�������u���b�N�̃T�C�Y��ύX���܂��B

                �T�C�Y��ύX���郁�����u���b�N�́A�q�[�v�̋󂫗̈�̑O������m�ۂ��ꂽ
                �����̃������u���b�N�ł���K�v������܂��B

  Arguments:    heap:      �t���[���q�[�v�̃n���h���B
                memBlock:  �T�C�Y��ύX���郁�����u���b�N�ւ̃|�C���^�B
                newSize:   �V�������蓖�Ă�T�C�Y(�o�C�g�P��)�B
                           4�����̒l���w�肳�ꂽ�ꍇ�́A4���w�肳�ꂽ���̂Ƃ��ď������܂��B

  Returns:      �֐������������ꍇ�A�ύX���ꂽ�������u���b�N�̃T�C�Y��Ԃ��܂�(�o�C�g�P��)�B
                �֐������s�����ꍇ�A0 ���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
u32
NNS_FndResizeForMBlockFrmHeap(
    NNSFndHeapHandle    heap,
    void*               memBlock,
    u32                 newSize
)
{
    NNSiFndHeapHead* pHeapHd = NULL;
    NNSiFndFrmHeapHead* pFrmHeapHd = NULL;

    NNS_ASSERT(IsValidFrmHeapHandle(heap));
    NNS_ASSERT(memBlock == NNSi_FndRoundDownPtr(memBlock, MIN_ALIGNMENT));  // �Œ���A�ŏ��A���C�����g�̋��E�ɂ��邩�`�F�b�N

    pHeapHd = heap;
    pFrmHeapHd = GetFrmHeapHeadPtrFromHeapHead(pHeapHd);

    NNS_ASSERT(
            ComparePtr(pHeapHd->heapStart, memBlock) <= 0
        &&  ComparePtr(pFrmHeapHd->headAllocator, memBlock) > 0);           // �������u���b�N�͑O���ɑ��݂��邱��
    NNS_ASSERT(
            pFrmHeapHd->pState == NULL
        ||  ComparePtr(pFrmHeapHd->pState, memBlock) < 0);                  // ��ԕۑ����������u���b�N�̌���ɖ�������

    /*
        newSize��0���邱�Ƃ͔F�߂Ȃ��悤�ɂ��Ă���B
        0�ɂ��Ă��܂��ƁAmemBlock���w���������u���b�N�����݂��Ȃ��Ȃ邽�߁B
    */
    if (newSize == 0)
    {
        newSize = 1;
    }
    newSize = NNSi_FndRoundUp(newSize, MIN_ALIGNMENT);

    {
        const u32 oldSize = GetOffsetFromPtr(memBlock, pFrmHeapHd->headAllocator);
        void* endAddress = AddU32ToPtr(memBlock, newSize);

        if (newSize == oldSize)  // �u���b�N�T�C�Y�ύX�Ȃ��̏ꍇ
        {
            return newSize;
        }

        if (newSize > oldSize)  // �g�傷��Ƃ�
        {
            if (ComparePtr(endAddress, pFrmHeapHd->tailAllocator) > 0)  // �T�C�Y������Ȃ��ꍇ
            {
                return 0;
            }

            FillAllocMemory(heap, pFrmHeapHd->headAllocator, newSize - oldSize);
        }
        else                    // �k������Ƃ�
        {
            FillFreeMemory(heap, endAddress, oldSize - newSize);
        }

        pFrmHeapHd->headAllocator = endAddress;

        return newSize;
    }
}

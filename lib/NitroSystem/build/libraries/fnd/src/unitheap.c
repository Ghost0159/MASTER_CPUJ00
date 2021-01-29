/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - fnd
  File:     unitheap.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.17 $
 *---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <nitro.h>
#include <nnsys/misc.h>
#include <nnsys/fnd/unitheap.h>
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

/* ------------------------------------------------------------------------
    �������u���b�N���X�g����
   ------------------------------------------------------------------------ */

static NNSiFndUntHeapMBlockHead*
PopMBlock(NNSiFndUntMBlockList* list)
{
    NNSiFndUntHeapMBlockHead* block = list->head;
    if (block)
    {
        list->head = block->pMBlkHdNext;
    }

    return block;
}

/*---------------------------------------------------------------------------*
  Name:         PushMBlock

  Description:  �������u���b�N�����X�g�̐擪�ɒǉ����܂��B

  Arguments:    link:   �ǉ����郊�X�g
                block:  �ǉ����郁�����u���b�N

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE void
PushMBlock(
    NNSiFndUntMBlockList*       list,
    NNSiFndUntHeapMBlockHead*   block
)
{
    block->pMBlkHdNext = list->head;
    list->head = block;
}


/*---------------------------------------------------------------------------*
  Name:         GetUnitHeapHeadPtrFromHeapHead

  Description:  �q�[�v�w�b�_�ւ̃|�C���^����A���j�b�g�q�[�v�w�b�_�ւ̃|�C���^���擾���܂��B

  Arguments:    pHeapHd:  �q�[�v�w�b�_�ւ̃|�C���^�B

  Returns:      ���j�b�g�q�[�v�w�b�_�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE NNSiFndUntHeapHead*
GetUnitHeapHeadPtrFromHeapHead(NNSiFndHeapHead* pHeapHd)
{
    return AddU32ToPtr(pHeapHd, sizeof(NNSiFndHeapHead));
}

static NNS_FND_INLINE BOOL
IsValidUnitHeapHandle(NNSFndHeapHandle handle)
{
    if(handle == NNS_FND_HEAP_INVALID_HANDLE)
    {
        return FALSE;
    }

    {
        NNSiFndHeapHead* pHeapHd = handle;
        return pHeapHd->signature == NNSI_UNTHEAP_SIGNATURE;
    }
}


/* ========================================================================
    �O���֐�(����J)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndDumpUnitHeap

  Description:  ���j�b�g�q�[�v�����̏���\�����܂��B
                ����̓f�o�b�O�p�̊֐��ł��B

  Arguments:    heap:    ���j�b�g�q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    void
    NNSi_FndDumpUnitHeap(NNSFndHeapHandle heap)
    {
        NNS_ASSERT(IsValidUnitHeapHandle(heap));

        {
            NNSiFndHeapHead *const pHeapHd = heap;
            NNSiFndUntHeapHead *const pUnitHeapHd = GetUnitHeapHeadPtrFromHeapHead(pHeapHd);
            const u32 heapSize = GetOffsetFromPtr(pHeapHd->heapStart, pHeapHd->heapEnd);

        	const u32 freeSize = NNS_FndCountFreeBlockForUnitHeap(heap) * pUnitHeapHd->mBlkSize;
        	const u32 usedSize = heapSize - freeSize;

            NNSi_FndDumpHeapHead(pHeapHd);

        	OS_Printf( "    %d / %d bytes (%6.2f%%) used\n",
        											usedSize, heapSize, 100.0f * usedSize / heapSize);
        }
    }

// #if ! defined(NNS_FINALROM)
#endif


/* ========================================================================
    �O���֐�(���J)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNS_FndCreateUnitHeapEx

  Description:  ���j�b�g�q�[�v���쐬���܂��B

  Arguments:    startAddress:  �q�[�v�̈�̐擪�A�h���X�B
                heapSize:      �q�[�v�̈�̃T�C�Y�B
                memBlockSize:  �������u���b�N�̃T�C�Y�B
                alignment:     �������u���b�N�̃A���C�����g�B
                               4,8,16,32�̂����ꂩ�̒l���w��ł��܂��B
                optFlag:       �I�v�V�����t���O�B

  Returns:      �֐������������ꍇ�A�쐬���ꂽ���j�b�g�q�[�v�̃n���h�����Ԃ�܂��B
                �֐������s����ƁANNS_FND_INVALID_HEAP_HANDLE ���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
NNSFndHeapHandle
NNS_FndCreateUnitHeapEx(
    void*   startAddress,
    u32     heapSize,
    u32     memBlockSize,
    int     alignment,
    u16     optFlag
)
{
    NNSiFndHeapHead* pHeapHd;
    void* heapEnd;

    SDK_NULL_ASSERT(startAddress);

    // alignment �̃`�F�b�N
    NNS_ASSERT(alignment % MIN_ALIGNMENT == 0);
    NNS_ASSERT(MIN_ALIGNMENT <= alignment && alignment <= 32);

    pHeapHd = NNSi_FndRoundUpPtr(startAddress, MIN_ALIGNMENT);
    heapEnd = NNSi_FndRoundDownPtr(AddU32ToPtr(startAddress, heapSize), MIN_ALIGNMENT);

    if (ComparePtr(pHeapHd, heapEnd) > 0)
    {
        return NNS_FND_HEAP_INVALID_HANDLE;
    }

    memBlockSize = NNSi_FndRoundUp(memBlockSize, alignment);    // �����̃u���b�N�T�C�Y

    {
        NNSiFndUntHeapHead* pUntHeapHd = GetUnitHeapHeadPtrFromHeapHead(pHeapHd);
        void* heapStart = NNSi_FndRoundUpPtr(AddU32ToPtr(pUntHeapHd, sizeof(NNSiFndUntHeapHead)), alignment);
        u32 elementNum;

        if (ComparePtr(heapStart, heapEnd) > 0)
        {
            return NNS_FND_HEAP_INVALID_HANDLE;
        }

        elementNum = GetOffsetFromPtr(heapStart, heapEnd) / memBlockSize;
        if (elementNum == 0)
        {
            return NNS_FND_HEAP_INVALID_HANDLE;
        }

        heapEnd = AddU32ToPtr(heapStart, elementNum * memBlockSize);

        NNSi_FndInitHeapHead(           // �q�[�v���ʏ�����
            pHeapHd,
            NNSI_UNTHEAP_SIGNATURE,
            heapStart,
            heapEnd,
            optFlag);

        pUntHeapHd->mbFreeList.head = heapStart;
        pUntHeapHd->mBlkSize = memBlockSize;

        {
            NNSiFndUntHeapMBlockHead* pMBlkHd = pUntHeapHd->mbFreeList.head;
            int i;

            for (i = 0; i < elementNum - 1; ++i, pMBlkHd = pMBlkHd->pMBlkHdNext)
            {
                pMBlkHd->pMBlkHdNext = AddU32ToPtr(pMBlkHd, memBlockSize);
            }

            pMBlkHd->pMBlkHdNext = NULL;
        }

        return pHeapHd;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndDestroyUnitHeap

  Description:  ���j�b�g�q�[�v��j�����܂��B

  Arguments:    heap: ���j�b�g�q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndDestroyUnitHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidUnitHeapHandle(heap));

    NNSi_FndFinalizeHeap(heap);
}


/*---------------------------------------------------------------------------*
  Name:         NNS_FndAllocFromUnitHeap

  Description:  ���j�b�g�q�[�v���烁�����u���b�N���m�ۂ��܂��B

  Arguments:    heap:   ���j�b�g�q�[�v�̃n���h���B

  Returns:      �������u���b�N�̊m�ۂ����������ꍇ�A�m�ۂ����������u���b�N�ւ�
                �|�C���^���Ԃ�܂��B
                ���s�����ꍇ�ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
void*
NNS_FndAllocFromUnitHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidUnitHeapHandle(heap));

    {
        NNSiFndUntHeapHead* pUntHeapHd = GetUnitHeapHeadPtrFromHeapHead(heap);
        NNSiFndUntHeapMBlockHead* pMBlkHd = PopMBlock(&pUntHeapHd->mbFreeList);

        if (pMBlkHd)
        {
            FillAllocMemory(heap, pMBlkHd, pUntHeapHd->mBlkSize);
        }

        return pMBlkHd;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndFreeToUnitHeap

  Description:  ���j�b�g�q�[�v�փ������u���b�N��ԋp���܂��B

  Arguments:    heap:     ���j�b�g�q�[�v�̃n���h���B
                memBlock: �ԋp���郁�����u���b�N�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndFreeToUnitHeap(
    NNSFndHeapHandle    heap,
    void*               memBlock
)
{
    NNS_ASSERT(IsValidUnitHeapHandle(heap));

    {
        NNSiFndUntHeapHead* pUntHeapHd = GetUnitHeapHeadPtrFromHeapHead(heap);

        FillFreeMemory(heap, memBlock, pUntHeapHd->mBlkSize);

        PushMBlock(&pUntHeapHd->mbFreeList, memBlock);
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndCountFreeBlockForUnitHeap

  Description:  ���j�b�g�q�[�v�̋󂫃������u���b�N�����擾���܂��B

  Arguments:    heap:     ���j�b�g�q�[�v�̃n���h���B

  Returns:      ���j�b�g�q�[�v�̋󂫃������u���b�N����Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u32
NNS_FndCountFreeBlockForUnitHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidUnitHeapHandle(heap));

    {
        NNSiFndUntHeapHead* pUntHeapHd = GetUnitHeapHeadPtrFromHeapHead(heap);
        NNSiFndUntHeapMBlockHead* pMBlkHd = pUntHeapHd->mbFreeList.head;
        u32 cnt = 0;

        for (; pMBlkHd; pMBlkHd = pMBlkHd->pMBlkHdNext)
        {
            ++cnt;
        }

        return cnt;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndCalcHeapSizeForUnitHeap

  Description:  �������u���b�N�̃T�C�Y�ƌ�����K�v�ȃq�[�v�̃T�C�Y���擾���܂��B

  Arguments:    memBlockSize:  �������u���b�N�̃T�C�Y(�o�C�g�l)�B
                memBlockNum:   �m�ۂ��郁�����u���b�N�̑����B
                alignment:     �������u���b�N�̃A���C�����g�B

  Returns:      �K�v�ȃq�[�v�̃T�C�Y��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u32
NNS_FndCalcHeapSizeForUnitHeap(
    u32     memBlockSize,
    u32     memBlockNum,
    int     alignment
)
{
    return
          // �q�[�v�������Ŏg�p����T�C�Y
          sizeof(NNSiFndHeapHead) + sizeof(NNSiFndUntHeapHead)

          // �A���C�����g�̒����ɕK�v�ȃT�C�Y�̍ő�
        + (alignment - 4)

          // �S���j�b�g���K�v�Ƃ���T�C�Y
        + memBlockNum * NNSi_FndRoundUp(memBlockSize, alignment);
}

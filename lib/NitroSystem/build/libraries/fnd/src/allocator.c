/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - fnd
  File:     allocator.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nnsys/misc.h>
#include <nnsys/fnd/expheap.h>
#include <nnsys/fnd/frameheap.h>
#include <nnsys/fnd/unitheap.h>
#include <nnsys/fnd/allocator.h>

/* ========================================================================
    static�֐�
   ======================================================================== */

/* ------------------------------------------------------------------------
    Exp Heap �p
   ------------------------------------------------------------------------ */

static void*
AllocatorAllocForExpHeap(
    NNSFndAllocator*    pAllocator,
    u32                 size
)
{
    NNSFndHeapHandle const heap = pAllocator->pHeap;
    int const alignment = (int)pAllocator->heapParam1;
    return NNS_FndAllocFromExpHeapEx(heap, size, alignment);
}

static void
AllocatorFreeForExpHeap(
    NNSFndAllocator*    pAllocator,
    void*               memBlock
)
{
    NNSFndHeapHandle const heap = pAllocator->pHeap;
    NNS_FndFreeToExpHeap(heap, memBlock);
}

/* ------------------------------------------------------------------------
    Frame Heap �p
   ------------------------------------------------------------------------ */

static void*
AllocatorAllocForFrmHeap(
    NNSFndAllocator*    pAllocator,
    u32                 size
)
{
    NNSFndHeapHandle const heap = pAllocator->pHeap;
    int const alignment = (int)pAllocator->heapParam1;
    return NNS_FndAllocFromFrmHeapEx(heap, size, alignment);
}

/*
    �t���[���q�[�v�ł̓������u���b�N�P�ʂ̃�����������ł��Ȃ����߁A
    ���̎����ł͉������Ȃ����Ƃɂ���B
*/
static void
AllocatorFreeForFrmHeap(
    NNSFndAllocator*    /*pAllocator*/,
    void*               /*memBlock*/
)
{
}


/* ------------------------------------------------------------------------
    Unit Heap �p
   ------------------------------------------------------------------------ */

/*
    ���j�b�g�q�[�v�̃������u���b�N�T�C�Y�𒴂���T�C�Y�̊��蓖�Ă͂ł��Ȃ��̂�
    NULL��Ԃ��B
*/
static void*
AllocatorAllocForUnitHeap(
    NNSFndAllocator*    pAllocator,
    u32                 size
)
{
    NNSFndHeapHandle const heap = pAllocator->pHeap;

    if (size > NNS_FndGetMemBlockSizeForUnitHeap(heap))
    {
        return NULL;
    }

    return NNS_FndAllocFromUnitHeap(heap);
}

static void
AllocatorFreeForUnitHeap(
    NNSFndAllocator*    pAllocator,
    void*               memBlock
)
{
    NNSFndHeapHandle const heap = pAllocator->pHeap;
    NNS_FndFreeToUnitHeap(heap, memBlock);
}


/* ------------------------------------------------------------------------
    SDK heap �p
   ------------------------------------------------------------------------ */

static void*
AllocatorAllocForSDKHeap(
    NNSFndAllocator*    pAllocator,
    u32                 size
)
{
    OSHeapHandle const heap = (int)pAllocator->pHeap;
    OSArenaId const id = (OSArenaId)pAllocator->heapParam1;
    return OS_AllocFromHeap(id, heap, size);
}

static void
AllocatorFreeForSDKHeap(
    NNSFndAllocator*    pAllocator,
    void*               memBlock
)
{
    OSHeapHandle const heap = (int)pAllocator->pHeap;
    OSArenaId const id = (OSArenaId)pAllocator->heapParam1;
    OS_FreeToHeap(id, heap, memBlock);
}



/* ========================================================================
    �O���֐�
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNS_FndAllocFromAllocator

  Description:  �A���P�[�^���烁�����u���b�N�m�ۂ��܂��B

                ���ۂɂ́A�ǂ̂悤�Ɋm�ۂ���邩�ǂ����̓A���P�[�^��
                ����Ɋ֘A�t�����Ă��郁�����}�l�[�W���̎����ɂ��܂��B

  Arguments:    pAllocator:  NNSFndAllocator�\���̂̃A�h���X�B
                size:        �������u���b�N�̃T�C�Y(�o�C�g)�B

  Returns:      �������u���b�N���m�ۂł����ꍇ�A���̃������u���b�N�̐擪�A�h���X��Ԃ��܂��B
                �m�ۂł��Ȃ������ꍇ�ANULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void*
NNS_FndAllocFromAllocator(
    NNSFndAllocator*    pAllocator,
    u32                 size
)
{
    NNS_ASSERT(pAllocator);
    return (*pAllocator->pFunc->pfAlloc)(pAllocator, size);
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndFreeToAllocator

  Description:  �������u���b�N��������A���P�[�^�֕ԋp���܂��B

                ���ۂɂ́A�ǂ̂悤�ɕԋp����邩�ǂ����̓A���P�[�^��
                ����Ɋ֘A�t�����Ă��郁�����}�l�[�W���̎����ɂ��܂��B

  Arguments:    pAllocator:  NNSFndAllocator�\���̂̃A�h���X�B
                memBlock:    ������郁�����u���b�N�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndFreeToAllocator(
    NNSFndAllocator*    pAllocator,
    void*               memBlock
)
{
    NNS_ASSERT(pAllocator && memBlock);
    (*pAllocator->pFunc->pfFree)(pAllocator, memBlock);
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndInitAllocatorForExpHeap

  Description:  �g���q�[�v���烁�����̊m�ۂƉ�����s���悤�ɃA���P�[�^�����������܂��B
                �A���P�[�^��ʂ��Ċm�ۂ����S�Ẵ������u���b�N�̃A���C�����g�l��
                ����alignment�Ŏw�肵���l�ɂȂ�܂��B

  Arguments:    pAllocator:  NNSFndAllocator�\���̂̃A�h���X�B
                heap:        �g���q�[�v�̃n���h���B
                alignment:   �m�ۂ���e�������u���b�N�ɓK�p����A���C�����g�l

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndInitAllocatorForExpHeap(
    NNSFndAllocator*    pAllocator,
    NNSFndHeapHandle    heap,
    int                 alignment
)
{
    static const NNSFndAllocatorFunc sAllocatorFunc =
    {
        AllocatorAllocForExpHeap,
        AllocatorFreeForExpHeap,
    };

    pAllocator->pFunc = &sAllocatorFunc;
    pAllocator->pHeap = heap;
    pAllocator->heapParam1 = (u32)alignment;
    pAllocator->heapParam2 = 0; // no use
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndInitAllocatorForFrmHeap

  Description:  �t���[���q�[�v���烁�����̊m�ۂƉ�����s���悤�ɃA���P�[�^�����������܂��B
                �A���P�[�^��ʂ��Ċm�ۂ����S�Ẵ������u���b�N�̃A���C�����g�l��
                ����alignment�Ŏw�肵���l�ɂȂ�܂��B

  Arguments:    pAllocator:  NNSFndAllocator�\���̂̃A�h���X�B
                heap:        �t���[���q�[�v�̃n���h���B
                alignment:   �m�ۂ���e�������u���b�N�ɓK�p����A���C�����g�l

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndInitAllocatorForFrmHeap(
    NNSFndAllocator*    pAllocator,
    NNSFndHeapHandle    heap,
    int                 alignment
)
{
    static const NNSFndAllocatorFunc sAllocatorFunc =
    {
        AllocatorAllocForFrmHeap,
        AllocatorFreeForFrmHeap,
    };

    pAllocator->pFunc = &sAllocatorFunc;
    pAllocator->pHeap = heap;
    pAllocator->heapParam1 = (u32)alignment;
    pAllocator->heapParam2 = 0; // no use
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndInitAllocatorForUnitHeap

  Description:  ���j�b�g�q�[�v���烁�����̊m�ۂƉ�����s���悤�ɃA���P�[�^�����������܂��B
                ���j�b�g�q�[�v�̃������u���b�N�T�C�Y���傫�ȃ������u���b�N���m�ۂ��邱�Ƃ͏o���܂���B
                ���̏ꍇ�A�֐�NNS_FndAllocFromAllocator() ��NULL��Ԃ��܂��B

  Arguments:    pAllocator:  NNSFndAllocator�\���̂̃A�h���X�B
                heap:        ���j�b�g�q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndInitAllocatorForUnitHeap(
    NNSFndAllocator*    pAllocator,
    NNSFndHeapHandle    heap
)
{
    static const NNSFndAllocatorFunc sAllocatorFunc =
    {
        AllocatorAllocForUnitHeap,
        AllocatorFreeForUnitHeap,
    };

    pAllocator->pFunc = &sAllocatorFunc;
    pAllocator->pHeap = heap;
    pAllocator->heapParam1 = 0; // no use
    pAllocator->heapParam2 = 0; // no use
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndInitAllocatorForSDKHeap

  Description:  NITRO-SDK��OS_CreateHeap()�֐��ō쐬�����q�[�v����
                �������̊m�ۂƉ�����s���悤�ɃA���P�[�^�����������܂��B

  Arguments:    pAllocator:  NNSFndAllocator�\���̂̃A�h���X�B
                id:          �q�[�v�̂���A���[�i�̃A���[�iID�B
                heap:        �q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndInitAllocatorForSDKHeap(
    NNSFndAllocator*    pAllocator,
    OSArenaId           id,
    OSHeapHandle        heap
)
{
    static const NNSFndAllocatorFunc sAllocatorFunc =
    {
        AllocatorAllocForSDKHeap,
        AllocatorFreeForSDKHeap,
    };

    pAllocator->pFunc = &sAllocatorFunc;
    pAllocator->pHeap = (void*)heap;
    pAllocator->heapParam1 = id;
    pAllocator->heapParam2 = 0; // no use
}


/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - fnd
  File:     unitheap.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_FND_UNITHEAP_H_
#define NNS_FND_UNITHEAP_H_

#include <nnsys/fnd/heapcommon.h>

#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================
    �^��`
   ======================================================================== */

typedef struct NNSiFndUntHeapMBlockHead NNSiFndUntHeapMBlockHead;

// �������u���b�N�̃w�b�_���
struct NNSiFndUntHeapMBlockHead
{
    NNSiFndUntHeapMBlockHead*  pMBlkHdNext;    // ���u���b�N
};


typedef struct NNSiFndUntMBlockList NNSiFndUntMBlockList;

// �������u���b�N�̃��X�g
struct NNSiFndUntMBlockList
{
    NNSiFndUntHeapMBlockHead*  head;           // �擪�Ɍq����Ă��郁�����u���b�N�ւ̃|�C���^
};


typedef struct NNSiFndUntHeapHead NNSiFndUntHeapHead;

// ���j�b�g�q�[�v�̃w�b�_���
struct NNSiFndUntHeapHead
{
    NNSiFndUntMBlockList    mbFreeList;     // �t���[���X�g
    u32                     mBlkSize;       // �������u���b�N�T�C�Y
};


/* ========================================================================
    �}�N���֐�
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNS_FndCreateUnitHeap

  Description:  ���j�b�g�q�[�v���쐬���܂��B

  Arguments:    startAddress:  �q�[�v�̈�̐擪�A�h���X�B
                heapSize:      �q�[�v�̈�̃T�C�Y�B
                memBlockSize:  �������u���b�N�̃T�C�Y�B

  Returns:      �֐������������ꍇ�A�쐬���ꂽ���j�b�g�q�[�v�̃n���h�����Ԃ�܂��B
                �֐������s����ƁANNS_FND_INVALID_HEAP_HANDLE ���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
#define                 NNS_FndCreateUnitHeap(startAddress, heapSize, memBlockSize) \
                            NNS_FndCreateUnitHeapEx(startAddress, heapSize, memBlockSize, NNS_FND_HEAP_DEFAULT_ALIGNMENT, 0)


/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetMemBlockSizeForUnitHeap

  Description:  ���j�b�g�q�[�v�̃������u���b�N�T�C�Y���擾���܂��B

  Arguments:    heap:  ���j�b�g�q�[�v�̃n���h���B

  Returns:      ���j�b�g�q�[�v�̃������u���b�N�T�C�Y��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#define                 NNS_FndGetMemBlockSizeForUnitHeap(heap) \
                            (((const NNSiFndUntHeapHead*)((const u8*)((const void*)(heap)) + sizeof(NNSiFndHeapHead)))->mBlkSize)


/* ========================================================================
    �֐��v���g�^�C�v
   ======================================================================== */

#if ! defined(NNS_FINALROM)

    void                    NNSi_FndDumpUnitHeap(
                                NNSFndHeapHandle    heap);

// #if ! defined(NNS_FINALROM)
#endif

NNSFndHeapHandle        NNS_FndCreateUnitHeapEx(
                            void*   startAddress,
                            u32     heapSize,
                            u32     memBlockSize,
                            int     alignment,
                            u16     optFlag);

void                    NNS_FndDestroyUnitHeap(
                            NNSFndHeapHandle    heap);

void*                   NNS_FndAllocFromUnitHeap(
                        	NNSFndHeapHandle    heap);

void                    NNS_FndFreeToUnitHeap(
                            NNSFndHeapHandle    heap,
                            void*               memBlock);

u32                     NNS_FndCountFreeBlockForUnitHeap(
                            NNSFndHeapHandle    heap);


u32                     NNS_FndCalcHeapSizeForUnitHeap(
                            u32     memBlockSize,
                            u32     memBlockNum,
                            int     alignment);


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_FND_UNITHEAP_H_ */
#endif

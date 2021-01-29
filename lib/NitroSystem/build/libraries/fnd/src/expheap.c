/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - fnd
  File:     expheap.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.28 $
 *---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <nitro.h>
#include <nnsys/misc.h>
#include <nnsys/fnd/expheap.h>
#include <nnsys/fnd/config.h>
#include "heapcommoni.h"


/* ========================================================================
    �}�N���萔
   ======================================================================== */

// �t���[�������u���b�N�̃V�O�l�`��
#define MBLOCK_FREE_SIGNATURE   ('FR')

// �g�p�������u���b�N�̃V�O�l�`��
#define MBLOCK_USED_SIGNATURE   ('UD')

// �O���[�vID�̍ő�l
#define MAX_GROUPID             0xff

// �O���[�vID�̃f�t�H���g�l
#define DEFAULT_GROUPID         0

// �A���C�����g�̍ŏ��l
#define MIN_ALIGNMENT           4

// �f�t�H���g�̃������m�ۃ��[�h
#define DEFAULT_ALLOC_MODE      NNS_FND_EXPHEAP_ALLOC_MODE_FIRST

// �t���[�u���b�N�Ƃ��ēo�^����ŏ��T�C�Y (�w�b�_�͊܂܂Ȃ��T�C�Y)
#define MIN_FREE_BLOCK_SIZE     4
// #define MIN_FREE_BLOCK_SIZE 16


/* ========================================================================
    �\���̒�`
   ======================================================================== */
typedef struct NNSiMemRegion NNSiMemRegion;

struct NNSiMemRegion
{
    void*       start;
    void*       end;
};


/* ========================================================================
    �}�N���֐�
   ======================================================================== */

#if ! defined(NNS_FINALROM)

    // �q�[�v�̌������̌x���o�͗p
    #define HEAP_WARNING(exp, ...) \
        (void) ((exp) && (OS_Printf(__VA_ARGS__), 0))

// #if ! defined(NNS_FINALROM)
#endif


/* ========================================================================
    static�֐�
   ======================================================================== */

/* ------------------------------------------------------------------------
    �|�C���^����
   ------------------------------------------------------------------------ */

static NNS_FND_INLINE void*
MaxPtr(void* a, void* b)
{
    return NNSiGetUIntPtr(a) > NNSiGetUIntPtr(b) ? a: b;
}

static NNS_FND_INLINE BOOL
IsValidExpHeapHandle(NNSFndHeapHandle handle)
{
    if (handle == NNS_FND_HEAP_INVALID_HANDLE)
    {
        return FALSE;
    }

    {
        NNSiFndHeapHead* pHeapHd = handle;
        return pHeapHd->signature == NNSI_EXPHEAP_SIGNATURE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         GetExpHeapHeadPtrFromHeapHead

  Description:  �q�[�v�w�b�_�ւ̃|�C���^����A�g���q�[�v�w�b�_�ւ̃|�C���^���擾���܂��B

  Arguments:    pHHead:  �q�[�v�w�b�_�ւ̃|�C���^�B

  Returns:      �g���q�[�v�w�b�_�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE NNSiFndExpHeapHead*
GetExpHeapHeadPtrFromHeapHead(NNSiFndHeapHead* pHHead)
{
    return AddU32ToPtr(pHHead, sizeof(NNSiFndHeapHead));
}

/*---------------------------------------------------------------------------*
  Name:         GetHeapHeadPtrFromExpHeapHead

  Description:  �g���q�[�v�w�b�_�ւ̃|�C���^����A�q�[�v�w�b�_�ւ̃|�C���^���擾���܂��B

  Arguments:    pEHHead:  �g���q�[�v�w�b�_�ւ̃|�C���^�B

  Returns:      �q�[�v�w�b�_�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE NNSiFndHeapHead*
GetHeapHeadPtrFromExpHeapHead(NNSiFndExpHeapHead* pEHHead)
{
    return SubU32ToPtr(pEHHead, sizeof(NNSiFndHeapHead));
}

/*---------------------------------------------------------------------------*
  Name:         GetExpHeapHeadPtrFromHandle

  Description:  �g���q�[�v�n���h������A�g���q�[�v�w�b�_�ւ̃|�C���^���擾���܂��B

  Arguments:    heap:  �g���q�[�v�n���h���B

  Returns:      �g���q�[�v�w�b�_�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE NNSiFndExpHeapHead*
GetExpHeapHeadPtrFromHandle(NNSFndHeapHandle heap)
{
    return GetExpHeapHeadPtrFromHeapHead(heap);
}

/*---------------------------------------------------------------------------*
  Name:         GetMemPtrForMBlock

  Description:  NNSiFndExpHeapMBlockHead�\���̂ւ̃|�C���^����A
                �������u���b�N�ւ̃|�C���^���擾���܂��B

  Arguments:    pMBlkHd:  NNSiFndExpHeapMBlockHead�\���̂ւ̃|�C���^�B

  Returns:      �������u���b�N�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE void*
GetMemPtrForMBlock(NNSiFndExpHeapMBlockHead* pMBlkHd)
{
    return AddU32ToPtr(pMBlkHd, sizeof(NNSiFndExpHeapMBlockHead));
}

static NNS_FND_INLINE const void*
GetMemCPtrForMBlock(const NNSiFndExpHeapMBlockHead* pMBlkHd)
{
    return AddU32ToCPtr(pMBlkHd, sizeof(NNSiFndExpHeapMBlockHead));
}

/*---------------------------------------------------------------------------*
  Name:         GetMBlockHeadPtr

  Description:  �������u���b�N�ւ̃|�C���^����A
                NNSiFndExpHeapMBlockHead�\���̂ւ̃|�C���^���擾���܂��B
                �������u���b�N�ւ̃|�C���^���擾���܂��B

  Arguments:    memBlock:  �������u���b�N�ւ̃|�C���^�B

  Returns:      NNSiFndExpHeapMBlockHead�\���̂ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE NNSiFndExpHeapMBlockHead*
GetMBlockHeadPtr(void* memBlock)
{
    return SubU32ToPtr(memBlock, sizeof(NNSiFndExpHeapMBlockHead));
}

static NNS_FND_INLINE const NNSiFndExpHeapMBlockHead*
GetMBlockHeadCPtr(const void* memBlock)
{
    return SubU32ToCPtr(memBlock, sizeof(NNSiFndExpHeapMBlockHead));
}

static NNS_FND_INLINE void*
GetMBlockEndAddr(NNSiFndExpHeapMBlockHead* pMBHead)
{
    return AddU32ToPtr(GetMemPtrForMBlock(pMBHead), pMBHead->blockSize);
}

/* ------------------------------------------------------------------------
    NNSiFndExpHeapMBlockHead �\���̃A�N�Z�X�֐�
   ------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*
  Name:         GetAlignmentForMBlock

  Description:  NNSiFndExpHeapMBlockHead�\���̂̃A���C�����g�l���擾���܂��B

  Arguments:    pMBlkHd:  NNSiFndExpHeapMBlockHead�\���̂ւ̃|�C���^�B

  Returns:      NNSiFndExpHeapMBlockHead�\���̂̃A���C�����g�l��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE u16
GetAlignmentForMBlock(const NNSiFndExpHeapMBlockHead* pMBlkHd)
{
    return (u16)NNSi_FndGetBitValue(pMBlkHd->attribute, 8, 7);
}

/*---------------------------------------------------------------------------*
  Name:         SetAlignmentForMBlock

  Description:  NNSiFndExpHeapMBlockHead�\���̂̃A���C�����g�l���Z�b�g���܂��B

  Arguments:    pMBlkHd:    NNSiFndExpHeapMBlockHead�\���̂ւ̃|�C���^�B
                alignment:  �Z�b�g����A���C�����g�l

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE void
SetAlignmentForMBlock(
    NNSiFndExpHeapMBlockHead*   pMBlkHd,
    u16                         alignment
)
{
    NNSi_FndSetBitValue(pMBlkHd->attribute, 8, 7, alignment);
}

static NNS_FND_INLINE u16
GetGroupIDForMBlock(const NNSiFndExpHeapMBlockHead* pMBHead)
{
    return (u16)NNSi_FndGetBitValue(pMBHead->attribute, 0, 8);
}

static NNS_FND_INLINE void
SetGroupIDForMBlock(
    NNSiFndExpHeapMBlockHead*   pMBHead,
    u16                         id
)
{
    NNSi_FndSetBitValue(pMBHead->attribute, 0, 8, id);
}

static NNS_FND_INLINE u16
GetAllocDirForMBlock(const NNSiFndExpHeapMBlockHead* pMBHead)
{
    return (u16)NNSi_FndGetBitValue(pMBHead->attribute, 15, 1);
}

static NNS_FND_INLINE void
SetAllocDirForMBlock(
    NNSiFndExpHeapMBlockHead*   pMBHead,
    u16                         mode
)
{
    NNSi_FndSetBitValue(pMBHead->attribute, 15, 1, mode);
}


/* ------------------------------------------------------------------------
    NNSiFndExpHeapHead �\���̃A�N�Z�X�֐�
   ------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*
  Name:         GetAllocMode

  Description:  �g���q�[�v�̃������m�ۃ��[�h���擾���܂��B

  Arguments:    pEHHead:  �g���q�[�v�w�b�_�ւ̃|�C���^�B

  Returns:      �g���q�[�v�̃������m�ۃ��[�h��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE u16
GetAllocMode(NNSiFndExpHeapHead* pEHHead)
{
    return (u16)NNSi_FndGetBitValue(pEHHead->feature, 0, 1);
}

/*---------------------------------------------------------------------------*
  Name:         SetAllocMode

  Description:  �g���q�[�v�̃������m�ۃ��[�h���Z�b�g���܂��B

  Arguments:    pEHHead:  �g���q�[�v�w�b�_�ւ̃|�C���^�B
                mode:     �������m�ۃ��[�h�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE void
SetAllocMode(
    NNSiFndExpHeapHead* pEHHead,
    u16                 mode
)
{
    NNSi_FndSetBitValue(pEHHead->feature, 0, 1, mode);
}

static void
GetRegionOfMBlock(
    NNSiMemRegion*              region,
    NNSiFndExpHeapMBlockHead*   block
)
{
    region->start = SubU32ToPtr(block, GetAlignmentForMBlock(block));
    region->end   = GetMBlockEndAddr(block);
}


/* ------------------------------------------------------------------------
    �������u���b�N���X�g����
   ------------------------------------------------------------------------ */

static NNSiFndExpHeapMBlockHead*
RemoveMBlock(
    NNSiFndExpMBlockList*       list,
    NNSiFndExpHeapMBlockHead*   block
)
{
    NNSiFndExpHeapMBlockHead *const prev = block->pMBHeadPrev;
    NNSiFndExpHeapMBlockHead *const next = block->pMBHeadNext;

    // �O�Q�ƃ����N
    if (prev)
    {
        prev->pMBHeadNext = next;
    }
    else
    {
        list->head = next;
    }

    // ���Q�ƃ����N
    if (next)
    {
        next->pMBHeadPrev = prev;
    }
    else
    {
        list->tail = prev;
    }

    return prev;
}

static NNSiFndExpHeapMBlockHead*
InsertMBlock(
    NNSiFndExpMBlockList*       list,
    NNSiFndExpHeapMBlockHead*   target,
    NNSiFndExpHeapMBlockHead*   prev
)
{
    NNSiFndExpHeapMBlockHead* next;

    // �O�Q�ƃ����N
    target->pMBHeadPrev = prev;
    if (prev)
    {
        next = prev->pMBHeadNext;
        prev->pMBHeadNext = target;
    }
    else
    {
        next = list->head;
        list->head = target;
    }

    // ���Q�ƃ����N
    target->pMBHeadNext = next;
    if (next)
    {
        next->pMBHeadPrev = target;
    }
    else
    {
        list->tail = target;
    }

    return target;
}

/*---------------------------------------------------------------------------*
  Name:         AppendMBlock

  Description:  �������u���b�N�����X�g�̍Ō�ɒǉ����܂��B

  Arguments:    link:   �ǉ����郊�X�g
                block:  �ǉ����郁�����u���b�N

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE void
AppendMBlock(
    NNSiFndExpMBlockList*       list,
    NNSiFndExpHeapMBlockHead*   block
)
{
    (void)InsertMBlock(list, block, list->tail);
}

/*---------------------------------------------------------------------------*
  Name:         InitMBlock

  Description:  �������u���b�N�̏��������s���܂��B

  Arguments:    pRegion:    �������u���b�N�Ƃ��郊�[�W������\���\���̂ւ̃|�C���^�B
                signature:  �������u���b�N�̃V�O�l�`���B

  Returns:      �����������������u���b�N�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNSiFndExpHeapMBlockHead*
InitMBlock(
    const NNSiMemRegion*    pRegion,
    u16                     signature
)
{
    NNSiFndExpHeapMBlockHead* block = pRegion->start;

    block->signature = signature;
    block->attribute = 0;
    block->blockSize = GetOffsetFromPtr(GetMemPtrForMBlock(block), pRegion->end);
    block->pMBHeadPrev = NULL;
    block->pMBHeadNext = NULL;

    return block;
}

/*---------------------------------------------------------------------------*
  Name:         InitFreeMBlock

  Description:  �������u���b�N���t���[�u���b�N�p�ɏ��������s���܂��B

  Arguments:    pRegion:    �������u���b�N�Ƃ��郊�[�W������\���\���̂ւ̃|�C���^�B

  Returns:      �����������������u���b�N�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNS_FND_INLINE NNSiFndExpHeapMBlockHead*
InitFreeMBlock(
    const NNSiMemRegion*    pRegion
)
{
    return InitMBlock(pRegion, MBLOCK_FREE_SIGNATURE);
}

/*---------------------------------------------------------------------------*
  Name:         InitExpHeap

  Description:  �g���q�[�v�̏��������s���܂��B

  Arguments:    startAddress:  �g���q�[�v�Ƃ��郁�����̊J�n�A�h���X�B
                endAddress:    �g���q�[�v�Ƃ��郁�����̏I���A�h���X +1�B
                optFlag:       �I�v�V�����t���O�B

  Returns:      �q�[�v�w�b�_�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNSiFndHeapHead*
InitExpHeap(
    void*   startAddress,
    void*   endAddress,
    u16     optFlag
)
{
    NNSiFndHeapHead* pHeapHd = startAddress;
    NNSiFndExpHeapHead* pExpHeapHd = GetExpHeapHeadPtrFromHeapHead(pHeapHd);

    NNSi_FndInitHeapHead(           // �q�[�v���ʏ�����
        pHeapHd,
        NNSI_EXPHEAP_SIGNATURE,
        AddU32ToPtr(pExpHeapHd, sizeof(NNSiFndExpHeapHead)),    // heapStart
        endAddress,                                             // heapEnd
        optFlag);

    pExpHeapHd->groupID = DEFAULT_GROUPID;      // �O���[�vID
    pExpHeapHd->feature = 0;
    SetAllocMode(pExpHeapHd, DEFAULT_ALLOC_MODE);

    // �t���[�̃u���b�N�����
    {
        NNSiFndExpHeapMBlockHead* pMBHead;
        NNSiMemRegion region;
        region.start = pHeapHd->heapStart;
        region.end   = pHeapHd->heapEnd;
        pMBHead = InitFreeMBlock(&region);

        // �u���b�N���X�g
        pExpHeapHd->mbFreeList.head = pMBHead;
        pExpHeapHd->mbFreeList.tail = pMBHead;
        pExpHeapHd->mbUsedList.head = NULL;
        pExpHeapHd->mbUsedList.tail = NULL;

        return pHeapHd;
    }
}

/*---------------------------------------------------------------------------*
  Name:         AllocUsedBlockFromFreeBlock

  Description:  �t���[�u���b�N�̒�����V�����������u���b�N���m�ۂ��܂��B

  Arguments:    pEHHead:      �g���q�[�v�w�b�_�ւ̃|�C���^�B
                pMBHeadFree:  �t���[�u���b�N�w�b�_�ւ̃|�C���^�B
                mblock:       �m�ۂ��郁�����u���b�N�̃A�h���X�B
                size:         �m�ۂ��郁�����u���b�N�̃T�C�Y�B
                direction:    �m�ە����B

  Returns:      �m�ۂ����������u���b�N�̐擪�̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static void*
AllocUsedBlockFromFreeBlock(
    NNSiFndExpHeapHead*         pEHHead,
    NNSiFndExpHeapMBlockHead*   pMBHeadFree,
    void*                       mblock,
    u32                         size,
    u16                         direction
)
{
    NNSiMemRegion freeRgnT;
    NNSiMemRegion freeRgnB;
    NNSiFndExpHeapMBlockHead* pMBHeadFreePrev;

    GetRegionOfMBlock(&freeRgnT, pMBHeadFree);
    freeRgnB.end   = freeRgnT.end;
    freeRgnB.start = AddU32ToPtr(mblock, size);
    freeRgnT.end   = SubU32ToPtr(mblock, sizeof(NNSiFndExpHeapMBlockHead));

    pMBHeadFreePrev = RemoveMBlock(&pEHHead->mbFreeList, pMBHeadFree);  // ��U�t���[�u���b�N���폜

    // �t���[�u���b�N���]�T�������ꍇ
    if (GetOffsetFromPtr(freeRgnT.start, freeRgnT.end) < sizeof(NNSiFndExpHeapMBlockHead) + MIN_FREE_BLOCK_SIZE)
    {
        freeRgnT.end = freeRgnT.start;  // �g�p�u���b�N�̃A���C�����g�l�Ɋ܂߂�
    }
    else
    {
        pMBHeadFreePrev = InsertMBlock(&pEHHead->mbFreeList, InitFreeMBlock(&freeRgnT), pMBHeadFreePrev);
    }

    // �t���[�u���b�N���]�T�������ꍇ
    if (GetOffsetFromPtr(freeRgnB.start, freeRgnB.end) < sizeof(NNSiFndExpHeapMBlockHead) + MIN_FREE_BLOCK_SIZE)
    {
        freeRgnB.start= freeRgnB.end;   // �g�p�u���b�N�Ɋ܂߂�
    }
    else
    {
        (void)InsertMBlock(&pEHHead->mbFreeList, InitFreeMBlock(&freeRgnB), pMBHeadFreePrev);
    }

    // �f�o�O�p�������[�U
    FillAllocMemory(GetHeapHeadPtrFromExpHeapHead(pEHHead), freeRgnT.end, GetOffsetFromPtr(freeRgnT.end, freeRgnB.start));

    // �V�K�u���b�N�쐬
    {
        NNSiFndExpHeapMBlockHead* pMBHeadNewUsed;
        NNSiMemRegion region;

        region.start = SubU32ToPtr(mblock, sizeof(NNSiFndExpHeapMBlockHead));
        region.end   = freeRgnB.start;

        pMBHeadNewUsed = InitMBlock(&region, MBLOCK_USED_SIGNATURE);
        SetAllocDirForMBlock(pMBHeadNewUsed, direction);
        SetAlignmentForMBlock(pMBHeadNewUsed, (u16)GetOffsetFromPtr(freeRgnT.end, pMBHeadNewUsed));
        SetGroupIDForMBlock(pMBHeadNewUsed, pEHHead->groupID);
        AppendMBlock(&pEHHead->mbUsedList, pMBHeadNewUsed);
    }

    return mblock;
}

/*---------------------------------------------------------------------------*
  Name:         AllocFromHead

  Description:  �q�[�v�̐擪���烁�����u���b�N���m�ۂ��܂��B

  Arguments:    pHeapHd:    �q�[�v�w�b�_�ւ̃|�C���^�B
                size:       �m�ۂ��郁�����u���b�N�̃T�C�Y�B
                alignment:  �A���C�����g�l�B

  Returns:      �������u���b�N�̊m�ۂ����������ꍇ�A�m�ۂ����������u���b�N�ւ�
                �|�C���^���Ԃ�܂��B
                ���s�����ꍇ�ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
static void*
AllocFromHead(
    NNSiFndHeapHead*    pHeapHd,
    u32                 size,
    int                 alignment
)
{
    NNSiFndExpHeapHead* pExpHeapHd = GetExpHeapHeadPtrFromHeapHead(pHeapHd);

    // �ŏ��ɂ݂��������̂����蓖�Ă邩
    const BOOL bAllocFirst = GetAllocMode(pExpHeapHd) == NNS_FND_EXPHEAP_ALLOC_MODE_FIRST;

    NNSiFndExpHeapMBlockHead* pMBlkHd      = NULL;
    NNSiFndExpHeapMBlockHead* pMBlkHdFound = NULL;
    u32 foundSize = 0xffffffff;
    void* foundMBlock = NULL;

    // �t���[�u���b�N����
    for (pMBlkHd = pExpHeapHd->mbFreeList.head; pMBlkHd; pMBlkHd = pMBlkHd->pMBHeadNext)
    {
        void *const mblock    = GetMemPtrForMBlock(pMBlkHd);
        void *const reqMBlock = NNSi_FndRoundUpPtr(mblock, alignment);
        const u32 offset      = GetOffsetFromPtr(mblock, reqMBlock);    // ������������

        if ( pMBlkHd->blockSize >= size + offset
         &&  foundSize > pMBlkHd->blockSize )
        {
            pMBlkHdFound  = pMBlkHd;
            foundSize     = pMBlkHd->blockSize;
            foundMBlock   = reqMBlock;

            if (bAllocFirst || foundSize == size)
            {
                break;
            }
        }
    }

    if (! pMBlkHdFound) // �����ɍ����u���b�N��������Ȃ�
    {
        return NULL;
    }

    return AllocUsedBlockFromFreeBlock( // ���������t���[�u���b�N����̈�m��
            pExpHeapHd,
            pMBlkHdFound,
            foundMBlock,
            size,
            NNS_FND_EXPHEAP_ALLOC_DIR_FRONT);
}

/*---------------------------------------------------------------------------*
  Name:         AllocFromTail

  Description:  �q�[�v�̖������烁�����u���b�N���m�ۂ��܂��B

  Arguments:    pHeapHd:    �q�[�v�w�b�_�ւ̃|�C���^�B
                size:       �m�ۂ��郁�����u���b�N�̃T�C�Y�B
                alignment:  �A���C�����g�l�B

  Returns:      �������u���b�N�̊m�ۂ����������ꍇ�A�m�ۂ����������u���b�N�ւ�
                �|�C���^���Ԃ�܂��B
                ���s�����ꍇ�ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
static void*
AllocFromTail(
    NNSiFndHeapHead*    pHeapHd,
    u32                 size,
    int                 alignment
)
{
    NNSiFndExpHeapHead* pExpHeapHd = GetExpHeapHeadPtrFromHeapHead(pHeapHd);

    // �ŏ��ɂ݂��������̂����蓖�Ă邩
    const BOOL bAllocFirst = GetAllocMode(pExpHeapHd) == NNS_FND_EXPHEAP_ALLOC_MODE_FIRST;

    NNSiFndExpHeapMBlockHead* pMBlkHd      = NULL;
    NNSiFndExpHeapMBlockHead* pMBlkHdFound = NULL;
    u32 foundSize = 0xffffffff;
    void* foundMBlock = NULL;

    // �t���[�u���b�N����
    for (pMBlkHd = pExpHeapHd->mbFreeList.tail; pMBlkHd; pMBlkHd = pMBlkHd->pMBHeadPrev)
    {
        void *const mblock    = GetMemPtrForMBlock(pMBlkHd);
        void *const mblockEnd = AddU32ToPtr(mblock, pMBlkHd->blockSize);
        void *const reqMBlock = NNSi_FndRoundDownPtr(SubU32ToPtr(mblockEnd, size), alignment);  // �A���C���ς݂̃A�h���X

        if ( ComparePtr(reqMBlock, mblock) >= 0
         &&  foundSize > pMBlkHd->blockSize )
        {
            pMBlkHdFound = pMBlkHd;
            foundSize    = pMBlkHd->blockSize;
            foundMBlock  = reqMBlock;

            if (bAllocFirst || foundSize == size)
            {
                break;
            }
        }
    }

    if (! pMBlkHdFound) // �����ɍ����u���b�N��������Ȃ�
    {
        return NULL;
    }

    return AllocUsedBlockFromFreeBlock( // ���������t���[�u���b�N����̈�m��
            pExpHeapHd,
            pMBlkHdFound,
            foundMBlock,
            size,
            NNS_FND_EXPHEAP_ALLOC_DIR_REAR);
}

/*---------------------------------------------------------------------------*
  Name:         RecycleRegion

  Description:  �󂫃��[�W�������t���[�������u���b�N�֑g�ݓ���܂��B
                �t���[�u���b�N�Ɨאڂ��Ă���ꍇ�́A�t���[�u���b�N���g�����܂��B
                �t���[�u���b�N�Ɨאڂ��Ă��炸�A���t���[�u���b�N�Ƃ���قǂ�
                �T�C�Y�������ꍇ�́A����ɗאڂ���g�p�ς݃u���b�N�̃A���C�����g�l�Ƃ��܂��B
                ����ɗאڂ���g�p�ς݃u���b�N�������ꍇ�́A�֐��͎��s���܂��B

  Arguments:    pEHHead:  �g���q�[�v�w�b�_�ւ̃|�C���^�B
                pRegion:  �󂫃��[�W�����ւ̃|�C���^�B

  Returns:      �֐������������ TRUE ��Ԃ��܂��B
                ���s����� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL
RecycleRegion(
    NNSiFndExpHeapHead*     pEHHead,
    const NNSiMemRegion*    pRegion
)
{
    NNSiFndExpHeapMBlockHead* pBlkPrFree  = NULL;   // ���O�t���[�u���b�N
    NNSiMemRegion freeRgn = *pRegion;

    // �w��G���A�ɗאڂ����t���[�G���A������
    {
        NNSiFndExpHeapMBlockHead* pBlk;

        for (pBlk = pEHHead->mbFreeList.head; pBlk; pBlk = pBlk->pMBHeadNext)
        {
            if (pBlk < pRegion->start)
            {
                pBlkPrFree = pBlk;
                continue;
            }

            if (pBlk == pRegion->end)   // ����ɗאڂ���u���b�N��?
            {
                // �󂫃��[�W����������
                freeRgn.end = GetMBlockEndAddr(pBlk);
                (void)RemoveMBlock(&pEHHead->mbFreeList, pBlk);

                // �w�b�_����NoUse�Ŗ��߂�
                FillNoUseMemory(GetHeapHeadPtrFromExpHeapHead(pEHHead), pBlk, sizeof(NNSiFndExpHeapMBlockHead));
            }
            break;
        }
    }

    // ���O�̃t���[�u���b�N���O���ɗאڂ���u���b�N��?
    if (pBlkPrFree && GetMBlockEndAddr(pBlkPrFree) == pRegion->start)
    {
        // �󂫃��[�W����������
        freeRgn.start = pBlkPrFree;
        pBlkPrFree = RemoveMBlock(&pEHHead->mbFreeList, pBlkPrFree);
    }

    if (GetOffsetFromPtr(freeRgn.start, freeRgn.end) < sizeof(NNSiFndExpHeapMBlockHead)) // �u���b�N�ɂȂ�Ȃ��傫��
    {
        return FALSE;   // NNS_FndResizeForMBlockExpHeap()�ŏ������T�C�Y���k�����悤�Ƃ��Ă��āA
                        // �����Ƀt���[�u���b�N�������ꍇ�ɂ����ɓ��B
    }

    // �f�o�O�p�������[�U
    FillFreeMemory(GetHeapHeadPtrFromExpHeapHead(pEHHead), pRegion->start, GetOffsetFromPtr(pRegion->start, pRegion->end));

    (void)InsertMBlock(&pEHHead->mbFreeList,
        InitFreeMBlock(&freeRgn),   // �t���[�u���b�N�쐬
        pBlkPrFree);

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         CheckMBlock

  Description:  �������u���b�N�̃w�b�_�̓��e���Ó��ł��邩�`�F�b�N���܂��B

  Arguments:    pMBHead:    �`�F�b�N���郁�����u���b�N�̃w�b�_�ւ̃|�C���^�B
                pHeapHd:    �g���q�[�v�̃w�b�_�ւ̃|�C���^�B
                signature:  �������u���b�N�̃V�O�l�`���B
                heapType:   �������u���b�N�̃^�C�v(�g�p or �t���[)
                flag:       �t���O�B

  Returns:      �������u���b�N�̃w�b�_�̓��e���Ó��Ȃ� TRUE�A
                �����łȂ��Ȃ� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    static BOOL
    CheckMBlock(
        const NNSiFndExpHeapMBlockHead* pMBHead,
        NNSiFndHeapHead*                pHeapHd,
        u16                             signature,
        const char*                     heapType,
        u32                             flag
    )
    {
        const BOOL bPrint = 0 != (flag & NNS_FND_HEAP_ERROR_PRINT);
        const void *const memBlock = GetMemCPtrForMBlock(pMBHead);

        if (pHeapHd)
        {
            if ( NNSiGetUIntPtr(pMBHead) < NNSiGetUIntPtr(pHeapHd->heapStart)
              || NNSiGetUIntPtr(memBlock) > NNSiGetUIntPtr(pHeapHd->heapEnd)
            )
            {
                HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " Bad %s memory block address. - address %08X, heap area [%08X - %08X)\n",
                    heapType, memBlock, pHeapHd->heapStart, pHeapHd->heapEnd);
                return FALSE;
            }
        }
        else
        {
            if (NNSiGetUIntPtr(pMBHead) >= 0x11000000)
            {
                HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " Bad %s memory block address. - address %08X\n",
                    heapType, memBlock);
                return FALSE;
            }
        }

        if (pMBHead->signature != signature)    // �V�O�l�`�����قȂ�?
        {
            HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " Bad %s memory block signature. - address %08X, signature %04X\n",
                heapType, memBlock, pMBHead->signature);
            return FALSE;
        }

        if (pMBHead->blockSize >= 0x01000000)   // �T�C�Y���傫������?
        {
            HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " Too large %s memory block. - address %08X, block size %08X\n",
                heapType, memBlock, pMBHead->blockSize);
            return FALSE;
        }

        if (pHeapHd)
        {
            if (NNSiGetUIntPtr(memBlock) + pMBHead->blockSize > NNSiGetUIntPtr(pHeapHd->heapEnd))
            {
                HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " wrong size %s memory block. - address %08X, block size %08X\n",
                    heapType, memBlock, pMBHead->blockSize);
                return FALSE;
            }
        }

        return TRUE;
    }

// #if ! defined(NNS_FINALROM)
#endif

/*---------------------------------------------------------------------------*
  Name:         CheckUsedMBlock

  Description:  �g�p�������u���b�N�̃w�b�_�̓��e���Ó��ł��邩�`�F�b�N���܂��B

  Arguments:    pMBHead:  �`�F�b�N���郁�����u���b�N�̃w�b�_�ւ̃|�C���^�B
                pHeapHd:  �g���q�[�v�̃w�b�_�ւ̃|�C���^�B
                flag:     �t���O�B

  Returns:      �������u���b�N�̃w�b�_�̓��e���Ó��Ȃ� TRUE�A
                �����łȂ��Ȃ� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    static NNS_FND_INLINE BOOL
    CheckUsedMBlock(
        const NNSiFndExpHeapMBlockHead* pMBHead,
        NNSiFndHeapHead*                pHeapHd,
        u32                             flag
    )
    {
        return CheckMBlock(pMBHead, pHeapHd, MBLOCK_USED_SIGNATURE, "used", flag);
    }

// #if ! defined(NNS_FINALROM)
#endif

/*---------------------------------------------------------------------------*
  Name:         CheckFreeMBlock

  Description:  �t���[�������u���b�N�̃w�b�_�̓��e���Ó��ł��邩�`�F�b�N���܂��B

  Arguments:    pMBHead:  �`�F�b�N���郁�����u���b�N�̃w�b�_�ւ̃|�C���^�B
                pHeapHd:  �g���q�[�v�̃w�b�_�ւ̃|�C���^�B
                flag:     �t���O�B

  Returns:      �������u���b�N�̃w�b�_�̓��e���Ó��Ȃ� TRUE�A
                �����łȂ��Ȃ� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    static NNS_FND_INLINE BOOL
    CheckFreeMBlock(
        const NNSiFndExpHeapMBlockHead* pMBHead,
        NNSiFndHeapHead*                pHeapHd,
        u32                             flag
    )
    {
        return CheckMBlock(pMBHead, pHeapHd, MBLOCK_FREE_SIGNATURE, "free", flag);
    }

// #if ! defined(NNS_FINALROM)
#endif

/*---------------------------------------------------------------------------*
  Name:         CheckMBlockPrevPtr

  Description:  �������u���b�N�̑O�ւ̃����N�����������`�F�b�N���܂��B

  Arguments:    pMBHead:      �`�F�b�N���郁�����u���b�N�̃w�b�_�ւ̃|�C���^�B
                pMBHeadPrev:  �`�F�b�N���郁�����u���b�N�̑O�̃������u���b�N�̃w�b�_�ւ̃|�C���^�B
                flag:         �t���O�B

  Returns:      �������u���b�N�̑O�ւ̃����N���������Ȃ� TRUE�A
                �����łȂ��Ȃ� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    static BOOL
    CheckMBlockPrevPtr(
        const NNSiFndExpHeapMBlockHead* pMBHead,
        const NNSiFndExpHeapMBlockHead* pMBHeadPrev,
        u32                             flag
    )
    {
        const BOOL bPrint = 0 != (flag & NNS_FND_HEAP_ERROR_PRINT);

        if (pMBHead->pMBHeadPrev != pMBHeadPrev)
        {
            HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " Wrong link memory block. - address %08X, previous address %08X != %08X\n",
                GetMemCPtrForMBlock(pMBHead), pMBHead->pMBHeadPrev, pMBHeadPrev);
            return FALSE;
        }

        return TRUE;
    }

// #if ! defined(NNS_FINALROM)
#endif

/*---------------------------------------------------------------------------*
  Name:         CheckMBlockNextPtr

  Description:  �������u���b�N�̎��ւ̃����N�����������`�F�b�N���܂��B

  Arguments:    pMBHead:      �`�F�b�N���郁�����u���b�N�̃w�b�_�ւ̃|�C���^�B
                pMBHeadNext:  �`�F�b�N���郁�����u���b�N�̎��̃������u���b�N�̃w�b�_�ւ̃|�C���^�B
                flag:         �t���O�B

  Returns:      �������u���b�N�̎��ւ̃����N���������Ȃ� TRUE�A
                �����łȂ��Ȃ� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    static BOOL
    CheckMBlockNextPtr(
        const NNSiFndExpHeapMBlockHead* pMBHead,
        const NNSiFndExpHeapMBlockHead* pMBHeadNext,
        u32                             flag
    )
    {
        const BOOL bPrint = 0 != (flag & NNS_FND_HEAP_ERROR_PRINT);

        if (pMBHead->pMBHeadNext != pMBHeadNext)
        {
            HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " Wrong link memory block. - address %08X, next address %08X != %08X\n",
                GetMemCPtrForMBlock(pMBHead), pMBHead->pMBHeadNext, pMBHeadNext);
            return FALSE;
        }

        return TRUE;
    }

// #if ! defined(NNS_FINALROM)
#endif

/*---------------------------------------------------------------------------*
  Name:         CheckMBlockLinkTail

  Description:  �������u���b�N�̃|�C���^���������u���b�N���X�g�̐擪/�����ł��邩�`�F�b�N���܂��B

  Arguments:    pMBHead:      �`�F�b�N���郁�����u���b�N�̃w�b�_�ւ̃|�C���^�B
                pMBHeadTail:  �������u���b�N���X�g�̐擪/�����������u���b�N�̃w�b�_�ւ̃|�C���^�B
                headType:     �擪��������\��������B
                flag:         �t���O�B

  Returns:      �������u���b�N�̃|�C���^���������u���b�N���X�g�̐擪/�����ł���Ȃ� TRUE�A
                �����łȂ��Ȃ� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    static BOOL
    CheckMBlockLinkTail(
        const NNSiFndExpHeapMBlockHead* pMBHead,
        const NNSiFndExpHeapMBlockHead* pMBHeadTail,
        const char*                     heapType,
        u32                             flag
    )
    {
        const BOOL bPrint = 0 != (flag & NNS_FND_HEAP_ERROR_PRINT);

        if (pMBHead != pMBHeadTail)
        {
            HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " Wrong memory brock list %s pointer. - address %08X, %s address %08X != %08X\n",
                heapType, GetMemCPtrForMBlock(pMBHead), heapType, pMBHead, pMBHeadTail);
            return FALSE;
        }

        return TRUE;
    }

// #if ! defined(NNS_FINALROM)
#endif

/*---------------------------------------------------------------------------*
  Name:         IsValidUsedMBlock

  Description:  �g�p�������u���b�N�̑Ó������`�F�b�N���܂��B

  Arguments:    memBlock:  �`�F�b�N���郁�����u���b�N�B
                heap:      �������u���b�N���ܗL����g���q�[�v�̃n���h���B
                           NULL���w�肷��ƁA�������u���b�N���q�[�v�Ɋ܂܂�Ă��邩��
                           �`�F�b�N�͍s���܂���B

  Returns:      �w�肵���������u���b�N�����Ȃ���� TRUE ��Ԃ��܂��B
                ��肪����� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    static BOOL
    IsValidUsedMBlock(
        const void*         memBlock,
        NNSFndHeapHandle    heap
    )
    {
        NNSiFndHeapHead* pHeapHd = heap;

        if (! memBlock)
        {
            return FALSE;
        }

        return CheckUsedMBlock(GetMBlockHeadCPtr(memBlock), pHeapHd, 0);
    }

// #if ! defined(NNS_FINALROM)
#endif

/* ========================================================================
    �O���֐�(����J)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndDumpExpHeap

  Description:  �g���q�[�v�����̏���\�����܂��B
                ����̓f�o�b�O�p�̊֐��ł��B

  Arguments:    heap:    �g���q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    void
    NNSi_FndDumpExpHeap(NNSFndHeapHandle heap)
    {
        NNS_ASSERT(IsValidExpHeapHandle(heap));

        {
            u32  usedSize = 0;
            u32  usedCnt = 0;
            u32  freeSize = 0;
            u32  freeCnt = 0;

            NNSiFndHeapHead* pHeapHd = heap;
            NNSiFndExpHeapHead* pExpHeapHd = GetExpHeapHeadPtrFromHandle(pHeapHd);

            NNSi_FndDumpHeapHead(pHeapHd);

            OS_Printf("     attr  address:   size    gid aln   prev_ptr next_ptr\n");   // �w�b�_�[�s

            // ---------------- UsedBlock �̃_���v ----------------
            OS_Printf("    (Used Blocks)\n" );

            if (pExpHeapHd->mbUsedList.head == NULL)
            {
                OS_Printf("     NONE\n");
            }
            else
            {
                NNSiFndExpHeapMBlockHead* pMBHead;

                for (pMBHead = pExpHeapHd->mbUsedList.head; pMBHead; pMBHead = pMBHead->pMBHeadNext)
                {
                    if (pMBHead->signature != MBLOCK_USED_SIGNATURE)
                    {
                        OS_Printf("    xxxxx %08x: --------  --- ---  (-------- --------)\nabort\n", pMBHead);
                        break;
                    }

                    OS_Printf("    %s %08x: %8d  %3d %3d  (%08x %08x)\n",
                        GetAllocDirForMBlock(pMBHead) == NNS_FND_EXPHEAP_ALLOC_DIR_REAR ? " rear" : "front",
                        GetMemPtrForMBlock(pMBHead),
                        pMBHead->blockSize,
                        GetGroupIDForMBlock( pMBHead ),
                        GetAlignmentForMBlock( pMBHead ),
                        pMBHead->pMBHeadPrev ? GetMemPtrForMBlock(pMBHead->pMBHeadPrev): NULL,
                        pMBHead->pMBHeadNext ? GetMemPtrForMBlock(pMBHead->pMBHeadNext): NULL
                    );

                    // ---- �g�p��
                    usedSize += sizeof(NNSiFndExpHeapMBlockHead) + pMBHead->blockSize + GetAlignmentForMBlock(pMBHead);

                    usedCnt ++;
                }
            }

            // ---------------- FreeBlock �̃_���v ----------------
            OS_Printf("    (Free Blocks)\n" );

            if (pExpHeapHd->mbFreeList.head == NULL)
            {
                OS_Printf("     NONE\n" );
            }
            else
            {
                NNSiFndExpHeapMBlockHead* pMBHead;

                for (pMBHead = pExpHeapHd->mbFreeList.head; pMBHead; pMBHead = pMBHead->pMBHeadNext)
                {
                    if (pMBHead->signature != MBLOCK_FREE_SIGNATURE)
                    {
                        OS_Printf("    xxxxx %08x: --------  --- ---  (-------- --------)\nabort\n", pMBHead);
                        break;
                    }

                    OS_Printf("    %s %08x: %8d  %3d %3d  (%08x %08x)\n",
                        " free",
                        GetMemPtrForMBlock(pMBHead),
                        pMBHead->blockSize,
                        GetGroupIDForMBlock( pMBHead ),
                        GetAlignmentForMBlock( pMBHead ),
                        pMBHead->pMBHeadPrev ? GetMemPtrForMBlock(pMBHead->pMBHeadPrev): NULL,
                        pMBHead->pMBHeadNext ? GetMemPtrForMBlock(pMBHead->pMBHeadNext): NULL
                    );

                    freeCnt ++;
                }
            }

            OS_Printf("\n");

            {
                u32 heapSize  = GetOffsetFromPtr(pHeapHd->heapStart, pHeapHd->heapEnd); // �q�[�v�T�C�Y(�f�[�^�̈�̃T�C�Y)
                OS_Printf("    %d / %d bytes (%6.2f%%) used (U:%d F:%d)\n",
                                   usedSize, heapSize, 100.0 * usedSize / heapSize, usedCnt, freeCnt);
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
  Name:         NNS_FndCreateExpHeapEx

  Description:  �g���q�[�v���쐬���܂��B

  Arguments:    startAddress: �q�[�v�̈�̐擪�A�h���X�B
                size:         �q�[�v�̈�̃T�C�Y�B
                optFlag:      �I�v�V�����t���O�B

  Returns:      �֐������������ꍇ�A�쐬���ꂽ�g���q�[�v�̃n���h�����Ԃ�܂��B
                �֐������s����ƁANNS_FND_HEAP_INVALID_HANDLE ���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
NNSFndHeapHandle
NNS_FndCreateExpHeapEx(
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
     ||  GetOffsetFromPtr(startAddress, endAddress) < sizeof(NNSiFndHeapHead) + sizeof(NNSiFndExpHeapHead)
                                                        + sizeof(NNSiFndExpHeapMBlockHead) + MIN_ALIGNMENT
    )
    {
        return NNS_FND_HEAP_INVALID_HANDLE;
    }

    {   // Exp�q�[�v����������
        NNSiFndHeapHead* pHeapHd = InitExpHeap(startAddress, endAddress, optFlag);
        return pHeapHd;  // �q�[�v�w�b�_�ւ̃|�C���^�����̂܂܃n���h���l�Ƃ���
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndDestroyExpHeap

  Description:  �g���q�[�v��j�����܂��B

  Arguments:    heap: �g���q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndDestroyExpHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidExpHeapHandle(heap));

    NNSi_FndFinalizeHeap(heap);
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndAllocFromExpHeapEx

  Description:  �g���q�[�v���烁�����u���b�N���m�ۂ��܂��B
                �������u���b�N�̃A���C�����g���w��ł��܂��B
                �A���C�����g�l�𕉂̒l�Ŏw�肷��ƁA�q�[�v�̋󂫗̈���������T���܂��B

  Arguments:    heap:      �g���q�[�v�̃n���h���B
                size:      �m�ۂ��郁�����u���b�N�̃T�C�Y(�o�C�g�P��)�B
                alignment: �m�ۂ��郁�����u���b�N�̃A���C�����g�B
                           4,8,16,32,-4,-8,-16,-32�̂����ꂩ�̒l���w��ł��܂��B

  Returns:      �������u���b�N�̊m�ۂ����������ꍇ�A�m�ۂ����������u���b�N�ւ�
                �|�C���^���Ԃ�܂��B
                ���s�����ꍇ�ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
void*
NNS_FndAllocFromExpHeapEx(
    NNSFndHeapHandle    heap,
    u32                 size,
    int                 alignment)
{
    void* memory = NULL;

    NNS_ASSERT(IsValidExpHeapHandle(heap));

    // alignment �̃`�F�b�N
    NNS_ASSERT(alignment % MIN_ALIGNMENT == 0);
    NNS_ASSERT(MIN_ALIGNMENT <= abs(alignment) && abs(alignment) <= 32);

    if (size == 0)
    {
        size = 1;
    }

    size = NNSi_FndRoundUp(size, MIN_ALIGNMENT);    // ���ۂɊm�ۂ���T�C�Y

    if (alignment >= 0)     // �O����m��
    {
        memory = AllocFromHead(heap, size, alignment);
    }
    else                    // ��납��m��
    {
        memory = AllocFromTail(heap, size, -alignment);
    }

    return memory;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndResizeForMBlockExpHeap

  Description:  �g���q�[�v����m�ۂ��ꂽ�������u���b�N�̃T�C�Y��ύX���܂��B

  Arguments:    heap:     �g���q�[�v�̃n���h���B
                memBlock: �T�C�Y��ύX���郁�����u���b�N�ւ̃|�C���^�B
                size:     �V�������蓖�Ă�T�C�Y(�o�C�g�P��)�B

  Returns:      �֐������������ꍇ�A�ύX���ꂽ�������u���b�N�̃T�C�Y��Ԃ��܂�(�o�C�g�P��)�B
                �֐������s�����ꍇ�A0 ���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
u32
NNS_FndResizeForMBlockExpHeap(
    NNSFndHeapHandle    heap,
    void*               memBlock,
    u32                 size
)
{
    NNSiFndExpHeapHead* pEHHead;
    NNSiFndExpHeapMBlockHead* pMBHead;

    NNS_ASSERT(IsValidExpHeapHandle(heap));
    NNS_ASSERT(IsValidUsedMBlock(memBlock, heap));

    pEHHead = GetExpHeapHeadPtrFromHandle(heap);
    pMBHead = GetMBlockHeadPtr(memBlock);

    size = NNSi_FndRoundUp(size, MIN_ALIGNMENT);
    if (size == pMBHead->blockSize)  // �u���b�N�T�C�Y�ύX�Ȃ��̏ꍇ
    {
        return size;
    }

    // �V�����G���A���g�傷��Ƃ�
    if (size > pMBHead->blockSize)
    {
        void* crUsedEnd = GetMBlockEndAddr(pMBHead);   // ���g�p�u���b�N��end address
        NNSiFndExpHeapMBlockHead* block;

        // ���̃t���[�u���b�N��T��
        for (block = pEHHead->mbFreeList.head; block; block = block->pMBHeadNext)
        {
            if (block == crUsedEnd)
            {
                break;
            }
        }

        // ���̃t���[�u���b�N���Ȃ����A�T�C�Y������Ȃ��ꍇ
        if (! block || size > pMBHead->blockSize + sizeof(NNSiFndExpHeapMBlockHead) + block->blockSize)
        {
            return 0;
        }

        {
            NNSiMemRegion rgnNewFree;
            void* oldFreeStart;
            NNSiFndExpHeapMBlockHead* nextBlockPrev;

            // �t���[�u���b�N�̃��[�W�������擾���A�t���[�u���b�N����U�O��
            GetRegionOfMBlock(&rgnNewFree, block);
            nextBlockPrev = RemoveMBlock(&pEHHead->mbFreeList, block);

            oldFreeStart = rgnNewFree.start;
            rgnNewFree.start = AddU32ToPtr(memBlock, size); // �V�����t���[�ƂȂ郊�[�W�����ʒu

            // �]�肪�������u���b�N�T�C�Y�����Ȃ�
            if (GetOffsetFromPtr(rgnNewFree.start, rgnNewFree.end) < sizeof(NNSiFndExpHeapMBlockHead))
            {
                rgnNewFree.start = rgnNewFree.end;  // �g�p�u���b�N�ɋz��
            }

            pMBHead->blockSize = GetOffsetFromPtr(memBlock, rgnNewFree.start);  // �Ώۃu���b�N�T�C�Y�ύX

            // �]�肪�������u���b�N�T�C�Y�ȏ�Ȃ�
            if (GetOffsetFromPtr(rgnNewFree.start, rgnNewFree.end) >= sizeof(NNSiFndExpHeapMBlockHead))
            {
                (void)InsertMBlock(&pEHHead->mbFreeList, InitFreeMBlock(&rgnNewFree), nextBlockPrev);   // �V�����t���[�u���b�N�����
            }

            FillAllocMemory(  // �g�����������t�B��
                heap,
                oldFreeStart,
                GetOffsetFromPtr(oldFreeStart, rgnNewFree.start));
        }
    }
    // �V�����G���A���k������Ƃ�
    else
    {
        NNSiMemRegion rgnNewFree;
        const u32 oldBlockSize = pMBHead->blockSize;

        rgnNewFree.start = AddU32ToPtr(memBlock, size); // �V�����t���[�ƂȂ郊�[�W�����ʒu
        rgnNewFree.end   = GetMBlockEndAddr(pMBHead);   // ���g�p�u���b�N��end address

        pMBHead->blockSize = size;  // �Ώۃu���b�N�T�C�Y�ύX

        if (! RecycleRegion(pEHHead, &rgnNewFree))    // �t���[���X�g�ɕԂ��Ă݂�
        {
            pMBHead->blockSize = oldBlockSize;  // ���s������A���ɖ߂�
        }
    }

    return pMBHead->blockSize;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndFreeToExpHeap

  Description:  �g���q�[�v�փ������u���b�N��ԋp���܂��B

  Arguments:    heap:     �g���q�[�v�̃n���h���B
                memBlock: �ԋp���郁�����u���b�N�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndFreeToExpHeap(
    NNSFndHeapHandle    heap,
    void*               memBlock
)
{
    NNS_ASSERT(IsValidExpHeapHandle(heap));

    {
        NNSiFndHeapHead* pHeapHd = heap;
        NNSiFndExpHeapHead* pExpHeapHd = GetExpHeapHeadPtrFromHandle(pHeapHd);
        NNSiFndExpHeapMBlockHead* pMBHead = GetMBlockHeadPtr(memBlock);
        NNSiMemRegion region;

        // ���̃q�[�v�̒��ɓ����Ă��邩
        NNS_ASSERT(pHeapHd->heapStart <= memBlock && memBlock < pHeapHd->heapEnd);

        GetRegionOfMBlock(&region, pMBHead);
        (void)RemoveMBlock(&pExpHeapHd->mbUsedList, pMBHead);   // �g�p���X�g����͂���
        (void)RecycleRegion(pExpHeapHd, &region);   // �w��A�h���X����w��T�C�Y���t���[�ɉ�����
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetTotalFreeSizeForExpHeap

  Description:  �g���q�[�v�̋󂫗̈�̃T�C�Y�̍��v���擾���܂��B

  Arguments:    heap:     �g���q�[�v�̃n���h���B

  Returns:      �g���q�[�v�̋󂫗̈�̃T�C�Y�̍��v��Ԃ��܂�(�o�C�g�P��)�B
 *---------------------------------------------------------------------------*/
u32
NNS_FndGetTotalFreeSizeForExpHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidExpHeapHandle(heap));

    {
        u32 sumSize = 0;
        NNSiFndExpHeapHead* pEHHead = GetExpHeapHeadPtrFromHandle(heap);
        NNSiFndExpHeapMBlockHead* pMBHead;

        for(pMBHead = pEHHead->mbFreeList.head; pMBHead; pMBHead = pMBHead->pMBHeadNext)
        {
            sumSize += pMBHead->blockSize;
        }

        return sumSize;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetAllocatableSizeForExpHeapEx

  Description:  �g���q�[�v���̊��蓖�ĉ\�ȍő�T�C�Y���擾���܂��B
                �������u���b�N�̃A���C�����g���w��ł��܂��B

  Arguments:    heap:      �g���q�[�v�̃n���h���B
                alignment: �m�ۂ��郁�����u���b�N�̃A���C�����g�B
                           4,8,16,32�̂����ꂩ�̒l���w��ł��܂��B

  Returns:      �g���q�[�v���̊��蓖�ĉ\�ȍő�T�C�Y��Ԃ��܂�(�o�C�g�P��)�B
 *---------------------------------------------------------------------------*/
u32
NNS_FndGetAllocatableSizeForExpHeapEx(
    NNSFndHeapHandle    heap,
    int                 alignment
)
{
    NNS_ASSERT(IsValidExpHeapHandle(heap));

    // alignment �̃`�F�b�N
    NNS_ASSERT(alignment % MIN_ALIGNMENT == 0);
    NNS_ASSERT(MIN_ALIGNMENT <= abs(alignment) && abs(alignment) <= 32);

    alignment = abs(alignment); // �O�̂��ߐ�����

    {
        NNSiFndExpHeapHead* pEHHead = GetExpHeapHeadPtrFromHandle(heap);
        u32 maxSize = 0;
        u32 offsetMin = 0xFFFFFFFF;
        NNSiFndExpHeapMBlockHead* pMBlkHd;

        for (pMBlkHd = pEHHead->mbFreeList.head; pMBlkHd; pMBlkHd = pMBlkHd->pMBHeadNext)
        {
            // �A���C�����g���l�������������u���b�N�ʒu
            void* baseAddress = NNSi_FndRoundUpPtr(GetMemPtrForMBlock(pMBlkHd), alignment);

            if (NNSiGetUIntPtr(baseAddress) < NNSiGetUIntPtr(GetMBlockEndAddr(pMBlkHd)))
            {
                const u32 blockSize = GetOffsetFromPtr(baseAddress, GetMBlockEndAddr(pMBlkHd));
                // �A���C�����g�ɂ��󂫃G���A
                const u32 offset = GetOffsetFromPtr(GetMemPtrForMBlock(pMBlkHd), baseAddress);

                /*
                    �T�C�Y���傫���ꍇ�A���邢�̓T�C�Y�������ł����ʂȋ�Ԃ���菬�����ꍇ�́A
                    �������u���b�N���X�V
                 */
                if ( maxSize < blockSize
                 ||  (maxSize == blockSize && offsetMin > offset)
                )
                {
                    maxSize = blockSize;
                    offsetMin= offset;
                }
            }
        }

        return maxSize;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndSetAllocModeForExpHeap

  Description:  �g���q�[�v�̃������m�ۃ��[�h���Z�b�g���܂��B

  Arguments:    heap:  �g���q�[�v�̃n���h���B
                mode:  �������m�ۃ��[�h�B

  Returns:      �ȑO�̊g���q�[�v�̃������m�ۃ��[�h��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u16
NNS_FndSetAllocModeForExpHeap(
    NNSFndHeapHandle    heap,
    u16                 mode
)
{
    NNS_ASSERT(IsValidExpHeapHandle(heap));

    {
        NNSiFndExpHeapHead *const pEHHead = GetExpHeapHeadPtrFromHandle(heap);
        u16 oldMode = GetAllocMode(pEHHead);
        SetAllocMode(pEHHead, mode);

        return oldMode;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetAllocModeForExpHeap

  Description:  �g���q�[�v�̃������m�ۃ��[�h���擾���܂��B

  Arguments:    heap:    �g���q�[�v�̃n���h���B

  Returns:      �g���q�[�v�̃������m�ۃ��[�h��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u16
NNS_FndGetAllocModeForExpHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidExpHeapHandle(heap));
    return GetAllocMode(GetExpHeapHeadPtrFromHandle(heap));
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndSetGroupIDForExpHeap

  Description:  �g���q�[�v�̃O���[�vID���Z�b�g���܂��B

  Arguments:    heap:    �g���q�[�v�̃n���h���B
                groupID: �Z�b�g����O���[�vID�l�B

  Returns:      �ȑO�̃O���[�vID�l���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
u16
NNS_FndSetGroupIDForExpHeap(
    NNSFndHeapHandle    heap,
    u16                 groupID)
{
    NNS_ASSERT(IsValidExpHeapHandle(heap));
    NNS_ASSERT(groupID <= MAX_GROUPID);

    {
        NNSiFndExpHeapHead* pEHHead = GetExpHeapHeadPtrFromHandle(heap);
        u16 oldGroupID = pEHHead->groupID;
        pEHHead->groupID = groupID;

        return oldGroupID;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetGroupIDForExpHeap

  Description:  �g���q�[�v�̃O���[�vID���擾���܂��B

  Arguments:    heap:    �g���q�[�v�̃n���h���B

  Returns:      �O���[�vID�l���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
u16
NNS_FndGetGroupIDForExpHeap(NNSFndHeapHandle heap)
{
    NNS_ASSERT(IsValidExpHeapHandle(heap));

    return GetExpHeapHeadPtrFromHandle(heap)->groupID;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndVisitAllocatedForExpHeap

  Description:  �g���q�[�v���犄�蓖�Ă�ꂽ�������u���b�N�S�Ăɑ΂��āA
                ���[�U���w�肵���֐����Ă΂��܂��B
                visitor�֐��ŌĂ΂�郁�����u���b�N�̏��Ԃ́A�m�ۂ������ԂɂȂ�܂��B

                visitor �̌^ NNSFndHeapVisitor �͎��̂悤�ɒ�`����Ă��܂��B

                    typedef void (*NNSFndHeapVisitor)(
                                    void*               memBlock,
                                    NNSFndHeapHandle    heap,
                                    u32                 userParam);

                                        memBlock:   �������u���b�N�ւ̃|�C���^�B
                                        heap:       �������u���b�N���ܗL����q�[�v�B
                                        userParam:  ���[�U�[�p�p�����[�^�B

  Arguments:    heap:       �g���q�[�v�̃n���h���B
                visitor:    �e�������u���b�N�ɑ΂��ČĂ΂���֐��B
                userParam:  visitor�֐��ɓn�����[�U�w��̃p�����[�^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_FndVisitAllocatedForExpHeap(
    NNSFndHeapHandle    heap,
    NNSFndHeapVisitor   visitor,
    u32                 userParam
)
{
    NNS_ASSERT(IsValidExpHeapHandle(heap));
    SDK_NULL_ASSERT(visitor);

    {
        NNSiFndExpHeapMBlockHead* pMBHead = GetExpHeapHeadPtrFromHandle(heap)->mbUsedList.head;

        while (pMBHead)
        {
            NNSiFndExpHeapMBlockHead* pMBHeadNext = pMBHead->pMBHeadNext;
            (*visitor)(GetMemPtrForMBlock(pMBHead), heap, userParam);
            pMBHead = pMBHeadNext;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetSizeForMBlockExpHeap

  Description:  �g���q�[�v����m�ۂ��ꂽ�������u���b�N�̃T�C�Y���擾���܂��B

  Arguments:    memBlock:  �T�C�Y���擾���郁�����u���b�N�ւ̃|�C���^�B

  Returns:      �w�肵���������u���b�N�̃T�C�Y��Ԃ��܂�(�o�C�g�P��)�B
 *---------------------------------------------------------------------------*/
u32
NNS_FndGetSizeForMBlockExpHeap(const void* memBlock)
{
    NNS_ASSERT(IsValidUsedMBlock(memBlock, NULL));

    return GetMBlockHeadCPtr(memBlock)->blockSize;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetGroupIDForMBlockExpHeap

  Description:  �g���q�[�v����m�ۂ��ꂽ�������u���b�N�̃O���[�vID���擾���܂��B

  Arguments:    memBlock:  �O���[�vID���擾���郁�����u���b�N�ւ̃|�C���^�B

  Returns:      �w�肵���������u���b�N�̃O���[�vID���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
u16
NNS_FndGetGroupIDForMBlockExpHeap(const void* memBlock)
{
    NNS_ASSERT(IsValidUsedMBlock(memBlock, NULL));

    return GetGroupIDForMBlock(GetMBlockHeadCPtr(memBlock));
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetAllocDirForMBlockExpHeap

  Description:  �g���q�[�v����m�ۂ��ꂽ�������u���b�N�̊m�ە������擾���܂��B

  Arguments:    memBlock:  �������u���b�N�ւ̃|�C���^�B

  Returns:      �w�肵���������u���b�N�̊m�ە������Ԃ�܂��B
 *---------------------------------------------------------------------------*/
u16
NNS_FndGetAllocDirForMBlockExpHeap(const void* memBlock)
{
    NNS_ASSERT(IsValidUsedMBlock(memBlock, NULL));

    return GetAllocDirForMBlock(GetMBlockHeadCPtr(memBlock));
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndCheckExpHeap

  Description:  �g���q�[�v���j�󂳂�Ă��Ȃ����ǂ������`�F�b�N���܂��B

  Arguments:    heap:     �g���q�[�v�̃n���h���B
                optFlag:  �t���O�B

  Returns:      �q�[�v�����킾�����ꍇ TRUE ��Ԃ��܂��B
                �q�[�v�ɃG���[���������ꍇ�AFALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    BOOL
    NNS_FndCheckExpHeap(
        NNSFndHeapHandle    heap,
        u32                 optFlag
    )
    {
        const BOOL bPrint = 0 != (optFlag & NNS_FND_HEAP_ERROR_PRINT);
        u32  totalBytes  = 0;

        if (! IsValidExpHeapHandle(heap))
        {
            HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " Invalid heap handle. - %08X\n", heap);
            return FALSE;
        }

        {
            NNSiFndHeapHead *const pHeapHd = heap;
            NNSiFndExpHeapHead *const pExpHeapHd = GetExpHeapHeadPtrFromHeapHead(pHeapHd);
            NNSiFndExpHeapMBlockHead* pMBHead = NULL;
            NNSiFndExpHeapMBlockHead* pMBHeadPrev = NULL;

            // �g�p�u���b�N
            for (pMBHead = pExpHeapHd->mbUsedList.head; pMBHead; pMBHeadPrev = pMBHead, pMBHead = pMBHead->pMBHeadNext)
            {
                if ( ! CheckUsedMBlock(pMBHead, pHeapHd, optFlag)
                  || ! CheckMBlockPrevPtr(pMBHead, pMBHeadPrev, optFlag)   // �O�u���b�N�ւ̃|�C���^��1���[�v�O�̃������u���b�N�̃|�C���^�Ɠ����łȂ�?
                )
                {
                    return FALSE;
                }

                // ��L�T�C�Y��ώZ
                totalBytes += sizeof(NNSiFndExpHeapMBlockHead) + pMBHead->blockSize + GetAlignmentForMBlock(pMBHead);
            }

            if (! CheckMBlockLinkTail(pMBHeadPrev, pExpHeapHd->mbUsedList.tail, "tail", optFlag))  // �����u���b�N���Ō�̃u���b�N�ւ̃|�C���^���w���Ă��邩?
            {
                return FALSE;
            }

            // �t���[�u���b�N
            pMBHead = NULL;
            pMBHeadPrev = NULL;
            for (pMBHead = pExpHeapHd->mbFreeList.head; pMBHead; pMBHeadPrev = pMBHead, pMBHead = pMBHead->pMBHeadNext)
            {
                if ( ! CheckFreeMBlock(pMBHead, pHeapHd, optFlag)
                  || ! CheckMBlockPrevPtr(pMBHead, pMBHeadPrev, optFlag)   // �O�u���b�N�ւ̃|�C���^��1���[�v�O�̃������u���b�N�̃|�C���^�Ɠ����łȂ�?
                )
                {
                    return FALSE;
                }

                // ��L�T�C�Y��ώZ
                totalBytes += sizeof(NNSiFndExpHeapMBlockHead) + pMBHead->blockSize;
            }

            if (! CheckMBlockLinkTail(pMBHeadPrev, pExpHeapHd->mbFreeList.tail, "tail", optFlag))  // �����u���b�N���Ō�̃u���b�N�ւ̃|�C���^���w���Ă��邩?
            {
                return FALSE;
            }

            // �S�̂̌��ʕ\��
            if (totalBytes != GetOffsetFromPtr(pHeapHd->heapStart, pHeapHd->heapEnd))
            {
                HEAP_WARNING(bPrint, "[NNS Foundation " "Exp" " Heap]" " Incorrect total memory block size. - heap size %08X, sum size %08X\n",
                    GetOffsetFromPtr(pHeapHd->heapStart, pHeapHd->heapEnd), totalBytes);
                return FALSE;
            }

            return TRUE;
        }
    }

// #if ! defined(NNS_FINALROM)
#endif

/*---------------------------------------------------------------------------*
  Name:         NNS_FndCheckForMBlockExpHeap

  Description:  �g���q�[�v�̃������u���b�N���j�󂳂�Ă��Ȃ����ǂ������`�F�b�N���܂��B

  Arguments:    memBlock:  �`�F�b�N���郁�����u���b�N�ւ̃|�C���^�B
                heap:      �g���q�[�v�̃n���h���B
                optFlag:   �t���O�B

  Returns:      �������u���b�N�����킾�����ꍇ TRUE ��Ԃ��܂��B
                �������u���b�N�ɃG���[���������ꍇ�AFALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    BOOL
    NNS_FndCheckForMBlockExpHeap(
        const void*         memBlock,
        NNSFndHeapHandle    heap,
        u32                 optFlag
    )
    {
        const NNSiFndExpHeapMBlockHead* pMBHead = NULL;
        NNSiFndHeapHead *const pHeapHd = heap;

        if (! memBlock)
        {
            return FALSE;
        }

        pMBHead = GetMBlockHeadCPtr(memBlock);

        if (! CheckUsedMBlock(pMBHead, pHeapHd, optFlag))
        {
            return FALSE;
        }

        if (pMBHead->pMBHeadPrev)
        {
            if ( ! CheckUsedMBlock(pMBHead->pMBHeadPrev, pHeapHd, optFlag)     // �O�u���b�N�̃V�O�l�`���ƃT�C�Y�̃`�F�b�N
              || ! CheckMBlockNextPtr(pMBHead->pMBHeadPrev, pMBHead, optFlag)  // �O�u���b�N�̎��u���b�N�ւ̃|�C���^���������w���Ă��邩?
            )
            {
                return FALSE;
            }
        }
        else
        {
            if (pHeapHd)
            {
                // prev�� NULL �̂Ƃ��́A���X�g��head�|�C���^�͎������w���Ă���͂�
                if (! CheckMBlockLinkTail(pMBHead, GetExpHeapHeadPtrFromHeapHead(pHeapHd)->mbUsedList.head, "head", optFlag))
                {
                    return FALSE;
                }
            }
        }

        if (pMBHead->pMBHeadNext)
        {
            if ( ! CheckUsedMBlock(pMBHead->pMBHeadNext, pHeapHd, optFlag)     // ���u���b�N�̃V�O�l�`���ƃT�C�Y�̃`�F�b�N
              || ! CheckMBlockPrevPtr(pMBHead->pMBHeadNext, pMBHead, optFlag)  // ���u���b�N�̑O�u���b�N�ւ̃|�C���^���������w���Ă��邩?
            )
            {
                return FALSE;
            }
        }
        else
        {
            if (pHeapHd)
            {
                // next�� NULL �̂Ƃ��́A���X�g��tail�|�C���^�͎������w���Ă���͂�
                if (! CheckMBlockLinkTail(pMBHead, GetExpHeapHeadPtrFromHeapHead(pHeapHd)->mbUsedList.tail, "tail", optFlag))
                {
                    return FALSE;
                }
            }
        }

        return TRUE;
    }

// #if ! defined(NNS_FINALROM)
#endif

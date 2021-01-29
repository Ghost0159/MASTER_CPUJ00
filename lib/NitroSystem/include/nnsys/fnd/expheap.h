/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - fnd
  File:     expheap.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_FND_EXPHEAP_H_
#define NNS_FND_EXPHEAP_H_

#include <nnsys/fnd/heapcommon.h>

#ifdef __cplusplus
extern "C" {
#endif


/* =======================================================================
    �萔��`
   ======================================================================== */

// �������m�ە���
enum
{
    NNS_FND_EXPHEAP_ALLOC_DIR_FRONT,    // �O�����m��
    NNS_FND_EXPHEAP_ALLOC_DIR_REAR      // ������m��
};

// �������m�ۃ��[�h
enum
{
    /*
        ���̑����l���Z�b�g����Ă���ƁA�m�ۂ��悤�Ƃ��Ă���
        �������u���b�N�̃T�C�Y�ȏ�̑傫�������A
        �ŏ��Ɍ��������󂫗̈悩�烁�����u���b�N���m�ۂ��܂��B
    */
    NNS_FND_EXPHEAP_ALLOC_MODE_FIRST,

    /*
        ���̑����l���Z�b�g����Ă���ƁA�m�ۂ��悤�Ƃ��Ă���
        �������u���b�N�̃T�C�Y�Ɉ�ԋ߂��T�C�Y�̋󂫗̈��T���A
        ���̋󂫗̈悩�烁�����u���b�N���m�ۂ��܂��B
    */
    NNS_FND_EXPHEAP_ALLOC_MODE_NEAR
};


/* =======================================================================
    �^��`
   ======================================================================== */

typedef struct NNSiFndExpHeapMBlockHead NNSiFndExpHeapMBlockHead;

// �������u���b�N�̃w�b�_���
struct NNSiFndExpHeapMBlockHead
{
    u16                         signature;      // �V�O�l�`��
    u16                         attribute;      // ����
                                                // [8:�O���[�vID]
                                                // [7:�A���C�������g]
                                                // [1:�e���|�����t���O]

    u32                         blockSize;      // �u���b�N�T�C�Y(�f�[�^�̈�̂�)

    NNSiFndExpHeapMBlockHead*   pMBHeadPrev;    // �O�u���b�N
    NNSiFndExpHeapMBlockHead*   pMBHeadNext;    // ���u���b�N
};

typedef struct NNSiFndExpMBlockList NNSiFndExpMBlockList;

// �������u���b�N�̃��X�g
struct NNSiFndExpMBlockList
{
    NNSiFndExpHeapMBlockHead*   head;   // �擪�Ɍq����Ă��郁�����u���b�N�ւ̃|�C���^
    NNSiFndExpHeapMBlockHead*   tail;   // ����Ɍq����Ă��郁�����u���b�N�ւ̃|�C���^
};

typedef struct NNSiFndExpHeapHead NNSiFndExpHeapHead;

// �g���q�[�v�̃w�b�_���
struct NNSiFndExpHeapHead
{
    NNSiFndExpMBlockList        mbFreeList;     // �t���[���X�g
    NNSiFndExpMBlockList        mbUsedList;     // �g�p���X�g

    u16                         groupID;        // �J�����g�O���[�vID (����8bit�̂�)
    u16                         feature;        // ����
};

// �������u���b�N���ɌĂяo�����R�[���o�b�N�֐��̌^
typedef void        (*NNSFndHeapVisitor)(
                        void*               memBlock,
                        NNSFndHeapHandle    heap,
                        u32                 userParam);

/* =======================================================================
    �}�N���֐�
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNS_FndCreateExpHeap

  Description:  �g���q�[�v���쐬���܂��B

  Arguments:    startAddress: �q�[�v�̈�̐擪�A�h���X�B
                size:         �q�[�v�̈�̃T�C�Y�B

  Returns:      �֐������������ꍇ�A�쐬���ꂽ�g���q�[�v�̃n���h�����Ԃ�܂��B
                �֐������s����ƁANNS_FND_HEAP_INVALID_HANDLE ���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
#define             NNS_FndCreateExpHeap(startAddress, size) \
                        NNS_FndCreateExpHeapEx(startAddress, size, 0)

/*---------------------------------------------------------------------------*
  Name:         NNS_FndAllocFromExpHeap

  Description:  �g���q�[�v���烁�����u���b�N���m�ۂ��܂��B
                �������u���b�N�̃A���C�����g��4�o�C�g�Œ�ł��B

  Arguments:    heap:   �g���q�[�v�̃n���h���B
                size:   �m�ۂ��郁�����u���b�N�̃T�C�Y(�o�C�g�P��)�B

  Returns:      �������u���b�N�̊m�ۂ����������ꍇ�A�m�ۂ����������u���b�N�ւ�
                �|�C���^���Ԃ�܂��B
                ���s�����ꍇ�ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
#define             NNS_FndAllocFromExpHeap(heap, size) \
                        NNS_FndAllocFromExpHeapEx(heap, size, NNS_FND_HEAP_DEFAULT_ALIGNMENT)

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetAllocatableSizeForExpHeap

  Description:  �g���q�[�v���̊��蓖�ĉ\�ȍő�T�C�Y���擾���܂��B
                �������u���b�N�̃A���C�����g��4�o�C�g�Œ�ł��B

  Arguments:    heap:     �g���q�[�v�̃n���h���B

  Returns:      �g���q�[�v���̊��蓖�ĉ\�ȍő�T�C�Y��Ԃ��܂�(�o�C�g�P��)�B
 *---------------------------------------------------------------------------*/
#define             NNS_FndGetAllocatableSizeForExpHeap(heap) \
                        NNS_FndGetAllocatableSizeForExpHeapEx(heap, NNS_FND_HEAP_DEFAULT_ALIGNMENT)


/* =======================================================================
    �֐��v���g�^�C�v
   ======================================================================== */

#if ! defined(NNS_FINALROM)

    void                NNSi_FndDumpExpHeap(
                            NNSFndHeapHandle    heap);

// #if ! defined(NNS_FINALROM)
#endif

NNSFndHeapHandle    NNS_FndCreateExpHeapEx(
                        void*   startAddress,
                        u32     size,
                        u16     optFlag);

void                NNS_FndDestroyExpHeap(
                        NNSFndHeapHandle    heap);

void*               NNS_FndAllocFromExpHeapEx(
                        NNSFndHeapHandle    heap,
                        u32                 size,
                        int                 alignment);

u32                 NNS_FndResizeForMBlockExpHeap(
                        NNSFndHeapHandle    heap,
                        void*               memBlock,
                        u32                 size);

void                NNS_FndFreeToExpHeap(
                        NNSFndHeapHandle    heap,
                        void*               memBlock);

u32                 NNS_FndGetTotalFreeSizeForExpHeap(
                        NNSFndHeapHandle    heap);

u32                 NNS_FndGetAllocatableSizeForExpHeapEx(
                        NNSFndHeapHandle    heap,
                        int                 alignment);

u16                 NNS_FndSetAllocModeForExpHeap(
                        NNSFndHeapHandle    heap,
                        u16                 mode);

u16                 NNS_FndGetAllocModeForExpHeap(
                        NNSFndHeapHandle    heap);

u16                 NNS_FndSetGroupIDForExpHeap(
                        NNSFndHeapHandle    heap,
                        u16                 groupID);

u16                 NNS_FndGetGroupIDForExpHeap(
                        NNSFndHeapHandle    heap);

void                NNS_FndVisitAllocatedForExpHeap(
                        NNSFndHeapHandle    heap,
                        NNSFndHeapVisitor   visitor,
                        u32                 userParam);

u32                 NNS_FndGetSizeForMBlockExpHeap(
                        const void*         memBlock);

u16                 NNS_FndGetGroupIDForMBlockExpHeap(
                        const void*         memBlock);

u16                 NNS_FndGetAllocDirForMBlockExpHeap(
                        const void*         memBlock);

#if defined(NNS_FINALROM)

    #define             NNS_FndCheckExpHeap(heap, optFlag)                      (TRUE)

    #define             NNS_FndCheckForMBlockExpHeap(memBlock, heap, optFlag)   (TRUE)

// #if defined(NNS_FINALROM)
#else

    BOOL                NNS_FndCheckExpHeap(
                            NNSFndHeapHandle    heap,
                            u32                 optFlag);

    BOOL                NNS_FndCheckForMBlockExpHeap(
                            const void*         memBlock,
                            NNSFndHeapHandle    heap,
                            u32                 optFlag);

// #if defined(NNS_FINALROM)
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_FND_EXPHEAP_H_ */
#endif

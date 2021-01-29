/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - fnd
  File:     frameheap.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_FND_FRAMEHEAP_H_
#define NNS_FND_FRAMEHEAP_H_

#include <nnsys/fnd/heapcommon.h>

#ifdef __cplusplus
extern "C" {
#endif


/* =======================================================================
    �萔��`
   ======================================================================== */

#define NNS_FND_FRMHEAP_FREE_HEAD (1 <<0)
#define NNS_FND_FRMHEAP_FREE_TAIL (1 <<1)
#define NNS_FND_FRMHEAP_FREE_ALL  (NNS_FND_FRMHEAP_FREE_HEAD | NNS_FND_FRMHEAP_FREE_TAIL)


/* =======================================================================
    �^��`
   ======================================================================== */

typedef struct NNSiFndFrmHeapState NNSiFndFrmHeapState;

// ��ԕۑ��p�\����
struct NNSiFndFrmHeapState
{
	u32						tagName;        // �^�O��
	void*					headAllocator;  // �t���[���q�[�v�̐擪�ʒu
	void*					tailAllocator;  // �t���[���q�[�v�̖����ʒu
	NNSiFndFrmHeapState*    pPrevState;     // 1�O�̏�ԕۑ��ւ̃|�C���^
};

typedef struct NNSiFndFrmHeapHead NNSiFndFrmHeapHead;

// �t���[���q�[�v�̃w�b�_���
struct NNSiFndFrmHeapHead
{
	void*				    headAllocator;  // �擪�������m�ۃ|�C���^
	void*				    tailAllocator;  // �����������m�ۃ|�C���^

	NNSiFndFrmHeapState*	pState;         // ��ԕۑ��p�����[�^
};


/* =======================================================================
    �}�N���֐�
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNS_FndCreateFrmHeap

  Description:  �t���[���q�[�v���쐬���܂��B

  Arguments:    startAddress: �q�[�v�̈�̐擪�A�h���X�B
                size:         �q�[�v�̈�̃T�C�Y�B

  Returns:      �֐������������ꍇ�A�쐬���ꂽ�t���[���q�[�v�̃n���h�����Ԃ�܂��B
                �֐������s����ƁANNS_FND_INVALID_HEAP_HANDLE ���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
#define             NNS_FndCreateFrmHeap(startAddress, size) \
                        NNS_FndCreateFrmHeapEx(startAddress, size, 0)

/*---------------------------------------------------------------------------*
  Name:         NNS_FndAllocFromFrmHeap

  Description:  �t���[���q�[�v���烁�����u���b�N���m�ۂ��܂��B
                �������u���b�N�̃A���C�����g��4�o�C�g�Œ�ł��B

  Arguments:    heap:   �t���[���q�[�v�̃n���h���B
                size:   �m�ۂ��郁�����u���b�N�̃T�C�Y(�o�C�g�P��)�B

  Returns:      �������u���b�N�̊m�ۂ����������ꍇ�A�m�ۂ����������u���b�N�ւ�
                �|�C���^���Ԃ�܂��B
                ���s�����ꍇ�ANULL���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
#define             NNS_FndAllocFromFrmHeap(heap, size) \
                        NNS_FndAllocFromFrmHeapEx(heap, size, NNS_FND_HEAP_DEFAULT_ALIGNMENT)

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetAllocatableSizeForFrmHeap

  Description:  �t���[���q�[�v���̊��蓖�ĉ\�ȍő�T�C�Y���擾���܂��B
                �������u���b�N�̃A���C�����g��4�o�C�g�Œ�ł��B

  Arguments:    heap:      �t���[���q�[�v�̃n���h���B

  Returns:      �t���[���q�[�v���̊��蓖�ĉ\�ȍő�T�C�Y��Ԃ��܂�(�o�C�g�P��)�B
 *---------------------------------------------------------------------------*/
#define             NNS_FndGetAllocatableSizeForFrmHeap(heap) \
                        NNS_FndGetAllocatableSizeForFrmHeapEx(heap, NNS_FND_HEAP_DEFAULT_ALIGNMENT)


/* =======================================================================
    �֐��v���g�^�C�v
   ======================================================================== */

void*               NNSi_FndGetFreeStartForFrmHeap(
                        NNSFndHeapHandle    heap);

void*               NNSi_FndGetFreeEndForFrmHeap(
                        NNSFndHeapHandle    heap);

#if ! defined(NNS_FINALROM)

    void                NNSi_FndDumpFrmHeap(
                            NNSFndHeapHandle    heap);

// #if ! defined(NNS_FINALROM)
#endif

NNSFndHeapHandle    NNS_FndCreateFrmHeapEx(
                        void*   startAddress,
                        u32     size,
                        u16     optFlag);

void                NNS_FndDestroyFrmHeap(
                        NNSFndHeapHandle    heap);

void*               NNS_FndAllocFromFrmHeapEx(
                        NNSFndHeapHandle    heap,
                        u32                 size,
                        int                 alignment);

void                NNS_FndFreeToFrmHeap(
                        NNSFndHeapHandle    heap,
                        int                 mode);

u32                 NNS_FndGetAllocatableSizeForFrmHeapEx(
                        NNSFndHeapHandle    heap,
                        int                 alignment);

BOOL                NNS_FndRecordStateForFrmHeap(
                        NNSFndHeapHandle    heap,
                        u32                 tagName);

BOOL                NNS_FndFreeByStateToFrmHeap(
                        NNSFndHeapHandle    heap,
                        u32                 tagName);

u32                 NNS_FndAdjustFrmHeap(
                        NNSFndHeapHandle    heap);

u32                 NNS_FndResizeForMBlockFrmHeap(
                        NNSFndHeapHandle    heap,
                        void*               memBlock,
                        u32                 newSize);


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_FND_FRAMEHEAP_H_ */
#endif

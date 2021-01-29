/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - fnd
  File:     heapcommon.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.12 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_FND_HEAPCOMMON_H_
#define NNS_FND_HEAPCOMMON_H_

#include <nitro/types.h>
#include <nnsys/fnd/list.h>

#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================
    �}�N���萔
   ======================================================================== */

// �����ȃq�[�v�n���h��
#define NNS_FND_HEAP_INVALID_HANDLE     NULL

// �q�[�v���烁���������蓖�Ă�Ƃ��̃f�t�H���g�̃A���C�����g�T�C�Y
#define NNS_FND_HEAP_DEFAULT_ALIGNMENT    4

// �g���q�[�v�̃V�O�l�`��
#define NNSI_EXPHEAP_SIGNATURE    ('EXPH')

// �t���[���q�[�v�̃V�O�l�`��
#define NNSI_FRMHEAP_SIGNATURE    ('FRMH')

// ���j�b�g�q�[�v�̃V�O�l�`��
#define NNSI_UNTHEAP_SIGNATURE    ('UNTH')


/* ------------------------------------------------------------------------
    �t�B���֌W
   ------------------------------------------------------------------------ */

// �������m�ێ��Ƀ�������0�ŃN���A
#define NNS_FND_HEAP_OPT_0_CLEAR        (1 <<0)

// �q�[�v�쐬���E�������m�ہE������Ƀ������[�U
#define NNS_FND_HEAP_OPT_DEBUG_FILL     (1 <<1)


/* ------------------------------------------------------------------------
    �q�[�v�`�F�b�N�֌W
   ------------------------------------------------------------------------ */

//  ���̃r�b�g�������Ă���ƃG���[�o��
#define NNS_FND_HEAP_ERROR_PRINT        (1 <<0)


/* ========================================================================
    enum�萔
   ======================================================================== */

enum {
    NNS_FND_HEAP_FILL_NOUSE,    // �f�o�b�O�t�B�����g�p��
    NNS_FND_HEAP_FILL_ALLOC,    // �f�o�b�O�t�B���m�ێ�
    NNS_FND_HEAP_FILL_FREE,     // �f�o�b�O�t�B�������

    NNS_FND_HEAP_FILL_MAX
};


/* =======================================================================
    �^��`
   ======================================================================== */

typedef struct NNSiFndHeapHead NNSiFndHeapHead;

// �q�[�v���ʃw�b�_
struct NNSiFndHeapHead
{
    u32             signature;

    NNSFndLink      link;
    NNSFndList      childList;

    void*           heapStart;      // �q�[�v�擪�A�h���X
    void*           heapEnd;        // �q�[�v����(+1)�A�h���X

    u32             attribute;      // ����
                                    // [8:�I�v�V�����t���O]
};

typedef NNSiFndHeapHead* NNSFndHeapHandle;   // �q�[�v�̃n���h����\���^


/* ========================================================================
    �}�N���֐�
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetHeapStartAddress

  Description:  �q�[�v�����p���郁�����̈�̊J�n�A�h���X���擾���܂��B

  Arguments:    heap:  �q�[�v�n���h���B

  Returns:      �q�[�v�����p���郁�����̈�̊J�n�A�h���X��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#define             NNS_FndGetHeapStartAddress(heap) \
                        ((void*)(heap))

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetHeapEndAddress

  Description:  �q�[�v�����p���郁�����̈�̏I���A�h���X +1 ���擾���܂��B

  Arguments:    heap:  �q�[�v�n���h���B

  Returns:      �q�[�v�����p���郁�����̈�̏I���A�h���X +1 ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#define             NNS_FndGetHeapEndAddress(heap) \
                        (((NNSiFndHeapHead*)(heap))->heapEnd)


/* =======================================================================
    �֐��v���g�^�C�v
   ======================================================================== */

NNSFndHeapHandle    NNS_FndFindContainHeap(
                        const void* memBlock);

#if defined(NNS_FINALROM)
    #define             NNS_FndDumpHeap(heap)				((void)0)
#else
	void                NNS_FndDumpHeap(
	                        NNSFndHeapHandle heap);
#endif

#if defined(NNS_FINALROM)
    #define             NNS_FndSetFillValForHeap(type, val) (0)
#else
    u32                 NNS_FndSetFillValForHeap(
                            int     type,
                            u32     val);
#endif

#if defined(NNS_FINALROM)
    #define             NNS_FndGetFillValForHeap(type)      (0)
#else
    u32                 NNS_FndGetFillValForHeap(
                            int     type);
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_FND_HEAPCOMMON_H_ */
#endif

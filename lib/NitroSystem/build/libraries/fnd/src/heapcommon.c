/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - fnd
  File:     heapcommon.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.15 $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nnsys/misc.h>
#include <nnsys/fnd/heapcommon.h>
#include <nnsys/fnd/expheap.h>
#include <nnsys/fnd/frameheap.h>
#include <nnsys/fnd/unitheap.h>
#include <nnsys/fnd/config.h>
#include "heapcommoni.h"


/* ========================================================================
    static�ϐ�
   ======================================================================== */

/* ------------------------------------------------------------------------
    ���X�g�֘A
   ------------------------------------------------------------------------ */

static NNSFndList sRootList;                // ���[�g�̃q�[�v���X�g
static BOOL sRootListInitialized = FALSE;   // sRootList������������Ă���ΐ^


/* ------------------------------------------------------------------------
    �t�B���֘A
   ------------------------------------------------------------------------ */

#if ! defined(NNS_FINALROM)

    static u32 sFillVals[NNS_FND_HEAP_FILL_MAX] =
    {
        0xC3C3C3C3, // �q�[�v�쐬���ɖ��߂�l
        0xF3F3F3F3, // �������u���b�N�m�ێ��ɖ��߂�l
        0xD3D3D3D3, // �������u���b�N������ɖ��߂�l
    };

// #if ! defined(NNS_FINALROM)
#endif


/* ========================================================================
    static�֐�
   ======================================================================== */

/* ------------------------------------------------------------------------
    ���X�g�֘A
   ------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*
  Name:         FindContainHeap

  Description:  �w�肳�ꂽ�������u���b�N���ܗL����q�[�v�����X�g����ċA�I��
                �T���o���܂��B

  Arguments:    pList:     ���X�g�ւ̃|�C���^
                memBlock:  �������u���b�N�ւ̃|�C���^

  Returns:      �w�肳�ꂽ�������u���b�N���m�ۂ����q�[�v��������΁A
                ���̃q�[�v�ւ̃|�C���^��Ԃ��܂��B
                ������Ȃ��������� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNSiFndHeapHead*
FindContainHeap(
    NNSFndList* pList,
    const void* memBlock
)
{
    NNSiFndHeapHead* pHeapHd = NULL;
    while (NULL != (pHeapHd = NNS_FndGetNextListObject(pList, pHeapHd)))
    {
        if ( NNSiGetUIntPtr(pHeapHd->heapStart) <= NNSiGetUIntPtr(memBlock)
         &&  NNSiGetUIntPtr(memBlock) < NNSiGetUIntPtr(pHeapHd->heapEnd)
        )
        {
            NNSiFndHeapHead* pChildHeapHd = FindContainHeap(&pHeapHd->childList, memBlock);
            if(pChildHeapHd)
            {
                return pChildHeapHd;
            }

            return pHeapHd;
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         FindListContainHeap

  Description:  �q�[�v���ܗL����e�q�[�v���������A���̐e�q�[�v�̃��X�g�ւ�
                �|�C���^��Ԃ��܂��B

  Arguments:    pHeapHd:  �����Ώۂ̃q�[�v�̃w�b�_�ւ̃|�C���^�B

  Returns:      �w�肵���q�[�v���ܗL����e�q�[�v��������΁A
                �e�q�[�v�̎q���X�g�ւ̃|�C���^��Ԃ��܂��B
                �e�q�[�v��������Ȃ���΃��[�g���X�g�ւ̃|�C���^���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
static NNSFndList*
FindListContainHeap(NNSiFndHeapHead* pHeapHd)
{
    NNSFndList* pList = &sRootList;

    NNSiFndHeapHead* pContainHeap = FindContainHeap(&sRootList, pHeapHd);

    if(pContainHeap)
    {
        pList = &pContainHeap->childList;
    }

    return pList;
}

#if 1
    static NNS_FND_INLINE void
    DumpHeapList() {}
#else
    static void
    DumpHeapList()
    {
        NNSiFndHeapHead* pHeapHd = NULL;
        int count = 0;

        OS_Printf("Dump Heap List\n");
        while (NULL != (pHeapHd = NNS_FndGetNextListObject(&sRootList, pHeapHd)))
        {
            count++;
            OS_Printf("[%d] -> %p %08X\n", count, pHeapHd, pHeapHd->signature);
        }
    }
#endif

/* ========================================================================
    �O���֐�(����J)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndInitHeapHead

  Description:  �q�[�v�w�b�_�̏��������s���܂��B

  Arguments:    pHeapHd:    �q�[�v�w�b�_�ւ̃|�C���^�B
                signature:  �V�O�l�`���B
                heapStart:  �q�[�v�������̊J�n�A�h���X�B
                heapEnd:    �q�[�v�������̏I���A�h���X +1�B
                optFlag:    �q�[�v�I�v�V�����B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNSi_FndInitHeapHead(
    NNSiFndHeapHead*    pHeapHd,
    u32                 signature,
    void*               heapStart,
    void*               heapEnd,
    u16                 optFlag
)
{
    pHeapHd->signature = signature;

    pHeapHd->heapStart = heapStart;
    pHeapHd->heapEnd   = heapEnd;

    pHeapHd->attribute = 0;
    SetOptForHeap(pHeapHd, optFlag);

    FillNoUseMemory(
        pHeapHd,
        heapStart,
        GetOffsetFromPtr(heapStart, heapEnd));

    NNS_FND_INIT_LIST(&pHeapHd->childList, NNSiFndHeapHead, link);

    // �q�[�v�̃��X�g����
    if(! sRootListInitialized)
    {
        NNS_FND_INIT_LIST(&sRootList, NNSiFndHeapHead, link);
        sRootListInitialized = TRUE;
    }

    NNS_FndAppendListObject(FindListContainHeap(pHeapHd), pHeapHd);
    DumpHeapList();

}

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndFinalizeHeap

  Description:  �q�[�v���ʂ̌�n�����s���܂��B

  Arguments:    pHeapHd:  �q�[�v�w�b�_�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNSi_FndFinalizeHeap(NNSiFndHeapHead* pHeapHd)
{
    // �q�[�v�̃��X�g����
    NNS_FndRemoveListObject(FindListContainHeap(pHeapHd), pHeapHd);
    DumpHeapList();
}


/*---------------------------------------------------------------------------*
  Name:         NNSi_FndDumpHeapHead

  Description:  �q�[�v�w�b�_�̏���\�����܂��B

  Arguments:    pHeapHd:  �q�[�v�w�b�_�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNSi_FndDumpHeapHead(NNSiFndHeapHead* pHeapHd)
{
    OS_Printf("[NNS Foundation ");

    switch(pHeapHd->signature)
    {
    case NNSI_EXPHEAP_SIGNATURE: OS_Printf("Exp");   break;
    case NNSI_FRMHEAP_SIGNATURE: OS_Printf("Frame"); break;
    case NNSI_UNTHEAP_SIGNATURE: OS_Printf("Unit");  break;
    default:
        NNS_ASSERT(FALSE);
    }

    OS_Printf(" Heap]\n");

    OS_Printf("    whole [%p - %p)\n", pHeapHd, pHeapHd->heapEnd);
}


/* ========================================================================
    �O���֐�(���J)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNS_FndFindContainHeap

  Description:  �������u���b�N���ܗL����q�[�v���������܂��B

  Arguments:    memBlock:  �����Ώۂ̃������u���b�N�B

  Returns:      �w�肵���������u���b�N���܂ރq�[�v��������΁A
                ���̃q�[�v�̃n���h����Ԃ��܂��B
                ������Ȃ���΁ANNS_FND_HEAP_INVALID_HANDLE ���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
NNSFndHeapHandle
NNS_FndFindContainHeap(const void* memBlock)
{
    return FindContainHeap(&sRootList, memBlock);
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndDumpHeap

  Description:  �q�[�v�����̏���\�����܂��B
                ����̓f�o�b�O�p�̊֐��ł��B

  Arguments:    heap:    �t���[���q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
#if ! defined(NNS_FINALROM)

    void
    NNS_FndDumpHeap(NNSFndHeapHandle heap)
    {
        NNSiFndHeapHead* pHeapHd = heap;
        switch(pHeapHd->signature)
        {
        case NNSI_EXPHEAP_SIGNATURE: NNSi_FndDumpExpHeap(heap);  break;
        case NNSI_FRMHEAP_SIGNATURE: NNSi_FndDumpFrmHeap(heap);  break;
        case NNSI_UNTHEAP_SIGNATURE: NNSi_FndDumpUnitHeap(heap); break;
        default:
            OS_Printf("[NNS Foundation] dump heap : unknown heap. - %p\n", heap);
        }
    }

// #if ! defined(NNS_FINALROM)
#endif

/*---------------------------------------------------------------------------*
  Name:         NNS_FndSetFillValForHeap

  Description:  �q�[�v�̍쐬���⃁�����u���b�N�̊m�ہE������Ƀ�������
                �Z�b�g����l���Z�b�g���܂��B
                ���̊֐��̓f�o�b�O�p�̊֐��ł��B
                �ŏIROM��(FINALROM)���C�u�����ł͏��0��Ԃ��܂��B

  Arguments:    type:  �擾����l�̎��
                val:   �Z�b�g����l

  Returns:      �ȑO�́A�������u���b�N�̊m�ێ��Ƀ������ɃZ�b�g����l��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

#if ! defined(NNS_FINALROM)

    u32
    NNS_FndSetFillValForHeap(
        int     type,
        u32     val
    )
    {
        NNS_ASSERT(type < NNS_FND_HEAP_FILL_MAX);

        {
            u32 oldVal = sFillVals[type];
            sFillVals[type] = val;
            return oldVal;
        }
    }

// #if ! defined(NNS_FINALROM)
#endif


/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetFillValForHeap

  Description:  �q�[�v�̍쐬���⃁�����u���b�N�̊m�ہE������Ƀ�������
                �Z�b�g����l���擾���܂��B
                ���̊֐��̓f�o�b�O�p�̊֐��ł��B
                �ŏIROM��(FINALROM)���C�u�����ł͏��0��Ԃ��܂��B

  Arguments:    type:  �擾����l�̎��

  Returns:      �w�肳�ꂽ��ނ̃������ɃZ�b�g����l��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

#if ! defined(NNS_FINALROM)

    u32
    NNS_FndGetFillValForHeap(int type)
    {
        NNS_ASSERT(type < NNS_FND_HEAP_FILL_MAX);

        return sFillVals[type];
    }

// #if ! defined(NNS_FINALROM)
#endif

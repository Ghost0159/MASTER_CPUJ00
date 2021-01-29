/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - fnd - heap
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nnsys/fnd.h>

#define HeapBufElementNum (4096/sizeof(u32))

static u32 sHeapBuf[HeapBufElementNum];

/*
    �O���[�vID�ƃT�C�Y�̍��v��ێ�����\���́B

        �֐�CalcGroupSizeForExpHeap()�ɂāA�����O���[�vID�̃������u���b�N�̃T�C�Y��
        �J�E���g����̂Ɏg�p���܂��B
 */
typedef struct TGroupSize TGroupSize;

struct TGroupSize
{
    u16     groupID;
    u16		padding;
    u32     size;
};

/*---------------------------------------------------------------------------*
  Name:         CalcGroupSizeForMBlockExpHeap

  Description:  NNS_FndVisitAllocatedForExpHeap()����e�������u���b�N�ɑ΂���
                �Ă΂���֐��B
                �������u���b�N�̃O���[�vID���w�肳�ꂽ���̂ƈ�v����ꍇ�A
                �������u���b�N�̃T�C�Y�����v�l�ɉ����܂��B

  Arguments:    memBlock:   �������u���b�N�B
                heap:       �g���q�[�v�ւ̃n���h���B
                userParam:  �O���[�vID�ƍ��v�l�����\���̂ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
CalcGroupSizeForMBlockExpHeap(
    void*               memBlock,
    NNSFndHeapHandle    heap,
    u32                 userParam
)
{
    #pragma unused(heap)

    TGroupSize* pGrSize = (TGroupSize*)userParam;

    if (NNS_FndGetGroupIDForMBlockExpHeap(memBlock) == pGrSize->groupID)
    {
        pGrSize->size += NNS_FndGetSizeForMBlockExpHeap(memBlock);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CalcGroupSizeForExpHeap

  Description:  �g���q�[�v�̃������u���b�N�̒��ŁA�w�肵���O���[�vID������
                �������u���b�N�̃T�C�Y�����v���܂��B

  Arguments:    heap: �g���q�[�v�̃n���h���B

  Returns:      �w�肵���O���[�vID�����������u���b�N�̃T�C�Y�̍��v�l���Ԃ�܂��B�B
 *---------------------------------------------------------------------------*/
static u32
CalcGroupSizeForExpHeap(
    NNSFndHeapHandle    heap,
    u16                 groupID
)
{
    TGroupSize groupSize;

    groupSize.groupID = groupID;
    groupSize.size = 0;

    NNS_FndVisitAllocatedForExpHeap(
        heap,
        CalcGroupSizeForMBlockExpHeap,
        (u32)&groupSize);

    return groupSize.size;
}

/*---------------------------------------------------------------------------*
  Name:         FreeDirForMBlockExpHeap

  Description:  NNS_FndVisitAllocatedForExpHeap()����e�������u���b�N�ɑ΂���
                �Ă΂���֐��B
                �������u���b�N�̊m�ۂ��ꂽ�������AuserParam�Ŏw�肵���l�Ɠ����Ȃ�
                ������܂��B

  Arguments:    memBlock:   �������u���b�N�B
                heap:       �g���q�[�v�̃n���h���B
                userParam:  �������u���b�N�̊m�ە����B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
FreeDirForMBlockExpHeap(
    void*               memBlock,
    NNSFndHeapHandle    heap,
    u32                 userParam
)
{
    if (NNS_FndGetAllocDirForMBlockExpHeap(memBlock) == userParam)
    {
        NNS_FndFreeToExpHeap(heap, memBlock);
    }
}

/*---------------------------------------------------------------------------*
  Name:         FreeTailForExpHeap

  Description:  �g���q�[�v�̈�̌������m�ۂ����������u���b�N��S�ĉ�����܂��B

  Arguments:    heap: �g���q�[�v�̃n���h���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
FreeTailForExpHeap(NNSFndHeapHandle heap)
{
    NNS_FndVisitAllocatedForExpHeap(
        heap,
        FreeDirForMBlockExpHeap,
        NNS_FND_EXPHEAP_ALLOC_DIR_REAR);
}


/*---------------------------------------------------------------------------*
  Name:         SampleExpHeap

  Description:  �g���q�[�v�̃T���v���ł��B

  Arguments:    heapAddress:  �q�[�v�Ɋ��蓖�Ă郁�����̐擪�A�h���X�B
                heapSize:     �q�[�v�Ɋ��蓖�Ă郁�����̃T�C�Y�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
SampleExpHeap(
    void*   heapAddress,
    u32     heapSize)
{
    void* pMBlocks[4];

    // �g���q�[�v�̍쐬
    NNSFndHeapHandle hExpHeap = NNS_FndCreateExpHeap(heapAddress, heapSize);

    // �ʏ�̃������u���b�N�m��
    pMBlocks[0] = NNS_FndAllocFromExpHeap(hExpHeap, 100);

    // �q�[�v�̈�̌������m�ہB�A���C�����g�̓f�t�H���g�l 4�B
    pMBlocks[2] = NNS_FndAllocFromExpHeapEx(hExpHeap, 100, -NNS_FND_HEAP_DEFAULT_ALIGNMENT);

    // �O���[�vID �� 1 �ɕύX
    (void)NNS_FndSetGroupIDForExpHeap(hExpHeap, 1);

    // �������u���b�N�m�ہB�A���C�����g�� 16�B
    pMBlocks[1] = NNS_FndAllocFromExpHeapEx(hExpHeap, 200, 16);

    // �q�[�v�̈�̌������m�ہB�A���C�����g�� 16�B
    pMBlocks[3] = NNS_FndAllocFromExpHeapEx(hExpHeap, 200, -16);

    NNS_FndDumpHeap(hExpHeap);

    // �O���[�v 0 �̃������u���b�N�̃T�C�Y�̍��v�l�����߂�B
    OS_Printf("[demo exp heap] group 0 size %d\n", CalcGroupSizeForExpHeap(hExpHeap, 0));

    // �O���[�v 1 �̃������u���b�N�̃T�C�Y�̍��v�l�����߂�B
    OS_Printf("[demo exp heap] group 1 size %d\n", CalcGroupSizeForExpHeap(hExpHeap, 1));

    OS_Printf("\n");

    // �������u���b�N�̊g�� 100 -> 300
    (void)NNS_FndResizeForMBlockExpHeap(hExpHeap, pMBlocks[1], 300);

    // �q�[�v�̈�̌������m�ۂ����������u���b�N�����
    FreeTailForExpHeap(hExpHeap);

    NNS_FndDumpHeap(hExpHeap);

    // �g���q�[�v�̔j��
    NNS_FndDestroyExpHeap(hExpHeap);
}


/*---------------------------------------------------------------------------*
  Name:         SampleFrameHeap

  Description:  �t���[���q�[�v�̃T���v���ł��B

  Arguments:    heapAddress:  �q�[�v�Ɋ��蓖�Ă郁�����̐擪�A�h���X�B
                heapSize:     �q�[�v�Ɋ��蓖�Ă郁�����̃T�C�Y�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
SampleFrameHeap(
    void*   heapAddress,
    u32     heapSize)
{
    void* pExpMBlock;
    void* pMBlocks[8];
    NNSFndHeapHandle hExpHeap;
    NNSFndHeapHandle hFrmHeap;
    u32 frmHeapSize;

    /*
        ��U�g���q�[�v�����A���̒��Ƀt���[���q�[�v�����܂��B
     */
        // �g���q�[�v�̍쐬
        hExpHeap = NNS_FndCreateExpHeap(heapAddress, heapSize);

        // �m�ۉ\�T�C�Y�S�Ă��L���郁�����u���b�N�����
        pExpMBlock = NNS_FndAllocFromExpHeap(
                        hExpHeap,
                        NNS_FndGetAllocatableSizeForExpHeap(hExpHeap));

        // �t���[���q�[�v�̍쐬
        hFrmHeap = NNS_FndCreateFrmHeap(
                        pExpMBlock,
                        NNS_FndGetSizeForMBlockExpHeap(pExpMBlock));

    /*
        �������u���b�N�̊m�ہA��Ԃ̕ۑ��A���A���s���܂��B
     */
        // �ʏ�̃������u���b�N�m��
        pMBlocks[0] = NNS_FndAllocFromFrmHeap(hFrmHeap, 100);

        // �q�[�v�̈�̌������m�ہB�A���C�����g�̓f�t�H���g�l 4�B
        pMBlocks[1] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 100, -NNS_FND_HEAP_DEFAULT_ALIGNMENT);

        // �������u���b�N�m�ۏ�Ԃ̕ۑ��B�^�O��'FRST'�B
        (void)NNS_FndRecordStateForFrmHeap(hFrmHeap, 'FRST');

        // �������u���b�N��O���ƌ������m�ہB�A���C�����g�� 8�B
        pMBlocks[2] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 200,  8);
        pMBlocks[3] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 200, -8);

        // �������u���b�N�m�ۏ�Ԃ̕ۑ��B�^�O��'SCND'�B
        (void)NNS_FndRecordStateForFrmHeap(hFrmHeap, 'SCND');

        // �������u���b�N��O���ƌ������m�ہB�A���C�����g�� 16�B
        pMBlocks[4] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 300,  16);
        pMBlocks[5] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 300, -16);

        // �������u���b�N�m�ۏ�Ԃ̕ۑ��B�^�O��'THRD'�B
        (void)NNS_FndRecordStateForFrmHeap(hFrmHeap, 'THRD');

        // �������u���b�N��O���ƌ������m�ہB�A���C�����g�� 32�B
        pMBlocks[6] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 300,  32);
        pMBlocks[7] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 300, -32);

        NNS_FndDumpHeap(hFrmHeap);

        // �^�O���w�肵�āA��ԕ��A
        (void)NNS_FndFreeByStateToFrmHeap(hFrmHeap, 'SCND');

        NNS_FndDumpHeap(hFrmHeap);

        // �^�O���w�肵�Ȃ��ŁA��ԕ��A('FRST'�̏�Ԃ֕��A)
        (void)NNS_FndFreeByStateToFrmHeap(hFrmHeap, 0);

        NNS_FndDumpHeap(hFrmHeap);

    /*
        �q�[�v�̈�̌������m�ۂ����������u���b�N��������A�t���[���q�[�v�̈���k�����܂��B
     */
        // �q�[�v�̌������m�ۂ����������u���b�N�����
        (void)NNS_FndFreeToFrmHeap(hFrmHeap, NNS_FND_FRMHEAP_FREE_TAIL);

        // �t���[���q�[�v���k��
        frmHeapSize = NNS_FndAdjustFrmHeap(hFrmHeap);

        // �t���[���q�[�v�̏k���ɂ��킹�āA�g���q�[�v�̃������u���b�N���k�����܂��B
        (void)NNS_FndResizeForMBlockExpHeap(hExpHeap, pExpMBlock, frmHeapSize);

        // �������u���b�N��ێ�����q�[�v���擾���Adump���܂��B
        // (�t���[���q�[�v:hFrmHeap�̓��e���\������܂��B)
        NNS_FndDumpHeap(NNS_FndFindContainHeap(pMBlocks[0]));

        // �g���q�[�v��Dump���܂��B
        NNS_FndDumpHeap(hExpHeap);

    /*
        ��n��
     */
        // �t���[���q�[�v�̔j��
        NNS_FndDestroyFrmHeap(hFrmHeap);

        // �g���q�[�v�̔j��
        NNS_FndDestroyExpHeap(hExpHeap);
}


/*---------------------------------------------------------------------------*
  Name:         SampleUnitHeap

  Description:  ���j�b�g�q�[�v�̃T���v���ł��B

  Arguments:    heapAddress:  �q�[�v�Ɋ��蓖�Ă郁�����̐擪�A�h���X�B
                heapSize:     �q�[�v�Ɋ��蓖�Ă郁�����̃T�C�Y�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
SampleUnitHeap(
    void*   heapAddress,
    u32     heapSize)
{
    void* pMBlocks[4];

    // ���j�b�g�q�[�v�̍쐬
    NNSFndHeapHandle hUnitHeap = NNS_FndCreateUnitHeapEx(
                                    heapAddress,
                                    heapSize,
                                    64,         // �������u���b�N�̃T�C�Y
                                    16,         // �A���C�����g
                                    0);         // �I�v�V����

    // �m�ۉ\�ȃ������u���b�N�̌���\��
    OS_Printf("[demo unit heap] free block num %d.\n", NNS_FndCountFreeBlockForUnitHeap(hUnitHeap));

    // �������u���b�N�m��
    pMBlocks[0] = NNS_FndAllocFromUnitHeap(hUnitHeap);
    pMBlocks[1] = NNS_FndAllocFromUnitHeap(hUnitHeap);
    pMBlocks[2] = NNS_FndAllocFromUnitHeap(hUnitHeap);
    pMBlocks[3] = NNS_FndAllocFromUnitHeap(hUnitHeap);

    // 3�Ԗڂ̃������u���b�N�����
    NNS_FndFreeToUnitHeap(hUnitHeap, pMBlocks[2]);

    // �m�ۉ\�ȃ������u���b�N�̌���\��
    OS_Printf("[demo unit heap] free block num %d.\n", NNS_FndCountFreeBlockForUnitHeap(hUnitHeap));

    NNS_FndDumpHeap(hUnitHeap);

    // ���j�b�g�q�[�v�̔j��
    NNS_FndDestroyUnitHeap(hUnitHeap);
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain
 *---------------------------------------------------------------------------*/
void
NitroMain(void)
{
    OS_Init();

	SampleExpHeap(sHeapBuf, sizeof sHeapBuf);
	SampleFrameHeap(sHeapBuf, sizeof sHeapBuf);
	SampleUnitHeap(sHeapBuf, sizeof sHeapBuf);

    while(1) {}
}

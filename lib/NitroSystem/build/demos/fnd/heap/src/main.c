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
    グループIDとサイズの合計を保持する構造体。

        関数CalcGroupSizeForExpHeap()にて、同じグループIDのメモリブロックのサイズを
        カウントするのに使用します。
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

  Description:  NNS_FndVisitAllocatedForExpHeap()から各メモリブロックに対して
                呼ばせる関数。
                メモリブロックのグループIDが指定されたものと一致する場合、
                メモリブロックのサイズを合計値に加えます。

  Arguments:    memBlock:   メモリブロック。
                heap:       拡張ヒープへのハンドル。
                userParam:  グループIDと合計値を持つ構造体へのポインタ。

  Returns:      なし。
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

  Description:  拡張ヒープのメモリブロックの中で、指定したグループIDを持つ
                メモリブロックのサイズを合計します。

  Arguments:    heap: 拡張ヒープのハンドル。

  Returns:      指定したグループIDを持つメモリブロックのサイズの合計値が返ります。。
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

  Description:  NNS_FndVisitAllocatedForExpHeap()から各メモリブロックに対して
                呼ばせる関数。
                メモリブロックの確保された方向が、userParamで指定した値と同じなら
                解放します。

  Arguments:    memBlock:   メモリブロック。
                heap:       拡張ヒープのハンドル。
                userParam:  メモリブロックの確保方向。

  Returns:      なし。
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

  Description:  拡張ヒープ領域の後方から確保したメモリブロックを全て解放します。

  Arguments:    heap: 拡張ヒープのハンドル。

  Returns:      なし。
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

  Description:  拡張ヒープのサンプルです。

  Arguments:    heapAddress:  ヒープに割り当てるメモリの先頭アドレス。
                heapSize:     ヒープに割り当てるメモリのサイズ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
SampleExpHeap(
    void*   heapAddress,
    u32     heapSize)
{
    void* pMBlocks[4];

    // 拡張ヒープの作成
    NNSFndHeapHandle hExpHeap = NNS_FndCreateExpHeap(heapAddress, heapSize);

    // 通常のメモリブロック確保
    pMBlocks[0] = NNS_FndAllocFromExpHeap(hExpHeap, 100);

    // ヒープ領域の後方から確保。アライメントはデフォルト値 4。
    pMBlocks[2] = NNS_FndAllocFromExpHeapEx(hExpHeap, 100, -NNS_FND_HEAP_DEFAULT_ALIGNMENT);

    // グループID を 1 に変更
    (void)NNS_FndSetGroupIDForExpHeap(hExpHeap, 1);

    // メモリブロック確保。アライメントは 16。
    pMBlocks[1] = NNS_FndAllocFromExpHeapEx(hExpHeap, 200, 16);

    // ヒープ領域の後方から確保。アライメントは 16。
    pMBlocks[3] = NNS_FndAllocFromExpHeapEx(hExpHeap, 200, -16);

    NNS_FndDumpHeap(hExpHeap);

    // グループ 0 のメモリブロックのサイズの合計値を求める。
    OS_Printf("[demo exp heap] group 0 size %d\n", CalcGroupSizeForExpHeap(hExpHeap, 0));

    // グループ 1 のメモリブロックのサイズの合計値を求める。
    OS_Printf("[demo exp heap] group 1 size %d\n", CalcGroupSizeForExpHeap(hExpHeap, 1));

    OS_Printf("\n");

    // メモリブロックの拡張 100 -> 300
    (void)NNS_FndResizeForMBlockExpHeap(hExpHeap, pMBlocks[1], 300);

    // ヒープ領域の後方から確保したメモリブロックを解放
    FreeTailForExpHeap(hExpHeap);

    NNS_FndDumpHeap(hExpHeap);

    // 拡張ヒープの破棄
    NNS_FndDestroyExpHeap(hExpHeap);
}


/*---------------------------------------------------------------------------*
  Name:         SampleFrameHeap

  Description:  フレームヒープのサンプルです。

  Arguments:    heapAddress:  ヒープに割り当てるメモリの先頭アドレス。
                heapSize:     ヒープに割り当てるメモリのサイズ。

  Returns:      なし。
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
        一旦拡張ヒープを作り、その中にフレームヒープを作ります。
     */
        // 拡張ヒープの作成
        hExpHeap = NNS_FndCreateExpHeap(heapAddress, heapSize);

        // 確保可能サイズ全てを占有するメモリブロックを作る
        pExpMBlock = NNS_FndAllocFromExpHeap(
                        hExpHeap,
                        NNS_FndGetAllocatableSizeForExpHeap(hExpHeap));

        // フレームヒープの作成
        hFrmHeap = NNS_FndCreateFrmHeap(
                        pExpMBlock,
                        NNS_FndGetSizeForMBlockExpHeap(pExpMBlock));

    /*
        メモリブロックの確保、状態の保存、復帰を行います。
     */
        // 通常のメモリブロック確保
        pMBlocks[0] = NNS_FndAllocFromFrmHeap(hFrmHeap, 100);

        // ヒープ領域の後方から確保。アライメントはデフォルト値 4。
        pMBlocks[1] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 100, -NNS_FND_HEAP_DEFAULT_ALIGNMENT);

        // メモリブロック確保状態の保存。タグは'FRST'。
        (void)NNS_FndRecordStateForFrmHeap(hFrmHeap, 'FRST');

        // メモリブロックを前方と後方から確保。アライメントは 8。
        pMBlocks[2] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 200,  8);
        pMBlocks[3] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 200, -8);

        // メモリブロック確保状態の保存。タグは'SCND'。
        (void)NNS_FndRecordStateForFrmHeap(hFrmHeap, 'SCND');

        // メモリブロックを前方と後方から確保。アライメントは 16。
        pMBlocks[4] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 300,  16);
        pMBlocks[5] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 300, -16);

        // メモリブロック確保状態の保存。タグは'THRD'。
        (void)NNS_FndRecordStateForFrmHeap(hFrmHeap, 'THRD');

        // メモリブロックを前方と後方から確保。アライメントは 32。
        pMBlocks[6] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 300,  32);
        pMBlocks[7] = NNS_FndAllocFromFrmHeapEx(hFrmHeap, 300, -32);

        NNS_FndDumpHeap(hFrmHeap);

        // タグを指定して、状態復帰
        (void)NNS_FndFreeByStateToFrmHeap(hFrmHeap, 'SCND');

        NNS_FndDumpHeap(hFrmHeap);

        // タグを指定しないで、状態復帰('FRST'の状態へ復帰)
        (void)NNS_FndFreeByStateToFrmHeap(hFrmHeap, 0);

        NNS_FndDumpHeap(hFrmHeap);

    /*
        ヒープ領域の後方から確保したメモリブロックを解放し、フレームヒープ領域を縮小します。
     */
        // ヒープの後方から確保したメモリブロックを解放
        (void)NNS_FndFreeToFrmHeap(hFrmHeap, NNS_FND_FRMHEAP_FREE_TAIL);

        // フレームヒープを縮小
        frmHeapSize = NNS_FndAdjustFrmHeap(hFrmHeap);

        // フレームヒープの縮小にあわせて、拡張ヒープのメモリブロックを縮小します。
        (void)NNS_FndResizeForMBlockExpHeap(hExpHeap, pExpMBlock, frmHeapSize);

        // メモリブロックを保持するヒープを取得し、dumpします。
        // (フレームヒープ:hFrmHeapの内容が表示されます。)
        NNS_FndDumpHeap(NNS_FndFindContainHeap(pMBlocks[0]));

        // 拡張ヒープをDumpします。
        NNS_FndDumpHeap(hExpHeap);

    /*
        後始末
     */
        // フレームヒープの破棄
        NNS_FndDestroyFrmHeap(hFrmHeap);

        // 拡張ヒープの破棄
        NNS_FndDestroyExpHeap(hExpHeap);
}


/*---------------------------------------------------------------------------*
  Name:         SampleUnitHeap

  Description:  ユニットヒープのサンプルです。

  Arguments:    heapAddress:  ヒープに割り当てるメモリの先頭アドレス。
                heapSize:     ヒープに割り当てるメモリのサイズ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
SampleUnitHeap(
    void*   heapAddress,
    u32     heapSize)
{
    void* pMBlocks[4];

    // ユニットヒープの作成
    NNSFndHeapHandle hUnitHeap = NNS_FndCreateUnitHeapEx(
                                    heapAddress,
                                    heapSize,
                                    64,         // メモリブロックのサイズ
                                    16,         // アライメント
                                    0);         // オプション

    // 確保可能なメモリブロックの個数を表示
    OS_Printf("[demo unit heap] free block num %d.\n", NNS_FndCountFreeBlockForUnitHeap(hUnitHeap));

    // メモリブロック確保
    pMBlocks[0] = NNS_FndAllocFromUnitHeap(hUnitHeap);
    pMBlocks[1] = NNS_FndAllocFromUnitHeap(hUnitHeap);
    pMBlocks[2] = NNS_FndAllocFromUnitHeap(hUnitHeap);
    pMBlocks[3] = NNS_FndAllocFromUnitHeap(hUnitHeap);

    // 3番目のメモリブロックを解放
    NNS_FndFreeToUnitHeap(hUnitHeap, pMBlocks[2]);

    // 確保可能なメモリブロックの個数を表示
    OS_Printf("[demo unit heap] free block num %d.\n", NNS_FndCountFreeBlockForUnitHeap(hUnitHeap));

    NNS_FndDumpHeap(hUnitHeap);

    // ユニットヒープの破棄
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

/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - fnd - archive2
  File:     nns_util.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

#include "nns_util.h"

#define SYSTEM_HEAP_SIZE        64*1024

GamePad          gGamePad;      // ゲームパッド
NNSFndHeapHandle gSysHeap;      // システムヒープ　　　　（拡張ヒープを使用）
NNSFndHeapHandle gAppHeap;      // アプリケーションヒープ（拡張ヒープを使用）


/*---------------------------------------------------------------------------*
  Name:         InitMemory

  Description:  システムヒープとアプリケーションヒープを作成します。両ヒープ
                にはNITRO-Systemの拡張ヒープを使用しています。
                
                システムヒープ用のメモリとして、SYSTEM_HEAP_SIZE分をメインア
                リーナから確保し、メインアリーナの残りを全てアプリケーション
                ヒープ用のメモリに確保しています。
                
                システムヒープは、ゲームシステム等のシステムプログラムで使用
                することを想定しています。アプリケーションヒープには、ゲーム
                で使用するデータをロードする為に使用します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
InitMemory(void)
{
    void* sysHeapMemory = OS_AllocFromMainArenaLo(SYSTEM_HEAP_SIZE, 16);

    u32   arenaLow      = ROUND_UP  (OS_GetMainArenaLo(), 16);
    u32   arenaHigh     = ROUND_DOWN(OS_GetMainArenaHi(), 16);
    u32   appHeapSize   = arenaHigh - arenaLow;
    void* appHeapMemory = OS_AllocFromMainArenaLo(appHeapSize, 16);

    gSysHeap = NNS_FndCreateExpHeap(sysHeapMemory, SYSTEM_HEAP_SIZE);
    gAppHeap = NNS_FndCreateExpHeap(appHeapMemory, appHeapSize     );
}




/*---------------------------------------------------------------------------*
  Name:         LoadCompressedArchive

  Description:  パス名で指定された圧縮されたアーカイブをメモリに展開し、
                ファイルシステムにマウントします。圧縮には、ntrcomp.exeを
                使用しています。

                アーカイブを読み込む為のメモリと、アーカイブをマウントする為
                に必要なNNSFndArchive 構造体用のメモリは、アプリケーションヒ
                ープから確保します。

                圧縮データは、拡張ヒープの後ろ側から確保しています。展開後
                のデータは、拡張ヒープの前側から確保しますので、圧縮データ
                解放後にフラグメンテーションが発生しません。

  Arguments:    name:   アーカイブをファイルシステム上で識別する為の名前。
                path:   アーカイブのパス名。

  Returns:      アーカイブのロードに成功すれば、NNSFndArchive 構造体へのポイ
                ンタを返します。失敗した場合には、NULLを返します。
 *---------------------------------------------------------------------------*/
NNSFndArchive*
LoadCompressedArchive(const char* name, const char* path)
{
    FSFile          file;
    u8*             compData;
    NNSFndArchive*  archive = NULL;

    FS_InitFile(&file);
    if (FS_OpenFile(&file, path))
    {
        u32 fileSize = FS_GetLength(&file);

        // 圧縮データを格納する為のメモリは、拡張ヒープの後ろから確保します。
        // （アライメントに負数を指定すると、後ろから確保します。）
        compData = NNS_FndAllocFromExpHeapEx(gAppHeap, fileSize, -16);
        
        if (compData == NULL)
        {
            (void)FS_CloseFile(&file);
            return NULL;
        }
        if (FS_ReadFile(&file, compData, (s32)fileSize) != fileSize)
        {
            NNS_FndFreeToExpHeap(gAppHeap, compData);
            (void)FS_CloseFile(&file);
            return NULL;
        }
        (void)FS_CloseFile(&file);
    }

    {
        // NNSFndArchive構造体とアーカイブデータを、一つのメモリブロックに格納します。
        u32 memorySize = ROUND_UP(sizeof(NNSFndArchive), 16) + ROUND_UP(MI_GetUncompressedSize(compData), 16);

        // 展開後のアーカイブデータは、ヒープの前から確保します。
        u8* memory = NNS_FndAllocFromExpHeapEx(gAppHeap, memorySize, 16);

        if (memory != NULL)
        {
            u8* binary = memory + ROUND_UP(sizeof(NNSFndArchive), 16);

            // アーカイブを展開します
            switch (MI_GetCompressionType(compData))
            {
                case MI_COMPRESSION_LZ      : MI_UncompressLZ16   (compData, binary);       break;
                case MI_COMPRESSION_HUFFMAN : MI_UncompressHuffman(compData, binary);       break;
                case MI_COMPRESSION_RL      : MI_UncompressRL16   (compData, binary);       break;
                default                     : SDK_ASSERT(FALSE);                            break;
            }
            DC_FlushRange(binary, MI_GetUncompressedSize(compData));

            if (NNS_FndMountArchive((NNSFndArchive*)memory, name, binary))
            {
                archive = (NNSFndArchive*)memory;
            }
            else
            {
                NNS_FndFreeToExpHeap(gAppHeap, memory);
            }
        }
        // 圧縮データを解放します。
        NNS_FndFreeToExpHeap(gAppHeap, compData);
    }
    return archive;
}

/*---------------------------------------------------------------------------*
  Name:         RemoveArchive

  Description:  指定されたアーカイブをメモリから削除します。
  
                指定されたアーカイブをファイルシステムからアンマウントし、ア
                ーカイブが読み込まれていたメモリを解放します。
                
  Arguments:    archive:    NNSアーカイブ構造体へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
RemoveArchive(NNSFndArchive* archive)
{
    (void)NNS_FndUnmountArchive(archive);
    NNS_FndFreeToExpHeap(gAppHeap, archive);
}

/*---------------------------------------------------------------------------*
  Name:         ReadGamePad

  Description:  キーを読み込み、トリガとリリーストリガを求めます。
                
  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void ReadGamePad(void)
{
    u16 status = PAD_Read();

    gGamePad.trigger = (u16)(status          & (status ^ gGamePad.button));
    gGamePad.release = (u16)(gGamePad.button & (status ^ gGamePad.button));
    gGamePad.button  = status;
}

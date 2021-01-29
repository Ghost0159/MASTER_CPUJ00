/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - fnd - archive
  File:     nns_util.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
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
  Name:         LoadArchive

  Description:  パス名で指定されたアーカイブをメモリに読み込み、ファイルシス
                テムにマウントします。この関数の実装では、DC_FlushRange 関数
                を呼び出す事により、読み込まれたアーカイブがメインメモリ上に
                存在する事が保証されます。

                アーカイブを読み込む為のメモリと、アーカイブをマウントする為
                に必要なNNSFndArchive 構造体用のメモリは、アプリケーションヒ
                ープから確保します。

  Arguments:    name:   アーカイブをファイルシステム上で識別する為の名前。
                path:   アーカイブのパス名。

  Returns:      アーカイブのロードに成功すれば、NNSFndArchive 構造体へのポイ
                ンタを返します。失敗した場合には、NULLを返します。
 *---------------------------------------------------------------------------*/
NNSFndArchive*
LoadArchive(const char* name, const char* path)
{
    FSFile          file;
    NNSFndArchive*  archive = NULL;

    FS_InitFile(&file);
    if (FS_OpenFile(&file, path))
    {
        u32 binarySize = FS_GetLength(&file);
        u32 memorySize = ROUND_UP(sizeof(NNSFndArchive), 16) + ROUND_UP(binarySize, 16);

        u8* memory     = (u8*)NNS_FndAllocFromExpHeapEx(gAppHeap, memorySize, 16);

        if (memory != NULL)
        {
            u8* binary = memory + ROUND_UP(sizeof(NNSFndArchive), 16);

            if (FS_ReadFile(&file, binary, (s32)binarySize) == binarySize)
            {
                DC_FlushRange(binary, binarySize);

                if (NNS_FndMountArchive((NNSFndArchive*)memory, name, binary))
                {
                    archive = (NNSFndArchive*)memory;
                }
                else
                {
                    NNS_FndFreeToExpHeap(gAppHeap, memory);
                }
            }
        }
        (void)FS_CloseFile(&file);
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

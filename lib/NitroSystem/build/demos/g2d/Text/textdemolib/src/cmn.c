/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib
  File:     cmn.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nnsys.h>
#include "g2d_textdemolib.h"


// ゲームパッド
CMNGamePad CMNGamePadState;



/*---------------------------------------------------------------------------*
  Name:         CMN_InitInterrupt

  Description:  割り込みの初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void CMN_InitInterrupt( void )
{
    // 個別割り込みフラグを全て不許可に
    (void)OS_SetIrqMask(0);

    // マスター割り込みフラグを許可に
    (void)OS_EnableIrq();
}



/*---------------------------------------------------------------------------*
  Name:         CMN_BeginVBlankIntr

  Description:  Vブランク割り込みの発生を開始します。

  Arguments:    vBlankFunc: Vブランク割り込みハンドラへのポインタ

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void CMN_BeginVBlankIntr( OSIrqFunction vBlankFunc )
{
    // 割り込みハンドラ登録
    OS_SetIrqFunction(OS_IE_V_BLANK, vBlankFunc);

    // VBlank割り込み許可
    (void)OS_EnableIrqMask( OS_IE_V_BLANK );

    // VBlank割り込み生成開始
    (void)GX_VBlankIntr(TRUE);
}



/*---------------------------------------------------------------------------*
  Name:         CMN_InitAllocator

  Description:  アリーナ全体を担当するようにアロケータを初期化します。

  Arguments:    pAllocator: 未初期化のアロケータへのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void CMN_InitAllocator( NNSFndAllocator* pAllocator )
{
    u32   arenaLow      = MATH_ROUNDUP  ((u32)OS_GetMainArenaLo(), 16);
    u32   arenaHigh     = MATH_ROUNDDOWN((u32)OS_GetMainArenaHi(), 16);
    u32   heapSize      = arenaHigh - arenaLow;
    void* heapMemory    = OS_AllocFromMainArenaLo(heapSize, 16);
    NNSFndHeapHandle    heapHandle;
    SDK_NULL_ASSERT( pAllocator );

    heapHandle = NNS_FndCreateExpHeap(heapMemory, heapSize);
    SDK_ASSERT( heapHandle != NNS_FND_HEAP_INVALID_HANDLE );

    NNS_FndInitAllocatorForExpHeap(pAllocator, heapHandle, 4);
}



/*---------------------------------------------------------------------------*
  Name:         CMN_InitFileSystem

  Description:  ファイルシステムを有効にします。
                また、ファイルテーブルをメモリに読み込みます。

  Arguments:    pAllocator: 有効なアロケータへのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
// ファイルシステム準備
void CMN_InitFileSystem( NNSFndAllocator* pAllocator )
{
    SDK_NULL_ASSERT( pAllocator );

    // ARM7との通信FIFO割り込み許可
    (void)OS_EnableIrqMask(OS_IE_SPFIFO_RECV);

    // ファイルシステム初期化
    FS_Init( FS_DMA_NOT_USE );

    // ファイルテーブルキャッシュ
    if( pAllocator != NULL )
    {
        const u32   need_size = FS_GetTableSize();
        void    *p_table = NNS_FndAllocFromAllocator( pAllocator, need_size );
        SDK_ASSERT(p_table != NULL);
        (void)FS_LoadTable(p_table, need_size);
    }
}



/*---------------------------------------------------------------------------*
  Name:         CMN_ClearVram

  Description:  VRAM をクリアします。
                VRAM が全て未割り当てでなければなりません。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
// VRAMクリア
void CMN_ClearVram( void )
{
    //---------------------------------------------------------------------------
    // All VRAM banks to LCDC
    //---------------------------------------------------------------------------
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);

    //---------------------------------------------------------------------------
    // Clear all LCDC space
    //---------------------------------------------------------------------------
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

    //---------------------------------------------------------------------------
    // Disable the banks on LCDC
    //---------------------------------------------------------------------------
    (void)GX_DisableBankForLCDC();

    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);      // clear OAM
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);        // clear the standard palette

    MI_CpuFillFast((void*)HW_DB_OAM, 192, HW_DB_OAM_SIZE); // clear OAM
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);  // clear the standard palette
}



/*---------------------------------------------------------------------------*
  Name:         CMN_ReadGamePad

  Description:  コントローラ入力を読み取り内部バッファに格納します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void CMN_ReadGamePad(void)
{
    u16 status = PAD_Read();

    CMNGamePadState.trigger = (u16)(status                  & (status ^ CMNGamePadState.button));
    CMNGamePadState.release = (u16)(CMNGamePadState.button  & (status ^ CMNGamePadState.button));
    CMNGamePadState.button  = status;
}



/*---------------------------------------------------------------------------*
  Name:         CMN_LoadFile

  Description:  ファイルをメモリにロードします。
                ファイルデータが不要になった場合は
                CMN_UnloadFile( *ppFile, pAlloc ) でファイルデータを
                解放します。

  Arguments:    ppFile: ファイルをロードしたメモリアドレスを受け取る
                        バッファへのポインタ。
                fpath:  ロードするファイルのパス
                pAlloc: アロケータへのポインタ

  Returns:      ロードしたファイルのファイルサイズを返します。
                0 の場合はファイルロードに失敗した事を表します。
                この場合 *ppFile の値は無効です。
 *---------------------------------------------------------------------------*/
u32 CMN_LoadFile(void** ppFile, const char* fpath, NNSFndAllocator* pAlloc)
{
    BOOL bSuccess;
    FSFile f;
    u32 length;
    u32 read;

    SDK_NULL_ASSERT( ppFile );
    SDK_NULL_ASSERT( fpath );
    SDK_NULL_ASSERT( pAlloc );

    FS_InitFile(&f);

    bSuccess = FS_OpenFile(&f, fpath);
    if( ! bSuccess )
    {
        OS_Warning("file (%s) not found", fpath);
        return 0;
    }

    length = FS_GetLength(&f);
    *ppFile = NNS_FndAllocFromAllocator(pAlloc, length);
    if( *ppFile == NULL )
    {
        OS_Warning("cant allocate memory for file: %s", fpath);
        return 0;
    }

    read = (u32)FS_ReadFile(&f, *ppFile, (s32)length);
    if( read != length )
    {
        OS_Warning("fail to load file: %s", fpath);
        NNS_FndFreeToAllocator(pAlloc, *ppFile);
        return 0;
    }

    bSuccess = FS_CloseFile(&f);
    if( ! bSuccess )
    {
        OS_Warning("fail to close file: %s", fpath);
    }

    return length;
}



/*---------------------------------------------------------------------------*
  Name:         CMN_UnloadFile

  Description:  ファイルデータを解放します。

  Arguments:    pFile:  ファイルデータへのポインタ
                pAlloc: ファイルロードに用いたアロケータへのポインタ

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void CMN_UnloadFile(void* pFile, NNSFndAllocator* pAlloc)
{
    NNS_FndFreeToAllocator(pAlloc, pFile);
}



/*---------------------------------------------------------------------------*
  Name:         CMN_LoadArchive

  Description:  パス名で指定されたアーカイブをメモリに読み込み、ファイルシス
                テムにマウントします。
                アーカイブが不要になった場合は
                CMN_RemoveArchive( 返り値, pAllocator ) でアーカイブを
                解放します。

  Arguments:    name:       アーカイブをファイルシステム上で識別する為の名前。
                path:       アーカイブのパス名。
                pAllocator: アロケータへのポインタ

  Returns:      アーカイブのロードに成功すれば、NNSFndArchive 構造体へのポイ
                ンタを返します。失敗した場合には、NULLを返します。
 *---------------------------------------------------------------------------*/
NNSFndArchive*
CMN_LoadArchive(const char* name, const char* path, NNSFndAllocator* pAllocator)
{
    FSFile          file;
    NNSFndArchive*  archive = NULL;

    SDK_NULL_ASSERT(name);
    SDK_NULL_ASSERT(path);
    SDK_NULL_ASSERT(pAllocator);

    FS_InitFile(&file);
    if (FS_OpenFile(&file, path))
    {
        u32 binarySize = FS_GetLength(&file);
        u32 memorySize = MATH_ROUNDUP(sizeof(NNSFndArchive), 16) + MATH_ROUNDUP(binarySize, 16);

        u8* memory     = (u8*)NNS_FndAllocFromAllocator(pAllocator, memorySize);

        if (memory != NULL)
        {
            u8* binary = memory + MATH_ROUNDUP(sizeof(NNSFndArchive), 16);

            if ((u32)FS_ReadFile(&file, binary, (s32)binarySize) == binarySize)
            {
                if (NNS_FndMountArchive((NNSFndArchive*)memory, name, binary))
                {
                    archive = (NNSFndArchive*)memory;
                }
            }
        }
        (void)FS_CloseFile(&file);
    }
    return archive;
}



/*---------------------------------------------------------------------------*
  Name:         CMN_RemoveArchive

  Description:  指定されたアーカイブをメモリから削除します。

                指定されたアーカイブをファイルシステムからアンマウントし、ア
                ーカイブが読み込まれていたメモリを解放します。

  Arguments:    archive:    NNSアーカイブ構造体へのポインタ。
                pAllocator: アーカイブロードに用いたアロケータへのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
CMN_RemoveArchive(NNSFndArchive* archive, NNSFndAllocator* pAllocator)
{
    SDK_NULL_ASSERT(archive);
    SDK_NULL_ASSERT(pAllocator);

    (void)NNS_FndUnmountArchive(archive);
    NNS_FndFreeToAllocator(pAllocator, archive);
}


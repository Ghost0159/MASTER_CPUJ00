/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - bg - ShareBg
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      カラーパレットとキャラクタを共有して BG を表示します。
//
//  操作方法
//      なし。
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>
#include <nnsys/fnd.h>
#include <string.h>

#include "loader.h"


#define BG_DATA_DIR         "/data/"

// アライメントにあわせて切り上げ
#define ROUND_UP(value, alignment) \
    (((u32)(value) + (alignment-1)) & ~(alignment-1))

// アライメントにあわせて切り下げ
#define ROUND_DOWN(value, alignment) \
    ((u32)(value) & ~(alignment-1))


typedef struct BGData
{
    void*                   pPltFile;
    void*                   pChrFile;
    void*                   pScnFile;
    NNSG2dPaletteData*      pPltData;
    NNSG2dCharacterData*    pChrData;
    NNSG2dScreenData*       pScnData;
}
BGData;

//------------------------------------------------------------------------------
// グローバル変数

NNSFndAllocator     gAllocator;         // メモリアロケータ

//------------------------------------------------------------------------------
// プロトタイプ宣言
static void VBlankIntr(void);


//****************************************************************************
// Initialization etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:

  Description:

  Arguments:

  Returns:
 *---------------------------------------------------------------------------*/
// 割り込み初期化
static void InitInterrupt( void )
{
    // 個別割り込みフラグを全て不許可に
    (void)OS_SetIrqMask(0);

    // マスター割り込みフラグを許可に
    (void)OS_EnableIrq();
}

// メモリアロケータ準備
static void InitAllocator( NNSFndAllocator* pAllocator )
{
    u32   arenaLow      = ROUND_UP  (OS_GetMainArenaLo(), 16);
    u32   arenaHigh     = ROUND_DOWN(OS_GetMainArenaHi(), 16);
    u32   heapSize      = arenaHigh - arenaLow;
    void* heapMemory    = OS_AllocFromMainArenaLo(heapSize, 16);
    NNSFndHeapHandle       heapHandle;
    SDK_NULL_ASSERT( pAllocator );

    heapHandle = NNS_FndCreateExpHeap(heapMemory, heapSize);
    SDK_ASSERT( heapHandle != NNS_FND_HEAP_INVALID_HANDLE );

    NNS_FndInitAllocatorForExpHeap(pAllocator, heapHandle, 4);
}

// ファイルシステム準備
static void InitFileSystem( NNSFndAllocator* pAllocator )
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

// VRAMクリア
static void ClearVram( void )
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

// VRAM割り当て
static void AssignVramBanks( void )
{
    GX_SetBankForBG(GX_VRAM_BG_256_AB);                         // VRAM-AB for BGs
    GX_SetBankForBGExtPltt(GX_VRAM_BGEXTPLTT_0123_E);           // VRAM-E  for BG Ext Plt

    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);                   // VRAM-C for BGs
    GX_SetBankForSubBGExtPltt(GX_VRAM_SUB_BGEXTPLTT_0123_H);    // VRAM-H for BG Ext Plt
}

// 初期化
static void Init( void )
{
    // Common initialization.
    {
        OS_Init();
        GX_Init();

        InitInterrupt();
    }

    {
        ClearVram();
        InitAllocator( &gAllocator );
        InitFileSystem( &gAllocator );
    }

    AssignVramBanks();
}




//****************************************************************************
// Main
//****************************************************************************

// ファイルパスを作成
static char* MakeFilePath(const char* fname, const char* fext)
{
    static char buf[128];
    SDK_NULL_ASSERT( fname );
    SDK_NULL_ASSERT( fext );

    (void)strcpy(buf, BG_DATA_DIR);
    (void)strcat(buf, fname);
    (void)strcat(buf, fext);
    return buf;
}

// カラーパレット、キャラクタ、スクリーンをファイルからメインメモリに読み込み
static void LoadBGData( BGData* pData , const char* fname)
{
    SDK_NULL_ASSERT( pData );
    SDK_NULL_ASSERT( fname );

    // load from file
    {
        pData->pPltFile = LoadNCLR( &pData->pPltData, MakeFilePath(fname, ".NCLR"), &gAllocator );
        SDK_NULL_ASSERT( pData->pPltFile );

        pData->pChrFile = LoadNCGR( &pData->pChrData, MakeFilePath(fname, ".NCGR"), &gAllocator );
        SDK_NULL_ASSERT( pData->pChrFile );

        pData->pScnFile = LoadNSCR( &pData->pScnData, MakeFilePath(fname, ".NSCR"), &gAllocator );
        SDK_NULL_ASSERT( pData->pScnFile );
    }
}

// メインメモリ上のファイルデータを解放
static void UnloadBGData( BGData* pData )
{
    SDK_NULL_ASSERT( pData );
    SDK_NULL_ASSERT( pData->pPltFile );
    SDK_NULL_ASSERT( pData->pChrFile );
    SDK_NULL_ASSERT( pData->pScnFile );
    NNS_FndFreeToAllocator( &gAllocator, pData->pPltFile );
    NNS_FndFreeToAllocator( &gAllocator, pData->pChrFile );
    NNS_FndFreeToAllocator( &gAllocator, pData->pScnFile );
}

static void BGSetup( void )
{
    // setup registers
    {
        GX_SetVisiblePlane( GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 );
    }

    // load bg
    {
        BGData bg;
    	void* pScnFile;
        NNSG2dScreenData* pScnData;

        LoadBGData(&bg, "BG");
        NNS_G2dBGSetup(
            NNS_G2D_BGSELECT_MAIN0,
            bg.pScnData,
            bg.pChrData,
            bg.pPltData,
            GX_BG_SCRBASE_0x0000,
            GX_BG_CHARBASE_0x08000
        );
        UnloadBGData(&bg);

        pScnFile = LoadNSCR( &pScnData, MakeFilePath("BG2", ".NSCR"), &gAllocator );
        SDK_NULL_ASSERT( pScnFile );

        NNS_G2dBGSetup(
            NNS_G2D_BGSELECT_MAIN1,
            pScnData,
        	NULL,                       // キャラクタをロードしません
        	NULL,                       // カラーパレットをロードしません
            GX_BG_SCRBASE_0x2000,
            GX_BG_CHARBASE_0x08000      // キャラクタを BG0 と共有します
        );
        NNS_FndFreeToAllocator( &gAllocator, pScnFile );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // Initialize App.
    {
        Init();
        BGSetup();
    }

    {
        GX_DispOn();
        GXS_DispOn();
    }

    // Main loop
    while( TRUE )
    {
    }
}


/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - bg - BgEx
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
//      圧縮パレットや部分キャラクタデータを用いて BG を表示します。
//      上画面 BG3: 通常の BG
//      上画面 BG1: 圧縮パレットを用いた BG
//      下画面 BG0: 部分キャラクタを用いた BG
//      下画面 BG2: 圧縮パレットと部分キャラクタを用いた BG
//
//  操作方法
//      なし。
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>
#include <nnsys/fnd.h>
#include <string.h>

#include "loader.h"


#define BG_DATA_DIR             "/data/"

#define BG_FILENAME_NORMAL      BG_DATA_DIR "BG_normal"
    // 通常 BG のリソースファイル名

#define BG_FILENAME_CMP         BG_DATA_DIR "BG_cmp"
    // 圧縮パレット使用 BG のリソースファイル名

#define BG_FILENAME_PART        BG_DATA_DIR "BG_part"
    // 部分キャラクタ使用 BG のリソースファイル名

#define BG_FILENAME_CMP_PART    BG_DATA_DIR "BG_cmp_part"
    // 圧縮パレットと部分キャラクタを使用 BG のリソースファイル名


// アライメントにあわせて切り上げ
#define ROUND_UP(value, alignment) \
    (((u32)(value) + (alignment-1)) & ~(alignment-1))

// アライメントにあわせて切り下げ
#define ROUND_DOWN(value, alignment) \
    ((u32)(value) & ~(alignment-1))


//---- BG リソースファイルをまとめる構造体
typedef struct BGData
{
    void*                       pPltFile;   // パレットファイルへのポインタ
    void*                       pChrFile;   // キャラクタファイルへのポインタ
    void*                       pScnFile;   // スクリーンファイルへのポインタ
    NNSG2dPaletteData*          pPltData;   // パレットデータへのポインタ
    NNSG2dCharacterData*        pChrData;   // キャラクタデータへのポインタ
    NNSG2dScreenData*           pScnData;   // スクリーンデータへのポインタ
    NNSG2dCharacterPosInfo*     pPosInfo;   // 部分キャラクタ位置情報へのポインタ
    NNSG2dPaletteCompressInfo*  pCmpInfo;   // パレット圧縮情報へのポインタ
}
BGData;



//------------------------------------------------------------------------------
// グローバル変数

NNSFndAllocator     gAllocator;         // メモリアロケータ

//------------------------------------------------------------------------------
// プロトタイプ宣言
static void VBlankIntr(void);








//****************************************************************************
// 初期化処理
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         InitInterrupt

  Description:  割り込み処理を初期化します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitInterrupt( void )
{
    // 個別割り込みフラグを全て不許可に
    (void)OS_SetIrqMask(0);

    // マスター割り込みフラグを許可に
    (void)OS_EnableIrq();
}



/*---------------------------------------------------------------------------*
  Name:         InitAllocator

  Description:  アプリケーション唯一のアロケータを初期化します。

  Arguments:    pAllocator: 初期化するアロケータへのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
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



/*---------------------------------------------------------------------------*
  Name:         InitFileSystem

  Description:  ファイルシステムを初期化します。

  Arguments:    pAllocator: アロケータへのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
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



/*---------------------------------------------------------------------------*
  Name:         ClearVram

  Description:  VRAM をクリアします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
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



/*---------------------------------------------------------------------------*
  Name:         AssignVramBanks

  Description:  VRAM の割り当てを行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void AssignVramBanks( void )
{
    GX_SetBankForBG(GX_VRAM_BG_256_AB);                         // VRAM-AB for BGs
    GX_SetBankForBGExtPltt(GX_VRAM_BGEXTPLTT_0123_E);           // VRAM-E  for BG Ext Plt

    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);                   // VRAM-C for BGs
    GX_SetBankForSubBGExtPltt(GX_VRAM_SUB_BGEXTPLTT_0123_H);    // VRAM-H for BG Ext Plt
}



/*---------------------------------------------------------------------------*
  Name:         Init

  Description:  サンプル用の初期化処理を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
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
// サンプルのメイン処理
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         CatFileExt

  Description:  指定されたファイル名に指定された拡張子を連結したものを返します。
 
  Arguments:    fname:  ファイル名。
                fext:   拡張子。

  Returns:      fname と fext を連結した文字列が格納されたバッファへのポインタ。
                このバッファは関数内部に静的に確保されているバッファですので
                次回呼び出し時に内容は破壊されます。
 *---------------------------------------------------------------------------*/
static const char* CatFileExt(const char* fname, const char* fext)
{
    static char buf[FS_FILE_NAME_MAX + 1];
    SDK_NULL_ASSERT( fname );
    SDK_NULL_ASSERT( fext );
	SDK_ASSERT( strlen(fname) + strlen(fext) <= FS_FILE_NAME_MAX );

    (void)strcpy(buf, fname);
    (void)strcat(buf, fext);
    return buf;
}



/*---------------------------------------------------------------------------*
  Name:         LoadBGData

  Description:  メイン関数です。

  Arguments:    pData:  ロードしたBGリソースへのポインタを格納する
                        BGData 構造体へのポインタ。
                fname:  ロードする BG リソースのファイル名（拡張子除く）

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void LoadBGData( BGData* pData , const char* fname)
{
    SDK_NULL_ASSERT( pData );
    SDK_NULL_ASSERT( fname );

    // load from file
    {
        // color palette
        pData->pPltFile = LoadNCLREx(
            &pData->pPltData,
            &pData->pCmpInfo,
            CatFileExt(fname, ".NCLR"),
            &gAllocator
        );
        SDK_NULL_ASSERT( pData->pPltFile );

        // character
        pData->pChrFile = LoadNCGREx(
            &pData->pChrData,
            &pData->pPosInfo,
            CatFileExt(fname, ".NCGR"),
            &gAllocator
        );
        SDK_NULL_ASSERT( pData->pChrFile );

        // screen
        pData->pScnFile = LoadNSCR(
            &pData->pScnData,
            CatFileExt(fname, ".NSCR"),
            &gAllocator
        );
        SDK_NULL_ASSERT( pData->pScnFile );
    }
}



/*---------------------------------------------------------------------------*
  Name:         UnloadBGData

  Description:  LoadBGData でロードしたBGリソース用のメモリを解放します。

  Arguments:    pData:  BGリソースへのポインタを格納する BGData 構造体へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
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



/*---------------------------------------------------------------------------*
  Name:         SetupBG

  Description:  指定された BG に指定された BGリソースを使用して BG を表示します。

  Arguments:    bg:         対象の BG 面。
                scrBase:    スクリーンベースブロック
                charBase:   キャラクタベースブロック
                bgFileName: BG リソースのファイル名（拡張子除く）

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SetupBG(
    NNSG2dBGSelect bg,
    GXBGScrBase scrBase,
    GXBGCharBase charBase,
    const char* bgFileName
)
{
    BGData bgData;

    LoadBGData(&bgData, bgFileName);
    NNS_G2dBGSetupEx(
        bg,
        bgData.pScnData,
        bgData.pChrData,
        bgData.pPltData,
        bgData.pPosInfo,
        bgData.pCmpInfo,
        scrBase,
        charBase
    );
    UnloadBGData(&bgData);
}



/*---------------------------------------------------------------------------*
  Name:         BGSetup

  Description:  BGの表示を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void BGSetup( void )
{
    // setup registers
    {
        GX_SetVisiblePlane ( GX_PLANEMASK_BG1 | GX_PLANEMASK_BG3 );
        GXS_SetVisiblePlane( GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 );

        G2_SetBG1Priority(0);
        G2_SetBG3Priority(1);
        G2S_SetBG0Priority(0);
        G2S_SetBG2Priority(1);
    }

    // load bg
    {
        SetupBG(
            NNS_G2D_BGSELECT_MAIN3,
            GX_BG_SCRBASE_0x0000,
            GX_BG_CHARBASE_0x08000,
            BG_FILENAME_NORMAL
        );
        SetupBG(
            NNS_G2D_BGSELECT_MAIN1,
            GX_BG_SCRBASE_0x0800,
            GX_BG_CHARBASE_0x10000,
            BG_FILENAME_CMP
        );
        SetupBG(
            NNS_G2D_BGSELECT_SUB0,
            GX_BG_SCRBASE_0x0000,
            GX_BG_CHARBASE_0x08000,
            BG_FILENAME_PART
        );
        SetupBG(
            NNS_G2D_BGSELECT_SUB2,
            GX_BG_SCRBASE_0x0800,
            GX_BG_CHARBASE_0x10000,
            BG_FILENAME_CMP_PART
        );
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


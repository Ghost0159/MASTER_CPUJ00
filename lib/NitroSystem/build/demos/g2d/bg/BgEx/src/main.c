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
//  �f���̓��e
//      ���k�p���b�g�╔���L�����N�^�f�[�^��p���� BG ��\�����܂��B
//      ���� BG3: �ʏ�� BG
//      ���� BG1: ���k�p���b�g��p���� BG
//      ����� BG0: �����L�����N�^��p���� BG
//      ����� BG2: ���k�p���b�g�ƕ����L�����N�^��p���� BG
//
//  ������@
//      �Ȃ��B
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>
#include <nnsys/fnd.h>
#include <string.h>

#include "loader.h"


#define BG_DATA_DIR             "/data/"

#define BG_FILENAME_NORMAL      BG_DATA_DIR "BG_normal"
    // �ʏ� BG �̃��\�[�X�t�@�C����

#define BG_FILENAME_CMP         BG_DATA_DIR "BG_cmp"
    // ���k�p���b�g�g�p BG �̃��\�[�X�t�@�C����

#define BG_FILENAME_PART        BG_DATA_DIR "BG_part"
    // �����L�����N�^�g�p BG �̃��\�[�X�t�@�C����

#define BG_FILENAME_CMP_PART    BG_DATA_DIR "BG_cmp_part"
    // ���k�p���b�g�ƕ����L�����N�^���g�p BG �̃��\�[�X�t�@�C����


// �A���C�����g�ɂ��킹�Đ؂�グ
#define ROUND_UP(value, alignment) \
    (((u32)(value) + (alignment-1)) & ~(alignment-1))

// �A���C�����g�ɂ��킹�Đ؂艺��
#define ROUND_DOWN(value, alignment) \
    ((u32)(value) & ~(alignment-1))


//---- BG ���\�[�X�t�@�C�����܂Ƃ߂�\����
typedef struct BGData
{
    void*                       pPltFile;   // �p���b�g�t�@�C���ւ̃|�C���^
    void*                       pChrFile;   // �L�����N�^�t�@�C���ւ̃|�C���^
    void*                       pScnFile;   // �X�N���[���t�@�C���ւ̃|�C���^
    NNSG2dPaletteData*          pPltData;   // �p���b�g�f�[�^�ւ̃|�C���^
    NNSG2dCharacterData*        pChrData;   // �L�����N�^�f�[�^�ւ̃|�C���^
    NNSG2dScreenData*           pScnData;   // �X�N���[���f�[�^�ւ̃|�C���^
    NNSG2dCharacterPosInfo*     pPosInfo;   // �����L�����N�^�ʒu���ւ̃|�C���^
    NNSG2dPaletteCompressInfo*  pCmpInfo;   // �p���b�g���k���ւ̃|�C���^
}
BGData;



//------------------------------------------------------------------------------
// �O���[�o���ϐ�

NNSFndAllocator     gAllocator;         // �������A���P�[�^

//------------------------------------------------------------------------------
// �v���g�^�C�v�錾
static void VBlankIntr(void);








//****************************************************************************
// ����������
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         InitInterrupt

  Description:  ���荞�ݏ��������������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitInterrupt( void )
{
    // �ʊ��荞�݃t���O��S�ĕs����
    (void)OS_SetIrqMask(0);

    // �}�X�^�[���荞�݃t���O������
    (void)OS_EnableIrq();
}



/*---------------------------------------------------------------------------*
  Name:         InitAllocator

  Description:  �A�v���P�[�V�����B��̃A���P�[�^�����������܂��B

  Arguments:    pAllocator: ����������A���P�[�^�ւ̃|�C���^�B

  Returns:      �Ȃ��B
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

  Description:  �t�@�C���V�X�e�������������܂��B

  Arguments:    pAllocator: �A���P�[�^�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitFileSystem( NNSFndAllocator* pAllocator )
{
    SDK_NULL_ASSERT( pAllocator );

    // ARM7�Ƃ̒ʐMFIFO���荞�݋���
    (void)OS_EnableIrqMask(OS_IE_SPFIFO_RECV);

    // �t�@�C���V�X�e��������
    FS_Init( FS_DMA_NOT_USE );

    // �t�@�C���e�[�u���L���b�V��
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

  Description:  VRAM ���N���A���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
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

  Description:  VRAM �̊��蓖�Ă��s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
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

  Description:  �T���v���p�̏������������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
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
// �T���v���̃��C������
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         CatFileExt

  Description:  �w�肳�ꂽ�t�@�C�����Ɏw�肳�ꂽ�g���q��A���������̂�Ԃ��܂��B
 
  Arguments:    fname:  �t�@�C�����B
                fext:   �g���q�B

  Returns:      fname �� fext ��A�����������񂪊i�[���ꂽ�o�b�t�@�ւ̃|�C���^�B
                ���̃o�b�t�@�͊֐������ɐÓI�Ɋm�ۂ���Ă���o�b�t�@�ł��̂�
                ����Ăяo�����ɓ��e�͔j�󂳂�܂��B
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

  Description:  ���C���֐��ł��B

  Arguments:    pData:  ���[�h����BG���\�[�X�ւ̃|�C���^���i�[����
                        BGData �\���̂ւ̃|�C���^�B
                fname:  ���[�h���� BG ���\�[�X�̃t�@�C�����i�g���q�����j

  Returns:      �Ȃ��B
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

  Description:  LoadBGData �Ń��[�h����BG���\�[�X�p�̃�������������܂��B

  Arguments:    pData:  BG���\�[�X�ւ̃|�C���^���i�[���� BGData �\���̂ւ̃|�C���^�B

  Returns:      �Ȃ��B
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

  Description:  �w�肳�ꂽ BG �Ɏw�肳�ꂽ BG���\�[�X���g�p���� BG ��\�����܂��B

  Arguments:    bg:         �Ώۂ� BG �ʁB
                scrBase:    �X�N���[���x�[�X�u���b�N
                charBase:   �L�����N�^�x�[�X�u���b�N
                bgFileName: BG ���\�[�X�̃t�@�C�����i�g���q�����j

  Returns:      �Ȃ��B
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

  Description:  BG�̕\�����s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
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

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
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


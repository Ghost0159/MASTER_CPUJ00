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


// �Q�[���p�b�h
CMNGamePad CMNGamePadState;



/*---------------------------------------------------------------------------*
  Name:         CMN_InitInterrupt

  Description:  ���荞�݂̏��������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void CMN_InitInterrupt( void )
{
    // �ʊ��荞�݃t���O��S�ĕs����
    (void)OS_SetIrqMask(0);

    // �}�X�^�[���荞�݃t���O������
    (void)OS_EnableIrq();
}



/*---------------------------------------------------------------------------*
  Name:         CMN_BeginVBlankIntr

  Description:  V�u�����N���荞�݂̔������J�n���܂��B

  Arguments:    vBlankFunc: V�u�����N���荞�݃n���h���ւ̃|�C���^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void CMN_BeginVBlankIntr( OSIrqFunction vBlankFunc )
{
    // ���荞�݃n���h���o�^
    OS_SetIrqFunction(OS_IE_V_BLANK, vBlankFunc);

    // VBlank���荞�݋���
    (void)OS_EnableIrqMask( OS_IE_V_BLANK );

    // VBlank���荞�ݐ����J�n
    (void)GX_VBlankIntr(TRUE);
}



/*---------------------------------------------------------------------------*
  Name:         CMN_InitAllocator

  Description:  �A���[�i�S�̂�S������悤�ɃA���P�[�^�����������܂��B

  Arguments:    pAllocator: ���������̃A���P�[�^�ւ̃|�C���^�B

  Returns:      �Ȃ��B
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

  Description:  �t�@�C���V�X�e����L���ɂ��܂��B
                �܂��A�t�@�C���e�[�u�����������ɓǂݍ��݂܂��B

  Arguments:    pAllocator: �L���ȃA���P�[�^�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
// �t�@�C���V�X�e������
void CMN_InitFileSystem( NNSFndAllocator* pAllocator )
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
  Name:         CMN_ClearVram

  Description:  VRAM ���N���A���܂��B
                VRAM ���S�Ė����蓖�ĂłȂ���΂Ȃ�܂���B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
// VRAM�N���A
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

  Description:  �R���g���[�����͂�ǂݎ������o�b�t�@�Ɋi�[���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
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

  Description:  �t�@�C�����������Ƀ��[�h���܂��B
                �t�@�C���f�[�^���s�v�ɂȂ����ꍇ��
                CMN_UnloadFile( *ppFile, pAlloc ) �Ńt�@�C���f�[�^��
                ������܂��B

  Arguments:    ppFile: �t�@�C�������[�h�����������A�h���X���󂯎��
                        �o�b�t�@�ւ̃|�C���^�B
                fpath:  ���[�h����t�@�C���̃p�X
                pAlloc: �A���P�[�^�ւ̃|�C���^

  Returns:      ���[�h�����t�@�C���̃t�@�C���T�C�Y��Ԃ��܂��B
                0 �̏ꍇ�̓t�@�C�����[�h�Ɏ��s��������\���܂��B
                ���̏ꍇ *ppFile �̒l�͖����ł��B
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

  Description:  �t�@�C���f�[�^��������܂��B

  Arguments:    pFile:  �t�@�C���f�[�^�ւ̃|�C���^
                pAlloc: �t�@�C�����[�h�ɗp�����A���P�[�^�ւ̃|�C���^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void CMN_UnloadFile(void* pFile, NNSFndAllocator* pAlloc)
{
    NNS_FndFreeToAllocator(pAlloc, pFile);
}



/*---------------------------------------------------------------------------*
  Name:         CMN_LoadArchive

  Description:  �p�X���Ŏw�肳�ꂽ�A�[�J�C�u���������ɓǂݍ��݁A�t�@�C���V�X
                �e���Ƀ}�E���g���܂��B
                �A�[�J�C�u���s�v�ɂȂ����ꍇ��
                CMN_RemoveArchive( �Ԃ�l, pAllocator ) �ŃA�[�J�C�u��
                ������܂��B

  Arguments:    name:       �A�[�J�C�u���t�@�C���V�X�e����Ŏ��ʂ���ׂ̖��O�B
                path:       �A�[�J�C�u�̃p�X���B
                pAllocator: �A���P�[�^�ւ̃|�C���^

  Returns:      �A�[�J�C�u�̃��[�h�ɐ�������΁ANNSFndArchive �\���̂ւ̃|�C
                ���^��Ԃ��܂��B���s�����ꍇ�ɂ́ANULL��Ԃ��܂��B
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

  Description:  �w�肳�ꂽ�A�[�J�C�u������������폜���܂��B

                �w�肳�ꂽ�A�[�J�C�u���t�@�C���V�X�e������A���}�E���g���A�A
                �[�J�C�u���ǂݍ��܂�Ă�����������������܂��B

  Arguments:    archive:    NNS�A�[�J�C�u�\���̂ւ̃|�C���^�B
                pAllocator: �A�[�J�C�u���[�h�ɗp�����A���P�[�^�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
CMN_RemoveArchive(NNSFndArchive* archive, NNSFndAllocator* pAllocator)
{
    SDK_NULL_ASSERT(archive);
    SDK_NULL_ASSERT(pAllocator);

    (void)NNS_FndUnmountArchive(archive);
    NNS_FndFreeToAllocator(pAllocator, archive);
}


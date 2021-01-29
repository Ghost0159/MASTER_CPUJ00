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

GamePad          gGamePad;      // �Q�[���p�b�h
NNSFndHeapHandle gSysHeap;      // �V�X�e���q�[�v�@�@�@�@�i�g���q�[�v���g�p�j
NNSFndHeapHandle gAppHeap;      // �A�v���P�[�V�����q�[�v�i�g���q�[�v���g�p�j


/*---------------------------------------------------------------------------*
  Name:         InitMemory

  Description:  �V�X�e���q�[�v�ƃA�v���P�[�V�����q�[�v���쐬���܂��B���q�[�v
                �ɂ�NITRO-System�̊g���q�[�v���g�p���Ă��܂��B
                
                �V�X�e���q�[�v�p�̃������Ƃ��āASYSTEM_HEAP_SIZE�������C���A
                ���[�i����m�ۂ��A���C���A���[�i�̎c���S�ăA�v���P�[�V����
                �q�[�v�p�̃������Ɋm�ۂ��Ă��܂��B
                
                �V�X�e���q�[�v�́A�Q�[���V�X�e�����̃V�X�e���v���O�����Ŏg�p
                ���邱�Ƃ�z�肵�Ă��܂��B�A�v���P�[�V�����q�[�v�ɂ́A�Q�[��
                �Ŏg�p����f�[�^�����[�h����ׂɎg�p���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
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

  Description:  �p�X���Ŏw�肳�ꂽ���k���ꂽ�A�[�J�C�u���������ɓW�J���A
                �t�@�C���V�X�e���Ƀ}�E���g���܂��B���k�ɂ́Antrcomp.exe��
                �g�p���Ă��܂��B

                �A�[�J�C�u��ǂݍ��ވׂ̃������ƁA�A�[�J�C�u���}�E���g�����
                �ɕK�v��NNSFndArchive �\���̗p�̃������́A�A�v���P�[�V�����q
                �[�v����m�ۂ��܂��B

                ���k�f�[�^�́A�g���q�[�v�̌�둤����m�ۂ��Ă��܂��B�W�J��
                �̃f�[�^�́A�g���q�[�v�̑O������m�ۂ��܂��̂ŁA���k�f�[�^
                �����Ƀt���O�����e�[�V�������������܂���B

  Arguments:    name:   �A�[�J�C�u���t�@�C���V�X�e����Ŏ��ʂ���ׂ̖��O�B
                path:   �A�[�J�C�u�̃p�X���B

  Returns:      �A�[�J�C�u�̃��[�h�ɐ�������΁ANNSFndArchive �\���̂ւ̃|�C
                ���^��Ԃ��܂��B���s�����ꍇ�ɂ́ANULL��Ԃ��܂��B
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

        // ���k�f�[�^���i�[����ׂ̃������́A�g���q�[�v�̌�납��m�ۂ��܂��B
        // �i�A���C�����g�ɕ������w�肷��ƁA��납��m�ۂ��܂��B�j
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
        // NNSFndArchive�\���̂ƃA�[�J�C�u�f�[�^���A��̃������u���b�N�Ɋi�[���܂��B
        u32 memorySize = ROUND_UP(sizeof(NNSFndArchive), 16) + ROUND_UP(MI_GetUncompressedSize(compData), 16);

        // �W�J��̃A�[�J�C�u�f�[�^�́A�q�[�v�̑O����m�ۂ��܂��B
        u8* memory = NNS_FndAllocFromExpHeapEx(gAppHeap, memorySize, 16);

        if (memory != NULL)
        {
            u8* binary = memory + ROUND_UP(sizeof(NNSFndArchive), 16);

            // �A�[�J�C�u��W�J���܂�
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
        // ���k�f�[�^��������܂��B
        NNS_FndFreeToExpHeap(gAppHeap, compData);
    }
    return archive;
}

/*---------------------------------------------------------------------------*
  Name:         RemoveArchive

  Description:  �w�肳�ꂽ�A�[�J�C�u������������폜���܂��B
  
                �w�肳�ꂽ�A�[�J�C�u���t�@�C���V�X�e������A���}�E���g���A�A
                �[�J�C�u���ǂݍ��܂�Ă�����������������܂��B
                
  Arguments:    archive:    NNS�A�[�J�C�u�\���̂ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
RemoveArchive(NNSFndArchive* archive)
{
    (void)NNS_FndUnmountArchive(archive);
    NNS_FndFreeToExpHeap(gAppHeap, archive);
}

/*---------------------------------------------------------------------------*
  Name:         ReadGamePad

  Description:  �L�[��ǂݍ��݁A�g���K�ƃ����[�X�g���K�����߂܂��B
                
  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void ReadGamePad(void)
{
    u16 status = PAD_Read();

    gGamePad.trigger = (u16)(status          & (status ^ gGamePad.button));
    gGamePad.release = (u16)(gGamePad.button & (status ^ gGamePad.button));
    gGamePad.button  = status;
}

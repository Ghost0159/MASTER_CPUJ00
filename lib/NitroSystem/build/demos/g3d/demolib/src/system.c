/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - demolib
  File:     system.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/

#include "g3d_demolib/system.h"

#define DEFAULT_DMA_NUMBER      MI_DMA_MAX_NUM
#define SYSTEM_HEAP_SIZE        64*1024


G3DDemoGamePad   G3DDemo_GamePad;       // �Q�[���p�b�h
NNSFndHeapHandle G3DDemo_SysHeap;       // �V�X�e���q�[�v�@�@�@�@�i�g���q�[�v���g�p�j
NNSFndHeapHandle G3DDemo_AppHeap;       // �A�v���P�[�V�����q�[�v�i�g���q�[�v���g�p�j


/*---------------------------------------------------------------------------*
  Name:         G3DDemo_VBlankIntr

  Description:  V�u�����N�̃R�[���o�b�N�����[�`���B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
G3DDemo_VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitSystem

  Description:  NITRO�̏��������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
G3DDemo_InitSystem(void)
{
    OS_Init();
    FX_Init();
    GX_SetPower(GX_POWER_ALL);
    GX_Init();
    OS_InitTick();

    GX_DispOff();
    GXS_DispOff();

    OS_SetIrqFunction(OS_IE_V_BLANK, G3DDemo_VBlankIntr);

    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);
    (void)OS_EnableIrq();

    FS_Init(DEFAULT_DMA_NUMBER);

    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request

    // �Q�[���p�b�h����ǂ݂��܂��B
    G3DDemo_ReadGamePad();
}

#define SYSTEM_HEAP_SIZE        64*1024

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitVRAM

  Description:  �u�q�`�l�̏��������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
G3DDemo_InitVRAM(void)
{
    // �S�Ẵo���N��LCDC�Ɋ��蓖�Ă�
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);

    // LCDC��ԑS�Ă��N���A
    MI_CpuClearFast( (void*)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE );
    
    // LCDC�Ɋ��蓖�Ă�ꂽ�o���N��Disable
    (void)GX_DisableBankForLCDC();

    MI_CpuFillFast ((void*)HW_OAM    , 192, HW_OAM_SIZE   );  // OAM �N���A
    MI_CpuFillFast ((void*)HW_DB_OAM , 192, HW_DB_OAM_SIZE);  // clear OAM

    MI_CpuClearFast((void*)HW_PLTT   , HW_PLTT_SIZE   );      // �p���b�g �N���A
    MI_CpuClearFast((void*)HW_DB_PLTT, HW_DB_PLTT_SIZE);      // clear the standard palette
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitMemory

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
G3DDemo_InitMemory(void)
{
    void* sysHeapMemory = OS_AllocFromMainArenaLo(SYSTEM_HEAP_SIZE, 16);

    u32   arenaLow      = G3DDEMO_ROUND_UP  (OS_GetMainArenaLo(), 16);
    u32   arenaHigh     = G3DDEMO_ROUND_DOWN(OS_GetMainArenaHi(), 16);
    u32   appHeapSize   = arenaHigh - arenaLow;
    void* appHeapMemory = OS_AllocFromMainArenaLo(appHeapSize, 16);

    G3DDemo_SysHeap = NNS_FndCreateExpHeap(sysHeapMemory, SYSTEM_HEAP_SIZE);
    G3DDemo_AppHeap = NNS_FndCreateExpHeap(appHeapMemory, appHeapSize      );
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_LoadFile

  Description:  �t�@�C���̓ǂݍ���
  
  Arguments:    path        �t�@�C���ւ̃p�X

  Returns:      �t�@�C���ǂݍ��ݐ�A�h���X
 *---------------------------------------------------------------------------*/
void* G3DDemo_LoadFile(const char *path)
{
    FSFile file;
    void*  memory;

    FS_InitFile(&file);
    if (FS_OpenFile(&file, path))
    {
        u32 fileSize = FS_GetLength(&file);

        memory = NNS_FndAllocFromExpHeapEx(G3DDemo_AppHeap, fileSize, 16);
        if (memory == NULL)
        {
            OS_Printf("no enough memory.\n");
        }
        else
        {
            if (FS_ReadFile(&file, memory, (s32)fileSize) != fileSize)   // �t�@�C���T�C�Y���ǂݍ��߂Ă��Ȃ��ꍇ
            {
                NNS_FndFreeToExpHeap(G3DDemo_AppHeap, memory);
                memory = NULL;
                OS_Printf("file reading failed.\n");
            }
        }
        (void)FS_CloseFile(&file);
    }
    return memory;
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_ReadGamePad

  Description:  �L�[��ǂݍ��݁A�g���K�ƃ����[�X�g���K�����߂܂��B
                
  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G3DDemo_ReadGamePad(void)
{
    u16 status = PAD_Read();

    G3DDemo_GamePad.trigger = (u16)(status                 & (status ^ G3DDemo_GamePad.button));
    G3DDemo_GamePad.release = (u16)(G3DDemo_GamePad.button & (status ^ G3DDemo_GamePad.button));
    G3DDemo_GamePad.button  = status;
}

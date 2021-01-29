/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - mcs - basic - nitro
  File:     nns_util.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

#include "nns_util.h"
#include <nnsys/misc.h>

GamePad          gGamePad;      // �Q�[���p�b�h
NNSFndHeapHandle gAppHeap;      // �A�v���P�[�V�����q�[�v�i�g���q�[�v���g�p�j


/*---------------------------------------------------------------------------*
  Name:         InitMemory

  Description:  �A�v���P�[�V�����q�[�v���쐬���܂��B���̃q�[�v��
                NITRO-System�̊g���q�[�v���g�p���Ă��܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
InitMemory(void)
{
    u32   arenaLow      = ROUND_UP  (OS_GetMainArenaLo(), 16);
    u32   arenaHigh     = ROUND_DOWN(OS_GetMainArenaHi(), 16);
    u32   appHeapSize   = arenaHigh - arenaLow;
    void* appHeapMemory = OS_AllocFromMainArenaLo(appHeapSize, 16);

    gAppHeap = NNS_FndCreateExpHeap(appHeapMemory, appHeapSize     );
}


/*---------------------------------------------------------------------------*
  Name:         LoadFile

  Description:  �o�b�t�@���m�ۂ��ăt�@�C����ǂݍ��݂܂��B

  Arguments:    filename:  �ǂݍ��ރt�@�C����

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���̓��e���i�[���Ă���o�b�t�@�ւ�
                �|�C���^��Ԃ��܂��B���̃o�b�t�@�͕s�K�v�ɂȂ����Ƃ���
                NNS_FndFreeToExpHeap �ŊJ�����Ȃ���΂Ȃ�܂���B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static void*
LoadFile(const char* filename)
{
    FSFile  file;
    void*   dataBuf = NULL;
    BOOL    bSuccess;

    SDK_NULL_ASSERT(filename);

    FS_InitFile(&file);
    bSuccess = FS_OpenFile(&file, filename);

    if(bSuccess)
    {
        const u32 fileSize = FS_GetLength(&file);

        dataBuf = NNS_FndAllocFromExpHeapEx(gAppHeap, fileSize, 16);
        NNS_NULL_ASSERT(dataBuf);

        if(fileSize != FS_ReadFile(&file, dataBuf, (s32)fileSize))
        {
            NNS_FndFreeToExpHeap(gAppHeap, dataBuf);
            dataBuf = NULL;
        }
        else
        {
            DC_FlushRange(dataBuf, fileSize);
        }

        bSuccess = FS_CloseFile(&file);
        NNS_ASSERT(bSuccess);
    }else{
        OS_Warning("Can't find the file : %s ", filename);
    }

    return dataBuf;
}

/*---------------------------------------------------------------------------*
  Name:         LoadPicture

  Description:  �t�@�C����ǂݍ��݃L�����N�^�A�J���[�p���b�g�A�X�N���[����
                �e�f�[�^��VRAM�ɓ]�����܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void
LoadPicture(void)
{
    void* chrData = LoadFile("/data/dog256.chr");
    void* scnData = LoadFile("/data/dog256.scn");
    void* palData = LoadFile("/data/dog256.pal");

    // �f�[�^��VRAM�̃��[�h�B
    GX_LoadBG0Char(chrData, 0, 0x10000);
    GX_LoadBG0Scr (scnData, 0, 0x00800);
    GX_LoadBGPltt (palData, 0, 0x00200);
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

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - file-3
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.3  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.2  2005/11/21 10:23:07  kitase_hirotake
  OS_InitPrintServer �̍폜

  Revision 1.1  2005/06/28 02:12:42  yosizaki
  initial upload.


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>


/*****************************************************************************/
/* variable */

/* use DMA-3 for FS */
static const u32 default_dma_no = MI_DMA_MAX_NUM;


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V�u�����N�R�[���o�b�N

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         my_fgets

  Description:  C �W���֐� fgets() �̗ގ���.

  Arguments:    buf            ������i�[��o�b�t�@
                len            buf �̃T�C�Y
                p_file         �ǂݏo�� FSFile �\����

  Returns:      �������ǂݏo����� buf, �����łȂ���� NULL.
 *---------------------------------------------------------------------------*/
static char *my_fgets(char *buf, int len, FSFile *p_file)
{
    int     ret = FS_ReadFile(p_file, buf, len - 1);
    if (ret < 0)
        return NULL;
    else
    {
        int     i;
        for (i = 0; i < ret; ++i)
        {
            u32     c = MI_ReadByte(buf + i);
            if (c == '\n')
            {
                ++i;
                break;
            }
        }
        MI_WriteByte(buf + i, (u8)'\0');
        (void)FS_SeekFile(p_file, i - ret, FS_SEEK_CUR);
    }
    return buf;
}

/*---------------------------------------------------------------------------*
  Name:         InitializeAllocateSystem

  Description:  ���C����������̃A���[�i�ɂă����������ăV�X�e��������������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �A�v���P�[�V�������C���G���g��

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    OS_Init();
    OS_InitThread();
    InitializeAllocateSystem();

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);
    (void)OS_EnableIrq();

    /* initialize file-system */
    FS_Init(default_dma_no);

    /* always preload FS table for faster directory access. */
    {
        u32     need_size = FS_GetTableSize();
        void   *p_table = OS_Alloc(need_size);
        SDK_ASSERT(p_table != NULL);
        (void)FS_LoadTable(p_table, need_size);
    }


    OS_Printf("\n"
              "++++++++++++++++++++++++++++++++++++++++\n"
              "test 1 : create temporary file from memory... \n\n");
    {
        FSFile  file;
        BOOL    ret;
        char    buf[256];

        static char map_mem[] =
            "here is memory buffer read as file.\n"
            "***********************************\n" "***********************************\n";

        /* �������t�@�C�����J�� */
        FS_InitFile(&file);
        ret = FS_CreateFileFromMemory(&file, map_mem, sizeof(map_mem));
        SDK_ASSERT(ret);
        OS_Printf("{\n");
        while (my_fgets(buf, sizeof(buf), &file) && (*buf != '\0'))
            OS_Printf("    %s", buf);
        OS_Printf("\n}\n");

        /* ���C�g�A�N�Z�X�œ��e��ς��� */
        (void)FS_SeekFileToBegin(&file);
        while (FS_WriteFile(&file, "#", 1) > 0)
        {
        }
        (void)FS_SeekFileToBegin(&file);
        OS_Printf("{\n");
        while (my_fgets(buf, sizeof(buf), &file) && (*buf != '\0'))
            OS_Printf("    %s", buf);
        OS_Printf("\n}\n");

        ret = FS_CloseFile(&file);
        SDK_ASSERT(ret);
    }

    OS_Printf("\n"
              "++++++++++++++++++++++++++++++++++++++++\n"
              "test 2 : create temporary file from ROM... \n\n");
    {
        FSFile  file, src;
        BOOL    ret;
        char    buf[256];


        FS_InitFile(&src);
        ret = FS_OpenFile(&src, "rom:/main.c");
        SDK_ASSERT(ret);

        FS_InitFile(&file);
        ret = FS_CreateFileFromRom(&file, FS_GetFileImageTop(&src), FS_GetLength(&src));
        SDK_ASSERT(ret);

        ret = FS_CloseFile(&src);
        SDK_ASSERT(ret);

        OS_Printf("{\n");
        while (my_fgets(buf, sizeof(buf), &file) && (*buf != '\0'))
            OS_Printf("    %s", buf);
        OS_Printf("\n}\n");

        ret = FS_CloseFile(&file);
        SDK_ASSERT(ret);

    }

    OS_Printf("\n" "++++++++++++++++++++++++++++++++++++++++\n" "end\n\n");

    OS_Terminate();

}

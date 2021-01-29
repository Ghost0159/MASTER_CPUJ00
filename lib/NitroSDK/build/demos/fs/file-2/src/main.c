/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - file-2
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.16  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.15  2005/11/21 10:22:48  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.14  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.13  2004/05/13 04:06:26  yosizaki
  change to multi-thread.

  Revision 1.12  2004/05/11 04:27:36  yosizaki
  change format of FSDirPos and FSFileID.

  Revision 1.11  2004/05/10 06:30:02  yosizaki
  fix FS_InitFile().

  Revision 1.10  2004/04/22 03:12:02  yosizaki
  fix around OS_InitAlloc().

  Revision 1.9  2004/04/22 02:30:20  yosizaki
  add FS_LoadTable().

  Revision 1.8  2004/04/19 06:50:33  yosizaki
  fix around TEG-CARD.

  Revision 1.7  2004/04/13 12:08:30  yosizaki
  fix header comment.

  Revision 1.6  2004/04/08 11:09:14  yosizaki
  change FSDir to FSFile

  Revision 1.5  2004/04/07 00:22:47  yosizaki
  fix warning "variable is not used"

  Revision 1.4  2004/04/06 11:58:33  yosizaki
  add tests for FS_TellDir(), FS_SeekDir(), FS_RewindDir()

  Revision 1.3  2004/04/06 06:58:28  yosizaki
  change FS_Init()

  Revision 1.2  2004/04/02 00:41:34  yosizaki
  fix around byte-access code.

  Revision 1.1  2004/04/01 11:26:19  yosizaki
  (none)


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/fs.h>


/* use DMA-3 for FS */
static const u32 default_dma_no = MI_DMA_MAX_NUM;


/* V-blank callback */
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/* similar to fgets */
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

/* list the directories and files */
static void RecursiveEnumDir(int tab, FSDirEntry *pe)
{
    FSFile  d;
    FS_InitFile(&d);
    /* in the recursive function, it costs much stack.
       so we use one FSDirEntry like this. */
    OS_Printf("%*s%s/\n", tab, "", pe->name);
    if (FS_SeekDir(&d, &pe->dir_id))
    {
        tab += 4;
        while (FS_ReadDir(&d, pe))
        {
            if (pe->is_directory)
                RecursiveEnumDir(tab, pe);
            else
                OS_Printf("%*s%s\n", tab, "", pe->name);
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         InitializeAllocateSystem

  Description:  メインメモリ上のアリーナにてメモリ割当てシステムを初期化する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    // OS_Initは呼ばれているという前提
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

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
              "test 1 : open specified files ... \n\n");
    {
        static const char *(paths[]) =
        {
        "/data\\hello.txt",
                "data/test1.txt", "/./data/../.\\.\\DAta\\.\\tesT2.tXT", "/src/main.c",};

        int     i;
        for (i = 0; i < sizeof(paths) / sizeof(*paths); ++i)
        {
            FSFile  file;
            const char *path = paths[i];
            BOOL    open_is_ok;
            FS_InitFile(&file);
            open_is_ok = FS_OpenFile(&file, path);
            OS_Printf("FS_OpenFile(\"%s\") ... %s!\n", path, open_is_ok ? "OK" : "ERROR");
            if (open_is_ok)
            {
                char    buf[256];
                OS_Printf("{\n");
                while (my_fgets(buf, sizeof(buf), &file) && (*buf != '\0'))
                    OS_Printf("    %s", buf);
                OS_Printf("\n}\n");
            }
        }

    }


    OS_Printf("\n"
              "++++++++++++++++++++++++++++++++++++++++\n" "test 2 : query directories ... \n\n");
    {
        FSFile  d;
        FSDirEntry e;

        FS_InitFile(&d);
        (void)FS_FindDir(&d, "");

        MI_WriteByte(e.name + 0, (u8)'\0');
        (void)FS_TellDir(&d, &e.dir_id);
        RecursiveEnumDir(0, &e);
    }


    OS_Printf("\n"
              "++++++++++++++++++++++++++++++++++++++++\n"
              "test 3 : FS_TellDir() and FS_SeekDir() ... \n\n");
    {
        FSFile  d;
        FSDirEntry e;
        FSDirPos pos_list[10];
        int     file_count = 0;

        BOOL    ret;

        FS_InitFile(&d);
        ret = FS_FindDir(&d, "data/");
        SDK_ASSERT(ret);

        for (;;)
        {
            FSDirPos pos;
            (void)FS_TellDir(&d, &pos);
            if (!FS_ReadDir(&d, &e))
                break;
            if (e.is_directory)
                continue;
            if (file_count < sizeof(pos_list) / sizeof(*pos_list))
            {
                pos_list[file_count] = pos;
                OS_Printf("TellDir[%d] : %s\n", file_count, e.name);
                ++file_count;
            }
        }
        ret = FS_RewindDir(&d);
        SDK_ASSERT(ret);
        ret = FS_ReadDir(&d, &e);
        SDK_ASSERT(ret);
        OS_Printf("RewindDir : %s\n", e.name);

        while (--file_count >= 0)
        {
            ret = FS_SeekDir(&d, &pos_list[file_count]);
            SDK_ASSERT(ret);
            ret = FS_ReadDir(&d, &e);
            SDK_ASSERT(ret);
            OS_Printf("SeekDir[%d] : %s\n", file_count, e.name);
        }

    }


    OS_Printf("\n" "++++++++++++++++++++++++++++++++++++++++\n" "end\n\n");

    for (;;)
    {
        OS_WaitVBlankIntr();
    }

}

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - libfsdemo
  File:     libfsdemo_util.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: libfsdemo_util.c,v $
  Revision 1.6  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.5  2005/11/21 10:24:01  kitase_hirotake
  OS_InitPrintServer �̍폜

  Revision 1.4  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.3  2004/11/02 10:24:20  yosizaki
  fix comment.

  Revision 1.2  2004/07/12 13:15:19  yosizaki
  add some functions.

  Revision 1.1  2004/07/09 00:57:23  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include "util.h"


/* V-blank callback */
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/* initialize apprication (arena and V-blank callback) */
void LIBFSDEMO_Init(void)
{
    void   *tmp;
    OSHeapHandle hh;

    OS_Init();
    OS_InitThread();
    (void)OS_EnableIrq();

    tmp = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tmp);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
        OS_Panic("ARM9: Fail to create heap...\n");
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
}


/* similar to fgets */
char   *LIBFSDEMO_fgets(char *buf, int len, FSFile *p_file)
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

/* similar to strlen */
int LIBFSDEMO_strlen(const char *str)
{
    int     i;
    for (i = 0; MI_ReadByte(str + i); ++i) ;
    return i;
}

/* list the directories and files (recursive sub-function) */
static void LIBFSDEMO_DumpDirectorySub(int tab, FSDirEntry *pe)
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
                LIBFSDEMO_DumpDirectorySub(tab, pe);
            else
                OS_Printf("%*s%s\n", tab, "", pe->name);
        }
    }
}

/* list the directories and files */
void LIBFSDEMO_DumpDirectory(FSFile *p_dir)
{
    FSDirEntry e;
    MI_WriteByte(e.name + 0, (u8)'\0');
    (void)FS_TellDir(p_dir, &e.dir_id);
    LIBFSDEMO_DumpDirectorySub(0, &e);
}

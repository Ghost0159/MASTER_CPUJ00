/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - arc-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.5  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.4  2005/11/21 10:21:57  kitase_hirotake
  OS_PrintServer ÇÃçÌèú
  SVC_WaitVBlankIntr Ç OS_WaitVBlankIntr Ç…ïœçX

  Revision 1.3  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.2  2005/02/18 08:57:08  yosizaki
  fix around hidden warnings.

  Revision 1.1  2004/07/09 01:00:17  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/fs.h>


#include "../libfsdemo/include/util.h"
#include "myrom.h"


/* use DMA-3 for FS */
static const u32 default_dma_no = MI_DMA_MAX_NUM;

/* custom ROM archives */

static MyRomArchive myrom_dup;         /* duplicate "rom" */
static MyRomArchive myrom_sub;         /* load NitroROM of sub-program binary */


void NitroMain(void)
{
    LIBFSDEMO_Init();

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
              "test 1 : query default \"rom\" directories ... \n\n");
    {
        static const char name[] = "rom";
        FSFile  d;
        char    buf[256];
        FS_InitFile(&d);
        (void)OS_SPrintf(buf, "%s:/", name);
        (void)FS_ChangeDir(buf);
        (void)FS_FindDir(&d, "");
        LIBFSDEMO_DumpDirectory(&d);
    }


    OS_Printf("\n"
              "++++++++++++++++++++++++++++++++++++++++\n"
              "test 2 : query duplication archive \"dup\" directories ... \n\n");
    {
        static const char name[] = "dup";

        const u32 base = 0;
        const CARDRomRegion *fnt = CARD_GetRomRegionFNT();
        const CARDRomRegion *fat = CARD_GetRomRegionFAT();

        MyRom_Init(&myrom_dup);
        if (!MyRom_Create(&myrom_dup, name, sizeof(name) - 1, base, fnt, fat))
        {
            OS_Panic("failed to load archive %s!", name);
        }
        else
        {
            FSFile  d;
            char    buf[256];
            FS_InitFile(&d);
            (void)OS_SPrintf(buf, "%s:/", name);
            (void)FS_ChangeDir(buf);
            (void)FS_FindDir(&d, "");
            LIBFSDEMO_DumpDirectory(&d);
        }
    }

    OS_Printf("\n"
              "++++++++++++++++++++++++++++++++++++++++\n"
              "test 3 : query sub-program archive \"sub\" directories ... \n\n");
    {
        FSFile  sub_binary;
        FS_InitFile(&sub_binary);
        if (!FS_OpenFile(&sub_binary, "rom:/main.srl"))
        {
            OS_Panic("failed to open sub-program binary");
        }
        else
        {
            static const char name[] = "sub";
            /* get offset of sub-program */
            const u32 base = FS_GetFileImageTop(&sub_binary);
            CARDRomRegion fnt;
            CARDRomRegion fat;
            (void)FS_SeekFile(&sub_binary,
                              (int)((u32)CARD_GetRomRegionFNT() - HW_ROM_HEADER_BUF), FS_SEEK_SET);
            (void)FS_ReadFile(&sub_binary, &fnt, sizeof(fnt));
            (void)FS_SeekFile(&sub_binary,
                              (int)((u32)CARD_GetRomRegionFAT() - HW_ROM_HEADER_BUF), FS_SEEK_SET);
            (void)FS_ReadFile(&sub_binary, &fat, sizeof(fat));
            (void)FS_CloseFile(&sub_binary);

            MyRom_Init(&myrom_sub);
            if (!MyRom_Create(&myrom_sub, name, sizeof(name) - 1, base, &fnt, &fat))
            {
                OS_Panic("failed to load archive %s!", name);
            }
            else
            {
                FSFile  d;
                char    buf[256];
                FS_InitFile(&d);
                (void)OS_SPrintf(buf, "%s:/", name);
                (void)FS_ChangeDir(buf);
                (void)FS_FindDir(&d, "");
                LIBFSDEMO_DumpDirectory(&d);
            }
        }

    }


    OS_Printf("\n" "++++++++++++++++++++++++++++++++++++++++\n" "end\n\n");

    for (;;)
    {
        OS_WaitVBlankIntr();
    }

}

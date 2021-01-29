/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - arc-2
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

  Revision 1.4  2005/11/21 10:22:11  kitase_hirotake
  OS_PrintServer ÇÃçÌèú
  SVC_WaitVBlankIntr Ç OS_WaitVBlankIntr Ç…ïœçX

  Revision 1.3  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.2  2005/02/18 08:57:15  yosizaki
  fix around hidden warnings.

  Revision 1.1  2004/07/12 13:18:31  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/fs.h>


#include "../libfsdemo/include/util.h"


/* use DMA-3 for FS */
static const u32 default_dma_no = MI_DMA_MAX_NUM;


/* custom ROM archives **********************************************/

struct FixedFile;
struct FixedDirectory;
struct FixedDirectoryList;

/* custom file image structure */
typedef struct FixedFile
{
    char    buffer[256];
}
FixedFile;

/* custom directory structure (equal to each directory entry of FNT) */
typedef struct FixedDirectory
{
    const char *name;
    union
    {
        void   *common_ptr;            /* for use of array-initializer */
        struct FixedDirectoryList *dir;
        FixedFile *file;
    }
    obj;
}
FixedDirectory;

/* custom directory-list structure (equal to table of FNT) */
typedef struct FixedDirectoryList
{
    int     parent;
    FixedDirectory *own;
}
FixedDirectoryList;

/* instance of FAT and FNT */
extern FixedFile file_list[];
extern FixedDirectoryList dir_list[];

extern const int file_max;
extern const int dir_max;


/* file images (simple text data within 256 BYTE) */
static FixedFile file_list[] = {
    {
     "hello, world!\n" "fixed file of root.\n"},
    {
     "fixed file 1.\n"},
    {
     "fixed file 2.\n"},
    {
     "fixed file 3.\n"},
};

/* directory blocks (read by FS_ReadDir) */
static FixedDirectory sub_dir1[] = {
    {"file1.txt", file_list + 1,},
    {"file2.txt", file_list + 2,},
    {"file3.txt", file_list + 3,},
    {NULL,},
};
static FixedDirectory sub_dir2[] = {
    {"test1.txt", file_list + 1,},
    {"test2.txt", file_list + 2,},
    {"test3.txt", file_list + 3,},
    {NULL,},
};
static FixedDirectory root_dir[] = {
    {"root.txt", file_list + 0,},
    {"sub1", dir_list + 1,},
    {"sub2", dir_list + 2,},
    {NULL,},
};

/* FNT header */
FixedDirectoryList dir_list[] = {
    {-1, root_dir,},
    {0, sub_dir1,},
    {0, sub_dir2,},
};

const int dir_max = sizeof(dir_list) / sizeof(*dir_list);
const int file_max = sizeof(file_list) / sizeof(*file_list);


static FSArchive custom_rom;

/* read/write callbacks*/
static FSResult CustomRom_ReadCallback(FSArchive *p_arc, void *dst, u32 pos, u32 size)
{
    (void)p_arc;
    MI_CpuCopy8((const void *)pos, dst, size);
    return FS_RESULT_SUCCESS;
}
static FSResult CustomRom_WriteCallback(FSArchive *p_arc, const void *src, u32 pos, u32 size)
{
    (void)p_arc;
    MI_CpuCopy8(src, (void *)pos, size);
    return FS_RESULT_SUCCESS;
}

/* user-procedure */
static FSResult CustomRom_ArchiveProc(FSFile *p_file, FSCommandType cmd)
{
    FSArchive *const p_rom = FS_GetAttachedArchive(p_file);

    switch (cmd)
    {

        /*
         * basic directory commands and file commands.
         *
         * although each command returns faithfully
         * the specification which file-system requires,
         * FAT and FNT are built by original specification.
         */

    case FS_COMMAND_SEEKDIR:
        {
            const FSDirPos *const arg = &p_file->arg.seekdir.pos;
            const int dir_id = arg->own_id;

            /*
             * [ file-system specification ]
             * "seek to directory position of 'pos'
             * which FS_ReadDir() or FS_TellDir() return".
             */
            p_file->prop.dir.pos = *arg;
            /*
             * [ file-system specification ]
             * "if both 'index' and 'pos' are 0, seek to first entry".
             */
            if ((arg->index == 0) && (arg->pos == 0))
            {
                p_file->prop.dir.pos.index = 0; /* not use */
                p_file->prop.dir.pos.pos = (u32)dir_list[dir_id].own;
            }
            /*
             * [ file-system specification ]
             * "if root directory, parent-ID means total directory count".
             */
            if (dir_id != 0)
                p_file->prop.dir.parent = (u16)dir_list[dir_id].parent;
            else
                p_file->prop.dir.parent = dir_max;
            return FS_RESULT_SUCCESS;
        }

    case FS_COMMAND_READDIR:
        {
            FSDirEntry *const p_entry = p_file->arg.readdir.p_entry;
            const FixedDirectory *const cur = (const FixedDirectory *)p_file->prop.dir.pos.pos;

            /*
             * [ file-system specification ]
             * "if end of directory-entry, return FS_RESULT_FAILURE".
             */
            if (!cur->name)
                return FS_RESULT_FAILURE;
            /* get name length */
            p_entry->name_len = (u32)LIBFSDEMO_strlen(cur->name);
            /*
             * [ file-system specification ]
             * "if 'skip_string' is 0, copy name string data with '\0'".
             */
            if (!p_file->arg.readdir.skip_string)
                MI_CpuCopy8(cur->name, p_entry->name, p_entry->name_len + 1);
            /* 
             * [ file-system specification ]
             * "if current entry is directory, set FSDirPos".
             */
            if ((cur->obj.dir >= dir_list) && (cur->obj.dir < dir_list + dir_max))
            {
                p_entry->is_directory = 1;
                p_entry->dir_id.arc = p_file->arc;
                p_entry->dir_id.own_id = (u16)(cur->obj.dir - dir_list);
                p_entry->dir_id.index = 0;
                p_entry->dir_id.pos = 0;
            }
            /*
             * [ file-system specification ]
             * "if current entry is file, set FSFileID".
             */
            else
            {
                p_entry->is_directory = 0;
                p_entry->file_id.arc = p_file->arc;
                p_entry->file_id.file_id = (u32)(cur->obj.file - file_list);
            }
            /*
             * [ file-system specification ]
             * "if read current-entry correctly, increment current position of entry.
             *  otherwise, keep current position.
             *  in both case, FS_TellDir() can refer to the position after".
             */
            p_file->prop.dir.pos.pos = (u32)(cur + 1);
            return FS_RESULT_SUCCESS;
        }

    case FS_COMMAND_OPENFILEFAST:
        {
            const int id = (int)p_file->arg.openfilefast.id.file_id;

            /*
             * [ file-system specification ]
             * "if invalid ID, return FS_RESULT_FAILURE".
             */
            if ((id < 0) || (id >= file_max))
                return FS_RESULT_FAILURE;
            else
            {
                const char *text = file_list[id].buffer;
                p_file->arg.openfiledirect.top = (u32)text;
                p_file->arg.openfiledirect.bottom = (u32)(text + LIBFSDEMO_strlen(text));
                p_file->arg.openfiledirect.index = (u32)id;
            }
            /* fall to FS_COMMAND_OPENFILEDIRECT */
        }

    case FS_COMMAND_OPENFILEDIRECT:
        {
            /* file region is set by memory address */
            p_file->prop.file.top = p_file->arg.openfiledirect.top;
            p_file->prop.file.pos = p_file->arg.openfiledirect.top;
            p_file->prop.file.bottom = p_file->arg.openfiledirect.bottom;
            p_file->prop.file.own_id = p_file->arg.openfiledirect.index;
            return FS_RESULT_SUCCESS;
        }

        /*
         * upper-layer commands or optional message commands.
         *
         * as returning "unknown",
         * some are substituted by standard routine
         * which uses basic commands (we implemented!),
         * others are simply ignored.
         */

    default:
    case FS_COMMAND_FINDPATH:
    case FS_COMMAND_GETPATH:
    case FS_COMMAND_CLOSEFILE:
    case FS_COMMAND_READFILE:
    case FS_COMMAND_WRITEFILE:
    case FS_COMMAND_ACTIVATE:
    case FS_COMMAND_IDLE:
        return FS_RESULT_PROC_UNKNOWN;

    }

}

/* setup */
static BOOL CreateCustomArchive(const char *name)
{
    const int name_len = LIBFSDEMO_strlen(name);
    FS_InitArchive(&custom_rom);

    if (!FS_RegisterArchiveName(&custom_rom, name, (u32)name_len))
    {
        OS_Panic("error! FS_RegisterArchiveName(%*s) failed.\n", name_len, name);
        return FALSE;
    }
    FS_SetArchiveProc(&custom_rom, CustomRom_ArchiveProc, (u32)FS_ARCHIVE_PROC_ALL);
    if (!FS_LoadArchive(&custom_rom, 0,
                        /*
                         * we implemented all of basic commands that need FAT or FNT.
                         * parameters are not used.
                         */
                        NULL, 0, NULL, 0, CustomRom_ReadCallback, CustomRom_WriteCallback))
    {
        OS_Panic("error! FS_LoadArchive() failed.\n");
        return FALSE;
    }
    return TRUE;
}



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
              "test 2 : query custom archive \"cst\" directories ... \n\n");
    {
        static const char name[] = "cst";
        if (!CreateCustomArchive(name))
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

    OS_Printf("\n" "++++++++++++++++++++++++++++++++++++++++\n" "end\n\n");

    for (;;)
    {
        OS_WaitVBlankIntr();
    }

}

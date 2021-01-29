/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - async
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.9  2006/05/08 01:32:14  yosizaki
  fix about file buffer.

  Revision 1.8  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.7  2005/11/21 10:22:30  kitase_hirotake
  OS_PrintServer �̍폜

  Revision 1.6  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.5  2005/02/18 08:57:23  yosizaki
  fix around hidden warnings.

  Revision 1.4  2004/07/19 11:41:50  yosizaki
  change to use libfsdemo.

  Revision 1.3  2004/06/22 02:00:14  yosizaki
  fix position of OS_Printf() in thread.

  Revision 1.2  2004/05/28 03:19:38  yosizaki
  fix debug-output of total size.

  Revision 1.1  2004/05/20 10:01:58  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/fs.h>


#include "../libfsdemo/include/util.h"




/* use DMA-3 for FS */
static const u32 default_dma_no = MI_DMA_MAX_NUM;


/********************************************************************
 *
 * asynchronous function checker thread.
 *
 * if(ROM on cartridge)  always synchronous operation.
 * else if(!TEG && !DMA) always synchronous operation.
 *
 * when synchronous operation, only thread[0] runs.
 *
 ********************************************************************/

enum
{
    data_00,
    data_ff,
    data_inc,
    data_dec,
    datamode_max
};

#define	TASK_THREAD_MAX	4

#define	TASK_REPEAT_TIMES	1

typedef struct
{
    OSThread thread;
    u8      stack[4096] ATTRIBUTE_ALIGN(32);
    u8      buf[CARD_ROM_PAGE_SIZE] ATTRIBUTE_ALIGN(32);
    int     mode;
    u32     priority;
    u8      padding[24];
}
TaskThread;


static TaskThread task_thread[TASK_THREAD_MAX] ATTRIBUTE_ALIGN(32);

static u32 total_read_size = 0;


/* endless loop to check asynchronous read operation */
static void AsyncAccessTask(void *p)
{
    TaskThread *const pt = (TaskThread *) p;
    int     i;
    int     repeat;
    int     count;
    int     total_size;
    BOOL    ret;
    FSFile  file;
    count = 0;
    repeat = 0;
    total_size = 0;
    for (;;)
    {
        /* open specified file */
        switch (pt->mode)
        {
        case data_00:
            ret = FS_OpenFile(&file, "data/00.bin");
            break;
        case data_ff:
            ret = FS_OpenFile(&file, "data/ff.bin");
            break;
        case data_inc:
            ret = FS_OpenFile(&file, "data/inc.bin");
            break;
        case data_dec:
            ret = FS_OpenFile(&file, "data/dec.bin");
            break;
        default:
            ret = FALSE;
            break;
        }
        SDK_ASSERT(ret);
        /* read all data and check content */
        for (;;)
        {
            int     n = FS_ReadFile(&file, pt->buf, sizeof(pt->buf));
            SDK_ASSERT(n >= 0);
            if (n == 0)
                break;
            switch (pt->mode)
            {
            case data_00:
                for (i = 0; i < n; ++i)
                    SDK_ASSERT(pt->buf[i] == 0x00);
                break;
            case data_ff:
                for (i = 0; i < n; ++i)
                    SDK_ASSERT(pt->buf[i] == 0xFF);
                break;
            case data_inc:
                for (i = 0; i < n; ++i)
                    SDK_ASSERT(pt->buf[i] == (u8)i);
                break;
            case data_dec:
                for (i = 0; i < n; ++i)
                    SDK_ASSERT(pt->buf[i] == (u8)~i);
                break;
            }
            total_size += n;
        }
        ret = FS_CloseFile(&file);
        SDK_ASSERT(ret);
        ++count;
        if (++repeat >= TASK_REPEAT_TIMES)
        {
            u32     current_total;
            OSIntrMode bak_psr = OS_DisableInterrupts();
            total_read_size += total_size;
            current_total = total_read_size;
            total_size = 0;
            OS_Printf("thread[%d](priority=%2d) : %8d times done. (mode=%d) ... total %8d byte\n",
                      pt - task_thread, pt->priority, count, pt->mode, current_total);
            (void)OS_RestoreInterrupts(bak_psr);
            repeat = 0;
            if (++pt->mode >= datamode_max)
                pt->mode = data_00;
        }
    }
}


static void CreateTask(TaskThread * pt, int mode)
{
    pt->mode = mode;
    pt->priority = (u32)(OS_THREAD_LAUNCHER_PRIORITY - (pt - task_thread));
    OS_CreateThread(&pt->thread,
                    AsyncAccessTask, pt,
                    pt->stack + sizeof(pt->stack), sizeof(pt->stack), pt->priority);
    OS_WakeupThreadDirect(&pt->thread);
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
              "test : create %d threads to check asynchronous access ... \n\n", TASK_THREAD_MAX);
    {
        int     i;
        for (i = 0; i < TASK_THREAD_MAX; ++i)
        {
            CreateTask(task_thread + i, data_00 + i);
        }
    }

    for (;;)
    {
        OS_Halt();
    }

}

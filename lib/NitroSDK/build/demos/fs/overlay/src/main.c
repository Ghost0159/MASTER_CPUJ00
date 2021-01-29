/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.14  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.13  2005/11/21 10:23:28  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.12  2005/11/17 01:27:55  adachi_hiroaki
  DMA番号指定時のMI_DMA_NUM+1をFS_DMA_NOT_USEにした

  Revision 1.11  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.10  2004/11/17 05:03:05  yosizaki
  change to use FS_LoadOverlayImageAsync.

  Revision 1.9  2004/10/18 13:30:52  yosizaki
  add sample code to use overlay in different methods.

  Revision 1.8  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.7  2004/04/22 03:12:35  yosizaki
  fix around OS_InitAlloc().

  Revision 1.6  2004/04/22 02:30:27  yosizaki
  add FS_LoadTable().

  Revision 1.5  2004/04/19 06:50:40  yosizaki
  fix around TEG-CARD.

  Revision 1.4  2004/04/13 12:08:02  yosizaki
  add ovaerlay to TCM.
  fix header comment.

  Revision 1.3  2004/04/06 07:00:35  yosizaki
  change FS_Init()
  change around FSOverlayID

  Revision 1.2  2004/04/05 02:49:17  yosizaki
  change argtype of FSLoadOverlay()

  Revision 1.1  2004/04/02 12:21:12  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/fs.h>
#include "func.h"


/* use DMA-3 for FS */
static const u32 default_dma_no = MI_DMA_MAX_NUM;

/* overlay id in *.lcf */
FS_EXTERN_OVERLAY(main_overlay_1);
FS_EXTERN_OVERLAY(main_overlay_2);
FS_EXTERN_OVERLAY(main_overlay_3);

/* overlay table */
static FSOverlayID p_id[] = {
    FS_OVERLAY_ID(main_overlay_1),
    FS_OVERLAY_ID(main_overlay_2),
    FS_OVERLAY_ID(main_overlay_3),
};

/* current overlay id */
static u32 overlay_id;
static FSOverlayID last_loaded_overlay;

static void offset_overlay_id(int i)
{
    const int overlay_max = (int)(sizeof(p_id) / sizeof(*p_id) - 1);
    i += (s32)overlay_id;
    if (i < 0)
        i = overlay_max;
    else if (i > overlay_max)
        i = 0;
    overlay_id = (u32)i;
    OS_Printf("current id = 0x%08X.\n", overlay_id);
}

/* load overlay in various method */
static BOOL load_overlay_high(MIProcessor target, FSOverlayID id)
{
    return FS_LoadOverlay(target, id);
}

static BOOL load_overlay_middle(MIProcessor target, FSOverlayID id)
{
    FSOverlayInfo info;
    if (!FS_LoadOverlayInfo(&info, target, id))
        return FALSE;
    if (!FS_LoadOverlayImage(&info))
        return FALSE;
    FS_StartOverlay(&info);
    return TRUE;
}

static BOOL load_overlay_low(MIProcessor target, FSOverlayID id)
{
    FSOverlayInfo info;
    if (!FS_LoadOverlayInfo(&info, target, id))
        return FALSE;
    {
        FSFile  file;
        FS_InitFile(&file);
        (void)FS_LoadOverlayImageAsync(&info, &file);
        (void)FS_WaitAsync(&file);
        (void)FS_CloseFile(&file);
    }
    FS_StartOverlay(&info);
    return TRUE;
}

#define	load_overlay	load_overlay_low


/* unload module[last_loaded_overlay] and load molude[overlay_id] */
static void load_func(void)
{
    static BOOL is_loaded = FALSE;

    BOOL    ret;
    if (is_loaded)
    {
        /* unload old module */
        ret = FS_UnloadOverlay(MI_PROCESSOR_ARM9, last_loaded_overlay);
        OS_Printf("overlay 0x%08X is unloaded.\n", last_loaded_overlay);
        SDK_ASSERT(ret);
        is_loaded = FALSE;
    }
    /* load new module.
       main_overlay_3 is in ITCM (0x01FF8000), so use CpuCopy(). */
    if (p_id[overlay_id] == FS_OVERLAY_ID(main_overlay_3))
        (void)FS_SetDefaultDMA(FS_DMA_NOT_USE);
    else
        (void)FS_SetDefaultDMA(default_dma_no);

    ret = load_overlay(MI_PROCESSOR_ARM9, p_id[overlay_id]);
    if (!ret)
    {
        OS_Printf("overlay 0x%08X is not available!\n", p_id[overlay_id]);
    }
    else
    {
        is_loaded = TRUE;
        last_loaded_overlay = p_id[overlay_id];
        OS_Printf("overlay 0x%08X is loaded.\n", p_id[overlay_id]);
    }
}

/* V-blank callback */
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
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
    /* initialize application */
    OS_Init();
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


    (void)GX_VBlankIntr(TRUE);

    OS_Printf("\n"
              "++++++++++++++++++++++++++++++++++++++++\n" "test : load overlay modules ... \n\n");

    {
        /* initialize default to func_1 */
        overlay_id = 0;
        load_func();

        /* address of function is specified in main.lsf */
        OS_Printf("func_1() : addr = 0x%08X.\n", func_1);
        OS_Printf("func_2() : addr = 0x%08X.\n", func_2);
        OS_Printf("func_3() : addr = 0x%08X.\n", func_3);

        /* select and load */
        {
            u32     btn = PAD_Read();

            for (;;)
            {

                /* wait key down */
                u32     btn_bak, btn_down;
                btn_bak = btn;
                OS_WaitVBlankIntr();
                btn = PAD_Read();
                btn_down = (u32)(~btn_bak & btn);

                /* if A, call current module */
                if (btn_down & PAD_BUTTON_A)
                {
                    /* if two overlays share the region,
                       FS-system can not guarantee except last-loaded one.
                       please be careful to use OVERLAYed function! */
                    if (last_loaded_overlay == p_id[0])
                        func_1();
                    else if (last_loaded_overlay == p_id[1])
                        func_2();
                    else if (last_loaded_overlay == p_id[2])
                        func_3();
                }

                /* if B, load current id */
                else if (btn_down & PAD_BUTTON_B)
                {
                    load_func();
                }

                /* if DOWN, increment id */
                else if (btn_down & PAD_KEY_DOWN)
                {
                    offset_overlay_id(+1);
                }

                /* if UP, decrement id */
                else if (btn_down & PAD_KEY_UP)
                {
                    offset_overlay_id(-1);
                }

            }
        }
    }

    OS_Printf("\n" "++++++++++++++++++++++++++++++++++++++++\n" "end\n\n");

}

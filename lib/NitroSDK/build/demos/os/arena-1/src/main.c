/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - arena-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.11  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.10  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.9  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.8  2004/03/30 08:38:22  yada
  小修正

  Revision 1.7  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.6  2004/02/13 10:40:36  yada
  ARENAを６個から９個に

  Revision 1.5  2004/02/12 11:41:07  yada
  ARM9/ARM7 振り分け作業

  Revision 1.4  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.3  2004/01/19 02:30:03  yada
  DTCMやMainEX領域のアリーナがないか小さい場合 alloc はしないようにした。

  Revision 1.2  2004/01/16 04:13:48  yada
  Extended MainMemory に対応

  Revision 1.1  2004/01/07 06:18:27  yada
  初版


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

//----------------------------------------------------------------

#define getArenaSize( id )  (u32)OS_GetArenaHi( (id) ) - (u32)OS_GetArenaLo( (id) )

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    void   *nstart;

    OS_Init();
    OS_Printf("================ SAMPLE START ================\n");

    //---- print each arena boundary
    OS_Printf("MainRAM-main arena lo = %x\n", OS_GetMainArenaLo());
    OS_Printf("             arena hi = %x\n", OS_GetMainArenaHi());
    OS_Printf("MainRAM-sub  arena lo = %x\n", OS_GetSubPrivArenaLo());
    OS_Printf("             arena hi = %x\n", OS_GetSubPrivArenaHi());
    OS_Printf("MainEX       arena lo = %x\n", OS_GetMainExArenaLo());
    OS_Printf("             arena hi = %x\n", OS_GetMainExArenaHi());
    OS_Printf("ITCM         arena lo = %x\n", OS_GetITCMArenaLo());
    OS_Printf("             arena hi = %x\n", OS_GetITCMArenaHi());
    OS_Printf("DTCM         arena lo = %x\n", OS_GetDTCMArenaLo());
    OS_Printf("             arena hi = %x\n", OS_GetDTCMArenaHi());
    OS_Printf("SharedM      arena lo = %x\n", OS_GetSharedArenaLo());
    OS_Printf("             arena hi = %x\n", OS_GetSharedArenaHi());
    OS_Printf("WorkRAM-main arena lo = %x\n", OS_GetWramMainArenaLo());
    OS_Printf("             arena hi = %x\n", OS_GetWramMainArenaHi());
    OS_Printf("WorkRAM-sub  arena lo = %x\n", OS_GetWramSubArenaLo());
    OS_Printf("             arena hi = %x\n", OS_GetWramSubArenaHi());
    OS_Printf("Sub PrivWork arena lo = %x\n", OS_GetWramSubPrivArenaLo());
    OS_Printf("             arena hi = %x\n", OS_GetWramSubPrivArenaHi());

    //---- メインメモリアリーナから確保してみる
    OS_Printf("---- now allocate from arena of MainRAM.\n");
    nstart = OS_AllocFromArenaLo(OS_ARENA_MAIN, 0x200, 8);
    OS_Printf("nstart %x\n", nstart);
    nstart = OS_AllocFromArenaHi(OS_ARENA_MAIN, 0x300, 8);
    OS_Printf("nstart %x\n", nstart);
    OS_Printf("MainRAM arena lo = %x\n", OS_GetMainArenaLo());
    OS_Printf("        arena hi = %x\n", OS_GetMainArenaHi());
    OS_Printf("        size     = %x\n", getArenaSize(OS_ARENA_MAIN));

    //---- ITCMアリーナ から確保してみる
    OS_Printf("---- now allocate from arena of ITCM.\n");
    nstart = OS_AllocFromArenaLo(OS_ARENA_ITCM, 0x200, 8);
    OS_Printf("nstart %x\n", nstart);
    nstart = OS_AllocFromArenaHi(OS_ARENA_ITCM, 0x300, 8);
    OS_Printf("nstart %x\n", nstart);
    OS_Printf("ITCM    arena lo = %x\n", OS_GetArenaLo(OS_ARENA_ITCM));
    OS_Printf("        arena hi = %x\n", OS_GetArenaHi(OS_ARENA_ITCM));
    OS_Printf("        size     = %x\n", getArenaSize(OS_ARENA_ITCM));

    //---- DTCMアリーナ から確保してみる
    if ((u32)OS_GetArenaHi(OS_ARENA_DTCM) - (u32)OS_GetArenaLo(OS_ARENA_DTCM) > 0x500)
    {
        OS_Printf("---- now allocate from arena of DTCM.\n");
        nstart = OS_AllocFromArenaLo(OS_ARENA_DTCM, 0x130, 8);
        OS_Printf("nstart %x\n", nstart);
        nstart = OS_AllocFromArenaHi(OS_ARENA_DTCM, 0x330, 8);
        OS_Printf("nstart %x\n", nstart);
        OS_Printf("DTCM    arena lo = %x\n", OS_GetArenaLo(OS_ARENA_DTCM));
        OS_Printf("        arena hi = %x\n", OS_GetArenaHi(OS_ARENA_DTCM));
        OS_Printf("        size     = %x\n", getArenaSize(OS_ARENA_DTCM));
    }

    //---- 共有メモリから確保してみる
    OS_Printf("---- now allocate from arena of Shared Memory.\n");
    nstart = OS_AllocFromArenaLo(OS_ARENA_SHARED, 0x130, 8);
    OS_Printf("nstart %x\n", nstart);
    nstart = OS_AllocFromArenaHi(OS_ARENA_SHARED, 0x330, 8);
    OS_Printf("nstart %x\n", nstart);
    OS_Printf("SharedM arena lo = %x\n", OS_GetArenaLo(OS_ARENA_SHARED));
    OS_Printf("        arena hi = %x\n", OS_GetArenaHi(OS_ARENA_SHARED));
    OS_Printf("        size     = %x\n", getArenaSize(OS_ARENA_SHARED));

    //---- ＷＲＡＭから確保してみる
    OS_Printf("---- now allocate from arena of WorkRAM.\n");
    nstart = OS_AllocFromArenaLo(OS_ARENA_WRAM_MAIN, 0x130, 8);
    OS_Printf("nstart %x\n", nstart);
    nstart = OS_AllocFromArenaHi(OS_ARENA_WRAM_MAIN, 0x330, 8);
    OS_Printf("nstart %x\n", nstart);
    OS_Printf("WRAM    arena lo = %x\n", OS_GetArenaLo(OS_ARENA_WRAM_MAIN));
    OS_Printf("        arena hi = %x\n", OS_GetArenaHi(OS_ARENA_WRAM_MAIN));
    OS_Printf("        size     = %x\n", getArenaSize(OS_ARENA_WRAM_MAIN));

    //---- 拡張メインメモリから確保してみる
    if ((u32)OS_GetArenaHi(OS_ARENA_MAINEX) - (u32)OS_GetArenaLo(OS_ARENA_MAINEX) > 0x500)
    {
        OS_Printf("---- now allocate from arena of Extended MainRAM.\n");
        nstart = OS_AllocFromArenaLo(OS_ARENA_MAINEX, 0x130, 8);
        OS_Printf("nstart %x\n", nstart);
        nstart = OS_AllocFromArenaHi(OS_ARENA_MAINEX, 0x330, 8);
        OS_Printf("nstart %x\n", nstart);
        OS_Printf("MainEX  arena lo = %x\n", OS_GetArenaLo(OS_ARENA_MAINEX));
        OS_Printf("        arena hi = %x\n", OS_GetArenaHi(OS_ARENA_MAINEX));
        OS_Printf("        size     = %x\n", getArenaSize(OS_ARENA_MAINEX));
    }

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

/*====== End of main.c ======*/

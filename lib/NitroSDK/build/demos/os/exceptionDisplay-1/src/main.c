/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - exceptionDisplay-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.20  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.19  2005/08/10 05:01:19  yada
  remove OS_InitThread()

  Revision 1.18  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.17  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.16  2004/10/04 00:34:45  yasu
  Add INDENT-OFF/ON to control code beautifier

  Revision 1.15  2004/09/28 12:19:54  yada
  not call OS_InitException() because OS_Init() called

  Revision 1.14  2004/06/08 00:18:00  yada
  add '#pragma unused()' for FINALROM

  Revision 1.13  2004/06/03 07:21:33  yada
  sorry, mistake update. back to 1.11. ignore update 1.12.

  Revision 1.12  2004/06/03 06:59:41  yada
  fix indent processing

  Revision 1.11  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.10  2004/03/30 08:38:22  yada
  小修正

  Revision 1.9  2004/03/17 08:18:38  yada
  #if → #ifdef

  Revision 1.8  2004/03/11 23:48:42  yada
  擬似命令実験コード抜いた

  Revision 1.7  2004/03/11 11:26:56  yada
  CWのthumbコンパイルバグ回避

  Revision 1.6  2004/03/09 04:13:14  yada
  cache考慮

  Revision 1.5  2004/03/09 01:20:22  yada
  fix top comment


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    OS_InitException(void);
void    myExceptionCallback(u32 context, void *arg);

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
#include <nitro/code32.h>
void NitroMain()
{
    OS_Init();

    //OS_InitException();   /*called in OS_Init()*/

    OS_SetUserExceptionHandler(myExceptionCallback, (void *)0);


    //---- ここでキャッシュをフラッシュするのは、
    //     例外が発生するとプロテクションユニットが変更されるので
    //     例外に関するアドレス等は確実に書き込んでおきたいから。
    DC_FlushAll();
    DC_WaitWriteBufferEmpty();


    //---- not ocurred exception div0
    {
        int     a = 1;
        int     b = 0;
        volatile int c = a / b;
    }

    //---- force to occur exception
    OS_Printf("now force to occur exception...\n");
    asm
    {
        /* *INDENT-OFF* */
        ldr      r0, =123
        ldr      r1, =0
        ldr      r2, =256
        ldr      r3, =1000
        ldr      r4, =2000
        ldr      r5, =3000
        ldr      r6, =4000
        ldr      r7, =5000
        mov      r8, pc
        ldr      r9, =0xff
        ldr      r10, =0xee
        ldr      r12, =0x123

#ifdef SDK_ARM9
        ldr      r11, [r1,#0]
#else  //SDK_ARM9
        dcd      0x06000010

#endif //SDK_ARM9
        
        /* *INDENT-ON* */
    }
    OS_Printf("not occurred exception.\n");

    OS_Terminate();
}

/*---------------------------------------------------------------------------*
  Name:         myExceptionCallback

  Description:  user callback for exception

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void myExceptionCallback(u32 context, void *arg)
{
#ifdef SDK_FINALROM
#pragma unused( context, arg )
#endif

    OS_Printf("---------------- USER EXCEPTION CALLBACK\n");
    OS_Printf("context=%x  arg=%x\n", context, arg);

    OS_Printf("---------------- Thread LIST\n");
    OS_DumpThreadList();
}

#include <nitro/codereset.h>

/*====== End of main.c ======*/

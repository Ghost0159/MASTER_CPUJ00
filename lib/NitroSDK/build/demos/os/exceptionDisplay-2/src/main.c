/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - exceptionDisplay-2
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.11  2006/04/21 08:19:34  kitase_hirotake
  add SDK_FORCE_EXPORT

  Revision 1.10  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.9  2005/08/10 05:01:19  yada
  remove OS_InitThread()

  Revision 1.8  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.7  2005/03/04 11:09:27  yasu
  __declspec(weak) を SDK_WEAK_SYMBOL に置き換え

  Revision 1.6  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.5  2004/09/28 12:19:54  yada
  not call OS_InitException() because OS_Init() called

  Revision 1.4  2004/06/08 00:18:21  yada
  add '#pragma unused()' for FINALROM

  Revision 1.3  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.2  2004/03/17 09:30:13  yada
  多重定義warning対策と、code32

  Revision 1.1  2004/03/09 05:54:31  yada
  firstRelease

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    OS_InitException(void);
void    myExceptionCallback(u32 context, void *arg);


#include <nitro/code32.h>
//---------------- 整数除算
//                 除数が0の場合、0アドレスにアクセスする
extern  _u32_div_f(void);

SDK_FORCE_EXPORT static asm s32 _s32_div_f(void)
{
       stmfd       sp!,{r4,lr}

       //---- compare divisor to 0
       cmp         r1,#0
       bne         _1

       //---- force to occur exception
       ldr         r11, [r1,#0]
_1:
       eor         r4,r0,r1
       and         r4,r4,#-2147483648
       cmp         r0,#0
       rsblt       r0,r0,#0
       addlt       r4,r4,#1
       cmp         r1,#0
       rsblt       r1,r1,#0
       bl          _u32_div_f
       ands        r3,r4,#-2147483648
       rsbne       r0,r0,#0
       ands        r3,r4,#1
       rsbne       r1,r1,#0
       ldmfd       sp!,{r4,lr}
       bx          lr
}
#include <nitro/codereset.h>

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
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


    //---- normal division
    {
        int     a = 120;
        volatile int b = 6;
        volatile int c = a / b;
        OS_Printf("a=%d b=%d c=%d\n", a, b, c);
    }


    //---- divisor is 0
    {
        int     a = 240;
        volatile b = 0;
        volatile int c = a / b;
        OS_Printf("a=%d b=%d c=%d\n", a, b, c);
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

/*====== End of main.c ======*/

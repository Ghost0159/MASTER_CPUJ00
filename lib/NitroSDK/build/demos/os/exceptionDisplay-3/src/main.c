/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - exceptionDisplay-3
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.16  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.15  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.14  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.13  2004/10/04 00:34:45  yasu
  Add INDENT-OFF/ON to control code beautifier

  Revision 1.12  2004/09/28 12:19:54  yada
  not call OS_InitException() because OS_Init() called

  Revision 1.11  2004/06/08 00:18:56  yada
  add '#pragma unused()' for FINALROM

  Revision 1.10  2004/06/03 07:29:45  yada
  sorry, update 1.9 is mistake. ignore comment of 1.9.
  now, remove some ifdef

  Revision 1.9  2004/06/03 06:59:41  yada
  fix indent processing

  Revision 1.8  2004/05/24 04:47:08  yada
  fix profile pragma

  Revision 1.7  2004/05/20 04:32:47  yada
  add pragma profile off to NitroMain()

  Revision 1.6  2004/05/12 05:23:33  yada
  add __PROFILE_ENTRY/EXIT definition as weak symbol

  Revision 1.5  2004/05/10 12:03:28  yada
  consider for no CALLTRACE case

  Revision 1.4  2004/04/23 06:02:30  yada
  small fix

  Revision 1.3  2004/04/16 05:38:34  yada
  consideration for THUMB

  Revision 1.2  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.1  2004/04/14 07:08:01  yada
  first release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

int     test1(int a);
int     test2(int a);
int     test3(int a);

void    myExceptionCallback(u32 context, void *arg);


#define MYTRACEBUFFER_SIZE 0x1000
u32     myTraceBuffer[MYTRACEBUFFER_SIZE / sizeof(u32)];

//======================================================================
//----------------------------------------------------------------
// test1
//
int test1(int a)
{
    return test2(a + 1);
}

//----------------------------------------------------------------
// test2
//
int test2(int a)
{
    return test3(a + 2);
}

//----------------------------------------------------------------
// test3
//
int test3(int a)
{
    //---- make exception occur
    asm
    {
        /* *INDENT-OFF* */
        ldr      r0, =0
#ifdef SDK_ARM9
        ldr      r2, [r0,#0]
#else  //SDK_ARM9
        dcd      0x06000010

#endif //SDK_ARM9
        /* *INDENT-ON* */
    }

    return a + 4;
}

//======================================================================
//  NitroMain
//
#pragma profile off
#include <nitro/code32.h>
void NitroMain()
{
    OS_Init();
    //OS_InitException();   /*called in OS_Init()*/

    OS_SetUserExceptionHandler(myExceptionCallback, (void *)0);
    DC_FlushAll();
    DC_WaitWriteBufferEmpty();


    //---- setup of function call trace
    OS_InitCallTrace(myTraceBuffer, MYTRACEBUFFER_SIZE, OS_CALLTRACE_LOG /*STACK*/);

    (void)test1(100);

    OS_Terminate();
}

//----------------------------------------------------------------
//   user callback for exception
//
void myExceptionCallback(u32 context, void *arg)
{
#ifdef SDK_FINALROM
#pragma unused( context, arg )
#endif

    OS_Printf("---------------- USER EXCEPTION CALLBACK\n");
    OS_Printf("context=%x  arg=%x\n", context, arg);

    OS_DumpCallTrace();
}

#include <nitro/codereset.h>

/*====== End of main.c ======*/

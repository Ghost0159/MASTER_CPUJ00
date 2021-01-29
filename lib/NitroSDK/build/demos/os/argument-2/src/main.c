/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - argument-2
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.2  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.1  2005/09/09 05:19:51  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

//================================================================================
#define ARG_BUFFER_SIZE   0x100
char    argBuffer[ARG_BUFFER_SIZE];

const char str[] = "dummy -a 123 \"my original string\" 456";

static void displayArg();

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    const char *originalBuffer;

    OS_Init();

    //---- save original argument buffer
    originalBuffer = OS_GetArgumentBuffer();

    //================ my arguments
    //---- convert to my argument buffer
    OS_ConvertToArguments(str, ' ', argBuffer, ARG_BUFFER_SIZE);

    //---- set my argument buffer
    OS_SetArgumentBuffer(argBuffer);

    //---- display
    displayArg();

    //================ original arguments
    //---- recover to original buffer
    OS_SetArgumentBuffer(originalBuffer);

    //---- display
    displayArg();


    OS_Printf("==== Finish sample.\n");
    OS_Exit(0);
}

/*---------------------------------------------------------------------------*
  Name:         displayArg

  Description:  display arguments

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
static void displayArg(void)
{
    int     argc;
    int     n;

    //---- display argc
    argc = OS_GetArgc();

    OS_Printf("argc = %d\n", argc);

    //---- display argv
    for (n = 0; n < argc; n++)
    {
        const char *argv = OS_GetArgv(n);
        OS_Printf("[%d] address=%x string=[%s]\n", n, argv, argv);
    }
    OS_Printf("\n");
}

/*====== End of main.c ======*/

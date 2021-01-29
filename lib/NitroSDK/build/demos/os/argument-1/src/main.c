/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - argument-1
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.6  2005/09/06 10:15:57  yasu
  '-' モードと '+' モードの実装の修正

  Revision 1.5  2005/09/06 09:17:33  yasu
  '-' モードと '+' モードの実装

  Revision 1.4  2005/08/30 10:38:39  yasu
  オプションでない通常の引数もオプションと透過的に扱えるようにした

  Revision 1.3  2005/08/30 08:59:47  yasu
  OS_GetOpt 追加

  Revision 1.2  2005/07/21 12:02:03  yasu
  indent/TAB 幅変更

  Revision 1.1  2005/07/21 02:32:30  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    int     argc;
    int     n, c;
    BOOL    opt_b = FALSE;
    const char *opt_s = NULL;
    const char *opt_t = NULL;

    OS_Init();

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

    //---- get option
    while ((c = OS_GetOpt("bs:t::")) > 0)
    {
        switch (c)
        {
        case 'b':
            opt_b = TRUE;
            break;

        case 's':
            opt_s = OS_GetOptArg();
            break;

        case 't':
            opt_t = OS_GetOptArg();
            break;

        case '?':
        default:
            OS_Printf("Error --- option '%c'\n", OS_GetOptOpt());
            break;
        }
    }

    OS_Printf("OPTION-b: %s\n", opt_b ? "TRUE" : "FALSE");
    OS_Printf("OPTION-s: %s\n", opt_s ? opt_s : "[NULL]");
    OS_Printf("OPTION-t: %s\n", opt_t ? opt_t : "[NULL]");

    for (n = OS_GetOptInd(); n < argc; n++)
    {
        OS_Printf("ARGV[%d] : %s\n", n, OS_GetArgv(n));
    }
    OS_Printf("\n");

    //---- Do it once
    (void)OS_GetOpt(NULL);             // Rewind

    //---- get option
    while ((c = OS_GetOpt("-bs:t::")) > 0)
    {
        switch (c)
        {
        case 1:
            OS_Printf("OPTION-b: %s\n", opt_b ? "TRUE" : "FALSE");
            OS_Printf("OPTION-s: %s\n", opt_s ? opt_s : "[NULL]");
            OS_Printf("OPTION-t: %s\n", opt_t ? opt_t : "[NULL]");
            OS_Printf("ARG     : %s\n", OS_GetOptArg());
            break;

        case 'b':
            opt_b = TRUE;
            break;

        case 's':
            opt_s = OS_GetOptArg();
            break;

        case 't':
            opt_t = OS_GetOptArg();
            break;

        case '?':
        default:
            OS_Printf("Error --- option '%c'\n", OS_GetOptOpt());
            break;
        }
    }

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

/*====== End of main.c ======*/

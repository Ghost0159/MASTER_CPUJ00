/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FX - demos - test
  File:     fx_test_common.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
  
  $Log: fx_test_common.c,v $
  Revision 1.6  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.5  2004/06/08 00:15:16  yada
  add '#pragma unused()' for FINALROM

  Revision 1.4  2004/04/07 01:27:57  yada
  fix header comment

  Revision 1.3  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.2  2004/01/29 01:35:09  kitani_toshikazu
  Fix some bugs for debug build, restore appended test cases.
 
  $NoKeywords: $
  *---------------------------------------------------------------------------*/

#ifndef  FX_TEST_COMMON_H_
#include "fx_test_common.h"
#endif //  FX_TEST_COMMON_H_


//------------------------------------------------------------------------------
// 
void MUST_SUCCEED_ASSERT(BOOL exp, int idx)
{
#ifdef SDK_FINALROM
#pragma unused( idx )
#endif

    if (!exp)
    {
        OS_Printf("****** Test Failure *********************\n");
        OS_Printf("*  Failure in a MUST SUCCEED TEST\n");
        OS_Printf("*  Check the test code for No.%d \n", idx);
        OS_Printf("****** Test Failure *********************\n");
    }
}

//------------------------------------------------------------------------------
// 
void MUST_FAIL_ASSERT(BOOL exp, int idx)
{
#ifdef SDK_FINALROM
#pragma unused( idx )
#endif

    if (exp)
    {
        OS_Printf("****** Test Failure *********************\n");
        OS_Printf("*  Failure in a MUST FAIL TEST\n");
        OS_Printf("*  Check the test code for No.%d \n", idx);
        OS_Printf("****** Test Failure *********************\n");
    }
}

//------------------------------------------------------------------------------
// 
void print_mtx43(const MtxFx43 *m)
{
    OutDetail("_\n");
    OutDetail("%f %f %f\n", m->_00 / 4096.0, m->_01 / 4096.0, m->_02 / 4096.0);
    OutDetail("%f %f %f\n", m->_10 / 4096.0, m->_11 / 4096.0, m->_12 / 4096.0);
    OutDetail("%f %f %f\n", m->_20 / 4096.0, m->_21 / 4096.0, m->_22 / 4096.0);
    OutDetail("%f %f %f\n", m->_30 / 4096.0, m->_31 / 4096.0, m->_32 / 4096.0);
    OutDetail("_\n");
}

//------------------------------------------------------------------------------
// Ú×‚ğ•\¦‚·‚é
void OutDetail(const char *fmt, ...)
{
#ifdef  FX_TEST__SHOW_DETAIL
    va_list vlist;

    va_start(vlist, fmt);
    OS_Printf(fmt, vlist);
    va_end(vlist);
#else
    (void)fmt;
#endif
}

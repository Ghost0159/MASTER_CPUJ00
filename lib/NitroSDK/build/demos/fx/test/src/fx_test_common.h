/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FX - demos - test
  File:     fx_test_common.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
  
  $Log: fx_test_common.h,v $
  Revision 1.6  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.5  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.4  2004/04/07 01:27:57  yada
  fix header comment

  Revision 1.3  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.2  2004/01/29 01:35:09  kitani_toshikazu
  Fix some bugs for debug build, restore appended test cases.
 
  $NoKeywords: $
  
  *---------------------------------------------------------------------------*/

#ifndef FX_TEST_COMMON_H_
#define FX_TEST_COMMON_H_

#include <nitro.h>
#include <math.h>

// テストの詳細な内容を出力します
// #define FX_TEST__SHOW_DETAIL 
// エラーとみなされる誤差の閾値
#define CRITICAL_ERROR_THRESHOLD    (double)( 1.0 / (double)( ( 1 << 3 ) ) )



extern void print_mtx43(const MtxFx43 *m);
// 詳細を報告する
extern void OutDetail(const char *fmt, ...);
extern void MUST_SUCCEED_ASSERT(BOOL exp, int idx);
extern void MUST_FAIL_ASSERT(BOOL exp, int idx);


// double ベクトル
typedef struct
{
    double  x;
    double  y;
    double  z;
}
VecD;

#endif // FX_TEST_COMMON_H_

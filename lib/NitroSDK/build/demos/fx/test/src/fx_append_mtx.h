/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FX - demos - test
  File:     fx_append_mtx.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
  
  $Log: fx_append_mtx.h,v $
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
#ifndef  FX_APPEND_MTX_H_
#define  FX_APPEND_MTX_H_

#include <nitro.h>
#include <math.h>

#ifndef  FX_TEST_COMMON_H_
#include "fx_test_common.h"
#endif //  FX_TEST_COMMON_H_

// Memo : 
// MTX_RotAxis33 など Fx32Vec を引数にとる関数群で、引数が正規ベクトルである事を要求する関数がある。
// 現在は vAxis が 正規 ベクタであることをチェックしない。
// コンパイルオプションによって正規ベクタであることをチェックするようになれば
// "正規 ベクタ チェックが正常に作動するか"をテスト項目に追加する 必要がある

//----------------------------------------------------------------------------
//  grobal function 
//----------------------------------------------------------------------------

// 追加テスト項目のエントリ
extern void MTXTest_appended();




#endif //  FX_APPEND_MTX_H_

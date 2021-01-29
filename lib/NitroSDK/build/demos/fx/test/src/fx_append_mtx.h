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
// MTX_RotAxis33 �Ȃ� Fx32Vec �������ɂƂ�֐��Q�ŁA���������K�x�N�g���ł��鎖��v������֐�������B
// ���݂� vAxis �� ���K �x�N�^�ł��邱�Ƃ��`�F�b�N���Ȃ��B
// �R���p�C���I�v�V�����ɂ���Đ��K�x�N�^�ł��邱�Ƃ��`�F�b�N����悤�ɂȂ��
// "���K �x�N�^ �`�F�b�N������ɍ쓮���邩"���e�X�g���ڂɒǉ����� �K�v������

//----------------------------------------------------------------------------
//  grobal function 
//----------------------------------------------------------------------------

// �ǉ��e�X�g���ڂ̃G���g��
extern void MTXTest_appended();




#endif //  FX_APPEND_MTX_H_

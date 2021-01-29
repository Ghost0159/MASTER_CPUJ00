/*---------------------------------------------------------------------------*
  Project:  NitroDWC - auth - util_alloc
  File:     util_alloc.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: util_alloc.h,v $
  Revision 1.15  2006/04/13 11:35:19  hayashi
  Change prefix Ranking->Rnk

  Revision 1.14  2006/03/02 05:21:58  nakata
  DWC_Svc���W���[���ǉ�
  DWC_Util_Alloc/DWC_Util_Free�֐��̖��O��DWC_Auth_Alloc/DWC_Auth_Free�֐��ɖ��O��ύX

  Revision 1.13  2005/12/22 01:39:06  nakata
  Ressurected util_alloc.h.

  Revision 1.8  2005/11/15 10:55:40  nakata
  Ressurect util_alloc.h for successful compilation.

  Revision 1.6  2005/09/01 00:19:46  nakata
  Removed util_alloc.c related codes from DWC_Http/DWC_Auth/DWC_Netcheck.

  Revision 1.5  2005/08/20 07:01:19  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_UTIL_ALLOC_H_
#define DWC_UTIL_ALLOC_H_

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif // [nakata] __cplusplus

//=============================================================================
// [nakata] �\����
//=============================================================================

// [nakata] �������m��/����֐��̂��߂̃C���^�t�F�[�X�^
typedef void *(*DWCAuthAlloc)(unsigned long name, long size);
typedef void (*DWCAuthFree)(unsigned long name, void *ptr, long size);

#ifdef __cplusplus
}
#endif // [nakata] __cplusplus

#endif // DWC_UTIL_ALLOC_H_

/*---------------------------------------------------------------------------*
  Project:  NitroDWC - auth - util_base64
  File:     util_base64.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: util_base64.h,v $
  Revision 1.6  2006/03/03 04:49:40  nakata
  NAS���Ԏ擾�@�\��ǉ������B

  Revision 1.5  2006/03/02 05:21:58  nakata
  DWC_Svc���W���[���ǉ�
  DWC_Util_Alloc/DWC_Util_Free�֐��̖��O��DWC_Auth_Alloc/DWC_Auth_Free�֐��ɖ��O��ύX

  Revision 1.4  2006/03/01 02:44:08  nakata
  CAL�擾�@�\��dwc_auth.c���番������

  Revision 1.3  2005/08/20 07:01:19  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_UTIL_BASE64_H_
#define DWC_UTIL_BASE64_H_
/*****************************************************************************/
/*                                                                           */
/*  BASE64�G���R�[�h/�f�R�[�h                                                */
/*      util_base64.h                                                        */
/*          ver. 0.5                                                         */
/*                                                                           */
/*****************************************************************************/

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif // [nakata] __cplusplus

//=============================================================================
// [nakata] �֐�
//=============================================================================

//-----------------------------------------------------------------------------
// �@�\ : �^����ꂽ�f�[�^���J�X�^�}�C�Y���ꂽBase64�ŃG���R�[�h
// ���� : src		- ���f�[�^�ւ̃|�C���^ 
//      : srclen	- ���f�[�^�̒���
//      : dst		- �ϊ���o�b�t�@�ւ̃|�C���^
//      : dstlen	- �ϊ���o�b�t�@�̒���
// �Ԓl : int  - -1       : ���s
//               ����ȊO : �ϊ����ʂ̒���
//-----------------------------------------------------------------------------
int DWC_Auth_Base64Encode(const char *src, const unsigned long srclen, char *dst, const unsigned long dstlen);

//-----------------------------------------------------------------------------
// �@�\ : �^����ꂽ�f�[�^���J�X�^�}�C�Y���ꂽBase64�Ńf�R�[�h
// ���� : src		- Base64�f�[�^�ւ̃|�C���^ 
//      : srclen	- Base64�f�[�^�̒���
//      : dst		- �ϊ���o�b�t�@�ւ̃|�C���^
//      : dstlen	- �ϊ���o�b�t�@�̒���
// �Ԓl : int  - -1       : ���s
//               ����ȊO : �ϊ����ʂ̒���
//-----------------------------------------------------------------------------
int DWC_Auth_Base64Decode(const char *src, const unsigned long srclen, char *dst, const unsigned long dstlen);

#ifdef __cplusplus
}
#endif // [nakata] __cplusplus


#endif // DWC_UTIL_BASE64_H_

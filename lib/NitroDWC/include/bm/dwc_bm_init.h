/*---------------------------------------------------------------------------*
  Project:  NitroDWC - bm - dwc_bm_init
  File:     dwc_bm_init.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_bm_init.h,v $
  Revision 1.2  2005/08/20 09:39:28  akimaru
  #define DWC_INIT_WORK_SIZE 0x700

  Revision 1.1  2005/08/20 07:01:20  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_BM_INIT_H_
#define DWC_BM_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// ��`
//=============================================================================
// �������֐����[�N�������T�C�Y
#define DWC_BM_INIT_WORK_SIZE 0x700
#define DWC_INIT_WORK_SIZE 0x700

//=============================================================================
// �֐�
//=============================================================================
//-----------------------------------------------------------------------------
// �@�\ : �������֐�
//        �� ���[�N�������� DWC_INIT_WORK_SIZE �ȏ�ł��A
//           32 �o�C�g�A���C������Ă���K�v������܂�
//        �� �֐��I����Ƀ��[�N�������͔j�����Ă��܂��܂���
// ���� : work  - ���[�N������
// �Ԓl : int   -      0 : ����I��
//                -10002 : �ڑ���ݒ肪��������������I��
//                -10003 : ���[�U ID ����������������I��
//                -10000 : DS �o�b�N�A�b�v�������̏������݃G���[�I��
//                -10001 : DS �o�b�N�A�b�v�������̓ǂݍ��݃G���[�I��
//-----------------------------------------------------------------------------
int  DWC_BM_Init(void* work);

#ifdef __cplusplus
}
#endif

#endif // DWC_BM_INIT_H_

/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - dwc_common
  File:     dwc_common.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_common.h,v $
  Revision 1.11  2005/12/26 06:43:20  nakata
  Moved ingamesn check functions from dwc_common.c to dwc_login.c.

  Revision 1.10  2005/12/26 04:23:18  nakata
  Added NAS return code 40 related functions to Auth.
  Added DWC_GetIngamesnCheckResult() to dwc_common.c/dwc_common.h.
  Fixed the bug that incorrect ingamesn is sent when auth is called in netcheck.

  Revision 1.9  2005/12/21 03:08:19  arakit
  ���C�h������̕����񒷂�Ԃ��֐�DWCi_WStrLen()���쐬�����B

  Revision 1.8  2005/10/28 09:11:20  arakit
  DWCi_GetCurrentTime()���폜�����B

  Revision 1.7  2005/09/16 07:27:13  arakit
  �ėpkey/value������̍ő咷��4096�ɕύX

  Revision 1.6  2005/09/05 11:42:02  arakit
  key/value�����񑀍�֐��̕����������P�p�����B

  Revision 1.5  2005/08/29 12:25:31  arakit
  ���Ԍv���֐�DWCi_GetCurrentTime()��ǉ������B

  Revision 1.4  2005/08/29 06:34:15  arakit
  �ėpkey/value�^������ǂݏ����֐���p�ӂ����B

  Revision 1.3  2005/08/23 13:53:05  arakit
  �����_���֐���MATH_Rand*���g���悤�ɂ����B

  Revision 1.2  2005/08/20 07:01:19  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_COMMON_H_
#define DWC_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif
    

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
// �ėpkey/value������̂��ꂼ��̍ő啶����
#define DWC_COMMONSTR_MAX_KEY_VALUE_LEN 4096
    

//----------------------------------------------------------------------------
// enum
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// typedef - function
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �ėpkey/value�^������쐬�֐�
  �����@�Fkey       �ݒ肵����key������
          value     �ݒ肵����value������
          string    key/value�^������i�[��|�C���^
          separator �e������𕪂����؂蕶��
  �߂�l�F�Z�b�g���ꂽkey/value�����񒷁iNULL�I�[�܂܂��j
  �p�r�@�Fkey/value�̕�������w�肵�ADWC�ėpkey/value��������쐬����
 *---------------------------------------------------------------------------*/
extern int  DWC_SetCommonKeyValueString(const char* key, const char* value, char* string, char separator);


/*---------------------------------------------------------------------------*
  �ėpkey/value�^������ǉ��֐�
  �����@�Fkey       �ݒ肵����key������
          value     �ݒ肵����value������
          string    key/value�^������i�[��|�C���^
          separator �e������𕪂����؂蕶��
  �߂�l�Fkey/value�����񂪒ǉ����ꂽ���key/value�����񒷁iNULL�I�[�܂܂��j
  �p�r�@�Fkey/value�̕�����������̕�����ɒǉ�����
 *---------------------------------------------------------------------------*/
extern int  DWC_AddCommonKeyValueString(const char* key, const char* value, char* string, char separator);


/*---------------------------------------------------------------------------*
  �ėpkey/value�^������value�擾�֐�
  �����@�Fkey       ���o������key������
          value     ���o����value������̊i�[��|�C���^�B
          string    key/value�^������
          separator �e������𕪂����؂蕶��
  �߂�l�Fvalue�����񒷁iNULL�I�[�܂܂��j�B���݂��Ȃ�key���w�肵���ꍇ��-1��Ԃ�
  �p�r�@�F�w�肵����؂蕶���ŋ�؂�ꂽDWC�ėpkey/value�^�����񂩂�A
          �w�肳�ꂽkey������ɑΉ�����value��������擾����B
 *---------------------------------------------------------------------------*/
extern int  DWC_GetCommonValueString(const char* key, char* value, const char* string, char separator);


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
extern u32  DWCi_GetMathRand32(u32 max);


extern u32  DWCi_WStrLen( const u16* str );


#ifdef __cplusplus
}
#endif


#endif // DWC_COMMON_H_

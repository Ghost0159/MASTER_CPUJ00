/*---------------------------------------------------------------------------*
  Project:  NitroDWC - auth - dwc_prof
  File:     dwc_prof.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_prof.h,v $
  Revision 1.11  2007/06/29 09:13:06  takayama
  ���[�W�����w��ł̕s��������`�F�b�N�J�n�֐���ǉ��B

  Revision 1.10  2007/04/20 04:20:53  takayama
  �R�����g�̌�L�C���B

  Revision 1.9  2007/04/09 02:45:59  takayama
  DWC_CheckProfanityAsync�֐��̈��� badnum �� badwordsnum �ɕύX�������Ƃ�
  �����ύX�B

  Revision 1.8  2007/04/04 07:38:05  takayama
  �g�p���Ă��Ȃ���`���폜�B

  Revision 1.7  2007/03/28 09:41:26  nakata
  DWC_CheckProfanityAsync�֐��̎d�l�ύX�ɔ����C���B

  Revision 1.6  2007/03/19 11:24:36  nakata
  DWCProfResult��DWCProfState�ɕύX����

  Revision 1.5  2007/03/16 05:35:47  nakata
  DWC_PROF_RESULT�萔����̕ύX

  Revision 1.4  2007/03/14 08:44:07  nakata
  DWC_PROF_RESULT_STRING_LENGTH�萔��ǉ�

  Revision 1.3  2006/11/22 12:55:27  nakata
  DWCProfResult�Ƀl�b�g���[�N�G���[��ǉ�

  Revision 1.2  2006/11/17 08:29:47  nakata
  �R�����g�C��

  Revision 1.1  2006/11/17 08:29:06  nakata
  �s��������`�F�b�N�֌W�w�b�_��ǉ�


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_PROF_H
#define DWC_PROF_H

#include "dwc_http.h"

#ifdef __cplusplus
extern "C" {
#endif // [nakata] __cplusplus

//=============================================================================
// [nakata] ��`
//=============================================================================

#define DWC_PROF_RESULT_STRING_LENGTH 51

// [nakata] �s���l�[���`�F�b�N���̃X�e�[�g�������񋓌^
typedef enum {
	DWC_PROF_STATE_NOT_INITIALIZED	= 0,// ��������
	DWC_PROF_STATE_OPERATING,				// HTTP�ʐM��
	DWC_PROF_STATE_SUCCESS,				// ���O�`�F�b�N����
	DWC_PROF_STATE_FAIL					// ���O�`�F�b�N���s
} DWCProfState;

// �s��������`�F�b�N�̃��[�W�����������񋓌^
typedef enum {
    DWC_PROF_REGION_SELF = 0x00,   // ���i�R�[�h�ɏ]��
    DWC_PROF_REGION_ALL  = 0xff    // �S���[�W����
} DWCProfRegion;

//=============================================================================
// [nakata] �֐�
//=============================================================================

//-----------------------------------------------------------------------------
// �@�\ : �s��������`�F�b�N�J�n�֐�
// ���� : words - �`�F�b�N���镶����(UTF16-LE)�̃|�C���^�z��
//      : wordsnum - words�̒���
//      : reserved - �Q�[�����o�b�h�l�[�������؂�ւ��p��4������ASCII������(�ʏ��NULL)
//      : timeout - �^�C���A�E�g���Ԃ�b�P�ʂŎw�肷��
//      : result - �`�F�b�N����
//      : badwordsnum - �s��������̐�
// �Ԓl : TRUE  : �擾�J�n
//        FALSE : �擾�J�n�ł��Ȃ�
//-----------------------------------------------------------------------------
BOOL DWC_CheckProfanityAsync(const u16 **words, int wordsnum, const char *reserved, int timeout, char *result, int *badwordsnum);

//-----------------------------------------------------------------------------
// �@�\ : �s��������`�F�b�N�J�n�֐�(���[�W�����w���)
// ���� : words - �`�F�b�N���镶����(UTF16-LE)�̃|�C���^�z��
//      : wordsnum - words�̒���
//      : reserved - �Q�[�����o�b�h�l�[�������؂�ւ��p��4������ASCII������(�ʏ��NULL)
//      : timeout - �^�C���A�E�g���Ԃ�b�P�ʂŎw�肷��
//      : result - �`�F�b�N����
//      : badwordsnum - �s��������̐�
//      : region - �Ώۂ̃��[�W����
// �Ԓl : TRUE  : �擾�J�n
//        FALSE : �擾�J�n�ł��Ȃ�
//-----------------------------------------------------------------------------
BOOL DWC_CheckProfanityExAsync(const u16 **words, int wordsnum, const char *reserved, int timeout, char *result, int *badwordsnum, DWCProfRegion region);

//-----------------------------------------------------------------------------
// �@�\ : �s��������`�F�b�N�擾�����i�s�֐�
// ���� : �Ȃ�
// �Ԓl : DWCProfState
//-----------------------------------------------------------------------------
DWCProfState DWC_CheckProfanityProcess(void);

#ifdef __cplusplus
}
#endif

#endif

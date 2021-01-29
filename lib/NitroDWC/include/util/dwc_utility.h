/*---------------------------------------------------------------------------*
  Project:  NitroDWC - util - dwc_utility
  File:     dwc_utility.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_utility.h,v $
  Revision 1.13  2007/03/28 05:17:18  takayama
  �d���n�k�Ẳp�ꃁ�b�Z�[�W����T�|�[�g�p�d�b�ԍ����폜�B

  Revision 1.12  2006/12/29 04:27:39  takayama
  EUC -> SJIS

  Revision 1.11  2006/12/18 02:17:42  takayama
  ��`���C��(KORIA -> HANGUL)�B

  Revision 1.10  2006/12/06 07:57:58  kokin
  �؍���Ή�

  Revision 1.9  2005/09/13 13:12:56  tsukada
  ���[�N�������T�C�Y���k��

  Revision 1.8  2005/09/10 14:07:44  tsukada
  �p�����[�^�d�l��ύX

  Revision 1.7  2005/09/01 13:22:17  tsukada
  �V�K

  Revision 1.6  2005/08/20 07:01:21  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_UTILITY_H_
#define DWC_UTILITY_H_
/*****************************************************************************/
/*                                                                           */
/*  DWC ���[�e�B���e�B �w�b�_�t�@�C��                                        */
/*      dwc_utility.h                                                        */
/*          ver. 1.2                                                         */
/*                                                                           */
/*****************************************************************************/

#ifdef  __cplusplus
extern  "C" {
#endif         // __cplusplus

//=============================================================================
// ��`
//=============================================================================
// ���[�N�������̃T�C�Y
#define  DWC_UTILITY_WORK_SIZE    0x40000

// �g�p����
enum  DWC_LANGUAGE {
	DWC_LANGUAGE_JAPANESE,             // ���{��
	DWC_LANGUAGE_ENGLISH,              // �p��
	DWC_LANGUAGE_FRENCH,               // �t�����X��
	DWC_LANGUAGE_GERMAN,               // �h�C�c��
	DWC_LANGUAGE_ITALIAN,              // �C�^���A��
	DWC_LANGUAGE_SPANISH,              // �X�y�C����
	DWC_LANGUAGE_HANGUL                // �؍���
};

// �ݒ胆�[�e�B���e�B�J�n�֐��Ԓl
enum  DWCUTILRESULT {
	DWC_UTIL_RESULT_SUCCESS =  0,      // ����
	DWC_UTIL_RESULT_FAILED  = ~0       // �����G���[
};

// �t���O
enum  DWCUTILFLAG0 {
	DWC_UTIL_FLAG0_TOP      = 0x0000,
	DWC_UTIL_FLAG0_SETTING  = 0x0001
};
enum  DWCUTILFLAG1 {
	DWC_UTIL_FLAG1_RAKU     = 0x0001,   // �炭�炭�����X�^�[�g�Ή�
	DWC_UTIL_FLAG1_USA      = 0x0002    // �p��̃T�|�[�g�p�d�b�ԍ��Ή�
};

//-----------------------------------------------------------------------------
// �p�����[�^
//-----------------------------------------------------------------------------
//-------- �d���n�F�k�� --------
// �g�b�v���j���[����J�n
#define  DWC_UTILITY_TOP_MENU_FOR_USA                                         \
	(DWC_UTIL_FLAG0_TOP)

// Wi-Fi �ڑ���I������J�n
#define  DWC_UTILITY_SETTING_FOR_USA                                          \
	(DWC_UTIL_FLAG0_SETTING)

//-------- �d���n�F���B --------
// �g�b�v���j���[����J�n
#define  DWC_UTILITY_TOP_MENU_FOR_EUR                                         \
	(DWC_UTIL_FLAG0_TOP)

// Wi-Fi �ڑ���I������J�n
#define  DWC_UTILITY_SETTING_FOR_EUR                                          \
	(DWC_UTIL_FLAG0_SETTING)

//-------- �d���n�F���{ --------
// �g�b�v���j���[����J�n
#define  DWC_UTILITY_TOP_MENU_FOR_JPN                                         \
	(DWC_UTIL_FLAG0_TOP | (DWC_UTIL_FLAG1_RAKU << 4))

// Wi-Fi �ڑ���I������J�n
#define  DWC_UTILITY_SETTING_FOR_JPN                                          \
	(DWC_UTIL_FLAG0_SETTING | (DWC_UTIL_FLAG1_RAKU << 4))

//-------- �d���n�F���� --------
// (�� ���{��͕s��)
// �g�b�v���j���[����J�n
#define  DWC_UTILITY_TOP_MENU_COMMON                                          \
	(DWC_UTIL_FLAG0_TOP)

// Wi-Fi �ڑ���I������J�n
#define  DWC_UTILITY_SETTING_COMMON                                           \
	(DWC_UTIL_FLAG0_SETTING)

#define  DWC_UTILITY_TOP_MENU_EXCEPT_JPN                                      \
	DWC_UTILITY_TOP_MENU_COMMON
#define  DWC_UTILITY_SETTING_EXCEPT_JPN                                       \
	DWC_UTILITY_SETTING_COMMON

//=============================================================================
// �֐�
//=============================================================================
//-----------------------------------------------------------------------------
// �@�\ : �ݒ胆�[�e�B���e�B�J�n
// ���� : work      - ���[�N�������̐擪�A�h���X
//                    �� DWC_UTILITY_WORK_SIZE �o�C�g�ȏ�ł��A
//                       32 �o�C�g�A���C������Ă���K�v������܂�
//                    �� �֐��I����͔j�����Ă��܂��܂���
//        language  - �A�v���P�[�V�����Ŏg�p���Ă��錾��
//        param     - �ȉ�����I�����Ă�������
//                        DWC_UTILITY_TOP_MENU_FOR_USA
//                        DWC_UTILITY_SETTING_FOR_USA
//                        DWC_UTILITY_TOP_MENU_FOR_EUR
//                        DWC_UTILITY_SETTING_FOR_EUR
//                        DWC_UTILITY_TOP_MENU_FOR_JPN
//                        DWC_UTILITY_SETTING_FOR_JPN
//                        DWC_UTILITY_TOP_MENU_COMMON
//                        DWC_UTILITY_SETTING_COMMON
// �Ԓl : int       - 0        : ����I��
//                    ����ȊO : �����G���[
//-----------------------------------------------------------------------------
int  DWC_StartUtility(void* work, int language, int param);

/*----------------------------------------------------------------------------/
�y�ݒ胆�[�e�B���e�B�̒��ӎ����z

�� ROM �C���[�W
  �ݒ胆�[�e�B���e�B�Ŏg�p����f�[�^�� ROM �C���[�W�Ƃ��Ď��K�v������܂��B
  �t�@�C��
      /file/dwc/utility.bin
  �� NITRO ���̃��[�g�f�B���N�g������
      /dwc/
  �̈ʒu�ɔz�u����K�v������܂��B
  ��̓I�ɂ́AROM �X�y�b�N�t�@�C���� RomSpec �Z�b�V������

      RomSpec
      {
	      .
	      .
	      .
          # ��������
		  HostRoot    ./file/dwc    # PC ���t�@�C���f�B���N�g����
          Root        /dwc
          File        utility.bin
          # �����܂�
      }

  �� 3�s��t�������Ă��������B

�� �A�[�J�C�u��
      "dwc"
  �Ƃ����A�[�J�C�u����\�񂵂Ă��܂��B
  �A�v���P�[�V�������ł͎g�p���Ȃ��ł��������B

�� �Ăяo�����̒���
  �E���ׂĂ̏������I�������Ă���ݒ胆�[�e�B���e�B�J�n�֐����R�[������
    ���������B
	�^�b�`�p�l���̃I�[�g�T���v�����O������~�����Ă��������B
  �E�ݒ胆�[�e�B���e�B�͐^�����ȉ�ʂ���n�܂�悤�ɍ쐬����Ă���̂ŁA
    ������ʂ���Ăяo���Ǝ��R�Ȍq����ɂȂ�܂��B

�� �֐��I�����̒���
  �ELCD �ւ̏o�͂� OFF �̏�ԂŏI�����܂��B
  �E���ׂĂ� IRQ ���荞�݂��֎~������ԂŏI�����܂��B
  �EVRAM �̏�Ԃ͕s��ł��B
  �E�������A�֐��̕Ԓl���G���[�̏ꍇ�́A���������������A�����ɏI���ƂȂ�܂��B

/*---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// �@�\ : �ݒ胆�[�e�B���e�B�J�n
//          ���[�N������������Ŋm�ۂ��܂�
// ���� : language  - �A�v���P�[�V�����Ŏg�p���Ă��錾��
//        param     - �ȉ�����I�����Ă�������
//                        DWC_UTILITY_TOP_MENU_EXCEPT_JPN
//                        DWC_UTILITY_SETTING_EXCEPT_JPN
//                        DWC_UTILITY_TOP_MENU_FOR_JPN
//                        DWC_UTILITY_SETTING_FOR_JPN
// �Ԓl : int       - 0        : ����I��
//                    ����ȊO : �����G���[
//-----------------------------------------------------------------------------
int  DWC_StartUtilityEx(int language, int param);


#ifdef  __cplusplus
}
#endif         // __cplusplus


#endif // DWC_UTILITY_H_

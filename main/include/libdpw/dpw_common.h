/*---------------------------------------------------------------------------*
  Project:  DP WiFi Library
  File:     dpw_common.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*! @file
	@brief	DP WiFi Common ���C�u����
	
	@author	Yamaguchi Ryo(yamaguchi_ryo@nintendo.co.jp)
	
*/

#ifndef DPW_COMMON_H_
#define DPW_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _NITRO
#include <dwc.h>
#else
#include <nitro.h>
#include <dwc.h>
#endif

// ----------------------------------------------------------------------------
// localize_spec_mark(LANG_ALL) imatake 2008/07/29
// PG5_WIFIRELEASE = yes �Ȃ�AWi-Fi�֌W�������[�X�T�[�o�ɐڑ�
#ifdef PG5_WIFIRELEASE
// �����[�X�T�[�o�ɐڑ�����ɂ͉��̃R�����g�A�E�g���폜���Ă��������B
#define DPW_SERVER_PUBLIC
#endif
// ----------------------------------------------------------------------------

/*-----------------------------------------------------------------------*
					�^�E�萔�錾
 *-----------------------------------------------------------------------*/

//! ���[���A�h���X�̒���(NULL�I�[������)
#define DPW_MAIL_ADDR_LENGTH 55

//! ���[���A�h���X�̔F�؂��J�n����p�X���[�h
#define DPW_MAIL_ADDR_AUTH_START_PASSWORD 0xffff

//! ���[���A�h���X�̔F�؂������I�ɒʂ��f�o�b�O�p�p�X���[�h�A��ɔF�،��ʂ�DPW_PROFILE_AUTHRESULT_SUCCESS�ɂȂ�܂��B�F�؃��[���͑��M����܂���B
#define DPW_MAIL_ADDR_AUTH_DEBUG_PASSWORD 9999

//! Dpw_Common_Profile�\���̂�mailRecvFlag�����o�̃r�b�g�t���O
typedef enum {
    DPW_PROFILE_MAILRECVFLAG_EXCHANGE = 0x1 //!< �|�P�������������ꂽ�Ƃ��̃��[������M���邩
}DPW_PROFILE_MAILRECVFLAG;

//! Dpw_Common_Profile�\���̂�flag�����o�̃r�b�g�t���O
typedef enum {
    DPW_PROFILE_FLAG_HANGEUL_AVAILABLE = 0x1 //!< �n���O��������\���ł��邩
}DPW_PROFILE_FLAG;

//! �����̏��o�^�p�\����
typedef struct {
    u8      version;                        //!< �o�[�W����
    u8      language;                       //!< ����R�[�h
	u8      countryCode;                    //!< �Z��ł��鍑�R�[�h
	u8      localCode;                      //!< �Z��ł���n���R�[�h
	u32     playerId;			            //!< �v���C���[ID
	u16     playerName[8];		            //!< �v���C���[��(�|�P�����R�[�h)
    u32     flag;                           //!< �e��t���O�ADPW_PROFILE_FLAG�񋓑̂̒l���Z�b�g���Ă��������B
    u8      macAddr[6];                     //!< MAC�A�h���X�A���C�u�������Ŋi�[����̂ŃZ�b�g����K�v�͂���܂���B
    u8      reserved[2];                    //!< �p�f�B���O
    char    mailAddr[DPW_MAIL_ADDR_LENGTH+1]; //!< ���[���A�h���X�BASCII�������NULL�I�[�������̂��Z�b�g���Ă��������B���[�����M�@�\���g�p���Ȃ��ꍇ�͋󕶎������Ă��������B
    u32     mailRecvFlag;                   //!< ���[����M�t���O�ADPW_PROFILE_MAILRECVFLAG�񋓑̂̒l���Z�b�g���Ă��������B
    u16     mailAddrAuthVerification;       //!< ���[���A�h���X�F�ؗp�m�F�R�[�h�B0�`999�̐�������͂��Ă��������B
    u16     mailAddrAuthPass;               //!< ���[���A�h���X�F�؃p�X���[�h�B�F�؂��J�n����Ƃ���DPW_MAIL_ADDR_AUTH_START_PASSWORD���Z�b�g���Ă��������BDPW_MAIL_ADDR_AUTH_DEBUG_PASSWORD���w�肷��Ƌ����I�ɔF�؂�ʂ��܂��B(�f�o�b�O�p)
} Dpw_Common_Profile;

//! Dpw_Common_ProfileResult�\���̂�code�����o�Ɋi�[����錋��
typedef enum {
    DPW_PROFILE_RESULTCODE_SUCCESS,             //!< ���̓o�^�ɐ������܂����BmailAddrAuthResult�����o�ɔF�،��ʂ��i�[����܂��B
    DPW_PROFILE_RESULTCODE_ERROR_INVALIDPARAM,  //!< ���M�����p�����[�^���s���ł��B
    DPW_PROFILE_RESULTCODE_ERROR_SERVERSTATE    //!< �T�[�o�̏�Ԃ������e�i���X�������͈ꎞ��~���ł��B
} DPW_PROFILE_RESULTCODE;

//! Dpw_Common_ProfileResult�\���̂�mailAddrAuthResult�����o(���[���A�h���X�F�،���)�Ɋi�[����錋�ʁB
typedef enum {
    DPW_PROFILE_AUTHRESULT_SUCCESS,     //!< �F�ؐ����B���[���@�\���L���ɂȂ�A���[���̗L��������31���ɉ�������܂����B���[���A�h���X�ɋ󕶎�����ꂽ�ꍇ�͕K�����̒l���Ԃ�A���[���@�\�������ɂȂ�܂��B
    DPW_PROFILE_AUTHRESULT_SEND,        //!< �F�؃��[���𑗐M���܂����B�F�؃��[���ɏ�����Ă���p�X���[�h��mailAddrAuthPass�ɃZ�b�g���A�ēxDpw_xx_SetProfileAsync�֐����Ă�ł��������B
    DPW_PROFILE_AUTHRESULT_SENDFAILURE, //!< �F�؃��[���̑��M�Ɏ��s���܂����B�����炭���[���A�h���X���Ԉ���Ă��܂��B
    DPW_PROFILE_AUTHRESULT_FAILURE      //!< �F�؂Ɏ��s���܂����B�F�؃��[���ɋL�ڂ���Ă���p�X���[�h�Ƒ��M���ꂽ�p�X���[�h���Ⴂ�܂��B�������͔F�؃��[���̑��M��ƈقȂ郁�[���A�h���X�ł��B
} DPW_PROFILE_AUTHRESULT;

//! �����̏��o�^���X�|���X�p�\����
typedef struct {
    u32 code;                   //!< ���ʃR�[�h�BDPW_PROFILE_RESULTCODE�񋓑̂̒l���i�[����܂��B
    u32 mailAddrAuthResult;     //!< ���[���A�h���X�F�،��ʁBcode�����o��DPW_PROFILE_RESULTCODE_SUCCESS���Z�b�g���ꂽ�ꍇ�̂݁A������DPW_PROFILE_AUTHRESULT�񋓑̂̒l���i�[����܂��B������DP�؍���łł͕K��0�ɂȂ�܂��B
} Dpw_Common_ProfileResult;

/*-----------------------------------------------------------------------*
					�O���[�o���ϐ���`
 *-----------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*
					�֐��O���錾
 *-----------------------------------------------------------------------*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // DPW_COMMON_H_
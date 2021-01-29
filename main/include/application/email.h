//==============================================================================
/**
 * @file	email.h
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2007.10.19(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================

#ifndef __EMAIL_H__
#define __EMAIL_H__

#include "system/procsys.h"
#include "libdpw/dpw_common.h"
#include "email_main.h"
	
#define	EMAIL_INPUT_ADDRESS		( EMAIL_MODE_INPUT_EMAIL )
#define	EMAIL_INPUT_NUMBER		( EMAIL_MODE_INPUT_AUTHENTICATE )
#define	EMAIL_INPUT_PASSWORD	( EMAIL_MODE_INPUT_PASSWORD )
#define	EMAIL_INPUT_VIEW		( EMAIL_MODE_INPUT_EMAIL_CHECK )

#define PASS_CODE_MAX		( 4 + 1 )
#define EMAIL_CODE_MAX		( 50 + 1 )
#define EMAIL_NUM_MAX		( 4 + 4 + 1 )
#define EMAIL_PASS_MAX		( 4 + 1 )
#define EMAIL_CODE_1LINE	( 25 )

#define EMAIL_CUR_BACK_CPX	( 5 )
#define EMAIL_CUR_END_CPX	( 7 )

#define EMAIL_CUR_BACK_PX	( 2 )
#define EMAIL_CUR_BACK_PY	( 6 )
#define EMAIL_CUR_END_PX	( 9 )
#define EMAIL_CUR_END_PY	( EMAIL_CUR_BACK_PY )


///< OMA ���W
#define EMAIL_OAM_BTN_OX	( 0 )	///< �{�^���̃I�t�Z�b�g
#define EMAIL_OAM_BTN_OY	( 2 )
#define EMAIL_OAM_PNL_OX	( 8 )	///< �p�l���̃I�t�Z�b�g
#define EMAIL_OAM_PNL_OY	( 8 )

///< �A�j����`
enum {	
	eEM_ANM_BACK_1	= 0,
	eEM_ANM_BACK_2,
	eEM_ANM_END_1,
	eEM_ANM_END_2,
	eEM_ANM_CUR_1,
	eEM_ANM_BAR_1,		///< ���g�p
	eEM_ANM_BAR_2,
	eEM_ANM_CUR_2,
	eEM_ANM_CUR_BIG,
};

enum {
	
	eINFO_ADRS_INPUT = 0,	///< e-mail���͂���
	eINFO_NUMBER_INPUT,		///< �F�؃R�[�h���͂���
	eINFO_PASS_INPUT,		///< �p�X���[�h���͂���
	eINFO_ADRS_RE_INPUT,	///< e-mail��������
	eINFO_ADRS_ERR,			///< e-mail�������[
	eINFO_PASS_ERR,			///< �p�X���������[
	
	eINFO_MAIL_ADDRESS = 0xFF,
};

enum {	
	EMAIL_CODE_0	= 0,
	EMAIL_CODE_ETC	= 10,
	EMAIL_CODE_A	= 15,
	EMAIL_CODE_a	= 41,
	EMAIL_BACK		= 67,	
	EMAIL_END,
	
	EMAIL_BTN_MAX	= 10 + 5 + 52 + 2,
};

typedef struct {
	
	int mode;		///< EMailMode�Q��
	STRBUF* now_str;	///< ���ݐݒ肳��Ă��郁�[���A�h���X
	STRBUF* str;		///< �i�[��(�Ăяo�����͌��ݐݒ肳��Ă���A�h���X�������Ă���)

	u16 authenticate_rand_code;		///<���[�J���Ō��肳���F�؃R�[�h�̏�3��
	u16 password;					///<���[�U�[���ݒ�ς݂̃p�X���[�h
	
	//�߂�l
	u8 ret_address_flag;			///<�A�h���X���͉�ʂ��I���������̏��(EMAIL_ADDRESS_RET_???)
	u32 ret_password;				///<�p�X���[�h���͂̌��ʒl
	u32 ret_authenticate_code;		///<�F�؃R�[�h���͂̌��ʒl(7��)orEMAIL_AUTHENTICATE_CODE_CANCEL
	
} EMAIL_PARAM;

extern const PROC_DATA EMail_ProcData;

extern PROC_RESULT EMail_Proc_Init( PROC* proc, int* seq );
extern PROC_RESULT EMail_Proc_Main( PROC* proc, int* seq );
extern PROC_RESULT EMail_Proc_Exit( PROC* proc, int* seq );


#endif


//==============================================================================
/**
 * @file	email_main.h
 * @brief	E���[����ʂ̃w�b�_
 * @author	matsuda
 * @date	2007.10.18(��)
 */
//==============================================================================
#ifndef __EMAIL_MAIN_H__
#define __EMAIL_MAIN_H__

#include "gflib/strbuf.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�T�u�v���Z�X�w��
enum{
	EMAIL_SUBPROC_MENU,					///<���j���[
	EMAIL_SUBPROC_ADDRESS_INPUT,		///<E���[������
	EMAIL_SUBPROC_AUTHENTICATE_INPUT,	///<�F�ؔԍ�����
	EMAIL_SUBPROC_PASSWORD_INPUT,		///<�p�X���[�h����
	EMAIL_SUBPROC_ADDRESS_CHECK,		///<E���[���m�F
	EMAIL_SUBPROC_GSPROFILE_GET,		///<�����̃v���t�@�C��ID�擾���[�h
	
	EMAIL_SUBPROC_END,
};


///���͉�ʂ̋N�����[�h
enum{
	EMAIL_MODE_INPUT_EMAIL,			///<E���[�����̓��[�h
	EMAIL_MODE_INPUT_AUTHENTICATE,	///<�F�ؔԍ����̓��[�h
	EMAIL_MODE_INPUT_PASSWORD,		///<�p�X���[�h���̓��[�h
	EMAIL_MODE_INPUT_EMAIL_CHECK,	///<E���[���m�F���[�h
};

///�F�؃R�[�h���͂��L�����Z���I�������ꍇ�̕Ԏ�
#define EMAIL_AUTHENTICATE_CODE_CANCEL		(0xffffffff)

///E���[���A�h���X�̕������T�C�Y(�Z�[�u�ł͂Ȃ��V�[���Ŏg�p����q�[�v�T�C�Y�p)
#define EMAIL_ADDRESS_LEN_SIZE		(100)	//STRBUF�̃T�C�Y�Ȃ̂ŗ]�T��������100������

///�A�h���X���͉�ʂ��I���������̖߂�l
enum{
	EMAIL_ADDRESS_RET_SET,			///<�A�h���X���Z�b�g���ďI��
	EMAIL_ADDRESS_RET_CANCEL,		///<�A�h���X���Z�b�g�����ɃL�����Z�����ďI��
};


//==============================================================================
//	�\���̒�`
//==============================================================================
///E���[����ʐ���p�V�X�e�����[�N�\����
typedef struct{
	PROC *sub_proc;					///<���s���̃T�u�v���Z�X
	SAVEDATA *savedata;				///<�Z�[�u�f�[�^�ւ̃|�C���^
	CONFIG *config;					///<�R���t�B�O

	void 			*heapPtr;		///<NitroDWC�ɓn���q�[�v���[�N�̉���p�|�C���^
	NNSFndHeapHandle heapHandle;	///<heapPtr��32�o�C�g�A���C�����g�ɍ��킹���|�C���^

	DWCInetControl   stConnCtrl;							// DWC�ڑ����[�N
	
	u16 authenticate_rand_code;		///<���[�J���Ō��肳���F�؃R�[�h�̏�3��
	
	u8 sub_menu_recovery_mode;		///<���j���[�N�����̃��[�h

	int comm_initialize_ok;

	int now_process;						// ���ݎ��s���̃T�u�v���Z�X
	int	sub_process;						// ���E�����T�u�v���O��������i���o�[
	int	sub_nextprocess;					// ���E�����T�uNEXT����i���o�[
	int sub_process_mode;					// 
	int	sub_returnprocess;					// next�v���Z�X���Ă�ŏI���������̖߂�v���Z�X

	void *sub_proc_parent_work;		///<�T�uPROC�Ăяo�����ɍ쐬����parent_work�ێ��|�C���^

	Dpw_Common_Profile dc_profile;					///<���M�v���t�B�[���f�[�^
	Dpw_Common_ProfileResult dc_profile_result;		///<���M���ʎ�M���[�N
	
	//�e�T�uPROC�Ԃ̃f�[�^�̂����Ŏg�p
	u32 ret_password;		///<�p�X���[�h���͂̌��ʒl
	u32 ret_authenticate_code;	///<�F�؃R�[�h���͂̌��ʒl(��4��)
	u8 ret_address_flag;		///<E���[���A�h���X���͉�ʂ��I���������̏��
	u8 subproc_padding[3];	//�p�f�B���O
	STRBUF *email_address;			///<E���[���A�h���X�̕�����Z�b�g�ꏊ
	STRBUF *now_email_address;		///<���ݐݒ肳��Ă���E���[���A�h���X

	BOOL dpw_tr_init;		///<TRUE:Dpw_Tr_Init���s�ς�
}EMAIL_SYSWORK;


//==============================================================================
//	�O���֐��錾
//==============================================================================
//-- email_main.c --//
extern PROC_RESULT EmailProc_Init( PROC * proc, int * seq );
extern PROC_RESULT EmailProc_Main( PROC * proc, int * seq );
extern PROC_RESULT EmailProc_End(PROC *proc, int *seq);
extern void Email_SubProcessChange( EMAIL_SYSWORK *esys, int subprccess, int mode );
extern void Email_SubProcessEndSet(EMAIL_SYSWORK *esys);
extern void Email_RecoveryMenuModeSet(EMAIL_SYSWORK *esys, int mode);
extern int Email_RecoveryMenuModeGet(EMAIL_SYSWORK *esys);
extern void Email_AddressSaveWorkSet(EMAIL_SYSWORK *esys);
extern void Email_AuthenticateCodeSaveWorkSet(EMAIL_SYSWORK *esys);
extern void Email_PasswordSaveWorkSet(EMAIL_SYSWORK *esys);
extern void Email_DCProfileCreate(EMAIL_SYSWORK *esys);
extern void Email_DCProfileSet_Address(EMAIL_SYSWORK *esys);
extern void Email_DCProfileSet_Authenticate(EMAIL_SYSWORK *esys);
extern void Email_AuthenticateRandCodeSet(EMAIL_SYSWORK *esys, u32 code);
extern u32 Email_AuthenticateRandCodeGet(EMAIL_SYSWORK *esys);
extern void Email_AuthenticateCodeSet(EMAIL_SYSWORK *esys, u32 code);
extern u32 Email_AuthenticateCodeGet(EMAIL_SYSWORK *esys);
extern void Email_PasswordNumberSet(EMAIL_SYSWORK *esys, u32 password);
extern u32 Email_PasswordNumberGet(EMAIL_SYSWORK *esys);
extern void Email_AddressReturnFlagSet(EMAIL_SYSWORK *esys, int flag);
extern int Email_AddressReturnFlagGet(EMAIL_SYSWORK *esys);
extern STRBUF * Email_AddressStrbufGet(EMAIL_SYSWORK *esys);

//-- email_enter.c --//
extern PROC_RESULT EmailMenu_Enter_Init( PROC * proc, int * seq );
extern PROC_RESULT EmailMenu_Enter_Main( PROC * proc, int * seq );
extern PROC_RESULT EmailMenu_Enter_End(PROC *proc, int *seq);

//-- email_input.c --//
extern PROC_RESULT EmailInput_Init( PROC * proc, int * seq );
extern PROC_RESULT EmailInput_Main( PROC * proc, int * seq );
extern PROC_RESULT EmailInput_End(PROC *proc, int *seq);

//-- email_tool.c --//
extern void Email_Strcode_to_Ascii(STRBUF *src, char *dest, int heap_id);
extern void Email_Ascii_to_Strcode(char *src, STRBUF *dest, int heap_id);

//-- email_test.c ����ŏ��� --//
extern PROC_RESULT EmailAddress_Init( PROC * proc, int * seq );
extern PROC_RESULT EmailAddress_Main( PROC * proc, int * seq );
extern PROC_RESULT EmailAddress_End(PROC *proc, int *seq);


#endif	//__EMAIL_MAIN_H__

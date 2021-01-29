//==============================================================================
/**
 * @file	email_savedata.h
 * @brief	E���[���Z�[�u�f�[�^����֘A
 * @author	matsuda
 * @date	2007.10.23(��)
 */
//==============================================================================
#ifndef	__EMAIL_SAVEDATA_H__
#define	__EMAIL_SAVEDATA_H__

#include "gflib/strbuf.h"
#include "libdpw/dpw_common.h"


//============================================================================================
//============================================================================================
//---------------------------------------------------------------------------
/**
 * @brief	�R���e�X�g�f�[�^�̕s���S�^��`
 */
//---------------------------------------------------------------------------
typedef struct _EMAIL_SAVEDATA EMAIL_SAVEDATA;


//==============================================================================
//	�萔��`
//==============================================================================
///�p�����[�^�Z�b�g���[�h
enum{
	EMAIL_PARAM_RECV_FLAG,			///<��M�ݒ�t���O(DPW_PROFILE_MAILRECVFLAG_EXCHANGE or 0)
	EMAIL_PARAM_AUTH_CODE_HIGH,		///<�F�؃R�[�h(��3��)
	EMAIL_PARAM_AUTH_CODE_LOW,		///<�F�؃R�[�h(��4��)
	EMAIL_PARAM_PASSWORD,			///<���[�U�[�����肷��p�X���[�h
};


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern int	EMAILSAVE_GetWorkSize(void);
extern void EMAILSAVE_Init(EMAIL_SAVEDATA * emaildata);
extern void EMAILSAVE_DataInitialize(SAVEDATA *sv);
extern BOOL EMAILSAVE_UseCheck(SAVEDATA *sv);
extern void EMAILSAVE_AddressSet(SAVEDATA *sv, const char *address);
extern char * EMAILSAVE_AddressGet(SAVEDATA *sv);
extern void EMAILSAVE_ParamSet(SAVEDATA *sv, int param_id, u32 data);
extern u32 EMAILSAVE_ParamGet(SAVEDATA *sv, int param_id);
extern u32 EMAILSAVE_DCProfileCreate_AuthStart(SAVEDATA *sv, Dpw_Common_Profile *dc_profile);
extern void EMAILSAVE_DCProfileCreate_Update(SAVEDATA *sv, Dpw_Common_Profile *dc_profile);


#endif	__EMAIL_SAVEDATA_H__

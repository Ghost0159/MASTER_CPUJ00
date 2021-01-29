//==============================================================================
/**
 * @file	email_savedata.c
 * @brief	E���[���Z�[�u�f�[�^����
 * @author	matsuda
 * @date	2007.10.23(��)
 */
//==============================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/savedata.h"
#include "savedata/email_savedata.h"
#include "gflib/strcode.h"
#include "gflib/strbuf_family.h"
#include "system/buflen.h"
#include "libdpw/dpw_common.h"
#include "savedata/wifihistory.h"
#include "savedata/mystatus.h"
#include "system/pm_str.h"


//==============================================================================
//	�萔��`
//==============================================================================
///E���[���A�h���X�̕�����(�I�['\0'����)
#define EMAIL_STR_LEN				(50 + 1)


//==============================================================================
//	�\���̒�`
//==============================================================================
///E���[���Z�[�u�f�[�^�\����
struct _EMAIL_SAVEDATA{
	char email_ascii[EMAIL_STR_LEN];	///<E���[���A�h���X
	int recv_flag;
	u16 auth_code_high;					///<�F�؃R�[�h(��3��)
	u16 auth_code_low;					///<�F�؃R�[�h(��4��)
	u32 password;						///<�p�X���[�h
};


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void EMAILSAVE_DCProfileCreateCommon(SAVEDATA *sv, Dpw_Common_Profile *dc_profile);



//--------------------------------------------------------------
/**
 * @brief	E���[���̃Z�[�u�f�[�^�T�C�Y���擾
 *
 * @param	none	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
int	EMAILSAVE_GetWorkSize(void)
{
	return sizeof(EMAIL_SAVEDATA);
}

//---------------------------------------------------------------------------
/**
 * @brief	E���[���Z�[�u�f�[�^�̏���������
 * @param	emaildata		E���[���Z�[�u�f�[�^�ւ̃|�C���^
 */
//---------------------------------------------------------------------------
void EMAILSAVE_Init(EMAIL_SAVEDATA * emaildata)
{
	MI_CpuClear8(emaildata, sizeof(EMAIL_SAVEDATA));
	
	memset(emaildata->email_ascii, '\0', EMAIL_STR_LEN);
	emaildata->recv_flag = DPW_PROFILE_MAILRECVFLAG_EXCHANGE;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_EMAIL)
	SVLD_SetCrc(GMDATA_ID_EMAIL);
#endif //CRC_LOADCHECK
}

//--------------------------------------------------------------
/**
 * @brief   E���[���Z�[�u�f�[�^����������Ԃɂ���
 *
 * @param   sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * �Z�[�u�V�X�e���ȊO�̏ꏊ���珉��������ꍇ�A���̊֐����g�p����
 */
//--------------------------------------------------------------
void EMAILSAVE_DataInitialize(SAVEDATA *sv)
{
	EMAILSAVE_Init(SaveData_Get(sv, GMDATA_ID_EMAIL));
}

//--------------------------------------------------------------
/**
 * @brief   E���[���A�h���X���ݒ肳��Ă��邩���ׂ�
 *
 * @param   sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @retval  TRUE:�ݒ肳��Ă���B
 * @retval  FALSE:�ݒ肳��Ă��Ȃ�
 */
//--------------------------------------------------------------
BOOL EMAILSAVE_UseCheck(SAVEDATA *sv)
{
	EMAIL_SAVEDATA *emaildata;
	
	emaildata = SaveData_Get(sv, GMDATA_ID_EMAIL);
	if(emaildata->email_ascii[0] == '\0'){
		return FALSE;
	}
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   E���[���A�h���X���Z�b�g����
 *
 * @param   sv			�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   address		�Z�b�g����E���[���A�h���X(ASCII�R�[�h)
 */
//--------------------------------------------------------------
void EMAILSAVE_AddressSet(SAVEDATA *sv, const char *address)
{
	EMAIL_SAVEDATA *emaildata;
	
	emaildata = SaveData_Get(sv, GMDATA_ID_EMAIL);
	strcpy(emaildata->email_ascii, address);
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_EMAIL)
	SVLD_SetCrc(GMDATA_ID_EMAIL);
#endif //CRC_LOADCHECK
}

//--------------------------------------------------------------
/**
 * @brief   E���[���A�h���X���擾����(ASCII�R�[�h)
 *
 * @param   sv			�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @retval  TRUE:E���[���A�h���X���Z�b�g����Ă���
 * @retval  FALSE:E���[���A�h���X�̓Z�b�g����Ă��Ȃ�
 */
//--------------------------------------------------------------
char * EMAILSAVE_AddressGet(SAVEDATA *sv)
{
	EMAIL_SAVEDATA *emaildata;
	
	emaildata = SaveData_Get(sv, GMDATA_ID_EMAIL);
	return emaildata->email_ascii;
}

//--------------------------------------------------------------
/**
 * @brief   E���[���Z�[�u�f�[�^�F�p�����[�^�Z�b�g
 *
 * @param   sv				�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   param_id		�p�����[�^ID
 * @param   data			�f�[�^
 */
//--------------------------------------------------------------
void EMAILSAVE_ParamSet(SAVEDATA *sv, int param_id, u32 data)
{
	EMAIL_SAVEDATA *emaildata = SaveData_Get(sv, GMDATA_ID_EMAIL);

	switch(param_id){
	case EMAIL_PARAM_RECV_FLAG:		// DPW_PROFILE_MAILRECVFLAG_EXCHANGE or 0
		emaildata->recv_flag = data;
		break;
	case EMAIL_PARAM_AUTH_CODE_HIGH:
		emaildata->auth_code_high = data;
		break;
	case EMAIL_PARAM_AUTH_CODE_LOW:
		emaildata->auth_code_low = data;
		break;
	case EMAIL_PARAM_PASSWORD:
		emaildata->password = data;
		break;
	}
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_EMAIL)
	SVLD_SetCrc(GMDATA_ID_EMAIL);
#endif //CRC_LOADCHECK
}

//--------------------------------------------------------------
/**
 * @brief   E���[���Z�[�u�f�[�^�F�p�����[�^�擾
 *
 * @param   sv				�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   param_id		�p�����[�^ID
 * 
 * @retval	�f�[�^
 */
//--------------------------------------------------------------
u32 EMAILSAVE_ParamGet(SAVEDATA *sv, int param_id)
{
	EMAIL_SAVEDATA *emaildata = SaveData_Get(sv, GMDATA_ID_EMAIL);

	switch(param_id){
	case EMAIL_PARAM_RECV_FLAG:		// DPW_PROFILE_MAILRECVFLAG_EXCHANGE or 0
		return emaildata->recv_flag;
	case EMAIL_PARAM_AUTH_CODE_HIGH:
		return emaildata->auth_code_high;
	case EMAIL_PARAM_AUTH_CODE_LOW:
		return emaildata->auth_code_low;
	case EMAIL_PARAM_PASSWORD:
		return emaildata->password;
	}
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief   �v���t�B�[���f�[�^���ʕ����쐬
 *
 * @param   sv				�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   dc_profile		�v���t�B�[�������
 */
//--------------------------------------------------------------
static void EMAILSAVE_DCProfileCreateCommon(SAVEDATA *sv, Dpw_Common_Profile *dc_profile)
{
	WIFI_HISTORY *wh = SaveData_GetWifiHistory(sv);
	MYSTATUS *my = SaveData_GetMyStatus(sv);
	char *email_address = EMAILSAVE_AddressGet(sv);
	
	MI_CpuClear8(dc_profile, sizeof(Dpw_Common_Profile));
	
	dc_profile->version = PM_VERSION;
	dc_profile->language = PM_LANG;
	dc_profile->countryCode = WIFIHISTORY_GetMyNation(wh);
	dc_profile->localCode = WIFIHISTORY_GetMyArea(wh);
	dc_profile->playerId = MyStatus_GetID(my);
	
	PM_strcpy(dc_profile->playerName, MyStatus_GetMyName(my));
	
	dc_profile->flag = 0;	//�n���O��������\���ł��邩
//	dc_profile->macAddr		���C�u�������Ŋi�[����̂ŃZ�b�g�̕K�v�Ȃ�
	
	strcpy(dc_profile->mailAddr, email_address);

	dc_profile->mailRecvFlag = EMAILSAVE_ParamGet(sv, EMAIL_PARAM_RECV_FLAG);
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_EMAIL)
	SVLD_SetCrc(GMDATA_ID_EMAIL);
#endif //CRC_LOADCHECK
}

//--------------------------------------------------------------
/**
 * @brief   �F�؊J�n�p�v���t�B�[���쐬
 *
 * @param   sv				�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   dc_profile		�v���t�B�[�������
 *
 * @retval  �F�ؗp�Ƀ��[�J���Ō��肳�ꂽ�o�^�R�[�h��3���̔ԍ�
 */
//--------------------------------------------------------------
u32 EMAILSAVE_DCProfileCreate_AuthStart(SAVEDATA *sv, Dpw_Common_Profile *dc_profile)
{
	u32 AuthVerification;
	
	EMAILSAVE_DCProfileCreateCommon(sv, dc_profile);
	
	AuthVerification = gf_rand() % 1000;	//0�`999�̒l�������_���ŃZ�b�g
	
	dc_profile->mailAddrAuthVerification = AuthVerification;
	dc_profile->mailAddrAuthPass = DPW_MAIL_ADDR_AUTH_START_PASSWORD;
	
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_EMAIL)
	SVLD_SetCrc(GMDATA_ID_EMAIL);
#endif //CRC_LOADCHECK
	return AuthVerification;
}

//--------------------------------------------------------------
/**
 * @brief   ���E�����A�o�g���^���[���̐ڑ��̍ۂɎ��s��������X�V�p�̃v���t�B�[���쐬
 *
 * @param   sv				�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   dc_profile		�v���t�B�[�������
 */
//--------------------------------------------------------------
void EMAILSAVE_DCProfileCreate_Update(SAVEDATA *sv, Dpw_Common_Profile *dc_profile)
{
	EMAIL_SAVEDATA *emaildata = SaveData_Get(sv, GMDATA_ID_EMAIL);

	EMAILSAVE_DCProfileCreateCommon(sv, dc_profile);
	
	dc_profile->mailAddrAuthVerification = emaildata->auth_code_high;	//�ꉞ����Ă���
	dc_profile->mailAddrAuthPass = emaildata->auth_code_low;
	OS_TPrintf("mailAddrAuthVerification = %d\n", dc_profile->mailAddrAuthVerification);
	OS_TPrintf("mailAddrAuthPass = %d\n", dc_profile->mailAddrAuthPass);
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_EMAIL)
	SVLD_SetCrc(GMDATA_ID_EMAIL);
#endif //CRC_LOADCHECK
}


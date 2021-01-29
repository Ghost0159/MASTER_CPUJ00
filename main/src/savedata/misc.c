//============================================================================================
/**
 * @file	misc.c
 * @brief	�����ރZ�[�u�f�[�^�̃A�N�Z�X
 * @author	tamada	GAME FREAK Inc.
 * @date	2006.01.26
 *
 * ���ނ���Ă��Ȃ��Z�[�u�f�[�^�͂Ƃ肠���������ɒǉ������B
 * �A�v���P�[�V�������x���A�Z�[�u�f�[�^�A�N�Z�X�֐����x�������
 * ����MISC�\���̂����o�����Ƃ͂ł��Ȃ��B
 * MISC�\���̂̓����o�Ɏ��\���̂ւ̃A�N�Z�X��񋟂��邾����
 * �C���^�[�t�F�C�X�Ƃ��ċ@�\���Ă���B
 *
 * �Z�[�u�f�[�^�Ƃ��Ă̐�����������ƒ�`�ł���悤�ɂȂ������_�ŁA
 * �����̃Z�[�u�\���̂͂�����ƕʃ\�[�X�ɐ؂蕪�����邱�ƂɂȂ�B
 *
 * 2006.06.02
 * ���Ԑ؂�ŕ��ނ��킩�����̂͂����ɒǉ����邱�Ƃɂ��܂����B
 * ���O�B
 */
//============================================================================================

#include "common.h"
#include "gflib/system.h"

#include "system/gamedata.h"

#include "savedata/savedata.h"

#include "savedata/misc.h"
#include "misc_local.h"

#include "seedbed_local.h"
#include "savedata/seedbed.h"
#include "gimmickwork_local.h"
#include "savedata/gimmickwork.h"


#include "gflib/strbuf_family.h"
#include "system/pm_str.h"

#include "msgdata/msg.naix"
#include "system/pms_data.h"
#include "system/pms_word.h"
#include "msgdata\msg_pmss_union.h"
#include "msgdata\msg_pmss_ready.h"
#include "msgdata\msg_pmss_won.h"
#include "msgdata\msg_pmss_lost.h"
#include "msgdata\msg_pms_word06.h"
#include "msgdata\msg_pms_word07.h"
#include "msgdata\msg_pms_word08.h"
#include "msgdata\msg_pms_word09.h"
#include "msgdata\msg_pms_word10.h"
#include "msgdata\msg_pms_word11.h"
#include "msgdata\msg_pms_word12.h"

//============================================================================================
//============================================================================================
//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�̎Q�Ɛ�ƃ����_���L�[�̐���\���̒�`
 * 			�a������f�[�^�̂݁ADP���炠��̂ŏ���
 */
//--------------------------------------------------------------
typedef struct{
	EX_CERTIFY_SAVE_KEY key[EXDATA_ID_MAX - EXDATA_ID_FRONTIER];		//�F�؃L�[
	EX_CERTIFY_SAVE_KEY old_key[EXDATA_ID_MAX - EXDATA_ID_FRONTIER];	//1�O�̔F�؃L�[
	u8 flag[EXDATA_ID_MAX - EXDATA_ID_FRONTIER];						//�Q�Ɛ�t���O
	u8 padding[3];
}EX_SAVE_KEY;

//---------------------------------------------------------------------------
/**
 * @brief	MISC�\���̂̒�`
 *
 * ���L�̂悤�ȃZ�[�u�f�[�^�u���b�N�́AMISC�̃����o�[�Ƃ��Ċm�ۂ���B
 * -�ǂ��ɕ��ނ���ׂ����A���f�ɖ������́B���͕��ނ�ۗ����Ă�����������
 * -�K�͂��������A���̂��߂ɃZ�[�u�u���b�N���m�ۂ���̂����������Ȃ�����
 */
//---------------------------------------------------------------------------
struct _MISC {
	SEEDBED seedbed[SEEDBED_MAX];
	GIMMICKWORK gimmick;
	STRCODE rivalname[PERSON_NAME_SIZE + EOM_SIZE];
	STRCODE monument_name[MONUMENT_NAME_SIZE + EOM_SIZE];
	
	//�v���`�i����ǉ�
	u16 favorite_monsno;		//���C�ɓ���|�P����
	u8  favorite_form_no:7;		//���C�ɓ���|�P�����̃t�H�����ԍ�
	u8  favorite_egg_flag:1;	//���C�ɓ���|�P�����̃^�}�S�t���O

	u8 extra_init_flag:1;		// �O���Z�[�u�������ς݂��ǂ����̃t���O(TRUE�������ς�)
	u8 battle_recoder_color:4;	// �o�g�����R�[�_�[�̐F(5�F)
	u8 				:3;			//�]��
	
	//���g�p�ɂȂ��� 2008.06.21(�y) matsuda(���XGDS�n�̑��M�ς݃t���O����������)
	u32 dummy;
	
	PMS_DATA gds_self_introduction;		// GDS�v���t�B�[���̎��ȏЉ�b�Z�[�W
	
	EX_SAVE_KEY ex_save_key;	///<�O���Z�[�u�̔F�؃L�[
};

#ifdef	PM_DEBUG
static const STRCODE RivalName[] = {PA_, bou_,RU_, EOM_ };
#endif
//============================================================================================
//============================================================================================

//---------------------------------------------------------------------------
/**
 * @brief	MISC�\���̂̃T�C�Y�擾
 * @return	int		MISC�\���̂̃T�C�Y
 */
//---------------------------------------------------------------------------
int MISC_GetWorkSize(void)
{
	return sizeof(MISC);
}

//---------------------------------------------------------------------------
/**
 */
//---------------------------------------------------------------------------
void MISC_Copy(const MISC * from, MISC * to)
{
	MI_CpuCopy8(from ,to, sizeof(MISC));
}

//============================================================================================
//============================================================================================
//---------------------------------------------------------------------------
/**
 * @brief	�F�X�Z�[�u�f�[�^�ێ����[�N�̏�����
 * @param	misc	�F�X�Z�[�u�f�[�^�ێ����[�N�ւ̃|�C���^
 */
//---------------------------------------------------------------------------
void MISC_Init(MISC * misc)
{
	MI_CpuClearFast(misc, sizeof(MISC));
	/* �ȉ��Ɍʕ����̏��������������� */
	SEEDBED_Init(misc->seedbed);
	GIMMICKWORK_Init(&misc->gimmick);
	MI_CpuFill16(misc->rivalname, EOM_, PERSON_NAME_SIZE + EOM_SIZE);
	MI_CpuFill16(misc->monument_name, EOM_, MONUMENT_NAME_SIZE + EOM_SIZE);
#ifdef	PM_DEBUG
	PM_strcpy(misc->rivalname, RivalName);
#endif

	PMSDAT_Init( &misc->gds_self_introduction, PMS_TYPE_UNION );
	misc->gds_self_introduction.sentence_id = pmss_union_01;
	misc->gds_self_introduction.word[0] 
		= PMSW_GetWordNumberByGmmID( NARC_msg_pms_word08_dat, pms_word08_100 );
	misc->gds_self_introduction.word[1] = PMS_WORD_NULL;

	{
		int i;
		for(i = 0; i < EXDATA_ID_MAX - EXDATA_ID_FRONTIER; i++){
			misc->ex_save_key.key[i] = EX_CERTIFY_SAVE_KEY_NO_DATA;
			misc->ex_save_key.old_key[i] = EX_CERTIFY_SAVE_KEY_NO_DATA;
		}
	}
	
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MISC)
	SVLD_SetCrc(GMDATA_ID_MISC);
#endif //CRC_LOADCHECK
}

//============================================================================================
//
//	�Z�[�u�f�[�^�擾�̂��߂̊֐�
//
//============================================================================================
//---------------------------------------------------------------------------
/**
 * @brief	MISC�Z�[�u�f�[�^�̎擾
 * @param	sv			�Z�[�u�f�[�^�ێ����[�N�ւ̃|�C���^
 * @return	MISC�\���̂ւ̃|�C���^
 */
//---------------------------------------------------------------------------
MISC * SaveData_GetMisc(SAVEDATA * sv)
{
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MISC)
	SVLD_CheckCrc(GMDATA_ID_MISC);
#endif //CRC_LOADCHECK
	return SaveData_Get(sv, GMDATA_ID_MISC);
}

//---------------------------------------------------------------------------
/**
 * @brief	MISC�Z�[�u�f�[�^�̎擾
 * @param	sv			�Z�[�u�f�[�^�ێ����[�N�ւ̃|�C���^
 * @return	MISC�\���̂ւ̃|�C���^
 */
//---------------------------------------------------------------------------
const MISC * SaveData_GetMiscReadOnly(const SAVEDATA * sv)
{
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MISC)
	SVLD_CheckCrc(GMDATA_ID_MISC);
#endif //CRC_LOADCHECK
	return SaveData_GetReadOnlyData(sv, GMDATA_ID_MISC);
}

//---------------------------------------------------------------------------
/**
 * @brief	���̂ݏ�ԃf�[�^�ւ̃|�C���^�擾
 * @param	sv			�Z�[�u�f�[�^�ێ����[�N�ւ̃|�C���^
 * @return	CONFIG		�ݒ���ێ����[�N�ւ̃|�C���^
 */
//---------------------------------------------------------------------------
SEEDBED * SaveData_GetSeedBed(SAVEDATA * sv)
{
	MISC * misc;
	misc = SaveData_Get(sv, GMDATA_ID_MISC);
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MISC)
	SVLD_CheckCrc(GMDATA_ID_MISC);
#endif //CRC_LOADCHECK
	return misc->seedbed;
}

//---------------------------------------------------------------------------
/**
 * @brief	�}�b�v�ŗL�̎d�|�����[�N�ւ̃|�C���^�擾
 * @param	sv			�Z�[�u�f�[�^�ێ����[�N�ւ̃|�C���^
 * @return	GIMMICKWORK	�d�|���p���[�N�ւ̃|�C���^
 */
//---------------------------------------------------------------------------
GIMMICKWORK * SaveData_GetGimmickWork(SAVEDATA * sv)
{
	MISC * misc;
	misc = SaveData_Get(sv, GMDATA_ID_MISC);
	return &misc->gimmick;
}

//============================================================================================
//============================================================================================
//---------------------------------------------------------------------------
/**
 */
//---------------------------------------------------------------------------
const STRCODE * MISC_GetRivalName(const MISC * misc)
{
	return misc->rivalname;
}

//---------------------------------------------------------------------------
/**
 */
//---------------------------------------------------------------------------
void MISC_SetRivalName(MISC * misc, STRBUF * str)
{
	STRBUF_GetStringCode( str, misc->rivalname, PERSON_NAME_SIZE + EOM_SIZE );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MISC)
	SVLD_SetCrc(GMDATA_ID_MISC);
#endif //CRC_LOADCHECK
}

//---------------------------------------------------------------------------
/**
 */
//---------------------------------------------------------------------------
const STRCODE * MISC_GetMonumentName(const MISC * misc)
{
	return misc->monument_name;
}

//---------------------------------------------------------------------------
/**
 */
//---------------------------------------------------------------------------
void MISC_SetMonumentName(MISC * misc, STRBUF * str)
{
	STRBUF_GetStringCode( str, misc->monument_name, MONUMENT_NAME_SIZE + EOM_SIZE );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MISC)
	SVLD_SetCrc(GMDATA_ID_MISC);
#endif //CRC_LOADCHECK
}


//==============================================================================
//==============================================================================
//----------------------------------------------------------
/**
 * @brief	�O���Z�[�u�f�[�^�̏������ς݃t���O�̃Z�b�g
 * @param	my		������ԕێ����[�N�ւ̃|�C���^
 */
//----------------------------------------------------------
void MISC_SetExtraInitFlag(MISC * misc)
{
	misc->extra_init_flag = 1;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MISC)
	SVLD_SetCrc(GMDATA_ID_MISC);
#endif //CRC_LOADCHECK
}

//----------------------------------------------------------
/**
 * @brief	�O���Z�[�u�f�[�^�̏������ς݃t���O�擾
 * @param	misc		������ԕێ����[�N�ւ̃|�C���^
 * @retval	1:�������ς݁A0:����������Ă��Ȃ�
 */
//----------------------------------------------------------
u32 MISC_GetExtraInitFlag(const MISC * misc)
{
	return misc->extra_init_flag;
}

#ifdef PM_DEBUG //==============
//--------------------
//	�O���Z�[�u�f�[�^�̏������ς݃t���O�����Z�b�g����
//		���f�o�b�O�p
//--------------------
void MISC_ClearExtraInitFlag(MISC * misc)
{
	misc->extra_init_flag = 0;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MISC)
	SVLD_SetCrc(GMDATA_ID_MISC);
#endif //CRC_LOADCHECK
}
#endif	//PM_DEBUG	===============

//==============================================================================
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���C�ɓ���|�P�����̃Z�b�g
 *
 * @param   misc		
 * @param   monsno		�|�P�����ԍ�
 * @param   form_no		�t�H�����ԍ�
 * @param   egg_flag	�^�}�S�t���O
 */
//--------------------------------------------------------------
void MISC_SetFavoriteMonsno(MISC * misc, int monsno, int form_no, int egg_flag)
{
	misc->favorite_monsno = monsno;
	misc->favorite_form_no = form_no;
	misc->favorite_egg_flag = egg_flag;
	
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MISC)
	SVLD_SetCrc(GMDATA_ID_MISC);
#endif //CRC_LOADCHECK
}

//--------------------------------------------------------------
/**
 * @brief   ���C�ɓ���|�P�����擾
 *
 * @param   misc		
 * @param   monsno		�|�P�����ԍ�(���C�ɓ����ݒ肵�Ă��Ȃ��ꍇ��0)
 * @param   form_no		�t�H�����ԍ�
 * @param   egg_flag	�^�}�S�t���O
 */
//--------------------------------------------------------------
void MISC_GetFavoriteMonsno(const MISC * misc, int *monsno, int *form_no, int *egg_flag)
{
	*monsno = misc->favorite_monsno;
	*form_no = misc->favorite_form_no;
	*egg_flag = misc->favorite_egg_flag;
}

//--------------------------------------------------------------
/**
 * @brief   GDS���ȏЉ�b�Z�[�W���擾
 *
 * @param   misc		
 * @param   pms			�����
 */
//--------------------------------------------------------------
void MISC_GetGdsSelfIntroduction(const MISC *misc, PMS_DATA *pms)
{
	*pms = misc->gds_self_introduction;
}

//--------------------------------------------------------------
/**
 * @brief   GDS���ȏЉ�b�Z�[�W���Z�b�g����
 *
 * @param   misc		
 * @param   pms			�Z�b�g���郁�b�Z�[�W
 */
//--------------------------------------------------------------
void MISC_SetGdsSelfIntroduction(MISC *misc, const PMS_DATA *pms)
{
	misc->gds_self_introduction = *pms;
}

//--------------------------------------------------------------
/**
 * @brief	�o�g�����R�[�_�[�̐F�擾
 *
 * @param	misc	
 * @param	color	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void MISC_GetBattleRecoderColor( const MISC * misc, u8* color )
{
	*color = misc->battle_recoder_color;
}


//--------------------------------------------------------------
/**
 * @brief	�o�g�����R�[�_�[�̐F�ݒ�
 *
 * @param	misc	
 * @param	color	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void MISC_SetBattleRecoderColor( MISC * misc, u8 color )
{
	misc->battle_recoder_color = color;
}

//--------------------------------------------------------------
/**
 * @brief   �Z�[�u�f�[�^����O���f�[�^�̔F�؃L�[�A�t���O���擾����
 *
 * @param   sv				�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   exdata_id		�O���f�[�^�ԍ�(EXDATA_ID_???)
 * @param   key				�F�؃L�[�����
 * @param   old_key			1�O�̔F�؃L�[�����
 * @param   flag			�t���O�����
 */
//--------------------------------------------------------------
void MISC_ExtraSaveKeyGet(const MISC *misc, EXDATA_ID exdata_id, EX_CERTIFY_SAVE_KEY *key, EX_CERTIFY_SAVE_KEY *old_key, u8 *flag)
{
	*key = misc->ex_save_key.key[exdata_id - EXDATA_ID_FRONTIER];
	*old_key = misc->ex_save_key.old_key[exdata_id - EXDATA_ID_FRONTIER];
	*flag = misc->ex_save_key.flag[exdata_id - EXDATA_ID_FRONTIER];
}

//--------------------------------------------------------------
/**
 * @brief   �Z�[�u�f�[�^����O���f�[�^�̔F�؃L�[�A�t���O���Z�b�g����
 *
 * @param   misc		
 * @param   exdata_id	
 * @param   key			�F�؃L�[
 * @param   key			1�O�̔F�؃L�[
 * @param   flag		�t���O
 * @param   flag		1�O�̃t���O
 */
//--------------------------------------------------------------
void MISC_ExtraSaveKeySet(MISC *misc, EXDATA_ID exdata_id, EX_CERTIFY_SAVE_KEY key, EX_CERTIFY_SAVE_KEY old_key, u8 flag)
{
	misc->ex_save_key.key[exdata_id - EXDATA_ID_FRONTIER] = key;
	misc->ex_save_key.old_key[exdata_id - EXDATA_ID_FRONTIER] = old_key;
	misc->ex_save_key.flag[exdata_id - EXDATA_ID_FRONTIER] = flag;
}

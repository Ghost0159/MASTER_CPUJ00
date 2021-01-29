//==============================================================================
/**
 * @file	frontier_ex.c
 * @brief	�o�g���t�����e�B�A�O���f�[�^(�i�s�Ɋ֌W�Ȃ��L�^�̂�)
 * @author	matsuda
 * @date	2007.07.18(��)
 */
//==============================================================================
#include "common.h"
#include "savedata\savedata.h"
#include "system\gamedata.h"
#include "gflib\strbuf_family.h"
#include "poketool\pokeparty.h"
#include "savedata\pokeparty_local.h"
#include "savedata\mystatus_local.h"
#include "savedata\config.h"

#include "savedata/frontier_savedata.h"
#include "savedata/frontier_ex.h"


//==============================================================================
//	�\���̒�`
//==============================================================================
///�o�g���X�e�[�W�O���L�^�f�[�^
typedef struct{
	u16 renshou_single[MONSNO_MAX];		///<�V���O���A���L�^
	u16 renshou_double[MONSNO_MAX];		///<�_�u���A���L�^
	u16 renshou_multi[MONSNO_MAX];		///<���C�����X�A���L�^
//	u16 renshou_wifi[MONSNO_MAX];		///<Wifi�A���L�^

	u16 padding;	//4�o�C�g���E�I�t�Z�b�g
}STAGE_EX;

//----------------------------------------------------------
/**
 *	�o�g���t�����e�B�A�O���f�[�^�\����
 */
//----------------------------------------------------------
struct _FRONTIER_EX_SAVEDATA {
	//�a������ȊO�̊O���Z�[�u�͕K����ԍŏ���EX_SAVE_KEY�\���̂�z�u���Ă�������
	EX_CERTIFY_SAVE_KEY save_key;			///<�F�؃L�[

	STAGE_EX stage;		//�o�g���X�e�[�W�O���L�^�f�[�^
};


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static u16 FrontierEx_StageRenshou_Set(FRONTIER_EX_SAVEDATA *fes, int id, int monsno, u16 data);
static u16 FrontierEx_StageRenshou_SetIfLarge(SAVEDATA *sv, FRONTIER_EX_SAVEDATA *fes, int id, int monsno, u16 data);


//==============================================================================
//
//	
//
//==============================================================================

//------------------------------------------------------------------
/**
 * �Z�[�u�f�[�^�T�C�Y��Ԃ�
 *
 * @retval  int		
 */
//------------------------------------------------------------------
int FrontierEx_GetWorkSize( void )
{
	GF_ASSERT(sizeof(FRONTIER_EX_SAVEDATA) % 4 == 0);	//4�o�C�g���E�ɂȂ��Ă��邩�m�F
	return sizeof(FRONTIER_EX_SAVEDATA);
}

//------------------------------------------------------------------
/**
 * �f�[�^������
 *
 * @param   data		
 *
 */
//------------------------------------------------------------------
void FrontierEx_Init(FRONTIER_EX_SAVEDATA *work)
{
	MI_CpuClear8(work, sizeof(FRONTIER_EX_SAVEDATA));
	work->save_key = EX_CERTIFY_SAVE_KEY_NO_DATA;
}

//------------------------------------------------------------------
/**
 * �t�����e�B�A�O���f�[�^�̃��[�h
 *
 * @param	sv		�Z�[�u�f�[�^�\���̂ւ̃|�C���^
 * @param	heapID	�f�[�^�����[�h���郁�������m�ۂ��邽�߂̃q�[�vID
 * @param	result	���[�h���ʂ��i�[���郏�[�N
 *
 * @retval	�ǂݍ��񂾃t�����e�B�A�O���f�[�^�ւ̃|�C���^
 */
//------------------------------------------------------------------
FRONTIER_EX_SAVEDATA * FrontierEx_Load(SAVEDATA *sv,int heapID,LOAD_RESULT *result)
{
	FRONTIER_EX_SAVEDATA *fes;
	
	fes = SaveData_Extra_LoadFrontierEx(sv,heapID,result);
	return fes;
}

//------------------------------------------------------------------
/**
 * �t�����e�B�A�O���f�[�^�̃Z�[�u
 *
 * @param	sv		�Z�[�u�f�[�^�\���̂ւ̃|�C���^
 * @param	data	�Z�[�u�f�[�^�ւ̃|�C���^(CHECK_TAIL_DATA�\���̕����܂߂����̂ł���
 * 						�K�v������̂�FrontierEx_Load�֐��̖߂�l�Ŏ�ɓ��ꂽ
 *						�o�b�t�@�|�C���^��n���K�v������B
 *
 * @retval	�Z�[�u����
 */
//------------------------------------------------------------------
SAVE_RESULT FrontierEx_Save(SAVEDATA *sv, FRONTIER_EX_SAVEDATA *fes)
{
	SAVE_RESULT result;
	
	result = SaveData_Extra_SaveFrontierEx(sv, fes);
	result |= SaveData_Save(sv);	//�O���̌�͒ʏ�Z�[�u
	return result;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �|�P�������̃X�e�[�W�ő�A�����擾
 *
 * @param   fes		�t�����e�B�A�O���f�[�^�ւ̃|�C���^
 * @param   id			FREXID_STAGE_RENSHOU_???
 * @param   monsno		�|�P�����ԍ�
 *
 * @retval  �ő�A����
 */
//--------------------------------------------------------------
u16 FrontierEx_StageRenshou_Get(SAVEDATA *sv, FRONTIER_EX_SAVEDATA *fes, int id, int monsno)
{
	if(SaveData_GetExtraInitFlag(sv) == FALSE){
		return 0;	//�O���Z�[�u������������Ă��Ȃ��̂Ńf�[�^�����Ɣ��肵�A���0��Ԃ�
	}
	
	switch(id){
	case FREXID_STAGE_RENSHOU_SINGLE:
		return fes->stage.renshou_single[monsno];
	case FREXID_STAGE_RENSHOU_DOUBLE:
		return fes->stage.renshou_double[monsno];
	case FREXID_STAGE_RENSHOU_MULTI:
		return fes->stage.renshou_multi[monsno];
//	case FREXID_STAGE_RENSHOU_WIFI:
//		return fes->stage.renshou_wifi[monsno];
	}
	GF_ASSERT(0);
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�������̃X�e�[�W�ő�A�����Z�b�g
 *
 * @param   fes		�t�����e�B�A�O���f�[�^�ւ̃|�C���^
 * @param   id			FREXID_STAGE_RENSHOU_???
 * @param   monsno		�|�P�����ԍ�
 * @param   data		�Z�b�g����l
 *
 * @retval  �Z�b�g��̒l
 */
//--------------------------------------------------------------
static u16 FrontierEx_StageRenshou_Set(FRONTIER_EX_SAVEDATA *fes, int id, int monsno, u16 data)
{
	if(data > FRONTIER_RECORD_LIMIT){
		data = FRONTIER_RECORD_LIMIT;
	}
	
	switch(id){
	case FREXID_STAGE_RENSHOU_SINGLE:
		fes->stage.renshou_single[monsno] = data;
		break;
	case FREXID_STAGE_RENSHOU_DOUBLE:
		fes->stage.renshou_double[monsno] = data;
		break;
	case FREXID_STAGE_RENSHOU_MULTI:
		fes->stage.renshou_multi[monsno] = data;
		break;
//	case FREXID_STAGE_RENSHOU_WIFI:
//		fes->stage.renshou_wifi[monsno] = data;
//		break;
	default:
		GF_ASSERT(0);
		return 0;
	}
	
	return data;
}

//�f�o�b�N�p
u16 Debug_FrontierEx_StageRenshou_Set(FRONTIER_EX_SAVEDATA *fes, int id, int monsno, u16 data)
{
	return FrontierEx_StageRenshou_Set(fes, id, monsno, data);
}

//--------------------------------------------------------------
/**
 * @brief   �傫����΍ő�A�������X�V����
 *
 * @param   fes		�t�����e�B�A�O���f�[�^�ւ̃|�C���^
 * @param   id			FREXID_STAGE_RENSHOU_???
 * @param   monsno		�|�P�����ԍ�
 * @param   data		�Z�b�g(��r)����l
 *
 * @retval  ���ʂ̒l
 */
//--------------------------------------------------------------
static u16 FrontierEx_StageRenshou_SetIfLarge(SAVEDATA *sv, FRONTIER_EX_SAVEDATA *fes, int id, int monsno, u16 data)
{
	u16 now;
	
	if(SaveData_GetExtraInitFlag(sv) == FALSE){
		return 0;	//�O���Z�[�u������������Ă��Ȃ��̂Ńf�[�^�����Ɣ��肵�A���0��Ԃ�
	}
	
	now = FrontierEx_StageRenshou_Get(sv, fes, id, monsno);
	OS_Printf( "���݂�now = %d\n", now );
	if(now < data){
		return FrontierEx_StageRenshou_Set(fes, id, monsno, data);
	}
	else{
		if(now > FRONTIER_RECORD_LIMIT){	//���ݒl�����~�b�g�I�[�o�[���Ă������̒�������
			return FrontierEx_StageRenshou_Set(fes, id, monsno, FRONTIER_RECORD_LIMIT);
		}
		return now;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�W�̊O���A���L�^���������݁A�Z�[�u���s��
 *
 * @param   sv					�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   record_id			FRID_STAGE_???
 * @param   record_monsno_id	FRID_STAGE_SINGLE_MONSNO | DOUBLE | MULTI
 * @param   friend_no			�F�B�蒠�ԍ�(�F�B�f�[�^�łȂ��ꍇ��FRONTIER_RECORD_NOT_FRIEND�w��)
 * @param   id					FREXID_STAGE_RENSHOU_???
 * @param   heap_id				�e���|�����Ƃ��Ďg�p����q�[�vID
 * @param   load_result			�O���Z�[�u�̃��[�h����
 * @param   save_result			�O���Z�[�u�̃Z�[�u����
 *
 * @retval  TRUE:�O���Z�[�u���ʏ�Z�[�u���s
 * @retval  FALSE:�O�����ʏ���Z�[�u�͂��Ȃ�����
 *
 * �O���Z�[�u�t�@�C�������[�h�@���@�풓�ɔz�u����Ă���t�����e�B�A���R�[�h����Z�b�g����l���擾
 * �@���@�O���Z�[�u�̒l�Ɣ�r���A�傫����΃Z�b�g�@���@�O���Z�[�u���s
 * �ȏ�̏������s���܂�
 */
//--------------------------------------------------------------
BOOL FrontierEx_StageRenshou_RenshouCopyExtra(SAVEDATA *sv, int record_id, int record_monsno_id, 
	int friend_no, int save_id, int heap_id, LOAD_RESULT *load_result, SAVE_RESULT *save_result)
{
	FRONTIER_SAVEWORK *fsv;
	FRONTIER_EX_SAVEDATA * fes;
	u16 ret_num, now, before;
	int monsno;
	BOOL ret = FALSE;
	
	GF_ASSERT(record_id >= FRID_STAGE_SINGLE_RENSHOU && record_id <= FRID_STAGE_MULTI_COMM_MONSNO);
	GF_ASSERT(record_monsno_id == FRID_STAGE_SINGLE_MONSNO || record_monsno_id == FRID_STAGE_DOUBLE_MONSNO || record_monsno_id == FRID_STAGE_MULTI_COMM_MONSNO);
	
	*load_result = LOAD_RESULT_OK;
	*save_result = SAVE_RESULT_OK;
	
	if(friend_no != FRONTIER_RECORD_NOT_FRIEND){
		return ret;//0;	//Wifi�̎��͖���
	}
	
	if(SaveData_GetExtraInitFlag(sv) == FALSE){
		return ret;//0;	//�O���Z�[�u������������Ă��Ȃ��̂Ńf�[�^�����Ɣ���
	}

	fsv = SaveData_GetFrontier(sv);
	now = FrontierRecord_Get(fsv, record_id, friend_no);
	monsno = FrontierRecord_Get(fsv, record_monsno_id, friend_no);
	OS_Printf( "now = %d\n", now );
	OS_Printf( "monsno = %d\n", monsno );
	
	fes = FrontierEx_Load(sv, heap_id, load_result);
	if(*load_result != LOAD_RESULT_OK){
		before = 0;	//�Z�[�u�j��or�L�[�ƈ�v���Ȃ��A�ꍇ�͐V�K�f�[�^�Ɖ��߂��A�J�E���^0�Ƃ���
	}
	else{
		before = FrontierEx_StageRenshou_Get(sv, fes, save_id, monsno);
	}
	
	ret_num = FrontierEx_StageRenshou_SetIfLarge(sv, fes, save_id, monsno, now);
	OS_Printf( "ret_num = %d\n", ret_num );
	OS_Printf( "now = %d\n", now );

	//07.12.27�ύX
	if(now != before){
	//if(now != ret_num){
		*save_result = FrontierEx_Save(sv, fes);	//�l���X�V���ꂽ���̂݃Z�[�u
		ret = TRUE;
		OS_Printf( "�l���X�V���ꂽ�I = %d\n", now );
	}
	
	if( fes != NULL ){
		sys_FreeMemoryEz(fes);
	}

	return ret;//ret_num;
}

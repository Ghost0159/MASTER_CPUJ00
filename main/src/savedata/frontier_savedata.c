//==============================================================================
/**
 * @file	frontier_savedata.c
 * @brief	�o�g���t�����e�B�A�֘A�̃Z�[�u�f�[�^
 * @author	matsuda
 * @date	2007.04.26(��)
 */
//==============================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/savedata.h"

#include "system/gamedata.h"
#include "system/pms_data.h"
#include "system/buflen.h"
#include "system/msgdata.h"
#include "battle/battle_common.h"
#include "battle/b_tower_data.h"

#include "savedata/frontier_savedata.h"
#include "frontier_savedata_local.h"

#include "b_tower_local.h"

#include "savedata/b_tower.h"
#include "savedata/factory_savedata.h"
#include "savedata/stage_savedata.h"
#include "savedata/castle_savedata.h"
#include "savedata/roulette_savedata.h"
#include "field/b_tower_scr_def.h"
#include "libdpw/dpw_bt.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_btower_app.h"



//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�@�Z�[�u�f�[�^�u���b�N�T�C�Y���擾
 *
 * @retval  �Z�[�u�f�[�^�u���b�N�T�C�Y
 */
//--------------------------------------------------------------
int FrontierData_GetWorkSize(void)
{
	return sizeof(FRONTIER_SAVEWORK);
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�@�Z�[�u�f�[�^�u���b�N������
 *
 * @param   save		
 */
//--------------------------------------------------------------
void FrontierData_Init(FRONTIER_SAVEWORK* save)
{
	//���ʃ��[�N������
//	TowerPlayData_Clear(&save->play);
	MI_CpuClear8(save, sizeof(FRONTIER_SAVEWORK));
	
	//�o�g���^���[������
	TowerScoreData_Clear(&save->tower.score);
	TowerPlayerMsg_Clear(&save->tower.player_msg);
	TowerWifiData_Clear(&save->tower.wifi);
	
	//�t�@�N�g���[������
	FACTORYSCORE_Init(&save->factory.score);	

	//�X�e�[�W������
	STAGESCORE_Init(&save->stage.score);	

	//�L���b�X��������
	CASTLESCORE_Init(&save->castle.score);	

	//���[���b�g������
	ROULETTESCORE_Init(&save->roulette.score);	

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�̃Z�[�u�f�[�^�|�C���^�擾
 *
 * @param   sv		
 *
 * @retval  �t�����e�B�A�Z�[�u�f�[�^�|�C���^
 */
//--------------------------------------------------------------
FRONTIER_SAVEWORK * SaveData_GetFrontier(SAVEDATA * sv)
{
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_CheckCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return SaveData_Get(sv, GMDATA_ID_FRONTIER);
}



//==============================================================================
//	�t�����e�B�A���R�[�h
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A���R�[�h�f�[�^�擾
 *
 * @param   save			
 * @param   id				���o���f�[�^ID(FRID_???)
 * @param   friend_no		�F�B�蒠�ԍ�(�F�B�f�[�^�łȂ��ꍇ��FRONTIER_RECORD_NOT_FRIEND�w��)
 *
 * @retval  ���R�[�h�f�[�^
 */
//--------------------------------------------------------------
u16 FrontierRecord_Get(FRONTIER_SAVEWORK* save, int id, int friend_no)
{
	FRONTIER_RECORD_SAVEWORK *frs = &save->frontier_record;
	
	if(id < FRID_MAX){
		//GF_ASSERT(friend_no == FRONTIER_RECORD_NOT_FRIEND);
		if(id >= FRID_TOWER_MULTI_WIFI_CLEAR_BIT){
			int access_id, shift_num;
			
			if(friend_no < 16){
				access_id = id;
				shift_num = friend_no;
			}
			else{
				access_id = id + 1;
				shift_num = friend_no - 16;
			}
			return (frs->record[access_id] >> shift_num) & 1;
		}
		return frs->record[id];
	}
	else{	//�F�B�蒠���̃f�[�^
		//GF_ASSERT(friend_no != FRONTIER_RECORD_NOT_FRIEND);
		
		if( friend_no == -1 ){
			OS_Printf( "friend_no = -1 �܂��ڑ����Ă��Ȃ��̂ŗF�B�ԍ����擾�o���܂���I\n" );
			GF_ASSERT(0);
			return 0;
		}
		return frs->wififriend_record[friend_no][id - FRID_MAX];
	}
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A���R�[�h�f�[�^�Z�b�g
 *
 * @param   save		
 * @param   id			�f�[�^ID(FRID_???)
 * @param   friend_no	�F�B�蒠�ԍ�(�F�B�f�[�^�łȂ��ꍇ��FRONTIER_RECORD_NOT_FRIEND�w��)
 * @param   data		�Z�b�g����f�[�^
 *
 * @retval	�Z�b�g��̒l
 */
//--------------------------------------------------------------
u16 FrontierRecord_Set(FRONTIER_SAVEWORK* save, int id, int friend_no, u16 data)
{
	FRONTIER_RECORD_SAVEWORK *frs = &save->frontier_record;

	if(data > FRONTIER_RECORD_LIMIT){
		data = FRONTIER_RECORD_LIMIT;
	}
	
	if(id < FRID_MAX){
		if(id >= FRID_TOWER_MULTI_WIFI_CLEAR_BIT){
			int access_id, shift_num;

			GF_ASSERT(friend_no != FRONTIER_RECORD_NOT_FRIEND);
			
			if(friend_no < 16){
				access_id = id;
				shift_num = friend_no;
			}
			else{
				access_id = id + 1;
				shift_num = friend_no - 16;
			}
			
			if(data == 0){
				frs->record[access_id] &= 0xffff ^ (1 << shift_num);
			}
			else{
				frs->record[access_id] |= 1 << shift_num;
			}
		}
		else{
			GF_ASSERT(friend_no == FRONTIER_RECORD_NOT_FRIEND);
			frs->record[id] = data;
		}
	}
	else{	//�F�B�蒠���̃f�[�^
		GF_ASSERT(friend_no != FRONTIER_RECORD_NOT_FRIEND);
		frs->wififriend_record[friend_no][id - FRID_MAX] = data;
	}
	
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return data;
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A���R�[�h�f�[�^������(�F�B�蒠���Ɏ��f�[�^�̂ݍ폜)
 *
 * @param   save		
 */
//--------------------------------------------------------------
void FrontierRecord_DataCrear(FRONTIER_SAVEWORK *save)
{
	int i;
	FRONTIER_RECORD_SAVEWORK *frs = &save->frontier_record;
	
	MI_CpuClear8(frs->wififriend_record, (sizeof(u16)*(FRID_WIFI_MAX-FRID_MAX))*WIFILIST_FRIEND_MAX);
	for(i = FRID_TOWER_MULTI_WIFI_CLEAR_BIT; i <= FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT_SUB; i++){
		frs->record[i] = 0;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A���R�[�h�f�[�^������
 *
 * @param   save			
 * @param   friend_no		�F�B�蒠�ԍ�
 */
//--------------------------------------------------------------
void FrontierRecord_ResetData(FRONTIER_SAVEWORK* save, int friend_no)
{
	FRONTIER_RECORD_SAVEWORK *frs = &save->frontier_record;
	int i, s;
	u16 data;
	
	GF_ASSERT(friend_no != FRONTIER_RECORD_NOT_FRIEND);
	
	for(i = friend_no; i < (WIFILIST_FRIEND_MAX-1); i++){
		MI_CpuCopy8(frs->wififriend_record[i+1], frs->wififriend_record[i], sizeof(u16)*(FRID_WIFI_MAX - FRID_MAX));
		
		for(s=FRID_TOWER_MULTI_WIFI_CLEAR_BIT; s < FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT_SUB; s += 2){
			data = FrontierRecord_Get(save, s, i+1);
			FrontierRecord_Set(save, s, i, data);
		}
	}
	i = WIFILIST_FRIEND_MAX-1;
	MI_CpuClear8(frs->wififriend_record[i], sizeof(u16)*(FRID_WIFI_MAX - FRID_MAX));

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A���R�[�h�f�[�^��A�̐l�̃f�[�^��B�̐l�Ƀ}�[�W����
 *
 * @param   save			
 * @param   delNo			
 * @param   friend_no		�F�B�蒠�ԍ�
 */
//--------------------------------------------------------------
void FrontierRecord_DataMarge(FRONTIER_SAVEWORK* save, int delNo, int friend_no)
{
	FRONTIER_RECORD_SAVEWORK *frs = &save->frontier_record;
	
	//���C�����X�Ńf�[�^�����Z����鎖�͂Ȃ��̂ŁA�}�[�W��������ǂ����0�B���̂���delNo��
	//�������O�̂��߃N���A���������邾���ɂ���B
	MI_CpuClear8(frs->wififriend_record[delNo], sizeof(u16)*(FRID_WIFI_MAX - FRID_MAX));

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A���R�[�h�f�[�^ ���Z
 *
 * @param   save		
 * @param   id			���o���f�[�^ID(FRID_???)
 * @param   friend_no	�F�B�蒠�ԍ�(�F�B�f�[�^�łȂ��ꍇ��FRONTIER_RECORD_NOT_FRIEND�w��)
 * @param   add_value	���Z�l
 * 
 * @retval	�X�V���ꂽ�l
 */
//--------------------------------------------------------------
u16 FrontierRecord_Add(FRONTIER_SAVEWORK* save, int id, int friend_no, int add_value)
{
	FRONTIER_RECORD_SAVEWORK *frs = &save->frontier_record;
	u16 data;
	
	data = FrontierRecord_Get(save, id, friend_no);
	data += add_value;
	return FrontierRecord_Set(save, id, friend_no, data);
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A���R�[�h�f�[�^ ���Z
 *
 * @param   save		
 * @param   id			���o���f�[�^ID(FRID_???)
 * @param   friend_no	�F�B�蒠�ԍ�(�F�B�f�[�^�łȂ��ꍇ��FRONTIER_RECORD_NOT_FRIEND�w��)
 * @param   sub_value	���Z�l
 * 
 * @retval	�X�V���ꂽ�l
 */
//--------------------------------------------------------------
u16 FrontierRecord_Sub(FRONTIER_SAVEWORK* save, int id, int friend_no, int sub_value)
{
	FRONTIER_RECORD_SAVEWORK *frs = &save->frontier_record;
	int data;
	
	data = FrontierRecord_Get(save, id, friend_no);
	data -= sub_value;
	if(data < 0){
		data = 0;
	}
	return FrontierRecord_Set(save, id, friend_no, data);
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A���R�[�h�f�[�^ �C���N�������g
 *
 * @param   save		
 * @param   id			���o���f�[�^ID(FRID_???)
 * @param   friend_no	�F�B�蒠�ԍ�(�F�B�f�[�^�łȂ��ꍇ��FRONTIER_RECORD_NOT_FRIEND�w��)
 * 
 * @retval	�X�V���ꂽ�l
 */
//--------------------------------------------------------------
u16 FrontierRecord_Inc(FRONTIER_SAVEWORK* save, int id, int friend_no)
{
	return FrontierRecord_Add(save, id, friend_no, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �傫����΃��R�[�h���X�V����
 *
 * @param   save		
 * @param   id			�f�[�^ID(FRID_???)
 * @param   friend_no	�F�B�蒠�ԍ�(�F�B�f�[�^�łȂ��ꍇ��FRONTIER_RECORD_NOT_FRIEND�w��)
 * @param   data		�Z�b�g����f�[�^
 *
 * @retval  ���ʂ̒l
 */
//--------------------------------------------------------------
u16 FrontierRecord_SetIfLarge(FRONTIER_SAVEWORK *save, int id, int friend_no, u16 data)
{
	u16 now;
	
	now = FrontierRecord_Get(save, id, friend_no);

	if(now < data){
		return FrontierRecord_Set(save, id, friend_no, data);
	} else {
		if(now > FRONTIER_RECORD_LIMIT){	//���ݒl�����~�b�g�I�[�o�[���Ă������̒�������
			return FrontierRecord_Set(save, id, friend_no, FRONTIER_RECORD_LIMIT);
		}
		return now;
	}
}


//==============================================================================
/**
 * @file	battle_rec_ov.c
 * @brief	�I�[�o�[���Cbattle_recorder�ł̂ݎg�p����^��֘A�c�[����
 * @author	matsuda
 * @date	2008.05.15(��)
 *
 * �{���Ȃ�battle_rec.c�Ɋ܂߂���̂����A�풓�������̊֌W��A�o�g�����R�[�_�[�ł���
 * �g�p���Ȃ��悤�Ȃ��̂́A�����Ɉړ�
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

#include "gds_local_common.h"
#include "savedata\battle_rec.h"
#include "itemtool/itemsym.h"
#include "battle/wazano_def.h"
#include "gds_profile_types.h"
#include "savedata/gds_profile.h"
#include "system/snd_perap.h"

#include "application/battle_recorder/gds_battle_rec.h"
#include "savedata/misc.h"
#include "savedata/regulation_data.h"

#include "battle_rec_local.h"
#include "gflib/strcode.h"
#include "gflib\msg_print.h"
#include "system\fontproc.h"
#include "system\msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_battle_rec.h"



//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void ErrorNameSet(STRBUF *dest_str, int heap_id);

//==============================================================================
//	
//==============================================================================
extern BATTLE_REC_SAVEDATA * brs;



//--------------------------------------------------------------
/**
 * @brief   BATTLE_PARAM�̃g���[�i�[���A�|�P�������𒲂ׂĕs���ȕ�����̏ꍇ�̓P�A����������
 *
 * @param   src			�^��Z�[�u�f�[�^�ւ̃|�C���^
 * @param   bp			�o�g���p�����[�^�ւ̃|�C���^
 * @param   heap_id		�e���|�����Ŏg�p����q�[�vID
 */
//--------------------------------------------------------------
void BattleRecTool_ErrorStrCheck(BATTLE_REC_SAVEDATA *src, BATTLE_PARAM *bp, int heap_id)
{
	BATTLE_REC_WORK *rec = &src->rec;
	BATTLE_REC_HEADER *head = &src->head;
	int client_max, temoti_max, client, temoti;
	STRBUF *check_str, *temp_str;
	STRCODE *check_strcode;
	int strcode_size = 64;	//�K���ɑ傫��
	int s, no, poke_count;
	POKEMON_PARAM *pp;
	
	BattleRec_ClientTemotiGet(head->mode, &client_max, &temoti_max);
	check_str = STRBUF_Create(strcode_size, heap_id);
	temp_str = STRBUF_Create(strcode_size, heap_id);
	check_strcode = sys_AllocMemory(heap_id, sizeof(STRCODE) * strcode_size);
	
	//�g���[�i�[���̕�����`�F�b�N
	for(client = 0; client < client_max; client++){
		//�I�[�����邩
		for(s = BUFLEN_PERSON_NAME - 1; s > -1; s--){
			if(bp->trainer_data[client].name[s] == EOM_){
				break;
			}
		}
		if(s == -1){	//�����R�[�h���Ȃ�
			OS_TPrintf("�g���[�i�[�� %d �ɏI�[�R�[�h���Ȃ�\n", client);
			ErrorNameSet(temp_str, heap_id);
			STRBUF_GetStringCode(temp_str, bp->trainer_data[client].name, BUFLEN_PERSON_NAME);
			continue;
		}
		
		//�s�������R�[�h�`�F�b�N
		STRBUF_Clear(check_str);
		STRBUF_SetStringCode(check_str, bp->trainer_data[client].name);
		if(FontProc_ErrorStrCheck(FONT_SYSTEM, check_str, temp_str) == FALSE){
			OS_TPrintf("�g���[�i�[�� %d �ɕs���ȕ����R�[�h�������Ă���\n", client);
			ErrorNameSet(temp_str, heap_id);
			STRBUF_GetStringCode(temp_str, bp->trainer_data[client].name, BUFLEN_PERSON_NAME);
			continue;
		}
	}
	
	//�|�P�������̕�����`�F�b�N
	for(client = 0; client < client_max; client++){
		poke_count = PokeParty_GetPokeCount(bp->poke_party[client]);
		for(no = 0; no < poke_count; no++){
			pp = PokeParty_GetMemberPointer(bp->poke_party[client], no);
			if(PokeParaGet(pp, ID_PARA_poke_exist, NULL) == FALSE){
				break;
			}
			//ID_PARA_nickname�Ŏ擾����ΕK���I�[�����Ă���̂ŏI�[�`�F�b�N�͂��Ȃ�
			//�퓬�ł�ID_PARA_nickname�����g�p���Ă��Ȃ��̂ŁB
			MI_CpuClear16(check_strcode, sizeof(STRCODE) * strcode_size);
			PokeParaGet(pp, ID_PARA_nickname, check_strcode);
			//�s�������R�[�h�`�F�b�N
			STRBUF_Clear(check_str);
			STRBUF_SetStringCode(check_str, check_strcode);
			if(FontProc_ErrorStrCheck(FONT_SYSTEM, check_str, temp_str) == FALSE){
				OS_TPrintf("�|�P������ client:%d no:%d �ɕs���ȕ����R�[�h�������Ă���\n", client, no);
				//�f�t�H���g�����ăZ�b�g
				PokeParaPut(pp, ID_PARA_default_name, NULL);
				continue;
			}
		}
	}
	
	STRBUF_Delete(check_str);
	STRBUF_Delete(temp_str);
	sys_FreeMemoryEz(check_strcode);
}

//--------------------------------------------------------------
/**
 * @brief   �G���[�����������ꍇ�ɕ\������㗝�����o�b�t�@�ɃZ�b�g
 *
 * @param   dest_str		�����
 * @param   heap_id			�e���|�����Ŏg�p����q�[�v
 */
//--------------------------------------------------------------
static void ErrorNameSet(STRBUF *dest_str, int heap_id)
{
	MSGDATA_MANAGER * msgman;

	STRBUF_Clear(dest_str);
	
	msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_battle_rec_dat, heap_id );
	MSGMAN_GetString( msgman, msg_error_trainer_name, dest_str );
	MSGMAN_Delete( msgman );
}


//==============================================================================
//	
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   GDS�Ŏ�M�����f�[�^���Z�[�u����
 *
 * @param   sv				�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   recv_data		GT_BATTLE_REC_RECV�\����
 * @param   num				LOADDATA_MYREC�ALOADDATA_DOWNLOAD0�ALOADDATA_DOWNLOAD1�c
 * @param   secure		�����ς݃t���O(TRUE:�����ς݂ɂ��ăZ�[�u����B
 * 						FALSE:�t���O���삵�Ȃ�(�T�[�o�[��secure�𗧂ĂĂ���Ȃ痧�����܂܂ɂȂ�)
 * @param   work0		�Z�[�u�i�s�𐧌䂷�郏�[�N�ւ̃|�C���^(�ŏ���0�N���A������ԂŌĂ�ŉ�����)
 * @param   work1		�Z�[�u�i�s�𐧌䂷�郏�[�N�ւ̃|�C���^(�ŏ���0�N���A������ԂŌĂ�ŉ�����)
 *
 * @retval  SAVE_RESULT_CONTINUE	�Z�[�u�����p����
 * @retval  SAVE_RESULT_LAST		�Z�[�u�����p�����A�Ō�̈�O
 * @retval  SAVE_RESULT_OK			�Z�[�u����I��
 * @retval  SAVE_RESULT_NG			�Z�[�u���s�I��
 *
 * GDS�Ŏ�M�����f�[�^��brs�ɃR�s�[�������_(BattleRec_DataSet)�ŕ�������Ă���̂ŁA
 * �����ňÍ������Ă���Z�[�u���s��
 */
//--------------------------------------------------------------
SAVE_RESULT BattleRec_GDS_RecvData_Save(SAVEDATA *sv, int num, u8 secure, u16 *work0, u16 *work1)
{
	SAVE_RESULT result;
	
	switch(*work0){
	case 0:
		GF_ASSERT(brs);
		
		if(secure == TRUE){
			brs->head.secure = secure;
			//secure�t���O���X�V���ꂽ�̂ŁA�ēxCRC����蒼��
			brs->head.magic_key = REC_OCC_MAGIC_KEY;
			brs->head.crc.crc16ccitt_hash = SaveData_CalcCRC(sv, &brs->head, 
				sizeof(BATTLE_REC_HEADER) - GDS_CRC_SIZE - DATANUMBER_SIZE);
		}
		
		//CRC���L�[�ɂ��ĈÍ���
		//��GDS�Ŏ�M�����f�[�^��brs�ɃR�s�[�������_(BattleRec_DataSet)�ŕ�������Ă���̂ŁB
		BattleRec_Coded(&brs->rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE, 
			brs->rec.crc.crc16ccitt_hash + ((brs->rec.crc.crc16ccitt_hash ^ 0xffff) << 16));
		(*work0)++;
		break;
	case 1:
		result = Local_BattleRecSave(sv, brs, num, work1);
		return result;
	}
	return SAVE_RESULT_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   GDS�ő��M����ׂɁA�f�[�^�ɕϊ��������s��
 *
 * @param   sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * ���̊֐��g�p���brs�́A���̂܂܍Đ�������ۑ����Ă͂����Ȃ��`�ɂȂ��Ă���̂ŁA
 * GDS�ő��M���I�������j�����邱�ƁI
 * �A���ABattleRec_GDS_MySendData_DataNumberSetSave�֐����g�p����ꍇ��OK�B
 */
//--------------------------------------------------------------
void BattleRec_GDS_SendData_Conv(SAVEDATA *sv)
{
	GF_ASSERT(brs);
	
	//��brs�ɓW�J����Ă���f�[�^�́A�{�̂�����������Ă���̂ő��M�O�ɍēx�Í������s��
	BattleRec_Coded(&brs->rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE, 
		brs->rec.crc.crc16ccitt_hash + ((brs->rec.crc.crc16ccitt_hash ^ 0xffff) << 16));
}

//--------------------------------------------------------------
/**
 * @brief   GDS�ő��M����brs�ɑ΂��āA�f�[�^�i���o�[���Z�b�g���Z�[�u���s��
 *
 * @param   sv				�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   data_number		�f�[�^�i���o�[
 * @param   work		�Z�[�u�i�s�𐧌䂷�郏�[�N�ւ̃|�C���^(�ŏ���0�N���A������ԂŌĂ�ŉ�����)
 * @param   work		�Z�[�u�i�s�𐧌䂷�郏�[�N�ւ̃|�C���^(�ŏ���0�N���A������ԂŌĂ�ŉ�����)
 * 
 * @retval	SAVE_RESULT_OK		�Z�[�u����
 * @retval	SAVE_RESULT_NG		�Z�[�u���s
 */
//--------------------------------------------------------------
SAVE_RESULT BattleRec_GDS_MySendData_DataNumberSetSave(SAVEDATA *sv, u64 data_number, u16 *work0, u16 *work1)
{
	SAVE_RESULT result;

	switch(*work0){
	case 0:
		GF_ASSERT(brs);
		
		//�f�[�^�i���o�[�͌��XCRC�̑Ώۂ���͂���Ă���̂ŁACRC���č쐬����K�v�͂Ȃ�
		brs->head.data_number = data_number;
		(*work0)++;
		break;
	case 1:
		//���M���ɍs���Ă���BattleRec_GDS_SendData_Conv�֐��Ŋ��ɈÍ������Ă���͂��Ȃ̂�
		//���̂܂܃Z�[�u���s��
		result = Local_BattleRecSave(sv, brs, LOADDATA_MYREC, work1);
		return result;
	}
	return SAVE_RESULT_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g�����R�[�_�[(�I�t���C��)�Ńr�f�I��������Abrs�ɓǂݍ���ł���o�g���r�f�I��
 * 			�����ς݃t���O���Z�b�g���ăZ�[�u����(�ꊇ�Z�[�u)
 * 			���ʏ�Z�[�u����
 *
 * @param   sv		�Z�[�u�f�[�^�ւ̃|�C���^
 * @param	num		�Z�[�u����f�[�^�i���o�[�iLOADDATA_MYREC�ALOADDATA_DOWNLOAD0�ALOADDATA_DOWNLOAD1�c�j
 *
 * @retval  SAVE_RESULT_???
 *
 * �����Ŗ{�̂̈Í����������s���ׁA���̊֐��Ȍ��brs�͉�����Ă��������B
 */
//--------------------------------------------------------------
SAVE_RESULT BattleRec_SecureSetSave(SAVEDATA *sv, int num)
{
	SAVE_RESULT result;

	GF_ASSERT(brs != NULL);
	
	brs->head.secure = TRUE;
	//secure�t���O���X�V���ꂽ�̂ŁA�ēxCRC����蒼��
	brs->head.magic_key = REC_OCC_MAGIC_KEY;
	brs->head.crc.crc16ccitt_hash = SaveData_CalcCRC(sv, &brs->head, 
		sizeof(BATTLE_REC_HEADER) - GDS_CRC_SIZE - DATANUMBER_SIZE);

	//CRC���L�[�ɂ��ĈÍ���
	BattleRec_Coded(&brs->rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE, 
		brs->rec.crc.crc16ccitt_hash + ((brs->rec.crc.crc16ccitt_hash ^ 0xffff) << 16));
	
	sys_SoftResetNG(SOFTRESET_TYPE_VIDEO);
	result = SaveData_Extra_SaveBattleRecData(sv, brs, num);
	if(result == SAVE_RESULT_OK){
		result = SaveData_Save(sv);
	}
	sys_SoftResetOK(SOFTRESET_TYPE_VIDEO);
	return result;
}



#ifdef PM_DEBUG
//--------------------------------------------------------------
/**
 * @brief   �ǂݍ��ݍς݂�brs�ɑ΂���secure�t���O���Z�b�g����
 *
 * @param   sv		
 */
//--------------------------------------------------------------
void DEBUG_BattleRec_SecureFlagSet(SAVEDATA *sv)
{
	GF_ASSERT(brs != NULL);
	
	brs->head.secure = TRUE;
	//secure�t���O���X�V���ꂽ�̂ŁA�ēxCRC����蒼��
	brs->head.magic_key = REC_OCC_MAGIC_KEY;
	brs->head.crc.crc16ccitt_hash = SaveData_CalcCRC(sv, &brs->head, 
		sizeof(BATTLE_REC_HEADER) - GDS_CRC_SIZE - DATANUMBER_SIZE);
}
#endif	//PM_DEBUG

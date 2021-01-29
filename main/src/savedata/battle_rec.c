//==============================================================================
/**
 * @file	battle_rec.c
 * @brief	�퓬�^��Z�[�u�f�[�^
 * @author	matsuda
 * @date	2007.03.06(��)
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


//==============================================================================
//	
//==============================================================================
BATTLE_REC_SAVEDATA * brs=NULL;

//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
//static	void BattleRec_BattleParamCreate(BATTLE_PARAM *bp,SAVEDATA *sv);
static void PokeParty_to_RecPokeParty(const POKEPARTY *party, REC_POKEPARTY *rec_party);
static void RecPokeParty_to_PokeParty(REC_POKEPARTY *rec_party, POKEPARTY *party);
static BOOL BattleRec_DataInitializeCheck(SAVEDATA *sv, BATTLE_REC_SAVEDATA *src);
static	BOOL BattleRecordCheckData(SAVEDATA *sv, const BATTLE_REC_SAVEDATA * src);
static	void	BattleRec_Decoded(void *data,u32 size,u32 code);
static void RecHeaderCreate(SAVEDATA *sv, BATTLE_REC_HEADER *head, const BATTLE_REC_WORK *rec, 
	int rec_mode, int counter);


//------------------------------------------------------------------
/**
 * �Z�[�u�f�[�^�T�C�Y��Ԃ�
 *
 * @retval  int		
 */
//------------------------------------------------------------------
int BattleRec_GetWorkSize( void )
{
#if 0
	OS_TPrintf("pokeparty_size = %d\n", sizeof(POKEPARTY) * 4);
	OS_TPrintf("mystatus_size = %d\n", sizeof(MYSTATUS) * 4);
	OS_TPrintf("config_size = %d\n", sizeof(CONFIG));
	
	OS_TPrintf("poke_paso = %d\n", sizeof(POKEMON_PASO_PARAM));
	OS_TPrintf("poke_calc = %d\n", sizeof(POKEMON_CALC_PARAM));
	
	OS_TPrintf("mail_data = %d\n", sizeof(_MAIL_DATA));

	OS_TPrintf("REC_POKEPARTY = %d\n", sizeof(REC_POKEPARTY));
	OS_TPrintf("REC_POKEPARA = %d\n", sizeof(REC_POKEPARA));
	OS_TPrintf("RECORD_PARAM = %d\n", sizeof(RECORD_PARAM));

	OS_TPrintf("TRAINER_DATA = %d\n", sizeof(TRAINER_DATA));
	OS_TPrintf("REC_BATTLE_PARAM = %d\n", sizeof(REC_BATTLE_PARAM));
	
	OS_TPrintf("BATTLE_REC_WORK = %d\n", sizeof(BATTLE_REC_WORK));
	OS_TPrintf("BATTLE_REC_HEADER = %d\n", sizeof(BATTLE_REC_HEADER));
	OS_TPrintf("_BATTLE_REC_SAVEDATA = %d\n", sizeof(struct _BATTLE_REC_SAVEDATA));

	OS_TPrintf("GDS_PROFILE = %d\n", sizeof(GDS_PROFILE));
#endif

	//�Z�N�^�[2�y�[�W�������T�C�Y���傫���Ȃ�����ExtraSaveDataTable�̊e�^��f�[�^��
	//�y�[�W�I�t�Z�b�g��+3�P�ʂɒ���
	GF_ASSERT(sizeof(BATTLE_REC_SAVEDATA) < SAVE_SECTOR_SIZE * 2);
	
	return sizeof(BATTLE_REC_SAVEDATA);
}

//--------------------------------------------------------------
/**
 * @brief   ���[�N������
 *
 * @param   rec		
 */
//--------------------------------------------------------------
void BattleRec_WorkInit(BATTLE_REC_SAVEDATA *rec)
{
	MI_CpuClear32( rec, sizeof(BATTLE_REC_SAVEDATA) );
	rec->save_key = EX_CERTIFY_SAVE_KEY_NO_DATA;
}

//------------------------------------------------------------------
/**
 * �f�[�^������
 *
 * @param   heapID	�������m�ۂ��邽�߂̃q�[�vID		
 *
 */
//------------------------------------------------------------------
void BattleRec_Init(SAVEDATA *sv,int heapID,LOAD_RESULT *result)
{
	if(brs != NULL){
		sys_FreeMemoryEz(brs);
		brs = NULL;
	}
	//���[�N���m�ۂ���ׂ�����LOAD�Ȃ̂ŁALOADDATA_MYREC�Œ�
	brs=SaveData_Extra_LoadBattleRecData(sv,heapID,result,LOADDATA_MYREC);
	BattleRec_WorkInit(brs);
}

//------------------------------------------------------------------
/**
 * �ΐ�^��f�[�^�̔j��
 */
//------------------------------------------------------------------
void BattleRec_Exit(void)
{
	GF_ASSERT(brs);
	sys_FreeMemoryEz(brs);
	brs=NULL;
}

//------------------------------------------------------------------
/**
 * �ΐ�^��f�[�^�̃��������m�ۂ��Ă��邩�`�F�b�N
 *
 * @retval	TRUE:���݂���@FALSE:���݂��Ȃ�
 */
//------------------------------------------------------------------
BOOL BattleRec_DataExistCheck(void)
{
	return (brs!=NULL);
}

//--------------------------------------------------------------
/**
 * @brief   ���[�N�̃A�h���X���擾
 *
 * @retval	brs�Ɋi�[����Ă��郏�[�N�A�h���X		
 */
//--------------------------------------------------------------
BATTLE_REC_SAVEDATA * BattleRec_WorkAdrsGet( void )
{
	GF_ASSERT(brs);

	return brs;
}

//--------------------------------------------------------------
/**
 * @brief   �F�؃L�[���������퓬�^�惏�[�N�̃A�h���X���擾
 *
 * @retval	brs�Ɋi�[����Ă��郏�[�N�A�h���X(�F�؃L�[����)
 */
//--------------------------------------------------------------
void * BattleRec_RecWorkAdrsGet( void )
{
	u8 *work;
	
	GF_ASSERT(brs);
	
	work = (u8*)brs;
	return &work[sizeof(EX_CERTIFY_SAVE_KEY)];
}

//------------------------------------------------------------------
/**
 * �ΐ�^��f�[�^�̃��[�h
 *
 * @param	sv		�Z�[�u�f�[�^�\���̂ւ̃|�C���^
 * @param	heapID	�f�[�^�����[�h���郁�������m�ۂ��邽�߂̃q�[�vID
 * @param	result	���[�h���ʂ��i�[���郏�[�NRECLOAD_RESULT_NULL,OK,NG,ERROR
 * @param	bp		���[�h�����f�[�^���琶������BATTLE_PARAM�\���̂ւ̃|�C���^
 * @param	num		���[�h����f�[�^�i���o�[�iLOADDATA_MYREC�ALOADDATA_DOWNLOAD0�ALOADDATA_DOWNLOAD1�c�j
 *
 * @retval	TRUE
 */
//------------------------------------------------------------------
BOOL BattleRec_Load(SAVEDATA *sv,int heapID,LOAD_RESULT *result,BATTLE_PARAM *bp,int num)
{
	BATTLE_REC_WORK *rec;
	BATTLE_REC_HEADER *head;
	
	//���łɓǂݍ��܂�Ă���f�[�^������Ȃ�A�j������
	if(brs){
		sys_FreeMemoryEz(brs);
		brs=NULL;
	}
	//�{���́Anum�ɂ���ēǂݍ��ݐ��ς���
	brs=SaveData_Extra_LoadBattleRecData(sv,heapID,result,num);
	if(*result != LOAD_RESULT_OK){
		*result = RECLOAD_RESULT_ERROR;
		return TRUE;
	}
	
	rec = &brs->rec;
	head = &brs->head;
	
	//����
#if 0
	BattleRec_Decoded(rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE, rec->checksum);
#else
	BattleRec_Decoded(rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE, 
		rec->crc.crc16ccitt_hash + ((rec->crc.crc16ccitt_hash ^ 0xffff) << 16));
#endif

	//�ǂݏo�����f�[�^�ɘ^��f�[�^�������Ă��邩�`�F�b�N
	if(BattleRec_DataInitializeCheck(sv, brs) == TRUE){
		*result = RECLOAD_RESULT_NULL;	//�������f�[�^�ׁ̈A�f�[�^�Ȃ�
		return TRUE;
	}

	//�f�[�^�̐������`�F�b�N
	if(BattleRecordCheckData(sv, brs) == FALSE){
	#ifdef OSP_REC_ON
		OS_TPrintf("�s���Ș^��f�[�^\n");
	#endif
		*result = RECLOAD_RESULT_NG;
		return TRUE;
	}

	//�ǂݏo���f�[�^��BATTLE_PARAM�ɃZ�b�g
	if(bp){
		BattleRec_BattleParamCreate(bp,sv);
	}

	*result = RECLOAD_RESULT_OK;	
	return TRUE;
}

//------------------------------------------------------------------
/**
 * ���ɑΐ�^��f�[�^�����݂��Ă��邩���ׂ�(BattleRec_Load�֐�����f�[�^�`�F�b�N�̂ݔ����o��������)
 *
 * @param	sv		�Z�[�u�f�[�^�\���̂ւ̃|�C���^
 * @param	heapID	�f�[�^�����[�h���郁�������m�ۂ��邽�߂̃q�[�vID
 * @param	result	���[�h���ʂ��i�[���郏�[�NRECLOAD_RESULT_NULL,OK,NG,ERROR
 * @param	bp		���[�h�����f�[�^���琶������BATTLE_PARAM�\���̂ւ̃|�C���^
 * @param	num		���[�h����f�[�^�i���o�[�iLOADDATA_MYREC�ALOADDATA_DOWNLOAD0�ALOADDATA_DOWNLOAD1�c�j
 *
 * @retval	TRUE:����Ș^��f�[�^�����݂��Ă���
 * @retval	FALSE:����Ș^��f�[�^�͑��݂��Ă��Ȃ�
 */
//------------------------------------------------------------------
BOOL BattleRec_DataOccCheck(SAVEDATA *sv,int heapID,LOAD_RESULT *result,int num)
{
	BATTLE_REC_WORK *rec;
	BATTLE_REC_HEADER *head;
	BATTLE_REC_SAVEDATA *all;
	
	all=SaveData_Extra_LoadBattleRecData(sv,heapID,result,num);
	if(*result != LOAD_RESULT_OK){
		*result = RECLOAD_RESULT_ERROR;
		sys_FreeMemoryEz(all);
		return FALSE;
	}
	
	rec = &all->rec;
	head = &all->head;
	
	//����
#if 0
	BattleRec_Decoded(rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE, rec->checksum);
#else
	BattleRec_Decoded(rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE, 
		rec->crc.crc16ccitt_hash + ((rec->crc.crc16ccitt_hash ^ 0xffff) << 16));
#endif

	//�ǂݏo�����f�[�^�ɘ^��f�[�^�������Ă��邩�`�F�b�N
	if(BattleRec_DataInitializeCheck(sv, all) == TRUE){
		*result = RECLOAD_RESULT_NULL;	//�������f�[�^�ׁ̈A�f�[�^�Ȃ�
		sys_FreeMemoryEz(all);
		return FALSE;
	}

	//�f�[�^�̐������`�F�b�N
	if(BattleRecordCheckData(sv, all) == FALSE){
	#ifdef OSP_REC_ON
		OS_TPrintf("�s���Ș^��f�[�^\n");
	#endif
		*result = RECLOAD_RESULT_NG;
		sys_FreeMemoryEz(all);
		return FALSE;
	}

	*result = RECLOAD_RESULT_OK;
	sys_FreeMemoryEz(all);
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �ΐ�^��̃Z�[�u�������܂Ƃ߂�����
 *
 * @param   sv		�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   num		LOADDATA_MYREC�ALOADDATA_DOWNLOAD0�ALOADDATA_DOWNLOAD1�c
 * @param   seq		�Z�[�u�i�s�𐧌䂷�郏�[�N�ւ̃|�C���^(�ŏ���0�N���A������ԂŌĂ�ŉ�����)
 *
 * @retval	SAVE_RESULT_CONTINUE	�Z�[�u�p����
 * @retval	SAVE_RESULT_LAST		�Z�[�u�p�����A�Ō�̕���
 * @retval	SAVE_RESULT_OK			�Z�[�u�I���A����
 * @retval	SAVE_RESULT_NG			�Z�[�u�I���A���s
 */
//--------------------------------------------------------------
SAVE_RESULT Local_BattleRecSave(SAVEDATA *sv, BATTLE_REC_SAVEDATA *work, int num, u16 *seq)
{
	SAVE_RESULT result;
	
	switch(*seq){
	case 0:
		sys_SoftResetNG(SOFTRESET_TYPE_VIDEO);
		sys_SioErrorNG_PtrSet(HEAPID_WORLD);
		
		result = SaveData_Extra_SaveBattleRecData(sv,work,num);
		if(result == SAVE_RESULT_OK){
			//result = SaveData_Save(sv);
			SaveData_DivSave_Init(sv, SVBLK_ID_MAX);
			(*seq)++;
			return SAVE_RESULT_CONTINUE;
		}
		
		sys_SoftResetOK(SOFTRESET_TYPE_VIDEO);
		return result;
	case 1:
		result = SaveData_DivSave_Main(sv);
		if(result == SAVE_RESULT_OK || result == SAVE_RESULT_NG){
			(*seq) = 0;
			sys_SioErrorNG_PtrFree();
			sys_SoftResetOK(SOFTRESET_TYPE_VIDEO);
		}
		return result;
	}
	return SAVE_RESULT_CONTINUE;
}

//------------------------------------------------------------------
/**
 * �ΐ�^��f�[�^�̃Z�[�u
 *
 * @param	sv				�Z�[�u�f�[�^�\���̂ւ̃|�C���^
 * @param	data			�Z�[�u�f�[�ւ̃|�C���^
 * @param   rec_mode		�^�惂�[�h(RECMODE_???)
 * @param   fight_count		����ڂ�
 * @param	num		���[�h����f�[�^�i���o�[�iLOADDATA_MYREC�ALOADDATA_DOWNLOAD0�ALOADDATA_DOWNLOAD1�c�j
 * @param   work0		�Z�[�u�i�s�𐧌䂷�郏�[�N�ւ̃|�C���^(�ŏ���0�N���A������ԂŌĂ�ŉ�����)
 * @param   work1		�Z�[�u�i�s�𐧌䂷�郏�[�N�ւ̃|�C���^(�ŏ���0�N���A������ԂŌĂ�ŉ�����)
 *
 * @retval	�Z�[�u����(SAVE_RESULT_OK or SAVE_RESULT_NG ���Ԃ�܂ŏꍇ�͖��t���[���Ăё����Ă�������)
 */
//------------------------------------------------------------------
SAVE_RESULT BattleRec_Save(SAVEDATA *sv, int rec_mode, int fight_count, int num, u16 *work0, u16 *work1)
{
	BATTLE_REC_HEADER *head;
	BATTLE_REC_WORK *rec;
	SAVE_RESULT result;
	
	switch(*work0){
	case 0:
		//�f�[�^���Ȃ��Ƃ��́A�������Ȃ�
		if(brs==NULL){
			return	SAVE_RESULT_NG;
		}
		head = &brs->head;
		rec = &brs->rec;
		
		//�^��f�[�^�{�̂����Ƀw�b�_�f�[�^�쐬
		RecHeaderCreate(sv, head, rec, rec_mode, fight_count);

		//CRC�쐬
		head->magic_key = REC_OCC_MAGIC_KEY;
		head->crc.crc16ccitt_hash = SaveData_CalcCRC(sv, head, 
			sizeof(BATTLE_REC_HEADER) - GDS_CRC_SIZE - DATANUMBER_SIZE);
		rec->magic_key = REC_OCC_MAGIC_KEY;
		rec->crc.crc16ccitt_hash = SaveData_CalcCRC(sv, rec, 
			sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE);

		//CRC���L�[�ɂ��ĈÍ���
		BattleRec_Coded(rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE, 
			rec->crc.crc16ccitt_hash + ((rec->crc.crc16ccitt_hash ^ 0xffff) << 16));
		
		*work1 = 0;
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
 * @brief   �w��ʒu�̘^��f�[�^���폜(������)���ăZ�[�u���s
 *
 * @param   sv			
 * @param   heap_id		
 * @param   num			
 * @param   work0		�Z�[�u�i�s�𐧌䂷�郏�[�N�ւ̃|�C���^(�ŏ���0�N���A������ԂŌĂ�ŉ�����)
 * @param   work1		�Z�[�u�i�s�𐧌䂷�郏�[�N�ւ̃|�C���^(�ŏ���0�N���A������ԂŌĂ�ŉ�����)
 *
 * @retval  SAVE_RESULT_CONTINUE	�Z�[�u�����p����
 * @retval  SAVE_RESULT_LAST		�Z�[�u�����p�����A�Ō�̈�O
 * @retval  SAVE_RESULT_OK			�Z�[�u����I��
 * @retval  SAVE_RESULT_NG			�Z�[�u���s�I��
 *
 * �������̓I�t���C���ōs���̂ŕ����Z�[�u�ł͂Ȃ��A�ꊇ�Z�[�u�ɂ��Ă��܂��B
 */
//--------------------------------------------------------------
SAVE_RESULT BattleRec_SaveDataErase(SAVEDATA *sv, int heap_id, int num)
{
	SAVE_RESULT result;
	LOAD_RESULT load_result;
	
	GF_ASSERT(brs == NULL);

	brs = SaveData_Extra_LoadBattleRecData(sv, heap_id, &load_result, num);
	BattleRec_WorkInit(brs);
	result = SaveData_Extra_SaveBattleRecData(sv, brs, num);
	if(result == SAVE_RESULT_OK){
		result = SaveData_Save(sv);
	}
	else{
		OS_TPrintf("erase error\n");
	}
	
	sys_FreeMemoryEz(brs);
	brs = NULL;
	return result;
}

//--------------------------------------------------------------
/**
 * @brief   �^�惂�[�h����N���C�A���g���Ǝ莝�����̏�����擾
 *
 * @param   rec_mode		�^�惂�[�h(RECMODE_???)
 * @param   client_max		�N���C�A���g�������
 * @param   temoti_max		�莝���ő吔�����
 */
//--------------------------------------------------------------
void BattleRec_ClientTemotiGet(int rec_mode, int *client_max, int *temoti_max)
{
	switch(rec_mode){
	case RECMODE_TOWER_MULTI:
	case RECMODE_FACTORY_MULTI:
	case RECMODE_FACTORY_MULTI100:
	case RECMODE_STAGE_MULTI:
	case RECMODE_CASTLE_MULTI:
	case RECMODE_ROULETTE_MULTI:
	case RECMODE_COLOSSEUM_MULTI:
		*client_max = CLIENT_MAX;
		*temoti_max = TEMOTI_POKEMAX / 2;
		break;
	default:
		*client_max = CLIENT_MAX / 2;
		*temoti_max = TEMOTI_POKEMAX;
		break;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �^��f�[�^�{�̂����Ƀw�b�_�[�f�[�^���쐬
 *
 * @param   head		�w�b�_�[�f�[�^�����
 * @param   rec			�^��f�[�^�{��
 * @param   rec_mode	�^�惂�[�h(RECMODE_???)
 * @param   counter		����ڂ�
 */
//--------------------------------------------------------------
static void RecHeaderCreate(SAVEDATA *sv, BATTLE_REC_HEADER *head, const BATTLE_REC_WORK *rec, 
	int rec_mode, int counter)
{
	int client, temoti, client_max, temoti_max, n, get_client, my_client;
	const REC_POKEPARA *para;
	const u8	stand_table[2][4]={{0,2,3,1},{3,1,0,2}};
	const u8	tower_table[4]={0,2,1,3};
		
	MI_CpuClear8(head, sizeof(BATTLE_REC_HEADER));
	
	BattleRec_ClientTemotiGet(rec_mode, &client_max, &temoti_max);

	n = 0;
	
	//�����̃N���C�A���g�i���o�[�擾
	if(rec->rbp.fight_type&FIGHT_TYPE_SIO){
		if(rec->rbp.fight_type&FIGHT_TYPE_TOWER){
			my_client=rec->rbp.comm_id*2;
		}
		else{
			my_client=rec->rbp.comm_id;
		}
	}
	else{
		my_client=0;
	}

	for(client = 0; client < client_max; client++){
		if((rec->rbp.fight_type&FIGHT_TYPE_MULTI)&&((rec->rbp.fight_type&FIGHT_TYPE_TOWER)==0)){
			for(get_client=0;get_client<client_max;get_client++){
				if(rec->rbp.comm_stand_no[get_client]==
				   stand_table[rec->rbp.comm_stand_no[my_client]&1][client]){
					break;
				}
			}
		}
		else if((rec->rbp.fight_type&FIGHT_TYPE_MULTI)&&(rec->rbp.fight_type&FIGHT_TYPE_TOWER)){
			get_client= tower_table[client];
		}
		else{
			get_client=client;
			if(my_client&1){
				get_client^=1;
			}
		}
		for(temoti = 0; temoti < temoti_max; temoti++){
			para = &(rec->rec_party[get_client].member[temoti]);
			if(para->tamago_flag == 0 && para->fusei_tamago_flag == 0){
				head->monsno[n] = para->monsno;
				head->form_no[n] = para->form_no;
			}
			n++;
		}
	}
	
	//���M�����[�V�����f�[�^�Z�b�g
	switch(rec_mode){
	case RECMODE_COLOSSEUM_SINGLE_STANDARD:		//MIX�̓V���O���ƈꏏ
	case RECMODE_COLOSSEUM_DOUBLE_STANDARD:
		head->regulation = *(Data_GetRegulation(sv, REGULATION_NO_STANDARD));
		break;
	case RECMODE_COLOSSEUM_SINGLE_FANCY:
	case RECMODE_COLOSSEUM_DOUBLE_FANCY:
		head->regulation = *(Data_GetRegulation(sv, REGULATION_NO_FANCY));
		break;
	case RECMODE_COLOSSEUM_SINGLE_LITTLE:
	case RECMODE_COLOSSEUM_DOUBLE_LITTLE:
		head->regulation = *(Data_GetRegulation(sv, REGULATION_NO_LITTLE));
		break;
	case RECMODE_COLOSSEUM_SINGLE_LIGHT:
	case RECMODE_COLOSSEUM_DOUBLE_LIGHT:
		head->regulation = *(Data_GetRegulation(sv, REGULATION_NO_LIGHT));
		break;
	case RECMODE_COLOSSEUM_SINGLE_DOUBLE:
	case RECMODE_COLOSSEUM_DOUBLE_DOUBLE:
		head->regulation = *(Data_GetRegulation(sv, REGULATION_NO_DOUBLE));
		break;
	case RECMODE_COLOSSEUM_SINGLE_ETC:
	case RECMODE_COLOSSEUM_DOUBLE_ETC:
		head->regulation = *(Data_GetRegulation(sv, REGULATION_NO_ETC));
		break;
	case RECMODE_COLOSSEUM_SINGLE:		//��������
	case RECMODE_COLOSSEUM_DOUBLE:
	default:
		head->regulation = *(Data_GetNoLimitRegulation());
		break;
	}
	
	head->battle_counter = counter;
	head->mode = rec_mode;
}

//--------------------------------------------------------------
/**
 * @brief   �ΐ�^��f�[�^�����������ꂽ�f�[�^�����ׂ�
 *
 * @param   src		�ΐ�^��f�[�^�ւ̃|�C���^
 *
 * @retval  TRUE:����������Ă���f�[�^
 * @retval  FALSE:���炩�̃f�[�^�������Ă���
 */
//--------------------------------------------------------------
static BOOL BattleRec_DataInitializeCheck(SAVEDATA *sv, BATTLE_REC_SAVEDATA *src)
{
	BATTLE_REC_WORK *rec = &src->rec;
	BATTLE_REC_HEADER *head = &src->head;

	if(SaveData_GetExtraInitFlag(sv) == FALSE){
		return TRUE;	//�O���Z�[�u�f�[�^������������Ă��Ȃ��̂Ńf�[�^�����Ɣ��肷��
	}
	
//	if(rec->crc.crc16ccitt_hash == 0 || head->crc.crc16ccitt_hash == 0){
	if(rec->magic_key != REC_OCC_MAGIC_KEY || head->magic_key != REC_OCC_MAGIC_KEY){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �Z�[�u�f�[�^�̐������`�F�b�N
 *
 * @param   src		�f�[�^�̐擪�A�h���X
 *
 * @retval  TRUE:�������B�@FALSE:�s��
 */
//--------------------------------------------------------------
static	BOOL BattleRecordCheckData(SAVEDATA *sv, const BATTLE_REC_SAVEDATA * src)
{
	const BATTLE_REC_WORK *rec = &src->rec;
	const BATTLE_REC_HEADER *head = &src->head;
	u16 hash;
	
//	if(rec->crc.crc16ccitt_hash == 0 || head->crc.crc16ccitt_hash == 0){
	if(rec->magic_key != REC_OCC_MAGIC_KEY || head->magic_key != REC_OCC_MAGIC_KEY){
		return FALSE;
	}
	
	//�w�b�_�[��CRC�n�b�V���v�Z
	hash = SaveData_CalcCRC(sv, head, 
		sizeof(BATTLE_REC_HEADER) -GDS_CRC_SIZE-DATANUMBER_SIZE);
	if(hash != head->crc.crc16ccitt_hash){
	#ifdef OSP_REC_ON
		OS_TPrintf("�w�b�_�[��CRC�n�b�V���s��\n");
	#endif
		return FALSE;
	}
	
	//�{�̑S�̂�CRC�n�b�V���v�Z
	hash = SaveData_CalcCRC(sv, rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE);
	if (hash != rec->crc.crc16ccitt_hash) {
	#ifdef OSP_REC_ON
		OS_TPrintf("�^��f�[�^�{�̂�CRC�n�b�V���s��\n");
	#endif
		return FALSE;
	}
	
	//�|�P�����p�����[�^�̕s���`�F�b�N
	{
		int client, temoti, wazano;
		const REC_POKEPARA *para;
		
		for(client = 0; client < CLIENT_MAX; client++){
			for(temoti = 0; temoti < TEMOTI_POKEMAX; temoti++){
				para = &(rec->rec_party[client].member[temoti]);
			#if 0
				//�_���^�}�S
				if(para->fusei_tamago_flag == 1){
					OS_TPrintf("�_���^�}�S���������Ă���\n");
					return FALSE;
				}
			#endif
				//�s���ȃ|�P�����ԍ�
				if(para->monsno > MONSNO_MAX){
				#ifdef OSP_REC_ON
					OS_TPrintf("�s���ȃ|�P�����ԍ�\n");
				#endif
					return FALSE;
				}
				//�s���ȃA�C�e���ԍ�
				if(para->item > ITEM_DATA_MAX){
				#ifdef OSP_REC_ON
					OS_TPrintf("�s���ȃA�C�e���ԍ�\n");
				#endif
					return FALSE;
				}
				//�s���ȋZ�ԍ�
				for(wazano = 0; wazano < WAZA_TEMOTI_MAX; wazano++){
					if(para->waza[wazano] > WAZANO_MAX){
					#ifdef OSP_REC_ON
						OS_TPrintf("�s���ȋZ�ԍ�\n");
					#endif
						return FALSE;
					}
				}
			}
		}
	}
	
	return TRUE;
}

//============================================================================================
/**
 *	�Í�����
 *
 * @param[in]	data	�Í�������f�[�^�̃|�C���^
 * @param[in]	size	�Í�������f�[�^�̃T�C�Y
 * @param[in]	code	�Í����L�[�̏����l
 */
//============================================================================================
void	BattleRec_Coded(void *data,u32 size,u32 code)
{
	CalcTool_Coded(data, size, code);
}

//============================================================================================
/**
 *	��������
 *
 * @param[in]	data	��������f�[�^�̃|�C���^
 * @param[in]	size	��������f�[�^�̃T�C�Y
 * @param[in]	code	�Í����L�[�̏����l
 */
//============================================================================================
static	void	BattleRec_Decoded(void *data,u32 size,u32 code)
{
	CalcTool_Decoded(data,size,code);
}


//============================================================================================
/**
 *	�퓬�^�揈��
 *
 * @param[in]	client_no	�^����s��ClientNo
 * @param[in]	pos			�^��f�[�^�̊i�[�ꏊ
 * @param[in]	data		�^��f�[�^
 */
//============================================================================================
void BattleRec_DataRec(int client_no,int pos,REC_DATA data)
{
	if(brs==NULL){
		return;
	}
	brs->rec.rp.rec_buffer[client_no][pos]=data;
}

//============================================================================================
/**
 *	�퓬�Đ��f�[�^�擾����
 *
 * @param[in]	rp			�퓬�V�X�e�����[�N�̍\���̃|�C���^
 * @param[in]	client_no	�^����s��ClientNo
 * @param[in]	pos			�^��f�[�^�̊i�[�ꏊ
 */
//============================================================================================
REC_DATA BattleRec_DataPlay(int client_no,int pos)
{
	GF_ASSERT(brs!=NULL);
	return brs->rec.rp.rec_buffer[client_no][pos];
}

//============================================================================================
/**
 *	BATTLE_PARAM�\���̕ۑ�����
 *
 * @param[in]	bp			BATTLE_PARAM�\���̂ւ̃|�C���^
 */
//============================================================================================
void BattleRec_BattleParamRec(BATTLE_PARAM *bp)
{
	int i;
	BATTLE_REC_WORK *rec;
	REC_BATTLE_PARAM *rbp;
	
	if(brs==NULL){
		return;
	}

	rec = &brs->rec;
	rbp = &rec->rbp;
#if 0
	rec->bp=*bp;
#else
	rbp->fight_type = bp->fight_type;
	rbp->win_lose_flag = bp->win_lose_flag;
//	rbp->btr = bp->btr;
	rbp->bg_id = bp->bg_id;
	rbp->ground_id = bp->ground_id;
	rbp->place_id = bp->place_id;
	rbp->zone_id = bp->zone_id;
	rbp->time_zone = bp->time_zone;
	rbp->shinka_place_mode = bp->shinka_place_mode;
	rbp->contest_see_flag = bp->contest_see_flag;
	rbp->mizuki_flag = bp->mizuki_flag;
	rbp->get_pokemon_client = bp->get_pokemon_client;
	rbp->weather = bp->weather;
	rbp->level_up_flag = bp->level_up_flag;
	rbp->battle_status_flag = bp->battle_status_flag;
	rbp->safari_ball = bp->safari_ball;
	rbp->regulation_flag = bp->regulation_flag;
	rbp->rand = bp->rand;
	rbp->comm_id = bp->comm_id;
	rbp->dummy = bp->dummy;
	rbp->total_turn = bp->total_turn;
	for(i = 0; i < CLIENT_MAX; i++){
		rbp->trainer_id[i] = bp->trainer_id[i];
		rbp->trainer_data[i] = bp->trainer_data[i];
		if(bp->server_version[i] == 0){
			rbp->server_version[i] = BATTLE_SERVER_VERSION;
		}
		else{
			rbp->server_version[i] = bp->server_version[i];
		}
		rbp->comm_stand_no[i] = bp->comm_stand_no[i];
		rbp->voice_waza_param[i] = bp->voice_waza_param[i];
	}
#endif

	//-- REC_BATTLE_PARAM�ł͂Ȃ��ꏊ�ɕۑ�����f�[�^���Z�b�g --//
	for(i=0;i<CLIENT_MAX;i++){
		PokeParty_to_RecPokeParty(bp->poke_party[i], &rec->rec_party[i]);
		MyStatus_Copy(bp->my_status[i],&rec->my_status[i]);
		rbp->voice_waza_param[i]=Snd_PerapVoiceWazaParamGet(bp->poke_voice[i]);
	}
	CONFIG_Copy(bp->config,&rec->config);
}

//--------------------------------------------------------------
/**
 * @brief   �T�[�o�[�o�[�W�����X�V����
 *
 * @param   id_no				ID
 * @param   server_version		�T�[�o�[�o�[�W����
 */
//--------------------------------------------------------------
void BattleRec_ServerVersionUpdate(int id_no, u32 server_version)
{
	BATTLE_REC_WORK *rec;
	REC_BATTLE_PARAM *rbp;

	if(brs==NULL){
		return;
	}
	
	rec = &brs->rec;
	rbp = &rec->rbp;
	rbp->server_version[id_no] = server_version;
//	OS_TPrintf("sio server_version = %x\n", rbp->server_version[id_no]);
}

//--------------------------------------------------------------
/**
 * @brief   ������ROM�̃T�[�o�[�o�[�W�������������T�[�o�[�o�[�W�������L�^����Ă��邩�m�F
 *
 * @param   none		
 *
 * @retval  TRUE:�S�Ď����Ɠ������ȉ��̃T�[�o�[�o�[�W����
 * @retval  FALSE:������ROM���������T�[�o�[�o�[�W�������L�^����Ă���
 */
//--------------------------------------------------------------
BOOL BattleRec_ServerVersionCheck(void)
{
	int i;
	BATTLE_REC_WORK *rec;
	REC_BATTLE_PARAM *rbp;
	
	if(brs == NULL){
		return TRUE;
	}

	rec = &brs->rec;
	rbp = &rec->rbp;
	for(i = 0; i < CLIENT_MAX; i++){
		if(rbp->server_version[i] > BATTLE_SERVER_VERSION){
			return FALSE;
		}
	}
	return TRUE;
}

//============================================================================================
/**
 *	BATTLE_PARAM�\���̐�������
 *
 * @param[in]	bp			BATTLE_PARAM�\���̂ւ̃|�C���^
 * @param[in]	sv			�Z�[�u�̈�ւ̃|�C���^
 */
//============================================================================================
void BattleRec_BattleParamCreate(BATTLE_PARAM *bp,SAVEDATA *sv)
{
	int	i;
	BATTLE_REC_WORK *rec = &brs->rec;
	
	bp->fight_type			=	rec->rbp.fight_type;
//	bp->win_lose_flag		=	rec->rbp.win_lose_flag;
//	bp->btr					=	rec->rbp.btr;
	bp->bg_id				=	rec->rbp.bg_id;
	bp->ground_id			=	rec->rbp.ground_id;
	bp->place_id			=	rec->rbp.place_id;
	bp->zone_id				=	rec->rbp.zone_id;
	bp->time_zone			=	rec->rbp.time_zone;
	bp->shinka_place_mode	=	rec->rbp.shinka_place_mode;
	bp->contest_see_flag	=	rec->rbp.contest_see_flag;
	bp->mizuki_flag			=	rec->rbp.mizuki_flag;
	bp->get_pokemon_client	=	rec->rbp.get_pokemon_client;
	bp->weather				=	rec->rbp.weather;
//	bp->level_up_flag		=	rec->rbp.level_up_flag;
	bp->battle_status_flag	=	rec->rbp.battle_status_flag|BATTLE_STATUS_FLAG_REC_BATTLE;
	bp->safari_ball			=	rec->rbp.safari_ball;
	bp->regulation_flag		=	rec->rbp.regulation_flag;
	bp->rand				=	rec->rbp.rand;
	bp->comm_id				=	rec->rbp.comm_id;
//	bp->total_turn			=	rec->rbp.total_turn;

	bp->win_lose_flag		=	0;
	bp->level_up_flag		=	0;

	ZukanWork_Copy(SaveData_GetZukanWork(sv),bp->zw);

	for(i=0;i<CLIENT_MAX;i++){
		bp->trainer_id[i]=rec->rbp.trainer_id[i];
		bp->trainer_data[i]=rec->rbp.trainer_data[i];
		bp->server_version[i]=rec->rbp.server_version[i];
		bp->comm_stand_no[i]=rec->rbp.comm_stand_no[i];
		RecPokeParty_to_PokeParty(&rec->rec_party[i], bp->poke_party[i]);
		MyStatus_Copy(&rec->my_status[i],bp->my_status[i]);
		bp->voice_waza_param[i]=rec->rbp.voice_waza_param[i];
	}
//	CONFIG_Copy(&rec->config,bp->config);
	CONFIG_Copy(SaveData_GetConfig(sv), bp->config);
	bp->config->window_type = rec->config.window_type;
	if(bp->config->window_type >= TALK_WINDOW_MAX){
		bp->config->window_type = 0;	//����ŉ�b�E�B���h�E�������鎖���l���ăP�A�����Ă���
	}
}

//--------------------------------------------------------------
/**
 * @brief   POKEPARTY��REC_POKEPARTY�ɕϊ�����
 *
 * @param   party			�ϊ����f�[�^�ւ̃|�C���^
 * @param   rec_party		�ϊ���̃f�[�^�����
 */
//--------------------------------------------------------------
static void PokeParty_to_RecPokeParty(const POKEPARTY *party, REC_POKEPARTY *rec_party)
{
	int i;
	POKEMON_PARAM *pp;
	
	MI_CpuClear8(rec_party, sizeof(REC_POKEPARTY));
	
	rec_party->PokeCountMax = PokeParty_GetPokeCountMax(party);
	rec_party->PokeCount = PokeParty_GetPokeCount(party);
	
	for(i = 0; i < rec_party->PokeCount; i++){
		pp = PokeParty_GetMemberPointer(party, i);
		PokePara_to_RecPokePara(pp, &rec_party->member[i]);
	}
}

//--------------------------------------------------------------
/**
 * @brief   REC_POKEPARTY��POKEPARTY�ɕϊ�����
 *
 * @param   rec_party		�ϊ����f�[�^�ւ̃|�C���^
 * @param   party			�ϊ���̃f�[�^�����
 */
//--------------------------------------------------------------
static void RecPokeParty_to_PokeParty(REC_POKEPARTY *rec_party, POKEPARTY *party)
{
	int i;
	POKEMON_PARAM *pp;
	u8 cb_id_para = 0;
	
	pp = PokemonParam_AllocWork(HEAPID_WORLD);
	
	PokeParty_Init(party, rec_party->PokeCountMax);
	for(i = 0; i < rec_party->PokeCount; i++){
		RecPokePara_to_PokePara(&rec_party->member[i], pp);
		PokeParaPut(pp, ID_PARA_cb_id, &cb_id_para);	//�J�X�^���{�[���͏o�Ȃ��悤�ɂ���
		PokeParty_Add(party, pp);
	}
	
	sys_FreeMemoryEz(pp);
}

//------------------------------------------------------------------
/**
 * �^��f�[�^�P���ǉ�
 *
 * @param   data		�a������Z�[�u�f�[�^�|�C���^
 * @param   party		�a������p�[�e�B�[�̃f�[�^
 * @param   date		�a��������t�f�[�^�iNITRO�̃f�[�^���̂܂܁j
 *
 */
//------------------------------------------------------------------
/*
void BattleRec_AddRecord( BATTLE_REC_SAVEDATA* data, const u8 *src)
{
	GF_ASSERT(data != NULL);

	MI_CpuCopy32(src, data, sizeof(BATTLE_REC_SAVEDATA));
}
*/

//------------------------------------------------------------------
/**
 * �a�����背�R�[�h����A�|�P�����f�[�^�擾
 *
 * @param   data		�a������f�[�^�|�C���^
 * @param   index		���R�[�h�C���f�b�N�X�i0���ŐV�ŁA���ɋ����Ȃ��Ă����j
 * @param   poke_pos	���̖ڂ̃|�P�������i0�`5�j
 * @param   poke_data	�|�P�����f�[�^���󂯎��\���̂̃|�C���^
 */
//------------------------------------------------------------------
/*
void BattleRec_GetRecord( const BATTLE_REC_SAVEDATA* data, int index, DENDOU_POKEMON_DATA* poke_data )
{
	const DENDOU_POKEMON_DATA_INSIDE*  recPoke;
	int i;

	GF_ASSERT(data != NULL);
	GF_ASSERT(data->savePoint < DENDOU_RECORD_MAX);
	GF_ASSERT(index < DENDOU_RECORD_MAX);

	index = recIndex_to_datIndex( data, index );
	recPoke = &(data->record[index].pokemon[poke_pos]);

	poke_data->monsno = recPoke->monsno;
	poke_data->level = recPoke->level;
	poke_data->personalRandom = recPoke->personalRandom;
	poke_data->idNumber = recPoke->idNumber;
	poke_data->formNumber = recPoke->formNumber;

	STRBUF_SetStringCode( poke_data->nickname, recPoke->nickname );
	STRBUF_SetStringCode( poke_data->oyaname, recPoke->oyaname );

	for(i=0; i<4; i++)
	{
		poke_data->waza[i] = recPoke->waza[i];
	}
}
*/

//==============================================================================
//
//	�f�[�^�擾
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �퓬�^��f�[�^����A�w�b�_�[��ʓr�m�ۂ������[�N�ɃR�s�[����
 *
 * @param   src		�퓬�^��Z�[�u�f�[�^�ւ̃|�C���^
 * @param   heap_id	�q�[�vID
 *
 * @retval  �m�ۂ��ꂽ�w�b�_�[�f�[�^�ւ̃|�C���^
 *
 * BattleRec_Load or BattleRec_Init���g�p����brs�Ƀf�[�^�����[�h���Ă���K�v������܂��B
 */
//--------------------------------------------------------------
BATTLE_REC_HEADER_PTR BattleRec_HeaderAllocCopy(int heap_id)
{
	BATTLE_REC_HEADER_PTR head;
	
	GF_ASSERT(brs != NULL);
	
	head = sys_AllocMemory(heap_id, sizeof(BATTLE_REC_HEADER));
	MI_CpuCopy32(&brs->head, head, sizeof(BATTLE_REC_HEADER));
	return head;
}

//--------------------------------------------------------------
/**
 * @brief   �퓬�^��f�[�^����AGDS�v���t�B�[����ʓr�m�ۂ������[�N�ɃR�s�[����
 *
 * @param   src		�퓬�^��Z�[�u�f�[�^�ւ̃|�C���^
 * @param   heap_id	�q�[�vID
 *
 * @retval  �m�ۂ��ꂽGDS�v���t�B�[���f�[�^�ւ̃|�C���^
 *
 * BattleRec_Load or BattleRec_Init���g�p����brs�Ƀf�[�^�����[�h���Ă���K�v������܂��B
 */
//--------------------------------------------------------------
GDS_PROFILE_PTR BattleRec_GDSProfileAllocCopy(int heap_id)
{
	GDS_PROFILE_PTR profile;

	GF_ASSERT(brs != NULL);
	
	profile = sys_AllocMemory(heap_id, sizeof(GDS_PROFILE));
	MI_CpuCopy32(&brs->profile, profile, sizeof(GDS_PROFILE));
	return profile;
	
}

//--------------------------------------------------------------
/**
 * @brief   GDS�v���t�B�[���̃|�C���^�擾
 *
 * @param   src		�퓬�^��Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @retval  �v���t�B�[���̃|�C���^
 *
 * BattleRec_Load or BattleRec_Init���g�p����brs�Ƀf�[�^�����[�h���Ă���K�v������܂��B
 */
//--------------------------------------------------------------
GDS_PROFILE_PTR BattleRec_GDSProfilePtrGet(void)
{
	GF_ASSERT(brs != NULL);
	return &brs->profile;
}

//--------------------------------------------------------------
/**
 * @brief   �^��{�̂̃|�C���^�擾
 *
 * @param   src		�퓬�^��Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @retval  �^��{�̂̃|�C���^
 *
 * BattleRec_Load or BattleRec_Init���g�p����brs�Ƀf�[�^�����[�h���Ă���K�v������܂��B
 */
//--------------------------------------------------------------
BATTLE_REC_WORK_PTR BattleRec_WorkPtrGet(void)
{
	GF_ASSERT(brs != NULL);
	return &brs->rec;
}

//--------------------------------------------------------------
/**
 * @brief   �^��w�b�_�̃|�C���^�擾
 *
 * @param   src		�퓬�^��Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @retval  �^��w�b�_�̃|�C���^
 *
 * BattleRec_Load or BattleRec_Init���g�p����brs�Ƀf�[�^�����[�h���Ă���K�v������܂��B
 */
//--------------------------------------------------------------
BATTLE_REC_HEADER_PTR BattleRec_HeaderPtrGet(void)
{
	GF_ASSERT(brs != NULL);
	return &brs->head;
}

//--------------------------------------------------------------
/**
 * @brief   �ǂݍ���ł���^��f�[�^�ɑ΂��āA�w��f�[�^�ŏ㏑������
 *
 * @param   gpp			GDS�v���t�B�[��
 * @param   head		�^��w�b�_
 * @param   rec			�^��{��
 * @param	bp			���̂܂܍Đ�����ꍇ��BATTLE_PARAM�\���̂ւ̃|�C���^��n��
 * @param   sv			�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * BattleRec_Load or BattleRec_Init���g�p����brs�Ƀf�[�^�����[�h���Ă���K�v������܂��B
 */
//--------------------------------------------------------------
void BattleRec_DataSet(GDS_PROFILE_PTR gpp, BATTLE_REC_HEADER_PTR head, BATTLE_REC_WORK_PTR rec, BATTLE_PARAM *bp, SAVEDATA *sv)
{
	GF_ASSERT(brs != NULL);
	MI_CpuCopy8(head, &brs->head, sizeof(BATTLE_REC_HEADER));
	MI_CpuCopy8(rec, &brs->rec, sizeof(BATTLE_REC_WORK));
	MI_CpuCopy8(gpp, &brs->profile, sizeof(GDS_PROFILE));

	//����
	BattleRec_Decoded(&brs->rec, sizeof(BATTLE_REC_WORK) - GDS_CRC_SIZE, 
		brs->rec.crc.crc16ccitt_hash + ((brs->rec.crc.crc16ccitt_hash ^ 0xffff) << 16));
	
	if(bp != NULL){
		BattleRec_BattleParamCreate(bp,sv);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �^��w�b�_�̃p�����[�^�擾
 *
 * @param   head		�^��w�b�_�ւ̃|�C���^
 * @param   index		�f�[�^INDEX(RECHEAD_IDX_???)
 * @param   param		�p�����[�^
 *
 * @retval  �擾�f�[�^
 */
//--------------------------------------------------------------
u64 RecHeader_ParamGet(BATTLE_REC_HEADER_PTR head, int index, int param)
{
	GF_ASSERT(DATANUMBER_SIZE <= sizeof(u64));
	
	switch(index){
	case RECHEAD_IDX_MONSNO:
		GF_ASSERT(param < HEADER_MONSNO_MAX);
		if(head->monsno[param] > MONSNO_END){
			return 0;
		}
		return head->monsno[param];
	case RECHEAD_IDX_FORM_NO:
		GF_ASSERT(param < HEADER_MONSNO_MAX);
		return head->form_no[param];

	case RECHEAD_IDX_COUNTER:
		if ( head->battle_counter > REC_COUNTER_MAX ){
			return REC_COUNTER_MAX;
		}
		return head->battle_counter;
		
	case RECHEAD_IDX_MODE:
		if(head->mode >= RECMODE_MAX){
			return RECMODE_COLOSSEUM_SINGLE;
		}
		return head->mode;
	case RECHEAD_IDX_DATA_NUMBER:
		return head->data_number;
	case RECHEAD_IDX_SECURE:
		return head->secure;
	}
	
	GF_ASSERT(0);	//�s����INDEX
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief   �퓬�^��w�b�_��Alloc����
 *
 * @param   heap_id		�q�[�vID
 *
 * @retval  GDS�v���t�B�[�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
BATTLE_REC_HEADER_PTR BattleRec_Header_AllocMemory(int heap_id)
{
	BATTLE_REC_HEADER_PTR brhp;
	
	brhp = sys_AllocMemory(heap_id, sizeof(BATTLE_REC_HEADER));
	MI_CpuClear8(brhp, sizeof(BATTLE_REC_HEADER));
	return brhp;
}

//--------------------------------------------------------------
/**
 * @brief   �퓬�^��w�b�_�����
 *
 * @param   brhp		GDS�v���t�B�[���ւ̃|�C���^
 */
//--------------------------------------------------------------
void BattleRec_Header_FreeMemory(BATTLE_REC_HEADER_PTR brhp)
{
	sys_FreeMemoryEz(brhp);
}


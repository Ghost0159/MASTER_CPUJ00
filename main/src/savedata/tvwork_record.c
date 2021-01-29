//============================================================================================
/**
 * @file	tvwork_record.c
 * @brief	�e���r�f�[�^�̃��R�[�h����
 * @author	tamada GAME FREAK inc.
 * @date	2006.05.31
 *
 * ���R�[�h��������Ă΂�邽�߂ɏ풓�̈�ɔz�u����Ă��Ȃ��B
 * ���̂��߂ǂ�����ł��Ăׂ�킯�ł͂Ȃ����Ƃɒ���
 */
//============================================================================================

#include "common.h"
#include "savedata/savedata_def.h"
#include "system/gamedata.h"

#include "gflib/gf_date.h"

#include "savedata/tv_work.h"
#include "tvwork_local.h"

#include "field/tv_topic.h"

#include "system/pm_rtc.h"
#include "system/pm_str.h"

#include "savedata\savedata.h"
#include "communication/comm_info.h"

#ifdef	DEBUG_ONLY_FOR_tamada
#define	DEBUG_PRT_TAMADA(...)	OS_TPrintf( __VA_ARGS__ )
#else
#define	DEBUG_PRT_TAMADA(...)	((void)0)
#endif


//==============================================================================
//	�f�[�^
//==============================================================================
///����ROM�ŕ����\�g�s�b�NID�ꗗ�F����	������ŕ����g�s�b�N���ς��Ȃ炱����ҏW����
ALIGN4 static const u8 TvQandTopic[] = {
	2,4,6,7,9,10,11,13,14,15,16,17,18,19,0xff,
};
///����ROM�ŕ����\�g�s�b�NID�ꗗ�F�Ď�	������ŕ����g�s�b�N���ς��Ȃ炱����ҏW����
ALIGN4 static const u8 TvWatchTopic[] = {
	1,2,3,4,6,7,8,10,13,15,16,18,19,20,21,22,23,25,26,0xff,
};
///����ROM�ŕ����\�g�s�b�NID�ꗗ�F�M�l�X	������ŕ����g�s�b�N���ς��Ȃ炱����ҏW����
ALIGN4 static const u8 TvRecordTopic[] = {
	1,3,4,5,8,9,10,11,0xff,
};

///DP�ŕ����\�g�s�b�NID�ꗗ�F����
ALIGN4 static const u8 DP_TvQandTopic[] = {
	2,4,6,7,9,10,11,13,0xff
};
///DP�ŕ����\�g�s�b�NID�ꗗ�F�Ď�
ALIGN4 static const u8 DP_TvWatchTopic[] = {
	1,2,3,4,6,10,13,15,17,18,19,20,21,22,23,0xff
};
///DP�ŕ����\�g�s�b�NID�ꗗ�F�M�l�X
ALIGN4 static const u8 DP_TvRecordTopic[] = {
	1,3,4,6,8,9,0xff
};

typedef enum{
	TV_DATA_TYPE_QAND,				///����ROM�ŕ����\�ȃg�s�b�N�F����
	TV_DATA_TYPE_WATCH,				///����ROM�ŕ����\�ȃg�s�b�N�F�Ď�
	TV_DATA_TYPE_RECORD,			///����ROM�ŕ����\�ȃg�s�b�N�F�M�l�X
	TV_DATA_TYPE_DP_QAND,			///DP�ŕ����\�ȃg�s�b�N�F����
	TV_DATA_TYPE_DP_WATCH,			///DP�ŕ����\�ȃg�s�b�N�F�Ď�
	TV_DATA_TYPE_DP_RECORD,			///DP�ŕ����\�ȃg�s�b�N�F�M�l�X
}TV_DATA_TYPE;

static const u8 * const TvTopicID_Tbl[] = {
	TvQandTopic,				//TV_DATA_TYPE_QAND,
	TvWatchTopic,           	//TV_DATA_TYPE_WATCH,
	TvRecordTopic,          	//TV_DATA_TYPE_RECORD,
	DP_TvQandTopic,         	//TV_DATA_TYPE_DP_QAND,
	DP_TvWatchTopic,        	//TV_DATA_TYPE_DP_WATCH,
	DP_TvRecordTopic,       	//TV_DATA_TYPE_DP_RECORD,
};


//============================================================================================
//
//			��`�Ȃ�
//
//============================================================================================
//----------------------------------------------------------
/**
 * @brief	����M����e���r�g�s�b�N�̐�
 */
//----------------------------------------------------------
#define	TV_RECV_MAX			(4)			

//----------------------------------------------------------
/**
 * @brief	�ʐM����l��
 *
 * �z��ł��Ȃ��̂łƂ肠�����B���ۂ�5�l�m�肩�H
 */
//----------------------------------------------------------
#define	TV_COMM_MEMBER_MAX	(16)

//============================================================================================
//
//			���M�f�[�^�T�C�Y�擾
//
//============================================================================================
//----------------------------------------------------------
/**
 * @brief	�e���r�p���M�f�[�^�T�C�Y
 * @param	sv		�Z�[�u�f�[�^�ւ̃A�h���X
 * @return	u32		���M�f�[�^�̃T�C�Y
 */
//----------------------------------------------------------
u32 TVWORK_GetSendQandADataSize(const SAVEDATA * sv)
{
	return sizeof(SEND_TOPIC) * MY_QANDA_MAX;
}
//----------------------------------------------------------
/**
 * @brief	�e���r�p���M�f�[�^�T�C�Y
 * @param	sv		�Z�[�u�f�[�^�ւ̃A�h���X
 * @return	u32		���M�f�[�^�̃T�C�Y
 */
//----------------------------------------------------------
u32 TVWORK_GetSendWatchDataSize(const SAVEDATA * sv)
{
	return sizeof(SEND_TOPIC) * MY_WATCH_MAX;
}

//----------------------------------------------------------
/**
 * @brief	�e���r�p���M�f�[�^�T�C�Y
 * @param	sv		�Z�[�u�f�[�^�ւ̃A�h���X
 * @return	u32		���M�f�[�^�̃T�C�Y
 */
//----------------------------------------------------------
u32 TVWORK_GetSendRecordDataSize(const SAVEDATA * sv)
{
	return sizeof(SEND_TOPIC) * MY_RECORD_MAX;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �w��TV_DATA_TYPE�ŕ����o����g�s�b�N���`�F�b�N
 *
 * @param   data_type		TV_DATA_TYPE
 * @param   topic_id		�g�s�b�NID
 *
 * @retval  TRUE:������
 * @retval  FALSE:�����s��
 */
//--------------------------------------------------------------
static BOOL TvTopicIDCheck(TV_DATA_TYPE data_type, int topic_id)
{
	int i;
	
	for(i = 0; TvTopicID_Tbl[data_type][i] != 0xff; i++){
		if(TvTopicID_Tbl[data_type][i] == topic_id){
			return TRUE;
		}
	}
	return FALSE;
}


//============================================================================================
//
//
//			���M�f�[�^����
//
//
//============================================================================================
//----------------------------------------------------------
/**
 * @brief		�����f�[�^���瑗�M�f�[�^���쐬����
 * @param		sv		�Z�[�u�f�[�^�ւ̃A�h���X
 * @param		send	���M�f�[�^���i�[����o�b�t�@
 * @param		mt		�����f�[�^�ւ̃|�C���^
 * @param		max		�����f�[�^�̐�
 * @param		dp_mode	TRUE:DP�p�̃e���r�f�[�^���쐬�B�@FALSE:�v���`�i�ȍ~�p�̃e���r�f�[�^�쐬
 * @param		data_type	TV_DATA_TYPE(dp_mode==TRUE���̂ݎg�p)
 */
//----------------------------------------------------------
static void MakeSendTopicByMyTopic(SAVEDATA * sv, SEND_TOPIC * send,
		MY_TOPIC * mt, int max, int dp_mode, int data_type)
{
	int i;
	MYSTATUS * my = SaveData_GetMyStatus(sv);

	for (i = 0; i < max; i++, send++) {
		MI_CpuClearFast(send, sizeof(SEND_TOPIC));

		//���M�ҏ��𖄂ߍ���
		send->owner_id = MyStatus_GetID(my);
		send->sex = MyStatus_GetMySex(my);
		send->rom_ver = PM_VERSION;
		send->region_code = PM_LANG;
		PM_strncpy(send->owner_name, MyStatus_GetMyName(my), PERSON_NAME_SIZE + EOM_SIZE);

		send->topic = mt[i].topic;
		send->topic.see_count = 0;	//���M�f�[�^�̎����J�E���g�̓N���A
		if(dp_mode == TRUE && TvTopicIDCheck(data_type, send->topic.topic_id) == FALSE){
			send->topic.topic_id = 0;	//�����o���Ȃ����̂̓g�s�b�NID�����ɂ���
		}
	}
	//���M�f�[�^�𐶐������玩���f�[�^�͍폜����
	//MI_CpuClearFast(mt, sizeof(MY_TOPIC) * max);
}
//----------------------------------------------------------
/**
 * @brief	����^�g�s�b�N�����f�[�^��Alloc(DP�����e���r�f�[�^)
 * @param	sv
 * @param	headID
 * @param	size
 */
//----------------------------------------------------------
SEND_TOPIC * TVWORK_AllocSendQandAData(SAVEDATA * sv, int headID, u32 size)
{
	SEND_TOPIC * buf;
	TV_WORK * tvwk;
	buf = sys_AllocMemoryLo(headID, size);
	tvwk = SaveData_GetTvWork(sv);
	GF_ASSERT(size == TVWORK_GetSendQandADataSize(sv));
	MakeSendTopicByMyTopic(sv, buf, tvwk->MyQandATopic, MY_QANDA_MAX, TRUE, TV_DATA_TYPE_DP_QAND);
	return buf;
}
//----------------------------------------------------------
/**
 * @brief	�Ď��^�g�s�b�N�����f�[�^��Alloc(DP�����e���r�f�[�^)
 * @param	sv
 * @param	headID
 * @param	size
 */
//----------------------------------------------------------
SEND_TOPIC * TVWORK_AllocSendWatchData(SAVEDATA * sv, int headID, u32 size)
{
	SEND_TOPIC * buf;
	TV_WORK * tvwk;
	buf = sys_AllocMemoryLo(headID, size);
	tvwk = SaveData_GetTvWork(sv);
	GF_ASSERT(size == TVWORK_GetSendWatchDataSize(sv));
	MakeSendTopicByMyTopic(sv, buf, tvwk->MyWatchTopic, MY_WATCH_MAX, TRUE, TV_DATA_TYPE_DP_WATCH);
	return buf;
}
//----------------------------------------------------------
/**
 * @brief	�M�l�X�^�g�s�b�N�����f�[�^��Alloc(DP�����e���r�f�[�^)
 * @param	sv
 * @param	headID
 * @param	size
 */
//----------------------------------------------------------
SEND_TOPIC * TVWORK_AllocSendRecordData(SAVEDATA * sv, int headID, u32 size)
{
	SEND_TOPIC * buf;
	TV_WORK * tvwk;
	buf = sys_AllocMemoryLo(headID, size);
	tvwk = SaveData_GetTvWork(sv);
	GF_ASSERT(size == TVWORK_GetSendRecordDataSize(sv));
	MakeSendTopicByMyTopic(sv, buf, tvwk->MyRecordTopic, MY_RECORD_MAX, TRUE, TV_DATA_TYPE_DP_RECORD);
	return buf;
}
//----------------------------------------------------------
/**
 * @brief	����^�g�s�b�N�����f�[�^��Alloc(�v���`�i�ȍ~�e���r�f�[�^)
 * @param	sv
 * @param	headID
 * @param	size
 */
//----------------------------------------------------------
SEND_TOPIC * TVWORK_AllocSendQandADataEx(SAVEDATA * sv, int headID, u32 size)
{
	SEND_TOPIC * buf;
	TV_WORK * tvwk;
	buf = sys_AllocMemoryLo(headID, size);
	tvwk = SaveData_GetTvWork(sv);
	GF_ASSERT(size == TVWORK_GetSendQandADataSize(sv));
	MakeSendTopicByMyTopic(sv, buf, tvwk->MyQandATopic, MY_QANDA_MAX, 0, TV_DATA_TYPE_QAND);
	return buf;
}
//----------------------------------------------------------
/**
 * @brief	�Ď��^�g�s�b�N�����f�[�^��Alloc(�v���`�i�ȍ~�e���r�f�[�^)
 * @param	sv
 * @param	headID
 * @param	size
 */
//----------------------------------------------------------
SEND_TOPIC * TVWORK_AllocSendWatchDataEx(SAVEDATA * sv, int headID, u32 size)
{
	SEND_TOPIC * buf;
	TV_WORK * tvwk;
	buf = sys_AllocMemoryLo(headID, size);
	tvwk = SaveData_GetTvWork(sv);
	GF_ASSERT(size == TVWORK_GetSendWatchDataSize(sv));
	MakeSendTopicByMyTopic(sv, buf, tvwk->MyWatchTopic, MY_WATCH_MAX, 0, TV_DATA_TYPE_WATCH);
	return buf;
}
//----------------------------------------------------------
/**
 * @brief	�M�l�X�^�g�s�b�N�����f�[�^��Alloc(�v���`�i�ȍ~�e���r�f�[�^)
 * @param	sv
 * @param	headID
 * @param	size
 */
//----------------------------------------------------------
SEND_TOPIC * TVWORK_AllocSendRecordDataEx(SAVEDATA * sv, int headID, u32 size)
{
	SEND_TOPIC * buf;
	TV_WORK * tvwk;
	buf = sys_AllocMemoryLo(headID, size);
	tvwk = SaveData_GetTvWork(sv);
	GF_ASSERT(size == TVWORK_GetSendRecordDataSize(sv));
	MakeSendTopicByMyTopic(sv, buf, tvwk->MyRecordTopic, MY_RECORD_MAX, 0, TV_DATA_TYPE_RECORD);
	return buf;
}


//============================================================================================
//
//
//			��M�f�[�^����
//
//
//============================================================================================
//----------------------------------------------------------
/**
 * @brief	��M�g�s�b�N�𑼐l�g�s�b�N�̈�ɃZ�b�g����
 * @param	ot			���l�g�s�b�N�̈�ւ̃|�C���^
 * @param	max			���l�g�s�b�N�̈�̃T�C�Y
 * @param	st			��M�g�s�b�N�f�[�^
 * @param   data_type	TV_DATA_TYPE
 * @retval	TRUE		�V���ɃZ�b�g����
 * @retval	FALSE		�Z�b�g���Ȃ�����
 * @retval	-1			�����o���Ȃ��g�s�b�N�̈׃Z�b�g���Ȃ�����
 */
//----------------------------------------------------------
static BOOL AddOtherTopic(OTHER_TOPIC * ot, int max, const SEND_TOPIC * st, TV_DATA_TYPE data_type)
{
	int i;
	if (st->topic.topic_id == 0) {
		DEBUG_PRT_TAMADA("NULL topic tr_id=%d\n",st->owner_id & 0xffff);
		//��̃g�s�b�N�f�[�^�������ꍇ
		return FALSE;
	}
	if(TvTopicIDCheck(data_type, st->topic.topic_id) == FALSE){
		return -1;	//�����o���Ȃ��g�s�b�N
	}
	
	for (i = 0; i < max; i++) {
		if (ot[i].topic.topic_id == 0) {
			DEBUG_PRT_TAMADA("SET! pos=%d topic=%d tr_id=%d\n",i, st->topic.topic_id, st->owner_id & 0xffff);
			ot[i] = *st;
			return TRUE;
		}
	}
	for (i = 0; i < max; i++) {
		if (ot[i].topic.see_count >= BROADCAST_MAX) {
			DEBUG_PRT_TAMADA("SET! pos=%d topic=%d tr_id=%d\n",i, st->topic.topic_id, st->owner_id & 0xffff);
			ot[i] = *st;
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------
/**
 * @brief	�e���r��M�f�[�^�܂��鏈���{��
 * @param	ot			���l�g�s�b�N�̈�ւ̃|�C���^
 * @param	max			���l�g�s�b�N�̈�̃T�C�Y
 * @param	member		�ʐM�l��
 * @param	my_id		�����̒ʐMID
 * @param	darray		��M�f�[�^�ւ̃|�C���^�̔z��ւ̃|�C���^
 * @param	darray		DP�e���r��M�f�[�^�ւ̃|�C���^�̔z��ւ̃|�C���^
 */
//----------------------------------------------------------
static const void ** MixRecvBufferSelect(int net_id, const void ** darray, const void ** dp_darray)
{
	MYSTATUS *myst;

	myst = CommInfoGetMyStatus(net_id);
	if(myst == NULL){
		return NULL;	//�����̗���I�ɂ��肦�Ȃ����ǈꉞ
	}
	if(MyStatus_GetRomCode(myst) == POKEMON_DP_ROM_CODE){
		return dp_darray;
	}
	return darray;
}

static void MixRecvData(OTHER_TOPIC * ot, int max, int member, int my_id, const void ** darray, const void ** dp_darray, TV_DATA_TYPE data_type)
{
	u8 count[TV_COMM_MEMBER_MAX];
	int pos, i, j;
	const void **buffer;
	
	DEBUG_PRT_TAMADA("OT=%08x max=%2d my_id=%5d\n", ot, max, my_id);
	for (i = 0; i < member; i++) {
		DEBUG_PRT_TAMADA("ID[%d]%08x\n", i, darray[i]);
	}

	MI_CpuClear8(count, TV_COMM_MEMBER_MAX);

	pos = 0;
	for (i = 0; i < member; i++) {
		if (darray[i] == NULL) { continue; }//���Ȃ��ꍇ�͎��̐l��
		for (j = 0; j < TV_RECV_MAX; j ++) {
			while (darray[pos] == NULL || pos == i || count[pos] == TV_RECV_MAX) {
				//���Ȃ��l�Ǝ����Ƃ��łɂ��炢�����̐l�̓p�X
				pos ++;
				pos %= member;
			}
			count[pos] ++;
			DEBUG_PRT_TAMADA(" %d[%d]->%d[%d]",i,j, pos, count[pos]);
			if (pos != my_id) {
				//�l�̃f�[�^�̏ꍇ�A�ǉ�����
				buffer = MixRecvBufferSelect(pos, darray, dp_darray);
				if(buffer != NULL){
					AddOtherTopic(ot, max, &((SEND_TOPIC *)buffer[pos])[j], data_type);
				}
			}
		}
		DEBUG_PRT_TAMADA("\n");
	}
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_TVWORK)
	SVLD_SetCrc(GMDATA_ID_TVWORK);
#endif //CRC_LOADCHECK
}

//----------------------------------------------------------
/**
 * @brief	�e���r�p�f�[�^�����鏈���i�Ď��^�jDP����
 * @param	tvwk			�e���r�f�[�^�ւ̃|�C���^
 * @param	member		�ʐM�l��
 * @param	my_id		�����̒ʐMID
 * @param	darray		��M�f�[�^�ւ̃|�C���^�̔z��ւ̃|�C���^
 * @param	dp_darray	DP�e���r��M�f�[�^�ւ̃|�C���^�̔z��ւ̃|�C���^
 *
 */
//----------------------------------------------------------
void TVWORK_MixWatchData(TV_WORK * tv, int member, int my_id, const void ** darray, const void ** dp_darray)
{
	MixRecvData(tv->OtherWatchTopic, OTHER_WATCH_MAX, member, my_id, darray, dp_darray, TV_DATA_TYPE_WATCH);
}

//----------------------------------------------------------
/**
 * @brief	�e���r�p�f�[�^�����鏈���i����^�jDP����
 * @param	tvwk			�e���r�f�[�^�ւ̃|�C���^
 * @param	member		�ʐM�l��
 * @param	my_id		�����̒ʐMID
 * @param	darray		��M�f�[�^�ւ̃|�C���^�̔z��ւ̃|�C���^
 * @param	dp_darray	DP�e���r��M�f�[�^�ւ̃|�C���^�̔z��ւ̃|�C���^
 *
 */
//----------------------------------------------------------
void TVWORK_MixRecordData(TV_WORK * tv, int member, int my_id, const void ** darray, const void ** dp_darray)
{
	MixRecvData(tv->OtherRecordTopic, OTHER_RECORD_MAX, member, my_id, darray, dp_darray, TV_DATA_TYPE_RECORD);
}

//----------------------------------------------------------
/**
 * @brief	�e���r�p�f�[�^�܂��鏈���i����^�jDP����
 * @param	tvwk			�e���r�f�[�^�ւ̃|�C���^
 * @param	member		�ʐM�l��
 * @param	my_id		�����̒ʐMID
 * @param	darray		��M�f�[�^�ւ̃|�C���^�̔z��ւ̃|�C���^
 * @param	dp_darray	DP�e���r��M�f�[�^�ւ̃|�C���^�̔z��ւ̃|�C���^
 *
 */
//----------------------------------------------------------
void TVWORK_MixQandAData(TV_WORK * tv, int member, int my_id, const void ** darray, const void ** dp_darray)
{
	MixRecvData(tv->OtherQandATopic, OTHER_QANDA_MAX, member, my_id, darray, dp_darray, TV_DATA_TYPE_QAND);
}


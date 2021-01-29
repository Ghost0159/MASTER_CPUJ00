//=============================================================================
/**
 * @file	comm_command_frontier.c
 * @brief	�t�����e�B�A�@�f�[�^�𑗂邽�߂̃R�}���h���e�[�u����
 *          comm_command_frontier.h �� enum �Ɠ������тł���K�v������܂�
 * @author	nohara
 * @date    2007.07.17
 */
//=============================================================================
#include "common.h"
#include "communication/communication.h"
#include "system/pms_data.h"
#include "savedata/b_tower.h"
#include "savedata/frontier_savedata.h"
#include "battle/battle_common.h"
#include "gflib/strbuf_family.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_tool.h"

#include "comm_command_frontier.h"
#include "frontier_tool.h"
#include "wifi_counter_tool.h"
#include "factory_tool.h"
#include "castle_tool.h"
#include "castle_def.h"

//��M�֐����O�ɂ���
#include "factory/factory_sys.h"
#include "stage/stage_sys.h"
#include "castle/castle_sys.h"
#include "roulette/roulette_sys.h"
//#include "../field/scr_stage_sub.h"
//#include "../field/scr_castle_sub.h"
//#include "../field/scr_roulette_sub.h"
#include "../field/fieldobj.h"
#include "../field/scr_tool.h"

#include "frontier_comm.h"


//==============================================================================
//
//  �e�[�u���ɏ����֐��̒�`
//
//==============================================================================
//�^���[(�t�B�[���h�AWiFi)
static void CommFrWiFiCounterTowerRecvBufPlayerData(int id_no,int size,void *pData,void *work);
BOOL CommFrWiFiCounterTowerSendBufTrainerData( BTOWER_SCRWORK* wk );
static void CommFrWiFiCounterTowerRecvBufTrainerData(int id_no,int size,void *pData,void *work);
static void CommFrWiFiCounterTowerRecvBufRetireSelect(int id_no,int size,void *pData,void *work);

#if 1
BOOL CommFrWiFiCounterSendBufBFNo( FRWIFI_SCRWORK* wk );
void CommFrWiFiCounterRecvBufBFNo(int id_no,int size,void *pData,void *work);
BOOL CommFrWiFiCounterSendBufRetireFlag( FRWIFI_SCRWORK* wk, u16 retire_flag );
void CommFrWiFiCounterRecvBufRetireFlag(int id_no,int size,void *pData,void *work);
BOOL CommFrWiFiCounterSendBufMonsNoItemNo( FRWIFI_SCRWORK* wk, u16 pos1, u16 pos2 );
void CommFrWiFiCounterRecvBufMonsNoItemNo(int id_no,int size,void *pData,void *work);
BOOL CommFrWiFiCounterSendBufStageRecordDel( FRWIFI_SCRWORK* wk, u16 stage_del_flag );
void CommFrWiFiCounterRecvBufStageRecordDel(int id_no,int size,void *pData,void *work);
BOOL CommFrWiFiCounterSendBufGameContinue( FRWIFI_SCRWORK* wk, u16 flag );
void CommFrWiFiCounterRecvBufGameContinue(int id_no,int size,void *pData,void *work);
#endif


//==============================================================================
//
//	�e�[�u���錾
//
//  comm_shar.h �� enum �Ɠ����Ȃ�тɂ��Ă�������
//  CALLBACK���Ă΂ꂽ���Ȃ��ꍇ��NULL�������Ă�������
//  �R�}���h�̃T�C�Y��Ԃ��֐��������Ă��炦��ƒʐM���y���Ȃ�܂�
//  _getZero�̓T�C�Y�Ȃ���Ԃ��܂��B_getVariable�͉σf�[�^�g�p���Ɏg���܂�
//
//==============================================================================
static const CommPacketTbl _CommPacketTbl[] = {
	//�t�@�N�g���[(�t�����e�B�A)
	{CommFactoryRecvBasicData,		_getVariable,	NULL},						//
	{CommFactoryRecvTrData,			_getVariable,	NULL},						// 
	{CommFactoryRecvRentalData,		_getVariable,	NULL},						// 
	{CommFactoryRecvEnemyPokeData,	_getVariable,	NULL},						//
	{CommFactoryRecvRetireFlag,		_getVariable,	NULL},						//
	{CommFactoryRecvTradeYesNoFlag,	_getVariable,	NULL},						//
	{CommFactoryRecvTemotiPokeData,	_getVariable,	NULL},						//
	
	//�t�@�N�g���[(�����^���A�������)
	{Factory_CommRecvBufBasicData,	_getVariable,	NULL},						//
	{Factory_CommRecvBufRentalData,	_getVariable,	NULL},						//
	{Factory_CommRecvBufCancelData,	_getVariable,	NULL},						//
	{Factory_CommRecvBufTradeFlag,	_getVariable,	NULL},						//

	//�X�e�[�W(�t�����e�B�A)
	{CommStageRecvMonsNo,			_getVariable,	NULL},						//
	{CommStageRecvBasicData,		_getVariable,	NULL},						//
	{CommStageRecvTrData,			_getVariable,	NULL},						//
	{CommStageRecvEnemyPokeData,	_getVariable,	NULL},						//
	{CommStageRecvRetireFlag,		_getVariable,	NULL},						//
	{CommStageRecvBufTemotiPokeData,_getVariable,	CommStageGetRecvDataBuff},	//����f�[�^

	//�X�e�[�W(�^�C�v�I�����)
	{Stage_CommRecvBufBasicData,	_getVariable,	NULL},						//
	{Stage_CommRecvBufPokeTypeData,	_getVariable,	NULL},						//
	{Stage_CommRecvBufFinalAnswerData,_getVariable,	NULL},						//

	//�L���b�X��(�t�����e�B�A)
	{CommCastleRecvBufBasicData,	_getVariable,	NULL},						//
	{CommCastleRecvBufTrData,		_getVariable,	NULL},						// 
	{CommCastleRecvBufSelData,		_getVariable,	NULL},						// 
	{CommCastleRecvBufEnemyPokeData,_getVariable,	NULL},						//
	{CommCastleRecvBufRetireFlag,	_getVariable,	NULL},						//
	{CommCastleRecvBufTradeYesNoFlag,_getVariable,	NULL},						//
	//{CommCastleRecvBufTemotiPokeData,_getVariable,	NULL},						//
	{CommCastleRecvBufTemotiPokeData,_getVariable,	CommCastleGetRecvDataBuff},	//����f�[�^

	//�L���b�X��(�莝���|�P�������)
	{CastleMine_CommRecvBufBasicData,	_getVariable,	NULL},						//
	{CastleMine_CommRecvBufRankUpType,	_getVariable,	NULL},						//
	{CastleMine_CommRecvBufCsrPos,		_getVariable,	NULL},						//
	{CastleMine_CommRecvBufModoru,		_getVariable,	NULL},						//
	
	//�L���b�X��(�G�g���[�i�[���)
	{CastleEnemy_CommRecvBufBasicData,	_getVariable,	NULL},						//
	{CastleEnemy_CommRecvBufRankUpType,	_getVariable,	NULL},						//
	{CastleEnemy_CommRecvBufCsrPos,		_getVariable,	NULL},						//
	{CastleEnemy_CommRecvBufModoru,		_getVariable,	NULL},						//
	
	//�t�����e�B�AWiFi��t(�t�����e�B�A)
	{CommFrWiFiCounterRecvBufBFNo,		_getVariable,	NULL},						//
	{CommFrWiFiCounterRecvBufRetireFlag,_getVariable,	NULL},						//
	{CommFrWiFiCounterRecvBufMonsNoItemNo,_getVariable,	NULL},						//
	{CommFrWiFiCounterRecvBufStageRecordDel,_getVariable,	NULL},					//
	{CommFrWiFiCounterRecvBufGameContinue,_getVariable,	NULL},						//

	//�^���[(�t�B�[���h�AWifi)
	{CommFrWiFiCounterTowerRecvBufPlayerData,_getVariable,	NULL},						//
	{CommFrWiFiCounterTowerRecvBufTrainerData,_getVariable,	NULL},						//
	{CommFrWiFiCounterTowerRecvBufRetireSelect,_getVariable,	NULL},					//

	//���[���b�g(�t�����e�B�A)
	{CommRouletteRecvBufBasicData,	_getVariable,	NULL},						//
	{CommRouletteRecvBufTrData,		_getVariable,	NULL},						// 
	{CommRouletteRecvBufEnemyPokeData,_getVariable,	NULL},						//
	{CommRouletteRecvBufRetireFlag,	_getVariable,	NULL},						//
	{CommRouletteRecvBufTradeYesNoFlag,_getVariable,	NULL},						//
	//{CommRouletteRecvBufTemotiPokeData,_getVariable,	NULL},						//
	{CommRouletteRecvBufTemotiPokeData,_getVariable,	CommRouletteGetRecvDataBuff},	//����f�[�^

	//���[���b�g(�����N�A�b�v���)
	{Roulette_CommRecvBufBasicData,	_getVariable,	NULL},						//
	{Roulette_CommRecvBufRankUpType,	_getVariable,	NULL},						//
	{Roulette_CommRecvBufCsrPos,		_getVariable,	NULL},						//

};

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�p�̃R�}���h�̌n�ɏ��������܂�
 *
 * @param   none
 *
 * @retval  none
 */
//--------------------------------------------------------------
void CommCommandFrontierInitialize(void* pWork)
{
    int length = sizeof(_CommPacketTbl)/sizeof(CommPacketTbl);
    CommCommandInitialize(_CommPacketTbl, length, pWork);
}


//==============================================================================
//
//	�^���[(�t�B�[���h,WiFi)
//
//==============================================================================
	
//--------------------------------------------------------------
/**
 * @brief	recv_buf�̃o�g���^���[�����Ă����v���C���[�f�[�^���擾
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CommFrWiFiCounterTowerRecvBufPlayerData(int id_no,int size,void *pData,void *work)
{
	u16	ret;
	int num;
	BTOWER_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******WIFI��t****** �o�g���^���[�@�v���C���[�f�[�^����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	ret = 0;
	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	wk->pare_sex = (u8)recv_buf[0];
	wk->pare_poke[0] = recv_buf[1];
	wk->pare_poke[1] = recv_buf[2];
	wk->pare_stage = recv_buf[3];

	wk->partner = 5+wk->pare_sex;

	OS_Printf("sio multi mem = %d,%d:%d,%d\n",wk->mem_poke[0],wk->mem_poke[1],wk->pare_poke[0],wk->pare_poke[1]);
	if(	wk->mem_poke[0] == wk->pare_poke[0] ||
		wk->mem_poke[0] == wk->pare_poke[1]){
		ret += 1;
	}
	if(	wk->mem_poke[1] == wk->pare_poke[0] ||
		wk->mem_poke[1] == wk->pare_poke[1]){
		ret += 2;
	}

	wk->check_work = ret;
	return;
}
	
//--------------------------------------------------------------
/**
 * @brief	���I�����g���[�i�[No���q�@�ɑ��M
 *
 * @param	wk			BTOWER_SCRWORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
BOOL CommFrWiFiCounterTowerSendBufTrainerData( BTOWER_SCRWORK* wk )
{
	int ret,size;

	size = (BTOWER_STOCK_TRAINER_MAX * 2);

	OS_Printf( "******WIFI��t****** �o�g���^���[�@�g���[�i�[No�𑗐M����\n" );

	MI_CpuCopy8( wk->trainer, wk->send_buf, size );

	if( CommSendData(FC_TOWER_TR_DATA,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̃o�g���^���[�����Ă����g���[�i�[�f�[�^���擾
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CommFrWiFiCounterTowerRecvBufTrainerData(int id_no,int size,void *pData,void *work)
{
	int num;
	BTOWER_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******WIFI��t****** �o�g���^���[�@�g���[�i�[No����M����\n" );
	OS_Printf( "id_no = %d\n", id_no );
	OS_Printf( "����id = %d\n", CommGetCurrentID() );

	num = 0;
	wk->recieve_count++;
	OS_Printf( "wk->recieve_count = %d\n", wk->recieve_count );

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		return;
	}

	MI_CpuCopy8( recv_buf, wk->trainer, BTOWER_STOCK_TRAINER_MAX*2 );
	OS_Printf(	"sio multi trainer01 = %d,%d:%d,%d\n",
				wk->trainer[0], wk->trainer[1], wk->trainer[2], wk->trainer[3] );
	OS_Printf(	"sio multi trainer02 = %d,%d:%d,%d\n",
				wk->trainer[4], wk->trainer[5], wk->trainer[6], wk->trainer[7] );
	OS_Printf(	"sio multi trainer03 = %d,%d:%d,%d\n",
				wk->trainer[8], wk->trainer[9], wk->trainer[10], wk->trainer[11] );
	OS_Printf(	"sio multi trainer04 = %d,%d\n",
				wk->trainer[12], wk->trainer[13] );
	return;
}


//--------------------------------------------------------------
/**
 * @brief	recv_buf�̃o�g���^���[�����Ă������^�C�A���邩�ǂ����̌��ʂ��擾
 *
 * @param	
 *
 * @retval	0	���^�C�A���Ȃ�
 * @retval	1	���^�C�A����
 */
//--------------------------------------------------------------
static void CommFrWiFiCounterTowerRecvBufRetireSelect(int id_no,int size,void *pData,void *work)
{
	int num;
	BTOWER_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******WIFI��t****** �o�g���^���[�@�g���[�i�[No����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->check_work = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf("sio multi retire = %d,%d\n",wk->retire_f,recv_buf[0]);
	if( wk->retire_f || recv_buf[0] ){
		wk->check_work = 1;
	}

	return;
}


//==============================================================================
//
//	�t�����e�B�AWiFi��t
//
//==============================================================================
#if 1
//--------------------------------------------------------------
/**
 * @brief	send_buf�ɑI�������{��No���Z�b�g
 *
 * @param	wk		FRWIFI_SCRWORK�^�̃|�C���^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommFrWiFiCounterSendBufBFNo( FRWIFI_SCRWORK* wk )
{
	int ret,size,id;

	OS_Printf( "******WIFI��t****** �I�������{��No���M\n" );

	size = FRWIFI_COMM_BUF_LEN;									//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	wk->send_buf[0]	= wk->bf_no;								//�{��No

#if 1
	//�A�������擾
	id = WifiCounterRensyouRecordIDGet( wk->bf_no );
	wk->send_buf[1] = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), id, 
											Frontier_GetFriendIndex(id) );
#endif

	if( CommSendData(FC_WIFI_COUNTER_NO,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̑I�������{��No���擾
 *
 * @param	wk			FRWIFI_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 */
//--------------------------------------------------------------
void CommFrWiFiCounterRecvBufBFNo(int id_no,int size,void *pData,void *work)
{
	u16 id;
	FRWIFI_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******WIFI��t****** �I�������{��No��M\n" );

	wk->recieve_count++;
	//OS_Printf( "wk->recieve_count = %d\n", wk->recieve_count );

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	wk->pair_bf_no = (u8)recv_buf[0];							//�{��No
	OS_Printf( "��M�Fwk->pair_bf_no = %d\n", wk->pair_bf_no );

	//�A�������擾
	wk->pair_rensyou = recv_buf[1];								//�A����
	OS_Printf( "��M�Fwk->pair_rensyou = %d\n", wk->pair_rensyou );
	return;
}


//--------------------------------------------------------------
/**
 * @brief	send_buf�ɑI�����ŉ���I�񂾂����Z�b�g
 *
 * @param	wk		FRWIFI_SCRWORK�^�̃|�C���^
 * @param	sel		0=�Â���A0�ȊO=���^�C��(���낭����͂���Ȃ�)
 *
 * @return	none
 *
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
BOOL CommFrWiFiCounterSendBufRetireFlag( FRWIFI_SCRWORK* wk, u16 retire_flag )
{
	int ret,size;

	size = FRWIFI_COMM_BUF_LEN;								//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	OS_Printf( "******WIFI��t****** ���^�C�A���M\n" );

	wk->send_buf[0] = retire_flag;
	OS_Printf( "���M�Fretire_flag = %d\n", wk->send_buf[0] );

	if( CommSendData(FC_WIFI_COUNTER_RETIRE_FLAG,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̑I�����ŉ���I�񂾂����擾
 *
 * @param	wk			FRWIFI_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
void CommFrWiFiCounterRecvBufRetireFlag(int id_no,int size,void *pData,void *work)
{
	int num;
	FRWIFI_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******WIFI��t****** ���^�C�A��M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	wk->pair_retire_flag = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_retire_flag = %d\n", wk->pair_retire_flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ƀ����X�^�[�i���o�[���Z�b�g
 *
 * @param	wk		FRWIFI_SCRWORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
BOOL CommFrWiFiCounterSendBufMonsNoItemNo( FRWIFI_SCRWORK* wk, u16 pos1, u16 pos2 )
{
	int ret,size;
	POKEMON_PARAM* pp;
	POKEPARTY* party;

	size = FRWIFI_COMM_BUF_LEN;								//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	OS_Printf( "******WIFI��t****** �����X�^�[�i���o�[���M\n" );

	party = SaveData_GetTemotiPokemon( wk->sv );

	OS_Printf( "pos1 = %d\n", pos1 );
	OS_Printf( "pos2 = %d\n", pos2 );
	wk->mine_poke_pos[0] = pos1;
	wk->mine_poke_pos[1] = pos2;
	
	//�|�P�������X�g���L�����Z�����Ă�����
	if( pos1 == 0xff ){
		wk->monsno[0] = 0;
		wk->itemno[0] = 0;
		wk->monsno[1] = 0;
		wk->itemno[1] = 0;
	}else{
		pp = PokeParty_GetMemberPointer( party, pos1 );
		wk->monsno[0] = PokeParaGet( pp, ID_PARA_monsno, NULL );
		wk->itemno[0] = PokeParaGet( pp, ID_PARA_item, NULL );

		pp = PokeParty_GetMemberPointer( party, pos2 );
		wk->monsno[1] = PokeParaGet( pp, ID_PARA_monsno, NULL );
		wk->itemno[1] = PokeParaGet( pp, ID_PARA_item, NULL );
	}

	wk->send_buf[0] = wk->monsno[0];
	wk->send_buf[1] = wk->itemno[0];
	OS_Printf( "���M�Fmonsno1 = %d\n", wk->send_buf[0] );
	OS_Printf( "���M�Fitemno1 = %d\n", wk->send_buf[1] );

	////////////////////////////////////////////////////////////
	wk->send_buf[2] = wk->monsno[1];
	wk->send_buf[3] = wk->itemno[1];
	OS_Printf( "���M�Fmonsno2 = %d\n", wk->send_buf[2] );
	OS_Printf( "���M�Fitemno2 = %d\n", wk->send_buf[3] );

	if( CommSendData(FC_WIFI_COUNTER_MONSNO_ITEMNO,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̃����X�^�[�i���o�[���擾
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommFrWiFiCounterRecvBufMonsNoItemNo(int id_no,int size,void *pData,void *work)
{
	FRWIFI_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******WIFI��t****** �����X�^�[�i���o�[��M\n" );

	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//����̎莝���̃|�P�����i���o�[���擾(�y�A���[�N�ɑ��)
	wk->pair_monsno[0] = recv_buf[0];
	wk->pair_itemno[0] = recv_buf[1];
	wk->pair_monsno[1] = recv_buf[2];
	wk->pair_itemno[1] = recv_buf[3];
	OS_Printf( "��M�Fwk->pair_monsno[0] = %d\n", wk->pair_monsno[0] );
	OS_Printf( "��M�Fwk->pair_itemno[0] = %d\n", wk->pair_itemno[0] );
	OS_Printf( "��M�Fwk->pair_monsno[1] = %d\n", wk->pair_monsno[1] );
	OS_Printf( "��M�Fwk->pair_itemno[1] = %d\n", wk->pair_itemno[1] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�ɃX�e�[�W�̋L�^�������Ă悢�����Z�b�g
 *
 * @param	wk		FRWIFI_SCRWORK�^�̃|�C���^
 * @param	sel		0=�Â���A0�ȊO=���^�C��(���낭����͂���Ȃ�)
 *
 * @return	none
 *
 * �����A�ǂ��炩���u�������v�̎��͒��~
 */
//--------------------------------------------------------------
BOOL CommFrWiFiCounterSendBufStageRecordDel( FRWIFI_SCRWORK* wk, u16 stage_del_flag )
{
	int ret,size;

	size = FRWIFI_COMM_BUF_LEN;								//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	OS_Printf( "******WIFI��t****** �X�e�[�W�L�^�����Ă悢�����M\n" );

	wk->send_buf[0] = stage_del_flag;
	OS_Printf( "���M�Fstage_del_flag = %d\n", wk->send_buf[0] );

	if( CommSendData(FC_WIFI_COUNTER_STAGE_RECORD_DEL,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̃X�e�[�W�̋L�^�������Ă悢�����擾
 *
 * @param	wk			FRWIFI_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * �����A�ǂ��炩���u�������v�̎��͒��~
 */
//--------------------------------------------------------------
void CommFrWiFiCounterRecvBufStageRecordDel(int id_no,int size,void *pData,void *work)
{
	int num;
	FRWIFI_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******WIFI��t****** �X�e�[�W�L�^�����Ă悢����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	wk->pair_stage_del = recv_buf[0];
	OS_Printf( "��M�Fwk->pair_stage_del = %d\n", wk->pair_stage_del );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɉ��������V�Ԃ����Z�b�g
 *
 * @param	wk		FRWIFI_SCRWORK�^�̃|�C���^
 * @param	sel		0=�͂��A1=������
 *
 * @return	none
 *
 * �����A�ǂ��炩���u�������v�̎��͒��~
 */
//--------------------------------------------------------------
BOOL CommFrWiFiCounterSendBufGameContinue( FRWIFI_SCRWORK* wk, u16 flag )
{
	int ret,size;

	size = FRWIFI_COMM_BUF_LEN;								//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	OS_Printf( "******WIFI��t****** ���������V�Ԃ��𑗐M\n" );

	wk->send_buf[0] = flag;
	OS_Printf( "���M�Fgame_continue = %d\n", wk->send_buf[0] );

	if( CommSendData(FC_WIFI_COUNTER_GAME_CONTINUE,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̈��������V�Ԃ����擾
 *
 * @param	wk			FRWIFI_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * �����A�ǂ��炩���u�������v�̎��͒��~
 */
//--------------------------------------------------------------
void CommFrWiFiCounterRecvBufGameContinue(int id_no,int size,void *pData,void *work)
{
	int num;
	FRWIFI_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******WIFI��t****** ���������V�Ԃ�����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	wk->pair_game_continue = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_game_continue = %d\n", wk->pair_game_continue );
	return;
}
#endif



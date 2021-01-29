//=============================================================================
/**
 * @file	frontier_comm.c
 * @brief	comm_command_frontier.c����Ă΂��ʐM����(��풆)
 * @author	nohara
 * @date    2007.12.05
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
#include "factory_tool.h"
#include "castle_tool.h"
#include "castle_def.h"

//��M�֐����O�ɂ���
#include "factory/factory_sys.h"
#include "stage/stage_sys.h"
#include "castle/castle_sys.h"
#include "roulette/roulette_sys.h"
#include "../field/scr_stage_sub.h"
#include "../field/scr_castle_sub.h"
#include "../field/scr_roulette_sub.h"
#include "../field/fieldobj.h"
#include "../field/scr_tool.h"


//==============================================================================
//
//  �e�[�u���ɏ����֐��̒�`
//
//==============================================================================
//�t�@�N�g���[
BOOL		CommFactorySendBasicData( FACTORY_SCRWORK* wk );
void CommFactoryRecvBasicData(int id_no,int size,void *pData,void *work);
BOOL		CommFactorySendTrData( FACTORY_SCRWORK* wk );
void CommFactoryRecvTrData(int id_no,int size,void *pData,void *work);
BOOL		CommFactorySendRentalData( FACTORY_SCRWORK* wk );
void CommFactoryRecvRentalData(int id_no,int size,void *pData,void *work);
BOOL		CommFactorySendEnemyPokeData( FACTORY_SCRWORK* wk );
void CommFactoryRecvEnemyPokeData(int id_no,int size,void *pData,void *work);
BOOL		CommFactorySendRetireFlag( FACTORY_SCRWORK* wk, u8 retire_flag );
void CommFactoryRecvRetireFlag(int id_no,int size,void *pData,void *work);
BOOL		CommFactorySendTradeYesNoFlag( FACTORY_SCRWORK* wk, u8 trade_yesno_flag );
void CommFactoryRecvTradeYesNoFlag(int id_no,int size,void *pData,void *work);
BOOL		CommFactorySendTemotiPokeData( FACTORY_SCRWORK* wk );
void CommFactoryRecvTemotiPokeData(int id_no,int size,void *pData,void *work);

//�X�e�[�W
BOOL		CommStageSendMonsNo( STAGE_SCRWORK* wk, const POKEMON_PARAM* pp );
void CommStageRecvMonsNo(int id_no,int size,void *pData,void *work);
BOOL		CommStageSendBasicData( STAGE_SCRWORK* wk );
void CommStageRecvBasicData(int id_no,int size,void *pData,void *work);
BOOL		CommStageSendTrData( STAGE_SCRWORK* wk );
void CommStageRecvTrData(int id_no,int size,void *pData,void *work);
BOOL		CommStageSendEnemyPokeData( STAGE_SCRWORK* wk );
void CommStageRecvEnemyPokeData(int id_no,int size,void *pData,void *work);
BOOL		CommStageSendRetireFlag( STAGE_SCRWORK* wk, u8 retire_flag );
void CommStageRecvRetireFlag(int id_no,int size,void *pData,void *work);
BOOL		CommStageSendBufTemotiPokeData( STAGE_SCRWORK* wk );
void CommStageRecvBufTemotiPokeData(int id_no,int size,void *pData,void *work);
u8* CommStageGetRecvDataBuff( int netID, void*pWork, int size );

//�L���b�X��
BOOL		CommCastleSendBufBasicData( CASTLE_SCRWORK* wk );
void CommCastleRecvBufBasicData(int id_no,int size,void *pData,void *work);
BOOL		CommCastleSendBufTrData( CASTLE_SCRWORK* wk );
void CommCastleRecvBufTrData(int id_no,int size,void *pData,void *work);
BOOL		CommCastleSendBufSelData( CASTLE_SCRWORK* wk );
void CommCastleRecvBufSelData(int id_no,int size,void *pData,void *work);
BOOL		CommCastleSendBufEnemyPokeData( CASTLE_SCRWORK* wk );
void CommCastleRecvBufEnemyPokeData(int id_no,int size,void *pData,void *work);
BOOL		CommCastleSendBufRetireFlag( CASTLE_SCRWORK* wk, u16 retire_flag );
void CommCastleRecvBufRetireFlag(int id_no,int size,void *pData,void *work);
BOOL		CommCastleSendBufTradeYesNoFlag( CASTLE_SCRWORK* wk, u16 trade_yesno_flag );
void CommCastleRecvBufTradeYesNoFlag(int id_no,int size,void *pData,void *work);
BOOL		CommCastleSendBufTemotiPokeData( CASTLE_SCRWORK* wk );
void CommCastleRecvBufTemotiPokeData(int id_no,int size,void *pData,void *work);
u8* CommCastleGetRecvDataBuff( int netID, void*pWork, int size );

//WiFi��t
#if 0
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

//���[���b�g
BOOL		CommRouletteSendBufBasicData( ROULETTE_SCRWORK* wk );
void CommRouletteRecvBufBasicData(int id_no,int size,void *pData,void *work);
BOOL		CommRouletteSendBufTrData( ROULETTE_SCRWORK* wk );
void CommRouletteRecvBufTrData(int id_no,int size,void *pData,void *work);
BOOL		CommRouletteSendBufEnemyPokeData( ROULETTE_SCRWORK* wk );
void CommRouletteRecvBufEnemyPokeData(int id_no,int size,void *pData,void *work);
BOOL		CommRouletteSendBufRetireFlag( ROULETTE_SCRWORK* wk, u16 retire_flag );
void CommRouletteRecvBufRetireFlag(int id_no,int size,void *pData,void *work);
BOOL		CommRouletteSendBufTradeYesNoFlag( ROULETTE_SCRWORK* wk, u16 trade_yesno_flag );
void CommRouletteRecvBufTradeYesNoFlag(int id_no,int size,void *pData,void *work);
BOOL		CommRouletteSendBufTemotiPokeData( ROULETTE_SCRWORK* wk );
void CommRouletteRecvBufTemotiPokeData(int id_no,int size,void *pData,void *work);
u8* CommRouletteGetRecvDataBuff( int netID, void*pWork, int size );


//==============================================================================
//
//	�X�e�[�W
//
//	�\���̂������āA���̃T�C�Y�A��M�͂��̌^�ő���ɕύX�\��B�B�B
//	
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   �|�P�����i���o�[���M����(�e�X�g�p)
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommStageSendMonsNo( STAGE_SCRWORK* wk, const POKEMON_PARAM* pp )
{
	//u8 *buf;
	u16 *buf;
	int size;
	int ret;

	size = 2;
	//buf = sys_AllocMemory( HEAPID_WORLD, size );
	buf = wk->buf;

	OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );

	if( CommGetCurrentID() == 0 ){
		buf[0] = 100;
	}else{
		buf[0] = 200;
	}

	if( CommSendData(FC_STAGE_MONSNO,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	//sys_FreeMemoryEz( buf );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�����i���o�[�ʐM��M����(�e�X�g�p)
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommStageRecvMonsNo(int id_no,int size,void *pData,void *work)
{
	STAGE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "id_no = %d\n", id_no );
	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );
	//MI_CpuCopy8(pData, consys->c_game.pp[breeder_no], pp_size);
	return;
}

//--------------------------------------------------------------
/**
 * @brief   ��{��񑗐M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommStageSendBasicData( STAGE_SCRWORK* wk )
{
	int i,num;
	MYSTATUS* my;
	//u8 *buf;
	u16 *buf;
	int ret,size;

	OS_Printf( "******�X�e�[�W****** ��{��񑗐M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );

	size = STAGE_COMM_BUF_LEN;							//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;
	num = 0;
	my	= SaveData_GetMyStatus( wk->sv );

	//buf[0] = ;
	num += 1;												//1

	//for( i=0; i < (PERSON_NAME_SIZE + EOM_SIZE) ;i++ ){
	num += (PERSON_NAME_SIZE + EOM_SIZE);					//9

	if( CommSendData(FC_STAGE_BASIC_DATA,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��{��� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 *
 * �o�g���X�e�[�W���[�N���m�ی�A�ŏ��Ɏ󂯎��f�[�^
 */
//--------------------------------------------------------------
void CommStageRecvBasicData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	STAGE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�X�e�[�W****** ��{����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//wk-> = ;
	num += 1;														//1

	//���O
	//for( i=0; i < (PERSON_NAME_SIZE + EOM_SIZE) ;i++ ){
	num += (PERSON_NAME_SIZE + EOM_SIZE);							//9

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �g���[�i�[��񑗐M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommStageSendTrData( STAGE_SCRWORK* wk )
{
	POKEPARTY* party;
	POKEMON_PARAM* poke;
	int i,num;
	u16 *buf;
	int ret,size;

	OS_Printf( "******�X�e�[�W****** �g���[�i�[��񑗐M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = STAGE_COMM_BUF_LEN;								//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;
	num = 0;

	//�G�g���[�i�[�f�[�^
	for( i=0; i < STAGE_LAP_MULTI_ENEMY_MAX ;i++ ){
		buf[i+num] = wk->tr_index[i];
		OS_Printf( "���M�Ftr_index[%d] = %d\n", i, buf[i+num] );
	}
	num += (STAGE_LAP_MULTI_ENEMY_MAX);						//10*2

	if( CommSendData(FC_STAGE_TR_DATA,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	OS_Printf( "ret = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �g���[�i�[��� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommStageRecvTrData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	STAGE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�X�e�[�W****** �g���[�i�[����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�G�g���[�i�[�f�[�^
	for( i=0; i < STAGE_LAP_MULTI_ENEMY_MAX ;i++ ){
		//wk->tr_index[i] = ((u8*)pData)[i+num];
		wk->tr_index[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}
	num += (STAGE_LAP_MULTI_ENEMY_MAX);								//10*2

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �G�|�P�����f�[�^���M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommStageSendEnemyPokeData( STAGE_SCRWORK* wk )
{
	int i,num;
	u16 *buf;
	int ret,size;
	
	OS_Printf( "******�X�e�[�W****** �G�|�P�����f�[�^���M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = STAGE_COMM_BUF_LEN;								//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;
	num = 0;

	//�|�P����index�e�[�u��
	for( i=0; i < STAGE_LAP_MULTI_ENEMY_MAX ;i++ ){
		buf[i] = wk->enemy_poke_index[i];
		OS_Printf( "���M�Fenemy_index[%d] = %d\n", i, buf[i] );
	}
	num += STAGE_LAP_MULTI_ENEMY_MAX;									//20

	if( CommSendData(FC_STAGE_ENEMY_POKE_DATA,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �G�|�P�����f�[�^ �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommStageRecvEnemyPokeData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	STAGE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�X�e�[�W****** �G�|�P�����f�[�^��M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�|�P����index�e�[�u��
	for( i=0; i < STAGE_LAP_MULTI_ENEMY_MAX ;i++ ){
		wk->enemy_poke_index[i] = recv_buf[i];
		OS_Printf( "��M�Fwk->enemy_index[%d] = %d\n", i, wk->enemy_poke_index[i] );
	}
	num += STAGE_LAP_MULTI_ENEMY_MAX;									//20

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �퓬��̃��j���[�ŉ���I�񂾂����M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param	sel				0=�Â���A0�ȊO=���^�C��(���낭����͂���Ȃ�)
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
BOOL CommStageSendRetireFlag( STAGE_SCRWORK* wk, u8 retire_flag )
{
	//u8 *buf;
	u16 *buf;
	int ret,size;
	
	OS_Printf( "******�X�e�[�W****** �퓬��̃��j���[�ŉ���I�񂾂����M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = STAGE_COMM_BUF_LEN;								//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;

	buf[0] = retire_flag;
	OS_Printf( "���M�Fretire_flag = %d\n", buf[0] );

	if( CommSendData(FC_STAGE_RETIRE_FLAG,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �퓬��̃��j���[�ŉ���I�񂾂� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
void CommStageRecvRetireFlag(int id_no,int size,void *pData,void *work)
{
	STAGE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�X�e�[�W****** �퓬��̃��j���[�ŉ���I�񂾂���M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//wk->pair_retire_flag = ((u8*)pData)[0];
	wk->pair_retire_flag = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_retire_flag = %d\n", wk->pair_retire_flag );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɏ莝���|�P�����f�[�^���Z�b�g
 *
 * @param	wk		STAGE_SCRWORK�^�̃|�C���^
 *
 * @return	none
 *
 * �e�@�A�q�@�̗����Ƃ��I�񂾎莝����n���K�v������
 */
//--------------------------------------------------------------
BOOL CommStageSendBufTemotiPokeData( STAGE_SCRWORK* wk )
{
	int i,num,ret,size,pp_size;
	POKEMON_PARAM* temp_poke;
	POKEPARTY* party;
	
	num = 0;
	size = STAGE_HUGE_BUF_LEN;

	//POKEMON_PARAM�̃T�C�Y���擾
	pp_size = PokemonParam_GetWorkSize();
	OS_Printf( "pokemon_param_size = %d\n", pp_size );

	party = SaveData_GetTemotiPokemon( wk->sv );					//�莝���p�[�e�B�擾
	temp_poke = PokeParty_GetMemberPointer( party, wk->mine_poke_pos[0] );

	MI_CpuCopy8( temp_poke, &wk->huge_buf[0], pp_size );

	OS_Printf( "���M�Fpokemon_param\n" );

	if( CommSendHugeData(FC_STAGE_TEMOTI_POKE_DATA,wk->huge_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	OS_Printf( "���M���ʁFCommSendHugeData = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̎莝���|�P�����f�[�^���擾
 *
 * @param	wk			STAGE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * �e�@�A�q�@�̗����Ƃ��I�񂾎莝����n���K�v������
 *
 * pair_rental_...���g���܂킵�Ă���
 */
//--------------------------------------------------------------
void CommStageRecvBufTemotiPokeData(int id_no,int size,void *pData,void *work)
{
	int i,num,pp_size;
	STAGE_SCRWORK* wk = work;
	const u8* recv_buf = pData;

	OS_Printf( "******�X�e�[�W****** �莝���|�P��������M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	pp_size = PokemonParam_GetWorkSize();
	MI_CpuCopy8( &recv_buf[0], wk->pair_poke, pp_size );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�̈�擾�֐�(����f�[�^�FCommSendHugeData�̎�M�p)
 *
 * @param   netID		
 * @param   pWork		
 * @param   size		
 *
 * @retval  
 */
//--------------------------------------------------------------
u8* CommStageGetRecvDataBuff( int netID, void*pWork, int size )
{
	STAGE_SCRWORK* wk = pWork;
	OS_Printf( "size = %d\n", size );
	GF_ASSERT(size <= STAGE_HUGE_BUF_LEN);
    return wk->recieve_huge_buf[netID];
}


//==============================================================================
//
//	�t�@�N�g���[
//
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   ��{��񑗐M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommFactorySendBasicData( FACTORY_SCRWORK* wk )
{
	int i,num;
	MYSTATUS* my;
	u16 *buf;
	int ret,size;

	OS_Printf( "******�t�@�N�g���[****** ��{��񑗐M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = FACTORY_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;
	num = 0;
	my	= SaveData_GetMyStatus( wk->sv );

	//buf[0] = 

	buf[1] = wk->trade_count;							//������
	buf[2] = wk->rensyou;								//���݂̘A����
	buf[3] = wk->lap;									//����
	num += 4;											//4

	OS_Printf( "____send trade_count = %d\n", buf[1] );
	OS_Printf( "____send rensyou = %d\n", buf[2] );
	OS_Printf( "____send lap = %d\n", buf[3] );

	//���O
	//for( i=0; i < (PERSON_NAME_SIZE + EOM_SIZE) ;i++ ){
	num += (PERSON_NAME_SIZE + EOM_SIZE);					//12

	if( CommSendData(FC_FACTORY_BASIC_DATA,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��{��� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommFactoryRecvBasicData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	FACTORY_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�t�@�N�g���[****** ��{����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//wk->	= ;
	//
	wk->pair_trade_count	= recv_buf[1];							//������
	OS_Printf( "��M�Fwk->pair_trade_count = %d\n", wk->pair_trade_count );
	wk->pair_rensyou		= recv_buf[2];							//���݂̘A����
	OS_Printf( "��M�Fwk->pair_rensyou = %d\n", wk->pair_rensyou );
	wk->pair_lap			= recv_buf[3];							//����
	OS_Printf( "��M�Fwk->pair_lap = %d\n", wk->pair_lap );
	num += 4;														//4

	OS_Printf( "____recv trade_count = %d\n", recv_buf[1] );
	OS_Printf( "____recv rensyou = %d\n", recv_buf[2] );
	OS_Printf( "____recv lap = %d\n", recv_buf[3] );

	//���O
	//for( i=0; i < (PERSON_NAME_SIZE + EOM_SIZE) ;i++ ){
	num += (PERSON_NAME_SIZE + EOM_SIZE);							//12

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �g���[�i�[��񑗐M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommFactorySendTrData( FACTORY_SCRWORK* wk )
{
	int i,num;
	u16 *buf;
	int ret,size;

	OS_Printf( "******�t�@�N�g���[****** �g���[�i�[��񑗐M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = FACTORY_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;
	num = 0;

	//�G�g���[�i�[�f�[�^
	for( i=0; i < FACTORY_LAP_ENEMY_MAX*2 ;i++ ){
		buf[i+num] = wk->tr_index[i];
		OS_Printf( "���M�Ftr_index[%d] = %d\n", i, buf[i+num] );
	}
	num += (FACTORY_LAP_ENEMY_MAX*2);						//14

	if( CommSendData(FC_FACTORY_TR_DATA,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	OS_Printf( "ret = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �g���[�i�[��� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommFactoryRecvTrData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	FACTORY_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�t�@�N�g���[****** �g���[�i�[����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�G�g���[�i�[�f�[�^
	for( i=0; i < FACTORY_LAP_ENEMY_MAX*2 ;i++ ){
		wk->tr_index[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}
	num += (FACTORY_LAP_ENEMY_MAX*2);								//14

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �����^�������f�[�^ ��񑗐M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommFactorySendRentalData( FACTORY_SCRWORK* wk )
{
	int i,num;
	u16 *buf;
	int ret,size;

	OS_Printf( "******�t�@�N�g���[****** �����^�������f�[�^���M\n" );
	OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = FACTORY_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;
	num = 0;

	//�|�P����index�e�[�u��
	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
		buf[i] = wk->pair_rental_poke_index[i];
		OS_Printf( "���M�Frental_index[%d] = %d\n", i, buf[i] );
	}
	num += FACTORY_RENTAL_POKE_MAX;										//6

	//�|�P�����̃p���[����
	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
		buf[i+num] = wk->pair_rental_pow_rnd[i];
		OS_Printf( "���M�Frental_pow_rnd[%d] = %d\n", i, buf[i+num] );
	}
	num += FACTORY_RENTAL_POKE_MAX;										//12
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
		//u32 rental_personal_rnd[FACTORY_RENTAL_POKE_MAX];
		buf[i+num] = (wk->pair_rental_personal_rnd[i] & 0xffff);
		buf[i+num+FACTORY_RENTAL_POKE_MAX] = 
								((wk->pair_rental_personal_rnd[i] >> 16) & 0xffff);
		OS_Printf( "���f�[�^ rental_personal_rnd[%d] = %d\n", i, wk->pair_rental_personal_rnd[i] );
		OS_Printf( "���M�Frental_personal_rnd[%d] = %d\n", i, buf[i+num] );
		OS_Printf( "���M�Frental_personal_rnd[%d] = %d\n", i+FACTORY_RENTAL_POKE_MAX, 
													buf[i+num+FACTORY_RENTAL_POKE_MAX] );
	}
	num += (FACTORY_RENTAL_POKE_MAX * 2);								//24

#if 0
	for( i=0; i < FACTORY_COMM_BUF_LEN ;i++ ){
		OS_Printf( "buf[%d] = %d\n", i, buf[i] );
	}
#endif

	if( CommSendData(FC_FACTORY_RENTAL_DATA,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	OS_Printf( "ret = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �����^�������f�[�^ �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommFactoryRecvRentalData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	POKEMON_PARAM* temp_poke;
	FACTORY_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�t�@�N�g���[****** �����^�������f�[�^��M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		OS_Printf( "id_no = %d �����̃f�[�^�͎󂯂Ƃ�Ȃ�\n", id_no );
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		OS_Printf( "id_no = %d �e�͑��M���邾���Ŏ󂯎��Ȃ�\n", id_no );
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�|�P����index�e�[�u��
	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
		//wk->pair_rental_poke_index[i] = recv_buf[i];
		//OS_Printf( "��M�Fwk->pair_rental_index[%d] = %d\n", i, wk->pair_rental_poke_index[i] );
		wk->rental_poke_index[i] = recv_buf[i];
		OS_Printf( "��M�Fwk->rental_index[%d] = %d\n", i, wk->rental_poke_index[i] );
	}
	num += FACTORY_RENTAL_POKE_MAX;										//6

	//�|�P�����̃p���[����
	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
		//wk->pair_rental_pow_rnd[i] = recv_buf[i+num];
		//OS_Printf( "��M�Fwk->pair_rental_pow_rnd[%d] = %d\n", i, wk->pair_rental_pow_rnd[i] );
		wk->rental_pow_rnd[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->rental_pow_rnd[%d] = %d\n", i, wk->rental_pow_rnd[i] );
	}
	num += FACTORY_RENTAL_POKE_MAX;										//12
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
		//u32 rental_personal_rnd[FACTORY_RENTAL_POKE_MAX];
		//wk->pair_rental_personal_rnd[i] = recv_buf[i+num];
		//wk->pair_rental_personal_rnd[i] |= (recv_buf[i+num+FACTORY_RENTAL_POKE_MAX] << 16);
		//OS_Printf( "��M�Fwk->pair_rental_personal_rnd[%d] = %d\n", i, 
		//										wk->pair_rental_personal_rnd[i] );
		wk->rental_personal_rnd[i] = recv_buf[i+num];
		wk->rental_personal_rnd[i] |= (recv_buf[i+num+FACTORY_RENTAL_POKE_MAX] << 16);

		OS_Printf( "��M�Fwk->rental_personal_rnd[%d] = %d\n", i, 
												wk->rental_personal_rnd[i] );
	}
	num += (FACTORY_RENTAL_POKE_MAX * 2);								//24

#if 0
	for( i=0; i < FACTORY_COMM_BUF_LEN ;i++ ){
		OS_Printf( "recv_buf[%d] = %d\n", i, recv_buf[i] );
	}
#endif

#if 0
	Factory_RentalPokeMake2( wk );
	//------------------------------------------------------------------
#endif

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �G�|�P�����f�[�^�@���M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommFactorySendEnemyPokeData( FACTORY_SCRWORK* wk )
{
	int i,num;
	u16 *buf;
	int ret,size;

	OS_Printf( "******�t�@�N�g���[****** �G�|�P�����f�[�^���M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = FACTORY_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;
	num = 0;

	//�|�P����index�e�[�u��
	for( i=0; i < FACTORY_ENEMY_POKE_MAX ;i++ ){
		buf[i] = wk->enemy_poke_index[i];
		OS_Printf( "���M�Fenemy_index[%d] = %d\n", i, buf[i] );
	}
	num += FACTORY_ENEMY_POKE_MAX;										//4

	//�|�P�����̃p���[����
	for( i=0; i < FACTORY_ENEMY_POKE_MAX ;i++ ){
		buf[i+num] = wk->enemy_pow_rnd[i];
		OS_Printf( "���M�Fenemy_pow_rnd[%d] = %d\n", i, buf[i+num] );
	}
	num += FACTORY_ENEMY_POKE_MAX;										//8
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < FACTORY_ENEMY_POKE_MAX ;i++ ){
		//u32 enemy_personal_rnd[FACTORY_ENEMY_POKE_MAX];
		buf[i+num] = (wk->enemy_personal_rnd[i] & 0xffff);
		buf[i+num+FACTORY_ENEMY_POKE_MAX] = 
								((wk->enemy_personal_rnd[i] >> 16) & 0xffff);
		OS_Printf( "���f�[�^ enemy_personal_rnd[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
		OS_Printf( "���M�Fenemy_personal_rnd[%d] = %d\n", i, buf[i+num] );
		OS_Printf( "���M�Fenemy_personal_rnd[%d] = %d\n", i+FACTORY_ENEMY_POKE_MAX, 
													buf[i+num+FACTORY_ENEMY_POKE_MAX] );
	}
	num += (FACTORY_ENEMY_POKE_MAX * 2);								//16

	if( CommSendData(FC_FACTORY_ENEMY_POKE_DATA,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	OS_Printf( "ret = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �G�|�P�����f�[�^ �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommFactoryRecvEnemyPokeData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	FACTORY_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�t�@�N�g���[****** �G�|�P�����f�[�^��M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�|�P����index�e�[�u��
	for( i=0; i < FACTORY_ENEMY_POKE_MAX ;i++ ){
		wk->enemy_poke_index[i] = recv_buf[i];
		OS_Printf( "��M�Fwk->enemy_index[%d] = %d\n", i, wk->enemy_poke_index[i] );
	}
	num += FACTORY_ENEMY_POKE_MAX;										//4

	//�|�P�����̃p���[����
	for( i=0; i < FACTORY_ENEMY_POKE_MAX ;i++ ){
		wk->enemy_pow_rnd[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->enemy_pow_rnd[%d] = %d\n", i, wk->enemy_pow_rnd[i] );
	}
	num += FACTORY_ENEMY_POKE_MAX;										//8
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < FACTORY_ENEMY_POKE_MAX ;i++ ){
		//u32 enemy_personal_rnd[FACTORY_ENEMY_POKE_MAX];
		wk->enemy_personal_rnd[i] = recv_buf[i+num];
		wk->enemy_personal_rnd[i] |= (recv_buf[i+num+FACTORY_ENEMY_POKE_MAX] << 16);
		OS_Printf( "��M�Fwk->enemy_personal_rnd[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
	}
	num += (FACTORY_ENEMY_POKE_MAX * 2);								//16

#if 0
	//B_TOWER_POKEMON enemy_poke[FACTORY_ENEMY_POKE_MAX];

	//�G�|�P��������x�ɐ���
	Frontier_PokemonParamCreateAll(	wk->enemy_poke, wk->enemy_poke_index, 
									wk->enemy_pow_rnd, wk->enemy_personal_rnd, NULL, 
									FACTORY_ENEMY_POKE_MAX, HEAPID_WORLD, ARC_PL_BTD_PM );
#endif

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �퓬��̃��j���[�ŉ���I�񂾂����M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param	sel				0=�Â���A0�ȊO=���^�C��(���낭����͂���Ȃ�)
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
BOOL CommFactorySendRetireFlag( FACTORY_SCRWORK* wk, u8 retire_flag )
{
	u16 *buf;
	int ret,size;
	
	OS_Printf( "******�t�@�N�g���[****** �퓬��̃��j���[�ŉ���I�񂾂��𑗐M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = FACTORY_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;

	buf[0] = retire_flag;
	OS_Printf( "���M�Fretire_flag = %d\n", buf[0] );

	if( CommSendData(FC_FACTORY_RETIRE_FLAG,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �퓬��̃��j���[�ŉ���I�񂾂� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
void CommFactoryRecvRetireFlag(int id_no,int size,void *pData,void *work)
{
	FACTORY_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�t�@�N�g���[****** �퓬��̃��j���[�ŉ���I�񂾂���M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//wk->pair_retire_flag = ((u8*)pData)[0];
	wk->pair_retire_flag = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_retire_flag = %d\n", wk->pair_retire_flag );

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �������������𑗐M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param	sel				0=�Â���A0�ȊO=���^�C��(���낭����͂���Ȃ�)
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
BOOL CommFactorySendTradeYesNoFlag( FACTORY_SCRWORK* wk, u8 trade_yesno_flag )
{
	u16 *buf;
	int ret,size;
	
	OS_Printf( "******�t�@�N�g���[****** �������������𑗐M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = FACTORY_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;

	buf[0] = trade_yesno_flag;
	OS_Printf( "���M�Ftrade_yesno_flag = %d\n", buf[0] );

	if( CommSendData(FC_FACTORY_TRADE_YESNO_FLAG,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �������������� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
void CommFactoryRecvTradeYesNoFlag(int id_no,int size,void *pData,void *work)
{
	FACTORY_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�t�@�N�g���[****** ����������������M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	wk->pair_trade_yesno_flag = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_trade_yesno_flag = %d\n", wk->pair_trade_yesno_flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �莝���|�P�����f�[�^�@���M����
 *
 * @param   wk				���[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommFactorySendTemotiPokeData( FACTORY_SCRWORK* wk )
{
	u8 m_max;
	int i,num;
	u16 *buf;
	int ret,size;
	u32 personal_rnd[2];
	u8 pow_rnd[2];
	POKEMON_PARAM* temp_poke;

	OS_Printf( "******�t�@�N�g���[****** �莝���|�P�����f�[�^���M\n" );
	//OS_Printf( "CommGetCurrentID() = %d\n", CommGetCurrentID() );
	
	size = FACTORY_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	buf = wk->buf;
	num = 0;

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Factory_GetMinePokeNum( wk->type );

	//�莝���|�P�p�[�e�B����pow_rnd,personal_rnd���擾
	//temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < m_max ;i++ ){
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, i );
		pow_rnd[i] = PokeParaGet( temp_poke, ID_PARA_pow_rnd, NULL );
		personal_rnd[i] = PokeParaGet( temp_poke, ID_PARA_personal_rnd, NULL );
		OS_Printf( "factory monsno[%d] = %d\n", i, PokeParaGet(temp_poke,ID_PARA_monsno,NULL) );
	}
	//sys_FreeMemoryEz( temp_poke );

	//�|�P����index�e�[�u��
	for( i=0; i < m_max ;i++ ){
		buf[i] = wk->temoti_poke_index[i];
		OS_Printf( "���M�Ftemoti_index[%d] = %d\n", i, buf[i] );
	}
	num += m_max;														//2

	//�|�P�����̃p���[����
	for( i=0; i < m_max ;i++ ){
		buf[i+num] = pow_rnd[i];
		OS_Printf( "���M�Ftemoti_pow_rnd[%d] = %d\n", i, buf[i+num] );
	}
	num += m_max;														//4
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < m_max ;i++ ){
		buf[i+num] = (personal_rnd[i] & 0xffff);
		buf[i+num+m_max] = ((personal_rnd[i] >> 16) & 0xffff);
		OS_Printf( "���M�Ftemoti_personal_rnd[%d] = %d\n", i, buf[i+num] );
		OS_Printf( "���M�Ftemoti_personal_rnd[%d] = %d\n", i+m_max, buf[i+num+m_max] );
	}
	num += (m_max * 2);													//8

	if( CommSendData(FC_FACTORY_TEMOTI_POKE_DATA,buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	OS_Printf( "ret = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �莝���|�P�����f�[�^ �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommFactoryRecvTemotiPokeData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	u8 m_max;
	u32 personal_rnd[2];
	u8 pow_rnd[2];
	POKEMON_PARAM* temp_poke;
	FACTORY_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�t�@�N�g���[****** �莝���|�P�����f�[�^��M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Factory_GetMinePokeNum( wk->type );

	//�|�P����index�e�[�u��
	for( i=0; i < m_max ;i++ ){
		wk->pair_rental_poke_index[i] = recv_buf[i];
		OS_Printf( "��M�Fwk->rental_index[%d] = %d\n", i, wk->pair_rental_poke_index[i] );
	}
	num += m_max;														//2

	//�|�P�����̃p���[����
	for( i=0; i < m_max ;i++ ){
		wk->pair_rental_pow_rnd[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->rental_pow_rnd[%d] = %d\n", i, wk->pair_rental_pow_rnd[i] );
	}
	num += m_max;														//4
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < m_max ;i++ ){
		wk->pair_rental_personal_rnd[i] = recv_buf[i+num];
		wk->pair_rental_personal_rnd[i] |= (recv_buf[i+num+m_max] << 16);
		OS_Printf( "��M�Fwk->rental_personal_rnd[%d] = %d\n", i, wk->pair_rental_personal_rnd[i] );
	}
	num += (m_max * 2);													//8

#if 0
	//----------------------------------------------------------------------------------------
	Factory_PairPokeMake( wk );
#endif

	return;
}


//==============================================================================
//
//	�L���b�X��
//
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɋ�{�����Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�^�̃|�C���^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 *
 * �L���b�X�����[�N���m�ی�A�ŏ��ɑ���f�[�^
 */
//--------------------------------------------------------------
BOOL CommCastleSendBufBasicData( CASTLE_SCRWORK* wk )
{
	int i,num,ret,size;
	CASTLESCORE* score_sv;

	OS_Printf( "******�L���b�X��****** ��{��񑗐M\n" );

	score_sv = SaveData_GetCastleScore( wk->sv );
	size = CASTLE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	num = 0;

	//wk->send_buf[0]		= ;
	wk->send_buf[1] = wk->rensyou;							//���݂̘A����
	wk->send_buf[2] = wk->lap;								//����
	num += 3;												//3

	//for( i=0; i < (PERSON_NAME_SIZE + EOM_SIZE) ;i++ ){
	num += (PERSON_NAME_SIZE + EOM_SIZE);					//11

	//CP
	wk->send_buf[num] = FrontierRecord_Get( SaveData_GetFrontier(wk->sv), 
									CastleScr_GetCPRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

	OS_Printf( "���M�Fcp = %d\n", wk->send_buf[num] );
	num+=1;													//12

	if( CommSendData(FC_CASTLE_BASIC_DATA,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̊�{�����擾
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * �L���b�X�����[�N���m�ی�A�ŏ��Ɏ󂯎��f�[�^
 */
//--------------------------------------------------------------
void CommCastleRecvBufBasicData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	CASTLE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** ��{����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;
	//OS_Printf( "wk->recieve_count = %d\n", wk->recieve_count );

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//wk->					= recv_buf[0];
	//
	wk->pair_rensyou		= recv_buf[1];							//���݂̘A����
	OS_Printf( "��M�Fwk->pair_rensyou = %d\n", wk->pair_rensyou );
	wk->pair_lap			= recv_buf[2];							//����
	OS_Printf( "��M�Fwk->pair_lap = %d\n", wk->pair_lap );
	num += 3;														//3

	//for( i=0; i < (PERSON_NAME_SIZE + EOM_SIZE) ;i++ ){
	num += (PERSON_NAME_SIZE + EOM_SIZE);							//11

	//CP
	wk->pair_cp = recv_buf[num];									//CP
	OS_Printf( "��M�Fwk->pair_cp = %d\n", wk->pair_cp );
	num+=1;															//12

	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ƀg���[�i�[�����Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
BOOL CommCastleSendBufTrData( CASTLE_SCRWORK* wk )
{
	int i,num,ret,size;
	
	OS_Printf( "******�L���b�X��****** �g���[�i�[��񑗐M\n" );

	num = 0;
	size = CASTLE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	//�G�g���[�i�[�f�[�^
	for( i=0; i < CASTLE_LAP_MULTI_ENEMY_MAX ;i++ ){
		wk->send_buf[i+num] = wk->tr_index[i];
		OS_Printf( "���M�Ftr_index[%d] = %d\n", i, wk->send_buf[i+num] );
	}
	num += CASTLE_LAP_MULTI_ENEMY_MAX;						//14

	if( CommSendData(FC_CASTLE_TR_DATA,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̃g���[�i�[�����擾
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * �L���b�X�����[�N���m�ی�A�ŏ��Ɏ󂯎��f�[�^
 */
//--------------------------------------------------------------
void CommCastleRecvBufTrData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	CASTLE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** �g���[�i�[����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		return;
	}

	//�G�g���[�i�[�f�[�^
	for( i=0; i < CASTLE_LAP_MULTI_ENEMY_MAX ;i++ ){
		wk->tr_index[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}
	num += CASTLE_LAP_MULTI_ENEMY_MAX;								//14

	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ƀ����N�A�������Ȃǂ̃��N�G�X�g���Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
BOOL CommCastleSendBufSelData( CASTLE_SCRWORK* wk )
{
	int i,num,ret,size;
	MYSTATUS* my;
	
	OS_Printf( "******�L���b�X��****** �����N�A�������Ȃǂ̃��N�G�X�g��񑗐M\n" );

	OS_Printf( "�����l\n" );
	OS_Printf( "wk->sel_type = %d\n", wk->sel_type );
	OS_Printf( "wk->pair_sel_type = %d\n\n", wk->pair_sel_type );
	OS_Printf( "wk->parent_check_flag = %d\n", wk->parent_check_flag );

	num = 0;
	size = CASTLE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	wk->send_buf[0] = wk->sel_type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );

	//�e
	if( CommGetCurrentID() == COMM_PARENT_ID ){

		//��Ɏq�̑I�������Ă��Ȃ��Ēl�������Ă��Ȃ����́A�e�̌���̓Z�b�g���Ă��܂�
		if( wk->parent_check_flag == CASTLE_SCR_MENU_NONE ){
			wk->parent_check_flag = wk->sel_type;

			//CP���炷
			//CASTLESCORE_SubCP( SaveData_GetCastleScore(wk->sv), CP_USE_NUKEMITI );
			
		//�q�̑I�������Ă��Ă���ł�����A�Ƒ���O�Ɂu��������v�`�F�b�N
		}else{

			//�q�����������I��ł��Ă��A�e�����������I��ł��Ȃ�������A�e�̗p
			if( (wk->parent_check_flag-CASTLE_SCR_MENU_MAX) == CASTLE_SCR_MENU_TAISEN ){
				if( wk->sel_type != CASTLE_SCR_MENU_TAISEN ){
					wk->parent_check_flag = wk->sel_type;
				}
			}
		}

	//�q
	}else{

		//���łɐe�̑I�������Ă��āA��������̎�
		if( wk->parent_check_flag == CASTLE_SCR_MENU_TAISEN ){

			//�q�̑I������������łȂ����A�q���̗p
			if( wk->sel_type != CASTLE_SCR_MENU_TAISEN ){
				wk->parent_check_flag = wk->sel_type;
			}
		}
	}

	wk->send_buf[1] = wk->parent_check_flag;
	OS_Printf( "���M�Fparent_check_flag = %d\n", wk->send_buf[1] );
	OS_Printf( "wk->sel_type = %d\n", wk->sel_type );
	OS_Printf( "wk->pair_sel_type = %d\n\n", wk->pair_sel_type );
	OS_Printf( "wk->parent_check_flag = %d\n", wk->parent_check_flag );

	if( CommSendData(FC_CASTLE_SEL_DATA,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̃����N�A�������Ȃǂ̃��N�G�X�g���擾
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 */
//--------------------------------------------------------------
void CommCastleRecvBufSelData(int id_no,int size,void *pData,void *work)
{
	POKEMON_PARAM* temp_poke;
	int i,num;
	CASTLE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** �����N�A�������Ȃǂ̃��N�G�X�g����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	OS_Printf( "�����l\n" );
	OS_Printf( "wk->sel_type = %d\n", wk->sel_type );
	OS_Printf( "wk->pair_sel_type = %d\n\n", wk->pair_sel_type );
	OS_Printf( "wk->parent_check_flag = %d\n", wk->parent_check_flag );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	wk->pair_sel_type = recv_buf[0];
	OS_Printf( "��M�Fwk->pair_sel_type = %d\n", wk->pair_sel_type );

	//wk->parent_check_flag = recv_buf[1];
	//OS_Printf( "��M�Fwk->parent_check_flag = %d\n", wk->parent_check_flag );

	//�e
	if( CommGetCurrentID() == COMM_PARENT_ID ){

		//�e�̌��肪���łɌ��܂��Ă�����A�q�̑I���͖���
		if( wk->parent_check_flag != CASTLE_SCR_MENU_NONE ){

			//�e���u��������v��I��ŁA�q���u��������v�ł͂Ȃ����́A�q���̗p
			if( wk->parent_check_flag == CASTLE_SCR_MENU_TAISEN ){
				if( wk->pair_sel_type != CASTLE_SCR_MENU_TAISEN ){

					//�q�̑I�����̗p���ꂽ���Ƃ��킩��悤�ɃI�t�Z�b�g��������
					wk->sel_type			= wk->pair_sel_type + CASTLE_SCR_MENU_MAX;
					wk->parent_check_flag	= wk->pair_sel_type + CASTLE_SCR_MENU_MAX;
				}
			}
		}else{

			//�e�̌��肪���܂��Ă��Ȃ����́A�q�ɂ���ł�����Ƒ��M����
			//BOOL CommCastleSendBufSelData( CASTLE_SCRWORK* wk )

			//����́A�e�����X�g��I��ł��鎞�ɁA����������������āA
			//��ɐi�܂��鏈�����K�v�Ƃ������ƁB
			//���X�g�̃|�C���^����A���������֐����Ăяo������

#if 0
			wk->sel_type = wk->pair_sel_type;
			wk->parent_check_flag = wk->pair_sel_type;
#else
			//�q�̑I�����̗p���ꂽ���Ƃ��킩��悤�ɃI�t�Z�b�g��������
			//wk->sel_type = wk->pair_sel_type + CASTLE_SCR_MENU_MAX;
			//
			//��M�A���M�̗��ꂾ�Ɓu��������v��I��ł��Ȃ��̂Ɂu��������Ɂv�ɂȂ��Ă��܂��Ώ�

			wk->parent_check_flag = wk->pair_sel_type + CASTLE_SCR_MENU_MAX;
#endif

			//�u��������v�ȊO��I��ł�����A���X�g���폜
			if( wk->pair_sel_type != CASTLE_SCR_MENU_TAISEN ){
				*wk->list_del_work = FSEV_WIN_COMPULSION_DEL;
			}
		}

	//�q
	}else{

		wk->parent_check_flag = recv_buf[1];
		OS_Printf( "��M�Fwk->parent_check_flag = %d\n", wk->parent_check_flag );
		
		//�q
		//wk->parent_check_flag = wk->pair_sel_type;
		
		//�u��������v�ȊO��I��ł�����A���X�g���폜
		if( wk->parent_check_flag != CASTLE_SCR_MENU_TAISEN ){
			*wk->list_del_work = FSEV_WIN_COMPULSION_DEL;
		}

#if 1
		//08.07.05
		//�e���u��������v��I��ł��āArecv_buf[1]�Ɂu��������v�������Ă���
		//�y�A���u��������v�ȊO��I�񂾎��́A���j���[�������I��������
		//
		//�e���u��������v��I��ł��āA�q�������e���|�x��āu��������v�ȊO��I��ł���ƁA
		//parent_check_flag�́u��������v�ɂȂ��Ă��܂��Ă���̂ŁA
		//�q�̎����̑I�����u��������v�ȊO�Ȃ�A����ŏ㏑������

		//�e���u��������v��I��ł���
		//���u���Ԃ�v�u�����āv�̎��͐揟���ɂȂ�̂Łu��������v���̃`�F�b�N�K�v�I
		if( wk->pair_sel_type == CASTLE_SCR_MENU_TAISEN ){

			OS_Printf( "\n**********\nwk->sel_type = %d\n", wk->sel_type );

			//�q�����łɑI�����Ă���
			if( wk->sel_type != CASTLE_SCR_MENU_NONE ){

				//�q���u��������v�ȊO��I��ł�����
				if( wk->sel_type != CASTLE_SCR_MENU_TAISEN ){
				//if( (wk->sel_type - CASTLE_SCR_MENU_MAX) != CASTLE_SCR_MENU_TAISEN ){

					//�q�̑I�����̗p���ꂽ���Ƃ��킩��悤�ɃI�t�Z�b�g��������(����)
					//wk->sel_type			= wk->sel_type + CASTLE_SCR_MENU_MAX;
					wk->parent_check_flag	= wk->sel_type + CASTLE_SCR_MENU_MAX;
				
					//wk->parent_check_flag	= wk->sel_type;
					OS_Printf( "����(�q)�Fwk->sel_type = %d\n", wk->sel_type );
					OS_Printf( "����(�q)�Fwk->parent_check_flag = %d\n", wk->parent_check_flag );
				}
			}
		}
#endif

	}

	OS_Printf( "��M�Fwk->pair_sel_type = %d\n", wk->pair_sel_type );
	OS_Printf( "wk->sel_type = %d\n", wk->sel_type );
	OS_Printf( "wk->pair_sel_type = %d\n\n", wk->pair_sel_type );
	OS_Printf( "wk->parent_check_flag = %d\n", wk->parent_check_flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�ɓG�|�P�����f�[�^���Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
BOOL CommCastleSendBufEnemyPokeData( CASTLE_SCRWORK* wk )
{
	int i,num,ret,size;
	
	OS_Printf( "******�L���b�X��****** �G�|�P�������M\n" );

	num = 0;
	size = CASTLE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	//�|�P����index�e�[�u��
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		wk->send_buf[i] = wk->enemy_poke_index[i];
		OS_Printf( "���M�Fenemy_index[%d] = %d\n", i, wk->send_buf[i] );
	}
	num += CASTLE_ENEMY_POKE_MAX;										//4

	//�|�P�����̃p���[����
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		wk->send_buf[i+num] = wk->enemy_pow_rnd[i];
		OS_Printf( "���M�Fenemy_pow_rnd[%d] = %d\n", i, wk->send_buf[i+num] );
	}
	num += CASTLE_ENEMY_POKE_MAX;										//8
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		//u32 enemy_personal_rnd[CASTLE_ENEMY_POKE_MAX];
		wk->send_buf[i+num] = (wk->enemy_personal_rnd[i] & 0xffff);
		wk->send_buf[i+num+CASTLE_ENEMY_POKE_MAX] = 
								((wk->enemy_personal_rnd[i] >> 16) & 0xffff);
		OS_Printf( "���f�[�^ enemy_personal_rnd[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
		OS_Printf( "���M�Fenemy_personal_rnd[%d] = %d\n", i, wk->send_buf[i+num] );
		OS_Printf( "���M�Fenemy_personal_rnd[%d] = %d\n", i+CASTLE_ENEMY_POKE_MAX, 
													wk->send_buf[i+num+CASTLE_ENEMY_POKE_MAX] );
	}
	num += (CASTLE_ENEMY_POKE_MAX * 2);								//16

	if( CommSendData(FC_CASTLE_ENEMY_POKE_DATA,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̓G�|�P�����f�[�^���擾
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 */
//--------------------------------------------------------------
void CommCastleRecvBufEnemyPokeData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	CASTLE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** �G�|�P��������M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�|�P����index�e�[�u��
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		wk->enemy_poke_index[i] = recv_buf[i];
		OS_Printf( "��M�Fwk->enemy_index[%d] = %d\n", i, wk->enemy_poke_index[i] );
	}
	num += CASTLE_ENEMY_POKE_MAX;										//4

	//�|�P�����̃p���[����
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		wk->enemy_pow_rnd[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->enemy_pow_rnd[%d] = %d\n", i, wk->enemy_pow_rnd[i] );
	}
	num += CASTLE_ENEMY_POKE_MAX;										//8
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		//u32 enemy_personal_rnd[CASTLE_ENEMY_POKE_MAX];
		wk->enemy_personal_rnd[i] = recv_buf[i+num];
		wk->enemy_personal_rnd[i] |= (recv_buf[i+num+CASTLE_ENEMY_POKE_MAX] << 16);
		OS_Printf( "��M�Fwk->enemy_personal_rnd[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
	}
	num += (CASTLE_ENEMY_POKE_MAX * 2);								//16

	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�ɐ퓬��̃��j���[�ŉ���I�񂾂����Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�^�̃|�C���^
 * @param	sel		0=�Â���A0�ȊO=���^�C��(���낭����͂���Ȃ�)
 *
 * @return	none
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
BOOL CommCastleSendBufRetireFlag( CASTLE_SCRWORK* wk, u16 retire_flag )
{
	int ret,size;

	size = CASTLE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	wk->send_buf[0] = retire_flag;
	OS_Printf( "���M�Fretire_flag = %d\n", wk->send_buf[0] );

	if( CommSendData(FC_CASTLE_RETIRE_FLAG,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̐퓬��̃��j���[�ŉ���I�񂾂����擾
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
void CommCastleRecvBufRetireFlag(int id_no,int size,void *pData,void *work)
{
	int num;
	CASTLE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** ���^�C�A����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	wk->pair_retire_flag = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_retire_flag = %d\n", wk->pair_retire_flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɍ����������������Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�^�̃|�C���^
 * @param	sel		0=�����������A1=�����������Ȃ�
 *
 * @return	none
 */
//--------------------------------------------------------------
BOOL CommCastleSendBufTradeYesNoFlag( CASTLE_SCRWORK* wk, u16 trade_yesno_flag )
{
	int ret,size;

	size = CASTLE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	wk->send_buf[0] = trade_yesno_flag;
	OS_Printf( "���M�Ftrade_yesno_flag = %d\n", wk->send_buf[0] );

	if( CommSendData(FC_CASTLE_TRADE_YESNO_FLAG,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̌��������������擾
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 */
//--------------------------------------------------------------
void CommCastleRecvBufTradeYesNoFlag(int id_no,int size,void *pData,void *work)
{
	int num;
	CASTLE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** �����͂� ����������M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	wk->pair_trade_yesno_flag = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_trade_yesno_flag = %d\n", wk->pair_trade_yesno_flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɏ莝���|�P�����f�[�^���Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�^�̃|�C���^
 *
 * @return	none
 *
 * �e�@�A�q�@�̗����Ƃ��I�񂾎莝����n���K�v������
 */
//--------------------------------------------------------------
BOOL CommCastleSendBufTemotiPokeData( CASTLE_SCRWORK* wk )
{
	int i,num,ret,size,pp_size;
	u8 m_max;
	POKEMON_PARAM* temp_poke;
	
	num = 0;
	size = CASTLE_HUGE_BUF_LEN;

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_SOLO );

	//POKEMON_PARAM�̃T�C�Y���擾
	pp_size = PokemonParam_GetWorkSize();
	OS_Printf( "pokemon_param_size = %d\n", pp_size );

	for( i=0; i < m_max ;i++ ){
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, i );
		MI_CpuCopy8( temp_poke, &wk->huge_buf[i*pp_size], pp_size );
	}

	OS_Printf( "���M�Fpokemon_param\n" );

	if( CommSendHugeData(FC_CASTLE_TEMOTI_POKE_DATA,wk->huge_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	OS_Printf( "���M���ʁFCommSendHugeData = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̎莝���|�P�����f�[�^���擾
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * �e�@�A�q�@�̗����Ƃ��I�񂾎莝����n���K�v������
 *
 * pair_rental_...���g���܂킵�Ă���
 */
//--------------------------------------------------------------
void CommCastleRecvBufTemotiPokeData(int id_no,int size,void *pData,void *work)
{
	u8 m_max;
	int i,num,pp_size;
	POKEMON_PARAM* temp_poke;
	CASTLE_SCRWORK* wk = work;
	const u8* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** �莝���|�P��������M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_SOLO );

	pp_size = PokemonParam_GetWorkSize();

	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < m_max ;i++ ){
		MI_CpuCopy8( &recv_buf[pp_size*i], temp_poke, pp_size );
		PokeParty_Add( wk->p_m_party, temp_poke );
	}
	sys_FreeMemoryEz( temp_poke );

	//////////////////////////////////////////////////////
	//�q�͎莝���|�P���������ɂ���
	if( CommGetCurrentID() != COMM_PARENT_ID ){
		PokeParty_ExchangePosition( wk->p_m_party, 0, 2 );
		PokeParty_ExchangePosition( wk->p_m_party, 1, 3 );
	}
	//////////////////////////////////////////////////////

	OS_Printf( "�|�P�����p�[�e�B�� = %d\n",	PokeParty_GetPokeCount(wk->p_m_party) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�̈�擾�֐�(����f�[�^�FCommSendHugeData�̎�M�p)
 *
 * @param   netID		
 * @param   pWork		
 * @param   size		
 *
 * @retval  
 */
//--------------------------------------------------------------
u8* CommCastleGetRecvDataBuff( int netID, void*pWork, int size )
{
	CASTLE_SCRWORK* wk = pWork;
	OS_Printf( "size = %d\n", size );
	GF_ASSERT(size <= CASTLE_HUGE_BUF_LEN);
    return wk->recieve_huge_buf[netID];
}


//==============================================================================
//
//	�t�����e�B�AWiFi��t
//
//==============================================================================
#if 0
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
	int ret,size;

	OS_Printf( "******WIFI��t****** �I�������{��No���M\n" );

	size = FRWIFI_COMM_BUF_LEN;

	wk->send_buf[0]	= wk->bf_no;								//�{��No

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

	size = FRWIFI_COMM_BUF_LEN;

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

	size = FRWIFI_COMM_BUF_LEN;

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
	}else{
		pp = PokeParty_GetMemberPointer( party, pos1 );
		wk->monsno[0] = PokeParaGet( pp, ID_PARA_monsno, NULL );
		wk->itemno[0] = PokeParaGet( pp, ID_PARA_item, NULL );
	}

	wk->send_buf[0] = wk->monsno[0];
	wk->send_buf[1] = wk->itemno[0];
	OS_Printf( "���M�Fmonsno1 = %d\n", wk->send_buf[0] );
	OS_Printf( "���M�Fitemno1 = %d\n", wk->send_buf[1] );

	////////////////////////////////////////////////////////////
	party = SaveData_GetTemotiPokemon( wk->sv );
	pp = PokeParty_GetMemberPointer( party, pos2 );

	wk->monsno[1] = PokeParaGet( pp, ID_PARA_monsno, NULL );
	wk->itemno[1] = PokeParaGet( pp, ID_PARA_item, NULL );

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

	size = FRWIFI_COMM_BUF_LEN;

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

	wk->pair_stage_del = (u8)recv_buf[0];
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

	size = FRWIFI_COMM_BUF_LEN;

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


//==============================================================================
//
//	���[���b�g
//
//	�\���̂������āA���̃T�C�Y�A��M�͂��̌^�ő���ɕύX�\��B�B�B
//	
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɋ�{�����Z�b�g
 *
 * @param	wk		ROULETTE_SCRWORK�^�̃|�C���^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 *
 * ���[���b�g���[�N���m�ی�A�ŏ��ɑ���f�[�^
 */
//--------------------------------------------------------------
BOOL CommRouletteSendBufBasicData( ROULETTE_SCRWORK* wk )
{
	int i,num,ret,size;
	ROULETTESCORE* score_sv;

	OS_Printf( "******���[���b�g****** ��{��񑗐M\n" );

	score_sv = SaveData_GetRouletteScore( wk->sv );
	size = ROULETTE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���
	num = 0;

	//wk->send_buf[0]	= 
	wk->send_buf[1] = wk->rensyou;						//���݂̘A����
	wk->send_buf[2] = wk->lap;							//����
	num += 3;											//3

	if( CommSendData(FC_ROULETTE_BASIC_DATA,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̊�{�����擾
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * ���[���b�g���[�N���m�ی�A�ŏ��Ɏ󂯎��f�[�^
 */
//--------------------------------------------------------------
void CommRouletteRecvBufBasicData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	ROULETTE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******���[���b�g****** ��{����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;
	//OS_Printf( "wk->recieve_count = %d\n", wk->recieve_count );

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//wk->	= 
	//
	wk->pair_rensyou		= recv_buf[1];							//���݂̘A����
	OS_Printf( "��M�Fwk->pair_rensyou = %d\n", wk->pair_rensyou );
	wk->pair_lap			= recv_buf[2];							//����
	OS_Printf( "��M�Fwk->pair_lap = %d\n", wk->pair_lap );
	num += 3;														//3

	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ƀg���[�i�[�����Z�b�g
 *
 * @param	wk		ROULETTE_SCRWORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
BOOL CommRouletteSendBufTrData( ROULETTE_SCRWORK* wk )
{
	int i,num,ret,size;
	
	OS_Printf( "******���[���b�g****** �g���[�i�[��񑗐M\n" );

	num = 0;
	size = ROULETTE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	//�G�g���[�i�[�f�[�^
	for( i=0; i < ROULETTE_LAP_MULTI_ENEMY_MAX ;i++ ){
		wk->send_buf[i+num] = wk->tr_index[i];
		OS_Printf( "���M�Ftr_index[%d] = %d\n", i, wk->send_buf[i+num] );
	}
	num += ROULETTE_LAP_MULTI_ENEMY_MAX;						//14

	if( CommSendData(FC_ROULETTE_TR_DATA,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̃g���[�i�[�����擾
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * ���[���b�g���[�N���m�ی�A�ŏ��Ɏ󂯎��f�[�^
 */
//--------------------------------------------------------------
void CommRouletteRecvBufTrData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	ROULETTE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******���[���b�g****** �g���[�i�[����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		return;
	}

	//�G�g���[�i�[�f�[�^
	for( i=0; i < ROULETTE_LAP_MULTI_ENEMY_MAX ;i++ ){
		wk->tr_index[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}
	num += ROULETTE_LAP_MULTI_ENEMY_MAX;								//14

	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�ɓG�|�P�����f�[�^���Z�b�g
 *
 * @param	wk		ROULETTE_SCRWORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
BOOL CommRouletteSendBufEnemyPokeData( ROULETTE_SCRWORK* wk )
{
	int i,num,ret,size;
	
	OS_Printf( "******���[���b�g****** �G�|�P�������M\n" );

	num = 0;
	size = ROULETTE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	//�|�P����index�e�[�u��
	for( i=0; i < ROULETTE_ENEMY_POKE_MAX ;i++ ){
		wk->send_buf[i] = wk->enemy_poke_index[i];
		OS_Printf( "���M�Fenemy_index[%d] = %d\n", i, wk->send_buf[i] );
	}
	num += ROULETTE_ENEMY_POKE_MAX;										//4

	//�|�P�����̃p���[����
	for( i=0; i < ROULETTE_ENEMY_POKE_MAX ;i++ ){
		wk->send_buf[i+num] = wk->enemy_pow_rnd[i];
		OS_Printf( "���M�Fenemy_pow_rnd[%d] = %d\n", i, wk->send_buf[i+num] );
	}
	num += ROULETTE_ENEMY_POKE_MAX;										//8
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < ROULETTE_ENEMY_POKE_MAX ;i++ ){
		//u32 enemy_personal_rnd[ROULETTE_ENEMY_POKE_MAX];
		wk->send_buf[i+num] = (wk->enemy_personal_rnd[i] & 0xffff);
		wk->send_buf[i+num+ROULETTE_ENEMY_POKE_MAX] = 
								((wk->enemy_personal_rnd[i] >> 16) & 0xffff);
		OS_Printf( "���f�[�^ enemy_personal_rnd[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
		OS_Printf( "���M�Fenemy_personal_rnd[%d] = %d\n", i, wk->send_buf[i+num] );
		OS_Printf( "���M�Fenemy_personal_rnd[%d] = %d\n", i+ROULETTE_ENEMY_POKE_MAX, 
													wk->send_buf[i+num+ROULETTE_ENEMY_POKE_MAX] );
	}
	num += (ROULETTE_ENEMY_POKE_MAX * 2);								//16

	//B_TOWER_POKEMON enemy_poke[ROULETTE_ENEMY_POKE_MAX];

	if( CommSendData(FC_ROULETTE_ENEMY_POKE_DATA,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̓G�|�P�����f�[�^���擾
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 */
//--------------------------------------------------------------
void CommRouletteRecvBufEnemyPokeData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	ROULETTE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******���[���b�g****** �G�|�P��������M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�|�P����index�e�[�u��
	for( i=0; i < ROULETTE_ENEMY_POKE_MAX ;i++ ){
		wk->enemy_poke_index[i] = recv_buf[i];
		OS_Printf( "��M�Fwk->enemy_index[%d] = %d\n", i, wk->enemy_poke_index[i] );
	}
	num += ROULETTE_ENEMY_POKE_MAX;										//4

	//�|�P�����̃p���[����
	for( i=0; i < ROULETTE_ENEMY_POKE_MAX ;i++ ){
		wk->enemy_pow_rnd[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->enemy_pow_rnd[%d] = %d\n", i, wk->enemy_pow_rnd[i] );
	}
	num += ROULETTE_ENEMY_POKE_MAX;										//8
	
	//�|�P�����̌�����(u32�Ȃ̂ŁAu16���Z�b�g���Ă���̂Œ��ӁI)
	for( i=0; i < ROULETTE_ENEMY_POKE_MAX ;i++ ){
		//u32 enemy_personal_rnd[ROULETTE_ENEMY_POKE_MAX];
		wk->enemy_personal_rnd[i] = recv_buf[i+num];
		wk->enemy_personal_rnd[i] |= (recv_buf[i+num+ROULETTE_ENEMY_POKE_MAX] << 16);
		OS_Printf( "��M�Fwk->enemy_personal_rnd[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
	}
	num += (ROULETTE_ENEMY_POKE_MAX * 2);								//16

#if 0
	//B_TOWER_POKEMON enemy_poke[ROULETTE_ENEMY_POKE_MAX];

	//�G�|�P��������x�ɐ���
	Frontier_PokemonParamCreateAll(	wk->enemy_poke, wk->enemy_poke_index, 
									wk->enemy_pow_rnd, wk->enemy_personal_rnd, NULL, 
									ROULETTE_ENEMY_POKE_MAX, HEAPID_WORLD, ARC_PL_BTD_PM );

	//�G�p�[�e�B�̃Z�b�g
	Roulette_EnemyPartySet( wk );
#endif
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�ɐ퓬��̃��j���[�ŉ���I�񂾂����Z�b�g
 *
 * @param	wk		ROULETTE_SCRWORK�^�̃|�C���^
 * @param	sel		0=�Â���A0�ȊO=���^�C��(���낭����͂���Ȃ�)
 *
 * @return	none
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
BOOL CommRouletteSendBufRetireFlag( ROULETTE_SCRWORK* wk, u16 retire_flag )
{
	int ret,size;

	size = ROULETTE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	wk->send_buf[0] = retire_flag;
	OS_Printf( "���M�Fretire_flag = %d\n", wk->send_buf[0] );

	if( CommSendData(FC_ROULETTE_RETIRE_FLAG,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̐퓬��̃��j���[�ŉ���I�񂾂����擾
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * ���݂����u�Â���v�̎��͌p���A
 * �����A�ǂ��炩���u���^�C���v�̎��͒��~
 */
//--------------------------------------------------------------
void CommRouletteRecvBufRetireFlag(int id_no,int size,void *pData,void *work)
{
	int num;
	ROULETTE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******���[���b�g****** ���^�C�A����M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	wk->pair_retire_flag = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_retire_flag = %d\n", wk->pair_retire_flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɍ����������������Z�b�g
 *
 * @param	wk		ROULETTE_SCRWORK�^�̃|�C���^
 * @param	sel		0=�����������A1=�����������Ȃ�
 *
 * @return	none
 */
//--------------------------------------------------------------
BOOL CommRouletteSendBufTradeYesNoFlag( ROULETTE_SCRWORK* wk, u16 trade_yesno_flag )
{
	int ret,size;

	size = ROULETTE_COMM_BUF_LEN;						//���ۂ�u16�f�[�^�Ȃ̂ł��̔���

	wk->send_buf[0] = trade_yesno_flag;
	OS_Printf( "���M�Ftrade_yesno_flag = %d\n", wk->send_buf[0] );

	if( CommSendData(FC_ROULETTE_TRADE_YESNO_FLAG,wk->send_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̌��������������擾
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 */
//--------------------------------------------------------------
void CommRouletteRecvBufTradeYesNoFlag(int id_no,int size,void *pData,void *work)
{
	int num;
	ROULETTE_SCRWORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******���[���b�g****** �����͂� ����������M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	wk->pair_trade_yesno_flag = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_trade_yesno_flag = %d\n", wk->pair_trade_yesno_flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɏ莝���|�P�����f�[�^���Z�b�g
 *
 * @param	wk		ROULETTE_SCRWORK�^�̃|�C���^
 *
 * @return	none
 *
 * �e�@�A�q�@�̗����Ƃ��I�񂾎莝����n���K�v������
 */
//--------------------------------------------------------------
BOOL CommRouletteSendBufTemotiPokeData( ROULETTE_SCRWORK* wk )
{
	int i,num,ret,size,pp_size;
	u8 m_max;
	POKEMON_PARAM* temp_poke;
	
	num = 0;
	size = ROULETTE_HUGE_BUF_LEN;

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_SOLO );

	//POKEMON_PARAM�̃T�C�Y���擾
	pp_size = PokemonParam_GetWorkSize();
	OS_Printf( "pokemon_param_size = %d\n", pp_size );

	for( i=0; i < m_max ;i++ ){
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, i );
		MI_CpuCopy8( temp_poke, &wk->huge_buf[i*pp_size], pp_size );
	}

	OS_Printf( "���M�Fpokemon_param\n" );

	if( CommSendHugeData(FC_ROULETTE_TEMOTI_POKE_DATA,wk->huge_buf,size) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	OS_Printf( "���M���ʁFCommSendHugeData = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	recv_buf�̎莝���|�P�����f�[�^���擾
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 * @param	recv_buf	
 *
 * @return	none
 *
 * �e�@�A�q�@�̗����Ƃ��I�񂾎莝����n���K�v������
 *
 * pair_rental_...���g���܂킵�Ă���
 */
//--------------------------------------------------------------
void CommRouletteRecvBufTemotiPokeData(int id_no,int size,void *pData,void *work)
{
	u8 m_max;
	int i,num,pp_size;
	POKEMON_PARAM* temp_poke;
	ROULETTE_SCRWORK* wk = work;
	const u8* recv_buf = pData;

	OS_Printf( "******���[���b�g****** �莝���|�P��������M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_SOLO );

	pp_size = PokemonParam_GetWorkSize();

	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < m_max ;i++ ){
		MI_CpuCopy8( &recv_buf[pp_size*i], temp_poke, pp_size );
		PokeParty_Add( wk->p_m_party, temp_poke );
	}
	sys_FreeMemoryEz( temp_poke );

	//////////////////////////////////////////////////////
	//�q�͎莝���|�P���������ɂ���
	if( CommGetCurrentID() != COMM_PARENT_ID ){
		PokeParty_ExchangePosition( wk->p_m_party, 0, 2 );
		PokeParty_ExchangePosition( wk->p_m_party, 1, 3 );
	}
	//////////////////////////////////////////////////////

	OS_Printf( "�|�P�����p�[�e�B�� = %d\n",	PokeParty_GetPokeCount(wk->p_m_party) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�̈�擾�֐�(����f�[�^�FCommSendHugeData�̎�M�p)
 *
 * @param   netID		
 * @param   pWork		
 * @param   size		
 *
 * @retval  
 */
//--------------------------------------------------------------
u8* CommRouletteGetRecvDataBuff( int netID, void*pWork, int size )
{
	ROULETTE_SCRWORK* wk = pWork;
	OS_Printf( "size = %d\n", size );
	GF_ASSERT(size <= ROULETTE_HUGE_BUF_LEN);
    return wk->recieve_huge_buf[netID];
}



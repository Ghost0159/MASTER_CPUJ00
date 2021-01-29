//=============================================================================
/**
 * @file	comm_command_frontier.h
 * @brief	�ʐM�̃R�}���h�ꗗ  �t�����e�B�A�p
 * @author	nohara
 * @date    2007.07.18
 */
//=============================================================================
#ifndef __COMM_COMMAND_FRONTIER_H__
#define __COMM_COMMAND_FRONTIER_H__

#include "communication/comm_command.h"
#include "savedata/b_tower.h"
#include "battle/b_tower_data.h"
#include "battle/battle_common.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_tool.h"

#include "factory_tool.h"
#include "stage_tool.h"
#include "castle_tool.h"
#include "roulette_tool.h"
#include "frontier_wifi.h"

//�^���[
#include "field/location.h"
#include "../field/b_tower_scr_def.h"
#include "../field/tower_scr_common.h"


//=============================================================================
//
/// �t�����e�B�A��p�ʐM�R�}���h�̒�`
//
//	1GameFrame��22byte�����ۂɑ��M�ł���
//
//=============================================================================
enum CommCommandBattle_e {
	//////////////////////////////////////////////////////////////////////////////////////
	//�t�@�N�g���[(�t�����e�B�A)
	FC_FACTORY_BASIC_DATA = CS_COMMAND_MAX,
	FC_FACTORY_TR_DATA,
	FC_FACTORY_RENTAL_DATA,
	FC_FACTORY_ENEMY_POKE_DATA,
	FC_FACTORY_RETIRE_FLAG,
	FC_FACTORY_TRADE_YESNO_FLAG,
	FC_FACTORY_TEMOTI_POKE_DATA,

	//�t�@�N�g���[(�����^���A�������)factory.c
	FC_FACTORY_PAIR,
	FC_FACTORY_PAIR_POKE,
	FC_FACTORY_PAIR_CANCEL,
	FC_FACTORY_TRADE,
	
	//////////////////////////////////////////////////////////////////////////////////////

	//�X�e�[�W(�t�����e�B�A)
	FC_STAGE_MONSNO,
	FC_STAGE_BASIC_DATA,
	FC_STAGE_TR_DATA,
	FC_STAGE_ENEMY_POKE_DATA,
	FC_STAGE_RETIRE_FLAG,
	FC_STAGE_TEMOTI_POKE_DATA,

	//�X�e�[�W(�^�C�v�I�����)
	FC_STAGE_PAIR,
	FC_STAGE_PAIR_POKETYPE,
	FC_STAGE_PAIR_FINAL_ANSWER,

	//////////////////////////////////////////////////////////////////////////////////////

	//�L���b�X��(�t�����e�B�A)
	FC_CASTLE_BASIC_DATA,
	FC_CASTLE_TR_DATA,
	FC_CASTLE_SEL_DATA,
	FC_CASTLE_ENEMY_POKE_DATA,
	FC_CASTLE_RETIRE_FLAG,
	FC_CASTLE_TRADE_YESNO_FLAG,
	FC_CASTLE_TEMOTI_POKE_DATA,

	//�L���b�X��(�莝���|�P�������)
	FC_CASTLE_MINE_PAIR,
	FC_CASTLE_MINE_REQ_TYPE,
	FC_CASTLE_MINE_CSR_POS,
	FC_CASTLE_MINE_MODORU,

	//�L���b�X��(�G�g���[�i�[���)
	FC_CASTLE_ENEMY_PAIR,
	FC_CASTLE_ENEMY_REQ_TYPE,
	FC_CASTLE_ENEMY_CSR_POS,
	FC_CASTLE_ENEMY_MODORU,

	//WiFi��t(�t�����e�B�A)
	FC_WIFI_COUNTER_NO,
	FC_WIFI_COUNTER_RETIRE_FLAG,
	FC_WIFI_COUNTER_MONSNO_ITEMNO,
	FC_WIFI_COUNTER_STAGE_RECORD_DEL,
	FC_WIFI_COUNTER_GAME_CONTINUE,

	//�^���[(�t�B�[���h,WiFi)
	FC_TOWER_PLAYER_DATA,
	FC_TOWER_TR_DATA,
	FC_TOWER_RETIRE_SELECT,

	//////////////////////////////////////////////////////////////////////////////////////

	//���[���b�g(�t�����e�B�A)
	FC_ROULETTE_BASIC_DATA,
	FC_ROULETTE_TR_DATA,
	FC_ROULETTE_ENEMY_POKE_DATA,
	FC_ROULETTE_RETIRE_FLAG,
	FC_ROULETTE_TRADE_YESNO_FLAG,
	FC_ROULETTE_TEMOTI_POKE_DATA,

	//���[���b�g(�����N�A�b�v���)
	FC_ROULETTE_PAIR,
	FC_ROULETTE_UP_TYPE,
	FC_ROULETTE_CSR_POS,

	//------------------------------------------------�����܂�
	FC_COMMAND_MAX   // �I�[--------------����͈ړ������Ȃ��ł�������
};


//==============================================================================
//
//	�O���֐��錾
//
//==============================================================================
extern void CommCommandFrontierInitialize(void* pWork);
extern BOOL CommFrontierSendPokePara(void*consys, int breeder_no, const POKEMON_PARAM *pp);
extern BOOL CommFrontierSendPokeParaAll(void *consys, POKEMON_PARAM **pp);

//�t�@�N�g���[
extern BOOL	CommFactorySendBasicData( FACTORY_SCRWORK* wk );
extern BOOL	CommFactorySendTrData( FACTORY_SCRWORK* wk );
extern BOOL	CommFactorySendRentalData( FACTORY_SCRWORK* wk );
extern BOOL	CommFactorySendEnemyPokeData( FACTORY_SCRWORK* wk );
extern BOOL	CommFactorySendRetireFlag( FACTORY_SCRWORK* wk, u8 retire_flag );
extern BOOL	CommFactorySendTradeYesNoFlag( FACTORY_SCRWORK* wk, u8 trade_yesno_flag );
extern BOOL	CommFactorySendTemotiPokeData( FACTORY_SCRWORK* wk );

//�X�e�[�W
extern BOOL CommStageSendMonsNo( STAGE_SCRWORK* wk, const POKEMON_PARAM* pp );
extern BOOL	CommStageSendBasicData( STAGE_SCRWORK* wk );
extern BOOL	CommStageSendTrData( STAGE_SCRWORK* wk );
extern BOOL	CommStageSendEnemyPokeData( STAGE_SCRWORK* wk );
extern BOOL	CommStageSendRetireFlag( STAGE_SCRWORK* wk, u8 retire_flag );
extern BOOL	CommStageSendBufTemotiPokeData( STAGE_SCRWORK* wk );

//�L���b�X��
extern BOOL	CommCastleSendBufBasicData( CASTLE_SCRWORK* wk );
extern BOOL	CommCastleSendBufTrData( CASTLE_SCRWORK* wk );
extern BOOL	CommCastleSendBufSelData( CASTLE_SCRWORK* wk );
extern BOOL	CommCastleSendBufEnemyPokeData( CASTLE_SCRWORK* wk );
extern BOOL	CommCastleSendBufRetireFlag( CASTLE_SCRWORK* wk, u16 retire_flag );
extern BOOL	CommCastleSendBufTradeYesNoFlag( CASTLE_SCRWORK* wk, u16 trade_yesno_flag );
extern BOOL	CommCastleSendBufTemotiPokeData( CASTLE_SCRWORK* wk );

//WiFi��t
extern BOOL CommFrWiFiCounterSendBufBFNo( FRWIFI_SCRWORK* wk );
extern BOOL CommFrWiFiCounterSendBufRetireFlag( FRWIFI_SCRWORK* wk, u16 retire_flag );
extern BOOL CommFrWiFiCounterSendBufMonsNoItemNo( FRWIFI_SCRWORK* wk, u16 pos1, u16 pos2 );
extern BOOL CommFrWiFiCounterSendBufStageRecordDel( FRWIFI_SCRWORK* wk, u16 stage_del_flag );
extern BOOL CommFrWiFiCounterSendBufGameContinue( FRWIFI_SCRWORK* wk, u16 flag );

//WiFi��t(�^���[)
extern BOOL CommFrWiFiCounterTowerSendBufTrainerData( BTOWER_SCRWORK* wk );

//���[���b�g
extern BOOL	CommRouletteSendBufBasicData( ROULETTE_SCRWORK* wk );
extern BOOL	CommRouletteSendBufTrData( ROULETTE_SCRWORK* wk );
extern BOOL	CommRouletteSendBufEnemyPokeData( ROULETTE_SCRWORK* wk );
extern BOOL	CommRouletteSendBufRetireFlag( ROULETTE_SCRWORK* wk, u16 retire_flag );
extern BOOL	CommRouletteSendBufTradeYesNoFlag( ROULETTE_SCRWORK* wk, u16 trade_yesno_flag );
extern BOOL	CommRouletteSendBufTemotiPokeData( ROULETTE_SCRWORK* wk );


#endif// __COMM_COMMAND_FRONTIER_H__



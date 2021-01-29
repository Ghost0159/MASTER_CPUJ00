//============================================================================================
/**
 * @file	fssc_wifi_counter.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�FWiFi��t
 * @author	Satoshi Nohara
 * @date	07.07.31
 */
//============================================================================================
#include "common.h"
#include <nitro/code16.h> 
#include "system/pm_str.h"
#include "system\msgdata.h"						//MSGMAN_GetString
#include "system\wordset.h"						//WordSet_RegistPlayerName
#include "system/lib_pack.h"
#include "system/snd_tool.h"
#include "savedata/frontier_savedata.h"

#include "field/eventflag.h"
#include "field/evwkdef.h"
#include "gflib/strbuf_family.h"				//����

#include "../field/field_subproc.h"				//TestBattleProcData

#include "frontier_types.h"
#include "frontier_main.h"
#include "frontier_scr.h"
#include "frontier_scrcmd.h"
#include "fss_task.h"
#include "comm_command_frontier.h"
#include "frontier_wifi.h"
#include "frontier_def.h"
#include "frontier_tool.h"
#include "wifi_counter_tool.h"
#include "fssc_wifi_counter_sub.h"

#include "field/eventflag.h"
#include "../field/scr_stage_sub.h"
#include "../field/scr_tool.h"

#include "poketool/status_rcv.h"
#include "communication/comm_system.h"
#include "communication/comm_def.h"
#include "communication/comm_tool.h"
#include "field/location.h"
#include "../field/b_tower_scr_def.h"
#include "../field/tower_scr_common.h"			//BTOWER_SCRWORK
#include "../field/b_tower_scr.h"				//TowerScr_WorkInit


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
BOOL FSSC_WiFiCounterWorkAlloc( FSS_TASK * core );
BOOL FSSC_WiFiCounterWorkFree( FSS_TASK * core );
BOOL FSSC_WiFiCounterSendBuf( FSS_TASK * core );
BOOL FSSC_WiFiCounterRecvBuf( FSS_TASK * core );
static BOOL WaitWiFiCounterRecvBuf( FSS_TASK * core );
BOOL FSSC_WiFiCounterBFNoCheck( FSS_TASK * core );
BOOL FSSC_WiFiCounterPokeList( FSS_TASK * core );
static BOOL EvWaitFrWifiCounterPokeList(FSS_TASK * core);
BOOL FSSC_WiFiCounterPokeListGetResult(FSS_TASK * core);
BOOL FSSC_WiFiCounterPairYameruCheck( FSS_TASK * core );
BOOL FSSC_WiFiCounterScrWork( FSS_TASK * core );
BOOL FSSC_WiFiCounterTowerCallBefore( FSS_TASK * core );
BOOL FSSC_WiFiCounterTowerSendTrainerData( FSS_TASK * core );
BOOL FSSC_WiFiCounterTowerRecvTrainerData( FSS_TASK * core );
static BOOL EvWaitWiFiCounterTowerRecvTrainerData(FSS_TASK * core);
BOOL FSSC_WiFiCounterTowerCallAfter( FSS_TASK * core );


//============================================================================================
//
//	�R�}���h
//
//============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�t�����e�B�AWiFi��t���[�N�m��
 *
 * @param	core	���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterWorkAlloc( FSS_TASK * core ) 
{
	FRWIFI_SCRWORK* wk;
	FRONTIER_EX_PARAM* ex_param;

	ex_param = Frontier_ExParamGet( core->fss->fmain );

	wk = FrWiFiCounterScr_WorkAlloc( ex_param->savedata );
	Frontier_SysWorkSet( core->fss->fmain, wk );

	OS_Printf( "�X�N���v�g�t�����e�B�AWiFi��t���[�N�m��\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�t�����e�B�AWiFi��t���[�N�J��
 *
 * @param	core	���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterWorkFree( FSS_TASK * core ) 
{
	FRWIFI_SCRWORK* wifi_scr_wk;
	wifi_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	FrWiFiCounterScr_WorkFree( wifi_scr_wk );
	OS_Printf( "�X�N���v�g�t�����e�B�AWiFi��t���[�N�J��\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * ���M
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"1"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterSendBuf( FSS_TASK * core )
{
	FRWIFI_SCRWORK* wifi_scr_wk;
	u16 type	= FSSTGetWorkValue( core );
	u16 param	= FSSTGetWorkValue( core );
	u16 param2	= FSSTGetWorkValue( core );
	u16* ret_wk	= FSSTGetWork( core );

	wifi_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	*ret_wk = FrWiFiScr_CommSetSendBuf( wifi_scr_wk, type, param, param2 );
	return 1;
}

//--------------------------------------------------------------
/**
 * ��M
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterRecvBuf( FSS_TASK * core )
{
	u16 wk_id = FSSTGetU16( core );

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = wk_id;

	FSST_SetWait( core, WaitWiFiCounterRecvBuf );
	return 1;
}

//return 1 = �I��
static BOOL WaitWiFiCounterRecvBuf( FSS_TASK * core )
{
	FRWIFI_SCRWORK* wifi_scr_wk;
	//u16 type = FSS_GetEventWorkValue( core, core->reg[0] );		//���ӁI

	wifi_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	//OS_Printf( "wkfi_scr_wk->recieve_count = %d\n", wifi_scr_wk->recieve_count );
	if( wifi_scr_wk->recieve_count >= FRONTIER_COMM_PLAYER_NUM ){
		wifi_scr_wk->recieve_count = 0;
		return 1;
	}

	return 0;
}

//--------------------------------------------------------------
/**
 * �{��No�̔�r
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterBFNoCheck( FSS_TASK * core )
{
	FRWIFI_SCRWORK* wifi_scr_wk;
	u16 bf_no	= FSSTGetWorkValue( core );
	u16* ret_wk	= FSSTGetWork( core );

	wifi_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	if( bf_no == wifi_scr_wk->pair_bf_no ){
		*ret_wk = 1;							//����(OK)
	}else{
		*ret_wk = 0;							//�Ⴄ(NG)
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * �|�P�������X�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"1"
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_WiFiCounterPokeList( FSS_TASK * core )
{
	FRWIFI_SCRWORK* wifi_scr_wk;
	FRONTIER_EX_PARAM* ex_param;
	u16 bf_no	= FSSTGetWorkValue( core );
	u16 type	= FSSTGetWorkValue( core );
	u16* ret_wk	= FSSTGetWork( core );
	
	wifi_scr_wk = Frontier_SysWorkGet( core->fss->fmain );
	ex_param = Frontier_ExParamGet( core->fss->fmain );

#if 0
	//�X�e�[�W
	if( bf_no == FRONTIER_NO_STAGE ){
		//wifi_scr_wk->mode = PL_MODE_BATTLE_STAGE;

	//�L���b�X��
	}else if( bf_no == FRONTIER_NO_CASTLE ){
		//wifi_scr_wk->mode = PL_MODE_BATTLE_CASTLE;

	}else{
		GF_ASSERT(0);
	}
#endif

	//(��)�|�P�������X�g�Ăяo����fsys���K�v
	wifi_scr_wk->fsys	= ex_param->fsys;
	//wifi_scr_wk->bf_no	= bf_no;
	wifi_scr_wk->ret_wk = ret_wk;

	FrWiFiCounter_PokeSelect( core->fss->fmain, wifi_scr_wk, type );

	//FSST_SetWait( core, EvWaitFrWifiCounterPokeList );
	return 1;
}

//return 1 = �I��
static BOOL EvWaitFrWifiCounterPokeList(FSS_TASK * core)
{
	FRWIFI_SCRWORK* wifi_scr_wk;
	wifi_scr_wk = Frontier_SysWorkGet( core->fss->fmain );

	if( 0 ){
		return 1;
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief	�|�P�������X�g�̌��ʂ��擾
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	0
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_WiFiCounterPokeListGetResult(FSS_TASK * core)
{
	int i;
	FRWIFI_SCRWORK* wifi_scr_wk;
	u16 num;
	u16* ret_wk	= FSSTGetWork( core );
	u16* ret2_wk= FSSTGetWork( core );

	wifi_scr_wk = Frontier_SysWorkGet( core->fss->fmain );

	if( wifi_scr_wk->pld->ret_sel == PL_SEL_POS_EXIT ){				//�߂�

		*ret_wk = 0xff;

		//�Q������{�݂��ēx�I�Ԃ��Ƃ��o����̂ŁA���[�N���N���A���Ȃ��Ƃ����Ȃ��I
		//�I�����Ă���ʒu�N���A(��)
		for( i=0; i < WIFI_COUNTER_ENTRY_POKE_MAX ;i++ ){
			wifi_scr_wk->sel[i] = 0;
		}

	}else if( wifi_scr_wk->pld->ret_sel == PL_SEL_POS_ENTER ){		//����

		OS_Printf( "�t�����e�B�AWiFi��t�@�|�P�����I������\n" );

		//���ݑI�΂�Ă���|�P������ۑ�
		*ret_wk = wifi_scr_wk->pld->in_num[0];
		*ret_wk -= 1;												//0-5�ɕύX
		OS_Printf( "*ret_wk = %d\n", *ret_wk );

		*ret2_wk = wifi_scr_wk->pld->in_num[1];
		if( *ret2_wk > 0 ){
			*ret2_wk -= 1;										//0-5�ɕύX
		}
	}

	sys_FreeMemoryEz( wifi_scr_wk->pld );
	wifi_scr_wk->pld = NULL;

	return 0;
}

//--------------------------------------------------------------
/**
 * ���肪�I�񂾃��j���[���擾
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterPairYameruCheck( FSS_TASK * core )
{
	FRWIFI_SCRWORK* wifi_scr_wk;
	u16* ret_wk	= FSSTGetWork( core );

	wifi_scr_wk	=  Frontier_SysWorkGet( core->fss->fmain );
	*ret_wk = wifi_scr_wk->pair_retire_flag;
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�FFRWIFI_SCRWORK�̃Z�b�g�A�Q�b�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterScrWork( FSS_TASK * core )
{
	int i;
	u16 id,my_rensyou;
	BTLTOWER_SCOREWORK* scoreSave;
	FRWIFI_SCRWORK* wifi_scr_wk;
	FRONTIER_EX_PARAM* ex_param;

	u8 code		= FSSTGetU8( core );
	u8 param1	= FSSTGetU8( core );
	u8 param2	= FSSTGetU8( core );
	u16* work	= FSSTGetWork( core );

	wifi_scr_wk	=  Frontier_SysWorkGet( core->fss->fmain );
	ex_param	= Frontier_ExParamGet( core->fss->fmain );

	switch( code ){

	//�ʐM�ݒ�
	case FWIFI_ID_COMM_COMMAND_INITIALIZE:
		CommCommandFrontierInitialize( wifi_scr_wk );
		break;

	//�p�[�g�i�[���|�P�������X�g���L�����Z���������擾
	case FWIFI_ID_GET_PAIR_POKELIST_CANCEL:
		*work = wifi_scr_wk->pair_monsno[0];
		break;

	//�{�݂��Ƃ̂��݂��̃|�P�����������𖞂����Ă��邩�`�F�b�N
	case FWIFI_ID_CHECK_ENTRY:

		//�o�g���^���[
		if( param1 == FRONTIER_NO_TOWER ){

			*work = 0;				//OK

			//�A�C�e���̔��`�F�b�N�͂ǂ��ł��Ă��邩�H�H
	
			//�o��|�P���������Ԃ��Ă��邩�`�F�b�N
			if( (wifi_scr_wk->monsno[0] == wifi_scr_wk->pair_monsno[0]) ||
				(wifi_scr_wk->monsno[0] == wifi_scr_wk->pair_monsno[1]) ){
				*work+=1;			//NG
			}

			if( (wifi_scr_wk->monsno[1] == wifi_scr_wk->pair_monsno[0]) ||
				(wifi_scr_wk->monsno[1] == wifi_scr_wk->pair_monsno[1]) ){
				*work+=2;			//NG
			}

		//�o�g���X�e�[�W
		}else if( param1 == FRONTIER_NO_STAGE ){

			//�����|�P������I��ł��邩�`�F�b�N
			if( wifi_scr_wk->monsno[0] == wifi_scr_wk->pair_monsno[0] ){
				*work = 0;			//OK
			}else{
				*work = 1;			//NG
			}

		//�o�g���L���b�X��
		//�o�g�����[���b�g
		//}else if( param1 == FRONTIER_NO_CASTLE ){
		}else if( (param1 == FRONTIER_NO_CASTLE) || (param1 == FRONTIER_NO_ROULETTE) ){

			*work = 0;				//OK

			//�o��|�P���������Ԃ��Ă��邩�`�F�b�N
			if( (wifi_scr_wk->monsno[0] == wifi_scr_wk->pair_monsno[0]) ||
				(wifi_scr_wk->monsno[0] == wifi_scr_wk->pair_monsno[1]) ){
				*work+=1;			//NG
			}

			if( (wifi_scr_wk->monsno[1] == wifi_scr_wk->pair_monsno[0]) ||
				(wifi_scr_wk->monsno[1] == wifi_scr_wk->pair_monsno[1]) ){
				*work+=2;			//NG
			}
		}
		break;
		
	//�o�g���X�e�[�W�F�A�������擾
	case FWIFI_ID_STAGE_GET_CLEAR_FLAG:
#if 0
		*work = (u16)STAGESCORE_GetScoreData(	SaveData_GetStageScore(ex_param->savedata), 
												STAGESCORE_ID_CLEAR_FLAG, 
												STAGE_TYPE_WIFI_MULTI, 0, NULL);
#else
		*work = (u16)FrontierRecord_Get(SaveData_GetFrontier(ex_param->savedata), 
										FRID_STAGE_MULTI_WIFI_CLEAR_BIT,
										Frontier_GetFriendIndex(FRID_STAGE_MULTI_WIFI_CLEAR_BIT) );
#endif
		break;

	//�o�g���X�e�[�W�F�A�����̃|�P�����i���o�[���擾
	case FWIFI_ID_STAGE_GET_CLEAR_MONSNO:
		*work = FrontierRecord_Get(	SaveData_GetFrontier(ex_param->savedata), 
					StageScr_GetMonsNoRecordID(STAGE_TYPE_WIFI_MULTI),
					Frontier_GetFriendIndex(StageScr_GetMonsNoRecordID(STAGE_TYPE_WIFI_MULTI)) );
		break;

	//�I�������|�P�����i���o�[���擾
	case FWIFI_ID_GET_ENTRY_MONSNO:
		*work = wifi_scr_wk->monsno[param1]; 
		break;

	//�o�g���X�e�[�W�F�A�����t���O�N���A�A�A�����R�[�h�N���A�A�^�C�v�J�E���g�N���A
	case FWIFI_ID_STAGE_SET_NEW_CHALLENGE:
		BattleStageSetNewChallenge( ex_param->savedata, SaveData_GetStageScore(ex_param->savedata),
									STAGE_TYPE_WIFI_MULTI );
		break;

	//�{��No�Z�b�g
	case FWIFI_ID_SET_BF_NO:
		OS_Printf( "�Z�b�g����{��No = %d\n", param1 );
		wifi_scr_wk->bf_no	= param1;
		break;

	//�X�e�[�W�̋L�^�������Ă������̃p�[�g�i�[�̑I�����擾
	case FWIFI_ID_GET_PAIR_STAGE_RECORD_DEL:
		*work = wifi_scr_wk->pair_stage_del;
		break;

	//�{��No�擾
	case FWIFI_ID_GET_BF_NO:
		*work = wifi_scr_wk->bf_no;
		break;

	//�p�[�g�i�[�������ėV�Ԃ��擾
	case FWIFI_ID_GET_PAIR_GAME_CONTINUE:
		*work = wifi_scr_wk->pair_game_continue;
		break;

	//�p�[�g�i�[�Ƃ̘A�������r
	case FWIFI_ID_CHECK_PAIR_RENSYOU:
		*work = 1;
		id = WifiCounterRensyouRecordIDGet( wifi_scr_wk->bf_no );
		my_rensyou = FrontierRecord_Get(SaveData_GetFrontier(wifi_scr_wk->sv), id, 
										Frontier_GetFriendIndex(id) );

		//�A�������H������Ă�����N���A����
		OS_Printf( "my_rensyou = %d\n", my_rensyou );
		OS_Printf( "pair_rensyou = %d\n", wifi_scr_wk->pair_rensyou );
		if( my_rensyou != wifi_scr_wk->pair_rensyou ){
			FrontierRecord_Set(	SaveData_GetFrontier(wifi_scr_wk->sv), 
								id, Frontier_GetFriendIndex(id), 0 );

			//�N���A�t���O���Ƃ�
			id = WifiCounterClearRecordIDGet( wifi_scr_wk->bf_no );
			FrontierRecord_Set(	SaveData_GetFrontier(wifi_scr_wk->sv), 
								id, Frontier_GetFriendIndex(id), 0 );

			OS_Printf( "my_rensyou = %d\n", my_rensyou );
			OS_Printf( "pair_rensyou = %d\n", wifi_scr_wk->pair_rensyou );
			*work = 0;	//NG
		}
		break;

	//�N���A�t���O���擾���āA�N���A
	case FWIFI_ID_GET_CLEAR_FLAG:
		id = WifiCounterClearRecordIDGet( wifi_scr_wk->bf_no );
		*work = FrontierRecord_Get(	SaveData_GetFrontier(wifi_scr_wk->sv),		//�N���A�t���O�擾
									id, Frontier_GetFriendIndex(id) );

		//�N���A�t���O���擾���āA0��������A�A�����R�[�h��0�ɂ��Ă���(�F�B�蒠�̋L�^�̂���)
		if( *work == 0 ){
			id = WifiCounterRensyouRecordIDGet( wifi_scr_wk->bf_no );
			FrontierRecord_Set(	SaveData_GetFrontier(wifi_scr_wk->sv), 
								id, Frontier_GetFriendIndex(id), 0 );

#if 1
			//�^���[�̎��͎��񐔂��N���A
			if( wifi_scr_wk->bf_no == FRONTIER_NO_TOWER ){

				//���݂̎��񐔃��Z�b�g
				scoreSave = SaveData_GetTowerScoreData( wifi_scr_wk->sv );
				TowerScoreData_SetStage( scoreSave, BTWR_MODE_WIFI_MULTI, BTWR_DATA_reset );
			}

			//�L���b�X���̎���CP�N���A
			if( wifi_scr_wk->bf_no == FRONTIER_NO_CASTLE ){

				FrontierRecord_Set(	SaveData_GetFrontier(wifi_scr_wk->sv), 
									FRID_CASTLE_MULTI_WIFI_CP,				//����CP
									Frontier_GetFriendIndex(id), 0 );

				FrontierRecord_Set(	SaveData_GetFrontier(wifi_scr_wk->sv), 
									FRID_CASTLE_MULTI_WIFI_USED_CP,			//�g�p����CP
									Frontier_GetFriendIndex(id), 0 );
			}

			//�t�@�N�g���[LV50�̎��͌����񐔃N���A
			if( wifi_scr_wk->bf_no == FRONTIER_NO_FACTORY_LV50 ){
				FrontierRecord_Set(	SaveData_GetFrontier(wifi_scr_wk->sv),	//������
				FactoryScr_GetTradeRecordID(0,FACTORY_TYPE_WIFI_MULTI),
				Frontier_GetFriendIndex(FactoryScr_GetTradeRecordID(0,FACTORY_TYPE_WIFI_MULTI)), 0);
			}

			//�t�@�N�g���[LV100�̎��͌����񐔃N���A
			if( wifi_scr_wk->bf_no == FRONTIER_NO_FACTORY_LV100 ){
				FrontierRecord_Set(	SaveData_GetFrontier(wifi_scr_wk->sv),	//������
				FactoryScr_GetTradeRecordID(1,FACTORY_TYPE_WIFI_MULTI),
				Frontier_GetFriendIndex(FactoryScr_GetTradeRecordID(1,FACTORY_TYPE_WIFI_MULTI)), 0);
			}
#endif

		}else{
			//�N���A�t���O�𗎂Ƃ��Ă���A�Z�[�u�ɍs���悤�ɂ���(�s���ȏI���̑Ώ�)
			FrontierRecord_Set(	SaveData_GetFrontier(wifi_scr_wk->sv),			//�N���A(ID����)
								id, Frontier_GetFriendIndex(id), 0 );
		}

		OS_Printf( "�A�� = %d\n", *work );
		OS_Printf( "�N���A�t���O = %d\n", *work );
		break;

	//���X�g�p���[�N���N���A(��)
	case FWIFI_ID_POKELIST_SEL_CLEAR:
		for( i=0; i < WIFI_COUNTER_ENTRY_POKE_MAX ;i++ ){
			wifi_scr_wk->sel[i] = 0;
		}
		wifi_scr_wk->pos = 0;
		break;

	};

	return 0;
}

//--------------------------------------------------------------
/**
 * �o�g���^���[�Ăяo���O�̏���
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterTowerCallBefore( FSS_TASK * core )
{
	int i;
	BTOWER_SCRWORK* wk;
	FRONTIER_EX_PARAM* ex_param;
	FRWIFI_SCRWORK* wifi_scr_wk;
	FRWIFI_SCRWORK tmp_wifi_scr_wk;

	ex_param = Frontier_ExParamGet( core->fss->fmain );
	wifi_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	tmp_wifi_scr_wk = *wifi_scr_wk;				//�R�s�[
	FrWiFiCounterScr_WorkFree( wifi_scr_wk );
	OS_Printf( "�X�N���v�g�t�����e�B�AWiFi��t���[�N�J��\n" );

	////////////////////////////
	//�Ăяo�����[�h���m�F����I
	////////////////////////////
	
	//���[�N�G���A���擾���ď�����
	//wk = TowerScr_WorkInit( ex_param->savedata, BTWR_PLAY_NEW, BTWR_MODE_COMM_MULTI );
	wk = TowerScr_WorkInit( ex_param->savedata, BTWR_PLAY_NEW, BTWR_MODE_WIFI_MULTI );
	Frontier_SysWorkSet( core->fss->fmain, wk );

	//�n���V�o�^
	//(�}���`�́A�܂��ʐM���Ă��Ȃ��̂ŁA�X�N���v�g(d32r0201.ev)�ŃZ�b�g���Ă���)
	//if(	(wk->play_mode == BTWR_MODE_COMM_MULTI) || (wk->play_mode == BTWR_MODE_WIFI_MULTI) ){
	if(	wk->play_mode == BTWR_MODE_WIFI_MULTI ){
		FrontierTool_WifiHistory( ex_param->savedata );
	}

	//�o�ꃁ���o�[���Z�b�g
	for( i=0; i < wk->member_num ;i++ ){
		wk->member[i] = tmp_wifi_scr_wk.mine_poke_pos[i];
		wk->mem_poke[i] = tmp_wifi_scr_wk.monsno[i];	
		wk->mem_item[i] = tmp_wifi_scr_wk.itemno[i];	
	}

	wk->pare_poke[0]= tmp_wifi_scr_wk.pair_monsno[0];
	wk->pare_poke[1]= tmp_wifi_scr_wk.pair_monsno[1];
	wk->pare_sex	= tmp_wifi_scr_wk.pair_sex;
	wk->partner		= 5+wk->pare_sex;
#if 0
	wk->pare_stage = recv_buf[3];
#endif

	//�v���`�i�Œǉ����ꂽWIFI�}���`�͒ʏ�̃����_�����g�p����̂ōX�V���Ȃ�
	//�v���C�����_���V�[�h���X�V����
	//TowerScr_PlayRandUpdate( wk, ex_param->savedata );

	//�e�@��������ΐ�g���[�i�[No���I
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		TowerScr_BtlTrainerNoSet( wk, ex_param->savedata );
	}

	//�e�͑ΐ�g���[�i�[No���M
	//�q�̂ݎ�M
	
	//�莝���S��
	PokeParty_RecoverAll( SaveData_GetTemotiPokemon(ex_param->savedata) );

	//BTOWER_SCRWORK�ɐ؂�ւ�
	CommCommandFrontierInitialize( wk );
	wk->recieve_count = 0;

	OS_Printf( "*********FSSC_WiFiCounterTowerCallBefore �I��\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * �o�g���^���[�@�ʐM�}���`�@���I�����g���[�i�[No���q�@�ɑ��M
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterTowerSendTrainerData( FSS_TASK * core )
{
	BTOWER_SCRWORK* wk;
	u16* ret_wk	= FSSTGetWork( core );

	OS_Printf( "\n]]]]]]]]]]]]\n�t�����e�B�AWiFi��t �o�g���^���[�@�g���[�i�[No�𑗐M�J�n\n" );
	wk =  Frontier_SysWorkGet( core->fss->fmain );

	*ret_wk = CommFrWiFiCounterTowerSendBufTrainerData( wk );
	return 1;
}

//--------------------------------------------------------------
/**
 * �o�g���^���[�@�����Ă����g���[�i�[�f�[�^����M�����҂�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"1"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterTowerRecvTrainerData( FSS_TASK * core )
{
	OS_Printf( "\n***********\n�t�����e�B�AWiFi��t �o�g���^���[�@�g���[�i�[No����M�J�n\n" );
	FSST_SetWait( core, EvWaitWiFiCounterTowerRecvTrainerData );
	return 1;
}

static BOOL EvWaitWiFiCounterTowerRecvTrainerData(FSS_TASK * core)
{
	BTOWER_SCRWORK* wk;
	wk =  Frontier_SysWorkGet( core->fss->fmain );

	//��M�l�����`�F�b�N
	if( wk->recieve_count < FRONTIER_COMM_PLAYER_NUM ){
		return FALSE;
	}

	wk->recieve_count = 0;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * �o�g���^���[�Ăяo����̌�n��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterTowerCallAfter( FSS_TASK * core )
{
	BTOWER_SCRWORK* btower_wk;
	btower_wk =  Frontier_SysWorkGet( core->fss->fmain );

	//���[�N�̈���
	TowerScr_WorkRelease( btower_wk );
	btower_wk = NULL;
	OS_Printf( "�o�g���^���[�N�J��\n" );
	return 0;
}



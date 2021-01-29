//============================================================================================
/**
 * @file	fss_roulette_sub.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�T�u�F���[���b�g
 * @author	Satoshi Nohara
 * @date	07.09.06
 */
//============================================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/record.h"
#include "system/buflen.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeparty.h"
#include "system/gamedata.h"
#include "gflib/strbuf_family.h"
#include "../field/fieldsys.h"
#include "savedata/frontier_savedata.h"
#include "savedata/b_tower.h"
#include "field/eventflag.h"
#include "field/evwkdef.h"
#include "../field/sysflag.h"
#include "../field/syswork.h"

#include "graphic/frontier_bg_def.h"
#include "application/roulette.h"
#include "frontier_types.h"
#include "frontier_main.h"
#include "frontier_map.h"
#include "frontier_scr.h"
#include "frontier_tool.h"
#include "frontier_def.h"
#include "frontier_scrcmd.h"
#include "frontier_scrcmd_sub.h"
#include "roulette_tool.h"
#include "fssc_roulette_sub.h"
#include "../field/scr_tool.h"		//RouletteScr_GetWinRecordID
#include "itemtool/itemsym.h"
#include "comm_command_frontier.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_def.h"

#include "battle/tokusyu_def.h"
#include "field/weather_no.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
ROULETTE_SCRWORK* RouletteScr_WorkAlloc( SAVEDATA* savedata, u16 init, u8 type, u16 pos1, u16 pos2, u16 pos3, u16* work );
void RouletteScr_WorkInit( ROULETTE_SCRWORK* wk, u16 init );
static void RouletteScr_InitDataSet( ROULETTE_SCRWORK* wk );
static void RouletteScr_ContinueDataSet( ROULETTE_SCRWORK* wk );
static void RouletteScr_ContinuePokePara( ROULETTE_SCRWORK* wk );
void RouletteScr_WorkRelease( ROULETTE_SCRWORK* wk );
void RouletteScr_GetResult( ROULETTE_SCRWORK* wk, void* roulette_call );
u16 RouletteCall_GetRetWork( void* param, u8 pos );
void RouletteScr_BtlBeforePartySet( ROULETTE_SCRWORK* wk );
void RouletteScr_BtlAfterPartySet( ROULETTE_SCRWORK* wk );
int RouletteScr_GetBtlWinPoint( ROULETTE_SCRWORK* wk, POKEPARTY* party, POKEPARTY* pair_party, int turn );
static int RouletteScr_GetBtlWinPointSub( ROULETTE_SCRWORK* wk, POKEPARTY* party, POKEPARTY* pair_party, u8 m_max );
void RouletteScr_BGPanelChange( void* p_work, FMAP_PTR fmap );
void RouletteScr_MainBGChange( void* p_work, FMAP_PTR fmap );
void RouletteScr_AddPokeIcon( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1 );
void RouletteScr_DelPokeIcon( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1 );
void RouletteScr_MineChgPokeIconVanish( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1, u16 param2 );
void RouletteScr_EnemyChgPokeIconVanish( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1, u16 param2 );
void RouletteScr_AddItemKeepIcon( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1 );
void RouletteScr_DelItemKeepIcon( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1 );
void RouletteScr_ChgItemKeepIconVanish( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1, u16 param2, u16 param3 );
void RouletteScr_ResColorChange( u8 evy, u8 flag );

//�ʐM
BOOL RouletteScr_CommSetSendBuf( ROULETTE_SCRWORK* wk, u16 type, u16 param );


//============================================================================================
//
//	�p�l���̃L�����N�^�A�X�N���[���e�[�u��
//
//============================================================================================
static const u16 roulette_panel_data_tbl[ROULETTE_EV_MAX][3] = {
	{ BR_KEKKA01_HP_NCGR_BIN,		BR_KEKKA01_HP_B_NSCR_BIN, BR_ROOM_KEKKA01_HP_NCLR },		//0
	{ BR_KEKKA02_DOKU_NCGR_BIN,		BR_KEKKA02_DOKU_B_NSCR_BIN, BR_ROOM_KEKKA02_DOKU_NCLR },
	{ BR_KEKKA03_MAHI_NCGR_BIN,		BR_KEKKA03_MAHI_B_NSCR_BIN, BR_ROOM_KEKKA03_MAHI_NCLR },
	{ BR_KEKKA04_YAKEDO_NCGR_BIN,	BR_KEKKA04_YAKEDOI_B_NSCR_BIN, BR_ROOM_KEKKA04_YAKEDOI_NCLR },
	{ BR_KEKKA05_NEMURI_NCGR_BIN,	BR_KEKKA05_NEMURI_B_NSCR_BIN, BR_ROOM_KEKKA05_NEMURI_NCLR },
	{ BR_KEKKA06_KOORI_NCGR_BIN,	BR_KEKKA06_KOORI_B_NSCR_BIN, BR_ROOM_KEKKA06_KOORI_NCLR },
	{ BR_KEKKA07_KINOMI_NCGR_BIN,	BR_KEKKA07_KINOMI_B_NSCR_BIN, BR_ROOM_KEKKA07_KINOMI_NCLR },
	{ BR_KEKKA08_DOUGU_NCGR_BIN,	BR_KEKKA08_DOUGU_B_NSCR_BIN, BR_ROOM_KEKKA08_DOUGU_NCLR },
	{ BR_KEKKA09_LEVEL_NCGR_BIN,	BR_KEKKA09_LEVEL_B_NSCR_BIN, BR_ROOM_KEKKA09_LEVEL_NCLR },
	////////////////////////////
	{ BR_KEKKA01_HP_NCGR_BIN,		BR_KEKKA01_HP_A_NSCR_BIN, BR_ROOM_KEKKA01_HP_NCLR },
	{ BR_KEKKA02_DOKU_NCGR_BIN,		BR_KEKKA02_DOKU_A_NSCR_BIN, BR_ROOM_KEKKA02_DOKU_NCLR },	//10
	{ BR_KEKKA03_MAHI_NCGR_BIN,		BR_KEKKA03_MAHI_A_NSCR_BIN, BR_ROOM_KEKKA03_MAHI_NCLR },
	{ BR_KEKKA04_YAKEDO_NCGR_BIN,	BR_KEKKA04_YAKEDOI_A_NSCR_BIN, BR_ROOM_KEKKA04_YAKEDOI_NCLR },
	{ BR_KEKKA05_NEMURI_NCGR_BIN,	BR_KEKKA05_NEMURI_A_NSCR_BIN, BR_ROOM_KEKKA05_NEMURI_NCLR },
	{ BR_KEKKA06_KOORI_NCGR_BIN,	BR_KEKKA06_KOORI_A_NSCR_BIN, BR_ROOM_KEKKA06_KOORI_NCLR },
	{ BR_KEKKA07_KINOMI_NCGR_BIN,	BR_KEKKA07_KINOMI_A_NSCR_BIN, BR_ROOM_KEKKA07_KINOMI_NCLR },
	{ BR_KEKKA08_DOUGU_NCGR_BIN,	BR_KEKKA08_DOUGU_A_NSCR_BIN, BR_ROOM_KEKKA08_DOUGU_NCLR },
	{ BR_KEKKA09_LEVEL_NCGR_BIN,	BR_KEKKA09_LEVEL_A_NSCR_BIN, BR_ROOM_KEKKA09_LEVEL_NCLR },
	////////////////////////////
	{ BR_KEKKA10_HARE_NCGR_BIN,		BR_KEKKA10_HARE_NSCR_BIN, BR_ROOM_KEKKA10_HARE_NCLR },
	{ BR_KEKKA11_AME_NCGR_BIN,		BR_KEKKA11_AME_NSCR_BIN, BR_ROOM_KEKKA11_AME_NCLR },
	{ BR_KEKKA12_SUNA_NCGR_BIN,		BR_KEKKA12_SUNA_NSCR_BIN, BR_ROOM_KEKKA12_SUNA_NCLR },		//20
	{ BR_KEKKA13_ARARE_NCGR_BIN,	BR_KEKKA13_ARARE_NSCR_BIN, BR_ROOM_KEKKA13_ARARE_NCLR },
	{ BR_KEKKA14_KIRI_NCGR_BIN,		BR_KEKKA14_KIRI_NSCR_BIN, BR_ROOM_KEKKA14_KIRI_NCLR },
	{ BR_KEKKA15_TRICK_NCGR_BIN,	BR_KEKKA15_TRICK_NSCR_BIN, BR_ROOM_KEKKA15_TRICK_NCLR },
	{ BR_KEKKA17_SPEEDUP_NCGR_BIN,	BR_KEKKA17_SPEEDUP_NSCR_BIN, BR_ROOM_KEKKA17_SPEEDUP_NCLR },
	{ BR_KEKKA18_SPEEDDN_NCGR_BIN,	BR_KEKKA18_SPEEDDN_NSCR_BIN, BR_ROOM_KEKKA18_SPEEDDN_NCLR },//25
	{ BR_KEKKA19_RANDOM_NCGR_BIN,	BR_KEKKA19_RANDOM_NSCR_BIN, BR_ROOM_KEKKA19_RANDOM_NCLR },
	////////////////////////////
	{ BR_KEKKA16_CHANGE_NCGR_BIN,	BR_KEKKA16_CHANGE_NSCR_BIN, BR_ROOM_KEKKA16_CHANGE_NCLR },	//27
	{ BR_KEKKA21_BP_NCGR_BIN,		BR_KEKKA21_BP_NSCR_BIN, BR_ROOM_KEKKA21_BP_NCLR },			//28
	{ BR_KEKKA20_THROUGH_NCGR_BIN,	BR_KEKKA20_THROUGH_NSCR_BIN, BR_ROOM_KEKKA20_THROUGH_NCLR },//29
	{ BR_KEKKA22_SUKA_NCGR_BIN,		BR_KEKKA22_SUKA_NSCR_BIN, BR_ROOM_KEKKA22_SUKA_NCLR },		//30
	{ BR_KEKKA23_BP2_NCGR_BIN,		BR_KEKKA23_BP2_NSCR_BIN, BR_ROOM_KEKKA23_BP2_NCLR },		//31
};


//============================================================================================
//
//	���񐔂ɂ���āA���펞�̊�{�̃��[���b�g���x�����߂�
//
//============================================================================================
static const u8 roulette_basic_spd_tbl[][2] = {
	//����	��{���x
	{ 7,		5 },		//7-
	{ 4,		4 },		//4-6
	{ 0,		3 },		//1-3
};
#define ROULETTE_BASIC_SPD_TBL_MAX	( NELEMS(roulette_basic_spd_tbl) )


//============================================================================================
//
//	�֐�
//
//============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���[�N�G���A�m��
 *
 * @param	none
 *	
 * �����Ȃ炸 RouletteScr_WorkRelease()�ŗ̈���J�����邱��
 */
//--------------------------------------------------------------
ROULETTE_SCRWORK* RouletteScr_WorkAlloc( SAVEDATA* savedata, u16 init, u8 type, u16 pos1, u16 pos2, u16 pos3, u16* work )
{
	u32 buf,exp;
	POKEPARTY* temoti_party;
	POKEMON_PARAM* temp_poke;
	ROULETTEDATA* ca_sv;
	u8 clear_flag,m_max;
	u16	i,chg_flg;
	static ROULETTE_SCRWORK* wk;					//���ƂŊm�F]]]]]]]]]]]]]]]]]]]]]]]]]
	ROULETTESCORE* score_sv;

	wk = sys_AllocMemory( HEAPID_WORLD, sizeof(ROULETTE_SCRWORK) );
	MI_CpuClear8( wk, sizeof(ROULETTE_SCRWORK) );

	//�Z�[�u�f�[�^�擾
	wk->roulette_savedata	= SaveData_GetRouletteData( savedata );
	wk->sv				= savedata;
	wk->heapID			= HEAPID_WORLD;
	wk->p_m_party		= PokeParty_AllocPartyWork( HEAPID_WORLD );
	wk->p_e_party		= PokeParty_AllocPartyWork( HEAPID_WORLD );
	wk->list_del_work	= work;
	wk->decide_ev_no	= ROULETTE_EV_NONE;
	
	ca_sv = wk->roulette_savedata;
	score_sv = SaveData_GetRouletteScore( savedata );

	//�V�K���A�p����
	if( init == 0 ){
		wk->type	= type;
		wk->round	= 0;
		//RouletteScr_InitDataSet(...)�̒��ŁA���񐔂ɂ���Ċ�{���x��ݒ肵�Ă���
		wk->csr_speed_level = CSR_SPEED_LEVEL_DEFAULT;
		wk->random_flag = 0;

		ROULETTEDATA_Init( ca_sv );	//���f�f�[�^������

		//WIFI�̂ݓ���
		if( wk->type == ROULETTE_TYPE_WIFI_MULTI ){
#if 0
			clear_flag = FrontierRecord_Get(SaveData_GetFrontier(wk->sv), 
									FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT,
									Frontier_GetFriendIndex(FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT) );
#else
			clear_flag = SysWork_WifiFrClearFlagGet( SaveData_GetEventWork(wk->sv) );
#endif

		}else{
			//�N���A�������t���O���擾
			clear_flag = (u8)ROULETTESCORE_GetScoreData(score_sv, ROULETTESCORE_ID_CLEAR_FLAG, 
														wk->type, 0, NULL );
		}

		if( clear_flag == 1 ){
			wk->rensyou = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
									RouletteScr_GetWinRecordID(wk->type),
									Frontier_GetFriendIndex(RouletteScr_GetWinRecordID(wk->type)) );
		}else{
			wk->rensyou = 0;
		}

		wk->lap		= (u16)(wk->rensyou / ROULETTE_LAP_ENEMY_MAX);
		wk->win_cnt	= 0;

		//�I�������莝���̈ʒu
		wk->mine_poke_pos[0] = pos1;
		wk->mine_poke_pos[1] = pos2;
		wk->mine_poke_pos[2] = pos3;
	}else{
		//���݂̃v���C�i�s�f�[�^�擾
		wk->type = (u8)ROULETTEDATA_GetPlayData( ca_sv, ROULETTEDATA_ID_TYPE, 0, 0, NULL);
		wk->round = (u8)ROULETTEDATA_GetPlayData( ca_sv, ROULETTEDATA_ID_ROUND, 0, 0, NULL);
		wk->csr_speed_level = (u8)ROULETTEDATA_GetPlayData( ca_sv, 
													ROULETTEDATA_ID_CSR_SPEED_LEVEL, 0, 0, NULL);
		wk->random_flag = (u8)ROULETTEDATA_GetPlayData( ca_sv, 
													ROULETTEDATA_ID_RANDOM_FLAG, 0, 0, NULL);
		wk->rensyou = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
									RouletteScr_GetWinRecordID(wk->type),
									Frontier_GetFriendIndex(RouletteScr_GetWinRecordID(wk->type)) );
		wk->lap		= (u16)(wk->rensyou / ROULETTE_LAP_ENEMY_MAX);

		for( i=0; i < ROULETTE_ENTRY_POKE_MAX ;i++ ){
			wk->mine_poke_pos[i] = (u8)ROULETTEDATA_GetPlayData(ca_sv, 
																ROULETTEDATA_ID_MINE_POKE_POS, 
																i, 0, NULL);
		}
	}

	//�����A�C�e����ۑ����Ă���
	for( i=0; i < ROULETTE_ENTRY_POKE_MAX ;i++ ){
		temp_poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(wk->sv), 
												wk->mine_poke_pos[i] );
		wk->itemno[i] = PokeParaGet( temp_poke, ID_PARA_item, NULL );
	}

	//�o��|�P�������Z�b�g
	temoti_party = SaveData_GetTemotiPokemon( wk->sv );
	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_SOLO );		//����
	for( i=0; i < m_max; i++ ){

		PokeParty_Add(	wk->p_m_party, 
						PokeParty_GetMemberPointer(temoti_party,wk->mine_poke_pos[i]) );

		////////////////////////////////////////////////////////////
		//�ۑ����Ă������AHP�APP�A�A�C�e���Ȃǂ����ɖ߂��Ȃ��ƃ_���I
		////////////////////////////////////////////////////////////
		
		//�A�C�e�����O��
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, i );
		buf = 0;
		PokeParaPut( temp_poke, ID_PARA_item, &buf );

		//���x������
		if( PokeParaGet(temp_poke,ID_PARA_level,NULL) > 50 ){
			exp = PokeLevelExpGet( PokeParaGet(temp_poke,ID_PARA_monsno,NULL), 50 );
			PokeParaPut( temp_poke, ID_PARA_exp, &exp );
			PokeParaCalc( temp_poke );
		}
	}

	//�n���V�o�^
	if( Roulette_CommCheck(wk->type) == TRUE ){
		FrontierTool_WifiHistory( wk->sv );
	}

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	���[�N�G���A���擾���ď���������
 *
 * @param	savedata	�Z�[�u�f�[�^�ւ̃|�C���^
 * @param	init		���������[�h BTWR_PLAY_NEW:���߂���ABTWR_PLAY_CONTINE:��������
 *	
 * �����Ȃ炸 RouletteScr_WorkRelease()�ŗ̈���J�����邱��
 */
//--------------------------------------------------------------
void RouletteScr_WorkInit( ROULETTE_SCRWORK* wk, u16 init )
{
	//�V�K���A�p����
	if( init == 0 ){
		//OS_Printf( "�O �V�K ����wk = %d\n", wk );
		RouletteScr_InitDataSet( wk );
		//OS_Printf( "�� �V�K ����wk = %d\n", wk );
	}else{
		//OS_Printf( "�O �p�� ����wk = %d\n", wk );
		RouletteScr_ContinueDataSet( wk );
		//OS_Printf( "�� �p�� ����wk = %d\n", wk );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�K�F�g���[�i�[�A�|�P�����f�[�^�𐶐�
 *
 * @param	wk		ROULETTE_SCRWORK�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void RouletteScr_InitDataSet( ROULETTE_SCRWORK* wk )
{
	u16 lap;
	int i;
	B_TOWER_POKEMON poke;
	POKEMON_PARAM* temp_poke;
	POKEPARTY* temoti_party;
	u16 poke_check_tbl[6];	//rental
	u16 item_check_tbl[6];	//rental

	OS_Printf( "�V�K�F�f�[�^�𐶐�\n" );

	//07.09.25 ��Ɏ���*2�̃g���[�i�[���擾
	//�V���O��(0-6)�A�}���`�̃p�[�g�i�[(7-13)�Ŏ擾
	//���̎��ɓo�ꂷ��g���[�i�[��index��S�Ď擾
	Roulette_EnemyLapAllTrainerIndexGet(	wk->type, RouletteScr_CommGetLap(wk), wk->tr_index, 
											ROULETTE_LAP_MULTI_ENEMY_MAX );
	OS_Printf( "lap = %d\n", RouletteScr_CommGetLap(wk) );

	//�f�o�b�N���
	for( i=0; i < ROULETTE_LAP_MULTI_ENEMY_MAX ;i++ ){
		OS_Printf( "tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}

	//�G�g���[�i�[���o���|�P��������
	Frontier_EnemyPokeMake(	Roulette_GetEnemyPokeNum(wk->type,ROULETTE_FLAG_TOTAL),
							wk->tr_index[wk->round], 
							wk->tr_index[wk->round+ROULETTE_LAP_ENEMY_MAX], 
							wk->enemy_poke_index, 
							wk->enemy_poke,						//B_TOWER_POKEMON
							wk->enemy_pow_rnd,
							wk->enemy_personal_rnd,
							Roulette_CommCheck(wk->type) );

	for( i=0; i < 4 ;i++ ){
		OS_Printf( "enemy_poke[%d] = %d\n", i, wk->enemy_poke[i].mons_no );
		OS_Printf( "enemy_index[%d] = %d\n", i, wk->enemy_poke_index[i] );
		OS_Printf( "enemy_pow[%d] = %d\n", i, wk->enemy_pow_rnd[i] );
		OS_Printf( "enemy_personal[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
	}

	//�ʐM�ő���̎��񐔂��擾������ɏ�������
	lap = RouletteScr_CommGetLap( wk );

	//���񐔂ɂ���āA���펞�̊�{�̃��[���b�g���x�����߂�
	for( i=0; i < ROULETTE_BASIC_SPD_TBL_MAX ;i++ ){
		if( lap >= roulette_basic_spd_tbl[i][0] ){
			wk->csr_speed_level = roulette_basic_spd_tbl[i][1];
			OS_Printf( "basic spd = %d\n", wk->csr_speed_level );
			break;
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���F�ۑ����Ă������f�[�^���g����B_TOWER_POKEMON�Ȃǂ𐶐�
 *
 * @param	wk		ROULETTE_SCRWORK�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void RouletteScr_ContinueDataSet( ROULETTE_SCRWORK* wk )
{
	u16 buf;
	int i;
	POKEMON_PARAM* pp;
	u8 count;
	B_TOWER_POKEMON bt_poke[ROULETTE_PARTY_POKE_MAX];
	u8 pow_rnd[ROULETTE_PARTY_POKE_MAX];
	u16 poke_index[ROULETTE_PARTY_POKE_MAX];
	u32 personal_rnd[ROULETTE_PARTY_POKE_MAX];
	OS_Printf( "�p���F�ۑ����Ă������f�[�^�����[�h\n" );

	//"�莝���|�P�����̏��"���[�h
	RouletteScr_ContinuePokePara( wk );

	//-----------------------------------------------------------------------------------
	
	//�ۑ����Ă������g���[�i�[��index�擾
	for( i=0; i < ROULETTE_LAP_MULTI_ENEMY_MAX ;i++ ){
		wk->tr_index[i] = (u16)ROULETTEDATA_GetPlayData(	wk->roulette_savedata, 
														ROULETTEDATA_ID_TR_INDEX, i, 0, NULL );
		//OS_Printf( "tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}

	//---------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------
	//�ۑ����Ă������G�|�P�����擾
	for( i=0; i < ROULETTE_ENEMY_POKE_MAX ;i++ ){

		poke_index[i] = (u16)ROULETTEDATA_GetPlayData(wk->roulette_savedata, 
												ROULETTEDATA_ID_ENEMY_POKE_INDEX, i, 0, NULL );
		//OS_Printf( "poke_index[%d] = %d\n", i, poke_index[i] );
		
		//�G�|�P�����̃C���f�b�N�X�i�[
		wk->enemy_poke_index[i] = poke_index[i];
	}

	//�莝���|�P��������x�ɐ���(personal�Ȃ�)
	Frontier_PokemonParamCreateAll(	bt_poke, poke_index, pow_rnd,
									NULL, personal_rnd, ROULETTE_ENEMY_POKE_MAX, HEAPID_WORLD,
									ARC_PL_BTD_PM );

	pp = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < ROULETTE_ENEMY_POKE_MAX ;i++ ){
		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake( &bt_poke[i], pp, Roulette_GetLevel(wk) );
		Roulette_PokePartyAdd( wk, wk->p_e_party, pp );
		OS_Printf( "�p��e_party[%d] monsno = %d\n", i, PokeParaGet(pp,ID_PARA_monsno,NULL) );

	}
	sys_FreeMemoryEz( pp );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���FHP�APP�ACONDITION�AITEM�����[�h
 *
 * @param	wk		ROULETTE_SCRWORK�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void RouletteScr_ContinuePokePara( ROULETTE_SCRWORK* wk )
{
	u8 count;
	u16 buf;
	int i;
	POKEMON_PARAM* pp;

	count = PokeParty_GetPokeCount( wk->p_m_party );

	//"�莝���|�P�����̏��"���[�h
	for( i=0; i < count ;i++ ){

		pp = PokeParty_GetMemberPointer( wk->p_m_party, i );

		buf = (u16)ROULETTEDATA_GetPlayData(wk->roulette_savedata, 
											ROULETTEDATA_ID_TEMOTI_ITEM, i, 0, NULL );
		PokeParaPut( pp, ID_PARA_item, &buf );			//ITEM
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���[�N�G���A���J������
 */
//--------------------------------------------------------------
void RouletteScr_WorkRelease( ROULETTE_SCRWORK* wk )
{
	int	heap;

	if( wk == NULL ){
		return;
	}

	if( wk->p_m_party != NULL ){
		sys_FreeMemoryEz( wk->p_m_party );
	}

	if( wk->p_e_party != NULL ){
		sys_FreeMemoryEz( wk->p_e_party );
	}

	MI_CpuClear8( wk, sizeof(ROULETTE_SCRWORK) );
	sys_FreeMemoryEz( wk );
	wk = NULL;

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���[���b�g��ʌĂяo����̌��ʎ擾
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void RouletteScr_GetResult( ROULETTE_SCRWORK* wk, void* roulette_call )
{
	int i;
	ROULETTE_CALL_WORK* call_wk = roulette_call;

	//���ʂ��擾����
	for( i=0; i < ROULETTE_RET_WORK_MAX ;i++ ){
		wk->ret_work[i] = RouletteCall_GetRetWork( roulette_call, i );
		OS_Printf( "wk->ret_work[%d] = %d\n", i, wk->ret_work[i] );
	}

	return;
}

//----------------------------------------------------------------------------
/**
 * @brief	�|�P�����I����ʂł̌��ʂ��擾
 *
 * @param	param	ROULETTE_CALL_WORK�̃|�C���^
 * @param	pos		ret_work[pos](0-5)
 *
 * @return	"����"
 */
//----------------------------------------------------------------------------
u16 RouletteCall_GetRetWork( void* param, u8 pos )
{
	ROULETTE_CALL_WORK* roulette_call = param;

	if( pos >= ROULETTE_RET_WORK_MAX ){
		GF_ASSERT(0);
		return 0;
	}

	return roulette_call->ret_work;
}


/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/
/*****************************************************/
void RouletteScr_SaveRestPlayData( ROULETTE_SCRWORK* wk, u8 mode );
u16	RouletteScr_IncRound( ROULETTE_SCRWORK* wk );
u16	RouletteScr_GetRound( ROULETTE_SCRWORK* wk );
u16 RouletteScr_GetEnemyObjCode( ROULETTE_SCRWORK* wk, u8 param );
u16 RouletteScr_GetTrIndex( ROULETTE_SCRWORK* wk, u8 param );
void RouletteScr_SetLose( ROULETTE_SCRWORK* wk );
void RouletteScr_SetClear( ROULETTE_SCRWORK* wk );

//--------------------------------------------------------------
/**
 * @brief	�x�ނƂ��Ɍ��݂̃v���C�󋵂��Z�[�u�ɏ����o��
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	mode	FR_MODE_CLEAR="�N���A",FR_MODE_LOSE="����",FR_MODE_REST="�x��"
 */
//--------------------------------------------------------------
void RouletteScr_SaveRestPlayData( ROULETTE_SCRWORK* wk, u8 mode )
{
	u16	i;
	u8 e_max;
	u8	buf8[4];
	u16	buf16[4];
	u32	buf32[4];
	u32 after,count;
	POKEMON_PARAM* pp;
	ROULETTEDATA* f_sv = wk->roulette_savedata;
	ROULETTESCORE* score_sv = SaveData_GetRouletteScore( wk->sv );

	//�^�C�v�ɂ���ă|�P�����̐����擾
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );	//�G

	//"�V���O���A�_�u���A�}���`�Awifi�}���`"�����o��
	buf8[0] = wk->type;
	ROULETTEDATA_PutPlayData( wk->roulette_savedata, ROULETTEDATA_ID_TYPE, 0, 0, buf8 );
	
	//�Z�[�u�t���O��L����ԂɃ��Z�b�g
	ROULETTEDATA_SetSaveFlag( wk->roulette_savedata, TRUE );

	//"���E���h��"�����o��(0-6�l�ڂ̉��l�ڂ�������炷)
	buf8[0] = wk->round;
	//OS_Printf( "wk->round = %d\n", wk->round );
	ROULETTEDATA_PutPlayData( wk->roulette_savedata, ROULETTEDATA_ID_ROUND, 0, 0, buf8 );

	//"�J�[�\���X�s�[�h���x��"�����o��
	buf8[0] = wk->csr_speed_level;
	//OS_Printf( "wk->csr_speed_level = %d\n", wk->csr_speed_level );
	ROULETTEDATA_PutPlayData( wk->roulette_savedata, ROULETTEDATA_ID_CSR_SPEED_LEVEL, 0, 0, buf8 );

	//"�J�[�\�������_���ړ��t���O"�����o��
	buf8[0] = wk->random_flag;
	//OS_Printf( "wk->random_flag = %d\n", wk->random_flag );
	ROULETTEDATA_PutPlayData( wk->roulette_savedata, ROULETTEDATA_ID_RANDOM_FLAG, 0, 0, buf8 );

#if 0
	//�A�� / 7 = ����
	//�A�� % 7 = ���l�ڂ�
#endif
	//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
	OS_Printf( "rensyou = %d\n", wk->rensyou );
	FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
						RouletteScr_GetWinRecordID(wk->type),
						Frontier_GetFriendIndex(RouletteScr_GetWinRecordID(wk->type)), wk->rensyou);
	
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//"�x��"�ȊO
	if( mode != FR_MODE_REST ){

		//"�ő�A����"�����o��
		after = FrontierRecord_SetIfLarge(	SaveData_GetFrontier(wk->sv),
								RouletteScr_GetMaxWinRecordID(wk->type),
								Frontier_GetFriendIndex(RouletteScr_GetMaxWinRecordID(wk->type)), 
								wk->rensyou );
		OS_Printf( "ROULETTE after = %d\n", after  );

		//"7�A��(�N���A)�������t���O"�����o��
		buf8[0] = wk->clear_flag;
		ROULETTESCORE_PutScoreData( score_sv, ROULETTESCORE_ID_CLEAR_FLAG, wk->type, 0, buf8 );

		//WIFI�̂ݓ���
		if( wk->type == ROULETTE_TYPE_WIFI_MULTI ){
			FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
					FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT,
					Frontier_GetFriendIndex(FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT), wk->clear_flag );
		}
	}
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	//"�g���[�i�[�C���f�b�N�X"�����o��
	for( i=0; i < ROULETTE_LAP_MULTI_ENEMY_MAX ;i++ ){
		buf16[0] = wk->tr_index[i];
		ROULETTEDATA_PutPlayData( wk->roulette_savedata, ROULETTEDATA_ID_TR_INDEX, i, 0, buf16 );
	}

	//"�莝���ʒu"�����o��
	for( i=0; i < ROULETTE_ENTRY_POKE_MAX ;i++ ){
		buf8[0] = wk->mine_poke_pos[i];
		ROULETTEDATA_PutPlayData(wk->roulette_savedata, ROULETTEDATA_ID_MINE_POKE_POS, i, 0, buf8 );
	}

	//"�莝���|�P�����̏��"�����o��
	count = PokeParty_GetPokeCount( wk->p_m_party );
	for( i=0; i < count ;i++ ){

		pp = PokeParty_GetMemberPointer( wk->p_m_party, i );

		buf16[0] = PokeParaGet( pp, ID_PARA_item, NULL );		//ITEM
		ROULETTEDATA_PutPlayData( wk->roulette_savedata, ROULETTEDATA_ID_TEMOTI_ITEM, i, 0, buf16 );
	}

	//�G
	count = PokeParty_GetPokeCount( wk->p_e_party );
	for( i=0; i < count ;i++ ){

		pp = PokeParty_GetMemberPointer( wk->p_e_party, i );

		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		//�����������X�^�[�i���o�[�ł͂Ȃ��I
		//�C���f�b�N�X�I
		//buf16[0] = PokeParaGet( pp, ID_PARA_monsno, NULL );
		buf16[0] = wk->enemy_poke_index[i];
		ROULETTEDATA_PutPlayData(	wk->roulette_savedata, ROULETTEDATA_ID_ENEMY_POKE_INDEX, 
									i, 0, buf16 );
		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���E���h�����C���N�������g
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
u16	RouletteScr_IncRound( ROULETTE_SCRWORK* wk )
{
	wk->round++;
	return wk->round;
}

//--------------------------------------------------------------
/**
 * @brief	���E���h�����擾
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
u16	RouletteScr_GetRound( ROULETTE_SCRWORK* wk )
{
	return wk->round;
}

//--------------------------------------------------------------
/**
 * @brief	�ΐ�g���[�i�[OBJ�R�[�h�擾
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	param	0=1�l�ځA1=2�l��(�G�̃p�[�g�i�|)
 */
//--------------------------------------------------------------
u16 RouletteScr_GetEnemyObjCode( ROULETTE_SCRWORK* wk, u8 param )
{
	B_TOWER_TRAINER bt_trd;
	B_TOWER_TRAINER_ROM_DATA* p_rom_tr;
	u8 index;

	//�擾����g���[�i�[�f�[�^�̃C���f�b�N�X���擾
	index = RouletteScr_GetTrIndex( wk, param );

	//ROM����g���[�i�[�f�[�^���m��
	p_rom_tr = Frontier_TrainerDataGet( &bt_trd, wk->tr_index[index], HEAPID_WORLD, ARC_PL_BTD_TR );
	sys_FreeMemoryEz( p_rom_tr );

	OS_Printf( "1�l�ځA2�l�ځH = %d\n", param );
	OS_Printf( "�G�g���[�i�[�^�C�v = %d\n", bt_trd.tr_type );
	OS_Printf( "�G�g���[�i�[OBJ�R�[�h = %d\n", Frontier_TrType2ObjCode(bt_trd.tr_type) );

	//�g���[�i�[�^�C�v����OBJ�R�[�h���擾���Ă���
	return Frontier_TrType2ObjCode( bt_trd.tr_type );
}

//--------------------------------------------------------------
/**
 * @brief	wk->tr_index[�g���[�i�[�f�[�^�̃C���f�b�N�X���擾]
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	param	0=1�l�ځA1=2�l��(�G�̃p�[�g�i�|)
 */
//--------------------------------------------------------------
u16 RouletteScr_GetTrIndex( ROULETTE_SCRWORK* wk, u8 param )
{
	return ( wk->round + (param * ROULETTE_LAP_ENEMY_MAX) );
}

//--------------------------------------------------------------
/**
 * @brief	�s�킵�����̏���
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void RouletteScr_SetLose( ROULETTE_SCRWORK* wk )
{
	u16 buf16[4];
	int i;

	OS_Printf( "\n�o�g�����[���b�g�f�[�^�@�s��Z�b�g\n" );

	//���݂�5�A���Ȃǂ�ۑ�����K�v������I
	RouletteScr_SaveRestPlayData( wk, FR_MODE_LOSE );		//�Z�[�u�f�[�^�ɑ��

	//�V�K���A�p�����́AWK_SCENE_ROULETTE_LOBBY�Ɋi�[����Ă���̂ŁA
	//������round�Ȃǂ��N���A���Ȃ��Ă��A
	//WK_SCENE_ROULETTE_LOBBY���p���ł͂Ȃ���ԂŃZ�[�u�����̂ŁA
	//��t�ɘb�������Ă��A�V�K����ɂȂ胏�[�N�̓N���A�����B
	return;
}

//--------------------------------------------------------------
/**
 * @brief	7�A��(�N���A)�������̏���
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void RouletteScr_SetClear( ROULETTE_SCRWORK* wk )
{
	OS_Printf( "\n�o�g�����[���b�g�f�[�^�@7�A��(�N���A)�Z�b�g\n" );
	
	/***********************************************************/
	//
	//���R�[�h�ɂ��f�[�^��ێ�����K�v�����邩�͕ۗ�
	//
	//
	//�X�R�A�E�B���h�E���J�����ɎQ�Ƃ���f�[�^���A
	//���R�[�h�Ƃ������H�H����Ƃ��A�ʐM�œn���f�[�^�H�H
	//
	/***********************************************************/

	wk->clear_flag = 1;						//7�A��(�N���A)�������t���OON

	//wk->rensyou		= 0;				//���݂̘A����
	if( wk->lap < ROULETTE_LAP_MAX ){
		wk->lap++;							//���񐔂̃C���N�������g
	}

#if 0
	//RouletteScr_CommGetLap�ł��ꂪ�o�Ȃ��悤�Ƀy�A���X�V
	if( wk->pair_lap < ROULETTE_LAP_MAX ){
		wk->pair_lap++;						//���񐔂̃C���N�������g
	}
#endif

	wk->round			= 0;				//�����l�ځH
	RouletteScr_SaveRestPlayData( wk, FR_MODE_CLEAR );		//�Z�[�u�f�[�^�ɑ��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�퓬�O��POKEPARTY�̃Z�b�g
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void RouletteScr_BtlBeforePartySet( ROULETTE_SCRWORK* wk )
{
	Roulette_EnemyPartySet( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�o�g�����POKEPARTY�̃Z�b�g
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void RouletteScr_BtlAfterPartySet( ROULETTE_SCRWORK* wk )
{
	int i,count,tmp_count;
	u8 e_max;
#if 0
	u16 check_monsno[ ROULETTE_TOTAL_POKE_MAX ];		//mine=4,enemy=4 = 8
	u16 check_itemno[ ROULETTE_TOTAL_POKE_MAX ];
	POKEMON_PARAM* temp_poke;
	B_TOWER_TRAINER bt_trd;
	B_TOWER_TRAINER_ROM_DATA* trd;

	//������	
	for( i=0; i < ROULETTE_TOTAL_POKE_MAX ; i++ ){	
		check_monsno[i] = 0;
		check_itemno[i] = 0;
	}

	//�^�C�v�ɂ���ă|�P�����̐����擾
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );//�G

	count = PokeParty_GetPokeCount( wk->p_m_party );
	for( i=0; i < count ; i++ ){									//�莝���|�P����
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, i );
		check_monsno[i] = PokeParaGet(temp_poke, ID_PARA_monsno, NULL);
		check_itemno[i] = PokeParaGet(temp_poke, ID_PARA_item, NULL);
	}
	tmp_count = count;

	count = PokeParty_GetPokeCount( wk->p_e_party );
	for( i=0; i < count ; i++ ){									//�G�|�P����
		temp_poke = PokeParty_GetMemberPointer( wk->p_e_party, i );
		check_monsno[i+tmp_count] = PokeParaGet(temp_poke, ID_PARA_monsno, NULL);
		check_itemno[i+tmp_count] = PokeParaGet(temp_poke, ID_PARA_item, NULL);
	}

	//ROM����g���[�i�[�f�[�^���m��
	trd = Frontier_TrainerDataGet( &bt_trd, wk->tr_index[wk->round], HEAPID_WORLD, ARC_PL_BTD_TR );

	SDK_ASSERTMSG( (tmp_count + count) <= ROULETTE_TOTAL_POKE_MAX, "check buf�̗v�f��������Ȃ��I");

	//���|�P�A�A�C�e���A�f�[�^���A�擾���鐔�A�����
	Frontier_PokemonIndexCreate(trd, check_monsno, check_itemno, 
								(tmp_count + count),				//�莝���A�G�̐�
								e_max, wk->enemy_poke_index, HEAPID_WORLD );

	sys_FreeMemoryEz( trd );

	//�G�|�P��������x�ɐ���(personal�Ȃ�)
	Frontier_PokemonParamCreateAll(	wk->enemy_poke, wk->enemy_poke_index, 
									wk->enemy_pow_rnd, NULL, wk->enemy_personal_rnd, 
									e_max, HEAPID_WORLD, ARC_PL_BTD_PM );
#else
	//�G�g���[�i�[���o���|�P��������
	Frontier_EnemyPokeMake(	Roulette_GetEnemyPokeNum(wk->type,ROULETTE_FLAG_TOTAL),
							wk->tr_index[wk->round], 
							wk->tr_index[wk->round+ROULETTE_LAP_ENEMY_MAX], 
							wk->enemy_poke_index, 
							wk->enemy_poke,						//B_TOWER_POKEMON
							wk->enemy_pow_rnd,
							wk->enemy_personal_rnd,
							Roulette_CommCheck(wk->type) );

	for( i=0; i < 4 ;i++ ){
		OS_Printf( "enemy_poke[%d] = %d\n", i, wk->enemy_poke[i].mons_no );
		OS_Printf( "enemy_index[%d] = %d\n", i, wk->enemy_poke_index[i] );
		OS_Printf( "enemy_pow[%d] = %d\n", i, wk->enemy_pow_rnd[i] );
		OS_Printf( "enemy_personal[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
	}
#endif

	//�G�p�[�e�B�̃Z�b�g
	Roulette_EnemyPartySet( wk );

	return;
}

#define ROULETTE_POINT_TBL_MAX	(5)
static const u8 roulette_bad_condition_point_tbl[ROULETTE_POINT_TBL_MAX] = { 8, 6, 4, 0, 0 };
static const u8 roulette_hinsi_point_tbl[ROULETTE_POINT_TBL_MAX] = { 6, 4, 2, 0, 0 };
static const u8 roulette_turn_point_tbl[ROULETTE_POINT_TBL_MAX][2] = {
	{ 3, 10 }, 
	{ 5,  6 },
	{ 7,  4 },
	{ 9,  2 },
	{ 10, 0 },
};

//--------------------------------------------------------------
/**
 * @brief	�o�g��������̃|�C���g�v�Z
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 *
 * @return	"�|�C���g"
 */
//--------------------------------------------------------------
int RouletteScr_GetBtlWinPoint( ROULETTE_SCRWORK* wk, POKEPARTY* party, POKEPARTY* pair_party, int turn )
{
	u8 m_max;
	int i;
	POKEMON_PARAM* pp;
	int ret;

	OS_Printf( "********************\n�퓬��̃|�C���g�v�Z\n" );

	ret = 0;
	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_SOLO );

	//�m���A��Ԃ̃|�C���g
	ret += RouletteScr_GetBtlWinPointSub( wk, party, pair_party, m_max );

	//�^�[�����̃|�C���g
	for( i=0; i < ROULETTE_POINT_TBL_MAX ;i++ ){
		if( turn < roulette_turn_point_tbl[i][0] ){
			ret += roulette_turn_point_tbl[i][1];
			OS_Printf( "�|�C���g = %d\n\n", roulette_turn_point_tbl[i][1] );
			break;
		}
	}
	OS_Printf( "turn�� = %d\n", turn );

	OS_Printf( "�|�C���g���v = %d\n", ret );
	return ret;
}

static int RouletteScr_GetBtlWinPointSub( ROULETTE_SCRWORK* wk, POKEPARTY* party, POKEPARTY* pair_party, u8 m_max )
{
	u32 hp;
	u8 tmp_hinsi;
	u8 tmp_bad_condition;
	int i,ret;
	POKEMON_PARAM* pp;

	//�N���A
	ret = 0;
	tmp_hinsi = 0;
	tmp_bad_condition = 0;

	for( i=0; i < m_max; i++ ){

		pp = PokeParty_GetMemberPointer( party, i );
		OS_Printf( "monsno = %d\n", PokeParaGet(pp,ID_PARA_monsno,NULL) );

		if( PokeParaGet(pp,ID_PARA_poke_exist,NULL) == FALSE ){
			continue;
		}

		hp = PokeParaGet( pp, ID_PARA_hp, NULL );

		//�C�₵�Ă���|�P������
		if( hp == 0 ){
			tmp_hinsi++;
		}

		//��Ԉُ�ɂȂ��Ă���|�P������
		if( PokeParaGet(pp,ID_PARA_condition,NULL) != 0 ){
			tmp_bad_condition++;
		}
	}

	//�ʐM�̎��̓y�A�̃|�P�����̏�Ԃ��`�F�b�N����
	if( Roulette_CommCheck(wk->type) == TRUE ){

		for( i=0; i < m_max; i++ ){

			pp = PokeParty_GetMemberPointer( pair_party, i );
			OS_Printf( "monsno = %d\n", PokeParaGet(pp,ID_PARA_monsno,NULL) );

			if( PokeParaGet(pp,ID_PARA_poke_exist,NULL) == FALSE ){
				continue;
			}

			hp = PokeParaGet( pp, ID_PARA_hp, NULL );

			//�C�₵�Ă���|�P������
			if( hp == 0 ){
				tmp_hinsi++;
			}

			//��Ԉُ�ɂȂ��Ă���|�P������
			if( PokeParaGet(pp,ID_PARA_condition,NULL) != 0 ){
				tmp_bad_condition++;
			}
		}
	}

	//�|�C���g�v�Z
	ret += roulette_bad_condition_point_tbl[tmp_bad_condition];
	OS_Printf( "ID_BAD_CONDITION�̐� = %d\n", tmp_bad_condition );
	OS_Printf( "�|�C���g = %d\n\n", roulette_bad_condition_point_tbl[tmp_bad_condition] );

	ret += roulette_hinsi_point_tbl[tmp_hinsi];
	OS_Printf( "ID_HINSI�̐� = %d\n", tmp_hinsi );
	OS_Printf( "�|�C���g = %d\n\n", roulette_hinsi_point_tbl[tmp_hinsi] );

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	�p�l����ύX����
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void RouletteScr_BGPanelChange( void* p_work, FMAP_PTR fmap )
{
	ARCHANDLE *hdl;
	ROULETTE_SCRWORK* wk;
	wk = (ROULETTE_SCRWORK*)p_work;

	OS_Printf( "�p�l���ύX�֐����Ă΂�܂����I\n" );
	OS_Printf( "wk->decide_ev_no = %d\n", wk->decide_ev_no );

	if( wk->decide_ev_no != ROULETTE_EV_NONE ){

		hdl = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_FRONTIERMAP );
		
		ArcUtil_HDL_BgCharSet(	hdl, roulette_panel_data_tbl[wk->decide_ev_no][0],		//�L����
								fmap->bgl, FRMAP_FRAME_EFF, 0, 0, TRUE, HEAPID_FRONTIERMAP );

		ArcUtil_HDL_ScrnSet(	hdl, roulette_panel_data_tbl[wk->decide_ev_no][1],		//�X�N���[��
								fmap->bgl, FRMAP_FRAME_EFF, 0, 0, 1, HEAPID_FRONTIERMAP );

		{
			//�p���b�g�f�[�^���g���p���b�gVRAM�ɓ]��
			NNSG2dPaletteData *palData;
			void *pal_work;
				
			pal_work = ArcUtil_HDL_PalDataGet(	hdl, 
												roulette_panel_data_tbl[wk->decide_ev_no][2],
												&palData, HEAPID_FRONTIERMAP );
			DC_FlushRange( palData->pRawData, palData->szByte );
			GX_BeginLoadBGExtPltt();	   //�p���b�g�f�[�^�̓]������
			GX_LoadBGExtPltt( palData->pRawData, 0x4000, 0x2000 );
		    GX_EndLoadBGExtPltt();         //�p���b�g�f�[�^�̓]������
		    sys_FreeMemoryEz( pal_work );
		}

		//GF_Disp_GX_VisibleControl( FRMAP_FRAME_EFF, VISIBLE_ON );		//BG�\��
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );		//BG�\��
		GF_BGL_LoadScreenV_Req( fmap->bgl, FRMAP_FRAME_EFF );

		ArchiveDataHandleClose( hdl );
	}else{
		//GF_Disp_GX_VisibleControl( FRMAP_FRAME_EFF, VISIBLE_OFF );	//BG��\��
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//BG��\��
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�w�i��ύX����
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void RouletteScr_MainBGChange( void* p_work, FMAP_PTR fmap )
{
	ARCHANDLE *hdl;
	ROULETTE_SCRWORK* wk;
	wk = (ROULETTE_SCRWORK*)p_work;

	OS_Printf( "�w�i�ύX�֐����Ă΂�܂����I\n" );

	hdl = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_FRONTIERMAP );
		
	ArcUtil_HDL_ScrnSet(	hdl, BR_ROOM_MUL_NSCR_BIN,		//�X�N���[��
							fmap->bgl, FRMAP_FRAME_MAP, 0, 0, 1, HEAPID_FRONTIERMAP );

	GF_BGL_LoadScreenV_Req( fmap->bgl, FRMAP_FRAME_MAP );

	ArchiveDataHandleClose( hdl );
	return;
}

typedef struct{
	s16  x;
	s16  y;
}POS;

//�������̃|�P�����A�C�R���\���ʒu
static const POS mine_icon_pos_tbl[] = {
	{ 33, 20 },
	{ 39, 43 },
	{ 43, 66 },
	{ 46, 89 },
};

//�G���̃|�P�����A�C�R���\���ʒu
static const POS enemy_icon_pos_tbl[] = {
	{ 222, 20 },
	{ 216, 43 },
	{ 213, 66 },
	{ 210, 89 },
};

#define ITEM_KEEP_OFFSET_X	(8)			//�A�C�e�������Ă���A�C�R���̕\���I�t�Z�b�gX
#define ITEM_KEEP_OFFSET_Y	(4)			//�A�C�e�������Ă���A�C�R���̕\���I�t�Z�b�gY

//--------------------------------------------------------------
/**
 * @brief	�|�P�����A�C�R���ǉ�
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	param1	0=�����A1=����
 */
//--------------------------------------------------------------
void RouletteScr_AddPokeIcon( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1 )
{
	u8 m_max,e_max;
	int i;
	POKEMON_PARAM* pp;

	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );

	if( param1 == 0 ){		//����
		for( i=0; i < m_max ;i++ ){	
			pp = PokeParty_GetMemberPointer( wk->p_m_party, i );
			wk->m_pokeicon[i] = Frontier_PokeIconAdd( fmap, pp, i, 
												mine_icon_pos_tbl[i].x, mine_icon_pos_tbl[i].y );
			CLACT_SetAnmFlag( wk->m_pokeicon[i]->act, 0 );		//�A�j����~
			//CLACT_SetDrawFlag( wk->m_pokeicon[i]->act, 0 );
		}
	}else{					//�G
		for( i=0; i < e_max ;i++ ){	
			pp = PokeParty_GetMemberPointer( wk->p_e_party, i );
			wk->e_pokeicon[i] = Frontier_PokeIconAdd( fmap, pp, i+m_max,
												enemy_icon_pos_tbl[i].x, enemy_icon_pos_tbl[i].y );
			CLACT_SetAnmFlag( wk->e_pokeicon[i]->act, 0 );		//�A�j����~
			//CLACT_SetDrawFlag( wk->e_pokeicon[i]->act, 0 );
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P�����A�C�R���폜
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	param1	0=�����A1=����
 */
//--------------------------------------------------------------
void RouletteScr_DelPokeIcon( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1 )
{
	u8 m_max,e_max;
	int i;

	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );

	if( param1 == 0 ){		//����
		for( i=0; i < m_max ;i++ ){	
			FSSC_Sub_PokeIconDel(	fmap, wk->m_pokeicon[i], i );
		}
	}else{					//�G
		for( i=0; i < e_max ;i++ ){	
			FSSC_Sub_PokeIconDel(	fmap, wk->e_pokeicon[i], i+m_max );
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	(����)�|�P�����A�C�R���o�j�b�V������
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	param1	0=��\���A1=�\��
 * @param	param2	���Ԗڂ�
 */
//--------------------------------------------------------------
void RouletteScr_MineChgPokeIconVanish( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1, u16 param2 )
{
	u8 m_max;

	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );

	//�s���Ȓl�͉������Ȃ�
	if( param2 >= m_max ){
		return;
	}

	if( param1 == 1){
		CLACT_SetDrawFlag( wk->m_pokeicon[param2]->act, 1 );		//�\��
	}else{
		CLACT_SetDrawFlag( wk->m_pokeicon[param2]->act, 0 );		//��\��
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	(�G)�|�P�����A�C�R���o�j�b�V������
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	param1	0=��\���A1=�\��
 * @param	param2	���Ԗڂ�
 */
//--------------------------------------------------------------
void RouletteScr_EnemyChgPokeIconVanish( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1, u16 param2 )
{
	u8 e_max;

	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );

	//�s���Ȓl�͉������Ȃ�
	if( param2 >= e_max ){
		return;
	}

	if( param1 == 1){
		CLACT_SetDrawFlag( wk->e_pokeicon[param2]->act, 1 );		//�\��
	}else{
		CLACT_SetDrawFlag( wk->e_pokeicon[param2]->act, 0 );		//��\��
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�e�������Ă��邩�A�C�R���ǉ�
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	param1	0=�����A1=����
 */
//--------------------------------------------------------------
void RouletteScr_AddItemKeepIcon( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1 )
{
	u8 m_max,e_max;
	int i;
	POKEMON_PARAM* pp;

	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );

	if( param1 == 0 ){		//����
		for( i=0; i < m_max ;i++ ){	
			wk->m_itemkeep[i] = Frontier_ItemIconAdd( fmap, 
										mine_icon_pos_tbl[i].x + ITEM_KEEP_OFFSET_X, 
										mine_icon_pos_tbl[i].y + ITEM_KEEP_OFFSET_Y );

			//�����Ă��Ȃ����͔�\��
			pp =  PokeParty_GetMemberPointer( wk->p_m_party, i );
			if( PokeParaGet(pp,ID_PARA_item,NULL) == 0 ){
				CLACT_SetDrawFlag( wk->m_itemkeep[i]->act, 0 );
			}
		}
	}else{					//�G
		for( i=0; i < e_max ;i++ ){	
			wk->e_itemkeep[i] = Frontier_ItemIconAdd( fmap,
										enemy_icon_pos_tbl[i].x + ITEM_KEEP_OFFSET_X, 
										enemy_icon_pos_tbl[i].y + ITEM_KEEP_OFFSET_Y );

			//�����Ă��Ȃ����͔�\��
			pp =  PokeParty_GetMemberPointer( wk->p_e_party, i );
			if( PokeParaGet(pp,ID_PARA_item,NULL) == 0 ){
				CLACT_SetDrawFlag( wk->e_itemkeep[i]->act, 0 );
			}
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�e�������Ă��邩�A�C�R���폜
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	param1	0=�����A1=����
 */
//--------------------------------------------------------------
void RouletteScr_DelItemKeepIcon( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1 )
{
	u8 m_max,e_max;
	int i;

	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );

	if( param1 == 0 ){		//����
		for( i=0; i < m_max ;i++ ){	
			FSSC_Sub_ItemIconDel(	fmap, wk->m_itemkeep[i] );
		}
	}else{					//�G
		for( i=0; i < e_max ;i++ ){	
			FSSC_Sub_ItemIconDel(	fmap, wk->e_itemkeep[i] );
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�e�������Ă��邩�A�C�R���̃o�j�b�V������
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 * @param	param1	0=�����A1=����
 * @param	param2	���Ԗڂ�
 * @param	param3	1=�\���A0=��\��
 */
//--------------------------------------------------------------
void RouletteScr_ChgItemKeepIconVanish( ROULETTE_SCRWORK* wk, FMAP_PTR fmap, u16 param1, u16 param2, u16 param3 )
{
	u32 item;
	u8 m_max,e_max;
	POKEMON_PARAM* pp;

	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );

	if( param1 == 0 ){		//����

		//�s���Ȓl�͉������Ȃ�
		if( param2 >= m_max ){
			return;
		}

		pp =  PokeParty_GetMemberPointer( wk->p_m_party, param2 );
		item = PokeParaGet( pp, ID_PARA_item, NULL );

		//�\����������
		if( param3 == 1 ){
			if( item == 0 ){
				CLACT_SetDrawFlag( wk->m_itemkeep[param2]->act, 0 );		//��\��
			}else{
				CLACT_SetDrawFlag( wk->m_itemkeep[param2]->act, 1 );		//�\��
			}

		//��\���ɂ�������
		}else{
			CLACT_SetDrawFlag( wk->m_itemkeep[param2]->act, 0 );			//��\��
		}

	}else{					//�G

		//�s���Ȓl�͉������Ȃ�
		if( param2 >= e_max ){
			return;
		}

		pp =  PokeParty_GetMemberPointer( wk->p_e_party, param2 );
		item = PokeParaGet( pp, ID_PARA_item, NULL );

		//�\����������
		if( param3 == 1 ){
			if( item == 0 ){
				CLACT_SetDrawFlag( wk->e_itemkeep[param2]->act, 0 );		//��\��
			}else{
				CLACT_SetDrawFlag( wk->e_itemkeep[param2]->act, 1 );		//�\��
			}

		//��\���ɂ�����
		}else{
			CLACT_SetDrawFlag( wk->e_itemkeep[param2]->act, 0 );			//��\��
		}
	}

	return;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J���[�p���b�g�ύX(�g���p���b�g�p)
 *
 *	@param	evy		evy
 */
//-----------------------------------------------------------------------------
#define ROULETTE_EX_PAL_SIZE		(0x1000)
#define ROULETTE_WAY_EX_PAL_NUM		(16)				//16�{
#define ROULETTE_BTL_EX_PAL_NUM		(9)					//9�{
#define ROULETTE_ROUKA_EX_PAL_NUM	(15)				//15�{

void RouletteScr_ResColorChange( u8 evy, u8 flag )
{
	u32 index,size;
	NNSG2dPaletteData *palData;
	void *p_work;
	ARCHANDLE *hdl_bg;
	u16* dest;

	//�ʘH�A�ΐ핔���Ńp���b�g���Ⴄ�̂ŁA�p���b�g�f�[�^��"�]���T�C�Y"�𕪂���
	if( flag == 0 ){
		index = BR_ROUKA_NCLR;

#if 0
		//(256(16*16)�J���[*16�{) = 0x1000 = 4096 * 2 = 8196 = 0x2000
		size  = (ROULETTE_EX_PAL_SIZE * 2);
#else
		size  = (ROULETTE_ROUKA_EX_PAL_NUM * 16) * 2;
#endif
	}else{
		index = BR_ROOM_NCLR;

		//256�F���t���Ɏg���Ă��Ȃ����߁B
		size  = (ROULETTE_BTL_EX_PAL_NUM * 16) * 2;
	}

	//�f�[�^�쐬��
	dest = sys_AllocMemory( HEAPID_FRONTIERMAP, ROULETTE_EX_PAL_SIZE*2 );
	memset( dest, 0, ROULETTE_EX_PAL_SIZE*2 );

	hdl_bg = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_FRONTIERMAP );
	p_work = ArcUtil_HDL_PalDataGet(hdl_bg, index, &palData, HEAPID_FRONTIERMAP );

	//OS_Printf( "pal size = %d\n", palData->szByte );			//8192 = 0x2000
	
	//256�Fx16�p���b�g 8k = 8192
	SoftFade( palData->pRawData, dest, ROULETTE_EX_PAL_SIZE, evy, 0x0000 );

	//512byte = 16�F x 16
	//512byte = 256�F x 1
	//8kbyte  = 256�F * 16

	//������DMA�]�����s���̂ŁA�O�����ē]�����̃f�[�^��DC_FlushRange����p����
	//���C���������ɏ����o���Ă����K�v������܂��B
	//DC_FlushRange(palData->pRawData, palData->szByte);
	DC_FlushRange( dest, ROULETTE_EX_PAL_SIZE*2 );

	GX_BeginLoadBGExtPltt();	   // �p���b�g�f�[�^�̓]������

	//GX_LoadBGExtPltt(palData->pRawData, 0x6000, ROULETTE_EX_PAL_SIZE*2);
	GX_LoadBGExtPltt( dest, 0x6000, size );

	GX_EndLoadBGExtPltt();         // �p���b�g�f�[�^�̓]������

	ArchiveDataHandleClose( hdl_bg );
	sys_FreeMemoryEz( dest );
	sys_FreeMemoryEz(p_work);
	return;
}


//==============================================================================================
//
//	�ʐM(CommStart)
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���M�E�F�C�g�@
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
BOOL RouletteScr_CommSetSendBuf( ROULETTE_SCRWORK* wk, u16 type, u16 param )
{
	int ret;

	switch( type ){

	case ROULETTE_COMM_BASIC:
		ret = CommRouletteSendBufBasicData( wk );
		break;

	case ROULETTE_COMM_TR:
		ret = CommRouletteSendBufTrData( wk );
		break;

	case ROULETTE_COMM_ENEMY:
		ret = CommRouletteSendBufEnemyPokeData( wk );
		break;

	case ROULETTE_COMM_RETIRE:
		ret = CommRouletteSendBufRetireFlag( wk, param );
		break;

	case ROULETTE_COMM_TRADE_YESNO:
		ret = CommRouletteSendBufTradeYesNoFlag( wk, param );
		break;

	case ROULETTE_COMM_TEMOTI:
		ret = CommRouletteSendBufTemotiPokeData( wk );
		break;
	};

	return ret;
}


//==============================================================================================
//
//	�p�l���C�x���g����
//
//==============================================================================================

//==============================================================================
//
//	����X�g(���񐔂ɂ���Ďg�p����e�[�u����������)
//
//	�E3(4)�C�S�ĂɎ�������
//	�E���łɉ��������Ă��鎞�́A�����㏑������
//
//==============================================================================
//����1-3
static const u16 roulette_item_1_3[] = {
	ITEM_OUZYANOSIRUSI,
	ITEM_SENSEINOTUME,
	ITEM_HIKARINOKONA,
	ITEM_METORONOOMU,
	ITEM_OOKINANEKKO,
	///////////////////////////////////
	ITEM_KIAINOHATIMAKI,
	ITEM_KIAINOTASUKI,
	ITEM_TABENOKOSI,
	///////////////////////////////////
	ITEM_KUROITEKKYUU,
	ITEM_KOUKOUNOSIPPO,
	ITEM_KUROIHEDORO,
};
#define ROULETTE_ITEM_1_3_MAX	( NELEMS(roulette_item_1_3) )

//����4-6
static const u16 roulette_item_4_6[] = {
	ITEM_SIROIHAABU,
	ITEM_PAWAHURUHAABU,
	ITEM_TIKARANOHATIMAKI,
	ITEM_MONOSIRIMEGANE,
	ITEM_KAIGARANOSUZU,
	///////////////////////////////////
	ITEM_PINTORENZU,
	ITEM_KOUKAKURENZU,
	ITEM_FOOKASURENZU,
	ITEM_TATUZINNOOBI,
	ITEM_INOTINOTAMA,
	///////////////////////////////////
	ITEM_KUROITEKKYUU,
	ITEM_KOUKOUNOSIPPO,
	ITEM_KUROIHEDORO,
};
#define ROULETTE_ITEM_4_6_MAX	( NELEMS(roulette_item_4_6) )

//����7
static const u16 roulette_item_7[] = {
	ITEM_KIAINOHATIMAKI,
	ITEM_KIAINOTASUKI,
	ITEM_TABENOKOSI,
	///////////////////////////////////
	ITEM_PINTORENZU,
	ITEM_KOUKAKURENZU,
	ITEM_FOOKASURENZU,
	ITEM_TATUZINNOOBI,
	ITEM_INOTINOTAMA,
	///////////////////////////////////
	ITEM_KODAWARIHATIMAKI,
	ITEM_KODAWARIMEGANE,
	ITEM_KODAWARISUKAAHU,
};
#define ROULETTE_ITEM_7_MAX	( NELEMS(roulette_item_7) )

//�e�[�u��
static const u16* roulette_item_tbl[] = {
	roulette_item_1_3,
	roulette_item_4_6,
	roulette_item_7,
};
#define ROULETTE_ITEM_TBL_MAX	( NELEMS(roulette_item_tbl) )


//==============================================================================
//
//	�؂̎����X�g(���񐔂ɂ���Ďg�p����e�[�u����������)
//
//	�E3(4)�C�S�ĂɎ�������
//	�E���łɉ��������Ă��鎞�́A�����㏑������
//
//==============================================================================
//����1-3
static const u16 roulette_seed_1_3[] = {
	ITEM_KURABONOMI,
	ITEM_KAGONOMI,
	ITEM_MOMONNOMI,
	ITEM_TIIGONOMI,
	ITEM_NANASINOMI,
	///////////////////////////////////
	ITEM_KIINOMI,
	ITEM_OBONNOMI,
	ITEM_RAMUNOMI,
};
#define ROULETTE_SEED_1_3_MAX	( NELEMS(roulette_seed_1_3) )

//����4-6
static const u16 roulette_seed_4_6[] = {
	///////////////////////////////////
	ITEM_OKKANOMI,
	ITEM_ITOKENOMI,
	ITEM_SOKUNONOMI,
	ITEM_RINDONOMI,
	ITEM_YATHENOMI,
	ITEM_YOPUNOMI,
	ITEM_BIAANOMI,
	ITEM_SYUKANOMI,
	ITEM_BAKOUNOMI,
	ITEM_UTANNOMI,
	ITEM_TANGANOMI,
	ITEM_YOROGINOMI,
	ITEM_KASIBUNOMI,
	ITEM_HABANNOMI,
	ITEM_NAMONOMI,
	ITEM_RIRIBANOMI,
	ITEM_HOZUNOMI,
	///////////////////////////////////
	ITEM_KIINOMI,
	ITEM_OBONNOMI,
	ITEM_RAMUNOMI,
};
#define ROULETTE_SEED_4_6_MAX	( NELEMS(roulette_seed_4_6) )

//����7
static const u16 roulette_seed_7[] = {
	ITEM_KIINOMI,
	ITEM_OBONNOMI,
	ITEM_RAMUNOMI,
	///////////////////////////////////
	ITEM_TIIRANOMI,
	ITEM_RYUGANOMI,
	ITEM_KAMURANOMI,
	ITEM_YATAPINOMI,
	ITEM_ZUANOMI,
	ITEM_SANNOMI,
	ITEM_SUTAANOMI,
};
#define ROULETTE_SEED_7_MAX	( NELEMS(roulette_seed_7) )

//�e�[�u��
static const u16* roulette_seed_tbl[] = {
	roulette_seed_1_3,
	roulette_seed_4_6,
	roulette_seed_7,
};
#define ROULETTE_SEED_TBL_MAX	( NELEMS(roulette_seed_tbl) )

typedef void (*rouletteFunc)(ROULETTE_SCRWORK* wk,POKEPARTY* party, u8 num);	//�֐��|�C���^�^

//�C�x���g
static void Roulette_EvHpDown( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvDoku( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvMahi( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvYakedo( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvNemuri( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvKoori( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvSeedGet( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvItemGet( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvLvUp( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvPlaceHare( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvPlaceAme( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvPlaceSunaarasi( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvPlaceArare( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvPlaceKiri( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvPlaceTrick( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvPlaceSpeedUp( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvPlaceSpeedDown( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvPlaceRandom( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvExPokeChange( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvExBPGet( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvExBtlWin( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );
static void Roulette_EvExSuka( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num );

//�C�x���g�e�[�u��
static void* ev_func_tbl[] = {
	Roulette_EvHpDown,
	Roulette_EvDoku,
	Roulette_EvMahi,
	Roulette_EvYakedo,
	Roulette_EvNemuri,
	Roulette_EvKoori,
	Roulette_EvSeedGet,
	Roulette_EvItemGet,
	Roulette_EvLvUp,
	/////////////////////////////////
	Roulette_EvHpDown,			//9
	Roulette_EvDoku,
	Roulette_EvMahi,
	Roulette_EvYakedo,
	Roulette_EvNemuri,
	Roulette_EvKoori,
	Roulette_EvSeedGet,
	Roulette_EvItemGet,
	Roulette_EvLvUp,
	/////////////////////////////////
	Roulette_EvPlaceHare,		//18
	Roulette_EvPlaceAme,
	Roulette_EvPlaceSunaarasi,
	Roulette_EvPlaceArare,
	Roulette_EvPlaceKiri,
	Roulette_EvPlaceTrick,
	Roulette_EvPlaceSpeedUp,
	Roulette_EvPlaceSpeedDown,
	Roulette_EvPlaceRandom,
	/////////////////////////////////
	Roulette_EvExPokeChange,	//27
	Roulette_EvExBPGet,
	Roulette_EvExBtlWin,
	Roulette_EvExSuka,
	Roulette_EvExBPGet,			//31
};

//--------------------------------------------------------------
/**
 * @brief	�C�x���g���s
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
void Roulette_EvSet( ROULETTE_SCRWORK* wk, u8 no )
{
	u8 max;
	POKEPARTY* party;
	rouletteFunc func;

	if( no < ROULETTE_EV_MINE_START ){
		party = wk->p_e_party;
		max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );
	}else{
		party = wk->p_m_party;
		max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );
	}

	func = ev_func_tbl[no];
	func( wk, party, max );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F�G�F�g�o����
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvHpDown( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	int i;
	u32 hpmax,hp;
	POKEMON_PARAM* poke;

	OS_Printf( "�C�x���g�F�g�o����\n" );

	for( i=0; i < num ;i++ ){
		poke =  PokeParty_GetMemberPointer( party, i );
		hpmax = PokeParaGet( poke, ID_PARA_hpmax, NULL );		//�ő�HP

		hp = (hpmax * 1.2);
		hp -= hpmax;		//20%
		hp = (hpmax - hp);

		PokeParaPut( poke, ID_PARA_hp, &hp );
		//PokeParaCalc( poke );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F�G�F��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvDoku( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	int i,ng_count;
	u32 condition,type1,type2,spe;
	POKEMON_PARAM* poke;

	OS_Printf( "�C�x���g�F��\n" );

	ng_count = 0;

	for( i=0; i < num ;i++ ){
		poke =  PokeParty_GetMemberPointer( party, i );

		type1 = PokeParaGet( poke, ID_PARA_type1, NULL );
		type2 = PokeParaGet( poke, ID_PARA_type2, NULL );
		spe	  = PokeParaGet( poke, ID_PARA_speabino, NULL );

		//�ŁA�|�A�Ɖu�̓_���I
		if(	(type1 == POISON_TYPE) || (type2 == POISON_TYPE) ||
			(type1 == METAL_TYPE) || (type2 == METAL_TYPE) || (spe == TOKUSYU_MENEKI) ){
			ng_count++;
		//
		}else{
			condition = CONDITION_DOKU;
			PokeParaPut( poke, ID_PARA_condition, &condition );
			//PokeParaCalc( poke );
		}
	}	

	//�S�ă`�F�b�N���Ă��鎞�́A�N���X�ɂ��邱�Ƃ��o���Ȃ�����
	if( ng_count >= num ){
		wk->poke_safe_flag = 1;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F�G�F���
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvMahi( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	int i,ng_count;
	u32 condition,type1,type2,spe;
	POKEMON_PARAM* poke;

	OS_Printf( "�C�x���g�F���\n" );

	ng_count = 0;

	for( i=0; i < num ;i++ ){
		poke =  PokeParty_GetMemberPointer( party, i );

		type1 = PokeParaGet( poke, ID_PARA_type1, NULL );
		type2 = PokeParaGet( poke, ID_PARA_type2, NULL );
		spe	  = PokeParaGet( poke, ID_PARA_speabino, NULL );

		//���߂�A���イ�Ȃ�̓_���I
		if(	(type1 == JIMEN_TYPE) || (type2 == JIMEN_TYPE) || (spe == TOKUSYU_ZYUUNAN) ){
			ng_count++;
		//
		}else{
			condition = CONDITION_MAHI;
			PokeParaPut( poke, ID_PARA_condition, &condition );
			//PokeParaCalc( poke );
		}
	}	

	//�S�ă`�F�b�N���Ă��鎞�́A�N���X�ɂ��邱�Ƃ��o���Ȃ�����
	if( ng_count >= num ){
		wk->poke_safe_flag = 1;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F�G�F�₯��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvYakedo( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	int i,ng_count;
	u32 condition,type1,type2,spe;
	POKEMON_PARAM* poke;

	OS_Printf( "�C�x���g�F�₯��\n" );

	ng_count = 0;

	for( i=0; i < num ;i++ ){
		poke =  PokeParty_GetMemberPointer( party, i );

		type1 = PokeParaGet( poke, ID_PARA_type1, NULL );
		type2 = PokeParaGet( poke, ID_PARA_type2, NULL );
		spe	  = PokeParaGet( poke, ID_PARA_speabino, NULL );

		//���A�݂��̃x�[���̓_���I
		if(	(type1 == FIRE_TYPE) || (type2 == FIRE_TYPE) || (spe == TOKUSYU_MIZUNOBEERU) ){
			ng_count++;
		//
		}else{
			condition = CONDITION_YAKEDO;
			PokeParaPut( poke, ID_PARA_condition, &condition );
			//PokeParaCalc( poke );
		}
	}	

	//�S�ă`�F�b�N���Ă��鎞�́A�N���X�ɂ��邱�Ƃ��o���Ȃ�����
	if( ng_count >= num ){
		wk->poke_safe_flag = 1;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F�G�F�˂ނ�
 *
 * @param	
 *
 * @return	none
 *
 * �d�l�ύX�őΏۂ�1�l�Œ�ƂȂ�܂����B
 */
//--------------------------------------------------------------
static void Roulette_EvNemuri( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	u8 check_pos;
	int i,ng_count;
	u32 condition,spe;
	POKEMON_PARAM* poke;

	OS_Printf( "�C�x���g�F����\n" );

	ng_count = 0;

	//�|�P�p�[�e�B�̂ǂ̈ʒu����`�F�b�N���Ă���������
	check_pos = ( wk->rand_pos % num );

	for( i=0; i < num ;i++ ){
		poke =  PokeParty_GetMemberPointer( party, check_pos );
		//OS_Printf( "�|�P�����i���o�[ = %d\n", PokeParaGet(poke,ID_PARA_monsno,NULL) );

		spe	  = PokeParaGet( poke, ID_PARA_speabino, NULL );

		//�ӂ݂�A��邫�̓_���I
		if(	(spe == TOKUSYU_HUMIN) || (spe == TOKUSYU_YARUKI) ){
			ng_count++;
			check_pos++;
			if( check_pos >= num ){
				check_pos = 0;
			}
		//
		}else{
			//condition = CONDITION_NEMURI;
			condition = ( gf_rand() % 4 + 2 );		//2-5�^�[������ɂȂ�
			PokeParaPut( poke, ID_PARA_condition, &condition );
			//PokeParaCalc( poke );
			break;
		}	
	}	

	//�S�ă`�F�b�N���Ă��鎞�́A�N���X�ɂ��邱�Ƃ��o���Ȃ�����
	if( ng_count >= num ){
		wk->poke_safe_flag = 1;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F�G�F������
 *
 * @param	
 *
 * @return	none
 *
 * �d�l�ύX�őΏۂ�1�l�Œ�ƂȂ�܂����B
 */
//--------------------------------------------------------------
static void Roulette_EvKoori( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	u8 check_pos;
	int i,ng_count;
	u32 condition,type1,type2,spe;
	POKEMON_PARAM* poke;

	OS_Printf( "�C�x���g�F�X\n" );

	ng_count = 0;

	//�|�P�p�[�e�B�̂ǂ̈ʒu����`�F�b�N���Ă���������
	check_pos = ( wk->rand_pos % num );

	for( i=0; i < num ;i++ ){
		poke =  PokeParty_GetMemberPointer( party, check_pos );

		type1 = PokeParaGet( poke, ID_PARA_type1, NULL );
		type2 = PokeParaGet( poke, ID_PARA_type2, NULL );
		spe	  = PokeParaGet( poke, ID_PARA_speabino, NULL );

		//������A�}�O�}�̂�낢�̓_���I
		if(	(type1 == KOORI_TYPE) || (type2 == KOORI_TYPE) || (spe == TOKUSYU_MAGUMANOYOROI) ){
			ng_count++;
			check_pos++;
			if( check_pos >= num ){
				check_pos = 0;
			}
		//
		}else{
			condition = CONDITION_KOORI;
			PokeParaPut( poke, ID_PARA_condition, &condition );
			//PokeParaCalc( poke );
			break;
		}	
	}	

	//�S�ă`�F�b�N���Ă��鎞�́A�N���X�ɂ��邱�Ƃ��o���Ȃ�����
	if( ng_count >= num ){
		wk->poke_safe_flag = 1;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F�G�F�؂̎��Q�b�g
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvSeedGet( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	int i,max;
	const u16* seed_tbl;
	u16 item,lap;
	POKEMON_PARAM* poke;

	OS_Printf( "�C�x���g�F�؂̎�\n" );

	lap = RouletteScr_CommGetLap( wk );

	if( lap < 3 ){
		seed_tbl = roulette_seed_tbl[0];
		max = ROULETTE_SEED_1_3_MAX;
	}else if( lap < 6 ){
		seed_tbl = roulette_seed_tbl[1];
		max = ROULETTE_SEED_4_6_MAX;
	}else{
		seed_tbl = roulette_seed_tbl[2];
		max = ROULETTE_SEED_7_MAX;
	}

	item = seed_tbl[ (wk->rand_pos % max) ];
	OS_Printf( "�Z�b�g����؂̎��i���o�[ = %d\n", item );

#if 0	//�f�o�b�N�I
	item = ITEM_ORENNOMI;
#endif

	for( i=0; i < num ;i++ ){
		poke =  PokeParty_GetMemberPointer( party, i );
		PokeParaPut( poke, ID_PARA_item, &item );
		//PokeParaCalc( poke );
	}	

	//�A�C�e�������Ă��邩�A�C�R���̃o�j�b�V���Z�b�g
	//Roulette_ItemKeepVanishSet( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F�G�F�A�C�e���Q�b�g
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvItemGet( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	int i,max;
	const u16* item_tbl;
	u16 item,lap;
	POKEMON_PARAM* poke;

	OS_Printf( "�C�x���g�F�A�C�e��\n" );

	lap = RouletteScr_CommGetLap( wk );

	if( lap < 3 ){
		item_tbl = roulette_item_tbl[0];
		max = ROULETTE_ITEM_1_3_MAX;
	}else if( lap < 6 ){
		item_tbl = roulette_item_tbl[1];
		max = ROULETTE_ITEM_4_6_MAX;
	}else{
		item_tbl = roulette_item_tbl[2];
		max = ROULETTE_ITEM_7_MAX;
	}

	item = item_tbl[ (wk->rand_pos % max) ];
	OS_Printf( "�Z�b�g����A�C�e���i���o�[ = %d\n", item );

	for( i=0; i < num ;i++ ){
		poke =  PokeParty_GetMemberPointer( party, i );
		PokeParaPut( poke, ID_PARA_item, &item );
		//PokeParaCalc( poke );
	}	

	//�A�C�e�������Ă��邩�A�C�R���̃o�j�b�V���Z�b�g
	//Roulette_ItemKeepVanishSet( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F�G�FLV�A�b�v
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvLvUp( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	int i;
	u32 exp,level;
	POKEMON_PARAM* poke;

	OS_Printf( "�C�x���g�F�k�u�A�b�v\n" );

	for( i=0; i < num ;i++ ){
		poke =  PokeParty_GetMemberPointer( party, i );
		level = PokeParaGet( poke, ID_PARA_level, NULL );
		level+=3;

		if( level > 100 ){
			OS_Printf( "���x����100�𒴂��Ă���I\n" );
			GF_ASSERT(0);
			level = 100;
		}

		exp = PokeLevelExpGet( PokeParaGet(poke,ID_PARA_monsno,NULL), level );
		PokeParaPut( poke, ID_PARA_exp, &exp );
		PokeParaCalc( poke );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F��Ɍ��ʁF�͂�
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvPlaceHare( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "�͂�@��ԂŐ퓬�ցI\n" );
	wk->weather = WEATHER_SYS_HIGHSUNNY;			//���{����
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F��Ɍ��ʁF����
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvPlaceAme( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "���߁@��ԂŐ퓬�ցI\n" );
	wk->weather = WEATHER_SYS_RAIN;					//�J
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F��Ɍ��ʁF���Ȃ��炵
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvPlaceSunaarasi( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "���Ȃ��炵�@��ԂŐ퓬�ցI\n" );
	wk->weather = WEATHER_SYS_SANDSTORM;			//����
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F��Ɍ��ʁF�����
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvPlaceArare( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "�����@��ԂŐ퓬�ցI\n" );
	wk->weather = WEATHER_SYS_SNOW;					//��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F��Ɍ��ʁF����
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvPlaceKiri( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "����@��ԂŐ퓬�ցI\n" );
	//wk->weather = WEATHER_SYS_FOGS;					//��
	wk->weather = WEATHER_SYS_MIST1;					//��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F��Ɍ��ʁF�g���b�N���[��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvPlaceTrick( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "�g���b�N���[���@��ԂŐ퓬�ցI\n" );
	wk->weather = WEATHER_SYS_TRICKROOM;			//�g���b�N���[��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F��Ɍ��ʁF�X�s�[�h�A�b�v
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvPlaceSpeedUp( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "�X�s�[�h�A�b�v\n" );

	if( wk->csr_speed_level < (CSR_WAIT_TBL_MAX - 1) ){
		wk->csr_speed_level++;						//�J�[�\���������X�s�[�h�̃��x��
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F��Ɍ��ʁF�X�s�[�h�_�E��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvPlaceSpeedDown( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "�X�s�[�h�_�E��\n" );

	if( wk->csr_speed_level > 0 ){
		wk->csr_speed_level--;			//�J�[�\���������X�s�[�h�̃��x��
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F��Ɍ��ʁF�����_��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvPlaceRandom( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "�����_��\n" );
	//wk->csr_speed_level = CSR_RANDOM_SPEED;		//�J�[�\���������X�s�[�h�̃��x��
	wk->random_flag = 1;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F����F�|�P�����`�F���W
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvExPokeChange( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "�|�P�����`�F���W�@��ԂŐ퓬�ցI\n" );
	//wk->place_poke_change_flag = 1;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F����FBP�Q�b�g
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvExBPGet( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	u16 add_bp;
	POKEMON_PARAM* poke;
	OS_Printf( "BP�Q�b�g\n" );

	//��p��ʂ𔲂��Ă���BP�𑫂�
	//TowerScoreData_SetBattlePoint( SaveData_GetTowerScoreData(wk->sv), add_bp, BTWR_DATA_add );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F����F�퓬�X���[
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvExBtlWin( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "�퓬�X���[\n" );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�F����F�X�J
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_EvExSuka( ROULETTE_SCRWORK* wk, POKEPARTY* party, u8 num )
{
	POKEMON_PARAM* poke;
	OS_Printf( "�X�J\n" );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ���[���b�g�N���A���ɖႦ��o�g���|�C���g���擾
 *
 * @param	wk
 *
 * @return	"�Ⴆ��o�g���|�C���g"
 */
//--------------------------------------------------------------------------------------------
u16	RouletteScr_GetAddBtlPoint( ROULETTE_SCRWORK* wk );
u16	RouletteScr_GetAddBtlPoint( ROULETTE_SCRWORK* wk )
{
	u8 add_bp;
	u16 lap;
	static const u8 bppoint_normal[ROULETTE_LAP_MAX+1] = {0, 
													2, 2, 2, 2, 4, 4, 5, 6 };//0,1���`8���ȍ~

	static const u8 bppoint_multi[ROULETTE_LAP_MAX+1] = {0, 
													7, 8, 9, 10, 13, 14, 16, 17 };//0,1���`8���ȍ~

	//lap = RouletteScr_CommGetLap( wk );
	lap = wk->lap;

	OS_Printf( "���� = %d\n", lap );
	OS_Printf( "�A���� = %d\n", wk->rensyou );

	//�V���O���A�_�u��
	if( (wk->type == ROULETTE_TYPE_SINGLE) || (wk->type == ROULETTE_TYPE_DOUBLE) ){

		//�ő���񐔃`�F�b�N
		if( lap >= ROULETTE_LAP_MAX ){
			add_bp = bppoint_normal[ ROULETTE_LAP_MAX ];		//�e�[�u����+1���Ă���̂�
		}else{
			add_bp = bppoint_normal[ lap ];
		}

	//�}���`�AWIFI
	}else{

		//�ő���񐔃`�F�b�N
		if( lap >= ROULETTE_LAP_MAX ){
			add_bp = bppoint_multi[ ROULETTE_LAP_MAX ];		//�e�[�u����+1���Ă���̂�
		}else{
			add_bp = bppoint_multi[ lap ];
		}

	}

	//�u���[���`�F�b�N
	if( wk->type == ROULETTE_TYPE_SINGLE ){
		if( (wk->rensyou == ROULETTE_LEADER_SET_1ST) || (wk->rensyou == ROULETTE_LEADER_SET_2ND) ){
			add_bp = 20;
		}
	}

	return add_bp;
}

//--------------------------------------------------------------------------------------------
/**
 * �퓬��̃A�C�e����Ԃ��Z�b�g����
 *
 * @param	btl_party	�퓬�Ŏg�p�����p�[�e�B
 * @param	party		�Z�b�g����p�[�e�B
 * @param	btl_pos		�퓬�Ŏg�p�����p�[�e�B�̈ʒu
 * @param	pos			�Z�b�g����p�[�e�B�̈ʒu
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void RouletteScr_BattleAfterItemSet( POKEPARTY* btl_party, POKEPARTY* party, int btl_pos, int pos );
void RouletteScr_BattleAfterItemSet( POKEPARTY* btl_party, POKEPARTY* party, int btl_pos, int pos )
{
	u16 item;
	POKEMON_PARAM* pp;
	POKEMON_PARAM* btl_pp;

	//�o�g���Ŏg�p�����|�P�����f�[�^����A�C�e�����擾
	btl_pp	= PokeParty_GetMemberPointer( btl_party, btl_pos );
	item	= PokeParaGet( btl_pp, ID_PARA_item, NULL );

	//�Z�b�g����|�P�����f�[�^
	pp =  PokeParty_GetMemberPointer( party, pos );
	PokeParaPut( pp, ID_PARA_item, &item );
	//PokeParaCalc( pp );
	return;
}



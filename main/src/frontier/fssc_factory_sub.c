//============================================================================================
/**
 * @file	fss_factory_sub.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�T�u�F�t�@�N�g���[
 * @author	Satoshi Nohara
 * @date	07.04.10
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

#include "application/factory.h"
#include "frontier_tool.h"
#include "frontier_def.h"
#include "factory_tool.h"
#include "fssc_factory_sub.h"
#include "../field/scr_tool.h"		//FactoryScr_GetWinRecordID
#include "itemtool/itemsym.h"
#include "comm_command_frontier.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_def.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
FACTORY_SCRWORK* FactoryScr_WorkAlloc( SAVEDATA* savedata, u16 init, u8 type, u8 level );
void FactoryScr_WorkInit( FACTORY_SCRWORK* wk, u16 init );
static void FactoryScr_InitDataSet( FACTORY_SCRWORK* wk );
static void FactoryScr_ContinueDataSet( FACTORY_SCRWORK* wk );
void FactoryScr_WorkRelease( FACTORY_SCRWORK* wk );
void FactoryScr_GetResult( FACTORY_SCRWORK* wk, void* factory_call );
u16 FactoryCall_GetRetWork( void* param, u8 pos );
void FactoryScr_RentalPartySet( FACTORY_SCRWORK* wk );
void FactoryScr_BtlAfterPartySet( FACTORY_SCRWORK* wk );
void FactoryScr_TradePokeChange( FACTORY_SCRWORK* wk );
void FactoryScr_TradeAfterPartySet( FACTORY_SCRWORK* wk );
void FactoryScr_TradeCountInc( FACTORY_SCRWORK* wk );
static void FactoryScr_RentalPokePosChg( FACTORY_SCRWORK* wk, u8 pos, u8 flag );

//�ʐM
BOOL FactoryScr_CommSetSendBuf( FACTORY_SCRWORK* wk, u16 type, u16 param );


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
 * �����Ȃ炸 FactoryScr_WorkRelease()�ŗ̈���J�����邱��
 */
//--------------------------------------------------------------
FACTORY_SCRWORK* FactoryScr_WorkAlloc( SAVEDATA* savedata, u16 init, u8 type, u8 level )
{
	FACTORYDATA* fa_sv;
	FACTORYSCORE* score_sv;
	const FACTORY_POKE_RANGE* poke_range;
	u8 clear_flag;
	u16	i,chg_flg;
	u32 clear_id;
	static FACTORY_SCRWORK* wk;					//���ƂŊm�F]]]]]]]]]]]]]]]]]]]]]]]]]

	wk = sys_AllocMemory( HEAPID_WORLD, sizeof(FACTORY_SCRWORK) );
	MI_CpuClear8( wk, sizeof(FACTORY_SCRWORK) );

	//�Z�[�u�f�[�^�擾
	wk->factory_savedata= SaveData_GetFactoryData( savedata );
	wk->sv				= savedata;
	wk->heapID			= HEAPID_WORLD;
	wk->p_m_party		= PokeParty_AllocPartyWork( HEAPID_WORLD );
	wk->p_e_party		= PokeParty_AllocPartyWork( HEAPID_WORLD );
	
	fa_sv = wk->factory_savedata;
	score_sv= SaveData_GetFactoryScore( savedata );

	//�V�K���A�p����
	if( init == 0 ){
		wk->type	= type;
		wk->level	= level;
		wk->round	= 0;

		FACTORYDATA_Init( fa_sv );	//���f�f�[�^������

		//WIFI�̂ݓ���
		if( wk->type == FACTORY_TYPE_WIFI_MULTI ){

			if( wk->level == FACTORY_LEVEL_50 ){
				clear_id = FRID_FACTORY_MULTI_WIFI_CLEAR_BIT;
			}else{
				clear_id = FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT;
			}

#if 0
			clear_flag = FrontierRecord_Get(SaveData_GetFrontier(wk->sv), 
											clear_id,
											Frontier_GetFriendIndex(clear_id) );
#else
			clear_flag = SysWork_WifiFrClearFlagGet( SaveData_GetEventWork(wk->sv) );
#endif

		}else{
			//�N���A�������t���O���擾
			clear_flag = (u8)FACTORYSCORE_GetScoreData(	score_sv, FACTORYSCORE_ID_CLEAR_FLAG, 
													(wk->level*FACTORY_TYPE_MAX)+wk->type, NULL );
		}

		if( clear_flag == 1 ){
			wk->rensyou = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
						FactoryScr_GetWinRecordID(wk->level,wk->type),
						Frontier_GetFriendIndex(FactoryScr_GetWinRecordID(wk->level,wk->type)) );
			wk->trade_count = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
						FactoryScr_GetTradeRecordID(wk->level,wk->type),
						Frontier_GetFriendIndex(FactoryScr_GetTradeRecordID(wk->level,wk->type)) );
		}else{
			wk->rensyou = 0;
			wk->trade_count = 0;
		}

		//�����^�����邾���ł������񐔂�1�񑝂���
		//FactoryScr_TradeCountInc( wk );				//������+1

		wk->win_cnt	= 0;
	}else{
		//���݂̃v���C�i�s�f�[�^�擾
		wk->type = (u8)FACTORYDATA_GetPlayData( fa_sv, FACTORYDATA_ID_TYPE, 0, NULL);
		wk->level = (u8)FACTORYDATA_GetPlayData( fa_sv, FACTORYDATA_ID_LEVEL,0,NULL);
		wk->round = (u8)FACTORYDATA_GetPlayData( fa_sv, FACTORYDATA_ID_ROUND,0,NULL);
		wk->rensyou = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
						FactoryScr_GetWinRecordID(wk->level,wk->type),
						Frontier_GetFriendIndex(FactoryScr_GetWinRecordID(wk->level,wk->type)) );
		wk->trade_count = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
						FactoryScr_GetTradeRecordID(wk->level,wk->type),
						Frontier_GetFriendIndex(FactoryScr_GetTradeRecordID(wk->level,wk->type)) );
	}

	wk->lap		= (u16)(wk->rensyou / FACTORY_LAP_ENEMY_MAX);

	OS_Printf( "____alloc trade_count = %d\n", wk->trade_count );
	OS_Printf( "____alloc ensyou = %d\n", wk->rensyou );
	OS_Printf( "____alloc lap = %d\n", wk->lap );

	//�n���V�o�^
	if( Factory_CommCheck(wk->type) == TRUE ){
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
 * �����Ȃ炸 FactoryScr_WorkRelease()�ŗ̈���J�����邱��
 */
//--------------------------------------------------------------
void FactoryScr_WorkInit( FACTORY_SCRWORK* wk, u16 init )
{
	//�V�K���A�p����
	if( init == 0 ){
		//OS_Printf( "�O �V�K ����wk = %d\n", wk );
		FactoryScr_InitDataSet( wk );
		//OS_Printf( "�� �V�K ����wk = %d\n", wk );
	}else{
		//OS_Printf( "�O �p�� ����wk = %d\n", wk );
		FactoryScr_ContinueDataSet( wk );
		//OS_Printf( "�� �p�� ����wk = %d\n", wk );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�K�F�g���[�i�[�A�|�P�����f�[�^�𐶐�
 *
 * @param	wk		FACTORY_SCRWORK�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void FactoryScr_InitDataSet( FACTORY_SCRWORK* wk )
{
	int i,check_count;
	B_TOWER_POKEMON check_poke[FACTORY_RENTAL_POKE_MAX*2];	//�ʐM�p��2�{�p��
	B_TOWER_POKEMON poke;
	POKEMON_PARAM* temp_poke;
	u16 poke_check_tbl[FACTORY_RENTAL_POKE_MAX];
	u16 item_check_tbl[FACTORY_RENTAL_POKE_MAX];

	OS_Printf( "�V�K�F�f�[�^�𐶐�\n" );

	//07.09.25 ��Ɏ���*2�̃g���[�i�[���擾
	//�V���O��(0-6)�A�}���`�̃p�[�g�i�[(7-13)�Ŏ擾
	//���̎��ɓo�ꂷ��g���[�i�[��index��S�Ď擾
	Factory_EnemyLapAllTrainerIndexGet( wk->type, FactoryScr_CommGetLap(wk), wk->tr_index,
										FACTORY_LAP_MULTI_ENEMY_MAX );
	OS_Printf( "lap = %d\n", FactoryScr_CommGetLap(wk) );

	//�f�o�b�N���
	for( i=0; i < FACTORY_LAP_MULTI_ENEMY_MAX ;i++ ){
		//OS_Printf( "tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}

	//�����^���|�P��������
	Factory_RentalPokeMake( FactoryScr_CommGetLap(wk), wk->level, 
							wk->rental_poke_index, 
							wk->rental_poke,					//B_TOWER_POKEMON
							wk->rental_pow_rnd,
							wk->rental_personal_rnd,
							wk->trade_count,
							NULL, NULL );

#if 1	//rental_poke_pos_chg
	//6�C�̂����A��납��2�C�̈ʒu�������_���ړ������
	FactoryScr_RentalPokePosChg( wk, 4, 0 );
	FactoryScr_RentalPokePosChg( wk, 5, 0 );
#endif

	//�R�s�[
	check_count = FACTORY_RENTAL_POKE_MAX;
	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
		check_poke[i] = wk->rental_poke[i];
	}

	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
		OS_Printf( "rental_poke[%d] = %d\n", i, wk->rental_poke[i].mons_no );
		OS_Printf( "rental_index[%d] = %d\n", i, wk->rental_poke_index[i] );
		OS_Printf( "rental_pow[%d] = %d\n", i, wk->rental_pow_rnd[i] );
		OS_Printf( "rental_personal[%d] = %d\n", i, wk->rental_personal_rnd[i] );
	}

	//�ʐM�̎�
	if( Factory_CommCheck(wk->type) == TRUE ){

		//�`�F�b�N�e�[�u���쐬
		for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
#if 0
			poke = *(B_TOWER_POKEMON*)(&wk->rental_poke[i]);
			poke_check_tbl[i] = poke.mons_no;
			item_check_tbl[i] = poke.item_no;
#else
			poke_check_tbl[i] = wk->rental_poke[i].mons_no;
			item_check_tbl[i] = wk->rental_poke[i].item_no;
#endif
		}

		Factory_RentalPokeMake( FactoryScr_CommGetLap(wk), wk->level, 
								wk->pair_rental_poke_index, 
								wk->pair_rental_poke,			//B_TOWER_POKEMON
								wk->pair_rental_pow_rnd,
								wk->pair_rental_personal_rnd,
								wk->pair_trade_count,
								poke_check_tbl, item_check_tbl );

#if 1	//rental_poke_pos_chg
		//6�C�̂����A��납��2�C�̈ʒu�������_���ړ������
		FactoryScr_RentalPokePosChg( wk, 4, 1 );
		FactoryScr_RentalPokePosChg( wk, 5, 1 );
#endif

		//�R�s�[
		check_count = ( FACTORY_RENTAL_POKE_MAX * 2 );
		for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
			check_poke[i+FACTORY_RENTAL_POKE_MAX] = wk->pair_rental_poke[i];
		}

		for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
			OS_Printf( "pair_rental_poke[%d] = %d\n", i, wk->pair_rental_poke[i].mons_no );
			OS_Printf( "pair_rental_index[%d] = %d\n", i, wk->pair_rental_poke_index[i] );
			OS_Printf( "pair_rental_pow[%d] = %d\n", i, wk->pair_rental_pow_rnd[i] );
			OS_Printf( "pair_rental_personal[%d] = %d\n", i, wk->pair_rental_personal_rnd[i] );
		}
	}

	//OS_Printf( "222wk = %d\n", wk );
	//OS_Printf( "222wk->p_party = %d\n", wk->p_party );

	//�G�g���[�i�[���o���|�P��������(�ʐM�Ńy�A�̃����^���|�P�����̔����l��)
#if 0	
	Factory_EnemyPokeMake(	Factory_GetEnemyPokeNum(wk->type,FACTORY_FLAG_TOTAL),
							wk->tr_index[wk->round], wk->level, 
							wk->rental_poke,					//B_TOWER_POKEMON
							wk->enemy_poke_index, 
							wk->enemy_poke,						//B_TOWER_POKEMON
							wk->enemy_pow_rnd,
							wk->enemy_personal_rnd );
#else
	Factory_EnemyPokeMake(	Factory_GetEnemyPokeNum(wk->type,FACTORY_FLAG_TOTAL),
							wk->tr_index[wk->round], wk->level, 
							check_poke,							//B_TOWER_POKEMON
							wk->enemy_poke_index, 
							wk->enemy_poke,						//B_TOWER_POKEMON
							wk->enemy_pow_rnd,
							wk->enemy_personal_rnd,
							check_count );
#endif

	for( i=0; i < 4 ;i++ ){
		OS_Printf( "enemy_poke[%d] = %d\n", i, wk->enemy_poke[i].mons_no );
		OS_Printf( "enemy_index[%d] = %d\n", i, wk->enemy_poke_index[i] );
		OS_Printf( "enemy_pow[%d] = %d\n", i, wk->enemy_pow_rnd[i] );
		OS_Printf( "enemy_personal[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
	}

	//OS_Printf( "333wk = %d\n", wk );
	//OS_Printf( "333wk->p_party = %d\n", wk->p_party );

	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){			//�����^��6�C
		temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
		Frontier_PokeParaMake( &wk->rental_poke[i], temp_poke, Factory_GetLevel(wk) );
		Frontier_PokePartyAdd( wk->sv, wk->p_m_party, temp_poke );
		sys_FreeMemoryEz( temp_poke );
	}

	//���\��
	OS_Printf( "�|�P�����p�[�e�B�� = %d\n",	PokeParty_GetPokeCount(wk->p_m_party) );
	for( i=0; i < FACTORY_PARTY_POKE_MAX; i++ ){
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, i );
		OS_Printf( "�V�Kparty[%d] monsno = %d\n", i, PokeParaGet(temp_poke,ID_PARA_monsno,NULL) );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����^���|�P�����̈ʒu�������_���œ���ւ�
 *
 * @param	wk		FACTORY_SCRWORK�ւ̃|�C���^
 * @param	pos		����ւ���ʒu
 * @param	flag	0=�����A1=�y�A
 */
//--------------------------------------------------------------
static void FactoryScr_RentalPokePosChg( FACTORY_SCRWORK* wk, u8 pos, u8 flag )
{
	u16 rand;
	u16 temp_rental_poke_index;											//�|�P����index�e�[�u��
	u8	temp_rental_pow_rnd;											//�|�P�����̃p���[����
	u32 temp_rental_personal_rnd;										//�|�P�����̌�����
	B_TOWER_POKEMON temp_rental_poke;

	rand = ( gf_rand() % FACTORY_RENTAL_POKE_MAX );
	OS_Printf( "����ւ���ꏊ = %d �� %d\n", pos, rand );

	//�����p
	if( flag == 0 ){

		//[pos]��temp�֑ޔ�
		temp_rental_poke_index			= wk->rental_poke_index[pos];		//�|�P����index�e�[�u��
		temp_rental_pow_rnd				= wk->rental_pow_rnd[pos];			//�|�P�����̃p���[����
		temp_rental_personal_rnd		= wk->rental_personal_rnd[pos];		//�|�P�����̌�����
		temp_rental_poke				= wk->rental_poke[pos];

		//[�����_��]��[pos]��
		wk->rental_poke_index[pos]		= wk->rental_poke_index[rand];		//�|�P����index�e�[�u��
		wk->rental_pow_rnd[pos]			= wk->rental_pow_rnd[rand];			//�|�P�����̃p���[����
		wk->rental_personal_rnd[pos]	= wk->rental_personal_rnd[rand];	//�|�P�����̌�����
		wk->rental_poke[pos]			= wk->rental_poke[rand];

		//�ޔ�����temp�������_���Ȉʒu��
		wk->rental_poke_index[rand]		= temp_rental_poke_index;			//�|�P����index�e�[�u��
		wk->rental_pow_rnd[rand]		= temp_rental_pow_rnd;				//�|�P�����̃p���[����
		wk->rental_personal_rnd[rand]	= temp_rental_personal_rnd;			//�|�P�����̌�����
		wk->rental_poke[rand]			= temp_rental_poke;

	//////////////////////////////////////////////////////////////////////////////////////////////
	//�y�A�p
	}else{

		//[pos]��temp�֑ޔ�
		temp_rental_poke_index			= wk->pair_rental_poke_index[pos];	//�|�P����index�e�[�u��
		temp_rental_pow_rnd				= wk->pair_rental_pow_rnd[pos];		//�|�P�����̃p���[����
		temp_rental_personal_rnd		= wk->pair_rental_personal_rnd[pos];//�|�P�����̌�����
		temp_rental_poke				= wk->pair_rental_poke[pos];

		//[�����_��]��[pos]��
		wk->pair_rental_poke_index[pos]	= wk->pair_rental_poke_index[rand];	//�|�P����index�e�[�u��
		wk->pair_rental_pow_rnd[pos]	= wk->pair_rental_pow_rnd[rand];	//�|�P�����̃p���[����
		wk->pair_rental_personal_rnd[pos]= wk->pair_rental_personal_rnd[rand];//�|�P�����̌�����
		wk->pair_rental_poke[pos]		= wk->pair_rental_poke[rand];

		//�ޔ�����temp�������_���Ȉʒu��
		wk->pair_rental_poke_index[rand]= temp_rental_poke_index;			//�|�P����index�e�[�u��
		wk->pair_rental_pow_rnd[rand]	= temp_rental_pow_rnd;				//�|�P�����̃p���[����
		wk->pair_rental_personal_rnd[rand]= temp_rental_personal_rnd;		//�|�P�����̌�����
		wk->pair_rental_poke[rand]		= temp_rental_poke;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���F�ۑ����Ă������f�[�^���g����B_TOWER_POKEMON�Ȃǂ𐶐�
 *
 * @param	wk		FACTORY_SCRWORK�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void FactoryScr_ContinueDataSet( FACTORY_SCRWORK* wk )
{
	int i;
	POKEMON_PARAM* temp_poke;
	u8 count,m_max;
	B_TOWER_POKEMON bt_poke[FACTORY_PARTY_POKE_MAX];
	u8 pow_rnd[FACTORY_PARTY_POKE_MAX];
	u16 poke_index[FACTORY_PARTY_POKE_MAX];
	u32 personal_rnd[FACTORY_PARTY_POKE_MAX];
	OS_Printf( "�p���F�ۑ����Ă������f�[�^�����[�h\n" );

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Factory_GetMinePokeNum( wk->type );							//����

	//-----------------------------------------------------------------------------------
	
	//�ۑ����Ă������g���[�i�[��index�擾
	for( i=0; i < FACTORY_LAP_MULTI_ENEMY_MAX ;i++ ){
		wk->tr_index[i] = (u16)FACTORYDATA_GetPlayData(	wk->factory_savedata, 
														FACTORYDATA_ID_TR_INDEX, i, NULL );
		//OS_Printf( "tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}

	//---------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------
	//�ۑ����Ă������莝���|�P�����擾
	for( i=0; i < FACTORY_MINE_POKE_MAX ;i++ ){

		poke_index[i] = (u16)FACTORYDATA_GetPlayData(wk->factory_savedata, 
												FACTORYDATA_ID_TEMOTI_POKE_INDEX, i, NULL );
		//OS_Printf( "poke_index[%d] = %d\n", i, poke_index[i] );
		
		personal_rnd[i] = (u32)FACTORYDATA_GetPlayData(wk->factory_savedata, 
												FACTORYDATA_ID_TEMOTI_PERSONAL_RND, i, NULL );
		OS_Printf( "personal_rnd[%d] = %d\n", i, personal_rnd[i] );
		
		pow_rnd[i] = (u8)FACTORYDATA_GetPlayData(wk->factory_savedata, 
												FACTORYDATA_ID_TEMOTI_POW_RND, i, NULL );
		//OS_Printf( "pow_rnd[%d] = %d\n", i, pow_rnd[i] );
		
		//�莝���Ƀ|�P�����̃C���f�b�N�X�i�[
		wk->temoti_poke_index[i] = poke_index[i];
	}

	//�莝���|�P��������x�ɐ���(personal����)
	Frontier_PokemonParamCreateAll(	bt_poke, poke_index, pow_rnd,
									//NULL, personal_rnd, FACTORY_MINE_POKE_MAX, HEAPID_WORLD );
									personal_rnd, NULL, FACTORY_MINE_POKE_MAX, HEAPID_WORLD,
									ARC_PL_BTD_PM );

	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < FACTORY_MINE_POKE_MAX ;i++ ){
		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake( &bt_poke[i], temp_poke, Factory_GetLevel(wk) );
		Frontier_PokePartyAdd( wk->sv, wk->p_m_party, temp_poke );
		OS_Printf( "�p��m_party[%d] monsno = %d\n", i, PokeParaGet(temp_poke,ID_PARA_monsno,NULL) );
	}
	sys_FreeMemoryEz( temp_poke );

	//---------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------
	//�ۑ����Ă������G�|�P�����擾
	for( i=0; i < FACTORY_ENEMY_POKE_MAX ;i++ ){

		poke_index[i] = (u16)FACTORYDATA_GetPlayData(wk->factory_savedata, 
												FACTORYDATA_ID_ENEMY_POKE_INDEX, i, NULL );
		//OS_Printf( "poke_index[%d] = %d\n", i, poke_index[i] );
		
		personal_rnd[i] = (u32)FACTORYDATA_GetPlayData(wk->factory_savedata, 
												FACTORYDATA_ID_ENEMY_PERSONAL_RND, i, NULL );
		//OS_Printf( "personal_rnd[%d] = %d\n", i, personal_rnd[i] );
		
		pow_rnd[i] = (u8)FACTORYDATA_GetPlayData(wk->factory_savedata, 
												FACTORYDATA_ID_ENEMY_POW_RND, i, NULL );
		//OS_Printf( "pow_rnd[%d] = %d\n", i, pow_rnd[i] );
		
		//�G�|�P�����̃C���f�b�N�X�i�[
		wk->enemy_poke_index[i] = poke_index[i];
	}

	//�莝���|�P��������x�ɐ���(personal����)
	Frontier_PokemonParamCreateAll(	bt_poke, poke_index, pow_rnd,
									//NULL, personal_rnd, FACTORY_ENEMY_POKE_MAX, HEAPID_WORLD );
									personal_rnd, NULL, FACTORY_ENEMY_POKE_MAX, HEAPID_WORLD,
									ARC_PL_BTD_PM );

	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < FACTORY_ENEMY_POKE_MAX ;i++ ){
		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake( &bt_poke[i], temp_poke, Factory_GetLevel(wk) );
		Frontier_PokePartyAdd( wk->sv, wk->p_e_party, temp_poke );
		OS_Printf( "�p��e_party[%d] monsno = %d\n", i, PokeParaGet(temp_poke,ID_PARA_monsno,NULL) );

	}
	sys_FreeMemoryEz( temp_poke );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���[�N�G���A���J������
 */
//--------------------------------------------------------------
void FactoryScr_WorkRelease( FACTORY_SCRWORK* wk )
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

	MI_CpuClear8( wk, sizeof(FACTORY_SCRWORK) );
	sys_FreeMemoryEz( wk );
	wk = NULL;

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�t�@�N�g���[��ʌĂяo����̌��ʎ擾
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void FactoryScr_GetResult( FACTORY_SCRWORK* wk, void* factory_call )
{
	int i;

	//���ʂ��擾����
	for( i=0; i < FACTORY_RET_WORK_MAX ;i++ ){
		wk->ret_work[i] = FactoryCall_GetRetWork( factory_call, i );
		OS_Printf( "wk->ret_work[%d] = %d\n", i, wk->ret_work[i] );
	}

	return;
}

//----------------------------------------------------------------------------
/**
 * @brief	�o�g���t�@�N�g���[�̃|�P�����I����ʂł̌��ʂ��擾
 *
 * @param	param	FACTORY_CALL_WORK�̃|�C���^
 * @param	pos		ret_work[pos](0-5)
 *
 * @return	"����"
 */
//----------------------------------------------------------------------------
u16 FactoryCall_GetRetWork( void* param, u8 pos )
{
	FACTORY_CALL_WORK* factory_call = param;

	if( pos >= FACTORY_RET_WORK_MAX ){
		GF_ASSERT(0);
		return 0;
	}

	return factory_call->ret_work[pos];
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
void FactoryScr_SaveRestPlayData( FACTORY_SCRWORK* wk, u8 mode );
u16	FactoryScr_IncRound( FACTORY_SCRWORK* wk );
u16	FactoryScr_GetRound( FACTORY_SCRWORK* wk );
u16 FactoryScr_GetEnemyObjCode( FACTORY_SCRWORK* wk, u8 param );
void FactoryScr_SetLose( FACTORY_SCRWORK* wk );
void FactoryScr_SetClear( FACTORY_SCRWORK* wk );

//--------------------------------------------------------------
/**
 * @brief	�x�ނƂ��Ɍ��݂̃v���C�󋵂��Z�[�u�ɏ����o��
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 * @param	mode	FR_MODE_CLEAR="�N���A",FR_MODE_LOSE="����",FR_MODE_REST="�x��"
 */
//--------------------------------------------------------------
void FactoryScr_SaveRestPlayData( FACTORY_SCRWORK* wk, u8 mode )
{
	u16	i,before_max_rensyou,after_max_rensyou;
	u8 m_max,e_max;
	u8	buf8[4];
	u16	buf16[4];
	u32	buf32[4];
	u32 after,count,clear_id;
	FRONTIER_SAVEWORK* fr_sv;
	POKEMON_PARAM* pp;
	FACTORYDATA* f_sv = wk->factory_savedata;
	FACTORYSCORE* score_sv= SaveData_GetFactoryScore( wk->sv );

	fr_sv = SaveData_GetFrontier( wk->sv );

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Factory_GetMinePokeNum( wk->type );							//����
	e_max = Factory_GetEnemyPokeNum( wk->type, FACTORY_FLAG_TOTAL );	//�G

	//"���x��50�A�I�[�v��"�����o��
	buf8[0] = wk->level;
	FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_LEVEL, 0, buf8 );

	//"�V���O���A�_�u���A�}���`�Awifi�}���`"�����o��
	buf8[0] = wk->type;
	FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_TYPE, 0, buf8 );
	
	//�Z�[�u�t���O��L����ԂɃ��Z�b�g
	FACTORYDATA_SetSaveFlag( wk->factory_savedata, TRUE );

	//"���E���h��"�����o��(0-6�l�ڂ̉��l�ڂ�������炷)
	buf8[0] = wk->round;
	//OS_Printf( "wk->round = %d\n", wk->round );
	//OS_Printf( "buf8[0] = %d\n", buf8[0] );
	FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_ROUND, 0, buf8 );

	//"������"�����o��
	FrontierRecord_Set(	fr_sv, 
						FactoryScr_GetTradeRecordID(wk->level,wk->type), 
						Frontier_GetFriendIndex(FactoryScr_GetTradeRecordID(wk->level,wk->type)), 
						wk->trade_count );

#if 0
	//�A�� / 7 = ����
	//�A�� % 7 = ���l�ڂ�
#endif
	//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
	//OS_Printf( "rensyou = %d\n", wk->rensyou );
	FrontierRecord_Set(	fr_sv, 
						FactoryScr_GetWinRecordID(wk->level,wk->type), 
						Frontier_GetFriendIndex(FactoryScr_GetWinRecordID(wk->level,wk->type)), 
						wk->rensyou );
	
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//"�x��"�ȊO
	if( mode != FR_MODE_REST ){

		//�ύX�O"�ő�A����"���擾���Ă���
		before_max_rensyou = FrontierRecord_Get(	fr_sv, 
						FactoryScr_GetMaxWinRecordID(wk->level,wk->type),
						Frontier_GetFriendIndex(FactoryScr_GetMaxWinRecordID(wk->level,wk->type)) );

		//"�ő�A����"�����o��
		after = FrontierRecord_SetIfLarge(	fr_sv,
						FactoryScr_GetMaxWinRecordID(wk->level,wk->type),
						Frontier_GetFriendIndex(FactoryScr_GetMaxWinRecordID(wk->level,wk->type)),
						wk->rensyou );

		//�ύX��"�ő�A����"���擾���Ă���
		after_max_rensyou = FrontierRecord_Get(	fr_sv, 
						FactoryScr_GetMaxWinRecordID(wk->level,wk->type),
						Frontier_GetFriendIndex(FactoryScr_GetMaxWinRecordID(wk->level,wk->type)) );

		//���݂̘A�������A�O�̍ő�A�����Ɠ�����
		if( wk->rensyou == before_max_rensyou ){

			//��r����"�ō��A���L�^��"�̌����񐔂��X�V����
			FrontierRecord_SetIfLarge(	fr_sv,
						FactoryScr_GetMaxTradeRecordID(wk->level,wk->type),
						Frontier_GetFriendIndex(FactoryScr_GetMaxTradeRecordID(wk->level,wk->type)),
						wk->trade_count );

		//�ő�A�������X�V������
		}else if( before_max_rensyou < after_max_rensyou ){

			//"�ō��A���L�^��"�̌����񐔂��X�V����
			FrontierRecord_Set(	fr_sv,
						FactoryScr_GetMaxTradeRecordID(wk->level,wk->type),
						Frontier_GetFriendIndex(FactoryScr_GetMaxTradeRecordID(wk->level,wk->type)),
						wk->trade_count );
		}

		//"7�A��(�N���A)�������t���O"�����o��
		buf8[0] = wk->clear_flag;
		//FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_CLEAR_FLAG, 0, buf8 );
		FACTORYSCORE_PutScoreData( score_sv, FACTORYSCORE_ID_CLEAR_FLAG, 
								(wk->level*FACTORY_TYPE_MAX)+wk->type, buf8 );

		//WIFI�̂ݓ���
		if( wk->type == FACTORY_TYPE_WIFI_MULTI ){

			if( wk->level == FACTORY_LEVEL_50 ){
				clear_id = FRID_FACTORY_MULTI_WIFI_CLEAR_BIT;
			}else{
				clear_id = FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT;
			}

			FrontierRecord_Set(	fr_sv, 
								clear_id,
								Frontier_GetFriendIndex(clear_id), wk->clear_flag );
		}
	}
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	//�g���[�i�[
	for( i=0; i < FACTORY_LAP_MULTI_ENEMY_MAX ;i++ ){
		buf16[0] = wk->tr_index[i];
		FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_TR_INDEX, i, buf16 );
	}

	//�莝��
	count = PokeParty_GetPokeCount( wk->p_m_party );
	for( i=0; i < count ;i++ ){

		pp = PokeParty_GetMemberPointer( wk->p_m_party, i );

		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		//�����������X�^�[�i���o�[�ł͂Ȃ��I
		//�C���f�b�N�X�I
		//buf16[0] = PokeParaGet( pp, ID_PARA_monsno, NULL );
		buf16[0] = wk->temoti_poke_index[i];
		FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_TEMOTI_POKE_INDEX, i, buf16 );
		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

		buf8[0] = PokeParaGet( pp, ID_PARA_pow_rnd, NULL );
		FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_TEMOTI_POW_RND, i, buf8 );

		buf32[0] = PokeParaGet( pp, ID_PARA_personal_rnd, NULL );
		FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_TEMOTI_PERSONAL_RND, i,buf32);
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
		FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_ENEMY_POKE_INDEX, i, buf16 );
		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

		buf8[0] = PokeParaGet( pp, ID_PARA_pow_rnd, NULL );
		FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_ENEMY_POW_RND, i, buf8 );

		buf32[0] = PokeParaGet( pp, ID_PARA_personal_rnd, NULL );
		FACTORYDATA_PutPlayData( wk->factory_savedata, FACTORYDATA_ID_ENEMY_PERSONAL_RND, i,buf32);
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���E���h�����C���N�������g
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
u16	FactoryScr_IncRound( FACTORY_SCRWORK* wk )
{
	wk->round++;
	return wk->round;
}

//--------------------------------------------------------------
/**
 * @brief	���E���h�����擾
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
u16	FactoryScr_GetRound( FACTORY_SCRWORK* wk )
{
	return wk->round;
}

//--------------------------------------------------------------
/**
 * @brief	�ΐ�g���[�i�[OBJ�R�[�h�擾
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 * @param	param	0=1�l�ځA1=2�l��(�G�̃p�[�g�i�|)
 */
//--------------------------------------------------------------
u16 FactoryScr_GetEnemyObjCode( FACTORY_SCRWORK* wk, u8 param )
{
	B_TOWER_TRAINER bt_trd;
	B_TOWER_TRAINER_ROM_DATA* p_rom_tr;
	u8 index;

	//�擾����g���[�i�[�f�[�^�̃C���f�b�N�X���擾
	index = wk->round + (param * FACTORY_LAP_ENEMY_MAX);

	//ROM����g���[�i�[�f�[�^���m��
	p_rom_tr = Frontier_TrainerDataGet( &bt_trd, wk->tr_index[index], HEAPID_WORLD, ARC_PL_BTD_TR );
	sys_FreeMemoryEz( p_rom_tr );

	//�g���[�i�[�^�C�v����OBJ�R�[�h���擾���Ă���
	return Frontier_TrType2ObjCode( bt_trd.tr_type );
}

//--------------------------------------------------------------
/**
 * @brief	�s�킵�����̏���
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void FactoryScr_SetLose( FACTORY_SCRWORK* wk )
{
	OS_Printf( "\n�o�g���t�@�N�g���[�f�[�^�@�s��Z�b�g\n" );

	//���݂�5�A���Ȃǂ�ۑ�����K�v������I
	FactoryScr_SaveRestPlayData( wk, FR_MODE_LOSE );		//�Z�[�u�f�[�^�ɑ��

	//�V�K���A�p�����́AWK_SCENE_FACTORY_LOBBY�Ɋi�[����Ă���̂ŁA
	//������round�Ȃǂ��N���A���Ȃ��Ă��A
	//WK_SCENE_FACTORY_LOBBY���p���ł͂Ȃ���ԂŃZ�[�u�����̂ŁA
	//��t�ɘb�������Ă��A�V�K����ɂȂ胏�[�N�̓N���A�����B
	return;
}

//--------------------------------------------------------------
/**
 * @brief	7�A��(�N���A)�������̏���
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void FactoryScr_SetClear( FACTORY_SCRWORK* wk )
{
	OS_Printf( "\n�o�g���t�@�N�g���[�f�[�^�@7�A��(�N���A)�Z�b�g\n" );
	
	wk->clear_flag = 1;						//7�A��(�N���A)�������t���OON

	//wk->rensyou		= 0;				//���݂̘A����
	if( wk->lap < FACTORY_LAP_MAX ){
		wk->lap++;							//���񐔂̃C���N�������g
	}

#if 0
	//FactoryScr_CommGetLap�ł��ꂪ�o�Ȃ��悤�Ƀy�A���X�V
	if( wk->pair_lap < FACTORY_LAP_MAX ){
		wk->pair_lap++;						//���񐔂̃C���N�������g
	}
#endif

	wk->round			= 0;				//�����l�ځH
	//wk->trade_count	= 0;				//������
	FactoryScr_SaveRestPlayData( wk, FR_MODE_CLEAR );		//�Z�[�u�f�[�^�ɑ��
	return;
}

/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/

//--------------------------------------------------------------
/**
 * @brief	�����^����ʌ��POKEPARTY�̃Z�b�g
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void FactoryScr_RentalPartySet( FACTORY_SCRWORK* wk )
{
	int i;
	u8 m_max,e_max;
	POKEMON_PARAM* temp_poke;
	const FACTORY_POKE_RANGE* poke_range;

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Factory_GetMinePokeNum( wk->type );							//����
	e_max = Factory_GetEnemyPokeNum( wk->type, FACTORY_FLAG_TOTAL );	//�G

	//�����p��party��������
	PokeParty_InitWork( wk->p_m_party );		//POKEPARTY��������
	
	//[0-2]	�莝��(�����^���őI��3��)
	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < m_max ;i++ ){

		//�����^���f�[�^�̕K�v�ȃf�[�^��POKEPARTY�ɃZ�b�g(field����͌ĂׂȂ��I)
		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake(&wk->rental_poke[ wk->ret_work[i] ], temp_poke, Factory_GetLevel(wk));
		Frontier_PokePartyAdd( wk->sv, wk->p_m_party, temp_poke );

		//�莝���Ƀ|�P�����̃C���f�b�N�X�i�[
		wk->temoti_poke_index[i] = wk->rental_poke_index[ wk->ret_work[i] ];

		OS_Printf("�퓬�Om_party[%d] monsno = %d\n", i, PokeParaGet(temp_poke,ID_PARA_monsno,NULL));
	}

	for( i=0; i < e_max ;i++ ){

		//�G�|�P�����f�[�^�̕K�v�ȃf�[�^��POKEPARTY�ɃZ�b�g(field����͌ĂׂȂ��I)
		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake( &wk->enemy_poke[i], temp_poke, Factory_GetLevel(wk) );
		Frontier_PokePartyAdd( wk->sv, wk->p_e_party, temp_poke );

		//�莝���Ƀ|�P�����̃C���f�b�N�X�i�[
		//wk->temoti_poke_index[i+m_max] = wk->enemy_poke_index[i];
		
		OS_Printf("�퓬�Oe_party[%d] monsno = %d\n", i, PokeParaGet(temp_poke,ID_PARA_monsno,NULL));
	}
	sys_FreeMemoryEz( temp_poke );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�o�g�����POKEPARTY�̃Z�b�g
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void FactoryScr_BtlAfterPartySet( FACTORY_SCRWORK* wk )
{
	int i,count,tmp_count;
	u8 e_max;
	u16 check_monsno[ FACTORY_TOTAL_POKE_MAX ];		//mine=4,enemy=4 = 8
	u16 check_itemno[ FACTORY_TOTAL_POKE_MAX ];
	POKEMON_PARAM* temp_poke;
	const FACTORY_POKE_RANGE* poke_range;

	//������	
	for( i=0; i < FACTORY_TOTAL_POKE_MAX ; i++ ){	
		check_monsno[i] = 0;
		check_itemno[i] = 0;
	}

	//�^�C�v�ɂ���ă|�P�����̐����擾
	e_max = Factory_GetEnemyPokeNum( wk->type, FACTORY_FLAG_TOTAL );//�G

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

		//���������������ɁA�O��enemy_poke_index���K�v�Ȃ̂����A
		//�����Ăяo���O�ɁA����enemy_poke_index�ɏ��������Ă��܂��̂ŁA
		//�g�p���Ă��Ȃ�rental_poke_index�ɑޔ������Ă���
		wk->rental_poke_index[i] = wk->enemy_poke_index[i];
	}

	poke_range = Factory_EnemyPokemonRangeGet( wk->tr_index[wk->round], wk->level );

	SDK_ASSERTMSG( (tmp_count + count) <= FACTORY_TOTAL_POKE_MAX, "check buf�̗v�f��������Ȃ��I" );

	//���|�P�A�A�C�e���A�f�[�^���A�擾���鐔�A�����
	Factory_PokemonIndexCreate( check_monsno, check_itemno, 
								(tmp_count + count),				//�莝���A�G�̐�
								e_max, wk->enemy_poke_index, 
								HEAPID_WORLD, poke_range, 0, wk->enemy_pow_rnd );

	//�G�|�P��������x�ɐ���(personal�Ȃ�)
	Frontier_PokemonParamCreateAll(	wk->enemy_poke, wk->enemy_poke_index, 
									wk->enemy_pow_rnd, NULL, wk->enemy_personal_rnd, 
									e_max, HEAPID_WORLD, ARC_PL_BTD_PM );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�g���[�h������̃|�P�����̕ύX
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void FactoryScr_TradePokeChange( FACTORY_SCRWORK* wk )
{
	POKEMON_PARAM* temp_poke;

	//������ʂœ���ւ���i���o�[���擾
	//ret_work[0]�莝���̉��Ԗڂ�ς��邩
	//ret_work[1]�G�̉��Ԗڂƕς��邩
	//�ɂȂ�

	//�����Ȃ����`�F�b�N
	if( wk->ret_work[0] == FACTORY_RET_CANCEL ){
		OS_Printf( "������ʌ�:�|�P��������ւ������I\n" );
	}else{
		OS_Printf( "������ʌ�:�|�P��������ւ��L��I%d��%d\n",wk->ret_work[0],wk->ret_work[1]);
		//PokeParty_ExchangePosition(	wk->p_party, wk->ret_work[0], wk->ret_work[1] );

		//POKEPARTY�փ|�P�����f�[�^�������Z�b�g
		temp_poke = PokeParty_GetMemberPointer( wk->p_e_party, wk->ret_work[1] );
		PokeParty_SetMemberData( wk->p_m_party, wk->ret_work[0], temp_poke );

		//�莝���Ƀ|�P�����̃C���f�b�N�X�i�[
		//wk->temoti_poke_index[ wk->ret_work[0] ] = wk->temoti_poke_index[ wk->ret_work[1] ];
		//wk->temoti_poke_index[ wk->ret_work[0] ] = wk->enemy_poke_index[ wk->ret_work[1] ];

		//�O��enemy_poke_index��rental_poke_index�ɑޔ������Ă���̂Ŏ擾
		wk->temoti_poke_index[ wk->ret_work[0] ] = wk->rental_poke_index[ wk->ret_work[1] ];

		FactoryScr_TradeCountInc( wk );				//������+1

		//���R�[�h�X�V
		RECORD_Inc( SaveData_GetRecord(wk->sv), RECID_FACTORY_POKE_CHANGE );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�g���[�h������̃|�P������POKEPARTY�̃Z�b�g
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 *
 * ���̓G�p�[�e�B���Z�b�g
 */
//--------------------------------------------------------------
void FactoryScr_TradeAfterPartySet( FACTORY_SCRWORK* wk )
{
	int i;
	u8 m_max,e_max;
	POKEMON_PARAM* temp_poke;
	int party_num;

	//�^�C�v�ɂ���ă|�P�����̐����擾
	m_max = Factory_GetMinePokeNum( wk->type );							//����
	e_max = Factory_GetEnemyPokeNum( wk->type, FACTORY_FLAG_TOTAL );	//�G

	PokeParty_InitWork( wk->p_e_party );		//POKEPARTY��������

	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < e_max ;i++ ){

		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake( &wk->enemy_poke[i], temp_poke, Factory_GetLevel(wk) );
		Frontier_PokePartyAdd( wk->sv, wk->p_e_party, temp_poke );

		//�莝���Ƀ|�P�����̃C���f�b�N�X�i�[
		//wk->temoti_poke_index[i+m_max] = wk->enemy_poke_index[i];
	}
	sys_FreeMemoryEz( temp_poke );

	//���\��
	//for( i=0; i < FACTORY_PARTY_POKE_MAX; i++ ){
	for( i=0; i < e_max; i++ ){
		temp_poke = PokeParty_GetMemberPointer( wk->p_e_party, i );
		OS_Printf( "���̓Ge_party[%d] monsno = %d\n", 
									i, PokeParaGet(temp_poke,ID_PARA_monsno,NULL) );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����������Č����񐔂��C���N�������g����
 *
 * @param	wk		FACTORY_SCRWORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryScr_TradeCountInc( FACTORY_SCRWORK* wk )
{
	if( wk->trade_count < FACTORY_TRADE_MAX ){
		wk->trade_count++;
	}
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
 * @param	wk			FACTORY_SCRWORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
BOOL FactoryScr_CommSetSendBuf( FACTORY_SCRWORK* wk, u16 type, u16 param )
{
	int ret;

	switch( type ){

	//
	case FACTORY_COMM_BASIC:
		ret = CommFactorySendBasicData( wk );
		break;

	case FACTORY_COMM_TR:
		ret = CommFactorySendTrData( wk );
		break;

	case FACTORY_COMM_RENTAL:
		ret = CommFactorySendRentalData( wk );
		break;

	case FACTORY_COMM_ENEMY:
		ret = CommFactorySendEnemyPokeData( wk );
		break;

	case FACTORY_COMM_RETIRE:
		ret = CommFactorySendRetireFlag( wk, param );
		break;

	case FACTORY_COMM_TRADE_YESNO:
		ret = CommFactorySendTradeYesNoFlag( wk, param );
		break;

	case FACTORY_COMM_TEMOTI:
		ret = CommFactorySendTemotiPokeData( wk );
		break;
	};

	return ret;
}


//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//
//factory_tool.c	Factory_RentalPokeMake
//
//��ɐe�@�̃����^��������A
//�e�@�̃����^����n���Ĕ��`�F�b�N���āA�q�@�̃����^��������
//
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

//--------------------------------------------------------------------------------------------
/**
 * �t�@�N�g���[�N���A���ɖႦ��o�g���|�C���g���擾
 *
 * @param	wk
 *
 * @return	"�Ⴆ��o�g���|�C���g"
 */
//--------------------------------------------------------------------------------------------
u16	FactoryScr_GetAddBtlPoint( FACTORY_SCRWORK* wk );
u16	FactoryScr_GetAddBtlPoint( FACTORY_SCRWORK* wk )
{
	u8 add_bp;
	u16 lap;
	static const u8 bppoint_normal[FACTORY_LAP_MAX+1] = { 0, 
												5, 5, 5, 5, 7, 7, 8, 9 };//0,1���`8���ȍ~

	static const u8 bppoint_multi[FACTORY_LAP_MAX+1] = { 0, 
												10, 11, 12, 13, 16, 17, 19, 21 };//0,1���`8���ȍ~

	//lap = FactoryScr_CommGetLap( wk );
	lap = wk->lap;

	OS_Printf( "���� = %d\n", lap );
	OS_Printf( "�A���� = %d\n", wk->rensyou );

	//�V���O���A�_�u��
	if( (wk->type == FACTORY_TYPE_SINGLE) || (wk->type == FACTORY_TYPE_DOUBLE) ){

		//�ő���񐔃`�F�b�N
		if( lap >= FACTORY_LAP_MAX ){
			add_bp = bppoint_normal[ FACTORY_LAP_MAX ];		//�e�[�u����+1���Ă���̂�
		}else{
			add_bp = bppoint_normal[ lap ];
		}
	
	//�}���`�AWIFI
	}else{

		//�ő���񐔃`�F�b�N
		if( lap >= FACTORY_LAP_MAX ){
			add_bp = bppoint_multi[ FACTORY_LAP_MAX ];		//�e�[�u����+1���Ă���̂�
		}else{
			add_bp = bppoint_multi[ lap ];
		}
	}

	//�u���[���`�F�b�N
	if( wk->type == FACTORY_TYPE_SINGLE ){
		if( (wk->rensyou == FACTORY_LEADER_SET_1ST) || (wk->rensyou == FACTORY_LEADER_SET_2ND) ){
			add_bp = 20;
		}
	}

	return add_bp;
}



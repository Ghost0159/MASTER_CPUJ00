//============================================================================================
/**
 * @file	fss_castle_sub.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�T�u�F�L���b�X��
 * @author	Satoshi Nohara
 * @date	07.07.04
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

#include "application/castle.h"
#include "frontier_tool.h"
#include "frontier_def.h"
#include "castle_tool.h"
#include "fssc_castle_sub.h"
#include "../field/scr_tool.h"		//CastleScr_GetWinRecordID
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
CASTLE_SCRWORK* CastleScr_WorkAlloc( SAVEDATA* savedata, u16 init, u8 type, u16 pos1, u16 pos2, u16 pos3, u16* work );
void CastleScr_WorkInit( CASTLE_SCRWORK* wk, u16 init );
static void CastleScr_InitDataSet( CASTLE_SCRWORK* wk );
static void CastleScr_ContinueDataSet( CASTLE_SCRWORK* wk );
static void CastleScr_ContinuePokePara( CASTLE_SCRWORK* wk );
void CastleScr_WorkRelease( CASTLE_SCRWORK* wk );
void CastleScr_GetResult( CASTLE_SCRWORK* wk, void* castle_call );
u16 CastleCall_GetRetWork( void* param, u8 pos );
void CastleScr_BtlBeforePartySet( CASTLE_SCRWORK* wk );
void CastleScr_BtlAfterPartySet( CASTLE_SCRWORK* wk );
void CastleScr_BtlWinPokeData( CASTLE_SCRWORK* wk );
int CastleScr_GetBtlWinCpPoint( CASTLE_SCRWORK* wk );

//�ʐM
BOOL CastleScr_CommSetSendBuf( CASTLE_SCRWORK* wk, u16 type, u16 param );


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
 * �����Ȃ炸 CastleScr_WorkRelease()�ŗ̈���J�����邱��
 */
//--------------------------------------------------------------
CASTLE_SCRWORK* CastleScr_WorkAlloc( SAVEDATA* savedata, u16 init, u8 type, u16 pos1, u16 pos2, u16 pos3, u16* work )
{
	u32 buf,exp;
	POKEPARTY* temoti_party;
	POKEMON_PARAM* temp_poke;
	CASTLEDATA* ca_sv;
	u8 clear_flag,m_max;
	u16	i,chg_flg;
	static CASTLE_SCRWORK* wk;					//���ƂŊm�F]]]]]]]]]]]]]]]]]]]]]]]]]
	CASTLESCORE* score_sv;

	wk = sys_AllocMemory( HEAPID_WORLD, sizeof(CASTLE_SCRWORK) );
	MI_CpuClear8( wk, sizeof(CASTLE_SCRWORK) );

	//�Z�[�u�f�[�^�擾
	wk->castle_savedata	= SaveData_GetCastleData( savedata );
	wk->sv				= savedata;
	wk->heapID			= HEAPID_WORLD;
	wk->p_m_party		= PokeParty_AllocPartyWork( HEAPID_WORLD );
	wk->p_e_party		= PokeParty_AllocPartyWork( HEAPID_WORLD );
	wk->list_del_work	= work;

	ca_sv = wk->castle_savedata;
	score_sv = SaveData_GetCastleScore( savedata );

	//�V�K���A�p����
	if( init == 0 ){
		wk->type	= type;
		wk->round	= 0;

		CASTLEDATA_Init( ca_sv );	//���f�f�[�^������

		//WIFI�̂ݓ���
		if( wk->type == CASTLE_TYPE_WIFI_MULTI ){
#if 0
			clear_flag = FrontierRecord_Get(SaveData_GetFrontier(wk->sv), 
										FRID_CASTLE_MULTI_WIFI_CLEAR_BIT,
										Frontier_GetFriendIndex(FRID_CASTLE_MULTI_WIFI_CLEAR_BIT) );
#else
			clear_flag = SysWork_WifiFrClearFlagGet( SaveData_GetEventWork(wk->sv) );
#endif

		}else{
			//�N���A�������t���O���擾
			clear_flag = (u8)CASTLESCORE_GetScoreData(	score_sv, CASTLESCORE_ID_CLEAR_FLAG, 
														wk->type, 0, NULL );
		}

		if( clear_flag == 1 ){
			wk->rensyou = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
									CastleScr_GetWinRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetWinRecordID(wk->type)) );
		}else{
			wk->rensyou = 0;

			//"CP"��0�ɂ���
			FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
								CastleScr_GetCPRecordID(wk->type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)), 0 );

			//"�g�p����CP"��0�ɂ���
			FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
								CastleScr_GetUsedCPRecordID(type),
								Frontier_GetFriendIndex(CastleScr_GetUsedCPRecordID(type)), 0 );

#if 1
			//�V�K�ŏ�����̎��́A1�I���W���Z�b�g
			for( i=0; i < CASTLE_RANK_TYPE_MAX ;i++ ){
				FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
						CastleScr_GetRankRecordID(wk->type,i),
						Frontier_GetFriendIndex(CastleScr_GetRankRecordID(wk->type,i)), 1 );
			}
#endif

		}

		wk->lap		= (u16)(wk->rensyou / CASTLE_LAP_ENEMY_MAX);
		wk->win_cnt	= 0;

		//�I�������莝���̈ʒu
		wk->mine_poke_pos[0] = pos1;
		wk->mine_poke_pos[1] = pos2;
		wk->mine_poke_pos[2] = pos3;

		//"+10����O�ɏ�������"����J�n����CP��ޔ�
		wk->temp_start_cp = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
								CastleScr_GetCPRecordID(wk->type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

		//����J�n�̎���CP+10����	
		FrontierRecord_Add(	SaveData_GetFrontier(wk->sv), 
							CastleScr_GetCPRecordID(wk->type),
							Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)), 10 );

	}else{
		//���݂̃v���C�i�s�f�[�^�擾
		wk->type = (u8)CASTLEDATA_GetPlayData( ca_sv, CASTLEDATA_ID_TYPE, 0, 0, NULL);
		wk->round = (u8)CASTLEDATA_GetPlayData( ca_sv, CASTLEDATA_ID_ROUND, 0, 0, NULL);
		wk->rensyou = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
									CastleScr_GetWinRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetWinRecordID(wk->type)) );
		wk->lap		= (u16)(wk->rensyou / CASTLE_LAP_ENEMY_MAX);

		for( i=0; i < CASTLE_ENTRY_POKE_MAX ;i++ ){
			wk->mine_poke_pos[i] = (u8)CASTLEDATA_GetPlayData(	ca_sv, CASTLEDATA_ID_MINE_POKE_POS, 
																i, 0, NULL);
		}
	}

	//�����A�C�e����ۑ����Ă���
	for( i=0; i < CASTLE_ENTRY_POKE_MAX ;i++ ){
		temp_poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(wk->sv), 
												wk->mine_poke_pos[i] );
		wk->itemno[i] = PokeParaGet( temp_poke, ID_PARA_item, NULL );
	}

	//�o��|�P�������Z�b�g
	temoti_party = SaveData_GetTemotiPokemon( wk->sv );
	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_SOLO );		//����
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

#ifdef PM_DEBUG
	//CP��9999�Z�b�g
	if( sys.cont & PAD_BUTTON_L ){
		FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
							CastleScr_GetCPRecordID(wk->type),
							Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)), 9999 );
	}
#endif

	//�n���V�o�^
	if( Castle_CommCheck(wk->type) == TRUE ){
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
 * �����Ȃ炸 CastleScr_WorkRelease()�ŗ̈���J�����邱��
 */
//--------------------------------------------------------------
void CastleScr_WorkInit( CASTLE_SCRWORK* wk, u16 init )
{
	//�V�K���A�p����
	if( init == 0 ){
		//OS_Printf( "�O �V�K ����wk = %d\n", wk );
		CastleScr_InitDataSet( wk );
		//OS_Printf( "�� �V�K ����wk = %d\n", wk );
	}else{
		//OS_Printf( "�O �p�� ����wk = %d\n", wk );
		CastleScr_ContinueDataSet( wk );
		//OS_Printf( "�� �p�� ����wk = %d\n", wk );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�K�F�g���[�i�[�A�|�P�����f�[�^�𐶐�
 *
 * @param	wk		CASTLE_SCRWORK�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void CastleScr_InitDataSet( CASTLE_SCRWORK* wk )
{
	u8 count;
	int i;
	B_TOWER_POKEMON poke;
	POKEMON_PARAM* temp_poke;
	POKEPARTY* temoti_party;
	u16 poke_check_tbl[6];	//rental
	u16 item_check_tbl[6];	//rental

	OS_Printf( "�V�K�F�f�[�^�𐶐�\n" );

	count = PokeParty_GetPokeCount( wk->p_m_party );
	for( i=0; i < count ;i++ ){
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, i );
		wk->pp[i][0] = PokeParaGet( temp_poke, ID_PARA_pp1, NULL );					//PP�ۑ�
		wk->pp[i][1] = PokeParaGet( temp_poke, ID_PARA_pp2, NULL );
		wk->pp[i][2] = PokeParaGet( temp_poke, ID_PARA_pp3, NULL );
		wk->pp[i][3] = PokeParaGet( temp_poke, ID_PARA_pp4, NULL );
	}

	//07.09.25 ��Ɏ���*2�̃g���[�i�[���擾
	//�V���O��(0-6)�A�}���`�̃p�[�g�i�[(7-13)�Ŏ擾
	//���̎��ɓo�ꂷ��g���[�i�[��index��S�Ď擾
	Castle_EnemyLapAllTrainerIndexGet(	wk->type, CastleScr_CommGetLap(wk), wk->tr_index, 
										CASTLE_LAP_MULTI_ENEMY_MAX );
	OS_Printf( "lap = %d\n", CastleScr_CommGetLap(wk) );

	//�f�o�b�N���
	for( i=0; i < CASTLE_LAP_MULTI_ENEMY_MAX ;i++ ){
		OS_Printf( "tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}

	//�G�g���[�i�[���o���|�P��������
	Frontier_EnemyPokeMake(	Castle_GetEnemyPokeNum(wk->type,CASTLE_FLAG_TOTAL),
							wk->tr_index[wk->round],
							wk->tr_index[wk->round+CASTLE_LAP_ENEMY_MAX], 
							wk->enemy_poke_index, 
							wk->enemy_poke,						//B_TOWER_POKEMON
							wk->enemy_pow_rnd,
							wk->enemy_personal_rnd,
							Castle_CommCheck(wk->type) );

	for( i=0; i < 4 ;i++ ){
		OS_Printf( "enemy_poke[%d] = %d\n", i, wk->enemy_poke[i].mons_no );
		OS_Printf( "enemy_index[%d] = %d\n", i, wk->enemy_poke_index[i] );
		OS_Printf( "enemy_pow[%d] = %d\n", i, wk->enemy_pow_rnd[i] );
		OS_Printf( "enemy_personal[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
	}

	//OS_Printf( "333wk = %d\n", wk );
	//OS_Printf( "333wk->p_party = %d\n", wk->p_party );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���F�ۑ����Ă������f�[�^���g����B_TOWER_POKEMON�Ȃǂ𐶐�
 *
 * @param	wk		CASTLE_SCRWORK�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void CastleScr_ContinueDataSet( CASTLE_SCRWORK* wk )
{
	u16 buf;
	int i;
	POKEMON_PARAM* pp;
	u8 count;
	B_TOWER_POKEMON bt_poke[CASTLE_PARTY_POKE_MAX];
	u8 pow_rnd[CASTLE_PARTY_POKE_MAX];
	u16 poke_index[CASTLE_PARTY_POKE_MAX];
	u32 personal_rnd[CASTLE_PARTY_POKE_MAX];
	OS_Printf( "�p���F�ۑ����Ă������f�[�^�����[�h\n" );

	//"�莝���|�P�����̏��"���[�h
	CastleScr_ContinuePokePara( wk );

	//-----------------------------------------------------------------------------------
	
	//�ۑ����Ă������g���[�i�[��index�擾
	for( i=0; i < CASTLE_LAP_MULTI_ENEMY_MAX ;i++ ){
		wk->tr_index[i] = (u16)CASTLEDATA_GetPlayData(	wk->castle_savedata, 
														CASTLEDATA_ID_TR_INDEX, i, 0, NULL );
		//OS_Printf( "tr_index[%d] = %d\n", i, wk->tr_index[i] );
	}

	//---------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------
	//�ۑ����Ă������G�|�P�����擾
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){

		poke_index[i] = (u16)CASTLEDATA_GetPlayData(wk->castle_savedata, 
												CASTLEDATA_ID_ENEMY_POKE_INDEX, i, 0, NULL );
		//OS_Printf( "poke_index[%d] = %d\n", i, poke_index[i] );
		
		//�G�|�P�����̃C���f�b�N�X�i�[
		wk->enemy_poke_index[i] = poke_index[i];
	}

	//�莝���|�P��������x�ɐ���(personal�Ȃ�)
	Frontier_PokemonParamCreateAll(	bt_poke, poke_index, pow_rnd,
									NULL, personal_rnd, CASTLE_ENEMY_POKE_MAX, HEAPID_WORLD,
									ARC_PL_BTD_PM );

	pp = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake( &bt_poke[i], pp, Castle_GetLevel(wk) );
		Castle_PokePartyAdd( wk, wk->p_e_party, pp );
		OS_Printf( "�p��e_party[%d] monsno = %d\n", i, PokeParaGet(pp,ID_PARA_monsno,NULL) );

	}
	sys_FreeMemoryEz( pp );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���FHP�APP�ACONDITION�AITEM�����[�h
 *
 * @param	wk		CASTLE_SCRWORK�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void CastleScr_ContinuePokePara( CASTLE_SCRWORK* wk )
{
	u8 count;
	u32 buf32;
	u16 buf16;
	u8	buf8;
	int i;
	POKEMON_PARAM* pp;

	count = PokeParty_GetPokeCount( wk->p_m_party );

	//"�莝���|�P�����̏��"���[�h
	for( i=0; i < count ;i++ ){

		pp = PokeParty_GetMemberPointer( wk->p_m_party, i );

		buf16 = (u16)CASTLEDATA_GetPlayData(	wk->castle_savedata, 
											CASTLEDATA_ID_TEMOTI_HP, i, 0, NULL );
		OS_Printf( "���[�h����HP = %d\n", buf16 );
		PokeParaPut( pp, ID_PARA_hp, &buf16 );			//HP

		buf8 = (u8)CASTLEDATA_GetPlayData(	wk->castle_savedata, 
											CASTLEDATA_ID_TEMOTI_PP, i, 0, NULL );
		PokeParaPut( pp, ID_PARA_pp1, &buf8 );			//PP1

		buf8 = (u8)CASTLEDATA_GetPlayData(	wk->castle_savedata, 
											CASTLEDATA_ID_TEMOTI_PP, i, 1, NULL );
		PokeParaPut( pp, ID_PARA_pp2, &buf8 );			//PP2

		buf8 = (u8)CASTLEDATA_GetPlayData(	wk->castle_savedata, 
											CASTLEDATA_ID_TEMOTI_PP, i, 2, NULL );
		PokeParaPut( pp, ID_PARA_pp3, &buf8 );			//PP3

		buf8 = (u8)CASTLEDATA_GetPlayData(	wk->castle_savedata, 
											CASTLEDATA_ID_TEMOTI_PP, i, 3, NULL );
		PokeParaPut( pp, ID_PARA_pp4, &buf8 );			//PP4

		buf32 = (u32)CASTLEDATA_GetPlayData(wk->castle_savedata, 
											CASTLEDATA_ID_TEMOTI_CONDITION, i, 0, NULL );
		PokeParaPut( pp, ID_PARA_condition, &buf32 );	//CONDITION

		buf16 = (u16)CASTLEDATA_GetPlayData(	wk->castle_savedata, 
											CASTLEDATA_ID_TEMOTI_ITEM, i, 0, NULL );
		PokeParaPut( pp, ID_PARA_item, &buf16 );		//ITEM
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���[�N�G���A���J������
 */
//--------------------------------------------------------------
void CastleScr_WorkRelease( CASTLE_SCRWORK* wk )
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

	MI_CpuClear8( wk, sizeof(CASTLE_SCRWORK) );
	sys_FreeMemoryEz( wk );
	wk = NULL;

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�L���b�X����ʌĂяo����̌��ʎ擾
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void CastleScr_GetResult( CASTLE_SCRWORK* wk, void* castle_call )
{
	int i;
	CASTLE_CALL_WORK* call_wk = castle_call;

	//���ʂ��擾����
	for( i=0; i < CASTLE_RET_WORK_MAX ;i++ ){
		wk->ret_work[i] = CastleCall_GetRetWork( castle_call, i );
		OS_Printf( "wk->ret_work[%d] = %d\n", i, wk->ret_work[i] );
	}

	//��񂪌��J���ꂽ���t���O
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		wk->enemy_temoti_flag[i]	= call_wk->enemy_temoti_flag[i];
		wk->enemy_level_flag[i]		= call_wk->enemy_level_flag[i];
		wk->enemy_tuyosa_flag[i]	= call_wk->enemy_tuyosa_flag[i];
		wk->enemy_waza_flag[i]		= call_wk->enemy_waza_flag[i];

		OS_Printf( "enemy_temoti_flag[%d] = %d\n", i, wk->enemy_temoti_flag[i] );
		OS_Printf( "enemy_level_flag[%d] = %d\n", i, wk->enemy_level_flag[i] );
		OS_Printf( "enemy_tuyosa_flag[%d] = %d\n", i, wk->enemy_tuyosa_flag[i] );
		OS_Printf( "enemy_waza_flag[%d] = %d\n", i, wk->enemy_waza_flag[i] );
	}

	//�p�[�g�i�[��CP
	wk->pair_cp	= call_wk->pair_cp;

	return;
}

//----------------------------------------------------------------------------
/**
 * @brief	�o�g���L���b�X���̃|�P�����I����ʂł̌��ʂ��擾
 *
 * @param	param	CASTLE_CALL_WORK�̃|�C���^
 * @param	pos		ret_work[pos](0-5)
 *
 * @return	"����"
 */
//----------------------------------------------------------------------------
u16 CastleCall_GetRetWork( void* param, u8 pos )
{
	CASTLE_CALL_WORK* castle_call = param;

	if( pos >= CASTLE_RET_WORK_MAX ){
		GF_ASSERT(0);
		return 0;
	}

	return castle_call->ret_work;
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
void CastleScr_SaveRestPlayData( CASTLE_SCRWORK* wk, u8 mode );
u16	CastleScr_IncRound( CASTLE_SCRWORK* wk );
u16	CastleScr_GetRound( CASTLE_SCRWORK* wk );
u16 CastleScr_GetEnemyObjCode( CASTLE_SCRWORK* wk, u8 param );
void CastleScr_SetLose( CASTLE_SCRWORK* wk );
void CastleScr_SetClear( CASTLE_SCRWORK* wk );

//--------------------------------------------------------------
/**
 * @brief	�x�ނƂ��Ɍ��݂̃v���C�󋵂��Z�[�u�ɏ����o��
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 * @param	mode	FR_MODE_CLEAR="�N���A",FR_MODE_LOSE="����",FR_MODE_REST="�x��"
 */
//--------------------------------------------------------------
void CastleScr_SaveRestPlayData( CASTLE_SCRWORK* wk, u8 mode )
{
	u16	i,before_max_rensyou,after_max_rensyou;
	u8 e_max;
	u8	buf8[4];
	u16	buf16[4];
	u32	buf32[4];
	u32 after,count,cp;
	FRONTIER_SAVEWORK* fr_sv;
	POKEMON_PARAM* pp;
	CASTLEDATA* f_sv = wk->castle_savedata;
	CASTLESCORE* score_sv = SaveData_GetCastleScore( wk->sv );

	fr_sv = SaveData_GetFrontier( wk->sv );

	//�^�C�v�ɂ���ă|�P�����̐����擾
	e_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_TOTAL );	//�G

	//"�V���O���A�_�u���A�}���`�Awifi�}���`"�����o��
	buf8[0] = wk->type;
	CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_TYPE, 0, 0, buf8 );
	
	//�Z�[�u�t���O��L����ԂɃ��Z�b�g
	CASTLEDATA_SetSaveFlag( wk->castle_savedata, TRUE );

	//"���E���h��"�����o��(0-6�l�ڂ̉��l�ڂ�������炷)
	buf8[0] = wk->round;
	//OS_Printf( "wk->round = %d\n", wk->round );
	CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_ROUND, 0, 0, buf8 );

#if 0
	//�A�� / 7 = ����
	//�A�� % 7 = ���l�ڂ�
#endif
	//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
	//OS_Printf( "rensyou = %d\n", wk->rensyou );
	FrontierRecord_Set(	fr_sv, 
						CastleScr_GetWinRecordID(wk->type),
						Frontier_GetFriendIndex(CastleScr_GetWinRecordID(wk->type)), wk->rensyou );
	
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//"�x��"�ȊO
	if( mode != FR_MODE_REST ){

		//�ύX�O"�ő�A����"���擾���Ă���
		before_max_rensyou = FrontierRecord_Get(	fr_sv, 
							CastleScr_GetMaxWinRecordID(wk->type),
							Frontier_GetFriendIndex(CastleScr_GetMaxWinRecordID(wk->type)) );

		//"�ő�A����"�����o��
		after = FrontierRecord_SetIfLarge(	fr_sv,
								CastleScr_GetMaxWinRecordID(wk->type),
								Frontier_GetFriendIndex(CastleScr_GetMaxWinRecordID(wk->type)), 
								wk->rensyou );

		//�ύX��"�ő�A����"���擾���Ă���
		after_max_rensyou = FrontierRecord_Get(	fr_sv, 
							CastleScr_GetMaxWinRecordID(wk->type),
							Frontier_GetFriendIndex(CastleScr_GetMaxWinRecordID(wk->type)) );

		//���݂�CP���擾���Ă���
		cp = FrontierRecord_Get(fr_sv, 
								CastleScr_GetCPRecordID(wk->type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );
		OS_Printf( "���݂�cp = %d\n", cp );

		//���݂̘A�������A�O�̍ő�A�����Ɠ�����
		if( wk->rensyou == before_max_rensyou ){

			//��r����"�ō��A���L�^��"��"�c��CP"���X�V����
			FrontierRecord_SetIfLarge(	fr_sv, 
					CastleScr_GetRemainderCPRecordID(wk->type),
					Frontier_GetFriendIndex(CastleScr_GetRemainderCPRecordID(wk->type)), 
					cp );

		//�ő�A�������X�V������
		}else if( before_max_rensyou < after_max_rensyou ){

			//"�ō��A���L�^��"��"�c��CP"���X�V����
			FrontierRecord_Set(	fr_sv, 
					CastleScr_GetRemainderCPRecordID(wk->type),
					Frontier_GetFriendIndex(CastleScr_GetRemainderCPRecordID(wk->type)), cp );
		}

		//"7�A��(�N���A)�������t���O"�����o��
		buf8[0] = wk->clear_flag;
		CASTLESCORE_PutScoreData( score_sv, CASTLESCORE_ID_CLEAR_FLAG, wk->type, 0, buf8 );

		//WIFI�̂ݓ���
		if( wk->type == CASTLE_TYPE_WIFI_MULTI ){
			FrontierRecord_Set(	fr_sv, 
					FRID_CASTLE_MULTI_WIFI_CLEAR_BIT,
					Frontier_GetFriendIndex(FRID_CASTLE_MULTI_WIFI_CLEAR_BIT), wk->clear_flag );
		}
	}
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	//"�g���[�i�[�C���f�b�N�X"�����o��
	for( i=0; i < CASTLE_LAP_MULTI_ENEMY_MAX ;i++ ){
		buf16[0] = wk->tr_index[i];
		CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_TR_INDEX, i, 0, buf16 );
	}

	//"�莝���ʒu"�����o��
	for( i=0; i < CASTLE_ENTRY_POKE_MAX ;i++ ){
		buf8[0] = wk->mine_poke_pos[i];
		CASTLEDATA_PutPlayData(	wk->castle_savedata, CASTLEDATA_ID_MINE_POKE_POS, i, 0, buf8 );
	}

	//"�莝���|�P�����̏��"�����o��
	count = PokeParty_GetPokeCount( wk->p_m_party );
	for( i=0; i < count ;i++ ){

		pp = PokeParty_GetMemberPointer( wk->p_m_party, i );

		buf16[0] = PokeParaGet( pp, ID_PARA_hp, NULL );			//HP
		OS_Printf( "�₷�ގ���HP = %d\n", buf16[0] );
		CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_TEMOTI_HP, i, 0, buf16 );

		buf8[0] = PokeParaGet( pp, ID_PARA_pp1, NULL );			//PP1
		CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_TEMOTI_PP, i, 0, buf8 );

		buf8[0] = PokeParaGet( pp, ID_PARA_pp2, NULL );			//PP2
		CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_TEMOTI_PP, i, 1, buf8 );

		buf8[0] = PokeParaGet( pp, ID_PARA_pp3, NULL );			//PP3
		CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_TEMOTI_PP, i, 2, buf8 );

		buf8[0] = PokeParaGet( pp, ID_PARA_pp4, NULL );			//PP4
		CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_TEMOTI_PP, i, 3, buf8 );

		buf32[0] = PokeParaGet( pp, ID_PARA_condition, NULL );	//CONDITION
		CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_TEMOTI_CONDITION, i, 0, buf32 );

		buf16[0] = PokeParaGet( pp, ID_PARA_item, NULL );		//ITEM
		CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_TEMOTI_ITEM, i, 0, buf16 );
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
		CASTLEDATA_PutPlayData( wk->castle_savedata, CASTLEDATA_ID_ENEMY_POKE_INDEX, i, 0, buf16 );
		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���E���h�����C���N�������g
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
u16	CastleScr_IncRound( CASTLE_SCRWORK* wk )
{
	wk->round++;
	return wk->round;
}

//--------------------------------------------------------------
/**
 * @brief	���E���h�����擾
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
u16	CastleScr_GetRound( CASTLE_SCRWORK* wk )
{
	return wk->round;
}

//--------------------------------------------------------------
/**
 * @brief	�ΐ�g���[�i�[OBJ�R�[�h�擾
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 * @param	param	0=1�l�ځA1=2�l��(�G�̃p�[�g�i�|)
 */
//--------------------------------------------------------------
u16 CastleScr_GetEnemyObjCode( CASTLE_SCRWORK* wk, u8 param )
{
	B_TOWER_TRAINER bt_trd;
	B_TOWER_TRAINER_ROM_DATA* p_rom_tr;
	u8 index;

	//�擾����g���[�i�[�f�[�^�̃C���f�b�N�X���擾
	index = wk->round + (param * CASTLE_LAP_ENEMY_MAX);

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
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void CastleScr_SetLose( CASTLE_SCRWORK* wk )
{
	u16 buf16[4];
	int i;

	OS_Printf( "\n�o�g���L���b�X���f�[�^�@�s��Z�b�g\n" );

	//���������̂݃����N�����ɖ߂�(7�A���̎��̓����N���̂܂܈����p��)
	buf16[0] = 1;
	for( i=0; i < CASTLE_RANK_TYPE_MAX ;i++ ){
		FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
					CastleScr_GetRankRecordID(wk->type,i),
					Frontier_GetFriendIndex(CastleScr_GetRankRecordID(wk->type,i)), 1 );
	}

	//���݂�5�A���Ȃǂ�ۑ�����K�v������I
	CastleScr_SaveRestPlayData( wk, FR_MODE_LOSE );		//�Z�[�u�f�[�^�ɑ��

#if 0
	//���݂̏�Ԃ͕ێ�����̂ŃN���A���Ȃ��Ă悢�@08.03.17
	//�Ē��킷�鎞�A�s���ɏI���������ɃN���A�����
	
	//"CP"��0�ɂ���
	FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
						CastleScr_GetCPRecordID(wk->type),
						Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)), 0 );
#endif

	//�V�K���A�p�����́AWK_SCENE_CASTLE_LOBBY�Ɋi�[����Ă���̂ŁA
	//������round�Ȃǂ��N���A���Ȃ��Ă��A
	//WK_SCENE_CASTLE_LOBBY���p���ł͂Ȃ���ԂŃZ�[�u�����̂ŁA
	//��t�ɘb�������Ă��A�V�K����ɂȂ胏�[�N�̓N���A�����B
	return;
}

//--------------------------------------------------------------
/**
 * @brief	7�A��(�N���A)�������̏���
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void CastleScr_SetClear( CASTLE_SCRWORK* wk )
{
	OS_Printf( "\n�o�g���L���b�X���f�[�^�@7�A��(�N���A)�Z�b�g\n" );
	
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
	if( wk->lap < CASTLE_LAP_MAX ){
		wk->lap++;							//���񐔂̃C���N�������g
	}

#if 0
	//CastleScr_CommGetLap�ł��ꂪ�o�Ȃ��悤�Ƀy�A���X�V
	if( wk->pair_lap < CASTLE_LAP_MAX ){
		wk->pair_lap++;						//���񐔂̃C���N�������g
	}
#endif

	wk->round			= 0;				//�����l�ځH
	CastleScr_SaveRestPlayData( wk, FR_MODE_CLEAR );		//�Z�[�u�f�[�^�ɑ��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�퓬�O��POKEPARTY�̃Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void CastleScr_BtlBeforePartySet( CASTLE_SCRWORK* wk )
{
	Castle_EnemyPartySet( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�o�g�����POKEPARTY�̃Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void CastleScr_BtlAfterPartySet( CASTLE_SCRWORK* wk )
{
	int i,count,tmp_count;
#if 0
	u8 e_max;
	u16 check_monsno[ CASTLE_TOTAL_POKE_MAX ];		//mine=4,enemy=4 = 8
	u16 check_itemno[ CASTLE_TOTAL_POKE_MAX ];
	POKEMON_PARAM* temp_poke;
	B_TOWER_TRAINER bt_trd;
	B_TOWER_TRAINER_ROM_DATA* trd;

	//������	
	for( i=0; i < CASTLE_TOTAL_POKE_MAX ; i++ ){	
		check_monsno[i] = 0;
		check_itemno[i] = 0;
	}

	//�^�C�v�ɂ���ă|�P�����̐����擾
	e_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_TOTAL );//�G

	count = PokeParty_GetPokeCount( wk->p_m_party );
	for( i=0; i < count ; i++ ){									//�莝���|�P����
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, i );
		check_monsno[i] = PokeParaGet(temp_poke, ID_PARA_monsno, NULL);
		check_itemno[i] = PokeParaGet(temp_poke, ID_PARA_item, NULL);

#if 0
		//PP�X�V
		wk->pp[i][0] = PokeParaGet( temp_poke, ID_PARA_pp1, NULL );
		wk->pp[i][1] = PokeParaGet( temp_poke, ID_PARA_pp2, NULL );
		wk->pp[i][2] = PokeParaGet( temp_poke, ID_PARA_pp3, NULL );
		wk->pp[i][3] = PokeParaGet( temp_poke, ID_PARA_pp4, NULL );
#endif
	}
	tmp_count = count;

	count = PokeParty_GetPokeCount( wk->p_e_party );
	for( i=0; i < count ; i++ ){									//�G�|�P����
		temp_poke = PokeParty_GetMemberPointer( wk->p_e_party, i );
		check_monsno[i+tmp_count] = PokeParaGet(temp_poke, ID_PARA_monsno, NULL);
		check_itemno[i+tmp_count] = PokeParaGet(temp_poke, ID_PARA_item, NULL);
	}

	SDK_ASSERTMSG( (tmp_count + count) <= CASTLE_TOTAL_POKE_MAX, "check buf�̗v�f��������Ȃ��I" );

	//ROM����g���[�i�[�f�[�^���m��
	trd = Frontier_TrainerDataGet( &bt_trd, wk->tr_index[wk->round], HEAPID_WORLD, ARC_PL_BTD_TR );

	//���|�P�A�A�C�e���A�f�[�^���A�擾���鐔�A�����
	Frontier_PokemonIndexCreate(	trd, check_monsno, check_itemno, 
									(tmp_count + count),				//�莝���A�G�̐�
									e_max, wk->enemy_poke_index, HEAPID_WORLD );

	sys_FreeMemoryEz( trd );

	//�G�|�P��������x�ɐ���(personal�Ȃ�)
	Frontier_PokemonParamCreateAll(	wk->enemy_poke, wk->enemy_poke_index, 
									wk->enemy_pow_rnd, NULL, wk->enemy_personal_rnd, 
									e_max, HEAPID_WORLD, ARC_PL_BTD_PM );



#else
	//�G�g���[�i�[���o���|�P��������
	Frontier_EnemyPokeMake(	Castle_GetEnemyPokeNum(wk->type,CASTLE_FLAG_TOTAL),
							wk->tr_index[wk->round],
							wk->tr_index[wk->round+CASTLE_LAP_ENEMY_MAX], 
							wk->enemy_poke_index, 
							wk->enemy_poke,						//B_TOWER_POKEMON
							wk->enemy_pow_rnd,
							wk->enemy_personal_rnd,
							Castle_CommCheck(wk->type) );

	for( i=0; i < 4 ;i++ ){
		OS_Printf( "enemy_poke[%d] = %d\n", i, wk->enemy_poke[i].mons_no );
		OS_Printf( "enemy_index[%d] = %d\n", i, wk->enemy_poke_index[i] );
		OS_Printf( "enemy_pow[%d] = %d\n", i, wk->enemy_pow_rnd[i] );
		OS_Printf( "enemy_personal[%d] = %d\n", i, wk->enemy_personal_rnd[i] );
	}
#endif

	//�G�p�[�e�B�̃Z�b�g
	Castle_EnemyPartySet( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�o�g����̃|�P�����f�[�^�X�V
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void CastleScr_BtlWinPokeData( CASTLE_SCRWORK* wk )
{
	u8 offset,m_max;
	int i,count;
	POKEMON_PARAM* temp_poke;

	//p_m_party�̃I�t�Z�b�g
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		offset = 0;
	}else{
		offset = CASTLE_COMM_POKE_NUM;
	}

	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_SOLO );
	count = PokeParty_GetPokeCount( wk->p_m_party );

	for( i=offset; i < (m_max+offset) ; i++ ){							//�莝���|�P����
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, i );

		//PP�X�V
		wk->pp[i-offset][0] = PokeParaGet( temp_poke, ID_PARA_pp1, NULL );
		wk->pp[i-offset][1] = PokeParaGet( temp_poke, ID_PARA_pp2, NULL );
		wk->pp[i-offset][2] = PokeParaGet( temp_poke, ID_PARA_pp3, NULL );
		wk->pp[i-offset][3] = PokeParaGet( temp_poke, ID_PARA_pp4, NULL );
	}

	//�莝���|�P�����̏�Ԃ��L���b�X���d�l��
	Castle_PokePartyRecoverAll( wk->p_m_party );

	//��񂪌��J���ꂽ���t���O���N���A
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		wk->enemy_temoti_flag[i]	= 0;
		wk->enemy_level_flag[i]		= 0;
		wk->enemy_tuyosa_flag[i]	= 0;
		wk->enemy_waza_flag[i]		= 0;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�o�g��������̂��炦��b�o�|�C���g�v�Z�A���₷
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 *
 * @return	"CP�|�C���g"
 */
//--------------------------------------------------------------
int CastleScr_GetBtlWinCpPoint( CASTLE_SCRWORK* wk )
{
	u8 m_max,e_max,offset;
	u32 hp,hpmax;
	int i,j,before_pp_max,after_pp_max;
	POKEMON_PARAM* pp;
	u8 tmp[20];
	int ret = 0;

	enum{
		ID_NO_HINSI,
		ID_HP_MAX,
		ID_HP_HALF_UP,
		ID_HP_HALF_DOWN,
		ID_CONDITION,
		ID_PP_5,
		ID_PP_10,
		ID_PP_15,
		ID_LV_PLUS,
		ID_LV_MINUS,			//10
	};

	OS_Printf( "********************\n�퓬��̂��炦��CP�v�Z\n" );

	//�N���A
	for( i=0; i < 20 ;i++ ){
		tmp[i] = 0;
	}

	wk->hinsi_flag = 0;
	after_pp_max = 0;

	//p_m_party�̃I�t�Z�b�g
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		offset = 0;
	}else{
		offset = CASTLE_COMM_POKE_NUM;
	}

	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_SOLO );
	e_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_TOTAL );

	for( i=offset; i < (m_max+offset); i++ ){

		pp = PokeParty_GetMemberPointer( wk->p_m_party, i );
		OS_Printf( "monsno = %d\n", PokeParaGet(pp,ID_PARA_monsno,NULL) );

		if( PokeParaGet(pp,ID_PARA_poke_exist,NULL) == FALSE ){
			continue;
		}

		hp		= PokeParaGet( pp, ID_PARA_hp, NULL );
		hpmax	= PokeParaGet( pp, ID_PARA_hpmax, NULL );

		//�C�₵�Ă��Ȃ��|�P������
		if( hp > 0 ){

			tmp[ID_NO_HINSI]++;

			//HP���^���̃|�P������
			if( hp == hpmax ){

				tmp[ID_HP_MAX]++;

			}else{

				//HP�����ȏ�̃|�P������(���^���܂܂�)
				if( hp >= (hpmax / 2) ){

					tmp[ID_HP_HALF_UP]++;

				}else{

					//HP�����ȉ�(�m���܂܂�)�̃|�P������
					tmp[ID_HP_HALF_DOWN]++;

				}
			}

			//��Ԉُ�ɂȂ��Ă��Ȃ��|�P������
			if( PokeParaGet(pp,ID_PARA_condition,NULL) == 0 ){
				tmp[ID_CONDITION]++;
			}

		}else{
			wk->hinsi_flag = 1;		//�m���|�P����������
		}

		//�퓬���PP���v���擾
		after_pp_max += PokeParaGet( pp, ID_PARA_pp1, NULL );
		after_pp_max += PokeParaGet( pp, ID_PARA_pp2, NULL );
		after_pp_max += PokeParaGet( pp, ID_PARA_pp3, NULL );
		after_pp_max += PokeParaGet( pp, ID_PARA_pp4, NULL );
	}

	//�ΐ�|�P����
	for( i=0; i < e_max; i++ ){

		//�ΐ푊��̃��x����+5�����|�P�����̐�
		if( wk->enemy_level_flag[i] == 1 ){

			tmp[ID_LV_PLUS]++;

		}else if( wk->enemy_level_flag[i] == 2 ){

			//�ΐ푊��̃��x����-5�����|�P�����̐�
			//tmp[ID_LV_MINUS]++;
		}
	}

	//�퓬�O��PP���v���擾
	before_pp_max = 0;
	for( i=0; i < m_max; i++ ){
		for( j=0; j < 4; j++ ){
			before_pp_max += wk->pp[i][j];
		}
	}

	//���vPP�̌������l��5�ȉ��̏ꍇ
	if( (before_pp_max - after_pp_max) <= 5 ){
		tmp[ID_PP_5]++;

	//���vPP�̌������l��10�ȉ��̏ꍇ
	}else if( (before_pp_max - after_pp_max) <= 10 ){
		tmp[ID_PP_10]++;

	//���vPP�̌������l��15�ȉ��̏ꍇ
	}else if( (before_pp_max - after_pp_max) <= 15 ){
		tmp[ID_PP_15]++;
	}

	//���_�v�Z
	ret += tmp[ID_NO_HINSI]		*3;
	OS_Printf( "ID_NO_HINSI = %d\n", tmp[ID_NO_HINSI] );
	ret += tmp[ID_HP_MAX]		*3;
	OS_Printf( "ID_HP_MAX = %d\n", tmp[ID_HP_MAX] );
	ret += tmp[ID_HP_HALF_UP]	*2;
	OS_Printf( "ID_HP_HALF_UP = %d\n", tmp[ID_HP_HALF_UP] );
	ret += tmp[ID_HP_HALF_DOWN]	*1;
	OS_Printf( "ID_HP_HALF_DOWN = %d\n", tmp[ID_HP_HALF_DOWN] );
	ret += tmp[ID_CONDITION]	*1;
	OS_Printf( "ID_CONDITION = %d\n", tmp[ID_CONDITION] );
	ret += tmp[ID_PP_5]			*8;
	OS_Printf( "ID_PP_5 = %d\n", tmp[ID_PP_5] );
	ret += tmp[ID_PP_10]		*6;
	OS_Printf( "ID_PP_10 = %d\n", tmp[ID_PP_10] );
	ret += tmp[ID_PP_15]		*4;
	OS_Printf( "ID_PP_15 = %d\n", tmp[ID_PP_15] );
	ret += tmp[ID_LV_PLUS]		*7;
	OS_Printf( "ID_LV_PLUS = %d\n", tmp[ID_LV_PLUS] );
	//ret -= tmp[ID_LV_MINUS]		*12;
	//OS_Printf( "ID_LV_MINUS = %d\n", tmp[ID_LV_MINUS] );

	if( ret <= 0 ){
		ret = 1;
	}

	OS_Printf( "���v = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	CP���₷
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	num			���₷�l
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleScr_AddCP( SAVEDATA* sv, u8 type, int num )
{
	u16 now_cp;

	//���݂�CP���擾
	now_cp = FrontierRecord_Get(SaveData_GetFrontier(sv), 
								CastleScr_GetCPRecordID(type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(type)) );

	//9999���~�b�g�`�F�b�N
	if( now_cp + num > CASTLE_CP_MAX ){
		FrontierRecord_Set(	SaveData_GetFrontier(sv), 
							CastleScr_GetCPRecordID(type),
							Frontier_GetFriendIndex(CastleScr_GetCPRecordID(type)), 
							CASTLE_CP_MAX );
	}else{
		FrontierRecord_Add(	SaveData_GetFrontier(sv), 
							CastleScr_GetCPRecordID(type),
							Frontier_GetFriendIndex(CastleScr_GetCPRecordID(type)), num );
	}

	//���R�[�h�X�V(����̓Q�[�������R�[�h�Ȃ̂Ő����͊O�ł����Ă����)
	RECORD_Add( SaveData_GetRecord(sv), RECID_CASTLE_POINT, num );
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
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
BOOL CastleScr_CommSetSendBuf( CASTLE_SCRWORK* wk, u16 type, u16 param )
{
	int ret;

	switch( type ){

	case CASTLE_COMM_BASIC:
		ret = CommCastleSendBufBasicData( wk );
		break;

	case CASTLE_COMM_TR:
		ret = CommCastleSendBufTrData( wk );
		break;

	case CASTLE_COMM_SEL:
		ret = CommCastleSendBufSelData( wk );
		break;

	case CASTLE_COMM_ENEMY:
		ret = CommCastleSendBufEnemyPokeData( wk );
		break;

	case CASTLE_COMM_RETIRE:
		ret = CommCastleSendBufRetireFlag( wk, param );
		break;

	case CASTLE_COMM_TRADE_YESNO:
		ret = CommCastleSendBufTradeYesNoFlag( wk, param );
		break;

	case CASTLE_COMM_TEMOTI:
		ret = CommCastleSendBufTemotiPokeData( wk );
		break;
	};

	return ret;
}


//==============================================================================================
//
//	���O�{CP�E�B���h�E
//
//==============================================================================================
#include "savedata/castle_savedata.h"
#include "frontier_scr.h"
#include "frontier_map.h"						//fmap->bgl
#include "system/window.h"						//BmpMenuWinWrite
#include "system/arc_tool.h"					//NARC_
#include "system/fontproc.h"					//FONT_SYSTEM
#include "communication/comm_info.h"			//CommInfoGetMyStatus
#include "msgdata\msg.naix"						//NARC_msg_??_dat
#include "msgdata\msg_castle_poke.h"

void FSSC_Sub_CPWinWrite( FSS_PTR fss, CASTLE_SCRWORK* bc_scr_wk );
void FSSC_Sub_CPWinDel( FSS_PTR fss, CASTLE_SCRWORK* bc_scr_wk );
void FSSC_Sub_CPWrite( FSS_PTR fss, CASTLE_SCRWORK* bc_scr_wk );
static void FSSC_Sub_CPWinWrite2( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );
static void FSSC_Sub_CPWrite2( FSS_PTR fss, GF_BGL_BMPWIN* win, MYSTATUS* my, u16 cp );

#define FSEVWIN_CP_BUF_SIZE		(10*2)			//CP���b�Z�[�W�o�b�t�@�T�C�Y

//--------------------------------------------------------------------------------------------
/**
 * CP�E�B���h�E�\��
 *
 * @param	fsys
 * @param	wk
 * @param	x		�\��X���W
 * @param	y		�\��Y���W
 *
 * @return	�m�ۂ���BMP�E�B���h�E
 *
 * EvWin_GoldWinDel�ō폜���邱�ƁI
 */
//--------------------------------------------------------------------------------------------
void FSSC_Sub_CPWinWrite( FSS_PTR fss, CASTLE_SCRWORK* bc_scr_wk )
{
	FMAP_PTR fmap = FSS_GetFMapAdrs( fss );

	GF_ASSERT( fss->pParentNameCPWin == NULL );
	GF_ASSERT( fss->pChildNameCPWin == NULL );

	//�V���O���A�_�u��
	if( Castle_CommCheck(bc_scr_wk->type) == FALSE ){
		fss->pParentNameCPWin	= GF_BGL_BmpWinAllocGet( HEAPID_WORLD, 1 );
		GF_BGL_BmpWinAdd(	fmap->bgl, fss->pParentNameCPWin, FRMAP_FRAME_WIN, 
							FFD_CP_WIN_PX, FFD_CP_WIN_PY,
							FFD_CP_WIN_SX, FFD_CP_WIN_SY, FFD_CP_WIN_PAL, FFD_CP_WIN_CGX );
		FSSC_Sub_CPWinWrite2( fmap->bgl, fss->pParentNameCPWin );
	//////////////////////////////////////////////////////////////////////////
	//�ʐM
	}else{
		fss->pParentNameCPWin	= GF_BGL_BmpWinAllocGet( HEAPID_WORLD, 1 );
		fss->pChildNameCPWin	= GF_BGL_BmpWinAllocGet( HEAPID_WORLD, 1 );

		GF_BGL_BmpWinAdd(	fmap->bgl, fss->pParentNameCPWin, FRMAP_FRAME_WIN, 
							FFD_CP_WIN_PX, FFD_CP_WIN_PY,
							FFD_CP_WIN_SX, FFD_CP_WIN_SY, FFD_CP_WIN_PAL, FFD_CP_WIN_CGX );

		GF_BGL_BmpWinAdd(	fmap->bgl, fss->pChildNameCPWin, FRMAP_FRAME_WIN, 
							FFD_CP2_WIN_PX, FFD_CP2_WIN_PY,
							FFD_CP2_WIN_SX, FFD_CP2_WIN_SY, FFD_CP2_WIN_PAL, FFD_CP2_WIN_CGX );

		FSSC_Sub_CPWinWrite2( fmap->bgl, fss->pParentNameCPWin );
		FSSC_Sub_CPWinWrite2( fmap->bgl, fss->pChildNameCPWin );
	}

	FSSC_Sub_CPWrite( fss, bc_scr_wk );
	return;
}

static void FSSC_Sub_CPWinWrite2( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win )
{
	//���j���[�E�B���h�E��`��
	BmpMenuWinWrite( win, WINDOW_TRANS_OFF, FR_MENU_WIN_CGX_NUM, FR_MENU_WIN_PAL );

	//�w��͈͂�h��Ԃ�
	GF_BGL_BmpWinDataFill( win, FBMP_COL_WHITE );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �������E�B���h�E�폜
 *
 * @param	wk
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FSSC_Sub_CPWinDel( FSS_PTR fss, CASTLE_SCRWORK* bc_scr_wk )
{
	GF_BGL_BMPWIN* l_win;
	GF_BGL_BMPWIN* r_win;

	//�V���O���A�_�u��
	if( Castle_CommCheck(bc_scr_wk->type) == FALSE ){
		GF_ASSERT( fss->pParentNameCPWin != NULL );
		l_win = fss->pParentNameCPWin;
		BmpMenuWinClear( l_win, WINDOW_TRANS_ON );
		GF_BGL_BmpWinFree( l_win, 1 );
	//////////////////////////////////////////////////////////////////////////
	//�ʐM
	}else{
		GF_ASSERT( fss->pParentNameCPWin != NULL );
		GF_ASSERT( fss->pChildNameCPWin != NULL );

		//�e
		if( CommGetCurrentID() == COMM_PARENT_ID ){
			l_win = fss->pParentNameCPWin;
			r_win = fss->pChildNameCPWin;
		//�q
		}else{
			l_win = fss->pChildNameCPWin;
			r_win = fss->pParentNameCPWin;
		}

		BmpMenuWinClear( l_win, WINDOW_TRANS_ON );
		GF_BGL_BmpWinFree( l_win, 1 );
		BmpMenuWinClear( r_win, WINDOW_TRANS_ON );
		GF_BGL_BmpWinFree( r_win, 1 );
	}

	fss->pParentNameCPWin = NULL;
	fss->pChildNameCPWin = NULL;
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �v���C���[���{CP�`��
 *
 * @param	fsys
 * @param	wk
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FSSC_Sub_CPWrite( FSS_PTR fss, CASTLE_SCRWORK* bc_scr_wk )
{
	u16 l_cp,r_cp;

	//�V���O���A�_�u��
	if( Castle_CommCheck(bc_scr_wk->type) == FALSE ){
		GF_ASSERT( fss->pParentNameCPWin != NULL );
		l_cp = FrontierRecord_Get(	SaveData_GetFrontier(bc_scr_wk->sv), 
							CastleScr_GetCPRecordID(bc_scr_wk->type),
							Frontier_GetFriendIndex(CastleScr_GetCPRecordID(bc_scr_wk->type)) );
		FSSC_Sub_CPWrite2( fss, fss->pParentNameCPWin, SaveData_GetMyStatus(bc_scr_wk->sv), l_cp );
	//////////////////////////////////////////////////////////////////////////
	//�ʐM
	}else{
		GF_ASSERT( fss->pParentNameCPWin != NULL );
		GF_ASSERT( fss->pChildNameCPWin != NULL );

		//�e
		if( CommGetCurrentID() == COMM_PARENT_ID ){
			l_cp = FrontierRecord_Get(	SaveData_GetFrontier(bc_scr_wk->sv), 
							CastleScr_GetCPRecordID(bc_scr_wk->type),
							Frontier_GetFriendIndex(CastleScr_GetCPRecordID(bc_scr_wk->type)) );
			r_cp = bc_scr_wk->pair_cp;
			OS_Printf( "�e�F���O�{CP�E�B���h�E\n" );
		//�q
		}else{
			l_cp = bc_scr_wk->pair_cp;
			r_cp = FrontierRecord_Get(	SaveData_GetFrontier(bc_scr_wk->sv), 
								CastleScr_GetCPRecordID(bc_scr_wk->type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(bc_scr_wk->type)) );
			OS_Printf( "�q�F���O�{CP�E�B���h�E\n" );
		}

		FSSC_Sub_CPWrite2( fss, fss->pParentNameCPWin, CommInfoGetMyStatus(0), l_cp );
		FSSC_Sub_CPWrite2( fss, fss->pChildNameCPWin, CommInfoGetMyStatus(1), r_cp );
	}

	return;
}

static void FSSC_Sub_CPWrite2( FSS_PTR fss, GF_BGL_BMPWIN* win, MYSTATUS* my, u16 cp )
{
	MSGDATA_MANAGER* man;
	STRBUF* tmp_buf = STRBUF_Create( FSEVWIN_CP_BUF_SIZE, fss->heap_id );
	STRBUF* tmp_buf2= STRBUF_Create( FSEVWIN_CP_BUF_SIZE, fss->heap_id );

	man = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_castle_poke_dat, HEAPID_WORLD );

	//////////////////////////////////////////////////////////////////////////////
	//CP
	WORDSET_RegisterNumber( fss->wordset, 0, cp,
							CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );

	MSGMAN_GetString( man, msg_castle_poke_cp_01, tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( fss->wordset, tmp_buf2, tmp_buf );

	GF_STR_PrintSimple( win, FONT_SYSTEM, tmp_buf2, CASTLE_NAME_CP_START_X, 2*8, MSG_NO_PUT, NULL );

	//////////////////////////////////////////////////////////////////////////////
	//���O
	WORDSET_RegisterPlayerName( fss->wordset, 0, my );

	MSGMAN_GetString( man, msg_castle_poke_name_01, tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( fss->wordset, tmp_buf2, tmp_buf );

	GF_STR_PrintSimple( win, FONT_SYSTEM, tmp_buf2, 0, 0, MSG_ALLPUT, NULL );

	//////////////////////////////////////////////////////////////////////////////
	STRBUF_Delete( tmp_buf );
	STRBUF_Delete( tmp_buf2 );

	MSGMAN_Delete( man );

	GF_BGL_BmpWinOn( win );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �L���b�X���N���A���ɖႦ��o�g���|�C���g���擾
 *
 * @param	wk
 *
 * @return	"�Ⴆ��o�g���|�C���g"
 */
//--------------------------------------------------------------------------------------------
u16	CastleScr_GetAddBtlPoint( CASTLE_SCRWORK* wk );
u16	CastleScr_GetAddBtlPoint( CASTLE_SCRWORK* wk )
{
	u8 add_bp;
	u16 lap;
	static const u8 bppoint_normal[CASTLE_LAP_MAX+1] = {0, 
													3, 3, 4, 4, 5, 5, 7, 7 };//0,1���`8���ȍ~

	static const u8 bppoint_multi[CASTLE_LAP_MAX+1]  = {0, 
													8, 9, 11, 12, 14, 15, 18, 18 };//0,1���`8���ȍ~

	//lap = CastleScr_CommGetLap( wk );
	lap = wk->lap;

	OS_Printf( "���� = %d\n", lap );
	OS_Printf( "�A���� = %d\n", wk->rensyou );

	//�V���O���A�_�u��
	if( (wk->type == CASTLE_TYPE_SINGLE) || (wk->type == CASTLE_TYPE_DOUBLE) ){

		//�ő���񐔃`�F�b�N
		if( lap >= CASTLE_LAP_MAX ){
			add_bp = bppoint_normal[ CASTLE_LAP_MAX ];		//�e�[�u����+1���Ă���̂�
		}else{
			add_bp = bppoint_normal[ lap ];
		}

	////////////////////////////////////////////////////////////////////////////////////
	//�}���`�AWIFI
	}else{

		//�ő���񐔃`�F�b�N
		if( lap >= CASTLE_LAP_MAX ){
			add_bp = bppoint_multi[ CASTLE_LAP_MAX ];		//�e�[�u����+1���Ă���̂�
		}else{
			add_bp = bppoint_multi[ lap ];
		}
	}

	//�u���[���`�F�b�N
	if( wk->type == CASTLE_TYPE_SINGLE ){
		if( (wk->rensyou == CASTLE_LEADER_SET_1ST) || (wk->rensyou == CASTLE_LEADER_SET_2ND) ){
			add_bp = 20;
		}
	}

	return add_bp;
}



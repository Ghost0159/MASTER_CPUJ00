//==============================================================================
/**
 * @file	roulette_tool.c
 * @brief	�o�g�����[���b�g�֘A�c�[����
 * @author	nohara
 * @date	2007.09.06
 */
//==============================================================================
#include "common.h"
#include "system/procsys.h"
#include "system/pm_str.h"
#include "poketool/poke_tool.h"
#include "poketool/status_rcv.h"
#include "itemtool/itemsym.h"

#include "frontier_tool.h"
#include "frontier_def.h"
#include "roulette_def.h"
#include "roulette_tool.h"

#include "msgdata/msg.naix"			//�b��


//==============================================================================
//	�f�[�^
//==============================================================================
//���g���[�i�[�f�[�^��0�I���W���Ȃ̂ŁA�f�[�^�쐬���̃i���o�[����-1���ĎQ�Ƃ���
///�T�񐔁A���l�ڂ̑��肩�A�ɂ��g���[�i�[�̑I�o�f�[�^�\����
static const struct{
	u16 start_no;		///<�P�`�U�l�ڂ̊J�nNo
	u16 end_no;			///<�P�`�U�l�ڂ̏I��No
	u16 boss_start;		///<7�l�ڂ̊J�nNo
	u16 boss_end;		///<7�l�ڂ̏I��No
}TrainerSelectRange[] = {
	{1-1,	100-1,	101-1,	120-1},
	{81-1,	120-1,	121-1,	140-1},
	{101-1,	140-1,	141-1,	160-1},
	{121-1,	160-1,	161-1,	180-1},
	{141-1,	180-1,	181-1,	200-1},
	{161-1,	200-1,	201-1,	220-1},
	{181-1,	220-1,	221-1,	240-1},
	{201-1,	300-1,	201-1,	300-1},
};


//==============================================================================
//	BP�l���C�x���g�̗�
//==============================================================================
static const u8 ev_bp_get_tbl[] = {
	1,		//����1-3
	1,		//����4-6
	1,		//����7-
};
static const u8 ev_bp_get_tbl2[] = {
	3,		//����1-3
	3,		//����4-6
	3,		//����7-
};
#define EV_BP_GET_TBL_MAX			( NELEMS(ev_bp_get_tbl) )


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static int Roulette_EnemyTrainerIndexGet( u8 type, int lap, int enemy_number );
void Roulette_EnemyLapAllTrainerIndexGet( u8 type, int lap, u16 trainer_index[], u8 num );


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �o������G�g���[�i�[�̃g���[�i�[�f�[�^No���擾����
 *
 * @param   lap					�T��(0�`)
 * @param   enemy_number		���Ԗڂɓo�ꂷ��G��(�O�`ROULETTE_LAP_ENEMY_MAX-1)
 *
 * @retval  �g���[�i�[�f�[�^No
 *
 * Lv50�A�I�[�v�����ʂł�
 */
//--------------------------------------------------------------
#define ROULETTE_LEADER_TR_INDEX_1ST	(311)	//21���(0�I���W��)
#define ROULETTE_LEADER_TR_INDEX_2ND	(312)	//42���

static int Roulette_EnemyTrainerIndexGet( u8 type, int lap, int enemy_number )
{
	int trainer_index, offset, start,check_num;
	
	//�u���[��(�V���O���̂�)
	if( type == ROULETTE_TYPE_SINGLE ){
		check_num = (lap * ROULETTE_LAP_ENEMY_MAX) + (enemy_number + 1);
		if( check_num == ROULETTE_LEADER_SET_1ST ){
			return ROULETTE_LEADER_TR_INDEX_1ST;
		}else if( check_num == ROULETTE_LEADER_SET_2ND ){
			return ROULETTE_LEADER_TR_INDEX_2ND;
		}
	}

	if(lap >= NELEMS(TrainerSelectRange)){
		lap = NELEMS(TrainerSelectRange) - 1;
	}

	if(enemy_number == ROULETTE_LAP_ENEMY_MAX-1){
		offset = TrainerSelectRange[lap].boss_end - TrainerSelectRange[lap].boss_start;
		start = TrainerSelectRange[lap].boss_start;
	}
	else{
		offset = TrainerSelectRange[lap].end_no - TrainerSelectRange[lap].start_no;
		start = TrainerSelectRange[lap].start_no;
	}
	
	trainer_index = start + (gf_rand() % offset);
	return trainer_index;
}

//--------------------------------------------------------------
/**
 * @brief   ���̎��ɓo�ꂷ��G�g���[�i�[Index��S�������Ȃ��悤�Ɏ擾����
 *
 * @param   lap					����(0�`)
 * @param   trainer_index		�g���[�i�[Index�����(ROULETTE_LAP_ENEMY_MAX���̗v�f�����K�v)
 * @param	num					�擾���鐔
 */
//--------------------------------------------------------------
void Roulette_EnemyLapAllTrainerIndexGet( u8 type, int lap, u16 trainer_index[], u8 num )
{
	int set_count = 0;
	int i;

	do{
		trainer_index[set_count] = Roulette_EnemyTrainerIndexGet( type, lap, set_count );

		//���`�F�b�N
		for(i = 0; i < set_count; i++){
			if(trainer_index[i] == trainer_index[set_count]){
				break;
			}
		}
		if(i != set_count){
			continue;
		}
		set_count++;
	}while(set_count < num);
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
//==============================================================================
//
//	07.04.10�ǉ�
//
//==============================================================================
#include "gflib/strbuf_family.h"
#include "savedata/savedata_def.h"
#include "savedata/savedata.h"
#include "savedata/zukanwork.h"
#include "savedata/record.h"
#include "savedata/b_tower_local.h"
#include "field/encount_set.h"
#include "field/ev_time.h"
#include "field/fieldobj_code.h"
#include "field/location.h"
#include "communication/comm_tool.h"
#include "communication/comm_system.h"
#include "communication/comm_info.h"
#include "communication/comm_def.h"
#include "battle/attr_def.h"
#include "battle/battle_common.h"
#include "battle/b_tower_data.h"


//==============================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================
u8 Roulette_GetMinePokeNum( u8 type, BOOL flag );
u8 Roulette_GetEnemyPokeNum( u8 type, BOOL flag );
BATTLE_PARAM* BtlRoulette_CreateBattleParam( ROULETTE_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param );
static u32 Roulette_GetFightType( u8 type );
u8 Roulette_GetLevel( ROULETTE_SCRWORK* wk );
BOOL Roulette_CommCheck( u8 type );
void Roulette_PokePartyRecoverAll( POKEPARTY* party );
void Roulette_EnemyPartySet( ROULETTE_SCRWORK* wk );
void Roulette_BirthInfoSet( ROULETTE_SCRWORK* wk, POKEMON_PARAM* poke );
void Roulette_PokePartyAdd( ROULETTE_SCRWORK* wk, POKEPARTY* party, POKEMON_PARAM* poke );
u16 Roulette_GetPanelBP( ROULETTE_SCRWORK* wk, u8 panel_no );
static u16 Roulette_GetTrAI( ROULETTE_SCRWORK* wk );
u16 RouletteScr_CommGetLap( ROULETTE_SCRWORK* wk );
u8 Roulette_GetEvPanelColor( u8 panel_no );


//==============================================================================
//
//	extern�錾
//
//==============================================================================
//�g���[�i�[�f�[�^�����i�������j
extern void BattleParam_TrainerDataMake( BATTLE_PARAM* bp );
//�g���[�i�[�f�[�^�����i�G���j
extern void BattleParam_EnemyTrainerDataMake( BATTLE_PARAM* bp );


//==============================================================================
//
//	�֐�
//
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   �^�C�v�ɂ���Ď����̃|�P�����̐����擾
 *
 * @param   type
 *
 * @param   "�|�P�����̐�"
 */
//--------------------------------------------------------------
u8 Roulette_GetMinePokeNum( u8 type, BOOL flag )
{
	switch( type ){

	case ROULETTE_TYPE_SINGLE:
	case ROULETTE_TYPE_DOUBLE:
		return 3;

	case ROULETTE_TYPE_MULTI:
	case ROULETTE_TYPE_WIFI_MULTI:
		if( flag == ROULETTE_FLAG_SOLO ){
			return ROULETTE_COMM_POKE_NUM;				//2
		}else{
			return ROULETTE_COMM_POKE_TOTAL_NUM;			//2+2=4
		}
	};

	//�G���[
	GF_ASSERT(0);
	return 3;
}

//--------------------------------------------------------------
/**
 * @brief   �^�C�v�ɂ���ēG�|�P�����̐����擾
 *
 * @param   type
 * @param   flag
 *
 * @param   "�|�P�����̐�"
 */
//--------------------------------------------------------------
u8 Roulette_GetEnemyPokeNum( u8 type, BOOL flag )
{
	switch( type ){

	case ROULETTE_TYPE_SINGLE:
	case ROULETTE_TYPE_DOUBLE:
		return 3;

	case ROULETTE_TYPE_MULTI:
	case ROULETTE_TYPE_WIFI_MULTI:
		if( flag == ROULETTE_FLAG_SOLO ){
			return ROULETTE_COMM_POKE_NUM;				//2
		}else{
			return ROULETTE_COMM_POKE_TOTAL_NUM;			//2+2=4
		}
	};

	//�G���[
	GF_ASSERT(0);
	return 3;
}

//--------------------------------------------------------------
/**
 * @brief	�o�g���L���b�X���p�@�o�g���p�����[�^����
 */
//--------------------------------------------------------------
BATTLE_PARAM* BtlRoulette_CreateBattleParam( ROULETTE_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param )
{
	int i;
	u32	val32;
	u8 m_max,e_max,mine_offset;
	BATTLE_PARAM* bp;
	POKEMON_PARAM* pp;
	B_TOWER_TRAINER bt_trd;
	B_TOWER_TRAINER_ROM_DATA* p_rom_tr;
	MSGDATA_MANAGER* man;
	STRBUF* msg;
	MYSTATUS* my;
	POKEPARTY* m_party;
	POKEPARTY* e_party;

	//�Q���|�P���������擾
	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_SOLO );
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_SOLO );

	//�퓬�p�����[�^�̐�������(field_battle.c)(fssc_roulette.c FSSC_RouletteCallGetResult��Delete)
	bp = BattleParam_Create( HEAPID_WORLD, Roulette_GetFightType(wk->type) );
	
	//�퓬�p�����[�^�̎��W�R�A(�t�B�[���h)
	BattleParam_SetParamByGameDataCore( bp, NULL, 
										ex_param->savedata,
										ex_param->zone_id,
										ex_param->fnote_data,
										ex_param->bag_cursor,
										ex_param->battle_cursor);

	//����͌Œ�I
	bp->bg_id		= BG_ID_ROULETTE;			//��{�w�i�w��
	bp->ground_id	= GROUND_ID_ROULETTE;		//��{�n�ʎw��

	//��̏�ԃZ�b�g(�V��)
	bp->weather		= wk->weather;

	//�|�P�����`�F���W�C�x���g�̎��̓Z�b�g����POKEPARTY���t�ɂȂ�
	m_party = wk->p_m_party;
	e_party = wk->p_e_party;
	if( wk->decide_ev_no == ROULETTE_EV_EX_POKE_CHANGE ){
		m_party = wk->p_e_party;
		e_party = wk->p_m_party;
	}

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//MINE�F�I�񂾎莝���|�P�������Z�b�g
	PokeParty_Init( bp->poke_party[POKEPARTY_MINE], m_max );

	//�e�͎�O��2�C�A�q�͌���2�C���Am_party����擾����
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		mine_offset = 0;
	}else{
		mine_offset = ROULETTE_COMM_POKE_NUM;
	}

	//MINE�F�����̃|�P�����f�[�^�Z�b�g
	pp = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < m_max ;i++ ){

		//POKEMON_PARAM�\���̂���POKEMON_PARAM�\���̂փR�s�[
		PokeCopyPPtoPP( PokeParty_GetMemberPointer(m_party,(mine_offset+i)), pp );

		//�퓬�p�����[�^�Z�b�g:�|�P������������
		BattleParam_AddPokemon( bp, pp, POKEPARTY_MINE );
		OS_Printf( "mine condition = %d\n", PokeParaGet(pp,ID_PARA_condition,NULL) );
	}
	sys_FreeMemoryEz( pp );

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//MINE�F�g���[�i�[�f�[�^����
	BattleParam_TrainerDataMake( bp );

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//ENEMY1�FROM����g���[�i�[�f�[�^���m��
	p_rom_tr = Frontier_TrainerDataGet( &bt_trd, wk->tr_index[wk->round], HEAPID_WORLD,
										ARC_PL_BTD_TR );
	sys_FreeMemoryEz( p_rom_tr );

	//ENEMY1�F�g���[�i�[�f�[�^���Z�b�g
	BattleParamTrainerDataSet( bp, &bt_trd, e_max, CLIENT_NO_ENEMY, HEAPID_WORLD );

	//ENEMY1�F�I�񂾎莝���|�P�������Z�b�g
	PokeParty_Init( bp->poke_party[POKEPARTY_ENEMY], 
					Roulette_GetEnemyPokeNum(wk->type,ROULETTE_FLAG_SOLO) );

	//AI�Z�b�g
	for( i=0; i < CLIENT_MAX ;i++ ){
		bp->trainer_data[i].aibit = Roulette_GetTrAI( wk );
	}
	OS_Printf( "�g���[�i�[��AI = %d\n", Roulette_GetTrAI(wk) );

	//ENEMY1�F�퓬�p�����[�^�Z�b�g:�|�P������������
	pp = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < e_max ;i++ ){

		//POKEMON_PARAM�\���̂���POKEMON_PARAM�\���̂փR�s�[
		PokeCopyPPtoPP( PokeParty_GetMemberPointer(e_party,i), pp );
		BattleParam_AddPokemon( bp, pp, POKEPARTY_ENEMY );
		OS_Printf( "enemy condition = %d\n", PokeParaGet(pp,ID_PARA_condition,NULL) );
	}
	sys_FreeMemoryEz( pp );

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�f�o�b�N���\��
//OS_Printf("p_party count = %d\n", PokeParty_GetPokeCount(wk->p_party) );
//OS_Printf("bp_party[mine]count= %d\n",PokeParty_GetPokeCount(bp->poke_party[POKEPARTY_MINE]));
//OS_Printf("bp_party[enemy]count= %d\n",PokeParty_GetPokeCount(bp->poke_party[POKEPARTY_ENEMY]));

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	switch( wk->type ){

	case ROULETTE_TYPE_MULTI:
	case ROULETTE_TYPE_WIFI_MULTI:

		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		//�p�[�g�i�[�̃f�[�^���i�[���Ă����Ȃ��ƃ_���I
		//B_TOWER_PARTNER_DATA	tr_data[2];

		//MINE2�F�g���[�i�[�f�[�^����
		BattleParam_TrainerDataMake( bp );

		//MINE2�F�p�[�g�i�[��MyStatus���擾���ăR�s�[
		my = CommInfoGetMyStatus( 1 - CommGetCurrentID() );
		MyStatus_Copy( my, bp->my_status[CLIENT_NO_MINE2] );

		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		//�p�[�g�i�[�̃f�[�^���i�[���Ă����Ȃ��ƃ_���I
		//B_TOWER_PARTNER_DATA	tr_data[2];

		//ENEMY2�FROM����g���[�i�[�f�[�^���m��
		p_rom_tr = Frontier_TrainerDataGet( &bt_trd, wk->tr_index[wk->round+ROULETTE_LAP_ENEMY_MAX],
											HEAPID_WORLD, ARC_PL_BTD_TR );
		sys_FreeMemoryEz( p_rom_tr );

		//ENEMY2�F�g���[�i�[�f�[�^���Z�b�g
		BattleParamTrainerDataSet( bp, &bt_trd, e_max, CLIENT_NO_ENEMY2, HEAPID_WORLD );

		//ENEMY2�F�I�񂾎莝���|�P�������Z�b�g
		PokeParty_Init( bp->poke_party[POKEPARTY_ENEMY_PAIR], 
						Roulette_GetEnemyPokeNum(wk->type,ROULETTE_FLAG_SOLO) );

		//ENEMY2�F�퓬�p�����[�^�Z�b�g:�|�P������������
		pp = PokemonParam_AllocWork( HEAPID_WORLD );
		for( i=0; i < e_max ;i++ ){

			//POKEMON_PARAM�\���̂���POKEMON_PARAM�\���̂փR�s�[
			PokeCopyPPtoPP( PokeParty_GetMemberPointer(e_party,(e_max+i)), pp );
			BattleParam_AddPokemon( bp, pp, POKEPARTY_ENEMY_PAIR );
		}
		sys_FreeMemoryEz( pp );

		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

		break;
	};

	//�񕜏���
	PokeParty_RecoverAll( wk->p_m_party );
	PokeParty_RecoverAll( wk->p_e_party );

	return bp;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�h����FIGHT_TYPE��Ԃ�
 */
//--------------------------------------------------------------
static u32 Roulette_GetFightType( u8 type )
{
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//
	//�L���b�X���p��FIGHT_TYPE���쐬���邩�m�F����I
	//
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	switch( type ){

	case ROULETTE_TYPE_SINGLE:
		return FIGHT_TYPE_BATTLE_TOWER_1vs1;

	case ROULETTE_TYPE_DOUBLE:
		return FIGHT_TYPE_BATTLE_TOWER_2vs2;

	case ROULETTE_TYPE_MULTI:
		return FIGHT_TYPE_BATTLE_TOWER_SIO_MULTI;	
		//return FIGHT_TYPE_MULTI_SIO;	

	case ROULETTE_TYPE_WIFI_MULTI:
		return FIGHT_TYPE_BATTLE_TOWER_SIO_MULTI;
		//return FIGHT_TYPE_MULTI_SIO;	
	}

	return FIGHT_TYPE_BATTLE_TOWER_1vs1;
}

//--------------------------------------------------------------
/**
 * @brief	���x���̒�`������ۂ̃��x�����擾
 *
 * @param	wk		
 *
 * @retval	"���ۂ̃��x��"
 */
//--------------------------------------------------------------
u8 Roulette_GetLevel( ROULETTE_SCRWORK* wk )
{
	return 50;
}

//--------------------------------------------------------------
/**
 * @brief	�ʐM�^�C�v���`�F�b�N
 *
 * @param	type		�^�C�v
 *
 * @return	"FALSE = �ʐM�ł͂Ȃ�"
 * @return	"TRUE  = �ʐM�ł���"
 */
//--------------------------------------------------------------
BOOL Roulette_CommCheck( u8 type )
{
	switch( type ){
	case ROULETTE_TYPE_MULTI:
	case ROULETTE_TYPE_WIFI_MULTI:
		return TRUE;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�����͂g�o�P�ŉ񕜁A��Ԉُ��
 *
 * @param	party		�񕜂���POKEPARTY�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void Roulette_PokePartyRecoverAll( POKEPARTY* party )
{
	int i, j,total;
	u32 buf;
	POKEMON_PARAM* pp;

	total = PokeParty_GetPokeCount( party );

	for( i=0; i < total; i++ ){

		pp = PokeParty_GetMemberPointer( party, i );

		if( PokeParaGet(pp,ID_PARA_poke_exist,NULL) == FALSE ){
			continue;
		}

		//HP=0��������AHP=1�ŉ�
		if( PokeParaGet(pp,ID_PARA_hp,NULL) == 0 ){
			buf = 1;
			PokeParaPut( pp, ID_PARA_hp, &buf );
		}

		//��Ԉُ��
		buf = 0;
		PokeParaPut( pp, ID_PARA_condition, &buf );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P�����z�������i�[
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 * @param	poke		POKEMON_PARAM�^�̃|�C���^
 *
 * @retval	none
 *
 * �e���Ƃ��Z�b�g���Ȃ��ƁA�X�e�[�^�X��ʂ̕\�������܂������Ȃ��̂ŃZ�b�g
 */
//--------------------------------------------------------------
void Roulette_BirthInfoSet( ROULETTE_SCRWORK* wk, POKEMON_PARAM* poke )
{
	/*******************************************/
	//�|�P�����̐��܂ꂽ�����ǂ����邩�v�m�F�I
	/*******************************************/

	PokeParaBirthInfoSet(	poke, SaveData_GetMyStatus(wk->sv),
							//int ball,int place,int ground_id,int heapID)
							ITEM_MONSUTAABOORU, 0, 0, HEAPID_WORLD );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P�����p�[�e�B�Ƀ|�P������ǉ�
 *
 * @param	wk			ROULETTE_SCRWORK�^�̃|�C���^
 * @param	party		POKEPARTY�^�̃|�C���^
 * @param	poke		POKEMON_PARAM�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void Roulette_PokePartyAdd( ROULETTE_SCRWORK* wk, POKEPARTY* party, POKEMON_PARAM* poke )
{
	Roulette_BirthInfoSet( wk, poke );	//�e���Ƃ��Z�b�g
	PokeParty_Add( party, poke );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�GPOKEPARTY�̃Z�b�g
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void Roulette_EnemyPartySet( ROULETTE_SCRWORK* wk )
{
	u32 buf;
	int i,j;
	u8 e_max;
	POKEPARTY* temoti_party;
	POKEMON_PARAM* temp_poke;
	POKEMON_PARAM* pp;

	PokeParty_InitWork( wk->p_e_party );								//POKEPARTY��������

	//�^�C�v�ɂ���ă|�P�����̐����擾
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );		//�G

	//�G�|�P�������Z�b�g
	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < e_max ;i++ ){

		//�G�|�P�����f�[�^�̕K�v�ȃf�[�^��POKEPARTY�ɃZ�b�g(field����͌ĂׂȂ��I)
		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake( &wk->enemy_poke[i], temp_poke, Roulette_GetLevel(wk) );
		Roulette_PokePartyAdd( wk, wk->p_e_party, temp_poke );

		OS_Printf("���̓Ge_party[%d] monsno = %d\n", i, PokeParaGet(temp_poke,ID_PARA_monsno,NULL));

		//�A�C�e�����O��
		pp = PokeParty_GetMemberPointer( wk->p_e_party, i );
		buf = 0;
		PokeParaPut( pp, ID_PARA_item, &buf );
	}
	sys_FreeMemoryEz( temp_poke );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Ⴆ��BP�|�C���g���擾(�p�l����BPGET�𓖂Ă���)
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
u16 Roulette_GetPanelBP( ROULETTE_SCRWORK* wk, u8 panel_no )
{
	u16 add_bp,lap;

	//���񐔂��擾
	lap = RouletteScr_CommGetLap( wk );
	//OS_Printf( "lap = %d\n", lap );		//0�I���W��

	//BP��
	if( panel_no == ROULETTE_EV_EX_BP_GET ){
		if( lap < 3  ){
			add_bp = ev_bp_get_tbl[0];
		}else if( lap < 6 ){
			add_bp = ev_bp_get_tbl[1];
		}else{
			add_bp = ev_bp_get_tbl[2];
		}

	//BP��
	}else{
		if( lap < 3  ){
			add_bp = ev_bp_get_tbl2[0];
		}else if( lap < 6 ){
			add_bp = ev_bp_get_tbl2[1];
		}else{
			add_bp = ev_bp_get_tbl2[2];
		}
	}

	return add_bp;
}

//--------------------------------------------------------------
/**
 * @brief	���񐔂ɂ���ăg���[�i�[AI�����肵�Ď擾
 *
 * @param	
 *
 * @return	"�g���[�i�[AI"
 */
//--------------------------------------------------------------
static u16 Roulette_GetTrAI( ROULETTE_SCRWORK* wk )
{
	u16 ai,lap;

#if 1
	//�u���[��
	if( wk->type == ROULETTE_TYPE_SINGLE ){
		if( (wk->tr_index[wk->round] == ROULETTE_LEADER_TR_INDEX_1ST) ||
			(wk->tr_index[wk->round] == ROULETTE_LEADER_TR_INDEX_2ND) ){
			return FR_AI_EXPERT;
		}
	}
#endif

	//���񐔂��擾
	lap = RouletteScr_CommGetLap( wk );

	//5���ڈȍ~�͑S�ăG�L�X�p�[�g
	ai = FR_AI_EXPERT;

	//0�I���W���Ȃ̂�+1���Čv�Z
	switch( (lap+1) ){

	case 1:
	case 2:
		ai = FR_AI_YASEI;
		break;

	case 3:
	case 4:
		ai = FR_AI_BASIC;
		break;
	};

	return ai;
}

//--------------------------------------------------------------
/**
 * @brief	"�e�q"�̎��񐔂��r���āA�g�p������񐔂��擾
 *
 * @param	wk		ROULETTE_SCRWORK�^�̃|�C���^
 *
 * @return	"����"
 */
//--------------------------------------------------------------
u16 RouletteScr_CommGetLap( ROULETTE_SCRWORK* wk )
{
	u16	lap;
	
	lap = wk->lap;

	if( Roulette_CommCheck(wk->type) == TRUE ){

		//�ʐM���ɂ͎��񐔂̑����ق��Œ��I
		if( wk->pair_lap > wk->lap ){
			lap = wk->pair_lap;
		}
	}

	return lap;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g��ނ���A�p�l���̐F���擾
 *
 * @param	
 *
 * @return	"�J���[��`"
 */
//--------------------------------------------------------------
u8 Roulette_GetEvPanelColor( u8 panel_no )
{
	if( panel_no < ROULETTE_EV_MINE_START ){
		return ROULETTE_PANEL_COLOR_RED;
	}else if( panel_no < ROULETTE_EV_PLACE_START ){
		return ROULETTE_PANEL_COLOR_BLACK;
	}else if( panel_no < ROULETTE_EV_EX_START ){
		return ROULETTE_PANEL_COLOR_WHITE;
	}

	return ROULETTE_PANEL_COLOR_YELLOW;
}



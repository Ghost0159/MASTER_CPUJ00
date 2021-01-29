//==============================================================================
/**
 * @file	castle_tool.c
 * @brief	�o�g���L���b�X���֘A�c�[����
 * @author	nohara
 * @date	2007.07.02
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
#include "castle_def.h"
#include "castle_tool.h"
#include "../field/scr_tool.h"

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
//	�v���g�^�C�v�錾
//==============================================================================
static int Castle_EnemyTrainerIndexGet( u8 type, int lap_num, int enemy_number );
void Castle_EnemyLapAllTrainerIndexGet( u8 type, int lap, u16 trainer_index[], u8 num );


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �o������G�g���[�i�[�̃g���[�i�[�f�[�^No���擾����
 *
 * @param   lap					�T��(0�`)
 * @param   enemy_number		���Ԗڂɓo�ꂷ��G��(�O�`CASTLE_LAP_ENEMY_MAX-1)
 *
 * @retval  �g���[�i�[�f�[�^No
 *
 * Lv50�A�I�[�v�����ʂł�
 */
//--------------------------------------------------------------
#define CASTLE_LEADER_TR_INDEX_1ST	(313)	//21���(0�I���W��)
#define CASTLE_LEADER_TR_INDEX_2ND	(314)	//42���

static int Castle_EnemyTrainerIndexGet( u8 type, int lap_num, int enemy_number )
{
	int trainer_index, offset, start,check_num,lap;
	
	//�u���[��(�V���O���̂�)
	if( type == CASTLE_TYPE_SINGLE ){
		check_num = (lap_num * CASTLE_LAP_ENEMY_MAX) + (enemy_number + 1);
		if( check_num == CASTLE_LEADER_SET_1ST ){
			return CASTLE_LEADER_TR_INDEX_1ST;
		}else if( check_num == CASTLE_LEADER_SET_2ND ){
			return CASTLE_LEADER_TR_INDEX_2ND;
		}
	}

	lap = lap_num;
	if(lap >= NELEMS(TrainerSelectRange)){
		lap = NELEMS(TrainerSelectRange) - 1;
	}
	
	if(enemy_number == CASTLE_LAP_ENEMY_MAX-1){
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
 * @param   trainer_index		�g���[�i�[Index�����(CASTLE_LAP_ENEMY_MAX���̗v�f�����K�v)
 * @param   num					�擾���鐔
 */
//--------------------------------------------------------------
void Castle_EnemyLapAllTrainerIndexGet( u8 type, int lap, u16 trainer_index[], u8 num )
{
	int set_count = 0;
	int i;
	
	do{
		trainer_index[set_count] = Castle_EnemyTrainerIndexGet( type, lap, set_count );
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
u8 Castle_GetMinePokeNum( u8 type, BOOL flag );
u8 Castle_GetEnemyPokeNum( u8 type, BOOL flag );
BATTLE_PARAM* BtlCastle_CreateBattleParam( CASTLE_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param );
static u32 Castle_GetFightType( u8 type );
u8 Castle_GetLevel( CASTLE_SCRWORK* wk );
BOOL Castle_CommCheck( u8 type );
void Castle_PokePartyRecoverAll( POKEPARTY* party );
void Castle_EnemyPartySet( CASTLE_SCRWORK* wk );
void Castle_BirthInfoSet( CASTLE_SCRWORK* wk, POKEMON_PARAM* poke );
void Castle_PokePartyAdd( CASTLE_SCRWORK* wk, POKEPARTY* party, POKEMON_PARAM* poke );
static u16 Castle_GetTrAI( CASTLE_SCRWORK* wk );
u16 CastleScr_CommGetLap( CASTLE_SCRWORK* wk );
u16 Castle_GetLap( u16 lap );


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
u8 Castle_GetMinePokeNum( u8 type, BOOL flag )
{
	switch( type ){

	case CASTLE_TYPE_SINGLE:
	case CASTLE_TYPE_DOUBLE:
		return 3;

	case CASTLE_TYPE_MULTI:
	case CASTLE_TYPE_WIFI_MULTI:
		if( flag == CASTLE_FLAG_SOLO ){
			return CASTLE_COMM_POKE_NUM;				//2
		}else{
			return CASTLE_COMM_POKE_TOTAL_NUM;			//2+2=4
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
u8 Castle_GetEnemyPokeNum( u8 type, BOOL flag )
{
	switch( type ){

	case CASTLE_TYPE_SINGLE:
	case CASTLE_TYPE_DOUBLE:
		return 3;

	case CASTLE_TYPE_MULTI:
	case CASTLE_TYPE_WIFI_MULTI:
		if( flag == CASTLE_FLAG_SOLO ){
			return CASTLE_COMM_POKE_NUM;				//2
		}else{
			return CASTLE_COMM_POKE_TOTAL_NUM;			//2+2=4
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
BATTLE_PARAM* BtlCastle_CreateBattleParam( CASTLE_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param )
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

	//�Q���|�P���������擾
	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_SOLO );
	e_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_SOLO );

	//�񕜏���
#if 0
	PokeParty_RecoverAll( wk->p_m_party );
	PokeParty_RecoverAll( wk->p_e_party );
#else
	//Castle_PokePartyRecoverAll( wk->p_m_party );	//CastleScr_BtlAfterPartySet�Ɉړ�
	PokeParty_RecoverAll( wk->p_e_party );
#endif

	//�퓬�p�����[�^�̐�������(field_battle.c)(fssc_castle.c FSSC_CastleCallGetResult��Delete)
	bp = BattleParam_Create( HEAPID_WORLD, Castle_GetFightType(wk->type) );
	
	//�퓬�p�����[�^�̎��W�R�A(�t�B�[���h)
	BattleParam_SetParamByGameDataCore( bp, NULL, 
										ex_param->savedata,
										ex_param->zone_id,
										ex_param->fnote_data,
										ex_param->bag_cursor,
										ex_param->battle_cursor);

	//����͌Œ�I
	bp->bg_id		= BG_ID_CASTLE;			//��{�w�i�w��
	bp->ground_id	= GROUND_ID_CASTLE;		//��{�n�ʎw��

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//MINE�F�I�񂾎莝���|�P�������Z�b�g
	PokeParty_Init( bp->poke_party[POKEPARTY_MINE], m_max );

	//�e�͎�O��2�C�A�q�͌���2�C���Ap_m_party����擾����
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		mine_offset = 0;
	}else{
		mine_offset = CASTLE_COMM_POKE_NUM;
	}

	//MINE�F�����̃|�P�����f�[�^�Z�b�g
	pp = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < m_max ;i++ ){

		//POKEMON_PARAM�\���̂���POKEMON_PARAM�\���̂փR�s�[
		PokeCopyPPtoPP( PokeParty_GetMemberPointer(wk->p_m_party,(mine_offset+i)), pp );

		//�퓬�p�����[�^�Z�b�g:�|�P������������
		BattleParam_AddPokemon( bp, pp, POKEPARTY_MINE );
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
					Castle_GetEnemyPokeNum(wk->type,CASTLE_FLAG_SOLO) );

	//AI�Z�b�g
	for( i=0; i < CLIENT_MAX ;i++ ){
		bp->trainer_data[i].aibit = Castle_GetTrAI( wk );
	}
	OS_Printf( "�g���[�i�[��AI = %d\n", Castle_GetTrAI(wk) );

	//ENEMY1�F�퓬�p�����[�^�Z�b�g:�|�P������������
	pp = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < e_max ;i++ ){

		//POKEMON_PARAM�\���̂���POKEMON_PARAM�\���̂փR�s�[
		PokeCopyPPtoPP( PokeParty_GetMemberPointer(wk->p_e_party,i), pp );
		BattleParam_AddPokemon( bp, pp, POKEPARTY_ENEMY );
	}
	sys_FreeMemoryEz( pp );

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�f�o�b�N���\��
//OS_Printf("p_party count = %d\n", PokeParty_GetPokeCount(wk->p_party) );
//OS_Printf("bp_party[mine]count= %d\n",PokeParty_GetPokeCount(bp->poke_party[POKEPARTY_MINE]));
//OS_Printf("bp_party[enemy]count= %d\n",PokeParty_GetPokeCount(bp->poke_party[POKEPARTY_ENEMY]));

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	switch( wk->type ){

	case CASTLE_TYPE_MULTI:
	case CASTLE_TYPE_WIFI_MULTI:

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
		p_rom_tr = Frontier_TrainerDataGet( &bt_trd, wk->tr_index[wk->round+CASTLE_LAP_ENEMY_MAX],
											HEAPID_WORLD, ARC_PL_BTD_TR );
		sys_FreeMemoryEz( p_rom_tr );

		//ENEMY2�F�g���[�i�[�f�[�^���Z�b�g
		BattleParamTrainerDataSet( bp, &bt_trd, e_max, CLIENT_NO_ENEMY2, HEAPID_WORLD );

		//ENEMY2�F�I�񂾎莝���|�P�������Z�b�g
		PokeParty_Init( bp->poke_party[POKEPARTY_ENEMY_PAIR], 
						Castle_GetEnemyPokeNum(wk->type,CASTLE_FLAG_SOLO) );

		//ENEMY2�F�퓬�p�����[�^�Z�b�g:�|�P������������
		pp = PokemonParam_AllocWork( HEAPID_WORLD );
		for( i=0; i < e_max ;i++ ){

			//POKEMON_PARAM�\���̂���POKEMON_PARAM�\���̂փR�s�[
			PokeCopyPPtoPP( PokeParty_GetMemberPointer(wk->p_e_party,(e_max+i)), pp );
			BattleParam_AddPokemon( bp, pp, POKEPARTY_ENEMY_PAIR );
		}
		sys_FreeMemoryEz( pp );

		//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

		break;
	};

	return bp;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�h����FIGHT_TYPE��Ԃ�
 */
//--------------------------------------------------------------
static u32 Castle_GetFightType( u8 type )
{
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//
	//�L���b�X���p��FIGHT_TYPE���쐬���邩�m�F����I
	//
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	switch( type ){

	case CASTLE_TYPE_SINGLE:
		return FIGHT_TYPE_BATTLE_TOWER_1vs1;

	case CASTLE_TYPE_DOUBLE:
		return FIGHT_TYPE_BATTLE_TOWER_2vs2;

	case CASTLE_TYPE_MULTI:
		return FIGHT_TYPE_BATTLE_TOWER_SIO_MULTI;	
		//return FIGHT_TYPE_MULTI_SIO;	

	case CASTLE_TYPE_WIFI_MULTI:
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
u8 Castle_GetLevel( CASTLE_SCRWORK* wk )
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
BOOL Castle_CommCheck( u8 type )
{
	switch( type ){
	case CASTLE_TYPE_MULTI:
	case CASTLE_TYPE_WIFI_MULTI:
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
void Castle_PokePartyRecoverAll( POKEPARTY* party )
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
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	poke		POKEMON_PARAM�^�̃|�C���^
 *
 * @retval	none
 *
 * �e���Ƃ��Z�b�g���Ȃ��ƁA�X�e�[�^�X��ʂ̕\�������܂������Ȃ��̂ŃZ�b�g
 */
//--------------------------------------------------------------
void Castle_BirthInfoSet( CASTLE_SCRWORK* wk, POKEMON_PARAM* poke )
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
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	party		POKEPARTY�^�̃|�C���^
 * @param	poke		POKEMON_PARAM�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void Castle_PokePartyAdd( CASTLE_SCRWORK* wk, POKEPARTY* party, POKEMON_PARAM* poke )
{
	Castle_BirthInfoSet( wk, poke );	//�e���Ƃ��Z�b�g
	PokeParty_Add( party, poke );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�GPOKEPARTY�̃Z�b�g
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
void Castle_EnemyPartySet( CASTLE_SCRWORK* wk )
{
	int i,j;
	u8 e_max;
	POKEMON_PARAM* temp_poke;

	PokeParty_InitWork( wk->p_e_party );								//POKEPARTY��������

	//�^�C�v�ɂ���ă|�P�����̐����擾
	e_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_TOTAL );		//�G

	//�G�|�P�������Z�b�g
	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < e_max ;i++ ){

		//�G�|�P�����f�[�^�̕K�v�ȃf�[�^��POKEPARTY�ɃZ�b�g(field����͌ĂׂȂ��I)
		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake( &wk->enemy_poke[i], temp_poke, Castle_GetLevel(wk) );
		Castle_PokePartyAdd( wk, wk->p_e_party, temp_poke );

		OS_Printf("���̓Ge_party[%d] monsno = %d\n", i, PokeParaGet(temp_poke,ID_PARA_monsno,NULL));
	}
	sys_FreeMemoryEz( temp_poke );

	return;
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
static u16 Castle_GetTrAI( CASTLE_SCRWORK* wk )
{
	u16 ai,lap;

#if 1
	//�u���[��
	if( wk->type == CASTLE_TYPE_SINGLE ){
		if( (wk->tr_index[wk->round] == CASTLE_LEADER_TR_INDEX_1ST) ||
			(wk->tr_index[wk->round] == CASTLE_LEADER_TR_INDEX_2ND) ){
			return FR_AI_EXPERT;
		}
	}
#endif

	//���񐔂��擾
	lap = CastleScr_CommGetLap( wk );

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
 * @param	wk		CASTLE_SCRWORK�^�̃|�C���^
 *
 * @return	"����"
 *
 * �ʐM���đ���̎��񐔂��擾�������ƂɎg�p�\
 */
//--------------------------------------------------------------
u16 CastleScr_CommGetLap( CASTLE_SCRWORK* wk )
{
	u16	lap;
	
	lap = wk->lap;

	if( Castle_CommCheck(wk->type) == TRUE ){

		//�ʐM���ɂ͎��񐔂̑����ق��Œ��I
		if( wk->pair_lap > wk->lap ){
			lap = wk->pair_lap;
		}
	}

	return lap;
}

//--------------------------------------------------------------
/**
 * @brief	�L���b�X�����񐔂������X�N���[�����X�V
 *
 * @param	wk		BATON_WORK�^�̃|�C���^
 * @param	frm		BG�t���[��
 *
 * @return	none
 */
//--------------------------------------------------------------
#define CASTLE_LAP_CHAR_XSIZE	(10)							//
#define CASTLE_LAP_CHAR_HALF	(CASTLE_LAP_CHAR_XSIZE / 2)		//�܂�Ԃ��n�_
//#define CASTLE_LAP_CHAR_YSIZE	(2)								//
#define CASTLE_LAP_CHAR_YSIZE	(3)								//
#define CASTLE_LAP_SCRN_SIZE	(CASTLE_LAP_CHAR_XSIZE * CASTLE_LAP_CHAR_YSIZE)
#define CASTLE_CHAR_START		(0x10)
#define CASTLE_CHAR_ADD			(0x60)
#define NUM_WRITE_X				(11)							//��������X
#define NUM_WRITE_Y				(6)								//��������Y
#define H_FLIP_BIT				(10)							//H�t���b�v�̃r�b�g

void Castle_LapNumWrite( GF_BGL_INI* ini, CASTLE_SCRWORK* wk, u32 frm );
static void Castle_LapNumWriteSub( u16* src, u16 lap_num );

void Castle_LapNumWrite( GF_BGL_INI* ini, CASTLE_SCRWORK* wk, u32 frm )
{
	int i;
	u16 src[CASTLE_LAP_SCRN_SIZE];

	Castle_LapNumWriteSub( src, CastleScr_CommGetLap(wk) );

	GF_BGL_ScrWrite(ini, frm, src, NUM_WRITE_X, NUM_WRITE_Y, 
					CASTLE_LAP_CHAR_XSIZE, CASTLE_LAP_CHAR_YSIZE );	//�X�N���[���f�[�^��������

	GF_BGL_LoadScreenV_Req( ini, frm );								//�X�N���[���f�[�^�]��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�o�b�t�@�ɏ�������
 *
 * @param	src		�������݃o�b�t�@
 * @param	lap_num	����(wk->lap��0�I���W��)
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_LapNumWriteSub( u16* src, u16 lap_num )
{
	u8 char_tbl[CASTLE_LAP_CHAR_XSIZE];
	u16 lap;
	u32 char_offset,i,j,no;

	lap = Castle_GetLap( lap_num );
	OS_Printf( "lap = %d\n", lap );

	//�t���b�v����̂ŎQ�ƈʒu���쐬
	for( i=0; i < CASTLE_LAP_CHAR_HALF ;i++ ){
		char_tbl[i] = i;													//0,1,2,3,4
		char_tbl[CASTLE_LAP_CHAR_HALF+i] = ( (CASTLE_LAP_CHAR_HALF-1) - i);	//4,3,2,1,0
	}

	//���񐔃L�����N�^�f�[�^�̃I�t�Z�b�g�擾
	char_offset = (CASTLE_CHAR_ADD * lap) + CASTLE_CHAR_START;
	OS_Printf( "char_offset = %d\n", char_offset );

	for( i=0; i < CASTLE_LAP_CHAR_YSIZE ;i++ ){
		for( j=0; j < CASTLE_LAP_CHAR_XSIZE ;j++ ){

			//0x20=CHAR�t�@�C����Y�I�t�Z�b�g
			no = char_offset + (i*0x20) + char_tbl[j];
			OS_Printf( "no = %d\n", no );
			src[ j + (i*CASTLE_LAP_CHAR_XSIZE) ] = no;

			//�t���b�v
			if( j >= CASTLE_LAP_CHAR_HALF ){
				src[ j + (i*CASTLE_LAP_CHAR_XSIZE) ] |= (1 << H_FLIP_BIT);	//H�t���b�v
			}
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	������l�����Ď��񐔂��擾
 *
 * @param	lap		����
 *
 * @return	"����"
 */
//--------------------------------------------------------------
u16 Castle_GetLap( u16 lap )
{
	if( lap >= CASTLE_LAP_MAX ){
		return (CASTLE_LAP_MAX - 1);
	}

	return lap;
}

//--------------------------------------------------------------
/**
 * @brief	CP���R�[�h�����炷(USE_CP���R�[�h���v�Z)
 *
 * @param	fro_sv		FRONTIER_SAVEWORK�^�̃|�C���^
 * @param	type		�o�g���^�C�v
 * @param	value		���炷CP�̒l
 *
 * @return	none
 */
//--------------------------------------------------------------
void Castle_CPRecord_Sub( FRONTIER_SAVEWORK* fro_sv, u8 type, int value )
{
	u16 use_cp;

	//CP���R�[�h�����炷
	FrontierRecord_Sub(	fro_sv, CastleScr_GetCPRecordID(type),
						Frontier_GetFriendIndex(CastleScr_GetCPRecordID(type)), value );

	//���݂̎g�p����CP���R�[�h���擾
	use_cp = FrontierRecord_Get(fro_sv, CastleScr_GetUsedCPRecordID(type),
								Frontier_GetFriendIndex(CastleScr_GetUsedCPRecordID(type)) );

	//�g�p����CP���R�[�h�𑝂₷
	//9999���~�b�g�`�F�b�N
	if( use_cp + value > CASTLE_USE_CP_MAX ){
		FrontierRecord_Set(	fro_sv, CastleScr_GetUsedCPRecordID(type),
							Frontier_GetFriendIndex(CastleScr_GetUsedCPRecordID(type)), 
							CASTLE_USE_CP_MAX );
	}else{
		FrontierRecord_Add(	fro_sv, CastleScr_GetUsedCPRecordID(type),
							Frontier_GetFriendIndex(CastleScr_GetUsedCPRecordID(type)), value );
	}

	return;
}


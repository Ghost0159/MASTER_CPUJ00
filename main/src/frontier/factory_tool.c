//==============================================================================
/**
 * @file	factory_tool.c
 * @brief	�o�g���t�@�N�g���[�֘A�c�[����
 * @author	Satoshi Nohara
 * @date	2007.08.31
 *
 * ���c���񂩂���p��
 */
//==============================================================================
#include "common.h"
#include "system/procsys.h"
#include "system/pm_str.h"
#include "poketool/poke_tool.h"
#include "poketool/status_rcv.h"

#include "frontier_tool.h"
#include "frontier_def.h"
#include "factory_def.h"
#include "factory_tool.h"

#include "msgdata/msg.naix"			//�b��


#define FACTORY_LEADER_TR_INDEX_1ST	(309)	//21���(0�I���W��)
#define FACTORY_LEADER_TR_INDEX_2ND	(310)	//42���

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

//���|�P�����f�[�^��1�I���W���Ŏn�܂�(0�͖����ȃf�[�^�Ȃ̂ŎQ�Ƃ��Ă̓_���I)
///���x��50�̓G���g�p���Ă���|�P�����f�[�^��Index�擾�͈�(�|�P������1�I���W��)
static const FACTORY_POKE_RANGE Level50EnemyPokemonRange[] = {
	{100,	1,		150,	0,		1},
	{120,	151,	250,	4,		1},
	{140,	251,	350,	8,		1},
	{160,	351,	486,	12,		1},
	{180,	487,	622,	16,		1},
	{200,	623,	758,	20,		1},
	{220,	759,	894,	24,		1},
	{300,	351,	950,	31,		0},		//�m�[�}���G�̏I�[
//	{312,	351,	486,	31,		0},		//�u���[���p
	{(FACTORY_LEADER_TR_INDEX_1ST+1),	351,	486,	12,		0},		//�u���[���p
	{(FACTORY_LEADER_TR_INDEX_2ND+1),	351,	486,	31,		0},		//�u���[���p
};

///�I�[�v�����x���̓G���g�p���Ă���|�P�����f�[�^��Index�擾�͈�(�|�P������1�I���W��)
static const FACTORY_POKE_RANGE OpenLevelEnemyPokemonRange[] = {
	{100,	351,	486,	0,		1},
	{120,	487,	622,	4,		1},
	{140,	623,	758,	8,		1},
	{160,	759,	894,	12,		1},
	{180,	351,	950,	16,		1},
	{200,	351,	950,	20,		1},
	{220,	351,	950,	24,		1},
	{300,	351,	950,	31,		0},		//�m�[�}���G�̏I�[
//	{312,	759,	950,	31,		0},		//�u���[���p
	{(FACTORY_LEADER_TR_INDEX_1ST+1),	759,	950,	12,		0},		//�u���[���p
	{(FACTORY_LEADER_TR_INDEX_2ND+1),	759,	950,	31,		0},		//�u���[���p
};

///�I�[�v�����x���̖��������^���|�P�����f�[�^��Index�擾�͈�(�|�P������1�I���W��)
static const FACTORY_POKE_RANGE OpenLevelMinePokemonRange[] = {
	{100,	351,	486,	0,		1},
	{120,	487,	622,	4,		1},
	{140,	623,	758,	8,		1},
	{160,	759,	894,	12,		1},
	{180,	623,	950,	16,		1},
	{200,	623,	950,	20,		1},
	{220,	623,	950,	24,		1},
	{300,	351,	950,	31,		0},		//�m�[�}���G�̏I�[
//	{312,	759,	950,	31,		0},		//�u���[���p
	{(FACTORY_LEADER_TR_INDEX_1ST+1),	759,	950,	12,		0},		//�u���[���p
	{(FACTORY_LEADER_TR_INDEX_2ND+1),	759,	950,	31,		0},		//�u���[���p
};

///���x��50�Ŏ������g�p�o����|�P�����f�[�^��Index�擾�͈�(����G�Ɣ͈͈͂ꏏ)
#define Level50MinePokemonRange		Level50EnemyPokemonRange

/******************************************************************************/
//31���e�[�u���ɕ�������ƃ_���Ȃ̂Œ��ӁI(FACTORY_POKE_RANGE�Ŕ�r���Ă��悳����)
/******************************************************************************/
//�����񐔂ɂ����1��̃��x���𒊏o���鎞�Ɏg�p���Ă���
#define LV50_ENEMY_POKE_RANGE_MAX	( NELEMS(Level50MinePokemonRange) )	//�ő吔
//#define MINE_POKE_RANGE_END		( Level50MinePokemonRange[LV50_ENEMY_POKE_RANGE_MAX-1].end )

//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static int Factory_EnemyTrainerIndexGet( u8 type, int lap, int enemy_number);
const FACTORY_POKE_RANGE *Factory_EnemyPokemonRangeGet(int trainer_index, 
	int factory_level);
const FACTORY_POKE_RANGE *Factory_MinePokemonRangeGet(int lap, int factory_level);
static int Factory_EnemyPokemonIndexGet(B_TOWER_TRAINER_ROM_DATA *trd, int trainer_index, 
	int factory_level);
void Factory_EnemyLapAllTrainerIndexGet( u8 type, int lap, u16 trainer_index[], u8 num );


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �o������G�g���[�i�[�̃g���[�i�[�f�[�^No���擾����
 *
 * @param   lap					�T��(0�`)
 * @param   enemy_number		���Ԗڂɓo�ꂷ��G��(�O�`FACTORY_LAP_ENEMY_MAX-1)
 *
 * @retval  �g���[�i�[�f�[�^No
 *
 * Lv50�A�I�[�v�����ʂł�
 */
//--------------------------------------------------------------
static int Factory_EnemyTrainerIndexGet( u8 type, int lap, int enemy_number)
{
	int trainer_index, offset, start, check_num;
	
	if(lap >= NELEMS(TrainerSelectRange)){
		lap = NELEMS(TrainerSelectRange) - 1;
	}
	
	//�u���[��(�V���O���̂�)
	if( type == FACTORY_TYPE_SINGLE ){
		check_num = (lap * FACTORY_LAP_ENEMY_MAX) + (enemy_number + 1);
		if( check_num == FACTORY_LEADER_SET_1ST ){
			return FACTORY_LEADER_TR_INDEX_1ST;
		}else if( check_num == FACTORY_LEADER_SET_2ND ){
			return FACTORY_LEADER_TR_INDEX_2ND;
		}
	}

	if(enemy_number == FACTORY_LAP_ENEMY_MAX-1){
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
 * @param   trainer_index		�g���[�i�[Index�����(FACTORY_LAP_ENEMY_MAX���̗v�f�����K�v)
 */
//--------------------------------------------------------------
void Factory_EnemyLapAllTrainerIndexGet( u8 type, int lap, u16 trainer_index[], u8 num )
{
	int set_count = 0;
	int i;
	
	do{
		trainer_index[set_count] = Factory_EnemyTrainerIndexGet( type, lap, set_count);
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

//--------------------------------------------------------------
/**
 * @brief   �G���g�p���Ă���|�P�����f�[�^�̒��o�͈͂��擾����
 *
 * @param   trainer_index		�g���[�i�[Index
 * @param   factory_level		FACTORY_LEVEL_???
 *
 * @retval  ���o�͈̓f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
const FACTORY_POKE_RANGE *Factory_EnemyPokemonRangeGet(int trainer_index, int factory_level)
{
	int tbl_no;
	static const FACTORY_POKE_RANGE *range_tbl;
	int range_num;
	
	if(factory_level == FACTORY_LEVEL_50){
		range_tbl = Level50EnemyPokemonRange;
		range_num = NELEMS(Level50EnemyPokemonRange);
	}
	else{
		range_tbl = OpenLevelEnemyPokemonRange;
		range_num = NELEMS(OpenLevelEnemyPokemonRange);
	}
	
	for(tbl_no = 0; tbl_no < range_num; tbl_no++){
		if(trainer_index < range_tbl[tbl_no].trainer_index){
			break;
		}
	}
	if(tbl_no >= range_num){
		GF_ASSERT(0);
		tbl_no = range_num - 1;
	}
	
	return &range_tbl[tbl_no];
}

//--------------------------------------------------------------
/**
 * @brief   �ŏ��Ƀ����^������6�̂̃|�P�����f�[�^�̒��o�͈͂��擾����
 *
 * @param   lap					����
 * @param   factory_level		FACTORY_LEVEL_???
 *
 * @retval  ���o�͈̓f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
const FACTORY_POKE_RANGE *Factory_MinePokemonRangeGet(int lap, int factory_level)
{
	//���Ń����^���|�P�����̒��o���u���[���̃f�[�^�Ɉ���������Ȃ�
	//�f�[�^���ő����(8)�܂ł����p�ӂ���Ă��Ȃ��̂Ő�������
	if(lap >= FACTORY_LAP_MAX){
		lap = FACTORY_LAP_MAX - 1;
	}
	
	if(factory_level == FACTORY_LEVEL_50){
		return &Level50MinePokemonRange[lap];
	}
	else{
		return &OpenLevelMinePokemonRange[lap];
	}
}

//--------------------------------------------------------------
/**
 * @brief   �G�g���[�i�[���g���Ă���|�P����Index���擾
 *
 * @param   trd					�g���[�i�[�f�[�^
 * @param   trainer_index		�g���[�i�[Index
 * @param   factory_level		FACTORY_LEVEL_???
 *
 * @retval  �|�P�����f�[�^Index
 */
//--------------------------------------------------------------
static int Factory_EnemyPokemonIndexGet(B_TOWER_TRAINER_ROM_DATA *trd, int trainer_index, 
	int factory_level)
{
	int poke_index;
	
	if(factory_level == FACTORY_LEVEL_50){
		poke_index = trd->use_poke_table[gf_rand() % trd->use_poke_cnt];
	}
	else{
		//�I�[�v�����x���̓g���[�i�[�f�[�^�ɓ����Ă���莝���|�P�����ꗗ����ł͂Ȃ�
		//�g���[�i�[�ԍ����L�[�Ɉ��͈͂̃|�P�������烉���_���Œ��o
		int tbl_no, offset;
		
		for(tbl_no = 0; tbl_no < NELEMS(OpenLevelEnemyPokemonRange); tbl_no++){
			if(trainer_index < OpenLevelEnemyPokemonRange[tbl_no].trainer_index){
				break;
			}
		}
		if(tbl_no == NELEMS(OpenLevelEnemyPokemonRange)){
			GF_ASSERT(0);
			tbl_no = NELEMS(OpenLevelEnemyPokemonRange) - 1;
		}
		
		offset = OpenLevelEnemyPokemonRange[tbl_no].end - OpenLevelEnemyPokemonRange[tbl_no].start;
		poke_index = OpenLevelEnemyPokemonRange[tbl_no].start + (gf_rand() % offset);
	}
	
	return poke_index;
}

//�����񐔃e�[�u��
static const trade_count_tbl[] = {
	//��  //�������ɂȂ�|�P������(���񐔂ł͂Ȃ��I)
	6,		//0
	13,		//1
	20,		//2
	27,		//3
	34,		//4
	//35�ȏ�//5
};
#define TRADE_COUNT_TBL_MAX		( NELEMS(trade_count_tbl) )


//--------------------------------------------------------------
/**
 * @brief   �t�@�N�g���[�Ŏg�p����|�P������Index�����߂�
 *			�����|�P�����A��������ɂȂ�Ȃ��悤��Index���擾���܂�
 *
 * @param   check_pokeno[]		���h�~�Ń`�F�b�N����|�P�����ԍ�(Index�ł͂Ȃ����ɒ���)
 * @param   check_itemno[]		���h�~�Ń`�F�b�N����A�C�e���ԍ�
 * @param   check_num			check_pokeno��check_itemno�̃f�[�^��
 * @param   get_count			���̂̃|�P����Index���擾���邩
 * @param   ret_pokeindex[]		���܂����|�P����Index�̌��ʑ����(get_count���̔z�񐔂����鎖!)
 * @param   heap_id				�q�[�vID
 * @param   range				�|�P����Index���o�͈�
 * @param   trade_count			������(0�ȊO��n���ƃ����^���|�P�������o�ɂȂ�܂�)
 * @param   pow_rnd				�p���[����(0-31)
 *
 * @retval	FALSE	���I���[�v��50��ȓ��ŏI�����
 * @retval	TRUE	���I���[�v��50��ȓ��ŏI���Ȃ�����
 *
 * ���h�~���g�p���Ȃ��ꍇ��check_num��0�ɂ��Ă��������B(check_pokeno, check_itemno��NULL)
 */
//--------------------------------------------------------------
BOOL Factory_PokemonIndexCreate(const u16 check_pokeno[], const u16 check_itemno[], 
	int check_num, int get_count, u16 get_pokeindex[], int heap_id, const FACTORY_POKE_RANGE *poke_range, u16 trade_count, u8 get_pow_rnd[] )
{
	u8 set_pow_rnd;
	int set_count, range_offset, set_index;
	B_TOWER_POKEMON_ROM_DATA prd_d[6];
	int i,range_up;
	const FACTORY_POKE_RANGE* range;
	
	//���
	range = poke_range;

	GF_ASSERT(get_count <= 6);	//prd_d�̔z�񐔂𒴂��Ă����ASSERT
	
	range_offset = range->end - range->start;
	set_count = 0;
	range_up = 0;
	
	//�����񐔂�0�ȊO���n���ꂽ���́A�����^���|�P�������o�Ɣ��f����I
	if( trade_count != 0 ){

		//1��̃����W����I�o����|�P������
		range_up = TRADE_COUNT_TBL_MAX;

		//�����񐔂Ɣ�r���āA1��̃����W����I�o����|�P������������
		for( i=0; i < TRADE_COUNT_TBL_MAX; i++ ){
			if( trade_count <= trade_count_tbl[i] ){
				range_up = i;
				break;
			}
		}
	}

	while(set_count != get_count){
		//�����^���񐔂��l��
		//
		//��  get_count = �Z�b�g���鐔
		//    set_count = �Z�b�g������
		//    range_up  = 1����Z�b�g���鐔
		//    range_up���鐔�܂ł͒ʏ�I�o�A���̌��1��̃����N����I�o
		if( (set_count >= get_count - range_up) && (range->rankup_flag == 1) ){

			range_offset = (range+1)->end - (range+1)->start;		//���o�͈�1��
			set_index = (range+1)->end - (gf_rand() % (range_offset+1));
			set_pow_rnd = (range+1)->pow_rnd;

			//��  start       = 251
			//  �@end         = 350 
			//    range_offset= 350 - 251 + 1 = 100
			//    rand        = 0_99
			//    set_index   = 350 - 99 = 251_350
			//    rankup_flag = 1 //�u���[���ƒʏ�TR�̍ŋ��̎��́A1��̃f�[�^�����Ȃ��I
			//                               //�e�[�u���I�[�o�[���Ȃ��悤�ɂ���
		}else{
			set_index = range->end - (gf_rand() % (range_offset+1));
			set_pow_rnd = range->pow_rnd;

			//��  start       = 151
			//  �@end         = 250 
			//    range_offset= 250 - 151 + 1 = 100
			//    rand        = 0_99
			//    set_index   = 250 - 99 = 151_250
		}

		//�o�g���^���[�|�P���������f�[�^�̓ǂݏo��
		Frontier_PokemonRomDataGet(&prd_d[set_count], set_index, ARC_PL_BTD_PM);
		
		//���܂Ő������Ă����|�P�����Ɣ���Ă��Ȃ����`�F�b�N
		for(i = 0; i < set_count; i++){
			if(prd_d[i].mons_no == prd_d[set_count].mons_no 
					|| prd_d[i].item_no == prd_d[set_count].item_no){
				break;	//�����
			}
		}
		if(i != set_count){
			continue;
		}
		
		//�`�F�b�N�p�ɓn����Ă��郂���X�^�[�i���o�[�A�����A�C�e���̔��`�F�b�N
		for(i = 0; i < check_num; i++){
			if(prd_d[set_count].mons_no == check_pokeno[i] 
					|| prd_d[set_count].item_no ==  check_itemno[i]){
				break;	//�����
			}
		}
		if(i != check_num){
			continue;
		}

		//OS_Printf( "�Z�b�g����poke index = %d\n", set_index );
		OS_Printf( "�Z�b�g����pow rnd = %d\n", set_pow_rnd );
		get_pokeindex[set_count] = set_index;
		get_pow_rnd[set_count] = set_pow_rnd;
		set_count++;
	}
	
	return FALSE;
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
u8 Factory_GetMinePokeNum( u8 type );
u8 Factory_GetEnemyPokeNum( u8 type, BOOL flag );
void Factory_RentalPokeMake( u16 lap, u8 level, u16* rental_poke_index, B_TOWER_POKEMON* rental_poke, u8* pow_rnd, u32* personal_rnd, u16 trade_count, u16* poke_check_tbl, u16* item_check_tbl );
void Factory_EnemyPokeMake( u8 set_num, u16 tr_index, u8 level, B_TOWER_POKEMON* rental_poke, u16* enemy_poke_index, B_TOWER_POKEMON* enemy_poke, u8* pow_rnd, u32* personal_rnd, int check_count );
BATTLE_PARAM* BtlFactory_CreateBattleParam( FACTORY_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param );
static u32 Factory_GetFightType( u8 type );
u8 Factory_GetLevel( FACTORY_SCRWORK* wk );
void Factory_RentalPokeMake2( FACTORY_SCRWORK* wk );
void Factory_PairPokeMake( FACTORY_SCRWORK* wk );
BOOL Factory_CommCheck( u8 type );
static u16 Factory_GetTrAI( FACTORY_SCRWORK* wk );
u16 FactoryScr_CommGetLap( FACTORY_SCRWORK* wk );


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
u8 Factory_GetMinePokeNum( u8 type )
{
	switch( type ){

	case FACTORY_TYPE_SINGLE:
	case FACTORY_TYPE_DOUBLE:
		return 3;

	case FACTORY_TYPE_MULTI:
	case FACTORY_TYPE_WIFI_MULTI:
		return 2;
	};

	//�G���[
	return 0;
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
u8 Factory_GetEnemyPokeNum( u8 type, BOOL flag )
{
	switch( type ){

	case FACTORY_TYPE_SINGLE:
	case FACTORY_TYPE_DOUBLE:
		return 3;

	case FACTORY_TYPE_MULTI:
	case FACTORY_TYPE_WIFI_MULTI:
		if( flag == FACTORY_FLAG_SOLO ){
			return 2;								//2
		}else{
			return 4;								//2+2=4
		}
	};

	//�G���[
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�����������^���o����|�P����6�C�����߂�
 *
 * @param	lap
 * @param	level
 * @param	rental_poke_index
 * @param	rental_poke
 * @param   pow_rnd
 * @param	personal_rnd
 * @param	trade_count
 *
 * @return	
 *
 * �ȉ��̏��ŏ������Ă���
 * �����^��6
 * (���`�F�b�N:�����^��6) �G3
 * (���`�F�b�N:�莝��3)(���`�F�b�N:�O�̓G3) �G3
 * ...
 */
//--------------------------------------------------------------
void Factory_RentalPokeMake( u16 lap, u8 level, u16* rental_poke_index, B_TOWER_POKEMON* rental_poke, u8 *pow_rnd, u32* personal_rnd, u16 trade_count, u16* poke_check_tbl, u16* item_check_tbl )
{
	const FACTORY_POKE_RANGE* poke_range;
	int i;
	
	//OS_Printf( "lap = %d\n", lap );
	poke_range = Factory_MinePokemonRangeGet( lap, level );

#if 0
	//�p���[�������
	for( i=0; i < FACTORY_RENTAL_POKE_MAX; i++ ){
		pow_rnd[i] = poke_range->pow_rnd;
	}
#endif
	
	//�ʐM�Ȃ��A���`�F�b�N�̕K�v�͂Ȃ��̂�NULL��n��
	//�ʐM����A�e�@�̃����^���Œ��o���ꂽ�|�P���������O����6�C���q�@�p�ɑI��

	//���|�P�A�A�C�e���A�f�[�^���A�擾���鐔�A�����
	if( poke_check_tbl == NULL ){
		Factory_PokemonIndexCreate( NULL, NULL, 0,
									FACTORY_RENTAL_POKE_MAX, rental_poke_index, 
									HEAPID_WORLD, poke_range, trade_count, pow_rnd );//���chk����
	}else{

		OS_Printf( "poke_check_tbl[0] = %d\n", poke_check_tbl[0] );
		OS_Printf( "poke_check_tbl[1] = %d\n", poke_check_tbl[1] );
		OS_Printf( "item_check_tbl[0] = %d\n", item_check_tbl[0] );
		OS_Printf( "item_check_tbl[1] = %d\n", item_check_tbl[1] );

		Factory_PokemonIndexCreate( poke_check_tbl, poke_check_tbl, FACTORY_RENTAL_POKE_MAX,
									FACTORY_RENTAL_POKE_MAX, rental_poke_index, 
									HEAPID_WORLD, poke_range, trade_count, pow_rnd );//���chk�L��
	}

	//0������Ƃ��������Ȃ邩�̃`�F�b�N
	//for( i=0; i < 6 ;i++ ){
	//	rental_poke_index[i] = 0;
	//}

	//�����^���|�P��������x�ɐ���(personal�Ȃ�)
	Frontier_PokemonParamCreateAll(	rental_poke, rental_poke_index, pow_rnd,
									NULL, personal_rnd, FACTORY_RENTAL_POKE_MAX, HEAPID_WORLD,
									ARC_PL_BTD_PM );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�G���o���ŏ��̃|�P����3(4)�C�����߂�
 *
 * @param	set_num
 * @param	tr_index
 * @param	level
 * @param	enemy_poke_index
 * @param	rental_poke
 * @param	enemy_poke
 * @param	personal_rnd
 * @param	check_count			���`�F�b�N���鐔
 *
 * @return	
 *
 * �ȉ��̏��ŏ������Ă���
 * �����^��6
 * (���`�F�b�N:�����^��6) �G3
 * (���`�F�b�N:�莝��3)(���`�F�b�N:�O�̓G3) �G3
 * ...
 */
//--------------------------------------------------------------
void Factory_EnemyPokeMake( u8 set_num, u16 tr_index, u8 level, B_TOWER_POKEMON* rental_poke, u16* enemy_poke_index, B_TOWER_POKEMON* enemy_poke, u8* pow_rnd, u32* personal_rnd, int check_count )
{
	int i;
	B_TOWER_POKEMON poke;
	const FACTORY_POKE_RANGE* poke_range;
	u16 poke_check_tbl[FACTORY_RENTAL_POKE_MAX*2];
	u16 item_check_tbl[FACTORY_RENTAL_POKE_MAX*2];

	poke_range = Factory_EnemyPokemonRangeGet( tr_index, level );

#if 0
	//�p���[�������
	for( i=0; i < set_num; i++ ){
		pow_rnd[i] = poke_range->pow_rnd;
	}
#endif
	
	//�`�F�b�N�e�[�u���쐬
	//for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){
	for( i=0; i < check_count ;i++ ){
		poke = *(B_TOWER_POKEMON*)(&rental_poke[i]);
		poke_check_tbl[i] = poke.mons_no;
		item_check_tbl[i] = poke.item_no;
	}

	//�G�|�P�����̃C���f�b�N�X�擾(���|�P�A�A�C�e���A�f�[�^���A�擾���鐔�A�����)
	//Factory_PokemonIndexCreate( poke_check_tbl, item_check_tbl, FACTORY_RENTAL_POKE_MAX,
	Factory_PokemonIndexCreate( poke_check_tbl, item_check_tbl, check_count,
								set_num, enemy_poke_index, 
								HEAPID_WORLD, poke_range, 0, pow_rnd );

	//�G�|�P��������x�ɐ���(personal�Ȃ�)
	Frontier_PokemonParamCreateAll(	enemy_poke, enemy_poke_index, 
									pow_rnd, NULL, personal_rnd, set_num, HEAPID_WORLD,
									ARC_PL_BTD_PM );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�o�g���t�@�N�g���[�p�@�o�g���p�����[�^����
 */
//--------------------------------------------------------------
BATTLE_PARAM* BtlFactory_CreateBattleParam( FACTORY_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param )
{
	int i;
	u32	val32;
	u8 m_max,e_max;
	BATTLE_PARAM* bp;
	POKEMON_PARAM* pp;
	B_TOWER_TRAINER bt_trd;
	B_TOWER_TRAINER_ROM_DATA* p_rom_tr;
	MSGDATA_MANAGER* man;
	STRBUF* msg;
	MYSTATUS* my;

	//�Q���|�P���������擾
	m_max = Factory_GetMinePokeNum( wk->type );
	e_max = Factory_GetEnemyPokeNum( wk->type, FACTORY_FLAG_SOLO );

	//�񕜏���
	PokeParty_RecoverAll( wk->p_m_party );
	PokeParty_RecoverAll( wk->p_e_party );

	//�퓬�p�����[�^�̐�������(field_battle.c)(fssc_factory.c FSSC_FactoryCallGetResult��Delete)
	bp = BattleParam_Create( HEAPID_WORLD, Factory_GetFightType(wk->type) );
	
	//�퓬�p�����[�^�̎��W�R�A(�t�B�[���h)
	BattleParam_SetParamByGameDataCore( bp, NULL, 
										ex_param->savedata,
										ex_param->zone_id,
										ex_param->fnote_data,
										ex_param->bag_cursor,
										ex_param->battle_cursor);

	//����͌Œ�I
	bp->bg_id		= BG_ID_FACTORY;		//��{�w�i�w��
	bp->ground_id	= GROUND_ID_FACTORY;	//��{�n�ʎw��

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//MINE�F�I�񂾎莝���|�P�������Z�b�g
	PokeParty_Init( bp->poke_party[POKEPARTY_MINE], m_max );

	//MINE�F�����̃|�P�����f�[�^�Z�b�g
	pp = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < m_max ;i++ ){

		//POKEMON_PARAM�\���̂���POKEMON_PARAM�\���̂փR�s�[
		PokeCopyPPtoPP( PokeParty_GetMemberPointer(wk->p_m_party,i), pp );

		//�퓬�p�����[�^�Z�b�g:�|�P������������
		BattleParam_AddPokemon( bp, pp, POKEPARTY_MINE );
		OS_Printf( "MINE1 power_rnd = %d\n", PokeParaGet(pp,ID_PARA_power_rnd,NULL) );
		OS_Printf( "MINE1 power_rnd = %d\n", PokeParaGet(pp,ID_PARA_power_rnd,NULL) );
		OS_Printf( "MINE1 pow_rnd = %d\n", PokeParaGet(pp,ID_PARA_pow_rnd,NULL) );
		OS_Printf( "MINE1 def_rnd = %d\n", PokeParaGet(pp,ID_PARA_def_rnd,NULL) );
		OS_Printf( "MINE1 spedef_rnd = %d\n", PokeParaGet(pp,ID_PARA_spedef_rnd,NULL) );
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
					Factory_GetEnemyPokeNum(wk->type,FACTORY_FLAG_SOLO) );

	//AI�Z�b�g
	for( i=0; i < CLIENT_MAX ;i++ ){
		bp->trainer_data[i].aibit = Factory_GetTrAI( wk );
	}
	OS_Printf( "�g���[�i�[��AI = %d\n", Factory_GetTrAI(wk) );

	//ENEMY1�F�퓬�p�����[�^�Z�b�g:�|�P������������
	pp = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < e_max ;i++ ){

		//POKEMON_PARAM�\���̂���POKEMON_PARAM�\���̂փR�s�[
		PokeCopyPPtoPP( PokeParty_GetMemberPointer(wk->p_e_party,i), pp );
		BattleParam_AddPokemon( bp, pp, POKEPARTY_ENEMY );
		OS_Printf( "ENEMY1 power_rnd = %d\n", PokeParaGet(pp,ID_PARA_power_rnd,NULL) );
		OS_Printf( "ENEMY1 pow_rnd = %d\n", PokeParaGet(pp,ID_PARA_pow_rnd,NULL) );
		OS_Printf( "ENEMY1 def_rnd = %d\n", PokeParaGet(pp,ID_PARA_def_rnd,NULL) );
		OS_Printf( "ENEMY1 spedef_rnd = %d\n", PokeParaGet(pp,ID_PARA_spedef_rnd,NULL) );
	}
	sys_FreeMemoryEz( pp );

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�f�o�b�N���\��
//OS_Printf("p_party count = %d\n", PokeParty_GetPokeCount(wk->p_party) );
//OS_Printf("bp_party[mine]count= %d\n",PokeParty_GetPokeCount(bp->poke_party[POKEPARTY_MINE]));
//OS_Printf("bp_party[enemy]count= %d\n",PokeParty_GetPokeCount(bp->poke_party[POKEPARTY_ENEMY]));

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	switch( wk->type ){

	case FACTORY_TYPE_MULTI:
	case FACTORY_TYPE_WIFI_MULTI:

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
		p_rom_tr = Frontier_TrainerDataGet( &bt_trd, wk->tr_index[wk->round+FACTORY_LAP_ENEMY_MAX],
											HEAPID_WORLD, ARC_PL_BTD_TR );
		sys_FreeMemoryEz( p_rom_tr );

		//ENEMY2�F�g���[�i�[�f�[�^���Z�b�g
		BattleParamTrainerDataSet( bp, &bt_trd, e_max, CLIENT_NO_ENEMY2, HEAPID_WORLD );

		//ENEMY2�F�I�񂾎莝���|�P�������Z�b�g
		PokeParty_Init( bp->poke_party[POKEPARTY_ENEMY_PAIR], 
						Factory_GetEnemyPokeNum(wk->type,FACTORY_FLAG_SOLO) );

		//ENEMY2�F�퓬�p�����[�^�Z�b�g:�|�P������������
		pp = PokemonParam_AllocWork( HEAPID_WORLD );
		for( i=0; i < e_max ;i++ ){

			//POKEMON_PARAM�\���̂���POKEMON_PARAM�\���̂փR�s�[
			PokeCopyPPtoPP( PokeParty_GetMemberPointer(wk->p_e_party,(e_max+i)), pp );
			BattleParam_AddPokemon( bp, pp, POKEPARTY_ENEMY_PAIR );
			OS_Printf( "ENEMY2 power_rnd = %d\n", PokeParaGet(pp,ID_PARA_power_rnd,NULL) );
			OS_Printf( "ENEMY2 pow_rnd = %d\n", PokeParaGet(pp,ID_PARA_pow_rnd,NULL) );
			OS_Printf( "ENEMY2 def_rnd = %d\n", PokeParaGet(pp,ID_PARA_def_rnd,NULL) );
			OS_Printf( "ENEMY2 spedef_rnd = %d\n", PokeParaGet(pp,ID_PARA_spedef_rnd,NULL) );
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
static u32 Factory_GetFightType( u8 type )
{
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//
	//�t�@�N�g���[�p��FIGHT_TYPE���쐬���邩�m�F����I
	//
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	switch( type ){

	case FACTORY_TYPE_SINGLE:
		return FIGHT_TYPE_BATTLE_TOWER_1vs1;

	case FACTORY_TYPE_DOUBLE:
		return FIGHT_TYPE_BATTLE_TOWER_2vs2;

	case FACTORY_TYPE_MULTI:
		return FIGHT_TYPE_BATTLE_TOWER_SIO_MULTI;	
		//return FIGHT_TYPE_MULTI_SIO;	

	case FACTORY_TYPE_WIFI_MULTI:
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
u8 Factory_GetLevel( FACTORY_SCRWORK* wk )
{
	if( wk->level == FACTORY_LEVEL_50 ){
		return 50;
	}

	return 100;
}

//--------------------------------------------------------------
/**
 * @brief	�����^�������f�[�^��M��A�����^���|�P��������x�ɐ���
 *
 * @param	wk
 *
 * @return	
 */
//--------------------------------------------------------------
void Factory_RentalPokeMake2( FACTORY_SCRWORK* wk )
{
	int i;
	POKEMON_PARAM* temp_poke;

	//�����^���|�P��������x�ɐ���(personal����)
	Frontier_PokemonParamCreateAll(	wk->rental_poke, wk->rental_poke_index, wk->rental_pow_rnd,
									wk->rental_personal_rnd, NULL,
									FACTORY_RENTAL_POKE_MAX, HEAPID_WORLD, 
									ARC_PL_BTD_PM );

	PokeParty_InitWork( wk->p_m_party );		//POKEPARTY��������

	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < FACTORY_RENTAL_POKE_MAX ;i++ ){			//�����^��6�C
		Frontier_PokeParaMake( &wk->rental_poke[i], temp_poke, Factory_GetLevel(wk) );
		Frontier_PokePartyAdd( wk->sv, wk->p_m_party, temp_poke );
	}
	sys_FreeMemoryEz( temp_poke );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�莝���|�P�����f�[�^��M��A�p�[�g�i�[�̎莝���|�P��������x�ɐ���
 *
 * @param	wk
 *
 * @return	
 */
//--------------------------------------------------------------
void Factory_PairPokeMake( FACTORY_SCRWORK* wk )
{
	int i,poke_count;
	POKEMON_PARAM* temp_poke;

	//[2,3]	(���肪�����^���őI��2��)
	B_TOWER_POKEMON bt_poke[FACTORY_COMM_POKE_NUM];

	//�|�P�������擾
	poke_count = PokeParty_GetPokeCount( wk->p_m_party );

	//[2,3]�����łɎg���Ă�����폜
	for( i=poke_count; i > FACTORY_COMM_POKE_NUM ;i-- ){ 
		PokeParty_Delete( wk->p_m_party, (i-1) );			//pos=0�I���W��
	}

	//�p�[�g�i�[�̎莝���|�P��������x�ɐ���(personal����)
	Frontier_PokemonParamCreateAll(	bt_poke, wk->pair_rental_poke_index, 
									wk->pair_rental_pow_rnd,
									wk->pair_rental_personal_rnd, NULL,
									FACTORY_COMM_POKE_NUM, HEAPID_WORLD, 
									ARC_PL_BTD_PM );

	temp_poke = PokemonParam_AllocWork( HEAPID_WORLD );
	for( i=0; i < FACTORY_COMM_POKE_NUM ;i++ ){

		//�o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
		Frontier_PokeParaMake( &bt_poke[i], temp_poke, Factory_GetLevel(wk) );
		Frontier_PokePartyAdd( wk->sv, wk->p_m_party, temp_poke );

		//�莝���Ƀ|�P�����̃C���f�b�N�X�i�[
		wk->temoti_poke_index[i+FACTORY_COMM_POKE_NUM] = wk->pair_rental_poke_index[i];
	}
	sys_FreeMemoryEz( temp_poke );

	OS_Printf( "�|�P�����p�[�e�B�� = %d\n",	PokeParty_GetPokeCount(wk->p_m_party) );
	return;
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
BOOL Factory_CommCheck( u8 type )
{
	switch( type ){
	case FACTORY_TYPE_MULTI:
	case FACTORY_TYPE_WIFI_MULTI:
		return TRUE;
	};

	return FALSE;

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
static u16 Factory_GetTrAI( FACTORY_SCRWORK* wk )
{
	u16 ai,lap;

#if 1
	//�u���[��
	if( wk->type == FACTORY_TYPE_SINGLE ){
		if( (wk->tr_index[wk->round] == FACTORY_LEADER_TR_INDEX_1ST) ||
			(wk->tr_index[wk->round] == FACTORY_LEADER_TR_INDEX_2ND) ){
			return FR_AI_EXPERT;
		}
	}
#endif

	//���񐔂��擾
	lap = FactoryScr_CommGetLap( wk );

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
 * @param	wk		FACTORY_SCRWORK�^�̃|�C���^
 *
 * @return	"����"
 */
//--------------------------------------------------------------
u16 FactoryScr_CommGetLap( FACTORY_SCRWORK* wk )
{
	u16	lap;
	
	lap = wk->lap;

	if( Factory_CommCheck(wk->type) == TRUE ){

		//�ʐM���ɂ͎��񐔂̑����ق��Œ��I
		if( wk->pair_lap > wk->lap ){
			lap = wk->pair_lap;
		}
	}

	return lap;
}



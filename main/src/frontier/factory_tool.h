//==============================================================================
/**
 * @file	factory_tool.h
 * @brief	�o�g���t�@�N�g���[�֘A�c�[����
 * @author	Satoshi Nohara
 * @date	2007.03.27
 *
 * ���c���񂩂���p��
 */
//==============================================================================
#ifndef __FACTORY_TOOL_H__
#define __FACTORY_TOOL_H__

#include "common.h"
#include "system/pms_data.h"
#include "savedata/b_tower.h"


//==============================================================================
//	�萔��`
//==============================================================================


//==============================================================================
//	�\���̒�`
//==============================================================================
///�����^���|�P�����̃f�[�^����̒��o�͈�
typedef struct{
	u16 trainer_index;	///<�g���[�i�[Index(����Index�����̃g���[�i�[���Ώ�)
	u16 start;			///<�|�P����Index�J�nNo
	u16 end;			///<�|�P����Index�I��No
	u8 pow_rnd;			///<�p���[����
	u8 rankup_flag;		///<�����N�A�b�v�\��
}FACTORY_POKE_RANGE;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void Factory_EnemyLapAllTrainerIndexGet( u8 type, int lap, u16 trainer_index[], u8 num );
extern u32 Factory_PokemonParamCreate(B_TOWER_POKEMON *pwd,
	u16 poke_index, int position, u8 pow_rnd, u32 personal_rnd, int heap_id);
extern void Factory_PokemonParamCreateAll(B_TOWER_POKEMON *pwd, u16 poke_index[], 
	u8 pow_rnd[], u32 personal_rnd[], u32 ret_personal_rnd[], int set_num, int heap_id);
extern const FACTORY_POKE_RANGE *Factory_EnemyPokemonRangeGet(int trainer_index, 
	int factory_level);
extern BOOL Factory_PokemonIndexCreate(const u16 check_pokeno[], const u16 check_itemno[], 
	int check_num, int get_count, u16 get_pokeindex[], int heap_id, const FACTORY_POKE_RANGE *range, u16 trade_count, u8 get_pow_rnd[] );
//extern const FACTORY_POKE_RANGE *Factory_MinePokemonRangeGet(int lap, int factory_level);


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
//	04.07.11�ǉ�
//
//==============================================================================
#include "battle/battle_common.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_tool.h"
#include "savedata/factory_savedata.h"		//FACTORYDATA
#include "factory_def.h"					//FACTORY_LAP_ENEMY_MAX
#include "frontier_types.h"
#include "frontier_main.h"


//============================================================================================
//
//	�o�g���t�@�N�g���[�@�X�N���v�g���[�N
//
//============================================================================================
typedef struct{

	int	heapID;											//�q�[�v

	u8	type;											//�V���O���A�_�u���A�}���`�Awifi�}���`
	u8	level;											//LV50�A�I�[�v��
	u8	round;											//�����l�ځH
	u8	mode;											//�����^���A����

	u16	trade_count;									//������
	u8	clear_flag;										//7�A��(�N���A)�������t���O(0��1)
	u8	brain_appear_msg_flag;							//�u���[���o�ꃁ�b�Z�[�W�\��������

	u16	rensyou;										//���݂̘A����
	u16	lap;											//���݂̎���

	u32	win_cnt;										//���݂܂ł̑�������
	int winlose_flag;									//���������t���O

	//�G�g���[�i�[�f�[�^
	u16 tr_index[FACTORY_LAP_MULTI_ENEMY_MAX];			//�g���[�i�[index�e�[�u��(*2=�_�u��)

	//�ΐ푊��f�[�^�i�[�ꏊ
	B_TOWER_PARTNER_DATA tr_data[2];

	//�����^���|�P�����f�[�^
	u16 rental_poke_index[FACTORY_RENTAL_POKE_MAX];		//�|�P����index�e�[�u��
	u8	rental_pow_rnd[FACTORY_RENTAL_POKE_MAX];		//�|�P�����̃p���[����
	u32 rental_personal_rnd[FACTORY_RENTAL_POKE_MAX];	//�|�P�����̌�����
	B_TOWER_POKEMON rental_poke[FACTORY_RENTAL_POKE_MAX];
	u16 dummy_rental_eom;								//�b�菈��(EOM��2byte�]�v�ɏ������܂�邽��)

	//�G�|�P�����f�[�^
	u16 enemy_poke_index[FACTORY_ENEMY_POKE_MAX];		//�|�P����index�e�[�u��
	u8	enemy_pow_rnd[FACTORY_ENEMY_POKE_MAX];			//�|�P�����̃p���[����
	u32 enemy_personal_rnd[FACTORY_ENEMY_POKE_MAX];		//�|�P�����̌�����
	B_TOWER_POKEMON enemy_poke[FACTORY_ENEMY_POKE_MAX];	//�|�P�����f�[�^
	u16 dummy_enemy_eom;								//�b�菈��(EOM��2byte�]�v�ɏ������܂�邽��)

	//�t�@�N�g���[��p��ʂ֓n���p�̃|�P�����f�[�^
	POKEPARTY* p_m_party;								//����
	POKEPARTY* p_e_party;								//�G
	u16 ret_work[FACTORY_RET_WORK_MAX];
	u16 temoti_poke_index[FACTORY_PARTY_POKE_MAX];		//�莝���ɂ����Ă���|�P����index�e�[�u��

	FACTORYDATA* factory_savedata;						//�t�@�N�g���[�Z�[�u�f�[�^�ւ̃|�C���^
	SAVEDATA* sv;
	void* p_work;

	TCB_PTR	rail_move_tcb;								//�}�b�v�̃��[������TCB
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	buf[FACTORY_COMM_BUF_LEN];

	//�ʐM�p�F��{���
	u8	pair_retire_flag;									//���^�C����I�񂾂�=1
	u8	pair_trade_yesno_flag;								//������������(0=�������A1=�������Ȃ�)
	u16	pair_lap;											//����

	u16	pair_trade_count;									//������
	u16	pair_rensyou;										//���݂̘A����(27�l�ڂƂ��̕\���ɕK�v)

	//�ʐM�p�F�����^���|�P�����f�[�^
	u16 pair_rental_poke_index[FACTORY_RENTAL_POKE_MAX];	//�|�P����index_tbl
	u8	pair_rental_pow_rnd[FACTORY_RENTAL_POKE_MAX];		//�|�P�����̃p���[����
	u32 pair_rental_personal_rnd[FACTORY_RENTAL_POKE_MAX];	//�|�P�����̌�����
	B_TOWER_POKEMON pair_rental_poke[FACTORY_RENTAL_POKE_MAX];
	u16 dummy_pair_rental_eom;							//�b�菈��(EOM��2byte�]�v�ɏ������܂�邽��)
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	
	//�ʐM�̑���M�o�b�t�@
	u8 recieve_count;										//��M�f�[�^���󂯎�����l�����J�E���g

	u32 dummy_work;
}FACTORY_SCRWORK;


//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern u8 Factory_GetMinePokeNum( u8 type );
extern u8 Factory_GetEnemyPokeNum( u8 type, BOOL flag );
extern void Factory_RentalPokeMake( u16 lap, u8 level, u16* rental_poke_index, B_TOWER_POKEMON* rental_poke, u8* pow_rnd, u32* personal_rnd, u16 trade_count, u16* poke_check_tbl, u16* item_check_tbl );
extern void Factory_EnemyPokeMake( u8 set_num, u16 tr_index, u8 level, B_TOWER_POKEMON* rental_poke, u16* enemy_poke_index, B_TOWER_POKEMON* enemy_poke, u8* pow_rnd, u32* personal_rnd, int check_count );
extern BATTLE_PARAM* BtlFactory_CreateBattleParam(FACTORY_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param);
extern u8 Factory_GetLevel( FACTORY_SCRWORK* wk );
extern void Factory_RentalPokeMake2( FACTORY_SCRWORK* wk );
extern void Factory_PairPokeMake( FACTORY_SCRWORK* wk );
extern BOOL Factory_CommCheck( u8 type );
extern u16 FactoryScr_CommGetLap( FACTORY_SCRWORK* wk );


#endif	//__FACTORY_TOOL_H__



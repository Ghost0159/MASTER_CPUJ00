//==============================================================================
/**
 * @file	stage_tool.h
 * @brief	�o�g���X�e�[�W�֘A�c�[����
 * @author	nohara
 * @date	07.06.07
 */
//==============================================================================
#ifndef __STAGE_TOOL_H__
#define __STAGE_TOOL_H__

#include "common.h"
#include "system/pms_data.h"
#include "savedata/b_tower.h"

#include "battle/battle_common.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_tool.h"
#include "savedata/stage_savedata.h"		//STAGEDATA
#include "stage_def.h"
#include "frontier_types.h"
#include "frontier_main.h"


//============================================================================================
//
//	�o�g���X�e�[�W�@�X�N���v�g���[�N
//
//============================================================================================
typedef struct{

	int	heapID;											//�q�[�v

	u8	type;											//�V���O���A�_�u���A�}���`�Awifi�}���`
	u8	round;											//�����l�ځH
	u8	clear_flag;										//7�A��(�N���A)�������t���O(0��1)
	u8	enemy_level;									//�G�|�P�����̃��x��

	u16	rensyou;										//���݂̘A����
	u16	lap;											//���݂̎���

	fx32 sqrt;											//������
	u32	win_cnt;										//���݂܂ł̑�������
	int winlose_flag;									//���������t���O

	//�G�g���[�i�[�f�[�^
	u16 tr_index[STAGE_LAP_MULTI_ENEMY_MAX];			//�g���[�i�[index�e�[�u��[18*2]

	//�ΐ푊��f�[�^�i�[�ꏊ
	B_TOWER_PARTNER_DATA tr_data[2];

	//�Q�����Ă���莝���|�P�����̈ʒu���
	u8 mine_poke_pos[STAGE_ENTRY_POKE_MAX];
	POKEPARTY* p_party;

	//�G�|�P�����f�[�^
	u16 enemy_poke_index[STAGE_LAP_MULTI_ENEMY_MAX];	//�|�P����index�e�[�u��
	B_TOWER_POKEMON enemy_poke[STAGE_LAP_MULTI_ENEMY_MAX];	//�|�P�����f�[�^
	u16 dummy_enemy_eom;								//�b�菈��(EOM��2byte�]�v�ɏ������܂�邽��)

	//�|�P�����̃^�C�v�I����ʂ֓n���p�̃|�P�����f�[�^
	u16 ret_work;

	u8 poke_type;										//���킷��|�P�����̃^�C�v
	u8 csr_pos;											//�I�������ʒu

	STAGEDATA* stage_savedata;							//�X�e�[�W�Z�[�u�f�[�^�ւ̃|�C���^
	SAVEDATA* sv;
	void* p_work;

	//�^�C�v���Ƃ̃��x��(�����N)
	u8 type_level[STAGE_TYPE_MAX][STAGE_TR_TYPE_MAX/2];	//[4][18/2]	4bit

	u16 itemno[STAGE_ENTRY_POKE_MAX];					//�����A�C�e��

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	buf[STAGE_COMM_BUF_LEN];
	u8	huge_buf[STAGE_HUGE_BUF_LEN];
	u8	recieve_huge_buf[STAGE_COMM_PLAYER_NUM][STAGE_HUGE_BUF_LEN];

	u16 pair_poke_level[STAGE_ENTRY_POKE_MAX];				//����̃|�P�����̃��x��

	//�ʐM�p�F��{���
	u8	pair_retire_flag;									//���^�C����I�񂾂�=1

	POKEMON_PARAM* pair_poke;

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	//�ʐM�̑���M�o�b�t�@
	u8 recieve_count;										//��M�f�[�^���󂯎�����l�����J�E���g

	u32 dummy_work;
}STAGE_SCRWORK;


//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern void Stage_CreateTrType( u8 csr_pos, u8 set_num, u8 rank, u8 round, u16 tr_index[] );
extern void Stage_CreateTrIndex( u8 type, u8 set_num, int lap, u8 rank, u8 round, u16 tr_index[] );
extern void Stage_CreatePokeIndex( u8 set_num, u8 poketype, u8 rank, u8 round, u16 monsno, u16 poke_index[], u8 brain_count );
extern u8 Stage_GetMinePokeNum( u8 type );
extern u8 Stage_GetEnemyPokeNum( u8 type );
extern BATTLE_PARAM* BtlStage_CreateBattleParam( STAGE_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param );
extern u32 Stage_PokemonParamCreate( B_TOWER_POKEMON *pwd, u16 poke_index, int position, u8 pow_rnd, u32 personal_rnd, int heap_id );
//extern void Stage_PokemonParamCreateAll( B_TOWER_POKEMON *pwd, u8 type_level, u16 poke_index[], int set_num, int heap_id, int arcPM );
extern u8 Stage_GetPokeType( u8 csr_pos );
extern BOOL Stage_CommCheck( u8 type );
extern u8 Stage_GetMineLevelRank( STAGE_SCRWORK* wk );
extern u16 StageScr_CommGetPokeLevel( STAGE_SCRWORK* wk );
extern fx32 StageScr_GetSqrt32( u32 param );


#endif	//__STAGE_TOOL_H__



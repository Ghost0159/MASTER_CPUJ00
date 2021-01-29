//==============================================================================
/**
 * @file	roulette_tool.h
 * @brief	�o�g�����[���b�g�֘A�c�[����
 * @author	nohara
 * @date	2007.09.06
 */
//==============================================================================
#ifndef __ROULETTE_TOOL_H__
#define __ROULETTE_TOOL_H__

#include "common.h"
#include "frontier_tool.h"


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
	
	u8 dummy;	///<4�o�C�g���E�I�t�Z�b�g
}ROULETTE_POKE_RANGE;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void Roulette_EnemyLapAllTrainerIndexGet( u8 type, int lap, u16 trainer_index[], u8 num );
extern u32 Roulette_PokemonParamCreate(B_TOWER_POKEMON *pwd,
	u16 poke_index, int position, u8 pow_rnd, u32 personal_rnd, int heap_id);
extern void Roulette_PokemonParamCreateAll(B_TOWER_POKEMON *pwd, u16 poke_index[], 
	u8 pow_rnd[], u32 personal_rnd[], u32 ret_personal_rnd[], int set_num, int heap_id);
extern const ROULETTE_POKE_RANGE *Roulette_EnemyPokemonRangeGet(int trainer_index, 
	int roulette_level);


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
#include "savedata/roulette_savedata.h"		//ROULETTEDATA
#include "roulette_def.h"					//ROULETTE_LAP_ENEMY_MAX
#include "frontier_types.h"
#include "frontier_main.h"


//============================================================================================
//
//	�o�g�����[���b�g�@�X�N���v�g���[�N
//
//============================================================================================
typedef struct{

	int	heapID;											//�q�[�v
	SAVEDATA* sv;
	ROULETTEDATA* roulette_savedata;					//���[���b�g�Z�[�u�f�[�^�ւ̃|�C���^
	void* p_work;

	u8	type;											//�V���O���A�_�u���A�}���`�Awifi�}���`
	u8	round;											//�����l�ځH
	u8	random_flag;									//�J�[�\�������_���ړ�on�t���O
	u8	decide_ev_no;									//���肵���C�x���g�i���o�[

	int	weather;										//�V��

	u16	rensyou;										//���݂̘A����
	u16	lap;											//���݂̎���

	u8	csr_speed_level;								//�J�[�\���X�s�[�h
	u8	point;											//�퓬���ʃ|�C���g
	u8	brain_appear_msg_flag;							//�u���[���o�ꃁ�b�Z�[�W�\��������
	u8	poke_safe_flag;									//�N���C�x���g�̌��ʂ𔭊����Ȃ�����

	u16	rand_pos;										//���ʂ̃����_��
	u16 dummy77;

	u32	win_cnt;										//���݂܂ł̑�������
	int winlose_flag;									//���������t���O

	//�Q�����Ă���莝���|�P�����̈ʒu���
	u8	mine_poke_pos[ROULETTE_ENTRY_POKE_MAX];
	u8	clear_flag;										//7�A��(�N���A)�������t���O(0��1)

	//�A�C�e���A�C�R��
	CATS_ACT_PTR m_pokeicon[ROULETTE_MINE_POKE_MAX];
	CATS_ACT_PTR e_pokeicon[ROULETTE_ENEMY_POKE_MAX];

	//�A�C�e�������Ă��邩�A�C�R��
	CATS_ACT_PTR m_itemkeep[ROULETTE_MINE_POKE_MAX];
	CATS_ACT_PTR e_itemkeep[ROULETTE_ENEMY_POKE_MAX];

	//���[���b�g��p��ʂ֓n���p�̃|�P�����f�[�^
	POKEPARTY* p_m_party;								//����
	POKEPARTY* p_e_party;								//�G

	//�G�g���[�i�[�f�[�^
	u16 tr_index[ROULETTE_LAP_MULTI_ENEMY_MAX];			//�g���[�i�[index�e�[�u��(*2=�_�u��)

	u32	temp_hp[ROULETTE_MINE_POKE_MAX];
	u32	temp_pow[ROULETTE_MINE_POKE_MAX];
	u32	temp_def[ROULETTE_MINE_POKE_MAX];
	u32	temp_agi[ROULETTE_MINE_POKE_MAX];
	u32	temp_spepow[ROULETTE_MINE_POKE_MAX];
	u32	temp_spedef[ROULETTE_MINE_POKE_MAX];

	//�ΐ푊��f�[�^�i�[�ꏊ
	B_TOWER_PARTNER_DATA tr_data[2];

	//�G�|�P�����f�[�^
	u16 enemy_poke_index[ROULETTE_ENEMY_POKE_MAX];		//�|�P����index�e�[�u��
	u8	enemy_pow_rnd[ROULETTE_ENEMY_POKE_MAX];			//�|�P�����̃p���[����
	u32 enemy_personal_rnd[ROULETTE_ENEMY_POKE_MAX];	//�|�P�����̌�����
	B_TOWER_POKEMON enemy_poke[ROULETTE_ENEMY_POKE_MAX];//�|�P�����f�[�^
	u16 dummy_enemy_eom;								//�b�菈��(EOM��2byte�]�v�ɏ������܂�邽��)

	u16 itemno[ROULETTE_ENTRY_POKE_MAX];				//�莝���|�P�����̏����A�C�e���ۑ��p

	u16 ret_work[ROULETTE_RET_WORK_MAX];

	//�A�C�e�������Ă���A�C�R����\������t���O
	//u16 itemkeep_draw_flag[ROULETTE_ENTRY_POKE_MAX];

	//�莝���|�P����
	//u16 item[ROULETTE_MINE_POKE_MAX];					//����

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	send_buf[ROULETTE_COMM_BUF_LEN];
	u8	huge_buf[ROULETTE_HUGE_BUF_LEN];
	u8	recieve_huge_buf[ROULETTE_COMM_PLAYER_NUM][ROULETTE_HUGE_BUF_LEN];

	//�ʐM�p�F��{���
	u8	pair_retire_flag;								//���^�C����I�񂾂�=1
	u8	pair_trade_yesno_flag;							//������������(0=�������A1=�������Ȃ�)
	u16	pair_lap;										//����
	u16	pair_rensyou;									//���݂̘A����(27�l�ڂƂ��̕\���ɕK�v)
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	u8	reverce_flag;									//�p�[�e�B�N�����]�t���O
	u8	pair_sel_type;
	u8	recieve_count;
	u8	dummy22;

	u16* list_del_work;

	u32	dummy_work;
}ROULETTE_SCRWORK;


//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern u8 Roulette_GetMinePokeNum( u8 type, BOOL flag );
extern u8 Roulette_GetEnemyPokeNum( u8 type, BOOL flag );
extern void Roulette_EnemyPokeMake( u8 set_num, u16 tr_index, u8 level, B_TOWER_POKEMON* rental_poke, u16* enemy_poke_index, B_TOWER_POKEMON* enemy_poke, u8* pow_rnd, u32* personal_rnd );
extern BATTLE_PARAM* BtlRoulette_CreateBattleParam(ROULETTE_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param);
extern u8 Roulette_GetLevel( ROULETTE_SCRWORK* wk );
extern BOOL Roulette_CommCheck( u8 type );
extern void Roulette_EnemyPartySet( ROULETTE_SCRWORK* wk );
extern void Roulette_PokePartyRecoverAll( POKEPARTY* party );
extern void Roulette_BirthInfoSet( ROULETTE_SCRWORK* wk, POKEMON_PARAM* poke );
extern void Roulette_PokePartyAdd( ROULETTE_SCRWORK* wk, POKEPARTY* party, POKEMON_PARAM* poke );
extern u16 Roulette_GetPanelBP( ROULETTE_SCRWORK* wk, u8 panel_no );
extern u16 RouletteScr_CommGetLap( ROULETTE_SCRWORK* wk );
extern u8 Roulette_GetEvPanelColor( u8 panel_no );


#endif	//__ROULETTE_TOOL_H__



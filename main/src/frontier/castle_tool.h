//==============================================================================
/**
 * @file	castle_tool.h
 * @brief	�o�g���L���b�X���֘A�c�[����
 * @author	nohara
 * @date	2007.07.04
 */
//==============================================================================
#ifndef __CASTLE_TOOL_H__
#define __CASTLE_TOOL_H__

#include "common.h"
#include "system/pms_data.h"
#include "savedata/b_tower.h"
#include "savedata/frontier_savedata.h"


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
}CASTLE_POKE_RANGE;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void Castle_EnemyLapAllTrainerIndexGet( u8 type, int lap, u16 trainer_index[], u8 num );
extern u32 Castle_PokemonParamCreate(B_TOWER_POKEMON *pwd,
	u16 poke_index, int position, u8 pow_rnd, u32 personal_rnd, int heap_id);
extern void Castle_PokemonParamCreateAll(B_TOWER_POKEMON *pwd, u16 poke_index[], 
	u8 pow_rnd[], u32 personal_rnd[], u32 ret_personal_rnd[], int set_num, int heap_id);
extern const CASTLE_POKE_RANGE *Castle_EnemyPokemonRangeGet(int trainer_index, 
	int castle_level);


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
#include "savedata/castle_savedata.h"		//CASTLEDATA
#include "castle_def.h"					//CASTLE_LAP_ENEMY_MAX
#include "frontier_types.h"
#include "frontier_main.h"


//============================================================================================
//
//	�o�g���L���b�X���@�X�N���v�g���[�N
//
//============================================================================================
typedef struct{

	int	heapID;											//�q�[�v
	SAVEDATA* sv;
	CASTLEDATA* castle_savedata;						//�L���b�X���Z�[�u�f�[�^�ւ̃|�C���^
	void* p_work;

	u8	type;											//�V���O���A�_�u���A�}���`�Awifi�}���`
	u8	round;											//�����l�ځH
	u8	hinsi_flag;										//�m���|�P���������邩�t���O
	u8	brain_appear_msg_flag;							//�u���[���o�ꃁ�b�Z�[�W�\��������

	u16	rensyou;										//���݂̘A����
	u16	lap;											//���݂̎���

	u32	win_cnt;										//���݂܂ł̑�������
	int winlose_flag;									//���������t���O

	u16 temp_start_cp;									//���펞��CP��ޔ�
	u16 temp_now_cp;									//���݂�CP��ޔ�

	//�Q�����Ă���莝���|�P�����̈ʒu���
	u8	mine_poke_pos[CASTLE_ENTRY_POKE_MAX];
	u8	clear_flag;										//7�A��(�N���A)�������t���O(0��1)

	//�L���b�X����p��ʂ֓n���p�̃|�P�����f�[�^
	POKEPARTY* p_m_party;								//����
	POKEPARTY* p_e_party;								//�G

	//�G�g���[�i�[�f�[�^
	u16 tr_index[CASTLE_LAP_MULTI_ENEMY_MAX];			//�g���[�i�[index�e�[�u��(*2=�_�u��)

	//�ΐ푊��f�[�^�i�[�ꏊ
	B_TOWER_PARTNER_DATA tr_data[2];

	//�G�|�P�����f�[�^
	u16 enemy_poke_index[CASTLE_ENEMY_POKE_MAX];		//�|�P����index�e�[�u��
	u8	enemy_pow_rnd[CASTLE_ENEMY_POKE_MAX];			//�|�P�����̃p���[����
	u32 enemy_personal_rnd[CASTLE_ENEMY_POKE_MAX];		//�|�P�����̌�����
	B_TOWER_POKEMON enemy_poke[CASTLE_ENEMY_POKE_MAX];	//�|�P�����f�[�^
	u16 dummy_enemy_eom;								//�b�菈��(EOM��2byte�]�v�ɏ������܂�邽��)

	u16 itemno[CASTLE_ENTRY_POKE_MAX];					//�����A�C�e��

	//�G�g���[�i�[��ʂ̏�񂪌��J���ꂽ���t���O
	u8	enemy_temoti_flag[CASTLE_ENEMY_POKE_MAX];
	u8	enemy_level_flag[CASTLE_ENEMY_POKE_MAX];
	u8	enemy_tuyosa_flag[CASTLE_ENEMY_POKE_MAX];
	u8	enemy_waza_flag[CASTLE_ENEMY_POKE_MAX];

	u16 ret_work[CASTLE_RET_WORK_MAX];

	//�莝���|�P����
	u16	hp[CASTLE_MINE_POKE_MAX];						//HP
	u16	pp[CASTLE_MINE_POKE_MAX][4];					//�Z4��PP
	u8	condition[CASTLE_MINE_POKE_MAX];				//�ŁA��ჂȂǂ̏��
	u16 item[CASTLE_MINE_POKE_MAX];						//����

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	send_buf[CASTLE_COMM_BUF_LEN];
	u8	huge_buf[CASTLE_HUGE_BUF_LEN];
	u8	recieve_huge_buf[CASTLE_COMM_PLAYER_NUM][CASTLE_HUGE_BUF_LEN];

	//�ʐM�p�F��{���
	u8	pair_retire_flag;									//���^�C����I�񂾂�=1
	u8	pair_trade_yesno_flag;								//������������(0=�������A1=�������Ȃ�)
	u16	pair_lap;											//����

	u16	pair_trade_count;									//������
	u16	pair_rensyou;										//���݂̘A����(27�l�ڂƂ��̕\���ɕK�v)
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	u8	sel_type;
	u8	pair_sel_type;
	u8	recieve_count;
	u8	parent_check_flag;

	u16 pair_cp;

	u16* list_del_work;

	u32	dummy_work;
}CASTLE_SCRWORK;


//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern u8 Castle_GetMinePokeNum( u8 type, BOOL flag );
extern u8 Castle_GetEnemyPokeNum( u8 type, BOOL flag );
extern void Castle_EnemyPokeMake( u8 set_num, u16 tr_index, u8 level, B_TOWER_POKEMON* rental_poke, u16* enemy_poke_index, B_TOWER_POKEMON* enemy_poke, u8* pow_rnd, u32* personal_rnd );
extern BATTLE_PARAM* BtlCastle_CreateBattleParam(CASTLE_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param);
extern u8 Castle_GetLevel( CASTLE_SCRWORK* wk );
extern BOOL Castle_CommCheck( u8 type );
extern void Castle_EnemyPartySet( CASTLE_SCRWORK* wk );
extern void Castle_PokePartyRecoverAll( POKEPARTY* party );
extern void Castle_BirthInfoSet( CASTLE_SCRWORK* wk, POKEMON_PARAM* poke );
extern void Castle_PokePartyAdd( CASTLE_SCRWORK* wk, POKEPARTY* party, POKEMON_PARAM* poke );
extern u16 CastleScr_CommGetLap( CASTLE_SCRWORK* wk );
extern void Castle_LapNumWrite( GF_BGL_INI* ini, CASTLE_SCRWORK* wk, u32 frm );
extern u16 Castle_GetLap( u16 lap );
extern void Castle_CPRecord_Sub( FRONTIER_SAVEWORK* fro_sv, u8 type, int value );


#endif	//__CASTLE_TOOL_H__



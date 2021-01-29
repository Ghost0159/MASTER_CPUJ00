//==============================================================================
/**
 * @file	tower5.s
 * @brief	�u�o�g���^���[�V���O���ΐ핔���v�t�����e�B�A�X�N���v�g(d31r0205,sp_d31r0205)
 * @author	nohara
 * @date	2007.05.30
 */
//==============================================================================

	.text

	.include	"../frontier_seq_def.h"
	.include	"../frontier_def.h"
	.include	"../../../include/msgdata/msg_d31r0205.h"
	.include	"../../../include/msgdata/msg_ev_win.h"
	.include	"../../../include/field/evwkdef.h"
	.include	"../../fielddata/script/saveflag.h"
	.include	"../../fielddata/script/savework.h"
	.include	"../../field/syswork_def.h"
	.include	"../../field/b_tower_scr_def.h"


//--------------------------------------------------------------------
//
//					     �X�N���v�g�{��
//
//	FSW_LOCAL1	WK_SCENE_D31R0205���i�[
//	FSW_LOCAL3	�L�^������
//
//	FSW_PARAM0	��l���̌����ڊi�[
//	FSW_PARAM2	�G�g���[�i�[��OBJ�R�[�h
//	FSW_PARAM3	SCWK_TEMP3��ύX(���̃X�N���v�g�ł������p���ł��邩������Ȃ��̂Œ��ӁI)
//
//--------------------------------------------------------------------
_EVENT_DATA		fss_tower_single_btl_start	//��ԏ��EVENT_DATA�͎������s
_EVENT_DATA_END								//�I��

//--------------------------------------------------------------------
//					     �f�o�b�N��`
//--------------------------------------------------------------------
//#define DEBUG_BTL_OFF					//�o�g���I�t�f�o�b�N
//#define DEBUG_BTL_LOSE_OFF			//�o�g���s�k�I�t�f�o�b�N
//#define DEBUG_LEADER				//���[�_�[�ƃo�g��(21�l�ځA49�l�ڂ�now_round��ύX���Ȃ��ƃ_��)


//--------------------------------------------------------------------
//							���W��`
//--------------------------------------------------------------------
#define PLAYER_X		(8*15)//(8*16)
#define PLAYER_Y		(8*14)//(8*24)
#define PCWOMAN2_X		(8*6)//(8*16)
#define PCWOMAN2_Y		(8*13)//(8*10)
#define ENEMY_X			(8*16)//(8*16)
#define ENEMY_Y			(8*7)//(8*6)
#define MINE_X			(8*16)//(8*16)
#define MINE_Y			(8*21)//(8*24)


/********************************************************************/
//
//SCENE_TOWER_SINGLE_BTL:���\�[�X���x��(�V���O���o�g�����ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
_RESOURCE_LABEL	single_btl_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA			FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

/*�G�g���[�i�[�̌���^�C�~���O���r���Ȃ̂ŕ�����*/
_RESOURCE_LABEL	enemy_set_resource
	_CHAR_RESOURCE_DATA			FSW_PARAM2,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_TOWER_SINGLE_BTL:�A�N�^�[(�V���O���o�g�����ɏ풓������A�N�^�[�Q)
//====================================================================
#define OBJID_PLAYER				(0)
#define OBJID_PCWOMAN2				(1)
#define OBJID_ENEMY					(2)
#define OBJID_MINE					(3)

//�X�N���v�gID(����Ή��\��)
#define EVENTID_TEST_SCR_OBJ1		(1)

_ACTOR_LABEL	single_btl_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP, \
							PLAYER_X,PLAYER_Y,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_ACTOR_DATA				OBJID_PCWOMAN2,FSW_PARAM6,WF2DMAP_WAY_C_RIGHT, \
							PCWOMAN2_X,PCWOMAN2_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_PARAM0,WF2DMAP_WAY_C_UP, \
							MINE_X,MINE_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

/*�G�g���[�i�[�̌���^�C�~���O���r���Ȃ̂ŕ�����*/
_ACTOR_LABEL	enemy_set_actor
	_ACTOR_DATA				OBJID_ENEMY,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							ENEMY_X,ENEMY_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END


/********************************************************************/
/*							�A�j��									*/
/********************************************************************/

//--------------------------------------------------------------------
//���@�@�����ʒu�ֈړ�
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_player_01
	_ANIME_DATA	FC_WAIT_4F,1
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA	FC_WALK_L_8F,3
	_ANIME_DATA	FC_DIR_U,1
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�l�~�[�@�����ʒu�ֈړ�
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_enemy_01
	//_ANIME_DATA	FC_WALK_D_8F,1
	//_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA	FC_WALK_R_8F,3
	//_ANIME_DATA	FC_DIR_D,1
	_ANIME_DATA	FC_WALK_D_8F,3
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//���@�@�ΐ�O�Ɉ���O��
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_player_02
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�l�~�[�@�ΐ�O�Ɉ���O��
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_enemy_02
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�l�~�[�@�s���ޏ�
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_enemy_03
	//_ANIME_DATA	FC_DIR_D,1
	_ANIME_DATA	FC_WALK_R_8F,1
	//_ANIME_DATA	FC_DIR_U,1
	_ANIME_DATA	FC_WALK_U_8F,2
	//_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA	FC_WALK_L_8F,3
	//_ANIME_DATA	FC_DIR_U,1
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//���@�@�ΐ폟����@��������U�����
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_player_03
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�ē���@�ΐ폟����@�߂Â�
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_pcwoman2_02
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//���@�@�񕜌�U�����
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_player_04
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�ē���@�񕜌㉺����
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_pcwoman2_03
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END
	
//--------------------------------------------------------------------
//���@�@��������
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_player_05
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//���[�_�[����@�����ʒu�ֈړ�
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0205_leader_01
	_ANIME_DATA	FC_WALK_R_16F,3
	_ANIME_DATA	FC_WALK_D_16F,3
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END


/********************************************************************/
/*							�J�n									*/
/********************************************************************/
fss_tower_single_btl_start:

	/*���[�N�m��*/
	_BATTLE_REC_INIT

	_SAVE_EVENT_WORK_GET	WK_SCENE_D31R0205,FSW_LOCAL1

	/*FLAG_CHANGE*/
	_CALL				ev_d31r0205_flag_change

	/*OBJ_CHANGE*/
	_CALL				ev_d31r0205_obj_change

	/*�E����WIFI�Q�[�g*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI,comm_tower_5_char_set_wifi
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_RETRY,comm_tower_5_char_set_wifi
	_JUMP				comm_tower_5_char_set_single
	_END

fss_tower_single_btl_start_sub:
	_CHAR_RESOURCE_SET	single_btl_set_resource					/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			single_btl_set_actor					/*�A�N�^�[�o�^*/

	/*�u���낭����v�Ŏg�p���郏�[�N�Ȃ̂ŃN���A���Ă���*/
	_LDVAL				FSW_LOCAL3,0

	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/********************/
	//SCENE_CHANGE_LABEL
	/********************/
	_IFVAL_JUMP			FSW_LOCAL1,EQ,1,ev_tower_rooms_common_entry		/*�ŏ�����*/
	_IFVAL_JUMP			FSW_LOCAL1,EQ,2,ev_tower_rooms_common_continue	/*��������*/
	_END

comm_tower_5_char_set_wifi:
	_LDVAL				FSW_PARAM6,WIFISW
	_JUMP				fss_tower_single_btl_start_sub
	_END

comm_tower_5_char_set_single:
	_LDVAL				FSW_PARAM6,BFSM
	_JUMP				fss_tower_single_btl_start_sub
	_END


/********************************************************************/
/*						FLAG_CHANGE_LABEL 							*/
/********************************************************************/
ev_d31r0205_flag_change:
	_IFVAL_JUMP			FSW_LOCAL1,EQ,0,ev_d31r0205_flag_change_end

	//_FLAG_SET			FV_D31R0205_TRAINER

	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_PARAM0
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_LOCAL0
	//_LDWK				OBJCHRWORK0,FSW_LOCAL0
	_RET

ev_d31r0205_flag_change_end:
	_RET


/********************************************************************/
/*						OBJ_CHANGE_LABEL 							*/
/********************************************************************/
ev_d31r0205_obj_change:
	//_OBJ_INVISIBLE	FLDOBJ_ID_PLAYER
	_IFVAL_JUMP			FSW_LOCAL1,EQ,3,ev_d31r0205_obj_pos_change
	_RET

ev_d31r0205_obj_pos_change:
	//_OBJ_POS_CHANGE	D31R0205_HERO,5,0,6,DIR_RIGHT
	_RET


/********************************************************************/
/*						���߂ĕ����ɓ��ꂵ���� 						*/
/********************************************************************/
ev_tower_rooms_common_entry:
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0205,3
	_SAVE_EVENT_WORK_GET	WK_SCENE_D31R0205,FSW_LOCAL1

	/*�G���[�`�F�b�N*/
	_BTOWER_TOOLS		BTWR_DEB_IS_WORK_NULL,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_rooms_common_exit

	_CALL				ev_tower_rooms_comanm_in
	_JUMP				ev_tower_rooms_sbtl_01
	_END


/********************************************************************/
/*						�V���O���ΐ�t���[	 						*/
/********************************************************************/
ev_tower_rooms_sbtl_01:
	_CALL				ev_tower_rooms_common_sbtl_call

#ifndef DEBUG_BTL_LOSE_OFF	/********************************************/
	/*�����Ė߂�Ƃ��̏���*/
	_IFVAL_JUMP			FSW_PARAM3,EQ,0,ev_tower_rooms_common_lose
#endif	/****************************************************************/

	/*���������̏���(�N���A���Ă邩�ǂ�����FSW_ANSWER�ɂ�����)*/
	_CALL				ev_tower_rooms_common_win_param
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_rooms_common_clear_ret

	//�G�l�~�[�ޏ�
	_CALL				ev_tower_rooms_comanm_eneout

	//�v���C���[�U�����
	_CALL				ev_tower_rooms_comanm_sbtl_end
	
	//�A�폈��

	//�|�P������
	_CALL				ev_tower_rooms_common_heal

	//�o�g�����[�_�[��`�F�b�N
	_JUMP				ev_tower_rooms_if_leader_battle
	_END

//�����Ė߂�Ƃ��̏���
ev_tower_rooms_common_lose:
	_JUMP				ev_tower_rooms_common_rec_lose
	_END

//�N���A���Ė߂�Ƃ��̏���
ev_tower_rooms_common_clear_ret:
	_JUMP				ev_tower_rooms_common_rec_win
	_END

//���[�_�[�ɏ������Ė߂�Ƃ��̏���
ev_tower_rooms_leader_clear_ret:
	_JUMP				ev_tower_rooms_leader_rec_win
	_END


/********************************************************************/
/*				�u�����قǂ́@�킢���@�L�^���܂����H�v				*/
/********************************************************************/
/*���[�_�[�ɏ���*/
ev_tower_rooms_leader_rec_win:
	/*�u�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower5_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_tower5_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX		FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower5_win_rec_yes		/*�u�͂��v*/

	//�N���A�p�����[�^�Z�b�g
	_BTOWER_TOOLS		BTWR_SUB_SET_CLEAR_SCORE,BTWR_NULL_PARAM,FSW_ANSWER

#if 1
	/*�u�݂��ƃ^���[�^�C�N�[���ɏ�������܂����ˁv*/
	_TALKMSG			msg_tower_101_02

	/*�u�v���C���[�͂a�o����������v*/
	_CALL				ev_tower5_battle_point_get
#endif

	_TALK_CLOSE
	_JUMP				ev_tower_rooms_common_exit
	_END

/*����*/
ev_tower_rooms_common_rec_win:
	/*�u�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower5_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_tower5_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX		FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower5_win_rec_yes		/*�u�͂��v*/

	//�N���A�p�����[�^�Z�b�g(�Z�[�u��ɏ�������)
	_BTOWER_TOOLS		BTWR_SUB_SET_CLEAR_SCORE,BTWR_NULL_PARAM,FSW_ANSWER

#if 1
	/*BTWR_MODE_RETRY�͉������Ȃ�*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_RETRY,comm_tower_rooms_common_rec_win_retry
#endif

#if 1
	/*�u�V��˔j���߂łƂ��v*/
	_TALKMSG			msg_tower_101_01

	/*�u�v���C���[�͂a�o����������v*/
	_CALL				ev_tower5_battle_point_get
#endif

	_TALK_CLOSE
	_JUMP				ev_tower_rooms_common_exit
	_END

/*wifiDL�́u�܂��̂ւ�ɂ��傤����vBP�͂����Ȃ�*/
comm_tower_rooms_common_rec_win_retry:
	/*�u�V��˔j���߂łƂ��v*/
	//_TALKMSG			msg_tower_101_05				/*�O�œ������Ƃ����̂ł���Ȃ�*/
	_TALK_CLOSE
	_JUMP				ev_tower_rooms_common_exit
	_END

/*����*/
ev_tower_rooms_common_rec_lose:
	/*�u�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower5_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_tower5_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX		FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower5_lose_rec_yes		/*�u�͂��v*/
	_TALK_CLOSE

	//�s��p�����[�^�Z�b�g(�Z�[�u��ɏ�������)
	_BTOWER_TOOLS		BTWR_SUB_SET_LOSE_SCORE,BTWR_NULL_PARAM,FSW_ANSWER

	_JUMP				ev_tower_rooms_common_exit
	_END

/*�u�͂��v*/
ev_tower5_lose_rec_yes:
	_CALL				ev_tower5_room_rec_lose		/*�s�k�̋L�^*/
	_RET

ev_tower5_win_rec_yes:
	_CALL				ev_tower5_room_rec_win		/*�����̋L�^*/
	_RET

/*�u�v���C���[�͂a�o����������v*/
ev_tower5_battle_point_get:
	_BTOWER_TOOLS		BTWR_SUB_ADD_BATTLE_POINT,BTWR_NULL_PARAM,FSW_ANSWER
	_NUMBER_NAME		1,FSW_ANSWER
	_PLAYER_NAME		0
	_TALKMSG			msg_tower_101_03
	_ME_PLAY			ME_BP_GET
	_ME_WAIT
	
	/*���|�[�g�ɂ�������ł��܂�*/
	//_TALKMSG			msg_tower_101_04
	_RET


/********************************************************************/
/*				�v���C���[������@�G�l�~�[�ޏ�						*/
/********************************************************************/
ev_tower_rooms_comanm_eneout:
	_OBJ_ANIME			OBJID_ENEMY,anm_d31r0205_enemy_03
	_OBJ_ANIME_WAIT

	/*�ΐ�g���[�i�[�폜*/
	_ACTOR_FREE			OBJID_ENEMY
	_CHAR_RESOURCE_FREE	FSW_PARAM2

	//�o�j�b�V���t���OON
	//_FLAG_SET			FV_OBJID_ETC
	//_OBJ_DEL			OBJID_ENEMY
	_RET


/********************************************************************/
/*			�v���C���[������@�v���C���[�U�����&�ē���߂Â�		*/
/********************************************************************/
ev_tower_rooms_comanm_sbtl_end:
	_OBJ_ANIME			OBJID_MINE,anm_d31r0205_player_03
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_d31r0205_pcwoman2_02
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*							�񕜏���								*/
/********************************************************************/
ev_tower_rooms_common_heal:
	_TALKMSG			msg_tower_65
	_ME_PLAY			SEQ_ASA
	_ME_WAIT
	_PC_KAIFUKU
	_RET


/********************************************************************/
/*			�V���O���o�g���������p�����[�^�Z�b�g					*/
/********************************************************************/
ev_tower_rooms_common_win_param:
	//���E���h���X�V
	_BTOWER_TOOLS		BTWR_SUB_INC_ROUND,BTWR_NULL_PARAM,FSW_ANSWER

	//�N���A���Ă邩�`�F�b�N
	_BTOWER_TOOLS		BTWR_SUB_IS_CLEAR,BTWR_NULL_PARAM,FSW_ANSWER
	_RET


/********************************************************************/
/*					��������ŕ����ɓ��ꂵ����						*/
/********************************************************************/
ev_tower_rooms_common_continue:
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0205,3
	_SAVE_EVENT_WORK_GET	WK_SCENE_D31R0205,FSW_LOCAL1
	
	/*�L�^�������[�N�Ƀf�[�^���������܂�Ă��Ȃ��̂Ń��j���[�\�����Ȃ�*/
	_LDVAL				FSW_LOCAL3,1

	//�G���[����
	_BTOWER_TOOLS		BTWR_DEB_IS_WORK_NULL,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_rooms_common_exit

	_CALL				ev_tower_rooms_comanm_in
	_CALL				ev_tower_rooms_comanm_continue

	//���́H�l�ڂ́c�c
	_JUMP				ev_tower_rooms_if_leader_battle
	_END

//���@�Ǝ�t��@����
ev_tower_rooms_comanm_in:
	_OBJ_ANIME			OBJID_MINE,anm_d31r0205_player_01
	_OBJ_ANIME_WAIT
	_RET

//���@�Ǝ�t��@��������n�߂��Ƃ��̓����
ev_tower_rooms_comanm_continue:
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_d31r0205_pcwoman2_02
	_OBJ_ANIME			OBJID_MINE,anm_d31r0205_player_05
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*					��������ޏꂵ�Ď�t�ɖ߂�						*/
/********************************************************************/
ev_tower_rooms_common_exit:
	/*���[�N�J��*/
	_BATTLE_REC_EXIT

	//�E�F�C�g
	_TIME_WAIT			30,FSW_ANSWER

	//��t�߂�t���O�Z�b�g
	//_LDVAL				WK_SCENE_D31R0201,1
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0201,1

	/*���݂̃v���C���[�h���擾*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER

	//�����Ă����Q�[�g�ʒu�֖߂�
	_SWITCH				FSW_ANSWER
	_CASE_JUMP			BTWR_MODE_WIFI,ev_tower_rooms_comexit_01
	_CASE_JUMP			BTWR_MODE_RETRY,ev_tower_rooms_comexit_01

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_ACTOR_FREE			OBJID_MINE
	_ACTOR_FREE			OBJID_PCWOMAN2
	_CHAR_RESOURCE_FREE	FSW_PARAM6

	//_MAP_CHANGE			ZONE_ID_D31R0201,0,11,6,DIR_UP
	//_END
	_SCRIPT_FINISH			/*_END����2D�}�b�v�I��*/

ev_tower_rooms_comexit_01:
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_ACTOR_FREE			OBJID_MINE
	_ACTOR_FREE			OBJID_PCWOMAN2
	_CHAR_RESOURCE_FREE	FSW_PARAM6

	//_MAP_CHANGE			ZONE_ID_D31R0201,0,15,6,DIR_UP
	//_END
	_SCRIPT_FINISH			/*_END����2D�}�b�v�I��*/


/********************************************************************/
/*							�e�X�g�H								*/
/********************************************************************/
ev_tower_rooms_test_entry:
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0205,0
	_SAVE_EVENT_WORK_GET	WK_SCENE_D31R0205,FSW_LOCAL1
	_CALL				ev_tower_rooms_common_sbtl_call
	_END


/********************************************************************/
/*					�V���O���o�g���Ăяo��							*/
/********************************************************************/
ev_tower_rooms_common_sbtl_call:
	/*�ΐ�g���[�i�[���I*/
	_BTOWER_TOOLS		BTWR_SUB_CHOICE_BTL_PARTNER,BTWR_NULL_PARAM,FSW_ANSWER

	/*�G�l�~�[����*/
	_CALL				ev_tower_rooms_comanm_enein

	/*�ΐ�O�䎌*/
	_TALKMSG_BTOWER_APPEAR	0
	_AB_KEYWAIT
	_TALK_CLOSE

	/*�݂��ɋ߂Â�*/
	_CALL				ev_tower_rooms_comanm_sbtl_start

	//_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK
	_ENCOUNT_EFFECT		FR_ENCOUNT_EFF_WIPE

	/*����������Ȃ������Z�b�g*/
	_BTOWER_TOOLS		BTWR_SUB_SET_FIRST_BTL_FLAG,BTWR_NULL_PARAM,FSW_ANSWER

#ifndef DEBUG_BTL_OFF	/************************************************/
	/*�퓬�Ăяo��*/
	//_BTOWER_TOOLS		BTWR_SUB_LOCAL_BTL_CALL,BTWR_NULL_PARAM,FSW_ANSWER
	_BTOWER_BATTLE_CALL
	//_TRAINER_LOSE_CHECK FSW_ANSWER
	//_LDWK				FSW_PARAM3,FSW_ANSWER
	_BTOWER_CALL_GET_RESULT	FSW_PARAM3
#endif	/****************************************************************/

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_RET

/*�G�l�~�[����*/
ev_tower_rooms_comanm_enein:
	/*�ΐ�g���[�i�[��OBJ�R�[�h�擾*/
	_BTOWER_TOOLS		BTWR_SUB_GET_ENEMY_OBJ,0,FSW_PARAM2
	//_BTOWER_TOOLS		BTWR_SUB_GET_ENEMY_OBJ,0,FSW_ANSWER
	//_LDWK				OBJCHRWORK1,FSW_ANSWER

	/*�ΐ�g���[�i�[�ǉ�*/
	_CHAR_RESOURCE_SET	enemy_set_resource				/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			enemy_set_actor					/*�A�N�^�[�o�^*/

	//�o�j�b�V���t���OOFF
	//_FLAG_RESET		FV_OBJID_ETC
	//_OBJ_ADD			OBJID_ENEMY
	_OBJ_ANIME			OBJID_ENEMY,anm_d31r0205_enemy_01	
	_OBJ_ANIME_WAIT
	_RET

/*�V���O���o�g���O�@�݂��Ɉ���߂Â�*/
ev_tower_rooms_comanm_sbtl_start:
	_OBJ_ANIME			OBJID_MINE,anm_d31r0205_player_02
	_OBJ_ANIME			OBJID_ENEMY,anm_d31r0205_enemy_02
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*						�V���O���ΐ�								*/
/********************************************************************/
ev_tower_rooms_sbtl_02:
	//�ʏ�A�� ���́H�l�ڂ̑���ł�
	_BTOWER_TOOLS		BTWR_SUB_GET_NOW_ROUND,BTWR_NULL_PARAM,FSW_PARAM3
	_NUMBER_NAME		0,FSW_PARAM3
	_TALKMSG			msg_tower_66

	////////////////////////////////////////////////////////////////////////////////
	/*�u���[���̃��X�g�փW�����v*/

	//�V���O���o�g�����[�h�łȂ����̓u���[���̃��X�g�փW�����v���邩���΂�
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,NE,BTWR_MODE_SINGLE,ev_tower_rooms_sbtl_02_00

	//�V���O���Ȃ烉�E���h�`�F�b�N
	_BTOWER_TOOLS		BTWR_SUB_GET_RENSHOU_CNT,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,LEADER_SET_1ST,ev_tower_rooms_leader_comin_02
	_IFVAL_JUMP			FSW_ANSWER,EQ,LEADER_SET_2ND,ev_tower_rooms_leader_comin_02
	////////////////////////////////////////////////////////////////////////////////

	_JUMP				ev_tower_rooms_sbtl_02_00
	_END

ev_tower_rooms_sbtl_02_00:
	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP			FSW_LOCAL3,EQ,0,ev_tower_rooms_sbtl_02_rec

	_BMPLIST_INIT		24,11,0,0,FSW_ANSWER			//B�L�����Z������
	_BMPLIST_MAKE_LIST	msg_tower_choice_06,FSEV_WIN_TALK_MSG_NONE,0			//�Â���
	_JUMP				ev_tower_rooms_sbtl_02_sub
	_END

ev_tower_rooms_sbtl_02_rec:
	_BMPLIST_INIT		23,9,0,0,FSW_ANSWER				//B�L�����Z������
	_BMPLIST_MAKE_LIST	msg_tower_choice_06,FSEV_WIN_TALK_MSG_NONE,0			//�Â���
	_BMPLIST_MAKE_LIST	msg_tower_choice_13,FSEV_WIN_TALK_MSG_NONE,1			/*���낭����*/
	_JUMP				ev_tower_rooms_sbtl_02_sub
	_END

ev_tower_rooms_sbtl_02_sub:
	_BMPLIST_MAKE_LIST	msg_tower_choice_07,FSEV_WIN_TALK_MSG_NONE,2			//�₷��
	_BMPLIST_MAKE_LIST	msg_tower_choice_08,FSEV_WIN_TALK_MSG_NONE,3			//���^�C�A
	_BMPLIST_START
	
	_TALK_CLOSE

	_SWITCH				FSW_ANSWER
	_CASE_JUMP			0,ev_tower_rooms_next			//�Â���
	_CASE_JUMP			1,ev_tower_rooms_is_rec			/*���낭����*/
	_CASE_JUMP			2,ev_tower_rooms_is_rest		//�₷��
	_CASE_JUMP			3,ev_tower_rooms_is_retire		//���^�C�A
	_JUMP				ev_tower_rooms_next				//�Â���
	_END


/********************************************************************/
/*							�Â���								*/
/********************************************************************/
ev_tower_rooms_next:
	//�ē�����Ƃ̈ʒu��
	_CALL				ev_tower_rooms_comanm_guide_out

	/*���[�N���N���A���Ă���*/
	_LDVAL				FSW_LOCAL3,0

	//�o�g��
	_JUMP				ev_tower_rooms_sbtl_01
	_END

//�񕜌�@�v���C���[�U��������ē��쉺����
ev_tower_rooms_comanm_guide_out:
	_OBJ_ANIME			OBJID_MINE,anm_d31r0205_player_04
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_d31r0205_pcwoman2_03
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*							�L�^����								*/
/********************************************************************/
ev_tower_rooms_is_rec:
	/*�u�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower5_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_tower5_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_tower_rooms_sbtl_02

	_CALL			ev_tower5_room_rec_win		/*�����̋L�^*/
	_JUMP			ev_tower_rooms_sbtl_02
	_END


/********************************************************************/
/*							���ʋL�^*/
/********************************************************************/
/*����*/
ev_tower5_room_rec_win:
	_CALL				ev_tower5_room_rec_common		/*ANSWER,LOCAL5�g�p��*/
	//_ADD_WK			FSW_LOCAL5,1
	//_SUB_WK			FSW_LOCAL5,1					/*���E���h���ł͂Ȃ��������Ȃ̂�*/
	_JUMP				ev_tower5_room_rec
	_END

/*����*/
ev_tower5_room_rec_lose:
	_CALL				ev_tower5_room_rec_common		/*ANSWER,LOCAL5�g�p��*/
	_ADD_WK				FSW_LOCAL5,1
	_JUMP				ev_tower5_room_rec
	_END

/*���ʕ���*/
ev_tower5_room_rec_common:
	/*�^��f�[�^�Z�[�u*/
	_BTOWER_TOOLS		BTWR_SUB_GET_RENSHOU_CNT,BTWR_NULL_PARAM,FSW_LOCAL5		/*�A����*/
	_RET

ev_tower5_room_rec:
	_JUMP				ev_tower5_room_rec_sub
	_END

ev_tower5_room_rec_sub:
	/*�L�^�������[�N�Z�b�g*/
	_LDVAL				FSW_LOCAL3,1

	_TALKMSG_ALL_PUT	msg_tower_100_05
	/*BTWR_MODE_SINGLE,BTWR_MODE_DOUBLE*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER		/*���[�h*/
	_ADD_WAITICON
	_BATTLE_REC_SAVE	FRONTIER_NO_TOWER,FSW_ANSWER,FSW_LOCAL5,FSW_LOCAL5
	_DEL_WAITICON

	//_BATTLE_REC_LOAD

	_IFVAL_JUMP			FSW_LOCAL5,EQ,1,ev_tower5_room_rec_true

	/*�u�L�^�o���܂���ł����v*/
	_TALKMSG			msg_tower_100_03
	_RET

ev_tower5_room_rec_true:
	_SE_PLAY			SEQ_SE_DP_SAVE
	/*�u�L�^����܂����v*/
	_PLAYER_NAME		0
	_TALKMSG			msg_tower_100_02
	_RET
	
/*�^��f�[�^���Ȃ������b�Z�[�W*/
ev_tower5_room_rec_msg1:
	_TALKMSG			msg_tower_100_01
	_RET

/*���łɘ^��f�[�^�����鎞���b�Z�[�W*/
ev_tower5_room_rec_msg2:
	_TALKMSG			msg_tower_100_04
	_RET


/********************************************************************/
/*							�₷��									*/
/********************************************************************/
ev_tower_rooms_is_rest:
	//���f���ċx�ނ��ǂ����H
	_TALKMSG			msg_tower_67

	_YES_NO_WIN			FSW_ANSWER

	_TALK_CLOSE
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_tower_rooms_common_rest
	_JUMP				ev_tower_rooms_sbtl_02
	_END

//�x�ނƂ��̏���
ev_tower_rooms_common_rest:
	//_LDVAL				WK_SCENE_D31R0201,2
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0201,2		/*��������ĊJ�����悤�ɂ���*/

	//�v���C�f�[�^�Z�[�u
	_BTOWER_TOOLS		BTWR_SUB_SAVE_REST_PLAY_DATA,BTWR_NULL_PARAM,FSW_ANSWER
	_BTOWER_WORK_RELEASE

	_TALKMSG			msg_tower_71
	_ADD_WAITICON

	_REPORT_SAVE		FSW_ANSWER
	//_REPORT_DIV_SAVE	FSW_ANSWER					/*�ʐM���Ă��Ȃ�*/
	_DEL_WAITICON
	_SE_PLAY			SEQ_SE_DP_SAVE
	_SE_WAIT			SEQ_SE_DP_SAVE

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_TALK_CLOSE

	/*���[�N�J��*/
	_BATTLE_REC_EXIT

//	_VANISH_DUMMY_OBJ_DEL
	//���Z�b�g�R�}���h
	_BTOWER_TOOLS		BTWR_TOOL_SYSTEM_RESET,BTWR_NULL_PARAM,FSW_ANSWER
	_END


/********************************************************************/
/*							���^�C��								*/
/********************************************************************/
ev_tower_rooms_is_retire:
	//���^�C�A���邩�ǂ����H
	_TALKMSG			msg_tower_68

	_YES_NO_WIN_EX		FSW_ANSWER

	_TALK_CLOSE

	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_tower_rooms_common_retire
	_JUMP				ev_tower_rooms_sbtl_02
	_END

//���^�C�A����Ƃ��̏���(�L�^���闬��Ȃ�)
ev_tower_rooms_common_retire:
	//�s��p�����[�^�Z�b�g
	_BTOWER_TOOLS		BTWR_SUB_SET_LOSE_SCORE,BTWR_NULL_PARAM,FSW_ANSWER
	_JUMP				ev_tower_rooms_common_exit
	_END


/********************************************************************/
/*						���[�_�[�ΐ�								*/
/********************************************************************/
ev_tower_rooms_if_leader_battle:
	//�V���O���o�g�����[�h���ǂ����H
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,NE,BTWR_MODE_SINGLE,ev_tower_rooms_sbtl_02

#ifdef DEBUG_LEADER	/****************************************************/
	/*�f�o�b�N�F���[�_�[�Ɛ퓬*/
	_JUMP				ev_tower_rooms_leader_comin
#endif	/****************************************************************/

	//�V���O���Ȃ烉�E���h�`�F�b�N
	_BTOWER_TOOLS		BTWR_SUB_GET_RENSHOU_CNT,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,LEADER_SET_1ST,ev_tower_rooms_leader_comin
	_IFVAL_JUMP			FSW_ANSWER,EQ,LEADER_SET_2ND,ev_tower_rooms_leader_comin
	_JUMP				ev_tower_rooms_sbtl_02
	_END

ev_tower_rooms_leader_comin:
	//�^���[�^�C�N�[���Ƃ̐퓬�ɂȂ�܂�
	_TALKMSG			msg_tower_boss_01
	_JUMP				ev_tower_rooms_leader_comin_02
	_END

ev_tower_rooms_leader_comin_02:
	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP			FSW_LOCAL3,EQ,0,ev_tower_rooms_leader_comin_rec

	_BMPLIST_INIT		24,11,0,0,FSW_ANSWER			//B�L�����Z������
	_BMPLIST_MAKE_LIST	msg_tower_choice_06,FSEV_WIN_TALK_MSG_NONE,0			//�Â���
	_JUMP				ev_tower_rooms_leader_comin_sub
	_END

ev_tower_rooms_leader_comin_rec:
	_BMPLIST_INIT		23,9,0,0,FSW_ANSWER				//B�L�����Z������
	_BMPLIST_MAKE_LIST	msg_tower_choice_06,FSEV_WIN_TALK_MSG_NONE,0			//�Â���
	_BMPLIST_MAKE_LIST	msg_tower_choice_13,FSEV_WIN_TALK_MSG_NONE,1			/*���낭����*/
	_JUMP				ev_tower_rooms_leader_comin_sub
	_END

ev_tower_rooms_leader_comin_sub:
	_BMPLIST_MAKE_LIST	msg_tower_choice_07,FSEV_WIN_TALK_MSG_NONE,2			//�₷��
	_BMPLIST_MAKE_LIST	msg_tower_choice_08,FSEV_WIN_TALK_MSG_NONE,3			//���^�C�A
	_BMPLIST_START
	
	_TALK_CLOSE
	_SWITCH				FSW_ANSWER
	_CASE_JUMP			0,ev_tower_rooms_leader_entry	//�Â���
	_CASE_JUMP			1,ev_tower_rooms_is_rec			/*���낭����*/
	_CASE_JUMP			2,ev_tower_rooms_is_rest		//�₷��
	_CASE_JUMP			3,ev_tower_rooms_is_retire		//���^�C�A
	_JUMP				ev_tower_rooms_leader_entry		//�Â���
	_END


/********************************************************************/
/*						�Â���@���[�_�[����						*/
/********************************************************************/
ev_tower_rooms_leader_entry:
	//�ē�����Ƃ̈ʒu��
	_CALL				ev_tower_rooms_comanm_guide_out

	/*���[�N���N���A���Ă���*/
	_LDVAL				FSW_LOCAL3,0

	//���[�_�[�f�[�^�Z�b�g
	_BTOWER_TOOLS		BTWR_SUB_CHOICE_BTL_PARTNER,BTWR_NULL_PARAM,FSW_ANSWER
	
	//���[�_�[����
	//_BGM_STOP			SEQ_BF_TOWWER
	//_BGM_PLAY			SEQ_PL_EYEBRAIN
	_CALL				ev_tower_rooms_comanm_leader_in

	//���[�_�[�ΐ�O�䎌
	_BTOWER_TOOLS		BTWR_SUB_GET_RENSHOU_CNT,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,LEADER_SET_2ND,ev_tower_rooms_leader_talk_01

	_PLAYER_NAME		0
	_RIVAL_NAME			1
	_TALKMSG			msg_tower_boss_02
	_JUMP				ev_tower_rooms_leader_talk01_end
	_END

ev_tower_rooms_leader_talk01_end:
	_TALK_CLOSE

	//�݂��ɋ߂Â�
	_CALL				ev_tower_rooms_comanm_sbtl_start

	//_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK
	_RECORD_INC			RECID_FRONTIER_BRAIN
	_BRAIN_ENCOUNT_EFFECT		FRONTIER_NO_TOWER

#ifndef DEBUG_BTL_OFF	/************************************************/
	/*�퓬�Ăяo��*/
	//_BTOWER_TOOLS		BTWR_SUB_LOCAL_BTL_CALL,BTWR_NULL_PARAM,FSW_ANSWER
	_BTOWER_BATTLE_CALL
	//_TRAINER_LOSE_CHECK FSW_ANSWER
	//_LDWK				FSW_PARAM3,FSW_ANSWER
	_BTOWER_CALL_GET_RESULT	FSW_PARAM3
#endif	/****************************************************************/

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

#ifndef DEBUG_BTL_LOSE_OFF	/********************************************/
	/*�����Ė߂�Ƃ��̏���*/
	_IFVAL_JUMP			FSW_PARAM3,EQ,0,ev_tower_rooms_common_lose
#endif	/****************************************************************/

	//���[�_�[�����䎌
	_BTOWER_TOOLS		BTWR_SUB_GET_RENSHOU_CNT,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,LEADER_SET_2ND,ev_tower_rooms_leader_talk_02

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_TOWER,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_NONE,ev_tower_meory_print_put_1st

	/*1st���[�_�[�퓬��msg*/
	_TALKMSG			msg_tower_boss_04
	_BTOWER_TOOLS		BTWR_SUB_SET_LEADER_CLEAR_FLAG,1,FSW_ANSWER

	_JUMP				ev_tower_rooms_leader_talk02_end
	_END

/*��t��1st�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_tower_meory_print_put_1st:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_TOWER,MEMORY_PRINT_PUT_1ST
	_RET

ev_tower_rooms_leader_talk02_end:
	_TALK_CLOSE
	
	_CALL				ev_tower_rooms_common_win_param

	//���[�_�[��̏ꍇ�A�K���I���
	_JUMP				ev_tower_rooms_leader_clear_ret
	_END

//����
ev_tower_rooms_leader_talk_01:
	_TALKMSG			msg_tower_boss_03
	_JUMP				ev_tower_rooms_leader_talk01_end
	_END

ev_tower_rooms_leader_talk_02:

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_TOWER,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_PUT_OK_1ST,ev_tower_meory_print_put_2nd

	/*2nd���[�_�[�퓬��msg*/
	_TALKMSG			msg_tower_boss_05
	_BTOWER_TOOLS		BTWR_SUB_SET_LEADER_CLEAR_FLAG,2,FSW_ANSWER
	_JUMP				ev_tower_rooms_leader_talk02_end
	_END

/*��t��2nd�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_tower_meory_print_put_2nd:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_TOWER,MEMORY_PRINT_PUT_2ND
	_RET


/********************************************************************/
/*							���[�_�[����							*/
/********************************************************************/
ev_tower_rooms_comanm_leader_in:
	//�_�~�[�Ńh�b�y���Q���K�[����
	//_LDVAL			OBJCHRWORK1,TOWERBOSS
	_LDVAL				FSW_PARAM2,TOWERBOSS

	/*�ΐ�g���[�i�[�ǉ�*/
	_CHAR_RESOURCE_SET	enemy_set_resource				/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			enemy_set_actor					/*�A�N�^�[�o�^*/

	//�o�j�b�V���t���OOFF
	//_FLAG_RESET		FV_OBJID_ETC
	//_OBJ_ADD			OBJID_ENEMY
	_OBJ_ANIME			OBJID_ENEMY,anm_d31r0205_leader_01
	_OBJ_ANIME_WAIT
	_RET



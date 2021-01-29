//==============================================================================
/**
 * @file	tower_6.s
 * @brief	�u�o�g���^���[�v�t�����e�B�A�X�N���v�g(�^���[�}���`�ΐ핔��)
 * @author	nohara
 * @date	2007.05.29
 */
//==============================================================================

	.text

	.include	"../frontier_seq_def.h"
	.include	"../frontier_def.h"
	.include	"../../../include/msgdata/msg_d31r0206.h"
	.include	"../../../include/msgdata/msg_ev_win.h"
	.include	"../../../include/field/evwkdef.h"
	.include	"../../fielddata/script/saveflag.h"
	.include	"../../fielddata/script/savework.h"
	.include	"../../field/b_tower_scr_def.h"
	.include	"../../fielddata/base_goods/goods_id_def.h"


//--------------------------------------------------------------------
//
//					     �X�N���v�g�{��
//
//	FSW_LOCAL0	�ʐM���ʂȂǂɎg�p
//	FSW_LOCAL1	�A�j�������鎩����OBJID
//	FSW_LOCAL2	�A�j��������p�[�g�i�[��OBJID
//	FSW_LOCAL3	(SCWK_TEMP0��u�������A���p���ɒ��ӁI)
//	FSW_LOCAL4	�u���΂炭���܂����������v��\�����ăE�B���h�E���폜���邩���ʗp
//	FSW_LOCAL5	�L�^�������Ɏg�p
//	FSW_LOCAL6	�L�^������
//
//	FSW_PARAM0	����
//	FSW_PARAM1	�p�[�g�i�[
//	FSW_PARAM2	�G�g���[�i�[1
//	FSW_PARAM3	�G�g���[�i�[2
//
//--------------------------------------------------------------------
_EVENT_DATA		fss_tower_6_start	//��ԏ��EVENT_DATA�͎������s
_EVENT_DATA_END						//�I��

//--------------------------------------------------------------------
//					     �f�o�b�N��`
//--------------------------------------------------------------------
//#define DEBUG_7_WIN					//7�A���f�o�b�N
//#define DEBUG_BTL_OFF					//�o�g���I�t�f�o�b�N
//#define DEBUG_BTL_LOSE_OFF			//�o�g���s�k�I�t�f�o�b�N


//--------------------------------------------------------------------
//							���W��`
//--------------------------------------------------------------------
#define PLAYER_X		(8*15)//(8*16)
#define PLAYER_Y		(8*15)//(8*16)
#define PCWOMAN2_01_X	(8*5)//(8*4)
#define PCWOMAN2_01_Y	(8*13)//(8*8)
#define PCWOMAN2_02_X	(PCWOMAN2_01_X)
#define PCWOMAN2_02_Y	(8*15)//(8*10)
#define MINE_X			(8*15)//(8*16)
#define MINE_Y			(8*21)//(8*20)
#define PAIR_X			(8*17)//(8*18)
#define PAIR_Y			(MINE_Y)
#define SIO_USER_0_X	(MINE_X)
#define SIO_USER_0_Y	(MINE_Y)
#define SIO_USER_1_X	(PAIR_X)
#define SIO_USER_1_Y	(PAIR_Y)
#define ENEMY_X			(PAIR_X)
#define ENEMY_Y			(8*7)
#define ENEMY2_X		(MINE_X)
#define ENEMY2_Y		(ENEMY_Y)


/********************************************************************/
//
//		SCENE_TOWER:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
//AI�}���`
_RESOURCE_LABEL	default_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	//_CHAR_RESOURCE_DATA	FSW_PARAM0,WF2DC_C_MOVENORMAL	//����
	_CHAR_RESOURCE_DATA		FSW_PARAM1,WF2DC_C_MOVENORMAL	//�p�[�g�i�[
	_CHAR_RESOURCE_DATA_END

//�ʐM�}���`
_RESOURCE_LABEL	comm_multi_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

/*�G�g���[�i�[�̌���^�C�~���O���r���Ȃ̂ŕ�����*/
_RESOURCE_LABEL	enemy_set_resource
	_CHAR_RESOURCE_DATA		FSW_PARAM2,WF2DC_C_MOVENORMAL	//�G1
	_CHAR_RESOURCE_DATA		FSW_PARAM3,WF2DC_C_MOVENORMAL	//�G2
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_TOWER:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
#define OBJID_PLAYER				(0)
#define OBJID_PCWOMAN2_01			(1)
#define OBJID_PCWOMAN2_02			(2)
#define OBJID_ENEMY					(3)			//�G1
#define OBJID_ENEMY2				(4)			//�G2
#define OBJID_MINE					(5)			//AI�F����
#define OBJID_PAIR					(6)			//AI�F�p�[�g�i�[
#define OBJID_SIO_USER_0			(7)			//�ʐM�F����
#define OBJID_SIO_USER_1			(8)			//�ʐM�F�p�[�g�i�[

//�X�N���v�gID(����Ή��\��)
#define EVENTID_TEST_SCR_OBJ1		(1)

//AI�}���`
_ACTOR_LABEL	default_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_DOWN, \
							PLAYER_X,PLAYER_Y,OFF/*�������g�A�N�^�[�Z�b�g*/
	//�W��
	_ACTOR_DATA				OBJID_PCWOMAN2_01,FSW_PARAM6,WF2DMAP_WAY_C_RIGHT, \
							PCWOMAN2_01_X,PCWOMAN2_01_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PCWOMAN2_02,FSW_PARAM6,WF2DMAP_WAY_C_RIGHT, \
							PCWOMAN2_02_X,PCWOMAN2_02_Y,ON,EVENTID_TEST_SCR_OBJ1
	//�����A�p�[�g�i�[
	_ACTOR_DATA				OBJID_MINE,FSW_PARAM0,WF2DMAP_WAY_C_UP, \
							MINE_X,MINE_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PAIR,FSW_PARAM1,WF2DMAP_WAY_C_UP, \
							PAIR_X,PAIR_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

//�ʐM�}���`
_ACTOR_LABEL	comm_multi_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_DOWN, \
							PLAYER_X,PLAYER_Y,OFF/*�������g�A�N�^�[�Z�b�g*/
	//�W��
	_ACTOR_DATA				OBJID_PCWOMAN2_01,FSW_PARAM6,WF2DMAP_WAY_C_RIGHT, \
							PCWOMAN2_01_X,PCWOMAN2_01_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PCWOMAN2_02,FSW_PARAM6,WF2DMAP_WAY_C_RIGHT, \
							PCWOMAN2_02_X,PCWOMAN2_02_Y,ON,EVENTID_TEST_SCR_OBJ1
	//�����A�p�[�g�i�[
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_UP, \
							SIO_USER_0_X,SIO_USER_0_Y,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_UP, \
							SIO_USER_1_X,SIO_USER_1_Y,ON
	_ACTOR_DATA_END

/*�G�g���[�i�[�̌���^�C�~���O���r���Ȃ̂ŕ�����*/
_ACTOR_LABEL	enemy_set_actor
	_ACTOR_DATA				OBJID_ENEMY,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							ENEMY_X,ENEMY_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_ENEMY2,FSW_PARAM3,WF2DMAP_WAY_C_DOWN, \
							ENEMY2_X,ENEMY2_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END


/********************************************************************/
//							�A�j��
/********************************************************************/

//--------------------------------------------------------------------
//���@/�p�[�g�i�[�@�����ʒu�ֈړ�
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_player_01
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_L_8F,3
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END
_ANIME_LABEL	anm_d31r0206_partner_01
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_L_8F,4
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�l�~�[/�G�l�~�[�Q�@�����ʒu�ֈړ�
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_enemy1_01
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,3
	_ANIME_DATA	FC_WALK_D_8F,3
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

_ANIME_LABEL	anm_d31r0206_enemy2_01
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,4
	_ANIME_DATA	FC_WALK_D_8F,2
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//���@/�p�[�g�i�[���ʁ@�ΐ�O�Ɉ���O��
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_player_02
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�l�~�[���ʁ@�ΐ�O�Ɉ���O��
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_enemy_02
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
/*�G�l�~�[���ʁ@�ΐ탁�b�Z�[�W�\�����ɑ�����*/
_ANIME_LABEL	anm_d31r0206_enemy_03
	_ANIME_DATA	FC_STAY_WALK_L_16F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�l�~�[/�G�l�~�[�Q�@�s���ޏ�
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_enemy1_03
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA_END
_ANIME_LABEL	anm_d31r0206_enemy2_03
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_WALK_L_8F,3
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//���@/�p�[�g�i�[�@�ΐ폟����@��������U�����
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_player_03
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�ē���@�ΐ폟����@�߂Â�
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_pcwoman2_01
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//���@/�p�[�g�i�[�@�񕜌�U�����
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_player_04
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�ē���@�񕜌㉺����
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_pcwoman2_02
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END
	
//--------------------------------------------------------------------
//���@�@��������
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0206_player_05
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END


/********************************************************************/
/*								�J�n								*/
/********************************************************************/
fss_tower_6_start:

	/*���[�N�m��*/
	_BATTLE_REC_INIT

	/*FLAG���x��*/
	_CALL				ev_d31r0206_flag_change

	/*OBJ���x��*/
	_CALL				ev_d31r0206_obj_change

	/*�u���낭����v�Ŏg�p���郏�[�N�Ȃ̂ŃN���A���Ă���*/
	_LDVAL				FSW_LOCAL6,0

	/*�ʐM�}���`*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,comm_tower_6_start
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,comm_tower_6_start

	/*AI�}���`*/
	_LDVAL				FSW_LOCAL1,OBJID_MINE
	_LDVAL				FSW_LOCAL2,OBJID_PAIR
	_LDVAL				FSW_PARAM6,BFSW1
	_CHAR_RESOURCE_SET	default_set_resource				/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_set_actor					/*�A�N�^�[�o�^*/
	_JUMP				comm_tower_6_start2
	_END

comm_tower_6_start:

	/*���s�Ɋ֌W�Ȃ��ʐM�t�����e�B�A�ɒ���(�ʐM�͒��f���Ȃ��̂Ő擪�ŏ������Ă����v)*/
	_SCORE_ADD			SCORE_ID_FRONTIER_COMM

	_LDVAL				FSW_LOCAL1,OBJID_SIO_USER_0
	_LDVAL				FSW_LOCAL2,OBJID_SIO_USER_1
	_LDVAL				FSW_PARAM6,BFSW1
	_CHAR_RESOURCE_SET	comm_multi_set_resource				/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			comm_multi_set_actor				/*�A�N�^�[�o�^*/
	_JUMP				comm_tower_6_start2
	_END

comm_tower_6_start2:
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*SCENE���x��*/
	_SAVE_EVENT_WORK_GET	WK_SCENE_D31R0206,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_roomd_common_entry
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_tower_roomd_common_continue
	_END


/********************************************************************/
/*					���߂ĕ����ɓ��ꂵ���Ƃ�						*/
/********************************************************************/
ev_tower_roomd_common_entry:
	//_LDVAL			WK_SCENE_D31R0206,3
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0206,3

	_CALL				ev_tower_roomd_comanm_in
	
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,ev_tower_roomd_siobtl_01
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,ev_tower_roomd_siobtl_01

	_JUMP				ev_tower_roomd_aibtl_01
	_END

//���@�ƃp�[�g�i�[����
ev_tower_roomd_comanm_in:
	_OBJ_ANIME			FSW_LOCAL1,anm_d31r0206_player_01
	_OBJ_ANIME			FSW_LOCAL2,anm_d31r0206_partner_01
	_OBJ_ANIME_WAIT
	_RET

ev_tower_roomd_siobtl_01:
	/*SIO�}���`�o�g���Ăяo���R����*/
	_CALL				ev_tower_roomd_common_siobtl_call

#ifndef DEBUG_BTL_LOSE_OFF	/********************************************/
#ifndef BTOWER_AUTO_DEB
	_IFVAL_JUMP			FSW_LOCAL3,EQ,0,ev_tower_roomd_common_lose
#endif
#endif	/****************************************************************/

	//���������̏���
	_CALL				ev_tower_roomd_common_win_param
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_roomd_common_clear_ret

	//�G�l�~�[�ޏ�
	_CALL				ev_tower_roomd_comanm_eneout

	//�v���C���[�U�����
	_CALL				ev_tower_roomd_comanm_btl_end

	//�|�P������
	_CALL				ev_tower_roomd_common_heal
	_JUMP				ev_tower_roomd_siobtl_02
	_END

//�A��
ev_tower_roomd_siobtl_02:
	_BTOWER_TOOLS		BTWR_SUB_GET_NOW_ROUND,BTWR_NULL_PARAM,FSW_ANSWER
	_NUMBER_NAME		0,FSW_ANSWER
	_TALKMSG			msg_tower_82

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP			FSW_LOCAL6,EQ,0,ev_tower_roomd_siobtl_02_rec

	_BMPLIST_INIT		24,13,0,0,FSW_ANSWER		//B�L�����Z������
	_BMPLIST_MAKE_LIST	msg_tower_choice_06,FSEV_WIN_TALK_MSG_NONE,0		//�Â���
	_JUMP				ev_tower_roomd_roomd_siobtl_02_sub
	_END

ev_tower_roomd_siobtl_02_rec:

	/*���C�A���X�ŁADP���܂܂�Ă�����A�L�^�����͂���Ȃ�*/
	_CALL				ev_tower_multi_dp_check
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_roomd_siobtl_03_rec	/*�L�^�Ȃ�*/

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_tower_roomd_siobtl_03_rec	/*�L�^�Ȃ�*/

	_BMPLIST_INIT		23,11,0,0,FSW_ANSWER		//B�L�����Z������
	_BMPLIST_MAKE_LIST	msg_tower_choice_06,FSEV_WIN_TALK_MSG_NONE,0		//�Â���
	_BMPLIST_MAKE_LIST	msg_tower_choice_13,FSEV_WIN_TALK_MSG_NONE,1		/*���낭����*/
	_JUMP				ev_tower_roomd_roomd_siobtl_02_sub
	_END

/*���C�A���X�ŁADP���܂܂�Ă�����A�L�^�����͂���Ȃ�*/
ev_tower_roomd_siobtl_03_rec:
	_BMPLIST_INIT		24,13,0,0,FSW_ANSWER		//B�L�����Z������
	_BMPLIST_MAKE_LIST	msg_tower_choice_06,FSEV_WIN_TALK_MSG_NONE,0		//�Â���
	_JUMP				ev_tower_roomd_roomd_siobtl_02_sub
	_END

ev_tower_roomd_roomd_siobtl_02_sub:
	_BMPLIST_MAKE_LIST	msg_tower_choice_08,FSEV_WIN_TALK_MSG_NONE,2		//���^�C�A
	_BMPLIST_START
	
	_TALK_CLOSE

	_LDVAL				FSW_LOCAL0,0				//���^�C�A���I�΂���1�ɂȂ�
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_tower_roomd_is_sio_retire

	/*���낭����*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_roomd_is_sio_rec

	_JUMP				ev_tower_roomd_siobtl_03
	_END

//�����邩�ǂ����̑I��(LOCAL0��1�Ȃ烊�^�C�A)
ev_tower_roomd_siobtl_03:
	/*���[�N���N���A���Ă���*/
	_LDVAL				FSW_LOCAL6,0

	//���F�B�̑I����҂��Ă��܂�
	_TALKMSG			msg_tower_114
	//�I��҂��ʐM����
	_COMM_RESET
	_COMM_SYNCHRONIZE	TOWER_COMM_MULTI_NEXT_SELECT

	_BTOWER_TOOLS		BTWR_SUB_RECV_BUF_CLEAR,BTWR_NULL_PARAM,FSW_ANSWER
	_JUMP				ev_tower_roomd_siobtl_03_retry
	_END

/*���M���s������đ��M*/
ev_tower_roomd_siobtl_03_retry:
	//�I�����ʑ��M
	_BTOWER_SEND_BUF	2,FSW_LOCAL0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_tower_roomd_siobtl_03_retry

	//�I�����ʎ�M
	_BTOWER_RECV_BUF	2,FSW_LOCAL0

	_TALK_CLOSE
	_SWITCH				FSW_LOCAL0	
	_CASE_JUMP			1,ev_tower_roomd_sio_retire	//���^�C�A

	//�A���I��
	//�ē�����Ƃ̈ʒu��
	_CALL				ev_tower_roomd_comanm_guide_out

	//�o�g��
	_JUMP				ev_tower_roomd_siobtl_01
	_END


/********************************************************************/
/*					�}���`�ΐ핔��in	�R����						*/
/********************************************************************/
//��������ޏꂵ�Ď󂯕t���ɖ߂�
ev_tower_roomd_common_exit:
	/*���[�N�J��*/
	_BATTLE_REC_EXIT

	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,ev_tower_roomd_sio_exit_multi
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,ev_tower_roomd_sio_exit

	//�E�F�C�g
	_TIME_WAIT			30,FSW_ANSWER

	//��t�߂�t���O�Z�b�g
	//_LDVAL			WK_SCENE_D31R0201,1
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0201,1

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_ACTOR_FREE			OBJID_PLAYER
	_ACTOR_FREE			OBJID_PCWOMAN2_01
	_ACTOR_FREE			OBJID_PCWOMAN2_02
	_ACTOR_FREE			OBJID_MINE
	_ACTOR_FREE			OBJID_PAIR
	_CHAR_RESOURCE_FREE	FSW_PARAM6
	//_CHAR_RESOURCE_FREE	FSW_PARAM0
	_CHAR_RESOURCE_FREE	FSW_PARAM1

	//_MAP_CHANGE			ZONE_ID_D31R0201,0,7,6,DIR_UP
	//_MAP_CHANGE			FSS_SCENEID_TOWER
	//_END

	_SCRIPT_FINISH			/*_END����2D�}�b�v�I��*/

/*�}���`�̂�*/
ev_tower_roomd_sio_exit_multi:
	/*TV����*/
	_TV_TEMP_FRIEND_SET	FRONTIER_NO_TOWER
	_JUMP				ev_tower_roomd_sio_exit
	_END

//�ʐM�}���`��������o�Ė߂�
ev_tower_roomd_sio_exit:

	//�E�F�C�g
	_TIME_WAIT			30,FSW_ANSWER
	
	//��t�߂�t���O�Z�b�g
	//_LDVAL			WK_SCENE_D31R0201,1
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0201,1
	
	//�ʐM����
	_COMM_RESET
	_COMM_SYNCHRONIZE	TOWER_COMM_MULTI_EXIT_WAIT

	/*���b�Z�[�W�E�B���h�E���폜���锻�ʗp*/
	_IFVAL_CALL			FSW_LOCAL4,EQ,100,ev_tower_roomd_sio_exit_close

	//WIFI(�|�P�Z���n��)�̂�
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,comm_tower_roomd_common_report_wifi

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	
	//�ʐM����
	_COMM_RESET
	_COMM_SYNCHRONIZE	TOWER_COMM_MULTI_SIO_END

	/*�ʐM�}���`(���C�A���X�ʐM�̂ݒʐM�ؒf����)*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,comm_tower_6_direct_end
	//_IFVAL_CALL			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,comm_tower_6_direct_end
	
	_ACTOR_FREE			OBJID_PLAYER
	_ACTOR_FREE			OBJID_PCWOMAN2_01
	_ACTOR_FREE			OBJID_PCWOMAN2_02
	_ACTOR_FREE			OBJID_SIO_USER_0
	_ACTOR_FREE			OBJID_SIO_USER_1
	_CHAR_RESOURCE_FREE	FSW_PARAM6
	//_CHAR_RESOURCE_FREE	FSW_PARAM0
	_CHAR_RESOURCE_FREE	FSW_PARAM1

	//_MAP_CHANGE_NONE_FADE	ZONE_ID_D31R0201,0,7,6,DIR_UP
	//_MAP_CHANGE_EX		FSS_SCENEID_TOWER,0

	//_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK

	//WIFI(�|�P�Z���n��)�̂�
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,comm_tower_roomd_common_exit_wifi

	//_END
	_SCRIPT_FINISH			/*_END����2D�}�b�v�I��*/

/*���b�Z�[�W�E�B���h�E���폜���锻�ʗp*/
ev_tower_roomd_sio_exit_close:
	_TALK_CLOSE
	_RET


/********************************************************************/
/*						WIFI�͑҂����킹��ʂ֖߂�					*/
//
/*	WiFi�Ăяo���̎��͂����ŏI�����Ă��܂��̂ŁA					*/
/*	�����Ń��[�h�Ń��[�N�̊J�����K�v								*/
/********************************************************************/
comm_tower_roomd_common_exit_wifi:
	_FR_WIFI_COUNTER_TOWER_CALL_AFTER

	_MAP_CHANGE_EX		FSS_SCENEID_WIFI_COUNTER,1
	_END

//WIFI(�|�P�Z���n��)�̂�
comm_tower_roomd_common_report_wifi:
	_TALKMSG_ALL_PUT	msg_tower_117
	_ADD_WAITICON
	_REPORT_DIV_SAVE	FSW_ANSWER
	_DEL_WAITICON
	_SE_PLAY			SEQ_SE_DP_SAVE
	_SE_WAIT			SEQ_SE_DP_SAVE
	_TALK_CLOSE
	_RET


/********************************************************************/
/*					���C�A���X�ʐM�͒ʐM�ؒf						*/
/********************************************************************/
comm_tower_6_direct_end:
	_COMM_DIRECT_END
	_RET


/********************************************************************/
/*					��������ŕ����ɓ��ꂵ����						*/
/********************************************************************/
ev_tower_roomd_common_continue:
	//_LDVAL			WK_SCENE_D31R0206,3
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0206,3
	
	/*�L�^�������[�N�Ƀf�[�^���������܂�Ă��Ȃ��̂Ń��j���[�\�����Ȃ�*/
	_LDVAL				FSW_LOCAL6,1

	_CALL				ev_tower_roomd_comanm_in
	_CALL				ev_tower_roomd_comanm_continue

	//�u���́H�l�ڂ́c�c�v
	_JUMP				ev_tower_roomd_aibtl_02
	_END

//���@�ƃp�[�g�i�[/�󂯕t����@��������n�߂��Ƃ��̓����
ev_tower_roomd_comanm_continue:
	_OBJ_ANIME			OBJID_PCWOMAN2_01,anm_d31r0206_pcwoman2_01
	_OBJ_ANIME			OBJID_PCWOMAN2_02,anm_d31r0206_pcwoman2_01
	_OBJ_ANIME			FSW_LOCAL1,anm_d31r0206_player_05
	_OBJ_ANIME			FSW_LOCAL2,anm_d31r0206_player_05
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*					�ʏ�A�� ���́H�l�ڂ̑���ł�					*/
/********************************************************************/
ev_tower_roomd_aibtl_02:
	_BTOWER_TOOLS		BTWR_SUB_GET_NOW_ROUND,BTWR_NULL_PARAM,FSW_ANSWER
	_NUMBER_NAME		0,FSW_ANSWER
	_TALKMSG			msg_tower_82

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP			FSW_LOCAL6,EQ,0,ev_tower_roomd_aibtl_02_rec

	_BMPLIST_INIT		24,11,0,0,FSW_ANSWER			//B�L�����Z������
	_BMPLIST_MAKE_LIST	msg_tower_choice_06,FSEV_WIN_TALK_MSG_NONE,0			//�Â���
	_JUMP				ev_tower_roomd_aibtl_02_sub
	_END

ev_tower_roomd_aibtl_02_rec:
	_BMPLIST_INIT		23,9,0,0,FSW_ANSWER				//B�L�����Z������
	_BMPLIST_MAKE_LIST	msg_tower_choice_06,FSEV_WIN_TALK_MSG_NONE,0			//�Â���
	_BMPLIST_MAKE_LIST	msg_tower_choice_13,FSEV_WIN_TALK_MSG_NONE,1			/*���낭����*/
	_JUMP				ev_tower_roomd_aibtl_02_sub
	_END

ev_tower_roomd_aibtl_02_sub:
	_BMPLIST_MAKE_LIST	msg_tower_choice_07,FSEV_WIN_TALK_MSG_NONE,2			//�₷��
	_BMPLIST_MAKE_LIST	msg_tower_choice_08,FSEV_WIN_TALK_MSG_NONE,3			//���^�C�A
	_BMPLIST_START
	
	_TALK_CLOSE

	_SWITCH				FSW_ANSWER
	_CASE_JUMP			0,ev_tower_roomd_ai_next		//�Â���
	_CASE_JUMP			1,ev_tower_roomd_is_ai_rec		/*���낭����*/
	_CASE_JUMP			2,ev_tower_roomd_is_ai_rest		//�₷��
	_CASE_JUMP			3,ev_tower_roomd_is_ai_retire	//���^�C�A
	_JUMP				ev_tower_roomd_ai_next			//�Â���
	_END


/********************************************************************/
/*							�Â���								*/
/********************************************************************/
ev_tower_roomd_ai_next:
	//�ē�����Ƃ̈ʒu��
	_CALL				ev_tower_roomd_comanm_guide_out

	/*���[�N���N���A���Ă���*/
	_LDVAL				FSW_LOCAL6,0

	//�o�g��
	_JUMP				ev_tower_roomd_aibtl_01
	_END

//�񕜌�@�v���C���[�U��������ē��쉺����
ev_tower_roomd_comanm_guide_out:
	_OBJ_ANIME			FSW_LOCAL1,anm_d31r0206_player_04
	_OBJ_ANIME			FSW_LOCAL2,anm_d31r0206_player_04
	_OBJ_ANIME			OBJID_PCWOMAN2_01,anm_d31r0206_pcwoman2_02
	_OBJ_ANIME			OBJID_PCWOMAN2_02,anm_d31r0206_pcwoman2_02
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*						�L�^����(AI�}���`)							*/
/********************************************************************/
ev_tower_roomd_is_ai_rec:
	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower6_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_tower6_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_tower_roomd_aibtl_02

	_CALL			ev_tower6_room_rec_win		/*�����̋L�^*/
	_JUMP			ev_tower_roomd_aibtl_02
	_END


/********************************************************************/
/*							���ʋL�^*/
/********************************************************************/
/*����*/
ev_tower6_room_rec_win:
	_CALL				ev_tower6_room_rec_common		/*ANSWER,LOCAL5�g�p��*/
	//_ADD_WK			FSW_LOCAL5,1
	//_SUB_WK			FSW_LOCAL5,1					/*���E���h���ł͂Ȃ��������Ȃ̂�*/
	_JUMP				ev_tower6_room_rec
	_END

/*����*/
ev_tower6_room_rec_lose:
	_CALL				ev_tower6_room_rec_common		/*ANSWER,LOCAL5�g�p��*/
	_ADD_WK				FSW_LOCAL5,1
	_JUMP				ev_tower6_room_rec
	_END

/*���ʕ���*/
ev_tower6_room_rec_common:
	/*�^��f�[�^�Z�[�u*/
	_BTOWER_TOOLS		BTWR_SUB_GET_RENSHOU_CNT,BTWR_NULL_PARAM,FSW_LOCAL5		/*�A����*/
	_RET

ev_tower6_room_rec:
	_JUMP				ev_tower6_room_rec_sub
	_END

ev_tower6_room_rec_sub:
	/*�L�^�������[�N�Z�b�g*/
	_LDVAL				FSW_LOCAL6,1

	_TALKMSG_ALL_PUT	msg_tower_101_05
	/*BTWR_MODE_MULTI(AI�}���`�̂���),BTWR_MODE_COMM_MULTI,BTWR_MODE_WIFI_MULTI*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER		/*���[�h*/
	_ADD_WAITICON
	_BATTLE_REC_SAVE	FRONTIER_NO_TOWER,FSW_ANSWER,FSW_LOCAL5,FSW_LOCAL5
	_DEL_WAITICON

	//_BATTLE_REC_LOAD

	_IFVAL_JUMP			FSW_LOCAL5,EQ,1,ev_tower6_room_rec_true

	/*�u�L�^�o���܂���ł����v*/
	_TALKMSG			msg_tower_101_03
	_RET

ev_tower6_room_rec_true:
	_SE_PLAY			SEQ_SE_DP_SAVE
	/*�u�L�^����܂����v*/
	_PLAYER_NAME		0
	_TALKMSG			msg_tower_101_02
	_RET
	
/*�^��f�[�^���Ȃ������b�Z�[�W*/
ev_tower6_room_rec_msg1:
	_TALKMSG			msg_tower_101_01
	_RET

/*���łɘ^��f�[�^�����鎞���b�Z�[�W*/
ev_tower6_room_rec_msg2:
	_TALKMSG			msg_tower_101_04
	_RET


/********************************************************************/
/*				AI�}���`���f���ċx�ނ��ǂ����H						*/
/********************************************************************/
ev_tower_roomd_is_ai_rest:
	_TALKMSG			msg_tower_83
	_YES_NO_WIN			FSW_ANSWER

	_TALK_CLOSE

	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_tower_roomd_common_rest
	_JUMP				ev_tower_roomd_aibtl_02
	_END

//�x�ނƂ��̏���
ev_tower_roomd_common_rest:
	//_LDVAL			WK_SCENE_D31R0201,2
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0201,2

	//�v���C�f�[�^�Z�[�u
	_BTOWER_TOOLS		BTWR_SUB_SAVE_REST_PLAY_DATA,BTWR_NULL_PARAM,FSW_ANSWER
	_BTOWER_WORK_RELEASE

	_TALKMSG			msg_tower_79
	_ADD_WAITICON
	_REPORT_SAVE		FSW_ANSWER
	//_REPORT_DIV_SAVE	FSW_ANSWER				/*�ʐM���Ă��Ȃ�(AI�}���`�̏����̗���)*/
	_DEL_WAITICON
	_SE_PLAY			SEQ_SE_DP_SAVE
	_SE_WAIT			SEQ_SE_DP_SAVE

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_TALK_CLOSE

	/*���[�N�J��*/
	_BATTLE_REC_EXIT

	//���Z�b�g�R�}���h
	_BTOWER_TOOLS		BTWR_TOOL_SYSTEM_RESET,BTWR_NULL_PARAM,FSW_ANSWER
	_END


/********************************************************************/
/*							���^�C�A								*/
/********************************************************************/
//AI�}���`���^�C�A���邩�ǂ����H
ev_tower_roomd_is_ai_retire:
	_TALKMSG			msg_tower_84
	_YES_NO_WIN_EX		FSW_ANSWER

	_TALK_CLOSE

	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_tower_roomd_common_retire
	_JUMP				ev_tower_roomd_aibtl_02
	_END

//���^�C�A����Ƃ��̏���(�^�悷�闬��Ȃ�)
ev_tower_roomd_common_retire:
	//�s��p�����[�^�Z�b�g
	_BTOWER_TOOLS		BTWR_SUB_SET_LOSE_SCORE,BTWR_NULL_PARAM,FSW_ANSWER

	//�����Ă���LOCALWORk0��0����
	_LDVAL				FSW_LOCAL0,0

	_JUMP				ev_tower_roomd_common_exit
	_END

//�����Ė߂�Ƃ��̏���
ev_tower_roomd_common_lose:

	//�����Ă���LOCALWORk0��0����
	_LDVAL				FSW_LOCAL0,0

	_JUMP				ev_tower_roomd_common_rec_lose
	_END


/********************************************************************/
/*				�u�����قǂ́@�킢���@�L�^���܂����H�v				*/
/********************************************************************/
ev_tower_roomd_common_rec_win:

#if 1
	/*�ʐM�}���`*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,comm_tower_roomd_common_rec_win_check
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,comm_tower_roomd_common_rec_win_check
	_JUMP				ev_tower_roomd_common_rec_ok_win
	_END

comm_tower_roomd_common_rec_win_check:
	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_tower_roomd_common_rec_win_sub	/*�L�^�Ȃ�*/

	/*���C�A���X�ŁADP���܂܂�Ă�����A�L�^�����͂���Ȃ�*/
	_CALL				ev_tower_multi_dp_check
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_roomd_common_rec_win_sub	/*�L�^�Ȃ�*/

	_JUMP				ev_tower_roomd_common_rec_ok_win
	_END

#endif

ev_tower_roomd_common_rec_ok_win:
	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower6_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_tower6_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX		FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower6_win_rec_yes		/*�u�͂��v*/
	_JUMP				ev_tower_roomd_common_rec_win_sub
	_END

ev_tower_roomd_common_rec_win_sub:

	//�N���A�p�����[�^�Z�b�g
	_BTOWER_TOOLS		BTWR_SUB_SET_CLEAR_SCORE,BTWR_NULL_PARAM,FSW_ANSWER

	/*�u�V��˔j���߂łƂ��v*/
	_TALKMSG			msg_tower_115

	/*�u�v���C���[�͂a�o����������v*/
	_BTOWER_TOOLS		BTWR_SUB_ADD_BATTLE_POINT,BTWR_NULL_PARAM,FSW_ANSWER
	_NUMBER_NAME		1,FSW_ANSWER
	_PLAYER_NAME		0
	_TALKMSG			msg_tower_116
	_ME_PLAY			ME_BP_GET
	_ME_WAIT
	
	/*�ʐM�̎��͕K���u���΂炭���܂����������v�\��*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,ev_tower_roomd_common_rec_win_sub3
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,ev_tower_roomd_common_rec_win_sub3

	_TALK_CLOSE
	_JUMP				ev_tower_roomd_common_exit
	_END

ev_tower_roomd_common_rec_win_sub3:	/*�L�^�Ȃ��̗���*/
	/*���b�Z�[�W�E�B���h�E���폜���锻�ʗp*/
	_LDVAL				FSW_LOCAL4,100
	_TALKMSG_NOSKIP		msg_tower_118
	_JUMP				ev_tower_roomd_common_exit
	_END

ev_tower_roomd_common_rec_lose:

	/*���b�Z�[�W�E�B���h�E���폜���锻�ʗp���N���A*/
	_LDVAL				FSW_LOCAL4,0

#if 1
	/*�ʐM�}���`*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,comm_tower_roomd_common_rec_lose_check
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,comm_tower_roomd_common_rec_lose_check
	_JUMP				ev_tower_roomd_common_rec_ok_lose
	_END

comm_tower_roomd_common_rec_lose_check:
	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_tower_roomd_common_rec_lose_sub2	/*�L�^�Ȃ�*/

	/*���C�A���X�ŁADP���܂܂�Ă�����A�L�^�����͂���Ȃ�*/
	_CALL				ev_tower_multi_dp_check
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_roomd_common_rec_lose_sub	/*�L�^�Ȃ�*/

	_JUMP				ev_tower_roomd_common_rec_ok_lose
	_END

#endif

/*�u���΂炭���܂����������v*/
ev_tower_roomd_common_rec_lose_sub2:
	/*���b�Z�[�W�E�B���h�E���폜���锻�ʗp*/
	_LDVAL				FSW_LOCAL4,100
	_TALKMSG_NOSKIP		msg_tower_118
	_JUMP				ev_tower_roomd_common_rec_lose_sub
	_END

/*�u���낭���܂����H�v*/
ev_tower_roomd_common_rec_ok_lose:
	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower6_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_tower6_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX		FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower6_lose_rec_yes		/*�u�͂��v*/

	/*�ʐM�}���`*/
	/*�u���΂炭���܂����������v�\����*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,ev_tower_roomd_common_rec_lose_sub2
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,ev_tower_roomd_common_rec_lose_sub2

	_TALK_CLOSE
	_JUMP				ev_tower_roomd_common_rec_lose_sub
	_END

ev_tower_roomd_common_rec_lose_sub:

	//�s��p�����[�^�Z�b�g
	_BTOWER_TOOLS		BTWR_SUB_SET_LOSE_SCORE,BTWR_NULL_PARAM,FSW_ANSWER

	_JUMP				ev_tower_roomd_common_exit
	_END

/*�u�͂��v*/
ev_tower6_lose_rec_yes:
	_CALL				ev_tower6_room_rec_lose		/*�s�k�̋L�^*/
	_RET

ev_tower6_win_rec_yes:
	_CALL				ev_tower6_room_rec_win		/*�����̋L�^*/
	_RET


/********************************************************************/
/*							AI�}���`�ΐ�							*/
/********************************************************************/
ev_tower_roomd_aibtl_01:
	_CALL				ev_tower_roomd_common_aibtl_call

#ifndef DEBUG_BTL_LOSE_OFF	/********************************************/
#ifndef BTOWER_AUTO_DEB
	_IFVAL_JUMP			FSW_LOCAL3,EQ,0,ev_tower_roomd_common_lose
#endif
#endif	/****************************************************************/

	//���������̏���(�N���A���Ă邩�ǂ�����FSW_ANSWER�ɂ�����)
	_CALL				ev_tower_roomd_common_win_param
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_roomd_common_clear_ret

	//�G�l�~�[�ޏ�
	_CALL				ev_tower_roomd_comanm_eneout

	//�v���C���[�U�����
	_CALL				ev_tower_roomd_comanm_btl_end

	//�|�P������
	_CALL				ev_tower_roomd_common_heal

	_JUMP				ev_tower_roomd_aibtl_02
	_END

//�o�g���������p�����[�^�Z�b�g�R����
ev_tower_roomd_common_win_param:
	//���E���h���X�V
	_BTOWER_TOOLS		BTWR_SUB_INC_ROUND,BTWR_NULL_PARAM,FSW_ANSWER

	//�N���A���Ă邩�`�F�b�N
	_BTOWER_TOOLS		BTWR_SUB_IS_CLEAR,BTWR_NULL_PARAM,FSW_ANSWER
	_RET

//�N���A���Ė߂�Ƃ��̏���
ev_tower_roomd_common_clear_ret:
	//�����Ă���LOCAL0��1����
	_LDVAL				FSW_LOCAL0,1

	_JUMP				ev_tower_roomd_common_rec_win
	_END

//�v���C���[������@�G�l�~�[�ޏ�
ev_tower_roomd_comanm_eneout:
	_OBJ_ANIME			OBJID_ENEMY,anm_d31r0206_enemy1_03
	_OBJ_ANIME			OBJID_ENEMY2,anm_d31r0206_enemy2_03
	_OBJ_ANIME_WAIT

	/*�ΐ�g���[�i�[�폜*/
	_ACTOR_FREE			OBJID_ENEMY
	_ACTOR_FREE			OBJID_ENEMY2
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_CHAR_RESOURCE_FREE	FSW_PARAM3

	//�o�j�b�V���t���OON
	//_FLAG_SET			FV_OBJID_ENEMY
	//_FLAG_SET			FV_OBJID_ENEMY2
	//_OBJ_DEL			OBJID_ENEMY
	//_OBJ_DEL			OBJID_ENEMY2
	_RET

//�v���C���[������@�v���C���[�U�����&�ē���߂Â�
ev_tower_roomd_comanm_btl_end:
	_OBJ_ANIME			FSW_LOCAL1,anm_d31r0206_player_03
	_OBJ_ANIME			FSW_LOCAL2,anm_d31r0206_player_03
	_OBJ_ANIME			OBJID_PCWOMAN2_01,anm_d31r0206_pcwoman2_01
	_OBJ_ANIME			OBJID_PCWOMAN2_02,anm_d31r0206_pcwoman2_01
	_OBJ_ANIME_WAIT
	_RET

ev_tower_roomd_common_heal:
	_TALKMSG			msg_tower_81
	_ME_PLAY			SEQ_ASA
	_ME_WAIT
	_PC_KAIFUKU
	_RET


/********************************************************************/
/*					AI�}���`�o�g���Ăяo���R����					*/
/********************************************************************/
ev_tower_roomd_common_aibtl_call:
	//���I
	_BTOWER_TOOLS		BTWR_SUB_CHOICE_BTL_PARTNER,BTWR_NULL_PARAM,FSW_ANSWER

	//�G�l�~�[����
	_CALL				ev_tower_roomd_comanm_enein

	//�ΐ�O�䎌
	_CALL				ev_tower_roomd_comanm_ene1_msganm
	_TALKMSG_BTOWER_APPEAR	0
	_AB_KEYWAIT
	_TALK_CLOSE

	_CALL				ev_tower_roomd_comanm_ene2_msganm
	_TALKMSG_BTOWER_APPEAR	1
	_AB_KEYWAIT
	_TALK_CLOSE

	//�݂��ɋ߂Â�
	_CALL				ev_tower_roomd_comanm_btl_start

	//_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK
	_ENCOUNT_EFFECT		FR_ENCOUNT_EFF_WIPE

#ifndef DEBUG_BTL_OFF	/************************************************/
	/*�퓬�Ăяo��*/
	//_BTOWER_TOOLS		BTWR_SUB_LOCAL_BTL_CALL,BTWR_NULL_PARAM,FSW_ANSWER
	_BTOWER_BATTLE_CALL
	//_TRAINER_LOSE_CHECK FSW_ANSWER
	//_LDWK				FSW_PARAM3,FSW_ANSWER
	_BTOWER_CALL_GET_RESULT	FSW_LOCAL3
#endif	/****************************************************************/

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_RET

//�G�l�~�[����
ev_tower_roomd_comanm_enein:
	//�L�����w��
	_BTOWER_TOOLS		BTWR_SUB_GET_ENEMY_OBJ,0,FSW_PARAM2
	_BTOWER_TOOLS		BTWR_SUB_GET_ENEMY_OBJ,1,FSW_PARAM3
	//_BTOWER_TOOLS		BTWR_SUB_GET_ENEMY_OBJ,0,FSW_ANSWER
	//_LDWK				OBJCHRWORK2,FSW_ANSWER
	//_BTOWER_TOOLS		BTWR_SUB_GET_ENEMY_OBJ,1,FSW_ANSWER
	//_LDWK				OBJCHRWORK3,FSW_ANSWER

	_CHAR_RESOURCE_SET	enemy_set_resource				/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			enemy_set_actor					/*�A�N�^�[�o�^*/

	//�o�j�b�V���t���OOFF
	//_FLAG_RESET		FV_OBJID_ENEMY
	//_FLAG_RESET		FV_OBJID_ENEMY2
	//_OBJ_ADD			OBJID_ENEMY
	//_OBJ_ADD			OBJID_ENEMY2
	_OBJ_ANIME			OBJID_ENEMY,anm_d31r0206_enemy1_01	
	_OBJ_ANIME			OBJID_ENEMY2,anm_d31r0206_enemy2_01	
	_OBJ_ANIME_WAIT
	_RET

//�o�g���O�@�݂��Ɉ���߂Â�
ev_tower_roomd_comanm_btl_start:
	_OBJ_ANIME			FSW_LOCAL1,anm_d31r0206_player_02
	_OBJ_ANIME			FSW_LOCAL2,anm_d31r0206_player_02
	_OBJ_ANIME			OBJID_ENEMY,anm_d31r0206_enemy_02
	_OBJ_ANIME			OBJID_ENEMY2,anm_d31r0206_enemy_02
	_OBJ_ANIME_WAIT
	_RET

/*�o�g���O���b�Z�[�W�\�����ɂ��̏ꑫ����*/
ev_tower_roomd_comanm_ene1_msganm:
	_OBJ_ANIME			OBJID_ENEMY,anm_d31r0206_enemy_03
	_OBJ_ANIME_WAIT
	_RET

ev_tower_roomd_comanm_ene2_msganm:
	_OBJ_ANIME			OBJID_ENEMY2,anm_d31r0206_enemy_03
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*				SIO�}���`�o�g���Ăяo���R����						*/
/********************************************************************/
ev_tower_roomd_common_siobtl_call:
	//���I
	_BTOWER_TOOLS		BTWR_SUB_CHOICE_BTL_PARTNER,BTWR_NULL_PARAM,FSW_ANSWER

	//�G�l�~�[����
	_CALL				ev_tower_roomd_comanm_enein

	//�ΐ�O�䎌
	_CALL				ev_tower_roomd_comanm_ene1_msganm
	_TALKMSG_BTOWER_APPEAR	0
	_TIME_WAIT			30,FSW_ANSWER
	_TALK_CLOSE

	_CALL				ev_tower_roomd_comanm_ene2_msganm
	_TALKMSG_BTOWER_APPEAR	1
	_TIME_WAIT			30,FSW_ANSWER
	_TALK_CLOSE
	
	//�݂��ɋ߂Â�
	_CALL				ev_tower_roomd_comanm_btl_start

	/*�u���΂炭���܂����������v*/
	_TALKMSG_NOSKIP		msg_tower_118

	//�ʐM����
	_COMM_RESET
	_COMM_SYNCHRONIZE	TOWER_COMM_MULTI_BATTLE_START
	
	_TALK_CLOSE
	_ENCOUNT_EFFECT		FR_ENCOUNT_EFF_WIPE

#ifndef DEBUG_BTL_OFF	/************************************************/
	/*�퓬�Ăяo��*/
	//_BTOWER_TOOLS		BTWR_SUB_LOCAL_BTL_CALL,BTWR_NULL_PARAM,FSW_ANSWER
	_BTOWER_BATTLE_CALL
	//_TRAINER_LOSE_CHECK FSW_ANSWER
	//_LDWK				FSW_PARAM3,FSW_ANSWER
	_BTOWER_CALL_GET_RESULT	FSW_LOCAL3
#endif	/****************************************************************/

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_RET

//�{���Ƀ��^�C�A��I�����邩�ǂ����H
ev_tower_roomd_is_sio_retire:
	_TALKMSG			msg_tower_84
	_YES_NO_WIN_EX		FSW_ANSWER

	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_roomd_siobtl_02

	//�L�����Z�����I�΂ꂽ�̂�FSW_LOCAL0��1����
	_LDVAL				FSW_LOCAL0,1
	_JUMP				ev_tower_roomd_siobtl_03
	_END

//�{���Ƀ��^�C�A
ev_tower_roomd_sio_retire:
	_TALKMSG			msg_tower_113

	//�E�F�C�g
	_TIME_WAIT			30,FSW_ANSWER
	_COMM_RESET
	_COMM_SYNCHRONIZE	TOWER_COMM_MULTI_RETIRE_WAIT

	_TALK_CLOSE
	_JUMP				ev_tower_roomd_common_retire	//���^�C�A
	_END


/********************************************************************/
/*						�L�^����(�ʐM)								*/
/********************************************************************/
ev_tower_roomd_is_sio_rec:

	/*���C�A���X�ŁADP���܂܂�Ă�����A�L�^�����͂���Ȃ�*/
	_CALL				ev_tower_multi_dp_check
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_roomd_siobtl_02	/*�L�^�Ȃ�*/

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_tower_roomd_siobtl_02	/*�L�^�Ȃ�*/

	_JUMP				ev_tower_roomd_is_sio_rec_sub
	_END

ev_tower_roomd_is_sio_rec_sub:
	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_tower6_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_tower6_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_tower_roomd_siobtl_02

	_CALL			ev_tower6_room_rec_win		/*�����̋L�^*/
	_JUMP			ev_tower_roomd_siobtl_02
	_END


/********************************************************************/
/*						FLAG_CHANGE_LABEL 							*/
/********************************************************************/
ev_d31r0206_flag_change:
	_SAVE_EVENT_WORK_GET	WK_SCENE_D31R0206,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_d31r0206_flag_change_end
	//_FLAG_SET			FV_D31R0206_TRAINER01
	//_FLAG_SET			FV_D31R0206_TRAINER02

	/*
	  �ʐM�̎q�̎��͐e�q�̍��E�ʒu�����ꂩ����
	  �J�����gID�擾�֐��͔�ڑ����ɌĂԂ�0���Ԃ�
	*/

	//_COMM_GET_CURRENT_ID	FSW_LOCAL0
	_IFVAL_JUMP			FSW_LOCAL0,EQ,1,ev_d31r0206_flag_change_child
	
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_PARAM0
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_PARAM1

	/*��ڑ����Ɛe�̂Ƃ�*/
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_PARAM2
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_LOCAL0
	//_LDWK				OBJCHRWORK0,FSW_LOCAL0
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_PARAM3
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_LOCAL0
	//_LDWK				OBJCHRWORK1,FSW_LOCAL0
	_RET

ev_d31r0206_flag_change_end:
	_RET

/*�ʐM�̎q�̎�*/
ev_d31r0206_flag_change_child:
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_PARAM0
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_PARAM1

	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_PARAM2
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_LOCAL0
	//_LDWK				OBJCHRWORK1,FSW_LOCAL0
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_PARAM3
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_LOCAL0
	//_LDWK				OBJCHRWORK0,FSW_LOCAL0
	_RET


/********************************************************************/
/*						OBJ_CHANGE_LABEL 							*/
/********************************************************************/
ev_d31r0206_obj_change:
	_SAVE_EVENT_WORK_GET	WK_SCENE_D31R0206,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,NE,0,ev_d31r0206_obj_player
	_JUMP				ev_d31r0206_obj_change_02
	_END

ev_d31r0206_obj_change_02:
	_SAVE_EVENT_WORK_GET	WK_SCENE_D31R0206,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,3,ev_d31r0206_obj_pos_change_set
	_JUMP				ev_d31r0206_obj_char_set
	_END

ev_d31r0206_obj_player:
	//_OBJ_INVISIBLE	OBJID_MINE
	//���}�X���炷
	//_PLAYER_POS_OFFSET_SET	8,0,0
	_RET

ev_d31r0206_obj_pos_change_set:
	//_OBJ_POS_CHANGE	OBJID_MINE,5,0,5,DIR_RIGHT
	//_OBJ_POS_CHANGE	OBJID_PAIR,5,0,6,DIR_RIGHT
	_JUMP				ev_d31r0206_obj_char_set
	_END

ev_d31r0206_obj_char_set:
	_RET


/********************************************************************/
/*		���C�A���X�ŁADP���܂܂�Ă�����A�L�^�����͂���Ȃ�		*/
/********************************************************************/
ev_tower_multi_dp_check:
	/*���C�A���X�̎�*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,ev_tower_roomd_is_sio_rec_check1

	_LDVAL				FSW_ANSWER,0
	_RET

/*���C�A���X�ŁADP���܂܂�Ă�����A�L�^�����͂���Ȃ�*/
ev_tower_roomd_is_sio_rec_check1:
	_CHECK_DP_ROM_CODE	FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_tower_multi_dp_check_ari

	_LDVAL				FSW_ANSWER,0
	_RET

ev_tower_multi_dp_check_ari:
	_LDVAL				FSW_ANSWER,1		/*DP����*/
	_RET



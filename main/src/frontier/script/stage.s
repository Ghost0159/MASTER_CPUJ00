//==============================================================================
/**
 * @file	stage.s
 * @brief	�u�o�g���X�e�[�W�v�t�����e�B�A�X�N���v�g
 * @author	nohara
 * @date	2007.06.11
 */
//==============================================================================

	.text

	.include	"../frontier_def.h"
	.include	"../stage_def.h"
	.include	"../frontier_seq_def.h"
	.include	"../../../include/msgdata/msg_stage_room.h"
	.include	"../../../include/field/evwkdef.h"
	.include	"../../fielddata/script/saveflag.h"
	.include	"../../fielddata/script/savework.h"
	.include	"../../field/syswork_def.h"
	.include	"../../particledata/pl_frontier/frontier_particle_def.h"	//SPA
	.include	"../../particledata/pl_frontier/frontier_particle_lst.h"	//EMIT


//--------------------------------------------------------------------
//					     �X�N���v�g�{��
//
//	FSW_PARAM0	���X�g�Ŏg�p
//	FSW_PARAM1	��l���̌����ڊi�[
//	FSW_PARAM2	�G�g���[�i�[1��OBJ�R�[�h
//	FSW_PARAM3	�G�g���[�i�[2��OBJ�R�[�h
//	FSW_PARAM5	����
//	FSW_PARAM6	�U������OBJ�R�[�h
//	FSW_LOCAL3	�L�^������
//	FSW_LOCAL5	�L�^�v�Z�p
//	FSW_LOCAL6	�ʐM��`
//
//--------------------------------------------------------------------
_EVENT_DATA		fss_stage_start	//��ԏ��EVENT_DATA�͎������s
_EVENT_DATA_END						//�I��

//--------------------------------------------------------------------
//					     �f�o�b�N��`
//--------------------------------------------------------------------
//#define DEBUG_10_WIN					//10�A���f�o�b�N
//#define DEBUG_BTL_OFF					//�o�g���I�t�f�o�b�N
//#define DEBUG_BTL_LOSE_OFF			//�o�g���s�k�I�t�f�o�b�N
//#define DEBUG_ANM_OFF				//�A�j���I�t�f�o�b�N
//#define DEBUG_7BTL_OFF				//�o�g��10��I�t�f�o�b�N


/********************************************************************/
//
//		SCENE_STAGE:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
_RESOURCE_LABEL	default_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	default_set_resource_multi
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

/*�g���[�i�[�Ŏg�p����OBJ�R�[�h�͎g�p���Ă̓_���I*/
_RESOURCE_LABEL	default_set_resource_boygirl
	_CHAR_RESOURCE_DATA		SHOPM1,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		SHOPW1,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

/*�ΐ핔��*/
_RESOURCE_LABEL	btl_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		MAID,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	btl_set_resource_multi
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		MAID,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_STAGE:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
#define OBJID_PLAYER				(0)
#define OBJID_SIO_USER_0			(1)
#define OBJID_SIO_USER_1			(2)
#define OBJID_PCWOMAN2				(3)
#define OBJID_SHOPM1				(4)
#define OBJID_SHOPW1				(5)
#define OBJID_SHOPM2				(6)
#define OBJID_SHOPW2				(7)
#define OBJID_SHOPM3				(8)
#define OBJID_SHOPW3				(9)
#define OBJID_MINE					(10)
#define OBJID_MAID					(11)
#define OBJID_ETC					(98)
#define OBJID_ETC2					(99)

//�X�N���v�gID(����Ή��\��)
#define EVENTID_TEST_SCR_OBJ1		(1)

_ACTOR_LABEL	way_set_actor
	_PLAYER_ACTOR_DATA	OBJID_PLAYER,WF2DMAP_WAY_C_DOWN,8*15,8*14,OFF/*�������g�̃A�N�^�Z�b�g*/
	_ACTOR_DATA				OBJID_PCWOMAN2,FSW_PARAM6,WF2DMAP_WAY_C_DOWN, \
							8*16,8*4,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_PARAM1,WF2DMAP_WAY_C_DOWN, \
							8*16,8*4,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	way_set_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_DOWN,8*15,8*14,OFF/*�������g�̃A�N�^�Z�b�g*/
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_DOWN,8*16,8*4,OFF
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_DOWN,8*16,8*4,OFF
	_ACTOR_DATA				OBJID_PCWOMAN2,FSW_PARAM6,WF2DMAP_WAY_C_DOWN, \
							8*16,8*4,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

/*�o�g��*/
_ACTOR_LABEL	default_set_actor
	_PLAYER_ACTOR_DATA	OBJID_PLAYER,WF2DMAP_WAY_C_DOWN,8*15,8*16,OFF/*�������g�̃A�N�^�Z�b�g*/
//_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_DOWN,8*16,8*16,ON/*�������g�̃A�N�^�Z�b�g*/
	_ACTOR_DATA				OBJID_MAID,MAID,WF2DMAP_WAY_C_RIGHT, \
							8*6,8*40,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_PARAM1,WF2DMAP_WAY_C_DOWN, \
							8*16,8*16,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	default_set_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_DOWN,8*15,8*16,OFF/*�������g�̃A�N�^�Z�b�g*/
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_DOWN,8*16,8*16,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_DOWN,8*16,8*14,ON
	_ACTOR_DATA				OBJID_MAID,MAID,WF2DMAP_WAY_C_RIGHT, \
							8*6,8*40,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

/********************************************************************/
//
//	SCENE_STAGE_BTL:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
_RESOURCE_LABEL	default_scene2_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		CAMPBOY, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		WAITER, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		WAITRESS, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		REPORTER, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		TOWERBOSS, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		SEVEN5, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	default_scene2_resource_multi
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		CAMPBOY, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		WAITER, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		WAITRESS, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		REPORTER, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		TOWERBOSS, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		SEVEN5, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_STAGE_BTL:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================

//--------------------------------------------------------------------
//	�󋵂ɂ���ĕς��郊�\�[�X
// LOCAL0 = OBJCODE
//--------------------------------------------------------------------
_RESOURCE_LABEL	etc_set_resource
	_CHAR_RESOURCE_DATA		FSW_PARAM2,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	etc_set_resource2
	_CHAR_RESOURCE_DATA		FSW_PARAM3,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//--------------------------------------------------------------------
//	�󋵂ɂ���ĕς���A�N�^�[
// LOCAL0 = OBJCODE
//--------------------------------------------------------------------
/*�擪����(1�l�ڂ�OFF�ɂ��Ă���)*/
_ACTOR_LABEL	etc_set_actor
	_ACTOR_DATA			OBJID_ETC,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							8*16,8*28,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

/*�����Ă���*/
_ACTOR_LABEL	etc_set_actor2
	_ACTOR_DATA			OBJID_ETC2,FSW_PARAM3,WF2DMAP_WAY_C_DOWN, \
							8*16,8*26,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

/********************************************************************/
//
//					�A�j���[�V�����f�[�^
//
/********************************************************************/

//--------------------------------------------------------------------
//�ΐ핔���܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_door
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,9
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_door_multi
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,8
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WALK_D_8F,2
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_door_pcwoman
#if 0
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,10
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
#else
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,9
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA	FC_WAIT_4F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
#endif
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�b������
//--------------------------------------------------------------------
_ANIME_LABEL anm_stay_walk_left
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�����܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_center_scroll
	_ANIME_DATA	FC_WALK_D_8F,12
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center_scroll_2
	_ANIME_DATA	FC_WALK_D_8F,4
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center_scroll_3
	_ANIME_DATA	FC_WALK_D_8F,4
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center_scroll_4
	_ANIME_DATA	FC_WALK_D_8F,4
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center
	_ANIME_DATA	FC_WALK_D_8F,12
	_ANIME_DATA	FC_WALK_L_8F,3
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

/*�ʐM�e(�ʒu��)*/
_ANIME_LABEL anm_sio_player_go_center
	_ANIME_DATA	FC_WALK_D_8F,12
	_ANIME_DATA	FC_WALK_L_8F,3
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

/*�ʐM�q(�ʒu��)*/
_ANIME_LABEL anm_player_go_center_multi
	_ANIME_DATA	FC_WALK_D_8F,13
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center_pcwoman
	_ANIME_DATA	FC_WALK_D_8F,11
	_ANIME_DATA	FC_WALK_L_8F,5
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//���ґO��
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_btl_room
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_btl_room_multi
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_btl_room_etc
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//��l���̑O�܂ŗ���
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_btl_room_100
	_ANIME_DATA	FC_WALK_L_8F,5
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_btl_room_101
	_ANIME_DATA	FC_WALK_R_8F,5
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�g���[�i�[���o�g�����[���̒����܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL etc_anime_room_in
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,6
	_ANIME_DATA	FC_WALK_R_8F,3
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

/*�ʐM(�ʒu��)*/
_ANIME_LABEL etc_anime_room_in_sio
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,6
	_ANIME_DATA	FC_WALK_R_8F,3
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

/*�ʐM(�ʒu��)*/
_ANIME_LABEL etc_anime_room_in_multi
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,7
	_ANIME_DATA	FC_WALK_R_8F,3
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA	FC_WALK_U_8F,6
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA	FC_WAIT_32F,1
	_ANIME_DATA	FC_WAIT_32F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain_2
	_ANIME_DATA	FC_WALK_D_8F,4
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain_2_2
	_ANIME_DATA	FC_WALK_D_8F,4
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain_2_3
	_ANIME_DATA	FC_WALK_D_8F,4
	_ANIME_DATA	FC_WALK_R_8F,3
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain_3
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�g���[�i�[���o�g�����[������o�čs��
//--------------------------------------------------------------------
_ANIME_LABEL etc_anime_go_out
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,7
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

/*�ʐM(�ʒu��)*/
_ANIME_LABEL etc_anime_go_out_sio
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,8
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

/*�ʐM(�ʒu��)*/
_ANIME_LABEL etc_anime_go_out_multi
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,8
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//����E��
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_right
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�������
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_left
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//���C�h�@�������
//--------------------------------------------------------------------
_ANIME_LABEL anm_maid_go_left
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//������܂Ŗ߂�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_center_scroll_back
	_ANIME_DATA	FC_WALK_U_8F,12
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�E������
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_dir_right
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END


/********************************************************************/
//
//								�ʘH
//
/********************************************************************/
fss_stage_start:
	//_EVENT_START

	/*lobby�őI�������^�C�v���K�v*/
	_SAVE_EVENT_WORK_GET	WK_SCENE_STAGE_TYPE,FSW_LOCAL1	/*type*/
	_SAVE_EVENT_WORK_GET	LOCALWORK3,FSW_LOCAL3			/*init*/
	_SAVE_EVENT_WORK_GET	LOCALWORK2,FSW_LOCAL2			/*�I�����Ď莝���|�P�����ʒu*/
	_SAVE_EVENT_WORK_GET	LOCALWORK5,FSW_LOCAL0			/*�I�����Ď莝���|�P�����ʒu2*/

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_PARAM1

	/*���񔻕ʃ��[�N*/
	_LDVAL				FSW_PARAM5,0

	/*���[�N�m��*/
	_BATTLE_REC_INIT
	_STAGE_WORK_ALLOC	FSW_LOCAL3,FSW_LOCAL1,FSW_LOCAL2,FSW_LOCAL0

	_STAGE_TOOL			FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_stage_start_multi

	_LDVAL				FSW_PARAM6,BFSM
	_CHAR_RESOURCE_SET	default_set_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			way_set_actor				/*�A�N�^�[�o�^*/
	_JUMP				ev_stage_start_data_set
	_END

ev_stage_start_multi:
	_CALL				ev_stage_comm_command_initialize_1

	/*���s�Ɋ֌W�Ȃ��ʐM�t�����e�B�A�ɒ���(�ʐM�͒��f���Ȃ��̂Ő擪�ŏ������Ă����v)*/
	_SCORE_ADD			SCORE_ID_FRONTIER_COMM

	_LDVAL				FSW_PARAM6,BFSW1
	_CHAR_RESOURCE_SET	default_set_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			way_set_actor_multi			/*�A�N�^�[�o�^*/
	_JUMP				ev_stage_start_data_set
	_END

ev_stage_start_data_set:
	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_STAGE_TOOL			FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_stage_start_data_set_del

	/*�ΐ핔���܂ŕ���*/
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_player_go_door_pcwoman
	_OBJ_ANIME			OBJID_MINE,anm_player_go_door
	_OBJ_ANIME_WAIT

	/*�ΐ핔���փ}�b�v�؂�ւ�*/
	_SE_PLAY			SEQ_SE_DP_KAIDAN2
	_SE_WAIT			SEQ_SE_DP_KAIDAN2
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_ACTOR_FREE			OBJID_PLAYER
	_ACTOR_FREE			OBJID_PCWOMAN2
	_ACTOR_FREE			OBJID_MINE
	_CHAR_RESOURCE_FREE	FSW_PARAM6
	_JUMP				ev_stage_start_data_set_del_sub
	_END

ev_stage_start_data_set_del:
	/*�ΐ핔���܂ŕ���*/
	_OBJ_ANIME		OBJID_PCWOMAN2,anm_player_go_door_pcwoman
	_OBJ_ANIME		OBJID_SIO_USER_0,anm_player_go_door
	_OBJ_ANIME		OBJID_SIO_USER_1,anm_player_go_door_multi
	_OBJ_ANIME_WAIT

	/*�ΐ핔���փ}�b�v�؂�ւ�*/
	_SE_PLAY			SEQ_SE_DP_KAIDAN2
	_SE_WAIT			SEQ_SE_DP_KAIDAN2
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_ACTOR_FREE			OBJID_SIO_USER_0
	_ACTOR_FREE			OBJID_SIO_USER_1
	_ACTOR_FREE			OBJID_PCWOMAN2
	//_ACTOR_FREE			OBJID_MINE
	_CHAR_RESOURCE_FREE	FSW_PARAM6
	_JUMP				ev_stage_start_data_set_del_sub
	_END


/********************************************************************/
//
//						�ΐ핔��(�X�e�[�W)
//
/********************************************************************/
ev_stage_start_data_set_del_sub:
	_MAP_CHANGE			FSS_SCENEID_STAGE_BTL

	/*�����ł͂܂����[�N���m�ۂ���Ă��Ȃ�*/
	_STAGE_TOOL			FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_stage_start_multi_btl_room

	_CHAR_RESOURCE_SET	btl_set_resource			/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_set_actor			/*�A�N�^�[�o�^*/
	_JUMP				ev_stage_start_data_set_btl_room
	_END

ev_stage_start_multi_btl_room:
	_CHAR_RESOURCE_SET	btl_set_resource_multi		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_set_actor_multi		/*�A�N�^�[�o�^*/
	_JUMP				ev_stage_start_data_set_btl_room
	_END

ev_stage_start_data_set_btl_room:
	/*�|�P�����\��*/
	_TEMOTI_POKE_ACT_ADD	FSW_LOCAL2,STAGE_POKE_ACT_X,STAGE_POKE_ACT_Y,POKE_DISP_MANAGER_ID_0, 0, 0x0000

	/*�X�|�b�g���C�g�A�X�g���{*/
	_PARTICLE_SPA_LOAD	SPAWORK_0, STAGE_SPA, FSS_CAMERA_ORTHO
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_SPOT01
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_SPOT01_BG
	_CALL				call_se_pasya_01			/*�X�g���{*/
	//_PARTICLE_WAIT

	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_WND_SET			72,160,230,192,0,1						/*�E�B���h�EON*/
	_JUMP				ev_stage_start_2
	_END


/********************************************************************/
//
/********************************************************************/
ev_stage_start_2:
	_JUMP				ev_stage_start_3
	_END


/********************************************************************/
//
/********************************************************************/
ev_stage_start_3:
	/*�u���낭����v�Ŏg�p���郏�[�N�Ȃ̂ŃN���A���Ă���*/
	_LDVAL				FSW_LOCAL3,0

#ifdef DEBUG_10_WIN	/************************************************/
	_JUMP				ev_stage_room_18_win
#endif	/************************************************************/

	//_SE_PLAY			SEQ_SE_DP_DENDOU				/*����*/

	/*�ʐM�}���`*/
	_STAGE_TOOL			FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_stage_start_3_multi

#ifndef DEBUG_ANM_OFF	/********************************************/
	/*�����܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center_scroll
	//_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center
	_OBJ_ANIME			OBJID_MINE,anm_player_go_center
	_OBJ_ANIME_WAIT
#endif	/************************************************************/

	_JUMP				ev_stage_room_start
	_END

ev_stage_start_3_multi:
	/*�ʘH�ɓ����Ă����Ɉړ�*/
	//_CALL			ev_stage_comm_command_initialize_1

	/*�����܂ŕ���*/
	_OBJ_ANIME		OBJID_PLAYER,anm_player_go_center_scroll
	_OBJ_ANIME		OBJID_SIO_USER_0,anm_sio_player_go_center
	_OBJ_ANIME		OBJID_SIO_USER_1,anm_player_go_center_multi
	_OBJ_ANIME_WAIT

	_JUMP			ev_stage_room_start
	_END

ev_stage_room_start:
	_WND_SET			10,160,230,192,0,0						/*�E�B���h�EOFF*/

	/*�X�|�b�g���C�g�A�X�g���{������*/
	//_PARTICLE_SPA_EXIT	SPAWORK_0

	//_PARTICLE_SPA_LOAD	SPAWORK_0, STAGE_SPA, FSS_CAMERA_ORTHO
	_PARTICLE_ADD_EMITTER	SPAWORK_0,STAGE_BS_SPOT02		/*�L����X�|�b�g���C�g*/
	//_PARTICLE_WAIT
	_TIME_WAIT		30,FSW_ANSWER

	/*�z���C�g�A�E�g*/
	_WHITE_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*���d�ʂ�\��*/
	_STAGE_TOOL			FS_ID_EFF_FRAME_ON,0,0,FSW_ANSWER

	/*�L����X�|�b�g���C�g������*/
	_PARTICLE_SPA_EXIT	SPAWORK_0

	//_SE_STOP		SEQ_SE_PL_HAND

	/*�z���C�g�C��*/
	_WHITE_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�|�P�����폜*/
	_TEMOTI_POKE_ACT_DEL	POKE_DISP_MANAGER_ID_0

	/*�����l�ڂ��擾*/
	_STAGE_TOOL		FS_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_stage_room_renshou_0			/*0		*/

	/*�L�^�������[�N�Ƀf�[�^���������܂�Ă��Ȃ��̂Ń��j���[�\�����Ȃ�*/
	_LDVAL			FSW_LOCAL3,1
	_JUMP			ev_stage_room_saikai							/*1-7	*/
	_END


/********************************************************************/
/*							�A���� 0								*/
/********************************************************************/
ev_stage_room_renshou_0:
	/*�u�ǂ̃^�C�v�Ƒΐ킷��H�v*/
	_TALKMSG		msg_stage_room_40

	/*�^�C�v�I����ʌĂяo���O�ɁA�^�C�v�̃����N���S�Ē���ς݂̍ő�ɂȂ��Ă����璲����������*/
	_STAGE_TOOL		FS_ID_TYPE_LEVEL_MAX,0,0,FSW_ANSWER

	/*�ʐM�}���`*/
	_STAGE_TOOL		FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_multi_room_go_rental

	_JUMP			ev_stage_room_go_rental_2
	_END

ev_stage_multi_room_go_rental:
	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_stage_room_wait

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_BOTH_1
	_COMM_RESET

	_COMM_SET_WIFI_BOTH_NET	1												/*��ʃf�[�^ON*/

	_JUMP				ev_stage_multi_room_go_rental_retry
	_END

ev_stage_multi_room_go_rental_retry:

	/*�ʐM�F�莝�����Ƃ�*/
	_LDVAL				FSW_LOCAL6,STAGE_COMM_TEMOTI
	_STAGE_SEND_BUF		FSW_LOCAL6,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_stage_multi_room_go_rental_retry	/*���M���s�́A�đ��M��*/

	_STAGE_RECV_BUF		FSW_LOCAL6
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_BOTH_0
	_COMM_RESET

	_COMM_SET_WIFI_BOTH_NET	0												/*��ʃf�[�^OFF*/

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_POKETYPE_SEL
	_COMM_RESET		/*��ʐ؂�ւ��O�ɏ�����*/

	_JUMP			ev_stage_room_go_rental_2
	_END

ev_stage_room_go_rental_2:
	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_TALK_CLOSE

	/*�ʐM�}���`*/
	_STAGE_TOOL		FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_stage_room_type_sel_sync

	/*�^�C�v�I����ʌĂяo��*/
	_STAGE_POKE_TYPE_SEL_CALL
	//_SET_MAP_PROC

	/*���d�ʂ�\��*/
	_STAGE_TOOL			FS_ID_EFF_FRAME_ON,0,0,FSW_ANSWER

	/*���������v�Z*/
	_STAGE_TOOL			FS_ID_SQRT,0,0,FSW_ANSWER

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�G�g���[�i�[�A�G�|�P������I�o*/
	_STAGE_WORK_ENEMY_SET	FSW_LOCAL3

	_JUMP			ev_stage_room_go
	_END

/*�ʐM�}���`*/
ev_stage_room_type_sel_sync:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_TYPE_SEL_CALL
	_COMM_RESET
	_RET


/********************************************************************/
/*						�o�g���֌�����								*/
/********************************************************************/
ev_stage_room_go:

	/*�ʐM�}���`*/
	_STAGE_TOOL		FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_room_go_2_multi

	/*����ł͂Ȃ����͉E�������A�j��������*/
	_IFVAL_CALL		FSW_PARAM5,NE,0,ev_stage_room_go_right_anm
	_LDVAL			FSW_PARAM5,1

	/*�u���[���o�ꂩ�`�F�b�N*/
	_STAGE_TOOL		FS_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_leader_1st_02
	_IFVAL_JUMP		FSW_ANSWER,EQ,2,ev_stage_leader_2nd_02

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_stage_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*�G�g���[�i�[�������܂ŕ���*/
	_CALL			ev_stage_trainer_set_1
	_OBJ_ANIME		OBJID_ETC,etc_anime_room_in
	_OBJ_ANIME_WAIT

	_TIME_WAIT		15,FSW_ANSWER

	_OBJ_ANIME		OBJID_ETC,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_STAGE_TOOL	FS_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_STAGE_APPEAR	0
	_AB_KEYWAIT
	_TALK_CLOSE

	/*���ґO�փA�j��*/
	_OBJ_ANIME		OBJID_MINE,anm_player_go_btl_room
	_OBJ_ANIME		OBJID_ETC,anm_player_go_btl_room_etc
	_OBJ_ANIME_WAIT

	_JUMP				ev_stage_room_go_sub
	_END

ev_stage_room_go_sub:
	_JUMP			ev_stage_battle
	_END

ev_stage_room_go_2_multi:

	/*����ł͂Ȃ����͉E�������A�j��������*/
	_IFVAL_CALL		FSW_PARAM5,NE,0,ev_stage_room_go_right_anm_sio
	_LDVAL			FSW_PARAM5,1

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_INIT_BEFORE
	_COMM_RESET

	_CALL			ev_stage_comm_command_initialize_2

	_JUMP			ev_stage_room_go_2_multi_retry
	_END

ev_stage_room_go_2_multi_retry:
	_LDVAL			FSW_LOCAL6,STAGE_COMM_TR
	_STAGE_SEND_BUF	FSW_LOCAL6,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_stage_room_go_2_multi_retry	/*���M���s�́A�đ��M��*/

	_STAGE_RECV_BUF	FSW_LOCAL6
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_TR_AFTER
	_COMM_RESET

	_JUMP			ev_stage_room_go_3_multi
	_END

ev_stage_room_go_3_multi:
	_LDVAL			FSW_LOCAL6,STAGE_COMM_ENEMY
	_STAGE_SEND_BUF	FSW_LOCAL6,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_stage_room_go_3_multi	/*���M���s�́A�đ��M��*/

	_STAGE_RECV_BUF	FSW_LOCAL6
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_TR_ENEMY

	/*�G�g���[�i�[�������܂ŕ���*/
	_CALL			ev_stage_trainer_set_2
	_OBJ_ANIME		OBJID_ETC,etc_anime_room_in_sio
	_OBJ_ANIME		OBJID_ETC2,etc_anime_room_in_multi
	_OBJ_ANIME_WAIT

	_TIME_WAIT		15,FSW_ANSWER

	_OBJ_ANIME		OBJID_ETC,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_STAGE_TOOL		FS_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_STAGE_APPEAR	0
	_TIME_WAIT		30,FSW_ANSWER
	_TALK_CLOSE

	_OBJ_ANIME		OBJID_ETC2,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_STAGE_TOOL		FS_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_STAGE_APPEAR	1
	_TIME_WAIT		30,FSW_ANSWER
	_TALK_CLOSE

	/*���ґO�փA�j��*/
	//_OBJ_ANIME		OBJID_PLAYER,anm_player_go_btl_room
	_OBJ_ANIME		OBJID_SIO_USER_0,anm_player_go_btl_room
	_OBJ_ANIME		OBJID_SIO_USER_1,anm_player_go_btl_room_multi
	_OBJ_ANIME		OBJID_ETC,anm_player_go_btl_room_etc
	_OBJ_ANIME		OBJID_ETC2,anm_player_go_btl_room_etc
	_OBJ_ANIME_WAIT

	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_stage_room_wait

	_JUMP			ev_stage_battle
	_END

/*����ł͂Ȃ����͉E�������A�j��������*/
ev_stage_room_go_right_anm:
	/*��l���O������A�j��*/
	_OBJ_ANIME		OBJID_MINE,anm_player_dir_right
	_OBJ_ANIME		OBJID_MAID,anm_maid_go_left
	_OBJ_ANIME_WAIT
	_RET

ev_stage_room_go_right_anm_sio:
	/*��l���O������A�j��*/
	_OBJ_ANIME		OBJID_SIO_USER_0,anm_player_dir_right
	_OBJ_ANIME		OBJID_SIO_USER_1,anm_player_dir_right
	_OBJ_ANIME		OBJID_MAID,anm_maid_go_left
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*					�g���[�i�[�̌����ڃZ�b�g						*/
/********************************************************************/
ev_stage_trainer_set_1:
	_STAGE_TOOL		FS_ID_GET_TR_OBJ_CODE,0,0,FSW_ANSWER
	_LDWK			FSW_PARAM2,FSW_ANSWER
	_CHAR_RESOURCE_SET	etc_set_resource
	_ACTOR_SET		etc_set_actor
	_RET

ev_stage_trainer_set_2:
	//_STAGE_TOOL		FS_ID_GET_TR_OBJ_CODE,1,0,FSW_ANSWER
	_STAGE_TOOL		FS_ID_GET_TR_OBJ_CODE,0,0,FSW_ANSWER
	_LDWK			FSW_PARAM2,FSW_ANSWER
	_CHAR_RESOURCE_SET	etc_set_resource
	_ACTOR_SET		etc_set_actor

	//_STAGE_TOOL		FS_ID_GET_TR_OBJ_CODE,0,0,FSW_ANSWER
	_STAGE_TOOL		FS_ID_GET_TR_OBJ_CODE,1,0,FSW_ANSWER
	_LDWK			FSW_PARAM3,FSW_ANSWER
	_CHAR_RESOURCE_SET	etc_set_resource2
	_ACTOR_SET		etc_set_actor2
	_RET

ev_stage_trainer_set_brain:
	_LDVAL			FSW_PARAM2,BRAINS2
	_CHAR_RESOURCE_SET	etc_set_resource
	_ACTOR_SET		etc_set_actor
	_RET


/********************************************************************/
//
/********************************************************************/
ev_stage_battle:
	/*�t�F�[�h�A�E�g*/
	//_BLACK_OUT	SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK

	/*�ʐM�}���`*/
	_STAGE_TOOL		FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_multi_battle_2

	/*�u���[���o�ꂩ�`�F�b�N*/
	_STAGE_TOOL		FS_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_leader_1st_33
	_IFVAL_JUMP		FSW_ANSWER,EQ,2,ev_stage_leader_2nd_33

	_ENCOUNT_EFFECT	FR_ENCOUNT_EFF_BOX_D
	_JUMP			ev_stage_battle_2
	_END

ev_stage_leader_1st_33:
ev_stage_leader_2nd_33:
	_RECORD_INC		RECID_FRONTIER_BRAIN
	_BRAIN_ENCOUNT_EFFECT	FRONTIER_NO_STAGE
	_JUMP			ev_stage_battle_2
	_END

/*�ʐM�}���`*/
ev_stage_multi_battle_2:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_BATTLE
	_COMM_RESET

	_TALK_CLOSE
	_ENCOUNT_EFFECT	FR_ENCOUNT_EFF_BOX_D
	_JUMP			ev_stage_battle_2
	_END


/********************************************************************/
//
/********************************************************************/
ev_stage_battle_2:

#ifndef DEBUG_FRONTIER_LOOP

#ifndef DEBUG_BTL_OFF	/************************************************/
	/*�퓬�Ăяo��*/
	_STAGE_BATTLE_CALL
	_STAGE_CALL_GET_RESULT
#endif	/****************************************************************/

#endif	//DEBUG_FRONTIER_LOOP

	/*�����A�C�e�������ɖ߂�*/
	_STAGE_TOOL		FS_ID_SET_TEMP_ITEM,0,0,FSW_ANSWER

	/*���d�ʂ�\��*/
	_STAGE_TOOL			FS_ID_EFF_FRAME_ON,0,0,FSW_ANSWER

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

#ifndef DEBUG_FRONTIER_LOOP

#ifndef DEBUG_BTL_LOSE_OFF	/********************************************/
	/*�퓬���ʂŕ���*/
	_STAGE_LOSE_CHECK		FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_stage_lose		/*�s�k*/
#endif	/****************************************************************/

#endif	//DEBUG_FRONTIER_LOOP

	/*����*/
	_RECORD_INC		RECID_STAGE_WIN

	/*�^�C�v�̃��x�����グ��*/
	_STAGE_TOOL		FS_ID_INC_TYPE_LEVEL,0,0,FSW_ANSWER

	/*�^�C�v�̃��x�����擾*/
	_STAGE_TOOL		FS_ID_GET_TYPE_LEVEL,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,STAGE_TYPE_LEVEL_MAX,ev_stage_record_inc

	/*�ʐM�}���`*/
	_STAGE_TOOL		FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_battle_2_multi

	/*�u���[���o�ꂩ�`�F�b�N*/
	_STAGE_TOOL		FS_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_leader_1st_03
	_IFVAL_JUMP		FSW_ANSWER,EQ,2,ev_stage_leader_2nd_03

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_stage_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	_PARTICLE_SPA_LOAD	SPAWORK_0, STAGE_SPA, FSS_CAMERA_ORTHO
	_CALL			se_kami_01							/*��1*/
	_PARTICLE_WAIT
	_PARTICLE_SPA_EXIT	SPAWORK_0

	_JUMP			ev_stage_battle_2_sub
	_END

/*�^�C�v�̃��x����10�ɏ��������烌�R�[�h�X�V*/
ev_stage_record_inc:
	_RECORD_INC		RECID_STAGE_RANK10_WIN
	_RET
	
ev_stage_battle_2_sub:
	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME		OBJID_ETC,etc_anime_go_out
	_OBJ_ANIME_WAIT
	_ACTOR_FREE		OBJID_ETC
	_CHAR_RESOURCE_FREE	FSW_PARAM2

	/*��l�����������*/
	_OBJ_ANIME		OBJID_MAID,anm_player_go_right
	_OBJ_ANIME		OBJID_MINE,anm_player_go_left
	_OBJ_ANIME_WAIT

	_JUMP			ev_stage_battle_3
	_END

ev_stage_battle_2_multi:
	//
	_PARTICLE_SPA_LOAD	SPAWORK_0, STAGE_SPA, FSS_CAMERA_ORTHO
	_CALL			se_kami_01							/*��1*/
	_PARTICLE_WAIT
	_PARTICLE_SPA_EXIT	SPAWORK_0

	_CALL			ev_stage_comm_command_initialize_3

	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME		OBJID_ETC,etc_anime_go_out_sio
	_OBJ_ANIME		OBJID_ETC2,etc_anime_go_out_multi
	_OBJ_ANIME_WAIT
	_ACTOR_FREE		OBJID_ETC
	_ACTOR_FREE		OBJID_ETC2
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_CHAR_RESOURCE_FREE	FSW_PARAM3

	/*��l�����������*/
	_OBJ_ANIME		OBJID_MAID,anm_player_go_right
	_OBJ_ANIME		OBJID_SIO_USER_0,anm_player_go_left
	_OBJ_ANIME		OBJID_SIO_USER_1,anm_player_go_left
	_OBJ_ANIME_WAIT

	_JUMP			ev_stage_battle_3
	_END

ev_stage_battle_3:
	_JUMP			ev_stage_battle_4
	_END

ev_stage_battle_4:

#ifdef DEBUG_FRONTIER_LOOP
	_JUMP			ev_stage_room_renshou_17
#endif	//DEBUG_FRONTIER_LOOP

	/*�����l��+1*/
	_STAGE_TOOL		FS_ID_INC_ROUND,1,0,FSW_ANSWER

	/*�A����+1*/
	_STAGE_TOOL		FS_ID_INC_RENSYOU,0,0,FSW_ANSWER

#ifndef DEBUG_7BTL_OFF	/********************************************/
	/*��10�l�ڂłȂ����͓��ɖ߂�*/
	_STAGE_TOOL	FS_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,NE,STAGE_LAP_ENEMY_MAX,ev_stage_room_renshou_17	/*�A������1-10�̎�*/
#endif	/************************************************************/

	_JUMP			ev_stage_room_18_win
	_END


/********************************************************************/
/*							10�A������(�L�^)						*/
/********************************************************************/
ev_stage_room_18_win:

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_stage_room_18_win_bp_sub

	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,0,ev_stage_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_stage_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_room_18_win_bp
	_JUMP			ev_stage_room_18_win_rec_yes
	_END

ev_stage_room_18_win_bp_sub:
	_TALKMSG_NOSKIP	msg_stage_room_wait
	_JUMP			ev_stage_room_18_win_bp
	_END

/*�u�͂��v*/
ev_stage_room_18_win_rec_yes:
	_CALL			ev_stage_room_rec_win		/*�����̋L�^*/
	_JUMP			ev_stage_room_18_win_bp
	_END


/********************************************************************/
/*							10�A������(BP�擾)						*/
/********************************************************************/
ev_stage_room_18_win_bp:
	//10�A��(�N���A)�p�����[�^�Z�b�g
	_STAGE_TOOL	FS_ID_SET_CLEAR,0,0,FSW_ANSWER

	/*10�A�������̂ŏI����*/
	//_LDVAL			WK_SCENE_STAGE_LOBBY,1				/*10�A������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_STAGE_LOBBY,1			/*10�A������n�܂�悤�ɂ��Ă���*/

	/*�u�o�g���g���[�h�@�V����@�Ƃ��ρ@���߂łƂ��v*/
	_STAGE_TOOL		FS_ID_GET_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	0,FSW_ANSWER,0,0
	_TALKMSG		msg_stage_room_20

	_PLAYER_NAME	0											/*�v���C���[���Z�b�g*/

	/*�o�g���|�C���g�ǉ�*/
	_STAGE_TOOL		FS_ID_GET_BP_POINT,0,0,FSW_ANSWER
	_NUMBER_NAME	1,FSW_ANSWER								/*���l�Z�b�g*/
	_BTL_POINT_ADD	FSW_ANSWER

	/*�u�a�o����������v*/
	_TALKMSG		msg_stage_lobby_21
	_ME_PLAY		ME_BP_GET
	_ME_WAIT

	/*TV����(�֐����ŁA�V���O���̎��������킵�Ă��郂���X�^�[�i���o�[���Z�b�g���Ă���)*/
	_TV_TEMP_STAGE_SET	

	_JUMP			ev_stage_room_bp_end
	_END


/********************************************************************/
/*							10�A�������I��							*/
/********************************************************************/
ev_stage_room_bp_end:
	_JUMP			ev_stage_lose_timing
	_END


/********************************************************************/
/*							�A���� 1-10								*/
/********************************************************************/
ev_stage_room_renshou_17:
	/*�u�|�P�������@�����ӂ��@���܂��傤�v*/
	_STAGE_TOOL		FS_ID_GET_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	0,FSW_ANSWER,0,0
	_TALKMSG		msg_stage_room_3
	_ME_PLAY		SEQ_ASA
	_ME_WAIT

	_PC_KAIFUKU
	_JUMP			ev_stage_room_saikai
	_END


/********************************************************************/
/*						���f�Z�[�u����̍ĊJ						*/
/********************************************************************/
ev_stage_room_saikai:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_stage_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	_JUMP			ev_stage_room_saikai_02
	_END
	
ev_stage_room_saikai_02:
	/*�u���́@����ڂł���v*/
	_STAGE_TOOL		FS_ID_GET_ROUND,0,0,FSW_ANSWER
	_ADD_WK			FSW_ANSWER,1
	_NUMBER_NAME	0,FSW_ANSWER
	_TALKMSG		msg_stage_room_4
	_JUMP			ev_stage_room_menu_make
	_END


/********************************************************************/
/*							���j���[����							*/
/********************************************************************/
ev_stage_room_menu_make:

	_STAGE_TOOL		FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_room_menu_make_comm

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,0,ev_stage_room_menu_make_04_set

	_JUMP			ev_stage_room_menu_make_03_set
	_END

/*�ʐM�̎��̃��j���[*/
ev_stage_room_menu_make_comm:

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_stage_room_menu_make_01_set

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,0,ev_stage_room_menu_make_02_set

	_JUMP			ev_stage_room_menu_make_01_set
	_END

ev_stage_room_menu_make_01_set:
	_LDVAL				FSW_ANSWER,1
	_JUMP				ev_stage_room_menu_make_sub
	_END

ev_stage_room_menu_make_02_set:
	_LDVAL				FSW_ANSWER,2
	_JUMP				ev_stage_room_menu_make_sub
	_END

ev_stage_room_menu_make_03_set:
	_LDVAL				FSW_ANSWER,3
	_JUMP				ev_stage_room_menu_make_sub
	_END

ev_stage_room_menu_make_04_set:
	_LDVAL				FSW_ANSWER,4
	_JUMP				ev_stage_room_menu_make_sub
	_END

/*�ǂ̃��j���[�`���ɂ��邩*/
ev_stage_room_menu_make_sub:
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_stage_room_menu_make_01
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_stage_room_menu_make_02
	_IFVAL_JUMP			FSW_ANSWER,EQ,3,ev_stage_room_menu_make_03
	_IFVAL_JUMP			FSW_ANSWER,EQ,4,ev_stage_room_menu_make_04
	_END

ev_stage_room_menu_make_01:
	_BMPLIST_INIT_EX	24,13,0,0,FSW_PARAM0			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_stage_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0		/*�Â���*/
	_JUMP			ev_stage_room_yasumu_next
	_END

ev_stage_room_menu_make_02:
	_BMPLIST_INIT_EX	23,11,0,0,FSW_PARAM0			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_stage_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0		/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_stage_room_choice_02,FSEV_WIN_TALK_MSG_NONE,1		/*���낭����*/
	_JUMP			ev_stage_room_yasumu_next
	_END

ev_stage_room_menu_make_03:
	_BMPLIST_INIT_EX	24,11,0,0,FSW_PARAM0			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_stage_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0		/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_stage_room_choice_03,FSEV_WIN_TALK_MSG_NONE,2		/*�₷��*/
	_JUMP			ev_stage_room_yasumu_next
	_END

ev_stage_room_menu_make_04:
	_BMPLIST_INIT_EX	23,9,0,0,FSW_PARAM0				/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_stage_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0		/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_stage_room_choice_02,FSEV_WIN_TALK_MSG_NONE,1		/*���낭����*/
	_BMPLIST_MAKE_LIST	msg_stage_room_choice_03,FSEV_WIN_TALK_MSG_NONE,2		/*�₷��*/
	_JUMP			ev_stage_room_yasumu_next
	_END

ev_stage_room_yasumu_next:
	_BMPLIST_MAKE_LIST	msg_stage_room_choice_04,FSEV_WIN_TALK_MSG_NONE,3		/*���^�C�A*/
	_BMPLIST_START
	_JUMP			ev_stage_room_menu_sel
	_END


/********************************************************************/
/*							���j���[����							*/
/********************************************************************/
ev_stage_room_menu_sel:

#ifdef DEBUG_FRONTIER_LOOP
	_JUMP			ev_room_choice_01
#endif	//DEBUG_FRONTIER_LOOP

	_IFVAL_JUMP		FSW_PARAM0,EQ,0,ev_room_choice_01	/*�Â���*/
	_IFVAL_JUMP		FSW_PARAM0,EQ,1,ev_room_choice_02	/*���낭����*/
	_IFVAL_JUMP		FSW_PARAM0,EQ,2,ev_room_choice_03	/*�₷��*/
	_IFVAL_JUMP		FSW_PARAM0,EQ,3,ev_room_choice_04	/*���^�C�A*/
	_JUMP			ev_room_choice_04
	_END


/********************************************************************/
/*						�u�Â���v								*/
/********************************************************************/
ev_room_choice_01:
	/*�ʐM�}���`*/
	_STAGE_TOOL		FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_room_choice_01_multi
	_JUMP			ev_room_choice_01_2
	_END

/*�ʐM�}���`*/
ev_room_choice_01_multi:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME		0
	_TALKMSG		msg_stage_room_10
	_JUMP			ev_room_choice_01_multi_retry
	_END

ev_room_choice_01_multi_retry:
	_LDVAL			FSW_LOCAL6,STAGE_COMM_RETIRE
	_STAGE_SEND_BUF	FSW_LOCAL6,0,FSW_ANSWER					/*0=�Â���*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_room_choice_01_multi_retry	/*���M���s�́A�đ��M��*/

	_STAGE_RECV_BUF	FSW_LOCAL6
	_COMM_RESET

	/*���肪�I�񂾃��j���[���擾*/
	_STAGE_TOOL		FS_ID_GET_RETIRE_FLAG,0,0,FSW_ANSWER

	/*�p�[�g�i�[�����^�C�A������*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_room_giveup_yes_multi	/*���^�C�A*/

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_MENU
	_COMM_RESET

	_JUMP			ev_room_choice_01_2
	_END

/*�p�[�g�i�[�����^�C�A������*/
ev_stage_room_giveup_yes_multi:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME		0
	_TALKMSG_NOSKIP		msg_stage_room_12
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER

	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_stage_room_wait
	_JUMP			ev_stage_room_giveup_yes_multi_sync
	_END

ev_room_choice_01_2:
	/*���[�N���N���A���Ă���*/
	_LDVAL			FSW_LOCAL3,0

	_JUMP			ev_stage_room_trade_yes_pair
	_END

ev_stage_room_trade_yes_pair:
	_JUMP			ev_stage_room_renshou_0
	_END


/********************************************************************/
/*							�L�^����*/
/********************************************************************/
ev_room_choice_02:
	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,0,ev_stage_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_stage_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER									/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_room_rec_no
	_JUMP			ev_stage_room_rec_yes
	_END

/*�u�͂��v*/
ev_stage_room_rec_yes:
	_CALL			ev_stage_room_rec_win		/*�����̋L�^*/
	_JUMP			ev_stage_room_saikai		/*���j���[�֖߂�*/
	_END

/*�u�������v*/
ev_stage_room_rec_no:
	_JUMP			ev_stage_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*							���ʋL�^*/
/********************************************************************/
/*����*/
ev_stage_room_rec_win:
	_CALL				ev_stage_room_rec_common		/*ANSWER,LOCAL5�g�p��*/
	//_ADD_WK			FSW_LOCAL5,1
	//_SUB_WK			FSW_LOCAL5,1					/*���E���h���ł͂Ȃ��������Ȃ̂�*/
	_JUMP				ev_stage_room_rec
	_END

/*����*/
ev_stage_room_rec_lose:
	_CALL				ev_stage_room_rec_common		/*ANSWER,LOCAL5�g�p��*/
	_ADD_WK				FSW_LOCAL5,1
	_JUMP				ev_stage_room_rec
	_END

/*���ʕ���*/
ev_stage_room_rec_common:
	/*�^��f�[�^�Z�[�u*/
	_STAGE_TOOL			FS_ID_GET_RENSYOU,0,0,FSW_LOCAL5
	_RET

ev_stage_room_rec:
	_JUMP				ev_stage_room_rec_sub
	_END

ev_stage_room_rec_sub:
	/*�L�^�������[�N�Z�b�g*/
	_LDVAL				FSW_LOCAL3,1

	_TALKMSG_ALL_PUT	msg_stage_room_6_4
	_STAGE_TOOL			FS_ID_GET_TYPE,0,0,FSW_ANSWER
	_ADD_WAITICON
	_BATTLE_REC_SAVE	FRONTIER_NO_STAGE,FSW_ANSWER,FSW_LOCAL5,FSW_LOCAL5
	_DEL_WAITICON

	//_BATTLE_REC_LOAD

	_IFVAL_JUMP			FSW_LOCAL5,EQ,1,ev_stage_room_rec_true

	/*�u�L�^�o���܂���ł����v*/
	_TALKMSG			msg_stage_room_6_2
	_RET

ev_stage_room_rec_true:
	/*�u�L�^����܂����v*/
	_SE_PLAY			SEQ_SE_DP_SAVE
	_PLAYER_NAME		0
	_TALKMSG			msg_stage_room_6_1
	_RET
	
/*�^��f�[�^���Ȃ������b�Z�[�W*/
ev_stage_room_rec_msg1:
	_TALKMSG			msg_stage_room_6
	_RET

/*���łɘ^��f�[�^�����鎞���b�Z�[�W*/
ev_stage_room_rec_msg2:
	_TALKMSG			msg_stage_room_6_3
	_RET


/********************************************************************/
/*							�₷��*/
/********************************************************************/
ev_room_choice_03:
	/*�u���|�[�g�������ďI�����܂����H�v*/
	_TALKMSG		msg_stage_room_7
	_YES_NO_WIN		FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_room_rest_no
	_JUMP			ev_stage_room_rest_yes
	_END

/*�u�͂��v*/
ev_stage_room_rest_yes:
	//_LDVAL			WK_SCENE_STAGE_LOBBY,2			/*��������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_STAGE_LOBBY,2		/*��������n�܂�悤�ɂ��Ă���*/

	//�v���C�f�[�^�Z�[�u
	_STAGE_TOOL	FS_ID_SAVE_REST_PLAY_DATA,0,0,FSW_ANSWER

	//_CALL			ev_stage_save			/*07.08.24 _CALL�ɒu��������*/
	_CALL			ev_stage_save_rest

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_STAGE_WORK_FREE

	_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_TALK_CLOSE

	//���Z�b�g�R�}���h
	_STAGE_TOOL	FS_ID_SYSTEM_RESET,0,0,FSW_ANSWER

	_END

/*�u�������v*/
ev_stage_room_rest_no:
	_JUMP			ev_stage_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*							���^�C�A*/
/********************************************************************/
ev_room_choice_04:
	/*�u�o�g���g���[�h�̒���𒆎~����H�v*/
	_TALKMSG		msg_stage_room_8
	_YES_NO_WIN_EX	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_room_giveup_no

	/*�ʐM*/
	_STAGE_TOOL		FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_room_giveup_yes_multi_wait_msg

	_JUMP			ev_stage_room_giveup_yes
	_END

/*�u���X���҂����������v*/
ev_stage_room_giveup_yes_multi_wait_msg:
	_TALKMSG_NOSKIP	msg_stage_room_wait
	_JUMP			ev_stage_room_giveup_yes_multi_retry
	_END

ev_stage_room_giveup_yes_multi_retry:
	_LDVAL			FSW_LOCAL6,STAGE_COMM_RETIRE
	_STAGE_SEND_BUF	FSW_LOCAL6,1,FSW_ANSWER						/*1=���^�C��*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_stage_room_giveup_yes_multi_retry	/*���M���s�́A�đ��M��*/

	//_STAGE_RECV_BUF	FSW_LOCAL6
	//_COMM_RESET
	_JUMP			ev_stage_room_giveup_yes_multi_sync
	_END

ev_stage_room_giveup_yes_multi_sync:

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_GIVE
	//_COMM_SYNCHRONIZE	DBC_TIM_STAGE_MENU
	_COMM_RESET
	_JUMP			ev_stage_room_giveup_yes
	_END

/*�u�͂��v*/
ev_stage_room_giveup_yes:
	//�s��p�����[�^�Z�b�g
	_STAGE_TOOL		FS_ID_SET_LOSE,0,0,FSW_ANSWER

	//_LDVAL			WK_SCENE_STAGE_LOBBY,3			/*���^�C������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_STAGE_LOBBY,3		/*���^�C������n�܂�悤�ɂ��Ă���*/

	_JUMP			ev_stage_room_end_save
	_END

/*�u�������v*/
ev_stage_room_giveup_no:
	_JUMP			ev_stage_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*								�s�k*/
/********************************************************************/
ev_stage_lose:
	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_stage_lose_timing_sub

	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,0,ev_stage_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_stage_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER							/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_lose_rec_no/*�u�������v*/
	_JUMP			ev_stage_lose_rec_yes				/*�u�͂��v*/
	_END

ev_stage_lose_timing_sub:
	_TALKMSG_NOSKIP	msg_stage_room_wait
	_JUMP			ev_stage_lose_timing_call
	_END

/*�u�͂��v*/
ev_stage_lose_rec_yes:
	_CALL			ev_stage_room_rec_lose		/*�s�k�̋L�^*/
	_JUMP			ev_stage_lose_timing_call
	_END

/*�u�������v*/
ev_stage_lose_rec_no:
	_JUMP			ev_stage_lose_timing_call
	_END

ev_stage_lose_timing_call:
	//�s��p�����[�^�Z�b�g
	_STAGE_TOOL		FS_ID_SET_LOSE,0,0,FSW_ANSWER

	//_LDVAL			WK_SCENE_STAGE_LOBBY,3			/*���^�C������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_STAGE_LOBBY,3		/*���^�C������n�܂�悤�ɂ��Ă���*/

	_JUMP			ev_stage_lose_timing
	_END

ev_stage_lose_timing:
	_STAGE_TOOL		FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_stage_multi_lose_timing
	_JUMP			ev_stage_room_end_save
	_END

/*�ʐM����*/
ev_stage_multi_lose_timing:
	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_stage_room_wait

	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_LOSE_END
	_COMM_RESET
	_JUMP				ev_stage_room_end_save
	_END

ev_stage_room_end_save:
	_CALL				ev_stage_save			/*07.08.24 _CALL�ɒu��������*/
	_TALK_CLOSE
	_JUMP				ev_stage_room_end
	_END


/********************************************************************/
/*							���ʏI��								*/
/********************************************************************/
ev_stage_room_end:

	/*�Z�[�u��ɓ���������*/
	_STAGE_TOOL			FS_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_stage_multi_end_timing

	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_STAGE_TOOL		FS_ID_GET_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,STAGE_TYPE_MULTI,ev_stage_room_multi_tv

	_STAGE_TOOL		FS_ID_GET_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,STAGE_TYPE_WIFI_MULTI,ev_stage_room_end_wifi

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_STAGE_WORK_FREE

	/*2D�}�b�v�T������*/
	//

	//_END
	_SCRIPT_FINISH			/*_END����2D�}�b�v�I��*/

/*�}���`�̂�*/
ev_stage_room_multi_tv:
	/*TV����*/
	_TV_TEMP_FRIEND_SET	FRONTIER_NO_STAGE
	_RET


/********************************************************************/
/*						�Z�[�u��ɓ���������						*/	
/********************************************************************/
ev_stage_multi_end_timing:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_SAVE_AFTER
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/
	_RET


/********************************************************************/
/*						WIFI�͑҂����킹��ʂ֖߂�					*/	
/********************************************************************/
ev_stage_room_end_wifi:
	/*�t�F�[�h�A�E�g*/
	//_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_STAGE_WORK_FREE

	_MAP_CHANGE_EX		FSS_SCENEID_WIFI_COUNTER,1
	_END


/********************************************************************/
//							�O���Z�[�u�Ȃ�(�x��)
/********************************************************************/
ev_stage_save_rest:
	/*�u���|�[�g�ɏ�������ł��܂��@�d����؂�Ȃ��ŉ������v*/
	_TALKMSG_ALL_PUT	msg_stage_lobby_24

	_ADD_WAITICON

	_CALL			ev_stage_save_sub

	_DEL_WAITICON
	_SE_PLAY		SEQ_SE_DP_SAVE
	_SE_WAIT		SEQ_SE_DP_SAVE
	_RET


/********************************************************************/
//							���ʃZ�[�u	
/********************************************************************/
ev_stage_save:
	/*�u���|�[�g�ɏ�������ł��܂��@�d����؂�Ȃ��ŉ������v*/
	_TALKMSG_ALL_PUT	msg_stage_lobby_24

	_ADD_WAITICON

	/*�o�g���X�e�[�W�̊O���A���L�^����������*/
	/*�Z�[�u���s�̕���͂���Ȃ�(08.02.26 ���c������)*/
	_STAGE_RENSHOU_COPY_EXTRA	FSW_ANSWER

	/*(WIFI_MULTI�ȊO�̃^�C�v)�O���Z�[�u�{�ʏ�Z�[�u�����s����Ȃ��������ƁA*/
	/*���R�[�h�I�ɊO���Z�[�u�̕K�v���Ȃ��������́A�ʏ�Z�[�u��������*/
	_IFVAL_CALL		FSW_ANSWER,EQ,0,ev_stage_save_sub

	_DEL_WAITICON
	_SE_PLAY		SEQ_SE_DP_SAVE
	_SE_WAIT		SEQ_SE_DP_SAVE
	_RET

/*(WIFI_MULTI�ȊO�̃^�C�v)�O���Z�[�u�{�ʏ�Z�[�u�����s����Ȃ��������ƁA*/
/*���R�[�h�I�ɊO���Z�[�u�̕K�v���Ȃ��������́A�ʏ�Z�[�u��������*/
ev_stage_save_sub:
	_REPORT_DIV_SAVE	FSW_ANSWER
	_RET

#if 0
	_ACTOR_FREE		OBJID_PCWOMAN2
	_ACTOR_FREE		OBJID_MINE
	/*�ϋq�폜*/
	_ACTOR_FREE		OBJID_SHOPM1
	_ACTOR_FREE		OBJID_SHOPW1
#endif


/********************************************************************/
/*							�X�g���{���o							*/
/********************************************************************/
#define STAGE_PASYA_01_NUM	(2)
#define STAGE_PASYA_02_NUM	(5)
#define STAGE_PASYA_03_NUM	(10)

call_se_pasya_01:
	_SE_PLAY			SEQ_SE_DP_DENDOU				/*����*/
	//_SE_PLAY			SEQ_SE_PL_HAND					/*BGM�ł����Ă���̂ł���Ȃ�*/
	//_SE_PLAY			SEQ_SE_PL_KATI
	_STAGE_TOOL			FS_ID_GET_LAP,0,0,FSW_ANSWER						/*���񐔎擾*/
	_IFVAL_JUMP			FSW_ANSWER,LT,STAGE_PASYA_01_NUM,se_pasya_01		/*�X�g���{��*/
	_IFVAL_JUMP			FSW_ANSWER,LT,STAGE_PASYA_02_NUM,se_pasya_02		/*�X�g���{��*/
	_JUMP				se_pasya_03											/*�X�g���{��*/
	_END

se_pasya_01:
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYA_01
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYA_02
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYAPASYA01
	_RET

se_pasya_02:
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYAPASYA02
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYAPASYA02_BG
	_RET

se_pasya_03:
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYAPASYA03
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYAPASYA03BG
	_RET


/********************************************************************/
/*						���ӂԂ�(��)���o							*/
/********************************************************************/
se_kami_01:
	/*���E���h�����擾*/
	_STAGE_TOOL		FS_ID_GET_ROUND,0,0,FSW_ANSWER
	_ADD_WK			FSW_ANSWER,1
	_IFVAL_JUMP		FSW_ANSWER,EQ,STAGE_LAP_ENEMY_MAX,se_kami_02	/*�A������10�̎�*/

	_SE_PLAY				SEQ_SE_DP_UG_022
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_01_L
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_01_R
	_SE_WAIT				SEQ_SE_DP_UG_022
	_SE_PLAY				SEQ_SE_DP_DENDOU
	_RET


/********************************************************************/
/*						���ӂԂ�(��)���o							*/
/********************************************************************/
se_kami_02:
	_SE_PLAY				SEQ_SE_DP_UG_022
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_02_L1
	//_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_02_L2
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_02_R1
	//_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_02_R2
	_SE_WAIT				SEQ_SE_DP_UG_022
	_SE_PLAY				SEQ_SE_DP_DENDOU
	_RET


/********************************************************************/
/*						���ӂԂ�+��e�[�v���o						*/
/********************************************************************/
se_kami_03:
	_SE_PLAY				SEQ_SE_DP_UG_022
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_02_L1
	//_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_02_L2
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_02_R1
	//_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMI_02_R2
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMITAPE_L
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_KAMITAPE_R
	_SE_WAIT				SEQ_SE_DP_UG_022
	_SE_PLAY				SEQ_SE_DP_DENDOU
	_RET


/********************************************************************/
/*						�u���[���̗���								*/
/********************************************************************/
ev_stage_leader_1st_02:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_stage_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*�o��*/
	_CALL				ev_stage_leader_appear

	/*�퓬�O�̃��b�Z�[�W(1����)*/
	_TALKMSG			msg_stage_room_boss_02
	_TALK_CLOSE

	/*���̈ʒu�ɖ߂�*/
	_CALL			ev_stage_leader_msg_after

	_JUMP				ev_stage_room_go_sub
	_END

ev_stage_leader_2nd_02:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_stage_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*�o��*/
	_CALL				ev_stage_leader_appear

	/*�퓬�O�̃��b�Z�[�W(2����)*/
	_TALKMSG			msg_stage_room_boss_03
	_TALK_CLOSE

	/*���̈ʒu�ɖ߂�*/
	_CALL			ev_stage_leader_msg_after

	_JUMP				ev_stage_room_go_sub
	_END

/*���̈ʒu�ɖ߂�*/
ev_stage_leader_msg_after:
	_OBJ_ANIME		OBJID_ETC,anm_player_go_btl_room_101
	_OBJ_ANIME_WAIT

	_TIME_WAIT			30,FSW_ANSWER

	/*���ґO�փA�j��*/
	_OBJ_ANIME		OBJID_MINE,anm_player_go_btl_room
	_OBJ_ANIME		OBJID_ETC,anm_player_go_btl_room_etc
	_OBJ_ANIME_WAIT
	_RET

/*�o��*/
ev_stage_leader_appear:

	_CALL				ev_stage_trainer_set_brain

	/*���d�ʂ��\��*/
	_STAGE_TOOL			FS_ID_EFF_FRAME_ON,1,0,FSW_ANSWER

	/*�|�P�����\��*/
	_TEMOTI_POKE_ACT_ADD	FSW_LOCAL2,STAGE_POKE_ACT_X,STAGE_POKE_ACT_Y,POKE_DISP_MANAGER_ID_0, 0, 0x0000

	_WND_SET			10,160,230,192,0,0						/*�E�B���h�EOFF*/

	_TIME_WAIT			10,FSW_ANSWER

	_WND_SET			10,160,230,192,54,1						/*�E�B���h�EON*/

	/*������܂Ŗ߂�*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center_scroll_back
	_OBJ_ANIME_WAIT

	/*�p�[�e�B�N�����o*/
	_PARTICLE_SPA_LOAD		SPAWORK_0,STAGE_SPA, FSS_CAMERA_ORTHO

	/*���E�����X�|�b�g���C�g*/
	_PARTICLE_ADD_EMITTER	SPAWORK_0,STAGE_BS_SPOT00
	_PARTICLE_ADD_EMITTER	SPAWORK_0,STAGE_BS_SPOT00_BG	/*�X�|�b�g���C�g*/
	//_PARTICLE_WAIT
	//_TIME_WAIT			60,FSW_ANSWER
	_TIME_WAIT			90,FSW_ANSWER

	_PARTICLE_ADD_EMITTER	SPAWORK_0,STAGE_BS_SPOT01		/*�X�|�b�g���C�g*/

	/*�u���[���o��*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain_3
	_OBJ_ANIME_WAIT

	_SE_PLAY			SEQ_SE_DP_DENDOU					/*����*/
	//_TIME_WAIT			30,FSW_ANSWER
	_SE_WAIT			SEQ_SE_DP_DENDOU

	/*�u���[���������܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center_scroll_2
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain_2
	_OBJ_ANIME_WAIT

	_SE_PLAY			SEQ_SE_DP_DENDOU					/*����*/
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYAPASYA03
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYAPASYA03BG
	_SE_WAIT			SEQ_SE_DP_DENDOU

	_WND_SET			10,160,230,192,10,0					/*�E�B���h�EOFF*/

	/*�u���[���������܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center_scroll_3
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain_2_2
	_OBJ_ANIME_WAIT

	_SE_PLAY			SEQ_SE_DP_DENDOU					/*����*/
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYAPASYA03
	_PARTICLE_ADD_EMITTER	SPAWORK_0, STAGE_BS_PASYAPASYA03BG
	_SE_WAIT			SEQ_SE_DP_DENDOU

	/*�u���[���������܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center_scroll_4
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain_2_3
	_OBJ_ANIME_WAIT

	/*�|�P�����폜*/
	_TEMOTI_POKE_ACT_DEL	POKE_DISP_MANAGER_ID_0

	/*�X�|�b�g���C�g������*/
	//_PARTICLE_SPA_EXIT	SPAWORK_0

	//_PARTICLE_SPA_LOAD	SPAWORK_0, STAGE_SPA, FSS_CAMERA_ORTHO
	_PARTICLE_ADD_EMITTER	SPAWORK_0,STAGE_BS_SPOT02		/*�L����X�|�b�g���C�g*/
	//_PARTICLE_WAIT
	_TIME_WAIT			30,FSW_ANSWER

	/*�z���C�g�A�E�g*/
	_WHITE_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*���d�ʂ�\��*/
	_STAGE_TOOL			FS_ID_EFF_FRAME_ON,0,0,FSW_ANSWER

	/*�X�|�b�g���C�g�A�X�g���{������*/
	_PARTICLE_SPA_EXIT	SPAWORK_0

	/*�z���C�g�C��*/
	_WHITE_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*��l���̑O�ɗ���*/
	_OBJ_ANIME		OBJID_ETC,anm_player_go_btl_room_100
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*						�u���[���ɏ�����̗���						*/
/********************************************************************/
ev_stage_leader_1st_03:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_stage_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*�u���[����|������*/
	_PARTICLE_SPA_LOAD	SPAWORK_0, STAGE_SPA, FSS_CAMERA_ORTHO
	_CALL				se_kami_03						/*���e�[�v*/
	_PARTICLE_WAIT
	_PARTICLE_SPA_EXIT	SPAWORK_0

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_STAGE,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_NONE,ev_stage_meory_print_put_1st

	/*�퓬��̃��b�Z�[�W(1����)*/
	_TALKMSG			msg_stage_room_boss_04
	_TALK_CLOSE
	_JUMP				ev_stage_battle_2_sub
	_END

/*��t��1st�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_stage_meory_print_put_1st:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_STAGE,MEMORY_PRINT_PUT_1ST
	_RET

ev_stage_leader_2nd_03:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_stage_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

#if 1
	/*�u���[����|������*/
	_PARTICLE_SPA_LOAD	SPAWORK_0, STAGE_SPA, FSS_CAMERA_ORTHO
	_CALL				se_kami_03						/*���e�[�v*/
	_PARTICLE_WAIT
	_PARTICLE_SPA_EXIT	SPAWORK_0
#endif

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_STAGE,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_PUT_OK_1ST,ev_stage_meory_print_put_2nd

	/*�퓬��̃��b�Z�[�W(2����)*/
	_TALKMSG			msg_stage_room_boss_05
	_TALK_CLOSE
	_JUMP				ev_stage_battle_2_sub
	_END

/*��t��2nd�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_stage_meory_print_put_2nd:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_STAGE,MEMORY_PRINT_PUT_2ND
	_RET


/********************************************************************/
/*					�A�������擾(FSW_ANSWER�g�p)					*/
/********************************************************************/
ev_stage_rensyou_get:
	/*�A�����擾*/
	_STAGE_TOOL			FS_ID_GET_RENSYOU,0,0,FSW_ANSWER

	/*���ł�9999�̎��́A0 �I���W���̕␳�������Ȃ�*/
	_IFVAL_JUMP			FSW_ANSWER,GE,STAGE_RENSYOU_MAX,ev_stage_rensyou_ret

	_ADD_WK				FSW_ANSWER,1
	_RET

ev_stage_rensyou_ret:
	_RET


/********************************************************************/
/*																	*/
/********************************************************************/
ev_stage_comm_command_initialize_1:
	_TIME_WAIT			1,FSW_ANSWER
	_STAGE_TOOL			FS_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_INIT_1
	_COMM_RESET
	_RET

ev_stage_comm_command_initialize_2:
	_TIME_WAIT			1,FSW_ANSWER
	_STAGE_TOOL			FS_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_INIT_2
	_COMM_RESET
	_RET

ev_stage_comm_command_initialize_3:
	_TIME_WAIT			1,FSW_ANSWER
	_STAGE_TOOL			FS_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_STAGE_INIT_3
	_COMM_RESET
	_RET



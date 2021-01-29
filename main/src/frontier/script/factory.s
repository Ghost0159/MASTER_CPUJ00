//==============================================================================
/**
 * @file	factory.s
 * @brief	�u�t�@�N�g���[�T�����v�t�����e�B�A�X�N���v�g
 * @author	nohara
 * @date	2007.04.11
 */
//==============================================================================

	.text

	.include	"../frontier_def.h"
	.include	"../factory_def.h"
	.include	"../frontier_seq_def.h"
	.include	"../../../include/msgdata/msg_factory_room.h"
	.include	"../../../include/field/evwkdef.h"
	.include	"../../fielddata/script/saveflag.h"
	.include	"../../fielddata/script/savework.h"
	.include	"../../field/syswork_def.h"
	.include	"../../particledata/pl_frontier/frontier_particle_def.h"	//SPA
	.include	"../../particledata/pl_frontier/frontier_particle_lst.h"	//EMIT


//--------------------------------------------------------------------
//					     �X�N���v�g�{��
//
//	FSW_PARAM0	���񐔌v�Z�p
//	FSW_PARAM1	���X�g�Ŏg�p
//	FSW_PARAM2	�G�g���[�i�[1��OBJ�R�[�h
//	FSW_PARAM3	�G�g���[�i�[2��OBJ�R�[�h
//	FSW_PARAM6	�ē�����l
//	FSW_LOCAL2	�ʐM��`
//	FSW_LOCAL3	�L�^������
//	FSW_LOCAL5	�^��v�Z�Ɏg�p
//	FSW_LOCAL7	��l���̌�����
//
//--------------------------------------------------------------------
_EVENT_DATA		fss_factory_start	//��ԏ��EVENT_DATA�͎������s
_EVENT_DATA_END						//�I��

//--------------------------------------------------------------------
//					     �f�o�b�N��`
//--------------------------------------------------------------------
//#define DEBUG_BTL_OFF					//�o�g���I�t�f�o�b�N
//#define DEBUG_BTL_LOSE_OFF			//�o�g���s�k�I�t�f�o�b�N
//#define DEBUG_7BTL_OFF				//�o�g��7��I�t�f�o�b�N


/********************************************************************/
//
//		SCENE_FACTORY:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
_RESOURCE_LABEL	default_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		ASSISTANTW,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		FSW_LOCAL7,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	default_set_resource_multi
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		ASSISTANTW,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	pcwoman2_set_resource
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_FACTORY:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
#define OBJID_PLAYER				(0)
#define OBJID_SIO_USER_0			(1)
#define OBJID_SIO_USER_1			(2)
#define OBJID_ASSISTANTW			(3)
#define OBJID_MINE					(4)
#define OBJID_PCWOMAN2				(5)
#define OBJID_ETC					(98)
#define OBJID_ETC2					(99)

//�X�N���v�gID(����Ή��\��)
#define EVENTID_TEST_SCR_OBJ1		(1)

_ACTOR_LABEL	default_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*14,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_ACTOR_DATA				OBJID_ASSISTANTW,ASSISTANTW,WF2DMAP_WAY_C_DOWN, \
							8*16,8*10,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*16,8*24,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	default_set_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*14,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_UP,8*16,8*24,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_UP,8*18,8*24,ON
	_ACTOR_DATA				OBJID_ASSISTANTW,ASSISTANTW,WF2DMAP_WAY_C_DOWN, \
							8*16,8*10,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*16,8*24,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	pcwoman2_set_actor
	_ACTOR_DATA				OBJID_PCWOMAN2,FSW_PARAM6,WF2DMAP_WAY_C_RIGHT, \
							8*12,8*22,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

//�퓬��ɖ߂��Ă�����
_ACTOR_LABEL	btl_after_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*14,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_ACTOR_DATA				OBJID_ASSISTANTW,ASSISTANTW,WF2DMAP_WAY_C_DOWN, \
							8*16,8*10,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*16,8*12,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	btl_after_set_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*14,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_UP,8*16,8*12,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_UP,8*18,8*12,ON
	_ACTOR_DATA				OBJID_ASSISTANTW,ASSISTANTW,WF2DMAP_WAY_C_DOWN, \
							8*16,8*10,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*16,8*12,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

/********************************************************************/
//
//	SCENE_FACTORY_BTL:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
_RESOURCE_LABEL	default_scene2_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_LOCAL7, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	default_scene2_resource_multi
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_FACTORY_BTL:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
_ACTOR_LABEL	default_scene2_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*14,OFF	//�������g�̃A�N�^�[�Z�b�g
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*16,8*24,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	default_scene2_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*14,OFF	//�������g�̃A�N�^�[�Z�b�g
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_UP,8*16,8*24,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_UP,8*18,8*24,ON
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*16,8*24,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

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
_ACTOR_LABEL	etc_set_actor
	_ACTOR_DATA			OBJID_ETC,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							8*16,8*8,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	etc_set_actor2
	_ACTOR_DATA			OBJID_ETC,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							8*18,8*8,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA			OBJID_ETC2,FSW_PARAM3,WF2DMAP_WAY_C_DOWN, \
							8*16,8*8,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

/********************************************************************/
//
//					�A�j���[�V�����f�[�^
//
/********************************************************************/

//--------------------------------------------------------------------
//�����^����t�̑O�܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_center
	_ANIME_DATA	FC_WALK_U_8F,6
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center_multi
	_ANIME_DATA	FC_WALK_U_8F,6
	_ANIME_DATA_END

//--------------------------------------------------------------------
//������
//--------------------------------------------------------------------
_ANIME_LABEL anm_pcwoman2_go_center
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�o�g�����[���ֈړ�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_btl_room
	_ANIME_DATA	FC_WAIT_8F,2
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_btl_room_multi
	_ANIME_DATA	FC_WAIT_8F,2
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
_ANIME_LABEL anm_man_left_walk
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_DIR_D,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�b������
//--------------------------------------------------------------------
_ANIME_LABEL anm_stay_walk_left
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�o�g�����[���̒����܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_btl_center
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_L_8F,5
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_btl_center_multi
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_L_8F,6
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�g���[�i�[���o�g�����[���̒����܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL etc_anime_room_in
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,5
	_ANIME_DATA	FC_WALK_D_8F,3
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_sio
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,4
	_ANIME_DATA	FC_WALK_D_8F,4
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_multi
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,5
	_ANIME_DATA	FC_WALK_D_8F,3
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,5
	_ANIME_DATA	FC_WALK_D_8F,3
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain_02
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�g���[�i�[���o�g�����[������o�čs��
//--------------------------------------------------------------------
_ANIME_LABEL etc_anime_go_out
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_WALK_L_8F,4
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_go_out_sio
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_WALK_L_8F,3
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_go_out_multi
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_WALK_L_8F,4
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�L�����L����
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_kyorokyoro
	_ANIME_DATA	FC_STAY_WALK_U_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA	FC_WAIT_4F,1
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA	FC_WAIT_4F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END


/********************************************************************/
//
//						�����^���`�ΐ�`����
//
/********************************************************************/
fss_factory_start:
	//_EVENT_START

	/*lobby�őI�������^�C�v�ƃ��x�����K�v*/
	_SAVE_EVENT_WORK_GET	WK_SCENE_FACTORY_TYPE,FSW_LOCAL1	/*type*/
	_SAVE_EVENT_WORK_GET	WK_SCENE_FACTORY_LEVEL,FSW_LOCAL2	/*level*/
	_SAVE_EVENT_WORK_GET	LOCALWORK3,FSW_LOCAL3				/*init*/

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	/*�����ł͂܂����[�N���m�ۂ���Ă��Ȃ�*/
	//_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	//_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_start_multi
	_IFVAL_JUMP		FSW_LOCAL1,EQ,FACTORY_TYPE_MULTI,ev_factory_start_multi
	_IFVAL_JUMP		FSW_LOCAL1,EQ,FACTORY_TYPE_WIFI_MULTI,ev_factory_start_multi

	_LDVAL				FSW_PARAM6,BFSM
	_CHAR_RESOURCE_SET	default_set_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_set_actor			/*�A�N�^�[�o�^*/
	_JUMP			ev_factory_start_data_set
	_END

ev_factory_start_multi:

	/*���s�Ɋ֌W�Ȃ��ʐM�t�����e�B�A�ɒ���(�ʐM�͒��f���Ȃ��̂Ő擪�ŏ������Ă����v)*/
	_SCORE_ADD			SCORE_ID_FRONTIER_COMM

	_LDVAL				FSW_PARAM6,BFSW1
	_CHAR_RESOURCE_SET	default_set_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_set_actor_multi		/*�A�N�^�[�o�^*/
	_JUMP			ev_factory_start_data_set
	_END

ev_factory_start_data_set:

	/*�U����*/
	_CHAR_RESOURCE_SET	pcwoman2_set_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			pcwoman2_set_actor			/*�A�N�^�[�o�^*/

	/*���[�N�m��*/
	_BATTLE_REC_INIT
	_FACTORY_WORK_ALLOC	FSW_LOCAL3,FSW_LOCAL1,FSW_LOCAL2

	/*���[���ǉ�*/
	_FACTORY_TOOL	FA_ID_ADD_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*�ʐM�F��{�����Ƃ�*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_multi_comm_basic

	_JUMP			ev_factory_start_2
	_END

/*�ʐM�F��{�����Ƃ�*/
ev_factory_multi_comm_basic:
	_CALL			ev_factory_comm_command_initialize_1
	_JUMP			ev_factory_multi_comm_basic_retry
	_END

ev_factory_multi_comm_basic_retry:
	_LDVAL			FSW_LOCAL2,FACTORY_COMM_BASIC
	_FACTORY_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_multi_comm_basic_retry		/*���M���s�A�đ��M��*/

	_FACTORY_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_BASIC_AFTER
	_COMM_RESET

	_JUMP			ev_factory_start_2
	_END


/********************************************************************/
//
/********************************************************************/
ev_factory_start_2:
	/*���[�N������*/
	_FACTORY_WORK_INIT	FSW_LOCAL3

	/*�ʐM�F�g���[�i�[�����Ƃ�*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_multi_comm_tr

	_JUMP			ev_factory_start_3
	_END

/*�ʐM�F�g���[�i�[�����Ƃ�*/
ev_factory_multi_comm_tr:
	_LDVAL			FSW_LOCAL2,FACTORY_COMM_TR
	_FACTORY_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_multi_comm_tr		/*���M���s�A�đ��M��*/

	_FACTORY_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_TR_AFTER
	_COMM_RESET

	_JUMP			ev_factory_start_3
	_END


/********************************************************************/
//
/********************************************************************/
ev_factory_start_3:
	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�u���낭����v�Ŏg�p���郏�[�N�Ȃ̂ŃN���A���Ă���*/
	_LDVAL			FSW_LOCAL3,0

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_start_3_multi

	/*�����܂ŕ���*/
	//_OBJ_ANIME		OBJID_PLAYER,anm_player_go_center
	_OBJ_ANIME		OBJID_MINE,anm_player_go_center
	_OBJ_ANIME		OBJID_PCWOMAN2,anm_pcwoman2_go_center
	_OBJ_ANIME_WAIT

	_JUMP			ev_factory_room_start
	_END

ev_factory_start_3_multi:

	/*�����܂ŕ���*/
	//_OBJ_ANIME		OBJID_PLAYER,anm_player_go_center
	_OBJ_ANIME		OBJID_MINE,anm_player_go_center
	_OBJ_ANIME		OBJID_PCWOMAN2,anm_pcwoman2_go_center
	_OBJ_ANIME		OBJID_SIO_USER_0,anm_player_go_center
	_OBJ_ANIME		OBJID_SIO_USER_1,anm_player_go_center_multi
	_OBJ_ANIME_WAIT

	_JUMP			ev_factory_room_start
	_END

ev_factory_room_start:

	/*�U�����폜*/
	_ACTOR_FREE		OBJID_PCWOMAN2
	_CHAR_RESOURCE_FREE	FSW_PARAM6

	/*�����l�ڂ��擾*/
	_FACTORY_TOOL	FA_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_room_renshou_0		/*0		*/

	/*�L�^�������[�N�Ƀf�[�^���������܂�Ă��Ȃ��̂Ń��j���[�\�����Ȃ�*/
	_LDVAL			FSW_LOCAL3,1
	_JUMP			ev_factory_room_saikai							/*1-7	*/
	_END


/********************************************************************/
/*							�A���� 0								*/
/********************************************************************/
ev_factory_room_renshou_0:
	/*�ΐ�g���[�i�[�̏����擾*/
	_CALL			ev_factory_room_tr_info_msg
	_JUMP			ev_factory_room_go_rental
	_END


/********************************************************************/
/*						�g���[�i�[���								*/
/********************************************************************/
ev_factory_room_tr_info_msg:

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_multi_room_tr_info_msg

	_JUMP			ev_factory_room_tr_info_msg_2
	_END


/********************************************************************/
/*					�g���[�i�[���(�G�g���[�i�[2�l)					*/
/********************************************************************/
ev_factory_multi_room_tr_info_msg:
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_ENEMY_BEFORE
	_COMM_RESET
	_JUMP			ev_factory_multi_room_tr_info_msg_retry
	_END

ev_factory_multi_room_tr_info_msg_retry:
	/*�ʐM�F�G�|�P�������Ƃ�*/
	_LDVAL			FSW_LOCAL2,FACTORY_COMM_ENEMY
	_FACTORY_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_multi_room_tr_info_msg_retry	/*���M���s�A�đ��M��*/

	_FACTORY_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_ENEMY_AFTER
	_COMM_RESET

	/*�G�|�P��������*/
	_FACTORY_TOOL	FA_ID_ENEMY_POKE_CREATE,0,0,FSW_ANSWER

	/*���񐔂��擾*/
	_FACTORY_TOOL	FA_ID_GET_LAP,0,0,FSW_ANSWER
	_LDWK			FSW_PARAM0,FSW_ANSWER
	_IFVAL_CALL		FSW_PARAM0,EQ,0,ev_factory_multi_lap_0	/*1����*/
	_IFVAL_CALL		FSW_PARAM0,EQ,1,ev_factory_multi_lap_1	/*2����*/
	_IFVAL_CALL		FSW_PARAM0,EQ,2,ev_factory_multi_lap_2	/*3����*/
	_IFVAL_CALL		FSW_PARAM0,EQ,3,ev_factory_multi_lap_3	/*4����*/
	_IFVAL_CALL		FSW_PARAM0,GE,4,ev_factory_multi_lap_4	/*5���ڈȍ~*/
	_RET

/*1����*/
ev_factory_multi_lap_0:
	_FACTORY_TOOL	FA_ID_GET_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	1,FSW_ANSWER,0,0
	_FACTORY_TOOL	FA_ID_GET_MONSNO,1,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	2,FSW_ANSWER,0,0

	_FACTORY_TOOL	FA_ID_GET_MONSNO,2,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	3,FSW_ANSWER,0,0
	_FACTORY_TOOL	FA_ID_GET_MONSNO,3,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	4,FSW_ANSWER,0,0

	_TALKMSG	msg_factory_room_36
	_RET

/*2����*/
ev_factory_multi_lap_1:
	_FACTORY_TOOL	FA_ID_GET_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	1,FSW_ANSWER,0,0

	_FACTORY_TOOL	FA_ID_GET_MONSNO,2,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	2,FSW_ANSWER,0,0

	_TALKMSG	msg_factory_room_37
	_RET

/*3����*/
ev_factory_multi_lap_2:
	_FACTORY_TOOL	FA_ID_GET_WAZANO,0,0,FSW_ANSWER
	_WAZA_NAME		0,FSW_ANSWER

	_FACTORY_TOOL	FA_ID_GET_WAZANO,2,0,FSW_ANSWER
	_WAZA_NAME		1,FSW_ANSWER

	_TALKMSG	msg_factory_room_38
	_RET

/*4����*/
ev_factory_multi_lap_3:
	/*�g���[�i�[�P���Q�̂ǂ��炩�̃����_�������A�P�A�Q�Ƃ��m��Ȃ��̂łǂ���ł������͂�*/
	_FACTORY_TOOL	FA_ID_GET_WAZANO,0,0,FSW_ANSWER
	_WAZA_NAME		0,FSW_ANSWER

	_TALKMSG	msg_factory_room_39
	_RET

/*5����*/
ev_factory_multi_lap_4:
	_JUMP			ev_factory_lap_4
	_END


/********************************************************************/
/*					�g���[�i�[���(�G�g���[�i�[1�l)					*/
/********************************************************************/
ev_factory_room_tr_info_msg_2:
	/*���񐔂��擾*/
	_FACTORY_TOOL	FA_ID_GET_LAP,0,0,FSW_ANSWER
	_LDWK			FSW_PARAM0,FSW_ANSWER
	_IFVAL_CALL		FSW_PARAM0,EQ,0,ev_factory_lap_0	/*1����*/
	_IFVAL_CALL		FSW_PARAM0,EQ,1,ev_factory_lap_1	/*2����*/
	_IFVAL_CALL		FSW_PARAM0,EQ,2,ev_factory_lap_2	/*3����*/
	_IFVAL_CALL		FSW_PARAM0,EQ,3,ev_factory_lap_3	/*4����*/
	_IFVAL_CALL		FSW_PARAM0,GE,4,ev_factory_lap_4	/*5���ڈȍ~*/
	_RET

/*1����*/
ev_factory_lap_0:
	//_FACTORY_TOOL	FA_ID_GET_WAZANO,0,0,FSW_ANSWER
	//_WAZA_NAME		0,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_GET_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	1,FSW_ANSWER,0,0

	//_FACTORY_TOOL	FA_ID_GET_WAZANO,1,0,FSW_ANSWER
	//_WAZA_NAME		2,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_GET_MONSNO,1,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	3,FSW_ANSWER,0,0

	//_FACTORY_TOOL	FA_ID_GET_WAZANO,2,0,FSW_ANSWER
	//_WAZA_NAME		4,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_GET_MONSNO,2,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	5,FSW_ANSWER,0,0

	_TALKMSG	msg_factory_room_30
	_RET

/*2����*/
ev_factory_lap_1:
	//_FACTORY_TOOL	FA_ID_GET_WAZANO,0,0,FSW_ANSWER
	//_WAZA_NAME		0,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_GET_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	1,FSW_ANSWER,0,0

	//_FACTORY_TOOL	FA_ID_GET_WAZANO,1,0,FSW_ANSWER
	//_WAZA_NAME		2,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_GET_MONSNO,1,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	3,FSW_ANSWER,0,0

	_TALKMSG	msg_factory_room_31
	_RET

/*3����*/
ev_factory_lap_2:
	_FACTORY_TOOL	FA_ID_GET_WAZANO,0,0,FSW_ANSWER
	_WAZA_NAME		0,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_GET_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA	1,FSW_ANSWER,0,0

	_TALKMSG	msg_factory_room_32
	_RET

/*4����*/
ev_factory_lap_3:
	_FACTORY_TOOL	FA_ID_GET_WAZANO,0,0,FSW_ANSWER
	_WAZA_NAME		0,FSW_ANSWER
	//_FACTORY_TOOL	FA_ID_GET_POKE_TYPE,0,0,FSW_ANSWER
	//_TYPE_NAME		0,FSW_ANSWER

	//_FACTORY_TOOL	FA_ID_GET_POKE_TYPE,1,0,FSW_ANSWER
	//_TYPE_NAME		1,FSW_ANSWER

	//_FACTORY_TOOL	FA_ID_GET_POKE_TYPE,2,0,FSW_ANSWER
	//_TYPE_NAME		2,FSW_ANSWER

	_TALKMSG	msg_factory_room_33
	_RET

/*5����*/
ev_factory_lap_4:
	_FACTORY_TOOL	FA_ID_GET_LARGE_TYPE,0,0,FSW_ANSWER

	_IFVAL_CALL		FSW_ANSWER,NE,0xff,ev_factory_lap_4_0		/*�����^�C�v������*/
	_IFVAL_CALL		FSW_ANSWER,EQ,0xff,ev_factory_lap_4_1		/*�����^�C�v���Ȃ�*/
	_RET

ev_factory_lap_4_0:
	_TYPE_NAME		0,FSW_ANSWER
	_TALKMSG	msg_factory_room_34
	_RET

ev_factory_lap_4_1:
	_TALKMSG	msg_factory_room_35
	_RET

ev_factory_room_go_rental:
	/*�u�����^�����@�s���܂��v*/
	_TALKMSG	msg_factory_room_1

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_multi_room_go_rental

	_JUMP			ev_factory_room_go_rental_2
	_END

ev_factory_multi_room_go_rental:
	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_factory_room_wait
	_JUMP			ev_factory_multi_room_go_rental_retry
	_END

ev_factory_multi_room_go_rental_retry:
	_LDVAL			FSW_LOCAL2,FACTORY_COMM_RENTAL
	_FACTORY_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_multi_room_go_rental_retry	/*���M���s�A�đ��M��*/

	_FACTORY_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�����^���|�P��������*/
	_FACTORY_TOOL	FA_ID_RENTAL_POKE_CREATE,0,0,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_RENTAL
	_COMM_RESET		/*��ʐ؂�ւ��O�ɏ�����*/
	_JUMP			ev_factory_room_go_rental_2
	_END

ev_factory_room_go_rental_2:
	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�����^���Ăяo��*/
	_TALK_CLOSE

	/*���[���폜*/
	_FACTORY_TOOL	FA_ID_DEL_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_factory_room_rental_sync

	_FACTORY_RENTAL_CALL
	//_SET_MAP_PROC

	/*�����^�����邾���ł������񐔂�+1�����*/
	_FACTORY_TOOL	FA_ID_INC_TRADE_COUNT,0,0,FSW_ANSWER

	_FACTORY_RENTAL_PARTY_SET		/*�����^��POKEPARTY�̃Z�b�g*/

	/*���[���ǉ�*/
	_FACTORY_TOOL	FA_ID_ADD_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_factory_room_go_rental_multi

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_JUMP			ev_factory_room_go
	_END

ev_factory_room_go_rental_multi:
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_RENTAL_AFTER
	_COMM_RESET
	_RET

/*�ʐM�}���`*/
ev_factory_room_rental_sync:
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_RENTAL_CALL
	_COMM_RESET
	_RET


/********************************************************************/
/*						�o�g���֌�����								*/
/********************************************************************/
ev_factory_room_go:
	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_go_multi

	/*�u����ł́@�����Ɂ@�ǂ����v*/
	_TALKMSG		msg_factory_room_2
	_AB_KEYWAIT
	_TALK_CLOSE

	/*�ړ��A�j��*/
	_OBJ_ANIME		OBJID_ASSISTANTW,anm_man_left_walk
	//_OBJ_ANIME		OBJID_PLAYER,anm_player_go_btl_room
	_OBJ_ANIME		OBJID_MINE,anm_player_go_btl_room
	_OBJ_ANIME_WAIT

	_JUMP			ev_factory_room_go_2
	_END

ev_factory_room_go_multi:

	/*�u����ł́@�����Ɂ@�ǂ����v*/
	_TALKMSG_NOSKIP	msg_factory_room_2
	_TIME_WAIT		FRONTIER_COMMON_WAIT,FSW_ANSWER
	_TALK_CLOSE

	_CALL			ev_factory_comm_command_initialize_2

	/*�ړ��A�j��*/
	_OBJ_ANIME		OBJID_ASSISTANTW,anm_man_left_walk
	//_OBJ_ANIME		OBJID_PLAYER,anm_player_go_btl_room
	_OBJ_ANIME		OBJID_MINE,anm_player_go_btl_room
	_OBJ_ANIME		OBJID_SIO_USER_0,anm_player_go_btl_room
	_OBJ_ANIME		OBJID_SIO_USER_1,anm_player_go_btl_room_multi
	_OBJ_ANIME_WAIT

	_JUMP			ev_factory_room_go_2
	_END

ev_factory_room_go_2:
	/*�ΐ핔���փ}�b�v�؂�ւ�*/
	_SE_PLAY		SEQ_SE_DP_KAIDAN2
	_SE_WAIT		SEQ_SE_DP_KAIDAN2
	_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_go_2_multi

	_ACTOR_FREE		OBJID_PLAYER
	_ACTOR_FREE		OBJID_ASSISTANTW
	_ACTOR_FREE		OBJID_MINE
	_CHAR_RESOURCE_FREE	ASSISTANTW
	//_CHAR_RESOURCE_FREE	FSW_LOCAL7

	/*���[���폜*/
	_FACTORY_TOOL	FA_ID_DEL_MAP_RAIL_TCB,0,0,FSW_ANSWER

	_MAP_CHANGE		FSS_SCENEID_FACTORY_BTL

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	_CHAR_RESOURCE_SET	default_scene2_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_scene2_actor		/*�A�N�^�[�o�^*/
	
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�����܂ŕ���*/
	//_OBJ_ANIME		OBJID_PLAYER,anm_player_go_btl_center
	_OBJ_ANIME		OBJID_MINE,anm_player_go_btl_center
	_OBJ_ANIME_WAIT

	/*�ΐ핔���̒n�ʃp���b�g�؂�ւ�*/
	_CALL			ev_btl_room_pal_chg

	/*�u���[���o�ꂩ�`�F�b�N*/
	_FACTORY_TOOL	FA_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_leader_1st_02
	_IFVAL_JUMP		FSW_ANSWER,EQ,2,ev_factory_leader_2nd_02

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_factory_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*�G�g���[�i�[�������܂ŕ���*/
	_CALL			ev_factory_trainer_set_1
	_OBJ_ANIME		OBJID_ETC,etc_anime_room_in
	_OBJ_ANIME_WAIT

	_TIME_WAIT		15,FSW_ANSWER

	_OBJ_ANIME			OBJID_ETC,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_FACTORY_TOOL	FA_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_FACTORY_APPEAR	0
	_AB_KEYWAIT
	_TALK_CLOSE
	_JUMP			ev_factory_room_go_2_sub
	_END

ev_factory_room_go_2_sub:
	_JUMP			ev_factory_battle
	_END

ev_factory_room_go_2_multi:
	_ACTOR_FREE		OBJID_SIO_USER_0
	_ACTOR_FREE		OBJID_SIO_USER_1
	_ACTOR_FREE		OBJID_ASSISTANTW
	_ACTOR_FREE		OBJID_MINE
	_CHAR_RESOURCE_FREE	ASSISTANTW

	/*���[���폜*/
	_FACTORY_TOOL	FA_ID_DEL_MAP_RAIL_TCB,0,0,FSW_ANSWER

	_MAP_CHANGE		FSS_SCENEID_FACTORY_BTL

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	_CHAR_RESOURCE_SET	default_scene2_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_scene2_actor_multi		/*�A�N�^�[�o�^*/
	
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�����܂ŕ���*/
	//_OBJ_ANIME		OBJID_PLAYER,anm_player_go_btl_center
	_OBJ_ANIME		OBJID_MINE,anm_player_go_btl_center
	_OBJ_ANIME		OBJID_SIO_USER_0,anm_player_go_btl_center
	_OBJ_ANIME		OBJID_SIO_USER_1,anm_player_go_btl_center_multi
	_OBJ_ANIME_WAIT

	/*�ΐ핔���̒n�ʃp���b�g�؂�ւ�*/
	_CALL			ev_btl_room_pal_chg

	/*�G�g���[�i�[�������܂ŕ���*/
	_CALL			ev_factory_trainer_set_2
	_OBJ_ANIME		OBJID_ETC,etc_anime_room_in_sio
	_OBJ_ANIME		OBJID_ETC2,etc_anime_room_in_multi
	_OBJ_ANIME_WAIT

	_TIME_WAIT		15,FSW_ANSWER

	_OBJ_ANIME		OBJID_ETC,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_FACTORY_TOOL	FA_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_FACTORY_APPEAR	0
	_TIME_WAIT		30,FSW_ANSWER
	_TALK_CLOSE

	_OBJ_ANIME		OBJID_ETC2,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_FACTORY_TOOL	FA_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_FACTORY_APPEAR	1
	_TIME_WAIT		30,FSW_ANSWER
	_TALK_CLOSE

	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_factory_room_wait

	_JUMP			ev_factory_battle
	_END


/********************************************************************/
/*					�g���[�i�[�̌����ڃZ�b�g						*/
/********************************************************************/
ev_factory_trainer_set_1:
	_FACTORY_TOOL	FA_ID_GET_TR_OBJ_CODE,0,0,FSW_ANSWER
	_LDWK			FSW_PARAM2,FSW_ANSWER
	_CHAR_RESOURCE_SET	etc_set_resource
	_ACTOR_SET		etc_set_actor
	_RET

ev_factory_trainer_set_2:
	_FACTORY_TOOL	FA_ID_GET_TR_OBJ_CODE,0,0,FSW_ANSWER
	_LDWK			FSW_PARAM2,FSW_ANSWER
	_CHAR_RESOURCE_SET	etc_set_resource
	//_ACTOR_SET		etc_set_actor

	_FACTORY_TOOL	FA_ID_GET_TR_OBJ_CODE,1,0,FSW_ANSWER
	_LDWK			FSW_PARAM3,FSW_ANSWER
	_CHAR_RESOURCE_SET	etc_set_resource2
	_ACTOR_SET		etc_set_actor2
	_RET

ev_factory_trainer_set_brain:
	_LDVAL			FSW_PARAM2,BRAINS1
	_CHAR_RESOURCE_SET	etc_set_resource
	_ACTOR_SET		etc_set_actor
	_RET


/********************************************************************/
//
/********************************************************************/
ev_factory_battle:
	/*�t�F�[�h�A�E�g*/
	//_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_multi_battle

	/*�u���[���o�ꂩ�`�F�b�N*/
	_FACTORY_TOOL	FA_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_leader_1st_31
	_IFVAL_JUMP		FSW_ANSWER,EQ,2,ev_factory_leader_2nd_31

	_ENCOUNT_EFFECT	FR_ENCOUNT_EFF_CLOSE
	_JUMP			ev_factory_battle_2
	_END

ev_factory_leader_1st_31:
ev_factory_leader_2nd_31:
	_RECORD_INC		RECID_FRONTIER_BRAIN
	_BRAIN_ENCOUNT_EFFECT	FRONTIER_NO_FACTORY_LV50
	//_BRAIN_ENCOUNT_EFFECT	FRONTIER_NO_FACTORY_LV100
	_JUMP			ev_factory_battle_2
	_END

/*�ʐM�}���`*/
ev_factory_multi_battle:
	/*�ʐM�F�莝�����Ƃ�*/
	_LDVAL			FSW_LOCAL2,FACTORY_COMM_TEMOTI
	_FACTORY_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_multi_battle	/*���M���s�A�đ��M��*/

	_FACTORY_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�y�A�̃|�P��������*/
	_FACTORY_TOOL	FA_ID_PAIR_POKE_CREATE,0,0,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_BATTLE
	_COMM_RESET

	_TALK_CLOSE
	_ENCOUNT_EFFECT	FR_ENCOUNT_EFF_CLOSE
	_JUMP			ev_factory_battle_2
	_END

ev_factory_battle_2:

#ifndef DEBUG_FRONTIER_LOOP

#ifndef DEBUG_BTL_OFF	/************************************************/
	/*�퓬�Ăяo��*/
	_FACTORY_BATTLE_CALL
	_FACTORY_CALL_GET_RESULT
#endif	/****************************************************************/

#endif	//DEBUG_FRONTIER_LOOP

	/*�ΐ핔���̒n�ʃp���b�g�؂�ւ�*/
	_FACTORY_TOOL	FA_ID_BTL_ROOM_PAL_CHG,4,0,FSW_ANSWER

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

#ifndef DEBUG_FRONTIER_LOOP

#ifndef DEBUG_BTL_LOSE_OFF	/********************************************/
	/*�퓬���ʂŕ���*/
	_FACTORY_LOSE_CHECK		FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_lose		/*�s�k*/
#endif	/****************************************************************/

#endif	//DEBUG_FRONTIER_LOOP

	/*����*/
	_RECORD_INC		RECID_FACTORY_WIN

	/*�u���[���o�ꂩ�`�F�b�N*/
	_FACTORY_TOOL	FA_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_leader_1st_03
	_IFVAL_JUMP		FSW_ANSWER,EQ,2,ev_factory_leader_2nd_03

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_factory_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	_JUMP			ev_factory_battle_2_sub
	_END

ev_factory_battle_2_sub:
	_CALL			ev_factory_enemy_tr_go_out			/*�G�g���[�i�[�ޏ�A�j��*/
	_JUMP			ev_factory_battle_3
	_END

ev_factory_battle_3:
	_CALL			ev_factory_room_return			/*���������ɖ߂�*/
	_JUMP			ev_factory_battle_4
	_END

ev_factory_battle_4:

#ifdef DEBUG_FRONTIER_LOOP
	_JUMP			ev_factory_room_renshou_17
#endif	//DEBUG_FRONTIER_LOOP

	/*�����l��+1*/
	_FACTORY_TOOL	FA_ID_INC_ROUND,1,0,FSW_ANSWER

	/*�A����+1*/
	_FACTORY_TOOL	FA_ID_INC_RENSYOU,0,0,FSW_ANSWER

#ifndef DEBUG_7BTL_OFF	/********************************************/
	/*��7�l�ڂłȂ����͓��ɖ߂�*/
	_FACTORY_TOOL	FA_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,NE,7,ev_factory_room_renshou_17		/*�A������1-6�̎�*/
#endif	/************************************************************/

	_JUMP			ev_factory_room_7_win
	_END


/********************************************************************/
/*							7�A������(�L�^)							*/
/********************************************************************/
ev_factory_room_7_win:

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_room_7_win_bp_sub

	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,0,ev_factory_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_factory_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER										/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_7_win_bp
	_JUMP			ev_factory_room_7_win_rec_yes
	_END

ev_factory_room_7_win_bp_sub:
	_TALKMSG_NOSKIP	msg_factory_room_wait
	_JUMP			ev_factory_room_7_win_bp
	_END

/*�u�͂��v*/
ev_factory_room_7_win_rec_yes:
	_CALL			ev_factory_room_rec_win		/*�����̋L�^*/
	_JUMP			ev_factory_room_7_win_bp
	_END


/********************************************************************/
/*							7�A������(BP�擾)						*/
/********************************************************************/
ev_factory_room_7_win_bp:
	//7�A��(�N���A)�p�����[�^�Z�b�g
	_FACTORY_TOOL	FA_ID_SET_CLEAR,0,0,FSW_ANSWER

	/*7�A�������̂ŏI����*/
	//_LDVAL			WK_SCENE_FACTORY_LOBBY,1				/*7�A������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_FACTORY_LOBBY,1			/*7�A������n�܂�悤�ɂ��Ă���*/

	/*�u�o�g���g���[�h�@�V����@�Ƃ��ρ@���߂łƂ��v*/
	_TALKMSG		msg_factory_lobby_20

	_PLAYER_NAME	0											/*�v���C���[���Z�b�g*/

	/*�o�g���|�C���g�ǉ�*/
	_FACTORY_TOOL	FA_ID_GET_BP_POINT,0,0,FSW_ANSWER
	_NUMBER_NAME	1,FSW_ANSWER								/*���l�Z�b�g*/
	_BTL_POINT_ADD	FSW_ANSWER

	/*�u�a�o����������v*/
	_TALKMSG		msg_factory_lobby_21
	_ME_PLAY		ME_BP_GET
	_ME_WAIT

	_JUMP			ev_factory_room_bp_end
	_END


/********************************************************************/
/*							7�A�������I��							*/
/********************************************************************/
ev_factory_room_bp_end:
	_JUMP			ev_factory_lose_timing
	_END


/********************************************************************/
/*							�A���� 1-7								*/
/********************************************************************/
ev_factory_room_renshou_17:
	/*�u�|�P�������@�����ӂ��@���܂��傤�v*/
	_TALKMSG		msg_factory_room_3
	_ME_PLAY		SEQ_ASA
	_ME_WAIT

	/*�o�g���Ăяo���̐퓬�p�����[�^�𐶐����鎞�ɉ񕜏��������Ă���*/
	//_PC_KAIFUKU

	_JUMP			ev_factory_room_saikai
	_END


/********************************************************************/
/*						���f�Z�[�u����̍ĊJ						*/
/********************************************************************/
ev_factory_room_saikai:
	/*�u���[���o�ꂩ�`�F�b�N*/
	_FACTORY_TOOL	FA_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_leader_1st_01
	_IFVAL_JUMP		FSW_ANSWER,EQ,2,ev_factory_leader_2nd_01

	_JUMP			ev_factory_room_saikai_00
	_END

ev_factory_room_saikai_00:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_factory_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	_JUMP			ev_factory_room_saikai_02
	_END

ev_factory_room_saikai_02:
	/*�u���́@����ڂł���v*/
	_FACTORY_TOOL	FA_ID_GET_ROUND,0,0,FSW_ANSWER
	_ADD_WK			FSW_ANSWER,1
	_NUMBER_NAME	0,FSW_ANSWER
	_TALKMSG		msg_factory_room_4

	_JUMP			ev_factory_room_menu_make
	_END


/********************************************************************/
/*							���j���[����							*/
/********************************************************************/
ev_factory_room_menu_make:

	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_menu_make_comm

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,0,ev_factory_room_menu_make_04_set

	_JUMP			ev_factory_room_menu_make_03_set
	_END

/*�ʐM�̎��̃��j���[*/
ev_factory_room_menu_make_comm:

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_room_menu_make_01_set

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,0,ev_factory_room_menu_make_02_set

	_JUMP			ev_factory_room_menu_make_01_set
	_END

ev_factory_room_menu_make_01_set:
	_LDVAL				FSW_ANSWER,1
	_JUMP				ev_factory_room_menu_make_sub
	_END

ev_factory_room_menu_make_02_set:
	_LDVAL				FSW_ANSWER,2
	_JUMP				ev_factory_room_menu_make_sub
	_END

ev_factory_room_menu_make_03_set:
	_LDVAL				FSW_ANSWER,3
	_JUMP				ev_factory_room_menu_make_sub
	_END

ev_factory_room_menu_make_04_set:
	_LDVAL				FSW_ANSWER,4
	_JUMP				ev_factory_room_menu_make_sub
	_END

/*�ǂ̃��j���[�`���ɂ��邩*/
ev_factory_room_menu_make_sub:
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_factory_room_menu_make_01
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_factory_room_menu_make_02
	_IFVAL_JUMP			FSW_ANSWER,EQ,3,ev_factory_room_menu_make_03
	_IFVAL_JUMP			FSW_ANSWER,EQ,4,ev_factory_room_menu_make_04
	_END

ev_factory_room_menu_make_01:
	_BMPLIST_INIT_EX	24,13,0,0,FSW_PARAM1			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_factory_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0	/*�Â���*/
	_JUMP			ev_factory_room_yasumu_next
	_END

ev_factory_room_menu_make_02:
	_BMPLIST_INIT_EX	23,11,0,0,FSW_PARAM1			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_factory_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0	/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_factory_room_choice_02,FSEV_WIN_TALK_MSG_NONE,1	/*���낭����*/
	_JUMP			ev_factory_room_yasumu_next
	_END

ev_factory_room_menu_make_03:
	_BMPLIST_INIT_EX	24,11,0,0,FSW_PARAM1			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_factory_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0	/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_factory_room_choice_03,FSEV_WIN_TALK_MSG_NONE,2	/*�₷��*/
	_JUMP			ev_factory_room_yasumu_next
	_END

ev_factory_room_menu_make_04:
	_BMPLIST_INIT_EX	23,9,0,0,FSW_PARAM1				/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_factory_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0	/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_factory_room_choice_02,FSEV_WIN_TALK_MSG_NONE,1	/*���낭����*/
	_BMPLIST_MAKE_LIST	msg_factory_room_choice_03,FSEV_WIN_TALK_MSG_NONE,2	/*�₷��*/
	_JUMP			ev_factory_room_yasumu_next
	_END

ev_factory_room_yasumu_next:
	_BMPLIST_MAKE_LIST	msg_factory_room_choice_04,FSEV_WIN_TALK_MSG_NONE,3	/*���^�C�A*/
	_BMPLIST_START
	_JUMP			ev_factory_room_menu_sel
	_END


/********************************************************************/
/*							���j���[����							*/
/********************************************************************/
ev_factory_room_menu_sel:

#ifdef DEBUG_FRONTIER_LOOP
	_JUMP			ev_room_choice_01
#endif	//DEBUG_FRONTIER_LOOP

	_IFVAL_JUMP		FSW_PARAM1,EQ,0,ev_room_choice_01	/*�Â���*/
	_IFVAL_JUMP		FSW_PARAM1,EQ,1,ev_room_choice_02	/*���낭����*/
	_IFVAL_JUMP		FSW_PARAM1,EQ,2,ev_room_choice_03	/*�₷��*/
	_IFVAL_JUMP		FSW_PARAM1,EQ,3,ev_room_choice_04	/*���^�C�A*/
	_JUMP			ev_room_choice_04
	_END


/********************************************************************/
/*						�u�Â���v								*/
/********************************************************************/
ev_room_choice_01:
	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_room_choice_01_multi
	_JUMP			ev_room_choice_01_2
	_END

/*�ʐM�}���`*/
ev_room_choice_01_multi:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME		0
	_TALKMSG		msg_factory_room_10
	_JUMP			ev_room_choice_01_multi_retry
	_END

ev_room_choice_01_multi_retry:
	_LDVAL			FSW_LOCAL2,FACTORY_COMM_RETIRE
	_FACTORY_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER						/*0=�Â���*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_room_choice_01_multi_retry	/*���M���s�A�đ��M��*/

	_FACTORY_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*���肪�I�񂾃��j���[���擾*/
	_FACTORY_TOOL	FA_ID_GET_RETIRE_FLAG,0,0,FSW_ANSWER

	/*�p�[�g�i�[�����^�C�A������*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_giveup_yes_multi	/*���^�C�A*/

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_MENU
	_COMM_RESET

	_JUMP			ev_room_choice_01_2
	_END

/*�p�[�g�i�[�����^�C�A������*/
ev_factory_room_giveup_yes_multi:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME		0
	_TALKMSG_NOSKIP	msg_factory_room_12
	_TIME_WAIT		FRONTIER_COMMON_WAIT,FSW_ANSWER
	_JUMP			ev_factory_room_giveup_yes_multi_sync
	_END

ev_room_choice_01_2:
	/*���[�N���N���A���Ă���*/
	_LDVAL			FSW_LOCAL3,0

	_FACTORY_BTL_AFTER_PARTY_SET		/*�o�g�����POKEPARTY�̃Z�b�g*/

	/*�ΐ�g���[�i�[�̏����擾*/
	_CALL			ev_factory_room_tr_info_msg

	/*�������[�h*/
	_FACTORY_TOOL	FA_ID_SET_MODE,FACTORY_MODE_TRADE,0,FSW_ANSWER

	/*�u�|�P�����́@����������@���܂����H�v*/
	_TALKMSG		msg_factory_room_5

#ifdef DEBUG_FRONTIER_LOOP
	_JUMP			ev_factory_room_trade_no
#endif	//DEBUG_FRONTIER_LOOP

	_YES_NO_WIN		FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_trade_no
	_JUMP			ev_factory_room_trade_yes
	_END

/*�u�͂��v*/
ev_factory_room_trade_yes:

	/*�ʐM*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_trade_yes_multi

	_JUMP			ev_factory_room_trade_yes_pair
	_END

ev_factory_room_trade_yes_multi:
	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_factory_room_wait
	_JUMP			ev_factory_room_trade_yes_multi_retry
	_END

ev_factory_room_trade_yes_multi_retry:
	_LDVAL			FSW_LOCAL2,FACTORY_COMM_TRADE_YESNO
	_FACTORY_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER								/*0=����������*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_room_trade_yes_multi_retry	/*���M���s�A�đ��M��*/

	_FACTORY_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�����A�Е���������I��ł����������ʂ֐i��*/
	_JUMP			ev_factory_room_trade_yes_pair
	_END

ev_factory_room_trade_yes_pair:
	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_TALK_CLOSE

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_multi_room_trade

	_JUMP			ev_factory_room_trade_yes_2
	_END

ev_factory_multi_room_trade:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_TRADE
	_COMM_RESET		/*��ʐ؂�ւ��O�ɏ�����*/
	_JUMP			ev_factory_room_trade_yes_2
	_END

ev_factory_room_trade_yes_2:
	/*���[���폜*/
	_FACTORY_TOOL	FA_ID_DEL_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_factory_room_trade_sync

	/*�����Ăяo��*/
	_FACTORY_TRADE_CALL
	_FACTORY_TRADE_POKE_CHANGE								/*���Ō�����+1*/

	/*���[���ǉ�*/
	_FACTORY_TOOL	FA_ID_ADD_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*���݂̌�����+1*/
	//_FACTORY_TOOL	FA_ID_INC_TRADE_COUNT,0,0,FSW_ANSWER	//

	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_factory_room_go_trade_multi

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_JUMP			ev_factory_room_trade_after
	_END

/*�ʐM�}���`*/
ev_factory_room_trade_sync:
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_TRADE_CALL
	_COMM_RESET
	_RET

ev_factory_room_go_trade_multi:
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_TRADE_AFTER
	_COMM_RESET
	_RET

/*�u�������v*/
ev_factory_room_trade_no:
	/*�ʐM*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_trade_no_multi

	_JUMP			ev_factory_room_trade_no_2
	_END

ev_factory_room_trade_no_multi:
	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_factory_room_wait
	_JUMP			ev_factory_room_trade_no_multi_retry

ev_factory_room_trade_no_multi_retry:
	_LDVAL			FSW_LOCAL2,FACTORY_COMM_TRADE_YESNO
	_FACTORY_SEND_BUF	FSW_LOCAL2,1,FSW_ANSWER								/*1=�����������Ȃ�*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_room_trade_no_multi_retry	/*���M���s�A�đ��M��*/

	_FACTORY_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*���肪���������������擾*/
	_FACTORY_TOOL	FA_ID_GET_TRADE_YESNO_FLAG,0,0,FSW_ANSWER

	/*�p�[�g�i�[��������������I��ł�����*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_room_trade_yes_pair	/*������ʂ֐i��*/

	/*��l�Ƃ�������I�΂Ȃ�������*/
	_JUMP			ev_factory_room_trade_no_2
	_END

ev_factory_room_trade_no_2:
	_TALK_CLOSE
	_JUMP			ev_factory_room_trade_after
	_END

ev_factory_room_trade_after:
	_FACTORY_TRADE_AFTER_PARTY_SET			/*�g���[�h���POKEPARTY�̃Z�b�g*/
	_JUMP			ev_factory_room_go		/*�o�g����*/
	_END


/********************************************************************/
/*							�L�^����*/
/********************************************************************/
ev_room_choice_02:
	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,0,ev_factory_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_factory_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER										/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_rec_no
	_JUMP			ev_factory_room_rec_yes
	_END

/*�u�͂��v*/
ev_factory_room_rec_yes:
	_CALL			ev_factory_room_rec_win		/*�����̋L�^*/
	_JUMP			ev_factory_room_saikai		/*���j���[�֖߂�*/
	_END

/*�u�������v*/
ev_factory_room_rec_no:
	_JUMP			ev_factory_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*							���ʋL�^*/
/********************************************************************/
/*����*/
ev_factory_room_rec_win:
	_CALL				ev_factory_room_rec_common		/*LOCAL5�g�p��*/
	//_ADD_WK			FSW_LOCAL5,1
	//_SUB_WK			FSW_LOCAL5,1					/*���E���h���ł͂Ȃ��������Ȃ̂�*/
	_JUMP				ev_factory_room_rec
	_END

/*����*/
ev_factory_room_rec_lose:
	_CALL				ev_factory_room_rec_common		/*LOCAL5�g�p��*/
	_ADD_WK				FSW_LOCAL5,1
	_JUMP				ev_factory_room_rec
	_END

/*���ʕ���*/
ev_factory_room_rec_common:
	//�A�����擾
	_FACTORY_TOOL		FA_ID_GET_RENSYOU,0,0,FSW_LOCAL5
	_RET

ev_factory_room_rec:
	_JUMP				ev_factory_room_rec_sub
	_END

ev_factory_room_rec_sub:
	/*�L�^�������[�N�Z�b�g*/
	_LDVAL				FSW_LOCAL3,1

	_TALKMSG_ALL_PUT	msg_factory_room_6_4
	_ADD_WAITICON
	_SAVE_EVENT_WORK_GET	WK_SCENE_FACTORY_LEVEL,FSW_ANSWER	/*level*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,FACTORY_LEVEL_50,ev_factory_room_rec_lv50
	_JUMP				ev_factory_room_rec_lv100
	_END

/*�^��f�[�^�Z�[�u*/
ev_factory_room_rec_lv50:
	_FACTORY_TOOL		FA_ID_GET_TYPE,0,0,FSW_ANSWER
	_BATTLE_REC_SAVE	FRONTIER_NO_FACTORY_LV50,FSW_ANSWER,FSW_LOCAL5,FSW_LOCAL5
	_JUMP				ev_factory_room_rec_sub2
	_END

ev_factory_room_rec_lv100:
	_FACTORY_TOOL		FA_ID_GET_TYPE,0,0,FSW_ANSWER
	_BATTLE_REC_SAVE	FRONTIER_NO_FACTORY_LV100,FSW_ANSWER,FSW_LOCAL5,FSW_LOCAL5
	_JUMP				ev_factory_room_rec_sub2
	_END

ev_factory_room_rec_sub2:
	_DEL_WAITICON

	//_BATTLE_REC_LOAD

	_IFVAL_JUMP			FSW_LOCAL5,EQ,1,ev_factory_room_rec_true

	/*�u�L�^�o���܂���ł����v*/
	_TALKMSG			msg_factory_room_6_2
	_RET

ev_factory_room_rec_true:
	_SE_PLAY			SEQ_SE_DP_SAVE
	/*�u�L�^����܂����v*/
	_PLAYER_NAME		0
	_TALKMSG			msg_factory_room_6_1
	_RET
	
/*�^��f�[�^���Ȃ������b�Z�[�W*/
ev_factory_room_rec_msg1:
	_TALKMSG			msg_factory_room_6
	_RET

/*���łɘ^��f�[�^�����鎞���b�Z�[�W*/
ev_factory_room_rec_msg2:
	_TALKMSG			msg_factory_room_6_3
	_RET


/********************************************************************/
/*							�₷��*/
/********************************************************************/
ev_room_choice_03:
	/*�u���|�[�g�������ďI�����܂����H�v*/
	_TALKMSG		msg_factory_room_7
	_YES_NO_WIN		FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_rest_no
	_JUMP			ev_factory_room_rest_yes
	_END

/*�u�͂��v*/
ev_factory_room_rest_yes:
	//_LDVAL			WK_SCENE_FACTORY_LOBBY,2			/*��������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_FACTORY_LOBBY,2		/*��������n�܂�悤�ɂ��Ă���*/

	//�v���C�f�[�^�Z�[�u
	_FACTORY_TOOL	FA_ID_SAVE_REST_PLAY_DATA,0,0,FSW_ANSWER

	_CALL			ev_factory_save				/*07.08.24 _CALL�ɒu��������*/
	_TALK_CLOSE

	_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*���[���폜*/
	_FACTORY_TOOL	FA_ID_DEL_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_FACTORY_WORK_FREE

	//���Z�b�g�R�}���h
	_FACTORY_TOOL	FA_ID_SYSTEM_RESET,0,0,FSW_ANSWER

	_END

/*�u�������v*/
ev_factory_room_rest_no:
	_JUMP			ev_factory_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*							���^�C�A*/
/********************************************************************/
ev_room_choice_04:
	/*�u�o�g���g���[�h�̒���𒆎~����H�v*/
	_TALKMSG		msg_factory_room_8
	_YES_NO_WIN_EX	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_giveup_no

	/*�ʐM*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_giveup_yes_multi_wait_msg

	/*�u�����^���|�P�����Ɓ@���a���肵�Ă����|�P�������@�������܂��傤�I�v*/
	_TALKMSG		msg_factory_lobby_23

	_JUMP			ev_factory_room_giveup_yes
	_END

/*�u���X���҂����������v*/
ev_factory_room_giveup_yes_multi_wait_msg:
	_TALKMSG_NOSKIP	msg_factory_room_wait
	_JUMP			ev_factory_room_giveup_yes_multi_retry
	_END

ev_factory_room_giveup_yes_multi_retry:
	_LDVAL			FSW_LOCAL2,FACTORY_COMM_RETIRE
	_FACTORY_SEND_BUF	FSW_LOCAL2,1,FSW_ANSWER								/*1=���^�C��*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_room_giveup_yes_multi_retry	/*���M���s�A�đ��M��*/

	//_FACTORY_RECV_BUF	FSW_LOCAL2
	//_COMM_RESET
	_JUMP			ev_factory_room_giveup_yes_multi_sync
	_END

ev_factory_room_giveup_yes_multi_sync:
	/*�u�����^���|�P�����Ɓ@���a���肵�Ă����|�P�������@�������܂��傤�I�v*/
	_TALKMSG		msg_factory_lobby_23

	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_factory_room_wait

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_GIVE
	//_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_MENU
	_COMM_RESET
	_JUMP			ev_factory_room_giveup_yes
	_END

/*�u�͂��v*/
ev_factory_room_giveup_yes:
	//�s��p�����[�^�Z�b�g
	_FACTORY_TOOL	FA_ID_SET_LOSE,0,0,FSW_ANSWER

	//_LDVAL			WK_SCENE_FACTORY_LOBBY,3			/*���^�C������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_FACTORY_LOBBY,3		/*���^�C������n�܂�悤�ɂ��Ă���*/

	_JUMP				ev_factory_room_end_save
	_END

/*�u�������v*/
ev_factory_room_giveup_no:
	_JUMP			ev_factory_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*								�s�k*/
/********************************************************************/
ev_factory_lose:
	_CALL			ev_factory_enemy_tr_go_out				/*�G�g���[�i�[�ޏ�A�j��*/
	_CALL			ev_factory_room_return					/*���������ɖ߂�*/

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_factory_lose_timing_sub

	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,0,ev_factory_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_factory_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER								/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_lose_rec_no	/*�u�������v*/
	_JUMP			ev_factory_lose_rec_yes					/*�u�͂��v*/
	_END

ev_factory_lose_timing_sub:
	_TALKMSG_NOSKIP	msg_factory_room_wait
	_JUMP			ev_factory_lose_timing_call
	_END

/*�u�͂��v*/
ev_factory_lose_rec_yes:
	_CALL			ev_factory_room_rec_lose				/*�s�k�̋L�^*/
	_JUMP			ev_factory_lose_timing_call
	_END

/*�u�������v*/
ev_factory_lose_rec_no:
	_JUMP			ev_factory_lose_timing_call
	_END

ev_factory_lose_timing_call:
	//�s��p�����[�^�Z�b�g
	_FACTORY_TOOL	FA_ID_SET_LOSE,0,0,FSW_ANSWER

	//_LDVAL			WK_SCENE_FACTORY_LOBBY,3			/*���^�C������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_FACTORY_LOBBY,3		/*���^�C������n�܂�悤�ɂ��Ă���*/

	_JUMP			ev_factory_lose_timing
	_END

ev_factory_lose_timing:
	/*�u�����^���|�P�����Ɓ@���a���肵�Ă����|�P�������@�������܂��傤�I�v*/
	_TALKMSG		msg_factory_lobby_23

	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_multi_lose_timing
	_JUMP			ev_factory_room_end_save
	_END

/*�ʐM����*/
ev_factory_multi_lose_timing:
	/*�u���X���҂����������v*/
	_TALKMSG_NOSKIP	msg_factory_room_wait

	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_LOSE_END
	_COMM_RESET
	_JUMP				ev_factory_room_end_save
	_END

ev_factory_room_end_save:
	_CALL				ev_factory_save				/*07.08.24 _CALL�ɒu��������*/
	_TALK_CLOSE
	_JUMP				ev_factory_room_end
	_END


/********************************************************************/
/*							���ʏI��								*/
/********************************************************************/
ev_factory_room_end:

	/*�Z�[�u��ɓ���������*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_factory_multi_end_timing

	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_FACTORY_TOOL	FA_ID_GET_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,FACTORY_TYPE_MULTI,ev_factory_room_multi_tv

	_FACTORY_TOOL	FA_ID_GET_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,FACTORY_TYPE_WIFI_MULTI,ev_factory_room_end_wifi

	/*���[���폜*/
	_FACTORY_TOOL	FA_ID_DEL_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_FACTORY_WORK_FREE

	/*2D�}�b�v�T������*/
	//

	//_END
	_SCRIPT_FINISH			/*_END����2D�}�b�v�I��*/

/*�}���`�̂�*/
ev_factory_room_multi_tv:
	/*TV����*/
	_TV_TEMP_FRIEND_SET	FRONTIER_NO_FACTORY_LV50
	//_TV_TEMP_FRIEND_SET	FRONTIER_NO_FACTORY_LV100
	_RET


/********************************************************************/
/*						�Z�[�u��ɓ���������						*/	
/********************************************************************/
ev_factory_multi_end_timing:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_SAVE_AFTER
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/
	_RET


/********************************************************************/
/*						WIFI�͑҂����킹��ʂ֖߂�					*/
/********************************************************************/
ev_factory_room_end_wifi:
	/*�t�F�[�h�A�E�g*/
	//_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK

	/*���[���폜*/
	_FACTORY_TOOL	FA_ID_DEL_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_FACTORY_WORK_FREE

	_MAP_CHANGE_EX		FSS_SCENEID_WIFI_COUNTER,1
	_END


/********************************************************************/
//							���ʃZ�[�u	
/********************************************************************/
ev_factory_save:
	/*�u���|�[�g�ɏ�������ł��܂��@�d����؂�Ȃ��ŉ������v*/
	_TALKMSG_ALL_PUT	msg_factory_lobby_24
	_ADD_WAITICON
	_REPORT_DIV_SAVE	FSW_ANSWER
	_DEL_WAITICON
	_SE_PLAY		SEQ_SE_DP_SAVE
	_SE_WAIT		SEQ_SE_DP_SAVE
	_RET


/********************************************************************/
/*						���ʁF���������ɖ߂�(CALL)					*/	
/********************************************************************/
ev_factory_room_return:
	/*���������ɖ߂�*/
	_SE_PLAY		SEQ_SE_DP_KAIDAN2
	_SE_WAIT		SEQ_SE_DP_KAIDAN2
	_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_return_multi

	_ACTOR_FREE		OBJID_PLAYER
	_ACTOR_FREE		OBJID_MINE
	//_ACTOR_FREE		OBJID_ETC
	//_CHAR_RESOURCE_FREE	FSW_PARAM2
	//_CHAR_RESOURCE_FREE	FSW_LOCAL7

	_MAP_CHANGE		FSS_SCENEID_FACTORY

	/*���[���ǉ�*/
	_FACTORY_TOOL	FA_ID_ADD_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	_CHAR_RESOURCE_SET	default_set_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			btl_after_set_actor			/*�A�N�^�[�o�^*/

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_RET

ev_factory_room_return_multi:
	_ACTOR_FREE		OBJID_SIO_USER_0
	_ACTOR_FREE		OBJID_SIO_USER_1
	_ACTOR_FREE		OBJID_MINE
	//_ACTOR_FREE		OBJID_ETC
	//_CHAR_RESOURCE_FREE	FSW_PARAM2

	_MAP_CHANGE		FSS_SCENEID_FACTORY

	/*���[���ǉ�*/
	_FACTORY_TOOL	FA_ID_ADD_MAP_RAIL_TCB,0,0,FSW_ANSWER

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	_CHAR_RESOURCE_SET	default_set_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			btl_after_set_actor_multi	/*�A�N�^�[�o�^*/

	/*�t�F�[�h�C��*/
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_RET


/********************************************************************/
/*						���ʁF�G�g���[�i�[�ޏ�(CALL)				*/	
/********************************************************************/
ev_factory_enemy_tr_go_out:
	/*�ʐM�}���`*/
	_FACTORY_TOOL	FA_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_enemy_tr_go_out_multi

	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME		OBJID_ETC,etc_anime_go_out
	_OBJ_ANIME_WAIT
	_ACTOR_FREE		OBJID_ETC
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_RET

ev_factory_enemy_tr_go_out_multi:
	_CALL			ev_factory_comm_command_initialize_3

	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME		OBJID_ETC,etc_anime_go_out_sio
	_OBJ_ANIME		OBJID_ETC2,etc_anime_go_out_multi
	_OBJ_ANIME_WAIT
	_ACTOR_FREE		OBJID_ETC
	_ACTOR_FREE		OBJID_ETC2
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_CHAR_RESOURCE_FREE	FSW_PARAM3
	_RET


/********************************************************************/
/*					�ΐ핔���̒n�ʃp���b�g�؂�ւ�					*/
/********************************************************************/
ev_btl_room_pal_chg:
	_FACTORY_TOOL	FA_ID_BTL_ROOM_PAL_CHG,0,0,FSW_ANSWER
	_TIME_WAIT		3,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_BTL_ROOM_PAL_CHG,1,0,FSW_ANSWER
	_TIME_WAIT		3,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_BTL_ROOM_PAL_CHG,2,0,FSW_ANSWER
	_TIME_WAIT		3,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_BTL_ROOM_PAL_CHG,3,0,FSW_ANSWER
	_TIME_WAIT		3,FSW_ANSWER
	_FACTORY_TOOL	FA_ID_BTL_ROOM_PAL_CHG,4,0,FSW_ANSWER
	_RET


/********************************************************************/
/*						�u���[���̗���								*/
/********************************************************************/
ev_factory_leader_1st_01:
ev_factory_leader_2nd_01:

	/*�L�^�������[�N�Ƀf�[�^���������܂�Ă��Ȃ� = ���f���A�Ƃ��čl����*/
	/*�o�ꂵ�܂��惁�b�Z�[�W�͕\�����Ȃ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,1,ev_factory_room_saikai_00

	/*��x�u���[���o�ꃁ�b�Z�[�W��\��������*/
	_FACTORY_TOOL	FA_ID_BRAIN_APPEAR_MSG_CHK,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_factory_room_saikai_00

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_factory_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*�u�u���[���Ƃ̏����ɂȂ�܂��I�o��͂����ł����H�v*/
	_TALKMSG			msg_factory_room_boss_01
	_JUMP				ev_factory_room_saikai_02
	_END

ev_factory_leader_1st_02:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_factory_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*�o��*/
	_CALL				ev_factory_leader_appear

	/*�퓬�O�̃��b�Z�[�W(1����)*/
	_CALL				ev_factory_leader_common
	_TALKMSG			msg_factory_room_boss_02
	_TALK_CLOSE

	_JUMP				ev_factory_room_go_2_sub
	_END

ev_factory_leader_2nd_02:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_factory_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*�o��*/
	_CALL				ev_factory_leader_appear

	/*�퓬�O�̃��b�Z�[�W(2����)*/
	_CALL				ev_factory_leader_common
	_TALKMSG			msg_factory_room_boss_03
	_TALK_CLOSE

	_JUMP				ev_factory_room_go_2_sub
	_END

/*���ʏ���*/
ev_factory_leader_common:
	/*��l���̃|�P��������Buf0,1,2�ɃZ�b�g*/
	_FACTORY_TOOL		FA_ID_BRAIN_PLAYER_POKE_NAME,0,0,FSW_ANSWER

	_GET_RND			FSW_ANSWER,90				/*0 - 89�̊ԂŃ����_��*/
	_ADD_WK				FSW_ANSWER,10				/*10 - 99�̊Ԃɂ���*/
	_NUMBER_NAME		3,FSW_ANSWER
	_RET

/*�o��*/
ev_factory_leader_appear:
	/*�����ɂ��ău���[���������܂ŕ���*/
	_CALL				ev_factory_trainer_set_brain
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain
	_OBJ_ANIME_WAIT

	/*�L�����L����*/
	_OBJ_ANIME		OBJID_MINE,anm_player_kyorokyoro
	_OBJ_ANIME_WAIT

	/*�n�k�Z�b�g*/
	//_SHAKE_SET		0,2,0,6
	//_SHAKE_WAIT

	/*�p�[�e�B�N�����o*/
	_PARTICLE_SPA_LOAD	SPAWORK_0,FACTORY_SPA, FSS_CAMERA_ORTHO

	_PARTICLE_ADD_EMITTER	SPAWORK_0, FACTORY_BF_WARP01
	_PARTICLE_ADD_EMITTER	SPAWORK_0, FACTORY_BF_WARP02
	_PARTICLE_ADD_EMITTER	SPAWORK_0, FACTORY_BF_WARP03
	_SE_PLAY			SEQ_SE_PL_FAC01

	/*�n�k�Z�b�g*/
	_TIME_WAIT			36,FSW_ANSWER
	//_SE_PLAY			SE_SHAKE
	_SHAKE_SET			0,3,2,10
	//_SHAKE_WAIT				/*�Ȃ��Ă��悢���A�I�����Ă���}�b�v�J�ڂȂǂ���悤�ɂ���*/

	/*�p�[�e�B�N���������Ƀu���[����\��*/
	_TIME_WAIT			11,FSW_ANSWER
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain_02
	_OBJ_ANIME_WAIT

	//_SE_STOP			SE_SHAKE
	_PARTICLE_WAIT

	_PARTICLE_SPA_EXIT	SPAWORK_0
	_RET


/********************************************************************/
/*						�u���[���ɏ�����̗���						*/
/********************************************************************/
ev_factory_leader_1st_03:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_factory_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_FACTORY,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_NONE,ev_factory_meory_print_put_1st

	/*�퓬��̃��b�Z�[�W(1����)*/
	_TALKMSG			msg_factory_room_boss_04
	_TALK_CLOSE
	_JUMP				ev_factory_battle_2_sub
	_END

/*��t��1st�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_factory_meory_print_put_1st:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_FACTORY,MEMORY_PRINT_PUT_1ST
	_RET

ev_factory_leader_2nd_03:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL			ev_factory_rensyou_get
	_NUMBER_NAME	0,FSW_ANSWER

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_FACTORY,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_PUT_OK_1ST,ev_factory_meory_print_put_2nd

	/*�퓬��̃��b�Z�[�W(2����)*/
	_TALKMSG			msg_factory_room_boss_05
	_TALK_CLOSE
	_JUMP				ev_factory_battle_2_sub
	_END

/*��t��2nd�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_factory_meory_print_put_2nd:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_FACTORY,MEMORY_PRINT_PUT_2ND
	_RET


/********************************************************************/
/*						�A�������擾(FSW_ANSWER�g�p)				*/
/********************************************************************/
ev_factory_rensyou_get:
	/*�A�����擾*/
	_FACTORY_TOOL	FA_ID_GET_RENSYOU,0,0,FSW_ANSWER

	/*���ł�9999�̎��́A0 �I���W���̕␳�������Ȃ�*/
	_IFVAL_JUMP		FSW_ANSWER,GE,FACTORY_RENSYOU_MAX,ev_factory_rensyou_ret

	_ADD_WK			FSW_ANSWER,1
	_RET

ev_factory_rensyou_ret:
	_RET


/********************************************************************/
/*																	*/
/********************************************************************/
ev_factory_comm_command_initialize_1:
	_TIME_WAIT			1,FSW_ANSWER
	_FACTORY_TOOL		FA_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_INIT_1
	_COMM_RESET
	_RET

ev_factory_comm_command_initialize_2:
	_TIME_WAIT			1,FSW_ANSWER
	_FACTORY_TOOL		FA_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_INIT_2
	_COMM_RESET
	_RET

ev_factory_comm_command_initialize_3:
	_TIME_WAIT			1,FSW_ANSWER
	_FACTORY_TOOL		FA_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FACTORY_INIT_3
	_COMM_RESET
	_RET



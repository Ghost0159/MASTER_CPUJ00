//==============================================================================
/**
 * @file	castle.s
 * @brief	�u�L���b�X���T�����v�t�����e�B�A�X�N���v�g
 * @author	nohara
 * @date	2007.07.04
 */
//==============================================================================

	.text

	.include	"../frontier_def.h"
	.include	"../castle_def.h"
	.include	"../frontier_seq_def.h"
	.include	"../../../include/msgdata/msg_castle_room.h"
	.include	"../../../include/field/evwkdef.h"
	.include	"../../fielddata/script/saveflag.h"
	.include	"../../fielddata/script/savework.h"
	.include	"../../field/syswork_def.h"
	.include	"../../particledata/pl_frontier/frontier_particle_def.h"	//SPA
	.include	"../../particledata/pl_frontier/frontier_particle_lst.h"	//EMIT

//--------------------------------------------------------------------
//					     �X�N���v�g�{��
//
//	FSW_PARAM0	�L�^�v�Z�p
//	FSW_PARAM1	���X�g�p
//	FSW_PARAM2	�G�g���[�i�[1��OBJ�R�[�h
//	FSW_PARAM3	�G�g���[�i�[2��OBJ�R�[�h
//	FSW_PARAM4	�擾����CP�̒l
//	FSW_PARAM5	���񂩔���
//	FSW_PARAM6	�U������OBJ�R�[�h
//
//	FSW_LOCAL1	�^�C�v
//	FSW_LOCAL2	�ʐM��`
//	FSW_LOCAL3	�L�^������
//	FSW_LOCAL4	�ۑ��Ȃ�
//	FSW_LOCAL5	�ΐ킩�p�X������
//	FSW_LOCAL6	�ۑ��Ȃ�
//	FSW_LOCAL7	�����̌�����
//
//--------------------------------------------------------------------
_EVENT_DATA		fss_castle_start	//��ԏ��EVENT_DATA�͎������s
_EVENT_DATA_END						//�I��

//--------------------------------------------------------------------
//					     �f�o�b�N��`
//--------------------------------------------------------------------
//#define DEBUG_BTL_OFF				//�o�g���I�t�f�o�b�N
//#define DEBUG_BTL_LOSE_OFF			//�o�g���s�k�I�t�f�o�b�N
//#define DEBUG_7BTL_OFF				//�o�g��7��I�t�f�o�b�N

//--------------------------------------------------------------------
//							��`
//--------------------------------------------------------------------
#define CASTLE_PASS_LAP_NUM		(3)	//�ʂ��݂����o�ꂷ�����


/********************************************************************/
//
//		SCENE_CASTLE:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
_RESOURCE_LABEL	default_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		BRAINS3,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	default_set_resource_multi
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		BRAINS3,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	pcwoman2_set_resource
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_CASTLE:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
#define OBJID_PLAYER				(0)
#define OBJID_SIO_USER_0			(1)
#define OBJID_SIO_USER_1			(2)
#define OBJID_BRAIN3_INFO			(3)
#define OBJID_MINE					(4)
#define OBJID_PRINCESS				(5)
#define OBJID_BRAIN3_BTL_ROOM		(6)			/*CP��n�����o�p*/
#define OBJID_PCWOMAN2				(7)
#define OBJID_ETC					(98)
#define OBJID_ETC2					(99)

//�X�N���v�gID(����Ή��\��)
#define EVENTID_TEST_SCR_OBJ1		(1)

_ACTOR_LABEL	default_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_LEFT,8*34,8*14,OFF/*�������g�̃A�N�^�Z�b�g*/
	_ACTOR_DATA				OBJID_BRAIN3_INFO,BRAINS3,WF2DMAP_WAY_C_RIGHT, \
							8*26,8*14,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_LEFT, \
							8*34,8*14,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	default_set_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*34,8*14,OFF	/*�������g�̃A�N�^�Z�b�g*/
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_LEFT,8*34,8*14,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_LEFT,8*34,8*16,ON
	_ACTOR_DATA				OBJID_BRAIN3_INFO,BRAINS3,WF2DMAP_WAY_C_RIGHT, \
							8*26,8*14,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	pcwoman2_set_actor
	_ACTOR_DATA				OBJID_PCWOMAN2,FSW_PARAM6,WF2DMAP_WAY_C_DOWN, \
							8*34,8*12,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

//�퓬��ɖ߂��Ă�����
_ACTOR_LABEL	btl_after_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_LEFT,8*28,8*14,OFF/*�������g�̃A�N�^�Z�b�g*/
	_ACTOR_DATA				OBJID_BRAIN3_INFO,BRAINS3,WF2DMAP_WAY_C_RIGHT, \
							8*26,8*14,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_LEFT, \
							8*28,8*14,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	btl_after_set_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_LEFT,8*28,8*14,OFF/*�������g�̃A�N�^�Z�b�g*/
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_LEFT,8*28,8*14,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_LEFT,8*28,8*16,ON
	_ACTOR_DATA				OBJID_BRAIN3_INFO,BRAINS3,WF2DMAP_WAY_C_RIGHT, \
							8*26,8*14,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

/********************************************************************/
//
//	SCENE_CASTLE_BTL:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
_RESOURCE_LABEL	default_scene2_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_LOCAL7, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		PRINCESS,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	default_scene2_resource_multi
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_LOCAL7, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		PRINCESS,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	brain_set_resource
	_CHAR_RESOURCE_DATA		BRAINS3,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_CASTLE_BTL:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
#define PLAYER_Y					(8*28)

_ACTOR_LABEL	default_scene2_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*18,OFF	//�������g�̃A�N�^�[�Z�b�g
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*16,PLAYER_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PRINCESS,PRINCESS,WF2DMAP_WAY_C_DOWN, \
							8*16,8*4,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	default_scene2_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*18,OFF	//�������g�̃A�N�^�[�Z�b�g
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_UP,8*16,PLAYER_Y,ON	/*�擪*/
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_UP,8*16,PLAYER_Y,ON
	_ACTOR_DATA				OBJID_PRINCESS,PRINCESS,WF2DMAP_WAY_C_DOWN, \
							8*16,8*4,ON,EVENTID_TEST_SCR_OBJ1
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
#define ETC_Y		(8*12)		//(8*13+4)

_ACTOR_LABEL	etc_set_actor
	_ACTOR_DATA			OBJID_ETC,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							8*16,ETC_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	etc_set_actor2
	_ACTOR_DATA			OBJID_ETC,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							8*16,ETC_Y,ON,EVENTID_TEST_SCR_OBJ1				//�擪
	_ACTOR_DATA			OBJID_ETC2,FSW_PARAM3,WF2DMAP_WAY_C_DOWN, \
							8*16,ETC_Y,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

//--------------------------------------------------------------------
//	�ォ��ǉ�����A�N�^�[
// LOCAL0 = OBJCODE
//--------------------------------------------------------------------
#define BRAIN_Y		(8*26)		//(8*24-4)

_ACTOR_LABEL	brain_set_actor
	_ACTOR_DATA			OBJID_BRAIN3_BTL_ROOM,BRAINS3,WF2DMAP_WAY_C_DOWN, \
							8*16,BRAIN_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END


/********************************************************************/
//
//	SCENE_CASTLE_BYPATH:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
_RESOURCE_LABEL	default_scene3_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_LOCAL7, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	default_scene3_resource_multi
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_LOCAL7, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_CASTLE_BYPATH:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
_ACTOR_LABEL	default_scene3_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*14,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*6,8*24,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	default_scene3_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*14,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_UP,8*6,8*24,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_UP,8*8,8*24,ON
	_ACTOR_DATA_END


/********************************************************************/
//
//					�A�j���[�V�����f�[�^
//
/********************************************************************/

//--------------------------------------------------------------------
//�����O�܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_center
	_ANIME_DATA	FC_WALK_L_8F,3
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center_multi
	_ANIME_DATA	FC_WALK_L_8F,3
	_ANIME_DATA_END

//--------------------------------------------------------------------
//������
//--------------------------------------------------------------------
_ANIME_LABEL anm_pcwoman2_go_center
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
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
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�������ֈړ�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_nukemiti_room
	_ANIME_DATA	FC_WAIT_8F,2
	_ANIME_DATA	FC_WALK_L_8F,7
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_nukemiti_room_multi
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WALK_L_8F,8
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

_ANIME_LABEL anm_man_go_nukemiti_room
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
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
_ANIME_LABEL anm_scroll_go_btl_center
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_btl_center
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_btl_center_sio
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_btl_center_multi
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�������𔲂���
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_nukemiti_center
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_WALK_R_8F,11
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_nukemiti_center_multi
	_ANIME_DATA	FC_WALK_U_8F,5
	_ANIME_DATA	FC_WALK_R_8F,11
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�g���[�i�[���o�g�����[���̒����܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL etc_anime_room_in
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA	FC_WALK_D_8F,2
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_sio
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA	FC_WALK_D_8F,2
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_multi
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

/*�u���[����\���Œ�ʒu��*/
_ANIME_LABEL etc_anime_room_in_brain
	//_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA	FC_WALK_D_8F,2
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

/*�u���[���\��*/
_ANIME_LABEL etc_anime_room_in_brain_02
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�g���[�i�[���o�g�����[������o�čs��
//--------------------------------------------------------------------
_ANIME_LABEL etc_anime_go_out
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_go_out_sio
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_go_out_multi
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_go_out_brain
	_ANIME_DATA	FC_WALK_D_8F,2
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA	FC_WALK_D_8F,5
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�u���[�����o�g�����[���̒����܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL brain_anime_room_in
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�u���[�����o�g�����[������o�čs��
//--------------------------------------------------------------------
_ANIME_LABEL brain_anime_go_out
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA	FC_WALK_D_8F,6
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�v�����Z�X	�b��
//--------------------------------------------------------------------
_ANIME_LABEL princess_anime_talk
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END


/********************************************************************/
//
//						�����^���`�ΐ�`����
//
/********************************************************************/
fss_castle_start:
	//_EVENT_START

	/*lobby�őI�������^�C�v�ƃ��x�����K�v*/
	_SAVE_EVENT_WORK_GET	WK_SCENE_CASTLE_TYPE,FSW_LOCAL1	/*type*/
	_SAVE_EVENT_WORK_GET	LOCALWORK3,FSW_LOCAL3				/*init*/
	_SAVE_EVENT_WORK_GET	LOCALWORK2,FSW_LOCAL4				/*pos1*/
	_SAVE_EVENT_WORK_GET	LOCALWORK5,FSW_LOCAL5				/*pos2*/
	_SAVE_EVENT_WORK_GET	LOCALWORK6,FSW_LOCAL6				/*pos3*/

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	/*����t���O*/
	_LDVAL				FSW_PARAM5,0

	/*�����ł͂܂����[�N���m�ۂ���Ă��Ȃ�*/
	//_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	//_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_start_multi
	_IFVAL_JUMP			FSW_LOCAL1,EQ,CASTLE_TYPE_MULTI,ev_castle_start_multi
	_IFVAL_JUMP			FSW_LOCAL1,EQ,CASTLE_TYPE_WIFI_MULTI,ev_castle_start_multi

	_LDVAL				FSW_PARAM6,BFSM
	_CHAR_RESOURCE_SET	default_set_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_set_actor			/*�A�N�^�[�o�^*/
	_JUMP				ev_castle_start_data_set
	_END

ev_castle_start_multi:

	/*���s�Ɋ֌W�Ȃ��ʐM�t�����e�B�A�ɒ���(�ʐM�͒��f���Ȃ��̂Ő擪�ŏ������Ă����v)*/
	_SCORE_ADD			SCORE_ID_FRONTIER_COMM

	_LDVAL				FSW_PARAM6,BFSW1
	_CHAR_RESOURCE_SET	default_set_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_set_actor_multi		/*�A�N�^�[�o�^*/
	_JUMP				ev_castle_start_data_set
	_END


/********************************************************************/
//
/********************************************************************/
ev_castle_start_data_set:
	_CHAR_RESOURCE_SET	pcwoman2_set_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			pcwoman2_set_actor			/*�A�N�^�[�o�^*/

	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*���[�N�m��*/
	_BATTLE_REC_INIT
	_CASTLE_WORK_ALLOC	FSW_LOCAL3,FSW_LOCAL1,FSW_LOCAL4,FSW_LOCAL5,FSW_LOCAL6,FSW_ANSWER

	/*�ʐM�F��{�����Ƃ�*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_comm_basic

	_JUMP				ev_castle_start_2
	_END

/*�ʐM�F��{�����Ƃ�*/
ev_castle_multi_comm_basic:
	_CALL				ev_castle_comm_command_initialize_1

//	_JUMP				ev_castle_multi_comm_basic_retry
//	_END
	_CALL				ev_castle_multi_comm_basic_call

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_BASIC_AFTER
	_COMM_RESET

	_JUMP				ev_castle_start_2
	_END

ev_castle_multi_comm_basic_retry:
	_LDVAL				FSW_LOCAL2,CASTLE_COMM_BASIC
	_CASTLE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_multi_comm_basic_retry	/*���M���s�́A�đ��M��*/

	_CASTLE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET
	_JUMP				ev_castle_start_2
	_END


/********************************************************************/
//
/********************************************************************/
/*�ʐM�F��{�����Ƃ�*/
ev_castle_multi_comm_basic_call:
	_JUMP				ev_castle_multi_comm_basic_call_retry
	_END

ev_castle_multi_comm_basic_call_retry:
	_LDVAL				FSW_LOCAL2,CASTLE_COMM_BASIC
	_CASTLE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_multi_comm_basic_call_retry/*���M���s�͍đ��M��*/

	_CASTLE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET
	_RET


/********************************************************************/
//
/********************************************************************/
ev_castle_start_2:
	/*���[�N������*/
	_CASTLE_WORK_INIT	FSW_LOCAL3

	/*�ʐM�F�g���[�i�[�����Ƃ�*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_comm_tr_2

	_JUMP				ev_castle_start_3
	_END

ev_castle_multi_comm_tr_2:

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_TR_BEFORE
	_COMM_RESET

	_JUMP				ev_castle_multi_comm_tr
	_END

/*�ʐM�F�g���[�i�[�����Ƃ�*/
ev_castle_multi_comm_tr:
	_LDVAL				FSW_LOCAL2,CASTLE_COMM_TR
	_CASTLE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_multi_comm_tr	/*���M���s�́A�đ��M��*/

	_CASTLE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_POKE_DATA
	_COMM_RESET

	_COMM_SET_WIFI_BOTH_NET	1										/*��ʃf�[�^ON*/
	_JUMP				ev_castle_multi_comm_poke
	_END

ev_castle_multi_comm_poke:
	/*�ʐM�F�莝�����Ƃ�*/
	_LDVAL				FSW_LOCAL2,CASTLE_COMM_TEMOTI
	_CASTLE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_multi_comm_poke	/*���M���s�́A�đ��M��*/

	_CASTLE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_BOTH_0
	_COMM_RESET

	_COMM_SET_WIFI_BOTH_NET	0										/*��ʃf�[�^OFF*/
	_JUMP				ev_castle_start_3
	_END


/********************************************************************/
//
/********************************************************************/
ev_castle_start_3:
	/*�u���낭����v�Ŏg�p���郏�[�N�Ȃ̂ŃN���A���Ă���*/
	_LDVAL				FSW_LOCAL3,0

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_start_3_multi

	/*�����܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center
	_OBJ_ANIME			OBJID_MINE,anm_player_go_center
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_pcwoman2_go_center
	_OBJ_ANIME_WAIT

	_JUMP				ev_castle_room_start
	_END

ev_castle_start_3_multi:
	/*�����܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center
	_OBJ_ANIME			OBJID_SIO_USER_0,anm_player_go_center
	_OBJ_ANIME			OBJID_SIO_USER_1,anm_player_go_center_multi
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_pcwoman2_go_center
	_OBJ_ANIME_WAIT
	_JUMP				ev_castle_room_start
	_END


/********************************************************************/
/*																	*/
/********************************************************************/
ev_castle_room_start:
	/*�����l�ڂ��擾*/
	_CASTLE_TOOL		FC_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_room_renshou_0		/*0	*/

	/*�L�^�������[�N�Ƀf�[�^���������܂�Ă��Ȃ��̂Ń��j���[�\�����Ȃ�*/
	_LDVAL				FSW_LOCAL3,1
	_JUMP				ev_castle_room_saikai							/*1-7*/
	_END


/********************************************************************/
/*							0�l��									*/
/********************************************************************/
ev_castle_room_renshou_0:
	/*�ʐM�}���`*/
	_CASTLE_TOOL	FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_castle_multi_room_enemy_poke_send2_2

	/*(�V���O���A�}���`�e�p)�G��POKEPARTY�Z�b�g*/
	_CASTLE_BTL_BEFORE_PARTY_SET

	_JUMP			ev_castle_room_go_rental
	_END


/********************************************************************/
/*					�G�|�P�����f�[�^��ʐM							*/
/********************************************************************/
ev_castle_multi_room_enemy_poke_send2_2:

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_ENEMY_BEFORE
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/

	_JUMP			ev_castle_multi_room_enemy_poke_send2
	_END

ev_castle_multi_room_enemy_poke_send2:
	/*�ʐM�F�G�|�P�������Ƃ�*/
	_LDVAL				FSW_LOCAL2,CASTLE_COMM_ENEMY
	_CASTLE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_multi_room_enemy_poke_send2/*���M���s�͍đ��M��*/

	_CASTLE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_ENEMY_AFTER
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/

	/*�G�|�P��������*/
	_CASTLE_TOOL		FC_ID_ENEMY_POKE_CREATE,0,0,FSW_ANSWER
	_CASTLE_TOOL		FC_ID_ENEMY_POKE_CREATE_2,0,0,FSW_ANSWER
	_RET


/********************************************************************/
/*																	*/
/********************************************************************/
ev_castle_room_go_rental:

	_CASTLE_TOOL		FC_ID_GET_GUIDE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_room_go_first
	_JUMP				ev_castle_room_go_second
	_END

/*�u���q�l�̃|�P�����̎����Ă��铹���a����܂��v*/
ev_castle_room_go_first:
	_TALKMSG			msg_castle_room_1			/*����*/
	_JUMP				ev_castle_room_go_sub
	_END

ev_castle_room_go_second:
	_PLAYER_NAME		0
	_TALKMSG			msg_castle_room_1_01		/*2��ڈȍ~*/
	_JUMP				ev_castle_room_go_sub
	_END

ev_castle_room_go_sub:
	//_SE_PLAY			SEQ_SE_DP_SUTYA
	//_SE_PLAY			SEQ_SE_PL_BAG_030

	/*������CP�����ƁA�ʐM���ɂ��ꂪ�łĂ��܂��̂Ń_��*/
	/*���̍ŏ���CP+10*/
	//_CASTLE_TOOL		FC_ID_GET_TYPE,0,0,FSW_ANSWER
	//_CASTLE_ADD_CP		FSW_ANSWER,10

	/*3���ڂ��擾*/
	_CASTLE_TOOL		FC_ID_GET_LAP,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,CASTLE_PASS_LAP_NUM,ev_castle_room_lap_3

	/*�u����ł͉������邩�I��ł��������v*/
	_TALKMSG			msg_castle_room_20

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_room_go_rental

	_JUMP				ev_castle_room_go_rental_2
	_END

/*�ʂ��݂��I���o����悤�ɂȂ����惁�b�Z�[�W*/
ev_castle_room_lap_3:
	_TALKMSG			msg_castle_room_26
	_RET

ev_castle_multi_room_go_rental:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_castle_room_wait
	_JUMP				ev_castle_multi_room_go_rental_retry
	_END

ev_castle_multi_room_go_rental_retry:
#if 0
	_LDVAL				FSW_LOCAL2,CASTLE_COMM_RENTAL
	_CASTLE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_multi_room_go_rental_retry/*���M���s�͍đ��M��*/

	_CASTLE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET
#endif

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_RENTAL
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/
	_JUMP				ev_castle_room_go_rental_2
	_END


/********************************************************************/
/*																	*/
/********************************************************************/
ev_castle_room_go_rental_2:

	/*�u����ł͉������邩�I��ł��������v*/
	//_TALKMSG			msg_castle_room_20
	_TALKMSG_ALL_PUT	msg_castle_room_21				/*�Ă����̐���(��b�E�B���h�E��\������)*/

	/*CP�E�B���h�E�\��*/
	_CP_WIN_WRITE
	_JUMP				ev_castle_room_go_rental_2_sub
	_END

ev_castle_room_go_rental_2_sub:
	/*���j���[�쐬*/
	/*3���ڈȏォ�擾*/
	_CASTLE_TOOL		FC_ID_GET_LAP,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,LT,CASTLE_PASS_LAP_NUM,ev_castle_room_lap_3_make_list_lt
	_IFVAL_CALL			FSW_LOCAL6,GE,CASTLE_PASS_LAP_NUM,ev_castle_room_lap_3_make_list_ge

	_BMPLIST_MAKE_LIST	msg_castle_room_choice_10,msg_castle_room_21,1	/*�Ă���*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_11,msg_castle_room_22,2	/*�g���[�i�[*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_13,msg_castle_room_24,4	/*��������*/

	/*�������쐬*/
	_CASTLE_TOOL		FC_ID_GET_LAP,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,GE,CASTLE_PASS_LAP_NUM,ev_castle_room_lap_3_make_list_ge_nukemiti

	_BMPLIST_START

#ifdef DEBUG_FRONTIER_LOOP
	_JUMP				ev_room_choice_24
#endif	//DEBUG_FRONTIER_LOOP

	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_room_choice_21	/*�Ă���*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,2,ev_room_choice_22	/*�g���[�i�[*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,4,ev_room_choice_24	/*��������*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,5,ev_room_choice_25	/*�ʂ��݂�*/

	/*�ʐM�̂݃��X�g�����폜�ł����ɔ����Ă���*/
	//_JUMP			ev_castle_send_sel2
	_JUMP			ev_castle_send_sel

	/*
	���q����Ɍ���
	�q���M:		�I�����N�G�X�g
	�e��M:		���X�g������@
	�e���M:		����ł�����@
	�q��M:		�e�̌�������炤 
	�e�q:		�e�̌���t���O�҂�

	���e����Ɍ���
	�e���M:		�I�����N�G�X�g����ł�����
	�q��M:		���X�g������
	�q���M:		���ɐe����M����Ă��Ӗ��̂Ȃ��f�[�^�𑗂�
	�e��M:		���łɐe���肪����Ă���̂ŃZ�b�g�͂��ꂸ�Ɏ�M
	�e�q:		�e�̌���t���O�҂�

	*/
	_END

/*2���ڂ܂ł̃E�B���h�E�ʒu*/
ev_castle_room_lap_3_make_list_lt:
	_BMPLIST_INIT_EX	24,11,0,0,FSW_ANSWER									/*B�L�����Z������*/
	//_BMPLIST_INIT_EX	24,11,0,0,FSW_ANSWER									/*B�L�����Z������*/
	_RET

/*3���ڈȍ~�ɑI���o����*/
ev_castle_room_lap_3_make_list_ge:

	/*�u���[���o�ꂩ�`�F�b�N*/
	_CASTLE_TOOL		FC_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_lap_3_make_list_lt
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_castle_room_lap_3_make_list_lt

	_BMPLIST_INIT_EX	24,9,0,0,FSW_ANSWER										/*B�L�����Z������*/
	//_BMPLIST_INIT_EX	24,11,0,0,FSW_ANSWER									/*B�L�����Z������*/
	_RET

////////////////////////////////
/*������ANSWER�͎g�p�s�I�I�I*/
////////////////////////////////
ev_castle_room_lap_3_make_list_ge_nukemiti:

	/*�u���[���o�ꂩ�`�F�b�N*/
	_CASTLE_TOOL		FC_ID_CHECK_BRAIN,0,0,FSW_PARAM4
	_IFVAL_JUMP			FSW_PARAM4,EQ,1,ev_castle_leader_nukemiti_off
	_IFVAL_JUMP			FSW_PARAM4,EQ,2,ev_castle_leader_nukemiti_off

	_BMPLIST_MAKE_LIST	msg_castle_room_choice_14,msg_castle_room_25,5	/*�ʂ��݂�*/
	//_BMPLIST_MAKE_LIST	msg_castle_room_choice_14,msg_castle_room_25,5		/*�ʂ��݂�*/
	_RET

/*�ʂ��݂���\�������Ȃ�*/
ev_castle_leader_nukemiti_off:
	_RET


/********************************************************************/
/*							�Ă���									*/
/********************************************************************/
ev_room_choice_21:
	/*�I�������^�C�v���Z�b�g*/
	_CASTLE_TOOL		FC_ID_SET_SEL_TYPE,CASTLE_SCR_MENU_TEMOTI,0,FSW_ANSWER

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_send_sel_temoti
	_JUMP				ev_castle_temoti_go
	_END

ev_castle_send_sel_temoti:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_castle_room_wait
	_JUMP				ev_castle_send_sel
	_END

ev_castle_temoti_go:
	/*����̑I�����̗p���ꂽ���`�F�b�N*/
	_CASTLE_TOOL		FC_ID_GET_PAIR_DECIDE_CHECK,0,0,FSW_ANSWER
	//_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_castle_pair_temoti_msg
	_CASTLE_TOOL		FC_ID_CLEAR_PARENT_CHECK_FLAG,0,0,FSW_ANSWER		/*�N���A���Ă���*/

	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�莝����ʌĂяo��*/
	_TALK_CLOSE

	/*CP�E�B���h�E�폜*/
	_CP_WIN_DEL

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_mine_call_sync

	_CASTLE_MINE_CALL
	//_SET_MAP_PROC

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_comm_command_initialize_2

	/*�ʐM�F��{�����Ƃ�*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_multi_comm_basic_call

	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_JUMP				ev_castle_room_go_rental_2
	_END

/*����̑I�����̗p���ꂽ���b�Z�[�W*/
ev_castle_pair_temoti_msg:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME			0
	_TALKMSG			msg_castle_room_26

	//�܂����������H

	_RET

/*�ʐM�}���`*/
ev_castle_mine_call_sync:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_MINE_CALL
	_COMM_RESET
	_RET


/********************************************************************/
/*							�g���[�i�[								*/
/********************************************************************/
ev_room_choice_22:
	/*�I�������^�C�v���Z�b�g*/
	_CASTLE_TOOL		FC_ID_SET_SEL_TYPE,CASTLE_SCR_MENU_TRAINER,0,FSW_ANSWER

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_send_sel_aite
	_JUMP				ev_castle_trainer_go
	_END

ev_castle_send_sel_aite:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_castle_room_wait
	_JUMP				ev_castle_send_sel
	_END

ev_castle_trainer_go:
	/*����̑I�����̗p���ꂽ���`�F�b�N*/
	_CASTLE_TOOL		FC_ID_GET_PAIR_DECIDE_CHECK,0,0,FSW_ANSWER
	//_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_castle_pair_trainer_msg
	_CASTLE_TOOL		FC_ID_CLEAR_PARENT_CHECK_FLAG,0,0,FSW_ANSWER		/*�N���A���Ă���*/

	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�G�g���[�i�[��ʌĂяo��*/
	_TALK_CLOSE

	/*CP�E�B���h�E�폜*/
	_CP_WIN_DEL

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_enemy_call_sync

	_CASTLE_ENEMY_CALL
	//_SET_MAP_PROC

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_comm_command_initialize_3

	/*�ʐM�F��{�����Ƃ�*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_multi_comm_basic_call

	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_JUMP				ev_castle_room_go_rental_2
	_END

/*����̑I�����̗p���ꂽ���b�Z�[�W*/
ev_castle_pair_trainer_msg:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME			0
	//_TALKMSG			msg_castle_room_27

	//�܂����������H

	_RET

/*�ʐM�}���`*/
ev_castle_enemy_call_sync:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_ENEMY_CALL
	_COMM_RESET
	_RET


/********************************************************************/
/*							��������								*/
/********************************************************************/
ev_room_choice_24:
	/*�I�������^�C�v���Z�b�g*/
	_CASTLE_TOOL		FC_ID_SET_SEL_TYPE,CASTLE_SCR_MENU_TAISEN,0,FSW_ANSWER

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_send_sel_taisen
	_JUMP				ev_castle_taisen_go
	_END

ev_castle_taisen_go:
	_LDVAL				FSW_LOCAL5,0										/*�ΐ킩�p�X���̔��ʗp*/
	_CASTLE_TOOL		FC_ID_CLEAR_PARENT_CHECK_FLAG,0,0,FSW_ANSWER		/*�N���A���Ă���*/
	_JUMP				ev_castle_room_go
	_END

ev_castle_send_sel_taisen:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_castle_room_wait
	_JUMP				ev_castle_send_sel
	_END


/********************************************************************/
/*																	*/
/********************************************************************/
ev_castle_comm_command_initialize_1:
	_TIME_WAIT			1,FSW_ANSWER
	_CASTLE_TOOL		FC_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_INIT_1
	_COMM_RESET
	_RET

ev_castle_comm_command_initialize_2:
	_TIME_WAIT			1,FSW_ANSWER
	_CASTLE_TOOL		FC_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_INIT_2
	_COMM_RESET
	_RET

ev_castle_comm_command_initialize_3:
	_TIME_WAIT			1,FSW_ANSWER
	_CASTLE_TOOL		FC_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_INIT_3
	_COMM_RESET
	_RET

ev_castle_comm_command_initialize_4:
	_TIME_WAIT			1,FSW_ANSWER
	_CASTLE_TOOL		FC_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_INIT_4
	_COMM_RESET
	_RET


/********************************************************************/
/*							�ʂ��݂�								*/
/********************************************************************/
ev_room_choice_25:
	/*�u���b�o������܂�����낵���ł����H�v*/
	_NUMBER_NAME		0,CP_USE_NUKEMITI
	_TALKMSG			msg_castle_room_29_1
	_YES_NO_WIN			FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_go_rental_2_sub	/*�u�������v*/

	/*CP������Ȃ����`�F�b�N*/
	_CASTLE_GET_CP		FSW_LOCAL1,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,LT,CP_USE_NUKEMITI,ev_castle_room_go_rental_3

	/*�I�������^�C�v���Z�b�g*/
	_CASTLE_TOOL		FC_ID_SET_SEL_TYPE,CASTLE_SCR_MENU_NUKEMITI,0,FSW_ANSWER

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_send_sel
	_JUMP				ev_castle_nukemiti_go
	_END

ev_castle_room_go_rental_3:
	/*�uCP������܂���v*/
	_TALKMSG			msg_castle_room_29_4
	_JUMP				ev_castle_room_go_rental_2_sub
	_END

ev_castle_nukemiti_go:
	_LDVAL				FSW_LOCAL5,1										/*�ΐ킩�p�X���̔��ʗp*/

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_nukemiti_go_comm
	_JUMP				ev_castle_nukemiti_go_normal
	_END

ev_castle_nukemiti_go_comm:
	/*����̑I�����̗p���ꂽ���`�F�b�N*/
	_CASTLE_TOOL		FC_ID_GET_PAIR_DECIDE_CHECK,0,0,FSW_ANSWER
	//_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_castle_pair_nukemiti_msg

	_CASTLE_TOOL		FC_ID_NUKEMITI_CP_SUB,0,0,FSW_ANSWER
	_CASTLE_TOOL		FC_ID_CLEAR_PARENT_CHECK_FLAG,0,0,FSW_ANSWER		/*�N���A���Ă���*/

	/*�u�ʂ��݂��͂����������I�v*/
	//_TALKMSG_NOSKIP		msg_castle_room_29_5
	//_TIME_WAIT			15,FSW_ANSWER
	//_JUMP				ev_castle_nukemiti_go_2

	/*�ΐ핔���ɍs������ɕύX(08/01/08)*/
	_JUMP				ev_castle_room_go
	_END

ev_castle_nukemiti_go_normal:
	_CASTLE_TOOL		FC_ID_GET_TYPE,0,0,FSW_ANSWER
	_CASTLE_SUB_CP		FSW_ANSWER,CP_USE_NUKEMITI

	/*�u�ʂ��݂��͂����������I�v*/
	//_TALKMSG			msg_castle_room_29_5
	//_AB_KEYWAIT
	//_JUMP				ev_castle_nukemiti_go_2

	/*�ΐ핔���ɍs������ɕύX(08/01/08)*/
	_JUMP				ev_castle_room_go
	_END


/********************************************************************/
/*																	*/
/********************************************************************/
ev_castle_nukemiti_go_2:
	_TALK_CLOSE

	/*CP�E�B���h�E�폜*/
	_CP_WIN_DEL

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_go_nukemiti_multi

	/*�ړ��A�j��*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_nukemiti_room
	_OBJ_ANIME			OBJID_BRAIN3_INFO,anm_man_go_nukemiti_room
	_OBJ_ANIME			OBJID_MINE,anm_player_go_nukemiti_room
	_OBJ_ANIME_WAIT

	_JUMP				ev_castle_room_go_nukemiti_2
	_END

/*����̑I�����̗p���ꂽ���b�Z�[�W*/
ev_castle_pair_nukemiti_msg:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME			0
	//_TALKMSG			msg_castle_room_29

	//�܂����������H

	_RET

ev_castle_room_go_nukemiti_multi:
	/*�ړ��A�j��*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_nukemiti_room
	_OBJ_ANIME			OBJID_BRAIN3_INFO,anm_man_go_nukemiti_room
	_OBJ_ANIME			OBJID_SIO_USER_0,anm_player_go_nukemiti_room
	_OBJ_ANIME			OBJID_SIO_USER_1,anm_player_go_nukemiti_room_multi
	_OBJ_ANIME_WAIT

	_JUMP				ev_castle_room_go_nukemiti_2
	_END

ev_castle_room_go_nukemiti_2:
	/*�����������փ}�b�v�؂�ւ�*/
	_SE_PLAY			SEQ_SE_DP_KAIDAN2
	_SE_WAIT			SEQ_SE_DP_KAIDAN2
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_go_nukemiti_2_multi

	_ACTOR_FREE			OBJID_PLAYER
	_ACTOR_FREE			OBJID_BRAIN3_INFO
	_ACTOR_FREE			OBJID_MINE
	_CHAR_RESOURCE_FREE	BRAINS3

	_MAP_CHANGE			FSS_SCENEID_CASTLE_BYPATH

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	_CHAR_RESOURCE_SET	default_scene3_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_scene3_actor		/*�A�N�^�[�o�^*/
	
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�������𔲂���*/
	//_OBJ_ANIME			OBJID_PLAYER,anm_player_go_nukemiti_center
	_OBJ_ANIME			OBJID_MINE,anm_player_go_nukemiti_center
	_OBJ_ANIME_WAIT

	_SE_PLAY			SEQ_SE_DP_KAIDAN2
	_SE_WAIT			SEQ_SE_DP_KAIDAN2
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_CALL				ev_castle_room_return			/*���������ɖ߂�*/
	_JUMP				ev_castle_room_go_nukemiti_3
	_END

ev_castle_room_go_nukemiti_2_multi:
	_ACTOR_FREE			OBJID_SIO_USER_0
	_ACTOR_FREE			OBJID_SIO_USER_1
	_ACTOR_FREE			OBJID_BRAIN3_INFO
	_CHAR_RESOURCE_FREE	BRAINS3

	_MAP_CHANGE			FSS_SCENEID_CASTLE_BYPATH

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	_CHAR_RESOURCE_SET	default_scene3_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_scene3_actor_multi		/*�A�N�^�[�o�^*/
	
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�������𔲂���*/
	//_OBJ_ANIME			OBJID_PLAYER,anm_player_go_nukemiti_center
	_OBJ_ANIME			OBJID_SIO_USER_0,anm_player_go_nukemiti_center
	_OBJ_ANIME			OBJID_SIO_USER_1,anm_player_go_nukemiti_center_multi
	_OBJ_ANIME_WAIT

	_SE_PLAY			SEQ_SE_DP_KAIDAN2
	_SE_WAIT			SEQ_SE_DP_KAIDAN2
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_CALL				ev_castle_room_return			/*���������ɖ߂�*/
	_JUMP				ev_castle_room_go_nukemiti_3
	_END


/********************************************************************/
/*					CP�͂��炦�Ȃ�����֐i�߂�						*/
/********************************************************************/
ev_castle_room_go_nukemiti_3:
	_CALL				ev_castle_battle_5

	/*PP�Ȃǂ̃|�P�����f�[�^�X�V(�G�̐��̂Ȃǂ̃t���O�N���A���K�v)*/
	_CASTLE_TOOL		FC_ID_BTL_WIN_POKE_DATA,0,0,FSW_ANSWER

	//_JUMP				ev_castle_battle_7

	/*��7�l�ڂłȂ����͓��ɖ߂�(�񕜂��܂����b�Z�[�W�͂Ȃ��I)*/
	_CASTLE_TOOL		FC_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,NE,7,ev_castle_room_saikai		/*�A������1-6�̎�*/

	/*�o�g���L�^�f�[�^�͂Ȃ��I*/
	_JUMP				ev_castle_room_7_win_bp						/*7�A��*/
	_END


/********************************************************************/
/*						�o�g���֌�����								*/
/********************************************************************/
ev_castle_room_go:
	/*CP�E�B���h�E�폜*/
	_CP_WIN_DEL

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_go_multi

	/*�u����ł́@�����Ɂ@�ǂ����v*/
	_TALKMSG			msg_castle_room_2
	_AB_KEYWAIT
	_TALK_CLOSE

	/*�ړ��A�j��*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_btl_room
	_OBJ_ANIME			OBJID_MINE,anm_player_go_btl_room
	_OBJ_ANIME_WAIT

	_JUMP				ev_castle_room_go_2
	_END

ev_castle_room_go_multi:
	/*�u����ł́@�����Ɂ@�ǂ����v*/
	_TALKMSG_NOSKIP		msg_castle_room_2
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_TALK_CLOSE

	/*�ړ��A�j��*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_btl_room
	_OBJ_ANIME			OBJID_SIO_USER_0,anm_player_go_btl_room
	_OBJ_ANIME			OBJID_SIO_USER_1,anm_player_go_btl_room_multi
	_OBJ_ANIME_WAIT
	_JUMP				ev_castle_room_go_2
	_END


/********************************************************************/
/*																	*/
/********************************************************************/
ev_castle_room_go_2:
	/*�ΐ핔���փ}�b�v�؂�ւ�*/
	_SE_PLAY			SEQ_SE_DP_KAIDAN2
	_SE_WAIT			SEQ_SE_DP_KAIDAN2
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*����̂�PCWOMAN2���폜*/
	_IFVAL_CALL			FSW_PARAM5,EQ,0,ev_castle_room_pcwoman2_del

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_go_2_multi

	_ACTOR_FREE			OBJID_PLAYER
	_ACTOR_FREE			OBJID_BRAIN3_INFO
	_ACTOR_FREE			OBJID_MINE
	_CHAR_RESOURCE_FREE	BRAINS3

	_MAP_CHANGE			FSS_SCENEID_CASTLE_BTL
	_CALL				ev_castle_room_common_01

	_CHAR_RESOURCE_SET	default_scene2_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_scene2_actor		/*�A�N�^�[�o�^*/
	
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�����܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_scroll_go_btl_center
	_OBJ_ANIME			OBJID_MINE,anm_player_go_btl_center
	_OBJ_ANIME_WAIT

	/*�X�N���[�����I�������L���L��������*/
	_PARTICLE_ADD_EMITTER	SPAWORK_0,CASTLE_BC_KIRA

	/*�p�X��I�����Ă�����*/
	_IFVAL_JUMP			FSW_LOCAL5,EQ,1,ev_castle_pass

	/*�u���[���o�ꂩ�`�F�b�N*/
	_CASTLE_TOOL		FC_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_leader_1st_02
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_castle_leader_2nd_02

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�G�g���[�i�[�������܂ŕ���*/
	_CALL				ev_castle_trainer_set_1
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in
	_OBJ_ANIME_WAIT

	_TIME_WAIT			15,FSW_ANSWER

	_OBJ_ANIME			OBJID_ETC,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_CASTLE_TOOL	FC_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_CASTLE_APPEAR	0
	_AB_KEYWAIT
	_TALK_CLOSE

	_JUMP				ev_castle_room_go_2_sub
	_END

ev_castle_room_go_2_sub:
	_JUMP				ev_castle_battle
	_END

ev_castle_room_go_2_multi:
	_ACTOR_FREE			OBJID_SIO_USER_0
	_ACTOR_FREE			OBJID_SIO_USER_1
	_ACTOR_FREE			OBJID_BRAIN3_INFO
	_CHAR_RESOURCE_FREE	BRAINS3

	_MAP_CHANGE			FSS_SCENEID_CASTLE_BTL
	_CALL				ev_castle_room_common_01

	_CHAR_RESOURCE_SET	default_scene2_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_scene2_actor_multi		/*�A�N�^�[�o�^*/
	
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�����܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_scroll_go_btl_center
	_OBJ_ANIME			OBJID_SIO_USER_0,anm_player_go_btl_center_sio
	_OBJ_ANIME			OBJID_SIO_USER_1,anm_player_go_btl_center_multi
	_OBJ_ANIME_WAIT

	/*�X�N���[�����I�������L���L��������*/
	_PARTICLE_ADD_EMITTER	SPAWORK_0,CASTLE_BC_KIRA

	/*�p�X��I�����Ă�����*/
	_IFVAL_JUMP			FSW_LOCAL5,EQ,1,ev_castle_pass

	/*�G�g���[�i�[�������܂ŕ���*/
	_CALL				ev_castle_trainer_set_2
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_sio
	_OBJ_ANIME			OBJID_ETC2,etc_anime_room_in_multi
	_OBJ_ANIME_WAIT

	_TIME_WAIT			15,FSW_ANSWER

	_OBJ_ANIME			OBJID_ETC,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_CASTLE_TOOL	FC_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_CASTLE_APPEAR	0
	_TIME_WAIT		30,FSW_ANSWER
	_TALK_CLOSE

	_OBJ_ANIME			OBJID_ETC2,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_CASTLE_TOOL	FC_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_CASTLE_APPEAR	1
	_TIME_WAIT		30,FSW_ANSWER
	_TALK_CLOSE

	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_castle_room_wait

	_JUMP			ev_castle_battle
	_END

/*����̂�PCWOMAN2���폜*/
ev_castle_room_pcwoman2_del:
	_ACTOR_FREE			OBJID_PCWOMAN2
	_CHAR_RESOURCE_FREE	FSW_PARAM6
	_LDVAL				FSW_PARAM5,1
	_RET

/*�V���O���A�}���`�̋��ʏ���*/
ev_castle_room_common_01:
	/*�p�[�e�B�N�����o*/
	_PARTICLE_SPA_LOAD	SPAWORK_0,CASTLE_SPA,FSS_CAMERA_ORTHO

	/*���񐔂ɂ����BG�̈ꕔ��ύX����*/
	_CASTLE_TOOL		FC_ID_LAP_NUM_WRITE,0,0,FSW_ANSWER

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7
	_RET


/********************************************************************/
/*						�p�X��I�����Ă�����						*/
/********************************************************************/
ev_castle_pass:

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_pass_multi

	/*�p�X�������̂���l�̃��b�Z�[�W*/
	_TALKMSG			msg_castle_room_35
	_AB_KEYWAIT
	_TALK_CLOSE
	_JUMP				ev_castle_battle_3_2
	_END

ev_castle_pass_multi:
	/*�p�X�������̂���l�̃��b�Z�[�W*/
	_TALKMSG_NOSKIP		msg_castle_room_35
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	//_TALK_CLOSE

	_TALKMSG_NOSKIP		msg_castle_room_wait

	//_JUMP				ev_castle_battle_3_2
	_CALL				ev_castle_battle_5
	_CALL				ev_castle_battle_6_2
	_TALK_CLOSE												/*��*/
	_JUMP				ev_castle_battle_3_sub
	_END


/********************************************************************/
/*					�g���[�i�[�̌����ڃZ�b�g						*/
/********************************************************************/
ev_castle_trainer_set_1:
	_CASTLE_TOOL		FC_ID_GET_TR_OBJ_CODE,0,0,FSW_ANSWER
	_LDWK				FSW_PARAM2,FSW_ANSWER
	_CHAR_RESOURCE_SET	etc_set_resource
	_ACTOR_SET			etc_set_actor
	_RET

ev_castle_trainer_set_2:
	_CASTLE_TOOL		FC_ID_GET_TR_OBJ_CODE,0,0,FSW_ANSWER
	_LDWK				FSW_PARAM2,FSW_ANSWER
	_CHAR_RESOURCE_SET	etc_set_resource
	//_ACTOR_SET			etc_set_actor

	_CASTLE_TOOL		FC_ID_GET_TR_OBJ_CODE,1,0,FSW_ANSWER
	_LDWK				FSW_PARAM3,FSW_ANSWER
	_CHAR_RESOURCE_SET	etc_set_resource2
	_ACTOR_SET			etc_set_actor2
	_RET

/*�u���[���Z�b�g*/
ev_castle_trainer_set_brain:
	_LDVAL				FSW_PARAM2,BRAINS3
	_CHAR_RESOURCE_SET	etc_set_resource
	_ACTOR_SET			etc_set_actor
	_RET


/********************************************************************/
//
/********************************************************************/
ev_castle_battle:
	/*�t�F�[�h�A�E�g*/
	//_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK

	/*�G���J�E���g�G�t�F�N�g�O�ɃL���L���I��������*/
	_PARTICLE_SPA_EXIT	SPAWORK_0

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_battle_2

	/*�u���[���o�ꂩ�`�F�b�N*/
	_CASTLE_TOOL		FC_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_leader_1st_33
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_castle_leader_2nd_33

	_ENCOUNT_EFFECT		FR_ENCOUNT_EFF_ZIGZAG
	_JUMP				ev_castle_battle_2
	_END

ev_castle_leader_1st_33:
ev_castle_leader_2nd_33:
	_RECORD_INC			RECID_FRONTIER_BRAIN
	_BRAIN_ENCOUNT_EFFECT	FRONTIER_NO_CASTLE
	_JUMP				ev_castle_battle_2
	_END

/*�ʐM�}���`*/
ev_castle_multi_battle_2:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_BATTLE
	_COMM_RESET

	_TALK_CLOSE
	_ENCOUNT_EFFECT		FR_ENCOUNT_EFF_ZIGZAG
	_JUMP				ev_castle_battle_2
	_END


/********************************************************************/
//
/********************************************************************/
ev_castle_battle_2:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	//_PARTICLE_SPA_EXIT	SPAWORK_0

#ifndef DEBUG_FRONTIER_LOOP

#ifndef DEBUG_BTL_OFF	/************************************************/
	/*�퓬�Ăяo��*/
	_CASTLE_BATTLE_CALL
	_CASTLE_CALL_GET_RESULT
#endif	/****************************************************************/

#endif	//DEBUG_FRONTIER_LOOP

	/*�p�[�e�B�N�����o*/
	_PARTICLE_SPA_LOAD	SPAWORK_0,CASTLE_SPA,FSS_CAMERA_ORTHO
	_PARTICLE_ADD_EMITTER	SPAWORK_0,CASTLE_BC_KIRA

	/*���񐔂ɂ����BG�̈ꕔ��ύX����*/
	_CASTLE_TOOL		FC_ID_LAP_NUM_WRITE,0,0,FSW_ANSWER

	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

#ifndef DEBUG_FRONTIER_LOOP

#ifndef DEBUG_BTL_LOSE_OFF	/********************************************/
	/*�퓬���ʂŕ���*/
	_CASTLE_LOSE_CHECK	FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_lose		/*�s�k*/
#endif	/****************************************************************/

#endif	//DEBUG_FRONTIER_LOOP

	/*����*/
	_RECORD_INC			RECID_CASTLE_WIN

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_battle_2_multi

	/*�u���[���o�ꂩ�`�F�b�N*/
	_CASTLE_TOOL		FC_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_leader_1st_03
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_castle_leader_2nd_03

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_go_out
	_OBJ_ANIME_WAIT
	_JUMP				ev_castle_battle_2_sub
	_END

ev_castle_battle_2_sub:
	_ACTOR_FREE			OBJID_ETC
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_JUMP				ev_castle_battle_3
	_END

ev_castle_battle_2_multi:
	_CALL				ev_castle_comm_command_initialize_4

	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_go_out_sio		/*��*/
	_OBJ_ANIME			OBJID_ETC2,etc_anime_go_out_multi	/*��*/
	_OBJ_ANIME_WAIT
	_ACTOR_FREE			OBJID_ETC
	_ACTOR_FREE			OBJID_ETC2
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_CHAR_RESOURCE_FREE	FSW_PARAM3

	_JUMP				ev_castle_battle_3
	_END


/********************************************************************/
/*					���������ɖ߂�O�ɂ��邱��						*/
/********************************************************************/
ev_castle_battle_3:
	_CALL				ev_castle_battle_5
	_CALL				ev_castle_battle_6
	_JUMP				ev_castle_battle_3_sub
	_END

/*�p�X�̎�*/
ev_castle_battle_3_2:
	_CALL				ev_castle_battle_5
	_CALL				ev_castle_battle_6_2
	_JUMP				ev_castle_battle_3_sub
	_END

ev_castle_battle_3_sub:
	_SE_PLAY			SEQ_SE_DP_KAIDAN2
	_SE_WAIT			SEQ_SE_DP_KAIDAN2

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_ACTOR_FREE			OBJID_PRINCESS
	_CHAR_RESOURCE_FREE	PRINCESS

	_CALL				ev_castle_room_return			/*���������ɖ߂�*/
	_JUMP				ev_castle_battle_4
	_END


/********************************************************************/
//
/********************************************************************/
ev_castle_battle_4:
	_JUMP				ev_castle_battle_7
	_END

ev_castle_battle_5:

#ifdef DEBUG_FRONTIER_LOOP
	_RET
#endif	//DEBUG_FRONTIER_LOOP

	/*�����l��+1*/
	_CASTLE_TOOL		FC_ID_INC_ROUND,1,0,FSW_ANSWER

	/*�A����+1*/
	_CASTLE_TOOL		FC_ID_INC_RENSYOU,0,0,FSW_ANSWER
	_RET

ev_castle_battle_6:
	_PLAYER_NAME		1

	/*CP�z�z�v�Z*/
	_CASTLE_TOOL		FC_ID_BTL_WIN_CP_POINT,0,0,FSW_PARAM4			/*�ޔ�*/
	_NUMBER_NAME		0,FSW_PARAM4

	_OBJ_ANIME			OBJID_PRINCESS,princess_anime_talk
	_OBJ_ANIME_WAIT

	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_msg00
	_JUMP				ev_castle_single_msg00
	_END

ev_castle_battle_6_sub:
	_TALK_CLOSE

	/*PP�Ȃǂ̃|�P�����f�[�^�X�V*/
	_CASTLE_TOOL		FC_ID_BTL_WIN_POKE_DATA,0,0,FSW_ANSWER

	_CHAR_RESOURCE_SET	brain_set_resource
	_ACTOR_SET			brain_set_actor
	_OBJ_ANIME			OBJID_BRAIN3_BTL_ROOM,brain_anime_room_in
	_OBJ_ANIME_WAIT

	/*�擾CP�ɂ���ă��b�Z�[�W��������*/
	_PLAYER_NAME		1
	_IFVAL_JUMP			FSW_PARAM4,GE,40,ev_castle_cp_num_01
	_IFVAL_JUMP			FSW_PARAM4,GE,25,ev_castle_cp_num_02
	_IFVAL_JUMP			FSW_PARAM4,GE,15,ev_castle_cp_num_03
	_JUMP				ev_castle_cp_num_04
	_END


/********************************************************************/
/*							CP�n�����b�Z�[�W						*/
/********************************************************************/
ev_castle_cp_num_01:
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_msg01
	_JUMP				ev_castle_single_msg01
	_END

ev_castle_cp_num_02:
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_msg02
	_JUMP				ev_castle_single_msg02
	_END

ev_castle_cp_num_03:
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_msg03
	_JUMP				ev_castle_single_msg03
	_END

ev_castle_cp_num_04:
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_msg04
	_JUMP				ev_castle_single_msg04
	_END

ev_castle_cp_num_common:
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_multi_msg05
	_JUMP				ev_castle_single_msg05
	_END

ev_castle_cp_num_common_sub:
	_ME_PLAY			ME_CP_GET
	_ME_WAIT
	_TALK_CLOSE

	_OBJ_ANIME			OBJID_BRAIN3_BTL_ROOM,brain_anime_go_out
	_OBJ_ANIME_WAIT
	_ACTOR_FREE			OBJID_BRAIN3_BTL_ROOM
	_CHAR_RESOURCE_FREE	BRAINS3
	////////////////////////////////////////////////////////////////////

	/*�ʐM�F��{�����Ƃ�*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_cp_num_multi
	_RET


/********************************************************************/
/*			CP��n������u���΂炭���܂����������v�}���`�p			*/
/********************************************************************/
ev_castle_cp_num_multi:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_castle_room_wait
	_CALL				ev_castle_multi_comm_basic_call
	_TALK_CLOSE												/*��*/
	_RET


/********************************************************************/
/*							�p�X�p									*/
/********************************************************************/
ev_castle_battle_6_2:
	/*CP�z�z�v�Z*/
	//_CASTLE_TOOL		FC_ID_BTL_WIN_CP_POINT,0,0,FSW_ANSWER

	/*PP�Ȃǂ̃|�P�����f�[�^�X�V*/
	_CASTLE_TOOL		FC_ID_BTL_WIN_POKE_DATA,0,0,FSW_ANSWER

	/*�ʐM�F��{�����Ƃ�*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_multi_comm_basic_call
	_RET

ev_castle_battle_7:

#ifndef DEBUG_7BTL_OFF	/********************************************/
	/*��7�l�ڂłȂ����͓��ɖ߂�*/
	_CASTLE_TOOL		FC_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,NE,7,ev_castle_room_renshou_17		/*�A������1-6�̎�*/
#endif	/************************************************************/

	_JUMP				ev_castle_room_7_win
	_END


/********************************************************************/
/*							7�A������(�L�^)							*/
/********************************************************************/
ev_castle_room_7_win:
	/*�p�X��I�����Ă�����*/
	_IFVAL_JUMP			FSW_LOCAL5,EQ,1,ev_castle_room_7_win_bp

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_castle_room_7_win_bp_sub

	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_castle_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX		FSW_ANSWER										/*�������f�t�H���g*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_7_win_bp
	_JUMP				ev_castle_room_7_win_rec_yes
	_END

ev_castle_room_7_win_bp_sub:
	_TALKMSG_NOSKIP		msg_castle_room_wait
	_JUMP				ev_castle_room_7_win_bp
	_END

/*�u�͂��v*/
ev_castle_room_7_win_rec_yes:
	_CALL				ev_castle_room_rec_win		/*�����̋L�^*/
	_JUMP				ev_castle_room_7_win_bp
	_END


/********************************************************************/
/*							7�A������(BP�擾)						*/
/********************************************************************/
ev_castle_room_7_win_bp:
	//7�A��(�N���A)�p�����[�^�Z�b�g
	_CASTLE_TOOL		FC_ID_SET_CLEAR,0,0,FSW_ANSWER

	/*7�A�������̂ŏI����*/
	//_LDVAL			WK_SCENE_CASTLE_LOBBY,1				/*7�A������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_CASTLE_LOBBY,1			/*7�A������n�܂�悤�ɂ��Ă���*/

	/*�u�V����@�Ƃ��ρ@���߂łƂ��v*/
	_PLAYER_NAME		0
	_TALKMSG			msg_castle_room_31

	_PLAYER_NAME		0										/*�v���C���[���Z�b�g*/

	/*�o�g���|�C���g�ǉ�*/
	_CASTLE_TOOL		FC_ID_GET_BP_POINT,0,0,FSW_ANSWER
	_NUMBER_NAME		1,FSW_ANSWER							/*���l�Z�b�g*/
	_BTL_POINT_ADD		FSW_ANSWER

	/*�u�a�o����������v*/
	_TALKMSG			msg_castle_room_32
	_ME_PLAY			ME_BP_GET
	_ME_WAIT

	_JUMP				ev_castle_room_bp_end
	_END


/********************************************************************/
/*							7�A�������I��							*/
/********************************************************************/
ev_castle_room_bp_end:
	_JUMP				ev_castle_lose_timing
	_END


/********************************************************************/
/*							�A���� 1-7								*/
/********************************************************************/
ev_castle_room_renshou_17:
	/*�p�X��I�����Ă�����*/
	_IFVAL_JUMP			FSW_LOCAL5,EQ,1,ev_castle_room_saikai

	/*�m���̃|�P���������邩�擾*/
	_CASTLE_TOOL		FC_ID_GET_HINSI_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_room_saikai	/*���Ȃ�*/

	/*�u�|�P�����̏�Ԃ������܂��v*/
	_TALKMSG			msg_castle_room_3
	//_ME_PLAY			SEQ_ASA
	//_ME_WAIT
	_SE_PLAY			SEQ_SE_DP_UG_020
	_SE_WAIT			SEQ_SE_DP_UG_020

	/*�o�g���Ăяo���̐퓬�p�����[�^�𐶐����鎞�ɏ�Ԉُ�{�C��񕜏��������Ă���*/
	//_PC_KAIFUKU

	_JUMP				ev_castle_room_saikai
	_END


/********************************************************************/
/*						���f�Z�[�u����̍ĊJ						*/
/********************************************************************/
ev_castle_room_saikai:
	/*�u���[���o�ꂩ�`�F�b�N*/
	_CASTLE_TOOL		FC_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_leader_1st_01
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_castle_leader_2nd_01

	_JUMP				ev_castle_room_saikai_00
	_END

ev_castle_room_saikai_00:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_menu_before_wait

	/*�u���́@����ڂł���v*/
	_CASTLE_TOOL		FC_ID_GET_ROUND,0,0,FSW_ANSWER
	_ADD_WK				FSW_ANSWER,1
	_NUMBER_NAME		0,FSW_ANSWER
	_TALKMSG			msg_castle_room_4
	_JUMP				ev_castle_room_saikai_02_sub
	_END

/*���j���[�\���O�̓���*/
ev_castle_room_menu_before_wait:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_castle_room_wait

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_MENU_BEFORE
	_COMM_RESET

	/*�A�������Z�b�g*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�u���́@����ڂł���v*/
	_CASTLE_TOOL		FC_ID_GET_ROUND,0,0,FSW_ANSWER
	_ADD_WK				FSW_ANSWER,1
	_NUMBER_NAME		0,FSW_ANSWER
	_TALKMSG_ALL_PUT	msg_castle_room_4
	_JUMP				ev_castle_room_saikai_02_sub
	_END

ev_castle_room_saikai_02_sub:
	_JUMP			ev_castle_room_menu_make
	_END


/********************************************************************/
/*							���j���[����							*/
/********************************************************************/
ev_castle_room_menu_make:
	_CASTLE_TOOL	FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_castle_room_menu_make_comm

	/*�p�X��I�����Ă�����u���낭����v���j���[�Ȃ�*/
	_IFVAL_JUMP		FSW_LOCAL5,EQ,1,ev_castle_room_menu_make_03_set

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,0,ev_castle_room_menu_make_04_set

	_JUMP			ev_castle_room_menu_make_03_set
	_END

/*�ʐM�̎��̃��j���[*/
ev_castle_room_menu_make_comm:

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_castle_room_menu_make_01_set

	/*�p�X��I�����Ă�����u���낭����v���j���[�Ȃ�*/
	_IFVAL_JUMP		FSW_LOCAL5,EQ,1,ev_castle_room_menu_make_01_set

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,0,ev_castle_room_menu_make_02_set

	_JUMP			ev_castle_room_menu_make_01_set
	_END

ev_castle_room_menu_make_01_set:
	_LDVAL				FSW_ANSWER,1
	_JUMP				ev_castle_room_menu_make_sub
	_END

ev_castle_room_menu_make_02_set:
	_LDVAL				FSW_ANSWER,2
	_JUMP				ev_castle_room_menu_make_sub
	_END

ev_castle_room_menu_make_03_set:
	_LDVAL				FSW_ANSWER,3
	_JUMP				ev_castle_room_menu_make_sub
	_END

ev_castle_room_menu_make_04_set:
	_LDVAL				FSW_ANSWER,4
	_JUMP				ev_castle_room_menu_make_sub
	_END

/*�ǂ̃��j���[�`���ɂ��邩*/
ev_castle_room_menu_make_sub:
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_menu_make_01
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_castle_room_menu_make_02
	_IFVAL_JUMP			FSW_ANSWER,EQ,3,ev_castle_room_menu_make_03
	_IFVAL_JUMP			FSW_ANSWER,EQ,4,ev_castle_room_menu_make_04
	_END

ev_castle_room_menu_make_01:
	_BMPLIST_INIT_EX	24,13,0,0,FSW_PARAM1			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0		/*�Â���*/
	_JUMP			ev_castle_room_yasumu_next
	_END

ev_castle_room_menu_make_02:
	_BMPLIST_INIT_EX	23,11,0,0,FSW_PARAM1			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0		/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_02,FSEV_WIN_TALK_MSG_NONE,1		/*���낭����*/
	_JUMP			ev_castle_room_yasumu_next
	_END

ev_castle_room_menu_make_03:
	_BMPLIST_INIT_EX	24,11,0,0,FSW_PARAM1			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0		/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_03,FSEV_WIN_TALK_MSG_NONE,2		/*�₷��*/
	_JUMP			ev_castle_room_yasumu_next
	_END

ev_castle_room_menu_make_04:
	_BMPLIST_INIT_EX	23,9,0,0,FSW_PARAM1				/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0		/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_02,FSEV_WIN_TALK_MSG_NONE,1		/*���낭����*/
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_03,FSEV_WIN_TALK_MSG_NONE,2		/*�₷��*/
	_JUMP			ev_castle_room_yasumu_next
	_END

ev_castle_room_yasumu_next:
	_BMPLIST_MAKE_LIST	msg_castle_room_choice_04,FSEV_WIN_TALK_MSG_NONE,3		/*���^�C�A*/
	_BMPLIST_START
	_JUMP			ev_castle_room_menu_sel
	_END


/********************************************************************/
/*							���j���[����							*/
/********************************************************************/
ev_castle_room_menu_sel:

#ifdef DEBUG_FRONTIER_LOOP
	_JUMP				ev_room_choice_01
#endif	//DEBUG_FRONTIER_LOOP

	_IFVAL_JUMP			FSW_PARAM1,EQ,0,ev_room_choice_01	/*�Â���*/
	_IFVAL_JUMP			FSW_PARAM1,EQ,1,ev_room_choice_02	/*���낭����*/
	_IFVAL_JUMP			FSW_PARAM1,EQ,2,ev_room_choice_03	/*�₷��*/
	_IFVAL_JUMP			FSW_PARAM1,EQ,3,ev_room_choice_04	/*���^�C�A*/
	_JUMP				ev_room_choice_04
	_END


/********************************************************************/
/*						�u�Â���v								*/
/********************************************************************/
ev_room_choice_01:
	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_room_choice_01_multi
	_JUMP				ev_room_choice_01_2
	_END

/*�ʐM�}���`*/
ev_room_choice_01_multi:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME			0
	_TALKMSG			msg_castle_room_10
	_JUMP				ev_room_choice_01_multi_retry
	_END

ev_room_choice_01_multi_retry:
	_LDVAL				FSW_LOCAL2,CASTLE_COMM_RETIRE
	_CASTLE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER							/*0=�Â���*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_room_choice_01_multi_retry	/*���M���s�́A�đ��M��*/

	_CASTLE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*���肪�I�񂾃��j���[���擾*/
	_CASTLE_TOOL		FC_ID_GET_RETIRE_FLAG,0,0,FSW_ANSWER

	/*�p�[�g�i�[�����^�C�A������*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_giveup_yes_multi	/*���^�C�A*/

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_MENU
	_COMM_RESET

	_JUMP				ev_room_choice_01_2
	_END

/*�p�[�g�i�[�����^�C�A������*/
ev_castle_room_giveup_yes_multi:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME			0
	_TALKMSG_NOSKIP		msg_castle_room_12
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_JUMP				ev_castle_room_giveup_yes_multi_sync
	_END


/********************************************************************/
/**/
/********************************************************************/
ev_room_choice_01_2:
	/*���[�N���N���A���Ă���*/
	_LDVAL				FSW_LOCAL3,0

	/*���̓G�|�P�����𐶐�*/
	_CASTLE_BTL_AFTER_PARTY_SET		/*�o�g�����POKEPARTY�̃Z�b�g*/

	/*�ʐM�}���`*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_multi_room_enemy_poke_send2_2

	_JUMP				ev_castle_room_go_rental_2
	_END


/********************************************************************/
/*							�L�^����*/
/********************************************************************/
ev_room_choice_02:
	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_castle_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX		FSW_ANSWER										/*�������f�t�H���g*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_rec_no
	_JUMP				ev_castle_room_rec_yes
	_END

/*�u�͂��v*/
ev_castle_room_rec_yes:
	_CALL				ev_castle_room_rec_win		/*�����̋L�^*/
	_JUMP				ev_castle_room_saikai		/*���j���[�֖߂�*/
	_END

/*�u�������v*/
ev_castle_room_rec_no:
	_JUMP				ev_castle_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*							���ʋL�^*/
/********************************************************************/
/*����*/
ev_castle_room_rec_win:
	_CALL				ev_castle_room_rec_common		/*ANSWER,PARAM0�g�p��*/
	//_ADD_WK			FSW_PARAM0,1
	//_SUB_WK			FSW_PARAM0,1					/*���E���h���ł͂Ȃ��������Ȃ̂�*/
	_JUMP				ev_castle_room_rec
	_END

/*����*/
ev_castle_room_rec_lose:
	_CALL				ev_castle_room_rec_common		/*ANSWER,PARAM0�g�p��*/
	_ADD_WK				FSW_PARAM0,1
	_JUMP				ev_castle_room_rec
	_END

/*���ʕ���*/
ev_castle_room_rec_common:
	/*�^��f�[�^�Z�[�u*/
	_CASTLE_TOOL		FC_ID_GET_RENSYOU,0,0,FSW_PARAM0
	_RET

ev_castle_room_rec:
	_JUMP				ev_castle_room_rec_sub
	_END

ev_castle_room_rec_sub:
	/*�L�^�������[�N�Z�b�g*/
	_LDVAL				FSW_LOCAL3,1

	/*WIFI�}���`�Ή� �^��O��CP���R�[�h��ޔ����āA���펞��CP�ɏ㏑��*/
	_CASTLE_TOOL		FC_ID_WIFI_MULTI_CP_TEMP,0,0,FSW_ANSWER

	_TALKMSG_ALL_PUT	msg_castle_room_6_4
	_CASTLE_TOOL		FC_ID_GET_TYPE,0,0,FSW_ANSWER
	_ADD_WAITICON
	_BATTLE_REC_SAVE	FRONTIER_NO_CASTLE,FSW_ANSWER,FSW_PARAM0,FSW_PARAM0
	_DEL_WAITICON

	/*WIFI�}���`�Ή� �^���ɑޔ����Ă��������݂�CP���R�[�h�ŏ㏑��*/
	_CASTLE_TOOL		FC_ID_WIFI_MULTI_CP_TEMP,1,0,FSW_ANSWER

	//_BATTLE_REC_LOAD

	_IFVAL_JUMP			FSW_PARAM0,EQ,1,ev_castle_room_rec_true

	/*�u�L�^�o���܂���ł����v*/
	_TALKMSG			msg_castle_room_6_2
	_RET

ev_castle_room_rec_true:
	_SE_PLAY			SEQ_SE_DP_SAVE
	/*�u�L�^����܂����v*/
	_PLAYER_NAME		0
	_TALKMSG			msg_castle_room_6_1
	_RET
	
/*�^��f�[�^���Ȃ������b�Z�[�W*/
ev_castle_room_rec_msg1:
	_TALKMSG			msg_castle_room_6
	_RET

/*���łɘ^��f�[�^�����鎞���b�Z�[�W*/
ev_castle_room_rec_msg2:
	_TALKMSG			msg_castle_room_6_3
	_RET


/********************************************************************/
/*							�₷��*/
/********************************************************************/
ev_room_choice_03:
	/*�u���|�[�g�������ďI�����܂����H�v*/
	_TALKMSG			msg_castle_room_7
	_YES_NO_WIN			FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_rest_no
	_JUMP				ev_castle_room_rest_yes
	_END

/*�u�͂��v*/
ev_castle_room_rest_yes:
	//_LDVAL			WK_SCENE_CASTLE_LOBBY,2			/*��������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_CASTLE_LOBBY,2		/*��������n�܂�悤�ɂ��Ă���*/

	//�v���C�f�[�^�Z�[�u
	_CASTLE_TOOL		FC_ID_SAVE_REST_PLAY_DATA,0,0,FSW_ANSWER

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_CASTLE_WORK_FREE

	_CALL				ev_castle_save			/*07.08.24 _CALL�ɒu��������*/

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_TALK_CLOSE

	//���Z�b�g�R�}���h
	_CASTLE_TOOL		FC_ID_SYSTEM_RESET,0,0,FSW_ANSWER
	_END

/*�u�������v*/
ev_castle_room_rest_no:
	_JUMP				ev_castle_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*							���^�C�A*/
/********************************************************************/
ev_room_choice_04:
	/*�u�o�g���L���b�X���̒���𒆎~����H�v*/
	_TALKMSG			msg_castle_room_8
	_YES_NO_WIN_EX		FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_giveup_no

	/*�ʐM*/
	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_giveup_yes_multi_wait_msg

	/*�u���a���肵�Ă���������@���Ԃ����܂��v*/
	_TALKMSG			msg_castle_room_34

	_JUMP				ev_castle_room_giveup_yes
	_END

/*�u���΂炭���҂����������v*/
ev_castle_room_giveup_yes_multi_wait_msg:
	_TALKMSG_NOSKIP		msg_castle_room_wait
	_JUMP				ev_castle_room_giveup_yes_multi_retry
	_END

ev_castle_room_giveup_yes_multi_retry:
	_LDVAL				FSW_LOCAL2,CASTLE_COMM_RETIRE
	_CASTLE_SEND_BUF	FSW_LOCAL2,1,FSW_ANSWER									/*1=���^�C��*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_room_giveup_yes_multi_retry	/*���M���s�͍đ��M*/

	//_CASTLE_RECV_BUF	FSW_LOCAL2
	//_COMM_RESET
	_JUMP				ev_castle_room_giveup_yes_multi_sync
	_END

ev_castle_room_giveup_yes_multi_sync:
	/*�u���a���肵�Ă���������@���Ԃ����܂��v*/
	_TALKMSG			msg_castle_room_34

	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_castle_room_wait

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_GIVE
	//_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_MENU
	_COMM_RESET
	_JUMP				ev_castle_room_giveup_yes
	_END

/*�u�͂��v*/
ev_castle_room_giveup_yes:
	//�s��p�����[�^�Z�b�g
	_CASTLE_TOOL		FC_ID_SET_LOSE,0,0,FSW_ANSWER

	//_LDVAL			WK_SCENE_CASTLE_LOBBY,3			/*���^�C������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_CASTLE_LOBBY,3		/*���^�C������n�܂�悤�ɂ��Ă���*/

	_JUMP				ev_castle_room_end_save
	_END

/*�u�������v*/
ev_castle_room_giveup_no:
	_JUMP				ev_castle_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*								�s�k*/
/********************************************************************/
ev_castle_lose:
	_SE_PLAY			SEQ_SE_DP_KAIDAN2
	_SE_WAIT			SEQ_SE_DP_KAIDAN2
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_ACTOR_FREE			OBJID_PRINCESS
	_CHAR_RESOURCE_FREE	PRINCESS
	_CALL				ev_castle_room_return			/*���������ɖ߂�*/

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_castle_lose_timing_sub

	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_castle_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_castle_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER								/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_castle_lose_rec_no	/*�u�������v*/
	_JUMP			ev_castle_lose_rec_yes					/*�u�͂��v*/
	_END

ev_castle_lose_timing_sub:
	_TALKMSG_NOSKIP		msg_castle_room_wait
	_JUMP			ev_castle_lose_timing_call
	_END

/*�u�͂��v*/
ev_castle_lose_rec_yes:
	_CALL			ev_castle_room_rec_lose					/*�s�k�̋L�^*/
	_JUMP			ev_castle_lose_timing_call
	_END

/*�u�������v*/
ev_castle_lose_rec_no:
	_JUMP			ev_castle_lose_timing_call
	_END

ev_castle_lose_timing_call:
	//�s��p�����[�^�Z�b�g
	_CASTLE_TOOL		FC_ID_SET_LOSE,0,0,FSW_ANSWER

	//_LDVAL			WK_SCENE_CASTLE_LOBBY,3			/*���^�C������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_CASTLE_LOBBY,3		/*���^�C������n�܂�悤�ɂ��Ă���*/

	_JUMP			ev_castle_lose_timing
	_END

ev_castle_lose_timing:
	/*�u���a���肵�Ă���������@���Ԃ����܂��v*/
	_TALKMSG		msg_castle_room_34

	_CASTLE_TOOL	FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_castle_multi_lose_timing
	_JUMP			ev_castle_room_end_save
	_END

/*�ʐM����*/
ev_castle_multi_lose_timing:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_castle_room_wait

	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_LOSE_END
	_COMM_RESET
	_JUMP				ev_castle_room_end_save
	_END

ev_castle_room_end_save:
	_CALL				ev_castle_save			/*07.08.24 _CALL�ɒu��������*/
	_TALK_CLOSE
	_JUMP				ev_castle_room_end
	_END


/********************************************************************/
/*							���ʏI��								*/
/********************************************************************/
ev_castle_room_end:

	/*�Z�[�u��ɓ���������*/
	_CASTLE_TOOL	FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_castle_multi_end_timing

	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_CASTLE_TOOL		FC_ID_GET_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,CASTLE_TYPE_MULTI,ev_castle_room_multi_tv

	_CASTLE_TOOL		FC_ID_GET_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,CASTLE_TYPE_WIFI_MULTI,ev_castle_room_end_wifi

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_CASTLE_WORK_FREE

	/*2D�}�b�v�T������*/
	//

	//_END
	_SCRIPT_FINISH			/*_END����2D�}�b�v�I��*/

/*�}���`�̂�*/
ev_castle_room_multi_tv:
	/*TV����*/
	_TV_TEMP_FRIEND_SET	FRONTIER_NO_CASTLE
	_RET


/********************************************************************/
/*						�Z�[�u��ɓ���������						*/	
/********************************************************************/
ev_castle_multi_end_timing:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_SAVE_AFTER
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/
	_RET


/********************************************************************/
/*						WIFI�͑҂����킹��ʂ֖߂�					*/	
/********************************************************************/
ev_castle_room_end_wifi:
	/*�t�F�[�h�A�E�g*/
	//_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_CASTLE_WORK_FREE

	_MAP_CHANGE_EX		FSS_SCENEID_WIFI_COUNTER,1
	_END


/********************************************************************/
//							���ʃZ�[�u	
/********************************************************************/
ev_castle_save:
	/*�u���|�[�g�ɏ�������ł��܂��@�d����؂�Ȃ��ŉ������v*/
	_TALKMSG_ALL_PUT	msg_castle_room_36
	_ADD_WAITICON
	_REPORT_DIV_SAVE	FSW_ANSWER
	_DEL_WAITICON
	_SE_PLAY			SEQ_SE_DP_SAVE
	_SE_WAIT			SEQ_SE_DP_SAVE
	_RET


/********************************************************************/
/*			�����N�A�ʂ��݂��Ȃǂ̑I����ʐM(��������ȊO)			*/
/********************************************************************/
ev_castle_send_sel:
	_LDVAL				FSW_LOCAL2,CASTLE_COMM_SEL
	_CASTLE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_castle_send_sel	/*���M���s�́A�đ��M��*/

	_CASTLE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�e�̌���҂�*/
	//_CASTLE_PARENT_WAIT	FSW_ANSWER
	_JUMP				ev_castle_send_sel2
	_END

ev_castle_send_sel2:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_CASTLE_CHANGE
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/

	/*�e�̌�����擾���ď�������*/
	_CASTLE_TOOL		FC_ID_GET_PARENT_CHECK_FLAG,0,0,FSW_ANSWER
	_SWITCH				FSW_ANSWER
	_CASE_JUMP			CASTLE_SCR_MENU_TEMOTI,ev_castle_temoti_go			/*�Ă���*/
	_CASE_JUMP			CASTLE_SCR_MENU_TRAINER,ev_castle_trainer_go		/*�g���[�i�[*/
	_CASE_JUMP			CASTLE_SCR_MENU_TAISEN,ev_castle_taisen_go			/*��������*/
	_CASE_JUMP			CASTLE_SCR_MENU_NUKEMITI,ev_castle_nukemiti_go		/*�ʂ��݂�*/
	_END


/********************************************************************/
/*					���ʏ����F���������ɖ߂�(CALL)					*/
/********************************************************************/
ev_castle_room_return:
	_PARTICLE_SPA_EXIT	SPAWORK_0

	_CASTLE_TOOL		FC_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_castle_room_return_multi

	_ACTOR_FREE			OBJID_PLAYER
	_ACTOR_FREE			OBJID_MINE
	//_ACTOR_FREE		OBJID_ETC
	//_CHAR_RESOURCE_FREE	FSW_PARAM2

	_MAP_CHANGE			FSS_SCENEID_CASTLE_ROOM

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	_LDVAL				FSW_PARAM6,BFSM
	_CHAR_RESOURCE_SET	default_set_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			btl_after_set_actor			/*�A�N�^�[�o�^*/

	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_RET

ev_castle_room_return_multi:
	_ACTOR_FREE			OBJID_SIO_USER_0
	_ACTOR_FREE			OBJID_SIO_USER_1
	//_ACTOR_FREE		OBJID_ETC
	//_CHAR_RESOURCE_FREE	FSW_PARAM2

	_MAP_CHANGE			FSS_SCENEID_CASTLE_ROOM

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	_LDVAL				FSW_PARAM6,BFSW1
	_CHAR_RESOURCE_SET	default_set_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			btl_after_set_actor_multi	/*�A�N�^�[�o�^*/

	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_RET


/********************************************************************/
/*						�u���[���̗���								*/
/********************************************************************/
ev_castle_leader_1st_01:
ev_castle_leader_2nd_01:

	/*�L�^�������[�N�Ƀf�[�^���������܂�Ă��Ȃ� = ���f���A�Ƃ��čl����*/
	/*�o�ꂵ�܂��惁�b�Z�[�W�͕\�����Ȃ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,1,ev_castle_room_saikai_00

	/*��x�u���[���o�ꃁ�b�Z�[�W��\��������*/
	_CASTLE_TOOL	FC_ID_BRAIN_APPEAR_MSG_CHK,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_castle_room_saikai_00

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�u�u���[���Ƃ̏����ɂȂ�܂��I�o��͂����ł����H�v*/
	_PLAYER_NAME		0
	_TALKMSG			msg_castle_room_boss_01
	_JUMP				ev_castle_room_saikai_02_sub
	_END

ev_castle_leader_1st_02:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�o��*/
	_CALL				ev_castle_leader_appear

	/*�퓬�O�̃��b�Z�[�W(1����)*/
	_TALKMSG			msg_castle_room_boss_02
	_TALK_CLOSE

	_JUMP				ev_castle_room_go_2_sub
	_END

ev_castle_leader_2nd_02:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�o��*/
	_CALL				ev_castle_leader_appear

	/*�퓬�O�̃��b�Z�[�W(2����)*/
	_PLAYER_NAME		0
	_TALKMSG			msg_castle_room_boss_03
	_TALK_CLOSE

	_JUMP				ev_castle_room_go_2_sub
	_END

/*�o��*/
ev_castle_leader_appear:
	/*�����ɂ��ău���[���������܂ŕ���*/
	_CALL				ev_castle_trainer_set_brain
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain
	_OBJ_ANIME_WAIT

	/*�p�[�e�B�N�����o*/
	//

	/*�\��*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain_02
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*						�u���[���ɏ������msg						*/
/********************************************************************/
ev_castle_leader_1st_03:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_CASTLE,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_NONE,ev_castle_meory_print_put_1st

	/*�퓬��̃��b�Z�[�W(1����)*/
	_PLAYER_NAME		0
	_TALKMSG			msg_castle_room_boss_04
	_TALK_CLOSE
	_JUMP				ev_castle_battle_2_sub_brain
	_END

/*��t��1st�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_castle_meory_print_put_1st:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_CASTLE,MEMORY_PRINT_PUT_1ST
	_RET

ev_castle_leader_2nd_03:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_castle_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_CASTLE,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_PUT_OK_1ST,ev_castle_meory_print_put_2nd

	/*�퓬��̃��b�Z�[�W(2����)*/
	_TALKMSG			msg_castle_room_boss_05
	_TALK_CLOSE
	_JUMP				ev_castle_battle_2_sub_brain
	_END

/*��t��2nd�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_castle_meory_print_put_2nd:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_CASTLE,MEMORY_PRINT_PUT_2ND
	_RET

/*�u���[����p�ޏ�*/
ev_castle_battle_2_sub_brain:
	_OBJ_ANIME			OBJID_ETC,etc_anime_go_out_brain
	_OBJ_ANIME_WAIT
	_JUMP				ev_castle_battle_2_sub
	_END


/********************************************************************/
/*					�A�������擾(FSW_ANSWER�g�p)					*/
/********************************************************************/
ev_castle_rensyou_get:
	/*�A�����擾*/
	_CASTLE_TOOL		FC_ID_GET_RENSYOU,0,0,FSW_ANSWER

	/*���ł�9999�̎��́A0 �I���W���̕␳�������Ȃ�*/
	_IFVAL_JUMP			FSW_ANSWER,GE,CASTLE_RENSYOU_MAX,ev_castle_rensyou_ret

	_ADD_WK				FSW_ANSWER,1
	_RET

ev_castle_rensyou_ret:
	_RET


/********************************************************************/
/*		(�J�g���A)�V���O�����A�ʐM���Ń��b�Z�[�W�\�����@�𕪂���	*/
/********************************************************************/
ev_castle_single_msg00:
	_TALKMSG			msg_castle_room_30
	_AB_KEYWAIT
	_JUMP				ev_castle_battle_6_sub
	_END

ev_castle_multi_msg00:
	_TALKMSG_NOSKIP		msg_castle_room_30
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_JUMP				ev_castle_battle_6_sub
	_END


/********************************************************************/
/*		(�R�N����01)�V���O�����A�ʐM���Ń��b�Z�[�W�\�����@�𕪂���	*/
/********************************************************************/
ev_castle_single_msg01:
	_TALKMSG			msg_castle_room_30_01_2
	_JUMP				ev_castle_cp_num_common
	_END

ev_castle_multi_msg01:
	_TALKMSG_NOSKIP		msg_castle_room_30_01
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_JUMP				ev_castle_cp_num_common
	_END


/********************************************************************/
/*		(�R�N����02)�V���O�����A�ʐM���Ń��b�Z�[�W�\�����@�𕪂���	*/
/********************************************************************/
ev_castle_single_msg02:
	_TALKMSG			msg_castle_room_30_02_2
	_JUMP				ev_castle_cp_num_common
	_END

ev_castle_multi_msg02:
	_TALKMSG_NOSKIP		msg_castle_room_30_02
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_JUMP				ev_castle_cp_num_common
	_END


/********************************************************************/
/*		(�R�N����03)�V���O�����A�ʐM���Ń��b�Z�[�W�\�����@�𕪂���	*/
/********************************************************************/
ev_castle_single_msg03:
	_TALKMSG			msg_castle_room_30_03_2
	_JUMP				ev_castle_cp_num_common
	_END

ev_castle_multi_msg03:
	_TALKMSG_NOSKIP		msg_castle_room_30_03
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_JUMP				ev_castle_cp_num_common
	_END


/********************************************************************/
/*		(�R�N����04)�V���O�����A�ʐM���Ń��b�Z�[�W�\�����@�𕪂���	*/
/********************************************************************/
ev_castle_single_msg04:
	_TALKMSG			msg_castle_room_30_04_2
	_JUMP				ev_castle_cp_num_common
	_END

ev_castle_multi_msg04:
	_TALKMSG_NOSKIP		msg_castle_room_30_04
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_JUMP				ev_castle_cp_num_common
	_END


/********************************************************************/
/*		(�R�N����05)�V���O�����A�ʐM���Ń��b�Z�[�W�\�����@�𕪂���	*/
/********************************************************************/
ev_castle_single_msg05:
	_TALKMSG			msg_castle_room_30_05
	/*ME�Đ��ɍs���̂ŃL�[�҂�����Ȃ�*/
	_JUMP				ev_castle_cp_num_common_sub
	_END

ev_castle_multi_msg05:
	_TALKMSG_NOSKIP		msg_castle_room_30_05
	_JUMP				ev_castle_cp_num_common_sub
	_END



//==============================================================================
/**
 * @file	tower.s
 * @brief	�u�o�g���^���[�v�t�����e�B�A�X�N���v�g(�^���[�}���`�ʘH)
 * @author	nohara
 * @date	2007.05.29
 */
//==============================================================================

	.text

	.include	"../frontier_seq_def.h"
	.include	"../../../include/field/evwkdef.h"
	.include	"../../fielddata/script/saveflag.h"
	.include	"../../fielddata/script/savework.h"
	.include	"../../field/b_tower_scr_def.h"


//--------------------------------------------------------------------
//
//					     �X�N���v�g�{��
//
//	FSW_LOCAL1		WK_SCENE_D31R0204���i�[
//	FSW_LOCAL2		�A�j�������鍶����OBJID
//	FSW_LOCAL3		�A�j��������E����OBJID
//	FSW_PARAM2		��l���̌����ڊi�[(AI�}���`�̎���)
//	FSW_PARAM3		�p�[�g�i�[�̌����ڊi�[(AI�}���`��5�l�W)
//
//--------------------------------------------------------------------
_EVENT_DATA		fss_tower_wayd_common	//��ԏ��EVENT_DATA�͎������s
_EVENT_DATA_END						//�I��


//--------------------------------------------------------------------
//							���W��`
//--------------------------------------------------------------------
#define PLAYER_X		(8*17)
#define PLAYER_Y		(8*11)
#define PCWOMAN2_01_X	(8*7)
#define PCWOMAN2_01_Y	(8*4)
#define PCWOMAN2_02_X	(8*29)
#define PCWOMAN2_02_Y	(PCWOMAN2_01_Y)
#define MINE_X			(PCWOMAN2_01_X)
#define MINE_Y			(8*4)
#define PAIR_X			(PCWOMAN2_02_X)
#define PAIR_Y			(MINE_Y)
#define SIO_USER_0_X	(PCWOMAN2_01_X)
#define SIO_USER_0_Y	(8*4)
#define SIO_USER_1_X	(PCWOMAN2_02_X)
#define SIO_USER_1_Y	(SIO_USER_0_Y)


/********************************************************************/
//
//		SCENE_TOWER:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
//AI�}���`
_RESOURCE_LABEL	default_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		FSW_PARAM2,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		FSW_PARAM3,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//�ʐM�}���`
_RESOURCE_LABEL	comm_multi_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END


//====================================================================
//	SCENE_TOWER:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
#define OBJID_PLAYER				(0)
#define OBJID_PCWOMAN2_01			(1)
#define OBJID_PCWOMAN2_02			(2)
#define OBJID_MINE					(3)		//AI�}���`�̎���(��l���͉�ʒ��S�Ŕ�\��)
#define OBJID_PAIR					(4)		//AI�}���`�̃p�[�g�i�[
#define OBJID_SIO_USER_0			(5)
#define OBJID_SIO_USER_1			(6)

//�X�N���v�gID(����Ή��\��)
#define EVENTID_TEST_SCR_OBJ1		(1)

//AI�}���`
_ACTOR_LABEL	default_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_DOWN, \
							PLAYER_X,PLAYER_Y,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_ACTOR_DATA				OBJID_PCWOMAN2_01,FSW_PARAM6,WF2DMAP_WAY_C_DOWN, \
							PCWOMAN2_01_X,PCWOMAN2_01_Y,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PCWOMAN2_02,FSW_PARAM6,WF2DMAP_WAY_C_DOWN, \
							PCWOMAN2_02_X,PCWOMAN2_02_Y,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							MINE_X,MINE_Y,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PAIR,FSW_PARAM3,WF2DMAP_WAY_C_DOWN, \
							PAIR_X,PAIR_Y,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

//�ʐM�}���`
_ACTOR_LABEL	comm_multi_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_DOWN, \
							PLAYER_X,PLAYER_Y,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_ACTOR_DATA				OBJID_PCWOMAN2_01,FSW_PARAM6,WF2DMAP_WAY_C_DOWN, \
							PCWOMAN2_01_X,PCWOMAN2_01_Y,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PCWOMAN2_02,FSW_PARAM6,WF2DMAP_WAY_C_DOWN, \
							PCWOMAN2_02_X,PCWOMAN2_02_Y,OFF,EVENTID_TEST_SCR_OBJ1
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_DOWN, \
							SIO_USER_0_X,SIO_USER_0_Y,OFF
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_DOWN, \
							SIO_USER_1_X,SIO_USER_1_Y,OFF
	_ACTOR_DATA_END


//====================================================================
//	�z�u���A�N�^�[
//====================================================================
//--------------------------------------------------------------
//	�z�u�����\�[�X���x��
//--------------------------------------------------------------
_ARTICLE_RESOURCE_LABEL	article_door_resource
	_ARTICLE_RESOURCE_DATA	ACTID_TOWER_DOOR
	_ARTICLE_RESOURCE_DATA_END

//--------------------------------------------------------------
//	�z�u���A�N�^�[
//--------------------------------------------------------------
_ARTICLE_ACTOR_LABEL	article_door_actor
	//id,x,y,visible,anm_start,act_work
	_ARTICLE_ACTOR_DATA		ACTID_TOWER_DOOR, 72, 48, ON, OFF, ACTWORK_0
	_ARTICLE_ACTOR_DATA		ACTID_TOWER_DOOR, 216, 48, ON, OFF, ACTWORK_1
	_ARTICLE_ACTOR_DATA_END


/********************************************************************/
/*							�A�j��									*/
/********************************************************************/

//--------------------------------------------------------------------
//�g���[�i�[�@In
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0204_pcwoman2_in
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,3
	_ANIME_DATA	FC_DIR_U,1
	_ANIME_DATA_END

_ANIME_LABEL	anm_d31r0204_trainer_in
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1
	_ANIME_DATA	FC_WALK_D_8F,2
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�g���[�i�[01�@walk
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0204_trainer_right
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,5
	_ANIME_DATA_END

_ANIME_LABEL	anm_d31r0204_trainer_left
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_L_8F,5
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�g���[�i�[ out�@
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0204_trainer_out
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�󂯕t����@walk
//--------------------------------------------------------------------
_ANIME_LABEL	anm_d31r0204_pcwoman2_right
	_ANIME_DATA	FC_WALK_R_8F,5
	_ANIME_DATA FC_WALK_U_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA	FC_WAIT_4F,1
	_ANIME_DATA	FC_DIR_U,1
	_ANIME_DATA	FC_WAIT_4F,1
	_ANIME_DATA_END

_ANIME_LABEL	anm_d31r0204_pcwoman2_left
	_ANIME_DATA	FC_WALK_L_8F,5
	_ANIME_DATA FC_WALK_U_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA	FC_WAIT_4F,1
	_ANIME_DATA	FC_DIR_U,1
	_ANIME_DATA	FC_WAIT_4F,1
	_ANIME_DATA_END
	
_ANIME_LABEL	anm_d31r0204_pcwoman2_out
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1
	_ANIME_DATA_END


/********************************************************************/
/*								�J�n								*/
/********************************************************************/
fss_tower_wayd_common:
	_SAVE_EVENT_WORK_GET	WK_SCENE_D31R0204,FSW_LOCAL1

	_CALL				ev_d31r0204_flag_change
	_CALL				ev_d31r0204_obj_change

	/*���\�[�X���A�N�^�[�o�^*/
	_LDVAL				FSW_PARAM6,BFSW1
	_SE_PLAY			SEQ_SE_DP_DOOR10
	_ARTICLE_RESOURCE_SET	article_door_resource
	_ARTICLE_ACTOR_SET		article_door_actor
	
	/*�E����H�t���b�v������*/
	_ARTICLE_ACTOR_FLIP_H_SET	ACTWORK_1

	/*�v���C���[�h�擾*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER

	/*�ʐM�}���`*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,tower_wayd_comm_multi
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,tower_wayd_comm_multi

	/*AI�}���`*/
	_LDVAL				FSW_LOCAL2,OBJID_MINE
	_LDVAL				FSW_LOCAL3,OBJID_PAIR
	_CHAR_RESOURCE_SET	default_set_resource				/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_set_actor					/*�A�N�^�[�o�^*/
	_JUMP				tower_wayd_common2
	_END

tower_wayd_comm_multi:
	_LDVAL				FSW_LOCAL2,OBJID_SIO_USER_0
	_LDVAL				FSW_LOCAL3,OBJID_SIO_USER_1
	_CHAR_RESOURCE_SET	comm_multi_set_resource				/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			comm_multi_set_actor				/*�A�N�^�[�o�^*/
	_JUMP				tower_wayd_common2
	_END

tower_wayd_common2:
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_CALL				ev_tower_wayd_comanm_in
	_CALL				ev_tower_wayd_comanm_walk
	_CALL				ev_tower_wayd_comanm_out

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*���X�ƃA�N�^�[�폜*/
	_ARTICLE_ACTOR_FREE		ACTWORK_0
	_ARTICLE_ACTOR_FREE		ACTWORK_1
	
	/*���\�[�X���(�A�N�^�[�̓��[�N�ԍ��A���\�[�X��ACTID�ŉ�����鎖�ɒ���)*/
	_ARTICLE_RESOURCE_FREE	ACTID_TOWER_DOOR

	/*�v���C���[�h�擾*/
	_BTOWER_TOOLS		BTWR_SUB_GET_PLAY_MODE,BTWR_NULL_PARAM,FSW_ANSWER

	/*�ʐM�}���`*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_COMM_MULTI,tower_wayd_comm_multi2
	_IFVAL_JUMP			FSW_ANSWER,EQ,BTWR_MODE_WIFI_MULTI,tower_wayd_comm_multi2

	/*AI�}���`*/
	_ACTOR_FREE			OBJID_PLAYER
	_ACTOR_FREE			OBJID_PCWOMAN2_01
	_ACTOR_FREE			OBJID_PCWOMAN2_02
	_ACTOR_FREE			OBJID_MINE
	_ACTOR_FREE			OBJID_PAIR
	_CHAR_RESOURCE_FREE	FSW_PARAM6
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_CHAR_RESOURCE_FREE	FSW_PARAM3
	_JUMP				tower_wayd_common3
	_END

tower_wayd_comm_multi2:
	_ACTOR_FREE			OBJID_PLAYER
	_ACTOR_FREE			OBJID_PCWOMAN2_01
	_ACTOR_FREE			OBJID_PCWOMAN2_02
	_ACTOR_FREE			OBJID_SIO_USER_0
	_ACTOR_FREE			OBJID_SIO_USER_1
	_CHAR_RESOURCE_FREE	FSW_PARAM6
	_JUMP				tower_wayd_common3
	_END

tower_wayd_common3:
	//_MAP_CHANGE		ZONE_ID_D31R0206,0,SB_ROOMD_PLAYER_SX,SB_ROOMD_PLAYER_SY,DIR_UP
	_MAP_CHANGE_EX		FSS_SCENEID_TOWER_MULTI_BTL,0
	_END


/********************************************************************/
/*						FLAG_CHANGE_LABEL 							*/
/********************************************************************/
ev_d31r0204_flag_change:
	_IFVAL_JUMP			FSW_LOCAL1,EQ,0,ev_d31r0204_flag_change_end

	/*
	  �ʐM�̎q�̎��͐e�q�̍��E�ʒu�����ꂩ����
	  �J�����gID�擾�֐��͔�ڑ����ɌĂԂ�0���Ԃ�
	*/

	//_COMM_GET_CURRENT_ID	FSW_LOCAL0
	_IFVAL_JUMP			FSW_LOCAL0,EQ,1,ev_d31r0204_flag_change_child

	/*��ڑ����Ɛe�̂Ƃ�*/
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_PARAM2
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_PARAM3
#if 0
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_PARAM3
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_LOCAL0
	//_LDWK				OBJCHRWORK0,FSW_LOCAL0
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_PARAM2
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_LOCAL0
	//_LDWK				OBJCHRWORK1,FSW_LOCAL0
#endif
	_JUMP				ev_d31r0204_flag_change_end
	_END

ev_d31r0204_flag_change_end:
	_RET

/*�ʐM�̎q�̎�*/
ev_d31r0204_flag_change_child:
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_PARAM2
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_PARAM3
#if 0
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_PARAM2
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE,FSW_LOCAL0
	//_LDWK				OBJCHRWORK1,FSW_LOCAL0
	_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_PARAM3
	//_BTOWER_TOOLS		BTWR_SUB_GET_MINE_OBJ,BTWR_PTCODE_MINE2,FSW_LOCAL0
	//_LDWK				OBJCHRWORK0,FSW_LOCAL0
#endif
	_RET


/********************************************************************/
/*				�o�g���^���[�@�}���`�ʘH�A�j���R����				*/
/********************************************************************/
ev_tower_wayd_comanm_in:
#if 0
	//�G���x�[�^�h�Aopen
	_SET_UP_DOOR_ANIME	0,0,3,2,77
	_SET_UP_DOOR_ANIME	0,0,14,2,78
	_SEQ_OPEN_DOOR		77
	_SEQ_OPEN_DOOR		78
	_WAIT_3D_ANIME		77
	_WAIT_3D_ANIME		78
#endif

	/*�A�j������e�X�g*/
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_0, 0	//0�Ԃ̃A�j�����s
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_1, 0	//0�Ԃ̃A�j�����s
	_ARTICLE_ACTOR_ANIME_WAIT	ACTWORK_0		//�A�j���I���҂�
	_ARTICLE_ACTOR_ANIME_STOP	ACTWORK_0		//�A�j����~
	_ARTICLE_ACTOR_ANIME_STOP	ACTWORK_1		//�A�j����~

	_OBJ_ANIME			OBJID_PCWOMAN2_01,anm_d31r0204_pcwoman2_in
	_OBJ_ANIME			OBJID_PCWOMAN2_02,anm_d31r0204_pcwoman2_in
	_OBJ_ANIME_WAIT

	//_OBJ_VISIBLE		OBJID_PLAYER
	//_OBJ_VISIBLE		OBJID_MINE

	//_OBJ_ANIME			OBJID_PLAYER,anm_d31r0204_trainer_in
	_OBJ_ANIME			FSW_LOCAL2,anm_d31r0204_trainer_in
	_OBJ_ANIME			FSW_LOCAL3,anm_d31r0204_trainer_in
	_OBJ_ANIME_WAIT

#if 0
	_SEQ_CLOSE_DOOR		77	//�h�A�߂�
	_SEQ_CLOSE_DOOR		78	//�h�A�߂�
	_WAIT_3D_ANIME		77	//�A�j���I���҂�
	_WAIT_3D_ANIME		78	//�A�j���I���҂�
	_FREE_3D_ANIME		77	//�h�A�A�j�����	
	_FREE_3D_ANIME		78	//�h�A�A�j�����	
#endif
	_RET

ev_tower_wayd_comanm_walk:
	_OBJ_ANIME			OBJID_PCWOMAN2_01,anm_d31r0204_pcwoman2_right
	//_OBJ_ANIME			OBJID_PLAYER,anm_d31r0204_trainer_right
	_OBJ_ANIME			FSW_LOCAL2,anm_d31r0204_trainer_right
	_OBJ_ANIME			OBJID_PCWOMAN2_02,anm_d31r0204_pcwoman2_left
	_OBJ_ANIME			FSW_LOCAL3,anm_d31r0204_trainer_left
	_OBJ_ANIME_WAIT
	_RET

ev_tower_wayd_comanm_out:
	_OBJ_ANIME			OBJID_PCWOMAN2_01,anm_d31r0204_pcwoman2_out
	_OBJ_ANIME			OBJID_PCWOMAN2_02,anm_d31r0204_pcwoman2_out
	//_OBJ_ANIME			OBJID_PLAYER,anm_d31r0204_trainer_out
	_OBJ_ANIME			FSW_LOCAL2,anm_d31r0204_trainer_out
	_OBJ_ANIME			FSW_LOCAL3,anm_d31r0204_trainer_out
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*						OBJ_CHANGE_LABEL 							*/
/********************************************************************/
ev_d31r0204_obj_change:
	_IFVAL_CALL			FSW_LOCAL1,NE,0,ev_d31r0204_obj_player
	//_OBJ_INVISIBLE		OBJID_PLAYER
	//_OBJ_INVISIBLE		OBJID_MINE
	_RET

ev_d31r0204_obj_player:
	//_OBJ_INVISIBLE		OBJID_PLAYER
	//���}�X���炷
	//_PLAYER_POS_OFFSET_SET	8,0,0	
	_RET
	


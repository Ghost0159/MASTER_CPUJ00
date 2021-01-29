//==============================================================================
/**
 * @file	roulette.s
 * @brief	�u���[���b�g�v�t�����e�B�A�X�N���v�g
 * @author	nohara
 * @date	2007.09.05
 */
//==============================================================================

	.text

	.include	"../frontier_def.h"
	.include	"../roulette_def.h"
	.include	"../frontier_seq_def.h"
	.include	"../../../include/msgdata/msg_roulette_room.h"
	.include	"../../../include/field/evwkdef.h"
	.include	"../../fielddata/script/saveflag.h"
	.include	"../../fielddata/script/savework.h"
	.include	"../../field/syswork_def.h"
	.include	"../../particledata/pl_frontier/frontier_particle_def.h"	//SPA
	.include	"../../particledata/pl_frontier/frontier_particle_lst.h"	//EMIT

//--------------------------------------------------------------------
//					     �X�N���v�g�{��
//
//	FSW_PARAM0	�^�揈���Ŏg�p
//	FSW_PARAM1	���X�g�����Ɏg�p
//	FSW_PARAM2	�G�g���[�i�[1��OBJ�R�[�h
//	FSW_PARAM3	�G�g���[�i�[2��OBJ�R�[�h
//	FSW_PARAM4	1=�񕜃V�[�P���X���΂�(�L�^�������΂�)
//	FSW_PARAM5	�p�l�������Ŏg�p
//	FSW_PARAM6	�ē��l������
//	FSW_LOCAL2	�ʐM��`
//	FSW_LOCAL3	�L�^������
//	FSW_LOCAL5	�p�l�������Ŏg�p
//	FSW_LOCAL6	�p�l�������Ŏg�p
//	FSW_LOCAL7	��l���̌�����
//
//--------------------------------------------------------------------
_EVENT_DATA		fss_roulette_start	//��ԏ��EVENT_DATA�͎������s
_EVENT_DATA_END						//�I��

#define PARTICLE_PANEL_EFF_WAIT		(60)	//���o�p�[�e�B�N���̋��ʃE�F�C�g

//--------------------------------------------------------------------
//					     �f�o�b�N��`
//--------------------------------------------------------------------
//#define DEBUG_BTL_OFF				//�o�g���I�t�f�o�b�N
//#define DEBUG_BTL_LOSE_OFF			//�o�g���s�k�I�t�f�o�b�N
//#define DEBUG_7BTL_OFF				//�o�g��7��I�t�f�o�b�N

/********************************************************************/
//
//		SCENE_ROULETTE:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
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

//====================================================================
//	SCENE_ROULETTE:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
#define OBJID_PLAYER				(0)
#define OBJID_SIO_USER_0			(1)
#define OBJID_SIO_USER_1			(2)
#define OBJID_MAN					(3)
#define OBJID_MINE					(4)
#define OBJID_PEOPLE1				(5)
#define OBJID_PEOPLE2				(6)
#define OBJID_PEOPLE3				(7)
#define OBJID_PEOPLE4				(8)
#define OBJID_PEOPLE5				(9)
//#define OBJID_PEOPLE6				(10)
#define OBJID_PEOPLE7				(11)
#define OBJID_PEOPLE8				(12)
#define OBJID_PEOPLE11				(13)
#define OBJID_PEOPLE12				(14)
#define OBJID_PEOPLE13				(15)
#define OBJID_PEOPLE14				(16)
#define OBJID_PEOPLE15				(17)
#define OBJID_PEOPLE16				(18)
#define OBJID_PCWOMAN2				(20)
#define OBJID_KAIHUKU				(97)
#define OBJID_ETC					(98)
#define OBJID_ETC2					(99)

//�X�N���v�gID(����Ή��\��)
#define EVENTID_TEST_SCR_OBJ1		(1)

//�ʘH�A�N�^�[
_ACTOR_LABEL	way_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*16,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_ACTOR_DATA				OBJID_PCWOMAN2,FSW_PARAM6,WF2DMAP_WAY_C_UP, \
							8*23,8*24,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*23,8*24,OFF,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	way_set_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,8*16,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_UP,8*23,8*24,OFF
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_UP,8*23,8*24,OFF
	_ACTOR_DATA				OBJID_PCWOMAN2,FSW_PARAM6,WF2DMAP_WAY_C_UP, \
							8*23,8*24,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END


/********************************************************************/
//
//	SCENE_ROULETTE_BTL:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
_RESOURCE_LABEL	default_scene2_resource
	//_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g	//�ʘH���炠��̂ł���Ȃ�
	//_CHAR_RESOURCE_DATA		FSW_LOCAL7, WF2DC_C_MOVENORMAL			//���łɂ���̂ł���Ȃ�
	_CHAR_RESOURCE_DATA		SUNGLASSES,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	default_scene2_resource_multi
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	_CHAR_RESOURCE_DATA		FSW_LOCAL7, WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		SUNGLASSES,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	people_scene_resource
	_CHAR_RESOURCE_DATA		MAN1,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		BIGMAN,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		MOUNT,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		IDOL,WF2DC_C_MOVENORMAL
	//_CHAR_RESOURCE_DATA		CAMPBOY,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	pcwoman2_scene2_resource
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//====================================================================
//	SCENE_ROULETTE_BTL:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
#define PLAYER_Y	(8*38-4)	//(8*38-4)
#define MINE_Y		(8*38)		//(8*38-4)

_ACTOR_LABEL	default_scene2_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,PLAYER_Y,OFF	//�������g�̃A�N�^�Z�b�g
	_ACTOR_DATA				OBJID_MINE,FSW_LOCAL7,WF2DMAP_WAY_C_UP, \
							8*16,MINE_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_MAN,SUNGLASSES,WF2DMAP_WAY_C_UP, \
							8*16,8*14,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	default_scene2_actor_multi
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_UP,8*15,PLAYER_Y,OFF	//�������g�̃A�N�^�Z�b�g
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_UP,8*16,MINE_Y,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_UP,8*18,MINE_Y,ON
	_ACTOR_DATA				OBJID_MAN,SUNGLASSES,WF2DMAP_WAY_C_UP, \
							8*16,8*14,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	pcwoman2_scene2_actor
	_ACTOR_DATA				OBJID_PCWOMAN2,FSW_PARAM6,WF2DMAP_WAY_C_RIGHT, \
							8*14,MINE_Y,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

//--------------------------------------------------------------------
//	�ϋq�A�N�^�[
//--------------------------------------------------------------------
_ACTOR_LABEL	default_scene3_actor
	_ACTOR_DATA				OBJID_PEOPLE1,MAN1,WF2DMAP_WAY_C_RIGHT, \
							8*8,8*30,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE2,BIGMAN,WF2DMAP_WAY_C_LEFT, \
							8*10,8*30,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE3,IDOL,WF2DMAP_WAY_C_DOWN, \
							8*22,8*32,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE4,MOUNT,WF2DMAP_WAY_C_UP, \
							8*24,8*36,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE5,MOUNT,WF2DMAP_WAY_C_DOWN, \
							8*4,8*34,ON,EVENTID_TEST_SCR_OBJ1
	//_ACTOR_DATA				OBJID_PEOPLE6,BADMAN,WF2DMAP_WAY_C_DOWN, \
	//						8*4,8*32,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE7,IDOL,WF2DMAP_WAY_C_DOWN, \
							8*24,8*30,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE8,MAN1,WF2DMAP_WAY_C_DOWN, \
							8*26,8*30,ON,EVENTID_TEST_SCR_OBJ1
	/*�����L�����𕡐��z�u*/
	_ACTOR_DATA				OBJID_PEOPLE11,MAN1,WF2DMAP_WAY_C_RIGHT, \
							8*8,8*32,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE12,IDOL,WF2DMAP_WAY_C_LEFT, \
							8*10,8*36,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE13,MOUNT,WF2DMAP_WAY_C_DOWN, \
							8*22,8*30,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE14,BIGMAN,WF2DMAP_WAY_C_RIGHT, \
							8*28,8*36,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE15,MAN1,WF2DMAP_WAY_C_RIGHT, \
							8*18,8*34,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA				OBJID_PEOPLE16,MAN1,WF2DMAP_WAY_C_UP, \
							8*12,8*30,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

//--------------------------------------------------------------------
//	�󋵂ɂ���ĕς��郊�\�[�X
// LOCAL0 = OBJCODE
//--------------------------------------------------------------------
_RESOURCE_LABEL	etc_set_resource
	_CHAR_RESOURCE_DATA		FSW_PARAM2,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

_RESOURCE_LABEL	etc_set_resource2
	_CHAR_RESOURCE_DATA		FSW_PARAM2,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA		FSW_PARAM3,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//--------------------------------------------------------------------
//	�󋵂ɂ���ĕς���A�N�^�[
// LOCAL0 = OBJCODE
//--------------------------------------------------------------------
_ACTOR_LABEL	etc_set_actor
	//_ACTOR_DATA			OBJID_ETC,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
	//						8*30,8*18,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA			OBJID_ETC,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							8*16,8*26,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END

_ACTOR_LABEL	etc_set_actor2
	//_ACTOR_DATA			OBJID_ETC,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
	//						8*30,8*20,ON,EVENTID_TEST_SCR_OBJ1
	//_ACTOR_DATA			OBJID_ETC2,FSW_PARAM3,WF2DMAP_WAY_C_DOWN, \
	//						8*30,8*18,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA			OBJID_ETC,FSW_PARAM2,WF2DMAP_WAY_C_DOWN, \
							8*16,8*26,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA			OBJID_ETC2,FSW_PARAM3,WF2DMAP_WAY_C_DOWN, \
							8*18,8*26,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END


/********************************************************************/
//
//		�񕜂��Ă����X�^�b�t
//
/********************************************************************/
_RESOURCE_LABEL	kaihuku_set_resource
	_CHAR_RESOURCE_DATA		FSW_PARAM6,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END

//--------------------------------------------------------------------
//--------------------------------------------------------------------
_ACTOR_LABEL	kaihuku_set_actor
	_ACTOR_DATA			OBJID_KAIHUKU,FSW_PARAM6,WF2DMAP_WAY_C_UP, \
							8*6,8*28,ON,EVENTID_TEST_SCR_OBJ1
	_ACTOR_DATA_END


//====================================================================
//	�z�u���A�N�^�[
//====================================================================
//--------------------------------------------------------------
//	�����v�z�u�����\�[�X���x��
//--------------------------------------------------------------
_ARTICLE_RESOURCE_LABEL	article_ramp_resource
	_ARTICLE_RESOURCE_DATA	ACTID_ROULETTE_RAMP
	_ARTICLE_RESOURCE_DATA_END

//--------------------------------------------------------------
//	�����v�z�u���A�N�^�[
//--------------------------------------------------------------
#define RAMP_L_X			(24)
#define RAMP_R_X			(257)
#define RAMP_SINGLE_Y1		(56)

#define RAMP_WAIT			(45)

/*�V���O��L*/
_ARTICLE_ACTOR_LABEL	article_ramp_single_l_actor
	//id,x,y,visible,anm_start,act_work
	_ARTICLE_ACTOR_DATA		ACTID_ROULETTE_RAMP, RAMP_L_X, RAMP_SINGLE_Y1, ON, OFF, ACTWORK_0
	_ARTICLE_ACTOR_DATA_END

/*�V���O��R*/
_ARTICLE_ACTOR_LABEL	article_ramp_single_r_actor
	//id,x,y,visible,anm_start,act_work
	_ARTICLE_ACTOR_DATA		ACTID_ROULETTE_RAMP, RAMP_R_X, RAMP_SINGLE_Y1, ON, OFF, ACTWORK_1
	_ARTICLE_ACTOR_DATA_END

//--------------------------------------------------------------
//	�|�P�p�l���z�u�����\�[�X���x��
//--------------------------------------------------------------
_ARTICLE_RESOURCE_LABEL	article_pokepanel_resource
	_ARTICLE_RESOURCE_DATA	ACTID_ROULETTE_POKEPANEL
	_ARTICLE_RESOURCE_DATA_END

//--------------------------------------------------------------
//	�|�P�p�l���z�u���A�N�^�[
//--------------------------------------------------------------
#define POKEPANEL_L_X			(44)
#define POKEPANEL_R_X			(212)
#define POKEPANEL_SINGLE_Y1		(42)
#define POKEPANEL_SINGLE_Y2		(82)

/*�V���O��L*/
_ARTICLE_ACTOR_LABEL	article_pokepanel_single_l_actor
	//id,x,y,visible,anm_start,act_work
	_ARTICLE_ACTOR_DATA		ACTID_ROULETTE_POKEPANEL, POKEPANEL_L_X, POKEPANEL_SINGLE_Y1, ON, OFF, ACTWORK_4
	_ARTICLE_ACTOR_DATA		ACTID_ROULETTE_POKEPANEL, POKEPANEL_L_X, POKEPANEL_SINGLE_Y2, ON, OFF, ACTWORK_5
	_ARTICLE_ACTOR_DATA_END

/*�V���O��R*/
_ARTICLE_ACTOR_LABEL	article_pokepanel_single_r_actor
	//id,x,y,visible,anm_start,act_work
	_ARTICLE_ACTOR_DATA		ACTID_ROULETTE_POKEPANEL, POKEPANEL_R_X, POKEPANEL_SINGLE_Y1, ON, OFF, ACTWORK_6
	_ARTICLE_ACTOR_DATA		ACTID_ROULETTE_POKEPANEL, POKEPANEL_R_X, POKEPANEL_SINGLE_Y2, ON, OFF, ACTWORK_7
	_ARTICLE_ACTOR_DATA_END


/********************************************************************/
//
//					�A�j���[�V�����f�[�^
//
/********************************************************************/

//--------------------------------------------------------------------
//�ΐ핔���܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_top
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1		/*�\��*/
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_WALK_L_8F,7
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1		/*��\��*/
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_top_multi
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1		/*�\��*/
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_WALK_L_8F,7
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1		/*��\��*/
	_ANIME_DATA_END

_ANIME_LABEL anm_man_go_top
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_WALK_L_8F,7
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1		/*��\��*/
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�b������
//--------------------------------------------------------------------
_ANIME_LABEL anm_stay_walk_left
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�J�n�ʒu�܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL anm_player_go_center_scroll
	_ANIME_DATA	FC_WALK_U_8F,11
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center
	_ANIME_DATA	FC_WALK_U_8F,6
	_ANIME_DATA	FC_WALK_L_8F,4
	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center_multi
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_WALK_U_8F,6
	_ANIME_DATA	FC_WALK_L_8F,4
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//������
//--------------------------------------------------------------------
_ANIME_LABEL anm_pcwoman2_go_center
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_STAY_WALK_U_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�g���[�i�[���o�g�����[���̒����܂ňړ�
//--------------------------------------------------------------------
_ANIME_LABEL etc_anime_room_in_single
	//_ANIME_DATA	FC_WALK_L_8F,3
	//_ANIME_DATA_END
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_R_8F,4
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in
	//_ANIME_DATA	FC_WALK_L_8F,3
	//_ANIME_DATA_END
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_R_8F,4
	_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_multi
	//_ANIME_DATA	FC_WALK_L_8F,3
	//_ANIME_DATA_END
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_R_8F,3
	_ANIME_DATA	FC_WALK_U_8F,3
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_go_btl_walk_left
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_go_btl_walk_right
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain
	//_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1		/*��\��*/
	//_ANIME_DATA	FC_WALK_L_8F,7
	//_ANIME_DATA	FC_WALK_D_8F,7
	//_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1		/*�\��*/
	//_ANIME_DATA_END
	_ANIME_DATA	FC_SYSCMD_VISIBLE_OFF,1		/*��\��*/
	_ANIME_DATA	FC_WALK_D_8F,3
	_ANIME_DATA	FC_SYSCMD_VISIBLE_ON,1		/*�\��*/
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain_12
	_ANIME_DATA	FC_STAY_WALK_R_4F,1
	_ANIME_DATA	FC_STAY_WALK_U_2F,1
	_ANIME_DATA	FC_STAY_WALK_L_2F,1
	_ANIME_DATA	FC_STAY_WALK_D_2F,1
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_room_in_brain_2
	_ANIME_DATA	FC_STAY_WALK_R_4F,1
	_ANIME_DATA	FC_STAY_WALK_U_2F,1
	_ANIME_DATA	FC_STAY_WALK_L_2F,1
	_ANIME_DATA	FC_STAY_WALK_D_2F,1
	_ANIME_DATA	FC_STAY_WALK_R_2F,1
	_ANIME_DATA	FC_WALK_U_8F,3

	_ANIME_DATA	FC_STAY_WALK_L_4F,1
	_ANIME_DATA	FC_STAY_WALK_D_2F,1
	_ANIME_DATA	FC_STAY_WALK_R_2F,1
	_ANIME_DATA	FC_STAY_WALK_U_2F,1
	_ANIME_DATA	FC_STAY_WALK_L_2F,1
	_ANIME_DATA	FC_STAY_WALK_D_2F,1
	_ANIME_DATA	FC_STAY_WALK_R_2F,1
	_ANIME_DATA	FC_STAY_WALK_U_2F,1
	_ANIME_DATA	FC_STAY_WALK_L_2F,1
	_ANIME_DATA	FC_STAY_WALK_D_2F,1
	_ANIME_DATA	FC_WALK_R_8F,4

	_ANIME_DATA	FC_STAY_WALK_U_4F,1
	_ANIME_DATA	FC_STAY_WALK_L_2F,1
	_ANIME_DATA	FC_STAY_WALK_D_2F,1
	_ANIME_DATA	FC_STAY_WALK_R_2F,1

	_ANIME_DATA	FC_WALK_U_8F,4
	_ANIME_DATA	FC_STAY_WALK_L_4F,1
	_ANIME_DATA	FC_STAY_WALK_D_2F,1
	_ANIME_DATA	FC_STAY_WALK_R_2F,1
	_ANIME_DATA	FC_STAY_WALK_U_2F,1
	_ANIME_DATA	FC_STAY_WALK_L_2F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center_scroll_brain
	_ANIME_DATA	FC_WALK_D_8F,7
	_ANIME_DATA_END

_ANIME_LABEL anm_player_go_center_scroll_brain2
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WALK_U_8F,7
	_ANIME_DATA_END

//--------------------------------------------------------------------
//��l���@�u���[��������
//--------------------------------------------------------------------
_ANIME_LABEL anm_mine_brain_down
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_mine_brain_down2
	_ANIME_DATA	FC_WAIT_32F,1
	_ANIME_DATA	FC_WAIT_32F,1
	_ANIME_DATA	FC_WAIT_32F,1
	_ANIME_DATA	FC_WAIT_16F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�g���[�i�[���o�g�����[������o�čs��
//--------------------------------------------------------------------
_ANIME_LABEL etc_anime_go_out
	//_ANIME_DATA	FC_WALK_R_8F,8
	//_ANIME_DATA_END
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA	FC_WALK_D_8F,8
	_ANIME_DATA_END

_ANIME_LABEL etc_anime_go_out_multi
	//_ANIME_DATA	FC_WALK_R_8F,8
	//_ANIME_DATA_END
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA	FC_WALK_D_8F,8
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�G�g���[�i�[���o�g�����[������o�čs��(�퓬���Ȃ���)
//--------------------------------------------------------------------
_ANIME_LABEL etc_anime_go_out_btl_off
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA	FC_WALK_D_8F,8
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�v���C���[�����������
//--------------------------------------------------------------------
_ANIME_LABEL mine_anime_go_out
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�p�l��������
//--------------------------------------------------------------------
_ANIME_LABEL anm_panel_see_01
	_ANIME_DATA	FC_STAY_WALK_U_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_panel_see_02
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL anm_panel_see_03
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�ϋq	���S������
//--------------------------------------------------------------------
_ANIME_LABEL people_anime_11
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_12
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_13
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_14
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_15
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_16
	_ANIME_DATA	FC_DIR_R,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_17
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_18
	_ANIME_DATA	FC_DIR_L,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_18_2
	_ANIME_DATA	FC_DIR_U,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�ϋq���W�܂�
//--------------------------------------------------------------------
_ANIME_LABEL people_anime_01
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA_END

_ANIME_LABEL people_anime_02
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_03
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA_END

_ANIME_LABEL people_anime_04
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA_END

_ANIME_LABEL people_anime_05
	_ANIME_DATA	FC_WALK_R_8F,4
	_ANIME_DATA_END

_ANIME_LABEL people_anime_06
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_07
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA_END

_ANIME_LABEL people_anime_08
	_ANIME_DATA	FC_WALK_L_8F,2
	_ANIME_DATA_END

_ANIME_LABEL people_anime_02_2
	_ANIME_DATA	FC_WALK_R_8F,2
	_ANIME_DATA	FC_STAY_WALK_U_8F,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_15_5
	_ANIME_DATA	FC_WALK_L_8F,1
	_ANIME_DATA	FC_STAY_WALK_U_8F,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_16_5
	_ANIME_DATA	FC_WALK_R_8F,1
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�ϋq	�������
//--------------------------------------------------------------------
_ANIME_LABEL people_anime_up_01
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_STAY_WALK_U_8F,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_up_02
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_4F,1
	_ANIME_DATA	FC_STAY_WALK_U_8F,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_up_03
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_STAY_WALK_U_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�ϋq	���ꎞ�̓���
//--------------------------------------------------------------------
_ANIME_LABEL people_anime_40
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_41
	_ANIME_DATA	FC_STAY_WALK_D_16F,1
	_ANIME_DATA	FC_STAY_WALK_D_16F,1
	_ANIME_DATA	FC_STAY_WALK_U_16F,1
	_ANIME_DATA	FC_STAY_WALK_U_16F,1
	_ANIME_DATA	FC_STAY_WALK_D_16F,1
	_ANIME_DATA	FC_STAY_WALK_D_16F,1
	_ANIME_DATA_END

_ANIME_LABEL people_anime_42
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�i��	����ł̓��[���b�g�����Ă݂�[
//--------------------------------------------------------------------
_ANIME_LABEL pcwoman2_anime_50
#if 0
	_ANIME_DATA	FC_STAY_WALK_L_4F,1
	_ANIME_DATA	FC_STAY_WALK_U_2F,1
	_ANIME_DATA	FC_STAY_WALK_R_2F,1
	_ANIME_DATA	FC_STAY_WALK_D_2F,1
#else
	_ANIME_DATA	FC_WALK_D_8F,1
#endif
	_ANIME_DATA_END

/*���̈ʒu�ɖ߂�*/
_ANIME_LABEL pcwoman2_anime_55
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END

_ANIME_LABEL pcwoman2_anime_57
#if 0
	_ANIME_DATA	FC_STAY_WALK_L_4F,1
	_ANIME_DATA	FC_STAY_WALK_U_4F,1
	_ANIME_DATA	FC_STAY_WALK_R_2F,1
	_ANIME_DATA	FC_STAY_WALK_D_4F,1
#else
	_ANIME_DATA	FC_STAY_WALK_D_2F,1
	_ANIME_DATA	FC_STAY_WALK_D_4F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
#endif
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�i��	�E������
//--------------------------------------------------------------------
_ANIME_LABEL pcwoman2_anime_80
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�i��	��������
//--------------------------------------------------------------------
_ANIME_LABEL pcwoman2_anime_90
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�i��	�b��
//--------------------------------------------------------------------
_ANIME_LABEL pcwoman2_anime_60
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END

_ANIME_LABEL pcwoman2_anime_65
	//_ANIME_DATA	FC_STAY_WALK_U_8F,1
	_ANIME_DATA	FC_WAIT_8F,1
	//_ANIME_DATA	FC_STAY_WALK_L_4F,1
	_ANIME_DATA	FC_DIR_L,1
	//_ANIME_DATA	FC_STAY_WALK_D_4F,1
	_ANIME_DATA	FC_DIR_D,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�i��	���̂݁A�A�C�e����n��
//--------------------------------------------------------------------
_ANIME_LABEL pcwoman2_anime_70
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_L_8F,3
	_ANIME_DATA_END

_ANIME_LABEL pcwoman2_anime_72
	_ANIME_DATA	FC_WALK_D_8F,1
	_ANIME_DATA	FC_WALK_R_8F,3
	_ANIME_DATA_END

//--------------------------------------------------------------------
//�i��	���̂݁A�A�C�e����n���Ė߂�
//--------------------------------------------------------------------
_ANIME_LABEL pcwoman2_anime_75				/*BP*/
	_ANIME_DATA	FC_WALK_R_8F,3
	//_ANIME_DATA	FC_WALK_U_8F,1
	//_ANIME_DATA	FC_WALK_U_8F,2
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END

_ANIME_LABEL pcwoman2_anime_75_item			/*�A�C�e���A���̂�*/
	_ANIME_DATA	FC_WALK_R_8F,3
	//_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END

_ANIME_LABEL pcwoman2_anime_77
	_ANIME_DATA	FC_WALK_L_8F,3
	//_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_WALK_U_8F,1
	_ANIME_DATA	FC_STAY_WALK_D_8F,1
	_ANIME_DATA_END

//--------------------------------------------------------------------
//��	�߂Â��Ă���
//--------------------------------------------------------------------
_ANIME_LABEL kaihuku_anime_10
	_ANIME_DATA	FC_WALK_U_4F,5
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END

_ANIME_LABEL kaihuku_anime_15
	_ANIME_DATA	FC_WALK_D_4F,6
	_ANIME_DATA_END

//--------------------------------------------------------------------
//��l��	�񕜂�����
//--------------------------------------------------------------------
_ANIME_LABEL player_kaihuku_anime_10
	_ANIME_DATA	FC_STAY_WALK_L_8F,1
	_ANIME_DATA_END

_ANIME_LABEL player_kaihuku_anime_15
	_ANIME_DATA	FC_STAY_WALK_R_8F,1
	_ANIME_DATA_END


/********************************************************************/
//
//							�ʘH
//
/********************************************************************/
fss_roulette_start:
	//_EVENT_START

	/*lobby�őI�������^�C�v�ƃ��x�����K�v*/
	_SAVE_EVENT_WORK_GET	WK_SCENE_ROULETTE_TYPE,FSW_LOCAL1	/*type*/
	_SAVE_EVENT_WORK_GET	LOCALWORK3,FSW_LOCAL3				/*init*/
	_SAVE_EVENT_WORK_GET	LOCALWORK2,FSW_LOCAL4				/*pos1*/
	_SAVE_EVENT_WORK_GET	LOCALWORK5,FSW_LOCAL5				/*pos2*/
	_SAVE_EVENT_WORK_GET	LOCALWORK6,FSW_LOCAL6				/*pos3*/

	/*���[�N�m��*/
	_BATTLE_REC_INIT
	_ROULETTE_WORK_ALLOC	FSW_LOCAL3,FSW_LOCAL1,FSW_LOCAL4,FSW_LOCAL5,FSW_LOCAL6,FSW_ANSWER

	/*��l���̌����ڊi�[*/
	_GET_MINE_OBJ		FSW_LOCAL7

	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_start_multi_way

	_LDVAL				FSW_PARAM6,BFSM
	_CALL				ev_roulette_actor_set_way
	_JUMP				ev_roulette_start_data_set_way
	_END

ev_roulette_start_multi_way:
	_CALL				ev_roulette_comm_command_initialize_1

	/*���s�Ɋ֌W�Ȃ��ʐM�t�����e�B�A�ɒ���(�ʐM�͒��f���Ȃ��̂Ő擪�ŏ������Ă����v)*/
	_SCORE_ADD			SCORE_ID_FRONTIER_COMM

	_LDVAL				FSW_PARAM6,BFSW1
	_CALL				ev_roulette_actor_set_way_multi
	_JUMP				ev_roulette_start_data_set_way
	_END

ev_roulette_start_data_set_way:
	_CALL				ev_roulette_rouka_pal_black							/*�Â�����*/

	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_go_btl_multi_way

	/*�ΐ핔���܂ŕ���*/
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_man_go_top
	_OBJ_ANIME			OBJID_MINE,anm_player_go_top
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
	_JUMP				ev_roulette_go_btl_sub
	_END

ev_roulette_go_btl_multi_way:
	/*�ΐ핔���܂ŕ���*/
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_man_go_top
	_OBJ_ANIME			OBJID_SIO_USER_0,anm_player_go_top
	_OBJ_ANIME			OBJID_SIO_USER_1,anm_player_go_top_multi
	_OBJ_ANIME_WAIT

	/*�ΐ핔���փ}�b�v�؂�ւ�*/
	_SE_PLAY			SEQ_SE_DP_KAIDAN2
	_SE_WAIT			SEQ_SE_DP_KAIDAN2
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_ACTOR_FREE			OBJID_SIO_USER_0
	_ACTOR_FREE			OBJID_SIO_USER_1
	_ACTOR_FREE			OBJID_PCWOMAN2
	_CHAR_RESOURCE_FREE	FSW_PARAM6
	_JUMP				ev_roulette_go_btl_sub
	_END


/********************************************************************/
//
//							�ΐ핔��
//
/********************************************************************/
ev_roulette_go_btl_sub:
	_MAP_CHANGE			FSS_SCENEID_ROULETTE_BTL

	_LDVAL				FSW_PARAM4,0					/*�񕜃V�[�P���X���f���[�N�N���A*/

	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_start_multi

	_LDVAL				FSW_PARAM6,BFSM
	_CALL				ev_roulette_actor_set_btl
	_JUMP				ev_roulette_start_data_set
	_END

ev_roulette_start_multi:
	_ROULETTE_TOOL		FR_ID_SET_MAIN_BG,0,0,FSW_ANSWER/*�w�i�ύX(�����ŒʐM�^�C�v���`�F�b�N)*/

	_LDVAL				FSW_PARAM6,BFSW1
	_CALL				ev_roulette_actor_set_btl_multi
	_JUMP				ev_roulette_start_data_set
	_END


/********************************************************************/
//
/********************************************************************/
ev_roulette_start_data_set:
	_TIME_WAIT			1,FSW_ANSWER

	_CALL				ev_roulette_actor_set_btl_pcwoman
	_CALL				ev_roulette_actor_set_btl_people

	/*�|�P�p�l�����\�[�X�{�A�N�^�[�Z�b�g*/
	_CALL				ev_pokepanel_set

	_CALL				ev_roulette_btl_pal_black				/*�Â�����*/

	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�ʐM�F��{�����Ƃ�*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_multi_comm_basic

	_JUMP				ev_roulette_start_2
	_END

/*�ʐM�F��{�����Ƃ�*/
ev_roulette_multi_comm_basic:
	/*�ʘH�ɓ����Ă����Ɉړ�*/
	//_CALL				ev_roulette_comm_command_initialize_1

//	_JUMP				ev_roulette_multi_comm_basic_retry
//	_END

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_BASIC
	_COMM_RESET

	_CALL				ev_roulette_multi_comm_basic_call
	_JUMP				ev_roulette_start_2
	_END

ev_roulette_multi_comm_basic_retry:
	_LDVAL				FSW_LOCAL2,ROULETTE_COMM_BASIC
	_ROULETTE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_multi_comm_basic_retry	/*���M���s�́A�đ��M��*/

	_ROULETTE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET
	_JUMP				ev_roulette_start_2
	_END


/********************************************************************/
//
/********************************************************************/
/*�ʐM�F��{�����Ƃ�*/
ev_roulette_multi_comm_basic_call:
	_LDVAL				FSW_LOCAL2,ROULETTE_COMM_BASIC
	_ROULETTE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_multi_comm_basic_call	/*���M���s�́A�đ��M��*/

	_ROULETTE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET
	_RET


/********************************************************************/
//
/********************************************************************/
ev_roulette_start_2:
	/*���[�N������*/
	_ROULETTE_WORK_INIT	FSW_LOCAL3

	/*�ʐM�F�g���[�i�[�����Ƃ�*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_multi_comm_tr_2

	_JUMP				ev_roulette_start_3
	_END

ev_roulette_multi_comm_tr_2:

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_TR
	_COMM_RESET

	_JUMP				ev_roulette_multi_comm_tr
	_END

/*�ʐM�F�g���[�i�[�����Ƃ�*/
ev_roulette_multi_comm_tr:
	_LDVAL				FSW_LOCAL2,ROULETTE_COMM_TR
	_ROULETTE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_multi_comm_tr	/*���M���s�́A�đ��M��*/

	_ROULETTE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_POKE_DATA
	_COMM_RESET

	_COMM_SET_WIFI_BOTH_NET	1										/*��ʃf�[�^ON*/
	_JUMP				ev_roulette_multi_comm_poke
	_END

ev_roulette_multi_comm_poke:
	/*�ʐM�F�莝�����Ƃ�*/
	_LDVAL				FSW_LOCAL2,ROULETTE_COMM_TEMOTI
	_ROULETTE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_multi_comm_poke	/*���M���s�́A�đ��M��*/

	_ROULETTE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET
	
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_BOTH_0
	_COMM_RESET

	_COMM_SET_WIFI_BOTH_NET	0										/*��ʃf�[�^OFF*/
	_JUMP				ev_roulette_start_3
	_END


/********************************************************************/
//
/********************************************************************/
ev_roulette_start_3:
	/*�u���낭����v�Ŏg�p���郏�[�N�Ȃ̂ŃN���A���Ă���*/
	_LDVAL				FSW_LOCAL3,0

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_start_3_multi

	/*�J�n�ʒu�܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center_scroll
	_OBJ_ANIME			OBJID_MINE,anm_player_go_center
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_pcwoman2_go_center
	_OBJ_ANIME			OBJID_PEOPLE2,people_anime_40			/*bigman �㉺�Ɉړ��A���E����*/
	_OBJ_ANIME			OBJID_PEOPLE5,people_anime_41			/*mount ���̏����*/
	_OBJ_ANIME			OBJID_PEOPLE7,people_anime_42			/*idol ���E�ɃL�����L����*/
	_OBJ_ANIME_WAIT

	_JUMP				ev_roulette_room_start
	_END

ev_roulette_start_3_multi:
	/*�J�n�ʒu�܂ŕ���*/
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center_scroll
	_OBJ_ANIME			OBJID_SIO_USER_0,anm_player_go_center
	_OBJ_ANIME			OBJID_SIO_USER_1,anm_player_go_center_multi
	_OBJ_ANIME			OBJID_PCWOMAN2,anm_pcwoman2_go_center
	_OBJ_ANIME			OBJID_PEOPLE2,people_anime_40			/*bigman �㉺�Ɉړ��A���E����*/
	_OBJ_ANIME			OBJID_PEOPLE5,people_anime_41			/*mount ���̏����*/
	_OBJ_ANIME			OBJID_PEOPLE7,people_anime_42			/*idol ���E�ɃL�����L����*/
	_OBJ_ANIME_WAIT
	_JUMP				ev_roulette_room_start
	_END


/********************************************************************/
/*																	*/
/********************************************************************/
ev_roulette_room_start:
	
	/*�U�����폜*/
	_ACTOR_FREE			OBJID_PCWOMAN2
	_CHAR_RESOURCE_FREE	FSW_PARAM6

	/*�ϋq�폜*/
	_CALL				ev_roulette_people_del

	/*�����l�ڂ��擾*/
	_ROULETTE_TOOL		FR_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_room_renshou_0		/*0	*/

	_CALL				ev_roulette_room_set_sub

	/*�ĊJ�������Ɏi��̌��������ɂ���*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_65
	_OBJ_ANIME_WAIT

	/*�L�^�������[�N�Ƀf�[�^���������܂�Ă��Ȃ��̂Ń��j���[�\�����Ȃ�*/
	_LDVAL				FSW_LOCAL3,1
	_JUMP				ev_roulette_room_saikai							/*1-7*/
	_END


/********************************************************************/
/*							0�l��									*/
/********************************************************************/
ev_roulette_room_renshou_0:
	_CALL			ev_roulette_room_set_sub
	_JUMP			ev_roulette_room_go_rental
	_END

ev_roulette_room_set_sub:
	_TIME_WAIT		1,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL	FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_roulette_multi_room_enemy_poke_send2

	/*(�V���O���A�}���`�e�p)�G��POKEPARTY�Z�b�g*/
	_ROULETTE_BTL_BEFORE_PARTY_SET

	_CALL			ev_roulette_poke_itemkeep_icon_add_mine			/*(MINE)�|�P�����A�C�R���ǉ�*/
	_RET


/********************************************************************/
/*					�G�|�P�����f�[�^��ʐM							*/
/********************************************************************/
ev_roulette_multi_room_enemy_poke_send2:

	/*recieve_count���N���A���Ă���(08.06.18 �ی�����)*/
	/*������ɏ�������ƁA�Е�����ɐi��Ŏ�M���������ƂɁA�N���A�����邩���m��Ȃ��̂Ń_���I*/
	_ROULETTE_TOOL	FR_ID_RECIEVE_COUNT_CLEAR,0,0,FSW_ANSWER

	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_ENEMY_BEFORE
	_COMM_RESET	

	_JUMP			ev_roulette_multi_room_enemy_poke_send2_retry
	_END

ev_roulette_multi_room_enemy_poke_send2_retry:
	/*�ʐM�F�G�|�P�������Ƃ�*/
	_LDVAL				FSW_LOCAL2,ROULETTE_COMM_ENEMY
	_ROULETTE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_multi_room_enemy_poke_send2_retry/*���s�đ��M*/

	_ROULETTE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_ENEMY_AFTER
	_COMM_RESET	

	/*�G�|�P�����𐶐�*/
	_ROULETTE_TOOL		FR_ID_ENEMY_POKE_CREATE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_ENEMY_POKE_CREATE_2,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER
	_RET


/********************************************************************/
/*																	*/
/********************************************************************/
ev_roulette_room_go_rental:
	/*�u�悤�����o�g�����[���b�g�ցI�v*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_65
	_OBJ_ANIME_WAIT
	_TALKMSG			msg_roulette_room_20
	_SE_PLAY			SEQ_SE_DP_DENDOU				/*����*/

	/*1�l�ڂ̎��́A����a���胁�b�Z�[�W�\��*/
	_ROULETTE_TOOL		FR_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_roulette_room_first_msg

//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]�f�o�b�N]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#if 0

/*
	_PARTICLE_SPA_LOAD	SPAWORK_0, ROULETTE_SPA, FSS_CAMERA_ORTHO

	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_KEMURI01_L
	_TIME_WAIT			120,FSW_ANSWER

	_PARTICLE_SPA_EXIT	SPAWORK_0
	_TIME_WAIT			120,FSW_ANSWER
*/

//////////////////////////////////////////////////////////////////////

	_LDVAL				FSW_LOCAL5,0
	_LDVAL				FSW_LOCAL6,0
	_CALL				debug_particle_test		//0
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//5
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//10
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//15
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//20
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//25
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//30
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//35
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//40
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//45
	_CALL				debug_particle_test
	_CALL				debug_particle_test		//47

	_LDVAL				FSW_LOCAL5,0
	_LDVAL				FSW_LOCAL6,0
#endif
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_multi_room_go_rental

	_JUMP				ev_roulette_room_go_rental_2
	_END

ev_roulette_multi_room_go_rental:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_roulette_room_wait
	_JUMP				ev_roulette_multi_room_go_rental_retry
	_END

ev_roulette_multi_room_go_rental_retry:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_RENTAL
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/
	_JUMP				ev_roulette_room_go_rental_2
	_END

/*�f�o�b�N(FSW_LOCAL5,6�g�p)*/
debug_particle_test:
	_PARTICLE_SPA_LOAD	SPAWORK_0, ROULETTE_SPA, FSS_CAMERA_ORTHO

	_ROULETTE_TOOL		FR_ID_ADD_EMITTER,FSW_LOCAL5,FSW_LOCAL6,FSW_ANSWER
	_TIME_WAIT			120,FSW_ANSWER

	_PARTICLE_SPA_EXIT	SPAWORK_0
	_TIME_WAIT			60,FSW_ANSWER
	_ADD_WK				FSW_LOCAL5,1		/*emit_no�C���N�������g*/
	_RET

/*1�l�ڂ̎��́A����a���胁�b�Z�[�W�\��*/
ev_roulette_room_first_msg:
	_TALKMSG			msg_roulette_room_1
	_RET


/********************************************************************/
/*		�u����ڂ̃Q�[���ł��I�����I�`�������W���[�͒N���I�v		*/
/********************************************************************/
ev_roulette_room_go_rental_2:
	/*�A�����擾*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_msg_01_single
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_msg_01_multi
	_END

ev_roulette_msg_01_single:

	_ROULETTE_TOOL		FR_ID_GET_ROUND,0,0,FSW_ANSWER
	_ADD_WK				FSW_ANSWER,1
	_NUMBER_NAME		0,FSW_ANSWER
	_TALKMSG			msg_roulette_room_21
	_AB_KEYWAIT
	_TALK_CLOSE
	_CALL				ev_room_choice_24				/*�g���[�i�[�o��*/

	/*�����O�f��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_50
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_21_01
	_AB_KEYWAIT

	_JUMP				ev_roulette_rank_go
	_END

ev_roulette_msg_01_multi:
	_ROULETTE_TOOL		FR_ID_GET_ROUND,0,0,FSW_ANSWER
	_ADD_WK				FSW_ANSWER,1
	_NUMBER_NAME		0,FSW_ANSWER
	_TALKMSG_NOSKIP		msg_roulette_room_21
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_TALK_CLOSE
	_CALL				ev_room_choice_24				/*�g���[�i�[�o��*/

	/*�����O�f��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_50
	_OBJ_ANIME_WAIT

	_TALKMSG_NOSKIP		msg_roulette_room_21_01
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_CALL
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/
	_JUMP				ev_roulette_rank_go
	_END


/********************************************************************/
/*						���[���b�g��ʂ�							*/
/********************************************************************/
ev_roulette_rank_go:
	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_TALK_CLOSE

	/*�u���[���o�ꂩ�`�F�b�N*/
	_ROULETTE_TOOL		FR_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_roulette_leader_1st_12
	_IFVAL_CALL			FSW_ANSWER,EQ,2,ev_roulette_leader_2nd_12

	/*�A�������擾(�u���[���`�F�b�N�p)(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get

	/*�i������̈ʒu�ɖ߂��Ă���*/
	//_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_55
	//_OBJ_ANIME_WAIT

	_CALL				ev_roulette_poke_itemkeep_icon_del		/*�|�P�����A�C�R���폜*/
	_CALL				ev_roulette_poke_itemkeep_icon_resource_del	/*���\�[�X�폜*/

	/*�|�P�p�l�����\�[�X�{�A�N�^�[�폜*/
	_CALL				ev_pokepanel_del

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_roulette_rank_go_multi

	_ROULETTE_CALL
	//_SET_MAP_PROC

	/*�����v���\�[�X*/
	_ARTICLE_RESOURCE_SET	article_ramp_resource

	/*�|�P�p�l�����\�[�X�{�A�N�^�[�Z�b�g*/
	_CALL					ev_pokepanel_set

	/*�ʐM�}���`*/
	//_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL5
	//_IFVAL_CALL			FSW_LOCAL5,EQ,0,ev_roulette_single_ramp_set
	//_IFVAL_CALL			FSW_LOCAL5,EQ,1,ev_roulette_multi_ramp_set

	_CALL				ev_roulette_poke_itemkeep_icon_add		/*�|�P�����A�C�R���ǉ�*/

	_ROULETTE_TOOL		FR_ID_SET_PANEL_BG,0,0,FSW_ANSWER		/*�p�l���ύX*/

	/*���肵���C�x���g�p�l�������s*/
	_ROULETTE_DECIDE_EV_NO_FUNC

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_roulette_comm_command_initialize_2

	/*�ʐM�F��{�����Ƃ�*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_roulette_multi_comm_basic_call

	_CALL				ev_roulette_btl_pal_black				/*BG�Â�����*/
	_CALL				ev_roulette_obj_pal_black				/*OBJ�Â�����*/

	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_JUMP				ev_room_choice_23
	_END

/*�p�[�e�B�N���폜(�u���[����p)*/
ev_roulette_leader_1st_12:
ev_roulette_leader_2nd_12:
	_PARTICLE_SPA_EXIT	SPAWORK_3
	_RET

/*����*/
ev_roulette_rank_go_multi:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_CALL_2
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/
	_RET


/********************************************************************/
/*						�����v�A�N�^�[�o�^							*/
/********************************************************************/
ev_roulette_single_l_ramp_actor_set:
	_ARTICLE_ACTOR_SET		article_ramp_single_l_actor
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_0,0		//0�Ԃ̃A�j�����s
	//_ARTICLE_ACTOR_ANIME_STOP	ACTWORK_0		//�A�j����~
	_RET

ev_roulette_single_r_ramp_actor_set:
	_ARTICLE_ACTOR_SET		article_ramp_single_r_actor
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_1,1		//1�Ԃ̃A�j�����s
	//_ARTICLE_ACTOR_ANIME_STOP	ACTWORK_1		//�A�j����~
	_RET


/********************************************************************/
/*						�����v�A�N�^�[�폜							*/
/********************************************************************/
ev_roulette_single_l_ramp_actor_free:
	_ARTICLE_ACTOR_FREE		ACTWORK_0
	_RET

ev_roulette_single_r_ramp_actor_free:
	_ARTICLE_ACTOR_FREE		ACTWORK_1
	_RET


/********************************************************************/
/*					���肵���C�x���g���o							*/
/********************************************************************/
ev_room_choice_23:
	_PARTICLE_SPA_LOAD	SPAWORK_0, ROULETTE_SPA, FSS_CAMERA_ORTHO

	/*���肵���C�x���g�i���o�[���擾*/
	_ROULETTE_TOOL		FR_ID_GET_DECIDE_EV_NO,0,0,FSW_ANSWER

	_SE_PLAY			SEQ_SE_DP_DENDOU								/*����*/

	_SWITCH				FSW_ANSWER
	_CASE_JUMP			ROULETTE_EV_ENEMY_HP_DOWN,ev_roulette_seq00		//�G
	_CASE_JUMP			ROULETTE_EV_ENEMY_DOKU,ev_roulette_seq01
	_CASE_JUMP			ROULETTE_EV_ENEMY_MAHI,ev_roulette_seq02
	_CASE_JUMP			ROULETTE_EV_ENEMY_YAKEDO,ev_roulette_seq03
	_CASE_JUMP			ROULETTE_EV_ENEMY_NEMURI,ev_roulette_seq04
	_CASE_JUMP			ROULETTE_EV_ENEMY_KOORI,ev_roulette_seq05
	_CASE_JUMP			ROULETTE_EV_ENEMY_SEED_GET,ev_roulette_seq06
	_CASE_JUMP			ROULETTE_EV_ENEMY_ITEM_GET,ev_roulette_seq07
	_CASE_JUMP			ROULETTE_EV_ENEMY_LV_UP,ev_roulette_seq08
	_CASE_JUMP			ROULETTE_EV_MINE_HP_DOWN,ev_roulette_seq09		//����
	_CASE_JUMP			ROULETTE_EV_MINE_DOKU,ev_roulette_seq10
	_CASE_JUMP			ROULETTE_EV_MINE_MAHI,ev_roulette_seq11
	_CASE_JUMP			ROULETTE_EV_MINE_YAKEDO,ev_roulette_seq12
	_CASE_JUMP			ROULETTE_EV_MINE_NEMURI,ev_roulette_seq13
	_CASE_JUMP			ROULETTE_EV_MINE_KOORI,ev_roulette_seq14
	_CASE_JUMP			ROULETTE_EV_MINE_SEED_GET,ev_roulette_seq15
	_CASE_JUMP			ROULETTE_EV_MINE_ITEM_GET,ev_roulette_seq16
	_CASE_JUMP			ROULETTE_EV_MINE_LV_UP,ev_roulette_seq17
	_CASE_JUMP			ROULETTE_EV_PLACE_HARE,ev_roulette_seq18		//��ɋN����
	_CASE_JUMP			ROULETTE_EV_PLACE_AME,ev_roulette_seq19
	_CASE_JUMP			ROULETTE_EV_PLACE_SUNAARASI,ev_roulette_seq20
	_CASE_JUMP			ROULETTE_EV_PLACE_ARARE,ev_roulette_seq21
	_CASE_JUMP			ROULETTE_EV_PLACE_KIRI,ev_roulette_seq22
	_CASE_JUMP			ROULETTE_EV_PLACE_TRICK,ev_roulette_seq23
	_CASE_JUMP			ROULETTE_EV_PLACE_SPEED_UP,ev_roulette_seq25
	_CASE_JUMP			ROULETTE_EV_PLACE_SPEED_DOWN,ev_roulette_seq26
	_CASE_JUMP			ROULETTE_EV_PLACE_RANDOM,ev_roulette_seq27
	_CASE_JUMP			ROULETTE_EV_EX_POKE_CHANGE,ev_roulette_seq24	//����
	_CASE_JUMP			ROULETTE_EV_EX_BP_GET,ev_roulette_seq28
	_CASE_JUMP			ROULETTE_EV_EX_BTL_WIN,ev_roulette_seq29
	_CASE_JUMP			ROULETTE_EV_EX_SUKA	,ev_roulette_seq30
	_CASE_JUMP			ROULETTE_EV_EX_BP_GET_BIG,ev_roulette_seq31
	_END


/********************************************************************/
/*							�GHP�C�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq00:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i��E������*/
	_CALL				ev_roulette_man_right

	_CALL				se_ramp_enemy

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_JUMP			FSW_LOCAL6,EQ,0,ev_roulette_seq00_single_02
	_IFVAL_JUMP			FSW_LOCAL6,EQ,1,ev_roulette_seq00_multi_02
	_END

ev_roulette_seq00_sub:
	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_CALL				se_spot_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq00_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq00_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq00_single_02:
	_LDVAL				FSW_LOCAL5,ROULETTE_BR_SPOT_PINK03
	_LDVAL				FSW_LOCAL6,ROULETTE_BR_SPOT_PINK04
	_LDVAL				FSW_PARAM5,ROULETTE_BR_SPOT_PINK03BASE
	_JUMP				ev_roulette_seq00_sub
	_END

ev_roulette_seq00_multi_02:
	_LDVAL				FSW_LOCAL5,ROULETTE_BR_SPOT_PINK03
	_LDVAL				FSW_LOCAL6,ROULETTE_BR_SPOT_PINK04
	_LDVAL				FSW_PARAM5,ROULETTE_BR_SPOT_PINK04BASE
	_JUMP				ev_roulette_seq00_sub
	_END

ev_roulette_seq00_single:
	_TALKMSG			msg_roulette_room_23
	_RET

ev_roulette_seq00_multi:
	_TALKMSG			msg_roulette_room_24
	_RET


/********************************************************************/
/*							�G�ǂ��C�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq01:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i��E������*/
	_CALL				ev_roulette_man_right

	_CALL				se_ramp_enemy

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_PURPLE03
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_PURPLE04
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq01_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq01_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq01_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq01_single_safe

	_TALKMSG			msg_roulette_room_27
	_RET

ev_roulette_seq01_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq01_multi_safe

	_TALKMSG			msg_roulette_room_28
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq01_single_safe:
	_TALKMSG			msg_roulette_room_30_23
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq01_multi_safe:
	_TALKMSG			msg_roulette_room_30_24
	_RET


/********************************************************************/
/*							�G�܂ЃC�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq02:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i��E������*/
	_CALL				ev_roulette_man_right

	_CALL				se_ramp_enemy

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_YELLOW03
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_YELLOW04
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq02_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq02_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq02_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq02_single_safe

	_TALKMSG			msg_roulette_room_29
	_RET

ev_roulette_seq02_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq02_multi_safe

	_TALKMSG			msg_roulette_room_30
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq02_single_safe:
	_TALKMSG			msg_roulette_room_30_25
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq02_multi_safe:
	_TALKMSG			msg_roulette_room_30_26
	_RET


/********************************************************************/
/*							�G�₯�ǃC�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq03:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i��E������*/
	_CALL				ev_roulette_man_right

	_CALL				se_ramp_enemy

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_RED03
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_RED04
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq03_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq03_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq03_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq03_single_safe

	_TALKMSG			msg_roulette_room_30_01
	_RET

ev_roulette_seq03_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq03_multi_safe

	_TALKMSG			msg_roulette_room_30_02
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq03_single_safe:
	_TALKMSG			msg_roulette_room_30_27
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq03_multi_safe:
	_TALKMSG			msg_roulette_room_30_28
	_RET


/********************************************************************/
/*							�G�˂ނ�C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq04:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i��E������*/
	_CALL				ev_roulette_man_right

	_CALL				se_ramp_enemy

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_WHITE03
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_WHITE04
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq04_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq04_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq04_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq04_single_safe

	_TALKMSG			msg_roulette_room_30_03
	_RET

ev_roulette_seq04_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq04_multi_safe

	_TALKMSG			msg_roulette_room_30_04
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq04_single_safe:
	_TALKMSG			msg_roulette_room_30_29
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq04_multi_safe:
	_TALKMSG			msg_roulette_room_30_30
	_RET


/********************************************************************/
/*							�G������C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq05:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i��E������*/
	_CALL				ev_roulette_man_right

	_CALL				se_ramp_enemy

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_AO03
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_AO04
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq05_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq05_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq05_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq05_single_safe

	_TALKMSG			msg_roulette_room_30_05
	_RET

ev_roulette_seq05_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq05_multi_safe

	_TALKMSG			msg_roulette_room_30_06
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq05_single_safe:
	_TALKMSG			msg_roulette_room_30_31
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq05_multi_safe:
	_TALKMSG			msg_roulette_room_30_32
	_RET


/********************************************************************/
/*							�G���̂݃C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq06:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_enemy

	_TALK_CLOSE
	_CALL				staff_anm_03

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_GET_ITEM,1,0,FSW_ANSWER
	_ITEM_NAME			2,FSW_ANSWER										/*�A�C�e�����Z�b�g*/

	/*�b��*/
	//_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	//_OBJ_ANIME_WAIT

	/*�A�C�e�������Ă���A�C�R���\��*/
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,0,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,1,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,2,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,3,1

	_TALKMSG			msg_roulette_room_30_08_01
	_TALK_CLOSE
	_CALL				staff_anm_04

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq06_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq06_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq06_single:
	_TALKMSG			msg_roulette_room_30_07
	_RET

ev_roulette_seq06_multi:
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER
	_TALKMSG			msg_roulette_room_30_08
	_RET


/********************************************************************/
/*							�G�A�C�e���C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq07:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_enemy

	_TALK_CLOSE
	_CALL				staff_anm_03

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_GET_ITEM,1,0,FSW_ANSWER
	_ITEM_NAME			2,FSW_ANSWER										/*�A�C�e�����Z�b�g*/

	/*�A�C�e�������Ă���A�C�R���\��*/
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,0,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,1,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,2,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,3,1

	_TALKMSG			msg_roulette_room_30_08_01
	_TALK_CLOSE
	_CALL				staff_anm_04

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq07_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq07_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq07_single:
	_TALKMSG			msg_roulette_room_30_07
	_RET

ev_roulette_seq07_multi:
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER
	_TALKMSG			msg_roulette_room_30_08
	_RET


/********************************************************************/
/*							�G���x���C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq08:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i��E������*/
	_CALL				ev_roulette_man_right

	_CALL				se_ramp_enemy

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_JUMP			FSW_LOCAL6,EQ,0,ev_roulette_seq08_single_02
	_IFVAL_JUMP			FSW_LOCAL6,EQ,1,ev_roulette_seq08_multi_02
	_END

ev_roulette_seq08_sub:
	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_CALL				se_spot_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq08_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq08_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq08_single_02:
	_LDVAL				FSW_LOCAL5,ROULETTE_BR_SPOT_GREEN03
	_LDVAL				FSW_LOCAL6,ROULETTE_BR_SPOT_GREEN04
	_LDVAL				FSW_PARAM5,ROULETTE_BR_SPOT_GREEN03BASE
	_JUMP				ev_roulette_seq08_sub
	_END

ev_roulette_seq08_multi_02:
	_LDVAL				FSW_LOCAL5,ROULETTE_BR_SPOT_GREEN03
	_LDVAL				FSW_LOCAL6,ROULETTE_BR_SPOT_GREEN04
	_LDVAL				FSW_PARAM5,ROULETTE_BR_SPOT_GREEN04BASE
	_JUMP				ev_roulette_seq08_sub
	_END

ev_roulette_seq08_single:
	_TALKMSG			msg_roulette_room_25
	_RET

ev_roulette_seq08_multi:
	_TALKMSG			msg_roulette_room_26
	_RET


/********************************************************************/
/*							�����g�o�C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq09:
	//_SE_PLAY			SEQ_SE_PL_FR01
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i�������*/
	_CALL				ev_roulette_man_left

	_CALL				se_ramp_mine

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_JUMP			FSW_LOCAL6,EQ,0,ev_roulette_seq09_single_02
	_IFVAL_JUMP			FSW_LOCAL6,EQ,1,ev_roulette_seq09_multi_02
	_END

ev_roulette_seq09_sub:
	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_CALL				se_spot_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq09_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq09_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq09_single_02:
	_LDVAL				FSW_LOCAL5,ROULETTE_BR_SPOT_PINK01
	_LDVAL				FSW_LOCAL6,ROULETTE_BR_SPOT_PINK02
	_LDVAL				FSW_PARAM5,ROULETTE_BR_SPOT_PINK01BASE
	_JUMP				ev_roulette_seq09_sub
	_END

ev_roulette_seq09_multi_02:
	_LDVAL				FSW_LOCAL5,ROULETTE_BR_SPOT_PINK01
	_LDVAL				FSW_LOCAL6,ROULETTE_BR_SPOT_PINK02
	_LDVAL				FSW_PARAM5,ROULETTE_BR_SPOT_PINK02BASE
	_JUMP				ev_roulette_seq09_sub
	_END

ev_roulette_seq09_single:
	_TALKMSG			msg_roulette_room_23
	_RET

ev_roulette_seq09_multi:
	_TALKMSG			msg_roulette_room_24
	_RET


/********************************************************************/
/*							�����ǂ��C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq10:
	//_SE_PLAY			SEQ_SE_PL_FR01
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i�������*/
	_CALL				ev_roulette_man_left

	_CALL				se_ramp_mine

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_PURPLE01
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_PURPLE02
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq10_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq10_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq10_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq10_single_safe

	_TALKMSG			msg_roulette_room_27
	_RET

ev_roulette_seq10_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq10_multi_safe

	_TALKMSG			msg_roulette_room_28
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq10_single_safe:
	_TALKMSG			msg_roulette_room_30_23
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq10_multi_safe:
	_TALKMSG			msg_roulette_room_30_24
	_RET


/********************************************************************/
/*							�����܂ЃC�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq11:
	//_SE_PLAY			SEQ_SE_PL_FR01
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i�������*/
	_CALL				ev_roulette_man_left

	_CALL				se_ramp_mine

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_YELLOW01
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_YELLOW02
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq11_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq11_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq11_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq11_single_safe

	_TALKMSG			msg_roulette_room_29
	_RET

ev_roulette_seq11_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq11_multi_safe

	_TALKMSG			msg_roulette_room_30
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq11_single_safe:
	_TALKMSG			msg_roulette_room_30_25
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq11_multi_safe:
	_TALKMSG			msg_roulette_room_30_26
	_RET


/********************************************************************/
/*						�����₯�ǃC�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq12:
	//_SE_PLAY			SEQ_SE_PL_FR01
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i�������*/
	_CALL				ev_roulette_man_left

	_CALL				se_ramp_mine

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_RED01
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_RED02
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq12_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq12_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq12_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq12_single_safe

	_TALKMSG			msg_roulette_room_30_01
	_RET

ev_roulette_seq12_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq12_multi_safe

	_TALKMSG			msg_roulette_room_30_02
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq12_single_safe:
	_TALKMSG			msg_roulette_room_30_27
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq12_multi_safe:
	_TALKMSG			msg_roulette_room_30_28
	_RET


/********************************************************************/
/*							�����˂ނ�C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq13:
	//_SE_PLAY			SEQ_SE_PL_FR01
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i�������*/
	_CALL				ev_roulette_man_left

	_CALL				se_ramp_mine

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_WHITE01
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_WHITE02
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq13_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq13_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq13_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq13_single_safe

	_TALKMSG			msg_roulette_room_30_03
	_RET

ev_roulette_seq13_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq13_multi_safe

	_TALKMSG			msg_roulette_room_30_04
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq13_single_safe:
	_TALKMSG			msg_roulette_room_30_29
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq13_multi_safe:
	_TALKMSG			msg_roulette_room_30_30
	_RET


/********************************************************************/
/*							����������C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq14:
	//_SE_PLAY			SEQ_SE_PL_FR01
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i�������*/
	_CALL				ev_roulette_man_left

	_CALL				se_ramp_mine

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_LDVAL				FSW_LOCAL5,ROULETTE_BS_LASOR_AO01
	_LDVAL				FSW_LOCAL6,ROULETTE_BS_LASOR_AO02
	_CALL				se_laser_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq14_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq14_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq14_single:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq14_single_safe

	_TALKMSG			msg_roulette_room_30_05
	_RET

ev_roulette_seq14_multi:
	/*�N�����ʂ𔭊����Ȃ�������*/
	_ROULETTE_TOOL		FR_ID_GET_POKE_SAFE_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq14_multi_safe

	_TALKMSG			msg_roulette_room_30_06
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq14_single_safe:
	_TALKMSG			msg_roulette_room_30_31
	_RET

/*�N�����ʂ𔭊����Ȃ�������*/
ev_roulette_seq14_multi_safe:
	_TALKMSG			msg_roulette_room_30_32
	_RET


/********************************************************************/
/*							�����؂̎��C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq15:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_mine

	_TALK_CLOSE
	_CALL				staff_anm_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_GET_ITEM,0,0,FSW_ANSWER
	_ITEM_NAME			2,FSW_ANSWER										/*�A�C�e�����Z�b�g*/

	/*�A�C�e�������Ă���A�C�R���\��*/
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,0,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,1,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,2,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,3,1

	_TALKMSG			msg_roulette_room_30_08_01
	_TALK_CLOSE
	_CALL				staff_anm_02_item

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq15_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq15_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq15_single:
	_TALKMSG			msg_roulette_room_30_07
	_RET

ev_roulette_seq15_multi:
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER
	_TALKMSG			msg_roulette_room_30_08
	_RET


/********************************************************************/
/*							�����A�C�e���C�x���g					*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq16:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_mine

	_TALK_CLOSE
	_CALL				staff_anm_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_GET_ITEM,0,0,FSW_ANSWER
	_ITEM_NAME			2,FSW_ANSWER										/*�A�C�e�����Z�b�g*/

	/*�A�C�e�������Ă���A�C�R���\��*/
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,0,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,1,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,2,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,3,1

	_TALKMSG			msg_roulette_room_30_08_01
	_TALK_CLOSE
	_CALL				staff_anm_02_item

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq16_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq16_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq16_single:
	_TALKMSG			msg_roulette_room_30_07
	_RET

ev_roulette_seq16_multi:
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER
	_TALKMSG			msg_roulette_room_30_08
	_RET


/********************************************************************/
/*							�������x���C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq17:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	/*�i�������*/
	_CALL				ev_roulette_man_left

	_CALL				se_ramp_mine

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_JUMP			FSW_LOCAL6,EQ,0,ev_roulette_seq17_single_02
	_IFVAL_JUMP			FSW_LOCAL6,EQ,1,ev_roulette_seq17_multi_02
	_END

ev_roulette_seq17_sub:
	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_CALL				se_spot_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq17_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq17_multi

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq17_single_02:
	_LDVAL				FSW_LOCAL5,ROULETTE_BR_SPOT_GREEN01
	_LDVAL				FSW_LOCAL6,ROULETTE_BR_SPOT_GREEN02
	_LDVAL				FSW_PARAM5,ROULETTE_BR_SPOT_GREEN01BASE
	_JUMP				ev_roulette_seq17_sub
	_END

ev_roulette_seq17_multi_02:
	_LDVAL				FSW_LOCAL5,ROULETTE_BR_SPOT_GREEN01
	_LDVAL				FSW_LOCAL6,ROULETTE_BR_SPOT_GREEN02
	_LDVAL				FSW_PARAM5,ROULETTE_BR_SPOT_GREEN02BASE
	_JUMP				ev_roulette_seq17_sub
	_END

ev_roulette_seq17_single:
	_TALKMSG			msg_roulette_room_25
	_RET

ev_roulette_seq17_multi:
	_TALKMSG			msg_roulette_room_26
	_RET


/********************************************************************/
/*							����C�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq18:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_all

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_CALL				se_mirror_red

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_09								/*����*/

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							�J�C�x���g								*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq19:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_all

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_CALL				se_mirror_blue

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_10								/*�J*/

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							�����C�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq20:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_all

	_CALL				se_smoke_01

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_11								/*����*/

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							�����C�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq21:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_all

	_CALL				ev_roulette_btl_pal_white							/*���ɖ߂�*/
	_CALL				se_mirror_white

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_12								/*�����*/

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							���C�x���g								*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq22:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_all

	_CALL				se_smoke_02

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_13								/*��*/

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							�g���b�N�C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq23:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_all

	_CALL				se_smoke_03

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_14								/*�g���b�N*/

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							�|�P�����`�F���W�C�x���g				*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq24:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_all

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq24_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq24_multi

	/*��������Ԃ��_�łɕύX*/
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_4,2		//2�Ԃ̃A�j�����s
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_5,6		//6�Ԃ̃A�j�����s
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_6,3		//3�Ԃ̃A�j�����s
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_7,7		//7�Ԃ̃A�j�����s

	_SE_PLAY			SEQ_SE_DP_Z_SEARCH									/*�`�F���W��*/

	/*���Ԃɏ����Ă���*/
	_ROULETTE_TOOL		FR_ID_MINE_CHG_POKE_ICON_VANISH,0,3,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ENEMY_CHG_POKE_ICON_VANISH,0,3,FSW_ANSWER
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,3,0
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,3,0
	_TIME_WAIT			14,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_MINE_CHG_POKE_ICON_VANISH,0,2,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ENEMY_CHG_POKE_ICON_VANISH,0,2,FSW_ANSWER
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,2,0
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,2,0
	_TIME_WAIT			14,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_MINE_CHG_POKE_ICON_VANISH,0,1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ENEMY_CHG_POKE_ICON_VANISH,0,1,FSW_ANSWER
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,1,0
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,1,0
	_TIME_WAIT			14,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_MINE_CHG_POKE_ICON_VANISH,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ENEMY_CHG_POKE_ICON_VANISH,0,0,FSW_ANSWER
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,0,0
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,0,0
	_TIME_WAIT			14,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_ICON_CHANGE,0,0,FSW_ANSWER					/*�\���ʒu�ύX*/

	_SE_PLAY			SEQ_SE_DP_Z_SEARCH									/*�`�F���W��*/

	/*���Ԃɕ\�����Ă���*/
	_ROULETTE_TOOL		FR_ID_MINE_CHG_POKE_ICON_VANISH,1,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ENEMY_CHG_POKE_ICON_VANISH,1,0,FSW_ANSWER
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,0,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,0,1
	_TIME_WAIT			14,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_MINE_CHG_POKE_ICON_VANISH,1,1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ENEMY_CHG_POKE_ICON_VANISH,1,1,FSW_ANSWER
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,1,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,1,1
	_TIME_WAIT			14,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_MINE_CHG_POKE_ICON_VANISH,1,2,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ENEMY_CHG_POKE_ICON_VANISH,1,2,FSW_ANSWER
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,2,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,2,1
	_TIME_WAIT			14,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_MINE_CHG_POKE_ICON_VANISH,1,3,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ENEMY_CHG_POKE_ICON_VANISH,1,3,FSW_ANSWER
	_ROULETTE_CHG_ITEMKEEP_VANISH	0,3,1
	_ROULETTE_CHG_ITEMKEEP_VANISH	1,3,1
	_TIME_WAIT			14,FSW_ANSWER

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_15								/*�|�P�����`�F���W*/
	_SE_STOP			SEQ_SE_DP_Z_SEARCH									/*�`�F���W��*/

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq24_single_2
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq24_multi_2

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

/*�p�l��������*/
ev_roulette_seq24_single:
	_OBJ_ANIME			OBJID_MINE,anm_panel_see_01
	_OBJ_ANIME			OBJID_ETC,anm_panel_see_01
	_OBJ_ANIME_WAIT
	_RET

ev_roulette_seq24_multi:
	_OBJ_ANIME			OBJID_SIO_USER_0,anm_panel_see_01
	_OBJ_ANIME			OBJID_SIO_USER_1,anm_panel_see_01
	_OBJ_ANIME			OBJID_ETC,anm_panel_see_01
	_OBJ_ANIME			OBJID_ETC2,anm_panel_see_01
	_OBJ_ANIME_WAIT
	_RET

/*���̌����ɖ߂�*/
ev_roulette_seq24_single_2:
	_OBJ_ANIME			OBJID_MINE,anm_panel_see_02
	_OBJ_ANIME			OBJID_ETC,anm_panel_see_03
	_OBJ_ANIME_WAIT
	_RET

ev_roulette_seq24_multi_2:
	_OBJ_ANIME			OBJID_SIO_USER_0,anm_panel_see_02
	_OBJ_ANIME			OBJID_SIO_USER_1,anm_panel_see_02
	_OBJ_ANIME			OBJID_ETC,anm_panel_see_03
	_OBJ_ANIME			OBJID_ETC2,anm_panel_see_03
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*							���xUP�C�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq25:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_mine

	_CALL				se_monitor_01

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_16								/*���[���b�g���xUP*/

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							���xDOWN�C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq26:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_mine

	_CALL				se_monitor_01

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_17								/*���[���b�g���xDOWN*/

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							���xRANDOM�C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq27:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_mine

	_CALL				se_monitor_01

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_18								/*���[���b�g����RANDOM*/

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							BP�ǉ��C�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq28:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_mine
	_TALK_CLOSE

	_CALL				staff_anm_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_GET_PANEL_BP,0,0,FSW_ANSWER
	_RECORD_ADD			RECID_ROULETTE_BP,FSW_ANSWER
	_BTL_POINT_ADD		FSW_ANSWER											/*�o�g���|�C���g�ǉ�*/
	_NUMBER_NAME		2,FSW_ANSWER

	/*�u������Ɂ�BP���v���[���g���܂��v*/
	/*�ʐM�}���`*/
	_SE_STOP			SEQ_SE_DP_DENDOU
	_ME_PLAY			ME_ROULETTE_BP_GET
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq28_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq28_multi
	_ME_WAIT
	_TALK_CLOSE

	/*���̈ʒu�ɖ߂�*/
	_CALL				staff_anm_02

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_19_02

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END

ev_roulette_seq28_single:
	_TALKMSG			msg_roulette_room_30_19
	_RET

ev_roulette_seq28_multi:
	_TALKMSG			msg_roulette_room_30_19_01
	_RET


/********************************************************************/
/*							BP��ǉ��C�x���g						*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq31:
	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_mine
	_TALK_CLOSE

	_CALL				staff_anm_01

	_ROULETTE_TOOL		FR_ID_TR_NAME,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_TR_NAME,1,1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_GET_PANEL_BP,0,0,FSW_ANSWER
	_RECORD_ADD			RECID_ROULETTE_BP,FSW_ANSWER
	_BTL_POINT_ADD		FSW_ANSWER											/*�o�g���|�C���g�ǉ�*/
	_NUMBER_NAME		2,FSW_ANSWER

	/*�ʐM�}���`*/
	_SE_STOP			SEQ_SE_DP_DENDOU
	_ME_PLAY			ME_ROULETTE_BP_GET
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL6
	_IFVAL_CALL			FSW_LOCAL6,EQ,0,ev_roulette_seq28_single
	_IFVAL_CALL			FSW_LOCAL6,EQ,1,ev_roulette_seq28_multi
	_ME_WAIT
	_TALK_CLOSE

	_CALL				staff_anm_02

	/*�b��*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_60
	_OBJ_ANIME_WAIT

	_TALKMSG			msg_roulette_room_30_19_02

	_SE_PLAY			SEQ_SE_DP_DENDOU									/*����*/
	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*							���퓬�X���[�C�x���g					*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq29:
	/*�񕜃V�[�P���X���΂�*/
	_LDVAL				FSW_PARAM4,1

	_TALKMSG			msg_roulette_room_22								/*����̃C�x���g������*/

	_CALL				se_ramp_enemy
	_TALK_CLOSE

	/*�G�̃A�C�R���A�N�^�[�̂ݍ폜*/
	_CALL				ev_roulette_poke_itemkeep_icon_del_enemy

	_CALL				ev_roulette_battle_btl_off							/*�퓬�Ȃ��ޏ�*/

	/*�i��E������*/
	_CALL				ev_roulette_man_right

	_TALKMSG			msg_roulette_room_30_20								/*�퓬�X���[*/

	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_seq29_single
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_seq29_multi
	_END

ev_roulette_seq29_single:
	_AB_KEYWAIT
	_JUMP				ev_roulette_seq29_sub
	_END

ev_roulette_seq29_multi:
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	//_AB_KEYWAIT

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_BTL_WIN_PANEL
	_COMM_RESET

	_JUMP				ev_roulette_seq29_sub
	_END

ev_roulette_seq29_sub:
	_TALK_CLOSE

	/*�u�[�C���OSE*/
	//_SE_PLAY			SEQ_SE_DP_SELECT
	//_SE_WAIT			SEQ_SE_DP_SELECT

	/*�i������̈ʒu�ɖ߂��Ă���*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_55
	_OBJ_ANIME_WAIT

	_ROULETTE_TOOL		FR_ID_PANEL_BG_OFF,0,0,FSW_ANSWER					/*�p�l����\��*/

	_JUMP				ev_roulette_battle_3								/*�퓬������̗����*/
	_END


/********************************************************************/
/*							���X�J�C�x���g							*/
/*	FSW_LOCAL5,6�g�p												*/
/********************************************************************/
ev_roulette_seq30:
	_TALKMSG			msg_roulette_room_30_21								/*����˂�*/

	/*�u�[�C���OSE*/
	//_SE_PLAY			SEQ_SE_DP_SELECT
	//_SE_WAIT			SEQ_SE_DP_SELECT

	_JUMP				ev_roulette_room_go_2								/*���������*/
	_END


/********************************************************************/
/*					SE+�����v���o(FSW_LOCAL5�g�p)					*/
/********************************************************************/
se_ramp_mine:
	_CALL				ev_roulette_single_l_ramp_actor_set

	_SE_PLAY			SEQ_SE_PL_FR05
	//_SE_WAIT			SEQ_SE_PL_FR05
	//_ARTICLE_ACTOR_ANIME_WAIT	ACTWORK_0		//�A�j���I���҂�
	_TIME_WAIT			RAMP_WAIT,FSW_ANSWER

	_CALL				ev_roulette_single_l_ramp_actor_free
	_RET

se_ramp_enemy:
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL5
	_CALL				ev_roulette_single_r_ramp_actor_set

	_SE_PLAY			SEQ_SE_PL_FR05
	//_SE_WAIT			SEQ_SE_PL_FR05
	//_ARTICLE_ACTOR_ANIME_WAIT	ACTWORK_1		//�A�j���I���҂�
	_TIME_WAIT			RAMP_WAIT,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL5
	_CALL				ev_roulette_single_r_ramp_actor_free
	_RET

se_ramp_all:
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_LOCAL5
	_CALL				ev_roulette_single_l_ramp_actor_set
	_CALL				ev_roulette_single_r_ramp_actor_set

	_SE_PLAY			SEQ_SE_PL_FR05
	//_SE_WAIT			SEQ_SE_PL_FR05
	//_ARTICLE_ACTOR_ANIME_WAIT	ACTWORK_0		//�A�j���I���҂�
	_TIME_WAIT			RAMP_WAIT,FSW_ANSWER

	_CALL				ev_roulette_single_l_ramp_actor_free
	_CALL				ev_roulette_single_r_ramp_actor_free
	_RET


/********************************************************************/
/*						SE+�~���[�{�[�����o							*/
/********************************************************************/
se_mirror_blue:
	_SE_PLAY			SEQ_SE_PL_FR04
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_MIRROR_BLUE
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_MIRROR_BLUEBG
	//_PARTICLE_WAIT
	_TIME_WAIT			PARTICLE_PANEL_EFF_WAIT,FSW_ANSWER
	_SE_WAIT			SEQ_SE_PL_FR04
	_RET

se_mirror_red:
	_SE_PLAY			SEQ_SE_PL_FR04
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_MIRROR_RED
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_MIRROR_REDBG
	//_PARTICLE_WAIT
	_TIME_WAIT			PARTICLE_PANEL_EFF_WAIT,FSW_ANSWER
	_SE_WAIT			SEQ_SE_PL_FR04
	_RET

se_mirror_white:
	_SE_PLAY			SEQ_SE_PL_FR04
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_MIRROR_WHITE
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_MIRROR_WHITEBG
	//_PARTICLE_WAIT
	_TIME_WAIT			PARTICLE_PANEL_EFF_WAIT,FSW_ANSWER
	_SE_WAIT			SEQ_SE_PL_FR04
	_RET


/********************************************************************/
/*						SE+�X���[�N���o								*/
/********************************************************************/
se_smoke_01:
	_SE_PLAY			SEQ_SE_PL_FR03
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_KEMURI01_L
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_KEMURI01_R
	//_PARTICLE_WAIT
	_TIME_WAIT			PARTICLE_PANEL_EFF_WAIT,FSW_ANSWER
	_SE_WAIT			SEQ_SE_PL_FR03
	_RET

se_smoke_02:
	_SE_PLAY			SEQ_SE_PL_FR03
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_KEMURI02_L
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_KEMURI02_R
	//_PARTICLE_WAIT
	_TIME_WAIT			PARTICLE_PANEL_EFF_WAIT,FSW_ANSWER
	_SE_WAIT			SEQ_SE_PL_FR03
	_RET

se_smoke_03:
	_SE_PLAY			SEQ_SE_PL_FR03
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_KEMURI03_L
	_PARTICLE_ADD_EMITTER	SPAWORK_0, ROULETTE_BR_KEMURI03_R
	//_PARTICLE_WAIT
	_TIME_WAIT			PARTICLE_PANEL_EFF_WAIT,FSW_ANSWER
	_SE_WAIT			SEQ_SE_PL_FR03
	_RET


/********************************************************************/
/*SE+���[�U�[���o(FSW_LOCAL5,6�g�p �t�@�C�������Ȃ���"BS"�Ȃ̂Œ���)*/
/********************************************************************/
se_laser_01:
	_SE_PLAY			SEQ_SE_PL_FR04
	_PARTICLE_ADD_EMITTER	SPAWORK_0,FSW_LOCAL5
	_PARTICLE_ADD_EMITTER	SPAWORK_0,FSW_LOCAL6
	//_PARTICLE_WAIT
	_TIME_WAIT			PARTICLE_PANEL_EFF_WAIT,FSW_ANSWER
	_SE_WAIT			SEQ_SE_PL_FR04
	_RET


/********************************************************************/
/*			SE+�X�|�b�g���C�g���o(FSW_LOCAL5,6,FSW_PARAM5�g�p)		*/
/********************************************************************/
se_spot_01:
	_SE_PLAY			SEQ_SE_PL_FR04
	_PARTICLE_ADD_EMITTER	SPAWORK_0,FSW_LOCAL5
	_PARTICLE_ADD_EMITTER	SPAWORK_0,FSW_LOCAL6
	_PARTICLE_ADD_EMITTER	SPAWORK_0,FSW_PARAM5

	//_PARTICLE_WAIT
	_TIME_WAIT			PARTICLE_PANEL_EFF_WAIT,FSW_ANSWER
	_SE_WAIT			SEQ_SE_PL_FR04
	_RET


/********************************************************************/
/*						SE+���j�^�[�_��								*/
/********************************************************************/
se_monitor_01:
	_SE_PLAY			SEQ_SE_PL_FR05
	//_SE_WAIT			SEQ_SE_PL_FR05
	_RET


/********************************************************************/
/*					��t�Ɏ�n�����A�j��							*/
/********************************************************************/
/*������*/
staff_anm_01:
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_70
	_OBJ_ANIME_WAIT
	_RET

staff_anm_02_item:
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_75_item
	_OBJ_ANIME_WAIT
	_RET

staff_anm_02:
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_75
	_OBJ_ANIME_WAIT
	_RET

/*�G��*/
staff_anm_03:
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_72
	_OBJ_ANIME_WAIT
	_RET

staff_anm_04:
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_77
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*						�g���[�i�[�o��								*/
/********************************************************************/
ev_room_choice_24:
	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_send_sel_taisen
	_JUMP				ev_roulette_taisen_go
	_END

ev_roulette_taisen_go:
	_JUMP				ev_roulette_room_etc_add
	_END

ev_roulette_send_sel_taisen:
	/*�u���΂炭���҂����������v*/
	//_TALKMSG_NOSKIP	msg_roulette_room_wait
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_CHANGE
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/

	//_TALK_CLOSE
	_JUMP				ev_roulette_room_etc_add
	_END


/********************************************************************/
/*																	*/
/********************************************************************/
ev_roulette_comm_command_initialize_1:
	_TIME_WAIT			1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_INIT_1
	_COMM_RESET
	_RET

ev_roulette_comm_command_initialize_2:
	_TIME_WAIT			1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_INIT_2
	_COMM_RESET
	_RET

ev_roulette_comm_command_initialize_3:
	_TIME_WAIT			1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_INIT_3
	_COMM_RESET
	_RET

ev_roulette_comm_command_initialize_4:
	_TIME_WAIT			1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_INIT_4
	_COMM_RESET
	_RET


/********************************************************************/
/*						�g���[�i�[�o��2								*/
/********************************************************************/
ev_roulette_room_etc_add:
	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_go_multi

	/*�u���[���o�ꂩ�`�F�b�N*/
	_ROULETTE_TOOL		FR_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_leader_1st_02
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_roulette_leader_2nd_02

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�G�g���[�i�[�������܂ŕ���*/
	_CALL				ev_roulette_trainer_set_1
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_single
	_OBJ_ANIME_WAIT

	_CALL				ev_roulette_poke_itemkeep_icon_add_enemy	/*(ENEMY)�|�P�����A�C�R���ǉ�*/
	_RET

ev_roulette_room_go_multi:
	/*�G�g���[�i�[�������܂ŕ���*/
	_CALL				ev_roulette_trainer_set_2
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in
	_OBJ_ANIME			OBJID_ETC2,etc_anime_room_in_multi
	_OBJ_ANIME_WAIT

	_CALL				ev_roulette_poke_itemkeep_icon_add_enemy	/*(ENEMY)�|�P�����A�C�R���ǉ�*/
	_RET


/********************************************************************/
/*																	*/
/********************************************************************/
ev_roulette_room_go_2:
	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_msg_02_single
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_msg_02_multi
	_END

ev_roulette_msg_02_single:
	_AB_KEYWAIT
	_TALK_CLOSE

	/*�i������̈ʒu�ɖ߂��Ă���*/
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_55
	_OBJ_ANIME_WAIT

	/*�u����ł̓o�g���X�^�[�g�I�v*/
	_TALKMSG			msg_roulette_room_30_22
	_AB_KEYWAIT
	_TALK_CLOSE

	_TIME_WAIT			15,FSW_ANSWER

	_OBJ_ANIME			OBJID_ETC,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_ROULETTE_TOOL	FR_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_ROULETTE_APPEAR	0
	_AB_KEYWAIT
	_TALK_CLOSE

	/*����O��*/
	_OBJ_ANIME			OBJID_MINE,anm_go_btl_walk_left
	_OBJ_ANIME			OBJID_ETC,anm_go_btl_walk_right
	_OBJ_ANIME_WAIT

	_JUMP				ev_roulette_battle
	_END

ev_roulette_msg_02_multi:
	/*�r���ɕ������肪����̂ŁA�E�F�C�g�ł͂Ȃ��A�L�[�҂������Ă��܂�*/
	//_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_AB_KEYWAIT

	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_roulette_room_wait

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_PANEL_INFO
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/

	//_AB_KEYWAIT
	//_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER

	/*�i������̈ʒu�ɖ߂��Ă���*/
	_TALK_CLOSE
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_55
	_OBJ_ANIME_WAIT

	/*�u����ł̓o�g���X�^�[�g�I�v*/
	_TALKMSG_NOSKIP		msg_roulette_room_30_22
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_TALK_CLOSE

	_TIME_WAIT			15,FSW_ANSWER

	_OBJ_ANIME			OBJID_ETC,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_ROULETTE_TOOL	FR_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_ROULETTE_APPEAR	0
	_TIME_WAIT		30,FSW_ANSWER
	_TALK_CLOSE

	_OBJ_ANIME			OBJID_ETC2,anm_stay_walk_left
	_OBJ_ANIME_WAIT

	/*�ΐ�O�䎌*/
	_ROULETTE_TOOL	FR_ID_SET_B_TOWER_PARTNER_DATA,0,0,FSW_ANSWER
	_TALKMSG_ROULETTE_APPEAR	1
	_TIME_WAIT		30,FSW_ANSWER
	_TALK_CLOSE

	/*����O��*/
	_OBJ_ANIME		OBJID_SIO_USER_0,anm_go_btl_walk_left
	_OBJ_ANIME		OBJID_SIO_USER_1,anm_go_btl_walk_left
	_OBJ_ANIME		OBJID_ETC,anm_go_btl_walk_right
	_OBJ_ANIME		OBJID_ETC2,anm_go_btl_walk_right
	_OBJ_ANIME_WAIT

	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_roulette_room_wait

	_JUMP				ev_roulette_battle
	_END


/********************************************************************/
/*					�g���[�i�[�̌����ڃZ�b�g						*/
/********************************************************************/
ev_roulette_trainer_set_1:
	_ROULETTE_TOOL		FR_ID_GET_TR_OBJ_CODE,0,0,FSW_ANSWER
	_LDWK				FSW_PARAM2,FSW_ANSWER
	_CALL				ev_roulette_actor_set_btl_tr
	_RET

ev_roulette_trainer_set_2:
	_ROULETTE_TOOL		FR_ID_GET_TR_OBJ_CODE,0,0,FSW_ANSWER
	_LDWK				FSW_PARAM2,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_GET_TR_OBJ_CODE,1,0,FSW_ANSWER
	_LDWK				FSW_PARAM3,FSW_ANSWER
	_CALL				ev_roulette_actor_set_btl_tr_multi
	_RET

ev_roulette_trainer_set_brain:
	_LDVAL				FSW_PARAM2,BRAINS4
	_CALL				ev_roulette_actor_set_btl_tr
	_RET


/********************************************************************/
/*						���݂�����O��								*/
/********************************************************************/
ev_roulette_battle:
	/*�t�F�[�h�A�E�g*/
	//_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_multi_battle_2

	/*�u���[���o�ꂩ�`�F�b�N*/
	_ROULETTE_TOOL		FR_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_leader_1st_33
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_roulette_leader_2nd_33

	_ENCOUNT_EFFECT		FR_ENCOUNT_EFF_BOX_LR
	_JUMP				ev_roulette_battle_2
	_END

ev_roulette_leader_1st_33:
ev_roulette_leader_2nd_33:
	_PARTICLE_SPA_EXIT	SPAWORK_0					/*�G���J�E���g�G�t�F�N�g�O�Ƀp�[�e�B�N���폜*/

	_RECORD_INC			RECID_FRONTIER_BRAIN
	_BRAIN_ENCOUNT_EFFECT	FRONTIER_NO_ROULETTE
	_JUMP				ev_roulette_battle_2
	_END


/*�ʐM�}���`*/
ev_roulette_multi_battle_2:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_BATTLE
	_COMM_RESET

	_TALK_CLOSE
	_ENCOUNT_EFFECT		FR_ENCOUNT_EFF_BOX_LR
	_JUMP				ev_roulette_battle_2
	_END


/********************************************************************/
//
/********************************************************************/
ev_roulette_battle_2:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	_CALL				ev_roulette_poke_itemkeep_icon_del			/*�|�P�����A�C�R���폜*/
	_CALL				ev_roulette_poke_itemkeep_icon_resource_del	/*���\�[�X�폜*/

	/*�u���[���o�ꂩ�`�F�b�N*/
	_ROULETTE_TOOL		FR_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_battle_2_0
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_roulette_battle_2_0

	/*�u���[���ȊO�́A���̃^�C�~���O�Ńp�[�e�B�N�����폜����*/
	_PARTICLE_SPA_EXIT	SPAWORK_0
	_JUMP				ev_roulette_battle_2_0
	_END

ev_roulette_battle_2_0:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get

	/*���\�[�X���(�A�N�^�[�̓��[�N�ԍ��A���\�[�X��ACTID�ŉ�����鎖�ɒ���)*/
	_ARTICLE_RESOURCE_FREE	ACTID_ROULETTE_RAMP

	/*�|�P�p�l�����\�[�X�{�A�N�^�[�폜*/
	_CALL				ev_pokepanel_del

#ifndef DEBUG_FRONTIER_LOOP

#ifndef DEBUG_BTL_OFF	/************************************************/
	/*�퓬�Ăяo��*/
	_ROULETTE_BATTLE_CALL
	_ROULETTE_CALL_GET_RESULT
#endif	/****************************************************************/

#endif	//DEBUG_FRONTIER_LOOP

	/*�Ƃ肠�����G�̃A�C�R���͕\�����Ȃ�*/
	_CALL			ev_roulette_poke_itemkeep_icon_add_mine			/*(MINE)�|�P�����A�C�R���ǉ�*/
	//_CALL				ev_roulette_poke_itemkeep_icon_add		/*�|�P�����A�C�R���ǉ�*/

	_ROULETTE_TOOL		FR_ID_SET_MAIN_BG,0,0,FSW_ANSWER/*�w�i�ύX(�����ŒʐM�^�C�v���`�F�b�N)*/

	/*�|�P�p�l�����\�[�X�{�A�N�^�[�Z�b�g*/
	_CALL					ev_pokepanel_set

	_CALL				ev_roulette_btl_pal_black					/*�Â�����*/
	_CALL				ev_roulette_obj_pal_black					/*OBJ�Â�����*/

	/*�t�F�[�h�C��*/
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

#ifndef DEBUG_FRONTIER_LOOP

#ifndef DEBUG_BTL_LOSE_OFF	/********************************************/
	/*�퓬���ʂŕ���*/
	_ROULETTE_LOSE_CHECK	FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_lose		/*�s�k*/
#endif	/****************************************************************/

#endif	//DEBUG_FRONTIER_LOOP

	_RECORD_INC			RECID_ROULETTE

	/*�|�P�����`�F���W�C�x���g�̎��̓A�C�R�����t�ɂ��Ă�������K�v�����邩���H*/
	/*���肵���C�x���g�i���o�[���擾*/
	//_ROULETTE_TOOL		FR_ID_GET_DECIDE_EV_NO,0,0,FSW_ANSWER
	//_IFVAL_CALL			FSW_ANSWER,EQ,ROULETTE_EV_PLACE_POKE_CHANGE,ev_btl_after_poke_icon_sub

	/*�u���[���o�ꂩ�`�F�b�N*/
	_ROULETTE_TOOL		FR_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_leader_1st_03
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_roulette_leader_2nd_03

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	_JUMP				ev_roulette_battle_2_sub
	_END

ev_roulette_battle_2_sub:
	_CALL				ev_roulette_battle_2_2								/*�ޏ�*/
	_JUMP				ev_roulette_battle_3								/*�퓬������̗����*/
	_END


/********************************************************************/
/*								����								*/
/********************************************************************/
ev_roulette_battle_2_2:
	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_battle_2_multi

	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_go_out
	_OBJ_ANIME			OBJID_MINE,mine_anime_go_out
	_OBJ_ANIME_WAIT
	_ACTOR_FREE			OBJID_ETC
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_RET

ev_roulette_battle_2_multi:
	_CALL				ev_roulette_comm_command_initialize_3

	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_go_out
	_OBJ_ANIME			OBJID_ETC2,etc_anime_go_out_multi
	_OBJ_ANIME			OBJID_SIO_USER_0,mine_anime_go_out
	_OBJ_ANIME			OBJID_SIO_USER_1,mine_anime_go_out
	_OBJ_ANIME_WAIT
	_ACTOR_FREE			OBJID_ETC
	_ACTOR_FREE			OBJID_ETC2
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_CHAR_RESOURCE_FREE	FSW_PARAM3
	_RET


/********************************************************************/
/*						�퓬���X���[�������̑ޏ�					*/
/********************************************************************/
ev_roulette_battle_btl_off:
	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_battle_2_multi_btl_off

	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_go_out_btl_off
	_OBJ_ANIME_WAIT
	_ACTOR_FREE			OBJID_ETC
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_RET

ev_roulette_battle_2_multi_btl_off:
	_CALL				ev_roulette_comm_command_initialize_4

	/*�G�g���[�i�[�ޏ�*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_go_out_btl_off
	_OBJ_ANIME			OBJID_ETC2,etc_anime_go_out_btl_off
	_OBJ_ANIME_WAIT
	_ACTOR_FREE			OBJID_ETC
	_ACTOR_FREE			OBJID_ETC2
	_CHAR_RESOURCE_FREE	FSW_PARAM2
	_CHAR_RESOURCE_FREE	FSW_PARAM3
	_RET


/********************************************************************/
//
/********************************************************************/
ev_roulette_battle_3:
	_JUMP				ev_roulette_battle_4
	_END

ev_roulette_battle_3_multi:
	//_ACTOR_FREE			OBJID_SIO_USER_0
	//_ACTOR_FREE			OBJID_SIO_USER_1

	/*��l���̌����ڊi�[*/
	//_GET_MINE_OBJ		FSW_LOCAL7

	//_CHAR_RESOURCE_SET	default_set_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_JUMP				ev_roulette_battle_4
	_END


/********************************************************************/
//
/********************************************************************/
ev_roulette_battle_4:
	_CALL				ev_roulette_battle_5
	_CALL				ev_roulette_battle_6
	_JUMP				ev_roulette_battle_7
	_END

ev_roulette_battle_5:

#ifdef DEBUG_FRONTIER_LOOP
	_RET
#endif	//DEBUG_FRONTIER_LOOP

	/*�����l��+1*/
	_ROULETTE_TOOL		FR_ID_INC_ROUND,1,0,FSW_ANSWER

	/*�A����+1*/
	_ROULETTE_TOOL		FR_ID_INC_RENSYOU,0,0,FSW_ANSWER
	_RET

ev_roulette_battle_6:
	/*�ʐM�F��{�����Ƃ�*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_roulette_multi_comm_basic_call
	_RET

ev_roulette_battle_7:

#ifndef DEBUG_7BTL_OFF	/********************************************/
	/*��7�l�ڂłȂ����͓��ɖ߂�*/
	_ROULETTE_TOOL		FR_ID_GET_ROUND,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,NE,7,ev_roulette_room_renshou_17		/*�A������1-6�̎�*/
#endif	/************************************************************/

	_JUMP				ev_roulette_room_7_win
	_END


/********************************************************************/
/*							7�A������(�L�^)							*/
/********************************************************************/
ev_roulette_room_7_win:

	/*�퓬���Ȃ��p�l���̎��͔�΂�*/
	_IFVAL_JUMP			FSW_PARAM4,GE,1,ev_roulette_room_7_win_bp

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_roulette_room_7_win_bp_sub

	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_roulette_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_roulette_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX		FSW_ANSWER										/*�������f�t�H���g*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_7_win_bp
	_JUMP				ev_roulette_room_7_win_rec_yes
	_END

ev_roulette_room_7_win_bp_sub:
	_TALKMSG_NOSKIP		msg_roulette_room_wait
	_JUMP				ev_roulette_room_7_win_bp
	_END

/*�u�͂��v*/
ev_roulette_room_7_win_rec_yes:
	_CALL				ev_roulette_room_rec_win		/*�����̋L�^*/
	_JUMP				ev_roulette_room_7_win_bp
	_END


/********************************************************************/
/*							7�A������(BP�擾)						*/
/********************************************************************/
ev_roulette_room_7_win_bp:
	//7�A��(�N���A)�p�����[�^�Z�b�g
	_ROULETTE_TOOL		FR_ID_SET_CLEAR,0,0,FSW_ANSWER

	/*7�A�������̂ŏI����*/
	//_LDVAL			WK_SCENE_ROULETTE_LOBBY,1				/*7�A������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_ROULETTE_LOBBY,1			/*7�A������n�܂�悤�ɂ��Ă���*/

	/*�u�V����@�Ƃ��ρ@���߂łƂ��v*/
	_TALKMSG			msg_roulette_room_31

	_PLAYER_NAME		0										/*�v���C���[���Z�b�g*/

	/*�o�g���|�C���g�ǉ�*/
	_ROULETTE_TOOL		FR_ID_GET_BP_POINT,0,0,FSW_ANSWER
	_NUMBER_NAME		1,FSW_ANSWER								/*���l�Z�b�g*/
	_BTL_POINT_ADD		FSW_ANSWER

	/*�u�a�o����������v*/
	_TALKMSG			msg_roulette_room_32
	_ME_PLAY			ME_BP_GET
	_ME_WAIT

	_JUMP				ev_roulette_room_bp_end
	_END


/********************************************************************/
/*							7�A�������I��							*/
/********************************************************************/
ev_roulette_room_bp_end:
	_JUMP				ev_roulette_lose_timing
	_END


/********************************************************************/
/*							�A���� 1-7								*/
/********************************************************************/
ev_roulette_room_renshou_17:

	/*�񕜃V�[�P���X���΂�*/
	_IFVAL_JUMP			FSW_PARAM4,GE,1,ev_roulette_room_renshou_17_sub

	/*�񕜂̐l�o��*/
	_CALL				ev_roulette_actor_set_btl_kaihuku
	_OBJ_ANIME			OBJID_KAIHUKU,kaihuku_anime_10
	_OBJ_ANIME_WAIT

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_renshou_17_multi

	_OBJ_ANIME			OBJID_MINE,player_kaihuku_anime_10
	_OBJ_ANIME_WAIT
	_JUMP				ev_roulette_room_renshou_17_common
	_END

/*�}���`�@�񕜂̕�������*/
ev_roulette_room_renshou_17_multi:
	_OBJ_ANIME			OBJID_SIO_USER_0,player_kaihuku_anime_10
	_OBJ_ANIME			OBJID_SIO_USER_1,player_kaihuku_anime_10
	_OBJ_ANIME_WAIT
	_JUMP				ev_roulette_room_renshou_17_common
	_END

ev_roulette_room_renshou_17_common:
	/*�u�|�P�����̏�Ԃ������܂��v*/
	_TALKMSG			msg_roulette_room_3
	_ME_PLAY			SEQ_ASA
	_ME_WAIT
	_TALK_CLOSE

	_PC_KAIFUKU

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_renshou_17_multi2

	/*�񕜂̐l�����Ă���*/
	_OBJ_ANIME			OBJID_KAIHUKU,kaihuku_anime_15
	_OBJ_ANIME			OBJID_MINE,player_kaihuku_anime_15
	_OBJ_ANIME_WAIT
	_JUMP				ev_roulette_room_renshou_17_common2
	_END

ev_roulette_room_renshou_17_multi2:
	_OBJ_ANIME			OBJID_KAIHUKU,kaihuku_anime_15
	_OBJ_ANIME			OBJID_SIO_USER_0,player_kaihuku_anime_15
	_OBJ_ANIME			OBJID_SIO_USER_1,player_kaihuku_anime_15
	_OBJ_ANIME_WAIT
	_JUMP				ev_roulette_room_renshou_17_common2
	_END

ev_roulette_room_renshou_17_common2:
	_ACTOR_FREE			OBJID_KAIHUKU
	_CHAR_RESOURCE_FREE	FSW_PARAM6
	_JUMP				ev_roulette_room_saikai
	_END

ev_roulette_room_renshou_17_sub:
	_JUMP				ev_roulette_room_saikai
	_END


/********************************************************************/
/*						���f�Z�[�u����̍ĊJ						*/
/********************************************************************/
ev_roulette_room_saikai:
	/*�u���[���o�ꂩ�`�F�b�N*/
	_ROULETTE_TOOL		FR_ID_CHECK_BRAIN,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_leader_1st_01
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_roulette_leader_2nd_01

	_JUMP				ev_roulette_room_saikai_00
	_END

ev_roulette_room_saikai_00:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	_JUMP				ev_roulette_room_saikai_02
	_END

ev_roulette_room_saikai_02:
	/*�A�������Z�b�g*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�u���́@����ڂł���v*/
	_ROULETTE_TOOL		FR_ID_GET_ROUND,0,0,FSW_ANSWER
	_ADD_WK				FSW_ANSWER,1
	_NUMBER_NAME		0,FSW_ANSWER
	_TALKMSG			msg_roulette_room_4

	_JUMP			ev_roulette_room_menu_make
	_END


/********************************************************************/
/*							���j���[����							*/
/********************************************************************/
ev_roulette_room_menu_make:

	_ROULETTE_TOOL	FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_roulette_room_menu_make_comm

	/*�퓬���Ȃ��p�l���̎��́u���낭����v���j���[�Ȃ�*/
	_IFVAL_JUMP		FSW_PARAM4,EQ,1,ev_roulette_room_menu_make_03_set

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,0,ev_roulette_room_menu_make_04_set

	_JUMP			ev_roulette_room_menu_make_03_set
	_END

/*�ʐM�̎��̃��j���[*/
ev_roulette_room_menu_make_comm:

	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_roulette_room_menu_make_01_set

	/*�퓬���Ȃ��p�l���̎��́u���낭����v���j���[�Ȃ�*/
	_IFVAL_JUMP		FSW_PARAM4,EQ,1,ev_roulette_room_menu_make_01_set

	/*�܂��L�^���Ă��Ȃ��A���f���A�ł͂Ȃ����́u���낭����v���j���[�ǉ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,0,ev_roulette_room_menu_make_02_set

	_JUMP			ev_roulette_room_menu_make_01_set
	_END

ev_roulette_room_menu_make_01_set:
	_LDVAL				FSW_ANSWER,1
	_JUMP				ev_roulette_room_menu_make_sub
	_END

ev_roulette_room_menu_make_02_set:
	_LDVAL				FSW_ANSWER,2
	_JUMP				ev_roulette_room_menu_make_sub
	_END

ev_roulette_room_menu_make_03_set:
	_LDVAL				FSW_ANSWER,3
	_JUMP				ev_roulette_room_menu_make_sub
	_END

ev_roulette_room_menu_make_04_set:
	_LDVAL				FSW_ANSWER,4
	_JUMP				ev_roulette_room_menu_make_sub
	_END

/*�ǂ̃��j���[�`���ɂ��邩*/
ev_roulette_room_menu_make_sub:
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_menu_make_01
	_IFVAL_JUMP			FSW_ANSWER,EQ,2,ev_roulette_room_menu_make_02
	_IFVAL_JUMP			FSW_ANSWER,EQ,3,ev_roulette_room_menu_make_03
	_IFVAL_JUMP			FSW_ANSWER,EQ,4,ev_roulette_room_menu_make_04
	_END

ev_roulette_room_menu_make_01:
	_BMPLIST_INIT_EX	24,13,0,0,FSW_PARAM1			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_roulette_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0	/*�Â���*/
	_JUMP			ev_roulette_room_yasumu_next
	_END

ev_roulette_room_menu_make_02:
	_BMPLIST_INIT_EX	23,11,0,0,FSW_PARAM1			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_roulette_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0	/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_roulette_room_choice_02,FSEV_WIN_TALK_MSG_NONE,1	/*���낭����*/
	_JUMP			ev_roulette_room_yasumu_next
	_END

ev_roulette_room_menu_make_03:
	_BMPLIST_INIT_EX	24,11,0,0,FSW_PARAM1			/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_roulette_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0	/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_roulette_room_choice_03,FSEV_WIN_TALK_MSG_NONE,2	/*�₷��*/
	_JUMP			ev_roulette_room_yasumu_next
	_END

ev_roulette_room_menu_make_04:
	_BMPLIST_INIT_EX	23,9,0,0,FSW_PARAM1				/*B�L�����Z������*/
	_BMPLIST_MAKE_LIST	msg_roulette_room_choice_01,FSEV_WIN_TALK_MSG_NONE,0	/*�Â���*/
	_BMPLIST_MAKE_LIST	msg_roulette_room_choice_02,FSEV_WIN_TALK_MSG_NONE,1	/*���낭����*/
	_BMPLIST_MAKE_LIST	msg_roulette_room_choice_03,FSEV_WIN_TALK_MSG_NONE,2	/*�₷��*/
	_JUMP			ev_roulette_room_yasumu_next
	_END

ev_roulette_room_yasumu_next:
	_BMPLIST_MAKE_LIST	msg_roulette_room_choice_04,FSEV_WIN_TALK_MSG_NONE,3	/*���^�C�A*/
	_BMPLIST_START
	_JUMP			ev_roulette_room_menu_sel
	_END


/********************************************************************/
/*							���j���[����							*/
/********************************************************************/
ev_roulette_room_menu_sel:

#ifdef DEBUG_FRONTIER_LOOP
	_JUMP			ev_room_choice_01
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
	_LDVAL				FSW_PARAM4,0				/*�񕜃V�[�P���X���f���[�N�N���A*/

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_room_choice_01_multi
	_JUMP				ev_room_choice_01_2
	_END

/*�ʐM�}���`*/
ev_room_choice_01_multi:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME			0
	_TALKMSG			msg_roulette_room_10
	_JUMP				ev_room_choice_01_multi_retry
	_END

ev_room_choice_01_multi_retry:
	_LDVAL				FSW_LOCAL2,ROULETTE_COMM_RETIRE
	_ROULETTE_SEND_BUF	FSW_LOCAL2,0,FSW_ANSWER							/*0=�Â���*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_room_choice_01_multi_retry	/*���M���s�́A�đ��M��*/

	_ROULETTE_RECV_BUF	FSW_LOCAL2
	_COMM_RESET

	/*���肪�I�񂾃��j���[���擾*/
	_ROULETTE_TOOL		FR_ID_GET_RETIRE_FLAG,0,0,FSW_ANSWER

	/*�p�[�g�i�[�����^�C�A������*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_giveup_yes_multi	/*���^�C�A*/

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_MENU
	_COMM_RESET

	_JUMP				ev_room_choice_01_2
	_END

/*�p�[�g�i�[�����^�C�A������*/
ev_roulette_room_giveup_yes_multi:
	/*�p�[�g�i�[����\��*/
	_PAIR_NAME			0
	_TALKMSG_NOSKIP		msg_roulette_room_12
	_TIME_WAIT			FRONTIER_COMMON_WAIT,FSW_ANSWER
	_JUMP				ev_roulette_room_giveup_yes_multi_sync
	_END


/********************************************************************/
/**/
/********************************************************************/
ev_room_choice_01_2:
	/*���[�N���N���A���Ă���*/
	_LDVAL				FSW_LOCAL3,0

	/*���̓G�|�P�����𐶐�*/
	_ROULETTE_BTL_AFTER_PARTY_SET		/*�o�g�����POKEPARTY�̃Z�b�g*/

	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM�}���`*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_roulette_multi_room_enemy_poke_send2

	_JUMP				ev_roulette_room_go_rental_2
	_END


/********************************************************************/
/*							�L�^����*/
/********************************************************************/
ev_room_choice_02:
	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_roulette_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL			FSW_ANSWER,EQ,1,ev_roulette_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX		FSW_ANSWER										/*�������f�t�H���g*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_rec_no
	_JUMP				ev_roulette_room_rec_yes
	_END

/*�u�͂��v*/
ev_roulette_room_rec_yes:
	_CALL				ev_roulette_room_rec_win	/*�����̋L�^*/
	_JUMP				ev_roulette_room_saikai		/*���j���[�֖߂�*/
	_END

/*�u�������v*/
ev_roulette_room_rec_no:
	_JUMP				ev_roulette_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*							���ʋL�^*/
/********************************************************************/
/*����*/
ev_roulette_room_rec_win:
	_CALL				ev_roulette_room_rec_common		/*ANSWER,PARAM0�g�p��*/
	//_ADD_WK			FSW_PARAM0,1
	//_SUB_WK			FSW_PARAM0,1					/*���E���h���ł͂Ȃ��������Ȃ̂�*/
	_JUMP				ev_roulette_room_rec
	_END

/*����*/
ev_roulette_room_rec_lose:
	_CALL				ev_roulette_room_rec_common		/*ANSWER,PARAM0�g�p��*/
	_ADD_WK				FSW_PARAM0,1
	_JUMP				ev_roulette_room_rec
	_END

/*���ʕ���*/
ev_roulette_room_rec_common:
	/*�^��f�[�^�Z�[�u*/
	_ROULETTE_TOOL		FR_ID_GET_RENSYOU,0,0,FSW_PARAM0
	_RET

ev_roulette_room_rec:
	_JUMP				ev_roulette_room_rec_sub
	_END

ev_roulette_room_rec_sub:
	/*�L�^�������[�N�Z�b�g*/
	_LDVAL				FSW_LOCAL3,1

	_TALKMSG_ALL_PUT	msg_roulette_room_6_4
	_ROULETTE_TOOL		FR_ID_GET_TYPE,0,0,FSW_ANSWER
	_ADD_WAITICON
	_BATTLE_REC_SAVE	FRONTIER_NO_ROULETTE,FSW_ANSWER,FSW_PARAM0,FSW_PARAM0
	_DEL_WAITICON

	//_BATTLE_REC_LOAD

	_IFVAL_JUMP			FSW_PARAM0,EQ,1,ev_roulette_room_rec_true

	/*�u�L�^�o���܂���ł����v*/
	_TALKMSG			msg_roulette_room_6_2
	_RET

ev_roulette_room_rec_true:
	_SE_PLAY			SEQ_SE_DP_SAVE
	/*�u�L�^����܂����v*/
	_PLAYER_NAME		0
	_TALKMSG			msg_roulette_room_6_1
	_RET

/*�^��f�[�^���Ȃ������b�Z�[�W*/
ev_roulette_room_rec_msg1:
	_TALKMSG			msg_roulette_room_6
	_RET

/*���łɘ^��f�[�^�����鎞���b�Z�[�W*/
ev_roulette_room_rec_msg2:
	_TALKMSG			msg_roulette_room_6_3
	_RET


/********************************************************************/
/*							�₷��*/
/********************************************************************/
ev_room_choice_03:
	/*�u���|�[�g�������ďI�����܂����H�v*/
	_TALKMSG			msg_roulette_room_7
	_YES_NO_WIN			FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_rest_no
	_JUMP				ev_roulette_room_rest_yes
	_END

/*�u�͂��v*/
ev_roulette_room_rest_yes:
	//_LDVAL			WK_SCENE_ROULETTE_LOBBY,2			/*��������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_ROULETTE_LOBBY,2		/*��������n�܂�悤�ɂ��Ă���*/

	//�v���C�f�[�^�Z�[�u
	_ROULETTE_TOOL		FR_ID_SAVE_REST_PLAY_DATA,0,0,FSW_ANSWER

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_ROULETTE_WORK_FREE

	_CALL				ev_roulette_save			/*07.08.24 _CALL�ɒu��������*/

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_TALK_CLOSE

	//���Z�b�g�R�}���h
	_ROULETTE_TOOL		FR_ID_SYSTEM_RESET,0,0,FSW_ANSWER
	_END

/*�u�������v*/
ev_roulette_room_rest_no:
	_JUMP				ev_roulette_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*							���^�C�A*/
/********************************************************************/
ev_room_choice_04:
	/*�u�o�g���L���b�X���̒���𒆎~����H�v*/
	_TALKMSG			msg_roulette_room_8
	_YES_NO_WIN_EX		FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_giveup_no

	/*�ʐM*/
	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_room_giveup_yes_multi_wait_msg

	/*�u���a���肵�Ă���������@���Ԃ����܂��I�v*/
	_TALKMSG			msg_roulette_room_34

	_JUMP				ev_roulette_room_giveup_yes
	_END

/*�u���΂炭���҂����������v*/
ev_roulette_room_giveup_yes_multi_wait_msg:
	_TALKMSG_NOSKIP		msg_roulette_room_wait
	_JUMP				ev_roulette_room_giveup_yes_multi_retry
	_END

ev_roulette_room_giveup_yes_multi_retry:
	_LDVAL				FSW_LOCAL2,ROULETTE_COMM_RETIRE
	_ROULETTE_SEND_BUF	FSW_LOCAL2,1,FSW_ANSWER									/*1=���^�C��*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_roulette_room_giveup_yes_multi_retry	/*���M���s�͍đ��M*/

	//_ROULETTE_RECV_BUF	FSW_LOCAL2
	//_COMM_RESET
	_JUMP				ev_roulette_room_giveup_yes_multi_sync
	_END

ev_roulette_room_giveup_yes_multi_sync:
	/*�u���a���肵�Ă���������@���Ԃ����܂��I�v*/
	_TALKMSG			msg_roulette_room_34

	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_roulette_room_wait

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_GIVE
	//_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_MENU
	_COMM_RESET
	_JUMP				ev_roulette_room_giveup_yes
	_END

/*�u�͂��v*/
ev_roulette_room_giveup_yes:
	//�s��p�����[�^�Z�b�g
	_ROULETTE_TOOL		FR_ID_SET_LOSE,0,0,FSW_ANSWER

	//_LDVAL			WK_SCENE_ROULETTE_LOBBY,3			/*���^�C������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_ROULETTE_LOBBY,3		/*���^�C������n�܂�悤�ɂ��Ă���*/

	_JUMP				ev_roulette_room_end_save
	_END
	_END

/*�u�������v*/
ev_roulette_room_giveup_no:
	_JUMP				ev_roulette_room_saikai		/*���j���[�֖߂�*/
	_END


/********************************************************************/
/*								�s�k*/
/********************************************************************/
ev_roulette_lose:
	/*������ROM���������T�[�o�[�o�[�W�����œ����Ă������́u���낭����v�o���Ȃ�*/
	_BATTLE_REC_SERVER_VER_CHK	FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,0,ev_roulette_lose_timing_sub

	/*�u�����قǂ́@�킢���@�L�^���܂����H�v*/
	_BATTLE_REC_DATA_OCC_CHECK	FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,0,ev_roulette_room_rec_msg1		/*�Ȃ���*/
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_roulette_room_rec_msg2		/*���鎞*/

	_YES_NO_WIN_EX	FSW_ANSWER								/*�������f�t�H���g*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_roulette_lose_rec_no	/*�u�������v*/
	_JUMP			ev_roulette_lose_rec_yes				/*�u�͂��v*/
	_END

ev_roulette_lose_timing_sub:
	_TALKMSG_NOSKIP	msg_roulette_room_wait
	_JUMP			ev_roulette_lose_timing_call
	_END

/*�u�͂��v*/
ev_roulette_lose_rec_yes:
	_CALL			ev_roulette_room_rec_lose				/*�s�k�̋L�^*/
	_JUMP			ev_roulette_lose_timing_call
	_END

/*�u�������v*/
ev_roulette_lose_rec_no:
	_JUMP			ev_roulette_lose_timing_call
	_END

ev_roulette_lose_timing_call:
	//�s��p�����[�^�Z�b�g
	_ROULETTE_TOOL		FR_ID_SET_LOSE,0,0,FSW_ANSWER

	//_LDVAL			WK_SCENE_ROULETTE_LOBBY,3			/*���^�C������n�܂�悤�ɂ��Ă���*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_ROULETTE_LOBBY,3		/*���^�C������n�܂�悤�ɂ��Ă���*/

	_JUMP			ev_roulette_lose_timing
	_END

ev_roulette_lose_timing:
	/*�u���a���肵�Ă���������@���Ԃ����܂��I�v*/
	_TALKMSG		msg_roulette_room_34

	_ROULETTE_TOOL	FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_roulette_multi_lose_timing
	_JUMP			ev_roulette_room_end_save
	_END

/*�ʐM����*/
ev_roulette_multi_lose_timing:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP	msg_roulette_room_wait

	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_LOSE_END
	_COMM_RESET
	_JUMP				ev_roulette_room_end_save
	_END

ev_roulette_room_end_save:
	_CALL				ev_roulette_save			/*07.08.24 _CALL�ɒu��������*/
	_TALK_CLOSE
	_JUMP				ev_roulette_room_end
	_END


/********************************************************************/
/*							���ʏI��								*/
/********************************************************************/
ev_roulette_room_end:

	/*�Z�[�u��ɓ���������*/
	_ROULETTE_TOOL	FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL		FSW_ANSWER,EQ,1,ev_roulette_multi_end_timing

	/*�t�F�[�h�A�E�g*/
	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*���̃^�C�~���O�ł�MINE�̃A�C�R�����o�Ă���\��*/
	_CALL				ev_roulette_poke_itemkeep_icon_del_mine	/*MINE�|�P�����A�C�R���폜*/
	//_CALL				ev_roulette_poke_itemkeep_icon_del		/*�|�P�����A�C�R���폜*/
	_CALL				ev_roulette_poke_itemkeep_icon_resource_del	/*���\�[�X�폜*/

	/*�|�P�p�l�����\�[�X�{�A�N�^�[�폜*/
	_CALL				ev_pokepanel_del

	_ROULETTE_TOOL		FR_ID_GET_TYPE,0,0,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,ROULETTE_TYPE_MULTI,ev_roulette_room_multi_tv

	_ROULETTE_TOOL		FR_ID_GET_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,ROULETTE_TYPE_WIFI_MULTI,ev_roulette_room_end_wifi

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_ROULETTE_WORK_FREE

	/*2D�}�b�v�T������*/
	//

	//_END
	_SCRIPT_FINISH			/*_END����2D�}�b�v�I��*/

/*�}���`�̂�*/
ev_roulette_room_multi_tv:
	/*TV����*/
	_TV_TEMP_FRIEND_SET	FRONTIER_NO_ROULETTE
	_RET


/********************************************************************/
/*						�Z�[�u��ɓ���������						*/	
/********************************************************************/
ev_roulette_multi_end_timing:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_ROULETTE_SAVE_AFTER
	_COMM_RESET			/*��ʐ؂�ւ��O�ɏ�����*/
	_RET


/********************************************************************/
/*						WIFI�͑҂����킹��ʂ֖߂�					*/	
/********************************************************************/
ev_roulette_room_end_wifi:
	/*�t�F�[�h�A�E�g*/
	//_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	//_WIPE_FADE_END_CHECK

	/*���[�N�J��*/
	_BATTLE_REC_EXIT
	_ROULETTE_WORK_FREE

	_MAP_CHANGE_EX		FSS_SCENEID_WIFI_COUNTER,1
	_END


/********************************************************************/
//							���ʃZ�[�u	
/********************************************************************/
ev_roulette_save:
	/*�u���|�[�g�ɏ�������ł��܂��@�d����؂�Ȃ��ŉ������v*/
	_TALKMSG_ALL_PUT	msg_roulette_room_36
	_ADD_WAITICON
	_REPORT_DIV_SAVE	FSW_ANSWER
	_DEL_WAITICON
	_SE_PLAY			SEQ_SE_DP_SAVE
	_SE_WAIT			SEQ_SE_DP_SAVE
	_RET


/********************************************************************/
/*(MINE)�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�ǉ��A���\�[�X�ǉ�(CALL)*/	
/********************************************************************/
ev_roulette_poke_itemkeep_icon_add_mine:
	/*�|�P�����A�C�R���A�A�C�e�������Ă��郊�\�[�X�Z�b�g*/
	_ROULETTE_TOOL		FR_ID_RESOURCE_SET_POKE_ICON,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_RESOURCE_SET_ITEMKEEP_ICON,0,0,FSW_ANSWER

	/*�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�ǉ�*/
	_ROULETTE_TOOL		FR_ID_ADD_ITEMKEEP_ICON,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ADD_POKE_ICON,0,0,FSW_ANSWER
	_RET


/********************************************************************/
/*(ENEMY)�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�ǉ�(CALL)		*/	
/********************************************************************/
ev_roulette_poke_itemkeep_icon_add_enemy:
	/*�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�ǉ�*/
	_ROULETTE_TOOL		FR_ID_ADD_ITEMKEEP_ICON,1,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ADD_POKE_ICON,1,0,FSW_ANSWER
	_RET


/********************************************************************/
/*(ALL)�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�ǉ��A���\�[�X�ǉ�(CALL)*/	
/********************************************************************/
ev_roulette_poke_itemkeep_icon_add:
	/*�|�P�����A�C�R���A�A�C�e�������Ă��郊�\�[�X�Z�b�g*/
	_ROULETTE_TOOL		FR_ID_RESOURCE_SET_POKE_ICON,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_RESOURCE_SET_ITEMKEEP_ICON,0,0,FSW_ANSWER

	/*�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�ǉ�*/
	_ROULETTE_TOOL		FR_ID_ADD_ITEMKEEP_ICON,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ADD_ITEMKEEP_ICON,1,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ADD_POKE_ICON,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_ADD_POKE_ICON,1,0,FSW_ANSWER
	_RET


/********************************************************************/
/*	(ALL)�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�폜(CALL)		*/	
/********************************************************************/
ev_roulette_poke_itemkeep_icon_del:
	/*�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�폜*/
	_ROULETTE_TOOL		FR_ID_DEL_ITEMKEEP_ICON,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_DEL_ITEMKEEP_ICON,1,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_DEL_POKE_ICON,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_DEL_POKE_ICON,1,0,FSW_ANSWER
	_RET


/********************************************************************/
/*	(ALL)�|�P�����A�C�R���A�A�C�e�������Ă��郊�\�[�X�폜(CALL)		*/	
/********************************************************************/
ev_roulette_poke_itemkeep_icon_resource_del:
	_ROULETTE_TOOL		FR_ID_RESOURCE_FREE_POKE_ICON,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_RESOURCE_FREE_ITEMKEEP_ICON,0,0,FSW_ANSWER
	_RET


/********************************************************************/
/*(MINE)�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�폜(CALL)		*/	
/********************************************************************/
ev_roulette_poke_itemkeep_icon_del_mine:
	/*�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�폜*/
	_ROULETTE_TOOL		FR_ID_DEL_ITEMKEEP_ICON,0,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_DEL_POKE_ICON,0,0,FSW_ANSWER
	_RET


/********************************************************************/
/*(ENEMY)�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�폜(CALL)		*/	
/********************************************************************/
ev_roulette_poke_itemkeep_icon_del_enemy:
	/*�|�P�����A�C�R���A�A�C�e�������Ă���A�N�^�[�폜*/
	_ROULETTE_TOOL		FR_ID_DEL_ITEMKEEP_ICON,1,0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_DEL_POKE_ICON,1,0,FSW_ANSWER
	_RET


/********************************************************************/
/*						�u���[���̗���								*/
/********************************************************************/
ev_roulette_leader_1st_01:
ev_roulette_leader_2nd_01:

	/*�L�^�������[�N�Ƀf�[�^���������܂�Ă��Ȃ� = ���f���A�Ƃ��čl����*/
	/*�o�ꂵ�܂��惁�b�Z�[�W�͕\�����Ȃ�*/
	_IFVAL_JUMP		FSW_LOCAL3,EQ,1,ev_roulette_room_saikai_00

	/*��x�u���[���o�ꃁ�b�Z�[�W��\��������*/
	_ROULETTE_TOOL	FR_ID_BRAIN_APPEAR_MSG_CHK,0,0,FSW_ANSWER
	_IFVAL_JUMP		FSW_ANSWER,EQ,1,ev_roulette_room_saikai_00

	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�u�u���[���Ƃ̏����ɂȂ�܂��I�o��͂����ł����H�v*/
	_TALKMSG			msg_roulette_room_boss_01
	_JUMP				ev_roulette_room_saikai_02
	_END

ev_roulette_leader_1st_02:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�o��*/
	_CALL				ev_roulette_leader_appear

	/*�퓬�O�̃��b�Z�[�W(1����)*/
	_TALKMSG			msg_roulette_room_boss_02
	_TALK_CLOSE
	_RET

ev_roulette_leader_2nd_02:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*�o��*/
	_CALL				ev_roulette_leader_appear

	/*�퓬�O�̃��b�Z�[�W(2����)*/
	_TALKMSG			msg_roulette_room_boss_03
	_TALK_CLOSE
	_RET

/*�o��*/
ev_roulette_leader_appear:

	_CALL				ev_roulette_actor_set_btl_people

	/*�ϋq ���S������*/
	_OBJ_ANIME			OBJID_PEOPLE1,people_anime_11			/*����*/
	_OBJ_ANIME			OBJID_PEOPLE2,people_anime_12			/*����*/
	_OBJ_ANIME			OBJID_PEOPLE3,people_anime_13
	_OBJ_ANIME			OBJID_PEOPLE4,people_anime_14
	_OBJ_ANIME			OBJID_PEOPLE5,people_anime_15
	//_OBJ_ANIME			OBJID_PEOPLE6,people_anime_16
	_OBJ_ANIME			OBJID_PEOPLE7,people_anime_17
	_OBJ_ANIME			OBJID_PEOPLE8,people_anime_18
	_OBJ_ANIME			OBJID_PEOPLE11,people_anime_11
	_OBJ_ANIME			OBJID_PEOPLE12,people_anime_12
	_OBJ_ANIME			OBJID_PEOPLE13,people_anime_13
	_OBJ_ANIME			OBJID_PEOPLE14,people_anime_13
	_OBJ_ANIME			OBJID_PEOPLE15,people_anime_18_2
	_OBJ_ANIME			OBJID_PEOPLE16,people_anime_11
	_OBJ_ANIME_WAIT

	_CALL				ev_roulette_trainer_set_brain
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain
	_OBJ_ANIME_WAIT

	/*�p�[�e�B�N���ǉ�(�u���[����p) 08.03.08*/
	_SE_PLAY			SEQ_SE_PL_FR04
	_PARTICLE_SPA_LOAD	SPAWORK_3, ROULETTE_BRAIN_SPA, FSS_CAMERA_ORTHO
	_PARTICLE_ADD_EMITTER	SPAWORK_3,ROULETTE_BRAIN_BR_MIRROR_YELLOW
	_PARTICLE_ADD_EMITTER	SPAWORK_3,ROULETTE_BRAIN_BR_MIRROR_YELLOWBG
	//_PARTICLE_WAIT

	/*�X�N���[��*/
	_OBJ_ANIME			OBJID_MINE,anm_mine_brain_down
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center_scroll_brain
	_OBJ_ANIME_WAIT

	/*����W�܂�*/
	_OBJ_ANIME			OBJID_PEOPLE1,people_anime_01			/*����*/
	_OBJ_ANIME			OBJID_PEOPLE2,people_anime_02			/*����*/
	_OBJ_ANIME			OBJID_PEOPLE3,people_anime_03
	_OBJ_ANIME			OBJID_PEOPLE4,people_anime_04
	_OBJ_ANIME			OBJID_PEOPLE5,people_anime_05
	//_OBJ_ANIME			OBJID_PEOPLE6,people_anime_06
	_OBJ_ANIME			OBJID_PEOPLE7,people_anime_07
	_OBJ_ANIME			OBJID_PEOPLE8,people_anime_08
	_OBJ_ANIME			OBJID_PEOPLE11,people_anime_01
	_OBJ_ANIME			OBJID_PEOPLE12,people_anime_02_2
	_OBJ_ANIME			OBJID_PEOPLE13,people_anime_03
	//_OBJ_ANIME			OBJID_PEOPLE14,people_anime_03		/*�������Ȃ�*/
	_OBJ_ANIME			OBJID_PEOPLE15,people_anime_15_5
	_OBJ_ANIME			OBJID_PEOPLE16,people_anime_16_5
	_OBJ_ANIME_WAIT

	_SE_PLAY			SEQ_SE_DP_DENDOU						/*����*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain_12
	_OBJ_ANIME_WAIT
	_SE_WAIT			SEQ_SE_DP_DENDOU

	/*�u���[���������܂ŕ���*/
	_OBJ_ANIME			OBJID_ETC,etc_anime_room_in_brain_2
	_OBJ_ANIME			OBJID_MINE,anm_mine_brain_down2
	_OBJ_ANIME			OBJID_PLAYER,anm_player_go_center_scroll_brain2
	_OBJ_ANIME			OBJID_PEOPLE1,people_anime_up_01
	_OBJ_ANIME			OBJID_PEOPLE2,people_anime_up_01
	_OBJ_ANIME			OBJID_PEOPLE3,people_anime_up_02
	_OBJ_ANIME			OBJID_PEOPLE4,people_anime_up_03
	_OBJ_ANIME			OBJID_PEOPLE5,people_anime_up_02
	//_OBJ_ANIME			OBJID_PEOPLE6,people_anime_up_01
	_OBJ_ANIME			OBJID_PEOPLE7,people_anime_up_02
	_OBJ_ANIME			OBJID_PEOPLE8,people_anime_up_01
	_OBJ_ANIME			OBJID_PEOPLE11,people_anime_up_03
	_OBJ_ANIME			OBJID_PEOPLE12,people_anime_up_01
	_OBJ_ANIME			OBJID_PEOPLE13,people_anime_up_01
	//_OBJ_ANIME			OBJID_PEOPLE14,people_anime_up_01
	_OBJ_ANIME			OBJID_PEOPLE15,people_anime_up_03
	_OBJ_ANIME			OBJID_PEOPLE16,people_anime_up_01
	_OBJ_ANIME_WAIT

	/*�ϋq�폜*/
	_CALL				ev_roulette_people_del

	_CALL				ev_roulette_poke_itemkeep_icon_add_enemy	/*(ENEMY)�|�P�����A�C�R���ǉ�*/
	_RET

ev_roulette_people_del:
	_ACTOR_FREE			OBJID_PEOPLE1
	_ACTOR_FREE			OBJID_PEOPLE2
	_ACTOR_FREE			OBJID_PEOPLE3
	_ACTOR_FREE			OBJID_PEOPLE4
	_ACTOR_FREE			OBJID_PEOPLE5
	//_ACTOR_FREE			OBJID_PEOPLE6
	_ACTOR_FREE			OBJID_PEOPLE7
	_ACTOR_FREE			OBJID_PEOPLE8
	_ACTOR_FREE			OBJID_PEOPLE11
	_ACTOR_FREE			OBJID_PEOPLE12
	_ACTOR_FREE			OBJID_PEOPLE13
	_ACTOR_FREE			OBJID_PEOPLE14
	_ACTOR_FREE			OBJID_PEOPLE15
	_ACTOR_FREE			OBJID_PEOPLE16
	_CHAR_RESOURCE_FREE	MAN1
	_CHAR_RESOURCE_FREE	BIGMAN
	_CHAR_RESOURCE_FREE	MOUNT
	_CHAR_RESOURCE_FREE	IDOL
	//_CHAR_RESOURCE_FREE CAMPBOY
	_RET


/********************************************************************/
/*						�u���[���ɏ������msg						*/
/********************************************************************/
ev_roulette_leader_1st_03:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_ROULETTE,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_NONE,ev_roulette_meory_print_put_1st

	/*�퓬��̃��b�Z�[�W(1����)*/
	_TALKMSG			msg_roulette_room_boss_04
	_TALK_CLOSE
	_JUMP				ev_roulette_battle_2_sub
	_END

/*��t��1st�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_roulette_meory_print_put_1st:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_ROULETTE,MEMORY_PRINT_PUT_1ST
	_RET

ev_roulette_leader_2nd_03:
	/*�A�������擾(FSW_ANSWER�g�p)*/
	_CALL				ev_roulette_rensyou_get
	_NUMBER_NAME		0,FSW_ANSWER

	/*���݂̋L�O�v�����g��Ԃ��擾*/
	_SAVE_EVENT_WORK_GET	SYS_WORK_MEMORY_PRINT_ROULETTE,FSW_ANSWER
	_IFVAL_CALL			FSW_ANSWER,EQ,MEMORY_PRINT_PUT_OK_1ST,ev_roulette_meory_print_put_2nd

	/*�퓬��̃��b�Z�[�W(2����)*/
	_TALKMSG			msg_roulette_room_boss_05
	_TALK_CLOSE
	_JUMP				ev_roulette_battle_2_sub
	_END

/*��t��2nd�L�O�v�����g��Ⴆ��悤�ɂ���*/
ev_roulette_meory_print_put_2nd:
	_SAVE_EVENT_WORK_SET	SYS_WORK_MEMORY_PRINT_ROULETTE,MEMORY_PRINT_PUT_2ND
	_RET


/********************************************************************/
/*				�|�P�p�l�����\�[�X�{�A�N�^�[�Z�b�g					*/
/********************************************************************/
ev_pokepanel_set:
	_ARTICLE_RESOURCE_SET	article_pokepanel_resource

	_ARTICLE_ACTOR_SET		article_pokepanel_single_l_actor
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_4,0		//0�Ԃ̃A�j�����s
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_5,4		//4�Ԃ̃A�j�����s

	_ARTICLE_ACTOR_SET		article_pokepanel_single_r_actor
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_6,1		//1�Ԃ̃A�j�����s
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_7,5		//5�Ԃ̃A�j�����s

#if 0
	/*�������ݒ�*/
	_ARTICLE_ACTOR_OBJ_MODE_SET	ACTWORK_4,1
	_ARTICLE_ACTOR_OBJ_MODE_SET	ACTWORK_5,1
	_ARTICLE_ACTOR_OBJ_MODE_SET	ACTWORK_6,1
	_ARTICLE_ACTOR_OBJ_MODE_SET	ACTWORK_7,1
#endif

	_RET

/********************************************************************/
/*				�|�P�p�l�����\�[�X�{�A�N�^�[�폜					*/
/********************************************************************/
ev_pokepanel_del:
	/*�|�P�p�l���A�N�^�[�폜*/
	_ARTICLE_ACTOR_FREE		ACTWORK_4
	_ARTICLE_ACTOR_FREE		ACTWORK_5
	_ARTICLE_ACTOR_FREE		ACTWORK_6
	_ARTICLE_ACTOR_FREE		ACTWORK_7

	/*���\�[�X���(�A�N�^�[�̓��[�N�ԍ��A���\�[�X��ACTID�ŉ�����鎖�ɒ���)*/
	_ARTICLE_RESOURCE_FREE	ACTID_ROULETTE_POKEPANEL
	_RET


/********************************************************************/
/*							�i��E������							*/
/********************************************************************/
ev_roulette_man_right:
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_80
	_OBJ_ANIME_WAIT
	_RET

ev_roulette_man_left:
	_OBJ_ANIME			OBJID_MAN,pcwoman2_anime_90
	_OBJ_ANIME_WAIT
	_RET


/********************************************************************/
/*					�A�������擾(FSW_ANSWER�g�p)					*/
/********************************************************************/
ev_roulette_rensyou_get:
	_ROULETTE_TOOL		FR_ID_GET_RENSYOU,0,0,FSW_ANSWER

	/*���ł�9999�̎��́A0 �I���W���̕␳�������Ȃ�*/
	_IFVAL_JUMP			FSW_ANSWER,GE,ROULETTE_RENSYOU_MAX,ev_roulette_rensyou_ret

	_ADD_WK				FSW_ANSWER,1
	_RET

ev_roulette_rensyou_ret:
	_RET


/********************************************************************/
/*						�p���b�g����(�Â�����)						*/
/********************************************************************/
ev_roulette_rouka_pal_black:
	_ROULETTE_TOOL		FR_ID_EFF_PAL_CHG,8,0,FSW_ANSWER
	_RET

ev_roulette_btl_pal_black:
	_ROULETTE_TOOL		FR_ID_EFF_PAL_CHG,8,1,FSW_ANSWER
	_RET

/********************************************************************/
/*						�p���b�g����(���ɖ߂�)						*/
/********************************************************************/
ev_roulette_btl_pal_white:
	_ROULETTE_TOOL		FR_ID_EFF_PAL_CHG,0,1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,0,0,FSW_ANSWER	/*OBJ*/
	_RET


/********************************************************************/
/*���[���b�g��ʁA�퓬��ʂ��畜�A��������OBJ�p���b�g����(�Â�����)	*/
//
//	�i��
//	�ΐ�g���[�i�[(�p�[�g�i�[)
//	��l��(�p�[�g�i�[)
/********************************************************************/
ev_roulette_obj_pal_black:
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_MAN,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_ETC,FSW_ANSWER

	_ROULETTE_TOOL		FR_ID_CHECK_COMM_TYPE,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_roulette_obj_pal_black_multi

	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_MINE,FSW_ANSWER
	_RET

ev_roulette_obj_pal_black_multi:
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_SIO_USER_0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_SIO_USER_1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_ETC2,FSW_ANSWER
	_RET


/********************************************************************/
/*			���\�[�X�o�^�ƃA�N�^�[�o�^�ƃp���b�g����				*/
/********************************************************************/
ev_roulette_actor_set_way:
	_CHAR_RESOURCE_SET	default_set_resource			/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			way_set_actor					/*�A�N�^�[�o�^*/
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PCWOMAN2,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_MINE,FSW_ANSWER
	_RET

ev_roulette_actor_set_way_multi:
	_CHAR_RESOURCE_SET	default_set_resource_multi		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			way_set_actor_multi				/*�A�N�^�[�o�^*/
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PCWOMAN2,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_SIO_USER_0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_SIO_USER_1,FSW_ANSWER
	_RET

ev_roulette_actor_set_btl:
	_CHAR_RESOURCE_SET	default_scene2_resource			/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_scene2_actor			/*�A�N�^�[�o�^*/
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_MINE,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_MAN,FSW_ANSWER
	_RET

ev_roulette_actor_set_btl_multi:
	_CHAR_RESOURCE_SET	default_scene2_resource_multi	/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			default_scene2_actor_multi		/*�A�N�^�[�o�^*/
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_MAN,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_SIO_USER_0,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_SIO_USER_1,FSW_ANSWER
	_RET

ev_roulette_actor_set_btl_pcwoman:
	_CHAR_RESOURCE_SET	pcwoman2_scene2_resource		/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			pcwoman2_scene2_actor			/*�A�N�^�[�o�^*/
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PCWOMAN2,FSW_ANSWER
	_RET

ev_roulette_actor_set_btl_people:
	_CHAR_RESOURCE_SET	people_scene_resource			/*�ϋq*/
	_ACTOR_SET			default_scene3_actor			/*�ϋq�A�N�^�[�o�^*/
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE1,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE2,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE3,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE4,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE5,FSW_ANSWER
	//_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE6,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE7,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE8,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE11,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE12,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE13,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE14,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE15,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_PEOPLE16,FSW_ANSWER
	_RET

ev_roulette_actor_set_btl_tr:
	_CHAR_RESOURCE_SET	etc_set_resource
	_ACTOR_SET			etc_set_actor
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_ETC,FSW_ANSWER
	_RET

ev_roulette_actor_set_btl_tr_multi:
	_CHAR_RESOURCE_SET	etc_set_resource2
	_ACTOR_SET			etc_set_actor2
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_ETC,FSW_ANSWER
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_ETC2,FSW_ANSWER
	_RET

ev_roulette_actor_set_btl_kaihuku:
	_CHAR_RESOURCE_SET	kaihuku_set_resource			/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			kaihuku_set_actor				/*�A�N�^�[�o�^*/
	_ROULETTE_TOOL		FR_ID_OBJ_PAL_CHG,8,OBJID_KAIHUKU,FSW_ANSWER
	_RET



//==============================================================================
/**
 * @file	fr_wifi.s
 * @brief	�u�o�g���t�����e�B�AWiFI��t�v
 * @author	nohara
 * @date	2007.07.27
 */
//==============================================================================

	.text

	.include	"../frontier_seq_def.h"
	.include	"../../../include/field/evwkdef.h"
	.include	"../../fielddata/script/saveflag.h"
	.include	"../../fielddata/script/savework.h"
	.include	"../../../include/msgdata/msg_bf_info.h"
	.include	"../frontier_def.h"
	.include	"../factory_def.h"
	.include	"../stage_def.h"
	.include	"../castle_def.h"
	.include	"../roulette_def.h"
	.include	"../../field/syswork_def.h"

//--------------------------------------------------------------------
//
//					     �X�N���v�g�{��
//
//	FSW_PARAM0		�{��No
//	FSW_PARAM1		�ʐM�^�C�v
//	FSW_PARAM2		�X�e�[�W�v�Z�p�A���������V�Ԃ�
//	FSW_PARAM3		***
//	FSW_PARAM6		�F�B�ƘA�����H������Ă��邩�̌���
//	FSW_LOCAL0		***
//	FSW_LOCAL1		***
//	FSW_LOCAL2		�I�������莝��pos1(LOCALWORK2�ɃR�s�[)
//	FSW_LOCAL3		init
//	FSW_LOCAL4		��߂�
//	FSW_LOCAL5		�I�������莝��pos2(LOCALWORK5�ɃR�s�[)
//	SYS_WORK_WIFI_FR_CLEAR_FLAG	�N���A�t���O���擾
//
//--------------------------------------------------------------------
_EVENT_DATA		fss_wifi_counter		//��ԏ��EVENT_DATA�͎������s
_EVENT_DATA		fss_wifi_counter_return	//�{�݂���߂��Ă�����
_EVENT_DATA_END							//�I��


//--------------------------------------------------------------------
//							���W��`
//--------------------------------------------------------------------
//#define PLAYER_X		(8*16)
//#define PLAYER_X		(8*13)
#define PLAYER_X		(8*14)
#define PLAYER_Y		(8*14)
//#define PCWOMAN2_01_X	(8*16)
//#define PCWOMAN2_01_Y	(8*10)
//#define SIO_USER_0_X	(8*16)
#define SIO_USER_0_X	(8*14)
//#define SIO_USER_0_Y	(8*14)
#define SIO_USER_0_Y	(8*8)
//#define SIO_USER_1_X	(8*18)
#define SIO_USER_1_X	(8*16)
//#define SIO_USER_1_Y	(8*14)
#define SIO_USER_1_Y	(8*8)


/********************************************************************/
//
//	SCENEID_WIFI_COUNTER:���\�[�X���x��(���IN���ɏ풓�����郊�\�[�X�Q)
//
/********************************************************************/
//�ʐM�}���`
_RESOURCE_LABEL	comm_multi_set_resource
	_PLAYER_RESOURCE_DATA		//�������g(HEROorHEROINE)�̃L�����Z�b�g
	_SIO_PLAYER_RESOURCE_DATA	//�ʐM�v���C���[�S���̃L�����Z�b�g
	//_CHAR_RESOURCE_DATA		PCWOMAN2,WF2DC_C_MOVENORMAL
	_CHAR_RESOURCE_DATA_END


//====================================================================
//	SCENEID_WIFI_COUNTER:�A�N�^�[(���IN���ɏ풓������A�N�^�[�Q)
//====================================================================
#define OBJID_PLAYER				(0)
//#define OBJID_PCWOMAN2_01			(1)
#define OBJID_SIO_USER_0			(2)
#define OBJID_SIO_USER_1			(3)

//�X�N���v�gID(����Ή��\��)
#define EVENTID_TEST_SCR_OBJ1		(1)

//�ʐM�}���`
_ACTOR_LABEL	comm_multi_set_actor
	_PLAYER_ACTOR_DATA		OBJID_PLAYER,WF2DMAP_WAY_C_DOWN, \
							PLAYER_X,PLAYER_Y,OFF	/*�������g�̃A�N�^�[�Z�b�g*/
	//_ACTOR_DATA				OBJID_PCWOMAN2_01,PCWOMAN2,WF2DMAP_WAY_C_DOWN, \
	//						PCWOMAN2_01_X,PCWOMAN2_01_Y,ON,EVENTID_TEST_SCR_OBJ1
	_SIO_PLAYER_ACTOR_DATA	0,OBJID_SIO_USER_0,WF2DMAP_WAY_C_UP, \
							SIO_USER_0_X,SIO_USER_0_Y,ON
	_SIO_PLAYER_ACTOR_DATA	1,OBJID_SIO_USER_1,WF2DMAP_WAY_C_UP, \
							SIO_USER_1_X,SIO_USER_1_Y,ON
	_ACTOR_DATA_END


/********************************************************************/
/*								�J�n								*/
/********************************************************************/
fss_wifi_counter:

	/*��*/
	_PC_KAIFUKU

	/*BGM�̃t�F�[�h�����Ȃ��ƃ_������*/
	_BGM_PLAY			SEQ_BF_TOWWER

	_FR_WIFI_COUNTER_WORK_ALLOC
	//_FR_WIFI_COUNTER_WORK_FREE

	_TIME_WAIT			1,FSW_ANSWER
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FR_WIFI_COUNTER_INIT_1
	_COMM_RESET

	_CHAR_RESOURCE_SET	comm_multi_set_resource				/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			comm_multi_set_actor				/*�A�N�^�[�o�^*/
	_JUMP				tower_wayd_common2
	_END

tower_wayd_common2:
	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_JUMP				bf_no_sel_menu
	_END

bf_no_sel_menu:
	/*�u�ǂ̎{�݂ɒ��킵�܂����H�v*/
	_TALKMSG			msg_bf_info_001

	/*���j���[�쐬*/
	_BMPLIST_INIT_EX	18,5,0,1,FSW_ANSWER									/*B�L�����Z���L��*/
	_BMPLIST_MAKE_LIST	msg_bf_info_choice_01,FSEV_WIN_TALK_MSG_NONE,FRONTIER_NO_TOWER			/*�o�g���^���[*/
	_BMPLIST_MAKE_LIST	msg_bf_info_choice_02,FSEV_WIN_TALK_MSG_NONE,FRONTIER_NO_FACTORY_LV50	/*�o�g���t�@�N�g���[*/
	_BMPLIST_MAKE_LIST	msg_bf_info_choice_03,FSEV_WIN_TALK_MSG_NONE,FRONTIER_NO_CASTLE			/*�o�g���L���b�X��*/
	_BMPLIST_MAKE_LIST	msg_bf_info_choice_04,FSEV_WIN_TALK_MSG_NONE,FRONTIER_NO_STAGE			/*�o�g���X�e�[�W*/
	_BMPLIST_MAKE_LIST	msg_bf_info_choice_05,FSEV_WIN_TALK_MSG_NONE,FRONTIER_NO_ROULETTE		/*�o�g�����[���b�g*/
	_BMPLIST_MAKE_LIST	msg_bf_info_choice_06,FSEV_WIN_TALK_MSG_NONE,FRONTIER_NO_YAMERU			/*��߂�*/
	_BMPLIST_START

	_IFVAL_JUMP		FSW_ANSWER,EQ,FRONTIER_NO_TOWER,ev_bf_info_choice_01	/*�o�g���^���[*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,FRONTIER_NO_FACTORY_LV50,ev_bf_info_choice_02	/*�t�@�N�g���[*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,FRONTIER_NO_CASTLE,ev_bf_info_choice_03	/*�o�g���L���b�X��*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,FRONTIER_NO_STAGE,ev_bf_info_choice_04	/*�o�g���X�e�[�W*/
	_IFVAL_JUMP		FSW_ANSWER,EQ,FRONTIER_NO_ROULETTE,ev_bf_info_choice_05	/*�o�g�����[���b�g*/
	_JUMP			ev_bf_info_choice_06									/*��߂�*/
	_END


/********************************************************************/
/*							�o�g���^���[							*/
/********************************************************************/
ev_bf_info_choice_01:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_SET_BF_NO,FRONTIER_NO_TOWER,0,FSW_ANSWER
	_LDVAL					FSW_LOCAL4,0
	_LDVAL					FSW_PARAM0,FRONTIER_NO_TOWER
	//_SAVE_EVENT_WORK_SET	WK_SCENE_TOWER_TYPE,TOWER_TYPE_WIFI_MULTI		/*type*/
	_SAVE_EVENT_WORK_SET	LOCALWORK3,0									/*init*/
	_JUMP					ev_bf_info_send_retire_flag
	_END


/********************************************************************/
/*							�o�g���t�@�N�g���[						*/
/********************************************************************/
ev_bf_info_choice_02:
	_SAVE_EVENT_WORK_SET	WK_SCENE_FACTORY_TYPE,FACTORY_TYPE_WIFI_MULTI	/*type*/
	_SAVE_EVENT_WORK_SET	LOCALWORK3,0									/*init*/
	_JUMP					ev_bf_info_choice_factory_level
	_END

ev_bf_info_choice_factory_level:
	/*�u���x���T�O�Ɓ@�I�[�v�����x���@�ǂ���ɒ��킷��H�v*/
	_TALKMSG			msg_bf_info_020

	_BMPMENU_INIT_EX	20,11,0,1,FSW_ANSWER			/*B�L�����Z���L��*/
	_BMPMENU_MAKE_LIST	msg_bf_info_choice_07,0			/*���x���T�O*/
	_BMPMENU_MAKE_LIST	msg_bf_info_choice_08,1			/*�I�[�v�����x��*/
	_BMPMENU_MAKE_LIST	msg_bf_info_choice_06,2			/*��߂�*/
	_BMPMENU_START

	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_bf_info_choice_07_level
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_bf_info_choice_08_level
	_JUMP				bf_no_sel_menu					/*�{�ݑI����*/
	_END

ev_bf_info_choice_07_level:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_SET_BF_NO,FRONTIER_NO_FACTORY_LV50,0,FSW_ANSWER
	_LDVAL					FSW_LOCAL4,0
	_LDVAL					FSW_PARAM0,FRONTIER_NO_FACTORY_LV50

	//_FR_WIFI_COUNTER_TOOL	FWIFI_ID_SET_LEVEL,50,0,FSW_ANSWER
	_SAVE_EVENT_WORK_SET	WK_SCENE_FACTORY_LEVEL,FACTORY_LEVEL_50			/*level*/
	_JUMP				ev_bf_info_send_retire_flag
	_END

ev_bf_info_choice_08_level:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_SET_BF_NO,FRONTIER_NO_FACTORY_LV100,0,FSW_ANSWER
	_LDVAL					FSW_LOCAL4,0
	_LDVAL					FSW_PARAM0,FRONTIER_NO_FACTORY_LV100

	//_FR_WIFI_COUNTER_TOOL	FWIFI_ID_SET_LEVEL,100,0,FSW_ANSWER
	_SAVE_EVENT_WORK_SET	WK_SCENE_FACTORY_LEVEL,FACTORY_LEVEL_OPEN		/*level*/
	_JUMP				ev_bf_info_send_retire_flag
	_END


/********************************************************************/
/*							�o�g���L���b�X��						*/
/********************************************************************/
ev_bf_info_choice_03:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_SET_BF_NO,FRONTIER_NO_CASTLE,0,FSW_ANSWER
	_LDVAL					FSW_LOCAL4,0
	_LDVAL					FSW_PARAM0,FRONTIER_NO_CASTLE
	_SAVE_EVENT_WORK_SET	WK_SCENE_CASTLE_TYPE,CASTLE_TYPE_WIFI_MULTI		/*type*/
	_SAVE_EVENT_WORK_SET	LOCALWORK3,0									/*init*/
	_JUMP					ev_bf_info_send_retire_flag
	_END


/********************************************************************/
/*							�o�g���X�e�[�W							*/
/********************************************************************/
ev_bf_info_choice_04:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_SET_BF_NO,FRONTIER_NO_STAGE,0,FSW_ANSWER
	_LDVAL					FSW_LOCAL4,0
	_LDVAL					FSW_PARAM0,FRONTIER_NO_STAGE
	_SAVE_EVENT_WORK_SET	WK_SCENE_STAGE_TYPE,STAGE_TYPE_WIFI_MULTI		/*type*/
	_SAVE_EVENT_WORK_SET	LOCALWORK3,0									/*init(08.06.28)*/
	_JUMP					ev_bf_info_send_retire_flag
	_END


/********************************************************************/
/*							�o�g�����[���b�g						*/
/********************************************************************/
ev_bf_info_choice_05:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_SET_BF_NO,FRONTIER_NO_ROULETTE,0,FSW_ANSWER
	_LDVAL					FSW_LOCAL4,0
	_LDVAL					FSW_PARAM0,FRONTIER_NO_ROULETTE
	_SAVE_EVENT_WORK_SET	WK_SCENE_ROULETTE_TYPE,ROULETTE_TYPE_WIFI_MULTI	/*type*/
	_SAVE_EVENT_WORK_SET	LOCALWORK3,0									/*init*/
	_JUMP					ev_bf_info_send_retire_flag
	_END


/********************************************************************/
/*								��߂�								*/
/********************************************************************/
ev_bf_info_choice_06:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_SET_BF_NO,FRONTIER_NO_YAMERU,0,FSW_ANSWER
	_LDVAL					FSW_LOCAL4,1
	_JUMP					ev_bf_info_send_retire_flag
	_END


/********************************************************************/
/*						��߂��I�񂾂����M						*/
/********************************************************************/
ev_bf_info_send_retire_flag:
	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_bf_info_005
	_JUMP				ev_bf_info_send_retire_flag_retry
	_END

ev_bf_info_send_retire_flag_retry:
	_LDVAL				FSW_PARAM1,FRONTIER_COMM_YAMERU
	_FR_WIFI_COUNTER_SEND_BUF	FSW_PARAM1,FSW_LOCAL4,0,FSW_ANSWER			/*1=��߂�*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_bf_info_send_retire_flag_retry	/*���M���s�͍đ��M*/

	_FR_WIFI_COUNTER_RECV_BUF	FSW_PARAM1
	_COMM_RESET

	/*�������u��߂�v�I�񂾂�*/
	//_IFVAL_JUMP			FSW_LOCAL4,EQ,1,ev_bf_info_game_end
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_BF_NO,0,0,FSW_ANSWER
	_IFVAL_JUMP				FSW_ANSWER,EQ,FRONTIER_NO_YAMERU,ev_bf_info_game_end

	/*���肪�I�񂾃��j���[���擾*/
	_FR_WIFI_COUNTER_YAMERU_CHECK	FSW_ANSWER

	/*�p�[�g�i�[���u��߂�v��I�񂾂�*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,bf_no_sel_yameru

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FR_WIFI_COUNTER_YAMERU
	_COMM_RESET

	_JUMP				ev_bf_info_sel_check
	_END


/********************************************************************/
/*						��߂邪�I�΂ꂽ							*/
/********************************************************************/
bf_no_sel_yameru:
	_JUMP				ev_bf_info_game_end_pair
	_END


/********************************************************************/
/*					�I�������{�݂��������`�F�b�N					*/
/********************************************************************/
ev_bf_info_sel_check:
	_LDVAL				FSW_PARAM1,FRONTIER_COMM_SEL
	_FR_WIFI_COUNTER_SEND_BUF	FSW_PARAM1,0,0,FSW_ANSWER				/*�{��No*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_bf_info_sel_check			/*���M���s�͍đ��M*/

	_FR_WIFI_COUNTER_RECV_BUF	FSW_PARAM1
	_COMM_RESET

	/*���݂����I�񂾎{�݃i���o�[���r*/
	_FR_WIFI_COUNTER_BFNO_CHECK	FSW_PARAM0,FSW_ANSWER

	_IFVAL_JUMP			FSW_ANSWER,EQ,1,ev_bf_info_sel_ok				/*OK*/

	/*�u�I�񂾎{�݂�����ƈႢ�܂��v*/
	_TALKMSG			msg_bf_info_004
	_JUMP				bf_no_sel_menu									/*�{�ݑI����*/
	_END


/********************************************************************/
/*						�I�������{�݂�����							*/
/********************************************************************/
ev_bf_info_sel_ok:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_BF_NO,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,FRONTIER_NO_TOWER,ev_bf_info_pokelist_start	/*�^���[*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,FRONTIER_NO_STAGE,ev_bf_info_pokelist_start	/*�X�e�[�W*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,FRONTIER_NO_CASTLE,ev_bf_info_pokelist_start	/*�L���b�X��*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,FRONTIER_NO_ROULETTE,ev_bf_info_pokelist_start/*���[���b�g*/
	_JUMP				ev_bf_info_pokelist_after	/*���X�g�Ăяo���̕K�v�Ȃ�*/
	_END


/********************************************************************/
/*						�|�P�������X�g�Ăяo��						*/
/********************************************************************/
ev_bf_info_pokelist_start:
	/*�u�Q������|�P������I��ł��������v*/
	_TALKMSG			msg_bf_info_003

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_TALK_CLOSE

	/*�|�P�������X�g�I�����̃N���A*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_POKELIST_SEL_CLEAR,0,0,FSW_ANSWER

	/*�|�P�������X�g�Ăяo��*/
	_JUMP				ev_bf_info_pokelist_call
	_END

/*�|�P�����X�e�[�^�X*/
ev_bf_info_pokestatus_call:
	_FR_WIFI_COUNTER_POKELIST	FSW_PARAM0,FR_WIFI_POKESEL_PST_CALL,FSW_ANSWER
	_FR_WIFI_COUNTER_POKELIST	FSW_PARAM0,FR_WIFI_POKESEL_PST_WAIT,FSW_ANSWER
	_JUMP						ev_bf_info_pokelist_call
	_END

/*�|�P�������X�g*/
ev_bf_info_pokelist_call:
	_FR_WIFI_COUNTER_POKELIST	FSW_PARAM0,FR_WIFI_POKESEL_PLIST_CALL,FSW_ANSWER
	_FR_WIFI_COUNTER_POKELIST	FSW_PARAM0,FR_WIFI_POKESEL_PLIST_WAIT,FSW_ANSWER
	_IFVAL_JUMP					FSW_ANSWER,EQ,FR_WIFI_POKESEL_PST_CALL,ev_bf_info_pokestatus_call

	/*�I�������|�P�����̕��єԍ��擾*/
	_FR_WIFI_COUNTER_POKELIST_GET_RESULT	FSW_LOCAL2,FSW_LOCAL5
	_SAVE_EVENT_WORK_SET	LOCALWORK2,FSW_LOCAL2						/*pos1*/
	_SAVE_EVENT_WORK_SET	LOCALWORK5,FSW_LOCAL5						/*pos2*/

	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_JUMP				ev_bf_info_send_pokelist_after
	_END

/********************************************************************/
/*				�|�P�������X�g�̌��ʂ𑗎�M						*/
/********************************************************************/
ev_bf_info_send_pokelist_after:

	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_bf_info_005
	_JUMP				ev_bf_info_send_pokelist_after2
	_END

ev_bf_info_send_pokelist_after2:
	/*�莝���ʒu�𑗐M���āA�|�P�����i���o�[�ƃA�C�e���i���o�[���`�F�b�N*/
	/*���X�g�ł�߂��I�񂾂����`�F�b�N*/
	_LDVAL				FSW_PARAM1,FRONTIER_COMM_MONSNO_ITEMNO
	_FR_WIFI_COUNTER_SEND_BUF	FSW_PARAM1,FSW_LOCAL2,FSW_LOCAL5,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_bf_info_send_pokelist_after2		/*���M���s�͍đ��M*/

	_FR_WIFI_COUNTER_RECV_BUF	FSW_PARAM1
	_COMM_RESET

	/*�L�����Z���`�F�b�N*/
	_IFVAL_JUMP			FSW_LOCAL2,EQ,0xff,bf_no_sel_menu				/*�{�ݑI����*/

	/*�p�[�g�i�[�̃L�����Z���`�F�b�N*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_PAIR_POKELIST_CANCEL,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_bf_info_pokelist_pair_cancel

	/*�莝���[���|�P�����ԍ��ϊ��@�����Ƀ^�}�S�`�F�b�N���s���Ă��܂�*/
	//_TEMOTI_MONSNO		FSW_LOCAL2,FSW_LOCAL1
	//_IFVAL_JUMP			FSW_LOCAL1,EQ,0,ev_lobby_choice_04			/*�^�}�S*/
	/*�|�P�������X�g�Ń^�}�S���I���o���Ȃ��̂���*/

	/*�{�݂��Ƃɕ���*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_BF_NO,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,FRONTIER_NO_TOWER,ev_bf_info_pokelist_tower_result
	_IFVAL_JUMP			FSW_ANSWER,EQ,FRONTIER_NO_STAGE,ev_bf_info_pokelist_stage_result
	_IFVAL_JUMP			FSW_ANSWER,EQ,FRONTIER_NO_CASTLE,ev_bf_info_pokelist_castle_result
	_IFVAL_JUMP			FSW_ANSWER,EQ,FRONTIER_NO_ROULETTE,ev_bf_info_pokelist_roulette_result
	_END


/********************************************************************/
/*					�p�[�g�i�[�̃L�����Z���`�F�b�N					*/
/********************************************************************/
ev_bf_info_pokelist_pair_cancel:
	/*�u������̓s���������悤�ł��v*/
	_PAIR_NAME			0
	_TALKMSG			msg_bf_info_002
	_JUMP				bf_no_sel_menu					/*�{�ݑI����*/
	_END


/********************************************************************/
/*					�^���[�̃|�P�������X�g���ʂ��`�F�b�N			*/
/********************************************************************/
ev_bf_info_pokelist_tower_result:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_CHECK_ENTRY,FRONTIER_NO_TOWER,0,FSW_ANSWER
	_SWITCH				FSW_ANSWER
	_CASE_JUMP			1,ev_wifi_tower_gate_connect_poke_same01	/*1�C�ڂ����*/
	_CASE_JUMP			2,ev_wifi_tower_gate_connect_poke_same02	/*2�C�ڂ����*/
	_CASE_JUMP			3,ev_wifi_tower_gate_connect_poke_same03	/*1,2�C�Ƃ����*/
	_JUMP				ev_bf_info_pokelist_after
	_END


/********************************************************************/
/*					�^���[�@�|�P����������Ă���					*/
/********************************************************************/
/*�|�P��������C����Ă���Ƃ�(��C��)*/
ev_wifi_tower_gate_connect_poke_same01:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		0,FSW_ANSWER,0,0
	_TALKMSG				msg_bf_info_010
	_JUMP					bf_no_sel_menu					/*�{�ݑI����*/
	_END

/*�|�P��������C����Ă���Ƃ�(��C��)*/
ev_wifi_tower_gate_connect_poke_same02:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,1,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		0,FSW_ANSWER,0,0
	_TALKMSG				msg_bf_info_010
	_JUMP					bf_no_sel_menu					/*�{�ݑI����*/
	_END

/*�|�P��������C����Ă���Ƃ�*/
ev_wifi_tower_gate_connect_poke_same03:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		0,FSW_ANSWER,0,0

	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,1,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		1,FSW_ANSWER,0,0

	_TALKMSG				msg_bf_info_011
	_JUMP					bf_no_sel_menu					/*�{�ݑI����*/
	_END


/********************************************************************/
/*				�X�e�[�W�̃|�P�������X�g���ʂ��`�F�b�N				*/
/********************************************************************/
ev_bf_info_pokelist_stage_result:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_CHECK_ENTRY,FRONTIER_NO_STAGE,0,FSW_ANSWER
	_IFVAL_JUMP				FSW_ANSWER,EQ,1,ev_wifi_stage_gate_connect_poke_same01	/*�Ⴄmonsno*/

#if 0
	���肪�L�^�������Ē��킵�Ă���ƁA

	�����Ă��Ȃ����́A�A�����ŁA
	���������́A�A������ɂȂ�̂ŁA

	�A�����̃|�P���������邩�Ȃǂ̃`�F�b�N�̑O�ɁA
	�A�������H������Ă��邩���`�F�b�N����K�v������

	�F�B�̓C���f�b�N�X�ƃv���t�@�C��ID������̂ŁA
	�ēx�����C���f�b�N�X�ɓ����F�B��o�^���Ă��A
	�Ⴄ�v���t�@�C��ID�ɂȂ�
#endif

	/*�X�e�[�W�̂ݐ�ɁA�A�������H������Ă��邩�`�F�b�N*/
	_CALL				ev_bf_info_pair_rensyou_ng_check_call
	_IFVAL_JUMP			FSW_PARAM6,EQ,0,ev_bf_info_pokelist_after_sub	/*�H������Ă�����V�K����*/

	/*�A�������`�F�b�N*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_STAGE_GET_CLEAR_FLAG,0,0,FSW_ANSWER
	_IFVAL_JUMP				FSW_ANSWER,EQ,0,ev_bf_info_pokelist_after				/*�V�K����*/

	/*�A�����̃|�P�����i���o�[�ƈ�v���邩�`�F�b�N*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_STAGE_GET_CLEAR_MONSNO,0,0,FSW_PARAM2
	_POKEMON_NAME_EXTRA		0,FSW_PARAM2,0,0

	/*�o��|�P�����i���o�[�擾*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		1,FSW_ANSWER,0,0
	_IFWK_JUMP				FSW_ANSWER,EQ,FSW_PARAM2,ev_bf_info_pokelist_after		/*�A����*/

	/*�A�����ŁA�V�K�|�P�����Œ��킵�悤�Ƃ�����*/
	_JUMP			ev_bf_info_stage_new
	_END


/********************************************************************/
/*					�X�e�[�W�@�L�^�����邯�ǂǂ�����H				*/
/********************************************************************/
ev_bf_info_stage_new:
	/*�u���ł̘A���L�^������܂����A�L�^�������ā��ŐV���ɒ��킵�܂����H�v*/
	_TALKMSG			msg_bf_info_025
	_BMPMENU_INIT_EX	25,13,1,1,FSW_ANSWER			/*B�L�����Z���L��*/
	_BMPMENU_MAKE_LIST	msg_bf_info_025_01,0			/*�͂�*/
	_BMPMENU_MAKE_LIST	msg_bf_info_025_02,1			/*������*/
	_BMPMENU_START

	/*�u����̑I����҂��Ă��܂��I���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_bf_info_031

	_LDWK				FSW_PARAM2,FSW_ANSWER			/*�ޔ�*/
	_JUMP				ev_bf_info_stage_new_retry
	_END

ev_bf_info_stage_new_retry:
	_LDVAL				FSW_PARAM1,FRONTIER_COMM_STAGE_RECORD_DEL
	_FR_WIFI_COUNTER_SEND_BUF	FSW_PARAM1,FSW_PARAM2,0,FSW_ANSWER		/*0=�͂��A1=������*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_bf_info_stage_new_retry		/*���M���s�͍đ��M*/

	_FR_WIFI_COUNTER_RECV_BUF	FSW_PARAM1
	_COMM_RESET

	/*���肪�u�������v��I�񂾎�*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_PAIR_STAGE_RECORD_DEL,0,0,FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,1,bf_no_sel_yameru_stage
	_IFVAL_JUMP			FSW_ANSWER,EQ,FSEV_WIN_B_CANCEL,bf_no_sel_yameru_stage

	/*�������u�������v��I�񂾎�*/
	_IFVAL_JUMP			FSW_PARAM2,EQ,1,bf_no_sel_yameru_stage
	_IFVAL_JUMP			FSW_PARAM2,EQ,FSEV_WIN_B_CANCEL,bf_no_sel_yameru_stage

	/*���݂����u�͂��v��I��*/
	_JUMP				ev_bf_info_stage_new_yes		/*�͂�*/
	_END

/*�u������x�I�����Ă��������v*/
bf_no_sel_yameru_stage:
	_TALKMSG			msg_bf_info_026
	_JUMP				bf_no_sel_menu					/*�{�ݑI����*/
	_END


/********************************************************************/
/*					�X�e�[�W�@�L�^�������@�͂�						*/
/********************************************************************/
ev_bf_info_stage_new_yes:
	/*�^�C�v���Ƃ̃��x���A7�A���������t���O�̃N���A�A���݂̘A�������N���A*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_STAGE_SET_NEW_CHALLENGE,0,0,FSW_ANSWER
	_JUMP					ev_bf_info_pokelist_after
	_END


/********************************************************************/
/*					�X�e�[�W�@�|�P�����������łȂ�					*/
/********************************************************************/
ev_wifi_stage_gate_connect_poke_same01:
	_TALKMSG			msg_bf_info_015
	_JUMP				bf_no_sel_menu					/*�{�ݑI����*/
	_END


/********************************************************************/
/*				�L���b�X���̃|�P�������X�g���ʂ��`�F�b�N			*/
/********************************************************************/
ev_bf_info_pokelist_castle_result:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_CHECK_ENTRY,FRONTIER_NO_CASTLE,0,FSW_ANSWER
	_SWITCH				FSW_ANSWER
	_CASE_JUMP			1,ev_wifi_castle_gate_connect_poke_same01	/*1�C�ڂ����*/
	_CASE_JUMP			2,ev_wifi_castle_gate_connect_poke_same02	/*2�C�ڂ����*/
	_CASE_JUMP			3,ev_wifi_castle_gate_connect_poke_same03	/*1,2�C�Ƃ����*/
	_JUMP				ev_bf_info_pokelist_after
	_END


/********************************************************************/
/*					�L���b�X���@�|�P����������Ă���				*/
/********************************************************************/
/*�|�P��������C����Ă���Ƃ�(��C��)*/
ev_wifi_castle_gate_connect_poke_same01:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		0,FSW_ANSWER,0,0
	_TALKMSG				msg_bf_info_013
	_JUMP					bf_no_sel_menu					/*�{�ݑI����*/
	_END

/*�|�P��������C����Ă���Ƃ�(��C��)*/
ev_wifi_castle_gate_connect_poke_same02:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,1,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		0,FSW_ANSWER,0,0
	_TALKMSG				msg_bf_info_013
	_JUMP					bf_no_sel_menu					/*�{�ݑI����*/
	_END

/*�|�P��������C����Ă���Ƃ�*/
ev_wifi_castle_gate_connect_poke_same03:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		0,FSW_ANSWER,0,0

	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,1,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		1,FSW_ANSWER,0,0

	_TALKMSG				msg_bf_info_014
	_JUMP					bf_no_sel_menu					/*�{�ݑI����*/
	_END


/********************************************************************/
/*	���[���b�g�̃|�P�������X�g���ʂ��`�F�b�N(�L���b�X���Ə�������)	*/
/********************************************************************/
ev_bf_info_pokelist_roulette_result:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_CHECK_ENTRY,FRONTIER_NO_ROULETTE,0,FSW_ANSWER
	_SWITCH				FSW_ANSWER
	_CASE_JUMP			1,ev_wifi_roulette_gate_connect_poke_same01	/*1�C�ڂ����*/
	_CASE_JUMP			2,ev_wifi_roulette_gate_connect_poke_same02	/*2�C�ڂ����*/
	_CASE_JUMP			3,ev_wifi_roulette_gate_connect_poke_same03	/*1,2�C�Ƃ����*/
	_JUMP				ev_bf_info_pokelist_after
	_END


/********************************************************************/
/*		���[���b�g�@�|�P����������Ă���(�L���b�X���Ə�������)		*/
/********************************************************************/
/*�|�P��������C����Ă���Ƃ�(��C��)*/
ev_wifi_roulette_gate_connect_poke_same01:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		0,FSW_ANSWER,0,0
	_TALKMSG				msg_bf_info_013
	_JUMP					bf_no_sel_menu					/*�{�ݑI����*/
	_END

/*�|�P��������C����Ă���Ƃ�(��C��)*/
ev_wifi_roulette_gate_connect_poke_same02:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,1,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		0,FSW_ANSWER,0,0
	_TALKMSG				msg_bf_info_013
	_JUMP					bf_no_sel_menu					/*�{�ݑI����*/
	_END

/*�|�P��������C����Ă���Ƃ�*/
ev_wifi_roulette_gate_connect_poke_same03:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,0,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		0,FSW_ANSWER,0,0

	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_ENTRY_MONSNO,1,0,FSW_ANSWER
	_POKEMON_NAME_EXTRA		1,FSW_ANSWER,0,0

	_TALKMSG				msg_bf_info_014
	_JUMP					bf_no_sel_menu					/*�{�ݑI����*/
	_END


/********************************************************************/
/*						�|�P�������X�g�I����						*/
/********************************************************************/
ev_bf_info_pokelist_after:

	/*�A�������H������Ă��邩�`�F�b�N*/
	_CALL				ev_bf_info_pair_rensyou_ng_check_call

	_JUMP				ev_bf_info_pokelist_after_sub
	_END

ev_bf_info_pokelist_after_sub:
#if 1
	/*�N���A�t���O���擾���ăN���A*/
	/*�N���A�t���O���擾���āA0��������A�A�����R�[�h��0�ɂ��Ă���(�F�B�蒠�̋L�^�̂���)*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_CLEAR_FLAG,0,0,FSW_ANSWER
	_SAVE_EVENT_WORK_SET	SYS_WORK_WIFI_FR_CLEAR_FLAG,FSW_ANSWER
#endif

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FR_WIFI_COUNTER_SAVE
	_COMM_RESET

	/*�u���|�[�g�ɏ�������ł��܂��@�d����؂�Ȃ��ŉ������v*/
	_TALKMSG_ALL_PUT	msg_bf_info_028
	_ADD_WAITICON
	_REPORT_DIV_SAVE	FSW_ANSWER
	_DEL_WAITICON
	_SE_PLAY			SEQ_SE_DP_SAVE
	_SE_WAIT			SEQ_SE_DP_SAVE

	/*�u����ł͂���΂��Ă��������v*/
	_TALKMSG_NOSKIP		msg_bf_info_027
	_TIME_WAIT			10, FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FR_WIFI_COUNTER_GO
	_COMM_RESET

	_TALK_CLOSE

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_ACTOR_FREE			OBJID_PLAYER
	//_ACTOR_FREE			OBJID_PCWOMAN2_01
	_ACTOR_FREE			OBJID_SIO_USER_0
	_ACTOR_FREE			OBJID_SIO_USER_1
	//_CHAR_RESOURCE_FREE	PCWOMAN2

	_RECORD_INC			RECID_FRONTIER_COUNT

	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_BF_NO,0,0,FSW_ANSWER
	_SWITCH				FSW_ANSWER
	_CASE_JUMP			FRONTIER_NO_TOWER,ev_wifi_tower_start
	_CASE_JUMP			FRONTIER_NO_FACTORY_LV50,ev_wifi_factory_start
	_CASE_JUMP			FRONTIER_NO_FACTORY_LV100,ev_wifi_factory_start
	_CASE_JUMP			FRONTIER_NO_STAGE,ev_wifi_stage_start
	_CASE_JUMP			FRONTIER_NO_CASTLE,ev_wifi_castle_start
	_CASE_JUMP			FRONTIER_NO_ROULETTE,ev_wifi_roulette_start
	_END


/********************************************************************/
/*				�A�������H������Ă��邩�`�F�b�N*/
/********************************************************************/
ev_bf_info_pair_rensyou_ng_check_call:
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_CHECK_PAIR_RENSYOU,0,0,FSW_ANSWER

	/*���ʂ�ޔ�*/
	_LDWK				FSW_PARAM6,FSW_ANSWER

	_IFVAL_CALL			FSW_ANSWER,EQ,0,ev_bf_info_pair_rensyou_ng
	_RET

/*�A�������H������Ă������Ƃ�`����*/
ev_bf_info_pair_rensyou_ng:
	_TALKMSG			msg_bf_info_006
	_TALKMSG_NOSKIP		msg_bf_info_005
	_RET


/********************************************************************/
/********************************************************************/
ev_wifi_tower_start:
	/*�^���[��ʌĂяo���O*/

	/*����*/
	//_FR_WIFI_COUNTER_WORK_FREE
	_FR_WIFI_COUNTER_TOWER_CALL_BEFORE				/*���̒��Ń��[�N�m�� BTWR_MODE_WIFI_MULTI*/

	/*�}���`ENTRY��ݒ�*/
	_SAVE_EVENT_WORK_SET	WK_SCENE_D31R0206,1		/*new game �}���`*/

	/*�^���[�̃f�[�^�����O�̓���*/
	_CALL				ev_wifi_counter_synchronize_tower

	_JUMP				ev_wifi_tower_tr_data_retry
	_END

/*�ΐ�g���[�i�[�f�[�^���M*/
ev_wifi_tower_tr_data_retry:
	_FR_WIFI_COUNTER_TOWER_SEND_TR_DATA	FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_wifi_tower_tr_data_retry

	/*�ΐ�g���[�i�[�f�[�^��M*/
	_FR_WIFI_COUNTER_TOWER_RECV_TR_DATA

	/*����*/
	//tower6.s�̒���_FR_WIFI_COUNTER_TOWER_CALL_AFTER���Ă�ł���

	/*�ʐM����*/
	_CALL				ev_wifi_counter_synchronize
	_MAP_CHANGE_EX		FSS_SCENEID_TOWER_MULTI_WAY,0
	_END

ev_wifi_factory_start:
	_FR_WIFI_COUNTER_WORK_FREE

	/*BGM�̃t�F�[�h�����Ȃ��ƃ_������*/
	_BGM_PLAY			SEQ_PL_BF_FACTORY

	/*�ʐM����*/
	_CALL				ev_wifi_counter_synchronize
	_MAP_CHANGE_EX		FSS_SCENEID_FACTORY,0
	_END

ev_wifi_stage_start:
	_FR_WIFI_COUNTER_WORK_FREE

	/*BGM�̃t�F�[�h�����Ȃ��ƃ_������*/
	_BGM_PLAY			SEQ_PL_BF_STAGE

	/*�ʐM����*/
	_CALL				ev_wifi_counter_synchronize
	_MAP_CHANGE_EX		FSS_SCENEID_STAGE_WAY,0
	_END

ev_wifi_castle_start:
	_FR_WIFI_COUNTER_WORK_FREE

	/*BGM�̃t�F�[�h�����Ȃ��ƃ_������*/
	_BGM_PLAY			SEQ_PL_BF_CASTLE02

	/*�ʐM����*/
	_CALL				ev_wifi_counter_synchronize
	_MAP_CHANGE_EX		FSS_SCENEID_CASTLE_ROOM,0
	_END

ev_wifi_roulette_start:
	_FR_WIFI_COUNTER_WORK_FREE

	/*BGM�̃t�F�[�h�����Ȃ��ƃ_������*/
	_BGM_PLAY			SEQ_PL_BF_ROULETTE

	/*�ʐM����*/
	_CALL				ev_wifi_counter_synchronize
	_MAP_CHANGE_EX		FSS_SCENEID_ROULETTE_WAY,0
	_END

/*�}�b�v�J�ڑO�̓���*/
ev_wifi_counter_synchronize:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FR_WIFI_COUNTER_GO2
	_COMM_RESET
	_RET

/*�^���[�̃f�[�^�����O�̓���*/
ev_wifi_counter_synchronize_tower:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FR_WIFI_COUNTER_TOWER
	_COMM_RESET
	_RET


/********************************************************************/
/*						�{�݂���߂��Ă�����						*/
/********************************************************************/
fss_wifi_counter_return:

	/*�����̋L�^������*/
	_FR_WIFI_COUNTER_LIST_SET_LAST_PLAY_DATE

	_FR_WIFI_COUNTER_WORK_ALLOC
	//_FR_WIFI_COUNTER_WORK_FREE

	_TIME_WAIT			1,FSW_ANSWER
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_COMM_COMMAND_INITIALIZE,0,0,FSW_ANSWER
	_TIME_WAIT			1,FSW_ANSWER

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FR_WIFI_COUNTER_INIT_2
	_COMM_RESET

	_CHAR_RESOURCE_SET	comm_multi_set_resource				/*�L�����N�^���\�[�X�o�^*/
	_ACTOR_SET			comm_multi_set_actor				/*�A�N�^�[�o�^*/

	/*BGM�̃t�F�[�h�����Ȃ��ƃ_������*/
	_BGM_PLAY			SEQ_BF_TOWWER

	_BLACK_IN			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	/*�u������Ƃ�����x�V�т܂����H�v*/
	_PAIR_NAME			0
	_TALKMSG			msg_bf_info_030
	_YES_NO_WIN			FSW_ANSWER
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_wifi_counter_return_yes
	_JUMP				ev_wifi_counter_return_no
	_END

ev_wifi_counter_return_yes:
	/*�u����̑I����҂��Ă��܂��I���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_bf_info_031
	_LDVAL				FSW_PARAM2,0
	_JUMP				ev_bf_info_send_game_continue
	_END
	
ev_wifi_counter_return_no:
	/*�u����̑I����҂��Ă��܂��I���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_bf_info_031
	_LDVAL				FSW_PARAM2,1
	_JUMP				ev_bf_info_send_game_continue
	_END


/********************************************************************/
/*						���������V�Ԃ��𑗐M						*/
/********************************************************************/
ev_bf_info_send_game_continue:
	_LDVAL				FSW_PARAM1,FRONTIER_COMM_GAME_CONTINUE
	_FR_WIFI_COUNTER_SEND_BUF	FSW_PARAM1,FSW_PARAM2,0,FSW_ANSWER		/*0=�͂��A1=������*/
	_IFVAL_JUMP			FSW_ANSWER,EQ,0,ev_bf_info_send_game_continue	/*���M���s�͍đ��M*/

	_FR_WIFI_COUNTER_RECV_BUF	FSW_PARAM1
	_COMM_RESET

	/*���肪�I�񂾃��j���[���擾*/
	_FR_WIFI_COUNTER_YAMERU_CHECK	FSW_ANSWER

	/*�p�[�g�i�[���u�������v��I�񂾂�*/
	_FR_WIFI_COUNTER_TOOL	FWIFI_ID_GET_PAIR_GAME_CONTINUE,0,0,FSW_ANSWER
	_IFVAL_JUMP				FSW_ANSWER,EQ,1,ev_bf_info_game_end_pair

	/*�������u�������v��I�񂾂�*/
	_IFVAL_JUMP			FSW_PARAM2,EQ,1,ev_bf_info_game_end

	_JUMP				bf_no_sel_menu					/*�{�ݑI����*/
	_END


/********************************************************************/
/*					(�F�B)���������V�΂Ȃ�							*/
/********************************************************************/
ev_bf_info_game_end_pair:
	_PAIR_NAME			0
	_TALKMSG_NOSKIP		msg_bf_info_032

	/*�u���΂炭���҂����������v*/
	_TALKMSG_NOSKIP		msg_bf_info_005

	_JUMP				ev_bf_info_game_end
	_END
	
/********************************************************************/
/*						���������V�΂Ȃ�							*/
/********************************************************************/
ev_bf_info_game_end:
	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FR_WIFI_COUNTER_END
	_COMM_RESET

	_TALK_CLOSE
	_JUMP				ev_bf_info_end
	_END


/********************************************************************/
/*						���[�N�J�����ďI��							*/
/********************************************************************/
ev_bf_info_end:
	/*WiFi�}�b�`���O��ʂ�*/
	//_END

	_BLACK_OUT			SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	_FR_WIFI_COUNTER_WORK_FREE

	/*�ʐM����*/
	_COMM_RESET
	_COMM_SYNCHRONIZE	DBC_TIM_FR_WIFI_COUNTER_END_2
	_COMM_RESET

	_SCRIPT_FINISH		/*_END����2D�}�b�v�I��*/



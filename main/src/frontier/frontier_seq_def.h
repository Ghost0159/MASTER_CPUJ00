//==============================================================================
/**
 * @file	frontier_seq_def.h
 * @brief	�t�����e�B�A�p�X�N���v�g�R�}���h�̃}�N����`�t�@�C��
 * @author	matsuda
 * @date	2007.03.28(��)
 */
//==============================================================================

#define	__ASM_NO_DEF_
//	.include	"../../../include/battle/battle_common.h"
//	.include	"../../../include/battle/battle_server.h"
//	.include	"../../../include/battle/wazano_def.h"
//	.include	"../../../include/battle/tokusyu_def.h"
//	.include	"../../../include/msgdata/msg_fightmsg_dp.h"
//	.include	"../../../include/poketool/poke_tool.h"
//	.include	"../../../include/poketool/tr_tool.h"
//	.include	"../../../include/poketool/waza_tool.h"
//	.include	"../../../include/itemtool/itemequip.h"
//	.include	"../../../include/savedata/record.h"
//	.include	"../battle_snd_def.h"
	.include	"../../data/sound/pl_sound_data.sadl"

#define __ASM_ONLY_INC_
	.include	"../fs_usescript.h"

//�f�[�^�����R�ȋ��E�ɃA���C�����邩�̐ݒ�
	.option alignment off

	.macro	INIT_CMD
DEF_CMD_COUNT	=	0
	.endm
		
	.macro	DEF_CMD	symname
\symname	=	DEF_CMD_COUNT
DEF_CMD_COUNT	=	( DEF_CMD_COUNT + 1 )
	.endm

//���߃V���{���錾
	INIT_CMD
	DEF_CMD	FR_DUMMY
	DEF_CMD	FR_END
	DEF_CMD FR_SCRIPT_FINISH
	DEF_CMD FR_MAP_CHANGE
	DEF_CMD FR_MAP_CHANGE_EX
	DEF_CMD	FR_TIME_WAIT
	DEF_CMD FR_LD_WK_VAL
	DEF_CMD FR_LD_WK_WK
	DEF_CMD FR_ADD_WK
	DEF_CMD FR_SUB_WK
	DEF_CMD FR_JUMP
	DEF_CMD FR_IF_JUMP
	DEF_CMD FR_CALL
	DEF_CMD FR_RET
	DEF_CMD FR_IF_CALL
	DEF_CMD	FR_TALK_MSG_ALL_PUT
	DEF_CMD	FR_TALK_MSG_NO_SKIP
	DEF_CMD	FR_TALK_MSG
	DEF_CMD FR_TALK_WIN_CLOSE
	DEF_CMD FR_WIPE_FADE_START
	DEF_CMD FR_WIPE_FADE_END_CHECK
	DEF_CMD FR_BMPMENU_INIT
	DEF_CMD FR_BMPMENU_INIT_EX
	DEF_CMD FR_BMPMENU_MAKE_LIST
	DEF_CMD FR_BMPMENU_MAKE_LIST_TALK_MSG
	DEF_CMD FR_BMPMENU_START
	DEF_CMD FR_BMPLIST_INIT
	DEF_CMD FR_BMPLIST_INIT_EX
	DEF_CMD FR_BMPLIST_MAKE_LIST
	DEF_CMD FR_BMPLIST_START
	DEF_CMD FR_BMPLIST_DEL
	DEF_CMD FR_YES_NO_WIN
	DEF_CMD FR_CP_WK_VAL
	DEF_CMD FR_CP_WK_WK
	DEF_CMD FR_CHAR_RESOURCE_SET
	DEF_CMD FR_CHAR_RESOURCE_FREE
	DEF_CMD FR_ACTOR_SET
	DEF_CMD FR_ACTOR_FREE
	DEF_CMD FR_ACTOR_VISIBLE_SET
	DEF_CMD FR_ACTOR_BG_PRI_SET
	DEF_CMD FR_OBJ_ANIME
	DEF_CMD FR_OBJ_ANIME_WAIT
	DEF_CMD FR_ARTICLE_RESOURCE_SET
	DEF_CMD FR_ARTICLE_RESOURCE_FREE
	DEF_CMD FR_ARTICLE_ACTOR_SET
	DEF_CMD FR_ARTICLE_ACTOR_FREE
	DEF_CMD FR_ARTICLE_ACTOR_VISIBLE_SET
	DEF_CMD FR_ARTICLE_ACTOR_FLIP_H_SET
	DEF_CMD FR_ARTICLE_ACTOR_OBJ_MODE_SET
	DEF_CMD FR_ARTICLE_ACTOR_ANIME_START
	DEF_CMD FR_ARTICLE_ACTOR_ANIME_STOP
	DEF_CMD FR_ARTICLE_ACTOR_ANIME_WAIT
	DEF_CMD FR_NAMEIN_PROC
	DEF_CMD	FR_COMM_TIMING_SYNC_START
	DEF_CMD	FR_COMM_TEMP_DATA_RESET
	DEF_CMD	FR_COMM_DIRECT_END
	DEF_CMD	FR_GET_RND
	DEF_CMD	FR_PC_KAIFUKU
	DEF_CMD	FR_ABKEY_WAIT
	DEF_CMD	FR_ABKEY_TIMEWAIT
	DEF_CMD	FR_LOSE_CHECK
	DEF_CMD	FR_SAVE_EVENT_WORK_SET			//�Z�[�u���[�N�ɒl���Z�b�g
	DEF_CMD	FR_SAVE_EVENT_WORK_GET			//�Z�[�u���[�N�̒l���擾
	DEF_CMD FR_ENCOUNT_EFFECT
	DEF_CMD FR_GET_MINE_OBJ
	DEF_CMD FR_PARTICLE_SPA_LOAD
	DEF_CMD FR_PARTICLE_SPA_EXIT
	DEF_CMD FR_PARTICLE_ADD_EMITTER
	DEF_CMD FR_PARTICLE_WAIT
	DEF_CMD FR_TEMOTI_POKE_ACT_ADD
	DEF_CMD FR_TEMOTI_POKE_ACT_DEL
	DEF_CMD FR_BRAIN_ENCOUNT_EFFECT
	DEF_CMD FR_RECORD_INC
	DEF_CMD FR_RECORD_ADD
	DEF_CMD FR_SCORE_ADD
	DEF_CMD FR_CHECK_DP_ROM_CODE
	DEF_CMD FR_SHAKE_SET
	DEF_CMD FR_SHAKE_WAIT
	DEF_CMD FR_WND_SET
	DEF_CMD FR_WND_WAIT
	
	//CP�E�B���h�E
	DEF_CMD	FR_CP_WIN_WRITE
	DEF_CMD	FR_CP_WIN_DEL
	DEF_CMD	FR_CP_WRITE
	//OBJ����R�[�h
	DEF_CMD FR_OBJMOVE_RESET
	DEF_CMD FR_OBJMOVE_KYORO
	//�T�E���h
	DEF_CMD	FR_SE_PLAY
	DEF_CMD	FR_SE_STOP
	DEF_CMD	FR_SE_WAIT
	DEF_CMD	FR_ME_PLAY
	DEF_CMD	FR_ME_WAIT
	DEF_CMD	FR_BGM_PLAY
	DEF_CMD	FR_BGM_STOP
	//�t�@�N�g���[	
	DEF_CMD	FR_FACTORY_WORK_ALLOC		//���[�N�m��
	DEF_CMD	FR_FACTORY_WORK_INIT		//���[�N������
	DEF_CMD	FR_FACTORY_WORK_FREE		//���[�N�J��
	DEF_CMD	FR_FACTORY_RENTAL_CALL		//�t�@�N�g���[�����^���Ăяo��
	DEF_CMD	FR_FACTORY_CALL_GET_RESULT	//�t�@�N�g���[�Ăяo�����ʎ擾
	DEF_CMD	FR_FACTORY_BATTLE_CALL		//�t�@�N�g���[�퓬�Ăяo��
	DEF_CMD	FR_FACTORY_TRADE_CALL		//�t�@�N�g���[�����Ăяo��
	DEF_CMD	FR_FACTORY_RENTAL_PARTY_SET			//�t�@�N�g���[�����^��POKEPARTY�̃Z�b�g
	DEF_CMD	FR_FACTORY_BTL_AFTER_PARTY_SET		//�t�@�N�g���[�퓬���POKEPARTY�̃Z�b�g
	DEF_CMD	FR_FACTORY_TRADE_POKE_CHANGE		//�t�@�N�g���[�g���[�h��̃|�P�����ύX
	DEF_CMD	FR_FACTORY_TRADE_AFTER_PARTY_SET	//�t�@�N�g���[�g���[�h���POKEPARTY�̃Z�b�g
	DEF_CMD	FR_FACTORY_TOOL				//�t�@�N�g���[FACTORY_SCRWORK�̑���
	DEF_CMD	FR_FACTORY_LOSE_CHECK		//�t�@�N�g���[�s�k�`�F�b�N
	DEF_CMD	FR_FACTORY_SEND_BUF				//�t�@�N�g���[�ʐMsend_buf
	DEF_CMD	FR_FACTORY_RECV_BUF				//�t�@�N�g���[�ʐMrecv_buf
	DEF_CMD	FR_FACTORY_APPEAR_MSG
	//�^��
	DEF_CMD	FR_BATTLE_REC_INIT
	DEF_CMD	FR_BATTLE_REC_SAVE
	DEF_CMD	FR_BATTLE_REC_LOAD
	DEF_CMD	FR_BATTLE_REC_EXIT
	DEF_CMD	FR_BATTLE_REC_DATA_OCC_CHECK
	DEF_CMD	FR_BATTLE_REC_SERVER_VER_CHK

	//�Z�[�u
	DEF_CMD	FR_REPORT_SAVE
	DEF_CMD	FR_REPORT_DIV_SAVE
	DEF_CMD	FR_EXTRA_SAVE_INIT
	DEF_CMD	FR_EXTRA_SAVE_INIT_CHECK
	DEF_CMD	FR_STAGE_RENSHOU_COPY_EXTRA
	DEF_CMD FR_WAITICON_ADD
	DEF_CMD FR_WAITICON_DEL
	//���[�h�Z�b�g
	DEF_CMD	FR_ITEM_NAME
	DEF_CMD	FR_NUMBER_NAME
	DEF_CMD	FR_PLAYER_NAME
	DEF_CMD	FR_PAIR_NAME
	DEF_CMD	FR_WAZA_NAME
	DEF_CMD	FR_POKEMON_NAME_EXTRA
	DEF_CMD	FR_TYPE_NAME
	DEF_CMD	FR_RIVAL_NAME
	//�o�g���|�C���g
	DEF_CMD	FR_GET_BTL_POINT
	DEF_CMD	FR_BTL_POINT_ADD
	DEF_CMD	FR_BTL_POINT_SUB
	//�o�g���^���[
	DEF_CMD	FR_BTOWER_TOOLS
	DEF_CMD	FR_BTOWER_APPEAR_MSG
	DEF_CMD	FR_BTOWER_WORK_RELEASE		//�o�g���^���[���[�N���
	DEF_CMD	FR_BTOWER_BATTLE_CALL
	DEF_CMD	FR_BTOWER_CALL_GET_RESULT	//�o�g���^���[�Ăяo�����ʎ擾
	DEF_CMD	FR_BTOWER_SEND_BUF			//�o�g���^���[�ėp�f�[�^���M
	DEF_CMD	FR_BTOWER_RECV_BUF			//�o�g���^���[�ėp�f�[�^��M
	//�X�e�[�W
	DEF_CMD	FR_STAGE_WORK_ALLOC			//���[�N�m��
	DEF_CMD	FR_STAGE_WORK_ENEMY_SET		//�G�g���[�i�[�A�G�|�P�����Z�b�g
	DEF_CMD	FR_STAGE_WORK_FREE			//���[�N�J��
	DEF_CMD	FR_STAGE_POKE_TYPE_SEL_CALL	//�X�e�[�W�|�P�����^�C�v�I����ʌĂяo��
	DEF_CMD	FR_STAGE_CALL_GET_RESULT	//�X�e�[�W�Ăяo�����ʎ擾
	DEF_CMD	FR_STAGE_BATTLE_CALL		//�X�e�[�W�퓬�Ăяo��
	DEF_CMD	FR_STAGE_TOOL				//�X�e�[�WFACTORY_SCRWORK�̑���
	DEF_CMD	FR_STAGE_LOSE_CHECK			//�X�e�[�W�s�k�`�F�b�N
	DEF_CMD	FR_STAGE_SEND_BUF			//�X�e�[�W�ʐMsend_buf
	DEF_CMD	FR_STAGE_RECV_BUF			//�X�e�[�W�ʐMrecv_buf
	DEF_CMD	FR_STAGE_APPEAR_MSG
	//
	DEF_CMD	FR_DEBUG_PRINT
	//�L���b�X��
	DEF_CMD	FR_CASTLE_WORK_ALLOC		//���[�N�m��
	DEF_CMD	FR_CASTLE_WORK_INIT			//���[�N������
	DEF_CMD	FR_CASTLE_WORK_FREE			//���[�N�J��
	DEF_CMD	FR_CASTLE_MINE_CALL			//�莝����ʌĂяo��
	DEF_CMD	FR_CASTLE_CALL_GET_RESULT	//�L���b�X���Ăяo�����ʎ擾
	DEF_CMD	FR_CASTLE_BATTLE_CALL		//�L���b�X���퓬�Ăяo��
	DEF_CMD	FR_CASTLE_ENEMY_CALL		//�G�g���[�i�[�Ăяo��
	DEF_CMD	FR_CASTLE_BTL_BEFORE_PARTY_SET	//�L���b�X���퓬�O��POKEPARTY�̃Z�b�g
	DEF_CMD	FR_CASTLE_BTL_AFTER_PARTY_SET	//�L���b�X���퓬���POKEPARTY�̃Z�b�g
	DEF_CMD	FR_CASTLE_TOOL				//�L���b�X��CASTLE_SCRWORK�̑���
	DEF_CMD	FR_CASTLE_LOSE_CHECK		//�L���b�X���s�k�`�F�b�N
	DEF_CMD	FR_CASTLE_SEND_BUF				//�L���b�X���ʐMsend_buf
	DEF_CMD	FR_CASTLE_RECV_BUF				//�L���b�X���ʐMrecv_buf
	DEF_CMD	FR_CASTLE_RANKUP_CALL		//�����N�A�b�v�Ăяo��
	DEF_CMD	FR_CASTLE_PARENT_CHECK_WAIT		//�e�̌���҂�
	DEF_CMD	FR_CASTLE_GET_CP
	DEF_CMD	FR_CASTLE_SUB_CP
	DEF_CMD	FR_CASTLE_ADD_CP
	DEF_CMD	FR_CASTLE_APPEAR_MSG
	//WiFi��t
	DEF_CMD	FR_WIFI_COUNTER_WORK_ALLOC			//
	DEF_CMD	FR_WIFI_COUNTER_WORK_FREE			//
	DEF_CMD	FR_WIFI_COUNTER_SEND_BUF			//WiFi��t�ʐMsend_buf
	DEF_CMD	FR_WIFI_COUNTER_RECV_BUF			//WiFi��t�ʐMrecv_buf
	DEF_CMD	FR_WIFI_COUNTER_BFNO_CHECK			//
	DEF_CMD	FR_WIFI_COUNTER_POKELIST			//
	DEF_CMD	FR_WIFI_COUNTER_POKELIST_GET_RESULT	//
	DEF_CMD	FR_WIFI_COUNTER_YAMERU_CHECK		//
	DEF_CMD	FR_WIFI_COUNTER_TOOL				//FRWIFI_SCRWORK�̑���
	DEF_CMD	FR_WIFI_COUNTER_TOWER_CALL_BEFORE	//
	DEF_CMD	FR_WIFI_COUNTER_TOWER_SEND_TR_DATA	//
	DEF_CMD	FR_WIFI_COUNTER_TOWER_RECV_TR_DATA	//
	DEF_CMD	FR_WIFI_COUNTER_TOWER_CALL_AFTER	//
	DEF_CMD	FR_WIFI_COUNTER_LIST_SET_LAST_PLAY_DATE	//
	//���[���b�g
	DEF_CMD	FR_ROULETTE_WORK_ALLOC				//���[�N�m��
	DEF_CMD	FR_ROULETTE_WORK_INIT				//���[�N������
	DEF_CMD	FR_ROULETTE_WORK_FREE				//���[�N�J��
	DEF_CMD	FR_ROULETTE_CALL_GET_RESULT			//���[���b�g�Ăяo�����ʎ擾
	DEF_CMD	FR_ROULETTE_BATTLE_CALL				//���[���b�g�퓬�Ăяo��
	DEF_CMD	FR_ROULETTE_BTL_BEFORE_PARTY_SET	//���[���b�g�퓬�O��POKEPARTY�̃Z�b�g
	DEF_CMD	FR_ROULETTE_BTL_AFTER_PARTY_SET		//���[���b�g�퓬���POKEPARTY�̃Z�b�g
	DEF_CMD	FR_ROULETTE_TOOL					//���[���b�gCASTLE_SCRWORK�̑���
	DEF_CMD	FR_ROULETTE_LOSE_CHECK				//���[���b�g�s�k�`�F�b�N
	DEF_CMD	FR_ROULETTE_SEND_BUF				//���[���b�g�ʐMsend_buf
	DEF_CMD	FR_ROULETTE_RECV_BUF				//���[���b�g�ʐMrecv_buf
	DEF_CMD	FR_ROULETTE_CALL					//���[���b�g��ʌĂяo��
	DEF_CMD	FR_ROULETTE_APPEAR_MSG
	DEF_CMD	FR_ROULETTE_DECIDE_EV_NO_FUNC		//���肵���p�l���C�x���g�������s
	DEF_CMD	FR_ROULETTE_CHG_ITEMKEEP_VANISH		//�A�C�e�������Ă���A�C�R���̃o�j�b�V��

	DEF_CMD	FR_TV_TEMP_STAGE_SET
	DEF_CMD	FR_TV_TEMP_FRIEND_SET
	DEF_CMD	FR_COMM_SET_WIFI_BOTH_NET

//=============================================================================
/**
 *	�C�x���g�f�[�^�֘A
 */
//=============================================================================

//-----------------------------------------------------------------------------
/**
 *	�C�x���g�f�[�^�@�e�[�u���錾
 */
//-----------------------------------------------------------------------------
	.macro	_EVENT_DATA	adrs
	.long	((\adrs-.)-4)
	.endm

//-----------------------------------------------------------------------------
/**
 *	�C�x���g�f�[�^�@�e�[�u���I��
 */
//-----------------------------------------------------------------------------
	.macro	_EVENT_DATA_END
	.short	FSS_DATA_END_CODE
	.endm

//==============================================================================
//
//	���\�[�X�֘A
//
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	���\�[�X�f�[�^���x��
 */
//-----------------------------------------------------------------------------
	.macro	_RESOURCE_LABEL label
	.align	4
	\label:
	.endm

//--------------------------------------------------------------
/**
 * ���\�[�X�f�[�^
 */
//--------------------------------------------------------------
	.macro	_CHAR_RESOURCE_DATA	charid, movetype
	.short	\charid
	.byte	\movetype
	.endm

//--------------------------------------------------------------
/**
 * �v���C���[�L����(HEROorHEROINE)�̃L�����N�^���\�[�X�f�[�^
 */
//--------------------------------------------------------------
	.macro	_PLAYER_RESOURCE_DATA
	_CHAR_RESOURCE_DATA		FSS_CODE_MYSELF, 0, WF2DC_C_MOVERUN
	.endm

//--------------------------------------------------------------
/**
 * �ʐM����̃v���C���[�L����(HEROorHEROINE)�̃L�����N�^���\�[�X�f�[�^
 */
//--------------------------------------------------------------
	.macro	_SIO_PLAYER_RESOURCE_DATA
	_CHAR_RESOURCE_DATA		FSS_CODE_SIO_PLAYER, 0, WF2DC_C_MOVERUN
	.endm

//--------------------------------------------------------------
/**
 * ���\�[�X�f�[�^�I�[
 */
//--------------------------------------------------------------
	.macro	_CHAR_RESOURCE_DATA_END
	.short	FSS_DATA_END_CODE
	.endm

//--------------------------------------------------------------
/**
 * �L�����N�^���\�[�X�o�^
 */
//--------------------------------------------------------------
	.macro	_CHAR_RESOURCE_SET	adrs
	.short	FR_CHAR_RESOURCE_SET
	.long	((\adrs-.)-4)
	.endm

//--------------------------------------------------------------
/**
 * �L�����N�^���\�[�X�폜
 */
//--------------------------------------------------------------
	.macro	_CHAR_RESOURCE_FREE	charid
	.short	FR_CHAR_RESOURCE_FREE
	.short	\charid
	.endm


//==============================================================================
//
//	�A�N�^�[�֘A
//
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�A�N�^�[�f�[�^���x��
 */
//-----------------------------------------------------------------------------
	.macro	_ACTOR_LABEL label
	.align	4
	\label:
	.endm

//--------------------------------------------------------------
/**
 * �A�N�^�[�f�[�^(PLAYID�w��)
 */
//--------------------------------------------------------------
	.macro	_ACTOR_DATA		objid, charid, way, x, y, visible, event_id
	.short	\objid
	.short	\charid
	.byte	\way
	.short	\x			//X�͑̂̒��S
	.short	\y			//Y�͑���
	.byte	\visible
	.byte	\event_id
	.endm

//--------------------------------------------------------------
/**
 * �v���C���[�L����(HEROorHEROINE)�̃A�N�^�[�f�[�^
 */
//--------------------------------------------------------------
	.macro	_PLAYER_ACTOR_DATA	objid, way, x, y, visible
	_ACTOR_DATA				\objid, FSS_CODE_MYSELF, \way, \x, \y, \visible, 0
	.endm

//--------------------------------------------------------------
/**
 * �ʐM�L�����N�^�̃A�N�^�[�f�[�^
 */
//--------------------------------------------------------------
	.macro	_SIO_PLAYER_ACTOR_DATA	netid, objid, way, x, y, visible
	.short	\objid
	.short	FSS_CODE_SIO_PLAYER
	.byte	\way
	.short	\x			//X�͑̂̒��S
	.short	\y			//Y�͑���
	.byte	\visible
	.byte	0
	.byte	\netid
	.endm

//--------------------------------------------------------------
/**
 * �A�N�^�[�f�[�^�I�[
 */
//--------------------------------------------------------------
	.macro	_ACTOR_DATA_END
	.short	FSS_DATA_END_CODE
	.endm

//--------------------------------------------------------------
/**
 * �A�N�^�[�o�^
 */
//--------------------------------------------------------------
	.macro	_ACTOR_SET	adrs
	.short	FR_ACTOR_SET
	.long	((\adrs-.)-4)
	.endm

//--------------------------------------------------------------
/**
 * �A�N�^�[�폜
 */
//--------------------------------------------------------------
	.macro	_ACTOR_FREE		objid
	.short	FR_ACTOR_FREE
	.short	\objid
	.endm

//--------------------------------------------------------------
/**
 * �A�N�^�[�\���E��\���ݒ�
 */
//--------------------------------------------------------------
	.macro	_ACTOR_VISIBLE_SET		objid, visible_flg
	.short	FR_ACTOR_VISIBLE_SET
	.short	\objid
	.byte	\visible_flg
	.endm

//--------------------------------------------------------------
/**
 * �A�N�^�[BG�D��ݒ�
 */
//--------------------------------------------------------------
	.macro	_ACTOR_BG_PRI_SET		objid, pri
	.short	FR_ACTOR_BG_PRI_SET
	.short	\objid
	.short	\pri
	.endm


//==============================================================================
//
//	OBJ�A�j��
//
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�A�j��
 */
//-----------------------------------------------------------------------------
	.macro	_OBJ_ANIME	obj_id,adrs
	.short	FR_OBJ_ANIME
	.short	\obj_id
	.long	((\adrs-.)-4)
	.endm

//-----------------------------------------------------------------------------
/**
 *	�A�j���[�V�����f�[�^���x��
 */
//-----------------------------------------------------------------------------
	.macro	_ANIME_LABEL label
	.align	4
	\label:
	.endm

//-----------------------------------------------------------------------------
/**
 *	�A�j���[�V�����f�[�^
 */
//-----------------------------------------------------------------------------
	.macro	_ANIME_DATA	code,num
	.short	\code
	.short	\num
	.endm

//--------------------------------------------------------------
/**
 *	�A�j���[�V�����f�[�^�I��
 */
//--------------------------------------------------------------
	.macro	_ANIME_DATA_END
	.short	FSS_DATA_END_CODE
	.endm

//-----------------------------------------------------------------------------
/**
 *	�A�j���E�F�C�g
 */
//-----------------------------------------------------------------------------
	.macro	_OBJ_ANIME_WAIT
	.short	FR_OBJ_ANIME_WAIT
	.endm


//==============================================================================
//
//	�z�u�����\�[�X�֘A
//
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�z�u�����\�[�X�f�[�^���x��
 */
//-----------------------------------------------------------------------------
	.macro	_ARTICLE_RESOURCE_LABEL label
	.align	4
	\label:
	.endm

//--------------------------------------------------------------
/**
 * �z�u�����\�[�X�f�[�^
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_RESOURCE_DATA	act_id
	.short	\act_id
	.endm

//--------------------------------------------------------------
/**
 * �z�u�����\�[�X�f�[�^�I�[
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_RESOURCE_DATA_END
	.short	FSS_DATA_END_CODE
	.endm

//--------------------------------------------------------------
/**
 * �z�u�����\�[�X�o�^
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_RESOURCE_SET	adrs
	.short	FR_ARTICLE_RESOURCE_SET
	.long	((\adrs-.)-4)
	.endm

//--------------------------------------------------------------
/**
 * �z�u�����\�[�X�폜
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_RESOURCE_FREE	act_id
	.short	FR_ARTICLE_RESOURCE_FREE
	.short	\act_id
	.endm


//==============================================================================
//
//	�z�u���A�N�^�[�֘A
//
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�z�u���A�N�^�[�f�[�^���x��
 */
//-----------------------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_LABEL label
	.align	4
	\label:
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[�f�[�^
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_DATA		act_id, x, y, visible, anm_start, set_actwork_no
	.short	\act_id
	.short	\x
	.short	\y
	.short	\visible
	.byte	\anm_start
	.byte	\set_actwork_no
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[�f�[�^�I�[
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_DATA_END
	.short	FSS_DATA_END_CODE
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[�o�^
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_SET	adrs
	.short	FR_ARTICLE_ACTOR_SET
	.long	((\adrs-.)-4)
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[�폜
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_FREE		actwork_no
	.short	FR_ARTICLE_ACTOR_FREE
	.short	\actwork_no
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[�\���E��\���ݒ�
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_VISIBLE_SET		actwork_no, visible_flg
	.short	FR_ARTICLE_ACTOR_VISIBLE_SET
	.short	\actwork_no
	.byte	\visible_flg
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[H�t���b�v�ݒ�
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_FLIP_H_SET		actwork_no
	.short	FR_ARTICLE_ACTOR_FLIP_H_SET
	.short	\actwork_no
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[OBJ���[�h�ݒ�(0=�ʏ�,1=������)
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_OBJ_MODE_SET		actwork_no,flag
	.short	FR_ARTICLE_ACTOR_OBJ_MODE_SET
	.short	\actwork_no
	.short	\flag
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[�F�A�j���J�n
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_ANIME_START		actwork_no, seq_no
	.short	FR_ARTICLE_ACTOR_ANIME_START
	.short	\actwork_no
	.short	\seq_no
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[�F�A�j����~
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_ANIME_STOP		actwork_no
	.short	FR_ARTICLE_ACTOR_ANIME_STOP
	.short	\actwork_no
	.endm

//--------------------------------------------------------------
/**
 * �z�u���A�N�^�[�F�A�j���I���҂�
 */
//--------------------------------------------------------------
	.macro	_ARTICLE_ACTOR_ANIME_WAIT		actwork_no
	.short	FR_ARTICLE_ACTOR_ANIME_WAIT
	.short	\actwork_no
	.endm


//==============================================================================
//
//	��{�R�}���h
//
//==============================================================================

//-----------------------------------------------------------------------------
/**
 *	�_�~�[
 */
//-----------------------------------------------------------------------------
	.macro	_DUMMY
	.short	FR_DUMMY
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�N���v�g�̏I��
 */
//-----------------------------------------------------------------------------
	.macro	_END
	.short	FR_END
	.endm

//-----------------------------------------------------------------------------
/**
 *	2D�}�b�v�̏I��
 */
//-----------------------------------------------------------------------------
	.macro	_SCRIPT_FINISH
	.short	FR_SCRIPT_FINISH
	.endm

//-----------------------------------------------------------------------------
/**
 *	�}�b�v�؂�ւ�
 */
//-----------------------------------------------------------------------------
	.macro	_MAP_CHANGE		scene_id
	.short	FR_MAP_CHANGE
	.short	\scene_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	�}�b�v�؂�ւ�(�X�N���v�g�t�@�C���܂邲�Ɛ؂�ւ�)
 *
 * param_work, answer�̒l�͕ێ�����܂��Blocal_work, register�͔j������܂�
 */
//-----------------------------------------------------------------------------
	.macro	_MAP_CHANGE_EX	scene_id, event_id
	.short	FR_MAP_CHANGE_EX
	.short	\scene_id
	.short	\event_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	�E�F�C�g
 */
//-----------------------------------------------------------------------------
	.macro	_TIME_WAIT time,ret_wk
	.short	FR_TIME_WAIT
	.short	\time
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�W�J���b�Z�[�W��\��(2byte)(�ꊇ�\��)
 *
 *	@param	msg_id	�\�����郁�b�Z�[�WID
 */
//-----------------------------------------------------------------------------
	.macro	_TALKMSG_ALL_PUT msg_id
	.short	FR_TALK_MSG_ALL_PUT
	.short	\msg_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	�W�J���b�Z�[�W��\��(2byte)(�X�L�b�v�Ȃ�)
 *
 *	@param	msg_id	�\�����郁�b�Z�[�WID
 */
//-----------------------------------------------------------------------------
	.macro	_TALKMSG_NOSKIP msg_id
	.short	FR_TALK_MSG_NO_SKIP
	.short	\msg_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	�W�J���b�Z�[�W��\��(2byte)
 *
 *	@param	msg_id	�\�����郁�b�Z�[�WID
 */
//-----------------------------------------------------------------------------
	.macro	_TALKMSG msg_id
	.short	FR_TALK_MSG
	.short	\msg_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	��b�E�B���h�E����
 */
//-----------------------------------------------------------------------------
	.macro	_TALK_CLOSE
	.short	FR_TALK_WIN_CLOSE
	.endm

//==============================================================================
//	���C�v
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	���C�v�t�F�[�h�X�^�[�g
 *
 *	���ӁI�@�t�F�[�h�ɂ�����LCD�ABG�ʂ̐ݒ�͌Œ�ɂ��Ă���
 */
//-----------------------------------------------------------------------------
	.macro	_WIPE_FADE_START		div,sync,type,color
	.short	FR_WIPE_FADE_START
	.short	\div
	.short	\sync
	.short	\type
	.short	\color
	.endm

//-----------------------------------------------------------------------------
/**
 *	���C�v�t�F�[�h�I���`�F�b�N
 *
 *	���ӁI�@�`�F�b�N����LCD�̐ݒ�͌Œ�ɂ��Ă���
 */
//-----------------------------------------------------------------------------
	.macro	_WIPE_FADE_END_CHECK
	.short	FR_WIPE_FADE_END_CHECK
	.endm

//-----------------------------------------------------------------------------
/**
 *	�u���b�N�A�E�g
 *
 *	���ӁI�@�t�F�[�h�ɂ�����LCD�ABG�ʂ̐ݒ�͌Œ�ɂ��Ă���
 */
//-----------------------------------------------------------------------------
	.macro	_BLACK_OUT	div,sync
	_WIPE_FADE_START	\div,\sync,WIPE_TYPE_FADEOUT,WIPE_FADE_BLACK
	.endm

//-----------------------------------------------------------------------------
/**
 *	�u���b�N�C��
 *
 *	���ӁI�@�t�F�[�h�ɂ�����LCD�ABG�ʂ̐ݒ�͌Œ�ɂ��Ă���
 */
//-----------------------------------------------------------------------------
	.macro	_BLACK_IN	div,sync
	_WIPE_FADE_START	\div,\sync,WIPE_TYPE_FADEIN,WIPE_FADE_BLACK
	.endm

//-----------------------------------------------------------------------------
/**
 *	�z���C�g�A�E�g
 *
 *	���ӁI�@�t�F�[�h�ɂ�����LCD�ABG�ʂ̐ݒ�͌Œ�ɂ��Ă���
 */
//-----------------------------------------------------------------------------
	.macro	_WHITE_OUT	div,sync
	_WIPE_FADE_START	\div,\sync,WIPE_TYPE_FADEOUT,WIPE_FADE_WHITE
	.endm

//-----------------------------------------------------------------------------
/**
 *	�z���C�g�C��
 *
 *	���ӁI�@�t�F�[�h�ɂ�����LCD�ABG�ʂ̐ݒ�͌Œ�ɂ��Ă���
 */
//-----------------------------------------------------------------------------
	.macro	_WHITE_IN	div,sync
	_WIPE_FADE_START	\div,\sync,WIPE_TYPE_FADEIN,WIPE_FADE_WHITE
	.endm

//==============================================================================
//	
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�͂��A�������I��
 */
//-----------------------------------------------------------------------------
	//.macro	_YES_NO_WIN ret_wk
	//.short	FR_YES_NO_WIN
	//.short	\ret_wk
	//.endm
	
	.macro	_YES_NO_WINDOW ret_wk,csr_pos
	.short	FR_YES_NO_WIN
	.short	\ret_wk
	.byte	\csr_pos
	.endm

	.macro	_YES_NO_WIN ret_wk
	_YES_NO_WINDOW	\ret_wk,0
	.endm

	.macro	_YES_NO_WIN_EX ret_wk
	_YES_NO_WINDOW	\ret_wk,1
	.endm

//==============================================================================
//	BMP���j���[
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	BMP���j���[�@������
 */
//-----------------------------------------------------------------------------
	.macro	_BMPMENU_INIT x,y,cursor,cancel,ret_wk
	.short	FR_BMPMENU_INIT
	.byte	\x
	.byte	\y
	.byte	\cursor
	.byte	\cancel
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	BMP���j���[�@������(�ǂݍ���ł���gmm�t�@�C�����g�p����)
 */
//-----------------------------------------------------------------------------
	.macro	_BMPMENU_INIT_EX x,y,cursor,cancel,ret_wk
	.short	FR_BMPMENU_INIT_EX
	.byte	\x
	.byte	\y
	.byte	\cursor
	.byte	\cancel
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	BMP���j���[�@���X�g�쐬(short�o�[�W����)
 */
//-----------------------------------------------------------------------------
	.macro	_BMPMENU_MAKE_LIST msg_id,param
	.short	FR_BMPMENU_MAKE_LIST
	.short	\msg_id
	.short	\param
	.endm

//-----------------------------------------------------------------------------
/**
 *	BMP���j���[�@���X�g�쐬(short�o�[�W����)(��b�E�B�����b�Z�[�W�w�肠��)
 */
//-----------------------------------------------------------------------------
	.macro	_BMPMENU_MAKE_LIST_TALK_MSG msg_id,talk_msg_id,param
	.short	FR_BMPMENU_MAKE_LIST_TALK_MSG
	.short	\msg_id
	.short	\talk_msg_id
	.short	\param
	.endm

//-----------------------------------------------------------------------------
/**
 *	BMP���j���[�@�J�n
 */
//-----------------------------------------------------------------------------
	.macro	_BMPMENU_START
	.short	FR_BMPMENU_START
	.endm

//==============================================================================
//	BMP���X�g
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	BMP���X�g�@������
 */
//-----------------------------------------------------------------------------
	.macro	_BMPLIST_INIT x,y,cursor,cancel,ret_wk
	.short	FR_BMPLIST_INIT
	.byte	\x
	.byte	\y
	.byte	\cursor
	.byte	\cancel
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	BMP���X�g�@������(�ǂݍ���ł���gmm�t�@�C�����g�p����)
 */
//-----------------------------------------------------------------------------
	.macro	_BMPLIST_INIT_EX x,y,cursor,cancel,ret_wk
	.short	FR_BMPLIST_INIT_EX
	.byte	\x
	.byte	\y
	.byte	\cursor
	.byte	\cancel
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	BMP���X�g�@���X�g�쐬(short�o�[�W����)
 */
//-----------------------------------------------------------------------------
	.macro	_BMPLIST_MAKE_LIST msg_id,talk_msg_id,param
	.short	FR_BMPLIST_MAKE_LIST
	.short	\msg_id
	.short	\talk_msg_id
	.short	\param
	.endm

//-----------------------------------------------------------------------------
/**
 *	BMP���X�g�@�J�n
 */
//-----------------------------------------------------------------------------
	.macro	_BMPLIST_START
	.short	FR_BMPLIST_START
	.endm

//-----------------------------------------------------------------------------
/**
 *	BMP���X�g�@�����폜	�o�g���L���b�X���݂̂Ŏg�p
 */
//-----------------------------------------------------------------------------
	.macro	_BMPLIST_DEL
	.short	FR_BMPLIST_DEL
	.endm

//==============================================================================
//	���
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	���[�N�ɒl����
 */
//-----------------------------------------------------------------------------
	.macro	_LDVAL	wk,val
	.short	FR_LD_WK_VAL
	.short	\wk
	.short	\val
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[�N�Ƀ��[�N�̒l����
 */
//-----------------------------------------------------------------------------
	.macro	_LDWK	wk1,wk2
	.short	FR_LD_WK_WK
	.short	\wk1
	.short	\wk2
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[�N�ɒl�𑫂�
 */
//-----------------------------------------------------------------------------
	.macro	_ADD_WK wk,num
	.short	FR_ADD_WK
	.short	\wk
	.short	\num
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[�N�ɒl��e��
 */
//-----------------------------------------------------------------------------
	.macro	_SUB_WK wk,num
	.short	FR_SUB_WK
	.short	\wk
	.short	\num
	.endm

//==============================================================================
//	��r
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	���[�N�ƒl�̔�r
 */
//-----------------------------------------------------------------------------
	.macro	_CMPVAL	wk,val
	.short	FR_CP_WK_VAL
	.short	\wk
	.short	\val
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[�N�ƃ��[�N�̔�r
 */
//-----------------------------------------------------------------------------
	.macro	_CMPWK	wk1,wk2
	.short	FR_CP_WK_WK
	.short	\wk1
	.short	\wk2
	.endm

//==============================================================================
//	�W�����v����
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�W�����v
 */
//-----------------------------------------------------------------------------
	.macro	_JUMP adrs
	.short	FR_JUMP
	.long	((\adrs-.)-4)
	.endm

//-----------------------------------------------------------------------------
/**
 *	IF�W�����v
 */
//-----------------------------------------------------------------------------
	.macro	_IF_JUMP cond,adrs
	.short	FR_IF_JUMP
	.byte	\cond
	.long	((\adrs-.)-4)
	.endm

//-----------------------------------------------------------------------------
/**
 *	�l���r�A�����ǂ���̏ꍇ����(�W�����v)
 */
//-----------------------------------------------------------------------------
	.macro	_IFVAL_JUMP wk,cond,val,adrs
	_CMPVAL	\wk,\val
	.short	FR_IF_JUMP
	.byte	\cond
	.long	((\adrs-.)-4)
	.endm

//-----------------------------------------------------------------------------
/**
 *	�l���r�A�����ǂ���̏ꍇ����(�R�[��)
 */
//-----------------------------------------------------------------------------
	.macro	_IFVAL_CALL wk,cond,val,adrs
	_CMPVAL	\wk,\val
	.short	FR_IF_CALL
	.byte	\cond
	.long	((\adrs-.)-4)
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[�N���r�A�����ǂ���̏ꍇ����(�W�����v)
 */
//-----------------------------------------------------------------------------
	.macro	_IFWK_JUMP wk,cond,wk2,adrs
	_CMPWK	\wk,\wk2
	.short	FR_IF_JUMP
	.byte	\cond
	.long	((\adrs-.)-4)
	.endm

//-----------------------------------------------------------------------------
/**
 *	�R�[��
 */
//-----------------------------------------------------------------------------
	.macro	_CALL adrs
	.short	FR_CALL
	.long	((\adrs-.)-4)
	.endm

//-----------------------------------------------------------------------------
/**
 *	�R�[�����Ăяo�����A�h���X�ɖ߂�
 */
//-----------------------------------------------------------------------------
	.macro	_RET
	.short	FR_RET
	.endm

//-----------------------------------------------------------------------------
/**
 *	IF�R�[��
 */
//-----------------------------------------------------------------------------
	.macro	_IF_CALL cond,adrs
	.short	FR_IF_CALL
	.byte	\cond
	.long	((\adrs-.)-4)
	.endm

//-----------------------------------------------------------------------------
/**
 *	switch �` case �\��
 *
 *  �X�N���v�g���䃏�[�N�Ŋm�ۂ���郏�[�N(SCWK_REG0)���g�p���Ă���̂ŁA
 *	FLAG_CHANGE,INIT_CHANGE�Ȃǂł͎g�p�ł��܂���I
 *	SCENE_CHANGE�͎g�p�ł��܂��B
 */
//-----------------------------------------------------------------------------
	.macro		_SWITCH wk
	_LDWK		FSW_REG0,\wk
	.endm

	.macro		_CASE_JUMP	val,adr
	_CMPVAL		FSW_REG0,\val
	_IF_JUMP	EQ,\adr
	.endm

//-----------------------------------------------------------------------------
/**
 * �X�N���v�g�E�B���h�E�\������B�L�����Z��
 */
//-----------------------------------------------------------------------------
	.macro		_CASE_CANCEL	adr
	_CMPVAL		FSW_REG0,FSEV_WIN_B_CANCEL
	_IF_JUMP	EQ,\adr
	.endm

//-----------------------------------------------------------------------------
/**
 *	���O���͉�ʌĂяo��
 */
//-----------------------------------------------------------------------------
	.macro	_NAMEIN_PROC
	.short	FR_NAMEIN_PROC
	.endm

//=============================================================================
/**
 *	�ʐM
 */
//=============================================================================

//-----------------------------------------------------------------------------
/**
 *	����
 */
//-----------------------------------------------------------------------------
	.macro	_COMM_SYNCHRONIZE	no
	.short	FR_COMM_TIMING_SYNC_START
	.short	\no
	.endm

//-----------------------------------------------------------------------------
/**
 * �@���Z�b�g	
 */
//-----------------------------------------------------------------------------
	.macro	_COMM_RESET
	.short	FR_COMM_TEMP_DATA_RESET
	.endm

//-----------------------------------------------------------------------------
/**
 * �@�ʐM�ؒf
 */
//-----------------------------------------------------------------------------
	.macro	_COMM_DIRECT_END
	.short	FR_COMM_DIRECT_END
	.endm

//-----------------------------------------------------------------------------
/**
 *	�����_�����擾
 */
//-----------------------------------------------------------------------------
	.macro	_GET_RND ret_wk,num
	.short	FR_GET_RND
	.short	\ret_wk
	.short	\num
	.endm

//-----------------------------------------------------------------------------
/**
 *	�|�P�Z����
 */
//-----------------------------------------------------------------------------
	.macro	_PC_KAIFUKU
	.short	FR_PC_KAIFUKU
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L�[�҂�
 */
//-----------------------------------------------------------------------------
	.macro	_AB_KEYWAIT
	.short	FR_ABKEY_WAIT
	.endm

//-----------------------------------------------------------------------------
/**
 * �L�[�҂��@OR�@���ԑ҂�
 */
//-----------------------------------------------------------------------------
	.macro	_AB_TIME_KEYWAIT	wait
	.short	FR_ABKEY_TIMEWAIT
	.short	\wait
	.endm

//-----------------------------------------------------------------------------
/**
 *	�g���[�i�[�s�k�`�F�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_TRAINER_LOSE_CHECK ret_wk
	.short	FR_LOSE_CHECK
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�Z�[�u���[�N�ɒl���Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_SAVE_EVENT_WORK_SET	work_no,param
	.short	FR_SAVE_EVENT_WORK_SET
	.short	\work_no
	.short	\param
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�Z�[�u���[�N�̒l���擾
 */
//-----------------------------------------------------------------------------
	.macro	_SAVE_EVENT_WORK_GET	work_no,work
	.short	FR_SAVE_EVENT_WORK_GET
	.short	\work_no
	.short	\work
	.endm

//-----------------------------------------------------------------------------
/**
 *	�G���J�E���g�G�t�F�N�g
 */
//-----------------------------------------------------------------------------
	.macro	_ENCOUNT_EFFECT		effect_no
	.short	FR_ENCOUNT_EFFECT
	.short	\effect_no
	.endm

//-----------------------------------------------------------------------------
/**
 *	��l���̐l��OBJ�R�[�h���擾
 */
//-----------------------------------------------------------------------------
	.macro	_GET_MINE_OBJ		ret_wk
	.short	FR_GET_MINE_OBJ
	.short	\ret_wk
	.endm


//==============================================================================
//	�p�[�e�B�N��
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	SPA���\�[�X�ǂݍ���
 */
//-----------------------------------------------------------------------------
	.macro	_PARTICLE_SPA_LOAD		work_id, spa_no, camera_type
	.short	FR_PARTICLE_SPA_LOAD
	.short	\work_id
	.short	\spa_no
	.short	\camera_type
	.endm

//-----------------------------------------------------------------------------
/**
 *	SPA���\�[�X���
 */
//-----------------------------------------------------------------------------
	.macro	_PARTICLE_SPA_EXIT		work_id
	.short	FR_PARTICLE_SPA_EXIT
	.short	\work_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	�G�~�b�^����
 */
//-----------------------------------------------------------------------------
	.macro	_PARTICLE_ADD_EMITTER		work_id, emit_no
	.short	FR_PARTICLE_ADD_EMITTER
	.short	\work_id
	.short	\emit_no
	.endm

//-----------------------------------------------------------------------------
/**
 *	�p�[�e�B�N���I���҂�
 */
//-----------------------------------------------------------------------------
	.macro	_PARTICLE_WAIT
	.short	FR_PARTICLE_WAIT
	.endm

//-----------------------------------------------------------------------------
/**
 *	�|�P�����A�N�^�[�\��
 */
//-----------------------------------------------------------------------------
	.macro	_TEMOTI_POKE_ACT_ADD	pos,x,y,id,evy,next_rgb
	.short	FR_TEMOTI_POKE_ACT_ADD
	.short	\pos
	.short	\x
	.short	\y
	.short	\id
	.short	\evy
	.short	\next_rgb
	.endm

//-----------------------------------------------------------------------------
/**
 *	�|�P�����A�N�^�[�폜
 */
//-----------------------------------------------------------------------------
	.macro	_TEMOTI_POKE_ACT_DEL	id
	.short	FR_TEMOTI_POKE_ACT_DEL
	.short	\id
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�����e�B�A�u���[����̃G���J�E���g�G�t�F�N�g����
 */
//-----------------------------------------------------------------------------
	.macro	_BRAIN_ENCOUNT_EFFECT	effect_no
	.short	FR_BRAIN_ENCOUNT_EFFECT
	.short	\effect_no
	.endm

//-----------------------------------------------------------------------------
/**
 *	���R�[�h�C���N�������g
 */
//-----------------------------------------------------------------------------
	.macro	_RECORD_INC	recid
	.short	FR_RECORD_INC
	.short	\recid
	.endm

//-----------------------------------------------------------------------------
/**
 *	���R�[�h�ǉ�
 */
//-----------------------------------------------------------------------------
	.macro	_RECORD_ADD	recid,val
	.short	FR_RECORD_ADD
	.short	\recid
	.short	\val
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�R�A�ǉ�
 */
//-----------------------------------------------------------------------------
	.macro	_SCORE_ADD	recid
	.short	FR_SCORE_ADD
	.short	\recid
	.endm

//-----------------------------------------------------------------------------
/**
 *	�����A����̂ǂ��炩��DP���`�F�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_CHECK_DP_ROM_CODE	ret_wk
	.short	FR_CHECK_DP_ROM_CODE
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�n�k�Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_SHAKE_SET	x, y, wait, loop
	.short	FR_SHAKE_SET
	.short	\x
	.short	\y
	.short	\wait
	.short	\loop
	.endm

//-----------------------------------------------------------------------------
/**
 *	�n�k�I���҂�
 */
//-----------------------------------------------------------------------------
	.macro	_SHAKE_WAIT
	.short	FR_SHAKE_WAIT
	.endm

//-----------------------------------------------------------------------------
/**
 *	Window�@�\�Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_WND_SET	x1, y1, x2, y2, wait, on_off
	.short	FR_WND_SET
	.short	\x1
	.short	\y1
	.short	\x2
	.short	\y2
	.short	\wait
	.short	\on_off
	.endm

//-----------------------------------------------------------------------------
/**
 *	Window�@�\�I���҂�
 */
//-----------------------------------------------------------------------------
	.macro	_WND_WAIT
	.short	FR_WND_WAIT
	.endm

//-----------------------------------------------------------------------------
/**
 *	CP�E�B���h�E�\��
 */
//-----------------------------------------------------------------------------
	.macro	_CP_WIN_WRITE
	.short	FR_CP_WIN_WRITE
	.endm

//-----------------------------------------------------------------------------
/**
 *	CP�E�B���h�E�폜
 */
//-----------------------------------------------------------------------------
	.macro	_CP_WIN_DEL
	.short	FR_CP_WIN_DEL
	.endm

//-----------------------------------------------------------------------------
/**
 *	CP�\��
 */
//-----------------------------------------------------------------------------
	.macro	_CP_WRITE
	.short	FR_CP_WRITE
	.endm

//==============================================================================
//	OBJ����R�[�h�����Ă�
//==============================================================================
//--------------------------------------------------------------
/**
 *	OBJ����R�[�h�ݒ�F���Z�b�g����(OBJ����Ȃ��ɂ���)
 */
//--------------------------------------------------------------
	.macro	_OBJMOVE_RESET		objid
	.short	FR_OBJMOVE_RESET
	.short	\objid
	.endm

//--------------------------------------------------------------
/**
 *	OBJ����R�[�h�ݒ�F����낫���
 */
//--------------------------------------------------------------
	.macro	_OBJMOVE_KYORO		objid, front_dir, dir_flg, first_wait
	.short	FR_OBJMOVE_KYORO
	.short	\objid
	.short	\front_dir			//FC_DIR_???
	.short	\dir_flg			//0 or 1�B�@front��DOWN�̏ꍇ�A0���ƁA�����E�A1���ƁA�E����
	.short	\first_wait			//�����E�F�C�g
	.endm


//=============================================================================
//	�T�E���h
//=============================================================================
//-----------------------------------------------------------------------------
/**
 *	SE��炷
 */
//-----------------------------------------------------------------------------
	.macro	_SE_PLAY no
	.short	FR_SE_PLAY
	.short	\no
	.endm

//-----------------------------------------------------------------------------
/**
 *	SE���~�߂�
 */
//-----------------------------------------------------------------------------
	.macro	_SE_STOP no
	.short	FR_SE_STOP
	.short	\no
	.endm

//-----------------------------------------------------------------------------
/**
 *	SE�I���҂�
 */
//-----------------------------------------------------------------------------
	.macro	_SE_WAIT	no
	.short	FR_SE_WAIT
	.short	\no
	.endm

//-----------------------------------------------------------------------------
/**
 *	ME��炷
 */
//-----------------------------------------------------------------------------
	.macro	_ME_PLAY no
	.short	FR_ME_PLAY
	.short	\no
	.endm

//-----------------------------------------------------------------------------
/**
 *	ME�I���҂�
 */
//-----------------------------------------------------------------------------
	.macro	_ME_WAIT
	.short	FR_ME_WAIT
	.endm

//-----------------------------------------------------------------------------
/**
 *	BGM��炷
 */
//-----------------------------------------------------------------------------
	.macro	_BGM_PLAY no
	.short	FR_BGM_PLAY
	.short	\no
	.endm

//-----------------------------------------------------------------------------
/**
 *	BGM���~�߂�
 */
//-----------------------------------------------------------------------------
	.macro	_BGM_STOP no
	.short	FR_BGM_STOP
	.short	\no
	.endm


//==============================================================================
//	�t�@�N�g���[
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[���[�N�m��
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_WORK_ALLOC	init,type,level
	.short	FR_FACTORY_WORK_ALLOC
	.short	\init
	.short	\type
	.short	\level
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[���[�N������
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_WORK_INIT	init
	.short	FR_FACTORY_WORK_INIT
	.short	\init
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[���[�N�J��
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_WORK_FREE
	.short	FR_FACTORY_WORK_FREE
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�����^���Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_RENTAL_CALL
	.short	FR_FACTORY_RENTAL_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�Ăяo�����ʎ擾
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_CALL_GET_RESULT
	.short	FR_FACTORY_CALL_GET_RESULT
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�퓬�Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_BATTLE_CALL
	.short	FR_FACTORY_BATTLE_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�����Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_TRADE_CALL
	.short	FR_FACTORY_TRADE_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�����^��POKEPARTY�̃Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_RENTAL_PARTY_SET
	.short	FR_FACTORY_RENTAL_PARTY_SET
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�o�g�����POKEPARTY�̃Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_BTL_AFTER_PARTY_SET
	.short	FR_FACTORY_BTL_AFTER_PARTY_SET
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[������̃|�P�����ύX
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_TRADE_POKE_CHANGE
	.short	FR_FACTORY_TRADE_POKE_CHANGE
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�������POKEPARTY�̃Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_TRADE_AFTER_PARTY_SET
	.short	FR_FACTORY_TRADE_AFTER_PARTY_SET
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[FACTORY_SCRWORK�̑���
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_TOOL	code,param1,param2,ret_work
	.short	FR_FACTORY_TOOL
	.byte	\code
	.byte	\param1
	.byte	\param2
	.short	\ret_work
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�s�k�`�F�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_LOSE_CHECK	ret_wk
	.short	FR_FACTORY_LOSE_CHECK
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�ʐMsend_buf
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_SEND_BUF	type,param,ret_wk
	.short	FR_FACTORY_SEND_BUF
	.short	\type
	.short	\param
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�ʐMrecv_buf(wk��type�������ēn���ĉ�����)
 */
//-----------------------------------------------------------------------------
	.macro	_FACTORY_RECV_BUF	wk
	.short	FR_FACTORY_RECV_BUF
	.short	\wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�t�@�N�g���[�ΐ�O���b�Z�[�W��\��(tr_idx)
 *  @param	tr_idx	��l�ڂ���l�ڂ��H
 */
//-----------------------------------------------------------------------------
	.macro	_TALKMSG_FACTORY_APPEAR	tr_id	
	.short	FR_FACTORY_APPEAR_MSG
	.byte	\tr_id
	.endm

//==============================================================================
//	�^��
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�f�[�^������
 */
//-----------------------------------------------------------------------------
	.macro	_BATTLE_REC_INIT
	.short	FR_BATTLE_REC_INIT
	.endm

//-----------------------------------------------------------------------------
/**
 *	�Z�[�u
 */
//-----------------------------------------------------------------------------
	.macro	_BATTLE_REC_SAVE	fr_no,type,rensyou,ret_wk
	.short	FR_BATTLE_REC_SAVE
	.short	\fr_no
	.short	\type
	.short	\rensyou
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[�h
 */
//-----------------------------------------------------------------------------
	.macro	_BATTLE_REC_LOAD
	.short	FR_BATTLE_REC_LOAD
	.endm

//-----------------------------------------------------------------------------
/**
 *	�j��
 */
//-----------------------------------------------------------------------------
	.macro	_BATTLE_REC_EXIT
	.short	FR_BATTLE_REC_EXIT
	.endm

//-----------------------------------------------------------------------------
/**
 *	�^��f�[�^�����łɂ��邩�`�F�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_BATTLE_REC_DATA_OCC_CHECK	ret_wk
	.short	FR_BATTLE_REC_DATA_OCC_CHECK
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	������ROM�����T�[�o�[�o�[�W�������������擾
 */
//-----------------------------------------------------------------------------
	.macro	_BATTLE_REC_SERVER_VER_CHK	ret_wk
	.short	FR_BATTLE_REC_SERVER_VER_CHK
	.short	\ret_wk
	.endm

//==============================================================================
//	�Z�[�u
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	���|�[�g��������
 */
//-----------------------------------------------------------------------------
	.macro	_REPORT_SAVE	ret_wk
	.short	FR_REPORT_SAVE
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	���|�[�g��������(����)
 */
//-----------------------------------------------------------------------------
	.macro	_REPORT_DIV_SAVE	ret_wk
	.short	FR_REPORT_DIV_SAVE
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�O���Z�[�u������
 */
//-----------------------------------------------------------------------------
	.macro	_EXTRA_SAVE_INIT
	.short	FR_EXTRA_SAVE_INIT
	.endm

//-----------------------------------------------------------------------------
/**
 *	�O���Z�[�u�������ς݂��`�F�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_EXTRA_SAVE_INIT_CHECK	ret_wk
	.short	FR_EXTRA_SAVE_INIT_CHECK
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�o�g���X�e�[�W�̊O���A���L�^���������݁A�Z�[�u���s��
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_RENSHOU_COPY_EXTRA	ret_wk
	.short	FR_STAGE_RENSHOU_COPY_EXTRA
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�ҋ@�A�C�R���\��
 */
//-----------------------------------------------------------------------------
	.macro	_ADD_WAITICON
	.short	FR_WAITICON_ADD
	.endm

//-----------------------------------------------------------------------------
/**
 *	�ҋ@�A�C�R������
 */
//-----------------------------------------------------------------------------
	.macro	_DEL_WAITICON
	.short	FR_WAITICON_DEL
	.endm

//==============================================================================
//	���[�h�Z�b�g
//==============================================================================

//-----------------------------------------------------------------------------
/**
 *	�A�C�e�������Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_ITEM_NAME	idx,itemno
	.short	FR_ITEM_NAME
	.byte	\idx
	.short	\itemno
	.endm

//-----------------------------------------------------------------------------
/**
 *	���l���Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_NUMBER_NAME	idx,number
	.short	FR_NUMBER_NAME
	.byte	\idx
	.short	\number
	.endm

//-----------------------------------------------------------------------------
/**
 *	��l����
 */
//-----------------------------------------------------------------------------
	.macro	_PLAYER_NAME	idx
	.short	FR_PLAYER_NAME
	.byte	\idx
	.endm

//-----------------------------------------------------------------------------
/**
 *	�p�[�g�i�[��
 */
//-----------------------------------------------------------------------------
	.macro	_PAIR_NAME	idx
	.short	FR_PAIR_NAME
	.byte	\idx
	.endm

//-----------------------------------------------------------------------------
/**
 *	�Z��
 */
//-----------------------------------------------------------------------------
	.macro	_WAZA_NAME	idx,waza
	.short	FR_WAZA_NAME
	.byte	\idx
	.short	\waza
	.endm

//-----------------------------------------------------------------------------
/**
 *	�����X�^�[�i���o�[����|�P������
 */
//-----------------------------------------------------------------------------
	.macro	_POKEMON_NAME_EXTRA	idx,mons,sex,flag
	.short	FR_POKEMON_NAME_EXTRA
	.byte	\idx
	.short	\mons
	.short	\sex
	.byte	\flag
	.endm

//-----------------------------------------------------------------------------
/**
 *	�^�C�v�̖��O
 */
//-----------------------------------------------------------------------------
	.macro	_TYPE_NAME	idx,type
	.short	FR_TYPE_NAME
	.byte	\idx
	.short	\type
	.endm

//-----------------------------------------------------------------------------
/**
 *	���C�o����
 */
//-----------------------------------------------------------------------------
	.macro	_RIVAL_NAME	idx
	.short	FR_RIVAL_NAME
	.byte	\idx
	.endm

//==============================================================================
//	�o�g���|�C���g
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�����o�g���|�C���g�擾
 */
//-----------------------------------------------------------------------------
	.macro	_GET_BTL_POINT	ret_wk
	.short	FR_GET_BTL_POINT
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�����o�g���|�C���g���Z
 */
//-----------------------------------------------------------------------------
	.macro	_BTL_POINT_ADD	value
	.short	FR_BTL_POINT_ADD
	.short	\value
	.endm

//-----------------------------------------------------------------------------
/**
 *	�����o�g���|�C���g���Z
 */
//-----------------------------------------------------------------------------
	.macro	_BTL_POINT_SUB	value
	.short	FR_BTL_POINT_SUB
	.short	\value
	.endm

//==============================================================================
//	�o�g���^���[
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�o�g���^���[ �R�}���h�c�[���Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_BTOWER_TOOLS	cmd,param,ret_wk
	.short	FR_BTOWER_TOOLS
	.short	\cmd
	.short	\param
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�^���[�ΐ�O���b�Z�[�W��\��(tr_idx)
 *  @param	tr_idx	��l�ڂ���l�ڂ��H
 */
//-----------------------------------------------------------------------------
	.macro	_TALKMSG_BTOWER_APPEAR	tr_id	
	.short	FR_BTOWER_APPEAR_MSG
	.byte	\tr_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	�o�g���^���[���[�N���
 */
//-----------------------------------------------------------------------------
	.macro	_BTOWER_WORK_RELEASE
	.short	FR_BTOWER_WORK_RELEASE
	.endm

//-----------------------------------------------------------------------------
/**
 *	�^���[�퓬�Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_BTOWER_BATTLE_CALL
	.short	FR_BTOWER_BATTLE_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�^���[�Ăяo�����ʎ擾
 */
//-----------------------------------------------------------------------------
	.macro	_BTOWER_CALL_GET_RESULT	ret_wk
	.short	FR_BTOWER_CALL_GET_RESULT
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�o�g���^���[ �ėp�f�[�^���M
 */
//-----------------------------------------------------------------------------
	.macro	_BTOWER_SEND_BUF	mode,param,ret_wk
	.short	FR_BTOWER_SEND_BUF
	.short	\mode
	.short	\param
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�o�g���^���[ �ėp�f�[�^��M
 *
 *	(���[�h���i�[����Ă��郏�[�N��ID�ƁA�߂�l���i�[���郏�[�N��ID��n��)
 */
//-----------------------------------------------------------------------------
	.macro	_BTOWER_RECV_BUF	mode,ret_wk
	.short	FR_BTOWER_RECV_BUF
	.short	\mode
	.short	\ret_wk
	.endm

//==============================================================================
//	�X�e�[�W
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�X�e�[�W���[�N�m��
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_WORK_ALLOC	init,type,pos,pos2
	.short	FR_STAGE_WORK_ALLOC
	.short	\init
	.short	\type
	.short	\pos
	.short	\pos2
	.endm

//-----------------------------------------------------------------------------
/**
 *	�G�g���[�i�[�A�G�|�P�����Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_WORK_ENEMY_SET	init
	.short	FR_STAGE_WORK_ENEMY_SET
	.short	\init
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�e�[�W���[�N�J��
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_WORK_FREE
	.short	FR_STAGE_WORK_FREE
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�e�[�W�|�P�����^�C�v�I���Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_POKE_TYPE_SEL_CALL
	.short	FR_STAGE_POKE_TYPE_SEL_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�e�[�W�Ăяo�����ʎ擾
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_CALL_GET_RESULT
	.short	FR_STAGE_CALL_GET_RESULT
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�e�[�W�퓬�Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_BATTLE_CALL
	.short	FR_STAGE_BATTLE_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�e�[�WSTAGE_SCRWORK�̑���
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_TOOL	code,param1,param2,ret_work
	.short	FR_STAGE_TOOL
	.byte	\code
	.byte	\param1
	.byte	\param2
	.short	\ret_work
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�e�[�W�s�k�`�F�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_LOSE_CHECK	ret_wk
	.short	FR_STAGE_LOSE_CHECK
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�e�[�W�ʐMsend_buf
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_SEND_BUF	type,param,ret_work
	.short	FR_STAGE_SEND_BUF
	.short	\type
	.short	\param
	.short	\ret_work
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�e�[�W�ʐMrecv_buf(wk��type�������ēn���ĉ�����)
 */
//-----------------------------------------------------------------------------
	.macro	_STAGE_RECV_BUF	wk
	.short	FR_STAGE_RECV_BUF
	.short	\wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�X�e�[�W�ΐ�O���b�Z�[�W��\��(tr_idx)
 *  @param	tr_idx	��l�ڂ���l�ڂ��H
 */
//-----------------------------------------------------------------------------
	.macro	_TALKMSG_STAGE_APPEAR	tr_id	
	.short	FR_STAGE_APPEAR_MSG
	.byte	\tr_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	�f�o�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_DEBUG_PRINT	wk_id
	.short	FR_DEBUG_PRINT
	.short	\wk_id
	.endm

//==============================================================================
//	�L���b�X��
//==============================================================================
//-----------------------------------------------------------------------------
/**
 *	�L���b�X�����[�N�m��
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_WORK_ALLOC	init,type,pos1,pos2,pos3,work
	.short	FR_CASTLE_WORK_ALLOC
	.short	\init
	.short	\type
	.short	\pos1
	.short	\pos2
	.short	\pos3
	.short	\work
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X�����[�N������
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_WORK_INIT	init
	.short	FR_CASTLE_WORK_INIT
	.short	\init
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X�����[�N�J��
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_WORK_FREE
	.short	FR_CASTLE_WORK_FREE
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���莝����ʌĂяo��
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_MINE_CALL
	.short	FR_CASTLE_MINE_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���Ăяo�����ʎ擾
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_CALL_GET_RESULT
	.short	FR_CASTLE_CALL_GET_RESULT
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���퓬�Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_BATTLE_CALL
	.short	FR_CASTLE_BATTLE_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���G�g���[�i�[�Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_ENEMY_CALL
	.short	FR_CASTLE_ENEMY_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���퓬�O��POKEPARTY�̃Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_BTL_BEFORE_PARTY_SET
	.short	FR_CASTLE_BTL_BEFORE_PARTY_SET
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���o�g�����POKEPARTY�̃Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_BTL_AFTER_PARTY_SET
	.short	FR_CASTLE_BTL_AFTER_PARTY_SET
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X��CASTLE_SCRWORK�̑���
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_TOOL	code,param1,param2,ret_work
	.short	FR_CASTLE_TOOL
	.byte	\code
	.byte	\param1
	.byte	\param2
	.short	\ret_work
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���s�k�`�F�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_LOSE_CHECK	ret_wk
	.short	FR_CASTLE_LOSE_CHECK
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���ʐMsend_buf
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_SEND_BUF	type,param,ret_work
	.short	FR_CASTLE_SEND_BUF
	.short	\type
	.short	\param
	.short	\ret_work
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���ʐMrecv_buf(wk��type�������ēn���ĉ�����)
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_RECV_BUF	wk
	.short	FR_CASTLE_RECV_BUF
	.short	\wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X�������N�A�b�v�Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_RANKUP_CALL
	.short	FR_CASTLE_RANKUP_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	�e�̌���҂�
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_PARENT_WAIT	ret_wk
	.short	FR_CASTLE_PARENT_CHECK_WAIT
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	CP�擾
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_GET_CP	type,ret_wk
	.short	FR_CASTLE_GET_CP
	.short	\type
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	CP���炷
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_SUB_CP	type,num
	.short	FR_CASTLE_SUB_CP
	.short	\type
	.short	\num
	.endm

//-----------------------------------------------------------------------------
/**
 *	CP���₷
 */
//-----------------------------------------------------------------------------
	.macro	_CASTLE_ADD_CP	type,num
	.short	FR_CASTLE_ADD_CP
	.short	\type
	.short	\num
	.endm

//-----------------------------------------------------------------------------
/**
 *	�L���b�X���ΐ�O���b�Z�[�W��\��(tr_idx)
 *  @param	tr_idx	��l�ڂ���l�ڂ��H
 */
//-----------------------------------------------------------------------------
	.macro	_TALKMSG_CASTLE_APPEAR	tr_id	
	.short	FR_CASTLE_APPEAR_MSG
	.byte	\tr_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	WiFi��t���[�N�m��
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_WORK_ALLOC
	.short	FR_WIFI_COUNTER_WORK_ALLOC
	.endm

//-----------------------------------------------------------------------------
/**
 *	WiFi��t���[�N�J��
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_WORK_FREE
	.short	FR_WIFI_COUNTER_WORK_FREE
	.endm

//-----------------------------------------------------------------------------
/**
 *	WiFi��t�ʐMsend_buf
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_SEND_BUF	type,param,param2,ret_work
	.short	FR_WIFI_COUNTER_SEND_BUF
	.short	\type
	.short	\param
	.short	\param2
	.short	\ret_work
	.endm

//-----------------------------------------------------------------------------
/**
 *	WiFi��t�ʐMrecv_buf(wk��type�������ēn���ĉ�����)
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_RECV_BUF	wk
	.short	FR_WIFI_COUNTER_RECV_BUF
	.short	\wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	WiFi��t �{��No��r
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_BFNO_CHECK	no,wk
	.short	FR_WIFI_COUNTER_BFNO_CHECK
	.short	\no
	.short	\wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	WiFi��t �|�P�������X�g
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_POKELIST	bf_no,type,ret_wk
	.short	FR_WIFI_COUNTER_POKELIST
	.short	\bf_no
	.short	\type
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	WiFi��t �|�P�������X�g�I�����ʎ擾
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_POKELIST_GET_RESULT	ret_wk,ret_wk2
	.short	FR_WIFI_COUNTER_POKELIST_GET_RESULT
	.short	\ret_wk
	.short	\ret_wk2
	.endm

//-----------------------------------------------------------------------------
/**
 *	WiFi��t ��߂�@�I���`�F�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_YAMERU_CHECK	ret_wk
	.short	FR_WIFI_COUNTER_YAMERU_CHECK
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	FRWIFI_SCRWORK�̑���
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_TOOL	code,param1,param2,ret_work
	.short	FR_WIFI_COUNTER_TOOL
	.byte	\code
	.byte	\param1
	.byte	\param2
	.short	\ret_work
	.endm

//-----------------------------------------------------------------------------
/**
 *	�^���[�Ăяo���O�̏���
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_TOWER_CALL_BEFORE
	.short	FR_WIFI_COUNTER_TOWER_CALL_BEFORE
	.endm

//-----------------------------------------------------------------------------
/**
 *	�^���[�g���[�i�[�f�[�^���M
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_TOWER_SEND_TR_DATA	ret_wk
	.short	FR_WIFI_COUNTER_TOWER_SEND_TR_DATA
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	�^���[�g���[�i�[�f�[�^��M
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_TOWER_RECV_TR_DATA
	.short	FR_WIFI_COUNTER_TOWER_RECV_TR_DATA
	.endm

//-----------------------------------------------------------------------------
/**
 *	�^���[�Ăяo����̌�n��
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_TOWER_CALL_AFTER
	.short	FR_WIFI_COUNTER_TOWER_CALL_AFTER
	.endm

//-----------------------------------------------------------------------------
/**
 *	�����̋L�^������(�t�����e�B�A��p)
 */
//-----------------------------------------------------------------------------
	.macro	_FR_WIFI_COUNTER_LIST_SET_LAST_PLAY_DATE
	.short	FR_WIFI_COUNTER_LIST_SET_LAST_PLAY_DATE
	.endm


//==============================================================================
//	���[���b�g
//==============================================================================

//-----------------------------------------------------------------------------
/**
 *	���[���b�g���[�N�m��
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_WORK_ALLOC	init,type,pos1,pos2,pos3,work
	.short	FR_ROULETTE_WORK_ALLOC
	.short	\init
	.short	\type
	.short	\pos1
	.short	\pos2
	.short	\pos3
	.short	\work
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g���[�N������
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_WORK_INIT	init
	.short	FR_ROULETTE_WORK_INIT
	.short	\init
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g���[�N�J��
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_WORK_FREE
	.short	FR_ROULETTE_WORK_FREE
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g�Ăяo�����ʎ擾
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_CALL_GET_RESULT
	.short	FR_ROULETTE_CALL_GET_RESULT
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g�퓬�Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_BATTLE_CALL
	.short	FR_ROULETTE_BATTLE_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g�퓬�O��POKEPARTY�̃Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_BTL_BEFORE_PARTY_SET
	.short	FR_ROULETTE_BTL_BEFORE_PARTY_SET
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g�o�g�����POKEPARTY�̃Z�b�g
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_BTL_AFTER_PARTY_SET
	.short	FR_ROULETTE_BTL_AFTER_PARTY_SET
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�gROULETTE_SCRWORK�̑���
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_TOOL	code,param1,param2,ret_work
	.short	FR_ROULETTE_TOOL
	.byte	\code
	.short	\param1
	.short	\param2
	.short	\ret_work
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g�s�k�`�F�b�N
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_LOSE_CHECK	ret_wk
	.short	FR_ROULETTE_LOSE_CHECK
	.short	\ret_wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g�ʐMsend_buf
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_SEND_BUF	type,param,ret_work
	.short	FR_ROULETTE_SEND_BUF
	.short	\type
	.short	\param
	.short	\ret_work
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g�ʐMrecv_buf(wk��type�������ēn���ĉ�����)
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_RECV_BUF	wk
	.short	FR_ROULETTE_RECV_BUF
	.short	\wk
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g�Ăяo��
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_CALL
	.short	FR_ROULETTE_CALL
	.endm

//-----------------------------------------------------------------------------
/**
 *	���[���b�g�ΐ�O���b�Z�[�W��\��(tr_idx)
 *  @param	tr_idx	��l�ڂ���l�ڂ��H
 */
//-----------------------------------------------------------------------------
	.macro	_TALKMSG_ROULETTE_APPEAR	tr_id	
	.short	FR_ROULETTE_APPEAR_MSG
	.byte	\tr_id
	.endm

//-----------------------------------------------------------------------------
/**
 *	���肵���C�x���g�p�l�����������s
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_DECIDE_EV_NO_FUNC
	.short	FR_ROULETTE_DECIDE_EV_NO_FUNC
	.endm

//-----------------------------------------------------------------------------
/**
 *	�A�C�e�������Ă���A�C�R���̃o�j�b�V��
 */
//-----------------------------------------------------------------------------
	.macro	_ROULETTE_CHG_ITEMKEEP_VANISH	param1,param2,param3
	.short	FR_ROULETTE_CHG_ITEMKEEP_VANISH
	.short	\param1
	.short	\param2
	.short	\param3
	.endm

//-----------------------------------------------------------------------------
/**
 *	TV:�X�e�[�W
 */
//-----------------------------------------------------------------------------
	.macro	_TV_TEMP_STAGE_SET
	.short	FR_TV_TEMP_STAGE_SET
	.endm

//-----------------------------------------------------------------------------
/**
 *	TV:�t�����e�B�A�Œ��ǂ�
 */
//-----------------------------------------------------------------------------
	.macro	_TV_TEMP_FRIEND_SET		no
	.short	FR_TV_TEMP_FRIEND_SET
	.short	\no
	.endm

//-----------------------------------------------------------------------------
/**
 * WIFI�ň�C�Ƀf�[�^�𑗂鎞�̑Ή�
 */
//-----------------------------------------------------------------------------
	.macro	_COMM_SET_WIFI_BOTH_NET	flag
	.short	FR_COMM_SET_WIFI_BOTH_NET
	.short	\flag
	.endm



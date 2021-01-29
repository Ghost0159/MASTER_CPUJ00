//==============================================================================
/**
 * @file	testscr3.s
 * @brief	�ȒP�Ȑ���������
 * @author	matsuda
 * @date	2007.03.30(��)
 */
//==============================================================================

	.text

	.include	"../frontier_seq_def.h"
	.include	"../../../include/msgdata/msg_cmsg_dance.h"


//--------------------------------------------------------------------
//					     �X�N���v�g�{��
//--------------------------------------------------------------------
_EVENT_DATA		test_article		//��ԏ��EVENT_DATA�͎������s
_EVENT_DATA		test_scr_obj0		
_EVENT_DATA		test_scr_obj1		//SCRID_TEST_SCR_OBJ1
_EVENT_DATA		test_scr_obj2		//SCRID_TEST_SCR_OBJ2
_EVENT_DATA_END						//�I��

#define EVENTID_TEST_SCR_ARTICLE	(0)
#define EVENTID_TEST_SCR_OBJ0		(1)
#define EVENTID_TEST_SCR_OBJ1		(2)
#define EVENTID_TEST_SCR_OBJ2		(3)


//==============================================================================
//	�z�u���A�N�^�[
//==============================================================================
//--------------------------------------------------------------
//	�z�u�����\�[�X���x��
//--------------------------------------------------------------
_ARTICLE_RESOURCE_LABEL	article_ball_resource
	_ARTICLE_RESOURCE_DATA	ACTID_TEST_BALL
	_ARTICLE_RESOURCE_DATA_END

//--------------------------------------------------------------
//	�z�u���A�N�^�[
//--------------------------------------------------------------
_ARTICLE_ACTOR_LABEL	article_ball_actor
	_ARTICLE_ACTOR_DATA		ACTID_TEST_BALL, 50, 50, ON, OFF, ACTWORK_0
	_ARTICLE_ACTOR_DATA		ACTID_TEST_BALL, 100, 90, ON, OFF, ACTWORK_1
	_ARTICLE_ACTOR_DATA		ACTID_TEST_BALL, 10, 20, ON, ON, ACTWORK_2
	_ARTICLE_ACTOR_DATA_END


//==============================================================================
//	�z�u���A�N�^�[�e�X�g
//==============================================================================
test_article:
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	//���\�[�X���A�N�^�[�o�^
	_ARTICLE_RESOURCE_SET	article_ball_resource
	_ARTICLE_ACTOR_SET		article_ball_actor
	
	_TIME_WAIT		90, FSW_LOCAL3
	

	//--------------------------------------------------------------
	//
	//	�A�j������e�X�g
	//
	//--------------------------------------------------------------
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_0, 1	//1�Ԃ̃A�j�����s
	_ARTICLE_ACTOR_ANIME_WAIT	ACTWORK_0		//�A�j���I���҂�
	
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_1, 2	//2�Ԃ̃A�j�����s
	_TIME_WAIT		60, FSW_LOCAL3
	_ARTICLE_ACTOR_ANIME_STOP	ACTWORK_1		//�A�j����~
	
//	_TIME_WAIT		60, FSW_LOCAL3

	//�ʉ�ʂɍs���Ă����݂̏�Ԃ̑ޔ��A���A���Ȃ��Ă��邩�m�F
	_NAMEIN_PROC
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_0, 1		//�A�j���J�n
	_TIME_WAIT		60, FSW_LOCAL3
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_1, 2		//�A�j���J�n
	_ARTICLE_ACTOR_ANIME_STOP	ACTWORK_0		//�A�j����~
	_ARTICLE_ACTOR_ANIME_START	ACTWORK_1, 0		//�A�j���J�n
	_TIME_WAIT		60, FSW_LOCAL3
	
	//--------------------------------------------------------------
	//	���\�[�X�ƃA�N�^�[�̍폜�e�X�g
	//		���ۂ̉^�p����_SCRIPT_FINISH�̃^�C�~���O�Ŏ�����������̂�
	//		VRAM��A�N�^�[���[�N���g���񂷕K�v���Ȃ��Ȃ�A�폜�͂��Ȃ���OK
	//--------------------------------------------------------------
/*	
	//���X�ƃA�N�^�[�폜
	_ARTICLE_ACTOR_FREE		ACTWORK_1
	_TIME_WAIT		30, FSW_LOCAL3
	_ARTICLE_ACTOR_FREE		ACTWORK_2
	_TIME_WAIT		30, FSW_LOCAL3
	_ARTICLE_ACTOR_FREE		ACTWORK_0
	_TIME_WAIT		30, FSW_LOCAL3
	
	//���\�[�X���(�A�N�^�[�̓��[�N�ԍ��A���\�[�X��ACTID�ŉ�����鎖�ɒ���)
	_ARTICLE_RESOURCE_FREE	ACTID_TEST_BALL
*/	
	_SCRIPT_FINISH


//==============================================================================
//	�}�b�v�؂�ւ��e�X�g
//==============================================================================
test_scr_obj0:
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_TALKMSG	msg_con_dance_begin_host_03
	_JUMP		test_end

test_scr_obj1:
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_TALKMSG	msg_con_dance_end_host_01
	_JUMP		test_end

test_scr_obj2:
	_BLACK_IN		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK
	_TALKMSG	msg_con_d_tut_02
	_JUMP		test_end


test_end:
	_TIME_WAIT		30, FSW_LOCAL3
	_TALK_CLOSE

	_BLACK_OUT		SCR_WIPE_DIV,SCR_WIPE_SYNC
	_WIPE_FADE_END_CHECK

	//_END
	_SCRIPT_FINISH


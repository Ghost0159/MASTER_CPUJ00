//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_ev_talk.c
 *	@brief		��b�C�x���g
 *	@author		tomoya takahashi
 *	@data		2007.12.18
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "system/wordset.h"
#include "system/msgdata.h"
#include "system/fontproc.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_wifi_aisatu.h"
#include "msgdata/msg_wifi_aisatu_japan.h"
#include "msgdata/msg_wifi_aisatu_english.h"
#include "msgdata/msg_wifi_aisatu_france.h"
#include "msgdata/msg_wifi_aisatu_germany.h"
#include "msgdata/msg_wifi_aisatu_italy.h"
#include "msgdata/msg_wifi_aisatu_spain.h"
#include "msgdata/msg_wifi_hiroba.h"

#include "wflby_event.h"
#include "wflby_snd.h"

#include "wflby_ev_talk.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------

#ifdef PM_DEBUG
#define WFLBY_EV_TALK_DEBUG_SEL_MSG	// ���ԂɎ��₪�o�Ă���
#endif

// ���ԂɎ��₪�o�Ă���
#ifdef WFLBY_EV_TALK_DEBUG_SEL_MSG
static BOOL s_WFLBY_EV_TALK_DEBUG_SEL_MSG_ON		= FALSE;
static s32	s_WFLBY_EV_TALK_DEBUG_SEL_MSG_PAGE	= 0;
#endif


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
/// �b����������V�[�P���X
//=====================================
enum {
	WFLBY_EV_TALK_A_SEQ_INIT,			// ������
	WFLBY_EV_TALK_A_SEQ_REQ,			// ���N�G�X�g
	WFLBY_EV_TALK_A_SEQ_ANSWAIT,		// �񓚑҂�
	WFLBY_EV_TALK_A_SEQ_AISATUMY,		// �������@�����̎���
	WFLBY_EV_TALK_A_SEQ_AISATUYOU,		// ��������	 ����̎���
	WFLBY_EV_TALK_A_SEQ_SYNC_RECV,		// ��b��ԓ���
	WFLBY_EV_TALK_A_SEQ_AISATUFOLLOW,	// �������t�H���[
	WFLBY_EV_TALK_A_SEQ_AISATUEND,		// �������̍Ō�̃��b�Z�[�W
	WFLBY_EV_TALK_A_SEQ_AISATUEND2,		// �������̍Ō�̃��b�Z�[�W�����P��b���Ă�Ƃ�
	WFLBY_EV_TALK_A_SEQ_SEL_GT_FIR,		// �K�W�F�b�g�A��b�Z���N�g	�P���
	WFLBY_EV_TALK_A_SEQ_SEL_GT_SEC,		// �K�W�F�b�g�A��b�Z���N�g	�Q��ڈȍ~
	WFLBY_EV_TALK_A_SEQ_SEL_GT00,		// �K�W�F�b�g�A��b�Z���N�g
	WFLBY_EV_TALK_A_SEQ_SEL_GT00WAIT,	// �K�W�F�b�g�A��b�Z���N�g�@�I��҂�
	WFLBY_EV_TALK_A_SEQ_SEND_G,			// �K�W�F�b�g���炤
	WFLBY_EV_TALK_A_SEQ_SEND_G_GETCHK,	// �K�W�F�b�g���炤���I��
	WFLBY_EV_TALK_A_SEQ_SEND_G_GETCHKWAIT,// �K�W�F�b�g���炤���I���܂�
	WFLBY_EV_TALK_A_SEQ_SEND_G_GET,		// �K�W�F�b�g���炤
	WFLBY_EV_TALK_A_SEQ_SEND_G_RECV_B_SE,// �K�W�F�b�g���炤B�̎�M�m�F
	WFLBY_EV_TALK_A_SEQ_SEND_G_RECV_B,	// �K�W�F�b�g���炤B�̎�M�m�F
	WFLBY_EV_TALK_A_SEQ_SEND_TALK,		// ��b���邱�Ƃ𑗐M
	WFLBY_EV_TALK_A_SEQ_SEL_T_K,		// ��b���ڑI��
	WFLBY_EV_TALK_A_SEQ_SEL_T_K_WAIT,	// ��b���ڑI��҂�
	WFLBY_EV_TALK_A_SEQ_SEL_T_M,		// �����̂��ƑI��
	WFLBY_EV_TALK_A_SEQ_SEL_T_M_SEL,	// �����̂��ƑI��
	WFLBY_EV_TALK_A_SEQ_SEL_T_M_WAIT,	// �����̂��ƑI��
	WFLBY_EV_TALK_A_SEQ_SEND_T,			// ��b�I����񑗐M
	WFLBY_EV_TALK_A_SEQ_SEND_T00,		// ��b�I����񑗐M
	WFLBY_EV_TALK_A_SEQ_SEND_TWAIT,		// ��b�I����񑗐M
	WFLBY_EV_TALK_A_SEQ_RECV_BT,		// ��b�I������M
	WFLBY_EV_TALK_A_SEQ_MSG_BT,			// ������b��񃁃b�Z�[�W�\��
	WFLBY_EV_TALK_A_SEQ_MSG_ABT_EQ,		// �I�����ڂ������Ƃ�
	WFLBY_EV_TALK_A_SEQ_MSG_BT_END,		// �������b�Z�[�W�Ō�
	WFLBY_EV_TALK_A_SEQ_ERR_ENDMSG,		// �G���[�I��
	WFLBY_EV_TALK_A_SEQ_ENDMSG,			// �ʏ�I��
	WFLBY_EV_TALK_A_SEQ_ANSNGMSG,		// �����Ȃ��I��
	WFLBY_EV_TALK_A_SEQ_SELNGMSG,		// B�I���ł��Ȃ��I��
	WFLBY_EV_TALK_A_SEQ_WAIT,			// �E�G�C�g
	WFLBY_EV_TALK_A_SEQ_MSGWAIT,		// ���b�Z�[�W�̏I���E�G�C�g
	WFLBY_EV_TALK_A_SEQ_END,			// �I��
} ;

//-------------------------------------
/// �b��������ꑤ�V�[�P���X
//=====================================
enum {
	WFLBY_EV_TALK_B_SEQ_INIT,		// ������
	WFLBY_EV_TALK_B_SEQ_AISATUYOU,	// �������@�����Ă̎���
	WFLBY_EV_TALK_B_SEQ_AISATUMY,	// ��������	�����̎���
	WFLBY_EV_TALK_B_SEQ_SYNC_RECV,		// ��b��ԓ���
	WFLBY_EV_TALK_B_SEQ_AISATUFOLLOW,	// �������t�H���[
	WFLBY_EV_TALK_B_SEQ_AISATUEND,		// �������̍Ō�̃��b�Z�[�W
	WFLBY_EV_TALK_B_SEQ_AISATUEND2,		// �������̍Ō�̃��b�Z�[�W�����P��b���Ă�Ƃ�
	WFLBY_EV_TALK_B_SEQ_RECV_A_FST,		// ����̑I����e�҂�	�ŏ�
	WFLBY_EV_TALK_B_SEQ_RECV_A_SEC,	// ����̑I����e�҂� �Q��ڈȍ~
	WFLBY_EV_TALK_B_SEQ_RECV_A,		// ����̑I����e�҂�
	WFLBY_EV_TALK_B_SEQ_MSG_G00,		// �K�W�F�b�g���b�Z�[�W
	WFLBY_EV_TALK_B_SEQ_MSG_G01,		// �K�W�F�b�g���b�Z�[�W
	WFLBY_EV_TALK_B_SEQ_MSG_G02,		// �K�W�F�b�g���b�Z�[�W
	WFLBY_EV_TALK_B_SEQ_MSG_G03,		// �K�W�F�b�g���b�Z�[�W
	WFLBY_EV_TALK_B_SEQ_MSG_G_EQ,	// �K�W�F�b�g���ꏏ���b�Z�[�W
	WFLBY_EV_TALK_B_SEQ_MSG_AT00,	// A��b�J�n���b�Z�[�W�\��
	WFLBY_EV_TALK_B_SEQ_RECV_A_SEL,	// A�̉�b���e���擾
	WFLBY_EV_TALK_B_SEQ_MSG_AT01,	// A�̑I�������\��
	WFLBY_EV_TALK_B_SEQ_MSG_AT02,	// A�̑I���Ȃ��\��
	WFLBY_EV_TALK_B_SEQ_MSG_AT03,	// A�̑I��I�񂾂��ƕ\��
	WFLBY_EV_TALK_B_SEQ_MSG_AT04,	// A�̑I������ɂȂ��\��
	WFLBY_EV_TALK_B_SEQ_SEL_T_M,	// �����̂�I��
	WFLBY_EV_TALK_B_SEQ_SEND_T,		// �����̑I���𑗐M
	WFLBY_EV_TALK_B_SEQ_MSG_BT,		// �����̑I���̕\��
	WFLBY_EV_TALK_B_SEQ_MSG_ABT_EQ,	// �I�����ڂ������Ƃ�
	WFLBY_EV_TALK_B_SEQ_MSG_AT_END,	// ��b�I�����b�Z�[�W
	WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG,	// �G���[�I��
	WFLBY_EV_TALK_B_SEQ_ENDMSG,		// �ʏ�I��
	WFLBY_EV_TALK_B_SEQ_NONEENDMSG,	// �������Ȃ��Ƃ��I��
	WFLBY_EV_TALK_B_SEQ_WAIT,			// �E�G�C�g
	WFLBY_EV_TALK_B_SEQ_MSGWAIT,		// ���b�Z�[�W�̏I���E�G�C�g
	WFLBY_EV_TALK_B_SEQ_END,		// �I��
} ;


//-------------------------------------
///	�����o�b�t�@��
//=====================================
#define WFLBY_EV_TALK_STRBUF_NUM	( 128 )

//-------------------------------------
///	���b�Z�[�W�\���E�G�C�g
//=====================================
#define WFLBY_EV_TALK_MSG_WAIT		( 128 )


//-------------------------------------
///	���E�̈��A���
//=====================================
#define WFLBY_EV_TALK_LANG_NUM	( LANG_SPAIN+1 )


//-------------------------------------
///	��b�f�[�^
//=====================================
enum {
	WFLBY_EV_TALK_TYPE_AISATSUSYNC,	// "���A��̓���"
	WFLBY_EV_TALK_TYPE_TALK_BEFORE,	// "��b���ڎ�M����	B���̃_�~�[�ԓ�"
	WFLBY_EV_TALK_TYPE_GADGET_END,	// "��b���ڎ�M����	B���̃_�~�[�ԓ�"
	WFLBY_EV_TALK_TYPE_GADGET,		// "�K�W�F�b�g����"
	WFLBY_EV_TALK_TYPE_GADGET_EQ,	// "�K�W�F�b�g�ꏏ"
	WFLBY_EV_TALK_TYPE_TALK_START,	// "��b�J�n"

	WFLBY_EV_TALK_TYPE_TALK_COL_00,
	WFLBY_EV_TALK_TYPE_TALK_COL_01,
	WFLBY_EV_TALK_TYPE_TALK_COL_02,
	WFLBY_EV_TALK_TYPE_TALK_COL_03,

	WFLBY_EV_TALK_TYPE_TALK_POKE_00,
	WFLBY_EV_TALK_TYPE_TALK_POKE_01,
	WFLBY_EV_TALK_TYPE_TALK_POKE_02,
	WFLBY_EV_TALK_TYPE_TALK_POKE_03,

	WFLBY_EV_TALK_TYPE_TALK_WET_00,
	WFLBY_EV_TALK_TYPE_TALK_WET_01,
	WFLBY_EV_TALK_TYPE_TALK_WET_02,
	WFLBY_EV_TALK_TYPE_TALK_WET_03,

	WFLBY_EV_TALK_TYPE_TALK_ACT_00,
	WFLBY_EV_TALK_TYPE_TALK_ACT_01,
	WFLBY_EV_TALK_TYPE_TALK_ACT_02,
	WFLBY_EV_TALK_TYPE_TALK_ACT_03,

	WFLBY_EV_TALK_TYPE_TALK_YOT_00,
	WFLBY_EV_TALK_TYPE_TALK_YOT_01,
	WFLBY_EV_TALK_TYPE_TALK_YOT_02,
	WFLBY_EV_TALK_TYPE_TALK_YOT_03,

	WFLBY_EV_TALK_TYPE_TALK_TRA_00,
	WFLBY_EV_TALK_TYPE_TALK_TRA_01,
	WFLBY_EV_TALK_TYPE_TALK_TRA_02,
	WFLBY_EV_TALK_TYPE_TALK_TRA_03,

	WFLBY_EV_TALK_TYPE_TALK_LOOK_00,
	WFLBY_EV_TALK_TYPE_TALK_LOOK_01,
	WFLBY_EV_TALK_TYPE_TALK_LOOK_02,
	WFLBY_EV_TALK_TYPE_TALK_LOOK_03,

	WFLBY_EV_TALK_TYPE_TALK_NEED_00,
	WFLBY_EV_TALK_TYPE_TALK_NEED_01,
	WFLBY_EV_TALK_TYPE_TALK_NEED_02,
	WFLBY_EV_TALK_TYPE_TALK_NEED_03,

	WFLBY_EV_TALK_TYPE_TALK_BAD_00,
	WFLBY_EV_TALK_TYPE_TALK_BAD_01,
	WFLBY_EV_TALK_TYPE_TALK_BAD_02,
	WFLBY_EV_TALK_TYPE_TALK_BAD_03,

	WFLBY_EV_TALK_TYPE_TALK_PLAY_00,
	WFLBY_EV_TALK_TYPE_TALK_PLAY_01,
	WFLBY_EV_TALK_TYPE_TALK_PLAY_02,
	WFLBY_EV_TALK_TYPE_TALK_PLAY_03,

	WFLBY_EV_TALK_TYPE_TALK_GAME_00,
	WFLBY_EV_TALK_TYPE_TALK_GAME_01,
	WFLBY_EV_TALK_TYPE_TALK_GAME_02,
	WFLBY_EV_TALK_TYPE_TALK_GAME_03,

	WFLBY_EV_TALK_TYPE_TALK_HAYARI_00,
	WFLBY_EV_TALK_TYPE_TALK_HAYARI_01,
	WFLBY_EV_TALK_TYPE_TALK_HAYARI_02,
	WFLBY_EV_TALK_TYPE_TALK_HAYARI_03,

	WFLBY_EV_TALK_TYPE_TALK_TOKUI_00,
	WFLBY_EV_TALK_TYPE_TALK_TOKUI_01,
	WFLBY_EV_TALK_TYPE_TALK_TOKUI_02,
	WFLBY_EV_TALK_TYPE_TALK_TOKUI_03,

	WFLBY_EV_TALK_TYPE_TALK_YAMERAREN_00,
	WFLBY_EV_TALK_TYPE_TALK_YAMERAREN_01,
	WFLBY_EV_TALK_TYPE_TALK_YAMERAREN_02,
	WFLBY_EV_TALK_TYPE_TALK_YAMERAREN_03,

	WFLBY_EV_TALK_TYPE_TALK_KODAWARI_00,
	WFLBY_EV_TALK_TYPE_TALK_KODAWARI_01,
	WFLBY_EV_TALK_TYPE_TALK_KODAWARI_02,
	WFLBY_EV_TALK_TYPE_TALK_KODAWARI_03,

	WFLBY_EV_TALK_TYPE_TALK_NANISURU_00,
	WFLBY_EV_TALK_TYPE_TALK_NANISURU_01,
	WFLBY_EV_TALK_TYPE_TALK_NANISURU_02,
	WFLBY_EV_TALK_TYPE_TALK_NANISURU_03,

	WFLBY_EV_TALK_TYPE_TALK_HOME_00,
	WFLBY_EV_TALK_TYPE_TALK_HOME_01,
	WFLBY_EV_TALK_TYPE_TALK_HOME_02,
	WFLBY_EV_TALK_TYPE_TALK_HOME_03,

	WFLBY_EV_TALK_TYPE_TALK_FRIPLY_00,
	WFLBY_EV_TALK_TYPE_TALK_FRIPLY_01,
	WFLBY_EV_TALK_TYPE_TALK_FRIPLY_02,
	WFLBY_EV_TALK_TYPE_TALK_FRIPLY_03,

	WFLBY_EV_TALK_TYPE_TALK_PTAISE_00,
	WFLBY_EV_TALK_TYPE_TALK_PTAISE_01,
	WFLBY_EV_TALK_TYPE_TALK_PTAISE_02,
	WFLBY_EV_TALK_TYPE_TALK_PTAISE_03,

	WFLBY_EV_TALK_TYPE_TALK_DONNAP_00,
	WFLBY_EV_TALK_TYPE_TALK_DONNAP_01,
	WFLBY_EV_TALK_TYPE_TALK_DONNAP_02,
	WFLBY_EV_TALK_TYPE_TALK_DONNAP_03,
	
	WFLBY_EV_TALK_TYPE_TALK_SIAWASE_00,
	WFLBY_EV_TALK_TYPE_TALK_SIAWASE_01,
	WFLBY_EV_TALK_TYPE_TALK_SIAWASE_02,
	WFLBY_EV_TALK_TYPE_TALK_SIAWASE_03,

	WFLBY_EV_TALK_TYPE_TALK_KOMARU_00,
	WFLBY_EV_TALK_TYPE_TALK_KOMARU_01,
	WFLBY_EV_TALK_TYPE_TALK_KOMARU_02,
	WFLBY_EV_TALK_TYPE_TALK_KOMARU_03,

	WFLBY_EV_TALK_TYPE_TALK_GANBARU_00,
	WFLBY_EV_TALK_TYPE_TALK_GANBARU_01,
	WFLBY_EV_TALK_TYPE_TALK_GANBARU_02,
	WFLBY_EV_TALK_TYPE_TALK_GANBARU_03,
	
	WFLBY_EV_TALK_TYPE_TALK_SPORTS_00,
	WFLBY_EV_TALK_TYPE_TALK_SPORTS_01,
	WFLBY_EV_TALK_TYPE_TALK_SPORTS_02,
	WFLBY_EV_TALK_TYPE_TALK_SPORTS_03,

	WFLBY_EV_TALK_TYPE_TALK_WORKS_00,
	WFLBY_EV_TALK_TYPE_TALK_WORKS_01,
	WFLBY_EV_TALK_TYPE_TALK_WORKS_02,
	WFLBY_EV_TALK_TYPE_TALK_WORKS_03,

	WFLBY_EV_TALK_TYPE_TALK_ZIMAN_00,
	WFLBY_EV_TALK_TYPE_TALK_ZIMAN_01,
	WFLBY_EV_TALK_TYPE_TALK_ZIMAN_02,
	WFLBY_EV_TALK_TYPE_TALK_ZIMAN_03,

	WFLBY_EV_TALK_TYPE_TALK_DONNA_00,
	WFLBY_EV_TALK_TYPE_TALK_DONNA_01,
	WFLBY_EV_TALK_TYPE_TALK_DONNA_02,
	WFLBY_EV_TALK_TYPE_TALK_DONNA_03,

	WFLBY_EV_TALK_TYPE_TALK_DONNAOTO_00,
	WFLBY_EV_TALK_TYPE_TALK_DONNAOTO_01,
	WFLBY_EV_TALK_TYPE_TALK_DONNAOTO_02,
	WFLBY_EV_TALK_TYPE_TALK_DONNAOTO_03,

	WFLBY_EV_TALK_TYPE_TALK_DONNANIOI_00,
	WFLBY_EV_TALK_TYPE_TALK_DONNANIOI_01,
	WFLBY_EV_TALK_TYPE_TALK_DONNANIOI_02,
	WFLBY_EV_TALK_TYPE_TALK_DONNANIOI_03,

	WFLBY_EV_TALK_TYPE_TALK_KININARU_00,
	WFLBY_EV_TALK_TYPE_TALK_KININARU_01,
	WFLBY_EV_TALK_TYPE_TALK_KININARU_02,
	WFLBY_EV_TALK_TYPE_TALK_KININARU_03,
	

	WFLBY_EV_TALK_TYPE_TALK_NONE,			//  "�킩��Ȃ���I��"

	WFLBY_EV_TALK_TYPE_NUM,
} ;


//-------------------------------------
///	�ŏI�f�[�^�ǉ����b�Z�[�W�I��
//=====================================
enum {
	WFLBY_EV_TALK_LIST_END_NONE,
	WFLBY_EV_TALK_LIST_END_YAMERU,
	WFLBY_EV_TALK_LIST_END_WAKARANAI,
} ;




//-------------------------------------
///	�g�s�b�N�O�̂��o����
//=====================================
enum {
	WFLBY_EV_DEF_PLAYER_TOPIC_INIT,		// ������
	WFLBY_EV_DEF_PLAYER_TOPIC_PARADE,	// PARADE���b�Z�[�W
	WFLBY_EV_DEF_PLAYER_TOPIC_CLOSE,	// �N���[�Y�O���b�Z�[�W
	WFLBY_EV_DEF_PLAYER_TOPIC_MSG00,	// �悤����
	WFLBY_EV_DEF_PLAYER_TOPIC_MSG01,	// �ǂ���ɂ��āE�E�E
	WFLBY_EV_DEF_PLAYER_TOPIC_LISTON,	// ���X�g�\��
	WFLBY_EV_DEF_PLAYER_TOPIC_LISTWAIT,	// ���X�g�҂�
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGGADGET,// �K�W�F�b�g
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI,		// �~�j�Q�[��
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_SEL,	// �~�j�Q�[������ɑI��
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_BS,	// �^�}����
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_BB,	// ���܂̂�
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_BL,	// �ӂ�������
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGWLD,	// ���E���v
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGTOP,	// �j���[�X
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGBOAD,	// ���Ճ{�[�h
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE,	// �C�x���g
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_SEL,		// �C�x���g����ɑI��
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_LIGHTING,	// �C�x���g�@���C�e�B���O
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_FIRE,		// �C�x���g�@�͂Ȃ�
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_PARADE,	// �C�x���g�@�p���[�h
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGANKETO,// �A���P�[�g
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGEND,	// �I����
	WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT,	// ���b�Z�[�W�I���҂�
	WFLBY_EV_DEF_PLAYER_TOPIC_END,	
} ;
#define WFLBY_EV_DEF_PLAYER_TOPIC_LISTWIN_X		( 16 )
#define WFLBY_EV_DEF_PLAYER_TOPIC_LISTWIN_Y		( 1 )
#define WFLBY_EV_DEF_PLAYER_TOPIC_LISTWIN_SIZX	( 15 )

//-------------------------------------
///	�g�s�b�N�O�̂��Z����
//=====================================
enum {
	WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_INIT,		// ������
	WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_MSGWAIT,	// ���b�Z�[�W�\�������҂�
	WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_END,	
} ;






//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�r�b�g�}�b�v���X�g
//=====================================
typedef struct {
	// �r�b�g�}�b�v���X�g��p
	BMPLIST_HEADER		bmplist;
	u32					bmplist_num;	// �r�b�g�}�b�v���X�g��
} WFLBY_EV_TALK_BMPLIST;


//-------------------------------------
///	���肩��_�~�[���b�Z�[�W���A���Ă�����A
//  ���b�Z�[�W�𑗐M����V�X�e��
//=====================================
typedef struct {
	u16		trans_on;
	u16		trans_data;
	u16		wait_seq;
	u16		start_seq;
	u32		recv_talk_seq;
} WFLBY_EV_TALK_RWSEND;



//-------------------------------------
///	��b�C�x���g���[�N
//=====================================
typedef struct {
	u8					sel_k;			// �I�񂾍���
	u8					sel_k_data;		// �I�񂾃f�[�^
	u8					recv_k_data;	// ��M�����I�уf�[�^
	u8					talk_idx;		// ��b�^�[�Q�b�g
	u8					my_timezone;	// �����̎��ԑ�
	u8					you_timezone;	// ����̎��ԑ�
	u8					wait;			// �E�G�C�g
	u8					next_seq;		// �E�G�C�g��̃V�[�P���X
	u16					my_idx;			// �����̃C���f�b�N�X
	u16					err_chk;		// �G���[�`�F�b�N���邩
	u8					gadget_eq;		// �K�W�F�b�g���ꏏ�Ȃ̂��t���O
	u8					sel_wait_flag;	// �I���E�G�C�g�t���O
	s16					sel_wait;		// �I���E�G�C�g
	WFLBY_3DPERSON*		p_npc;			// NPC

	WFLBY_EV_TALK_BMPLIST bmplist;

	WFLBY_EV_TALK_RWSEND	rw_send;	// �_�~�[���b�Z�[�W����M���Ă��瑗�M����V�X�e��

	// �����Ƀf�[�^���Z�b�g���邽�߂̃f�[�^
	u16	rireki_flag;
	u16	rireki_actno;
	s32 rireki_userid;

	BOOL b_count_add;	// ��b�J�E���^��1�������̂�


	// �ގ��������[�U�̃v���t�B�[�����Q�Ƃ���\�����o��̂Ŕp�~
//	const WFLBY_USER_PROFILE* cp_mypr;
//	const WFLBY_USER_PROFILE* cp_youpr;

} WFLBY_EV_TALK_WK;




//-------------------------------------
///	��l����������C�x���g���[�N
//=====================================
typedef struct {
	u32						ret_seq;
	WFLBY_EV_TALK_BMPLIST	bmplist;
} WFLBY_EV_DEF_PLAYER_WFTOPIC_WK;

//-----------------------------------------------------------------------------
/**
 *			���X�g�f�[�^
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	��{�w�b�_�[
//=====================================
static const BMPLIST_HEADER sc_WFLBY_EV_TALK_HEADER = {
	NULL,
	NULL,
	NULL,
	NULL,

	0,	//���X�g���ڐ�
	6,	//�\���ő區�ڐ�
	0,								//���x���\���w���W
	8,								//���ڕ\���w���W
	0,								//�J�[�\���\���w���W
	0,								//�\���x���W
	FBMP_COL_BLACK,					//�\�������F
	FBMP_COL_WHITE,					//�\���w�i�F
	FBMP_COL_BLK_SDW,				//�\�������e�F
	0,								//�����Ԋu�w
	16,								//�����Ԋu�x
	BMPLIST_NO_SKIP,				//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,					//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0,								//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)

	NULL
};




//-------------------------------------
///	�K�W�F�b�g����b���I��
//=====================================
enum{	
	WFLBY_EV_TALK_LIST_SEL_GT_GAGET,
	WFLBY_EV_TALK_LIST_SEL_GT_TALK,
	WFLBY_EV_TALK_LIST_SEL_GT_END,
	WFLBY_EV_TALK_LIST_SEL_GT_NUM,
};

//-------------------------------------
///	�K�W�F�b�g���炤���I��
//=====================================
enum{	
	WFLBY_EV_TALK_LIST_SEL_G_GET,
	WFLBY_EV_TALK_LIST_SEL_G_END,
	WFLBY_EV_TALK_LIST_SEL_G_NUM,
};

//-------------------------------------
///	��b���ڑI��
//=====================================
enum{	
	WFLBY_EV_TALK_LIST_SEL_TK_COL,		// �F
	WFLBY_EV_TALK_LIST_SEL_TK_POKE,		// �|�P
	WFLBY_EV_TALK_LIST_SEL_TK_WEA,		// �V�C
	WFLBY_EV_TALK_LIST_SEL_TK_ACT,		// �Ȃɂ��Ă�
	WFLBY_EV_TALK_LIST_SEL_TK_YOTE,		// �\��
	WFLBY_EV_TALK_LIST_SEL_TK_TRA,		// ��傱��
	WFLBY_EV_TALK_LIST_SEL_TK_LOOK,		// �ɂĂ�
	WFLBY_EV_TALK_LIST_SEL_TK_NEED,		// �ق�������
	WFLBY_EV_TALK_LIST_SEL_TK_BAD,		// �ɂ��ĂȂ���
	WFLBY_EV_TALK_LIST_SEL_TK_PLAY,		// �����тɂ����Ȃ�
	WFLBY_EV_TALK_LIST_SEL_TK_GAME,		// �Ƃ����ȃQ�[��
	WFLBY_EV_TALK_LIST_SEL_TK_HAYARI,	// �͂���Ă邱�Ƃ�
	WFLBY_EV_TALK_LIST_SEL_TK_TOKUI,	// �Ƃ����Ȃ��Ƃ�
	WFLBY_EV_TALK_LIST_SEL_TK_YEMERAREN,// ��߂�Ȃ�����
	WFLBY_EV_TALK_LIST_SEL_TK_KODAWARI,	// ��������Ă邱��
	WFLBY_EV_TALK_LIST_SEL_TK_NANISURU,	// ���̂��ƂȂɂ���
	WFLBY_EV_TALK_LIST_SEL_TK_HOME,		// �Ƃ̎���ɂȂɂ�����
	WFLBY_EV_TALK_LIST_SEL_TK_FRIPLY,	// �F�B�ƂȂɂ��Ă�����
	WFLBY_EV_TALK_LIST_SEL_TK_PTAISE,	// �������ȂЂƂ͂���
	WFLBY_EV_TALK_LIST_SEL_TK_DONNAP,	// �ǂ�ȂЂƂ��Ă�����
	WFLBY_EV_TALK_LIST_SEL_TK_SIAWASE,	// �����킹���Ăǂ�ȂƂ�
	WFLBY_EV_TALK_LIST_SEL_TK_KOMARU,	// ���܂��Ă邱�Ƃ͂���
	WFLBY_EV_TALK_LIST_SEL_TK_GANBARU,	// ����΂��Ă邱�Ƃ͂���
	WFLBY_EV_TALK_LIST_SEL_TK_SPORTS,	// �Ƃ����ȃX�|�[�c
	WFLBY_EV_TALK_LIST_SEL_TK_WORKS,	// ����̐E��
	WFLBY_EV_TALK_LIST_SEL_TK_ZIMAN,	// ����ł�Ƃ���̎���
	WFLBY_EV_TALK_LIST_SEL_TK_DONNA,	// ����ł�Ƃ���͂ǂ�ȂƂ�
	WFLBY_EV_TALK_LIST_SEL_TK_DONNAOTO,	// �ǂ�Ȃ��Ƃ��Ă�
	WFLBY_EV_TALK_LIST_SEL_TK_DONNANIOI,// �ǂ�Ȃɂ������Ă�
	WFLBY_EV_TALK_LIST_SEL_TK_KININARU,	// �����ɂȂ��Ă邱�Ƃ���
	
//	WFLBY_EV_TALK_LIST_SEL_TK_END,		// ��߂�
	WFLBY_EV_TALK_LIST_SEL_TK_NUM,
};
#define WFLBY_EV_TALK_TK_SEL_NUM			( 4 )
#define WFLBY_EV_TALK_TK_LISTWIN_X			( 8 )
#define WFLBY_EV_TALK_TK_LISTWIN_Y			( 3 )
#define WFLBY_EV_TALK_TK_LISTWIN_SIZX			( 23 )

//-------------------------------------
///	��b���e
//=====================================
enum{	
	WFLBY_EV_TALK_LIST_SEL_TK_S_00,	
	WFLBY_EV_TALK_LIST_SEL_TK_S_01,	
	WFLBY_EV_TALK_LIST_SEL_TK_S_02,	
	WFLBY_EV_TALK_LIST_SEL_TK_S_04,	
	WFLBY_EV_TALK_LIST_SEL_TK_S_MAX,			// A[���ڐ�]	B[�킩��Ȃ�]
	WFLBY_EV_TALK_LIST_SEL_TK_S_B_MAX,			// ���ڐ�
};


//-------------------------------------
///	�g�s�b�N���o���񃊃X�g
//=====================================
enum {
	WFLBY_EV_TOPIC_LIST_SEL_GADGET,
	WFLBY_EV_TOPIC_LIST_SEL_MINI,
	WFLBY_EV_TOPIC_LIST_SEL_WLD,
	WFLBY_EV_TOPIC_LIST_SEL_TOP,
	WFLBY_EV_TOPIC_LIST_SEL_FOOT,
	WFLBY_EV_TOPIC_LIST_SEL_ANKETO,
	WFLBY_EV_TOPIC_LIST_SEL_TIME,	
	WFLBY_EV_TOPIC_LIST_SEL_END,	
	WFLBY_EV_TOPIC_LIST_SEL_NUM,	
} ;
// �C�x���g���X�g
enum {
	WFLBY_EV_TOPIC_LIST_SEL_EVE_LIGHTING,
	WFLBY_EV_TOPIC_LIST_SEL_EVE_FIRE,
	WFLBY_EV_TOPIC_LIST_SEL_EVE_PARADE,
	WFLBY_EV_TOPIC_LIST_SEL_EVE_END,
	WFLBY_EV_TOPIC_LIST_SEL_EVE_NUM,	
} ;
// �~�j�Q�[�����X�g
enum {
	WFLBY_EV_TOPIC_LIST_SEL_MG_BALLSLOW,
	WFLBY_EV_TOPIC_LIST_SEL_MG_BALANCE,
	WFLBY_EV_TOPIC_LIST_SEL_MG_BALLOON,
	WFLBY_EV_TOPIC_LIST_SEL_MG_END,
	WFLBY_EV_TOPIC_LIST_SEL_MG_NUM,	
} ;




//-----------------------------------------------------------------------------
/**
 *		VIP���b�Z�[�W�f�[�^�e�[�u��
 */
//-----------------------------------------------------------------------------
typedef struct{
	u16 normal_msg;
	u16 vip_msg;
} WFLBY_VIP_MSGDATA;
static const WFLBY_VIP_MSGDATA sc_WFLBY_VIP_MSGTBL[] = {
	{ msg_hiroba_aisatu_03_01, msg_vip_aisatu_03_01 },
	{ msg_hiroba_aisatu_03_02, msg_vip_aisatu_03_02 },
	{ msg_hiroba_aisatu_04_02, msg_vip_aisatu_04_02 },
	{ msg_hiroba_aisatu_04_03, msg_vip_aisatu_04_03 },
	{ msg_hiroba_osyaberi_00_01, msg_vip_osyaberi_00_01 },
	{ msg_hiroba_osyaberi_00_02, msg_vip_osyaberi_00_02 },
	{ msg_hiroba_osyaberi_00_03, msg_vip_osyaberi_00_03 },
	{ msg_hiroba_osyaberi_00_04, msg_vip_osyaberi_00_04 },
	{ msg_hiroba_osyaberi_00_05, msg_vip_osyaberi_00_05 },
	{ msg_hiroba_osyaberi_00_06, msg_vip_osyaberi_00_06 },
	{ msg_hiroba_osyaberi_00_07, msg_vip_osyaberi_00_07 },
	{ msg_hiroba_osyaberi_00_08, msg_vip_osyaberi_00_08 },
	{ msg_hiroba_osyaberi_00_09, msg_vip_osyaberi_00_09 },
	{ msg_hiroba_osyaberi_01_01, msg_vip_osyaberi_01_01 },
	{ msg_hiroba_osyaberi_01_02, msg_vip_osyaberi_01_02 },
	{ msg_hiroba_osyaberi_01_03, msg_vip_osyaberi_01_03 },
	{ msg_hiroba_osyaberi_01_04, msg_vip_osyaberi_01_04 },
	{ msg_hiroba_osyaberi_01_05, msg_vip_osyaberi_01_05 },
	{ msg_hiroba_osyaberi_01_06, msg_vip_osyaberi_01_06 },
	{ msg_hiroba_osyaberi_01_07, msg_vip_osyaberi_01_07 },
	{ msg_hiroba_osyaberi_01_08, msg_vip_osyaberi_01_08 },
	{ msg_hiroba_osyaberi_01_09, msg_vip_osyaberi_01_09 },
	{ msg_hiroba_osyaberi_01_10, msg_vip_osyaberi_01_10 },
	{ msg_hiroba_osyaberi_02_01, msg_vip_osyaberi_02_01 },
	{ msg_hiroba_osyaberi_02_02, msg_vip_osyaberi_02_02 },
	{ msg_hiroba_osyaberi_02_03, msg_vip_osyaberi_02_03 },
	{ msg_hiroba_osyaberi_02_04, msg_vip_osyaberi_02_04 },
	{ msg_hiroba_osyaberi_02_05, msg_vip_osyaberi_02_05 },
	{ msg_hiroba_osyaberi_02_06, msg_vip_osyaberi_02_06 },
	{ msg_hiroba_osyaberi_02_07, msg_vip_osyaberi_02_07 },
	{ msg_hiroba_osyaberi_02_08, msg_vip_osyaberi_02_08 },
	{ msg_hiroba_osyaberi_02_09, msg_vip_osyaberi_02_09 },
	{ msg_hiroba_osyaberi_02_10, msg_vip_osyaberi_02_10 },
	{ msg_hiroba_osyaberi_04_01, msg_vip_osyaberi_04_01 },
	{ msg_hiroba_osyaberi_04_02, msg_vip_osyaberi_04_02 },
	{ msg_hiroba_osyaberi_04_03, msg_vip_osyaberi_04_03 },
	{ msg_hiroba_osyaberi_04_04, msg_vip_osyaberi_04_04 },
	{ msg_hiroba_osyaberi_04_05, msg_vip_osyaberi_04_05 },
	{ msg_hiroba_osyaberi_04_06, msg_vip_osyaberi_04_06 },
	{ msg_hiroba_osyaberi_04_07, msg_vip_osyaberi_04_07 },
	{ msg_hiroba_osyaberi_04_08, msg_vip_osyaberi_04_08 },
	{ msg_hiroba_osyaberi_04_09, msg_vip_osyaberi_04_09 },
	{ msg_hiroba_osyaberi_04_10, msg_vip_osyaberi_04_10 },
	{ msg_hiroba_osyaberi_04_11, msg_vip_osyaberi_04_11 },
	{ msg_hiroba_osyaberi_04_12, msg_vip_osyaberi_04_12 },
	{ msg_hiroba_osyaberi_04_13, msg_vip_osyaberi_04_13 },
	{ msg_hiroba_osyaberi_04_14, msg_vip_osyaberi_04_14 },
	{ msg_hiroba_osyaberi_04_15, msg_vip_osyaberi_04_15 },
	{ msg_hiroba_osyaberi_04_16, msg_vip_osyaberi_04_16 },
	{ msg_hiroba_osyaberi_04_17, msg_vip_osyaberi_04_17 },
	{ msg_hiroba_osyaberi_04_18, msg_vip_osyaberi_04_18 },
	{ msg_hiroba_osyaberi_04_19, msg_vip_osyaberi_04_19 },
	{ msg_hiroba_osyaberi_04_20, msg_vip_osyaberi_04_20 },
	{ msg_hiroba_osyaberi_04_21, msg_vip_osyaberi_04_21 },
	{ msg_hiroba_osyaberi_04_22, msg_vip_osyaberi_04_22 },
	{ msg_hiroba_osyaberi_04_23, msg_vip_osyaberi_04_23 },
	{ msg_hiroba_osyaberi_04_24, msg_vip_osyaberi_04_24 },
	{ msg_hiroba_osyaberi_04_25, msg_vip_osyaberi_04_25 },
	{ msg_hiroba_osyaberi_04_26, msg_vip_osyaberi_04_26 },
	{ msg_hiroba_osyaberi_04_27, msg_vip_osyaberi_04_27 },
	{ msg_hiroba_osyaberi_04_28, msg_vip_osyaberi_04_28 },
	{ msg_hiroba_osyaberi_04_29, msg_vip_osyaberi_04_29 },
	{ msg_hiroba_osyaberi_04_30, msg_vip_osyaberi_04_30 },
	{ msg_hiroba_osyaberi_04_31, msg_vip_osyaberi_04_31 },
	{ msg_hiroba_osyaberi_04_32, msg_vip_osyaberi_04_32 },
	{ msg_hiroba_osyaberi_04_33, msg_vip_osyaberi_04_33 },
	{ msg_hiroba_osyaberi_04_34, msg_vip_osyaberi_04_34 },
	{ msg_hiroba_osyaberi_04_35, msg_vip_osyaberi_04_35 },
	{ msg_hiroba_osyaberi_04_36, msg_vip_osyaberi_04_36 },
	{ msg_hiroba_osyaberi_04_37, msg_vip_osyaberi_04_37 },
	{ msg_hiroba_osyaberi_04_38, msg_vip_osyaberi_04_38 },
	{ msg_hiroba_osyaberi_04_39, msg_vip_osyaberi_04_39 },
	{ msg_hiroba_osyaberi_04_40, msg_vip_osyaberi_04_40 },
	{ msg_hiroba_osyaberi_04_41, msg_vip_osyaberi_04_41 },
	{ msg_hiroba_osyaberi_05_01, msg_vip_osyaberi_05_01 },
	{ msg_hiroba_osyaberi_05_02, msg_vip_osyaberi_05_02 },
	{ msg_hiroba_osyaberi_05_03, msg_vip_osyaberi_05_03 },
	{ msg_hiroba_osyaberi_05_04, msg_vip_osyaberi_05_04 },
	{ msg_hiroba_osyaberi_05_05, msg_vip_osyaberi_05_05 },
	{ msg_hiroba_osyaberi_05_06, msg_vip_osyaberi_05_06 },
	{ msg_hiroba_osyaberi_05_07, msg_vip_osyaberi_05_07 },
	{ msg_hiroba_osyaberi_05_08, msg_vip_osyaberi_05_08 },
	{ msg_hiroba_osyaberi_05_09, msg_vip_osyaberi_05_09 },
	{ msg_hiroba_osyaberi_05_10, msg_vip_osyaberi_05_10 },
	{ msg_hiroba_osyaberi_05_11, msg_vip_osyaberi_05_11 },
	{ msg_hiroba_osyaberi_05_12, msg_vip_osyaberi_05_12 },
	{ msg_hiroba_osyaberi_05_13, msg_vip_osyaberi_05_13 },
	{ msg_hiroba_osyaberi_05_14, msg_vip_osyaberi_05_14 },
	{ msg_hiroba_osyaberi_05_15, msg_vip_osyaberi_05_15 },
	{ msg_hiroba_osyaberi_05_16, msg_vip_osyaberi_05_16 },
	{ msg_hiroba_osyaberi_05_17, msg_vip_osyaberi_05_17 },
	{ msg_hiroba_osyaberi_05_18, msg_vip_osyaberi_05_18 },
	{ msg_hiroba_osyaberi_05_19, msg_vip_osyaberi_05_19 },
	{ msg_hiroba_osyaberi_05_20, msg_vip_osyaberi_05_20 },
	{ msg_hiroba_osyaberi_05_21, msg_vip_osyaberi_05_21 },
	{ msg_hiroba_osyaberi_05_22, msg_vip_osyaberi_05_22 },
	{ msg_hiroba_osyaberi_05_23, msg_vip_osyaberi_05_23 },
	{ msg_hiroba_osyaberi_05_24, msg_vip_osyaberi_05_24 },
	{ msg_hiroba_osyaberi_05_25, msg_vip_osyaberi_05_25 },
	{ msg_hiroba_osyaberi_05_26, msg_vip_osyaberi_05_26 },
	{ msg_hiroba_osyaberi_05_27, msg_vip_osyaberi_05_27 },
	{ msg_hiroba_osyaberi_05_28, msg_vip_osyaberi_05_28 },
	{ msg_hiroba_osyaberi_05_29, msg_vip_osyaberi_05_29 },
	{ msg_hiroba_osyaberi_05_30, msg_vip_osyaberi_05_30 },
	{ msg_hiroba_osyaberi_05_31, msg_vip_osyaberi_05_31 },
	{ msg_hiroba_osyaberi_05_32, msg_vip_osyaberi_05_32 },
	{ msg_hiroba_osyaberi_05_33, msg_vip_osyaberi_05_33 },
	{ msg_hiroba_osyaberi_05_34, msg_vip_osyaberi_05_34 },
	{ msg_hiroba_osyaberi_05_35, msg_vip_osyaberi_05_35 },
	{ msg_hiroba_osyaberi_05_36, msg_vip_osyaberi_05_36 },
	{ msg_hiroba_osyaberi_05_37, msg_vip_osyaberi_05_37 },
	{ msg_hiroba_osyaberi_05_38, msg_vip_osyaberi_05_38 },
	{ msg_hiroba_osyaberi_05_39, msg_vip_osyaberi_05_39 },
	{ msg_hiroba_osyaberi_05_40, msg_vip_osyaberi_05_40 },
	{ msg_hiroba_osyaberi_06_01, msg_vip_osyaberi_06_01 },
	{ msg_hiroba_osyaberi_06_02, msg_vip_osyaberi_06_02 },
	{ msg_hiroba_osyaberi_06_03, msg_vip_osyaberi_06_03 },
	{ msg_hiroba_osyaberi_06_04, msg_vip_osyaberi_06_04 },
	{ msg_hiroba_osyaberi_06_05, msg_vip_osyaberi_06_05 },
	{ msg_hiroba_osyaberi_06_06, msg_vip_osyaberi_06_06 },
	{ msg_hiroba_osyaberi_06_07, msg_vip_osyaberi_06_07 },
	{ msg_hiroba_osyaberi_06_08, msg_vip_osyaberi_06_08 },
	{ msg_hiroba_osyaberi_06_09, msg_vip_osyaberi_06_09 },
	{ msg_hiroba_osyaberi_06_10, msg_vip_osyaberi_06_10 },
	{ msg_hiroba_item_00_01, msg_vip_item_00_01 },
	{ msg_hiroba_item_00_02, msg_vip_item_00_02 },
	{ msg_hiroba_item_00_03, msg_vip_item_00_03 },
	{ msg_hiroba_item_00_04, msg_vip_item_00_04 },
	{ msg_hiroba_item_00_05, msg_vip_item_00_05 },
	{ msg_hiroba_item_00_06, msg_vip_item_00_06 },
	{ msg_hiroba_item_00_07, msg_vip_item_00_07 },
	{ msg_hiroba_item_00_08, msg_vip_item_00_08 },
	{ msg_hiroba_osyaberi_07_01, msg_vip_osyaberi_07_01 }, 
	{ msg_hiroba_osyaberi_07_02, msg_vip_osyaberi_07_02 }, 
	{ msg_hiroba_osyaberi_08_01, msg_vip_osyaberi_08_01 }, 
	{ msg_hiroba_osyaberi_01_11, msg_vip_osyaberi_01_11 },
	{ msg_hiroba_osyaberi_01_12, msg_vip_osyaberi_01_12 },
	{ msg_hiroba_osyaberi_01_13, msg_vip_osyaberi_01_13 },
	{ msg_hiroba_osyaberi_01_14, msg_vip_osyaberi_01_14 },
	{ msg_hiroba_osyaberi_01_15, msg_vip_osyaberi_01_15 },
	{ msg_hiroba_osyaberi_01_16, msg_vip_osyaberi_01_16 },
	{ msg_hiroba_osyaberi_01_17, msg_vip_osyaberi_01_17 },
	{ msg_hiroba_osyaberi_01_18, msg_vip_osyaberi_01_18 },
	{ msg_hiroba_osyaberi_01_19, msg_vip_osyaberi_01_19 },
	{ msg_hiroba_osyaberi_01_20, msg_vip_osyaberi_01_20 },
	{ msg_hiroba_osyaberi_01_21, msg_vip_osyaberi_01_21 },
	{ msg_hiroba_osyaberi_01_22, msg_vip_osyaberi_01_22 },
	{ msg_hiroba_osyaberi_01_23, msg_vip_osyaberi_01_23 },
	{ msg_hiroba_osyaberi_01_24, msg_vip_osyaberi_01_24 },
	{ msg_hiroba_osyaberi_01_25, msg_vip_osyaberi_01_25 },
	{ msg_hiroba_osyaberi_01_26, msg_vip_osyaberi_01_26 },
	{ msg_hiroba_osyaberi_01_27, msg_vip_osyaberi_01_27 },
	{ msg_hiroba_osyaberi_01_28, msg_vip_osyaberi_01_28 },
	{ msg_hiroba_osyaberi_01_29, msg_vip_osyaberi_01_29 },
	{ msg_hiroba_osyaberi_01_30, msg_vip_osyaberi_01_30 },
	{ msg_hiroba_osyaberi_02_11, msg_vip_osyaberi_02_11 },
	{ msg_hiroba_osyaberi_02_12, msg_vip_osyaberi_02_12 },
	{ msg_hiroba_osyaberi_02_13, msg_vip_osyaberi_02_13 },
	{ msg_hiroba_osyaberi_02_14, msg_vip_osyaberi_02_14 },
	{ msg_hiroba_osyaberi_02_15, msg_vip_osyaberi_02_15 },
	{ msg_hiroba_osyaberi_02_16, msg_vip_osyaberi_02_16 },
	{ msg_hiroba_osyaberi_02_17, msg_vip_osyaberi_02_17 },
	{ msg_hiroba_osyaberi_02_18, msg_vip_osyaberi_02_18 },
	{ msg_hiroba_osyaberi_02_19, msg_vip_osyaberi_02_19 },
	{ msg_hiroba_osyaberi_02_20, msg_vip_osyaberi_02_20 },
	{ msg_hiroba_osyaberi_02_21, msg_vip_osyaberi_02_21 },
	{ msg_hiroba_osyaberi_02_22, msg_vip_osyaberi_02_22 },
	{ msg_hiroba_osyaberi_02_23, msg_vip_osyaberi_02_23 },
	{ msg_hiroba_osyaberi_02_24, msg_vip_osyaberi_02_24 },
	{ msg_hiroba_osyaberi_02_25, msg_vip_osyaberi_02_25 },
	{ msg_hiroba_osyaberi_02_26, msg_vip_osyaberi_02_26 },
	{ msg_hiroba_osyaberi_02_27, msg_vip_osyaberi_02_27 },
	{ msg_hiroba_osyaberi_02_28, msg_vip_osyaberi_02_28 },
	{ msg_hiroba_osyaberi_02_29, msg_vip_osyaberi_02_29 },
	{ msg_hiroba_osyaberi_02_30, msg_vip_osyaberi_02_30 },
	{ msg_hiroba_osyaberi_04_42, msg_vip_osyaberi_04_42 },
	{ msg_hiroba_osyaberi_04_43, msg_vip_osyaberi_04_43 },
	{ msg_hiroba_osyaberi_04_44, msg_vip_osyaberi_04_44 },
	{ msg_hiroba_osyaberi_04_45, msg_vip_osyaberi_04_45 },
	{ msg_hiroba_osyaberi_04_46, msg_vip_osyaberi_04_46 },
	{ msg_hiroba_osyaberi_04_47, msg_vip_osyaberi_04_47 },
	{ msg_hiroba_osyaberi_04_48, msg_vip_osyaberi_04_48 },
	{ msg_hiroba_osyaberi_04_49, msg_vip_osyaberi_04_49 },
	{ msg_hiroba_osyaberi_04_50, msg_vip_osyaberi_04_50 },
	{ msg_hiroba_osyaberi_04_51, msg_vip_osyaberi_04_51 },
	{ msg_hiroba_osyaberi_04_52, msg_vip_osyaberi_04_52 },
	{ msg_hiroba_osyaberi_04_53, msg_vip_osyaberi_04_53 },
	{ msg_hiroba_osyaberi_04_54, msg_vip_osyaberi_04_54 },
	{ msg_hiroba_osyaberi_04_55, msg_vip_osyaberi_04_55 },
	{ msg_hiroba_osyaberi_04_56, msg_vip_osyaberi_04_56 },
	{ msg_hiroba_osyaberi_04_57, msg_vip_osyaberi_04_57 },
	{ msg_hiroba_osyaberi_04_58, msg_vip_osyaberi_04_58 },
	{ msg_hiroba_osyaberi_04_59, msg_vip_osyaberi_04_59 },
	{ msg_hiroba_osyaberi_04_60, msg_vip_osyaberi_04_60 },
	{ msg_hiroba_osyaberi_04_61, msg_vip_osyaberi_04_61 },
	{ msg_hiroba_osyaberi_04_62, msg_vip_osyaberi_04_62 },
	{ msg_hiroba_osyaberi_04_63, msg_vip_osyaberi_04_63 },
	{ msg_hiroba_osyaberi_04_64, msg_vip_osyaberi_04_64 },
	{ msg_hiroba_osyaberi_04_65, msg_vip_osyaberi_04_65 },
	{ msg_hiroba_osyaberi_04_66, msg_vip_osyaberi_04_66 },
	{ msg_hiroba_osyaberi_04_67, msg_vip_osyaberi_04_67 },
	{ msg_hiroba_osyaberi_04_68, msg_vip_osyaberi_04_68 },
	{ msg_hiroba_osyaberi_04_69, msg_vip_osyaberi_04_69 },
	{ msg_hiroba_osyaberi_04_70, msg_vip_osyaberi_04_70 },
	{ msg_hiroba_osyaberi_04_71, msg_vip_osyaberi_04_71 },
	{ msg_hiroba_osyaberi_04_72, msg_vip_osyaberi_04_72 },
	{ msg_hiroba_osyaberi_04_73, msg_vip_osyaberi_04_73 },
	{ msg_hiroba_osyaberi_04_74, msg_vip_osyaberi_04_74 },
	{ msg_hiroba_osyaberi_04_75, msg_vip_osyaberi_04_75 },
	{ msg_hiroba_osyaberi_04_76, msg_vip_osyaberi_04_76 },
	{ msg_hiroba_osyaberi_04_77, msg_vip_osyaberi_04_77 },
	{ msg_hiroba_osyaberi_04_78, msg_vip_osyaberi_04_78 },
	{ msg_hiroba_osyaberi_04_79, msg_vip_osyaberi_04_79 },
	{ msg_hiroba_osyaberi_04_80, msg_vip_osyaberi_04_80 },
	{ msg_hiroba_osyaberi_04_81, msg_vip_osyaberi_04_81 },
	{ msg_hiroba_osyaberi_04_82, msg_vip_osyaberi_04_82 },
	{ msg_hiroba_osyaberi_04_83, msg_vip_osyaberi_04_83 },
	{ msg_hiroba_osyaberi_04_84, msg_vip_osyaberi_04_84 },
	{ msg_hiroba_osyaberi_04_85, msg_vip_osyaberi_04_85 },
	{ msg_hiroba_osyaberi_04_86, msg_vip_osyaberi_04_86 },
	{ msg_hiroba_osyaberi_04_87, msg_vip_osyaberi_04_87 },
	{ msg_hiroba_osyaberi_04_88, msg_vip_osyaberi_04_88 },
	{ msg_hiroba_osyaberi_04_89, msg_vip_osyaberi_04_89 },
	{ msg_hiroba_osyaberi_04_90, msg_vip_osyaberi_04_90 },
	{ msg_hiroba_osyaberi_04_91, msg_vip_osyaberi_04_91 },
	{ msg_hiroba_osyaberi_04_92, msg_vip_osyaberi_04_92 },
	{ msg_hiroba_osyaberi_04_93, msg_vip_osyaberi_04_93 },
	{ msg_hiroba_osyaberi_04_94, msg_vip_osyaberi_04_94 },
	{ msg_hiroba_osyaberi_04_95, msg_vip_osyaberi_04_95 },
	{ msg_hiroba_osyaberi_04_96, msg_vip_osyaberi_04_96 },
	{ msg_hiroba_osyaberi_04_97, msg_vip_osyaberi_04_97 },
	{ msg_hiroba_osyaberi_04_98, msg_vip_osyaberi_04_98 },
	{ msg_hiroba_osyaberi_04_99, msg_vip_osyaberi_04_99 },
	{ msg_hiroba_osyaberi_04_100, msg_vip_osyaberi_04_100 },
	{ msg_hiroba_osyaberi_04_101, msg_vip_osyaberi_04_101 },
	{ msg_hiroba_osyaberi_04_102, msg_vip_osyaberi_04_102 },
	{ msg_hiroba_osyaberi_04_103, msg_vip_osyaberi_04_103 },
	{ msg_hiroba_osyaberi_04_104, msg_vip_osyaberi_04_104 },
	{ msg_hiroba_osyaberi_04_105, msg_vip_osyaberi_04_105 },
	{ msg_hiroba_osyaberi_04_106, msg_vip_osyaberi_04_106 },
	{ msg_hiroba_osyaberi_04_107, msg_vip_osyaberi_04_107 },
	{ msg_hiroba_osyaberi_04_108, msg_vip_osyaberi_04_108 },
	{ msg_hiroba_osyaberi_04_109, msg_vip_osyaberi_04_109 },
	{ msg_hiroba_osyaberi_04_110, msg_vip_osyaberi_04_110 },
	{ msg_hiroba_osyaberi_04_111, msg_vip_osyaberi_04_111 },
	{ msg_hiroba_osyaberi_04_112, msg_vip_osyaberi_04_112 },
	{ msg_hiroba_osyaberi_04_113, msg_vip_osyaberi_04_113 },
	{ msg_hiroba_osyaberi_04_114, msg_vip_osyaberi_04_114 },
	{ msg_hiroba_osyaberi_04_115, msg_vip_osyaberi_04_115 },
	{ msg_hiroba_osyaberi_04_116, msg_vip_osyaberi_04_116 },
	{ msg_hiroba_osyaberi_04_117, msg_vip_osyaberi_04_117 },
	{ msg_hiroba_osyaberi_04_118, msg_vip_osyaberi_04_118 },
	{ msg_hiroba_osyaberi_04_119, msg_vip_osyaberi_04_119 },
	{ msg_hiroba_osyaberi_04_120, msg_vip_osyaberi_04_120 },
	{ msg_hiroba_osyaberi_04_121, msg_vip_osyaberi_04_121 },
	{ msg_hiroba_osyaberi_05_41, msg_vip_osyaberi_05_41 },
	{ msg_hiroba_osyaberi_05_42, msg_vip_osyaberi_05_42 },
	{ msg_hiroba_osyaberi_05_43, msg_vip_osyaberi_05_43 },
	{ msg_hiroba_osyaberi_05_44, msg_vip_osyaberi_05_44 },
	{ msg_hiroba_osyaberi_05_45, msg_vip_osyaberi_05_45 },
	{ msg_hiroba_osyaberi_05_46, msg_vip_osyaberi_05_46 },
	{ msg_hiroba_osyaberi_05_47, msg_vip_osyaberi_05_47 },
	{ msg_hiroba_osyaberi_05_48, msg_vip_osyaberi_05_48 },
	{ msg_hiroba_osyaberi_05_49, msg_vip_osyaberi_05_49 },
	{ msg_hiroba_osyaberi_05_50, msg_vip_osyaberi_05_50 },
	{ msg_hiroba_osyaberi_05_51, msg_vip_osyaberi_05_51 },
	{ msg_hiroba_osyaberi_05_52, msg_vip_osyaberi_05_52 },
	{ msg_hiroba_osyaberi_05_53, msg_vip_osyaberi_05_53 },
	{ msg_hiroba_osyaberi_05_54, msg_vip_osyaberi_05_54 },
	{ msg_hiroba_osyaberi_05_55, msg_vip_osyaberi_05_55 },
	{ msg_hiroba_osyaberi_05_56, msg_vip_osyaberi_05_56 },
	{ msg_hiroba_osyaberi_05_57, msg_vip_osyaberi_05_57 },
	{ msg_hiroba_osyaberi_05_58, msg_vip_osyaberi_05_58 },
	{ msg_hiroba_osyaberi_05_59, msg_vip_osyaberi_05_59 },
	{ msg_hiroba_osyaberi_05_60, msg_vip_osyaberi_05_60 },
	{ msg_hiroba_osyaberi_05_61, msg_vip_osyaberi_05_61 },
	{ msg_hiroba_osyaberi_05_62, msg_vip_osyaberi_05_62 },
	{ msg_hiroba_osyaberi_05_63, msg_vip_osyaberi_05_63 },
	{ msg_hiroba_osyaberi_05_64, msg_vip_osyaberi_05_64 },
	{ msg_hiroba_osyaberi_05_65, msg_vip_osyaberi_05_65 },
	{ msg_hiroba_osyaberi_05_66, msg_vip_osyaberi_05_66 },
	{ msg_hiroba_osyaberi_05_67, msg_vip_osyaberi_05_67 },
	{ msg_hiroba_osyaberi_05_68, msg_vip_osyaberi_05_68 },
	{ msg_hiroba_osyaberi_05_69, msg_vip_osyaberi_05_69 },
	{ msg_hiroba_osyaberi_05_70, msg_vip_osyaberi_05_70 },
	{ msg_hiroba_osyaberi_05_71, msg_vip_osyaberi_05_71 },
	{ msg_hiroba_osyaberi_05_72, msg_vip_osyaberi_05_72 },
	{ msg_hiroba_osyaberi_05_73, msg_vip_osyaberi_05_73 },
	{ msg_hiroba_osyaberi_05_74, msg_vip_osyaberi_05_74 },
	{ msg_hiroba_osyaberi_05_75, msg_vip_osyaberi_05_75 },
	{ msg_hiroba_osyaberi_05_76, msg_vip_osyaberi_05_76 },
	{ msg_hiroba_osyaberi_05_77, msg_vip_osyaberi_05_77 },
	{ msg_hiroba_osyaberi_05_78, msg_vip_osyaberi_05_78 },
	{ msg_hiroba_osyaberi_05_79, msg_vip_osyaberi_05_79 },
	{ msg_hiroba_osyaberi_05_80, msg_vip_osyaberi_05_80 },
	{ msg_hiroba_osyaberi_05_81, msg_vip_osyaberi_05_81 },
	{ msg_hiroba_osyaberi_05_82, msg_vip_osyaberi_05_82 },
	{ msg_hiroba_osyaberi_05_83, msg_vip_osyaberi_05_83 },
	{ msg_hiroba_osyaberi_05_84, msg_vip_osyaberi_05_84 },
	{ msg_hiroba_osyaberi_05_85, msg_vip_osyaberi_05_85 },
	{ msg_hiroba_osyaberi_05_86, msg_vip_osyaberi_05_86 },
	{ msg_hiroba_osyaberi_05_87, msg_vip_osyaberi_05_87 },
	{ msg_hiroba_osyaberi_05_88, msg_vip_osyaberi_05_88 },
	{ msg_hiroba_osyaberi_05_89, msg_vip_osyaberi_05_89 },
	{ msg_hiroba_osyaberi_05_90, msg_vip_osyaberi_05_90 },
	{ msg_hiroba_osyaberi_05_91, msg_vip_osyaberi_05_91 },
	{ msg_hiroba_osyaberi_05_92, msg_vip_osyaberi_05_92 },
	{ msg_hiroba_osyaberi_05_93, msg_vip_osyaberi_05_93 },
	{ msg_hiroba_osyaberi_05_94, msg_vip_osyaberi_05_94 },
	{ msg_hiroba_osyaberi_05_95, msg_vip_osyaberi_05_95 },
	{ msg_hiroba_osyaberi_05_96, msg_vip_osyaberi_05_96 },
	{ msg_hiroba_osyaberi_05_97, msg_vip_osyaberi_05_97 },
	{ msg_hiroba_osyaberi_05_98, msg_vip_osyaberi_05_98 },
	{ msg_hiroba_osyaberi_05_99, msg_vip_osyaberi_05_99 },
	{ msg_hiroba_osyaberi_05_100, msg_vip_osyaberi_05_100 },
	{ msg_hiroba_osyaberi_05_101, msg_vip_osyaberi_05_101 },
	{ msg_hiroba_osyaberi_05_102, msg_vip_osyaberi_05_102 },
	{ msg_hiroba_osyaberi_05_103, msg_vip_osyaberi_05_103 },
	{ msg_hiroba_osyaberi_05_104, msg_vip_osyaberi_05_104 },
	{ msg_hiroba_osyaberi_05_105, msg_vip_osyaberi_05_105 },
	{ msg_hiroba_osyaberi_05_106, msg_vip_osyaberi_05_106 },
	{ msg_hiroba_osyaberi_05_107, msg_vip_osyaberi_05_107 },
	{ msg_hiroba_osyaberi_05_108, msg_vip_osyaberi_05_108 },
	{ msg_hiroba_osyaberi_05_109, msg_vip_osyaberi_05_109 },
	{ msg_hiroba_osyaberi_05_110, msg_vip_osyaberi_05_110 },
	{ msg_hiroba_osyaberi_05_111, msg_vip_osyaberi_05_111 },
	{ msg_hiroba_osyaberi_05_112, msg_vip_osyaberi_05_112 },
	{ msg_hiroba_osyaberi_05_113, msg_vip_osyaberi_05_113 },
	{ msg_hiroba_osyaberi_05_114, msg_vip_osyaberi_05_114 },
	{ msg_hiroba_osyaberi_05_115, msg_vip_osyaberi_05_115 },
	{ msg_hiroba_osyaberi_05_116, msg_vip_osyaberi_05_116 },
	{ msg_hiroba_osyaberi_05_117, msg_vip_osyaberi_05_117 },
	{ msg_hiroba_osyaberi_05_118, msg_vip_osyaberi_05_118 },
	{ msg_hiroba_osyaberi_05_119, msg_vip_osyaberi_05_119 },
	{ msg_hiroba_osyaberi_05_120, msg_vip_osyaberi_05_120 },
	{ msg_hiroba_osyaberi_06_11, msg_vip_osyaberi_06_11 },
	{ msg_hiroba_osyaberi_06_12, msg_vip_osyaberi_06_12 },
	{ msg_hiroba_osyaberi_06_13, msg_vip_osyaberi_06_13 },
	{ msg_hiroba_osyaberi_06_14, msg_vip_osyaberi_06_14 },
	{ msg_hiroba_osyaberi_06_15, msg_vip_osyaberi_06_15 },
	{ msg_hiroba_osyaberi_06_16, msg_vip_osyaberi_06_16 },
	{ msg_hiroba_osyaberi_06_17, msg_vip_osyaberi_06_17 },
	{ msg_hiroba_osyaberi_06_18, msg_vip_osyaberi_06_18 },
	{ msg_hiroba_osyaberi_06_19, msg_vip_osyaberi_06_19 },
	{ msg_hiroba_osyaberi_06_20, msg_vip_osyaberi_06_20 },
	{ msg_hiroba_osyaberi_06_21, msg_vip_osyaberi_06_21 },
	{ msg_hiroba_osyaberi_06_22, msg_vip_osyaberi_06_22 },
	{ msg_hiroba_osyaberi_06_23, msg_vip_osyaberi_06_23 },
	{ msg_hiroba_osyaberi_06_24, msg_vip_osyaberi_06_24 },
	{ msg_hiroba_osyaberi_06_25, msg_vip_osyaberi_06_25 },
	{ msg_hiroba_osyaberi_06_26, msg_vip_osyaberi_06_26 },
	{ msg_hiroba_osyaberi_06_27, msg_vip_osyaberi_06_27 },
	{ msg_hiroba_osyaberi_06_28, msg_vip_osyaberi_06_28 },
	{ msg_hiroba_osyaberi_06_29, msg_vip_osyaberi_06_29 },
	{ msg_hiroba_osyaberi_06_30, msg_vip_osyaberi_06_30 },
};


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

static u32 WFLBY_EV_TALK_GetTimeZone( const WFLBY_USER_PROFILE* cp_pr );
static STRBUF* WFLBY_EV_TALK_GetStrBuf( WFLBY_ROOMWK* p_rmwk, u32 plidx, u32 msgidx );
static STRBUF* WFLBY_EV_TALK_GetStrBuf_Aisatu03_02( WFLBY_ROOMWK* p_rmwk, u32 aidx, u32 bidx );


static BOOL WFLBY_EV_TALK_WK_Init( WFLBY_EV_TALK_WK* p_evwk, WFLBY_SYSTEM* p_system, u32 idx );
static void WFLBY_EV_TALK_WK_Exit( WFLBY_EV_TALK_WK* p_evwk );
static void WFLBY_EV_TALK_WK_Wait( WFLBY_EV_TALK_WK* p_evwk, u8 wait, u8 ret_seq, WFLBY_EVENTWK* p_wk, u8 wait_seq );
static void WFLBY_EV_TALK_WK_WaitMain( WFLBY_EV_TALK_WK* p_evwk, WFLBY_EVENTWK* p_wk );
static void WFLBY_EV_TALK_WK_MsgEndWaitMain( WFLBY_EV_TALK_WK* p_evwk, WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk );
static void WFLBY_EV_TALK_WK_AisatuDraw( WFLBY_EV_TALK_WK* p_evwk, WFLBY_SYSTEM* p_system, WFLBY_ROOMWK* p_rmwk, u32 timezone, const WFLBY_USER_PROFILE* cp_mypr, const WFLBY_USER_PROFILE* cp_youpr );
static void WFLBY_EV_TALK_WK_Aisatu_SetWordSet( WFLBY_ROOMWK* p_rmwk, u32 buffno, u32 timezone, const WFLBY_USER_PROFILE* cp_pr );
static BOOL WFLBY_EV_TALK_WK_AisatuFollowDraw( WFLBY_EV_TALK_WK* p_evwk, WFLBY_SYSTEM* p_system, WFLBY_ROOMWK* p_rmwk );
static void WFLBY_EV_TALK_WK_WordsetSetItem( WFLBY_EV_TALK_WK* p_evwk, WFLBY_SYSTEM* p_system, WFLBY_ROOMWK* p_rmwk, u32 bufid, BOOL my );
static void WFLBY_EV_TALK_WK_PrintTimeWaitMsg( WFLBY_EV_TALK_WK* p_evwk, WFLBY_ROOMWK* p_rmwk );
static void WFLBY_EV_TALK_WK_StartSelectWait( WFLBY_EV_TALK_WK* p_evwk );
static void WFLBY_EV_TALK_WK_StopSelectWait( WFLBY_EV_TALK_WK* p_evwk );
static void WFLBY_EV_TALK_WK_CheckAndPrintSelectWait( WFLBY_EV_TALK_WK* p_evwk, WFLBY_ROOMWK* p_rmwk );
static void WFLBY_EV_TALK_WK_MainSelectWait( WFLBY_EV_TALK_WK* p_evwk );
static void WFLBY_EV_TALK_WK_SetRirekiData( WFLBY_EV_TALK_WK* p_evwk, const WFLBY_USER_PROFILE* cp_youpr );


static void WFLBY_EV_TALK_WK_InitBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk, u32 num, u32 msg_start, u32 last_sel_b );
static void WFLBY_EV_TALK_WK_InitBmpListRandom( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk, u32 allnum, u32 sel_num, u32 msg_start, u32 last_sel_b );
static void WFLBY_EV_TOPIC_WK_InitBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk );
static void WFLBY_EV_TOPIC_WK_InitEventBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk );
static void WFLBY_EV_TOPIC_WK_InitMinigameBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk );
static void WFLBY_EV_TALK_WK_ExitBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk );


static void WFLBY_EV_TALK_RWSEND_Init( WFLBY_EV_TALK_RWSEND* p_wk );
static void WFLBY_EV_TALK_RWSEND_Start( WFLBY_EV_TALK_RWSEND* p_wk, const WFLBY_SYSTEM* cp_system, u16 send_data, u16 wait_seq );
static void WFLBY_EV_TALK_RWSEND_Main( WFLBY_EV_TALK_RWSEND* p_wk, WFLBY_SYSTEM* p_system );
static BOOL WFLBY_EV_TALK_RWSEND_CheckEnd( const WFLBY_EV_TALK_RWSEND* cp_wk );
static u16 WFLBY_EV_TALK_RWSEND_GetTalkSeq( const WFLBY_EV_TALK_RWSEND* cp_wk );


//----------------------------------------------------------------------------
/**
 *	@brief	�b�������J�n
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_EV_TALK_StartA( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_TALK_WK* p_evwk;
	WFLBY_SYSTEM* p_system;
	WFLBY_EVENT*  p_event;
	u32 talk_seq;
	u32 recv_data;
	STRBUF* p_str;
	const WFLBY_USER_PROFILE* cp_mypr;
	const WFLBY_USER_PROFILE* cp_youpr;

	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );		// �V�X�e���f�[�^�擾
	p_event		= WFLBY_ROOM_GetEvent( p_rmwk );			// �C�x���g���[�N�擾
	p_evwk		= WFLBY_EVENTWK_GetWk( p_wk );				// �C�x���g�f�[�^�擾
	talk_seq	= WFLBY_SYSTEM_GetTalkSeq( p_system );		// ��b�V�[�P���X�擾
	recv_data	= WFLBY_SYSTEM_GetTalkRecv( p_system );		// ��M�f�[�^

	// ����[������������EX�I��
	if( WFLBY_SYSTEM_CheckTalkError( p_system ) ){
		if( p_evwk ){
			if( p_evwk->err_chk ){
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ERR_ENDMSG );
				WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
				p_evwk->err_chk = FALSE;
			}
		}
	}

	if( p_evwk ){
		cp_mypr		= WFLBY_SYSTEM_GetMyProfileLocal( p_system );
#if PL_T0866_080715_FIX
		if( p_evwk->p_npc ){
			cp_youpr	= WFLBY_SYSTEM_GetUserProfile( p_system, p_evwk->talk_idx );
		}else{
			cp_youpr	= NULL;
		}
#else
		cp_youpr	= WFLBY_SYSTEM_GetUserProfile( p_system, p_evwk->talk_idx );
#endif
	}

	// �����������㎩���Ƒ���̃��[�U�v���t�B�[�����擾����
	if( WFLBY_EVENTWK_GetSeq( p_wk ) > WFLBY_EV_TALK_A_SEQ_INIT ){
		if( p_evwk->err_chk ){

			// ���肪���Ȃ��Ȃ����狭���I��
			if( cp_youpr == NULL ){	
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ERR_ENDMSG );
				WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
				p_evwk->err_chk = FALSE;
			}
		}
	}
	

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ������
	case WFLBY_EV_TALK_A_SEQ_INIT:

#ifdef WFLBY_EV_TALK_DEBUG_SEL_MSG
		// ���ԂɎ��₪�o�Ă���f�o�b�NON
		if( sys.cont & PAD_BUTTON_R ){
			s_WFLBY_EV_TALK_DEBUG_SEL_MSG_ON = TRUE;
		}else{
			s_WFLBY_EV_TALK_DEBUG_SEL_MSG_ON = FALSE;
		}
#endif


		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_TALK_WK) );

		// �����f�[�^������
		p_evwk->rireki_flag		= FALSE;
		p_evwk->rireki_actno	= 0;
		p_evwk->rireki_userid	= 0;

		// �������b�Z�[�W���M�V�X�e����������
		WFLBY_EV_TALK_RWSEND_Init( &p_evwk->rw_send );
		
		//  ��b�^�[�Q�b�g���擾����
		{
			WFLBY_3DPERSON* p_npc;
			WFLBY_3DOBJCONT* p_objcont;
			WFLBY_3DPERSON* p_player;
			u32 idx;
			u32 way;
			BOOL result;

			p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
			p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			p_npc		= WFLBY_3DOBJCONT_GetFrontPerson( p_objcont, p_player );
#if	PL_T0866_080715_FIX
			if( p_npc == NULL ){
				// ��b�G���[�I����
				p_evwk->err_chk = FALSE;// (�ꉞ�s��)
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_END );
				break;
			}
#endif
			idx			= WFLBY_3DOBJCONT_GetWkObjData( p_npc, WF2DMAP_OBJPM_PLID );
			way			= WFLBY_3DOBJCONT_GetWkObjData( p_player,  WF2DMAP_OBJPM_WAY );
			way			= WF2DMPA_OBJToolRetWayGet( way );

			// 080630	��b��Ԃ̃��[�N���N���A
			WFLBY_SYSTEM_EndTalk( p_system );

			// ���[�N������
			// �߂�l�Řb�����肪�ݐВ������ׂ�
			result = WFLBY_EV_TALK_WK_Init( p_evwk, p_system, idx );
		
			// NPC���Ƃ߂違������������
			p_evwk->p_npc	= p_npc;
			WFLBY_3DOBJCONT_DRAW_SetUpdata( p_npc, FALSE );
			WFLBY_3DOBJCONT_DRAW_SetWay( p_npc, way );
			// �C�x���g���Ƃ߂�
			WFLBY_EVENT_SetPrivateEventMove( p_event, idx, FALSE );

			// �b��������
			Snd_SePlay( WFLBY_SND_TALK );

			// ��b�V�[�P���X��
			WFLBY_SYSTEM_SetMyStatus( p_system, WFLBY_STATUS_TALK );

			if( result == TRUE ){
				// NPC�̃v���t�B�[����\������
				WFLBY_ROOM_UNDERWIN_TrCardOn( p_rmwk, idx, FALSE );

				// ��b���N�G�X�g��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_REQ );

			}else{

				// ��b���N�G�X�g��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ANSNGMSG );
				break;
			}

		}
		break;

	// ���N�G�X�g
	case WFLBY_EV_TALK_A_SEQ_REQ:
		{
			BOOL result;

			result = WFLBY_SYSTEM_SendTalkReq( p_system, p_evwk->talk_idx );
			if( result == TRUE ){
				// �����҂���
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ANSWAIT );
			}else{
				// �������s
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ANSNGMSG );
			}
		}
		break;
		
	// �񓚑҂�
	case WFLBY_EV_TALK_A_SEQ_ANSWAIT:
		
		// �Ԏ����A���Ă������H
		switch( talk_seq ){
		// �Ԏ��̌���OKO
		case WFLBY_TALK_SEQ_A_SEL:	// �b��������	�I��

			// ���̐l�Ƙb�����ƁA�}�X�N�ݒ�
			WFLBY_SYSTEM_SetTalkMsk( p_system, p_evwk->talk_idx );

			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_AISATUMY );
			break;

		// �Ԏ��̌���NG
		case WFLBY_TALK_SEQ_NONE:	// �����Ȃ�	��b���I�������Ă�������
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ANSNGMSG );
			break;

		// �҂����
		case WFLBY_TALK_SEQ_B_ANS:	// ������ꑤ	����
			break;

		// ���̑��ɂ͂Ȃ�Ȃ��͂�
		// EXEND������
		case WFLBY_TALK_SEQ_B_SEL:	// ������ꑤ	�I��
		case WFLBY_TALK_SEQ_EXEND:	// �ً}�I��	��b���I�������Ă�������
		default:
			// �ؒf�I��
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ERR_ENDMSG );
			break;
		}
		break;

	// ��������	�����̎���
	case WFLBY_EV_TALK_A_SEQ_AISATUMY:
		// �`���m�[�g
		WFLBY_SYSTEM_FNOTE_SetTalk( p_system, p_evwk->talk_idx );

		// ���R�[�h
		WFLBY_SYSTEM_RECORD_AddTalk( p_system );

		// �V�񂾏��
		WFLBY_SYSTEM_PLAYED_SetPlay( p_system, WFLBY_PLAYED_TALK );
		
		WFLBY_EV_TALK_WK_AisatuDraw( p_evwk, p_system, p_rmwk, p_evwk->my_timezone, cp_mypr, cp_youpr );
		// �E�G�C�g�̂��ƈ��A�t�H���[��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_AISATUYOU, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
		break;

	// ��������	����̎���
	case WFLBY_EV_TALK_A_SEQ_AISATUYOU:
		WFLBY_EV_TALK_WK_AisatuDraw( p_evwk, p_system, p_rmwk, p_evwk->you_timezone, cp_youpr, cp_mypr );

		// �E�G�C�g�̌㓯��������
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SYNC_RECV, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );

		// �����J�n	�����̃f�[�^���M
		WFLBY_SYSTEM_SendTalkData( p_system, WFLBY_EV_TALK_TYPE_AISATSUSYNC );
		break;

	// ��b��ԓ���
	case WFLBY_EV_TALK_A_SEQ_SYNC_RECV:		
		// �Ԏ����A���Ă������H
		switch( talk_seq ){
		case WFLBY_TALK_SEQ_A_SEL:	// �b��������	�I��
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );


			// ���̐l�Ƃ̉�b�����߂Ă��ŕ���H
			if( WFLBY_SYSTEM_GetTalkFirst( p_system ) ){
				// ���߂Ă̂Ƃ�
				// ���A�t�H���[��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_AISATUFOLLOW );
			}else{
				// �����͂Ȃ������Ƃ�����
				// ���A�I���Q��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_AISATUEND2 );
			}
			break;

		// �҂����
		case WFLBY_TALK_SEQ_B_SEL:	// ������ꑤ	�I��
			if( WFLBY_ROOM_TALKWIN_CheckTimeWait( p_rmwk ) == FALSE ){
				WFLBY_ROOM_TALKWIN_StartTimeWait( p_rmwk );
			}

			// �҂����Ԃ�WFLBY_TALK_RECV_HALF_WAIT�ɂȂ����烁�b�Z�[�W��\��
			WFLBY_EV_TALK_WK_PrintTimeWaitMsg( p_evwk, p_rmwk );
			break;

		// ���̑��ɂ͂Ȃ�Ȃ��͂�
		// EXEND������
		case WFLBY_TALK_SEQ_NONE:	// �����Ȃ�	��b���I�������Ă�������
		case WFLBY_TALK_SEQ_EXEND:	// �ً}�I��	��b���I�������Ă�������
		case WFLBY_TALK_SEQ_B_ANS:	// ������ꑤ	����
		default:
			// �ؒf�I��
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ERR_ENDMSG );
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;
		}
		break;

	// �������t�H���[
	case WFLBY_EV_TALK_A_SEQ_AISATUFOLLOW:
		{
			BOOL result;
			// �t�H���[���K�v�H
			result = WFLBY_EV_TALK_WK_AisatuFollowDraw( p_evwk, p_system, p_rmwk );
			if( result == TRUE ){
				// �E�G�C�g�̂��ƈ��A�Ō��
				WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
						WFLBY_EV_TALK_A_SEQ_AISATUEND, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
			}else{
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_AISATUEND );
			}
		}
		break;
		
	// �������̍Ō�̃��b�Z�[�W
	case WFLBY_EV_TALK_A_SEQ_AISATUEND:
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_aisatu_03_01 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_aisatu_03_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�����̑I����
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEL_GT_FIR, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
		break;

	// ���A�̍Ō�̃��b�Z�[�W	�����P��͂Ȃ��Ă���Ƃ�
	case WFLBY_EV_TALK_A_SEQ_AISATUEND2:

		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->my_idx, 1 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_aisatu_03_02 );
		p_str = WFLBY_EV_TALK_GetStrBuf_Aisatu03_02( p_rmwk, p_evwk->talk_idx, p_evwk->my_idx );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�����̑I����
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEL_GT_FIR, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
		break;

	// �K�W�F�b�g�A��b�Z���N�g	�P���
	case WFLBY_EV_TALK_A_SEQ_SEL_GT_FIR:
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_00_01 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_00_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEL_GT00, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
		break;
		
	// �K�W�F�b�g�A��b�Z���N�g	�Q��ڈȍ~
	case WFLBY_EV_TALK_A_SEQ_SEL_GT_SEC:		
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_00_09 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_00_09 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEL_GT00, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
		break;

	// �K�W�F�b�g�A��b�Z���N�g ���X�g�\��
	case WFLBY_EV_TALK_A_SEQ_SEL_GT00:
		WFLBY_EV_TALK_WK_InitBmpList( &p_evwk->bmplist, p_rmwk, WFLBY_EV_TALK_LIST_SEL_GT_NUM, msg_hiroba_menu_00_01, WFLBY_EV_TALK_LIST_END_NONE );
		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist.bmplist, 0, 0 );
		WFLBY_EV_TALK_WK_StartSelectWait( p_evwk );	// �I��҂��J�n
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEL_GT00WAIT );
		break;

	// �K�W�F�b�g�A��b�Z���N�g�@�I��҂�
	case WFLBY_EV_TALK_A_SEQ_SEL_GT00WAIT:
		{
			u32 result;
			BOOL del;
			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );
			switch( result ){
			case WFLBY_EV_TALK_LIST_SEL_GT_GAGET:
				//  �K�W�F�b�g���b�Z�[�W��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEND_G );
				del = TRUE;
				break;
				
			case WFLBY_EV_TALK_LIST_SEL_GT_TALK:
				//  ��b�I����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEND_TALK );

				// ��b�g�s�b�N�쐬
				{
					u32 npc_plno;
					npc_plno = WFLBY_3DOBJCONT_GetWkObjData( p_evwk->p_npc, WF2DMAP_OBJPM_PLID );
					WFLBY_SYSTEM_TOPIC_SendConnect( p_system, plno, npc_plno );
				}
				
				del = TRUE;
				break;
				
			case WFLBY_EV_TALK_LIST_SEL_GT_END:

				// �I���𑗐M
				WFLBY_SYSTEM_SendTalkEnd( p_system );
				
				//  �I����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ENDMSG );
				del = TRUE;
				break;

			default:
				WFLBY_EV_TALK_WK_CheckAndPrintSelectWait( p_evwk, p_rmwk );
				del = FALSE;
				break;
			}

			// ���X�g�j��
			if( del == TRUE ){
				OS_TPrintf( "SEL_GT00WAIT result = %d\n", result );
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
				WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );
				WFLBY_EV_TALK_WK_StopSelectWait( p_evwk );
			}
		}
		break;

	// �K�W�F�b�g���炤�����J�n
	case WFLBY_EV_TALK_A_SEQ_SEND_G:

		// �K�W�F�b�g���ꏏ���`�F�b�N
		if( WFLBY_SYSTEM_GetProfileItem( cp_mypr ) == WFLBY_SYSTEM_GetProfileItem( cp_youpr ) ){

			// �K�W�F�b�g�ꏏ	
			p_evwk->gadget_eq = TRUE;
			
			// ������쐬
			WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
			WFLBY_EV_TALK_WK_WordsetSetItem( p_evwk, p_system, p_rmwk, 1, FALSE );
		//	p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_item_00_01 );
			p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_item_00_01 );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

			// B�̕ԓ��҂��ɖ߂�
			WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
					WFLBY_EV_TALK_A_SEQ_SEND_G_RECV_B, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );

			// �K�W�F�b�g���ꏏ���Ƒ��M
			WFLBY_SYSTEM_SendTalkData( p_system, WFLBY_EV_TALK_TYPE_GADGET_EQ );
		}else{

			// �K�W�F�b�g�Ⴄ
			p_evwk->gadget_eq = FALSE;

			// �Ⴄ
			// ������쐬
			WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
			WFLBY_EV_TALK_WK_WordsetSetItem( p_evwk, p_system, p_rmwk, 1, TRUE );
			WFLBY_EV_TALK_WK_WordsetSetItem( p_evwk, p_system, p_rmwk, 2, FALSE );
//			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_item_00_02 );
			p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_item_00_02 );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

			// �E�G�C�g�̌�A���炤���I����
			WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
					WFLBY_EV_TALK_A_SEQ_SEND_G_GETCHK, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );

		}
		break;
		
	// �K�W�F�b�g���炤���I��
	case WFLBY_EV_TALK_A_SEQ_SEND_G_GETCHK:
		WFLBY_EV_TALK_WK_InitBmpList( &p_evwk->bmplist, p_rmwk, WFLBY_EV_TALK_LIST_SEL_G_NUM, msg_hiroba_menu_00_04, WFLBY_EV_TALK_LIST_END_YAMERU );
		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist.bmplist, 0, 0 );

		WFLBY_EV_TALK_WK_StartSelectWait( p_evwk );
		
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEND_G_GETCHKWAIT );
		break;

	// �K�W�F�b�g���炤���I���܂�
	case WFLBY_EV_TALK_A_SEQ_SEND_G_GETCHKWAIT:
		{
			u32 result;
			BOOL del;
			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );
			switch( result ){
			case WFLBY_EV_TALK_LIST_SEL_G_GET:

				// �K�W�F�b�g�����炤�̂ł��炤���Ƃ𑗐M
				WFLBY_SYSTEM_SendTalkData( p_system, WFLBY_EV_TALK_TYPE_GADGET );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEND_G_GET );

				// �`���m�[�g
				WFLBY_SYSTEM_FNOTE_SetGetGadget( p_system, p_evwk->talk_idx );


				// �K�W�F�b�g�����g�s�b�N�쐬
				{
					u32 npc_plno;
					npc_plno = WFLBY_3DOBJCONT_GetWkObjData( p_evwk->p_npc, WF2DMAP_OBJPM_PLID );
					WFLBY_SYSTEM_TOPIC_SendItem( p_system, plno, npc_plno, WFLBY_SYSTEM_GetProfileItem( cp_youpr ) );
				}
				del = TRUE;
				break;
				
			case WFLBY_EV_TALK_LIST_SEL_G_END:
				//  ��b�I����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEL_GT_SEC );
				del = TRUE;
				break;
				
			default:
				WFLBY_EV_TALK_WK_CheckAndPrintSelectWait( p_evwk, p_rmwk );
				del = FALSE;
				break;
			}

			// ���X�g�j��
			if( del == TRUE ){
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
				WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );

				WFLBY_EV_TALK_WK_StopSelectWait( p_evwk );
			}
		}
		break;

	// �K�W�F�b�g���炤
	case WFLBY_EV_TALK_A_SEQ_SEND_G_GET:
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		WFLBY_EV_TALK_WK_WordsetSetItem( p_evwk, p_system, p_rmwk, 1, FALSE );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_item_00_04 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_item_00_04 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A���ڑI����
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEND_G_RECV_B_SE, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );

		// �K�W�F�b�g��ݒ�
		{
			u32 item;
			item = WFLBY_SYSTEM_GetProfileItem( cp_youpr );
			WFLBY_SYSTEM_SetMyItem( p_system, item );
		}
		break;

	case WFLBY_EV_TALK_A_SEQ_SEND_G_RECV_B_SE:
		// �K�W�F�b�g���炤B�̎�M�m�F
		// SE�Đ�
		Snd_SePlay( WFLBY_SND_TOUCH_TOY_CHG );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEND_G_RECV_B );
		break;

	// �K�W�F�b�g���炤B�̎�M�m�F
	case WFLBY_EV_TALK_A_SEQ_SEND_G_RECV_B:	

		// �Ԏ����A���Ă������H
		switch( talk_seq ){
		case WFLBY_TALK_SEQ_A_SEL:	// �b��������	�I��

			// �Ō�ɂ������ƂɃK�W�F�b�g������ݒ�
			WFLBY_EV_TALK_WK_SetRirekiData( p_evwk, cp_youpr );
			
			if( p_evwk->gadget_eq == TRUE ){
				// ��b�I����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ENDMSG );
			}else{
				// �č��ڑI����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEL_GT_SEC );
			}

			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;

		// �҂����
		case WFLBY_TALK_SEQ_B_SEL:	// ������ꑤ	�I��
			if( WFLBY_ROOM_TALKWIN_CheckTimeWait( p_rmwk ) == FALSE ){
				WFLBY_ROOM_TALKWIN_StartTimeWait( p_rmwk );
			}

			// �҂����Ԃ�WFLBY_TALK_RECV_HALF_WAIT�ɂȂ����烁�b�Z�[�W��\��
			WFLBY_EV_TALK_WK_PrintTimeWaitMsg( p_evwk, p_rmwk );
			break;

		// ���̑��ɂ͂Ȃ�Ȃ��͂�
		// EXEND������
		case WFLBY_TALK_SEQ_NONE:	// �����Ȃ�	��b���I�������Ă�������
		case WFLBY_TALK_SEQ_EXEND:	// �ً}�I��	��b���I�������Ă�������
		case WFLBY_TALK_SEQ_B_ANS:	// ������ꑤ	����
		default:
			// �ؒf�I��
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ERR_ENDMSG );
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;
		}
		break;

	// ��b���邱�Ƃ𑗐M
	case WFLBY_EV_TALK_A_SEQ_SEND_TALK:
		WFLBY_SYSTEM_SendTalkData( p_system, WFLBY_EV_TALK_TYPE_TALK_START );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEL_T_K );
		break;
		
	// ��b���ڑI��
	case WFLBY_EV_TALK_A_SEQ_SEL_T_K:
		WFLBY_EV_TALK_WK_InitBmpListRandom( &p_evwk->bmplist, p_rmwk, WFLBY_EV_TALK_LIST_SEL_TK_NUM, WFLBY_EV_TALK_TK_SEL_NUM, msg_hiroba_menu_01_01, WFLBY_EV_TALK_LIST_END_NONE );
		WFLBY_ROOM_LISTWIN_Start_Ex( p_rmwk, &p_evwk->bmplist.bmplist, 0, 0, WFLBY_EV_TALK_TK_LISTWIN_X, WFLBY_EV_TALK_TK_LISTWIN_Y, WFLBY_EV_TALK_TK_LISTWIN_SIZX );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEL_T_K_WAIT );


		// ���ڑI��҂��[�������̂��̂�I���@�܂łP�̂�����Ƃ��đ҂�
		WFLBY_EV_TALK_WK_StartSelectWait( p_evwk );
		break;

	// ��b���ڑI��҂�
	case WFLBY_EV_TALK_A_SEQ_SEL_T_K_WAIT:	
		{
			u32 result;

#ifdef WFLBY_EV_TALK_DEBUG_SEL_MSG
			// ���ԂɎ��₪�o�Ă���f�o�b�N
			if( s_WFLBY_EV_TALK_DEBUG_SEL_MSG_ON == TRUE ){
				if( (sys.trg & PAD_KEY_RIGHT) || (sys.trg & PAD_KEY_LEFT) ){

					if( sys.trg & PAD_KEY_RIGHT ){
						s_WFLBY_EV_TALK_DEBUG_SEL_MSG_PAGE = (s_WFLBY_EV_TALK_DEBUG_SEL_MSG_PAGE+1) % 8;
					}else{
						s_WFLBY_EV_TALK_DEBUG_SEL_MSG_PAGE --;
						if( s_WFLBY_EV_TALK_DEBUG_SEL_MSG_PAGE < 0 ){
							s_WFLBY_EV_TALK_DEBUG_SEL_MSG_PAGE += 8;
						}
					}

					WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
					WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );

					// ���X�g�ĕ`��
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEL_T_K );
				}
			}
#endif

			
			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );
			switch( result ){
/*
			case WFLBY_EV_TALK_LIST_SEL_TK_END:	// ��߂�
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
				WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );
				WFLBY_EV_TALK_WK_StopSelectWait( p_evwk );
				// �I����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEL_GT_SEC );
				break;
//*/

			case BMPLIST_NULL:
			case BMPLIST_CANCEL:
				WFLBY_EV_TALK_WK_CheckAndPrintSelectWait( p_evwk, p_rmwk );
				break;

			// �Ȃ񂩑I��
			default:
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
				WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );
//				WFLBY_EV_TALK_WK_StopSelectWait( p_evwk );	// ���̉�b���e��I�Ԃ��̂������đ҂�

				// ��b���ڐ��ݒ�
				p_evwk->sel_k = result;

				// �ڍ׍��ڃ`�F�b�N��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEL_T_M );
				break;

			}
		}
		break;
		
	// �����̂��ƑI��
	case WFLBY_EV_TALK_A_SEQ_SEL_T_M:
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_02_01+p_evwk->sel_k );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_06_01+p_evwk->sel_k );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEL_T_M_SEL, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
		break;

	// �����̂��ƑI��
	case WFLBY_EV_TALK_A_SEQ_SEL_T_M_SEL:	
		WFLBY_EV_TALK_WK_InitBmpList( &p_evwk->bmplist, p_rmwk, WFLBY_EV_TALK_LIST_SEL_TK_S_MAX, 
				msg_hiroba_osyaberi_03_01 + (p_evwk->sel_k*WFLBY_EV_TALK_LIST_SEL_TK_S_MAX), WFLBY_EV_TALK_LIST_END_NONE );
		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist.bmplist, 0, 0 );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEL_T_M_WAIT );
		break;

	// �����̂��ƑI��
	case WFLBY_EV_TALK_A_SEQ_SEL_T_M_WAIT:
		{
			u32 result;
			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );
			switch( result ){
			case WFLBY_EV_TALK_LIST_SEL_TK_S_00:	
			case WFLBY_EV_TALK_LIST_SEL_TK_S_01:	
			case WFLBY_EV_TALK_LIST_SEL_TK_S_02:	
			case WFLBY_EV_TALK_LIST_SEL_TK_S_04:	
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
				WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );
				WFLBY_EV_TALK_WK_StopSelectWait( p_evwk );

				// ��b���ڐݒ�
				p_evwk->sel_k_data = (p_evwk->sel_k*WFLBY_EV_TALK_LIST_SEL_TK_S_MAX) + result;

				// �ڍ׍��ڃ`�F�b�N��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SEND_T );
				break;

			default:
				WFLBY_EV_TALK_WK_CheckAndPrintSelectWait( p_evwk, p_rmwk );
				break;
			}
		}
		break;
	// ��b�I����񑗐M
	case WFLBY_EV_TALK_A_SEQ_SEND_T:
		// �������M�J�n
		WFLBY_EV_TALK_RWSEND_Start( &p_evwk->rw_send, p_system, WFLBY_EV_TALK_TYPE_TALK_COL_00 + p_evwk->sel_k_data, WFLBY_TALK_SEQ_A_SEL );

		// ���A�N�V�������b�Z�[�W�\��
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_04_01+p_evwk->sel_k_data );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_05_01+p_evwk->sel_k_data );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEND_T00, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );

		break;

	// ��b���A�N�V����
	case WFLBY_EV_TALK_A_SEQ_SEND_T00:
		// ���A�N�V�������b�Z�[�W2�\��
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_06_01+p_evwk->sel_k );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_02_01+p_evwk->sel_k );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEND_TWAIT, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
		break;

	// ��b�I����񑗐M
	case  WFLBY_EV_TALK_A_SEQ_SEND_TWAIT:
		// �������M���I��������`�F�b�N
		{
			BOOL result;
			u16 recv_talk_seq;
			result = WFLBY_EV_TALK_RWSEND_CheckEnd( &p_evwk->rw_send );
			if( result == TRUE ){
				recv_talk_seq = WFLBY_EV_TALK_RWSEND_GetTalkSeq( &p_evwk->rw_send );

				// ����
				if( recv_talk_seq == WFLBY_TALK_SEQ_A_SEL ){

					// ��M�҂���
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_RECV_BT );
				}else{

					// �ؒf�I��
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ERR_ENDMSG );
					WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
				}
			}else{

				// �I����ĂȂ��̂őҋ@

				// �^�C���E�G�C�g�}�[�N�o��
				if( WFLBY_ROOM_TALKWIN_CheckTimeWait( p_rmwk ) == FALSE ){
					WFLBY_ROOM_TALKWIN_StartTimeWait( p_rmwk );
				}

			}
		}
		break;

	// ��b�I������M
	case WFLBY_EV_TALK_A_SEQ_RECV_BT:

		
		// ��b�f�[�^��M�҂�
		switch( talk_seq ){
		// ��b�I������
		case WFLBY_TALK_SEQ_A_SEL:	// �b��������	�I��

			// ����ۑ�
			WFLBY_EV_TALK_WK_SetRirekiData( p_evwk, cp_youpr );
			
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_MSG_BT );
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;


		// �҂����
		case WFLBY_TALK_SEQ_B_SEL:	// ������ꑤ	�I��
			if( WFLBY_ROOM_TALKWIN_CheckTimeWait( p_rmwk ) == FALSE ){
				WFLBY_ROOM_TALKWIN_StartTimeWait( p_rmwk );
			}

			// �҂����Ԃ�WFLBY_TALK_RECV_HALF_WAIT�ɂȂ����烁�b�Z�[�W��\��
			WFLBY_EV_TALK_WK_PrintTimeWaitMsg( p_evwk, p_rmwk );
			break;

		// ���̑��ɂ͂Ȃ�Ȃ��͂�
		// EXEND������
		case WFLBY_TALK_SEQ_NONE:	// ��b���I�������Ă�������
		case WFLBY_TALK_SEQ_B_ANS:	// ������ꑤ	����
		case WFLBY_TALK_SEQ_EXEND:	// �ً}�I��	��b���I�������Ă�������
		default:
			// �ؒf�I��
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_ERR_ENDMSG );
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;
		}
		break;

	// ������b��񃁃b�Z�[�W�\��
	case WFLBY_EV_TALK_A_SEQ_MSG_BT:
		{
			u16 recv_k_data = (recv_data-WFLBY_EV_TALK_TYPE_TALK_COL_00);

			// �킩��Ȃ��Ƃ����Ă�����u�킩��Ȃ����b�Z�[�W�I���v
			// �킩��Ȃ��ُ�̐������A���Ă��Ă��킩��Ȃ��ɂ���
			if( (recv_data >= WFLBY_EV_TALK_TYPE_TALK_NONE) || 
				(recv_data < WFLBY_EV_TALK_TYPE_TALK_COL_00 ) ){
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_A_SEQ_SELNGMSG );
				break;
			}
			
			// ���A�N�V�������b�Z�[�W�\��
			WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_05_01+recv_k_data );
			p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_04_01+recv_k_data );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

			// ���ڂ��ꏏ���`�F�b�N
			if( recv_k_data == p_evwk->sel_k_data ){
				WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
						WFLBY_EV_TALK_A_SEQ_MSG_ABT_EQ, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
			}else{
				WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
						WFLBY_EV_TALK_A_SEQ_MSG_BT_END, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
			}
		}
		break;
		
	// �I�����ڂ������Ƃ�
	case WFLBY_EV_TALK_A_SEQ_MSG_ABT_EQ:	
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_00_06 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_00_06 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEL_GT_SEC, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );

		break;

	// �������b�Z�[�W�Ō�
	case WFLBY_EV_TALK_A_SEQ_MSG_BT_END:		
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		// �Ȃ�قǂȂ�
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_00_05 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_00_05 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_SEL_GT_SEC, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );

		break;

	// �G���[�I��
	case WFLBY_EV_TALK_A_SEQ_ERR_ENDMSG:	

		// ���X�g���o�Ă��邩������Ȃ��̂ŏ���
		WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
		WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );
		
		// �I�𒆃��b�Z�[�W�\��
		// ������쐬
//		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_aisatu_04_04 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_aisatu_04_04 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �����G���[�`�F�b�N�K�v�Ȃ�
		p_evwk->err_chk = FALSE;

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_END, p_wk, WFLBY_EV_TALK_A_SEQ_WAIT );
//				WFLBY_EV_TALK_A_SEQ_END, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );

		// �����I���������Ƃ�������
		WFLBY_SYSTEM_SendTalkExEnd( p_system );
		break;
	// �ʏ�I��
	case WFLBY_EV_TALK_A_SEQ_ENDMSG:		
		// �I�𒆃��b�Z�[�W�\��
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_aisatu_04_02 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_aisatu_04_02 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �����G���[�`�F�b�N�K�v�Ȃ�
		p_evwk->err_chk = FALSE;

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_END, p_wk, WFLBY_EV_TALK_A_SEQ_WAIT );
//				WFLBY_EV_TALK_A_SEQ_END, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );

		break;
		
	// �����Ȃ��I��
	case WFLBY_EV_TALK_A_SEQ_ANSNGMSG:
		// �I�𒆃��b�Z�[�W�\��
		// ������쐬
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_aisatu_04_01 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_aisatu_04_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �����G���[�`�F�b�N�K�v�Ȃ�
		p_evwk->err_chk = FALSE;

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_END, p_wk, WFLBY_EV_TALK_A_SEQ_WAIT );
//				WFLBY_EV_TALK_A_SEQ_END, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
		break;

	// B�I��NG���b�Z�[�W
	case WFLBY_EV_TALK_A_SEQ_SELNGMSG:		// B�I���ł��Ȃ��I��
		// �I�𒆃��b�Z�[�W�\��
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_04_41 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_04_41 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �����G���[�`�F�b�N�K�v�Ȃ�
		p_evwk->err_chk = FALSE;

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_A_SEQ_END, p_wk, WFLBY_EV_TALK_A_SEQ_WAIT );
//				WFLBY_EV_TALK_A_SEQ_END, p_wk, WFLBY_EV_TALK_A_SEQ_MSGWAIT );
		break;

	// �ҋ@����
	case WFLBY_EV_TALK_A_SEQ_WAIT:
		WFLBY_EV_TALK_WK_WaitMain( p_evwk, p_wk );
		break;

	// ���b�Z�[�W�̏I���E�G�C�g
	case WFLBY_EV_TALK_A_SEQ_MSGWAIT:
		WFLBY_EV_TALK_WK_MsgEndWaitMain( p_evwk, p_wk, p_rmwk );
		break;

	// �I��
	case WFLBY_EV_TALK_A_SEQ_END:		

		// �Ō�ɂ������Ƃɉ�b��ݒ�
		if( p_evwk->rireki_flag == TRUE ){
			WFLBY_SYSTEM_SetLastAction( p_system, p_evwk->rireki_actno, p_evwk->rireki_userid );
		}

		// ��b�I���ɂ���
		WFLBY_SYSTEM_EndTalk( p_system );

		// ���b�Z�[�W�I��
		WFLBY_ROOM_TALKWIN_Off( p_rmwk );
		
#if	PL_T0866_080715_FIX 
#else
		// ���[�N���̔j��
		WFLBY_EV_TALK_WK_Exit( p_evwk );

		// �C�x���g���[�N�j��
		WFLBY_EVENTWK_DeleteWk( p_wk );
#endif

		// �ʏ�V�[�P���X��
		WFLBY_SYSTEM_SetMyStatus( p_system, WFLBY_STATUS_LOGIN );
		{
			WFLBY_3DPERSON* p_player;
			WFLBY_3DOBJCONT* p_objcont;
			u32 idx;

			// �v���C���[���擾
			p_objcont = WFLBY_ROOM_GetObjCont( p_rmwk );
			p_player = WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );

#if PL_T0866_080715_FIX
			if( p_evwk->p_npc != NULL ){
				// NPC��������
				WFLBY_3DOBJCONT_DRAW_SetUpdata( p_evwk->p_npc, TRUE );

				// �C�x���g��������
				idx	= WFLBY_3DOBJCONT_GetWkObjData( p_evwk->p_npc, WF2DMAP_OBJPM_PLID );
				WFLBY_EVENT_SetPrivateEventMove( p_event, idx, TRUE );

				// �v���t�B�[���\������߂�
				WFLBY_ROOM_UNDERWIN_TrCardOff( p_rmwk );
			}
#else

			// NPC��������
			WFLBY_3DOBJCONT_DRAW_SetUpdata( p_evwk->p_npc, TRUE );

			// �C�x���g��������
			idx	= WFLBY_3DOBJCONT_GetWkObjData( p_evwk->p_npc, WF2DMAP_OBJPM_PLID );
			WFLBY_EVENT_SetPrivateEventMove( p_event, idx, TRUE );

			// �v���t�B�[���\������߂�
			WFLBY_ROOM_UNDERWIN_TrCardOff( p_rmwk );
#endif
		}

#if	PL_T0866_080715_FIX 
		// ���[�N���̔j��
		WFLBY_EV_TALK_WK_Exit( p_evwk );

		// �C�x���g���[�N�j��
		WFLBY_EVENTWK_DeleteWk( p_wk );
#else
#endif
		return TRUE;	// ���荞�݃C�x���g�I��
	}

	WFLBY_EV_TALK_WK_MainSelectWait( p_evwk );


	// �������M�V�X�e�����C��
	WFLBY_EV_TALK_RWSEND_Main( &p_evwk->rw_send, p_system );
	

	
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�b���������J�n
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_EV_TALK_StartB( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_TALK_WK* p_evwk;
	WFLBY_SYSTEM* p_system;
	u32 talk_seq;
	u32 recv_data;
	STRBUF* p_str;
	const WFLBY_USER_PROFILE* cp_mypr;
	const WFLBY_USER_PROFILE* cp_youpr;

	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );		// �V�X�e���f�[�^�擾
	p_evwk		= WFLBY_EVENTWK_GetWk( p_wk );				// �C�x���g�f�[�^�擾
	talk_seq	= WFLBY_SYSTEM_GetTalkSeq( p_system );		// ��b�V�[�P���X�擾
	recv_data	= WFLBY_SYSTEM_GetTalkRecv( p_system );		// ��M�f�[�^

	// ����[������������EX�I��
	if( WFLBY_SYSTEM_CheckTalkError( p_system ) ){
		if( p_evwk ){
			if( p_evwk->err_chk == TRUE ){
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG );
				WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
				p_evwk->err_chk = FALSE;
			}
		}
	}

	if( p_evwk ){
		cp_mypr		= WFLBY_SYSTEM_GetMyProfileLocal( p_system );
		cp_youpr	= WFLBY_SYSTEM_GetUserProfile( p_system, p_evwk->talk_idx );
	}

	// �����������㎩���Ƒ���̃��[�U�v���t�B�[�����擾����
	if( WFLBY_EVENTWK_GetSeq( p_wk ) > WFLBY_EV_TALK_B_SEQ_INIT ){
		if( p_evwk->err_chk == TRUE ){
			// ���肪���Ȃ��Ȃ����狭���I��
			if( cp_youpr == NULL ){	
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG );
				WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
				p_evwk->err_chk = FALSE;
			}
		}
	}
	

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ������
	case WFLBY_EV_TALK_B_SEQ_INIT:
		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_TALK_WK) );

		p_evwk->rireki_flag		= FALSE;
		p_evwk->rireki_actno	= 0;
		p_evwk->rireki_userid	= 0;
		
		{
			u32 talk_idx;
			BOOL result;

			talk_idx = WFLBY_SYSTEM_GetTalkIdx( p_system );

			// ���[�N������
			// �߂�l�Řb�����肪�ݐВ������ׂ�
			result = WFLBY_EV_TALK_WK_Init( p_evwk, p_system, talk_idx );

			// �b��������
			Snd_SePlay( WFLBY_SND_TALK );

			// ��b�V�[�P���X��
			WFLBY_SYSTEM_SetMyStatus( p_system, WFLBY_STATUS_TALK );

			// �b�����肪�����炻�̂܂ܐi��
			if( result == TRUE ){
				// NPC�̃v���t�B�[����\������
				WFLBY_ROOM_UNDERWIN_TrCardOn( p_rmwk, talk_idx, FALSE );

				// ���̐l�Ƙb�����ƁA�}�X�N�ݒ�
				WFLBY_SYSTEM_SetTalkMsk( p_system, talk_idx );

				// ��b���N�G�X�g��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_AISATUYOU );
			}else{
				// �b�����肪���Ȃ�������I���
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG );
				break;
			}

		}
		break;

	// ��������	�����Ă̎���
	case WFLBY_EV_TALK_B_SEQ_AISATUYOU:

		// ���A���肪�ڂ̑O�ɗ���܂ő҂�
		if( WFLBY_SYSTEM_CheckTalkBStart( p_system ) == FALSE ){
			break;
		}
		
		WFLBY_EV_TALK_WK_AisatuDraw( p_evwk, p_system, p_rmwk, p_evwk->you_timezone, cp_youpr, cp_mypr );

		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_AISATUMY, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// ��������	�����̎���
	case WFLBY_EV_TALK_B_SEQ_AISATUMY:

		// �`���m�[�g
		WFLBY_SYSTEM_FNOTE_SetTalk( p_system, p_evwk->talk_idx );

		// ���R�[�h
		WFLBY_SYSTEM_RECORD_AddTalk( p_system );

		// �V�񂾏��
		WFLBY_SYSTEM_PLAYED_SetPlay( p_system, WFLBY_PLAYED_TALK );



		WFLBY_EV_TALK_WK_AisatuDraw( p_evwk, p_system, p_rmwk, p_evwk->my_timezone, cp_mypr, cp_youpr );

		// �E�G�C�g�̌�ʐM����
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_SYNC_RECV, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );

		break;

	// ��b��ԓ���
	case WFLBY_EV_TALK_B_SEQ_SYNC_RECV:
		// ��b�f�[�^��M�҂�
		switch( talk_seq ){
		// �����m�F�f�[�^��M
		case WFLBY_TALK_SEQ_B_SEL:
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );

			// �����Ƀ_�~�[����
			WFLBY_SYSTEM_SendTalkData( p_system,  WFLBY_EV_TALK_TYPE_AISATSUSYNC );

			// ���̐l�Ƃ̉�b�����߂Ă��H
			if( WFLBY_SYSTEM_GetTalkFirst( p_system ) ){
				// ���߂Ă̂Ƃ�
				// ���A�t�H���[��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_AISATUFOLLOW );
			}else{
				// �����͂Ȃ������Ƃ�����
				// �E�G�C�g�̂��ƈ��A�I���Q��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_AISATUEND2 );
			}
			break;

		// �҂����
		case WFLBY_TALK_SEQ_A_SEL:	// ������ꑤ	�I��
			if( WFLBY_ROOM_TALKWIN_CheckTimeWait( p_rmwk ) == FALSE ){
				WFLBY_ROOM_TALKWIN_StartTimeWait( p_rmwk );
			}

			// �҂����Ԃ�WFLBY_TALK_RECV_HALF_WAIT�ɂȂ����烁�b�Z�[�W��\��
			WFLBY_EV_TALK_WK_PrintTimeWaitMsg( p_evwk, p_rmwk );
			break;


		// ���̑��ɂ͂Ȃ�Ȃ��͂�
		// EXEND������
		case WFLBY_TALK_SEQ_B_ANS:	// ������ꑤ	����
		case WFLBY_TALK_SEQ_EXEND:	// �ً}�I��	��b���I�������Ă�������
		case WFLBY_TALK_SEQ_NONE:	// ��b���I�������Ă�������
		default:
			// �ؒf�I��
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG );
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;
		}
		break;

	// �������t�H���[
	case WFLBY_EV_TALK_B_SEQ_AISATUFOLLOW:
		{
			BOOL result;
			// �t�H���[���K�v�H
			result = WFLBY_EV_TALK_WK_AisatuFollowDraw( p_evwk, p_system, p_rmwk );
			if( result == TRUE ){
				// �E�G�C�g�̂��ƈ��A�Ō��
				WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
						WFLBY_EV_TALK_B_SEQ_AISATUEND, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
			}else{
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_AISATUEND );
			}
		}
		break;
		
	// �������̍Ō�̃��b�Z�[�W
	case WFLBY_EV_TALK_B_SEQ_AISATUEND:
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_aisatu_03_01 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_aisatu_03_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_RECV_A_FST, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// ���A�̍Ō�̃��b�Z�[�W	�����P��͂Ȃ��Ă���Ƃ�
	case WFLBY_EV_TALK_B_SEQ_AISATUEND2:

		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->my_idx, 1 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_aisatu_03_02 );
		p_str = WFLBY_EV_TALK_GetStrBuf_Aisatu03_02( p_rmwk, p_evwk->talk_idx, p_evwk->my_idx );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�����̑I����
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_RECV_A_FST, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// ����̑I����e�҂�	�ŏ�
	case WFLBY_EV_TALK_B_SEQ_RECV_A_FST:
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_00_02 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_00_02 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_RECV_A, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// ����̑I����e�҂� �Q��ڈȍ~
	case WFLBY_EV_TALK_B_SEQ_RECV_A_SEC:	
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_00_08 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_00_08 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_RECV_A, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// ����̑I����e�҂�
	case WFLBY_EV_TALK_B_SEQ_RECV_A:
		// ��b�f�[�^��M�҂�
		switch( talk_seq ){
		// ��b���K�W�F�b�g�̑I���f�[�^����
		case WFLBY_TALK_SEQ_B_SEL:	// �b��������ꑤ	�I��
			switch( recv_data ){
			// �K�W�F�b�g�������
			case WFLBY_EV_TALK_TYPE_GADGET:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_MSG_G00 );

				// ��b����!!!
				WFLBY_SYSTEM_AddTalkCount( p_system );
				p_evwk->b_count_add = TRUE;

				// �K�W�F�b�g�����𗚗��ɐݒ�
				WFLBY_EV_TALK_WK_SetRirekiData( p_evwk, cp_youpr );
				break;
			// �K�W�F�b�g���ꏏ
			case WFLBY_EV_TALK_TYPE_GADGET_EQ:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_MSG_G_EQ );

				// ��b����!!!
				WFLBY_SYSTEM_AddTalkCount( p_system );
				p_evwk->b_count_add = TRUE;

				// �K�W�F�b�g�����𗚗��ɐݒ�
				WFLBY_EV_TALK_WK_SetRirekiData( p_evwk, cp_youpr );
				break;

			// ���A
			case WFLBY_EV_TALK_TYPE_TALK_START:
				// �����Ƀ_�~�[����
				WFLBY_SYSTEM_SendTalkData( p_system,  WFLBY_EV_TALK_TYPE_TALK_BEFORE );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_MSG_AT00 );
				break;

			// ���̑��͂��肦�Ȃ�
			default:
				// �ؒf�I��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG );
				WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
				break;
			}
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;

		// �҂����
		case WFLBY_TALK_SEQ_A_SEL:	// ������ꑤ	�I��
			if( WFLBY_ROOM_TALKWIN_CheckTimeWait( p_rmwk ) == FALSE ){
				WFLBY_ROOM_TALKWIN_StartTimeWait( p_rmwk );
			}

			// �҂����Ԃ�WFLBY_TALK_RECV_HALF_WAIT�ɂȂ����烁�b�Z�[�W��\��
			WFLBY_EV_TALK_WK_PrintTimeWaitMsg( p_evwk, p_rmwk );
			break;

		// �I����I������
		case WFLBY_TALK_SEQ_NONE:	// ��b���I�������Ă�������
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_ENDMSG );
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;

		// ���̑��ɂ͂Ȃ�Ȃ��͂�
		// EXEND������
		case WFLBY_TALK_SEQ_B_ANS:	// ������ꑤ	����
		case WFLBY_TALK_SEQ_EXEND:	// �ً}�I��	��b���I�������Ă�������
		default:
			// �ؒf�I��
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG );
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;
		}
		break;

	// �K�W�F�b�g���b�Z�[�W
	case WFLBY_EV_TALK_B_SEQ_MSG_G00:		// �K�W�F�b�g���b�Z�[�W
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		WFLBY_EV_TALK_WK_WordsetSetItem( p_evwk, p_system, p_rmwk, 1, TRUE );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_item_00_05 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_item_00_05 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_MSG_G01, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;
		
	// �K�W�F�b�g���b�Z�[�W
	case WFLBY_EV_TALK_B_SEQ_MSG_G01:		
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		WFLBY_EV_TALK_WK_WordsetSetItem( p_evwk, p_system, p_rmwk, 1, TRUE );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_item_00_06 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_item_00_06 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_MSG_G02, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// �K�W�F�b�g���b�Z�[�W
	case WFLBY_EV_TALK_B_SEQ_MSG_G02:		
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		WFLBY_EV_TALK_WK_WordsetSetItem( p_evwk, p_system, p_rmwk, 1, TRUE );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_item_00_07 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_item_00_07 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�h�������ɂ���ˁh
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_MSG_G03, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// �K�W�F�b�g���b�Z�[�W
	case WFLBY_EV_TALK_B_SEQ_MSG_G03:

		// �킯�Ă�����SE
		Snd_SePlay( WFLBY_SND_TOUCH_TOY_CHG );

		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_item_00_08 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_RECV_A_SEC, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );

		// �_�~�[����
		WFLBY_SYSTEM_SendTalkData( p_system,  WFLBY_EV_TALK_TYPE_GADGET_END );

		// �T�u��ʂ̕\�����X�V
		{
			u32  item;

			item = WFLBY_SYSTEM_GetProfileItem( WFLBY_SYSTEM_GetMyProfileLocal( p_system ) );
			WFLBY_ROOM_UNDERWIN_TrCardGadGetChange( p_rmwk, item );
		}
		break;

	// �K�W�F�b�g���ꏏ���b�Z�[�W
	case WFLBY_EV_TALK_B_SEQ_MSG_G_EQ:		
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		WFLBY_EV_TALK_WK_WordsetSetItem( p_evwk, p_system, p_rmwk, 1, TRUE );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_item_00_01 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_item_00_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_ENDMSG, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );

		// �_�~�[����
		WFLBY_SYSTEM_SendTalkData( p_system,  WFLBY_EV_TALK_TYPE_GADGET_END );
		break;

	// A��b�J�n���b�Z�[�W�\��
	case WFLBY_EV_TALK_B_SEQ_MSG_AT00:
		// ������Ƃ�����ׂ肵�悤��I
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_08_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_RECV_A_SEL, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;
		
	// A�̉�b���e���擾
	case WFLBY_EV_TALK_B_SEQ_RECV_A_SEL:	
		// ��b�f�[�^��M�҂�
		switch( talk_seq ){
		// ��b�̑I���f�[�^����
		case WFLBY_TALK_SEQ_B_SEL:	// �b��������ꑤ	�I��
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_MSG_AT01 );
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );

			// ��b����!!!
			WFLBY_SYSTEM_AddTalkCount( p_system );
			p_evwk->b_count_add = TRUE;
			break;

		// �҂����
		case WFLBY_TALK_SEQ_A_SEL:	// ������ꑤ	�I��
			if( WFLBY_ROOM_TALKWIN_CheckTimeWait( p_rmwk ) == FALSE ){
				WFLBY_ROOM_TALKWIN_StartTimeWait( p_rmwk );
			}

			// �҂����Ԃ�WFLBY_TALK_RECV_HALF_WAIT�ɂȂ����烁�b�Z�[�W��\��
			WFLBY_EV_TALK_WK_PrintTimeWaitMsg( p_evwk, p_rmwk );
			break;


		// ���̑��ɂ͂Ȃ�Ȃ��͂�
		// EXEND������
		case WFLBY_TALK_SEQ_B_ANS:	// ������ꑤ	����
		case WFLBY_TALK_SEQ_EXEND:	// �ً}�I��	��b���I�������Ă�������
		case WFLBY_TALK_SEQ_NONE:	// ��b���I�������Ă�������
		default:
			// �ؒf�I��
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG );
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			break;
		}
		break;
		
	// A�̑I�������\��
	case WFLBY_EV_TALK_B_SEQ_MSG_AT01:	// A�̑I�������\��
		// �s���Ȏ�M�f�[�^�`�F�b�N
		if( (recv_data < WFLBY_EV_TALK_TYPE_TALK_COL_00) || (recv_data >= WFLBY_EV_TALK_TYPE_TALK_NONE) ){
			// �ؒf�I��
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG );
			break;
		}
		
		// ��M�f�[�^���獀�ڂƃf�[�^�i���o�[�����߂�
		p_evwk->recv_k_data		= recv_data - WFLBY_EV_TALK_TYPE_TALK_COL_00;
		p_evwk->sel_k			= p_evwk->recv_k_data / WFLBY_EV_TALK_LIST_SEL_TK_S_MAX;

		// ���ڂ̂��Ƃ�����
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_01_01+p_evwk->sel_k );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_01_01+p_evwk->sel_k );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_MSG_AT02, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// A�̑I���Ȃ��\��
	case WFLBY_EV_TALK_B_SEQ_MSG_AT02:

		// ���ڂ̂��Ƃ�����
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_02_01+p_evwk->sel_k );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_02_01+p_evwk->sel_k );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_MSG_AT03, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;
		
	// A�̑I��I�񂾂��ƕ\��
	case WFLBY_EV_TALK_B_SEQ_MSG_AT03:
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_04_01+p_evwk->recv_k_data );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_04_01+p_evwk->recv_k_data );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_MSG_AT04, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// A�̑I������ɂȂ��\��
	case WFLBY_EV_TALK_B_SEQ_MSG_AT04:	
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_06_01+p_evwk->sel_k );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_06_01+p_evwk->sel_k );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A�b������̑I���܂�
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_SEL_T_M, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// �����̂�I��
	case WFLBY_EV_TALK_B_SEQ_SEL_T_M:	
		WFLBY_EV_TALK_WK_InitBmpList( &p_evwk->bmplist, p_rmwk, WFLBY_EV_TALK_LIST_SEL_TK_S_B_MAX, 
				msg_hiroba_osyaberi_03_01 + (p_evwk->sel_k*WFLBY_EV_TALK_LIST_SEL_TK_S_MAX), 
				WFLBY_EV_TALK_LIST_END_WAKARANAI );
		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist.bmplist, 0, 0 );
		WFLBY_EV_TALK_WK_StartSelectWait( p_evwk );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_SEND_T );
		break;
		
	// �����̑I���𑗐M
	case WFLBY_EV_TALK_B_SEQ_SEND_T:		
		{
			u32 result;
			BOOL del;

			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );
			switch( result ){
			case WFLBY_EV_TALK_LIST_SEL_TK_S_00:	
			case WFLBY_EV_TALK_LIST_SEL_TK_S_01:	
			case WFLBY_EV_TALK_LIST_SEL_TK_S_02:	
			case WFLBY_EV_TALK_LIST_SEL_TK_S_04:	

				// ��b���ڐݒ�
				p_evwk->sel_k_data = (p_evwk->sel_k*WFLBY_EV_TALK_LIST_SEL_TK_S_MAX) + result;
				WFLBY_SYSTEM_SendTalkData( p_system, p_evwk->sel_k_data+WFLBY_EV_TALK_TYPE_TALK_COL_00 );

				// �����̑I����\���`�F�b�N��
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_MSG_BT );

				del = TRUE;
				break;

			case WFLBY_EV_TALK_LIST_SEL_TK_S_MAX:			// 	B[�킩��Ȃ�]
				// ��b���ڐݒ�
				p_evwk->sel_k_data = WFLBY_EV_TALK_TYPE_TALK_NONE;
				WFLBY_SYSTEM_SendTalkData( p_system, p_evwk->sel_k_data );

				// �������Ȃ��Ƃ��̏I����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_TALK_B_SEQ_NONEENDMSG );

				del = TRUE;
				break;

			default:
				WFLBY_EV_TALK_WK_CheckAndPrintSelectWait( p_evwk, p_rmwk );
				del = FALSE;
				break;
			}

			if( del ){
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
				WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );
				WFLBY_EV_TALK_WK_StopSelectWait( p_evwk );

				// �������I�������̂ŉ�b�����ɐݒ�
				WFLBY_EV_TALK_WK_SetRirekiData( p_evwk, cp_youpr );
			}
		}
		break;

	// �����̑I���̕\��
	case WFLBY_EV_TALK_B_SEQ_MSG_BT:
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_04_01+p_evwk->sel_k_data );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_05_01+p_evwk->sel_k_data );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �I�����ڂ��ꏏ���`�F�b�N
		if( p_evwk->recv_k_data == p_evwk->sel_k_data ){

			// �������傾�˂�
			WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
					WFLBY_EV_TALK_B_SEQ_MSG_ABT_EQ, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		}else{
			// ���肪�Ƃ���
			WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
					WFLBY_EV_TALK_B_SEQ_MSG_AT_END, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		}
		break;

	// �I�����ڂ������Ƃ�
	case WFLBY_EV_TALK_B_SEQ_MSG_ABT_EQ:	
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_00_06 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_00_06 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// ���肪�Ƃ���
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_MSG_AT_END, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// ��b�I�����b�Z�[�W
	case WFLBY_EV_TALK_B_SEQ_MSG_AT_END:	
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_00_07 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_00_07 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// ���肪�Ƃ���
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_RECV_A_SEC, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		
		break;

	// �G���[�I��
	case WFLBY_EV_TALK_B_SEQ_ERR_ENDMSG:	

		// ���X�g���o�Ă��邩������Ȃ��̂ŏ���
		WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
		WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );

		// �I�𒆃��b�Z�[�W�\��
		// ������쐬
//		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_aisatu_04_04 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_aisatu_04_04 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �����G���[�`�F�b�N�K�v�Ȃ�
		p_evwk->err_chk = FALSE;

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_END, p_wk, WFLBY_EV_TALK_B_SEQ_WAIT );
//				WFLBY_EV_TALK_B_SEQ_END, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );

		// �����I���������Ƃ�������
		WFLBY_SYSTEM_SendTalkExEnd( p_system );
		break;

	// �ʏ�I��
	case WFLBY_EV_TALK_B_SEQ_ENDMSG:		
		// �I�𒆃��b�Z�[�W�\��
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_aisatu_04_02 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_aisatu_04_02 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �����G���[�`�F�b�N�K�v�Ȃ�
		p_evwk->err_chk = FALSE;

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_END, p_wk, WFLBY_EV_TALK_B_SEQ_WAIT );
//				WFLBY_EV_TALK_B_SEQ_END, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;

	// �������Ȃ��Ƃ��I��
	case WFLBY_EV_TALK_B_SEQ_NONEENDMSG:	
		// ������쐬
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
//		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_00_04 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_00_04 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �E�G�C�g�̌�A���X�g�\��
		WFLBY_EV_TALK_WK_Wait( p_evwk, WFLBY_EV_TALK_MSG_WAIT, 
				WFLBY_EV_TALK_B_SEQ_ENDMSG, p_wk, WFLBY_EV_TALK_B_SEQ_MSGWAIT );
		break;
		
	// �ҋ@����
	case WFLBY_EV_TALK_B_SEQ_WAIT:
		WFLBY_EV_TALK_WK_WaitMain( p_evwk, p_wk );
		break;

	// ���b�Z�[�W�̏I���E�G�C�g
	case WFLBY_EV_TALK_B_SEQ_MSGWAIT:
		WFLBY_EV_TALK_WK_MsgEndWaitMain( p_evwk, p_wk, p_rmwk );
		break;

	// �I��
	case WFLBY_EV_TALK_B_SEQ_END:		

		// �Ō�ɂ������Ƃɉ�b��ݒ�
		if( p_evwk->rireki_flag == TRUE ){
			WFLBY_SYSTEM_SetLastAction( p_system, p_evwk->rireki_actno, p_evwk->rireki_userid );
		}

		// ��b�Ȃǉ��������ɏI���ɂ�����
		// ��b�J�E���g�𑫂�
		if( p_evwk->b_count_add == FALSE ){
			// ��b����!!!
			WFLBY_SYSTEM_AddTalkCountEx( p_system, p_evwk->talk_idx );
		}

		// ��b�I���ɂ���
		WFLBY_SYSTEM_EndTalk( p_system );
		
		// ���b�Z�[�W�I��
		WFLBY_ROOM_TALKWIN_Off( p_rmwk );

		// ���[�N���̔j��
		WFLBY_EV_TALK_WK_Exit( p_evwk );
		
		// �C�x���g���[�N�j��
		WFLBY_EVENTWK_DeleteWk( p_wk );

		// �ʏ�V�[�P���X��
		WFLBY_SYSTEM_SetMyStatus( p_system, WFLBY_STATUS_LOGIN );
		{
			WFLBY_3DPERSON* p_player;
			WFLBY_3DOBJCONT* p_objcont;

			// �v���C���[���擾
			p_objcont = WFLBY_ROOM_GetObjCont( p_rmwk );
			p_player = WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );

			// �v���t�B�[���\������߂�
			WFLBY_ROOM_UNDERWIN_TrCardOff( p_rmwk );
		}
		return TRUE;	// ���荞�݃C�x���g�I��
	}

	WFLBY_EV_TALK_WK_MainSelectWait( p_evwk );

	return FALSE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N���o�����b
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_EV_DEF_PlayerA_SWTOPIC_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_PLAYER_WFTOPIC_WK* p_evwk;
	STRBUF* p_str;
	WFLBY_SYSTEM* p_system;

	p_evwk = WFLBY_EVENTWK_GetWk( p_wk );
	p_system = WFLBY_ROOM_GetSystemData( p_rmwk );
	
	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ������
	case WFLBY_EV_DEF_PLAYER_TOPIC_INIT:		
		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_DEF_PLAYER_WFTOPIC_WK) );

		// �b��������ꂽ��
		Snd_SePlay( WFLBY_SND_STAFF );

		// �N���[�Y�O�����Ƃ��ŕύX
		if( WFLBY_SYSTEM_Event_GetEndCM( p_system ) == TRUE ){

			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_CLOSE );
		}else{

			// PARADE�����`�F�b�N
			if( WFLBY_SYSTEM_Event_GetParade( p_system ) == TRUE ){
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_PARADE );
			}else{
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSG00 );
			}
		}
		break;

	// PARADE���b�Z�[�W
	case WFLBY_EV_DEF_PLAYER_TOPIC_PARADE:	
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_06 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_END;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;

	// �N���[�Y�O���b�Z�[�W
	case WFLBY_EV_DEF_PLAYER_TOPIC_CLOSE:	
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_04 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_END;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;

	// �悤����
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSG00:	
		
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;

	// �ǂ���ɂ��āE�E�E
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSG01:	
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_02 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_LISTON;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;
		
	// ���X�g�\��
	case WFLBY_EV_DEF_PLAYER_TOPIC_LISTON:	
		WFLBY_EV_TOPIC_WK_InitBmpList( &p_evwk->bmplist, p_rmwk );
		WFLBY_ROOM_LISTWIN_Start_Ex( p_rmwk, &p_evwk->bmplist.bmplist, 0, 0, 
				WFLBY_EV_DEF_PLAYER_TOPIC_LISTWIN_X, WFLBY_EV_DEF_PLAYER_TOPIC_LISTWIN_Y, 
				WFLBY_EV_DEF_PLAYER_TOPIC_LISTWIN_SIZX );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_LISTWAIT );
		break;
		
	// ���X�g�҂�
	case WFLBY_EV_DEF_PLAYER_TOPIC_LISTWAIT:	
		{
			u32 result;
			BOOL delete = FALSE;
			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );

			switch( result ){
			case WFLBY_EV_TOPIC_LIST_SEL_GADGET:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGGADGET );
				delete = TRUE;
				break;
			case WFLBY_EV_TOPIC_LIST_SEL_MINI:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI );
				delete = TRUE;
				break;
			case WFLBY_EV_TOPIC_LIST_SEL_WLD:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWLD );
				delete = TRUE;
				break;
			case WFLBY_EV_TOPIC_LIST_SEL_TOP:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGTOP );
				delete = TRUE;
				break;
			case WFLBY_EV_TOPIC_LIST_SEL_FOOT:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGBOAD );
				delete = TRUE;
				break;
			case WFLBY_EV_TOPIC_LIST_SEL_ANKETO:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGANKETO );
				delete = TRUE;
				break;
			case WFLBY_EV_TOPIC_LIST_SEL_TIME:	
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE );
				delete = TRUE;
				break;
			case BMPLIST_CANCEL:
				Snd_SePlay( SEQ_SE_DP_SELECT );	// CANCEL��
			case WFLBY_EV_TOPIC_LIST_SEL_END:	
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGEND );
				delete = TRUE;
				break;
			
			default:
				break;
			}

			if( delete == TRUE ){
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
				WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );
			}
		}
		break;

	// �K�W�F�b�g
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGGADGET:
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_09 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;
		
	// �~�j�Q�[��
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI:	
		WFLBY_EV_TOPIC_WK_InitMinigameBmpList( &p_evwk->bmplist, p_rmwk );
		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist.bmplist, 0, 0 );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_SEL );
		break;

	// �~�j�Q�[������ɑI��
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_SEL:	
		{
			u32 result;
			BOOL delete = FALSE;
			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );

			switch( result ){
			case WFLBY_EV_TOPIC_LIST_SEL_MG_BALLSLOW:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_BS );
				delete = TRUE;
				break;
				
			case WFLBY_EV_TOPIC_LIST_SEL_MG_BALANCE:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_BB );
				delete = TRUE;
				break;
				
			case WFLBY_EV_TOPIC_LIST_SEL_MG_BALLOON:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_BL );
				delete = TRUE;
				break;

			case BMPLIST_CANCEL:
				Snd_SePlay( SEQ_SE_DP_SELECT );	// CANCEL��
			case WFLBY_EV_TOPIC_LIST_SEL_MG_END:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSG01 );
				delete = TRUE;
				break;

			
			default:
				break;
			}

			if( delete == TRUE ){
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
				WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );
			}
		}
		break;
	
	// �^�}����
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_BS:	
		WFLBY_ROOM_MSG_SetMinigame( p_rmwk, WFLBY_GAME_BALLSLOW, 0 );
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_10 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;
	
	// ���܂̂�
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_BB:	
		WFLBY_ROOM_MSG_SetMinigame( p_rmwk, WFLBY_GAME_BALANCEBALL, 0 );
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_11 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;

	// �ӂ�������
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGMINI_BL:	
		WFLBY_ROOM_MSG_SetMinigame( p_rmwk, WFLBY_GAME_BALLOON, 0 );
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_12 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;
		
	// ���E���v
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGWLD:	
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_04 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;
		
	// �j���[�X
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGTOP:	
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_05 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;
		
	// ���Ճ{�[�h
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGBOAD:	
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_06 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;
		
	// �C�x���g
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE:	

		WFLBY_EV_TOPIC_WK_InitEventBmpList( &p_evwk->bmplist, p_rmwk );
		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist.bmplist, 0, 0 );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_SEL );
		break;

	// �C�x���g����ɑI��
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_SEL:
		{
			u32 result;
			BOOL delete = FALSE;
			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );

			switch( result ){
			case WFLBY_EV_TOPIC_LIST_SEL_EVE_LIGHTING:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_LIGHTING );
				delete = TRUE;
				break;
				
			case WFLBY_EV_TOPIC_LIST_SEL_EVE_FIRE:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_FIRE );
				delete = TRUE;
				break;
				
			case WFLBY_EV_TOPIC_LIST_SEL_EVE_PARADE:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_PARADE );
				delete = TRUE;
				break;

			case BMPLIST_CANCEL:
				Snd_SePlay( SEQ_SE_DP_SELECT );	// CANCEL��
			case WFLBY_EV_TOPIC_LIST_SEL_EVE_END:
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSG01 );
				delete = TRUE;
				break;

			default:
				break;
			}

			if( delete == TRUE ){
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );
				WFLBY_EV_TALK_WK_ExitBmpList( &p_evwk->bmplist, p_rmwk );
			}
		}
		break;

	// �C�x���g�@���C�e�B���O
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_LIGHTING:	
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_13 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;

	// �C�x���g�@�͂Ȃ�
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_FIRE:		
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_14 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;

	// �C�x���g�@�p���[�h
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGEVE_PARADE:	
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_15 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;

	// �A���P�[�g
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGANKETO:
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_16 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;
		
	// ��߂�
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGEND:	
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infob_08 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYER_TOPIC_END;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT );
		break;
		
	// ���b�Z�[�W�I���҂�
	case WFLBY_EV_DEF_PLAYER_TOPIC_MSGWAIT:	
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) ){
			WFLBY_EVENTWK_SetSeq( p_wk, p_evwk->ret_seq );
		}
		break;

	// �I��
	case WFLBY_EV_DEF_PLAYER_TOPIC_END:	
		WFLBY_EVENTWK_DeleteWk( p_wk );
		WFLBY_ROOM_TALKWIN_Off( p_rmwk );

		{
			WFLBY_3DOBJCONT* p_objcont;
			WFLBY_3DPERSON* p_person;

			p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
			p_person	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_person, WFLBY_3DOBJCONT_MOVEPLAYER );
		}
		return TRUE;
	}

	return FALSE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N���Z�����b		�V�т����߂�l
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_EV_DEF_PlayerA_SWTOPIC_PLAYED_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_PLAYER_WFTOPIC_WK* p_evwk;
	WFLBY_SYSTEM* p_system;
	STRBUF* p_str;

	p_evwk = WFLBY_EVENTWK_GetWk( p_wk );
	p_system = WFLBY_ROOM_GetSystemData( p_rmwk );
	
	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	case WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_INIT:		// ������

		// �b��������ꂽ��
		Snd_SePlay( WFLBY_SND_STAFF );

		// PARADE����PARADE�̂��Ƃ�����
		if( WFLBY_SYSTEM_Event_GetEndCM( p_system ) == FALSE ){
			// PARADE�����`�F�b�N
			if( WFLBY_SYSTEM_Event_GetParade( p_system ) == TRUE ){
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_06 );
				// ���b�Z�[�W�`��
				WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_MSGWAIT );
				break;
			}
		}else{
			// �I�����͏I�����̂��Ƃ�����
			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_04 );
			// ���b�Z�[�W�`��
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_MSGWAIT );
			break;
		}
		
		{
			u32 played_type;



			played_type = WFLBY_SYSTEM_PLAYED_GetOsusumePlayType( p_system );
			// �����o�����`�F�b�N
			switch(played_type){
			// �^�}����
			case WFLBY_PLAYED_BALLTHROW:	
				WFLBY_ROOM_MSG_SetMinigame( p_rmwk, WFLBY_GAME_BALLSLOW, 0 );
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_01 );
				break;
			// Balance�{�[��
			case WFLBY_PLAYED_BALANCEBALL:
				WFLBY_ROOM_MSG_SetMinigame( p_rmwk, WFLBY_GAME_BALANCEBALL, 0 );
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_01 );
				break;
			// �ӂ�������
			case WFLBY_PLAYED_BALLOON:		
				WFLBY_ROOM_MSG_SetMinigame( p_rmwk, WFLBY_GAME_BALLOON, 0 );
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_01 );
				break;
			// ���E���v	
			case WFLBY_PLAYED_WORLDTIMER:	
				WFLBY_ROOM_MSG_SetMinigame( p_rmwk, WFLBY_GAME_WLDTIMER, 0 );
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_05 );
				break;
			// �L��j���[�X
			case WFLBY_PLAYED_TOPIC:			
				WFLBY_ROOM_MSG_SetMinigame( p_rmwk, WFLBY_GAME_NEWS, 0 );
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_05 );
				break;
			// ����BOARD
			case WFLBY_PLAYED_FOOTBOARD:		
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_02 );
				break;
			// �A���P�[�g
			case WFLBY_PLAYED_ANKETO:
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_07 );
				break;
			// �b������
			case WFLBY_PLAYED_TALK:			
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_03 );
				break;
			// �����S���V��
			default:
			case WFLBY_PLAYED_NUM:			
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_04 );
				break;
			}


			// ���b�Z�[�W�`��
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		}

		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_MSGWAIT );
		break;
		
	case WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_MSGWAIT:	// ���b�Z�[�W�\�������҂�
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) ){
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_END );
		}
		break;
		
	case WFLBY_EV_DEF_PLAYER_TOPIC_PLAYED_END:	
		WFLBY_ROOM_TALKWIN_Off( p_rmwk );

		{
			WFLBY_3DOBJCONT* p_objcont;
			WFLBY_3DPERSON* p_person;

			p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
			p_person	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_person, WFLBY_3DOBJCONT_MOVEPLAYER );
		}
		return TRUE;
	}

	return FALSE;
}




#ifdef WFLBY_DEBUG_TALK_ALLWORLD
static u8 s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_MY = 0;
static u8 s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_YOU = 0;
static u8 s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_TIMEZONE = 0;
BOOL WFLBY_EV_TALK_StartA_AllWorld( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_TALK_WK* p_evwk;
	WFLBY_SYSTEM* p_system;
	STRBUF* p_str;
	const WFLBY_USER_PROFILE* cp_mypr;
	const WFLBY_USER_PROFILE* cp_youpr;

	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );		// �V�X�e���f�[�^�擾
	p_evwk		= WFLBY_EVENTWK_GetWk( p_wk );				// �C�x���g�f�[�^�擾

	cp_mypr		= WFLBY_SYSTEM_GetMyProfile( p_system );
	cp_youpr	= WFLBY_SYSTEM_GetUserProfile( p_system, 1 );	// �K����


	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// �\��
	case 0:
		WFLBY_SYSTEM_DEBUG_SetLangCode( (WFLBY_USER_PROFILE*)cp_mypr, s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_MY );
		WFLBY_SYSTEM_DEBUG_SetLangCode( (WFLBY_USER_PROFILE*)cp_youpr, s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_YOU );

		WFLBY_EV_TALK_WK_AisatuDraw( NULL, p_system, p_rmwk, s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_TIMEZONE, cp_mypr, cp_youpr );

		WFLBY_EVENTWK_SetSeq( p_wk, 1 );
		break;

	// ���[�v
	case 1:

		// A���������܂ł܂�
		if( (sys.trg & PAD_BUTTON_A) == 0 ){
			break;
		}

		if( (s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_TIMEZONE+1) < 3 ){
			s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_TIMEZONE++;
		}else{
			s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_TIMEZONE = 0;

			if( (s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_YOU+1) < WFLBY_EV_TALK_LANG_NUM ){
				s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_YOU ++;
			}else{
				s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_YOU = 0;

				if( (s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_MY+1) < WFLBY_EV_TALK_LANG_NUM ){
					s_WFLBY_DEBUG_TALK_ALLWORLD_COUNT_MY ++;
				}else{

					// �I����
					WFLBY_EVENTWK_SetSeq( p_wk, 2 );
					break;
				}
			}
		}

		// �ĕ\����
		WFLBY_EVENTWK_SetSeq( p_wk, 0 );
		break;


	// �I��
	case 2:
		// ���b�Z�[�W�I��
		WFLBY_ROOM_TALKWIN_Off( p_rmwk );

		// ���[�N���̔j��
		WFLBY_EV_TALK_WK_Exit( p_evwk );
		
		// �C�x���g���[�N�j��
		WFLBY_EVENTWK_DeleteWk( p_wk );

		// �ʏ�V�[�P���X��
		WFLBY_SYSTEM_SetMyStatus( p_system, WFLBY_STATUS_LOGIN );
		{
			WFLBY_3DPERSON* p_player;
			WFLBY_3DOBJCONT* p_objcont;

			// �v���C���[���擾
			p_objcont = WFLBY_ROOM_GetObjCont( p_rmwk );
			p_player = WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );
		}
		return TRUE;	// ���荞�݃C�x���g�I��
		break;
	}


	return FALSE;
}
#endif



//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	���A���ԃ]�[�����擾����
 *
 *	@param	cp_pr		�v���t�B�[��
 *
 *	@return	���ԃ]�[��
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_EV_TALK_GetTimeZone( const WFLBY_USER_PROFILE* cp_pr )
{
	WFLBY_TIME time;
	time = WFLBY_SYSTEM_GetProfileWldTime( cp_pr );
	
	switch( time.hour ){
	// ��
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		return WFLBY_AISATSU_MORNING;

	// ��
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
		return WFLBY_AISATSU_NOON;

	// ��
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 0:
	case 1:
	case 2:
	case 3:
		return WFLBY_AISATSU_NIGHT;
	}

	GF_ASSERT( 0 );	// ���肦�Ȃ�
	return WFLBY_AISATSU_NOON;
}



//----------------------------------------------------------------------------
/**
 *	@brief	��������擾����
 *
 *	@param	p_rmwk		���[�N
 *	@param	plidx		���̕����𔭌�����v���C���[�C���f�b�N�X
 *	@param	msgidx		���b�Z�[�W�C���f�b�N�X
 *
 *	@return	������
 */
//-----------------------------------------------------------------------------
static STRBUF* WFLBY_EV_TALK_GetStrBuf( WFLBY_ROOMWK* p_rmwk, u32 plidx, u32 msgidx )
{
	BOOL vip;
	WFLBY_SYSTEM* p_system;
	int i;

	p_system = WFLBY_ROOM_GetSystemData( p_rmwk );
	
	// plidx�̐l��VIP����Ȃ����`�F�b�N
	vip = WFLBY_SYSTEM_GetUserVipFlag( p_system, plidx );
	if( vip == TRUE ){
		
		// msgidx��VIP�p���b�Z�[�W�����邩�`�F�b�N
		for( i=0; i<NELEMS(sc_WFLBY_VIP_MSGTBL); i++ ){
			if( msgidx == sc_WFLBY_VIP_MSGTBL[i].normal_msg ){
				// VIP�p���b�Z�[�W�ɏ���������
				msgidx = sc_WFLBY_VIP_MSGTBL[i].vip_msg;	
				break;
			}
		}
	}
	
	return WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msgidx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�Q��ڂɂ������Ƃ��̕�������擾����
 *
 *	@param	p_rmwk		���[�N
 *	@param	aidx		�b������������IDX
 *	@param	bidx		�b��������ꂽ����IDX
 *
 *	@return	������
 */
//-----------------------------------------------------------------------------
static STRBUF* WFLBY_EV_TALK_GetStrBuf_Aisatu03_02( WFLBY_ROOMWK* p_rmwk, u32 aidx, u32 bidx )
{
	BOOL a_vip, b_vip;
	WFLBY_SYSTEM* p_system;
	u32 msgidx;

	p_system = WFLBY_ROOM_GetSystemData( p_rmwk );
	
	// plidx�̐l��VIP����Ȃ����`�F�b�N
	a_vip	= WFLBY_SYSTEM_GetUserVipFlag( p_system, aidx );
	b_vip	= WFLBY_SYSTEM_GetUserVipFlag( p_system, bidx );
	
	msgidx = msg_hiroba_aisatu_03_02;
	if( (a_vip == TRUE) && (b_vip == TRUE) ){
		msgidx = msg_vip_aisatu_03_02_01;
	}
	else if( (a_vip == TRUE) && (b_vip == FALSE) ){
		msgidx = msg_vip_aisatu_03_02;
	}
	else if( (a_vip == FALSE) && (b_vip == TRUE) ){
		msgidx = msg_vip_aisatu_03_02_00;
	}
	
	return WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msgidx );
}






//----------------------------------------------------------------------------
/**
 *	@brief	��b�f�[�^������
 *
 *	@param	p_evwk		�C�x���g���[�N
 *	@param	p_system	�V�X�e���f�[�^
 *	@param	idx			��b����C���f�b�N�X
 *
 *	@retval	TRUE	��b����ݐ�
 *	@retval	FALSE	��b���肪���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_TALK_WK_Init( WFLBY_EV_TALK_WK* p_evwk, WFLBY_SYSTEM* p_system, u32 idx )
{
	GF_ASSERT( idx != DWC_LOBBY_INVALID_USER_ID );
	
	// ��b����
	p_evwk->talk_idx = idx;

	//����
	p_evwk->my_idx = WFLBY_SYSTEM_GetMyIdx( p_system );

	// �G���[�`�F�b�NON
	p_evwk->err_chk = TRUE;
	
	// �v���t�B�[���f�[�^�𔲂��o��
	{
		const WFLBY_USER_PROFILE* cp_mypr;
		const WFLBY_USER_PROFILE* cp_youpr;
		cp_mypr		= WFLBY_SYSTEM_GetMyProfileLocal( p_system );
		cp_youpr	= WFLBY_SYSTEM_GetUserProfile( p_system, p_evwk->talk_idx );
		if( cp_youpr == NULL ){
			return FALSE;
		}

		// �^�C���]�[���擾
		p_evwk->my_timezone		= WFLBY_EV_TALK_GetTimeZone( cp_mypr );
		p_evwk->you_timezone	= WFLBY_EV_TALK_GetTimeZone( cp_youpr );

	}

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�̔j������
 *
 *	@param	p_evwk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_Exit( WFLBY_EV_TALK_WK* p_evwk )
{
}



//----------------------------------------------------------------------------
/**
 *	@brief	�E�G�C�g	�J�n
 *
 *	@param	p_evwk		���[�N
 *	@param	wait		�E�G�C�g�l�i���b�Z�[�W�I���҂��̂Ƃ��͂ǂ�Ȓl�ł��n�j�j
 *	@param	ret_seq		�߂��Ă���V�[�P���X
 *	@param	p_wk		�C�x���g���[�N
 *	@param	wait_seq	�ҋ@�V�[�P���X
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_Wait( WFLBY_EV_TALK_WK* p_evwk, u8 wait, u8 ret_seq, WFLBY_EVENTWK* p_wk, u8 wait_seq )
{
	p_evwk->wait		= wait;
	p_evwk->next_seq	= ret_seq;

	// �ҋ@�V�[�P���X��
	WFLBY_EVENTWK_SetSeq( p_wk, wait_seq );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�G�C�g���C��
 *
 *	@param	p_evwk	���[�N
 *	@param	p_wk	�C�x���g���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_WaitMain( WFLBY_EV_TALK_WK* p_evwk, WFLBY_EVENTWK* p_wk )
{
	if( p_evwk->wait > 0 ){
		p_evwk->wait --;
	}else{

		// �V�[�P���X��߂�
		WFLBY_EVENTWK_SetSeq( p_wk, p_evwk->next_seq );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�I���҂�
 *
 *	@param	p_evwk	���[�N
 *	@param	p_wk	�C�x���g���[�N
 *	@param	p_rmwk	�������[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_MsgEndWaitMain( WFLBY_EV_TALK_WK* p_evwk, WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk )
{
	if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) ){
		// �V�[�P���X��߂�
		WFLBY_EVENTWK_SetSeq( p_wk, p_evwk->next_seq );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���A�̕\��
 *
 *	@param	p_evwk		���[�N
 *	@param	p_system	�V�X�e�����[�N
 *	@param	p_rmwk		���[�����[�N
 *	@param	timezone	�\�����鎞�ԃ]�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_AisatuDraw( WFLBY_EV_TALK_WK* p_evwk, WFLBY_SYSTEM* p_system, WFLBY_ROOMWK* p_rmwk, u32 timezone, const WFLBY_USER_PROFILE* cp_mypr, const WFLBY_USER_PROFILE* cp_youpr )
{
	u32 data_id;
	u32 my_lang, you_lang;
	static const u16 sc_WFLBY_EV_TALK_AISATSU_MSGIDX[WFLBY_EV_TALK_LANG_NUM][WFLBY_EV_TALK_LANG_NUM] = {
		// NONE
		{
			msg_hiroba_aisatu_01_32,		// NONE
			msg_hiroba_aisatu_01_32,		// LANG_JAPAN
			msg_hiroba_aisatu_01_32,		// LANG_ENGLISH
			msg_hiroba_aisatu_01_32,		// LANG_FRANCE
			msg_hiroba_aisatu_01_32,		// LANG_ITALY
			msg_hiroba_aisatu_01_32,		// LANG_GERMANY
			msg_hiroba_aisatu_01_32,		// NONE
			msg_hiroba_aisatu_01_32,		// LANG_SPAIN
		},

		// LANG_JAPAN
		{
			msg_hiroba_aisatu_01_02,		// NONE
			msg_hiroba_aisatu_01_01,		// LANG_JAPAN
			msg_hiroba_aisatu_01_02,		// LANG_ENGLISH
			msg_hiroba_aisatu_01_04,		// LANG_FRANCE
			msg_hiroba_aisatu_01_06,		// LANG_ITALY
			msg_hiroba_aisatu_01_08,		// LANG_GERMANY
			msg_hiroba_aisatu_01_02,		// NONE
			msg_hiroba_aisatu_01_10,		// LANG_SPAIN
		},

		// LANG_ENGLISH
		{
			msg_hiroba_aisatu_01_33,		// NONE
			msg_hiroba_aisatu_01_03,		// LANG_JAPAN
			msg_hiroba_aisatu_01_33,		// LANG_ENGLISH
			msg_hiroba_aisatu_01_12,		// LANG_FRANCE
			msg_hiroba_aisatu_01_14,		// LANG_ITALY
			msg_hiroba_aisatu_01_16,		// LANG_GERMANY
			msg_hiroba_aisatu_01_33,		// NONE
			msg_hiroba_aisatu_01_18,		// LANG_SPAIN
		},

		// LANG_FRANCE
		{
			msg_hiroba_aisatu_01_13,		// NONE
			msg_hiroba_aisatu_01_05,		// LANG_JAPAN
			msg_hiroba_aisatu_01_13,		// LANG_ENGLISH
			msg_hiroba_aisatu_01_34,		// LANG_FRANCE
			msg_hiroba_aisatu_01_20,		// LANG_ITALY
			msg_hiroba_aisatu_01_22,		// LANG_GERMANY
			msg_hiroba_aisatu_01_13,		// NONE
			msg_hiroba_aisatu_01_24,		// LANG_SPAIN
		},

		// LANG_ITALY
		{
			msg_hiroba_aisatu_01_15,		// NONE
			msg_hiroba_aisatu_01_07,		// LANG_JAPAN
			msg_hiroba_aisatu_01_15,		// LANG_ENGLISH
			msg_hiroba_aisatu_01_21,		// LANG_FRANCE
			msg_hiroba_aisatu_01_35,		// LANG_ITALY
			msg_hiroba_aisatu_01_26,		// LANG_GERMANY
			msg_hiroba_aisatu_01_15,		// NONE
			msg_hiroba_aisatu_01_28,		// LANG_SPAIN
		},

		// LANG_GERMANY
		{
			msg_hiroba_aisatu_01_17,		// NONE
			msg_hiroba_aisatu_01_09,		// LANG_JAPAN
			msg_hiroba_aisatu_01_17,		// LANG_ENGLISH
			msg_hiroba_aisatu_01_23,		// LANG_FRANCE
			msg_hiroba_aisatu_01_27,		// LANG_ITALY
			msg_hiroba_aisatu_01_36,		// LANG_GERMANY
			msg_hiroba_aisatu_01_17,		// NONE
			msg_hiroba_aisatu_01_30,		// LANG_SPAIN
		},

		// NONE
		{
			msg_hiroba_aisatu_01_32,		// NONE
			msg_hiroba_aisatu_01_32,		// LANG_JAPAN
			msg_hiroba_aisatu_01_32,		// LANG_ENGLISH
			msg_hiroba_aisatu_01_32,		// LANG_FRANCE
			msg_hiroba_aisatu_01_32,		// LANG_ITALY
			msg_hiroba_aisatu_01_32,		// LANG_GERMANY
			msg_hiroba_aisatu_01_32,		// NONE
			msg_hiroba_aisatu_01_32,		// LANG_SPAIN
		},

		// LANG_SPAIN
		{
			msg_hiroba_aisatu_01_19,		// NONE
			msg_hiroba_aisatu_01_11,		// LANG_JAPAN
			msg_hiroba_aisatu_01_19,		// LANG_ENGLISH
			msg_hiroba_aisatu_01_25,		// LANG_FRANCE
			msg_hiroba_aisatu_01_29,		// LANG_ITALY
			msg_hiroba_aisatu_01_31,		// LANG_GERMANY
			msg_hiroba_aisatu_01_19,		// NONE
			msg_hiroba_aisatu_01_37,		// LANG_SPAIN
		},
	};

	// �����̈��A�����[�h�Z�b�g�ɐݒ�
	WFLBY_EV_TALK_WK_Aisatu_SetWordSet( p_rmwk, 0, timezone, cp_mypr );
	// ����̈��A�����[�h�Z�b�g�ɐݒ�
	WFLBY_EV_TALK_WK_Aisatu_SetWordSet( p_rmwk, 1, timezone, cp_youpr );

	// �����Ƒ���̍��R�[�h�擾
	// �s���ȍ��R�[�h�Ȃ�p��ɂ���
	my_lang = WFLBY_SYSTEM_GetProfileRagionCode( cp_mypr );
	you_lang = WFLBY_SYSTEM_GetProfileRagionCode( cp_youpr );

	data_id = sc_WFLBY_EV_TALK_AISATSU_MSGIDX[my_lang][you_lang];
	
	
/*	

	// ����Ǝ����̍�����\�����镶���f�[�^���擾
	switch( WFLBY_SYSTEM_GetProfileRagionCode( cp_mypr ) ){
	// ���������{
	case LANG_JAPAN:
		WFLBY_ROOM_MSG_SetAisatsuJapan( p_rmwk, 0, timezone );
		switch( WFLBY_SYSTEM_GetProfileRagionCode( cp_youpr ) ){
		case LANG_JAPAN:	
			data_id = msg_hiroba_aisatu_01_01;
			WFLBY_ROOM_MSG_SetAisatsuJapan( p_rmwk, 1, timezone );
			break;
		case LANG_ENGLISH:	
			data_id = msg_hiroba_aisatu_01_02; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		case LANG_FRANCE:	
			data_id = msg_hiroba_aisatu_01_04; 
			WFLBY_ROOM_MSG_SetAisatsuFrance( p_rmwk, 1, timezone );
			break;
		case LANG_ITALY:	
			data_id = msg_hiroba_aisatu_01_06; 
			WFLBY_ROOM_MSG_SetAisatsuItaly( p_rmwk, 1, timezone );
			break;
		case LANG_GERMANY:	
			data_id = msg_hiroba_aisatu_01_08; 
			WFLBY_ROOM_MSG_SetAisatsuGerMany( p_rmwk, 1, timezone );
			break;
		case LANG_SPAIN:	
			data_id = msg_hiroba_aisatu_01_10; 
			WFLBY_ROOM_MSG_SetAisatsuSpain( p_rmwk, 1, timezone );
			break;
		default:			
			data_id = msg_hiroba_aisatu_01_02; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		}
		break;

	// �������t�����X
	case LANG_FRANCE:	
		WFLBY_ROOM_MSG_SetAisatsuFrance( p_rmwk, 0, timezone );
		switch( WFLBY_SYSTEM_GetProfileRagionCode( cp_youpr ) ){
		case LANG_JAPAN:	
			data_id = msg_hiroba_aisatu_01_05; 
			WFLBY_ROOM_MSG_SetAisatsuJapan( p_rmwk, 1, timezone );
			break;
		case LANG_ENGLISH:	
			data_id = msg_hiroba_aisatu_01_13; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		case LANG_FRANCE:	
			data_id = msg_hiroba_aisatu_01_33; 
			WFLBY_ROOM_MSG_SetAisatsuFrance( p_rmwk, 1, timezone );
			break;
		case LANG_ITALY:	
			data_id = msg_hiroba_aisatu_01_19; 
			WFLBY_ROOM_MSG_SetAisatsuItaly( p_rmwk, 1, timezone );
			break;
		case LANG_GERMANY:	
			data_id = msg_hiroba_aisatu_01_21; 
			WFLBY_ROOM_MSG_SetAisatsuGerMany( p_rmwk, 1, timezone );
			break;
		case LANG_SPAIN:	
			data_id = msg_hiroba_aisatu_01_22; 
			WFLBY_ROOM_MSG_SetAisatsuSpain( p_rmwk, 1, timezone );
			break;
		default:			
			data_id = msg_hiroba_aisatu_01_13; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		}
		break;

	// �������C�^���A
	case LANG_ITALY:
		WFLBY_ROOM_MSG_SetAisatsuItaly( p_rmwk, 0, timezone );
		switch( WFLBY_SYSTEM_GetProfileRagionCode( cp_youpr ) ){
		case LANG_JAPAN:	
			data_id = msg_hiroba_aisatu_01_07; 
			WFLBY_ROOM_MSG_SetAisatsuJapan( p_rmwk, 1, timezone );
			break;
		case LANG_ENGLISH:	
			data_id = msg_hiroba_aisatu_01_14; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		case LANG_FRANCE:	
			data_id = msg_hiroba_aisatu_01_20; 
			WFLBY_ROOM_MSG_SetAisatsuFrance( p_rmwk, 1, timezone );
			break;
		case LANG_ITALY:	
			data_id = msg_hiroba_aisatu_01_34; 
			WFLBY_ROOM_MSG_SetAisatsuItaly( p_rmwk, 1, timezone );
			break;
		case LANG_GERMANY:	
			data_id = msg_hiroba_aisatu_01_08; 
			WFLBY_ROOM_MSG_SetAisatsuGerMany( p_rmwk, 1, timezone );
			break;
		case LANG_SPAIN:	
			data_id = msg_hiroba_aisatu_01_10; 
			WFLBY_ROOM_MSG_SetAisatsuSpain( p_rmwk, 1, timezone );
			break;
		default:			
			data_id = msg_hiroba_aisatu_01_14; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		}
		break;

	// �������h�C�c
	case LANG_GERMANY:
		WFLBY_ROOM_MSG_SetAisatsuGerMany( p_rmwk, 0, timezone );
		switch( WFLBY_SYSTEM_GetProfileRagionCode( cp_youpr ) ){
		case LANG_JAPAN:	
			data_id = msg_hiroba_aisatu_01_09; 
			WFLBY_ROOM_MSG_SetAisatsuJapan( p_rmwk, 1, timezone );
			break;
		case LANG_ENGLISH:	
			data_id = msg_hiroba_aisatu_01_16; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		case LANG_FRANCE:	
			data_id = msg_hiroba_aisatu_01_23; 
			WFLBY_ROOM_MSG_SetAisatsuFrance( p_rmwk, 1, timezone );
			break;
		case LANG_ITALY:	
			data_id = msg_hiroba_aisatu_01_28; 
			WFLBY_ROOM_MSG_SetAisatsuItaly( p_rmwk, 1, timezone );
			break;
		case LANG_GERMANY:	
			data_id = msg_hiroba_aisatu_01_35; 
			WFLBY_ROOM_MSG_SetAisatsuGerMany( p_rmwk, 1, timezone );
			break;
		case LANG_SPAIN:	
			data_id = msg_hiroba_aisatu_01_31; 
			WFLBY_ROOM_MSG_SetAisatsuSpain( p_rmwk, 1, timezone );
			break;
		default:			
			data_id = msg_hiroba_aisatu_01_16; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		}
		break;

	// �������X�y�C��
	case LANG_SPAIN:	
		WFLBY_ROOM_MSG_SetAisatsuSpain( p_rmwk, 0, timezone );
		switch( WFLBY_SYSTEM_GetProfileRagionCode( cp_youpr ) ){
		case LANG_JAPAN:	
			data_id = msg_hiroba_aisatu_01_11; 
			WFLBY_ROOM_MSG_SetAisatsuJapan( p_rmwk, 1, timezone );
			break;
		case LANG_ENGLISH:	
			data_id = msg_hiroba_aisatu_01_18; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		case LANG_FRANCE:	
			data_id = msg_hiroba_aisatu_01_26; 
			WFLBY_ROOM_MSG_SetAisatsuFrance( p_rmwk, 1, timezone );
			break;
		case LANG_ITALY:	
			data_id = msg_hiroba_aisatu_01_30; 
			WFLBY_ROOM_MSG_SetAisatsuItaly( p_rmwk, 1, timezone );
			break;
		case LANG_GERMANY:	
			data_id = msg_hiroba_aisatu_01_31; 
			WFLBY_ROOM_MSG_SetAisatsuGerMany( p_rmwk, 1, timezone );
			break;
		case LANG_SPAIN:	
			data_id = msg_hiroba_aisatu_01_36; 
			WFLBY_ROOM_MSG_SetAisatsuSpain( p_rmwk, 1, timezone );
			break;
		default:			
			data_id = msg_hiroba_aisatu_01_18; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		}
		break;
	
	// �������p��
	case LANG_ENGLISH:
	// �������s���ȂƂ��͉p��ŉ�b
	default:
		WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 0, timezone );
		switch( WFLBY_SYSTEM_GetProfileRagionCode( cp_youpr ) ){
		case LANG_JAPAN:	
			data_id = msg_hiroba_aisatu_01_03; 
			WFLBY_ROOM_MSG_SetAisatsuJapan( p_rmwk, 1, timezone );
			break;
		case LANG_ENGLISH:	
			data_id = msg_hiroba_aisatu_01_32; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		case LANG_FRANCE:	
			data_id = msg_hiroba_aisatu_01_13; 
			WFLBY_ROOM_MSG_SetAisatsuFrance( p_rmwk, 1, timezone );
			break;
		case LANG_ITALY:	
			data_id = msg_hiroba_aisatu_01_14; 
			WFLBY_ROOM_MSG_SetAisatsuItaly( p_rmwk, 1, timezone );
			break;
		case LANG_GERMANY:	
			data_id = msg_hiroba_aisatu_01_15; 
			WFLBY_ROOM_MSG_SetAisatsuGerMany( p_rmwk, 1, timezone );
			break;
		case LANG_SPAIN:	
			data_id = msg_hiroba_aisatu_01_17; 
			WFLBY_ROOM_MSG_SetAisatsuSpain( p_rmwk, 1, timezone );
			break;
		default:			
			data_id = msg_hiroba_aisatu_01_32; 
			WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, 1, timezone );
			break;
		}
		break;

	}
//*/


	WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, data_id ) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���]�[���̈��A�����[�h�Z�b�g�ɐݒ�
 *
 *	@param	p_rmwk			�������[�N
 *	@param	buffno			���[�h�Z�b�g�o�b�t�@�i���o�[
 *	@param	timezone		�^�C���]�[��
 *	@param	cp_pr			�v���t�B�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_Aisatu_SetWordSet( WFLBY_ROOMWK* p_rmwk, u32 buffno, u32 timezone, const WFLBY_USER_PROFILE* cp_pr )
{
	switch( WFLBY_SYSTEM_GetProfileRagionCode( cp_pr ) ){
	case LANG_JAPAN:	
		WFLBY_ROOM_MSG_SetAisatsuJapan( p_rmwk, buffno, timezone );
		break;
	case LANG_ENGLISH:	
		WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, buffno, timezone );
		break;
	case LANG_FRANCE:	
		WFLBY_ROOM_MSG_SetAisatsuFrance( p_rmwk, buffno, timezone );
		break;
	case LANG_ITALY:	
		WFLBY_ROOM_MSG_SetAisatsuItaly( p_rmwk, buffno, timezone );
		break;
	case LANG_GERMANY:	
		WFLBY_ROOM_MSG_SetAisatsuGerMany( p_rmwk, buffno, timezone );
		break;
	case LANG_SPAIN:	
		WFLBY_ROOM_MSG_SetAisatsuSpain( p_rmwk, buffno, timezone );
		break;
	default:			
		WFLBY_ROOM_MSG_SetAisatsuEnglish( p_rmwk, buffno, timezone );
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���A�t�H���[�̕\��
 *	
 *	@param	p_evwk		���[�N
 *	@param	p_system	�V�X�e�����[�N
 *	@param	p_rmwk		�������[�N
 *
 *	@retval	TRUE	�t�H���[��\������
 *	@retval	FALSE	�t�H���[����Ȃ�����
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_TALK_WK_AisatuFollowDraw( WFLBY_EV_TALK_WK* p_evwk, WFLBY_SYSTEM* p_system, WFLBY_ROOMWK* p_rmwk )
{
	u32 data_id;
	const WFLBY_USER_PROFILE* cp_mypr;
	const WFLBY_USER_PROFILE* cp_youpr;

	cp_mypr		= WFLBY_SYSTEM_GetMyProfileLocal( p_system );
	cp_youpr	= WFLBY_SYSTEM_GetUserProfile( p_system, p_evwk->talk_idx );
	
	// ����������̌��ꂪ�s���łȂ���
	if( WFLBY_SYSTEM_CheckProfileRagionCode( cp_mypr ) == FALSE ){
		return FALSE;
	}
	if( WFLBY_SYSTEM_CheckProfileRagionCode( cp_youpr ) == FALSE ){
		return FALSE;
	}
	
	// ���ԑт��Ⴄ���`�F�b�N
	if( p_evwk->my_timezone != p_evwk->you_timezone ){

		// ����\�����邩
		switch( p_evwk->you_timezone ){
		case WFLBY_AISATSU_MORNING:
			data_id = msg_hiroba_aisatu_02_03;
			break;
			
		case WFLBY_AISATSU_NOON:
			data_id = msg_hiroba_aisatu_02_01;
			break;
			
		case WFLBY_AISATSU_NIGHT:
			data_id = msg_hiroba_aisatu_02_02;
			break;
		}
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, data_id ) );
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�ɃA�C�e������ǉ�
 *
 *	@param	p_evwk		���[�N
 *	@param	p_system	�V�X�e�����[�N
 *	@param	p_rmwk		�������[�N
 *	@param	bufid		�o�b�t�@ID
 *	@param	my			�����̃f�[�^�H
 *	
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_WordsetSetItem( WFLBY_EV_TALK_WK* p_evwk, WFLBY_SYSTEM* p_system, WFLBY_ROOMWK* p_rmwk, u32 bufid, BOOL my )
{
	u32 item;
	const WFLBY_USER_PROFILE* cp_mypr;
	const WFLBY_USER_PROFILE* cp_youpr;

	cp_mypr		= WFLBY_SYSTEM_GetMyProfileLocal( p_system );
	cp_youpr	= WFLBY_SYSTEM_GetUserProfile( p_system, p_evwk->talk_idx );

	
	if( my==FALSE ){
		item = WFLBY_SYSTEM_GetProfileItem( cp_youpr );
	}else{
		item = WFLBY_SYSTEM_GetProfileItem( cp_mypr );
	}

	WFLBY_ROOM_MSG_SetItem( p_rmwk, item, bufid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���E�G�C�g�@�̒��Ԃ܂Ŏ��Ԃ�������A���ԃ��b�Z�[�W��\������
 *
 *	@param	p_evwk		�C�x���g���[�N
 *	@param	p_rmwk		�������[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_PrintTimeWaitMsg( WFLBY_EV_TALK_WK* p_evwk, WFLBY_ROOMWK* p_rmwk )
{
	WFLBY_SYSTEM* p_system;
	STRBUF* p_str;

	p_system = WFLBY_ROOM_GetSystemData( p_rmwk );
	
	if( WFLBY_SYSTEM_GetRecvWaitTime( p_system ) == WFLBY_TALK_RECV_HALF_WAIT ){
		WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
		p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_07_02 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		WFLBY_ROOM_TALKWIN_StartTimeWait( p_rmwk );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I��҂����ԃJ�E���^�@�J�n
 *
 *	@param	p_evwk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_StartSelectWait( WFLBY_EV_TALK_WK* p_evwk )
{
	p_evwk->sel_wait		= 0;
	p_evwk->sel_wait_flag	= TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I��҂����ԃJ�E���^�@��~
 *
 *	@param	p_evwk 
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_StopSelectWait( WFLBY_EV_TALK_WK* p_evwk )
{
	p_evwk->sel_wait		= 0;
	p_evwk->sel_wait_flag	= FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I��҂��J�E���^�E�G�C�g����
 *
 *	@param	p_evwk	�C�x���g���[�N
 *	@param	p_rmwk	�������[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_CheckAndPrintSelectWait( WFLBY_EV_TALK_WK* p_evwk, WFLBY_ROOMWK* p_rmwk )
{
	STRBUF* p_str;

	// �I��҂���ԂŁA���Ԃ�WFLBY_TALK_RECV_HALF_WAIT�ɂ������烁�b�Z�[�W�𗬂�
	if( p_evwk->sel_wait_flag == TRUE ){

		if( p_evwk->sel_wait == WFLBY_TALK_RECV_HALF_WAIT ){
			WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, p_evwk->talk_idx, 0 );
			p_str = WFLBY_EV_TALK_GetStrBuf( p_rmwk, p_evwk->talk_idx, msg_hiroba_osyaberi_07_01 );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I��҂��J�E���^�E�G�C�g����	���C��
 *
 *	@param	p_evwk		�C�x���g���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_MainSelectWait( WFLBY_EV_TALK_WK* p_evwk )
{
	if( p_evwk->sel_wait_flag == TRUE ){
		if( p_evwk->sel_wait <= WFLBY_TALK_RECV_WAIT ){
			p_evwk->sel_wait ++;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����f�[�^�ݒ�
 *
 *	@param	p_evwk		���[�N
 *	@param	cp_youpr	����̃v���t�B�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_SetRirekiData( WFLBY_EV_TALK_WK* p_evwk, const WFLBY_USER_PROFILE* cp_youpr )
{
	u32 lastact;
	lastact = WFLBY_SYSTEM_GetProfileTrTypetoLastActionNo( cp_youpr );
	p_evwk->rireki_flag		= TRUE;
	p_evwk->rireki_actno	= lastact;
	p_evwk->rireki_userid	= WFLBY_SYSTEM_GetProfileUserID( cp_youpr );
}




//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v���X�g�f�[�^�쐬
 *
 *	@param	p_evwk		���[�N
 *	@param	p_rmwk		�������[�N
 *	@param	num			���X�g��
 *	@param	msg_start	���b�Z�[�W�J�n�ʒu
 *	@param	last_sel_b	�I��B�ŏI�s��ǉ����邩�H
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_InitBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk, u32 num, u32 msg_start, u32 last_sel_b )
{
	int i;
	const STRBUF* cp_str;

	// �o�b�t�@�쐬
	WFLBY_ROOM_LISTWIN_CreateBmpList( p_rmwk, num );
	p_wk->bmplist_num = num;

	// �f�[�^�쐬
	for( i=0; i<num; i++ ){
		// �ŏI�s�`�F�b�N
		if( (last_sel_b != WFLBY_EV_TALK_LIST_END_NONE) &&
			(i == (num-1)) ){
			if( last_sel_b == WFLBY_EV_TALK_LIST_END_YAMERU ){
				cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_menu_00_03 );
			}else{
				cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_03_41 );
			}
		}else{
			cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_start+i );
		}
		WFLBY_ROOM_LISTWIN_SetBmpListStr( p_rmwk, cp_str, i );
	}

	// �r�b�g�}�b�v���X�g�w�b�_�[�ɐݒ�
	p_wk->bmplist			= sc_WFLBY_EV_TALK_HEADER;
	p_wk->bmplist.count	= p_wk->bmplist_num;
	if( p_wk->bmplist.line > p_wk->bmplist_num ){
		p_wk->bmplist.line = p_wk->bmplist_num;
	}
	p_wk->bmplist.list	= WFLBY_ROOM_LISTWIN_GetBmpList( p_rmwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief
 *
 *	@param	p_evwk		���[�N
 *	@param	p_rmwk		�������[�N
 *	@param	allnum		���b�Z�[�W���ׂĂ̐�
 *	@param	sel_num		�I�����鐔(last_sel_b��NONE����Ȃ��Ȃ�ǉ����鍀�ڂ��܂߂��l)
 *	@param	msg_start	���b�Z�[�W�J�n�ʒu
 *	@param	last_sel_b	�I��B�ŏI�s��ǉ����邩�H
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_InitBmpListRandom( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk, u32 allnum, u32 sel_num, u32 msg_start, u32 last_sel_b )
{
	int i;
	u32 sel_msg;
	u32 msg_param;
	const STRBUF* cp_str;
	BOOL result;
	

	// �o�b�t�@�쐬
	WFLBY_ROOM_LISTWIN_CreateBmpList( p_rmwk, sel_num );
	p_wk->bmplist_num = sel_num;

	// �f�[�^�쐬
	for( i=0; i<sel_num; i++ ){
		// �ŏI�s�`�F�b�N
		if( (last_sel_b != WFLBY_EV_TALK_LIST_END_NONE) &&
			(i == (sel_num-1)) ){

			msg_param = allnum;
			if( last_sel_b == WFLBY_EV_TALK_LIST_END_YAMERU ){
				cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_menu_00_03 );
			}else{
				cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_hiroba_osyaberi_03_41 );
			}
		}else{


#ifdef WFLBY_EV_TALK_DEBUG_SEL_MSG
			// �f�o�b�N��
			if( s_WFLBY_EV_TALK_DEBUG_SEL_MSG_ON == TRUE ){
				sel_msg		= i+(4*s_WFLBY_EV_TALK_DEBUG_SEL_MSG_PAGE);
				sel_msg		= sel_msg % allnum;
				msg_param	= sel_msg;

				cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_start+sel_msg );

			}else{

				// �ʏ�
				// 
				// �d�����Ȃ����ڂ��ł�܂�
				do{
					sel_msg		= gf_mtRand() % allnum;
					msg_param	= sel_msg;
					result		= WFLBY_ROOM_LISTWIN_CheckBmpListParam( p_rmwk, msg_param );
				}while( result == TRUE );
				cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_start+sel_msg );

			}
#else
			// �ʏ�
			// 
			// �d�����Ȃ����ڂ��ł�܂�
			do{
				sel_msg		= gf_mtRand() % allnum;
				msg_param	= sel_msg;
				result		= WFLBY_ROOM_LISTWIN_CheckBmpListParam( p_rmwk, msg_param );
			}while( result == TRUE );
			cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_AISATSU, msg_start+sel_msg );
#endif

			
		}
		WFLBY_ROOM_LISTWIN_SetBmpListStr( p_rmwk, cp_str, msg_param );
	}

	// �r�b�g�}�b�v���X�g�w�b�_�[�ɐݒ�
	p_wk->bmplist			= sc_WFLBY_EV_TALK_HEADER;
	p_wk->bmplist.count	= p_wk->bmplist_num;
	if( p_wk->bmplist.line > p_wk->bmplist_num ){
		p_wk->bmplist.line = p_wk->bmplist_num;
	}
	p_wk->bmplist.list	= WFLBY_ROOM_LISTWIN_GetBmpList( p_rmwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�O���o����p�r�b�g�}�b�v���X�g
 *
 *	@param	p_wk		���[�N
 */	
//-----------------------------------------------------------------------------
static void WFLBY_EV_TOPIC_WK_InitBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk )
{
	int i;
	STRBUF* p_str;
	static const u8 sc_LISTMSG[ WFLBY_EV_TOPIC_LIST_SEL_NUM ] = {
		msg_hiroba_infowin_01,
		msg_hiroba_infowin_02,
		msg_hiroba_infowin_03,
		msg_hiroba_infowin_04,
		msg_hiroba_infowin_05,
		msg_hiroba_infowin_10,
		msg_hiroba_infowin_06,
		msg_hiroba_infowin_07,
	};

	// �o�b�t�@�쐬
	WFLBY_ROOM_LISTWIN_CreateBmpList( p_rmwk, WFLBY_EV_TOPIC_LIST_SEL_NUM );
	p_wk->bmplist_num = WFLBY_EV_TOPIC_LIST_SEL_NUM;

	// �f�[�^�쐬
	for( i=0; i<WFLBY_EV_TOPIC_LIST_SEL_NUM; i++ ){
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, sc_LISTMSG[i] );
		WFLBY_ROOM_LISTWIN_SetBmpListStr( p_rmwk, p_str, i );
	}

	// �r�b�g�}�b�v���X�g�w�b�_�[�ɐݒ�
	p_wk->bmplist			= sc_WFLBY_EV_TALK_HEADER;
	p_wk->bmplist.count		= p_wk->bmplist_num;
	p_wk->bmplist.line		= p_wk->bmplist_num;
	p_wk->bmplist.list		= WFLBY_ROOM_LISTWIN_GetBmpList( p_rmwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�O���o����p�C�x���g�I�����X�g
 *
 *	@param	p_wk	���[�N
 *	@param	p_rmwk	�������[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TOPIC_WK_InitEventBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk )
{
	int i;
	STRBUF* p_str;

	// �o�b�t�@�쐬
	WFLBY_ROOM_LISTWIN_CreateBmpList( p_rmwk, WFLBY_EV_TOPIC_LIST_SEL_EVE_NUM );
	p_wk->bmplist_num = WFLBY_EV_TOPIC_LIST_SEL_EVE_NUM;

	// �f�[�^�쐬
	for( i=0; i<WFLBY_EV_TOPIC_LIST_SEL_EVE_NUM-1; i++ ){
		WFLBY_ROOM_MSG_SetTimeEvent( p_rmwk, WFLBY_EVENT_GMM_NEON+i, 0 );
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infowin_09 );
		WFLBY_ROOM_LISTWIN_SetBmpListStr( p_rmwk, p_str, i );
	}

	// ��߂��ǉ�
	p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infowin_07 );
	WFLBY_ROOM_LISTWIN_SetBmpListStr( p_rmwk, p_str, WFLBY_EV_TOPIC_LIST_SEL_EVE_NUM-1 );
	

	// �r�b�g�}�b�v���X�g�w�b�_�[�ɐݒ�
	p_wk->bmplist			= sc_WFLBY_EV_TALK_HEADER;
	p_wk->bmplist.count		= p_wk->bmplist_num;
	p_wk->bmplist.line		= p_wk->bmplist_num;
	p_wk->bmplist.list		= WFLBY_ROOM_LISTWIN_GetBmpList( p_rmwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�O���o����p�C�x���g�I�����X�g
 *
 *	@param	p_wk	���[�N
 *	@param	p_rmwk	�������[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TOPIC_WK_InitMinigameBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk )
{
	int i;
	STRBUF* p_str;

	// �o�b�t�@�쐬
	WFLBY_ROOM_LISTWIN_CreateBmpList( p_rmwk, WFLBY_EV_TOPIC_LIST_SEL_MG_NUM );
	p_wk->bmplist_num = WFLBY_EV_TOPIC_LIST_SEL_MG_NUM;

	// �f�[�^�쐬
	for( i=0; i<WFLBY_EV_TOPIC_LIST_SEL_MG_NUM-1; i++ ){
		WFLBY_ROOM_MSG_SetMinigame( p_rmwk, WFLBY_GAME_BALLSLOW+i, 0 );
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infowin_08 );
		WFLBY_ROOM_LISTWIN_SetBmpListStr( p_rmwk, p_str, i );
	}

	// ��߂��ǉ�
	p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infowin_07 );
	WFLBY_ROOM_LISTWIN_SetBmpListStr( p_rmwk, p_str, WFLBY_EV_TOPIC_LIST_SEL_MG_NUM-1 );

	// �r�b�g�}�b�v���X�g�w�b�_�[�ɐݒ�
	p_wk->bmplist			= sc_WFLBY_EV_TALK_HEADER;
	p_wk->bmplist.count		= p_wk->bmplist_num;
	p_wk->bmplist.line		= p_wk->bmplist_num;
	p_wk->bmplist.list		= WFLBY_ROOM_LISTWIN_GetBmpList( p_rmwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v���X�g�f�[�^��j��
 *
 *	@param	p_evwk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_WK_ExitBmpList( WFLBY_EV_TALK_BMPLIST* p_wk, WFLBY_ROOMWK* p_rmwk )
{
	WFLBY_ROOM_LISTWIN_DeleteBmpList( p_rmwk );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�_�~�[���b�Z�[�W��M��@�f�[�^���M����V�X�e��	������
 *	
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_RWSEND_Init( WFLBY_EV_TALK_RWSEND* p_wk )
{
	p_wk->trans_on = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�_�~�[���b�Z�[�W��M��@�f�[�^���M����V�X�e��	�J�n
 *
 *	@param	p_wk			���[�N
 *	@param	cp_system		�V�X�e�����[�N
 *	@param	send_data		���M�f�[�^
 *	@param	wait_seq		��M�҂����Ă��b�V�[�P���X
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_RWSEND_Start( WFLBY_EV_TALK_RWSEND* p_wk, const WFLBY_SYSTEM* cp_system, u16 send_data, u16 wait_seq )
{
	GF_ASSERT( p_wk->trans_on == FALSE );
	p_wk->trans_on		= TRUE;
	p_wk->trans_data	= send_data;
	p_wk->wait_seq		= wait_seq;
	p_wk->start_seq		= WFLBY_SYSTEM_GetTalkSeq( cp_system );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�_�~�[���b�Z�[�W��M��@�f�[�^���M����V�X�e��	���C���֐�
 *
 *	@param	p_wk		���[�N
 *	@param	p_system	���r�[�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_TALK_RWSEND_Main( WFLBY_EV_TALK_RWSEND* p_wk, WFLBY_SYSTEM* p_system )
{
	u32 talk_seq;

	// ���쒆���`�F�b�N
	if( p_wk->trans_on == FALSE ){
		return;
	}

	// ��b�V�[�P���X�擾
	talk_seq	= WFLBY_SYSTEM_GetTalkSeq( p_system );		// ��b�V�[�P���X�擾

	// ��b�V�[�P���X�ۑ�
	p_wk->recv_talk_seq = talk_seq;

	// �҂��Ă���V�[�P���X�ɂȂ�����f�[�^���M
	if( talk_seq == p_wk->wait_seq ){
		// �f�[�^�]��
		WFLBY_SYSTEM_SendTalkData( p_system, p_wk->trans_data );
		p_wk->trans_on = FALSE;
	}
	// �V�[�P���X���ς�����炨�������A���M�����ɏI��
	else if( talk_seq != p_wk->start_seq ){
		p_wk->trans_on = FALSE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�_�~�[���b�Z�[�W��M��@�f�[�^���M����V�X�e��	�I���`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_TALK_RWSEND_CheckEnd( const WFLBY_EV_TALK_RWSEND* cp_wk )
{
	if( cp_wk->trans_on == TRUE ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�_�~�[���b�Z�[�W��M��@�f�[�^���M����V�X�e��	��M������b�V�[�P���X��Ԃ�
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	��b�V�[�P���X
 */
//-----------------------------------------------------------------------------
static u16 WFLBY_EV_TALK_RWSEND_GetTalkSeq( const WFLBY_EV_TALK_RWSEND* cp_wk )
{
	return cp_wk->recv_talk_seq;
}







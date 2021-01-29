//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_ev_def.c
 *	@brief		�풓�C�x���g		�e�L�����N�^�̏풓�C�x���g
 *	@author		tomoya takahashi
 *	@data		2007.11.26
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "msgdata/msg_wifi_h_info.h"
#include "msgdata/msg_wifi_hiroba.h"
#include "msgdata/msg_wifi_system.h"

#include "system/fontproc.h"

#include "wflby_ev_def.h"
#include "wflby_ev_talk.h"
#include "wflby_ev_mg.h"
#include "wflby_ev_anketo.h"
#include "wflby_3dmatrix.h"
#include "wflby_snd.h"

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
#define WFLBY_DEBUG_EV_DEF_GADGET_GET	// �I�������K�W�F�b�g�����炦��悤�ɂ���
//#define WFLBY_DEBUG_EV_DEF_FLOAT_RETRY	// �t���[�g�ގ����Ă�������x�߂��Ă���
//#define WFLBY_DEBUG_EV_DEF_ANKETO	// �A���P�[�g���f�������{�^���Ɋ���U��
#endif

#ifdef WFLBY_DEBUG_EV_DEF_GADGET_GET
extern BOOL D_Tomoya_WiFiLobby_DebugStart;	// �f�o�b�N�J�n���
#endif



//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�C�x���g�ɂ���ċN������C�x���g�f�[�^�p
//=====================================
// �C�x���g�^�C�v
enum {
	WFLBY_EV_DEF_EVTYPE_DEF,	// �풓�C�x���g
	WFLBY_EV_DEF_EVTYPE_INTR,	// ���荞�݃C�x���g
	WFLBY_EV_DEF_EVTYPE_NUM,	// �C�x���g�^�C�v��
	WFLBY_EV_DEF_EVTYPE_END,	// ���̃f�[�^���I�[�f�[�^
} ;
// �N���C�x���gNo�ő吔
#define WFLBY_EV_DEF_EVNO_MAX	( 8 )
// �N���C�x���gNo�s���l
#define WFLBY_EV_DEF_EVNO_END	( 0xff )



//-------------------------------------
///	�S���ʁ@�K�W�F�b�g�C�x���g
//=====================================
enum {
	WFLBY_EV_DEF_ALL_GADGET_SEQ_INIT,
	WFLBY_EV_DEF_ALL_GADGET_SEQ_WAIT,
	WFLBY_EV_DEF_ALL_GADGET_SEQ_END,
} ;




//-------------------------------------
///	�v���C���[�ʏ����萔
//=====================================
// �ʏ�
enum{
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_ADD,
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_FADEWAIT,
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVESTARTWAIT,
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE00,	// �O��
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE01,	// �O��
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE02,	// �^�[��
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE03,	// ����
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVEWAIT,	// ���슮���҂�
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG00,		// �h�悤�����h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG01,		// �h�P�߂̃^�C�v�������łˁh
//	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG02,		// �h�I��\���i�P�߁j�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG03,		// �h�I���i�P�߁j�h
//	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG04,		// �h����ł悢�ł����H�i�P�߁jo�h	����Ȃ�
//	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG05,		// �hYESNO�\���i�P�߁j�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG06,		// �hYESNO�i�P�߁j�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG07,		// �h�Q�߂̃^�C�v�������łˁh
//	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG08,		// �h�I��\���i�Q�߁j�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG09,		// �h�I���i�Q�߁j�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG10,		// �h����ł悢�ł����H�i�Q�߁j�h
//	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG11,		// �hYESNO�\���i�Q�߁j�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG12,		// �hYESNO�i�Q�߁j�h
//	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG13,		// �h�v���t�B�[������������h
//	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG14,		// �h�^�b�`�g�C����������I�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG15,		// �h�^�b�`�g�C����������I�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG16,		// �h�������܂Ł@���̂���łˁI�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG17,		// �h��낵����΁@�^�C�v�I���h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG18,		// �h�P���I�������Ƃ��́@���o����̂����t�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG19,		// �h�Q���I�������Ƃ��́@���o����̂����t�h
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT,	// ���b�Z�[�W�I���҂�
	WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_END,
};
#define WFLBY_EV_DEF_PLAYERIN_NORMAL_MOVEWAIT			( 16 )
static const BMPLIST_HEADER sc_WFLBY_EV_DEF_WAZATYPE_HEADER = {
	NULL,
	NULL,
	NULL,
	NULL,

	WFLBY_POKEWAZA_TYPE_NUM,		//���X�g���ڐ�
	7,								//�\���ő區�ڐ�
	0,								//���x���\���w���W
	8,								//���ڕ\���w���W
	0,								//�J�[�\���\���w���W
	0,								//�\���x���W
	FBMP_COL_BLACK,					//�\�������F
	FBMP_COL_WHITE,					//�\���w�i�F
	FBMP_COL_BLK_SDW,				//�\�������e�F
	0,								//�����Ԋu�w
	16,								//�����Ԋu�x
	BMPLIST_LRKEY_SKIP,				//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,					//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0,								//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)

	NULL
};

// �w��O���b�h�ʒu�ɏo��
enum{
	WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_ADD,
	WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_FADEWAIT,
	WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_PLMOVE,
	WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_APLFORCEEND_MSG,
	WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_APLFORCEEND_MSGWAIT,
	WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_END,
};
// �~�j�Q�[����ɏo��
enum{
	WFLBY_EV_DEF_PLAYERIN_MG_SEQ_ADD,
	WFLBY_EV_DEF_PLAYERIN_MG_SEQ_FADEWAIT,
	WFLBY_EV_DEF_PLAYERIN_MG_SEQ_PLMOVE,
	WFLBY_EV_DEF_PLAYERIN_MG_SEQ_MSG,
	WFLBY_EV_DEF_PLAYERIN_MG_SEQ_MSGEND,
	WFLBY_EV_DEF_PLAYERIN_MG_SEQ_END,
};
// �w��O���b�h�ʒu�ɏo���A���P�[�g��
enum{
	WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_ADD,
	WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_FADEWAIT,
	WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_PLMOVE,
	WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_APLFORCEEND_MSG,
	WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_APLFORCEEND_MSGWAIT,
	WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_END,
};

//-------------------------------------
///	�v���C���[�풓����萔
//=====================================
enum{
	WFLBY_EV_DEF_PLAYER_SEQ_INIT,
	WFLBY_EV_DEF_PLAYER_SEQ_EVCHECK,
};

//-------------------------------------
///	�v���C���[�ʏ�ގ��V�[�P���X
//=====================================
enum {
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_MSGSTART,			// �ގ��`�F�b�N���b�Z�[�W�J�n
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_MSGWAIT,			// �ގ��`�F�b�N���b�Z�[�W�҂�
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_MSGSTART01,		// �ގ��`�F�b�N���b�Z�[�W�J�n
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_MSGWAIT01,		// �ގ��`�F�b�N���b�Z�[�W�҂�
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_YESNO,			// YESNO�E�B���h�E�\��
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_YESNOWAIT,		// YESNO�I��҂�
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_FLYUP,			// �ގ����
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_END,				// �ގ�
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_RET,				// �߂�
	WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_RETWAIT,			// �߂芮���܂�
};

//-------------------------------------
///	���E���v�ւ̃V�[�P���X
//=====================================
enum {
	WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_LOGIN,
	WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_LOGINWAIT,
	WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_START,
	WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NG,
	WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NGWAIT,
	WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_CLOSE,
	WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_CLOSEWAIT,
	WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NGMSG,
	WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NGMSGWAIT,
} ;
#define WFLBY_EV_DEF_PLAYER_SUBCHAN_MSG_WAIT	( 8 )

//-------------------------------------
///	�~�j�Q�[���ւ̃V�[�P���X
//=====================================
enum {
	WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_START,
	WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_WAIT,
	WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_END,
	WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_RET,
	WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_RETWAIT,
	WFLBY_EV_DEF_PLAYER_MG_SEQ_MSG,
	WFLBY_EV_DEF_PLAYER_MG_SEQ_MSGWAIT,
	WFLBY_EV_DEF_PLAYER_MG_SEQ_RET_NORMAL,
} ;

//-------------------------------------
///	�A���P�[�g�ւ̃V�[�P���X
//=====================================
enum {
	WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_START,
	WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_START_ANKETO,
	WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_MOVE,
	WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_WAIT,
	WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_MSG,
	WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_MSGWAIT,
	WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_NORMAL,
} ;

//-------------------------------------
///	�v���C���[�t���[�g
//=====================================
enum{
	WFLBY_EV_FLOAT_SEQ_INIT,
	WFLBY_EV_FLOAT_SEQ_FLOAT_CHECK,		// �\�񕔕�
	WFLBY_EV_FLOAT_SEQ_FLOAT_INWAIT,	// ����܂ő҂�
	WFLBY_EV_FLOAT_SEQ_FLOAT_JUMP,		// �W�����v
	WFLBY_EV_FLOAT_SEQ_FLOAT_JUMPON,	// ���n�A�j��
	WFLBY_EV_FLOAT_SEQ_FLOAT_MAIN,		// ���C��	�{�^����������
	WFLBY_EV_FLOAT_SEQ_FLOAT_END_DOWN,	// ���Ȃ��̂łP��������
	WFLBY_EV_FLOAT_SEQ_FLOAT_END_DOWNWAIT,// ���Ȃ��̂łP��������
	WFLBY_EV_FLOAT_SEQ_FLOAT_END_BEFORE,// �t���[�g�O�Ȃ̂ŏ��Ȃ����b�Z�[�W
	WFLBY_EV_FLOAT_SEQ_FLOAT_END_FULL,	// �t���[�g�������ς��Ȃ̂ŏ��Ȃ�
	WFLBY_EV_FLOAT_SEQ_FLOAT_END_AFTER,	// �p���[�h��Ȃ̂ŏ��Ȃ����b�Z�[�W
	WFLBY_EV_FLOAT_SEQ_FLOAT_MSGWAIT,	// ���b�Z�[�W�҂�
	WFLBY_EV_FLOAT_SEQ_FLOAT_WINOFFWAIT,// �E�B���h�E�n�e�e�҂�
	WFLBY_EV_FLOAT_SEQ_END,				//  �I��
	WFLBY_EV_FLOAT_SEQ_ENDWAIT,			//  �I���҂�
	WFLBY_EV_FLOAT_SEQ_RET,				//  �I��
};
//�W�����v
#define WFLBY_EV_FLOAT_JUMP_ANM_SYNC	( 24 )	// �W�����v�V���N��
#define WFLBY_EV_FLOAT_JUMP_ANM_SYNC_ZS	( 4 )	// Z�̒l��ݒ肷��J�n�V���N��
#define WFLBY_EV_FLOAT_JUMP_ANM_SYNC_Z	( 20 )	// �W�����v�V�[�P���XZ�p�V���N��
#define WFLBY_EV_FLOAT_JUMP_CHECK_X		( FX32_CONST( 16 ) )	// ��я��J�n�t���[�gX�I�t�Z�b�g
#define WFLBY_EV_FLOAT_JUMP_MOVE_Y		( FX32_CONST( 16 ) )	// ��ԍ���
// ���n
#define WFLBY_EV_FLOAT_JUMPON_ANM_SYNC	( 16 )	// ���n�A�j��
#define WFLBY_EV_FLOAT_JUMPON_ANM_ONE	( 8 )	// �㉺����1��Ɏg�p����V���N��
#define WFLBY_EV_FLOAT_JUMPON_ANM_MOVEY	( FX32_CONST(3) )	// ��������

#define WFLBY_EV_FLOAT_CAMERA_MOVE_END	( FX32_CONST( 96 ) )	// �J�����X�N���[���̏I���n�_


//-------------------------------------
///	NPC����V�[�P���X
//=====================================
enum{
	WFLBY_EV_DEF_NPCWAIT_SEQ_INIT,
	WFLBY_EV_DEF_NPCWAIT_SEQ_WAIT,
	WFLBY_EV_DEF_NPCWAIT_SEQ_INWAIT,
	WFLBY_EV_DEF_NPCWAIT_SEQ_STATUS_CHECK,
	WFLBY_EV_DEF_NPCWAIT_SEQ_FLYDOWN,
	WFLBY_EV_DEF_NPCWAIT_SEQ_END,
};

//-------------------------------------
///	NPC�ޏ�V�[�P���X
//=====================================
enum{
	WFLBY_EV_DEF_NPCEND_SEQ_OUT,
	WFLBY_EV_DEF_NPCEND_SEQ_OUTWAIT,
	WFLBY_EV_DEF_NPCEND_SEQ_END,
};

//-------------------------------------
///	NPC�ʏ�V�[�P���X
//=====================================
enum{
	WFLBY_EV_DEF_NPCMAIN_SEQ_INIT,
	WFLBY_EV_DEF_NPCMAIN_SEQ_MAIN,		// ���C��
	WFLBY_EV_DEF_NPCMAIN_SEQ_SYNCWAIT,
};

//-------------------------------------
///	�I�u�W�FID���X�g
//=====================================
// �I�u�W�FID�̐�
#define WFLBY_EV_DEF_NPC_OBJID_MINIGAME_NUM	( 4 )	// �~�j�Q�[��
#define WFLBY_EV_DEF_NPC_OBJID_TOPIC_NUM	( 4 )	// �g�s�b�N
#define WFLBY_EV_DEF_NPC_OBJID_WLDTIMER_NUM	( 4 )	// ���E���v
#define WFLBY_EV_DEF_NPC_OBJID_FOOT_NUM		( 8 )	// ���Ճ{�[�h
	
// �{�[������
static const u32 sc_WFLBY_EV_DEF_NPC_OBJID_BS[ WFLBY_EV_DEF_NPC_OBJID_MINIGAME_NUM ]={
	WFLBY_MAPOBJID_NPCMG00MAT00,
	WFLBY_MAPOBJID_NPCMG00MAT01,
	WFLBY_MAPOBJID_NPCMG00MAT02,
	WFLBY_MAPOBJID_NPCMG00MAT03,
};
// �{�[���̂�
static const u32 sc_WFLBY_EV_DEF_NPC_OBJID_BB[ WFLBY_EV_DEF_NPC_OBJID_MINIGAME_NUM ]={
	WFLBY_MAPOBJID_NPCMG01MAT00,
	WFLBY_MAPOBJID_NPCMG01MAT01,
	WFLBY_MAPOBJID_NPCMG01MAT02,
	WFLBY_MAPOBJID_NPCMG01MAT03,
};
// ���D����
static const u32 sc_WFLBY_EV_DEF_NPC_OBJID_BL[ WFLBY_EV_DEF_NPC_OBJID_MINIGAME_NUM ]={
	WFLBY_MAPOBJID_NPCMG02MAT00,
	WFLBY_MAPOBJID_NPCMG02MAT01,
	WFLBY_MAPOBJID_NPCMG02MAT02,
	WFLBY_MAPOBJID_NPCMG02MAT03,
};
// �g�s�b�N
static const u32 sc_WFLBY_EV_DEF_NPC_OBJID_TP[ WFLBY_EV_DEF_NPC_OBJID_TOPIC_NUM ] = {
	WFLBY_MAPOBJID_TOPIC_00,
	WFLBY_MAPOBJID_TOPIC_01,
	WFLBY_MAPOBJID_TOPIC_02,
	WFLBY_MAPOBJID_TOPIC_03,
};
// ���E���v
static const u32 sc_WFLBY_EV_DEF_NPC_OBJID_WT[ WFLBY_EV_DEF_NPC_OBJID_WLDTIMER_NUM ] = {
	WFLBY_MAPOBJID_WLDTIMER_00,
	WFLBY_MAPOBJID_WLDTIMER_01,
	WFLBY_MAPOBJID_WLDTIMER_02,
	WFLBY_MAPOBJID_WLDTIMER_03,
};
// ���Ճ{�[�h��
static const u32 sc_WFLBY_EV_DEF_NPC_OBJID_FT00[ WFLBY_EV_DEF_NPC_OBJID_FOOT_NUM ] = {
	WFLBY_MAPOBJID_FOOT_00_00,
	WFLBY_MAPOBJID_FOOT_00_01,
	WFLBY_MAPOBJID_FOOT_00_02,
	WFLBY_MAPOBJID_FOOT_00_03,
	WFLBY_MAPOBJID_FOOT_00_04,
	WFLBY_MAPOBJID_FOOT_00_05,
	WFLBY_MAPOBJID_FOOT_00_06,
	WFLBY_MAPOBJID_FOOT_00_07,
};
// ���Ճ{�[�h��
static const u32 sc_WFLBY_EV_DEF_NPC_OBJID_FT01[ WFLBY_EV_DEF_NPC_OBJID_FOOT_NUM ] = {
	WFLBY_MAPOBJID_FOOT_01_00,
	WFLBY_MAPOBJID_FOOT_01_01,
	WFLBY_MAPOBJID_FOOT_01_02,
	WFLBY_MAPOBJID_FOOT_01_03,
	WFLBY_MAPOBJID_FOOT_01_04,
	WFLBY_MAPOBJID_FOOT_01_05,
	WFLBY_MAPOBJID_FOOT_01_06,
	WFLBY_MAPOBJID_FOOT_01_07,
};


//-------------------------------------
///	NPC�W�����v�V�[�P���X
//=====================================
enum {
	WFLBY_EV_DEF_NPCJUMP_SEQ_INIT,
	WFLBY_EV_DEF_NPCJUMP_SEQ_UP,
	WFLBY_EV_DEF_NPCJUMP_SEQ_UPWAIT,
	WFLBY_EV_DEF_NPCJUMP_SEQ_DOWN,
	WFLBY_EV_DEF_NPCJUMP_SEQ_DOWNWAIT,
	WFLBY_EV_DEF_NPCJUMP_SEQ_WAIT,
	WFLBY_EV_DEF_NPCJUMP_SEQ_RET_UP,
	WFLBY_EV_DEF_NPCJUMP_SEQ_RET_UPWAIT,
	WFLBY_EV_DEF_NPCJUMP_SEQ_RET_DOWN,
	WFLBY_EV_DEF_NPCJUMP_SEQ_RET_DOWNWAIT,
	WFLBY_EV_DEF_NPCJUMP_SEQ_END,
} ;
typedef enum{	// �W�����v����	�^�C�v
	WFLBY_EV_DEF_NPCJUMP_MOVETYPE_NORMAL,		// �ʏ�
	WFLBY_EV_DEF_NPCJUMP_MOVETYPE_STARTDOWN,	// �����Ă���Ƃ��납��͂��߂�
} WFLBY_EV_DEF_NPCJUMP_MOVETYPE;


//-------------------------------------
///	NPC�t���[�g�ɏ���đގ�
//=====================================
enum {
	WFLBY_EV_DEF_NPCFLOAT_SEQ_INIT,
	WFLBY_EV_DEF_NPCFLOAT_SEQ_UPWAIT,
	WFLBY_EV_DEF_NPCFLOAT_SEQ_DOWNWAIT,
	WFLBY_EV_DEF_NPCFLOAT_SEQ_MAIN,
	WFLBY_EV_DEF_NPCFLOAT_SEQ_END,
} ;
#define WFLBY_EV_DEF_NPCFLOAT_DIF_X			( FX32_CONST( -16 ) )	// ���n�_
#define WFLBY_EV_DEF_NPCFLOAT_DOWN_COUNT	( 24 )					// �J�E���^


//-------------------------------------
///	�ėp�h���h���A�j��
//=====================================
#define WFLBY_EV_DEF_DONDON_SYNC_ONE		( 4 )
#define WFLBY_EV_DEF_DONDON_MOVEY			( FX32_CONST( 6 ) )
#define WFLBY_EV_DEF_DONDON_NEXT_WAIT_MAX	( 4 )
enum{
	WFLBY_EV_DEF_DONDON_SEQ_END,
	WFLBY_EV_DEF_DONDON_SEQ_WAIT,
	WFLBY_EV_DEF_DONDON_SEQ_MOVE,
};


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------


//-------------------------------------
///	�ėp�h���h���A�j��
//=====================================
typedef struct {
	fx32 y;
	s8	count;			// �J�E���^
	u8	seq;			// �V�[�P���X
	u8	next_wait;		// ���񗐐��Ő�������鎟�̃h���܂ł̃E�G�C�g
	u8	next_ok;		// �����W�����v�����邩�H
} WFLBY_EV_DEF_DONDON;


//-------------------------------------
///	��l����������C�x���g���[�N
//=====================================
typedef struct {
	u16 ret_seq;
	u16 gadget;
	s32	wait;

	// �r�b�g�}�b�v���X�g��p
	BMPLIST_HEADER		bmplist;

	// �I�������Z�^�C�v
	u16					type1;	
	u16					type2;	
} WFLBY_EV_DEF_PLAYER_IN_WK;


//-------------------------------------
///	��l���풓�C�x���g���[�N
//=====================================
typedef struct {
	u32 infodraw_retmvtype;
} WFLBY_EV_DEF_PLAYER_WK;

//-------------------------------------
///	�T�u�`�����l���ڑ��C�x���g���[�N
//=====================================
typedef struct {
	s32 wait;
	u16	msg_idx;
	u16 msg_type;
	u32 minigame;
} WFLBY_EV_DEF_PLAYER_SUBCHAN_WK;

//-------------------------------------
///	�~�j�Q�[���ڑ��C�x���g���[�N
//=====================================
typedef struct {
	WFLBY_EV_MG_PARAM mg_param;
} WFLBY_EV_DEF_PLAYER_MG_WK;

//-------------------------------------
///	�A���P�[�g�ڑ��C�x���g
//=====================================
typedef struct {
	u16 ret_msg;	
	u16 apl_no;
} WFLBY_EV_DEF_PLAYER_ANKETO_WK;


//-------------------------------------
///	�v���C���[�t���[�g
//=====================================
typedef struct {
	u32				floatidx;		// �\�񂵂��t���[�g���ȃC���f�b�N�X
	u16				next_seq;		// ���ɐi�ރV�[�P���X
	u16				station;		// �҂��Ă���w�萔
	s16				count;			// �G�t�F�N�g�J�E���^
	u8				msg_off_ret_seq;// ���b�Z�[�W��\���߂�V�[�P���X
	u8				wait;
	WFLBY_3DPERSON* p_obj;
	WFLBY_EV_DEF_DONDON dondon;		// �h���h���A�j��
} WFLBY_EV_FLOAT_INSIDE;



//-------------------------------------
///	NPC��풓�C�x���g�@
//�@����ꏊ�ɐl���W�����v���Ĕz�u����
//=====================================
typedef struct {
	u8	gridx;
	u8	gridy;
	u8	status;
	u8	talk_flag;
	u8	movetype;
	u8	pad[3];
} WFLBY_EV_DEF_NPCJUMP_PARAM;

//-------------------------------------
///	NPC�풓�C�x���g���[�N
//=====================================
typedef struct {
	WFLBY_EV_DEF_NPCJUMP_PARAM intrparam;		// ���荞�݃C�x���g�p���[�N
} WFLBY_EV_DEF_NPC_WK;

//-------------------------------------
///	NPC�t���[�g�C�x���g���[�N
//=====================================
typedef struct {
	WFLBY_3DPERSON* p_npc;
	u32				float_idx;
	u32				float_offs;

	// �����A�j��
	fx32			start_y;	// �J�n�����W
	fx32			on_x;		// ���n�����W
	s32				count;

	WFLBY_EV_DEF_DONDON dondon;		// �h���h���A�j��
} WFLBY_EV_DEF_NPCFLOAT_WK;




//-------------------------------------
///	�C�x���g�ɂ���ċN������C�x���g
//  �p�����[�^�����R�[���o�b�N�֐�
//=====================================
typedef void (*pWFLBY_EV_DEF_PFUNC)( WFLBY_EV_DEF_PLAYER_WK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno, u32 eventno );
//-------------------------------------
///	�C�x���g�ɂ���ċN������C�x���g
//=====================================
typedef struct {
	u8	event[ WFLBY_EV_DEF_EVNO_MAX ];		// �����C�x���g�i���o�[�@WFLBY_MAPEVID_EV_�`
	u32 evtype;								// �C�x���g�^�C�v	WFLBY_EV_DEF_EVTYPE_�`
	WFLBY_EVENT_DATA	evdata;				// �C�x���g�֐��e�[�u��
	pWFLBY_EV_DEF_PFUNC p_pfunc;			// �p�����[�^�����R�[���o�b�N�������A�C�x���g�^�C�v��WFLBY_EV_DEF_EVTYPE_INTR�̂Ƃ��̂�
} WFLBY_EV_DEF_EVDATA;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
// �S�̂Ŏg�p
//=====================================
static BOOL WFLBY_EV_DEF_WayKeyInput( WF2DMAP_WAY way );
static WFLBY_3DPERSON* WFLBY_EV_DEF_StartObjPlayer( WFLBY_ROOMWK* p_rmwk, u32 plno );
static void WFLBY_EV_DEF_StartObjPlayerEx( WFLBY_ROOMWK* p_rmwk, u32 plno, u32 gridx, u32 gridy, WF2DMAP_WAY way );


//-------------------------------------
///	���p�h���h���A�j��
//=====================================
static void WFLBY_EV_DEF_DONDON_Start( WFLBY_EV_DEF_DONDON* p_wk );
static fx32 WFLBY_EV_DEF_DONDON_Main( WFLBY_EV_DEF_DONDON* p_wk );
static void WFLBY_EV_DEF_DONDON_EndReq( WFLBY_EV_DEF_DONDON* p_wk );
static BOOL WFLBY_EV_DEF_DONDON_CheckMove( const WFLBY_EV_DEF_DONDON* cp_wk );



//-------------------------------------
///	���ʊ��荞�݃C�x���g
//=====================================
// �K�W�F�b�g�Ȃ炵�C�x���g
static BOOL WFLBY_EV_DEF_ALL_IntrGadGet_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );

//-------------------------------------
// �v���C���[�풓�C�x���g
//=====================================
// ���ꃁ�C���C�x���g
static BOOL WFLBY_EV_DEF_PlayerInNormal_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
static BOOL WFLBY_EV_DEF_PlayerInGrid_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
static BOOL WFLBY_EV_DEF_PlayerInMinigame_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
static BOOL WFLBY_EV_DEF_PlayerInAnketo_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );

// �풓���C���C�x���g
static void WFLBY_EV_DEF_Player_StartEventTbl( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno, const WFLBY_EV_DEF_EVDATA* cp_data, u32 eventno );
static BOOL WFLBY_EV_DEF_PlayerMain_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
static BOOL WFLBY_EV_DEF_PlayerMain_After( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
// �ޏ�C�x���g
static BOOL WFLBY_EV_DEF_PlayerOutNormal_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
static BOOL WFLBY_EV_DEF_PlayerOutSubChan_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
static BOOL WFLBY_EV_DEF_PlayerOutMiniGame_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
static BOOL WFLBY_EV_DEF_PlayerOutAnketo( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
static u32 WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoChannel( u32 evno );
static u32 WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoRetNum( u32 evno );
static u32 WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoStatus( u32 evno );
// �I���C�x���g
static BOOL WFLBY_EV_DEF_PlayerA_SWIN_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
// �t���[�g
static BOOL WFLBY_EV_FLOAT_Inside( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
static void WFLBY_EV_FLOAT_SetCameraPos( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_CAMERA* p_camera );
static BOOL WFLBY_EV_FLOAT_CheckJumpOn( const WFLBY_EV_FLOAT_INSIDE* cp_wk, const WFLBY_3DMAPOBJ_CONT* cp_mapobjcont );
static BOOL WFLBY_EV_FLOAT_JumpMove( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, WFLBY_CAMERA* p_camera );
static BOOL WFLBY_EV_FLOAT_JumpOnMove( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont );
static void WFLBY_EV_FLOAT_MatrixSet( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, WFLBY_ROOMWK* p_rmwk );
static void WFLBY_EV_FLOAT_MatrixDonDonSet( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, WFLBY_ROOMWK* p_rmwk );
// �Z�^�C�v�r�b�g�}�b�v���X�g
static void WFLBY_EV_DEF_PlayerInNormal_WazaTypeBmpListInit( WFLBY_EV_DEF_PLAYER_IN_WK* p_wk, WFLBY_ROOMWK* p_rmwk );
static void WFLBY_EV_DEF_PlayerInNormal_WazaTypeBmpListDelete( WFLBY_EV_DEF_PLAYER_IN_WK* p_wk, WFLBY_ROOMWK* p_rmwk );




//-------------------------------------
// NPC�풓�C�x���g
//=====================================
// NPC�o���҂�
static BOOL WFLBY_EV_DEF_NpcWait_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
// NPC���C��
static BOOL WFLBY_EV_DEF_NpcMain_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
// NPC�ގ�
static BOOL WFLBY_EV_DEF_NpcEnd_Brfore( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
// NPC�t���[�g�ގ�
static BOOL WFLBY_EV_DEF_NpcFloat_Brfore( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );


//-------------------------------------
///	NPC��풆�C�x���g
//=====================================
// NPC�w��ʒu�W�����v	�ʏ��ԂɑJ�ڌ㌳�ɖ߂�
static BOOL WFLBY_EV_DEF_NpcJump_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );



//-------------------------------------
///	�p�����[�^�쐬�֐�
//=====================================
// �w��ʒu�W�����v
static BOOL WFLBY_EV_DEF_NpcJumpParamMake( WFLBY_EV_DEF_NPCJUMP_PARAM* p_param, WFLBY_SYSTEM* p_system, WFLBY_ROOMWK* p_rmwk, WFLBY_3DPERSON* p_npc, u8 status, WFLBY_EV_DEF_NPCJUMP_MOVETYPE movetype );
static BOOL WFLBY_EV_DEF_NpcJumpParam_GetOBJIDPos( const WFLBY_MAPCONT* cp_mapcont, const WFLBY_3DOBJCONT* cp_objcont, const u32* cp_objid, u32 num, WF2DMAP_POS* p_pos );



//-----------------------------------------------------------------------------
/**
 *				�C�x���g�f�[�^
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�S���ʃK�W�F�b�g�C�x���g
//=====================================
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_ALL_GADGET = {
	WFLBY_EV_DEF_ALL_IntrGadGet_Before,
	NULL
};

//-------------------------------------
// �v���C���[����C�x���g
//=====================================
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_PLAYERIN[WFLBY_EV_DEF_PLAYERIN_NUM] = {
	{
		WFLBY_EV_DEF_PlayerInNormal_Before,
		NULL,
	},
	{
		WFLBY_EV_DEF_PlayerInGrid_Before,
		NULL,
	},
	{
		WFLBY_EV_DEF_PlayerInMinigame_Before,
		NULL,
	},
	{
		WFLBY_EV_DEF_PlayerInAnketo_Before,
		NULL,
	},
};

//-------------------------------------
// �v���C���[���C���풓�C�x���g
//=====================================
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_PLAYERMAIN = {
	WFLBY_EV_DEF_PlayerMain_Before,
	WFLBY_EV_DEF_PlayerMain_After,
};

//-------------------------------------
// �v���C���[�~�j�Q�[���Q���C�x���g
//=====================================
// �}�b�`���O
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_PLAYER_MGSTART = {
	WFLBY_EV_MG_Start,
	NULL,
};
// �ؒf
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_PLAYER_MGEND = {
	WFLBY_EV_MG_End,
	NULL,
};

//-------------------------------------
// NPC�o���҂��풓�C�x���g
//=====================================
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_NPCWAIT = {
	WFLBY_EV_DEF_NpcWait_Before,
	NULL,
};

//-------------------------------------
// NPC���C���풓�C�x���g
//=====================================
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_NPCMAIN = {
	WFLBY_EV_DEF_NpcMain_Before,
	NULL,
};

//-------------------------------------
// NPC�ގ��풓�C�x���g
//=====================================
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_NPCEND = {
	WFLBY_EV_DEF_NpcEnd_Brfore,
	NULL,
};
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_NPCFLOAT = {
	WFLBY_EV_DEF_NpcFloat_Brfore,
	NULL,
};

//-------------------------------------
// NPC�W�����v��풓�C�x���g
//=====================================
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_NPCJUMP = {
	WFLBY_EV_DEF_NpcJump_Before,
	NULL,
};


//-------------------------------------
// �C�x���gNO�ŋN������C�x���g
//=====================================
// ��ɏ��ƋN���������
static const WFLBY_EV_DEF_EVDATA sc_WFLBY_EV_DEF_EVENT_MOUNTSTART[] = {
	// �o������ޏ�
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_PLAYER_OUT,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_DEF,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerOutNormal_Before,
			NULL,
		},
		NULL,
	},
	// ���E���v�Ɉڍs
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_WLDTIMER_00,
			WFLBY_MAPEVID_EV_WLDTIMER_01,
			WFLBY_MAPEVID_EV_WLDTIMER_02,
			WFLBY_MAPEVID_EV_WLDTIMER_03,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_DEF,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerOutSubChan_Before,
			NULL,
		},
		NULL,
	},
	// �g�s�b�N�Ɉڍs
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_TOPIC_00,
			WFLBY_MAPEVID_EV_TOPIC_01,
			WFLBY_MAPEVID_EV_TOPIC_02,
			WFLBY_MAPEVID_EV_TOPIC_03,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_DEF,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerOutSubChan_Before,
			NULL,
		},
		NULL,
	},

	// ���Ճ{�[�h
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_FOOT_00_00,
			WFLBY_MAPEVID_EV_FOOT_00_01,
			WFLBY_MAPEVID_EV_FOOT_00_02,
			WFLBY_MAPEVID_EV_FOOT_00_03,
			WFLBY_MAPEVID_EV_FOOT_00_04,
			WFLBY_MAPEVID_EV_FOOT_00_05,
			WFLBY_MAPEVID_EV_FOOT_00_06,
			WFLBY_MAPEVID_EV_FOOT_00_07,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_DEF,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerOutSubChan_Before,
			NULL,
		},
		NULL,
	},

	// ���Ճ{�[�h
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_FOOT_01_00,
			WFLBY_MAPEVID_EV_FOOT_01_01,
			WFLBY_MAPEVID_EV_FOOT_01_02,
			WFLBY_MAPEVID_EV_FOOT_01_03,
			WFLBY_MAPEVID_EV_FOOT_01_04,
			WFLBY_MAPEVID_EV_FOOT_01_05,
			WFLBY_MAPEVID_EV_FOOT_01_06,
			WFLBY_MAPEVID_EV_FOOT_01_07,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_DEF,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerOutSubChan_Before,
			NULL,
		},
		NULL,
	},
	
	// �t���[�g�ɏ��
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_FLOAT00,
			WFLBY_MAPEVID_EV_FLOAT01,
			WFLBY_MAPEVID_EV_FLOAT02,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_DEF,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_FLOAT_Inside,
			NULL,
		},
		NULL,
	},

	// �~�j�Q�[���Ɉڍs
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_BALLSLOW,
			WFLBY_MAPEVID_EV_BALANCE,
			WFLBY_MAPEVID_EV_BALLOON,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_DEF,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerOutMiniGame_Before,
			NULL,
		},
		NULL,
	},

	// �A���P�[�g�Ɉڍs
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_ANKETO_BOARD,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_DEF,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerOutAnketo,
			NULL,
		},
		NULL,
	},
	
	// �I�[�f�[�^
	{
		{WFLBY_EV_DEF_EVNO_END,},WFLBY_EV_DEF_EVTYPE_END,{NULL,NULL,},NULL,
	},
};

// �ڂ̑O��A���������Ƃ�
static const WFLBY_EV_DEF_EVDATA sc_WFLBY_EV_DEF_EVENT_ASTART[] = {

	// ������̂��o����
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_SW_IN,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_INTR,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerA_SWIN_Before,
			NULL,
		},
		NULL,
	},

	// �g�s�b�N�̂��o����
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_SW_TOPIC,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_INTR,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerA_SWTOPIC_Before,
			NULL,
		},
		NULL,
	},


	// �g�s�b�N�̂��Z����
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_SW_TOPIC2,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_INTR,
		// �C�x���g�֐��|�C���^
		{
#ifdef WFLBY_DEBUG_TALK_ALLWORLD
			WFLBY_EV_TALK_StartA_AllWorld,
#else
			WFLBY_EV_DEF_PlayerA_SWTOPIC_PLAYED_Before,
#endif
			NULL,
		},
		NULL,
	},

	// �~�j�Q�[��
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_KANBAN00,
			WFLBY_MAPEVID_EV_KANBAN01,
			WFLBY_MAPEVID_EV_KANBAN02,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_INTR,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerA_MINIGAME_Before,
			NULL,
		},
		NULL,
	},

	// �s�G��
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_ANKETO_MAN,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_INTR,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_TALK_Piero,
			NULL,
		},
		NULL,
	},

	// �I�[�f�[�^
	{
		{WFLBY_EV_DEF_EVNO_END,},WFLBY_EV_DEF_EVTYPE_END,{NULL,NULL,},NULL,
	},
};

// �ڂ̑O��UP���������Ƃ�
static const WFLBY_EV_DEF_EVDATA sc_WFLBY_EV_DEF_EVENT_WAYUPSTART[] = {

	// �~�j�Q�[��
	{	// �N���C�x���gNO
		{	
			WFLBY_MAPEVID_EV_KANBAN00,
			WFLBY_MAPEVID_EV_KANBAN01,
			WFLBY_MAPEVID_EV_KANBAN02,
			WFLBY_EV_DEF_EVNO_END,
		},
		// �C�x���g�^�C�v
		WFLBY_EV_DEF_EVTYPE_INTR,
		// �C�x���g�֐��|�C���^
		{
			WFLBY_EV_DEF_PlayerA_MINIGAME_Before,
			NULL,
		},
		NULL,
	},
	
	// �I�[�f�[�^
	{
		{WFLBY_EV_DEF_EVNO_END,},WFLBY_EV_DEF_EVTYPE_END,{NULL,NULL,},NULL,
	},
};




//-------------------------------------
// �b���������荞�݃C�x���g
//=====================================
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_TALK_A = {
	WFLBY_EV_TALK_StartA,
	NULL,
};

//-------------------------------------
// �b������ꊄ�荞�݃C�x���g
//=====================================
static const WFLBY_EVENT_DATA	sc_WFLBY_EV_DEF_TALK_B = {
	WFLBY_EV_TALK_StartB,
	NULL,
};


//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�풓�C�x���g��ݒ�
 *
 *	@param	p_rmwk 
 */
//-----------------------------------------------------------------------------
void WFLBY_EV_DEF_StartPlayer( WFLBY_ROOMWK* p_rmwk )
{
	WFLBY_ROOM_SAVE* p_save;
	WFLBY_EVENT* p_event;
	u32 plno;
	u32 status;

	// �f�[�^�擾
	p_save	= WFLBY_ROOM_GetSaveData( p_rmwk ); 
	p_event	= WFLBY_ROOM_GetEvent( p_rmwk );
	plno	= WFLBY_ROOM_GetPlno( p_rmwk );

	// ���O�C����Ԃɂ���
	WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData( p_rmwk ), WFLBY_STATUS_LOGIN );

	
	// �v���C���[����C�x���g���s��
	GF_ASSERT( p_save->pl_inevno < WFLBY_EV_DEF_PLAYERIN_NUM );

	WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERIN[ p_save->pl_inevno ], NULL );

#if PL_T0867_080716_FIX
	// ������ړ����܂���̂Řb���������Ă����l�͉�b�W�����v���Ȃ�
	WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, TRUE );
#endif

#ifdef WFLBY_DEBUG_EV_DEF_FLOAT_RETRY
	OS_TPrintf( "���t���[�g�f�o�b�N�@�\	B�{�^��	������x�����ɖ߂��Ă���\n" );
#endif
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC	�풓�C�x���g
 *
 *	@param	p_rmwk 
 */
//-----------------------------------------------------------------------------

void WFLBY_EV_DEF_StartNpc( WFLBY_ROOMWK* p_rmwk )
{
	int i;
	u32 plno;
	WFLBY_EVENT* p_event;

	plno	= WFLBY_ROOM_GetPlno( p_rmwk );
	p_event = WFLBY_ROOM_GetEvent( p_rmwk );

	for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
		if( plno != i ){	// �v���C���[�ȊO
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, i, &sc_WFLBY_EV_DEF_NPCWAIT, NULL );
		}
	}
}



//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�����Ă�������̃L�[����͂��Ă��邩�`�F�b�N
 *
 *	@param	way		�����Ă������
 *
 *	@retval	TRUE	�����Ă���
 *	@retval	FALSE	�����Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_WayKeyInput( WF2DMAP_WAY way )
{
	u32 key;
	switch( way ){
	case WF2DMAP_WAY_UP:
		key = PAD_KEY_UP;
		break;
	case WF2DMAP_WAY_DOWN:
		key = PAD_KEY_DOWN;
		break;
	case WF2DMAP_WAY_LEFT:
		key = PAD_KEY_LEFT;
		break;
	case WF2DMAP_WAY_RIGHT:
		key = PAD_KEY_RIGHT;
		break;
	}
	if( sys.cont & key ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�e�[�u���̃C�x���g���N������
 *
 *	@param	p_wk		�C�x���g���[�N
 *	@param	p_rmwk		�������[�N
 *	@param	cp_data		�C�x���g�e�[�u��
 *	@param	eventno		�C�x���g�i���o�[
 *
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_DEF_Player_StartEventTbl( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno, const WFLBY_EV_DEF_EVDATA* cp_data, u32 eventno )
{
	int no;
	int i;
	WFLBY_EV_DEF_PLAYER_WK* p_plwk;
	WFLBY_EVENT* p_event;

	// �C�x���g���[�N�擾
	p_plwk = WFLBY_EVENTWK_GetWk( p_wk );

	// ��������f�[�^�擾
	{
		p_event = WFLBY_ROOM_GetEvent( p_rmwk );
	}


	no = 0;
	while( cp_data[no].evtype != WFLBY_EV_DEF_EVTYPE_END ){

		// �N���C�x���gNo���`�F�b�N
		for( i=0; i<WFLBY_EV_DEF_EVNO_MAX; i++ ){

			// �I���R�[�h
			if( cp_data[no].event[i] == WFLBY_EV_DEF_EVNO_END ){
				break;
			}

			// �C�x���g�i���o�[���ꏏ�Ȃ炻�̃C�x���g�𔭓�
			if( cp_data[no].event[i] == eventno ){
				// �C�x���g�^�C�v�ŋN�����@���ς��
				switch( cp_data[no].evtype ){
				// �풓�C�x���g
				case WFLBY_EV_DEF_EVTYPE_DEF:

					// �����̃��[�N���͂�
					WFLBY_EVENTWK_DeleteWk( p_wk );

					WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &cp_data[no].evdata, NULL );
					break;	
				// ���荞�݃C�x���g
				case WFLBY_EV_DEF_EVTYPE_INTR:
					// �p�����[�^����
					if( cp_data[no].p_pfunc ){
						cp_data[no].p_pfunc( p_plwk, p_rmwk, plno, cp_data[no].event[i] );
					}
					WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_event, plno, &cp_data[no].evdata, NULL );
					break;		
				}
				return ;
			}
		}

		// ���̃e�[�u����
		no++;
	}

	// �ݒ�ł��Ȃ�����
	GF_ASSERT(0);
	return ;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l���̓o�^
 */
//-----------------------------------------------------------------------------
static WFLBY_3DPERSON* WFLBY_EV_DEF_StartObjPlayer( WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_3DPERSON* p_player;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_CAMERA* p_camera;
	VecFx32 vec;


	// �f�[�^�擾
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	
	p_player = WFLBY_3DOBJCONT_AddPlayer( p_objcont, plno );


	// �^�[�Q�b�g��ݒ�
	p_camera = WFLBY_ROOM_GetCamera( p_rmwk );
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_player, &vec );
	WFLBY_CAMERA_SetTargetPerson3D( p_camera, vec.x, vec.y, vec.z );

	return p_player;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l���̓o�^	�O���b�h�w��
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_DEF_StartObjPlayerEx( WFLBY_ROOMWK* p_rmwk, u32 plno, u32 gridx, u32 gridy, WF2DMAP_WAY way )
{
	WFLBY_3DPERSON* p_player;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_CAMERA*	p_camera;

	// �f�[�^�擾
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_camera	= WFLBY_ROOM_GetCamera( p_rmwk );
	
	p_player = WFLBY_3DOBJCONT_AddPlayerEx( p_objcont, plno, gridx, gridy );

	// ������������
	WFLBY_3DOBJCONT_SetWkReqCmd( p_objcont, p_player, WF2DMAP_CMD_NONE, way );

	// �^�[�Q�b�g��ݒ�
	WFLBY_CAMERA_SetTargetPerson( p_camera,
			p_player );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�h���h���A�j��	�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_DEF_DONDON_Start( WFLBY_EV_DEF_DONDON* p_wk )
{
	p_wk->y			= 0;
	p_wk->count		= 0;
	p_wk->seq		= WFLBY_EV_DEF_DONDON_SEQ_WAIT;
	p_wk->next_wait	= gf_mtRand() % WFLBY_EV_DEF_DONDON_NEXT_WAIT_MAX;
	p_wk->next_ok	= TRUE;
}
	
//----------------------------------------------------------------------------
/**
 *	@brief	�h���h���A�j��	���C��
 *
 *	@param	p_wk	���[�N
 *
 *	@param	y���W
 */
//-----------------------------------------------------------------------------
static fx32 WFLBY_EV_DEF_DONDON_Main( WFLBY_EV_DEF_DONDON* p_wk )
{
	u16 rot_y;
	
	switch( p_wk->seq ){
	case WFLBY_EV_DEF_DONDON_SEQ_END:
		p_wk->y = 0;
		break;
		
	case WFLBY_EV_DEF_DONDON_SEQ_WAIT:
		if( (p_wk->next_wait - 1) > 0 ){
			p_wk->next_wait --;
		}else{
			p_wk->seq = WFLBY_EV_DEF_DONDON_SEQ_MOVE;
		}
		break;
		
	case WFLBY_EV_DEF_DONDON_SEQ_MOVE:
		p_wk->count ++;

		rot_y	= (p_wk->count * 0x7fff) / WFLBY_EV_DEF_DONDON_SYNC_ONE;
		p_wk->y	= FX_Mul( FX_SinIdx( rot_y ), WFLBY_EV_DEF_DONDON_MOVEY );

		if( p_wk->count >= WFLBY_EV_DEF_DONDON_SYNC_ONE ){
			if( p_wk->next_ok == TRUE ){
				// �҂����Ԑݒ�
				p_wk->count		= 0;
				p_wk->seq		= WFLBY_EV_DEF_DONDON_SEQ_WAIT;
				p_wk->next_wait	= gf_mtRand() % WFLBY_EV_DEF_DONDON_NEXT_WAIT_MAX;

			}else{
				// �I��
				p_wk->seq = WFLBY_EV_DEF_DONDON_SEQ_END;
			}
		}
		break;

	}

	return p_wk->y;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�h���h���A�j��	�����W�擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�����W
 */
//-----------------------------------------------------------------------------
static fx32 WFLBY_EV_DEF_DONDON_GetY( const WFLBY_EV_DEF_DONDON* cp_wk )
{
	return cp_wk->y;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�����N�G�X�g�ݒ�
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_DEF_DONDON_EndReq( WFLBY_EV_DEF_DONDON* p_wk )
{
	p_wk->next_ok = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�h���h���A�j�������`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	���쒆
 *	@retval	FALSE	������
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_DONDON_CheckMove( const WFLBY_EV_DEF_DONDON* cp_wk )
{
	if( cp_wk->seq == WFLBY_EV_DEF_DONDON_SEQ_END ){
		return FALSE;
	}
	return TRUE;
}




//----------------------------------------------------------------------------
/**
 *	@brief	�S�l���L�����N�^����	�K�W�F�b�g�g�p�C�x���g
 *
 *	@param	p_wk		���[�N
 *	@param	p_rmwk		�������[�N
 *	@param	plno		�v���C���[IDX
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_ALL_IntrGadGet_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_3DOBJCONT*	p_objcont;
	WFLBY_3DPERSON*		p_person;
	WFLBY_SYSTEM*		p_system;

	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_person	= WFLBY_3DOBJCONT_GetPlIDWk( p_objcont, plno );
	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
	
	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// �������ƁA�v���C���[�̓�����Œ�ƁA�K�W�F�b�g�J�n
	case WFLBY_EV_DEF_ALL_GADGET_SEQ_INIT:
		// ������������
//		WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_person, WF2DMAP_CMD_NONE, WF2DMAP_WAY_DOWN );

		// �K�W�F�b�g�J�n
		WFLBY_ROOM_GadGet_Play( p_rmwk, plno );

		// �E�G�C�g��Ԃ�
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_ALL_GADGET_SEQ_WAIT );

		// ��������l�����`�F�b�N
		if( plno == WFLBY_SYSTEM_GetMyIdx( p_system ) ){
			// ������ʃ{�^������J�n
			WFLBY_ROOM_GadGet_SetStop( p_rmwk, FALSE );
		}
		break;
		
	// �K�W�F�b�g�I���҂�
	case WFLBY_EV_DEF_ALL_GADGET_SEQ_WAIT:
		if( WFLBY_ROOM_GadGet_PlayWait( p_rmwk, plno ) == TRUE ){
			// �I��������
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_ALL_GADGET_SEQ_END );
		}
		break;
		
	case WFLBY_EV_DEF_ALL_GADGET_SEQ_END:
		// ����ĊJ
		// ��������l�����`�F�b�N
		if( plno == WFLBY_SYSTEM_GetMyIdx( p_system ) ){
			// ��l������ݒ�
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_person, WFLBY_3DOBJCONT_MOVEPLAYER );

#if	PL_T0834_080709_FIX
		WFLBY_ROOM_GadGet_ClearLockWait( p_rmwk );
#else
		//
#endif
		}else{
		

			// profile�\�����̏ꍇ����𕜋A�����Ȃ�
			// ����ȊO�͕��A������
			if( (WFLBY_ROOM_UNDERWIN_TrCardGetDraw( p_rmwk ) == TRUE) ){
				if( plno != WFLBY_ROOM_UNDERWIN_TrCardGetPlIdx( p_rmwk ) ){
					// NPC����ݒ�
					WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_person, WFLBY_3DOBJCONT_MOVENPC );
				}
			}else{
				// NPC����ݒ�
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_person, WFLBY_3DOBJCONT_MOVENPC );
			}

			// ���[�J����Ԃ�BUSY�����ɖ߂�
			// ��b������~
			WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, FALSE );

			// NPC��ԕ\���s�\
//			WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, FALSE );				
		}
		
#if	PL_T0834_080709_FIX
		//
#else
		WFLBY_ROOM_GadGet_ClearLockWait( p_rmwk );
#endif
		return TRUE;
	}

	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[	�o���Ǘ��C�x���g
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerInNormal_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_PLAYER_IN_WK* p_evwk;
	WFLBY_3DPERSON* p_player;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_SYSTEM*	p_system;
	STRBUF* p_str;
	
	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
	p_evwk		= WFLBY_EVENTWK_GetWk( p_wk );

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ��l���ɓo�^
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_ADD:
		// work�쐬
		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_DEF_PLAYER_IN_WK) );

		//  �Z�^�C�v���X�g�f�[�^���쐬
		WFLBY_EV_DEF_PlayerInNormal_WazaTypeBmpListInit( p_evwk, p_rmwk );

		// �����ɓ��ꂽ�̂�BGM��ύX
		WFLBY_SYSTEM_SetNowBGM( p_system );

		// �K�W�F�b�g�ݒ�
		{

#ifdef WFLBY_DEBUG_EV_DEF_GADGET_GET
			const WFLBY_USER_PROFILE* cp_profile;

			// �f�o�b�N���s�����Ƃ��̂݁A�I�񂾃K�W�F�b�g�𔽉f
			if( D_Tomoya_WiFiLobby_DebugStart == TRUE ){
				cp_profile = WFLBY_SYSTEM_GetUserProfile( p_system, plno );
				p_evwk->gadget = WFLBY_SYSTEM_GetProfileItem( cp_profile );
			}else{
				p_evwk->gadget = WFLBY_SYSTEM_RandItemGet( p_system );
			}
#else
			p_evwk->gadget = WFLBY_SYSTEM_RandItemGet( p_system );
#endif
			WFLBY_SYSTEM_SetMyItem( p_system, p_evwk->gadget );
		}
		
		// ��l���̓o�^
		p_player = WFLBY_EV_DEF_StartObjPlayer( p_rmwk, plno );

		// ���Ƃ�
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEFLYDOWN );

		WFLBY_EVENTWK_AddSeq( p_wk );
		break;
	// �t�F�[�h�C�������Ɠ��슮���҂�
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_FADEWAIT:
		if( (WFLBY_ROOM_GetFadeFlag( p_rmwk ) == FALSE) &&
			(WFLBY_3DOBJCONT_CheckWkMoveEnd( p_player ) == TRUE) ){

			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVENONE );

			// �^�[�Q�b�g��ݒ�
			{
				WFLBY_CAMERA* p_camera;
				p_camera = WFLBY_ROOM_GetCamera( p_rmwk );
				WFLBY_CAMERA_SetTargetPerson( p_camera,
						p_player );
			}

			p_evwk->wait = WFLBY_EV_DEF_PLAYERIN_NORMAL_MOVEWAIT;


			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	// ����E�G�C�g
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVESTARTWAIT:
		p_evwk->wait --;
		if( p_evwk->wait == 0 ){

			// �ʐM�G���[�`�F�b�N	ON
			WFLBY_ROOM_SetErrorCheckFlag( p_rmwk, TRUE );

			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	// �O��
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE00:	
		WFLBY_3DOBJCONT_SetWkReqCmd( p_objcont, p_player, WF2DMAP_OBJST_WALK, WF2DMAP_WAY_UP );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVEWAIT );
		break;

	// �O��
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE01:	
		WFLBY_3DOBJCONT_SetWkReqCmd( p_objcont, p_player, WF2DMAP_OBJST_WALK, WF2DMAP_WAY_UP );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE02;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVEWAIT );
		break;

	// �^�[��
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE02:	
		WFLBY_3DOBJCONT_SetWkReqCmd( p_objcont, p_player, WF2DMAP_OBJST_TURN, WF2DMAP_WAY_RIGHT );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE03;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVEWAIT );
		break;

	// ����
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVE03:	
		WFLBY_3DOBJCONT_SetWkReqCmd( p_objcont, p_player, WF2DMAP_OBJST_WALK, WF2DMAP_WAY_RIGHT );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG00;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVEWAIT );
		break;

	// ���슮���҂�
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MOVEWAIT:	
		if( WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_ST ) == WF2DMAP_OBJST_NONE ){
			WFLBY_EVENTWK_SetSeq( p_wk, p_evwk->ret_seq );
		}
		break;

	// �h�悤�����h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG00:

#if PL_T0867_080716_FIX
		// �����ړ����Ȃ��̂ŁA���l����b�W�����v����OK
		WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif
		
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG15;	// ��Ƀ^�b�`�g�C�����炤
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT );

		break;

	// �h�P�߂̃^�C�v�������łˁh
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG01:		
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_t02 );
		WFLBY_ROOM_TALKWIN_PrintStrAllPut( p_rmwk, p_str );

		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist, 0, 0 );
		WFLBY_ROOM_LISTWIN_YAZIRUSHI_SetDraw( p_rmwk, TRUE );	// ���\��
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG03 );
		break;
		
/*
	// �h�I��\���i�P�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG02:
		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist, 0, 0 );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG03 );
		break;
//*/
		
	// �h�I���i�P�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG03:		
		{
			u32 result;
			u32 str_no;
			BOOL delete = FALSE;
			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );

			switch( result ){
			case BMPLIST_NULL:	
				break;

			// �I�΂Ȃ�
			case BMPLIST_CANCEL:
				Snd_SePlay( SEQ_SE_DP_SELECT );	// CANCEL��
			case WFLBY_POKEWAZA_TYPE_NONE:
				p_evwk->type1 = WFLBY_POKEWAZA_TYPE_NONE;
				delete = TRUE;
				break;
			
			// �Z�^�C�v��I��
			default:
				p_evwk->type1 = result;
				delete = TRUE;
				WFLBY_ROOM_MSG_SetWazaType( p_rmwk, 0, result );
				str_no = msg_hiroba_infoa_t05;
				break;
			}

			if( delete == TRUE ){
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );

				if( p_evwk->type1 != WFLBY_POKEWAZA_TYPE_NONE ){

					p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, str_no );
					WFLBY_ROOM_TALKWIN_PrintStrAllPut( p_rmwk, p_str );

					WFLBY_ROOM_YESNOWIN_Start( p_rmwk );
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG06 );
				}else{

					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG16 );	// �I������
				}
			}
		}
		break;
		
/*
	// �h����ł悢�ł����H�i�P�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG04:		
		break;
*/
		
/*
	// �hYESNO�\���i�P�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG05:
		WFLBY_ROOM_YESNOWIN_Start( p_rmwk );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG06 );
		break;
*/
		
	// �hYESNO�i�P�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG06:		
		{
			WFLBY_ROOM_YESNO_RET result;

			result = WFLBY_ROOM_YESNOWIN_Main( p_rmwk );
			switch( result ){
			// �͂�
			case WFLBY_ROOM_YESNO_OK:
				// YESNO�E�B���h�E��������
				WFLBY_ROOM_YESNOWIN_End( p_rmwk );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG07 );

				// �I�������Z�^�C�v��ݒ�
				WFLBY_SYSTEM_SetSelectWazaType( p_system, p_evwk->type1, p_evwk->type2 );
				WFLBY_ROOM_UNDERWIN_TrCardWazaTypeWrite( p_rmwk );

				Snd_SePlay( WFLBY_SND_TYPE_SET );
				break;
				
			// ������
			case WFLBY_ROOM_YESNO_NO:	
				// YESNO�E�B���h�E��������
				WFLBY_ROOM_YESNOWIN_End( p_rmwk );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG01 );
				break;

			default:
				break;
			}
		}
		break;
		
	// �h�Q�߂̃^�C�v�������łˁh
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG07:		
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_t06 );
		WFLBY_ROOM_TALKWIN_PrintStrAllPut( p_rmwk, p_str );
		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist, 0, 0 );
		WFLBY_ROOM_LISTWIN_YAZIRUSHI_SetDraw( p_rmwk, TRUE );	// ���\��
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG09 );
		break;
		
/*
	// �h�I��\���i�Q�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG08:
		WFLBY_ROOM_LISTWIN_Start( p_rmwk, &p_evwk->bmplist, 0, 0 );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG09 );
		break;
//*/
		
	// �h�I���i�Q�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG09:		
		{
			u32 result;
			u32 str_no;
			BOOL delete = FALSE;
			result = WFLBY_ROOM_LISTWIN_Main( p_rmwk );

			switch( result ){
			case BMPLIST_NULL:	
				break;

			// �I�΂Ȃ�
			case BMPLIST_CANCEL:
				Snd_SePlay( SEQ_SE_DP_SELECT );	// CANCEL��
			case WFLBY_POKEWAZA_TYPE_NONE:
				p_evwk->type2 = WFLBY_POKEWAZA_TYPE_NONE;
				delete = TRUE;
				break;
			
			// �Z�^�C�v��I��
			default:
				p_evwk->type2 = result;
				delete = TRUE;
				WFLBY_ROOM_MSG_SetWazaType( p_rmwk, 0, result );
				str_no = msg_hiroba_infoa_t05;
				break;
			}

			if( delete == TRUE ){
				WFLBY_ROOM_LISTWIN_End( p_rmwk, NULL, NULL );

				if( p_evwk->type2 != WFLBY_POKEWAZA_TYPE_NONE ){

					p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, str_no );
					WFLBY_ROOM_TALKWIN_PrintStrAllPut( p_rmwk, p_str );

					WFLBY_ROOM_YESNOWIN_Start( p_rmwk );
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG12 );
				}else{

					// �P�͑I��ł�̂ŁA���o����̂����t
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG18 );
				}
			}
		}
		break;
		
/*
	// �h����ł悢�ł����H�i�Q�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG10:		
		break;
*/
/*		
	// �hYESNO�\���i�Q�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG11:		
		WFLBY_ROOM_YESNOWIN_Start( p_rmwk );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG12 );
		break;
*/
		
	// �hYESNO�i�Q�߁j�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG12:		
		{
			WFLBY_ROOM_YESNO_RET result;

			result = WFLBY_ROOM_YESNOWIN_Main( p_rmwk );
			switch( result ){
			// �͂�
			case WFLBY_ROOM_YESNO_OK:
				
				// YESNO�E�B���h�E��������
				WFLBY_ROOM_YESNOWIN_End( p_rmwk );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG19 );	//�@���o����̂����t

				// �I�������Z�^�C�v��ݒ�
				WFLBY_SYSTEM_SetSelectWazaType( p_system, p_evwk->type1, p_evwk->type2 );
				WFLBY_ROOM_UNDERWIN_TrCardWazaTypeWrite( p_rmwk );

				Snd_SePlay( WFLBY_SND_TYPE_SET );
				break;
				
			// ������
			case WFLBY_ROOM_YESNO_NO:	
				// YESNO�E�B���h�E��������
				WFLBY_ROOM_YESNOWIN_End( p_rmwk );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG07 );
				break;

			default:
				break;
			}
		}
		break;

/*
	// �h�v���t�B�[������������h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG13:
		Snd_SePlay( WFLBY_SND_TOUCH_TOY_CHG );

		// �I�������Z�^�C�v��ݒ�
		WFLBY_SYSTEM_SetSelectWazaType( p_system, p_evwk->type1, p_evwk->type2 );

		// �K�W�F�b�g�擾	�{�^���\���@�E�E�E��
		// �g���[�i�[�J�[�h�\���ɕύX
		WFLBY_ROOM_GadGet_Get( p_rmwk );

		// �l�����ݒ�
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, plno, 0 );
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_t06_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG14;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT );
		break;
*/
/*
	// �h�^�b�`�g�C����������I�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG14:

		// �l�����ݒ�
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_t07 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG15;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT );

		break;
*/

	// �h�K�W�F�b�g����������h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG15:


		Snd_SePlay( WFLBY_SND_TOUCH_TOY_CHG );

		// �K�W�F�b�g�擾
		WFLBY_ROOM_GadGet_Get( p_rmwk );

		// �l�����ݒ�
		WFLBY_ROOM_MSG_SetIdxPlayerName( p_rmwk, plno, 0 );
		WFLBY_ROOM_MSG_SetItem( p_rmwk, p_evwk->gadget, 1 );
		{
			BOOL vip;
			vip = WFLBY_SYSTEM_GetUserVipFlag( p_system, plno );
			if( vip == FALSE ){
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_02 );
			}else{
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_02_vip );
			}
		}
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG17;	// �^�C�v�I����
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT );
		break;

	// �h���̂���łˁI�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG16:
		
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_03 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_END;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT );
		break;

	// �h��낵����΁@�^�C�v�I���h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG17:

		if( !((Snd_SePlayCheck( WFLBY_SND_TOUCH_TOY_CHG ) == FALSE) && (sys.trg & (PAD_BUTTON_DECIDE|PAD_BUTTON_CANCEL))) ){
			break;
		}

		Snd_SePlay( SEQ_SE_DP_SELECT );
		
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_t01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG01;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT );


		// �Z�^�C�v������
		WFLBY_SYSTEM_SetSelectWazaType( p_system, p_evwk->type1, p_evwk->type2 );
		// �g���[�i�J�[�h�\��
		WFLBY_ROOM_UNDERWIN_TrCardOn( p_rmwk, plno, FALSE );
		break;

	// �h�P���I�������Ƃ��́@���o����̂����t�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG18:

		WFLBY_ROOM_MSG_SetWazaType( p_rmwk, 0, p_evwk->type1 );
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_t07 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG16;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT );
		break;
		
	// �h�Q���I�������Ƃ��́@���o����̂����t�h
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG19:
		WFLBY_ROOM_MSG_SetWazaType( p_rmwk, 0, p_evwk->type1 );
		WFLBY_ROOM_MSG_SetWazaType( p_rmwk, 1, p_evwk->type2 );
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_t06_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		p_evwk->ret_seq = WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSG16;
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT );
		break;

	// ���b�Z�[�W�I���҂�
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_MSGWAIT:	
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){
			WFLBY_EVENTWK_SetSeq( p_wk, p_evwk->ret_seq );
		}
		break;

	// ���ꊮ��
	case WFLBY_EV_DEF_PLAYERIN_NORMAL_SEQ_END:
		// ��{����̊֐���ݒ�
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );
		

		// ���b�Z�[�WOFF
		WFLBY_ROOM_TALKWIN_Off( p_rmwk );

		// �Z�^�C�v���X�g�f�[�^�j��
		WFLBY_EV_DEF_PlayerInNormal_WazaTypeBmpListDelete( p_evwk, p_rmwk );

		// ���[�N�j��
		WFLBY_EVENTWK_DeleteWk( p_wk );

		// �v���C���[���C���풓�C�x���g��ݒ�
		{
			WFLBY_EVENT* p_event;
			u32 plno;
			p_event = WFLBY_ROOM_GetEvent( p_rmwk );
			plno = WFLBY_ROOM_GetPlno( p_rmwk );
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERMAIN, NULL );

		}

		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[	�o���Ǘ��C�x���g	WFLBY_ROOM_SAVE�̃O���b�h�ʒu�ɏo��
 *			�O���b�h���w�肵�ē���(SUBCHAN����̑ޔ�)
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerInGrid_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_3DPERSON* p_player;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_ROOM_SAVE* p_save;
	WFLBY_SYSTEM*	p_system;

	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_save		= WFLBY_ROOM_GetSaveData( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );


	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
		
	// ��l����o�^
	case WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_ADD:
		// ��l���̓o�^
		WFLBY_EV_DEF_StartObjPlayerEx( p_rmwk, plno,
				p_save->pl_gridx, p_save->pl_gridy,
				WF2DMAP_WAY_DOWN );


		// ���O�A�E�g������
		// �G���[����Ȃ����
		if( WFLBY_ERR_CheckError() == FALSE ){
			DWC_LOBBY_SUBCHAN_Logout();
		}

		// BGM���t�F�[�h�C��
		WFLBY_SYSTEM_SetBGMVolumeDown( p_system, FALSE );

		WFLBY_EVENTWK_AddSeq( p_wk );
		break;
	// �t�F�[�h�C���҂�
	case WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_FADEWAIT:

		// LOGOUT�҂�
		// �G���[����Ȃ����
		if( (WFLBY_ERR_CheckError() == FALSE) ){
			if( DWC_LOBBY_SUBCHAN_LogoutWait() == FALSE ){
				break;
			}
		}
		
		// �t�F�[�h�C���҂�
		p_save = WFLBY_ROOM_GetSaveData( p_rmwk );
		if( WFLBY_ROOM_GetFadeFlag( p_rmwk ) == FALSE ){
			WFLBY_EVENTWK_AddSeq( p_wk );
			// ���샊�N�G�X�g
			WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_player, WF2DMAP_CMD_WALK, p_save->pl_way );
		}
		break;

	// �v���C���[���쏈��
	case WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_PLMOVE:
		{
			u32 plst;
			plst = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_ST );
			if( plst == WF2DMAP_OBJST_NONE ){


#if PL_T0867_080716_FIX
				// �����ړ����Ȃ��̂ŁA���l����b�W�����v����OK
				WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif

				// �ʐM�G���[�`�F�b�N	ON
				WFLBY_ROOM_SetErrorCheckFlag( p_rmwk, TRUE );

				if( WFLBY_SYSTEM_APLFLAG_GetForceEnd( p_system ) == TRUE ){
					WFLBY_EVENTWK_AddSeq( p_wk );
				}else{
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_END );
				}
			}
		}
		break;

	// �A�v���P�[�V���������ގ����b�Z�[�W�̕\��
	case WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_APLFORCEEND_MSG:
		{
			STRBUF* p_str;
			WFLBY_ROOM_MSG_SetMinigame( p_rmwk, p_save->minigame, 0 );
			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_INFO, msg_wifi_h_info_05_02 );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
		
	// �A�v���P�[�V���������ގ����b�Z�[�W�̕\�������҂�
	case WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_APLFORCEEND_MSGWAIT:
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){
			WFLBY_ROOM_TALKWIN_Off( p_rmwk );
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
	
	// ���ꊮ��
	case WFLBY_EV_DEF_PLAYERIN_GRID_SEQ_END:
		// ��{����̊֐���ݒ�
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );

		{
			WFLBY_EVENT* p_event;
			u32 plno;
			p_event = WFLBY_ROOM_GetEvent( p_rmwk );
			plno	= WFLBY_ROOM_GetPlno( p_rmwk );
			// �v���C���[���C���풓�C�x���g��ݒ�
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERMAIN, NULL );
		}
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����̕��A	WFLBY_ROOM_SAVE�̃O���b�h�ʒu�ɏo��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerInMinigame_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_3DPERSON* p_player;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_EVENT* p_event;
	WFLBY_ROOM_SAVE* p_save;
	WFLBY_SYSTEM*	p_system;


	p_event		= WFLBY_ROOM_GetEvent( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_save		= WFLBY_ROOM_GetSaveData( p_rmwk );

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
		
	// ��l����o�^
	case WFLBY_EV_DEF_PLAYERIN_MG_SEQ_ADD:
		{
			
			// ��l���̓o�^
			WFLBY_EV_DEF_StartObjPlayerEx( p_rmwk, plno,
					p_save->pl_gridx, p_save->pl_gridy,
					WF2DMAP_WAY_DOWN );

			WFLBY_EVENTWK_AddSeq( p_wk );

			// ���荞�݃C�x���g�Őؒf����
			WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_event, plno, 
					&sc_WFLBY_EV_DEF_PLAYER_MGEND, NULL );

			// BGM�ύX���b�N������
			WFLBY_SYSTEM_SetLockBGMChange( p_system, FALSE );

			// BGM��ύX
			WFLBY_SYSTEM_SetNowBGM( p_system );
			
		}
		break;
	// �t�F�[�h�C���҂�
	case WFLBY_EV_DEF_PLAYERIN_MG_SEQ_FADEWAIT:

		// �t�F�[�h�C���҂�
		{
			WFLBY_ROOM_SAVE* p_save;
			p_save = WFLBY_ROOM_GetSaveData( p_rmwk );
			if( WFLBY_ROOM_GetFadeFlag( p_rmwk ) == FALSE ){
				WFLBY_EVENTWK_AddSeq( p_wk );
				// ���샊�N�G�X�g
				WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_player, WF2DMAP_CMD_WALK, p_save->pl_way );
			}
		}
		break;

	// �v���C���[���쏈��
	case WFLBY_EV_DEF_PLAYERIN_MG_SEQ_PLMOVE:
		{
			u32 plst;
			plst = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_ST );
			if( plst == WF2DMAP_OBJST_NONE ){

#if PL_T0867_080716_FIX
				// �����ړ����Ȃ��̂ŁA���l����b�W�����v����OK
				WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif

				// �ʐM�G���[�`�F�b�N	ON
				WFLBY_ROOM_SetErrorCheckFlag( p_rmwk, TRUE );

				if( WFLBY_SYSTEM_MG_CheckErrorEnd( p_system ) == TRUE ){
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_MG_SEQ_MSG );
				}else{
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_MG_SEQ_END );
				}
			}
		}
		break;
	
	// �����I������烁�b�Z�[�W�\��
	case WFLBY_EV_DEF_PLAYERIN_MG_SEQ_MSG:
		{
			STRBUF* p_str;
			WFLBY_ROOM_MSG_SetMinigame( p_rmwk, p_save->minigame, 0 );
			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_INFO, msg_wifi_h_info_01_09 );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	// �����I������烁�b�Z�[�W�\��
	case WFLBY_EV_DEF_PLAYERIN_MG_SEQ_MSGEND:
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){
			WFLBY_ROOM_TALKWIN_Off( p_rmwk );
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
	
	// ���ꊮ��
	case WFLBY_EV_DEF_PLAYERIN_MG_SEQ_END:
		// ��{����̊֐���ݒ�
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );

		{
			WFLBY_EVENT* p_event;
			u32 plno;
			p_event = WFLBY_ROOM_GetEvent( p_rmwk );
			plno	= WFLBY_ROOM_GetPlno( p_rmwk );
			// �v���C���[���C���풓�C�x���g��ݒ�
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERMAIN, NULL );
		}
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g����߂��Ă���
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerInAnketo_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_3DPERSON* p_player;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_ROOM_SAVE* p_save;
	WFLBY_SYSTEM*	p_system;

	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_save		= WFLBY_ROOM_GetSaveData( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );


	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
		
	// ��l����o�^
	case WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_ADD:
		// ��l���̓o�^
		WFLBY_EV_DEF_StartObjPlayerEx( p_rmwk, plno,
				p_save->pl_gridx, p_save->pl_gridy,
				WF2DMAP_WAY_DOWN );


		// BGM���t�F�[�h�C��
		WFLBY_SYSTEM_SetBGMVolumeDown( p_system, FALSE );

		WFLBY_EVENTWK_AddSeq( p_wk );
		break;
	// �t�F�[�h�C���҂�
	case WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_FADEWAIT:

		// �t�F�[�h�C���҂�
		p_save = WFLBY_ROOM_GetSaveData( p_rmwk );
		if( WFLBY_ROOM_GetFadeFlag( p_rmwk ) == FALSE ){
			WFLBY_EVENTWK_AddSeq( p_wk );
			// ���샊�N�G�X�g
			WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_player, WF2DMAP_CMD_WALK, p_save->pl_way );
		}
		break;

	// �v���C���[���쏈��
	case WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_PLMOVE:
		{
			u32 plst;
			plst = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_ST );
			if( plst == WF2DMAP_OBJST_NONE ){

#if PL_T0867_080716_FIX
				// �����ړ����Ȃ��̂ŁA���l����b�W�����v����OK
				WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif

				// �ʐM�G���[�`�F�b�N	ON
				WFLBY_ROOM_SetErrorCheckFlag( p_rmwk, TRUE );

				if( WFLBY_SYSTEM_APLFLAG_GetForceEnd( p_system ) == TRUE ){
					WFLBY_EVENTWK_AddSeq( p_wk );
				}else{
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_END );
				}
			}
		}
		break;

	// �A�v���P�[�V���������ގ����b�Z�[�W�̕\��
	case WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_APLFORCEEND_MSG:
		{
			STRBUF* p_str;
			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_survey_guide_a_05 );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
		
	// �A�v���P�[�V���������ގ����b�Z�[�W�̕\�������҂�
	case WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_APLFORCEEND_MSGWAIT:
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){
			WFLBY_ROOM_TALKWIN_Off( p_rmwk );
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
	
	// ���ꊮ��
	case WFLBY_EV_DEF_PLAYERIN_ANKETO_SEQ_END:
		// ��{����̊֐���ݒ�
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );

		{
			WFLBY_EVENT* p_event;
			u32 plno;
			p_event = WFLBY_ROOM_GetEvent( p_rmwk );
			plno	= WFLBY_ROOM_GetPlno( p_rmwk );
			// �v���C���[���C���풓�C�x���g��ݒ�
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERMAIN, NULL );
		}
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[���C��		����
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerMain_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_PLAYER_WK* p_plwk;

	// �C�x���g���[�N�擾
	p_plwk = WFLBY_EVENTWK_GetWk( p_wk );


	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ������
	case WFLBY_EV_DEF_PLAYER_SEQ_INIT:
		WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_DEF_PLAYER_WK) );
		WFLBY_EVENTWK_AddSeq( p_wk );
		break;
		
	// �C�x���g�`�F�b�N �ȂǂȂ�
	case WFLBY_EV_DEF_PLAYER_SEQ_EVCHECK:
		WFLBY_ROOM_GadGet_SetStop( p_rmwk, FALSE );	// �{�^�������J�n

		{
			u32 st;
			u32 way;
			WFLBY_3DPERSON* p_player;
			WF2DMAP_POS	pos, waypos;
			u32 event, wayevent;
			WFLBY_3DOBJCONT* p_objcont;
			WFLBY_3DMAPOBJ_CONT* p_mapobjcont;
			WFLBY_MAPCONT* p_mapcont;
			WFLBY_SYSTEM* p_system;
			WFLBY_EVENT*	p_event;
			WFLBY_3DPERSON* p_pierrot;


			p_event		= WFLBY_ROOM_GetEvent( p_rmwk );
			p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
			p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
			p_mapcont	= WFLBY_ROOM_GetMapCont( p_rmwk );
			p_mapobjcont= WFLBY_ROOM_GetMapObjCont( p_rmwk );	


			p_pierrot		= WFLBY_3DOBJCONT_GetPierrot( p_objcont );
			if( p_pierrot != NULL ){

				// �����ŁA�s�G���𑀍삷��
				// �s�G������ђ��˂��肷��̂�l�����쏈���̊O���ł���Ă��܂��ƁA
				// �l���Ɨ����Ă���Œ��Ƀs�G�����W�����s�������\��������
				// �l�I���C�x���g�`PARADE�I���܂Ŕ�ђ��˂�
				if( (WFLBY_SYSTEM_Event_GetEndCM( p_system ) == FALSE) &&
					(WFLBY_SYSTEM_Event_GetNeon( p_system ) == TRUE) &&
					(WFLBY_SYSTEM_FLAG_GetAnketoView( p_system ) == FALSE) ){

					// �Ƃт͂˓���
					if( WFLBY_3DOBJCONT_GetWkMove( p_pierrot ) != WFLBY_3DOBJCONT_MOVEJUMP ){
						WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_pierrot, WFLBY_3DOBJCONT_MOVEJUMP );
					}
				}else{

					// �Ƃ܂铮��
					if( WFLBY_3DOBJCONT_GetWkMove( p_pierrot ) != WFLBY_3DOBJCONT_MOVENONE ){
						WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_pierrot, WFLBY_3DOBJCONT_MOVENONE );
					}
				}
			}


			// �v���C���[���擾
			p_player = WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			
			// �v���C���[�X�e�[�^�X��NONE�̂Ƃ��ɃC�x���g�𔭐��`�F�b�N���s��
			st		= WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_ST );
			way		= WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_WAY );

			// ���W�擾
			pos		= WFLBY_3DOBJCONT_GetWkPos( p_player );
			waypos	= WF2DMAP_OBJToolWayPosGet( pos, way );

			// �C�x���g�擾
			event		= WFLBY_MAPCONT_EventGet( p_mapcont, WF2DMAP_POS2GRID(pos.x), WF2DMAP_POS2GRID(pos.y) );
			wayevent	= WFLBY_MAPCONT_EventGet( p_mapcont, WF2DMAP_POS2GRID(waypos.x), WF2DMAP_POS2GRID(waypos.y) );

			// ����NPC�̏�Ԃ�\�����Ă���Ƃ��́A
			// NPC��INFODRAW_BUSY��ԂɂȂ�����\����OFF�ɂ���
			{
				u32 npcid;
				npcid = WFLBY_ROOM_UNDERWIN_TrCardGetPlIdx( p_rmwk );
				if( npcid != plno ){
					// ��ԕ`��r�W�[�ɂȂ�����OFF
					if( WFBLY_ROOM_CheckNpcInfoDrawBusy( p_rmwk, npcid ) ){
						WFLBY_ROOM_UNDERWIN_TrCardOff( p_rmwk );
					}
				}
			}

			// ��Ԃ�NONE�Ȃ�C�x���g�`�F�b�N�Ȃ�
			// NONE�ȊO�Ȃ�g���[�i�[�J�[�hOFF�`�F�b�N
			if( st != WF2DMAP_OBJST_NONE ){

				if( WFLBY_ROOM_UNDERWIN_TrCardCheckCanOff( p_rmwk ) == TRUE ){
					WFLBY_3DPERSON* p_npc;
					u32 npc_id;
					// ���������A�U������A�����ԂŁA
					// trainer�J�[�h���o���Ă�����{�^���ɖ߂�
					if( (st == WF2DMAP_OBJST_WALK) ||
						(st == WF2DMAP_OBJST_RUN) ||
						(st == WF2DMAP_OBJST_TURN) ){

						npc_id = WFLBY_ROOM_UNDERWIN_TrCardGetPlIdx( p_rmwk );
						if( npc_id != plno ){
							p_npc = WFLBY_3DOBJCONT_GetPlIDWk( p_objcont, npc_id );

							// �����~��ԂȂ�ݒ�
							if( p_npc ){
								if( WFLBY_3DOBJCONT_GetWkMove( p_npc ) == WFLBY_3DOBJCONT_MOVENONE ){
									WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, p_plwk->infodraw_retmvtype );	// NPC����ĊJ
								}
							}
						}
						WFLBY_ROOM_UNDERWIN_TrCardOff( p_rmwk );
					}
				}
				
			}else{

#if PL_T0867_080716_FIX
				// ���ĂȂ��̂ŁA���l����b�W�����v����OK
				WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif

				// �^�b�`�g�C��������
				WFLBY_ROOM_GadGet_SetStop( p_rmwk, FALSE );

				// �C�x���g�ɏ�������`�F�b�N
				if( WFLBY_MAPCONT_Event_CheckMount( event ) == TRUE ){


					// ���̃C�x���g����
					WFLBY_EV_DEF_Player_StartEventTbl( p_wk, p_rmwk, plno, 
							sc_WFLBY_EV_DEF_EVENT_MOUNTSTART, event );
					// �C�x���g���������瓮����ꎞ��~
					WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVENONE );

					// �N�������C�x���g��ݒ�
					WFLBY_ROOM_SetEvNo( p_rmwk, event );

					WFLBY_ROOM_GadGet_SetStop( p_rmwk, TRUE );	// �{�^�������Ȃ�

#if PL_T0867_080716_FIX
					// ������Ƃ��̃C�x���g�͂��ׂăI�[�g������s���\��������B
					// �b���������Ă����l�͉�b�W�����v���Ȃ�
					WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, TRUE );
#endif

					return FALSE;	
				}


				// �K�W�F�b�g�̎��s
				if( WFLBY_ROOM_GadGet_GetFlag( p_rmwk ) == TRUE ){
					// �l�������~
					WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVENONE );
					
					// ���荞�݃C�x���g����
					WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_ALL_GADGET, NULL );
					WFLBY_ROOM_GadGet_SetStop( p_rmwk, TRUE );	// �{�^�������Ȃ�
					return FALSE;
				}


				// �b��������ꂽ��
				if( WFLBY_SYSTEM_CheckTalkReq( p_system ) ){
					WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_TALK_B, NULL );
					// �C�x���g���������瓮����ꎞ��~
					WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVENONE );
					WFLBY_ROOM_GadGet_SetStop( p_rmwk, TRUE );	// �{�^�������Ȃ�
					return FALSE;
				}


				// �I���֌W
				if( sys.trg & PAD_BUTTON_DECIDE ){
					// �ڂ̑O�ɒN�����邩�`�F�b�N
					// �����炻�̐l�Ƙb��
					{
						WFLBY_3DPERSON* p_npc;
						u32 status;
						BOOL talk_busy;
						u32 npc_id;
						
						p_npc		= WFLBY_3DOBJCONT_GetFrontPerson( p_objcont, p_player );

						// ��b��
						if( p_npc ){

							// ���ׂĂ��n�߂�O�ɂ��̐l�̃��[�J����Ԃ����b�������Ă��悢��Ԃ��`�F�b�N
							status		= WFLBY_3DOBJCONT_GetWkObjData( p_npc, WF2DMAP_OBJPM_ST );
							npc_id		= WFLBY_3DOBJCONT_GetWkObjData( p_npc, WF2DMAP_OBJPM_PLID );

							if( npc_id != WFLBY_MAPPERSON_PIERROT_ID ){
								talk_busy	= WFBLY_ROOM_CheckNpcTalkBusy( p_rmwk, npc_id );
								// ��Ԃ�NONE����Ȃ��Ƙb���������Ȃ�
								if( (status == WF2DMAP_OBJST_NONE) && (talk_busy == FALSE) ){	
									WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_event, plno, 
											&sc_WFLBY_EV_DEF_TALK_A, NULL );
									// �C�x���g���������瓮����ꎞ��~
									WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVENONE );
									WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENONE );

									WFLBY_ROOM_GadGet_SetStop( p_rmwk, TRUE );	// �{�^�������Ȃ�
									return FALSE;
								}
							}
						}
					}

					// �����`�F�b�N
					if( WFLBY_MAPCONT_Event_CheckFrontDouzou( wayevent ) == TRUE ){
						u32 way;
						// ��������������Ă���K�v������
						way = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_WAY );
						if( way == WF2DMAP_WAY_UP ){
						
							// �����𓮂���
							WFLBY_3DMAPOBJCONT_MAP_StartDouzouAnm( p_mapobjcont, 
									WF2DMAP_POS2GRID(waypos.x),
									WF2DMAP_POS2GRID(waypos.y) );

							// �N�������C�x���g��ݒ�
							WFLBY_ROOM_SetEvNo( p_rmwk, wayevent );

							WFLBY_ROOM_GadGet_SetStop( p_rmwk, TRUE );	// �{�^�������Ȃ�

							return FALSE;
						}
					}

					// �ڂ̑O�ɃC�x���g�����邩�`�F�b�N
					if( WFLBY_MAPCONT_Event_CheckFrontDecide( wayevent ) == TRUE ){

						// ���̃C�x���g����
						WFLBY_EV_DEF_Player_StartEventTbl( p_wk, p_rmwk, plno, 
								sc_WFLBY_EV_DEF_EVENT_ASTART, wayevent );
						// �C�x���g���������瓮����ꎞ��~
						WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVENONE );

						// �N�������C�x���g��ݒ�
						WFLBY_ROOM_SetEvNo( p_rmwk, wayevent );

						WFLBY_ROOM_GadGet_SetStop( p_rmwk, TRUE );	// �{�^�������Ȃ�

						// ���̃C�x���g�N��
						return FALSE;	
					}
				}

				// �����Ă�������ɃL�[�������Ă��邩
				if( (WFLBY_EV_DEF_WayKeyInput( way ) == TRUE) ){
					
					// ����Ɍ����Ă����������ł���K�v������
					if( ( way == WF2DMAP_WAY_UP ) ){ 
						if( WFLBY_MAPCONT_Event_CheckFrontKey( wayevent ) == TRUE ){

							// ���̃C�x���g����
							WFLBY_EV_DEF_Player_StartEventTbl( p_wk, p_rmwk, plno, 
									sc_WFLBY_EV_DEF_EVENT_WAYUPSTART, wayevent );
							
							// �C�x���g���������瓮����ꎞ��~
							WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVENONE );


							// �N�������C�x���g��ݒ�
							WFLBY_ROOM_SetEvNo( p_rmwk, wayevent );

							WFLBY_ROOM_GadGet_SetStop( p_rmwk, TRUE );	// �{�^�������Ȃ�

							//  ���̃C�x���g���N��
							return FALSE;	
						}
					}

					// ���̕������l������Ȃ����`�F�b�N
					{
						WFLBY_3DPERSON* p_npc;
						u32 npc_id;
						u32 chara_id;
						
						p_npc		= WFLBY_3DOBJCONT_GetFrontPerson( p_objcont, p_player );

						// �v���t�B�[���̕\����
						if( p_npc ){

							// ���ꂪ�A�s�G�������l���`�F�b�N
							npc_id		= WFLBY_3DOBJCONT_GetWkObjData( p_npc, WF2DMAP_OBJPM_PLID );
							if( npc_id != WFLBY_MAPPERSON_PIERROT_ID ){

								// �\�����ł͂Ȃ����`�F�b�N
								if( WFLBY_ROOM_UNDERWIN_TrCardGetDraw( p_rmwk ) == FALSE ){
									if( WFBLY_ROOM_CheckNpcInfoDrawBusy( p_rmwk, npc_id ) == FALSE ){
										WFLBY_ROOM_UNDERWIN_TrCardOn( p_rmwk, npc_id, TRUE );

										// InfoDraw���r�W�[��Ԃł͂Ȃ���NPC�̓�����v���C���[�������삵�Ă����v
										//
										// ��l���͓��삷�邩�A��b�����Ȃ�����Ⴄ�C�x���g�ɂ͔�΂Ȃ��A
										// ��b�̂Ƃ���NPC���͏��������̂ŁA
										// infodraw_retmvtype�̒l�͎����Ă����v

										// ���̐l�̓����ۑ����Ă���
										p_plwk->infodraw_retmvtype = WFLBY_3DOBJCONT_GetWkMove( p_npc );
										// �����~
										WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENONE );	

										return FALSE;
									}
								}
							}
							
						}
					}
				}


				// X�{�^�����������玩���̃v���t�B�[����\��
				if( sys.trg & PAD_BUTTON_X ){
					// �X�C�b�`�\��
					if( WFLBY_ROOM_UNDERWIN_TrCardGetDraw( p_rmwk ) == FALSE ){
						WFLBY_ROOM_UNDERWIN_TrCardOn( p_rmwk, WFLBY_SYSTEM_GetMyIdx( p_system ), FALSE );
						Snd_SePlay( WFLBY_SND_PROFILE );
					}else{
						u32 trcard_plidx;
						// �������`�F�b�N
						trcard_plidx = WFLBY_ROOM_UNDERWIN_TrCardGetPlIdx( p_rmwk );
						if( trcard_plidx == plno ){
							WFLBY_ROOM_UNDERWIN_TrCardOff( p_rmwk );
							Snd_SePlay( WFLBY_SND_PROFILE );
						}
					}
					return FALSE;
				}
			}
		}
		break;

	}

	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l�����샊�N�G�X�g�ݒ��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerMain_After( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	u32 st;
	WFLBY_3DPERSON* p_player;
	WFLBY_3DOBJCONT* p_objcont;

	// �v���C���[���擾
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_player = WFLBY_3DOBJCONT_GetPlayer( p_objcont );
	
	// �v���C���[�X�e�[�^�X��NONE�ӊO�ɂȂ�����^�b�`�g�C������OFF
	st		= WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_ST );

	if( st != WF2DMAP_OBJST_NONE ){
		// �^�b�`�g�C�����Ȃ�
		WFLBY_ROOM_GadGet_SetStop( p_rmwk, TRUE );

#if PL_T0867_080716_FIX
		// �ړ����Ȃ̂ŁA���l����b�W�����v����NG
		WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, TRUE );
#endif
	}

	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�o������ޏ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerOutNormal_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// �ގ��`�F�b�N���b�Z�[�W�J�n
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_MSGSTART:			
		// �����~
		{
			WFLBY_3DOBJCONT* p_objcont;
			WFLBY_3DPERSON* p_person;

			p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
			p_person	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_person, WFLBY_3DOBJCONT_MOVENONE );
			
		}

		// ��ԕύX
		// �͂Ȃ��������Ȃ��悤�ɕύX
		WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), WFLBY_STATUS_LOGOUT );

		Snd_SePlay( WFLBY_SND_MINIGAME );

		{
			STRBUF* p_str;

			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_wifi_hiroba_01_03 );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	// �ގ��`�F�b�N���b�Z�[�W�҂�
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_MSGWAIT:	
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	// �ގ��`�F�b�N���b�Z�[�W�J�n
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_MSGSTART01:		
		{
			STRBUF* p_str;

			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_ERR, dwc_message_0010 );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
	
	// �ގ��`�F�b�N���b�Z�[�W�҂�
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_MSGWAIT01:		
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
		
	// YESNO�E�B���h�E�\��
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_YESNO:			
		WFLBY_ROOM_YESNOWIN_StartNo( p_rmwk );
		WFLBY_EVENTWK_AddSeq( p_wk );
		break;
		
	// YESNO�I��҂�
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_YESNOWAIT:		
		{
			u32 result;
			BOOL list_end = FALSE;

			result = WFLBY_ROOM_YESNOWIN_Main( p_rmwk );
			switch( result ){
			case WFLBY_ROOM_YESNO_OK:	// �͂�
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_FLYUP );
				list_end = TRUE;
				break;
				
			case WFLBY_ROOM_YESNO_NO:	// ������
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_RET );
				list_end = TRUE;
				break;
				
			default:
			case WFLBY_ROOM_YESNO_WAIT:	// �I�ђ�
				break;
			}

			if( list_end == TRUE ){
				WFLBY_ROOM_YESNOWIN_End( p_rmwk );
				WFLBY_ROOM_TALKWIN_Off( p_rmwk );
			}
		}
		break;

	// �ގ����
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_FLYUP:			

		// �ʐM�G���[�`�F�b�N	OFF
		WFLBY_ROOM_SetErrorCheckFlag( p_rmwk, FALSE );
		
		{
			WFLBY_CAMERA* p_camera;
			WFLBY_3DOBJCONT* p_objcont;
			WFLBY_3DPERSON* p_person;

			p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
			p_person	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );

			// �J��������֘A�t�����͂�
			p_camera = WFLBY_ROOM_GetCamera( p_rmwk );
			WFLBY_CAMERA_ResetTargetPerson( p_camera );

			// ��ɒ���
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_person, WFLBY_3DOBJCONT_MOVEFLYUP );
		}

		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_END );
		break;

	// �ގ�
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_END:				
		{
			WFLBY_3DOBJCONT* p_objcont;
			WFLBY_3DPERSON* p_person;

			p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
			p_person	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );

			if( WFLBY_3DOBJCONT_CheckWkMoveEnd( p_person ) == TRUE ){
				WFLBY_ROOM_SetEndFlag( p_rmwk, TRUE );
				WFLBY_ROOM_SetEndRet( p_rmwk, WFLBY_ROOM_RET_ROOM_OUT );

				// ��ԕύX
				WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), WFLBY_STATUS_LOGOUT );

				return TRUE;	// �I��
			}
		}
		break;
		
	// �߂�
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_RET:
		// ��ɕ�������
		{
			WFLBY_3DOBJCONT* p_objcont;
			WFLBY_3DPERSON* p_person;

			p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
			p_person	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_person, WF2DMAP_CMD_WALK, WF2DMAP_WAY_UP );
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	// �߂芮���܂�
	case WFLBY_EV_DEF_PLAYER_OUTNORMAL_SEQ_RETWAIT:			
		// ����I����҂��āA�ʏ�C�x���g�ɖ߂�
		{
			WFLBY_3DOBJCONT* p_objcont;
			WFLBY_3DPERSON* p_person;
			WFLBY_EVENT* p_event;
			u32 status;

			// ��ԕύX
			// ���ɖ߂�
			WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), WFLBY_STATUS_LOGIN );

			p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
			p_event		= WFLBY_ROOM_GetEvent( p_rmwk );
			p_person	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			status		= WFLBY_3DOBJCONT_GetWkObjData( p_person, WF2DMAP_OBJPM_ST );
			if( status == WF2DMAP_OBJST_NONE ){
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_person, WFLBY_3DOBJCONT_MOVEPLAYER );
				// �C�x���g�ݒ�
				WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERMAIN, NULL );
				
			}
		}
		break;
	}

	return FALSE;	
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���v�@���r�[�j���[�X�@����BOARD��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerOutSubChan_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_3DOBJCONT*	p_objcont;
	WFLBY_3DPERSON*		p_player;
	WFLBY_SYSTEM*		p_system;
	u32 evno;
	WFLBY_EV_DEF_PLAYER_SUBCHAN_WK* p_evwk;

	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_player	= WFLBY_3DOBJCONT_GetPlIDWk( p_objcont, plno );
	evno		= WFLBY_ROOM_GetEvNo( p_rmwk );
	p_evwk		= WFLBY_EVENTWK_GetWk( p_wk );
	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// �T�u�`�����l���փ��O�C��
	case WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_LOGIN:

		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_DEF_PLAYER_SUBCHAN_WK) );

		{
			u8 channelno;

			// �`�����l���i���o�[����~�j�Q�[���^�C�v���쐬
			channelno = WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoChannel( evno );
			switch( channelno ){
			case DWC_LOBBY_SUBCHAN_FOOT1:			// ���Ճ{�[�h�P
				p_evwk->minigame = WFLBY_GAME_FOOTWHITE;
				break;
			case DWC_LOBBY_SUBCHAN_FOOT2:			// ���Ճ{�[�h�Q
				p_evwk->minigame = WFLBY_GAME_FOOTBLACK;
				break;
			case DWC_LOBBY_SUBCHAN_CLOCK:			// ���E���v
				p_evwk->minigame = WFLBY_GAME_WLDTIMER;
				break;
			case DWC_LOBBY_SUBCHAN_NEWS:				// ���r�[�j���[�X
				p_evwk->minigame = WFLBY_GAME_NEWS;
				break;
			default:
				GF_ASSERT(0);
				p_evwk->minigame = WFLBY_GAME_WLDTIMER;
				break;
			}
		}

		// ��������Ȃ���Ԃ���Ȃ����`�F�b�N
		if( WFLBY_SYSTEM_Event_GetEndCM( p_system ) == TRUE ){
			p_evwk->msg_idx		= msg_wifi_h_info_05_03;
			p_evwk->msg_type	= WFLBY_DEFMSG_TYPE_INFO;
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_CLOSE );
			break;
		}

		// �ʐM�ߏ��Ԃ���Ȃ����`�F�b�N
		if( WFLBY_SYSTEM_GetExcessFoodError( p_system ) == TRUE ){
			p_evwk->msg_idx		= msg_hiroba_end_03;
			p_evwk->msg_type	= WFLBY_DEFMSG_TYPE_HIROBA;
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_CLOSE );
			break;
		}


		{
			BOOL result;
			u8 channel_no;

			channel_no = WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoChannel( evno );

			// �T�u�`�����l���ڑ��l���`�F�b�N
			if( DWC_LOBBY_SUBCHAN_LoginCheck( channel_no ) == FALSE ){
				p_evwk->msg_idx = msg_wifi_h_info_05_01;
				p_evwk->msg_type	= WFLBY_DEFMSG_TYPE_INFO;
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NG );
				break;
			}

			
			result = DWC_LOBBY_SUBCHAN_Login( channel_no );
			GF_ASSERT( result );
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
		
	//  ���O�C���ҋ@
	case WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_LOGINWAIT:
		{
			DWC_LOBBY_SUBCHAN_LOGIN_RESULT result;

			result = DWC_LOBBY_SUBCHAN_LoginWait();
			if( result == DWC_LOBBY_SUBCHAN_LOGIN_OK ){
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_START );

				// �~�j�Q�[���Q���g�s�b�N����
				{
					u8 subchan_num;
					u8 minigame;
					int i;
					u8 pl_no[ NEWS_TOPICNAME_NUM ] = {0};
					u32 enumplno;
					u8 count;
					WFLBY_SYSTEM*		p_system;

					// �V�X�e�����[�N�擾
					p_system = WFLBY_ROOM_GetSystemData( p_rmwk );

					// �T�u�`�����l���ɎQ�����Ă���l�̐�
					subchan_num = WFLBY_SYSTEM_GetSubChanPlNum( p_system );


					if( (p_evwk->minigame == WFLBY_GAME_WLDTIMER) ||
						(p_evwk->minigame == WFLBY_GAME_NEWS) ){
						// �Q���҃��X�g�쐬
						count = 0;
						for( i=0; i<NEWS_TOPICNAME_NUM; i++ ){
							enumplno = WFLBY_SYSTEM_GetSubChanEnum( p_system, i );
							if( enumplno != DWC_LOBBY_USERIDTBL_IDX_NONE ){
								if( count < NEWS_TOPICNAME_NUM ){
									pl_no[ count ] = enumplno;
								}
								count ++;
							}
						}
						OS_TPrintf( "check subchan_num=%d  in_num=%d\n", subchan_num, count );
					}else{
						pl_no[0] = plno;
					}

					WFLBY_SYSTEM_TOPIC_SendMiniGame( p_system, p_evwk->minigame, subchan_num, 
							pl_no[0], pl_no[1], pl_no[2], pl_no[3], FALSE );
				}
				
			// ���O�C�����s
			}else if( result == DWC_LOBBY_SUBCHAN_LOGIN_NG ){
				p_evwk->msg_idx = msg_wifi_h_info_05_01;
				p_evwk->msg_type	= WFLBY_DEFMSG_TYPE_INFO;
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NG );
			}
		}
		break;
		
	// �A�v���J�n
	case WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_START:
		{
			u32 endret;
			u32 status;
			u32 played_type;

			endret = WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoRetNum( evno );
			status = WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoStatus( evno );
			
			WFLBY_ROOM_SetEndFlag( p_rmwk, TRUE );
			WFLBY_ROOM_SetEndRet( p_rmwk, endret );

			// ��ԕύX
			WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), status );

			// �{�E�P���m�[�g
			WFLBY_SYSTEM_FNOTE_SetPlayMinigame( p_system, p_evwk->minigame );

			// �V�񂾏��o�b�t�@
			switch( p_evwk->minigame ){	
			case WFLBY_GAME_FOOTWHITE:	// ���Ճ{�[�h�@��
			case WFLBY_GAME_FOOTBLACK:	// ���Ճ{�[�h�@��
				played_type = WFLBY_PLAYED_FOOTBOARD;
				break;
				
			case WFLBY_GAME_WLDTIMER:	// ���E���v
				played_type = WFLBY_PLAYED_WORLDTIMER;
				break;
				
			case WFLBY_GAME_NEWS:		// ���r�[�j���[�X
				played_type = WFLBY_PLAYED_TOPIC;
				break;
			default:
				GF_ASSERT(0);	// ���肦�Ȃ�
				break;
			}
			WFLBY_SYSTEM_PLAYED_SetPlay( p_system, played_type );

			// �o���p�^�[���ݒ�
			{
				WF2DMAP_POS	pos;
				WFLBY_ROOM_SAVE* p_save;
				WF2DMAP_WAY	way;	

				// �v���C���[�̌����Ă�������̋t�����ɏo��
				way = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_WAY );
				way = WF2DMPA_OBJToolRetWayGet( way );

				p_save		= WFLBY_ROOM_GetSaveData( p_rmwk );

				// ���̈ʒu�̂P�����̈ʒu�ɏo��
				pos = WFLBY_3DOBJCONT_GetWkPos( p_player );

				p_save->pl_inevno	= WFLBY_EV_DEF_PLAYERIN_GRID;
				p_save->pl_gridx	= WF2DMAP_POS2GRID( pos.x );
				p_save->pl_gridy	= WF2DMAP_POS2GRID( pos.y );
				p_save->pl_way		= way;
				p_save->minigame	= p_evwk->minigame; 
			}



			// ���[�N�j��
			WFLBY_EVENTWK_DeleteWk( p_wk );

			return TRUE;	// �I��
		}
		break;
		
	// �T�u�`�����l���ɓ���Ȃ������Ƃ�,�͂������
	case WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NG:
		{
			u8 moveno;
			u8 way;
			

			// �͂����ꓮ�������
			// �v���C���[�̌���������������߂�
			way = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_WAY );
			switch( way ){
			case WF2DMAP_WAY_UP:
				moveno = WFLBY_3DOBJCONT_MOVEROTADOWN;
				break;
			case WF2DMAP_WAY_DOWN:
				moveno = WFLBY_3DOBJCONT_MOVEROTAUP;
				break;
			case WF2DMAP_WAY_LEFT:
				moveno = WFLBY_3DOBJCONT_MOVEROTARIGHT;
				break;

			case WF2DMAP_WAY_RIGHT:
				moveno = WFLBY_3DOBJCONT_MOVEROTALEFT;
				break;

			default:
				GF_ASSERT(0);
				break;
			}
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, moveno );
			
			// ���슮���҂���
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	// �͂����ꓮ��҂�
	case WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NGWAIT:
		{
			BOOL				result;

			result = WFLBY_3DOBJCONT_CheckWkMoveEnd( p_player );
			if( result == TRUE ){

				// ���b�Z�[�W�\���܂ł̃E�G�C�g
				p_evwk->wait = WFLBY_EV_DEF_PLAYER_SUBCHAN_MSG_WAIT;

				// ���b�Z�[�W�\����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NGMSG );
			}
		}
		break;

	// �����������Ƃ�
	case WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_CLOSE:
		{
			u8 way;
			

			// �͂����ꓮ�������
			// �v���C���[�̌���������������߂�
			way = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_WAY );
			way = WF2DMPA_OBJToolRetWayGet( way );
			WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_player, WF2DMAP_CMD_WALK, way );

			// ���슮���҂���
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
		
	// �����������Ƃ�
	case WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_CLOSEWAIT:
		{
			u32 st;

			st = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_ST );
			if( st == WF2DMAP_OBJST_NONE ){
				// ���b�Z�[�W�\���܂ł̃E�G�C�g
				p_evwk->wait = WFLBY_EV_DEF_PLAYER_SUBCHAN_MSG_WAIT;

				// ���b�Z�[�W�\����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NGMSG );
			}
		}
		break;

	// ���b�Z�[�W�\��
	case WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NGMSG:
		p_evwk->wait --;
		if( p_evwk->wait <= 0 ){	
			STRBUF*				p_str;			
			// ���b�Z�[�W�̕\��
			WFLBY_ROOM_MSG_SetMinigame( p_rmwk, p_evwk->minigame, 0 );
			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, p_evwk->msg_type, p_evwk->msg_idx );
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
			// ���b�Z�[�W�����҂�
			WFLBY_EVENTWK_AddSeq( p_wk );

#if PL_T0867_080716_FIX
			// �����I�[�g�ړ����Ȃ��̂ŁA���l����b�W�����v����OK
			WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif
		}
		break;
	
	// ���b�Z�[�W�I���҂�
	case WFLBY_EV_DEF_PLAYER_SUBCHAN_SEQ_NGMSGWAIT:
		{
			BOOL result;

			result = WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk );
			if( result ){

				// ���[�N�j��
				WFLBY_EVENTWK_DeleteWk( p_wk );
				
				// ���Ƃ̃v���C���[�C�x���g�ɖ߂�
				// ��{����̊֐���ݒ�
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );

				// ���b�Z�[�WOFF
				WFLBY_ROOM_TALKWIN_Off( p_rmwk );

				// �v���C���[���C���풓�C�x���g��ݒ�
				{
					WFLBY_EVENT* p_event;
					p_event = WFLBY_ROOM_GetEvent( p_rmwk );
					WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERMAIN, NULL );
				}
			}
		}
		break;
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���ɍs��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerOutMiniGame_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_PLAYER_MG_WK* p_evwk;
	WFLBY_EVENT* p_event;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_3DPERSON* p_player;
	WFLBY_SYSTEM* p_system;

	p_evwk = WFLBY_EVENTWK_GetWk( p_wk );

	// �V�X�e�����[�N�擾
	p_event		= WFLBY_ROOM_GetEvent( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	case WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_START:
		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_DEF_PLAYER_MG_WK) );

#ifdef WFLBY_DEBUG_ROOM_MINIGAME_AUTO
		WFLBY_DEBUG_ROOM_MINIGAME_AUTO_FLAG = TRUE;
#endif

		// ��l�������~
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVENONE );

		// �V�[�P���X��i�߂Ă���
		WFLBY_EVENTWK_AddSeq( p_wk );

		// ���荞�݃C�x���g����
		p_evwk->mg_param.in_ok		= WFLBY_EV_MG_RET_NG_NUM;
		{
			u32 evno;
			
			evno = WFLBY_ROOM_GetEvNo( p_rmwk );

			switch( evno ){
			case WFLBY_MAPEVID_EV_BALLSLOW:
				p_evwk->mg_param.mg_type		= DWC_LOBBY_MG_BALLSLOW;
				p_evwk->mg_param.wflby_mg_type	= WFLBY_GAME_BALLSLOW;
				p_evwk->mg_param.wflby_mg_status= WFLBY_STATUS_BALLSLOW;
				break;
			case WFLBY_MAPEVID_EV_BALANCE:
				p_evwk->mg_param.mg_type	= DWC_LOBBY_MG_BALANCEBALL;
				p_evwk->mg_param.wflby_mg_type	= WFLBY_GAME_BALANCEBALL;
				p_evwk->mg_param.wflby_mg_status= WFLBY_STATUS_BALANCEBALL;
				break;
			case WFLBY_MAPEVID_EV_BALLOON:
				p_evwk->mg_param.mg_type	= DWC_LOBBY_MG_BALLOON;
				p_evwk->mg_param.wflby_mg_type	= WFLBY_GAME_BALLOON;
				p_evwk->mg_param.wflby_mg_status= WFLBY_STATUS_BALLOON;
				break;
			default:
				GF_ASSERT(0);
				break;
			}

			// �}�b�`���O���ɏ�ԕύX
			WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), WFLBY_STATUS_MATCHING );
		}
		
		WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYER_MGSTART, &p_evwk->mg_param );
		
		break;
		
	case WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_WAIT:
		// �~�j�Q�[���ɎQ���ł������`�F�b�N
		if( p_evwk->mg_param.in_ok == WFLBY_EV_MG_RET_OK ){
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_END );
		}else{
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_RET );
		}
		break;
		
	// �~�j�Q�[���ɐi��
	case WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_END:
		{
			u32 endret;
			u32 status;
			u32 evno;
			u32 played_type;
			
			evno = WFLBY_ROOM_GetEvNo( p_rmwk );

			switch( evno ){
			case WFLBY_MAPEVID_EV_BALLSLOW:
				endret		= WFLBY_ROOM_RET_ROOM_BALLSLOW;
				played_type = WFLBY_PLAYED_BALLTHROW;
				break;
			case WFLBY_MAPEVID_EV_BALANCE:
				endret		= WFLBY_ROOM_RET_ROOM_BALANCE;
				played_type = WFLBY_PLAYED_BALANCEBALL;
				break;
			case WFLBY_MAPEVID_EV_BALLOON:
				endret		= WFLBY_ROOM_RET_ROOM_BALLOON;
				played_type = WFLBY_PLAYED_BALLOON;
				break;
			}

			WFLBY_ROOM_SetEndFlag( p_rmwk, TRUE );
			WFLBY_ROOM_SetEndRet( p_rmwk, endret );

			// �{�E�P���m�[�g
			WFLBY_SYSTEM_FNOTE_SetPlayMinigame( p_system, p_evwk->mg_param.wflby_mg_type );

			// ���R�[�h
			WFLBY_SYSTEM_RECORD_AddMinigame( p_system );
			
			// �V�񂾏��o�b�t�@
			WFLBY_SYSTEM_PLAYED_SetPlay( p_system, played_type );

			// �o���p�^�[���ݒ�
			{
				WF2DMAP_POS	pos;
				WFLBY_ROOM_SAVE* p_save;
				WF2DMAP_WAY	way;	

				// �v���C���[�̌����Ă�������̋t�����ɏo��
				way = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_WAY );
				way = WF2DMPA_OBJToolRetWayGet( way );

				p_save		= WFLBY_ROOM_GetSaveData( p_rmwk );

				// ���̈ʒu�̂P�����̈ʒu�ɏo��
				pos = WFLBY_3DOBJCONT_GetWkPos( p_player );

				p_save->pl_inevno	= WFLBY_EV_DEF_PLAYERIN_MINIGAME;
				p_save->pl_gridx	= WF2DMAP_POS2GRID( pos.x );
				p_save->pl_gridy	= WF2DMAP_POS2GRID( pos.y );
				p_save->pl_way		= way;
				p_save->minigame	= p_evwk->mg_param.wflby_mg_type; 
			}


			// BGM�ύX�����b�N
			WFLBY_SYSTEM_SetLockBGMChange( WFLBY_ROOM_GetSystemData( p_rmwk ), TRUE );


			// ���[�N�j��
			WFLBY_EVENTWK_DeleteWk( p_wk );

			return TRUE;	// �I��
		}
		break;
		
	// �t�B�[���h�ɖ߂�
	case WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_RET:
		// �����Ă���t�����ɂP�O���b�h��������
		{
			WF2DMAP_POS	pos;
			WF2DMAP_WAY	way;	

			// �v���C���[�̌����Ă�������̋t�����ɏo��
			way = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_WAY );
			way = WF2DMPA_OBJToolRetWayGet( way );
			
			WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_player, WF2DMAP_CMD_WALK, way );
		
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
		
	// �����҂�
	case WFLBY_EV_DEF_PLAYER_MG_SEQ_INTR_RETWAIT:
		{
			u32 st;

			st = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_ST );
			if( st == WF2DMAP_OBJST_NONE ){

#if PL_T0867_080716_FIX
				// �����I�[�g�ړ����Ȃ��̂ŁA���l����b�W�����v����OK
				WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif
				
				WFLBY_EVENTWK_AddSeq( p_wk );
			}
		}
		break;

	// NG���b�Z�[�W�\��
	case WFLBY_EV_DEF_PLAYER_MG_SEQ_MSG:
		{
			STRBUF* p_str;
			u32 msgidx;
			BOOL msg_write = TRUE;

			switch( p_evwk->mg_param.in_ok ){
			case WFLBY_EV_MG_RET_NG_NUM:			// �l���������ς��œ���Ȃ�
				// �~�j�Q�[���������[�h�Z�b�g�ɐݒ�
				WFLBY_ROOM_MSG_SetMinigame( p_rmwk, p_evwk->mg_param.wflby_mg_type, 0 );
				msgidx = msg_wifi_h_info_01_04;
				break;
			case WFLBY_EV_MG_RET_NG_PLAYING:		// �Q�[�����Ȃ̂œ���Ȃ�
				msgidx = msg_wifi_h_info_01_05;
				break;
			case WFLBY_EV_MG_RET_NG_TIME:		// �~�j�Q�[����I�����Ă���̂œ���Ȃ�
				// �~�j�Q�[���������[�h�Z�b�g�ɐݒ�
				WFLBY_ROOM_MSG_SetMinigame( p_rmwk, p_evwk->mg_param.wflby_mg_type, 0 );
				msgidx = msg_wifi_h_info_01_06;
				break;
			case WFLBY_EV_MG_RET_NG_MIN:			// �K�v�Ȑl���������Ȃ�����
				msgidx = msg_wifi_h_info_04_01;
				break;
			case WFLBY_EV_MG_RET_NG_DISCON:		// �N�����ؒf����
				msgidx = msg_wifi_h_info_02_05;
				break;
			case WFLBY_EV_MG_RET_NG_BCAN:		// BCancel����
				msgidx = msg_wifi_h_info_05_10;
				break;

			case WFLBY_EV_MG_RET_NG_LOCK:		// �ȑO�e�̂Ƃ���BCancel�����̂ŎQ�����b�N��
				msgidx = msg_wifi_h_info_01_07;
				break;
				
			default:
			case WFLBY_EV_MG_RET_NG_MY:			// ����f����
				msg_write = FALSE;
				break;
			}

			// �`��
			if( msg_write ){
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_INFO, msgidx );
				WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

				WFLBY_EVENTWK_AddSeq( p_wk );
			}else{
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_MG_SEQ_RET_NORMAL );
			}
		}
		break;
		
	// ���b�Z�[�W�I���҂�
	case WFLBY_EV_DEF_PLAYER_MG_SEQ_MSGWAIT:
		// ���b�Z�[�W�I���҂�
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){

			// ���b�Z�[�WOFF
			WFLBY_ROOM_TALKWIN_Off( p_rmwk );

			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	// ���ɖ߂�
	case WFLBY_EV_DEF_PLAYER_MG_SEQ_RET_NORMAL:
		// ���[�N�j��
		WFLBY_EVENTWK_DeleteWk( p_wk );

		// ��Ԃ�߂�
		WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), WFLBY_STATUS_LOGIN );
		
		// ���Ƃ̃v���C���[�C�x���g�ɖ߂�
		// ��{����̊֐���ݒ�
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );

		// �v���C���[���C���풓�C�x���g��ݒ�
		{
			WFLBY_EVENT* p_event;
			p_event = WFLBY_ROOM_GetEvent( p_rmwk );
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERMAIN, NULL );
		}
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�ɍs��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerOutAnketo( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_PLAYER_ANKETO_WK* p_evwk;
	WFLBY_EVENT* p_event;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_3DPERSON* p_player;
	WFLBY_SYSTEM* p_system;
	STRBUF* p_str;

	p_evwk = WFLBY_EVENTWK_GetWk( p_wk );

	// �V�X�e�����[�N�擾
	p_event		= WFLBY_ROOM_GetEvent( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// �A���P�[�g�C�x���g�J�n
	// �܂��̓A���P�[�g�̎�t���Ȃ̂��A���\���Ȃ̂��`�F�b�N
	case WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_START:

		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_DEF_PLAYER_SUBCHAN_WK) );


		// ��Ԃ��A���P�[�g�ɂ���
		WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), WFLBY_STATUS_ANKETO );

		// �I�����ĂȂ����`�F�b�N
#ifdef WFLBY_DEBUG_EV_DEF_ANKETO
		if( (WFLBY_SYSTEM_Event_GetEndCM( p_system ) == TRUE) || (sys.cont & PAD_BUTTON_L) ){
#else
		if( (WFLBY_SYSTEM_Event_GetEndCM( p_system ) == TRUE) ){
#endif
			p_evwk->ret_msg = msg_survey_monitor_a_10;
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_MOVE );
			break;
		}

		// ���ʕ\��
#ifdef WFLBY_DEBUG_EV_DEF_ANKETO
		if( (WFLBY_SYSTEM_Event_GetNeon( p_system ) == TRUE) || (sys.cont & PAD_BUTTON_SELECT) ){
#else
		if( (WFLBY_SYSTEM_Event_GetNeon( p_system ) == TRUE) ){
#endif
			// ���ʕ\���J�n
			p_evwk->apl_no = WFLBY_ROOM_RET_ROOM_ANKETO_OUTPUT;
		}else{

#ifdef WFLBY_DEBUG_EV_DEF_ANKETO
			p_evwk->apl_no = WFLBY_ROOM_RET_ROOM_ANKETO_INPUT;
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_START_ANKETO );
			break;
#endif
			
			// ���[���݁H
			if( WFLBY_SYSTEM_FLAG_GetAnketoInput( p_system ) == TRUE ){
				p_evwk->ret_msg = msg_survey_monitor_b_n03;
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_MOVE );
				break;
			}else{
				p_evwk->apl_no = WFLBY_ROOM_RET_ROOM_ANKETO_INPUT;
			}
		}
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_START_ANKETO );
		break;

	// �A���P�[�g�֐i��
	case WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_START_ANKETO:
		{
			u32 endret;
			u32 status;
			
			if( p_evwk->apl_no == WFLBY_ROOM_RET_ROOM_ANKETO_INPUT ){
				endret		= WFLBY_ROOM_RET_ROOM_ANKETO_INPUT;
			}else{
				endret		= WFLBY_ROOM_RET_ROOM_ANKETO_OUTPUT;

				// �A���P�[�g�����t���O��ݒ�
				WFLBY_SYSTEM_FLAG_SetAnketoView( p_system );
			}

			WFLBY_ROOM_SetEndFlag( p_rmwk, TRUE );
			WFLBY_ROOM_SetEndRet( p_rmwk, endret );

			// ��������ƂɃA���P�[�g��ǉ�
			// tomoya 080618
			WFLBY_SYSTEM_SetLastAction( p_system, WFLBY_LASTACTION_ANKETO, DWC_LOBBY_INVALID_USER_ID );

			// �o���p�^�[���ݒ�
			{
				WF2DMAP_POS	pos;
				WFLBY_ROOM_SAVE* p_save;
				WF2DMAP_WAY	way;	

				// �v���C���[�̌����Ă�������̋t�����ɏo��
				way = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_WAY );
				way = WF2DMPA_OBJToolRetWayGet( way );

				p_save		= WFLBY_ROOM_GetSaveData( p_rmwk );

				// ���̈ʒu�̂P�����̈ʒu�ɏo��
				pos = WFLBY_3DOBJCONT_GetWkPos( p_player );

				p_save->pl_inevno	= WFLBY_EV_DEF_PLAYERIN_ANKETO;
				p_save->pl_gridx	= WF2DMAP_POS2GRID( pos.x );
				p_save->pl_gridy	= WF2DMAP_POS2GRID( pos.y );
				p_save->pl_way		= way;
				p_save->minigame	= 0; 
			}

			// ���[�N�j��
			WFLBY_EVENTWK_DeleteWk( p_wk );

			return TRUE;	// �I��
		}
		break;

	// �߂�
	case WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_MOVE:
		// �����Ă���t�����ɂP�O���b�h��������
		{
			WF2DMAP_POS	pos;
			WF2DMAP_WAY	way;	

			// �v���C���[�̌����Ă�������̋t�����ɏo��
			way = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_WAY );
			way = WF2DMPA_OBJToolRetWayGet( way );
			
			WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_player, WF2DMAP_CMD_WALK, way );
		
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;
		
	// �߂�܂�
	case WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_WAIT:
		{
			u32 st;

			st = WFLBY_3DOBJCONT_GetWkObjData( p_player, WF2DMAP_OBJPM_ST );
			if( st == WF2DMAP_OBJST_NONE ){

#if PL_T0867_080716_FIX
				// �����I�[�g�ړ����Ȃ��̂ŁA���l����b�W�����v����OK
				WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif

				WFLBY_EVENTWK_AddSeq( p_wk );
			}
		}
		break;

	// �A���P�[�g�ւ͍s���Ȃ�
	case WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_MSG:
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, p_evwk->ret_msg );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		WFLBY_EVENTWK_AddSeq( p_wk );
		break;

	case WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_MSGWAIT:
		// ���b�Z�[�W�I���҂�
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){

			// ���b�Z�[�WOFF
			WFLBY_ROOM_TALKWIN_Off( p_rmwk );

			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	case WFLBY_EV_DEF_PLAYER_ANKETO_SEQ_RET_NORMAL:
		// ���[�N�j��
		WFLBY_EVENTWK_DeleteWk( p_wk );

		// ��Ԃ�߂�
		WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), WFLBY_STATUS_LOGIN );
		
		// ���Ƃ̃v���C���[�C�x���g�ɖ߂�
		// ��{����̊֐���ݒ�
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );

		// �v���C���[���C���풓�C�x���g��ݒ�
		{
			WFLBY_EVENT* p_event;
			p_event = WFLBY_ROOM_GetEvent( p_rmwk );
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERMAIN, NULL );
		}
		break;

	default:
		GF_ASSERT( 0 );
		break;
	}

	return FALSE;	// �I��
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�i���o�[����`�����l���i���o�[��Ԃ�
 *
 *	@param	evno	�C�x���g�i���o�[
 *
 *	@return	�`�����l���i���o�[
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoChannel( u32 evno )
{
	u32 channel_no;

	switch( evno ){
	case WFLBY_MAPEVID_EV_WLDTIMER_00:
	case WFLBY_MAPEVID_EV_WLDTIMER_01:
	case WFLBY_MAPEVID_EV_WLDTIMER_02:
	case WFLBY_MAPEVID_EV_WLDTIMER_03:
		channel_no = DWC_LOBBY_SUBCHAN_CLOCK;
		break;

	case WFLBY_MAPEVID_EV_TOPIC_00:
	case WFLBY_MAPEVID_EV_TOPIC_01:
	case WFLBY_MAPEVID_EV_TOPIC_02:
	case WFLBY_MAPEVID_EV_TOPIC_03:
		channel_no = DWC_LOBBY_SUBCHAN_NEWS;
		break;

	case WFLBY_MAPEVID_EV_FOOT_00_00:
	case WFLBY_MAPEVID_EV_FOOT_00_01:
	case WFLBY_MAPEVID_EV_FOOT_00_02:
	case WFLBY_MAPEVID_EV_FOOT_00_03:
	case WFLBY_MAPEVID_EV_FOOT_00_04:
	case WFLBY_MAPEVID_EV_FOOT_00_05:
	case WFLBY_MAPEVID_EV_FOOT_00_06:
	case WFLBY_MAPEVID_EV_FOOT_00_07:
		channel_no = DWC_LOBBY_SUBCHAN_FOOT1;
		break;

	case WFLBY_MAPEVID_EV_FOOT_01_00:
	case WFLBY_MAPEVID_EV_FOOT_01_01:
	case WFLBY_MAPEVID_EV_FOOT_01_02:
	case WFLBY_MAPEVID_EV_FOOT_01_03:
	case WFLBY_MAPEVID_EV_FOOT_01_04:
	case WFLBY_MAPEVID_EV_FOOT_01_05:
	case WFLBY_MAPEVID_EV_FOOT_01_06:
	case WFLBY_MAPEVID_EV_FOOT_01_07:
		channel_no = DWC_LOBBY_SUBCHAN_FOOT2;
		break;

	default:
		GF_ASSERT(0);
		channel_no = 0;
		break;
	}

	return channel_no;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ɉ���������̂���ɕԂ��l���擾����
 *
 *	@param	evno	�C�x���g�i���o�[
 *
 *	@return	�Ԃ��l
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoRetNum( u32 evno )
{
	u32 ret_num;

	switch( evno ){
	case WFLBY_MAPEVID_EV_WLDTIMER_00:
	case WFLBY_MAPEVID_EV_WLDTIMER_01:
	case WFLBY_MAPEVID_EV_WLDTIMER_02:
	case WFLBY_MAPEVID_EV_WLDTIMER_03:
		ret_num = WFLBY_ROOM_RET_ROOM_WORLDTIMER;
		break;

	case WFLBY_MAPEVID_EV_TOPIC_00:
	case WFLBY_MAPEVID_EV_TOPIC_01:
	case WFLBY_MAPEVID_EV_TOPIC_02:
	case WFLBY_MAPEVID_EV_TOPIC_03:
		ret_num = WFLBY_ROOM_RET_ROOM_TOPIC;
		break;

	case WFLBY_MAPEVID_EV_FOOT_00_00:
	case WFLBY_MAPEVID_EV_FOOT_00_01:
	case WFLBY_MAPEVID_EV_FOOT_00_02:
	case WFLBY_MAPEVID_EV_FOOT_00_03:
	case WFLBY_MAPEVID_EV_FOOT_00_04:
	case WFLBY_MAPEVID_EV_FOOT_00_05:
	case WFLBY_MAPEVID_EV_FOOT_00_06:
	case WFLBY_MAPEVID_EV_FOOT_00_07:
		ret_num = WFLBY_ROOM_RET_ROOM_FOOT1;
		break;

	case WFLBY_MAPEVID_EV_FOOT_01_00:
	case WFLBY_MAPEVID_EV_FOOT_01_01:
	case WFLBY_MAPEVID_EV_FOOT_01_02:
	case WFLBY_MAPEVID_EV_FOOT_01_03:
	case WFLBY_MAPEVID_EV_FOOT_01_04:
	case WFLBY_MAPEVID_EV_FOOT_01_05:
	case WFLBY_MAPEVID_EV_FOOT_01_06:
	case WFLBY_MAPEVID_EV_FOOT_01_07:
		ret_num = WFLBY_ROOM_RET_ROOM_FOOT2;
		break;

	default:
		GF_ASSERT(0);
		ret_num = 0;
		break;
	}

	return ret_num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�i���o�[�����Ԓ萔���擾����
 *
 *	@param	evno	�C�x���g�i���o�[
 *		
 *	@return	��Ԓ萔
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_EV_DEF_PlayerOutSubChan_GetEvnoStatus( u32 evno )
{
	u32 ret_num;

	switch( evno ){
	case WFLBY_MAPEVID_EV_WLDTIMER_00:
	case WFLBY_MAPEVID_EV_WLDTIMER_01:
	case WFLBY_MAPEVID_EV_WLDTIMER_02:
	case WFLBY_MAPEVID_EV_WLDTIMER_03:
		ret_num = WFLBY_STATUS_WORLDTIMER;
		break;

	case WFLBY_MAPEVID_EV_TOPIC_00:
	case WFLBY_MAPEVID_EV_TOPIC_01:
	case WFLBY_MAPEVID_EV_TOPIC_02:
	case WFLBY_MAPEVID_EV_TOPIC_03:
		ret_num = WFLBY_STATUS_TOPIC;
		break;

	case WFLBY_MAPEVID_EV_FOOT_00_00:
	case WFLBY_MAPEVID_EV_FOOT_00_01:
	case WFLBY_MAPEVID_EV_FOOT_00_02:
	case WFLBY_MAPEVID_EV_FOOT_00_03:
	case WFLBY_MAPEVID_EV_FOOT_00_04:
	case WFLBY_MAPEVID_EV_FOOT_00_05:
	case WFLBY_MAPEVID_EV_FOOT_00_06:
	case WFLBY_MAPEVID_EV_FOOT_00_07:
		ret_num = WFLBY_STATUS_FOOTBOAD00;
		break;

	case WFLBY_MAPEVID_EV_FOOT_01_00:
	case WFLBY_MAPEVID_EV_FOOT_01_01:
	case WFLBY_MAPEVID_EV_FOOT_01_02:
	case WFLBY_MAPEVID_EV_FOOT_01_03:
	case WFLBY_MAPEVID_EV_FOOT_01_04:
	case WFLBY_MAPEVID_EV_FOOT_01_05:
	case WFLBY_MAPEVID_EV_FOOT_01_06:
	case WFLBY_MAPEVID_EV_FOOT_01_07:
		ret_num = WFLBY_STATUS_FOOTBOAD01;
		break;

	default:
		GF_ASSERT(0);
		ret_num = 0;
		break;
	}

	return ret_num;
}


//----------------------------------------------------------------------------
/**
 *	@brief		��������o����
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_PlayerA_SWIN_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	
	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	case 0:
		{
			STRBUF* p_str;
			WFLBY_SYSTEM* p_system;

			p_system = WFLBY_ROOM_GetSystemData( p_rmwk );

			if( WFLBY_SYSTEM_Event_GetEndCM( p_system ) == TRUE ){
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_04 );
			}else{
				// PARADE�����`�F�b�N
				if( WFLBY_SYSTEM_Event_GetParade( p_system ) == TRUE ){
					p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoc_06 );
				}else{
					p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_03 );
				}
			}
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

			// �b��������ꂽ��
			Snd_SePlay( WFLBY_SND_STAFF );

			
			WFLBY_EVENTWK_AddSeq( p_wk );
		}
		break;

	case 1:
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) ){
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
		break;
	}

	return FALSE;
}






//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g��荞�݃C�x���g
 *
 *	@param	p_wk		���[�N
 *	@param	p_rmwk		�L�ꃏ�[�N
 *	@param	plno		�v���C���[NO
 *
 *	@retval	TRUE	�C�x���g�I��
 *	@retval	FALSE	�C�x���g�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_FLOAT_Inside( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_FLOAT_INSIDE*	p_evwk;
	WFLBY_3DMAPOBJ_CONT*	p_mapobjcont;
	WFLBY_3DOBJCONT*		p_objcont;
	WFLBY_CAMERA*			p_camera;
	WFLBY_SYSTEM*			p_system;
	BOOL					result;
	STRBUF*					p_str;

	p_objcont		= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_mapobjcont	= WFLBY_ROOM_GetMapObjCont( p_rmwk );
	p_camera		= WFLBY_ROOM_GetCamera( p_rmwk );
	p_system		= WFLBY_ROOM_GetSystemData( p_rmwk );
		
	// ���[�N�擾
	p_evwk = WFLBY_EVENTWK_GetWk( p_wk );
	
	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ������
	case WFLBY_EV_FLOAT_SEQ_INIT:
		{
			u32	evno;

			p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_FLOAT_INSIDE) );

			// ������l�擾
			p_evwk->p_obj = WFLBY_3DOBJCONT_GetPlIDWk( p_objcont, plno );

			evno	= WFLBY_ROOM_GetEvNo( p_rmwk );
			switch( evno ){
			case WFLBY_MAPEVID_EV_FLOAT00:
				p_evwk->station = WFLBY_FLOAT_STATION_LEFT;
				break;
				
			case WFLBY_MAPEVID_EV_FLOAT01:
				p_evwk->station = WFLBY_FLOAT_STATION_CENTER;
				break;
				
			case WFLBY_MAPEVID_EV_FLOAT02:
				p_evwk->station = WFLBY_FLOAT_STATION_RIGHT;
				break;

			default:
				GF_ASSERT( 0 );
				break;
			}
		}

		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_CHECK );
		break;

	// �\�񕔕�
	case WFLBY_EV_FLOAT_SEQ_FLOAT_CHECK:		
		// �t���[�g���n�܂��Ă�H
		if( WFLBY_SYSTEM_Event_GetParade( p_system ) == FALSE ){
			// �܂��͂��܂��ĂȂ� 1���������ă��b�Z�[�W
			p_evwk->next_seq = WFLBY_EV_FLOAT_SEQ_FLOAT_END_BEFORE;	
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_END_DOWN );
		}else{

			// �t���[�g���I����ĂȂ����`�F�b�N
			if( WFLBY_SYSTEM_Event_GetEndCM( p_system ) == TRUE ){
				// �t���[�g�I��
				p_evwk->next_seq = WFLBY_EV_FLOAT_SEQ_FLOAT_END_AFTER;	
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_END_DOWN );
			}else{

				// �\��ł��邩�`�F�b�N
				p_evwk->floatidx = WFLBY_SYSTEM_SetFloatReserve( p_system, plno, p_evwk->station );

				if( p_evwk->floatidx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
					// �\��ł����I
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_INWAIT );

					// ���b�Z�[�W�\�����^�C���E�G�C�g�ݒ�
					p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_INFO, msg_wifi_h_info_06_02 );
					WFLBY_ROOM_TALKWIN_PrintStrAllPut( p_rmwk, p_str );
					WFLBY_ROOM_TALKWIN_StartTimeWait( p_rmwk );
				}else{
					// �ł��Ȃ�
					p_evwk->next_seq = WFLBY_EV_FLOAT_SEQ_FLOAT_END_FULL;	
					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_END_DOWN );
				}
			}
		}
		break;

	// ����܂ő҂�
	case WFLBY_EV_FLOAT_SEQ_FLOAT_INWAIT:	

		// B�{�^����KEYDOWN��Cancel����
		if( (sys.trg & PAD_BUTTON_CANCEL) || (sys.trg & PAD_KEY_DOWN) ){
			// �\��Cancel
			WFLBY_SYSTEM_ResetFloatReserve( p_system, plno );

			// ���b�Z�[�WOFF
			WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );
			p_evwk->msg_off_ret_seq = WFLBY_EV_FLOAT_SEQ_FLOAT_END_DOWN;

			// 1���������Ēʏ�C�x���g�ɖ߂�
			p_evwk->next_seq = WFLBY_EV_FLOAT_SEQ_RET;

			// WINOFF->END_DOWN->RET
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_WINOFFWAIT );

		}else{
			
			// �ړI�̍��Ȃ��ڂ̑O�ɂ������я��
			if( WFLBY_EV_FLOAT_CheckJumpOn( p_evwk, p_mapobjcont ) == TRUE ){
				// ���b�Z�[�WOFF
				WFLBY_ROOM_TALKWIN_StopTimeWait( p_rmwk );

				// �t���[�g�ɏ�������Ƃ𑗐M
				WFLBY_SYSTEM_SendFloatOn( p_system );
				p_evwk->count = 0;	// �W�����v�J�n

				//�@�X�e�[�^�X���t���[�g�j����
				WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData( p_rmwk ), WFLBY_STATUS_FLOAT );

				// ����ȍ~�̓^�C���A�E�g�����͂Ȃ�
				WFLBY_ROOM_OffTimeOutCheck( p_rmwk );

				//  �J��������l���̍��W�����̂܂ܒǔ�����悤�ɕύX����B
				WFLBY_CAMERA_ResetTargetPerson( p_camera );

				// ��l���̕`����X�V���Ȃ��悤�ɕύX
				WFLBY_3DOBJCONT_DRAW_SetUpdata( p_evwk->p_obj, FALSE );

				// �W�����v�A�j���ݒ�
				WFLBY_3DOBJCONT_DRAW_SetAnmJump( p_evwk->p_obj, TRUE );
				
				// �W�����v��
				Snd_SePlay( WFLBY_SND_JUMP );
				
				// ������ʃt���[�g�ɕύX
				WFLBY_ROOM_UNDERWIN_FloatBttnOn( p_rmwk );

				// �W�����v������
				// WINOFF->JUMP
				p_evwk->msg_off_ret_seq = WFLBY_EV_FLOAT_SEQ_FLOAT_JUMP;
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_WINOFFWAIT );

				// �{�E�P���m�[�g
				WFLBY_SYSTEM_FNOTE_SetParade( p_system );
			}
		}
		break;
		
	// �W�����v
	case WFLBY_EV_FLOAT_SEQ_FLOAT_JUMP:
		// �ڂ̑O�ŁA�V�[�g�̈ʒu�܂ŃW�����v
		// �W�����v���̃J����������Ǘ�
		result = WFLBY_EV_FLOAT_JumpMove( p_evwk, p_mapobjcont, p_camera );
		if( result == TRUE ){

			// �W�����v�A�j����~
			WFLBY_3DOBJCONT_DRAW_SetAnmJump( p_evwk->p_obj, FALSE );

			// �O����������
			WFLBY_3DOBJCONT_DRAW_SetWay( p_evwk->p_obj, WF2DMAP_WAY_LEFT );

			// ���点��
			WFLBY_3DOBJCONT_DRAW_SetLight( p_evwk->p_obj, 1 << WFLBY_DRAW_LIGHT_FLOAT );
			
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_JUMPON );

			p_evwk->count = 0;
		}
		break;


	// ���n�A�j��
	case WFLBY_EV_FLOAT_SEQ_FLOAT_JUMPON:
		// �܂��͍��W�����킹��
		WFLBY_EV_FLOAT_MatrixSet( p_evwk, p_mapobjcont, p_rmwk );
		// ���̍��W���J�����ɐݒ�
		WFLBY_EV_FLOAT_SetCameraPos( p_evwk, p_camera );

		// ���n���W������ɐݒ�
		result = WFLBY_EV_FLOAT_JumpOnMove( p_evwk, p_mapobjcont );
		if( result == TRUE ){
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_MAIN );

			// ������ʃ{�^������J�n
			WFLBY_ROOM_GadGet_SetStop( p_rmwk, FALSE );
		}
		break;
		
	// ���C��	�{�^����������
	case WFLBY_EV_FLOAT_SEQ_FLOAT_MAIN:		
		// �I�u�W�F�̈ʒu��ݒ�
		{
			VecFx32 vec;
			
			//
			WFLBY_EV_FLOAT_MatrixSet( p_evwk, p_mapobjcont, p_rmwk );

			// �J�������W�ɂ�����
			WFLBY_EV_FLOAT_SetCameraPos( p_evwk, p_camera );

			// �\�����W���擾
			WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_evwk->p_obj, &vec );

			// ���炮��G�t�F�N�g�̒l��ݒ�
			WFLBY_EV_FLOAT_MatrixDonDonSet( p_evwk, p_mapobjcont, p_rmwk );

#ifdef WFLBY_DEBUG_EV_DEF_FLOAT_RETRY
			if( sys.trg & PAD_BUTTON_B ){
				WFLBY_ROOM_SAVE* p_save;

				// �\�񃊃Z�b�g
				WFLBY_SYSTEM_ResetFloatReserve( p_system, plno );

				p_save				= WFLBY_ROOM_GetSaveData( p_rmwk );
				p_save->pl_inevno	= WFLBY_EV_DEF_PLAYERIN_MINIGAME;
				p_save->pl_gridx	= 23;
				p_save->pl_gridy	= 8;
				p_save->pl_way		= WF2DMAP_WAY_DOWN;
				
				WFLBY_ROOM_SetEndFlag( p_rmwk, TRUE );
				WFLBY_ROOM_SetEndRet( p_rmwk, WFLBY_ROOM_RET_ROOM_RETURN );
				WFLBY_ROOM_TALKWIN_Off( p_rmwk );
				// ��ԕύX
				WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), WFLBY_STATUS_LOGIN );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_ENDWAIT );
				break;
			}
#endif
			

			// �ړ��̃��~�b�g�ɂ�����J�����̃X�N���[�����~�܂�A���̂܂܃t�F�[�h�A�E�g
			if( vec.x < WFLBY_EV_FLOAT_CAMERA_MOVE_END ){

				// �I����
				p_evwk->wait = 0;
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_END );
			}
		}
		break;

	// ���Ȃ��̂łP��������
	case WFLBY_EV_FLOAT_SEQ_FLOAT_END_DOWN:	
		WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_evwk->p_obj, WF2DMAP_CMD_WALK, WF2DMAP_WAY_DOWN );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_END_DOWNWAIT );
		break;

	// ���Ȃ��̂łP��������
	case WFLBY_EV_FLOAT_SEQ_FLOAT_END_DOWNWAIT:
		if( WFLBY_3DOBJCONT_GetWkObjData( p_evwk->p_obj, WF2DMAP_OBJPM_ST ) == WF2DMAP_OBJST_NONE ){


#if PL_T0867_080716_FIX
			// �����I�[�g�ړ����Ȃ��̂ŁA���l����b�W�����v����OK
			WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif
			WFLBY_EVENTWK_SetSeq( p_wk, p_evwk->next_seq );
		}
		break;

	// �t���[�g�O�Ȃ̂ŏ��Ȃ����b�Z�[�W
	case WFLBY_EV_FLOAT_SEQ_FLOAT_END_BEFORE:

		// ���b�Z�[�W�\��
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_INFO, msg_wifi_h_info_06_01 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �ʏ�̃C�x���g�ɖ߂�
		p_evwk->next_seq = WFLBY_EV_FLOAT_SEQ_RET;

		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_MSGWAIT );
		break;
	
	// �t���[�g�������ς��Ȃ̂ŏ��Ȃ�
	case WFLBY_EV_FLOAT_SEQ_FLOAT_END_FULL:
		// ���b�Z�[�W�\��
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_INFO, msg_wifi_h_info_06_03 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �ʏ�̃C�x���g�ɖ߂�
		p_evwk->next_seq = WFLBY_EV_FLOAT_SEQ_RET;

#if PL_T0867_080716_FIX
		// �����I�[�g�ړ����Ȃ��̂ŁA���l����b�W�����v����OK
		WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif

		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_MSGWAIT );
		break;

	// �p���[�h��Ȃ̂ŏ��Ȃ����b�Z�[�W
	case WFLBY_EV_FLOAT_SEQ_FLOAT_END_AFTER:	
		// ���b�Z�[�W�\��
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_INFO, msg_wifi_h_info_06_04 );
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );

		// �ʏ�̃C�x���g�ɖ߂�
		p_evwk->next_seq = WFLBY_EV_FLOAT_SEQ_RET;

#if PL_T0867_080716_FIX
		// �����I�[�g�ړ����Ȃ��̂ŁA���l����b�W�����v����OK
		WFLBY_ROOM_SetPlayerEventAfterMove( p_rmwk, FALSE );
#endif

		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_FLOAT_MSGWAIT );
		break;

	// ���b�Z�[�W�҂�
	case WFLBY_EV_FLOAT_SEQ_FLOAT_MSGWAIT:
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){
			WFLBY_EVENTWK_SetSeq( p_wk, p_evwk->next_seq );
		}
		break;

	// �E�B���h�E�n�e�e�҂�(TimeWait���n�e�e�ɂ���̂��������Ă���l�r�f���n�e�e���邽��)
	case WFLBY_EV_FLOAT_SEQ_FLOAT_WINOFFWAIT:
		WFLBY_ROOM_TALKWIN_Off( p_rmwk );
		WFLBY_EVENTWK_SetSeq( p_wk, p_evwk->msg_off_ret_seq );
		break;

	// �ʏ�I��
	case WFLBY_EV_FLOAT_SEQ_END:				//  �I��

		WFLBY_EV_FLOAT_MatrixSet( p_evwk, p_mapobjcont, p_rmwk );


		// ���炮��G�t�F�N�g�̒l��ݒ�
		WFLBY_EV_FLOAT_MatrixDonDonSet( p_evwk, p_mapobjcont, p_rmwk );


		
		p_evwk->wait ++;
		if( p_evwk->wait > 120 ){
		
			WFLBY_ROOM_SetEndFlag( p_rmwk, TRUE );
			WFLBY_ROOM_SetEndRet( p_rmwk, WFLBY_ROOM_RET_ROOM_OUT );
			
			WFLBY_ROOM_TALKWIN_Off( p_rmwk );

			// ��ԕύX
			WFLBY_SYSTEM_SetMyStatus( WFLBY_ROOM_GetSystemData(p_rmwk), WFLBY_STATUS_LOGOUT );

			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_FLOAT_SEQ_ENDWAIT );
		}	
		break;

	//  �I���҂�
	case WFLBY_EV_FLOAT_SEQ_ENDWAIT:			

		WFLBY_EV_FLOAT_MatrixSet( p_evwk, p_mapobjcont, p_rmwk );


		// ���炮��G�t�F�N�g�̒l��ݒ�
		WFLBY_EV_FLOAT_MatrixDonDonSet( p_evwk, p_mapobjcont, p_rmwk );

		break;

	//  �ʏ�̃C�x���g�ɖ߂�
	case WFLBY_EV_FLOAT_SEQ_RET:				
		{
			WFLBY_EVENT* p_event;

			p_event = WFLBY_ROOM_GetEvent( p_rmwk );

			WFLBY_ROOM_TALKWIN_Off( p_rmwk );

			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_evwk->p_obj, WFLBY_3DOBJCONT_MOVEPLAYER );
			// ���[�N�j��
			WFLBY_EVENTWK_DeleteWk( p_wk );

			// �C�x���g�ݒ�
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_PLAYERMAIN, NULL );
		}
		break;

	}

	return FALSE;
}




//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�J�������W��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	p_camera	�J����
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_FLOAT_SetCameraPos( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_CAMERA* p_camera )
{
	VecFx32 vec;
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_obj, &vec );

	// �J�������W�ɂ�����
	WFLBY_CAMERA_SetTarget3D( p_camera, vec.x, vec.y, vec.z );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�W�����v�J�n���邩�`�F�b�N
 *
 *	@param	cp_wk			���[�N
 *	@param	cp_mapobjcont	�}�b�v�I�u�W�F�Ǘ��V�X�e��
 *
 *	@retval	TRUE	�W�����v�J�n
 *	@retval	FALSE	�W�����v���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_FLOAT_CheckJumpOn( const WFLBY_EV_FLOAT_INSIDE* cp_wk, const WFLBY_3DMAPOBJ_CONT* cp_mapobjcont )
{
	VecFx32 obj_mat;
	VecFx32 sheet_mat;
	u32 float_idx, float_ofs;

	WFLBY_SYSTEM_GetFloatIdxOfs( cp_wk->floatidx, &float_idx, &float_ofs );

	// �V�[�g���W
	WFLBY_3DMAPOBJCONT_FLOAT_GetSheetPos( cp_mapobjcont, float_idx, float_ofs, &sheet_mat );

	// �����̍��W
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( cp_wk->p_obj, &obj_mat );	

	// �ʒu���r
	if( (obj_mat.x + WFLBY_EV_FLOAT_JUMP_CHECK_X) >= sheet_mat.x ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�W�����v����
 *
 *	@param	p_wk			���[�N
 *	@param	p_mapobjcont	�}�b�v�I�u�W�F�Ǘ��V�X�e�� 
 *	@paran	p_camera		�J����
 *
 *	@retval	TRUE	���슮��
 *	@retval	FALSE	����r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_FLOAT_JumpMove( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, WFLBY_CAMERA* p_camera )
{
	VecFx32 obj_mat;
	VecFx32 sheet_mat;
	u32 float_idx, float_ofs;
	WF2DMAP_POS	obj_pos;
	VecFx32 set_mat;
	u16 rot_y;
	BOOL ret;
	

	// �J�E���g
	p_wk->count ++;
	if( p_wk->count < WFLBY_EV_FLOAT_JUMP_ANM_SYNC ){
		ret = FALSE;
	}else{
		p_wk->count = WFLBY_EV_FLOAT_JUMP_ANM_SYNC;
		ret = TRUE;
	}
	

	WFLBY_SYSTEM_GetFloatIdxOfs( p_wk->floatidx, &float_idx, &float_ofs );

	// �V�[�g���W
	WFLBY_3DMAPOBJCONT_FLOAT_GetSheetPos( p_mapobjcont, float_idx, float_ofs, &sheet_mat );

	// �����̍��W
	obj_pos = WFLBY_3DOBJCONT_GetWkPos( p_wk->p_obj );
	WFLBY_3DMATRIX_GetPosVec( &obj_pos, &obj_mat );	
	
	// �V�[�g�ʒu�Ǝ��f�[�^�ʒu�̊Ԃ�count�̃J�E���^���ړ�����
	set_mat.x = 0;
	set_mat.y = FX_Div( FX_Mul( FX32_CONST(p_wk->count), (sheet_mat.y - obj_mat.y) ), FX32_CONST( WFLBY_EV_FLOAT_JUMP_ANM_SYNC ) );
	if( p_wk->count >= WFLBY_EV_FLOAT_JUMP_ANM_SYNC_ZS ){
		set_mat.z = FX_Div( FX_Mul( FX32_CONST(p_wk->count-WFLBY_EV_FLOAT_JUMP_ANM_SYNC_ZS), (sheet_mat.z - obj_mat.z) ), FX32_CONST( WFLBY_EV_FLOAT_JUMP_ANM_SYNC_Z ) );
	}else{
		set_mat.z = 0;
	}

	// �������J�����̍��W
	VEC_Add( &set_mat, &obj_mat, &set_mat );
	WFLBY_CAMERA_SetTarget3D( p_camera, set_mat.x, set_mat.y, set_mat.z );

	// �W�����v�Ȃ̂łx�ɃT�C���J�[�u������
	rot_y = (p_wk->count * 0x7fff) / WFLBY_EV_FLOAT_JUMP_ANM_SYNC;
	set_mat.y += FX_Mul( FX_SinIdx( rot_y ), WFLBY_EV_FLOAT_JUMP_MOVE_Y );

	// ���W��ݒ�
	WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_wk->p_obj, &set_mat );

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���n�A�j��
 *
 *	@param	p_wk			���[�N
 *	@param	p_mapobjcont	�}�b�v�I�u�W�F
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_FLOAT_JumpOnMove( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont )
{
	s32 local_count;
	BOOL ret;
	u16 rota_y;
	fx32 move_y;
	VecFx32 matrix;
	
	// �J�E���g
	p_wk->count ++;
	if( p_wk->count < WFLBY_EV_FLOAT_JUMPON_ANM_SYNC ){
		ret = FALSE;
	}else{
		p_wk->count = WFLBY_EV_FLOAT_JUMPON_ANM_SYNC;
		ret = TRUE;
	}

	// ���[�J���J�E���g�l
	local_count = p_wk->count % WFLBY_EV_FLOAT_JUMPON_ANM_ONE;
	rota_y = (local_count * 0x7fff) / WFLBY_EV_FLOAT_JUMPON_ANM_ONE;
	move_y = FX_Mul( FX_SinIdx( rota_y ), WFLBY_EV_FLOAT_JUMPON_ANM_MOVEY );

	// ���ݒ肳��Ă�����W�ɑ�������
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_obj, &matrix );
	matrix.y += move_y;
	WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_wk->p_obj, &matrix );

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W��ݒ�
 *
 *	@param	p_wk			���[�N
 *	@param	p_mapobjcont	�}�b�v�I�u�W�F�Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_FLOAT_MatrixSet( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, WFLBY_ROOMWK* p_rmwk )
{
	VecFx32 vec;
	u32 float_offs;
	u32 float_idx;

	WFLBY_SYSTEM_GetFloatIdxOfs( p_wk->floatidx, &float_idx, &float_offs );

	WFLBY_3DMAPOBJCONT_FLOAT_GetSheetPos( p_mapobjcont, float_idx, float_offs, &vec );
	WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_wk->p_obj, &vec );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�h���h���A�j���̌��ʂ����W�ɔ��f
 *
 *	@param	p_wk			���[�N
 *	@param	p_mapobjcont	�}�b�v�I�u�W�F�Ǘ��V�X�e��
 *	@param	p_rmwk			�������[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_FLOAT_MatrixDonDonSet( WFLBY_EV_FLOAT_INSIDE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, WFLBY_ROOMWK* p_rmwk )
{
	VecFx32 vec;
	fx32 y;
	u32 float_idx, float_ofs;

	WFLBY_SYSTEM_GetFloatIdxOfs( p_wk->floatidx, &float_idx, &float_ofs );

	// �t���[�g�V�F�C�N�A�j�����̓I�u�W�F�����炮��
	if( WFLBY_ROOM_FLOAT_CheckShakeAnm( p_rmwk, float_idx ) == TRUE ){
		if( WFLBY_EV_DEF_DONDON_CheckMove( &p_wk->dondon ) == FALSE ){
			WFLBY_EV_DEF_DONDON_Start( &p_wk->dondon );
		}
	}else{
		if( WFLBY_EV_DEF_DONDON_CheckMove( &p_wk->dondon ) == TRUE ){
			WFLBY_EV_DEF_DONDON_EndReq( &p_wk->dondon );
		}
	}
	//  �h���h���A�j��
	y = WFLBY_EV_DEF_DONDON_Main( &p_wk->dondon );

	// �\�����W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_obj, &vec );

	// ���炮��G�t�F�N�g�̒l��ݒ�
	vec.y += y;
	WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_wk->p_obj, &vec );
}


// �Z�^�C�v�r�b�g�}�b�v���X�g
//----------------------------------------------------------------------------
/**
 *	@brief	�Z�^�C�v�r�b�g�}�b�v���X�g�f�[�^���쐬
 *
 *	@param	p_wk		���[�N
 *	@param	p_rmwk		�������[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_DEF_PlayerInNormal_WazaTypeBmpListInit( WFLBY_EV_DEF_PLAYER_IN_WK* p_wk, WFLBY_ROOMWK* p_rmwk )
{
	int i;
	const STRBUF* cp_str;

	WFLBY_ROOM_LISTWIN_CreateBmpList( p_rmwk, WFLBY_POKEWAZA_TYPE_NUM );

	// �Z�^�C�v��ݒ�
	for( i=WFLBY_POKEWAZA_TYPE_NORMAL; i<WFLBY_POKEWAZA_TYPE_NUM; i++ ){

		// �I���Ȃ��̍��ڈȊO�̓��[�h�Z�b�g�Őݒ�
		WFLBY_ROOM_MSG_SetWazaType( p_rmwk, 0, i );
		cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_t03 );

		WFLBY_ROOM_LISTWIN_SetBmpListStr( p_rmwk, cp_str, i );
	}
	// �Ō�Ɂh����΂Ȃ��h
	cp_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_t04 );
	WFLBY_ROOM_LISTWIN_SetBmpListStr( p_rmwk, cp_str, WFLBY_POKEWAZA_TYPE_NONE );

	p_wk->bmplist		= sc_WFLBY_EV_DEF_WAZATYPE_HEADER;
	p_wk->bmplist.list	= WFLBY_ROOM_LISTWIN_GetBmpList( p_rmwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z�^�C�v�r�b�g�}�b�v���X�g�f�[�^��j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_rmwk		�������[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EV_DEF_PlayerInNormal_WazaTypeBmpListDelete( WFLBY_EV_DEF_PLAYER_IN_WK* p_wk, WFLBY_ROOMWK* p_rmwk )
{
	WFLBY_ROOM_LISTWIN_DeleteBmpList( p_rmwk );
}





//----------------------------------------------------------------------------
/**
 *	@brief	NPC�o���҂��C�x���g
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_NpcWait_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_NPC_WK*	p_evwk;
	WFLBY_3DPERSON*			p_npc;
	WFLBY_SYSTEM*			p_system;
	WFLBY_3DOBJCONT*		p_objcont;
	WFLBY_EVENT*			p_event;


	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_npc		= WFLBY_3DOBJCONT_GetPlIDWk( p_objcont, plno );
	p_evwk		= WFLBY_EVENTWK_GetWk( p_wk );
	p_event		= WFLBY_ROOM_GetEvent( p_rmwk );
	
	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ������
	case WFLBY_EV_DEF_NPCWAIT_SEQ_INIT:
		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_DEF_NPC_WK) );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCWAIT_SEQ_WAIT );
		
	// �o���҂�
	case WFLBY_EV_DEF_NPCWAIT_SEQ_WAIT:
		{
			BOOL in_flag, in_tbl;
			
			// �������ǉ����ꂽ���`�F�b�N	�ǉ�����ĂȂ��Ă��A���݂����璆�ɓ���
			in_flag = WFLBY_SYSTEM_FLAG_GetUserIn( p_system, plno );
			in_tbl	= WFLBY_SYSTEM_GetUserRoomIn( p_system, plno );
			if( (in_flag == TRUE) || (in_tbl == TRUE) ){

				// ������܂ŗ��Ƃ�������
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCWAIT_SEQ_INWAIT );
			}
		}
		break;

	// �o�^�ł���܂œo�^��������
	case WFLBY_EV_DEF_NPCWAIT_SEQ_INWAIT:
		{
			const WFLBY_USER_PROFILE* cp_profile;
			u32 tr_type;

			// �v���t�B�[���擾
			cp_profile = WFLBY_SYSTEM_GetUserProfile( p_system, plno );
			if( cp_profile == NULL ){
				// ���Ȃ��Ȃ��Ă��܂����̂ŁA�ҋ@�ɖ߂�
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCWAIT_SEQ_WAIT );
				break;
			}

			// ���̐l���K�W�F�b�g������������`�F�b�N
			if( WFLBY_SYSTEM_CheckGetItem( cp_profile ) == TRUE ){

				p_npc = WFLBY_3DOBJCONT_AddNpc( p_objcont, plno,
						WFLBY_SYSTEM_GetProfileTrType( cp_profile ) );
				if( p_npc ){	// �o�^�ł����玟��

					// ��\��
					WFLBY_3DOBJCONT_DRAW_SetDraw( p_npc, FALSE );

					WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCWAIT_SEQ_STATUS_CHECK );
				}
			}
		}
		break;

	// ��Ԃ��`�F�b�N���āA�~�j�Q�[���Ȃǂ��s���Ă���ꍇ��JUMP���荞�݃C�x���g��
	case WFLBY_EV_DEF_NPCWAIT_SEQ_STATUS_CHECK:
		{
			u32 status;
			const WFLBY_USER_PROFILE* cp_profile;
			BOOL result;
			
			cp_profile	= WFLBY_SYSTEM_GetUserProfile( p_system, plno );

			// ���Ȃ��Ȃ��ĂȂ����`�F�b�N
			if( cp_profile == NULL ){
				// Wait�ɖ߂�
				if( p_npc != NULL ){
					WFLBY_3DOBJCONT_Delete( p_npc );
				}
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCWAIT_SEQ_WAIT );
				break;
			}
			
			status		= WFLBY_SYSTEM_GetProfileStatus( cp_profile );

			// �ύX�������e�ɂ��A�����ύX����
			// ����ꏊ�ɔ�΂��ꍇ
			// �ʒu�ƕ������w�肵�Ĕ�΂�
			result = WFLBY_EV_DEF_NpcJumpParamMake( &p_evwk->intrparam, p_system, p_rmwk, p_npc, status, WFLBY_EV_DEF_NPCJUMP_MOVETYPE_STARTDOWN );
			if( result ){

				// �A���Ă�����I����
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCWAIT_SEQ_END );

				// NPC�̓����~
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENONE );

				// NPC��ԕ\���s�\
				WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, TRUE );				
				// ���[�J����Ԃ�BUSY�ɂ���
				WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, TRUE );

				// �`��X�V��~
				WFLBY_3DOBJCONT_DRAW_SetUpdata( p_npc, FALSE );

				// ���荞�݃C�x���g����
				// �����Ȃ�W�����v�C�����Ă���Ƃ��납��͂��߂�
				WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_NPCJUMP, &p_evwk->intrparam );
			}else{
				
				// �X�e�[�^�X�I�ɃW�����v���Ă����K�v���Ȃ��Ȃ�
				// ���ʂɗ��Ƃ�
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVEFLYDOWN );

				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCWAIT_SEQ_FLYDOWN );

				// ���[�J����Ԃ�BUSY�ɂ���
				WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, TRUE );
				// NPC��ԕ\���s�\
				WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, TRUE );				

				// �\��
				WFLBY_3DOBJCONT_DRAW_SetDraw( p_npc, TRUE );
			}
		}
		break;
		
	// �����Ă��鏈��
	case WFLBY_EV_DEF_NPCWAIT_SEQ_FLYDOWN:
		{
			BOOL result;
			result  = WFLBY_3DOBJCONT_CheckWkMoveEnd( p_npc );
			if( result == TRUE ){
				WFLBY_EVENTWK_AddSeq( p_wk );
			}
		}
		break;
		
	case WFLBY_EV_DEF_NPCWAIT_SEQ_END:

		// ���[�N�j��
		WFLBY_EVENTWK_DeleteWk( p_wk );
		
		// ���C�������
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENPC );
		WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_NPCMAIN, NULL );

		// ���[�J����Ԃ�BUSY��߂�
		WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, FALSE );
		// NPC��ԕ\���\
		WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, FALSE );				
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC���C���C�x���g
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_NpcMain_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_NPC_WK*	p_evwk;		// �C�x���g���[�N
	WFLBY_3DPERSON*			p_npc;
	WFLBY_3DPERSON*			p_player;
	WFLBY_SYSTEM*			p_system;
	WFLBY_3DOBJCONT*		p_objcont;
	WFLBY_EVENT*			p_event;

	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_event		= WFLBY_ROOM_GetEvent( p_rmwk );
	p_npc		= WFLBY_3DOBJCONT_GetPlIDWk( p_objcont, plno );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );

	p_evwk		= WFLBY_EVENTWK_GetWk( p_wk );
			

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ������
	case WFLBY_EV_DEF_NPCMAIN_SEQ_INIT:
		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_DEF_NPC_WK) );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCMAIN_SEQ_MAIN );

	// ���C��
	case WFLBY_EV_DEF_NPCMAIN_SEQ_MAIN:
		// �ގ��`�F�b�N
		if( WFLBY_SYSTEM_FLAG_GetUserOut(p_system, plno) ){
			// �ގ�������
			// ���[�N�j��
			WFLBY_EVENTWK_DeleteWk( p_wk );

			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENONE );	// �����~
			// NPC��ԕ\���s�\
			WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, TRUE );				

			// ���[�J����Ԃ�BUSY�ɂ���
			WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, TRUE );

			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_NPCEND, NULL );

			break;
		}

		// �v���t�B�[���ύX�����`�F�b�N
		if( WFLBY_SYSTEM_FLAG_GetUserProfileUpdate(p_system, plno) ){
		}

		// �t���[�g�`�F�b�N
		if( WFLBY_SYSTEM_Event_GetParade( p_system ) == TRUE ){
			u32 idx;

			// ����ĂȂ����`�F�b�N
			idx = WFLBY_SYSTEM_GetFloatPlIdxReserve( p_system, plno );
			if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
				// ����Ă�̂Ńt���[�g�C�x���g��
				// NPC�̓����~
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENONE );

				// NPC��ԕ\���s�\
				WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, TRUE );				
				// ���[�J����Ԃ�BUSY�ɂ���
				WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, TRUE );

				// �`��X�V��~
				WFLBY_3DOBJCONT_DRAW_SetUpdata( p_npc, FALSE );

				// ���[�N�j��
				WFLBY_EVENTWK_DeleteWk( p_wk );

				WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_NPCFLOAT, &p_evwk->intrparam );
				break;
			}
		}

		// �ʒu���΂����`�F�b�N
		{
			u32 status;
			const WFLBY_USER_PROFILE* cp_profile;
			BOOL result;
			
			cp_profile = WFLBY_SYSTEM_GetUserProfile( p_system, plno );
			status = WFLBY_SYSTEM_GetProfileStatus( cp_profile );

			// �ύX�������e�ɂ��A�����ύX����
			// ����ꏊ�ɔ�΂��ꍇ
			// �ʒu�ƕ������w�肵�Ĕ�΂�
			result = WFLBY_EV_DEF_NpcJumpParamMake( &p_evwk->intrparam, p_system, p_rmwk, p_npc, status, WFLBY_EV_DEF_NPCJUMP_MOVETYPE_NORMAL );
			if( result ){

				// NPC�̓����~
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENONE );

				// NPC��ԕ\���s�\
				WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, TRUE );				
				// ���[�J����Ԃ�BUSY�ɂ���
				WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, TRUE );
				// �`��X�V��~
				WFLBY_3DOBJCONT_DRAW_SetUpdata( p_npc, FALSE );

				// ���荞�݃C�x���g����
				WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_NPCJUMP, &p_evwk->intrparam );
				break;
			}


			// �K�W�F�b�g�̎��s
			if( WFLBY_SYSTEM_GetGadGetFlag( p_system, plno ) == TRUE ){
				// �l�������~
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENONE );

				// NPC��ԕ\���s�\
//				WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, TRUE );				�^�b�`�g�C��OK
				// ���[�J����Ԃ�BUSY�ɂ���
				WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, TRUE );
				
				// ���荞�݃C�x���g����
				WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_ALL_GADGET, NULL );
				break;
			}
		}
		
		break;
		
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ގ�����
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_NpcEnd_Brfore( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_3DPERSON*		p_npc;
	WFLBY_3DOBJCONT*	p_objcont;

	p_objcont = WFLBY_ROOM_GetObjCont( p_rmwk );

	p_npc = WFLBY_3DOBJCONT_GetPlIDWk( p_objcont, plno );
	
	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// �ގ�
	case WFLBY_EV_DEF_NPCEND_SEQ_OUT:
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVEFLYUP );	// ��ɒ���ł�
		WFLBY_EVENTWK_AddSeq( p_wk );
		break;

	// �ގ������҂�
	case WFLBY_EV_DEF_NPCEND_SEQ_OUTWAIT:
		{
			BOOL result;

			result = WFLBY_3DOBJCONT_CheckWkMoveEnd( p_npc );
			if( result ){
				WFLBY_EVENTWK_AddSeq( p_wk );
			}
		}
		break;

	// �I��
	case WFLBY_EV_DEF_NPCEND_SEQ_END:
		// �j��
		WFLBY_3DOBJCONT_Delete( p_npc );

		// �����҂���
		{
			WFLBY_EVENT* p_event;
			p_event = WFLBY_ROOM_GetEvent( p_rmwk );
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_NPCWAIT, NULL );
		}

		break;

	}

	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�ɏ���đގ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_NpcFloat_Brfore( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_NPCFLOAT_WK* p_evwk;
	WFLBY_3DOBJCONT*		p_objcont;
	WFLBY_3DMAPOBJ_CONT*	p_mapobjcont;
	WFLBY_SYSTEM*			p_system;

	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_mapobjcont= WFLBY_ROOM_GetMapObjCont( p_rmwk );
	p_evwk		= WFLBY_EVENTWK_GetWk( p_wk );

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ������
	case WFLBY_EV_DEF_NPCFLOAT_SEQ_INIT:
		{
			p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_DEF_NPCFLOAT_WK) );
			p_evwk->p_npc		= WFLBY_3DOBJCONT_GetPlIDWk( p_objcont, plno );
			p_evwk->float_idx	= WFLBY_SYSTEM_GetFloatPlIdxReserve( p_system, plno );
			GF_ASSERT( p_evwk->float_idx != DWC_LOBBY_USERIDTBL_IDX_NONE );

			WFLBY_SYSTEM_GetFloatIdxOfs( p_evwk->float_idx, &p_evwk->float_idx, &p_evwk->float_offs );

			OS_TPrintf( "npc=%d float_idx %d float_offs %d \n", plno, p_evwk->float_idx, p_evwk->float_offs );
			
			
			// ��ɔ�яオ��
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_evwk->p_npc, WFLBY_3DOBJCONT_MOVEFLYUP );
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCFLOAT_SEQ_UPWAIT );
		}
		break;

	// ��яオ��
	case WFLBY_EV_DEF_NPCFLOAT_SEQ_UPWAIT:
		if( WFLBY_3DOBJCONT_CheckWkMoveEnd( p_evwk->p_npc ) ){

			// ���n�_���v�Z
			{
				VecFx32 matrix;

				WFLBY_3DMAPOBJCONT_FLOAT_GetSheetPos( p_mapobjcont, p_evwk->float_idx,
						p_evwk->float_offs, &matrix );
				p_evwk->on_x	= matrix.x + WFLBY_EV_DEF_NPCFLOAT_DIF_X;
				WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_evwk->p_npc, &matrix );
				p_evwk->start_y	= matrix.y;
				p_evwk->count	= 0;
			}

			
			// ���������蔻��֌W�Ȃ��̂ŁA�O�C�O�̈ʒu�Ɉړ�
			{
				WF2DMAP_POS pos = {0};
				WFLBY_3DOBJCONT_SetWkPos( p_evwk->p_npc, pos );
			}

			// �����Ă��鉹
			Snd_SePlay( WFLBY_SND_PLOUT );

			// ��]�J�n
			WFLBY_3DOBJCONT_DRAW_SetAnmRota( p_evwk->p_npc, TRUE );
			
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCFLOAT_SEQ_DOWNWAIT );
		}
		break;

	// ���n
	case WFLBY_EV_DEF_NPCFLOAT_SEQ_DOWNWAIT:
		{
			VecFx32 matrix;
			VecFx32 set_mat;
			BOOL end = FALSE;

			//  �J�E���g����
			p_evwk->count ++;
			if( p_evwk->count >= WFLBY_EV_DEF_NPCFLOAT_DOWN_COUNT ){
				p_evwk->count = WFLBY_EV_DEF_NPCFLOAT_DOWN_COUNT;
				end = TRUE;
			}

			// �V�[�g�ʒu�擾
			WFLBY_3DMAPOBJCONT_FLOAT_GetSheetPos( p_mapobjcont, p_evwk->float_idx,
					p_evwk->float_offs, &matrix );

			set_mat.z = matrix.z;
			set_mat.x = p_evwk->on_x;
			set_mat.y = FX_Div( FX_Mul( FX32_CONST(p_evwk->count), (matrix.y - p_evwk->start_y) ), FX32_CONST( WFLBY_EV_DEF_NPCFLOAT_DOWN_COUNT ) );
			set_mat.y += p_evwk->start_y;

			WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_evwk->p_npc, &set_mat );

			// �J�E���^���I�������璅�n����
			if( end == TRUE ){

				// ��]�I����
				WFLBY_3DOBJCONT_DRAW_SetAnmRota( p_evwk->p_npc, FALSE );
				
				// ���邭����
				WFLBY_3DOBJCONT_DRAW_SetLight( p_evwk->p_npc, 1<<WFLBY_DRAW_LIGHT_FLOAT  );

				// ����������
				WFLBY_3DOBJCONT_DRAW_SetWay( p_evwk->p_npc, WF2DMAP_WAY_LEFT );
				
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCFLOAT_SEQ_MAIN );
			}
		}
		break;

	// ���C��
	case WFLBY_EV_DEF_NPCFLOAT_SEQ_MAIN:
		{
			VecFx32 matrix;
			fx32 y;

			// �t���[�g�V�F�C�N�A�j�����̓I�u�W�F�����炮��
			if( WFLBY_ROOM_FLOAT_CheckShakeAnm( p_rmwk, p_evwk->float_idx ) == TRUE ){
				if( WFLBY_EV_DEF_DONDON_CheckMove( &p_evwk->dondon ) == FALSE ){
					WFLBY_EV_DEF_DONDON_Start( &p_evwk->dondon );
				}
			}else{
				if( WFLBY_EV_DEF_DONDON_CheckMove( &p_evwk->dondon ) == TRUE ){
					WFLBY_EV_DEF_DONDON_EndReq( &p_evwk->dondon );
				}
			}
			//  �h���h���A�j��
			y = WFLBY_EV_DEF_DONDON_Main( &p_evwk->dondon );

			// �V�[�g�ʒu�擾
			WFLBY_3DMAPOBJCONT_FLOAT_GetSheetPos( p_mapobjcont, p_evwk->float_idx,
					p_evwk->float_offs, &matrix );

			matrix.y += y;

			// �V�[�g�ʒu�ɂ����Əo��
			WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_evwk->p_npc, &matrix );

			// ����Ă���t���[�g����\���ɓ����~������I����
			if( WFLBY_SYSTEM_GetFloatMove( p_system, p_evwk->float_idx ) == WFLBY_FLOAT_STATE_AFTER ){
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCFLOAT_SEQ_END );
			}
		}
		break;

	// �I��
	case WFLBY_EV_DEF_NPCFLOAT_SEQ_END:
		// �j��
		WFLBY_3DOBJCONT_Delete( p_evwk->p_npc );

		// ���[�N�j��
		WFLBY_EVENTWK_DeleteWk( p_wk );

		// �����҂���
		{
			WFLBY_EVENT* p_event;
			p_event = WFLBY_ROOM_GetEvent( p_rmwk );
			WFLBY_EVENT_SET_PRIVATE_EVENT( p_event, plno, &sc_WFLBY_EV_DEF_NPCWAIT, NULL );
		}
		break;
	}

	return FALSE;
}




//-------------------------------------
///	NPC��풆�C�x���g
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	����ꏊ��NPC���W�����v�����Ă�������
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_NpcJump_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_DEF_NPCJUMP_PARAM* p_param;
	WFLBY_3DOBJCONT*			p_objcont;
	WFLBY_3DPERSON*				p_npc;
	WFLBY_3DPERSON*				p_player;
	WFLBY_SYSTEM*				p_system;
	
	// �p�����[�^�擾
	p_param = WFLBY_EVENTWK_GetParam( p_wk );

	// �V�X�e���f�[�^
	p_system = WFLBY_ROOM_GetSystemData( p_rmwk );
	
	// NPC�̃f�[�^�擾
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_npc		= WFLBY_3DOBJCONT_GetPlIDWk( p_objcont, plno );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	// ����������
	case WFLBY_EV_DEF_NPCJUMP_SEQ_INIT:
		if( p_param->movetype == WFLBY_EV_DEF_NPCJUMP_MOVETYPE_NORMAL ){
			WFLBY_EVENTWK_AddSeq( p_wk );
		}else{
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_DEF_NPCJUMP_SEQ_DOWN );
		}
		break;

	// ���
	case WFLBY_EV_DEF_NPCJUMP_SEQ_UP:

		// ���̐l�̓����ύX
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVEFLYUP );
		WFLBY_EVENTWK_AddSeq( p_wk );
		break;

	// �̂ڂ�҂�
	case WFLBY_EV_DEF_NPCJUMP_SEQ_UPWAIT:
	// �߂�̏��҂�
	case WFLBY_EV_DEF_NPCJUMP_SEQ_RET_UPWAIT:
	// �߂�̂�����܂�
	case WFLBY_EV_DEF_NPCJUMP_SEQ_RET_DOWNWAIT:
		{
			BOOL result;
			result  = WFLBY_3DOBJCONT_CheckWkMoveEnd( p_npc );
			if( result == TRUE ){
				WFLBY_EVENTWK_AddSeq( p_wk );
			}
		}
		break;

	// �V�����ꏊ�ɉ���
	case WFLBY_EV_DEF_NPCJUMP_SEQ_DOWN:

		// ������ꏊ�̐ݒ�
		{
			WF2DMAP_POS pos;
			pos = WFLBY_3DOBJCONT_GetWkPos( p_npc );
			WFLBY_3DOBJCONT_DRAW_SetMatrix( p_npc, &pos );
		}

		//  �\����OFF�Ȃ�ON�ɂ���
		if( WFLBY_3DOBJCONT_DRAW_GetDraw( p_npc ) == FALSE ){
			// �\��
			WFLBY_3DOBJCONT_DRAW_SetDraw( p_npc, TRUE );
		}

		// ���Ƃ�
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVEFLYDOWN );
		WFLBY_EVENTWK_AddSeq( p_wk );
		break;

	// ����҂�
	case WFLBY_EV_DEF_NPCJUMP_SEQ_DOWNWAIT:
		{
			BOOL result;
			result  = WFLBY_3DOBJCONT_CheckWkMoveEnd( p_npc );
			if( result == TRUE ){
				WFLBY_EVENTWK_AddSeq( p_wk );

				// ������~
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENONE );

				// ��b�r�W�[��Ԃ�����
				WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, FALSE );

				// NPC��ԕ\���\
				WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, FALSE );
			}
		}
		break;

	// ��ԑJ�ڑ҂�
	case WFLBY_EV_DEF_NPCJUMP_SEQ_WAIT:
		{
			const WFLBY_USER_PROFILE* cp_profile;
			const WFLBY_USER_PROFILE* cp_profile_player;
			BOOL add_seq = FALSE;

			// �����������̂ŁA��b��Ԃ̂Ƃ��Ȃ̂��`�F�b�N
			if( (p_param->talk_flag == TRUE) && 
				(WFLBY_SYSTEM_CheckTalkBStart( p_system ) == FALSE) ){
				u32 retway;
				u32 way;
				u32 status;

				cp_profile_player	= WFLBY_SYSTEM_GetMyProfileLocal( p_system );
				status				= WFLBY_SYSTEM_GetProfileStatus( cp_profile_player );
				
				// ��l����Ԃ���b�ɂȂ�����U�����
				if( status == WFLBY_STATUS_TALK ){
					// ��l���̉�b���J�n������
					WFLBY_SYSTEM_SetTalkBStart( p_system );

					// ��l���������̕��Ɍ�������
					way		= WFLBY_3DOBJCONT_GetWkObjData( p_npc, WF2DMAP_OBJPM_WAY );
					retway	= WF2DMPA_OBJToolRetWayGet( way );
					WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_player, WF2DMAP_CMD_NONE, retway );
				}
			}

			cp_profile	= WFLBY_SYSTEM_GetUserProfile( p_system, plno );

			do{
				// �ގ��`�F�b�N
				if( cp_profile == NULL ){	// ���łɑގ����Ă���
					add_seq = TRUE;
					break;
				}


				// ��b��ԂȂ�A��b���I�������Ƃ���łP��߂点��
				// �����������̂ŁA��b��Ԃ̂Ƃ��Ȃ̂��`�F�b�N
				if( (p_param->talk_flag == TRUE) ){
					// ��l���̉�b���I�������NPC���g���߂�
					if( WFLBY_SYSTEM_CheckTalk( p_system ) == FALSE ){
						add_seq = TRUE;
						break;
					}
				}
				
				// �w��̏�Ԃ��ς�����猳�̈ʒu�ɖ߂�
				if( WFLBY_SYSTEM_FLAG_GetUserProfileUpdate(p_system, plno) ){
					u32 status;

					status		= WFLBY_SYSTEM_GetProfileStatus( cp_profile );

					// �X�e�[�^�X���ς�����猳�̂̈ʒu�ɖ߂�
					if( status != p_param->status ){
						add_seq = TRUE;
						break;
					}
				}
			}while(0);


			// �V�[�P���X��i�߂邩�`�F�b�N
			if( add_seq == TRUE ){
				WFLBY_EVENTWK_AddSeq( p_wk );

				// ��b�r�W�[��Ԃ�ݒ�
				WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, TRUE );

				// NPC��ԕ\���s�\
				WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, TRUE );
			}
		}
		break;

	// �߂�̏��
	case WFLBY_EV_DEF_NPCJUMP_SEQ_RET_UP:
		// ���̐l�̓����ύX
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVEFLYUP );
		WFLBY_EVENTWK_AddSeq( p_wk );
		break;

	// �߂�̂�����
	case WFLBY_EV_DEF_NPCJUMP_SEQ_RET_DOWN:
		// ���̈ʒu�ɖ߂�邩�`�F�b�N
		// ���̏ꏊ�Ɏ�l�������Ȃ����`�F�b�N
		{
			WFLBY_3DPERSON* p_player;
			WF2DMAP_POS		pos;

			p_player = WFLBY_3DOBJCONT_GetPlayer( p_objcont );
			if( WFLBY_3DOBJCONT_CheckGridHit( p_player, 
						p_param->gridx, 
						p_param->gridy ) == FALSE ){

				pos.x = WF2DMAP_GRID2POS( p_param->gridx );
				pos.y = WF2DMAP_GRID2POS( p_param->gridy );

				// ���W��ݒ�
				// ������ꏊ�̐ݒ�
				WFLBY_3DOBJCONT_SetWkPos( p_npc, pos );
				WFLBY_3DOBJCONT_DRAW_SetMatrix( p_npc, &pos );

				// ���Ƃ�
				WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVEFLYDOWN );
				WFLBY_EVENTWK_AddSeq( p_wk );
			}
		}
		break;

	// �I��
	case WFLBY_EV_DEF_NPCJUMP_SEQ_END:
		// NPC���앜�A
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_npc, WFLBY_3DOBJCONT_MOVENPC );

		// ���[�J����Ԃ�BUSY�����ɖ߂�
		WFLBY_ROOM_SetNpcTalkBusy( p_rmwk, plno, FALSE );

		// NPC��ԕ\���\
		WFLBY_ROOM_SetNpcInfoDrawBusy( p_rmwk, plno, FALSE );

		// ��풓�C�x���g�I��
		return TRUE;

	}

	return FALSE;
}


//-------------------------------------
///	�p�����[�^�쐬�֐�
//=====================================

//----------------------------------------------------------------------------
/**
 *	@brief	�W�����v�ʒu�ړ��p�����[�^�쐬���W�����v�ʒu�ړ��C�x���g�𔭓����邩�`�F�b�N
 *
 *	@param	p_param		�p�����[�^�i�[��
 *	@param	p_system	�V�X�e�����[�N
 *	@param	p_rmwk		���[�����[�N
 *	@param	p_npc		NPC���[�N
 *	@param	status		���
 *	@param	movetype	����^�C�v
 *
 *	@retval	TRUE		�C�x���g����
 *	@retval	FALSE		�C�x���g���������Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_NpcJumpParamMake( WFLBY_EV_DEF_NPCJUMP_PARAM* p_param, WFLBY_SYSTEM* p_system, WFLBY_ROOMWK* p_rmwk, WFLBY_3DPERSON* p_npc, u8 status, WFLBY_EV_DEF_NPCJUMP_MOVETYPE movetype )
{
	u32 plno;
	u32 way;
	WF2DMAP_POS pos;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_3DPERSON* p_player;
	WFLBY_MAPCONT*	p_mapcont;
	BOOL talk_flag = FALSE;
	BOOL result;

	// �I�u�W�F�Ǘ��V�X�e���擾
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );
	
	// �}�b�v�Ǘ��V�X�e���擾
	p_mapcont	= WFLBY_ROOM_GetMapCont( p_rmwk );


	plno = WFLBY_3DOBJCONT_GetWkObjData( p_npc, WF2DMAP_OBJPM_PLID );

	// �����̏����l�͉�
	way = WF2DMAP_WAY_DOWN;
	
	// status�I�ɃW�����v������̂��`�F�b�N
	switch( status ){

	// ��΂ɃW�����v����
	case WFLBY_STATUS_BALLSLOW:		// �ʂ���
		result = WFLBY_EV_DEF_NpcJumpParam_GetOBJIDPos( p_mapcont, p_objcont, 
					sc_WFLBY_EV_DEF_NPC_OBJID_BS, WFLBY_EV_DEF_NPC_OBJID_MINIGAME_NUM, &pos );
		if( result == FALSE ){
			return FALSE;
		}
		break;
		
	case WFLBY_STATUS_BALANCEBALL:	// �ʏ��
		result = WFLBY_EV_DEF_NpcJumpParam_GetOBJIDPos( p_mapcont, p_objcont, 
					sc_WFLBY_EV_DEF_NPC_OBJID_BB, WFLBY_EV_DEF_NPC_OBJID_MINIGAME_NUM, &pos );
		if( result == FALSE ){
			return FALSE;
		}
		break;
		
	case WFLBY_STATUS_BALLOON:		// ���D���
		result = WFLBY_EV_DEF_NpcJumpParam_GetOBJIDPos( p_mapcont, p_objcont, 
					sc_WFLBY_EV_DEF_NPC_OBJID_BL, WFLBY_EV_DEF_NPC_OBJID_MINIGAME_NUM, &pos );
		if( result == FALSE ){
			return FALSE;
		}
		break;
		
	case WFLBY_STATUS_FOOTBOAD00:	// ���Ճ{�[�h
		way = WF2DMAP_WAY_UP;
		result = WFLBY_EV_DEF_NpcJumpParam_GetOBJIDPos( p_mapcont, p_objcont, 
					sc_WFLBY_EV_DEF_NPC_OBJID_FT00, WFLBY_EV_DEF_NPC_OBJID_FOOT_NUM, &pos );
		if( result == FALSE ){
			return FALSE;
		}
		break;
		
	case WFLBY_STATUS_FOOTBOAD01:	// ���Ճ{�[�h
		way = WF2DMAP_WAY_UP;
		result = WFLBY_EV_DEF_NpcJumpParam_GetOBJIDPos( p_mapcont, p_objcont, 
					sc_WFLBY_EV_DEF_NPC_OBJID_FT01, WFLBY_EV_DEF_NPC_OBJID_FOOT_NUM, &pos );
		if( result == FALSE ){
			return FALSE;
		}
		break;
		
	case WFLBY_STATUS_WORLDTIMER:	// ���E���v
		way = WF2DMAP_WAY_RIGHT;
		result = WFLBY_EV_DEF_NpcJumpParam_GetOBJIDPos( p_mapcont, p_objcont, 
					sc_WFLBY_EV_DEF_NPC_OBJID_WT, WFLBY_EV_DEF_NPC_OBJID_WLDTIMER_NUM, &pos );
		if( result == FALSE ){
			return FALSE;
		}
		break;
		
	case WFLBY_STATUS_TOPIC:		// �j���[�X
		way = WF2DMAP_WAY_UP;
		result = WFLBY_EV_DEF_NpcJumpParam_GetOBJIDPos( p_mapcont, p_objcont, 
					sc_WFLBY_EV_DEF_NPC_OBJID_TP, WFLBY_EV_DEF_NPC_OBJID_TOPIC_NUM, &pos );
		if( result == FALSE ){
			return FALSE;
		}
		break;
		
	case WFLBY_STATUS_FLOAT:		// �t���[�g
		// ��������
		return FALSE;

	// ��b��Ԃɂ���Ă�
	case WFLBY_STATUS_TALK:			// ��b
		// ��l������b���H
		if( WFLBY_SYSTEM_CheckTalk( p_system ) == FALSE ){
			return FALSE;
		}

		// ��l���͘b��������ꂽ��
		if( WFLBY_SYSTEM_CheckTalkReq( p_system ) == FALSE ){
			return FALSE;
		}

		// ��l�����b��������ꂽ�l�H
		if( WFLBY_SYSTEM_GetTalkIdx( p_system ) != plno ){
			return FALSE;
		}

#if PL_T0867_080716_FIX
		// ��l������b�\��ԂɂȂ������`�F�b�N
		// �����Ȃ̂ŁA�W�����v���Ȃ�
		if( WFLBY_ROOM_CheckPlayerEventAfterMove( p_rmwk ) == TRUE ){
			return FALSE;
		}
#endif

		// ��l���̋߂��ɔ��ł�������
		// ��l���̎���S�O���b�h�ŋ󂢂Ă���Ƃ����T���B
		result = WFLBY_3DOBJCONT_GetOpenGird4Way( p_objcont, p_player, &way, &pos );
		GF_ASSERT( result == TRUE );

		// way�͎�l���̌����ׂ�����
		way = WF2DMPA_OBJToolRetWayGet( way );	// �����͋t����
		talk_flag = TRUE;
		break;
	

	case WFLBY_STATUS_NONE:
	case WFLBY_STATUS_LOGIN:			// ���O�C�����
	case WFLBY_STATUS_LOGOUT:		// �ގ�
	case WFLBY_STATUS_UNKNOWN:		// �s��
	case WFLBY_STATUS_MATCHING:		// �~�j�Q�[���̃}�b�`���O��
	case WFLBY_STATUS_ANKETO:		// �A���P�[�g��
	defalut:
		return FALSE;
	}

	// �\��&�߂�ꏊ��ݒ�
	{
		WF2DMAP_POS retpos;

		// �߂�ꏊ�擾
		retpos = WFLBY_3DOBJCONT_GetWkPos( p_npc );

		// �ʒu��\��
		WFLBY_3DOBJCONT_SetWkPosAndWay( p_npc, pos, way );

		// �p�����[�^�ݒ�
		p_param->gridx		= WF2DMAP_POS2GRID( retpos.x );
		p_param->gridy		= WF2DMAP_POS2GRID( retpos.y );
		p_param->status		= status;
		p_param->talk_flag	= talk_flag;
		p_param->movetype	= movetype;
	}



	return TRUE;
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F���X�g�ɂ���I�u�W�FID�̃}�b�v�ʒu�ɗ����Ă�����ꏊ�����邩�`�F�b�N����
 *
 *	@param	cp_mapcont		�}�b�v�Ǘ��V�X�e��
 *	@param	cp_objcont		�I�u�W�F�Ǘ��V�X�e��
 *	@param	cp_objid		�I�u�W�FID���X�g
 *	@param	num				���X�g��
 *	@param	p_pos			�����Ă�����ʒu
 *
 *	@retval	TRUE	������ꏊ��������
 *	@retval	FALSE	������ꏊ���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EV_DEF_NpcJumpParam_GetOBJIDPos( const WFLBY_MAPCONT* cp_mapcont, const WFLBY_3DOBJCONT* cp_objcont, const u32* cp_objid, u32 num, WF2DMAP_POS* p_pos )
{
	u16 gridx, gridy;
	int i;
	BOOL result;

	for( i=0; i<num; i++ ){
		result = WFLBY_MAPCONT_SarchObjID( cp_mapcont, cp_objid[i], &gridx, &gridy, 0 );
		if( result == TRUE ){	// �������炻�̈ʒu�ɐl�����Ȃ����`�F�b�N
			if( WFLBY_3DOBJCONT_CheckSysGridHit( cp_objcont, gridx, gridy ) == NULL ){
				
				// ���̈ʒu���󂢂Ă���
				p_pos->x = WF2DMAP_GRID2POS( gridx );
				p_pos->y = WF2DMAP_GRID2POS( gridy );
				return TRUE;
			}
		}
	}

	return FALSE;
}


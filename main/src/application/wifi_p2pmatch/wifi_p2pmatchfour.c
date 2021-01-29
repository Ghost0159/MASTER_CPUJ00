//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_p2pmatchfour.c
 *	@brief		wifi�S�l�}�b�`���O��p���
 *	@author		tomoya takahashi
 *	@data		2007.05.22
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "wifi/dwc_rap.h"
#include "wifi/vchat.h"
#include "system/bmp_list.h"

#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_wifi_lobby.h"

#include "system/window.h"
#include "system/bmp_menu.h"
#include "system/wipe.h"
#include "system/brightness.h"
#include "system/fontproc.h"

#include "system/pm_overlay.h"

#include "communication/communication.h"
#include "communication/comm_state.h"

#include "comm_command_wfp2pmf.h"
#include "comm_command_wfp2pmf_func.h"

#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wf2dmap_objdraw.h"

#include "wifi/dwc_rapfriend.h"

#include "system/snd_tool.h"
#include "wifi_p2pmatch_se.h"


#define __WIFI_P2PMATCHFOUR_H_GLOBAL
#include "wifi_p2pmatchfour.h"

#include "application\namein.h"
#include "wifi_p2pmatch_local.h"
#include "wifi_p2pmatchfour_local.h"

#include "wifip2pmatch.naix"			// �O���t�B�b�N�A�[�J�C�u��`


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
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

// WIFI2DMAP�V�X�e���I�[�o�[���C
FS_EXTERN_OVERLAY(wifi_2dmapsys);

// �Z���A�N�^�[
#define WIFI_P2PMATCH_RESNUM	(4)	// ���\�[�X��
#define WIFI_P2PMATCH_CLACTWK_NUM	(4)		// �A�N�^�[���[�N��
#define WIFI_P2PMATCH_LOADRESNUM	(4)		// �ǂݍ��ރ��\�[�X��
#define WIFI_P2PMATCH_VRAMMAINSIZ	(128*1024)
#define WIFI_P2PMATCH_VRAMSUBSIZ	(16*1024)
#define WIFI_P2PMATCH_VRAMTRMAN_NUM	( 16 )

// ���b�Z�[�W�\������
#define WIFI_P2PMATCH_MSG_WAIT	( 30 )


#define _TIMING_GAME_CHECK  (13)// �Ȃ���������
#define _TIMING_GAME_CHECK2  (14)// �Ȃ���������
#define _TIMING_GAME_START  (15)// �^�C�~���O�����낦��
#define _TIMING_GAME_START2  (18)// �^�C�~���O�����낦��
#define _TIMING_BATTLE_END  (16)// �^�C�~���O�����낦��


#define RECONNECT_WAIT	(90)

#define KO_ENTRYNUM_CHECK_WAIT	(10*30)

#define CONNECT_TIMEOUT		(60*30)

//-------------------------------------
///	���C������
//=====================================
enum {
	WFP2PMF_MAIN_WIPE_S,
	WFP2PMF_MAIN_WIPE_E,
	WFP2PMF_MAIN_MATCHWAIT,
	WFP2PMF_MAIN_EWIPE_S,
	WFP2PMF_MAIN_EWIPE_E,
} ;


//-------------------------------------
///	�e����STATUS
//=====================================
enum {
	WFP2PMF_OYA_STATUS_INIT,				// �e�̏�����
	WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT,	// CONNECT�҂�������
	WFP2PMF_OYA_STATUS_CONNECT_WAIT,		// CONNECT�҂�
	WFP2PMF_OYA_STATUS_CONNECT_SELECT_INIT00,// CONNECT�҂�
	WFP2PMF_OYA_STATUS_CONNECT_SELECT_INIT01,// CONNECT�����l�����邩�I��������
	WFP2PMF_OYA_STATUS_CONNECT_SELECT,		// CONNECT�����l�����邩�I��
	WFP2PMF_OYA_STATUS_CONNECT_MIN_INIT,	// �Q�[���J�n���������A�l��������Ȃ�
	WFP2PMF_OYA_STATUS_CONNECT_MIN,			// �Q�[���J�n���������A�l��������Ȃ�
	WFP2PMF_OYA_STATUS_START_INIT00,			// �Q�[���J�n�����@�J�n
	WFP2PMF_OYA_STATUS_START_INIT01,			// �Q�[���J�n�����@�J�n
	WFP2PMF_OYA_STATUS_START_SELECT,		// �Q�[���J�n�����ɍs�����I��
	WFP2PMF_OYA_STATUS_START,				// �Q�[���J�n��
	WFP2PMF_OYA_STATUS_STARTNUMCHECK,		// �Q�[���J�n�@�l���`�F�b�N
	WFP2PMF_OYA_STATUS_STARTSYNC_INIT,		// �Q�[���J�n����
	WFP2PMF_OYA_STATUS_STARTSYNC,			// �Q�[���J�n����
	WFP2PMF_OYA_STATUS_STATE_SEND,			// �Q�[���J�n	�X�e�[�^�X���M
	WFP2PMF_OYA_STATUS_STATE_CHECK,			// �Q�[���J�n	�X�e�[�^�X�`�F�b�N
	WFP2PMF_OYA_STATUS_MYSTATUS_WAIT,		// �Q�[���J�n	�f�[�^�҂�
	WFP2PMF_OYA_STATUS_GAME_START,			// �J�n
	WFP2PMF_OYA_STATUS_END_INIT00,			// �ҋ@��Ԃ���߂邩�@�J�n
	WFP2PMF_OYA_STATUS_END_INIT01,			// �ҋ@��Ԃ���߂邩�@�J�n
	WFP2PMF_OYA_STATUS_END_SELECT,			// �ҋ@��Ԃ���߂邩�@�I��
	WFP2PMF_OYA_STATUS_END2_INIT00,			// �ҋ@��Ԃ���߂�Ɓ@���U�@�J�n
	WFP2PMF_OYA_STATUS_END2_INIT01,			// �ҋ@��Ԃ���߂�Ɓ@���U�@�J�n
	WFP2PMF_OYA_STATUS_END2_SELECT,			// �ҋ@��Ԃ���߂�Ɓ@���U�@�I��
	WFP2PMF_OYA_STATUS_END3_INIT,			// �ҋ@��Ԃ���߂�Ɓ@���U�@�J�n
	WFP2PMF_OYA_STATUS_END,					// �ҋ@��ԏI��
	WFP2PMF_OYA_STATUS_ERR_INIT,			// �ʐM�G���[�ɂ��I�� �J�n
	WFP2PMF_OYA_STATUS_ERR_INIT01,			// �ʐM�G���[�ɂ��I�� �J�n
	WFP2PMF_OYA_STATUS_ERR,					// �ʐM�G���[�ɂ��I��
	WFP2PMF_OYA_STATUS_VCHAT_INIT00,		// �{�C�X�`���b�g�ύX	�J�n
	WFP2PMF_OYA_STATUS_VCHAT_INIT01,		// �{�C�X�`���b�g�ύX	�J�n
	WFP2PMF_OYA_STATUS_VCHAT_SELECT,		// �{�C�X�`���b�g�ύX	�I��
	WFP2PMF_OYA_STATUS_NUM
} ;

//-------------------------------------
///	�q������
//=====================================
enum {
	WFP2PMF_KO_STATUS_ENTRY_OYAWAITINIT,// �e�̏��擾�҂�
	WFP2PMF_KO_STATUS_ENTRY_OYAWAIT,	// �e�̏��擾�҂�
	WFP2PMF_KO_STATUS_ENTRY_INIT,		// ENTRY�����҂��J�n
	WFP2PMF_KO_STATUS_ENTRY_WAIT,		// ENTRY�����҂�
	WFP2PMF_KO_STATUS_ENTRY_OKINIT,		// ENTRYOK������
	WFP2PMF_KO_STATUS_ENTRY_OK,			// ENTRYOK
	WFP2PMF_KO_STATUS_ENTRY_NGINIT,		// ENTRYNG������
	WFP2PMF_KO_STATUS_ENTRY_NG,			// ENTRYNG
	WFP2PMF_KO_STATUS_CONNECT_INIT,		// CONNECT�����҂��J�n
	WFP2PMF_KO_STATUS_CONNECT_WAIT,		// CONNECT�����҂�
	WFP2PMF_KO_STATUS_START,			// �J�n
	WFP2PMF_KO_STATUS_STATE_SEND,			// �Q�[���J�n	�X�e�[�^�X���M
	WFP2PMF_KO_STATUS_STATE_CHECK,			// �Q�[���J�n	�X�e�[�^�X�`�F�b�N
	WFP2PMF_KO_STATUS_MYSTATUS_WAIT,		// �Q�[���J�n	�f�[�^�҂�
	WFP2PMF_KO_STATUS_GAME_START,			// �J�n
	WFP2PMF_KO_STATUS_ERR_INIT,			// �G���[�I��	�J�n
	WFP2PMF_KO_STATUS_ERR_INIT01,		// �G���[�I��	�J�n
	WFP2PMF_KO_STATUS_ERR,				// �G���[�I��
	WFP2PMF_KO_STATUS_VCHAT_INIT00,		// �{�C�X�`���b�g�ύX	�J�n
	WFP2PMF_KO_STATUS_VCHAT_INIT01,		// �{�C�X�`���b�g�ύX	�J�n
	WFP2PMF_KO_STATUS_VCHAT_SELECT,		// �{�C�X�`���b�g�ύX	�I��
	WFP2PMF_KO_STATUS_CONLOCK_INIT00,		// �ʐMLOCK���Ȃ̂ōĐڑ�
	WFP2PMF_KO_STATUS_CONLOCK_INIT01,		// �ʐMLOCK���Ȃ̂ōĐڑ�
	WFP2PMF_KO_STATUS_CONLOCK_SELECT,		// �ʐMLOCK���Ȃ̂ōĐڑ�
	WFP2PMF_KO_STATUS_CONLOCK_RECON,		// �ʐMLOCK���@�Đڑ�
	WFP2PMF_KO_STATUS_CONLOCK_RECON01,		// �ʐMLOCK���@�Đڑ��O�P
	WFP2PMF_KO_STATUS_CONLOCK_RECONWAIT,	// �ʐMLOCK���@�Đڑ��҂�
	WFP2PMF_KO_STATUS_CONLOCK_GRAPHIC_RESET00,	// �ʐMLOCK���@��ʍ\�z
	WFP2PMF_KO_STATUS_CONLOCK_GRAPHIC_RESET01,	// �ʐMLOCK���@��ʍ\�z
	WFP2PMF_KO_STATUS_CONLOCK_DECON,		// �ʐMLOCK���@�ؒf
	WFP2PMF_KO_STATUS_NUM
} ;

//-------------------------------------
///	�`��I�u�W�F�N�g����V�[�P���X
//=====================================
enum {
	WFP2PMF_DRAWOBJ_MOVENONE,
	WFP2PMF_DRAWOBJ_MOVEINIT,
	WFP2PMF_DRAWOBJ_MOVEIN,
	WFP2PMF_DRAWOBJ_MOVENORMAL,
	WFP2PMF_DRAWOBJ_MOVENUM,
} ;
#define WFP2PMF_DRAWOBJ_INTIME	( 16 )	// ���o�G�t�F�N�g�^�C��
// �e������INTIME�̍�
static const u8 WFP2PMF_DRAWOBJ_INTIME_DIV[ WF2DMAP_WAY_NUM ] = {
	8,
	0,
	4,
	12,
};



//-------------------------------------
///	�G���g���[�t���O
//=====================================
enum {
	WFP2PMF_ENTRY_WAIT,
	WFP2PMF_ENTRY_KO,
	WFP2PMF_ENTRY_NG,
	WFP2PMF_ENTRY_LOCK,
} ;


//-------------------------------------
///	OAM�\���V�[�P���X
//=====================================
enum {
	WFP2PMF_OAM_SEQ_NONE,
	WFP2PMF_OAM_SEQ_INIT,
	WFP2PMF_OAM_SEQ_WAIT,
} ;



//-------------------------------------
///		BGL
//=====================================
#define WFP2PMF_BACK_PAL		(0)	// �w�i�p���b�g
#define WFP2PMF_BACK_PALNUM		(18)// �A�j���̐�
#define WFP2PMF_BACK_PALANM	(3)// �A�j���t���[��
#define WFP2PMF_TALK_PAL	(1)	// �����p���b�g
// �p���b�g�A�j���f�[�^
static const u8 WFP2PMF_BACK_PAL_IDX[ WFP2PMF_BACK_PALNUM ] = {
	5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 5, 5,
};

// VCHATICON
#define WFP2PMF_VCHAT_ICON_FRM_NUM	( GF_BGL_FRAME2_M )
#define WFP2PMF_VCHAT_ICON_PAL		( 5 )
#define WFP2PMF_VCHAT_ICON_PALNUM	( 1 )
#define WFP2PMF_VCHAT_ICON_CG		( 0 )
#define WFP2PMF_VCHAT_ICON_CGSIZ	( 24*2 )
#define WFP2PMF_VCHAT_ICON_SIZ		(2)
#define WFP2PMF_VCHAT_ICON_WX		(18)
#define WFP2PMF_VCHAT_ICON_WY		(5)
enum{
	WFP2PMF_VCHATICON_NONE,
	WFP2PMF_VCHATICON_OFF,
	WFP2PMF_VCHATICON_ON,
};


// BG1
#define WFP2PMF_BG1_TALKWIN_CGX	( 1 )
#define WFP2PMF_BG1_TALKWIN_PAL	( 2 )
#define WFP2PMF_BG1_MENUWIN_CGX	( WFP2PMF_BG1_TALKWIN_CGX+TALK_WIN_CGX_SIZ )
#define WFP2PMF_BG1_MENUWIN_PAL	( 3 )

// BG2
#define WFP2PMF_BG2_MENUWIN_CGX	( WFP2PMF_VCHAT_ICON_CG+WFP2PMF_VCHAT_ICON_CGSIZ )
#define WFP2PMF_BG2_MENUWIN_PAL	( 4 )

// ���b�Z�[�W�E�B���h�E
#define WFP2PMF_MSGWIN_X		( 2 )
#define WFP2PMF_MSGWIN_Y		( 19 )
#define WFP2PMF_MSGWIN_SIZX		( 27 )
#define WFP2PMF_MSGWIN_SIZY		( 4 )
#define WFP2PMF_MSGWIN_PAL		( WFP2PMF_TALK_PAL )
#define WFP2PMF_MSGWIN_CGX		( WFP2PMF_BG1_MENUWIN_CGX + MENU_WIN_CGX_SIZ )
#define WFP2PMF_MSGWIN_SIZ		( WFP2PMF_MSGWIN_SIZX*WFP2PMF_MSGWIN_SIZY )

// �^�C�g���E�B���h�E
#define WFP2PMF_TITLEWIN_X		( 1 )
#define WFP2PMF_TITLEWIN_Y		( 1 )
#define WFP2PMF_TITLEWIN_SIZX	( 27 )
#define WFP2PMF_TITLEWIN_SIZY	( 2 )
#define WFP2PMF_TITLEWIN_PAL	( WFP2PMF_TALK_PAL )
#define WFP2PMF_TITLEWIN_CGX	( WFP2PMF_MSGWIN_CGX + WFP2PMF_MSGWIN_SIZ )
#define WFP2PMF_TITLEWIN_SIZ	( WFP2PMF_TITLEWIN_SIZX*WFP2PMF_TITLEWIN_SIZY )

// �{�C�X�`���b�g�E�B���h�E
#define WFP2PMF_VCHATWIN_X		( 2 )
#define WFP2PMF_VCHATWIN_Y		( 16 )
#define WFP2PMF_VCHATWIN_SIZX	( 28 )
#define WFP2PMF_VCHATWIN_SIZY	( 2 )
#define WFP2PMF_VCHATWIN_PAL	( WFP2PMF_TALK_PAL )
#define WFP2PMF_VCHATWIN_CGX	( WFP2PMF_TITLEWIN_CGX + WFP2PMF_TITLEWIN_SIZ )
#define WFP2PMF_VCHATWIN_SIZ	( WFP2PMF_VCHATWIN_SIZX*WFP2PMF_VCHATWIN_SIZY )

// CONNECT���X�g�E�B���h�E
#define WFP2PMF_CONLISTWIN_X		( 1 )
#define WFP2PMF_CONLISTWIN_Y		( 5 )
#define WFP2PMF_CONLISTWIN_SIZX	( 20 )
#define WFP2PMF_CONLISTWIN_SIZY	( 8 )
#define WFP2PMF_CONLISTWIN_PAL	( WFP2PMF_TALK_PAL )
#define WFP2PMF_CONLISTWIN_CGX	( WFP2PMF_VCHATWIN_CGX + WFP2PMF_VCHATWIN_SIZ )
#define WFP2PMF_CONLISTWIN_SIZ	( WFP2PMF_CONLISTWIN_SIZX*WFP2PMF_CONLISTWIN_SIZY )
#define WFP2PMF_CONLISTWIN_ONELIST_Y	( 2 )
#define WFP2PMF_CONLISTWIN_ID_X	( 72 )

// NEWCONNECT���X�g�E�B���h�E
#define WFP2PMF_NEWCONWIN_X		( 23 )
#define WFP2PMF_NEWCONWIN_Y		( 5 )
#define WFP2PMF_NEWCONWIN_SIZX	( 7 )
#define WFP2PMF_NEWCONWIN_SIZY	( 5 )
#define WFP2PMF_NEWCONWIN_PAL	( WFP2PMF_TALK_PAL )
#define WFP2PMF_NEWCONWIN_CGX	( WFP2PMF_CONLISTWIN_CGX + WFP2PMF_CONLISTWIN_SIZ )
#define WFP2PMF_NEWCONWIN_SIZ	( WFP2PMF_NEWCONWIN_SIZX*WFP2PMF_NEWCONWIN_SIZY )

// yes no win
#define WFP2PMF_YESNOWIN_FRM_NUM	( GF_BGL_FRAME2_M )
#define WFP2PMF_YESNOWIN_X			( 25 )
#define WFP2PMF_YESNOWIN_Y			( 13 )
#define WFP2PMF_YESNOWIN_SIZX		( 5 )
#define WFP2PMF_YESNOWIN_SIZY		( 4 )
#define WFP2PMF_YESNOWIN_PAL		( WFP2PMF_TALK_PAL )
#define WFP2PMF_YESNOWIN_CGX		( WFP2PMF_BG2_MENUWIN_CGX + MENU_WIN_CGX_SIZ )
#define WFP2PMF_YESNOWIN_SIZ		( WFP2PMF_YESNOWIN_SIZX * WFP2PMF_YESNOWIN_SIZY )

static const BMPWIN_DAT WFP2PMF_YESNOBMPDAT = {
	WFP2PMF_YESNOWIN_FRM_NUM, WFP2PMF_YESNOWIN_X, WFP2PMF_YESNOWIN_Y,
	WFP2PMF_YESNOWIN_SIZX, WFP2PMF_YESNOWIN_SIZY, 
	WFP2PMF_YESNOWIN_PAL, WFP2PMF_YESNOWIN_CGX
};

#define	_COL_N_BLACK	( GF_PRINTCOLOR_MAKE( 1, 2, 15 ) )		// �t�H���g�J���[�F��
#define	_COL_N_BLACK_C	( GF_PRINTCOLOR_MAKE( 1, 2, 0 ) )		// �t�H���g�J���[�F��


//-------------------------------------
///	���b�Z�[�W�f�[�^ msgdata
//=====================================
#define WFP2PMF_MSGDATA_STRNUM	 ( 256 )
#define WFP2PMF_MSGNO_NONE		(0xff)
#define WFP2PMF_MSGENDWAIT		( 30 )		//  ���b�Z�[�W�\����҂E�G�C�g



//-------------------------------------
///	�^�C�g���f�[�^
//=====================================
static u8 WFP2PMF_TITLE[ WFP2PMF_TYPE_NUM ] = {
	msg_wifilobby_131,
	msg_wifilobby_150,
	msg_wifilobby_150,
	msg_wifilobby_150,
};


//-------------------------------------
///	�I�u�W�F�N�g�o�^�f�[�^
//=====================================
static const WF2DMAP_OBJDATA WFP2PMF_OBJData[ WIFI_P2PMATCH_NUM_MAX ] = {
	{// �e
		208, 48, 0, WF2DMAP_OBJST_NONE, 
		WF2DMAP_WAY_DOWN, 0
	},
	{
		232, 72, 1, WF2DMAP_OBJST_NONE, 
		WF2DMAP_WAY_LEFT, 0
	},
	{
		208, 96, 2, WF2DMAP_OBJST_NONE, 
		WF2DMAP_WAY_UP, 0
	},
	{
		184, 72, 3, WF2DMAP_OBJST_NONE, 
		WF2DMAP_WAY_RIGHT, 0
	},
};



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�f�[�^
//=====================================
typedef struct {
	TEST_MATCH_WORK*	p_match;		// �T�[�o�[�Ƒ���M���鎩���ƗF�B�̏��
	u8					oya;			// �e���ǂ���
	u8					status;			// ���
	u8					new_con;		// �V�����R�l�N�g
	u8					match_result;	// ��ɕԂ�����
	s32					timer;			// �^�C�}�[
	s32					timeout;		// �^�C���A�E�g
	u8					timeout_flag;	// �^�C���A�E�g�̗L��
	u8					vchat_flag;		// Vchat�t���O
	u8					ko_newcon_flag;	// �q�p�V�K�ڑ������������`�F�b�N
	u8					entry;			// �G���g���[�t���O
	u8					logout_in[WIFI_P2PMATCH_NUM_MAX];// LOG�A�E�g���m
	u8					logout_id[WIFI_P2PMATCH_NUM_MAX];// LOG�A�E�g�����l��ID
	u8					vchat_tmp[WIFI_P2PMATCH_NUM_MAX];	// �P�O��VCHAT
	u8					vchat_now[WIFI_P2PMATCH_NUM_MAX];	// �P�O��VCHAT
	u8					conlist_rewrite;// conlist���ĕ`��
	u8					last_netid;		// �V�K�R�l�N�g���b�N�t���O
	u8					oya_friendid;	// �e�̗F�B�R�[�h
	u8					err_check;		// �G���[�`�F�b�N���K�v��
	u8					entry_on[WIFI_P2PMATCH_NUM_MAX];// �G���g���[�t���O
	u8					oya_start;		// �e����Q�[���J�n����M
	u8					ng_msg_idx;		// NG���b�Z�[�W
} WFP2PMF_DATA;

//-------------------------------------
///	�@�`��I�u�W�F�N�g���[�N
//=====================================
typedef struct {
	WF2DMAP_OBJWK*		p_objwk;		// �I�u�W�F�N�g�V�X�e��
	WF2DMAP_OBJDRAWWK*	p_objdrawwk;	// �I�u�W�F�N�g�`�惏�[�N
	u16 seq;
	s16 count;
} WFP2PMF_DRAWOBJ;


//-------------------------------------
///	�`��f�[�^
//=====================================
typedef struct {
	GF_BGL_INI*			p_bgl;				// GF_BGL_INI
	WORDSET*			p_wordset;			// ���b�Z�[�W�W�J�p���[�N�}�l�[�W���[
	MSGDATA_MANAGER*	p_msgman;			// ���b�Z�[�W�f�[�^�}�l�[�W���[
	STRBUF*				p_msgstr;			// ���b�Z�[�W�p������o�b�t�@
	STRBUF*				p_msgtmp;			// ���b�Z�[�W�p������o�b�t�@
	u8					msg_no;				// ���b�Z�[�W���������i���o�[
	u8					msg_wait;			// ���b�Z�[�W�E�G�C�g
	u16					msg_speed;			// ���b�Z�[�W�X�s�[�h

	CLACT_SET_PTR 			clactSet;						// �Z���A�N�^�[�Z�b�g
	CLACT_U_EASYRENDER_DATA	renddata;						// �ȈՃ����_�[�f�[�^
	CLACT_U_RES_MANAGER_PTR	resMan[WIFI_P2PMATCH_RESNUM];	// �L�����E�p���b�g���\�[�X�}�l�[�W��

	WF2DMAP_OBJSYS*		p_objsys;			// �I�u�W�F�N�g�V�X�e��
	WF2DMAP_OBJDRAWSYS* p_objdraw;			// �I�u�W�F�N�g�`��V�X�e��
	WFP2PMF_DRAWOBJ		objdata[WIFI_P2PMATCH_CLACTWK_NUM];	// �I�u�W�F�N�g�f�[�^


	GF_BGL_BMPWIN		msgwin;				// ��b�E�C���h�E
	GF_BGL_BMPWIN		titlewin;			// �����̏�ԕ\��
	GF_BGL_BMPWIN		vchatwin;			// �{�C�X�`���b�g�E�B���h�E
	GF_BGL_BMPWIN		conlistwin;			// �R�l�N�g���X�g�E�B���h�E
	GF_BGL_BMPWIN		newconwin;			// �V�K�R�l�N�g�E�B���h�E
	BMPMENU_WORK*		p_yesnowin;			// yes no�E�B���h�E

	void* p_vchatscrnbuf;
	NNSG2dScreenData*  p_vchatscrn;			// �{�C�X�`���b�g�A�C�R��

	void* p_timewaiticon;

	// �w�i�A�j��
	void*				p_plttbuff;
	NNSG2dPaletteData*	p_pltt;
	s16					pltt_count;
	s16					pltt_idx;
} WFP2PMF_DRAW;


//-------------------------------------
///	�S�l�}�b�`���O��ʃ��[�N
//=====================================
typedef struct _WFP2P_WK{
	WFP2PMF_DATA	data;
	WFP2PMF_DRAW	draw;
} WFP2PMF_WK;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void VBlankFunc( void * work );

static BOOL WFP2PMF_MyDataOyaCheck( const WFP2PMF_DATA* cp_data );
static void WFP2PMF_GraphicInit( WFP2PMF_WK* p_wk, const WFP2PMF_INIT* cp_init, u32 heapID );
static void WFP2PMF_GraphicDelete( WFP2PMF_WK* p_wk, u32 heapID );
static void WFP2PMF_GraphicBankSet( void );
static void WFP2PMF_GraphicBGLInit( WFP2PMF_DRAW* p_draw, u32 heapID );
static void WFP2PMF_GraphicBGLDelete( WFP2PMF_DRAW* p_draw );
static void WFP2PMF_GraphicBmpInit( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 heapID );
static void WFP2PMF_GraphicBmpDelete( WFP2PMF_DRAW* p_draw, u32 heapID );
static void WFP2PMF_GraphicMsgInit( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 heapID );
static void WFP2PMF_GraphicMsgDelete( WFP2PMF_DRAW* p_draw );
static void WFP2PMF_GraphicBmpMsgInit( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 heapID );
static void WFP2PMF_GraphicClactInit( WFP2PMF_DRAW* p_draw, u32 heapID );
static void WFP2PMF_GraphicClactDelete( WFP2PMF_DRAW* p_draw );

static void WFP2PMF_GraphicMsgBmpStrPut( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 msg_idx, u32 heapID );
static void WFP2PMF_GraphicMsgBmpStrPutEx( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 msg_idx, u8 wait, u32 heapID );
static void WFP2PMF_GraphicMsgBmpStrPutAll( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 msg_idx, u32 heapID );
static BOOL WFP2PMF_GraphicMsgBmpStrMain( WFP2PMF_DRAW* p_draw );
static void WFP2PMF_GraphicMsgBmpStrOff( WFP2PMF_DRAW* p_draw );
static void WFP2PMF_GraphicNewConStrPut( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, u32 heapID );
static void WFP2PMF_GraphicNewConStrOff( WFP2PMF_DRAW* p_draw );
static void WFP2PMF_GraphicConlistStrPutAll( WFP2PMF_WK* p_wk, const WFP2PMF_INIT* cp_init, u32 heapID );
static void WFP2PMF_GraphicConlistStrPut( WFP2PMF_WK* p_wk, WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, u32 heapID );
static void WFP2PMF_GraphicConlistStrOff( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, u32 heapID );

static void WFP2PMF_WordSetPlayerNameSet( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, int set_no, u32 heapID );
static void WFP2PMF_WordSetPlayerIDSet( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, int set_no, u32 heapID );

static void WFP2PMF_PlttAnm( WFP2PMF_DRAW* p_draw );
static void WFP2PMF_PlttAnm_Trans( WFP2PMF_DRAW* p_draw, u32 idx );

static BOOL WFP2PMF_VchatSwitch( WFP2PMF_WK* p_wk, u32 heapID );


static void WFP2PMF_ErrCheck( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_param, u32 heapID );
static void WFP2PMF_ReConErrCheck( WFP2PMF_WK* p_wk );
static BOOL WFP2PMF_MatchCheck( const WFP2PMF_WK* cp_wk );
static BOOL WFP2PMF_CommWiFiMatchStart( u32 friendno, u32 type );
static BOOL WFP2PMF_CheckP2PMatchFriendStatus( const WFP2PMF_WK* cp_wk, const WFP2PMF_INIT* cp_param, int friend );

static void WFP2PMF_StatusChange( WFP2PMF_WK* p_wk, const WFP2PMF_INIT* cp_init );

static BOOL WFP2PMF_StatusVChatOn( WFP2PMF_WK* p_wk );
static BOOL WFP2PMF_StatusVChatOff( WFP2PMF_WK* p_wk );
static void WFP2PMF_StatusVChatOnBmpFade( WFP2PMF_WK* p_wk );
static void WFP2PMF_StatusVChatOffBmpFade( WFP2PMF_WK* p_wk );

static int WFP2PMF_GetEntryNum( WFP2PMF_WK* p_wk );
static BOOL WFP2PMF_SetEntryOya( WFP2PMF_WK* p_wk, u32 heapID );

static void WFP2PMF_CommSendNewEntryNg( WFP2PMF_WK* p_wk );
static void WFP2PMF_CommSendAllEntryNg( WFP2PMF_WK* p_wk );

static void WFP2PMF_LogOutCheckStart( WFP2PMF_WK* p_wk );
static void WFP2PMF_LogOutCheckEnd( WFP2PMF_WK* p_wk );
static BOOL WFP2PMF_LogOutCheck( WFP2PMF_WK* p_wk );
static void WFP2PMF_LogOutCallBack( u16 aid, void* pWork );

static BOOL WFP2PMF_ConListWriteCheck( WFP2PMF_WK* p_wk );
static void WFP2PMF_ConListWriteReq( WFP2PMF_WK* p_wk );

static void WFP2PMF_CommInfoSetEntry( WFP2PMF_WK* p_wk, u32 aid, u32 heapID );

static void WFP2PMF_DrawOamInit( WFP2PMF_WK* p_wk, u32 aid, u32 heapID );
static void WFP2PMF_DrawOamMain( WFP2PMF_WK* p_wk );
static void WFP2PMF_DrawOamDelete( WFP2PMF_WK* p_wk );

static BOOL WFP2PMF_NewConLockCheck( const WFP2PMF_WK* cp_wk );

static void WFP2PMF_NewConLockSend( WFP2PMF_WK* p_wk, u32 aid );
static void WFP2PMF_NewConLockNewCon( WFP2PMF_WK* p_wk );

static void WFP2PMF_KoRecvEntry( WFP2PMF_WK* p_wk, u32 heapID );

static void WFP2PMF_TimeWaitIconOn( WFP2PMF_WK* p_wk );
static void WFP2PMF_TimeWaitIconOff( WFP2PMF_WK* p_wk, const WFP2PMF_INIT* cp_init, u32 heapID );

static void WFP2PMF_DrawObjMoveNone( WFP2PMF_WK* p_wk, WFP2PMF_DRAWOBJ* p_obj );
static void WFP2PMF_DrawObjMoveInit( WFP2PMF_WK* p_wk, WFP2PMF_DRAWOBJ* p_obj );
static void WFP2PMF_DrawObjMoveIn( WFP2PMF_WK* p_wk, WFP2PMF_DRAWOBJ* p_obj );
static void WFP2PMF_DrawObjMoveNormal( WFP2PMF_WK* p_wk, WFP2PMF_DRAWOBJ* p_obj );

static void WFP2PMF_VchatCommSend( WFP2PMF_WK* p_wk );

static BOOL WFP2PMF_MatchOkCheck( WFP2PMF_WK* p_wk );

static void WFP2PMF_FNOTE_Set( WFP2PMF_INIT* p_param, u32 heapID );

static BOOL WFP2PMF_Oya_CheckConnectPlayer( const WFP2PMF_WK* cp_wk, const WFP2PMF_INIT* cp_param );


static void WFP2PMF_TimeOut_Start( WFP2PMF_WK* p_wk );
static void WFP2PMF_TimeOut_Stop( WFP2PMF_WK* p_wk );
static BOOL WFP2PMF_TimeOut_Main( WFP2PMF_WK* p_wk );

//-------------------------------------
/// �e����f�[�^
//=====================================
static BOOL WFP2PMF_OyaInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaConnectWaitInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaConnectWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaConnectSelectInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaConnectSelectInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaConnectSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaConnectMinInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaConnectMin( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaStartInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaStartInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaStartSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );		
static BOOL WFP2PMF_OyaStart( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );				
static BOOL WFP2PMF_OyaStartNumCheck( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaStartSyncInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaStartSync( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaStateSend( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaStateCheck( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaMyStatusWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaGameStart( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_OyaEndInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaEndInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaEndSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaEnd2Init00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaEnd2Init01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaEnd2Select( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaEnd3Init( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaEnd( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );					
static BOOL WFP2PMF_OyaErrInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaErrInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaErr( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );					
static BOOL WFP2PMF_OyaVchatInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaVchatInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );			
static BOOL WFP2PMF_OyaVchatSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );		




//-------------------------------------
/// �q����f�[�^
//=====================================
static BOOL WFP2PMF_KoEntryOyaWaitInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoEntryOyaWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoEntryInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoEntryWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoEntryOkInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoEntryOk( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoEntryNgInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoEntryNg( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoConnectInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoConnectWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoStart( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );	   
static BOOL WFP2PMF_KoStateSend( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoStateCheck( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoMyStatusWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoGameStart( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoErrInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );	   
static BOOL WFP2PMF_KoErrInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );	   
static BOOL WFP2PMF_KoErr( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );		   
static BOOL WFP2PMF_KoVchatInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );  
static BOOL WFP2PMF_KoVchatInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );  
static BOOL WFP2PMF_KoVchatSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoConLockInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );  
static BOOL WFP2PMF_KoConLockInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );  
static BOOL WFP2PMF_KoConLockSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoConLockReCon( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoConLockReCon01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoConLockReConWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoConLockReConGraphicReset00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoConLockReConGraphicReset01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );
static BOOL WFP2PMF_KoConLockDeCon( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID );


//-------------------------------------
///	�e�֐��e�[�u��
//=====================================
static BOOL (* const pOyaFunc[ WFP2PMF_OYA_STATUS_NUM ])( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID ) = {
	WFP2PMF_OyaInit,								// WFP2PMF_OYA_STATUS_INIT,				// �e�̏�����
	WFP2PMF_OyaConnectWaitInit,                     // WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT,	// CONNECT�҂�������
	WFP2PMF_OyaConnectWait,                         // WFP2PMF_OYA_STATUS_CONNECT_WAIT,		// CONNECT�҂�
	WFP2PMF_OyaConnectSelectInit00,                 // WFP2PMF_OYA_STATUS_CONNECT_SELECT_INIT00,// CONNECT�҂�
	WFP2PMF_OyaConnectSelectInit01,                 // WFP2PMF_OYA_STATUS_CONNECT_SELECT_INIT01,// CONNECT�����l�����邩�I��������
	WFP2PMF_OyaConnectSelect,                       // WFP2PMF_OYA_STATUS_CONNECT_SELECT,		// CONNECT�����l�����邩�I��
	WFP2PMF_OyaConnectMinInit,                      // WFP2PMF_OYA_STATUS_CONNECT_MIN_INIT,	// �Q�[���J�n���������A�l��������Ȃ�
	WFP2PMF_OyaConnectMin,                          // WFP2PMF_OYA_STATUS_CONNECT_MIN,			// �Q�[���J�n���������A�l��������Ȃ�
	WFP2PMF_OyaStartInit00,                         // WFP2PMF_OYA_STATUS_START_INIT00,			// �Q�[���J�n�����@�J�n
	WFP2PMF_OyaStartInit01,                         // WFP2PMF_OYA_STATUS_START_INIT01,			// �Q�[���J�n�����@�J�n
	WFP2PMF_OyaStartSelect,                         // WFP2PMF_OYA_STATUS_START_SELECT,		// �Q�[���J�n�����ɍs�����I��
	WFP2PMF_OyaStart,                               // WFP2PMF_OYA_STATUS_START,				// �Q�[���J�n��
	WFP2PMF_OyaStartNumCheck,                       // WFP2PMF_OYA_STATUS_STARTNUMCHECK,		// �Q�[���J�n�@�l���`�F�b�N
	WFP2PMF_OyaStartSyncInit,                       // WFP2PMF_OYA_STATUS_STARTSYNC_INIT,		// �Q�[���J�n����
	WFP2PMF_OyaStartSync,                           // WFP2PMF_OYA_STATUS_STARTSYNC,			// �Q�[���J�n����
	WFP2PMF_OyaStateSend,                           // WFP2PMF_OYA_STATUS_STATE_SEND,			// �Q�[���J�n	�X�e�[�^�X���M
	WFP2PMF_OyaStateCheck,                          // WFP2PMF_OYA_STATUS_STATE_CHECK,			// �Q�[���J�n	�X�e�[�^�X�`�F�b�N
	WFP2PMF_OyaMyStatusWait,                        // WFP2PMF_OYA_STATUS_MYSTATUS_WAIT,		// �Q�[���J�n	�f�[�^�҂�
	WFP2PMF_OyaGameStart,	                        // WFP2PMF_OYA_STATUS_GAME_START,			// �J�n
	WFP2PMF_OyaEndInit00,                           // WFP2PMF_OYA_STATUS_END_INIT00,			// �ҋ@��Ԃ���߂邩�@�J�n
	WFP2PMF_OyaEndInit01,                           // WFP2PMF_OYA_STATUS_END_INIT01,			// �ҋ@��Ԃ���߂邩�@�J�n
	WFP2PMF_OyaEndSelect,                           // WFP2PMF_OYA_STATUS_END_SELECT,			// �ҋ@��Ԃ���߂邩�@�I��
	WFP2PMF_OyaEnd2Init00,                          // WFP2PMF_OYA_STATUS_END2_INIT00,			// �ҋ@��Ԃ���߂�Ɓ@���U�@�J�n
	WFP2PMF_OyaEnd2Init01,                          // WFP2PMF_OYA_STATUS_END2_INIT01,			// �ҋ@��Ԃ���߂�Ɓ@���U�@�J�n
	WFP2PMF_OyaEnd2Select,                          // WFP2PMF_OYA_STATUS_END2_SELECT,			// �ҋ@��Ԃ���߂�Ɓ@���U�@�I��
	WFP2PMF_OyaEnd3Init,                            // WFP2PMF_OYA_STATUS_END3_INIT,			// �ҋ@��Ԃ���߂�Ɓ@���U�@�J�n
	WFP2PMF_OyaEnd,                                 // WFP2PMF_OYA_STATUS_END,					// �ҋ@��ԏI��
	WFP2PMF_OyaErrInit,                             // WFP2PMF_OYA_STATUS_ERR_INIT,			// �ʐM�G���[�ɂ��I�� �J�n
	WFP2PMF_OyaErrInit01,                           // WFP2PMF_OYA_STATUS_ERR_INIT01,			// �ʐM�G���[�ɂ��I�� �J�n
	WFP2PMF_OyaErr,                                 // WFP2PMF_OYA_STATUS_ERR,					// �ʐM�G���[�ɂ��I��
	WFP2PMF_OyaVchatInit00,                         // WFP2PMF_OYA_STATUS_VCHAT_INIT00,		// �{�C�X�`���b�g�ύX	�J�n
	WFP2PMF_OyaVchatInit01,                         // WFP2PMF_OYA_STATUS_VCHAT_INIT01,		// �{�C�X�`���b�g�ύX	�J�n
	WFP2PMF_OyaVchatSelect,                         // WFP2PMF_OYA_STATUS_VCHAT_SELECT,		// �{�C�X�`���b�g�ύX	�I��
};

//-------------------------------------
///	�q�֐��e�[�u��
//=====================================
static BOOL (* const pKoFunc[ WFP2PMF_KO_STATUS_NUM ])( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID ) = {
	WFP2PMF_KoEntryOyaWaitInit,					// WFP2PMF_KO_STATUS_ENTRY_OYAWAITINIT,// �e�̏��擾�҂�
	WFP2PMF_KoEntryOyaWait,                     // WFP2PMF_KO_STATUS_ENTRY_OYAWAIT,	// �e�̏��擾�҂�
	WFP2PMF_KoEntryInit,                        // WFP2PMF_KO_STATUS_ENTRY_INIT,		// ENTRY�����҂��J�n
	WFP2PMF_KoEntryWait,                        // WFP2PMF_KO_STATUS_ENTRY_WAIT,		// ENTRY�����҂�
	WFP2PMF_KoEntryOkInit,                      // WFP2PMF_KO_STATUS_ENTRY_OKINIT,		// ENTRYOK������
	WFP2PMF_KoEntryOk,                          // WFP2PMF_KO_STATUS_ENTRY_OK,			// ENTRYOK
	WFP2PMF_KoEntryNgInit,                      // WFP2PMF_KO_STATUS_ENTRY_NGINIT,		// ENTRYNG������
	WFP2PMF_KoEntryNg,                          // WFP2PMF_KO_STATUS_ENTRY_NG,			// ENTRYNG
	WFP2PMF_KoConnectInit,                      // WFP2PMF_KO_STATUS_CONNECT_INIT,		// CONNECT�����҂��J�n
	WFP2PMF_KoConnectWait,                      // WFP2PMF_KO_STATUS_CONNECT_WAIT,		// CONNECT�����҂�
	WFP2PMF_KoStart,                            // WFP2PMF_KO_STATUS_START,			// �J�n
	WFP2PMF_KoStateSend,                        // WFP2PMF_KO_STATUS_STATE_SEND,			// �Q�[���J�n	�X�e�[�^�X���M
	WFP2PMF_KoStateCheck,                       // WFP2PMF_KO_STATUS_STATE_CHECK,			// �Q�[���J�n	�X�e�[�^�X�`�F�b�N
	WFP2PMF_KoMyStatusWait,                     // WFP2PMF_KO_STATUS_MYSTATUS_WAIT,		// �Q�[���J�n	�f�[�^�҂�
	WFP2PMF_KoGameStart,	                    // WFP2PMF_KO_STATUS_GAME_START,			// �J�n
	WFP2PMF_KoErrInit,                          // WFP2PMF_KO_STATUS_ERR_INIT,			// �G���[�I��	�J�n
	WFP2PMF_KoErrInit01,                        // WFP2PMF_KO_STATUS_ERR_INIT01,		// �G���[�I��	�J�n
	WFP2PMF_KoErr,                              // WFP2PMF_KO_STATUS_ERR,				// �G���[�I��
	WFP2PMF_KoVchatInit00,                      // WFP2PMF_KO_STATUS_VCHAT_INIT00,		// �{�C�X�`���b�g�ύX	�J�n
	WFP2PMF_KoVchatInit01,                      // WFP2PMF_KO_STATUS_VCHAT_INIT01,		// �{�C�X�`���b�g�ύX	�J�n
	WFP2PMF_KoVchatSelect,                      // WFP2PMF_KO_STATUS_VCHAT_SELECT,		// �{�C�X�`���b�g�ύX	�I��
	WFP2PMF_KoConLockInit00,                    // WFP2PMF_KO_STATUS_CONLOCK_INIT00,		// �ʐMLOCK���Ȃ̂ōĐڑ�
	WFP2PMF_KoConLockInit01,                    // WFP2PMF_KO_STATUS_CONLOCK_INIT01,		// �ʐMLOCK���Ȃ̂ōĐڑ�
	WFP2PMF_KoConLockSelect,                    // WFP2PMF_KO_STATUS_CONLOCK_SELECT,		// �ʐMLOCK���Ȃ̂ōĐڑ�
	WFP2PMF_KoConLockReCon,                     // WFP2PMF_KO_STATUS_CONLOCK_RECON,		// �ʐMLOCK���@�Đڑ�
	WFP2PMF_KoConLockReCon01,                   // WFP2PMF_KO_STATUS_CONLOCK_RECON01,		// �ʐMLOCK���@�Đڑ��O�P
	WFP2PMF_KoConLockReConWait,                 // WFP2PMF_KO_STATUS_CONLOCK_RECONWAIT,	// �ʐMLOCK���@�Đڑ��҂�
	WFP2PMF_KoConLockReConGraphicReset00,       // WFP2PMF_KO_STATUS_CONLOCK_GRAPHIC_RESET00,	// �ʐMLOCK���@��ʍ\�z
	WFP2PMF_KoConLockReConGraphicReset01,       // WFP2PMF_KO_STATUS_CONLOCK_GRAPHIC_RESET01,	// �ʐMLOCK���@��ʍ\�z
	WFP2PMF_KoConLockDeCon,                     // WFP2PMF_KO_STATUS_CONLOCK_DECON,		// �ʐMLOCK���@�ؒf
};


// �`��I�u�W�F�N�g����
static void (* const pDrawObjMoveFunc[ WFP2PMF_DRAWOBJ_MOVENUM ])( WFP2PMF_WK* p_wk, WFP2PMF_DRAWOBJ* p_obj ) = {
	WFP2PMF_DrawObjMoveNone,
	WFP2PMF_DrawObjMoveInit,
	WFP2PMF_DrawObjMoveIn,
	WFP2PMF_DrawObjMoveNormal,
};


//----------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F������
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//-----------------------------------------------------------------------------
PROC_RESULT WifiP2PMatchFourProc_Init( PROC * proc, int * seq )
{
	WFP2PMF_WK* p_wk;
	WFP2PMF_INIT* p_init = PROC_GetParentWork(proc);
	BOOL result;

	// wifi_2dmap�I�[�o�[���C�Ǎ���
	Overlay_Load( FS_OVERLAY_ID(wifi_2dmapsys), OVERLAY_LOAD_NOT_SYNCHRONIZE);

	// �ʐM�����`�F�b�N
	result = CommStateIsWifiConnect();
	GF_ASSERT( result == TRUE );

	// �q�[�v�쐬
    sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFI_FOURMATCH, 0x18000 );
    sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFI_FOURMATCHVCHAT, 0xa000 );

	// �v���Z�X���[�N�쐬
	p_wk = PROC_AllocWork( proc, sizeof(WFP2PMF_WK), HEAPID_WIFI_FOURMATCH );
	MI_CpuFill8( p_wk, 0, sizeof(WFP2PMF_WK) );


	// VCHAT���[�N�̏�����
	MI_CpuFill8( p_wk->data.vchat_tmp, TRUE, sizeof(u8)*WIFI_P2PMATCH_NUM_MAX );// �P�O��VCHAT
	MI_CpuFill8( p_wk->data.vchat_now, TRUE, sizeof(u8)*WIFI_P2PMATCH_NUM_MAX );// �P�O��VCHAT



	// VRAM�]���}�l�[�W��������
	initVramTransferManagerHeap( WIFI_P2PMATCH_VRAMTRMAN_NUM, HEAPID_WIFI_FOURMATCH );


	// �ʐM�f�[�^�擾
	p_wk->data.p_match = CommStateGetMatchWork();
	p_wk->data.p_match->myMatchStatus.vchat = p_wk->data.p_match->myMatchStatus.vchat_org;

	// �������e���`�F�b�N
	p_wk->data.oya = WFP2PMF_MyDataOyaCheck( &p_wk->data );

	// ���[�N������
	p_wk->data.new_con = INVALID_NETID;

	// �ʐM�R�}���h�ݒ�
	CommCommandWFP2PMFInitialize( p_wk );

	// �ؒf�`�F�b�N�J�n
	WFP2PMF_LogOutCheckStart( p_wk );


	// �`��f�[�^������
	WFP2PMF_GraphicInit( p_wk, p_init, HEAPID_WIFI_FOURMATCH );

    // VBlank�֐��Z�b�g
    sys_VBlankFuncChange( VBlankFunc, p_wk );

	WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��

	// �G���[�`�F�b�N�@�J�n
	p_wk->data.err_check = TRUE;


	return PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F���C�����[�v
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//-----------------------------------------------------------------------------
PROC_RESULT WifiP2PMatchFourProc_Main( PROC * proc, int * seq )
{
	WFP2PMF_WK* p_wk = PROC_GetWork( proc );
	WFP2PMF_INIT* p_init = PROC_GetParentWork(proc);
	BOOL result;

	// ���C������
	switch( *seq ){
	case WFP2PMF_MAIN_WIPE_S:
        // ���C�v�t�F�[�h�J�n
        WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK,
                        COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFI_FOURMATCH);
		(*seq)++;
		break;

	case WFP2PMF_MAIN_WIPE_E:
        if( WIPE_SYS_EndCheck() ){
            (*seq)++;
        }
		break;

	case WFP2PMF_MAIN_MATCHWAIT:

		// �G���[�`�F�b�N
		if( p_wk->data.err_check ){
			
			WFP2PMF_ErrCheck( p_wk, p_init, HEAPID_WIFI_FOURMATCH );
		}

		if( p_wk->data.oya ){

			// CommInfo�����邩���`�F�b�N
			if( CommInfoIsInitialize() == TRUE ){
				
				// �݂�Ȃ�VCHAT�t���O���`�F�b�N���đ��M
				WFP2PMF_VchatCommSend( p_wk );	

				// �V�KPOKEDATA����M������݂�Ȃɑ��M
				CommInfoSendArray_ServerSide();	// �݂�Ȃ���������POKEDATA�𑗐M

				// last_netID��ENTRY�ɂȂ�O�ɂ�����傫��NETID�̐l��NEWENTRY��
				// �Ȃ�����Đڑ����Ă��炤
				WFP2PMF_NewConLockNewCon( p_wk );
			}

			
			// �e�̓���
			result = pOyaFunc[ p_wk->data.status ]( p_wk, p_init, HEAPID_WIFI_FOURMATCH );
		}else{


			result = pKoFunc[ p_wk->data.status ]( p_wk, p_init, HEAPID_WIFI_FOURMATCH );

			// CommInfo�����邩���`�F�b�N
			if( CommInfoIsInitialize() == TRUE ){
				//  �����ȊO�̐l�̃G���g���[����
				WFP2PMF_KoRecvEntry( p_wk, HEAPID_WIFI_FOURMATCH );
			}
		}

		// ���X�g�̕`��`�F�b�N
		if( WFP2PMF_ConListWriteCheck( p_wk ) ){
			// ���X�g�̕`��
			WFP2PMF_GraphicConlistStrPutAll( p_wk, p_init, HEAPID_WIFI_FOURMATCH );
		}

		// OAM���C������
		WFP2PMF_DrawOamMain( p_wk );

		// �I��
		if( result == TRUE ){

			// �}�b�`���O���ʊi�[
			p_init->result = p_wk->data.match_result;
			p_init->vchat = p_wk->data.vchat_flag;

			(*seq)++;
		}
		break;

	case WFP2PMF_MAIN_EWIPE_S:
		// ���C�v�t�F�[�h�J�n
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK,
						COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFI_FOURMATCH);
		(*seq)++;
		break;
	case WFP2PMF_MAIN_EWIPE_E:
        if( WIPE_SYS_EndCheck() ){
			return PROC_RES_FINISH;
        }
		break;
	}

	// �p���b�g�A�j��
	WFP2PMF_PlttAnm( &p_wk->draw );


	// OAM�`�揈��
	CLACT_Draw( p_wk->draw.clactSet );
	
    return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F�I������
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//-----------------------------------------------------------------------------
PROC_RESULT WifiP2PMatchFourProc_End( PROC * proc, int * seq )
{
	WFP2PMF_WK* p_wk = PROC_GetWork( proc );
	WFP2PMF_INIT* p_init = PROC_GetParentWork(proc);


    sys_VBlankFuncChange( NULL, NULL );	// VBlankReset

	// LOGOUT�`�F�b�N�I��
	WFP2PMF_LogOutCheckEnd( p_wk );

	// OAM�j������
	WFP2PMF_DrawOamDelete( p_wk );

	// VCHAT���Ƃ߂�
	WFP2PMF_StatusVChatOff( p_wk );

	// �`��f�[�^�j��
	WFP2PMF_GraphicDelete( p_wk, HEAPID_WIFI_FOURMATCH );

	// VRAMTRMAN
	DellVramTransferManager();
	
	// �v���Z�X���[�N�j��
    PROC_FreeWork( proc );				// PROC���[�N�J��

    sys_DeleteHeap( HEAPID_WIFI_FOURMATCHVCHAT );
    sys_DeleteHeap( HEAPID_WIFI_FOURMATCH );

	// �I�[�o�[���C�j��
	Overlay_UnloadID( FS_OVERLAY_ID(wifi_2dmapsys) );

    return PROC_RES_FINISH;
}



// �ʐM�f�[�^��M
//----------------------------------------------------------------------------
/**	
 *	@brief	�ʐM�f�[�^��M
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 */
//-----------------------------------------------------------------------------
void WFP2PMF_CommResultRecv( WFP2P_WK* p_wk, const WFP2PMF_COMM_RESULT* cp_data )
{
	TOMOYA_PRINT( "recv result [%d]\n", cp_data->flag );
			
	if( p_wk->data.oya == FALSE ){
		if( cp_data->netID == CommGetCurrentID() ){

			
			switch( cp_data->flag ){
			case WFP2PMF_CON_OK:
				p_wk->data.entry = WFP2PMF_ENTRY_KO;
				break;
			
			case WFP2PMF_CON_NG:
				p_wk->data.entry = WFP2PMF_ENTRY_NG;
				break;
				
			case WFP2PMF_CON_LOCK:
				p_wk->data.entry = WFP2PMF_ENTRY_LOCK;
				break;
			}
			
		}else{

			switch( cp_data->flag ){
			case WFP2PMF_CON_OK:
				p_wk->data.entry_on[ cp_data->netID ] = TRUE;
				break;
			
			case WFP2PMF_CON_NG:
			case WFP2PMF_CON_LOCK:
				if( CommInfoIsInitialize() == TRUE ){
					CommInfoDeletePlayer( cp_data->netID );
				}
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���J�n�@�����J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFP2PMF_CommStartRecv( WFP2P_WK* p_wk )
{
	// 1�񂵂�������Ȃ��悤��
	if( p_wk->data.oya_start == FALSE ){
		p_wk->data.oya_start = TRUE;
		// �����J�n
		CommTimingSyncStart(_TIMING_GAME_CHECK);
	}
	OS_TPrintf( "sync start \n" );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[��VCHAT�f�[�^�@�擾
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFP2PMF_CommVchatRecv( WFP2P_WK* p_wk, const WFP2PMF_COMM_VCHAT* cp_data )
{
	if( p_wk->data.oya != TRUE ){	// �e����Ȃ��Ƃ�����
		memcpy( p_wk->data.vchat_now, cp_data->vchat_now, sizeof(u8)*WIFI_P2PMATCH_NUM_MAX );
	}
}



//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
// �@VBLANK
static void VBlankFunc( void * work )
{
	WFP2PMF_WK* p_wk = work;

    // BG��������
	GF_BGL_VBlankFunc( p_wk->draw.p_bgl );

    // Vram�]���}�l�[�W���[���s
    DoVramTransferManager();

    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃f�[�^���e���q���`�F�b�N
 *
 *	@param	cp_data			���[�N
 *
 *	@retval	TRUE	�e
 *	@retval	FALSE	�q
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_MyDataOyaCheck( const WFP2PMF_DATA* cp_data )
{
	if( (CommGetCurrentID() == COMM_PARENT_ID) ){
		// �ҋ@�Ȃ�e
		return TRUE;
	}
	// ����ȊO�͎q
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�f�[�^������
 *
 *	@param	p_wk		���[�N	
 *	@param	cp_init		�������f�[�^
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicInit( WFP2PMF_WK* p_wk, const WFP2PMF_INIT* cp_init, u32 heapID )
{
	// �o���N�ݒ�
	WFP2PMF_GraphicBankSet();
	
	// BGL�쐬
	WFP2PMF_GraphicBGLInit( &p_wk->draw, heapID );
	
	// �r�b�g�}�b�v�f�[�^�쐬
	WFP2PMF_GraphicBmpInit( &p_wk->draw, cp_init, heapID );

	// ���b�Z�[�W�f�[�^������
	WFP2PMF_GraphicMsgInit( &p_wk->draw, cp_init, heapID );

	// �r�b�g�}�b�v�Ƀ��b�Z�[�W����������
	WFP2PMF_GraphicBmpMsgInit( &p_wk->draw, cp_init, heapID );

	// ���X�g
	// �����̖��O��ID��ݒ肷��
	WFP2PMF_GraphicConlistStrPutAll( p_wk, cp_init, heapID );

	// �Z���A�N�^�[
	WFP2PMF_GraphicClactInit( &p_wk->draw, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�f�[�^�j��
 *
 *	@param	p_wk	���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicDelete( WFP2PMF_WK* p_wk, u32 heapID )
{
	// �Z���A�N�^�[
	WFP2PMF_GraphicClactDelete( &p_wk->draw );
	
	// ���b�Z�[�W�f�[�^�j��
	WFP2PMF_GraphicMsgDelete( &p_wk->draw );
	
	// �r�b�g�}�b�v�f�[�^�j��
	WFP2PMF_GraphicBmpDelete( &p_wk->draw, heapID );
	
	// BGL�j��
	WFP2PMF_GraphicBGLDelete( &p_wk->draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���N�ݒ�
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicBankSet( void )
{
    GF_BGL_DISPVRAM tbl = {
        GX_VRAM_BG_128_A,				// ���C��2D�G���W����BG
        GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g

        GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
        GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g

//        GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
		GX_VRAM_OBJ_128_B,				// ���C��2D�G���W����OBJ
        GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g

        GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
        GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g

        GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
        GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
    };
    GF_Disp_SetBank( &tbl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BGL������
 *		
 *	@param	p_draw		���[�N
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicBGLInit( WFP2PMF_DRAW* p_draw, u32 heapID )
{
	ARCHANDLE* p_handle;

	p_draw->p_bgl = GF_BGL_BglIniAlloc( heapID );

    // BG SYSTEM
    {
        GF_BGL_SYS_HEADER BGsys_data = {
            GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D,
        };
        GF_BGL_InitBG( &BGsys_data );
    }

    // �w�i��
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
            2, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( p_draw->p_bgl, GF_BGL_FRAME0_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME0_M, 32, 0, heapID);
        GF_BGL_ScrClear( p_draw->p_bgl, GF_BGL_FRAME0_M );


    }

    // ���C�����1
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
            1, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( p_draw->p_bgl, GF_BGL_FRAME1_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME1_M, 32, 0, heapID);
        GF_BGL_ScrClear( p_draw->p_bgl, GF_BGL_FRAME1_M );
    }

    // YesNo���X�g
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_23,
            0, 0, 0, FALSE
        };
        GF_BGL_BGControlSet( p_draw->p_bgl, GF_BGL_FRAME2_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME2_M, 32, 0, heapID);
        GF_BGL_ScrClear( p_draw->p_bgl, GF_BGL_FRAME2_M );
    }

    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );

	// �T�u�ʂ͕\��OFF
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_OFF );


	p_handle = ArchiveDataHandleOpen( ARC_WIFIP2PMATCH_GRA, heapID );

	// VCHAT�A�C�R���p�L�����N�^�̓Ǎ���
    ArcUtil_HDL_PalSet( p_handle, NARC_wifip2pmatch_wf_match_all_icon_NCLR, 
			PALTYPE_MAIN_BG,  WFP2PMF_VCHAT_ICON_PAL*32,
			WFP2PMF_VCHAT_ICON_PALNUM*32, heapID );
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_wf_match_all_icon_NCGR,
			p_draw->p_bgl, WFP2PMF_VCHAT_ICON_FRM_NUM, WFP2PMF_VCHAT_ICON_CG*32, WFP2PMF_VCHAT_ICON_CGSIZ*32, FALSE, heapID );
	p_draw->p_vchatscrnbuf = ArcUtil_HDL_ScrnDataGet( p_handle, 
			NARC_wifip2pmatch_wf_match_all_icon_NSCR, FALSE,
			&p_draw->p_vchatscrn, heapID );

	// �w�i�ǂݍ���
	{
		p_draw->p_plttbuff = ArcUtil_HDL_PalDataGet( p_handle, NARC_wifip2pmatch_wifi_check_bg_NCLR, 
				&p_draw->p_pltt, heapID );
		ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_wifi_check_bg_NCGR,
				p_draw->p_bgl, GF_BGL_FRAME0_M, 0, 0, FALSE, heapID );
		ArcUtil_HDL_ScrnSet( p_handle, NARC_wifip2pmatch_wifi_check_bg_NSCR,
				p_draw->p_bgl, GF_BGL_FRAME0_M, 0, 0, FALSE, heapID );

		// �p���b�g�A�j��������
		// �Ƃ肠�����A�O�t���[���ڂ𑗐M
		WFP2PMF_PlttAnm_Trans( p_draw, 0 );
		p_draw->pltt_count	= WFP2PMF_BACK_PALANM;
		p_draw->pltt_idx	= 1;
	}
	
	ArchiveDataHandleClose( p_handle );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BGL�j��
 *
 *	@param	p_draw	���[�N
 *
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicBGLDelete( WFP2PMF_DRAW* p_draw )
{
	sys_FreeMemoryEz( p_draw->p_vchatscrnbuf );
	sys_FreeMemoryEz( p_draw->p_plttbuff );
	
    GF_BGL_BGControlExit( p_draw->p_bgl, GF_BGL_FRAME2_M );
    GF_BGL_BGControlExit( p_draw->p_bgl, GF_BGL_FRAME1_M );
    GF_BGL_BGControlExit( p_draw->p_bgl, GF_BGL_FRAME0_M );

    sys_FreeMemoryEz( p_draw->p_bgl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v������
 *
 *	@param	p_draw		���[�N
 *	@param	cp_init		�������f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicBmpInit( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 heapID )
{
	int wintype;
	
	// �p���b�g�A�L�����N�^�f�[�^�]��
    TalkFontPaletteLoad( PALTYPE_MAIN_BG, WFP2PMF_TALK_PAL*0x20, heapID );
	wintype = CONFIG_GetWindowType(SaveData_GetConfig(cp_init->p_savedata)); 
	TalkWinGraphicSet(
		p_draw->p_bgl, GF_BGL_FRAME1_M, WFP2PMF_BG1_TALKWIN_CGX, WFP2PMF_BG1_TALKWIN_PAL,  wintype, heapID );
	MenuWinGraphicSet(
		p_draw->p_bgl, GF_BGL_FRAME1_M, WFP2PMF_BG1_MENUWIN_CGX, WFP2PMF_BG1_MENUWIN_PAL, 0, heapID );
	MenuWinGraphicSet(
		p_draw->p_bgl, GF_BGL_FRAME2_M, WFP2PMF_BG2_MENUWIN_CGX, WFP2PMF_BG2_MENUWIN_PAL, 0, heapID );

	// �r�b�g�}�b�v�쐬
	// ���b�Z�[�W�E�B���h�E
    GF_BGL_BmpWinAdd( p_draw->p_bgl , &p_draw->msgwin, GF_BGL_FRAME1_M,
			WFP2PMF_MSGWIN_X, WFP2PMF_MSGWIN_Y,
			WFP2PMF_MSGWIN_SIZX, WFP2PMF_MSGWIN_SIZY, 
			WFP2PMF_MSGWIN_PAL, WFP2PMF_MSGWIN_CGX );
    GF_BGL_BmpWinDataFill( &p_draw->msgwin, 15 );
    BmpTalkWinWrite(&p_draw->msgwin, WINDOW_TRANS_OFF,
		WFP2PMF_BG1_TALKWIN_CGX, WFP2PMF_BG1_TALKWIN_PAL );


	// �^�C�g���E�B���h�E
    GF_BGL_BmpWinAdd( p_draw->p_bgl , &p_draw->titlewin, GF_BGL_FRAME1_M,
			WFP2PMF_TITLEWIN_X, WFP2PMF_TITLEWIN_Y,
			WFP2PMF_TITLEWIN_SIZX, WFP2PMF_TITLEWIN_SIZY, 
			WFP2PMF_TITLEWIN_PAL, WFP2PMF_TITLEWIN_CGX );
    GF_BGL_BmpWinDataFill( &p_draw->titlewin, 15 );
	BmpMenuWinWrite( &p_draw->titlewin, WINDOW_TRANS_OFF, 
			WFP2PMF_BG1_MENUWIN_CGX, WFP2PMF_BG1_MENUWIN_PAL );


	// �{�C�X�`���b�g�E�B���h�E
    GF_BGL_BmpWinAdd( p_draw->p_bgl , &p_draw->vchatwin, GF_BGL_FRAME1_M,
			WFP2PMF_VCHATWIN_X, WFP2PMF_VCHATWIN_Y,
			WFP2PMF_VCHATWIN_SIZX, WFP2PMF_VCHATWIN_SIZY, 
			WFP2PMF_VCHATWIN_PAL, WFP2PMF_VCHATWIN_CGX );
    GF_BGL_BmpWinDataFill( &p_draw->vchatwin, 0 );

	// CONNECT���X�g
    GF_BGL_BmpWinAdd( p_draw->p_bgl , &p_draw->conlistwin, GF_BGL_FRAME1_M,
			WFP2PMF_CONLISTWIN_X, WFP2PMF_CONLISTWIN_Y,
			WFP2PMF_CONLISTWIN_SIZX, WFP2PMF_CONLISTWIN_SIZY, 
			WFP2PMF_CONLISTWIN_PAL, WFP2PMF_CONLISTWIN_CGX );
    GF_BGL_BmpWinDataFill( &p_draw->conlistwin, 15 );
	BmpMenuWinWrite( &p_draw->conlistwin, WINDOW_TRANS_OFF, 
			WFP2PMF_BG1_MENUWIN_CGX, WFP2PMF_BG1_MENUWIN_PAL );

	// NEWCON�E�B���h�E
    GF_BGL_BmpWinAdd( p_draw->p_bgl , &p_draw->newconwin, GF_BGL_FRAME1_M,
			WFP2PMF_NEWCONWIN_X, WFP2PMF_NEWCONWIN_Y,
			WFP2PMF_NEWCONWIN_SIZX, WFP2PMF_NEWCONWIN_SIZY, 
			WFP2PMF_NEWCONWIN_PAL, WFP2PMF_NEWCONWIN_CGX );
    GF_BGL_BmpWinDataFill( &p_draw->newconwin, 15 );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v�j��
 *
 *	@param	p_draw	���[�N 
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicBmpDelete( WFP2PMF_DRAW* p_draw, u32 heapID )
{
	// ���b�Z�[�W������
	WFP2PMF_GraphicMsgBmpStrOff( p_draw );
	
	// �r�b�g�}�b�v�̔j��
	// ���b�Z�[�W
	BmpTalkWinClear( &p_draw->msgwin, WINDOW_TRANS_ON );
	GF_BGL_BmpWinDel( &p_draw->msgwin );

	// �^�C�g��
	BmpMenuWinClear( &p_draw->titlewin, WINDOW_TRANS_ON );
	GF_BGL_BmpWinDel( &p_draw->titlewin );

	// �{�C�X�`���b�g
	GF_BGL_BmpWinDel( &p_draw->vchatwin );

	// CONNECT���X�g
	BmpMenuWinClear( &p_draw->conlistwin, WINDOW_TRANS_ON );
	GF_BGL_BmpWinDel( &p_draw->conlistwin );

	// �V�R�l�N�g���X�g
	BmpMenuWinClear( &p_draw->newconwin, WINDOW_TRANS_ON );
	GF_BGL_BmpWinDel( &p_draw->newconwin );
	
	// YesNo�̃��[�N����������j��
	if( p_draw->p_yesnowin != NULL ){
		BmpYesNoWinDel( p_draw->p_yesnowin, heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�f�[�^������
 *
 *	@param	p_draw		���[�N
 *	@param	cp_init		�������f�[�^
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicMsgInit( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 heapID )
{
    p_draw->p_wordset  = WORDSET_Create( heapID );
    p_draw->p_msgman = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wifi_lobby_dat, heapID );
	p_draw->p_msgstr = STRBUF_Create( WFP2PMF_MSGDATA_STRNUM, heapID );
	p_draw->p_msgtmp = STRBUF_Create( WFP2PMF_MSGDATA_STRNUM, heapID );
	p_draw->msg_no = WFP2PMF_MSGNO_NONE;
	p_draw->msg_speed = CONFIG_GetMsgPrintSpeed(SaveData_GetConfig(cp_init->p_savedata));
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�f�[�^�j��
 *
 *	@param	p_draw		���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicMsgDelete( WFP2PMF_DRAW* p_draw )
{
	STRBUF_Delete( p_draw->p_msgtmp );
	STRBUF_Delete( p_draw->p_msgstr );
    MSGMAN_Delete( p_draw->p_msgman );
    WORDSET_Delete( p_draw->p_wordset );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v���b�Z�[�W������
 *
 *	@param	p_draw		���[�N
 *	@param	cp_ini		�������f�[�^t
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicBmpMsgInit( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 heapID )
{
	// �^�C�g��
	switch( cp_init->type ){
	case WFP2PMF_TYPE_POFIN:				// �|�t�B��
		break;
		
	case WFP2PMF_TYPE_BUCKET:			// ���܂���
		WORDSET_RegisterWiFiLobbyGameName( p_draw->p_wordset, 0, WFLBY_GAME_BALLSLOW );
		break;
		
	case WFP2PMF_TYPE_BALANCE_BALL:		// ���܂̂�	
		WORDSET_RegisterWiFiLobbyGameName( p_draw->p_wordset, 0, WFLBY_GAME_BALANCEBALL );
		break;
		
	case WFP2PMF_TYPE_BALLOON:			// �ӂ�������
		WORDSET_RegisterWiFiLobbyGameName( p_draw->p_wordset, 0, WFLBY_GAME_BALLOON );
		break;
	}
    MSGMAN_GetString(  p_draw->p_msgman, WFP2PMF_TITLE[ cp_init->type ], p_draw->p_msgtmp );
    WORDSET_ExpandStr(  p_draw->p_wordset, p_draw->p_msgstr, p_draw->p_msgtmp );
	GF_STR_PrintColor( &p_draw->titlewin, FONT_SYSTEM, p_draw->p_msgstr, 0, 0, MSG_NO_PUT, _COL_N_BLACK, NULL);
	GF_BGL_BmpWinOnVReq( &p_draw->titlewin );
	
	// ���X�g
	GF_BGL_BmpWinOnVReq( &p_draw->conlistwin );
	
	
	// VCHT
    MSGMAN_GetString(  p_draw->p_msgman, msg_wifilobby_132, p_draw->p_msgstr );
    GF_STR_PrintColor( &p_draw->vchatwin, FONT_SYSTEM, p_draw->p_msgstr, 0, 0, MSG_NO_PUT, _COL_N_BLACK_C, NULL);
    GF_BGL_BmpWinOnVReq( &p_draw->vchatwin );
	
	// ���b�Z�[�W
    GF_BGL_BmpWinOnVReq( &p_draw->msgwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[������
 *	
 *	@param	p_draw		���[�N
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicClactInit( WFP2PMF_DRAW* p_draw, u32 heapID )
{
	int i;

    // OAM�}�l�[�W���[�̏�����
    NNS_G2dInitOamManagerModule();

    // ���LOAM�}�l�[�W���쐬
    // �����_���pOAM�}�l�[�W���쐬
    // �����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
    REND_OAMInit(
        0, 126,		// ���C�����OAM�Ǘ��̈�
        0, 31,		// ���C����ʃA�t�B���Ǘ��̈�
        0, 126,		// �T�u���OAM�Ǘ��̈�
        0, 31,		// �T�u��ʃA�t�B���Ǘ��̈�
        heapID);


    // �L�����N�^�}�l�[�W���[������
    {
        CHAR_MANAGER_MAKE cm = {
            WIFI_P2PMATCH_LOADRESNUM,
            WIFI_P2PMATCH_VRAMMAINSIZ,
            WIFI_P2PMATCH_VRAMSUBSIZ,
            0
        };
		cm.heap = heapID;
        InitCharManagerReg(&cm, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K );
    }
    // �p���b�g�}�l�[�W���[������
    InitPlttManager(WIFI_P2PMATCH_LOADRESNUM, heapID);

    // �ǂݍ��݊J�n�ʒu��������
    CharLoadStartAll();
    PlttLoadStartAll();

	//�ʐM�A�C�R���p�ɃL�������p���b�g����
	CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
	CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
	

	// �Z���A�N�^�[�Z�b�g�쐬
	p_draw->clactSet = CLACT_U_SetEasyInit( WIFI_P2PMATCH_CLACTWK_NUM, &p_draw->renddata, heapID );
	// �L�����ƃp���b�g�̃��\�[�X�}�l�[�W���쐬
	for( i=0; i<WIFI_P2PMATCH_RESNUM; i++ ){
		p_draw->resMan[i] = CLACT_U_ResManagerInit(WIFI_P2PMATCH_LOADRESNUM, i, heapID);
	}


	// �I�u�W�F�N�g�V�X�e��������
	p_draw->p_objsys = WF2DMAP_OBJSysInit( WIFI_P2PMATCH_CLACTWK_NUM, heapID );
	p_draw->p_objdraw = WF2DMAP_OBJDrawSysInit( p_draw->clactSet, NULL, WIFI_P2PMATCH_CLACTWK_NUM, NNS_G2D_VRAM_TYPE_2DMAIN, heapID );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�j��
 *
 *	@param	p_draw		���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicClactDelete( WFP2PMF_DRAW* p_draw )
{
	int i;

	// �I�u�W�F�N�g���[�N�j��
	WF2DMAP_OBJDrawSysExit( p_draw->p_objdraw );	
	WF2DMAP_OBJSysExit( p_draw->p_objsys );	

	// �A�N�^�[�̔j��
	CLACT_DestSet( p_draw->clactSet );

	for( i=0; i<WIFI_P2PMATCH_RESNUM; i++ ){
		CLACT_U_ResManagerDelete( p_draw->resMan[i] );
	}

    // ���\�[�X���
    DeleteCharManager();
    DeletePlttManager();

    //OAM�����_���[�j��
    REND_OAM_Delete();
}


//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�f�[�^�\��
 *
 *	@param	p_draw		���[�N
 *	@param	msg_idx		���b�Z�[�WIDX
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicMsgBmpStrPut( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 msg_idx, u32 heapID )
{
	WFP2PMF_GraphicMsgBmpStrPutEx( p_draw, cp_init, msg_idx, 0, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\��	���b�Z�[�W�I�����WAIT�ݒ�o�[�W����
 *
 *	@param	p_draw		���[�N
 *	@param	msg_idx		���b�Z�[�WIDX
 *	@param	wait		�҂�����
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicMsgBmpStrPutEx( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 msg_idx, u8 wait, u32 heapID )
{
	STRBUF* p_expand;
	int wintype;
	
	// �P�O�̕����I����Ă��Ȃ�������Ƃ߂�
    if( p_draw->msg_no != WFP2PMF_MSGNO_NONE){
        if(GF_MSG_PrintEndCheck( p_draw->msg_no )!=0){
			TOMOYA_PRINT( "msg stop\n" );
            GF_STR_PrintForceStop( p_draw->msg_no );
			p_draw->msg_no = WFP2PMF_MSGNO_NONE;
        }
    }


	
	// ��������
    GF_BGL_BmpWinDataFill( &p_draw->msgwin, 15 );
	p_expand = STRBUF_Create( WFP2PMF_MSGDATA_STRNUM, heapID );
	MSGMAN_GetString(  p_draw->p_msgman, msg_idx, p_expand );
	WORDSET_ExpandStr( p_draw->p_wordset, p_draw->p_msgstr, p_expand );
    p_draw->msg_no = GF_STR_PrintColor( &p_draw->msgwin, FONT_TALK, p_draw->p_msgstr, 0, 0, p_draw->msg_speed, _COL_N_BLACK, NULL);
    GF_BGL_BmpWinOnVReq( &p_draw->msgwin );
	STRBUF_Delete( p_expand );


	// �E�B���h�E�f�[�^�ē]��
	wintype = CONFIG_GetWindowType(SaveData_GetConfig(cp_init->p_savedata)); 
	TalkWinGraphicSet(
		p_draw->p_bgl, GF_BGL_FRAME1_M, WFP2PMF_BG1_TALKWIN_CGX, WFP2PMF_BG1_TALKWIN_PAL,  wintype, heapID );

	p_draw->msg_wait = wait;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W����C�ɕ\��
 *
 *	@param	p_draw		���[�N
 *	@param	msg_idx		���b�Z�[�WIDX
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicMsgBmpStrPutAll( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, u32 msg_idx, u32 heapID )
{
	int wintype;

	// �P�O�̕����I����Ă��Ȃ�������Ƃ߂�
    if( p_draw->msg_no != WFP2PMF_MSGNO_NONE ){
        if(GF_MSG_PrintEndCheck( p_draw->msg_no )!=0){
            GF_STR_PrintForceStop( p_draw->msg_no );
        }
		p_draw->msg_no = WFP2PMF_MSGNO_NONE;
    }
	
    GF_BGL_BmpWinDataFill( &p_draw->msgwin, 15 );
    MSGMAN_GetString(  p_draw->p_msgman, msg_idx, p_draw->p_msgstr );
    GF_STR_PrintColor( &p_draw->msgwin, FONT_TALK, p_draw->p_msgstr, 0, 0, MSG_NO_PUT, _COL_N_BLACK, NULL);
    GF_BGL_BmpWinOnVReq( &p_draw->msgwin );

	// �E�B���h�E�f�[�^�ē]��
	wintype = CONFIG_GetWindowType(SaveData_GetConfig(cp_init->p_savedata)); 
	TalkWinGraphicSet(
		p_draw->p_bgl, GF_BGL_FRAME1_M, WFP2PMF_BG1_TALKWIN_CGX, WFP2PMF_BG1_TALKWIN_PAL,  wintype, heapID );

	p_draw->msg_wait = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\�������`�F�b�N
 *
 *	@param	cp_draw		���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�܂�������ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_GraphicMsgBmpStrMain( WFP2PMF_DRAW* p_draw )
{
	if( p_draw->msg_no == WFP2PMF_MSGNO_NONE ){
		if( p_draw->msg_wait > 0 ){
			p_draw->msg_wait --;
			return FALSE;
		}
	}else{

		if( GF_MSG_PrintEndCheck( p_draw->msg_no ) == 0 ){
			if( p_draw->msg_no != WFP2PMF_MSGNO_NONE ){
				p_draw->msg_no = WFP2PMF_MSGNO_NONE;
			}
		}
	}
	
	if( (p_draw->msg_no == WFP2PMF_MSGNO_NONE) && (p_draw->msg_wait == 0) ){
		return TRUE;
	}

	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\��OFF����
 *
 *	@param	p_draw	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicMsgBmpStrOff( WFP2PMF_DRAW* p_draw )
{
	// �P�O�̕����I����Ă��Ȃ�������Ƃ߂�
    if( p_draw->msg_no != WFP2PMF_MSGNO_NONE ){
        if(GF_MSG_PrintEndCheck( p_draw->msg_no )!=0){
            GF_STR_PrintForceStop( p_draw->msg_no );
        }
		p_draw->msg_no = WFP2PMF_MSGNO_NONE;
    }
    GF_BGL_BmpWinDataFill( &p_draw->msgwin, 15 );
    GF_BGL_BmpWinOnVReq( &p_draw->msgwin );
	p_draw->msg_wait = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�K�R�l�N�g�v���C���[�̕\��
 *
 *	@param	p_draw		���[�N
 *	@param	cp_init		�������f�[�^
 *	@param	netid	�F�B�i���o�[
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicNewConStrPut( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, u32 heapID )
{
	STRBUF* p_expand;
	STRBUF* p_setstr;

	p_expand = STRBUF_Create( WFP2PMF_MSGDATA_STRNUM, heapID );
	p_setstr = STRBUF_Create( WFP2PMF_MSGDATA_STRNUM, heapID );
	
	WFP2PMF_WordSetPlayerNameSet( p_draw, cp_init, netid, 0, heapID );
	WFP2PMF_WordSetPlayerIDSet( p_draw, cp_init, netid, 1, heapID );
	MSGMAN_GetString(  p_draw->p_msgman, msg_wifilobby_135, p_expand );
	WORDSET_ExpandStr( p_draw->p_wordset, p_setstr, p_expand );

	// ��������
    GF_BGL_BmpWinDataFill( &p_draw->newconwin, 15 );
    GF_STR_PrintColor( &p_draw->newconwin, FONT_SYSTEM, p_setstr, 0, 0, MSG_NO_PUT, _COL_N_BLACK, NULL);
	BmpMenuWinWrite( &p_draw->newconwin, WINDOW_TRANS_OFF, 
			WFP2PMF_BG1_MENUWIN_CGX, WFP2PMF_BG1_MENUWIN_PAL );
    GF_BGL_BmpWinOnVReq( &p_draw->newconwin );
	
	STRBUF_Delete( p_expand );
	STRBUF_Delete( p_setstr );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�K�R�l�N�g�\��OFF
 *	
 *	@param	p_draw	�`�惏�[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicNewConStrOff( WFP2PMF_DRAW* p_draw )
{
	// �V�R�l�N�g���X�g
	BmpMenuWinClear( &p_draw->newconwin, WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( &p_draw->newconwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�̑S�`��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_init		�f�[�^
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicConlistStrPutAll( WFP2PMF_WK* p_wk, const WFP2PMF_INIT* cp_init, u32 heapID )
{
	int i;
	int connum = WIFI_P2PMATCH_NUM_MAX;


	// COMMINFO�@�������Â݃`�F�b�N
	if( CommInfoIsInitialize() == FALSE ){
		return ;
	}
	
	// �G���g���[�l�����\��
	for( i=0; i<connum; i++ ){
		
		if( (i == COMM_PARENT_ID) && (CommInfoIsNewName(i) == TRUE) ){	// �e�͊m����
			WFP2PMF_GraphicConlistStrPut( p_wk, &p_wk->draw, cp_init, i, heapID );
		}else{
			if( (CommInfoGetEntry(i) == TRUE) ){	// ���̑���ENTRY��ԂȂ�\��
				WFP2PMF_GraphicConlistStrPut( p_wk, &p_wk->draw, cp_init, i, heapID );
			}else{
				// ����
				WFP2PMF_GraphicConlistStrOff( &p_wk->draw, cp_init, i, heapID );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�l�N�g���X�g�@���b�Z�[�W�\��
 *
 *	@param	p_draw		���[�N
 *	@param	cp_init		�f�[�^
 *	@param	netid	�F�B�i���o�[
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicConlistStrPut( WFP2PMF_WK* p_wk, WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, u32 heapID )
{
	STRBUF* p_expand;
	STRBUF* p_setstr;
	u8 vchat;
	u8 friendno;

	p_expand = STRBUF_Create( WFP2PMF_MSGDATA_STRNUM, heapID );
	p_setstr = STRBUF_Create( WFP2PMF_MSGDATA_STRNUM, heapID );
	
	WFP2PMF_WordSetPlayerNameSet( p_draw, cp_init, netid, 0, heapID );
    MSGMAN_GetString(  p_draw->p_msgman, msg_wifilobby_133, p_expand );
    WORDSET_ExpandStr( p_draw->p_wordset, p_setstr, p_expand );

	// ��������
    GF_BGL_BmpWinFill( &p_draw->conlistwin, 15, 
			0, netid * (WFP2PMF_CONLISTWIN_ONELIST_Y*8), 
			(WFP2PMF_CONLISTWIN_SIZX*8), (WFP2PMF_CONLISTWIN_ONELIST_Y*8) );

	
    GF_STR_PrintColor( &p_draw->conlistwin, FONT_SYSTEM, p_setstr, 
			0, netid * (WFP2PMF_CONLISTWIN_ONELIST_Y*8),
			MSG_NO_PUT, _COL_N_BLACK, NULL);


	WFP2PMF_WordSetPlayerIDSet( p_draw, cp_init, netid, 0, heapID );
    MSGMAN_GetString(  p_draw->p_msgman, msg_wifilobby_134, p_expand );
    WORDSET_ExpandStr( p_draw->p_wordset, p_setstr, p_expand );

    GF_STR_PrintColor( &p_draw->conlistwin, FONT_SYSTEM, p_setstr, 
			WFP2PMF_CONLISTWIN_ID_X, netid * (WFP2PMF_CONLISTWIN_ONELIST_Y*8),
			MSG_NO_PUT, _COL_N_BLACK, NULL);
	
    GF_BGL_BmpWinOnVReq( &p_draw->conlistwin );

	// VCHAT�f�[�^�`��
	if( netid == CommGetCurrentID() ){
		if( p_wk->data.p_match->myMatchStatus.vchat ){
			vchat = WFP2PMF_VCHATICON_ON;
		}else{
			vchat = WFP2PMF_VCHATICON_OFF;
		}
		friendno = 0;
	}else{
		if( p_wk->data.vchat_now[netid] ){
			vchat = WFP2PMF_VCHATICON_ON;
		}else{
			vchat = WFP2PMF_VCHATICON_OFF;
		}
	}
	// �F�B�i���o���A���Ă��Ă��Ȃ��Ȃ�`�悵�Ȃ�
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, WFP2PMF_VCHAT_ICON_FRM_NUM,
			WFP2PMF_VCHAT_ICON_WX, 
			WFP2PMF_VCHAT_ICON_WY+(netid*WFP2PMF_CONLISTWIN_ONELIST_Y), 
			WFP2PMF_VCHAT_ICON_SIZ, WFP2PMF_VCHAT_ICON_SIZ,
			p_draw->p_vchatscrn->rawData, 
			vchat*WFP2PMF_VCHAT_ICON_SIZ, 0,
			p_draw->p_vchatscrn->screenWidth/8,
			p_draw->p_vchatscrn->screenHeight/8 );
	GF_BGL_ScrPalChange( p_draw->p_bgl, WFP2PMF_VCHAT_ICON_FRM_NUM,
			WFP2PMF_VCHAT_ICON_WX, 
			WFP2PMF_VCHAT_ICON_WY+(netid*WFP2PMF_CONLISTWIN_ONELIST_Y), 
			WFP2PMF_VCHAT_ICON_SIZ, WFP2PMF_VCHAT_ICON_SIZ, WFP2PMF_VCHAT_ICON_PAL );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, WFP2PMF_VCHAT_ICON_FRM_NUM );
	
	STRBUF_Delete( p_expand );
	STRBUF_Delete( p_setstr );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�l�N�g���X�g�\����\��
 *
 *	@param	p_draw		���[�N
 *	@param	cp_init		�f�[�^
 *	@param	netid	�F�B�i���o�[
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_GraphicConlistStrOff( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, u32 heapID )
{
	// ��������
    GF_BGL_BmpWinFill( &p_draw->conlistwin, 15, 
			0, netid * (WFP2PMF_CONLISTWIN_ONELIST_Y*8), 
			(WFP2PMF_CONLISTWIN_SIZX*8), (WFP2PMF_CONLISTWIN_ONELIST_Y*8) );

	GF_BGL_ScrWriteExpand( p_draw->p_bgl, WFP2PMF_VCHAT_ICON_FRM_NUM,
			WFP2PMF_VCHAT_ICON_WX, 
			WFP2PMF_VCHAT_ICON_WY+netid*WFP2PMF_CONLISTWIN_ONELIST_Y, 
			WFP2PMF_VCHAT_ICON_SIZ, WFP2PMF_VCHAT_ICON_SIZ,
			p_draw->p_vchatscrn->rawData, 
			WFP2PMF_VCHATICON_NONE*WFP2PMF_VCHAT_ICON_SIZ, 0,
			p_draw->p_vchatscrn->screenWidth/8,
			p_draw->p_vchatscrn->screenHeight/8 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, WFP2PMF_VCHAT_ICON_FRM_NUM );

    GF_BGL_BmpWinOnVReq( &p_draw->conlistwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�Ƀv���C���[����ݒ肷��
 *
 *	@param	p_draw		���[�N
 *	@param	cp_init		�f�[�^
 *	@param	netid	�F�B�ԍ�
 *	@param	set_no		���[�h�Z�b�g�ւ̐ݒ�ꏊ
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_WordSetPlayerNameSet( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, int set_no, u32 heapID )
{
	MYSTATUS* p_target;
	p_target = CommInfoGetMyStatus( netid );
	TOMOYA_PRINT( "player netid %d\n", netid );
	WORDSET_RegisterPlayerName( p_draw->p_wordset, set_no, p_target );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�Ƀv���C���[ID��ݒ肷��
 *
 *	@param	p_draw		���[�N
 *	@param	cp_init		�f�[�^
 *	@param	netid		NETID
 *	@param	set_no		���[�h�Z�b�g�ւ̐ݒ�ꏊ
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_WordSetPlayerIDSet( WFP2PMF_DRAW* p_draw, const WFP2PMF_INIT* cp_init, int netid, int set_no, u32 heapID )
{
	MYSTATUS* p_target;
	u16 id;
	p_target = CommInfoGetMyStatus( netid );
	id = MyStatus_GetID_Low( p_target );
	WORDSET_RegisterNumber( p_draw->p_wordset, set_no, id, 5, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_HANKAKU );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�A�j�����C��
 *
 *	@param	p_draw		���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_PlttAnm( WFP2PMF_DRAW* p_draw )
{
	if( p_draw->pltt_count <= 0 ){
		//  �]��
		WFP2PMF_PlttAnm_Trans( p_draw, p_draw->pltt_idx );

		p_draw->pltt_count	= WFP2PMF_BACK_PALANM;
		p_draw->pltt_idx	= (p_draw->pltt_idx+1) % WFP2PMF_BACK_PALNUM;
	}else{
		p_draw->pltt_count --;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�]��
 *
 *	@param	p_draw		�`�惏�[�N
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_PlttAnm_Trans( WFP2PMF_DRAW* p_draw, u32 idx )
{
	BOOL result;
	u16* p_data;
	u32 num;

	num = WFP2PMF_BACK_PAL_IDX[ idx ];

	p_data = p_draw->p_pltt->pRawData;
	result = AddVramTransferManager( NNS_GFD_DST_2D_BG_PLTT_MAIN, WFP2PMF_BACK_PAL*32, 
			&p_data[ num*16 ], 32 );
	GF_ASSERT( result );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�C�X�`���b�g�@�X�C�b�`��؂�ւ���
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�X�C�b�`�@ON
 *	@retval	FALSE	�X�C�b�`�@OFF
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_VchatSwitch( WFP2PMF_WK* p_wk, u32 heapID )
{
    p_wk->data.p_match->myMatchStatus.vchat_org = 1 - p_wk->data.p_match->myMatchStatus.vchat_org;
    p_wk->data.p_match->myMatchStatus.vchat = p_wk->data.p_match->myMatchStatus.vchat_org;
    mydwc_setVChat( p_wk->data.p_match->myMatchStatus.vchat );
    mydwc_setMyInfo( &(p_wk->data.p_match->myMatchStatus), sizeof(_WIFI_MACH_STATUS) );
	
	if( p_wk->data.p_match->myMatchStatus.vchat == TRUE ){

		WFP2PMF_StatusVChatOnBmpFade( p_wk );

	}else{
		WFP2PMF_StatusVChatOffBmpFade( p_wk );
	}
	
    return p_wk->data.p_match->myMatchStatus.vchat_org;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�@�G���[�`�F�b�N����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_ErrCheck( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_param, u32 heapID )
{
	BOOL result;
	BOOL connect_num_errror;
	BOOL timeout;
//	BOOL parent_status_check;
	
	// �G���[�ɂ����Ă����甲����
	if( p_wk->data.oya ){
		if( (p_wk->data.status == WFP2PMF_OYA_STATUS_ERR_INIT) ||
			(p_wk->data.status == WFP2PMF_OYA_STATUS_ERR_INIT01) ||
			(p_wk->data.status == WFP2PMF_OYA_STATUS_ERR) ){
			return ;
		}

		// �G���g���[�����P�ȉ��Ń}�b�`���Ă��Ȃ��Ƃ��͉������Ȃ��Ă悢
		// �������A�G���g���[�����Q�ȏ�ɂȂ����牽�炩�̃`�F�b�N���s���K�v������
		// �G���g���[���P�Ƃ����͎̂������G���g���[���Ă����ԂƂ������Ƃł�
		if( CommWifiIsMatched() <= 2 ){	// �}�b�`�X�e�[�g�I�ɂ��������Ȃ��Ă�����A�G���[�`�F�b�N����
			if( (WFP2PMF_GetEntryNum( p_wk ) <= 1) && (WFP2PMF_MatchCheck( p_wk ) == FALSE) ){
				return ;
			}
		}


		/*WIFIP2PModeCheck�������̂ł���Ȃ�	tomoya 
		// status�`�F�b�N�ؒf����
//		result = WFP2PMF_Oya_CheckConnectPlayer( p_wk, p_param );
		// status���s���ȂЂƂ�������I����
	
//		parent_status_check = TRUE;
//		//*/

	}else{
		if( (p_wk->data.status == WFP2PMF_KO_STATUS_ERR_INIT) ||
			(p_wk->data.status == WFP2PMF_KO_STATUS_ERR_INIT01) ||
			(p_wk->data.status == WFP2PMF_KO_STATUS_ERR) ||
			(p_wk->data.status == WFP2PMF_KO_STATUS_ENTRY_NG) ||
			(p_wk->data.status == WFP2PMF_KO_STATUS_ENTRY_NGINIT) ){
			return ;
		}

	}

	// �^�C���A�E�g�`�F�b�N
	timeout = WFP2PMF_TimeOut_Main( p_wk );

	// �ؒf�`�F�b�N
	result = WFP2PMF_LogOutCheck( p_wk );

	//  �G���g���[�����ʐM�l������������G���[��
	if( WFP2PMF_GetEntryNum( p_wk ) > CommGetConnectNum() ){
		connect_num_errror = TRUE;		
	}else{
		connect_num_errror = FALSE;		
	}
	
	// �G���[�`�F�b�N
/*
    if( (CommWifiIsMatched() == 3) || 
		(CommWifiIsMatched() > 4) ||		// �ؒf�`�F�b�N�͑��ł��
*/
	// 080704	tomoya takahashi
    if( (CommWifiIsMatched() >= 3) ||	// Disconnect���݂�
		(CommStateIsWifiError()) || 
		CommStateIsWifiDisconnect() || 
		(CommIsConnect(COMM_PARENT_ID) == FALSE) ||	// �e�Ɛؒf����
		(WFP2PMF_MatchCheck( p_wk ) == FALSE) ||	// �N���ƂȂ����Ă���̂ɁA�ʐM���Ă��Ȃ��Ƃ��͂���
		(connect_num_errror == TRUE) ||		// �G���g���[�����ʐM�l�����ւ���
		(timeout == TRUE) ||		// �ڑ��^�C���A�E�g
//		(parent_status_check == FALSE) ||	// �e�ƒʐM�X�e�[�^�X���ꏏ���`�F�b�N�i_WIFI_MACH_STATUS�j
		(result == TRUE) ){			// �ؒf

#ifdef WFP2P_DEBUG
		if( (CommWifiIsMatched() >= 3) ){
			TOMOYA_PRINT( "(CommWifiIsMatched() == %d)\n", CommWifiIsMatched() );
		}
		if( CommStateIsWifiError() ){
			TOMOYA_PRINT( "CommStateIsWifiError()\n" );
		}
		if( CommStateIsWifiDisconnect() ){
			TOMOYA_PRINT( "CommStateIsWifiDisconnect()\n" );
		}
		if( !CommIsConnect(COMM_PARENT_ID) ){
			TOMOYA_PRINT( "!CommIsConnect(COMM_PARENT_ID)\n" );
		}
		if( (WFP2PMF_MatchCheck( p_wk ) == FALSE) ){
			TOMOYA_PRINT( "(WFP2PMF_MatchCheck( p_wk ) == FALSE)\n" );
		}
		if( result ){
			TOMOYA_PRINT( "errcheck  logout\n" );
		}
		if( timeout ){
			TOMOYA_PRINT( "errcheck  timeout\n" );
		}
#endif

		// CommWifiIsMatched() == 4�̐ؒf�����͖�������
		if( p_wk->data.oya ){
			p_wk->data.status = WFP2PMF_OYA_STATUS_ERR_INIT;
		}else{
			p_wk->data.status = WFP2PMF_KO_STATUS_ERR_INIT;
		}
    }
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Đڑ����Ǝ��̃G���[�`�F�b�N
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_ReConErrCheck( WFP2PMF_WK* p_wk )
{
	// �G���[�`�F�b�N
#if 0
    if( (CommWifiIsMatched() == 3) || 
		(CommWifiIsMatched() > 4) ||		// �ؒf�`�F�b�N�͑��ł��
		(CommStateIsWifiError())  ){	// �ؒf
#endif
    if( (CommWifiIsMatched() > 3) || 
		CommStateIsWifiDisconnect() ||
		(CommStateIsWifiError())  ){	// �ؒf

#ifdef WFP2P_DEBUG
		if( (CommWifiIsMatched() >= 3) ){
			TOMOYA_PRINT( "(CommWifiIsMatched() == %d)\n", CommWifiIsMatched() );
		}
		if( CommStateIsWifiError() ){
			TOMOYA_PRINT( "CommStateIsWifiError()\n" );
		}
#endif

		p_wk->data.status = WFP2PMF_KO_STATUS_ERR_INIT;
    }
}

//----------------------------------------------------------------------------
/**
 *	@brief	�N���ƂȂ����Ă��邩�`�F�b�N
 *
 *	@param	cp_wk	���[�N 
 *
 *	@retval	TRUE	�Ȃ����Ă���
 *	@retval	FALSE	�Ȃ����Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_MatchCheck( const WFP2PMF_WK* cp_wk )
{
	if( CommGetConnectNum() > 0 ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�`���O�J�n
 *
 *	@param	friendno		�F�B�ԍ�
 *	@param	type			�}�b�`���O�^�C�v
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	���s
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_CommWiFiMatchStart( u32 friendno, u32 type )
{
	BOOL ret;

	ret = CommWifiBattleStart( friendno );
	if( ret == TRUE ){
		switch( type ){
		case WFP2PMF_TYPE_POFIN:				// �|�t�B��
			CommStateChangeWiFiPofin();
			break;
			
		case WFP2PMF_TYPE_BUCKET:			// ���܂���
		case WFP2PMF_TYPE_BALANCE_BALL:		// ���܂̂�	
		case WFP2PMF_TYPE_BALLOON:			// �ӂ�������
			CommStateChangeWiFiClub();	
			break;
		}
	}
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�̃X�e�[�^�X�Ǝ����̃X�e�[�^�X����v���邩�`�F�b�N����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	��v
 *	@retval	FALSE	��v���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_CheckP2PMatchFriendStatus( const WFP2PMF_WK* cp_wk, const WFP2PMF_INIT* cp_param, int friend )
{
	if( friend == WIFILIST_FRIEND_MAX ){
		return TRUE;
	}

	switch( cp_param->type ){
	case WFP2PMF_TYPE_POFIN:
		if( (cp_wk->data.p_match->friendMatchStatus[ friend ].status == WIFI_STATUS_POFIN) ||
			(cp_wk->data.p_match->friendMatchStatus[ friend ].status == WIFI_STATUS_POFIN_WAIT) ){
			return TRUE;
		}
		break;

	case WFP2PMF_TYPE_BUCKET:
		if( (cp_wk->data.p_match->friendMatchStatus[ friend ].status == WIFI_STATUS_BUCKET) ||
			(cp_wk->data.p_match->friendMatchStatus[ friend ].status == WIFI_STATUS_BUCKET_WAIT) ){
			return TRUE;
		}
		break;

	case WFP2PMF_TYPE_BALANCE_BALL:
		if( (cp_wk->data.p_match->friendMatchStatus[ friend ].status == WIFI_STATUS_BALANCEBALL) ||
			(cp_wk->data.p_match->friendMatchStatus[ friend ].status == WIFI_STATUS_BALANCEBALL_WAIT) ){
			return TRUE;
		}
		break;

	case WFP2PMF_TYPE_BALLOON:
		if( (cp_wk->data.p_match->friendMatchStatus[ friend ].status == WIFI_STATUS_BALLOON) ||
			(cp_wk->data.p_match->friendMatchStatus[ friend ].status == WIFI_STATUS_BALLOON_WAIT) ){
			return TRUE;
		}
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��Ԃ��A��W��Ԃ����s��ԂɕύX����
 *
 *	@param	p_wk		���[�N
 *	@param	cp_init		�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_StatusChange( WFP2PMF_WK* p_wk, const WFP2PMF_INIT* cp_init )
{
	switch( cp_init->type ){
	case WFP2PMF_TYPE_POFIN:
		p_wk->data.p_match->myMatchStatus.status = WIFI_STATUS_POFIN;
		break;

	case WFP2PMF_TYPE_BUCKET:
		p_wk->data.p_match->myMatchStatus.status = WIFI_STATUS_BUCKET;
		break;

	case WFP2PMF_TYPE_BALANCE_BALL:
		p_wk->data.p_match->myMatchStatus.status = WIFI_STATUS_BALANCEBALL;
		break;

	case WFP2PMF_TYPE_BALLOON:
		p_wk->data.p_match->myMatchStatus.status = WIFI_STATUS_BALLOON;
		break;
	}
	mydwc_setMyInfo( &(p_wk->data.p_match->myMatchStatus), sizeof(_WIFI_MACH_STATUS) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�C�X�`���b�g	ON
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_StatusVChatOn( WFP2PMF_WK* p_wk )
{
	if( p_wk->data.vchat_flag == TRUE ){
		// ���������Ă���
		return FALSE;
	}
	
	// �}�b�`���O���Ă�����{�C�X�`���b�g�𓮂���
	if( WFP2PMF_MatchCheck( p_wk ) == TRUE ){
		mydwc_startvchat( HEAPID_WIFI_FOURMATCHVCHAT );
		p_wk->data.vchat_flag = TRUE;

		TOMOYA_PRINT( "vcht on\n" );
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�C�X�`���b�gOFF
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_StatusVChatOff( WFP2PMF_WK* p_wk )
{
	if( p_wk->data.vchat_flag == FALSE ){
		// �����Ƃ܂��Ă�
		return FALSE;
	}

	mydwc_stopvchat();
	p_wk->data.vchat_flag = FALSE;

	TOMOYA_PRINT( "vcht off\n" );

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	VCHT�̐ݒ��BGM�̃t�F�[�h
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_StatusVChatOnBmpFade( WFP2PMF_WK* p_wk )
{
	if( WFP2PMF_StatusVChatOn( p_wk ) ){
		// BGM�̉��ʂ𗎂Ƃ�
		Snd_VChatVolSetBySeqNo( Snd_NowBgmNoGet() );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	VCHT�̐ݒ��BGM�̃t�F�[�h
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_StatusVChatOffBmpFade( WFP2PMF_WK* p_wk )
{
	if( WFP2PMF_StatusVChatOff( p_wk ) ){
		Snd_PlayerSetInitialVolume( SND_HANDLE_FIELD, BGM_WIFILOBBY_VOL );
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[�����擾
 */
//-----------------------------------------------------------------------------
static int WFP2PMF_GetEntryNum( WFP2PMF_WK* p_wk )
{
	if( p_wk->data.oya ){
		return CommInfoGetEntryNum();
	}else{
		return CommInfoGetEntryNum();
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�Ǝ�����菬����NETID�̐l��ENTRY��Ԃɂ���
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_SetEntryOya( WFP2PMF_WK* p_wk, u32 heapID )
{
	int i;
	int currentid;
	BOOL result = TRUE;


	// �e�Ǝ�����AID��菬����AID�̎q�̓G���g���[��Ԃɂ���
	currentid = CommGetCurrentID();
	currentid --;

//	TOMOYA_PRINT( "my id = %d\n", currentid + 1 );
	
	for( i=currentid; i>=COMM_PARENT_ID; i-- ){
		if( CommInfoIsBattleNewName( i ) == TRUE ){
			WFP2PMF_CommInfoSetEntry( p_wk, i, heapID );

			TOMOYA_PRINT( "entry id = %d\n", i );
			
		}else if( CommInfoGetEntry( i ) == FALSE ) {
			// NEWENTRY�ł��Ȃ�ENTRY�ł��Ȃ��Ƃ��́A
			// �܂��󂯎���Ă��Ȃ��l�̃f�[�^������
			result = FALSE;
		}
	}
	
	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	NEW�G���g���[�̐l�ɒʐMNG�𑗐M
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_CommSendNewEntryNg( WFP2PMF_WK* p_wk )
{
	int i;
	WFP2PMF_COMM_RESULT result;
	BOOL send_result;

	result.flag = WFP2PMF_CON_NG;

	for( i= 1; i<WIFI_P2PMATCH_NUM_MAX; i++ ){
		if( CommInfoIsBattleNewName( i ) == TRUE ){
			result.netID  = i;
			send_result = CommSendData( CNM_WFP2PMF_RESULT, &result, sizeof(WFP2PMF_COMM_RESULT) );
			GF_ASSERT( send_result == TRUE );
			CommInfoDeletePlayer( i );	// ���Ȃ����Ƃɂ���
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ȃ����Ă��邷�ׂẴv���C���[�ɐؒf��ʒm
 *
 *	@param	p_wk	���[�N
 */	
//-----------------------------------------------------------------------------
static void WFP2PMF_CommSendAllEntryNg( WFP2PMF_WK* p_wk )
{
	int i;
	WFP2PMF_COMM_RESULT result;
	BOOL send_result;

	result.flag = WFP2PMF_CON_NG;

	for( i= 1; i<WIFI_P2PMATCH_NUM_MAX; i++ ){
		if( CommInfoIsNewName( i ) == TRUE ){
			result.netID  = i;
			send_result = CommSendData( CNM_WFP2PMF_RESULT, &result, sizeof(WFP2PMF_COMM_RESULT) );
			GF_ASSERT( send_result == TRUE );
			CommInfoDeletePlayer( i );	// ���Ȃ����Ƃɂ���
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	LOG Out�`�F�b�N�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_LogOutCheckStart( WFP2PMF_WK* p_wk )
{
	mydwc_setDisconnectCallback( WFP2PMF_LogOutCallBack, p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	LOG out�@�`�F�b�N�I��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_LogOutCheckEnd( WFP2PMF_WK* p_wk )
{
	mydwc_setDisconnectCallback( NULL, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	LOGOUT�`�F�b�N
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	LOG�A�E�g���m		ERR������
 *	@retval	FALSE	LOG�A�E�g�Ȃ��@�܂��͊֌W�Ȃ��l��LOG�A�E�g����
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_LogOutCheck( WFP2PMF_WK* p_wk )
{
	int i;
	BOOL result = FALSE;
	
	for( i=0;i<WIFI_P2PMATCH_NUM_MAX; i++ ){
		
		if( p_wk->data.logout_in[i] == TRUE ){

			// ���̐l��ENTER���傤�������`�F�b�N
			if( CommInfoGetEntry( p_wk->data.logout_id[i] ) == TRUE ){
				result = TRUE;
			}

			// ���[�N������
			p_wk->data.logout_in[i] = FALSE;
			p_wk->data.logout_id[i] = 0;
		}
	}

	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	LOGOUT�R�[���o�b�N	�ؒf���ɌĂ΂��
 *
 *	@param	aid			�ؒf����AID
 *	@param	pWork		���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_LogOutCallBack( u16 aid, void* pWork )
{
	WFP2PMF_WK* p_wk = pWork;
	
	p_wk->data.logout_in[aid] = TRUE;
	p_wk->data.logout_id[aid] = aid;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�l�N�g���X�g���ĕ`�悷��K�v�����邩�`�F�b�N
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�ĕ`��
 *	@retval FALSE	�ĕ`��̕K�v�Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_ConListWriteCheck( WFP2PMF_WK* p_wk )
{
	int i;
	BOOL result = FALSE;
	
	// �N����VCHAT�t���O���ς���Ă���
	for( i=0; i<WIFI_P2PMATCH_NUM_MAX; i++ ){
		if( p_wk->data.vchat_tmp[i] != p_wk->data.vchat_now[i] ){
			result = TRUE;
		}
		p_wk->data.vchat_tmp[i] = p_wk->data.vchat_now[i];
	}
	
	// �ĕ`��t���O�������Ă���
	if( p_wk->data.conlist_rewrite ){
		p_wk->data.conlist_rewrite = FALSE;
		result = TRUE;
	}

	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�l�N�g���X�g�ĕ`��t���O�̐ݒ�
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_ConListWriteReq( WFP2PMF_WK* p_wk )
{
	p_wk->data.conlist_rewrite = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	COMMINFO�̃��[�N�ɃG���g���[��ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	aid		AID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_CommInfoSetEntry( WFP2PMF_WK* p_wk, u32 aid, u32 heapID )
{
	// ENTRY����Ȃ�������ENTRY
	if( CommInfoGetEntry( aid ) == FALSE ){
		CommInfoSetEntry( aid );

		// �O��NETID���i�[
		p_wk->data.last_netid = aid;
	
		// OAM�`��J�n
		WFP2PMF_DrawOamInit( p_wk, aid, heapID );

		// �R�l�N�g���X�g�ĕ`��
		WFP2PMF_ConListWriteReq( p_wk );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM�`��J�n
 *
 *	@param	p_wk		���[�N
 *	@param	aid			AID
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_DrawOamInit( WFP2PMF_WK* p_wk, u32 aid, u32 heapID )
{
	WF2DMAP_OBJDATA objdata;
	MYSTATUS* p_status;

	// ��������Ă��邩�牽�����Ȃ�
	if( p_wk->draw.objdata[aid].p_objwk != NULL ){
		return ;
	}
 
	// �������f�[�^�R�s�[
	objdata = WFP2PMF_OBJData[ aid ];
	if( aid != CommGetCurrentID() ){
		p_status = CommInfoGetMyStatus( aid );
		GF_ASSERT( p_status != NULL );
		objdata.charaid = MyStatus_GetTrainerView( p_status );
	}else{
		if( p_wk->data.p_match->myMatchStatus.sex == PM_MALE ){
			objdata.charaid = HERO;
		}else{
			objdata.charaid = HEROINE;
		}
	}
	
	// �I�u�W�F�N�g������
	p_wk->draw.objdata[aid].p_objwk = WF2DMAP_OBJWkNew( p_wk->draw.p_objsys, &objdata );


	// �`�惊�\�[�X�̓Ǎ���
	if( WF2DMAP_OBJDrawSysResCheck( p_wk->draw.p_objdraw, objdata.charaid ) == FALSE ){
		WF2DMAP_OBJDrawSysResSet( p_wk->draw.p_objdraw, objdata.charaid, 
				WF_2DC_MOVETURN, heapID );
	}


	// �`�惏�[�N�쐬
	p_wk->draw.objdata[aid].p_objdrawwk = WF2DMAP_OBJDrawWkNew( 
			p_wk->draw.p_objdraw, p_wk->draw.objdata[aid].p_objwk,
			FALSE, heapID );

	// �ŏ��͔�\��
	WF2DMAP_OBJDrawWkDrawFlagSet( p_wk->draw.objdata[aid].p_objdrawwk, FALSE );

	// ����J�n
	p_wk->draw.objdata[aid].seq++;
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM�`�惁�C������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_DrawOamMain( WFP2PMF_WK* p_wk )
{
	int i;

	// �`��I�u�W�F�N�g����
	for( i=0; i<WIFI_P2PMATCH_NUM_MAX; i++ ){
		pDrawObjMoveFunc[p_wk->draw.objdata[i].seq]( p_wk, &p_wk->draw.objdata[i] );
	}

	// �R�}���h�I�u�W�F�N�g����
	WF2DMAP_OBJSysMain( p_wk->draw.p_objsys );
	
	// �`��X�V
	WF2DMAP_OBJDrawSysUpdata( p_wk->draw.p_objdraw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM�`��j������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_DrawOamDelete( WFP2PMF_WK* p_wk )
{
	// �S���\�[�X�j��
	WF2DMAP_OBJDrawSysAllResDel( p_wk->draw.p_objdraw );

	// �`��I�u�W�F�f�[�^�j��
	memset( p_wk->draw.objdata, 0, sizeof( WFP2PMF_DRAWOBJ )*WIFI_P2PMATCH_CLACTWK_NUM );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�ɒʐMLOCK�𑗐M
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_NewConLockSend( WFP2PMF_WK* p_wk, u32 aid )
{
	WFP2PMF_COMM_RESULT result;
	BOOL send_result;

	result.netID = aid;
	result.flag = WFP2PMF_CON_LOCK;
	send_result = CommSendData( CNM_WFP2PMF_RESULT, &result, sizeof(WFP2PMF_COMM_RESULT) );
	GF_ASSERT( send_result == TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	last_netID��ENTRY�ɂȂ�O�ɂ�����傫��NETID�̐l��NEWENTRY��
 *          �Ȃ�����Đڑ����Ă��炤
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_NewConLockNewCon( WFP2PMF_WK* p_wk )
{
	int i;
	
	for( i=0; i<WIFI_P2PMATCH_NUM_MAX; i++ ){
		if( p_wk->data.last_netid+1 < i ){	// �Ō��ENTRY����NETID���傫��

			if( CommInfoIsBattleNewName( i ) == TRUE ){
				// �ؒf�肢�𑗐M
				WFP2PMF_NewConLockSend( p_wk, i );
				// ���[�U�[�f�[�^�j��
				CommInfoDeletePlayer( i );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�̎�M�����l��ENTRY����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_KoRecvEntry( WFP2PMF_WK* p_wk, u32 heapID )
{
	int i;

	for( i=0; i<WIFI_P2PMATCH_NUM_MAX; i++ ){
		if( p_wk->data.entry_on[ i ] == TRUE ){

			if( CommInfoIsNewName( i ) == TRUE ){
				WFP2PMF_CommInfoSetEntry( p_wk, i, heapID );

				p_wk->data.entry_on[ i ] = FALSE;

				// �V�K�R�l�N�g��������
				p_wk->data.ko_newcon_flag = TRUE;
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	TimeWaitIcon�\��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_TimeWaitIconOn( WFP2PMF_WK* p_wk )
{
	GF_ASSERT( p_wk->draw.p_timewaiticon == NULL );
	p_wk->draw.p_timewaiticon = TimeWaitIconAdd( &p_wk->draw.msgwin, WFP2PMF_BG1_TALKWIN_CGX );
}

//----------------------------------------------------------------------------
/**
 *	@brief	TimeWaitIcon�@��\��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_TimeWaitIconOff( WFP2PMF_WK* p_wk, const WFP2PMF_INIT* cp_init, u32 heapID )
{
	int wintype;
	
	if( p_wk->draw.p_timewaiticon == NULL ){
		return ;
	}

	TimeWaitIconDel(p_wk->draw.p_timewaiticon);  // �^�C�}�[�~��
	p_wk->draw.p_timewaiticon = NULL;
	
	// �E�B���h�E�f�[�^�ē]��
	wintype = CONFIG_GetWindowType(SaveData_GetConfig(cp_init->p_savedata)); 
	TalkWinGraphicSet(
		p_wk->draw.p_bgl, GF_BGL_FRAME1_M, WFP2PMF_BG1_TALKWIN_CGX, WFP2PMF_BG1_TALKWIN_PAL,  wintype, heapID );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F	����Ȃ�
 *	
 *	@param	p_wk		�V�X�e�����[�N
 *	@param	p_obj		�I�u�W�F�N�g�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_DrawObjMoveNone( WFP2PMF_WK* p_wk, WFP2PMF_DRAWOBJ* p_obj )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g�@���쏉����
 *
 *	@param	p_wk		�V�X�e�����[�N
 *	@param	p_obj		�I�u�W�F�N�g�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_DrawObjMoveInit( WFP2PMF_WK* p_wk, WFP2PMF_DRAWOBJ* p_obj )
{
	u32 way;
	
	// �`��X�V��~
	WF2DMAP_OBJDrawWkUpdataFlagSet( p_obj->p_objdrawwk, FALSE );

	// ���邭��A�j���ݒ�
	WF2DMAP_OBJDrawWkKuruAnimeStart( p_obj->p_objdrawwk );

	// �`��J�n
	WF2DMAP_OBJDrawWkDrawFlagSet( p_obj->p_objdrawwk, TRUE );

	// ���o�����
	p_obj->seq = WFP2PMF_DRAWOBJ_MOVEIN;

	// ���o����G�t�F�N�g�^�C��
	way = WF2DMAP_OBJWkDataGet( p_obj->p_objwk, WF2DMAP_OBJPM_WAY );
	p_obj->count = WFP2PMF_DRAWOBJ_INTIME + WFP2PMF_DRAWOBJ_INTIME_DIV[way];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g	����
 *
 *	@param	p_wk		�V�X�e�����[�N
 *	@param	p_obj		�I�u�W�F�N�g�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_DrawObjMoveIn( WFP2PMF_WK* p_wk, WFP2PMF_DRAWOBJ* p_obj )
{
	// ���邭��A�j�����C��
	WF2DMAP_OBJDrawWkKuruAnimeMain( p_obj->p_objdrawwk );

	// �^�C���v�Z
	p_obj->count --;
	if( p_obj->count <= 0 ){
		// ���o�G�t�F�N�g�I��
		WF2DMAP_OBJDrawWkKuruAnimeEnd( p_obj->p_objdrawwk );

		// �`��A�b�v�f�[�g�J�n
		WF2DMAP_OBJDrawWkUpdataFlagSet( p_obj->p_objdrawwk, TRUE );

		// �ʏ�`���
		p_obj->seq = WFP2PMF_DRAWOBJ_MOVENORMAL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g	�ʏ펞
 *
 *	@param	p_wk		�V�X�e�����[�N
 *	@param	p_obj		�I�u�W�F�N�g�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_DrawObjMoveNormal( WFP2PMF_WK* p_wk, WFP2PMF_DRAWOBJ* p_obj )
{
	// ���ɉ������Ȃ�
}

//----------------------------------------------------------------------------
/**
 *	@brief	VChat�t���O�𑗐M
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_VchatCommSend( WFP2PMF_WK* p_wk )
{
	int i;
	int roop_num;
	BOOL result = FALSE;
	int friendno;
	int vchat;
	WFP2PMF_COMM_VCHAT comm_vchat;

	// Info���Ȃ��Ȃ�����Ă΂Ȃ�
	if( CommInfoIsInitialize() == FALSE ){
		
		return ;
	}

	// �G���g���[�ς݂̐l�̐����擾
	roop_num = CommInfoGetEntryNum();
	
	// �V�����l�������Ă�����A���̃{�C�X�`���b�g��Ԃ͑��M����
    if(CommInfoIsNewInfoData()){
		if(!CommIsSendCommand_ServerSize(CS_COMM_INFO_END)){
			result = TRUE;
		}
    }
	
	// �N����VCHAT�t���O���ς���Ă���
	for( i=0; i<roop_num; i++ ){
		if( i==COMM_PARENT_ID ){
			vchat = p_wk->data.p_match->myMatchStatus.vchat;
		}else{
			friendno = CommInfoSearchWifiListIndex( i );
			GF_ASSERT( friendno != WIFILIST_FRIEND_MAX );
			vchat = p_wk->data.p_match->friendMatchStatus[ friendno ].vchat;
		}
		if( p_wk->data.vchat_now[i] != vchat ){
			result = TRUE;
		}
		p_wk->data.vchat_now[i] =vchat;
	}

	// �t���O���ς���Ă����瑗�M����
	if( result == TRUE ){
		BOOL send_result;
		memcpy( comm_vchat.vchat_now, p_wk->data.vchat_now, sizeof(u8)*WIFI_P2PMATCH_NUM_MAX );
		CommSendData( CNM_WFP2PMF_VCHAT, &comm_vchat, sizeof(WFP2PMF_COMM_VCHAT) );
		TOMOYA_PRINT( "VCHATFLAG send\n" );
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�`���O���������ă~�j�Q�[���̊J�n�֐i�ނ��`�F�b�N
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�}�b�`���O�����[���~�j�Q�[���J�n��
 *	@retval	FALSE	�܂���W��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_MatchOkCheck( WFP2PMF_WK* p_wk )
{
	if(CommIsTimingSync(_TIMING_GAME_CHECK)){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�E�P���m�[�g	�f�[�^�ݒ�
 *
 *	@param	p_param 
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_FNOTE_Set( WFP2PMF_INIT* p_param, u32 heapID )
{
	void* p_buf;
	FNOTE_DATA* p_fnote;
	_WIFI_MACH_STATUS* p_status;

	p_fnote		= SaveData_GetFNote( p_param->p_savedata );
	p_buf = FNOTE_SioIDOnlyDataMake( heapID, FNOTE_ID_PL_WIFICLUB );
	FNOTE_DataSave( p_fnote, p_buf, FNOTE_TYPE_SIO );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�ڑ����Ă����v���C���[�̏�Ԃ��`�F�b�N����
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�s��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_Oya_CheckConnectPlayer( const WFP2PMF_WK* cp_wk, const WFP2PMF_INIT* cp_param )
{
	int i;
	int friend;
	BOOL ret;
	BOOL result;

	ret = TRUE;
	
	// �ʐM�ڑ������l�S����STATUS���`�F�b�N���A
	// �s���Ȑl��������A�ؒf����B
	for( i=0; i<WIFI_P2PMATCH_NUM_MAX; i++ ){
		if( i != CommGetCurrentID() ){
			if( CommIsConnect(i) == TRUE ){

				// STATUS�`�F�b�N
				friend = CommInfoSearchWifiListIndex( i );
				if( friend != WIFILIST_FRIEND_MAX ){
					result = WFP2PMF_CheckP2PMatchFriendStatus( cp_wk, cp_param, friend );
				}else{

					// �������������ɂ͂��Ȃ��͂������A
					// ���̐l��ؒf
					result = FALSE;
				}

				// �ؒf����
				if( result == FALSE ){
					ret = FALSE;

					// ���̐l��ؒf
					DWC_CloseConnectionHard(i);
				}
				
			}
		}
	}

	return ret;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���A�E�g	�X�^�[�g
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_TimeOut_Start( WFP2PMF_WK* p_wk )
{
	p_wk->data.timeout_flag = TRUE;
	p_wk->data.timeout		= CONNECT_TIMEOUT;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���A�E�g	�X�g�b�v
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFP2PMF_TimeOut_Stop( WFP2PMF_WK* p_wk )
{
	p_wk->data.timeout_flag = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���A�E�g	���C��
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�G���[���
 *	@retval	FALSE	�ʏ�
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_TimeOut_Main( WFP2PMF_WK* p_wk )
{
	// �^�C���E�G�C�g�`�F�b�N�����H
	if( p_wk->data.timeout_flag == FALSE ){
		return FALSE;
	}
	
	if( p_wk->data.timeout > 0 ){
		p_wk->data.timeout--;
		return FALSE;
	}
	return TRUE;
}




//-----------------------------------------------------------------------------
/**
 *		�e�@��ԁ@�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�e		������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// VCHATONOFF
	if( p_wk->data.p_match->myMatchStatus.vchat == TRUE ){

		WFP2PMF_StatusVChatOnBmpFade( p_wk );
	}

	// ��W���́AClosedCallback�����Ă��A�ؒf�����ɑJ�ڂ��Ȃ��悤�Ƀ��b�N����
	mydwc_SetClosedDisconnectFlag( FALSE );

	CommStateSetErrorCheck(FALSE,TRUE);	// �ؒf�̓G���[����Ȃ�

	// OAM�`��J�n
	WFP2PMF_DrawOamInit( p_wk, CommGetCurrentID(), heapID );

	p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e		�R�l�N�g�҂���ԏ�����
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaConnectWaitInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// ���b�Z�[�W
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_107, heapID );

	p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e		�R�l�N�g�҂����
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaConnectWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;


	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}
	
	// �V�K�R�l�N�g�҂�
	// �ʐM�����`�F�b�N
	if( WFP2PMF_MatchCheck( p_wk ) == TRUE ){
		int new_con_netid;
		WFP2PMF_COMM_RESULT result;
		BOOL send_result;


		new_con_netid = CommInfoGetNewNameID();

		if( new_con_netid != INVALID_NETID ){

			// ������������Ȃɂ����Ȃ�
			if( p_wk->data.new_con != COMM_PARENT_ID ){
				// ���̐l��m���Ă��邩�`�F�b�N
				if( CommInfoSearchWifiListIndex( new_con_netid ) == WIFILIST_FRIEND_MAX ){
					// �m��Ȃ��̂ŁA���ۂ���
					result.netID	= new_con_netid;
					result.flag		= WFP2PMF_CON_NG;		

					// �ؒf�𖽗߂���
					CommInfoDeletePlayer( new_con_netid );

					// �q�@�ɒʐM�ؒf��ʒm
					send_result = CommSendData( CNM_WFP2PMF_RESULT, &result, sizeof(WFP2PMF_COMM_RESULT) );
					GF_ASSERT( send_result == TRUE );

				}else{
					p_wk->data.new_con = new_con_netid;
				}
			}
		}
	}

	do{
		// �i�ރ{�^����������
		if( sys.trg & PAD_BUTTON_DECIDE ){
			Snd_SePlay( _SE_DESIDE );

			// �ʐM�����`�F�b�N
			if( WFP2PMF_MatchCheck( p_wk ) == FALSE ){
				// �N�Ƃ��}�b�`���O���Ă��Ȃ�
				p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_MIN_INIT;
				break;
			}else{
			
				// �G���g���[��������Ă��邩�`�F�b�N
				if( p_init->comm_min <= WFP2PMF_GetEntryNum( p_wk ) ){
					p_wk->data.status = WFP2PMF_OYA_STATUS_START_INIT00;
					break;
				}else{
					// ����Ă��Ȃ��̂ŁA�V�Ԃ̂���߂�̂������ɂ���
					p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_MIN_INIT;
					break;
				}
			}
		}
		// �L�����Z�������`�F�b�N
		if( sys.trg & PAD_BUTTON_CANCEL ){
			Snd_SePlay( _SE_DESIDE );
//			p_wk->data.status = WFP2PMF_OYA_STATUS_END_INIT00;	// �I���`�F�b�N���Q����s��
			p_wk->data.status = WFP2PMF_OYA_STATUS_END2_INIT00;	// �I���`�F�b�N���Q����s��
			break;
		}
		
		// �V�R�l�N�g�������I
		if( p_wk->data.new_con != INVALID_NETID ){

			Snd_SePlay( _SE_DESIDE );

			p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_SELECT_INIT00;
			break;
		}

		// �{�C�X�`���b�g�`�F�b�N
		if( sys.trg & PAD_BUTTON_X ){
			p_wk->data.status = WFP2PMF_OYA_STATUS_VCHAT_INIT00;
			Snd_SePlay( _SE_DESIDE );
			break;
		}

	}while(0);
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e			�V�R�l�N�g�I���@������00
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaConnectSelectInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{

	// ���̐l�̏��\��
	WFP2PMF_GraphicNewConStrPut( &p_wk->draw, p_init, p_wk->data.new_con, heapID );

	// ���b�Z�[�W�̕\��
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_108, heapID );

	p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_SELECT_INIT01;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e			�V�R�l�N�g�I���@������01
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaConnectSelectInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;
	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}


	// YESNO�E�B���h�E���o��
	p_wk->draw.p_yesnowin = BmpYesNoSelectInit( p_wk->draw.p_bgl,
							&WFP2PMF_YESNOBMPDAT,
							WFP2PMF_BG2_MENUWIN_CGX, WFP2PMF_BG2_MENUWIN_PAL,
							heapID );
	p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_SELECT;
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�V�R�l�N�g�I��
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaConnectSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int yesno_ret;
	WFP2PMF_COMM_RESULT result;
	BOOL send_result;
	
	// YES�Ȃ�R�l�N�g�@No�Ȃ�ؒf
	yesno_ret = BmpYesNoSelectMain( p_wk->draw.p_yesnowin, heapID );

	
	if( yesno_ret != BMPMENU_NULL ){
		result.netID = p_wk->data.new_con;
	
		if( yesno_ret == 0 ){	// �͂���I��
			result.flag = WFP2PMF_CON_OK;		

			// �G���g���[
			WFP2PMF_CommInfoSetEntry( p_wk, p_wk->data.new_con, heapID );

			
			// Vchat���Ȃ�
			if( p_wk->data.p_match->myMatchStatus.vchat == TRUE ){
				// �{�C�X�`���b�gOFF
				WFP2PMF_StatusVChatOff( p_wk );
			
				// �{�C�X�`���b�gON
				WFP2PMF_StatusVChatOnBmpFade( p_wk );
			}


			// �l����MAX�ɂȂ�����Q�[���J�n��
			if( p_init->comm_max <= WFP2PMF_GetEntryNum( p_wk ) ){
				p_wk->data.status = WFP2PMF_OYA_STATUS_START_INIT00;
			}else{
				// MAX�ɂȂ��Ă��Ȃ��Ȃ�҂���Ԃɖ߂�
				p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT;
			}

		}else{

			result.flag = WFP2PMF_CON_NG;		
			// �L�����Z���Ȃ�ؒf
			// �ؒf�𖽗߂���
			CommInfoDeletePlayer( p_wk->data.new_con );

			// �҂���Ԃɖ߂�
			p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT;
		}

		// �q�@�ɒʐMOK��ʒm
		send_result = CommSendData( CNM_WFP2PMF_RESULT, &result, sizeof(WFP2PMF_COMM_RESULT) );
		GF_ASSERT( send_result == TRUE );

		// �V�K�R�l�N�g�I��
		p_wk->data.new_con = INVALID_NETID;
		WFP2PMF_GraphicNewConStrOff( &p_wk->draw );
		p_wk->draw.p_yesnowin = NULL;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�J�n���������ǁA�l��������Ȃ������Ƃ�	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaConnectMinInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// ���b�Z�[�W�\��
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_122, heapID );

	p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_MIN;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�J�n���������ǁA�l��������Ȃ������Ƃ�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaConnectMin( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// ��߂邩�����V�[�P���X��
	p_wk->data.status = WFP2PMF_OYA_STATUS_END_INIT00;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�J�n	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaStartInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// ���b�Z�[�W�\��	���̃����o�[�ł�낵���ł����H
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_111, heapID );

	p_wk->data.status = WFP2PMF_OYA_STATUS_START_INIT01;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�J�n	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaStartInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// YESNO�E�B���h�E���o��
	p_wk->draw.p_yesnowin = BmpYesNoSelectInit( p_wk->draw.p_bgl,
							&WFP2PMF_YESNOBMPDAT,
							WFP2PMF_BG2_MENUWIN_CGX, WFP2PMF_BG2_MENUWIN_PAL,
							heapID );
	p_wk->data.status = WFP2PMF_OYA_STATUS_START_SELECT;
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�J�n	�I��
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaStartSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int yesno_ret;
	
	// YES�Ȃ�R�l�N�g�@No�Ȃ�ؒf
	yesno_ret = BmpYesNoSelectMain( p_wk->draw.p_yesnowin, heapID );
	if( yesno_ret != BMPMENU_NULL ){
		if( yesno_ret == 0 ){	// �͂���I��

			p_wk->data.status = WFP2PMF_OYA_STATUS_START;
		}else{

			// �l����MAX�ɂȂ�����Q�[���I����
			if( p_init->comm_max <= WFP2PMF_GetEntryNum( p_wk ) ){
				p_wk->data.status = WFP2PMF_OYA_STATUS_END_INIT00;
			}else{
				p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT;
			}
		}

		// �I��
		p_wk->draw.p_yesnowin = NULL;
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@���ۂɊJ�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaStart( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// �����J�n�^�C�}�[�Z�b�g
	p_wk->data.timer = 30;

	// ENTRY�ȊO�̐l��ؒf����
	WFP2PMF_CommSendNewEntryNg( p_wk );

	// �ʐM�ҋ@�����b�Z�[�W�\��
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_066, heapID );

	// �҂��A�C�R���\��
	WFP2PMF_TimeWaitIconOn( p_wk );

	// �^�C���E�G�C�g�J�n
	WFP2PMF_TimeOut_Start( p_wk );

	// �l���`�F�b�N��
	p_wk->data.status = WFP2PMF_OYA_STATUS_STARTNUMCHECK;

	// �N���C�A���g���b�N���ꉞ����
	mydwc_ResetClientBlock();
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�J�n�O�Ɂ@�ʐM�l���`�F�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaStartNumCheck( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int result;
	
	
	// ENTRY�l���ƒʐM���̐l�����ꏏ�ɂȂ�����I���
	if( CommGetConnectNum() != CommInfoGetEntryNum() ){
		TOMOYA_PRINT( "connectnum notmatch\n" );
		// ENTRY�ȊO�̐l��ؒf����
		WFP2PMF_CommSendNewEntryNg( p_wk );
		return FALSE;
	}

	// �V�K�R�l�N�g���b�N
	result = mydwc_SetClientBlock();
	if( result == FALSE ){
		TOMOYA_PRINT( "mydwc_SetClientBlock false  %d  \n", result );
		return FALSE;
	}

	// ClosedCallback��������ؒf�����֑J�ڂ���
	mydwc_SetClosedDisconnectFlag( TRUE );
	// �����J�n��
	p_wk->data.status = WFP2PMF_OYA_STATUS_STARTSYNC_INIT;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�����J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaStartSyncInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;	

	if(p_wk->data.timer == 0){
		result = CommSendData( CNM_WFP2PMF_START, NULL, 0 );	// �Q�[���J�n
		if( result ){
			p_wk->data.status = WFP2PMF_OYA_STATUS_STARTSYNC;
		}
	}else{
		p_wk->data.timer--;
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�����҂�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaStartSync( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	if(CommIsTimingSync(_TIMING_GAME_CHECK)){

		// �X�e�[�^�X��������
		WFP2PMF_StatusChange( p_wk, p_init );

		CommToolTempDataReset();
		CommTimingSyncStart(_TIMING_GAME_CHECK2);
		p_wk->data.status = WFP2PMF_OYA_STATUS_STATE_SEND;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�X�e�[�^�X���M
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaStateSend( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	if(CommIsTimingSync(_TIMING_GAME_CHECK2)){
        u16 status = p_wk->data.p_match->myMatchStatus.status;
		BOOL result;
        result = CommToolSetTempData(CommGetCurrentID() ,&status);
		if( result == TRUE ){
	        p_wk->data.status = WFP2PMF_OYA_STATUS_STATE_CHECK;
		}
    }
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�X�e�[�^�X�`�F�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaStateCheck( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int i;
	int con_num;
	int current_id;
	const u16* cp_data;
	int ok_num;

	// �G���g���[�����擾
	con_num = WFP2PMF_GetEntryNum( p_wk );

	current_id = CommGetCurrentID();

	ok_num = 0;

	for( i=0; i<con_num; i++ ){

		if( current_id != i ){

			// �X�e�[�^�X���ꏏ���`�F�b�N
			cp_data = CommToolGetTempData( i );	

			// �X�e�[�^�X�̎�M���������Ă��邩�`�F�b�N
			if( cp_data != NULL ){
				if( cp_data[ 0 ] == p_wk->data.p_match->myMatchStatus.status ){
					ok_num ++;
				}else{
					p_wk->data.status = WFP2PMF_OYA_STATUS_ERR_INIT;
				}
			}
		}else{
			// ������OK
			ok_num ++;
		}
	}

	// �S���̃X�e�[�^�X����M���ă`�F�b�N���ʂ�OK�Ȃ玟�̏�����
	if( ok_num == con_num ){
		p_wk->data.status = WFP2PMF_OYA_STATUS_MYSTATUS_WAIT;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�X�e�[�^�X�`�F�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaMyStatusWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
//	CommInfoSendPokeData();
	CommStateSetErrorCheck(TRUE,TRUE);
	CommTimingSyncStart(_TIMING_GAME_START2);
	p_wk->data.status = WFP2PMF_OYA_STATUS_GAME_START;

	return FALSE;
}	

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@	�Q�[���J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaGameStart( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	if(CommIsTimingSync(_TIMING_GAME_START2)){
		// �����J�n�Ȃ�
		p_wk->data.match_result = TRUE;

		// �{�E�P���m�[�g�ݒ�
		WFP2PMF_FNOTE_Set( p_init, heapID );
		
		// WaitIcon������
		WFP2PMF_TimeWaitIconOff( p_wk, p_init, heapID );

		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�I���`�F�b�N�@������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaEndInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// ���b�Z�[�W�̕\��
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_123, heapID );

	p_wk->data.status = WFP2PMF_OYA_STATUS_END_INIT01;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�I���`�F�b�N�@������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaEndInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;
	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// YESNO�E�B���h�E���o��
	p_wk->draw.p_yesnowin = BmpYesNoSelectInitEx( p_wk->draw.p_bgl,
							&WFP2PMF_YESNOBMPDAT,
							WFP2PMF_BG2_MENUWIN_CGX, WFP2PMF_BG2_MENUWIN_PAL,
							1, heapID );
	p_wk->data.status = WFP2PMF_OYA_STATUS_END_SELECT;
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�I���`�F�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaEndSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int yesno_ret;
	
	// YES�Ȃ�R�l�N�g�@No�Ȃ�ؒf
	yesno_ret = BmpYesNoSelectMain( p_wk->draw.p_yesnowin, heapID );
	if( yesno_ret != BMPMENU_NULL ){
		if( yesno_ret == 0 ){	// �͂���I��
			p_wk->data.status = WFP2PMF_OYA_STATUS_END2_INIT00;
		}else{

			// �ʐM���Ă邩�`�F�b�N
			if( WFP2PMF_MatchCheck( p_wk ) ){
				// �l����MAX�ɂȂ�����Q�[���I����
				if( p_init->comm_max <= WFP2PMF_GetEntryNum( p_wk ) ){
					p_wk->data.status = WFP2PMF_OYA_STATUS_START_INIT00;
				}else{
					p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT;
				}
			}else{
				p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT;
			}
		}

		p_wk->draw.p_yesnowin = NULL;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�I���`�F�b�N2
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaEnd2Init00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// ���b�Z�[�W�\��	���̃����o�[�ł�낵���ł����H
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_124, heapID );

	p_wk->data.status = WFP2PMF_OYA_STATUS_END2_INIT01;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�I���`�F�b�N2
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaEnd2Init01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// YESNO�E�B���h�E���o��
	p_wk->draw.p_yesnowin = BmpYesNoSelectInitEx( p_wk->draw.p_bgl,
							&WFP2PMF_YESNOBMPDAT,
							WFP2PMF_BG2_MENUWIN_CGX, WFP2PMF_BG2_MENUWIN_PAL,
							1,heapID );
	p_wk->data.status = WFP2PMF_OYA_STATUS_END2_SELECT;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�I���`�F�b�N2
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaEnd2Select( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int yesno_ret;
	
	// YES�Ȃ�R�l�N�g�@No�Ȃ�ؒf
	yesno_ret = BmpYesNoSelectMain( p_wk->draw.p_yesnowin, heapID );
	if( yesno_ret != BMPMENU_NULL ){
		if( yesno_ret == 0 ){	// �͂���I��

			p_wk->data.status = WFP2PMF_OYA_STATUS_END3_INIT;
		}else{

			// �ʐM���Ă邩�`�F�b�N
			if( WFP2PMF_MatchCheck( p_wk ) ){
				// �l����MAX�ɂȂ�����Q�[���I����
				if( p_init->comm_max <= WFP2PMF_GetEntryNum( p_wk ) ){
					p_wk->data.status = WFP2PMF_OYA_STATUS_START_INIT00;
				}else{
					p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT;
				}
			}else{
				p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT;
			}
		}

		// �I��
		p_wk->draw.p_yesnowin = NULL;
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�I���`�F�b�N3
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaEnd3Init( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_125, heapID );

	// ���b�Z�[�W�\���Ԋu
	p_wk->data.timer = WIFI_P2PMATCH_MSG_WAIT;

	p_wk->data.status = WFP2PMF_OYA_STATUS_END;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�I������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaEnd( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// ���b�Z�[�W�\�����Ԃ��I���܂ł܂�
	p_wk->data.timer --;
	if( p_wk->data.timer > 0 ){
		return FALSE;
	}

	
	p_wk->data.match_result = FALSE;

	// �}�b�`���O���Ȃ�}�b�`���O������
//	if( WFP2PMF_MatchCheck(p_wk) ){  //�ēx�Ȃ����������ֈڍs���� 080525/k.o/0193
        CommStateWifiMatchEnd();
//	}

	// �݂�Ȑؒf����
	WFP2PMF_CommSendAllEntryNg( p_wk );

	// �ʐM��Ԃ����ɖ߂�
	CommStateChangeWiFiLogin();


	CommStateSetErrorCheck(FALSE,TRUE);  // �G���[���������甽������ 080525/k.o/0193
	
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�G���[����	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaErrInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// YesNo�̃��[�N����������j��
	if( p_wk->draw.p_yesnowin != NULL ){
		BmpYesNoWinDel( p_wk->draw.p_yesnowin, heapID );
		p_wk->draw.p_yesnowin = NULL;
	}
	WFP2PMF_GraphicNewConStrOff( &p_wk->draw );

	// �҂��A�C�R����\��
	WFP2PMF_TimeWaitIconOff( p_wk, p_init, heapID );


	p_wk->data.status = WFP2PMF_OYA_STATUS_ERR_INIT01;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�G���[����	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaErrInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// ���b�Z�[�W�\��	���̃����o�[�ł�낵���ł����H
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_120, heapID );

	p_wk->data.status = WFP2PMF_OYA_STATUS_ERR;

	// �݂�Ȑؒf����
	WFP2PMF_CommSendAllEntryNg( p_wk );

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@�G���[����
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaErr( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// �}�b�`���O������
    CommStateWifiMatchEnd();


	// �ʐM��Ԃ����ɖ߂�
	CommStateChangeWiFiLogin();

	// �݂�Ȑؒf����
	WFP2PMF_CommSendAllEntryNg( p_wk );


	// �I������
	p_wk->data.match_result = FALSE;

	CommStateSetErrorCheck(FALSE,TRUE);

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@vchat����	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaVchatInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	u32 idx;

	if( p_wk->data.p_match->myMatchStatus.vchat == TRUE ){
		idx = msg_wifilobby_128;
	}else{
		idx = msg_wifilobby_127;
	}
	
	// ���b�Z�[�W�\��	���̃����o�[�ł�낵���ł����H
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, idx, heapID );

	p_wk->data.status = WFP2PMF_OYA_STATUS_VCHAT_INIT01;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@vchat����	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaVchatInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// YESNO�E�B���h�E���o��
	p_wk->draw.p_yesnowin = BmpYesNoSelectInit( p_wk->draw.p_bgl,
							&WFP2PMF_YESNOBMPDAT,
							WFP2PMF_BG2_MENUWIN_CGX, WFP2PMF_BG2_MENUWIN_PAL,
							heapID );
	p_wk->data.status = WFP2PMF_OYA_STATUS_VCHAT_SELECT;
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@vchat����
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_OyaVchatSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int yesno_ret;
	
	// YES�Ȃ�R�l�N�g�@No�Ȃ�ؒf
	yesno_ret = BmpYesNoSelectMain( p_wk->draw.p_yesnowin, heapID );
	if( yesno_ret != BMPMENU_NULL ){
		if( yesno_ret == 0 ){	// �͂���I��
			
			WFP2PMF_VchatSwitch( p_wk, heapID );

			// �R�l�N�g���X�g�ĕ`��
			WFP2PMF_ConListWriteReq( p_wk );

		}
		p_wk->data.status = WFP2PMF_OYA_STATUS_CONNECT_WAIT_INIT;

		// �I��
		p_wk->draw.p_yesnowin = NULL;
	}
	
	return FALSE;
}



//-----------------------------------------------------------------------------
/**
 *		�q�@��ԁ@�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�e�̏�񂪓͂��܂ő҂�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoEntryOyaWaitInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	CommInfoSendPokeData();	// Info�f�[�^���M

	TOMOYA_PRINT( "�q�@�ʐM�f�[�^���M\n" );

	CommStateSetErrorCheck(FALSE,TRUE);	// �ؒf�̓G���[����Ȃ�

	p_wk->data.status = WFP2PMF_KO_STATUS_ENTRY_OYAWAIT;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�e�̏�񂪓͂��܂ő҂�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoEntryOyaWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{


	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );

	// �e����ؒf���ꂽ�炦��[������
	if( CommIsConnect(COMM_PARENT_ID) == FALSE ){
		p_wk->data.status = WFP2PMF_KO_STATUS_ERR_INIT;
	}else{
		
		// �e�̃f�[�^�����ł���̂͑҂�
		if( CommInfoGetEntry( COMM_PARENT_ID ) ){
			p_wk->data.status = WFP2PMF_KO_STATUS_ENTRY_INIT;
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�G���g���[�҂��@������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoEntryInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{

	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );

	// ���b�Z�[�W
	WFP2PMF_WordSetPlayerNameSet( &p_wk->draw, p_init, COMM_PARENT_ID, 0, heapID );
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_118, heapID );

	// �҂��A�C�R���\��
	WFP2PMF_TimeWaitIconOn( p_wk );

	p_wk->data.status = WFP2PMF_KO_STATUS_ENTRY_WAIT;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�G���g���[�҂�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoEntryWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	if( p_wk->data.entry != WFP2PMF_ENTRY_WAIT ){

		// WaitIcon������
		WFP2PMF_TimeWaitIconOff( p_wk, p_init, heapID );

		switch( p_wk->data.entry ){
		case WFP2PMF_ENTRY_KO:
			p_wk->data.status = WFP2PMF_KO_STATUS_ENTRY_OKINIT;
			break;
		case WFP2PMF_ENTRY_NG:
			p_wk->data.status		= WFP2PMF_KO_STATUS_ENTRY_NGINIT;
			p_wk->data.ng_msg_idx	= msg_wifilobby_130;
			break;
		case WFP2PMF_ENTRY_LOCK:
			p_wk->data.status		= WFP2PMF_KO_STATUS_ENTRY_NGINIT;
			p_wk->data.ng_msg_idx	= msg_wifilobby_136;

			// 080707	tomoya
			// �Đڑ��́A��������N���u��ʂɂ��ǂ��Ă���ƂȂ�܂����B
//			p_wk->data.status = WFP2PMF_KO_STATUS_CONLOCK_INIT00;
			break;
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�G���g���[�����肪�Ƃ�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoEntryOkInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// ���b�Z�[�W
	WFP2PMF_WordSetPlayerNameSet( &p_wk->draw, p_init, COMM_PARENT_ID, 0, heapID );
	WFP2PMF_GraphicMsgBmpStrPutEx( &p_wk->draw, p_init, msg_wifilobby_129, WFP2PMF_MSGENDWAIT, heapID );

	p_wk->data.status = WFP2PMF_KO_STATUS_ENTRY_OK;

	// �������G���g���[�ɂ���
	WFP2PMF_CommInfoSetEntry( p_wk, CommGetCurrentID(), HEAPID_WIFI_FOURMATCH );

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�G���g���[�����肪�Ƃ�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoEntryOk( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	p_wk->data.status = WFP2PMF_KO_STATUS_CONNECT_INIT;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�G���g���[���~��Ȃ������Ƃ�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoEntryNgInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// ���b�Z�[�W
	WFP2PMF_WordSetPlayerNameSet( &p_wk->draw, p_init, COMM_PARENT_ID, 0, heapID );
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, p_wk->data.ng_msg_idx, heapID );

	p_wk->data.status = WFP2PMF_KO_STATUS_ENTRY_NG;

	// 080703	tomoya	�ʐM�I���́A���b�Z�[�W�\�����ɍs��
	// �}�b�`���O������
	CommStateWifiMatchEnd();

	// �ʐM��Ԃ����ɖ߂�
	CommStateChangeWiFiLogin();

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�G���g���[���~��Ȃ������Ƃ�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoEntryNg( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// 080703	tomoya	�ʐM�I���́A���b�Z�[�W�\�����ɍs��
#if 0
	// �}�b�`���O������
	CommStateWifiMatchEnd();

	// �ʐM��Ԃ����ɖ߂�
	CommStateChangeWiFiLogin();
#endif

	// �I������
	p_wk->data.match_result = FALSE;

	CommStateSetErrorCheck(FALSE,TRUE);

	return TRUE;

}


//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�R�l�N�g�J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConnectInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{

	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );

	// ���b�Z�[�W
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_126, heapID );

	// �҂��A�C�R���\��
	WFP2PMF_TimeWaitIconOn( p_wk );

	p_wk->data.status = WFP2PMF_KO_STATUS_CONNECT_WAIT;

	// VCHATONOFF
	if( p_wk->data.p_match->myMatchStatus.vchat == TRUE ){

		WFP2PMF_StatusVChatOnBmpFade( p_wk );
	}


	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@���b�Z�[�W�I���҂�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConnectWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}


	p_wk->data.status = WFP2PMF_KO_STATUS_START;

	return FALSE;

}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�����҂�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoStart( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{

	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );

	OS_TPrintf( "Ko start \n" );
    
	// �V�����l���Ȃ�������VCHAT���Z�b�g
	if( p_wk->data.ko_newcon_flag ){
		p_wk->data.ko_newcon_flag = FALSE;

		// Vchat���Ȃ�
		if( p_wk->data.p_match->myMatchStatus.vchat == TRUE ){
			// �{�C�X�`���b�gOFF
			WFP2PMF_StatusVChatOff( p_wk );
			
			// �{�C�X�`���b�gOPOFIN_WAITN
			WFP2PMF_StatusVChatOnBmpFade( p_wk );
		}

		// �R�l�N�g���X�g�ĕ`��
		WFP2PMF_ConListWriteReq( p_wk );
	}

	if( WFP2PMF_MatchOkCheck( p_wk ) == TRUE ){
		CommToolTempDataReset();
		CommTimingSyncStart(_TIMING_GAME_CHECK2);

		WFP2PMF_TimeWaitIconOff( p_wk, p_init, heapID );

		// �ʐM�ҋ@���b�Z�[�W�\��
		WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_066, heapID );

		// �^�C���E�G�C�g�J�n
		WFP2PMF_TimeOut_Start( p_wk );

		// �q�@���̐l���`�F�b�N�E�G�C�g
		p_wk->data.timer = KO_ENTRYNUM_CHECK_WAIT;

		// �҂��A�C�R���\��
		WFP2PMF_TimeWaitIconOn( p_wk );
		
		p_wk->data.status = WFP2PMF_KO_STATUS_STATE_SEND;
		
	}else{

		// VChatOnOff
		if( sys.trg & PAD_BUTTON_X ){

			// �҂��A�C�R����\��
			WFP2PMF_TimeWaitIconOff( p_wk, p_init, heapID );
			p_wk->data.status = WFP2PMF_KO_STATUS_VCHAT_INIT00;
		}
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�X�e�[�^�X���M
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoStateSend( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int con_num;

    if(CommIsTimingSync(_TIMING_GAME_CHECK2)){
        u16 status = p_wk->data.p_match->myMatchStatus.status;
		BOOL result;

        // �G���g���[�����擾
        con_num = WFP2PMF_GetEntryNum( p_wk );

        if(CommGetConnectNum()!=con_num){
			p_wk->data.timer--;
			if( p_wk->data.timer <= 0 ){
	            // �������ؒf������
		        p_wk->data.status = WFP2PMF_KO_STATUS_ERR_INIT;
			}
			return FALSE;
        }


        result = CommToolSetTempData(CommGetCurrentID() ,&status);
		if( result == TRUE ){
	        p_wk->data.status = WFP2PMF_KO_STATUS_STATE_CHECK;
		}
        
    }
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�X�e�[�^�X�`�F�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoStateCheck( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int i;
	int con_num;
	int current_id;
	const u16* cp_data;
	int ok_num;

	// �G���g���[�����擾
	con_num = WFP2PMF_GetEntryNum( p_wk );

	current_id = CommGetCurrentID();

	ok_num = 0;

	for( i=0; i<con_num; i++ ){

		if( current_id != i ){

			// �X�e�[�^�X���ꏏ���`�F�b�N
			cp_data = CommToolGetTempData( i );	

			// �X�e�[�^�X�̎�M���������Ă��邩�`�F�b�N
			if( cp_data != NULL ){
				if( cp_data[ 0 ] == p_wk->data.p_match->myMatchStatus.status ){
					ok_num ++;
				}else{
					// �������ؒf������
					p_wk->data.status = WFP2PMF_KO_STATUS_ERR_INIT;
				}
			}
		}else{
			// ������OK
			ok_num ++;
		}
	}

	// �S���̃X�e�[�^�X����M���ă`�F�b�N���ʂ�OK�Ȃ玟�̏�����
	if( ok_num == con_num ){
		p_wk->data.status = WFP2PMF_KO_STATUS_MYSTATUS_WAIT;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�X�e�[�^�X�n���M
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoMyStatusWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
//	CommInfoSendPokeData();
	CommStateSetErrorCheck(TRUE,TRUE);
	CommTimingSyncStart(_TIMING_GAME_START2);
	p_wk->data.status = WFP2PMF_KO_STATUS_GAME_START;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@	�Q�[���J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoGameStart( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	if(CommIsTimingSync(_TIMING_GAME_START2)){

		// �҂��A�C�R����\��
		WFP2PMF_TimeWaitIconOff( p_wk, p_init, heapID );

		// �����J�n�Ȃ�
		p_wk->data.match_result = TRUE;

		// �{�E�P���m�[�g�ݒ�
		WFP2PMF_FNOTE_Set( p_init, heapID );

		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�G���[����	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoErrInit( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{

	// YesNo�̃��[�N����������j��
	if( p_wk->draw.p_yesnowin != NULL ){
		BmpYesNoWinDel( p_wk->draw.p_yesnowin, heapID );
		p_wk->draw.p_yesnowin = NULL;
	}
	WFP2PMF_GraphicNewConStrOff( &p_wk->draw );

	// �҂��A�C�R���\��
	WFP2PMF_TimeWaitIconOff( p_wk, p_init, heapID );


	p_wk->data.status = WFP2PMF_KO_STATUS_ERR_INIT01;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�G���[����	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoErrInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{

	// ���b�Z�[�W�\��	���̃����o�[�ł�낵���ł����H
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_120, heapID );

	p_wk->data.status = WFP2PMF_KO_STATUS_ERR;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�G���[����
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoErr( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// �}�b�`���O������
    CommStateWifiMatchEnd();


	// �ʐM��Ԃ����ɖ߂�
	CommStateChangeWiFiLogin();

	// �I������
	p_wk->data.match_result = FALSE;

	CommStateSetErrorCheck(FALSE,TRUE);

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�{�C�X�`���b�gONOFF
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoVchatInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	u32 idx;

	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );


	if( p_wk->data.p_match->myMatchStatus.vchat == TRUE ){
		idx = msg_wifilobby_128;
	}else{
		idx = msg_wifilobby_127;
	}
	
	// ���b�Z�[�W�\��	���̃����o�[�ł�낵���ł����H
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, idx, heapID );

	p_wk->data.status = WFP2PMF_KO_STATUS_VCHAT_INIT01;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�{�C�X�`���b�gONOFF
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoVchatInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;


	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );


	// ��W���I�������VCHAT�ύX������CANCEL
	if( WFP2PMF_MatchOkCheck( p_wk ) == TRUE ){
		p_wk->data.status = WFP2PMF_KO_STATUS_START;
		// ���b�Z�[�W�\��OFF
		WFP2PMF_GraphicMsgBmpStrOff( &p_wk->draw );
		return FALSE;
	}

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// YESNO�E�B���h�E���o��
	p_wk->draw.p_yesnowin = BmpYesNoSelectInit( p_wk->draw.p_bgl,
							&WFP2PMF_YESNOBMPDAT,
							WFP2PMF_BG2_MENUWIN_CGX, WFP2PMF_BG2_MENUWIN_PAL,
							heapID );
	p_wk->data.status = WFP2PMF_KO_STATUS_VCHAT_SELECT;
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@�{�C�X�`���b�gONOFF
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoVchatSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int yesno_ret;

	// �������ႢNETID�̐l�̃f�[�^����M������
	// �G���g���[��Ԃɂ���
	WFP2PMF_SetEntryOya( p_wk, heapID );

	// ��W���I�������VCHAT�ύX������CANCEL
	if( WFP2PMF_MatchOkCheck( p_wk ) == TRUE ){
		p_wk->data.status = WFP2PMF_KO_STATUS_START;
		// ���b�Z�[�W�\��OFF
		WFP2PMF_GraphicMsgBmpStrOff( &p_wk->draw );
		BmpYesNoWinDel( p_wk->draw.p_yesnowin, heapID );
		p_wk->draw.p_yesnowin = NULL;
		return FALSE;
	}
	
	// YES�Ȃ�R�l�N�g�@No�Ȃ�ؒf
	yesno_ret = BmpYesNoSelectMain( p_wk->draw.p_yesnowin, heapID );
	if( yesno_ret != BMPMENU_NULL ){
		if( yesno_ret == 0 ){	// �͂���I��
			
			WFP2PMF_VchatSwitch( p_wk, heapID );

			// �R�l�N�g���X�g�ĕ`��
			WFP2PMF_ConListWriteReq( p_wk );

		}
		p_wk->data.status = WFP2PMF_KO_STATUS_CONNECT_INIT;

		// �I��
		p_wk->draw.p_yesnowin = NULL;
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM���b�N���̂��߁@�Đڑ��@�������O�O
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConLockInit00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{

	// �e�̗F�Bnumber���擾
	p_wk->data.oya_friendid = CommInfoSearchWifiListIndex( COMM_PARENT_ID );
	GF_ASSERT( p_wk->data.oya_friendid != WIFILIST_FRIEND_MAX );

	// ���[�h�Z�b�g�ɖ��O�������Ă����Ƃ�
	WFP2PMF_WordSetPlayerNameSet( &p_wk->draw, p_init, COMM_PARENT_ID, 0, heapID );


	CommStateWifiMatchEnd();

	// �G���[�`�F�b�N�@OFF
	p_wk->data.err_check = FALSE;
	
	// ���b�Z�[�W�\��	���̃����o�[�ł�낵���ł����H
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_136, heapID );

	p_wk->data.status = WFP2PMF_KO_STATUS_CONLOCK_INIT01;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM���b�N���̂��߁@�Đڑ��@�������O�P
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConLockInit01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;

	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	// YESNO�E�B���h�E���o��
	p_wk->draw.p_yesnowin = BmpYesNoSelectInit( p_wk->draw.p_bgl,
							&WFP2PMF_YESNOBMPDAT,
							WFP2PMF_BG2_MENUWIN_CGX, WFP2PMF_BG2_MENUWIN_PAL,
							heapID );
	p_wk->data.status = WFP2PMF_KO_STATUS_CONLOCK_SELECT;
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM���b�N���̂��߁@�Đڑ��@�I��
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConLockSelect( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	int yesno_ret;

	
	// YES�Ȃ�R�l�N�g�@No�Ȃ�ؒf
	yesno_ret = BmpYesNoSelectMain( p_wk->draw.p_yesnowin, heapID );
	if( yesno_ret != BMPMENU_NULL ){
		if( yesno_ret == 0 ){	// �͂���I��
			p_wk->data.status = WFP2PMF_KO_STATUS_CONLOCK_RECON;
		}else{
			p_wk->data.status = WFP2PMF_KO_STATUS_CONLOCK_DECON;
		}
		// �I��
		p_wk->draw.p_yesnowin = NULL;
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM���b�N���̂��߁@�Đڑ��@�Đڑ�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConLockReCon( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{

	// ���b�Z�[�W�\��	�`����ɂ��������݂��イ�ł�
	WFP2PMF_GraphicMsgBmpStrPut( &p_wk->draw, p_init, msg_wifilobby_014, heapID );

	// �҂��A�C�R���\��
	WFP2PMF_TimeWaitIconOn( p_wk );

	// �E�G�C�g
	p_wk->data.timer = RECONNECT_WAIT;

	p_wk->data.status = WFP2PMF_KO_STATUS_CONLOCK_RECON01;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM���b�N���̂��߁@�Đڑ��@�Đڑ�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConLockReCon01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	BOOL result;


	// �E�G�C�g
	if( p_wk->data.timer > 0 ){
		p_wk->data.timer--;
		return FALSE;
	}
	result = WFP2PMF_GraphicMsgBmpStrMain( &p_wk->draw );
	// ���b�Z�[�W�\�����I���܂ł܂�
	if( result == FALSE ){
		return FALSE;
	}

	TOMOYA_PRINT( "wifi�ڑ��� %d\n", p_wk->data.oya_friendid );

	// �Đڑ�
	if( WFP2PMF_CommWiFiMatchStart( p_wk->data.oya_friendid, p_init->type ) ){

        p_wk->data.status = WFP2PMF_KO_STATUS_CONLOCK_RECONWAIT;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM���b�N���̂��߁@�Đڑ��@�Đڑ��҂�
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConLockReConWait( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// �����ł͓Ǝ���ERR�`�F�b�N���s��
	WFP2PMF_ReConErrCheck( p_wk );
	
	// �ҋ@
	if( (CommWifiIsMatched()==1) && (
		CommIsConnect(COMM_PARENT_ID) == TRUE) ){

		//
		CommSetWifiBothNet(FALSE);
			
		// �҂��A�C�R���\��
		WFP2PMF_TimeWaitIconOff( p_wk, p_init, heapID );

		// ���C�v�t�F�[�h�J�n
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK,
						COMM_BRIGHTNESS_SYNC, 1, heapID);


		p_wk->data.status = WFP2PMF_KO_STATUS_CONLOCK_GRAPHIC_RESET00;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM���b�N���̂��߁@�Đڑ��@��ʍč\�z
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConLockReConGraphicReset00( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	if( WIPE_SYS_EndCheck() ){

		// �܂��͒ʐM�̊J��
		CommInfoInitialize(p_init->p_savedata,NULL);   //Info������

		// �G���[�`�F�b�N�@ON
		p_wk->data.err_check = TRUE;

		// ��ʍč\�z
		{
			// OAM�j������
			WFP2PMF_DrawOamDelete( p_wk );
			// �`��f�[�^�j��
			WFP2PMF_GraphicDelete( p_wk, heapID );

			// �`�惏�[�N���N���[��
			memset( &p_wk->draw, 0, sizeof(WFP2PMF_DRAW) );

			// �ؒf�`�F�b�N�J�n
			WFP2PMF_LogOutCheckStart( p_wk );

			// �`��f�[�^������
			WFP2PMF_GraphicInit( p_wk, p_init, heapID );
			
			WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��
		}

		// ���[�N�̃N���A
		{
			p_wk->data.ko_newcon_flag = 0;
			p_wk->data.entry = 0;
			memset( p_wk->data.logout_in, 0, sizeof(u8)*WIFI_P2PMATCH_NUM_MAX );// LOG�A�E�g���m
			memset( p_wk->data.logout_id, 0, sizeof(u8)*WIFI_P2PMATCH_NUM_MAX );// LOG�A�E�g���m
			MI_CpuFill8( p_wk->data.vchat_tmp, TRUE, sizeof(u8)*WIFI_P2PMATCH_NUM_MAX );// �P�O��VCHAT
			MI_CpuFill8( p_wk->data.vchat_now, TRUE, sizeof(u8)*WIFI_P2PMATCH_NUM_MAX );// �P�O��VCHAT
			p_wk->data.conlist_rewrite = 0;	// conlist���ĕ`��
			p_wk->data.oya_friendid = 0;	// �e�̗F�B�R�[�h
			memset( p_wk->data.entry_on, 0, sizeof(u8)*WIFI_P2PMATCH_NUM_MAX );// �G���g���[�t���O
		}

		// ���C�v�t�F�[�h�J�n
        WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK,
                        COMM_BRIGHTNESS_SYNC, 1, heapID);

		p_wk->data.status = WFP2PMF_KO_STATUS_CONLOCK_GRAPHIC_RESET01;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM���b�N���̂��߁@�Đڑ��@��ʍč\�z
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConLockReConGraphicReset01( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	if( WIPE_SYS_EndCheck() ){
		// �ڑ��J�n
		p_wk->data.status = WFP2PMF_KO_STATUS_ENTRY_OYAWAITINIT;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM���b�N���̂��߁@�Đڑ��@�I������
 *
 *	@param	p_wk		���[�N
 *	@param	p_init		��̃A�v������̃f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�}�b�`���O�I��
 *	@retval	FALSE	�}�b�`���O��
 */
//-----------------------------------------------------------------------------
static BOOL WFP2PMF_KoConLockDeCon( WFP2PMF_WK* p_wk, WFP2PMF_INIT* p_init, u32 heapID )
{
	// �}�b�`���O������
    CommStateWifiMatchEnd();


	// �ʐM��Ԃ����ɖ߂�
	CommStateChangeWiFiLogin();

	// �I������
	p_wk->data.match_result = FALSE;

	CommStateSetErrorCheck(FALSE,TRUE);

	return TRUE;
}


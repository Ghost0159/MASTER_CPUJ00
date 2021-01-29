//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		minigame_tool.c
 *	@brief		�~�j�Q�[���c�[��
 *	@author		tomoya takahashi
 *	@data		2007.10.22
 *
 *
 *	�������̔��ȉ�
 *		�J�n��ʂƌ��ʉ�ʂ�PROC�Ƃ��č쐬�����ق����悩�����B
 *		���̃V�X�e�����ƊJ�n��ʂƌ��ʉ�ʂ��o���Ă���̂̓~�j�Q�[�����ł����A
 *
 *		�~�j�Q�[���Ď��V�X�e��
 *				�[���J�n���PROC
 *				�[���~�j�Q�[��PROC
 *				�[�����ʉ��PROC
 *		�ƃ~�j�Q�[�����Ď����Ă���V�X�e�����Ăяo���悤�ɂ���ׂ��ł����B
 *
 *		�������邱�ƂŊJ�n��ʂƌ��ʉ�ʂ̕ύX�����ڃ~�j�Q�[���ɉe�����Ȃ��Ȃ�A
 *		�J���������ƃX���[�Y�������Ǝv���܂��B
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "system/fontproc.h"
#include "system/clact_util.h"
#include "system/wipe.h"
#include "system/msgdata.h"
#include "system/wordset.h"
#include "system/particle.h"
#include "system/window.h"
#include "system/touch_subwindow.h"
#include "system/laster.h"

#include "savedata/config.h"
#include "savedata/wifilist.h"

#include "contest/con_tool.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_minigame_tool.h"
#include "msgdata/msg_wifi_place_msg_world.h"

#include "graphic/wlmngm_tool.naix"

#include "minigame_tool_snd.h"
#include "application/wifi_lobby/minigame_tool.h"
#include "minigame_tool_local.h"
#include "minigame_commcomand.h"
#include "minigame_commcomand_func.h"
#include "wflby_def.h"

#include "communication/communication.h"
#include "wifi/dwc_rapfriend.h"

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
//#define DEBUG_SYNCSTART_A	// �����̊J�n��A�{�^���ɂ���
//#define DEBUG_MINIGAME_AUTO_PLAY	// WiFi�N���u�ł̂�AUTO���[�v�v���C
//#define DEBUG_MINIGAME_DISCONNECT	// A�{�^���Őؒf
#endif


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

#define MNGM_RESCONTID			( 5000 )


//-------------------------------------
///	�ėp�V�X�e��
//=====================================
// �Z���A�N�^�[���\�[�X�Ǘ�
#define MNGM_CLACTRES_RESMAN_NUM	( 4 )		// ���\�[�X�}�l�[�W����
#define MNGM_CLACTRES_DEF_BG_PRI	( 1 )		// BG�D�揇��

// VRAM�]���A�j���@���[�N��
#define MNGM_VRAMTR_WK_NUM			( 16 )

// ���b�Z�[�W�Ǘ�
#define MNGM_MSG_STRBUF_NUM			( 128 )
#define MNGM_MSG_PLTT				( 12 )
#define MNGM_MSG_COLOR				( GF_PRINTCOLOR_MAKE( 15,14,0 ) )
#define MNGM_MSG_VIP_COLOR			( GF_PRINTCOLOR_MAKE( 5,6,0 ) )
#define MNGM_MSG_TALKCOLOR			( GF_PRINTCOLOR_MAKE( 1,2,15 ) )
#define MNGM_MSG_TIME_1DATA			( 100 )	// miri�b�ł�1�b�̒P��
#define MNGM_MSG_TIME_1DATADIV		( 30 )	// 1�b�̃V���N��

// ��b�E�B���h�E�Ǘ�
enum{
	MNGM_TALKWIN_IDX_MAIN,
	MNGM_TALKWIN_IDX_SUB,
	MNGM_TALKWIN_IDX_NUM,
};
#define MNGM_TALKWIN_CGX			( 1 )	// ���D�̃E�B���h�E�L�����N�^��
#define MNGM_TALKWIN_PAL			( 13 )
#define MNGM_TALKWIN_BMP_X			( 2 )
#define MNGM_TALKWIN_BMP_Y			( 19 )
#define MNGM_TALKWIN_BMP_Y_SUB		( 1 )
#define MNGM_TALKWIN_BMP_SIZX		( 27 )
#define MNGM_TALKWIN_BMP_SIZY		( 4 )
#define	MNGM_TALKWIN_BMP_PAL		( MNGM_MSG_PLTT )
#define	MNGM_TALKWIN_BMP_CGX		( (MNGM_TALKWIN_CGX + TALK_WIN_CGX_SIZ) )
#define MNGM_TALKWIN_MSG_SPEED		( 1 )
#define MNGM_TALKWIN_BMP_CGX_END	( MNGM_TALKWIN_BMP_CGX + (MNGM_TALKWIN_BMP_SIZX*MNGM_TALKWIN_BMP_SIZY) )
  
//-------------------------------------
///	�v���C��[PLATE
//=====================================
#define MNGM_PLAYER_PLATE_SCRN_SIZ_X	( 32 )
#define MNGM_PLAYER_PLATE_SCRN_SIZ_Y	( 6 )
enum{// BG�p���b�g�i���o�[
	MNGM_PLAYER_BG_PLTT_RED,
	MNGM_PLAYER_BG_PLTT_BLUE,
	MNGM_PLAYER_BG_PLTT_GREEN,
	MNGM_PLAYER_BG_PLTT_YELLOW,
	MNGM_PLAYER_BG_PLTT_ANM,
	MNGM_PLAYER_BG_PLTT_BACK,
	MNGM_PLAYER_BG_PLTT_XX00,
	MNGM_PLAYER_BG_PLTT_XX01,
};
enum{// �A�j���pBG�p���b�g�i���o�[
	MNGM_PLAYER_BG_ANMPLTT_BALLOON,
	MNGM_PLAYER_BG_ANMPLTT_BALLSLOW,
	MNGM_PLAYER_BG_ANMPLTT_BALANCE,
	MNGM_PLAYER_BG_ANMPLTT_TOPANM0_0,
	MNGM_PLAYER_BG_ANMPLTT_TOPANM0_1,
	MNGM_PLAYER_BG_ANMPLTT_TOPANM0_2,
	MNGM_PLAYER_BG_ANMPLTT_TOPANM0_3,
	MNGM_PLAYER_BG_ANMPLTT_TOPANM1_0,
	MNGM_PLAYER_BG_ANMPLTT_TOPANM1_1,
	MNGM_PLAYER_BG_ANMPLTT_TOPANM1_2,
	MNGM_PLAYER_BG_ANMPLTT_TOPANM1_3,
	MNGM_PLAYER_BG_ANMPLTT_ANM
};
// PLATE��OAM�ʒu
#define MNGM_PLATE_CLACT_PLATE_ANM_NUM	( 4 )
static const s16 sc_MNGM_PLATE_CLACT_PLATE_POS[ MNGM_PLATE_CLACT_PLATE_ANM_NUM ][2] = {
	{ 128, 44 },	// Y���W��sc_MNGM_PLAYER_PLATE_DATA��top�����o����̃I�t�Z�b�g
	{ 236, 24 },
	{ 4, 24 },
	{ 128, 4 }
};
// BG�p���b�g��
#define MNGM_BGPLTT_NUM			( 9 )
static const u8 sc_MNGM_BG_PLTT_PLNO_TBL[ WFLBY_MINIGAME_MAX ] = {
	0,1,3,2
};
// OAM�A�j���V�[�P���X
enum{
	MNGM_PLAYER_OAM_ANMSEQ_1P,
	MNGM_PLAYER_OAM_ANMSEQ_3P,
	MNGM_PLAYER_OAM_ANMSEQ_4P,
	MNGM_PLAYER_OAM_ANMSEQ_2P,
	MNGM_PLAYER_OAM_ANMSEQ_TOP,
	MNGM_PLAYER_OAM_ANMSEQ_SECOND,
	MNGM_PLAYER_OAM_ANMSEQ_THIRD,
	MNGM_PLAYER_OAM_ANMSEQ_FORE,
};
// OAM�p���b�g��
#define MNGM_PLAYER_OAM_PLTT_NUM   (5)
// �����L���OOAM�̈ʒu
// Y���W��sc_MNGM_PLAYER_PLATE_DATA��top�����o����̃I�t�Z�b�g
#define MNGM_PLATE_CLACT_RANK_X	( 24 )
#define MNGM_PLATE_CLACT_RANK_Y	( 21 )

// PLATE�̓���
#define	MNGM_PLATE_MOVE_START		( -255 )
#define	MNGM_PLATE_MOVE_SPEED		( 28 )
#define	MNGM_PLATE_MOVE_COUNT		( 8 )
#define MNGM_PLATE_MOVE_ENTRY_END	( -8 )
#define MNGM_PLATE_MOVE_WAIT_ENTRY	( 3 )
static const s16 sc_MNGM_PLATE_MOVE_RESULT_END[ WFLBY_MINIGAME_MAX ] = {
	0, -10, -16,  -24
};


//-------------------------------------
///	BG�ʂ̃X�N���[���X�s�[�h
//=====================================
#define MNGM_BGSCROLL_SPEED	( 2 )



//-------------------------------------
///	�G���g���[��ʃ��[�N
//=====================================
enum{
	MNGM_ENTRY_SEQ_WIPEIN_INIT,
	MNGM_ENTRY_SEQ_WIPEIN_WAIT,
	MNGM_ENTRY_SEQ_LOGOIN_WAIT,
	MNGM_ENTRY_SEQ_MSGON_INIT,
	MNGM_ENTRY_SEQ_MSGON_WAIT,
	MNGM_ENTRY_SEQ_BGMOVE_START,
	MNGM_ENTRY_SEQ_LOGOOUT_WAIT,
	MNGM_ENTRY_SEQ_PLLIST_ON,
	MNGM_ENTRY_SEQ_BGMOVE_MAIN,
	MNGM_ENTRY_SEQ_BGMOVE_WAIT,
	MNGM_ENTRY_SEQ_BGMOVE_SYNC,
	MNGM_ENTRY_SEQ_BGMOVE_OUT,
	MNGM_ENTRY_SEQ_BGMOVE_OUTWAIT,
	MNGM_ENTRY_SEQ_WIPEOUT_INIT,
	MNGM_ENTRY_SEQ_WIPEOUT_WAIT,
	MNGM_ENTRY_SEQ_RAREGAME_WAIT,
	MNGM_ENTRY_SEQ_END,
};
#define MNGM_ENTRY_MSGWAIT			( 64 )	// ���b�Z�[�W�\���E�G�C�g
#define MNGM_ENTRY_WAIT				( 92 )	// �e�[�u����������E�G�C�g
#define MNGM_ENTRY_OUTMOVEWAIT		( 8 )	// �e�[�u����������E�G�C�g

// BGL
#define MNGM_ENTRY_BMP_X		( 5 )
#define MNGM_ENTRY_BMP_Y		( 1 )
#define MNGM_ENTRY_BMP_Y_DIF	( 6 )
#define MNGM_ENTRY_BMP_SIZX		( 26 )
#define MNGM_ENTRY_BMP_SIZY		( 4 )
#define MNGM_ENTRY_BMP_PAL		( MNGM_MSG_PLTT )
#define MNGM_ENTRY_BMP_CGX		( 513 )

// OAM�ݒ�
#define MNGM_ENTRY_CLACT_RESNUM		( 1 )	// �Z���A�N�^�[�Z�b�g�̃��[�N��
#define MNGM_ENTRY_CLACT_WKNUM		( 16 )	// �Z���A�N�^�[�Z�b�g�̃��[�N��
#define MNGM_ENTRY_CLACT_CHARNUM	( 1 )	// �Z���A�N�^�[�L�����N�^�̃��[�N��
#define MNGM_ENTRY_CLACT_PLTTNUM	( 1 )	// �Z���A�N�^�[�p���b�g�̃��[�N��

// �G���g���[�\�����b�Z�[�W�ʒu
#define MNGM_ENTRY_DRAWNAME_X	( 8 )


//-------------------------------------
///	���U���g��ʃ��[�N
//=====================================
enum{	// �ʂ���
	MNGM_RESULT_BALLSLOW_SEQ_WIPEIN_INIT,
	MNGM_RESULT_BALLSLOW_SEQ_WIPEIN_WAIT,
	MNGM_RESULT_BALLSLOW_SEQ_MSGON_INIT,
	MNGM_RESULT_BALLSLOW_SEQ_MSGON_WAIT,
	MNGM_RESULT_BALLSLOW_SEQ_MSGON_WAIT2,
	MNGM_RESULT_BALLSLOW_SEQ_COUNTDOWN_INIT,
	MNGM_RESULT_BALLSLOW_SEQ_COUNTDOWN,
	MNGM_RESULT_BALLSLOW_SEQ_TBLMOVE_WAIT,
	MNGM_RESULT_BALLSLOW_SEQ_EFFECT_ON,
	MNGM_RESULT_BALLSLOW_SEQ_EFFECT_WAIT,

	// �K�W�F�b�g�o���ꍇ
	MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG00_ON,	// ���ʂɑ΂��郁�b�Z�[�W
	MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG00_WAIT,
	MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG_ON,		// �K�W�F�b�g�ɑ΂��郁�b�Z�[�W
	MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG_WAIT,
	MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG_WAIT2,
	
	// �I������
	MNGM_RESULT_BALLSLOW_SEQ_SYNCSTART,
	MNGM_RESULT_BALLSLOW_SEQ_SYNCWAIT,

	// ���̂܂܏I������ꍇ
	MNGM_RESULT_BALLSLOW_SEQ_WIPEOUT_INIT,
	MNGM_RESULT_BALLSLOW_SEQ_WIPEOUT_WAIT,

	// �Ē���𕷂��ꍇ
	MNGM_RESULT_BALLSLOW_SEQ_RETRY_INIT,
	MNGM_RESULT_BALLSLOW_SEQ_RETRY_MAIN,

	MNGM_RESULT_BALLSLOW_SEQ_END,

	// �S���̃X�R�A���O�̂Ƃ��̏���
	MNGM_RESULT_BALLSLOW_SEQ_ALLSCORE0_WAIT,
	MNGM_RESULT_BALLSLOW_SEQ_ALLSCORE0_DRAW,
};

enum{	// ������
	MNGM_RESULT_BALLOON_SEQ_WIPEIN_INIT,
	MNGM_RESULT_BALLOON_SEQ_WIPEIN_WAIT,
	MNGM_RESULT_BALLOON_SEQ_MSGON_INIT,
	MNGM_RESULT_BALLOON_SEQ_MSGON_WAIT,
	MNGM_RESULT_BALLOON_SEQ_MSGON_WAIT2,

	// ���D�\������
	MNGM_RESULT_BALLOON_SEQ_TBLON,
	MNGM_RESULT_BALLOON_SEQ_BALLOON_ON,
	MNGM_RESULT_BALLOON_SEQ_BALLOON_ONWAIT,

	// ���������\��
	MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_00ON,
	MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_00WAIT,

	// �K�W�F�b�g�o���ꍇ
	MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_ON,
	MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_WAIT,
	MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_WAIT2,

	// �G�t�F�N�g���o��
	MNGM_RESULT_BALLOON_SEQ_EFFECT_WAIT,
	
	
	// �I������
	MNGM_RESULT_BALLOON_SEQ_SYNCSTART,
	MNGM_RESULT_BALLOON_SEQ_SYNCWAIT,

	// ���̂܂܏I������ꍇ
	MNGM_RESULT_BALLOON_SEQ_WIPEOUT_INIT,
	MNGM_RESULT_BALLOON_SEQ_WIPEOUT_WAIT,

	// �Ē���𕷂��ꍇ
	MNGM_RESULT_BALLOON_SEQ_RETRY_INIT,
	MNGM_RESULT_BALLOON_SEQ_RETRY_MAIN,

	MNGM_RESULT_BALLOON_SEQ_END,
};

#define MNGM_RESULT_MSGDRAW_WAIT		( 102 )	// �E�G�C�g
#define MNGM_RESULT_RESULT_DRAW_WAIT	( 30 )// ���ʂ������鎞��
#define MNGM_RESULT_RESULT_SYNC_WAIT	( 102 )// ���ʂ������鎞��
#define MNGM_RESULT_BALLOON_DRAW_WAIT	( 16 )	// ���D���o���܂ł̎���

// OAM�ݒ�
#define MNGM_RESULT_CLACT_RESNUM	( 2 )	// �Z���A�N�^�[�Z�b�g�̃��[�N��
#define MNGM_RESULT_CLACT_WKNUM		( 32 )	// �Z���A�N�^�[�Z�b�g�̃��[�N��
#define MNGM_RESULT_CLACT_CHARNUM	( 2 )	// �Z���A�N�^�[�L�����N�^�̃��[�N��
#define MNGM_RESULT_CLACT_PLTTNUM	( 2 )	// �Z���A�N�^�[�p���b�g�̃��[�N��

// ���ʕ\���E�G�C�g
#define MNGM_RESULT_DRAWON_WAIT_34	( 16 )	// �\���Ԋu
#define MNGM_RESULT_DRAWON_WAIT_12	( 32 )	// �\���Ԋu
#define MNGM_RESULT_DRAWNAME_X	( 8 )
#define MNGM_RESULT_DRAWNAME_Y	( 0 )
#define MNGM_RESULT_DRAWSCORE_X	( 176 )
#define MNGM_RESULT_DRAWSCORE_Y	( 0 )

// �p���b�g�]���A�j��
#define MNGM_RESULT_PALANM_DEFTRPAL	( 0 )	// ��{�]���p���b�g�ʒu
#define MNGM_RESULT_PALANM_FRAME	( 8 )	// �p���b�g�A�j���t���[��

// ���ʉ�ʂ̃X�N���[����
enum{
	MNGM_RESULT_SCRN_TBL,
	MNGM_RESULT_SCRN_NUM,
};

// ���g���C���[�N
#define MNGM_RESULT_RETRY_CHAR_OFFS		(MNGM_TALKWIN_BMP_CGX_END)	// �L�����N�^�I�t�Z�b�g
#define MNGM_RESULT_RETRY_PLTT_OFFS		(2)	// �p���b�g�I�t�Z�b�g
#define MNGM_RESULT_RETRY_X				(24)
#define MNGM_RESULT_RETRY_Y				(8)
enum{
	MNGM_RESULT_RETRY_SEQ_WIPEIN,		// ���C�v�A�E�g
	MNGM_RESULT_RETRY_SEQ_WIPEINWAIT,	// ���C�v�A�E�g�E�G�C�g
	MNGM_RESULT_RETRY_SEQ_MSGON,		// ���b�Z�[�W�\��
	MNGM_RESULT_RETRY_SEQ_MSGWAIT,	
	MNGM_RESULT_RETRY_SEQ_TPON,			// �^�b�`�{�^���\��
	MNGM_RESULT_RETRY_SEQ_TPMAIN,
	MNGM_RESULT_RETRY_SEQ_RECV,			// �݂�Ȃ̑I������M
	MNGM_RESULT_RETRY_SEQ_RECVMSGON,	// �N������߂��I�񂾂�������E�E�E
	MNGM_RESULT_RETRY_SEQ_RECVMSGWAIT,	// 
	MNGM_RESULT_RETRY_SEQ_RECVMSGWAIT2,	// 
	MNGM_RESULT_RETRY_SEQ_WIPEOUT,		// ���C�v�A�E�g
	MNGM_RESULT_RETRY_SEQ_WIPEOUTWAIT,	// ���C�v�A�E�g�E�G�C�g
	MNGM_RESULT_RETRY_SEQ_END,
};

// BALLOON
enum{
	MNGM_RESULT_BALLOON_SEQ_ON,
	MNGM_RESULT_BALLOON_SEQ_BLN_ON,
	MNGM_RESULT_BALLOON_SEQ_BLN_ONWAIT,
	MNGM_RESULT_BALLOON_SEQ_BLN_END,
};
// OAM
#define MNGM_RESULT_BALLOON_X				( 5 )
#define MNGM_RESULT_BALLOON_Y				( 3 )
#define MNGM_RESULT_BALLOON_CLACT_NUM		( MNGM_RESULT_BALLOON_X*MNGM_RESULT_BALLOON_Y )
#define MNGM_RESULT_BALLOON_CONTID			( MNGM_RESCONTID+1 )
#define MNGM_RESULT_BALLOON_BG_PRI			( 0 )
#define MNGM_RESULT_BALLOON_DEF_X			( 74 )
#define MNGM_RESULT_BALLOON_SIZ_X			( 24 )
#define MNGM_RESULT_BALLOON_DEF_Y			( 40 )
#define MNGM_RESULT_BALLOON_SIZ_Y			( 36 )
#define MNGM_RESULT_BALLOON_ANM_RED			( 2 )

// BG
#define MNGM_RESULT_BALLOON_SYSWIN_CGX		( MNGM_TALKWIN_BMP_CGX_END )
#define MNGM_RESULT_BALLOON_SYSWIN_PAL		( MNGM_TALKWIN_PAL+1 )
#define MNGM_RESULT_BALLOON_SYSWIN_SCRNSIZX	( 32 )
#define MNGM_RESULT_BALLOON_SYSWIN_SCRNSIZY	( 20 )
/*
#define MNGM_RESULT_BALLOON_BMP_X			( 4 )
#define MNGM_RESULT_BALLOON_BMP_Y			( 3 )
#define MNGM_RESULT_BALLOON_BMP_SIZX		( 23 )
#define MNGM_RESULT_BALLOON_BMP_SIZY		( 14 )
#define MNGM_RESULT_BALLOON_BMP_PAL			( MNGM_MSG_PLTT )
#define MNGM_RESULT_BALLOON_BMP_CGX			( MNGM_RESULT_BALLOON_SYSWIN_CGX+MENU_WIN_CGX_SIZ )
*/

// ����
#define MNGM_RESULT_BALLOON_WAIT			( 8 )




//-------------------------------------
///	START	TIMEUP	���[�N
//=====================================
#define MNGM_COUNT_OAMRESNUM	(1)
enum{	// REDY	START
	MNGM_COUNT_START_SEQ_NONE,
	MNGM_COUNT_START_SEQ_INIT,
	MNGM_COUNT_START_SEQ_MSK,
	MNGM_COUNT_START_SEQ_ANM,
	MNGM_COUNT_START_SEQ_MSK2,
	MNGM_COUNT_START_SEQ_END,
};
enum{	// TIME UP
	MNGM_COUNT_TIMEUP_SEQ_NONE,
	MNGM_COUNT_TIMEUP_SEQ_INIT,
	MNGM_COUNT_TIMEUP_SEQ_MSK,
	MNGM_COUNT_TIMEUP_SEQ_ANM,
	MNGM_COUNT_TIMEUP_SEQ_MSK2,
	MNGM_COUNT_TIMEUP_SEQ_END,
};
#define MNGM_COUNT_ANM_X	( 128 )
#define MNGM_COUNT_ANM_Y	( 97 )
#define MNGM_COUNT_ANM_PRI	( 64 )
#define MNGM_COUNT_TIMEUP_ANM_NUM	( 50 )
#define MNGM_COUNT_REDY_ANM			( 7 )
#define MNGM_COUNT_3_ANM			( 9 )
#define MNGM_COUNT_2_ANM			( 11 )
#define MNGM_COUNT_1_ANM			( 13 )
#define MNGM_COUNT_STARTFRAME		( 15 )
#define MNGM_COUNT_STARTPALANMTIMING	( 4 )	// �p���b�g�A�j���^�C�~���O
// �}�X�N�V�X�e��
enum{
	MNGM_COUNT_MSK_OAM_TOP,
	MNGM_COUNT_MSK_OAM_BTTM,
	MNGM_COUNT_MSK_OAM_NUM,
};
#define MNGM_COUNT_MSK_X		( 128 )
#define MNGM_COUNT_MSK_Y		( 96 )
#define MNGM_COUNT_MSK_Y_D		( 2 )
#define MNGM_COUNT_MSK_PRI		( 0 )
#define MNGM_COUNT_MSK_MY		( 23 )	// Y�Ɉړ�����l
#define MNGM_COUNT_MSK_COUNT	( 8 )	// ����J�E���g
//#define MNGM_COUNT_MSK_COUNT	( 512 )	// ����J�E���g
static const VecFx32 sc_MNGM_COUNT_MSK_MAT[ MNGM_COUNT_MSK_OAM_NUM ] = {
	{ FX32_CONST( MNGM_COUNT_MSK_X ), FX32_CONST( MNGM_COUNT_MSK_Y ), 0 },
	{ FX32_CONST( MNGM_COUNT_MSK_X ), FX32_CONST( MNGM_COUNT_MSK_Y+MNGM_COUNT_MSK_Y_D ), 0 },
};

#define MNGM_COUNT_MSK_SY0		( MNGM_COUNT_MSK_Y - MNGM_COUNT_MSK_MY )
#define MNGM_COUNT_MSK_SY1		( (MNGM_COUNT_MSK_Y+MNGM_COUNT_MSK_Y_D) + MNGM_COUNT_MSK_MY )
enum{
	MNGM_COUNT_MSK_HBUF_READ,
	MNGM_COUNT_MSK_HBUF_WRITE,
	MNGM_COUNT_MSK_HBUF_NUM,
};



//-------------------------------------
///	�^�C�g�����S�V�X�e��
//=====================================
#define MNGM_TITLELOGO_BMP_X	(6)
#define MNGM_TITLELOGO_BMP_Y	(11)
#define MNGM_TITLELOGO_BMP_SIZX	(20)
#define MNGM_TITLELOGO_BMP_SIZY	(10)
#define MNGM_TITLELOGO_BMP_PAL	(8)
#define MNGM_TITLELOGO_BMP_CGX	(MNGM_ENTRY_BMP_CGX)
static const GF_PRINTCOLOR sc_MNGM_TITLELOGO_BMP_COL[ WFLBY_GAME_P2PGAME_NUM ] = {
	GF_PRINTCOLOR_MAKE( 7,8,9 ),
	GF_PRINTCOLOR_MAKE( 4,5,6 ),
	GF_PRINTCOLOR_MAKE( 1,2,3 ),
};

#define MNGM_TITLELOGO_BMP_SIZXDOT	(MNGM_TITLELOGO_BMP_SIZX*8)

// ����V�[�P���X
enum{
	MNGM_TITLELOGO_INMOVESEQ_IN,
	MNGM_TITLELOGO_INMOVESEQ_BOUND00,
	MNGM_TITLELOGO_INMOVESEQ_BOUND01,
	MNGM_TITLELOGO_INMOVESEQ_END,
};
enum{
	MNGM_TITLELOGO_OUTMOVESEQ_OUT,
	MNGM_TITLELOGO_OUTMOVESEQ_END,
};
// ����̒l		���D�����Y���W�ɔ��f	�ʂ���Ƌʏ��͉�
// ����O	�C��
static const fx32 sc_MNGM_TITLELOGO_MOVE00_START[ WFLBY_GAME_P2PGAME_NUM ] = {
	-FX32_CONST(200),
	FX32_CONST(200),
	FX32_CONST(160),
};
static const fx32 sc_MNGM_TITLELOGO_MOVE00_END[ WFLBY_GAME_P2PGAME_NUM ] = {
	FX32_CONST(0),
	FX32_CONST(0),
	FX32_CONST(0),
};
static const fx32 sc_MNGM_TITLELOGO_MOVE00_SPEED[ WFLBY_GAME_P2PGAME_NUM ] = {
	FX32_CONST(0),
	FX32_CONST(0),
	FX32_CONST(0),
};
#define MNGM_TITLELOGO_MOVE00_COUNTMAX	( 16 )

// ����P	�͂˂�O	�����ʒu�͓���O�̏I���ʒu
static const fx32 sc_MNGM_TITLELOGO_MOVE01_END[ WFLBY_GAME_P2PGAME_NUM ] = {
	-FX32_CONST(18),
	FX32_CONST(18),
	FX32_CONST(16),
};
static const fx32 sc_MNGM_TITLELOGO_MOVE01_SPEED[ WFLBY_GAME_P2PGAME_NUM ] = {
	-FX32_CONST(9),
	FX32_CONST(9),
	FX32_CONST(8),
};
#define MNGM_TITLELOGO_MOVE01_COUNTMAX	( 4 )

// ����Q	�͂˂�P	�����ʒu�͓���P�̏I���ʒu�@�@�I���ʒu�͓���O�̏I���l
static const fx32 sc_MNGM_TITLELOGO_MOVE02_SPEED[ WFLBY_GAME_P2PGAME_NUM ] = {
	FX32_CONST(0),
	FX32_CONST(0),
	FX32_CONST(0),
};
#define MNGM_TITLELOGO_MOVE02_COUNTMAX	( 4 )

// ����R	�A�E�g	�����ʒu�͓���O�̏I���ʒu
static const fx32 sc_MNGM_TITLELOGO_MOVE03_END[ WFLBY_GAME_P2PGAME_NUM ] = {
	FX32_CONST(200),
	-FX32_CONST(200),
	-FX32_CONST(160),
};
static const fx32 sc_MNGM_TITLELOGO_MOVE03_SPEED[ WFLBY_GAME_P2PGAME_NUM ] = {
	FX32_CONST(0),
	FX32_CONST(0),
	FX32_CONST(0),
};
#define MNGM_TITLELOGO_MOVE03_COUNTMAX	( 8 )




//-------------------------------------
///	HBLANKBG�X�N���[���V�X�e��
//=====================================
#define WFLBY_HBLANK_BGSCR_BUFNUM	(192)


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�ėp�V�X�e��
//=====================================

//	�ėp��������
typedef struct {
	fx32 x;
	fx32 s_x;
	fx32 s_s;	// �����x
	fx32 s_a;	// �����x
	int count_max;
} MNGM_ADDMOVE_WORK;



// �Z���A�N�^�[���\�[�X�Ǘ�
typedef struct {
	BOOL				data;		// �f�[�^�����邩
	CLACT_U_RES_OBJ_PTR	p_resobj[ MNGM_CLACTRES_RESMAN_NUM ];
	CLACT_HEADER		header;
} MNGM_CLACTRESOBJ;

typedef struct {
	CLACT_U_RES_MANAGER_PTR	p_resman[MNGM_CLACTRES_RESMAN_NUM];
	MNGM_CLACTRESOBJ*		p_obj;
	u32						objnum;
} MNGM_CLACTRES;

// �Z���A�N�^�[�Ǘ�
typedef struct{
    CLACT_SET_PTR           p_clactset;		// �Z���A�N�^�[�Z�b�g
    CLACT_U_EASYRENDER_DATA renddata;       // �ȈՃ����_�[�f�[�^
} MNGM_CLACT;

// BGL�Ǘ�
typedef struct{
	u32					frame;
	GF_BGL_BGCNT_HEADER	cnt;
} MNGM_BGL_DATA;
typedef struct {
	GF_BGL_INI*				p_bgl;	// bgl
	const MNGM_BGL_DATA*	cp_tbl;	// bgl�f�[�^�e�[�u�� 
	u32						tblnum;	// �f�[�^�e�[�u����
} MNGM_BGL;

//  ���b�Z�[�W�f�[�^�Ǘ�
typedef struct {
	MSGDATA_MANAGER*	p_msgman;
	WORDSET*			p_wordset;
	STRBUF*				p_str;
	STRBUF*				p_tmp;
} MNGM_MSG;

// �g�[�N�E�B���h�E�V�X�e��
typedef struct {
	GF_BGL_BMPWIN	win[MNGM_TALKWIN_IDX_NUM];
	u16				msg_no[MNGM_TALKWIN_IDX_NUM];
	STRBUF*			p_str[MNGM_TALKWIN_IDX_NUM];
	void*			p_timewait[MNGM_TALKWIN_IDX_NUM];
	BOOL			vip;
} MNGM_TALKWIN;

// �v���[�g�̃v���C���[�l���ɑΉ������z�u�ʒu�f�[�^
typedef struct {
	u8	top[WFLBY_MINIGAME_MAX];		// �\�����L�����N�^�ʒu
	u8	scrn_y[WFLBY_MINIGAME_MAX];		// �X�N���[���ǂݍ���Y�L�����N�^�ʒu
	u8	oam_anm[WFLBY_MINIGAME_MAX];	// OAM�̃A�j��
} MNGM_PLAYER_PLATE_DATA;

// �G���g���[�v���C���[���e�[�u���I�u�W�F
// ���̂����������肷�邾�낤���番���Ƃ�
typedef struct {
	GF_BGL_BMPWIN		win;
	CLACT_WORK_PTR		p_clwk;			// �A�N�^�[
	CLACT_WORK_PTR		p_rank;			// ���ʃA�N�^�[
	u16					playernum;		// ���[�U��
	u8					idx;			// ���[�U�̃e�[�u���C���f�b�N�X
	u8					plidx;			// �������猩�����̃��[�U�̃e�[�u���C���f�b�N�X

	MNGM_ADDMOVE_WORK	draw_x;			// �`�悘�ʒu
	s16					draw_x_count;	// ����counter
	s16					draw_x_wait;	// ����J�n�܂ł̃E�G�C�g
	s32					draw_x_snd;		// �炷��
} MNGM_PLATE_PLAYER;

// �ʐM�v���C���[�f�[�^
typedef struct{
	u8	nation[ WFLBY_MINIGAME_MAX ];	// ��ID
	u8	area[ WFLBY_MINIGAME_MAX ];		// �n��ID
	const MYSTATUS* cp_status[ WFLBY_MINIGAME_MAX ];	// ���ꂼ��̐l�̃f�[�^
} MNGM_COMM_PDATA;

// �^�C�g�����S
typedef struct {
	GF_BGL_BMPWIN		bmp;
	u32					gametype;
	STRBUF*				p_str;

	MNGM_ADDMOVE_WORK	move;			// ���샏�[�N
	s16					count;			// �J�E���^
	s16					seq;			// �V�[�P���X
} MNGM_TITLE_LOGO;


//-------------------------------------
///	HBLANKBG�X�N���[���V�X�e��
//=====================================
typedef struct {
	MNGM_BGL* p_bgl;
	s16 scrll_x[2][WFLBY_HBLANK_BGSCR_BUFNUM];	// buffer
	LASTER_SYS_PTR	p_laster;
} MNGM_HBLANK_PLATEBGSCR;


//-------------------------------------
///	�G���g���[��ʃ��[�N
//=====================================

typedef struct _MNGM_ENTRYWK{
	u8						seq;							// �V�[�P���X
	u8						wait;							// �E�G�C�g�J�E���^
	u16						heapID;							// �q�[�v
	u16						gametype;						// �Q�[���^�C�v
	u8						raregame_type;					// rare�Q�[���^�C�v
	u8						raregame_recv;					// rare�Q�[���^�C�v����M������
	MNGM_ENRES_PARAM		comm_param;						// ���ʃp�����[�^
	MNGM_COMM_PDATA			comm_pdata;						// �v���C���[�f�[�^
	MNGM_BGL				bgl;							// BG�V�X�e��
	MNGM_MSG				msg;							// ���b�Z�[�W�V�X�e��
	MNGM_TALKWIN			talkwin;						// ��b�E�B���h�E
	MNGM_CLACT				clact;							// �Z���A�N�^�[�V�X�e��
	MNGM_CLACTRES			clres;							// �Z���A�N�^�[���\�[�X
	MNGM_CLACTRESOBJ*		p_resobj;						// ���\�[�X�I�u�W�F
	MNGM_TITLE_LOGO			titlelogo;						// �^�C�g�����S
	void*					p_scrnbuf;						// �X�N���[���o�b�t�@
	NNSG2dScreenData*		p_scrn;							// �X�N���[���f�[�^
	MNGM_PLATE_PLAYER		playertbl[WFLBY_MINIGAME_MAX];	// �v���C���[�e�[�u��
	MNGM_HBLANK_PLATEBGSCR	bgscrl;							// BG�X�N���[���V�X�e��
	TCB_PTR					tcb;							// �^�X�N
	TCB_PTR					tcb_vwait;						// VBlank�^�X�N
}MNGM_ENTRYWK;


//-------------------------------------
///	���ʉ�ʃ��[�N
//=====================================
//-------------------------------------
///	3D�V�X�e�����[�N
//=====================================
typedef struct {
	GF_G3DMAN*	p_3dman;
	PTC_PTR		p_ptc;
	void*		p_ptc_work;
} MNGM_3DSYS;

//-------------------------------------
///	���g���C���[�N
//=====================================
typedef struct {
	u8				seq;	// �V�[�P���X
	u8				vchat;	// �{�C�X�`���b�g
	u16				wait;
	TOUCH_SW_SYS*	p_ts;	// �^�b�`�V�X�e��
	TOUCH_SW_PARAM	param;

	// ���g���C�e��M�p
	// ���g���C�q��M�p
	u8					recv_num;
	u8					recv_replay;
	u8					ko_recv;
	u8					ko_recv_replay;
} MNGM_RETRY_WK;

//-------------------------------------
///	���D�A�j�����[�N
//=====================================
typedef struct {
//	GF_BGL_BMPWIN		win;
	void*				p_scrnbuff;
	NNSG2dScreenData*	p_scrn;
	CLACT_WORK_PTR		p_clwk[MNGM_RESULT_BALLOON_CLACT_NUM];
	MNGM_CLACTRESOBJ*	p_resobj;
	u8					seq;
	u8					count;
	u8					idx;
	u8					num;
} MNGM_BALLOON_WK;


typedef struct _MNGM_RESULTWK{
	u16						wait;							// �E�G�C�g�J�E���^
	u16						heapID;							// �q�[�v
	u8						seq;							// �V�[�P���X
	u8						draw_nation;					// ������\������̂�
	u8						end_flag;						// �I���t���O
	u8						gametype;
	u8						replay;							// �ēx�V�Ԃ��t���O
	u8						gadget_update;					// �K�W�F�b�g�A�b�v�f�[�g�t���O
	u8						pad[2];
	MNGM_ENRES_PARAM		comm_param;						// ���ʃp�����[�^
	MNGM_RESULT_PARAM		param;							// �p�����[�^
	MNGM_COMM_PDATA			comm_pdata;						// �v���C���[�f�[�^
	MNGM_BGL				bgl;							// BG�V�X�e��
	MNGM_MSG				msg;							// ���b�Z�[�W�V�X�e��
	MNGM_TALKWIN			talkwin;						// ��b�E�B���h�E
	MNGM_CLACT				clact;							// �Z���A�N�^�[�V�X�e��
	MNGM_CLACTRES			clres;							// �Z���A�N�^�[���\�[�X
	MNGM_CLACTRESOBJ*		p_resobj;						// ���\�[�X�I�u�W�F
	MNGM_RETRY_WK			retrysys;						// ���g���C�V�X�e��
	MNGM_BALLOON_WK			balloon;						// ���D���[�N
	void*					p_scrnbuf[MNGM_RESULT_SCRN_NUM];// �X�N���[���o�b�t�@
	NNSG2dScreenData*		p_scrn[MNGM_RESULT_SCRN_NUM];	// �X�N���[���f�[�^
	MNGM_PLATE_PLAYER		playertbl[WFLBY_MINIGAME_MAX];	// �v���C���[�e�[�u��
	MNGM_HBLANK_PLATEBGSCR	bgscrl;							// BG�X�N���[���V�X�e��
	TCB_PTR					tcb;							// �^�X�N
	TCB_PTR					tcb_vwait;						// �^�X�N
	WFLBY_ITEMTYPE			last_gadget;					// �A�b�v�f�[�g�O�̃K�W�F�b�g

	// ���ʕ\���V�X�e���p
	u16						draw_result;					// ���\�����鏇��
	u16						draw_result_wait;				// �\�����鏇�ʂ܂ł̃E�G�C�g

	// �p���b�g�G�t�F�N�g�p
	void*					p_plttbuf;						// �p���b�g�o�b�t�@
	NNSG2dPaletteData*		p_pltt;							// �p���b�g�f�[�^
	u16						pltt_time;
	u16						pltt_start;

	// 1��ME�Đ�
	BOOL	top_me_play;	

}MNGM_RESULTWK;


//-------------------------------------
///	START	TIMEUP	���[�N
//=====================================
// OAM�}�X�N�V�X�e��
typedef struct {
	CLACT_WORK_PTR	p_msk[MNGM_COUNT_MSK_OAM_NUM];
	s16				count;
	s16				move_y;
	u8				mskon[MNGM_COUNT_MSK_HBUF_NUM][192];
	TCB_PTR			p_tcb;
	GXWndPlane		off_outplane;
	GXWndPlane		on_outplane;
	int				def_wnd1;
} MNGM_COUNT_MSKWK;

typedef struct _MNGM_COUNTWK{
	u32						heapID;		// heap
	u16						seq;		// �V�[�P���X
	s16						count;		// �J�E���g
	CLACT_SET_PTR			p_clset;	// �Z���A�N�^�[�Z�b�g
	ARCHANDLE*				p_handle;	// �n���h��
	MNGM_CLACTRES			resman;		// ���\�[�X�}�l�[�W��
	MNGM_CLACTRESOBJ*		p_resobj;	// ���\�[�X�I�u�W�F
	TCB_PTR					p_tcb;		// ����^�X�N

	CLACT_WORK_PTR			p_anm;		// �A�j��OAM
	MNGM_COUNT_MSKWK		msk;		// �}�X�N�V�X�e��
}MNGM_COUNTWK;






//-----------------------------------------------------------------------------
/**
 *				�f�[�^�֘A
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	Rare�Q�[���f�[�^
//=====================================
static const u8 sc_MNGM_RAREGAME_BUCKET_TBL[ MNGM_RAREGAME_BUCKET_NUM ] = {
	MNGM_RAREPAR_BUCKET_NORMAL, 
	MNGM_RAREPAR_BUCKET_REVERSE,
	MNGM_RAREPAR_BUCKET_BIG
};
static const u8 sc_MNGM_RAREGAME_BALANCEBALL_TBL[ MNGM_RAREGAME_BALANCEBALL_NUM ] = {
	MNGM_RAREPAR_BALANCEBALL_NORMAL, 
	MNGM_RAREPAR_BALANCEBALL_BIG,
};
static const u8 sc_MNGM_RAREGAME_BALLOON_TBL[ MNGM_RAREGAME_BALLOON_NUM ] = {
	MNGM_RAREPAR_BALLOON_NORMAL, 
	MNGM_RAREPAR_BALLOON_THICK,
	MNGM_RAREPAR_BALLOON_FINE,
};




//-------------------------------------
///	CLACT�f�[�^���[�N
//=====================================
static const CHAR_MANAGER_MAKE sc_MNGM_CHARMAN_INIT = {
	0,
	16*1024,	// 16K
	16*1024,	// 16K
	0
};


//-------------------------------------
///	�G���g���[��ʃ��[�N
//=====================================
// �o���N�ݒ�
static const GF_BGL_DISPVRAM sc_MNGM_ENTRY_BANK = {
	GX_VRAM_BG_256_AB,				// ���C��2D�G���W����BG
	GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
	GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
	GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
	GX_VRAM_OBJ_16_G,				// ���C��2D�G���W����OBJ
	GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
	GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
	GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
	GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
	GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
};
// BGL�ݒ�
static const GF_BGL_SYS_HEADER sc_MNGM_ENTRY_BGINIT = {
	GX_DISPMODE_GRAPHICS,
	GX_BGMODE_0,
	GX_BGMODE_0,
	GX_BG0_AS_2D
};
#define MNGM_ENTRY_BGL_USE	( 5 )
static const MNGM_BGL_DATA sc_MNGM_ENTRY_BGCNT[MNGM_ENTRY_BGL_USE] = {
	{	// �w�i
		GF_BGL_FRAME3_M,
		{
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			3, 0, 0, FALSE
		},
	},
	{	// �e�[�u��&�^�C�g�����S���b�Z�[�W��
		GF_BGL_FRAME1_M,
		{
			0, 0, 0x2000, 0, GF_BGL_SCRSIZ_512x512, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xc800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			1, 0, 0, FALSE
		},
	},
	{
		GF_BGL_FRAME2_M,
		{
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xc000, GX_BG_CHARBASE_0x18000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		},
	},
	{	// �^�C�g�����S&�e�[�u���p������
		GF_BGL_FRAME0_M,
		{
			0, 0, 0x2000, 0, GF_BGL_SCRSIZ_512x512, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xa000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
			2, 0, 0, FALSE
		},
	},

	// �T�u��
	{
		GF_BGL_FRAME0_S,
		{
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		},
	},
};


//-------------------------------------
///	����
//=====================================
// PLATE�\���ʒu�f�[�^
// �\���ʒu�ƃA�j��NO�͎����̈ʒu���玞�v���Ƀf�[�^���z�u����Ă��܂��B
//	1		2 1		 2
//					1 3
//	0		 0		 0
// �ǂݍ��ݐ�̓v���C���[�i���o�[�̏����Ƀf�[�^���z�u����Ă��܂��B
static const MNGM_PLAYER_PLATE_DATA sc_MNGM_PLAYER_PLATE_DATA[ WFLBY_MINIGAME_MAX ] = {
	{	// �Q��1�l
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
	},
	{	// �Q��2�l
		{14, 4, 0, 0},		// �\���ʒu�́A���[�U�̃v���C��ID�ŕω����܂��B
		{18, 0, 0, 0},		// �ǂݍ��ݐ�̓��[�U�̃v���C��ID�ŕω����܂���B
		{ 0, 3, 0, 0},		// �A�j��NO�̓��[�U�̃v���C��ID�ŕω����܂��B
	},
	{	// �Q��3�l
		{14, 8, 2, 0},
		{18, 0, 6, 0},
		{ 0, 2, 1, 0},
	},
	{	// �Q��4�l
		{18, 12, 0,  6,},
		{18,  0, 6, 12,},
		{ 0,  2, 3,  1,},
	},
};
// �v���C�l�����̋ʂ��ꕗ�犄�p�̃e�[�u���Q�ƃC���f�b�N�X
// ���̃C���f�b�N�X��sc_MNGM_PLAYER_PLATE_DATA�̊e�e�[�u�����Q�Ƃ���
static const u8	sc_MNGM_PLAYER_PLATE_BSBLIDX_DATA[ WFLBY_MINIGAME_MAX ][ WFLBY_MINIGAME_MAX ] = {
	// �Q��1�l
	{ 0,0,0,0 },
	// �Q��2�l
	{ 0,1,0,0 },
	// �Q��3�l
	{ 0,2,1,0 },
	// �Q��4�l
	{ 0,3,1,2 },
};

#if 0
// �v���C�l�����̋ʏ��p�̃e�[�u���Q�ƃC���f�b�N�X
// �����͏�ɂO�Ԗڂ̐���������
// ���̑��̓v���C���[ID�̏����ɂ��̃e�[�u��������
// ���̃C���f�b�N�X��sc_MNGM_PLAYER_PLATE_DATA�̊e�e�[�u�����Q�Ƃ���
static const u8	sc_MNGM_PLAYER_PLATE_BBIDX_DATA[ WFLBY_MINIGAME_MAX ][ WFLBY_MINIGAME_MAX ] = {
	// �Q��1�l
	{ 0,0,0,0 },
	// �Q��2�l
	{ 0,1,0,0 },
	// �Q��3�l
	{ 0,1,2,0 },
	// �Q��4�l
	{ 0,1,2,3 },
};
#endif



//-------------------------------------
///	���U���g��ʃ��[�N
//=====================================
// �o���N�ݒ�
static const GF_BGL_DISPVRAM sc_MNGM_RESULT_BANK = {
	GX_VRAM_BG_256_AB,				// ���C��2D�G���W����BG
	GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
	GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
	GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
	GX_VRAM_OBJ_16_G,				// ���C��2D�G���W����OBJ
	GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
	GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
	GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
	GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
	GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
};
// BGL�ݒ�
static const GF_BGL_SYS_HEADER sc_MNGM_RESULT_BGINIT = {
	GX_DISPMODE_GRAPHICS,
	GX_BGMODE_0,
	GX_BGMODE_0,
	GX_BG0_AS_2D
};
#define MNGM_RESULT_BGL_USE	( 6 )
static const MNGM_BGL_DATA sc_MNGM_RESULT_BGCNT[MNGM_RESULT_BGL_USE] = {
	{	// �w�i
		GF_BGL_FRAME3_M,
		{
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
			3, 0, 0, FALSE
		},
	},
	{	// �e�[�u��
		GF_BGL_FRAME1_M,
		{
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
			2, 0, 0, FALSE
		},
	},
	{	// ���b�Z�[�W�Q��
		GF_BGL_FRAME2_M,
		{
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		},
	},
	{	// �e�[�u���p���b�Z�[�W
		GF_BGL_FRAME0_M,
		{
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xc000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
			1, 0, 0, FALSE
		},
	},

	// �T�u��
	{
		GF_BGL_FRAME0_S,
		{
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		},
	},
	{
		GF_BGL_FRAME1_S,
		{
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		},
	},
};






//-----------------------------------------------------------------------------
/**
 *				���b�Z�[�W�e�[�u��
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�m�[�}���EVIP�@���b�Z�[�W�Ή��\
//=====================================
typedef struct {
	u16 normal;
	u16 vip;
} MNGM_MSG_DATA;
static const MNGM_MSG_DATA sc_MNGM_MSG_DATA[] = {
	{ msg_00, msg_00 },
	{ msg_01, msg_01 },
	{ msg_02, msg_02_2 },
	{ msg_03, msg_03 },
	{ msg_04, msg_04 },
	{ msg_05, msg_05 },
	{ msg_06, msg_06 },
	{ msg_07, msg_07 },
	{ msg_08, msg_08 },
	{ msg_09, msg_09 },
	{ msg_10, msg_10_2 },
	{ msg_11, msg_11 },
	{ msg_12, msg_12 },
	{ msg_13, msg_13 },
	{ msg_14, msg_14 },
	{ msg_15, msg_15 },
	{ msg_16, msg_16 },
	{ msg_17, msg_17_2 },
	{ msg_18, msg_18_2 },
	{ msg_19, msg_19 },
};





//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�ėp�V�X�e��
//=====================================
//	�ėp��������
static void MNGM_AddMoveReqFx( MNGM_ADDMOVE_WORK* p_wk, fx32 s_x, fx32 e_x, fx32 s_s, int count_max );
static BOOL	MNGM_AddMoveMainFx( MNGM_ADDMOVE_WORK* p_wk, s32 count );

// �X�N���[���L�����N�^�I�t�Z�b�g������������
static void MNGM_SCRN_AddCharOfs( NNSG2dScreenData* p_scrn, u32 char_offs );

// �Z���A�N�^�[���\�[�X�Ǘ�
static void MNGM_CLACTRES_Init( MNGM_CLACTRES* p_wk, u32 objnum, u32 heapID );
static void MNGM_CLACTRES_Exit( MNGM_CLACTRES* p_wk );
static MNGM_CLACTRESOBJ* MNGM_CLACTRES_Load( MNGM_CLACTRES* p_wk, ARCHANDLE* p_handle, u32 pal_idx, u32 palnum, u32 cg_idx, u32 cell_idx, u32 anm_idx, u32 contid, u32 heapID );
static CLACT_WORK_PTR MNGM_CLACTRES_Add( MNGM_CLACTRESOBJ* p_obj, CLACT_SET_PTR p_clset, s16 x, s16 y, u16 pri, u32 heapID );
static void MNGM_CLACTRES_OBJ_Init( MNGM_CLACTRES* p_wk, MNGM_CLACTRESOBJ* p_obj, ARCHANDLE* p_handle, u32 pal_idx, u32 palnum, u32 cg_idx, u32 cell_idx, u32 anm_idx, u32 contid, u32 heapID );
static void MNGM_CLACTRES_OBJ_Exit( MNGM_CLACTRES* p_wk, MNGM_CLACTRESOBJ* p_obj );

// BGL�Ǘ�
static void MNGM_BGL_Init( MNGM_BGL* p_wk, const GF_BGL_SYS_HEADER* cp_sys, const MNGM_BGL_DATA* cp_cnt, u32 cnt_num, u32 heapID );
static void MNGM_BGL_Exit( MNGM_BGL* p_wk );
static void MNGM_BGL_VBlank( MNGM_BGL* p_wk );

// CLACT�Ǘ�
static void MNGM_CLACT_Init( MNGM_CLACT* p_wk, u32 objnum, u32 charnum, u32 plttnum, u32 heapID );
static void MNGM_CLACT_Exit( MNGM_CLACT* p_wk );
static void MNGM_CLACT_Draw( MNGM_CLACT* p_wk );
static void MNGM_CLACT_VBlank( MNGM_CLACT* p_wk );

//  ���b�Z�[�W�Ǘ�
static void MNGM_MSG_Init( MNGM_MSG* p_wk, u32 heapID );
static void MNGM_MSG_Exit( MNGM_MSG* p_wk );
static void MNGM_MSG_ClearStrBuff( MNGM_MSG* p_wk );
static void MNGM_MSG_SetPlayerName( MNGM_MSG* p_wk, const MYSTATUS* cp_player );
static void MNGM_MSG_SetNationName( MNGM_MSG* p_wk, u32 nation );
static void MNGM_MSG_SetAreaName( MNGM_MSG* p_wk, u32 nation, u32 area );
static void MNGM_MSG_SetScore( MNGM_MSG* p_wk, u32 number, u32 keta );
static void MNGM_MSG_SetBalloonNum( MNGM_MSG* p_wk, u32 number );
static void MNGM_MSG_SetTime( MNGM_MSG* p_wk, u32 time );
static void MNGM_MSG_SetGadget( MNGM_MSG* p_wk, u32 gadget );
static void MNGM_MSG_SetGameName( MNGM_MSG* p_wk, u32 game );
static void MNGM_MSG_GetStr( MNGM_MSG* p_wk, STRBUF* p_str, u32 msgidx );
static void MNGM_MSG_Print( MNGM_MSG* p_wk, u32 no, GF_BGL_BMPWIN* p_win, u8 x, u8 y );
static void MNGM_MSG_PrintRightSide( MNGM_MSG* p_wk, u32 no, GF_BGL_BMPWIN* p_win, u8 x, u8 y );
static u32 MNGM_MSG_PrintScr( MNGM_MSG* p_wk, u32 no, GF_BGL_BMPWIN* p_win, STRBUF* p_str, u32 wait );
static void MNGM_MSG_PrintColor( MNGM_MSG* p_wk, u32 no, GF_BGL_BMPWIN* p_win, u8 x, u8 y, GF_PRINTCOLOR col );

// ��b�E�B���h�E
static void MNGM_TALKWIN_Init( MNGM_TALKWIN* p_wk, MNGM_BGL* p_bgl, SAVEDATA* p_save, BOOL vip, u32 heapID );
static void MNGM_TALKWIN_Exit( MNGM_TALKWIN* p_wk );
static void MNGM_TALKWIN_MsgPrint( MNGM_TALKWIN* p_wk, MNGM_MSG* p_msg, u32 msgidx, u32 idx );
static void MNGM_TALKWIN_MsgOff( MNGM_TALKWIN* p_wk, u32 idx );
static BOOL MNGM_TALKWIN_MsgEndCheck( const MNGM_TALKWIN* p_wk, u32 idx );
static void MNGM_TALKWIN_SetPos( MNGM_TALKWIN* p_wk, u8 x, u8 y, u32 idx );
static void MNGM_TALKWIN_SetTimeWork( MNGM_TALKWIN* p_wk, u32 idx );
static void MNGM_TALKWIN_CleanTimeWork( MNGM_TALKWIN* p_wk, u32 idx );

// �v���C���[PLATE
static void MNGM_PLATE_PLAYERTBL_Init( MNGM_PLATE_PLAYER* p_player, u32 player_num, u32 gametype, NNSG2dScreenData* p_scrn, MNGM_BGL* p_bgl, u32 idx, u32 myplayerid, MNGM_CLACT* p_clact, MNGM_CLACTRESOBJ* p_oamres, u32 heapID );
static void MNGM_PLATE_PLAYERTBL_DrawName( MNGM_PLATE_PLAYER* p_player, MNGM_MSG* p_msg, const MYSTATUS* cp_status, s16 x, s16 y, u32 vip );
static void MNGM_PLATE_PLAYERTBL_DrawNation( MNGM_PLATE_PLAYER* p_player, MNGM_MSG* p_msg, BOOL draw_nation, u32 nation, u32 area, s16 x );
static void MNGM_PLATE_PLAYERTBL_DrawRank( MNGM_PLATE_PLAYER* p_player, u32 rank );
static void MNGM_PLATE_PLAYERTBL_DrawScore( MNGM_PLATE_PLAYER* p_player, MNGM_MSG* p_msg, u32 score, s16 x, s16 y, u32 keta );
static void MNGM_PLATE_PLAYERTBL_DrawTime( MNGM_PLATE_PLAYER* p_player, MNGM_MSG* p_msg, u32 frame, s16 x, s16 y );
static void MNGM_PLATE_PLAYERTBL_BgWriteVReq( MNGM_PLATE_PLAYER* p_player, MNGM_BGL* p_bgl );
static void MNGM_PLATE_PLAYERTBL_DrawOamWay( MNGM_PLATE_PLAYER* p_player );
static void MNGM_PLATE_PLAYERTBL_Delete( MNGM_PLATE_PLAYER* p_player );
static void MNGM_PLATE_PLAYERTBL_StartMove( MNGM_PLATE_PLAYER* p_player, s16 end_x, s16 wait );
static void MNGM_PLATE_PLAYERTBL_StartMoveRet( MNGM_PLATE_PLAYER* p_player, s16 wait );
static BOOL MNGM_PLATE_PLAYERTBL_MainMove( MNGM_PLATE_PLAYER* p_player );
static void MNGM_PLATE_PLAYERTBL_SetOamMatrix( MNGM_PLATE_PLAYER* p_wk, s32 draw_x );

// �v���C���[PLATE
static u32 MNGM_PLAYER_PLATE_BSBLIDX_Get( u32 playernum, u32 playerid, u32 myplayerid );


// HBLANKBG�X�N���[���V�X�e��
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_Init( MNGM_HBLANK_PLATEBGSCR* p_wk, MNGM_BGL* p_bgl, u32 heapID );
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_Exit( MNGM_HBLANK_PLATEBGSCR* p_wk );
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_SetPlateScr( MNGM_HBLANK_PLATEBGSCR* p_wk, const MNGM_PLATE_PLAYER* cp_plate );
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_VBlank( MNGM_HBLANK_PLATEBGSCR* p_wk );
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_HBlank( void* p_work );


// �v���C���[�ʐM�f�[�^
static void MNGM_COMMPDATA_Init( MNGM_COMM_PDATA* p_wk, const MNGM_ENRES_PARAM* cp_comm_param );

// ������\������̂��A�n�於��\������̂��̃`�F�b�N
static BOOL MNGM_COMMPARAM_CheckDrawNation( const MNGM_ENRES_PARAM* cp_wk, const MNGM_COMM_PDATA* cp_data );

// �K�W�F�b�g�̃A�b�v�f�[�g
extern WFLBY_ITEMTYPE MNGM_ITEM_Update( WFLBY_ITEMTYPE data );

// �^�C�g�����S�V�X�e��
static void MNGM_TITLELOGO_Init( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk, MNGM_MSG* p_msg, u32 gametype, ARCHANDLE* p_handle, u32 heapID );
static void MNGM_TITLELOGO_Exit( MNGM_TITLE_LOGO* p_wk );
static void MNGM_TITLELOGO_InStart( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk, u32 heapID );
static void MNGM_TITLELOGO_OutStart( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk );
static BOOL MNGM_TITLELOGO_InMain( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk );
static BOOL MNGM_TITLELOGO_OutMain( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk );
static void MNGM_TITLELOGO_SetMoveMatrix( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk, s32 num );
static void MNGM_TITLELOGO_SetMoveMatrixVReq( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk, s32 num );


// �w�i�p���b�g�]��
static void MNGM_BACKPLTT_Trans( ARCHANDLE* p_handle, u32 gametype, u32 heapID );

//-------------------------------------
///	�G���g���[��ʃ��[�N
//=====================================
static MNGM_ENTRYWK* MNGM_ENTRY_CommonInit( const MNGM_ENRES_PARAM* cp_commparam, u32 gametype, u32 heapID );
static void MNGM_ENTRY_GraphicLoad( MNGM_ENTRYWK* p_wk, u32 heapID );
static void MNGM_ENTRY_GraphicDelete( MNGM_ENTRYWK* p_wk );
static void MNGM_ENTRY_Tcb( TCB_PTR tcb, void* p_work );
static void MNGM_ENTRY_VWaitTcb( TCB_PTR tcb, void* p_work );
static MNGM_RAREGAME_TYPE MNGM_ENTRY_RareGameSelect( const u8* cp_ParTbl, u32 num );
static BOOL MNGM_ENTRY_PLATE_ALLMove( MNGM_ENTRYWK* p_wk );


//-------------------------------------
///	���U���g��ʃ��[�N
//=====================================
static MNGM_RESULTWK* MNGM_RESULT_CommonInit( const MNGM_ENRES_PARAM* cp_commparam, const MNGM_RESULT_PARAM* cp_param, u32 gametype, u32 heapID );
static void MNGM_RESULT_3DInit( MNGM_3DSYS* p_wk, u32 heapID );
static void MNGM_RESULT_3DExit( MNGM_3DSYS* p_wk );
static void MNGM_RESULT_3DMain( MNGM_3DSYS* p_wk );
static void MNGM_RESULT_3DDraw( MNGM_3DSYS* p_wk );
static void MNGM_RESULT_3DAnmLoad( MNGM_3DSYS* p_wk, ARCHANDLE* p_handle, u32 dataidx, u32 heapID );
static void MNGM_RESULT_3DAnmRelease( MNGM_3DSYS* p_wk );
static void MNGM_RESULT_3DAnmStart( MNGM_3DSYS* p_wk, u32 emitnum );
static BOOL MNGM_RESULT_3DAnmEndCheck( const MNGM_3DSYS* cp_wk );
static void MNGM_RESULT_GraphicLoad( MNGM_RESULTWK* p_wk, u32 heapID );
static void MNGM_RESULT_GraphicDelete( MNGM_RESULTWK* p_wk );
static void MNGM_RESULT_Tcb_BallSlowBalanceBall( TCB_PTR tcb, void* p_work );
static void MNGM_RESULT_Tcb_Balloon( TCB_PTR tcb, void* p_work );
static void MNGM_RESULT_VWaitTcb( TCB_PTR tcb, void* p_work );
static void MNGM_RESULT_PalTrEffectInit( MNGM_RESULTWK* p_wk, ARCHANDLE* p_handle, u32 heapID );
static void MNGM_RESULT_PalTrEffectStart( MNGM_RESULTWK* p_wk );
static void MNGM_RESULT_PalTrEffect( MNGM_RESULTWK* p_wk );
static void MNGM_RESULT_PalTrEffectExit( MNGM_RESULTWK* p_wk );
static void MNGM_RESULT_Retry_Init( MNGM_RETRY_WK* p_wk, MNGM_BGL* p_bgl, u32 vchat, u32 heapID );
static void MNGM_RESULT_Retry_Exit( MNGM_RETRY_WK* p_wk );
static BOOL MNGM_RESULT_Retry_Main( MNGM_RETRY_WK* p_wk, MNGM_TALKWIN* p_talkwin, MNGM_MSG* p_msg, u32 gametype, u32 heapID );
static BOOL MNGM_RESULT_RetryGet( const MNGM_RETRY_WK* cp_wk );
static void MNGM_RESULT_RetryOyaRecv( MNGM_RETRY_WK* p_wk, u32 netid, BOOL replay, u32 playernum );
static void MNGM_RESULT_RetryKoRecv( MNGM_RETRY_WK* p_wk, BOOL replay );
static void MNGM_RESULT_Balloon_GraphicInit( MNGM_BALLOON_WK* p_wk, MNGM_BGL* p_bgl, MNGM_CLACT* p_clact, MNGM_CLACTRES* p_clres, ARCHANDLE* p_handle, u32 heapID );
static void MNGM_RESULT_Balloon_GraphicExit( MNGM_BALLOON_WK* p_wk );
static void MNGM_RESULT_Balloon_Start( MNGM_BALLOON_WK* p_wk, u32 num );
static BOOL MNGM_RESULT_Balloon_Main( MNGM_BALLOON_WK* p_wk, MNGM_BGL* p_bgl );

static void MNGM_RESULT_BallslowBalancePlayerTblDraw( MNGM_RESULTWK* p_wk, u32 plidx, u32 rank );
static u32 MNGM_RESULT_BallslowBalanceGetNoTouchScore( const MNGM_RESULTWK* cp_wk );
static void MNGM_RESULT_BallslowBalance_SendMinigameTopResult( MNGM_RESULTWK* p_wk );
static void MNGM_RESULT_Balloon_SendMinigameTopResult( MNGM_RESULTWK* p_wk );

static BOOL MNGM_RESULT_PlayerTblMove( MNGM_RESULTWK* p_wk );

static void MNGM_RESULT_SndTopMePlay( MNGM_RESULTWK* p_wk );
static BOOL MNGM_RESULT_SndTopMeEndWait( MNGM_RESULTWK* p_wk );
static void MNGM_RESULT_SetPlayNum( MNGM_RESULTWK* p_wk );




//-------------------------------------
///	START	TIMEUP	���[�N
//=====================================
static void MNGM_COUNT_MskInit( MNGM_COUNT_MSKWK* p_wk, MNGM_CLACTRESOBJ* p_resobj, CLACT_SET_PTR p_clset, u32 heapID );
static BOOL MNGM_COUNT_MskMain( MNGM_COUNT_MSKWK* p_wk );
static BOOL MNGM_COUNT_MskReMain( MNGM_COUNT_MSKWK* p_wk );
static void MNGM_COUNT_MskCommon( MNGM_COUNT_MSKWK* p_wk, s32 add );
static void MNGM_COUNT_MskExit( MNGM_COUNT_MSKWK* p_wk );
static void MNGM_COUNT_MskSetMsk( MNGM_COUNT_MSKWK* p_wk );
static void MNGM_COUNT_MskSetMskVBlank( TCB_PTR tcb, void* p_work );
static void MNGM_COUNT_MskSetMskHBlank( void* p_work );

static void MNGM_COUNT_StartTcb( TCB_PTR tcb, void* p_work );
static void MNGM_COUNT_TimeUpTcb( TCB_PTR tcb, void* p_work );



//-------------------------------------
///	�G���g���[��ʁA���ʉ�ʋ��ʃp�����[�^�c�[��
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[��ʁA���ʉ�ʋ��ʃp�����[�^�쐬
 *
 *	@param	p_param		�p�����[�^�i�[��
 *	@param	lobby_flag	���r�[�t���O
 *	@param	p_save		�Z�[�u�f�[�^
 *	@param	vchat		�{�C�X�`���b�g
 *	@param	p_lobby_wk	���r�[���[�N
 */
//-----------------------------------------------------------------------------
void MNGM_ENRES_PARAM_Init( MNGM_ENRES_PARAM* p_param, BOOL lobby_flag, SAVEDATA* p_save, BOOL vchat, WFLBY_MINIGAME_WK* p_lobby_wk )
{
	int i;
	int count;
	u32 netid;
	MYSTATUS* p_status;

	memset( p_param,  0, sizeof(MNGM_ENRES_PARAM) );
	
	p_param->num = CommInfoGetEntryNum();
	netid = CommGetCurrentID();

	// PLNO����NETID�e�[�u�����쐬
	count = 0;
	for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
		p_status = CommInfoGetMyStatus( i );
		if( p_status != NULL ){
			
			// ������NETID�Ȃ�PLNO��ۑ�
			if( netid == i ){
				p_param->my_playerid = count;
			}
			
			// plno�̂Ƃ����NETID��ۑ�
			p_param->pnetid[count] = i;
			count ++;
		}
	}
	// CommGetConnectNum�̖߂�l�Ǝ��ۂ�MYSTATUS�̎�M���ɖ���������
	GF_ASSERT( count == p_param->num );

	// replay���⏈���ɂ���
	if( lobby_flag == FALSE ){
		p_param->replay = TRUE;
	}

	p_param->p_save	= p_save;
	p_param->vchat	= vchat;

	// VIP�t���O���R�s�[
	p_param->p_lobby_wk = p_lobby_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	NETID����PLNO���擾����
 *
 *	@param	cp_param	�G���g���[��ʁA���ʉ�ʋ��ʃp�����[�^
 *	@param	netid		�l�b�gID
 *
 *	@retval	PLNO	�i�v���C���[�i���o�[�j
 *	@retval	���Ă͂܂�Ȃ�NETID�Ȃ�WFLBY_MINIGAME_MAX��Ԃ�
 */
//-----------------------------------------------------------------------------
u32 MNGM_ENRES_PARAM_GetNetIDtoPlNO( const MNGM_ENRES_PARAM* cp_param, u32 netid )
{
	int i;

	for( i=0; i<cp_param->num; i++ ){
		if( cp_param->pnetid[i] == netid ){
			return i;
		}
	}
	return WFLBY_MINIGAME_MAX;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[NO����NETID���擾����
 *
 *	@param	cp_param	�G���g���[��ʁA���ʉ�ʋ��ʃp�����[�^
 *	@param	plno		�v���C���[�i���o�[
 *
 *	@return	NETID
 */
//-----------------------------------------------------------------------------
u32 MNGM_ENRES_PARAM_GetPlNOtoNetID( const MNGM_ENRES_PARAM* cp_param, u32 plno )
{
	GF_ASSERT( plno < cp_param->num );

	return cp_param->pnetid[ plno ];
}


//----------------------------------------------------------------------------
/**
 *	@brief	����NETID�̐l��VIP���ǂ����`�F�b�N����
 *
 *	@param	cp_param		�p�����[�^
 *	@param	netid			�l�b�gID
 *
 *	@retval	TRUE	VIP
 *	@retval	FALSE	VIP����Ȃ�
 *
 *	VIP�̎��́A���̐l�̖��O��F�ɂ��Ă�������
 */
//-----------------------------------------------------------------------------
BOOL MNGM_ENRES_PARAM_GetVipFlag( const MNGM_ENRES_PARAM* cp_param, u32 netid )
{
	u32 plidx;
	BOOL vip;
	
	if( netid >= WFLBY_MINIGAME_MAX ){
		GF_ASSERT( netid < WFLBY_MINIGAME_MAX );
		return FALSE;
	}
	
	plidx = cp_param->p_lobby_wk->plidx.plidx[ netid ];
	if( plidx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
		return FALSE;
	}
	
	vip =  cp_param->p_lobby_wk->vipflag.vip[ plidx ];

//	OS_TPrintf( "plidx %d,  vip %d\n", plidx, vip );
	return vip;
}

//----------------------------------------------------------------------------
/**
 *	@brief	MY�X�e�[�^�X���擾
 *
 *	@param	cp_param		�p�����[�^
 *	@param	netid			�l�b�gID
 *
 *	@retval	MY�X�e�[�^�X
 *	@retval	NULL			���̂ЂƂ��Ȃ�
*/
//-----------------------------------------------------------------------------
MYSTATUS* MNGM_ENRES_PARAM_GetMystatus( const MNGM_ENRES_PARAM* cp_param, u32 netid )
{
	u32 plno;

	if( netid >= WFLBY_MINIGAME_MAX ){
		GF_ASSERT( netid < WFLBY_MINIGAME_MAX );
		return NULL;
	}

	// Wi-Fi�N���u�̂Ƃ�
	if( cp_param->replay == TRUE ){
		plno		= MNGM_ENRES_PARAM_GetNetIDtoPlNO( cp_param, netid );

		// �s��NAME�Ŗ��O���u������Ă���\��������̂�
		// �����̖��O�̓Z�[�u�f�[�^����Ƃ��Ă���
		if( plno == cp_param->my_playerid ){
			return SaveData_GetMyStatus( cp_param->p_save );
		}
		return CommInfoGetMyStatus( netid );
	}

	// Wi-Fi�L��̂Ƃ�
	GF_ASSERT( cp_param->p_lobby_wk != NULL );
	return cp_param->p_lobby_wk->p_mystate[netid];
}

 

//-------------------------------------
///	�G���g���[��ʃ��[�N
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[���[�N�쐬
 *
 *	@param	cp_param		�p�����[�^
 *	@param	heapID 
 *
 *	@return	�G���g���[���[�N
 */
//-----------------------------------------------------------------------------
// �ʓ���
MNGM_ENTRYWK* MNGM_ENTRY_InitBallSlow( const MNGM_ENRES_PARAM* cp_commparam, u32 heapID )
{
	return MNGM_ENTRY_CommonInit( cp_commparam, WFLBY_GAME_BALLSLOW, heapID );
}

// Balance�{�[��
MNGM_ENTRYWK* MNGM_ENTRY_InitBalanceBall( const MNGM_ENRES_PARAM* cp_commparam, u32 heapID )
{
	return MNGM_ENTRY_CommonInit( cp_commparam, WFLBY_GAME_BALANCEBALL, heapID );
}

// �ӂ�������
MNGM_ENTRYWK* MNGM_ENTRY_InitBalloon( const MNGM_ENRES_PARAM* cp_commparam, u32 heapID )
{
	return MNGM_ENTRY_CommonInit( cp_commparam, WFLBY_GAME_BALLOON, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[��ʔj��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void MNGM_ENTRY_Exit( MNGM_ENTRYWK* p_wk )
{
	
	// TCB�j��
	TCB_Delete( p_wk->tcb );
	TCB_Delete( p_wk->tcb_vwait );

	// ���X�^�[�V�X�e���j��
	MNGM_PLAYER_PLATE_HBLANK_BGSCR_Exit( &p_wk->bgscrl );

	// �^�C�g�����S���[�N�j��
	MNGM_TITLELOGO_Exit( &p_wk->titlelogo );

	
	// �v���C���[���̃f�[�^�j��
	{
		int i;
		for( i=0; i<p_wk->comm_param.num; i++ ){
			MNGM_PLATE_PLAYERTBL_Delete( &p_wk->playertbl[i] );
		}
	}

	// ���b�Z�[�W�V�X�e���j��
	MNGM_TALKWIN_Exit( &p_wk->talkwin );
	
	// �O���t�B�b�N�j��
	MNGM_ENTRY_GraphicDelete( p_wk );

	// ���b�Z�[�W�V�X�e���j��
	MNGM_MSG_Exit( &p_wk->msg );	

	// �O���t�B�b�N�ݒ����
	MNGM_CLACTRES_Exit( &p_wk->clres );
	MNGM_CLACT_Exit( &p_wk->clact );
	MNGM_BGL_Exit( &p_wk->bgl );

	// ���[�N�j��
	sys_FreeMemoryEz( p_wk );	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[��ʏI���҂�
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL MNGM_ENTRY_Wait( const MNGM_ENTRYWK* cp_wk )
{
	if( cp_wk->seq >= MNGM_ENTRY_SEQ_END ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	rare�Q�[���^�C�v�̎擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	rare�Q�[���^�C�v
 */
//-----------------------------------------------------------------------------
MNGM_RAREGAME_TYPE MNGM_ENTRY_GetRareGame( const MNGM_ENTRYWK* cp_wk )
{
	GF_ASSERT( cp_wk->raregame_recv == TRUE );
	return cp_wk->raregame_type;
}


//-------------------------------------
///	���ʉ�ʃ��[�N
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	���ʉ�ʃ��[�N	�쐬
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
// �{�[������
MNGM_RESULTWK* MNGM_RESULT_InitBallSlow( const MNGM_ENRES_PARAM* cp_commparam, const MNGM_RESULT_PARAM* cp_param, u32 heapID )
{
	return MNGM_RESULT_CommonInit( cp_commparam, cp_param, WFLBY_GAME_BALLSLOW, heapID );
}

// Balance�{�[��
MNGM_RESULTWK* MNGM_RESULT_InitBalanceBall( const MNGM_ENRES_PARAM* cp_commparam, const MNGM_RESULT_PARAM* cp_param, u32 heapID )
{
	return MNGM_RESULT_CommonInit( cp_commparam, cp_param, WFLBY_GAME_BALANCEBALL, heapID );
}

// ���犄
MNGM_RESULTWK* MNGM_RESULT_InitBalloon( const MNGM_ENRES_PARAM* cp_commparam, const MNGM_RESULT_PARAM* cp_param, u32 heapID )
{
	return MNGM_RESULT_CommonInit( cp_commparam, cp_param, WFLBY_GAME_BALLOON, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ʉ�ʔj��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void MNGM_RESULT_Exit( MNGM_RESULTWK* p_wk )
{
	

	// TCB�j��
	TCB_Delete( p_wk->tcb );
	TCB_Delete( p_wk->tcb_vwait );

	// ���X�^�[�V�X�e���j��
	MNGM_PLAYER_PLATE_HBLANK_BGSCR_Exit( &p_wk->bgscrl );
	
	// �v���C���[���̃f�[�^�j��
	{
		int i;
		for( i=0; i<p_wk->comm_param.num; i++ ){
			MNGM_PLATE_PLAYERTBL_Delete( &p_wk->playertbl[i] );
		}
	}

	// ���g���C���[�N�̔j��
	MNGM_RESULT_Retry_Exit( &p_wk->retrysys );
	
	// �O���t�B�b�N�j��
	MNGM_RESULT_GraphicDelete( p_wk );

	// ���b�Z�[�W�V�X�e���j��
	MNGM_TALKWIN_Exit( &p_wk->talkwin );

	// ���b�Z�[�W�V�X�e���j��
	MNGM_MSG_Exit( &p_wk->msg );	

	// �O���t�B�b�N�ݒ����
	MNGM_CLACTRES_Exit( &p_wk->clres );
	MNGM_CLACT_Exit( &p_wk->clact );
	MNGM_BGL_Exit( &p_wk->bgl );

	//  �A���t�@OFF
	G2_BlendNone();
	G2S_BlendNone();

	//
	DellVramTransferManager();


	// ���[�N�j��
	sys_FreeMemoryEz( p_wk );	
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ʉ��	�����I���`�F�b�N�֐�
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL MNGM_RESULT_Wait( const MNGM_RESULTWK* cp_wk )
{
	return cp_wk->end_flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ēx�V�Ԃ̂��`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�ēx�V��
 *	@retval	FALSE	�����V�΂Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL MNGM_RESULT_GetReplay( const MNGM_RESULTWK* cp_wk )
{
	return cp_wk->replay;
}


// ���ʉ�ʃp�����[�^�����c�[��
//----------------------------------------------------------------------------
/**
 *	@brief	���ʉ�ʃp�����[�^Rank�����c�[��
 *
 *	@param	p_param		�p�����[�^
 *	@param	plnum		�v���C���[��
 */
//-----------------------------------------------------------------------------
void MNGM_RESULT_CalcRank( MNGM_RESULT_PARAM* p_param, u32 plnum )
{
	u8 rankplnotbl[ WFLBY_MINIGAME_MAX ];	// ���ʂ̏���PLNO���i�[����
	int i, j;
	
	for( i=0; i<plnum; i++ ){
		
		// �����̃f�[�^�i�[������߂�
		for( j=i; j>0; j-- ){

			// j�̈ʒu��PLNO�̃X�R�A�Ǝ����̃X�R�A���r��
			// ���������炻�̎���PLNO�̃X�R�A�Ǝ����̃X�R�A���r����
			if( p_param->score[ i ] <= p_param->score[ rankplnotbl[ j-1 ] ] ){
				break;
			}else{
				rankplnotbl[ j ] = rankplnotbl[ j-1 ];
			}
		}

		// j�̈ʒu�Ɏ�����PLNO������
		rankplnotbl[ j ] = i;
	}

	// ���߂��ɕ��т�\��
	{
		for( i=0; i<plnum; i++ ){
			OS_TPrintf( "%d	", p_param->score[ rankplnotbl[ i ] ] );
		}
		OS_TPrintf( "\n" );
	}

	// �X�R�A�̑������ɕ���PLNOTBL���g�p���ď��ʂ��i�[���Ă���
	{
		u32 rank;
		for( i=0; i<plnum; i++ ){
			rank = i;	// ��{�̏��ʂŏ�����
			
			if( i>0 ){
				// �O�̐l�Ɠ����X�R�A�Ȃ瓯������
				if( p_param->score[ rankplnotbl[ i ] ] == p_param->score[ rankplnotbl[ i-1 ] ] ){
					rank = p_param->result[ rankplnotbl[ i-1 ] ];
				}
			}

			p_param->result[ rankplnotbl[ i ] ] = rank;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	Rare�Q�[���^�C�v��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	type		�^�C�v
 */
//-----------------------------------------------------------------------------
void MNGM_ENTRY_SetRareGame( MNGM_ENTRYWK* p_wk,  MNGM_RAREGAME_TYPE type )
{
	p_wk->raregame_type = type;
	p_wk->raregame_recv	= TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q���炫�������邩�f�[�^����M
 *
 *	@param	p_wk		���[�N
 *	@param	netid		�l�b�gID
 *	@param	retry		replay���ǂ���
 */
//-----------------------------------------------------------------------------
void MNGM_RESULT_SetKoRetry( MNGM_RESULTWK* p_wk, u32 netid,  BOOL retry )
{
	if( p_wk->comm_param.my_playerid == 0 ){
		MNGM_RESULT_RetryOyaRecv( &p_wk->retrysys, netid, retry, p_wk->comm_param.num );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e���瑱���邩�f�[�^����M
 *
 *	@param	p_wk		���[�N
 *	@param	retry		���g���C
 */
//-----------------------------------------------------------------------------
void MNGM_RESULT_SetOyaRetry( MNGM_RESULTWK* p_wk, BOOL retry )
{
	MNGM_RESULT_RetryKoRecv( &p_wk->retrysys, retry );
}



//-------------------------------------
///	�ؒf�G���[�`�F�b�N
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�ؒf�G���[�`�F�b�N
 *
 *	@retval	TRUE	�ؒf�G���[��������
 *	@retval	FALSE	�ؒf�G���[�Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL MNGM_ERROR_CheckDisconnect( MNGM_ENRES_PARAM* p_commparam )
{
	BOOL ret = FALSE;
	
	if( p_commparam->p_lobby_wk != NULL ){
		
#ifdef DEBUG_MINIGAME_DISCONNECT
		if( sys.cont & PAD_BUTTON_A ){
			ret = TRUE;
		}
#endif

		// �ʐM�l�����ς������G���[
		if( p_commparam->num != CommGetConnectNum() ){
			ret = TRUE;
		}

		//  �ؒf�G���[
		if( CommStateIsWifiDisconnect() == TRUE ){
			ret = TRUE;
		}

		// �}�b�`���O�G���[
		if( CommWifiIsMatched() >= 2 ){
			ret = TRUE;
		}

		// 
		if( ret == TRUE ){
			p_commparam->p_lobby_wk->error_end = TRUE;
		}

		//  �G���[�I����ԂȂ�TRUE��Ԃ�������
		if( p_commparam->p_lobby_wk->error_end == TRUE ){
			ret = TRUE;
		}
	}
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ؒf����
 *
 *	@param	p_commparam		���[�N
 *
 *	@retval	TRUE	�ؒf����
 *	@retval	FALSE	�ؒf��
 */
//-----------------------------------------------------------------------------
BOOL MNGM_ERROR_DisconnectWait( const MNGM_ENRES_PARAM* cp_commparam )
{
	GF_ASSERT( cp_commparam->p_lobby_wk != NULL );
	GF_ASSERT( cp_commparam->p_lobby_wk->error_end == TRUE );
	
	if( CommInfoIsInitialize() == TRUE ){

		// 080624	�߂�Ƃ��ɃR�}���h�������Ă���\��������̂ŁA
		// �����Ŋ��S�ɐؒf������
		// �ʐM���I��������
		// Info�I��
		CommInfoFinalize();

		// �ʐM�ؒf
		CommStateWifiP2PEnd();
	}else{

		// �ؒf����
		if( CommStateIsWifiLoginMatchState() == TRUE ){
			return TRUE;
		}
	}

	return FALSE;
}



//-------------------------------------
///	START	TIMEUP	���[�N
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	OAM�G�t�F�N�g���[�N	���쐬����
 *
 *	@param	p_clset		�Z���A�N�^�[�Z�b�g
 *	@param	heapID		�q�[�vID
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
MNGM_COUNTWK* MNGM_COUNT_Init( CLACT_SET_PTR p_clset, u32 heapID )
{
	MNGM_COUNTWK* p_wk;

	p_wk = sys_AllocMemory( heapID, sizeof( MNGM_COUNTWK ) );
	memset( p_wk, 0, sizeof( MNGM_COUNTWK ) );

	p_wk->p_clset = p_clset;
	
	// �n���h���I�[�v��
	p_wk->p_handle = ArchiveDataHandleOpen( ARC_WLMNGM_TOOL_GRA, heapID );

	// ���\�[�X�}�l�[�W���쐬
	MNGM_CLACTRES_Init( &p_wk->resman, MNGM_COUNT_OAMRESNUM, heapID );

	// ���\�[�X�ǂݍ���
	p_wk->p_resobj = MNGM_CLACTRES_Load( &p_wk->resman,
			p_wk->p_handle, 
			NARC_wlmngm_tool_font_boad_NCLR, MNGM_COUNT_PALNUM,
			NARC_wlmngm_tool_font_boad_NCGR,
			NARC_wlmngm_tool_font_boad_NCER,
			NARC_wlmngm_tool_font_boad_NANR,
			MNGM_RESCONTID,
			heapID );


	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM���[�N	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void MNGM_COUNT_Exit( MNGM_COUNTWK* p_wk )
{
	// �^�X�N�j��
	if( p_wk->p_tcb != NULL ){
		// ����j��
		TCB_Delete( p_wk->p_tcb );
		p_wk->p_tcb = NULL;
	}
	
	// �A�N�^�[�j��
	if( p_wk->p_anm != NULL ){
		CLACT_Delete( p_wk->p_anm );
	}

	// �}�X�N�V�X�e���j��
	MNGM_COUNT_MskExit( &p_wk->msk );

	// ���\�[�X�j��
	MNGM_CLACTRES_Exit( &p_wk->resman );

	// �n���h���j��
	ArchiveDataHandleClose( p_wk->p_handle );

	// ���[�N�j��
	sys_FreeMemoryEz( p_wk );
}

//--------------------------------------------------------------
/**
 * @brief   �p���b�g�W�J�ʒu���擾
 *
 * @param   p_wk		���[�N
 * @param   p_clset		�Z���A�N�^�[�Z�b�g
 *
 * @retval  �p���b�g�ʒu
 */
//--------------------------------------------------------------
int MNGM_PalNoGet( MNGM_COUNTWK* p_wk )
{
	CLACT_U_RES_OBJ_PTR res 
		= CLACT_U_ResManagerGetIDResObjPtr( p_wk->resman.p_resman[1], MNGM_RESCONTID );
	return CLACT_U_PlttManagerGetPlttNo(res, NNS_G2D_VRAM_TYPE_2DMAIN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	Redy	Start	�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void MNGM_COUNT_StartStart( MNGM_COUNTWK* p_wk )
{
	GF_ASSERT( p_wk->p_tcb == NULL );
	p_wk->p_tcb = TCB_Add( MNGM_COUNT_StartTcb, p_wk, 0 );
	p_wk->seq = MNGM_COUNT_START_SEQ_INIT;
}
void MNGM_COUNT_StartTimeUp( MNGM_COUNTWK* p_wk )
{
	GF_ASSERT( p_wk->p_tcb == NULL );
	p_wk->p_tcb = TCB_Add( MNGM_COUNT_TimeUpTcb, p_wk, 0 );
	p_wk->seq = MNGM_COUNT_TIMEUP_SEQ_INIT;

	Snd_SePlay( MNGM_SND_TIMEUP ); 
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G�t�F�N�g�������������`�F�b�NMNGM_ENTRY_SEQ_END
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL MNGM_COUNT_Wait( const MNGM_COUNTWK* p_wk )
{
	if( p_wk->seq == 0 ){
		return TRUE;
	}
	return FALSE;
}





//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�ėp�V�X�e��
//=====================================
//	�ėp��������
//----------------------------------------------------------------------------
/**
 *	@brief	�����ړ��v�Z���[�N	������
 *
 *	@param	p_wk		���[�N
 *	@param	s_x			�J�n�ʒu
 *	@param	e_x			�I���ʒu
 *	@param	s_s			�����x
 *	@param	count_max	�J�E���g�ő�l
 */
//-----------------------------------------------------------------------------
static void MNGM_AddMoveReqFx( MNGM_ADDMOVE_WORK* p_wk, fx32 s_x, fx32 e_x, fx32 s_s, int count_max )
{
	fx32 t_x_t;	// �^�C���̂Q��
	fx32 vot;	// �����x���^�C��
	fx32 dis;
	fx32 a;

	dis = e_x - s_x;
	
	// �����l�����߂�
	// a = 2(x - vot)/(t*t)
	t_x_t = (count_max * count_max) << FX32_SHIFT;
	vot = FX_Mul( s_s, count_max * FX32_ONE );
	vot = dis - vot;
	vot = FX_Mul( vot, 2*FX32_ONE );
	a = FX_Div( vot, t_x_t );

	p_wk->x = s_x;
	p_wk->s_x = s_x;
	p_wk->s_s = s_s;
	p_wk->s_a = a;
	p_wk->count_max = count_max;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ړ��v�Z���[�N	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	count		�J�E���^
 */
//-----------------------------------------------------------------------------
static BOOL	MNGM_AddMoveMainFx( MNGM_ADDMOVE_WORK* p_wk, s32 count )
{
	fx32 dis;
	fx32 t_x_t;
	fx32 calc_work;
	fx32 vot;
	BOOL ret;

	if( count >= p_wk->count_max ){
		count = p_wk->count_max;
		ret = TRUE;
	}else{
		ret = FALSE;
	}
	
	// �������x�^��
	// dis = vot + 1/2( a*(t*t) )
	vot = FX_Mul( p_wk->s_s, count << FX32_SHIFT );
	t_x_t = (count * count) << FX32_SHIFT;
	calc_work = FX_Mul( p_wk->s_a, t_x_t );
	calc_work = FX_Div( calc_work, 2*FX32_ONE );	// 1/2(a*(t*t))
	dis = vot + calc_work;	///<�ړ�����

	p_wk->x = p_wk->s_x + dis;

	return ret;
}


// �X�N���[��
//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���̃L�����N�^�I�t�Z�b�g��ύX����
 *	
 *	@param	p_scrn		�X�N���[���o�b�t�@
 *	@param	char_offs	�ύX����I�t�Z�b�g
 */
//-----------------------------------------------------------------------------
static void MNGM_SCRN_AddCharOfs( NNSG2dScreenData* p_scrn, u32 char_offs )
{
	int i;
	u16* p_scrndata;
	int size;

	size = p_scrn->szByte / 2;

	// �X�N���[���f�[�^���
	p_scrndata = (u16*)p_scrn->rawData;

	for(i=0; i<size; i++){
		p_scrndata[ i ] += char_offs;
	}
}

// �Z���A�N�^�[���\�[�X�Ǘ�
//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[���\�[�X�Ǘ��V�X�e��	������
 *
 *	@param	p_wk		���[�N
 *	@param	objnum		�I�u�W�F�N�g��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_CLACTRES_Init( MNGM_CLACTRES* p_wk, u32 objnum, u32 heapID )
{
	int i;

	// ���\�[�X�}�l�[�W���쐬
	for( i=0; i<MNGM_CLACTRES_RESMAN_NUM; i++ ){
		p_wk->p_resman[i] = CLACT_U_ResManagerInit(objnum, i, heapID);
	}

	// ���\�[�X�I�u�W�F�N�g�쐬
	p_wk->p_obj = sys_AllocMemory( heapID, sizeof(MNGM_CLACTRESOBJ)* objnum );
	memset( p_wk->p_obj, 0, sizeof(MNGM_CLACTRESOBJ)* objnum );
	p_wk->objnum = objnum;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[���\�[�X�Ǘ��V�X�e��	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_CLACTRES_Exit( MNGM_CLACTRES* p_wk )
{
	int i;

	// �S���\�[�X�I�u�W�F�̔j��
	for( i=0; i<p_wk->objnum; i++ ){
		if( p_wk->p_obj[i].data == TRUE ){
			MNGM_CLACTRES_OBJ_Exit( p_wk, &p_wk->p_obj[i] );
		}
	}

	// ���\�[�X�I�u�W�F��j��
	for( i=0; i<MNGM_CLACTRES_RESMAN_NUM; i++ ){
		CLACT_U_ResManagerDelete( p_wk->p_resman[i] );
	}

	// ���\�[�X�I�u�W�F�j��
	sys_FreeMemoryEz( p_wk->p_obj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���\�[�X��ǂݍ���
 *
 *	@param	p_wk			���[�N
 *	@param	p_handle		�n���h��
 *	@param	pal_idx			�p���b�gIDX
 *	@param	palnum			�p���b�g��
 *	@param	cg_idx			�L�����N�^IDX
 *	@param	cell_idx		�Z��IDX
 *	@param	anm_idx			�A�j��IDX
 *	@param	contid			�Ǘ�ID
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
static MNGM_CLACTRESOBJ* MNGM_CLACTRES_Load( MNGM_CLACTRES* p_wk, ARCHANDLE* p_handle, u32 pal_idx, u32 palnum, u32 cg_idx, u32 cell_idx, u32 anm_idx, u32 contid, u32 heapID )
{
	int i;
	MNGM_CLACTRESOBJ* p_obj = NULL;
	
	// �󂢂Ă�I�u�W�F��T��
	for( i=0; i<p_wk->objnum; i++ ){
		if( p_wk->p_obj[i].data == FALSE ){
			p_obj = &p_wk->p_obj[i];
			break;
		}
	}
	GF_ASSERT( p_obj );
	
	MNGM_CLACTRES_OBJ_Init( p_wk, p_obj, p_handle, pal_idx, palnum, cg_idx, cell_idx, anm_idx, contid, heapID );

	return p_obj;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�N�^�[�̓o�^
 *
 *	@param	p_obj		�I�u�W�F�N�g
 *	@param	p_clset		�Z���A�N�^�[�Z�b�g
 *	@param	x			�����W
 *	@param	y			�����W
 *	@param	pri			�D�揇��
 *	@param	heapID		�q�[�vID
 *
 *	@return	�A�N�^�[
 */
//-----------------------------------------------------------------------------
static CLACT_WORK_PTR MNGM_CLACTRES_Add( MNGM_CLACTRESOBJ* p_obj, CLACT_SET_PTR p_clset, s16 x, s16 y, u16 pri, u32 heapID )
{
	CLACT_ADD_SIMPLE add;


	add.ClActSet		= p_clset;
	add.ClActHeader		= &p_obj->header;
	add.mat.x			= x << FX32_SHIFT;
	add.mat.y			= y << FX32_SHIFT;
	add.pri				= pri;
	add.DrawArea		= NNS_G2D_VRAM_TYPE_2DMAIN;
	add.heap			= heapID;

	return CLACT_AddSimple( &add );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�̓ǂݍ���
 *
 *	@param	p_wk		���[�N
 *	@param	p_obj		���\�[�X�I�u�W�F�N�g
 *	@param	p_handle	�n���h��
 *	@param	pal_idx		�p���b�gIDX
 *	@param	palnum		�p���b�g��
 *	@param	cg_idx		�L�����N�^IDX
 *	@param	cell_idx	�Z��IDX
 *	@param	anm_idx		�A�j��IDX
 *	@param	contid		�Ǘ�ID
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_CLACTRES_OBJ_Init( MNGM_CLACTRES* p_wk, MNGM_CLACTRESOBJ* p_obj, ARCHANDLE* p_handle, u32 pal_idx, u32 palnum, u32 cg_idx, u32 cell_idx, u32 anm_idx, u32 contid, u32 heapID )
{
	BOOL result;
	
	GF_ASSERT( p_obj->data == FALSE );

	p_obj->data = TRUE;
	
	
	// �L�����N�^�ǂݍ���
	p_obj->p_resobj[0] = CLACT_U_ResManagerResAddArcChar_ArcHandle( 
				p_wk->p_resman[0], p_handle,
				cg_idx,
				FALSE, contid, NNS_G2D_VRAM_TYPE_2DMAIN, heapID );

	// �p���b�g�ǂݍ���
	p_obj->p_resobj[1] = CLACT_U_ResManagerResAddArcPltt_ArcHandle( 
			p_wk->p_resman[1], p_handle,
			pal_idx,
			FALSE, contid, 
			NNS_G2D_VRAM_TYPE_2DMAIN, palnum, heapID );

	// �Z���ǂݍ���
	p_obj->p_resobj[2] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_wk->p_resman[2], p_handle,
				cell_idx,
				FALSE, contid, CLACT_U_CELL_RES, heapID );

	// �A�j���ǂݍ���
	p_obj->p_resobj[3] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_wk->p_resman[3], p_handle,
				anm_idx,
				FALSE, contid, CLACT_U_CELLANM_RES, heapID );


	// VRAM�W�J
	result =CLACT_U_CharManagerSetAreaCont( p_obj->p_resobj[ 0 ] );
	GF_ASSERT( result );
	CLACT_U_ResManagerResOnlyDelete( p_obj->p_resobj[ 0 ] );
	result =CLACT_U_PlttManagerSetCleanArea( p_obj->p_resobj[ 1 ] );
	GF_ASSERT( result );
	CLACT_U_ResManagerResOnlyDelete( p_obj->p_resobj[ 1 ] );

	// �w�b�_�[�쐬
	CLACT_U_MakeHeader( &p_obj->header, contid, contid, contid, contid,
					CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
					0, MNGM_CLACTRES_DEF_BG_PRI,
					p_wk->p_resman[0],
					p_wk->p_resman[1],
					p_wk->p_resman[2],
					p_wk->p_resman[3],
					NULL, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���\�[�X�I�u�W�F�̔j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_obj		�I�u�W�F
 */
//-----------------------------------------------------------------------------
static void MNGM_CLACTRES_OBJ_Exit( MNGM_CLACTRES* p_wk, MNGM_CLACTRESOBJ* p_obj )
{
	int i;

	GF_ASSERT( p_obj->data == TRUE );

	p_obj->data = FALSE;
	
	// VRAM����j��
	CLACT_U_CharManagerDelete( p_obj->p_resobj[0] );
	CLACT_U_PlttManagerDelete( p_obj->p_resobj[1] );

	// ���\�[�X�j��
	for( i=0; i<MNGM_CLACTRES_RESMAN_NUM; i++ ){
		CLACT_U_ResManagerResDelete( p_wk->p_resman[i], 
				p_obj->p_resobj[i] );
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	BGL�V�X�e��������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_sys		�V�X�e���w�b�_�[
 *	@param	cp_cnt		�Ǘ��w�b�_�[
 *	@param	cnt_num		�Ǘ��f�[�^��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_BGL_Init( MNGM_BGL* p_wk, const GF_BGL_SYS_HEADER* cp_sys, const MNGM_BGL_DATA* cp_cnt, u32 cnt_num, u32 heapID )
{
	
	// BG�ݒ�
	GF_BGL_InitBG(cp_sys);

	p_wk->p_bgl = GF_BGL_BglIniAlloc( heapID );
	p_wk->cp_tbl = cp_cnt;
	p_wk->tblnum = cnt_num;

	// ���C���ƃT�u��؂�ւ���
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();


	// BG�R���g���[���ݒ�
	{
		int i;

		for( i=0; i<cnt_num; i++ ){
			GF_BGL_BGControlSet( p_wk->p_bgl, 
					cp_cnt[i].frame, &cp_cnt[i].cnt,
					GF_BGL_MODE_TEXT );
			GF_BGL_ClearCharSet( cp_cnt[i].frame, 32, 0, heapID);
			GF_BGL_ScrClear( p_wk->p_bgl, cp_cnt[i].frame );

			// �X�N���[���ʒu������
			GF_BGL_ScrollSet( p_wk->p_bgl, cp_cnt[i].frame, GF_BGL_SCROLL_X_SET, 0 );
			GF_BGL_ScrollSet( p_wk->p_bgl, cp_cnt[i].frame, GF_BGL_SCROLL_Y_SET, 0 );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	BGL�Ǘ��V�X�e��	�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_BGL_Exit( MNGM_BGL* p_wk )
{

	// �X�N���[���N���[��
	GF_BGL_ScrollSet( p_wk->p_bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_Y_SET, 0 );

	// �a�f�R���g���[���j��
	{
		int i;

		for( i=0; i<p_wk->tblnum; i++ ){
			GF_BGL_BGControlExit( p_wk->p_bgl, p_wk->cp_tbl[i].frame );
		}
	}
	
	// BGL�j��
	sys_FreeMemoryEz( p_wk->p_bgl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�֐�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_BGL_VBlank( MNGM_BGL* p_wk )
{
    // BG��������
    GF_BGL_VBlankFunc( p_wk->p_bgl );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[������
 *
 *	@param	p_wk		���[�N
 *	@param	objnum		�I�u�W�F�N�g��
 *	@param	charnum		�L�����N�^��
 *	@param	plttnum		�p���b�g��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_CLACT_Init( MNGM_CLACT* p_wk, u32 objnum, u32 charnum, u32 plttnum, u32 heapID )
{
    int i;

    // OAM�}�l�[�W���[�̏�����
    NNS_G2dInitOamManagerModule();

    // ���LOAM�}�l�[�W���쐬
    // �����_���pOAM�}�l�[�W���쐬
    // �����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
    REND_OAMInit(
        0, 126,     // ���C�����OAM�Ǘ��̈�
        0, 31,      // ���C����ʃA�t�B���Ǘ��̈�
        0, 126,     // �T�u���OAM�Ǘ��̈�
        0, 31,      // �T�u��ʃA�t�B���Ǘ��̈�
        heapID);


    // �L�����N�^�}�l�[�W���[������
	{

		CHAR_MANAGER_MAKE char_init = sc_MNGM_CHARMAN_INIT;
		char_init.CharDataNum	= charnum;
		char_init.heap			= heapID;
	    InitCharManagerReg(&char_init, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K );
	}
    // �p���b�g�}�l�[�W���[������
    InitPlttManager(plttnum, heapID);

    // �ǂݍ��݊J�n�ʒu��������
    CharLoadStartAll();
    PlttLoadStartAll();

    //�ʐM�A�C�R���p�ɃL�������p���b�g����
    CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
    

    // �Z���A�N�^�[�Z�b�g�쐬
    p_wk->p_clactset = CLACT_U_SetEasyInit( objnum, &p_wk->renddata, heapID );

	// ����ʂɒʐM�A�C�R�����o��
	WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��


	// �\���J�n
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_CLACT_Exit( MNGM_CLACT* p_wk )
{
    // �A�N�^�[�̔j��
    CLACT_DestSet( p_wk->p_clactset );

    // ���\�[�X���
    DeleteCharManager();
    DeletePlttManager();

    //OAM�����_���[�j��
    REND_OAM_Delete();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�\������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_CLACT_Draw( MNGM_CLACT* p_wk )
{
	CLACT_Draw( p_wk->p_clactset );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[	VBlank����
 *
 *	@param p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_CLACT_VBlank( MNGM_CLACT* p_wk )
{
    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();
}



//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�V�X�e��	������
 *
 *	@param	p_wk		���[�N		
 *	@param	heapID		�Ё[��ID
 */	
//-----------------------------------------------------------------------------
static void MNGM_MSG_Init( MNGM_MSG* p_wk, u32 heapID )
{
	p_wk->p_msgman	= MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_minigame_tool_dat, heapID );
	p_wk->p_wordset	= WORDSET_CreateEx( WORDSET_DEFAULT_SETNUM, WORDSET_COUNTRY_BUFLEN, heapID );
	p_wk->p_str		= STRBUF_Create( MNGM_MSG_STRBUF_NUM, heapID );
	p_wk->p_tmp		= STRBUF_Create( MNGM_MSG_STRBUF_NUM, heapID );

	// ���b�Z�[�W�p���b�g��]������
    TalkFontPaletteLoad( PALTYPE_MAIN_BG, MNGM_MSG_PLTT*0x20, heapID );
    TalkFontPaletteLoad( PALTYPE_SUB_BG,  MNGM_MSG_PLTT*0x20, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�V�X�e��	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_Exit( MNGM_MSG* p_wk )
{
	STRBUF_Delete( p_wk->p_tmp );
	STRBUF_Delete( p_wk->p_str );
	WORDSET_Delete( p_wk->p_wordset );
	MSGMAN_Delete( p_wk->p_msgman );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�����̃��b�Z�[�W�o�b�t�@�����ׂĂ��當���ŃN���A
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_ClearStrBuff( MNGM_MSG* p_wk )
{
	WORDSET_ClearAllBuffer( p_wk->p_wordset );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�[���\��
 *		
 *	@param	p_wk			���[�N
 *	@param	cp_player		�v���C���[��
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_SetPlayerName( MNGM_MSG* p_wk, const MYSTATUS* cp_player )
{
	WORDSET_RegisterPlayerName( p_wk->p_wordset, 0, cp_player );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������o�^����
 *
 *	@param	p_wk			���[�N
 *	@param	nation			��ID
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_SetNationName( MNGM_MSG* p_wk, u32 nation )
{
	WORDSET_RegisterCountryName( p_wk->p_wordset, 0, nation );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n�於��o�^����
 *
 *	@param	p_wk			���[�N
 *	@param	nation			��ID
 *	@param	area			�n��ID
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_SetAreaName( MNGM_MSG* p_wk, u32 nation, u32 area )
{
	WORDSET_RegisterLocalPlaceName( p_wk->p_wordset, 0, nation, area );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������i�[����
 *
 *	@param	p_wk		���[�N
 *	@param	number		�i���o�[
 *	@param	keta		��
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_SetScore( MNGM_MSG* p_wk, u32 number, u32 keta )
{
	WORDSET_RegisterNumber( p_wk->p_wordset, 0, number,
			keta, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���������D�̐����i�[����
 *
 *	@param	p_wk		���[�N
 *	@param	number		��
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_SetBalloonNum( MNGM_MSG* p_wk, u32 number )
{
	WORDSET_RegisterNumber( p_wk->p_wordset, 0, number,
			2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ԃ��i�[����
 *
 *	@param	p_wk		���[�N
 *	@param	time		����
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_SetTime( MNGM_MSG* p_wk, u32 time )
{
	u32 sec;
	u32 miri;

	miri = (time * MNGM_MSG_TIME_1DATA) / MNGM_MSG_TIME_1DATADIV;
	sec  = miri / MNGM_MSG_TIME_1DATA;	// 100miri�b��1�b
	miri -= sec*MNGM_MSG_TIME_1DATA;
	
	WORDSET_RegisterNumber( p_wk->p_wordset, 0, sec,
			2, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT );

	WORDSET_RegisterNumber( p_wk->p_wordset, 1, miri,
			2, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g����ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	gadget		�K�W�F�b�g�i���o�[
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_SetGadget( MNGM_MSG* p_wk, u32 gadget )
{
	WORDSET_RegisterWiFiLobbyItemName( p_wk->p_wordset, 1, gadget );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[������ݒ肷��
 *	
 *	@param	p_wk		���[�N
 *	@param	game		�Q�[���i���o�[
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_SetGameName( MNGM_MSG* p_wk, u32 game )
{
	WORDSET_RegisterWiFiLobbyGameName( p_wk->p_wordset, 0, game );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�f�[�^���擾����
 *
 *	@param	p_wk	���[�N
 *	@param	p_str	���b�Z�[�W�f�[�^�i�[��
 *	@param	msgidx	���b�Z�[�W�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_GetStr( MNGM_MSG* p_wk, STRBUF* p_str, u32 msgidx )
{
	MSGMAN_GetString( p_wk->p_msgman, msgidx, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_str, p_wk->p_tmp );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�v�����g
 *
 *	@param	p_wk		���[�N
 *	@param	no			���b�Z�[�WNO
 *	@param	p_win		�E�B���h�E
 *	@param	x			�����W
 *	@param	y			�����W
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_Print( MNGM_MSG* p_wk, u32 no, GF_BGL_BMPWIN* p_win, u8 x, u8 y )
{
	MNGM_MSG_PrintColor( p_wk, no, p_win, x, y, MNGM_MSG_COLOR );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�[�߂ɂ��ĕ������\��
 *
 *	@param	p_wk		���[�N
 *	@param	no			���b�Z�[�WNO
 *	@param	p_win		�E�B���h�E
 *	@param	x			�E�[�����W
 *	@param	y			�����W
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_PrintRightSide( MNGM_MSG* p_wk, u32 no, GF_BGL_BMPWIN* p_win, u8 x, u8 y )
{
	u32 strsize;
	s32 draw_x;
	
	MSGMAN_GetString( p_wk->p_msgman, no, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_str, p_wk->p_tmp );

	strsize = FontProc_GetPrintStrWidth( FONT_SYSTEM, p_wk->p_str, 0 );
	draw_x = x - strsize;
	if( draw_x < 0 ){
		draw_x = 0;
	}
	
	GF_STR_PrintColor( p_win, FONT_SYSTEM, p_wk->p_str,
			draw_x,y,MSG_NO_PUT, MNGM_MSG_COLOR, NULL);
}

//----------------------------------------------------------------------------
/**

 *	@brief	���b�Z�[�W�v�����g
 *
 *	@param	p_wk		���[�N
 *	@param	no			���b�Z�[�WNO
 *	@param	p_win		�E�B���h�E
 *	@param	p_str		�����o�b�t�@
 *	@param	wait		���b�Z�[�W�`��E�G�C�g
 *
 *	@retval	���b�Z�[�W�i���o�[
 */
//-----------------------------------------------------------------------------
static u32 MNGM_MSG_PrintScr( MNGM_MSG* p_wk, u32 no, GF_BGL_BMPWIN* p_win, STRBUF* p_str, u32 wait )
{
	MSGMAN_GetString( p_wk->p_msgman, no, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_str, p_wk->p_tmp );
	
	return GF_STR_PrintColor( p_win, FONT_TALK, p_str,
			0, 0, wait, MNGM_MSG_TALKCOLOR, NULL);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�w�胁�b�Z�[�W�`��
 *
 *	@param	p_wk		���[�N
 *	@param	no			���b�Z�[�WNO
 *	@param	p_win		�E�B���h�E
 *	@param	x			�����W
 *	@param	y			�����W
 *	@param	col			�F
 */
//-----------------------------------------------------------------------------
static void MNGM_MSG_PrintColor( MNGM_MSG* p_wk, u32 no, GF_BGL_BMPWIN* p_win, u8 x, u8 y, GF_PRINTCOLOR col )
{
	MSGMAN_GetString( p_wk->p_msgman, no, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_str, p_wk->p_tmp );
	
	GF_STR_PrintColor( p_win, FONT_SYSTEM, p_wk->p_str,
			x,y,MSG_NO_PUT, col, NULL);
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E������
 *
 *	@param	p_wk		���[�N
 *	@param	p_bgl		�����V�X�e��
 *	@param	p_save		�Z�[�u
 *	@param	vip			VIP�t���O
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void MNGM_TALKWIN_Init( MNGM_TALKWIN* p_wk, MNGM_BGL* p_bgl, SAVEDATA* p_save, BOOL vip, u32 heapID )
{
	u32 type = CONFIG_GetWindowType(SaveData_GetConfig(p_save)); 
	u32 frame;
	int i;
	u8 y;

	//  �I�[�g���b�Z�[�W�ݒ�
	ConTool_MsgPrintFlagSet( TRUE );
	
	// �g�[�N�E�B���h�E�̃E�B���h�E��]��
    TalkWinGraphicSet(
            p_bgl->p_bgl, GF_BGL_FRAME2_M, MNGM_TALKWIN_CGX, 
			MNGM_TALKWIN_PAL,  type, heapID );
    TalkWinGraphicSet(
            p_bgl->p_bgl, GF_BGL_FRAME0_S, MNGM_TALKWIN_CGX, 
			MNGM_TALKWIN_PAL,  type, heapID );

	// �E�B���h�E�̍쐬
	for( i=0; i<MNGM_TALKWIN_IDX_NUM; i++ ){
		if( i==MNGM_TALKWIN_IDX_MAIN ){
			frame = GF_BGL_FRAME2_M;
			y = MNGM_TALKWIN_BMP_Y;
		}else{
			frame = GF_BGL_FRAME0_S;
			y = MNGM_TALKWIN_BMP_Y_SUB;
		}
		GF_BGL_BmpWinAdd( p_bgl->p_bgl, &p_wk->win[i], frame,
				MNGM_TALKWIN_BMP_X, y,
				MNGM_TALKWIN_BMP_SIZX, MNGM_TALKWIN_BMP_SIZY,
				MNGM_TALKWIN_BMP_PAL, MNGM_TALKWIN_BMP_CGX);
		GF_BGL_BmpWinDataFill( &p_wk->win[i], 0 );

		// ���b�Z�[�W�p������o�b�t�@�쐬
		p_wk->p_str[i] = STRBUF_Create( MNGM_MSG_STRBUF_NUM, heapID );
	}

	p_wk->vip = vip;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_TALKWIN_Exit( MNGM_TALKWIN* p_wk )
{
	int i;
	
	// �E�B���h�E�ƕ�����o�b�t�@��j��
	for( i=0; i<MNGM_TALKWIN_IDX_NUM; i++ ){
		MNGM_TALKWIN_MsgOff( p_wk, i );
		STRBUF_Delete( p_wk->p_str[i] );
		GF_BGL_BmpWinDel( &p_wk->win[i] );
	}

	//  �I�[�g���b�Z�[�W�ݒ�j��
	ConTool_MsgPrintFlagReset();
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E	�v�����g
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		�E�B���h�E	
 *	@param	msgidx		���b�Z�[�WIDX
 *	@param	idx			���C�����T�u��
 */
//-----------------------------------------------------------------------------
static void MNGM_TALKWIN_MsgPrint( MNGM_TALKWIN* p_wk, MNGM_MSG* p_msg, u32 msgidx, u32 idx )
{
	int i;
	
	// VIP�Ȃ�msgidx��VIP�p�̂��̂ɂ���
	if( p_wk->vip == TRUE ){
		for( i=0; i<NELEMS(sc_MNGM_MSG_DATA); i++ ){
			if( sc_MNGM_MSG_DATA[i].normal == msgidx ){
				msgidx = sc_MNGM_MSG_DATA[i].vip;
			}
		}
	}
	
	
	// �\�����Ȃ�Ƃ߂�
	if( GF_MSG_PrintEndCheck( p_wk->msg_no[idx] ) ){
		GF_STR_PrintForceStop( p_wk->msg_no[idx] );
	}
	
	// ���ԃE�B���h�E���~�߂�
	MNGM_TALKWIN_CleanTimeWork( p_wk, idx );
	
	GF_BGL_BmpWinDataFill( &p_wk->win[idx], 15 );
	p_wk->msg_no[idx] = MNGM_MSG_PrintScr( p_msg, msgidx, &p_wk->win[idx],
			p_wk->p_str[idx], MNGM_TALKWIN_MSG_SPEED );

	// �E�C���h�E����������
    BmpTalkWinWrite(&p_wk->win[idx], WINDOW_TRANS_OFF, MNGM_TALKWIN_CGX, MNGM_TALKWIN_PAL );

	// �]�����N�G�X�g
	GF_BGL_BmpWinOnVReq( &p_wk->win[idx] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�E�B���h�E������
 *
 *	@param	p_wk		���[�N
 *	@param	idx			���C�����T�u��
 */
//-----------------------------------------------------------------------------
static void MNGM_TALKWIN_MsgOff( MNGM_TALKWIN* p_wk, u32 idx )
{
	MNGM_TALKWIN_CleanTimeWork( p_wk, idx );
	// �\�����Ȃ�Ƃ߂�
	if( GF_MSG_PrintEndCheck( p_wk->msg_no[idx] ) ){
		GF_STR_PrintForceStop( p_wk->msg_no[idx] );
	}
	BmpTalkWinClear( &p_wk->win[idx], WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( &p_wk->win[idx] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�E�B���h�E	���b�Z�[�W�I���`�F�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	idx			���C�����T�u��
 *	
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_TALKWIN_MsgEndCheck( const MNGM_TALKWIN* p_wk, u32 idx )
{
	u32 result;
	result = GF_MSG_PrintEndCheck(p_wk->msg_no[idx]);
	if( result == 0 ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�E�B���h�E	���W�ݒ�
 *	
 *	@param	p_wk		���[�N
 *	@param	x			�����W
 *	@param	y			�����W
 *	@param	idx			���C�����T�u��
 */
//-----------------------------------------------------------------------------
static void MNGM_TALKWIN_SetPos( MNGM_TALKWIN* p_wk, u8 x, u8 y, u32 idx )
{
	GF_BGL_BmpWinSet_PosX( &p_wk->win[idx], x );
	GF_BGL_BmpWinSet_PosY( &p_wk->win[idx], y );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃ}�[�N���o��
 *	
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_TALKWIN_SetTimeWork( MNGM_TALKWIN* p_wk, u32 idx )
{
	if( p_wk->p_timewait[idx] == NULL ){
		p_wk->p_timewait[idx] = TimeWaitIconAdd( &p_wk->win[idx], MNGM_TALKWIN_CGX );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃ}�[�N������
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void MNGM_TALKWIN_CleanTimeWork( MNGM_TALKWIN* p_wk, u32 idx )
{
	if( p_wk->p_timewait[idx] ){
        TimeWaitIconTaskDel(p_wk->p_timewait[idx]);  // �^�C�}�[�~��
		p_wk->p_timewait[idx] = NULL;
	}
}






//-------------------------------------
///	�G���g���[��ʃ��[�N
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[��ʍ쐬	����
 *
 *	@param	cp_commparam	���ʃp�����[�^
 *	@param	gametype		�Q�[���^�C�v
 *	@param	heapID			�q�[�vID
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
static MNGM_ENTRYWK* MNGM_ENTRY_CommonInit( const MNGM_ENRES_PARAM* cp_commparam, u32 gametype, u32 heapID )
{
	MNGM_ENTRYWK* p_wk;
	int i;

	// BGM�Đ�
	Snd_DataSetByScene( SND_SCENE_WIFI_LOBBY_GAME, SEQ_PL_WIFIGAME, 0 );

	p_wk = sys_AllocMemory( heapID, sizeof(MNGM_ENTRYWK) );
	memset( p_wk, 0, sizeof(MNGM_ENTRYWK) );

	// �p�����[�^���R�s�[
	memcpy( &p_wk->comm_param, cp_commparam, sizeof(MNGM_ENRES_PARAM) );

	// �Q�[���^�C�v�ݒ�
	p_wk->gametype = gametype;

	G2_BlendNone();
	G2S_BlendNone();
	GX_SetVisibleWnd(0);
	GXS_SetVisibleWnd(0);

	// �o���N�ݒ�
	GF_Disp_SetBank( &sc_MNGM_ENTRY_BANK );

	// �q�[�vID�ۑ�
	p_wk->heapID = heapID;

	// �ʐM�R�}���h�ݒ�
	CommCommandMNGMEntryInitialize( p_wk );

	// �ʐM�f�[�^��ݒ�
	MNGM_COMMPDATA_Init( &p_wk->comm_pdata, &p_wk->comm_param );

	// BG
	MNGM_BGL_Init( &p_wk->bgl, &sc_MNGM_ENTRY_BGINIT, sc_MNGM_ENTRY_BGCNT, MNGM_ENTRY_BGL_USE, heapID );

	// CLACT
	MNGM_CLACT_Init( &p_wk->clact, MNGM_ENTRY_CLACT_WKNUM, MNGM_ENTRY_CLACT_CHARNUM, MNGM_ENTRY_CLACT_PLTTNUM, heapID );

	// CLACTRES
	MNGM_CLACTRES_Init( &p_wk->clres, MNGM_ENTRY_CLACT_RESNUM, heapID );

	// ���b�Z�[�W�V�X�e���쐬
	MNGM_MSG_Init( &p_wk->msg, heapID );	

	// �O���t�B�b�N�ǂݍ���
	MNGM_ENTRY_GraphicLoad( p_wk, heapID );

	// ���b�Z�[�W�V�X�e��������
	{
		BOOL vip;
		u32 netid;

		netid = MNGM_ENRES_PARAM_GetPlNOtoNetID( &p_wk->comm_param, p_wk->comm_param.my_playerid );
		
		vip = MNGM_ENRES_PARAM_GetVipFlag( &p_wk->comm_param, netid );
		MNGM_TALKWIN_Init( &p_wk->talkwin, &p_wk->bgl, p_wk->comm_param.p_save, 
				vip,  heapID );
	}


	// �^�X�N����
	p_wk->tcb		= TCB_Add( MNGM_ENTRY_Tcb, p_wk, 0 );
	p_wk->tcb_vwait = VWaitTCB_Add( MNGM_ENTRY_VWaitTcb, p_wk, 0 );

	// VCHAT ON
	if( p_wk->comm_param.vchat ){
		// �{�C�X�`���b�g�J�n
		mydwc_startvchat( heapID );
	}
	
	return p_wk;
}
//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[���	�O���t�B�b�N�ǂݍ���
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_ENTRY_GraphicLoad( MNGM_ENTRYWK* p_wk, u32 heapID )
{
	ARCHANDLE* p_handle;

	p_handle = ArchiveDataHandleOpen( ARC_WLMNGM_TOOL_GRA, heapID );

	// �p���b�g
	ArcUtil_HDL_PalSet( p_handle, NARC_wlmngm_tool_minigame_win_NCLR,
			PALTYPE_MAIN_BG, 0, MNGM_BGPLTT_NUM*32, heapID );
	
	// �L�����N�^	
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wlmngm_tool_minigame_win_bg_NCGR,
			p_wk->bgl.p_bgl, 
			GF_BGL_FRAME1_M, 0, 0,
			FALSE, heapID );

	// �X�N���[��
	p_wk->p_scrnbuf =  ArcUtil_HDL_ScrnDataGet( p_handle,
			NARC_wlmngm_tool_minigame_win_bg1_NSCR,
			FALSE, &p_wk->p_scrn, heapID );

	// �w�i�ݒ�
	ArcUtil_HDL_ScrnSet( p_handle, NARC_wlmngm_tool_minigame_win_bg0_NSCR, p_wk->bgl.p_bgl,
			GF_BGL_FRAME3_M, 0, 0, FALSE, heapID );
	// �w�i�p���b�g�]��
	MNGM_BACKPLTT_Trans( p_handle, p_wk->gametype, heapID );
	

	// OAM��ǂݍ���
	p_wk->p_resobj = MNGM_CLACTRES_Load( &p_wk->clres, p_handle, 
			NARC_wlmngm_tool_minigame_win_oam_NCLR, MNGM_PLAYER_OAM_PLTT_NUM,
			NARC_wlmngm_tool_minigame_win_oam_NCGR,
			NARC_wlmngm_tool_minigame_win_oam_NCER,
			NARC_wlmngm_tool_minigame_win_oam_NANR,
			MNGM_RESCONTID, heapID );

	// �^�C�g�����S
	MNGM_TITLELOGO_Init( &p_wk->titlelogo, &p_wk->bgl, &p_wk->msg, p_wk->gametype, p_handle, heapID );

	ArchiveDataHandleClose( p_handle );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[���	�O���t�B�b�N	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_ENTRY_GraphicDelete( MNGM_ENTRYWK* p_wk )
{
	// �X�N���[����j��
	sys_FreeMemoryEz( p_wk->p_scrnbuf );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[���	�^�X�N
 *
 *	@param	tcb			TCB
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_ENTRY_Tcb( TCB_PTR tcb, void* p_work )
{
	MNGM_ENTRYWK* p_wk = p_work;
	BOOL  result;

	switch( p_wk->seq ){
	case MNGM_ENTRY_SEQ_WIPEIN_INIT:
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEIN, 
				WIPE_TYPE_FADEIN, WIPE_FADE_OUTCOLOR, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, p_wk->heapID );

		// �^�C�g�����S�`��
		MNGM_TITLELOGO_InStart( &p_wk->titlelogo, &p_wk->bgl, p_wk->heapID );
		p_wk->seq ++;
		break;
		
	case MNGM_ENTRY_SEQ_WIPEIN_WAIT:
		MNGM_TITLELOGO_InMain( &p_wk->titlelogo, &p_wk->bgl );	// ���S�C���@����
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){
			p_wk->seq ++;
		}
		break;

	// ���S����E�G�C�g
	case MNGM_ENTRY_SEQ_LOGOIN_WAIT:
		result = MNGM_TITLELOGO_InMain( &p_wk->titlelogo, &p_wk->bgl );	// ���S�C���@����
		if( result == TRUE ){
			p_wk->seq++;
		}
		break;

	// �G���g���[���b�Z�[�W�\��
	case MNGM_ENTRY_SEQ_MSGON_INIT:
		MNGM_TALKWIN_MsgPrint( &p_wk->talkwin, &p_wk->msg, msg_00, MNGM_TALKWIN_IDX_MAIN );
		p_wk->seq ++;
		break;
		
	case MNGM_ENTRY_SEQ_MSGON_WAIT:
		result = MNGM_TALKWIN_MsgEndCheck( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );
		if( result ){
			p_wk->seq ++;
			
			p_wk->wait = MNGM_ENTRY_MSGWAIT;
		}
		break;
		
	case MNGM_ENTRY_SEQ_BGMOVE_START:
		if( p_wk->wait > 0 ){
			p_wk->wait --;
			if( p_wk->wait == 0 ){
				MNGM_TALKWIN_MsgOff( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );

				MNGM_TITLELOGO_OutStart( &p_wk->titlelogo, &p_wk->bgl );
				p_wk->seq++;
			}
		}
		break;

	// ���S�A�E�g�҂�
	case MNGM_ENTRY_SEQ_LOGOOUT_WAIT:
		result = MNGM_TITLELOGO_OutMain( &p_wk->titlelogo, &p_wk->bgl );
		if( result == TRUE ){
			p_wk->seq ++;
		}
		break;

	
	// �v���C���[���X�g�\��
	case MNGM_ENTRY_SEQ_PLLIST_ON:
		// �v���C���[���̃f�[�^�쐬
		{
			int i;
			BOOL draw_nation;	// ��ID�Œn���\�����邩
			BOOL vip;
			u32 netid;

			//  HBLANK�����J�n
			MNGM_PLAYER_PLATE_HBLANK_BGSCR_Init( &p_wk->bgscrl, &p_wk->bgl, p_wk->heapID );

			// �����ŕ\�����邩�`�F�b�N
			draw_nation = MNGM_COMMPARAM_CheckDrawNation( &p_wk->comm_param, &p_wk->comm_pdata );
			
			for( i=0; i<p_wk->comm_param.num; i++ ){
				MNGM_PLATE_PLAYERTBL_Init( &p_wk->playertbl[i], p_wk->comm_param.num, 
						p_wk->gametype,
						p_wk->p_scrn, &p_wk->bgl, i, p_wk->comm_param.my_playerid,
						&p_wk->clact, p_wk->p_resobj, 
						p_wk->heapID );

				// ���O�\��
				netid = MNGM_ENRES_PARAM_GetPlNOtoNetID( &p_wk->comm_param, i );
				vip = MNGM_ENRES_PARAM_GetVipFlag( &p_wk->comm_param, netid );
				MNGM_PLATE_PLAYERTBL_DrawName( &p_wk->playertbl[i], &p_wk->msg, p_wk->comm_pdata.cp_status[i], MNGM_ENTRY_DRAWNAME_X, 0, vip );
				// �����\��
				MNGM_PLATE_PLAYERTBL_DrawNation( &p_wk->playertbl[i], &p_wk->msg, 
						draw_nation, p_wk->comm_pdata.nation[i],
						p_wk->comm_pdata.area[i], MNGM_ENTRY_DRAWNAME_X );

				// ����p�����[�^������
				MNGM_PLATE_PLAYERTBL_StartMove( &p_wk->playertbl[i], MNGM_PLATE_MOVE_ENTRY_END, MNGM_PLATE_MOVE_WAIT_ENTRY*i );
			}
		}


		//  �������ݖ���
		{
			int i;
			for( i=0; i<p_wk->comm_param.num; i++ ){
				
				MNGM_PLATE_PLAYERTBL_BgWriteVReq( &p_wk->playertbl[i], &p_wk->bgl );
				MNGM_PLATE_PLAYERTBL_DrawOamWay( &p_wk->playertbl[i] );
			}
		}

		p_wk->wait = MNGM_ENTRY_WAIT;
		p_wk->seq ++;
		break;
		
	case MNGM_ENTRY_SEQ_BGMOVE_MAIN:
		if( MNGM_ENTRY_PLATE_ALLMove( p_wk ) == TRUE ){
			p_wk->seq ++;
		}
		break;

	case MNGM_ENTRY_SEQ_BGMOVE_WAIT:
#ifdef DEBUG_SYNCSTART_A
		if( sys.trg & PAD_BUTTON_A ){
			p_wk->seq ++;
			CommTimingSyncStart( MNGM_SYNC_END );

			// VCHAT OFF
			if( p_wk->comm_param.vchat ){
				// �{�C�X�`���b�g�I��
				mydwc_stopvchat();
			}

		}
#else
		p_wk->wait --;
		if( p_wk->wait == 0 ){
			p_wk->seq ++;
			CommTimingSyncStart( MNGM_SYNC_END );

			// VCHAT OFF
			if( p_wk->comm_param.vchat ){
				// �{�C�X�`���b�g�I��
				mydwc_stopvchat();
			}
		}
#endif
		break;

	case MNGM_ENTRY_SEQ_BGMOVE_SYNC:
		if( CommIsTimingSync( MNGM_SYNC_END ) ){
			p_wk->seq ++;
		}
		break;

	case MNGM_ENTRY_SEQ_BGMOVE_OUT:
		{
			int i;
			for( i=0; i<p_wk->comm_param.num; i++ ){
				// ����p�����[�^������
				MNGM_PLATE_PLAYERTBL_StartMoveRet( &p_wk->playertbl[i], i*MNGM_PLATE_MOVE_WAIT_ENTRY );
			}
			p_wk->seq ++;
			p_wk->wait = MNGM_ENTRY_OUTMOVEWAIT;
		}
		break;

	case MNGM_ENTRY_SEQ_BGMOVE_OUTWAIT:
		p_wk->wait--;
		MNGM_ENTRY_PLATE_ALLMove( p_wk );

		if( p_wk->wait <= 0 ){
			p_wk->seq ++;
		}
		break;
	
	case MNGM_ENTRY_SEQ_WIPEOUT_INIT:
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEOUT, 
				WIPE_TYPE_FADEIN, WIPE_FADE_BLACK, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, p_wk->heapID );

		MNGM_ENTRY_PLATE_ALLMove( p_wk );

		// Rare�Q�[���f�[�^���M
		if( p_wk->comm_param.my_playerid == 0 ){
			u32 raregame_type;
			
			switch( p_wk->gametype ){
			case WFLBY_GAME_BALLSLOW:	// �ʓ���
				raregame_type = MNGM_ENTRY_RareGameSelect( sc_MNGM_RAREGAME_BUCKET_TBL, MNGM_RAREGAME_BUCKET_NUM );
				break;

			case WFLBY_GAME_BALANCEBALL:// �ʏ��
				raregame_type = MNGM_ENTRY_RareGameSelect( sc_MNGM_RAREGAME_BALANCEBALL_TBL, MNGM_RAREGAME_BALANCEBALL_NUM );
				break;

			case WFLBY_GAME_BALLOON:	// �ӂ�������
				raregame_type = MNGM_ENTRY_RareGameSelect( sc_MNGM_RAREGAME_BALLOON_TBL, MNGM_RAREGAME_BALLOON_NUM );
				break;
			}

			CommSendData( CNM_MNGM_ENTRY_RAREGAME, &raregame_type, sizeof(u32) );
		}

		p_wk->seq ++;
		break;
		
	case MNGM_ENTRY_SEQ_WIPEOUT_WAIT:
		MNGM_ENTRY_PLATE_ALLMove( p_wk );
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){

			// ���X�^�[�V�X�e���j��
			MNGM_PLAYER_PLATE_HBLANK_BGSCR_Exit( &p_wk->bgscrl );

			p_wk->seq ++;
		}
		break;

	case MNGM_ENTRY_SEQ_RAREGAME_WAIT:
		if( p_wk->raregame_recv == TRUE ){
			p_wk->seq ++;
		}
		break;

	case MNGM_ENTRY_SEQ_END:
		break;
	}

	MNGM_CLACT_Draw( &p_wk->clact );

	// �X�N���[���ʂ��X�N���[��������
	GF_BGL_ScrollReq( p_wk->bgl.p_bgl, GF_BGL_FRAME3_M, 
			GF_BGL_SCROLL_Y_INC, MNGM_BGSCROLL_SPEED );
	GF_BGL_ScrollReq( p_wk->bgl.p_bgl, GF_BGL_FRAME1_S, 
			GF_BGL_SCROLL_Y_INC, MNGM_BGSCROLL_SPEED );
}

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�^�X�N
 */
//-----------------------------------------------------------------------------
static void MNGM_ENTRY_VWaitTcb( TCB_PTR tcb, void* p_work )
{
	MNGM_ENTRYWK* p_wk = p_work;

	// BG��VBLANK����
    MNGM_BGL_VBlank( &p_wk->bgl );

	// OAM��VBLANK����
	MNGM_CLACT_VBlank( &p_wk->clact );

	// ���X�^�[�V�X�e��
	MNGM_PLAYER_PLATE_HBLANK_BGSCR_VBlank( &p_wk->bgscrl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	rare�Q�[���^�C�v�̑I������
 *
 *	@param	sc_ParTbl	�e�[�u��
 *	@param	num			rare�f�[�^��
 *
 *	@return	rare�Q�[���^�C�v
 */
//-----------------------------------------------------------------------------
static MNGM_RAREGAME_TYPE MNGM_ENTRY_RareGameSelect( const u8* cp_ParTbl, u32 num )
{
	u32 rand;
	u32 check;
	int i;

	rand = gf_mtRand() % 100;

//	OS_TPrintf( "rand %d\n", rand );
	
	check = 0;
	for( i=0; i<num; i++ ){
		check += cp_ParTbl[i];
//		OS_TPrintf( "check %d\n", check );
		if( check > rand ){
			return i;
		}
	}

	GF_ASSERT(0);	// ���肦�Ȃ�
	return num-1;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���g���[��ʗp	�SPLATE����
 *	
 *	@param	p_wk	��[��
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_ENTRY_PLATE_ALLMove( MNGM_ENTRYWK* p_wk )
{
	int i;
	BOOL result;
	BOOL plate_end = TRUE;

	// �S����PLATE�𓮂���
	for( i=0; i<p_wk->comm_param.num; i++ ){


		// ����������Ă邩�`�F�b�N
		if( p_wk->playertbl[i].p_clwk != NULL ){

			result = MNGM_PLATE_PLAYERTBL_MainMove( &p_wk->playertbl[i] );
			MNGM_PLAYER_PLATE_HBLANK_BGSCR_SetPlateScr( &p_wk->bgscrl, &p_wk->playertbl[i] );
			if( result == FALSE ){
				plate_end = FALSE;
			}
		}else{
			plate_end = FALSE;
		}
	}

	return plate_end;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�e�[�u��	������
 *
 *	@param	p_player		�v���C���[�e�[�u��
 *	@param	player_num		�v���C���[��
 *	@param	gametype		�Q�[���^�C�v
 *	@param	p_scrn			�X�N���[���f�[�^
 *	@param	p_bgl			BGL�Ǘ��V�X�e��	
 *	@param	idx				�C���f�b�N�X
 *	@param	p_clact			�Z���A�N�^�[
 *	@param	p_oamres		�Z�����\�[�X
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_Init( MNGM_PLATE_PLAYER* p_player, u32 player_num, u32 gametype, NNSG2dScreenData* p_scrn, MNGM_BGL* p_bgl, u32 idx, u32 myplayerid, MNGM_CLACT* p_clact, MNGM_CLACTRESOBJ* p_oamres, u32 heapID )
{
	s32 player_idx;
	// player_idx�ŃC���f�b�N�X�Q�Ƃ���e�[�u����
	// top��oam_anm�̃e�[�u��
	// �ʒu���ς��e�[�u����player_idx�ŎQ�Ƃ���

	// �ʒu�̓��[�U��ID�ŃC���f�b�N�X���ω�����
	switch( gametype ){
	case WFLBY_GAME_BALLSLOW:	// �ʓ���
	case WFLBY_GAME_BALLOON:	// �ӂ�������
		player_idx = MNGM_PLAYER_PLATE_BSBLIDX_Get( player_num, idx, myplayerid );
		break;
		
	// �ʏ�肾���A���т����Ȃ�ς�邽�ߕ��G�ɂȂ�B
	// �����͏�ɂO�̃C���f�b�N�X�����āA
	// ����ȊO�́A�����ɕ��Ԍ`�ɂȂ�
	case WFLBY_GAME_BALANCEBALL:		// �ʏ��
		if( myplayerid > idx ){		
			// ������菬�����v���C���[�͎����̕����炷
			player_idx = idx + 1;
		}else if( myplayerid == idx ){
			// �����͏��0
			player_idx = 0;
		}else{
			// �����ȍ~�̃v���C���[�͂��̂܂�
			player_idx = idx;
		}
//		player_idx = sc_MNGM_PLAYER_PLATE_BBIDX_DATA[ player_num-1 ][ player_idx ];
		break;

	default:
		player_idx = idx;
		break;
	}

	// �\���v���C�I���e�B�ݒ�
	GF_BGL_PrioritySet( GF_BGL_FRAME1_M, 2 );
	GF_BGL_PrioritySet( GF_BGL_FRAME0_M, 1 );
	
	// �Ƃ肠�������̐l�̗̈���쐬���āA�f�[�^����������
	GF_BGL_BmpWinAdd(
				p_bgl->p_bgl, &p_player->win, GF_BGL_FRAME0_M,
				MNGM_ENTRY_BMP_X, 
				MNGM_ENTRY_BMP_Y + sc_MNGM_PLAYER_PLATE_DATA[ player_num-1 ].top[ player_idx ], 
				MNGM_ENTRY_BMP_SIZX, MNGM_ENTRY_BMP_SIZY, MNGM_ENTRY_BMP_PAL,
				MNGM_ENTRY_BMP_CGX + ((MNGM_ENTRY_BMP_SIZX*MNGM_ENTRY_BMP_SIZY)*idx) );
	GF_BGL_BmpWinDataFill( &p_player->win, 0 );


	// �X�N���[������������
	GF_BGL_ScrWriteExpand(
				p_bgl->p_bgl, GF_BGL_FRAME1_M,
				0, sc_MNGM_PLAYER_PLATE_DATA[ player_num-1 ].top[ player_idx ], 
				MNGM_PLAYER_PLATE_SCRN_SIZ_X, MNGM_PLAYER_PLATE_SCRN_SIZ_Y,
				p_scrn->rawData, 0, 
				sc_MNGM_PLAYER_PLATE_DATA[ player_num-1 ].scrn_y[ idx ], 
				p_scrn->screenWidth/8,
				p_scrn->screenHeight/8 );


	// OAM�ݒ肷��
	{
		u8 anm;
		u8 x, y;

		anm = sc_MNGM_PLAYER_PLATE_DATA[ player_num-1 ].oam_anm[ player_idx ];
		x	= sc_MNGM_PLATE_CLACT_PLATE_POS[anm][0];
		y	= (sc_MNGM_PLAYER_PLATE_DATA[ player_num-1 ].top[ player_idx ]*8) + sc_MNGM_PLATE_CLACT_PLATE_POS[anm][1];
		p_player->p_clwk = MNGM_CLACTRES_Add( p_oamres, p_clact->p_clactset, x, y, 0, heapID );
		CLACT_AnmChg( p_player->p_clwk, anm );
		CLACT_PaletteOffsetChg( p_player->p_clwk, idx );
		CLACT_SetDrawFlag( p_player->p_clwk, FALSE );
		CLACT_SetAnmFlag( p_player->p_clwk, TRUE );
	}

	// RankOAM�ݒ�
	{
		u8 x, y;

		x	= MNGM_PLATE_CLACT_RANK_X;
		y	= (sc_MNGM_PLAYER_PLATE_DATA[ player_num-1 ].top[ player_idx ]*8) + MNGM_PLATE_CLACT_RANK_Y;
		p_player->p_rank = MNGM_CLACTRES_Add( p_oamres, p_clact->p_clactset, x, y, 0, heapID );
		CLACT_AnmChg( p_player->p_rank, 0 );
		CLACT_SetDrawFlag( p_player->p_rank, FALSE );
	}

	p_player->idx		= idx;
	p_player->plidx		= player_idx;
	p_player->playernum = player_num;

}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̖��O��`�悷��
 *
 *	@param	p_player		�v���C���[�e�[�u��
 *	@param	cp_status		���̐l�̃X�e�[�^�X�f�[�^
 *	@param	x				�\�������W
 *	@param	y				�\�������W
 *	@param	vip				������VIP���ǂ���
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_DrawName( MNGM_PLATE_PLAYER* p_player, MNGM_MSG* p_msg, const MYSTATUS* cp_status, s16 x, s16 y, u32 vip )
{
	MNGM_MSG_SetPlayerName( p_msg, cp_status );
	if( vip == TRUE ){
		MNGM_MSG_PrintColor( p_msg, msg_10, &p_player->win, x, y, MNGM_MSG_VIP_COLOR );
	}else{
		MNGM_MSG_Print( p_msg, msg_10, &p_player->win, x, y );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n����`�悷��
 *
 *	@param	p_player		�v���C���[�e�[�u��
 *	@param	p_msg			���b�Z�[�W�V�X�e��
 *	@param	draw_nation		������\������̂�
 *	@param	nation			��ID
 *	@param	area			�n��ID
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_DrawNation( MNGM_PLATE_PLAYER* p_player, MNGM_MSG* p_msg, BOOL draw_nation, u32 nation, u32 area, s16 x )
{
	// �o�g�n����������
	if(draw_nation){
		if( nation != country000 ){
			MNGM_MSG_ClearStrBuff( p_msg );
			MNGM_MSG_SetNationName( p_msg, nation );
			MNGM_MSG_Print( p_msg, msg_11, &p_player->win, x,16 );
		}
	}else{
		if( nation != country000 ){
			MNGM_MSG_ClearStrBuff( p_msg );
			MNGM_MSG_SetAreaName( p_msg, nation, area );
			MNGM_MSG_Print( p_msg, msg_12, &p_player->win, x,16 );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̐l�̏��ʂ���������
 *
 *	@param	p_player		�v���C���[�e�[�u��
 *	@param	rank			�����N
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_DrawRank( MNGM_PLATE_PLAYER* p_player, u32 rank )
{
	CLACT_SetDrawFlag( p_player->p_rank, TRUE );
	CLACT_AnmChg( p_player->p_rank, MNGM_PLAYER_OAM_ANMSEQ_TOP+rank );

	CLACT_SetAnmFlag( p_player->p_rank, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�R�A��\������
 *	
 *	@param	p_player		�v���C���[�e�[�u��
 *	@param	p_msg			���b�Z�[�W�V�X�e��
 *	@param	score			�X�R�A
 *	@param	x				�E�[x���W
 *	@param	y				y���W
 *	@param	keta			����
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_DrawScore( MNGM_PLATE_PLAYER* p_player, MNGM_MSG* p_msg, u32 score, s16 x, s16 y, u32 keta )
{
	MNGM_MSG_SetScore( p_msg, score, keta );
	if( keta == 5 ){
		MNGM_MSG_PrintRightSide( p_msg, msg_14, &p_player->win, x, y );
	}else{
		MNGM_MSG_PrintRightSide( p_msg, msg_13, &p_player->win, x, y );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ԃ�\������
 *	
 *	@param	p_player	�v���C���[�e�[�u��
 *	@param	p_msg		���b�Z�[�W�V�X�e��
 *	@param	frame		����
 *	@param	x			�E�[�����W
 *	@param	y			�����W
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_DrawTime( MNGM_PLATE_PLAYER* p_player, MNGM_MSG* p_msg, u32 frame, s16 x, s16 y )
{
	MNGM_MSG_SetTime( p_msg, frame );
	MNGM_MSG_PrintRightSide( p_msg, msg_13, &p_player->win, x, y );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�������ݖ��߂��o��
 *
 *	@param	p_player		�v���C���[�e�[�u��
 *	@param	p_bgl			BGL�V�X�e��
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_BgWriteVReq( MNGM_PLATE_PLAYER* p_player, MNGM_BGL* p_bgl )
{
	GF_BGL_BmpWinOnVReq( &p_player->win );
	GF_BGL_LoadScreenV_Req( p_bgl->p_bgl, GF_BGL_FRAME1_M );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����������}�[�N��\��
 *
 *	@param	p_player	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_DrawOamWay( MNGM_PLATE_PLAYER* p_player )
{
	CLACT_SetDrawFlag( p_player->p_clwk, TRUE );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�e�[�u��	�j��
 *
 *	@param	p_player		�v���C���[�e�[�u��
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_Delete( MNGM_PLATE_PLAYER* p_player )
{
	if( p_player->p_clwk ){
		CLACT_Delete( p_player->p_clwk );
		p_player->p_clwk = NULL;
	}
	if( p_player->p_rank ){
		CLACT_Delete( p_player->p_rank );
		p_player->p_rank = NULL;
	}
	if( GF_BGL_BmpWinAddCheck( &p_player->win ) == TRUE ){
		GF_BGL_BmpWinDel( &p_player->win );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	PLATE�̕\���ʒu�����ݒ�
 *
 *	@param	p_player		�v���C���[�e�[�u��
 *	@param	end_x			�I�[�ʒu�����W
 *	@param	wait			�E�G�C�g
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_StartMove( MNGM_PLATE_PLAYER* p_player, s16 end_x, s16 wait )
{
	MNGM_AddMoveReqFx( &p_player->draw_x, FX32_CONST(MNGM_PLATE_MOVE_START),
			FX32_CONST(end_x), FX32_CONST(MNGM_PLATE_MOVE_SPEED), MNGM_PLATE_MOVE_COUNT );
	p_player->draw_x_count	= 0;
	p_player->draw_x_wait	= wait;
	p_player->draw_x_snd	= MNGM_SND_BAR_IN;		// �炷��

	// OAM�ɂ����W��ݒ肷��
	MNGM_PLATE_PLAYERTBL_SetOamMatrix( p_player, -(p_player->draw_x.x>>FX32_SHIFT) );
}
//----------------------------------------------------------------------------
/**
 *	@brief	PLATE�̕\���ʒu	������Ƃ��납���ʒ[�Ɉړ�����
 *
 *	@param	p_player		�v���C���[�e�[�u��
 *	@param	wait			�E�G�C�g
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_StartMoveRet( MNGM_PLATE_PLAYER* p_player, s16 wait )
{
	MNGM_AddMoveReqFx( &p_player->draw_x, p_player->draw_x.x,
			FX32_CONST(MNGM_PLATE_MOVE_START), 0, MNGM_PLATE_MOVE_COUNT );
	p_player->draw_x_count	= 0;
	p_player->draw_x_wait	= wait;
	p_player->draw_x_snd	= MNGM_SND_BAR_OUT;		// �炷��

	// OAM�ɂ����W��ݒ肷��
	MNGM_PLATE_PLAYERTBL_SetOamMatrix( p_player, -(p_player->draw_x.x>>FX32_SHIFT) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	PLATE�\���ʒu����@���C��
 *
 *	@param	p_player	�v���C���[�e�[�u��
 *
 *	@retval	TRUE	����
 *	@retval	FLASE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_PLATE_PLAYERTBL_MainMove( MNGM_PLATE_PLAYER* p_player )
{
	BOOL ret;
	u8 x, y;
	if(p_player->draw_x_wait <= 0){
		if( p_player->draw_x_count == 0 ){
			// �ŏ��ɂP�񉹂��o��
			Snd_SePlay( p_player->draw_x_snd );
		}
		p_player->draw_x_count++;
		ret = MNGM_AddMoveMainFx( &p_player->draw_x, p_player->draw_x_count );

		// OAM�ɂ����W��ݒ肷��
		MNGM_PLATE_PLAYERTBL_SetOamMatrix( p_player, -(p_player->draw_x.x>>FX32_SHIFT) );
		return ret;
	}
	p_player->draw_x_wait--;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM�̍��W��ݒ�
 *
 *	@param	p_wk		�v���C���[�e�[�u�����[�N
 *	@param	draw_x		�`��ʒu
 */
//-----------------------------------------------------------------------------
static void MNGM_PLATE_PLAYERTBL_SetOamMatrix( MNGM_PLATE_PLAYER* p_wk, s32 draw_x )
{
	u8 anm;
	u8 x, y;
	VecFx32 mat;
	
	// ����
	anm = sc_MNGM_PLAYER_PLATE_DATA[ p_wk->playernum-1 ].oam_anm[ p_wk->plidx ];
	x	= sc_MNGM_PLATE_CLACT_PLATE_POS[anm][0];
	y	= (sc_MNGM_PLAYER_PLATE_DATA[ p_wk->playernum-1 ].top[ p_wk->plidx ]*8) + sc_MNGM_PLATE_CLACT_PLATE_POS[anm][1];
	
	mat.x = (x+draw_x)<<FX32_SHIFT;
	mat.y = y<<FX32_SHIFT;
	CLACT_SetMatrix( p_wk->p_clwk, &mat );


	// �����L���O
	x	= MNGM_PLATE_CLACT_RANK_X;
	y	= (sc_MNGM_PLAYER_PLATE_DATA[ p_wk->playernum-1 ].top[ p_wk->plidx ]*8) + MNGM_PLATE_CLACT_RANK_Y;
	
	mat.x = (x+draw_x)<<FX32_SHIFT;
	mat.y = y<<FX32_SHIFT;
	CLACT_SetMatrix( p_wk->p_rank, &mat );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[PLATE
 *
 *	@param	playernum		�v���C���[��
 *	@param	playerid		�\������l��ID
 *	@param	myplayerid		������ID
 *
 *	@return	���̐l���Q�Ƃ���ׂ�sc_MNGM_PLAYER_PLATE_DATA�̃C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static u32 MNGM_PLAYER_PLATE_BSBLIDX_Get( u32 playernum, u32 playerid, u32 myplayerid )
{
	s32 myidx, plidx, playeridx;
	int i;

	// �����̃e�[�u���C���f�b�N�X�ƕ\������l�̃e�[�u���C���f�b�N�X���擾
	for( i=0; i<playernum; i++ ){
		if( sc_MNGM_PLAYER_PLATE_BSBLIDX_DATA[ playernum-1 ][ i ] == playerid ){
			plidx = i;
		}
		if( sc_MNGM_PLAYER_PLATE_BSBLIDX_DATA[ playernum-1 ][ i ] == myplayerid ){
			myidx = i;
		}
	}

	playeridx = plidx - myidx;
	if( playeridx < 0 ){
		playeridx += playernum;
	}

	return playeridx;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[PLATE	HBLANK����	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_bgl		BGL�Ǘ��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_Init( MNGM_HBLANK_PLATEBGSCR* p_wk, MNGM_BGL* p_bgl, u32 heapID )
{
	int i, j;
	
	GF_ASSERT( p_wk->p_laster == NULL );

	p_wk->p_bgl = p_bgl;

	// �����X�N���[�����W�ݒ�
	GF_BGL_ScrollSet( p_wk->p_bgl->p_bgl, GF_BGL_FRAME0_M, GF_BGL_SCROLL_X_SET, MNGM_PLATE_MOVE_START );
	GF_BGL_ScrollSet( p_wk->p_bgl->p_bgl, GF_BGL_FRAME1_M, GF_BGL_SCROLL_X_SET, MNGM_PLATE_MOVE_START );

	// buffer�̒l��������
	for( i=0; i<2; i++ ){
		MI_CpuFill16( &p_wk->scrll_x[i], MNGM_PLATE_MOVE_START, sizeof(s16)*WFLBY_HBLANK_BGSCR_BUFNUM );
	}

	p_wk->p_laster = LASTER_Init( heapID, &p_wk->scrll_x[0], &p_wk->scrll_x[1] );

	// �ŏ��̓X�C�b�`bufferOFF
	LASTER_SwitchFlagSet( p_wk->p_laster, LASTER_SWITCH_OFF );

	// HBLANK�֐��ݒ�
	sys_HBlankIntrSet( MNGM_PLAYER_PLATE_HBLANK_BGSCR_HBlank, p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[PLATE�@HBLANK����	�j��
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_Exit( MNGM_HBLANK_PLATEBGSCR* p_wk )
{
	if( p_wk->p_laster != NULL ){
		sys_HBlankIntrSet( NULL, NULL );

		LASTER_Delete( p_wk->p_laster );
		p_wk->p_laster = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[PLATE	HBLANK����	�l�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_plate	PLATE���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_SetPlateScr( MNGM_HBLANK_PLATEBGSCR* p_wk, const MNGM_PLATE_PLAYER* cp_plate )
{
	s16* p_buff;
	int i;
	int idx;

	GF_ASSERT( p_wk->p_laster != NULL );

	p_buff = LASTER_GetWriteBuff( p_wk->p_laster );

	//  ���W�ݒ�
	for( i=0; i<MNGM_PLAYER_PLATE_SCRN_SIZ_Y*8; i++ ){
		idx = i-1;
		idx += sc_MNGM_PLAYER_PLATE_DATA[ cp_plate->playernum-1 ].top[ cp_plate->plidx ]*8;
		if( idx < 0 ){
			idx += WFLBY_HBLANK_BGSCR_BUFNUM;
		}
		p_buff[idx] = cp_plate->draw_x.x>>FX32_SHIFT;
	}

	// buffer�������������̂ŃX�C�b�`buffer
	LASTER_SwitchFlagSet( p_wk->p_laster, LASTER_SWITCH_ON );
}

//----------------------------------------------------------------------------
/**
 *	@brief	HBLANK�V�X�e��VBLANK����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_VBlank( MNGM_HBLANK_PLATEBGSCR* p_wk )
{
	if( p_wk->p_laster == NULL ){
		return ;
	}

	// buffer�̃X�C�b�`���O
	LASTER_VBlank( p_wk->p_laster );

	// buffer���X�C�b�`�����̂Ŏ��̏������������܂ŃX�C�b�`OFF
	LASTER_SwitchFlagSet( p_wk->p_laster, LASTER_SWITCH_OFF );
}

//----------------------------------------------------------------------------
/**
 *	@brief	HBLANK����
 *
 *	@param	p_work
 */
//-----------------------------------------------------------------------------
static void MNGM_PLAYER_PLATE_HBLANK_BGSCR_HBlank( void* p_work )
{
	MNGM_HBLANK_PLATEBGSCR* p_wk = p_work;
	int v_count;
	const s16* cp_buff;
	
	v_count = GX_GetVCount();
	if( v_count >= WFLBY_HBLANK_BGSCR_BUFNUM ){
		return ;
	}

	cp_buff = LASTER_GetReadBuff( p_wk->p_laster );

#if PL_T0849_080709_FIX
	if( GX_IsHBlank() ){
		GF_BGL_ScrollSet( p_wk->p_bgl->p_bgl, GF_BGL_FRAME0_M, GF_BGL_SCROLL_X_SET, cp_buff[v_count] );
		GF_BGL_ScrollSet( p_wk->p_bgl->p_bgl, GF_BGL_FRAME1_M, GF_BGL_SCROLL_X_SET, cp_buff[v_count] );
	}
#else
	GF_BGL_ScrollSet( p_wk->p_bgl->p_bgl, GF_BGL_FRAME0_M, GF_BGL_SCROLL_X_SET, cp_buff[v_count] );
	GF_BGL_ScrollSet( p_wk->p_bgl->p_bgl, GF_BGL_FRAME1_M, GF_BGL_SCROLL_X_SET, cp_buff[v_count] );
#endif
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM�v���C���[�f�[�^��ݒ�
 *
 *	@param	p_wk			���[�N
 *	@param	cp_comm_param	�v���C���[ID�ɑΉ�����NETID
 */
//-----------------------------------------------------------------------------
static void MNGM_COMMPDATA_Init( MNGM_COMM_PDATA* p_wk, const MNGM_ENRES_PARAM* cp_comm_param )
{
	int i;

	for( i=0; i<cp_comm_param->num; i++ ){
		p_wk->cp_status[i]	= MNGM_ENRES_PARAM_GetMystatus( cp_comm_param, cp_comm_param->pnetid[i] );
		GF_ASSERT(  p_wk->cp_status[i] );
		p_wk->nation[i]		= CommInfoGetMyNation( cp_comm_param->pnetid[i] );
		p_wk->area[i]		= CommInfoGetMyArea( cp_comm_param->pnetid[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ŕ\������̂��`�F�b�N
 *
 *	@param	cp_wk	�p�����[�^
 *	@param	cp_data	�ʐM��{�f�[�^
 *
 *	@retval	TRUE	������\��
 *	@retval	FALSE	�n�於��\��
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_COMMPARAM_CheckDrawNation( const MNGM_ENRES_PARAM* cp_wk, const MNGM_COMM_PDATA* cp_data )
{
	int i;
	BOOL draw_nation;	// ��ID�Œn���\�����邩

	draw_nation = FALSE;
	for( i=0; i<cp_wk->num-1; i++ ){
		// �݂�Ȃ̍����Ⴄ���A�����_�~�[�icountry000�j�̂Ƃ������ŕ\��
		if( (cp_data->nation[i] != cp_data->nation[i+1]) || 
			(cp_data->nation[i] == country000) || 
			(cp_data->nation[i+1] == country000) ){
			draw_nation = TRUE;
		}
	}

	return draw_nation;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�A�b�v�f�[�g
 *
 *	@param	data	���̃K�W�F�b�g�f�[�^
 *
 *	@return	Update��̃K�W�F�b�g�f�[�^
 */
//-----------------------------------------------------------------------------
WFLBY_ITEMTYPE MNGM_ITEM_Update( WFLBY_ITEMTYPE data )
{
	u32 gadget_type;
	u32 gadget_offs;
	WFLBY_ITEMTYPE ret_gadget;

	gadget_offs = data % WFLBY_ITEM_DANKAI;
	gadget_type = data / WFLBY_ITEM_DANKAI;
	if( (gadget_offs+1) < WFLBY_ITEM_DANKAI ){
		gadget_offs ++;
	}
	
	ret_gadget = gadget_type * WFLBY_ITEM_DANKAI;
	ret_gadget += gadget_offs;

	return ret_gadget;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g�����S	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_bglwk		BGL���[�N
 *	@param	gametype	�Q�[���^�C�v
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_TITLELOGO_Init( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk, MNGM_MSG* p_msg, u32 gametype, ARCHANDLE* p_handle, u32 heapID )
{
	GF_BGL_BmpWinAdd( p_bglwk->p_bgl, &p_wk->bmp, GF_BGL_FRAME1_M,
			MNGM_TITLELOGO_BMP_X, MNGM_TITLELOGO_BMP_Y,
			MNGM_TITLELOGO_BMP_SIZX, MNGM_TITLELOGO_BMP_SIZY,
			MNGM_TITLELOGO_BMP_PAL, MNGM_TITLELOGO_BMP_CGX);
	GF_BGL_BmpWinDataFill( &p_wk->bmp, 0 );

	// ���b�Z�[�W�𐶐�
	p_wk->p_str = STRBUF_Create( MNGM_MSG_STRBUF_NUM, heapID );
	MNGM_MSG_SetGameName( p_msg, gametype );
	MNGM_MSG_GetStr( p_msg, p_wk->p_str, msg_16 );

	p_wk->gametype = gametype;

	// �`��OFF
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );	

	// �L�����N�^�X�N���[���]��
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wlmngm_tool_minigame_win_poke_NCGR,
			p_bglwk->p_bgl, 
			GF_BGL_FRAME0_M, 0, 0,
			FALSE, heapID );
	ArcUtil_HDL_ScrnSet( p_handle, NARC_wlmngm_tool_minigame_win_bg2_NSCR, p_bglwk->p_bgl,
			GF_BGL_FRAME0_M, 0, 0, FALSE, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g�����S	�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_TITLELOGO_Exit( MNGM_TITLE_LOGO* p_wk )
{
	GF_BGL_BmpWinDel( &p_wk->bmp );

	STRBUF_Delete( p_wk->p_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���S�̓��ꏈ���J�n
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */	
//-----------------------------------------------------------------------------
static void MNGM_TITLELOGO_InStart( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk, u32 heapID )
{
	u32 x_size;
	u32 draw_x;

	// ����p�����[�^�̏�����
	p_wk->count = 0;
	p_wk->seq = 0;
	MNGM_AddMoveReqFx( &p_wk->move, 
			sc_MNGM_TITLELOGO_MOVE00_START[p_wk->gametype], 
			sc_MNGM_TITLELOGO_MOVE00_END[p_wk->gametype],
			sc_MNGM_TITLELOGO_MOVE00_SPEED[p_wk->gametype],
			MNGM_TITLELOGO_MOVE00_COUNTMAX );

	// �����ʒu�ݒ�
	MNGM_TITLELOGO_SetMoveMatrix( p_wk, p_bglwk, p_wk->move.x>>FX32_SHIFT );


	
	// �{�^���t�H���g�ŏ�������
	FontProc_LoadFont( FONT_BUTTON, heapID );	//�{�^���t�H���g�̃��[�h

	// �����ɕ\������
	x_size	= FontProc_GetPrintStrWidth( FONT_BUTTON, p_wk->p_str, 0 );
	draw_x	= (MNGM_TITLELOGO_BMP_SIZXDOT/2) - (x_size/2);

	GF_STR_PrintColor(
		&p_wk->bmp, FONT_BUTTON, p_wk->p_str, draw_x,
		0, MSG_ALLPUT, sc_MNGM_TITLELOGO_BMP_COL[ p_wk->gametype ], NULL );

	FontProc_UnloadFont( FONT_BUTTON );				//�{�^���t�H���g�̔j��
	
	// �`��ON
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ޏ�̏����J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_TITLELOGO_OutStart( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk )
{
	// ����p�����[�^�̏�����
	p_wk->count = 0;
	p_wk->seq = 0;
	MNGM_AddMoveReqFx( &p_wk->move, 
			sc_MNGM_TITLELOGO_MOVE00_END[p_wk->gametype], 
			sc_MNGM_TITLELOGO_MOVE03_END[p_wk->gametype],
			sc_MNGM_TITLELOGO_MOVE03_SPEED[p_wk->gametype],
			MNGM_TITLELOGO_MOVE03_COUNTMAX );

	// �����ʒu�ݒ�
	MNGM_TITLELOGO_SetMoveMatrixVReq( p_wk, p_bglwk, p_wk->move.x>>FX32_SHIFT );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g�����S	���C��
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_TITLELOGO_InMain( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk )
{
	BOOL result;
	
	switch( p_wk->seq ){
	case MNGM_TITLELOGO_INMOVESEQ_IN:
		result = MNGM_AddMoveMainFx( &p_wk->move, p_wk->count );
		MNGM_TITLELOGO_SetMoveMatrixVReq( p_wk, p_bglwk, p_wk->move.x>>FX32_SHIFT );
		p_wk->count ++;
		if( result == TRUE ){
			//  
			Snd_SePlay( MNGM_SND_TITLE );

			// ���̓���̏�����
			MNGM_AddMoveReqFx( &p_wk->move, 
					sc_MNGM_TITLELOGO_MOVE00_END[p_wk->gametype], 
					sc_MNGM_TITLELOGO_MOVE01_END[p_wk->gametype],
					sc_MNGM_TITLELOGO_MOVE01_SPEED[p_wk->gametype],
					MNGM_TITLELOGO_MOVE01_COUNTMAX );
			p_wk->seq++;	
			p_wk->count = 0;
		}
		break;

	case MNGM_TITLELOGO_INMOVESEQ_BOUND00:
		result = MNGM_AddMoveMainFx( &p_wk->move, p_wk->count );
		MNGM_TITLELOGO_SetMoveMatrixVReq( p_wk, p_bglwk, p_wk->move.x>>FX32_SHIFT );
		p_wk->count ++;
		if( result == TRUE ){
			// ���̓���̏�����
			MNGM_AddMoveReqFx( &p_wk->move, 
					sc_MNGM_TITLELOGO_MOVE01_END[p_wk->gametype], 
					sc_MNGM_TITLELOGO_MOVE00_END[p_wk->gametype],
					sc_MNGM_TITLELOGO_MOVE02_SPEED[p_wk->gametype],
					MNGM_TITLELOGO_MOVE02_COUNTMAX );
			p_wk->seq++;	
			p_wk->count = 0;
		}
		break;

	case MNGM_TITLELOGO_INMOVESEQ_BOUND01:
		result = MNGM_AddMoveMainFx( &p_wk->move, p_wk->count );
		MNGM_TITLELOGO_SetMoveMatrixVReq( p_wk, p_bglwk, p_wk->move.x>>FX32_SHIFT );
		p_wk->count ++;
		if( result == TRUE ){
			p_wk->seq++;
		}
		break;

	case MNGM_TITLELOGO_INMOVESEQ_END:
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g�����S	�A�E�g���C��
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_TITLELOGO_OutMain( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk )
{
	BOOL result;
	
	switch( p_wk->seq ){
	case MNGM_TITLELOGO_OUTMOVESEQ_OUT:
		result = MNGM_AddMoveMainFx( &p_wk->move, p_wk->count );
		MNGM_TITLELOGO_SetMoveMatrixVReq( p_wk, p_bglwk, p_wk->move.x>>FX32_SHIFT );
		p_wk->count ++;
		if( result == TRUE ){
			p_wk->seq++;
		}
		break;
		
	case MNGM_TITLELOGO_OUTMOVESEQ_END:
		GF_BGL_BmpWinOff( &p_wk->bmp );

		// �`��OFF
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );	

		// �����ʒu�ݒ�
		MNGM_TITLELOGO_SetMoveMatrix( p_wk, p_bglwk, 0 );

		// BG�ʂ��N���[��
		GF_BGL_ScrClear( p_bglwk->p_bgl, GF_BGL_FRAME0_M );
		GF_BGL_ScrClear( p_bglwk->p_bgl, GF_BGL_FRAME1_M );

		// �`��ON
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );	
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	������W��ݒ肷��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_TITLELOGO_SetMoveMatrix( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk, s32 num )
{
	switch( p_wk->gametype ){

	case WFLBY_GAME_BALLSLOW:	// �ʓ���
	case WFLBY_GAME_BALANCEBALL:	// �ʏ��
		GF_BGL_ScrollSet( p_bglwk->p_bgl, GF_BGL_FRAME0_M, 
				GF_BGL_SCROLL_X_SET, num );
		GF_BGL_ScrollSet( p_bglwk->p_bgl, GF_BGL_FRAME1_M, 
				GF_BGL_SCROLL_X_SET, num );
		break;

	case WFLBY_GAME_BALLOON:		// �ӂ�������
		GF_BGL_ScrollSet( p_bglwk->p_bgl, GF_BGL_FRAME0_M, 
				GF_BGL_SCROLL_Y_SET, num );
		GF_BGL_ScrollSet( p_bglwk->p_bgl, GF_BGL_FRAME1_M, 
				GF_BGL_SCROLL_Y_SET, num );
		break;

	default:
		GF_ASSERT(0);
		break;
	}
}
static void MNGM_TITLELOGO_SetMoveMatrixVReq( MNGM_TITLE_LOGO* p_wk, MNGM_BGL* p_bglwk, s32 num )
{
	switch( p_wk->gametype ){

	case WFLBY_GAME_BALLSLOW:	// �ʓ���
	case WFLBY_GAME_BALANCEBALL:	// �ʏ��
		GF_BGL_ScrollReq( p_bglwk->p_bgl, GF_BGL_FRAME0_M, 
				GF_BGL_SCROLL_X_SET, num );
		GF_BGL_ScrollReq( p_bglwk->p_bgl, GF_BGL_FRAME1_M, 
				GF_BGL_SCROLL_X_SET, num );
		break;

	case WFLBY_GAME_BALLOON:		// �ӂ�������
		GF_BGL_ScrollReq( p_bglwk->p_bgl, GF_BGL_FRAME0_M, 
				GF_BGL_SCROLL_Y_SET, num );
		GF_BGL_ScrollReq( p_bglwk->p_bgl, GF_BGL_FRAME1_M, 
				GF_BGL_SCROLL_Y_SET, num );
		break;

	default:
		GF_ASSERT(0);
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�w�i�p���b�g�]��
 *
 *	@param	p_handle		�n���h��
 *	@param	gametype		�Q�[���^�C�v
 */
//-----------------------------------------------------------------------------
static void MNGM_BACKPLTT_Trans( ARCHANDLE* p_handle, u32 gametype, u32 heapID )
{
	u32 palnum;
	//  �Q�[���^�C�v�ɍ��킹�Ĕw�i�F��ς���
	switch( gametype ){
	case WFLBY_GAME_BALLSLOW:	// �ʓ���
		palnum = MNGM_PLAYER_BG_ANMPLTT_BALLSLOW;
		break;
	case WFLBY_GAME_BALANCEBALL:	// �ʏ��
		palnum = MNGM_PLAYER_BG_ANMPLTT_BALANCE;
		break;
	case WFLBY_GAME_BALLOON:		// �ӂ�������
		palnum = MNGM_PLAYER_BG_ANMPLTT_BALLOON;
		break;

	default:
		palnum = MNGM_PLAYER_BG_ANMPLTT_BALLSLOW;
		break;
	}

	ArcUtil_HDL_PalSetEx( p_handle, NARC_wlmngm_tool_minigame_win_ani_NCLR, 
			PALTYPE_MAIN_BG, palnum*32, MNGM_PLAYER_BG_PLTT_BACK*32, 32, heapID );

	ArcUtil_HDL_PalSetEx( p_handle, NARC_wlmngm_tool_minigame_win_ani_NCLR, 
			PALTYPE_SUB_BG, palnum*32, MNGM_PLAYER_BG_PLTT_BACK*32, 32, heapID );
}




//-------------------------------------
///	���U���g��ʃ��[�N
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	���U���g��ʃ��[�N	�쐬
 *
 *	@param	cp_commparam		���ʃp�����[�^
 *	@param	cp_param			�p�����[�^
 *	@param	heapID				�q�[�vID
 */
//-----------------------------------------------------------------------------
static MNGM_RESULTWK* MNGM_RESULT_CommonInit( const MNGM_ENRES_PARAM* cp_commparam, const MNGM_RESULT_PARAM* cp_param, u32 gametype, u32 heapID )
{
	MNGM_RESULTWK* p_wk;
	int i;

	p_wk = sys_AllocMemory( heapID, sizeof(MNGM_RESULTWK) );
	memset( p_wk, 0, sizeof(MNGM_RESULTWK) );

	// �p�����[�^���R�s�[
	memcpy( &p_wk->param, cp_param, sizeof(MNGM_RESULT_PARAM) );
	memcpy( &p_wk->comm_param, cp_commparam, sizeof(MNGM_ENRES_PARAM) );

	// �Q�[���^�C�v�ݒ�
	p_wk->gametype = gametype;

	// �ʐM�R�}���h�ݒ�
	CommCommandMNGMInitialize( p_wk );

	// ���W�X�^���Z�b�g
	G2_BlendNone();
	G2S_BlendNone();
	GX_SetVisibleWnd(0);
	GXS_SetVisibleWnd(0);
	

	// �o���N�ݒ�
	GF_Disp_SetBank( &sc_MNGM_RESULT_BANK );

	// �q�[�vID�ۑ�
	p_wk->heapID = heapID;

	// �ʐM�f�[�^��ݒ�
	MNGM_COMMPDATA_Init( &p_wk->comm_pdata, &p_wk->comm_param );

	// VramTransferManager������
	initVramTransferManagerHeap( MNGM_VRAMTR_WK_NUM, heapID );


	// BG
	MNGM_BGL_Init( &p_wk->bgl, &sc_MNGM_RESULT_BGINIT, sc_MNGM_RESULT_BGCNT, MNGM_RESULT_BGL_USE, heapID );

	// OAM
	MNGM_CLACT_Init( &p_wk->clact, MNGM_RESULT_CLACT_WKNUM, MNGM_RESULT_CLACT_CHARNUM, MNGM_RESULT_CLACT_PLTTNUM, heapID );

	// CLACT
	MNGM_CLACTRES_Init( &p_wk->clres, MNGM_RESULT_CLACT_RESNUM, heapID );

	// ���b�Z�[�W�V�X�e���쐬
	MNGM_MSG_Init( &p_wk->msg, heapID );	

	// �O���t�B�b�N�ǂݍ���
	MNGM_RESULT_GraphicLoad( p_wk, heapID );

	// ���b�Z�[�W�V�X�e��������
	{
		BOOL vip;
		u32 netid;

		netid = MNGM_ENRES_PARAM_GetPlNOtoNetID( &p_wk->comm_param, p_wk->comm_param.my_playerid );
		vip = MNGM_ENRES_PARAM_GetVipFlag( &p_wk->comm_param, netid );
		MNGM_TALKWIN_Init( &p_wk->talkwin, &p_wk->bgl, p_wk->comm_param.p_save, 
				vip,  heapID );
	}

	// ���g���C���[�N�쐬
	MNGM_RESULT_Retry_Init( &p_wk->retrysys, &p_wk->bgl, p_wk->comm_param.vchat, heapID );

	// ��ID�Œn���\�����邩�`�F�b�N
	{
		int i;

		// �����ŕ\�����邩�`�F�b�N
		p_wk->draw_nation = MNGM_COMMPARAM_CheckDrawNation( &p_wk->comm_param, &p_wk->comm_pdata );
 
	}

	// �Q�[���ɂ���ĕύX���镔��
	// �v���C���[ID�Ŕz�u��ύX���邩�ݒ�
	// �^�X�N����
	switch( p_wk->gametype ){
	case WFLBY_GAME_BALLSLOW:	// �ʓ���
	case WFLBY_GAME_BALANCEBALL:	// �ʏ��
		p_wk->tcb			 = TCB_Add( MNGM_RESULT_Tcb_BallSlowBalanceBall, p_wk, 0 );
		break;
		
	case WFLBY_GAME_BALLOON:		// �ӂ�������
		p_wk->tcb			 = TCB_Add( MNGM_RESULT_Tcb_Balloon, p_wk, 0 );
		break;

	default:
		GF_ASSERT(0);	// ��������
		p_wk->tcb			 = TCB_Add( MNGM_RESULT_Tcb_BallSlowBalanceBall, p_wk, 0 );
		break;
	}
		
	p_wk->tcb_vwait = VWaitTCB_Add( MNGM_RESULT_VWaitTcb, p_wk, 0 );

	// VCHAT ON
	if( p_wk->comm_param.vchat ){
		// �{�C�X�`���b�g�J�n
		mydwc_startvchat( heapID );
	}
	
	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	3D�V�X�e��������
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_3DInit( MNGM_3DSYS* p_wk, u32 heapID )
{
	p_wk->p_3dman = GF_G3DMAN_Init( heapID, GF_G3DMAN_LNK, GF_G3DTEX_128K, 
				GF_G3DMAN_LNK, GF_G3DPLT_64K, NULL );
	Particle_SystemWorkInit();		// �p�[�e�B�N��������
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�V�X�e���j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_3DExit( MNGM_3DSYS* p_wk )
{
	Particle_SystemExitAll();		//  �p�[�e�B�N���j��

    GF_G3D_Exit( p_wk->p_3dman );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�V�X�e�����C��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_3DMain( MNGM_3DSYS* p_wk )
{
	Particle_CalcAll();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�V�X�e���`��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_3DDraw( MNGM_3DSYS* p_wk )
{
    //�R�c�`��J�n
    GF_G3X_Reset();
	
	// �p�[�e�B�N���\������
	Particle_DrawAll();

    /* �W�I���g���������_�����O�G���W���֘A�������̃X���b�v */
    GF_G3_RequestSwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�V�X�e��	�p�[�e�B�N���A�j���ǂݍ���
 *
 *	@param	p_wk		���[�N
 *	@param	p_handle	�n���h��
 *	@param	dataidx		�f�[�^�C���f�b�N�X
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_3DAnmLoad( MNGM_3DSYS* p_wk, ARCHANDLE* p_handle, u32 dataidx, u32 heapID )
{
	void* p_res;
	GF_CAMERA_PTR p_camera;

	// �p�[�e�B�N�����[�N�쐬
	p_wk->p_ptc_work = sys_AllocMemory( heapID, PARTICLE_LIB_HEAP_SIZE );
	p_wk->p_ptc = Particle_SystemCreate( 
			NULL, NULL, 
			p_wk->p_ptc_work, PARTICLE_LIB_HEAP_SIZE, 
			TRUE, heapID );
	GF_ASSERT( p_wk->p_ptc );


	// �J�����ݒ�
	Particle_CameraTypeSet(p_wk->p_ptc, GF_CAMERA_ORTHO);
	p_camera = Particle_GetCameraPtr( p_wk->p_ptc );
	GFC_SetCameraClip(FX32_ONE, FX32_ONE * 900, p_camera);

	// ���\�[�X�ǂݍ��݁��ݒ�
	p_res = ArcUtil_HDL_Load(p_handle, dataidx, FALSE, heapID, ALLOC_TOP);
	Particle_ResourceSet(p_wk->p_ptc, p_res, PTC_AUTOTEX_LNK|PTC_AUTOPLTT_LNK, FALSE);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�V�X�e��	�p�[�e�B�N���A�j���j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_3DAnmRelease( MNGM_3DSYS* p_wk )
{
	Particle_SystemExit( p_wk->p_ptc );
	sys_FreeMemoryEz( p_wk->p_ptc_work );
	p_wk->p_ptc			= NULL;
	p_wk->p_ptc_work	= NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�V�X�e��	�p�[�e�B�N���A�j���J�n
 *
 *	@param	p_wk		���[�N
 *	@param	emitnum		�G�~�b�^��
 *	@param	p_pos		�ʒu
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_3DAnmStart( MNGM_3DSYS* p_wk, u32 emitnum )
{
	int i;
	VecFx32 pos = {0,0,0};

	// �G�~�b�^�̓o�^
	Particle_CreateEmitter( p_wk->p_ptc, emitnum, &pos );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�V�X�e��	�p�[�e�B�N���A�j��	�I���`�F�b�N
 *
 *	@param	cp_wk		���[�N
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_RESULT_3DAnmEndCheck( const MNGM_3DSYS* cp_wk )
{
	// ���쒆�̃G�~�b�^���Ȃ��Ȃ�Γ���I��
	if( Particle_GetEmitterNum(cp_wk->p_ptc) == 0 ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���U���g��ʃO���t�B�b�N�ǂݍ���
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_GraphicLoad( MNGM_RESULTWK* p_wk, u32 heapID )
{
	ARCHANDLE* p_handle;

	p_handle = ArchiveDataHandleOpen( ARC_WLMNGM_TOOL_GRA, heapID );

	// �p���b�g
	ArcUtil_HDL_PalSet( p_handle, NARC_wlmngm_tool_minigame_win_NCLR,
			PALTYPE_MAIN_BG, 0, MNGM_BGPLTT_NUM*32, heapID );
	ArcUtil_HDL_PalSet( p_handle, NARC_wlmngm_tool_minigame_win_NCLR,
			PALTYPE_SUB_BG, 0, MNGM_BGPLTT_NUM*32, heapID );
	
	// �L�����N�^	
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wlmngm_tool_minigame_win_bg_NCGR,
			p_wk->bgl.p_bgl, 
			GF_BGL_FRAME1_M, 0, 0,
			FALSE, heapID );
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wlmngm_tool_minigame_win_bg_NCGR,
			p_wk->bgl.p_bgl, 
			GF_BGL_FRAME1_S, 0, 0x4000,
			FALSE, heapID );

	// �X�N���[��
	{
		int i;
		for( i=0; i<MNGM_RESULT_SCRN_NUM; i++ ){
			p_wk->p_scrnbuf[i] =  ArcUtil_HDL_ScrnDataGet( p_handle,
					NARC_wlmngm_tool_minigame_win_bg1_NSCR+i,
					FALSE, &p_wk->p_scrn[i], heapID );
		}
	}

	// �w�i�ݒ�
	ArcUtil_HDL_ScrnSet( p_handle, NARC_wlmngm_tool_minigame_win_bg0_NSCR, p_wk->bgl.p_bgl,
			GF_BGL_FRAME3_M, 0, 0, FALSE, heapID );
	ArcUtil_HDL_ScrnSet( p_handle, NARC_wlmngm_tool_minigame_win_bg0_NSCR, p_wk->bgl.p_bgl,
			GF_BGL_FRAME1_S, 0, 0, FALSE, heapID );

	// �w�i�p�p���b�g�]��
	MNGM_BACKPLTT_Trans( p_handle, p_wk->gametype, heapID );


	// OAM��ǂݍ���
	p_wk->p_resobj = MNGM_CLACTRES_Load( &p_wk->clres, p_handle, 
			NARC_wlmngm_tool_minigame_win_oam_NCLR, MNGM_PLAYER_OAM_PLTT_NUM,
			NARC_wlmngm_tool_minigame_win_oam_NCGR,
			NARC_wlmngm_tool_minigame_win_oam_NCER,
			NARC_wlmngm_tool_minigame_win_oam_NANR,
			MNGM_RESCONTID, heapID );

	// �p���b�g�A�j���G�t�F�N�g
	MNGM_RESULT_PalTrEffectInit( p_wk, p_handle, heapID );


	// BALLOON
	MNGM_RESULT_Balloon_GraphicInit( &p_wk->balloon, &p_wk->bgl, &p_wk->clact,
			&p_wk->clres, p_handle, heapID );

	ArchiveDataHandleClose( p_handle );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ʉ�ʗp�O���t�B�b�N�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_GraphicDelete( MNGM_RESULTWK* p_wk )
{
	// BALLOON
	MNGM_RESULT_Balloon_GraphicExit( &p_wk->balloon );
	
	// �X�N���[����j��
	{
		int i;
		for( i=0; i<MNGM_RESULT_SCRN_NUM; i++ ){
			sys_FreeMemoryEz( p_wk->p_scrnbuf[i] );
		}
	}

	// �p���b�g�A�j���G�t�F�N�g��j��
	MNGM_RESULT_PalTrEffectExit( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ʉ�ʃ��C���^�X�N
 *
 *	@param	tcb
 *	@param	p_work 
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_Tcb_BallSlowBalanceBall( TCB_PTR tcb, void* p_work )
{
	MNGM_RESULTWK* p_wk = p_work;
	BOOL result;

	switch( p_wk->seq ){
	case MNGM_RESULT_BALLSLOW_SEQ_WIPEIN_INIT:

		// �A�b�v�f�[�g�O�̃K�W�F�b�g�����߂�
		p_wk->last_gadget = p_wk->comm_param.p_lobby_wk->gadget;

		// replay�t���O��TRUE�Ȃ�WiFi�F�B�����Ȃ̂ŃK�W�F�b�g�̓A�b�v�f�[�g���Ȃ�
		if( p_wk->comm_param.replay == TRUE ){
			p_wk->gadget_update = FALSE;
		}else{
			u32 notouch_score;

			// �������ĂȂ��Ƃ��̃X�R�A���擾
			notouch_score = MNGM_RESULT_BallslowBalanceGetNoTouchScore( p_wk );

			OS_TPrintf( "score %d\n", p_wk->param.score[ p_wk->comm_param.my_playerid ] );
			
			// �K�W�F�b�g�A�b�v�f�[�g�`�F�b�N
			// ���������ʂP�ʂ�������K�W�F�b�g���A�b�v�f�[�g
			// �X�R�A��notouch_score���傫���K�v������
			if( (p_wk->param.result[ p_wk->comm_param.my_playerid ] == 0) && 
				(p_wk->param.score[ p_wk->comm_param.my_playerid ] > notouch_score) ){
				// �K�W�F�b�g�A�b�v�f�[�g
				p_wk->comm_param.p_lobby_wk->gadget = MNGM_ITEM_Update( p_wk->comm_param.p_lobby_wk->gadget );
				p_wk->gadget_update = TRUE;

			}else{
				p_wk->gadget_update = FALSE;
			}

			// �e�̓g�s�b�N�쐬
			if( p_wk->comm_param.my_playerid == 0 ){
				MNGM_RESULT_BallslowBalance_SendMinigameTopResult( p_wk );
			}
		}



		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEIN, 
				WIPE_TYPE_FADEIN, WIPE_FADE_OUTCOLOR, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, p_wk->heapID );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLSLOW_SEQ_WIPEIN_WAIT:
		result = WIPE_SYS_EndCheck();
		if( result ){
			p_wk->seq ++;
		}
		break;

	case MNGM_RESULT_BALLSLOW_SEQ_MSGON_INIT:
		MNGM_TALKWIN_MsgPrint( &p_wk->talkwin, &p_wk->msg, msg_01, MNGM_TALKWIN_IDX_MAIN );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLSLOW_SEQ_MSGON_WAIT:
		result = MNGM_TALKWIN_MsgEndCheck( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );
		if( result ){
			p_wk->wait = MNGM_ENTRY_MSGWAIT;
			p_wk->seq ++;
		}
		break;

	case MNGM_RESULT_BALLSLOW_SEQ_MSGON_WAIT2:
		if( p_wk->wait > 0 ){
			p_wk->wait --;
			if( p_wk->wait == 0 ){
				int i;
				BOOL all_score_0 = TRUE;
				u32 notouch_score;


				MNGM_TALKWIN_MsgOff( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );

				// HBLANK�V�X�e��������
				MNGM_PLAYER_PLATE_HBLANK_BGSCR_Init( &p_wk->bgscrl, &p_wk->bgl, p_wk->heapID );

				// �������ĂȂ��Ƃ��̃X�R�A���擾
				notouch_score = MNGM_RESULT_BallslowBalanceGetNoTouchScore( p_wk );

				// �S���̃X�R�A��notouch_score�̂Ƃ��͑S����p_wk->comm_param.num�ʂƂ��ĕ\������
				for( i=0; i<p_wk->comm_param.num; i++ ){
					if( p_wk->param.score[ i ] > notouch_score ){
						all_score_0 = FALSE;
					}
				}
				if( all_score_0 == TRUE ){
					p_wk->draw_result_wait	= MNGM_RESULT_DRAWON_WAIT_34;
					p_wk->seq				= MNGM_RESULT_BALLSLOW_SEQ_ALLSCORE0_WAIT;
				}else{

					// �����łȂ���΂S�ʂ���\��
					p_wk->seq ++;
					p_wk->draw_result = 4;
				}
			}
		}
		break;
		
	// ���ɕ\�����鏇�ʂ���������
	case MNGM_RESULT_BALLSLOW_SEQ_COUNTDOWN_INIT:
		{
			int i;
			BOOL sarch;
			
			sarch = FALSE;
			do{
				p_wk->draw_result --;	// ���̕\�����ʂɂ���
				// ���ɕ\�����鏇�ʂ���������
				for( i=0; i<p_wk->comm_param.num; i++ ){
					if( p_wk->param.result[ i ] == p_wk->draw_result ){
						sarch = TRUE;;
					}
				}
			}while( sarch == FALSE );

			// �\���܂ł̃E�G�C�g��ݒ�
			switch( p_wk->draw_result ){
			case 3:
			case 2:
				p_wk->draw_result_wait = MNGM_RESULT_DRAWON_WAIT_34;
				break;

			case 1:
			case 0:
				p_wk->draw_result_wait = MNGM_RESULT_DRAWON_WAIT_12;
				break;

			default :
				GF_ASSERT(0);	// ��������
				break;
			}

			// �e�[�u������
			MNGM_RESULT_PlayerTblMove( p_wk );

			p_wk->seq ++;
		}
		break;

	// ���ʂ̕\��
	// 2�ʂ�1�ʂ͓����ɕ\������
	case MNGM_RESULT_BALLSLOW_SEQ_COUNTDOWN:
		p_wk->draw_result_wait --;

		// �e�[�u������
		MNGM_RESULT_PlayerTblMove( p_wk );

		if( p_wk->draw_result_wait > 0 ){
			break;
		}
		
		// �\������
		{
			int i;
			BOOL draw;

			for( i=0; i<p_wk->comm_param.num; i++ ){
				draw = FALSE;
				if( p_wk->param.result[i] == p_wk->draw_result ){
					draw = TRUE;
				}else{
					if( (p_wk->draw_result == 1) && 
						(p_wk->param.result[i] == 0) ){
						draw = TRUE;
					}
				}

				// �\��
				if( draw ){
					MNGM_RESULT_BallslowBalancePlayerTblDraw( p_wk, i, p_wk->param.result[i] );
				}
			}
		}

		// �܂�2�ʂ�\�����ĂȂ���Α�����
		if( p_wk->draw_result > 1 ){
			p_wk->seq  = MNGM_RESULT_BALLSLOW_SEQ_COUNTDOWN_INIT;
		}else{
			p_wk->seq ++;
		}

		break;

	// �e�[�u�����삪�S���I���̂�҂�
	case MNGM_RESULT_BALLSLOW_SEQ_TBLMOVE_WAIT:
		if( MNGM_RESULT_PlayerTblMove( p_wk ) ){
			p_wk->seq++;
		}
		break;
		
	case MNGM_RESULT_BALLSLOW_SEQ_EFFECT_ON:
		p_wk->seq ++;

		// 1�ʂ̐l�̏ꏊ�ɃG�t�F�N�g���o��
		MNGM_RESULT_PalTrEffectStart( p_wk );

		p_wk->wait = MNGM_RESULT_RESULT_DRAW_WAIT;
		break;

	case MNGM_RESULT_BALLSLOW_SEQ_EFFECT_WAIT:

		if( p_wk->wait > 0 ){
			p_wk->wait --;
		}
		if( p_wk->wait == 0 ){	

			p_wk->seq = MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG00_ON;
			p_wk->wait = 0;
		}
		break;

	// ���ʂɑ΂��郁�b�Z�[�W
	case MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG00_ON:
		{
			u32 msgid;

			// ���ʃ��b�Z�[�W
			if( p_wk->param.result[ p_wk->comm_param.my_playerid ] == 0 ){
				msgid = msg_17;
			}else{
				msgid = msg_18;
			}

			// �g���[�i���ݒ�
			MNGM_MSG_SetPlayerName( &p_wk->msg, 
					p_wk->comm_pdata.cp_status[ p_wk->comm_param.my_playerid ] );
			MNGM_TALKWIN_MsgPrint( &p_wk->talkwin, &p_wk->msg, msgid, MNGM_TALKWIN_IDX_MAIN );
		}
		p_wk->seq ++;
		break;

	case MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG00_WAIT:
		result = MNGM_TALKWIN_MsgEndCheck( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );
		if( result ){

			if( MNGM_RESULT_SndTopMeEndWait( p_wk ) == TRUE ){

				// WiFi�N���u�Ȃ�K�W�F�b�g�̂��Ƃ͂���Ȃ�
				if( p_wk->comm_param.replay == TRUE ){
					p_wk->wait	= MNGM_RESULT_MSGDRAW_WAIT;	// �`��E�G�C�g
					p_wk->seq	= MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG_WAIT2;
				}else{
					p_wk->wait	= MNGM_RESULT_MSGDRAW_WAIT;
					p_wk->seq	++;
				}
			}
		}
		break;

	// �K�W�F�b�g���b�Z�[�W����
	case MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG_ON:

		// ���b�Z�[�W�X�V�҂�
		if( p_wk->wait > 0 ){
			p_wk->wait --;
			break;
		}

		{
			u32 msgid;

			// �K�W�F�b�g���������x��MAX�ŃA�b�v���Ȃ������Ƃ�
			if( p_wk->gadget_update ){

				// �K�W�F�b�g�ݒ�
				MNGM_MSG_SetGadget( &p_wk->msg, p_wk->comm_param.p_lobby_wk->gadget );
				if( p_wk->last_gadget == p_wk->comm_param.p_lobby_wk->gadget ){
					msgid = msg_19;
				}else{
					msgid = msg_02;
				}
			}else{

				// WiFi�L��Ȃ̂ŃK�W�F�b�g���A�b�v���Ȃ�����
				msgid = msg_09;
			}

			// �g���[�i���ݒ�
			MNGM_MSG_SetPlayerName( &p_wk->msg, 
					p_wk->comm_pdata.cp_status[ p_wk->comm_param.my_playerid ] );
			MNGM_TALKWIN_MsgPrint( &p_wk->talkwin, &p_wk->msg, msgid, MNGM_TALKWIN_IDX_MAIN );
		}
		p_wk->seq  = MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG_WAIT;
		break;
		
	case MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG_WAIT:
		result = MNGM_TALKWIN_MsgEndCheck( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );
		if( result ){
			p_wk->wait = MNGM_RESULT_MSGDRAW_WAIT;
			p_wk->seq ++;
		}
		break;

	case MNGM_RESULT_BALLSLOW_SEQ_GADGET_MSG_WAIT2:
		if( p_wk->wait > 0 ){
			p_wk->wait --;
			if( p_wk->wait == 0 ){
				MNGM_TALKWIN_MsgOff( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );
				p_wk->wait = MNGM_RESULT_RESULT_DRAW_WAIT;
				p_wk->seq ++;

			}
		}
		break;

	// �ʐM�����J�n
	case MNGM_RESULT_BALLSLOW_SEQ_SYNCSTART:
#ifdef DEBUG_SYNCSTART_A
		if( sys.trg & PAD_BUTTON_A ){
			p_wk->seq ++;
			CommTimingSyncStart( MNGM_SYNC_RESULT_END );

			// VCHAT OFF
			if( p_wk->comm_param.vchat ){
				// �{�C�X�`���b�g�I��
				mydwc_stopvchat();
			}
		}
#else

		if( p_wk->wait > 0 ){
			p_wk->wait--;
		}else{
		
			CommTimingSyncStart( MNGM_SYNC_RESULT_END );

			// VCHAT OFF
			if( p_wk->comm_param.vchat ){
				// �{�C�X�`���b�g�I��
				mydwc_stopvchat();
			}
			p_wk->seq ++;
		}	
#endif

		break;
		
	case MNGM_RESULT_BALLSLOW_SEQ_SYNCWAIT:
		if( CommIsTimingSync( MNGM_SYNC_RESULT_END ) ){

			// ���g���C�𕷂��Ȃ烊�g���C��
			if( p_wk->comm_param.replay ){
				p_wk->seq = MNGM_RESULT_BALLSLOW_SEQ_RETRY_INIT;
			}else{
				p_wk->seq ++;
			}
		}
		break;
		
	case MNGM_RESULT_BALLSLOW_SEQ_WIPEOUT_INIT:
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEOUT, 
				WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, p_wk->heapID );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLSLOW_SEQ_WIPEOUT_WAIT:
		result = WIPE_SYS_EndCheck();
		if( result ){
			p_wk->seq  = MNGM_RESULT_BALLSLOW_SEQ_END;
		}
		break;

	// �Ē���𕷂��ꍇ
	case MNGM_RESULT_BALLSLOW_SEQ_RETRY_INIT:

		// �V�񂾉񐔂�ݒ�
		MNGM_RESULT_SetPlayNum( p_wk );
		
		p_wk->seq ++;
		break;

	case MNGM_RESULT_BALLSLOW_SEQ_RETRY_MAIN:
		result = MNGM_RESULT_Retry_Main( &p_wk->retrysys, &p_wk->talkwin, &p_wk->msg, p_wk->gametype, p_wk->heapID );
		if( result ){
			// ���g���C�ݒ�
			p_wk->replay = MNGM_RESULT_RetryGet( &p_wk->retrysys );
			p_wk->seq ++;
		}
		break;
		
	case MNGM_RESULT_BALLSLOW_SEQ_END:

		// HBLANK�V�X�e���j��
		MNGM_PLAYER_PLATE_HBLANK_BGSCR_Exit( &p_wk->bgscrl );
		
		p_wk->end_flag = TRUE;
		break;


	// �S���̃X�R�A���O�̂Ƃ��̏���
	case MNGM_RESULT_BALLSLOW_SEQ_ALLSCORE0_WAIT:
		p_wk->draw_result_wait --;
		if( p_wk->draw_result_wait <= 0 ){
			p_wk->seq ++;
		}
		break;
		
	case MNGM_RESULT_BALLSLOW_SEQ_ALLSCORE0_DRAW:
		{
			int i;
			
			// �`��
			for( i=0; i<p_wk->comm_param.num; i++ ){
				// ���ʂ�����������
				p_wk->param.result[i] = p_wk->comm_param.num-1;
				MNGM_RESULT_BallslowBalancePlayerTblDraw( p_wk, i, p_wk->param.result[i] );
			}

			// �G�t�F�N�g�`��ɖ߂�
			p_wk->seq = MNGM_RESULT_BALLSLOW_SEQ_TBLMOVE_WAIT;
		}
		break;

	}

	// �p���b�g�G�t�F�N�g����
	MNGM_RESULT_PalTrEffect( p_wk );	// 1�ʂ̃X�N���[�������G�t�F�N�g


	// CLACT�`��
	MNGM_CLACT_Draw( &p_wk->clact );


	// �X�N���[���ʂ��X�N���[��������
	GF_BGL_ScrollReq( p_wk->bgl.p_bgl, GF_BGL_FRAME3_M, 
			GF_BGL_SCROLL_Y_INC, MNGM_BGSCROLL_SPEED );
	GF_BGL_ScrollReq( p_wk->bgl.p_bgl, GF_BGL_FRAME1_S, 
			GF_BGL_SCROLL_Y_INC, MNGM_BGSCROLL_SPEED );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���D���p�^�X�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_Tcb_Balloon( TCB_PTR tcb, void* p_work )
{
	MNGM_RESULTWK* p_wk = p_work;
	BOOL result;

	switch( p_wk->seq ){
	case MNGM_RESULT_BALLOON_SEQ_WIPEIN_INIT:

		// replay�t���O��TRUE�Ȃ�WiFi�F�B�����Ȃ̂ŃK�W�F�b�g�̓A�b�v�f�[�g���Ȃ�
		if( p_wk->comm_param.replay == TRUE ){
			p_wk->gadget_update = FALSE;
		}else{
			// �K�W�F�b�g�A�b�v�f�[�g�`�F�b�N
			if( p_wk->param.balloon >= MNGM_BALLOON_GADGET_UPDATE ){
				// �K�W�F�b�g�A�b�v�f�[�g
				p_wk->comm_param.p_lobby_wk->gadget = MNGM_ITEM_Update( p_wk->comm_param.p_lobby_wk->gadget );
				p_wk->gadget_update = TRUE;
			}else{
				p_wk->gadget_update = FALSE;
			}

			// �e�Ȃ猋�ʂ̃j���[�X���쐬
			if( p_wk->comm_param.my_playerid == 0 ){
				MNGM_RESULT_Balloon_SendMinigameTopResult( p_wk );
			}
		}

		
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEIN, 
				WIPE_TYPE_FADEIN, WIPE_FADE_OUTCOLOR, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, p_wk->heapID );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_WIPEIN_WAIT:
		result = WIPE_SYS_EndCheck();
		if( result ){
			p_wk->seq ++;

			p_wk->draw_result = 4;
		}
		break;

	case MNGM_RESULT_BALLOON_SEQ_MSGON_INIT:
		MNGM_TALKWIN_MsgPrint( &p_wk->talkwin, &p_wk->msg, msg_01, MNGM_TALKWIN_IDX_MAIN );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_MSGON_WAIT:
		result = MNGM_TALKWIN_MsgEndCheck( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );
		if( result ){
			p_wk->wait = MNGM_RESULT_MSGDRAW_WAIT;
			p_wk->seq ++;
		}
		break;

	case MNGM_RESULT_BALLOON_SEQ_MSGON_WAIT2:
		if( p_wk->wait > 0 ){
			p_wk->wait --;
			if( p_wk->wait == 0 ){
				MNGM_TALKWIN_MsgOff( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );
				p_wk->seq ++;
			}
		}
		break;
		
	// ���D�\������
	case MNGM_RESULT_BALLOON_SEQ_TBLON:
		{
			int i;
			BOOL  vip;
			u32 netid;

			MNGM_PLAYER_PLATE_HBLANK_BGSCR_Init( &p_wk->bgscrl, &p_wk->bgl, p_wk->heapID );
			
			for( i=0; i<p_wk->comm_param.num; i++ ){
				MNGM_PLATE_PLAYERTBL_Init( &p_wk->playertbl[i], p_wk->comm_param.num, 
						p_wk->gametype,
						p_wk->p_scrn[ MNGM_RESULT_SCRN_TBL ], &p_wk->bgl, i, p_wk->comm_param.my_playerid,
						&p_wk->clact, p_wk->p_resobj, 
						p_wk->heapID );

				// ���O�\��
				netid = MNGM_ENRES_PARAM_GetPlNOtoNetID( &p_wk->comm_param, i );
				vip = MNGM_ENRES_PARAM_GetVipFlag( &p_wk->comm_param, netid );
				MNGM_PLATE_PLAYERTBL_DrawName( &p_wk->playertbl[i], &p_wk->msg, p_wk->comm_pdata.cp_status[i], MNGM_RESULT_DRAWNAME_X, 0, vip );
				// �����\��
				MNGM_PLATE_PLAYERTBL_DrawNation( &p_wk->playertbl[i], &p_wk->msg, 
						p_wk->draw_nation, p_wk->comm_pdata.nation[i],
						p_wk->comm_pdata.area[i], MNGM_RESULT_DRAWNAME_X );

				// �\��
				MNGM_PLATE_PLAYERTBL_BgWriteVReq( &p_wk->playertbl[i], &p_wk->bgl );
				MNGM_PLATE_PLAYERTBL_DrawOamWay( &p_wk->playertbl[i] );

				// ����p�����[�^������
				MNGM_PLATE_PLAYERTBL_StartMove( &p_wk->playertbl[i], MNGM_PLATE_MOVE_ENTRY_END, MNGM_PLATE_MOVE_WAIT_ENTRY*i );
			}
		}
		p_wk->seq ++;
		p_wk->wait = MNGM_RESULT_BALLOON_DRAW_WAIT;
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_BALLOON_ON:
		result = MNGM_RESULT_PlayerTblMove( p_wk );
		if( result == TRUE ){

			p_wk->wait --;
			if( p_wk->wait <= 0 ){
				MNGM_RESULT_Balloon_Start( &p_wk->balloon, p_wk->param.balloon );
				p_wk->seq ++;
			}
		}
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_BALLOON_ONWAIT:
		result = MNGM_RESULT_Balloon_Main( &p_wk->balloon, &p_wk->bgl );
		if( result == TRUE ){

			if( p_wk->gadget_update == TRUE ){
				MNGM_RESULT_SndTopMePlay( p_wk );
			}

			p_wk->seq ++;
			
		}
		break;

	// ���D������������\��
	case MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_00ON:
		MNGM_MSG_SetBalloonNum( &p_wk->msg, p_wk->param.balloon );
		MNGM_TALKWIN_MsgPrint( &p_wk->talkwin, &p_wk->msg, msg_07, MNGM_TALKWIN_IDX_MAIN );
		p_wk->wait = MNGM_RESULT_MSGDRAW_WAIT;
		p_wk->seq ++;
		break;

	case MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_00WAIT:
		result = MNGM_TALKWIN_MsgEndCheck( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );
		if( result == TRUE ){

			if( p_wk->wait <= 0 ){

				if( MNGM_RESULT_SndTopMeEndWait( p_wk ) == TRUE ){
					
					// WiFi�N���u�̂Ƃ��̓K�W�F�b�g�͊֌W�Ȃ�
					if( p_wk->comm_param.replay == TRUE ){
						p_wk->wait = MNGM_RESULT_MSGDRAW_WAIT;
						p_wk->seq = MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_WAIT2;
					}else{
						p_wk->seq ++;
					}
				}	
			}else{
				p_wk->wait --;
			}
		}
		break;

	// �K�W�F�b�g���b�Z�[�W����
	case MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_ON:
		{
			u32 msg_no;
			if( p_wk->gadget_update ){
				msg_no = msg_08;
			}else{
				msg_no = msg_09;
			}
			MNGM_TALKWIN_MsgPrint( &p_wk->talkwin, &p_wk->msg, msg_no, MNGM_TALKWIN_IDX_MAIN );
		}
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_WAIT:
		result = MNGM_TALKWIN_MsgEndCheck( &p_wk->talkwin, MNGM_TALKWIN_IDX_MAIN );
		if( result ){
			p_wk->wait = MNGM_RESULT_MSGDRAW_WAIT;

			if( p_wk->gadget_update ){
				p_wk->seq  = MNGM_RESULT_BALLOON_SEQ_EFFECT_WAIT;
			}else{
				p_wk->seq ++;
			}
		}
		break;

	// �G�t�F�N�g���o���Ȃ��Ƃ��̃E�G�C�g
	case MNGM_RESULT_BALLOON_SEQ_GADGET_MSG_WAIT2:
		if( p_wk->wait > 0 ){
			p_wk->wait --;
			if( p_wk->wait == 0 ){
				p_wk->seq  = MNGM_RESULT_BALLOON_SEQ_SYNCSTART;
			}
		}
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_EFFECT_WAIT:
		
		// �o�ߎ��Ԃ��v�Z
		if( p_wk->wait > 0 ){
			p_wk->wait --;
		}
		
		if( p_wk->wait == 0 ){
			p_wk->seq  = MNGM_RESULT_BALLOON_SEQ_SYNCSTART;
		}
		break;

	// �ʐM�����J�n
	case MNGM_RESULT_BALLOON_SEQ_SYNCSTART:
		// VCHAT OFF
		if( p_wk->comm_param.vchat ){
			// �{�C�X�`���b�g�I��
			mydwc_stopvchat();
		}
		CommTimingSyncStart( MNGM_SYNC_RESULT_END );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_SYNCWAIT:
		if( CommIsTimingSync( MNGM_SYNC_RESULT_END ) ){

			// ���g���C�𕷂�p_wk->draw_nation�Ȃ烊�g���C��
			if( p_wk->comm_param.replay ){

				p_wk->seq = MNGM_RESULT_BALLOON_SEQ_RETRY_INIT;
			}else{
				p_wk->seq ++;
			}
		}
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_WIPEOUT_INIT:
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEOUT, 
				WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, p_wk->heapID );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_WIPEOUT_WAIT:
		result = WIPE_SYS_EndCheck();
		if( result ){
			p_wk->seq  = MNGM_RESULT_BALLOON_SEQ_END;
		}
		break;

	// �Ē���𕷂��ꍇ
	case MNGM_RESULT_BALLOON_SEQ_RETRY_INIT:

		// �V�񂾉񐔂�ݒ�
		MNGM_RESULT_SetPlayNum( p_wk );
		
		p_wk->seq ++;
		break;

	case MNGM_RESULT_BALLOON_SEQ_RETRY_MAIN:
		result = MNGM_RESULT_Retry_Main( &p_wk->retrysys, &p_wk->talkwin, &p_wk->msg, p_wk->gametype, p_wk->heapID );
		if( result ){
			// ���g���C�ݒ�
			p_wk->replay = MNGM_RESULT_RetryGet( &p_wk->retrysys );
			p_wk->seq ++;
		}
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_END:

		MNGM_PLAYER_PLATE_HBLANK_BGSCR_Exit( &p_wk->bgscrl );
		p_wk->end_flag = TRUE;
		break;
	}

	// �p���b�g�G�t�F�N�g����
	MNGM_RESULT_PalTrEffect( p_wk );	// 1�ʂ̃X�N���[�������G�t�F�N�g

	// CLACT�`��
	MNGM_CLACT_Draw( &p_wk->clact );


	// �X�N���[���ʂ��X�N���[��������
	GF_BGL_ScrollReq( p_wk->bgl.p_bgl, GF_BGL_FRAME3_M, 
			GF_BGL_SCROLL_Y_INC, MNGM_BGSCROLL_SPEED );
	GF_BGL_ScrollReq( p_wk->bgl.p_bgl, GF_BGL_FRAME1_S, 
			GF_BGL_SCROLL_Y_INC, MNGM_BGSCROLL_SPEED );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ʉ��VWait�^�X�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_VWaitTcb( TCB_PTR tcb, void* p_work )
{
	MNGM_RESULTWK* p_wk = p_work;


    // Vram�]���}�l�[�W���[���s
    DoVramTransferManager();

	// BG��VBLANK����
    MNGM_BGL_VBlank( &p_wk->bgl );

	// OAM��VBLANK����
	MNGM_CLACT_VBlank( &p_wk->clact );

	// HBLANK�V�X�e��
	MNGM_PLAYER_PLATE_HBLANK_BGSCR_VBlank( &p_wk->bgscrl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�]���A�j���G�t�F�N�g	������
 *
 *	@param	p_wk			���[�N
 *	@param	p_handle		�n���h��
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_PalTrEffectInit( MNGM_RESULTWK* p_wk, ARCHANDLE* p_handle, u32 heapID )
{
	// �p���b�g�f�[�^�擾
	p_wk->p_plttbuf = ArcUtil_HDL_PalDataGet( p_handle, 
			NARC_wlmngm_tool_minigame_win_ani_NCLR, 
			&p_wk->p_pltt, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G�t�F�N�g���J�n
 *
 *	@param	p_wk			���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_PalTrEffectStart( MNGM_RESULTWK* p_wk )
{
	int i;
	
	p_wk->pltt_start = TRUE;

	// 1�ʂ������特��炷
	for( i=0; i<p_wk->comm_param.num; i++ ){
		if( p_wk->param.result[i] == 0 ){

			// ���ꂪ�����Ȃ特��炷
			if( p_wk->comm_param.my_playerid == i ){
				MNGM_RESULT_SndTopMePlay( p_wk );
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�]���A�j���G�t�F�N�g	���C��
 *
 *	@param	p_wk			���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_PalTrEffect( MNGM_RESULTWK* p_wk )
{
	int i;
	u32 tr_addr;	// �������ݐ�A�h���X
	u8* p_rd_addr;	// �ǂݍ��ݐ�A�h���X
	BOOL result;

	if( p_wk->pltt_start == FALSE ){
		return;
	}

	// �]�����ԃ`�F�b�N
	if( (p_wk->pltt_time == 0) || 
		(p_wk->pltt_time == MNGM_RESULT_PALANM_FRAME) ){

		for( i=0; i<p_wk->comm_param.num; i++ ){
			if( p_wk->param.result[i] == 0 ){	// 1�ʂ̂Ƃ������]������

				tr_addr		= (sc_MNGM_BG_PLTT_PLNO_TBL[i]*32);
				p_rd_addr	= p_wk->p_pltt->pRawData; 

				// �]���f�[�^�A�h���X������
				if( p_wk->pltt_time == 0 ){
					p_rd_addr += ((MNGM_PLAYER_BG_ANMPLTT_TOPANM0_0+sc_MNGM_BG_PLTT_PLNO_TBL[i])*32);
				}else{
					p_rd_addr += ((MNGM_PLAYER_BG_ANMPLTT_TOPANM1_0+sc_MNGM_BG_PLTT_PLNO_TBL[i])*32);
				}
				
				// �Ή������e�[�u���̃p���b�g��]��
				result = AddVramTransferManager( NNS_GFD_DST_2D_BG_PLTT_MAIN, 
						tr_addr, p_rd_addr, 32 );
				GF_ASSERT( result );	// �^�X�N�ݒ肪�ł��Ȃ�����
			}	
		}

	}

	p_wk->pltt_time = (p_wk->pltt_time + 1) % (MNGM_RESULT_PALANM_FRAME*2);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�]���A�j���G�t�F�N�g	�j��
 *
 *	@param	p_wk			���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_PalTrEffectExit( MNGM_RESULTWK* p_wk )
{
	sys_FreeMemoryEz( p_wk->p_plttbuf );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���g���C�V�X�e��������
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_Retry_Init( MNGM_RETRY_WK* p_wk, MNGM_BGL* p_bgl, u32 vchat, u32 heapID )
{

	memset( p_wk, 0, sizeof(MNGM_RETRY_WK) );
	
	p_wk->param.p_bgl		= p_bgl->p_bgl;
	p_wk->param.bg_frame	= GF_BGL_FRAME0_S;
	p_wk->param.char_offs	= MNGM_RESULT_RETRY_CHAR_OFFS;
	p_wk->param.pltt_offs = MNGM_RESULT_RETRY_PLTT_OFFS;
	p_wk->param.x			= MNGM_RESULT_RETRY_X;
	p_wk->param.y			= MNGM_RESULT_RETRY_Y;
	p_wk->p_ts = TOUCH_SW_AllocWork( heapID );

	p_wk->vchat	 = vchat;


	// ���g���C��M�f�[�^
	p_wk->recv_replay = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���g���C�V�X�e��	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_Retry_Exit( MNGM_RETRY_WK* p_wk )
{
	TOUCH_SW_FreeWork( p_wk->p_ts );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���g���C�V�X�e��	���C��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_RESULT_Retry_Main( MNGM_RETRY_WK* p_wk, MNGM_TALKWIN* p_talkwin, MNGM_MSG* p_msg, u32 gametype, u32 heapID )
{
	u32 result;
	
	switch( p_wk->seq ){
	case MNGM_RESULT_RETRY_SEQ_WIPEIN:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, 
				WIPE_TYPE_FADEIN, WIPE_FADE_BLACK,
				WIPE_DEF_DIV, WIPE_DEF_SYNC, heapID );

		// �T�u�ɃA�C�R���𑗐M
		WirelessIconEasy_HoldLCD( FALSE, heapID );

		// VCHAT ON
		if( p_wk->vchat ){
			// �{�C�X�`���b�g�J�n
			mydwc_startvchat( heapID );
		}
		p_wk->seq++;
		break;
		
	case MNGM_RESULT_RETRY_SEQ_WIPEINWAIT:
		if( WIPE_SYS_EndCheck() ){
			p_wk->seq ++;
		}
		break;
		
	case MNGM_RESULT_RETRY_SEQ_MSGON:
		MNGM_MSG_SetGameName( p_msg, gametype );
		MNGM_TALKWIN_MsgPrint( p_talkwin, p_msg, msg_03, MNGM_TALKWIN_IDX_SUB );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_RETRY_SEQ_MSGWAIT:
		result = MNGM_TALKWIN_MsgEndCheck( p_talkwin, MNGM_TALKWIN_IDX_SUB );
		if( result == TRUE ){
			p_wk->seq ++;
		}
		break;
		
	case MNGM_RESULT_RETRY_SEQ_TPON:	
		TOUCH_SW_Init( p_wk->p_ts, &p_wk->param );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_RETRY_SEQ_TPMAIN:
		result = TOUCH_SW_Main( p_wk->p_ts );

		// AUTO�v���C
#ifdef DEBUG_MINIGAME_AUTO_PLAY
		result = TOUCH_SW_RET_YES;
#endif
		
		if( (result == TOUCH_SW_RET_YES) ||
			(result == TOUCH_SW_RET_NO)	){

			
			if( result == TOUCH_SW_RET_YES ){
				CommSendData( CNM_MNGM_RETRY_YES, NULL, 0 );
			}else if( result == TOUCH_SW_RET_NO ){
				CommSendData( CNM_MNGM_RETRY_NO, NULL, 0 );
			}

			MNGM_TALKWIN_MsgPrint( p_talkwin, p_msg, msg_15, MNGM_TALKWIN_IDX_SUB );
			MNGM_TALKWIN_SetTimeWork( p_talkwin, MNGM_TALKWIN_IDX_SUB );
			p_wk->seq ++;

		}

		break;

	case MNGM_RESULT_RETRY_SEQ_RECV:			// �݂�Ȃ̑I������M
		if( p_wk->ko_recv ){
			if( p_wk->ko_recv_replay ){
				p_wk->seq = MNGM_RESULT_RETRY_SEQ_WIPEOUT;	// OK
			}else{
				p_wk->seq ++;
			}
		}
		break;
		
	case MNGM_RESULT_RETRY_SEQ_RECVMSGON:	// �N������߂��I�񂾂�������E�E�E
		MNGM_TALKWIN_MsgPrint( p_talkwin, p_msg, msg_06, MNGM_TALKWIN_IDX_SUB );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_RETRY_SEQ_RECVMSGWAIT:	// 
		result = MNGM_TALKWIN_MsgEndCheck( p_talkwin, MNGM_TALKWIN_IDX_SUB );
		if( result == TRUE ){
			p_wk->seq ++;
			p_wk->wait = MNGM_RESULT_MSGDRAW_WAIT;
		}
		break;

	case MNGM_RESULT_RETRY_SEQ_RECVMSGWAIT2:
		p_wk->wait --;
		if( p_wk->wait == 0 ){
			p_wk->seq ++;
		}
		break;
		
	case MNGM_RESULT_RETRY_SEQ_WIPEOUT:		// ���C�v�A�E�g
		WIPE_SYS_Start( WIPE_PATTERN_S, WIPE_TYPE_FADEOUT, 
				WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK,
				WIPE_DEF_DIV, WIPE_DEF_SYNC, heapID );
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_RETRY_SEQ_WIPEOUTWAIT:	// ���C�v�A�E�g�E�G�C�g
		if( WIPE_SYS_EndCheck() ){

			// ���C���ɃA�C�R���𑗐M
			WirelessIconEasyEnd();

			// VCHAT OFF
			if( p_wk->vchat ){
				// �{�C�X�`���b�g�I��
				mydwc_stopvchat();
			}
			p_wk->seq ++;
		}
		break;

	case MNGM_RESULT_RETRY_SEQ_END:
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���g���C���邩���擾����
 *
 *	@param	cp_wk	���[�N
 *	
 *	@retval	TRUE	���g���C����
 *	@retval	FALSE	���g���C���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_RESULT_RetryGet( const MNGM_RETRY_WK* cp_wk )
{
	return cp_wk->ko_recv_replay;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���g���C�e�̎�M����
 *
 *	@param	p_wk		���[�N
 *	@param	netid	�v���C���[�i���o�[
 *	@param	replay		replay
 *	@param	playernum	�v���C���[��
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_RetryOyaRecv( MNGM_RETRY_WK* p_wk, u32 netid, BOOL replay, u32 playernum )
{
	// ������M���Ė������`�F�b�N
	if( (p_wk->recv_num & (1<<netid)) == 0 ){

		if( replay == FALSE ){	
			p_wk->recv_replay = FALSE;
		}
		
		// ��M����
		p_wk->recv_num |= (1<<netid);
	}

	// �݂�Ȃ����M�������H
	{
		int i;
		int recv_num;

		recv_num = 0;
		for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
			if( (p_wk->recv_num & (1<<i)) ){
				recv_num ++;
			}
		}

		// �݂�Ȃ���󂯎������replay���邩���M
		if( recv_num == playernum ){
			if( p_wk->recv_replay ){
				CommSendData( CNM_MNGM_RETRY_OK, NULL, 0 );
			}else{
				CommSendData( CNM_MNGM_RETRY_NG, NULL, 0 );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e���烊�g���C����M
 *
 *	@param	p_wk		���[�N
 *	@param	replay		���g���C
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_RetryKoRecv( MNGM_RETRY_WK* p_wk, BOOL replay )
{
	p_wk->ko_recv = TRUE;
	p_wk->ko_recv_replay = replay;
}

//----------------------------------------------------------------------------
/**
 *	@brief	BALLOON�O���t�B�b�N������
 *
 *	@param	p_wk		���[�N
 *	@param	p_bgl		BGL�V�X�e��
 *	@param	p_clact		�Z���A�N�^�[
 *	@param	p_clres		���\�[�X�V�X�e��
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_Balloon_GraphicInit( MNGM_BALLOON_WK* p_wk, MNGM_BGL* p_bgl, MNGM_CLACT* p_clact, MNGM_CLACTRES* p_clres, ARCHANDLE* p_handle, u32 heapID )
{
	memset( p_wk, 0, sizeof(MNGM_BALLOON_WK) );

/*
	//	�V�X�e���E�B���h�E
	{
		// �L�����N�^�A�p���b�g�]��
		MenuWinGraphicSet( p_bgl->p_bgl, GF_BGL_FRAME2_M, MNGM_RESULT_BALLOON_SYSWIN_CGX,
			MNGM_RESULT_BALLOON_SYSWIN_PAL, MENU_TYPE_SYSTEM, heapID );

		// �E�B���h�E�쐬
		GF_BGL_BmpWinAdd( p_bgl->p_bgl, &p_wk->win, GF_BGL_FRAME2_M,
				MNGM_RESULT_BALLOON_BMP_X, MNGM_RESULT_BALLOON_BMP_Y,
				MNGM_RESULT_BALLOON_BMP_SIZX, MNGM_RESULT_BALLOON_BMP_SIZY,
				MNGM_RESULT_BALLOON_BMP_PAL, MNGM_RESULT_BALLOON_BMP_CGX);
		GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
	}
//*/

	//�@�Ǝ��E�B���h�E
	{
		ArcUtil_HDL_PalSet( p_handle, NARC_wlmngm_tool_mini_fusen_score_NCLR, PALTYPE_MAIN_BG, 
				MNGM_RESULT_BALLOON_SYSWIN_PAL*32, 32, heapID );
		ArcUtil_HDL_BgCharSet( p_handle, NARC_wlmngm_tool_mini_fusen_score_NCGR,
				p_bgl->p_bgl, GF_BGL_FRAME2_M, MNGM_RESULT_BALLOON_SYSWIN_CGX, 0, FALSE, heapID );
		p_wk->p_scrnbuff = ArcUtil_HDL_ScrnDataGet( p_handle, NARC_wlmngm_tool_mini_fusen_score_NSCR, 
				FALSE, &p_wk->p_scrn, heapID );
	}

	//�@OAM	
	{
		int i, j;
		int idx;
		int x, y;
		
		// OAM��ǂݍ���
		p_wk->p_resobj = MNGM_CLACTRES_Load( p_clres, p_handle, 
				NARC_wlmngm_tool_result_fusen_NCLR, 1,
				NARC_wlmngm_tool_result_fusen_NCGR,
				NARC_wlmngm_tool_result_fusen_NCER,
				NARC_wlmngm_tool_result_fusen_NANR,
				MNGM_RESULT_BALLOON_CONTID, heapID );

		// �A�N�^�[���쐬
		y = MNGM_RESULT_BALLOON_DEF_Y;
		for( i=0; i<MNGM_RESULT_BALLOON_Y; i++ ){
			x = MNGM_RESULT_BALLOON_DEF_X;

			for( j=0; j<MNGM_RESULT_BALLOON_X; j++ ){
				idx = (i*MNGM_RESULT_BALLOON_X)+j;
				p_wk->p_clwk[idx] = MNGM_CLACTRES_Add( p_wk->p_resobj,
						p_clact->p_clactset, x, y, 0, heapID );
				CLACT_SetDrawFlag( p_wk->p_clwk[idx], FALSE );
				CLACT_AnmChg( p_wk->p_clwk[idx], i );
				CLACT_BGPriorityChg( p_wk->p_clwk[idx], MNGM_RESULT_BALLOON_BG_PRI );

				x += MNGM_RESULT_BALLOON_SIZ_X;
			}

			y += MNGM_RESULT_BALLOON_SIZ_Y;
		}
	}
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	BALLOON	�O���t�B�b�N�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_Balloon_GraphicExit( MNGM_BALLOON_WK* p_wk )
{

	//  OAM�j��
	{
		int i;

		for( i=0; i<MNGM_RESULT_BALLOON_CLACT_NUM; i++ ){
			CLACT_Delete( p_wk->p_clwk[i] );
		}
	}

	

	// �V�X�e���E�B���h�E�j��
	{
		sys_FreeMemoryEz( p_wk->p_scrnbuff );
		// �E�B���h�E�쐬
//		GF_BGL_BmpWinDel( &p_wk->win ); 
	}
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	����J�n
 *
 *	@param	p_wk	���[�N
 *	@param	num		��������
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_Balloon_Start( MNGM_BALLOON_WK* p_wk, u32 num )
{
	p_wk->num = num;
	p_wk->seq = 0;
	p_wk->idx = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	BALLOON�E�B���h�E	����
 *
 *	@param	p_wk	���[�N
 *	@param	p_bgl	BGL�V�X�e��
 *
 *	@retval	TRUE	�I����
 *	@ratval	FALSE	�܂�
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_RESULT_Balloon_Main( MNGM_BALLOON_WK* p_wk, MNGM_BGL* p_bgl )
{
	switch( p_wk->seq ){
	case MNGM_RESULT_BALLOON_SEQ_ON:

		MNGM_SCRN_AddCharOfs( p_wk->p_scrn,  MNGM_RESULT_BALLOON_SYSWIN_CGX ); 
		GF_BGL_ScrWrite( p_bgl->p_bgl, GF_BGL_FRAME2_M, p_wk->p_scrn->rawData, 
				0, 0,
				MNGM_RESULT_BALLOON_SYSWIN_SCRNSIZX, MNGM_RESULT_BALLOON_SYSWIN_SCRNSIZY );
		GF_BGL_ScrPalChange( p_bgl->p_bgl, GF_BGL_FRAME2_M, 0, 0,
				MNGM_RESULT_BALLOON_SYSWIN_SCRNSIZX, MNGM_RESULT_BALLOON_SYSWIN_SCRNSIZY,
				MNGM_RESULT_BALLOON_SYSWIN_PAL );
		GF_BGL_LoadScreenV_Req( p_bgl->p_bgl, GF_BGL_FRAME2_M );

		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_BLN_ON:

		// �I�����̐��ɂȂ�����I���
		if( p_wk->idx == p_wk->num ){
			p_wk->seq = MNGM_RESULT_BALLOON_SEQ_BLN_END;
			break;	
		}
		Snd_SePlay( MNGM_SND_BALLOON );
		
		CLACT_SetDrawFlag( p_wk->p_clwk[ (p_wk->idx % MNGM_RESULT_BALLOON_CLACT_NUM) ], TRUE );
		p_wk->count = MNGM_RESULT_BALLOON_WAIT;
		p_wk->seq ++;
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_BLN_ONWAIT:
		p_wk->count--;
		if( p_wk->count == 0 ){

			p_wk->idx ++;
			if( (p_wk->idx % MNGM_RESULT_BALLOON_CLACT_NUM) == 0 ){
				int i;

				// ����ɃI�����łȂ����
				if( p_wk->idx != p_wk->num ){

					for( i=0; i<MNGM_RESULT_BALLOON_CLACT_NUM; i++ ){
						// �SOAM���N���[��
						CLACT_SetDrawFlag( p_wk->p_clwk[i], FALSE );
						// �SOAM��ԂɕύX
						CLACT_AnmChg( p_wk->p_clwk[i], MNGM_RESULT_BALLOON_ANM_RED );
					}
				}
			}
			p_wk->seq = MNGM_RESULT_BALLOON_SEQ_BLN_ON;
		}
		break;
		
	case MNGM_RESULT_BALLOON_SEQ_BLN_END:
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�^�}������Balance�{�[���̃v���C���[�e�[�u����`�悷�鏈��
 *
 *	@param	p_wk		���[�N
 *	@param	plidx		�v���C���[IDX
 *	@param	rank		Rank
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_BallslowBalancePlayerTblDraw( MNGM_RESULTWK* p_wk, u32 plidx, u32 rank )
{
	BOOL  vip;
	u32 keta;
	u32 netid;
	
	MNGM_PLATE_PLAYERTBL_Init( &p_wk->playertbl[plidx], 
			p_wk->comm_param.num, p_wk->gametype,
			p_wk->p_scrn[ MNGM_RESULT_SCRN_TBL ], &p_wk->bgl, plidx, 
			p_wk->comm_param.my_playerid,
			&p_wk->clact, 
			p_wk->p_resobj, 
			p_wk->heapID );

	// ���O�\��
	netid = MNGM_ENRES_PARAM_GetPlNOtoNetID( &p_wk->comm_param, plidx );
	vip = MNGM_ENRES_PARAM_GetVipFlag( &p_wk->comm_param, netid );
	MNGM_PLATE_PLAYERTBL_DrawName( &p_wk->playertbl[plidx], &p_wk->msg, p_wk->comm_pdata.cp_status[plidx], MNGM_RESULT_DRAWNAME_X, MNGM_RESULT_DRAWNAME_Y, vip );
	// �����\��
	MNGM_PLATE_PLAYERTBL_DrawNation( &p_wk->playertbl[plidx], &p_wk->msg, 
			p_wk->draw_nation, p_wk->comm_pdata.nation[plidx],
			p_wk->comm_pdata.area[plidx], MNGM_RESULT_DRAWNAME_X );

	// �ʏ��Ƌʂ���ŕς�镔��
	if( p_wk->gametype == WFLBY_GAME_BALLSLOW ){
		keta = 5;
	}else{
		keta = 6;
	}

	// �X�R�A�\��
	MNGM_PLATE_PLAYERTBL_DrawScore( &p_wk->playertbl[plidx], &p_wk->msg,
			p_wk->param.score[plidx], 
			MNGM_RESULT_DRAWSCORE_X + (-sc_MNGM_PLATE_MOVE_RESULT_END[WFLBY_MINIGAME_MAX-(rank+1)]),
			MNGM_RESULT_DRAWSCORE_Y, keta );

	// �������ݖ���
	MNGM_PLATE_PLAYERTBL_BgWriteVReq( &p_wk->playertbl[plidx], &p_wk->bgl );

	// �����ɂ���OAM�̕������o��
//	if( plidx == 0 ){	// plidx��0�Ȃ玩��
//		MNGM_PLATE_PLAYERTBL_DrawOamWay( &p_wk->playertbl[plidx] );
//	}

	// ���ʕ\��
	MNGM_PLATE_PLAYERTBL_DrawRank( &p_wk->playertbl[plidx], rank );

	// Rank�̓���J�n
	MNGM_PLATE_PLAYERTBL_StartMove( &p_wk->playertbl[plidx], sc_MNGM_PLATE_MOVE_RESULT_END[rank], 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�e�[�u��	����
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_RESULT_PlayerTblMove( MNGM_RESULTWK* p_wk )
{
	int i;
	BOOL result;
	BOOL plate_end = TRUE;

	// �S����PLATE�𓮂���
	for( i=0; i<p_wk->comm_param.num; i++ ){

		// ����������Ă邩�`�F�b�N
		if( p_wk->playertbl[i].p_clwk != NULL ){

			result = MNGM_PLATE_PLAYERTBL_MainMove( &p_wk->playertbl[i] );
			MNGM_PLAYER_PLATE_HBLANK_BGSCR_SetPlateScr( &p_wk->bgscrl, &p_wk->playertbl[i] );
			if( result == FALSE ){
				plate_end = FALSE;
			}
		}else{
			plate_end = FALSE;
		}
	}

	return plate_end;
}


//----------------------------------------------------------------------------
/**
 *	@brief	1�ʂ�ME���Đ�����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_SndTopMePlay( MNGM_RESULTWK* p_wk )
{
	if( p_wk->top_me_play == FALSE ){
		Snd_MePlay( SEQ_PL_WINMINI2 );
		p_wk->top_me_play = TRUE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	1�ʂ�ME�̍Đ��I���҂�
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	ME�Đ�����
 *	@retval	FALSE	ME�Đ���
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_RESULT_SndTopMeEndWait( MNGM_RESULTWK* p_wk )
{
	if( p_wk->top_me_play == TRUE ){

		// �I���҂��{BGM���A
		if( Snd_MePlayCheckBgmPlay() == 0 ){ 
			p_wk->top_me_play = FALSE;
		}
	}

	if( p_wk->top_me_play == TRUE ){
		return FALSE;
	}else{
		return TRUE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�ƗV�񂾉񐔂�ݒ肷��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_SetPlayNum( MNGM_RESULTWK* p_wk )
{
	WIFI_LIST* p_list = SaveData_GetWifiListData( p_wk->comm_param.p_save );
	int i;
	int netID, ret;
	int pos;
	DWCFriendData* p_friend_data;

	for( i=0; i<p_wk->comm_param.num; i++ ){
		netID = p_wk->comm_param.pnetid[i];
		p_friend_data = CommInfoGetDWCFriendCode( netID );
		ret = dwc_checkfriendByToken( p_wk->comm_param.p_save,
				p_friend_data, &pos );
		switch( ret ){
		case DWCFRIEND_INLIST:// �c ���łɁA�����f�[�^�����X�g��ɂ���B���̏ꍇ�͉�������K�v�Ȃ��B
		case DWCFRIEND_OVERWRITE:// �c ���łɁA�����f�[�^�����X�g��ɂ��邪�A�㏑�����邱�Ƃ��]�܂����ꍇ�B
			switch(p_wk->gametype){
			case WFLBY_GAME_BALLSLOW:	// �ʓ���
				WifiList_AddMinigameBallSlow( p_list, pos, 1 );
				break;
			case WFLBY_GAME_BALANCEBALL:	// �ʏ��
				WifiList_AddMinigameBalanceBall( p_list, pos, 1 );
				break;
			case WFLBY_GAME_BALLOON:		// �ӂ�������
				WifiList_AddMinigameBalloon( p_list, pos, 1 );
				break;
			}
			break;

		default:
			// �F�B�o�^���Ă��Ȃ�
			break;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N����gametype�̒l�����āA�������Ă��Ȃ��Ƃ��̃X�R�A���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�������ĂȂ����̃X�R�A
 */
//-----------------------------------------------------------------------------
static u32 MNGM_RESULT_BallslowBalanceGetNoTouchScore( const MNGM_RESULTWK* cp_wk )
{
	u32 ret;
	switch( cp_wk->gametype ){
	case WFLBY_GAME_BALLSLOW:
		ret = 0;
		break;

	case WFLBY_GAME_BALANCEBALL:
		ret = MNGM_BALANCEBALL_NOTOUCH_SCORE;
		break;

	case WFLBY_GAME_BALLOON:
		// �����BALLSLOW��BALANCEBALL�p
		GF_ASSERT(0);
		ret = 0;
		break;
	}
	return ret;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�^�}�����@�ʏ��@�~�j�Q�[�����ʃf�[�^���M
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_BallslowBalance_SendMinigameTopResult( MNGM_RESULTWK* p_wk )
{
	int i;
	u32 topidx[ WFLBY_MINIGAME_MAX ] = {0};
	u32 topnum;
	u32 notouch_score;
	
	// 1�ʂ������瑗�M����
	topnum = 0;
	notouch_score = MNGM_RESULT_BallslowBalanceGetNoTouchScore( p_wk );
	for( i=0; i<p_wk->comm_param.num; i++ ){
		if( (p_wk->param.result[i] == 0) && (p_wk->param.score[i] != notouch_score) ){
			topidx[ topnum ] = p_wk->comm_param.p_lobby_wk->plidx.plidx[i];
			topnum ++;
		}
	}
	if( topnum > 0 ){
		WFLBY_SYSTEM_TOPIC_SendMiniGameTopResult( p_wk->comm_param.p_lobby_wk->p_wflbysystem,
				p_wk->gametype, topnum, topidx[0], topidx[1], topidx[2], topidx[3] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���犄	�~�j�Q�[�����ʃf�[�^���M
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_RESULT_Balloon_SendMinigameTopResult( MNGM_RESULTWK* p_wk )
{
	if( p_wk->comm_param.num > 0 ){
		if( p_wk->param.balloon >= MNGM_BALLOON_GADGET_UPDATE ){

			WFLBY_SYSTEM_TOPIC_SendMiniGameTopResult( p_wk->comm_param.p_lobby_wk->p_wflbysystem,
					p_wk->gametype, p_wk->comm_param.num, 
					p_wk->comm_param.p_lobby_wk->plidx.plidx[0],
					p_wk->comm_param.p_lobby_wk->plidx.plidx[1], 
					p_wk->comm_param.p_lobby_wk->plidx.plidx[2], 
					p_wk->comm_param.p_lobby_wk->plidx.plidx[3] );
		}
	}
}





//-------------------------------------
///	START	TIMEUP	���[�N
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	OAM�}�X�N�V�X�e��
 *
 *	@param	p_wk		���[�N
 *	@param	p_resobj	���\�[�X�I�u�W�F
 *	@param	p_clset		�Z���A�N�^�[�Z�b�g
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MNGM_COUNT_MskInit( MNGM_COUNT_MSKWK* p_wk, MNGM_CLACTRESOBJ* p_resobj, CLACT_SET_PTR p_clset, u32 heapID )
{
	int i;
	int wnd_draw;

	memset( p_wk->mskon[MNGM_COUNT_MSK_HBUF_READ], 0, 192 );
	memset( p_wk->mskon[MNGM_COUNT_MSK_HBUF_WRITE], 0, 192 );
	p_wk->count		= 0;
	p_wk->move_y	= 0;
	p_wk->p_tcb		= NULL;

	for( i=0; i<MNGM_COUNT_MSK_OAM_NUM; i++ ){
		p_wk->p_msk[i] = MNGM_CLACTRES_Add( p_resobj, p_clset, 
				sc_MNGM_COUNT_MSK_MAT[i].x >> FX32_SHIFT, 
				sc_MNGM_COUNT_MSK_MAT[i].y >> FX32_SHIFT, 
				MNGM_COUNT_MSK_PRI, heapID );

		CLACT_AnmChg( p_wk->p_msk[i], 5 );
		CLACT_BGPriorityChg( p_wk->p_msk[i], 0 );
	}

	// HBLANK�֐��ݒ�
	wnd_draw = GX_GetVisibleWnd();
	wnd_draw &= ~GX_WNDMASK_W0;
	GX_SetVisibleWnd( wnd_draw );
	G2_SetWnd0InsidePlane( GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|GX_WND_PLANEMASK_BG2|GX_WND_PLANEMASK_BG3|GX_WND_PLANEMASK_OBJ, TRUE );
	G2_SetWnd0Position( 0, 0, 0, 0 );
	p_wk->off_outplane	= G2_GetWndOutsidePlane(); 
	p_wk->def_wnd1 = GX_GetVisibleWnd() & GX_WNDMASK_W1;
	p_wk->on_outplane.planeMask	= GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|GX_WND_PLANEMASK_BG2|GX_WND_PLANEMASK_BG3;
	p_wk->on_outplane.effect	= TRUE;
	sys_HBlankIntrSet( MNGM_COUNT_MskSetMskHBlank, p_wk );


	// ���̓t���b�v������
	CLACT_SetFlip( p_wk->p_msk[ MNGM_COUNT_MSK_OAM_BTTM ], CLACT_FLIP_V );

	// �}�X�N�ݒ�
	MNGM_COUNT_MskSetMsk( p_wk );

	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�X�N���C��	
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL MNGM_COUNT_MskMain( MNGM_COUNT_MSKWK* p_wk )
{
	BOOL ret = FALSE;

	p_wk->count ++;
	if( p_wk->count >= MNGM_COUNT_MSK_COUNT ){
		p_wk->count = MNGM_COUNT_MSK_COUNT;
		ret = TRUE;
	}
	MNGM_COUNT_MskCommon( p_wk, 1 );
		
	return ret;
}
static BOOL MNGM_COUNT_MskReMain( MNGM_COUNT_MSKWK* p_wk )
{
	BOOL ret = FALSE;

	p_wk->count --;
	if( p_wk->count <= 0 ){
		p_wk->count = 1;
		ret = TRUE;

		CLACT_SetDrawFlag( p_wk->p_msk[0], FALSE );
		CLACT_SetDrawFlag( p_wk->p_msk[1], FALSE );
	}
	MNGM_COUNT_MskCommon( p_wk, -1 );
		
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�X�N	����	���ʕ���
 *
 *	@param	p_wk 
 *	@param	add
 */
//-----------------------------------------------------------------------------
static void MNGM_COUNT_MskCommon( MNGM_COUNT_MSKWK* p_wk, s32 add )
{
	VecFx32 mat;
	int i;
	int move_y_oam;

	p_wk->move_y = (p_wk->count * MNGM_COUNT_MSK_MY) / MNGM_COUNT_MSK_COUNT;

	if( ((p_wk->count+add) <= MNGM_COUNT_MSK_COUNT) && ((p_wk->count+add) >= 0) ){
		move_y_oam = ((p_wk->count+add) * MNGM_COUNT_MSK_MY) / MNGM_COUNT_MSK_COUNT;
	}else{
		move_y_oam = p_wk->move_y;
	}
			
	for( i=0; i<MNGM_COUNT_MSK_OAM_NUM; i++ ){
		mat = sc_MNGM_COUNT_MSK_MAT[i];
		if( i==0 ){
			mat.y -= FX32_CONST( move_y_oam );
		}else{
			mat.y += FX32_CONST( move_y_oam );
		}

		CLACT_SetMatrix( p_wk->p_msk[i], &mat );
	}

	MNGM_COUNT_MskSetMsk( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�X�N�V�X�e��	�j��
 *		
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_COUNT_MskExit( MNGM_COUNT_MSKWK* p_wk )
{
	int i;

	if( p_wk->p_tcb ){
		TCB_Delete( p_wk->p_tcb );
		p_wk->p_tcb = NULL;
	}

	for( i=0; i<MNGM_COUNT_MSK_OAM_NUM; i++ ){
		if( p_wk->p_msk[i] != NULL ){
			CLACT_Delete( p_wk->p_msk[i] );
			p_wk->p_msk[i] = NULL;
		}
	}

	// HBLANK�j��
	sys_HBlankIntrStop();

	// �E�B���h�E�}�X�N�j��
	GX_SetVisibleWnd( p_wk->def_wnd1 );
	G2_SetWndOutsidePlane( p_wk->off_outplane.planeMask, p_wk->off_outplane.effect );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�}�X�N�ݒ�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_COUNT_MskSetMsk( MNGM_COUNT_MSKWK* p_wk )
{
	int i;

	memset( p_wk->mskon[MNGM_COUNT_MSK_HBUF_WRITE], 0, 192 );
	// �}�X�N�ݒ�o�b�t�@���쐬����
	// �㑤
	for( i=0; i<MNGM_COUNT_MSK_MY-p_wk->move_y; i++ ){
		p_wk->mskon[MNGM_COUNT_MSK_HBUF_WRITE][ MNGM_COUNT_MSK_SY0+i ] = TRUE;
		p_wk->mskon[MNGM_COUNT_MSK_HBUF_WRITE][ MNGM_COUNT_MSK_SY1-i ] = TRUE;
	}
	
	if( p_wk->p_tcb == NULL ){
		
		p_wk->p_tcb = VWaitTCB_Add( MNGM_COUNT_MskSetMskVBlank, p_wk, 0 );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank���Ԓ��ɃE�B���h�E�ݒ�
 *
 *	@param	tcb		TCB
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_COUNT_MskSetMskVBlank( TCB_PTR tcb, void* p_work )
{
	MNGM_COUNT_MSKWK* p_wk = p_work;

	// �o�b�t�@���X�C�b�`
	memcpy( p_wk->mskon[MNGM_COUNT_MSK_HBUF_READ], p_wk->mskon[MNGM_COUNT_MSK_HBUF_WRITE], 192 );

	TCB_Delete( p_wk->p_tcb );
	p_wk->p_tcb = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	HBLANK�֐�
 *
 *	@param	p_work	���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_COUNT_MskSetMskHBlank( void* p_work )
{
	MNGM_COUNT_MSKWK*	p_wk = p_work;
	s32					vcount;
	int wnd_draw;
	
	vcount = GX_GetVCount();

	vcount = (vcount + 1) % 256;	// ���̃��C���̐ݒ���s��
	
	if( vcount < 192 ){
		wnd_draw = GX_GetVisibleWnd();

#if PL_T0849_080709_FIX
		if( GX_IsHBlank() ){
			if( (vcount < MNGM_COUNT_MSK_SY0) || (vcount > MNGM_COUNT_MSK_SY1) ){
				G2_SetWndOutsidePlane( p_wk->off_outplane.planeMask, p_wk->off_outplane.effect );
				wnd_draw |= p_wk->def_wnd1;
			}else{
				G2_SetWndOutsidePlane( p_wk->on_outplane.planeMask, p_wk->on_outplane.effect );
				wnd_draw &= ~(GX_WNDMASK_W1 | p_wk->def_wnd1);
			}
			

			if( p_wk->mskon[ MNGM_COUNT_MSK_HBUF_READ ][ vcount ] == TRUE ){
				GX_SetVisibleWnd( wnd_draw|GX_WNDMASK_W0 );
			}else{
				wnd_draw &= ~GX_WNDMASK_W0;
				GX_SetVisibleWnd( wnd_draw );
			}
		}
#else

		if( (vcount < MNGM_COUNT_MSK_SY0) || (vcount > MNGM_COUNT_MSK_SY1) ){
			G2_SetWndOutsidePlane( p_wk->off_outplane.planeMask, p_wk->off_outplane.effect );
			wnd_draw |= p_wk->def_wnd1;
		}else{
			G2_SetWndOutsidePlane( p_wk->on_outplane.planeMask, p_wk->on_outplane.effect );
			wnd_draw &= ~(GX_WNDMASK_W1 | p_wk->def_wnd1);
		}
		

		if( p_wk->mskon[ MNGM_COUNT_MSK_HBUF_READ ][ vcount ] == TRUE ){
			GX_SetVisibleWnd( wnd_draw|GX_WNDMASK_W0 );
		}else{
			wnd_draw &= ~GX_WNDMASK_W0;
			GX_SetVisibleWnd( wnd_draw );
		}
#endif
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�J�n	�G�t�F�N�g	�^�X�N
 *
 *	@param	tcb			TCB
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_COUNT_StartTcb( TCB_PTR tcb, void* p_work )
{
	MNGM_COUNTWK* p_wk = p_work;
	BOOL result;
	u16 last_seq;
	u16 now_seq;

	switch( p_wk->seq ){
	case MNGM_COUNT_START_SEQ_NONE:
		break;
		
	// ������
	case MNGM_COUNT_START_SEQ_INIT:
		MNGM_COUNT_MskInit( &p_wk->msk, p_wk->p_resobj, 
				p_wk->p_clset, p_wk->heapID );

		p_wk->p_anm = MNGM_CLACTRES_Add( p_wk->p_resobj, 
				p_wk->p_clset, 
				MNGM_COUNT_ANM_X, 
				MNGM_COUNT_ANM_Y, 
				MNGM_COUNT_ANM_PRI, p_wk->heapID );

		CLACT_BGPriorityChg( p_wk->p_anm, 0 );
		p_wk->seq ++;
		break;
		
	// �}�X�N�V�X�e��
	case MNGM_COUNT_START_SEQ_MSK:
		result = MNGM_COUNT_MskMain( &p_wk->msk );
		if( result == TRUE ){
			p_wk->seq ++;
			p_wk->count = 0;
		}
		break;
		
	// �A�j��
	case MNGM_COUNT_START_SEQ_ANM:

		// �ύX�O�̃t���[��NO
		last_seq = CLACT_AnmFrameGet( p_wk->p_anm );
		
		// �A�j��
		CLACT_AnmFrameChg( p_wk->p_anm, FX32_CONST(2) );

		// �ύX��̂ӂ�[��NO
		now_seq = CLACT_AnmFrameGet( p_wk->p_anm );

		// �ύX�O�ƌ�Ńt���[�����؂�ւ�����特���o��
		if( last_seq != now_seq ){
			switch( now_seq ){
			case MNGM_COUNT_REDY_ANM:
				Snd_SePlay( MNGM_SND_COUNT );
				break;
				
			case MNGM_COUNT_3_ANM:
				Snd_SePlay( MNGM_SND_COUNT );
				break;
				
			case MNGM_COUNT_2_ANM:
				Snd_SePlay( MNGM_SND_COUNT );
				break;
				
			case MNGM_COUNT_1_ANM:
				Snd_SePlay( MNGM_SND_COUNT );
				break;
				
			case MNGM_COUNT_STARTFRAME:
				Snd_SePlay( MNGM_SND_START );
				break;

			default:
				break;
			}
		}

		// �p���b�g�A�j��
		if( now_seq == MNGM_COUNT_STARTFRAME ){
			if( p_wk->count == 0 ){
				CLACT_PaletteOffsetChgAddTransPlttNo( p_wk->p_anm, 0 );
			}else if( p_wk->count == MNGM_COUNT_STARTPALANMTIMING ){
				CLACT_PaletteOffsetChgAddTransPlttNo( p_wk->p_anm, 1 );
			}

			p_wk->count ++;
			if( p_wk->count >= MNGM_COUNT_STARTPALANMTIMING*2 ){
				p_wk->count = 0;
			}
		}
		
		result = CLACT_AnmActiveCheck( p_wk->p_anm );
		if( result == FALSE ){
			p_wk->seq ++;
			CLACT_PaletteOffsetChgAddTransPlttNo( p_wk->p_anm, 0 );
		}
		break;

	case MNGM_COUNT_START_SEQ_MSK2:
		result = MNGM_COUNT_MskReMain( &p_wk->msk );
		if( result == TRUE ){
			// �A�N�^�[�j��
			CLACT_Delete( p_wk->p_anm );
			p_wk->p_anm = NULL;
			p_wk->seq ++;
			p_wk->count = 0;
		}
		break;

	case MNGM_COUNT_START_SEQ_END:
		p_wk->count ++;
		if( p_wk->count < 2 ){
			break;
		}

		p_wk->seq = MNGM_COUNT_START_SEQ_NONE;

		// �}�X�N�V�X�e���j��
		MNGM_COUNT_MskExit( &p_wk->msk );

		// ����j��
		TCB_Delete( p_wk->p_tcb );
		p_wk->p_tcb = NULL;
		break;

	default:
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I��	�G�t�F�N�g	�^�X�N
 *
 *	@param	tcb			TCB
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void MNGM_COUNT_TimeUpTcb( TCB_PTR tcb, void* p_work )
{
	MNGM_COUNTWK* p_wk = p_work;
	BOOL result;

	switch( p_wk->seq ){
	case MNGM_COUNT_TIMEUP_SEQ_NONE:
		break;
		
	// ������
	case MNGM_COUNT_TIMEUP_SEQ_INIT:
		MNGM_COUNT_MskInit( &p_wk->msk, p_wk->p_resobj, 
				p_wk->p_clset, p_wk->heapID );

		p_wk->p_anm = MNGM_CLACTRES_Add( p_wk->p_resobj, 
				p_wk->p_clset, 
				MNGM_COUNT_ANM_X, 
				MNGM_COUNT_ANM_Y, 
				MNGM_COUNT_ANM_PRI, p_wk->heapID );

		CLACT_AnmChg( p_wk->p_anm, 1 );
		CLACT_BGPriorityChg( p_wk->p_anm, 0 );
		p_wk->seq ++;
		break;
		
	// �}�X�N�V�X�e��
	case MNGM_COUNT_TIMEUP_SEQ_MSK:
		result = MNGM_COUNT_MskMain( &p_wk->msk );
		if( result == TRUE ){
			CLACT_SetAnmFlag( p_wk->p_anm, TRUE );
			p_wk->seq ++;
			p_wk->count = 0;
		}
		break;
		
	// �A�j��
	case MNGM_COUNT_TIMEUP_SEQ_ANM:

		p_wk->count ++;
		if( p_wk->count >= MNGM_COUNT_TIMEUP_ANM_NUM ){
			p_wk->seq ++;
		}
		break;

	case MNGM_COUNT_TIMEUP_SEQ_MSK2:
		result = MNGM_COUNT_MskReMain( &p_wk->msk );
		if( result == TRUE ){
			// �A�N�^�[�j��
			CLACT_Delete( p_wk->p_anm );
			p_wk->p_anm = NULL;
			p_wk->count = 0;
			p_wk->seq ++;
		}
		break;

	case MNGM_COUNT_TIMEUP_SEQ_END:
		p_wk->count ++;
		if( p_wk->count < 2 ){
			break;
		}
		p_wk->seq = MNGM_COUNT_TIMEUP_SEQ_NONE;

		// �}�X�N�V�X�e���j��
		MNGM_COUNT_MskExit( &p_wk->msk );

		// ����j��
		TCB_Delete( p_wk->p_tcb );
		p_wk->p_tcb = NULL;
		break;
		
	default:
		break;
	}
}




//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_anketo.c
 *	@brief		�A���P�[�g���
 *	@author		tomoya takahashi
 *	@data		2008.05.29
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]


#include "common.h"


#include "gflib/strbuf_family.h"	// �C�ӎ���Ŏg�p

#include "system/clact_util.h"
#include "system/wipe.h"
#include "system/render_oam.h"
#include "system/pm_overlay.h"
#include "system/fontproc.h"
#include "system/msgdata.h"
#include "system/window.h"
#include "system/wordset.h"
#include "system/bmp_menu.h"

#include "communication/wm_icon.h"


#include "msgdata/msg.naix"
#include "msgdata/msg_wifi_hiroba.h"
#include "msgdata/msg_wifi_50_answers_a.h"
#include "msgdata/msg_wifi_50_answers_b.h"
#include "msgdata/msg_wifi_50_answers_c.h"
#include "msgdata/msg_wifi_50_questions.h"
#include "msgdata/msg_wifi_Special_answers_a.h"
#include "msgdata/msg_wifi_Special_answers_b.h"
#include "msgdata/msg_wifi_Special_answers_c.h"
#include "msgdata/msg_wifi_Special_questions.h"

#include "graphic/wifi_lobby_other.naix"
#include "graphic/lobby_news.naix"

#include "savedata/config.h"
#include "savedata/wifi_hiroba_save.h"

#include "wflby_anketo.h"
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
#endif


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�`��V�X�e��
//=====================================
// VRAM�]���^�X�N
#define ANKETO_VRAMTRANS_TASKNUM		( 32 )
// OAM
#define ANKETO_OAM_CONTNUM		( 32 )
#define ANKETO_RESMAN_NUM		( 4 )	// OAM���\�[�X�}�l�[�W����
#define ANKETO_SF_MAT_Y		( FX32_CONST(256) )
static const CHAR_MANAGER_MAKE sc_ANKETO_CHARMAN_INIT = {
	ANKETO_OAM_CONTNUM,
	16*1024,	// 16K
	0,	// 16K
	HEAPID_ANKETO
};


//-------------------------------------
///	�p���b�g�ݒ�
//=====================================
enum {
	ANKETO_PLTT_MAIN_BACK00,
	ANKETO_PLTT_MAIN_BACK01,
	ANKETO_PLTT_MAIN_BACK02,
	ANKETO_PLTT_MAIN_BACK03,
	ANKETO_PLTT_MAIN_TALKFONT,
	ANKETO_PLTT_MAIN_TALKWIN,
	ANKETO_PLTT_MAIN_SYSWIN,
} ;
#define ANKETO_PLTT_MAINBACK_PLTTNUM	(4)

#define ANKETO_PLTT_FONT_COL_INPUT		( GF_PRINTCOLOR_MAKE( 1, 2, 0 ) )
#define ANKETO_PLTT_FONT_COL_OUTPUT00	( GF_PRINTCOLOR_MAKE( 9, 10, 0 ) )
#define ANKETO_PLTT_FONT_COL_OUTPUT01	( GF_PRINTCOLOR_MAKE( 11, 12, 0 ) )
#define ANKETO_PLTT_FONT_COL_VIP00		( GF_PRINTCOLOR_MAKE( 13, 14, 0 ) )
#define ANKETO_PLTT_FONT_COL_VIP01		( GF_PRINTCOLOR_MAKE( 15, 14, 0 ) )

#define ANKETO_PLTT_TITLEFONT_COL_INPUT		( GF_PRINTCOLOR_MAKE( 7, 8, 0 ) )
#define ANKETO_PLTT_TITLEFONT_COL_OUTPUT00	( GF_PRINTCOLOR_MAKE( 3, 4, 0 ) )
#define ANKETO_PLTT_TITLEFONT_COL_OUTPUT01	( GF_PRINTCOLOR_MAKE( 5, 6, 0 ) )


//-------------------------------------
///	��b�E�B���h�E
//=====================================
#define ANKETO_TALKWIN_CGX		(1)	
#define ANKETO_TALKWIN_PAL		(ANKETO_PLTT_MAIN_TALKWIN)
#define ANKETO_TALKWIN_CGXEND	(ANKETO_TALKWIN_CGX+TALK_WIN_CGX_SIZ)

// ���C����ʂ̃V�X�e���E�B���h�E
#define ANKETO_SYSTEMWIN_CGX		(ANKETO_TALKWIN_CGXEND)
#define ANKETO_SYSTEMWIN_PAL		(ANKETO_PLTT_MAIN_SYSWIN)
#define ANKETO_SYSTEMWIN_CGXEND		(ANKETO_SYSTEMWIN_CGX+MENU_WIN_CGX_SIZ)


#define ANKETO_TALK_TALKWIN_X		( 2 )
#define ANKETO_TALK_TALKWIN_Y		( 19 )
#define ANKETO_TALK_TALKWIN_SIZX	( 27 )
#define ANKETO_TALK_TALKWIN_SIZY	( 4 )
#define ANKETO_TALK_TALKWIN_CGX		( ANKETO_SYSTEMWIN_CGXEND )
#define ANKETO_TALK_TALKWIN_CGXEND	( ANKETO_TALK_TALKWIN_CGX+(ANKETO_TALK_TALKWIN_SIZX*ANKETO_TALK_TALKWIN_SIZY) )
#define ANKETO_TALK_COL			( GF_PRINTCOLOR_MAKE( 1, 2, 15 ) )

#define ANKETO_TALK_YESNOWIN_X		( 25 )
#define ANKETO_TALK_YESNOWIN_Y		( 13 )
#define ANKETO_TALK_YESNOWIN_SIZX	( 6 )
#define ANKETO_TALK_YESNOWIN_SIZY	( 4 )
#define ANKETO_TALK_YESNOWIN_CGX		( ANKETO_TALK_TALKWIN_CGXEND )
#define ANKETO_TALK_YESNOWIN_CGXEND	( ANKETO_TALK_YESNOWIN_CGX+(ANKETO_TALK_YESNOWIN_SIZX*ANKETO_TALK_YESNOWIN_SIZY) )
static const BMPWIN_DAT sc_YESNO_BMPDAT = {
	GF_BGL_FRAME1_M,
	ANKETO_TALK_YESNOWIN_X,		ANKETO_TALK_YESNOWIN_Y,
	ANKETO_TALK_YESNOWIN_SIZX,	ANKETO_TALK_YESNOWIN_SIZY,
	ANKETO_PLTT_MAIN_TALKFONT,	ANKETO_TALK_YESNOWIN_CGX,
};


//-------------------------------------
///	���b�Z�[�W�}�l�[�W���萔
//=====================================
enum {
	ANKETO_MSGMAN_MSG,		// �ʏ탁�b�Z�[�W
	ANKETO_MSGMAN_Q_NORMAL,	// ����m�[�}��
	ANKETO_MSGMAN_Q_SPECIAL,// ����SPECIAL
	ANKETO_MSGMAN_A_N_A,	// �m�[�}��	��A
	ANKETO_MSGMAN_A_N_B,	// �m�[�}��	��B
	ANKETO_MSGMAN_A_N_C,	// �m�[�}��	��C
	ANKETO_MSGMAN_A_S_A,	// �X�y�V����	��A
	ANKETO_MSGMAN_A_S_B,	// �X�y�V����	��B
	ANKETO_MSGMAN_A_S_C,	// �X�y�V����	��C
	ANKETO_MSGMAN_NUM,		// �}�l�[�W����
} ;
#define ANKETO_MSGMAN_STRBUFNUM	(256)


//-------------------------------------
///	�A���P�[�g���͉��
//=====================================
enum {
	ANKETO_INPUT_BMP_ANS,
	ANKETO_INPUT_BMP_NUM,
} ;
#define ANKETO_INPUT_OAM_CONTID	( 20 )
#define ANKETO_INPUT_OAM_BGPRI	( 0 )

#define ANKETO_INPUTWIN_X			( 11 )
#define ANKETO_INPUTWIN_Y			( 7 )
#define ANKETO_INPUTWIN_SIZX		( 15 )
#define ANKETO_INPUTWIN_SIZY		( 8 )
#define ANKETO_INPUTWIN_CGX			( 1 )
#define ANKETO_INPUTWIN_CGXEND		( ANKETO_INPUTWIN_CGX+(ANKETO_INPUTWIN_SIZX*ANKETO_INPUTWIN_SIZY) )
#define ANKETO_INPUT_ANSWER_COL			( ANKETO_PLTT_FONT_COL_INPUT )
#define ANKETO_INPUTWIN_ANS00_Y		( 0 )
#define ANKETO_INPUTWIN_ANS01_Y		( 24 )
#define ANKETO_INPUTWIN_ANS02_Y		( 48 )
static const BMPWIN_DAT sc_ANKETO_INPUT_BMPDAT[ ANKETO_INPUT_BMP_NUM ] = {
	{
		GF_BGL_FRAME2_M,
		ANKETO_INPUTWIN_X,		ANKETO_INPUTWIN_Y,
		ANKETO_INPUTWIN_SIZX,	ANKETO_INPUTWIN_SIZY,
		ANKETO_PLTT_MAIN_BACK03,	ANKETO_INPUTWIN_CGX,
	},
};

enum{
	ANKETO_INPUT_SEQ_ANKETO_YARU_Q,		// �A���P�[�g���H
	ANKETO_INPUT_SEQ_ANKETO_YARU_Q_ON,// �A���P�[�g���H
	ANKETO_INPUT_SEQ_ANKETO_YARU_Q_WAIT,// �A���P�[�g���H
//	ANKETO_INPUT_SEQ_ANKETO_RECV,		// ��M�J�n
//	ANKETO_INPUT_SEQ_ANKETO_RECVWAIT,	// ��M��
	ANKETO_INPUT_SEQ_ANKETO_Q_DRAW00,	// ����`��
	ANKETO_INPUT_SEQ_ANKETO_Q_DRAW,		// ����`��
	ANKETO_INPUT_SEQ_ANKETO_Q_SELECT,	// �񓚑I��
	ANKETO_INPUT_SEQ_ANKETO_SEND,		// ��M�J�n
	ANKETO_INPUT_SEQ_ANKETO_SENDWAIT,	// ��M��
	ANKETO_INPUT_SEQ_ANKETO_ARIGATO,	// ���肪�Ƃ����b�Z�[�W�`��
	ANKETO_INPUT_SEQ_ANKETO_END,		// �I��
	ANKETO_INPUT_SEQ_ANKETO_MASWAIT,	// ���b�Z�[�W�҂�
};
static const VecFx32 sc_ANKETO_INPUT_CURSOR_MAT[] = {
	{ FX32_CONST(72),	FX32_CONST(64),0 },
	{ FX32_CONST(72),	FX32_CONST(88),0 },
	{ FX32_CONST(72),	FX32_CONST(112),0 },
};



//-------------------------------------
///	�A���P�[�g�o�͉��
//=====================================
enum {
//	ANKETO_OUTPUT_SEQ_ANKETO_RECV,			// �A���P�[�g��M
//	ANKETO_OUTPUT_SEQ_ANKETO_RECVWAIT,		// �A���P�[�g��M��
	ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE00,	// ���񂩂���
	ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE01,	// ���e
	ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE02,	// �ł���
	ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE03,	// ���񂩂��͂���Ȃł���
	ANKETO_OUTPUT_SEQ_ANKETO_NRESULT,		// ���ʕ\��
	ANKETO_OUTPUT_SEQ_ANKETO_NRESULT_MAIN,	// ���ʕ\��
	ANKETO_OUTPUT_SEQ_ANKETO_NA_WAIT,		// A�{�^���҂�
	ANKETO_OUTPUT_SEQ_ANKETO_YESNO_DRAW,	// YESNO�\��
	ANKETO_OUTPUT_SEQ_ANKETO_YESNO_WAIT,	// YESNO�҂�
	ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEOUT,	// ���t�F�[�h
	ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEOUTWAIT,// ���t�F�[�h
	ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEIN,		// ���t�F�[�h
	ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEINWAIT,	// ���t�F�[�h
	ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE00,	// �O���
	ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE01,	// ���e
	ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE02,	// �ł���
	ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE03,	// �O��͂���Ȃł���
	ANKETO_OUTPUT_SEQ_ANKETO_LRESULT,		// ���ʕ\��
	ANKETO_OUTPUT_SEQ_ANKETO_LRESULT_MAIN,	// ���ʕ\��
	ANKETO_OUTPUT_SEQ_ANKETO_LA_WAIT,		// A�{�^���҂�

	ANKETO_OUTPUT_SEQ_ANKETO_ENDMSG,		// �I�����b�Z�[�W
	ANKETO_OUTPUT_SEQ_ANKETO_END,			// �I��

	ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT,		// ���b�Z�[�W�҂�
};
enum {
	ANKETO_OUTPUT_BMP_TITLE,
	ANKETO_OUTPUT_BMP_ANS,
	ANKETO_OUTPUT_BMP_ANS_BAR00,
	ANKETO_OUTPUT_BMP_ANS_BAR01,
	ANKETO_OUTPUT_BMP_ANS_BAR02,
	ANKETO_OUTPUT_BMP_ANS_MY,
	ANKETO_OUTPUT_BMP_NUM,
} ;

// TITLE
#define ANKETO_OUTPUTWIN_TITLE_X		( 7 )
#define ANKETO_OUTPUTWIN_TITLE_Y		( 1 )
#define ANKETO_OUTPUTWIN_TITLE_SIZX		( 18 )
#define ANKETO_OUTPUTWIN_TITLE_SIZY		( 3 )
#define ANKETO_OUTPUTWIN_TITLE_CGX		( 1 )
#define ANKETO_OUTPUTWIN_TITLE_CGXEND	( ANKETO_OUTPUTWIN_TITLE_CGX+(ANKETO_OUTPUTWIN_TITLE_SIZX*ANKETO_OUTPUTWIN_TITLE_SIZY) )

// ANS
#define ANKETO_OUTPUTWIN_ANS_X			( 5 )
#define ANKETO_OUTPUTWIN_ANS_Y			( 5 )
#define ANKETO_OUTPUTWIN_ANS_SIZX		( 12 )
#define ANKETO_OUTPUTWIN_ANS_SIZY		( 9 )
#define ANKETO_OUTPUTWIN_ANS_CGX		( ANKETO_OUTPUTWIN_TITLE_CGXEND )
#define ANKETO_OUTPUTWIN_ANS_CGXEND		( ANKETO_OUTPUTWIN_ANS_CGX+(ANKETO_OUTPUTWIN_ANS_SIZX*ANKETO_OUTPUTWIN_ANS_SIZY) )
#define ANKETO_OUTPUTWIN_ANS00_Y		( 0 )
#define ANKETO_OUTPUTWIN_ANS01_Y		( 24 )
#define ANKETO_OUTPUTWIN_ANS02_Y		( 48 )

// BAR00
#define ANKETO_OUTPUTWIN_BAR00_X		( 0x11 )
#define ANKETO_OUTPUTWIN_BAR00_Y		( 5 )
#define ANKETO_OUTPUTWIN_BAR00_SIZX		( 11 )
#define ANKETO_OUTPUTWIN_BAR00_SIZY		( 2 )
#define ANKETO_OUTPUTWIN_BAR00_CGX		( ANKETO_OUTPUTWIN_ANS_CGXEND )
#define ANKETO_OUTPUTWIN_BAR00_CGXEND	( ANKETO_OUTPUTWIN_BAR00_CGX+(ANKETO_OUTPUTWIN_BAR00_SIZX*ANKETO_OUTPUTWIN_BAR00_SIZY) )

// BAR01
#define ANKETO_OUTPUTWIN_BAR01_X		( 0x11 )
#define ANKETO_OUTPUTWIN_BAR01_Y		( 8 )
#define ANKETO_OUTPUTWIN_BAR01_SIZX		( 11 )
#define ANKETO_OUTPUTWIN_BAR01_SIZY		( 2 )
#define ANKETO_OUTPUTWIN_BAR01_CGX		( ANKETO_OUTPUTWIN_BAR00_CGXEND )
#define ANKETO_OUTPUTWIN_BAR01_CGXEND	( ANKETO_OUTPUTWIN_BAR01_CGX+(ANKETO_OUTPUTWIN_BAR01_SIZX*ANKETO_OUTPUTWIN_BAR01_SIZY) )

// BAR02
#define ANKETO_OUTPUTWIN_BAR02_X		( 0x11 )
#define ANKETO_OUTPUTWIN_BAR02_Y		( 11 )
#define ANKETO_OUTPUTWIN_BAR02_SIZX		( 11 )
#define ANKETO_OUTPUTWIN_BAR02_SIZY		( 2 )
#define ANKETO_OUTPUTWIN_BAR02_CGX		( ANKETO_OUTPUTWIN_BAR01_CGXEND )
#define ANKETO_OUTPUTWIN_BAR02_CGXEND	( ANKETO_OUTPUTWIN_BAR02_CGX+(ANKETO_OUTPUTWIN_BAR02_SIZX*ANKETO_OUTPUTWIN_BAR02_SIZY) )

// MY
#define ANKETO_OUTPUTWIN_MY_X		( 3 )
#define ANKETO_OUTPUTWIN_MY_Y		( 15 )
#define ANKETO_OUTPUTWIN_MY_SIZX	( 25 )
#define ANKETO_OUTPUTWIN_MY_SIZY	( 2 )
#define ANKETO_OUTPUTWIN_MY_CGX		( ANKETO_OUTPUTWIN_BAR02_CGXEND )
#define ANKETO_OUTPUTWIN_MY_CGXEND	( ANKETO_OUTPUTWIN_MY_CGX+(ANKETO_OUTPUTWIN_MY_SIZX*ANKETO_OUTPUTWIN_MY_SIZY) )

#define ANKETO_OUTPUTWIN_MY_NAME_WRITE_SIZE_X	( 12*8 )
#define ANKETO_OUTPUTWIN_MY_ANSWER_WRITE_SIZE_X	( 13*8 )
#define ANKETO_OUTPUTWIN_MY_ANSWER_X			( 12*8 )



static const BMPWIN_DAT sc_ANKETO_OUTPUT_BMPDAT[ ANKETO_OUTPUT_BMP_NUM ] = {

	// TITLE
	{
		GF_BGL_FRAME2_M,
		ANKETO_OUTPUTWIN_TITLE_X,		ANKETO_OUTPUTWIN_TITLE_Y,
		ANKETO_OUTPUTWIN_TITLE_SIZX,	ANKETO_OUTPUTWIN_TITLE_SIZY,
		ANKETO_PLTT_MAIN_BACK03,	ANKETO_OUTPUTWIN_TITLE_CGX,
	},

	// ANS
	{
		GF_BGL_FRAME2_M,
		ANKETO_OUTPUTWIN_ANS_X,		ANKETO_OUTPUTWIN_ANS_Y,
		ANKETO_OUTPUTWIN_ANS_SIZX,	ANKETO_OUTPUTWIN_ANS_SIZY,
		ANKETO_PLTT_MAIN_BACK03,	ANKETO_OUTPUTWIN_ANS_CGX,
	},

	// BAR00
	{
		GF_BGL_FRAME2_M,
		ANKETO_OUTPUTWIN_BAR00_X,		ANKETO_OUTPUTWIN_BAR00_Y,
		ANKETO_OUTPUTWIN_BAR00_SIZX,	ANKETO_OUTPUTWIN_BAR00_SIZY,
		ANKETO_PLTT_MAIN_TALKFONT,	ANKETO_OUTPUTWIN_BAR00_CGX,
	},

	// BAR01
	{
		GF_BGL_FRAME2_M,
		ANKETO_OUTPUTWIN_BAR01_X,		ANKETO_OUTPUTWIN_BAR01_Y,
		ANKETO_OUTPUTWIN_BAR01_SIZX,	ANKETO_OUTPUTWIN_BAR01_SIZY,
		ANKETO_PLTT_MAIN_TALKFONT,	ANKETO_OUTPUTWIN_BAR01_CGX,
	},

	// BAR02
	{
		GF_BGL_FRAME2_M,
		ANKETO_OUTPUTWIN_BAR02_X,		ANKETO_OUTPUTWIN_BAR02_Y,
		ANKETO_OUTPUTWIN_BAR02_SIZX,	ANKETO_OUTPUTWIN_BAR02_SIZY,
		ANKETO_PLTT_MAIN_TALKFONT,	ANKETO_OUTPUTWIN_BAR02_CGX,
	},

	// MY
	{
		GF_BGL_FRAME2_M,
		ANKETO_OUTPUTWIN_MY_X,		ANKETO_OUTPUTWIN_MY_Y,
		ANKETO_OUTPUTWIN_MY_SIZX,	ANKETO_OUTPUTWIN_MY_SIZY,
		ANKETO_PLTT_MAIN_BACK03,	ANKETO_OUTPUTWIN_MY_CGX,
	},
	
};


// �o�[�\��
#define ANKETO_BAR_100WIDTH	( 80 )
#define ANKETO_BAR_COUNTMAX	( 20 )
#define ANKETO_BAR_CG_X		( 247 )
#define ANKETO_BAR_CG_Y		( 16 )
#define ANKETO_BAR_CG_SIZX		( 1 )
#define ANKETO_BAR_CG_SIZY		( 16 )
#define ANKETO_BAR_CGFILE_X		( 256 )
#define ANKETO_BAR_CGFILE_Y		( 32 )
#define ANKETO_BAR_WRITE_X		( 4 )		// BMP�`��J�n�ʒu


//-------------------------------------
///	�A���P�[�g����	���b�Z�[�W�o�b�t�@
//=====================================
#define ANKETO_MESSAGE_BUFFNUM	( 256 )

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�A���P�[�g��{���
//
//// ��M���b�Z�[�W
//=====================================
typedef struct {
	u32		question_no;
	u32		anketo_no;
	BOOL	special;
	BOOL	make_question;
	STRBUF* p_question;
	STRBUF* p_ans[ ANKETO_ANSWER_NUM ];
} ANKETO_QUESTION_DATA;


//-------------------------------------
///	�A���P�[�g���͉�ʂ̏�����
//=====================================
typedef struct {
	GF_BGL_BMPWIN win[ ANKETO_INPUT_BMP_NUM ];
	s32 cursor;
	CLACT_WORK_PTR		p_cursor_act;
	CLACT_U_RES_OBJ_PTR	p_cursor_res[ANKETO_RESMAN_NUM];
	u16 seq;
	u16 ret_seq;

	ANKETO_QUESTION_DATA question_now;
} ANKETO_INPUT;

//-------------------------------------
///	�A���P�[�g�o�͉�ʂ̏�����
//=====================================
typedef struct {
	GF_BGL_BMPWIN win[ ANKETO_OUTPUT_BMP_NUM ];
	u16 seq;
	u16 ret_seq;

	s32 count;	// �o�[�\���J�E���^
	s32	draw_width[ANKETO_ANSWER_NUM]; 
	s32	drawend_width[ANKETO_ANSWER_NUM]; 

	void* p_charres;
	NNSG2dCharacterData* p_char;

	ANKETO_QUESTION_DATA now;
	ANKETO_QUESTION_DATA last;

	ANKETO_QUESTION_RESULT result_now;
	ANKETO_QUESTION_RESULT result_last;
} ANKETO_OUTPUT;


//-------------------------------------
///	��ʍ\�����[�N
//=====================================
typedef union{
	ANKETO_INPUT input;
	ANKETO_OUTPUT output;
} ANKETO_LOCAL_WK;



//-------------------------------------
///	��b�E�B���h�E�V�X�e��
//=====================================
typedef struct {
	u32 msgno;
	s32 msgwait;
	GF_BGL_BMPWIN win;
	STRBUF*			p_str;
	void*			p_timewait;

	BMPMENU_WORK* p_yesno;
} ANKETO_TALKWIN;



//-------------------------------------
///	���b�Z�[�W�V�X�e���̏�����
//=====================================
typedef struct {
	MSGDATA_MANAGER*	p_msgman[ANKETO_MSGMAN_NUM];		// ��{���b�Z�[�W
	WORDSET*			p_wordset;							// ���[�h�Z�b�g
	STRBUF*				p_msgstr;							// ���b�Z�[�W�o�b�t�@
	STRBUF*				p_msgtmp;							// ���b�Z�[�W�o�b�t�@

	WFLBY_SYSTEM*		p_system;							// ���O�\���p
} ANKETO_MSGMAN;


//-------------------------------------
///	�`��n���[�N
//=====================================
typedef struct {
	// BG
	GF_BGL_INI*				p_bgl;

	// OAM
    CLACT_SET_PTR           p_clactset;		// �Z���A�N�^�[�Z�b�g
    CLACT_U_EASYRENDER_DATA renddata;       // �ȈՃ����_�[�f�[�^
    CLACT_U_RES_MANAGER_PTR p_resman[ANKETO_RESMAN_NUM]; // �L�����E�p���b�g���\�[�X�}�l�[�W��
	// �A�[�J�C�u
	ARCHANDLE* p_handle;
} ANKETO_DRAWSYS;


//-------------------------------------
///	�A���P�[�g�\����
//=====================================
typedef struct {
	ANKETO_DRAWSYS	drawsys;
	ANKETO_MSGMAN	msgsys;
	ANKETO_LOCAL_WK	local_wk;
	ANKETO_TALKWIN	talkwin;
} ANKETO_WORK;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void ANKETO_VBlank( void* p_work );

//-------------------------------------
///	�`��V�X�e��
//=====================================
static void ANKETO_InitGraphic( ANKETO_DRAWSYS* p_wk, SAVEDATA* p_save, u32 heapID );
static void ANKETO_ExitGraphic( ANKETO_DRAWSYS* p_wk );
static void ANKETO_MainGraphic( ANKETO_DRAWSYS* p_wk );
static void ANKETO_VBlankGraphic( ANKETO_DRAWSYS* p_wk );
static void ANKETO_BgInit( ANKETO_DRAWSYS* p_wk, CONFIG* p_config, u32 heapID );
static void ANKETO_BgExit( ANKETO_DRAWSYS* p_wk );
static void ANKETO_OamInit( ANKETO_DRAWSYS* p_wk, u32 heapID );
static void ANKETO_OamExit( ANKETO_DRAWSYS* p_wk );


//-------------------------------------
///	���b�Z�[�W�V�X�e��
//=====================================
static void ANKETO_MsgManInit( ANKETO_MSGMAN* p_wk, WFLBY_SYSTEM* p_system, u32 heapID );
static void ANKETO_MsgManExit( ANKETO_MSGMAN* p_wk );
static STRBUF* ANKETO_MsgManGetStr( ANKETO_MSGMAN* p_wk, u32 type, u32 msg );
static STRBUF* ANKETO_MsgManGetQuestionStr( ANKETO_MSGMAN* p_wk, u32 msg );
static STRBUF* ANKETO_MsgManGetAnswerStr( ANKETO_MSGMAN* p_wk, u32 msg, u32 answer );
static void ANKETO_MsgManSetMyName( ANKETO_MSGMAN* p_wk, u32 bufno, u32 heapID );


//-------------------------------------
///	����f�[�^���[�N
//=====================================
static void ANKETO_QUESTION_DATA_DebugInit( ANKETO_QUESTION_DATA* p_wk );
static void ANKETO_QUESTION_DATA_Init( ANKETO_QUESTION_DATA* p_wk, BOOL now, u32 heapID );
static void ANKETO_QUESTION_DATA_Exit( ANKETO_QUESTION_DATA* p_wk );
static STRBUF* ANKETO_QUESTION_DATA_GetQuestionStr( const ANKETO_QUESTION_DATA* cp_wk, ANKETO_MSGMAN* p_msg );
static STRBUF* ANKETO_QUESTION_DATA_GetAnswerStr( const ANKETO_QUESTION_DATA* cp_wk, ANKETO_MSGMAN* p_msg, u32 answer );

//-------------------------------------
///	���ʃf�[�^���[�N
//=====================================
static void ANKETO_QUESTION_RESULT_DebugInit( ANKETO_QUESTION_RESULT* p_wk );
static void ANKETO_QUESTION_RESULT_Init( ANKETO_QUESTION_RESULT* p_wk );
static u32 ANKETO_QUESTION_RESULT_GetBerWidth( const ANKETO_QUESTION_RESULT* cp_wk, u32 answer, u32 max_width );
static void ANKETO_QUESTION_RESULT_CalcHirobaResult( ANKETO_QUESTION_RESULT* p_wk, const WFLBY_SYSTEM* cp_system );


//-------------------------------------
///	���͉��
//=====================================
static void ANKETO_LOCAL_INPUT_Init( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_PARAM* p_param, u32 heapID );
static BOOL ANKETO_LOCAL_INPUT_Main( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_TALKWIN* p_talkwin, ANKETO_PARAM* p_param, u32 heapID );
static void ANKETO_LOCAL_INPUT_Exit( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys );

static void ANKETO_INPUT_Init( ANKETO_INPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, u32 heapID );
static BOOL ANKETO_INPUT_Main( ANKETO_INPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_TALKWIN* p_talkwin, ANKETO_PARAM* p_param, u32 heapID );
static void ANKETO_INPUT_Exit( ANKETO_INPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys );
static void ANKETO_INPUT_DrawAnswer( ANKETO_INPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys );
static void ANKETO_INPUT_DrawCursor( ANKETO_INPUT* p_wk );
static void ANKETO_INPUT_AnmCursor( ANKETO_INPUT* p_wk );


//-------------------------------------
///	�o�͉��
//=====================================
static void ANKETO_LOCAL_OUTPUT_Init( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_PARAM* p_param, u32 heapID );
static BOOL ANKETO_LOCAL_OUTPUT_Main( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_TALKWIN* p_talkwin, ANKETO_PARAM* p_param, u32 heapID );
static void ANKETO_LOCAL_OUTPUT_Exit( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys );

static void ANKETO_OUTPUT_Init( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_PARAM* p_param, u32 heapID );
static BOOL ANKETO_OUTPUT_Main( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_TALKWIN* p_talkwin, ANKETO_PARAM* p_param, u32 heapID );
static void ANKETO_OUTPUT_Exit( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys );
static void ANKETO_OUTPUT_DrawTitle( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, u32 msgidx, GF_PRINTCOLOR col );
static void ANKETO_OUTPUT_DrawAnswer( ANKETO_OUTPUT* p_wk, const ANKETO_QUESTION_DATA* cp_data, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, const WFLBY_ANKETO* cp_answer, u32 heapID, GF_PRINTCOLOR col, GF_PRINTCOLOR vipcol, BOOL vip );
static void ANKETO_OUTPUT_DrawBarStart( ANKETO_OUTPUT* p_wk, const ANKETO_QUESTION_RESULT* cp_data, ANKETO_DRAWSYS* p_drawsys, u32 palno );
static BOOL ANKETO_OUTPUT_DrawBarMain( ANKETO_OUTPUT* p_wk, ANKETO_DRAWSYS* p_drawsys );
static void ANKETO_OUTPUT_SetLastWeekGraphic( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, u32 heapID );


//-------------------------------------
///	��b�E�B���h�E
//=====================================
static void ANKETO_TalkWin_Init( ANKETO_TALKWIN* p_wk, ANKETO_DRAWSYS* p_sys, SAVEDATA* p_save, u32 heapID );
static void ANKETO_TalkWin_Exit( ANKETO_TALKWIN* p_wk, u32 heapID );
static void ANKETO_TalkWin_Print( ANKETO_TALKWIN* p_wk, const STRBUF* cp_str );
static void ANKETO_TalkWin_PrintAll( ANKETO_TALKWIN* p_wk, const STRBUF* cp_str );
static void ANKETO_TalkWin_StartTimeWait( ANKETO_TALKWIN* p_wk );
static void ANKETO_TalkWin_StopTimeWait( ANKETO_TALKWIN* p_wk );
static BOOL ANKETO_TalkWin_CheckTimeWait( const ANKETO_TALKWIN* cp_wk );
static BOOL ANKETO_TalkWin_EndWait( const ANKETO_TALKWIN* cp_wk );
static void ANKETO_TalkWin_Off( ANKETO_TALKWIN* p_wk );
static void ANKETO_TalkWin_StartYesNo( ANKETO_TALKWIN* p_wk, ANKETO_DRAWSYS* p_sys, u32 heapID );
static u32	ANKETO_TalkWin_MainYesNo( ANKETO_TALKWIN* p_wk, u32 heapID );
static void ANKETO_TalkWin_EndYesNo( ANKETO_TALKWIN* p_wk, u32 heapID );


//-----------------------------------------------------------------------------
/**
 *					���
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	BANK�ݒ�
//=====================================
static const GF_BGL_DISPVRAM sc_ANKETO_BANK = {
	GX_VRAM_BG_256_AB,				// ���C��2D�G���W����BG
	GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
	GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
	GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
	GX_VRAM_OBJ_16_G,				// ���C��2D�G���W����OBJ
	GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
	GX_VRAM_SUB_OBJ_NONE,			// �T�u2D�G���W����OBJ
	GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
	GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
	GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
};

//-------------------------------------
///	BG�ݒ�
//=====================================
static const GF_BGL_SYS_HEADER sc_BGINIT = {
	GX_DISPMODE_GRAPHICS,
	GX_BGMODE_0,
	GX_BGMODE_0,
	GX_BG0_AS_2D
};

//-------------------------------------
///	�a�f�R���g���[���f�[�^
//=====================================
#define ANKETO_BGCNT_NUM	( 4 )	// �a�f�R���g���[���e�[�u����
static const u32 sc_ANKETO_BGCNT_FRM[ ANKETO_BGCNT_NUM ] = {
	GF_BGL_FRAME0_M,
	GF_BGL_FRAME1_M,
	GF_BGL_FRAME2_M,
	GF_BGL_FRAME0_S,
};
static const GF_BGL_BGCNT_HEADER sc_ANKETO_BGCNT_DATA[ ANKETO_BGCNT_NUM ] = {
	{	// GF_BGL_FRAME0_M	�i�w�i�j
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
		3, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME1_M	�i�t���[���ʁj
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		1, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME2_M	�i���b�Z�[�W�ʁj
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
		2, 0, 0, FALSE
	},

	{	// GF_BGL_FRAME0_S	�i�����w�i�j
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},
};



//-------------------------------------
///	�e���	�֐��|�C���^
//=====================================
static void (*p_Init[ ANKETO_MOVE_TYPE_NUM ])( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_PARAM* p_param, u32 heapID ) = {
	ANKETO_LOCAL_INPUT_Init,
	ANKETO_LOCAL_OUTPUT_Init,
};
static BOOL (*p_Main[ ANKETO_MOVE_TYPE_NUM ])( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_TALKWIN* p_talkwin, ANKETO_PARAM* p_param, u32 heapID ) = {
	ANKETO_LOCAL_INPUT_Main,
	ANKETO_LOCAL_OUTPUT_Main,
};
static void (*p_Exit[ ANKETO_MOVE_TYPE_NUM ])( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys ) = {
	ANKETO_LOCAL_INPUT_Exit,
	ANKETO_LOCAL_OUTPUT_Exit,
};



//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�v���Z�X	������
 */
//-----------------------------------------------------------------------------
PROC_RESULT ANKETO_Init(PROC* p_proc, int* p_seq)
{
	ANKETO_WORK*	p_wk;
	ANKETO_PARAM*	p_param;


	p_param = PROC_GetParentWork( p_proc );

	//�q�[�v�G���A�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_ANKETO, 0x50000 );


	// ���[�N�쐬
	p_wk = PROC_AllocWork( p_proc, sizeof(ANKETO_WORK), HEAPID_ANKETO );
	memset( p_wk, 0, sizeof(ANKETO_WORK) );


	// �`��V�X�e��������
	ANKETO_InitGraphic( &p_wk->drawsys, p_param->p_save, HEAPID_ANKETO );

	// ���b�Z�[�W�V�X�e��������
	ANKETO_MsgManInit( &p_wk->msgsys, p_param->p_system, HEAPID_ANKETO );

	// ��b�E�B���h�E������
	ANKETO_TalkWin_Init( &p_wk->talkwin,  &p_wk->drawsys, p_param->p_save, HEAPID_ANKETO );

	// ���ꂼ��̉�ʂ̏�����
	GF_ASSERT( p_param->move_type < ANKETO_MOVE_TYPE_NUM );
	p_Init[ p_param->move_type ]( &p_wk->local_wk, &p_wk->msgsys, &p_wk->drawsys, p_param, HEAPID_ANKETO );

	// ���荞�ݐݒ�
	sys_VBlankFuncChange( ANKETO_VBlank, p_wk );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	return	PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�v���Z�X	���C��
 */
//-----------------------------------------------------------------------------
PROC_RESULT ANKETO_Main(PROC* p_proc, int* p_seq)
{
	ANKETO_WORK* p_wk;
	ANKETO_PARAM* p_param;
	BOOL result;

	p_wk	= PROC_GetWork( p_proc );
	p_param = PROC_GetParentWork( p_proc );

	switch( (*p_seq) ){
	case 0:
		WIPE_SYS_Start(WIPE_PATTERN_WMS, 
				WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_ANKETO );

		// BGM���t�F�[�h�A�E�g
		WFLBY_SYSTEM_SetBGMVolumeDown( p_param->p_system, TRUE );

		(*p_seq) ++;
		break;

	case 1:
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){
			(*p_seq) ++;
		}
		break;

	case 2:

		// �I���`�F�b�N
		if( WIPE_SYS_EndCheck() == TRUE ){

			if( WFLBY_SYSTEM_Event_GetEndCM( p_param->p_system ) == TRUE ){
				WFLBY_SYSTEM_APLFLAG_SetForceEnd( p_param->p_system );
				(*p_seq) ++;
				break;
			}

			if( WFLBY_ERR_CheckError() == TRUE ){
				(*p_seq) ++;
				break;
			}
		}
		

		// ���ꂼ��̃��C���֐�
		result = p_Main[ p_param->move_type ]( &p_wk->local_wk, &p_wk->msgsys, &p_wk->drawsys, &p_wk->talkwin, p_param, HEAPID_ANKETO );

		if( result == TRUE ){
			(*p_seq) ++;
		}
		break;

	case 3:
		WIPE_SYS_Start(WIPE_PATTERN_WMS, 
				WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_ANKETO );
		(*p_seq) ++;
		break;

	case 4:
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){

			// BTS�ʐM549�o�O�̏C��
			// TimeWaitIcon�̒�~
			ANKETO_TalkWin_StopTimeWait( &p_wk->talkwin );
			return PROC_RES_FINISH;
		}
		break;
	}

	// �`��V�X�e��
	ANKETO_MainGraphic( &p_wk->drawsys );
	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�v���Z�X	�j��
 */
//-----------------------------------------------------------------------------
PROC_RESULT ANKETO_Exit(PROC* p_proc, int* p_seq)
{
	ANKETO_WORK* p_wk;
	ANKETO_PARAM* p_param;

	p_wk	= PROC_GetWork( p_proc );
	p_param = PROC_GetParentWork( p_proc );

	// ���荞�ݐݒ�
	sys_VBlankFuncChange( NULL, NULL );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~


	// ���ꂼ��̔j��
	p_Exit[ p_param->move_type ]( &p_wk->local_wk, &p_wk->msgsys, &p_wk->drawsys );

	// ��b�E�B���h�E�j��
	ANKETO_TalkWin_Exit( &p_wk->talkwin, HEAPID_ANKETO );

	// ���b�Z�[�W�V�X�e���j��
	ANKETO_MsgManExit( &p_wk->msgsys );

	// �O���t�B�b�N���[�N�j��
	ANKETO_ExitGraphic( &p_wk->drawsys );

	//���[�N�j��
	PROC_FreeWork( p_proc );
	
	//�q�[�v�j��
	sys_DeleteHeap( HEAPID_ANKETO );

	// SE�X�g�b�v
	Snd_SeStopAll(0);

	return PROC_RES_FINISH;
}





//-----------------------------------------------------------------------------
/**
 *			�Ղ炢�ׁ[�Ɗ֐�
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�gVBlank����
 *
 *	@param	p_work	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_VBlank( void* p_work )
{
	ANKETO_WORK* p_wk = p_work;
	
	ANKETO_VBlankGraphic( &p_wk->drawsys );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N������
 *
 *	@param	p_wk		���[�N
 *	@param	p_save		�Z�[�u�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_InitGraphic( ANKETO_DRAWSYS* p_wk, SAVEDATA* p_save, u32 heapID )
{
	CONFIG*			p_config;
	
	// �R���t�B�O�f�[�^�擾
	p_config = SaveData_GetConfig( p_save );
	
	// �A�[�J�C�u�n���h��������
	p_wk->p_handle = ArchiveDataHandleOpen( ARC_WIFILOBBY_OTHER_GRA, heapID );

	// Vram�]���}�l�[�W���쐬
	initVramTransferManagerHeap( ANKETO_VRAMTRANS_TASKNUM, heapID );

	// �o���N�ݒ�
	GF_Disp_SetBank( &sc_ANKETO_BANK );

	// BG������
	ANKETO_BgInit( p_wk, p_config, heapID );	

	// OAM�ݒ�
	ANKETO_OamInit( p_wk, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_ExitGraphic( ANKETO_DRAWSYS* p_wk )
{
	// �A�[�J�C�u�n���h��
	ArchiveDataHandleClose( p_wk->p_handle );

	// Vram�]���}�l�[�W���j��
	DellVramTransferManager();
	
	// BG�ݒ�
	ANKETO_BgExit( p_wk );

	// OAM�ݒ�
	ANKETO_OamExit( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N���C������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_MainGraphic( ANKETO_DRAWSYS* p_wk )
{
	CLACT_Draw( p_wk->p_clactset );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�NVBlank����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_VBlankGraphic( ANKETO_DRAWSYS* p_wk )
{
    // BG��������
    GF_BGL_VBlankFunc( p_wk->p_bgl );

    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();

	// Vram�]��
	DoVramTransferManager();
}



//----------------------------------------------------------------------------
/**
 *	@brief	BG������
 *
 *	@param	p_wk		���[�N
 *	@param	p_config	�R���t�B�O�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_BgInit( ANKETO_DRAWSYS* p_wk, CONFIG* p_config, u32 heapID )
{
	// BG�ݒ�
	GF_BGL_InitBG(&sc_BGINIT);
	
	// BGL�쐬
	p_wk->p_bgl = GF_BGL_BglIniAlloc( heapID );

	// ���C���ƃT�u��؂�ւ���
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();


	// BG�R���g���[���ݒ�
	{
		int i;

		for( i=0; i<ANKETO_BGCNT_NUM; i++ ){
			GF_BGL_BGControlSet( p_wk->p_bgl, 
					sc_ANKETO_BGCNT_FRM[i], &sc_ANKETO_BGCNT_DATA[i],
					GF_BGL_MODE_TEXT );
			GF_BGL_ClearCharSet( sc_ANKETO_BGCNT_FRM[i], 32, 0, heapID);
			GF_BGL_ScrClear( p_wk->p_bgl, sc_ANKETO_BGCNT_FRM[i] );
		}
	}

	// �t�H���g�J���[
    TalkFontPaletteLoad( PALTYPE_MAIN_BG, ANKETO_PLTT_MAIN_TALKFONT*0x20, heapID );

	// �g�[�N�E�B���h�E
	{
		u8 win_num = CONFIG_GetWindowType( p_config );
		TalkWinGraphicSet( p_wk->p_bgl, GF_BGL_FRAME1_M,
				ANKETO_TALKWIN_CGX, ANKETO_TALKWIN_PAL,
				win_num, heapID );
	}

	// �V�X�e���E�B���h�E
    MenuWinGraphicSet(
        p_wk->p_bgl, GF_BGL_FRAME1_M, 
		ANKETO_SYSTEMWIN_CGX, ANKETO_SYSTEMWIN_PAL, 0, heapID );


	// ������ʂɁAWi�|Fi�}�[�N���o��
	{
		ArcUtil_BgCharSet(ARC_LOBBY_NEWS, NARC_lobby_news_wifi_mark_bg_NCGR, p_wk->p_bgl, GF_BGL_FRAME0_S, 0, 0, FALSE, heapID);
		ArcUtil_ScrnSet(ARC_LOBBY_NEWS, NARC_lobby_news_wifi_mark_bg_NSCR, p_wk->p_bgl, GF_BGL_FRAME0_S, 0, 0, FALSE, heapID);
		ArcUtil_PalSet( ARC_LOBBY_NEWS, NARC_lobby_news_lobby_news_bg_NCLR, PALTYPE_SUB_BG, 0, 0, heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_BgExit( ANKETO_DRAWSYS* p_wk )
{
	// �a�f�R���g���[���j��
	{
		int i;

		for( i=0; i<ANKETO_BGCNT_NUM; i++ ){
			GF_BGL_BGControlExit( p_wk->p_bgl, sc_ANKETO_BGCNT_FRM[i] );
		}
	}
	
	// BGL�j��
	sys_FreeMemoryEz( p_wk->p_bgl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM�V�X�e���̏�����
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_OamInit( ANKETO_DRAWSYS* p_wk, u32 heapID )
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
    InitCharManagerReg(&sc_ANKETO_CHARMAN_INIT, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K );
    // �p���b�g�}�l�[�W���[������
    InitPlttManager(ANKETO_OAM_CONTNUM, heapID);

    // �ǂݍ��݊J�n�ʒu��������
    CharLoadStartAll();
    PlttLoadStartAll();

    //�ʐM�A�C�R���p�ɃL�������p���b�g����
    CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
    

    // �Z���A�N�^�[�Z�b�g�쐬
    p_wk->p_clactset = CLACT_U_SetEasyInit( ANKETO_OAM_CONTNUM, &p_wk->renddata, heapID );

    // �L�����ƃp���b�g�̃��\�[�X�}�l�[�W���쐬
    for( i=0; i<ANKETO_RESMAN_NUM; i++ ){
        p_wk->p_resman[i] = CLACT_U_ResManagerInit(ANKETO_OAM_CONTNUM, i, heapID);
    }

	// ����ʂɒʐM�A�C�R�����o��
	WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��


	// �\���J�n
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM�V�X�e���̔j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_OamExit( ANKETO_DRAWSYS* p_wk )
{
    int i;

    // �A�N�^�[�̔j��
    CLACT_DestSet( p_wk->p_clactset );

    for( i=0; i<ANKETO_RESMAN_NUM; i++ ){
        CLACT_U_ResManagerDelete( p_wk->p_resman[i] );
    }

    // ���\�[�X���
    DeleteCharManager();
    DeletePlttManager();

    //OAM�����_���[�j��
    REND_OAM_Delete();
}


//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�Ǘ��V�X�e��������
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_MsgManInit( ANKETO_MSGMAN* p_wk, WFLBY_SYSTEM* p_system, u32 heapID )
{
	int i;
	static const sc_ANKETO_MSGMANTBL[ ANKETO_MSGMAN_NUM ] = {
		NARC_msg_wifi_hiroba_dat,
		NARC_msg_wifi_50_questions_dat,
		NARC_msg_wifi_Special_questions_dat,
		NARC_msg_wifi_50_answers_a_dat,
		NARC_msg_wifi_50_answers_b_dat,
		NARC_msg_wifi_50_answers_c_dat,
		NARC_msg_wifi_Special_answers_a_dat,
		NARC_msg_wifi_Special_answers_b_dat,
		NARC_msg_wifi_Special_answers_c_dat,
	};

	p_wk->p_system = p_system;

	// ���b�Z�[�W�}�l�[�W��
	for( i=0; i<ANKETO_MSGMAN_NUM; i++ ){
		p_wk->p_msgman[i] = MSGMAN_Create(MSGMAN_TYPE_NORMAL,ARC_MSG,
				sc_ANKETO_MSGMANTBL[i],heapID );
	}

	// ���[�h�Z�b�g
	p_wk->p_wordset = WORDSET_Create( heapID );

	// ���L���b�Z�[�W�̈�m��
	p_wk->p_msgstr = STRBUF_Create( ANKETO_MSGMAN_STRBUFNUM, heapID );
	p_wk->p_msgtmp = STRBUF_Create( ANKETO_MSGMAN_STRBUFNUM, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�Ǘ��V�X�e���j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_MsgManExit( ANKETO_MSGMAN* p_wk )
{
	int i;

	for( i=0; i<ANKETO_MSGMAN_NUM; i++ ){
		MSGMAN_Delete(p_wk->p_msgman[i]);
	}

	WORDSET_Delete( p_wk->p_wordset );

	// ���L���b�Z�[�W�o�b�t�@
	STRBUF_Delete( p_wk->p_msgstr );
	STRBUF_Delete( p_wk->p_msgtmp );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�̎擾
 *
 *	@param	p_wk	���[�N
 *	@param	type	�}�l�[�W���^�C�v
 *	@param	msg		���b�Z�[�W�i���o�[
 *
 *	@return	���b�Z�[�W
 */
//-----------------------------------------------------------------------------
static STRBUF* ANKETO_MsgManGetStr( ANKETO_MSGMAN* p_wk, u32 type, u32 msg )
{
	GF_ASSERT( type < ANKETO_MSGMAN_NUM );
	
	MSGMAN_GetString( p_wk->p_msgman[type], msg, p_wk->p_msgtmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_msgstr, p_wk->p_msgtmp );
	return p_wk->p_msgstr;
}

//----------------------------------------------------------------------------
/**
 *	@brief	msg�i���o�[�ɂ����������Ԃ�
 *
 *	@param	p_wk	���[�N
 *	@param	msg		���b�Z�[�W�i���o�[�i�O�`�S�X�ʏ�@�T�O�`�T�X���ʁj
 *
 *	@return	���b�Z�[�W
 */
//-----------------------------------------------------------------------------
static STRBUF* ANKETO_MsgManGetQuestionStr( ANKETO_MSGMAN* p_wk, u32 msg )
{
	if( msg >= (ANKETO_NORMAL_Q_NUM+ANKETO_SPECIAL_Q_NUM) ){
		GF_ASSERT( msg >= (ANKETO_NORMAL_Q_NUM+ANKETO_SPECIAL_Q_NUM) );
		msg = 0;
	}
	
	if( msg < ANKETO_NORMAL_Q_NUM ){
		return ANKETO_MsgManGetStr( p_wk, ANKETO_MSGMAN_Q_NORMAL, msg );
	}
		
	return ANKETO_MsgManGetStr( p_wk, ANKETO_MSGMAN_Q_SPECIAL, msg-ANKETO_NORMAL_Q_NUM );
}

//----------------------------------------------------------------------------
/**
 *	@brief	msg�i���o�[�ɂ������񓚂�Ԃ�
 *
 *	@param	p_wk	���[�N
 *	@param	msg		���b�Z�[�W�i���o�[�i�O�`�S�X�ʏ�@�T�O�`�T�X���ʁj
 *	@param	answer	�񓚃^�C�v�iANKETO_ANSWER_A�@�E�E�E�j
 *
 *	@return	��
 */
//-----------------------------------------------------------------------------
static STRBUF* ANKETO_MsgManGetAnswerStr( ANKETO_MSGMAN* p_wk, u32 msg, u32 answer )
{
	if( msg >= (ANKETO_NORMAL_Q_NUM+ANKETO_SPECIAL_Q_NUM) ){
		GF_ASSERT( msg >= (ANKETO_NORMAL_Q_NUM+ANKETO_SPECIAL_Q_NUM) );
		msg = 0;
	}
	
	if( msg < ANKETO_NORMAL_Q_NUM ){
		return ANKETO_MsgManGetStr( p_wk, ANKETO_MSGMAN_A_N_A+answer, msg );
	}
		
	return ANKETO_MsgManGetStr( p_wk, ANKETO_MSGMAN_A_S_A+answer, msg - ANKETO_NORMAL_Q_NUM );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̖��O�����[�h�Z�b�g�ɐݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	bufno		�o�b�t�@ID
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_MsgManSetMyName( ANKETO_MSGMAN* p_wk, u32 bufno, u32 heapID )
{
	MYSTATUS* p_mystatus;
	const WFLBY_USER_PROFILE* cp_profile;

	p_mystatus = MyStatus_AllocWork( heapID );
	cp_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_wk->p_system );
	WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_mystatus, heapID );

	// ���O��ݒ�
	WORDSET_RegisterPlayerName( p_wk->p_wordset, bufno, p_mystatus );

	sys_FreeMemoryEz( p_mystatus );
}




//----------------------------------------------------------------------------
/**
 *	@brief	LOCAL_WK�����ʃ��[�N�Ƃ��ăC���v�b�g���Ǘ�����
 */
//-----------------------------------------------------------------------------
static void ANKETO_LOCAL_INPUT_Init( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_PARAM* p_param, u32 heapID )
{
	ANKETO_INPUT_Init( &p_wk->input, p_msg, p_drawsys, heapID );
}

static BOOL ANKETO_LOCAL_INPUT_Main( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_TALKWIN* p_talkwin, ANKETO_PARAM* p_param, u32 heapID )
{
	return ANKETO_INPUT_Main( &p_wk->input, p_msg, p_drawsys, p_talkwin, p_param, heapID );
}

static void ANKETO_LOCAL_INPUT_Exit( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys )
{
	ANKETO_INPUT_Exit( &p_wk->input, p_msg, p_drawsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���͉��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W�V�X�e��
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_INPUT_Init( ANKETO_INPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, u32 heapID )
{
	// �w�i�ݒ�
	{
		ArcUtil_HDL_PalSet( p_drawsys->p_handle, NARC_wifi_lobby_other_bg_NCLR, PALTYPE_MAIN_BG, ANKETO_PLTT_MAIN_BACK00, ANKETO_PLTT_MAINBACK_PLTTNUM*32, heapID );
		ArcUtil_HDL_BgCharSet( p_drawsys->p_handle, NARC_wifi_lobby_other_bg_NCGR, p_drawsys->p_bgl, GF_BGL_FRAME0_M, 0, 0, FALSE, heapID );
		ArcUtil_HDL_ScrnSet( p_drawsys->p_handle, NARC_wifi_lobby_other_bg00_NSCR, p_drawsys->p_bgl, GF_BGL_FRAME0_M, 0, 0, FALSE, heapID );
	}

	// OAM�ǂݍ���
	{
		BOOL result;

		// �L�����N�^�ǂݍ���
		p_wk->p_cursor_res[0] = CLACT_U_ResManagerResAddArcChar_ArcHandle( 
				p_drawsys->p_resman[0], p_drawsys->p_handle,
				NARC_wifi_lobby_other_oam_NCGR,
				FALSE, ANKETO_INPUT_OAM_CONTID, NNS_G2D_VRAM_TYPE_2DMAIN, heapID );

		p_wk->p_cursor_res[1] = CLACT_U_ResManagerResAddArcPltt_ArcHandle( 
				p_drawsys->p_resman[1], p_drawsys->p_handle,
				NARC_wifi_lobby_other_oam_NCLR,
				FALSE, ANKETO_INPUT_OAM_CONTID, 
				NNS_G2D_VRAM_TYPE_2DMAIN, 3, heapID );
		result =CLACT_U_CharManagerSetAreaCont( p_wk->p_cursor_res[0] );
		GF_ASSERT( result );
		result =CLACT_U_PlttManagerSetCleanArea( p_wk->p_cursor_res[1] );
		GF_ASSERT( result );
		CLACT_U_ResManagerResOnlyDelete( p_wk->p_cursor_res[0] );
		CLACT_U_ResManagerResOnlyDelete( p_wk->p_cursor_res[1] );

		// �Z��
		p_wk->p_cursor_res[2] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_drawsys->p_resman[2], p_drawsys->p_handle,
				NARC_wifi_lobby_other_oam_NCER,
				FALSE, ANKETO_INPUT_OAM_CONTID, CLACT_U_CELL_RES, heapID );

		// �Z���A�j��
		p_wk->p_cursor_res[3] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_drawsys->p_resman[3], p_drawsys->p_handle,
				NARC_wifi_lobby_other_oam_NANR,
				FALSE, ANKETO_INPUT_OAM_CONTID, CLACT_U_CELLANM_RES, heapID );
	}

	// cursor�쐬
	{
		CLACT_HEADER		header;
		CLACT_ADD_SIMPLE	add = {0};

		// �w�b�_�[�쐬
		CLACT_U_MakeHeader( &header, ANKETO_INPUT_OAM_CONTID, 
				ANKETO_INPUT_OAM_CONTID, 
				ANKETO_INPUT_OAM_CONTID, 
				ANKETO_INPUT_OAM_CONTID, 
				CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
				0, ANKETO_INPUT_OAM_BGPRI,
				p_drawsys->p_resman[0],
				p_drawsys->p_resman[1],
				p_drawsys->p_resman[2],
				p_drawsys->p_resman[3],
				NULL, NULL );


		add.ClActSet	= p_drawsys->p_clactset;
		add.ClActHeader = &header;
		add.pri			= 0;
		add.DrawArea	= NNS_G2D_VRAM_TYPE_2DMAIN;
		add.heap		= heapID;

		// �A�N�^�[�쐬
		p_wk->p_cursor_act = CLACT_AddSimple( &add );
		CLACT_SetDrawFlag( p_wk->p_cursor_act, FALSE );
	}
	
	// �E�B���h�E�쐬
	{
		int i;

		for( i=0; i<ANKETO_INPUT_BMP_NUM; i++ ){
			GF_BGL_BmpWinAddEx( p_drawsys->p_bgl, &p_wk->win[i], 
					&sc_ANKETO_INPUT_BMPDAT[i] );
		}
	}

	ANKETO_QUESTION_DATA_Init( &p_wk->question_now, TRUE, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���͉�ʃ��C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W�V�X�e��
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	heapID		�q�[�vID	�������n�����ǁAINPUT_Exit�����Ă΂�Ă����v�Ȃ悤�ɂ��邱��
 */
//-----------------------------------------------------------------------------
static BOOL ANKETO_INPUT_Main( ANKETO_INPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_TALKWIN* p_talkwin, ANKETO_PARAM* p_param, u32 heapID )
{
	u32 msg_idx;
	STRBUF* p_str;
	
	switch( p_wk->seq ){
	case ANKETO_INPUT_SEQ_ANKETO_YARU_Q:		// �A���P�[�g���H
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_b_n02 );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_INPUT_SEQ_ANKETO_YARU_Q_ON;
		p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_MASWAIT;
		break;

	case ANKETO_INPUT_SEQ_ANKETO_YARU_Q_ON:// �A���P�[�g���H
		ANKETO_TalkWin_StartYesNo( p_talkwin, p_drawsys, heapID );
		p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_YARU_Q_WAIT;
		break;

	case ANKETO_INPUT_SEQ_ANKETO_YARU_Q_WAIT:// �A���P�[�g���H
		{
			u32 result;

			result = ANKETO_TalkWin_MainYesNo( p_talkwin, heapID );
			switch( result ){
			// �͂�
			case 0:
				// �I����
				p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_Q_DRAW00;
				break;

			// �́[
			case BMPMENU_CANCEL:
				// �I����
				p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_END;
				break;
			}
		}
		break;


#if 0
	case ANKETO_INPUT_SEQ_ANKETO_RECV:		// ��M�J�n

		Snd_SePlay( ANKETO_SND_RECV_WAIT );

//		ANKETO_QUESTION_DATA_DebugInit( &p_wk->question_now );

		// ��M���b�Z�[�W
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_02_01 );
		ANKETO_TalkWin_PrintAll( p_talkwin, p_str );

		// �^�C���E�G�C�g�\��
		ANKETO_TalkWin_StartTimeWait( p_talkwin );

		p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_RECVWAIT;
		break;

	case ANKETO_INPUT_SEQ_ANKETO_RECVWAIT:	// ��M��

		if( (gf_mtRand() % 100) == 0 ){

			Snd_SePlay( ANKETO_SND_RECV );

			ANKETO_TalkWin_StopTimeWait( p_talkwin );


			p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_02_02 );
			ANKETO_TalkWin_Print( p_talkwin, p_str );

			p_wk->ret_seq = ANKETO_INPUT_SEQ_ANKETO_Q_DRAW00;
			p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_MASWAIT;
		}
		break;
#endif

	case ANKETO_INPUT_SEQ_ANKETO_Q_DRAW00:	// ����`��
		if( p_wk->question_now.special == FALSE ){
			msg_idx = msg_survey_monitor_b_n01;
		}else{
			msg_idx = msg_survey_monitor_b_s01;
		}
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_idx );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_INPUT_SEQ_ANKETO_Q_DRAW;
		p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_MASWAIT;
		break;

	case ANKETO_INPUT_SEQ_ANKETO_Q_DRAW:		// ����`��
		// �񓚓��e��\��
		ANKETO_INPUT_DrawAnswer( p_wk, p_msg, p_drawsys );

		// �����\��
		p_str = ANKETO_QUESTION_DATA_GetQuestionStr( &p_wk->question_now, p_msg );
		ANKETO_TalkWin_PrintAll( p_talkwin, p_str );

		// OAM��\��
		ANKETO_INPUT_DrawCursor( p_wk );

		// OAM�A�j��
		ANKETO_INPUT_AnmCursor( p_wk );
		
		p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_Q_SELECT;
		break;

	case ANKETO_INPUT_SEQ_ANKETO_Q_SELECT:	// �񓚑I��

		// A�őI��
		if( sys.trg & PAD_BUTTON_DECIDE ){
			Snd_SePlay( ANKETO_SND_SELECT );

			p_wk->seq ++;
			break;
		}
		
		if( sys.trg & PAD_KEY_UP ){
			if( (p_wk->cursor - 1) >= 0 ){
				Snd_SePlay( ANKETO_SND_CURSOR );
				p_wk->cursor --;
			}
		}else if( sys.trg & PAD_KEY_DOWN ){
			if( (p_wk->cursor + 1) < ANKETO_ANSWER_NUM ){
				Snd_SePlay( ANKETO_SND_CURSOR );
				p_wk->cursor ++;
			}
		}

		ANKETO_INPUT_DrawCursor( p_wk );
		break;

	case ANKETO_INPUT_SEQ_ANKETO_SEND:		// ��M�J�n

		// ��{���ɑI�����ڂ�ݒ�
		WFLBY_SYSTEM_SetAnketoData( p_param->p_system, 
				p_wk->question_now.anketo_no, p_wk->cursor );

		// �T�[�o�ɑ��M
		DWC_LOBBY_ANKETO_SubMit( p_wk->cursor );

		// ��M���b�Z�[�W
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_b_02_01 );
		ANKETO_TalkWin_PrintAll( p_talkwin, p_str );

		// �^�C���E�G�C�g�\��
		ANKETO_TalkWin_StartTimeWait( p_talkwin );

		Snd_SePlay( ANKETO_SND_SEND_WAIT );

		p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_SENDWAIT;
		break;

	case ANKETO_INPUT_SEQ_ANKETO_SENDWAIT:	// ��M��
		{
			u32 send_result;
			send_result = DWC_LOBBY_ANKETO_WaitSubMit();
			if( send_result != DWC_LOBBY_ANKETO_STATE_SENDING ){

				Snd_SePlay( ANKETO_SND_SEND );

				ANKETO_TalkWin_StopTimeWait( p_talkwin );


				p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_b_02_02 );
				ANKETO_TalkWin_Print( p_talkwin, p_str );

				p_wk->ret_seq = ANKETO_INPUT_SEQ_ANKETO_ARIGATO;
				p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_MASWAIT;
			}
		}
		break;

	case ANKETO_INPUT_SEQ_ANKETO_ARIGATO:	// ���肪�Ƃ����b�Z�[�W�`��
		if( p_wk->question_now.special == FALSE ){
			msg_idx = msg_survey_monitor_b_n03;
		}else{
			msg_idx = msg_survey_monitor_b_s03;
		}
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_idx );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_INPUT_SEQ_ANKETO_END;
		p_wk->seq = ANKETO_INPUT_SEQ_ANKETO_MASWAIT;
		break;

	case ANKETO_INPUT_SEQ_ANKETO_END:		// �I��
		// ��b�E�B���h�E��\��
		ANKETO_TalkWin_Off( p_talkwin );
		return TRUE;

	case ANKETO_INPUT_SEQ_ANKETO_MASWAIT:	// ���b�Z�[�W�҂�
		if( ANKETO_TalkWin_EndWait( p_talkwin ) == TRUE ){
			p_wk->seq = p_wk->ret_seq;
		}
		break;

	default:
		GF_ASSERT(0);
		break;

	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���͉��	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W�V�X�e��
 *	@param	p_drawsys	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void ANKETO_INPUT_Exit( ANKETO_INPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys )
{
	// ����f�[�^
	ANKETO_QUESTION_DATA_Exit( &p_wk->question_now );
	
	// �E�B���h�E�j��
	{
		int i;

		for( i=0; i<ANKETO_INPUT_BMP_NUM; i++ ){
			GF_BGL_BmpWinDel( &p_wk->win[i] );
		}
	}


	// cursor�j��
	{
		CLACT_Delete( p_wk->p_cursor_act );
	}

	// OAM�j��
	{
		int i;
		
		// VRAM�̈�j��
		CLACT_U_CharManagerDelete( p_wk->p_cursor_res[0] );
		CLACT_U_PlttManagerDelete( p_wk->p_cursor_res[1] );

		for( i=0; i<ANKETO_RESMAN_NUM; i++ ){
			CLACT_U_ResManagerResDelete( p_drawsys->p_resman[i], p_wk->p_cursor_res[i] );
		
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�񓚂�`�悷��
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W���[�N
 *	@param	p_drawsys	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void ANKETO_INPUT_DrawAnswer( ANKETO_INPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys )
{
	STRBUF* p_str;

	GF_BGL_BmpWinDataFill( &p_wk->win[ANKETO_INPUT_BMP_ANS], 0 );

	p_str = ANKETO_QUESTION_DATA_GetAnswerStr( &p_wk->question_now, p_msg, ANKETO_ANSWER_A );
	GF_STR_PrintColor( &p_wk->win[ANKETO_INPUT_BMP_ANS], 
			FONT_TALK, p_str, 0, ANKETO_INPUTWIN_ANS00_Y,
			MSG_NO_PUT, ANKETO_INPUT_ANSWER_COL, NULL );

	p_str = ANKETO_QUESTION_DATA_GetAnswerStr( &p_wk->question_now, p_msg, ANKETO_ANSWER_B );
	GF_STR_PrintColor( &p_wk->win[ANKETO_INPUT_BMP_ANS], 
			FONT_TALK, p_str, 0, ANKETO_INPUTWIN_ANS01_Y,
			MSG_NO_PUT, ANKETO_INPUT_ANSWER_COL, NULL );

	p_str = ANKETO_QUESTION_DATA_GetAnswerStr( &p_wk->question_now, p_msg, ANKETO_ANSWER_C );
	GF_STR_PrintColor( &p_wk->win[ANKETO_INPUT_BMP_ANS], 
			FONT_TALK, p_str, 0, ANKETO_INPUTWIN_ANS02_Y,
			MSG_NO_PUT, ANKETO_INPUT_ANSWER_COL, NULL );

	GF_BGL_BmpWinOnVReq( &p_wk->win[ANKETO_INPUT_BMP_ANS] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	cursor�`��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_INPUT_DrawCursor( ANKETO_INPUT* p_wk )
{
	CLACT_SetDrawFlag( p_wk->p_cursor_act, TRUE );

	CLACT_SetMatrix( p_wk->p_cursor_act, &sc_ANKETO_INPUT_CURSOR_MAT[ p_wk->cursor ] );
}
static void ANKETO_INPUT_AnmCursor( ANKETO_INPUT* p_wk )
{
	CLACT_SetAnmFlag( p_wk->p_cursor_act, TRUE );
}



//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E�V�X�e��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 *	@param	p_save		�Z�[�u�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_TalkWin_Init( ANKETO_TALKWIN* p_wk, ANKETO_DRAWSYS* p_sys, SAVEDATA* p_save, u32 heapID )
{
	//  �r�b�g�}�b�v�m��
	GF_BGL_BmpWinAdd(
				p_sys->p_bgl, &p_wk->win, 
				GF_BGL_FRAME1_M,
				ANKETO_TALK_TALKWIN_X, ANKETO_TALK_TALKWIN_Y,
				ANKETO_TALK_TALKWIN_SIZX, ANKETO_TALK_TALKWIN_SIZY,
				ANKETO_PLTT_MAIN_TALKFONT,
				ANKETO_TALK_TALKWIN_CGX );

	// �N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );

	// ������o�b�t�@�쐬
	p_wk->p_str = STRBUF_Create( ANKETO_MSGMAN_STRBUFNUM, heapID );

	// ���b�Z�[�W�\���E�G�C�g��ݒ�
	{
		CONFIG* p_config;
		p_config = SaveData_GetConfig( p_save );
		p_wk->msgwait = CONFIG_GetMsgPrintSpeed( p_config );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E	�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_TalkWin_Exit( ANKETO_TALKWIN* p_wk, u32 heapID )
{
	//  ���ׂĒ�~
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}
	ANKETO_TalkWin_StopTimeWait( p_wk );
	
	// ������o�b�t�@�j��
	STRBUF_Delete( p_wk->p_str );

	// �r�b�g�}�b�v�j��
	GF_BGL_BmpWinDel( &p_wk->win );

	// YESNO�E�B���h�E�j��
	ANKETO_TalkWin_EndYesNo( p_wk, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�J�n
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		����
 */
//-----------------------------------------------------------------------------
static void ANKETO_TalkWin_Print( ANKETO_TALKWIN* p_wk, const STRBUF* cp_str )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �r�b�g�}�b�v�̃N���A
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
	
	// ������R�s�[
	STRBUF_Copy( p_wk->p_str, cp_str );
	p_wk->msgno = GF_STR_PrintColor( &p_wk->win, FONT_TALK, p_wk->p_str, 0, 0,
			p_wk->msgwait, ANKETO_TALK_COL, NULL );

	// �E�B���h�E����������
	BmpTalkWinWrite( &p_wk->win, WINDOW_TRANS_OFF, ANKETO_TALKWIN_CGX, ANKETO_TALKWIN_PAL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�J�n	�S�\��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
static void ANKETO_TalkWin_PrintAll( ANKETO_TALKWIN* p_wk, const STRBUF* cp_str )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �r�b�g�}�b�v�̃N���A
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
	
	// ������R�s�[
	STRBUF_Copy( p_wk->p_str, cp_str );
	GF_STR_PrintColor( &p_wk->win, FONT_TALK, p_wk->p_str, 0, 0,
			MSG_NO_PUT, ANKETO_TALK_COL, NULL );

	// �E�B���h�E����������
	BmpTalkWinWrite( &p_wk->win, WINDOW_TRANS_OFF, ANKETO_TALKWIN_CGX, ANKETO_TALKWIN_PAL );
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���A�E�g�E�G�C�g	�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_TalkWin_StartTimeWait( ANKETO_TALKWIN* p_wk )
{
	GF_ASSERT( p_wk->p_timewait == NULL );
	
	p_wk->p_timewait = TimeWaitIconAdd( &p_wk->win, ANKETO_TALKWIN_CGX );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���A�E�g�E�G�C�g	�I��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_TalkWin_StopTimeWait( ANKETO_TALKWIN* p_wk )
{
	if( p_wk->p_timewait != NULL ){
		TimeWaitIconDel( p_wk->p_timewait );
		p_wk->p_timewait = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���E�G�C�g	�`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�^�C���E�G�C�g��
 *	@retval	FALSE	�^�C���E�G�C�g�\�����ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL ANKETO_TalkWin_CheckTimeWait( const ANKETO_TALKWIN* cp_wk )
{
	if( cp_wk->p_timewait != NULL ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\���I��
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL ANKETO_TalkWin_EndWait( const ANKETO_TALKWIN* cp_wk )
{
	if( GF_MSG_PrintEndCheck( cp_wk->msgno ) == 0 ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E��\��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_TalkWin_Off( ANKETO_TALKWIN* p_wk )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �^�C���E�G�C�g������
	ANKETO_TalkWin_StopTimeWait( p_wk );

	// �S�̂�����
	BmpTalkWinClear( &p_wk->win, WINDOW_TRANS_OFF );

	// V���N�G�X�g
	GF_BGL_BmpWinOffVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	YesNo�E�B���h�E	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_TalkWin_StartYesNo( ANKETO_TALKWIN* p_wk, ANKETO_DRAWSYS* p_sys, u32 heapID )
{
	GF_ASSERT( p_wk->p_yesno == NULL );
	p_wk->p_yesno = BmpYesNoSelectInit( p_sys->p_bgl, &sc_YESNO_BMPDAT, 
			ANKETO_SYSTEMWIN_CGX, ANKETO_SYSTEMWIN_PAL, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	YesNo�E�B���h�E	���C��
 *
 *	@param	p_wk	���[�N
 *
 * @retval	"BMPMENU_NULL	�I������Ă��Ȃ�"
 * @retval	"0				�͂���I��"
 * @retval	"BMPMENU_CANCEL	������or�L�����Z��"
 */
//-----------------------------------------------------------------------------
static u32	ANKETO_TalkWin_MainYesNo( ANKETO_TALKWIN* p_wk, u32 heapID )
{
	u32 ret;
	ret = BmpYesNoSelectMain( p_wk->p_yesno, heapID );
	if( ret != BMPMENU_NULL ){
		p_wk->p_yesno = NULL;
	}
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	YesNo�E�B���h�E�j��
 *
 *	@param	p_wk	���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_TalkWin_EndYesNo( ANKETO_TALKWIN* p_wk, u32 heapID )
{
	if( p_wk->p_yesno != NULL ){
		BmpYesNoWinDel( p_wk->p_yesno, heapID );
		p_wk->p_yesno = NULL;
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	���[�J�����[�N���g�p���ďo�͉�ʂ�\��
 */
//-----------------------------------------------------------------------------
static void ANKETO_LOCAL_OUTPUT_Init( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_PARAM* p_param, u32 heapID )
{
	ANKETO_OUTPUT_Init( &p_wk->output, p_msg, p_drawsys, p_param, heapID );
}

static BOOL ANKETO_LOCAL_OUTPUT_Main( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_TALKWIN* p_talkwin, ANKETO_PARAM* p_param, u32 heapID )
{
	return ANKETO_OUTPUT_Main( &p_wk->output, p_msg, p_drawsys, p_talkwin, p_param, heapID );
}

static void ANKETO_LOCAL_OUTPUT_Exit( ANKETO_LOCAL_WK* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys )
{
	ANKETO_OUTPUT_Exit( &p_wk->output, p_msg, p_drawsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�͉��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	p_param		�p�����[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_OUTPUT_Init( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_PARAM* p_param, u32 heapID )
{
	// �w�i�ݒ�
	{
		ArcUtil_HDL_PalSet( p_drawsys->p_handle, NARC_wifi_lobby_other_bg_NCLR, PALTYPE_MAIN_BG, ANKETO_PLTT_MAIN_BACK00, ANKETO_PLTT_MAINBACK_PLTTNUM*32, heapID );
		ArcUtil_HDL_BgCharSet( p_drawsys->p_handle, NARC_wifi_lobby_other_bg_NCGR, p_drawsys->p_bgl, GF_BGL_FRAME0_M, 0, 0, FALSE, heapID );
		ArcUtil_HDL_ScrnSet( p_drawsys->p_handle, NARC_wifi_lobby_other_bg01_NSCR, p_drawsys->p_bgl, GF_BGL_FRAME0_M, 0, 0, FALSE, heapID );
	}

	// �E�B���h�E�쐬
	{
		int i;

		for( i=0; i<ANKETO_OUTPUT_BMP_NUM; i++ ){
			GF_BGL_BmpWinAddEx( p_drawsys->p_bgl, &p_wk->win[i], 
					&sc_ANKETO_OUTPUT_BMPDAT[i] );
		}
	}

	// �^�C�g��
	ANKETO_OUTPUT_DrawTitle( p_wk, p_msg, msg_survey_monitor_a_04_header, ANKETO_PLTT_TITLEFONT_COL_OUTPUT00 );

	// bar�p�̃L�����N�^�ǂݍ���
	{
		p_wk->p_charres = ArcUtil_HDL_CharDataGet( p_drawsys->p_handle, NARC_wifi_lobby_other_bg_NCGR, FALSE, &p_wk->p_char, heapID );
	}

	// �A���P�[�g���e�ƌ��ʂ���M
	{
		ANKETO_QUESTION_DATA_Init( &p_wk->now, TRUE, heapID );
		ANKETO_QUESTION_DATA_Init( &p_wk->last, FALSE, heapID );

		// ��{��񂩂�A���P�[�g���ʂ��W�v
		// �����W�v���Ă�Ȃ�A���̃f�[�^���g�p����
		if( WFLBY_SYSTEM_GetAnketoResult( p_param->p_system, &p_wk->result_now ) == FALSE ){
			ANKETO_QUESTION_RESULT_CalcHirobaResult( &p_wk->result_now, p_param->p_system );
			WFLBY_SYSTEM_SetAnketoResult( p_param->p_system, &p_wk->result_now );	// �W�v�f�[�^��ݒ�
		}

		ANKETO_QUESTION_RESULT_Init( &p_wk->result_last );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�͉��	���C������
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	p_talkwin	��b�E�B���h�E
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL ANKETO_OUTPUT_Main( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, ANKETO_TALKWIN* p_talkwin, ANKETO_PARAM* p_param, u32 heapID )
{
	STRBUF* p_str;
	u32 msg_idx;
	
	switch( p_wk->seq ){
#if 0
	case ANKETO_OUTPUT_SEQ_ANKETO_RECV:			// �A���P�[�g��M
		// ��M�֘A�̃��b�Z�[�W�\��
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_02_01 );
		ANKETO_TalkWin_PrintAll( p_talkwin, p_str );


		// �^�C���E�G�C�g�\��
		ANKETO_TalkWin_StartTimeWait( p_talkwin );

		Snd_SePlay( ANKETO_SND_RECV_WAIT );

		p_wk->seq ++;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_RECVWAIT:		// �A���P�[�g��M��

		// ��M����
		if( (gf_mtRand() % 100) == 0 ){

			Snd_SePlay( ANKETO_SND_RECV );

			// �A���P�[�g���e�ƌ��ʂ���M
			{
				ANKETO_QUESTION_DATA_DebugInit( &p_wk->now );
				ANKETO_QUESTION_DATA_DebugInit( &p_wk->last );

				// ��{��񂩂�A���P�[�g���ʂ��W�v
				// �����W�v���Ă�Ȃ�A���̃f�[�^���g�p����
				if( WFLBY_SYSTEM_GetAnketoResult( p_param->p_system, &p_wk->result_now ) == FALSE ){
					ANKETO_QUESTION_RESULT_CalcHirobaResult( &p_wk->result_now, p_param->p_system, &p_wk->now );
					WFLBY_SYSTEM_SetAnketoResult( p_param->p_system, &p_wk->result_now );	// �W�v�f�[�^��ݒ�
				}
		
				ANKETO_QUESTION_RESULT_DebugInit( &p_wk->result_last );
			}
			
			ANKETO_TalkWin_StopTimeWait( p_talkwin );


			p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_02_02 );
			ANKETO_TalkWin_Print( p_talkwin, p_str );

			p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE00;
			p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		}
		break;
#endif

	case ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE00:	// ���񂩂���
		if( p_wk->now.special == FALSE ){
			msg_idx = msg_survey_monitor_a_n03_01;
		}else{
			msg_idx = msg_survey_monitor_a_s03_01;
		}
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_idx );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE01;
		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE01:	// ���e
		p_str = ANKETO_QUESTION_DATA_GetQuestionStr( &p_wk->now, p_msg );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE02;
		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE02:	// �ł���
		if( sys.trg & PAD_BUTTON_DECIDE ){

			p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_n03_02 );
			ANKETO_TalkWin_Print( p_talkwin, p_str );

			p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE03;
			p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		}
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_NQ_WRITE03:	// ���񂩂��͂���Ȃł���
		if( p_wk->now.special == FALSE ){
			msg_idx = msg_survey_monitor_a_n04;
		}else{
			msg_idx = msg_survey_monitor_a_s04;
		}
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_idx );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_NRESULT;
		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_NRESULT:		// ���ʕ\��
		{
			WFLBY_ANKETO anketo_data;
			const WFLBY_USER_PROFILE* cp_profile;
			BOOL vip;

			cp_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_param->p_system );
			WFLBY_SYSTEM_GetProfileAnketoData( cp_profile, &anketo_data );

			vip = WFLBY_SYSTEM_GetUserVipFlag( p_param->p_system, WFLBY_SYSTEM_GetMyIdx( p_param->p_system ) );
		
			// �񓚓��e��\������
			ANKETO_OUTPUT_DrawAnswer( p_wk, &p_wk->now, p_msg, p_drawsys, 
					&anketo_data, heapID, ANKETO_PLTT_FONT_COL_OUTPUT00, ANKETO_PLTT_FONT_COL_VIP00, vip );

			// �o�[�\���J�n
			ANKETO_OUTPUT_DrawBarStart( p_wk, &p_wk->result_now, p_drawsys, ANKETO_PLTT_MAIN_BACK01 );

			Snd_SePlay( ANKETO_SND_BAR );
		}

		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_NRESULT_MAIN;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_NRESULT_MAIN:	// ���ʕ\��
		{
			BOOL result;
			result = ANKETO_OUTPUT_DrawBarMain( p_wk, p_drawsys );
			if( result == TRUE ){
				// SE���Ƃ߂�
				//Snd_SeStopBySeqNo( ANKETO_SND_BAR, 0 );
				
				p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_NA_WAIT;
			}
		}
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_NA_WAIT:		// A�{�^���҂�

		if( sys.trg & PAD_BUTTON_DECIDE ){

			// ��T�̌��ʂ͂���H
			if( p_wk->last.question_no != DWC_LOBBY_ANKETO_NOT_QUESTION ){
				p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_YESNO_DRAW;
			}else{
				// �I�����b�Z�[�W��I���
				p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_06_00 );
				ANKETO_TalkWin_Print( p_talkwin, p_str );

				p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_ENDMSG;
				p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
			}
		}
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_YESNO_DRAW:	// YESNO�\��
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_05 );
		ANKETO_TalkWin_PrintAll( p_talkwin, p_str );
		ANKETO_TalkWin_StartYesNo( p_talkwin, p_drawsys, heapID );

		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_YESNO_WAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_YESNO_WAIT:	// YESNO�҂�
		{
			u32 result;
			result = ANKETO_TalkWin_MainYesNo( p_talkwin, heapID );
			switch( result ){
			// �͂�
			case 0:
				// ��T�\����
				p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEOUT;
				break;

			// �́[
			case BMPMENU_CANCEL:
				// �I����
				p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_ENDMSG;
				break;
			}
		}
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEOUT:	// ���t�F�[�h
		WIPE_SYS_Start(WIPE_PATTERN_WMS, 
				WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, heapID );

		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEOUTWAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEOUTWAIT:// ���t�F�[�h
		if( WIPE_SYS_EndCheck() == TRUE ){
			// ��ʂ�؂�ւ���
			ANKETO_OUTPUT_SetLastWeekGraphic( p_wk, p_msg, p_drawsys, heapID );

			p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEIN;
		}
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEIN:		// ���t�F�[�h
		WIPE_SYS_Start(WIPE_PATTERN_WMS, 
				WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, heapID );

		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEINWAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LQ_WIPEINWAIT:	// ���t�F�[�h
		if( WIPE_SYS_EndCheck() == TRUE ){
			p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE00;
		}
		break;


	case ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE00:	// �O���
		if( p_wk->last.special == FALSE ){
			msg_idx = msg_survey_monitor_a_n06_01;
		}else{
			msg_idx = msg_survey_monitor_a_s06_01;
		}
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_idx );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE01;
		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE01:	// ���e
		p_str = ANKETO_QUESTION_DATA_GetQuestionStr( &p_wk->last, p_msg );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE02;
		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE02:	// �ł���
		if( sys.trg & PAD_BUTTON_DECIDE ){

			p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_n06_02 );
			ANKETO_TalkWin_Print( p_talkwin, p_str );

			p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE03;
			p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		}
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LQ_WRITE03:	// �O��͂���Ȃł���
		if( p_wk->last.special == FALSE ){
			msg_idx = msg_survey_monitor_a_n07;
		}else{
			msg_idx = msg_survey_monitor_a_s07;
		}
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_idx );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_LRESULT;
		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LRESULT:		// ���ʕ\��
		{
			WFLBY_ANKETO anketo;
			WFLBY_SAVEDATA* p_hirobasave;
			BOOL vip;

			// �Z�[�u�f�[�^����P�O�ɓ������A���P�[�g�f�[�^�擾
			p_hirobasave		= SAVEDATA_WFLBY_GetWork( p_param->p_save );
			anketo.anketo_no	= SAVEDATA_WFLBY_GetQuestionNumber( p_hirobasave );
			anketo.select		= SAVEDATA_WFLBY_GetMyAnswer( p_hirobasave );

			vip = WFLBY_SYSTEM_GetUserVipFlag( p_param->p_system, WFLBY_SYSTEM_GetMyIdx( p_param->p_system ) );
		
			// �񓚓��e��\������
			ANKETO_OUTPUT_DrawAnswer( p_wk, &p_wk->last, p_msg, p_drawsys, &anketo, heapID, 
					ANKETO_PLTT_FONT_COL_OUTPUT01, ANKETO_PLTT_FONT_COL_VIP01, vip );

			// �o�[�\���J�n
			ANKETO_OUTPUT_DrawBarStart( p_wk, &p_wk->result_last, p_drawsys, ANKETO_PLTT_MAIN_BACK02 );

			Snd_SePlay( ANKETO_SND_BAR );
		}
		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_LRESULT_MAIN;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LRESULT_MAIN:	// ���ʕ\��
		{
			BOOL result;
			result = ANKETO_OUTPUT_DrawBarMain( p_wk, p_drawsys );
			if( result == TRUE ){

				// SE���Ƃ߂�
				//Snd_SeStopBySeqNo( ANKETO_SND_BAR, 0 );
				p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_LA_WAIT;
			}
		}
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_LA_WAIT:		// A�{�^���҂�
		if( sys.trg & PAD_BUTTON_DECIDE ){
			// �I����
			p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_ENDMSG;
		}
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_ENDMSG:		// �I�����b�Z�[�W
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_08 );
		ANKETO_TalkWin_Print( p_talkwin, p_str );

		p_wk->ret_seq = ANKETO_OUTPUT_SEQ_ANKETO_END;
		p_wk->seq = ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT;
		break;

	case ANKETO_OUTPUT_SEQ_ANKETO_END:			// �I��
		// ��b�E�B���h�E��\��
		ANKETO_TalkWin_Off( p_talkwin );
		return TRUE;	

	case ANKETO_OUTPUT_SEQ_ANKETO_MSGWAIT:		// ���b�Z�[�W�҂�
		if( ANKETO_TalkWin_EndWait( p_talkwin ) == TRUE ){
			p_wk->seq = p_wk->ret_seq;
		}
		break;

	default:
		GF_ASSERT(0);
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�͉��	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W���[�N
 *	@param	p_drawsys	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void ANKETO_OUTPUT_Exit( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys )
{
	// �f�[�^�j��
	{
		ANKETO_QUESTION_DATA_Exit( &p_wk->now );
		ANKETO_QUESTION_DATA_Exit( &p_wk->last );
	}
	
	// �L�����N�^�f�[�^�j��
	sys_FreeMemoryEz( p_wk->p_charres );
	p_wk->p_charres = NULL;
	
	// �E�B���h�E�j��
	{
		int i;

		for( i=0; i<ANKETO_OUTPUT_BMP_NUM; i++ ){
			GF_BGL_BmpWinDel( &p_wk->win[i] );
		}
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���̕`��
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W
 *	@param	msgidx		���b�Z�[�WIDX
 */
//-----------------------------------------------------------------------------
static void ANKETO_OUTPUT_DrawTitle( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, u32 msgidx, GF_PRINTCOLOR col )
{
	STRBUF* p_str;
	u32 width;
	s32 x;
	

	GF_BGL_BmpWinDataFill( &p_wk->win[ANKETO_OUTPUT_BMP_TITLE], 0 );

	p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msgidx );

	// �Z���^�����O
	width = FontProc_GetPrintStrWidth( FONT_TALK, p_str, 0 );
	x = ((ANKETO_OUTPUTWIN_TITLE_SIZX*8) - width) / 2;
	GF_STR_PrintColor( &p_wk->win[ANKETO_OUTPUT_BMP_TITLE], 
			FONT_TALK, p_str, x, 4,
			MSG_NO_PUT, col, NULL );

	GF_BGL_BmpWinOnVReq( &p_wk->win[ANKETO_OUTPUT_BMP_TITLE] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�񓚂�\������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 *	@param	p_msg		���b�Z�[�W
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	cp_answer	�����̓���
 *	@param	heapID
 *	@param	col			�\���J���[
 *	@param	vip			VIP
 */
//-----------------------------------------------------------------------------
static void ANKETO_OUTPUT_DrawAnswer( ANKETO_OUTPUT* p_wk, const ANKETO_QUESTION_DATA* cp_data, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, const WFLBY_ANKETO* cp_answer, u32 heapID, GF_PRINTCOLOR col, GF_PRINTCOLOR vipcol, BOOL vip )
{
	STRBUF* p_str;

	GF_BGL_BmpWinDataFill( &p_wk->win[ANKETO_OUTPUT_BMP_ANS], 0 );

	GF_BGL_BmpWinDataFill( &p_wk->win[ANKETO_OUTPUT_BMP_ANS_MY], 0 );

	p_str = ANKETO_QUESTION_DATA_GetAnswerStr( cp_data, p_msg, ANKETO_ANSWER_A );
	GF_STR_PrintColor( &p_wk->win[ANKETO_OUTPUT_BMP_ANS], 
			FONT_TALK, p_str, 0, ANKETO_OUTPUTWIN_ANS00_Y,
			MSG_NO_PUT, col, NULL );

	p_str = ANKETO_QUESTION_DATA_GetAnswerStr( cp_data, p_msg, ANKETO_ANSWER_B );
	GF_STR_PrintColor( &p_wk->win[ANKETO_OUTPUT_BMP_ANS], 
			FONT_TALK, p_str, 0, ANKETO_OUTPUTWIN_ANS01_Y,
			MSG_NO_PUT, col, NULL );

	p_str = ANKETO_QUESTION_DATA_GetAnswerStr( cp_data, p_msg, ANKETO_ANSWER_C );
	GF_STR_PrintColor( &p_wk->win[ANKETO_OUTPUT_BMP_ANS], 
			FONT_TALK, p_str, 0, ANKETO_OUTPUTWIN_ANS02_Y,
			MSG_NO_PUT, col, NULL );

	// �����̓���
	if( (cp_answer->anketo_no == cp_data->anketo_no) && (cp_answer->select < ANKETO_ANSWER_NUM) ){	// �ʂ��ԍ�����v���邩�`�F�b�N
		s32 str_width;
		s32 write_x;
		GF_PRINTCOLOR mycol;

		if( vip == TRUE ){
			mycol = vipcol;	
		}else{
			mycol = col;	
		}
		
		ANKETO_MsgManSetMyName( p_msg, 0, heapID );
		p_str = ANKETO_MsgManGetStr( p_msg, ANKETO_MSGMAN_MSG, msg_survey_monitor_a_04_pselect );
		str_width	= FontProc_GetPrintStrWidth( FONT_TALK, p_str, 0 );
		write_x		= ((ANKETO_OUTPUTWIN_MY_NAME_WRITE_SIZE_X - str_width) / 2);
		
		GF_STR_PrintColor( &p_wk->win[ANKETO_OUTPUT_BMP_ANS_MY], 
				FONT_TALK, p_str, write_x, 0,
				MSG_NO_PUT, mycol, NULL );

		// �����̕��������������
		p_str = ANKETO_QUESTION_DATA_GetAnswerStr( cp_data, p_msg, cp_answer->select );
		str_width	= FontProc_GetPrintStrWidth( FONT_TALK, p_str, 0 );
		write_x		= ((ANKETO_OUTPUTWIN_MY_ANSWER_WRITE_SIZE_X - str_width) / 2);
		
		GF_STR_PrintColor( &p_wk->win[ANKETO_OUTPUT_BMP_ANS_MY], 
				FONT_TALK, p_str, ANKETO_OUTPUTWIN_MY_ANSWER_X+write_x, 0,
				MSG_NO_PUT, col, NULL );
	}
	

	GF_BGL_BmpWinOnVReq( &p_wk->win[ANKETO_OUTPUT_BMP_ANS_MY] );
	GF_BGL_BmpWinOnVReq( &p_wk->win[ANKETO_OUTPUT_BMP_ANS] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�[�\���J�n
 *
 *	@param	p_wk			���[�N
 *	@param	cp_data			�f�[�^
 *	@param	p_drawsys		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void ANKETO_OUTPUT_DrawBarStart( ANKETO_OUTPUT* p_wk, const ANKETO_QUESTION_RESULT* cp_data, ANKETO_DRAWSYS* p_drawsys, u32 palno )
{
	int i;
	
	p_wk->count = 0;
	for( i=0; i<ANKETO_ANSWER_NUM; i++ ){
		p_wk->draw_width[ i ] = 0;
		p_wk->drawend_width[ i ] = ANKETO_QUESTION_RESULT_GetBerWidth( cp_data, i, ANKETO_BAR_100WIDTH ); 
		GF_BGL_BmpWinSet_Pal( &p_wk->win[ ANKETO_OUTPUT_BMP_ANS_BAR00+i ], palno );

		GF_BGL_BmpWinDataFill( &p_wk->win[ ANKETO_OUTPUT_BMP_ANS_BAR00+i ], 0 );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�[�\�����C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *
 *	@retval	TRUE	�o�[�\���I��
 *	@retval	FLASE	�o�[�\����
 */
//-----------------------------------------------------------------------------
static BOOL ANKETO_OUTPUT_DrawBarMain( ANKETO_OUTPUT* p_wk, ANKETO_DRAWSYS* p_drawsys )
{
	s32 draw_width;
	s32 draw_start;
	s32 width;
	int i, j;
	int skip_count;

	// �Ō�܂ŏ����Ă���I����
	if( p_wk->count >= ANKETO_BAR_COUNTMAX ){
		return TRUE;
	}
	
	// �J�E���g�̕\�����������߂�
	p_wk->count ++;
	draw_width = (p_wk->count * ANKETO_BAR_100WIDTH) / ANKETO_BAR_COUNTMAX;
	
	// �`�悵�Ȃ��Ƃ����Ȃ������܂�
	skip_count = 0;
	for( i=0; i<ANKETO_ANSWER_NUM; i++ ){

		// �I����Ă邩�`�F�b�N
		if( p_wk->draw_width[i] == p_wk->drawend_width[i] ){
			skip_count ++;
			continue;
		}

		// ���������̒������傫�������疳�����ɐL�΂�
		if( p_wk->drawend_width[ i ] >= draw_width ){
			// ���̂܂ܕ`��
			width		= draw_width - p_wk->draw_width[ i ];
			draw_start	= p_wk->draw_width[ i ];
			p_wk->draw_width[ i ] = draw_width;

		// ���Ƃ́A�c�蕔����`�悷��
		}else{
			width		= p_wk->drawend_width[ i ] - p_wk->draw_width[ i ];
			draw_start	= p_wk->draw_width[ i ];
			p_wk->draw_width[ i ] = p_wk->drawend_width[ i ];
		}

		// �`��
		for( j=0; j<width; j++ ){
			GF_BGL_BmpWinPrint( &p_wk->win[ ANKETO_OUTPUT_BMP_ANS_BAR00+i ], p_wk->p_char->pRawData,
					ANKETO_BAR_CG_X, ANKETO_BAR_CG_Y, 
					ANKETO_BAR_CGFILE_X, ANKETO_BAR_CGFILE_Y,
					ANKETO_BAR_WRITE_X+draw_start+j, 0,
					ANKETO_BAR_CG_SIZX, ANKETO_BAR_CG_SIZY );
		}
		GF_BGL_BmpWinOnVReq( &p_wk->win[ ANKETO_OUTPUT_BMP_ANS_BAR00+i ] );
	}

	// �S���I����Ă���I���
	if( skip_count == ANKETO_ANSWER_NUM ){
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��T�̃A���P�[�g���ʕ\���O���t�B�b�N�̕\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_OUTPUT_SetLastWeekGraphic( ANKETO_OUTPUT* p_wk, ANKETO_MSGMAN* p_msg, ANKETO_DRAWSYS* p_drawsys, u32 heapID )
{
	// �w�i�ݒ�
	{
		ArcUtil_HDL_ScrnSet( p_drawsys->p_handle, NARC_wifi_lobby_other_bg02_NSCR, p_drawsys->p_bgl, GF_BGL_FRAME0_M, 0, 0, FALSE, heapID );
	}

	// �E�B���h�E�쐬
	{
		int i;

		for( i=0; i<ANKETO_OUTPUT_BMP_NUM; i++ ){
			GF_BGL_BmpWinDataFill( &p_wk->win[i], 0 );
			GF_BGL_BmpWinOn( &p_wk->win[i] );
		}
	}

	// �^�C�g��
	ANKETO_OUTPUT_DrawTitle( p_wk, p_msg, msg_survey_monitor_a_07_header, ANKETO_PLTT_TITLEFONT_COL_OUTPUT01 );
}




//----------------------------------------------------------------------------
/**
 *	@brief	������e���[�N	�f�o�b�N������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_QUESTION_DATA_DebugInit( ANKETO_QUESTION_DATA* p_wk )
{
	p_wk->special			= gf_mtRand() % 2;
	p_wk->question_no		= gf_mtRand() % (ANKETO_NORMAL_Q_NUM+ANKETO_SPECIAL_Q_NUM);
	p_wk->anketo_no			= 0;
	p_wk->make_question		= 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	������e���[�N	������
 *
 *	@param	p_wk	���[�N
 *	@param	now		���̎��₩
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void ANKETO_QUESTION_DATA_Init( ANKETO_QUESTION_DATA* p_wk, BOOL now, u32 heapID )
{
	u32 data_start;
	u32 msg_start;
	u16* p_str;
	int i;
	
	// ���b�Z�[�W�o�b�t�@�쐬
	p_wk->p_question = STRBUF_Create( ANKETO_MESSAGE_BUFFNUM, heapID );
	for( i=0; i<ANKETO_ANSWER_NUM; i++ ){
		p_wk->p_ans[i] = STRBUF_Create( ANKETO_MESSAGE_BUFFNUM, heapID );
	}
	
	if( now == TRUE ){
		data_start	= DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_SERIAL;
		msg_start	= DWC_LOBBY_ANKETO_MESSAGE_NOW_QUESTION;
	}else{
		data_start	= DWC_LOBBY_ANKETO_DATA_LAST_QUESTION_SERIAL;
		msg_start	= DWC_LOBBY_ANKETO_MESSAGE_LAST_QUESTION;
	}

	p_wk->special		= DWC_LOBBY_ANKETO_GetData( data_start+DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_SPECIAL );
	p_wk->question_no	= DWC_LOBBY_ANKETO_GetData( data_start+DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_NO );
	p_wk->anketo_no		= DWC_LOBBY_ANKETO_GetData( data_start+DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_SERIAL );
	p_wk->make_question	= DWC_LOBBY_ANKETO_GetData( data_start+DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_NINI );

	if( p_wk->make_question == TRUE ){
		// ����Ɖ񓚈Ă��擾
		p_str = DWC_LOBBY_ANKETO_GetMessage( msg_start+DWC_LOBBY_ANKETO_MESSAGE_NOW_QUESTION );
		GF_ASSERT( p_str );
		STRBUF_SetStringCode( p_wk->p_question, p_str );
		for( i=0; i<ANKETO_ANSWER_NUM; i++ ){
			p_str = DWC_LOBBY_ANKETO_GetMessage( msg_start+DWC_LOBBY_ANKETO_MESSAGE_NOW_ANSWER_00+i );
			GF_ASSERT( p_str );
			STRBUF_SetStringCode( p_wk->p_ans[i], p_str );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�f�[�^�̔j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ANKETO_QUESTION_DATA_Exit( ANKETO_QUESTION_DATA* p_wk )
{
	int i;
	
	// ���b�Z�[�W�o�b�t�@�j��
	STRBUF_Delete( p_wk->p_question );
	for( i=0; i<ANKETO_ANSWER_NUM; i++ ){
		STRBUF_Delete( p_wk->p_ans[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	������e���[�N�@���⃁�b�Z�[�W�擾
 *
 *	@param	p_wk	���⃏�[�N
 *	@param	p_msg	���b�Z�[�W���[�N
 */
//-----------------------------------------------------------------------------
static STRBUF* ANKETO_QUESTION_DATA_GetQuestionStr( const ANKETO_QUESTION_DATA* cp_wk, ANKETO_MSGMAN* p_msg )
{
	if( cp_wk->make_question == FALSE ){
		return ANKETO_MsgManGetQuestionStr( p_msg, cp_wk->question_no );
	}
	return cp_wk->p_question;
}

//----------------------------------------------------------------------------
/**
 *	@brief	������e���[�N	�񓚃��b�Z�[�W�擾
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W���[�N
 *	@param	answer		�񓚃^�C�v�iANKETO_ANSWER_A�@�E�E�E�j
 */
//-----------------------------------------------------------------------------
static STRBUF* ANKETO_QUESTION_DATA_GetAnswerStr( const ANKETO_QUESTION_DATA* cp_wk, ANKETO_MSGMAN* p_msg, u32 answer )
{
	if( cp_wk->make_question == FALSE ){
		return ANKETO_MsgManGetAnswerStr( p_msg, cp_wk->question_no, answer );
	}
	return cp_wk->p_ans[ answer ];
}  

//----------------------------------------------------------------------------
/**
 *	@brief	���ʃ��[�N	�f�o�b�N������
 */
//-----------------------------------------------------------------------------
static void ANKETO_QUESTION_RESULT_DebugInit( ANKETO_QUESTION_RESULT* p_wk )
{
	int i;
	
	p_wk->ans_all = 0;
	for( i=0; i<ANKETO_ANSWER_NUM; i++ ){
		p_wk->ans[i] = gf_mtRand() % 0xffffffff;
		p_wk->ans_all += p_wk->ans[i];
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O��̌��ʂ����߂�
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
static void ANKETO_QUESTION_RESULT_Init( ANKETO_QUESTION_RESULT* p_wk )
{
	int i;

	p_wk->ans_all = 0;
	for( i=0; i<ANKETO_ANSWER_NUM; i++ ){
		p_wk->ans[i] = DWC_LOBBY_ANKETO_GetData( DWC_LOBBY_ANKETO_DATA_LAST_RESULT_A+i );
		p_wk->ans_all += p_wk->ans[i];
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�[�̒��������߂�
 *
 *	@param	cp_wk		���[�N
 *	@param	answer		���ʃi���o�[
 *	@param	max_width	�ő�̒���
 *
 *	@return	����
 */
//-----------------------------------------------------------------------------
static u32 ANKETO_QUESTION_RESULT_GetBerWidth( const ANKETO_QUESTION_RESULT* cp_wk, u32 answer, u32 max_width )
{
	u64 max_width_64, answer_64;
	GF_ASSERT( answer < ANKETO_ANSWER_NUM );

	max_width_64	= max_width;
	answer_64		= cp_wk->ans[ answer ];
	
	return (u32)(((u64)(max_width_64 * answer_64)) / cp_wk->ans_all);
}

//----------------------------------------------------------------------------
/**
 *	@brief	������Ђ�Γ��̌��ʂ��W�v
 *
 *	@param	p_wk		���[�N
 *	@param	cp_system	�V�X�e��
 */
//-----------------------------------------------------------------------------
static void ANKETO_QUESTION_RESULT_CalcHirobaResult( ANKETO_QUESTION_RESULT* p_wk, const WFLBY_SYSTEM* cp_system )
{
	const WFLBY_USER_PROFILE* cp_profile;
	WFLBY_ANKETO anketo;
	int i;
	u32 lang;
	BOOL sum;

	memset( p_wk, 0, sizeof(ANKETO_QUESTION_RESULT) );

	for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
		cp_profile = WFLBY_SYSTEM_GetUserProfile( cp_system, i );
		if( cp_profile != NULL ){
			lang = WFLBY_SYSTEM_GetProfileRagionCodeOrg( cp_profile );
			WFLBY_SYSTEM_GetProfileAnketoData( cp_profile, &anketo );
			sum = DWC_LOBBY_ANKETO_GetLanguageSummarize( DWC_LOBBY_ANKETO_LANGUAGE_NOW, lang );
			if( (sum == TRUE) && (anketo.select < ANKETO_ANSWER_NUM) ){
				p_wk->ans[ anketo.select ] ++;
				p_wk->ans_all ++;
			}
		}
	}
}



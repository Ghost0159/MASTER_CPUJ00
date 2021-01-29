//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_connect.c
 *	@brief		Wi-Fi���r�[�ɐڑ�	�ؒf�����v���b�N
 *	@author		tomoya takahashi
 *	@data		2007.12.12
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "gflib/msg_print.h"

#include "system/wordset.h"
#include "system/msgdata.h"
#include "system/arc_tool.dat"
#include "system/wipe.h"
#include "system/bmp_menu.h"
#include "system/window.h"
#include "system/fontproc.h"
#include "system/pm_rtc.h"

#include "communication/comm_state.h"

#include "savedata/config.h"
#include "savedata/record.h"


#include "msgdata/msg.naix"
#include "msgdata/msg_wifi_lobby.h"
#include "msgdata/msg_wifi_hiroba.h"
#include "msgdata/msg_wifi_system.h"
#include "msgdata/msg_debug_tomoya.h"

#include "communication/wm_icon.h"

#include "wflby_connect.h"
#include "wflby_def.h"
#include "wflby_snd.h"

#include "application/connect_anm.h"

// �_�~�[�O���t�B�b�N�ł�
#include "application/wifi_p2pmatch/wifip2pmatch.naix"

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
#define WFLBY_CONNECT_DEBUG_START		// �����̎�ނȂǂ�I�����Ă��炤
#endif

#ifdef WFLBY_CONNECT_DEBUG_START
static s32 DEBUG_SEL_ROOM;		// �I�����������Ȃ�Ȃ�Ȃ�
static s32 DEBUG_SEL_SEASON;
static s32 DEBUG_SEL_ITEM;
extern BOOL D_Tomoya_WiFiLobby_DebugStart;	// �f�o�b�N�J�n���
#endif



//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	���C���V�[�P���X
//=====================================
enum {
	WFLBY_CONNECT_SEQ_FADEIN,
	WFLBY_CONNECT_SEQ_FADEWAIT,
	WFLBY_CONNECT_SEQ_LOGIN_CHECK,
	WFLBY_CONNECT_SEQ_LOGIN_CHECK_WAIT,
	
#ifdef WFLBY_CONNECT_DEBUG_START
	WFLBY_CONNECT_SEQ_DEBUG_SEL_ROOM,
	WFLBY_CONNECT_SEQ_DEBUG_SEL_ROOMWAIT,
	WFLBY_CONNECT_SEQ_DEBUG_SEL_SEASON,
	WFLBY_CONNECT_SEQ_DEBUG_SEL_SEASONWAIT,
	WFLBY_CONNECT_SEQ_DEBUG_SEL_ITEM,
	WFLBY_CONNECT_SEQ_DEBUG_SEL_ITEMWAIT,
#endif

	WFLBY_CONNECT_SEQ_LOGIN,
	WFLBY_CONNECT_SEQ_LOGIN_WAIT_DWC,
	WFLBY_CONNECT_SEQ_LOGIN_WAIT,
	WFLBY_CONNECT_SEQ_FADEOUT,
	WFLBY_CONNECT_SEQ_FADEOUTWAIT,
	WFLBY_CONNECT_SEQ_ERRON,
	WFLBY_CONNECT_SEQ_ERR,
	WFLBY_CONNECT_SEQ_RETRY,
	WFLBY_CONNECT_SEQ_RETRYWAIT,
	WFLBY_CONNECT_SEQ_RETRYLOGOUTWAIT,
	WFLBY_CONNECT_SEQ_LOGOUT,
	WFLBY_CONNECT_SEQ_LOGOUTWAIT,
} ;



//-------------------------------------
///	�ގ��V�[�P���X
//=====================================
enum {
	WFLBY_DISCONNECT_SEQ_FADEIN,
	WFLBY_DISCONNECT_SEQ_FADEWAIT,
#if 0	// �^�C���A�E�g���b�Z�[�W�͍L����ŏo�����ƂɂȂ���
	WFLBY_DISCONNECT_SEQ_TIMEOUT_MSG,
	WFLBY_DISCONNECT_SEQ_TIMEOUT_MSG_WAIT,
#endif
	WFLBY_DISCONNECT_SEQ_LOGOUT_MSG,
	WFLBY_DISCONNECT_SEQ_WLDSEND,
	WFLBY_DISCONNECT_SEQ_WLDWAIT,
	WFLBY_DISCONNECT_SEQ_LOGOUT,
	WFLBY_DISCONNECT_SEQ_LOGOUT_WAIT,
	WFLBY_DISCONNECT_SEQ_LOGOUT_END,
	WFLBY_DISCONNECT_SEQ_LOGOUT_END_MSGWAIT,
	WFLBY_DISCONNECT_SEQ_FADEOUT,
	WFLBY_DISCONNECT_SEQ_FADEOUTWAIT,
} ;
#define WFLBY_DISCONNECT_WLDSENDWAIT_TIMEOUT	( 30*30 )	// ���E�f�[�^���M�����`�F�b�N�^�C���A�E�g
#define WFLBY_DISCONNECT_LOGOUT_ENDMSG_WAIT		( 3*30 )	// ���b�Z�[�W�\���Ԋu





//-------------------------------------
///	�\���V�X�e��
//=====================================
// BG
enum{
	// ���C�����
	WFLBY_BGCNT_MAIN_BACK,
	WFLBY_BGCNT_MAIN_WIN,
	
	// �T�u���
	WFLBY_BGCNT_SUB_BACK,

	WFLBY_BGCNT_NUM,	// �a�f�R���g���[���e�[�u����
};
// ���C��PLTT
enum{
	WFLBY_MAIN_PLTT_BACKSTART,
	WFLBY_MAIN_PLTT_BACKEND = 0xb,
	WFLBY_MAIN_PLTT_TALKFONT = WFLBY_MAIN_PLTT_BACKEND,
	WFLBY_MAIN_PLTT_SYSFONT,
	WFLBY_MAIN_PLTT_TALKWIN,
	WFLBY_MAIN_PLTT_SYSWIN,
	WFLBY_MAIN_PLTT_NUM,
};
// �E�B���h�E�O���t�B�b�N
#define WFLBY_TALKWINGRA_CGX		( 1 )
#define WFLBY_SYSWINGRA_CGX			( WFLBY_TALKWINGRA_CGX+TALK_WIN_CGX_SIZ )
#define WFLBY_SYSWINGRA_CGXEND		( WFLBY_SYSWINGRA_CGX+MENU_WIN_CGX_SIZ )



//-------------------------------------
///	�E�B���h�E�V�X�e��
//=====================================
#define WFLBY_WINSYS_STRBUFNUM	( 256 )




//-------------------------------------
///	��b�E�B���h�E
//=====================================
#define WFLBY_TALKWIN_X			( 2 )
#define WFLBY_TALKWIN_Y			( 19 )
#define WFLBY_TALKWIN_SIZX		( 27 )
#define WFLBY_TALKWIN_SIZY		( 4 )
#define WFLBY_TALKWIN_CGX		( WFLBY_SYSWINGRA_CGXEND )
#define WFLBY_TALKWIN_CGXEND	( WFLBY_TALKWIN_CGX+(WFLBY_TALKWIN_SIZX*WFLBY_TALKWIN_SIZY) )


//-------------------------------------
///	�V�X�e���E�C���h�E
//=====================================
#define WFLBY_SYSTEMWIN_X			( 4 )
#define WFLBY_SYSTEMWIN_Y			( 4 )
#define WFLBY_SYSTEMWIN_SIZX		( 23 )
#define WFLBY_SYSTEMWIN_SIZY		( 16 )
#define WFLBY_SYSTEMWIN_CGX		( WFLBY_TALKWIN_CGXEND )
#define WFLBY_SYSTEMWIN_CGXEND	( WFLBY_SYSTEMWIN_CGX+(WFLBY_SYSTEMWIN_SIZX*WFLBY_SYSTEMWIN_SIZY) )

//-------------------------------------
///	YESNO�E�B���h�E
//=====================================
#define WFLBY_YESNOWIN_X			( 25 )
#define WFLBY_YESNOWIN_Y			( 13 )
#define WFLBY_YESNOWIN_SIZX		( 6 )
#define WFLBY_YESNOWIN_SIZY		( 4 )
#define WFLBY_YESNOWIN_CGX		( WFLBY_SYSTEMWIN_CGXEND )
#define WFLBY_YESNOWIN_CGXEND	( WFLBY_YESNOWIN_CGX+(WFLBY_YESNOWIN_SIZX*WFLBY_YESNOWIN_SIZY) )

//-------------------------------------
///	�^�C�g��
//=====================================
#define WFLBY_TITLEWIN_X		( 5 )
#define WFLBY_TITLEWIN_Y		( 1 )
#define WFLBY_TITLEWIN_SIZX		( 22 )
#define WFLBY_TITLEWIN_SIZY		( 2 )
#define WFLBY_TITLEWIN_CGX		( WFLBY_YESNOWIN_CGXEND )
#define WFLBY_TITLEWIN_CGXEND	( WFLBY_TITLEWIN_CGX+(WFLBY_TITLEWIN_SIZX*WFLBY_TITLEWIN_SIZY) )
#define WFLBY_TITLEWIN_COL		( GF_PRINTCOLOR_MAKE( 15, 14, 0 ) )






//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�E�B���h�E�V�X�e��
//=====================================
typedef struct {
	WORDSET*			p_wordset;	// ���[�h�Z�b�g
	MSGDATA_MANAGER*	p_msgman;	// ���[�h�Z�b�g
	GF_BGL_BMPWIN		win;		// ��b�E�B���h�E
	STRBUF*				p_str;		// ������o�b�t�@
	STRBUF*				p_tmp;		// ������o�b�t�@
	u32					fontid;		// ���b�Z�[�W�̃t�H���gID	
	void*				p_timewait;	// �^�C���E�G�C�g
	u32					msgspeed;
	u32					msgno;

#ifdef WFLBY_CONNECT_DEBUG_START
	MSGDATA_MANAGER*	p_debug_msgman;	// ���[�h�Z�b�g
#endif
} WFLBY_WINWK;


//-------------------------------------
///	���O���p���[�N
//=====================================
typedef struct {
	SAVEDATA*			p_save;		// �Z�[�u�f�[�^
	u32					seq;		// �V�[�P���X
	u32					wait;		// �ėp�E�G�C�g
	GF_BGL_INI*			p_bgl;		// bgl�R���g���[��
	WFLBY_WINWK			title;		// �^�C�g���E�B���h�E
	WFLBY_WINWK			talk;		// ��b�E�B���h�E
	WFLBY_WINWK			talk_system;// ��b�E�B���h�E
	WFLBY_WINWK			system;		// �V�X�e���E�B���h�E
	BMPMENU_WORK*		p_yesno;	// yesno�E�B���h�E
	
	CONNECT_BG_PALANM	cbp;		// Wifi�ڑ�BG�p���b�g�A�j������
} WFLBY_CONNECTWK;


//-----------------------------------------------------------------------------
/**
 *					�f�[�^����
 */
//-----------------------------------------------------------------------------

//-------------------------------------
///	�O���t�B�b�N�֘A
//=====================================
// �o���N�ݒ�
static const GF_BGL_DISPVRAM sc_WFLBY_BANK = {
	GX_VRAM_BG_128_A,				// ���C��2D�G���W����BG
	GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
	GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
	GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
	GX_VRAM_OBJ_128_B,				// ���C��2D�G���W����OBJ
	GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
	GX_VRAM_SUB_OBJ_NONE,			// �T�u2D�G���W����OBJ
	GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
	GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
	GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
};

// BG�ݒ�
static const GF_BGL_SYS_HEADER sc_BGINIT = {
	GX_DISPMODE_GRAPHICS,
	GX_BGMODE_0,
	GX_BGMODE_0,
	GX_BG0_AS_2D
};

// BG�R���g���[��
static const u32 sc_WFLBY_BGCNT_FRM[ WFLBY_BGCNT_NUM ] = {
	GF_BGL_FRAME0_M,
	GF_BGL_FRAME1_M,
	GF_BGL_FRAME0_S,
};
static const GF_BGL_BGCNT_HEADER sc_WFLBY_BGCNT_DATA[ WFLBY_BGCNT_NUM ] = {
	// ���C�����
	{	// sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_MAIN_BACK]
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		1, 0, 0, FALSE
	},
	{	// sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_MAIN_WIN]
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},

	// �T�u���
	{	// sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_SUB_BACK]
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},
};
static const BMPWIN_DAT	sc_WFLBY_BMPWIN_DAT_YESNO = {
	GF_BGL_FRAME1_M, WFLBY_YESNOWIN_X, WFLBY_YESNOWIN_Y,
	WFLBY_YESNOWIN_SIZX, WFLBY_YESNOWIN_SIZY, 
	WFLBY_MAIN_PLTT_SYSFONT, WFLBY_YESNOWIN_CGX,
};




//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_VBlank( void* p_work );


static void WFLBY_CONNECT_GraphicInit( WFLBY_CONNECTWK* p_wk, u32 heapID );
static void WFLBY_CONNECT_GraphicExit( WFLBY_CONNECTWK* p_wk );
static void WFLBY_CONNECT_GraphicVBlank( WFLBY_CONNECTWK* p_wk );

static void WFLBY_CONNECT_WIN_Init( WFLBY_WINWK* p_wk, GF_BGL_INI* p_bgl, u32 fontid, u32 msgid, u32 x, u32 y, u32 sizx, u32 sizy, u32 cgx, SAVEDATA* p_save, u32 heapID );
static void WFLBY_CONNECT_WIN_Print( WFLBY_WINWK* p_wk, u32 strid );
static void WFLBY_CONNECT_WIN_Off( WFLBY_WINWK* p_wk );
static void WFLBY_CONNECT_WIN_StartTimeWait( WFLBY_WINWK* p_wk );
static void WFLBY_CONNECT_WIN_EndTimeWait( WFLBY_WINWK* p_wk );
static void WFLBY_CONNECT_WIN_SetErrNumber( WFLBY_WINWK* p_wk, u32 number );
static void WFLBY_CONNECT_WIN_Exit( WFLBY_WINWK* p_wk );
static void WFLBY_CONNECT_WIN_PrintWait( WFLBY_WINWK* p_wk, u32 strid );
static BOOL WFLBY_CONNECT_WIN_PrintEndWait( const WFLBY_WINWK* cp_wk );

static void WFLBY_CONNECT_WIN_PrintTitle( WFLBY_WINWK* p_wk, u32 strid );


#ifdef WFLBY_CONNECT_DEBUG_START
static void WFLBY_CONNECT_WIN_PrintDEBUG( WFLBY_WINWK* p_wk, u32 strid, u32 num );
static void WFLBY_CONNECT_WIN_PrintDEBUG2( WFLBY_WINWK* p_wk, u32 strid, u32 item );
#endif



//----------------------------------------------------------------------------
/**
 *	@brief	���O�C��	�J�n
 *
 *	@param	p_proc		�v���b�N���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBY_CONNECT_Init(PROC* p_proc, int* p_seq)
{
	WFLBY_CONNECTWK* p_wk;
	WFLBY_CONNECT_PARAM* p_param;

	// �q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WFLBY_ROOM, 0x20000 );
	
	// ���[�N�쐬
	p_wk = PROC_AllocWork( p_proc, sizeof(WFLBY_CONNECTWK), HEAPID_WFLBY_ROOM );
	memset( p_wk, 0, sizeof(WFLBY_CONNECTWK) );

	// �p�����[�^�擾
	p_param	= PROC_GetParentWork( p_proc );
	p_wk->p_save		= p_param->p_save;


	// BGM�`�F���W
	Snd_DataSetByScene( SND_SCENE_P2P, SEQ_WIFILOBBY, 0 );	//wifi���r�[�Đ�

	// ���̃v���b�N���Őݒ肷��p�����[�^��������
	p_param->enter			= FALSE;


	// �O���t�B�b�N������
	WFLBY_CONNECT_GraphicInit( p_wk, HEAPID_WFLBY_ROOM );

	// �E�B���h�E�V�X�e��������
	WFLBY_CONNECT_WIN_Init( &p_wk->talk, p_wk->p_bgl, FONT_TALK, NARC_msg_wifi_lobby_dat,
			WFLBY_TALKWIN_X, WFLBY_TALKWIN_Y, 
			WFLBY_TALKWIN_SIZX, WFLBY_TALKWIN_SIZY,
			WFLBY_TALKWIN_CGX, p_wk->p_save, HEAPID_WFLBY_ROOM );
	WFLBY_CONNECT_WIN_Init( &p_wk->talk_system, p_wk->p_bgl, FONT_TALK, NARC_msg_wifi_system_dat,
			WFLBY_TALKWIN_X, WFLBY_TALKWIN_Y, 
			WFLBY_TALKWIN_SIZX, WFLBY_TALKWIN_SIZY,
			WFLBY_TALKWIN_CGX, p_wk->p_save, HEAPID_WFLBY_ROOM );
	WFLBY_CONNECT_WIN_Init( &p_wk->system, p_wk->p_bgl, FONT_SYSTEM, NARC_msg_wifi_system_dat,
			WFLBY_SYSTEMWIN_X, WFLBY_SYSTEMWIN_Y,
			WFLBY_SYSTEMWIN_SIZX, WFLBY_SYSTEMWIN_SIZY,
			WFLBY_SYSTEMWIN_CGX, p_wk->p_save, HEAPID_WFLBY_ROOM );

	WFLBY_CONNECT_WIN_Init( &p_wk->title, p_wk->p_bgl, FONT_TALK, NARC_msg_wifi_lobby_dat,
			WFLBY_TITLEWIN_X, WFLBY_TITLEWIN_Y, 
			WFLBY_TITLEWIN_SIZX, WFLBY_TITLEWIN_SIZY,
			WFLBY_TITLEWIN_CGX, p_wk->p_save, HEAPID_WFLBY_ROOM );


	//  �^�C�g����������
	WFLBY_CONNECT_WIN_PrintTitle( &p_wk->title, msg_wifilobby_018 );


	// VBlank�֐��ݒ�
	sys_VBlankFuncChange( WFLBY_CONNECT_VBlank, p_wk );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	return PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�C�����C��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBY_CONNECT_Main(PROC* p_proc, int* p_seq)
{
	WFLBY_CONNECTWK* p_wk;
	WFLBY_CONNECT_PARAM* p_param;
	
	p_wk	= PROC_GetWork( p_proc );
	p_param	= PROC_GetParentWork( p_proc );


	switch( *p_seq ){
	// �t�F�[�h�C��
	case WFLBY_CONNECT_SEQ_FADEIN:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_OUTCOLOR, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WFLBY_ROOM );
		(*p_seq)++;
		break;

	// �t�F�[�h�C���҂�
	case WFLBY_CONNECT_SEQ_FADEWAIT:
		if( WIPE_SYS_EndCheck() ){
			(*p_seq)++;
		}
		break;

	// ���O�C���`�F�b�N
	case WFLBY_CONNECT_SEQ_LOGIN_CHECK:
		if( p_param->check_skip == FALSE ){
			WFLBY_CONNECT_WIN_Print( &p_wk->talk_system, dwc_message_0002 );
			p_wk->p_yesno = BmpYesNoSelectInit( p_wk->p_bgl, &sc_WFLBY_BMPWIN_DAT_YESNO, 
					WFLBY_SYSWINGRA_CGX, WFLBY_MAIN_PLTT_SYSWIN, HEAPID_WFLBY_ROOM );
			(*p_seq)++;
		}else{
#ifdef WFLBY_CONNECT_DEBUG_START
			// ���ԒZ�k���[�h�ł���������I�΂Ȃ�
			if( D_Tomoya_WiFiLobby_DebugStart == TRUE ){
				(*p_seq) = WFLBY_CONNECT_SEQ_DEBUG_SEL_ROOM;
			}else{
				(*p_seq) = WFLBY_CONNECT_SEQ_LOGIN;
			}
#else
			(*p_seq) = WFLBY_CONNECT_SEQ_LOGIN;
#endif
		}
		break;

	// ���O�C���`�F�b�N�҂�
	case WFLBY_CONNECT_SEQ_LOGIN_CHECK_WAIT:
		{
			u32 result;
			result = BmpYesNoSelectMain( p_wk->p_yesno, HEAPID_WFLBY_ROOM );
			switch( result ){
			// YES
			case 0:	

#ifdef WFLBY_CONNECT_DEBUG_START
				// ���ԒZ�k���[�h�ł���������I�΂Ȃ�
				if( D_Tomoya_WiFiLobby_DebugStart == TRUE ){
					(*p_seq) = WFLBY_CONNECT_SEQ_DEBUG_SEL_ROOM;
				}else{
					(*p_seq) = WFLBY_CONNECT_SEQ_LOGIN;
				}
#else
				(*p_seq) = WFLBY_CONNECT_SEQ_LOGIN;
#endif
				break;

			// NO
			case BMPMENU_CANCEL:
				WFLBY_CONNECT_WIN_Off( &p_wk->talk );
				(*p_seq) = WFLBY_CONNECT_SEQ_FADEOUT;
				break;
			}
		}
		break;

#ifdef WFLBY_CONNECT_DEBUG_START
	case WFLBY_CONNECT_SEQ_DEBUG_SEL_ROOM:

		WFLBY_CONNECT_WIN_PrintDEBUG( &p_wk->talk, MSG_WFLBY_00, DEBUG_SEL_ROOM );
		(*p_seq)++;

	case WFLBY_CONNECT_SEQ_DEBUG_SEL_ROOMWAIT:
		if( sys.trg & PAD_KEY_UP ){
			DEBUG_SEL_ROOM = (DEBUG_SEL_ROOM + 1) % WFLBY_ROOM_NUM;
			WFLBY_CONNECT_WIN_PrintDEBUG( &p_wk->talk, MSG_WFLBY_00, DEBUG_SEL_ROOM );
		}
		if( sys.trg & PAD_KEY_DOWN ){
			
			DEBUG_SEL_ROOM --;
			if( DEBUG_SEL_ROOM < 0 ){
				DEBUG_SEL_ROOM += WFLBY_ROOM_NUM;
			}
			WFLBY_CONNECT_WIN_PrintDEBUG( &p_wk->talk, MSG_WFLBY_00, DEBUG_SEL_ROOM );
		}
		if( sys.trg & PAD_BUTTON_A ){
			(*p_seq)++;
		}
		break;

	case WFLBY_CONNECT_SEQ_DEBUG_SEL_SEASON:
		WFLBY_CONNECT_WIN_PrintDEBUG( &p_wk->talk, MSG_WFLBY_01, DEBUG_SEL_SEASON );
		(*p_seq)++;

	case WFLBY_CONNECT_SEQ_DEBUG_SEL_SEASONWAIT:
		if( sys.trg & PAD_KEY_UP ){
			DEBUG_SEL_SEASON = (DEBUG_SEL_SEASON + 1) % WFLBY_SEASON_NUM;
			WFLBY_CONNECT_WIN_PrintDEBUG( &p_wk->talk, MSG_WFLBY_01, DEBUG_SEL_SEASON );
		}
		if( sys.trg & PAD_KEY_DOWN ){
			
			DEBUG_SEL_SEASON --;
			if( DEBUG_SEL_SEASON < 0 ){
				DEBUG_SEL_SEASON += WFLBY_SEASON_NUM;
			}
			WFLBY_CONNECT_WIN_PrintDEBUG( &p_wk->talk, MSG_WFLBY_01, DEBUG_SEL_SEASON );
		}
		if( sys.trg & PAD_BUTTON_A ){
			(*p_seq)++;
		}
		break;

	case WFLBY_CONNECT_SEQ_DEBUG_SEL_ITEM:
		WFLBY_CONNECT_WIN_PrintDEBUG2( &p_wk->talk, MSG_WFLBY_02, DEBUG_SEL_ITEM );
		(*p_seq)++;

	case WFLBY_CONNECT_SEQ_DEBUG_SEL_ITEMWAIT:
		if( sys.repeat & PAD_KEY_UP ){
			DEBUG_SEL_ITEM = (DEBUG_SEL_ITEM + 1) % WFLBY_ITEM_NUM;
			WFLBY_CONNECT_WIN_PrintDEBUG2( &p_wk->talk, MSG_WFLBY_02, DEBUG_SEL_ITEM );
		}
		if( sys.repeat & PAD_KEY_DOWN ){
			
			DEBUG_SEL_ITEM --;
			if( DEBUG_SEL_ITEM < 0 ){
				DEBUG_SEL_ITEM += WFLBY_ITEM_NUM;
			}
			WFLBY_CONNECT_WIN_PrintDEBUG2( &p_wk->talk, MSG_WFLBY_02, DEBUG_SEL_ITEM );
		}
		if( sys.trg & PAD_BUTTON_A ){
			(*p_seq)++;
		}
		break;
#endif

	// ���O�C���J�n
	case WFLBY_CONNECT_SEQ_LOGIN:
		{
			WFLBY_USER_PROFILE* p_profile;

#ifdef WFLBY_CONNECT_DEBUG_START
			if( D_Tomoya_WiFiLobby_DebugStart == TRUE ){
				WFLBY_SYSTEM_DEBUG_SetItem( p_param->p_system, DEBUG_SEL_ITEM );
				p_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_param->p_system );
				CommStateWifiLobbyLogin_Debug( p_wk->p_save, p_profile, DEBUG_SEL_SEASON, DEBUG_SEL_ROOM );
			}else{
				p_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_param->p_system );
				CommStateWifiLobbyLogin( p_wk->p_save, p_profile );
			}
#else
			p_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_param->p_system );
			CommStateWifiLobbyLogin( p_wk->p_save, p_profile );
#endif
		}

		WFLBY_CONNECT_WIN_Print( &p_wk->talk_system, dwc_message_0008 );
		WFLBY_CONNECT_WIN_StartTimeWait( &p_wk->talk_system );
		(*p_seq)++;
		break;

	// DWC ���O�C���҂�
	case WFLBY_CONNECT_SEQ_LOGIN_WAIT_DWC:
		// �G���[����
		if( CommStateIsWifiError() || CommStateWifiLobbyError() ){
			WFLBY_CONNECT_WIN_EndTimeWait( &p_wk->talk_system );
			(*p_seq) = WFLBY_CONNECT_SEQ_ERRON;
		}

		if( CommStateWifiLobbyDwcLoginCheck() == TRUE ){	// DWC_LoginAsync�̐ڑ�����
			// WiFi�N���u�ł̏�Ԃ�NONE�ɂ��鏈��
			WFLBY_SYSTEM_WiFiClubBuff_Init( p_param->p_system );
			(*p_seq)++;
		}
		break;

	// ���O�C���҂�
	case WFLBY_CONNECT_SEQ_LOGIN_WAIT:
		
		// �G���[����
		if( CommStateIsWifiError() || CommStateWifiLobbyError() ){
			WFLBY_CONNECT_WIN_EndTimeWait( &p_wk->talk_system );
			(*p_seq) = WFLBY_CONNECT_SEQ_ERRON;
		}

		if( CommStateIsWifiLoginState() ){

			WFLBY_CONNECT_WIN_EndTimeWait( &p_wk->talk_system );

			{
				RECORD* p_rec;

				p_rec = SaveData_GetRecord( p_wk->p_save );
				RECORD_Score_Add( p_rec, SCORE_ID_LOBBY_LOGIN );
			}

			// ���O�C���������Ԃ�ۑ�
			{
				p_param->p_wflby_counter->time = GF_RTC_GetDateTimeBySecond();
			}
			
			p_param->enter = TRUE;	// ���ꊮ��
			(*p_seq)++;
		}
		break;

	// �G���[�\��
	case WFLBY_CONNECT_SEQ_ERRON:
		{
			COMMSTATE_DWCERROR* pErr;
			int msgno,err_no;
			if( CommStateIsWifiError() ){
				pErr = CommStateGetWifiError();
				msgno = WFLBY_ERR_GetStrID( pErr->errorCode,  pErr->errorType);
                err_no = pErr->errorCode;
			}else{
				err_no = DWC_LOBBY_GetErr();
				err_no = DWC_LOBBY_GetErrNo( err_no );
				msgno = dwc_lobby_0001;
			}

			// WiFi�N���u�ł̏�Ԃ�NONE�ɂ���o�b�t�@��j��
			WFLBY_SYSTEM_WiFiClubBuff_Exit( p_param->p_system );

			WFLBY_CONNECT_WIN_Off( &p_wk->talk );
			WFLBY_CONNECT_WIN_Off( &p_wk->talk_system );
			WFLBY_CONNECT_WIN_SetErrNumber( &p_wk->system, err_no );
			WFLBY_CONNECT_WIN_Print( &p_wk->system, msgno );
			(*p_seq)++;
		}
		break;

	// �L�[���͑҂�
	case WFLBY_CONNECT_SEQ_ERR:
		if( sys.trg & PAD_BUTTON_DECIDE ){
			WFLBY_ERR_TYPE err_type;
			int err_no;

			if( CommStateIsWifiError() ){
				COMMSTATE_DWCERROR* pErr = CommStateGetWifiError();
				err_type = WFLBY_ERR_GetErrType( pErr->errorCode, pErr->errorType );
				if( err_type == WFLBY_ERR_TYPE_RETRY ){
					// �Đڑ�
					(*p_seq) = WFLBY_CONNECT_SEQ_RETRY;
				}else{
					// ������
					(*p_seq) = WFLBY_CONNECT_SEQ_LOGOUT;
					break;
				}
			}else{

				// �Đڑ�
				(*p_seq) = WFLBY_CONNECT_SEQ_RETRY;
			}
		}
		break;

	// �Đڑ�����
	case WFLBY_CONNECT_SEQ_RETRY:
		WFLBY_CONNECT_WIN_Off( &p_wk->system );
		WFLBY_CONNECT_WIN_Print( &p_wk->talk, msg_wifilobby_052 );

		p_wk->p_yesno = BmpYesNoSelectInit( p_wk->p_bgl, &sc_WFLBY_BMPWIN_DAT_YESNO, 
				WFLBY_SYSWINGRA_CGX, WFLBY_MAIN_PLTT_SYSWIN, HEAPID_WFLBY_ROOM );
		(*p_seq)++;
		break;
	
	// �Đڑ���
	case WFLBY_CONNECT_SEQ_RETRYWAIT:
		{
			u32 result;
			result = BmpYesNoSelectMain( p_wk->p_yesno, HEAPID_WFLBY_ROOM );
			switch( result ){
			// YES
			case 0:	
				// �������񃍃O�A�E�g
				CommStateWifiLobbyLogout();
				(*p_seq) = WFLBY_CONNECT_SEQ_RETRYLOGOUTWAIT;
				break;

			// NO
			case BMPMENU_CANCEL:
				(*p_seq) = WFLBY_CONNECT_SEQ_LOGOUT;
				break;
			}
		}
		break;

	// �Đڑ����̃��O�A�E�g�҂�
	case WFLBY_CONNECT_SEQ_RETRYLOGOUTWAIT:
		if( CommStateIsInitialize() == FALSE ){
			(*p_seq) = WFLBY_CONNECT_SEQ_LOGIN;
		}
		break;

	// ���O�A�E�g
	case WFLBY_CONNECT_SEQ_LOGOUT:
		WFLBY_CONNECT_WIN_Off( &p_wk->talk );
		WFLBY_CONNECT_WIN_Off( &p_wk->talk_system );
		WFLBY_CONNECT_WIN_Off( &p_wk->system );
		CommStateWifiLobbyLogout();
		(*p_seq)++;
		break;
		
	// ���O�A�E�g�܂�
	case WFLBY_CONNECT_SEQ_LOGOUTWAIT:
		if( CommStateIsInitialize() == FALSE ){
			WFLBY_CONNECT_WIN_Off( &p_wk->talk );
			WFLBY_CONNECT_WIN_Off( &p_wk->talk_system );
			(*p_seq) = WFLBY_CONNECT_SEQ_FADEOUT;
		}
		break;

	// �t�F�[�h�A�E�g
	case WFLBY_CONNECT_SEQ_FADEOUT:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WFLBY_ROOM );
		(*p_seq)++;
		break;

	// �t�F�[�h�A�E�g�҂�
	case WFLBY_CONNECT_SEQ_FADEOUTWAIT:
		if( WIPE_SYS_EndCheck() ){
			return PROC_RES_FINISH;
		}
		break;

	default:
		GF_ASSERT( 0 );
		break;
	}

	ConnectBGPalAnm_Main(&p_wk->cbp);
	
	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�C���@�I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBY_CONNECT_Exit(PROC* p_proc, int* p_seq)
{
	WFLBY_CONNECTWK* p_wk;
	
	p_wk	= PROC_GetWork( p_proc );

	// ���荞�ݐݒ�
	sys_VBlankFuncChange( NULL, NULL );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	ConnectBGPalAnm_End(&p_wk->cbp);

	// �E�B���h�E�V�X�e���j��
	WFLBY_CONNECT_WIN_Exit( &p_wk->talk );
	WFLBY_CONNECT_WIN_Exit( &p_wk->talk_system );
	WFLBY_CONNECT_WIN_Exit( &p_wk->system );
	WFLBY_CONNECT_WIN_Exit( &p_wk->title );

	// �O���t�B�b�N�j��
	WFLBY_CONNECT_GraphicExit( p_wk );

	ConnectBGPalAnm_OccSet(&p_wk->cbp, FALSE);

	// ���[�N�j��
	sys_FreeMemoryEz( p_wk );

	// �q�[�v�j��
	sys_DeleteHeap( HEAPID_WFLBY_ROOM );

	return PROC_RES_FINISH;
}




//----------------------------------------------------------------------------
/**
 *	@brief	�ؒf����	������
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBY_DISCONNECT_Init(PROC* p_proc, int* p_seq)
{
	WFLBY_CONNECTWK* p_wk;
	WFLBY_DISCONNECT_PARAM* p_param;

	// �q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WFLBY_ROOM, 0x20000 );
	
	// ���[�N�쐬
	p_wk = PROC_AllocWork( p_proc, sizeof(WFLBY_CONNECTWK), HEAPID_WFLBY_ROOM );
	memset( p_wk, 0, sizeof(WFLBY_CONNECTWK) );

	// �p�����[�^�擾
	p_param	= PROC_GetParentWork( p_proc );
	p_wk->p_save		= p_param->p_save;

	// �O���t�B�b�N������
	WFLBY_CONNECT_GraphicInit( p_wk, HEAPID_WFLBY_ROOM );

	// �E�B���h�E�V�X�e��������
	WFLBY_CONNECT_WIN_Init( &p_wk->talk, p_wk->p_bgl, FONT_TALK, NARC_msg_wifi_system_dat,
			WFLBY_TALKWIN_X, WFLBY_TALKWIN_Y, 
			WFLBY_TALKWIN_SIZX, WFLBY_TALKWIN_SIZY,
			WFLBY_TALKWIN_CGX, p_wk->p_save, HEAPID_WFLBY_ROOM );
	WFLBY_CONNECT_WIN_Init( &p_wk->system, p_wk->p_bgl, FONT_SYSTEM, NARC_msg_wifi_system_dat,
			WFLBY_SYSTEMWIN_X, WFLBY_SYSTEMWIN_Y,
			WFLBY_SYSTEMWIN_SIZX, WFLBY_SYSTEMWIN_SIZY,
			WFLBY_SYSTEMWIN_CGX, p_wk->p_save, HEAPID_WFLBY_ROOM );


	WFLBY_CONNECT_WIN_Init( &p_wk->title, p_wk->p_bgl, FONT_TALK, NARC_msg_wifi_lobby_dat,
			WFLBY_TITLEWIN_X, WFLBY_TITLEWIN_Y, 
			WFLBY_TITLEWIN_SIZX, WFLBY_TITLEWIN_SIZY,
			WFLBY_TITLEWIN_CGX, p_wk->p_save, HEAPID_WFLBY_ROOM );


	//  �^�C�g����������
	WFLBY_CONNECT_WIN_PrintTitle( &p_wk->title, msg_wifilobby_018 );


	// VBlank�֐��ݒ�
	sys_VBlankFuncChange( WFLBY_CONNECT_VBlank, p_wk );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	return PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ؒf����	���C��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBY_DISCONNECT_Main(PROC* p_proc, int* p_seq)
{
	WFLBY_CONNECTWK* p_wk;
	WFLBY_DISCONNECT_PARAM* p_param;
	
	p_wk	= PROC_GetWork( p_proc );
	p_param	= PROC_GetParentWork( p_proc );

	switch( *p_seq ){
	// �t�F�[�h�C��
	case WFLBY_DISCONNECT_SEQ_FADEIN:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_OUTCOLOR, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WFLBY_ROOM );
		(*p_seq)++;
		break;

	// �t�F�[�h�C���E�G�C�g
	case WFLBY_DISCONNECT_SEQ_FADEWAIT:
		if( WIPE_SYS_EndCheck() ){
#if 0	// �^�C���A�E�g���b�Z�[�W�͍L����ŏo�����ƂɂȂ���
			if( p_param->timeout == TRUE ){
				(*p_seq) = WFLBY_DISCONNECT_SEQ_TIMEOUT_MSG;
			}else{
			}
#endif
		(*p_seq) = WFLBY_DISCONNECT_SEQ_LOGOUT_MSG;
		}
		break;

#if 0	// �^�C���A�E�g���b�Z�[�W�͍L����ŏo�����ƂɂȂ���
	// �^�C���A�E�g�ގ������b�Z�[�W�\��
	case WFLBY_DISCONNECT_SEQ_TIMEOUT_MSG:
		WFLBY_CONNECT_WIN_PrintWait( &p_wk->talk, msg_hiroba_end_01 );
		(*p_seq) = WFLBY_DISCONNECT_SEQ_TIMEOUT_MSG_WAIT;
		break;

	// �^�C���A�E�g�ގ������b�Z�[�W�\���҂�
	case WFLBY_DISCONNECT_SEQ_TIMEOUT_MSG_WAIT:
		if( WFLBY_CONNECT_WIN_PrintEndWait( &p_wk->talk ) ){
			(*p_seq) = WFLBY_DISCONNECT_SEQ_LOGOUT_MSG;
		}
		break;
#endif

	// �ގ����b�Z�[�W�\��
	case WFLBY_DISCONNECT_SEQ_LOGOUT_MSG:
		WFLBY_CONNECT_WIN_Print( &p_wk->talk, dwc_message_0011 );
		// �G���[�`�F�b�N
		if( CommStateIsWifiError() || CommStateWifiLobbyError() ){
			// �G���[�Ȃ炷����LOGOUT
			(*p_seq) = WFLBY_DISCONNECT_SEQ_LOGOUT;
			WFLBY_CONNECT_WIN_StartTimeWait( &p_wk->talk );
		}else{
			(*p_seq) = WFLBY_DISCONNECT_SEQ_WLDSEND;
			WFLBY_CONNECT_WIN_StartTimeWait( &p_wk->talk );
		}
		break;

	//  ���E�f�[�^���M
	case WFLBY_DISCONNECT_SEQ_WLDSEND:
		DWC_LOBBY_WLDDATA_Send();

		// �^�C���E�G�C�g�ݒ�
		p_wk->wait = WFLBY_DISCONNECT_WLDSENDWAIT_TIMEOUT;
		(*p_seq) = WFLBY_DISCONNECT_SEQ_WLDWAIT;
		break;
		
	//  ���E�f�[�^���M�����҂�
	case WFLBY_DISCONNECT_SEQ_WLDWAIT:
		if( (p_wk->wait - 1) >= 0 ){
			p_wk->wait --;
		}
		// �f�[�^���u���[�h�L���X�g����邩�^�C���A�E�g�����邩�Ń��O�A�E�g�����ɂ���
		if( DWC_LOBBY_WLDDATA_SendWait() || (p_wk->wait == 0) ){
			(*p_seq) = WFLBY_DISCONNECT_SEQ_LOGOUT;
		}
		break;

	// ���O�A�E�g����
	case WFLBY_DISCONNECT_SEQ_LOGOUT:
		CommStateWifiLobbyLogout();
		(*p_seq) = WFLBY_DISCONNECT_SEQ_LOGOUT_WAIT;
		break;

	case WFLBY_DISCONNECT_SEQ_LOGOUT_WAIT:
		if( CommStateIsInitialize() == FALSE ){

			// �^�C���A�E�g�I��
			WFLBY_CONNECT_WIN_EndTimeWait( &p_wk->talk_system );

			// WiFi�N���u�p�ɑ�����_�~�[����j������
			WFLBY_SYSTEM_WiFiClubBuff_Exit( p_param->p_system );
			
			(*p_seq) = WFLBY_DISCONNECT_SEQ_LOGOUT_END;
		}
		break;

	// ���O�A�E�g�������b�Z�[�W
	case WFLBY_DISCONNECT_SEQ_LOGOUT_END:
		WFLBY_CONNECT_WIN_Print( &p_wk->talk, dwc_message_0012 );

		// �^�C���E�G�C�g�ݒ�
		p_wk->wait = WFLBY_DISCONNECT_LOGOUT_ENDMSG_WAIT;

		(*p_seq) = WFLBY_DISCONNECT_SEQ_LOGOUT_END_MSGWAIT;
		break;

	// ���O�A�E�g�������b�Z�[�W�҂�
	case WFLBY_DISCONNECT_SEQ_LOGOUT_END_MSGWAIT:
		if( (p_wk->wait - 1) >= 0 ){
			p_wk->wait --;
		}
		if( p_wk->wait == 0 ){
			(*p_seq) = WFLBY_DISCONNECT_SEQ_FADEOUT;
		}
		break;

	case WFLBY_DISCONNECT_SEQ_FADEOUT:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WFLBY_ROOM );
		(*p_seq)++;
		break;

	case WFLBY_DISCONNECT_SEQ_FADEOUTWAIT:
		if( WIPE_SYS_EndCheck() ){
			WFLBY_CONNECT_WIN_EndTimeWait( &p_wk->talk );
			return PROC_RES_FINISH;
		}
		break;

	}

	ConnectBGPalAnm_Main(&p_wk->cbp);
	
	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ؒf����	�j��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBY_DISCONNECT_Exit(PROC* p_proc, int* p_seq)
{
	WFLBY_CONNECTWK* p_wk;
	
	p_wk	= PROC_GetWork( p_proc );

	// ���荞�ݐݒ�
	sys_VBlankFuncChange( NULL, NULL );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	ConnectBGPalAnm_End(&p_wk->cbp);

	// �E�B���h�E�V�X�e���j��
	WFLBY_CONNECT_WIN_Exit( &p_wk->talk );
	WFLBY_CONNECT_WIN_Exit( &p_wk->system );
	WFLBY_CONNECT_WIN_Exit( &p_wk->title );

	// �O���t�B�b�N�j��
	WFLBY_CONNECT_GraphicExit( p_wk );

	ConnectBGPalAnm_OccSet(&p_wk->cbp, FALSE);

	// ���[�N�j��
	sys_FreeMemoryEz( p_wk );

	// �q�[�v�j��
	sys_DeleteHeap( HEAPID_WFLBY_ROOM );

	return PROC_RES_FINISH;
}





//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�֐�
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_VBlank( void* p_work )
{
	WFLBY_CONNECTWK* p_wk = p_work;

	WFLBY_CONNECT_GraphicVBlank( p_wk );
	
	ConnectBGPalAnm_VBlank(&p_wk->cbp);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N������
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_GraphicInit( WFLBY_CONNECTWK* p_wk, u32 heapID )
{
	// ���������W�X�^OFF
	G2_BlendNone();
	G2S_BlendNone();
	
	// BANK�ݒ�
	GF_Disp_SetBank( &sc_WFLBY_BANK );

	// �o�b�N�O���E���h�����ɂ���
	{
		GF_BGL_BackGroundColorSet( sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_MAIN_BACK], 0 );
	}
	

	// BG�ݒ�
	{
		int i;

		GF_BGL_InitBG(&sc_BGINIT);

		p_wk->p_bgl = GF_BGL_BglIniAlloc( heapID );

		for( i=0; i<WFLBY_BGCNT_NUM; i++ ){
			GF_BGL_BGControlSet( p_wk->p_bgl, 
					sc_WFLBY_BGCNT_FRM[i], &sc_WFLBY_BGCNT_DATA[i],
					GF_BGL_MODE_TEXT );
			GF_BGL_ClearCharSet( sc_WFLBY_BGCNT_FRM[i], 32, 0, heapID);
			GF_BGL_ScrClear( p_wk->p_bgl, sc_WFLBY_BGCNT_FRM[i] );
		}
	}
	

	// �E�B���h�E�ݒ�
	{
		CONFIG* p_config;
		u8 winnum;

		p_config = SaveData_GetConfig( p_wk->p_save );
		winnum = CONFIG_GetWindowType( p_config );

		// �����p���b�g
		SystemFontPaletteLoad( PALTYPE_MAIN_BG, WFLBY_MAIN_PLTT_SYSFONT*32, heapID );
		TalkFontPaletteLoad( PALTYPE_MAIN_BG, WFLBY_MAIN_PLTT_TALKFONT*32, heapID );

		// �E�B���h�E�O���t�B�b�N
		MenuWinGraphicSet(
				p_wk->p_bgl, sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_MAIN_WIN], WFLBY_SYSWINGRA_CGX,
				WFLBY_MAIN_PLTT_SYSWIN, 0, heapID );
		TalkWinGraphicSet(
				p_wk->p_bgl, sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_MAIN_WIN], WFLBY_TALKWINGRA_CGX, 
				WFLBY_MAIN_PLTT_TALKWIN, winnum, heapID );
	}


	// �o�b�N�O���t�B�b�N����������
	{
		// �J���[�p���b�g
		ArcUtil_PalSet( ARC_WIFIP2PMATCH_GRA, 
				NARC_wifip2pmatch_conect_NCLR, 
				PALTYPE_MAIN_BG, WFLBY_MAIN_PLTT_BACKSTART, 
				WFLBY_MAIN_PLTT_BACKEND*32, heapID );
		ArcUtil_PalSet( ARC_WIFIP2PMATCH_GRA, 
				NARC_wifip2pmatch_conect_NCLR, 
				PALTYPE_SUB_BG, WFLBY_MAIN_PLTT_BACKSTART, 
				WFLBY_MAIN_PLTT_BACKEND*32, heapID );

		// �L�����N�^
		ArcUtil_BgCharSet( ARC_WIFIP2PMATCH_GRA, 
				NARC_wifip2pmatch_conect_NCGR, p_wk->p_bgl, 
				sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_MAIN_BACK], 0, 0, FALSE, heapID );
		ArcUtil_BgCharSet( ARC_WIFIP2PMATCH_GRA, 
				NARC_wifip2pmatch_conect_sub_NCGR, p_wk->p_bgl, 
				sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_SUB_BACK], 0, 0, FALSE, heapID );

		// �X�N���[��
		ArcUtil_ScrnSet( ARC_WIFIP2PMATCH_GRA, NARC_wifip2pmatch_conect_01_NSCR,
				p_wk->p_bgl, sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_MAIN_BACK], 0, 0, FALSE, heapID );
		ArcUtil_ScrnSet( ARC_WIFIP2PMATCH_GRA, NARC_wifip2pmatch_conect_sub_NSCR,
				p_wk->p_bgl, sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_SUB_BACK], 0, 0, FALSE, heapID );
	}

	// �ʐM�O���t�B�b�NON
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
	
	{
		ARCHANDLE* p_handle;
		p_handle = ArchiveDataHandleOpen( ARC_WIFIP2PMATCH_GRA, heapID );

		//Wifi�ڑ�BG�p���b�g�A�j���V�X�e��������
		ConnectBGPalAnm_Init(&p_wk->cbp, p_handle, 
			NARC_wifip2pmatch_conect_anm_NCLR, heapID);

		ArchiveDataHandleClose( p_handle );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�j��
 *	
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_GraphicExit( WFLBY_CONNECTWK* p_wk )
{
	// BG�̔j��
	{
		int i;

		for( i=0; i<WFLBY_BGCNT_NUM; i++ ){
			GF_BGL_BGControlExit( p_wk->p_bgl, sc_WFLBY_BGCNT_FRM[i] );
		}

		// BGL�j��
		sys_FreeMemoryEz( p_wk->p_bgl );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N	VBLANK
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_GraphicVBlank( WFLBY_CONNECTWK* p_wk )
{
	// BGLVBLANK
    GF_BGL_VBlankFunc( p_wk->p_bgl );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�I�u�W�F������
 *	
 *	@param	p_wk		���[�N
 *	@param	p_bgl		BGL
 *	@param	fontid		�t�H���gID
 *	@param	msgid		���b�Z�[�WID
 *	@param	x			�����W
 *	@param	y			�����W
 *	@param	sizx		�T�C�Y��
 *	@param	sizy		�T�C�Y��
 *	@param	cgx			�L�����N�^�I�t�Z�b�g
 *	@param	p_save		�Z�[�u�f�[�^
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_Init( WFLBY_WINWK* p_wk, GF_BGL_INI* p_bgl, u32 fontid, u32 msgid, u32 x, u32 y, u32 sizx, u32 sizy, u32 cgx, SAVEDATA* p_save, u32 heapID )
{
	p_wk->p_wordset = WORDSET_Create( heapID );
	p_wk->p_msgman	= MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, msgid, heapID );
	p_wk->p_str		= STRBUF_Create( WFLBY_WINSYS_STRBUFNUM, heapID );
	p_wk->p_tmp		= STRBUF_Create( WFLBY_WINSYS_STRBUFNUM, heapID );
	p_wk->fontid	= fontid;
	p_wk->msgspeed	= CONFIG_GetMsgPrintSpeed( SaveData_GetConfig( p_save ) );
	p_wk->msgno		= 0;

#ifdef WFLBY_CONNECT_DEBUG_START
	p_wk->p_debug_msgman	= MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_debug_tomoya_dat, heapID );;	// ���[�h�Z�b�g
#endif

	GF_BGL_BmpWinAdd( p_bgl, &p_wk->win, 
			sc_WFLBY_BGCNT_FRM[WFLBY_BGCNT_MAIN_WIN], x, y, sizx, sizy, WFLBY_MAIN_PLTT_TALKFONT, cgx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�I�u�W�F	���b�Z�[�W�\��
 *
 *	@param	p_wk		���[�N
 *	@param	strid		�\��STR
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_Print( WFLBY_WINWK* p_wk, u32 strid )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}
	
	// �E�B���h�E�̃N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );

	// �\��
	MSGMAN_GetString( p_wk->p_msgman, strid, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_str, p_wk->p_tmp );

	GF_STR_PrintSimple(&p_wk->win, FONT_TALK, p_wk->p_str,
			0,0,MSG_NO_PUT,NULL);
	
	if( p_wk->fontid == FONT_SYSTEM ){	
		BmpMenuWinWrite( &p_wk->win, WINDOW_TRANS_OFF, 
				WFLBY_SYSWINGRA_CGX, WFLBY_MAIN_PLTT_SYSWIN );
	}else{
		BmpTalkWinWrite( &p_wk->win, WINDOW_TRANS_OFF, 
				WFLBY_TALKWINGRA_CGX, WFLBY_MAIN_PLTT_TALKWIN );
	}
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�\��OFF
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_Off( WFLBY_WINWK* p_wk )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	if( p_wk->fontid == FONT_SYSTEM ){
		BmpMenuWinClear( &p_wk->win, WINDOW_TRANS_OFF );
		GF_BGL_BmpWinOffVReq( &p_wk->win );
	}else{
		if( p_wk->p_timewait ){
			WFLBY_CONNECT_WIN_EndTimeWait( p_wk );
		}

		BmpTalkWinClear( &p_wk->win, WINDOW_TRANS_OFF );
		GF_BGL_BmpWinOffVReq( &p_wk->win );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���E�G�C�g�J�n
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_StartTimeWait( WFLBY_WINWK* p_wk )
{
	if( p_wk->fontid == FONT_TALK ){
		GF_ASSERT( p_wk->p_timewait == NULL );
		p_wk->p_timewait = TimeWaitIconAdd( &p_wk->win, WFLBY_TALKWINGRA_CGX );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���E�G�C�g��~
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_EndTimeWait( WFLBY_WINWK* p_wk )
{
	if( p_wk->fontid == FONT_TALK ){
		GF_ASSERT( p_wk->p_timewait != NULL );
		TimeWaitIconDel( p_wk->p_timewait );
		p_wk->p_timewait = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�I�u�W�F	OFF
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_Exit( WFLBY_WINWK* p_wk )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	if( p_wk->p_timewait ){
		WFLBY_CONNECT_WIN_EndTimeWait( p_wk );
	}
	
	GF_BGL_BmpWinDel( &p_wk->win );

	STRBUF_Delete( p_wk->p_tmp );
	STRBUF_Delete( p_wk->p_str );
	MSGMAN_Delete( p_wk->p_msgman );
	WORDSET_Delete( p_wk->p_wordset );

#ifdef WFLBY_CONNECT_DEBUG_START
	MSGMAN_Delete( p_wk->p_debug_msgman );
#endif
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����Z�[�W�`��
 *
 *	@param	p_wk		���[�N
 *	@param	strid		������
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_PrintWait( WFLBY_WINWK* p_wk, u32 strid )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}
	
	// �E�B���h�E�̃N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );

	// �\��
	MSGMAN_GetString( p_wk->p_msgman, strid, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_str, p_wk->p_tmp );

	p_wk->msgno = GF_STR_PrintSimple(&p_wk->win, FONT_TALK, p_wk->p_str,
			0,0, p_wk->msgspeed,NULL);
	
	if( p_wk->fontid == FONT_SYSTEM ){	
		BmpMenuWinWrite( &p_wk->win, WINDOW_TRANS_OFF, 
				WFLBY_SYSWINGRA_CGX, WFLBY_MAIN_PLTT_SYSWIN );
	}else{
		BmpTalkWinWrite( &p_wk->win, WINDOW_TRANS_OFF, 
				WFLBY_TALKWINGRA_CGX, WFLBY_MAIN_PLTT_TALKWIN );
	}
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W���I���������`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�I��
 	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_CONNECT_WIN_PrintEndWait( const WFLBY_WINWK* cp_wk )
{
	if( GF_MSG_PrintEndCheck( cp_wk->msgno ) == 0 ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g�������`��֐�
 *
 *	@param	p_wk		���[�N	
 *	@param	strid		������ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_PrintTitle( WFLBY_WINWK* p_wk, u32 strid )
{
	// �E�B���h�E�̃N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 0 );

	// �\��
	MSGMAN_GetString( p_wk->p_msgman, strid, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_str, p_wk->p_tmp );

	GF_STR_PrintColor(&p_wk->win, FONT_TALK, p_wk->p_str,
			0,0, MSG_ALLPUT, WFLBY_TITLEWIN_COL, NULL);

}


#ifdef WFLBY_CONNECT_DEBUG_START
//----------------------------------------------------------------------------
/**
 *	@brief	�f�o�b�N�����̕\��
 *
 *	@param	p_wk		���[�N
 *	@param	strid		������
 *	@param	num			����
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_PrintDEBUG( WFLBY_WINWK* p_wk, u32 strid, u32 num )
{
	// ���[�h�Z�b�g�ɐ����ݒ�
	WORDSET_RegisterNumber( p_wk->p_wordset, 0, num, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );

	// �E�B���h�E�̃N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );

	// �\��
	MSGMAN_GetString( p_wk->p_debug_msgman, strid, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_str, p_wk->p_tmp );

	GF_STR_PrintSimple(&p_wk->win, FONT_TALK, p_wk->p_str,
			0,0,MSG_NO_PUT,NULL);
	
	if( p_wk->fontid == FONT_SYSTEM ){	
		BmpMenuWinWrite( &p_wk->win, WINDOW_TRANS_OFF, 
				WFLBY_SYSWINGRA_CGX, WFLBY_MAIN_PLTT_SYSWIN );
	}else{
		BmpTalkWinWrite( &p_wk->win, WINDOW_TRANS_OFF, 
				WFLBY_TALKWINGRA_CGX, WFLBY_MAIN_PLTT_TALKWIN );
	}
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

static void WFLBY_CONNECT_WIN_PrintDEBUG2( WFLBY_WINWK* p_wk, u32 strid, u32 item )
{
	// ���[�h�Z�b�g�ɐ����ݒ�
	WORDSET_RegisterWiFiLobbyItemName( p_wk->p_wordset, 0, item );

	// �E�B���h�E�̃N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );

	// �\��
	MSGMAN_GetString( p_wk->p_debug_msgman, strid, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_str, p_wk->p_tmp );

	GF_STR_PrintSimple(&p_wk->win, FONT_TALK, p_wk->p_str,
			0,0,MSG_NO_PUT,NULL);
	
	if( p_wk->fontid == FONT_SYSTEM ){	
		BmpMenuWinWrite( &p_wk->win, WINDOW_TRANS_OFF, 
				WFLBY_SYSWINGRA_CGX, WFLBY_MAIN_PLTT_SYSWIN );
	}else{
		BmpTalkWinWrite( &p_wk->win, WINDOW_TRANS_OFF, 
				WFLBY_TALKWINGRA_CGX, WFLBY_MAIN_PLTT_TALKWIN );
	}
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}
#endif


//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�ɃG���[�i���o�[��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	number		����
 */
//-----------------------------------------------------------------------------
static void WFLBY_CONNECT_WIN_SetErrNumber( WFLBY_WINWK* p_wk, u32 number )
{
    WORDSET_RegisterNumber( p_wk->p_wordset, 0, number,
                           5, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);
}

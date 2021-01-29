//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		lobby_news.c
 *	@brief		���r�[�j���[�X
 *	@author		tomoya takahashi
 *	@data		2007.10.09
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "gflib/strbuf.h"
#include "gflib/msg_print.h"

#include "system/clact_util.h"
#include "system/wipe.h"
#include "system/render_oam.h"
#include "system/fontproc.h"
#include "system/msgdata.h"
#include "system/wordset.h"

#include "communication/communication.h"

#include "src/graphic/lobby_news.naix"

#include "msgdata/msg.naix"
#include "msgdata/msg_wflby_news.h"

#include "lobby_news.h"
#include "lobby_news_data.h"
#include "lobby_news_snd.h"
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
#define WFLBY_DEBUG_LOBBY_NEWS_TOPIC_MAKE
#endif

#ifdef WFLBY_DEBUG_LOBBY_NEWS_TOPIC_MAKE
static u32 sc_WFLBY_DEBUG_LOBBY_NEWS_TOPIC_MAKE_TOPIC = NEWS_TOPICTYPE_CONNECT;
#endif


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	Vram�]���}�l�[�W���@�^�X�N��
//=====================================
#define NEWSDRAW_VRAMTRANS_TASKNUM	( 48 )

//-------------------------------------
///	OAM�ݒ�
//=====================================
#define NEWSDRAW_OAM_CONTNUM	( 32 )
#define NEWSDRAW_RESMAN_NUM		( 4 )	// OAM���\�[�X�}�l�[�W����
#define NEWSDRAW_SF_MAT_Y		( FX32_CONST(256) )
static const CHAR_MANAGER_MAKE sc_NEWSDRAW_CHARMAN_INIT = {
	NEWSDRAW_OAM_CONTNUM,
	96*1024,	// 96K
	16*1024,	// 16K
	HEAPID_NEWSDRAW
};


//-------------------------------------
///	BANK�ݒ�
//=====================================
static const GF_BGL_DISPVRAM sc_NEWSDRAW_BANK = {
	GX_VRAM_BG_256_AB,				// ���C��2D�G���W����BG
	GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
	GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
	GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
	GX_VRAM_OBJ_96_EFG,				// ���C��2D�G���W����OBJ
	GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
	GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
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
#define NEWSDRAW_BGCNT_NUM	( 5 )	// �a�f�R���g���[���e�[�u����
static const u32 sc_NEWSDRAW_BGCNT_FRM[ NEWSDRAW_BGCNT_NUM ] = {
	GF_BGL_FRAME0_M,
	GF_BGL_FRAME1_M,
	GF_BGL_FRAME2_M,
	GF_BGL_FRAME3_M,
	GF_BGL_FRAME0_S,
};
static const GF_BGL_BGCNT_HEADER sc_NEWSDRAW_BGCNT_DATA[ NEWSDRAW_BGCNT_NUM ] = {
	{	// GF_BGL_FRAME0_M	���b�Z�[�W��
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME1_M	�t���[����
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
		1, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME2_M	�w�i
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
		3, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME3_M	���낢��f�[�^�\��
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
		2, 0, 0, FALSE
	},
	
	// �T�u
	{	// GF_BGL_FRAME0_S
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},
};

//-------------------------------------
///	�p���b�g�i���o�[
//=====================================
enum {
	NEWS_PLTT_BACK,
	NEWS_PLTT_TIME,
	NEWS_PLTT_PLAYER,
	NEWS_PLTT_YOU,
	NEWS_PLTT_VIP,
	NEWS_PLTT_SUB,
	NEWS_PLTT_DUMMY00,
	NEWS_PLTT_FONT,
	NEWS_PLTT_FONT01,
	NEWS_PLTT_FONT02,
	NEWS_PLTT_TALKFONT,
} ;

//-------------------------------------
///	���b�Z�[�W�p���b�g���ǂݍ��ݎ��J���[�I�t�Z�b�g
//=====================================
enum {
	NEWS_PLTT_FONT_BACK,	// �w�i�J���[
	NEWS_PLTT_FONT_NORM,	// �ʏ핶���F
	NEWS_PLTT_FONT_NORM_B,	// �ʏ핶���F�w�i
	NEWS_PLTT_FONT_RED,		// ���������F
	NEWS_PLTT_FONT_RED_B,	// ���������F�w�i
	NEWS_PLTT_FONT_TRN,		// �ʏ�g���[�i�[�����F
	NEWS_PLTT_FONT_TRN_B,	// �ʏ�g���[�i�[�����F�w�i
	NEWS_PLTT_FONT_FONT,	// �ʏ핶���F
	NEWS_PLTT_FONT_FONT_B,	// �ʏ핶���F�w�i
	NEWS_PLTT_FONT_TRVIP,	// VIP�g���[�i�[�����F
	NEWS_PLTT_FONT_TRVIP_B,	// VIP�g���[�i�[�����F�w�i
} ;

//-------------------------------------
///	���b�Z�[�W�p���b�g���]�����J���[�I�t�Z�b�g
//=====================================
enum {
	NEWS_PLTT_FONTTR_BACK,		// �w�i�J���[
	NEWS_PLTT_FONTTR_NORM,		// �ʏ핶���F
	NEWS_PLTT_FONTTR_NORM_B,	// �ʏ핶���F�w�i
	NEWS_PLTT_FONTTR_RED,		// ���������F
	NEWS_PLTT_FONTTR_RED_B,		// ���������F�w�i
	NEWS_PLTT_FONTTR_TR_00,		// �g���[�i�[0�����F
	NEWS_PLTT_FONTTR_TR_00_B,	// �g���[�i�[0�����F�w�i
	NEWS_PLTT_FONTTR_TR_01,		// �g���[�i�[1�����F
	NEWS_PLTT_FONTTR_TR_01_B,	// �g���[�i�[1�����F�w�i
	NEWS_PLTT_FONTTR_TR_02,		// �g���[�i�[2�����F
	NEWS_PLTT_FONTTR_TR_02_B,	// �g���[�i�[2�����F�w�i
	NEWS_PLTT_FONTTR_TR_03,		// �g���[�i�[3�����F
	NEWS_PLTT_FONTTR_TR_03_B,	// �g���[�i�[3�����F�w�i
	NEWS_PLTT_FONTTR_FONT,		// �ʏ핶���F
	NEWS_PLTT_FONTTR_FONT_B,	// �ʏ핶���F�w�i
} ;






//-------------------------------------
///	���C���V�[�P���X
//=====================================
enum{
	NEWSDRAW_SEQ_FADEIN,
	NEWSDRAW_SEQ_FADEINWAIT,
	NEWSDRAW_SEQ_MAIN,
	NEWSDRAW_SEQ_FADEOUT,
	NEWSDRAW_SEQ_FADEOUTWAIT,
};

//-------------------------------------
///	�X�N���[���Z�b�g
//=====================================
enum{
	NEWSDRAW_TIME_BLOCKTYPE_NORMAL,
	NEWSDRAW_TIME_BLOCKTYPE_ICON,
	NEWSDRAW_TIME_BLOCKTYPE_RED,
	NEWSDRAW_TIME_BLOCKTYPE_NONE,
	NEWSDRAW_TIME_BLOCKTYPE_ICON1,
	NEWSDRAW_TIME_BLOCKTYPE_ICON2,
	NEWSDRAW_TIME_BLOCKTYPE_ICON3,
};
#define NEWSDRAW_SCRN_TIMEBLOCK_X	( 6 )	// �X�N���[���؂����
#define NEWSDRAW_SCRN_TIMEBLOCK_Y	( 0 )
#define NEWSDRAW_TIME_BLOCK_SIZX		(1)		// �u���b�N�T�C�YX
#define NEWSDRAW_TIME_BLOCK_SIZY		(2)		// �u���b�N�T�C�YY
#define NEWSDRAW_TIME_SCRN_DRAWX		(5)		// �������݊J�n����L�����ʒu
#define NEWSDRAW_TIME_SCRN_DRAWY		(4)		// �������݊J�n����L�����ʒu
enum{
	NEWSDRAW_PLAYERTYPE_NML,
	NEWSDRAW_PLAYERTYPE_OUT,
	NEWSDRAW_PLAYERTYPE_IN,
};
#define NEWSDRAW_SCRN_PLAYERBLOCK_X	(0)		// �X�N���[���؂����
#define NEWSDRAW_SCRN_PLAYERBLOCK_Y	(0)
#define NEWSDRAW_PLAYER_BLOCK_SIZX		(2)		// �u���b�N�T�C�YX
#define NEWSDRAW_PLAYER_BLOCK_SIZY		(2)		// �u���b�N�T�C�YY
#define NEWSDRAW_PLAYER_SCRN_DRAWX		(21)	// �������݊J�n����L�����ʒu
#define NEWSDRAW_PLAYER_SCRN_DRAWY		(4)		// �������݊J�n����L�����ʒu
#define NEWSDRAW_PLAYER_SCRN_PAT_X		(3)		// �X�N���[���ɏ������܂�Ă���l�̃p�^�[���̗�
#define NEWSDRAW_PLAYER_SCRN_PAT_Y		(4)
#define NEWSDRAW_PLAYER_SCRN_OLDPAT_Y	(2)		// �Â��l�\���p�^�[���J�n�ʒu


//-------------------------------------
///	�g�s�b�N�E�B���h�E
//=====================================
#define	NEWSDRAW_TOPIC_NUM		( 3 )		// �\������g�s�b�N�X�̐�
#define NEWSDRAW_TOPIC_STRNUM	( 256 )		// ������
#define NEWSDRAW_TOPIC_COUNT1	( 7 )		// �P�������̃J�E���g
#define NEWSDRAW_TOPIC_COUNT2	( 6 )		// �P�������̃J�E���g
#define NEWSDRAW_TOPIC_COUNT3	( 5 )		// �P�������̃J�E���g
#define NEWSDRAW_TOPIC_CHARSIZ	( 12 )		// �P�����̃T�C�Y
#define NEWSDRAW_TOPIC_YOHAKU	( 256 )		// ������\������Ƃ��̗]���i�㉺�ɂ��j
static const u8 NEWSDRAW_TOPIC_DRAW_Y[ NEWSDRAW_TOPIC_NUM ] = {
	15,
	18,
	21,
};
/*
static const u8 NEWSDRAW_TOPIC_COUNT[ NEWSDRAW_TOPIC_NUM ] = {
	NEWSDRAW_TOPIC_COUNT1,
	NEWSDRAW_TOPIC_COUNT2,
	NEWSDRAW_TOPIC_COUNT3,
};//*/
// BTS�ʐM�o�O601�̑Ώ�	tomoya
#define NEWSDRAW_TOPIC_COUNT_STRNUM	( 48 )
static const u32 NEWSDRAW_TOPIC_COUNT[ NEWSDRAW_TOPIC_NUM ] = {
	NEWSDRAW_TOPIC_COUNT1*NEWSDRAW_TOPIC_COUNT_STRNUM,
	NEWSDRAW_TOPIC_COUNT2*NEWSDRAW_TOPIC_COUNT_STRNUM,
	NEWSDRAW_TOPIC_COUNT3*NEWSDRAW_TOPIC_COUNT_STRNUM,
};
#define NEWSDRAW_TOPIC_BMP_X	( 1 )		// �r�b�g�}�b�vX
#define NEWSDRAW_TOPIC_BMP_SX	( 30 )		// �r�b�g�}�b�v�T�C�YX
#define NEWSDRAW_TOPIC_BMP_SY	( 2 )		// �r�b�g�}�b�v�T�C�YY
#define NEWSDRAW_TOPIC_BMP_CGX		( 0x300 )	// CGX
#define NEWSDRAW_TOPIC_BMP_CGXSIZ	( NEWSDRAW_TOPIC_BMP_SX*NEWSDRAW_TOPIC_BMP_SY )
#define NEWSDRAW_TOPIC_BMP_PAL	( NEWS_PLTT_FONT )	// �p���b�g
#define NEWSDRAW_TOPIC_DMBMP_SX	( 180 )		// �r�b�g�}�b�v�T�C�YX(120��������)
#define NEWSDRAW_TOPIC_DMBMP_SY	( 2 )		// �r�b�g�}�b�v�T�C�YY



//-------------------------------------
///	���ԃE�B���h�E
//=====================================
#define NEWSDRAW_TIME_EFFECT_COUNT		(16)	// ���U�C�N�G�t�F�N�g�E�G�C�g
#define NEWSDRAW_TIME_TIME_EFFECT_COUNT	(32)	// ���U�C�N�G�t�F�N�g�E�G�C�g
#define NEWSDRAW_TIME_Y_SIZ				(4)		// �`��G���A�u���b�N�T�C�Y
#define NEWSDRAW_TIME_X_SIZ				(10)	// �`��G���A�u���b�N�T�C�Y
#define NEWSDRAW_TIME_BLOCKNUM		(NEWSDRAW_TIME_Y_SIZ*NEWSDRAW_TIME_X_SIZ)	// �`��G���A�u���b�N�T�C�Y
typedef struct{
	u32 icon;			// �\������A�C�R��
} NEWSDRAW_TIME_EFF_ANM;
#define NEWSDRAW_TIME_EFF_DATA_NUM		( 4 )
static const NEWSDRAW_TIME_EFF_ANM NEWSDRAW_TIME_EFF_ANM_DATA[ NEWSDRAW_TIME_EFF_DATA_NUM ] = {
	{ NEWSDRAW_TIME_BLOCKTYPE_ICON },
	{ NEWSDRAW_TIME_BLOCKTYPE_ICON1 },
	{ NEWSDRAW_TIME_BLOCKTYPE_ICON2 },
	{ NEWSDRAW_TIME_BLOCKTYPE_ICON3 },
};


//-------------------------------------
///	�v���C���[�E�B���h�E
//=====================================
enum{
	NEWSDRAW_PLAYERWIN_SEQ_WAIT,		// �ҋ@���
	NEWSDRAW_PLAYERWIN_SEQ_INSIDE,		// �l�������Ă���
	NEWSDRAW_PLAYERWIN_SEQ_INSIDEMAIN,	// �l�������Ă���
	NEWSDRAW_PLAYERWIN_SEQ_OUTSIDE,		// �l�������Ă���
	NEWSDRAW_PLAYERWIN_SEQ_OUTSIDEMAIN,	// �l�������Ă���
};
#define	NEWSDRAW_PLAYERWIN_COUNT_INSIDE		( 32 )	// �����J�E���g
#define	NEWSDRAW_PLAYERWIN_COUNT_OUTSIDE	( 32 )	// �ގ��J�E���g
#define NEWSDRAW_PLAYERWIN_ANIME_NUM		( 4 )	// ���̉񐔊G���؂�ւ��
#define	NEWSDRAW_PLAYERWIN_INSIDE_ONEFRAME	( NEWSDRAW_PLAYERWIN_COUNT_INSIDE/4 )	// �����J�E���g
#define	NEWSDRAW_PLAYERWIN_OUTSIDE_ONEFRAME	( NEWSDRAW_PLAYERWIN_COUNT_OUTSIDE/4 )	// �ގ��J�E���g
#define NEWSDRAW_PLAYERWIN_DRAW_X			( 5 )	// �\���l��
#define NEWSDRAW_PLAYERWIN_DRAW_Y			( 4 )	// �\���l��


//-------------------------------------
///	�^�C�g���E�B���h�E
//=====================================
#define NEWSDRAW_TITLEWIN_STRNUM	( 128 )	// STRBUF�̕�����
#define NEWSDRAW_TITLEWIN_COL		( GF_PRINTCOLOR_MAKE( NEWS_PLTT_FONTTR_FONT, NEWS_PLTT_FONTTR_FONT_B, 0 ) )	// �����J���[
#define NEWSDRAW_TITLEWIN_APL_COL	( GF_PRINTCOLOR_MAKE( 3, 4, 0 ) )	// �����J���[
enum {
	NEWSDRAW_TITLEWIN_APLNAME,	// ���̗V�т̖��O
	NEWSDRAW_TITLEWIN_TIME,		// ����
	NEWSDRAW_TITLEWIN_PLAYER,	// �v���C���[�\��
	NEWSDRAW_TITLEWIN_TOPIC,	// �g�s�b�N
	NEWSDRAW_TITLEWIN_NUM,
} ;
// �A�v����
#define NEWSDRAW_TITLEWIN_APLNAME_X		( 11 )		// �r�b�g�}�b�vX
#define NEWSDRAW_TITLEWIN_APLNAME_Y		( 0 )		// �r�b�g�}�b�vY
#define NEWSDRAW_TITLEWIN_APLNAME_SX	( 20 )		// �r�b�g�}�b�v�T�C�YX
#define NEWSDRAW_TITLEWIN_APLNAME_SY	( 8 )		// �r�b�g�}�b�v�T�C�YY
#define NEWSDRAW_TITLEWIN_APLNAME_CGX	( 0x1 )	// CGX
#define NEWSDRAW_TITLEWIN_APLNAME_PAL	( NEWS_PLTT_TALKFONT )	// �p���b�g
#define NEWSDRAW_TITLEWIN_APLNAME_DRAW_X	( 0 )
#define NEWSDRAW_TITLEWIN_APLNAME_DRAW_Y	( 6 )

// ����
#define NEWSDRAW_TITLEWIN_TIME_X		( 0 )		// �r�b�g�}�b�vX
#define NEWSDRAW_TITLEWIN_TIME_Y		( 6 )		// �r�b�g�}�b�vY
#define NEWSDRAW_TITLEWIN_TIME_SX		( 6 )		// �r�b�g�}�b�v�T�C�YX
#define NEWSDRAW_TITLEWIN_TIME_SY		( 3 )		// �r�b�g�}�b�v�T�C�YY
#define NEWSDRAW_TITLEWIN_TIME_CGX		( NEWSDRAW_TITLEWIN_APLNAME_CGX + (NEWSDRAW_TITLEWIN_APLNAME_SX*NEWSDRAW_TITLEWIN_APLNAME_SY) )	// CGX
#define NEWSDRAW_TITLEWIN_TIME_PAL		( NEWS_PLTT_FONT )	// �p���b�g
#define NEWSDRAW_TITLEWIN_TIME_DRAW_X	( 2 )
#define NEWSDRAW_TITLEWIN_TIME_DRAW_Y	( 4 )

// �v���C���[�\��
#define NEWSDRAW_TITLEWIN_PLAYER_X		( 15 )		// �r�b�g�}�b�vX
#define NEWSDRAW_TITLEWIN_PLAYER_Y		( 6 )		// �r�b�g�}�b�vY
#define NEWSDRAW_TITLEWIN_PLAYER_SX		( 6 )		// �r�b�g�}�b�v�T�C�YX
#define NEWSDRAW_TITLEWIN_PLAYER_SY		( 5 )		// �r�b�g�}�b�v�T�C�YY
#define NEWSDRAW_TITLEWIN_PLAYER_CGX	( NEWSDRAW_TITLEWIN_TIME_CGX + (NEWSDRAW_TITLEWIN_TIME_SX*NEWSDRAW_TITLEWIN_TIME_SY) )	// CGX
#define NEWSDRAW_TITLEWIN_PLAYER_PAL	( NEWS_PLTT_FONT )	// �p���b�g
#define NEWSDRAW_TITLEWIN_PLAYER_DRAW_X	( 4 )
#define NEWSDRAW_TITLEWIN_PLAYER_DRAW_Y	( 4 )

// �g�s�b�N�\��
#define NEWSDRAW_TITLEWIN_TOPIC_X		( 4 )		// �r�b�g�}�b�vX
#define NEWSDRAW_TITLEWIN_TOPIC_Y		( 12 )		// �r�b�g�}�b�vY
#define NEWSDRAW_TITLEWIN_TOPIC_SX		( 18 )		// �r�b�g�}�b�v�T�C�YX
#define NEWSDRAW_TITLEWIN_TOPIC_SY		( 3 )		// �r�b�g�}�b�v�T�C�YY
#define NEWSDRAW_TITLEWIN_TOPIC_CGX		( NEWSDRAW_TITLEWIN_PLAYER_CGX + (NEWSDRAW_TITLEWIN_PLAYER_SX*NEWSDRAW_TITLEWIN_PLAYER_SY) )	// CGX
#define NEWSDRAW_TITLEWIN_TOPIC_PAL		( NEWS_PLTT_FONT )	// �p���b�g
#define NEWSDRAW_TITLEWIN_TOPIC_DRAW_X	( 4 )
#define NEWSDRAW_TITLEWIN_TOPIC_DRAW_Y	( 4 )

typedef struct{
	u8	x;
	u8	y;
	u8	sizx;
	u8	sizy;
	u16	cgx;
	u8	pal;
	u8	dx:4;
	u8	dy:4;
} NEWSDRAW_BMPDATA;
static const NEWSDRAW_BMPDATA NEWSDRAW_TITLE_BMPDATA[ NEWSDRAW_TITLEWIN_NUM ] = {
	{
		NEWSDRAW_TITLEWIN_APLNAME_X,
		NEWSDRAW_TITLEWIN_APLNAME_Y, 
		NEWSDRAW_TITLEWIN_APLNAME_SX, 
		NEWSDRAW_TITLEWIN_APLNAME_SY,
		NEWSDRAW_TITLEWIN_APLNAME_CGX,
		NEWSDRAW_TITLEWIN_APLNAME_PAL,
		NEWSDRAW_TITLEWIN_APLNAME_DRAW_X,NEWSDRAW_TITLEWIN_APLNAME_DRAW_Y
	},
	{
		NEWSDRAW_TITLEWIN_TIME_X,
		NEWSDRAW_TITLEWIN_TIME_Y, 
		NEWSDRAW_TITLEWIN_TIME_SX, 
		NEWSDRAW_TITLEWIN_TIME_SY,
		NEWSDRAW_TITLEWIN_TIME_CGX,
		NEWSDRAW_TITLEWIN_TIME_PAL,
		NEWSDRAW_TITLEWIN_TIME_DRAW_X,NEWSDRAW_TITLEWIN_TIME_DRAW_Y
	},
	{
		NEWSDRAW_TITLEWIN_PLAYER_X,
		NEWSDRAW_TITLEWIN_PLAYER_Y, 
		NEWSDRAW_TITLEWIN_PLAYER_SX, 
		NEWSDRAW_TITLEWIN_PLAYER_SY,
		NEWSDRAW_TITLEWIN_PLAYER_CGX,
		NEWSDRAW_TITLEWIN_PLAYER_PAL,
		NEWSDRAW_TITLEWIN_PLAYER_DRAW_X, NEWSDRAW_TITLEWIN_PLAYER_DRAW_Y
	},
	{
		NEWSDRAW_TITLEWIN_TOPIC_X,
		NEWSDRAW_TITLEWIN_TOPIC_Y, 
		NEWSDRAW_TITLEWIN_TOPIC_SX, 
		NEWSDRAW_TITLEWIN_TOPIC_SY,
		NEWSDRAW_TITLEWIN_TOPIC_CGX,
		NEWSDRAW_TITLEWIN_TOPIC_PAL,
		NEWSDRAW_TITLEWIN_TOPIC_DRAW_X, NEWSDRAW_TITLEWIN_TOPIC_DRAW_Y
	},
};
// �p���b�g�I�t�Z�b�g
static u16 NEWSDRAW_TITLEWIN_PLTT_OFFS[ NEWSDRAW_TITLEWIN_NUM ] = {
	0, 0xc*2, 0xd*2, 0xf*2
};
// ��
static u32 NEWSDRAW_TITLEWIN_SND_TBL[ NEWSDRAW_TITLEWIN_NUM ] = {
	0, NEWSDRAW_NEWTIME, NEWSDRAW_NEWPLAYER, NEWSDRAW_NEWTOPIC
};
#define NEWSDRAW_TITLEWIN_EFFECT_COUNT		( 28 )	// �p���b�g�t�F�[�h�̃V���N��
#define NEWSDRAW_TITLEWIN_EFFECT_COUNT_HF	( 2 )	// �p���b�g�t�F�[�h�̖��邭�Ȃ�܂ł̃V���N��
#define NEWSDRAW_TITLEWIN_EFFECT_COUNT_WT	( 12 )	// �E�G�C�g
#define NEWSDRAW_TITLEWIN_EFFECT_COUNT_HF2	( NEWSDRAW_TITLEWIN_EFFECT_COUNT - (NEWSDRAW_TITLEWIN_EFFECT_COUNT_HF+NEWSDRAW_TITLEWIN_EFFECT_COUNT_WT) )	// ������V���N��
#define NEWSDRAW_TITLEWIN_EFFECT_COL		( 0x000e )	// ��{�p���b�g�J���[
#define NEWSDRAW_TITLEWIN_EFFECT_COL_END	( 0x0019 )	// �ύX��
#define NEWSDRAW_TITLEWIN_EFFECT_SND_PLY	( 0 )	// ����炷�^�C�~���O


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�O���t�B�b�N�Z�b�g
//=====================================
typedef struct {
	void*				p_buff;
	NNSG2dScreenData*	p_scrn;
} NEWSDRAW_SCRNSET;


//-------------------------------------
///	���ԃE�B���h�E
//=====================================
typedef struct {
	// �f�[�^
	WFLBY_TIME	last_rest_time;	// �ȑO�X�V�����c�莞��
	u8			effect_flag;	// ���U�C�N�G�t�F�N�g�t���O
	u8			updata;			// �X�V�t���O
	u16			count;			// �J�E���^
	u32			change_second;	// �u���b�N�`���ύX����Ԋu�i�b�j
	

	// �`��f�[�^
} NEWSDRAW_TIMEWIN;

//-------------------------------------
///	�v���C���[�E�B���h�E
//=====================================
typedef struct {
	// �f�[�^
	s16 count;
	u16 seq;

	// �`��f�[�^
} NEWSDRAW_PLAYERWIN;

//-------------------------------------
///	�g�s�b�N�@�g���[�i�J���[�f�[�^
//=====================================
typedef struct {
	u8	trcol[ WFLBY_MINIGAME_MAX ];
} NEWSDRAW_TOPIC_TRCOL;


//-------------------------------------
///	�g�s�b�N�X�E�B���h�E
//=====================================
// 1�g�s�b�N�X�f�[�^
typedef struct {
	// �f�[�^	
	u8				move;
	u8				pal;	
	u16				count;
	u16				count_max;
	u16				str_siz;
	STRBUF*			p_str;
	GF_BGL_BMPWIN	bmp;

	// �g�s�b�N�J���[�f�[�^
	NEWSDRAW_TOPIC_TRCOL trcol;
} NEWSDRAW_TOPIC;
typedef struct {
	// �f�[�^
	NEWSDRAW_TOPIC			topic[NEWSDRAW_TOPIC_NUM];	// �g�s�b�N�f�[�^

	// �`��f�[�^
	GF_BGL_BMPWIN	bmp[NEWSDRAW_TOPIC_NUM];
	STRBUF*			p_str;

	//  �p���b�g�f�[�^
	void*				p_plttbuff;
	NNSG2dPaletteData*	p_pltt;
} NEWSDRAW_TOPICWIN;


//-------------------------------------
///	�^�C�g�����[�N
//=====================================
// �G�t�F�N�g���[�N
typedef struct {
	u16		on;			// �G�t�F�N�g��Ԃ�
	u16		col_count;	// �J�E���^
	u16		buff;		// �p���b�g�o�b�t�@
	u16		offs;		// �p���b�g�W�J�I�t�Z�b�g
	u32		sound;		// ��
} NEWSDRAW_TITLEEFF;
// ���[�N
typedef struct {
	GF_BGL_BMPWIN		bmp[ NEWSDRAW_TITLEWIN_NUM ];
	NEWSDRAW_TITLEEFF	eff[ NEWSDRAW_TITLEWIN_NUM ];	// �G�t�F�N�g���[�N
} NEWSDRAW_TITLEWIN;




//-------------------------------------
///	�\���V�X�e��
//=====================================
typedef struct {
	// BG
	GF_BGL_INI*				p_bgl;

	// OAM
    CLACT_SET_PTR           p_clactset;		// �Z���A�N�^�[�Z�b�g
    CLACT_U_EASYRENDER_DATA renddata;       // �ȈՃ����_�[�f�[�^
    CLACT_U_RES_MANAGER_PTR p_resman[NEWSDRAW_RESMAN_NUM]; // �L�����E�p���b�g���\�[�X�}�l�[�W��

	// �A�[�J�C�u
	ARCHANDLE* p_handle;
} NEWSDRAW_DRAWSYS;


//-------------------------------------
///	���[�N
//=====================================
typedef struct {
	// �`��f�[�^
	NEWS_DATA*			p_data;
	const WFLBY_TIME*	cp_nowtime;

	// �`��V�X�e��
	NEWSDRAW_DRAWSYS	draw;

	// �O���t�B�b�N�X�N���[���Z�b�g
	NEWSDRAW_SCRNSET	scrn;

	// ���ԃE�B���h�E
	NEWSDRAW_TIMEWIN	time;

	// �v���C���[�E�B���h�E
	NEWSDRAW_PLAYERWIN	player;

	// �g�s�b�N�E�B���h�E
	NEWSDRAW_TOPICWIN	topic;

	// �^�C�g���E�B���h�E
	NEWSDRAW_TITLEWIN	title;

} NEWSDRAW_WK;



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

// �S��
static void NEWSDRAW_WkVBlank( void* p_work );
static void NEWSDRAW_WkDraw( NEWSDRAW_WK* p_wk, const WFLBY_SYSTEM* cp_system );



// �`��V�X�e��
static void NEWSDRAW_DrawSysInit( NEWSDRAW_DRAWSYS* p_wk, u32 heapID );
static void NEWSDRAW_DrawSysExit( NEWSDRAW_DRAWSYS* p_wk );
static void NEWSDRAW_DrawSysDraw( NEWSDRAW_DRAWSYS* p_wk );
static void NEWSDRAW_DrawSysVBlank( NEWSDRAW_DRAWSYS* p_wk );
static void NEWSDRAW_DrawSysBgInit( NEWSDRAW_DRAWSYS* p_wk, u32 heapID );
static void NEWSDRAW_DrawSysBgExit( NEWSDRAW_DRAWSYS* p_wk );
static void NEWSDRAW_DrawSysOamInit( NEWSDRAW_DRAWSYS* p_wk, u32 heapID );
static void NEWSDRAW_DrawSysOamExit( NEWSDRAW_DRAWSYS* p_wk );

// �X�N���[���Z�b�g
static void NEWSDRAW_ScrnSetInit( NEWSDRAW_SCRNSET* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 heapID );
static void NEWSDRAW_ScrnSetExit( NEWSDRAW_SCRNSET* p_wk );
static void NEWSDRAW_ScrnWriteTimeBlock( NEWSDRAW_SCRNSET* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 block_type, u8 x, u8 y );
static void NEWSDRAW_ScrnWritePlayer( NEWSDRAW_SCRNSET* p_wk, NEWSDRAW_DRAWSYS* p_draw, u8 x, u8 y, u32 player_type, u32 sex, BOOL mydata, BOOL olddata, BOOL vip );

// ���ԃE�B���h�E
static void NEWSDRAW_TimeWinInit( NEWSDRAW_TIMEWIN* p_wk, NEWSDRAW_DRAWSYS* p_draw, NEWSDRAW_SCRNSET* p_scrn, const NEWS_DATA* cp_data, const WFLBY_TIME* cp_nowtime, const WFLBY_SYSTEM* cp_system, u32 heapID );
static void NEWSDRAW_TimeWinExit( NEWSDRAW_TIMEWIN* p_wk );
static BOOL NEWSDRAW_TimeWinMain( NEWSDRAW_TIMEWIN* p_wk, const NEWS_DATA* cp_data, const WFLBY_TIME* cp_nowtime );
static void NEWSDRAW_TimeWinDraw( NEWSDRAW_TIMEWIN* p_wk, NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw );
static void NEWSDRAW_TimeWinDrawMosaic( NEWSDRAW_TIMEWIN* p_wk, NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw );
static void NEWSDRAW_TimeWinDrawUpdate( NEWSDRAW_TIMEWIN* p_wk, NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw );

// �v���C���[�E�B���h�E
static void NEWSDRAW_PlayerWinInit( NEWSDRAW_PLAYERWIN* p_wk, NEWSDRAW_DRAWSYS* p_draw, NEWSDRAW_SCRNSET* p_scrn, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system, u32 heapID );
static void NEWSDRAW_PlayerWinExit( NEWSDRAW_PLAYERWIN* p_wk );
static BOOL NEWSDRAW_PlayerWinMain( NEWSDRAW_PLAYERWIN* p_wk, const NEWS_DATA* cp_data );
static void NEWSDRAW_PlayerWinDraw( NEWSDRAW_PLAYERWIN* p_wk, NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system );
static void NEWSDRAW_PlayerWinWrite( NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system, u32 player_type );
static void NEWSDRAW_PlayerWinInsideAnim( NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system, u32 count );
static void NEWSDRAW_PlayerWinOutsideAnim( NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system, u32 count );


// �g�s�b�N�f�[�^����g�s�b�N�J���[�f�[�^���擾����
static void NEWSDRAW_TopicTrColDataGet( NEWSDRAW_TOPIC_TRCOL* p_wk, const NEWS_DATA* cp_data, NEWS_TOPICTYPE type, const WFLBY_SYSTEM* cp_system );
static void NEWSDRAW_TopicTrColDataTrans( const NEWSDRAW_TOPIC_TRCOL* cp_wk, const NNSG2dPaletteData* cp_pltt, u32 plttidx );

// �g�s�b�N�X�E�B���h�E
static void NEWSDRAW_TopicWinInit( NEWSDRAW_TOPICWIN* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 heapID );
static void NEWSDRAW_TopicWinExit( NEWSDRAW_TOPICWIN* p_wk );
static BOOL NEWSDRAW_TopicWinMain( NEWSDRAW_TOPICWIN* p_wk, NEWS_DATA* p_data, const WFLBY_SYSTEM* cp_system, u32 heapID );
static void NEWSDRAW_TopicWinDraw( NEWSDRAW_TOPICWIN* p_wk );
static void NEWSDRAW_TopicInit( NEWSDRAW_TOPIC* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 idx, u32 heapID );
static void NEWSDRAW_TopicExit( NEWSDRAW_TOPIC* p_wk );
static void NEWSDRAW_TopicStart( NEWSDRAW_TOPIC* p_wk, const STRBUF* cp_str, u32 speed, const NEWSDRAW_TOPIC_TRCOL* cp_trcol, const NNSG2dPaletteData* cp_pltt );
static void NEWSDRAW_TopicEnd( NEWSDRAW_TOPIC* p_wk );
static BOOL NEWSDRAW_TopicMain( NEWSDRAW_TOPIC* p_wk );
static void NEWSDRAW_TopicDraw( const NEWSDRAW_TOPIC* cp_wk, GF_BGL_BMPWIN* p_bmp );


// �^�C�g���E�B���h�E
static void NEWSDRAW_TitleWinInit( NEWSDRAW_TITLEWIN* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 heapID );
static void NEWSDRAW_TitleWinExit( NEWSDRAW_TITLEWIN* p_wk );
static void NEWSDRAW_TitleWinDraw( NEWSDRAW_TITLEWIN* p_wk );
static void NEWSDRAW_TitleWinEffectStart( NEWSDRAW_TITLEWIN* p_wk, u32 title_no );
static void NEWSDRAW_TitleEffInit( NEWSDRAW_TITLEEFF* p_wk, u16 offs, u32 snd );
static void NEWSDRAW_TitleEffExit( NEWSDRAW_TITLEEFF* p_wk );
static void NEWSDRAW_TitleEffStart( NEWSDRAW_TITLEEFF* p_wk );
static void NEWSDRAW_TitleEffMain( NEWSDRAW_TITLEEFF* p_wk );
static void NEWSDRAW_TitleEffDraw( NEWSDRAW_TITLEEFF* p_wk );



#ifdef WFLBY_DEBUG_LOBBY_NEWS_TOPIC_MAKE
static void NEWSDRAW_DEBUG_TopicMake( NEWSDRAW_WK* p_wk, WFLBY_SYSTEM* p_system, u32 heapID );
#endif



//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�j���[�X	������
 *
 *	@param	p_proc		�v���b�N���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT NEWS_DRAW_Init(PROC* p_proc, int* p_seq)
{
	NEWSDRAW_WK* p_wk;
	NEWS_DRAW_DATA* p_param;
	
	p_param = PROC_GetParentWork( p_proc );

	//�q�[�v�G���A�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_NEWSDRAW, 0x50000 );

	// ���[�N�쐬
	p_wk = PROC_AllocWork( p_proc, sizeof(NEWSDRAW_WK), HEAPID_NEWSDRAW );
	memset( p_wk, 0, sizeof(NEWSDRAW_WK) );

	// �f�[�^�i�[
	p_wk->p_data		= p_param->p_data;
	p_wk->cp_nowtime	= p_param->cp_nowtime;

	// �\���V�X�e��������
	NEWSDRAW_DrawSysInit( &p_wk->draw, HEAPID_NEWSDRAW );

	// �X�N���[���Z�b�g������
	NEWSDRAW_ScrnSetInit( &p_wk->scrn, &p_wk->draw, HEAPID_NEWSDRAW );

	// ���ԃE�B���h�E������
	NEWSDRAW_TimeWinInit( &p_wk->time, &p_wk->draw, &p_wk->scrn, p_wk->p_data, p_wk->cp_nowtime, p_param->p_system, HEAPID_NEWSDRAW );

	// �v���C���[�E�B���h�E������
	NEWSDRAW_PlayerWinInit( &p_wk->player, &p_wk->draw, &p_wk->scrn, p_wk->p_data, p_param->p_system, HEAPID_NEWSDRAW );

	// �g�s�b�N�E�B���h�E������
	NEWSDRAW_TopicWinInit( &p_wk->topic, &p_wk->draw, HEAPID_NEWSDRAW );

	// �^�C�g���쐬
	NEWSDRAW_TitleWinInit( &p_wk->title, &p_wk->draw, HEAPID_NEWSDRAW );

	// ���荞�ݐݒ�
	sys_VBlankFuncChange( NEWSDRAW_WkVBlank, p_wk );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	
	return PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�j���[�X	���C��
 */
//-----------------------------------------------------------------------------
PROC_RESULT NEWS_DRAW_Main(PROC* p_proc, int* p_seq)
{
	NEWSDRAW_WK* p_wk;
	NEWS_DRAW_DATA* p_param;
	BOOL result;
	
	p_wk	= PROC_GetWork( p_proc );
	p_param = PROC_GetParentWork( p_proc );

#ifdef WFLBY_DEBUG_ROOM_WLDTIMER_AUTO
	WFLBY_DEBUG_ROOM_WFLBY_TIMER_AUTO = TRUE;
	sys.trg		|= PAD_BUTTON_B;
	sys.cont	|= PAD_BUTTON_B;
#endif

	switch( *p_seq ){
	case NEWSDRAW_SEQ_FADEIN:
		WIPE_SYS_Start(WIPE_PATTERN_WMS, 
				WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_NEWSDRAW );

		// BGM���t�F�[�h�A�E�g
		WFLBY_SYSTEM_SetBGMVolumeDown( p_param->p_system, TRUE );

		// ���r�[�ɓ����Ă��������o��
//		Snd_SePlay( WFLBY_SND_NEWSIN );

		(*p_seq) ++;
		break;
		
	case NEWSDRAW_SEQ_FADEINWAIT:
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){
			(*p_seq) ++;
		}
		break;
		
	case NEWSDRAW_SEQ_MAIN:

		if( (sys.trg & PAD_BUTTON_B) || 
			(WFLBY_SYSTEM_Event_GetEndCM( p_param->p_system ) == TRUE) ||
			(WFLBY_ERR_CheckError() == TRUE) ){
			if( (WFLBY_SYSTEM_Event_GetEndCM( p_param->p_system ) == TRUE) ){
				WFLBY_SYSTEM_APLFLAG_SetForceEnd( p_param->p_system );
			}
			(*p_seq) = NEWSDRAW_SEQ_FADEOUT;
		}else{

#ifdef WFLBY_DEBUG_LOBBY_NEWS_TOPIC_MAKE
			if( sys.trg & PAD_BUTTON_R ){
				NEWSDRAW_DEBUG_TopicMake( p_wk, p_param->p_system, HEAPID_NEWSDRAW );
			}
#endif
			
			

			// ���ԃE�B���h�E����
			result = NEWSDRAW_TimeWinMain( &p_wk->time, p_wk->p_data, p_wk->cp_nowtime );
			if( result == TRUE ){
				NEWSDRAW_TitleWinEffectStart( &p_wk->title, NEWSDRAW_TITLEWIN_TIME );
			}

			// �v���C���[�E�B���h�E����
			result = NEWSDRAW_PlayerWinMain( &p_wk->player, p_wk->p_data );
			if( result == TRUE ){
				NEWSDRAW_TitleWinEffectStart( &p_wk->title, NEWSDRAW_TITLEWIN_PLAYER );
			}

			// �g�s�b�N�E�B���h�E����
			result = NEWSDRAW_TopicWinMain( &p_wk->topic, p_wk->p_data, p_param->p_system, HEAPID_NEWSDRAW );
			if( result == TRUE ){
				NEWSDRAW_TitleWinEffectStart( &p_wk->title, NEWSDRAW_TITLEWIN_TOPIC );
			}

		}
		break;
		
	case NEWSDRAW_SEQ_FADEOUT:
		WIPE_SYS_Start(WIPE_PATTERN_WMS, 
				WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_NEWSDRAW );
		(*p_seq) ++;
		break;
		
	case NEWSDRAW_SEQ_FADEOUTWAIT:
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){
			return PROC_RES_FINISH;
		}
		break;
	}


	// �`��
	NEWSDRAW_WkDraw( p_wk, p_param->p_system );

	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�j���[�X	�j��
 */
//-----------------------------------------------------------------------------
PROC_RESULT NEWS_DRAW_Exit(PROC* p_proc, int* p_seq)
{
	NEWSDRAW_WK* p_wk;
	NEWS_DRAW_DATA* p_param;
	
	p_wk	= PROC_GetWork( p_proc );
	p_param = PROC_GetParentWork( p_proc );

	// �^�C�g���j��
	NEWSDRAW_TitleWinExit( &p_wk->title );

	// ���ԃE�B���h�E�j��
	NEWSDRAW_TimeWinExit( &p_wk->time );

	// �v���C���[�E�B���h�E�j��
	NEWSDRAW_PlayerWinExit( &p_wk->player );

	// �g�s�b�N�E�B���h�E�j��
	NEWSDRAW_TopicWinExit( &p_wk->topic );

	// �X�N���[���Z�b�g�j��
	NEWSDRAW_ScrnSetExit( &p_wk->scrn );

	// ���荞�ݐݒ�
	sys_VBlankFuncChange( NULL, NULL );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~


	// �`��V�X�e���j��
	NEWSDRAW_DrawSysExit( &p_wk->draw );

	//���[�N�j��
	PROC_FreeWork( p_proc );
	
	//�q�[�v�j��
	sys_DeleteHeap( HEAPID_NEWSDRAW );

	return PROC_RES_FINISH;
}





//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	VBLANK����
 *
 *	@param	p_work 
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_WkVBlank( void* p_work )
{
	NEWSDRAW_WK* p_wk = p_work;

	NEWSDRAW_DrawSysVBlank( &p_wk->draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_system	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_WkDraw( NEWSDRAW_WK* p_wk, const WFLBY_SYSTEM* cp_system )
{
	// ���ԃE�B���h�E�\������
	NEWSDRAW_TimeWinDraw( &p_wk->time, &p_wk->scrn, &p_wk->draw );

	// �v���C���[�E�B���h�E�\������
	NEWSDRAW_PlayerWinDraw( &p_wk->player, &p_wk->scrn, &p_wk->draw, p_wk->p_data, cp_system );

	// �g�s�b�N�̕`��
	NEWSDRAW_TopicWinDraw( &p_wk->topic );

	// �^�C�g���E�B���h�E����
	NEWSDRAW_TitleWinDraw( &p_wk->title );
	
	// �`��V�X�e���`��
	NEWSDRAW_DrawSysDraw( &p_wk->draw );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�\�����̏�����
 *
 *	@param	p_wk		�\���V�X�e�����[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_DrawSysInit( NEWSDRAW_DRAWSYS* p_wk, u32 heapID )
{
	// �A�[�J�C�u�n���h��
	p_wk->p_handle = ArchiveDataHandleOpen( ARC_LOBBY_NEWS, heapID );

	// Vram�]���}�l�[�W���쐬
	initVramTransferManagerHeap( NEWSDRAW_VRAMTRANS_TASKNUM, heapID );
	
	// �o���N�ݒ�
	GF_Disp_SetBank( &sc_NEWSDRAW_BANK );

	// BG�ݒ�
	NEWSDRAW_DrawSysBgInit( p_wk, heapID );

	// OAM�ݒ�
	NEWSDRAW_DrawSysOamInit( p_wk, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\�����̔j��
 *
 *	@param	p_wk		�\���V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_DrawSysExit( NEWSDRAW_DRAWSYS* p_wk )
{
	// �A�[�J�C�u�n���h��
	ArchiveDataHandleClose( p_wk->p_handle );

	// Vram�]���}�l�[�W���j��
	DellVramTransferManager();
	
	// BG�ݒ�
	NEWSDRAW_DrawSysBgExit( p_wk );

	// OAM�ݒ�
	NEWSDRAW_DrawSysOamExit( p_wk );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�揈��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_DrawSysDraw( NEWSDRAW_DRAWSYS* p_wk )
{
	CLACT_Draw( p_wk->p_clactset );
}

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_DrawSysVBlank( NEWSDRAW_DRAWSYS* p_wk )
{
    // BG��������
    GF_BGL_VBlankFunc( p_wk->p_bgl );

    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();

	// Vram�]��
	DoVramTransferManager();
}

// BG
static void NEWSDRAW_DrawSysBgInit( NEWSDRAW_DRAWSYS* p_wk, u32 heapID )
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

		for( i=0; i<NEWSDRAW_BGCNT_NUM; i++ ){
			GF_BGL_BGControlSet( p_wk->p_bgl, 
					sc_NEWSDRAW_BGCNT_FRM[i], &sc_NEWSDRAW_BGCNT_DATA[i],
					GF_BGL_MODE_TEXT );
			GF_BGL_ClearCharSet( sc_NEWSDRAW_BGCNT_FRM[i], 32, 0, heapID);
			GF_BGL_ScrClear( p_wk->p_bgl, sc_NEWSDRAW_BGCNT_FRM[i] );
		}
	}

	// BG�ݒ�
	{
		// �p���b�g
		ArcUtil_HDL_PalSet( p_wk->p_handle, NARC_lobby_news_lobby_news_bg_NCLR, 
				PALTYPE_MAIN_BG, 0, 0, heapID );

		//  �t�H���g�J���[�ݒ�
		TalkFontPaletteLoad( PALTYPE_MAIN_BG, NEWS_PLTT_TALKFONT*32, heapID );

		// �L�����N�^
		ArcUtil_HDL_BgCharSet( p_wk->p_handle,
				NARC_lobby_news_lobby_news_bg_NCGR, 
				p_wk->p_bgl, GF_BGL_FRAME1_M, 0, 0, FALSE, heapID );

		// �X�N���[��
		ArcUtil_HDL_ScrnSet(p_wk->p_handle, 
				NARC_lobby_news_lobby_news_bg1_NSCR, 
				p_wk->p_bgl, GF_BGL_FRAME1_M, 0, 0, FALSE, heapID);
		ArcUtil_HDL_ScrnSet(p_wk->p_handle, 
				NARC_lobby_news_lobby_news_bg2_NSCR, 
				p_wk->p_bgl, GF_BGL_FRAME2_M, 0, 0, FALSE, heapID);

	}

	// �T�u��ʐݒ�
	{
		// �p���b�g
		ArcUtil_HDL_PalSet( p_wk->p_handle, NARC_lobby_news_lobby_news_bg_NCLR, 
				PALTYPE_SUB_BG, 0, 0, heapID );

		// �L�����N�^
		ArcUtil_HDL_BgCharSet( p_wk->p_handle,
				NARC_lobby_news_wifi_mark_bg_NCGR, 
				p_wk->p_bgl, GF_BGL_FRAME0_S, 0, 0, FALSE, heapID );

		// �X�N���[��
		ArcUtil_HDL_ScrnSet(p_wk->p_handle, 
				NARC_lobby_news_wifi_mark_bg_NSCR, 
				p_wk->p_bgl, GF_BGL_FRAME0_S, 0, 0, FALSE, heapID);
	}

	// ��{�L�����N�^�p���b�g�t���[��
	// �o�b�N�O���E���h�J���[�ݒ�
//	GF_BGL_BackGroundColorSet( GF_BGL_FRAME0_M, GX_RGB( 0,0,0 ) );
}
static void NEWSDRAW_DrawSysBgExit( NEWSDRAW_DRAWSYS* p_wk )
{
	// �a�f�R���g���[���j��
	{
		int i;

		for( i=0; i<NEWSDRAW_BGCNT_NUM; i++ ){
			GF_BGL_BGControlExit( p_wk->p_bgl, sc_NEWSDRAW_BGCNT_FRM[i] );
		}
	}
	
	// BGL�j��
	sys_FreeMemoryEz( p_wk->p_bgl );

	// ���C���ƃT�u�����ɖ߂�
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();
}

// OAM
static void NEWSDRAW_DrawSysOamInit( NEWSDRAW_DRAWSYS* p_wk, u32 heapID )
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
    InitCharManagerReg(&sc_NEWSDRAW_CHARMAN_INIT, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K );
    // �p���b�g�}�l�[�W���[������
    InitPlttManager(NEWSDRAW_OAM_CONTNUM, heapID);

    // �ǂݍ��݊J�n�ʒu��������
    CharLoadStartAll();
    PlttLoadStartAll();

    //�ʐM�A�C�R���p�ɃL�������p���b�g����
    CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
    

    // �Z���A�N�^�[�Z�b�g�쐬
    p_wk->p_clactset = CLACT_U_SetEasyInit( NEWSDRAW_OAM_CONTNUM, &p_wk->renddata, heapID );

    // �T�[�t�F�[�X�ʒu���ړ�������
    CLACT_U_SetSubSurfaceMatrix( &p_wk->renddata, 0, NEWSDRAW_SF_MAT_Y );
    
    // �L�����ƃp���b�g�̃��\�[�X�}�l�[�W���쐬
    for( i=0; i<NEWSDRAW_RESMAN_NUM; i++ ){
        p_wk->p_resman[i] = CLACT_U_ResManagerInit(NEWSDRAW_OAM_CONTNUM, i, heapID);
    }

	// ����ʂɒʐM�A�C�R�����o��
	WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��

	// �\���J�n
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
}
static void NEWSDRAW_DrawSysOamExit( NEWSDRAW_DRAWSYS* p_wk )
{
    int i;

    // �A�N�^�[�̔j��
    CLACT_DestSet( p_wk->p_clactset );

    for( i=0; i<NEWSDRAW_RESMAN_NUM; i++ ){
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
 *	@brief	�X�N���[���Z�b�g������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`�惏�[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_ScrnSetInit( NEWSDRAW_SCRNSET* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 heapID )
{
	p_wk->p_buff = ArcUtil_HDL_ScrnDataGet( p_draw->p_handle, 
			NARC_lobby_news_lobby_news_chara_NSCR,
			FALSE, &p_wk->p_scrn, heapID);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���Z�b�g�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_ScrnSetExit( NEWSDRAW_SCRNSET* p_wk )
{
	sys_FreeMemoryEz( p_wk->p_buff );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���u���b�N����������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	block_type	�u���b�N�^�C�v
 *	@param	x			X�u���b�N�ʒu{0�`NEWSDRAW_TIME_X_SIZ-1}
 *	@param	y 			Y�u���b�N�ʒu{0�`NEWSDRAW_TIME_Y_SIZ-1}
 *
 *	block_type
 *		NEWSDRAW_TIME_BLOCKTYPE_NORMAL,	// �ʏ�
 *		NEWSDRAW_TIME_BLOCKTYPE_ICON,	// �A�C�R��
 *		NEWSDRAW_TIME_BLOCKTYPE_RED,	// ���b�h
 *		NEWSDRAW_TIME_BLOCKTYPE_NONE,	// �����Ȃ�
 *		NEWSDRAW_TIME_BLOCKTYPE_ICON1,	// �A�C�R���@�A�j���P
 *		NEWSDRAW_TIME_BLOCKTYPE_ICON2,	// �A�C�R���@�A�j���Q
 *		NEWSDRAW_TIME_BLOCKTYPE_ICON3,	// �A�C�R���@�A�j���R
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_ScrnWriteTimeBlock( NEWSDRAW_SCRNSET* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 block_type, u8 x, u8 y )
{
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, GF_BGL_FRAME3_M, 
			NEWSDRAW_TIME_SCRN_DRAWX+(x*NEWSDRAW_TIME_BLOCK_SIZX), 
			NEWSDRAW_TIME_SCRN_DRAWY+(y*NEWSDRAW_TIME_BLOCK_SIZY),
			NEWSDRAW_TIME_BLOCK_SIZX, NEWSDRAW_TIME_BLOCK_SIZY,
			p_wk->p_scrn->rawData,
			NEWSDRAW_SCRN_TIMEBLOCK_X + block_type, 
			NEWSDRAW_SCRN_TIMEBLOCK_Y,
			p_wk->p_scrn->screenWidth/8,
			p_wk->p_scrn->screenHeight/8 );

	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, GF_BGL_FRAME3_M );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�f�[�^�\��
 *
 *	@param	p_wk			���[�N
 *	@param	p_draw			�`��V�X�e��
 *	@param	x				���`��ʒu
 *	@param	y				���`��ʒu
 *	@param	player_type		�v���C���[�^�C�v
 *	@param	sex				����
 *	@param	mydata			�����̃f�[�^��
 *	@param	olddata			�̂̃f�[�^��
 *	@param	vip				VIP���ǂ���
 *
 *	player_type
			NEWSDRAW_PLAYERTYPE_NML,
			NEWSDRAW_PLAYERTYPE_OUT,
			NEWSDRAW_PLAYERTYPE_IN,
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_ScrnWritePlayer( NEWSDRAW_SCRNSET* p_wk, NEWSDRAW_DRAWSYS* p_draw, u8 x, u8 y, u32 player_type, u32 sex, BOOL mydata, BOOL olddata, BOOL vip )
{
	u32 r_x, r_y;

	r_x = player_type % NEWSDRAW_PLAYER_SCRN_PAT_X;
	r_y = player_type / NEWSDRAW_PLAYER_SCRN_PAT_X;

	// �Â��l�̏����Ă���X�N���[���ʒu�Ɉړ�
	if( olddata ){
		r_y += NEWSDRAW_PLAYER_SCRN_OLDPAT_Y;
	}

	// ���ʕ�������
	if( sex == PM_FEMALE ){
		r_y ++;
	}
	
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, GF_BGL_FRAME3_M, 
			NEWSDRAW_PLAYER_SCRN_DRAWX+(x*NEWSDRAW_PLAYER_BLOCK_SIZX), 
			NEWSDRAW_PLAYER_SCRN_DRAWY+(y*NEWSDRAW_PLAYER_BLOCK_SIZY),
			NEWSDRAW_PLAYER_BLOCK_SIZX, NEWSDRAW_PLAYER_BLOCK_SIZY,
			p_wk->p_scrn->rawData,
			NEWSDRAW_SCRN_PLAYERBLOCK_X + (r_x*NEWSDRAW_PLAYER_BLOCK_SIZX), 
			NEWSDRAW_SCRN_PLAYERBLOCK_Y + (r_y*NEWSDRAW_PLAYER_BLOCK_SIZY),
			p_wk->p_scrn->screenWidth/8,
			p_wk->p_scrn->screenHeight/8 );

	// vip�͐�����
	if( vip ){
		GF_BGL_ScrPalChange( p_draw->p_bgl, GF_BGL_FRAME3_M,
				NEWSDRAW_PLAYER_SCRN_DRAWX+(x*NEWSDRAW_PLAYER_BLOCK_SIZX), 
				NEWSDRAW_PLAYER_SCRN_DRAWY+(y*NEWSDRAW_PLAYER_BLOCK_SIZY),
				NEWSDRAW_PLAYER_BLOCK_SIZX, NEWSDRAW_PLAYER_BLOCK_SIZY,
				NEWS_PLTT_VIP );
	}else{

		// �����̃f�[�^�̓I�����W������
		if( mydata ){
			GF_BGL_ScrPalChange( p_draw->p_bgl, GF_BGL_FRAME3_M,
					NEWSDRAW_PLAYER_SCRN_DRAWX+(x*NEWSDRAW_PLAYER_BLOCK_SIZX), 
					NEWSDRAW_PLAYER_SCRN_DRAWY+(y*NEWSDRAW_PLAYER_BLOCK_SIZY),
					NEWSDRAW_PLAYER_BLOCK_SIZX, NEWSDRAW_PLAYER_BLOCK_SIZY,
					NEWS_PLTT_YOU );
		}
	}

	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, GF_BGL_FRAME3_M );
}





//----------------------------------------------------------------------------
/**
 *	@brief	���ԃE�B���h�E	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	p_scrn		�X�N���[���Z�b�g
 *	@pamra	cp_data		�f�[�^
 *	@param	cp_nowtime	���̎���
 *	@param	cp_system	�V�X�e�����[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TimeWinInit( NEWSDRAW_TIMEWIN* p_wk, NEWSDRAW_DRAWSYS* p_draw, NEWSDRAW_SCRNSET* p_scrn, const NEWS_DATA* cp_data, const WFLBY_TIME* cp_nowtime, const WFLBY_SYSTEM* cp_system, u32 heapID )
{
	u32 closetime;
	u32 closetime_minute;
	
	// �c�莞�ԏ����l
	// ���b�N��̎��ԁ{�P��
	closetime = WFLBY_SYSTEM_GetCloseTime( cp_system );	
	closetime_minute = closetime / 60;	// �b�P�ʂȂ̂ŕ��P�ʂ�
	p_wk->last_rest_time.hour	= 0;
	p_wk->last_rest_time.minute = closetime_minute + 1;
	p_wk->last_rest_time.second	= 0;

	// �u���b�N�̕`�������������Ԋu
	p_wk->change_second = closetime / NEWSDRAW_TIME_BLOCKNUM;	

//	OS_TPrintf( "closetime %d minute %d change %d\n", closetime, closetime_minute, p_wk->change_second );

	// ���U�C�N�A�j���ݒ�ON
	p_wk->effect_flag	= TRUE;
	p_wk->count			= 0;

	// �`��X�V
	p_wk->updata		= FALSE;

	// 1�x���s
	NEWSDRAW_TimeWinMain( p_wk, cp_data, cp_nowtime );
	
	// �`��f�[�^������
	NEWSDRAW_TimeWinDraw( p_wk, p_scrn, p_draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃE�B���h�E	�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TimeWinExit( NEWSDRAW_TIMEWIN* p_wk )
{
	// �`��f�[�^�j��
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃE�B���h�E	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 *	@param	cp_nowtime	���ݎ���
 *
 *	@retval	TRUE	�\���X�V����
 *	@retval	FALSE	�\���X�V���ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL NEWSDRAW_TimeWinMain( NEWSDRAW_TIMEWIN* p_wk, const NEWS_DATA* cp_data, const WFLBY_TIME* cp_nowtime )
{
	BOOL result;
	WFLBY_TIME time;
	WFLBY_TIME rest_time;
	WFLBY_TIME rest_sum;
	BOOL ret = FALSE;
	
	// LOCK���Ă��邩�`�F�b�N
	result = NEWS_DSET_GetLockTime( cp_data, &time );	
	if( result == FALSE ){
		// ���U�C�N�A�j��
		return ret;
	}

	// ���U�C�N�A�j�����Ȃ��~����
	if( p_wk->effect_flag == TRUE ){
		p_wk->effect_flag = FALSE;
		
		// ����������
		p_wk->updata	= TRUE;
		ret				= TRUE;
	}

	// ����LOCK����̌o�߂ŃX�N���[���ɏ�������
	WFLBY_TIME_Sub( cp_nowtime, &time, &rest_time );

	// �c�莞�ԂƂP�O�̎c�莞�Ԃ�p_wk->change_second�b
	// �ȏ�̕�������΁A��������
	WFLBY_TIME_Sub( &rest_time, &p_wk->last_rest_time, &rest_sum );
	if( (rest_sum.second >= p_wk->change_second) ||
		(rest_sum.minute > 0) ||
		(rest_sum.hour	 > 0) ){

		// ����������
		p_wk->updata = TRUE;

		// �������������Ԃ�ۑ�
		p_wk->last_rest_time = rest_time;

		// ����������
		ret = TRUE;
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃE�B���h�E�`�揈��
 *
 *	@param	p_wk	���[�N
 *	@param	p_scrn	�X�N���[���Z�b�g
 *	@param	p_draw	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TimeWinDraw( NEWSDRAW_TIMEWIN* p_wk, NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw )
{
	
	if( p_wk->effect_flag == TRUE ){
		NEWSDRAW_TimeWinDrawMosaic( p_wk, p_scrn, p_draw );
	}else{
		NEWSDRAW_TimeWinDrawUpdate( p_wk, p_scrn, p_draw );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃf�[�^�`��A�b�v�f�[�g	�G�t�F�N�g��
 *
 *	@param	p_wk		���[�N
 *	@param	p_scrn		�X�N���[��
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TimeWinDrawMosaic( NEWSDRAW_TIMEWIN* p_wk, NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw )
{
	p_wk->count ++;
	if( NEWSDRAW_TIME_EFFECT_COUNT < p_wk->count ){
		p_wk->count = 0;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃf�[�^�`��A�b�v�f�[�g
 *
 *	@param	p_wk		���[�N
 *	@param	p_scrn		�X�N���[��
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TimeWinDrawUpdate( NEWSDRAW_TIMEWIN* p_wk, NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw )
{
	int i, j;
	u32 block;
	u16 block_type;
	s32 x, y;
	s32 block_num;

	block = p_wk->last_rest_time.second;
	block += p_wk->last_rest_time.minute * 60;
	block += p_wk->last_rest_time.hour * 3600;

	// �\���u���b�N��
	block /= p_wk->change_second;

	// �A�b�v�f�[�g�t���O�ŏ���������
	if( p_wk->updata ){

		// �J�E���^������
		p_wk->count = 0;

		p_wk->updata = FALSE;

		// �X�N���[���̈��Clean
		GF_BGL_ScrFill( p_draw->p_bgl, GF_BGL_FRAME3_M, 0,
				NEWSDRAW_TIME_SCRN_DRAWX, NEWSDRAW_TIME_SCRN_DRAWY,
				NEWSDRAW_TIME_X_SIZ*NEWSDRAW_TIME_BLOCK_SIZX,
				NEWSDRAW_TIME_Y_SIZ*NEWSDRAW_TIME_BLOCK_SIZY,
				0 );
		GF_BGL_LoadScreenV_Req( p_draw->p_bgl, GF_BGL_FRAME3_M );
		
		
		// �c�莞�ԂŃX�N���[����\������
		for( i=0; i<NEWSDRAW_TIME_Y_SIZ; i++ ){
			
			y = (NEWSDRAW_TIME_Y_SIZ-1)-i;
			
			for( j=0; j<NEWSDRAW_TIME_X_SIZ; j++ ){
			
				x = (NEWSDRAW_TIME_X_SIZ-1)-j;	

				block_num = ((y*NEWSDRAW_TIME_X_SIZ)+x);
				
				if( block_num >= block ){
					if( block_num == block ){
						block_type = NEWSDRAW_TIME_BLOCKTYPE_ICON;
//						OS_Printf( "icon first x[%d] y[%d] block[%d]\n", x, y, block );
					}else if( y==(NEWSDRAW_TIME_Y_SIZ-1) ){
						block_type = NEWSDRAW_TIME_BLOCKTYPE_RED;
					}else{
						block_type = NEWSDRAW_TIME_BLOCKTYPE_NORMAL;
					}
					// ��������
					NEWSDRAW_ScrnWriteTimeBlock( p_scrn, p_draw, block_type, 
							x, y );
				}
			}
		}
		
	}else{

		// BLOCK�l�̃I�[�o�[�`�F�b�N
		if( block < NEWSDRAW_TIME_BLOCKNUM ){
			u32 eff_count;
			u32 icon_anm;
			u32 icon_one_time;
			
			// �G�t�F�N�g�J�E���g����
			p_wk->count ++;

			// �G�t�F�N�g�J�E���g�l�v�Z
			eff_count = p_wk->count % NEWSDRAW_TIME_TIME_EFFECT_COUNT;

			// ���W
			x = block % NEWSDRAW_TIME_X_SIZ;
			y = block / NEWSDRAW_TIME_X_SIZ;

			// �J�E���g
			if( eff_count == 0 ){
//				OS_Printf( "icon eff x[%d] y[%d] block[%d]\n", x, y, block );

				// icon���P���炷�^�C�~���O	change_second�͕b�P�ʂȂ̂ŕ`��t���[���P�ʂɂ���
				icon_one_time = (p_wk->change_second * 30) / NEWSDRAW_TIME_EFF_DATA_NUM;

				// �������ރA�C�R���̃^�C�v���擾
				icon_anm = NEWSDRAW_TIME_BLOCKTYPE_ICON3;
				for( i=0; i<NEWSDRAW_TIME_EFF_DATA_NUM; i++ ){
					if( icon_one_time*(i+1) > p_wk->count ){
						icon_anm = NEWSDRAW_TIME_EFF_ANM_DATA[i].icon;
						break;
					}
				}
				// ��������
				NEWSDRAW_ScrnWriteTimeBlock( p_scrn, p_draw, 
						icon_anm, x, y );
			}else if( eff_count == (NEWSDRAW_TIME_TIME_EFFECT_COUNT/2) ){
				// ��������
				NEWSDRAW_ScrnWriteTimeBlock( p_scrn, p_draw, 
						NEWSDRAW_TIME_BLOCKTYPE_NONE, x, y );
			}
		}
	}

}





//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�E�B���h�E	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	p_scrn		�X�N���[���f�[�^
 *	@param	cp_data		�`��f�[�^
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_PlayerWinInit( NEWSDRAW_PLAYERWIN* p_wk, NEWSDRAW_DRAWSYS* p_draw, NEWSDRAW_SCRNSET* p_scrn, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system, u32 heapID )
{
	// �`�揉����
	{
		// �����f�[�^�̕`��
		NEWSDRAW_PlayerWinWrite( p_scrn, p_draw, cp_data, cp_system, NEWSDRAW_PLAYERTYPE_NML );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�E�B���h�E	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_PlayerWinExit( NEWSDRAW_PLAYERWIN* p_wk )
{
	// �`��j��
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�E�B���h�E	���C��
 *	
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 *
 *	@retval	TRUE	�X�V����
 *	@retval	FALSE	�X�V���Ȃ�����
 */
//-----------------------------------------------------------------------------
static BOOL NEWSDRAW_PlayerWinMain( NEWSDRAW_PLAYERWIN* p_wk, const NEWS_DATA* cp_data )
{
	u32 event;
	BOOL ret = FALSE;
	
	// �C�x���g���N��������ĕ`�悷��
	event = NEWS_DSET_GetPlayerEvent( cp_data );
	switch( event ){
	case NEWS_ROOMEV_NONE:
		break;
		
	case NEWS_ROOMEV_IN:		// �N�������Ă���
		// �l�������Ă����V�[�P���X��
		p_wk->seq = NEWSDRAW_PLAYERWIN_SEQ_INSIDE;
		ret = TRUE;
		break;
		
	case NEWS_ROOMEV_OUT:	// �N���o�čs����
		// �l���o�Ă����V�[�P���X��
		p_wk->seq = NEWSDRAW_PLAYERWIN_SEQ_OUTSIDE;
		ret = TRUE;
		break;
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�E�B���h�E�@�A�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_scrn		�X�N���[���Z�b�g
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_data		�f�[�^
 *	@param	cp_system	�V�X�e���f�[�^
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_PlayerWinDraw( NEWSDRAW_PLAYERWIN* p_wk, NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system )
{
	switch( p_wk->seq ){
	case NEWSDRAW_PLAYERWIN_SEQ_WAIT:		// �ҋ@���
		break;
		
	case NEWSDRAW_PLAYERWIN_SEQ_INSIDE:		// �l�������Ă���
		p_wk->seq = NEWSDRAW_PLAYERWIN_SEQ_INSIDEMAIN;
		p_wk->count = 0;
		break;
		
	case NEWSDRAW_PLAYERWIN_SEQ_INSIDEMAIN:	// �l�������Ă���
		NEWSDRAW_PlayerWinInsideAnim( p_scrn, p_draw, cp_data, cp_system, p_wk->count );
		p_wk->count ++;
		if( p_wk->count >= NEWSDRAW_PLAYERWIN_COUNT_INSIDE ){
			p_wk->seq = NEWSDRAW_PLAYERWIN_SEQ_WAIT;
		}
		break;
		
	case NEWSDRAW_PLAYERWIN_SEQ_OUTSIDE:		// �l�������Ă���
		p_wk->seq = NEWSDRAW_PLAYERWIN_SEQ_OUTSIDEMAIN;
		p_wk->count = 0;
		break;

	case NEWSDRAW_PLAYERWIN_SEQ_OUTSIDEMAIN:	// �l�������Ă���
		NEWSDRAW_PlayerWinOutsideAnim( p_scrn, p_draw, cp_data, cp_system, p_wk->count );
		p_wk->count ++;
		if( p_wk->count >= NEWSDRAW_PLAYERWIN_COUNT_OUTSIDE ){
			p_wk->seq = NEWSDRAW_PLAYERWIN_SEQ_WAIT;
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l�̊G��\������
 *
 *	@param	p_scrn		�X�N���[�����[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_data		�`��f�[�^
 *	@param	player_type	�\���v���C���[�^�C�v
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_PlayerWinWrite( NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system, u32 player_type )
{
	int i, j;
	u32 sex;
	BOOL mydata;
	BOOL olddata;
	BOOL newdata;
	BOOL inside;
	BOOL vip;
	u32 player;
	u32 draw_player_type;

	// �X�N���[���̈��Clean
	GF_BGL_ScrFill( p_draw->p_bgl, GF_BGL_FRAME3_M, 0,
			NEWSDRAW_PLAYER_SCRN_DRAWX, NEWSDRAW_PLAYER_SCRN_DRAWY,
			NEWSDRAW_PLAYERWIN_DRAW_X*NEWSDRAW_PLAYER_BLOCK_SIZX,
			NEWSDRAW_PLAYERWIN_DRAW_Y*NEWSDRAW_PLAYER_BLOCK_SIZY,
			0 );

	// �S�v���C���[��\������
	for( i=0; i<NEWSDRAW_PLAYERWIN_DRAW_Y; i++ ){
		for( j=0; j<NEWSDRAW_PLAYERWIN_DRAW_X; j++ ){
			
			player	= (i*NEWSDRAW_PLAYERWIN_DRAW_X)+j;

			inside	= NEWS_DSET_CheckPlayerInside( cp_data, player );
			if( inside ){
				
				// �\���p�����[�^�̎擾
				sex		= NEWS_DSET_GetPlayerSex( cp_data, player );
				mydata	= NEWS_DSET_GetPlayerSpecialMy( cp_data, player );
				olddata	= NEWS_DSET_GetPlayerSpecialOld( cp_data, player );
				newdata	= NEWS_DSET_GetPlayerNewPlayer( cp_data, player );
				vip		= WFLBY_SYSTEM_GetUserVipFlag( cp_system, player );

				// �V���������Ă����l�̕\���̏ꍇ�͏�ɒʏ�t���[����\������
				if( (player_type == NEWSDRAW_PLAYERTYPE_IN) && (newdata == TRUE) ){
					draw_player_type = NEWSDRAW_PLAYERTYPE_NML;
				}else{
					draw_player_type = player_type;
				}

				// �\��
				NEWSDRAW_ScrnWritePlayer( p_scrn, p_draw,
						j, i, draw_player_type, sex, mydata, olddata, vip );
			}
		}
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�C���T�C�h�A�j���`��
 *
 *	@param	p_scrn		�X�N���[�����[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_data		�f�[�^
 *	@param	cp_system	�V�X�e�����[�N
 *	@param	count		�J�E���^
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_PlayerWinInsideAnim( NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system, u32 count )
{	
	u32 player_type;
	
	// �G�ύX�^�C�~���O�`�F�b�N
	if( (count % NEWSDRAW_PLAYERWIN_INSIDE_ONEFRAME) == 0 ){

		player_type = count / NEWSDRAW_PLAYERWIN_INSIDE_ONEFRAME;
		if( (player_type % 2) == 0 ){
			player_type = NEWSDRAW_PLAYERTYPE_IN;
		}else{
			player_type = NEWSDRAW_PLAYERTYPE_NML;
		}
		NEWSDRAW_PlayerWinWrite( p_scrn, p_draw, cp_data, cp_system, player_type );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�A�E�g�T�C�h�A�j���`��
 *
 *	@param	p_scrn		�X�N���[�����[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_data		�f�[�^
 *	@param	cp_system	�V�X�e�����[�N
 *	@param	count		�J�E���^
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_PlayerWinOutsideAnim( NEWSDRAW_SCRNSET* p_scrn, NEWSDRAW_DRAWSYS* p_draw, const NEWS_DATA* cp_data, const WFLBY_SYSTEM* cp_system, u32 count )
{
	u32 player_type;
	
	// �G�ύX�^�C�~���O�`�F�b�N
	if( (count % NEWSDRAW_PLAYERWIN_INSIDE_ONEFRAME) == 0 ){

		player_type = count / NEWSDRAW_PLAYERWIN_OUTSIDE_ONEFRAME;
		if( (player_type % 2) == 0 ){
			player_type = NEWSDRAW_PLAYERTYPE_OUT;
		}else{
			player_type = NEWSDRAW_PLAYERTYPE_NML;
		}
		NEWSDRAW_PlayerWinWrite( p_scrn, p_draw, cp_data, cp_system, player_type );
	}
}





//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J���[�f�[�^�擾
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 *	@param	type		�^�C�v
 *	@param	cp_system	�V�X�e���f�[�^
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicTrColDataGet( NEWSDRAW_TOPIC_TRCOL* p_wk, const NEWS_DATA* cp_data, NEWS_TOPICTYPE type, const WFLBY_SYSTEM* cp_system )
{
	int i;
	u32 pl_idx;
	BOOL vip;

	for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
		pl_idx = NEWS_DSET_GetTopicUserIdx( cp_data, type, i );
		if( pl_idx != WFLBY_PLAYER_MAX ){

			vip = WFLBY_SYSTEM_GetUserVipFlag( cp_system, pl_idx );
			if( vip == TRUE ){
				p_wk->trcol[i] = NEWS_PLTT_FONT_TRVIP;	// ������VIP�I
			}else{
				p_wk->trcol[i] = NEWS_PLTT_FONT_TRN;	// �����͕���
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�]������
 *
 *	@param	cp_wk		���[�N
 *	@param	cp_pltt		�p���b�g�f�[�^
 *	@param	plttidx		�]����p���b�g�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicTrColDataTrans( const NEWSDRAW_TOPIC_TRCOL* cp_wk, const NNSG2dPaletteData* cp_pltt, u32 plttidx )
{
	BOOL result;
	int i;
	const u8* cp_plttdata;

	cp_plttdata = cp_pltt->pRawData;
	
	for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
		// �g���[�i�[�J���[�ɍ������F��]������^�X�N���쐬
		result = AddVramTransferManager( 
				NNS_GFD_DST_2D_BG_PLTT_MAIN,
				(plttidx*0x20)+(NEWS_PLTT_FONTTR_TR_00*2)+(i*0x4),	// �]����
				(void*)(&cp_plttdata[ (NEWS_PLTT_FONT*0x20)+(cp_wk->trcol[i]*2) ]), 
				0x4 );
		GF_ASSERT( result );
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�E�B���h�E	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicWinInit( NEWSDRAW_TOPICWIN* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 heapID )
{
	int i;

	// �g�s�b�N���[�N������
	for( i=0; i<NEWSDRAW_TOPIC_NUM; i++ ){
		NEWSDRAW_TopicInit( &p_wk->topic[i], p_draw, i, heapID );

		// �r�b�g�}�b�v������
		GF_BGL_BmpWinAdd(
					p_draw->p_bgl, &p_wk->bmp[i], GF_BGL_FRAME3_M,
					NEWSDRAW_TOPIC_BMP_X, NEWSDRAW_TOPIC_DRAW_Y[i],
					NEWSDRAW_TOPIC_BMP_SX, NEWSDRAW_TOPIC_BMP_SY,
					NEWSDRAW_TOPIC_BMP_PAL+i, 
					NEWSDRAW_TOPIC_BMP_CGX+(NEWSDRAW_TOPIC_BMP_CGXSIZ*i) );

		GF_BGL_BmpWinDataFill( &p_wk->bmp[i], 0 );
		
		GF_BGL_BmpWinOnVReq( &p_wk->bmp[i] );
	}

	// ������擾
	p_wk->p_str = STRBUF_Create( NEWSDRAW_TOPIC_STRNUM, heapID );
	

	// �p���b�g�f�[�^
	p_wk->p_plttbuff = ArcUtil_HDL_PalDataGet( p_draw->p_handle, 
			NARC_lobby_news_lobby_news_bg_NCLR,
			&p_wk->p_pltt, heapID);


	// �ʏ�̕����F���P�p���b�g����0xD�A0xE�ɓ]������
	GF_BGL_PaletteSet( GF_BGL_FRAME0_M, 
			&((u8*)p_wk->p_pltt->pRawData)[ (NEWS_PLTT_FONT*0x20)+(NEWS_PLTT_FONT_FONT*2) ],
			0x4, (NEWS_PLTT_FONT*0x20)+(NEWS_PLTT_FONTTR_FONT*2) );
	
	GF_BGL_PaletteSet( GF_BGL_FRAME0_M, 
			&((u8*)p_wk->p_pltt->pRawData)[ (NEWS_PLTT_FONT*0x20)+(NEWS_PLTT_FONT_FONT*2) ],
			0x4, (NEWS_PLTT_FONT01*0x20)+(NEWS_PLTT_FONTTR_FONT*2) );

	GF_BGL_PaletteSet( GF_BGL_FRAME0_M, 
			&((u8*)p_wk->p_pltt->pRawData)[ (NEWS_PLTT_FONT*0x20)+(NEWS_PLTT_FONT_FONT*2) ],
			0x4, (NEWS_PLTT_FONT02*0x20)+(NEWS_PLTT_FONTTR_FONT*2) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�E�B���h�E	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicWinExit( NEWSDRAW_TOPICWIN* p_wk )
{
	int i;
	
	// �p���b�g�f�[�^�j��
	sys_FreeMemoryEz( p_wk->p_plttbuff );
	
	// ������j��
	STRBUF_Delete( p_wk->p_str );

	// �g�s�b�N�f�[�^�j��
	for( i=0; i<NEWSDRAW_TOPIC_NUM; i++ ){
		// �r�b�g�}�b�v�j��
		GF_BGL_BmpWinDel( &p_wk->bmp[i] );
		
		NEWSDRAW_TopicExit( &p_wk->topic[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�E�B���h�E	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	cp_system	�L��V�X�e���f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�ύX��������
 *	@retval	FALSE	�ύX���Ȃ�����
 */
//-----------------------------------------------------------------------------
static BOOL NEWSDRAW_TopicWinMain( NEWSDRAW_TOPICWIN* p_wk, NEWS_DATA* p_data, const WFLBY_SYSTEM* cp_system, u32 heapID )
{
	int i;
	BOOL result;
	NEWS_TOPICTYPE type;
	NEWSDRAW_TOPIC_TRCOL trcol;
	WFLBY_VIPFLAG vip_flag;

	// VIP�t���O�擾
	WFLBY_SYSTEM_GetVipFlagWk( cp_system, &vip_flag );
	
	// ���i�̃g�s�b�N���瓮����
	for( i=NEWSDRAW_TOPIC_NUM-1; i>=0; i-- ){
		if( p_wk->topic[i].move ){
			result = NEWSDRAW_TopicMain( &p_wk->topic[i] );
			if( result == TRUE ){
				// �P���̒i�Ƀf�[�^���ʂ�
				if( (i+1) < NEWSDRAW_TOPIC_NUM ){
					NEWSDRAW_TopicStart( &p_wk->topic[i+1], p_wk->topic[i].p_str, NEWSDRAW_TOPIC_COUNT[i+1], &p_wk->topic[i].trcol, p_wk->p_pltt );
				}
				NEWSDRAW_TopicEnd( &p_wk->topic[i] );
			} 
		}
	}

	// 1�i�ڂ̃g�s�b�N���󂢂Ă���V�K�g�s�b�N�ǉ�
	if( p_wk->topic[0].move == FALSE ){

		// �g�s�b�N���������A�����ł���g�s�b�N���P�Đ�����
		while( NEWS_DSET_CheckToppic( p_data ) == TRUE ){
			type = NEWS_DSET_GetTopicType( p_data ); 
			// �擪�f�[�^�擾
			result = NEWS_DSET_GetTopicData( p_data, &vip_flag, type, p_wk->p_str, heapID );
			if( result == TRUE ){

				// �g�s�b�N��ǉ��ł����̂ŃI����
				NEWSDRAW_TopicTrColDataGet( &trcol, p_data, type, cp_system );
				NEWSDRAW_TopicStart( &p_wk->topic[0], p_wk->p_str, NEWSDRAW_TOPIC_COUNT[0], &trcol, p_wk->p_pltt );

				// �擪�̃f�[�^������
				NEWS_DSET_RemoveTopTopic( p_data );
				return TRUE;
			}

			// �擪�̃f�[�^������
			NEWS_DSET_RemoveTopTopic( p_data );
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�E�B���h�E	�`��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicWinDraw( NEWSDRAW_TOPICWIN* p_wk )
{
	int i;

	
	// �g�s�b�N�����r�b�g�}�b�v�ɏ�������
	for( i=0; i<NEWSDRAW_TOPIC_NUM; i++ ){
		if( p_wk->topic[i].move == TRUE ){
			// ��������
			NEWSDRAW_TopicDraw( &p_wk->topic[i], &p_wk->bmp[i] );
		} 
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N	������
 *	
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	idx			���̃g�s�b�N�̃C���f�b�N�X�l
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicInit( NEWSDRAW_TOPIC* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 idx, u32 heapID )
{
	p_wk->move	= FALSE;
	p_wk->count	= 0;
	p_wk->p_str = STRBUF_Create(NEWSDRAW_TOPIC_STRNUM, heapID );

	p_wk->pal	= NEWS_PLTT_FONT + idx;

	// �_�~�[BMPWIN�쐬
	GF_BGL_BmpWinAdd(
				p_draw->p_bgl, &p_wk->bmp, GF_BGL_FRAME3_M,
				0, 0,
				NEWSDRAW_TOPIC_DMBMP_SX, NEWSDRAW_TOPIC_DMBMP_SY,
				NEWSDRAW_TOPIC_BMP_PAL, 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicExit( NEWSDRAW_TOPIC* p_wk )
{
	STRBUF_Delete( p_wk->p_str );

	//  �_�~�[BMPWIN�j��
	GF_BGL_BmpWinDel( &p_wk->bmp );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N	�J�n
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 *	@param	speed		�X�N���[���Ɏg�p����V���N��
 *	@param	cp_trcol	�g���[�i�[�J���[
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicStart( NEWSDRAW_TOPIC* p_wk, const STRBUF* cp_str, u32 speed, const NEWSDRAW_TOPIC_TRCOL* cp_trcol, const NNSG2dPaletteData* cp_pltt )
{
	// ���̓R�s�[
	STRBUF_Copy( p_wk->p_str, cp_str );
	p_wk->move		= TRUE;
	p_wk->count		= 0;

// BTS�ʐM�o�O601�̑Ώ�	tomoya
//	p_wk->count_max	= speed * STRBUF_GetLen( cp_str );	// ����������ő�J�E���g�����߂�
	p_wk->count_max	= speed;
	p_wk->str_siz	= FontProc_GetPrintStrWidth( FONT_TALK, cp_str, 0 );
	p_wk->str_siz	+= NEWSDRAW_TOPIC_YOHAKU;	// ��̗]������

	// �����񒷂��I�[�o�[
	GF_ASSERT( (NEWSDRAW_TOPIC_DMBMP_SX*8) >= p_wk->str_siz );

	// �_�~�[�r�b�g�}�b�v�ɏ�������
	GF_BGL_BmpWinDataFill( &p_wk->bmp, 0 );

	// ��������������
	GF_STR_PrintSimple( &p_wk->bmp, FONT_TALK, p_wk->p_str,
			0, 0,
			MSG_NO_PUT, NULL);

	// �g���[�i�[�J���[�f�[�^��ۑ�
	p_wk->trcol = *cp_trcol;


	// �g���[�i�̕����F�����킹��
	NEWSDRAW_TopicTrColDataTrans( &p_wk->trcol, cp_pltt, p_wk->pal );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�I��
 *	
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicEnd( NEWSDRAW_TOPIC* p_wk )
{
	p_wk->move = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N	���C��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static BOOL NEWSDRAW_TopicMain( NEWSDRAW_TOPIC* p_wk )
{
	BOOL ret = TRUE;
	
	if( (p_wk->count + 1) <= p_wk->count_max ){
		p_wk->count ++;
		ret = FALSE;
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�̕`��	
 *
 *	@param	cp_wk	���[�N
 *	@param	p_bmp	�`���r�b�g�}�b�v
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TopicDraw( const NEWSDRAW_TOPIC* cp_wk, GF_BGL_BMPWIN* p_bmp )
{
	s32	x;	// ��ƂȂ邘���W
	s32 wx;	// �������ݐ悘
	s32 rx;	// �ǂݍ��ݐ悘
	s32 sx;	// �ǂݍ��݃T�C�Y��

	// ������W�擾
	x = ( cp_wk->count * cp_wk->str_siz ) / cp_wk->count_max;

	// �]�������A�{���S�\�������ŏ������̒l���ς��
	if( x < NEWSDRAW_TOPIC_YOHAKU ){
		// �]������
		wx = 255 - x;
		rx = 0;
		sx = x;
	}else{
		// ��낪�󔒂̕���
		wx = 0;
		rx = x - NEWSDRAW_TOPIC_YOHAKU;
		sx = (cp_wk->str_siz - x);
		if( sx > 255 ){
			sx = 255;
		}
	}

	// �r�b�g�}�b�v�N���[��
	GF_BGL_BmpWinFill( p_bmp, 0,
			0, 0,
			255, NEWSDRAW_TOPIC_DMBMP_SY*8 );	

	// ��������
	GF_BGL_BmpWinPrintEx( p_bmp,
			cp_wk->bmp.chrbuf,		// �L�����N�^�o�b�t�@
			rx, 0,					//  chrbuf�ǂݍ��݊J�n�ʒu
			NEWSDRAW_TOPIC_DMBMP_SX*8,	// chrbuf�̃T�C�Y
			NEWSDRAW_TOPIC_DMBMP_SY*8,	// chrbuf�̃T�C�Y
			wx, 0,						// �������ݐ�ʒu
			sx, NEWSDRAW_TOPIC_DMBMP_SY*8, 15 );// �������݃T�C�Y

	GF_BGL_BmpWinOnVReq( p_bmp );
}





//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���E�B���h�E	�`�揉����
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TitleWinInit( NEWSDRAW_TITLEWIN* p_wk, NEWSDRAW_DRAWSYS* p_draw, u32 heapID )
{
	int i;
	MSGDATA_MANAGER*	p_msgman;
	STRBUF*				p_str;

	memset( p_wk, 0, sizeof(NEWSDRAW_TITLEWIN) );

	// ���b�Z�[�W�f�[�^������
	p_msgman	= MSGMAN_Create(MSGMAN_TYPE_NORMAL,ARC_MSG,NARC_msg_wflby_news_dat,heapID );
	p_str		= STRBUF_Create( NEWSDRAW_TITLEWIN_STRNUM, heapID );

	for( i=0; i<NEWSDRAW_TITLEWIN_NUM; i++ ){

		// �^�C�g���G�t�F�N�g������
		NEWSDRAW_TitleEffInit( &p_wk->eff[i], NEWSDRAW_TITLEWIN_PLTT_OFFS[i], NEWSDRAW_TITLEWIN_SND_TBL[i] );

		// ���b�Z�[�W
		GF_BGL_BmpWinAdd(
				p_draw->p_bgl, &p_wk->bmp[i], GF_BGL_FRAME0_M,
				NEWSDRAW_TITLE_BMPDATA[i].x, NEWSDRAW_TITLE_BMPDATA[i].y,
				NEWSDRAW_TITLE_BMPDATA[i].sizx, NEWSDRAW_TITLE_BMPDATA[i].sizy,
				NEWSDRAW_TITLE_BMPDATA[i].pal, NEWSDRAW_TITLE_BMPDATA[i].cgx );

		GF_BGL_BmpWinDataFill( &p_wk->bmp[i], 0 );

		// �^�C�g������������
		MSGMAN_GetString( p_msgman, wflby_news_00+i, p_str );
		

/*
		if( i==NEWSDRAW_TITLEWIN_APLNAME ){
			FontProc_LoadFont( FONT_BUTTON, heapID );	//�{�^���t�H���g�̃��[�h
			
			GF_STR_PrintColor(
				&p_wk->bmp[i], FONT_BUTTON, p_str, NEWSDRAW_TITLE_BMPDATA[i].dx,
				NEWSDRAW_TITLE_BMPDATA[i].dy, MSG_NO_PUT, NEWSDRAW_TITLEWIN_COL, NULL );

			FontProc_UnloadFont( FONT_BUTTON );				//�{�^���t�H���g�̔j��
		}else{
			GF_STR_PrintColor(
				&p_wk->bmp[i], FONT_SYSTEM, p_str, NEWSDRAW_TITLE_BMPDATA[i].dx,
				NEWSDRAW_TITLE_BMPDATA[i].dy, MSG_NO_PUT, NEWSDRAW_TITLEWIN_COL, NULL );
		}
//*/


		if( i==NEWSDRAW_TITLEWIN_APLNAME ){
			GF_STR_PrintColor(
				&p_wk->bmp[i], FONT_SYSTEM, p_str, NEWSDRAW_TITLE_BMPDATA[i].dx,
				NEWSDRAW_TITLE_BMPDATA[i].dy, MSG_NO_PUT, NEWSDRAW_TITLEWIN_APL_COL, NULL );
		}else{
	
			GF_STR_PrintColor(
				&p_wk->bmp[i], FONT_SYSTEM, p_str, NEWSDRAW_TITLE_BMPDATA[i].dx,
				NEWSDRAW_TITLE_BMPDATA[i].dy, MSG_NO_PUT, NEWSDRAW_TITLEWIN_COL, NULL );
		}
		
		GF_BGL_BmpWinOnVReq( &p_wk->bmp[i] );
	}

	// ���b�Z�[�W�f�[�^�j��
	STRBUF_Delete( p_str );
	MSGMAN_Delete( p_msgman );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���E�B���h�E�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TitleWinExit( NEWSDRAW_TITLEWIN* p_wk )
{
	int i;


	for( i=0; i<NEWSDRAW_TITLEWIN_NUM; i++ ){
		GF_BGL_BmpWinDel( &p_wk->bmp[i] );

		// �^�C�g���G�t�F�N�g�j��
		NEWSDRAW_TitleEffExit( &p_wk->eff[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���E�B���h�E���C��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TitleWinDraw( NEWSDRAW_TITLEWIN* p_wk )
{
	int i;

	for( i=0; i<NEWSDRAW_TITLEWIN_NUM; i++ ){
		NEWSDRAW_TitleEffDraw( &p_wk->eff[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���E�B���h�E		�G�t�F�N�g�J�n
 *
 *	@param	p_wk		���[�N
 *	@param	title_no	�^�C�g��NO
	NEWSDRAW_TITLEWIN_TIME,		// ����
	NEWSDRAW_TITLEWIN_PLAYER,	// �v���C���[�\��
	NEWSDRAW_TITLEWIN_TOPIC,	// �g�s�b�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TitleWinEffectStart( NEWSDRAW_TITLEWIN* p_wk, u32 title_no )
{
	GF_ASSERT( title_no < NEWSDRAW_TITLEWIN_NUM );
	GF_ASSERT( title_no != NEWSDRAW_TITLEWIN_APLNAME );

	NEWSDRAW_TitleEffStart( &p_wk->eff[title_no] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���E�B���h�E	�G�t�F�N�g������
 *
 *	@param	p_wk		���[�N
 *	@param	offs		�p���b�g�I�t�Z�b�g
 *	@param	snd			�T�E���hNO
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TitleEffInit( NEWSDRAW_TITLEEFF* p_wk, u16 offs, u32 snd )
{
	memset( p_wk, 0, sizeof(NEWSDRAW_TITLEEFF) );
	p_wk->offs = offs;
	p_wk->sound  = snd;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���E�B���h�E	�G�t�F�N�g�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TitleEffExit( NEWSDRAW_TITLEEFF* p_wk )
{
	memset( p_wk, 0, sizeof(NEWSDRAW_TITLEEFF) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���E�B���h�E	�G�t�F�N�g�J�n
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TitleEffStart( NEWSDRAW_TITLEEFF* p_wk )
{
	p_wk->on		= TRUE;
	p_wk->col_count = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���E�B���h�E	�G�t�F�N�g���C��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWSDRAW_TitleEffDraw( NEWSDRAW_TITLEEFF* p_wk )
{
	u32 evy;
	u16 col;

	// �I���`�F�b�N
	if( p_wk->col_count > NEWSDRAW_TITLEWIN_EFFECT_COUNT ){
		p_wk->on = FALSE;
	}

	
	if( p_wk->on == FALSE ){
		return ;
	}

	// ����炷�^�C�~���O
	if( p_wk->col_count == NEWSDRAW_TITLEWIN_EFFECT_SND_PLY ){
		Snd_SePlay( p_wk->sound );
	}

	// �J���[�W�������߂�
	if( p_wk->col_count < NEWSDRAW_TITLEWIN_EFFECT_COUNT_HF ){
		evy = (p_wk->col_count * 16) / NEWSDRAW_TITLEWIN_EFFECT_COUNT_HF;
	
	// �ҋ@��
	}else if( p_wk->col_count < NEWSDRAW_TITLEWIN_EFFECT_COUNT_HF+NEWSDRAW_TITLEWIN_EFFECT_COUNT_WT ){
		evy = 16;
		
	// �����Ă���
	}else {
		evy = ((p_wk->col_count - (NEWSDRAW_TITLEWIN_EFFECT_COUNT_HF+NEWSDRAW_TITLEWIN_EFFECT_COUNT_WT)) * 16) / NEWSDRAW_TITLEWIN_EFFECT_COUNT_HF2;
		evy = 16 - evy;
	}

	//  �ݒ�
	col = NEWSDRAW_TITLEWIN_EFFECT_COL;
	SoftFade( &col, &p_wk->buff, 1, evy, NEWSDRAW_TITLEWIN_EFFECT_COL_END );

	// �]��
	DC_FlushRange( &p_wk->buff, 2 );
	GX_LoadBGPltt( 
			&p_wk->buff,
			p_wk->offs,
			2 );
	
	// �J�E���g
	p_wk->col_count ++;
}



#ifdef WFLBY_DEBUG_LOBBY_NEWS_TOPIC_MAKE
static void NEWSDRAW_DEBUG_TopicMake( NEWSDRAW_WK* p_wk, WFLBY_SYSTEM* p_system, u32 heapID )
{
	MYSTATUS* p_status;
	const WFLBY_USER_PROFILE* cp_profile;

	cp_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_system );
	p_status = MyStatus_AllocWork( heapID );
	WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_status, heapID );
	
	switch( sc_WFLBY_DEBUG_LOBBY_NEWS_TOPIC_MAKE_TOPIC ){
	case NEWS_TOPICTYPE_CONNECT:			// ���A
		{
			NEWS_DATA_SET_CONNECT data;
			data.cp_p1	= p_status;
			data.cp_p2	= p_status;
			data.idx_p1	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p2	= WFLBY_SYSTEM_GetMyIdx( p_system );
			NEWS_DSET_SetConnect( p_wk->p_data, &data );
		}
		break;

	case NEWS_TOPICTYPE_ITEM:			// �K�W�F�b�g�̌���
		{
			NEWS_DATA_SET_ITEM data;
			data.cp_p1	= p_status;
			data.cp_p2	= p_status;
			data.idx_p1	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p2	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.item	= 0;
			NEWS_DSET_SetItem( p_wk->p_data, &data );
		}
		break;

	case NEWS_TOPICTYPE_MINIGAME:		// �~�j�Q�[��
		{
			NEWS_DATA_SET_MINIGAME data;
			data.minigame = gf_mtRand() % 3;
			data.num	= 4;
			data.cp_p1	= p_status;
			data.cp_p2	= p_status;
			data.cp_p3	= p_status;
			data.cp_p4	= p_status;
			data.idx_p1	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p2	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p3	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p4	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.play	= TRUE;
			NEWS_DSET_SetMiniGame( p_wk->p_data, &data );
		}
		break;

	case NEWS_TOPICTYPE_FOOTBOARD:		// �������ƃ{�[�h
		{
			NEWS_DATA_SET_FOOTBOARD data;
			data.board = WFLBY_GAME_FOOTWHITE;
			data.num	= 6;
			data.cp_p1	= p_status;
			data.idx_p1	= WFLBY_SYSTEM_GetMyIdx( p_system );
			NEWS_DSET_SetFootBoard( p_wk->p_data, &data );
		}
		break;

	case NEWS_TOPICTYPE_WORLDTIMER:		// ���E���v
		{
			NEWS_DATA_SET_WORLDTIMER	data;
			data.num	= 4;
			data.cp_p1	= p_status;
			data.cp_p2	= p_status;
			data.cp_p3	= p_status;
			data.cp_p4	= p_status;
			data.idx_p1	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p2	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p3	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p4	= WFLBY_SYSTEM_GetMyIdx( p_system );
			NEWS_DSET_SetWorldTimer( p_wk->p_data, &data );
		}
		break;

	case NEWS_TOPICTYPE_LOBBYNEWS:		// ���r�[�j���[�X
		{
			NEWS_DATA_SET_LOBBYNEWS	data;
			data.num	= 4;
			data.cp_p1	= p_status;
			data.cp_p2	= p_status;
			data.cp_p3	= p_status;
			data.cp_p4	= p_status;
			data.idx_p1	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p2	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p3	= WFLBY_SYSTEM_GetMyIdx( p_system );
			data.idx_p4	= WFLBY_SYSTEM_GetMyIdx( p_system );
			NEWS_DSET_SetLobbyNews( p_wk->p_data, &data );
		}
		break;

	case NEWS_TOPICTYPE_TIMEEVENT:		// ���ԃC�x���g
		{
			NEWS_DATA_SET_TIMEEVENT	data;
			data.cp_time	= p_wk->cp_nowtime;
			data.event_no	= WFLBY_EVENT_FIRE;
			NEWS_DSET_SetTimeEvent( p_wk->p_data, &data );
		}
		break;

	case NEWS_TOPICTYPE_VIPIN:			// VIP�����������Ƃ��̃j���[�X
		{
			NEWS_DATA_SET_TIMEEVENT	data;
			data.cp_time	= p_wk->cp_nowtime;
			data.event_no	= WFLBY_EVENT_PARADE;
			NEWS_DSET_SetTimeEvent( p_wk->p_data, &data );
		}
		break;
	}

	sc_WFLBY_DEBUG_LOBBY_NEWS_TOPIC_MAKE_TOPIC = (sc_WFLBY_DEBUG_LOBBY_NEWS_TOPIC_MAKE_TOPIC + 1) % NEWS_TOPICTYPE_NUM;

	sys_FreeMemoryEz( p_status );
}
#endif

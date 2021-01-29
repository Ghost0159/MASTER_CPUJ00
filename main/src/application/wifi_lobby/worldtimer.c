//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		worldtimer.c
 *	@brief		���E���v
 *	@author		tomoya takahashi
 *	@data		2007.09.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "system/d3dobj.h"
#include "system/clact_util.h"
#include "system/wipe.h"
#include "system/render_oam.h"
#include "system/pm_overlay.h"
#include "system/fontproc.h"
#include "system/msgdata.h"
#include "system/window.h"
#include "system/wordset.h"
#include "system/touch_subwindow.h"

#include "communication/communication.h"

#include "savedata/wifihistory.h"
#include "savedata/config.h"

#include "graphic/worldtimer.naix"

#include "msgdata/msg.naix"
#include "msgdata/msg_worldtimer.h"
#include "msgdata/msg_wifi_place_msg_world.h"
#include "msgdata/msg_wifi_place_msg_GBR.h"

#include "application/wifi_lobby/worldtimer.h"
#include "application/wifi_lobby/wldtimer_snd.h"
#include "application/wifi_earth/wifi_earth_place.naix"
#include "application/wifi_earth/wifi_earth.naix"
#include "application/wifi_country.h"

#include "application/wifi_lobby/worldtimer_place.h"

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


//-----------------------------------------------------------------------------
/**
 *				�f�o�b�N�ݒ�
 */
//-----------------------------------------------------------------------------
#ifdef  PM_DEBUG
#define WLDTIMER_DEBUG_ALLPLACEOPEN		// �S�n��̕\��
//#define WLDTIMER_DEBUG_TIMEZONE	// �^�C���]�[�����R���g���[������
#endif

// �^�C���]�[���R���g���[��
#ifdef WLDTIMER_DEBUG_TIMEZONE
static s32 s_WLDTIMER_DEBUG_TimeZone;
#endif

#ifdef WLDTIMER_DEBUG_ALLPLACEOPEN
static BOOL s_WLDTIMER_DEBUG_ALLPLACEOPEN_FLAG = FALSE;
#endif


//-----------------------------------------------------------------------------
//	src/application/wifi_earth/wifi_earth����R�s�[
//-----------------------------------------------------------------------------

//�n�_�o�^�ő吔
#define WORLD_PLACE_NUM_MAX	(0x400)	//���������P�O�O�O�����炢�Ƃ�

//�n�����f����������`
#define INIT_EARTH_TRANS_XVAL	(0)
#define INIT_EARTH_TRANS_YVAL	(0)
#define INIT_EARTH_TRANS_ZVAL	(0)
#define INIT_EARTH_SCALE_XVAL	(FX32_ONE)
#define INIT_EARTH_SCALE_YVAL	(FX32_ONE)
#define INIT_EARTH_SCALE_ZVAL	(FX32_ONE)
#define INIT_EARTH_ROTATE_XVAL	(0x1980)	//�������f�t�H���g�ʒu�ɂ���
#define INIT_EARTH_ROTATE_YVAL	(0xcfe0)	//�������f�t�H���g�ʒu�ɂ���
#define INIT_EARTH_ROTATE_ZVAL	(0)

//�J������������`
#define INIT_CAMERA_TARGET_XVAL	(0)
#define INIT_CAMERA_TARGET_YVAL	(0)
#define INIT_CAMERA_TARGET_ZVAL	(0)
#define INIT_CAMERA_POS_XVAL	(0)
#define INIT_CAMERA_POS_YVAL	(0)
#define INIT_CAMERA_POS_ZVAL	(0x128000)
#define INIT_CAMERA_DISTANCE_NEAR	(0x050000)
#define INIT_CAMERA_DISTANCE_FAR	(0x128000)
#define INIT_CAMERA_PERSPWAY	(0x05c1)
#define INIT_CAMERA_CLIP_NEAR	(0)
#define INIT_CAMERA_CLIP_FAR	(FX32_ONE*100)
#define CAMERA_ANGLE_MIN		(-0x4000+0x200)
#define CAMERA_ANGLE_MAX		(0x4000-0x200)

//���C�g��������`
#define USE_LIGHT_NUM			(0)
#define LIGHT_VECDEF			(FX32_ONE-1)
#define INIT_LIGHT_ANGLE_XVAL	(0)
#define INIT_LIGHT_ANGLE_YVAL	(0)
#define INIT_LIGHT_ANGLE_ZVAL	(-LIGHT_VECDEF)

//���쏈���p�萔
#define CAMERA_MOVE_SPEED_NEAR	(0x20)	//�J�����ߋ������㉺���E�ړ��X�s�[�h	
#define CAMERA_MOVE_SPEED_FAR	(0x200)	//�J�������������㉺���E�ړ��X�s�[�h
#define CAMERA_INOUT_SPEED	(0x8000)	//�J�������߈ړ��X�s�[�h
#define MARK_SCALE_INCDEC	(0x80)		//�n�_�}�[�N���߈ړ����g�k�䗦
#define EARTH_LIMIT_ROTATE_XMIN	(0x1300)	//���{�g��n�}�̏c��]���~�b�g�����i����j
#define EARTH_LIMIT_ROTATE_XMAX	(0x2020)	//���{�g��n�}�̏c��]���~�b�g����i�k�C���t���j
#define EARTH_LIMIT_ROTATE_YMIN	(0xcc80)	//���{�g��n�}�̉���]���~�b�g�����i�k�C�����H�j
#define EARTH_LIMIT_ROTATE_YMAX	(0xd820)	//���{�g��n�}�̉���]���~�b�g����i����j

// �A�C�R���E�B���h�E
#define	EARTH_ICON_WIN_PX		(25)
#define	EARTH_ICON_WIN_PY		(21)
#define	EARTH_ICON_WIN_SX		(6)
#define	EARTH_ICON_WIN_SY		(2)
#define DOTSIZE					(8)

//�n����]���[�h��`
enum {
	JAPAN_MODE = 0,
	GLOBAL_MODE,
};

//�J���������t���O��`
enum {
	CAMERA_FAR = 0,
	CAMERA_NEAR,
};

//���X�g�f�[�^�\���́i�o�C�i���f�[�^�����ۂɎg�p���郊�X�g�p�f�[�^�ɕϊ��j
typedef struct EARTH_DEMO_LIST_DATA_tag
{
	s16 x;
	s16 y;
	MtxFx33 rotate;	//���Ή�]�s������炩���ߌv�Z���ĕۑ�
	u16	col;
	u16	nationID;
	u16	areaID;
}EARTH_DEMO_LIST_DATA;

//���X�g�f�[�^�\���́i�o�C�i���f�[�^�����ۂɎg�p���郊�X�g�p�f�[�^�ɕϊ��j
typedef struct EARTH_DEMO_LIST_tag
{
	u32	listcount;
	EARTH_DEMO_LIST_DATA	place[WORLD_PLACE_NUM_MAX];
}EARTH_DEMO_LIST;
//------------------------------------------------------------------------------







//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�ɂق�@���R�[�h
//=====================================
#define WLDTIMER_JPN_NATIONID	(country103)



//-------------------------------------
///	���b�Z�[�W�o�b�t�@��
//=====================================
#define WLDTIMER_MSGMAN_STRBUFNUM	(128)


//-------------------------------------
///	Vram�]���}�l�[�W���@�^�X�N��
//=====================================
#define WLDTIMER_VRAMTRANS_TASKNUM	( 64 )


//-------------------------------------
///	����Ǘ�
//=====================================
enum{
	WLDTIMER_SEQ_FADEIN,
	WLDTIMER_SEQ_FADEINWAIT,
	WLDTIMER_SEQ_MAIN,
	WLDTIMER_SEQ_ENDCHECK,
	WLDTIMER_SEQ_ENDWAIT,
	WLDTIMER_SEQ_FADEOUT,
	WLDTIMER_SEQ_FADEOUTWAIT,
};
enum{	// �n���V��ʂ̃V�[�P���X
	WLDTIMER_MAINSEQ_CONTROL,
	WLDTIMER_MAINSEQ_CAMERAMOVE,

	WLDTIMER_MAINSEQ_ENDCHECK,
	WLDTIMER_MAINSEQ_ENDCHECKWAIT,
};
enum{	// �^�X�N��ʂ̃V�[�P���X
	WLDTIMER_SUB_SEQ_CONTROL,
};

//-------------------------------------
///	�n��ݒ�
//=====================================
enum{
	WLDTIMER_PLACE_COL_IN,		// �����J���[
	WLDTIMER_PLACE_COL_OUT,		// �ގ��J���[
	WLDTIMER_PLACE_COL_TOUCH,	// �^�b�`�ʒu�J���[
	WLDTIMER_PLACE_COL_NUM,		// �J���[��
	
	// �����ɂ��Ȃ��n��ɐݒ肳��Ă���J���[
	WLDTIMER_PLACE_COL_NONE = WLDTIMER_PLACE_COL_NUM,	
};
static const u32 sc_WLDTIMER_PLACE_MDL[ WLDTIMER_PLACE_COL_NUM ] = {	// ���f�����\�[�X
	NARC_worldtimer_earth_mark_1_nsbmd,
	NARC_worldtimer_earth_mark_2_nsbmd,
	NARC_worldtimer_earth_mark_3_nsbmd,
};
#define WLDTIMER_PLACE_TOUCH_SCALE_Z_ADD	( FX32_CONST(0.1) )


//-------------------------------------
///	BANK�ݒ�
//=====================================
static const GF_BGL_DISPVRAM sc_WLDTIMER_BANK = {
	GX_VRAM_BG_16_F,				// ���C��2D�G���W����BG
	GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
	GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
	GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
	GX_VRAM_OBJ_16_G,				// ���C��2D�G���W����OBJ
	GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
	GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
	GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
	GX_VRAM_TEX_01_AB,				// �e�N�X�`���C���[�W�X���b�g
	GX_VRAM_TEXPLTT_0123_E			// �e�N�X�`���p���b�g�X���b�g
};

//-------------------------------------
///	BG�ݒ�
//=====================================
static const GF_BGL_SYS_HEADER sc_BGINIT = {
	GX_DISPMODE_GRAPHICS,
	GX_BGMODE_0,
	GX_BGMODE_0,
	GX_BG0_AS_3D
};


//-------------------------------------
///	�a�f�R���g���[���f�[�^
//=====================================
#define WLDTIMER_BGCNT_NUM	( 5 )	// �a�f�R���g���[���e�[�u����
static const u32 sc_WLDTIMER_BGCNT_FRM[ WLDTIMER_BGCNT_NUM ] = {
	GF_BGL_FRAME1_M,
	GF_BGL_FRAME2_S,
	GF_BGL_FRAME3_S,
	GF_BGL_FRAME0_S,
	GF_BGL_FRAME1_S,
};
static const GF_BGL_BGCNT_HEADER sc_WLDTIMER_BGCNT_DATA[ WLDTIMER_BGCNT_NUM ] = {
	{	// GF_BGL_FRAME1_M
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0x3800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME2_S
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		2, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME3_S
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		3, 0, 0, FALSE
	},

	// �T�u��ʂO�C�P�͓����L�����N�^�I�t�Z�b�g
	{	// GF_BGL_FRAME0_S
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME1_S
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
		1, 0, 0, FALSE
	},
};


//-------------------------------------
///	�a�f�p���b�g�\��
//=====================================
enum{	// �T�u
	WLDTIMER_PALSUB_BACK00,
	WLDTIMER_PALSUB_BACK01,
	WLDTIMER_PALSUB_BACK02,
	WLDTIMER_PALSUB_BACK03,
	WLDTIMER_PALSUB_NONE00,
	WLDTIMER_PALSUB_BACK05,
	WLDTIMER_PALSUB_NONE02,
	WLDTIMER_PALSUB_NONE03,
	WLDTIMER_PALSUB_NONE04,
	WLDTIMER_PALSUB_NONE05,
	WLDTIMER_PALSUB_TALKWIN,
	WLDTIMER_PALSUB_FONT,
	WLDTIMER_PALSUB_NONE08,
	WLDTIMER_PALSUB_NONE09,
	WLDTIMER_PALSUB_SPFONT,
	WLDTIMER_PALSUB_BACK04,
};
enum{	// ���C��
	WLDTIMER_PALMAIN_SYSTEM,
	WLDTIMER_PALMAIN_FONT,
	WLDTIMER_PALMAIN_TALK,
	WLDTIMER_PALMAIN_BTTN,
	WLDTIMER_PALMAIN_NONE02,
	WLDTIMER_PALMAIN_NONE03,
	WLDTIMER_PALMAIN_NONE04,
	WLDTIMER_PALMAIN_NONE05,
	WLDTIMER_PALMAIN_NONE06,
	WLDTIMER_PALMAIN_NONE07,
	WLDTIMER_PALMAIN_NONE08,
	WLDTIMER_PALMAIN_NONE09,
	WLDTIMER_PALMAIN_NONE10,
	WLDTIMER_PALMAIN_NONE11,
	WLDTIMER_PALMAIN_NONE12,
	WLDTIMER_PALMAIN_NONE13,
};

//-------------------------------------
///	OAM�ݒ�
//=====================================
#define WLDTIMER_OAM_CONTNUM	( 32 )
#define WLDTIMER_RESMAN_NUM		( 4 )	// OAM���\�[�X�}�l�[�W����
#define WLDTIMER_SF_MAT_Y		( FX32_CONST(256) )
static const CHAR_MANAGER_MAKE sc_WLDTIMER_CHARMAN_INIT = {
	WLDTIMER_OAM_CONTNUM,
	16*1024,	// 16K
	16*1024,	// 16K
	HEAPID_WLDTIMER
};


//-------------------------------------
///	�J����
//=====================================
static const VecFx32 sc_WLDTIMER_CAMERA_TARGET={
	INIT_CAMERA_TARGET_XVAL,
	INIT_CAMERA_TARGET_YVAL,
	INIT_CAMERA_TARGET_ZVAL
};
static const VecFx32 sc_WLDTIMER_CAMERA_POS={
	INIT_CAMERA_POS_XVAL,
	INIT_CAMERA_POS_YVAL,
	INIT_CAMERA_POS_ZVAL
};

//-------------------------------------
///	�ڋ߂��Ă���Ƃ��̒n��X�P�[���l
//=====================================
#define INIT_EARTH_SCALE_NEAR_XVAL	(0x300)
#define INIT_EARTH_SCALE_NEAR_YVAL	(0x300)
#define INIT_EARTH_SCALE_NEAR_ZVAL	(FX32_ONE)


//-------------------------------------
///	�n���V�Ǘ��Ǘ�
//=====================================
enum{
	WLDTIMER_EARTHCONT_RET_NONE,			// �����Ȃ�
	WLDTIMER_EARTHCONT_RET_CAMERA_MOVE,		// �J��������
	WLDTIMER_EARTHCONT_RET_END,				// �I��
};
#define WLDTIMER_POINTPLANE_HITAREA	( 0x80 )	// �n��Ƀ|�C���g���������Ă��邩�`�F�b�N����G���A


//-------------------------------------
///	�^�b�`
//=====================================
enum{
	WLDTIMER_TOUCH_PM_RESULT,
	WLDTIMER_TOUCH_PM_LENX,
	WLDTIMER_TOUCH_PM_LENY,
};
#define WLDTIMER_TOUCH_ZOOMWAIT_COUNT	( 4 )	// trg���炱�̃V���N�ȓ��Ƀ^�b�`����߂���J�����𓮂���
#define WLDTIMER_TOUCH_LEN_LIMIT		( 0x3f )	// �X���C�h�������~�b�g
#define WLDTIMER_TOUCH_EARTHSPEED_DIVNEAR	( 3 )	// �^�b�`�Œn���V���񂷂Ƃ��Ɋ���l
#define WLDTIMER_TOUCH_EARTHSPEED_DIVFAR	( 6 )	// �^�b�`�Œn���V���񂷂Ƃ��Ɋ���l
#define	WLDTIMER_TOUCH_END_MSG_COL	( GF_PRINTCOLOR_MAKE( 2, 1, 15 ) )




//-------------------------------------
///	 ���ԑуf�[�^	ZONETYPE
//=====================================
enum{
	WLDTIMER_ZONETYPE_MORNING,	// ��
	WLDTIMER_ZONETYPE_NOON,		// ��
	WLDTIMER_ZONETYPE_EVENING,	// �[��
	WLDTIMER_ZONETYPE_NIGHT,	// ��
	WLDTIMER_ZONETYPE_MIDNIGHT,	// �[��
	WLDTIMER_ZONETYPE_NUM,		// ��
};
enum{	// �|�P����
	WLDTIMER_TIME_POKE_EEFI= 0,		// �G�[�t�B�[
	WLDTIMER_TIME_POKE_POPPO,		// �|�b�|
	WLDTIMER_TIME_POKE_MITSUHANII,	// �~�c�n�j�[
	WLDTIMER_TIME_POKE_CHERIMU,		// �`�F����
	WLDTIMER_TIME_POKE_SORUROKKU,	// �\�����b�N
	WLDTIMER_TIME_POKE_KIMAWARI,	// �L�}����
	WLDTIMER_TIME_POKE_YAMIKARASU,	// ���~�J���X	
	WLDTIMER_TIME_POKE_FUWANTE,		// �t�����e
	WLDTIMER_TIME_POKE_SURIPA,		// �X���[�p�[
	WLDTIMER_TIME_POKE_GOOSUTO,		// �S�[�X�g
	WLDTIMER_TIME_POKE_RUNATON,		// ���i�g�[��
	WLDTIMER_TIME_POKE_BURAKKI,		// �u���b�L�[
	WLDTIMER_TIME_POKE_NUM,
};
#define	WLDTIMER_TIMEZONE_DATANUM	( 24 )	// �^�C���]�[����
#define WLDTIMER_TIMEZONE_START_HOUR	(4)	// �C���f�b�N�X0�̎���
typedef struct{
	u8			zone;		// �]�[��ID
	u8			mons;		// �|�P�����̊G
} WLDTIMER_TIMERZONE;
static const WLDTIMER_TIMERZONE	sc_WLDTIMER_TIMEZONE_DATA[ WLDTIMER_TIMEZONE_DATANUM ] ={
	{	// 4hour
		WLDTIMER_ZONETYPE_MORNING,	WLDTIMER_TIME_POKE_EEFI
	},
	{	// 5hour
		WLDTIMER_ZONETYPE_MORNING,	WLDTIMER_TIME_POKE_EEFI
	},
	{	// 6hour
		WLDTIMER_ZONETYPE_MORNING,	WLDTIMER_TIME_POKE_POPPO
	},
	{	// 7hour
		WLDTIMER_ZONETYPE_MORNING,	WLDTIMER_TIME_POKE_POPPO  
	},
	{	// 8hour
		WLDTIMER_ZONETYPE_MORNING,	WLDTIMER_TIME_POKE_MITSUHANII
	},
	{	// 9hour
		WLDTIMER_ZONETYPE_MORNING,	WLDTIMER_TIME_POKE_MITSUHANII
	},
	{	// 10hour
		WLDTIMER_ZONETYPE_NOON,		WLDTIMER_TIME_POKE_CHERIMU
	},
	{	// 11hour
		WLDTIMER_ZONETYPE_NOON,		WLDTIMER_TIME_POKE_CHERIMU
	},
	{	// 12hour
		WLDTIMER_ZONETYPE_NOON,		WLDTIMER_TIME_POKE_SORUROKKU
	},
	{	// 13hour
		WLDTIMER_ZONETYPE_NOON,		WLDTIMER_TIME_POKE_SORUROKKU
	},
	{	// 14hour
		WLDTIMER_ZONETYPE_NOON,		WLDTIMER_TIME_POKE_KIMAWARI
	},
	{	// 15hour
		WLDTIMER_ZONETYPE_NOON,		WLDTIMER_TIME_POKE_KIMAWARI
	},
	{	// 16hour
		WLDTIMER_ZONETYPE_EVENING,	WLDTIMER_TIME_POKE_YAMIKARASU
	},
	{	// 17hour
		WLDTIMER_ZONETYPE_EVENING,	WLDTIMER_TIME_POKE_YAMIKARASU
	},
	{	// 18hour
		WLDTIMER_ZONETYPE_EVENING,	WLDTIMER_TIME_POKE_FUWANTE
	},
	{	// 19hour
		WLDTIMER_ZONETYPE_EVENING,	WLDTIMER_TIME_POKE_FUWANTE
	},
	{	// 20hour
		WLDTIMER_ZONETYPE_NIGHT,	WLDTIMER_TIME_POKE_SURIPA
	},
	{	// 21hour
		WLDTIMER_ZONETYPE_NIGHT,	WLDTIMER_TIME_POKE_SURIPA
	},
	{	// 22hour
		WLDTIMER_ZONETYPE_NIGHT,	WLDTIMER_TIME_POKE_GOOSUTO
	},
	{	// 23hour
		WLDTIMER_ZONETYPE_NIGHT,	WLDTIMER_TIME_POKE_GOOSUTO
	},
	{	// 0hour
		WLDTIMER_ZONETYPE_MIDNIGHT,	WLDTIMER_TIME_POKE_RUNATON
	},
	{	// 1hour
		WLDTIMER_ZONETYPE_MIDNIGHT,	WLDTIMER_TIME_POKE_RUNATON
	},
	{	// 2hour
		WLDTIMER_ZONETYPE_MIDNIGHT,	WLDTIMER_TIME_POKE_BURAKKI
	},
	{	// 3hour
		WLDTIMER_ZONETYPE_MIDNIGHT,	WLDTIMER_TIME_POKE_BURAKKI
	},
};




//-------------------------------------
///	���ԕ\�����	
//=====================================
//	�V�[�P���X
enum{
	WLDTIMER_VIEWER_SEQ_MAIN,
	WLDTIMER_VIEWER_SEQ_FADE_INIT,
	WLDTIMER_VIEWER_SEQ_FADE,
	WLDTIMER_VIEWER_SEQ_FADE_SKIP_INIT,
	WLDTIMER_VIEWER_SEQ_FADE_SKIP,
};

// �\���f�[�^�o�b�t�@��
// �\���f�[�^3��  
// �L���[�Ō���p�o�b�t�@1��
#define WLDTIMER_VIEWER_BUFFNUM	( 4 )	
enum{	// �\������
	WLDTIMER_VIEWER_DRAW_TOP,	// ��
	WLDTIMER_VIEWER_DRAW_MID,	// ��
	WLDTIMER_VIEWER_DRAW_UND,	// ��
	WLDTIMER_VIEWER_DRAWNUM
};
// �X�N���[���]���ʒu
#define WLDTIMER_VIEWER_SCRN_X	( 0 )	// �\���J�n�ʒu
#define WLDTIMER_VIEWER_SCRN_Y	( 0 )	
#define WLDTIMER_VIEWER_SCRN_SX	( 32 )	// �X�N���[�����T�C�Y
#define WLDTIMER_VIEWER_SCRN_SY	( 6 )	// �X�N���[���c�T�C�Y
// �A�j���f�[�^
static const u8 WLDTIMER_VIEWER_ANM_FRAME[ WLDTIMER_ZONETYPE_NUM ] = {
	30,		// ��
	30,		// ��
	30,		// �[��
	30,		// ��
	30,		// �[��
};
// ���b�Z�[�W
#define WLDTIMER_VIEWER_MSG_TITLE_X		( 0 )
#define WLDTIMER_VIEWER_MSG_TITLE_Y		( 0 )
#define WLDTIMER_VIEWER_MSG_NATION_X	( 0 )
#define WLDTIMER_VIEWER_MSG_NATION_Y	( 16 )
#define WLDTIMER_VIEWER_MSG_AREA_X		( 0 )
#define WLDTIMER_VIEWER_MSG_AREA_Y		( 32 )
#define	WLDTIMER_VIEWER_MSG_NATION_COL	( GF_PRINTCOLOR_MAKE( 1, 2, 0 ) )
#define	WLDTIMER_VIEWER_MSG_AREA_COL	( GF_PRINTCOLOR_MAKE( 1, 2, 0 ) )
static const GF_PRINTCOLOR WLDTIMER_VIEWER_MSG_TITLE_COL[ WLDTIMER_ZONETYPE_NUM ] = {
	GF_PRINTCOLOR_MAKE( 5, 6, 0 ),
	GF_PRINTCOLOR_MAKE( 7, 8, 0 ),
	GF_PRINTCOLOR_MAKE( 9, 10, 0 ),
	GF_PRINTCOLOR_MAKE( 11, 12, 0 ),
	GF_PRINTCOLOR_MAKE( 11, 12, 0 ),
};

// �t�F�[�h������
#define WLDTIMER_VIEWER_FADE_DIV_ONE	( 6 )	// �P�]�[���^�C�v�̕�����
#define WLDTIMER_VIEWER_FADE_DIV		( 3*6 )	// �S������
#define WLDTIMER_VIEWER_FADE_DIV_START	( 2 )	// WLDTIMER_VIEWER_FADE_DIV_START�Ât�F�[�h���Ă���
enum{
	WLDTIMER_VIEWER_FADE_DIV_SEQ_CHANGE00,
//	WLDTIMER_VIEWER_FADE_DIV_SEQ_WAIT00,
//	WLDTIMER_VIEWER_FADE_DIV_SEQ_WAIT01,
//	WLDTIMER_VIEWER_FADE_DIV_SEQ_WAIT02,
	WLDTIMER_VIEWER_FADE_DIV_SEQ_CHANGE01,
	WLDTIMER_VIEWER_FADE_DIV_SEQ_NUM,
};
#define WLDTIMER_VIEWER_FADE_SKIP_ONE		( 2 )	// �X�L�b�v����C�Ƀt�F�[�h�����郉�C��
#define WLDTIMER_VIEWER_FADE_SKIP_COUNTNUM	( WLDTIMER_VIEWER_FADE_DIV/WLDTIMER_VIEWER_FADE_SKIP_ONE )
#define WLDTIMER_VIEWER_FADE_SKIP01_START	( 1 )	// �X�L�b�v����C�Ƀt�F�[�h�����郉�C��
// �|�P�����o���[��
#define WLDTIMER_VIEWER_POKEBLN_NCGR_GET(x)	(NARC_worldtimer_world_w_balloon00_NCGR + ((x)*3))
#define WLDTIMER_VIEWER_POKEBLN_NCLR_GET(x)	(NARC_worldtimer_world_w_balloon_NCLR)
#define WLDTIMER_VIEWER_POKEBLN_NCER_GET(x)	(NARC_worldtimer_world_w_balloon00_NCER + ((x)*3))
#define WLDTIMER_VIEWER_POKEBLN_NANR_GET(x)	(NARC_worldtimer_world_w_balloon00_NANR + ((x)*3))
#define WLDTIMER_VIEWER_POKEBLN_NCLR_CONTID	( 0 )	// �p���b�g�Ǘ�ID
#define WLDTIMER_VIEWER_POKEBLN_PL_NUM		(12)		// �ǂݍ��ރp���b�g��
#define WLDTIMER_VIEWER_POKEBLN_BG_PRI		(1)		// BG�D�揇��
#define WLDTIMER_VIEWER_POKEBLN_SFT_PRI		(64)	// �\�t�g�D�揇��
#define WLDTIMER_VIEWER_POKEBLN_WAIT		(128)	// ���̕\���܂ł̃E�G�C�g
#define WLDTIMER_VIEWER_POKEBLN_OAM_SIZ		(32)	
#define WLDTIMER_VIEWER_POKEBLN_OAM_HFSIZ	(16)	



//-------------------------------------
///	�|�P�����o���[���A�j���f�[�^
//=====================================
typedef struct {
	s16 sx;				// �J�n�ʒu
	s16 sy;
	s16 ex;				// �I���ʒu
	s16 ey;
	s16 countmax;		// ����J�E���g�ő�l
	s16 wx;				// ��ꕝ
	s16 wcountmax;		// ��ꕝ�J�E���g�ő�
	u16 pad;
} WLDTIMER_POKEBLN_MOVEDATA;
#define WLDTIMER_POKEBLN_MOVEDATANUM	(WLDTIMER_TIME_POKE_NUM)
#define WLDTIMER_POKEBLN_WX_LIGHT		( 12 )
#define WLDTIMER_POKEBLN_WC_LIGHT		( 54 )
#define WLDTIMER_POKEBLN_WX_MIDDLE		( 8 )
#define WLDTIMER_POKEBLN_WC_MIDDLE		( 80 )
#define WLDTIMER_POKEBLN_WX_HEAVY		( 4 )
#define WLDTIMER_POKEBLN_WC_HEAVY		( 100 )
#define WLDTIMER_POKEBLN_CM_LIGHT		( 220 )	// �y�����D�̏�ɂ����X�s�[�h
#define WLDTIMER_POKEBLN_CM_MIDDLE		( 319 )	// ���ʕ��D�̏�ɂ����X�s�[�h
#define WLDTIMER_POKEBLN_CM_HEAVY		( 399 )	// �d�����D�̏�ɂ����X�s�[�h
static const WLDTIMER_POKEBLN_MOVEDATA sc_WLDTIMER_POKEBLN_MOVEDATA_TBL[ WLDTIMER_POKEBLN_MOVEDATANUM ][ WLDTIMER_VIEWER_DRAWNUM ] = {
	//		sx		sy		ex		ey	cmax						wx							wcmax
	{	//�G�[�t�B�[�|�P����
		{	160,	48,		190,	0,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
		{	206,	96,		160,	48,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
		{	150,	144,	206,	96,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
	},
	{	// �ۂ��ۃ|�P����
		{	190,	48,		140,	0,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	150,	96,		190,	48,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	200,	144,	150,	96,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
	},
	{	// �~�c�n�j�[�|�P����
		{	128,	48,		160,	0,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	210,	96,		128,	48,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	160,	144,	210,	96,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
	},
	{	// �`�F�����|�P����
		{	180,	48,		138,	0,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	138,	96,		180,	48,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	180,	144,	138,	96,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
	},
	{	// �\�����b�N�|�P����
		{	180,	48,		160,	0,	WLDTIMER_POKEBLN_CM_HEAVY,	WLDTIMER_POKEBLN_WX_HEAVY,	WLDTIMER_POKEBLN_WC_HEAVY },	// ��
		{	130,	96,		180,	48,	WLDTIMER_POKEBLN_CM_HEAVY,	WLDTIMER_POKEBLN_WX_HEAVY,	WLDTIMER_POKEBLN_WC_HEAVY },	// ��
		{	128,	144,	130,	96,	WLDTIMER_POKEBLN_CM_HEAVY,	WLDTIMER_POKEBLN_WX_HEAVY,	WLDTIMER_POKEBLN_WC_HEAVY },	// ��
	},
	{	// �L�}�����|�P����
		{	180,	48,		116,	0,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
		{	116,	96,		180,	48,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
		{	180,	144,	116,	96,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
	},
	{	// ���~�J���X�|�P����
		{	180,	48,		240,	0,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	160,	96,		180,	48,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	96,		144,	160,	96,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
	},
	{	// �t�����e�|�P����
		{	144,	48,		218,	0,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	218,	96,		144,	48,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
		{	144,	144,	218,	96,	WLDTIMER_POKEBLN_CM_LIGHT,	WLDTIMER_POKEBLN_WX_LIGHT,	WLDTIMER_POKEBLN_WC_LIGHT },	// ��
	},
	{	// �X���[�p�[�|�P����
		{	160,	48,		192,	0,	WLDTIMER_POKEBLN_CM_HEAVY,	WLDTIMER_POKEBLN_WX_HEAVY,	WLDTIMER_POKEBLN_WC_HEAVY },	// ��
		{	192,	96,		160,	48,	WLDTIMER_POKEBLN_CM_HEAVY,	WLDTIMER_POKEBLN_WX_HEAVY,	WLDTIMER_POKEBLN_WC_HEAVY },	// ��
		{	160,	144,	192,	96,	WLDTIMER_POKEBLN_CM_HEAVY,	WLDTIMER_POKEBLN_WX_HEAVY,	WLDTIMER_POKEBLN_WC_HEAVY },	// ��
	},
	{	// �S�[�X�g�|�P����
		{	140,	48,		96,		0,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
		{	190,	96,		140,	48,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
		{	240,	144,	190,	96,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
	},
	{	// ���i�g�[���|�P����
		{	212,	48,		160,	0,	WLDTIMER_POKEBLN_CM_HEAVY,	WLDTIMER_POKEBLN_WX_HEAVY,	WLDTIMER_POKEBLN_WC_HEAVY },	// ��
		{	212,	96,		212,	48,	WLDTIMER_POKEBLN_CM_HEAVY,	WLDTIMER_POKEBLN_WX_HEAVY,	WLDTIMER_POKEBLN_WC_HEAVY },	// ��
		{	160,	144,	212,	96,	WLDTIMER_POKEBLN_CM_HEAVY,	WLDTIMER_POKEBLN_WX_HEAVY,	WLDTIMER_POKEBLN_WC_HEAVY },	// ��
	},
	{	// �u���b�L�[�|�P����
		{	123,	48,		170,	0,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
		{	198,	96,		123,	48,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
		{	133,	144,	198,	96,	WLDTIMER_POKEBLN_CM_MIDDLE,	WLDTIMER_POKEBLN_WX_MIDDLE,	WLDTIMER_POKEBLN_WC_MIDDLE },	// ��
	},
};
 
//-------------------------------------
///	�E�B���h�E�֘A
//
//----------------------------------------------------------------------------
/**
//----------------------------------------------------------------------------
/*	���ƒn�悩�炻�̈ʒu�𒆐S�Ɏ����Ă���*
 *	@brief
 @param			���[�N
 */	
//-----------------------------------------------------------------------------
//=====================================
// �T�u��ʂ̃g�[�N�E�B���h�E
#define WLDTIMER_SUB_TALKWIN_CGX	(10)	// �t���[���L�����N�^�̌�ɑ���
#define WLDTIMER_SUB_TALKWIN_PAL	(WLDTIMER_PALSUB_TALKWIN)
#define WLDTIMER_SUB_TALKWIN_CGXEND	(WLDTIMER_SUB_TALKWIN_CGX+TALK_WIN_CGX_SIZ)

// ���C����ʂ̉�b�E�B���h�E
#define WLDTIMER_MAIN_TALKWIN_CGX		(1)
#define WLDTIMER_MAIN_TALKWIN_PAL		(WLDTIMER_PALMAIN_TALK)
#define WLDTIMER_MAIN_TALKWIN_CGXEND	(WLDTIMER_MAIN_TALKWIN_CGX+TALK_WIN_CGX_SIZ)

// ���C����ʂ̃V�X�e���E�B���h�E
#define WLDTIMER_MAIN_SYSTEMWIN_CGX		(WLDTIMER_MAIN_TALKWIN_CGXEND)
#define WLDTIMER_MAIN_SYSTEMWIN_PAL		(WLDTIMER_PALMAIN_SYSTEM)
#define WLDTIMER_MAIN_SYSTEMWIN_CGXEND	(WLDTIMER_MAIN_SYSTEMWIN_CGX+MENU_WIN_CGX_SIZ)

										
//-------------------------------------
///	���C����ʃ{�^��
//=====================================
#define WLDTIMER_MAIN_BTTNBMP_X		(EARTH_ICON_WIN_PX)
#define WLDTIMER_MAIN_BTTNBMP_Y		(EARTH_ICON_WIN_PY)
#define WLDTIMER_MAIN_BTTNBMP_SIZX	(EARTH_ICON_WIN_SX)
#define WLDTIMER_MAIN_BTTNBMP_SIZY	(EARTH_ICON_WIN_SY)
#define WLDTIMER_MAIN_BTTNBMP_CGX	( WLDTIMER_MAIN_SYSTEMWIN_CGXEND )
#define WLDTIMER_MAIN_BTTNBMP_PAL	( WLDTIMER_PALMAIN_FONT )
#define WLDTIMER_MAIN_BTTNBMP_CGXEND ( WLDTIMER_MAIN_BTTNBMP_CGX+(WLDTIMER_MAIN_BTTNBMP_SIZX*WLDTIMER_MAIN_BTTNBMP_SIZY) )

//-------------------------------------
///	���C����ʉ�b�E�B���h�E
//=====================================
#define WLDTIMER_MAIN_TALKBMP_X		(2)
#define WLDTIMER_MAIN_TALKBMP_Y		(1)
#define WLDTIMER_MAIN_TALKBMP_SIZX	(27)
#define WLDTIMER_MAIN_TALKBMP_SIZY	(4)
#define WLDTIMER_MAIN_TALKBMP_CGX	( WLDTIMER_MAIN_BTTNBMP_CGXEND )
#define WLDTIMER_MAIN_TALKBMP_PAL	( WLDTIMER_PALMAIN_FONT )
#define WLDTIMER_MAIN_TALKBMP_CGXEND ( WLDTIMER_MAIN_TALKBMP_CGX+(WLDTIMER_MAIN_TALKBMP_SIZX*WLDTIMER_MAIN_TALKBMP_SIZY) )

//-------------------------------------
///	���C����ʃ^�b�`�{�^���E�B���h�E
//=====================================
#define WLDTIMER_MAIN_SUBBTTN_CGX	( WLDTIMER_MAIN_TALKBMP_CGXEND )
#define WLDTIMER_MAIN_SUBBTTN_PLTT	( WLDTIMER_PALMAIN_BTTN )
#define WLDTIMER_MAIN_SUBBTTN_X		( 25 )
#define WLDTIMER_MAIN_SUBBTTN_Y		( 6 )
#define WLDTIMER_MAIN_SUBBTTN_CGXEND	( WLDTIMER_MAIN_SUBBTTN_CGX+TOUCH_SW_USE_CHAR_NUM )
static const TOUCH_SW_PARAM sc_TOUCH_SW_PARAM = {
	NULL,
	GF_BGL_FRAME1_M,
	WLDTIMER_MAIN_SUBBTTN_CGX,
	WLDTIMER_MAIN_SUBBTTN_PLTT,
	WLDTIMER_MAIN_SUBBTTN_X,
	WLDTIMER_MAIN_SUBBTTN_Y,
};


//-------------------------------------
///	�T�u��ʉ�b�E�B���h�E
//=====================================
#define WLDTIMER_SUB_TALKBMP_X		(2)
#define WLDTIMER_SUB_TALKBMP_Y		(19)
#define WLDTIMER_SUB_TALKBMP_SIZX	(27)
#define WLDTIMER_SUB_TALKBMP_SIZY	(4)
#define WLDTIMER_SUB_TALKBMP_CGX	( WLDTIMER_SUB_TALKWIN_CGXEND )
#define WLDTIMER_SUB_TALKBMP_PAL	( WLDTIMER_PALSUB_FONT )
#define WLDTIMER_SUB_TALKBMP_CGSIZ	( WLDTIMER_SUB_TALKBMP_SIZX*WLDTIMER_SUB_TALKBMP_SIZY )

//-------------------------------------
///	�r���[�A�[���b�Z�[�W
//=====================================
#define WLDTIMER_VIEWER_MSGBMP_X		( 0 )
#define WLDTIMER_VIEWER_MSGBMP_Y		( 0 )
#define WLDTIMER_VIEWER_MSGBMP_SIZX		( 27 )
#define WLDTIMER_VIEWER_MSGBMP_SIZY		( 6 )
#define WLDTIMER_VIEWER_MSGBMP_CGX		( WLDTIMER_SUB_TALKBMP_CGX+WLDTIMER_SUB_TALKBMP_CGSIZ )
#define WLDTIMER_VIEWER_MSGBMP_PAL		( WLDTIMER_PALSUB_SPFONT )
#define WLDTIMER_VIEWER_MSGBMP_CGSIZ	( WLDTIMER_VIEWER_MSGBMP_SIZX*WLDTIMER_VIEWER_MSGBMP_SIZY )


//-------------------------------------
///	�^�C���]�[���A�j���������f�[�^
//=====================================
#define WLDTIMER_TIMEZONEANM_SCRNANM_MAX	(2)
typedef struct {
	// �X�N���[���A�j���f�[�^
	u16	scrn_idx[ WLDTIMER_TIMEZONEANM_SCRNANM_MAX ];	// �Ȃ��Ƃ�NULL
	u16	scrn_frame;

	// �p���b�g�A�j���f�[�^
	u16		pltt_idx;		// �Ȃ��Ƃ��͂ǂ�Ȓl�ł�OK
	u16		pltt_frame;		// �����Ƃ��͂O
	u16		pltt_no;		// �]���p���b�g
} WLDTIMER_TIMEZONEANM_INIT;

static const WLDTIMER_TIMEZONEANM_INIT	sc_WLDTIMER_TIMEZONE_ANMINIT[ WLDTIMER_ZONETYPE_NUM ] ={
	{	// ��
		{
			NARC_worldtimer_world_watch1_NSCR,
		},
		1,
		NARC_worldtimer_world_w0anim_NCLR,	4, 0,
	},
	{	// ��
		{
			NARC_worldtimer_world_watch2a_NSCR,
			NARC_worldtimer_world_watch2b_NSCR,
		},
		2,
		0,	0, 1,
	},
	{	// �[��
		{
			NARC_worldtimer_world_watch3_NSCR,
		},
		1,
		NARC_worldtimer_world_w2anim_NCLR,	4, 2,
	},
	{	// ��
		{
			NARC_worldtimer_world_watch4_NSCR,
		},
		1,
		NARC_worldtimer_world_w3anim_NCLR,	16, 3,
	},
	{	// �[��
		{
			NARC_worldtimer_world_watch5_NSCR,
		},
		1,
		NARC_worldtimer_world_w3anim_NCLR,	16, 3,
	},
};

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	����t���O
//=====================================
typedef struct{
	u32	world	:1;		// ���E�\�����[�h���ǂ���
	u32	japan	:1;		// ���{ROM���ǂ���
	u32	pad		:31;
}WLDTIMER_FLAG;

//-------------------------------------
///	�\���V�X�e��
//=====================================
typedef struct {
	// BG
	GF_BGL_INI*				p_bgl;

	// OAM
    CLACT_SET_PTR           p_clactset;		// �Z���A�N�^�[�Z�b�g
    CLACT_U_EASYRENDER_DATA renddata;       // �ȈՃ����_�[�f�[�^
    CLACT_U_RES_MANAGER_PTR p_resman[WLDTIMER_RESMAN_NUM]; // �L�����E�p���b�g���\�[�X�}�l�[�W��
	

	// 3D
	GF_G3DMAN* p_3dman;

	// �A�[�J�C�u
	ARCHANDLE* p_handle;
} WLDTIMER_DRAWSYS;


//-------------------------------------
///	�n���V���[�N
//=====================================
typedef struct {
	// �\���f�[�^
	VecFx32 trans;
	VecFx32 scale;
	VecFx32	rotate;

	// �O���t�B�b�N
    D3DOBJ	obj;
	D3DOBJ_MDL	mdl;
	
} WLDTIMER_EARTH;


//-------------------------------------
///	�n��f�[�^
//=====================================
typedef struct {
	// �f�[�^
	EARTH_DEMO_LIST	placelist;

	// �\���f�[�^
	VecFx32	list_scale;
    D3DOBJ	obj[WLDTIMER_PLACE_COL_NUM];
	D3DOBJ_MDL	mdl[WLDTIMER_PLACE_COL_NUM];
} WLDTIMER_PLACE;

//-------------------------------------
///	�J�������[�N
//=====================================
typedef struct {
	GF_CAMERA_PTR	p_camera;	// �J�����I�u�W�F
	CAMERA_ANGLE	angle;		// �J�����A���O��
	fx32			dist;		// �J��������
	u16				status;		// �J�������
	u16				move;		// ����t���O
} WLDTIMER_CAMERA;

//-------------------------------------
///	�n����P�f�[�^
//	�T�u��ʂ̏��E�C���h�E�ɓn���f�[�^
//=====================================
typedef struct {
	u32		timezone;		// ���ԑ�	sc_WLDTIMER_TIMEZONE_DATA�̃C���f�b�N�X
	u32		nation;			// ��ID
	u32		area;			// �n��ID
} WLDTIMER_POINTDATA;

//-------------------------------------
///	���A���A�[���A��A�[��@�A�j���f�[�^
//=====================================
typedef struct {

	// �J�E���^
	u16 count;
	u16 count_max;

	// �e�����A�j��ONOFF
	u8	drawflag[4];		// �㒆��

	// �A�j�����\�[�X
	void*				p_scrnbuff[ WLDTIMER_TIMEZONEANM_SCRNANM_MAX ];	// �X�N���[���A�j���o�b�t�@
	NNSG2dScreenData*	p_scrndata[ WLDTIMER_TIMEZONEANM_SCRNANM_MAX ];	// frame�����X�N���[���t�@�C��������
	u16					scrnframe;
	u16					scrnframe_now;
	void*				p_plttbuff;		// �p���b�g�A�j���o�b�t�@
	NNSG2dPaletteData*	p_plttdata;		// plttframe���̖{���p���b�g������
	u8					plttframe;
	u8					plttframe_now;	
	u8					plttno;			// �]���p���b�g
	u8					pad;
	
} WLDTIMER_TIMEZONEANM;

//-------------------------------------
///		�t�F�[�h�f�[�^
//=====================================
typedef struct {
	u8	start;	// �J�E���g����X�^�[�g�t���O
	s8	count;	// �J�E���^�[
	u8	pad[2];
} WLDTIMER_VIEWER_FADE;

//-------------------------------------
///	�|�P�����o���[������
//	���ē���Ɖ�����
//=====================================
typedef struct {
	// ���ݍ��W
	s16 x;	
	s16 y;	

	s16 count;	// ����J�E���g
	s16 wcount;	// ���J�E���g

	// �R�̈ʒu�������|�P�������̃t���O
	u16 under_equaldata;	
	u16 top_equaldata;	
	

	// ����f�[�^
	const WLDTIMER_POKEBLN_MOVEDATA* cp_data;

} WLDTIMER_POKEBLN_MOVE;


//-------------------------------------
///	�|�P�����@���D
//=====================================
typedef struct {
	// �f�[�^
	WLDTIMER_POKEBLN_MOVE	move[ WLDTIMER_VIEWER_DRAWNUM ];
	s16						drawtype;		// ���݂̕\���^�C�v	
	s16						wait;			// ���̊J�n����
	
	// �`��t���O
	u8					drawflag[4];	// �㒆��
	u8					pokegra[4];		// �`��|�P����

	// �O���t�B�b�N
	CLACT_WORK_PTR		p_act[ WLDTIMER_TIME_POKE_NUM ];
	CLACT_U_RES_OBJ_PTR	p_res[ WLDTIMER_TIME_POKE_NUM ][ WLDTIMER_RESMAN_NUM ];
} WLDTIMER_POKEBALLOON;

//-------------------------------------
///	�T�u��ʃE�B���h�E���[�N
//=====================================
typedef struct {
	s16 wnd0;		// �f�[�^�ݒ�t���O
	s16 wnd1;		// �f�[�^�ݒ�t���O
	s16 wnd0_sy;
	s16 wnd0_ey;
	s16 wnd1_sy;
	s16 wnd1_ey;

	TCB_PTR p_tcb;	// V�u�����N�^�X�N
} WLDTIMER_VWND;


//-------------------------------------
///	�T�u��ʏ��E�C���h�E
//=====================================
typedef struct {
	// �V�[�P���X
	u32 seq;
	
	// �f�[�^�L���[
	WLDTIMER_POINTDATA	data[ WLDTIMER_VIEWER_BUFFNUM ];
	u16 top;		// �擪
	u16	tail;		// ����
	u32 datacount;	// �������Ă��錏��

	// �t�F�[�h
	u16						fadecount;	// �t�F�[�h�J�E���^
	u16						fade_divnum;// �������ăt�F�[�h���Ă�����
	WLDTIMER_VIEWER_FADE	fade[ WLDTIMER_VIEWER_FADE_DIV ];
	u32						drawtype_zonetype[ WLDTIMER_VIEWER_DRAWNUM ];
	void*					p_fadescrnbuff;	// �t�F�[�h�p�X�N���[���A�j���o�b�t�@
	NNSG2dScreenData*		p_fadescrndata;	// �t�F�[�h�p�X�N���[���t�@�C��

	// �`��f�[�^
	u16						msg_now;						// ���̃��b�Z�[�W
	u16						pad;							// �A�j���J�E���^
	WLDTIMER_TIMEZONEANM	anm[ WLDTIMER_ZONETYPE_NUM ];	// �^�C���]�[���A�j���f�[�^
	GF_BGL_BMPWIN			msg[ WLDTIMER_VIEWER_DRAWNUM ];	// ���b�Z�[�W�f�[�^
	GF_BGL_BMPWIN			dummy;							// �t�F�[�h�p���b�Z�[�W
	GF_BGL_BMPWIN			talkwin;						// ��b���b�Z�[�W�f�[�^
	WLDTIMER_POKEBALLOON	poke;							// �|�P����BALLOON
	WLDTIMER_VWND			wnd;							// �E�B���h�E
} WLDTIMER_VIEWER;



//-------------------------------------
///	�^�b�`�Ǘ�
//=====================================
typedef struct {
	int tp_result;	// �^�b�`���p�b�h���ɕύX��������
	int tp_seq;		// �^�b�`�V�[�P���X
	int tp_x;		// �^�b�`�����W
	int tp_y;		// �^�b�`�����W
	int tp_lenx;	// �X���C�h�����W
	int tp_leny;	// �X���C�h�����W
	int tp_count;	// �z�[���h�J�E���g

	// �{�^���\��
	GF_BGL_BMPWIN bttn;

} WLDTIMER_TOUCH;


//-------------------------------------
///	�I���\��
//=====================================
typedef struct {
	u32				seq;
	u32				msg_no;
	u32				msg_wait;
	STRBUF*			p_str;
	GF_BGL_BMPWIN	win;
	TOUCH_SW_SYS*	p_touch_sw;
} WLDTIMER_END_MSG;


//-------------------------------------
///	���b�Z�[�W�V�X�e��
//=====================================
typedef struct {
	MSGDATA_MANAGER*	p_msgman;		// ��{���b�Z�[�W
	WORDSET*			p_wordset;		// ���[�h�Z�b�g
	STRBUF*				p_msgstr;		// ���b�Z�[�W�o�b�t�@
	STRBUF*				p_msgtmp;		// ���b�Z�[�W�o�b�t�@
} WLDTIMER_MSGMAN;



//-------------------------------------
///	���E���v���[�N
//=====================================
typedef struct {

	// ����Ǘ�
	u16				main_seq;	// ���C����ʃV�[�P���X
	u16				sub_seq;	// �T�u��ʃV�[�P���X
	WLDTIMER_FLAG	flag;		// ����t���O
	

	//�Z�[�u�f�[�^�|�C���^
	WIFI_HISTORY*	p_wifisv;
	CONFIG*			p_config;

	// �����̓o�^�n��
	int		my_nation;
	int		my_area;

	// ���E����
	WFLBY_TIME	worldtime;

	// �\���V�X�e��
	WLDTIMER_DRAWSYS drawsys;

	// ���b�Z�[�W�Ǘ��V�X�e��
	WLDTIMER_MSGMAN msgman;

	// �n���V�Ǘ�
	WLDTIMER_EARTH earth;

	// �n�_���X�g
	WLDTIMER_PLACE place;
	
	// �J����
	WLDTIMER_CAMERA camera;

	// �^�b�`�Ǘ�
	WLDTIMER_TOUCH touch;

	// ���r���[�A�[
	WLDTIMER_VIEWER view;

	// ������ʏI���`�F�b�N
	WLDTIMER_END_MSG end_msg;

} WLDTIMER_WK;






//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	src/application/wifi_earth/wifi_earth����R�s�[
//	�����A���S�ȃR�s�[�ł͂Ȃ��A�\���̂̍\���ȂǕύX���Ă���܂��B
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthListLoad( WLDTIMER_PLACE* p_wk, const WFLBY_WLDTIMER* cp_data, u32 heapID );
static void WLDTIMER_EarthListSet( WLDTIMER_PLACE* p_wk,u32 index, s16 x,s16 y,u16 nationID,u16 areaID, const WFLBY_WLDTIMER* cp_data );

static void WLDTIMER_EarthVecFx32_to_MtxFx33( MtxFx33* dst, const VecFx32* src );
static void WLDTIMER_EarthVecFx32_to_MtxFx33_place( MtxFx33* dst, const VecFx32* src );
static void WLDTIMER_Earth_TouchPanelParamGet( int prevx,int prevy,int* dirx_p,int* lenx_p,int* diry_p,int* leny_p );
static u32 WLDTIMER_Earth3D_Control( WLDTIMER_WK* p_wk,int keytrg,int keycont );

//-----------------------------------------------------------------------------


// �c�[��
static void WLDTIMER_MinusRotateChange( VecFx32* p_rot );
static BOOL WLDTIMER_RotateCheck( s32 minx, s32 maxx, u16 rotx );

// �S��
static BOOL WLDTIMER_WkMainControl( WLDTIMER_WK* p_wk );
static void WLDTIMER_WkSubControl( WLDTIMER_WK* p_wk );
static void WLDTIMER_WkDraw( WLDTIMER_WK* p_wk );
static void WLDTIMER_WkVBlank( void* p_work );

// �n���V�Ǘ�
static u32 WLDTIMER_EarthControl( WLDTIMER_WK* p_wk );
static void WLDTIMER_EarthStartSetUp( WLDTIMER_WK* p_wk, const WLDTIMER_PARAM* cp_param );
static BOOL WLDTIMER_EarthGetPointData( const WLDTIMER_WK* cp_wk, WLDTIMER_POINTDATA* p_data );
static u32 WLDTIMER_EarthGetPlaceData( const WLDTIMER_WK* cp_wk );
static u32 WLDTIMER_EarthGetRotateDist( const VecFx32* cp_earth, const VecFx32* cp_place );
static u32 WLDTIMER_EarthGetTimeZone( const WLDTIMER_WK* cp_wk, u32 placeindex, WFLBY_TIME worldtime );
static BOOL WLDTIMER_EarthSetNationAreaRotate( WLDTIMER_WK* p_wk, u8 nationID, u8 areaID );

// ����t���O 
static void WLDTIMER_FlagControl( WLDTIMER_WK* p_wk, const WLDTIMER_PARAM* cp_param );

// �\���V�X�e��
static void WLDTIMER_DrawSysInit( WLDTIMER_DRAWSYS* p_wk, CONFIG* p_config, u32 heapID );
static void WLDTIMER_DrawSysExit( WLDTIMER_DRAWSYS* p_wk );
static void WLDTIMER_DrawSysDraw( WLDTIMER_DRAWSYS* p_wk );
static void WLDTIMER_DrawSysVBlank( WLDTIMER_DRAWSYS* p_wk );
static void WLDTIMER_DrawSysBgInit( WLDTIMER_DRAWSYS* p_wk, CONFIG* p_config, u32 heapID );
static void WLDTIMER_DrawSysBgExit( WLDTIMER_DRAWSYS* p_wk );
static void WLDTIMER_DrawSysOamInit( WLDTIMER_DRAWSYS* p_wk, u32 heapID );
static void WLDTIMER_DrawSysOamExit( WLDTIMER_DRAWSYS* p_wk );
static void WLDTIMER_DrawSys3DInit( WLDTIMER_DRAWSYS* p_wk, u32 heapID );
static void WLDTIMER_DrawSys3DExit( WLDTIMER_DRAWSYS* p_wk );
static void WLDTIMER_DrawSys3DSetUp( void );

// �n���V�f�[�^
static void WLDTIMER_EarthInit( WLDTIMER_EARTH* p_wk, WLDTIMER_DRAWSYS* p_drawsys, u32 heapID );
static void WLDTIMER_EarthExit( WLDTIMER_EARTH* p_wk );
static void WLDTIMER_EarthDraw( WLDTIMER_EARTH* p_wk );
static void WLDTIMER_EarthGetTrans( const WLDTIMER_EARTH* cp_wk, VecFx32* p_mat );
static void WLDTIMER_EarthGetRotMtx( const WLDTIMER_EARTH* cp_wk, MtxFx33* p_rot );
static void WLDTIMER_EarthGetRotaVec( const WLDTIMER_EARTH* cp_wk, VecFx32* p_vec );
static void WLDTIMER_EarthSetRotaVec( WLDTIMER_EARTH* p_wk, const VecFx32* cp_vec );

// �n��f�[�^
static void WLDTIMER_PlaceInit( WLDTIMER_PLACE* p_wk, const WFLBY_WLDTIMER* cp_data, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_FLAG flag, u32 heapID );
static void WLDTIMER_PlaceExit( WLDTIMER_PLACE* p_wk );
static void WLDTIMER_PlaceDraw( WLDTIMER_PLACE* p_wk, const WLDTIMER_EARTH* cp_earth );
static u32 WLDTIMER_PlaceGetCol( const WFLBY_WLDTIMER* cp_data, u16 nationID, u16 areaID );
static u32 WLDTIMER_PlaceGetData( const WLDTIMER_PLACE* cp_wk, u16 nationID, u16 areaID, VecFx32* p_vec );
static u32 WLDTIMER_PlaceGetIdx( const WLDTIMER_PLACE* cp_wk, u16 nationID, u16 areaID );
static void WLDTIMER_PlaceGetIdxRotVec( const WLDTIMER_PLACE* cp_wk, VecFx32* p_vec, int index );
static u32 WLDTIMER_PlaceGetIdxCol( const WLDTIMER_PLACE* cp_wk, int index );
static u32 WLDTIMER_PlaceGetIdxNationID( const WLDTIMER_PLACE* cp_wk, int index );
static u32 WLDTIMER_PlaceGetIdxAreaID( const WLDTIMER_PLACE* cp_wk, int index );
static u32 WLDTIMER_PlaceGetListNum( const WLDTIMER_PLACE* cp_wk );

// �J�����f�[�^
static void WLDTIMER_CameraInit( WLDTIMER_CAMERA* p_wk, WLDTIMER_FLAG flag, u32 heapID );
static void WLDTIMER_CameraExit( WLDTIMER_CAMERA* p_wk );
static void WLDTIMER_CameraDraw( const WLDTIMER_CAMERA* cp_wk );
static void WLDTIMER_CameraMoveReq( WLDTIMER_CAMERA* p_wk );
static BOOL WLDTIMER_CameraMove( WLDTIMER_CAMERA* p_wk, WLDTIMER_PLACE* p_place );
static u32	WLDTIMER_CameraGetStatus( const WLDTIMER_CAMERA* cp_wk );

// �^�b�`�Ǘ�
static void WLDTIMER_TouchInit( WLDTIMER_TOUCH* p_wk, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_MSGMAN* p_msgman, u32 heapID );
static void WLDTIMER_TouchExit( WLDTIMER_TOUCH* p_wk );
static void WLDTIMER_TouchSetParam( WLDTIMER_TOUCH* p_touch );
static int WLDTIMER_TouchGetParam( const WLDTIMER_TOUCH* cp_touch, u32 type );
static void WLDTIMER_TouchBttnOff( WLDTIMER_TOUCH* p_wk );
static void WLDTIMER_TouchBttnOn( WLDTIMER_TOUCH* p_wk );

// �I���`�F�b�N�Ǘ�
static void WLDTIMER_EndMsgInit( WLDTIMER_END_MSG* p_wk, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_MSGMAN* p_msgman, SAVEDATA* p_save, u32 heapID );
static void WLDTIMER_EndMsgExit( WLDTIMER_END_MSG* p_wk );
static void WLDTIMER_EndMsgStart( WLDTIMER_END_MSG* p_wk );
static u32 WLDTIMER_EndMsgMain( WLDTIMER_END_MSG* p_wk );
static void WLDTIMER_EndMsgEnd( WLDTIMER_END_MSG* p_wk );


// �T�u��ʏ��E�B���h�E
static void WLDTIMER_ViewerInit( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_MSGMAN* p_msgman, u32 heapID );
static void WLDTIMER_ViewerExit( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys );
static void WLDTIMER_ViewerMain( WLDTIMER_VIEWER* p_wk, WLDTIMER_MSGMAN* p_msgman, WLDTIMER_DRAWSYS* p_drawsys );
static void WLDTIMER_ViewerTalkWinOff( WLDTIMER_VIEWER* p_wk );
static void WLDTIMER_ViewerTalkWinOn( WLDTIMER_VIEWER* p_wk );
static BOOL WLDTIMER_ViewerPushData( WLDTIMER_VIEWER* p_wk, const WLDTIMER_POINTDATA* cp_data );
static BOOL WLDTIMER_ViewerEqualDataCheck( const WLDTIMER_VIEWER* cp_wk, const WLDTIMER_POINTDATA* cp_data );
static void WLDTIMER_ViewerQInit( WLDTIMER_VIEWER* p_wk );
static void WLDTIMER_ViewerQExit( WLDTIMER_VIEWER* p_wk );
static void WLDTIMER_ViewerQPush( WLDTIMER_VIEWER* p_wk, const WLDTIMER_POINTDATA* cp_data );
static void WLDTIMER_ViewerQPop( WLDTIMER_VIEWER* p_wk );
static BOOL WLDTIMER_ViewerQGetData( const WLDTIMER_VIEWER* cp_wk, WLDTIMER_POINTDATA* p_data, u32 index );
static void WLDTIMER_ViewerAnmCont( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys );
static void WLDTIMER_ViewerFadeInit( WLDTIMER_VIEWER* p_wk, WLDTIMER_MSGMAN* p_msgman, WLDTIMER_DRAWSYS* p_drawsys );
static BOOL WLDTIMER_ViewerFade( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys );
static void WLDTIMER_ViewerFade_SkipInit( WLDTIMER_VIEWER* p_wk );
static BOOL WLDTIMER_ViewerFade_Skip( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys );
static void WLDTIMER_ViewerFade_SkipInit01( WLDTIMER_VIEWER* p_wk );
static BOOL WLDTIMER_ViewerFade_Skip01( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys );
static BOOL WLDTIMER_ViewerFade_Main( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys, int fade_timing );
static void WLDTIMER_ViewerFadeDiv_Start( WLDTIMER_VIEWER* p_wk, u32 idx );
static BOOL WLDTIMER_ViewerFadeDiv_Main( WLDTIMER_VIEWER* p_wk, u32 idx, WLDTIMER_DRAWSYS* p_drawsys );
static void WLDTIMER_ViewerFadeScrn_LineTrans( WLDTIMER_VIEWER* p_wk, u32 y, WLDTIMER_DRAWSYS* p_drawsys );
static void WLDTIMER_ViewerWndInit( WLDTIMER_VWND* p_wk );
static void WLDTIMER_ViewerWndExit( WLDTIMER_VWND* p_wk );
static void WLDTIMER_ViewerWnd0SetPos( WLDTIMER_VWND* p_wk, s16 sy, s16 ey );
static void WLDTIMER_ViewerWnd1SetPos( WLDTIMER_VWND* p_wk, s16 sy, s16 ey );
static void WLDTIMER_ViewerWndTcb( TCB_PTR p_tcb, void* p_work );


// �������񏑂�����
static void WLDTIMER_ViewerMsgInit( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys, u32 heapID );
static void WLDTIMER_ViewerMsgExit( WLDTIMER_VIEWER* p_wk ); 
static void WLDTIMER_ViewerMsgWrite( WLDTIMER_VIEWER* p_wk, u32 drawtype, const WLDTIMER_POINTDATA* cp_data, WLDTIMER_MSGMAN* p_msgman );
static u8* WLDTIMER_ViewerMsgGetCharBuffPtr( WLDTIMER_VIEWER* p_wk, u32 drawtype, u32 y );
static u8* WLDTIMER_ViewerDummyGetCharBuffPtr( WLDTIMER_VIEWER* p_wk, u32 y );
static void WLDTIMER_ViewerMsgCharTrans( WLDTIMER_VIEWER* p_wk, u32 drawtype, u32 y, const u8* cp_buff );


// �^�C���]�[���A�j��
static void WLDTIMER_TimeZoneAnm_Init( WLDTIMER_TIMEZONEANM* p_wk, WLDTIMER_DRAWSYS* p_drawsys, const WLDTIMER_TIMEZONEANM_INIT* cp_init, u16 count_max, u32 heapID );
static void WLDTIMER_TimeZoneAnm_Exit( WLDTIMER_TIMEZONEANM* p_wk );
static void WLDTIMER_TimeZoneAnm_Main( WLDTIMER_TIMEZONEANM* p_wk, WLDTIMER_DRAWSYS* p_drawsys );
static void WLDTIMER_TimeZoneAnm_SetFlag( WLDTIMER_TIMEZONEANM* p_wk, u32 drawtype, BOOL flag );
static void WLDTIMER_TimeZoneAnm_ResetFlag( WLDTIMER_TIMEZONEANM* p_wk );
static void WLDTIMER_TimeZoneAnm_LineTrans( WLDTIMER_TIMEZONEANM* p_wk, u32 y, WLDTIMER_DRAWSYS* p_drawsys );


// �|�P�����o���[��
static void WLDTIMER_PokeBaloon_Init( WLDTIMER_POKEBALLOON* p_wk, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_VWND* p_wnd, u32 heapID );
static void WLDTIMER_PokeBaloon_Exit( WLDTIMER_POKEBALLOON* p_wk, WLDTIMER_DRAWSYS* p_drawsys );
static void WLDTIMER_PokeBaloon_Reset( WLDTIMER_POKEBALLOON* p_wk );
static void WLDTIMER_PokeBaloon_SetDraw( WLDTIMER_POKEBALLOON* p_wk, u8 drawtype, u8 timezone, u8 last_timezone );
static void WLDTIMER_PokeBaloon_CleanDraw( WLDTIMER_POKEBALLOON* p_wk );
static void WLDTIMER_PokeBaloon_Main( WLDTIMER_POKEBALLOON* p_wk, WLDTIMER_VWND* p_wnd );
static BOOL WLDTIMER_PokeBaloon_Start( WLDTIMER_POKEBALLOON* p_wk, u32 drawtype, WLDTIMER_VWND* p_wnd );
static BOOL WLDTIMER_PokeBaloon_Move( WLDTIMER_POKEBALLOON* p_wk, u32 drawtype, WLDTIMER_VWND* p_wnd );
static void WLDTIMER_PokeBln_MoveInit( WLDTIMER_POKEBLN_MOVE* p_wk, const WLDTIMER_POKEBLN_MOVEDATA* cp_data );
static void WLDTIMER_PokeBln_UnderEqualDataSet( WLDTIMER_POKEBLN_MOVE* p_wk, BOOL flag );
static void WLDTIMER_PokeBln_TopEqualDataSet( WLDTIMER_POKEBLN_MOVE* p_wk, BOOL flag );
static BOOL WLDTIMER_PokeBln_MoveMain( WLDTIMER_POKEBLN_MOVE* p_wk, WLDTIMER_VWND* p_wnd );
static void WLDTIMER_PokeBln_MoveReset( WLDTIMER_POKEBLN_MOVE* p_wk, WLDTIMER_VWND* p_wnd );
static void WLDTIMER_PokeBln_MoveGetPos( const WLDTIMER_POKEBLN_MOVE* cp_wk, VecFx32* p_pos );
static void WLDTIMER_PokeBln_MatrixCalc( WLDTIMER_POKEBLN_MOVE* p_wk );
static void WLDTIMER_PokeBln_WndMskSet( const WLDTIMER_POKEBLN_MOVE* cp_wk, WLDTIMER_VWND* p_wnd );
static void WLDTIMER_PokeBln_ActSetMatrix( WLDTIMER_POKEBALLOON* p_wk, u32 drawtype );


// �^�C���]�[���f�[�^�擾
static u16 WLDTIMER_TIMEZONE_GetZoneType( u32 timezone );
static u16 WLDTIMER_TIMEZONE_GetMons( u32 timezone );

// ���b�Z�[�W�V�X�e��
static void WLDTIMER_MsgManInit( WLDTIMER_MSGMAN* p_wk, u32 heapID );
static void WLDTIMER_MsgManExit( WLDTIMER_MSGMAN* p_wk );
static STRBUF* WLDTIMER_MsgManGetStr( WLDTIMER_MSGMAN* p_wk, u32 msg );
static STRBUF* WLDTIMER_MsgManCountryGetStr( WLDTIMER_MSGMAN* p_wk, u32 nationID );
static STRBUF* WLDTIMER_MsgManPlaceGetStr( WLDTIMER_MSGMAN* p_wk, u32 nationID, u32 areaID );
static void WLDTIMER_MsgManGetStrBuff( WLDTIMER_MSGMAN* p_wk, u32 msg, STRBUF* p_str );

// �\���f�[�^�x�[�X
static u32 WLDTIMER_WFLBYDATA_GetFirst( const WFLBY_WLDTIMER* cp_data );




//----------------------------------------------------------------------------
/**
 *	@brief	���E���v	������
 *
 *	@param	p_proc		�v���b�N���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WLDTIMER_Init(PROC* p_proc, int* p_seq)
{
	WLDTIMER_WK* p_wk;
	WLDTIMER_PARAM* p_param;

	// �S�n��\���Ńo�b�N�@�\
#ifdef WLDTIMER_DEBUG_ALLPLACEOPEN
	if( sys.cont & PAD_BUTTON_R ){
		s_WLDTIMER_DEBUG_ALLPLACEOPEN_FLAG = TRUE;
	}else{
		s_WLDTIMER_DEBUG_ALLPLACEOPEN_FLAG = FALSE;
	}
#endif
	
	p_param = PROC_GetParentWork( p_proc );

	OS_TPrintf( "end cgx %d\n", WLDTIMER_MAIN_SUBBTTN_CGXEND );

	//�q�[�v�G���A�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WLDTIMER, 0x50000 );

	// ���[�N�쐬
	p_wk = PROC_AllocWork( p_proc, sizeof(WLDTIMER_WK), HEAPID_WLDTIMER );
	memset( p_wk, 0, sizeof(WLDTIMER_WK) );

	// �Z�[�u�f�[�^�擾
	p_wk->p_wifisv = SaveData_GetWifiHistory( p_param->p_save );
	p_wk->p_config = SaveData_GetConfig( p_param->p_save );

	// �����̓o�^�n��擾
	p_wk->my_nation = WIFIHISTORY_GetMyNation( p_wk->p_wifisv );
	p_wk->my_area = WIFIHISTORY_GetMyArea( p_wk->p_wifisv );

	// ���E����
	p_wk->worldtime = p_param->worldtime;

//	OS_TPrintf( "world time hour[%d] minute[%d] second[%d]\n", p_wk->worldtime.hour, p_wk->worldtime.minute, p_wk->worldtime.second );

	// �t���O������
	WLDTIMER_FlagControl( p_wk, p_param );

	// �O���t�B�b�N������
	WLDTIMER_DrawSysInit( &p_wk->drawsys, p_wk->p_config, HEAPID_WLDTIMER );

	// �n���V������
	WLDTIMER_EarthInit( &p_wk->earth, &p_wk->drawsys, HEAPID_WLDTIMER );

	// �����V�X�e��������
	WLDTIMER_MsgManInit( &p_wk->msgman, HEAPID_WLDTIMER );

	// �n�揉����
	WLDTIMER_PlaceInit( &p_wk->place, p_param->cp_data, &p_wk->drawsys, p_wk->flag, HEAPID_WLDTIMER );

	// �J����������
	WLDTIMER_CameraInit( &p_wk->camera, p_wk->flag, HEAPID_WLDTIMER );

	// �^�b�`������
	WLDTIMER_TouchInit( &p_wk->touch, &p_wk->drawsys, &p_wk->msgman, HEAPID_WLDTIMER );

	// �I���`�F�b�N������
	WLDTIMER_EndMsgInit( &p_wk->end_msg, &p_wk->drawsys, &p_wk->msgman, p_param->p_save, HEAPID_WLDTIMER );

	// �r���[�A�[������
	WLDTIMER_ViewerInit( &p_wk->view, &p_wk->drawsys, &p_wk->msgman, HEAPID_WLDTIMER );
	
	// �n���V�J�n�Z�b�g�A�b�v
	WLDTIMER_EarthStartSetUp( p_wk, p_param );

	
	// ���荞�ݐݒ�
	sys_VBlankFuncChange( WLDTIMER_WkVBlank, p_wk );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	return	PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���v	���C��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WLDTIMER_Main(PROC* p_proc, int* p_seq)
{
	WLDTIMER_WK* p_wk;
	WLDTIMER_PARAM* p_param;
	BOOL result;
	
	p_wk	= PROC_GetWork( p_proc );
	p_param = PROC_GetParentWork( p_proc );



#ifdef WFLBY_DEBUG_ROOM_WLDTIMER_AUTO
	WFLBY_DEBUG_ROOM_WFLBY_TIMER_AUTO = TRUE;
	sys.trg		|= PAD_BUTTON_B;
	sys.cont	|= PAD_BUTTON_B;
#endif
	

	switch( *p_seq ){
	case WLDTIMER_SEQ_FADEIN:
		WIPE_SYS_Start(WIPE_PATTERN_WMS, 
				WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WLDTIMER );

		// BGM���t�F�[�h�A�E�g
		WFLBY_SYSTEM_SetBGMVolumeDown( p_param->p_system, TRUE );

		// ���r�[�ɓ����Ă��������o��
//		Snd_SePlay( WFLBY_SND_WLDTMIN );

		(*p_seq) ++;
		break;
		
	case WLDTIMER_SEQ_FADEINWAIT:
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){
			(*p_seq) ++;
		}
		break;
		
	case WLDTIMER_SEQ_MAIN:
		result = WLDTIMER_WkMainControl( p_wk );
		WLDTIMER_WkSubControl( p_wk );

		if( WFLBY_SYSTEM_Event_GetEndCM( p_param->p_system ) == TRUE ){
			WFLBY_SYSTEM_APLFLAG_SetForceEnd( p_param->p_system );
			result = TRUE;
		}

		if( WFLBY_ERR_CheckError() == TRUE ){
			result = TRUE;
		}
		
		if( result == TRUE ){	// �I���`�F�b�N
			(*p_seq) = WLDTIMER_SEQ_FADEOUT;
		}
		break;
		
	case WLDTIMER_SEQ_FADEOUT:
		WIPE_SYS_Start(WIPE_PATTERN_WMS, 
				WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WLDTIMER );
		(*p_seq) ++;
		break;
		
	case WLDTIMER_SEQ_FADEOUTWAIT:
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){
			return PROC_RES_FINISH;
		}
		break;
	}

	// �`��
	WLDTIMER_WkDraw( p_wk );

	return	PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���v	�j��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WLDTIMER_Exit(PROC* p_proc, int* p_seq)
{
	WLDTIMER_WK* p_wk;
	WLDTIMER_PARAM* p_param;
	
	p_wk	= PROC_GetWork( p_proc );
	p_param = PROC_GetParentWork( p_proc );

	// ���荞�ݐݒ�
	sys_VBlankFuncChange( NULL, NULL );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	// �r���[�A�[�j��
	WLDTIMER_ViewerExit( &p_wk->view, &p_wk->drawsys );

	// �I���`�F�b�N�j��
	WLDTIMER_EndMsgExit( &p_wk->end_msg );

	// �^�b�`�j��
	WLDTIMER_TouchExit( &p_wk->touch );

	// �J�����j��
	WLDTIMER_CameraExit( &p_wk->camera );

	// �n���V�j��
	WLDTIMER_EarthExit( &p_wk->earth );

	// �n��f�[�^�j��
	WLDTIMER_PlaceExit( &p_wk->place );

	// �����V�X�e���j��
	WLDTIMER_MsgManExit( &p_wk->msgman );

	// �`��V�X�e���j��
	WLDTIMER_DrawSysExit( &p_wk->drawsys );

	//���[�N�j��
	PROC_FreeWork( p_proc );
	
	//�q�[�v�j��
	sys_DeleteHeap( HEAPID_WLDTIMER );

	return	PROC_RES_FINISH;
}




#ifdef  PM_DEBUG
PROC_RESULT WLDTIMER_DebugInit(PROC* p_proc, int* p_seq)
{

	return WLDTIMER_Init( p_proc, p_seq );
}

PROC_RESULT WLDTIMER_DebugExit(PROC* p_proc, int* p_seq)
{
	PROC_RESULT result;

	result = WLDTIMER_Exit( p_proc, p_seq );

	return result;
}
#endif



//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	src/application/wifi_earth/wifi_earth����R�s�[
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�n�_���X�g��ǂݍ���
 *
 *	@param	p_wk	���[�N
 *	@param	cp_data	�\���n��f�[�^�o�b�t�@
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthListLoad( WLDTIMER_PLACE* p_wk, const WFLBY_WLDTIMER* cp_data, u32 heapID )
{
	ARCHANDLE* p_handle = ArchiveDataHandleOpen( ARC_WIFI_EARCH_PLACE, heapID );
	
	//�n�_���X�g����������
	p_wk->placelist.listcount = 0;

	{//�n�_�}�[�N��]�������i���f�[�^�o�C�i���f�[�^���[�h�j
		void* filep;
		EARTH_DATA_NATION* listp;
		u32	size;
		int	i,listcount;

		filep = ArcUtil_HDL_LoadEx( p_handle, NARC_wifi_earth_place_place_pos_wrd_dat, 
								FALSE, heapID, ALLOC_TOP, &size );

		listp = (EARTH_DATA_NATION*)filep;	//�t�@�C���ǂݍ��ݗp�ɕϊ�
		listcount = size/6;				//�n�_���擾�i�f�[�^���F�P�n�_�ɂ��U�o�C�g�j

		listp++;	//1origin�̂��ߓǂݔ�΂�
		for(i=1;i<listcount;i++){
			if(listp->flag != 2){	//2�̏ꍇ�͒n�惊�X�g�����݂���
				WLDTIMER_EarthListSet(p_wk,p_wk->placelist.listcount,
						listp->x,listp->y,i,0, cp_data);
				p_wk->placelist.listcount++;
			}
			listp++;
		}
		sys_FreeMemoryEz(filep);
	}
	{//�n�_�}�[�N��]�������i�n��f�[�^�o�C�i���f�[�^���[�h�j
		void* filep;
		EARTH_DATA_AREA* listp;
		u32	size, data_id;
		int	i,datLen,index,listcount;

		index = 1;	//1orgin
		datLen = WIFI_COUNTRY_GetDataLen();

		while(index < datLen){

			data_id = WIFI_COUNTRY_DataIndexToPlaceDataID( index );
			filep = ArcUtil_HDL_LoadEx( p_handle, data_id, FALSE, 
									heapID, ALLOC_TOP, &size );

			listp = (EARTH_DATA_AREA*)filep;	//�t�@�C���ǂݍ��ݗp�ɕϊ�
			listcount = size/4;		//�n�_���擾�i�f�[�^���F�P�n�_�ɂ��S�o�C�g�j

			listp++;	//1origin�̂��ߓǂݔ�΂�
			for(i=1;i<listcount;i++){//1orgin
				WLDTIMER_EarthListSet(p_wk,p_wk->placelist.listcount,listp->x,listp->y,
								WIFI_COUNTRY_DataIndexToCountryCode(index),i, cp_data);
				p_wk->placelist.listcount++;
				listp++;
			}
			sys_FreeMemoryEz(filep);
			index++;
		}
	}

	ArchiveDataHandleClose( p_handle );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n�_�������X�g�ɐݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	index		�C���f�b�N�X
 *	@param	x			�����Ή�]�p�x
 *	@param	y			�����Ή�]�p�x
 *	@param	nationID	��ID
 *	@param	areaID		�n��ID
 *	@param	cp_data		�\���n��f�[�^�o�b�t�@
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthListSet( WLDTIMER_PLACE* p_wk,u32 index, s16 x,s16 y,u16 nationID,u16 areaID, const WFLBY_WLDTIMER* cp_data )
{
	MtxFx33 rotMtx = {FX32_ONE,0,0,0,FX32_ONE,0,0,0,FX32_ONE};
	VecFx32 rotVec;

	p_wk->placelist.place[index].x = x;	//�w��]�I�t�Z�b�g�擾
	p_wk->placelist.place[index].y = y;	//�x��]�I�t�Z�b�g�擾
	
	rotVec.x = x;
	rotVec.y = y;
	rotVec.z = INIT_EARTH_ROTATE_ZVAL;
	WLDTIMER_EarthVecFx32_to_MtxFx33_place(&rotMtx,&rotVec);	//�����ʒu����̑��΍��W�v�Z
	p_wk->placelist.place[index].rotate = rotMtx;

	//�}�[�N�F�ݒ�(�ގ��t���O�ɂ��킹��)
#ifdef WLDTIMER_DEBUG_ALLPLACEOPEN
	if( s_WLDTIMER_DEBUG_ALLPLACEOPEN_FLAG == TRUE ){
		p_wk->placelist.place[index].col = WLDTIMER_PLACE_COL_IN;
	}else{
		if( cp_data != NULL ){
			p_wk->placelist.place[index].col = WLDTIMER_PlaceGetCol( cp_data, nationID, areaID );
		}
	}
#else
	if( cp_data != NULL ){
		p_wk->placelist.place[index].col = WLDTIMER_PlaceGetCol( cp_data, nationID, areaID );
	}
#endif
//	OS_Printf( "p_wk->placelist.place[index].col %d  nation %d  area %d\n", p_wk->placelist.place[index].col, nationID, areaID );

	p_wk->placelist.place[index].nationID = nationID;	//�Y�����h�c
	p_wk->placelist.place[index].areaID = areaID;		//�Y���n��h�c
}


//----------------------------------------------------------------------------
/**
 *	@brief	3D��]�v�Z
 *
 *	@param	dst		�����o����s��
 *	@param	src		��]�x�N�g��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthVecFx32_to_MtxFx33( MtxFx33* dst, const VecFx32* src )
{
	MtxFx33 tmp;

	MTX_RotY33(	dst,FX_SinIdx((u16)src->y),FX_CosIdx((u16)src->y));

	MTX_RotX33(	&tmp,FX_SinIdx((u16)src->x),FX_CosIdx((u16)src->x));
	MTX_Concat33(dst,&tmp,dst);

	MTX_RotZ33(	&tmp,FX_SinIdx((u16)src->z),FX_CosIdx((u16)src->z));
	MTX_Concat33(dst,&tmp,dst);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD��]�v�Z	���Ή�]
 *
 *	@param	dst		�����o����s��
 *	@param	src		��]�x�N�g��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthVecFx32_to_MtxFx33_place( MtxFx33* dst, const VecFx32* src )
{
	MtxFx33 tmp;

	MTX_RotY33(	dst,FX_SinIdx((u16)src->x),FX_CosIdx((u16)src->x));

	MTX_RotX33(	&tmp,FX_SinIdx((u16)-src->y),FX_CosIdx((u16)-src->y));
	MTX_Concat33(dst,&tmp,dst);

	MTX_RotZ33(	&tmp,FX_CosIdx((u16)src->z),FX_SinIdx((u16)src->z));
	MTX_Concat33(dst,&tmp,dst);
}

static void WLDTIMER_Earth_TouchPanelParamGet( int prevx,int prevy,int* dirx_p,int* lenx_p,int* diry_p,int* leny_p )
{
	int x_dir = 0;
	int y_dir = 0;
	int x_len = 0;
	int y_len = 0;

	//�w�������ړ����擾
	if(sys.tp_x != 0xffff){
		x_len = sys.tp_x - prevx;
		if(x_len < 0){
			x_len ^= -1;
			x_dir = PAD_KEY_RIGHT;
		}else{
			if(x_len > 0){
				x_dir = PAD_KEY_LEFT;
			}
		}
	}
	x_len &= WLDTIMER_TOUCH_LEN_LIMIT;	//���~�b�^�[
	*dirx_p = x_dir;
	*lenx_p = x_len;

	//�x�������ړ����擾
	if(sys.tp_y != 0xffff){
		y_len = sys.tp_y - prevy;
		if(y_len < 0){
			y_len ^= -1;
			y_dir = PAD_KEY_DOWN;
		}else{
			if(y_len > 0){
				y_dir = PAD_KEY_UP;
			}
		}
	}
	y_len &= WLDTIMER_TOUCH_LEN_LIMIT;	//���~�b�^�[
	*diry_p = y_dir;
	*leny_p = y_len;
}

//----------------------------------
//�n������֐�
//----------------------------------
static u32 WLDTIMER_Earth3D_Control( WLDTIMER_WK* p_wk,int keytrg,int keycont )
{
	u16 rotate_speed_x;
	u16 rotate_speed_y;
	s16 rotate_x;
	s16 rotate_y;
	u32 result = WLDTIMER_EARTHCONT_RET_NONE;
	VecFx32 rotate;
	int tp_result;
	int tp_lenx, tp_leny;
	u32 camera_status;

	// �e�f�[�^�擾
	WLDTIMER_EarthGetRotaVec( &p_wk->earth, &rotate );	// �n���V��]�x�N�g��
	tp_result	= WLDTIMER_TouchGetParam(				// �^�b�`���ʃp�b�h�l
					&p_wk->touch, WLDTIMER_TOUCH_PM_RESULT );
	tp_lenx		= WLDTIMER_TouchGetParam(				// �^�b�`�X���C�h���l
					&p_wk->touch, WLDTIMER_TOUCH_PM_LENX );
	tp_leny		= WLDTIMER_TouchGetParam(				// �^�b�`�X���C�h���l
					&p_wk->touch, WLDTIMER_TOUCH_PM_LENY );
	camera_status = WLDTIMER_CameraGetStatus(			// �J�������
					&p_wk->camera );	

	rotate_x = rotate.x;
	rotate_y = rotate.y;

	//�J�������߈ړ�����i���E�n���V���[�h�̂݁j
	if((keytrg & PAD_BUTTON_A)||(tp_result & PAD_BUTTON_A)){
		if(p_wk->flag.world == GLOBAL_MODE){
			result = WLDTIMER_EARTHCONT_RET_CAMERA_MOVE;
		}
		return result;
	}
	//�J�����ړ��X�s�[�h�����ݒ�
	if(camera_status == CAMERA_FAR){
		//������
		if((tp_lenx)||(tp_leny)){ //�^�b�`�p�l�����͂���
			rotate_speed_x = (CAMERA_MOVE_SPEED_FAR/WLDTIMER_TOUCH_EARTHSPEED_DIVFAR) * tp_lenx;
			rotate_speed_y = (CAMERA_MOVE_SPEED_FAR/WLDTIMER_TOUCH_EARTHSPEED_DIVFAR) * tp_leny;
		}else{
			rotate_speed_x = CAMERA_MOVE_SPEED_FAR;
			rotate_speed_y = CAMERA_MOVE_SPEED_FAR;
		}
	}else{
		//�ߋ���
		if((tp_lenx)||(tp_leny)){ //�^�b�`�p�l�����͂���
			rotate_speed_x = (CAMERA_MOVE_SPEED_NEAR/WLDTIMER_TOUCH_EARTHSPEED_DIVNEAR) * tp_lenx;
			rotate_speed_y = (CAMERA_MOVE_SPEED_NEAR/WLDTIMER_TOUCH_EARTHSPEED_DIVNEAR) * tp_leny;
		}else{
			rotate_speed_x = CAMERA_MOVE_SPEED_NEAR;
			rotate_speed_y = CAMERA_MOVE_SPEED_NEAR;
		}
	}
	//�J�����㉺���E�ړ�����
	if((keycont & PAD_KEY_LEFT)||(tp_result & PAD_KEY_LEFT)){
		if(p_wk->flag.world == GLOBAL_MODE){
			rotate.y += rotate_speed_x;
		}else{
			//if((u16)wk->rotate.y < EARTH_LIMIT_ROTATE_YMAX){
			if(rotate_y < (s16)EARTH_LIMIT_ROTATE_YMAX){
				rotate.y += rotate_speed_x;
			}
		}
	}
	if((keycont & PAD_KEY_RIGHT)||(tp_result & PAD_KEY_RIGHT)){
		if(p_wk->flag.world == GLOBAL_MODE){
			rotate.y -= rotate_speed_x;
		}else{
			//if((u16)wk->rotate.y > EARTH_LIMIT_ROTATE_YMIN){
			if(rotate_y > (s16)EARTH_LIMIT_ROTATE_YMIN){
				rotate.y -= rotate_speed_x;
			}
		}
	}
	if((keycont & PAD_KEY_UP)||(tp_result & PAD_KEY_UP)){
		if(p_wk->flag.world == GLOBAL_MODE){
			//if(((u16)(wk->rotate.x + rotate_speed_y)) < CAMERA_ANGLE_MAX){
			if((rotate_x + rotate_speed_y) < CAMERA_ANGLE_MAX){
				rotate.x += rotate_speed_y;
			}else{
				rotate.x = CAMERA_ANGLE_MAX;
			}
		}else{
			//if((u16)wk->rotate.x < EARTH_LIMIT_ROTATE_XMAX){
			if(rotate_x < (s16)EARTH_LIMIT_ROTATE_XMAX){
				rotate.x += rotate_speed_y;
			}
		}
	}
	if((keycont & PAD_KEY_DOWN)||(tp_result & PAD_KEY_DOWN)){
		if(p_wk->flag.world == GLOBAL_MODE){
			//if(((u16)(wk->rotate.x - rotate_speed_y)) > CAMERA_ANGLE_MIN){
			if((rotate_x - rotate_speed_y) > CAMERA_ANGLE_MIN){
				rotate.x -= rotate_speed_y;
			}else{
				rotate.x = CAMERA_ANGLE_MIN;
			}
		}else{
			//if((u16)wk->rotate.x > EARTH_LIMIT_ROTATE_XMIN){
			if(rotate_x > (s16)EARTH_LIMIT_ROTATE_XMIN){
				rotate.x -= rotate_speed_y;
			}
		}
	}
	WLDTIMER_EarthSetRotaVec( &p_wk->earth, &rotate );
	return result;
}
//------------------------------------------------------------------------------


//----------------------------------------------------------------------------
/**
 *	@brief	�}�C�i�X�̒l�̉�]�p�x��R�U�O�x�ȏ�̉�]�p�x���Ȃ���
 *
 *	@param	p_rot	��]�x�N�g��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_MinusRotateChange( VecFx32* p_rot )
{
	if( p_rot->x >= 0 ){
		p_rot->x = p_rot->x % 0xffff;
	}else{
		p_rot->x = p_rot->x + ( 0xffff* ((MATH_ABS( p_rot->x )/0xffff)+1) );
	}
	if( p_rot->y >= 0 ){
		p_rot->y = p_rot->y % 0xffff;
	}else{
		p_rot->y = p_rot->y + ( 0xffff* ((MATH_ABS( p_rot->y )/0xffff)+1) );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�p�x�`�F�b�N
 *
 *	@retval	TRUE	�͂����Ă���
 *	@retval	FALSE	�͂����Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_RotateCheck( s32 minx, s32 maxx, u16 rotx )
{
	if( minx < 0 ){
		minx += 0xffff;
		if( ((minx < rotx) && (0xffff > rotx)) ||
			((0 < rotx) && (maxx > rotx)) ){
			return TRUE;
		}
	}else{
		if( ((minx < rotx) && (maxx > rotx)) ){ 
			return TRUE;
		}
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�n���V��ʑ�����
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	���s
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_WkMainControl( WLDTIMER_WK* p_wk )
{
	u32 result;

#ifdef WLDTIMER_DEBUG_TIMEZONE
	if( sys.trg & PAD_BUTTON_R ){
		s_WLDTIMER_DEBUG_TimeZone = (s_WLDTIMER_DEBUG_TimeZone + 2) % WLDTIMER_TIMEZONE_DATANUM;
		OS_Printf( "debug timezone %d\n", s_WLDTIMER_DEBUG_TimeZone );
	}
	if( sys.trg & PAD_BUTTON_L ){
		if( (s_WLDTIMER_DEBUG_TimeZone - 1) >= 0 ){
			s_WLDTIMER_DEBUG_TimeZone--;
		}else{
			s_WLDTIMER_DEBUG_TimeZone = WLDTIMER_TIMEZONE_DATANUM-2;
		}
		OS_Printf( "debug timezone %d\n", s_WLDTIMER_DEBUG_TimeZone );
	}
#endif

	
	switch( p_wk->main_seq ){
	// �^�b�`����
	case WLDTIMER_MAINSEQ_CONTROL:
		result = WLDTIMER_EarthControl( p_wk );
		switch( result ){
		case WLDTIMER_EARTHCONT_RET_NONE:		
			break;
			
		case WLDTIMER_EARTHCONT_RET_CAMERA_MOVE:
			WLDTIMER_CameraMoveReq( &p_wk->camera );
			p_wk->main_seq = WLDTIMER_MAINSEQ_CAMERAMOVE;
			break;
			
		case WLDTIMER_EARTHCONT_RET_END:
			p_wk->main_seq = WLDTIMER_MAINSEQ_ENDCHECK;	// �I���`�F�b�N
			break;
		}
		break;
		
	// �J��������
	case WLDTIMER_MAINSEQ_CAMERAMOVE:
		result = WLDTIMER_CameraMove( &p_wk->camera, &p_wk->place );
		if( result == TRUE ){
			p_wk->main_seq = WLDTIMER_MAINSEQ_CONTROL;
		}
		break;

	case WLDTIMER_MAINSEQ_ENDCHECK:
		WLDTIMER_EndMsgStart( &p_wk->end_msg );
		WLDTIMER_ViewerTalkWinOff( &p_wk->view );
		WLDTIMER_TouchBttnOff( &p_wk->touch );
		p_wk->main_seq ++;
		break;
		
	case WLDTIMER_MAINSEQ_ENDCHECKWAIT:
		{
			u32 result;
			result = WLDTIMER_EndMsgMain( &p_wk->end_msg );
			switch( result ){
			case TOUCH_SW_RET_NORMAL:	// �����Ȃ�
				break;

			case TOUCH_SW_RET_NO:		// ������
				WLDTIMER_ViewerTalkWinOn( &p_wk->view );
				WLDTIMER_EndMsgEnd( &p_wk->end_msg );
				WLDTIMER_TouchBttnOn( &p_wk->touch );
				p_wk->main_seq = WLDTIMER_MAINSEQ_CONTROL;
				break;;

			default:
			case TOUCH_SW_RET_YES:		// �͂�
				return TRUE;

			}
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
 *	@brief	�^�X�N��ʑ�����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_WkSubControl( WLDTIMER_WK* p_wk )
{
	switch( p_wk->sub_seq ){
	case WLDTIMER_SUB_SEQ_CONTROL:
		WLDTIMER_ViewerMain( &p_wk->view,  &p_wk->msgman, &p_wk->drawsys );
		break;
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�\������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_WkDraw( WLDTIMER_WK* p_wk )
{
	GF_G3X_Reset();
	
	// �J�����ݒ�
	WLDTIMER_CameraDraw( &p_wk->camera );
	
	// �n���V�`��
	WLDTIMER_EarthDraw( &p_wk->earth );

	// �n��f�[�^�`��
	WLDTIMER_PlaceDraw( &p_wk->place, &p_wk->earth );

	// �`��V�X�e���`��
	WLDTIMER_DrawSysDraw( &p_wk->drawsys );

	// 
	GF_G3_RequestSwapBuffers(GX_SORTMODE_AUTO,GX_BUFFERMODE_W);
}

//----------------------------------------------------------------------------
/**
 *	@brief	VBLANK����
 *
 *	@param	p_work 
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_WkVBlank( void* p_work )
{
	WLDTIMER_WK* p_wk = p_work;

	WLDTIMER_DrawSysVBlank( &p_wk->drawsys );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�n���V�Ǘ�
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	WLDTIMER_EARTHCONT_RET_NONE,			// �����Ȃ�
 *	@retval	WLDTIMER_EARTHCONT_RET_CAMERA_MOVE,		// �J��������
 *	@retval	WLDTIMER_EARTHCONT_RET_END,				// �I��
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_EarthControl( WLDTIMER_WK* p_wk )
{
	u32 ret;
	BOOL result;
	
	WLDTIMER_TouchSetParam( &p_wk->touch );

	//�I������
	if((p_wk->touch.tp_result & PAD_BUTTON_B))
	{
		Snd_SePlay( WLDTIMER_SND_YAMERU );
		return WLDTIMER_EARTHCONT_RET_END;
	}
	else{
		// �ʒu�I��
		if( (sys.trg & PAD_BUTTON_X) ){
			// �n���V�������Ă���ʒu�̒n��f�[�^���擾
			{
				WLDTIMER_POINTDATA draw_point;
				result = WLDTIMER_EarthGetPointData( p_wk, &draw_point );
				if( result == TRUE ){

#ifdef WLDTIMER_DEBUG_TIMEZONE
					result = FALSE;	// �d���`�F�b�NOFF
#else
					// �ꏏ�̃f�[�^���Ȃ����`�F�b�N
					result = WLDTIMER_ViewerEqualDataCheck( &p_wk->view, &draw_point );
#endif
					if( result == FALSE ){

						// �����Ă���n�悪�������̂ŏ���ʂɕ\��
						OS_Printf( "�������I\n" );
						result = WLDTIMER_ViewerPushData( &p_wk->view, &draw_point );
						if( result == FALSE ){
							OS_Printf( "���͖����݂����B\n" );
						}else{
							// ���̈ʒu�ɂ��킹��
							WLDTIMER_EarthSetNationAreaRotate( p_wk, 
									draw_point.nation, draw_point.area );
							Snd_SePlay( WLDTIMER_SND_XSELECT );
						}
					}
				}
			}
		}else{

			//�n����]�R���g���[��
			ret = WLDTIMER_Earth3D_Control(p_wk, sys.trg, sys.cont);
		}
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n���V��]�p�x�̏����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_param	�\���f�[�^�x�[�X
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthStartSetUp( WLDTIMER_WK* p_wk, const WLDTIMER_PARAM* cp_param )
{
	int index;
	int nation;
	int area;
	BOOL result;
	

	// �����̒n���ݒ肷��
	result = WLDTIMER_EarthSetNationAreaRotate( p_wk, p_wk->my_nation, p_wk->my_area );
	
	if( result == FALSE ){
		
		// �\��������̂ɂȂ�������f�[�^�x�[�X�̂P�Ԗڂ̒n��ɐݒ�
		index = WLDTIMER_WFLBYDATA_GetFirst( cp_param->cp_data );
		
		if( index != WFLBY_WLDTIMER_DATA_MAX ){		// �\�����邩�`�F�b�N
			
			nation = WFLBY_WLDTIMER_GetNation( cp_param->cp_data, index );
			area = WFLBY_WLDTIMER_GetArea( cp_param->cp_data, index );

			result = WLDTIMER_EarthSetNationAreaRotate( p_wk, nation, area );


			// �����͒n��f�[�^���Ȃ��ƍ���
			GF_ASSERT( result == TRUE );
		}else{

			// �ŏ��̃f�[�^����Ȃ��Ƃ�
		}
		
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n���V�̃|�C���g�ʒu�������Ă���n��f�[�^�̎擾
 *
 *	@param	cp_wk		���[�N
 *	@param	p_data		�n��f�[�^�i�[��
 *
 *	@retval	TRUE	�n�悪������
 *	@retval	FALSE	�n�悪�Ȃ�����
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_EarthGetPointData( const WLDTIMER_WK* cp_wk, WLDTIMER_POINTDATA* p_data )
{
	u32 index;
	
	// �߂��ʒu�ɂ���n��f�[�^�����߂�
	index = WLDTIMER_EarthGetPlaceData( cp_wk );

	// �C���f�b�N�X�����������`�F�b�N
	if( index >= WLDTIMER_PlaceGetListNum( &cp_wk->place ) ){
		return FALSE;	// �Ȃ�����
	}
	
	// ���̒n��̏����i�[
	p_data->timezone	= WLDTIMER_EarthGetTimeZone( cp_wk, index, cp_wk->worldtime );
	p_data->nation		= WLDTIMER_PlaceGetIdxNationID( &cp_wk->place, index );
	p_data->area		= WLDTIMER_PlaceGetIdxAreaID( &cp_wk->place, index );

	return TRUE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�߂��ɂ���n��f�[�^���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	�n��f�[�^��index
 *	@retval	cp_wk->place.planelist.listcount	�Ȃ�����
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_EarthGetPlaceData( const WLDTIMER_WK* cp_wk )
{
	int i;
	s16		minx;
	s16		maxx;
	s16		miny;
	s16		maxy;
	u32 dist;		// �����v�Z
	u32 mindist;	// �ŏ�����
	VecFx32 rotate;
	VecFx32 place_rotate;
	u32	place_col;
	u32 listnum;
	u32 minindex;
	BOOL xcheck;
	BOOL ycheck;

	// �n�惊�X�g���擾
	listnum = WLDTIMER_PlaceGetListNum( &cp_wk->place );

	// �n���V�̉�]�x�N�g���擾
	WLDTIMER_EarthGetRotaVec( &cp_wk->earth, &rotate );
	
	// �����`�����߂�
	minx = (s16)(rotate.x - WLDTIMER_POINTPLANE_HITAREA);
	maxx = (s16)(rotate.x + WLDTIMER_POINTPLANE_HITAREA);
	miny = (s16)(rotate.y - WLDTIMER_POINTPLANE_HITAREA);
	maxy = (s16)(rotate.y + WLDTIMER_POINTPLANE_HITAREA);	

	// �ŏ����������肦�Ȃ������ŏ�����
	mindist = WLDTIMER_POINTPLANE_HITAREA*2;	
	minindex = listnum;

	for( i=0; i<listnum; i++ ){
		
		// ���̒n��ɉ�]�x�N�g���擾
		WLDTIMER_PlaceGetIdxRotVec( &cp_wk->place, &place_rotate, i );
		// ���̒n��̕\���J���[�擾
		place_col = WLDTIMER_PlaceGetIdxCol( &cp_wk->place, i );

		if( (place_rotate.x > minx)&&(place_rotate.x < maxx) &&
			(place_rotate.y > miny)&&(place_rotate.y < maxy) &&
			(place_col != WLDTIMER_PLACE_COL_NONE) ){

			// ���������߂�ׂɉ�]�p�x�𒲐�
			WLDTIMER_MinusRotateChange( &rotate );
			WLDTIMER_MinusRotateChange( &place_rotate );

			// ��]�p�x�̋������擾
			dist = WLDTIMER_EarthGetRotateDist( &rotate, &place_rotate );

			if( dist < mindist ){
				mindist = dist;
				minindex = i;
			}
		}
	}

	// �ŏ��̋����̃C���f�b�N�X��Ԃ�
	return minindex;



#if 0
	int i;
	s32		minx;
	s32		maxx;
	s32		miny;
	s32		maxy;
	u32 dist;		// �����v�Z
	u32 mindist;	// �ŏ�����
	VecFx32 rotate;
	VecFx32 place_rotate;
	u32	place_col;
	u32 listnum;
	u32 minindex;
	BOOL xcheck;
	BOOL ycheck;

	// �n�惊�X�g���擾
	listnum = WLDTIMER_PlaceGetListNum( &cp_wk->place );

	// �n���V�̉�]�x�N�g���擾
	WLDTIMER_EarthGetRotaVec( &cp_wk->earth, &rotate );

	// ��]�p�x�𒲐�
	WLDTIMER_MinusRotateChange( &rotate );
	
	// �����`�����߂�
	minx = (s32)(rotate.x - WLDTIMER_POINTPLANE_HITAREA);
	maxx = (s32)(rotate.x + WLDTIMER_POINTPLANE_HITAREA);
	miny = (s32)(rotate.y - WLDTIMER_POINTPLANE_HITAREA);
	maxy = (s32)(rotate.y + WLDTIMER_POINTPLANE_HITAREA);	

	// �ŏ����������肦�Ȃ������ŏ�����
	mindist = WLDTIMER_POINTPLANE_HITAREA*2;	
	minindex = listnum;

	for( i=0; i<listnum; i++ ){
		
		// ���̒n��ɉ�]�x�N�g���擾
		WLDTIMER_PlaceGetIdxRotVec( &cp_wk->place, &place_rotate, i );
		// ��]�p�x�𒲐�
		WLDTIMER_MinusRotateChange( &place_rotate );
		// ���̒n��̕\���J���[�擾
		place_col = WLDTIMER_PlaceGetIdxCol( &cp_wk->place, i );

		xcheck = WLDTIMER_RotateCheck( minx, maxx, place_rotate.x );
		ycheck = WLDTIMER_RotateCheck( miny, maxy, place_rotate.y );

		if( (xcheck == TRUE) &&
			(ycheck == TRUE) &&
			(place_col != WLDTIMER_PLACE_COL_NONE) ){

			// ��]�p�x�̋������擾
			dist = WLDTIMER_EarthGetRotateDist( &rotate, &place_rotate );

			if( dist < mindist ){
				mindist = dist;
				minindex = i;
			}
		}
	}

	// �ŏ��̋����̃C���f�b�N�X��Ԃ�
	return minindex;
#endif
}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�x�N�g���̋��������߂�
 *
 *	@param	cp_earth		�n���V�̉�]�x�N�g��
 *	@param	cp_place		�n��̉�]�x�N�g��
 *
 *	@return	�����iu32�P�ʁj
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_EarthGetRotateDist( const VecFx32* cp_earth, const VecFx32* cp_place )
{
	s32 dif_x, dif_y;
	u32 dist;

	// �������߂��ق���I��
	dif_x = MATH_ABS(cp_earth->x - cp_place->x);
	dif_y = MATH_ABS(cp_earth->y - cp_place->y);
	if( dif_x > RotKey(180) ){
		dif_x = 0xffff - dif_x;
	}
	if( dif_y > RotKey(180) ){
		dif_y = 0xffff - dif_y;
	}

	dist = FX_Sqrt( ((dif_x*dif_x)+(dif_y*dif_y)) << FX32_SHIFT ) >> FX32_SHIFT;

	return dist;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃ]�[���̎擾
 *
 *	@param	cp_wk		���[�N
 *	@param	placeindex	�n��f�[�^�̃C���f�b�N�X
 *	@param	worldtime	���E����
 *
 *	@return	���ԃ]�[��
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_EarthGetTimeZone( const WLDTIMER_WK* cp_wk, u32 placeindex, WFLBY_TIME worldtime )
{
#ifdef WLDTIMER_DEBUG_TIMEZONE
	return s_WLDTIMER_DEBUG_TimeZone;
#else
	s32 timezone;	// ���ԃ]�[��
	u16 rota;
	VecFx32 gmt_rotate;	// ���]�x�N�g��
	VecFx32 place_rotate;	// �n��̉�]�x�N�g��
	int index;

	// ���E���Ԃ����^�C���]�[�������߂�
	timezone = worldtime.hour - WLDTIMER_TIMEZONE_START_HOUR;

	// ���]�x�N�g���̎擾
	index = WLDTIMER_PlaceGetIdx( &cp_wk->place, 
			WLDTIMER_GMT_NATIONID, WLDTIMER_GMT_AREAID );
	GF_ASSERT( index < WLDTIMER_PlaceGetListNum( &cp_wk->place ) );
	WLDTIMER_PlaceGetIdxRotVec( &cp_wk->place, &gmt_rotate, index );
	
	// �n��̉�]�x�N�g���擾
	WLDTIMER_PlaceGetIdxRotVec( &cp_wk->place, &place_rotate, placeindex );

	// Y����]�p�x�̍����玞�Ԃ����߂�
	rota = gmt_rotate.y - place_rotate.y;
	timezone += (rota*WLDTIMER_TIMEZONE_DATANUM)/0xffff;	// ��]�l����^�C���e�[�u�������߂�

//	OS_Printf( "timezone %d  rota %d\n", timezone, rota );

	// �}�C�i�X�̒l��������A�I�[�o�[�����肵�Ă���͂��Ȃ̂Œ���
	if( timezone < 0 ){
		timezone += WLDTIMER_TIMEZONE_DATANUM;
	}
	if( timezone >= WLDTIMER_TIMEZONE_DATANUM ){
		timezone %= WLDTIMER_TIMEZONE_DATANUM;
	}


	return timezone;
#endif
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ƒn�悩�炻�̈ʒu�𒆐S�Ɏ����Ă���
 *
 *	@param	p_wk		���[�N
 *	@param	nationID	��ID
 *	@param	areaID		�n��ID
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	���s
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_EarthSetNationAreaRotate( WLDTIMER_WK* p_wk, u8 nationID, u8 areaID )
{
	VecFx32 vec;
	VecFx32 earth_vec;
	u32 col;

	// �n��̕\���F�Ƒ��Ή�]�x�N�g�����擾
	// �\���F�́A���̒n�悪�\������f�[�^�ɂ��邩��\��
	col = WLDTIMER_PlaceGetData( &p_wk->place, nationID, areaID, &vec );
	
	if( col != WLDTIMER_PLACE_COL_NONE ){			// �\�����邩�`�F�b�N
		
		WLDTIMER_EarthGetRotaVec( &p_wk->earth, &earth_vec );
		vec.z = earth_vec.z;						// ���͒n���V�������p��
		WLDTIMER_EarthSetRotaVec( &p_wk->earth, &vec );
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�t���O�ݒ�
 *
 *	@param	p_flag		�t���O���[�N
 *	@param	cp_param	�\���f�[�^
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_FlagControl( WLDTIMER_WK* p_wk, const WLDTIMER_PARAM* cp_param )
{
	int i;

	// �t���O��������
	p_wk->flag.japan = 0;
	p_wk->flag.world = JAPAN_MODE;

	// ���{ROM���`�F�b�N
	if( CasetteLanguage == LANG_JAPAN ){
		p_wk->flag.japan = TRUE;
	}else{
		p_wk->flag.japan = FALSE;
	}

	// ���E�\�����[�h�����ׂ�
	if( p_wk->flag.japan == TRUE ){
		for( i=0; i<WFLBY_WLDTIMER_DATA_MAX; i++ ){
			
			// �f�[�^�L��
			if( cp_param->cp_data->data[i].flag == TRUE ){
				// ���{�ȊO�̍����������琢�E�\�����[�h�ɂ���
				if( cp_param->cp_data->data[i].nation != WLDTIMER_JPN_NATIONID ){
					p_wk->flag.world = GLOBAL_MODE;
				}
			}
		}
	}else{
		p_wk->flag.world = GLOBAL_MODE;
	}


#ifdef WLDTIMER_DEBUG_ALLPLACEOPEN
	if( s_WLDTIMER_DEBUG_ALLPLACEOPEN_FLAG == TRUE ){
		p_wk->flag.world = GLOBAL_MODE;
	}
#endif
}


//----------------------------------------------------------------------------
/**
 *	@brief	�\�����̏�����
 *
 *	@param	p_wk		�\���V�X�e�����[�N
 *	@param	p_config	�R���t�B�O�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_DrawSysInit( WLDTIMER_DRAWSYS* p_wk, CONFIG* p_config, u32 heapID )
{
	// �A�[�J�C�u�n���h��
	p_wk->p_handle = ArchiveDataHandleOpen( ARC_WORLDTIMER, heapID );

	// Vram�]���}�l�[�W���쐬
	initVramTransferManagerHeap( WLDTIMER_VRAMTRANS_TASKNUM, heapID );
	
	// �o���N�ݒ�
	GF_Disp_SetBank( &sc_WLDTIMER_BANK );

	// BG�ݒ�
	WLDTIMER_DrawSysBgInit( p_wk, p_config, heapID );

	// OAM�ݒ�
	WLDTIMER_DrawSysOamInit( p_wk, heapID );

	// 3D�ݒ�
	WLDTIMER_DrawSys3DInit( p_wk, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\�����̔j��
 *
 *	@param	p_wk		�\���V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_DrawSysExit( WLDTIMER_DRAWSYS* p_wk )
{
	// �A�[�J�C�u�n���h��
	ArchiveDataHandleClose( p_wk->p_handle );

	// Vram�]���}�l�[�W���j��
	DellVramTransferManager();
	
	// BG�ݒ�
	WLDTIMER_DrawSysBgExit( p_wk );

	// OAM�ݒ�
	WLDTIMER_DrawSysOamExit( p_wk );

	// 3D�ݒ�
	WLDTIMER_DrawSys3DExit( p_wk );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�揈��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_DrawSysDraw( WLDTIMER_DRAWSYS* p_wk )
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
static void WLDTIMER_DrawSysVBlank( WLDTIMER_DRAWSYS* p_wk )
{
    // BG��������
    GF_BGL_VBlankFunc( p_wk->p_bgl );

    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();

	// Vram�]��
	DoVramTransferManager();
}

// BG
static void WLDTIMER_DrawSysBgInit( WLDTIMER_DRAWSYS* p_wk, CONFIG* p_config, u32 heapID )
{
	// BG�ݒ�
	GF_BGL_InitBG(&sc_BGINIT);
	
	// BGL�쐬
	p_wk->p_bgl = GF_BGL_BglIniAlloc( heapID );

	// ���C���ƃT�u��؂�ւ���
	sys.disp3DSW = DISP_3D_TO_SUB;
	GF_Disp_DispSelect();


	// BG�R���g���[���ݒ�
	{
		int i;

		for( i=0; i<WLDTIMER_BGCNT_NUM; i++ ){
			GF_BGL_BGControlSet( p_wk->p_bgl, 
					sc_WLDTIMER_BGCNT_FRM[i], &sc_WLDTIMER_BGCNT_DATA[i],
					GF_BGL_MODE_TEXT );
			GF_BGL_ClearCharSet( sc_WLDTIMER_BGCNT_FRM[i], 32, 0, heapID);
			GF_BGL_ScrClear( p_wk->p_bgl, sc_WLDTIMER_BGCNT_FRM[i] );
		}
	}

	// ��{�L�����N�^�p���b�g�t���[��
	// �T�u���
	ArcUtil_HDL_PalSet( p_wk->p_handle, NARC_worldtimer_world_watch_NCLR,
			PALTYPE_SUB_BG, 0, 0, heapID );
	
	// �t���[��
	{
		ArcUtil_HDL_BgCharSet( p_wk->p_handle, 
				NARC_worldtimer_world_watch_frame_NCGR, p_wk->p_bgl,
				GF_BGL_FRAME0_S, 0, 0, FALSE, heapID );

		ArcUtil_HDL_ScrnSet( p_wk->p_handle,
				NARC_worldtimer_world_watch_frame_NSCR, p_wk->p_bgl,
				GF_BGL_FRAME0_S, 0, 0, FALSE, heapID );
	}

	// �w�i
	{
		ArcUtil_HDL_BgCharSet( p_wk->p_handle,
				NARC_worldtimer_world_watch_NCGR, p_wk->p_bgl, 
				GF_BGL_FRAME2_S, 0, 0, FALSE, heapID );

		ArcUtil_HDL_ScrnSet( p_wk->p_handle,
				NARC_worldtimer_world_watch_wall_NSCR, p_wk->p_bgl,
				GF_BGL_FRAME3_S, 0, 0, FALSE, heapID );
	}
	
	// �t�H���g�J���[
    TalkFontPaletteLoad( PALTYPE_MAIN_BG, WLDTIMER_PALMAIN_FONT*0x20, heapID );
    TalkFontPaletteLoad( PALTYPE_SUB_BG, WLDTIMER_PALSUB_FONT*0x20, heapID );

    MenuWinGraphicSet(
        p_wk->p_bgl, GF_BGL_FRAME1_M, 
		WLDTIMER_MAIN_SYSTEMWIN_CGX, WLDTIMER_PALMAIN_SYSTEM, 0, heapID );

	// �V�X�e���E�B���h�E
    MenuWinGraphicSet(
        p_wk->p_bgl, GF_BGL_FRAME1_M, 
		WLDTIMER_MAIN_SYSTEMWIN_CGX, WLDTIMER_PALMAIN_SYSTEM, 0, heapID );

	// �g�[�N�E�B���h�E
	{
		u8 win_num = CONFIG_GetWindowType( p_config );
		TalkWinGraphicSet( p_wk->p_bgl, GF_BGL_FRAME0_S,
				WLDTIMER_SUB_TALKWIN_CGX, WLDTIMER_SUB_TALKWIN_PAL,
				win_num, heapID );

		TalkWinGraphicSet( p_wk->p_bgl, GF_BGL_FRAME1_M,
				WLDTIMER_MAIN_TALKWIN_CGX, WLDTIMER_MAIN_TALKWIN_PAL,
				win_num, heapID );
	}

	// �o�b�N�O���E���h�J���[�ݒ�
	GF_BGL_BackGroundColorSet( GF_BGL_FRAME0_M, 0x72ca );
}
static void WLDTIMER_DrawSysBgExit( WLDTIMER_DRAWSYS* p_wk )
{
	// �a�f�R���g���[���j��
	{
		int i;

		for( i=0; i<WLDTIMER_BGCNT_NUM; i++ ){
			GF_BGL_BGControlExit( p_wk->p_bgl, sc_WLDTIMER_BGCNT_FRM[i] );
		}
	}
	
	// BGL�j��
	sys_FreeMemoryEz( p_wk->p_bgl );

	// ���C���ƃT�u�����ɖ߂�
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();
}

// OAM
static void WLDTIMER_DrawSysOamInit( WLDTIMER_DRAWSYS* p_wk, u32 heapID )
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
    InitCharManagerReg(&sc_WLDTIMER_CHARMAN_INIT, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K );
    // �p���b�g�}�l�[�W���[������
    InitPlttManager(WLDTIMER_OAM_CONTNUM, heapID);

    // �ǂݍ��݊J�n�ʒu��������
    CharLoadStartAll();
    PlttLoadStartAll();

    //�ʐM�A�C�R���p�ɃL�������p���b�g����
    CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
    

    // �Z���A�N�^�[�Z�b�g�쐬
    p_wk->p_clactset = CLACT_U_SetEasyInit( WLDTIMER_OAM_CONTNUM, &p_wk->renddata, heapID );

    // �T�[�t�F�[�X�ʒu���ړ�������
    CLACT_U_SetSubSurfaceMatrix( &p_wk->renddata, 0, WLDTIMER_SF_MAT_Y );
    
    // �L�����ƃp���b�g�̃��\�[�X�}�l�[�W���쐬
    for( i=0; i<WLDTIMER_RESMAN_NUM; i++ ){
        p_wk->p_resman[i] = CLACT_U_ResManagerInit(WLDTIMER_OAM_CONTNUM, i, heapID);
    }

	// ����ʂɒʐM�A�C�R�����o��
	WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��


	// �\���J�n
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
}
static void WLDTIMER_DrawSysOamExit( WLDTIMER_DRAWSYS* p_wk )
{
    int i;

    // �A�N�^�[�̔j��
    CLACT_DestSet( p_wk->p_clactset );

    for( i=0; i<WLDTIMER_RESMAN_NUM; i++ ){
        CLACT_U_ResManagerDelete( p_wk->p_resman[i] );
    }

    // ���\�[�X���
    DeleteCharManager();
    DeletePlttManager();

    //OAM�����_���[�j��
    REND_OAM_Delete();
}

// 3D
static void WLDTIMER_DrawSys3DInit( WLDTIMER_DRAWSYS* p_wk, u32 heapID )
{
	// �RD�ݒ�
	p_wk->p_3dman = GF_G3DMAN_Init( heapID, GF_G3DMAN_LNK, GF_G3DTEX_256K, 
			GF_G3DMAN_LNK, GF_G3DPLT_64K, WLDTIMER_DrawSys3DSetUp );

	// ���C�g������
	NNS_G3dGlbLightVector(USE_LIGHT_NUM,
			INIT_LIGHT_ANGLE_XVAL,INIT_LIGHT_ANGLE_YVAL,INIT_LIGHT_ANGLE_ZVAL);
}
static void WLDTIMER_DrawSys3DExit( WLDTIMER_DRAWSYS* p_wk )
{
    GF_G3D_Exit( p_wk->p_3dman );
}
static void WLDTIMER_DrawSys3DSetUp( void )
{
	// �R�c�g�p�ʂ̐ݒ�(�\�����v���C�I���e�B�[)
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    G2_SetBG0Priority(1);

	// �e��`�惂�[�h�̐ݒ�(�V�F�[�h���A���`�G�C���A�X��������)
    G3X_SetShading( GX_SHADING_TOON );
    G3X_AntiAlias( TRUE );
	G3X_AlphaTest( FALSE, 0 );	// �A���t�@�e�X�g�@�@�I�t
	G3X_AlphaBlend( TRUE );		// �A���t�@�u�����h�@�I��
	G3X_EdgeMarking( TRUE );
	G3X_SetFog( FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0 );

	// �N���A�J���[�̐ݒ�
    G3X_SetClearColor(GX_RGB(26,26,26),0,0x7fff,63,FALSE);	//color,alpha,depth,polygonID,fog
	// �r���[�|�[�g�̐ݒ�
    G3_ViewPort(0, 0, 255, 191);

	// �o�b�N�J���[�ݒ�
}


//----------------------------------------------------------------------------
/**
 *	@brief	�n���V������
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthInit( WLDTIMER_EARTH* p_wk, WLDTIMER_DRAWSYS* p_drawsys, u32 heapID )
{
	// �\���f�[�^������
	{
		VEC_Set( &p_wk->trans,			// ���W
				INIT_EARTH_TRANS_XVAL,
				INIT_EARTH_TRANS_YVAL,
				INIT_EARTH_TRANS_ZVAL );

		VEC_Set( &p_wk->scale,			// �g��
				INIT_EARTH_SCALE_XVAL,
				INIT_EARTH_SCALE_YVAL,
				INIT_EARTH_SCALE_ZVAL );

		VEC_Set( &p_wk->rotate,			// ��]
				INIT_EARTH_ROTATE_XVAL,
				INIT_EARTH_ROTATE_YVAL,
				INIT_EARTH_ROTATE_ZVAL );
	}

	// �\�����\�[�X�ǂݍ���
	{
		D3DOBJ_MdlLoadH( &p_wk->mdl, p_drawsys->p_handle, 
				NARC_worldtimer_wifi_earth_nsbmd, heapID );
		D3DOBJ_Init( &p_wk->obj, &p_wk->mdl );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n���V�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthExit( WLDTIMER_EARTH* p_wk )
{
	// ���\�[�X�̔j��
	{
		D3DOBJ_MdlDelete( &p_wk->mdl );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n���V�`��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthDraw( WLDTIMER_EARTH* p_wk )
{
	MtxFx33 rotmtx;


	// ��]�s��擾
	WLDTIMER_EarthGetRotMtx( p_wk, &rotmtx );
	
	// ���W�A�g���ݒ�
	D3DOBJ_SetMatrix( &p_wk->obj, 
			p_wk->trans.x, p_wk->trans.y, p_wk->trans.z );
	D3DOBJ_SetScale( &p_wk->obj, 
			p_wk->scale.x, p_wk->scale.y, p_wk->scale.z );


	// �\��
	D3DOBJ_DrawRMtx( &p_wk->obj,
					 &rotmtx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n���V�\�����W���擾
 *
 *	@param	cp_wk		���[�N
 *	@param	p_mat		���W�i�[��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthGetTrans( const WLDTIMER_EARTH* cp_wk, VecFx32* p_mat )
{
	*p_mat = cp_wk->trans;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n���V��]�s����擾
 *
 *	@param	cp_wk	���[�N
 *	@param	p_rot	�s��i�[��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthGetRotMtx( const WLDTIMER_EARTH* cp_wk, MtxFx33* p_rot )
{
	WLDTIMER_EarthVecFx32_to_MtxFx33( p_rot, &cp_wk->rotate );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n���V	��]�x�N�g���擾
 *
 *	@param	cp_wk	���[�N
 *	@param	p_vec	��]�x�N�g��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthGetRotaVec( const WLDTIMER_EARTH* cp_wk, VecFx32* p_vec )
{
	*p_vec = cp_wk->rotate;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n���V	��]�x�N�g���ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_vec		��]�x�N�g��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthSetRotaVec( WLDTIMER_EARTH* p_wk, const VecFx32* cp_vec )
{
	p_wk->rotate = *cp_vec;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�n��f�[�^������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�\���n��f�[�^
 *	@param	flag		�t���O�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PlaceInit( WLDTIMER_PLACE* p_wk, const WFLBY_WLDTIMER* cp_data, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_FLAG flag, u32 heapID )
{
	int i;
	
	// �n�惊�X�g�ǂݍ���
	WLDTIMER_EarthListLoad( p_wk, cp_data, heapID );

	// �`�惊�\�[�X�ǂݍ���
	for( i=0; i<WLDTIMER_PLACE_COL_NUM; i++ ){
		D3DOBJ_MdlLoadH( &p_wk->mdl[i], p_drawsys->p_handle,// �ǂݍ���
				sc_WLDTIMER_PLACE_MDL[i], heapID );
		D3DOBJ_Init( &p_wk->obj[i], &p_wk->mdl[i] );		// �\���I�u�W�F������
	}

	// �}�[�N�X�P�[��������
	if( flag.world == JAPAN_MODE ){
		VEC_Set( &p_wk->list_scale,
				 INIT_EARTH_SCALE_NEAR_XVAL,
				 INIT_EARTH_SCALE_NEAR_YVAL,
				 INIT_EARTH_SCALE_NEAR_ZVAL );
	}else{
		VEC_Set( &p_wk->list_scale,
				 INIT_EARTH_SCALE_XVAL,
				 INIT_EARTH_SCALE_YVAL,
				 INIT_EARTH_SCALE_ZVAL );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n��f�[�^�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PlaceExit( WLDTIMER_PLACE* p_wk )
{
	int i;
	
	// �`�惊�\�[�X�j��
	for( i=0; i<WLDTIMER_PLACE_COL_NUM; i++ ){
		D3DOBJ_MdlDelete( &p_wk->mdl[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n��f�[�^�̕\��
 *	
 *	@param	p_wk		���[�N
 *	@param	cp_earth	�n�����[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PlaceDraw( WLDTIMER_PLACE* p_wk, const WLDTIMER_EARTH* cp_earth )
{
	MtxFx33 rotate_tmp;
	int i;
	MtxFx33 earthrotmtx;
	VecFx32 trans;

	// �n���V�̕\���f�[�^�擾
	WLDTIMER_EarthGetTrans( cp_earth, &trans );
	WLDTIMER_EarthGetRotMtx( cp_earth, &earthrotmtx );

	// �\�����f���ɕ\���f�[�^���i�[
	for( i=0; i<WLDTIMER_PLACE_COL_NUM; i++ ){
		D3DOBJ_SetMatrix( &p_wk->obj[i], trans.x, trans.y, trans.z );
		// �^�b�`�}�[�N�͐�΂ɑS���̃}�[�N�̏�ɕ\�������
		if( i==WLDTIMER_PLACE_COL_TOUCH ){
			D3DOBJ_SetScale( &p_wk->obj[i], 
					p_wk->list_scale.x, 
					p_wk->list_scale.y,
					p_wk->list_scale.z+WLDTIMER_PLACE_TOUCH_SCALE_Z_ADD );
		}else{
			D3DOBJ_SetScale( &p_wk->obj[i], 
					p_wk->list_scale.x, p_wk->list_scale.y, p_wk->list_scale.z );
		}
	}

	// �G���Ă���ʒu�̕\��
	{
		MTX_Identity33( &rotate_tmp );
		D3DOBJ_DrawRMtx( &p_wk->obj[ WLDTIMER_PLACE_COL_TOUCH ],
						 &rotate_tmp );
	}
	

	// �e�n�_�̕\��
	{
		for(i=0;i<p_wk->placelist.listcount;i++){
			MTX_Concat33(&p_wk->placelist.place[i].rotate,&earthrotmtx,&rotate_tmp);

			if(p_wk->placelist.place[i].col != WLDTIMER_PLACE_COL_NONE){
				D3DOBJ_DrawRMtx( &p_wk->obj[ p_wk->placelist.place[i].col ],
								 &rotate_tmp );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ID�ƒn��ID���炻�̒n������F�ŕ\��������悢����Ԃ�
 *
 *	@param	cp_data			�\���n��f�[�^
 *	@param	nationID		��ID
 *	@param	areaID			�G���AID
 *
 *	@retval WLDTIMER_PLACE_COL_IN,		// �����J���[
 *	@retval WLDTIMER_PLACE_COL_OUT,		// �ގ��J���[
 *	@retval	WLDTIMER_PLACE_COL_NONE,	// �����ɂ��Ȃ��n��ɐݒ肳��Ă���J���[
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_PlaceGetCol( const WFLBY_WLDTIMER* cp_data, u16 nationID, u16 areaID )
{
	int i;
	BOOL outside;
	BOOL dataflag;
	u8 nation, area;

	for( i=0; i<WFLBY_WLDTIMER_DATA_MAX; i++ ){
		dataflag = WFLBY_WLDTIMER_GetDataFlag( cp_data, i );	// �f�[�^�L��
		if( dataflag ){
			outside = WFLBY_WLDTIMER_GetOutSide( cp_data, i );	// �ގ��t���O
			nation	= WFLBY_WLDTIMER_GetNation( cp_data, i );	// ��ID
			area	= WFLBY_WLDTIMER_GetArea( cp_data, i );		// �n��ID

			// ���ƒn�悪�������`�F�b�N
			if( (nation == nationID) && (area == areaID) ){
				
				// �ގ��������`�F�b�N
				if( outside == TRUE ){
					return	WLDTIMER_PLACE_COL_OUT; 
				}
				return	WLDTIMER_PLACE_COL_IN; 
			}
		}
	}

	return WLDTIMER_PLACE_COL_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n��f�[�^�̎擾
 *
 *	@param	cp_wk		���[�N
 *	@param	nationID	��ID
 *	@param	areaID		�G���AID
 *	@param	p_vec		���Ή�]�x�N�g���擾
 *
 *	@return	�\���J���[�擾
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_PlaceGetData( const WLDTIMER_PLACE* cp_wk, u16 nationID, u16 areaID, VecFx32* p_vec )
{
	u32 col = WLDTIMER_PLACE_COL_NONE;
	int index;
	int listnum;

	listnum = WLDTIMER_PlaceGetListNum( cp_wk );

	// �C���f�b�N�X�̎擾
	index = WLDTIMER_PlaceGetIdx( cp_wk, nationID, areaID );
	if( index < listnum ){

		// �J���[�擾
		col = WLDTIMER_PlaceGetIdxCol( cp_wk, index );

		// ��]�x�N�g���擾
		WLDTIMER_PlaceGetIdxRotVec( cp_wk, p_vec, index );
	}

	return col;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ID�ƒn��ID����n��f�[�^�x�[�X�̃C���f�b�N�X�l���擾
 *
 *	@param	cp_wk			�n��f�[�^�x�[�X
 *	@param	nationID		��ID
 *	@param	areaID			�n��ID
 *
 *	@return	index			�C���f�b�N�X
 *	@retval	cp_wk->placelist.listcount	�Ȃ瓖�Ă͂܂�̂����Ȃ�
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_PlaceGetIdx( const WLDTIMER_PLACE* cp_wk, u16 nationID, u16 areaID )
{
	int i;
	
	for( i=0; i<cp_wk->placelist.listcount; i++ ){

		if( (cp_wk->placelist.place[i].nationID == nationID) &&
			(cp_wk->placelist.place[i].areaID == areaID) ){
			return i;
		}
	}

	return cp_wk->placelist.listcount;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C���f�b�N�X�̉�]�x�N�g�����擾
 *
 *	@param	cp_wk		���[�N
 *	@param	p_vec		��]�x�N�g���i�[��
 *	@param	index		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PlaceGetIdxRotVec( const WLDTIMER_PLACE* cp_wk, VecFx32* p_vec, int index )
{
	GF_ASSERT( index < cp_wk->placelist.listcount );
	
	p_vec->x = cp_wk->placelist.place[ index ].x;
	p_vec->y = cp_wk->placelist.place[ index ].y;
	p_vec->z = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C���f�b�N�X�̃J���[���擾
 *
 *	@param	cp_wk		���[�N
 *	@param	index		�C���f�b�N�X
 *
 *	@return	�J���[
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_PlaceGetIdxCol( const WLDTIMER_PLACE* cp_wk, int index )
{
	GF_ASSERT( index < cp_wk->placelist.listcount );
	
	return cp_wk->placelist.place[ index ].col;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C���f�b�N�X�̍�ID���擾
 *
 *	@param	cp_wk		���[�N
 *	@param	index		�C���f�b�N�X
 *
 *	@return	��ID
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_PlaceGetIdxNationID( const WLDTIMER_PLACE* cp_wk, int index )
{
	GF_ASSERT( index < cp_wk->placelist.listcount );
	
	return cp_wk->placelist.place[ index ].nationID;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C���f�b�N�X�̒n��ID���擾
 *
 *	@param	cp_wk		���[�N
 *	@param	index		�C���f�b�N�X
 *
 *	@return	�n��ID
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_PlaceGetIdxAreaID( const WLDTIMER_PLACE* cp_wk, int index )
{
	GF_ASSERT( index < cp_wk->placelist.listcount );
	
	return cp_wk->placelist.place[ index ].areaID;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C���f�b�N�X�̍��ƒn��̃��X�g���擾
 *
 *	@param	cp_wk	���[�N
 *		
 *	@return	���X�g��
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_PlaceGetListNum( const WLDTIMER_PLACE* cp_wk )
{
	return cp_wk->placelist.listcount;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�J����������
 *
 *	@param	p_wk	���[�N
 *	@param	flag	����t���O
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_CameraInit( WLDTIMER_CAMERA* p_wk, WLDTIMER_FLAG flag, u32 heapID )
{
	// �J�����쐬
	p_wk->p_camera = GFC_AllocCamera( heapID );
	
	//�J�������C�u����������
	GFC_InitCameraTC(	&sc_WLDTIMER_CAMERA_TARGET,&sc_WLDTIMER_CAMERA_POS,
						INIT_CAMERA_PERSPWAY,
						GF_CAMERA_PERSPECTIV,
						FALSE,
						p_wk->p_camera);

	//�N���b�v�֘A�ݒ�
	GFC_SetCameraClip(INIT_CAMERA_CLIP_NEAR,INIT_CAMERA_CLIP_FAR,p_wk->p_camera);
	GFC_SetCameraView(GF_CAMERA_PERSPECTIV,p_wk->p_camera);
	//�J�����n�m
	GFC_AttachCamera(p_wk->p_camera);

	if(flag.world == JAPAN_MODE){
		//���Emode����Ȃ��̂ŁA�߂�����
		p_wk->status = CAMERA_NEAR;
		p_wk->dist = INIT_CAMERA_DISTANCE_NEAR;
	}else{
		//���Emode�Ȃ̂ŉ�������
		p_wk->status = CAMERA_FAR;
		p_wk->dist = INIT_CAMERA_DISTANCE_FAR;
	}

	// �����J�����ݒ�
	GFC_SetCameraDistance(p_wk->dist,p_wk->p_camera);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_CameraExit( WLDTIMER_CAMERA* p_wk )
{
	GFC_FreeCamera( p_wk->p_camera );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����\��
 *
 *	@param	cp_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_CameraDraw( const WLDTIMER_CAMERA* cp_wk )
{
	GFC_CameraLookAt();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J��������	���N�G�X�g
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_CameraMoveReq( WLDTIMER_CAMERA* p_wk )
{
	if( p_wk->status == CAMERA_FAR ){
		p_wk->status = CAMERA_NEAR;
		Snd_SePlay( WLDTIMER_SND_ZOMEIN );
	}else{
		p_wk->status = CAMERA_FAR;
		Snd_SePlay( WLDTIMER_SND_ZOMEOUT );
	}
	p_wk->move = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J��������	
 *
 *	@param	p_wk	���[�N
 *	@param	p_place	�n�惏�[�N
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_CameraMove( WLDTIMER_CAMERA* p_wk, WLDTIMER_PLACE* p_place )
{
	if( p_wk->move == FALSE ){
		return TRUE;
	}

	switch( p_wk->status ){
	case CAMERA_NEAR:
		if(p_wk->dist > (INIT_CAMERA_DISTANCE_NEAR + CAMERA_INOUT_SPEED)){
			p_wk->dist -= CAMERA_INOUT_SPEED;
			p_place->list_scale.x -= MARK_SCALE_INCDEC;
			p_place->list_scale.y = p_place->list_scale.x;
		}else{
			p_wk->dist = INIT_CAMERA_DISTANCE_NEAR;
			p_wk->move = FALSE;
		}
		break;

	case CAMERA_FAR:
		if(p_wk->dist < (INIT_CAMERA_DISTANCE_FAR - CAMERA_INOUT_SPEED)){
			p_wk->dist += CAMERA_INOUT_SPEED;
			p_place->list_scale.x += MARK_SCALE_INCDEC;
			p_place->list_scale.y = p_place->list_scale.x;
		}else{
			p_wk->dist = INIT_CAMERA_DISTANCE_FAR;
			p_wk->move = FALSE;
		}
		break;
	}
	GFC_SetCameraDistance(p_wk->dist,p_wk->p_camera);

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J������Ԃ̎擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	CAMERA_FAR = 0,	// ������
 *	@retval	CAMERA_NEAR,	// �ߋ���
 */
//-----------------------------------------------------------------------------
static u32	WLDTIMER_CameraGetStatus( const WLDTIMER_CAMERA* cp_wk )
{
	return cp_wk->status;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�^�b�`�Ǘ�������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TouchInit( WLDTIMER_TOUCH* p_wk, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_MSGMAN* p_msgman, u32 heapID )
{
	memset( p_wk, 0, sizeof(WLDTIMER_TOUCH) );

	// �{�^���r�b�g�}�b�v�쐬
	GF_BGL_BmpWinAdd(
				p_drawsys->p_bgl, &p_wk->bttn, GF_BGL_FRAME1_M,
				WLDTIMER_MAIN_BTTNBMP_X, WLDTIMER_MAIN_BTTNBMP_Y,
				WLDTIMER_MAIN_BTTNBMP_SIZX, WLDTIMER_MAIN_BTTNBMP_SIZY, 
				WLDTIMER_MAIN_BTTNBMP_PAL, WLDTIMER_MAIN_BTTNBMP_CGX );
	
	GF_BGL_BmpWinDataFill( &p_wk->bttn, 15 );

	// ��߂�`��
	{
		STRBUF* p_str;
		p_str = WLDTIMER_MsgManGetStr( p_msgman, msg_01 );

		FontProc_LoadFont( FONT_BUTTON, heapID );	//�{�^���t�H���g�̃��[�h
		GF_STR_PrintColor(&p_wk->bttn,FONT_BUTTON,p_str,
				4,0,MSG_NO_PUT, WLDTIMER_TOUCH_END_MSG_COL, NULL);
		FontProc_UnloadFont( FONT_BUTTON );				//�{�^���t�H���g�̔j��
	}

	// �E�B���h�E�`��
	BmpMenuWinWrite(&p_wk->bttn,WINDOW_TRANS_ON,
			WLDTIMER_MAIN_SYSTEMWIN_CGX,WLDTIMER_MAIN_SYSTEMWIN_PAL);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�b�`�Ǘ��j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TouchExit( WLDTIMER_TOUCH* p_wk )
{
	// �r�b�g�}�b�v�j��
	GF_BGL_BmpWinDel( &p_wk->bttn );
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	����̃^�b�`�p�l���������[�N�ɐݒ�
 *
 *	@param	p_touch		���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TouchSetParam( WLDTIMER_TOUCH* p_touch )
{
	int dirx,lenx,diry,leny;

	p_touch->tp_result = 0;

	if(sys.tp_trg){
		if(	(sys.tp_x >= ((EARTH_ICON_WIN_PX) * DOTSIZE))&&
			(sys.tp_x <= ((EARTH_ICON_WIN_PX + EARTH_ICON_WIN_SX) * DOTSIZE))&&
			(sys.tp_y >= ((EARTH_ICON_WIN_PY) * DOTSIZE))&&
			(sys.tp_y <= ((EARTH_ICON_WIN_PY + EARTH_ICON_WIN_SY) * DOTSIZE))){
			//�u��߂�v
			p_touch->tp_result = PAD_BUTTON_B;
			return;
		} else {
			p_touch->tp_seq = 0;
			p_touch->tp_lenx = 0;
			p_touch->tp_leny = 0;
			p_touch->tp_count = 0;
			p_touch->tp_result = 0;
			//����̌��o�ʒu��ۑ�
			p_touch->tp_x = sys.tp_x;
			p_touch->tp_y = sys.tp_y;
			p_touch->tp_count = WLDTIMER_TOUCH_ZOOMWAIT_COUNT;
		}
	}
	if(sys.tp_cont){
		switch(p_touch->tp_seq){
		case 0:
			//�ŏ��̃J�E���g�̓g���K�[�F���p�ɖ���
			if(!p_touch->tp_count){
				p_touch->tp_seq++;
			}else{
				p_touch->tp_count--;
			}
		case 1:
			WLDTIMER_Earth_TouchPanelParamGet(p_touch->tp_x,p_touch->tp_y,&dirx,&lenx,&diry,&leny);
			p_touch->tp_result = dirx | diry;
			p_touch->tp_lenx = lenx;
			p_touch->tp_leny = leny;
			p_touch->tp_x = sys.tp_x;
			p_touch->tp_y = sys.tp_y;
			break;
		}
	}else{
		if(p_touch->tp_count){
			p_touch->tp_result = PAD_BUTTON_A;
		}
		p_touch->tp_seq = 0;
		p_touch->tp_lenx = 0;
		p_touch->tp_leny = 0;
		p_touch->tp_count = 0;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�b�`�p�l���p�����[�^�擾
 *
 *	@param	cp_touch	�^�b�`���[�N
 *	@param	type		�擾�p�����[�^�^�C�v
 *
 *	@return	�p�����[�^
 */
//-----------------------------------------------------------------------------
static int WLDTIMER_TouchGetParam( const WLDTIMER_TOUCH* cp_touch, u32 type )
{
	int ret;
	switch( type ){
	case WLDTIMER_TOUCH_PM_RESULT:
		ret = cp_touch->tp_result;
		break;
		
	case WLDTIMER_TOUCH_PM_LENX:
		ret = cp_touch->tp_lenx;
		break;
		
	case WLDTIMER_TOUCH_PM_LENY:
		ret = cp_touch->tp_leny;
		break;

	default:
		break;
	}
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���\��OFF
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TouchBttnOff( WLDTIMER_TOUCH* p_wk )
{
	BmpMenuWinClear( &p_wk->bttn, WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( &p_wk->bttn );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���\��ON
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TouchBttnOn( WLDTIMER_TOUCH* p_wk )
{
	GF_BGL_BmpWinOnVReq( &p_wk->bttn );
	BmpMenuWinWrite(&p_wk->bttn,WINDOW_TRANS_ON,
			WLDTIMER_MAIN_SYSTEMWIN_CGX,WLDTIMER_MAIN_SYSTEMWIN_PAL);
}


//----------------------------------------------------------------------------
/**
 *	@brief	�I���`�F�b�N	������
 *
 *	@param	p_wk			���[�N
 *	@param	p_drawsys		�`�惏�[�N
 *	@param	p_msgman		���b�Z�[�W�Ǘ�
 *	@param	p_save			�Z�[�u�f�[�^
 *	@param	heapID			�q�[�v
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EndMsgInit( WLDTIMER_END_MSG* p_wk, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_MSGMAN* p_msgman, SAVEDATA* p_save, u32 heapID )
{
	memset( p_wk, 0, sizeof(WLDTIMER_TOUCH) );

	// ���b�Z�[�W�X�s�[�h
	{
		CONFIG* p_config;
		p_config = SaveData_GetConfig( p_save );
		p_wk->msg_wait = CONFIG_GetMsgPrintSpeed( p_config );
	}

	// ���b�Z�[�W�o�b�t�@
	p_wk->p_str = STRBUF_Create( WLDTIMER_MSGMAN_STRBUFNUM, heapID );

	// �{�^���r�b�g�}�b�v�쐬
	GF_BGL_BmpWinAdd(
				p_drawsys->p_bgl, &p_wk->win, GF_BGL_FRAME1_M,
				WLDTIMER_MAIN_TALKBMP_X, WLDTIMER_MAIN_TALKBMP_Y,
				WLDTIMER_MAIN_TALKBMP_SIZX, WLDTIMER_MAIN_TALKBMP_SIZY, 
				WLDTIMER_MAIN_TALKBMP_PAL, WLDTIMER_MAIN_TALKBMP_CGX );
	
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );

	// �`��
	WLDTIMER_MsgManGetStrBuff( p_msgman, msg_05, p_wk->p_str );

	// �T�u�E�B���h�E�^�b�`���[�N������
	p_wk->p_touch_sw = TOUCH_SW_AllocWork( heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I���`�F�b�N	���[�N�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EndMsgExit( WLDTIMER_END_MSG* p_wk )
{
	// ���b�Z�[�W�\�����Ȃ�OFF
	if( GF_MSG_PrintEndCheck( p_wk->msg_no ) != 0 ){
		GF_STR_PrintForceStop( p_wk->msg_no  );
	}
	
	// ���b�Z�[�W�o�b�t�@�j��
	STRBUF_Delete( p_wk->p_str );
	
	// �T�u�E�B���h�E�^�b�`���[�N�j��
	TOUCH_SW_FreeWork( p_wk->p_touch_sw );

	// �r�b�g�}�b�v�j��
	GF_BGL_BmpWinDel( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I���`�F�b�N	�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EndMsgStart( WLDTIMER_END_MSG* p_wk )
{
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
	// �{�^���ƃ��b�Z�[�W�\��
	BmpTalkWinWrite( &p_wk->win, WINDOW_TRANS_OFF, 
			WLDTIMER_MAIN_TALKWIN_CGX, WLDTIMER_MAIN_TALKWIN_PAL );
	GF_BGL_BmpWinOnVReq( &p_wk->win );

	p_wk->msg_no = GF_STR_PrintSimple(&p_wk->win,FONT_TALK,p_wk->p_str,
			0,0,p_wk->msg_wait, NULL);

	p_wk->seq = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I���`�F�b�N	���C��
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TOUCH_SW_RET_NORMAL	// �����Ȃ�
 *	@retval	TOUCH_SW_RET_YES	// �͂�
 *	@retval	TOUCH_SW_RET_NO		// ������
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_EndMsgMain( WLDTIMER_END_MSG* p_wk )
{
	u32 ret;
	switch( p_wk->seq ){
	case 0:
		ret = TOUCH_SW_RET_NORMAL;
		// ���b�Z�[�W�I���҂�
		if( GF_MSG_PrintEndCheck( p_wk->msg_no ) == 0 ){
			TOUCH_SW_PARAM param;
			param		= sc_TOUCH_SW_PARAM;
			param.p_bgl = GF_BGL_BmpWinGet_BglIni( &p_wk->win );
			TOUCH_SW_Init( p_wk->p_touch_sw, &param );
			p_wk->seq ++;
		}
		break;
	case 1:
		ret = TOUCH_SW_Main( p_wk->p_touch_sw );
		break;
	}
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I���`�F�b�N�@����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EndMsgEnd( WLDTIMER_END_MSG* p_wk )
{
	TOUCH_SW_Reset( p_wk->p_touch_sw );

	BmpTalkWinClear( &p_wk->win, WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( &p_wk->win );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[������
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	p_msgman	���b�Z�[�W
 *	@param	heapID		�q�[�v�h�c
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerInit( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_MSGMAN* p_msgman, u32 heapID )
{
	p_wk->seq = WLDTIMER_VIEWER_SEQ_MAIN;

	// �L���[�p�����[�^������
	WLDTIMER_ViewerQInit( p_wk );

	// �^�C���]�[���A�j��������
	{
		int i;
		
		for( i=0; i<WLDTIMER_ZONETYPE_NUM; i++ ){
			WLDTIMER_TimeZoneAnm_Init( &p_wk->anm[ i ], p_drawsys, &sc_WLDTIMER_TIMEZONE_ANMINIT[i], WLDTIMER_VIEWER_ANM_FRAME[i], heapID );
		}
	}

	// �g�[�N���b�Z�[�W�\��
	{
		STRBUF* p_str;

		GF_BGL_BmpWinAdd(
					p_drawsys->p_bgl, &p_wk->talkwin, GF_BGL_FRAME0_S,
					WLDTIMER_SUB_TALKBMP_X, WLDTIMER_SUB_TALKBMP_Y,
					WLDTIMER_SUB_TALKBMP_SIZX, WLDTIMER_SUB_TALKBMP_SIZY, 
					WLDTIMER_SUB_TALKBMP_PAL, WLDTIMER_SUB_TALKBMP_CGX );
		GF_BGL_BmpWinDataFill( &p_wk->talkwin, 15 );

		p_str = WLDTIMER_MsgManGetStr( p_msgman, msg_00 );
		GF_STR_PrintSimple(&p_wk->talkwin,FONT_TALK,p_str,0,0,MSG_NO_PUT,NULL);

		BmpTalkWinWrite( &p_wk->talkwin, WINDOW_TRANS_ON, 
				WLDTIMER_SUB_TALKWIN_CGX, WLDTIMER_SUB_TALKWIN_PAL );
	}

	// �n�惁�b�Z�[�W������
	WLDTIMER_ViewerMsgInit( p_wk, p_drawsys, heapID );


	// �t�F�[�h�p�X�N���[���f�[�^�ǂݍ���
	{
		p_wk->p_fadescrnbuff = ArcUtil_HDL_ScrnDataGet(p_drawsys->p_handle, 
				NARC_worldtimer_world_watch_roll_NSCR, FALSE,
				&p_wk->p_fadescrndata, heapID);

	}

	// �|�P�����o���[��������
	WLDTIMER_PokeBaloon_Init( &p_wk->poke, p_drawsys, &p_wk->wnd, heapID );

	// �E�B���h�E�}�X�N�ݒ�
	WLDTIMER_ViewerWndInit( &p_wk->wnd );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerExit( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys )
{
	// �E�B���h�E�}�X�N
	WLDTIMER_ViewerWndExit( &p_wk->wnd );

	// �|�P�����o���[���j��
	WLDTIMER_PokeBaloon_Exit( &p_wk->poke, p_drawsys );

	// �t�F�[�h�X�N���[���f�[�^�j��
	sys_FreeMemoryEz( p_wk->p_fadescrnbuff );

	// �n�惁�b�Z�[�W�j��
	WLDTIMER_ViewerMsgExit( p_wk );
	
	// ���b�Z�[�W�j��
	GF_BGL_BmpWinDel( &p_wk->talkwin );
	
	// �L���[�p�����[�^�j��
	WLDTIMER_ViewerQExit( p_wk );
	
	// �^�C���]�[���A�j���j��
	{
		int i;
		
		for( i=0; i<WLDTIMER_ZONETYPE_NUM; i++ ){
			WLDTIMER_TimeZoneAnm_Exit( &p_wk->anm[i] );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[���C��
 *	
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerMain( WLDTIMER_VIEWER* p_wk, WLDTIMER_MSGMAN* p_msgman, WLDTIMER_DRAWSYS* p_drawsys )
{
	BOOL result;
	
	switch( p_wk->seq ){

	// ���C������
	case WLDTIMER_VIEWER_SEQ_MAIN:
		// �^�C���]�[���A�j�����C��
		WLDTIMER_ViewerAnmCont( p_wk, p_drawsys );

		// �|�P�����o���[�����C��
		WLDTIMER_PokeBaloon_Main( &p_wk->poke, &p_wk->wnd );
		break;
		
	// �t�F�[�h������
	case WLDTIMER_VIEWER_SEQ_FADE_INIT:
		WLDTIMER_ViewerFadeInit( p_wk, p_msgman, p_drawsys );
		p_wk->seq = WLDTIMER_VIEWER_SEQ_FADE;
		break;
	
	// �t�F�[�h����
	case WLDTIMER_VIEWER_SEQ_FADE:
		result = WLDTIMER_ViewerFade( p_wk, p_drawsys );
		if( result == TRUE ){
			p_wk->seq = WLDTIMER_VIEWER_SEQ_MAIN;
		}
		break;

	// �t�F�[�h�X�L�b�v������
	case WLDTIMER_VIEWER_SEQ_FADE_SKIP_INIT:
		WLDTIMER_ViewerFade_SkipInit01( p_wk );
		WLDTIMER_ViewerFade_Skip01( p_wk, p_drawsys );	// �P����������
		p_wk->seq = WLDTIMER_VIEWER_SEQ_FADE_SKIP;
		break;

	// �t�F�[�h�X�L�b�v����
	case WLDTIMER_VIEWER_SEQ_FADE_SKIP:
		result = WLDTIMER_ViewerFade_Skip01( p_wk, p_drawsys );
		if( result == TRUE ){
			p_wk->seq = WLDTIMER_VIEWER_SEQ_MAIN;
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E��OFF
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerTalkWinOff( WLDTIMER_VIEWER* p_wk )
{
	BmpTalkWinClear( &p_wk->talkwin, WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( &p_wk->talkwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E��ON
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerTalkWinOn( WLDTIMER_VIEWER* p_wk )
{
	GF_BGL_BmpWinOnVReq( &p_wk->talkwin );
	BmpTalkWinWrite( &p_wk->talkwin, WINDOW_TRANS_OFF, 
			WLDTIMER_SUB_TALKWIN_CGX, WLDTIMER_SUB_TALKWIN_PAL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[�����v�b�V��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�ǉ��f�[�^
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�^�C�~���O����邢
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_ViewerPushData( WLDTIMER_VIEWER* p_wk, const WLDTIMER_POINTDATA* cp_data )
{
	// �؂�ւ��A�j����
	switch( p_wk->seq ){
	case WLDTIMER_VIEWER_SEQ_MAIN:
		p_wk->seq = WLDTIMER_VIEWER_SEQ_FADE_INIT;
		break;
		
	case WLDTIMER_VIEWER_SEQ_FADE:
		p_wk->seq = WLDTIMER_VIEWER_SEQ_FADE_SKIP_INIT;
		return FALSE;
		
	// �^�C�~���O����邢�E�E�E
	case WLDTIMER_VIEWER_SEQ_FADE_SKIP_INIT:
	case WLDTIMER_VIEWER_SEQ_FADE_SKIP:
	case WLDTIMER_VIEWER_SEQ_FADE_INIT:
		return FALSE;
	}

	// �f�[�^���v�b�V��
	WLDTIMER_ViewerQPush( p_wk, cp_data );

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����f�[�^�����邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	cp_data		�`�F�b�N����f�[�^
 *
 *	@retval	TRUE	�f�[�^������
 *	@retval	FALSE	�f�[�^���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_ViewerEqualDataCheck( const WLDTIMER_VIEWER* cp_wk, const WLDTIMER_POINTDATA* cp_data )
{
	int i;
	WLDTIMER_POINTDATA point;
	BOOL result;

	for( i=0; i<WLDTIMER_VIEWER_DRAWNUM; i++ ){
		result = WLDTIMER_ViewerQGetData( cp_wk, &point, i );
		if( result == FALSE ){
			return FALSE;	// �ꏏ�̂Ȃ�
		}

		if( (point.nation == cp_data->nation) &&
			(point.area == cp_data->area) ){
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[�p�̏���
 */
//-----------------------------------------------------------------------------
// ������
static void WLDTIMER_ViewerQInit( WLDTIMER_VIEWER* p_wk )
{
	p_wk->top = 0;
	p_wk->tail = 0;
	p_wk->datacount = 0;
}
// �j��
static void WLDTIMER_ViewerQExit( WLDTIMER_VIEWER* p_wk )
{
	p_wk->top = 0;
	p_wk->tail = 0;
	p_wk->datacount = 0;
}
// �v�b�V��
static void WLDTIMER_ViewerQPush( WLDTIMER_VIEWER* p_wk, const WLDTIMER_POINTDATA* cp_data )
{
	// ����+1���擪�̎����t
	if( ((p_wk->tail + 1)%WLDTIMER_VIEWER_BUFFNUM) == p_wk->top ){
		// ���t
		// 1�f�[�^���|�b�v���ăf�[�^�i�[
		WLDTIMER_ViewerQPop( p_wk );
	}

	// �f�[�^�ݒ�
	p_wk->data[ p_wk->tail ] = *cp_data;

	// �����ʒu�𓮂���
	p_wk->tail = (p_wk->tail + 1)%WLDTIMER_VIEWER_BUFFNUM;

	// �f�[�^���J�E���g
	p_wk->datacount ++;

//	OS_Printf( "set nation %d  area %d time %d\n", cp_data->nation, cp_data->area, ((cp_data->timezone + WLDTIMER_TIMEZONE_START_HOUR) % WLDTIMER_TIMEZONE_DATANUM) );
}
// �|�b�v
static void WLDTIMER_ViewerQPop( WLDTIMER_VIEWER* p_wk )
{
	// �擪=����	�f�[�^���Ȃ�
	if( p_wk->tail == p_wk->top ){
		return ;
	}

	// �擪�ʒu�𓮂���
	p_wk->top = (p_wk->top + 1)%WLDTIMER_VIEWER_BUFFNUM;

	p_wk->datacount --;
}
//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[�p���獡�����Ă���f�[�^���擾����
 *
 *	@param	cp_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	index		tail�ʒu����̃C���f�b�N�X
 *
 *	@retval	TRUE	�f�[�^����
 *	@retval	FALSE	�f�[�^�Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_ViewerQGetData( const WLDTIMER_VIEWER* cp_wk, WLDTIMER_POINTDATA* p_data, u32 index )
{
	s32 index_num;
	
	if( index >= cp_wk->datacount ){
		return FALSE;
	}

	
	// �擾����o�b�t�@�̃C���f�b�N�X�����߂�
	index_num = (cp_wk->tail-1) - index;
	if( index_num < 0 ){
		index_num += WLDTIMER_VIEWER_BUFFNUM;
	}

	// 
	*p_data =  cp_wk->data[ index_num ];

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[�A�j���Ǘ�
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerAnmCont( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys )
{
	int i;

	for( i=0; i<WLDTIMER_ZONETYPE_NUM; i++ ){
		WLDTIMER_TimeZoneAnm_Main( &p_wk->anm[i], p_drawsys );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�F�[�h����������
 *
 *	@param	p_wk		���[�N
 *	@param	p_msgman	���b�Z�[�W�}�l�[�W��
 *	@param	p_drawsys	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerFadeInit( WLDTIMER_VIEWER* p_wk, WLDTIMER_MSGMAN* p_msgman, WLDTIMER_DRAWSYS* p_drawsys )
{
	// �t�F�[�h���[�N������
	memset( p_wk->fade, 0, sizeof(WLDTIMER_VIEWER_FADE)*WLDTIMER_VIEWER_FADE_DIV );
	p_wk->fadecount = 0;
	p_wk->fade_divnum = 0;
	
	// �V�������b�Z�[�W�ƃA�j���t���O��ݒ�
	// �A�j����������
	{
		int i;
		int drawtype;
		int zonetype;
		BOOL dataflag;
		int timezone_tmp;
		WLDTIMER_POINTDATA pointdata;

		// �A�j���t���O�����Z�b�g
		for( i=0; i<WLDTIMER_ZONETYPE_NUM; i++ ){
			WLDTIMER_TimeZoneAnm_ResetFlag( &p_wk->anm[i] );
		}

		// �|�P�����o���[�������Z�b�g
		WLDTIMER_PokeBaloon_Reset( &p_wk->poke );

		// �^�C���]�[���`�F�b�N���[�N������
		timezone_tmp = -1;

		// UNDER����TOP�ւƃf�[�^���擾
		for( i=0; i<WLDTIMER_VIEWER_DRAWNUM; i++ ){
			drawtype = WLDTIMER_VIEWER_DRAW_UND - i;
			dataflag = WLDTIMER_ViewerQGetData( p_wk, &pointdata, i );

			if( dataflag == TRUE ){

				// �]�[���^�C�v�擾
				zonetype = WLDTIMER_TIMEZONE_GetZoneType( pointdata.timezone );
					
				// �A�j���t���O�ݒ�
				WLDTIMER_TimeZoneAnm_SetFlag( &p_wk->anm[ zonetype ], drawtype, TRUE );

				// �n��f�[�^��������
				WLDTIMER_ViewerMsgWrite( p_wk, drawtype, &pointdata, p_msgman );

				// �`��^�C�v�[���^�C���]�[���e�[�u���Ɋi�[
				p_wk->drawtype_zonetype[ drawtype ] = zonetype;

				// �|�P�����o���[���ݒ�
				WLDTIMER_PokeBaloon_SetDraw( &p_wk->poke, drawtype, 
						pointdata.timezone, timezone_tmp );

				// �t�F�[�h���镪���̐������߂�
				p_wk->fade_divnum += WLDTIMER_VIEWER_FADE_DIV_ONE;

				// �^�C���]�[���ۑ�
				timezone_tmp = pointdata.timezone;
			}else{
				// �`��^�C�v�[���^�C���]�[���e�[�u���Ɋi�[
				p_wk->drawtype_zonetype[ drawtype ] = WLDTIMER_ZONETYPE_NUM;

				// �^�C���]�[��������
				timezone_tmp = -1;
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�F�[�h���C��
 *
 *	@param	p_wk		���[�N	
 *	@param	p_drawsys	�`��V�X�e��
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_ViewerFade( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys )
{
	return WLDTIMER_ViewerFade_Main( p_wk, p_drawsys, WLDTIMER_VIEWER_FADE_DIV_START );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�L�b�v���̃t�F�[�h������
 *
 *	@param	p_wk		���[�N
 *
 *	�X�L�b�v�����͂P�x�@ViewerFadeInit���P�x�s��ꂽ��ԂŌĂ΂��K�v������
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerFade_SkipInit( WLDTIMER_VIEWER* p_wk )
{
	int startnum;	// �t�F�[�h�J�n��
	int restnum;
	int i;
	
	// �t�F�[�h�J�E���^����X�L�b�v�p�t�F�[�h�J�E���^�����߂�
	startnum = p_wk->fadecount / WLDTIMER_VIEWER_FADE_DIV_START;

	// ���ꂩ��X�L�b�v���Ă����Ƃ��ɁA
	// ��C�Ƀt�F�[�h�����鐔�Ŋ���؂�邿�傤�ǂ����l�ɒ������Ă���
	restnum = (startnum % WLDTIMER_VIEWER_FADE_SKIP_ONE);
	if( restnum > 0 ){
		startnum += WLDTIMER_VIEWER_FADE_SKIP_ONE-restnum;
	}

	for( i=0; i<startnum; i++ ){
		WLDTIMER_ViewerFadeDiv_Start( p_wk, i );
	}

	// �t�F�[�h���E���g�l�𒲐�����
	p_wk->fadecount = startnum / WLDTIMER_VIEWER_FADE_SKIP_ONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�L�b�v���̃t�F�[�h����
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL WLDTIMER_ViewerFade_Skip( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys )
{
	int i;
	BOOL result;
	BOOL ret = TRUE;


	// �t�F�[�h�J�n�`�F�b�N
	if( p_wk->fadecount < WLDTIMER_VIEWER_FADE_SKIP_COUNTNUM ){
		for( i=0;  i<WLDTIMER_VIEWER_FADE_SKIP_ONE; i++ ){
			WLDTIMER_ViewerFadeDiv_Start( p_wk, 
					(p_wk->fadecount*WLDTIMER_VIEWER_FADE_SKIP_ONE)+i );
		}
	}

	// �J�E���g����
	if( (p_wk->fadecount+1) < WLDTIMER_VIEWER_FADE_SKIP_COUNTNUM ){
		p_wk->fadecount ++;
	}

	//�@�S���J�n�����Ȃ��瓮����
	for( i=0; i<p_wk->fade_divnum; i++ ){
		result = WLDTIMER_ViewerFadeDiv_Main( p_wk, i, p_drawsys );

		// 1�ł��I����ĂȂ���΁A������
		if( result == FALSE ){
			ret = FALSE;
		}
	}

	// �I�����Ƀ|�P�����o���[���̕\����OFF����
	if( ret == TRUE ){
		WLDTIMER_PokeBaloon_CleanDraw( &p_wk->poke );
		WLDTIMER_ViewerWnd1SetPos( &p_wk->wnd, 0, 0 );
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�L�b�v����	�㉺����
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerFade_SkipInit01( WLDTIMER_VIEWER* p_wk )
{
	int startnum;
	
	// �t�F�[�h�J�E���^����X�L�b�v�p�t�F�[�h�J�E���^�����߂�
	startnum = p_wk->fadecount / WLDTIMER_VIEWER_FADE_DIV_START;
	p_wk->fadecount = startnum * WLDTIMER_VIEWER_FADE_SKIP01_START;
}
static BOOL WLDTIMER_ViewerFade_Skip01( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys )
{
	return WLDTIMER_ViewerFade_Main( p_wk, p_drawsys, WLDTIMER_VIEWER_FADE_SKIP01_START );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�F�[�h���ʏ���
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	fade_timing	�^�C�~���O
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_ViewerFade_Main( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys, int fade_timing )
{
	u32 start;
	int i;
	BOOL result;
	BOOL ret = TRUE;
	
	// �t�F�[�h�J�n�`�F�b�N
	if((p_wk->fadecount % fade_timing) == 0){
		start = p_wk->fadecount / fade_timing;
		WLDTIMER_ViewerFadeDiv_Start( p_wk, start );
	}

	// �J�E���g����
	if( (p_wk->fadecount+1) < (WLDTIMER_VIEWER_FADE_DIV*fade_timing) ){
		p_wk->fadecount ++;
	}

	// ���C������
	for( i=0; i<p_wk->fade_divnum; i++ ){
		result = WLDTIMER_ViewerFadeDiv_Main( p_wk, i, p_drawsys );

		// 1�ł��I����ĂȂ���΁A������
		if( result == FALSE ){
			ret = FALSE;
		}
	}

	// �I�����Ƀ|�P�����o���[���̕\����OFF����
	if( ret == TRUE ){
		WLDTIMER_PokeBaloon_CleanDraw( &p_wk->poke );
		WLDTIMER_ViewerWnd1SetPos( &p_wk->wnd, 0, 0 );
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����t�F�[�h����	�J�n
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerFadeDiv_Start( WLDTIMER_VIEWER* p_wk, u32 idx )
{
	p_wk->fade[ idx ].start = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����t�F�[�h����	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�t�F�[�h�C���f�b�N�X
 *	@param	p_drawsys	�`��V�X�e��
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_ViewerFadeDiv_Main( WLDTIMER_VIEWER* p_wk, u32 idx, WLDTIMER_DRAWSYS* p_drawsys )
{
	u32 y;
	u32 ofs_y;
	u32 drawtype;
	u32 zonetype;
	
	if( p_wk->fade[idx].start == FALSE ){
		return FALSE;	// �͂��܂��Ă����Ȃ�
	}

	// �I���`�F�b�N
	if( p_wk->fade[idx].count >= WLDTIMER_VIEWER_FADE_DIV_SEQ_NUM ){
		return TRUE;
	}

	// �����̕`��^�C�v�A�^�C���]�[�������߂�
	y		 = (WLDTIMER_VIEWER_FADE_DIV - idx) - 1;	//1org��0org�ɂ���
	drawtype = y / WLDTIMER_VIEWER_FADE_DIV_ONE;
	ofs_y	 = y % WLDTIMER_VIEWER_FADE_DIV_ONE;
	zonetype = p_wk->drawtype_zonetype[ drawtype ];

//	OS_Printf( "fade idx[%d] y[%d] drawtype[%d] zonetype[%d] count[%d]\n", idx, y, drawtype, zonetype, p_wk->fade[idx].count );

	switch( p_wk->fade[idx].count ){

	// �Ђ�����Ԃ��t���[�����o��
	case WLDTIMER_VIEWER_FADE_DIV_SEQ_CHANGE00:
		// �Ђ�����Ԃ��X�N���[���ɐ؂�ւ�
		// �N���[���L�����N�^�̓]��
		{
			u8* p_buff;
			p_buff = WLDTIMER_ViewerDummyGetCharBuffPtr( p_wk, ofs_y );
			WLDTIMER_ViewerMsgCharTrans( p_wk, drawtype, ofs_y, p_buff );
		}
		// �w�i�̂�����[����Ђ�����Ԃ�
		{
			WLDTIMER_ViewerFadeScrn_LineTrans( p_wk, y, p_drawsys );
		}
		// OAM�E�B���h�E�ݒ�
		{
			WLDTIMER_ViewerWnd1SetPos( &p_wk->wnd, y*8, WLDTIMER_VIEWER_FADE_DIV*8 );
		}
		break;

	// �V�����ʂ��o��
	case WLDTIMER_VIEWER_FADE_DIV_SEQ_CHANGE01:
		// �V�X�N���[���o��
		// �V�L�����N�^�̓]��
		{
			u8* p_buff;
			p_buff = WLDTIMER_ViewerMsgGetCharBuffPtr( p_wk, drawtype, ofs_y );
			WLDTIMER_ViewerMsgCharTrans( p_wk, drawtype, ofs_y, p_buff );
		}
		// �w�i�̂�����[����Ђ�����Ԃ�
		{
			WLDTIMER_TimeZoneAnm_LineTrans( &p_wk->anm[ zonetype ], y, p_drawsys );
		}
		break;

	// �҂�
	default:
		break;
	}

	p_wk->fade[idx].count++;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�F�[�h�p�X�N���[���f�[�^��]������
 *
 *	@param	p_wk		���[�N
 *	@param	drawtyep	�`��^�C�v
 *	@param	y			�����W
 *	@param	p_drawsys	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerFadeScrn_LineTrans( WLDTIMER_VIEWER* p_wk, u32 y, WLDTIMER_DRAWSYS* p_drawsys )
{

	GF_BGL_ScrWriteExpand(
			p_drawsys->p_bgl, GF_BGL_FRAME2_S, 
			WLDTIMER_VIEWER_SCRN_X, 
			WLDTIMER_VIEWER_SCRN_Y+y,
			WLDTIMER_VIEWER_SCRN_SX, 1,
			p_wk->p_fadescrndata->rawData,
			WLDTIMER_VIEWER_SCRN_X,
			WLDTIMER_VIEWER_SCRN_Y+y,
			p_wk->p_fadescrndata->screenWidth/8,
			p_wk->p_fadescrndata->screenHeight/8 );

	// �]���t���O�𗧂Ă�
	GF_BGL_LoadScreenV_Req( p_drawsys->p_bgl, GF_BGL_FRAME2_S );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�}�X�N������
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerWndInit( WLDTIMER_VWND* p_wk )
{
	// �E�B���h�E�̒���OBJ�͕\�����Ȃ�
	GXS_SetVisibleWnd( GX_WNDMASK_W0|GX_WNDMASK_W1 );
	G2S_SetWndOutsidePlane( GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|GX_WND_PLANEMASK_BG2|GX_WND_PLANEMASK_BG3|GX_WND_PLANEMASK_OBJ, FALSE );
	G2S_SetWnd0InsidePlane( GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|GX_WND_PLANEMASK_BG2|GX_WND_PLANEMASK_BG3, FALSE );
	G2S_SetWnd1InsidePlane( GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|GX_WND_PLANEMASK_BG2|GX_WND_PLANEMASK_BG3, FALSE );

	WLDTIMER_ViewerWnd0SetPos( p_wk, 0, 0 );
	WLDTIMER_ViewerWnd1SetPos( p_wk, 0, 0 );

	// �E�B���h�E�f�[�^�ݒ�^�X�N
	p_wk->p_tcb = VWaitTCB_Add( WLDTIMER_ViewerWndTcb, p_wk, 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�}�X�N�j��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerWndExit( WLDTIMER_VWND* p_wk )
{
	TCB_Delete( p_wk->p_tcb );
	GXS_SetVisibleWnd( GX_WNDMASK_NONE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�}�X�N�̍��W��ݒ�
 *
 *	@param	sy	Y�J�n
 *	@param	ey	Y�I��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerWnd0SetPos( WLDTIMER_VWND* p_wk, s16 sy, s16 ey )
{
	if( sy < 0 ){
		sy = 0;
	}
	if( ey < 0 ){
		ey = 0;
	}
	p_wk->wnd0 = TRUE;
	p_wk->wnd0_sy = sy;
	p_wk->wnd0_ey = ey;
}
static void WLDTIMER_ViewerWnd1SetPos( WLDTIMER_VWND* p_wk, s16 sy, s16 ey )
{
	if( sy < 0 ){
		sy = 0;
	}
	if( ey < 0 ){
		ey = 0;
	}
	p_wk->wnd1 = TRUE;
	p_wk->wnd1_sy = sy;
	p_wk->wnd1_ey = ey;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�f�[�^�ݒ�^�X�N
 *
 *	@param	p_tcb	TCB
 *	@param	p_work	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerWndTcb( TCB_PTR p_tcb, void* p_work )
{
	WLDTIMER_VWND* p_wk = p_work;

	if( p_wk->wnd0 ){
		G2S_SetWnd0Position( 0, p_wk->wnd0_sy, 255, p_wk->wnd0_ey );
		p_wk->wnd0 = FALSE;
	}
	if( p_wk->wnd1 ){
		G2S_SetWnd1Position( 0, p_wk->wnd1_sy, 255, p_wk->wnd1_ey );
		p_wk->wnd1 = FALSE;
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[���b�Z�[�W������
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerMsgInit( WLDTIMER_VIEWER* p_wk, WLDTIMER_DRAWSYS* p_drawsys, u32 heapID )
{
	int i;
	int cgx;


	cgx = WLDTIMER_VIEWER_MSGBMP_CGX;
	for( i=0; i<WLDTIMER_VIEWER_DRAWNUM; i++ ){

		GF_BGL_BmpWinAdd(
			p_drawsys->p_bgl, &p_wk->msg[i], GF_BGL_FRAME1_S,
			WLDTIMER_VIEWER_MSGBMP_X, 
			WLDTIMER_VIEWER_MSGBMP_Y + (WLDTIMER_VIEWER_MSGBMP_SIZY*i),
			WLDTIMER_VIEWER_MSGBMP_SIZX, WLDTIMER_VIEWER_MSGBMP_SIZY,
			WLDTIMER_VIEWER_MSGBMP_PAL, cgx );

		cgx += WLDTIMER_VIEWER_MSGBMP_CGSIZ;

		GF_BGL_BmpWinDataFill( &p_wk->msg[i], 0 );

		GF_BGL_BmpWinOn( &p_wk->msg[i] );
	}

	GF_BGL_BmpWinAdd(
		p_drawsys->p_bgl, &p_wk->dummy, GF_BGL_FRAME1_S,
		WLDTIMER_VIEWER_MSGBMP_X, 
		WLDTIMER_VIEWER_MSGBMP_Y,
		WLDTIMER_VIEWER_MSGBMP_SIZX, WLDTIMER_VIEWER_MSGBMP_SIZY,
		WLDTIMER_VIEWER_MSGBMP_PAL, WLDTIMER_VIEWER_MSGBMP_CGX );
	GF_BGL_BmpWinDataFill( &p_wk->dummy, 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[���b�Z�[�W�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerMsgExit( WLDTIMER_VIEWER* p_wk )
{
	int i;

	for( i=0; i<WLDTIMER_VIEWER_DRAWNUM; i++ ){
		GF_BGL_BmpWinDel( &p_wk->msg[i] );
	}
	GF_BGL_BmpWinDel( &p_wk->dummy );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[���b�Z�[�W�̏�������
 *
 *	@param	p_wk		���[�N
 *	@param	drawtype	�`��ʒu
 *	@param	cp_data		�`��f�[�^
 *	@param	p_msgman	���b�Z�[�W�}�l�[�W��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerMsgWrite( WLDTIMER_VIEWER* p_wk, u32 drawtype, const WLDTIMER_POINTDATA* cp_data, WLDTIMER_MSGMAN* p_msgman )
{
	STRBUF* p_str;
	GF_BGL_BMPWIN* p_bmp;
	u32 zonetype;
	
	GF_ASSERT( drawtype < WLDTIMER_VIEWER_DRAWNUM );
	
	p_bmp = &p_wk->msg[drawtype];
	
	// �N���A�J���[�ŏ�����
	GF_BGL_BmpWinDataFill( p_bmp, 0 );

	// �]�[���^�C�v�擾
	zonetype = WLDTIMER_TIMEZONE_GetZoneType( cp_data->timezone );

	// �^�C�g���\��
	{
		p_str = WLDTIMER_MsgManGetStr( p_msgman, msg_02 );
		GF_STR_PrintColor(p_bmp,FONT_TALK,p_str,
				WLDTIMER_VIEWER_MSG_TITLE_X,WLDTIMER_VIEWER_MSG_TITLE_Y,
				MSG_NO_PUT, WLDTIMER_VIEWER_MSG_TITLE_COL[ zonetype ], NULL);
	}

	// ���\��
	{
		p_str = WLDTIMER_MsgManCountryGetStr( p_msgman, cp_data->nation );
		GF_STR_PrintColor(p_bmp,FONT_TALK,p_str,
				WLDTIMER_VIEWER_MSG_NATION_X,WLDTIMER_VIEWER_MSG_NATION_Y,
				MSG_NO_PUT, WLDTIMER_VIEWER_MSG_NATION_COL, NULL);
	}

	// �n��\��
	{
		p_str = WLDTIMER_MsgManPlaceGetStr( p_msgman, cp_data->nation, cp_data->area );
		GF_STR_PrintColor(p_bmp,FONT_TALK,p_str,
				WLDTIMER_VIEWER_MSG_AREA_X,WLDTIMER_VIEWER_MSG_AREA_Y,
				MSG_NO_PUT, WLDTIMER_VIEWER_MSG_NATION_COL, NULL);
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[���b�Z�[�W	�`��ʒu�Ƃ���ɂ��ʒu�̃L�����N�^�|�C���^�擾
 *			�Ђ�����Ԃ��G�t�F�N�g�p
 *
 *	@param	p_wk		���[�N
 *	@param	drawtype	�`��ʒu
 *	@param	y			�`��ʒu����Y���W
 */
//-----------------------------------------------------------------------------
static u8* WLDTIMER_ViewerMsgGetCharBuffPtr( WLDTIMER_VIEWER* p_wk, u32 drawtype, u32 y )
{
	u8* p_buff;
	GF_ASSERT( drawtype < WLDTIMER_VIEWER_DRAWNUM );

	// �L�����N�^�o�b�t�@�擾
	p_buff = p_wk->msg[ drawtype ].chrbuf;

	// �ړI�ʒu�̃|�C���^��Ԃ�
	return &p_buff[((WLDTIMER_VIEWER_MSGBMP_SIZX*y)*32)];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���[�A�[�_�~�[���b�Z�[�W	���ʒu�̃L�����N�^�|�C���^�擾
 *			�Ђ�����Ԃ��G�t�F�N�g�p
 *
 *	@param	p_wk		���[�N
 *	@param	y			�`��ʒu����Y���W
 */
//-----------------------------------------------------------------------------
static u8* WLDTIMER_ViewerDummyGetCharBuffPtr( WLDTIMER_VIEWER* p_wk, u32 y )
{
	u8* p_buff;

	// �L�����N�^�o�b�t�@�擾
	p_buff = p_wk->dummy.chrbuf;

	// �ړI�ʒu�̃|�C���^��Ԃ�
	return &p_buff[((WLDTIMER_VIEWER_MSGBMP_SIZX*y)*32)];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^�f�[�^�]��
 *
 *	@param	p_wk		���[�N
 *	@param	drawtype	�`��^�C�v
 *	@param	y			�����W
 *	@param	cp_buff		�L�����N�^�o�b�t�@
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_ViewerMsgCharTrans( WLDTIMER_VIEWER* p_wk, u32 drawtype, u32 y, const u8* cp_buff )
{
	u16 cgx;

	// �]����I�t�Z�b�g�擾
	cgx = GF_BGL_BmpWinGet_Chrofs( &p_wk->msg[ drawtype ] );

	// Y���W�̃I�t�Z�b�g�ɂ���
	cgx += (WLDTIMER_VIEWER_MSGBMP_SIZX*y);

	// �]��
	AddVramTransferManager( NNS_GFD_DST_2D_BG1_CHAR_SUB, cgx*32, 
			(void*)cp_buff, WLDTIMER_VIEWER_MSGBMP_SIZX*32 );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���]�[���A�j��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	cp_init		�������f�[�^
 *	@param	�J�E���g�ő�l
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TimeZoneAnm_Init( WLDTIMER_TIMEZONEANM* p_wk, WLDTIMER_DRAWSYS* p_drawsys, const WLDTIMER_TIMEZONEANM_INIT* cp_init, u16 count_max, u32 heapID )
{
	int i;
	
	memset( p_wk, 0, sizeof(WLDTIMER_TIMEZONEANM) );

	// �J�E���g�f�[�^
	p_wk->count_max = count_max;

	// �X�N���[���ǂݍ���
	if( cp_init->scrn_frame > 0 ){
		p_wk->scrnframe = cp_init->scrn_frame;	// ��
		for( i=0; i<p_wk->scrnframe; i++ ){
			
			p_wk->p_scrnbuff[i] = ArcUtil_HDL_ScrnDataGet( 
					p_drawsys->p_handle, cp_init->scrn_idx[i],
					FALSE, &p_wk->p_scrndata[i], heapID );
		}
	}

	// �p���b�g�ǂݍ���
	if( cp_init->pltt_frame > 0 ){
		
		p_wk->plttframe = cp_init->pltt_frame;
		p_wk->plttno	= cp_init->pltt_no;
		
		p_wk->p_plttbuff = ArcUtil_HDL_PalDataGet( 
				p_drawsys->p_handle, cp_init->pltt_idx,
				&p_wk->p_plttdata, heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���]�[���A�j��	�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TimeZoneAnm_Exit( WLDTIMER_TIMEZONEANM* p_wk )
{
	int i;
	
	// �X�N���[���j��
	if( p_wk->scrnframe > 0 ){
		for( i=0; i<p_wk->scrnframe; i++ ){
			sys_FreeMemoryEz( p_wk->p_scrnbuff[i] );
		}
	}

	// �p���b�g�ǂݍ���
	if( p_wk->plttframe > 0 ){
		sys_FreeMemoryEz( p_wk->p_plttbuff );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���]�[���A�j��	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	count		�J�E���g�l
 *	@param	count_max	�ő�J�E���g
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TimeZoneAnm_Main( WLDTIMER_TIMEZONEANM* p_wk, WLDTIMER_DRAWSYS* p_drawsys )
{
	int i;
	u32 pltt_frame, scrn_frame;

	// �t���[���v�Z
	scrn_frame = (p_wk->count * p_wk->scrnframe) / p_wk->count_max;
	pltt_frame = (p_wk->count * p_wk->plttframe) / p_wk->count_max;

	// �J�E���g����
	p_wk->count = (p_wk->count + 1) % p_wk->count_max;
		
	// �X�N���[���A�j��
	if( (p_wk->scrnframe>0) && (scrn_frame!=p_wk->scrnframe_now) ){
		p_wk->scrnframe_now = scrn_frame;	// �ۑ�
		for( i=0; i<WLDTIMER_VIEWER_DRAWNUM; i++ ){
			if( p_wk->drawflag[ i ] == TRUE ){
				// �]������
				GF_BGL_ScrWriteExpand(
						p_drawsys->p_bgl, GF_BGL_FRAME2_S, 
						WLDTIMER_VIEWER_SCRN_X, 
						WLDTIMER_VIEWER_SCRN_Y+(WLDTIMER_VIEWER_SCRN_SY*i),
						WLDTIMER_VIEWER_SCRN_SX, WLDTIMER_VIEWER_SCRN_SY,
						p_wk->p_scrndata[ p_wk->scrnframe_now ]->rawData,
						WLDTIMER_VIEWER_SCRN_X,
						WLDTIMER_VIEWER_SCRN_Y+(WLDTIMER_VIEWER_SCRN_SY*i),
						p_wk->p_scrndata[ p_wk->scrnframe_now ]->screenWidth/8,
						p_wk->p_scrndata[ p_wk->scrnframe_now ]->screenHeight/8 );

				// �]���t���O�𗧂Ă�
				GF_BGL_LoadScreenV_Req( p_drawsys->p_bgl, GF_BGL_FRAME2_S );
			}
		}
	}

	// �p���b�g�A�j��
	if( (p_wk->plttframe>0) && (pltt_frame!=p_wk->plttframe_now) ){
		BOOL result;
		u8* p_data;
		
		p_wk->plttframe_now = pltt_frame;	// �ۑ�

		// �]��
		p_data = (u8*)p_wk->p_plttdata->pRawData;
		result = AddVramTransferManager( NNS_GFD_DST_2D_BG_PLTT_SUB,
				p_wk->plttno*32, &p_data[p_wk->plttframe_now*32], 32 );
		GF_ASSERT( result );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j��ONOFF�t���O��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	drawtype	�`��^�C�v
 *	@param	flag		�t���O
 *	
 *	drawtype
		WLDTIMER_VIEWER_DRAW_TOP,	// ��
		WLDTIMER_VIEWER_DRAW_MID,	// ��
		WLDTIMER_VIEWER_DRAW_UND,	// ��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TimeZoneAnm_SetFlag( WLDTIMER_TIMEZONEANM* p_wk, u32 drawtype, BOOL flag )
{
	GF_ASSERT( drawtype < WLDTIMER_VIEWER_DRAWNUM );
	p_wk->drawflag[ drawtype ] = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���t���O��Ԃ�S��OFF�ɂ���
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TimeZoneAnm_ResetFlag( WLDTIMER_TIMEZONEANM* p_wk )
{
	memset( p_wk->drawflag, 0, sizeof(u8)*4 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���f�[�^�����C���œ]������
 *
 *	@param	p_wk		���[�N
 *	@param	y			Y���W
 *	@param	p_drawsys	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_TimeZoneAnm_LineTrans( WLDTIMER_TIMEZONEANM* p_wk, u32 y, WLDTIMER_DRAWSYS* p_drawsys )
{
	// �t���[���̃X�N���[���f�[�^��Y���C��������]��
	if( p_wk->scrnframe>0 ){
		GF_BGL_ScrWriteExpand(
				p_drawsys->p_bgl, GF_BGL_FRAME2_S, 
				WLDTIMER_VIEWER_SCRN_X, 
				WLDTIMER_VIEWER_SCRN_Y+y,
				WLDTIMER_VIEWER_SCRN_SX, 1,
				p_wk->p_scrndata[ p_wk->scrnframe_now ]->rawData,
				WLDTIMER_VIEWER_SCRN_X,
				WLDTIMER_VIEWER_SCRN_Y+y,
				p_wk->p_scrndata[ p_wk->scrnframe_now ]->screenWidth/8,
				p_wk->p_scrndata[ p_wk->scrnframe_now ]->screenHeight/8 );

		// �]���t���O�𗧂Ă�
		GF_BGL_LoadScreenV_Req( p_drawsys->p_bgl, GF_BGL_FRAME2_S );
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����o���[��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	p_wnd		�E�B���h�E�V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBaloon_Init( WLDTIMER_POKEBALLOON* p_wk, WLDTIMER_DRAWSYS* p_drawsys, WLDTIMER_VWND* p_wnd, u32 heapID )
{
	int i;
	CLACT_HEADER header;
	BOOL result;
	CLACT_ADD_SIMPLE add = {NULL};

	// 0�N���A
	memset( p_wk, 0, sizeof(WLDTIMER_POKEBALLOON) );

	// �f�[�^������
	p_wk->drawtype	= WLDTIMER_VIEWER_DRAW_UND;	//  ������\��
	p_wk->wait		= WLDTIMER_VIEWER_POKEBLN_WAIT;
	

	// �o�^��{���쐬
	add.ClActSet	= p_drawsys->p_clactset;
	add.ClActHeader = &header;
	add.pri			= WLDTIMER_VIEWER_POKEBLN_SFT_PRI;
	add.DrawArea	= NNS_G2D_VRAM_TYPE_2DSUB;
	add.heap		= heapID;

	// �p���b�g�͋���
	p_wk->p_res[ 0 ][ 1 ] = CLACT_U_ResManagerResAddArcPltt_ArcHandle( 
			p_drawsys->p_resman[1], p_drawsys->p_handle,
			WLDTIMER_VIEWER_POKEBLN_NCLR_GET(i),
			FALSE, WLDTIMER_VIEWER_POKEBLN_NCLR_CONTID, 
			NNS_G2D_VRAM_TYPE_2DSUB, WLDTIMER_VIEWER_POKEBLN_PL_NUM, heapID );
	result =CLACT_U_PlttManagerSetCleanArea( p_wk->p_res[ 0 ][ 1 ] );
	GF_ASSERT( result );
	CLACT_U_ResManagerResOnlyDelete( p_wk->p_res[ 0 ][ 1 ] );
		
	
	
	// �|�P�����̃O���t�B�b�N�ǂݍ���
	for( i=0; i<WLDTIMER_TIME_POKE_NUM; i++ ){

		// �L�����N�^�ǂݍ���
		p_wk->p_res[ i ][ 0 ] = CLACT_U_ResManagerResAddArcChar_ArcHandle( 
				p_drawsys->p_resman[0], p_drawsys->p_handle,
				WLDTIMER_VIEWER_POKEBLN_NCGR_GET(i),
				FALSE, i, NNS_G2D_VRAM_TYPE_2DSUB, heapID );
		
		// �Z��
		p_wk->p_res[ i ][ 2 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_drawsys->p_resman[2], p_drawsys->p_handle,
				WLDTIMER_VIEWER_POKEBLN_NCER_GET(i),
				FALSE, i, CLACT_U_CELL_RES, heapID );

		// �Z���A�j��
		p_wk->p_res[ i ][ 3 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_drawsys->p_resman[3], p_drawsys->p_handle,
				WLDTIMER_VIEWER_POKEBLN_NANR_GET(i),
				FALSE, i, CLACT_U_CELLANM_RES, heapID );

		// VRAM�]��
		result =CLACT_U_CharManagerSetAreaCont( p_wk->p_res[ i ][ 0 ] );
		GF_ASSERT( result );
		CLACT_U_ResManagerResOnlyDelete( p_wk->p_res[ i ][ 0 ] );

		// �w�b�_�[�쐬
		CLACT_U_MakeHeader( &header, i, WLDTIMER_VIEWER_POKEBLN_NCLR_CONTID, i, i,
				CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
				0, WLDTIMER_VIEWER_POKEBLN_BG_PRI,
				p_drawsys->p_resman[0],
				p_drawsys->p_resman[1],
				p_drawsys->p_resman[2],
				p_drawsys->p_resman[3],
				NULL, NULL );

		// �A�N�^�[�쐬
		p_wk->p_act[ i ] = CLACT_AddSimple( &add );

		// �I�[�g�A�j��ON
		CLACT_SetAnmFlag( p_wk->p_act[ i ], TRUE );
		CLACT_SetAnmFrame( p_wk->p_act[ i ], FX32_ONE );
	}

	// OAM��\��
	WLDTIMER_PokeBaloon_CleanDraw( p_wk );
	WLDTIMER_ViewerWnd1SetPos( p_wnd, 0, 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����o���[��	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_drawsys	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBaloon_Exit( WLDTIMER_POKEBALLOON* p_wk, WLDTIMER_DRAWSYS* p_drawsys )
{
	int i, j;

	// �����Ȃ��Ă����~
	Snd_SeStopBySeqNo( WLDTIMER_SND_BALLOON, 0 );	// ���D����~


	// ���\�[�X�ƃ��[�N�j��
	for( i=0; i<WLDTIMER_TIME_POKE_NUM; i++ ){
		// ���[�N�j��
		CLACT_Delete( p_wk->p_act[i] );

		// VRAM�J��
		CLACT_U_CharManagerDelete( p_wk->p_res[i][0] );
		if( i==0 ){
			// �p���b�g�j��
			CLACT_U_PlttManagerDelete( p_wk->p_res[i][1] );
			CLACT_U_ResManagerResDelete( p_drawsys->p_resman[1], p_wk->p_res[i][1] );
		}
		
		// ���\�[�X�j��
		for( j=0; j<WLDTIMER_RESMAN_NUM; j++ ){
			if(j!=1){	// �p���b�g�ȊO�j��
				CLACT_U_ResManagerResDelete( p_drawsys->p_resman[j], p_wk->p_res[i][j] );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����o���[��	�A�j���f�[�^�@���Z�b�g
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBaloon_Reset( WLDTIMER_POKEBALLOON* p_wk )
{
	memset( p_wk->drawflag, 0, sizeof(u8)*4 );
	memset( p_wk->pokegra, 0, sizeof(u8)*4 );

	p_wk->drawtype	= WLDTIMER_VIEWER_DRAW_UND;
	p_wk->wait		= WLDTIMER_VIEWER_POKEBLN_WAIT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����o���[��	�`��t���O�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	drawtype	�`��^�C�v
 *	@param	timezone	�^�C���]�[��
 *	@param	last_timezone	�P�܂��̃^�C���]�[��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBaloon_SetDraw( WLDTIMER_POKEBALLOON* p_wk, u8 drawtype, u8 timezone, u8 last_timezone )
{
	int  last_pokegra;
	p_wk->drawflag[ drawtype ]	= TRUE;
	p_wk->pokegra[ drawtype ]	= WLDTIMER_TIMEZONE_GetMons( timezone );

	// ���쏉����
	WLDTIMER_PokeBln_MoveInit( &p_wk->move[drawtype],
			&sc_WLDTIMER_POKEBLN_MOVEDATA_TBL[ p_wk->pokegra[drawtype] ][drawtype] );

	// 1�O�̃o���[���̏�equal�t���O�ݒ�
	// ���̃o���[���̉�equal�t���O�ݒ�
	if( last_timezone<WLDTIMER_TIMEZONE_DATANUM ){
		last_pokegra = WLDTIMER_TIMEZONE_GetMons( last_timezone );
		if( last_pokegra == p_wk->pokegra[ drawtype ] ){
			if( drawtype < WLDTIMER_VIEWER_DRAW_UND ){
				WLDTIMER_PokeBln_TopEqualDataSet( &p_wk->move[drawtype+1], TRUE );
				WLDTIMER_PokeBln_UnderEqualDataSet( &p_wk->move[drawtype], TRUE );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����o���[��	�`��N���[��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBaloon_CleanDraw( WLDTIMER_POKEBALLOON* p_wk )
{
	int i;
	for( i=0; i<WLDTIMER_TIME_POKE_NUM; i++ ){
		CLACT_SetDrawFlag( p_wk->p_act[i], FALSE );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����o���[��	���C������
 *
 *	@param	p_wk	���[�N
 *	@param	p_wnd	�E�B���h�E�V�X�e��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBaloon_Main( WLDTIMER_POKEBALLOON* p_wk, WLDTIMER_VWND* p_wnd )
{
	BOOL result;
	
	// �J�n�E�G�C�g
	if( p_wk->wait > 0 ){
		p_wk->wait --;

		// �E�G�C�g���O�Ȃ瓮��J�n����������
		if( p_wk->wait == 0 ){
			result = WLDTIMER_PokeBaloon_Start( p_wk, p_wk->drawtype, p_wnd );
			if( result == TRUE ){
				Snd_SePlay( WLDTIMER_SND_BALLOON );	// ���D��
			}
		}
		return ;
	}
	
	// ����
	result = WLDTIMER_PokeBaloon_Move( p_wk, p_wk->drawtype, p_wnd );

	if( result == TRUE ){
		if( (p_wk->drawtype-1) >= 0 ){
			p_wk->drawtype--;
			WLDTIMER_PokeBaloon_Start( p_wk, p_wk->drawtype, p_wnd );
		}else{
			Snd_SeStopBySeqNo( WLDTIMER_SND_BALLOON, 0 );	// ���D����~
			p_wk->drawtype = WLDTIMER_VIEWER_DRAW_UND;
			p_wk->wait = WLDTIMER_VIEWER_POKEBLN_WAIT;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���[���I�u�W�F�N�g	����J�n
 *
 *	@param	p_wk		���[�N
 *	@param	drawtype	�`��^�C�v
 *	@param	p_wnd		�E�B���h�E
 *
 *	@retval	TRUE	����J�n����
 *	@retval	FALSE	�J�n���Ȃ�����
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_PokeBaloon_Start( WLDTIMER_POKEBALLOON* p_wk, u32 drawtype, WLDTIMER_VWND* p_wnd )
{
	// �A�j���L���`�F�b�N
	if( p_wk->drawflag[ drawtype ] == FALSE ){
		return FALSE;
	}

	// ���샊�Z�b�g
	WLDTIMER_PokeBln_MoveReset( &p_wk->move[drawtype], p_wnd );

	// �\��ON
	CLACT_SetDrawFlag( p_wk->p_act[ p_wk->pokegra[ drawtype ] ], TRUE );

	// ���W�ݒ�
	WLDTIMER_PokeBln_ActSetMatrix( p_wk, drawtype );

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���[���I�u�W�F����
 *
 *	@param	p_wk		���[�N
 *	@param	drawtype	���삳����`��^�C�v
 *	@param	p_wnd		�E�B���h�E�V�X�e��
 *
 *	@retval	TRUE	���슮��
 *	@retval	FALSE	����r��
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_PokeBaloon_Move( WLDTIMER_POKEBALLOON* p_wk, u32 drawtype, WLDTIMER_VWND* p_wnd )
{
	BOOL result;

	// �A�j���I���`�F�b�N
	if( p_wk->drawflag[ drawtype ] == FALSE ){
		return TRUE;
	}

	// �A�j�����C��
	result = WLDTIMER_PokeBln_MoveMain( &p_wk->move[ drawtype ], p_wnd );	// ����
	WLDTIMER_PokeBln_ActSetMatrix( p_wk, drawtype );				// ���W�ݒ�

	if( result == TRUE ){
		CLACT_SetDrawFlag( p_wk->p_act[ p_wk->pokegra[ drawtype ] ], FALSE );
	}

	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����o���[���@���샏�[�N������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBln_MoveInit( WLDTIMER_POKEBLN_MOVE* p_wk, const WLDTIMER_POKEBLN_MOVEDATA* cp_data )
{
	p_wk->cp_data	= cp_data;
	p_wk->x			= cp_data->sx;
	p_wk->y			= cp_data->sy + WLDTIMER_VIEWER_POKEBLN_OAM_HFSIZ;
	p_wk->count		= 0;
	p_wk->wcount	= 0;
	p_wk->under_equaldata	= FALSE;
	p_wk->top_equaldata		= FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�̈ʒu�������|�P�������̃t���O�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	mskon 
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBln_UnderEqualDataSet( WLDTIMER_POKEBLN_MOVE* p_wk, BOOL flag )
{
	p_wk->under_equaldata = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�̈ʒu�������|�P�������̃t���O�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	mskon 
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBln_TopEqualDataSet( WLDTIMER_POKEBLN_MOVE* p_wk, BOOL flag )
{
	p_wk->top_equaldata = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���상�C��
 *
 *	@param	p_wk	���[�N
 *	@param	p_wnd	�E�B���h�E
 *
 *	@retval	TRUE	�I��
 *	@retval	FLASE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WLDTIMER_PokeBln_MoveMain( WLDTIMER_POKEBLN_MOVE* p_wk, WLDTIMER_VWND* p_wnd )
{
	
	// �I���`�F�b�N
	if( p_wk->count > p_wk->cp_data->countmax ){
		return TRUE;
	}

	// ���W�����߂�
	WLDTIMER_PokeBln_MatrixCalc( p_wk );


	// �J�E���g����
	p_wk->count ++;	// �ʏ퓮��
	if( (p_wk->wcount+1) > p_wk->cp_data->wcountmax ){	// �����A�j��
		p_wk->wcount = 0;
	}else{
		p_wk->wcount++;
	}

	// �E�B���h�E�}�X�N�ݒ�
	WLDTIMER_PokeBln_WndMskSet( p_wk, p_wnd );

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����o���[������	���Z�b�g
 *
 *	@param	p_wk	���[�N
 *	@param	p_wnd	�E�B���h�E
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBln_MoveReset( WLDTIMER_POKEBLN_MOVE* p_wk, WLDTIMER_VWND* p_wnd )
{
	p_wk->count = 0;
	p_wk->wcount = 0;

	// ���W�����߂�
	WLDTIMER_PokeBln_MatrixCalc( p_wk );
	// �E�B���h�E�}�X�N�ݒ�
	WLDTIMER_PokeBln_WndMskSet( p_wk, p_wnd );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����o���[���\�����W���擾
 *
 *	@param	cp_wk		���[�N
 *	@param	p_pos		�\�����W
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBln_MoveGetPos( const WLDTIMER_POKEBLN_MOVE* cp_wk, VecFx32* p_pos )
{
	p_pos->x = cp_wk->x << FX32_SHIFT;
	p_pos->y = (cp_wk->y << FX32_SHIFT) + WLDTIMER_SF_MAT_Y;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�̌v�Z�Ɗi�[�̂ݍs��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBln_MatrixCalc( WLDTIMER_POKEBLN_MOVE* p_wk )
{
	u16 rota;
	s16 sy, ey;

	// �オ�ꏏ
	if( p_wk->top_equaldata == FALSE ){
		ey = p_wk->cp_data->ey - WLDTIMER_VIEWER_POKEBLN_OAM_HFSIZ;
	}else{
		ey = p_wk->cp_data->ey;
	}

	// �����ꏏ
	if( p_wk->under_equaldata == FALSE ){
	  	sy = p_wk->cp_data->sy + WLDTIMER_VIEWER_POKEBLN_OAM_HFSIZ;
	}else{
	  	sy = p_wk->cp_data->sy;
	}

	// �ʏ�ړ����W�v�Z
	p_wk->x = p_wk->cp_data->sx + (((p_wk->cp_data->ex - p_wk->cp_data->sx) * p_wk->count) / p_wk->cp_data->countmax);
	p_wk->y = sy + (((ey - sy) * p_wk->count) / p_wk->cp_data->countmax);

	// ���v�Z
	rota = (p_wk->wcount*0xffff)/p_wk->cp_data->wcountmax;
	p_wk->x += FX_Mul( FX32_CONST(p_wk->cp_data->wx), FX_SinIdx( rota ) ) >> FX32_SHIFT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�}�X�N��ݒ�
 *
 *	@param	cp_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBln_WndMskSet( const WLDTIMER_POKEBLN_MOVE* cp_wk, WLDTIMER_VWND* p_wnd )
{
	// Y���W�ŃE�B���h�E�}�X�N��ݒ肷��
	WLDTIMER_ViewerWnd0SetPos( p_wnd, 0, 0 );	// �ݒ�̉���
	// ���ɐݒ肷�邩��������
	if( cp_wk->under_equaldata == FALSE ){
		if( cp_wk->y > (cp_wk->cp_data->sy - WLDTIMER_VIEWER_POKEBLN_OAM_HFSIZ) ){
			WLDTIMER_ViewerWnd0SetPos( p_wnd, cp_wk->cp_data->sy, 
					cp_wk->cp_data->sy+WLDTIMER_VIEWER_POKEBLN_OAM_SIZ );
		}
	}
	// ��ɐݒ肷�邩�`�F�b�N
	if( cp_wk->top_equaldata == FALSE ){
		if( (cp_wk->y - WLDTIMER_VIEWER_POKEBLN_OAM_HFSIZ) <= cp_wk->cp_data->ey ){
			WLDTIMER_ViewerWnd0SetPos( p_wnd, cp_wk->cp_data->ey-WLDTIMER_VIEWER_POKEBLN_OAM_SIZ, 
					cp_wk->cp_data->ey );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̃|�P�����ɍ��̍��W��ݒ肷��
 *	
 *	@param	p_wk		���[�N
 *	@param	drawtype	�`��^�C�v
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_PokeBln_ActSetMatrix( WLDTIMER_POKEBALLOON* p_wk, u32 drawtype )
{
	VecFx32 pos;
	CLACT_WORK_PTR p_obj;

	WLDTIMER_PokeBln_MoveGetPos( &p_wk->move[ drawtype ], &pos );	// ���W�ݒ�
	
	p_obj = p_wk->p_act[ p_wk->pokegra[ drawtype ] ];
	CLACT_SetMatrix( p_obj, &pos );
}




//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���]�[������]�[���^�C�v���擾
 *
 *	@param	timezone	�^�C���]�[��
 *
 *	@return	�]�[���^�C�v
 */
//-----------------------------------------------------------------------------
static u16 WLDTIMER_TIMEZONE_GetZoneType( u32 timezone )
{
	GF_ASSERT( timezone<WLDTIMER_TIMEZONE_DATANUM );
	return sc_WLDTIMER_TIMEZONE_DATA[timezone].zone;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���]�[������\�������X�^�[���擾
 *
 *	@param	timezone	�^�C���]�[��
 *
 *	@return	�\�������X�^�[
 */
//-----------------------------------------------------------------------------
static u16 WLDTIMER_TIMEZONE_GetMons( u32 timezone )
{
	GF_ASSERT( timezone<WLDTIMER_TIMEZONE_DATANUM );
	return sc_WLDTIMER_TIMEZONE_DATA[timezone].mons;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�}�l�[�W��������
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_MsgManInit( WLDTIMER_MSGMAN* p_wk, u32 heapID )
{
	// ��{�}�l�[�W���쐬
	{
		p_wk->p_msgman = MSGMAN_Create(MSGMAN_TYPE_NORMAL,ARC_MSG,NARC_msg_worldtimer_dat,heapID );
	}

	// �n�惁�b�Z�[�W
	{
		p_wk->p_wordset = WORDSET_CreateEx( WORDSET_DEFAULT_SETNUM, WORDSET_COUNTRY_BUFLEN, heapID );
	}

	// ���L���b�Z�[�W�̈�m��
	p_wk->p_msgstr = STRBUF_Create( WLDTIMER_MSGMAN_STRBUFNUM, heapID );
	p_wk->p_msgtmp = STRBUF_Create( WLDTIMER_MSGMAN_STRBUFNUM, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�}�l�[�W���j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_MsgManExit( WLDTIMER_MSGMAN* p_wk )
{
	// ��{�}�l�[�W��
	{
		MSGMAN_Delete(p_wk->p_msgman);
	}

	// �n��}�l�[�W��
	{
		WORDSET_Delete( p_wk->p_wordset );
	}

	// ���L���b�Z�[�W�o�b�t�@
	STRBUF_Delete( p_wk->p_msgstr );
	STRBUF_Delete( p_wk->p_msgtmp );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W���擾����
 *
 *	@param	p_wk	���[�N
 *	@param	msg		���b�Z�[�W
 *
 *	@return	������
 */
//-----------------------------------------------------------------------------
static STRBUF* WLDTIMER_MsgManGetStr( WLDTIMER_MSGMAN* p_wk, u32 msg )
{
	MSGMAN_GetString( p_wk->p_msgman, msg, p_wk->p_msgstr );
	return p_wk->p_msgstr;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W���擾
 *
 *	@param	p_wk		���[�N
 *	@param	msg			���b�Z�[�W
 *	@param	p_str		�o�b�t�@
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_MsgManGetStrBuff( WLDTIMER_MSGMAN* p_wk, u32 msg, STRBUF* p_str )
{
	MSGMAN_GetString( p_wk->p_msgman, msg, p_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̖��O���擾
 *
 *	@param	p_wk		���[�N
 *	@param	nationID	���h�c
 *
 *	@return	������
 */
//-----------------------------------------------------------------------------
static STRBUF* WLDTIMER_MsgManCountryGetStr( WLDTIMER_MSGMAN* p_wk, u32 nationID )
{
	MSGMAN_GetString( p_wk->p_msgman, msg_03, p_wk->p_msgtmp );
	WORDSET_RegisterCountryName( p_wk->p_wordset, 0, nationID );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_msgstr, p_wk->p_msgtmp );
	return p_wk->p_msgstr;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n�於���擾
 *
 *	@param	p_wk		���[�N
 *	@param	nationID	���h�c
 *	@param	areaID		�n��h�c
 *
 *	@return	������
 */
//-----------------------------------------------------------------------------
static STRBUF* WLDTIMER_MsgManPlaceGetStr( WLDTIMER_MSGMAN* p_wk, u32 nationID, u32 areaID )
{
	MSGMAN_GetString( p_wk->p_msgman, msg_03, p_wk->p_msgtmp );
	WORDSET_RegisterLocalPlaceName( p_wk->p_wordset, 0, nationID, areaID );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_msgstr, p_wk->p_msgtmp );
	return p_wk->p_msgstr;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���E�n�}�\���n��f�[�^����ŏ��ɔ��������f�[�^��Ԃ�
 *
 *	@param	cp_data	���E�n�}�\���n��f�[�^
 *
 *	@retval	�f�[�^��INDEX		
 *	@retval	WFLBY_WLDTIMER_DATA_MAX	�݂���Ȃ�����
 */
//-----------------------------------------------------------------------------
static u32 WLDTIMER_WFLBYDATA_GetFirst( const WFLBY_WLDTIMER* cp_data )
{
	int i;
	u8 dataflag;

	for( i=0; i<WFLBY_WLDTIMER_DATA_MAX; i++ ){
		dataflag = WFLBY_WLDTIMER_GetDataFlag( cp_data, i );
		if( dataflag == TRUE ){
			return i;
		}
	}
	return WFLBY_WLDTIMER_DATA_MAX;
}




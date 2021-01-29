//============================================================================================
/**
 * @file	vs_demo.c
 * @brief	�ʐM�ΐ�f��
 * @author	Hiroyuki Nakamura
 * @date	06.04.26
 */
//============================================================================================
#include "common.h"
#include "gflib/strbuf_family.h"
#include "system/procsys.h"
#include "system/palanm.h"
#include "system/lib_pack.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "system/fontoam.h"
#include "system/fontproc.h"
#include "system/msgdata.h"
#include "system/wordset.h"
#include "system/numfont.h"
#include "system/wipe.h"
#include "system/particle.h"
#include "system/snd_tool.h"
#include "system/buflen.h"
#include "poketool/pokeparty.h"
#include "poketool/pokeicon.h"
#include "poketool/tr_tool.h"
#include "itemtool/item.h"
#include "application/app_tool.h"
#include "application/p_status.h"
#include "application/pokelist.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_pokelist.h"
#include "msgdata/msg_taisen_rokuga.h"
#include "communication/wm_icon.h"
#include "communication/comm_system.h"
#include "communication/comm_def.h"
#include "communication/comm_state.h"

#include "battle_input.h"
#include "graphic/pl_batt_obj_def.h"
#include "../particledata/particledata.h"
#include "../particledata/particledata.naix"
#include "../field/zonedata.h"

#include "vs_demo.h"
#include "vs_demo_def.h"
#include "vs_demo_gra.naix"
#include "application/plist_gra.naix"
#include "application/pst_gra.naix"

#include "system/bmp_menu.h"
#include "system/window.h"
#include "savedata/battle_rec.h"
#include "communication/comm_tool.h"
#include "communication/comm_info.h"
#include "comm_command_battle.h"
#include "itemtool/myitem.h"


//============================================================================================
//	�萔��`
//============================================================================================
typedef struct _VSD_WORK	VSD_WORK;
typedef u8 (*VSD_FUNC)(VSD_WORK*);

// �}���`�m�F�p�|�P�������[�N
typedef struct {
	u32	chr_arc;	// �L�����f�[�^�̃A�[�NID
	u16 mons;		// �|�P�����ԍ�
	u16	hp;			// HP
	u16	mhp;		// MHP
	u16	item;		// �A�C�e��
	u8	lv;			// ���x��
	u8	sex;		// ����
	u8	sex_put;	// ���ʕ\��
	u8	egg;		// �^�}�S�t���O
	u8	cb;			// �J�X�^���{�[��
	u8	form;		// �t�H����No
	u32	st;			// ��Ԉُ�
}VSD_POKE;

// �f����ʃ��[�N
struct _VSD_WORK {
	VS_DEMO_DATA * dat;		// �O���ݒ�f�[�^
	const VSD_FUNC * func;	// �����e�[�u��

	PALETTE_FADE_PTR pfd;	// �p���b�g�t�F�[�h�f�[�^

	GF_BGL_INI * bgl;					// BGL�f�[�^
	GF_BGL_BMPWIN	win[VSD_BMP_MAX];	// BMP�E�B���h�E

	// �Z���A�N�^�[
	CATS_SYS_PTR	csp;
	CATS_RES_PTR	crp;
	CATS_ACT_PTR	cap[VS_CLA_PLMAX];
	u32	cap_max;				// �g�p���Ă���Z���A�N�^�[��

	/******************************* �}���`�m�F�p *************************************/
	VSD_POKE	poke[6];	// �|�P�������[�N

	u16	plate_top[PL_PANEL_SX*PL_PANEL_SY];		// �擪�̃v���[�g�̃X�N���[���f�[�^
	u16	plate_normal[PL_PANEL_SX*PL_PANEL_SY];	// �ʏ�̃v���[�g�̃X�N���[���f�[�^
	u16	plate_none[PL_PANEL_SX*PL_PANEL_SY];	// ���Ȃ��ꍇ�̃v���[�g�̃X�N���[���f�[�^

	u16	pal[16*3];						// �p���b�g�ۑ�
	u16	plate_bg[2][PL_PANEL_SX*24];	// �X�N���[���̂��߂�BG�X�N���[���ۑ��̈�

	/******************************* VS��ʗp *************************************/
	FONTOAM_SYS_PTR fsp;		// �t�H���gOAM�V�X�e��
	FONTOAM_OBJ_PTR	fop[4];		// �t�H���gOAM�f�[�^

	GF_G3DMAN * g3dm;
	PTC_PTR	ptc;		// �p�[�e�B�N���V�X�e��
	void * ptm;			// �p�[�e�B�N���Ŏg�p����q�[�v�G���A

	u8	ball_gx[4];		// �莝���{�[���̊�{X���W(0,1=�������A2,3=�G��)
	u8	ball_gy[4];		// �莝���{�[���̊�{Y���W(0,1=�������A2,3=�G��)
	u8	name_gx[4];		// �g���[�i�[���̊�{X���W(0,1=�������A2,3=�G��)
	u8	name_gy[4];		// �g���[�i�[���̊�{Y���W(0,1=�������A2,3=�G��)

	s32	scr_spd;		// �X�N���[�����x

	s16	old_sx;			// ��ʂ�X�h�ꕝ�L��
	s16	old_sy;			// ��ʂ�Y�h�ꕝ�L��

	s8	shake_x;		// ��ʂ�X�h�ꕝ
	s8	shake_y;		// ��ʂ�Y�h�ꕝ

	u8	push_mv;		// �v���[�g�����o������

	/******************************************************************************/

	u8	seq;		// ���C���V�[�P���X
	u8	func_seq;	// �����V�[�P���X
	u8	wait;		// �E�F�C�g
	u8	cnt;		// �ėp�J�E���^
	
	u16 save_seq0;
	u16 save_seq1;

//--------------------------------------------------------------
//	��������v���`�i�p�ɒǉ�
//--------------------------------------------------------------
	GF_BGL_BMPWIN	talk_win;			//��b�E�B���h�E
	GF_BGL_BMPWIN	sub_talk_win;		//��b�E�B���h�E(���Ɂu�͂�/�������v�E�B���h�E���o����)
	GXBG0As bg_2d3d;	// BG0�ʂ̎g�p

	MSGDATA_MANAGER *rec_msgman;
	WORDSET *rec_wordset;
	STRBUF *rec_talkmsg_buf;			///<Alloc����������o�b�t�@�ւ̃|�C���^
	int talk_msgid;
	int rec_win_mode;					///<�퓬�^��Ď��֐��̃��[�h
	BMPMENU_WORK *yesno_menu;
	void *time_icon;					///<���v�A�C�R��
	
	int rec_occ;						///<TRUE:�^��Z�[�u�\  FALSE:�֎~
	LOAD_RESULT load_result;			///<���ɘ^��f�[�^�����݂��Ă��邩�ǂ���
	BOOL dp_entry;						///<TRUE:DP���������Ă���
};


#define	P1_NAME_PX		( 32 )
#define	P1_NAME_PY		( 8 )
#define	P1_HP_PX		( 56 )
#define	P1_HP_PY		( 32 )
#define	P1_HP_SX		( 24 )
#define	P1_HP_SY		( 8 )
#define	P1_HPGAGE_PX	( 48+16 )
#define	P1_HPGAGE_PY	( 24 )
#define	P1_HPGAGE_SX	( 64 )
#define	P1_HPGAGE_SY	( 8 )
#define	P1_LV_PX		( 0 )
#define	P1_LV_PY		( 32 )


//============================================================================================
//	�v���g�^�C�v�錾
//============================================================================================
static void VSDemo_Main( TCB_PTR tcb, void * work );
static void VSD_VramSet(void);

static u8 VSD_SeqInit( VSD_WORK * wk );
static u8 VSD_SeqWait( VSD_WORK * wk );
static u8 VSD_SeqScrInMain( VSD_WORK * wk );
static u8 VSD_SeqShake( VSD_WORK * wk );
static u8 VSD_SeqScrOut( VSD_WORK * wk );
static u8 VSD_SeqShakeWaitSet( VSD_WORK * wk );
static u8 VSD_SeqPushCheck( VSD_WORK * wk );
static u8 VSD_SeqPush( VSD_WORK * wk );
static u8 VSD_SeqLeftPush( VSD_WORK * wk );
static u8 VSD_SeqRightPush( VSD_WORK * wk );
static u8 VSD_SeqEndSet( VSD_WORK * wk );
static u8 VSD_SeqEnd( TCB_PTR tcb, VSD_WORK * wk );
static u8 VSD_SeqPuchWaitSet( VSD_WORK * wk );
static u8 VSD_SeqResultOpen( VSD_WORK * wk );
static u8 VSD_SeqResultPut( VSD_WORK * wk );
static u8 VSD_SeqDrawPut( VSD_WORK * wk );
static u8 VSD_SeqPlatePalChg( VSD_WORK * wk );
static u8 VSD_SeqVsStartInit( VSD_WORK * wk );
static u8 VSD_SeqVsEndInit( VSD_WORK * wk );
static u8 VSD_SeqPokeCheckInit( VSD_WORK * wk );
static u8 VSD_PokeCheckMove( VSD_WORK * wk );
static BOOL VSD_GetStandID(void);

static void VSD_ClactResManInit( VSD_WORK * wk, TCATS_RESOURCE_NUM_LIST * crnl, int max );
static CATS_ACT_PTR VSD_ClactAdd( VSD_WORK * wk, const u32 * id );
static void VSD_ObjFree( VSD_WORK * wk );
static void VSD_ObjAnm( VSD_WORK * wk );

static void VSD_VsVBlank( void * work );
static void VSD_3DBgInit( VSD_WORK * wk, GXBG0As bg_2d3d, int end_flag );
static void VSD_3DBgExit( VSD_WORK * wk );
static void VSD_VsBgGraphicSet( VSD_WORK * wk );
static void VSD_VsAlphaSet(void);
static void VSD_VsObjMake( VSD_WORK * wk );
static void VSD_VsObjGlobalPosSet( VSD_WORK * wk, int offset_left_y, int offset_right_y );
static void VSD_VsBallObjMake( VSD_WORK * wk );
static void VSD_VsBallObjMove( VSD_WORK * wk, u8 flg, s16 x, s16 y );
static void VSD_VsBallAnmChg( VSD_WORK * wk );
static void VSD_VsFontOamMake( VSD_WORK * wk );
static void VSD_VsFontOamMove( VSD_WORK * wk, u32 id, s16 x, s16 y );
static void VSD_VsFontOamDelete( VSD_WORK * wk );
static void VSD_VsParticleInit( VSD_WORK * wk );
static int VSD_VsParticleMain( VSD_WORK * wk );
static void VSD_VsParticleDelete( VSD_WORK * wk );
static void VSD_VsEndObjMake( VSD_WORK * wk );
static void VSD_VsResultObjMake( VSD_WORK * wk );
static void VSD_VsResultMove( VSD_WORK * wk );

static void VSD_PokeCheckBgInit( VSD_WORK * wk );
static void VSD_PokeCheckBgExit( VSD_WORK * wk );
static void VSD_PokeCheckBgGraphicSet( VSD_WORK * wk );
static void VSD_PokeCheckObjMake( VSD_WORK * wk );
static void VSD_PokeParamGet( VSD_WORK * wk );
static void VSD_PokeCheckBgPut( VSD_WORK * wk );
static void VSD_PokeCheckBgCopy( VSD_WORK * wk );
static void VSD_PokeCheckBgMove( VSD_WORK * wk, u8 cnt );
static void VSD_PokeCheckBmpAdd( VSD_WORK * wk );
static void VSD_PokeCheckBmpExit( VSD_WORK * wk );
static void VSD_PokeCheckBmpPut( VSD_WORK * wk );
static void VSD_PokeCheckPokeIconMake( VSD_WORK * wk );
static void VSD_PokeCheckItemIconMake( VSD_WORK * wk );
static void VSD_PokeCheckStIconMake( VSD_WORK * wk );
static void VSD_PokeCheckBallMake( VSD_WORK * wk );
static void VSD_PokeCheckObjMove( VSD_WORK * wk, s16 mv );

static u32 sAllocTex( u32 size, BOOL is4x4comp );
static u32 sAllocTexPalette( u32 size, BOOL is4pltt );

static void VSD_FnoteMake( VS_DEMO_DATA * dat );

static u8 VSD_SeqRecSaveSelect( VSD_WORK * wk );
static u8 VSD_SeqTimingWait( VSD_WORK * wk );
static void VSD_VsMessageWindowSet( VSD_WORK * wk );
static void VSD_VsBmpExit( VSD_WORK * wk );
static void VSD_VsMsgmanExit( VSD_WORK * wk );
static void VSD_TalkWindowPut(VSD_WORK *wk);
static void VSD_RecSelectCheck(VSD_WORK *wk);
static BOOL VSD_RecModeCheck(VSD_WORK *wk);
static void VSD_TimeIconAdd(VSD_WORK *wk);
static void VSD_TimeIconDelete(VSD_WORK *wk);


//============================================================================================
//	�O���[�o���ϐ�
//============================================================================================
// VS��ʁA���s��ʂ̃Z���A�N�^�[�p�����[�^
static const u32 VS_ClactParam[][7] =
{	// chr_id, pal_id, cell_id, anm_id, bg_pri, obj_pri, pal_num
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },

	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },
	{ VS_CHR_ID_BALL, VS_PAL_ID_BALL, VS_CEL_ID_BALL, VS_ANM_ID_BALL, 2, 0, 0 },

	{ VS_CHR_ID_RESULT, VS_PAL_ID_BALL, VS_CEL_ID_RESULT, VS_ANM_ID_RESULT, 1, 0, 1 },
	{ VS_CHR_ID_RESULT, VS_PAL_ID_BALL, VS_CEL_ID_RESULT, VS_ANM_ID_RESULT, 1, 0, 1 },
};

// �����e�[�u���F�|�P�����m�F���
static const VSD_FUNC PokeCheckFuncTable[] = {
	VSD_SeqPokeCheckInit,
	VSD_SeqWait,			// �E�F�C�g
	VSD_PokeCheckMove,
	VSD_SeqWait,			// �E�F�C�g
	VSD_SeqEndSet,
};

// �����e�[�u���FVS���
static const VSD_FUNC VsStartFuncTable[] = {
	VSD_SeqVsStartInit,
	VSD_SeqWait,			// �E�F�C�g
	VSD_SeqScrInMain,		// �X�N���[���C�����C��
	VSD_SeqShake,			// �h�炷
	VSD_SeqScrOut,			// �X�N���[���A�E�g
};

// �����e�[�u���F���s����
static const VSD_FUNC VsEndWinFuncTable[] = {
	VSD_SeqVsEndInit,
	VSD_SeqWait,		// �E�F�C�g
	VSD_SeqScrInMain,	// �X�N���[���C�����C��
	VSD_SeqShake,		// �h�炷
	VSD_SeqShakeWaitSet,
	VSD_SeqWait,
	VSD_SeqPushCheck,
	VSD_SeqPush,
	VSD_SeqPuchWaitSet,
	VSD_SeqResultOpen,
	VSD_SeqWait,
	VSD_SeqResultPut,
	VSD_SeqWait,
	VSD_SeqPlatePalChg,
	VSD_SeqWait,
	VSD_SeqRecSaveSelect,
	VSD_SeqWait,
	VSD_SeqTimingWait,
	VSD_SeqEndSet,
};

// �����e�[�u���F��������
static const VSD_FUNC VsEndDrawFuncTable[] = {
	VSD_SeqVsEndInit,
	VSD_SeqWait,		// �E�F�C�g
	VSD_SeqScrInMain,	// �X�N���[���C�����C��
	VSD_SeqShake,		// �h�炷
	VSD_SeqPuchWaitSet,
	VSD_SeqResultOpen,
	VSD_SeqWait,
	VSD_SeqDrawPut,
	VSD_SeqWait,
	VSD_SeqRecSaveSelect,
	VSD_SeqWait,
	VSD_SeqTimingWait,
	VSD_SeqEndSet,
};

// ���s��ʁF�v���[�g�����o���e�[�u��
static const u8 VSD_PushMoveTbl[][2] =
{
	// BG, OBJ
//	{ 8, 8 }, { 0, 8 }, { 0, 8 }, { 4, 12 }, { 0, 12 }, { 0, 12 },
	{ 6, 6 }, { 0, 6 }, { 0, 6 }, { 4, 10 }, { 0, 10 }, { 0, 10 },
};

// �m�F��ʁFBMP�E�B���h�E��`
static const BMPWIN_DAT PokePanelBmpData[] =
{
	// �P�̖ڂ̃|�P����
	{	// ���O
		GF_BGL_FRAME1_M, WIN_NAME1_PX, WIN_NAME1_PY,
		WIN_NAME1_SX, WIN_NAME1_SY, WIN_NAME1_PAL, WIN_NAME1_CGX
	},
	{	// Lv
		GF_BGL_FRAME1_M, WIN_LV1_PX, WIN_LV1_PY,
		WIN_LV1_SX, WIN_LV1_SY, WIN_LV1_PAL, WIN_LV1_CGX
	},
	{	// HP/MHP
		GF_BGL_FRAME1_M, WIN_HP1_PX, WIN_HP1_PY,
		WIN_HP1_SX, WIN_HP1_SY, WIN_HP1_PAL, WIN_HP1_CGX
	},
	{	// HP�o�[
		GF_BGL_FRAME1_M, WIN_HPBAR1_PX, WIN_HPBAR1_PY,
		WIN_HPBAR1_SX, WIN_HPBAR1_SY, WIN_HPBAR1_PAL, WIN_HPBAR1_CGX
	},

	// �Q�̖ڂ̃|�P����
	{	// ���O
		GF_BGL_FRAME1_M, WIN_NAME2_PX, WIN_NAME2_PY,
		WIN_NAME2_SX, WIN_NAME2_SY, WIN_NAME2_PAL, WIN_NAME2_CGX
	},
	{	// Lv
		GF_BGL_FRAME1_M, WIN_LV2_PX, WIN_LV2_PY,
		WIN_LV2_SX, WIN_LV2_SY, WIN_LV2_PAL, WIN_LV2_CGX
	},
	{	// HP/MHP
		GF_BGL_FRAME1_M, WIN_HP2_PX, WIN_HP2_PY,
		WIN_HP2_SX, WIN_HP2_SY, WIN_HP2_PAL, WIN_HP2_CGX
	},
	{	// HP�o�[
		GF_BGL_FRAME1_M, WIN_HPBAR2_PX, WIN_HPBAR2_PY,
		WIN_HPBAR2_SX, WIN_HPBAR2_SY, WIN_HPBAR2_PAL, WIN_HPBAR2_CGX
	},

	// �R�̖ڂ̃|�P����
	{	// ���O
		GF_BGL_FRAME1_M, WIN_NAME3_PX, WIN_NAME3_PY,
		WIN_NAME3_SX, WIN_NAME3_SY, WIN_NAME3_PAL, WIN_NAME3_CGX
	},
	{	// Lv
		GF_BGL_FRAME1_M, WIN_LV3_PX, WIN_LV3_PY,
		WIN_LV3_SX, WIN_LV3_SY, WIN_LV3_PAL, WIN_LV3_CGX
	},
	{	// HP/MHP
		GF_BGL_FRAME1_M, WIN_HP3_PX, WIN_HP3_PY,
		WIN_HP3_SX, WIN_HP3_SY, WIN_HP3_PAL, WIN_HP3_CGX
	},
	{	// HP�o�[
		GF_BGL_FRAME1_M, WIN_HPBAR3_PX, WIN_HPBAR3_PY,
		WIN_HPBAR3_SX, WIN_HPBAR3_SY, WIN_HPBAR3_PAL, WIN_HPBAR3_CGX
	},

	// �S�̖ڂ̃|�P����
	{	// ���O
		GF_BGL_FRAME1_M, WIN_NAME4_PX, WIN_NAME4_PY,
		WIN_NAME4_SX, WIN_NAME4_SY, WIN_NAME4_PAL, WIN_NAME4_CGX
	},
	{	// Lv
		GF_BGL_FRAME1_M, WIN_LV4_PX, WIN_LV4_PY,
		WIN_LV4_SX, WIN_LV4_SY, WIN_LV4_PAL, WIN_LV4_CGX
	},
	{	// HP/MHP
		GF_BGL_FRAME1_M, WIN_HP4_PX, WIN_HP4_PY,
		WIN_HP4_SX, WIN_HP4_SY, WIN_HP4_PAL, WIN_HP4_CGX
	},
	{	// HP�o�[
		GF_BGL_FRAME1_M, WIN_HPBAR4_PX, WIN_HPBAR4_PY,
		WIN_HPBAR4_SX, WIN_HPBAR4_SY, WIN_HPBAR4_PAL, WIN_HPBAR4_CGX
	},

	// �T�̖ڂ̃|�P����
	{	// ���O
		GF_BGL_FRAME1_M, WIN_NAME5_PX, WIN_NAME5_PY,
		WIN_NAME5_SX, WIN_NAME5_SY, WIN_NAME5_PAL, WIN_NAME5_CGX
	},
	{	// Lv
		GF_BGL_FRAME1_M, WIN_LV5_PX, WIN_LV5_PY,
		WIN_LV5_SX, WIN_LV5_SY, WIN_LV5_PAL, WIN_LV5_CGX
	},
	{	// HP/MHP
		GF_BGL_FRAME1_M, WIN_HP5_PX, WIN_HP5_PY,
		WIN_HP5_SX, WIN_HP5_SY, WIN_HP5_PAL, WIN_HP5_CGX
	},
	{	// HP�o�[
		GF_BGL_FRAME1_M, WIN_HPBAR5_PX, WIN_HPBAR5_PY,
		WIN_HPBAR5_SX, WIN_HPBAR5_SY, WIN_HPBAR5_PAL, WIN_HPBAR5_CGX
	},

	// �U�̖ڂ̃|�P����
	{	// ���O
		GF_BGL_FRAME1_M, WIN_NAME6_PX, WIN_NAME6_PY,
		WIN_NAME6_SX, WIN_NAME6_SY, WIN_NAME6_PAL, WIN_NAME6_CGX
	},
	{	// Lv
		GF_BGL_FRAME1_M, WIN_LV6_PX, WIN_LV6_PY,
		WIN_LV6_SX, WIN_LV6_SY, WIN_LV6_PAL, WIN_LV6_CGX
	},
	{	// HP/MHP
		GF_BGL_FRAME1_M, WIN_HP6_PX, WIN_HP6_PY,
		WIN_HP6_SX, WIN_HP6_SY, WIN_HP6_PAL, WIN_HP6_CGX
	},
	{	// HP�o�[
		GF_BGL_FRAME1_M, WIN_HPBAR6_PX, WIN_HPBAR6_PY,
		WIN_HPBAR6_SX, WIN_HPBAR6_SY, WIN_HPBAR6_PAL, WIN_HPBAR6_CGX
	},
};

// �m�F��ʁF�v���[�gBG�\���ʒu
static const u8 PokeCheckBgPos[][2] =
{	// LTX, LTY
	{ 0, 0 }, { 0, 6 }, { 0, 12 }, { 16, 1 }, { 16, 7 }, { 16, 13 }
};

// �m�F��ʁFOBJ�\�����W�e�[�u��
static const u8 PokeCheckObjPosTable[][10] =
{	// �|�P�����A�A�C�e���A�J�X�^���{�[���A��Ԉُ�A�{�[��
	{  30,  16,  38,  24,  46,  24,  36,  44,  16,  14 },	// �P�C��
	{  30,  64,  38,  72,  46,  72,  36,  92,  16,  62 },	// �Q�C��
	{  30, 112,  38, 120,  46, 120,  36, 140,  16, 110 },	// �R�C��
	{ 158,  24, 166,  32, 174,  32, 164,  52, 144,  22 },	// �S�C��
	{ 158,  72, 166,  80, 174,  80, 164, 100, 144,  70 },	// �T�C��
	{ 158, 120, 166, 128, 174, 128, 164, 148, 144, 118 }	// �U�C��
};

// �m�F��ʂ̃Z���A�N�^�[�p�����[�^
static const u32 PokeCheck_ClactParam[][7] =
{	// chr_id, pal_id, cell_id, anm_id, bg_pri, obj_pri, pal_num
	{ VS_CHR_ID_POKE1, VS_PAL_ID_POKE, VS_CEL_ID_POKE, VS_ANM_ID_POKE, 1, 1, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ST, VS_PAL_ID_ST, VS_CEL_ID_ST, VS_ANM_ID_ST, 1, 0, 0 },
	{ VS_CHR_ID_PLBALL, VS_PAL_ID_PLBALL, VS_CEL_ID_PLBALL, VS_ANM_ID_PLBALL, 1, 2, 0 },

	{ VS_CHR_ID_POKE2, VS_PAL_ID_POKE, VS_CEL_ID_POKE, VS_ANM_ID_POKE, 1, 1, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ST, VS_PAL_ID_ST, VS_CEL_ID_ST, VS_ANM_ID_ST, 1, 0, 0 },
	{ VS_CHR_ID_PLBALL, VS_PAL_ID_PLBALL, VS_CEL_ID_PLBALL, VS_ANM_ID_PLBALL, 1, 2, 0 },

	{ VS_CHR_ID_POKE3, VS_PAL_ID_POKE, VS_CEL_ID_POKE, VS_ANM_ID_POKE, 1, 1, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ST, VS_PAL_ID_ST, VS_CEL_ID_ST, VS_ANM_ID_ST, 1, 0, 0 },
	{ VS_CHR_ID_PLBALL, VS_PAL_ID_PLBALL, VS_CEL_ID_PLBALL, VS_ANM_ID_PLBALL, 1, 2, 0 },

	{ VS_CHR_ID_POKE4, VS_PAL_ID_POKE, VS_CEL_ID_POKE, VS_ANM_ID_POKE, 1, 1, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ST, VS_PAL_ID_ST, VS_CEL_ID_ST, VS_ANM_ID_ST, 1, 0, 0 },
	{ VS_CHR_ID_PLBALL, VS_PAL_ID_PLBALL, VS_CEL_ID_PLBALL, VS_ANM_ID_PLBALL, 1, 2, 0 },

	{ VS_CHR_ID_POKE5, VS_PAL_ID_POKE, VS_CEL_ID_POKE, VS_ANM_ID_POKE, 1, 1, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ST, VS_PAL_ID_ST, VS_CEL_ID_ST, VS_ANM_ID_ST, 1, 0, 0 },
	{ VS_CHR_ID_PLBALL, VS_PAL_ID_PLBALL, VS_CEL_ID_PLBALL, VS_ANM_ID_PLBALL, 1, 2, 0 },

	{ VS_CHR_ID_POKE6, VS_PAL_ID_POKE, VS_CEL_ID_POKE, VS_ANM_ID_POKE, 1, 1, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ITEM, VS_PAL_ID_ITEM, VS_CEL_ID_ITEM, VS_ANM_ID_ITEM, 1, 0, 0 },
	{ VS_CHR_ID_ST, VS_PAL_ID_ST, VS_CEL_ID_ST, VS_ANM_ID_ST, 1, 0, 0 },
	{ VS_CHR_ID_PLBALL, VS_PAL_ID_PLBALL, VS_CEL_ID_PLBALL, VS_ANM_ID_PLBALL, 1, 2, 0 },
};


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
//	���W
//--------------------------------------------------------------
///��b�E�B���h�E�ʒu�F���[
#define VS_DEMO_BMPWIN_TALK_POS_X			(0x2)
///��b�E�B���h�E�ʒu�F��[
#define VS_DEMO_BMPWIN_TALK_POS_Y			(0x13)
///��b�E�B���h�E�̉���
#define VS_DEMO_BMPWIN_TALK_SIZE_X			(27)
///��b�E�B���h�E�̏c��
#define VS_DEMO_BMPWIN_TALK_SIZE_Y			(4)

///�E�B���h�E�̉��Ɂu�͂�/�������v�E�B���h�E���o���ꍇ�̉�b�E�B���h�E�̉���
#define VS_DEMO_BMPWIN_TALK_YESNO_SET_SIZE_X			(27)	//(27-8)
///�E�B���h�E�̉��Ɂu�͂�/�������v�E�B���h�E���o���ꍇ�̉�b�E�B���h�E�̏c��
#define VS_DEMO_BMPWIN_TALK_YESNO_SET_SIZE_Y			(4)

///�u�͂�/�������v�E�B���h�E�ʒu�F���[
#define VS_DEMO_BMPWIN_YESNO_POS_X			(25)
///�u�͂�/�������v�E�B���h�E�ʒu�F��[
#define VS_DEMO_BMPWIN_YESNO_POS_Y			(13)
///�u�͂�/�������v�E�B���h�E�̉���
#define VS_DEMO_BMPWIN_YESNO_SIZE_X			(6)
///�u�͂�/�������v�E�B���h�E�̏c��
#define VS_DEMO_BMPWIN_YESNO_SIZE_Y			(4)

//--------------------------------------------------------------
//	�L�����N�^�W�J�ʒu��p���b�g�ʒu
//--------------------------------------------------------------
///��b�E�B���h�E�̃L�����N�^�W�J�ʒu
#define VS_DEMO_TALKWIN_CGX_OFFSET	(1)
///��b�E�B���h�E�̃p���b�g�ԍ�
#define VS_DEMO_TALKWIN_PALNO		(15)

///�V�X�e���E�B���h�E�̃L�����N�^�W�J�ʒu
#define VS_DEMO_SYSWIN_CGX_OFFSET	(VS_DEMO_TALKWIN_CGX_OFFSET + TALK_WIN_CGX_SIZ)
#define VS_DEMO_SYSWIN_PALNO		(14)

///�V�X�e���t�H���g�̃p���b�g�ԍ�
#define VS_DEMO_SYSFONT_PALNO		(13)

///��b�E�B���h�E���̃t�H���g�L�����N�^�J�n�ʒu
#define VS_DEMO_TALK_STR_CGX_START	(VS_DEMO_SYSWIN_CGX_OFFSET + MENU_WIN_CGX_SIZ)
///�u�͂�/�������v�E�B���h�E���̃t�H���g�L�����N�^�J�n�ʒu
#define VS_DEMO_YESNO_STR_CGX_START	(VS_DEMO_TALK_STR_CGX_START + (VS_DEMO_BMPWIN_TALK_SIZE_X * VS_DEMO_BMPWIN_TALK_SIZE_Y))


//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///��b�E�B���h�E���̕�����o�b�t�@�̃T�C�Y
#define VS_DEMO_TALK_MESSAGE_BUF_SIZE	(2*160)		///<���߂ɂP�U�O������

///�Z�[�u�I����̑҂�����(�Z�[�u�������b�Z�[�W�������鎞��)
#define VSD_SAVE_AFTER_WAIT				(30)

//==============================================================================
//	�f�[�^
//==============================================================================
//�u�͂�/�������v�I���E�B���h�E�̐ݒ�f�[�^
static const BMPWIN_DAT VsDemoYesNoWinData =
{
	GF_BGL_FRAME0_M,				//�E�C���h�E�g�p�t���[��
	25, 13,							//�E�C���h�E�̈�̍���XY���W
//	25, 19,							//�E�C���h�E�̈�̍���XY���W
	6, 4,							//�E�C���h�E�̈��XY�T�C�Y
	VS_DEMO_SYSFONT_PALNO,			//�E�C���h�E�̈�̃p���b�g�i���o�[ 
	VS_DEMO_YESNO_STR_CGX_START,	//�E�C���h�E�L�����̈�̊J�n�L�����N�^�i���o�[
};

//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///�퓬�^��̊Ď����[�h
enum{
	REC_WIN_MODE_STOP,			///<�������Ȃ�
	REC_WIN_MODE_FIRST_MSG,		///<�ŏ��̃��b�Z�[�W�\��
	REC_WIN_MODE_FIRST_KEY,		///<�ŏ��̃��b�Z�[�W�\����ԂŃL�[�҂�
	REC_WIN_MODE_SELECT_MSG,	///<�I�����b�Z�[�W�\��
	REC_WIN_MODE_SELECT_KEY,	///<�I�����b�Z�[�W�\����ԂŃL�[�҂�
	REC_WIN_MODE_DECIDE_MSG,	///<���胁�b�Z�[�W�\��
	REC_WIN_MODE_DECIDE_END,	///<�Z�[�u����őҋ@���
	REC_WIN_MODE_CLEAR,			///<���b�Z�[�W���N���A���đ҂���Ԃɂ���
	REC_WIN_MODE_CLEAR_END,		///<�҂����
};


//--------------------------------------------------------------------------------------------
/**
 * �^�X�N�ǉ�
 *
 * @param	dat		�o�b�O�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void VSDemo_TaskAdd( VS_DEMO_DATA * dat )
{
	VSD_WORK * wk;

	wk = TCB_GetWork( PMDS_taskAdd(VSDemo_Main,sizeof(VSD_WORK),100,dat->heap) );

	memset( wk, 0, sizeof(VSD_WORK) );
	wk->dat = dat;
	wk->seq = SEQ_VSD_INIT;
	wk->rec_occ = BattleRec_ServerVersionCheck();
	if(wk->rec_occ == TRUE){
		if(wk->dat->bp != NULL && wk->dat->bp->savedata != NULL){
			wk->rec_occ = MyItem_CheckItem(SaveData_GetMyItem(wk->dat->bp->savedata), 
				ITEM_BATORUREKOODAA, 1, dat->heap);
		}
	}
}

//--------------------------------------------------------------------------------------------
/**
 * ���C���^�X�N
 *
 * @param	tcb
 * @param	work
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSDemo_Main( TCB_PTR tcb, void * work )
{
	VSD_WORK * wk = work;

	switch( wk->seq ){
	case SEQ_VSD_INIT:		// ������
		wk->seq = VSD_SeqInit( wk );
		break;

	case SEQ_VSD_MAIN:
		while(1){
			u8	ret = wk->func[wk->func_seq]( wk );
			if( ret != VSD_FUNC_LOOP ){
				wk->func_seq++;
			}
			if( ret != VSD_FUNC_REPEAT ){
				break;
			}
		}

		//�ΐ�^��Ď�����
		if(wk->dat->mode == VSD_MODE_END){
			VSD_RecSelectCheck(wk);
		}
		
		break;

	case SEQ_VSD_END:		// �I��
		if( VSD_SeqEnd( tcb, wk ) == TRUE ){
			return;
		}
	}

	if( wk->func_seq != 0 ){	// ���������ȊO
		VSD_ObjAnm( wk );
		CATS_Draw( wk->crp );
	}
	if( wk->dat->mode != VSD_MODE_MULTI ){
		G3_SwapBuffers( GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * VRAM�ݒ�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VramSet(void)
{
	GF_BGL_DISPVRAM vramSetTable = {
		GX_VRAM_BG_128_A,				// ���C��2D�G���W����BG
		GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
		GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
		GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
		GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
		GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
		GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
		GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
		GX_VRAM_TEX_01_BC,				// �e�N�X�`���C���[�W�X���b�g
		GX_VRAM_TEXPLTT_01_FG			// �e�N�X�`���p���b�g�X���b�g
	};
	GF_Disp_SetBank( &vramSetTable );

	//VRAM�N���A
	MI_CpuClear32((void*)HW_BG_VRAM, HW_BG_VRAM_SIZE);
}

//--------------------------------------------------------------
/**
 * @brief   �퓬�^��� ����E���Ȃ� ���Ď�
 *
 * @param   wk		���[�N
 */
//--------------------------------------------------------------
static void VSD_RecSelectCheck(VSD_WORK *wk)
{
	if(VSD_RecModeCheck(wk) == FALSE){
		return;
	}
	
	switch(wk->rec_win_mode){
	case REC_WIN_MODE_STOP:
		break;
	case REC_WIN_MODE_FIRST_MSG:	//�u�ΐ�^��Z�[�u���܂����v���b�Z�[�W�\��
		VSD_TalkWindowPut(wk);
		//������̎擾
		MSGMAN_GetString(wk->rec_msgman, msg_taisen_rokuga01_01, wk->rec_talkmsg_buf);
		//������̕\��
		wk->talk_msgid = GF_STR_PrintSimple(&wk->talk_win, FONT_TALK, wk->rec_talkmsg_buf, 0,0, 
			MSG_ALLPUT, NULL);

		GF_BGL_LoadScreenV_Req(wk->bgl, GF_BGL_FRAME0_M);
		wk->rec_win_mode = REC_WIN_MODE_FIRST_KEY;
		break;
	case REC_WIN_MODE_FIRST_KEY:
		if(wk->rec_occ == TRUE){
			if(sys.trg & PAD_BUTTON_A){
				wk->rec_win_mode = REC_WIN_MODE_SELECT_MSG;
			}
		}
		break;
	case REC_WIN_MODE_SELECT_MSG:	//�u�����قǂ́@�����������@���낭���܂����H�v���b�Z�[�W�\��
		//������̎擾
		if(wk->load_result == RECLOAD_RESULT_OK){
			MSGMAN_GetString(wk->rec_msgman, msg_taisen_rokuga01_09, wk->rec_talkmsg_buf);
		}
		else{
			MSGMAN_GetString(wk->rec_msgman, msg_taisen_rokuga01_02, wk->rec_talkmsg_buf);
		}
		//�̈�N���A���E�B���h�E�`��
		GF_BGL_BmpWinFill( &wk->sub_talk_win, FBMP_COL_WHITE, 0, 0,
						VS_DEMO_BMPWIN_TALK_YESNO_SET_SIZE_X * 8, 
						VS_DEMO_BMPWIN_TALK_YESNO_SET_SIZE_Y * 8 );
		BmpTalkWinWrite( &wk->sub_talk_win, WINDOW_TRANS_ON,
						VS_DEMO_TALKWIN_CGX_OFFSET, VS_DEMO_TALKWIN_PALNO );
		//������̕\��
		wk->talk_msgid = GF_STR_PrintSimple(&wk->sub_talk_win, FONT_TALK, 
			wk->rec_talkmsg_buf, 0,0, MSG_ALLPUT, NULL);

		//�͂��E�������E�B���h�E�\��
		wk->yesno_menu = BmpYesNoSelectInitEx(wk->bgl,&VsDemoYesNoWinData,
			VS_DEMO_SYSWIN_CGX_OFFSET, VS_DEMO_SYSWIN_PALNO, 1, wk->dat->heap);

		GF_BGL_LoadScreenV_Req(wk->bgl, GF_BGL_FRAME0_M);
		wk->rec_win_mode = REC_WIN_MODE_SELECT_KEY;
		break;
	case REC_WIN_MODE_SELECT_KEY:	//�u�͂�/�������v�I��҂�
		{
			u32 list_result = BmpYesNoSelectMain(wk->yesno_menu, wk->dat->heap);

			switch(list_result){
			case 0:		//�u�͂��v
				wk->rec_win_mode = REC_WIN_MODE_DECIDE_MSG;
				wk->yesno_menu = NULL;
				break;
			case BMPMENU_CANCEL:	//�u�������v
				wk->rec_win_mode = REC_WIN_MODE_FIRST_MSG;
				wk->yesno_menu = NULL;
				break;
			}
		}
		break;
	case REC_WIN_MODE_DECIDE_MSG:	//�u���΂炭�@���܂����������v���b�Z�[�W�\��
		VSD_TalkWindowPut(wk);
		//������̎擾
		MSGMAN_GetString(wk->rec_msgman, msg_taisen_rokuga01_05, wk->rec_talkmsg_buf);
		//������̕\��
		wk->talk_msgid = GF_STR_PrintSimple(&wk->talk_win, FONT_TALK, wk->rec_talkmsg_buf, 0,0, 
			MSG_ALLPUT, NULL);

		GF_BGL_LoadScreenV_Req(wk->bgl, GF_BGL_FRAME0_M);
		wk->rec_win_mode = REC_WIN_MODE_DECIDE_END;
		break;
	case REC_WIN_MODE_DECIDE_END:
		break;
	
	case REC_WIN_MODE_CLEAR:
		if(wk->yesno_menu != NULL){
			BmpYesNoWinDel(wk->yesno_menu, wk->dat->heap);
			wk->yesno_menu = NULL;
		}
		BmpTalkWinClear(&wk->talk_win, WINDOW_TRANS_ON);

		GF_BGL_LoadScreenV_Req(wk->bgl, GF_BGL_FRAME0_M);
		wk->rec_win_mode = REC_WIN_MODE_CLEAR_END;
		break;
	case REC_WIN_MODE_CLEAR_END:
		break;
	}
}

//--------------------------------------------------------------------------------------------
/**
 * �������V�[�P���X
 *
 * @param	wk		���[�N
 *
 * @return	�ڍs����V�[�P���X
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqInit( VSD_WORK * wk )
{
	G2_BlendNone();

	sys_VBlankFuncChange( NULL, NULL );	// VBlank�Z�b�g
	sys_HBlankIntrStop();				// HBlank��~

	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	GX_SetVisiblePlane( 0 );
	GXS_SetVisiblePlane( 0 );

	G2_BlendNone();
	G2S_BlendNone();

	VSD_VramSet();

	wk->bgl = GF_BGL_BglIniAlloc( wk->dat->heap );
	wk->pfd = PaletteFadeInit( wk->dat->heap );
//	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_BG, FADE_PAL_ONE_SIZE*3, wk->dat->heap );
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_BG, FADE_PAL_ONE_SIZE*16, wk->dat->heap );
	GF_BGL_BackGroundColorSet(GF_BGL_FRAME0_S, 0x0000);
	
	wk->cnt  = 0;
	wk->wait = INIT_WAIT;

	wk->func_seq = 0;

	switch( wk->dat->mode ){
	case VSD_MODE_MULTI:
		wk->func = PokeCheckFuncTable;
		break;
	case VSD_MODE_IN:
		wk->func = VsStartFuncTable;
		break;
	case VSD_MODE_END:
		if( wk->dat->result == VSD_RESULT_DRAW ){
			wk->func = VsEndDrawFuncTable;
		}else{
			wk->func = VsEndWinFuncTable;
		}
		VSD_FnoteMake( wk->dat );
		break;
	}

	return SEQ_VSD_MAIN;
}


//--------------------------------------------------------------------------------------------
/**
 * ���C�������F�m�F��ʂ̏�����
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqPokeCheckInit( VSD_WORK * wk )
{
	switch( wk->cnt ){
	case 0:
		VSD_PokeCheckBgInit( wk );
		VSD_PokeCheckBgGraphicSet( wk );
		break;
	case 1:
		VSD_PokeParamGet( wk );
		VSD_PokeCheckObjMake( wk );
		break;
	case 2:
		wk->cnt = 0;
		wk->wait = POKECHECK_INIT_WAIT;
		VSD_PokeCheckBgPut( wk );
		VSD_PokeCheckBmpAdd( wk );
		VSD_PokeCheckBmpPut( wk );
		VSD_PokeCheckBgCopy( wk );
		WirelessIconEasy();
		APP_WipeStart( APP_WIPE_IN, wk->dat->heap );
		sys_VBlankFuncChange( VSD_VsVBlank, wk );	// VBlank�Z�b�g
		return VSD_FUNC_NEXT;
	}
	wk->cnt++;
	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������FVS��ʂ̏�����
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqVsStartInit( VSD_WORK * wk )
{
	switch( wk->cnt ){
	case 0:
		VSD_3DBgInit( wk, GX_BG0_AS_3D, FALSE );
		VSD_VsBgGraphicSet( wk );
		VSD_VsAlphaSet();
		break;
	case 1:
		VSD_VsObjGlobalPosSet( wk, 0, 0 );
		VSD_VsObjMake( wk );
		WirelessIconEasy();
		VSD_VsParticleInit( wk );
		break;
	case 2:
		APP_WipeStart( APP_WIPE_IN, wk->dat->heap );
		sys_VBlankFuncChange( VSD_VsVBlank, wk );	// VBlank�Z�b�g
		wk->cnt = 0;
		wk->shake_x = START_SHAKE_X;
		wk->shake_y = START_SHAKE_Y;
		wk->scr_spd = SCROLL_SPEED;
		return VSD_FUNC_NEXT;
	}
	wk->cnt++;
	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̏�����
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqVsEndInit( VSD_WORK * wk )
{
	switch( wk->cnt ){
	case 0:
		VSD_3DBgInit( wk, GX_BG0_AS_2D, TRUE );
		VSD_VsBgGraphicSet( wk );
		VSD_VsMessageWindowSet(wk);
		VSD_VsAlphaSet();
		{//���ɘ^��f�[�^�����݂��Ă��邩�ǂ���
			BOOL rec_load_ret;
			LOAD_RESULT load_result;
			
			rec_load_ret = BattleRec_DataOccCheck(
				wk->dat->bp->savedata, wk->dat->heap, &load_result, LOADDATA_MYREC);
			wk->load_result = load_result;
		}
		{//DP���������Ă��邩
			int i;
			MYSTATUS *myst;
			
			wk->dp_entry = 0;
			for(i = 0; i < COMM_MACHINE_MAX; i++){
				myst = CommInfoGetMyStatus(i);
				if(myst != NULL){
					OS_TPrintf("rom_code = %d\n", MyStatus_GetRomCode(myst));
					if(MyStatus_GetRomCode(myst) == POKEMON_DP_ROM_CODE){
						wk->dp_entry = TRUE;
						OS_TPrintf("dp_entry !\n");
						break;
					}
				}
			}
		}
		break;
	case 1:
		VSD_VsObjGlobalPosSet( wk, -END_DISP_OFFSET_LEFT_Y, -END_DISP_OFFSET_RIGHT_Y );
		VSD_VsEndObjMake( wk );
		WirelessIconEasy();
		break;
	case 2:
		APP_WipeStart( APP_WIPE_IN, wk->dat->heap );
		sys_VBlankFuncChange( VSD_VsVBlank, wk );	// VBlank�Z�b�g
		wk->cnt = 0;
		wk->shake_x = END_SHAKE_X;
		wk->shake_y = END_SHAKE_Y;
		wk->scr_spd = END_SCROLL_SPEED;

		//�ΐ�^��m�F���b�Z�[�W�p�̃��b�Z�[�W�}�l�[�W���쐬
		wk->rec_msgman = MSGMAN_Create(MSGMAN_TYPE_NORMAL, ARC_MSG, 
			NARC_msg_taisen_rokuga_dat, wk->dat->heap);
		wk->rec_wordset = WORDSET_Create(wk->dat->heap);
		wk->rec_talkmsg_buf = STRBUF_Create(VS_DEMO_TALK_MESSAGE_BUF_SIZE, wk->dat->heap);
		//�ΐ�^��Ď��֐��̓���J�n
		wk->rec_win_mode = REC_WIN_MODE_FIRST_MSG;

		return VSD_FUNC_NEXT;
	}

	wk->cnt++;
	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F�E�F�C�g
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqWait( VSD_WORK * wk )
{
	if( WIPE_SYS_EndCheck() == FALSE ){ return VSD_FUNC_LOOP; }

	if( wk->wait == wk->cnt ){
		wk->cnt = 0;
		return VSD_FUNC_NEXT;
	}
	wk->cnt++;
	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������FVS��ʁE�m�F��ʂ̃X�N���[���C������
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqScrInMain( VSD_WORK * wk )
{
	if( GF_BGL_ScrollGetX( wk->bgl, GF_BGL_FRAME3_M ) <= SCROLL_END ){
		wk->cnt = 0;
		wk->old_sx = 0;
		wk->old_sy = 0;

		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_X_SET, SCROLL_END );
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_X_SET, -SCROLL_END );

		VSD_VsBallObjMove( wk, 0, 0, 0 );
		VSD_VsBallObjMove( wk, 1, 0, 0 );
		VSD_VsBallObjMove( wk, 2, 0, 0 );
		VSD_VsBallObjMove( wk, 3, 0, 0 );

		VSD_VsFontOamMove( wk, 0, 0, 0 );
		VSD_VsFontOamMove( wk, 1, 0, 0 );
		VSD_VsFontOamMove( wk, 2, 0, 0 );
		VSD_VsFontOamMove( wk, 3, 0, 0 );

		return VSD_FUNC_NEXT;
	}

	GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_X_DEC, wk->scr_spd );
	GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_X_INC, wk->scr_spd );

	wk->cnt++;

	VSD_VsBallObjMove( wk, 0, wk->scr_spd * wk->cnt - SCROLL_INIT, 0 );
	VSD_VsBallObjMove( wk, 1, wk->scr_spd * wk->cnt - SCROLL_INIT, 0 );
	VSD_VsBallObjMove( wk, 2, SCROLL_INIT - wk->scr_spd * wk->cnt, 0 );
	VSD_VsBallObjMove( wk, 3, SCROLL_INIT - wk->scr_spd * wk->cnt, 0 );

	VSD_VsFontOamMove( wk, 0, wk->scr_spd * wk->cnt - SCROLL_INIT, 0 );
	VSD_VsFontOamMove( wk, 1, wk->scr_spd * wk->cnt - SCROLL_INIT, 0 );
	VSD_VsFontOamMove( wk, 2, SCROLL_INIT - wk->scr_spd * wk->cnt, 0 );
	VSD_VsFontOamMove( wk, 3, SCROLL_INIT - wk->scr_spd * wk->cnt, 0 );

	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������FVS��ʁE�m�F��ʂ̗h�炷����
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqShake( VSD_WORK * wk )
{
	s16	x, y;

	if( VSD_VsParticleMain( wk ) == FALSE && wk->cnt > SHAKE_CNT ){
		wk->cnt = 0;
		return VSD_FUNC_NEXT;
	}

	if( wk->cnt == 0 ){
		if( wk->dat->mode == VSD_MODE_IN ){
			Snd_SePlay( VSD_SE_VS_PLATE_BREAK );
		}else{
			Snd_SePlay( VSD_SE_RESULT_PLATE_BREAK );
		}
	}

	if( wk->cnt == SHAKE_CNT ){
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_X_SET, SCROLL_END );
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_X_SET, -SCROLL_END );
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_Y_SET, 0 );
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_Y_SET, 0 );

		VSD_VsBallObjMove( wk, 0, 0, 0 );
		VSD_VsBallObjMove( wk, 1, 0, 0 );
		VSD_VsBallObjMove( wk, 2, 0, 0 );
		VSD_VsBallObjMove( wk, 3, 0, 0 );

		VSD_VsFontOamMove( wk, 0, 0, 0 );
		VSD_VsFontOamMove( wk, 1, 0, 0 );
		VSD_VsFontOamMove( wk, 2, 0, 0 );
		VSD_VsFontOamMove( wk, 3, 0, 0 );

	}else if( wk->cnt < SHAKE_CNT ){

		x = ( gf_rand() % wk->shake_x ) - ( wk->shake_x / 2 );
		y = ( gf_rand() % wk->shake_y ) - ( wk->shake_y / 2 );

		if( ( x < 0 && wk->old_sx < 0 ) || ( x >0 && wk->old_sx > 0 ) ){
			x *= -1;
		}
		if( ( y < 0 && wk->old_sy < 0 ) || ( y >0 && wk->old_sy > 0 ) ){
			y *= -1;
		}

		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_X_SET, SCROLL_END+x );
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_X_SET, -SCROLL_END+x );
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_Y_SET, y );
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_Y_SET, y );

		VSD_VsBallObjMove( wk, 0, x, y );
		VSD_VsBallObjMove( wk, 1, x, y );
		VSD_VsBallObjMove( wk, 2, x, y );
		VSD_VsBallObjMove( wk, 3, x, y );

		VSD_VsFontOamMove( wk, 0, x, y );
		VSD_VsFontOamMove( wk, 1, x, y );
		VSD_VsFontOamMove( wk, 2, x, y );
		VSD_VsFontOamMove( wk, 3, x, y );

		wk->old_sx = x;
		wk->old_sy = y;
	}

	wk->cnt++;

	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������FVS��ʂ̃X�N���[���A�E�g����
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqScrOut( VSD_WORK * wk )
{
	if( wk->cnt == 8 ){
		WIPE_SYS_Start(
			WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT,
			WIPE_TYPE_FADEOUT, WIPE_FADE_WHITE, WIPE_DEF_DIV, WIPE_DEF_SYNC, wk->dat->heap );
	}

	if( wk->cnt >= 8 && WIPE_SYS_EndCheck() == TRUE ){
		wk->seq = SEQ_VSD_END;
		return VSD_FUNC_NEXT;
	}

	if( wk->cnt == 0 ){
		Snd_SePlay( VSD_SE_VS_PLATE_OUT );
	}

	GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_X_INC, SCROLL_SPEED );
	GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_X_DEC, SCROLL_SPEED );

	wk->cnt++;

	VSD_VsBallObjMove( wk, 0, -SCROLL_SPEED * wk->cnt, 0 );
	VSD_VsBallObjMove( wk, 1, -SCROLL_SPEED * wk->cnt, 0 );
	VSD_VsBallObjMove( wk, 2, SCROLL_SPEED * wk->cnt, 0 );
	VSD_VsBallObjMove( wk, 3, SCROLL_SPEED * wk->cnt, 0 );

	VSD_VsFontOamMove( wk, 0, -SCROLL_SPEED * wk->cnt, 0 );
	VSD_VsFontOamMove( wk, 1, -SCROLL_SPEED * wk->cnt, 0 );
	VSD_VsFontOamMove( wk, 2, SCROLL_SPEED * wk->cnt, 0 );
	VSD_VsFontOamMove( wk, 3, SCROLL_SPEED * wk->cnt, 0 );

	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̗h�炵����̃E�F�C�g�ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqShakeWaitSet( VSD_WORK * wk )
{
	wk->wait = SHAKE_WAIT;
	return VSD_FUNC_REPEAT;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̃v���[�g�����o�������ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqPushCheck( VSD_WORK * wk )
{
	if( VSD_GetStandID() == TRUE ){
		if( wk->dat->result == VSD_RESULT_WIN ){
			wk->push_mv = VSD_TYPE_LEFT;
		}else{
			wk->push_mv = VSD_TYPE_RIGHT;
		}
	}else{
		if( wk->dat->result == VSD_RESULT_WIN ){
			wk->push_mv = VSD_TYPE_RIGHT;
		}else{
			wk->push_mv = VSD_TYPE_LEFT;
		}
	}
	return VSD_FUNC_REPEAT;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̃v���[�g�����o�������i�����j
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqPush( VSD_WORK * wk )
{
	if( wk->push_mv == VSD_TYPE_LEFT ){
		return VSD_SeqLeftPush( wk );
	}
	return VSD_SeqRightPush( wk );
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̃v���[�g�����o�������i���������j
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqLeftPush( VSD_WORK * wk )
{
	GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_X_DEC, VSD_PushMoveTbl[wk->cnt][0] );
	GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_X_DEC, VSD_PushMoveTbl[wk->cnt][0] );
	VSD_VsBallObjMove( wk, 0, VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsBallObjMove( wk, 1, VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsBallObjMove( wk, 2, VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsBallObjMove( wk, 3, VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsFontOamMove( wk, 0, VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsFontOamMove( wk, 1, VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsFontOamMove( wk, 2, VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsFontOamMove( wk, 3, VSD_PushMoveTbl[wk->cnt][1], 0 );

	if( wk->cnt == 0 ){
		Snd_SePlay( VSD_SE_RESULT_PLATE_MOVE );
	}

	if( wk->cnt == 5 ){
		wk->cnt = 0;
		return VSD_FUNC_NEXT;
	}else{
		wk->cnt++;
	}

	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̃v���[�g�����o�������i�E�������j
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqRightPush( VSD_WORK * wk )
{
	GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_X_INC, VSD_PushMoveTbl[wk->cnt][0] );
	GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_X_INC, VSD_PushMoveTbl[wk->cnt][0] );
	VSD_VsBallObjMove( wk, 0, -VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsBallObjMove( wk, 1, -VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsBallObjMove( wk, 2, -VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsBallObjMove( wk, 3, -VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsFontOamMove( wk, 0, -VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsFontOamMove( wk, 1, -VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsFontOamMove( wk, 2, -VSD_PushMoveTbl[wk->cnt][1], 0 );
	VSD_VsFontOamMove( wk, 3, -VSD_PushMoveTbl[wk->cnt][1], 0 );

	if( wk->cnt == 0 ){
		Snd_SePlay( VSD_SE_RESULT_PLATE_MOVE );
	}

	if( wk->cnt == 5 ){
		wk->cnt = 0;
		return VSD_FUNC_NEXT;
	}else{
		wk->cnt++;
	}

	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̃v���[�g�����o����̃E�F�C�g�ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqPuchWaitSet( VSD_WORK * wk )
{
	wk->wait = PUSH_WAIT;
	return VSD_FUNC_REPEAT;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̏��s�E�B���h�E�I�[�v��
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqResultOpen( VSD_WORK * wk )
{
	if( wk->cnt == 0 ){
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );

		GX_SetVisibleWnd( GX_WNDMASK_W0 );
		G2_SetWnd0InsidePlane(
			GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|
			GX_WND_PLANEMASK_BG2|GX_WND_PLANEMASK_BG3|GX_WND_PLANEMASK_OBJ, TRUE );
		G2_SetWndOutsidePlane(
			GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG2|
			GX_WND_PLANEMASK_BG3|GX_WND_PLANEMASK_OBJ, FALSE );
	}

	if( wk->cnt == RESULT_OPEN_CNT ){
		wk->cnt = 0;
		wk->wait = RESULT_WAIT;
		GX_SetVisibleWnd( GX_WNDMASK_NONE );
		return VSD_FUNC_NEXT;
	}else{
		G2_SetWnd0Position( 0, 95-wk->cnt - END_DISP_OFFSET_Y, 255, 
			96+wk->cnt - END_DISP_OFFSET_Y );
		wk->cnt++;
	}

	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̏��s�\��
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqResultPut( VSD_WORK * wk )
{
	Snd_SePlay( VSD_SE_RESULT_PUT );

	CATS_ObjectEnableCap( wk->cap[VS_CLA_RESULT1], 1 );
	CATS_ObjectEnableCap( wk->cap[VS_CLA_RESULT2], 1 );
	CATS_ObjectAnimeSeqSetCap( wk->cap[VS_CLA_RESULT1], 0 );
	wk->wait = RESULT_WAIT;
	return VSD_FUNC_NEXT;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̈��������\��
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqDrawPut( VSD_WORK * wk )
{
	Snd_SePlay( VSD_SE_RESULT_PUT );

	CATS_ObjectEnableCap( wk->cap[VS_CLA_RESULT1], 1 );
	wk->wait = RESULT_END_WAIT;
	return VSD_FUNC_NEXT;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F�퓬�^��u�Z�[�u���܂����H�v
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqRecSaveSelect( VSD_WORK * wk )
{
	if(VSD_RecModeCheck(wk) == FALSE){
		//�Z�[�u�f�[�^�|�C���^�������Ă��Ȃ��Ȃ�΃X�L�b�v
		wk->wait = 1;
		return VSD_FUNC_NEXT;
	}

	if(wk->dp_entry == 0 && wk->yesno_menu != NULL){
		return VSD_FUNC_LOOP;
	}
	
	if(wk->rec_win_mode == REC_WIN_MODE_DECIDE_MSG){
		return VSD_FUNC_LOOP;
	}
	
	if(wk->rec_win_mode != REC_WIN_MODE_DECIDE_END 
			&& wk->rec_win_mode != REC_WIN_MODE_DECIDE_MSG
			&& wk->rec_win_mode != REC_WIN_MODE_CLEAR 
			&& wk->rec_win_mode != REC_WIN_MODE_CLEAR_END){
		wk->rec_win_mode = REC_WIN_MODE_CLEAR;
		wk->cnt = 0xff;
	}
	
	switch(wk->cnt){
	case 0:	//�Z�[�u���s�����b�Z�[�W�\��
		VSD_TalkWindowPut(wk);
		//������̎擾
		MSGMAN_GetString(wk->rec_msgman, msg_taisen_rokuga01_08, wk->rec_talkmsg_buf);
		//������̕\��
		wk->talk_msgid = GF_STR_PrintSimple(&wk->talk_win, FONT_TALK, wk->rec_talkmsg_buf, 0,0, 
			MSG_ALLPUT, NULL);
		VSD_TimeIconAdd(wk);
		
		wk->cnt++;
		break;
	case 1:	//�Z�[�u���s
		{
			SAVE_RESULT save_result;
			
			save_result = BattleRec_Save(wk->dat->bp->savedata, wk->dat->rec_mode, 0, 
				LOADDATA_MYREC, &wk->save_seq0, &wk->save_seq1);

			//�u�Z�[�u���܂����I�v or �u�Z�[�u���s�I�v
			if(save_result == SAVE_RESULT_OK){
				MSGMAN_GetString(wk->rec_msgman, msg_taisen_rokuga01_06, wk->rec_talkmsg_buf);
				Snd_SePlay(SEQ_SE_DP_SAVE);
			}
			else if(save_result == SAVE_RESULT_NG){
				MSGMAN_GetString(wk->rec_msgman, msg_taisen_rokuga01_07, wk->rec_talkmsg_buf);
			}
			else{
				break;
			}
			
			VSD_TimeIconDelete(wk);
			GF_BGL_BmpWinFill( &wk->talk_win, FBMP_COL_WHITE, 0, 0,
				VS_DEMO_BMPWIN_TALK_SIZE_X * 8, VS_DEMO_BMPWIN_TALK_SIZE_Y * 8 );
			//������̕\��
			wk->talk_msgid = GF_STR_PrintSimple(&wk->talk_win, FONT_TALK, 
				wk->rec_talkmsg_buf, 0,0, MSG_ALLPUT, NULL);
		}
		
		wk->wait = 0;
		wk->cnt++;
		break;
	case 2:
		wk->wait++;
		if(wk->wait > VSD_SAVE_AFTER_WAIT){
			wk->wait = 0;
			wk->cnt++;
		}
		break;
	default:
		BmpTalkWinClear(&wk->talk_win, WINDOW_TRANS_ON);
		GF_BGL_LoadScreenV_Req(wk->bgl, GF_BGL_FRAME0_M);
		
		wk->cnt = 0;
		wk->wait = RESULT_END_WAIT / 3;	//�Z�[�u���Ă������A�E�F�C�g�Z�߂ɂ���
		return VSD_FUNC_NEXT;
	}
	GF_BGL_LoadScreenV_Req(wk->bgl, GF_BGL_FRAME0_M);
	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʏI�����̓������
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqTimingWait( VSD_WORK * wk )
{
	switch(wk->cnt){
	case 0:
		VSD_TalkWindowPut(wk);
		//������̎擾
		MSGMAN_GetString(wk->rec_msgman, msg_taisen_rokuga01_10, wk->rec_talkmsg_buf);
		//������̕\��
		wk->talk_msgid = GF_STR_PrintSimple(&wk->talk_win, FONT_TALK, wk->rec_talkmsg_buf, 0,0, 
			MSG_ALLPUT, NULL);

		CommTimingSyncStart( COMM_BATTLE_WIN_LOSE_WAIT_NUM );

		if(wk->rec_occ == FALSE){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		}

		wk->cnt++;
		break;
	case 1:	//�Z�[�u���s
		if(CommIsTimingSync( COMM_BATTLE_WIN_LOSE_WAIT_NUM )){
			wk->cnt = 0;
			return VSD_FUNC_NEXT;
		}
		break;
	default:
		GF_ASSERT(0);
		break;
	}
	GF_BGL_LoadScreenV_Req(wk->bgl, GF_BGL_FRAME0_M);
	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F���s��ʂ̃p���b�g�ύX
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqPlatePalChg( VSD_WORK * wk )
{
	if( VSD_GetStandID() == TRUE ){
		if( wk->dat->result == VSD_RESULT_WIN ){
		//	PaletteFadeReq( wk->pfd, PF_BIT_MAIN_BG, RESULT_WIN_CHG_PAL, 0, 0, 4, 0 );
			PaletteWorkCopy(wk->pfd, FADE_MAIN_BG, RESULT_LOSE_LOAD_PAL * 16, 
				FADE_MAIN_BG, RESULT_LOSE_CHG_PAL * 16, 0x20);
		}else{
		//	PaletteFadeReq( wk->pfd, PF_BIT_MAIN_BG, RESULT_LOSE_CHG_PAL, 0, 0, 4, 0 );
			PaletteWorkCopy(wk->pfd, FADE_MAIN_BG, RESULT_LOSE_LOAD_PAL * 16, 
				FADE_MAIN_BG, RESULT_WIN_CHG_PAL * 16, 0x20);
		}
	}else{
		if( wk->dat->result == VSD_RESULT_WIN ){
		//	PaletteFadeReq( wk->pfd, PF_BIT_MAIN_BG, RESULT_LOSE_CHG_PAL, 0, 0, 4, 0 );
			PaletteWorkCopy(wk->pfd, FADE_MAIN_BG, RESULT_LOSE_LOAD_PAL * 16, 
				FADE_MAIN_BG, RESULT_WIN_CHG_PAL * 16, 0x20);
		}else{
		//	PaletteFadeReq( wk->pfd, PF_BIT_MAIN_BG, RESULT_WIN_CHG_PAL, 0, 0, 4, 0 );
			PaletteWorkCopy(wk->pfd, FADE_MAIN_BG, RESULT_LOSE_LOAD_PAL * 16, 
				FADE_MAIN_BG, RESULT_LOSE_CHG_PAL * 16, 0x20);
		}
	}
	PaletteTransSwitch(wk->pfd, TRUE);
	Snd_SePlay( VSD_SE_RESULT_FADE );
	wk->wait = RESULT_END_WAIT;
	return VSD_FUNC_NEXT;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F�m�F��ʂ̏����ړ�����
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_PokeCheckMove( VSD_WORK * wk )
{
	if( wk->cnt == 0 ){
		Snd_SePlay( VSD_SE_MULTI_POKE_IN );
	}

	if( wk->cnt == 16 ){
		wk->cnt  = 0;
		wk->wait = POKECHECK_END_WAIT;
		return VSD_FUNC_NEXT;
	}
	wk->cnt++;

	VSD_PokeCheckObjMove( wk, MULTI_SCROLL_INIT-wk->cnt );
	VSD_PokeCheckBgMove( wk, wk->cnt );

	return VSD_FUNC_LOOP;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F�I���Z�b�g
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqEndSet( VSD_WORK * wk )
{
	APP_WipeStart( APP_WIPE_OUT, wk->dat->heap );
	wk->seq = SEQ_VSD_END;
	return VSD_FUNC_NEXT;
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�������F�I������
 *
 * @param	wk		���[�N
 *
 * @return	�����R�}���h
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_SeqEnd( TCB_PTR tcb, VSD_WORK * wk )
{
	if( WIPE_SYS_EndCheck() == FALSE ){ return FALSE; }

	sys_VBlankFuncChange( NULL, NULL );	// VBlank�Z�b�g

	VSD_TimeIconDelete(wk);
	
	if( wk->dat->mode != VSD_MODE_MULTI ){
		VSD_VsBmpExit(wk);
		VSD_VsMsgmanExit(wk);
		VSD_3DBgExit( wk );
		VSD_VsParticleDelete( wk );
		VSD_VsFontOamDelete( wk );
	}else{
		VSD_PokeCheckBgExit( wk );
		VSD_PokeCheckBmpExit( wk );
	}

	VSD_ObjFree( wk );
	DellVramTransferManager();

	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_BG );
	PaletteFadeFree( wk->pfd );

	wk->dat->end_flg = 1;
	PMDS_taskDel( tcb );

	return TRUE;
}

//--------------------------------------------------------------------------------------------
/**
 * �����ʒu�擾
 *
 * @param	none
 *
 * @retval	"TRUE = ��"
 * @retval	"FALSE = �E"
 */
//--------------------------------------------------------------------------------------------
static BOOL VSD_GetStandID(void)
{
	switch( CommGetStandNo(CommGetCurrentID()) ){
	case 0:
	case 2:
		return TRUE;
	case 1:
	case 3:
		return FALSE;
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------
/**
 * ���\�[�X�}�l�[�W���[������
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_ClactResManInit( VSD_WORK * wk, TCATS_RESOURCE_NUM_LIST * crnl, int max )
{
	initVramTransferManagerHeap( 64, wk->dat->heap );

	wk->csp = CATS_AllocMemory( wk->dat->heap );
	wk->crp = CATS_ResourceCreate( wk->csp );
	{
		TCATS_OAM_INIT	coi = {
			0, 128,
			0, 32,
			0, 128,
			0, 32,
		};
		TCATS_CHAR_MANAGER_MAKE ccmm = {
			0,
			1024*64,
			1024*16,
			GX_OBJVRAMMODE_CHAR_1D_32K,
			GX_OBJVRAMMODE_CHAR_1D_32K
		};
		ccmm.num = max;
		CATS_SystemInit( wk->csp, &coi, &ccmm, 16 );
	}
	CATS_ClactSetInit( wk->csp, wk->crp, max );
	CATS_ResourceManagerInit( wk->csp, wk->crp, crnl );

	GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
}

//--------------------------------------------------------------------------------------------
/**
 * �Z���A�N�^�[�ǉ��i�ʁj
 *
 * @param	wk		���[�N
 * @param	id		�ǉ�����Z���A�N�^�[��ID
 * @param	pri		BG�Ƃ̃v���C�I���e�B
 *
 * @return	�Z���A�N�^�[�f�[�^
 */
//--------------------------------------------------------------------------------------------
static CATS_ACT_PTR VSD_ClactAdd( VSD_WORK * wk, const u32 * id )
{
	TCATS_OBJECT_ADD_PARAM_S	prm;

	prm.x = 0;
	prm.y = 0;
	prm.z = 0;

	prm.anm = 0;
	prm.pri = id[5];
	prm.pal = id[6];
	prm.d_area = NNS_G2D_VRAM_TYPE_2DMAIN;

	prm.id[0] = id[0];
	prm.id[1] = id[1];
	prm.id[2] = id[2];
	prm.id[3] = id[3];
	
	prm.bg_pri = id[4];
	prm.vram_trans = 0;

	return CATS_ObjectAdd_S( wk->csp, wk->crp, &prm );
}

//--------------------------------------------------------------------------------------------
/**
 * �Z���A�N�^�[���
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_ObjFree( VSD_WORK * wk )
{
	u32	i;

	for( i=0; i<wk->cap_max; i++ ){
		CATS_ActorPointerDelete_S( wk->cap[i] );
	}
	CATS_ResourceDestructor_S( wk->csp, wk->crp );
	CATS_FreeMemory( wk->csp );
}

//--------------------------------------------------------------------------------------------
/**
 * �Z���A�N�^�[�A�j��
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_ObjAnm( VSD_WORK * wk )
{
	u16	i;

	for( i=0; i<wk->cap_max; i++ ){
		CLACT_AnmFrameChg( wk->cap[i]->act, FX32_ONE );
	}
}


//============================================================================================
//	VS���/���s���
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * VBlank�֐�
 *
 * @param	work	���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsVBlank( void * work )
{
	VSD_WORK * wk = work;

	GF_BGL_VBlankFunc( wk->bgl );
	PaletteFadeTrans( wk->pfd );

	// �Z���A�N�^�[
	DoVramTransferManager();	// Vram�]���}�l�[�W���[���s
	CATS_RenderOamTrans();
	
	OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}

//--------------------------------------------------------------------------------------------
/**
 * BG������
 *
 * @param	wk	���[�N
 * @param   end_flag	TRUE:�ΐ��̉�ʁ@FALSE:�ΐ�O�̉��
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_3DBgInit( VSD_WORK * wk, GXBG0As bg_2d3d, int end_flag )
{
	wk->bg_2d3d = bg_2d3d;
	
	{	// BG SYSTEM
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D,
		};
		BGsys_data.bg0_2Dor3D = bg_2d3d;
		GF_BGL_InitBG( &BGsys_data );
	}

	{	// BG (CHAR)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_23,
			3, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( wk->bgl, GF_BGL_FRAME3_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( wk->bgl, GF_BGL_FRAME3_M );
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_X_SET, SCROLL_INIT );
	}

	{	// BG (CHAR)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_23,
			2, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( wk->bgl, GF_BGL_FRAME2_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( wk->bgl, GF_BGL_FRAME2_M );
		GF_BGL_ScrollReq( wk->bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_X_SET, -SCROLL_INIT );
	}

	{	// PARAM FONT (BMP)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			1, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( wk->bgl, GF_BGL_FRAME1_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( wk->bgl, GF_BGL_FRAME1_M );
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
		if(end_flag == TRUE){
			GF_BGL_ScrollSet( wk->bgl, GF_BGL_FRAME1_M, GF_BGL_SCROLL_Y_SET, END_DISP_OFFSET_Y);
		}
	}

	if(bg_2d3d == GX_BG0_AS_2D){
		//�E�B���h�E��(�u���̐퓬���L�^���܂����H�v)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x18000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( wk->bgl, GF_BGL_FRAME0_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( wk->bgl, GF_BGL_FRAME0_M );
		if(wk->rec_occ == TRUE){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		}
		else{
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
		}
	}

}

//--------------------------------------------------------------------------------------------
/**
 * BMP�폜
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsBmpExit( VSD_WORK * wk )
{
	if(wk->talk_win.chrbuf != NULL){
		GF_BGL_BmpWinDel( &wk->talk_win );
	}
	if(wk->sub_talk_win.chrbuf != NULL){
		GF_BGL_BmpWinDel( &wk->sub_talk_win );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * ���b�Z�[�W�}�l�[�W���폜
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsMsgmanExit( VSD_WORK * wk )
{
	if(wk->rec_talkmsg_buf != NULL){
		STRBUF_Delete(wk->rec_talkmsg_buf);
	}
	if(wk->rec_wordset != NULL){
		WORDSET_Delete(wk->rec_wordset);
	}
	if(wk->rec_msgman != NULL){
		MSGMAN_Delete(wk->rec_msgman);
	}
}

//--------------------------------------------------------------------------------------------
/**
 * BG���
 *
 * @param	wk	���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_3DBgExit( VSD_WORK * wk )
{
	GF_Disp_GX_VisibleControl(
		GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 |
		GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, VISIBLE_OFF );

	GF_BGL_BGControlExit( wk->bgl, GF_BGL_FRAME3_M );
	GF_BGL_BGControlExit( wk->bgl, GF_BGL_FRAME2_M );
	GF_BGL_BGControlExit( wk->bgl, GF_BGL_FRAME1_M );
	if(wk->bg_2d3d == GX_BG0_AS_2D){
		GF_BGL_BGControlExit( wk->bgl, GF_BGL_FRAME0_M );
	}

	sys_FreeMemoryEz( wk->bgl );
}

//--------------------------------------------------------------------------------------------
/**
 * BG�O���t�B�b�N�Z�b�g
 *
 * @param	wk	���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsBgGraphicSet( VSD_WORK * wk )
{
	ARCHANDLE* hdl;
	
	hdl = ArchiveDataHandleOpen( ARC_VS_DEMO_GRA,  wk->dat->heap ); 
	
	ArcUtil_HDL_BgCharSet(
		hdl, NARC_vs_demo_gra_t_buttle_ncgr,
		wk->bgl, GF_BGL_FRAME1_M, 0, 0, 0, wk->dat->heap );
	ArcUtil_HDL_ScrnSet(
		hdl, NARC_vs_demo_gra_t_buttle03_nscr,
		wk->bgl, GF_BGL_FRAME1_M, 0, 0, 0, wk->dat->heap );
	ArcUtil_HDL_ScrnSet(
		hdl, NARC_vs_demo_gra_t_buttle01_nscr,
		wk->bgl, GF_BGL_FRAME2_M, 0, 0, 0, wk->dat->heap );
	ArcUtil_HDL_ScrnSet(
		hdl, NARC_vs_demo_gra_t_buttle02_nscr,
		wk->bgl, GF_BGL_FRAME3_M, 0, 0, 0, wk->dat->heap );

	ArcUtil_HDL_PalSet(
		hdl, NARC_vs_demo_gra_t_buttle_nclr, PALTYPE_MAIN_BG, 0, 0, wk->dat->heap );
	PaletteWorkSet_VramCopy( wk->pfd, FADE_MAIN_BG, 0, 0x20*5 );

	GF_BGL_BackGroundColorSet( GF_BGL_FRAME1_M, 0x18c6 );
	
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------------------------------------
/**
 * ���b�Z�[�W�E�B���h�E�o�^
 *
 * @param	wk	���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsMessageWindowSet( VSD_WORK * wk )
{
	WINTYPE win_type;

	GF_ASSERT(wk->bg_2d3d == GX_BG0_AS_2D && wk->dat != NULL && wk->dat->bp != NULL && wk->dat->bp->config != NULL);
	
	//��b�E�B���h�E
	win_type = CONFIG_GetWindowType(wk->dat->bp->config);
	TalkWinGraphicSet(wk->bgl, GF_BGL_FRAME0_M, VS_DEMO_TALKWIN_CGX_OFFSET, 
		VS_DEMO_TALKWIN_PALNO, win_type, wk->dat->heap);
	PaletteWorkSet_VramCopy( wk->pfd, FADE_MAIN_BG, VS_DEMO_TALKWIN_PALNO * 16, 0x20*1 );

	//���j���[�E�C���h�E�L�������p���b�g�ǂݍ��݁i�E�C���h�E�O���j
	MenuWinGraphicSet(wk->bgl, GF_BGL_FRAME0_M, 
		VS_DEMO_SYSWIN_CGX_OFFSET, VS_DEMO_SYSWIN_PALNO, MENU_TYPE_SYSTEM, wk->dat->heap);
	PaletteWorkSet_VramCopy(wk->pfd, FADE_MAIN_BG, VS_DEMO_SYSWIN_PALNO * 16, 0x20*1 );

	//�t�H���g�p���b�g�ǂݍ��݁i�V�X�e���j
	SystemFontPaletteLoad( PALTYPE_MAIN_BG, VS_DEMO_SYSFONT_PALNO * 0x20, wk->dat->heap);
	PaletteWorkSet_VramCopy(wk->pfd, FADE_MAIN_BG, VS_DEMO_SYSFONT_PALNO * 16, 0x20*1 );
	
	//BMP�E�B���h�E�ǉ�
	GF_BGL_BmpWinAdd(wk->bgl, &wk->talk_win, GF_BGL_FRAME0_M, 
		VS_DEMO_BMPWIN_TALK_POS_X, VS_DEMO_BMPWIN_TALK_POS_Y,
		VS_DEMO_BMPWIN_TALK_SIZE_X, VS_DEMO_BMPWIN_TALK_SIZE_Y,
		VS_DEMO_SYSFONT_PALNO, VS_DEMO_TALK_STR_CGX_START);
	GF_BGL_BmpWinAdd(wk->bgl, &wk->sub_talk_win, GF_BGL_FRAME0_M, 
		VS_DEMO_BMPWIN_TALK_POS_X, VS_DEMO_BMPWIN_TALK_POS_Y,
		VS_DEMO_BMPWIN_TALK_YESNO_SET_SIZE_X, VS_DEMO_BMPWIN_TALK_YESNO_SET_SIZE_Y,
		VS_DEMO_SYSFONT_PALNO, VS_DEMO_TALK_STR_CGX_START);
}

//--------------------------------------------------------------------------------------------
/**
 * �������ݒ�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsAlphaSet(void)
{
//	G2_SetBlendAlpha(
//		GX_BLEND_PLANEMASK_BG0|GX_BLEND_PLANEMASK_BG1,
//		GX_BLEND_PLANEMASK_BG2|GX_BLEND_PLANEMASK_BG3|GX_BLEND_PLANEMASK_OBJ, 23, 8 );
	G2_SetBlendAlpha(
		GX_BLEND_PLANEMASK_BG1,
		GX_BLEND_PLANEMASK_BG2|GX_BLEND_PLANEMASK_BG3|GX_BLEND_PLANEMASK_OBJ, 8, 8 );
}

//--------------------------------------------------------------------------------------------
/**
 * VS��ʁFOBJ�ǉ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsObjMake( VSD_WORK * wk )
{
	TCATS_RESOURCE_NUM_LIST	crnl = {
		VS_CHR_ID_MAX, VS_PAL_ID_MAX, VS_CEL_ID_MAX, VS_ANM_ID_MAX, 0, 0
	};
	u32	i;

	VSD_ClactResManInit( wk, &crnl, VS_CLA_MAX+8+1 );

	VSD_VsBallObjMake( wk );

	for( i=0; i<VS_CLA_MAX; i++ ){
		wk->cap[i] = VSD_ClactAdd( wk, &VS_ClactParam[i][0] );
	}
	VSD_VsBallObjMove( wk, 0, -SCROLL_INIT, 0 );
	VSD_VsBallObjMove( wk, 1, -SCROLL_INIT, 0 );
	VSD_VsBallObjMove( wk, 2, SCROLL_INIT, 0 );
	VSD_VsBallObjMove( wk, 3, SCROLL_INIT, 0 );
	VSD_VsBallAnmChg( wk );

	wk->cap_max = VS_CLA_MAX;

	VSD_VsFontOamMake( wk );
	VSD_VsFontOamMove( wk, 0, -SCROLL_INIT, 0 );
	VSD_VsFontOamMove( wk, 1, -SCROLL_INIT, 0 );
	VSD_VsFontOamMove( wk, 2, SCROLL_INIT, 0 );
	VSD_VsFontOamMove( wk, 3, SCROLL_INIT, 0 );
}

//--------------------------------------------------------------------------------------------
/**
 * �{�[��OBJ���\�[�X�ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsBallObjMake( VSD_WORK * wk )
{
	
	ARCHANDLE* hdl;
	
	hdl = ArchiveDataHandleOpen( ARC_BATT_OBJ,  wk->dat->heap ); 
	
	CATS_LoadResourceCharArcH(
		wk->csp, wk->crp, hdl,
		BATTLE_STOCK_M_NCGR_BIN, 1, NNS_G2D_VRAM_TYPE_2DMAIN, VS_CHR_ID_BALL );
/*
	CATS_LoadResourcePlttArc(
		wk->csp, wk->crp, ARC_BATT_OBJ, BATTLE_WOBJ_NCLR,
		0, 2, NNS_G2D_VRAM_TYPE_2DMAIN, VS_PAL_ID_BALL );
*/
	CATS_LoadResourcePlttArc(
		wk->csp, wk->crp, ARC_VS_DEMO_GRA,
		NARC_vs_demo_gra_t_buttle_font_nclr, 0, 2, NNS_G2D_VRAM_TYPE_2DMAIN, VS_PAL_ID_BALL );
	CATS_LoadResourceCellArcH(
		wk->csp, wk->crp, hdl, BATTLE_STOCK_M_NCER_BIN, 1, VS_CEL_ID_BALL );
	CATS_LoadResourceCellAnmArcH(
		wk->csp, wk->crp, hdl, BATTLE_STOCK_M_NANR_BIN, 1, VS_ANM_ID_BALL );
	
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------------------------------------
/**
 * �{�[��OBJ�ړ�
 *
 * @param	wk		���[�N
 * @param	flg		0=�������O��, 1=�������㔼, 2=�G���O��, 3=�G���㔼
 * @param	px		X�ړ��l
 * @param	py		Y�ړ��l
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsBallObjMove( VSD_WORK * wk, u8 flg, s16 px, s16 py )
{
	u32	i;
	u8	start[] = { VS_CLA_BALL01, VS_CLA_BALL04, VS_CLA_BALL11, VS_CLA_BALL14 };

	for( i=0; i<3; i++ ){
		CATS_ObjectPosSetCap(
			wk->cap[ start[flg]+i ],
			wk->ball_gx[flg] + i*VS_CLA_BALL_SX + px, wk->ball_gy[flg] + py );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * �{�[���A�j���؂�ւ����C��
 *
 * @param	wk		���[�N
 * @param	party	�|�P�p�[�e�B
 * @param	max		�{�[���̐�
 * @param	cla_id	�Z���A�N�^�[��ID
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsPartyBallAnmChg( VSD_WORK * wk, POKEPARTY * party, u16 max, u16 cla_id )
{
	POKEMON_PARAM * pp;
	u8	i;
	u8	prm, pp_cnt, add_cnt;

	pp_cnt  = (u8)PokeParty_GetPokeCount( party );
	add_cnt = 0;

	for( i=0; i<max; i++ ){
		CATS_ObjectAnimeSeqSetCap( wk->cap[i+cla_id], STOCK_ANMSEQ_NONE );
		if( pp_cnt > i ){
			pp = PokeParty_GetMemberPointer( party, i );

			if( PokeParaGet( pp, ID_PARA_monsno, NULL ) == 0 ||
				PokeParaGet( pp, ID_PARA_tamago_flag, NULL ) != 0 ){
				continue;
			}

			prm = BadStatusIconAnmGet( pp );

			if( prm == ST_ICON_HINSI ){
				CATS_ObjectAnimeSeqSetCap( wk->cap[add_cnt+cla_id], STOCK_ANMSEQ_DEAD );
			}else if( prm != ST_ICON_NONE ){
				CATS_ObjectAnimeSeqSetCap( wk->cap[add_cnt+cla_id], STOCK_ANMSEQ_NG );
			}else{
				CATS_ObjectAnimeSeqSetCap( wk->cap[add_cnt+cla_id], STOCK_ANMSEQ_ALIVE );
			}

			add_cnt++;
		}
	}
}

//--------------------------------------------------------------------------------------------
/**
 * �{�[���A�j���؂�ւ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsBallAnmChg( VSD_WORK * wk )
{
	if( wk->dat->type == VSD_TYPE_NORMAL ){
		VSD_VsPartyBallAnmChg( wk, wk->dat->pp[VSD_PARTY_LEFT1], 6, VS_CLA_BALL01 );
		VSD_VsPartyBallAnmChg( wk, wk->dat->pp[VSD_PARTY_RIGHT1], 6, VS_CLA_BALL11 );
	}else{
		VSD_VsPartyBallAnmChg( wk, wk->dat->pp[VSD_PARTY_LEFT1], 3, VS_CLA_BALL01 );
		VSD_VsPartyBallAnmChg( wk, wk->dat->pp[VSD_PARTY_LEFT2], 3, VS_CLA_BALL04 );
		VSD_VsPartyBallAnmChg( wk, wk->dat->pp[VSD_PARTY_RIGHT1], 3, VS_CLA_BALL11 );
		VSD_VsPartyBallAnmChg( wk, wk->dat->pp[VSD_PARTY_RIGHT2], 3, VS_CLA_BALL14 );
	}
}



//--------------------------------------------------------------------------------------------
/**
 * �t�H���gOAM�쐬�i�ʁj
 *
 * @param	wk		���[�N
 * @param	id		�쐬ID
 * @param	str		������
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsFontOamMakeMain( VSD_WORK * wk, u32 id, STRBUF * str )
{
	GF_BGL_BMPWIN * win;
	FONTOAM_INIT	init;

	win = &wk->win[0];

	GF_BGL_BmpWinObjAdd( wk->bgl, win, VSD_BMP_PNAME_SX, VSD_BMP_PNAME_SY, 0, 0 );

	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, 0, MSG_NO_PUT, GF_PRINTCOLOR_MAKE(3,4,0), NULL );

	init.fontoam_sys = wk->fsp;
	init.bmp         = win;
	init.clact_set   = CATS_GetClactSetPtr( wk->crp );
	init.pltt        = CATS_PlttProxy( wk->crp, VS_PAL_ID_BALL );
	init.parent      = NULL;
	init.char_ofs    = (VSD_BMP_PNAME1_CGX-VSD_BMP_PNAME_SIZ*id)*0x20;
	init.x           = 0;
	init.y           = 0;
	init.bg_pri      = 2;
	init.soft_pri    = 0;
	init.draw_area   = NNS_G2D_VRAM_TYPE_2DMAIN;
	init.heap        = wk->dat->heap;
	
	wk->fop[id] = FONTOAM_Init( &init );
	FONTOAM_SetPaletteNo( wk->fop[id], 0 );
	
	GF_BGL_BmpWinDel( win );	// �������
}

//--------------------------------------------------------------------------------------------
/**
 * �t�H���gOAM�쐬
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsFontOamMake( VSD_WORK * wk )
{
	wk->fsp = FONTOAM_SysInit( 8, wk->dat->heap );

	VSD_VsFontOamMakeMain( wk, 0, wk->dat->name[VSD_PARTY_LEFT1] );
	VSD_VsFontOamMakeMain( wk, 2, wk->dat->name[VSD_PARTY_RIGHT1] );

	if( wk->dat->type == VSD_TYPE_MULTI ){
		VSD_VsFontOamMakeMain( wk, 1, wk->dat->name[VSD_PARTY_LEFT2] );
		VSD_VsFontOamMakeMain( wk, 3, wk->dat->name[VSD_PARTY_RIGHT2] );
	}else{
		VSD_VsFontOamMakeMain( wk, 1, wk->dat->name[VSD_PARTY_LEFT1] );		// �_�~�[
		VSD_VsFontOamMakeMain( wk, 3, wk->dat->name[VSD_PARTY_RIGHT1] );	// �_�~�[
		FONTOAM_SetDrawFlag( wk->fop[1], FALSE );
		FONTOAM_SetDrawFlag( wk->fop[3], FALSE );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * �t�H���gOAM�ړ�
 *
 * @param	wk		���[�N
 * @param	id		0=�������O��, 1=�������㔼, 2=�G���O��, 3=�G���㔼
 * @param	px		X�ړ��l
 * @param	py		Y�ړ��l
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsFontOamMove( VSD_WORK * wk, u32 id, s16 x, s16 y )
{
	FONTOAM_SetMat( wk->fop[id], wk->name_gx[id]+x, wk->name_gy[id]+y );
}

//--------------------------------------------------------------------------------------------
/**
 * �t�H���gOAM�폜
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsFontOamDelete( VSD_WORK * wk )
{
	u32	i;

	if( wk->dat->mode == VSD_MODE_MULTI ){ return; }

	for( i=0; i<4; i++ ){
		FONTOAM_Delete( wk->fop[i] );
	}
	FONTOAM_SysDelete( wk->fsp );
}

//--------------------------------------------------------------------------------------------
/**
 * OBJ�������ʒu�ֈړ�
 *
 * @param	wk		���[�N
 * @param   offset_left_y	�I�t�Z�b�gY(����)
 * @param   offset_right_y	�I�t�Z�b�gY(�E��)
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsObjGlobalPosSet( VSD_WORK * wk, int offset_left_y, int offset_right_y )
{
	if( wk->dat->type == VSD_TYPE_MULTI ){
		wk->ball_gx[0] = VS_CLA_BALL00M_PX;
		wk->ball_gy[0] = VS_CLA_BALL00M_PY + offset_left_y;
		wk->ball_gx[1] = VS_CLA_BALL03M_PX;
		wk->ball_gy[1] = VS_CLA_BALL03M_PY + offset_left_y;
		wk->ball_gx[2] = VS_CLA_BALL10M_PX;
		wk->ball_gy[2] = VS_CLA_BALL10M_PY + offset_right_y;
		wk->ball_gx[3] = VS_CLA_BALL13M_PX;
		wk->ball_gy[3] = VS_CLA_BALL13M_PY + offset_right_y;

		wk->name_gx[0] = VS_FOAM_NAME00M_PX;
		wk->name_gy[0] = VS_FOAM_NAME00M_PY + offset_left_y;
		wk->name_gx[1] = VS_FOAM_NAME03M_PX;
		wk->name_gy[1] = VS_FOAM_NAME03M_PY + offset_left_y;
		wk->name_gx[2] = VS_FOAM_NAME10M_PX;
		wk->name_gy[2] = VS_FOAM_NAME10M_PY + offset_right_y;
		wk->name_gx[3] = VS_FOAM_NAME13M_PX;
		wk->name_gy[3] = VS_FOAM_NAME13M_PY + offset_right_y;
	}else{
		wk->ball_gx[0] = VS_CLA_BALL00_PX;
		wk->ball_gy[0] = VS_CLA_BALL00_PY + offset_left_y;
		wk->ball_gx[1] = VS_CLA_BALL03_PX;
		wk->ball_gy[1] = VS_CLA_BALL03_PY + offset_left_y;
		wk->ball_gx[2] = VS_CLA_BALL10_PX;
		wk->ball_gy[2] = VS_CLA_BALL10_PY + offset_right_y;
		wk->ball_gx[3] = VS_CLA_BALL13_PX;
		wk->ball_gy[3] = VS_CLA_BALL13_PY + offset_right_y;

		wk->name_gx[0] = VS_FOAM_NAME00_PX;
		wk->name_gy[0] = VS_FOAM_NAME00_PY + offset_left_y;
		wk->name_gx[2] = VS_FOAM_NAME10_PX;
		wk->name_gy[2] = VS_FOAM_NAME10_PY + offset_right_y;
	}
}

//--------------------------------------------------------------------------------------------
/**
 * ���s��ʁFOBJ�ǉ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsEndObjMake( VSD_WORK * wk )
{
	TCATS_RESOURCE_NUM_LIST	crnl = {
		VS_CHR_ID_MAX, VS_PAL_ID_MAX, VS_CEL_ID_MAX, VS_ANM_ID_MAX, 0, 0
	};
	u32	i;

	VSD_ClactResManInit( wk, &crnl, VS_CLA_END_MAX+8+1 );

	VSD_VsBallObjMake( wk );
	VSD_VsResultObjMake( wk );

	for( i=0; i<VS_CLA_END_MAX; i++ ){
		wk->cap[i] = VSD_ClactAdd( wk, &VS_ClactParam[i][0] );
	}

	VSD_VsBallObjMove( wk, 0, -SCROLL_INIT, 0 );
	VSD_VsBallObjMove( wk, 1, -SCROLL_INIT, 0 );
	VSD_VsBallObjMove( wk, 2, SCROLL_INIT, 0 );
	VSD_VsBallObjMove( wk, 3, SCROLL_INIT, 0 );
	VSD_VsBallAnmChg( wk );

	VSD_VsResultMove( wk );

	VSD_VsFontOamMake( wk );
	VSD_VsFontOamMove( wk, 0, -SCROLL_INIT, 0 );
	VSD_VsFontOamMove( wk, 1, -SCROLL_INIT, 0 );
	VSD_VsFontOamMove( wk, 2, SCROLL_INIT, 0 );
	VSD_VsFontOamMove( wk, 3, SCROLL_INIT, 0 );

	wk->cap_max = VS_CLA_END_MAX;

}

//--------------------------------------------------------------------------------------------
/**
 * ���sOBJ���\�[�X�ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsResultObjMake( VSD_WORK * wk )
{
	
	ARCHANDLE* hdl;
	
	hdl = ArchiveDataHandleOpen( ARC_VS_DEMO_GRA,  wk->dat->heap ); 
	
	CATS_LoadResourceCharArcH(
		wk->csp, wk->crp, hdl,
		NARC_vs_demo_gra_t_buttle_font_ncgr, 0, NNS_G2D_VRAM_TYPE_2DMAIN, VS_CHR_ID_RESULT );
	CATS_LoadResourceCellArcH(
		wk->csp, wk->crp, hdl,
		NARC_vs_demo_gra_t_battle_font_ncer, 0, VS_CEL_ID_RESULT );
	CATS_LoadResourceCellAnmArcH(
		wk->csp, wk->crp, hdl,
		NARC_vs_demo_gra_t_battle_font_nanr, 0, VS_ANM_ID_RESULT );
	
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------------------------------------
/**
 * ���sOBJ�\���ʒu�Z�b�g
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsResultMove( VSD_WORK * wk )
{
	CATS_ObjectEnableCap( wk->cap[VS_CLA_RESULT1], 0 );
	CATS_ObjectEnableCap( wk->cap[VS_CLA_RESULT2], 0 );

	if( wk->dat->result == VSD_RESULT_DRAW ){
		CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT1], RESULT_DRAW_PX, RESULT_OBJ_PY );
		CATS_ObjectAnimeSeqSetCap( wk->cap[VS_CLA_RESULT1], VSD_RESULT_DRAW-1 );
		return;
	}

	if( VSD_GetStandID() == TRUE ){
		if( wk->dat->result == VSD_RESULT_WIN ){
			CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT1], RESULT_OBJ_PX1, RESULT_OBJ_PY );
			CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT2], RESULT_OBJ_PX2, RESULT_OBJ_PY );
		}else{
			CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT1], RESULT_OBJ_PX2, RESULT_OBJ_PY );
			CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT2], RESULT_OBJ_PX1, RESULT_OBJ_PY );
		}
	}else{
		if( wk->dat->result == VSD_RESULT_WIN ){
			CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT1], RESULT_OBJ_PX2, RESULT_OBJ_PY );
			CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT2], RESULT_OBJ_PX1, RESULT_OBJ_PY );
		}else{
			CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT1], RESULT_OBJ_PX1, RESULT_OBJ_PY );
			CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT2], RESULT_OBJ_PX2, RESULT_OBJ_PY );
		}
	}
	CATS_ObjectAnimeSeqSetCap( wk->cap[VS_CLA_RESULT1], VSD_RESULT_WIN-1 );
	CATS_ObjectAnimeSeqSetCap( wk->cap[VS_CLA_RESULT2], VSD_RESULT_LOSE-1 );


/*
	switch( wk->dat->result ){
	case VSD_RESULT_WIN:
		CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT1], RESULT_OBJ_PX1, RESULT_OBJ_PY );
		CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT2], RESULT_OBJ_PX2, RESULT_OBJ_PY );
		CATS_ObjectAnimeSeqSetCap( wk->cap[VS_CLA_RESULT1], VSD_RESULT_WIN-1 );
		CATS_ObjectAnimeSeqSetCap( wk->cap[VS_CLA_RESULT2], VSD_RESULT_LOSE-1 );
		break;
	case VSD_RESULT_LOSE:
		CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT1], RESULT_OBJ_PX2, RESULT_OBJ_PY );
		CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT2], RESULT_OBJ_PX1, RESULT_OBJ_PY );
		CATS_ObjectAnimeSeqSetCap( wk->cap[VS_CLA_RESULT1], VSD_RESULT_WIN-1 );
		CATS_ObjectAnimeSeqSetCap( wk->cap[VS_CLA_RESULT2], VSD_RESULT_LOSE-1 );
		break;
	default:
		CATS_ObjectPosSetCap( wk->cap[VS_CLA_RESULT1], RESULT_DRAW_PX, RESULT_OBJ_PY );
		CATS_ObjectAnimeSeqSetCap( wk->cap[VS_CLA_RESULT1], VSD_RESULT_DRAW-1 );
	}
*/
}


//============================================================================================
//	�p�[�e�B�N��
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �p�[�e�B�N��������
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsParticleInit( VSD_WORK * wk )
{
	GF_CAMERA_PTR	camera;
	void * res;

	wk->g3dm = GF_G3DMAN_Init(
					wk->dat->heap,
					GF_G3DMAN_LNK, GF_G3DTEX_512K, GF_G3DMAN_LNK, GF_G3DPLT_32K, NULL );

	G3X_AlphaBlend( TRUE );

	// �p�[�e�B�N���V�X�e��������
	Particle_SystemWorkInit();

	// �p�[�e�B�N���V�X�e���쐬
	wk->ptm = sys_AllocMemory( wk->dat->heap, PARTICLE_LIB_HEAP_SIZE );
	wk->ptc = Particle_SystemCreate(
				sAllocTex, sAllocTexPalette, wk->ptm, PARTICLE_LIB_HEAP_SIZE, TRUE, wk->dat->heap );
	camera  = Particle_GetCameraPtr( wk->ptc );
	GFC_SetCameraClip( BP_NEAR, BP_FAR, camera );

	// ���\�[�X�ǂݍ��݁��o�^
	res = Particle_ArcResourceLoad(
			ARC_PARTICLE_DATA, NARC_particledata_vs_battle_spa, wk->dat->heap );
	Particle_ResourceSet( wk->ptc, res, PTC_AUTOTEX_LNK | PTC_AUTOPLTT_LNK, TRUE );

	// �p�[�e�B�N������
	Particle_CreateEmitterCallback( wk->ptc, VS_BATTLE_T_BUTTLE01, NULL, NULL );
	Particle_CreateEmitterCallback( wk->ptc, VS_BATTLE_T_BUTTLE01A, NULL, NULL );
	Particle_CreateEmitterCallback( wk->ptc, VS_BATTLE_T_BUTTLE01B, NULL, NULL );
	Particle_CreateEmitterCallback( wk->ptc, VS_BATTLE_T_BUTTLE02, NULL, NULL );
	Particle_CreateEmitterCallback( wk->ptc, VS_BATTLE_T_BUTTLE03, NULL, NULL );
}

//--------------------------------------------------------------------------------------------
/**
 * �p�[�e�B�N���`��
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static int VSD_VsParticleMain( VSD_WORK * wk )
{
	const MtxFx43 * camera;
	int	draw_num;

	if( wk->dat->mode != VSD_MODE_IN ){ return FALSE; }

	GF_G3X_Reset();

	if( Particle_GetEmitterNum( wk->ptc ) == 0 ){ return FALSE; }

	Particle_DrawAll();		// �p�[�e�B�N���`��
	Particle_CalcAll();		// �p�[�e�B�N���v�Z

	return TRUE;
}

//--------------------------------------------------------------------------------------------
/**
 * �p�[�e�B�N���폜
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_VsParticleDelete( VSD_WORK * wk )
{
	if( wk->dat->mode != VSD_MODE_IN ){ return; }

	Particle_SystemExit( wk->ptc );
	sys_FreeMemoryEz( wk->ptm );

	GF_G3D_Exit( wk->g3dm );
}

//--------------------------------------------------------------------------------------------
/**
 * �e�N�X�`��VRAM�A�h���X��Ԃ����߂̃R�[���o�b�N�֐�
 *
 * @param   size		�e�N�X�`���T�C�Y
 * @param   is4x4comp	4x4���k�e�N�X�`���ł��邩�ǂ����̃t���O(TRUE=���k�e�N�X�`��)
 *
 * @return  �ǂݍ��݂��J�n����VRAM�̃A�h���X
 */
//--------------------------------------------------------------------------------------------
static u32 sAllocTex( u32 size, BOOL is4x4comp )
{
	u32 address;
	NNSGfdTexKey key;
	
	key = NNS_GfdAllocTexVram(size, is4x4comp, 0);
	Particle_LnkTexKeySet(key);		//�����N�h���X�g���g�p���Ă���̂ŃL�[�����Z�b�g
	GF_ASSERT(key != NNS_GFD_ALLOC_ERROR_TEXKEY);
	
	address = NNS_GfdGetTexKeyAddr(key);
#ifdef OSP_VSDEMO_ON
	OS_TPrintf("vram=%d\n", address);
#endif
	return address;
}

//--------------------------------------------------------------------------------------------
/**
 * �e�N�X�`���p���b�gVRAM�A�h���X��Ԃ����߂̃R�[���o�b�N�֐�
 *
 * @param	size		�e�N�X�`���T�C�Y
 * @param	is4pltt		4�F�p���b�g�ł��邩�ǂ����̃t���O
 *
 * @retval	�ǂݍ��݂��J�n����VRAM�̃A�h���X
 *
 * direct�`���̃e�N�X�`���̏ꍇ�ASPL_LoadTexPlttByCallbackFunction��
 * �R�[���o�b�N�֐����Ăяo���܂���B
 */
//--------------------------------------------------------------------------------------------
static u32 sAllocTexPalette( u32 size, BOOL is4pltt )
{
	NNSGfdPlttKey key;
	u32 address;
	
	key = NNS_GfdAllocPlttVram(size, is4pltt, NNS_GFD_ALLOC_FROM_LOW);
	Particle_PlttLnkTexKeySet(key);	//�����N�h���X�g���g�p���Ă���̂ŃL�[�����Z�b�g
	if(key == NNS_GFD_ALLOC_ERROR_PLTTKEY){
		GF_ASSERT(0);// && "�p�[�e�B�N���Ńp���b�g�̊m�ۂ��o���܂���I\n");
	}
	address = NNS_GfdGetPlttKeyAddr(key);
#ifdef OSP_VSDEMO_ON
	OS_TPrintf("vram=%d\n", address);
#endif
	return address;
}


//============================================================================================
//	�|�P�����m�F���
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * BG������
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBgInit( VSD_WORK * wk )
{
	{	// BG SYSTEM
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	{	// PARAM FONT (BMP)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
			1, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( wk->bgl, GF_BGL_FRAME1_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( wk->bgl, GF_BGL_FRAME1_M );
	}

	{	// PLATE (CHAR)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			2, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( wk->bgl, GF_BGL_FRAME2_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( wk->bgl, GF_BGL_FRAME2_M );
	}

	{	// BG (CHAR)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			3, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( wk->bgl, GF_BGL_FRAME3_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * BG�폜
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBgExit( VSD_WORK * wk )
{
	GF_Disp_GX_VisibleControl(
		GX_PLANEMASK_BG1|GX_PLANEMASK_BG2|GX_PLANEMASK_BG3|GX_PLANEMASK_OBJ, VISIBLE_OFF );

	GF_BGL_BGControlExit( wk->bgl, GF_BGL_FRAME3_M );
	GF_BGL_BGControlExit( wk->bgl, GF_BGL_FRAME2_M );
	GF_BGL_BGControlExit( wk->bgl, GF_BGL_FRAME1_M );

	sys_FreeMemoryEz( wk->bgl );
}

//--------------------------------------------------------------------------------------------
/**
 * BG�O���t�B�b�N�Z�b�g
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBgGraphicSet( VSD_WORK * wk )
{
	ARCHANDLE* hdl;

	hdl  = ArchiveDataHandleOpen( ARC_PLIST_GRA,  wk->dat->heap );
	
	ArcUtil_HDL_BgCharSet(
		hdl, NARC_plist_gra_p_list_NCGR,
		wk->bgl, GF_BGL_FRAME3_M, 0, 0, 0, wk->dat->heap );
	ArcUtil_HDL_ScrnSet(
		hdl, NARC_plist_gra_p_list_NSCR,
		wk->bgl, GF_BGL_FRAME3_M, 0, 0, 0, wk->dat->heap );

	{
		NNSG2dPaletteData * pal;
		void * buf;
		u16 * cpy;
		
		buf = ArchiveDataLoadAllocByHandle( hdl, NARC_plist_gra_p_list_NCLR, wk->dat->heap );
		NNS_G2dGetUnpackedPaletteData( buf, &pal );
		GF_BGL_PaletteSet( GF_BGL_FRAME3_M, (void *)pal->pRawData, pal->szByte, 0 );
		cpy = (u16 *)pal->pRawData;
		memcpy( wk->pal, &cpy[PAL_PANEL_0*16], 32*3 );
		sys_FreeMemoryEz( buf );
	}

	PoleList_PokePlateScreenGet(
		wk->dat->heap, wk->plate_top, wk->plate_normal, wk->plate_none );
		
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------------------------------------
/**
 * BG�v���[�g�Z�b�g
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBgPut( VSD_WORK * wk )
{
	u16 * plate;
	u32	i;

	for( i=0; i<6; i++ ){
		if( i == 0 || i == 3 ){
			plate = wk->plate_top;
		}else if( wk->poke[i].mons == 0 ){
			plate = wk->plate_none;
		}else{
			plate = wk->plate_normal;
		}
		GF_BGL_ScrWriteExpand(
			wk->bgl, GF_BGL_FRAME2_M, PokeCheckBgPos[i][0], PokeCheckBgPos[i][1],
			PL_PANEL_SX, PL_PANEL_SY, (const void *)plate, 0, 0, PL_PANEL_SX, PL_PANEL_SY );

		if( wk->poke[i].mons != 0 ){
			GF_BGL_ScrPalChange(
				wk->bgl, GF_BGL_FRAME2_M, PokeCheckBgPos[i][0],
				PokeCheckBgPos[i][1], PL_PANEL_SX, PL_PANEL_SY, PAL_PANEL_0+i );

			if( ( i < 3 && wk->dat->type == VSD_TYPE_RIGHT ) ||
				( i >= 3 && wk->dat->type == VSD_TYPE_LEFT ) ){
				GF_BGL_PaletteSet(
					GF_BGL_FRAME2_M, (void *)&wk->pal[16*1], 16*2, (PAL_PANEL_0+i)*16*2 );
			}else{
				GF_BGL_PaletteSet(
					GF_BGL_FRAME2_M, (void *)&wk->pal[0], 16*2, (PAL_PANEL_0+i)*16*2 );
			}
		}
	}
	GF_BGL_LoadScreenV_Req( wk->bgl, GF_BGL_FRAME2_M );
}

//--------------------------------------------------------------------------------------------
/**
 * �C�����Ă���BG�v���[�g���R�s�[�i���C���j
 *
 * @param	wk		���[�N
 * @param	cpy		�R�s�[��
 * @param	frm		�R�s�[����BG�t���[��
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBgCopyMain( VSD_WORK * wk, u16 * cpy, u8 frm )
{
	u16 * buf;
	u16	px, i;

	buf = GF_BGL_ScreenAdrsGet( wk->bgl, frm );
	if( wk->dat->type == VSD_TYPE_LEFT ){
		px = 16;
	}else{
		px = 0;
	}
	for( i=0; i<24; i++ ){
		memcpy( &cpy[i*16], &buf[i*32+px], PL_PANEL_SX*2 );
		memset( &buf[i*32+px], 0, PL_PANEL_SX*2 );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * �C�����Ă���BG�v���[�g���R�s�[
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBgCopy( VSD_WORK * wk )
{
	VSD_PokeCheckBgCopyMain( wk, &wk->plate_bg[0][0], GF_BGL_FRAME1_M );
	VSD_PokeCheckBgCopyMain( wk, &wk->plate_bg[1][0], GF_BGL_FRAME2_M );
	GF_BGL_LoadScreenV_Req( wk->bgl, GF_BGL_FRAME1_M );
	GF_BGL_LoadScreenV_Req( wk->bgl, GF_BGL_FRAME2_M );
}

//--------------------------------------------------------------------------------------------
/**
 * BG�v���[�g�ړ�
 *
 * @param	wk		���[�N
 * @param	cnt		�ړ��J�E���g
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBgMove( VSD_WORK * wk, u8 cnt )
{
	if( wk->dat->type == VSD_TYPE_LEFT ){
		GF_BGL_ScrWriteExpand(
			wk->bgl, GF_BGL_FRAME1_M, 16+16-cnt, 0, cnt, 24,
			(const void *)&wk->plate_bg[0][0], 0, 0, 16, 24 );
		GF_BGL_ScrWriteExpand(
			wk->bgl, GF_BGL_FRAME2_M, 16+16-cnt, 0, cnt, 24,
			(const void *)&wk->plate_bg[1][0], 0, 0, 16, 24 );
	}else{
		GF_BGL_ScrWriteExpand(
			wk->bgl, GF_BGL_FRAME1_M, 0, 0, cnt, 24,
			(const void *)&wk->plate_bg[0][0], 16-cnt, 0, 16, 24 );
		GF_BGL_ScrWriteExpand(
			wk->bgl, GF_BGL_FRAME2_M, 0, 0, cnt, 24,
			(const void *)&wk->plate_bg[1][0], 16-cnt, 0, 16, 24 );
	}
	GF_BGL_LoadScreenV_Req( wk->bgl, GF_BGL_FRAME1_M );
	GF_BGL_LoadScreenV_Req( wk->bgl, GF_BGL_FRAME2_M );
}

//--------------------------------------------------------------------------------------------
/**
 * �|�P�����p�����[�^�擾���C��
 *
 * @param	wk		���[�N
 * @param	party	�|�P�p�[�e�B
 * @param	start	�J�n�ʒu
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokePartyParamGet( VSD_WORK * wk, POKEPARTY * party, u8 start )
{
	POKEMON_PARAM * pp;
	u16	i;
	u16	pp_cnt;

	pp_cnt = (u16)PokeParty_GetPokeCount( party );

//	memset( wk->poke, 0, sizeof(VSD_POKE)*6 );

	for( i=0; i<3; i++ ){
		if( pp_cnt <= i ){
			wk->poke[i+start].chr_arc = PokeIconCgxArcIndexGetByMonsNumber( 0, 0, 0 );
			continue;
		}

		pp = PokeParty_GetMemberPointer( party, i );

		wk->poke[i+start].chr_arc = PokeIconCgxArcIndexGetByPP( pp );

		wk->poke[i+start].mons = (u16)PokeParaGet( pp, ID_PARA_monsno, NULL );
		if( wk->poke[i+start].mons == 0 ){ continue; }

		wk->poke[i+start].egg  = (u16)PokeParaGet( pp, ID_PARA_tamago_flag, NULL );
		wk->poke[i+start].hp   = (u16)PokeParaGet( pp, ID_PARA_hp, NULL );
		wk->poke[i+start].mhp  = (u16)PokeParaGet( pp, ID_PARA_hpmax, NULL );
		wk->poke[i+start].lv   = (u16)PokeParaGet( pp, ID_PARA_level, NULL );
		wk->poke[i+start].item = (u16)PokeParaGet( pp, ID_PARA_item, NULL );
		wk->poke[i+start].cb   = (u8)PokeParaGet( pp, ID_PARA_cb_id, NULL );
		wk->poke[i+start].form = (u8)PokeParaGet( pp, ID_PARA_form_no, NULL );
		if( PokeParaGet( pp, ID_PARA_nidoran_nickname, NULL ) == TRUE ){
			wk->poke[i+start].sex_put = 0;
		}else{
			wk->poke[i+start].sex_put = 1;
		}
		wk->poke[i+start].sex = PokeSexGet( pp );
		wk->poke[i+start].st  = (u8)BadStatusIconAnmGet( pp );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * �|�P�����p�����[�^�擾
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeParamGet( VSD_WORK * wk )
{
	memset( wk->poke, 0, sizeof(VSD_POKE)*6 );
	VSD_PokePartyParamGet( wk, wk->dat->pp[VSD_PARTY_LEFT1], 0 );
	VSD_PokePartyParamGet( wk, wk->dat->pp[VSD_PARTY_LEFT2], 3 );
}


//--------------------------------------------------------------------------------------------
/**
 * �|�P�����A�C�R���A�j���擾
 *
 * @param	wk		���[�N
 * @param	pos		�ʒu
 *
 * @return	�A�j���ԍ�
 */
//--------------------------------------------------------------------------------------------
static u8 VSD_PokeIconAnmGet( VSD_WORK * wk, u32 pos )
{
	if( wk->poke[pos].hp == 0 ){
		return POKEICON_ANM_DEATH;
	}

	if( wk->poke[pos].st != ST_ICON_NONE && wk->poke[pos].st != ST_ICON_POKERUS ){
		return POKEICON_ANM_STCHG;
	}

	switch( GetHPGaugeDottoColor(wk->poke[pos].hp,wk->poke[pos].mhp,HPGAUGE_DOTTO_MAX) ){
	case HP_DOTTO_MAX:
		return POKEICON_ANM_HPMAX;
	case HP_DOTTO_GREEN:	// ��
		return POKEICON_ANM_HPGREEN;
	case HP_DOTTO_YELLOW:	// ��
		return POKEICON_ANM_HPYERROW;
	case HP_DOTTO_RED:		// ��
		return POKEICON_ANM_HPRED;
	}

	return POKEICON_ANM_DEATH;
}

//--------------------------------------------------------------------------------------------
/**
 * �Z���A�N�^�[�ǉ�
 *
 * @param	wk		���[�N
 * @param	pos		�ʒu
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeClactAdd( VSD_WORK * wk, u32 pos )
{
	u32	i;

	for( i=0; i<5; i++ ){
		wk->cap[pos*5+i] = VSD_ClactAdd( wk, &PokeCheck_ClactParam[pos*5+i][0] );
		CATS_ObjectPosSetCap(
			wk->cap[pos*5+i], PokeCheckObjPosTable[pos][i*2], PokeCheckObjPosTable[pos][i*2+1] );
	}
	if( wk->poke[pos].mons == 0 ){
		for( i=0; i<5; i++ ){
			CATS_ObjectEnableCap( wk->cap[pos*5+i], 0 );
		}
	}else{
		// �|�P�A�C�R��
		CATS_ObjectPaletteSetCap(
			wk->cap[pos*5+0],
			PokeIconPalNumGet(wk->poke[pos].mons,wk->poke[pos].form,wk->poke[pos].egg) );
		CATS_ObjectAnimeSeqSetCap(
			wk->cap[pos*5+0], VSD_PokeIconAnmGet( wk, pos ) );

		// �A�C�e���A�C�R��
		if( wk->poke[pos].item == 0 ){
			CATS_ObjectEnableCap( wk->cap[pos*5+1], 0 );
		}else if( ItemMailCheck( wk->poke[pos].item ) == TRUE ){
			CATS_ObjectAnimeSeqSetCap( wk->cap[pos*5+1], 1 );
		}else{
			CATS_ObjectAnimeSeqSetCap( wk->cap[pos*5+1], 0 );
		}
		// �J�X�^���{�[��
		if( wk->poke[pos].cb == 0 ){
			CATS_ObjectEnableCap( wk->cap[pos*5+2], 0 );
		}else{
			CATS_ObjectAnimeSeqSetCap( wk->cap[pos*5+2], 2 );
		}
		// ��Ԉُ�A�C�R��
		if( wk->poke[pos].st == ST_ICON_NONE ){
			CATS_ObjectEnableCap( wk->cap[pos*5+3], 0 );
		}else{
			CATS_ObjectAnimeSeqSetCap( wk->cap[pos*5+3], wk->poke[i].st );
		}
	}
}

//--------------------------------------------------------------------------------------------
/**
 * �Z���A�N�^�[�ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckObjMake( VSD_WORK * wk )
{
	TCATS_RESOURCE_NUM_LIST	crnl = {
		VS_CHR_ID_MV_MAX, VS_PAL_ID_MV_MAX, VS_CEL_ID_MV_MAX, VS_ANM_ID_MV_MAX, 0, 0
	};
	u32	i;

	VSD_ClactResManInit( wk, &crnl, VS_CLA_PLMAX+1 );

	VSD_PokeCheckPokeIconMake( wk );
	VSD_PokeCheckItemIconMake( wk );
	VSD_PokeCheckStIconMake( wk );
	VSD_PokeCheckBallMake( wk );

	for( i=0; i<6; i++ ){
		VSD_PokeClactAdd( wk, i );
	}

	VSD_PokeCheckObjMove( wk, MULTI_SCROLL_INIT );

	wk->cap_max = VS_CLA_PLMAX;
}

//--------------------------------------------------------------------------------------------
/**
 * �|�P�����A�C�R���̃��\�[�X�ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckPokeIconMake( VSD_WORK * wk )
{
	u32	i;
		
	ARCHANDLE* hdl;

	hdl  = ArchiveDataHandleOpen( ARC_POKEICON,  wk->dat->heap ); 

	CATS_LoadResourcePlttArcH(
		wk->csp, wk->crp, hdl,
		PokeIconPalArcIndexGet(), 0, 3, NNS_G2D_VRAM_TYPE_2DMAIN, VS_PAL_ID_POKE );

	CATS_LoadResourceCellArcH(
		wk->csp, wk->crp, hdl, PokeIconAnmCellArcIndexGet(), 0, VS_CEL_ID_POKE );

	CATS_LoadResourceCellAnmArcH(
		wk->csp, wk->crp, hdl, PokeIconAnmCellAnmArcIndexGet(), 0, VS_ANM_ID_POKE );

	for( i=0; i<6; i++ ){
		CATS_LoadResourceCharArcH(
			wk->csp, wk->crp, hdl,
			wk->poke[i].chr_arc, 0, NNS_G2D_VRAM_TYPE_2DMAIN, VS_CHR_ID_POKE1+i );
	}
	
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------------------------------------
/**
 * �A�C�e���A�C�R���̃��\�[�X�ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckItemIconMake( VSD_WORK * wk )
{
	
	ARCHANDLE* hdl;

	hdl  = ArchiveDataHandleOpen( ARC_PLIST_GRA,  wk->dat->heap ); 
	
	CATS_LoadResourceCharArcH(
		wk->csp, wk->crp, hdl,
		NARC_plist_gra_item_icon_NCGR, 0, NNS_G2D_VRAM_TYPE_2DMAIN, VS_CHR_ID_ITEM );

	CATS_LoadResourcePlttArcH(
		wk->csp, wk->crp, hdl,
		NARC_plist_gra_item_icon_NCLR, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, VS_PAL_ID_ITEM );

	CATS_LoadResourceCellArcH(
		wk->csp, wk->crp, hdl, NARC_plist_gra_item_icon_NCER, 0, VS_CEL_ID_ITEM );

	CATS_LoadResourceCellAnmArcH(
		wk->csp, wk->crp, hdl, NARC_plist_gra_item_icon_NANR, 0, VS_ANM_ID_ITEM );
		
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------------------------------------
/**
 * ��Ԉُ�A�C�R���̃��\�[�X�ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckStIconMake( VSD_WORK * wk )
{
	
	ARCHANDLE* hdl;

	hdl  = ArchiveDataHandleOpen( ARC_PSTATUS_GRA,  wk->dat->heap ); 
	
	CATS_LoadResourceCharArcH(
		wk->csp, wk->crp, hdl,
		NARC_pst_gra_p_st_ijou_NCGR, 0, NNS_G2D_VRAM_TYPE_2DMAIN, VS_CHR_ID_ST );

	CATS_LoadResourcePlttArcH(
		wk->csp, wk->crp, hdl,
		NARC_pst_gra_p_st_ijou_NCLR, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, VS_PAL_ID_ST );

	CATS_LoadResourceCellArcH(
		wk->csp, wk->crp, hdl, NARC_pst_gra_p_st_ijou_NCER, 0, VS_CEL_ID_ST );

	CATS_LoadResourceCellAnmArcH(
		wk->csp, wk->crp, hdl, NARC_pst_gra_p_st_ijou_NANR, 0, VS_ANM_ID_ST );

	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------------------------------------
/**
 * �{�[���̃��\�[�X�ݒ�
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBallMake( VSD_WORK * wk )
{

	ARCHANDLE* hdl;

	hdl  = ArchiveDataHandleOpen( ARC_PLIST_GRA,  wk->dat->heap ); 
	
	CATS_LoadResourceCharArcH(
		wk->csp, wk->crp, hdl,
		NARC_plist_gra_list_ball_NCGR, 0, NNS_G2D_VRAM_TYPE_2DMAIN, VS_CHR_ID_PLBALL );

	CATS_LoadResourcePlttArcH(
		wk->csp, wk->crp, hdl,
		NARC_plist_gra_list_obj_NCLR, 0, 2, NNS_G2D_VRAM_TYPE_2DMAIN, VS_PAL_ID_PLBALL );

	CATS_LoadResourceCellArcH(
		wk->csp, wk->crp, hdl, NARC_plist_gra_list_ball_NCER, 0, VS_CEL_ID_PLBALL );

	CATS_LoadResourceCellAnmArcH(
		wk->csp, wk->crp, hdl, NARC_plist_gra_list_ball_NANR, 0, VS_ANM_ID_PLBALL );
	
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------------------------------------
/**
 * OBJ�̈ړ�
 *
 * @param	wk		���[�N
 * @param	mv		�ړ��J�E���g
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckObjMove( VSD_WORK * wk, s16 mv )
{
	s32	i, j, start;

	if( wk->dat->type == VSD_TYPE_LEFT ){
		start = 3;
		mv *= 8;
	}else{
		start = 0;
		mv *= -8;
	}

	for( i=0; i<3; i++ ){
		for( j=0; j<5; j++ ){
			CATS_ObjectPosSetCap(
				wk->cap[(start+i)*5+j],
				PokeCheckObjPosTable[start+i][j*2]+mv,
				PokeCheckObjPosTable[start+i][j*2+1] );
		}
	}
}

//--------------------------------------------------------------------------------------------
/**
 * BMP�Z�b�g
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBmpAdd( VSD_WORK * wk )
{
	u32	i;

	for( i=0; i<VSD_BMP_MAX; i++ ){
		GF_BGL_BmpWinAddEx( wk->bgl, &wk->win[i], &PokePanelBmpData[i] );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * BMP�폜
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBmpExit( VSD_WORK * wk )
{
	u32	i;

	for( i=0; i<VSD_BMP_MAX; i++ ){
		GF_BGL_BmpWinDel( &wk->win[i] );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * �j�b�N�l�[���\��
 *
 * @param	wk		���[�N
 * @param	mman	���b�Z�[�W�}�l�[�W��
 * @param	nfont	8x8�t�H���g
 * @param	wset	���[�h�Z�b�g
 * @param	buf		������W�J�o�b�t�@
 * @param	pos		�ʒu
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckPutName(
				VSD_WORK * wk, MSGDATA_MANAGER * mman,
				NUMFONT * nfnt, WORDSET * wset, STRBUF * buf, u32 pos )
{
	POKEMON_PARAM * pp;
	GF_BGL_BMPWIN * win;
	STRBUF * str;

	win = &wk->win[pos*4+WIN_NAME1];

	// ���O
	if( pos < 3 ){
		pp = PokeParty_GetMemberPointer( wk->dat->pp[VSD_PARTY_LEFT1], pos );
	}else{
		pp = PokeParty_GetMemberPointer( wk->dat->pp[VSD_PARTY_LEFT2], pos-3 );
	}
	str = MSGMAN_AllocString( mman, mes_pokelist_01_09+pos );
	WORDSET_RegisterPokeNickName( wset, 0, PPPPointerGet(pp) );
	WORDSET_ExpandStr( wset, buf, str );
	STRBUF_Delete( str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, buf,
		PRM_NICKNAME_PX, PRM_NICKNAME_PY, MSG_NO_PUT, PCOL_N_WHITE, NULL );
	// ����
	if( wk->poke[pos].sex_put == 0 ){
		if( wk->poke[pos].sex == PARA_MALE ){
			MSGMAN_GetString( mman, mes_pokelist_01_28, buf );
			GF_STR_PrintColor(
				win, FONT_SYSTEM, buf,
				PRM_SEX_PX, PRM_SEX_PY, MSG_NO_PUT, PCOL_N_BLUE, NULL );
		}else if( wk->poke[pos].sex == PARA_FEMALE ){
			MSGMAN_GetString( mman, mes_pokelist_01_29, buf );
			GF_STR_PrintColor(
				win, FONT_SYSTEM, buf,
				PRM_SEX_PX, PRM_SEX_PY, MSG_NO_PUT, PCOL_N_RED, NULL );
		}
	}
}

//--------------------------------------------------------------------------------------------
/**
 * ���x���\��
 *
 * @param	wk		���[�N
 * @param	mman	���b�Z�[�W�}�l�[�W��
 * @param	nfont	8x8�t�H���g
 * @param	wset	���[�h�Z�b�g
 * @param	buf		������W�J�o�b�t�@
 * @param	pos		�ʒu
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckPutLv(
				VSD_WORK * wk, MSGDATA_MANAGER * mman,
				NUMFONT * nfnt, WORDSET * wset, STRBUF * buf, u32 pos )
{
	GF_BGL_BMPWIN * win;
	STRBUF * str;

	win = &wk->win[pos*4+WIN_LV1];

	// LV
	NUMFONT_WriteSet(
		nfnt, NUMFONT_MARK_LV, wk->poke[pos].lv, 3, NUMFONT_MODE_LEFT, win, STR_LV_PX, STR_LV_PY - 3 );
#if 0
	NUMFONT_WriteMark( nfnt, NUMFONT_MARK_LV, win, STR_LV_PX, STR_LV_PY );
	str = MSGMAN_AllocString( mman, mes_pokelist_01_03+pos );
	WORDSET_RegisterNumber(
		wset, 0, wk->poke[pos].lv, 3, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
	WORDSET_ExpandStr( wset, buf, str );
	STRBUF_Delete( str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, buf, NUMFONT_MARK_WIDTH, 0, MSG_NO_PUT, PCOL_N_WHITE, NULL );
#endif
}

//--------------------------------------------------------------------------------------------
/**
 * HP�\��
 *
 * @param	wk		���[�N
 * @param	mman	���b�Z�[�W�}�l�[�W��
 * @param	nfont	8x8�t�H���g
 * @param	wset	���[�h�Z�b�g
 * @param	buf		������W�J�o�b�t�@
 * @param	pos		�ʒu
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckPutHP(
				VSD_WORK * wk, MSGDATA_MANAGER * mman,
				NUMFONT * nfnt, WORDSET * wset, STRBUF * buf, u32 pos )
{
	GF_BGL_BMPWIN * win;
	STRBUF * str;
	u32	siz;
	int px, py;
	
	px = STR_SLASH_PX - STR_SLASH_PX;
	py = STR_SLASH_PY + 2;

	win = &wk->win[pos*4+WIN_HP1];

	NUMFONT_WriteNumber(
		nfnt, wk->poke[pos].hp, 3, NUMFONT_MODE_SPACE, win, px, py );
	NUMFONT_WriteMark(
		nfnt, NUMFONT_MARK_SLASH, win, px+NUMFONT_NUM_WIDTH*3, py );
	NUMFONT_WriteNumber(
		nfnt, wk->poke[pos].mhp, 3, NUMFONT_MODE_LEFT,
		win, px+(NUMFONT_NUM_WIDTH*3)+8, py );
#if 0
	// HP
	str = MSGMAN_AllocString( mman, mes_pokelist_01_21+pos );
	WORDSET_RegisterNumber(
		wset, 0, wk->poke[pos].hp, 3, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
	WORDSET_ExpandStr( wset, buf, str );
	STRBUF_Delete( str );
	siz = FontProc_GetPrintStrWidth( FONT_SYSTEM, buf, 0 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, buf, STR_SLASH_PX-siz, STR_SLASH_PY, MSG_NO_PUT, PCOL_N_WHITE, NULL );
	// �X���b�V��
	MSGMAN_GetString( mman, mes_pokelist_01_27, buf );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, buf, STR_SLASH_PX, STR_SLASH_PY, MSG_NO_PUT, PCOL_N_WHITE, NULL );
	siz = FontProc_GetPrintStrWidth( FONT_SYSTEM, buf, 0 );
	// MHP
	str = MSGMAN_AllocString( mman, mes_pokelist_01_15+pos );
	WORDSET_RegisterNumber(
		wset, 0, wk->poke[pos].mhp, 3, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
	WORDSET_ExpandStr( wset, buf, str );
	STRBUF_Delete( str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, buf, STR_SLASH_PX+siz, STR_SLASH_PY, MSG_NO_PUT, PCOL_N_WHITE, NULL );
#endif
}

//--------------------------------------------------------------------------------------------
/**
 * HP�Q�[�W�\��
 *
 * @param	wk		���[�N
 * @param	pos		�ʒu
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckPutGage( VSD_WORK * wk, u32 pos )
{
	GF_BGL_BMPWIN * win;
	u32	siz;

	win = &wk->win[pos*4+WIN_HPBAR1];

	// HP�Q�[�W
	siz = GetHPGaugeDottoColor( wk->poke[pos].hp, wk->poke[pos].mhp, HPGAUGE_DOTTO_MAX );
#ifdef OSP_VSDEMO_ON
	OS_Printf( "HP COLLOR : %d\n", siz );
#endif
	switch( siz ){
	case HP_DOTTO_NULL:
		break;
	case HP_DOTTO_MAX:		// ��
	case HP_DOTTO_GREEN:
		GF_BGL_PaletteSet(
			GF_BGL_FRAME1_M, &wk->pal[PALPOS_GREEN],
			2*2, (win->palnum*16+HPGAUGE_COL1)*2 );
		break;
	case HP_DOTTO_YELLOW:	// ��
		GF_BGL_PaletteSet(
			GF_BGL_FRAME1_M, &wk->pal[PALPOS_YELLOW],
			2*2, (win->palnum*16+HPGAUGE_COL1)*2 );
		break;
	case HP_DOTTO_RED:		// ��
		GF_BGL_PaletteSet(
			GF_BGL_FRAME1_M, &wk->pal[PALPOS_RED],
			2*2, (win->palnum*16+HPGAUGE_COL1)*2 );
		break;
	}

	siz = GetNumDotto( wk->poke[pos].hp, wk->poke[pos].mhp, HPGAUGE_DOTTO_MAX );
	GF_BGL_BmpWinFill( win, HPGAUGE_COL2, 0, 2, siz, 1 );
	GF_BGL_BmpWinFill( win, HPGAUGE_COL1, 0, 3, siz, 2 );
	GF_BGL_BmpWinFill( win, HPGAUGE_COL2, 0, 5, siz, 1 );
}

//--------------------------------------------------------------------------------------------
/**
 * BMP�\�����C��
 *
 * @param	wk		���[�N
 * @param	mman	���b�Z�[�W�}�l�[�W��
 * @param	nfont	8x8�t�H���g
 * @param	wset	���[�h�Z�b�g
 * @param	buf		������W�J�o�b�t�@
 * @param	pos		�ʒu
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBmpPutMain(
				VSD_WORK * wk, MSGDATA_MANAGER * mman,
				NUMFONT * nfnt, WORDSET * wset, STRBUF * buf, u32 pos )
{
	POKEMON_PARAM * pp;
	GF_BGL_BMPWIN * win;
	STRBUF * str;
	u32	siz;

	win = &wk->win[pos*4];

	GF_BGL_BmpWinDataFill( &win[WIN_NAME1], 0 );
	GF_BGL_BmpWinDataFill( &win[WIN_LV1], 0 );
	GF_BGL_BmpWinDataFill( &win[WIN_HP1], 0 );
	GF_BGL_BmpWinDataFill( &win[WIN_HPBAR1], 0 );

	VSD_PokeCheckPutName( wk, mman, nfnt, wset, buf, pos );
	VSD_PokeCheckPutLv( wk, mman, nfnt, wset, buf, pos );
	VSD_PokeCheckPutHP( wk, mman, nfnt, wset, buf, pos );
	VSD_PokeCheckPutGage( wk, pos );

	GF_BGL_BmpWinOnVReq( &win[WIN_NAME1] );
	GF_BGL_BmpWinOnVReq( &win[WIN_LV1] );
	GF_BGL_BmpWinOnVReq( &win[WIN_HP1] );
	GF_BGL_BmpWinOnVReq( &win[WIN_HPBAR1] );
}

//--------------------------------------------------------------------------------------------
/**
 * BMP�\��
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_PokeCheckBmpPut( VSD_WORK * wk )
{
	MSGDATA_MANAGER * mman;		// ���b�Z�[�W�f�[�^�}�l�[�W��
	NUMFONT * nfnt;				// 8x8�t�H���g
	WORDSET * wset;				// �P��Z�b�g
	STRBUF * buf;
	u32	i;

	mman = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_pokelist_dat, wk->dat->heap );
	nfnt = NUMFONT_Create( 15, 14, 0, wk->dat->heap );
	wset = WORDSET_Create( wk->dat->heap );
	buf  = STRBUF_Create( TMP_MSG_SIZ, wk->dat->heap );

	for( i=0; i<6; i++ ){
		if( wk->poke[i].mons == 0 ){ continue; }
		VSD_PokeCheckBmpPutMain( wk, mman, nfnt, wset, buf, i );
	}

	STRBUF_Delete( buf );
	MSGMAN_Delete( mman );
	NUMFONT_Delete( nfnt );
	WORDSET_Delete( wset );
}



//--------------------------------------------------------------------------------------------
/**
 * �`���m�[�g�f�[�^�쐬
 *
 * @param	dat		VS�f���f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VSD_FnoteMake( VS_DEMO_DATA * dat )
{
	void * buf;
	STRCODE * name1;
	STRCODE * name2;
	u8	sex1;
	u8	sex2;
	u8	result;
	u8	sio_id;

	if( dat->bp == NULL ){ return; }

	sio_id = CommGetCurrentID();	// �����̒ʐMID
	result = dat->result - 1;		// �`���m�[�g�̏��s�ɕϊ�

#ifdef OSP_VSDEMO_ON
	OS_Printf( "VSD_RESULT (1) : %d\n", dat->result );
	OS_Printf( "VSD_RESULT (2) : %d\n", result );
#endif

	// ���j�I��
	if( ZoneData_IsUnionRoom( dat->bp->zone_id ) == TRUE ){
		sex1  = TT_TrainerTypeSexGet( dat->bp->trainer_data[sio_id^1].tr_type );
		name1 = sys_AllocMemory( dat->heap, sizeof(STRCODE)*BUFLEN_PERSON_NAME );
		STRBUF_GetStringCode( dat->name[CommGetStandNo(sio_id^1)], name1, BUFLEN_PERSON_NAME );
		buf = FNOTE_SioUnionBattleDataMake( name1, sex1, result, dat->heap );
		sys_FreeMemoryEz( name1 );
	// �ʐM�ΐ�
	}else{
		switch( CommStateGetServiceNo() ){
		case COMM_MODE_BATTLE_SINGLE:	// 1vs1 �o�g��
			sex1  = TT_TrainerTypeSexGet( dat->bp->trainer_data[sio_id^1].tr_type );
			name1 = sys_AllocMemory( dat->heap, sizeof(STRCODE)*BUFLEN_PERSON_NAME );
			STRBUF_GetStringCode( dat->name[CommGetStandNo(sio_id^1)], name1, BUFLEN_PERSON_NAME );
			buf = FNOTE_SioSingleBtlDataMake( name1, sex1, result, dat->heap );
			sys_FreeMemoryEz( name1 );
			break;

		case COMM_MODE_BATTLE_DOUBLE:	// 1vs1 �_�u���o�g��
			sex1  = TT_TrainerTypeSexGet( dat->bp->trainer_data[sio_id^1].tr_type );
			name1 = sys_AllocMemory( dat->heap, sizeof(STRCODE)*BUFLEN_PERSON_NAME );
			STRBUF_GetStringCode( dat->name[CommGetStandNo(sio_id^1)], name1, BUFLEN_PERSON_NAME );
			buf = FNOTE_SioDoubleBtlDataMake( name1, sex1, result, dat->heap );
			sys_FreeMemoryEz( name1 );
			break;

		case COMM_MODE_BATTLE_MIX_1ON1:	// 1vs1 �~�b�N�X�o�g��
			sex1  = TT_TrainerTypeSexGet( dat->bp->trainer_data[sio_id^1].tr_type );
			name1 = sys_AllocMemory( dat->heap, sizeof(STRCODE)*BUFLEN_PERSON_NAME );
			STRBUF_GetStringCode( dat->name[CommGetStandNo(sio_id^1)], name1, BUFLEN_PERSON_NAME );
			buf = FNOTE_SioMixBtl1DataMake( name1, sex1, result, dat->heap );
			sys_FreeMemoryEz( name1 );
			break;

		case COMM_MODE_BATTLE_MULTI:	// 2vs2 �}���`�o�g��
			name1 = sys_AllocMemory( dat->heap, sizeof(STRCODE)*BUFLEN_PERSON_NAME );
			name2 = sys_AllocMemory( dat->heap, sizeof(STRCODE)*BUFLEN_PERSON_NAME );
//			if( sio_id == 0 || sio_id == 2 ){
			if( VSD_GetStandID() == TRUE ){
				sex1  = TT_TrainerTypeSexGet( dat->bp->trainer_data[1].tr_type );
				sex2  = TT_TrainerTypeSexGet( dat->bp->trainer_data[3].tr_type );
				STRBUF_GetStringCode( dat->name[1], name1, BUFLEN_PERSON_NAME );
				STRBUF_GetStringCode( dat->name[3], name2, BUFLEN_PERSON_NAME );
			}else{
				sex1  = TT_TrainerTypeSexGet( dat->bp->trainer_data[0].tr_type );
				sex2  = TT_TrainerTypeSexGet( dat->bp->trainer_data[2].tr_type );
				STRBUF_GetStringCode( dat->name[0], name1, BUFLEN_PERSON_NAME );
				STRBUF_GetStringCode( dat->name[2], name2, BUFLEN_PERSON_NAME );
			}
			buf = FNOTE_SioMultiBtlDataMake( name1, name2, sex1, sex2, result, dat->heap );
			sys_FreeMemoryEz( name1 );
			sys_FreeMemoryEz( name2 );
			break;

		default:
			return;
		}
	}

	FNOTE_DataSave( dat->bp->fnote_data, buf, FNOTE_TYPE_SIO );
}

//--------------------------------------------------------------
/**
 * @brief   ��b�E�B���h�E��`��
 *
 * @param   wk		���[�N
 */
//--------------------------------------------------------------
static void VSD_TalkWindowPut(VSD_WORK *wk)
{
	//�̈�N���A���E�B���h�E�`��
	GF_BGL_BmpWinFill( &wk->talk_win, FBMP_COL_WHITE, 0, 0,
					VS_DEMO_BMPWIN_TALK_SIZE_X * 8, VS_DEMO_BMPWIN_TALK_SIZE_Y * 8 );
	BmpTalkWinWrite( &wk->talk_win, WINDOW_TRANS_ON,
					VS_DEMO_TALKWIN_CGX_OFFSET, VS_DEMO_TALKWIN_PALNO );
}

//--------------------------------------------------------------
/**
 * @brief   �퓬�^��\���`�F�b�N
 *
 * @param   wk		���[�N
 *
 * @retval  TRUE:�^��\�B�@FALSE:�^��s��
 */
//--------------------------------------------------------------
static BOOL VSD_RecModeCheck(VSD_WORK *wk)
{
	if(wk->dat->bp->savedata == NULL || BattleRec_DataExistCheck() == FALSE){
		return FALSE;
	}
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���v�A�C�R������
 *
 * @param   wk		���[�N
 */
//--------------------------------------------------------------
static void VSD_TimeIconAdd(VSD_WORK *wk)
{
	if(wk->time_icon == NULL){
		wk->time_icon = TimeWaitIconAdd(&wk->talk_win, VS_DEMO_TALKWIN_CGX_OFFSET);
	}
}

//--------------------------------------------------------------
/**
 * @brief   ���v�A�C�R�����폜
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void VSD_TimeIconDelete(VSD_WORK *wk)
{
	if(wk->time_icon != NULL){
		TimeWaitIconDel(wk->time_icon);
		wk->time_icon = NULL;
	}
}















#ifdef PM_DEBUG
//============================================================================================
//============================================================================================
//============================================================================================
//	�f�o�b�O����
//============================================================================================
#include "msgdata/msg.naix"
#include "msgdata/msg_debug_nakahiro.h"


static void D_VSDemoVramSet(void);

PROC_RESULT D_VSDemoProc_Init00( PROC * proc, int * seq );
PROC_RESULT D_VSDemoProc_Init01( PROC * proc, int * seq );
PROC_RESULT D_VSDemoProc_Init10( PROC * proc, int * seq );
PROC_RESULT D_VSDemoProc_Init11( PROC * proc, int * seq );
PROC_RESULT D_VSDemoProc_Init20( PROC * proc, int * seq );
PROC_RESULT D_VSDemoProc_Init21( PROC * proc, int * seq );
PROC_RESULT D_VSDemoProc_Init22( PROC * proc, int * seq );
PROC_RESULT D_VsDemoProc_Main( PROC * proc, int * seq );
PROC_RESULT D_VsDemoProc_End( PROC * proc, int * seq );



static POKEMON_PARAM * vspp[12];
static STRBUF * vsstr[4];

//--------------------------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F������
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//--------------------------------------------------------------------------------------------
static PROC_RESULT D_VsDemoProc_Init( PROC * proc, int * seq, u8 mode, u8 type, u8 result )
{
	VS_DEMO_DATA * dat;
	u32	i;

	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_BAG, 0x80000 );

	dat = PROC_AllocWork( proc, sizeof(VS_DEMO_DATA), HEAPID_BAG );
	memset( dat, 0, sizeof(VS_DEMO_DATA) );

	dat->heap = HEAPID_BAG;
	dat->mode = mode;
	dat->type = type;
	dat->result = result;

	dat->bp = NULL;

	// �|�P�����쐬
	dat->pp[0] = PokeParty_AllocPartyWork( HEAPID_BAG );
	dat->pp[1] = PokeParty_AllocPartyWork( HEAPID_BAG );
	dat->pp[2] = PokeParty_AllocPartyWork( HEAPID_BAG );
	dat->pp[3] = PokeParty_AllocPartyWork( HEAPID_BAG );
	for( i=0; i<12; i++ ){
		vspp[i] = PokemonParam_AllocWork( HEAPID_BAG );
	}

	for( i=0; i<11; i++ ){
		if( i == 5 || i == 11 ){
			PokeParaSet( vspp[i], 0, 50, POW_RND, RND_SET, 0, ID_SET, 0 );
		}else{
			u16	item = 1;
			u8	cb = (u8)i+1;
			PokeParaSet( vspp[i], i+1, 50, POW_RND, RND_SET, 0, ID_SET, 0 );
			PokeParaPut( vspp[i], ID_PARA_item, &item );
			PokeParaPut( vspp[i], ID_PARA_cb_id, &cb );
		}
		PokeParty_Add( dat->pp[i/3], vspp[i] );
	}
/*
	PokeParaSet( vspp[0], 1, 50, POW_RND, RND_SET, 0, ID_SET, 0 );
	PokeParty_Add( dat->pp[0], vspp[0] );
	PokeParaSet( vspp[1], 3, 50, POW_RND, RND_SET, 0, ID_SET, 0 );
	PokeParty_Add( dat->pp[2], vspp[1] );
	PokeParaSet( vspp[2], 1, 50, POW_RND, RND_SET, 0, ID_SET, 0 );
	PokeParty_Add( dat->pp[1], vspp[2] );
	PokeParaSet( vspp[3], 3, 50, POW_RND, RND_SET, 0, ID_SET, 0 );
	PokeParty_Add( dat->pp[3], vspp[3] );
*/
	// ���O�쐬
	{
		MSGDATA_MANAGER * man;

		man = MSGMAN_Create(
				MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_debug_nakahiro_dat, HEAPID_BAG );
		vsstr[0] = MSGMAN_AllocString( man, msg_vsd_name00 );
		vsstr[1] = MSGMAN_AllocString( man, msg_vsd_name01 );
		vsstr[2] = MSGMAN_AllocString( man, msg_vsd_name02 );
		vsstr[3] = MSGMAN_AllocString( man, msg_vsd_name03 );

		dat->name[VSD_PARTY_LEFT1]  = vsstr[0];
		dat->name[VSD_PARTY_LEFT2]  = vsstr[1];
		dat->name[VSD_PARTY_RIGHT1] = vsstr[2];
		dat->name[VSD_PARTY_RIGHT2] = vsstr[3];

		MSGMAN_Delete( man );
	}

	VSDemo_TaskAdd( dat );

//	D_VSDemoVramSet();

	sys_VBlankFuncChange( NULL, NULL );	// VBlank�Z�b�g

	Snd_DataSetByScene( SND_SCENE_BATTLE, SEQ_BA_TRAIN, 1 );	// �o�g���ȍĐ�

	return PROC_RES_FINISH;
}

PROC_RESULT D_VSDemoProc_Init00( PROC * proc, int * seq )
{
	return D_VsDemoProc_Init( proc, seq, VSD_MODE_MULTI, VSD_TYPE_LEFT, 0 );
}
PROC_RESULT D_VSDemoProc_Init01( PROC * proc, int * seq )
{
	return D_VsDemoProc_Init( proc, seq, VSD_MODE_MULTI, VSD_TYPE_RIGHT, 0 );
}
PROC_RESULT D_VSDemoProc_Init10( PROC * proc, int * seq )
{
	return D_VsDemoProc_Init( proc, seq, VSD_MODE_IN, VSD_TYPE_NORMAL, 0 );
}
PROC_RESULT D_VSDemoProc_Init11( PROC * proc, int * seq )
{
	return D_VsDemoProc_Init( proc, seq, VSD_MODE_IN, VSD_TYPE_MULTI, 0 );
}
PROC_RESULT D_VSDemoProc_Init20( PROC * proc, int * seq )
{
	return D_VsDemoProc_Init( proc, seq, VSD_MODE_END, VSD_TYPE_NORMAL, VSD_RESULT_WIN );
}
PROC_RESULT D_VSDemoProc_Init21( PROC * proc, int * seq )
{
	return D_VsDemoProc_Init( proc, seq, VSD_MODE_END, VSD_TYPE_MULTI, VSD_RESULT_LOSE );
}
PROC_RESULT D_VSDemoProc_Init22( PROC * proc, int * seq )
{
	return D_VsDemoProc_Init( proc, seq, VSD_MODE_END, VSD_TYPE_NORMAL, VSD_RESULT_DRAW );
}



//--------------------------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F���C��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//--------------------------------------------------------------------------------------------
PROC_RESULT D_VsDemoProc_Main( PROC * proc, int * seq )
{
	VS_DEMO_DATA * dat  = PROC_GetWork( proc );

	if( dat->end_flg == 1 ){
		return PROC_RES_FINISH;
	}
	return PROC_RES_CONTINUE;
}

//--------------------------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F�I��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//--------------------------------------------------------------------------------------------
PROC_RESULT D_VsDemoProc_End( PROC * proc, int * seq )
{
	VS_DEMO_DATA * dat = PROC_GetWork( proc );
	u32	i;

	sys_FreeMemoryEz( dat->pp[0] );
	sys_FreeMemoryEz( dat->pp[1] );
	sys_FreeMemoryEz( dat->pp[2] );
	sys_FreeMemoryEz( dat->pp[3] );
	for( i=0; i<12; i++ ){
		sys_FreeMemoryEz( vspp[i] );
	}

	for( i=0; i<4; i++ ){
		if( vsstr[i] != NULL ){ STRBUF_Delete( vsstr[i] ); }
	}

	PROC_FreeWork( proc );				// ���[�N�J��

	sys_VBlankFuncChange( NULL, NULL );		// VBlank�Z�b�g

	sys_DeleteHeap( HEAPID_BAG );

	return PROC_RES_FINISH;
}


#endif

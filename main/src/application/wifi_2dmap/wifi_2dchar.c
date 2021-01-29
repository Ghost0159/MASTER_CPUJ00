//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_2dchar.c
 *	@brief		wifi�QD�L�����N�^�ǂݍ��݃V�X�e��
 *	@author		tomoya takahshi
 *	@data		2007.02.07
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "include/system/arc_util.h"
#include "include/system/clact_util.h"
#include "src/graphic/wifi2dchar.naix"
#include "src/graphic/wifi_unionobj.naix"
#include "src/field/fieldobj_code.h"

#define __WIFI_2DCHAR_H_GLOBAL
#include "application/wifi_2dmap/wifi_2dchar.h"

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

//-------------------------------------
///	�L�����N�^�[�C���f�b�N�X
enum {
	WF_2DC_HERO,
	WF_2DC_HEROINE,
	WF_2DC_BOY1,
	WF_2DC_BOY3,
	WF_2DC_MAN3,
	WF_2DC_BADMAN,
	WF_2DC_EXPLORE,
	WF_2DC_FIGHTER,
	WF_2DC_GORGGEOUSM,
	WF_2DC_MYSTERY,
	WF_2DC_GIRL1,
	WF_2DC_GIRL2,
	WF_2DC_WOMAN2,
	WF_2DC_WOMAN3,
	WF_2DC_IDOL,
	WF_2DC_LADY,
	WF_2DC_COWGIRL,
	WF_2DC_GORGGEOUSW,

	WF_2DC_BOY2,
	WF_2DC_GIRL3,
	WF_2DC_CAMPBOY,
	WF_2DC_PICNICGIRL,
	WF_2DC_BABYGIRL1,
	WF_2DC_MIDDLEMAN1,
	WF_2DC_MIDDLEWOMAN1,
	WF_2DC_WAITER,
	WF_2DC_WAITRESS,
	WF_2DC_MAN1,
	WF_2DC_WOMAN1,
	WF_2DC_CAMERAMAN,
	WF_2DC_REPORTER,
	WF_2DC_FARMER,
	WF_2DC_CYCLEM,
	WF_2DC_CYCLEW,
	WF_2DC_OLDMAN1,
	WF_2DC_MAN5,
	WF_2DC_WOMAN5,
	WF_2DC_BABYBOY1,
	WF_2DC_SPORTSMAN,
	WF_2DC_FISHING,
	WF_2DC_SEAMAN,
	WF_2DC_MOUNT,
	WF_2DC_MAN2,
	WF_2DC_BIGMAN,
	WF_2DC_ASSISTANTM,
	WF_2DC_GENTLEMAN,
	WF_2DC_WORKMAN,
	WF_2DC_CLOWN,
	WF_2DC_POLICEMAN,
	WF_2DC_AMBRELLA,
	WF_2DC_PIKACHU,
	WF_2DC_SEVEN1,
	WF_2DC_SEVEN2,
	WF_2DC_SEVEN3,
	WF_2DC_SEVEN4,
	WF_2DC_SEVEN5,
	WF_2DC_TOWERBOSS,

	WF_2DC_ACHAMO,
	WF_2DC_ARTIST,
	WF_2DC_ASSISTANTW,
	WF_2DC_BABY,
	WF_2DC_BAG,
	WF_2DC_BALLOON,
	WF_2DC_BEACHBOY,
	WF_2DC_BEACHGIRL,
	WF_2DC_BIGFOUR1,
	WF_2DC_BIGFOUR2,
	WF_2DC_BIGFOUR3,
	WF_2DC_BIGFOUR4,
	WF_2DC_BIRD,
	WF_2DC_BOY4,
	WF_2DC_CHAMPION,
	WF_2DC_DOCTOR,
	WF_2DC_DSBOY,
	WF_2DC_ENECO,
	WF_2DC_FREEZES,
	WF_2DC_GINGABOSS,
	WF_2DC_GINGAM,
	WF_2DC_GINGAW,
	WF_2DC_GIRL4,
	WF_2DC_GKANBU1,
	WF_2DC_GKANBU2,
	WF_2DC_GKANBU3,
	WF_2DC_KINOCOCO,
	WF_2DC_KODUCK,
	WF_2DC_KOIKING,
	WF_2DC_LEADER1,
	WF_2DC_LEADER2,
	WF_2DC_LEADER3,
	WF_2DC_LEADER4,
	WF_2DC_LEADER5,
	WF_2DC_LEADER6,
	WF_2DC_LEADER7,
	WF_2DC_LEADER8,
	WF_2DC_MAID,
	WF_2DC_MAMA,
	WF_2DC_MIDDLEMAN2,
	WF_2DC_MIDDLEWOMAN2,
	WF_2DC_MIKAN,
	WF_2DC_MIMITUTO,
	WF_2DC_MINUN,
	WF_2DC_MOSS,
	WF_2DC_OLDMAN2,
	WF_2DC_OLDWOMAN1,
	WF_2DC_OLDWOMAN2,
	WF_2DC_OOKIDO,
	WF_2DC_PATIRITUSU,
	WF_2DC_PCWOMAN1,
	WF_2DC_PCWOMAN2,
	WF_2DC_PCWOMAN3,
	WF_2DC_PIPPI,
	WF_2DC_POLE,
	WF_2DC_PRASLE,
	WF_2DC_PURIN,
	WF_2DC_RIVEL,
	WF_2DC_ROCK,
	WF_2DC_SHOPM1,
	WF_2DC_SHOPW1,
	WF_2DC_SKIERM,
	WF_2DC_SKIERW,
	WF_2DC_SPPOKE1,
	WF_2DC_SPPOKE10,
	WF_2DC_SPPOKE11,
	WF_2DC_SPPOKE12,
	WF_2DC_SPPOKE2,
	WF_2DC_SPPOKE3,
	WF_2DC_SPPOKE4,
	WF_2DC_SPPOKE5,
	WF_2DC_SPPOKE6,
	WF_2DC_SPPOKE7,
	WF_2DC_SPPOKE8,
	WF_2DC_SPPOKE9,
	WF_2DC_SUNGLASSES,
	WF_2DC_SWIMMERM,
	WF_2DC_SWIMMERW,
	WF_2DC_TREE,
	WF_2DC_VENTHOLE,
	WF_2DC_WANRIKY,
	WF_2DC_WOMAN6,
	
	// �{�f��
	WF_2DC_BRAINS1,
	WF_2DC_BRAINS2,
	WF_2DC_BRAINS3,
	WF_2DC_BRAINS4,
	WF_2DC_PRINCESS,
	WF_2DC_BFSM,
	WF_2DC_BFSW1,
	WF_2DC_BFSW2,
	WF_2DC_WIFISM,
	WF_2DC_WIFISW,
	WF_2DC_DPHERO,
	WF_2DC_DPHEROINE,

	// �f�[�^�Ƃ��đg�ݍ���ł��Ȃ�����
/*
	WF_2DC_ICPO,
	WF_2DC_GKANBU4,
	WF_2DC_NAETLE,
	WF_2DC_HAYASHIGAME,
	WF_2DC_DODAITOSE,
	WF_2DC_HIKOZARU,
	WF_2DC_MOUKAZARU,
	WF_2DC_GOUKAZARU,
	WF_2DC_POCHAMA,
	WF_2DC_POTTAISHI,
	WF_2DC_EMPERTE,
	WF_2DC_GUREGGRU,
	WF_2DC_GIRATINAORIGIN,
	WF_2DC_ROTOM,
	WF_2DC_ROTOMF,
	WF_2DC_ROTOMS,
	WF_2DC_ROTOMI,
	WF_2DC_ROTOMW,
	WF_2DC_ROTOMG,
	WF_2DC_DIRECTOR,
	WF_2DC_FIREBOX,
	WF_2DC_SKYBOX,
	WF_2DC_ICEBOX,
	WF_2DC_WATERBOX,
	WF_2DC_GRASSBOX,
	WF_2DC_GSET1,
	WF_2DC_GSET2,
//*/	
	
	WF_2DC_CHARNUM,
} ;
#define WF_2DC_RESM_OBJ_NUM	(WF_2DC_CHARNUM+1)	// ���\�[�X��	+1�͉A


// ���\�[�X�^�C�v
typedef enum {
	WF_2DC_RES_NML,	// �ʏ�
	WF_2DC_RES_UNI,	// ���j�I��
} WF_2DC_RESTYPE;

// �A�[�N�n���h���^�C�v
enum{
	WF_2DC_ARCHANDLE_NML,	// �ʏ�
	WF_2DC_ARCHANDLE_UNI,	// ���j�I��
	WF_2DC_ARCHANDLE_NUM,
};



//
#define WF_2DC_UNICHAR_NUM		(16)	// ���j�I���L�����N�^�̐�



#define WF_2DC_RESMAN_NUM	(CLACT_U_MULTI_RES)	// �쐬���郊�\�[�X�}�l�[�W����
#define WF_2DC_ANMRESMAN_OBJNUM	( (WF_2DC_MOVENUM*2)+WF_2DC_UNICHAR_NUM+1 )	// �Z���ƃA�j���̃��\�[�X�}�l�[�W���I�u�W�F�N�g��	+1�͉A
enum{
	WF_2DC_ANMRES_ANM_CELL,
	WF_2DC_ANMRES_ANM_ANM,
	WF_2DC_ANMRES_ANM_NOFLIP_CELL,
	WF_2DC_ANMRES_ANM_NOFLIP_ANM,
	WF_2DC_ANMRES_ANM_NUM,
};
static const u8 sc_WF_2DC_ANMRES_ANMCONTID[ WF_2DC_ANMRES_ANM_NUM ] = {
	0,0,
	1,1,
};
 
// �ʏ�L�����N�^�[
#define WF_2DC_ARC_CHARIDX	( NARC_wifi2dchar_pl_boy01_NCLR )	// �L�����N�^�O���t�B�b�N�̊J�n
#define WF_2DC_ARC_GETNCL(x)	(((x)*2)+WF_2DC_ARC_CHARIDX)	// �p���b�g�擾
#define WF_2DC_ARC_GETNCG(x)	(WF_2DC_ARC_GETNCL(x) + 1)		// �L�����N�^�擾

// ���j�I���L�����N�^�[
#define WF_2DC_ARC_UNICHARIDX	( NARC_wifi_unionobj_wf_match_top_trainer_boy1_NCGR )	// �L�����N�^�O���t�B�b�N�̊J�n
#define WF_2DC_ARC_GETUNINCL	(NARC_wifi_unionobj_wf_match_top_trainer_NCLR)	// �p���b�g�擾
#define WF_2DC_ARC_GETUNINCG(x)	(WF_2DC_ARC_UNICHARIDX + ((x) - WF_2DC_BOY1))		// �L�����N�^�擾
#define WF_2DC_UNIPLTT_NUM		( 8 )	// ���j�I���L�����N�^���g�p����p���b�g�{��
#define WF_2DC_ARC_UNIANMIDX	( NARC_wifi_unionobj_normal00_NCER )	// ���j�I���A�j���f�[�^
#define WF_2DC_ARC_GETUNICEL(x)	(WF_2DC_ARC_UNIANMIDX+((x)*2))	// �Z��dataid�Q�b�g
#define WF_2DC_ARC_GETUNIANM(x)	(WF_2DC_ARC_GETUNICEL(x)+1)		// �A�j��dataid�Q�b�g


// ���\�[�X�Ǘ�ID
#define WF_2DC_ARC_CONTANMID	( 0x100 )	// �A�j�����\�[�X�Ǘ�ID
#define WF_2DC_ARC_CONTSHADOWID	( 0x120 )	// �A���\�[�X�Ǘ�ID
#define WF_2DC_ARC_CONTCHARID	( 0x200 )	// �L�����N�^���\�[�X�Ǘ�ID
#define WF_2DC_ARC_CONTUNIONPLTTID	( 0x402 )	// ���j�I���p���b�g���\�[�X�Ǘ�ID
												// ���j�I���ŏo�Ă���L�����N�^�̊Ǘ�ID�̏ꏊ�Ȃ̂ő��v
#define WF_2DC_ARC_CONTUNIONANMID	(0x180)	// ���j�I���A�j�����\�[�X�Ǘ�ID

#define WF_2DC_ARC_TURNANMCHARSIZ			(0x800)	// �U������̂ݎ��̃L�����N�^�T�C�Y
#define WF_2DC_ARC_TURNANMCHARSIZ_NOFLIP	(0x800)	// �U������̂ݎ��̃L�����N�^�T�C�Y
#define WF_2DC_ARC_NORMALANMCHARSIZ			(0x1400)// �����{�U������̂ݎ��̃L�����N�^�T�C�Y
#define WF_2DC_ARC_NORMALANMCHARSIZ_NOTFLIP	(0x1800)// �����{�U������̂ݎ��̃L�����N�^�T�C�Y	�t���b�v�Ȃ��̃L�����̂Ƃ�


// �e�A�j���̃t���[����
#define WF_2DC_ANM_WAY_FRAME	(1)		// �����ς��A�j��
#define WF_2DC_ANM_WALK_FRAME	(WF_COMMON_WALK_FRAME)		// �����A�j��
#define WF_2DC_ANM_TURN_FRAME	(WF_COMMON_TURN_FRAME)		// �U������A�j��
#define WF_2DC_ANM_RUN_FRAME	(WF_COMMON_RUN_FRAME)		// ����A�j��
#define WF_2DC_ANM_WALLWALK_FRAME	(WF_COMMON_WALLWALK_FRAME)	// �Ǖ����A�j��
#define WF_2DC_ANM_SLOWWALK_FRAME	(WF_COMMON_SLOWWALK_FRAME)	// �Ǖ����A�j��
#define WF_2DC_ANM_HIGHWALK2_FRAME	(WF_COMMON_HIGHWALK2_FRAME)	// ���������A�j��
#define WF_2DC_ANM_HIGHWALK4_FRAME	(WF_COMMON_HIGHWALK4_FRAME)	// ���������A�j��
#define WF_2DC_ANM_ROTA_FRAME	(0xff)	// ��]�A�j��

// �A�j���̃V�[�P���X��
#define WF_2DC_ANM_ROTA			(4)
#define WF_2DC_ANM_WALK			(5)		// �����A�j���̃V�[�P���X��
#define WF_2DC_ANM_RUN			(9)		// ����A�j���̃V�[�P���X��
#define WF_2DC_ANM_FRAME		(2*FX32_ONE)	// 16 fure
#define WF_2DC_ANM_SLOWFRAME (FX32_ONE)
#define WF_2DC_ANM_HIGH2 (8*FX32_ONE)			// 
#define WF_2DC_ANM_HIGH4 (6*FX32_ONE)
	
// ���j�I���L�����N�^�e�[�u��
static const u8 WF_2DC_UnionChar[WF_2DC_UNICHAR_NUM] = {
	WF_2DC_BOY1,
	WF_2DC_BOY3,
	WF_2DC_MAN3,
	WF_2DC_BADMAN,
	WF_2DC_EXPLORE,
	WF_2DC_FIGHTER,
	WF_2DC_GORGGEOUSM,
	WF_2DC_MYSTERY,
	WF_2DC_GIRL1,
	WF_2DC_GIRL2,
	WF_2DC_WOMAN2,
	WF_2DC_WOMAN3,
	WF_2DC_IDOL,
	WF_2DC_LADY,
	WF_2DC_COWGIRL,
	WF_2DC_GORGGEOUSW,
};


// ����
#define WF_2DC_SHADOW_MAT_OFS_X		( 8 )
#define WF_2DC_SHADOW_MAT_OFS_Y		( 14 )

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�L�����N�^���\�[�X�f�[�^
//=====================================
typedef struct {
	WF_2DC_RESTYPE type;

	CLACT_U_RES_OBJ_PTR resobj[2];	//CG PL���\�[�X�I�u�W�F
	u32					drawtype;	// �]����
	WF_2DC_MOVETYPE		movetype;	// ����^�C�v
	CLACT_HEADER		header;		// �A�N�^�[�w�b�_�[
} WF_2DCRES;

/*
typedef struct {
	CLACT_U_RES_OBJ_PTR resobj[2];	//CG PL���\�[�X�I�u�W�F
	u32					drawtype;	// �]����
	WF_2DC_MOVETYPE		movetype;	// ����^�C�v
	CLACT_HEADER		header;		// �A�N�^�[�w�b�_�[
} WF_2DCRES;
//*/

//-------------------------------------
///	�A�j�����\�[�X�f�[�^
//=====================================
typedef struct {
	CLACT_U_RES_OBJ_PTR resobj[WF_2DC_ANMRES_ANM_NUM];	// CE AN���\�[�X�I�u�W�F
} WF_2DCANMRES;


//-------------------------------------
/// 2D�L�����N�^���[�N
//=====================================
typedef struct _WF_2DCWK {
	const WF_2DCRES* cp_res;	// ���\�[�X�f�[�^
	CLACT_WORK_PTR	p_clwk;		// �l��
	CLACT_WORK_PTR	p_shadow;	// �A
	WF_2DC_ANMTYPE	patanmtype;
	WF_COMMON_WAY	patanmway;
	s16				patanmframe;
	s16				pad_0;
} WF_2DCWK;

//-------------------------------------
///	�A���\�[�X
//=====================================
typedef struct {
	CLACT_U_RES_OBJ_PTR resobj[4];	//���\�[�X�I�u�W�F
	CLACT_HEADER		header;		// �A�N�^�[�w�b�_�[
} WF_2DCSH_RES;


//-------------------------------------
///	2D�L�����N�^�Ǘ��V�X�e��
//=====================================
typedef struct _WF_2DCSYS {
	CLACT_SET_PTR p_clset;
	PALETTE_FADE_PTR p_pfd;
	WF_2DCWK*		p_wk;
	u32 objnum;
	WF_2DCRES		res[ WF_2DC_CHARNUM ];
	WF_2DCANMRES	anmres[ WF_2DC_MOVENUM ];
	WF_2DCANMRES	unionres[ WF_2DC_UNICHAR_NUM ];
	WF_2DCSH_RES	shadowres;
	u32				shadow_pri;		// �A�̃\�t�g�D�揇�ʊJ�n�ʒu
	CLACT_U_RES_MANAGER_PTR p_res_man[ WF_2DC_RESMAN_NUM ];
	CLACT_U_RES_OBJ_PTR p_unionpltt;
	ARCHANDLE*	p_handle[WF_2DC_ARCHANDLE_NUM];
} WF_2DCSYS;


//-------------------------------------
///	�O���t�B�b�N�t�@�C���ƃL�����N�^�R�[�h�������N
//����f�[�^�̍\����
//=====================================
typedef struct {
	u16 code;
	u16 wf2dc_char;
} WF_2DC_CHARDATA;


//-----------------------------------------------------------------------------
/**
 *					�f�[�^��
 */
//-----------------------------------------------------------------------------

///	�O���t�B�b�N�t�@�C���ƃL�����N�^�R�[�h�������N
static const WF_2DC_CHARDATA FIELDOBJCODE_CharIdx[WF_2DC_CHARNUM] = {
	{HERO,		WF_2DC_HERO},
	{BOY1,		WF_2DC_BOY1},
	{BOY3,      WF_2DC_BOY3},
	{MAN3,      WF_2DC_MAN3},
	{BADMAN,    WF_2DC_BADMAN},
	{EXPLORE,   WF_2DC_EXPLORE},
	{FIGHTER,   WF_2DC_FIGHTER},
	{GORGGEOUSM,WF_2DC_GORGGEOUSM},
	{MYSTERY,   WF_2DC_MYSTERY},
	{HEROINE,	WF_2DC_HEROINE},
	{GIRL1,     WF_2DC_GIRL1},
	{GIRL2,     WF_2DC_GIRL2},
	{WOMAN2,    WF_2DC_WOMAN2},
	{WOMAN3,    WF_2DC_WOMAN3},
	{IDOL,      WF_2DC_IDOL},
	{LADY,      WF_2DC_LADY},
	{COWGIRL,   WF_2DC_COWGIRL},
	{GORGGEOUSW,WF_2DC_GORGGEOUSW},
	{BOY2,			WF_2DC_BOY2},
	{GIRL3,			WF_2DC_GIRL3},
	{CAMPBOY,		WF_2DC_CAMPBOY},
	{PICNICGIRL,	WF_2DC_PICNICGIRL},
	{BABYGIRL1,		WF_2DC_BABYGIRL1},
	{MIDDLEMAN1,	WF_2DC_MIDDLEMAN1},
	{MIDDLEWOMAN1,	WF_2DC_MIDDLEWOMAN1},
	{WAITER,		WF_2DC_WAITER},
	{WAITRESS,		WF_2DC_WAITRESS},
	{MAN1,			WF_2DC_MAN1},
	{WOMAN1,		WF_2DC_WOMAN1},
	{CAMERAMAN,		WF_2DC_CAMERAMAN},
	{REPORTER,		WF_2DC_REPORTER},
	{FARMER,		WF_2DC_FARMER},
	{CYCLEM,		WF_2DC_CYCLEM},
	{CYCLEW,		WF_2DC_CYCLEW},
	{OLDMAN1,		WF_2DC_OLDMAN1},
	{MAN5,			WF_2DC_MAN5},
	{WOMAN5,		WF_2DC_WOMAN5},
	{BABYBOY1,		WF_2DC_BABYBOY1},
	{SPORTSMAN,		WF_2DC_SPORTSMAN},
	{FISHING,		WF_2DC_FISHING},
	{SEAMAN,		WF_2DC_SEAMAN},
	{MOUNT,			WF_2DC_MOUNT},
	{MAN2,			WF_2DC_MAN2},
	{BIGMAN,		WF_2DC_BIGMAN},
	{ASSISTANTM,	WF_2DC_ASSISTANTM},
	{GENTLEMAN,		WF_2DC_GENTLEMAN},
	{WORKMAN,		WF_2DC_WORKMAN},
	{CLOWN,			WF_2DC_CLOWN},
	{POLICEMAN,		WF_2DC_POLICEMAN},
	{AMBRELLA,		WF_2DC_AMBRELLA},
	{PIKACHU,		WF_2DC_PIKACHU},
	{SEVEN1,		WF_2DC_SEVEN1},
	{SEVEN2,		WF_2DC_SEVEN2},
	{SEVEN3,		WF_2DC_SEVEN3},
	{SEVEN4,		WF_2DC_SEVEN4},
	{SEVEN5,		WF_2DC_SEVEN5},
	{TOWERBOSS,		WF_2DC_TOWERBOSS},
	{ACHAMO,		WF_2DC_ACHAMO},
	{ARTIST,		WF_2DC_ARTIST},
	{ASSISTANTW,	WF_2DC_ASSISTANTW},
	{BABY,			WF_2DC_BABY},
	{BAG,			WF_2DC_BAG},
	{BALLOON,		WF_2DC_BALLOON},
	{BEACHBOY,		WF_2DC_BEACHBOY},
	{BEACHGIRL,		WF_2DC_BEACHGIRL},
	{BIGFOUR1,		WF_2DC_BIGFOUR1},
	{BIGFOUR2,		WF_2DC_BIGFOUR2},
	{BIGFOUR3,		WF_2DC_BIGFOUR3},
	{BIGFOUR4,		WF_2DC_BIGFOUR4},
	{BIRD,			WF_2DC_BIRD},
	{BOY4,			WF_2DC_BOY4},
	{CHAMPION,		WF_2DC_CHAMPION},
	{DOCTOR,		WF_2DC_DOCTOR},
	{DSBOY,			WF_2DC_DSBOY},
	{ENECO,			WF_2DC_ENECO},
	{FREEZES,		WF_2DC_FREEZES},
	{GINGABOSS,		WF_2DC_GINGABOSS},
	{GINGAM,		WF_2DC_GINGAM},
	{GINGAW,		WF_2DC_GINGAW},
	{GIRL4,			WF_2DC_GIRL4},
	{GKANBU1,		WF_2DC_GKANBU1},
	{GKANBU2,		WF_2DC_GKANBU2},
	{GKANBU3,		WF_2DC_GKANBU3},
	{KINOCOCO,		WF_2DC_KINOCOCO},
	{KODUCK,		WF_2DC_KODUCK},
	{KOIKING,		WF_2DC_KOIKING},
	{LEADER1,		WF_2DC_LEADER1},
	{LEADER2,		WF_2DC_LEADER2},
	{LEADER3,		WF_2DC_LEADER3},
	{LEADER4,		WF_2DC_LEADER4},
	{LEADER5,		WF_2DC_LEADER5},
	{LEADER6,		WF_2DC_LEADER6},
	{LEADER7,		WF_2DC_LEADER7},
	{LEADER8,		WF_2DC_LEADER8},
	{MAID,			WF_2DC_MAID},
	{MAMA,			WF_2DC_MAMA},
	{MIDDLEMAN2,	WF_2DC_MIDDLEMAN2},
	{MIDDLEWOMAN2,	WF_2DC_MIDDLEWOMAN2},
	{MIKAN,			WF_2DC_MIKAN},
	{MIMITUTO,		WF_2DC_MIMITUTO},
	{MINUN,			WF_2DC_MINUN},
	{MOSS,			WF_2DC_MOSS},
	{OLDMAN2,		WF_2DC_OLDMAN2},
	{OLDWOMAN1,		WF_2DC_OLDWOMAN1},
	{OLDWOMAN2,		WF_2DC_OLDWOMAN2},
	{OOKIDO,		WF_2DC_OOKIDO},
	{PATIRITUSU,	WF_2DC_PATIRITUSU},
	{PCWOMAN1,		WF_2DC_PCWOMAN1},
	{PCWOMAN2,		WF_2DC_PCWOMAN2},
	{PCWOMAN3,		WF_2DC_PCWOMAN3},
	{PIPPI,			WF_2DC_PIPPI},
	{POLE,			WF_2DC_POLE},
	{PRASLE,		WF_2DC_PRASLE},
	{PURIN,			WF_2DC_PURIN},
	{RIVEL,			WF_2DC_RIVEL},
	{ROCK,			WF_2DC_ROCK},
	{SHOPM1,		WF_2DC_SHOPM1},
	{SHOPW1,		WF_2DC_SHOPW1},
	{SKIERM,		WF_2DC_SKIERM},
	{SKIERW,		WF_2DC_SKIERW},
	{SPPOKE1,		WF_2DC_SPPOKE1},
	{SPPOKE10,		WF_2DC_SPPOKE10},
	{SPPOKE11,		WF_2DC_SPPOKE11},
	{SPPOKE12,		WF_2DC_SPPOKE12},
	{SPPOKE2,		WF_2DC_SPPOKE2},
	{SPPOKE3,		WF_2DC_SPPOKE3},
	{SPPOKE4,		WF_2DC_SPPOKE4},
	{SPPOKE5,		WF_2DC_SPPOKE5},
	{SPPOKE6,		WF_2DC_SPPOKE6},
	{SPPOKE7,		WF_2DC_SPPOKE7},
	{SPPOKE8,		WF_2DC_SPPOKE8},
	{SPPOKE9,		WF_2DC_SPPOKE9},
	{SUNGLASSES,	WF_2DC_SUNGLASSES},
	{SWIMMERM,		WF_2DC_SWIMMERM},
	{SWIMMERW,		WF_2DC_SWIMMERW},
	{TREE,			WF_2DC_TREE},
	{VENTHOLE,		WF_2DC_VENTHOLE},
	{WANRIKY,		WF_2DC_WANRIKY},
	{WOMAN6,		WF_2DC_WOMAN6},
	{BRAINS1,		WF_2DC_BRAINS1},
	{BRAINS2,		WF_2DC_BRAINS2},
	{BRAINS3,		WF_2DC_BRAINS3},
	{BRAINS4,		WF_2DC_BRAINS4},
	{PRINCESS,		WF_2DC_PRINCESS},
	{BFSM,			WF_2DC_BFSM},
	{BFSW1,			WF_2DC_BFSW1},
	{BFSW2,			WF_2DC_BFSW2},
	{WIFISM,		WF_2DC_WIFISM},
	{WIFISW,		WF_2DC_WIFISW},
	{DPHERO,		WF_2DC_DPHERO},		// DP��l���j
	{DPHEROINE,		WF_2DC_DPHEROINE},	// DP��l����

/*
	// �f�[�^�Ƃ��đg�ݍ���ł��Ȃ�����
	{ICPO,			WF_2DC_ICPO},
	{GKANBU4,		WF_2DC_GKANBU4},
	{NAETLE,		WF_2DC_NAETLE},
	{HAYASHIGAME,	WF_2DC_HAYASHIGAME},
	{DODAITOSE,		WF_2DC_DODAITOSE},
	{HIKOZARU,		WF_2DC_HIKOZARU},
	{MOUKAZARU,		WF_2DC_MOUKAZARU},
	{GOUKAZARU,		WF_2DC_GOUKAZARU},
	{POCHAMA,		WF_2DC_POCHAMA},
	{POTTAISHI,		WF_2DC_POTTAISHI},
	{EMPERTE,		WF_2DC_EMPERTE},
	{GUREGGRU,		WF_2DC_GUREGGRU},
	{GIRATINAORIGIN,WF_2DC_GIRATINAORIGIN},
	{ROTOM,			WF_2DC_ROTOM},
	{ROTOMF,		WF_2DC_ROTOMF},
	{ROTOMS,		WF_2DC_ROTOMS},
	{ROTOMI,		WF_2DC_ROTOMI},
	{ROTOMW,		WF_2DC_ROTOMW},
	{ROTOMG,		WF_2DC_ROTOMG},
	{DIRECTOR,		WF_2DC_DIRECTOR},
	{FIREBOX,		WF_2DC_FIREBOX},
	{SKYBOX,		WF_2DC_SKYBOX},
	{ICEBOX,		WF_2DC_ICEBOX},
	{WATERBOX,		WF_2DC_WATERBOX},
	{GRASSBOX,		WF_2DC_GRASSBOX},
	{GSET1,			WF_2DC_GSET1},
	{GSET2,			WF_2DC_GSET2},
	*/
};

/// �e�A�j���̏I���t���[����
static const u8 WF_2DC_AnmFrame[WF_2DC_ANMNUM] = {
	WF_2DC_ANM_WAY_FRAME, 
	WF_2DC_ANM_ROTA_FRAME, 
	WF_2DC_ANM_WALK_FRAME,
	WF_2DC_ANM_TURN_FRAME,
	WF_2DC_ANM_RUN_FRAME,
	WF_2DC_ANM_WALLWALK_FRAME,
	WF_2DC_ANM_SLOWWALK_FRAME,
	WF_2DC_ANM_HIGHWALK2_FRAME,
	WF_2DC_ANM_HIGHWALK4_FRAME,
	0,
};

//----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�p���b�g�t�F�[�h�V�X�e���Ƀp���b�g�J���[��ݒ�
//=====================================
static void WF_2DC_PFDPalSet( WF_2DCSYS* p_sys, CLACT_U_RES_OBJ_PTR p_pal, u32 num );

//-------------------------------------
/// �A�j�����\�[�X�֘A
//=====================================
static void WF_2DC_AnmResLoad( WF_2DCSYS* p_sys, WF_2DC_MOVETYPE movetype, u32 heap );
static void WF_2DC_AnmResDel( WF_2DCSYS* p_sys, WF_2DC_MOVETYPE movetype );
static BOOL WF_2DC_AnmResCheck( const WF_2DCSYS* cp_sys, WF_2DC_MOVETYPE movetype );
static u32 WF_2DC_CharNoGet( u32 view_type );
static BOOL WF_2DC_AnmModeLinkCheck( u32 view_type, WF_2DC_MOVETYPE movetype );
static u32 WF_2DC_AnmResContIdGet( WF_2DC_MOVETYPE movetype, u32 res_type, BOOL flip );

//-------------------------------------
/// �L�����E�p���b�g���\�[�X�֘A
//=====================================
static void WF_2DC_CharResLoad( WF_2DCSYS* p_sys, u32 char_no, u32 draw_type, WF_2DC_MOVETYPE movetype, u32 heap, WF_2DC_RESTYPE restype );
static void WF_2DC_CharResLoadNml( WF_2DCSYS* p_sys, u32 char_no, u32 draw_type, WF_2DC_MOVETYPE movetype, u32 heap );
static void WF_2DC_CharResLoadUni( WF_2DCSYS* p_sys, u32 char_no, u32 draw_type, WF_2DC_MOVETYPE movetype, u32 heap );
static void WF_2DC_CharResDel( WF_2DCSYS* p_sys, u32 char_no );
static void WF_2DC_CharResDelNml( WF_2DCSYS* p_sys, u32 char_no );
static void WF_2DC_CharResDelUni( WF_2DCSYS* p_sys, u32 char_no );
static BOOL WF_2DC_CharResCheck( const WF_2DCSYS* cp_sys, u32 char_no );
static u32 WF_2DC_CharResDrawTypeGet( const WF_2DCRES* cp_res );
static WF_2DC_MOVETYPE WF_2DC_CharResMoveTypeGet( const WF_2DCRES* cp_res );


//-------------------------------------
///	�A���\�[�X�֘A
//=====================================
static void WF_2DC_ShResLoad( WF_2DCSYS* p_sys, u32 draw_type, u32 heap );
static void WF_2DC_ShResDel( WF_2DCSYS* p_sys );
static BOOL WF_2DC_ShResCheck( const WF_2DCSH_RES* cp_shadowres );



//-------------------------------------
/// ���j�I�����\�[�X�֘A
//=====================================
static void WF_2DC_UniCharPlttResLoad( WF_2DCSYS* p_sys, u32 draw_type, u32 heap );
static void WF_2DC_UniCharPlttResDel( WF_2DCSYS* p_sys );
static void WF_2DC_UniCharAnmResLoad( WF_2DCSYS* p_sys, u32 heap );
static void WF_2DC_UniCharAnmResDel( WF_2DCSYS* p_sys );

//-------------------------------------
/// �I�u�W�F���[�N����֘A
//=====================================
static WF_2DCWK* WF_2DC_WkCleanGet( WF_2DCSYS* p_sys );
static BOOL WF_2DC_WkCleanCheck( const WF_2DCWK* cp_wk );
static void WF_2DC_WkPatAnmUpdate( WF_2DCWK* p_wk );


//-------------------------------------
/// �A�j���Ǘ��֘A
//=====================================
static void WF_2DC_WkPatAnmWayInit( WF_2DCWK* p_wk, WF_COMMON_WAY way );
static void WF_2DC_WkPatAnmRotaInit( WF_2DCWK* p_wk, WF_COMMON_WAY way );
static void WF_2DC_WkPatAnmWalkInit( WF_2DCWK* p_wk, WF_COMMON_WAY way );
static void WF_2DC_WkPatAnmTurnInit( WF_2DCWK* p_wk, WF_COMMON_WAY way );
static void WF_2DC_WkPatAnmRunInit( WF_2DCWK* p_wk, WF_COMMON_WAY way );
static void WF_2DC_WkPatAnmWallWalkInit( WF_2DCWK* p_wk, WF_COMMON_WAY way );
static void WF_2DC_WkPatAnmSlowWalkInit( WF_2DCWK* p_wk, WF_COMMON_WAY way );
static void WF_2DC_WkPatAnmHighWalk2Init( WF_2DCWK* p_wk, WF_COMMON_WAY way );
static void WF_2DC_WkPatAnmHighWalk4Init( WF_2DCWK* p_wk, WF_COMMON_WAY way );

static void WF_2DC_WkPatAnmRotaMain( WF_2DCWK* p_wk );
static void WF_2DC_WkPatAnmWalkMain( WF_2DCWK* p_wk );
static void WF_2DC_WkPatAnmTurnMain( WF_2DCWK* p_wk );
static void WF_2DC_WkPatAnmRunMain( WF_2DCWK* p_wk );
static void WF_2DC_WkPatAnmWallWalkMain( WF_2DCWK* p_wk );
static void WF_2DC_WkPatAnmSlowWalkMain( WF_2DCWK* p_wk );
static void WF_2DC_WkPatAnmHighWalk2Main( WF_2DCWK* p_wk );
static void WF_2DC_WkPatAnmHighWalk4Main( WF_2DCWK* p_wk );

//----------------------------------------------------------------------------
/**
 *	@brief	WIFI�@2D�`��V�X�e���쐬
 *
 *	@param	p_clset		�g�p����A�N�^�[�Z�b�g
 *	@param	pfd			�p���b�g�t�F�[�h�|�C���^
 *	@param	objnum		�I�u�W�F�N�g��
 *	@param	heap		�q�[�v
 *
 *	@return	�쐬�����V�X�e���|�C���^
 */
//-----------------------------------------------------------------------------
WF_2DCSYS* WF_2DC_SysInit( CLACT_SET_PTR p_clset, PALETTE_FADE_PTR p_pfd, u32 objnum, u32 heap )
{
	WF_2DCSYS* p_sys;
	int i;

	// �������m��
	p_sys = sys_AllocMemory( heap, sizeof(WF_2DCSYS) );
	memset( p_sys, 0, sizeof(WF_2DCSYS) );

	// �A�N�^�[�Z�b�g�͏ォ����炤
	p_sys->p_clset = p_clset;

	// �p���b�g�t�F�[�h�V�X�e��
	p_sys->p_pfd = p_pfd;

	// �A�[�J�C�u�n���h���I�[�v��
	p_sys->p_handle[WF_2DC_ARCHANDLE_NML] = ArchiveDataHandleOpen( ARC_WIFI2DCHAR, heap );
	p_sys->p_handle[WF_2DC_ARCHANDLE_UNI] = ArchiveDataHandleOpen( ARC_WIFIUNIONCHAR, heap );

	// ���\�[�X�}�l�[�W��������
	for( i=0; i<2; i++ ){	// CG�APL�́A�L�����N�^��
		p_sys->p_res_man[i] = CLACT_U_ResManagerInit( WF_2DC_RESM_OBJ_NUM, i, heap );
	}
	for( i=0; i<2; i++ ){	// CE,AN�̓A�j���^�C�v��
		p_sys->p_res_man[i+2] = CLACT_U_ResManagerInit( WF_2DC_ANMRESMAN_OBJNUM, i+2, heap );
	}

	// �A�j���[�V�������\�[�X�ǂݍ���
	for( i=0; i<WF_2DC_MOVENUM; i++ ){
		WF_2DC_AnmResLoad( p_sys, i, heap );
	}

	// �A�N�^�[���[�N�쐬
	p_sys->objnum = objnum;
	p_sys->p_wk = sys_AllocMemory( heap, sizeof(WF_2DCWK)*p_sys->objnum );
	memset( p_sys->p_wk, 0, sizeof(WF_2DCWK)*p_sys->objnum );
 
	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WIFI�@2D�`��V�X�e���j��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WF_2DC_SysExit( WF_2DCSYS* p_sys )
{
	int i;
	
	// ���[�N�j��
	for( i=0; i<p_sys->objnum; i++ ){
		if( WF_2DC_WkCleanCheck( &p_sys->p_wk[i] ) == FALSE ){
			WF_2DC_WkDel( &p_sys->p_wk[i] );
		} 
	}
	
	// �A�j���[�V�������\�[�X�j��
	for( i=0; i<WF_2DC_MOVENUM; i++ ){
		WF_2DC_AnmResDel( p_sys, i );
	}

	// �L�����N�^���\�[�X�j��
	WF_2DC_AllResDel( p_sys );

	// �A�[�J�C�u�n���h���N���[�Y
	ArchiveDataHandleClose( p_sys->p_handle[WF_2DC_ARCHANDLE_NML] );
	ArchiveDataHandleClose( p_sys->p_handle[WF_2DC_ARCHANDLE_UNI] );
	
	// ���\�[�X�}�l�[�W���j��
	for( i=0; i<WF_2DC_RESMAN_NUM; i++ ){
		CLACT_U_ResManagerDelete( p_sys->p_res_man[i] );
	}

	// ���[�N�j��
	sys_FreeMemoryEz( p_sys->p_wk );
	sys_FreeMemoryEz( p_sys );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���\�[�X�o�^
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	view_type		�����ځiFIELDOBJCODE�j
 *	@param	draw_type		�]����
 *	@param	movetype		����^�C�v
 *	@param	heap			�q�[�v
 *
 *	draw_type
	main�ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAIN
	sub�ɓo�^ �FNNS_G2D_VRAM_TYPE_2DSUB
	�����ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAX
 */
//-----------------------------------------------------------------------------
void WF_2DC_ResSet( WF_2DCSYS* p_sys, u32 view_type, u32 draw_type, WF_2DC_MOVETYPE movetype, u32 heap )
{
	u32 char_no;
	BOOL result;

	// �ǂݍ��݉\���`�F�b�N
	result = WF_2DC_AnmModeLinkCheck( view_type, movetype );
	GF_ASSERT( result );

	// �����ڂƐ��ʂ���L�����N�^�[�ԍ��擾
	char_no = WF_2DC_CharNoGet( view_type );
	
	// �ǂݍ���
	WF_2DC_CharResLoad( p_sys, char_no, draw_type, movetype, heap, WF_2DC_RES_NML );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^���\�[�X�j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	view_type	������FIELDOBJCODE
 *
 */
//-----------------------------------------------------------------------------
void WF_2DC_ResDel( WF_2DCSYS* p_sys, u32 view_type )
{
	u32 char_no;

	char_no = WF_2DC_CharNoGet( view_type );

	// ���\�[�X�j��
	WF_2DC_CharResDel( p_sys, char_no );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ǂݍ���ł��邩�`�F�b�N
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	view_type	�����ځiFIELDOBJCODE�j
 *
 *	@retval	TRUE	�ǂݍ���ł���
 *	@retval	FALSE	�ǂݍ���łȂ�
 */
//-----------------------------------------------------------------------------
BOOL WF_2DC_ResCheck( const WF_2DCSYS* cp_sys, u32 view_type )
{
	u32 char_no;

	// �����ڂƐ��ʂ���L�����N�^�[�ԍ��擾
	char_no = WF_2DC_CharNoGet( view_type );
	return WF_2DC_CharResCheck( cp_sys, char_no );	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�S���\�[�X�̔j��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WF_2DC_AllResDel( WF_2DCSYS* p_sys )
{
	int i;

	// ���j�I�����\�[�X�̔j��
	if( p_sys->p_unionpltt != NULL ){
		WF_2DC_UnionResDel( p_sys );
	}
	
	// ���̑��̃��\�[�X�͂�
	for( i=0; i<WF_2DC_CHARNUM; i++ ){
		if( WF_2DC_CharResCheck( p_sys, i ) == TRUE ){
			// ���\�[�X�j��
			WF_2DC_CharResDel( p_sys, i );
		}
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�I���L�����N�^�̓o�^
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	draw_type		�]����
 *	@param	movetype		����^�C�v
 *	@param	heap			�q�[�v
 *
 *	draw_type
	main�ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAIN
	sub�ɓo�^ �FNNS_G2D_VRAM_TYPE_2DSUB
	�����ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAX
 */
//-----------------------------------------------------------------------------
void WF_2DC_UnionResSet( WF_2DCSYS* p_sys, u32 draw_type, WF_2DC_MOVETYPE movetype, u32 heap )
{
	int i;

	// ����\���`�F�b�N
	// ��l���ȊO�́A����ȊO�󂯕t���Ă��Ȃ�
	GF_ASSERT( movetype != WF_2DC_MOVERUN );


	// ���j�I���A�j�����\�[�X�Ǎ���
	WF_2DC_UniCharPlttResLoad( p_sys, draw_type, heap );
	
	// �A�j�����\�[�X�o�^
	WF_2DC_UniCharAnmResLoad( p_sys, heap );

	for( i=0; i<WF_2DC_UNICHAR_NUM; i++ ){
		// �ǂݍ���
		WF_2DC_CharResLoad( p_sys, WF_2DC_UnionChar[i], draw_type, movetype, heap, WF_2DC_RES_UNI );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�I���L�����N�^�̃��\�[�X�͂�
 *
 *	@param	p_sys			�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WF_2DC_UnionResDel( WF_2DCSYS* p_sys )
{
	int i;

	// ���j�I���p���b�g�j��
	WF_2DC_UniCharPlttResDel( p_sys );

	// �A�j�����\�[�X�j��
	WF_2DC_UniCharAnmResDel( p_sys );

	for( i=0; i<WF_2DC_UNICHAR_NUM; i++ ){
		// �L�����N�^�͂�
		WF_2DC_CharResDel( p_sys, WF_2DC_UnionChar[i] );
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A�̃��\�[�X��ǂݍ���
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	draw_type		�]����
 *	@param	shadow_pri		�A�̕\���D�揇��
 *	@param	heap			�q�[�vID
 *
 *	draw_type
	main�ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAIN
	sub�ɓo�^ �FNNS_G2D_VRAM_TYPE_2DSUB
	�����ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAX
 */
//-----------------------------------------------------------------------------
void WF_2DC_ShadowResSet( WF_2DCSYS* p_sys, u32 draw_type, u32 shadow_pri, u32 heap )
{
	WF_2DC_ShResLoad( p_sys, draw_type, heap );
	p_sys->shadow_pri = shadow_pri;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�̃��\�[�X��j������
 *	
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WF_2DC_ShadowResDel( WF_2DCSYS* p_sys )
{
	WF_2DC_ShResDel( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^�A�N�^�[�o�^
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_data		�o�^�f�[�^
 *	@param	view_type	FIELDOBJCODE
 *	@param	movetype	����^�C�v
 *	@param	heap		�q�[�v
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
WF_2DCWK* WF_2DC_WkAdd( WF_2DCSYS* p_sys, const WF_2DC_WKDATA* cp_data, u32 view_type, u32 heap )
{
	WF_2DCWK* p_wk;
	CLACT_ADD add;
	u32 char_no;
	
	// ��̃��[�N�擾
	p_wk = WF_2DC_WkCleanGet( p_sys );

	// �L�����N�^�i���o�[�擾
	char_no = WF_2DC_CharNoGet( view_type );

	// �ǂݍ��ݍς݃`�F�b�N
	GF_ASSERT( WF_2DC_CharResCheck( p_sys, char_no ) == TRUE );
	
	memset( &add, 0, sizeof(CLACT_ADD) );
	add.ClActSet = p_sys->p_clset;
	add.ClActHeader = &p_sys->res[ char_no ].header;
	add.mat.x	= cp_data->x << FX32_SHIFT;
	add.mat.y	= cp_data->y << FX32_SHIFT;
	add.sca.x	= FX32_ONE;
	add.sca.y	= FX32_ONE;
	add.sca.z	= FX32_ONE;
	add.pri		= cp_data->pri;
	add.heap	= heap;

	// �\���G���A
	if( (p_sys->res[ char_no ].drawtype == NNS_G2D_VRAM_TYPE_MAX) || 
		(p_sys->res[ char_no ].drawtype == NNS_G2D_VRAM_TYPE_2DMAIN) ){
		add.DrawArea = NNS_G2D_VRAM_TYPE_2DMAIN;
	}else{
		add.DrawArea = NNS_G2D_VRAM_TYPE_2DSUB;
	}
	
	// �A�N�^�[�o�^
	p_wk->p_clwk = CLACT_Add( &add );
	CLACT_BGPriorityChg( p_wk->p_clwk, cp_data->bgpri );

	// ���샂�[�h�ɂ�菉���A�j����ύX
	if( p_sys->res[ char_no ].movetype == WF_2DC_MOVENORMAL ){
		// ��������
		CLACT_AnmChg( p_wk->p_clwk, WF_2DC_ANM_WALK+WF_COMMON_BOTTOM );
	}else{
		// ������
		CLACT_AnmChg( p_wk->p_clwk, WF_COMMON_BOTTOM );
	}

	// �A�j���f�[�^������
	p_wk->patanmtype = WF_2DC_ANMNONE;
	p_wk->patanmframe = 0;
	p_wk->patanmway = WF_COMMON_BOTTOM;

	// ���\�[�X�ۑ�
	p_wk->cp_res = &p_sys->res[ char_no ];

	// �A���[�N�̍쐬
	if( WF_2DC_ShResCheck( &p_sys->shadowres ) ){

		memset( &add, 0, sizeof(CLACT_ADD) );
		add.ClActSet = p_sys->p_clset;
		add.ClActHeader = &p_sys->shadowres.header;
		add.mat.x	= cp_data->x << FX32_SHIFT;
		add.mat.y	= cp_data->y << FX32_SHIFT;
		add.sca.x	= FX32_ONE;
		add.sca.y	= FX32_ONE;
		add.sca.z	= FX32_ONE;
		add.pri		= p_sys->shadow_pri;
		add.heap	= heap;

		// �\���G���A
		if( (p_sys->res[ char_no ].drawtype == NNS_G2D_VRAM_TYPE_MAX) || 
			(p_sys->res[ char_no ].drawtype == NNS_G2D_VRAM_TYPE_2DMAIN) ){
			add.DrawArea = NNS_G2D_VRAM_TYPE_2DMAIN;
		}else{
			add.DrawArea = NNS_G2D_VRAM_TYPE_2DSUB;
		}

		p_wk->p_shadow = CLACT_Add( &add );

		CLACT_BGPriorityChg( p_wk->p_shadow, cp_data->bgpri );
	}else{
		p_wk->p_shadow = NULL;
	}

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�N�^�[�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WF_2DC_WkDel( WF_2DCWK* p_wk )
{
	if( p_wk->p_shadow != NULL ){
		CLACT_Delete( p_wk->p_shadow );
	}
	CLACT_Delete( p_wk->p_clwk );
	memset( p_wk, 0, sizeof(WF_2DCWK) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[���[�N�|�C���^�̎擾
 *
 *	@param	p_wk	���[�N
 *	
 *	@return	�Z���A�N�^�[���[�N
 */
//-----------------------------------------------------------------------------
CLACT_WORK_PTR WF_2DC_WkClWkGet( WF_2DCWK* p_wk )
{
	return p_wk->p_clwk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[���[�N�|�C���^�̎擾
 *
 *	@param	p_wk	���[�N
 *	
 *	@return	�Z���A�N�^�[���[�N
 */
//-----------------------------------------------------------------------------
CONST_CLACT_WORK_PTR WF_2DC_WkConstClWkGet( const WF_2DCWK* cp_wk )
{
	return cp_wk->p_clwk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�̐ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	x		�����W
 *	@param	y		�����W
 */
//-----------------------------------------------------------------------------
void WF_2DC_WkMatrixSet( WF_2DCWK* p_wk, s16 x, s16 y )
{
	VecFx32	mat;

	mat.x = x<<FX32_SHIFT;
	mat.y = y<<FX32_SHIFT;
	mat.z = 0;
	CLACT_SetMatrix( p_wk->p_clwk, &mat );

	if( p_wk->p_shadow ){
		mat.x += WF_2DC_SHADOW_MAT_OFS_X << FX32_SHIFT;
		mat.y += WF_2DC_SHADOW_MAT_OFS_Y << FX32_SHIFT;
		CLACT_SetMatrix( p_wk->p_shadow, &mat );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�擾
 *
 *	@param	p_wk	���[�N
 *	@param	x_y		�t���O
 *
 *	x_y
	WF_2DC_GET_X,
	WF_2DC_GET_Y
 *
 *
 *	@return	�t���O�̍��W�̒l
 */
//-----------------------------------------------------------------------------
s16 WF_2DC_WkMatrixGet( WF_2DCWK* p_wk, WF_2DC_MAT x_y )
{
	const VecFx32* cp_mat;

	cp_mat = CLACT_GetMatrix( p_wk->p_clwk );

	if( x_y == WF_2DC_GET_X ){
		return cp_mat->x >> FX32_SHIFT;
	}
	return cp_mat->y >> FX32_SHIFT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���D�揇�ʂ�ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	pri		�\���D�揇��
 */
//-----------------------------------------------------------------------------
void WF_2DC_WkDrawPriSet( WF_2DCWK* p_wk, u16 pri )
{
	CLACT_DrawPriorityChg( p_wk->p_clwk, pri );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���D�揇�ʂ��擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�\���D�揇��
 */
//-----------------------------------------------------------------------------
u16 WF_2DC_WkDrawPriGet( const WF_2DCWK* cp_wk )
{
	return CLACT_DrawPriorityGet( cp_wk->p_clwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V������i�߂�
 *
 *	@param	p_wk	���[�N
 *	@param	frame	�t���[��
 */
//-----------------------------------------------------------------------------
void WF_2DC_WkAnmAddFrame( WF_2DCWK* p_wk, fx32 frame )
{
	CLACT_AnmFrameChg( p_wk->p_clwk, frame );
}

//----------------------------------------------------------------------------
/**
 *	@brie	�\����\���ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	flag	�t���O
 */
//-----------------------------------------------------------------------------
void WF_2DC_WkDrawFlagSet( WF_2DCWK* p_wk, BOOL flag )
{
	CLACT_SetDrawFlag( p_wk->p_clwk, flag );
	if( p_wk->p_shadow ){
		CLACT_SetDrawFlag( p_wk->p_shadow, flag );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\����\���擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�\��
 *	@retval	FALSE	��\��
 */
//-----------------------------------------------------------------------------
BOOL WF_2DC_WkDrawFlagGet( const WF_2DCWK* cp_wk )
{
	return CLACT_GetDrawFlag( cp_wk->p_clwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	����^�C�v���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	����^�C�v
 */
//-----------------------------------------------------------------------------
WF_2DC_MOVETYPE WF_2DC_WkMoveTypeGet( const WF_2DCWK* cp_wk )
{
	return cp_wk->cp_res->movetype;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V�����^�C�v���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�A�j���[�V�����^�C�v
 */
//-----------------------------------------------------------------------------
WF_2DC_ANMTYPE WF_2DC_WkAnmTypeGet( const WF_2DCWK* cp_wk )
{
	return cp_wk->patanmtype;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V�����t���[���ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	frame		�t���[��
 */
//-----------------------------------------------------------------------------
void WF_2DC_WkAnmFrameSet( WF_2DCWK* p_wk, u16 frame )
{
	CLACT_AnmFrameSet( p_wk->p_clwk, frame );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V�����t���[���擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	�t���[���C���f�b�N�X
 */
//-----------------------------------------------------------------------------
u16 WF_2DC_WkAnmFrameGet( const WF_2DCWK* cp_wk )
{
	return CLACT_AnmFrameGet( cp_wk->p_clwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�̈ʒu�𑀍삷��
 *
 *	@param	p_wk	���[�N
 *	@param	x		�����W
 *	@param	y		�����W
 */
//-----------------------------------------------------------------------------
void WF_2DC_WkShadowMatrixSet( WF_2DCWK* p_wk, s16 x, s16 y )
{
	VecFx32	mat;
	if( p_wk->p_shadow != NULL ){

		mat.x = x+WF_2DC_SHADOW_MAT_OFS_X<<FX32_SHIFT;
		mat.y = y+WF_2DC_SHADOW_MAT_OFS_Y<<FX32_SHIFT;
		mat.z = 0;
		CLACT_SetMatrix( p_wk->p_shadow, &mat );
	}
}




//----------------------------------------------------------------------------
/**
 *	@brief	Pattern�A�j���J�n
 *
 *	@param	p_wk		���[�N
 *	@param	anmtype		�A�j���^�C�v
 *	@param	anmway		�A�j������
 */
//-----------------------------------------------------------------------------
void WF_2DC_WkPatAnmStart( WF_2DCWK* p_wk, WF_2DC_ANMTYPE anmtype, WF_COMMON_WAY anmway )
{
	static void (* const pFunc[])( WF_2DCWK*, WF_COMMON_WAY ) = {
		WF_2DC_WkPatAnmWayInit,
		WF_2DC_WkPatAnmRotaInit,
		WF_2DC_WkPatAnmWalkInit,
		WF_2DC_WkPatAnmTurnInit,
		WF_2DC_WkPatAnmRunInit,
		WF_2DC_WkPatAnmWallWalkInit,
		WF_2DC_WkPatAnmSlowWalkInit,
		WF_2DC_WkPatAnmHighWalk2Init,
		WF_2DC_WkPatAnmHighWalk4Init,
		NULL
	};

	GF_ASSERT( anmtype < WF_2DC_ANMNUM );
	GF_ASSERT( anmway < WF_COMMON_WAYNUM );

	p_wk->patanmtype = anmtype;
	p_wk->patanmframe = 0;
	
	if( pFunc[ anmtype ] != NULL ){
		pFunc[ anmtype ]( p_wk, anmway );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	Pattern�A�j���I���`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL WF_2DC_WkPatAnmEndCheck( const WF_2DCWK* cp_wk )
{
	if( WF_2DC_AnmFrame[ cp_wk->patanmtype ] == 0xff ){	// ���[�v�ݒ�
		return FALSE;
	}

	//�I���`�F�b�N
	if( WF_2DC_AnmFrame[ cp_wk->patanmtype ] <= cp_wk->patanmframe ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	Pattern�A�j����i�߂�
 *
 *	@param	p_wk		���[�N
 *	@param	speed		�X�s�[�h
 */
//-----------------------------------------------------------------------------
void WF_2DC_WkPatAnmAddFrame( WF_2DCWK* p_wk )
{
	BOOL updata = FALSE;
	
	if( WF_2DC_AnmFrame[ p_wk->patanmtype ] == 0xff ){	// ���[�v�ݒ�
		updata = TRUE;	
	}else{	
	
		if( WF_2DC_AnmFrame[ p_wk->patanmtype ] >= p_wk->patanmframe + 1 ){
			updata = TRUE;
		}
	}

	if( updata == TRUE ){
		WF_2DC_WkPatAnmUpdate( p_wk );	// �A�j���X�V
		p_wk->patanmframe ++;
	}
}


//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V�������\�[�X�ǂݍ���
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	movetype		����^�C�v
 *	@param	heap			�q�[�v
 */
//-----------------------------------------------------------------------------
static void WF_2DC_AnmResLoad( WF_2DCSYS* p_sys, WF_2DC_MOVETYPE movetype, u32 heap )
{
	int i;
	u32 res_type;
	u32 anm_contid;
	static const u32 sc_WF_2DC_ANMRES_ANM_IDX[ WF_2DC_MOVENUM ][ WF_2DC_ANMRES_ANM_NUM ] = {
		{ // WF_2DC_MOVERUN
			NARC_wifi2dchar_run_NCER,
			NARC_wifi2dchar_run_NANR,
			NARC_wifi2dchar_run_NCER,
			NARC_wifi2dchar_run_NANR,
		},
		{ // WF_2DC_MOVENORMAL
			NARC_wifi2dchar_normal_NCER,
			NARC_wifi2dchar_normal_NANR,
			NARC_wifi2dchar_normal01_NCER,
			NARC_wifi2dchar_normal01_NANR,
		},
		{ // WF_2DC_MOVETURN
			NARC_wifi2dchar_npc_NCER,
			NARC_wifi2dchar_npc_NANR,
			NARC_wifi2dchar_npc01_NCER,
			NARC_wifi2dchar_npc01_NANR,
		},
	};


	GF_ASSERT( p_sys->anmres[ movetype ].resobj[0] == NULL );
	
	for( i=0; i<WF_2DC_ANMRES_ANM_NUM; i++ ){

		res_type	= CLACT_U_CELL_RES+(i%2);
		anm_contid	= WF_2DC_ARC_CONTANMID + (movetype * 2) + sc_WF_2DC_ANMRES_ANMCONTID[i];
		
		p_sys->anmres[movetype].resobj[i] = 
			CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
				p_sys->p_res_man[res_type], p_sys->p_handle[WF_2DC_ARCHANDLE_NML],
				sc_WF_2DC_ANMRES_ANM_IDX[movetype][i], FALSE,
				anm_contid, res_type, heap );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V�������\�[�X�j��
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	movetype		����^�C�v
 */
//-----------------------------------------------------------------------------
static void WF_2DC_AnmResDel( WF_2DCSYS* p_sys, WF_2DC_MOVETYPE movetype )
{
	int i;
	u32 res_type;
	
	GF_ASSERT( p_sys->anmres[ movetype ].resobj[0] != NULL );
	
	for( i=0; i<WF_2DC_ANMRES_ANM_NUM; i++ ){
		res_type = CLACT_U_CELL_RES+(i%2);

		CLACT_U_ResManagerResDelete( p_sys->p_res_man[res_type],
					p_sys->anmres[movetype].resobj[i] );
		p_sys->anmres[ movetype ].resobj[i] = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V�������\�[�X�@�ǂݍ��ݍς݂��`�F�b�N
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	movetype	����^�C�v
 *
 *	@retval	TRUE	�ǂݍ���ł���
 *	@retval	FALSE	�ǂݍ���łȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WF_2DC_AnmResCheck( const WF_2DCSYS* cp_sys, WF_2DC_MOVETYPE movetype )
{
	if( cp_sys->anmres[ movetype ].resobj[0] == NULL ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ڂ���L�����N�^NO���擾����
 *
 *	@param	view_type	�����ځiFIELDOBJCODE�j
 *
 *	@return	�L�����N�^NO
 */
//-----------------------------------------------------------------------------
static u32 WF_2DC_CharNoGet( u32 view_type )
{
	int i;

	for( i=0; i<WF_2DC_CHARNUM; i++ ){
		if( view_type == FIELDOBJCODE_CharIdx[i].code ){
			return FIELDOBJCODE_CharIdx[i].wf2dc_char;
		}
	}
	GF_ASSERT( 0 );
	return 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�w�肳�ꂽ�l���ɓ���^�C�v���w��ł��邩�`�F�b�N
 *
 *	@param	view_type		������
 *	@param	movetype		����^�C�v
 *
 *	@retval	TRUE	�w��ł���
 *	@retval	FALSE	�w��ł��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WF_2DC_AnmModeLinkCheck( u32 view_type, WF_2DC_MOVETYPE movetype )
{
	if( movetype == WF_2DC_MOVERUN ){
		if( (view_type == HERO) || (view_type == HEROINE) ||
			(view_type == DPHERO) || (view_type == DPHEROINE) ){
			return TRUE;
		}else{
			return FALSE;
		}
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���\�[�X�̊Ǘ�ID���擾����
 *
 *	@param	movetype		����^�C�v
 *	@param	res_type		���\�[�X�^�C�v
 *	@param	flip			�L�����N�^�̃t���b�v�̗L��
 *
 *	@return	���\�[�X�Ǘ�ID
 */
//-----------------------------------------------------------------------------
u32 WF_2DC_AnmResContIdGet( WF_2DC_MOVETYPE movetype, u32 res_type, BOOL flip )
{
	u32 idx;
	
	GF_ASSERT( (res_type == CLACT_U_CELL_RES) || (res_type == CLACT_U_CELLANM_RES) );
	
	idx = res_type-CLACT_U_CELL_RES;
	if( flip == FALSE ){
		idx += WF_2DC_ANMRES_ANM_NOFLIP_CELL;
	}
	
	return (WF_2DC_ARC_CONTANMID + (movetype * 2) + sc_WF_2DC_ANMRES_ANMCONTID[idx]);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^���\�[�X�ǂݍ���
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	char_no		�L�����N�^�ԍ�
 *	@param	draw_type	�]����
 *	@param	movetype	����^�C�v
 *	@param	heap		�q�[�v
 *	@param	restype		���\�[�X�^�C�v
 */
//-----------------------------------------------------------------------------
static void WF_2DC_CharResLoad( WF_2DCSYS* p_sys, u32 char_no, u32 draw_type, WF_2DC_MOVETYPE movetype, u32 heap, WF_2DC_RESTYPE restype )
{
	// �ǂݍ��ݍς݂łȂ����Ƃ��`�F�b�N
	GF_ASSERT( p_sys->res[ char_no ].resobj[0] == NULL );

	if( restype == WF_2DC_RES_NML ){
		WF_2DC_CharResLoadNml( p_sys, char_no, draw_type, movetype, heap );
	}else{
		WF_2DC_CharResLoadUni( p_sys, char_no, draw_type, movetype, heap );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^�[���\�[�X�ǂݍ��ݒʏ펞
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	char_no		�L�����N�^�ԍ�
 *	@param	draw_type	�]����
 *	@param	movetype	����^�C�v
 *	@param	heap		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WF_2DC_CharResLoadNml( WF_2DCSYS* p_sys, u32 char_no, u32 draw_type, WF_2DC_MOVETYPE movetype, u32 heap )
{
	NNSG2dCharacterData* p_char;
	BOOL result;
	BOOL flip;	// �t���b�v���g�p����L�����N�^��
	u32 cell_contid, anm_contid;

//	OS_Printf( "char_no %d\n", char_no );

	// �ǂݍ���
	p_sys->res[ char_no ].resobj[ 0 ] = 
		CLACT_U_ResManagerResAddArcChar_ArcHandle(
				p_sys->p_res_man[ 0 ], p_sys->p_handle[WF_2DC_ARCHANDLE_NML],
				WF_2DC_ARC_GETNCG( char_no ),
				FALSE, WF_2DC_ARC_CONTCHARID+char_no, draw_type, heap );

	p_sys->res[ char_no ].resobj[ 1 ] = 
		CLACT_U_ResManagerResAddArcPltt_ArcHandle(
				p_sys->p_res_man[ 1 ], p_sys->p_handle[WF_2DC_ARCHANDLE_NML],
				WF_2DC_ARC_GETNCL( char_no ),
				FALSE, WF_2DC_ARC_CONTCHARID+char_no, draw_type, 1, heap );

//	OS_TPrintf( "pltt contid=0x%x  ncl=%d\n", WF_2DC_ARC_CONTCHARID+char_no, WF_2DC_ARC_GETNCL( char_no ) );

	//  �L�����N�^�f�[�^�擾
	p_char = CLACT_U_ResManagerGetResObjResChar( p_sys->res[ char_no ].resobj[ 0 ] );

	// �t���b�v���g�p����L�����N�^���T�C�Y�����Ē��ׂ�
	if( p_char->szByte == WF_2DC_ARC_NORMALANMCHARSIZ_NOTFLIP ){
		// �t���b�v�Ȃ��̃L�����N�^�T�C�Y
		flip = FALSE;
	}else{
		// ���̑��̏ꍇ�̓t���b�v���g�p���Ă���
		flip = TRUE;	
	}

	// �]���T�C�Y����
	if( movetype == WF_2DC_MOVETURN ){
		// �L�����N�^�T�C�Y���팸�ł���
		if( flip ){	// �t���b�v�̂���Ȃ��ŃT�C�Y���ς��
			p_char->szByte = WF_2DC_ARC_TURNANMCHARSIZ;
		}else{
			p_char->szByte = WF_2DC_ARC_TURNANMCHARSIZ_NOFLIP;
		}
	}else if( movetype == WF_2DC_MOVENORMAL ){
		// �L�����N�^�T�C�Y���팸�ł���
		if( flip ){	// �t���b�v�̂���Ȃ��ŃT�C�Y���ς��
			p_char->szByte = WF_2DC_ARC_NORMALANMCHARSIZ;
		}else{
			p_char->szByte = WF_2DC_ARC_NORMALANMCHARSIZ_NOTFLIP;
		}
	}
	
	// �]��
	result = CLACT_U_CharManagerSetCharModeAdjustAreaCont( p_sys->res[ char_no ].resobj[0] );
	GF_ASSERT( result );
	result = CLACT_U_PlttManagerSetCleanArea( p_sys->res[ char_no ].resobj[1] );
	GF_ASSERT( result );

	// �p���b�g�t�F�[�h�ݒ�
	if( p_sys->p_pfd ){
		WF_2DC_PFDPalSet( p_sys, p_sys->res[ char_no ].resobj[1], 1 );
	}

	// ���\�[�X�����j��
	CLACT_U_ResManagerResOnlyDelete( p_sys->res[ char_no ].resobj[0] );
	CLACT_U_ResManagerResOnlyDelete( p_sys->res[ char_no ].resobj[1] );

	// �Ǘ��p�����[�^�ݒ�
	p_sys->res[ char_no ].drawtype = draw_type;
	p_sys->res[ char_no ].movetype = movetype;
	p_sys->res[ char_no ].type = WF_2DC_RES_NML;

	// �Z���A�N�^�[�w�b�_�[�쐬
	cell_contid	= WF_2DC_AnmResContIdGet( movetype, CLACT_U_CELL_RES, flip );
	anm_contid	= WF_2DC_AnmResContIdGet( movetype, CLACT_U_CELLANM_RES, flip );
	CLACT_U_MakeHeader( &p_sys->res[ char_no ].header, 
			WF_2DC_ARC_CONTCHARID+char_no, WF_2DC_ARC_CONTCHARID+char_no,
			cell_contid, anm_contid,
			CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,0,0,
			p_sys->p_res_man[0], p_sys->p_res_man[1], 
			p_sys->p_res_man[2], p_sys->p_res_man[3],
			NULL, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^�[���\�[�X�ǂݍ���	���j�I����
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	char_no		�L�����N�^�ԍ�
 *	@param	draw_type	�]����
 *	@param	movetype	����^�C�v
 *	@param	heap		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WF_2DC_CharResLoadUni( WF_2DCSYS* p_sys, u32 char_no, u32 draw_type, WF_2DC_MOVETYPE movetype, u32 heap )
{
	NNSG2dCharacterData* p_char;
	BOOL result;
	u32 union_idx;

	// ���j�I���L�����N�^�Ƃ��Ẵi���o�[���擾
	union_idx = char_no - WF_2DC_BOY1;

//	OS_Printf( "char_no %d\n", char_no );

	// �ǂݍ���
	p_sys->res[ char_no ].resobj[ 0 ] = 
		CLACT_U_ResManagerResAddArcChar_ArcHandle(
				p_sys->p_res_man[ 0 ], p_sys->p_handle[WF_2DC_ARCHANDLE_UNI],
				WF_2DC_ARC_GETUNINCG( char_no ),
				FALSE, WF_2DC_ARC_CONTCHARID+char_no, draw_type, heap );


	// �]���T�C�Y����
	if( movetype == WF_2DC_MOVETURN ){
		// �L�����N�^�T�C�Y���팸�ł���
		p_char = CLACT_U_ResManagerGetResObjResChar( p_sys->res[ char_no ].resobj[ 0 ] );
		p_char->szByte = WF_2DC_ARC_TURNANMCHARSIZ;
	}else if( movetype == WF_2DC_MOVENORMAL ){
		// �L�����N�^�T�C�Y���팸�ł���
		p_char = CLACT_U_ResManagerGetResObjResChar( p_sys->res[ char_no ].resobj[ 0 ] );
		p_char->szByte = WF_2DC_ARC_NORMALANMCHARSIZ;
	}
	
	// �]��
	result = CLACT_U_CharManagerSetCharModeAdjustAreaCont( p_sys->res[ char_no ].resobj[0] );
	GF_ASSERT( result );

	// ���\�[�X�����j��
	CLACT_U_ResManagerResOnlyDelete( p_sys->res[ char_no ].resobj[0] );

	// �p���b�g�f�[�^�����ɂ�NULL�����Ă���
	p_sys->res[ char_no ].resobj[1] = NULL;

	// �Ǘ��p�����[�^�ݒ�
	p_sys->res[ char_no ].drawtype = draw_type;
	p_sys->res[ char_no ].movetype = movetype;
	p_sys->res[ char_no ].type = WF_2DC_RES_UNI;

	// �Z���A�N�^�[�w�b�_�[�쐬
	CLACT_U_MakeHeader( &p_sys->res[ char_no ].header, 
			WF_2DC_ARC_CONTCHARID+char_no, WF_2DC_ARC_CONTUNIONPLTTID,
			WF_2DC_ARC_CONTUNIONANMID+union_idx, 
			WF_2DC_ARC_CONTUNIONANMID+union_idx,
			CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,0,0,
			p_sys->p_res_man[0], p_sys->p_res_man[1], 
			p_sys->p_res_man[2], p_sys->p_res_man[3],
			NULL, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^���\�[�X�j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	char_no		�L�����N�^�ԍ�
 */
//-----------------------------------------------------------------------------
static void WF_2DC_CharResDel( WF_2DCSYS* p_sys, u32 char_no )
{
	// �ǂݍ��ݍς݂ł��邱�Ƃ��`�F�b�N
	GF_ASSERT( p_sys->res[ char_no ].resobj[0] != NULL );

	if( p_sys->res[ char_no ].type == WF_2DC_RES_NML ){
		WF_2DC_CharResDelNml( p_sys, char_no );
	}else{
		WF_2DC_CharResDelUni( p_sys, char_no );
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^���\�[�X�j��	�ʏ펞
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	char_no		�L�����N�^�ԍ�
 */
//-----------------------------------------------------------------------------
static void WF_2DC_CharResDelNml( WF_2DCSYS* p_sys, u32 char_no )
{
	// VRAM�Ǘ�����j��
	CLACT_U_CharManagerDelete( p_sys->res[ char_no ].resobj[0] );
	CLACT_U_PlttManagerDelete( p_sys->res[ char_no ].resobj[1] );
	
	// ���\�[�X�j��
	CLACT_U_ResManagerResDelete( p_sys->p_res_man[0], p_sys->res[ char_no ].resobj[0] );
	CLACT_U_ResManagerResDelete( p_sys->p_res_man[1], p_sys->res[ char_no ].resobj[1] );

	p_sys->res[ char_no ].resobj[0] = NULL;
	p_sys->res[ char_no ].resobj[1] = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^���\�[�X�j��	���j�I����
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	char_no		�L�����N�^�ԍ�
 */
//-----------------------------------------------------------------------------
static void WF_2DC_CharResDelUni( WF_2DCSYS* p_sys, u32 char_no )
{
	// VRAM�Ǘ�����j��
	CLACT_U_CharManagerDelete( p_sys->res[ char_no ].resobj[0] );
	
	// ���\�[�X�j��
	CLACT_U_ResManagerResDelete( p_sys->p_res_man[0], p_sys->res[ char_no ].resobj[0] );

	p_sys->res[ char_no ].resobj[0] = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^���\�[�X���ǂݍ��܂�Ă��邩�`�F�b�N
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	char_no		�L�����N�^�ԍ�
 *
 *	@retval	TRUE	�ǂݍ��݊���
 *	@retval	FALSE	�ǂݍ���ł��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WF_2DC_CharResCheck( const WF_2DCSYS* cp_sys, u32 char_no )
{
	if( cp_sys->res[ char_no ].resobj[0] == NULL ){
		return FALSE;
	}
	return TRUE;	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��^�C�v�̎擾
 *
 *	@param	cp_res	���\�[�X���[�N
 *
 *	@return	�`��^�C�v�擾
 */
//-----------------------------------------------------------------------------
static u32 WF_2DC_CharResDrawTypeGet( const WF_2DCRES* cp_res )
{
	return cp_res->drawtype;
}

//----------------------------------------------------------------------------
/**
 *	@brief	����^�C�v�擾
 *
 *	@param	cp_res	���\�[�X���[�N
 *	
 *	@return	����^�C�v
 */
//-----------------------------------------------------------------------------
static WF_2DC_MOVETYPE WF_2DC_CharResMoveTypeGet( const WF_2DCRES* cp_res )
{
	return cp_res->movetype;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���\�[�X��ǂݍ���
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	draw_type	�\�����
 *	@param	heap		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WF_2DC_ShResLoad( WF_2DCSYS* p_sys, u32 draw_type, u32 heap )
{
	int i;
	BOOL result;
	BOOL hero_load;
	u32 hero_no;
	GF_ASSERT( p_sys->shadowres.resobj[0] == NULL );

	// ��l�����ǂݍ��܂�Ă��邩�`�F�b�N����

	hero_load = FALSE;
	if( WF_2DC_CharResCheck( p_sys, WF_2DC_HERO ) == TRUE ){
		hero_no		= WF_2DC_HERO;
		hero_load	= TRUE;
	}
	else if( WF_2DC_CharResCheck( p_sys, WF_2DC_HEROINE ) == TRUE ){
		hero_no		= WF_2DC_HEROINE;
		hero_load	= TRUE;
	}
	else if( WF_2DC_CharResCheck( p_sys, WF_2DC_DPHERO ) == TRUE ){
		hero_no		= WF_2DC_DPHERO;
		hero_load	= TRUE;
	}
	else if( WF_2DC_CharResCheck( p_sys, WF_2DC_DPHEROINE ) == TRUE ){
		hero_no		= WF_2DC_DPHEROINE;
		hero_load	= TRUE;
	}

	GF_ASSERT_MSG( hero_load == TRUE, "shodow load miss  hero or heroine not load" );

	// �A�j�����\�[�X
	for( i=0; i<2; i++ ){
		p_sys->shadowres.resobj[i+2] = 
			CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
				p_sys->p_res_man[i+2], p_sys->p_handle[WF_2DC_ARCHANDLE_NML],
				NARC_wifi2dchar_hero_ine_kage_NCER+i, FALSE,
				WF_2DC_ARC_CONTSHADOWID, CLACT_U_CELL_RES+i, heap );
	}

	// �L�����N�^���\�[�X
	// �ǂݍ���
	p_sys->shadowres.resobj[0] = 
		CLACT_U_ResManagerResAddArcChar_ArcHandle(
				p_sys->p_res_man[0], p_sys->p_handle[WF_2DC_ARCHANDLE_NML],
				NARC_wifi2dchar_hero_ine_kage_NCGR,
				FALSE, WF_2DC_ARC_CONTSHADOWID, draw_type, heap );

	
	// �]��
	result = CLACT_U_CharManagerSetCharModeAdjustAreaCont( p_sys->shadowres.resobj[0] );
	GF_ASSERT( result );

	// ���\�[�X�����j��
	CLACT_U_ResManagerResOnlyDelete( p_sys->shadowres.resobj[0] );


	// �Z���A�N�^�[�w�b�_�[�쐬
	CLACT_U_MakeHeader( &p_sys->shadowres.header, 
			WF_2DC_ARC_CONTSHADOWID, WF_2DC_ARC_CONTCHARID+hero_no,
			WF_2DC_ARC_CONTSHADOWID, WF_2DC_ARC_CONTSHADOWID,
			CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,0,0,
			p_sys->p_res_man[0], p_sys->p_res_man[1], 
			p_sys->p_res_man[2], p_sys->p_res_man[3],
			NULL, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���\�[�X�̔j��
 *	
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_ShResDel( WF_2DCSYS* p_sys )
{
	int i;
	
	if( WF_2DC_ShResCheck( &p_sys->shadowres ) == TRUE ){
		
		// �L�����N�^���\�[�X�j��
		{
			// VRAM�Ǘ�����j��
			CLACT_U_CharManagerDelete( p_sys->shadowres.resobj[0] );
			
			// ���\�[�X�j��
			CLACT_U_ResManagerResDelete( p_sys->p_res_man[0], p_sys->shadowres.resobj[0] );
			p_sys->shadowres.resobj[0] = NULL;
		}

		// �A�j�����\�[�X�j��
		for( i=0; i<2; i++ ){
			CLACT_U_ResManagerResDelete( p_sys->p_res_man[i+2],
						p_sys->shadowres.resobj[i+2] );
			p_sys->shadowres.resobj[i+2] = NULL;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���\�[�X���ǂݍ��܂�Ă��邩�`�F�b�N
 *
 *	@param	cp_shadowres	���[�N
 *
 *	@retval	TRUE	���\�[�X�ǂݍ��ݍς�
 *	@retval	FALSE	���\�[�X���ǂݍ��܂�Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WF_2DC_ShResCheck( const WF_2DCSH_RES* cp_shadowres )
{

	if( cp_shadowres->resobj[0] != NULL ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�I���L�����N�^�p�p���b�g�Ǎ���
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	draw_type	�`���
 *	@param	heap
 */
//-----------------------------------------------------------------------------
static void WF_2DC_UniCharPlttResLoad( WF_2DCSYS* p_sys, u32 draw_type, u32 heap )
{
	BOOL result;

	GF_ASSERT( p_sys->p_unionpltt == NULL );
	
	// ���j�I���p���b�g�ǂݍ���
	p_sys->p_unionpltt = 
		CLACT_U_ResManagerResAddArcPltt_ArcHandle(
				p_sys->p_res_man[ 1 ], p_sys->p_handle[WF_2DC_ARCHANDLE_UNI],
				WF_2DC_ARC_GETUNINCL,
				FALSE, WF_2DC_ARC_CONTUNIONPLTTID, draw_type, WF_2DC_UNIPLTT_NUM,
				heap );

	// �]��
	result = CLACT_U_PlttManagerSetCleanArea( p_sys->p_unionpltt );
	GF_ASSERT( result );

	// �p���b�g�t�F�[�h�ݒ�
	if( p_sys->p_pfd ){
		WF_2DC_PFDPalSet( p_sys, p_sys->p_unionpltt, WF_2DC_UNIPLTT_NUM );
	}

	// ���\�[�X�����j��
	CLACT_U_ResManagerResOnlyDelete( p_sys->p_unionpltt );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�I���L�����N�^�p�p���b�g�j��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_UniCharPlttResDel( WF_2DCSYS* p_sys )
{
	GF_ASSERT( p_sys->p_unionpltt != NULL );
	
	// ���j�I���p���b�g�j��
	CLACT_U_PlttManagerDelete( p_sys->p_unionpltt );
	CLACT_U_ResManagerResDelete( p_sys->p_res_man[1], p_sys->p_unionpltt );
	p_sys->p_unionpltt = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�I���L�����N�^�p�A�j�����\�[�X�Ǎ���
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_UniCharAnmResLoad( WF_2DCSYS* p_sys, u32 heap )
{
	int i;

	GF_ASSERT( p_sys->unionres[ 0 ].resobj[0] == NULL );

	for( i=0; i<WF_2DC_UNICHAR_NUM; i++ ){

		p_sys->unionres[i].resobj[ 0 ] = 
			CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
				p_sys->p_res_man[2], p_sys->p_handle[WF_2DC_ARCHANDLE_UNI],
				WF_2DC_ARC_GETUNICEL(i), FALSE,
				WF_2DC_ARC_CONTUNIONANMID+i, CLACT_U_CELL_RES, heap );

		p_sys->unionres[i].resobj[ 1 ] = 
			CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
				p_sys->p_res_man[3], p_sys->p_handle[WF_2DC_ARCHANDLE_UNI],
				WF_2DC_ARC_GETUNIANM(i), FALSE,
				WF_2DC_ARC_CONTUNIONANMID+i, CLACT_U_CELLANM_RES, heap );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�I���L�����N�^�p�A�j�����\�[�X�j��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_UniCharAnmResDel( WF_2DCSYS* p_sys )
{
	int i;

	GF_ASSERT( p_sys->unionres[ 0 ].resobj[0] != NULL );

	for( i=0; i<WF_2DC_UNICHAR_NUM; i++ ){

		CLACT_U_ResManagerResDelete( p_sys->p_res_man[2],
					p_sys->unionres[i].resobj[ 0 ] );

		CLACT_U_ResManagerResDelete( p_sys->p_res_man[3],
					p_sys->unionres[i].resobj[ 1 ] );

		p_sys->unionres[ i ].resobj[0] = NULL;
		p_sys->unionres[ i ].resobj[1] = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��̃��[�N���擾
 *
 *	@param	p_sys	���[�N
 *
 *	@return	��̃��[�N
 */
//-----------------------------------------------------------------------------
static WF_2DCWK* WF_2DC_WkCleanGet( WF_2DCSYS* p_sys )
{
	int i;

	for( i=0; i<p_sys->objnum; i++ ){
		if( WF_2DC_WkCleanCheck( &p_sys->p_wk[i] ) == TRUE ){
			return &p_sys->p_wk[i];
		}
	}
	GF_ASSERT( 0 );
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	Pattern�A�j���X�V
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmUpdate( WF_2DCWK* p_wk )
{
	static void (* const pFunc[])( WF_2DCWK*  ) = {
		NULL,
		WF_2DC_WkPatAnmRotaMain,
		WF_2DC_WkPatAnmWalkMain,
		WF_2DC_WkPatAnmTurnMain,
		WF_2DC_WkPatAnmRunMain,
		WF_2DC_WkPatAnmWallWalkMain,
		WF_2DC_WkPatAnmSlowWalkMain,
		WF_2DC_WkPatAnmHighWalk2Main,
		WF_2DC_WkPatAnmHighWalk4Main,
		NULL
	};
	if( pFunc[p_wk->patanmtype] != NULL ){
		pFunc[p_wk->patanmtype]( p_wk );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ς��A�j��������
 *
 *	@param	p_wk	���[�N
 *	@param	way		����
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmWayInit( WF_2DCWK* p_wk, WF_COMMON_WAY way )
{
	p_wk->patanmway = way;
	CLACT_AnmChgCheck( p_wk->p_clwk, p_wk->patanmway );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�A�j��������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmRotaInit( WF_2DCWK* p_wk, WF_COMMON_WAY way )
{
	p_wk->patanmway = way;
	CLACT_AnmChgCheck( p_wk->p_clwk, WF_2DC_ANM_ROTA );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����A�j��������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmWalkInit( WF_2DCWK* p_wk, WF_COMMON_WAY way )
{
	p_wk->patanmway = way;
	CLACT_AnmChgCheck( p_wk->p_clwk, WF_2DC_ANM_WALK+p_wk->patanmway );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�U������A�j��������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmTurnInit( WF_2DCWK* p_wk, WF_COMMON_WAY way )
{
	CLACT_AnmChgCheck( p_wk->p_clwk, WF_2DC_ANM_WALK+p_wk->patanmway );

	// ��Őݒ肷�邱�ƂłQ�t���[���ڂŕ������ς��
	p_wk->patanmway = way;
}

//----------------------------------------------------------------------------
/**
 *	@brief	����A�j��������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmRunInit( WF_2DCWK* p_wk, WF_COMMON_WAY way )
{
	p_wk->patanmway = way;
	CLACT_AnmChgCheck( p_wk->p_clwk, WF_2DC_ANM_RUN+p_wk->patanmway );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ǖ��������A�j��������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmWallWalkInit( WF_2DCWK* p_wk, WF_COMMON_WAY way )
{
	p_wk->patanmway = way;
	CLACT_AnmChgCheck( p_wk->p_clwk, WF_2DC_ANM_WALK+p_wk->patanmway );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����������A�j��������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmSlowWalkInit( WF_2DCWK* p_wk, WF_COMMON_WAY way )
{
	p_wk->patanmway = way;
	CLACT_AnmChgCheck( p_wk->p_clwk, WF_2DC_ANM_WALK+p_wk->patanmway );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����A�j���Q�t���[��
 *
 *	@param	p_wk		���[�N
 *	@param	way			����
 */		
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmHighWalk2Init( WF_2DCWK* p_wk, WF_COMMON_WAY way )
{
	p_wk->patanmway = way;
	CLACT_AnmChgCheck( p_wk->p_clwk, WF_2DC_ANM_WALK+p_wk->patanmway );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����A�j���S�t���[��
 *
 *	@param	p_wk		���[�N
 *	@param	way			����
 */		
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmHighWalk4Init( WF_2DCWK* p_wk, WF_COMMON_WAY way )
{
	p_wk->patanmway = way;
	CLACT_AnmChgCheck( p_wk->p_clwk, WF_2DC_ANM_WALK+p_wk->patanmway );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�A�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmRotaMain( WF_2DCWK* p_wk )
{
	CLACT_AnmFrameChg( p_wk->p_clwk, WF_2DC_ANM_FRAME );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����A�j�����C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmWalkMain( WF_2DCWK* p_wk )
{
	CLACT_AnmFrameChg( p_wk->p_clwk, WF_2DC_ANM_FRAME );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�U������A�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmTurnMain( WF_2DCWK* p_wk )
{
	if( p_wk->patanmframe == 0 ){
		CLACT_AnmFrameSet( p_wk->p_clwk, 1 );
	}else{
		CLACT_AnmChg( p_wk->p_clwk, WF_2DC_ANM_WALK+p_wk->patanmway );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	����A�j�����C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmRunMain( WF_2DCWK* p_wk )
{
	CLACT_AnmFrameChg( p_wk->p_clwk, WF_2DC_ANM_FRAME );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ǖ��������A�j�����C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmWallWalkMain( WF_2DCWK* p_wk )
{
	CLACT_AnmFrameChg( p_wk->p_clwk, WF_2DC_ANM_SLOWFRAME );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����������A�j�����C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmSlowWalkMain( WF_2DCWK* p_wk )
{
	CLACT_AnmFrameChg( p_wk->p_clwk, WF_2DC_ANM_SLOWFRAME );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����Q�t���A�j�����C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmHighWalk2Main( WF_2DCWK* p_wk )
{
	CLACT_AnmFrameChg( p_wk->p_clwk, WF_2DC_ANM_HIGH2 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����S�t���A�j�����C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WF_2DC_WkPatAnmHighWalk4Main( WF_2DCWK* p_wk )
{
	CLACT_AnmFrameChg( p_wk->p_clwk, WF_2DC_ANM_HIGH4 );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���[�N���g���Ă��邩�`�F�b�N
 *
 *	@param cp_wk	���[�N
 *
 *	@retval		TRUE�@		���g�p
 *	@retval		FALSE		�g�p��
 */
//-----------------------------------------------------------------------------
static BOOL WF_2DC_WkCleanCheck( const WF_2DCWK* cp_wk )
{
	if( cp_wk->p_clwk == NULL ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�f�[�^��ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_pal		�p���b�g���\�[�X�f�[�^
 *	@param	num			��
 */
//-----------------------------------------------------------------------------
static void WF_2DC_PFDPalSet( WF_2DCSYS* p_sys, CLACT_U_RES_OBJ_PTR p_pal, u32 num )
{
	NNSG2dPaletteData* p_paldata;
	u32 pal_no[2];

	p_paldata = CLACT_U_ResManagerGetResObjResPltt( p_pal );
	pal_no[0] = CLACT_U_PlttManagerGetPlttNo( p_pal, NNS_G2D_VRAM_TYPE_2DMAIN );
	pal_no[1] = CLACT_U_PlttManagerGetPlttNo( p_pal, NNS_G2D_VRAM_TYPE_2DSUB );
	
	if( pal_no[0] != CLACT_U_PLTT_NO_NONE ){
		PaletteWorkSet( p_sys->p_pfd, p_paldata->pRawData, FADE_MAIN_OBJ, pal_no[0]*16, num*32 );
	}
	if( pal_no[1] != CLACT_U_PLTT_NO_NONE ){
		PaletteWorkSet( p_sys->p_pfd, p_paldata->pRawData, FADE_SUB_OBJ, pal_no[1]*16, num*32 );
	}
}

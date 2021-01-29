//==============================================================================
/**
 * @file	footprint_main.c
 * @brief	���Ճ{�[�h���C��
 * @author	matsuda
 * @date	2008.01.18(��)
 */
//==============================================================================
#include "common.h"
#include "system/snd_tool.h"
#include "system/bmp_list.h"
#include "system/pmfprint.h"
#include "savedata\system_data.h"
#include "system/bmp_menu.h"
#include "system/procsys.h"
#include "system/wipe.h"
#include "system/window.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "system/fontproc.h"
#include "gflib/strbuf_family.h"
#include "msgdata/msg_wflby_footprint.h"
#include "system\font_arc.h"
#include "savedata/config.h"
#include "communication/wm_icon.h"
#include "system/fontoam.h"
#include "system/d3dobj.h"
#include "system/clact_tool.h"

#include "application/wifi_lobby/wflby_system_def.h"
#include "application/footprint_main.h"
#include "footprint_common.h"
#include "graphic/footprint_board.naix"
#include "footprint_stamp.h"
#include "footprint_comm.h"
#include "footprint_tool.h"
#include "footprint_id.h"
#include "footprint_control.h"
#include "footprint_snd_def.h"

#include "battle/battle_common.h"	//POKEMON_TEMOTI_MAX��`
#include "poketool/poke_tool.h"
#include "poketool/pokeparty.h"
#include "poketool/pokefoot.h"	//POKEFOOT_ARC_CHAR_DMMY��`�̈�
#include "system/touch_subwindow.h"


//==============================================================================
//	�萔��`
//==============================================================================
///HEAPID_FOOTPRINT���m�ۂ���q�[�v�T�C�Y
#define FOOTPRINT_HEAP_SIZE		(0x50000)

///�Ǘ�����t�H���gOAM��
#define FOOT_FONTOAM_MAX		(4)	//�������߂�

//--------------------------------------------------------------
//	�J�����ݒ�
//--------------------------------------------------------------
#define FOOTPRINT_CAMERA_MODE			GF_CAMERA_PERSPECTIV	//(GF_CAMERA_ORTHO)

#define FOOTPRINT_CAMERA_PERSPWAY		(FX_GET_ROTA_NUM(22))
#define FOOTPRINT_CAMERA_NEAR			( FX32_ONE )
#define FOOTPRINT_CAMERA_FAR			( FX32_ONE * 900 )
#define FOOTPRINT_CAMERA_TX				( 0 )		/// target
#define FOOTPRINT_CAMERA_TY				( -FX32_ONE * 8 )
#define FOOTPRINT_CAMERA_TZ				( 0 )

///�J�����̒����_�܂ł̋���
#define FOOTPRINT_CAMERA_DISTANCE		(0x7c000)	//(0x96 << FX32_SHIFT)

///�J�����A���O��
static const CAMERA_ANGLE FootprintCameraAngle = {
	FX_GET_ROTA_NUM(0), FX_GET_ROTA_NUM(0), FX_GET_ROTA_NUM(0),
};

//--------------------------------------------------------------
//	�{�[�h�̃��f���ݒ�
//--------------------------------------------------------------
#define BOARD_X		(FX32_CONST(0))
#define BOARD_Y		(FX32_CONST(0))
#define BOARD_Z		(FX32_CONST(0))
#define BOARD_SCALE	(FX32_CONST(1.00f))

//--------------------------------------------------------------
//	CL_ACT�p�̒萔��`
//--------------------------------------------------------------
///���C��	OAM�Ǘ��̈�E�J�n
#define FOOTPRINT_OAM_START_MAIN			(0)
///���C��	OAM�Ǘ��̈�E�I��
#define FOOTPRINT_OAM_END_MAIN				(128)
///���C��	�A�t�B���Ǘ��̈�E�J�n
#define FOOTPRINT_OAM_AFFINE_START_MAIN		(0)
///���C��	�A�t�B���Ǘ��̈�E�I��
#define FOOTPRINT_OAM_AFFINE_END_MAIN		(32)
///�T�u	OAM�Ǘ��̈�E�J�n
#define FOOTPRINT_OAM_START_SUB				(0)
///�T�u	OAM�Ǘ��̈�E�I��
#define FOOTPRINT_OAM_END_SUB				(128)
///�T�u �A�t�B���Ǘ��̈�E�J�n
#define FOOTPRINT_OAM_AFFINE_START_SUB		(0)
///�T�u	�A�t�B���Ǘ��̈�E�I��
#define FOOTPRINT_OAM_AFFINE_END_SUB		(32)

///�L�����}�l�[�W���F�L�����N�^ID�Ǘ���(���ʁ{�����)
#define FOOTPRINT_CHAR_MAX					(48 + 48)
///�L�����}�l�[�W���F���C����ʃT�C�Y(byte�P��)
#define FOOTPRINT_CHAR_VRAMSIZE_MAIN		(1024 * 0x40)	//64K
///�L�����}�l�[�W���F�T�u��ʃT�C�Y(byte�P��)
#define FOOTPRINT_CHAR_VRAMSIZE_SUB			(512 * 0x20)	//32K

///���C����ʁ{�T�u��ʂŎg�p����A�N�^�[����
#define FOOTPRINT_ACTOR_MAX					(64 + 64)	//���C����� + �T�u���

///OBJ�Ŏg�p����p���b�g�{��(���ʁ{�����)
#define FOOTPRINT_OAM_PLTT_MAX				(16 + 16)

///OAM���\�[�X�F�L�����o�^�ő吔(���C����� + �T�u���)
#define FOOTPRINT_OAMRESOURCE_CHAR_MAX		(FOOTPRINT_CHAR_MAX)
///OAM���\�[�X�F�p���b�g�o�^�ő吔(���C����� + �T�u���)
#define FOOTPRINT_OAMRESOURCE_PLTT_MAX		(FOOTPRINT_OAM_PLTT_MAX)
///OAM���\�[�X�F�Z���o�^�ő吔
#define FOOTPRINT_OAMRESOURCE_CELL_MAX		(64)
///OAM���\�[�X�F�Z���A�j���o�^�ő吔
#define FOOTPRINT_OAMRESOURCE_CELLANM_MAX	(64)
///OAM���\�[�X�F�}���`�Z���o�^�ő吔
#define FOOTPRINT_OAMRESOURCE_MCELL_MAX		(8)
///OAM���\�[�X�F�}���`�Z���A�j���o�^�ő吔
#define FOOTPRINT_OAMRESOURCE_MCELLANM_MAX	(8)

//--------------------------------------------------------------
//	�V�X�e��
//--------------------------------------------------------------
///�Q���҃��X�g���X�V���鎞�ԊԊu
#define ENTRY_LIST_UPDATE_TIME				(30)

///�^�C���A�b�v���Ă���I���܂ł̑҂�����
#define FOOT_TIMEUP_WAIT					(90)

//--------------------------------------------------------------
//	�A�N�^�[
//--------------------------------------------------------------
///�C���N��X���W�J�n�ʒu
#define INK_POS_START_X		(16)
///�C���N�ׂ̗荇���C���N�Ƃ̔z�u�ԊuX
#define INK_POS_SPACE_X		(32)
///�C���N��Y���W
#define INK_POS_Y			(176)

///�C���N�̉��n��X���W�J�n�ʒu
#define INK_FOUNDATION_POS_START_X		(INK_POS_START_X)
///�C���N�̉��n�ׂ̗荇���C���N�̉��n�Ƃ̔z�u�ԊuX
#define INK_FOUNDATION_POS_SPACE_X		(INK_POS_SPACE_X)
///�C���N�̉��n��Y���W
#define INK_FOUNDATION_POS_Y			(INK_POS_Y)

///�C���N�̑��Ղ�X���W�J�n�ʒu
#define INK_FOOT_POS_START_X		(INK_POS_START_X)
///�C���N�̑��Ղׂ̗荇���C���N�̑��ՂƂ̔z�u�ԊuX
#define INK_FOOT_POS_SPACE_X		(INK_POS_SPACE_X)
///�C���N�̑��Ղ�Y���W
#define INK_FOOT_POS_Y			(INK_POS_Y)

///�^�b�`�G�t�F�N�g�𓯎��ɏo����ő吔
#define TOUCH_EFF_MAX			(3)

//--------------------------------------------------------------
//	�X�N���[��
//--------------------------------------------------------------
///�C���N�̘g��X���W�J�n�ʒu
#define SCRN_INK_POS_START_X		(0)
///�C���N�̘g��Y���W
#define SCRN_INK_POS_Y			(0x14)
///�C���N�̘g�ׂ̗荇���C���N�̘g�Ƃ̔z�u�ԊuX
#define SCRN_INK_POS_SIZE_X		(4)
///�C���N�̘g�ׂ̗荇���C���N�̘g�Ƃ̔z�u�ԊuX
#define SCRN_INK_POS_SIZE_Y		(4)

///�T�u��ʁF���O���X�g�̃p���b�g�F�J�n�J���[�ʒu
#define SUBBG_LIST_COLOR_START	(2 * 16 + 1)

///���O�\�����X�g�̃X�N���[���T�C�Y
#define NAMELIST_SCRN_SIZE		(0x800)

//--------------------------------------------------------------
//	�u��߂�v�{�^��
//--------------------------------------------------------------
///�u��߂�v�{�^����BG�J���[�ʒu
#define EXIT_BUTTON_COLOR_POS		(0*16 + 9)
///�u��߂�v�{�^�������������ɂ�����t�F�[�hevy�l
#define EXIT_BUTTON_COLOR_EVY		(8)
///�u��߂�v�{�^�������������ɂ�����t�F�[�h�̃J���[�R�[�h
#define EXIT_BUTTON_COLOR_CODE		(0x0000)

//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///FootPrintTool_NameAllUpdate�֐��̖߂�l
typedef enum{
	FOOTPRINT_NAME_UPDATE_STATUS_NULL,			///<�ω��Ȃ�
	FOOTPRINT_NAME_UPDATE_STATUS_ENTRY,			///<�V�K�G���g���[����
	FOOTPRINT_NAME_UPDATE_STATUS_LEAVE_ROOM,	///<�ގ��Ҕ���
}FOOTPRINT_NAME_UPDATE_STATUS;


//--------------------------------------------------------------
//	
//--------------------------------------------------------------
enum{
	FONTOAM_LEFT,		///<X���[���W
	FONTOAM_CENTER,		///<X���S���W
};


//==============================================================================
//	�\���̒�`
//==============================================================================
///�{�[�h����\����
typedef struct{
	D3DOBJ_MDL  mdl;
	D3DOBJ      obj;
}BOARD_PARAM;

///�t�H���g�A�N�^�[���[�N
typedef struct{
	FONTOAM_OBJ_PTR fontoam;
	CHAR_MANAGER_ALLOCDATA cma;
	u16 font_len;
}FONT_ACTOR;


///���Ճ{�[�h����\����
typedef struct _FOOTPRINT_SYS{
	FOOTPRINT_PARAM *parent_work;		///<parent_work
	SAVEDATA *sv;						///<�Z�[�u�f�[�^�ւ̃|�C���^
	
	GF_BGL_INI		*bgl;				///<BG�V�X�e���ւ̃|�C���^
	PALETTE_FADE_PTR pfd;				///<�p���b�g�V�X�e��
	FONTOAM_SYS_PTR fontoam_sys;		///<�t�H���gOAM�V�X�e���ւ̃|�C���^
	GF_G3DMAN *g3Dman;
	TCB_PTR update_tcb;					///<Update�pTCB�ւ̃|�C���^
	CATS_SYS_PTR		csp;
	CATS_RES_PTR		crp;
	
	TOUCH_SW_SYS *yesno_button;			///<�{�^���́u�͂��E�������v�V�X�e��
	u8 yesno_button_use;				///<TRUE:�u�͂��E�������v�{�^�����g�p��
	u8 timeup_wait;						///<�^�C���A�b�v���Ă���I���܂ł̑҂�����
	
	// �`��܂��̃��[�N�i���BMP�p�̕��������j
	WORDSET			*wordset;							// ���b�Z�[�W�W�J�p���[�N�}�l�[�W���[
	MSGDATA_MANAGER *msgman;						// ���O���̓��b�Z�[�W�f�[�^�}�l�[�W���[

	// BMPWIN�`�����
	GF_BGL_BMPWIN		name_win[FOOTPRINT_BMPWIN_NAME_MAX]; //���O�\���p��BMPWIN
	GF_BGL_BMPWIN		talk_win;					 //��b���b�Z�[�W�p��BMPWIN
	STRBUF *talk_strbuf;				///<��b���b�Z�[�W�p�o�b�t�@
	u8 msg_index;						///<���b�Z�[�W�C���f�b�N�X

	GF_CAMERA_PTR camera;				///<�J�����ւ̃|�C���^
	fx32 world_width;					///<�ݒu����Ă���J�����ɕ\������Ă��郏�[���h���W�̕�
	fx32 world_height;					///<�ݒu����Ă���J�����ɕ\������Ă��郏�[���h���W�̍���
	
	//�{�[�h
	BOARD_PARAM board;					///<�{�[�h���䃏�[�N
	
	ARCHANDLE *handle_footprint;		///<footprint_board.narc�̃n���h��
	ARCHANDLE *handle_footmark;			///<���ՃO���t�B�b�N�̃n���h��
	
	FOOTPRINT_MY_COMM_STATUS my_comm_status;	///<�����̒ʐM�X�e�[�^�X
	
	s32 entry_userid[FOOTPRINT_ENTRY_MAX];		///<�Q���҂̃��[�U�[ID���X�g
	int entry_list_update_timer;			///<�Q���҂̃��[�U�[ID���X�g���X�V����^�C�����J�E���g
	
	STAMP_SYSTEM_WORK ssw;				///<�X�^���v�V�X�e�����[�N
	STAMP_PARAM my_stamp_param[POKEMON_TEMOTI_MAX];		///<�����̎莝���X�^���v�p�����[�^
	u8 select_no;						///<�I�����Ă��鑫�ՃX�^���v�̔ԍ�
	u8 yameru_pal_pos;					///<�u��߂�v�t�H���gOAM�̃p���b�g�ԍ�
	
	CATS_ACT_PTR cap_ink[POKEMON_TEMOTI_MAX];	///<�C���N�A�N�^�[
	CATS_ACT_PTR cap_ink_foundation[POKEMON_TEMOTI_MAX];	///<�C���N�̉��n�A�N�^�[
	CATS_ACT_PTR cap_ink_foot[POKEMON_TEMOTI_MAX];	///<�C���N�̏�ɔz�u���鑫�ՃA�N�^�[
	CATS_ACT_PTR cap_name_frame;				///<���O���͂ޘg�A�N�^�[
	CATS_ACT_PTR cap_name_foot[FOOTPRINT_ENTRY_MAX];	///<���O�̉��̑��ՃA�N�^�[
	CATS_ACT_PTR cap_touch_eff[TOUCH_EFF_MAX];	///<�^�b�`�G�t�F�N�g�A�N�^�[
	
	u16 name_foot_monsno[FOOTPRINT_ENTRY_MAX];	///<���O�̉��ɏo���Ă���|�P�����ԍ�
	u16 name_foot_color[FOOTPRINT_ENTRY_MAX];	///<���O�̉��ɏo���Ă���J���[
	
	int game_status;					///<�Q�[�����(FOOTPRINT_GAME_STATUS_???)
	
	u16 namelist_scrn[NAMELIST_SCRN_SIZE];			///<�T�u��ʂ̖��O�\�����X�g�̃X�N���[���f�[�^
	
	FONT_ACTOR fontoam_exit;			///<�u��߂�v

	int ink_now;						///<�C���N�̌��݂̎c��
	int ink_calc;						///<�C���N�Q�[�W�̌��݌v�Z�ʒu
	int ink_sub;						///<�C���N�̌��Z��
	
	BOOL arceus_flg;					///<TRUE:�A���Z�E�XOK
}FOOTPRINT_SYS;


//==============================================================================
//	CLACT�p�f�[�^
//==============================================================================
static	const TCATS_OAM_INIT FootprintTcats = {
	FOOTPRINT_OAM_START_MAIN, FOOTPRINT_OAM_END_MAIN,
	FOOTPRINT_OAM_AFFINE_START_MAIN, FOOTPRINT_OAM_AFFINE_END_MAIN,
	FOOTPRINT_OAM_START_SUB, FOOTPRINT_OAM_END_SUB,
	FOOTPRINT_OAM_AFFINE_START_SUB, FOOTPRINT_OAM_AFFINE_END_SUB,
};

static	const TCATS_CHAR_MANAGER_MAKE FootprintCcmm = {
	FOOTPRINT_CHAR_MAX,
	FOOTPRINT_CHAR_VRAMSIZE_MAIN,
	FOOTPRINT_CHAR_VRAMSIZE_SUB,
	GX_OBJVRAMMODE_CHAR_1D_128K,	//64K	�J�E���g�_�E���G�t�F�N�g��128KOBJ�Ȃ̂ō��킹��
	GX_OBJVRAMMODE_CHAR_1D_32K
};

static const TCATS_RESOURCE_NUM_LIST FootprintResourceList = {
	FOOTPRINT_OAMRESOURCE_CHAR_MAX,
	FOOTPRINT_OAMRESOURCE_PLTT_MAX,
	FOOTPRINT_OAMRESOURCE_CELL_MAX,
	FOOTPRINT_OAMRESOURCE_CELLANM_MAX,
	FOOTPRINT_OAMRESOURCE_MCELL_MAX,
	FOOTPRINT_OAMRESOURCE_MCELLANM_MAX,
};


//==============================================================================
//	�f�[�^
//==============================================================================
///�Q���҂̖��O�\���ʒu(BMPWIN�̈ʒu)
ALIGN4 static const u8 NameBmpwinPos[][2] = {	//X,Y
	{7,		5},
	{0x16,	5},
	{7,		5 + 5*1},
	{0x16,	5 + 5*1},
	{7,		5 + 5*2},
	{0x16,	5 + 5*2},
	{7,		5 + 5*3},
	{0x16,	5 + 5*3},
};

///�T�u��ʁF���O���̑��ՃA�N�^�[�̍��W
ALIGN4 static const s16 Sub_FootmarkPos[][2] = {	//x, y
	{32,		48},
	{0x13*8,	48},
	{32,		48 + 5*8},
	{0x13*8,	48 + 5*8},
	{32,		48 + 10*8},
	{0x13*8,	48 + 10*8},
	{32,		48 + 15*8},
	{0x13*8,	48 + 15*8},
};

///�T�u��ʁF������\�����O���͂ރt���[���̃A�N�^�[���W
ALIGN4 static const s16 Sub_NameFramePos[][2] = {	//x, y
	{9*8,		6*8 + 5*8*0},
	{0x18*8,	6*8 + 5*8*0},
	{9*8,		6*8 + 5*8*1},
	{0x18*8,	6*8 + 5*8*1},
	{9*8,		6*8 + 5*8*2},
	{0x18*8,	6*8 + 5*8*2},
	{9*8,		6*8 + 5*8*3},
	{0x18*8,	6*8 + 5*8*3},
};

///�T�u��ʁF���O���X�g�̋�`�͈�(�X�N���[��)
ALIGN4 static const u16 Sub_ListScrnRange[][4] = {	//x, y, size_x, size_y
	{2,			4 + 5*0,		14, 4},
	{0x11,		4 + 5*0,		14, 4},
	{2,			4 + 5*1,		14, 4},
	{0x11,		4 + 5*1,		14, 4},
	{2,			4 + 5*2,		14, 4},
	{0x11,		4 + 5*2,		14, 4},
	{2,			4 + 5*3,		14, 4},
	{0x11,		4 + 5*3,		14, 4},
};

//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///�莝���|�P���������Ȃ����ɃC���N�p���b�g�𖄂߂�X�N���[���R�[�h
static const u16 MyInkPaletteEraseScrnCode[] = {	//1�i�ځA2�i�ځc
	0x15, 0x35, 0x35, 0x55,
};

//==============================================================================
//	�A�N�^�[�w�b�_
//==============================================================================
///�C���N �A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S InkObjParam = {
	0,0, 0,		//x, y, z
	0, SOFTPRI_INK, PALOFS_INK,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_INK,			//�L����
		PLTTID_OBJ_COMMON,	//�p���b�g
		CELLID_INK,			//�Z��
		CELLANMID_INK,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_INK,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///�C���N�̉��n �A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S InkFoundationObjParam = {
	0,0, 0,		//x, y, z
	0, SOFTPRI_INK_FOUNDATION, PALOFS_INK_FOUNDATION,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_INK_FOUNDATION,			//�L����
		PLTTID_OBJ_COMMON,	//�p���b�g
		CELLID_INK_FOUNDATION,			//�Z��
		CELLANMID_INK_FOUNDATION,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_INK_FOUNDATION,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///�C���N�̏�ɔz�u���鑫�� �A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S InkFootObjParam = {
	0,0, 0,		//x, y, z
	0, SOFTPRI_INK_FOOT, 0,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_INK_FOOT_0,			//�L����
		PLTTID_OBJ_INK_FOOT,	//�p���b�g
		CELLID_INK_FOOT,			//�Z��
		CELLANMID_INK_FOOT,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_INK_FOOT,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///�C���N�p���b�g���^�b�`�������ɏo���G�t�F�N�g �A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S TouchEffObjParam = {
	0,0, 0,		//x, y, z
	0, SOFTPRI_TOUCH_EFF, PALOFS_TOUCH_EFF,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_TOUCH_EFF,			//�L����
		PLTTID_OBJ_COMMON,	//�p���b�g
		CELLID_TOUCH_EFF,			//�Z��
		CELLANMID_TOUCH_EFF,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_TOUCH_EFF,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///���O���͂ޘg �A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S NameFrameObjParam = {
	0,0, 0,		//x, y, z
	0, SOFTPRI_SUB_NAME_FRAME, PALOFS_SUB_NAME_FRAME,	//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DSUB,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_SUB_NAME_FRAME,			//�L����
		PLTTID_SUB_OBJ_COMMON,			//�p���b�g
		CELLID_SUB_NAME_FRAME,			//�Z��
		CELLANMID_SUB_NAME_FRAME,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_SUB_NAME_FRAME,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///���O�̉��ɏo������ �A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S NameFootObjParam = {
	0,0, 0,		//x, y, z
	0, SOFTPRI_SUB_NAME_FOOT, PALOFS_SUB_NAME_FOOT,	//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DSUB,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_SUB_NAME_FOOT_0,			//�L����
		PLTTID_SUB_OBJ_COMMON,			//�p���b�g
		CELLID_SUB_NAME_FOOT,			//�Z��
		CELLANMID_SUB_NAME_FOOT,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_SUB_NAME_FOOT,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void Footprint_Update(TCB_PTR tcb, void *work);
static void VBlankFunc( void * work );
static void FootPrint_VramBankSet(GF_BGL_INI *bgl);
static void BgExit( GF_BGL_INI * ini );
static void BgGraphicSet( FOOTPRINT_SYS * fps, ARCHANDLE* p_handle );
static void BmpWinInit( FOOTPRINT_SYS *fps );
static void BmpWinDelete( FOOTPRINT_SYS *fps );
static GF_G3DMAN * Footprint_3D_Init(int heap_id);
static void FootprintSimpleSetUp(void);
static void Footprint_3D_Exit(GF_G3DMAN *g3Dman);
static void Footprint_CameraInit(FOOTPRINT_SYS *fps);
static void Footprint_CameraExit(FOOTPRINT_SYS *fps);
static void Model3DSet( FOOTPRINT_SYS * fps, ARCHANDLE* p_handle );
static void Model3DDel(FOOTPRINT_SYS *fps);
static void Model3D_Update(FOOTPRINT_SYS *fps);
static void Debug_CameraMove(FOOTPRINT_SYS *fps);
static void Footprint_MyCommStatusSet(FOOTPRINT_SYS *fps);
static void Footprint_InParamCreate(FOOTPRINT_SYS_PTR fps, FOOTPRINT_IN_PARAM *in_para);
static void Footprint_Temoti_to_StampParam(int board_type,SAVEDATA * sv, STAMP_PARAM *stamp_array);
static void DefaultResourceSet_Main(FOOTPRINT_SYS *fps, ARCHANDLE *hdl_main);
static void DefaultResourceSet_Sub(FOOTPRINT_SYS *fps, ARCHANDLE *hdl_main);
static void DefaultActorSet_Main(FOOTPRINT_SYS *fps);
static void DefaultActorDel_Main(FOOTPRINT_SYS *fps);
static void DefaultActorSet_Sub(FOOTPRINT_SYS *fps);
static void DefaultActorDel_Sub(FOOTPRINT_SYS *fps);
static void MyInkPaletteSettings(FOOTPRINT_SYS *fps);
static BOOL OBJFootCharRewrite(int monsno, int form_no, CATS_ACT_PTR cap, ARCHANDLE *hdl_main, ARCHANDLE *hdl_mark, NNS_G2D_VRAM_TYPE vram_type, BOOL arceus_flg);
static void Footprint_TouchEffAdd(FOOTPRINT_SYS_PTR fps, int hit_pos);
static void Footprint_SelectInkPaletteFade(FOOTPRINT_SYS_PTR fps, int hit_pos);
static void Footprint_TouchEffUpdate(FOOTPRINT_SYS_PTR fps);
static FOOTPRINT_NAME_UPDATE_STATUS FootPrintTool_NameAllUpdate(FOOTPRINT_SYS *fps);
static void Sub_FontOamCreate(FOOTPRINT_SYS_PTR fps, FONT_ACTOR *font_actor, const STRBUF *str, 
	FONT_TYPE font_type, GF_PRINTCOLOR color, int pal_offset, int pal_id, 
	int x, int y, int pos_center);
static void Sub_FontOamDelete(FONT_ACTOR *font_actor);
static void FontLenGet(const STRBUF *str, 
	FONT_TYPE font_type, int *ret_dot_len, int *ret_char_len);
static void Footprint_InkGaugeUpdate(FOOTPRINT_SYS_PTR fps);
static BOOL Footprint_InkGauge_Consume(FOOTPRINT_SYS_PTR fps, int consume_num);


//==============================================================================
//	�f�o�b�O�p�ϐ��Ȃ�
//==============================================================================
#ifdef PM_DEBUG
//����^�C�v���ɕ��ׂ����i�l
static const u32 DebugSeikakuTbl[] = {1, 14, 6, 5, 0, 3, 8, 9, 18, 15};

///�f�o�b�O�p���[�N
static struct{
	u8 occ_seikaku;		//���i����L��
	u8 move_type;		//�X�^���v�̓���^�C�v
	u8 consume_zero;	//TRUE=�C���N�Q�[�W������Ȃ��Ȃ�
	u32 backup_personal_rnd;
}DebugFoot = {0};
#endif



//--------------------------------------------------------------
/**
 * @brief   �v���Z�X�֐��F������
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
PROC_RESULT FootPrintProc_Init( PROC * proc, int * seq )
{
	FOOTPRINT_SYS *fps;
	
	sys_VBlankFuncChange(NULL, NULL);	// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	GX_SetVisiblePlane(0);
	GXS_SetVisiblePlane(0);
	GX_SetVisibleWnd(GX_WNDMASK_NONE);
	GXS_SetVisibleWnd(GX_WNDMASK_NONE);
	G2_BlendNone();
	G2S_BlendNone();

	sys_CreateHeap(HEAPID_BASE_APP, HEAPID_FOOTPRINT, FOOTPRINT_HEAP_SIZE);

	fps = PROC_AllocWork(proc, sizeof(FOOTPRINT_SYS), HEAPID_FOOTPRINT );
	MI_CpuClear8(fps, sizeof(FOOTPRINT_SYS));
	fps->parent_work = PROC_GetParentWork(proc);
#ifdef PM_DEBUG
	if(fps->parent_work->wflby_sys == NULL){
		fps->sv = fps->parent_work->debug_sv;
	}
	else{
		fps->sv = WFLBY_SYSTEM_GetSaveData(fps->parent_work->wflby_sys);
	}
#else
	fps->sv = WFLBY_SYSTEM_GetSaveData(fps->parent_work->wflby_sys);
#endif
	
	fps->arceus_flg = WFLBY_SYSTEM_FLAG_GetArceus(fps->parent_work->wflby_sys);
	Footprint_MyCommStatusSet(fps);
	Footprint_Temoti_to_StampParam(fps->parent_work->board_type, fps->sv, fps->my_stamp_param);
	fps->ink_now = INK_GAUGE_TANK_MAX;
	fps->ink_calc = fps->ink_now;
	
//	simple_3DBGInit(HEAPID_FOOTPRINT);
	fps->g3Dman = Footprint_3D_Init(HEAPID_FOOTPRINT);

	//�p���b�g�t�F�[�h�V�X�e���쐬
	fps->pfd = PaletteFadeInit(HEAPID_FOOTPRINT);
	PaletteTrans_AutoSet(fps->pfd, TRUE);
	PaletteFadeWorkAllocSet(fps->pfd, FADE_MAIN_BG, 0x200, HEAPID_FOOTPRINT);
	PaletteFadeWorkAllocSet(fps->pfd, FADE_SUB_BG, 0x200, HEAPID_FOOTPRINT);
	PaletteFadeWorkAllocSet(fps->pfd, FADE_MAIN_OBJ, 0x200-0x40, HEAPID_FOOTPRINT);	//�ʐM�A�C�R��-���[�J���C�Y�p
	PaletteFadeWorkAllocSet(fps->pfd, FADE_SUB_OBJ, 0x200, HEAPID_FOOTPRINT);
	PaletteTrans_AutoSet(fps->pfd, TRUE);
	
	fps->bgl = GF_BGL_BglIniAlloc(HEAPID_FOOTPRINT);

	initVramTransferManagerHeap(64, HEAPID_FOOTPRINT);

	sys_KeyRepeatSpeedSet( SYS_KEYREPEAT_SPEED_DEF, SYS_KEYREPEAT_WAIT_DEF );

	//VRAM���蓖�Đݒ�
	FootPrint_VramBankSet(fps->bgl);

	// �^�b�`�p�l���V�X�e��������
	InitTPSystem();
	InitTPNoBuff(4);

	// �{�^���p�t�H���g��ǂݍ���
	FontProc_LoadFont(FONT_BUTTON, HEAPID_FOOTPRINT);

	//���b�Z�[�W�}�l�[�W���쐬
	fps->wordset		 = WORDSET_Create(HEAPID_FOOTPRINT);
	fps->msgman       = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wflby_footprint_dat, HEAPID_FOOTPRINT );

	//�t�H���gOAM�V�X�e���쐬
	fps->fontoam_sys = FONTOAM_SysInit(FOOT_FONTOAM_MAX, HEAPID_FOOTPRINT);

	//�n���h�����J����(���ՂȂǕp�ɂɃO���t�B�b�N���[�h���s����̂Ńn���h���J���������ɂ���
	fps->handle_footprint = ArchiveDataHandleOpen( ARC_FOOTPRINT_GRA, HEAPID_FOOTPRINT );
	fps->handle_footmark = ArchiveDataHandleOpen(ARC_POKEFOOT_GRA, HEAPID_FOOTPRINT);

	// BG�O���t�B�b�N�]��
	BgGraphicSet( fps, fps->handle_footprint );
	//3D���f���]��
	Model3DSet(fps, fps->handle_footprint);

	//�J�����쐬
	Footprint_CameraInit(fps);

	// BMPWIN�m��
	BmpWinInit( fps );
	
	fps->talk_strbuf = STRBUF_Create(256, HEAPID_FOOTPRINT);
	
	//�A�N�^�[�V�X�e���쐬
	fps->csp=CATS_AllocMemory(HEAPID_FOOTPRINT);
	CATS_SystemInit(fps->csp,&FootprintTcats,&FootprintCcmm,FOOTPRINT_OAM_PLTT_MAX);
	//�ʐM�A�C�R���p�ɃL�������p���b�g����
	CLACT_U_WmIcon_SetReserveAreaCharManager(
		NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
	CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
	fps->crp=CATS_ResourceCreate(fps->csp);
	CATS_ClactSetInit(fps->csp, fps->crp, FOOTPRINT_ACTOR_MAX);
	CATS_ResourceManagerInit(fps->csp,fps->crp,&FootprintResourceList);
	CLACT_U_SetSubSurfaceMatrix(CATS_EasyRenderGet(fps->csp), 0, FOOTPRINT_SUB_ACTOR_DISTANCE);

	// Wifi�ʐM�A�C�R��
    WirelessIconEasy();

	//�X�^���v�V�X�e���쐬
	StampSys_Init(&fps->ssw, fps->arceus_flg);

	//�풓OBJ�o�^
	DefaultResourceSet_Main(fps, fps->handle_footprint);
	DefaultResourceSet_Sub(fps, fps->handle_footprint);
	DefaultActorSet_Main(fps);
	DefaultActorSet_Sub(fps);

	//�����̃C���N�p���b�g�����ݒ�
	MyInkPaletteSettings(fps);
	
	//�{�^���u�͂��E�������v
	fps->yesno_button = TOUCH_SW_AllocWork(HEAPID_FOOTPRINT);
	
	// ���C�v�t�F�[�h�J�n
	WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK, 
		WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_FOOTPRINT );

	// BGM���t�F�[�h�A�E�g
	if( fps->parent_work->wflby_sys != NULL ){
		WFLBY_SYSTEM_SetBGMVolumeDown( fps->parent_work->wflby_sys, TRUE );
	}

	// ���Ճ{�[�h�ɓ��������Đ�
//	Snd_SePlay( WFLBY_SND_FOOTIN );

	// BG�ʕ\��ON
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG0, VISIBLE_ON );
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG1, VISIBLE_ON );
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_ON );
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG3, VISIBLE_ON );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );

	//���C����ʐݒ�
	sys.disp3DSW = DISP_3D_TO_SUB;
	GF_Disp_DispSelect();
	GF_Disp_DispOn();
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);
	GF_Disp_GXS_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);

	MsgPrintSkipFlagSet(MSG_SKIP_ON);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);

	fps->update_tcb = TCB_Add(Footprint_Update, fps, 60000);

	sys_VBlankFuncChange(VBlankFunc, fps);


	return PROC_RES_FINISH;
}

//--------------------------------------------------------------
/**
 * @brief   �v���Z�X�֐��F���C��
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
PROC_RESULT FootPrintProc_Main( PROC * proc, int * seq )
{
	FOOTPRINT_SYS * fps  = PROC_GetWork( proc );
	enum{
		SEQ_INIT,
		SEQ_IN_WIPE_WAIT,
		SEQ_MAIN,
		SEQ_EXIT_SELECT_INIT,
		SEQ_EXIT_SELECT_MSG_WAIT,
		SEQ_EXIT_SELECT,
		SEQ_TIMEUP_INIT,
		SEQ_TIMEUP_MSG_WAIT,
		SEQ_TIMEUP_WAIT,
		SEQ_OUT_INIT,
		SEQ_OUT_WAIT,
	};
	
	//�X�^���v�V�X�e���X�V
	StampSys_Update(&fps->ssw, fps->camera, fps->game_status, fps->parent_work->board_type);

	switch(*seq){
	case SEQ_INIT:
		//�ʐM
		fps->my_comm_status.ready = TRUE;	//�S�Ă̏����������������̂Ŏ�MOK
		Footprint_Comm_Init(fps);
		{//������񑗐M
			FOOTPRINT_IN_PARAM in_para;
			Footprint_InParamCreate(fps, &in_para);
//			Footprint_Send_PlayerIn(&in_para);
		}
		(*seq)++;
		break;
	case SEQ_IN_WIPE_WAIT:
		if(WIPE_SYS_EndCheck() == TRUE){
			fps->game_status = FOOTPRINT_GAME_STATUS_NORMAL;
			(*seq)++;
		}
		break;
	case SEQ_MAIN:
		if(sys.tp_trg && sys.tp_y < 160){
			if(Footprint_InkGauge_Consume(fps, INK_GAUGE_CONSUME_STAMP) == TRUE){
				fps->my_stamp_param[fps->select_no].x = sys.tp_x;
				fps->my_stamp_param[fps->select_no].y = sys.tp_y;
			#ifdef PM_DEBUG
				if(DebugFoot.occ_seikaku){
					DebugFoot.backup_personal_rnd 
						= fps->my_stamp_param[fps->select_no].personal_rnd;
					if(sys.cont & PAD_BUTTON_B){
						fps->my_stamp_param[fps->select_no].personal_rnd 
							= DebugSeikakuTbl[gf_rand() % NELEMS(DebugSeikakuTbl)];
					}
					else{
						fps->my_stamp_param[fps->select_no].personal_rnd 
							= DebugSeikakuTbl[DebugFoot.move_type];
					}
				}
			#endif
				Footprint_StampAdd(
					fps, &fps->my_stamp_param[fps->select_no], fps->my_comm_status.user_id);
				Footprint_Send_Stamp(&fps->my_stamp_param[fps->select_no]);
			#ifdef PM_DEBUG
				if(DebugFoot.occ_seikaku){
					fps->my_stamp_param[fps->select_no].personal_rnd 
						= DebugFoot.backup_personal_rnd;
				}
			#endif
			}
			else{	//�C���N������Ȃ�
				;		//����SE�Ƃ��炷����
			}
		}

		{
			int hit;
			
			hit = FootprintTool_InkPalTouchUpdate(fps->my_stamp_param, fps->select_no);
			if(hit < 6){
				fps->select_no = hit;
				Footprint_TouchEffAdd(fps, hit);
			}
			else if((*seq) == SEQ_MAIN && hit == FOOT_TOUCH_RET_EXIT){	//�u��߂�v��������
				Snd_SePlay(FOOTPRINT_SE_TOUCH_EXIT);
				SoftFadePfd(fps->pfd, FADE_MAIN_BG, EXIT_BUTTON_COLOR_POS, 1, 
					EXIT_BUTTON_COLOR_EVY, EXIT_BUTTON_COLOR_CODE);
				SoftFadePfd(fps->pfd, FADE_MAIN_OBJ, fps->yameru_pal_pos * 16, 16, 
					EXIT_BUTTON_COLOR_EVY, EXIT_BUTTON_COLOR_CODE);
				*seq = SEQ_EXIT_SELECT_INIT;
			}
		}
		break;
	case SEQ_EXIT_SELECT_INIT:
		//��b�E�B���h�E�`��
		GF_BGL_BmpWinDataFill(&fps->talk_win, 0xf);
		BmpTalkWinWrite(&fps->talk_win, WINDOW_TRANS_ON, 
			WINCGX_TALKWIN_START, FOOT_MAINBG_TALKWIN_PAL);
		//���b�Z�[�W�\��
		MSGMAN_GetString(fps->msgman, msg_footprint_exit_select, fps->talk_strbuf);
		fps->msg_index = GF_STR_PrintSimple(&fps->talk_win, FONT_TALK, 
			fps->talk_strbuf, 0, 0, 
			CONFIG_GetMsgPrintSpeed(SaveData_GetConfig(fps->sv)), NULL);
		(*seq)++;
		break;
	case SEQ_EXIT_SELECT_MSG_WAIT:
		if(GF_MSG_PrintEndCheck(fps->msg_index) == 0){
			//�u�͂��E�������v�{�^�����o��
			TOUCH_SW_PARAM tsp;
			
			tsp.p_bgl	  = fps->bgl;
			tsp.bg_frame  = FOOT_FRAME_WIN;
			tsp.char_offs = WINCGX_BUTTON_YESNO_START;
			tsp.pltt_offs = FOOT_MAINBG_BUTTON_YESNO_PAL;
			tsp.x		  = 25;
			tsp.y		  = 6;
			TOUCH_SW_Init(fps->yesno_button, &tsp);
			
			fps->yesno_button_use = TRUE;
			(*seq)++;
		}
		break;
	case SEQ_EXIT_SELECT:
		//VWait�^�X�N�œ]������Ă���̂ŁAInit���ł͂Ȃ�������VRAM����R�s�[����
		PaletteWorkSet_VramCopy(fps->pfd, 
			FADE_MAIN_BG, FOOT_MAINBG_BUTTON_YESNO_PAL*16, 0x20*2);
		{
			u32 yesno_ret = TOUCH_SW_Main(fps->yesno_button);
			switch(yesno_ret){
			case TOUCH_SW_RET_YES:
				TOUCH_SW_Reset(fps->yesno_button);
				fps->yesno_button_use = 0;
				BmpTalkWinClear(&fps->talk_win, WINDOW_TRANS_ON);
				//SoftFadePfd(fps->pfd, FADE_MAIN_BG, EXIT_BUTTON_COLOR_POS, 1, 
				//	0, EXIT_BUTTON_COLOR_CODE);
				//SoftFadePfd(fps->pfd, FADE_MAIN_OBJ, fps->yameru_pal_pos * 16, 16, 
				//	0, EXIT_BUTTON_COLOR_CODE);
				
				fps->my_comm_status.ready = FALSE;	//�I�������ɓ���̂Ŏ�M�󂯎��Ȃ�
				fps->game_status = FOOTPRINT_GAME_STATUS_FINISH;
				*seq = SEQ_OUT_INIT;
				break;
			case TOUCH_SW_RET_NO:
				TOUCH_SW_Reset(fps->yesno_button);
				fps->yesno_button_use = 0;
				BmpTalkWinClear(&fps->talk_win, WINDOW_TRANS_ON);
				SoftFadePfd(fps->pfd, FADE_MAIN_BG, EXIT_BUTTON_COLOR_POS, 1, 
					0, EXIT_BUTTON_COLOR_CODE);
				SoftFadePfd(fps->pfd, FADE_MAIN_OBJ, fps->yameru_pal_pos * 16, 16, 
					0, EXIT_BUTTON_COLOR_CODE);
				*seq = SEQ_MAIN;
				break;
			}
		}
		break;
	case SEQ_TIMEUP_INIT:
		//��b�E�B���h�E�`��
		GF_BGL_BmpWinDataFill(&fps->talk_win, 0xf);
		BmpTalkWinWrite(&fps->talk_win, WINDOW_TRANS_ON, 
			WINCGX_TALKWIN_START, FOOT_MAINBG_TALKWIN_PAL);
		//���b�Z�[�W�\��
		MSGMAN_GetString(fps->msgman, msg_footprint_timeup, fps->talk_strbuf);
		fps->msg_index = GF_STR_PrintSimple(&fps->talk_win, FONT_TALK, 
			fps->talk_strbuf, 0, 0, 
			CONFIG_GetMsgPrintSpeed(SaveData_GetConfig(fps->sv)), NULL);
		(*seq)++;
		break;
	case SEQ_TIMEUP_MSG_WAIT:
		if(GF_MSG_PrintEndCheck(fps->msg_index) == 0){
			(*seq)++;
		}
		break;
	case SEQ_TIMEUP_WAIT:
		fps->timeup_wait++;
		if(fps->timeup_wait > FOOT_TIMEUP_WAIT){
			*seq = SEQ_OUT_INIT;
		}
		break;
	
	case SEQ_OUT_INIT:
		//�ގ���񑗐M
//		Footprint_Send_PlayerOut();

		if(WIPE_SYS_EndCheck() == FALSE){
			WIPE_SYS_ExeEnd();	//���炭�X�y�V�����G�t�F�N�g�̃t���b�V�����Ȃ̂Ő؂�
		}

		// ���C�v�t�F�[�h�J�n
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK, 
			WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_FOOTPRINT );
		(*seq)++;
		break;
	case SEQ_OUT_WAIT:
		if(WIPE_SYS_EndCheck() == TRUE){
			(*seq)++;
		}
		break;
	default:
		DWC_LOBBY_SUBCHAN_CleanMsgCmd();	//�R�}���h�N���[��
		return PROC_RES_FINISH;
	}

	Footprint_TouchEffUpdate(fps);
	Footprint_InkGaugeUpdate(fps);

	if(fps->game_status == FOOTPRINT_GAME_STATUS_NORMAL){
		//�Q���҃��X�g�X�V����
		fps->entry_list_update_timer++;
		if(fps->entry_list_update_timer > ENTRY_LIST_UPDATE_TIME){
			fps->entry_list_update_timer = 0;
			FootPrintTool_NameAllUpdate(fps);
		}
		
		//�C���N�p���b�g�^�b�`���菈��
		if(((WFLBY_ERR_CheckError() == TRUE) 
				|| (WFLBY_SYSTEM_Event_GetEndCM(fps->parent_work->wflby_sys) == TRUE))
				&& (*seq) != SEQ_EXIT_SELECT_MSG_WAIT){	//���b�Z�[�W�`�撆�͏������Ȃ�
			//WIFI�L��̐������Ԃ�����
			if(fps->yesno_button_use == TRUE){
				//�u�͂��E�������v�{�^�����o�Ă���Ȃ����
				TOUCH_SW_Reset(fps->yesno_button);
			}
			fps->my_comm_status.ready = FALSE;	//�I�������ɓ���̂Ŏ�M�󂯎��Ȃ�
			fps->game_status = FOOTPRINT_GAME_STATUS_FINISH;
			if(WFLBY_ERR_CheckError() == TRUE){	//�ʐM�G���[�ɂ��I��
				(*seq) = SEQ_OUT_INIT;
			}
			else{	//�^�C���A�b�v�ɂ��I��
				Snd_SePlay(FOOTPRINT_SE_TIMEUP);
				WFLBY_SYSTEM_APLFLAG_SetForceEnd( fps->parent_work->wflby_sys );	// �����I���������Ƃ����r�[�V�X�e���ɒʒm2.18 tomoya takahashi
				(*seq) = SEQ_TIMEUP_INIT;
			}
		}
	}

	//�f�o�b�O�J�����ړ�
	Debug_CameraMove(fps);
	
#ifdef PM_DEBUG
	if(sys.trg & PAD_BUTTON_START){
		DebugFoot.occ_seikaku ^= 1;
	}
	if(sys.trg & PAD_BUTTON_SELECT){
		DebugFoot.move_type++;
		if(DebugFoot.move_type >= NELEMS(DebugSeikakuTbl)){
			DebugFoot.move_type = 0;
		}
		OS_TPrintf("�f�o�b�O move_type = %d\n", DebugFoot.move_type);
	}
	if(sys.trg & PAD_BUTTON_Y){
		DebugFoot.consume_zero ^= 1;
	}
#endif

	return PROC_RES_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �v���Z�X�֐��F�I��
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
PROC_RESULT FootPrintProc_End( PROC * proc, int * seq )
{
	FOOTPRINT_SYS * fps  = PROC_GetWork( proc );

	TCB_Delete(fps->update_tcb);

	DefaultActorDel_Main(fps);
	DefaultActorDel_Sub(fps);

	//�{�^���́u�͂��E�������v�폜
	TOUCH_SW_FreeWork(fps->yesno_button);
	
	//�X�^���v�V�X�e���j��
	StampSys_Exit(&fps->ssw);

	STRBUF_Delete(fps->talk_strbuf);
	
	//�t�H���g�폜
	FontProc_UnloadFont(FONT_BUTTON);

	//�t�H���gOAM�V�X�e���폜
	FONTOAM_SysDelete(fps->fontoam_sys);

	// ���b�Z�[�W�}�l�[�W���[�E���[�h�Z�b�g�}�l�[�W���[���
	MSGMAN_Delete( fps->msgman );
	WORDSET_Delete( fps->wordset );
	
	BmpWinDelete( fps );
	
	// BG_SYSTEM���
	BgExit( fps->bgl );
	sys_FreeMemoryEz( fps->bgl );

	//�A�N�^�[�V�X�e���폜
	CATS_ResourceDestructor_S(fps->csp,fps->crp);
	CATS_FreeMemory(fps->csp);

	//�p���b�g�t�F�[�h�V�X�e���폜
	PaletteFadeWorkAllocFree(fps->pfd, FADE_MAIN_BG);
	PaletteFadeWorkAllocFree(fps->pfd, FADE_SUB_BG);
	PaletteFadeWorkAllocFree(fps->pfd, FADE_MAIN_OBJ);
	PaletteFadeWorkAllocFree(fps->pfd, FADE_SUB_OBJ);
	PaletteFadeFree(fps->pfd);

	//3D���f�����
	Model3DDel(fps);
	//�J�����폜
	Footprint_CameraExit(fps);

	//simple_3DBGExit();
	Footprint_3D_Exit(fps->g3Dman);

	//�n���h������
	ArchiveDataHandleClose( fps->handle_footprint );
	ArchiveDataHandleClose( fps->handle_footmark );

	sys_VBlankFuncChange( NULL, NULL );		// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	//Vram�]���}�l�[�W���[�폜
	DellVramTransferManager();

	StopTP();		//�^�b�`�p�l���̏I��

	MsgPrintSkipFlagSet(MSG_SKIP_OFF);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);

	WirelessIconEasyEnd();

#ifdef PM_DEBUG
	if(fps->parent_work->wflby_sys == NULL){
		//�f�o�b�O�Ăяo���̏ꍇ��parent_work�̉�����s��
		sys_FreeMemoryEz(fps->parent_work);
	}
#endif

	PROC_FreeWork( proc );				// PROC���[�N�J��
	sys_DeleteHeap( HEAPID_FOOTPRINT );

	return PROC_RES_FINISH;
}


//==============================================================================
//	
//==============================================================================
static void Footprint_Update(TCB_PTR tcb, void *work)
{
	FOOTPRINT_SYS *fps = work;
	
	StampSys_HitCheck(&fps->ssw);

	Model3D_Update(fps);

	CATS_Draw(fps->crp);
	CATS_UpdateTransfer();
	GF_G3_RequestSwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
	

#ifdef PM_DEBUG		//�|���S���̃��C���Y�I�[�o�[�`�F�b�N
	if(G3X_IsLineBufferUnderflow() != 0){
		OS_TPrintf("--------------���C���Y�I�[�o�[�����I�I----------\n");
		//GF_ASSERT(0 && "���C���Y�I�[�o�[���������܂���");
		G3X_ResetLineBufferUnderflow();
	}
	else if(G3X_GetRenderedLineCount() < 10){
		OS_TPrintf("========== ���C���Y�I�[�o�[�������������ł��c Count = %d\n", 
			G3X_GetRenderedLineCount());
	}
#endif
}

//--------------------------------------------------------------------------------------------
/**
 * VBlank�֐�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VBlankFunc( void * work )
{
	FOOTPRINT_SYS *fps = work;
	
	StampSys_VWaitUpdate(&fps->ssw, fps->game_status);

	// �Z���A�N�^�[Vram�]���}�l�[�W���[���s
	DoVramTransferManager();

	// �����_�����LOAM�}�l�[�W��Vram�]��
	CATS_RenderOamTrans();
	PaletteFadeTrans(fps->pfd);
	
	GF_BGL_VBlankFunc(fps->bgl);

	OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}

//--------------------------------------------------------------
/**
 * @brief   Vram�o���N�ݒ���s��
 *
 * @param   bgl		BGL�f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void FootPrint_VramBankSet(GF_BGL_INI *bgl)
{
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	
	//VRAM�ݒ�
	{
		GF_BGL_DISPVRAM vramSetTable = {
			GX_VRAM_BG_128_C,				// ���C��2D�G���W����BG
			GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g

			GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
			GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g

			GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
			GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g

			GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
			GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g

			GX_VRAM_TEX_01_AB,				// �e�N�X�`���C���[�W�X���b�g
			GX_VRAM_TEXPLTT_01_FG			// �e�N�X�`���p���b�g�X���b�g
		};
		GF_Disp_SetBank( &vramSetTable );

		//VRAM�N���A
		MI_CpuClear32((void*)HW_BG_VRAM, HW_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
	}

	// BG SYSTEM
	{
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	//���C����ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FOOT_FRAME_WIN	�E�B���h�E��
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
				FOOT_BGPRI_WIN, 0, 0, FALSE
			},
			///<FOOT_FRAME_PANEL	�p�l����
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
				FOOT_BGPRI_PANEL, 0, 0, FALSE
			},
			///<FOOT_FRAME_BG	�w�i
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
				FOOT_BGPRI_BG, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, FOOT_FRAME_WIN, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FOOT_FRAME_WIN );
		GF_BGL_ScrollSet(bgl, FOOT_FRAME_WIN, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FOOT_FRAME_WIN, GF_BGL_SCROLL_Y_SET, 0);

		GF_BGL_BGControlSet(bgl, FOOT_FRAME_PANEL, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FOOT_FRAME_PANEL );
		GF_BGL_ScrollSet(bgl, FOOT_FRAME_PANEL, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FOOT_FRAME_PANEL, GF_BGL_SCROLL_Y_SET, 0);
		
		GF_BGL_BGControlSet(bgl, FOOT_FRAME_BG, &TextBgCntDat[2], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FOOT_FRAME_BG );
		GF_BGL_ScrollSet(bgl, FOOT_FRAME_BG, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FOOT_FRAME_BG, GF_BGL_SCROLL_Y_SET, 0);

		//3D��
		G2_SetBG0Priority(FOOT_BGPRI_3D);
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
	}
	//�T�u��ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FOOT_SUBFRAME_WIN	�e�L�X�g��
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
				FOOT_SUBBGPRI_WIN, 0, 0, FALSE
			},
			///<FOOT_SUBFRAME_PLATE	�v���[�g
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
				FOOT_SUBBGPRI_PLATE, 0, 0, FALSE
			},
			///<FOOT_SUBFRAME_BG	�w�i
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x6800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
				FOOT_SUBBGPRI_BG, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, FOOT_SUBFRAME_WIN, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FOOT_SUBFRAME_WIN );
		GF_BGL_ScrollSet(bgl, FOOT_SUBFRAME_WIN, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FOOT_SUBFRAME_WIN, GF_BGL_SCROLL_Y_SET, 0);
		
		GF_BGL_BGControlSet(bgl, FOOT_SUBFRAME_PLATE, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FOOT_SUBFRAME_PLATE );
		GF_BGL_ScrollSet(bgl, FOOT_SUBFRAME_PLATE, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FOOT_SUBFRAME_PLATE, GF_BGL_SCROLL_Y_SET, 0);

		GF_BGL_BGControlSet(bgl, FOOT_SUBFRAME_BG, &TextBgCntDat[2], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FOOT_SUBFRAME_BG );
		GF_BGL_ScrollSet(bgl, FOOT_SUBFRAME_BG, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FOOT_SUBFRAME_BG, GF_BGL_SCROLL_Y_SET, 0);
	}

	GF_BGL_ClearCharSet( FOOT_SUBFRAME_WIN, 32, 0, HEAPID_FOOTPRINT );
}

//--------------------------------------------------------------------------------------------
/**
 * BG���
 *
 * @param	ini		BGL�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgExit( GF_BGL_INI * ini )
{
	GF_BGL_BGControlExit( ini, FOOT_SUBFRAME_PLATE );
	GF_BGL_BGControlExit( ini, FOOT_SUBFRAME_WIN );
	GF_BGL_BGControlExit( ini, FOOT_SUBFRAME_BG );
	GF_BGL_BGControlExit( ini, FOOT_FRAME_BG );
	GF_BGL_BGControlExit( ini, FOOT_FRAME_PANEL );
	GF_BGL_BGControlExit( ini, FOOT_FRAME_WIN );

}

//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^�Z�b�g
 *
 * @param	fps		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicSet( FOOTPRINT_SYS * fps, ARCHANDLE* p_handle )
{
	GF_BGL_INI *bgl = fps->bgl;
	u16 *panel_scrn;
	
	//-- ���C����� --//
	PaletteWorkSet_Arc(fps->pfd, ARC_FOOTPRINT_GRA, NARC_footprint_board_a_board_sita_NCLR, 
		HEAPID_FOOTPRINT, FADE_MAIN_BG, 0x200-0x40, 0);
	ArcUtil_HDL_BgCharSet(p_handle, NARC_footprint_board_a_board_sita_NCGR, fps->bgl, 
		FOOT_FRAME_PANEL, 0, 0, 0, HEAPID_FOOTPRINT);
	ArcUtil_HDL_ScrnSet(p_handle, NARC_footprint_board_a_board_sita_NSCR, fps->bgl, 
		FOOT_FRAME_PANEL, 0, 0, 0, HEAPID_FOOTPRINT);
	ArcUtil_HDL_ScrnSet(p_handle, NARC_footprint_board_a_board_sita_bg_NSCR, fps->bgl, 
		FOOT_FRAME_BG, 0, 0, 0, HEAPID_FOOTPRINT);

	//-- �T�u��� --//
	PaletteWorkSet_Arc(fps->pfd, ARC_FOOTPRINT_GRA, NARC_footprint_board_ashiato_board_NCLR, 
		HEAPID_FOOTPRINT, FADE_SUB_BG, 0, 0);
	if(fps->parent_work->board_type == FOOTPRINT_BOARD_TYPE_WHITE){
		PaletteWorkCopy(fps->pfd, FADE_SUB_BG, 16*1, FADE_SUB_BG, 16*0, 0x20);
	}
	ArcUtil_HDL_BgCharSet(p_handle, NARC_footprint_board_ashiato_board_NCGR, fps->bgl, 
		FOOT_SUBFRAME_PLATE, 0, 0, 0, HEAPID_FOOTPRINT);
	ArcUtil_HDL_ScrnSet(p_handle, NARC_footprint_board_ashiato_board_NSCR, fps->bgl, 
		FOOT_SUBFRAME_PLATE, 0, 0, 0, HEAPID_FOOTPRINT);
	ArcUtil_HDL_ScrnSet(p_handle, NARC_footprint_board_ashiato_board_bg_NSCR, fps->bgl, 
		FOOT_SUBFRAME_BG, 0, 0, 0, HEAPID_FOOTPRINT);
	panel_scrn = GF_BGL_ScreenAdrsGet(fps->bgl, FOOT_SUBFRAME_PLATE);
	MI_CpuCopy16(panel_scrn, fps->namelist_scrn, NAMELIST_SCRN_SIZE);
	MI_CpuClear16(panel_scrn, NAMELIST_SCRN_SIZE);
	
	{
		int win_type;
	
		win_type = CONFIG_GetWindowType(SaveData_GetConfig(fps->sv));
		
		// ��b�E�B���h�E�p���b�g�]��
		PaletteWorkSet_Arc(fps->pfd, ARC_WINFRAME, TalkWinPalArcGet(win_type), HEAPID_FOOTPRINT, 
			FADE_MAIN_BG, 0x20, FOOT_MAINBG_TALKWIN_PAL * 16);
		// ��b�E�C���h�E�O���t�B�b�N�]��
		TalkWinGraphicSet(fps->bgl, FOOT_FRAME_WIN, WINCGX_TALKWIN_START, 
			FOOT_MAINBG_TALKWIN_PAL,  win_type, HEAPID_FOOTPRINT);

		//�V�X�e���t�H���g�p���b�g�]��
		PaletteWorkSet_Arc(fps->pfd, ARC_FONT, NARC_font_system_ncrl, HEAPID_FOOTPRINT, 
			FADE_MAIN_BG, 0x20, FOOT_MAINBG_TALKFONT_PAL * 16);
		if(fps->parent_work->board_type == FOOTPRINT_BOARD_TYPE_WHITE){
			PaletteWorkSet_Arc(fps->pfd, ARC_FONT, NARC_font_system_ncrl, HEAPID_FOOTPRINT, 
				FADE_SUB_BG, 0x20, FOOT_SUBBG_TALKFONT_PAL * 16);
		}
		else{
			PaletteWorkSet_Arc(fps->pfd, ARC_FOOTPRINT_GRA, 
				NARC_footprint_board_a_board_font_s_NCLR, HEAPID_FOOTPRINT, 
				FADE_SUB_BG, 0x20, FOOT_SUBBG_TALKFONT_PAL * 16);
		}
	}

	GF_BGL_LoadScreenV_Req(fps->bgl, FOOT_SUBFRAME_PLATE);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓OBJ�p���\�[�X�̃Z�b�g
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_Main(FOOTPRINT_SYS *fps, ARCHANDLE *hdl_main)
{
	int i, pal_pos;
	u16 color_code;
	
	//-- ���C�����OBJ�풓�p���b�g --//
	CATS_LoadResourcePlttWorkArcH(fps->pfd, FADE_MAIN_OBJ, fps->csp, fps->crp, 
		hdl_main, NARC_footprint_board_a_board_eff_NCLR, 0, FOOTPRINT_COMMON_PAL_NUM, 
		NNS_G2D_VRAM_TYPE_2DMAIN, PLTTID_OBJ_COMMON);

	//-- �C���N(�C���N�̉��n������) --//
	CATS_LoadResourceCharArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_ashiato_gage_NCGR, 0, NNS_G2D_VRAM_TYPE_2DMAIN, CHARID_INK);
	CATS_LoadResourceCellArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_ashiato_gage_NCER, 0, CELLID_INK);
	CATS_LoadResourceCellAnmArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_ashiato_gage_NANR, 0, CELLANMID_INK);

	//-- �C���N�̏�ɔz�u���鑫�� --//
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		CATS_LoadResourceCharArcH(fps->csp, fps->crp, hdl_main, 
			NARC_footprint_board_wifi_mark_NCGR, 0, 
			NNS_G2D_VRAM_TYPE_2DMAIN, CHARID_INK_FOOT_0 + i);
	}
	CATS_LoadResourceCellArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_wifi_mark_NCER, 0, CELLID_INK_FOOT);
	CATS_LoadResourceCellAnmArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_wifi_mark_NANR, 0, CELLANMID_INK_FOOT);
	//�C���N�̏�ɔz�u���鑫�Ղ̃p���b�g(�f�[�^�̓_�~�[�B�̈悾���m��)
	//�_�~�[�O���t�B�b�N��7�A���Ճf�[�^��4�Ԃ̃J���[�ŏ�����Ă���̂ŁA
	//��{�m�ۂ��āA���՗p�Ɋ��蓖�ĂĂ��܂��B�S�ẴJ���[�𑫐Ղ̐F�Ŗ��߂�
	pal_pos = CATS_LoadResourcePlttWorkArcH(fps->pfd, FADE_MAIN_OBJ, fps->csp, fps->crp, 
		hdl_main, NARC_footprint_board_a_board_eff_NCLR, 0, 1, 
		NNS_G2D_VRAM_TYPE_2DMAIN, PLTTID_OBJ_INK_FOOT);
	if(fps->parent_work->board_type == FOOTPRINT_BOARD_TYPE_WHITE){
		color_code = INKPAL_FOOT_COLOR_CODE_WHITE;
	}
	else{
		color_code = INKPAL_FOOT_COLOR_CODE_BLACK;
	}
	PaletteWork_Clear(fps->pfd, FADE_MAIN_OBJ, FADEBUF_ALL, color_code, 
		pal_pos * 16, pal_pos * 16 + 16);
	
	//-- �^�b�`�G�t�F�N�g --//
	CATS_LoadResourceCharArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_a_board_eff_NCGR, 0, NNS_G2D_VRAM_TYPE_2DMAIN, CHARID_TOUCH_EFF);
	CATS_LoadResourceCellArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_a_board_eff_NCER, 0, CELLID_TOUCH_EFF);
	CATS_LoadResourceCellAnmArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_a_board_eff_NANR, 0, CELLANMID_TOUCH_EFF);
		
	//-- FONTOAM --//
	fps->yameru_pal_pos = CATS_LoadResourcePlttWorkArc(
		fps->pfd, FADE_MAIN_OBJ, fps->csp, fps->crp, 
		ARC_FOOTPRINT_GRA, NARC_footprint_board_a_board_font_b_NCLR, 0, 1, 
		NNS_G2D_VRAM_TYPE_2DMAIN, PLTTID_OBJ_FONTOAM);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[�o�^
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void DefaultActorSet_Main(FOOTPRINT_SYS *fps)
{
	int i;
	TCATS_OBJECT_ADD_PARAM_S head;
	
	//-- �C���N --//
	head = InkObjParam;
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		head.x = INK_POS_START_X + INK_POS_SPACE_X * i;
		head.y = INK_POS_Y;
		fps->cap_ink[i] = CATS_ObjectAdd_S(fps->csp, fps->crp, &head);
		CATS_ObjectAnimeSeqSetCap(fps->cap_ink[i], i);
		CATS_ObjectUpdate(fps->cap_ink[i]->act);
	}

	//-- �C���N�̉��n --//
	head = InkFoundationObjParam;
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		head.x = INK_FOUNDATION_POS_START_X + INK_FOUNDATION_POS_SPACE_X * i;
		head.y = INK_FOUNDATION_POS_Y;
		fps->cap_ink_foundation[i] = CATS_ObjectAdd_S(fps->csp, fps->crp, &head);
		CATS_ObjectAnimeSeqSetCap(fps->cap_ink_foundation[i], i);
		CATS_ObjectUpdate(fps->cap_ink_foundation[i]->act);
	}
	
	//-- �C���N�̏�ɔz�u���鑫�� --//
	head = InkFootObjParam;
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		head.x = INK_FOOT_POS_START_X + INK_FOOT_POS_SPACE_X * i;
		head.y = INK_FOOT_POS_Y;
		head.id[CLACT_U_CHAR_RES] = CHARID_INK_FOOT_0 + i;
		fps->cap_ink_foot[i] = CATS_ObjectAdd_S(fps->csp, fps->crp, &head);
		CATS_ObjectUpdate(fps->cap_ink_foot[i]->act);
	}

	//-- �u��߂�vFONTOAM --//
	{
		STRBUF *str_ptr;
		
		str_ptr = MSGMAN_AllocString(fps->msgman, msg_footprint_exit);
		
		Sub_FontOamCreate(fps, &fps->fontoam_exit, str_ptr, FONT_BUTTON,
			GF_PRINTCOLOR_MAKE(1,2,3), 0, 
			PLTTID_OBJ_FONTOAM, 0x1c * 8, 176, FONTOAM_CENTER);
		FONTOAM_SetDrawFlag(fps->fontoam_exit.fontoam, TRUE);
		
		STRBUF_Delete(str_ptr);
	}
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[���폜����
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void DefaultActorDel_Main(FOOTPRINT_SYS *fps)
{
	int i;
	
	//-- �C���N --//
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		CATS_ActorPointerDelete_S(fps->cap_ink[i]);
	}
	
	//-- �C���N�̉��n --//
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		CATS_ActorPointerDelete_S(fps->cap_ink_foundation[i]);
	}

	//-- �C���N�̏�ɔz�u���鑫�� --//
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		CATS_ActorPointerDelete_S(fps->cap_ink_foot[i]);
	}

	//-- FONTOAM --//
	Sub_FontOamDelete(&fps->fontoam_exit);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̃Z�b�g
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_Sub(FOOTPRINT_SYS *fps, ARCHANDLE *hdl_main)
{
	int i;
	
	//-- �T�u���OBJ�풓�p���b�g --//
	CATS_LoadResourcePlttWorkArcH(fps->pfd, FADE_SUB_OBJ, fps->csp, fps->crp, 
		hdl_main, NARC_footprint_board_ashiato_frame_NCLR, 0, FOOTPRINT_SUB_COMMON_PAL_NUM, 
		NNS_G2D_VRAM_TYPE_2DSUB, PLTTID_SUB_OBJ_COMMON);

	//-- ���O���͂ރt���[�� --//
	CATS_LoadResourceCharArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_ashiato_frame_NCGR, 0, NNS_G2D_VRAM_TYPE_2DSUB, 
		CHARID_SUB_NAME_FRAME);
	CATS_LoadResourceCellArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_ashiato_frame_NCER, 0, CELLID_SUB_NAME_FRAME);
	CATS_LoadResourceCellAnmArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_ashiato_frame_NANR, 0, CELLANMID_SUB_NAME_FRAME);
		
	//-- ���O�̉��̑��� --//
	for(i = 0; i < FOOTPRINT_ENTRY_MAX; i++){
		CATS_LoadResourceCharArcH(fps->csp, fps->crp, hdl_main, 
			NARC_footprint_board_foot_dummy_NCGR, 0, NNS_G2D_VRAM_TYPE_2DSUB, 
			CHARID_SUB_NAME_FOOT_0 + i);
	}
	CATS_LoadResourceCellArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_foot_dummy_NCER, 0, CELLID_SUB_NAME_FOOT);
	CATS_LoadResourceCellAnmArcH(fps->csp, fps->crp, hdl_main, 
		NARC_footprint_board_foot_dummy_NANR, 0, CELLANMID_SUB_NAME_FOOT);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[�o�^
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void DefaultActorSet_Sub(FOOTPRINT_SYS *fps)
{
	int i;
	TCATS_OBJECT_ADD_PARAM_S head;
	
	//-- ���O���͂ޘg --//
	fps->cap_name_frame = CATS_ObjectAdd_S(fps->csp, fps->crp, &NameFrameObjParam);
	CATS_ObjectUpdate(fps->cap_name_frame->act);
	CATS_ObjectEnableCap(fps->cap_name_frame, CATS_ENABLE_FALSE);	//�ŏ��͔�\��

	//-- ���O�̉��̑��� --//
	head = NameFootObjParam;
	for(i = 0; i < FOOTPRINT_ENTRY_MAX; i++){
		head.id[CLACT_U_CHAR_RES] = CHARID_SUB_NAME_FOOT_0 + i;
		fps->cap_name_foot[i] = CATS_ObjectAdd_S(fps->csp, fps->crp, &head);
		CATS_ObjectPosSetCap_SubSurface(fps->cap_name_foot[i], 
			Sub_FootmarkPos[i][0], Sub_FootmarkPos[i][1], FOOTPRINT_SUB_ACTOR_DISTANCE);
		CATS_ObjectUpdate(fps->cap_name_foot[i]->act);
		CATS_ObjectEnableCap(fps->cap_name_foot[i], CATS_ENABLE_FALSE);	//�ŏ��͔�\��
	}
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[���폜����
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void DefaultActorDel_Sub(FOOTPRINT_SYS *fps)
{
	int i;
	
	//-- ���O���͂ޘg --//
	CATS_ActorPointerDelete_S(fps->cap_name_frame);
	
	//-- ���O�̉��̑��� --//
	for(i = 0; i < FOOTPRINT_ENTRY_MAX; i++){
		CATS_ActorPointerDelete_S(fps->cap_name_foot[i]);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �����̃C���N�p���b�g�̏����ݒ���s��
 *
 * @param  fps
 */
//--------------------------------------------------------------
static void MyInkPaletteSettings(FOOTPRINT_SYS *fps)
{
	int i, y;
	u16 *def_pal, *trans_pal;
	int first_hit = 0;
	
	def_pal = PaletteWorkDefaultWorkGet(fps->pfd, FADE_MAIN_OBJ);
	trans_pal = PaletteWorkTransWorkGet(fps->pfd, FADE_MAIN_OBJ);
	
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		if(fps->my_stamp_param[i].monsno == 0 || fps->my_stamp_param[i].monsno > MONSNO_END){
			CATS_ObjectEnableCap(fps->cap_ink[i], CATS_ENABLE_FALSE);
			CATS_ObjectEnableCap(fps->cap_ink_foundation[i], CATS_ENABLE_FALSE);
			CATS_ObjectEnableCap(fps->cap_ink_foot[i], CATS_ENABLE_FALSE);

			//BG�̃C���N�̘g������
			for(y = 0; y < SCRN_INK_POS_SIZE_Y; y++){
				GF_BGL_ScrFill(fps->bgl, FOOT_FRAME_PANEL, MyInkPaletteEraseScrnCode[y], 
					SCRN_INK_POS_START_X + SCRN_INK_POS_SIZE_X * i, SCRN_INK_POS_Y + y,
					SCRN_INK_POS_SIZE_X, 1, GF_BGL_SCRWRT_PALIN);
			}
		}
		else{
			//���Ղ̃L����������������
			OBJFootCharRewrite(fps->my_stamp_param[i].monsno, fps->my_stamp_param[i].form_no, 
				fps->cap_ink_foot[i], fps->handle_footprint,
				fps->handle_footmark, NNS_G2D_VRAM_TYPE_2DMAIN, fps->arceus_flg);
			
			//�C���N�̃J���[��ύX����
			def_pal[PALOFS_INK * 16 + COLOR_NO_INK_START + i] = fps->my_stamp_param[i].color;
			trans_pal[PALOFS_INK * 16 + COLOR_NO_INK_START + i] = fps->my_stamp_param[i].color;
			//�C���N�̉��n��ύX����(�����ۂ�����)
			SoftFade(&def_pal[PALOFS_INK * 16 + COLOR_NO_INK_START + i],
				&def_pal[PALOFS_INK_FOUNDATION * 16 + COLOR_NO_INK_START + i],
				1, INK_FOUNDATION_EVY, INK_FOUNDATION_COLOR);
			trans_pal[PALOFS_INK_FOUNDATION * 16 + COLOR_NO_INK_START + i]
				= def_pal[PALOFS_INK_FOUNDATION * 16 + COLOR_NO_INK_START + i];
			
			if(first_hit == 0){
				first_hit++;
				fps->select_no = i;
			}
		}
	}
	Footprint_SelectInkPaletteFade(fps, 0);
	GF_BGL_LoadScreenV_Req(fps->bgl, FOOT_FRAME_PANEL);
}

//--------------------------------------------------------------
/**
 * @brief   OBJ�̑��Ճ}�[�N������������
 *
 * @param   monsno			�|�P�����ԍ�
 * @param   cap				���������Ώۂ̃A�N�^�[�ւ̃|�C���^
 * @param   hdl_mark		���ՃO���t�B�b�N�̃n���h��
 * @param   NNS_G2D_VRAM_TYPE	NNS_G2D_VRAM_TYPE_2DMAIN or NNS_G2D_VRAM_TYPE_2DSUB
 * @param   arceus_flg		�A���Z�E�X���J�t���O(TRUE:���JOK)
 *
 * @retval  TRUE:����������
 * @retval  FALSE:���������Ȃ�����
 */
//--------------------------------------------------------------
static BOOL OBJFootCharRewrite(int monsno, int form_no, CATS_ACT_PTR cap, ARCHANDLE *hdl_main, ARCHANDLE *hdl_mark, NNS_G2D_VRAM_TYPE vram_type, BOOL arceus_flg)
{
	void *obj_vram;
	NNSG2dImageProxy * image;
	void *pSrc;
	NNSG2dCharacterData *pChar;
	u8 *read_up, *read_bottom;
	
	if(monsno == 0 || monsno > MONSNO_END){
		return FALSE;
	}
	
	OS_TPrintf("��������monsno = %d\n", monsno);
	if(FootprintTool_FootDispCheck(monsno, form_no, arceus_flg) == TRUE){
		pSrc = ArcUtil_HDL_Load(hdl_mark, POKEFOOT_ARC_CHAR_DMMY + monsno, //����
			TRUE, HEAPID_FOOTPRINT, ALLOC_BOTTOM);
		NNS_G2dGetUnpackedCharacterData(pSrc, &pChar);
		DC_FlushRange(pChar->pRawData, 0x20 * 8);
		read_up = &((u8*)pChar->pRawData)[0x20 * 4];	//128K�}�b�s���O�ŋ󔒂������Ă���̂�
		read_bottom = pChar->pRawData;
	}
	else{	//���Ղ������̂�Wifi�}�[�N
		pSrc = ArcUtil_HDL_Load(hdl_main, NARC_footprint_board_wifi_mark_NCGR, //WIFI�}�[�N
			FALSE, HEAPID_FOOTPRINT, ALLOC_BOTTOM);
		NNS_G2dGetUnpackedCharacterData(pSrc, &pChar);
		DC_FlushRange(pChar->pRawData, 0x20 * 4);
		read_up = pChar->pRawData;
		read_bottom = &((u8*)pChar->pRawData)[0x20 * 2];
	}
	
	if(vram_type == NNS_G2D_VRAM_TYPE_2DMAIN){
		obj_vram = G2_GetOBJCharPtr();
	}
	else{
		obj_vram = G2S_GetOBJCharPtr();
	}
	image = CLACT_ImageProxyGet(cap->act);
	
	//��������
	MI_CpuCopy16(read_up, (void*)((u32)obj_vram
		+ image->vramLocation.baseAddrOfVram[vram_type]), 
		0x20 * 2);
	MI_CpuCopy16(read_bottom, (void*)((u32)obj_vram + 0x20*2
		+ image->vramLocation.baseAddrOfVram[vram_type]), 
		0x20 * 2);
	
	sys_FreeMemoryEz(pSrc);
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �J�����쐬
 *
 * @param   fps		�Q�[�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Footprint_CameraInit(FOOTPRINT_SYS *fps)
{
	VecFx32	target = { FOOTPRINT_CAMERA_TX, FOOTPRINT_CAMERA_TY, FOOTPRINT_CAMERA_TZ };

	fps->camera = GFC_AllocCamera( HEAPID_FOOTPRINT );

	GFC_InitCameraTDA(&target, FOOTPRINT_CAMERA_DISTANCE, &FootprintCameraAngle,
						FOOTPRINT_CAMERA_PERSPWAY, FOOTPRINT_CAMERA_MODE, FALSE, fps->camera);

	GFC_SetCameraClip( FOOTPRINT_CAMERA_NEAR, FOOTPRINT_CAMERA_FAR, fps->camera);
	
	GFC_AttachCamera(fps->camera);
	
	{//�ݒ肵���J�����ʒu�ł́A���[���h��Ԃ͈̔͂��擾����
		u16 persp_way;
		fx32 distance, aspect;
		fx32 width, height;
		
		persp_way = GFC_GetCameraPerspWay(fps->camera);
		distance = GFC_GetCameraDistance(fps->camera);
		aspect = FX32_ONE * 4 / 3;
		GetPerspectiveScreenSize(persp_way, distance, aspect, &width, &height);
		OS_TPrintf("width = %d(%x), height = %d(%x)\n", width, width, height, height);
		
		fps->world_width = width;
		fps->world_height = height;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �J�������
 *
 * @param   fps		�Q�[�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Footprint_CameraExit(FOOTPRINT_SYS *fps)
{
	GFC_FreeCamera(fps->camera);
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���Z�b�g
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void Model3DSet( FOOTPRINT_SYS * fps, ARCHANDLE* p_handle )
{
	int data_id;
	
	if(fps->parent_work->board_type == FOOTPRINT_BOARD_TYPE_WHITE){
		data_id = NARC_footprint_board_a_board2_nsbmd;
	}
	else{
		data_id = NARC_footprint_board_a_board1_nsbmd;
	}
	
    //���f���f�[�^�ǂݍ���
	D3DOBJ_MdlLoadH(&fps->board.mdl, p_handle, data_id, HEAPID_FOOTPRINT);

    //�����_�[�I�u�W�F�N�g�ɓo�^
    D3DOBJ_Init( &fps->board.obj, &fps->board.mdl );

    //���W�ݒ�
    D3DOBJ_SetMatrix( &fps->board.obj, BOARD_X, BOARD_Y, BOARD_Z);
    D3DOBJ_SetScale(&fps->board.obj, BOARD_SCALE, BOARD_SCALE, BOARD_SCALE);
    D3DOBJ_SetDraw( &fps->board.obj, TRUE );
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���폜
 *
 * @param   fps		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void Model3DDel(FOOTPRINT_SYS *fps)
{
    // �S���\�[�X�j��
    D3DOBJ_MdlDelete( &fps->board.mdl ); 
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���X�V
 *
 * @param   fps		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void Model3D_Update(FOOTPRINT_SYS *fps)
{
	VecFx32 scale_vec, trans;
	MtxFx33 rot;
	
	scale_vec.x = FX32_ONE;
	scale_vec.y = FX32_ONE;
	scale_vec.z = FX32_ONE;
	
	trans.x = 0;
	trans.y = 0;
	trans.z = 0;
	
	MTX_Identity33(&rot);

	//�R�c�`��J�n
	GF_G3X_Reset();
	
	GFC_AttachCamera(fps->camera);
	GFC_SetCameraView(FOOTPRINT_CAMERA_MODE, fps->camera); //���ˉe�ݒ�
	GFC_CameraLookAt();

	// ���C�g�ƃA���r�G���g
	NNS_G3dGlbLightVector( 0, 0, -FX32_ONE, 0 );
	NNS_G3dGlbLightColor( 0, GX_RGB( 28,28,28 ) );
	NNS_G3dGlbMaterialColorDiffAmb( GX_RGB( 31,31,31 ), GX_RGB( 31,31,31 ), FALSE );
	NNS_G3dGlbMaterialColorSpecEmi( GX_RGB( 31,31,31 ), GX_RGB( 31,31,31 ), FALSE );
	
	// �ʒu�ݒ�
	NNS_G3dGlbSetBaseTrans(&trans);
	// �p�x�ݒ�
	NNS_G3dGlbSetBaseRot(&rot);
	// �X�P�[���ݒ�
	NNS_G3dGlbSetBaseScale(&scale_vec);

//	NNS_G3dGlbFlush();
	
	// �`��
	NNS_G3dGePushMtx();
	{
		//�{�[�h
		D3DOBJ_Draw( &fps->board.obj );
		//�X�^���v
		StampSys_ObjDraw(&fps->ssw);
	}
	NNS_G3dGePopMtx(1);
}

//------------------------------------------------------------------
/**
 * BMPWIN�����i�����p�l���Ƀt�H���g�`��j
 *
 * @param   fps		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void BmpWinInit( FOOTPRINT_SYS *fps )
{
	int i;
	
	//-- ���C����� --//
	GF_BGL_BmpWinAdd(fps->bgl, &fps->talk_win, FOOT_FRAME_WIN,
		2, 1, 27, 4, FOOT_MAINBG_TALKFONT_PAL, WINCGX_MESSAGE_START);
	GF_BGL_BmpWinDataFill(&fps->talk_win, 0xf);
	
	//-- �T�u��� --//
	//�Q���҂̖��O�\��
	for(i = 0; i < FOOTPRINT_BMPWIN_NAME_MAX; i++){
		GF_BGL_BmpWinAdd(fps->bgl, &fps->name_win[i], FOOT_SUBFRAME_WIN,
			NameBmpwinPos[i][0], NameBmpwinPos[i][1], 
			WINCGX_SUB_NAME_SIZE_X, WINCGX_SUB_NAME_SIZE_Y,
			FOOT_SUBBG_TALKFONT_PAL, 
			WINCGX_SUB_NAME_START + (WINCGX_SUB_NAME_SIZE_X * WINCGX_SUB_NAME_SIZE_Y) * i);
		GF_BGL_BmpWinDataFill(&fps->name_win[i], 0x00);
	}
}

//------------------------------------------------------------------
/**
 * $brief   �m�ۂ���BMPWIN�����
 *
 * @param   fps		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void BmpWinDelete( FOOTPRINT_SYS *fps )
{
	int i;
	
	//-- ���C����� --//
	GF_BGL_BmpWinDel(&fps->talk_win);
	
	//-- �T�u��� --//
	for(i = 0; i < FOOTPRINT_BMPWIN_NAME_MAX; i++){
		GF_BGL_BmpWinDel(&fps->name_win[i]);
	}
}

//--------------------------------------------------------------
/**
 * @brief   ���Ճ{�[�h�p3DBG�������֐�
 * 
 * @param   �q�[�vID
 */
//--------------------------------------------------------------
static GF_G3DMAN * Footprint_3D_Init(int heap_id)
{
	GF_G3DMAN *g3Dman;
	
	g3Dman = GF_G3DMAN_Init(heap_id, GF_G3DMAN_LNK, GF_G3DTEX_256K, 
		GF_G3DMAN_LNK, GF_G3DPLT_32K, FootprintSimpleSetUp);
	return g3Dman;
}

static void FootprintSimpleSetUp(void)
{
	// �R�c�g�p�ʂ̐ݒ�(�\�����v���C�I���e�B�[)
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    G2_SetBG0Priority(1);

	// �e��`�惂�[�h�̐ݒ�(�V�F�[�h���A���`�G�C���A�X��������)
    G3X_SetShading( GX_SHADING_TOON );
    G3X_AntiAlias( TRUE );
	G3X_AlphaTest( FALSE, 0 );	// �A���t�@�e�X�g�@�@�I�t
	G3X_AlphaBlend( TRUE );		// �A���t�@�u�����h�@�I��
	G3X_EdgeMarking( FALSE );
	G3X_SetFog( FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0 );

	// �N���A�J���[�̐ݒ�
    G3X_SetClearColor(GX_RGB(0,0,0),0,0x7fff,63,FALSE);	//color,alpha,depth,polygonID,fog

	// �r���[�|�[�g�̐ݒ�
    G3_ViewPort(0, 0, 255, 191);
}

//--------------------------------------------------------------
/**
 * @brief   ���Ճ{�[�h�p3DBG�I������
 *
 * @param   g3Dman		
 */
//--------------------------------------------------------------
static void Footprint_3D_Exit(GF_G3DMAN *g3Dman)
{
	GF_G3D_Exit(g3Dman);
}

//--------------------------------------------------------------
/**
 * @brief   �����̒ʐM�X�e�[�^�X���Z�b�g����
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void Footprint_MyCommStatusSet(FOOTPRINT_SYS *fps)
{
	FOOTPRINT_MY_COMM_STATUS *my_comm;
	
	my_comm = &fps->my_comm_status;
	
	my_comm->user_id = DWC_LOBBY_GetMyUserID();
}

//--------------------------------------------------------------
/**
 * @brief   FOOTPRINT_IN_PARAM�̒��g���쐬����
 *
 * @param   fps			
 * @param   in_para		�f�[�^�Z�b�g��
 */
//--------------------------------------------------------------
static void Footprint_InParamCreate(FOOTPRINT_SYS_PTR fps, FOOTPRINT_IN_PARAM *in_para)
{
	STAMP_PARAM *stamp;
	
	MI_CpuClear8(in_para, sizeof(FOOTPRINT_IN_PARAM));
	stamp = &in_para->stamp;
	
	//�ŏ��ɖ��O�̉��ɕ\�������鑫�Ղ̏�Ԃ��Z�b�g����
	//�擪�ɂ���|�P�����̑��Ղ��Z�b�g�B
	//�^�}�S�̏ꍇ�͔�΂�
	stamp->monsno = 150;
	stamp->color = 0x001f;
	stamp->personal_rnd = 0x0101;
	stamp->form_no = 0;
}

//--------------------------------------------------------------
/**
 * @brief   �����̒ʐM�X�e�[�^�X���擾����
 * @param   fps		
 * @retval  �ʐM�X�e�[�^�X�ւ̃|�C���^
 */
//--------------------------------------------------------------
FOOTPRINT_MY_COMM_STATUS * Footprint_MyCommStatusGet(FOOTPRINT_SYS_PTR fps)
{
	return &fps->my_comm_status;
}

//--------------------------------------------------------------
/**
 * @brief   �莝���|�P�������X�^���v�p�����[�^�ɃZ�b�g����
 *
 * @param   board_type		�{�[�h�̎��(FOOTPRINT_BOARD_TYPE_???)
 * @param   sv				�Z�[�u�f�[�^�ւ̃|�C���^
 * @param   stamp_array		�����(�X�^���v�p�����[�^�̔z��ւ̃|�C���^)
 */
//--------------------------------------------------------------
static void Footprint_Temoti_to_StampParam(int board_type, SAVEDATA * sv, STAMP_PARAM *stamp_array)
{
	POKEPARTY *party;
	int poke_max;
	POKEMON_PARAM *pp;
	int i;
	
	MI_CpuClear8(stamp_array, sizeof(STAMP_PARAM) * POKEMON_TEMOTI_MAX);
	
	party = SaveData_GetTemotiPokemon(sv);
	poke_max = PokeParty_GetPokeCount(party);
	for(i = 0; i < poke_max; i++){
		pp = PokeParty_GetMemberPointer(party, i);
		stamp_array[i].monsno = PokeParaGet(pp, ID_PARA_monsno_egg, NULL);
		stamp_array[i].personal_rnd = PokeParaGet(pp, ID_PARA_personal_rnd, NULL);
		stamp_array[i].form_no = PokeParaGet(pp, ID_PARA_form_no, NULL);
		stamp_array[i].color 
			= FootprintTool_StampColorGet(board_type, PokeParaGet(pp, ID_PARA_id_no, NULL));
	#ifdef PM_DEBUG
		if(sys.cont & PAD_BUTTON_B){	//�����_���ŐF��ς���
			stamp_array[i].color = FootprintTool_StampColorGet(board_type, gf_rand());
		}
	#endif
	}
}

//--------------------------------------------------------------
/**
 * @brief   �J�����ݒu�ʒu�ł́A���[���h��ԍ��W�͈͂��擾����
 *
 * @param   fps		
 * @param   width		�ݒu����Ă���J�����ɕ\������Ă��郏�[���h���W�̕�
 * @param   height		�ݒu����Ă���J�����ɕ\������Ă��郏�[���h���W�̍���
 */
//--------------------------------------------------------------
void Footprint_WorldWidthHeightGet(FOOTPRINT_SYS_PTR fps, fx32 *width, fx32 *height)
{
	*width = fps->world_width;
	*height = fps->world_height;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�𐶐�����
 *
 * @param   fps			
 * @param   param		�X�^���v�p�����[�^
 *
 * @retval  TRUE:�����B�@FALSE:���s
 */
//--------------------------------------------------------------
BOOL Footprint_StampAdd(FOOTPRINT_SYS_PTR fps, const STAMP_PARAM *param, s32 user_id)
{
	int ret;
	u32 user_index;
	
	ret = StampSys_Add(fps, &fps->ssw, param, 
		fps->handle_footprint, fps->handle_footmark, fps->arceus_flg);
	if(ret == TRUE){
		//���ʂ̑��Ճ}�[�N��ύX����
		user_index = DWC_LOBBY_SUBCHAN_GetUserIDIdx(user_id);
		if(user_index != DWC_LOBBY_USERIDTBL_IDX_NONE){
			if(fps->name_foot_monsno[user_index] != param->monsno
					|| fps->name_foot_color[user_index] != param->color){
				fps->name_foot_monsno[user_index] = param->monsno;
				fps->name_foot_color[user_index] = param->color;
				OBJFootCharRewrite(param->monsno, param->form_no, fps->cap_name_foot[user_index], 
					fps->handle_footprint, fps->handle_footmark, 
					NNS_G2D_VRAM_TYPE_2DSUB, fps->arceus_flg);
				CATS_ObjectEnableCap(fps->cap_name_foot[user_index], CATS_ENABLE_TRUE);
				//�p���b�g���ς���
				PaletteWork_Clear(fps->pfd, FADE_SUB_BG, FADEBUF_ALL, param->color, 
					SUBBG_LIST_COLOR_START + user_index, SUBBG_LIST_COLOR_START + user_index + 1);
			}
		}
		
		Snd_SePlay(FOOTPRINT_SE_STAMP);
	}
	
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u�`�����l���̃��X�g���Q�Ƃ��Ė��O���X�g���X�V����
 *
 * @param   msgman		
 * @param   wordset		
 * @param   win[]		
 * @param   wflby_sys		
 * @param   entry_userid[]		
 *
 * @retval  FOOTPRINT_NAME_UPDATE_STATUS_???
 * 			(ENTRY��LEAVE_ROOM�̗��������������ꍇ��ENTRY��D�悵�ĕԂ��܂�)
 */
//--------------------------------------------------------------
static FOOTPRINT_NAME_UPDATE_STATUS FootPrintTool_NameAllUpdate(FOOTPRINT_SYS *fps)
{
	DWC_LOBBY_CHANNEL_USERID chan_user;
	int i, entry_num, out_num, player_max;
	
	entry_num = 0;
	out_num = 0;
	
	DWC_LOBBY_SUBCHAN_GetUserIDTbl(&chan_user);	// �T�u�`�����l�������[�UID�񋓎擾
	for(i = 0; i < FOOTPRINT_ENTRY_MAX; i++){
		OS_TPrintf("chan_user.cp_tbl[%d] = %d\n", i, chan_user.cp_tbl[i]);
		if(fps->entry_userid[i] != chan_user.cp_tbl[i]){
			//�ύX�����������̂����ĕ`��
			if(chan_user.cp_tbl[i] != DWC_LOBBY_INVALID_USER_ID){
				OS_TPrintf("�V�K���[�U�[���`�� index = %d\n", i);
				FootPrintTool_NameDraw(fps->msgman, fps->wordset, fps->name_win, 
					fps->parent_work->wflby_sys, chan_user.cp_tbl[i]);
				//���O���X�g�̃X�N���[���`��
				{
					u16 *panel_scrn;
					int x, y;
					
					panel_scrn = GF_BGL_ScreenAdrsGet(fps->bgl, FOOT_SUBFRAME_PLATE);
					for(y = Sub_ListScrnRange[i][1]; y < Sub_ListScrnRange[i][1] + Sub_ListScrnRange[i][3]; y++){
						MI_CpuCopy16(&fps->namelist_scrn[y*32 + Sub_ListScrnRange[i][0]], 
							&panel_scrn[y*32 + Sub_ListScrnRange[i][0]], 
							Sub_ListScrnRange[i][2] * 2);
					}
					GF_BGL_LoadScreenV_Req(fps->bgl, FOOT_SUBFRAME_PLATE);
				}

				entry_num++;
			}
			else{
				OS_TPrintf("���[�U�[�������@index = %d��\n", i);
				FootPrintTool_NameErase(fps->name_win, i);
				fps->name_foot_monsno[i] = 0;
				fps->name_foot_color[i] = 0;
				CATS_ObjectEnableCap(fps->cap_name_foot[i], CATS_ENABLE_FALSE);	//���Ք�\��
				GF_BGL_ScrFill(fps->bgl, FOOT_SUBFRAME_PLATE, 0, 
					Sub_ListScrnRange[i][0], Sub_ListScrnRange[i][1],
					Sub_ListScrnRange[i][2], Sub_ListScrnRange[i][3], GF_BGL_SCRWRT_PALNL);
				GF_BGL_LoadScreenV_Req(fps->bgl, FOOT_SUBFRAME_PLATE);
				out_num++;
			}
			
			//������\���g�̈ʒu���Z�b�g
			if(chan_user.cp_tbl[i] == fps->my_comm_status.user_id){
				CATS_ObjectPosSetCap_SubSurface(fps->cap_name_frame,
					Sub_NameFramePos[i][0], Sub_NameFramePos[i][1], FOOTPRINT_SUB_ACTOR_DISTANCE);
				CATS_ObjectEnableCap(fps->cap_name_frame, CATS_ENABLE_TRUE);
			}
		}
		fps->entry_userid[i] = chan_user.cp_tbl[i];
//		OS_TPrintf("�Q����(%d)��userid = %d\n", i, chan_user.cp_tbl[i]);
	}

	//�Q���l�����J�E���g
	player_max = 0;
	for(i = 0; i < FOOTPRINT_ENTRY_MAX; i++){
		if(fps->entry_userid[i] != DWC_LOBBY_INVALID_USER_ID){
			player_max++;
		}
	}
	fps->ssw.player_max = player_max;
	
	if(entry_num > 0){
		Snd_SePlay(FOOTPRINT_SE_ENTRY);
		return FOOTPRINT_NAME_UPDATE_STATUS_ENTRY;
	}
	else if(out_num > 0){
		Snd_SePlay(FOOTPRINT_SE_LEAVE_ROOM);
		return FOOTPRINT_NAME_UPDATE_STATUS_LEAVE_ROOM;
	}
	return FOOTPRINT_NAME_UPDATE_STATUS_NULL;
}

//--------------------------------------------------------------
/**
 * @brief   �Q���҂̖��O�`��
 *
 * @param   fps				
 * @param   user_index		���[�U�[id
 */
//--------------------------------------------------------------
void Footprint_NameWrite(FOOTPRINT_SYS_PTR fps, s32 user_id)
{
	FootPrintTool_NameDraw(fps->msgman, fps->wordset, fps->name_win, 
		fps->parent_work->wflby_sys, user_id);
}

//--------------------------------------------------------------
/**
 * @brief   �Q���҂̖��O����������
 *
 * @param   fps		
 * @param   user_index		���[�U�[index
 */
//--------------------------------------------------------------
void Footprint_NameErase(FOOTPRINT_SYS_PTR fps, u32 user_index)
{
	FootPrintTool_NameErase(fps->name_win, user_index);
}

//--------------------------------------------------------------
/**
 * @brief   �^�b�`�G�t�F�N�g�𐶐�����
 *
 * @param   fps			
 * @param   hit_pos		�C���N�p���b�g�̃q�b�g�ʒu
 */
//--------------------------------------------------------------
static void Footprint_TouchEffAdd(FOOTPRINT_SYS_PTR fps, int hit_pos)
{
	TCATS_OBJECT_ADD_PARAM_S head;
	int i;
	
	//�^�b�`�G�t�F�N�g�A�N�^�[����
	head = TouchEffObjParam;
	for(i = 0; i < TOUCH_EFF_MAX; i++){
		if(fps->cap_touch_eff[i] == NULL){
			head.x = INK_FOOT_POS_START_X + INK_FOOT_POS_SPACE_X * hit_pos;
			head.y = INK_FOOT_POS_Y;
			fps->cap_touch_eff[i] = CATS_ObjectAdd_S(fps->csp, fps->crp, &head);
//			CATS_ObjectUpdate(fps->cap_touch_eff[i]->act);
			break;
		}
	}
	
	Footprint_SelectInkPaletteFade(fps, hit_pos);

	Snd_SePlay(FOOTPRINT_SE_TOUCH_INK);
}

//--------------------------------------------------------------
/**
 * @brief   ���ݑI�𒆂̃C���N�p���b�g�̐F�𗎂Ƃ�
 *
 * @param   fps			
 * @param   hit_pos		�C���N�p���b�g�̃q�b�g�ʒu
 */
//--------------------------------------------------------------
static void Footprint_SelectInkPaletteFade(FOOTPRINT_SYS_PTR fps, int hit_pos)
{
	u16 *def_pal, *trans_pal;

	//���������̃C���N�p���b�g�̐F�𗎂Ƃ�
	def_pal = PaletteWorkDefaultWorkGet(fps->pfd, FADE_MAIN_OBJ);
	trans_pal = PaletteWorkTransWorkGet(fps->pfd, FADE_MAIN_OBJ);
	//�p���b�g���܂����ʂ�ɂ���
	MI_CpuCopy16(&def_pal[PALOFS_INK * 16 + COLOR_NO_INK_START], 
		&trans_pal[PALOFS_INK * 16 + COLOR_NO_INK_START], POKEMON_TEMOTI_MAX * 2);
	//�Ώۈʒu�̃p���b�g���Â�����
	SoftFade(&def_pal[PALOFS_INK * 16 + COLOR_NO_INK_START + hit_pos],
		&trans_pal[PALOFS_INK * 16 + COLOR_NO_INK_START + hit_pos], 
		1, TOUCH_EFF_EVY, TOUCH_EFF_COLOR);
}

//--------------------------------------------------------------
/**
 * @brief   �^�b�`�G�t�F�N�g�X�V����
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void Footprint_TouchEffUpdate(FOOTPRINT_SYS_PTR fps)
{
	int i;
	
	for(i = 0; i < TOUCH_EFF_MAX; i++){
		if(fps->cap_touch_eff[i] != NULL){
			if(CATS_ObjectAnimeActiveCheckCap(fps->cap_touch_eff[i]) == FALSE){
				CATS_ActorPointerDelete_S(fps->cap_touch_eff[i]);
				fps->cap_touch_eff[i] = NULL;
			}
			else{
				CATS_ObjectUpdate(fps->cap_touch_eff[i]->act);
			}
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   �t�H���gOAM���쐬����
 *
 * @param   aci			BI�V�X�e�����[�N�ւ̃|�C���^
 * @param   font_actor	���������t�H���gOAM�֘A�̃��[�N�����
 * @param   str			������
 * @param   font_type	�t�H���g�^�C�v(FONT_SYSTEM��)
 * @param   color		�t�H���g�J���[�\��
 * @param   pal_offset	�p���b�g�ԍ��I�t�Z�b�g
 * @param   pal_id		�o�^�J�n�p���b�gID
 * @param   x			���WX
 * @param   y			���WY
 * @param   pos_center  FONTOAM_LEFT(X���[���W) or FONTOAM_CENTER(X���S���W)
 */
//--------------------------------------------------------------
static void Sub_FontOamCreate(FOOTPRINT_SYS_PTR fps, FONT_ACTOR *font_actor, const STRBUF *str, 
	FONT_TYPE font_type, GF_PRINTCOLOR color, int pal_offset, int pal_id, 
	int x, int y, int pos_center)
{
	FONTOAM_INIT finit;
	GF_BGL_BMPWIN bmpwin;
	CHAR_MANAGER_ALLOCDATA cma;
	int vram_size;
	FONTOAM_OBJ_PTR fontoam;
	GF_BGL_INI *bgl;
	CATS_RES_PTR crp;
	int font_len, char_len;
	
	GF_ASSERT(font_actor->fontoam == NULL);
	
	bgl = fps->bgl;
	crp = fps->crp;
	
	//������̃h�b�g������A�g�p����L���������Z�o����
	FontLenGet(str, font_type, &font_len, &char_len);

	//BMP�쐬
	GF_BGL_BmpWinInit(&bmpwin);
	GF_BGL_BmpWinObjAdd(bgl, &bmpwin, char_len, 16 / 8, 0, 0);
	GF_STR_PrintExpand(&bmpwin, font_type, str, 0, 0, MSG_NO_PUT, color, 
		0, 0, NULL);
//	GF_STR_PrintColor(&bmpwin, font_type, str, 0, 0, MSG_NO_PUT, color, NULL );

	vram_size = FONTOAM_NeedCharSize(&bmpwin, NNS_G2D_VRAM_TYPE_2DMAIN,  HEAPID_FOOTPRINT);
	CharVramAreaAlloc(vram_size, CHARM_CONT_AREACONT, NNS_G2D_VRAM_TYPE_2DMAIN, &cma);
	
	//���W�ʒu�C��
	if(pos_center == FONTOAM_CENTER){
		x -= font_len / 2;
	}
//	y += ACTIN_SUB_ACTOR_DISTANCE_INTEGER - 8;
	y -= 8;
	
	finit.fontoam_sys = fps->fontoam_sys;
	finit.bmp = &bmpwin;
	finit.clact_set = CATS_GetClactSetPtr(crp);
	finit.pltt = CATS_PlttProxy(crp, pal_id);
	finit.parent = NULL;
	finit.char_ofs = cma.alloc_ofs;
	finit.x = x;
	finit.y = y;
	finit.bg_pri = ACTBGPRI_EXIT_FONT;
	finit.soft_pri = SOFTPRI_EXIT_FONT;
	finit.draw_area = NNS_G2D_VRAM_TYPE_2DMAIN;
	finit.heap = HEAPID_FOOTPRINT;
	
	fontoam = FONTOAM_Init(&finit);
//	FONTOAM_SetPaletteOffset(fontoam, pal_offset);
	FONTOAM_SetPaletteOffsetAddTransPlttNo(fontoam, pal_offset);
	FONTOAM_SetMat(fontoam, x, y);
	
	//�������
	GF_BGL_BmpWinDel(&bmpwin);
	
	font_actor->fontoam = fontoam;
	font_actor->cma = cma;
	font_actor->font_len = font_len;
}

//--------------------------------------------------------------
/**
 * @brief   ��������Ă���t�H���gOAM��S�č폜������
 * @param   fps		BI�V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Sub_FontOamDelete(FONT_ACTOR *font_actor)
{
	FONTOAM_Delete(font_actor->fontoam);
	CharVramAreaFree(&font_actor->cma);
}

//--------------------------------------------------------------
/**
 * @brief   ������̒������擾����
 *
 * @param   str				������ւ̃|�C���^
 * @param   font_type		�t�H���g�^�C�v
 * @param   ret_dot_len		�h�b�g�������
 * @param   ret_char_len	�L�����������
 */
//--------------------------------------------------------------
static void FontLenGet(const STRBUF *str, FONT_TYPE font_type, int *ret_dot_len, int *ret_char_len)
{
	int dot_len, char_len;
	
	//������̃h�b�g������A�g�p����L���������Z�o����
	dot_len = FontProc_GetPrintStrWidth(font_type, str, 0);
	char_len = dot_len / 8;
	if(FX_ModS32(dot_len, 8) != 0){
		char_len++;
	}
	
	*ret_dot_len = dot_len;
	*ret_char_len = char_len;
}

//--------------------------------------------------------------
/**
 * @brief   �C���N�Q�[�W�X�V����
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void Footprint_InkGaugeUpdate(FOOTPRINT_SYS_PTR fps)
{
	int offset_y, i;
	
	if(fps->ink_now == fps->ink_calc){	//�C���N����
		fps->ink_now += INK_GAUGE_RESTORE_NUM;
		if(fps->ink_now > INK_GAUGE_TANK_MAX){
			fps->ink_now = INK_GAUGE_TANK_MAX;
		}
		fps->ink_calc = fps->ink_now;
	}
	else{	//�C���N�����炵��
		fps->ink_calc -= fps->ink_sub;
		if(fps->ink_calc < fps->ink_now){
			fps->ink_calc = fps->ink_now;
		}
	}
	
	//�C���N�̗ʂ���Q�[�W�̌��ݒn������
	offset_y = INK_GAUGE_LEN - (fps->ink_calc >> 8);
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		CATS_ObjectPosSetCap_SubSurface(fps->cap_ink[i],
			INK_POS_START_X + INK_POS_SPACE_X * i, 
			INK_POS_Y + offset_y, FOOTPRINT_SUB_ACTOR_DISTANCE);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �C���N�Q�[�W�������
 *
 * @param   fps				
 * @param   consume_num		�����
 *
 * @retval  TRUE:�C���N�����肽(����OK)
 * @retval  FALSE:�C���N������Ȃ�(����NG)
 */
//--------------------------------------------------------------
static BOOL Footprint_InkGauge_Consume(FOOTPRINT_SYS_PTR fps, int consume_num)
{
#ifdef PM_DEBUG
	if(DebugFoot.consume_zero){
		consume_num = 0;
	}
#endif

	if(fps->ink_now < consume_num){
		return FALSE;	//�C���N������Ȃ�
	}
	
	if(fps->ink_calc < fps->ink_now){
		GF_ASSERT(0);	//ink_calc��now�������̒l�ɂȂ鎖�͂��肦�Ȃ�
		fps->ink_calc = fps->ink_now;
	}
	fps->ink_now -= consume_num;
	
	fps->ink_sub = (fps->ink_calc - fps->ink_now) / INK_GAUGE_SUB_FRAME;
	
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �f�o�b�O�@�\�F�J�����ړ�
 *
 * @param   fps		
 */
//--------------------------------------------------------------
static void Debug_CameraMove(FOOTPRINT_SYS *fps)
{
#ifdef PM_DEBUG
	VecFx32 move = {0,0,0};
	fx32 value = FX32_ONE;
	int add_angle = 64;
	CAMERA_ANGLE angle = {0,0,0,0};
	int mode = 0;
	enum{
		MODE_SHIFT,			//���s�ړ�
		MODE_DISTANCE,		//����
		MODE_ANGLE_REV,		//���]
	};
	
	if((sys.cont & PAD_BUTTON_L) && (sys.cont & PAD_BUTTON_R)){
		mode = MODE_DISTANCE;
	}
	else if(sys.cont & PAD_BUTTON_L){
		mode = MODE_SHIFT;
	}
	else if(sys.cont & PAD_BUTTON_R){
		mode = MODE_ANGLE_REV;
	}
	else{
		return;
	}
	
	switch(mode){
	case MODE_SHIFT:
		if(sys.cont & PAD_KEY_LEFT){
			move.x -= value;
		}
		if(sys.cont & PAD_KEY_RIGHT){
			move.x += value;
		}
		if(sys.cont & PAD_KEY_UP){
			move.y += value;
		}
		if(sys.cont & PAD_KEY_DOWN){
			move.y -= value;
		}
		if(sys.cont & PAD_BUTTON_X){
			move.z -= value;
		}
		if(sys.cont & PAD_BUTTON_B){
			move.z += value;
		}
		GFC_ShiftCamera(&move, fps->camera);
		move = GFC_GetCameraPos(fps->camera);
		OS_TPrintf("�J�����ʒu x=%d(16�i:%x), y=%d(16�i:%x), z=%d(16�i:%x)\n", move.x, move.x, move.y, move.y, move.z, move.z);
		break;
	
	case MODE_ANGLE_REV:
		if(sys.cont & PAD_KEY_LEFT){
			angle.y -= add_angle;
		}
		if(sys.cont & PAD_KEY_RIGHT){
			angle.y += add_angle;
		}
		if(sys.cont & PAD_KEY_UP){
			angle.x += add_angle;
		}
		if(sys.cont & PAD_KEY_DOWN){
			angle.x -= add_angle;
		}
		if(sys.cont & PAD_BUTTON_X){
			angle.z -= add_angle;
		}
		if(sys.cont & PAD_BUTTON_B){
			angle.z += add_angle;
		}
		GFC_AddCameraAngleRev(&angle, fps->camera);
		angle = GFC_GetCameraAngle(fps->camera);
		OS_TPrintf("�J�����A���O���@x=%d, y=%d, z=%d\n", angle.x, angle.y, angle.z);
		break;
	case MODE_DISTANCE:
		if(sys.cont & PAD_KEY_UP){
			GFC_AddCameraDistance(FX32_ONE, fps->camera);
		}
		if(sys.cont & PAD_KEY_DOWN){
			GFC_AddCameraDistance(-FX32_ONE, fps->camera);
		}
		OS_TPrintf("�J����������%d(16�i:%x)\n", GFC_GetCameraDistance(fps->camera), GFC_GetCameraDistance(fps->camera));
		break;
	}
#endif
}


//==============================================================================
/**
 * @file	frontier_enc_cutin.c
 * @brief	�t�����e�B�A�u���[����̃J�b�g�C���G�t�F�N�g
 * @author	matsuda
 * @date	2008.01.28(��)
 */
//==============================================================================
#include "common.h"
#include "system/pm_overlay.h"
#include "system/procsys.h"
#include "system/pmfprint.h"
#include "system/palanm.h"
#include "system/wipe.h"
#include "system/snd_tool.h"
#include "system/bmp_list.h"
#include "system/font_arc.h"
#include "system/window.h"
//#include "script.h"
#include "..\fielddata\script\connect_def.h"
#include "system/clact_tool.h"
#include "communication/wm_icon.h"

#include "system/msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_debug_matsu.h"

#include "frontier_types.h"
#include "fss_scene.h"
#include "frontier_main.h"
#include "frontier_map.h"
#include "frontier_tcb_pri.h"
#include "frontier_particle.h"
#include "frontier_actor.h"

#include "graphic/frontier_bg_def.h"

#include "system/fontoam.h"
#include "msgdata/msg_trname.h"
#include "field/field_encounteffect.naix"
#include "frontier_id.h"
#include "frontier_tcb_pri.h"
#include "system/fontproc.h"
#include "system/lib_pack.h"
#include "frontier_enc_cutin.h"
#include "system/brightness.h"


//==============================================================================
//	�萔��`
//==============================================================================
// OAM�f�[�^
// �A�N�^�[�f�[�^��
#define ENCOUNT_SPTR_BRAIN_CELL_NUM		( 8 )	// ���[�N��
#define ENCOUNT_SPTR_BRAIN_CELL_RES_NUM	( 3 )	// ���\�[�X��
#define ENCOUNT_SPTR_BRAIN_CELL_CONT_ID	( 600000 )
#define ENCOUNT_SPTR_BRAIN_LEADER_DRAW_BOTTOM	( 30 )	// ���͂��̃T�C�Y�܂ŕ\������
#define ENCOUNT_SPTR_BRAIN_LEADER_COLOR_FADE		( GX_RGB( 0,0,0 ) )
#define ENCOUNT_SPTR_BRAIN_LEADER_COLOR_FADE_EVY	( 14 )
#define ENCOUNT_SPTR_BRAIN_LEADER_CHAR_OFS_CX	(0)	// �L�����N�^�f�[�^�����o�����́AX�L�����N�^�I�t�Z�b�g�l

#define ENCOUNT_SPTR_BRAIN_ZIGUZAGU_IN_WND ( GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|GX_WND_PLANEMASK_BG2|GX_WND_PLANEMASK_BG3|GX_WND_PLANEMASK_OBJ)				// �W�O�U�O�����E�B���h�E��
#define ENCOUNT_SPTR_BRAIN_ZIGUZAGU_OUT_WND ( GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|GX_WND_PLANEMASK_BG2|GX_WND_PLANEMASK_OBJ)				// �W�O�U�O�����E�B���h�E�O
enum{	//VSOAM
	ENCOUNT_SPTR_BRAIN_VSOAM_ANM0,
	ENCOUNT_SPTR_BRAIN_VSOAM_ANM1,
	ENCOUNT_SPTR_BRAIN_VSOAM_ANM2,
	ENCOUNT_SPTR_BRAIN_VSOAM,
	ENCOUNT_SPTR_BRAIN_VSOAM_NUM,
};
#define ENCOUNT_SPTR_BRAIN_TRNAME_X		( 0 )	// �g���[�i�[��X�L�������W
#define ENCOUNT_SPTR_BRAIN_TRNAME_Y		( 11 )	// �g���[�i�[��Y�L�������W
#define ENCOUNT_SPTR_BRAIN_TRNAME_SIZX	( 16 )	// �g���[�i�[��BMPX�L�����T�C�Y
#define ENCOUNT_SPTR_BRAIN_TRNAME_SIZY	( 2 )	// �g���[�i�[��BMPY�L�����T�C�Y
#define ENCOUNT_SPTR_BRAIN_TRNAME_CGX		( FFD_FREE_CGX )
#define ENCOUNT_SPTR_BRAIN_TRNAME_PAL		( FFD_MESFONT_PAL )
#define ENCOUNT_SPTR_BRAIN_TRNAME_COL		( GF_PRINTCOLOR_MAKE( 1, 2, 0 ) )

// �����p�����[�^	�i�G���Ă悢�����j
#define ENCOUNT_SPTR_BRAIN_OAM_START_X	( 272*FX32_ONE )	// OAM�����J�nX
#define ENCOUNT_SPTR_BRAIN_OAM_X_SS		( -64*FX32_ONE )	// �������x
#define ENCOUNT_SPTR_BRAIN_OAM_START_Y	( 66*FX32_ONE )		// OAMY���W
#define ENCOUNT_SPTR_BRAIN_OAM_MOVE_SYNC	( 4 )				// OAM�����V���N
#define ENCOUNT_SPTR_BRAIN_TRNAME_OFS_X	( -92 )				// OAM�̈ʒu���猩��TRNAME���ǂ̈ʒu�ɕ\�����邩
#define ENCOUNT_SPTR_BRAIN_ZIGUZAGU_DOT	( 8 )				// �W�O�U�O��؂�Y�h�b�g��
#define ENCOUNT_SPTR_BRAIN_ZIGUZAGU_WIDTH	( 16 )				// �W�O�U�O��
#define ENCOUNT_SPTR_BRAIN_ZIGUZAGU_SYNC	( 6 )				// �W�O�U�O�����V���N
#define ENCOUNT_SPTR_BRAIN_BG_CY			( 68 )	// BG�̒��S
#define ENCOUNT_SPTR_BRAIN_BG_AUTOMOVE_X	( 30 )	// AUTO�����X�s�[�h
#define ENCOUNT_SPTR_BRAIN_BG_AFTER_WAIT	( 10 )				// BG������E�G�C�g
#define	ENCOUNT_SPTR_BRAIN_FLASHOUT_SYNC	( 3 )				// �t���b�V���A�E�g�V���N��
#define	ENCOUNT_SPTR_BRAIN_FLASHIN_SYNC	( 3 )				// �t���b�V���C���V���N��
#define ENCOUNT_SPTR_BRAIN_FLASH_AFTER_WAIT	( 26 )			// �t���b�V����̃E�G�C�g
#define ENCOUNT_SPTR_BRAIN_SCALE_S			( FX32_CONST(1.0f) )	// �g��J�n
#define ENCOUNT_SPTR_BRAIN_SCALE_E			( FX32_CONST(1.0f) )	// �g��J�n
#define ENCOUNT_SPTR_BRAIN_FADEOUT_SYNC		( 15 )			// �t�F�[�h�A�E�g�V���N
#define ENCOUNT_SPTR_BRAIN_VSMARK_X		( FX32_CONST( 72 ) )	// VSMARK�̈ʒu
#define ENCOUNT_SPTR_BRAIN_VSMARK_Y		( FX32_CONST( 82 ) )	// VSMARK�̈ʒu
#define ENCOUNT_SPTR_BRAIN_VSANMSYNC		( 6 )	// �P�̊g�k�A�j���̃V���N��
#define ENCOUNT_SPTR_BRAIN_VSANMTIMING	( 3 )	//	�����o���^�C�~���O
#define ENCOUNT_SPTR_BRAIN_3DAREA_BRIGHTNESS	( -14 )// 3D�ʂ��Â�����l

//--------------------------------------------------------------
//	�V�K��`
//--------------------------------------------------------------
///�J�b�g�C��BG�̃p���b�g�W�J�ʒu
#define ENCOUNT_CUTIN_BG_PALNO		( FFD_SYSFONT_PAL )
///�t�H���gOAM�̎g�p��(�g���[�i�[���̕\���݂̂Ɏg�p)
#define ENCOUNT_FONT_OAM_MAX		(4)
///�W�O�U�OBG�̃p���b�g�W�J�ʒu
#define ZIGZAG_BG_PALNO				(FFD_MENUFRAME_PAL)
///�W�O�U�OBG�̃p���b�g�A�j���{��
#define ZIGZAG_BG_PAL_ANM_NUM		(8)
///�W�O�U�OBG�̃p���b�g�A�j���̃E�F�C�g
#define ZIGZAG_BGPALANM_WAIT		(0)

enum{
	WINEFF_OPEN,
	WINEFF_CLOSE,
};

///�E�B���h�E���J���ő�h�b�g��
#define WNDOPEN_EFF_OPEN_Y_DOT		(8*8 + 4)
///�E�B���h�E���J�����x(����8�r�b�g����)
#define WNDOPEN_EFF_ADD_Y			(0x0800)
///�E�B���h�E�̃Z���^�[���W
#define WNDOPEN_EFF_CENTER_Y		(10*8)

///��o��
enum{
	FACE_IN_END_X = 256 - 6 * 8,
	FACE_IN_START_X = FACE_IN_END_X + 6 * 8,
	FACE_IN_START_Y = 10 * 8,
	FACE_IN_SPEED_X = 0x0f00,	//�C�����Ă��鎞�̑��x(����8�r�b�g����)
};

enum{
	FONTOAM_LEFT,		///<X���[���W
	FONTOAM_CENTER,		///<X���S���W
};


///�t�H���gOAM�쐬���A�O����BMP�w�肷��ꍇ�Ɏg�p
typedef struct{
	GF_BGL_BMPWIN bmpwin;
	u16 char_len;
	u16 font_len;
}FONT_EX_BMPWIN;

///�t�H���g�A�N�^�[���[�N
typedef struct{
	FONTOAM_OBJ_PTR fontoam;
	CHAR_MANAGER_ALLOCDATA cma;
	u16 font_len;
}FONT_ACTOR;

///�E�B���h�E�@�\�̊J�����G�t�F�N�g�p���[�N
typedef struct{
	s32 y1;						///<�E�B���h�E���WY(�㑤)�F����8�r�b�g����
	s32 y2;						///<�E�B���h�E���WY(����)�F����8�r�b�g����
	
	u8 end_flag;				///<�I����TRUE���Z�b�g�����
	u8 seq;						///<�V�[�P���X�ԍ�

	u8 wnd0_x1;						///<�E�B���h�E0��X1���W
	u8 wnd0_y1;
	u8 wnd0_x2;
	u8 wnd0_y2;

	u8 wnd1_x1;
	u8 wnd1_y1;
	u8 wnd1_x2;
	u8 wnd1_y2;
}WNDOPEN_EFF_WORK;

///��X���C�h�C�����䃏�[�N
typedef struct{
	int x;
	int seq;
}FACE_MOVE_WORK;

//-------------------------------------
//
//	�G���J�E���g�G�t�F�N�g���[�N
//
//=====================================
typedef struct _ENCOUNT_EFFECT_WORK{
	int seq;
	int wait;
	int count;
	void* work;		// �e���A���R�Ɏg�p
	GF_BGL_INI *bgl;
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	PALETTE_FADE_PTR pfd;
	u16* end;		// �I�������p
	ARCHANDLE* p_handle;	// �A�[�J�C�u�n���h��
	s16 map_x;
	s16 map_y;
	
	u8 effect_no;			///<�G�t�F�N�g�ԍ�
	FONTOAM_SYS_PTR fontoam_sys;		///<�t�H���gOAM�V�X�e���ւ̃|�C���^

	TCB_PTR tcb_vwait;		///<Vwait�X�VTCB
	
	//BG�p���b�g�A�j��
	int palanm_wait;		///<�p���b�g�A�j���̃E�F�C�g
	int palanm_no;			///<�p���b�g�A�j���ʒu
	u16 palbuf[ZIGZAG_BG_PAL_ANM_NUM * 16];	///�p���b�g�A�j���o�b�t�@
	TCB_PTR tcb_bgpalanm;	///<�p���b�g�A�j���X�VTCB

	//�E�B���h�E
	WNDOPEN_EFF_WORK wineff;
	
	//��X���C�h�C��
	FACE_MOVE_WORK face_move_work;
	int face_palno;
	u32 obj_palbit;
}ENCOUNT_EFFECT_WORK;

//-------------------------------------
//	�ėp����
//=====================================
typedef struct {
	fx32 x;
	fx32 s_x;
	fx32 dis_x;
	int count;
	int count_max;
} ENC_MOVE_WORK_FX;

//-------------------------------------
///	VS�A�j��
//=====================================
typedef struct {
	s16 timing;
	s16 movenum;
	CATS_ACT_PTR cap_vs[ ENCOUNT_SPTR_BRAIN_VSOAM_NUM ];
	ENC_MOVE_WORK_FX vsscale[ ENCOUNT_SPTR_BRAIN_VSOAM_NUM ];
} ENCOUNT_SPTR_BRAIN_VSANM;
//
//-------------------------------------
//	�W�����[�_�[
//=====================================
typedef struct {
	ENC_MOVE_WORK_FX scale;
	ENCOUNT_SPTR_BRAIN_VSANM vsanm;
	s32 wait;
	
	//�V�K��`
	FONT_ACTOR fontoam_trainer_name;	///�t�H���gOAM(�g���[�i�[��)
	CATS_ACT_PTR cap_face;
} ENCOUNT_SPTR_BRAIN;


// ���ꂼ��ŕς��Ƃ���
typedef struct {
	u32	trno;
	u8	nclr;
	u8	ncgr;
	u8	ncer;
	u8	nanr;
	u8	bg_nclr;
	u8	bg_ncgr;
	u8	bg_nscr;
	u8	pad;
} ENCOUNT_SPTR_BRAIN_PARAM;

// ���ꂼ��̒萔
static const ENCOUNT_SPTR_BRAIN_PARAM ENCOUNT_SPTR_BrainDef[] = {
	{	//FRONTIER_NO_TOWER
		TR_KUROTUGU_01,			// ���[�_�[�^�C�v
		NARC_field_encounteffect_trpl_fbface05_NCLR,
		NARC_field_encounteffect_trpl_fbface05_NCGR,
		NARC_field_encounteffect_trpl_fbface05_NCER,
		NARC_field_encounteffect_trpl_fbface05_NANR,
		NARC_field_encounteffect_cutin_fbrain05_NCLR,
		NARC_field_encounteffect_cutin_fbrain05_NCGR,
		NARC_field_encounteffect_cutin_fbrain05_NSCR,
		0,
	},
	{	//FRONTIER_NO_FACTORY_LV50
		TR_FBOSS2_01,			// ���[�_�[�^�C�v
		NARC_field_encounteffect_trpl_fbface02_NCLR,
		NARC_field_encounteffect_trpl_fbface02_NCGR,
		NARC_field_encounteffect_trpl_fbface02_NCER,
		NARC_field_encounteffect_trpl_fbface02_NANR,
		NARC_field_encounteffect_cutin_fbrain02_NCLR,
		NARC_field_encounteffect_cutin_fbrain02_NCGR,
		NARC_field_encounteffect_cutin_fbrain02_NSCR,
		0,
	},
	{	//FRONTIER_NO_FACTORY_LV100
		TR_FBOSS2_01,			// ���[�_�[�^�C�v
		NARC_field_encounteffect_trpl_fbface02_NCLR,
		NARC_field_encounteffect_trpl_fbface02_NCGR,
		NARC_field_encounteffect_trpl_fbface02_NCER,
		NARC_field_encounteffect_trpl_fbface02_NANR,
		NARC_field_encounteffect_cutin_fbrain02_NCLR,
		NARC_field_encounteffect_cutin_fbrain02_NCGR,
		NARC_field_encounteffect_cutin_fbrain02_NSCR,
		0,
	},
	{	//FRONTIER_NO_CASTLE
		TR_FBOSS4_01,			// ���[�_�[�^�C�v
		NARC_field_encounteffect_trpl_fbface01_NCLR,
		NARC_field_encounteffect_trpl_fbface01_NCGR,
		NARC_field_encounteffect_trpl_fbface01_NCER,
		NARC_field_encounteffect_trpl_fbface01_NANR,
		NARC_field_encounteffect_cutin_fbrain01_NCLR,
		NARC_field_encounteffect_cutin_fbrain01_NCGR,
		NARC_field_encounteffect_cutin_fbrain01_NSCR,
		0,
	},
	{	//FRONTIER_NO_STAGE
		TR_FBOSS1_01,			// ���[�_�[�^�C�v
		NARC_field_encounteffect_trpl_fbface04_NCLR,
		NARC_field_encounteffect_trpl_fbface04_NCGR,
		NARC_field_encounteffect_trpl_fbface04_NCER,
		NARC_field_encounteffect_trpl_fbface04_NANR,
		NARC_field_encounteffect_cutin_fbrain04_NCLR,
		NARC_field_encounteffect_cutin_fbrain04_NCGR,
		NARC_field_encounteffect_cutin_fbrain04_NSCR,
		0,
	},
	{	//FRONTIER_NO_ROULETTE
		TR_FBOSS3_01,			// ���[�_�[�^�C�v
		NARC_field_encounteffect_trpl_fbface03_NCLR,
		NARC_field_encounteffect_trpl_fbface03_NCGR,
		NARC_field_encounteffect_trpl_fbface03_NCER,
		NARC_field_encounteffect_trpl_fbface03_NANR,
		NARC_field_encounteffect_cutin_fbrain03_NCLR,
		NARC_field_encounteffect_cutin_fbrain03_NCGR,
		NARC_field_encounteffect_cutin_fbrain03_NSCR,
		0,
	},
};

///�A�N�^�[�\�t�g�v���C�I���e�B
enum{
	CUTIN_SOFTPRI_VS = 10,
	CUTIN_SOFTPRI_NAME,
	CUTIN_SOFTPRI_FACE,
};

///�A�N�^�[BG�v���C�I���e�B
#define CUTIN_BGPRI_VS			(0)
#define CUTIN_BGPRI_NAME		(0)
#define CUTIN_BGPRI_FACE		(0)

///�� �A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S FaceObjParam = {
	0,0,0,		//X,Y,Z
	0, CUTIN_SOFTPRI_FACE, 0,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		FR_CHARID_CUTIN_FACE,			//�L����
		FR_PLTTID_CUTIN_FACE,			//�p���b�g
		FR_CELLID_CUTIN_FACE,			//�Z��
		FR_CELLANMID_CUTIN_FACE,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	CUTIN_BGPRI_FACE,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///VS �A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S VsObjParam = {
	0,0,0,		//X,Y,Z
	0, CUTIN_SOFTPRI_VS, 0,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		FR_CHARID_CUTIN_VS,			//�L����
		FR_PLTTID_CUTIN_VS,			//�p���b�g
		FR_CELLID_CUTIN_VS,			//�Z��
		FR_CELLANMID_CUTIN_VS,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	CUTIN_BGPRI_VS,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void ENC_End( ENCOUNT_EFFECT_WORK* eew, TCB_PTR tcb );
static void VWait_EncountUpdateTCB(TCB_PTR tcb, void *work);
static void CutinEffect_MainTCB(TCB_PTR tcb, void* work);
static BOOL EncountEffect_FBrainMain( ENCOUNT_EFFECT_WORK *eew, u32 heapID, const ENCOUNT_SPTR_BRAIN_PARAM* cp_def );
static void Sub_FontOamCreate(ENCOUNT_EFFECT_WORK *eew, FONT_ACTOR *font_actor, const STRBUF *str, 
	FONT_TYPE font_type, GF_PRINTCOLOR color, int pal_offset, int pal_id, 
	int x, int y, int pos_center, FONT_EX_BMPWIN *ex_bmpwin);
static void Sub_FontOamDelete(FONT_ACTOR *font_actor);
static void EncountEffect_SpTr_VsAnmInit(ENCOUNT_EFFECT_WORK *eew, ENCOUNT_SPTR_BRAIN_VSANM* p_wk, fx32 x, fx32 y, u32 heapID );
static void EncountEffect_SpTr_VsAnmExit( ENCOUNT_SPTR_BRAIN_VSANM* p_wk );
static BOOL EncountEffect_SpTr_VsAnmMain( ENCOUNT_SPTR_BRAIN_VSANM* p_wk );
static void ENC_MoveReqFx( ENC_MOVE_WORK_FX* p_work, fx32 s_x, fx32 e_x, int count_max );
static BOOL ENC_MoveMainFx( ENC_MOVE_WORK_FX* p_work );
static VecFx32 ENC_MakeVec( fx32 x, fx32 y, fx32 z );
static void ZigzagBG_Load(ENCOUNT_EFFECT_WORK *eew, const ENCOUNT_SPTR_BRAIN_PARAM *cp_def);
static void ZigzagPalAnmTCB(TCB_PTR tcb, void *work);
static void WndOpenEffStart(ENCOUNT_EFFECT_WORK *eew, int open_close);
static void WndEffOpenTask(TCB_PTR tcb, void *work);
static void WndEffCloseTask(TCB_PTR tcb, void *work);
static BOOL FaceIn_Main(ENCOUNT_EFFECT_WORK *eew, ENCOUNT_SPTR_BRAIN* task_w, FACE_MOVE_WORK *fmw);
static void FontLenGet(const STRBUF *str, FONT_TYPE font_type, int *ret_dot_len, int *ret_char_len);



//----------------------------------------------------------------------------
/**
 *
 *@brief	�G���J�E���g�G�t�F�N�g�J�n
 *
 *@param	No		�G���J�E���g�G�t�F�N�gNO(FRONTIER_NO_???)
 *@param	*fsw	�t�B�[���h�V�X�e�����[�N�̃|�C���^
 *@param	end		�I���`�F�b�N�p�@TRUE�F�I������ 
 *
 *@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
void BrainCutin_EffectStart(int No, GF_BGL_INI *bgl, CATS_SYS_PTR csp, CATS_RES_PTR crp, PALETTE_FADE_PTR pfd, u16* end, s16 map_x, s16 map_y )
{
	TCB_PTR tcb;
	ENCOUNT_EFFECT_WORK *eew;
	
	tcb = PMDS_taskAdd(CutinEffect_MainTCB, sizeof(ENCOUNT_EFFECT_WORK), TCBPRI_ENC_CUTIN, HEAPID_FRONTIERMAP);
	eew = TCB_GetWork(tcb);
	eew->bgl = bgl;
	eew->csp = csp;
	eew->crp = crp;
	eew->pfd = pfd;
	eew->end = end;		// �I���t���O�ݒ�
	eew->map_x = map_x;
	eew->map_y = map_y;
	eew->p_handle = ArchiveDataHandleOpen( ARC_ENCOUNTEFFECT, HEAPID_FRONTIERMAP );
	if(eew->end != NULL){
		*(eew->end) = FALSE;
	}
	eew->effect_no = No - 1;	//0origin�ɂ���

	//VRAM�N���A	�X�N���[���̃N���A�R�[�h��0�ł͂Ȃ��ׁA�ǂ����N���A�R�[�h�ɂȂ��Ă��Ă�
	//				���Ȃ��悤�Ɉ�x�S�ẴL�����N�^�̈���N���A���Ă���
	MI_CpuClear32(GF_BGL_CgxGet(FRMAP_FRAME_WIN), 0x8000);

	GF_BGL_ScrollReq(bgl, FRMAP_FRAME_WIN, GF_BGL_SCROLL_X_SET, 0);
	GF_BGL_ScrollReq(bgl, FRMAP_FRAME_WIN, GF_BGL_SCROLL_Y_SET, 0);

	// BG�ݒ�
	ZigzagBG_Load(eew, &ENCOUNT_SPTR_BrainDef[eew->effect_no]);
	//Vwait�X�V�^�X�N����
	eew->tcb_vwait = VWaitTCB_Add(VWait_EncountUpdateTCB, eew, 1);
}

//----------------------------------------------------------------------------
/**
 * encount_effect_sub�ȊO�̊O������J
 *	@brief	���[�N�ƃ^�X�N�j���֐�
 *
 *	@param	eew �G���J�E���g�G�t�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static void ENC_End( ENCOUNT_EFFECT_WORK* eew, TCB_PTR tcb )
{
	TCB_Delete(eew->tcb_bgpalanm);
	TCB_Delete(eew->tcb_vwait);
	
	ArchiveDataHandleClose( eew->p_handle );
	sys_FreeMemory(HEAPID_FRONTIERMAP, eew->work);
	PMDS_taskDel(tcb);	//�^�X�N�I��
}

static void VWait_EncountUpdateTCB(TCB_PTR tcb, void *work)
{
	ENCOUNT_EFFECT_WORK *eew = work;
	
	//�E�B���h�E���W
	G2_SetWnd0Position(eew->wineff.wnd0_x1, eew->wineff.wnd0_y1, 
		eew->wineff.wnd0_x2, eew->wineff.wnd0_y2);
	G2_SetWnd1Position(eew->wineff.wnd1_x1, eew->wineff.wnd1_y1, 
		eew->wineff.wnd1_x2, eew->wineff.wnd1_y2);
}

//----------------------------------------------------------------------------
/**
 *	@brief		�e�W��
 */
//-----------------------------------------------------------------------------
static void CutinEffect_MainTCB(TCB_PTR tcb, void* work)
{
	ENCOUNT_EFFECT_WORK *eew = work;
	BOOL result;
	result = EncountEffect_FBrainMain( eew, HEAPID_FRONTIERMAP, &ENCOUNT_SPTR_BrainDef[eew->effect_no]);
	if( result == TRUE ){
		ENC_End( eew, tcb );
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�t�����e�B�A�u���[����p�G���J�E���g�G�t�F�N�g
 *
 *	@param	eew		�G�t�F�N�g���[�N
 *	@param	heapID	�q�[�vID
 *	@param	cp_def	�f�t�@�C����`
 *
 *	@retval	TRUE		�I��
 *	@retval	FALSE		�r��
 */
//-----------------------------------------------------------------------------
static BOOL EncountEffect_FBrainMain( ENCOUNT_EFFECT_WORK *eew, u32 heapID, const ENCOUNT_SPTR_BRAIN_PARAM* cp_def )
{
	ENCOUNT_SPTR_BRAIN* task_w = eew->work;
	BOOL result;
	const VecFx32* cp_matrix;
	VecFx32 matrix;
	VecFx32 scale;
	int bottom_y;
	int i, palno = 0;
	STRBUF* p_str;
	enum{
		ENCOUNT_SPTR_BRAIN_START,
		ENCOUNT_SPTR_BRAIN_FLASH_INI,
		ENCOUNT_SPTR_BRAIN_FLASH_WAIT,
		ENCOUNT_SPTR_BRAIN_FLASH_INI_2,
		ENCOUNT_SPTR_BRAIN_FLASH_WAIT_2,
		ENCOUNT_SPTR_BRAIN_BG_IN,
		ENCOUNT_SPTR_BRAIN_BG_INWAIT,
		ENCOUNT_SPTR_BRAIN_VS_ANMWAIT,
		ENCOUNT_SPTR_BRAIN_OAM_IN,
		ENCOUNT_SPTR_BRAIN_OAM_INWAIT,
		ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT,
		ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT_START,
		ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT_WAIT,
		ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT_IN_START,
		ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT_IN_WAIT,
		ENCOUNT_SPTR_BRAIN_WAIT,
		ENCOUNT_SPTR_BRAIN_WIPE,
		ENCOUNT_SPTR_BRAIN_WIPEWAIT,
		ENCOUNT_SPTR_BRAIN_END,
	};

	switch( eew->seq ){
	case ENCOUNT_SPTR_BRAIN_START:
		eew->work = sys_AllocMemory( heapID, sizeof(ENCOUNT_SPTR_BRAIN) );	
		memset( eew->work, 0, sizeof(ENCOUNT_SPTR_BRAIN) );
		task_w = eew->work;

		// �t�H���gOAM�p���b�g�ǂݍ���	
		palno = CATS_LoadResourcePlttWorkArcH(eew->pfd, FADE_MAIN_OBJ, eew->csp, eew->crp, 
			eew->p_handle, NARC_field_encounteffect_cutin_gym_font_NCLR, 0, 1, 
			NNS_G2D_VRAM_TYPE_2DMAIN, FR_PLTTID_CUTIN_NAME);
		OS_TPrintf("fontoam palno = %d\n", palno);
		eew->obj_palbit |= 1 << palno;
		
		//�g���[�i�[���\���ׂ̈�FONTOAM���쐬
		eew->fontoam_sys = FONTOAM_SysInit(ENCOUNT_FONT_OAM_MAX, HEAPID_FRONTIERMAP);
		{
			MSGDATA_MANAGER *man;
			STRBUF *name;
			
			man = MSGMAN_Create(MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_trname_dat, heapID);
			name = MSGMAN_AllocString(man, cp_def->trno);
			Sub_FontOamCreate(eew, &task_w->fontoam_trainer_name, name, FONT_SYSTEM,
				ENCOUNT_SPTR_BRAIN_TRNAME_COL, 0, 
				FR_PLTTID_CUTIN_NAME, FACE_IN_END_X + ENCOUNT_SPTR_BRAIN_TRNAME_OFS_X + eew->map_x,
				ENCOUNT_SPTR_BRAIN_TRNAME_Y * 8 + eew->map_y, FONTOAM_LEFT, NULL);
			FONTOAM_SetDrawFlag(task_w->fontoam_trainer_name.fontoam, FALSE);	//��\��
			STRBUF_Delete(name);
			MSGMAN_Delete(man);
		}
		
		//���\�[�X���[�h
		{
			//-- �� --//
			eew->face_palno = CATS_LoadResourcePlttWorkArcH(
				eew->pfd, FADE_MAIN_OBJ, eew->csp, eew->crp, 
				eew->p_handle, cp_def->nclr, 0, 1, 
				NNS_G2D_VRAM_TYPE_2DMAIN, FR_PLTTID_CUTIN_FACE);
			eew->obj_palbit |= 1 << eew->face_palno;
			CATS_LoadResourceCharArcH(eew->csp, eew->crp, eew->p_handle, 
				cp_def->ncgr, 0, NNS_G2D_VRAM_TYPE_2DMAIN, FR_CHARID_CUTIN_FACE);
			CATS_LoadResourceCellArcH(eew->csp, eew->crp, eew->p_handle, 
				cp_def->ncer, 0, FR_CELLID_CUTIN_FACE);
			CATS_LoadResourceCellAnmArcH(eew->csp, eew->crp, eew->p_handle, 
				cp_def->nanr, 0, FR_CELLANMID_CUTIN_FACE);
			SoftFadePfd(eew->pfd, FADE_MAIN_OBJ, eew->face_palno * 16, 16, 
				ENCOUNT_SPTR_BRAIN_LEADER_COLOR_FADE_EVY, ENCOUNT_SPTR_BRAIN_LEADER_COLOR_FADE);
			//-- VS --//
			palno = CATS_LoadResourcePlttWorkArcH(eew->pfd, FADE_MAIN_OBJ, eew->csp, eew->crp, 
				eew->p_handle, NARC_field_encounteffect_cutin_gym_vs_NCLR, 0, 1, 
				NNS_G2D_VRAM_TYPE_2DMAIN, FR_PLTTID_CUTIN_VS);
			eew->obj_palbit |= 1 << palno;
			CATS_LoadResourceCharArcH(eew->csp, eew->crp, eew->p_handle, 
				NARC_field_encounteffect_cutin_gym_vs128k_NCGR, 0, 
				NNS_G2D_VRAM_TYPE_2DMAIN, FR_CHARID_CUTIN_VS);
			CATS_LoadResourceCellArcH(eew->csp, eew->crp, eew->p_handle, 
				NARC_field_encounteffect_cutin_gym_vs128k_NCER, 0, FR_CELLID_CUTIN_VS);
			CATS_LoadResourceCellAnmArcH(eew->csp, eew->crp, eew->p_handle, 
				NARC_field_encounteffect_cutin_gym_vs128k_NANR, 0, FR_CELLANMID_CUTIN_VS);
		}

		// �A�N�^�[�o�^
		task_w->cap_face = CATS_ObjectAdd_S(eew->csp, eew->crp, &FaceObjParam);
		CATS_ObjectEnableCap(task_w->cap_face, CATS_ENABLE_FALSE);
		CATS_ObjectUpdate(task_w->cap_face->act);
		
		EncountEffect_SpTr_VsAnmInit(eew, &task_w->vsanm, 
			ENCOUNT_SPTR_BRAIN_VSMARK_X + FX32_CONST(eew->map_x), 
			ENCOUNT_SPTR_BRAIN_VSMARK_Y + FX32_CONST(eew->map_y), heapID );

		// �J���[�p���b�g��ݒ�
	//	ENC_CLACT_ResColorChange( task_w->clact, heapID, cp_def->type, ENCOUNT_SPTR_BRAIN_LEADER_COLOR_FADE_EVY, ENCOUNT_SPTR_BRAIN_LEADER_COLOR_FADE );

		// �W�O�U�O�E�B���h�E������
	//	task_w->p_ziguzagu = ENC_BG_WndZiguzagu_Alloc();

		eew->seq ++;
		break;
		
	case ENCOUNT_SPTR_BRAIN_FLASH_INI:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
						WIPE_FADE_WHITE, 3, WIPE_DEF_SYNC, heapID );
		eew->seq++;
		break;
		
	case ENCOUNT_SPTR_BRAIN_FLASH_WAIT:
		if( WIPE_SYS_EndCheck() ) {
			eew->seq++;
		}
		break;
	case ENCOUNT_SPTR_BRAIN_FLASH_INI_2:
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
						WIPE_FADE_WHITE, 3, WIPE_DEF_SYNC, heapID );
		eew->seq++;
		break;
		
	case ENCOUNT_SPTR_BRAIN_FLASH_WAIT_2:
		if( WIPE_SYS_EndCheck() ) {
			eew->seq++;
		}
		break;

	case ENCOUNT_SPTR_BRAIN_BG_IN:
		//�E�B���h�E�I�[�v��
		WndOpenEffStart(eew, WINEFF_OPEN);
		eew->seq++;
		break;
		
	case ENCOUNT_SPTR_BRAIN_BG_INWAIT:
		if(eew->wineff.end_flag == TRUE){
			eew->seq++;
			task_w->wait = ENCOUNT_SPTR_BRAIN_BG_AFTER_WAIT;
		}
		break;

	case ENCOUNT_SPTR_BRAIN_VS_ANMWAIT:

		task_w->wait --;
		if( task_w->wait >= 0 ){
			break;
		}

		// VS���o��
		result = EncountEffect_SpTr_VsAnmMain( &task_w->vsanm );
		if( result == TRUE ){
			eew->seq++;
		}
		break;
		
		
	case ENCOUNT_SPTR_BRAIN_OAM_IN:
		eew->seq++;
		break;
		
	case ENCOUNT_SPTR_BRAIN_OAM_INWAIT:
		result = FaceIn_Main(eew, task_w, &eew->face_move_work);
		if( result == TRUE ){
			eew->seq++;
		}
		break;

	case ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT:
		task_w->wait = ENCOUNT_SPTR_BRAIN_BG_AFTER_WAIT;
		eew->seq++;
		break;

	case ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT_START:
		task_w->wait --;
		if( task_w->wait >= 0 ){
			break;
		}

		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
						WIPE_FADE_WHITE, 3, WIPE_DEF_SYNC, heapID );
		eew->seq++;
		break;
		
	case ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT_WAIT:
		if( WIPE_SYS_EndCheck() ) {
			//�G���J�E���g�G�t�F�N�g��Add�����ȊO��OBJ�̋P�x�𗎂Ƃ�
			OS_TPrintf("color conce palbit = %d, %d\n", eew->obj_palbit, eew->obj_palbit^0x3fff);
			ColorConceChangePfd(eew->pfd, FADE_MAIN_OBJ, eew->obj_palbit ^ 0x3fff, 
				ENCOUNT_SPTR_BRAIN_LEADER_COLOR_FADE_EVY, 0x0000);
			// ��̃J���[�p���b�g�ύX
			SoftFadePfd(eew->pfd, FADE_MAIN_OBJ, eew->face_palno * 16, 16, 
				0, ENCOUNT_SPTR_BRAIN_LEADER_COLOR_FADE);

			// �RD�ʂɃu���C�g�l�X��������
			SetBrightness( ENCOUNT_SPTR_BRAIN_3DAREA_BRIGHTNESS,
					PLANEMASK_BG2|PLANEMASK_BG3|PLANEMASK_BD, MASK_MAIN_DISPLAY );

			//�@�������o��	�W�����[�_�[OAM�̈ʒu����ɏo��
			FONTOAM_SetDrawFlag(task_w->fontoam_trainer_name.fontoam, TRUE);
			eew->seq++;
		}
		break;
	case ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT_IN_START:
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
						WIPE_FADE_WHITE, 3, WIPE_DEF_SYNC, heapID );
		eew->seq++;
		break;
		
	case ENCOUNT_SPTR_BRAIN_OAM_FLASH_OUT_IN_WAIT:
		if( WIPE_SYS_EndCheck() ) {
			task_w->wait = ENCOUNT_SPTR_BRAIN_FLASH_AFTER_WAIT;
			eew->seq++;
		}
		break;
		
	case ENCOUNT_SPTR_BRAIN_WAIT:
		task_w->wait --;
		if( task_w->wait < 0 ){
			eew->seq ++;
		}
		break;
		
	case ENCOUNT_SPTR_BRAIN_WIPE:

		// �z���C�g�A�E�g
		WIPE_SYS_Start( WIPE_PATTERN_M, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_WHITE, ENCOUNT_SPTR_BRAIN_FADEOUT_SYNC, 1, HEAPID_FRONTIERMAP );
		eew->seq ++;
		break;
		
	case ENCOUNT_SPTR_BRAIN_WIPEWAIT:
		if( WIPE_SYS_EndCheck() ) {
			eew->seq ++;
		}
		break;
		
	case ENCOUNT_SPTR_BRAIN_END:
		WIPE_SetBrightness( WIPE_DISP_SUB, WIPE_FADE_WHITE );
		
		if(eew->end != NULL){
			*(eew->end) = TRUE;		// �^�X�N�I�����
		}
		
		//FONTOAM�j��
		Sub_FontOamDelete(&task_w->fontoam_trainer_name);
		FONTOAM_SysDelete(eew->fontoam_sys);
		
		// OAM�j��
		CATS_ActorPointerDelete_S(task_w->cap_face);
		EncountEffect_SpTr_VsAnmExit( &task_w->vsanm );

		// �E�B���h�E�j��
	//	GX_SetVisibleWnd(GX_WNDMASK_NONE);

		// �u���C�g�l�X�͂�
	//	SetBrightness( BRIGHTNESS_NORMAL,
	//			PLANEMASK_NONE, MASK_MAIN_DISPLAY );
		
		return TRUE;
	}

	return FALSE;
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
 * @param   ex_bmpwin	�Ăяo�����Ńt�H���gOAM�Ɋ֘A�t����BMPWIN�������Ă���ꍇ�͂����œn���B
 *          			NULL�̏ꍇ�͒��ō쐬����܂��B(ex_bmpwin���g�p����ꍇ��str��NULL��OK)
 */
//--------------------------------------------------------------
static void Sub_FontOamCreate(ENCOUNT_EFFECT_WORK *eew, FONT_ACTOR *font_actor, const STRBUF *str, 
	FONT_TYPE font_type, GF_PRINTCOLOR color, int pal_offset, int pal_id, 
	int x, int y, int pos_center, FONT_EX_BMPWIN *ex_bmpwin)
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
	
	bgl = eew->bgl;
	crp = eew->crp;
	
	//������̃h�b�g������A�g�p����L���������Z�o����
	if(ex_bmpwin == NULL){
		FontLenGet(str, font_type, &font_len, &char_len);
	}
	else{
		font_len = ex_bmpwin->font_len;
		char_len = ex_bmpwin->char_len;
	}

	//BMP�쐬
	if(ex_bmpwin == NULL){
		GF_BGL_BmpWinInit(&bmpwin);
		GF_BGL_BmpWinObjAdd(bgl, &bmpwin, char_len, 16 / 8, 0, 0);
		GF_STR_PrintExpand(&bmpwin, font_type, str, 0, 0, MSG_NO_PUT, color, 
			0, 0, NULL);
//		GF_STR_PrintColor(&bmpwin, font_type, str, 0, 0, MSG_NO_PUT, color, NULL );
	}
	else{
		bmpwin = ex_bmpwin->bmpwin;
	}

	vram_size = FONTOAM_NeedCharSize(&bmpwin, NNS_G2D_VRAM_TYPE_2DMAIN,  HEAPID_FRONTIERMAP);
	CharVramAreaAlloc(vram_size, CHARM_CONT_AREACONT, NNS_G2D_VRAM_TYPE_2DMAIN, &cma);
	
	//���W�ʒu�C��
	if(pos_center == FONTOAM_CENTER){
		x -= font_len / 2;
	}
//	y += ACTIN_SUB_ACTOR_DISTANCE_INTEGER - 8;
	y -= 8;
	
	finit.fontoam_sys = eew->fontoam_sys;
	finit.bmp = &bmpwin;
	finit.clact_set = CATS_GetClactSetPtr(crp);
	finit.pltt = CATS_PlttProxy(crp, pal_id);
	finit.parent = NULL;
	finit.char_ofs = cma.alloc_ofs;
	finit.x = x;
	finit.y = y;
	finit.bg_pri = CUTIN_BGPRI_NAME;
	finit.soft_pri = CUTIN_SOFTPRI_NAME;
	finit.draw_area = NNS_G2D_VRAM_TYPE_2DMAIN;
	finit.heap = HEAPID_FRONTIERMAP;
	
	fontoam = FONTOAM_Init(&finit);
//	FONTOAM_SetPaletteOffset(fontoam, pal_offset);
	FONTOAM_SetPaletteOffsetAddTransPlttNo(fontoam, pal_offset);
	FONTOAM_SetMat(fontoam, x, y);
	
	//�������
	if(ex_bmpwin == NULL){
		GF_BGL_BmpWinDel(&bmpwin);
	}
	
	font_actor->fontoam = fontoam;
	font_actor->cma = cma;
	font_actor->font_len = font_len;
}

//--------------------------------------------------------------
/**
 * @brief   ��������Ă���t�H���gOAM��S�č폜������
 * @param   eew		BI�V�X�e�����[�N�ւ̃|�C���^
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

//----------------------------------------------------------------------------
/**
 *	@brief	VS�A�j��	������
 *	
 *	@param	task_w		�^�X�N���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void EncountEffect_SpTr_VsAnmInit(ENCOUNT_EFFECT_WORK *eew, ENCOUNT_SPTR_BRAIN_VSANM* p_wk, fx32 x, fx32 y, u32 heapID )
{
	int i;
	TCATS_OBJECT_ADD_PARAM_S head = VsObjParam;
	
	head.x = x >> FX32_SHIFT;
	head.y = y >> FX32_SHIFT;
	
	p_wk->timing = 0;
	p_wk->movenum = 0;

	for( i=0; i<ENCOUNT_SPTR_BRAIN_VSOAM_NUM; i++ ){
		
		p_wk->cap_vs[i] = CATS_ObjectAdd_S(eew->csp, eew->crp, &head);
		CATS_ObjectEnableCap(p_wk->cap_vs[i], CATS_ENABLE_FALSE);

		if( i!=ENCOUNT_SPTR_BRAIN_VSOAM ){	// �Ō�̂P������
			CLACT_SetAffineParam( p_wk->cap_vs[i]->act, CLACT_AFFINE_DOUBLE );
			CLACT_AnmChg( p_wk->cap_vs[i]->act, 1 );
			// �g��k���f�[�^�쐬
			ENC_MoveReqFx( &p_wk->vsscale[i],
					FX32_CONST(2),
					FX32_CONST(1),
					ENCOUNT_SPTR_BRAIN_VSANMSYNC );
		}else{
			// �g��k���f�[�^�쐬
			ENC_MoveReqFx( &p_wk->vsscale[i],
					FX32_CONST(1),
					FX32_CONST(1),
					ENCOUNT_SPTR_BRAIN_VSANMSYNC );
		}
	}
}
static void EncountEffect_SpTr_VsAnmExit( ENCOUNT_SPTR_BRAIN_VSANM* p_wk )
{
	int i;
	for( i=0; i<ENCOUNT_SPTR_BRAIN_VSOAM_NUM; i++ ){
		CATS_ActorPointerDelete_S(p_wk->cap_vs[i]);
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	VS�A�j��	���C��
 *
 *	@param	task_w		�^�X�N���[�N
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL EncountEffect_SpTr_VsAnmMain( ENCOUNT_SPTR_BRAIN_VSANM* p_wk )
{
	int i;
	BOOL result;
	BOOL ret = TRUE;
	VecFx32 scale;
		
	// �݂�Ȃ𓮂����^�C�~���O�𐧌�
	if( p_wk->movenum < ENCOUNT_SPTR_BRAIN_VSOAM_NUM ){
		ret = FALSE;	// �܂��S�����������ĂȂ�
		p_wk->timing--;
		if( p_wk->timing <= 0 ){
			p_wk->timing = ENCOUNT_SPTR_BRAIN_VSANMTIMING;
			p_wk->movenum ++;
		}
	}

	// �������Ă����l����������
	for( i=0; i<p_wk->movenum; i++ ){
		result = ENC_MoveMainFx( &p_wk->vsscale[i] );

		scale = ENC_MakeVec( 
				p_wk->vsscale[i].x, 
				p_wk->vsscale[i].x, p_wk->vsscale[i].x );
		CLACT_SetScale( p_wk->cap_vs[i]->act, &scale );
		CLACT_SetDrawFlag( p_wk->cap_vs[i]->act, TRUE );
		// 1�ł��܂����������Ă��Ȃ��̂�����Ȃ�r��
		if( result == FALSE ){
			ret = FALSE;
		}
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���쏉����
 *
 *	@param	p_work			���샏�[�N
 *	@param	s_x				�J�nx���W
 *	@param	e_x				�I��x���W
 *	@param	count_max		�J�E���g�ő�l
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
static void ENC_MoveReqFx( ENC_MOVE_WORK_FX* p_work, fx32 s_x, fx32 e_x, int count_max )
{
	p_work->x = s_x;
	p_work->s_x = s_x;
	p_work->dis_x = e_x - s_x;
	p_work->count_max = count_max;
	p_work->count = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���상�C��
 *
 *	@param	p_work	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL ENC_MoveMainFx( ENC_MOVE_WORK_FX* p_work )
{
	fx32 w_x;

	// ���ݍ��W�擾
	w_x = FX_Mul( p_work->dis_x, p_work->count << FX32_SHIFT );
	w_x = FX_Div( w_x, p_work->count_max << FX32_SHIFT );
	
	p_work->x = w_x + p_work->s_x;

	if( (p_work->count + 1) <= p_work->count_max ){
		p_work->count++;
		return FALSE;
	}

	p_work->count = p_work->count_max;
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�x�N�g���쐬
 *
 *	@param	x	���l
 *	@param	y	���l
 *	@param	z	���l
 *
 *	@return	�x�N�g��
 */
//-----------------------------------------------------------------------------
static VecFx32 ENC_MakeVec( fx32 x, fx32 y, fx32 z )
{
	VecFx32 vec;
	vec.x = x;
	vec.y = y;
	vec.z = z;
	return vec;
}

//--------------------------------------------------------------
/**
 * @brief   �W�O�U�OBG�����[�h����
 *
 * @param   eew			
 * @param   cp_def		
 */
//--------------------------------------------------------------
static void ZigzagBG_Load(ENCOUNT_EFFECT_WORK *eew, const ENCOUNT_SPTR_BRAIN_PARAM *cp_def)
{
	//�E�B���h�E�@�\���g�p���āA�E�B���h�E�������\������Ȃ��悤�ɂ���
	GX_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
	G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | 
		GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
	G2_SetWnd1InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | 
		GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
	G2_SetWndOutsidePlane((GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | 
		GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3| GX_WND_PLANEMASK_OBJ) ^ GX_WND_PLANEMASK_BG1,
		TRUE);
	G2_SetWnd0Position(0,0,0,0);
	G2_SetWnd1Position(0,0,0,0);


	//�p���b�g�ǂݍ���
	PaletteWorkSet_Arc(eew->pfd, ARC_ENCOUNTEFFECT, 
		cp_def->bg_nclr, HEAPID_FRONTIERMAP, FADE_MAIN_BG, 0x20, 
		ZIGZAG_BG_PALNO * 16);

	ArcUtil_HDL_BgCharSet(eew->p_handle, cp_def->bg_ncgr, eew->bgl, FRMAP_FRAME_WIN, 
		0, 0, FALSE, HEAPID_FRONTIERMAP );

	// �X�N���[���f�[�^�ǂݍ���
	ArcUtil_HDL_ScrnSet(eew->p_handle, cp_def->bg_nscr, eew->bgl, 
		FRMAP_FRAME_WIN, 0, 0, 0, HEAPID_FRONTIERMAP);

	// �p���b�g�����킹��
	GF_BGL_ScrPalChange(eew->bgl, FRMAP_FRAME_WIN, 0, 0, 32, 32, ZIGZAG_BG_PALNO);

	// �X�N���[���f�[�^�]��
	GF_BGL_LoadScreenV_Req(eew->bgl, FRMAP_FRAME_WIN);
	
	{//�p���b�g�A�j���p�̃f�[�^���o�b�t�@�ɃR�s�[
		void *plbuf;
		NNSG2dPaletteData* paldata;
		
		plbuf = ArcUtil_PalDataGet(ARC_ENCOUNTEFFECT, 
			cp_def->bg_nclr, &paldata, HEAPID_FRONTIERMAP);
		MI_CpuCopy16(paldata->pRawData, eew->palbuf, ZIGZAG_BG_PAL_ANM_NUM * 32);
		sys_FreeMemoryEz(plbuf);
	}
	
	//�p���b�g�A�j���X�V�p��TCB�𐶐�
	eew->tcb_bgpalanm = TCB_Add(ZigzagPalAnmTCB, eew, TCBPRI_ENC_CUTIN_BGPALANM);
}

//--------------------------------------------------------------
/**
 * @brief   �W�O�U�OBG�̃p���b�g�A�j���X�VTCB
 *
 * @param   tcb		
 * @param   work		
 */
//--------------------------------------------------------------
static void ZigzagPalAnmTCB(TCB_PTR tcb, void *work)
{
	ENCOUNT_EFFECT_WORK *eew = work;
	
	eew->palanm_wait++;
	if(eew->palanm_wait < ZIGZAG_BGPALANM_WAIT){
		return;
	}
	eew->palanm_wait = 0;
	eew->palanm_no++;
	if(eew->palanm_no >= ZIGZAG_BG_PAL_ANM_NUM){
		eew->palanm_no = 0;
	}
	PaletteWorkSet(eew->pfd, &eew->palbuf[eew->palanm_no * 16], 
		FADE_MAIN_BG, ZIGZAG_BG_PALNO * 16, 0x20);
}



//--------------------------------------------------------------
/**
 * @brief   �E�B���h�E�@�\�̊J�����G�t�F�N�g�J�n
 *
 * @param   open_close		WINEFF_OPEN or WINEFF_CLOSE
 * @param   end_flag		�G�t�F�N�g�I����A���̃��[�N��TRUE���Z�b�g����܂�
 */
//--------------------------------------------------------------
static void WndOpenEffStart(ENCOUNT_EFFECT_WORK *eew, int open_close)
{
	WNDOPEN_EFF_WORK *wew;
	
	wew = &eew->wineff;
	MI_CpuClear8(wew, sizeof(WNDOPEN_EFF_WORK));
	
	if(open_close == WINEFF_OPEN){
		wew->y1 = WNDOPEN_EFF_CENTER_Y << 8;
		wew->y2 = WNDOPEN_EFF_CENTER_Y << 8;
		TCB_Add(WndEffOpenTask, wew, TCBPRI_ENC_CUTIN);
	}
	else{
		wew->y1 = (WNDOPEN_EFF_CENTER_Y - WNDOPEN_EFF_OPEN_Y_DOT/2) << 8;
		wew->y2 = (WNDOPEN_EFF_CENTER_Y + WNDOPEN_EFF_OPEN_Y_DOT/2) << 8;
		TCB_Add(WndEffCloseTask, wew, TCBPRI_ENC_CUTIN);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �E�B���h�E�@�\�F�I�[�v���G�t�F�N�g���s
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		WNDOPEN_EFF_WORK�\����
 */
//--------------------------------------------------------------
static void WndEffOpenTask(TCB_PTR tcb, void *work)
{
	WNDOPEN_EFF_WORK *wew = work;
	
	switch(wew->seq){
	case 0:
		wew->y1 -= WNDOPEN_EFF_ADD_Y;
		wew->y2 += WNDOPEN_EFF_ADD_Y;
		if(wew->y1 <= (WNDOPEN_EFF_CENTER_Y<<8) - ((WNDOPEN_EFF_OPEN_Y_DOT/2)<<8)){
			wew->y1 = (WNDOPEN_EFF_CENTER_Y - WNDOPEN_EFF_OPEN_Y_DOT/2) << 8;
			wew->y2 = (WNDOPEN_EFF_CENTER_Y + WNDOPEN_EFF_OPEN_Y_DOT/2) << 8;
			wew->seq++;
		}
		//left=0, right=255�ł��ƉE�[��1�h�b�g�c��̂�2���g���Ė�����͂�
		wew->wnd0_x1 = 0;
		wew->wnd0_y1 = wew->y1 >> 8;
		wew->wnd0_x2 = 255;
		wew->wnd0_y2 = wew->y2 >> 8;
		wew->wnd1_x1 = 1;
		wew->wnd1_y1 = wew->y1 >> 8;
		wew->wnd1_x2 = 0;
		wew->wnd1_y2 = wew->y2 >> 8;
		break;
	default:
		wew->end_flag = TRUE;
		TCB_Delete(tcb);
		return;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �E�B���h�E�@�\�F�N���[�Y�G�t�F�N�g���s
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		WNDOPEN_EFF_WORK�\����
 */
//--------------------------------------------------------------
static void WndEffCloseTask(TCB_PTR tcb, void *work)
{
	WNDOPEN_EFF_WORK *wew = work;
	
	switch(wew->seq){
	case 0:
		wew->y1 += WNDOPEN_EFF_ADD_Y;
		wew->y2 -= WNDOPEN_EFF_ADD_Y;
		if(wew->y1 >= (WNDOPEN_EFF_CENTER_Y<<8)){
			wew->y1 = WNDOPEN_EFF_CENTER_Y << 8;
			wew->y2 = WNDOPEN_EFF_CENTER_Y << 8;
			wew->seq++;
		}
		//left=0, right=255�ł��ƉE�[��1�h�b�g�c��̂�2���g���Ė�����͂�
		wew->wnd0_x1 = 0;
		wew->wnd0_y1 = wew->y1 >> 8;
		wew->wnd0_x2 = 255;
		wew->wnd0_y2 = wew->y2 >> 8;
		wew->wnd1_x1 = 1;
		wew->wnd1_y1 = wew->y1 >> 8;
		wew->wnd1_x2 = 0;
		wew->wnd1_y2 = wew->y2 >> 8;
		break;
	default:
		wew->end_flag = TRUE;
		TCB_Delete(tcb);
		return;
	}
}

//--------------------------------------------------------------
/**
 * @brief   ��X���C�h�C���̃��C������
 *
 * @param   task_w		
 * @param   fmw			
 *
 * @retval  TRUE:�X���C�h�C������
 */
//--------------------------------------------------------------
static BOOL FaceIn_Main(ENCOUNT_EFFECT_WORK *eew, ENCOUNT_SPTR_BRAIN* task_w, FACE_MOVE_WORK *fmw)
{
	int end = 0;
	
	switch(fmw->seq){
	case 0:
		CATS_ObjectPosSetCap_SubSurface(task_w->cap_face, 
			FACE_IN_START_X + eew->map_x, FACE_IN_START_Y + eew->map_y, 
			FMAP_SUB_ACTOR_DISTANCE);
		CATS_ObjectEnableCap(task_w->cap_face, CATS_ENABLE_TRUE);
		fmw->x = FACE_IN_START_X << 8;
		fmw->seq++;
		break;
	case 1:
		fmw->x -= FACE_IN_SPEED_X;
		if(fmw->x <= (FACE_IN_END_X << 8)){
			fmw->x = FACE_IN_END_X << 8;
			fmw->seq++;
		}
		CATS_ObjectPosSetCap_SubSurface(
			task_w->cap_face, fmw->x >> 8, FACE_IN_START_Y + eew->map_y, 
			FMAP_SUB_ACTOR_DISTANCE);
		break;
	default:
		return TRUE;
	}
	return FALSE;
}


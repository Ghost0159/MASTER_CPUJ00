//==============================================================================
/**
 * @file	codein_pv.h
 * @brief	�������̓C���^�[�t�F�[�X private�w�b�_
 * @author	goto
 * @date	2007.07.11(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#ifndef __CODEIN_PV_H__
#define __CODEIN_PV_H__

#include "codein.h"
#include "codein_gra.naix"
#include "system\font_arc.h"

#define HEAP_SIZE_CODEIN	( 0x40000 )				///< HEAP�T�C�Y
#define CODE_OAM_MAX		( 12 )					///< �����R�[�h��
#define BAR_OAM_MAX			( CODE_BLOCK_MAX - 1 )	///< �u���b�N
#define CUR_OAM_MAX			( 3 )
#define BTN_OAM_MAX			( 2 )

#define POS_CODE_Y			( 24 )

///< ���C���V�[�P���X
enum {
	
	eSEQ_INIT		= 0,
	eSEQ_INPUT,
	eSEQ_FOCUS_MOVE,
//	eSEQ_CUR_MOVE,
	eSEQ_END,
};


///< OAM�o�^�p�h�c
enum {
	
	eID_CODE_OAM	= 1000,
	eID_CUR_OAM,
	eID_BTN_OAM,
	eID_FNT_OAM,
		
};

///< �����蔻�胉�x��
enum {
	
	eHRT_CODE_0 = 0,
	eHRT_CODE_1,
	eHRT_CODE_2,
	eHRT_CODE_3,
	eHRT_CODE_4,
	eHRT_CODE_5,
	eHRT_CODE_6,
	eHRT_CODE_7,
	eHRT_CODE_8,
	eHRT_CODE_9,
	eHRT_CODE_10,
	eHRT_CODE_11,
	
	eHRT_NUM_0,
	eHRT_NUM_1,
	eHRT_NUM_2,
	eHRT_NUM_3,
	eHRT_NUM_4,
	eHRT_NUM_5,
	eHRT_NUM_6,
	eHRT_NUM_7,
	eHRT_NUM_8,
	eHRT_NUM_9,
	
	eHRT_BACK,
	eHRT_END,
	
	eHRT_MAX,
};

///< �A�j���R�[�h
enum {
	
	eANM_CODE_LN	= 0,	///< Large Nothing
	eANM_CODE_L0	= 1,	///< Large Num = 0
	eANM_CODE_SN	= 11,	///< Small Nothing
	eANM_CODE_S0	= 12,	///< Small Num = 0
	eANM_CODE_BAR	= 22,	///< bar	
	
	eANM_CUR_BAR	= 0,
	eANM_CUR_SQ,
	eANM_CUR_SQ_L,
	eANM_CUR_TOUCH,
};

///< ���[�h
enum {
	
	eMPS_SET		= 0,
	eMPS_MOVE_SET,	
};

///< �e�����p
enum {
	
	eSTATE_INPUT	= 0,
	eSTATE_FOCUS_MOVE,
	eSTATE_CUR_MOVE,	
	
	eSTATE_BLANK	= 0xFF,
};

///< PAL
enum {
	
	ePAL_WIN		= 10,
	ePAL_FRAME,
	ePAL_FONT,
	ePAL_BACK,
};

///< �T�C�Y�Ƃ�
#define L_SIZE			( 32 )	///< �ł����v���[�g�̊G�̃T�C�Y
#define S_SIZE			( 8 )	///< ���������v���[�g�̊G�̃T�C�Y
#define M_SIZE			( ( L_SIZE + S_SIZE ) / 2 )		///< 2���ׂ荇���� XXXX-xxx�Ƃ�xxxx-XXX
#define MOVE_WAIT		( 2 )
	
#define POS_4_4_4_p1	( 72 +  4 - 20 )
#define POS_4_4_4_p2	( 24 + 16 - 20 )
#define POS_4_4_4_p3	( 24 +  4 - 20 )

#define POS_2_5_5_p1	( 72 +  4 - 20 )
#define POS_2_5_5_p2	( 24 + 16 - 20 - 8 )
#define POS_2_5_5_p3	( 24 +  4 - 20 - 8 )

#define SCALE_CNT_F		( 6 )
#define SCALE_CNT_H		( SCALE_CNT_F / 2 )

#define CUR_BAR_OY		( 16 )

// -----------------------------------------
//
//	�ړ��Ǘ��p
//
// -----------------------------------------
typedef struct {
	
	s16 x;
	s16 y;
	
	u8	wait;	
	u8	scale;
	
} CODE_MOVE_WORK;


// -----------------------------------------
//
//	�R�[�h�Ǘ�OAM
//
// -----------------------------------------
typedef struct {
	
	int				state;		///< code�Ȃ琔���E����ȊO�Ȃ玩���̈ʒu
	int				group;		///< �O���[�v
	BOOL			size;		///< TRUE = laege
	
	CATS_ACT_PTR	cap;		///< OAM	
	RECT_HIT_TBL*	hit;		///< �����蔻��(�A�h���X����������ĉς�����)
	CODE_MOVE_WORK	move_wk;	///< �ړ�����
	
} CODE_OAM;


// -----------------------------------------
//
//	�e�����p���[�N
//
// -----------------------------------------
typedef struct {
	
	int		param;				///< �ǂ�ȏ��������邩	
	int		target;				///< �Ώۂ͉���
	int		work;				///< �K���Ɏg�����[�N
	
} CODEIN_STATE;


// -----------------------------------------
//
//	�V�X�e���n���[�N
//
// -----------------------------------------
typedef struct {
	
	ARCHANDLE*			p_handle;			///< �A�[�J�C�u�n���h��
	
	CATS_SYS_PTR		csp;				///< OAM�V�X�e��
	CATS_RES_PTR		crp;				///< ���\�[�X�ꊇ�Ǘ�

	GF_BGL_INI*			bgl;				///< BGL
	PALETTE_FADE_PTR	pfd;				///< �p���b�g�t�F�[�h
	
	BUTTON_MAN*			btn;				///< �{�^��	
	RECT_HIT_TBL		rht[ eHRT_MAX ];	///< �����蔻��(manager�ɓo�^����)
	
	BOOL				touch;				///< ���[�h

	FONTOAM_SYS_PTR			fontoam_sys;
	FONTOAM_OBJ_PTR			font_obj[ BTN_OAM_MAX ];
	CHAR_MANAGER_ALLOCDATA	font_vram[ BTN_OAM_MAX ];
	
	GF_BGL_BMPWIN		win;
	
} CODEIN_SYS;


// -----------------------------------------
//
//	���C�����[�N
//
// -----------------------------------------
typedef struct {
	
	CODE_OAM		code[ CODE_OAM_MAX ];
	CODE_OAM		bar[ BAR_OAM_MAX ];
	CODE_OAM		cur[ CUR_OAM_MAX ];
	CODE_OAM		btn[ BTN_OAM_MAX ];
	
	s16				x_tbl[ CODE_BLOCK_MAX + 1 ];
	u16				b_tbl[ CODE_BLOCK_MAX + 1 ][ 2 ];
	
	int				seq;
	int				wait;
	int				gene_seq;
	int				gene_cnt;
	
	int				code_max;		///< �R�[�h���͐�

	int				focus_now;		///< ���͑Ώ�
	int				focus_old;		///< ���͑Ώ�
	int				ls;				///< �g��u���b�N�J�n�ʒu
	int				le;				///< �g��u���b�N�I���ʒu
	int				ss;				///< �k���u���b�N�J�n�ʒu
	int				se;				///< �k���u���b�N�J�n�ʒu
	
	CODEIN_SYS		sys;			///< �V�X�e��
	CODEIN_STATE	state;			///< ���

	CODEIN_PARAM	param;			///< �O��������炤�p�����[�^
	
} CODEIN_WORK;

///< codein_pv.c
extern void CI_pv_ParamInit( CODEIN_WORK* wk );
extern BOOL CI_pv_MainUpdate( CODEIN_WORK* wk );
extern void CI_pv_ButtonManagerInit( CODEIN_WORK* wk );
extern void CI_pv_ButtonManagerCallBack( u32 button, u32 event, void* work );

extern void CI_pv_StateUpdate( CODEIN_WORK* wk );
extern void CI_pv_StateInit( CODEIN_WORK* wk );
extern void CI_pv_FocusSet( CODEIN_WORK* wk, int next_focus );

extern void CI_pv_SeqChange( CODEIN_WORK* wk, int seq );
extern BOOL CI_pv_MainInit( CODEIN_WORK* wk );
extern BOOL CI_pv_MainInput( CODEIN_WORK* wk );
extern BOOL CI_pv_MainFocusMove( CODEIN_WORK* wk );
extern BOOL CI_pv_MainEnd( CODEIN_WORK* wk );

extern int CI_pv_FocusTopSerach( CODEIN_WORK* wk, int next );
extern int CI_pv_FocusBottomSerach( CODEIN_WORK* wk, int next );
extern void CI_KEY_Main( CODEIN_WORK* wk );

extern void CI_pv_Input_back( CODEIN_WORK* wk );
extern void CI_pv_Input_End( CODEIN_WORK* wk );

///< codein_disp.c
extern void CI_pv_disp_CATS_Init( CODEIN_WORK* wk );
extern void CI_pv_disp_CodeRes_Load( CODEIN_WORK* wk );
extern void CI_pv_disp_CodeOAM_Create( CODEIN_WORK* wk );
extern int	CI_pv_disp_CodeAnimeGet( int state, BOOL size );
extern void CI_pv_disp_SizeFlagSet( CODEIN_WORK* wk );
extern void CI_pv_disp_MovePosSet( CODEIN_WORK* wk, int mode );

extern void CI_pv_disp_HitTableSet( CODEIN_WORK* wk );

extern void CI_pv_disp_CurOAM_Create( CODEIN_WORK* wk );
extern void CI_pv_disp_BtnOAM_Create( CODEIN_WORK* wk );

extern void CI_pv_disp_CurBar_PosSet( CODEIN_WORK* wk, int id );
extern void CI_pv_disp_CurSQ_PosSet( CODEIN_WORK* wk, int id );
extern void CI_pv_disp_CurSQ_PosSetEx( CODEIN_WORK* wk, int id, int cur_id );
extern void CI_pv_disp_CurUpdate( CODEIN_WORK* wk );
extern void CI_pv_disp_CurOAM_AnimeChange( CODEIN_WORK* wk, int no, int anime );
extern void CI_pv_disp_BtnUpdate( CODEIN_WORK* wk );

extern void CI_pv_disp_CurOAM_Visible( CODEIN_WORK* wk, int no, BOOL flag );
extern BOOL CI_pv_disp_CurOAM_VisibleGet( CODEIN_WORK* wk, int no );


extern void CI_pv_FontOam_SysInit( CODEIN_WORK* wk );
extern void CI_pv_FontOam_SysDelete( CODEIN_WORK* wk );
extern void CI_pv_FontOam_ResourceLoad( CODEIN_WORK* wk );
extern void CI_pv_FontOam_Add( CODEIN_WORK* wk );
extern void CI_pv_FontOam_Enable( CODEIN_WORK* wk, BOOL flag );
extern void CI_pv_FontOam_Create( CODEIN_WORK* wk, int no, int x, int y, int pal_offset );

extern void CI_pv_disp_BMP_WindowAdd( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win, int frm, int x, int y, int sx, int sy, int ofs );
extern void CI_pv_BMP_MsgSet( GF_BGL_BMPWIN * win, int mes_id );

extern void CI_pv_disp_CodeRes_Delete( CODEIN_WORK* wk );

#endif	///< __CODEIN_PV_H__
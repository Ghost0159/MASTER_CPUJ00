//==============================================================================
/**
 * @file	br_codein.h
 * @brief	�������̓C���^�[�t�F�[�X private�w�b�_
 * @author	goto
 * @date	2007.07.11(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#ifndef __BR_CODEIN_H__
#define __BR_CODEIN_H__

//#include "br_codein.h"
#include "application/codein/codein_gra.naix"
//#include "application/codein/codein.h"
#include "system\font_arc.h"

#include "gflib/strbuf.h"
#include "savedata/config.h"
#include "gflib/button_man.h"

#define CODE_BLOCK_MAX		( 3 )		///< ���̓u���b�N�ő吔

// -----------------------------------------
//
//	�Ăяo�����ŊǗ����郏�[�N
//
// -----------------------------------------
typedef struct {
	
//	int			heap_id;					///< �g�p����HEAPID
	int			word_len;					///< ���͕�����	
	int			block[ CODE_BLOCK_MAX + 1 ];///< ���̓u���b�N�@xx-xxxx-xxx �Ƃ�
	
	int			end_state;					///< �I�����̏��
	STRBUF*		strbuf;						///< �󗓂Ȃ�o�b�t�@�����łȂ���΃f�t�H���g�l
	
	CONFIG*		cfg;						///< window_type�擾�p
	
} BR_CODEIN_PARAM;

extern const PROC_DATA CodeInput_ProcData;

extern BR_CODEIN_PARAM*	BRCodeInput_ParamCreate( int heap_id, int word_len, int block[], CONFIG* cfg );
extern void				BRCodeInput_ParamDelete( BR_CODEIN_PARAM* codein_param );


//--------------------------------------------------------------
/**
 * @brief	xxxx-xxxx-xxxx �̃u���b�N��`�����
 *
 * @param	block[]	
 *
 * @retval	static inline	
 *
 */
//--------------------------------------------------------------
static inline void CodeIn_BlockDataMake_4_4_4( int block[] )
{
#if 1
	block[ 0 ] = 4;
	block[ 1 ] = 4;
	block[ 2 ] = 4;
#else
	block[ 0 ] = 2;
	block[ 1 ] = 5;
	block[ 2 ] = 5;
#endif
}


//--------------------------------------------------------------
/**
 * @brief	xx-xxxxx-xxxxx �̃u���b�N��`�����
 *
 * @param	block[]	
 *
 * @retval	static inline	
 *
 */
//--------------------------------------------------------------
static inline void CodeIn_BlockDataMake_2_5_5( int block[] )
{
	block[ 0 ] = 2;
	block[ 1 ] = 5;
	block[ 2 ] = 5;
}


#define HEAP_SIZE_BR_CODEIN	( 0x40000 )				///< HEAP�T�C�Y
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
	
	eBR_CODEIN_HIT_MAX,
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
	
} BR_CODEIN_STATE;


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
	RECT_HIT_TBL		rht[ eBR_CODEIN_HIT_MAX ];	///< �����蔻��(manager�ɓo�^����)
	
	BOOL				touch;				///< ���[�h

//	FONTOAM_SYS_PTR			fontoam_sys;
//	FONTOAM_OBJ_PTR			font_obj[ BTN_OAM_MAX ];
//	CHAR_MANAGER_ALLOCDATA	font_vram[ BTN_OAM_MAX ];
//	GF_BGL_BMPWIN		win;
	
} BR_CODEIN_SYS;


// -----------------------------------------
//
//	���C�����[�N
//
// -----------------------------------------
typedef struct {
	
	CODE_OAM		code[ CODE_OAM_MAX ];
	CODE_OAM		bar[ BAR_OAM_MAX ];
	CODE_OAM		cur[ CUR_OAM_MAX ];
//	CODE_OAM		btn[ BTN_OAM_MAX ];
	
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
	
	BR_CODEIN_SYS		sys;			///< �V�X�e��
	BR_CODEIN_STATE	state;			///< ���

	BR_CODEIN_PARAM	param;			///< �O��������炤�p�����[�^
	
	BOOL			push_ok;
	u64				data_number;
	
	BR_WORK*		br_work;
	
} BR_CODEIN_WORK;

#define BRCODE_MAXLEN	( 12 )


extern BR_CODEIN_PARAM* BRCodeInput_ParamCreate( int heap_id, int word_len, int block[], CONFIG* cfg );
extern void BRCodeInput_ParamDelete( BR_CODEIN_PARAM* codein_param );
extern void BRCI_pv_FocusSet( BR_CODEIN_WORK* wk, int next_focus );
extern void BRCI_pv_ParamInit( BR_CODEIN_WORK* wk );
extern void BRCI_pv_SeqChange( BR_CODEIN_WORK* wk, int seq );
extern BOOL BRCI_pv_MainInit( BR_CODEIN_WORK* wk );
extern BOOL BRCI_pv_MainEnd( BR_CODEIN_WORK* wk );
extern BOOL BRCI_pv_MainInput( BR_CODEIN_WORK* wk );
extern BOOL BRCI_pv_MainFocusMove( BR_CODEIN_WORK* wk );
extern BOOL BRCI_pv_MainUpdate( BR_CODEIN_WORK* wk );
extern void BRCI_pv_Input_End( BR_CODEIN_WORK* wk );
extern void BRCI_pv_Input_back( BR_CODEIN_WORK* wk );
extern void BRCI_pv_ButtonManagerInit( BR_CODEIN_WORK* wk );
extern void BRCI_pv_ButtonManagerCallBack( u32 button, u32 event, void* work );
extern void BRCI_pv_StateUpdate( BR_CODEIN_WORK* wk );
extern void BRCI_pv_StateInit( BR_CODEIN_WORK* wk );
extern int BRCI_pv_FocusTopSerach( BR_CODEIN_WORK* wk, int next );
extern int BRCI_pv_FocusBottomSerach( BR_CODEIN_WORK* wk, int next );
extern void BRCI_pv_disp_CodeRes_Load( BR_CODEIN_WORK* wk );
extern void BRCI_pv_disp_CodeRes_Delete( BR_CODEIN_WORK* wk );
extern void BRCI_pv_disp_CodeOAM_Create( BR_CODEIN_WORK* wk );
extern void BRCI_pv_disp_CurOAM_Create( BR_CODEIN_WORK* wk );
extern void BRCI_pv_disp_CurOAM_Visible( BR_CODEIN_WORK* wk, int no, BOOL flag );
extern BOOL BRCI_pv_disp_CurOAM_VisibleGet( BR_CODEIN_WORK* wk, int no );
extern void BRCI_pv_disp_CurOAM_AnimeChange( BR_CODEIN_WORK* wk, int no, int anime );
extern void BRCI_pv_disp_CurBar_PosSet( BR_CODEIN_WORK* wk, int id );
extern void BRCI_pv_disp_CurSQ_PosSet( BR_CODEIN_WORK* wk, int id );
extern void BRCI_pv_disp_CurSQ_PosSetEx( BR_CODEIN_WORK* wk, int id, int cur_id );
extern void BRCI_pv_disp_CurUpdate( BR_CODEIN_WORK* wk );
extern int BRCI_pv_disp_CodeAnimeGet( int state, BOOL size );
extern void BRCI_pv_disp_SizeFlagSet( BR_CODEIN_WORK* wk );
extern void BRCI_pv_disp_MovePosSet( BR_CODEIN_WORK* wk, int mode );
extern void BRCI_pv_disp_HitTableSet( BR_CODEIN_WORK* wk );


#endif	///< __BR_CODEIN_PV_H__

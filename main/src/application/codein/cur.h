//==============================================================================
/**
 * @file	cur.h
 * @brief	
 * @author	goto
 * @date	2007.07.23(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#ifndef __CUR_H__
#define __CUR_H__

//#define CUR_VBLANK_UPDATE

#define CUR_F			( 2 )
#define CUR_RAD_ADD		( 8 / CUR_F )
#define CUR_RAD			( 720 )		/// old = 720
#define CUR_MAX			( 12 )		/// 720 / 12 = 60
#define CUR_R_X			( 15 * 1 )
#define CUR_R_Y			( 15 * 1 )

///< �V�X�e��
typedef struct {
	
	ARCHANDLE*			p_handle;			///< �A�[�J�C�u�n���h��	
	CATS_SYS_PTR		csp;				///< OAM�V�X�e��
	CATS_RES_PTR		crp;				///< ���\�[�X�ꊇ�Ǘ�
	PALETTE_FADE_PTR	pfd;				///< �p���b�g�t�F�[�h	
	
} CURSOR_SYS;

///< �p�����[�^
typedef struct {
	
	int					use;
	int					heap_id;
	int					d_area;
	int					type;
	fx32				surface_y;
	
} CURSOR_PARAM;

///< �J�[�\��1��
typedef struct {
	
	CATS_ACT_PTR		cap;
	
	s16					tx;
	s16					ty;
	
	int					rot_seq;
	int					rad;
	
} CURSOR_ONE;

///< ���[�N
typedef struct {
	
	CURSOR_SYS			sys;
	CURSOR_PARAM		param;
	
	CURSOR_ONE			cur[ CUR_MAX ];
	int					seq;
	
	int					f_idx;
	int					f_cnt;	
	
	BOOL				bTcb;
	TCB_PTR				tcb;
	int					tcb_seq;
	
	int					range_rx;
	int					range_ry;
	
	int					vcount;
	
} CURSOR_WORK;




extern CURSOR_WORK*	Cursor_SystemCreate( CURSOR_PARAM param, CATS_SYS_PTR csp, CATS_RES_PTR crp, PALETTE_FADE_PTR pfd );


extern BOOL			Cursor_Create( CURSOR_WORK* wk, int color );
extern BOOL			Cursor_Active( CURSOR_WORK* wk, BOOL active );
extern BOOL			Cursor_PosSet( CURSOR_WORK* wk, s16 x, s16 y );
extern BOOL 		Cursor_PosSet_Check( CURSOR_WORK* wk, s16 x, s16 y );
extern BOOL			Cursor_PosSetFix( CURSOR_WORK* wk, s16 x, s16 y );
extern BOOL			Cursor_Delete( CURSOR_WORK* wk );

extern BOOL			Cursor_Visible( CURSOR_WORK* wk, BOOL flag );
extern void			Cursor_R_Set( CURSOR_WORK* wk, int rx, int ry );

extern void 		Cursor_ColorTypeChange( CURSOR_WORK* wk, int color );

extern void			Cursor_SizeL( CURSOR_WORK* wk );
extern void			Cursor_SizeS( CURSOR_WORK* wk );

#endif

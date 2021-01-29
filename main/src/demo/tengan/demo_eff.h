#ifndef __DEMO_EFF_H__
#define __DEMO_EFF_H__


#define DEMO_TR_DAN_HIGH_BALL_MOVE_SYNC		( 5 )	// �ړ��V���N
#define DEMO_TR_DAN_HIGH_BLOCK_SYNC			( 1 )	// ���C�v�V���N
#define DEMO_TR_DAN_HIGH_BLOCK_START_SYNC	( 1 )	// ���C�v�V���N


#define TCB_TSK_PRI	(5)
#define TCB_VWAIT_BR_TSK_PRI	(10)

//-------------------------------------
//	�ėp����
//=====================================
typedef struct {
	int x;
	int s_x;
	int dis_x;
	int count;
	int count_max;
} DEFF_MOVE_WORK;

extern void DEFF_MoveReq( DEFF_MOVE_WORK* p_work, int s_x, int e_x, int count_max );
extern BOOL	DEFF_MoveMain( DEFF_MOVE_WORK* p_work );

// �t���b�V��
extern void DEFF_EncountFlashTask(int disp, u32 bright_color, u32 sub_bright_color, BOOL* end, u32 flash_num );

//-------------------------------------
//	
//	Ͻ���u���C�g�l�X�֘A
//	
//=====================================
typedef struct {
	DEFF_MOVE_WORK brightness;
	int disp;
} BR_BRIGHTNESS_FADE;

#define DEMO_TR_DAN_HIGH_BLOCK_FILLCOLOR	( 15 )	// �h���

extern void DEFF_SetMstBrightness( int disp, int no );
extern void DEFF_ChangeMstBrightness( BR_BRIGHTNESS_FADE* p_data, int start, int end, int disp, int sync );
extern BOOL DEFF_ChangeMstBrightnessMain( BR_BRIGHTNESS_FADE* p_data );
extern void DEFF_V_SetMstBrightness( int disp, int no );


// �r�b�g�}�b�v�u���b�N��h��Ԃ��I�u�W�F�N�g
typedef struct _DEFF_BMP_FILL_BLOCK DEFF_BMP_FILL_BLOCK;

extern DEFF_BMP_FILL_BLOCK* DEFF_BMP_FillBlockAlloc( u32 heapID );
extern void DEFF_BMP_FillBlockDelete( DEFF_BMP_FILL_BLOCK* p_work );
extern void DEFF_BMP_FillBlockStart( DEFF_BMP_FILL_BLOCK* p_work, int s_x, int e_x, int s_y, int e_y, int sync, GF_BGL_BMPWIN* p_bmp, u32 width, u32 height, u8 col );
extern BOOL DEFF_BMP_FillBlockMain( DEFF_BMP_FILL_BLOCK* p_work );

// �r�b�g�}�b�v�u���b�N���ߐs�����V�X�e��
typedef struct _DEFF_BMP_FILL_BLOCK_MOVE DEFF_BMP_FILL_BLOCK_MOVE;

extern DEFF_BMP_FILL_BLOCK_MOVE* DEFF_BMP_FillBlockMoveAlloc( u32 heapID );
extern void DEFF_BMP_FillBlockMoveDelete( DEFF_BMP_FILL_BLOCK_MOVE* p_work );
extern void DEFF_BMP_FillBlockMoveStart( DEFF_BMP_FILL_BLOCK_MOVE* p_work, u8 sync, u8 start, GF_BGL_BMPWIN* p_bmp, u8 col );

extern BOOL DEFF_BMP_FillBlockMoveMain_Random( DEFF_BMP_FILL_BLOCK_MOVE* p_work );
extern BOOL DEFF_BMP_FillBlockMoveMain_LinesUP( DEFF_BMP_FILL_BLOCK_MOVE* p_work );
extern BOOL DEFF_BMP_FillBlockMoveMain_Place( DEFF_BMP_FILL_BLOCK_MOVE* p_work );


#define DEMO_LASTER_SCALE_SS		( FX32_CONST( 0.1f ) )	// �g�k�����x
#define DEMO_LASTER_SIN_R		( FX32_CONST( 18 ) )	// ���a
#define DEMO_LASTER_SP			( 4*100 )				// ���X�^�[�X�s�[�h
#define DEMO_LASTER_SIN_ADDR		( (0xffff/192) * 2 )	// ���X�^�[�p�x�X�s�[�h

#define DEMO_LASTER_TASK_PRI		( TCB_TSK_PRI - 1 )
#define DEMO_LASTER_FLIP_DOT		( 2 )


//-------------------------------------
//	���X�^�[�X�N���[���@�^�X�N�D�揇��
//=====================================
#define DEMO_EFF_LASTER_SCRLL_VWAIT_BUFF_SWITCH_TASK_PRI	( 1024 )
#define DEMO_EFF_LASTER_SCRLL_VINTR_DMA_SET_TASK_PRI	( 1024 )
#define DEMO_TR_WATER_HIGH_SIN_R		( FX32_CONST( 12 ) )	// ���a
#define DEMO_TR_WATER_HIGH_LASTER_SP	( 100 )	// ���X�^�[�X�s�[�h
#define DEMO_TR_WATER_HIGH_SIN_ADDR	( (0xffff/192) * 2 )// ���X�^�[�p�x�X�s�[�h
#define DEMO_TR_WATER_HIGH_LASTER_TASK_PRI	( 0x10000 - 1 )
#define DEMO_TR_WATER_HIGH_LASTER_FLIP_DOT	( 2 )
//-------------------------------------
//	p_laster�X�N���[�����[�N
//=====================================
typedef struct {
	LASTER_SCROLL_PTR p_laster;
	TCB_PTR VDma;
	u32 dmacount;
} DEMO_LASTER_SCROLL;


extern void DEFF_Laster_Init( DEMO_LASTER_SCROLL* p_laster, u32 heapID );
extern void DEFF_Laster_Delete( DEMO_LASTER_SCROLL* p_laster );
extern void DEFF_Laster_Start( DEMO_LASTER_SCROLL* p_laster, u8 start, u8 end, u16 add_r, fx32 r_w, s16 scr_sp, u32 bg_no, u32 init_num, u32 tsk_pri, int mode );
extern void DEFF_Laster_BuffYSet( DEMO_LASTER_SCROLL* p_laster, s16 start_y );
extern void DEFF_Laster_BuffXFlip( DEMO_LASTER_SCROLL* p_laster, u32 dot );


//-------------------------------------
//	
//	�f�t�H���g���X�^�[�V�X�e��
//
//	�R�O���̂P�t���[��Var
//
//	���̃V�X�e������邱��
//		����������		���܂Ƃ߂čs��
//		�j������		���܂Ƃ߂čs��
//		VBlank���ԏ���	���܂Ƃ߂čs��
//
//	�g���l����邱��
//		�o�b�t�@�ւ̒l�̐ݒ�
//	
//=====================================
#define FEFTOOL_DEFLASTER_BUFF_SIZE	(192)
typedef struct _FEFTOOL_DEFLASTER*			FEFTOOL_DEFLASTER_PTR;
typedef const struct _FEFTOOL_DEFLASTER*	CONST_FEFTOOL_DEFLASTER_PTR;

extern FEFTOOL_DEFLASTER_PTR FEFTool_InitDefLaster( u32 addr, u32 init_num, int heap );
extern void FEFTool_DeleteDefLaster( FEFTOOL_DEFLASTER_PTR p_lst );
extern void* FEFTool_GetDefLasterWriteBuff( CONST_FEFTOOL_DEFLASTER_PTR cp_lst );
extern const void* FEFTool_GetDefLasterReadBuff( CONST_FEFTOOL_DEFLASTER_PTR cp_lst );
extern void FEFTool_StopDefLaster( FEFTOOL_DEFLASTER_PTR p_lst );
extern void FEFTool_StartDefLaster( FEFTOOL_DEFLASTER_PTR p_lst );
extern void FEFTool_SetDefLasterTransAddr( FEFTOOL_DEFLASTER_PTR p_lst, u32 addr );
extern u32	FEFTool_GetDefLasterTransAddr( FEFTOOL_DEFLASTER_PTR p_lst );

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
//
//	���[�V�����u���[�������\����
//
//=====================================
typedef struct {
	// �O���t�B�b�N���[�h
	GXDispMode dispMode;	// �f�B�X�v���C���[�h
	GXBGMode bgMode;		// BG���[�h	
	GXBG0As bg0_2d3d;		// BG0��3d�Ɏg�p���邩

	// �L���v�`��
	GXCaptureSize sz;		// �L���v�`���T�C�Y
    GXCaptureMode mode;		// �L���v�`�����[�h
    GXCaptureSrcA a;		// �L���v�`����A
    GXCaptureSrcB b;		// �L���v�`����B
    GXCaptureDest dest;		// �L���v�`���f�[�^�]��Vram
    int eva;				// �u�����h�W��A
    int evb;				// �u�����h�W��B

	int heap_id;			// �g�p����q�[�vID
}DEMO_MOTION_BL_DATA;


typedef struct _DEMO_MOTION_BL_TASK*	DEMO_MOTION_BL_PTR;	// ���[�V�����u���[����I�u�W�F�N�g

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *
 *@brief	���[�V�����u���[���g�����Ԃɏ�����
 *
 *@param	init		�������f�[�^
 *
 *@return	DEMO_MOTION_BL_PTR	���[�V�����u���[�I�u�W�F�N�g
 *
 *
 */
//-----------------------------------------------------------------------------
extern DEMO_MOTION_BL_PTR DEMO_MOTION_BL_Init(DEMO_MOTION_BL_DATA* init);

//----------------------------------------------------------------------------
/**
 *
 *@brief	���[�V�����u���[������
 *
 *@param	data		���[�V�����u���[�I�u�W�F�N�g
 *
 *@return	none
 *	
 *	
 *
 */
//-----------------------------------------------------------------------------
extern void DEMO_MOTION_BL_Delete(DEMO_MOTION_BL_PTR* data, GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0_2d3d);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�r���Ńp�����[�^�[��ύX����
 *
 *@param	eva			�u�����h�W��A		Capture��
 *@param	evb			�u�����h�W��B		Capture�f�[�^�]����
 *
 *@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
extern void DEMO_MOTION_BL_ParamChg(DEMO_MOTION_BL_PTR data, int eva, int evb);


#endif
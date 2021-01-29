
#include "common.h"

#include "system/lib_pack.h"
#include "system/brightness.h"
#include "system/arc_util.h"
#include "system/palanm.h"
#include "system/laster.h"

#include "demo_eff.h"

#include "system/lib_pack.h"



//-----------------------------------------------------------------------------
//
//	�ŏ��̃s�J�s�J�p�^�X�N�J�n�֐�
//
//-----------------------------------------------------------------------------


static void DEFF_BrightnessVset( TCB_PTR tcb, void* work );

typedef struct{
	int seq;
	u32 flash_num;
	int count;
	int disp;		// ��ʎw��
	u32 end_bright;
	u32 sub_bright;
	BR_BRIGHTNESS_FADE br_main;
	BR_BRIGHTNESS_FADE br_sub;
	BOOL* end;		// �I�������p
} BR_FLASH_TASK;
#define FLASH_EFF_SYNC_SUB	( 8 )
#define FLASH_EFF_SYNC_ONE	( 3 )

// �J�n�G�t�F�N�g�^�X�N�i�ŏ��̌�����̂Ȃǂ𐧌�j
static void BR_FlashTask(TCB_PTR tcb, void *work);

enum{
	FADE_SUB_EFFECT,
	FADE_EFFECT,
	FADE_WAIT,
	FADE_RET_EFFECT,
	FADE_RET_WAIT,
	END_EFFECT,
};

//----------------------------------------------------------------------------
/**
 *
 *@brief	�ŏ��̃t���b�V���̕\��������^�X�N�����s����
 *					
 *@param	disp	���C����ʂɂ�����:MASK_MAIN_DISPLAY
 *					�T�u��ʂɂ�����:MASK_SUB_DISPLAY
 *					����ʂɂ�����:MASK_DOUBLE_DISPLAY
 *
 *@param	end		�I��������TRUE��Ԃ��t���O
 *@param	flash_num	�t���b�V����
 *
 *@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
void DEFF_EncountFlashTask(int disp, u32 bright_color, u32 sub_bright_color, BOOL* end, u32 flash_num )
{
	TCB_PTR tcb;
	BR_FLASH_TASK *eew;

	eew = sys_AllocMemory( HEAPID_WORLD, sizeof(BR_FLASH_TASK) );
	memset( eew, 0, sizeof(BR_FLASH_TASK) );
	TCB_Add( BR_FlashTask, eew, TCB_TSK_PRI );
	
	eew->end = end;		// �I���t���O�ݒ�
	if(eew->end != NULL){
		*(eew->end) = FALSE;
	}
	eew->disp = disp;
	eew->end_bright = bright_color;
	eew->sub_bright = sub_bright_color;
	eew->flash_num	= flash_num;
}

//------------------------------------------------------------------
/**
 * 
 * �G�t�F�N�g
 *
 * �ŏ��̃s�J�s�J�\��
 * 
 * @param   tcb		�^�X�N�|�C���^
 * @param   work	���[�N�|�C���^
 *
 * @retval  none		
 *
 * �s�J�s�J�@20�V���N�g�p
 */
//------------------------------------------------------------------
static void BR_FlashTask(TCB_PTR tcb, void *work)
{
	BR_FLASH_TASK *eew = work;

	switch(eew->seq){
	case FADE_SUB_EFFECT:
		if(eew->disp == MASK_MAIN_DISPLAY){
			//�u���b�N�A�E�g
//			ChangeBrightnessRequest( 40,-16,0, eew->plane, MASK_SUB_DISPLAY);
			DEFF_ChangeMstBrightness( &eew->br_sub, 0, eew->sub_bright, MASK_SUB_DISPLAY, FLASH_EFF_SYNC_SUB );
		}else{
			if(eew->disp == MASK_SUB_DISPLAY){
				//�u���b�N�A�E�g
//				ChangeBrightnessRequest(40,-16,0, eew->plane, MASK_MAIN_DISPLAY);
				DEFF_ChangeMstBrightness( &eew->br_sub, 0, eew->sub_bright, MASK_MAIN_DISPLAY, FLASH_EFF_SYNC_SUB );
			}
		}
		eew->seq++;
		break;

	case FADE_EFFECT:
		//�z���C�g�A�E�g
//		ChangeBrightnessRequest(4,16,0,eew->plane,eew->disp);
		DEFF_ChangeMstBrightness( &eew->br_main, 0, eew->end_bright, eew->disp, FLASH_EFF_SYNC_ONE );
		eew->seq++;
		break;
		
	case FADE_WAIT:
		if( DEFF_ChangeMstBrightnessMain( &eew->br_main ) ){
			eew->seq++;
		}
		break;
	case FADE_RET_EFFECT:
		//�z���C�g�C��
//		ChangeBrightnessRequest(4,0,16,eew->plane,eew->disp);
		DEFF_ChangeMstBrightness( &eew->br_main, eew->end_bright, 0, eew->disp, FLASH_EFF_SYNC_ONE );
		eew->seq++;
		break;
		
	case FADE_RET_WAIT:
		if( DEFF_ChangeMstBrightnessMain( &eew->br_main ) ){
			
			eew->count++;
			if(eew->count==eew->flash_num){
				eew->seq = END_EFFECT;
			}else{
				eew->seq = FADE_EFFECT;
			}
		}
		break;
	case END_EFFECT:		// ���C���G�t�F�N�g�Ɉړ�
		eew->seq = 0;
		eew->count = 0;

		if(eew->end != NULL){
			*(eew->end) = TRUE;		// �^�X�N�I�����
		}

		TCB_Delete(tcb);	//�^�X�N�I��
		sys_FreeMemoryEz( eew );
		return ;
	}

	// �T�u�ʃt�F�[�h
	DEFF_ChangeMstBrightnessMain( &eew->br_sub );
}


//----------------------------------------------------------------------------
/**
 *	@brief	Ͻ���u���C�g�l�X�ݒ�
 *
 *	@param	disp	��
 *	@param	no		�l
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void DEFF_SetMstBrightness( int disp, int no )
{
	// �u���C�g�l�X����
	if( disp == MASK_MAIN_DISPLAY ){
		GX_SetMasterBrightness(no);
	}else{
		GXS_SetMasterBrightness(no);
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	Ͻ���u���C�g�l�X�ύX�@�t�F�[�h�f�[�^�i�[
 *
 *	@param	p_data		�i�[��
 *	@param	start		�J�n�l
 *	@param	end			�I���l
 *	@param	disp		��
 *	@param	sync		�V���N��
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void DEFF_ChangeMstBrightness( BR_BRIGHTNESS_FADE* p_data, int start, int end, int disp, int sync )
{
	p_data->disp = disp;
	DEFF_MoveReq( &p_data->brightness, start, end, sync );
}

//----------------------------------------------------------------------------
/**
 *	@brief	Ͻ���u���C�g�l�X�ύX�@�t�F�[�h���C��
 *
 *	@param	p_data	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL DEFF_ChangeMstBrightnessMain( BR_BRIGHTNESS_FADE* p_data )
{
	BOOL result;
	result = DEFF_MoveMain( &p_data->brightness );
	
	// V�u�����N���Ԓ��ɐݒ肵�Ȃ��Ƃ��ꂢ�Ƀt�F�[�h���Ȃ�
	VWaitTCB_Add( DEFF_BrightnessVset, p_data, TCB_VWAIT_BR_TSK_PRI );
	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	V�u�����N���Ԓ�����ײ�Ƚ�l��ݒ肷��^�X�N
 *
 *	@param	tcb		TCB���[�N
 *	@param	work	���[�N
 *
 *	@return	none
 *	V�u�����N���Ԓ��ɐݒ肵�Ȃ��Ƃ��ꂢ�Ƀt�F�[�h���Ȃ�
 */
//-----------------------------------------------------------------------------
static void DEFF_BrightnessVset( TCB_PTR tcb, void* work )
{
	BR_BRIGHTNESS_FADE* p_data = work;
	DEFF_SetMstBrightness( p_data->disp, p_data->brightness.x );
	TCB_Delete( tcb );
}




//-----------------------------------------------------------------------------
//
//	�r�b�g�}�b�v����
//
//-----------------------------------------------------------------------------
// �r�b�g�}�b�v�E�B���h�E�h��Ԃ��I�u�W�F�N�g
typedef struct _DEFF_BMP_FILL_OBJ{
	DEFF_MOVE_WORK move_x;
	DEFF_MOVE_WORK move_y;
	GF_BGL_BMPWIN* p_bmp;
	u8 width;
	u8 height;
	u8 move_flg;
	u8 col;
} ;

// �r�b�g�}�b�v�E�B���h�E�u���b�N��h��Ԃ��I�u�W�F�N�g
typedef struct _DEFF_BMP_FILL_BLOCK{
	DEFF_MOVE_WORK move_x;
	DEFF_MOVE_WORK move_y;
	GF_BGL_BMPWIN* p_bmp;
	u8 width;
	u8 height;
	u8 move_flg;
	u8 col;
} ;

static void DEFF_BMP_Fill( GF_BGL_BMPWIN* p_bmp, s32 top, s32 bottom, s32 left, s32 right, u8 col );

// �r�b�g�}�b�v�u���b�N���ߐs�����V�X�e��
#define DEMO_BLOCK_MOVE_WIDTH	( 32 )	// �u���b�N�T�C�Y
#define DEMO_BLOCK_MOVE_HEIGHT	( 32 )	// �u���b�N�T�C�Y
#define DEMO_BLOCK_MOVE_MAT_Y	( -16 )	// �J�n���W
#define DEMO_BLOCK_MOVE_MOVE_Y	( 192 + DEMO_BLOCK_MOVE_MAT_Y )	// �ړ�����
#define DEMO_BLOCK_MOVE_OBJNUM	( 48 )	// x�u���b�N�Ŗ��߂�
#define DEMO_BLOCK_MOVE_X_NUM	( 8 )	// ���̐�
#define DEMO_BLOCK_MOVE_Y_NUM	( 6 )	// �c�̐�

static const u8 DEMO_BLOCK_MOVE_MoveParam_Random[ DEMO_BLOCK_MOVE_X_NUM ] = {	// �o�Ă���x���W�̏��ԃe�[�u��
	 0, 2, 5, 7, 1, 6, 3, 4 
};

static const u8 DEMO_BLOCK_MOVE_MoveParam_LinesUP[][ DEMO_BLOCK_MOVE_X_NUM ] = {	// �o�Ă���x���W�̏��ԃe�[�u��
	{ 0, 1, 2, 3, 4, 5, 6, 7, },
	{ 7, 6, 5, 4, 3, 2, 1, 0, },
};


typedef struct _DEFF_BMP_FILL_BLOCK_MOVE{
	GF_BGL_BMPWIN* p_bmp;
	DEFF_BMP_FILL_BLOCK* p_block[ DEMO_BLOCK_MOVE_OBJNUM ];
	u8	block_count;
	u8	line_x;
	u8	line_y;
	u8	block_line;
	u8	block_end_count;
	u8	move_sync;
	u8	start_sync;
	s8	wait;
	u8	col;
	u8	init_flg;
	u8	dummy[1];
};



//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	p_work	
 * @param	s_x	
 * @param	e_x	
 * @param	count_max	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void DEFF_MoveReq( DEFF_MOVE_WORK* p_work, int s_x, int e_x, int count_max )
{
	p_work->x = s_x;
	p_work->s_x = s_x;
	p_work->dis_x = e_x - s_x;
	p_work->count_max = count_max;
	p_work->count = 0;
}


//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	p_work	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL DEFF_MoveMain( DEFF_MOVE_WORK* p_work )
{
	int w_x;
	
	w_x = p_work->dis_x * p_work->count;
	w_x = w_x / p_work->count_max;
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
 *		�y�r�b�g�}�b�v�u���b�N��h��Ԃ��I�u�W�F�N�g�z
 *	@brief	���[�N�̊m��
 *
 *	@param	heapID	�q�[�vID
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
DEFF_BMP_FILL_BLOCK* DEFF_BMP_FillBlockAlloc( u32 heapID )
{
	DEFF_BMP_FILL_BLOCK* p_work;

	p_work = sys_AllocMemory( heapID, sizeof(DEFF_BMP_FILL_BLOCK) );
	memset( p_work, 0, sizeof(DEFF_BMP_FILL_BLOCK) );
	return p_work;
}

//----------------------------------------------------------------------------
/**
 *		�y�r�b�g�}�b�v�u���b�N��h��Ԃ��I�u�W�F�N�g�z
 *	@brief	���[�N�j��
 *
 *	@param	p_work	���[�N
 */
//-----------------------------------------------------------------------------
void DEFF_BMP_FillBlockDelete( DEFF_BMP_FILL_BLOCK* p_work )
{
	sys_FreeMemoryEz( p_work );
}

//----------------------------------------------------------------------------
/**
 *		�y�r�b�g�}�b�v�u���b�N��h��Ԃ��I�u�W�F�N�g�z
 *	@brief	�����J�n
 *
 *	@param	p_work		���[�N
 *	@param	s_x			�h��Ԃ��J�n�����W
 *	@param	e_x			�h��Ԃ��I�������W
 *	@param	s_y			�h��Ԃ��J�n�����W
 *	@param	e_y			�h��Ԃ��I�������W
 *	@param	sync		�V���N��
 *	@param	p_bmp		�r�b�g�}�b�v
 *	@param	width		��
 *	@param	height		����
 *	@param	col			�h��Ԃ��p���b�g�ԍ�
 */
//-----------------------------------------------------------------------------
void DEFF_BMP_FillBlockStart( DEFF_BMP_FILL_BLOCK* p_work, int s_x, int e_x, int s_y, int e_y, int sync, GF_BGL_BMPWIN* p_bmp, u32 width, u32 height, u8 col )
{
	GF_ASSERT( p_work->move_flg == FALSE );

	// �����p�����[�^
	DEFF_MoveReq( &p_work->move_x, s_x, e_x, sync );
	DEFF_MoveReq( &p_work->move_y, s_y, e_y, sync );

	// ���̑��p�����[�^
	p_work->p_bmp	= p_bmp;	// �h��Ԃ��r�b�g�}�b�v
	p_work->width	= width;	// �h��Ԃ���
	p_work->height	= height;	// �h��Ԃ�����
	p_work->col		= col;		// �h��Ԃ��J���[�ԍ�

	p_work->move_flg = TRUE;
}

//----------------------------------------------------------------------------
/**
 *		�y�r�b�g�}�b�v�u���b�N��h��Ԃ��I�u�W�F�N�g�z
 *	@brief	�������C��
 *
 *	@param	p_work	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL DEFF_BMP_FillBlockMain( DEFF_BMP_FILL_BLOCK* p_work )
{
	BOOL result;
	s16 top, bottom, left, right;
	if( p_work->move_flg == FALSE ){
		return TRUE;
	}

	// 1�O������
	left = p_work->move_x.x - (p_work->width/2);
	top = p_work->move_y.x - (p_work->height/2);
	right = left + p_work->width;
	bottom = top + p_work->height;
//	DEFF_BMP_Fill( p_work->p_bmp, top, bottom, left, right, 0 );
	
	result = DEFF_MoveMain( &p_work->move_x );
	DEFF_MoveMain( &p_work->move_y );

	left = p_work->move_x.x - (p_work->width/2);
	top = p_work->move_y.x - (p_work->height/2);
	right = left + p_work->width;
	bottom = top + p_work->height;

	DEFF_BMP_Fill( p_work->p_bmp, top, bottom, left, right, p_work->col );

	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v�E�B���h�E�h��Ԃ��@��`
 *
 *	@param	p_bmp		���[�N
 *	@param	top			��
 *	@param	bottom		��
 *	@param	left		��
 *	@param	right		�E
 *	@param	col			�J���[�ԍ�
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
static void DEFF_BMP_Fill( GF_BGL_BMPWIN* p_bmp, s32 top, s32 bottom, s32 left, s32 right, u8 col )
{
	// ��ʊO�`�F�b�N
	if( (right <= 0) || (bottom <= 0) ){
		return ;
	}

	// �ꏏ�Ȃ�`�悵�Ȃ�
	if( (left == right) || (top == bottom) ){
		return ;
	}
	
	if( left < 0 ){
		left = 0;
	}
	if( right > 256 ){
		right = 256;
	}
	if( top < 0 ){
		top = 0;
	}
	if( bottom > 256 ){
		bottom = 256;
	}

	// �r�b�g�}�b�v�h��Ԃ�
	GF_BGL_BmpWinFill( p_bmp, col,
			left,
			top,
			right - left,
			bottom - top );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�m��
 *
 *	@param	heapID	�q�[�v
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
DEFF_BMP_FILL_BLOCK_MOVE* DEFF_BMP_FillBlockMoveAlloc( u32 heapID )
{
	DEFF_BMP_FILL_BLOCK_MOVE* p_work;
	int i;

	p_work = sys_AllocMemory( heapID, sizeof(DEFF_BMP_FILL_BLOCK_MOVE) );
	memset( p_work, 0, sizeof(DEFF_BMP_FILL_BLOCK_MOVE) );

	// �I�u�W�F�N�g�쐬
	for( i=0; i<DEMO_BLOCK_MOVE_OBJNUM; i++ ){
		p_work->p_block[i] = DEFF_BMP_FillBlockAlloc( heapID );
	}
	return p_work;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�j��
 *
 *	@param	p_work	���[�N
 */
//-----------------------------------------------------------------------------
void DEFF_BMP_FillBlockMoveDelete( DEFF_BMP_FILL_BLOCK_MOVE* p_work )
{
	int i;
	
	// �u���b�N�I�u�W�F�j��
	for( i=0; i<DEMO_BLOCK_MOVE_OBJNUM; i++ ){
		DEFF_BMP_FillBlockDelete( p_work->p_block[i] );
	}
	sys_FreeMemoryEz( p_work );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����J�n
 *	
 *	@param	p_work	���[�N
 *	@param	sync	�P�̃I�u�W�F�������V���N��
 *	@param	start	�P�̃I�u�W�F���˃^�C�~���O
 *	@param	p_bmp	�r�b�g�}�b�v
 *	@param	col		�J���[
 */
//-----------------------------------------------------------------------------
void DEFF_BMP_FillBlockMoveStart( DEFF_BMP_FILL_BLOCK_MOVE* p_work, u8 sync, u8 start, GF_BGL_BMPWIN* p_bmp, u8 col )
{
	p_work->p_bmp		= p_bmp;
	p_work->col			= col;
	p_work->move_sync	= sync;
	p_work->start_sync	= start;
	p_work->block_count = 0;
	p_work->line_x		= 0;
	p_work->line_y		= 0;
	p_work->block_line  = 0;
	p_work->block_end_count = 0;
	p_work->wait		= 0;
	p_work->init_flg	= 1;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C���֐�
 *
 *	@param	p_work	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
///< �����_���ɕ~���l�߂��Ă���
BOOL DEFF_BMP_FillBlockMoveMain_Random( DEFF_BMP_FILL_BLOCK_MOVE* p_work )
{
	int x, s_y, e_y;
	int i;
	BOOL result;
	
	if( p_work->init_flg == 0 ){
		return TRUE;
	}
	
	// ���˕�
	if( p_work->block_count < DEMO_BLOCK_MOVE_OBJNUM ){
		p_work->wait --;
		if( p_work->wait <= 0 ){
			p_work->wait = p_work->start_sync;

			x = DEMO_BLOCK_MOVE_MoveParam_Random[ p_work->block_count % DEMO_BLOCK_MOVE_X_NUM ];
			x = (DEMO_BLOCK_MOVE_WIDTH/2) + (x * DEMO_BLOCK_MOVE_WIDTH);
			s_y = DEMO_BLOCK_MOVE_MAT_Y - ((p_work->block_count/DEMO_BLOCK_MOVE_X_NUM) * DEMO_BLOCK_MOVE_HEIGHT);
			e_y = DEMO_BLOCK_MOVE_MOVE_Y - ((p_work->block_count/DEMO_BLOCK_MOVE_X_NUM) * DEMO_BLOCK_MOVE_HEIGHT);
			// ����
			DEFF_BMP_FillBlockStart( 
					p_work->p_block[ p_work->block_count ],
					x, x,
					s_y, e_y,
					p_work->move_sync, p_work->p_bmp,
					DEMO_BLOCK_MOVE_WIDTH, DEMO_BLOCK_MOVE_HEIGHT,
					p_work->col );

			p_work->block_count ++;
		}
	}
	

	// ������
	for( i=p_work->block_end_count; i<p_work->block_count; i++ ){
		result = DEFF_BMP_FillBlockMain( p_work->p_block[i] );
		if( result == TRUE ){
			p_work->block_end_count++;
		}
	}

	// �S�����I���`�F�b�N
	if( (p_work->block_end_count >= DEMO_BLOCK_MOVE_OBJNUM) &&
		(result == TRUE) ){
		p_work->init_flg = FALSE;
		return TRUE;
	}

	return FALSE;
}

///< �݂��Ⴂ�Ƀ��C������ɏオ��
BOOL DEFF_BMP_FillBlockMoveMain_LinesUP( DEFF_BMP_FILL_BLOCK_MOVE* p_work )
{
	int x, s_y, e_y;
	int i;
	BOOL result;
	
	if( p_work->init_flg == 0 ){
		return TRUE;
	}
	
	// ���˕�
	if( p_work->block_count < DEMO_BLOCK_MOVE_OBJNUM ){
		p_work->wait --;
		if( p_work->wait <= 0 ){
			p_work->wait = p_work->start_sync;

			x = DEMO_BLOCK_MOVE_MoveParam_LinesUP[ p_work->block_line ][ p_work->block_count % DEMO_BLOCK_MOVE_X_NUM ];
			x = (DEMO_BLOCK_MOVE_WIDTH/2) + (x * DEMO_BLOCK_MOVE_WIDTH);
			s_y = DEMO_BLOCK_MOVE_MAT_Y - ((p_work->block_count/DEMO_BLOCK_MOVE_X_NUM) * DEMO_BLOCK_MOVE_HEIGHT);
			e_y = DEMO_BLOCK_MOVE_MOVE_Y - ((p_work->block_count/DEMO_BLOCK_MOVE_X_NUM) * DEMO_BLOCK_MOVE_HEIGHT);
			// ����
			DEFF_BMP_FillBlockStart( 
					p_work->p_block[ p_work->block_count ],
					x, x,
					s_y, e_y,
					p_work->move_sync, p_work->p_bmp,
					DEMO_BLOCK_MOVE_WIDTH, DEMO_BLOCK_MOVE_HEIGHT,
					p_work->col );
			
			p_work->block_count ++;
			
			if ( ( p_work->block_count % DEMO_BLOCK_MOVE_X_NUM ) == 0 ){
				p_work->block_line ^= 1;
			}
		}
	}
	

	// ������
	for( i=p_work->block_end_count; i<p_work->block_count; i++ ){
		result = DEFF_BMP_FillBlockMain( p_work->p_block[i] );
		if( result == TRUE ){
			p_work->block_end_count++;
		}
	}

	// �S�����I���`�F�b�N
	if( (p_work->block_end_count >= DEMO_BLOCK_MOVE_OBJNUM) &&
		(result == TRUE) ){
		p_work->init_flg = FALSE;
		return TRUE;
	}

	return FALSE;
}


///< �u���b�N����
static const u8 PlaceTable[][ 6 ] = {
	{ 0, 1, 2, 3, 4, 5 },
	{ 5, 4, 3, 2, 1, 0 },
};
BOOL DEFF_BMP_FillBlockMoveMain_Place( DEFF_BMP_FILL_BLOCK_MOVE* p_work )
{
	int x, s_y, e_y;
	int i;
	BOOL result;
	
	if( p_work->init_flg == 0 ){
		return TRUE;
	}
	
	// ���˕�
	if( p_work->block_count < DEMO_BLOCK_MOVE_OBJNUM ){
		p_work->wait --;
		if( p_work->wait <= 0 ){
			p_work->wait = p_work->start_sync;

			x	= p_work->line_x;
			x	= ( DEMO_BLOCK_MOVE_WIDTH / 2 ) + ( x * DEMO_BLOCK_MOVE_WIDTH );
			s_y = DEMO_BLOCK_MOVE_HEIGHT * ( PlaceTable[ p_work->block_line ][ p_work->line_y ] * 1);//DEMO_BLOCK_MOVE_HEIGHT );
			e_y = s_y + DEMO_BLOCK_MOVE_HEIGHT;
			
//			OS_Printf( "2:x = %3d  sy = %3d  ey = %3d\n", x, s_y, e_y );

			DEFF_BMP_FillBlockStart( 
					p_work->p_block[ p_work->block_count ],
					x, x,
					s_y, e_y,
					p_work->move_sync, p_work->p_bmp,
					DEMO_BLOCK_MOVE_WIDTH, DEMO_BLOCK_MOVE_HEIGHT,
					p_work->col );
			
			x	= 7 - p_work->line_x;
			x	= ( DEMO_BLOCK_MOVE_WIDTH / 2 ) + ( x * DEMO_BLOCK_MOVE_WIDTH );
			s_y = DEMO_BLOCK_MOVE_HEIGHT * ( PlaceTable[ p_work->block_line ^ 1 ][ p_work->line_y ] * 1);//DEMO_BLOCK_MOVE_HEIGHT );
			e_y = s_y + DEMO_BLOCK_MOVE_HEIGHT;
			
//			OS_Printf( "2:x = %3d  sy = %3d  ey = %3d\n", x, s_y, e_y );

			DEFF_BMP_FillBlockStart( 
					p_work->p_block[ p_work->block_count+1 ],
					x, x,
					s_y, e_y,
					p_work->move_sync, p_work->p_bmp,
					DEMO_BLOCK_MOVE_WIDTH, DEMO_BLOCK_MOVE_HEIGHT,
					p_work->col );
					
			p_work->block_count++;
			p_work->block_count++;
			p_work->line_y++;
			
			if ( ( p_work->line_y % 6 ) == 0 ){
				p_work->block_line ^= 1;
				p_work->line_x++;
				p_work->line_y = 0;
			}
		}
	}
	

	// ������
	for( i=p_work->block_end_count; i<p_work->block_count; i++ ){
		result = DEFF_BMP_FillBlockMain( p_work->p_block[i] );
		if( result == TRUE ){
			p_work->block_end_count++;
		}
	}

	// �S�����I���`�F�b�N
	if( (p_work->block_end_count >= DEMO_BLOCK_MOVE_OBJNUM) &&
		(result == TRUE) ){
		p_work->init_flg = FALSE;
		return TRUE;
	}

	return FALSE;
}








static TCB_PTR EncountEffect_LasterScroll_VDmaTaskReq( DEMO_LASTER_SCROLL* p_lstscr );
static void EncountEffect_LasterScroll_VDmaTCB( TCB_PTR tcb, void* p_work );
//static void DEFF_Laster_Init( DEMO_LASTER_SCROLL* p_laster, u32 heapID );
//static void DEFF_Laster_Delete( DEMO_LASTER_SCROLL* p_laster );
//static void DEFF_Laster_Start( DEMO_LASTER_SCROLL* p_laster, u8 start, u8 end, u16 add_r, fx32 r_w, s16 scr_sp, u32 bg_no, u32 init_num, u32 tsk_pri );
//static void DEFF_Laster_BuffYSet( DEMO_LASTER_SCROLL* p_laster, s16 start_y );
//static void DEFF_Laster_BuffXFlip( DEMO_LASTER_SCROLL* p_laster, u32 dot );

//----------------------------------------------------------------------------
/**
 *	@brief	���X�^�[�X�N���[���V�X�e���@V�u�����NDMA�]�����N�G�X�g�^�X�N
 *
 *	@param	p_lstscr	���X�^�[�X�N���[���V�X�e��
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
static TCB_PTR EncountEffect_LasterScroll_VDmaTaskReq( DEMO_LASTER_SCROLL* p_lstscr )
{
	return VIntrTCB_Add( EncountEffect_LasterScroll_VDmaTCB,
			p_lstscr, DEMO_EFF_LASTER_SCRLL_VINTR_DMA_SET_TASK_PRI );
}

//----------------------------------------------------------------------------
/**
 *	@brief	DMA�]���^�X�N
 *
 *	@param	tcb		�^�X�N���[�N
 *	@param	p_work	���[�N
 *
 *	@return
 */
//-----------------------------------------------------------------------------
static void EncountEffect_LasterScroll_VDmaTCB( TCB_PTR tcb, void* p_work )
{
	DEMO_LASTER_SCROLL* p_lstscr = p_work;

	if( p_lstscr->dmacount >= 2 ){
		LASTER_ScrollVBuffSwitch( p_lstscr->p_laster );
		p_lstscr->dmacount = 0;
	}

	LASTER_ScrollVDma( p_lstscr->p_laster );
	p_lstscr->dmacount ++;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���X�^�[������
 *
 *	@param	p_laster	���X�^�[���[�N
 *	@param	heapID		�q�[�vID
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void DEFF_Laster_Init( DEMO_LASTER_SCROLL* p_laster, u32 heapID )
{
	p_laster->p_laster = LASTER_ScrollInit( heapID );
	p_laster->dmacount = 0;
	p_laster->VDma = EncountEffect_LasterScroll_VDmaTaskReq( p_laster );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�^�[�j��
 *
 *	@param	p_laster	���X�^�[���[�N
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void DEFF_Laster_Delete( DEMO_LASTER_SCROLL* p_laster )
{
	TCB_Delete( p_laster->VDma );
	LASTER_ScrollEnd( p_laster->p_laster );
	LASTER_ScrollDelete( p_laster->p_laster );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�^�[�J�n
 *
 *	@param	p_laster	���[�N
 *	@param	start		���X�^�[�X�N���[��������J�n�����W
 *	@param	end			���X�^�[�X�N���[��������I�������W
 *	@param	add_r		sin�J�[�u�e�[�u���쐬���̊p�x�𑫂��Ă����l(0�`65535)
 *	@param	r_w			sin�J�[�u�̔��a
 *	@param	scr_sp		���X�^�[�X�N���[���X�s�[�h	����*100�����l	�}�C�i�X�L��
 *	@param	bg_no		���X�^�[��������BG�̔ԍ�
 *	@param	init_num	���[�N������������l
 *	@param	tsk_pri		�^�X�N�D�揇��
 *
 *	@return	none
 *
 * bg_no
 * 		LASTER_SCROLL_MBG0,
 *		LASTER_SCROLL_MBG1,
 *		LASTER_SCROLL_MBG2,
 *		LASTER_SCROLL_MBG3,
 *		LASTER_SCROLL_SBG0,
 *		LASTER_SCROLL_SBG1,
 *		LASTER_SCROLL_SBG2,
 *		LASTER_SCROLL_SBG3
 */
//-----------------------------------------------------------------------------
void DEFF_Laster_Start( DEMO_LASTER_SCROLL* p_laster, u8 start, u8 end, u16 add_r, fx32 r_w, s16 scr_sp, u32 bg_no, u32 init_num, u32 tsk_pri, int mode )
{
	if ( mode == 0 ){
		LASTER_ScrollStart( p_laster->p_laster, start, end, add_r, r_w, scr_sp,	bg_no, init_num, tsk_pri );
	}
	else {
		LASTER_YScrollStart( p_laster->p_laster, start, end, add_r, r_w, scr_sp,	bg_no, init_num, tsk_pri );
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	���X�^�[�J�nY�ʒu�ݒ�
 *
 *	@param	p_laster
 *	@param	start_y 
 *
 *	@return
 */
//-----------------------------------------------------------------------------
void DEFF_Laster_BuffYSet( DEMO_LASTER_SCROLL* p_laster, s16 start_y )
{
	u32* p_buff;
	u32* p_write;
	int i;
	u32 num;

	p_write = sys_AllocMemory( HEAPID_WORLD, sizeof(u32) * LASTER_SCROLL_BUFF_SIZE );
	memset( p_write, 9, sizeof(u32) * LASTER_SCROLL_BUFF_SIZE );

	p_buff = LASTER_GetScrollWriteBuff( p_laster->p_laster );
	
	for( i=0; i<192; i++ ){
		if( ((-start_y + i) >= 0) &&
			((-start_y + i) < 192 ) ){
			num = p_buff[ (-start_y + i) ];
		}else{
			num = 0;
		}
		p_write[i] = num;
	}

	memcpy( p_buff, p_write, sizeof(u32) * LASTER_SCROLL_BUFF_SIZE );
	
	sys_FreeMemoryEz( p_write );	
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�^�[�̉����W��dot���Ƃɔ��]������
 *
 *	@param	p_laster	���X�^�[���[�N
 *	@param	dot			�h�b�g��
 */
//-----------------------------------------------------------------------------
void DEFF_Laster_BuffXFlip( DEMO_LASTER_SCROLL* p_laster, u32 dot )
{
	u32* p_buff;
	int i;
	s16 x;
	
	p_buff = LASTER_GetScrollWriteBuff( p_laster->p_laster );

	for( i=0; i<192; i++ ){
		x = p_buff[i] & 0xffff;
		if( ((i / dot) % 2) == 0 ){
			p_buff[i] = (x & 0xffff);
		}else{
			p_buff[i] = (-x & 0xffff);
		}
	}
}










//-----------------------------------------------------------------------------
/**
 *		���X�^�[�p�V�X�e��		LASTER
 */
//-----------------------------------------------------------------------------
//-------------------------------------
//	
//	HDMA�R���g���[��
//	
//=====================================
typedef void (*pFEFTool_LSVBCFunc)( void* work );
// 1/30 ���J�o�[����V�X�e��
typedef struct _FEFTOOL_LASTER_VBLANK_CONT{
	TCB_PTR		vdma_tcb;
	TCB_PTR		vbuff_tcb;
	BOOL vblank_lst_flg;// VBlank�ōs���������s�����̃t���O
	BOOL dma_set_flg;	//���C�����[�v����V�u���������`�F�b�N����t���O
	
	pFEFTool_LSVBCFunc	vblank1;	// �R�O�t���[���̒ʏ��V�u�����N�p����
	pFEFTool_LSVBCFunc	vblank2;	// �r����V�u�����N�p����
	void * work;
} FEFTOOL_LSVBC;

//----------------------------------------------------------------------------
/**
 *
 *	@brief	�ʏ�V�u�����N
 *
 *	@param	tcb		TCB���[�N
 *	@param	work	���[�N
 *	
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
static void FEFTool_LSVBC_VBlank1( TCB_PTR tcb, void* work )
{
	FEFTOOL_LSVBC* wk = work;
	
	if( wk->vblank_lst_flg ){
		// ����V�u�����N���Ԓ���DMA�ݒ���s��
		// �Q��ڂ͂����ōs���̂łP��ڂ������C�����[�v
		// �Ȃ��ł���V�u�����N���Ԓ��ɂ���Ă��炤
		wk->dma_set_flg = TRUE;

		// �ʏ��V�u�����N����
		wk->vblank1( wk->work );

	}else{
		LASTER_DmaStop();
	}
}
//----------------------------------------------------------------------------
/**
 *
 *	@brief	�r���ɓ���V�u�����N���ԗp�֐�
 *
 *	@param	tcb
 *	@param	work 
 *
 *	@return
 *
 *
 */
//-----------------------------------------------------------------------------
static void FEFTool_LSVBC_VBlank2( TCB_PTR tcb, void* work )
{
	FEFTOOL_LSVBC* wk = work;
	
	if( wk->vblank_lst_flg && wk->dma_set_flg ){

		// �r����V�u�����N����
		wk->vblank2( wk->work );
		
		wk->dma_set_flg = FALSE;
	}
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	���X�^�[V�u�����N�V�X�e��
 *
 *	@param	p_lsvbc		���X�^�[V�u�����N�V�X�e�����[�N
 *	@param	work		�����ɓn�����[�N
 *	@param	pfunc1		�ʏ��V�u�����N
 *	@param	pfunc2		�r����V�u�����N
 *
 *	@return
 *
 *
 */
//-----------------------------------------------------------------------------
static void FEFTool_LSVBC_Init( FEFTOOL_LSVBC* p_lsvbc, void* work, pFEFTool_LSVBCFunc pfunc1, pFEFTool_LSVBCFunc pfunc2 )
{
	GF_ASSERT( p_lsvbc );

	p_lsvbc->vblank_lst_flg	= TRUE;
	p_lsvbc->dma_set_flg	= FALSE;
	p_lsvbc->work			= work;
	p_lsvbc->vblank1		= pfunc1;
	p_lsvbc->vblank2		= pfunc2;

	// �^�X�N�o�^
	p_lsvbc->vbuff_tcb = VWaitTCB_Add( FEFTool_LSVBC_VBlank1, p_lsvbc, 0 );
	p_lsvbc->vdma_tcb = VIntrTCB_Add( FEFTool_LSVBC_VBlank2, p_lsvbc, 0 );
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	���X�^�[�pV�u�����N�Ǘ��V�X�e���j��
 *
 *	@param	p_lsvbc		�j�����郉�X�^�[V�u�����N�Ǘ��V�X�e��
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
static void FEFTool_LSVBC_Delete( FEFTOOL_LSVBC* p_lsvbc )
{
	GF_ASSERT( p_lsvbc );

	// VBlank�֐��j��
	if( p_lsvbc->vbuff_tcb != NULL ){
		TCB_Delete( p_lsvbc->vbuff_tcb );
	}
	if( p_lsvbc->vdma_tcb != NULL ){
		TCB_Delete( p_lsvbc->vdma_tcb );
	}
	LASTER_DmaStop();
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	V�u�����N�V�X�e���̂P����~
 *
 *	@param	p_lsvbc		���[�N
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
static void FEFTool_LSVBC_Stop( FEFTOOL_LSVBC* p_lsvbc )
{
	GF_ASSERT( p_lsvbc );
	p_lsvbc->vblank_lst_flg = FALSE;
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	V�u�����N�V�X�e���̍ĊJ
 *
 *	@param	p_lsvbc		���[�N
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
static void FEFTool_LSVBC_Start( FEFTOOL_LSVBC* p_lsvbc )
{
	GF_ASSERT( p_lsvbc );
	p_lsvbc->vblank_lst_flg = TRUE;
}


//-------------------------------------
//	
//	�o�b�t�@�����������삵�Ȃ�
//	
//=====================================
typedef struct _FEFTOOL_DEFLASTER{
	FEFTOOL_LSVBC vblank;

	// ���X�^�[�f�[�^
	LASTER_SYS_PTR lst;	// ���X�^�[�V�X�e��
	u32	buff1[ FEFTOOL_DEFLASTER_BUFF_SIZE ];	// �_�u���o�b�t�@
	u32	buff2[ FEFTOOL_DEFLASTER_BUFF_SIZE ];	// �_�u���o�b�t�@
	u32	addr;	// DMA�]����
	
} FEFTOOL_DEFLASTER;

//----------------------------------------------------------------------------
/**
 *
 *	@brief	DMA�]���ݒ�
 *
 *	@param	p_lst	�f�t�H���g���X�^�[�V�X�e��
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
static void FEFTool_DefLasterDmaTransSet( FEFTOOL_DEFLASTER* wk )
{
	const void* buff;

	// �Q�Ɨp�o�b�t�@�擾
	buff = LASTER_GetReadBuff( wk->lst );

	LASTER_DmaStop();
	
	// HBlankDmaTrans�ɐݒ�
	LASTER_DmaSet( buff, (void*)wk->addr, 4, LASTER_DMA_TYPE32 );
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	���X�^�[�pV�u�����N�Ǘ��V�X�e���ɓn���ʏ�V�u�����N�֐�
 *
 *	@param	work	���[�N
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
static void FEFTool_DefLasterNormVblank( void* work )
{
	FEFTOOL_DEFLASTER* wk = work;

	// �o�b�t�@�X�C�b�`
	LASTER_VBlank( wk->lst );

	// HDMA�o�^
	FEFTool_DefLasterDmaTransSet( wk );
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief ���X�^�[�pV�u�����N�Ǘ��V�X�e���ɓn���r���ɌĂ΂��V�u�����N�p����
 *
 *	@param		work	���[�N
 *	
 *	@return		none
 *
 *
 */
//-----------------------------------------------------------------------------
static void FEFTool_DefLasterCenterVblank( void* work )
{
	FEFTOOL_DEFLASTER* wk = work;

	// HDMA�o�^
	FEFTool_DefLasterDmaTransSet( wk );
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	�f�t�H���g���X�^�[����������
 *
 *	@param	addr		�]���惌�W�X�^�A�h���X
 *	@param	init_num	�o�b�t�@�̏����l
 *	@param	heap		�q�[�vID
 *
 *	@return	�쐬�������X�^�[�����f�[�^
 *
 *
 */
//-----------------------------------------------------------------------------
FEFTOOL_DEFLASTER_PTR FEFTool_InitDefLaster( u32 addr, u32 init_num, int heap )
{
	FEFTOOL_DEFLASTER_PTR p_lst;
	
	p_lst = sys_AllocMemory( heap, sizeof(FEFTOOL_DEFLASTER) );
	memset( p_lst, 0, sizeof(FEFTOOL_DEFLASTER) );
	GF_ASSERT( p_lst );


	p_lst->lst			= LASTER_Init( heap, p_lst->buff1, p_lst->buff2 );
	GF_ASSERT( p_lst->lst );
	p_lst->addr			= addr;

	// �o�b�t�@�̏�����
	MI_CpuFill32( p_lst->buff1, init_num, sizeof(u32) * FEFTOOL_DEFLASTER_BUFF_SIZE );
	MI_CpuFill32( p_lst->buff2, init_num, sizeof(u32) * FEFTOOL_DEFLASTER_BUFF_SIZE );

	// V�u�����N�Ǘ��V�X�e����������
	FEFTool_LSVBC_Init( &p_lst->vblank, p_lst, FEFTool_DefLasterNormVblank, FEFTool_DefLasterCenterVblank );

	return p_lst;
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	���X�^�[�����̏I��
 *
 *	@param	p_lst	�f�t�H���g���X�^�[�V�X�e�����[�N
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
void FEFTool_DeleteDefLaster( FEFTOOL_DEFLASTER_PTR p_lst )
{
	GF_ASSERT( p_lst );

	// Vblank�Ǘ��V�X�e���j��
	FEFTool_LSVBC_Delete( &p_lst->vblank );
	
	// ���X�^�[�V�X�e���j��
	if( p_lst->lst != NULL ){
		LASTER_Delete( p_lst->lst );
	}

	sys_FreeMemoryEz( p_lst );
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	�������ݗp�o�b�t�@�̎擾
 *
 *	@param	cp_lst	�f�t�H���g���X�^�[�V�X�e��
 *
 *	@return	�������ݗp�o�b�t�@
 *
 *
 */
//-----------------------------------------------------------------------------
void* FEFTool_GetDefLasterWriteBuff( CONST_FEFTOOL_DEFLASTER_PTR cp_lst )
{
	GF_ASSERT( cp_lst );

	return LASTER_GetWriteBuff( cp_lst->lst );
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	�Q�Ɨp�o�b�t�@���擾
 *
 *	@param	cp_lst	�f�t�H���g���X�^�[�V�X�e��
 *
 *	@return	�Q�Ɨp�o�b�t�@
 *
 *
 */
//-----------------------------------------------------------------------------
const void* FEFTool_GetDefLasterReadBuff( CONST_FEFTOOL_DEFLASTER_PTR cp_lst )
{
	GF_ASSERT( cp_lst );

	return LASTER_GetReadBuff( cp_lst->lst );
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	���X�^�[�����̒�~
 *
 *	@param	p_lst	�f�t�H���g���X�^�[���[�N
 *
 *	@return	none
 *
 * V�u�����N�֐����@�\���Ȃ��Ȃ�̂ŁA
 * ��ʂɃ��X�^�[�����f����Ȃ��Ȃ�܂��B
 *
 */
//-----------------------------------------------------------------------------
void FEFTool_StopDefLaster( FEFTOOL_DEFLASTER_PTR p_lst )
{
	GF_ASSERT( p_lst );

	FEFTool_LSVBC_Stop( &p_lst->vblank );
}

//----------------------------------------------------------------------------
/**
 *
 *	@brief	���X�^�[�����X�^�[�g
 *
 *	@param	p_lst	�f�t�H���g���X�^�[�V�X�e��
 *
 *	@return	none
 *
 * ��ʂɃ��X�^�[�����f�����悤�ɂȂ�܂��B
 *
 */
//-----------------------------------------------------------------------------
void FEFTool_StartDefLaster( FEFTOOL_DEFLASTER* p_lst )
{
	GF_ASSERT( p_lst );

	FEFTool_LSVBC_Start( &p_lst->vblank );
}


//----------------------------------------------------------------------------
/**
 *
 *	@brief	�f�[�^�]����A�h���X���Đݒ�
 *
 *	@param	p_lst	�f�t�H���g���X�^�[�V�X�e��
 *	@param	addr	�]����A�h���X
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
void FEFTool_SetDefLasterTransAddr( FEFTOOL_DEFLASTER_PTR p_lst, u32 addr )
{
	GF_ASSERT( p_lst );

	p_lst->addr = addr;
}


//--------------------------------------------------------------
/**
 * @brief	�f�[�^�]����A�h���X���擾
 *
 * @param	p_lst	�f�t�H���g���X�^�[�V�X�e��
 *
 * @retval	u32		�]����A�h���X
 *
 */
//--------------------------------------------------------------
u32 FEFTool_GetDefLasterTransAddr( FEFTOOL_DEFLASTER_PTR p_lst )
{
	GF_ASSERT( p_lst );

	return p_lst->addr;
}

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
//	
//	���[�V�����u���[�I�u�W�F�N�g
//	
//=====================================
typedef struct _DEMO_MOTION_BL_TASK{
	GXVRamLCDC		lcdc;		// ����LCDC
	DEMO_MOTION_BL_DATA	data;
	BOOL			init_flg;
	TCB_PTR			tcb;	
} DEMO_MOTION_BL_TASK;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void VBlankLCDCChange(TCB_PTR tcb, void* work);	// LCDC�̏�Ԃ�ݒ肷��^�X�N
static void DEMO_MOTION_BL_Task(TCB_PTR tcb, void* work);		// ���[�V�����u���[�^�X�N
static void DEMO_MOTION_BL_Capture( DEMO_MOTION_BL_DATA* p_data );



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
DEMO_MOTION_BL_PTR DEMO_MOTION_BL_Init(DEMO_MOTION_BL_DATA* init)
{
	TCB_PTR task;
	DEMO_MOTION_BL_PTR mb;
	
	// ���[�V�����u���[�^�X�N���Z�b�g
	task = PMDS_taskAdd(DEMO_MOTION_BL_Task, sizeof(DEMO_MOTION_BL_TASK), 5, init->heap_id);
	mb = TCB_GetWork(task);

	mb->data = *init;
	mb->tcb = task;
	mb->init_flg = FALSE;

	mb->lcdc = GX_GetBankForLCDC();

	DEMO_MOTION_BL_Capture( &mb->data );

	// LCDC�`�F���W
	VWaitTCB_Add( VBlankLCDCChange, mb, 0 );
//	addVBlankData(mb, VBlankLCDCChange);

	return mb;
}



//----------------------------------------------------------------------------
/**
 *
 *@brief	���[�V�����u���[�̉���
 *
 *@param	data		���[�V�����u���[�I�u�W�F
 *@param	dispMode	������̃f�B�X�v���C���[�h
 *@param	bgMode		�������BG���[�h
 *@param	bg0_2d3d	�������BG�O���RD�Ɏg�p���邩
 *
 *@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
void DEMO_MOTION_BL_Delete(DEMO_MOTION_BL_PTR* data, GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0_2d3d)
{
	

	// �O���t�B�b�N���[�h��߂�
	GX_SetGraphicsMode(dispMode, bgMode,bg0_2d3d);

	GX_SetBankForLCDC((*data)->lcdc);
	
	switch((*data)->data.dispMode){
	case GX_DISPMODE_VRAM_A:
		MI_CpuClearFast( (void*)HW_LCDC_VRAM_A, HW_VRAM_A_SIZE );
		break;
	case GX_DISPMODE_VRAM_B:
		MI_CpuClearFast( (void*)HW_LCDC_VRAM_B, HW_VRAM_B_SIZE );
		break;
	case GX_DISPMODE_VRAM_C:
		MI_CpuClearFast( (void*)HW_LCDC_VRAM_C, HW_VRAM_C_SIZE );
		break;
	case GX_DISPMODE_VRAM_D:
		MI_CpuClearFast( (void*)HW_LCDC_VRAM_D, HW_VRAM_D_SIZE );
		break;
	default:
		GX_SetBankForLCDC(GX_VRAM_LCDC_NONE);
		break;
	}
		
	
	// �^�X�N�j��
	PMDS_taskDel((*data)->tcb);
	*data = NULL;
}

//----------------------------------------------------------------------------
/**
 *
 *@brief	�r���Ńp�����[�^��ύX
 *
 *@param	data	���[�V�����u���[�I�u�W�F
 *@param	eva		�u�����h�W��A
 *@param	evb		�u�����h�W��B
 *
 *@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
void DEMO_MOTION_BL_ParamChg(DEMO_MOTION_BL_PTR data, int eva, int evb)
{
	data->data.eva = eva;
	data->data.evb = evb;
}


//----------------------------------------------------------------------------
/**
 *
 *@brief	���[�V�����u���[�@�L���v�`���^�X�N
 *
 *@param	tcb		�^�X�N�|�C���^
 *@param	work	���[�V�����u���[�f�[�^
 *
 *@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
static void DEMO_MOTION_BL_Task(TCB_PTR tcb, void* work)
{
	DEMO_MOTION_BL_PTR mb = work;
	
	//�����������҂�
	if( mb->init_flg ){
		GX_SetCapture(
				mb->data.sz,			// �L���v�`���T�C�Y
				mb->data.mode,			// �L���v�`�����[�h
				mb->data.a,				// �L���v�`���u�����hA
				mb->data.b,				// �L���v�`���u�����hB
				mb->data.dest,			// �]��Vram
				mb->data.eva,			// �u�����h�W��A
				mb->data.evb);			// �u�����h�W��B
				
//				OS_Printf( " %d, %d\n", mb->data.eva, mb->data.evb );
	}
}

//----------------------------------------------------------------------------
/**
 *
 *@brief	LCDC�̏�Ԃ�ݒ肷��^�X�N
 *
 *@param	work	�ݒ肷��l�������Ă���
 *
 *@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
static void VBlankLCDCChange(TCB_PTR tcb, void* work)
{
	DEMO_MOTION_BL_PTR mb = (DEMO_MOTION_BL_PTR)work;

	// �`��Vram�ݒ�
	switch(mb->data.dispMode){
	case GX_DISPMODE_VRAM_A:
		GX_SetBankForLCDC(GX_VRAM_LCDC_A);
		break;
	case GX_DISPMODE_VRAM_B:
		GX_SetBankForLCDC(GX_VRAM_LCDC_B);
		break;
	case GX_DISPMODE_VRAM_C:
		GX_SetBankForLCDC(GX_VRAM_LCDC_C);
		break;
	case GX_DISPMODE_VRAM_D:
		GX_SetBankForLCDC(GX_VRAM_LCDC_D);
		break;
	default:
		GX_SetBankForLCDC(GX_VRAM_LCDC_NONE);
		break;
	}

	// �O���t�B�b�N���[�h�ݒ�
	GX_SetGraphicsMode(mb->data.dispMode, mb->data.bgMode,mb->data.bg0_2d3d);	

	// ����������
	mb->init_flg = TRUE;

	TCB_Delete( tcb );
}

//----------------------------------------------------------------------------
/**
 *	@brief	Capture�֐�
 *
 *	@param	p_data 
 *
 *	@return
 */
//-----------------------------------------------------------------------------
static void DEMO_MOTION_BL_Capture( DEMO_MOTION_BL_DATA* p_data )
{
	// �`��Vram������
	switch(p_data->dispMode){
	case GX_DISPMODE_VRAM_A:
		MI_CpuClearFast( (void*)HW_LCDC_VRAM_A, HW_VRAM_A_SIZE );
		break;
	case GX_DISPMODE_VRAM_B:
		MI_CpuClearFast( (void*)HW_LCDC_VRAM_B, HW_VRAM_B_SIZE );
		break;
	case GX_DISPMODE_VRAM_C:
		MI_CpuClearFast( (void*)HW_LCDC_VRAM_C, HW_VRAM_C_SIZE );
		break;
	case GX_DISPMODE_VRAM_D:
		MI_CpuClearFast( (void*)HW_LCDC_VRAM_D, HW_VRAM_D_SIZE );
		break;
	default:
		break;
	}

	GX_SetCapture(
			p_data->sz,			// �L���v�`���T�C�Y
			p_data->mode,			// �L���v�`�����[�h
			p_data->a,				// �L���v�`���u�����hA
			p_data->b,				// �L���v�`���u�����hB
			p_data->dest,			// �]��Vram
			16,						// �u�����h�W��A
			0);						// �u�����h�W��B	
}

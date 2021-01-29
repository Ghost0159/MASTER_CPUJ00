//==============================================================================================
/**
 * @file	roulette_obj.c
 * @brief	�u�o�g�����[���b�g�vOBJ(�J�[�\���A�A�C�R��)
 * @author	Satoshi Nohara
 * @date	2007.09.05
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "poketool/pokeicon.h"

#include "roulette_sys.h"
#include "roulette_clact.h"
#include "roulette_obj.h"


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
//OBJ(�J�[�\���A�A�C�R��)
struct _ROULETTE_OBJ{
	u32	pause;						//�|�[�Y�t���O
	u32 work;						//�ėp���[�N

	u16 counter;					//�J�E���^�[
	u8	dummy;
	u8	disp;

	u16 init_x;
	u16 init_y;
	CLACT_WORK_PTR p_clact;			//�Z���A�N�^�[���[�N�|�C���^
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
ROULETTE_OBJ* RouletteObj_Create( ROULETTE_CLACT* roulette_clact, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, u16 x, u16 y, u32 pri, int bg_pri, u8 disp );
static ROULETTE_OBJ* RouletteObj_CreateSub( ROULETTE_CLACT* roulette_clact, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, u32 pri, int bg_pri, u8 disp );
void* RouletteObj_Delete( ROULETTE_OBJ* wk );
void RouletteObj_Vanish( ROULETTE_OBJ* wk, int flag );
void RouletteObj_Pause( ROULETTE_OBJ* wk, int flag );
u16 RouletteObj_GetObjX( ROULETTE_OBJ* wk );
u16 RouletteObj_GetObjY( ROULETTE_OBJ* wk );
void RouletteObj_SetObjPos( ROULETTE_OBJ* wk, u16 x, u16 y );
void RouletteObj_AnmChg( ROULETTE_OBJ* wk, u32 num );
void RouletteObj_IconPalChg( ROULETTE_OBJ* wk, POKEMON_PARAM* pp );
u16 RouletteObj_GetAnmFrame( ROULETTE_OBJ* wk );
void RouletteObj_SetInitPosOffset( ROULETTE_OBJ* wk, int offset_x, int offset_y );
void RouletteObj_SetScaleAffine( ROULETTE_OBJ* wk, u32 no );
void RouletteObj_SetAnmFlag( ROULETTE_OBJ* wk, u8 flag );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ�쐬
 *
 * @param	roulette_clact	ROULETTE_CLACT�^�̃|�C���^
 * @param	data_index		�f�[�^index
 * @param	heapID			�q�[�vID
 *
 * @retval	"ROULETTE_OBJ���[�N�ւ̃|�C���^"
 */
//--------------------------------------------------------------
ROULETTE_OBJ* RouletteObj_Create( ROULETTE_CLACT* roulette_clact, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, u16 x, u16 y, u32 pri, int bg_pri, u8 disp )
{
	ROULETTE_OBJ* wk;
	VecFx32	vec;

	wk = sys_AllocMemory( HEAPID_ROULETTE, sizeof(ROULETTE_OBJ) );		//�������m��
	memset( wk, 0, sizeof(ROULETTE_OBJ) );

	//�A�j���e�[�u���̎w�肪���鎞
	wk->p_clact = RouletteClact_SetActor(	roulette_clact, char_no, pltt_no, cell_no, anm_no, 
											pri, bg_pri, disp );

	wk->init_x		= x;
	wk->init_y		= y;
	wk->disp		= disp;

	//�����J�[�\�����W���Z�b�g
	vec.x = (x * FX32_ONE);
	vec.y = (y * FX32_ONE);

	if( disp == DISP_SUB ){
		vec.y += SUB_SURFACE_Y;				//���W��␳
	}

	CLACT_SetMatrix( wk->p_clact, &vec );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ���[�N�폜
 *
 * @param	wk		ROULETTE_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void* RouletteObj_Delete( ROULETTE_OBJ* wk )
{
	CLACT_Delete( wk->p_clact );

	//ROULETTE_OBJ�̃����o�Ń������m�ۂ������̂���������J������

	sys_FreeMemoryEz( wk );
	return NULL;
}

//--------------------------------------------------------------
/**
 * @brief	�o�j�b�V������
 *
 * @param	wk		ROULETTE_OBJ�^�̃|�C���^
 * @param	flag	0:��`�� 1:�����_���`��	
 *
 * @return	none
 */
//--------------------------------------------------------------
void RouletteObj_Vanish( ROULETTE_OBJ* wk, int flag )
{
	CLACT_SetDrawFlag( wk->p_clact, flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�[�Y
 *
 * @param	wk		ROULETTE_OBJ�^�̃|�C���^
 * @param	flag	0:�|�[�Y���� 1:�|�[�Y
 *
 * @return	none
 */
//--------------------------------------------------------------
void RouletteObj_Pause( ROULETTE_OBJ* wk, int flag )
{
	wk->pause = flag;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��X�ʒu���擾
 *
 * @param	wk		ROULETTE_OBJ�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
u16 RouletteObj_GetObjX( ROULETTE_OBJ* wk )
{
	VecFx32	vec;
	vec = *( CLACT_GetMatrix(wk->p_clact) );
	return (vec.x / FX32_ONE);
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��Y�ʒu���擾
 *
 * @param	wk		ROULETTE_OBJ�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
u16 RouletteObj_GetObjY( ROULETTE_OBJ* wk )
{
	VecFx32	vec;
	vec = *( CLACT_GetMatrix(wk->p_clact) );
	return (vec.y / FX32_ONE);
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ʒu���Z�b�g
 *
 * @param	wk		ROULETTE_OBJ�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void RouletteObj_SetObjPos( ROULETTE_OBJ* wk, u16 x, u16 y )
{
	VecFx32	vec;

	//���W���擾
	vec = *( CLACT_GetMatrix(wk->p_clact) );
	vec.x = (x * FX32_ONE);
	vec.y = (y * FX32_ONE);

	if( wk->disp == DISP_SUB ){
		vec.y += SUB_SURFACE_Y;				//���W��␳
	}

	//���W��ݒ�
	CLACT_SetMatrix( wk->p_clact, &vec );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���؂�ւ�
 *
 * @param	wk		ROULETTE_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void RouletteObj_AnmChg( ROULETTE_OBJ* wk, u32 num )
{
	CLACT_SetAnmFrame( wk->p_clact, FX32_ONE );
	//CLACT_AnmFrameSet( wk->p_clact, 0 );		//�A�j���[�V�����t���[�����Z�b�g
	CLACT_AnmChgCheck( wk->p_clact, num );		//�A�j���[�V�����̃V�[�P���X���`�F���W����(Check)
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�R���A�j��
 *
 * @param	wk		ROULETTE_OBJ���[�N�̃|�C���^
 * @param	pp		POKEMON_PARAM���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void RouletteObj_IconPalChg( ROULETTE_OBJ* wk, POKEMON_PARAM* pp )
{
	//�p���b�g�؂�ւ�
	//CLACT_PaletteOffsetChg�֐��̌��ʂɃp���b�g�̓]����擪�p���b�g�i���o�[�����Z
	CLACT_PaletteOffsetChgAddTransPlttNo( wk->p_clact, PokeIconPalNumGetByPP(pp) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���t���[���擾
 *
 * @param	wk		ROULETTE_OBJ���[�N�̃|�C���^
 *
 * @retval	"�A�j���t���[��"
 */
//--------------------------------------------------------------
u16 RouletteObj_GetAnmFrame( ROULETTE_OBJ* wk )
{
	return CLACT_AnmFrameGet( wk->p_clact );
}

//--------------------------------------------------------------
/**
 * @brief	�������W�Ɏw�肵���I�t�Z�b�g�𑫂��č��W�Z�b�g
 *
 * @param	wk		ROULETTE_OBJ���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void RouletteObj_SetInitPosOffset( ROULETTE_OBJ* wk, int offset_x, int offset_y )
{
	RouletteObj_SetObjPos( wk, wk->init_x+offset_x, wk->init_y+offset_y );
	return;
}

//--------------------------------------------------------------
///	�g�嗦
//--------------------------------------------------------------
static const VecFx32 scale_tbl[] =
{
	{ FX32_ONE,				FX32_ONE, FX32_ONE },				//���{
	
	//{ FX32_ONE*2,			FX32_ONE*2,				FX32_ONE*2	},
	{ FX32_ONE+(FX32_ONE/2),FX32_ONE+(FX32_ONE/2),	FX32_ONE+(FX32_ONE/2) },
	{ FX32_ONE+(FX32_ONE/4),FX32_ONE+(FX32_ONE/4),	FX32_ONE+(FX32_ONE/4) },
	{ FX32_ONE+(FX32_ONE/5),FX32_ONE+(FX32_ONE/5),	FX32_ONE+(FX32_ONE/5) },
	{ FX32_ONE+(FX32_ONE/7),FX32_ONE+(FX32_ONE/7),	FX32_ONE+(FX32_ONE/7) },

	{ FX32_ONE-(FX32_ONE/2),FX32_ONE-(FX32_ONE/2),	FX32_ONE-(FX32_ONE/2) },
	{ FX32_ONE-(FX32_ONE/4),FX32_ONE-(FX32_ONE/4),	FX32_ONE-(FX32_ONE/4) },
	{ FX32_ONE-(FX32_ONE/5),FX32_ONE-(FX32_ONE/5),	FX32_ONE-(FX32_ONE/5) },
	{ FX32_ONE-(FX32_ONE/7),FX32_ONE-(FX32_ONE/7),	FX32_ONE-(FX32_ONE/7) },
};

//--------------------------------------------------------------
/**
 * @brief	�g��k��
 *
 * @param	wk		ROULETTE_OBJ���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void RouletteObj_SetScaleAffine( ROULETTE_OBJ* wk, u32 no )
{
	u8 tbl_no;
	tbl_no = no;

#if 0
	if( sys.cont & PAD_BUTTON_L ){
		//�g��
		if( sys.cont & PAD_KEY_UP ){
			tbl_no = 1;
		}else if( sys.cont & PAD_KEY_LEFT ){
			tbl_no = 2;
		}else if( sys.cont & PAD_KEY_DOWN ){
			tbl_no = 3;
		}else if( sys.cont & PAD_KEY_RIGHT ){
			tbl_no = 4;
		}else{
			tbl_no = 0;
		}
	}else{
		//�k��
		if( sys.cont & PAD_KEY_UP ){
			tbl_no = 5;
		}else if( sys.cont & PAD_KEY_LEFT ){
			tbl_no = 6;
		}else if( sys.cont & PAD_KEY_DOWN ){
			tbl_no = 7;
		}else if( sys.cont & PAD_KEY_RIGHT ){
			tbl_no = 8;
		}else{
			tbl_no = 0;
		}
	}
#endif

	//CLACT_SetScaleAffine( wk->p_clact, &scale_tbl[tbl_no], CLACT_AFFINE_NORMAL );
	CLACT_SetScaleAffine( wk->p_clact, &scale_tbl[tbl_no], CLACT_AFFINE_DOUBLE );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�I�[�g�A�j���t���O�ݒ�
 *
 * @param	wk		ROULETTE_OBJ���[�N�̃|�C���^
 * @param	flag	0=��A�j���A1=�A�j��
 *
 * @retval	none
 */
//--------------------------------------------------------------
void RouletteObj_SetAnmFlag( ROULETTE_OBJ* wk, u8 flag )
{
	CLACT_SetAnmFlag( wk->p_clact, flag );
	return;
}



//==============================================================================================
/**
 * @file	scratch_obj.c
 * @brief	�u�X�N���b�`�vOBJ(�J�[�\���A�A�C�R��)
 * @author	Satoshi Nohara
 * @date	2007.12.11
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "poketool/pokeicon.h"

#include "scratch_sys.h"
#include "scratch_clact.h"
#include "scratch_obj.h"


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
//OBJ(�J�[�\���A�A�C�R��)
struct _SCRATCH_OBJ{
	u16	pause;						//�|�[�Y�t���O
	u8	disp;						//DISP_MAIN,DISP_SUB
	u8	dummy;
	u32 work;						//�ėp���[�N
	u32 counter;					//�J�E���^�[
	CLACT_WORK_PTR p_clact;			//�Z���A�N�^�[���[�N�|�C���^
};

//�J�[�\���̃p���b�g
enum{
	PAL_OBJ_MOVE = 0,
	PAL_OBJ_STOP,
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
SCRATCH_OBJ* ScratchObj_Create( SCRATCH_CLACT* scratch_clact, u32 data_no, u32 anm_no, u16 x, u16 y, u8 disp, u32 bg_pri, u32 pri );
void* ScratchObj_Delete( SCRATCH_OBJ* wk );
void ScratchObj_Vanish( SCRATCH_OBJ* wk, int flag );
void ScratchObj_Priority( SCRATCH_OBJ* wk, u32 Priority );
void ScratchObj_Pause( SCRATCH_OBJ* wk, int flag );
void ScratchObj_SetObjPos( SCRATCH_OBJ* wk, int x, int y );
void ScratchObj_GetObjPos( SCRATCH_OBJ* wk, int* x, int* y );
void ScratchObj_AnmChg( SCRATCH_OBJ* wk, u32 num );
void ScratchObj_IconPalChg( SCRATCH_OBJ* wk, POKEMON_PARAM* pp );
void ScratchObj_SetScaleAffine( SCRATCH_OBJ* wk, u32 no );
void ScratchObj_SetScaleAffine2( SCRATCH_OBJ* wk, u8 no );
void ScratchObj_SetScaleAffineTbl( SCRATCH_OBJ* wk, VecFx32* tbl );
void ScratchObj_PaletteNoChg( SCRATCH_OBJ* wk, u32 pltt_num );
u32 ScratchObj_PaletteNoGet( SCRATCH_OBJ* wk );
void ScratchObj_SetAnmFlag( SCRATCH_OBJ* wk, int flag );
void ScratchObj_MosaicSet( SCRATCH_OBJ* wk, BOOL flag );
BOOL ScratchObj_AnmActiveCheck( SCRATCH_OBJ* wk );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ�쐬
 *
 * @param	scratch_clact	SCRATCH_CLACT�^�̃|�C���^
 * @param	data_index		�f�[�^index
 * @param	heapID			�q�[�vID
 *
 * @retval	"SCRATCH_OBJ���[�N�ւ̃|�C���^"
 */
//--------------------------------------------------------------
SCRATCH_OBJ* ScratchObj_Create( SCRATCH_CLACT* scratch_clact, u32 data_no, u32 anm_no, u16 x, u16 y, u8 disp, u32 bg_pri, u32 pri )
{
	SCRATCH_OBJ* wk;
	VecFx32	vec;

	wk = sys_AllocMemory( HEAPID_SCRATCH, sizeof(SCRATCH_OBJ) );		//�������m��
	memset( wk, 0, sizeof(SCRATCH_OBJ) );

	wk->p_clact = ScratchClact_SetActor( scratch_clact, data_no, anm_no, bg_pri, pri, disp );
	wk->disp	= disp;

#if 0
	//�����J�[�\�����W���Z�b�g
	vec.x = (x * FX32_ONE);
	vec.y = (y * FX32_ONE);
	CLACT_SetMatrix( wk->p_clact, &vec );
#endif
	OS_Printf( "x = %d\n", x );
	OS_Printf( "y = %d\n", y );
	ScratchObj_SetObjPos( wk, x, y );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ���[�N�폜
 *
 * @param	wk		SCRATCH_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void* ScratchObj_Delete( SCRATCH_OBJ* wk )
{
	CLACT_Delete( wk->p_clact );

	//SCRATCH_OBJ�̃����o�Ń������m�ۂ������̂���������J������

	sys_FreeMemoryEz( wk );
	return NULL;
}

//--------------------------------------------------------------
/**
 * @brief	�o�j�b�V������
 *
 * @param	wk		SCRATCH_OBJ�^�̃|�C���^
 * @param	flag	0:��`�� 1:�����_���`��	
 *
 * @return	none
 */
//--------------------------------------------------------------
void ScratchObj_Vanish( SCRATCH_OBJ* wk, int flag )
{
	CLACT_SetDrawFlag( wk->p_clact, flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C�I���e�B����
 *
 * @param	wk			SCRATCH_OBJ�^�̃|�C���^
 * @param	Priority	�v���C�I���e�B
 *
 * @return	none
 */
//--------------------------------------------------------------
void ScratchObj_Priority( SCRATCH_OBJ* wk, u32 Priority )
{
	CLACT_DrawPriorityChg( wk->p_clact, Priority );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�[�Y
 *
 * @param	wk		SCRATCH_OBJ�^�̃|�C���^
 * @param	flag	0:�|�[�Y���� 1:�|�[�Y
 *
 * @return	none
 */
//--------------------------------------------------------------
void ScratchObj_Pause( SCRATCH_OBJ* wk, int flag )
{
	wk->pause = flag;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ʒu���Z�b�g
 *
 * @param	wk		SCRATCH_OBJ�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void ScratchObj_SetObjPos( SCRATCH_OBJ* wk, int x, int y )
{
	VecFx32	vec;

	vec.x = (x * FX32_ONE);
	vec.y = (y * FX32_ONE);

#if 1
	if( wk->disp == DISP_SUB ){
		//vec.y += SUB_SURFACE_Y;				//�T�u���
		vec.y += SCRATCH_SUB_ACTOR_DISTANCE;	//�T�u���
		//OS_Printf( "�T�u��ʂł��I\n" );
	}
#endif

	//���W��ݒ�
	CLACT_SetMatrix( wk->p_clact, &vec );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ʒu���擾
 *
 * @param	wk		SCRATCH_OBJ�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void ScratchObj_GetObjPos( SCRATCH_OBJ* wk, int* x, int* y )
{
	const VecFx32* vec;

	vec = CLACT_GetMatrix( wk->p_clact );

	*x = (vec->x / FX32_ONE);
	*y = (vec->y / FX32_ONE);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���؂�ւ�
 *
 * @param	wk		SCRATCH_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void ScratchObj_AnmChg( SCRATCH_OBJ* wk, u32 num )
{
	CLACT_SetAnmFrame( wk->p_clact, FX32_ONE );
	//CLACT_AnmFrameSet( wk->p_clact, 0 );	//�A�j���[�V�����t���[�����Z�b�g
	CLACT_AnmChgCheck( wk->p_clact, num );		//�A�j���[�V�����̃V�[�P���X���`�F���W����
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�R���A�j��
 *
 * @param	wk		SCRATCH_OBJ���[�N�̃|�C���^
 * @param	pp		POKEMON_PARAM���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void ScratchObj_IconPalChg( SCRATCH_OBJ* wk, POKEMON_PARAM* pp )
{
	//�p���b�g�؂�ւ�
	//CLACT_PaletteOffsetChg�֐��̌��ʂɃp���b�g�̓]����擪�p���b�g�i���o�[�����Z
	CLACT_PaletteOffsetChgAddTransPlttNo( wk->p_clact, PokeIconPalNumGetByPP(pp) );
	return;
}

//--------------------------------------------------------------
///	�g�嗦
//--------------------------------------------------------------
static const VecFx32 scale_tbl[] =
{
	{ FX32_ONE,				FX32_ONE,				FX32_ONE },				//���{
	
	{ FX32_ONE+(FX32_ONE/5),FX32_ONE+(FX32_ONE/5),	FX32_ONE+(FX32_ONE/5) },
	{ FX32_ONE+(FX32_ONE/4),FX32_ONE+(FX32_ONE/4),	FX32_ONE+(FX32_ONE/4) },
	{ FX32_ONE+(FX32_ONE/3),FX32_ONE+(FX32_ONE/3),	FX32_ONE+(FX32_ONE/3) },
	{ FX32_ONE+(FX32_ONE/2),FX32_ONE+(FX32_ONE/2),	FX32_ONE+(FX32_ONE/2) },


	{ FX32_ONE+(FX32_ONE)-(FX32_ONE/5),	FX32_ONE+(FX32_ONE)-(FX32_ONE/5),FX32_ONE+(FX32_ONE)-(FX32_ONE/5) },
	//{ FX32_ONE+(FX32_ONE),	FX32_ONE+(FX32_ONE),	FX32_ONE+(FX32_ONE) },



	//{ FX32_ONE*2,			FX32_ONE*2,				FX32_ONE*2	},
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
 * @param	wk		SCRATCH_OBJ���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void ScratchObj_SetScaleAffine( SCRATCH_OBJ* wk, u32 no )
{
	u8 tbl_no;
	tbl_no = no;

	//CLACT_SetScaleAffine( wk->p_clact, &scale_tbl[tbl_no], CLACT_AFFINE_NORMAL );
	//CLACT_SetScaleAffine( wk->p_clact, &scale_tbl[tbl_no], CLACT_AFFINE_DOUBLE );	//�{�p

	CLACT_SetScaleAffine( wk->p_clact, &scale_tbl[0], CLACT_AFFINE_NORMAL );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�g��k��
 *
 * @param	wk		SCRATCH_OBJ���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void ScratchObj_SetScaleAffine2( SCRATCH_OBJ* wk, u8 no )
{
	VecFx32 scale_tbl;

	scale_tbl.x = FX32_ONE + (FX32_ONE / no);
	scale_tbl.y = FX32_ONE + (FX32_ONE / no);
	scale_tbl.z = FX32_ONE + (FX32_ONE / no);

	//CLACT_SetScaleAffine( wk->p_clact, tbl, CLACT_AFFINE_DOUBLE );	//�{�p
	CLACT_SetScaleAffine( wk->p_clact, (const VecFx32*)&scale_tbl, CLACT_AFFINE_DOUBLE );	//�{�p
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�g��k��
 *
 * @param	wk		SCRATCH_OBJ���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void ScratchObj_SetScaleAffineTbl( SCRATCH_OBJ* wk, VecFx32* tbl )
{
	CLACT_SetScaleAffine( wk->p_clact, tbl, CLACT_AFFINE_DOUBLE );	//�{�p
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���b�gNo
 *
 * @param	wk			SCRATCH_OBJ���[�N�̃|�C���^
 * @param	pltt_num	�p���b�gNo
 *
 * @retval	none
 */
//--------------------------------------------------------------
void ScratchObj_PaletteNoChg( SCRATCH_OBJ* wk, u32 pltt_num )
{
	//CLACT_PaletteNoChg( wk->p_clact, pltt_num );
	
	CLACT_PaletteNoChgAddTransPlttNo( wk->p_clact, pltt_num );
	
	//CLACT_PaletteOffsetChg( wk->p_clact, pltt_num );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���b�gNo�擾
 *
 * @param	wk			SCRATCH_OBJ���[�N�̃|�C���^
 * @param	pltt_num	�p���b�gNo
 *
 * @retval	"�p���b�gNo"
 */
//--------------------------------------------------------------
u32 ScratchObj_PaletteNoGet( SCRATCH_OBJ* wk )
{
	return CLACT_PaletteNoGet( wk->p_clact );
}

//--------------------------------------------------------------
/**
 * @brief	�I�[�g�A�j���t���O��ON,OFF
 *
 * @param	wk			SCRATCH_OBJ���[�N�̃|�C���^
 * @param	flag		0=��A�j���A1=�I�[�g�A�j��
 *
 * @retval	none
 */
//--------------------------------------------------------------
void ScratchObj_SetAnmFlag( SCRATCH_OBJ* wk, int flag )
{
	CLACT_SetAnmFlag( wk->p_clact, flag );
	CLACT_SetAnmFrame( wk->p_clact, FX32_ONE );
	CLACT_AnmChg( wk->p_clact, CLACT_AnmGet(wk->p_clact) );	//�A�j���[�V�����̃V�[�P���X���`�F���W
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���U�C�N�ݒ�
 *
 * @param	wk			SCRATCH_OBJ���[�N�̃|�C���^
 * @param	flag		TRUE=ON�AFALSE=OFF
 *
 * @retval	none
 */
//--------------------------------------------------------------
void ScratchObj_MosaicSet( SCRATCH_OBJ* wk, BOOL flag )
{
	//��0-15(0�͒ʏ�\��)
	//���U�C�N�̕���ݒ�
	//G2_SetOBJMosaicSize( task_w->mosaic.x, task_w->mosaic.x );

	CLACT_MosaicSet( wk->p_clact, flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���[�V���������`�F�b�N
 *
 * @param	wk		SCRATCH_OBJ�^�̃|�C���^
 *
 * @retval	TRUE	�A�j���[�V������
 * @retval	FALSE	�X�g�b�v
 */
//--------------------------------------------------------------
BOOL ScratchObj_AnmActiveCheck( SCRATCH_OBJ* wk )
{
	return CLACT_AnmActiveCheck( wk->p_clact );
}



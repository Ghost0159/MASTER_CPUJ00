//==============================================================================================
/**
 * @file	stage_obj.c
 * @brief	�u�o�g���X�e�[�W�vOBJ(�J�[�\���A�A�C�R��)
 * @author	Satoshi Nohara
 * @date	2007.03.20
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "poketool/pokeicon.h"

#include "stage_sys.h"
#include "stage_clact.h"
#include "stage_obj.h"


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
//OBJ(�J�[�\���A�A�C�R��)
struct _STAGE_OBJ{
	u32	pause;						//�|�[�Y�t���O
	u32 work;						//�ėp���[�N
	u32 counter;					//�J�E���^�[
	const u8* anm_tbl;				//�J�[�\���A�j���e�[�u���̃|�C���^
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
STAGE_OBJ* StageObj_Create( STAGE_CLACT* stage_clact, u32 data_no, u32 anm_no, u16 x, u16 y, const u8* anm_tbl );
void* StageObj_Delete( STAGE_OBJ* wk );
void StageObj_Vanish( STAGE_OBJ* wk, int flag );
void StageObj_Pause( STAGE_OBJ* wk, int flag );
void StageObj_SetObjPos( STAGE_OBJ* wk, u16 x, u16 y );
void StageObj_AnmChg( STAGE_OBJ* wk, u32 num );
void StageObj_IconPalChg( STAGE_OBJ* wk, POKEMON_PARAM* pp );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ�쐬
 *
 * @param	stage_clact	STAGE_CLACT�^�̃|�C���^
 * @param	data_index		�f�[�^index
 * @param	heapID			�q�[�vID
 *
 * @retval	"STAGE_OBJ���[�N�ւ̃|�C���^"
 */
//--------------------------------------------------------------
STAGE_OBJ* StageObj_Create( STAGE_CLACT* stage_clact, u32 data_no, u32 anm_no, u16 x, u16 y, const u8* anm_tbl )
{
	STAGE_OBJ* wk;
	VecFx32	vec;

	wk = sys_AllocMemory( HEAPID_STAGE, sizeof(STAGE_OBJ) );		//�������m��
	memset( wk, 0, sizeof(STAGE_OBJ) );

	wk->anm_tbl		= anm_tbl;				//�J�[�\���̃A�j���e�[�u���̃|�C���^

	//�A�j���e�[�u���̎w�肪���鎞
	if( anm_tbl != NULL ){
		wk->p_clact = StageClact_SetActor( stage_clact, data_no, anm_no, 0, DISP_MAIN );
	}else{
		wk->p_clact = StageClact_SetActor( stage_clact, data_no, anm_no, 0, DISP_MAIN );
	}

	//�����J�[�\�����W���Z�b�g
	vec.x = (x * FX32_ONE);
	vec.y = (y * FX32_ONE);
	CLACT_SetMatrix( wk->p_clact, &vec );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ���[�N�폜
 *
 * @param	wk		STAGE_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void* StageObj_Delete( STAGE_OBJ* wk )
{
	CLACT_Delete( wk->p_clact );

	//STAGE_OBJ�̃����o�Ń������m�ۂ������̂���������J������

	sys_FreeMemoryEz( wk );
	return NULL;
}

//--------------------------------------------------------------
/**
 * @brief	�o�j�b�V������
 *
 * @param	wk		STAGE_OBJ�^�̃|�C���^
 * @param	flag	0:��`�� 1:�����_���`��	
 *
 * @return	none
 */
//--------------------------------------------------------------
void StageObj_Vanish( STAGE_OBJ* wk, int flag )
{
	CLACT_SetDrawFlag( wk->p_clact, flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�[�Y
 *
 * @param	wk		STAGE_OBJ�^�̃|�C���^
 * @param	flag	0:�|�[�Y���� 1:�|�[�Y
 *
 * @return	none
 */
//--------------------------------------------------------------
void StageObj_Pause( STAGE_OBJ* wk, int flag )
{
	wk->pause = flag;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ʒu���Z�b�g
 *
 * @param	wk		STAGE_OBJ�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void StageObj_SetObjPos( STAGE_OBJ* wk, u16 x, u16 y )
{
	VecFx32	vec;

	//�A�j����؂�ւ���
	if( wk->anm_tbl != NULL ){
		CLACT_AnmChgCheck( wk->p_clact, wk->anm_tbl[0] );
	}

	//���W���擾
	vec = *( CLACT_GetMatrix(wk->p_clact) );
	vec.x = (x * FX32_ONE);
	vec.y = (y * FX32_ONE);

	//���W��ݒ�
	CLACT_SetMatrix( wk->p_clact, &vec );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���؂�ւ�
 *
 * @param	wk		STAGE_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void StageObj_AnmChg( STAGE_OBJ* wk, u32 num )
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
 * @param	wk		STAGE_OBJ���[�N�̃|�C���^
 * @param	pp		POKEMON_PARAM���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void StageObj_IconPalChg( STAGE_OBJ* wk, POKEMON_PARAM* pp )
{
	//�p���b�g�؂�ւ�
	//CLACT_PaletteOffsetChg�֐��̌��ʂɃp���b�g�̓]����擪�p���b�g�i���o�[�����Z
	CLACT_PaletteOffsetChgAddTransPlttNo( wk->p_clact, PokeIconPalNumGetByPP(pp) );
	return;
}



//==============================================================================================
/**
 * @file	factory_obj.c
 * @brief	�u�o�g���t�@�N�g���[�vOBJ(�E�B���h�E)
 * @author	Satoshi Nohara
 * @date	2007.10.10
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"

#include "factory_sys.h"
#include "factory_clact.h"
#include "factory_obj.h"


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
//OBJ
struct _FACTORY_OBJ{
	int	init_x;						//�����ʒu
	int	init_y;						//�����ʒu
	CLACT_WORK_PTR p_clact;			//�Z���A�N�^�[���[�N�|�C���^
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
FACTORY_OBJ*	FactoryObj_Create( FACTORY_CLACT* factory_clact, u32 anm_no, int x, int y, u32 heapID );
void*			FactoryObj_Delete( FACTORY_OBJ* wk );
void			FactoryObj_Vanish( FACTORY_OBJ* wk, int flag );
VecFx32			FactoryObj_Move( FACTORY_OBJ* wk, int x, int y );
void			FactoryObj_SetPos( FACTORY_OBJ* wk, int x, int y );
const VecFx32*	FactoryObj_GetPos( FACTORY_OBJ* wk );
BOOL			FactoryObj_Main( FACTORY_OBJ* wk );
int				FactoryObj_GetInitX( FACTORY_OBJ* wk );
int				FactoryObj_GetInitY( FACTORY_OBJ* wk );
BOOL			FactoryObj_AnmActiveCheck( FACTORY_OBJ* wk );
void			FactoryObj_AnmChg( FACTORY_OBJ* wk, u32 num );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	OBJ�쐬
 *
 * @param	factory_clact	FACTORY_CLACT�^�̃|�C���^
 * @param	data_index		�f�[�^index
 * @param	heapID			�q�[�vID
 *
 * @retval	"FACTORY_OBJ���[�N�ւ̃|�C���^"
 */
//--------------------------------------------------------------
FACTORY_OBJ* FactoryObj_Create( FACTORY_CLACT* factory_clact, u32 anm_no, int x, int y, u32 heapID )
{
	FACTORY_OBJ* wk;

	wk = sys_AllocMemory( heapID, sizeof(FACTORY_OBJ) );		//�������m��
	memset( wk, 0, sizeof(FACTORY_OBJ) );

	wk->init_x = x;						//�����ʒu
	wk->init_y = y;						//�����ʒu

	//����A�^�C�v�͌Œ�(data_no,anm_no,pri)
	wk->p_clact = FactoryClact_SetActor( factory_clact, 0, anm_no, 0, 1, DISP_MAIN );

	FactoryObj_SetPos( wk, x, y );		//���W�Z�b�g
	
	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	OBJ���[�N�폜
 *
 * @param	wk		FACTORY_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void* FactoryObj_Delete( FACTORY_OBJ* wk )
{
	CLACT_Delete( wk->p_clact );

	//FACTORY_OBJ�̃����o�Ń������m�ۂ������̂���������J������

	sys_FreeMemoryEz( wk );
	return NULL;
}

//--------------------------------------------------------------
/**
 * @brief	�o�j�b�V������
 *
 * @param	wk		FACTORY_OBJ�^�̃|�C���^
 * @param	flag	0:��`�� 1:�����_���`��	
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryObj_Vanish( FACTORY_OBJ* wk, int flag )
{
	CLACT_SetDrawFlag( wk->p_clact, flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�ړ�
 *
 * @param	wk		FACTORY_OBJ�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
VecFx32 FactoryObj_Move( FACTORY_OBJ* wk, int x, int y )
{
	VecFx32	vec;

	vec = *( CLACT_GetMatrix(wk->p_clact) );
	vec.x += (x * FX32_ONE);
	vec.y += (y * FX32_ONE);
	CLACT_SetMatrix( wk->p_clact, &vec );
	return *( CLACT_GetMatrix(wk->p_clact) );
}

//--------------------------------------------------------------
/**
 * @brief	���W�Z�b�g
 *
 * @param	wk		FACTORY_OBJ�^�̃|�C���^
 * @param	x		X���W
 * @param	y		Y���W
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryObj_SetPos( FACTORY_OBJ* wk, int x, int y )
{
	VecFx32	vec;

	vec.x = (x * FX32_ONE);
	vec.y = (y * FX32_ONE);

	//vec.y += SUB_SURFACE_Y;						//1=�T�u��ʂɕύX

	CLACT_SetMatrix( wk->p_clact, &vec );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���W�擾
 *
 * @param	wk		FACTORY_OBJ�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
const VecFx32* FactoryObj_GetPos( FACTORY_OBJ* wk )
{
	return CLACT_GetMatrix( wk->p_clact );
}

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	wk		FACTORY_OBJ�^�̃|�C���^
 *
 * @retval	"FALSE = �폜���Ă悢�ATRUE = ��������̏������p����"
 */
//--------------------------------------------------------------
BOOL FactoryObj_Main( FACTORY_OBJ* wk )
{
	//�G���[���
	//if( wk == NULL ){
	//	return;
	//}

	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief	���� X�ʒu�擾
 *
 * @param	wk		FACTORY_OBJ�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
int FactoryObj_GetInitX( FACTORY_OBJ* wk )
{
	return wk->init_x;
}

//--------------------------------------------------------------
/**
 * @brief	���� Y�ʒu�擾
 *
 * @param	wk		FACTORY_OBJ�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
int FactoryObj_GetInitY( FACTORY_OBJ* wk )
{
	return wk->init_y;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���[�V���������`�F�b�N
 *
 * @param	wk		FACTORY_OBJ�^�̃|�C���^
 *
 * @retval	TRUE	�A�j���[�V������
 * @retval	FALSE	�X�g�b�v
 */
//--------------------------------------------------------------
BOOL FactoryObj_AnmActiveCheck( FACTORY_OBJ* wk )
{
	return CLACT_AnmActiveCheck( wk->p_clact );
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���؂�ւ�
 *
 * @param	wk		FACTORY_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void FactoryObj_AnmChg( FACTORY_OBJ* wk, u32 num )
{
	CLACT_SetAnmFrame( wk->p_clact, FX32_ONE );
	//CLACT_AnmFrameSet( wk->p_clact, 0 );	//�A�j���[�V�����t���[�����Z�b�g
	CLACT_AnmChg( wk->p_clact, num );		//�A�j���[�V�����̃V�[�P���X���`�F���W����
	return;
}



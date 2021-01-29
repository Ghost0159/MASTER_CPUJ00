//==============================================================================================
/**
 * @file	factory_ball.c
 * @brief	�u�o�g���t�@�N�g���[�v�{�[��
 * @author	Satoshi Nohara
 * @date	2007.03.14
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"

#include "factory_sys.h"
#include "factory_clact.h"
#include "factory_ball.h"


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
//�{�[��OBJ
struct _FACTORY_BALL{
	u16	decide_flag;				//���肵���t���O
	u16	dmy;						//
	int	init_x;						//�����ʒu
	int	init_y;						//�����ʒu
	CLACT_WORK_PTR p_clact;			//�Z���A�N�^�[���[�N�|�C���^
};

#define FACTORY_BALL_OFFSET_X	(248)//(216)		//�����^���{�[���ŏ��̉E�։B���I�t�Z�b�g


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
FACTORY_BALL*	FactoryBall_Create( FACTORY_CLACT* factory_clact, int x, int y, u32 heapID );
void*			FactoryBall_Delete( FACTORY_BALL* wk );
void			FactoryBall_Vanish( FACTORY_BALL* wk, int flag );
VecFx32			FactoryBall_Move( FACTORY_BALL* wk, int x, int y );
void			FactoryBall_SetPos( FACTORY_BALL* wk, int x, int y );
const VecFx32*	FactoryBall_GetPos( FACTORY_BALL* wk );
void			FactoryBall_SetDecideFlag( FACTORY_BALL* wk, u8 hit );
u8				FactoryBall_GetDecideFlag( FACTORY_BALL* wk );
BOOL			FactoryBall_Main( FACTORY_BALL* wk );
void			FactoryBall_Decide( FACTORY_BALL* wk );
void			FactoryBall_Cancel( FACTORY_BALL* wk );
void			FactoryBall_Entrance( FACTORY_BALL* wk );
int				FactoryBall_GetInitX( FACTORY_BALL* wk );
int				FactoryBall_GetInitY( FACTORY_BALL* wk );
BOOL			FactoryBall_AnmActiveCheck( FACTORY_BALL* wk );
void			FactoryBall_AnmChg( FACTORY_BALL* wk, u32 num );
void			FactoryBall_PalChg( FACTORY_BALL* wk, u32 pal );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�{�[��OBJ�쐬
 *
 * @param	factory_clact	FACTORY_CLACT�^�̃|�C���^
 * @param	data_index		�f�[�^index
 * @param	heapID			�q�[�vID
 *
 * @retval	"FACTORY_BALL���[�N�ւ̃|�C���^"
 */
//--------------------------------------------------------------
FACTORY_BALL* FactoryBall_Create( FACTORY_CLACT* factory_clact, int x, int y, u32 heapID )
{
	FACTORY_BALL* wk;

	wk = sys_AllocMemory( heapID, sizeof(FACTORY_BALL) );		//�������m��
	memset( wk, 0, sizeof(FACTORY_BALL) );

	wk->decide_flag	= 0;				//���肵���t���O
	wk->init_x = x;						//�����ʒu
	wk->init_y = y;						//�����ʒu

	//����A�^�C�v�͌Œ�(data_no,anm_no,pri)
	wk->p_clact = FactoryClact_SetActor( factory_clact, 0, ANM_BALL_STOP, 0, 0, DISP_MAIN );

	FactoryBall_SetPos( wk, x, y );		//���W�Z�b�g
	
	//�p���b�g�i���o�[��ύX
	CLACT_PaletteNoChg( wk->p_clact, PAL_BALL_STOP );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	�{�[��OBJ���[�N�폜
 *
 * @param	wk		FACTORY_BALL���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void* FactoryBall_Delete( FACTORY_BALL* wk )
{
	CLACT_Delete( wk->p_clact );

	//FACTORY_BALL�̃����o�Ń������m�ۂ������̂���������J������

	sys_FreeMemoryEz( wk );
	return NULL;
}

//--------------------------------------------------------------
/**
 * @brief	�o�j�b�V������
 *
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 * @param	flag	0:��`�� 1:�����_���`��	
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryBall_Vanish( FACTORY_BALL* wk, int flag )
{
	CLACT_SetDrawFlag( wk->p_clact, flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�ړ�
 *
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
VecFx32 FactoryBall_Move( FACTORY_BALL* wk, int x, int y )
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
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 * @param	x		X���W
 * @param	y		Y���W
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryBall_SetPos( FACTORY_BALL* wk, int x, int y )
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
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
const VecFx32* FactoryBall_GetPos( FACTORY_BALL* wk )
{
	return CLACT_GetMatrix( wk->p_clact );
}

//--------------------------------------------------------------
/**
 * @brief	���肵���t���O�̏�Ԃ�ݒ�
 *
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 * @param	sw		""
 *
 * @retval	none
 */
//--------------------------------------------------------------
void FactoryBall_SetDecideFlag( FACTORY_BALL* wk, u8 flag )
{
	wk->decide_flag = flag;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���肵���t���O�̏�Ԃ��擾(�����^�����[�h�������g�p�s��)
 *
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 *
 * @retval	""
 */
//--------------------------------------------------------------
u8 FactoryBall_GetDecideFlag( FACTORY_BALL* wk )
{
	return wk->decide_flag;
}

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 *
 * @retval	"FALSE = �폜���Ă悢�ATRUE = ��������̏������p����"
 */
//--------------------------------------------------------------
BOOL FactoryBall_Main( FACTORY_BALL* wk )
{
	//�G���[���
	//if( wk == NULL ){
	//	return;
	//}

	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief	���肵����ԂɕύX
 *
 * @param	factory_clact	FACTORY_CLACT�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void FactoryBall_Decide( FACTORY_BALL* wk )
{
	FactoryBall_SetDecideFlag( wk, 1 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�L�����Z����ԂɕύX
 *
 * @param	factory_clact	FACTORY_CLACT�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void FactoryBall_Cancel( FACTORY_BALL* wk )
{
	FactoryBall_SetDecideFlag( wk, 0 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	����ʒu�ɃZ�b�g����
 *
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 *
 * @return	"TRUE = ���ꊮ���AFALSE = �܂�"
 */
//--------------------------------------------------------------
void FactoryBall_Entrance( FACTORY_BALL* wk )
{
	VecFx32	vec;
	const VecFx32* p_vec;

	p_vec = FactoryBall_GetPos( wk );		//���݂̍��W�擾

	vec.x = ((p_vec->x / FX32_ONE)+FACTORY_BALL_OFFSET_X) * FX32_ONE;
	vec.y = p_vec->y;

	CLACT_SetMatrix( wk->p_clact, &vec );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� X�ʒu�擾
 *
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
int FactoryBall_GetInitX( FACTORY_BALL* wk )
{
	return wk->init_x;
}

//--------------------------------------------------------------
/**
 * @brief	���� Y�ʒu�擾
 *
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
int FactoryBall_GetInitY( FACTORY_BALL* wk )
{
	return wk->init_y;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���[�V���������`�F�b�N
 *
 * @param	wk		FACTORY_BALL�^�̃|�C���^
 *
 * @retval	TRUE	�A�j���[�V������
 * @retval	FALSE	�X�g�b�v
 */
//--------------------------------------------------------------
BOOL FactoryBall_AnmActiveCheck( FACTORY_BALL* wk )
{
	return CLACT_AnmActiveCheck( wk->p_clact );
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���؂�ւ�
 *
 * @param	wk		FACTORY_BALL���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void FactoryBall_AnmChg( FACTORY_BALL* wk, u32 num )
{
	CLACT_SetAnmFrame( wk->p_clact, FX32_ONE );
	//CLACT_AnmFrameSet( wk->p_clact, 0 );	//�A�j���[�V�����t���[�����Z�b�g
	CLACT_AnmChg( wk->p_clact, num );		//�A�j���[�V�����̃V�[�P���X���`�F���W����
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���b�g�؂�ւ�
 *
 * @param	wk		FACTORY_BALL���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void FactoryBall_PalChg( FACTORY_BALL* wk, u32 pal )
{
	CLACT_PaletteNoChg( wk->p_clact, pal );
	return;
}



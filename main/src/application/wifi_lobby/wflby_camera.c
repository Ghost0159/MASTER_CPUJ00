//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_camera.c
 *	@brief		WiFi�L��J����	
 *	@author		tomoya takahashi
 *	@data		2007.11.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "wflby_camera.h"
#include "wflby_3dmatrix.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define WFLBY_CAMERA_DIST		( 0x29aec1 )			// ����
static const CAMERA_ANGLE sc_WFLBY_CAMERA_ANGLE = {		// �A���O��
	-0x29fe,0,0
};
#define WFLBY_CAMERA_TYPE		( GF_CAMERA_PERSPECTIV )// �J�����̃^�C�v
#define WFLBY_CAMERA_PARS		( 0x05c1 )				// �p�[�X
#define WFLBY_CAMERA_NEAR		( FX32_CONST( 150 ) )	// �N���b�s���O�@�j�A
#define WFLBY_CAMERA_FAR		( FX32_CONST( 900 ) )	// �N���b�s���O�@�t�@�[
#define WFLBY_CAMERA_X_OFS		( FX32_CONST( 8 ) )		// �J�����^�[�Q�b�g��X�I�t�Z�b�g
#define WFLBY_CAMERA_Z_OFS		( -FX32_CONST( 32 ) )		// �J�����^�[�Q�b�g��Z�I�t�Z�b�g

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�J�����V�X�e��
//=====================================
typedef struct _WFLBY_CAMERA {
	GF_CAMERA_PTR			p_camera;
	const WFLBY_3DPERSON*	cp_person;
	VecFx32					target;
}WFLBY_CAMERA;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------
/**
 *	@brief	�J����������
 *
 *	@param	heapID	�q�[�vID
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
WFLBY_CAMERA* WFLBY_CAMERA_Init( u32 heapID )
{
	WFLBY_CAMERA* p_wk;
	VecFx32 target;

	p_wk = sys_AllocMemory( heapID, sizeof(WFLBY_CAMERA) );
	memset( p_wk, 0, sizeof(WFLBY_CAMERA) );

	// �������m��
	p_wk->p_camera = GFC_AllocCamera( heapID );

	//�J�����Z�b�g
	GFC_InitCameraTDA( &p_wk->target,
				WFLBY_CAMERA_DIST,	// ����
				&sc_WFLBY_CAMERA_ANGLE,	//
				WFLBY_CAMERA_PARS,	// ���ˉe�̊p�x
				WFLBY_CAMERA_TYPE,
				TRUE,
				p_wk->p_camera );

	//�J�����̗L����
	GFC_AttachCamera( p_wk->p_camera );
	
	//�j�A�E�t�@�[�ݒ�
	GFC_SetCameraClip( WFLBY_CAMERA_NEAR, WFLBY_CAMERA_FAR, p_wk->p_camera );

	OS_TPrintf( "�����ˉe\n" );

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����j��
 *
 *	@param	p_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_CAMERA_Exit( WFLBY_CAMERA* p_sys )
{

	GFC_PurgeCamera();
		
	GFC_FreeCamera( p_sys->p_camera );

	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����`��
 *
 *	@param	p_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_CAMERA_Draw( WFLBY_CAMERA* p_sys )
{
	// �^�[�Q�b�g���W�v�Z
	if( p_sys->cp_person ){
		WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_sys->cp_person, &p_sys->target );
		p_sys->target.x += WFLBY_CAMERA_X_OFS;
		p_sys->target.z += WFLBY_CAMERA_Z_OFS;
	}
	
	// �J�������f
	GFC_CameraLookAt();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����^�[�Q�b�g�ݒ�
 *
 *	@param	p_sys	���[�N
 *	@param	posx	�����W
 *	@param	posy	�����W
 */
//-----------------------------------------------------------------------------
void WFLBY_CAMERA_SetTarget( WFLBY_CAMERA* p_sys, s32 posx, s32 posy )
{
	WF2DMAP_POS pos;
	pos.x = posx;
	pos.y = posy;
	WFLBY_3DMATRIX_GetPosVec( &pos, &p_sys->target );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�[�Q�b�g���W��ݒ�	�RD
 *	
 *	@param	p_sys	�V�X�e��
 *	@param	x		�����W
 *	@param	y		�����W
 *	@param	z		�����W
 */
//-----------------------------------------------------------------------------
void WFLBY_CAMERA_SetTarget3D( WFLBY_CAMERA* p_sys, fx32 x, fx32 y, fx32 z )
{
	p_sys->target.x = x;
	p_sys->target.y = y;
	p_sys->target.z = z;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[��p���W�ݒ�
 *
 *	@param	p_sys	�V�X�e��
 *	@param	x		�����W
 *	@param	y		�����W
 *	@param	z		�����W
 */
//-----------------------------------------------------------------------------
void WFLBY_CAMERA_SetTargetPerson3D( WFLBY_CAMERA* p_sys, fx32 x, fx32 y, fx32 z )
{
	p_sys->target.x = x + WFLBY_CAMERA_X_OFS;
	p_sys->target.y = y;
	p_sys->target.z = z + WFLBY_CAMERA_Z_OFS;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�[�Q�b�g�v���C���[��ݒ�
 *
 *	@param	p_sys		���[�N
 *	@param	cp_person	�l��
 */
//-----------------------------------------------------------------------------
void WFLBY_CAMERA_SetTargetPerson( WFLBY_CAMERA* p_sys, const WFLBY_3DPERSON* cp_person )
{
	p_sys->cp_person = cp_person;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�[�Q�b�g�l�����擾
 *
 *	@param	cp_sys	���[�N
 *
 *	@return	�^�[�Q�b�g�l��	NULL:���Ȃ�
 */
//-----------------------------------------------------------------------------
const WFLBY_3DPERSON* WFLBY_CAMERA_GetTargetPerson( const WFLBY_CAMERA* cp_sys )
{
	return cp_sys->cp_person;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�[�Q�b�g�l�������Z�b�g�i�Ȃ����j
 *
 *	@param	p_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_CAMERA_ResetTargetPerson( WFLBY_CAMERA* p_sys )
{
	p_sys->cp_person = NULL;
}

#ifdef WFLBY_CAMERA_DEBUG

#define WFLBY_CAMERA_DEBUG_DIST		( 0x61b89b )			// ����
static const CAMERA_ANGLE sc_WFLBY_CAMERA_DEBUG_ANGLE = {		// �A���O��
	-0x239e,0,0
};
#define WFLBY_CAMERA_DEBUG_TYPE		( GF_CAMERA_ORTHO )// �J�����̃^�C�v
#define WFLBY_CAMERA_DEBUG_PARS		( 0x0281 )				// �p�[�X
#define WFLBY_CAMERA_DEBUG_NEAR		( FX32_CONST( 150 ) )	// �N���b�s���O�@�j�A
#define WFLBY_CAMERA_DEBUG_FAR		( FX32_CONST( 1735 ) )	// �N���b�s���O�@�t�@�[

WFLBY_CAMERA* WFLBY_CAMERA_DEBUG_CameraInit( u32 heapID )
{
	WFLBY_CAMERA* p_wk;
	VecFx32 target;

	p_wk = sys_AllocMemory( heapID, sizeof(WFLBY_CAMERA) );
	memset( p_wk, 0, sizeof(WFLBY_CAMERA) );

	// �������m��
	p_wk->p_camera = GFC_AllocCamera( heapID );

	//�J�����Z�b�g
	GFC_InitCameraTDA( &p_wk->target,
				WFLBY_CAMERA_DEBUG_DIST,	// ����
				&sc_WFLBY_CAMERA_DEBUG_ANGLE,	//
				WFLBY_CAMERA_DEBUG_PARS,	// ���ˉe�̊p�x
				WFLBY_CAMERA_DEBUG_TYPE,
				TRUE,
				p_wk->p_camera );

	//�J�����̗L����
	GFC_AttachCamera( p_wk->p_camera );
	
	//�j�A�E�t�@�[�ݒ�
	GFC_SetCameraClip( WFLBY_CAMERA_DEBUG_NEAR, WFLBY_CAMERA_DEBUG_FAR, p_wk->p_camera );


	OS_TPrintf( "���ˉe\n" );

	return p_wk;
}
#endif

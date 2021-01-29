//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_3dmatrix.c
 *	@brief		�L�ꕔ��	�RD���W�Ǘ��֐�
 *	@author		tomoya takahashi
 *	@data		2007.11.13
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
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

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	wf2d�V�X�e���̃|�W�V�������W���RD���W�ɕϊ�����
 *
 *	@param	cp_pos		�|�W�V����
 *	@param	p_vec		�RD���W
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMATRIX_GetPosVec( const WF2DMAP_POS* cp_pos, VecFx32* p_vec )
{
	p_vec->x = cp_pos->x * WFLBY_3DMATRIX_GRID_SIZ;
	p_vec->z = cp_pos->y * WFLBY_3DMATRIX_GRID_SIZ;
	p_vec->y = WFLBY_3DMATRIX_FLOOR_MAT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD���W��wf2d�V�X�e���̃|�W�V�������W�ɕϊ�����
 *
 *	@param	cp_vec	�RD���W
 *	@param	p_pos	�|�W�V����
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMATRIX_GetVecPos( const VecFx32* cp_vec, WF2DMAP_POS* p_pos )
{
	p_pos->x = cp_vec->x / WFLBY_3DMATRIX_GRID_SIZ;
	p_pos->y = cp_vec->z / WFLBY_3DMATRIX_GRID_SIZ;
}





//----------------------------------------------------------------------------
/**
 *	@brief	�ėp�֐�	�e�N�X�`����W�J���Ď��f�[�^��j������
 *
 *	@param	pp_in		�ǂݍ��񂾃o�b�t�@�i�[��
 *	@param	p_handle	�n���h��
 *	@param	data_idx	�f�[�^IDX
 *	@param	gheapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJ_TEX_LoatCutTex( void** pp_in, ARCHANDLE* p_handle, u32 data_idx, u32 gheapID )
{
	u32 tex_cut_size;
	void* p_file;
	NNSGfdTexKey	texKey;		// �e�N�X�`���L�[
	NNSGfdTexKey	tex4x4Key;	// 4x4�e�N�X�`���L�[
	NNSGfdPlttKey	plttKey;	// �p���b�g�L�[
	NNSG3dResTex*	p_tex;

	// �܂����ʂɃ��f����ǂݍ���
	p_file = ArcUtil_HDL_Load( p_handle, data_idx, FALSE, gheapID, ALLOC_BOTTOM );

	// VRAM�ɓW�J
	{
		// �ꎞ�I�Ɋe�|�C���^���擾����
		p_tex		= NNS_G3dGetTex( p_file );
		// �]������
		LoadVRAMTexture( p_tex );
	}

	// �e�N�X�`���L�[�擾
	{
		NNS_G3dTexReleaseTexKey( p_tex, &texKey, &tex4x4Key );
		plttKey = NNS_G3dPlttReleasePlttKey( p_tex );	
	}

	
	// �e�N�X�`��������j������
	{
		tex_cut_size		= TEXRESM_TOOL_CutTexDataSizeGet( p_file );	// �e�N�X�`����j���������̃T�C�Y���擾
		*pp_in		= sys_AllocMemory( gheapID, tex_cut_size );	// �e�N�X�`����j�������T�C�Y�̃��������m��
		memcpy( *pp_in, p_file, tex_cut_size );	// ���������R�s�[
	}

	// �e�N�X�`���L�[��ݒ�
	{
		p_tex		= NNS_G3dGetTex( *pp_in );
		NNS_G3dTexSetTexKey(p_tex, texKey, tex4x4Key);
		NNS_G3dPlttSetPlttKey(p_tex, plttKey);
	}


	// �ꎞ�t�@�C����j��
	sys_FreeMemoryEz( p_file );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�J�����O�`�F�b�N
 *
 *	@param	cp_mdl		���f��
 *	@param	cp_obj		�`��I�u�W�F
 *
 *	@retval	TRUE	�`�悷��
 *	@retval	FALSE	�`�悵�Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DMAPOBJ_MDL_BOXCheck( const D3DOBJ_MDL* cp_mdl, const D3DOBJ* cp_obj )
{
	VecFx32 matrix;
	VecFx32 scale;
	MtxFx33 mtx;
	MtxFx33 calc_mtx;
	u32 rot_x;
	u32 rot_y;
	u32 rot_z;


	D3DOBJ_GetMatrix( cp_obj,  &matrix.x, &matrix.y, &matrix.z );
	D3DOBJ_GetScale( cp_obj,  &scale.x, &scale.y, &scale.z );
	rot_x = D3DOBJ_GetRota( cp_obj, D3DOBJ_ROTA_WAY_X );
	rot_y = D3DOBJ_GetRota( cp_obj, D3DOBJ_ROTA_WAY_Y );
	rot_z = D3DOBJ_GetRota( cp_obj, D3DOBJ_ROTA_WAY_Z );

	// ��]�s��쐬
	MTX_Identity33( &mtx );
	MTX_RotX33( &calc_mtx, FX_SinIdx( rot_x ), FX_CosIdx( rot_x ) );
	MTX_Concat33( &calc_mtx, &mtx, &mtx );
	MTX_RotZ33( &calc_mtx, FX_SinIdx( rot_z ), FX_CosIdx( rot_z ) );
	MTX_Concat33( &calc_mtx, &mtx, &mtx );
	MTX_RotY33( &calc_mtx, FX_SinIdx( rot_y ), FX_CosIdx( rot_y ) );
	MTX_Concat33( &calc_mtx, &mtx, &mtx );
	
	// �`��`�F�b�N
	return BB_CullingCheck3DModel( cp_mdl->pModel, 
				&matrix,
				&mtx,
				&scale );
}
		

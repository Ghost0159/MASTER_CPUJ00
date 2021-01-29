//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		d3dobj.c
 *	@brief		�t�B�[���h�RD�I�u�W�F�N�g�ȈՍ쐬
 *	@author		tomoya takahashi
 *	@data		2006.04.25
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"


#include "include/system/arc_util.h"

#define	__D3DOBJ_H_GLOBAL
#include "include/system/d3dobj.h"

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
// �V�X�e���S�̂̏������j���^�X�N
#define D3DOBJ_TEXTRANS_TCB_PRI	( 1024 )

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
static void D3DOBJ_MdlTransTcb( TCB_PTR tcb, void* p_work );
static void D3DOBJ_AnmDataLoad_Core( D3DOBJ_ANM* p_anm, const D3DOBJ_MDL* cp_mdl, void* p_data, NNSFndAllocator* pallocator );

static void D3DOBJ_MdlLoadComm( D3DOBJ_MDL* p_mdl );


//----------------------------------------------------------------------------
/**
 *	@brief	�RD���f���ǂݍ���
 *
 *	@param	p_mdl		���f�����[�N
 *	@param	arc_idx		�A�[�J�C�uID
 *	@param	data_idx	�f�[�^IDX
 *	@param	heap		�q�[�v
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_MdlLoad( D3DOBJ_MDL* p_mdl, u32 arc_idx, u32 data_idx, u32 heap )
{
	// ���f���ް��ǂݍ���
	p_mdl->pResMdl = ArcUtil_Load( arc_idx, data_idx, FALSE, heap, ALLOC_TOP );

	// ���f���f�[�^�ݒ聕�e�N�X�`���]��
	D3DOBJ_MdlLoadComm( p_mdl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD���f���ǂݍ���	�n���h���o�[�W����
 *
 *	@param	p_mdl		���f�����[�N
 *	@param	p_handle	�A�[�J�C�u�n���h��
 *	@param	data_idx	�f�[�^IDX
 *	@param	heap		�q�[�v
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_MdlLoadH( D3DOBJ_MDL* p_mdl, ARCHANDLE* p_handle, u32 data_idx, u32 heap )
{
	// ���f���ް��ǂݍ���
	p_mdl->pResMdl = ArcUtil_HDL_Load( p_handle, data_idx, FALSE, heap, ALLOC_TOP );

	// ���f���f�[�^�ݒ聕�e�N�X�`���]��
	D3DOBJ_MdlLoadComm( p_mdl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���f���]���^�X�N
 *
 *	@param	tcb		TCB���[�N
 *	@param	p_work	���[�N
 *	
 *	@return	none
 */
//-----------------------------------------------------------------------------
static void D3DOBJ_MdlTransTcb( TCB_PTR tcb, void* p_work )
{
	D3DOBJ_MDL* p_mdl = p_work;

	// ���f���ް���]��
	LoadVRAMTexture( p_mdl->pMdlTex );
	BindTexture( p_mdl->pResMdl, p_mdl->pMdlTex );

	TCB_Delete( tcb );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���f�����[�N�j��
 *
 *	@param	p_mdl ���f�����[�N
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_MdlDelete( D3DOBJ_MDL* p_mdl )
{
	NNSG3dTexKey texKey;
	NNSG3dTexKey tex4x4Key;
	NNSG3dPlttKey plttKey;

	
	if( p_mdl->pMdlTex ){
		//VramKey�j��
		NNS_G3dTexReleaseTexKey( p_mdl->pMdlTex, &texKey, &tex4x4Key );
		NNS_GfdFreeTexVram( texKey );	
		NNS_GfdFreeTexVram( tex4x4Key );	

		plttKey = NNS_G3dPlttReleasePlttKey( p_mdl->pMdlTex );
		NNS_GfdFreePlttVram( plttKey );
	}
	
	// �S���\�[�X�j��
	if(p_mdl->pResMdl){
		sys_FreeMemoryEz( p_mdl->pResMdl );
	}
	memset( p_mdl, 0, sizeof(D3DOBJ_MDL) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�A�j���f�[�^�ǂݍ���
 *
 *	@param	p_anm		�A�j�����[�N
 *	@param	cp_mdl		���f�����[�N
 *	@param	arc_idx		�A�[�J�C�u�t�@�C��ID
 *	@param	data_idx	�f�[�^IDX
 *	@param	heap		�q�[�v
 *	@param	pallocator	�A���P�[�^
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_AnmLoad( D3DOBJ_ANM* p_anm, const D3DOBJ_MDL* cp_mdl, u32 arc_idx, u32 data_idx, u32 heap, NNSFndAllocator* pallocator )
{
	void* p_data;
	// �A�j���ް��ǂݍ���
	p_data = ArcUtil_Load( arc_idx, data_idx, FALSE, heap, ALLOC_TOP );
	D3DOBJ_AnmDataLoad_Core( p_anm, cp_mdl, p_data, pallocator );
	p_anm->res_copy = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�A�j���f�[�^�ǂݍ���
 *
 *	@param	p_anm		�A�j�����[�N
 *	@param	cp_mdl		���f�����[�N
 *	@param	arc_idx		�A�[�J�C�u�t�@�C��ID
 *	@param	data_idx	�f�[�^IDX
 *	@param	heap		�q�[�v
 *	@param	pallocator	�A���P�[�^
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_AnmLoadH( D3DOBJ_ANM* p_anm, const D3DOBJ_MDL* cp_mdl, ARCHANDLE* p_handle, u32 data_idx, u32 heap, NNSFndAllocator* pallocator )
{
	void* p_data;
	// �A�j���ް��ǂݍ���
	p_data = ArcUtil_HDL_Load( p_handle, data_idx, FALSE, heap, ALLOC_TOP );
	D3DOBJ_AnmDataLoad_Core( p_anm, cp_mdl, p_data, pallocator );
	p_anm->res_copy = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ǂݍ��ݍς݃f�[�^����
 *
 *	@param	p_anm		�A�j�����[�N
 *	@param	cp_mdl		���f�����[�N
 *	@param	p_data		�A�j���f�[�^�o�b�t�@
 *	@param	pallocator	�A���P�[�^
 */
//-----------------------------------------------------------------------------
void D3DOBJ_AnmLoad_Data( D3DOBJ_ANM* p_anm, const D3DOBJ_MDL* cp_mdl, void* p_data, NNSFndAllocator* pallocator )
{
	D3DOBJ_AnmDataLoad_Core( p_anm, cp_mdl, p_data, pallocator );
	p_anm->res_copy = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�A�j���f�[�^�j��
 *
 *	@param	p_anm		�A�j�����[�N
 *	@param	pallocator	�A���P�[�^
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_AnmDelete( D3DOBJ_ANM* p_anm, NNSFndAllocator* pallocator )
{
	if(p_anm->pResAnm){
		NNS_G3dFreeAnmObj( pallocator, p_anm->pAnmObj );
		if( p_anm->res_copy == FALSE ){
			sys_FreeMemoryEz( p_anm->pResAnm );
		}
	}
	memset( p_anm, 0, sizeof(D3DOBJ_ANM) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�v�Đ�
 *
 *	@param	p_anm	�A�j���I�u�W�F�N�g
 *	@param	add		�����l
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_AnmLoop( D3DOBJ_ANM* p_anm, fx32 add )
{
	fx32 end_frame = NNS_G3dAnmObjGetNumFrame( p_anm->pAnmObj );
	
	if( add > 0 ){
		p_anm->frame = (p_anm->frame + add) % end_frame;
	}else{
		p_anm->frame = p_anm->frame + add;
		if( p_anm->frame < 0 ){
			p_anm->frame += end_frame;
		}
	}

	NNS_G3dAnmObjSetFrame( p_anm->pAnmObj, p_anm->frame );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�v�Ȃ��Đ�
 *
 *	@param	p_anm �A�j�����[�N
 *	@param	add		�����l
 *
 *	@retval	TRUE	�Đ��I��
 *	@retval	FALSE	�Đ��r��
 */
//-----------------------------------------------------------------------------
BOOL D3DOBJ_AnmNoLoop( D3DOBJ_ANM* p_anm, fx32 add )
{
	fx32 end_frame = NNS_G3dAnmObjGetNumFrame( p_anm->pAnmObj );
	BOOL ret = FALSE;

	if( add > 0 ){
		if( (p_anm->frame + add) < end_frame ){
			p_anm->frame += add;
		}else{
			p_anm->frame = end_frame;
			ret = TRUE;
		}
	}else{
		if( (p_anm->frame + add) >= 0 ){
			p_anm->frame += add;
		}else{
			p_anm->frame = 0;
			ret = TRUE;
		}
	}
	NNS_G3dAnmObjSetFrame( p_anm->pAnmObj, p_anm->frame );
	return ret;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���l�ݒ�
 *
 *	@param	p_anm	�A�j���I�u�W�F
 *	@param	num		�ݒ�l
 *	
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_AnmSet( D3DOBJ_ANM* p_anm, fx32 num )
{
	p_anm->frame = num;
	NNS_G3dAnmObjSetFrame( p_anm->pAnmObj, num );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���t���[���擾
 *
 *	@param	p_anm	�A�j���I�u�W�F
 *
 *	@return	�t���[����
 */
//-----------------------------------------------------------------------------
fx32 D3DOBJ_AnmGet( const D3DOBJ_ANM* cp_anm )
{
	return cp_anm->frame;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V�����t���[�������擾����
 *
 *	@param	cp_anm	�A�j���I�u�W�F
 *
 *	@return	�A�j���[�V�����̏I������t���[��
 */
//-----------------------------------------------------------------------------
fx32 D3DOBJ_AnmGetFrameNum( const D3DOBJ_ANM* cp_anm )
{
	return NNS_G3dAnmObjGetNumFrame( cp_anm->pAnmObj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g������
 *
 *	@param	p_draw	���[�N
 *	@param	p_mdl	���f�����[�N
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_Init( D3DOBJ* p_draw, D3DOBJ_MDL* p_mdl )
{
	memset( p_draw, 0, sizeof(D3DOBJ) );
	NNS_G3dRenderObjInit( &p_draw->render, p_mdl->pModel );
	p_draw->draw_flg	= TRUE;
	p_draw->scale.x		= FX32_ONE;
	p_draw->scale.y		= FX32_ONE;
	p_draw->scale.z		= FX32_ONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���f�[�^�������N
 *
 *	@param	p_draw	���[�N
 *	@param	p_anm	�A�j�����[�N
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_AddAnm( D3DOBJ* p_draw, D3DOBJ_ANM* p_anm )
{
	NNS_G3dRenderObjAddAnmObj( &p_draw->render, p_anm->pAnmObj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���f�[�^�̃����N���͂�
 *
 *	@param	p_draw	���[�N
 *	@param	p_anm	�A�j�����[�N
 */
//-----------------------------------------------------------------------------
void D3DOBJ_DelAnm( D3DOBJ* p_draw, D3DOBJ_ANM* p_anm )
{
	NNS_G3dRenderObjRemoveAnmObj( &p_draw->render, p_anm->pAnmObj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���f���`��		
 *
 *	@param	p_draw �`�惏�[�N
 *
 *	@return	none
 *
 */
//-----------------------------------------------------------------------------
void D3DOBJ_Draw( D3DOBJ* p_draw )
{
	MtxFx33 mtx;
	MtxFx33 calc_mtx;

	if( p_draw->draw_flg ){
		// ��]�s��쐬
		MTX_Identity33( &mtx );
		MTX_RotX33( &calc_mtx, FX_SinIdx( p_draw->rota[D3DOBJ_ROTA_WAY_X] ), FX_CosIdx( p_draw->rota[D3DOBJ_ROTA_WAY_X] ) );
		MTX_Concat33( &calc_mtx, &mtx, &mtx );
		MTX_RotZ33( &calc_mtx, FX_SinIdx( p_draw->rota[D3DOBJ_ROTA_WAY_Z] ), FX_CosIdx( p_draw->rota[D3DOBJ_ROTA_WAY_Z] ) );
		MTX_Concat33( &calc_mtx, &mtx, &mtx );
		MTX_RotY33( &calc_mtx, FX_SinIdx( p_draw->rota[D3DOBJ_ROTA_WAY_Y] ), FX_CosIdx( p_draw->rota[D3DOBJ_ROTA_WAY_Y] ) );
		MTX_Concat33( &calc_mtx, &mtx, &mtx );

		simple_3DModelDraw( &p_draw->render, &p_draw->matrix, &mtx, &p_draw->scale );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���f���`��		��]�͓Ǝ��ɍ쐬�������Ƃ�������̂�
 *
 *	@param	p_draw �`�惏�[�N
 *	@param	cp_mtx ��]�s��
 */
//-----------------------------------------------------------------------------
void D3DOBJ_DrawRMtx( D3DOBJ* p_draw, const MtxFx33* cp_mtx )
{
	if( p_draw->draw_flg ){
		simple_3DModelDraw( &p_draw->render, &p_draw->matrix, (MtxFx33*)cp_mtx, &p_draw->scale );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��t���O�ݒ�
 *
 *	@param	p_draw	���[�N
 *	@param	flag	�t���O
 *
 *	@return	none
 *
 *	flag
 *		TRUE	�\��
 *		FALSE	��\��
 */
//-----------------------------------------------------------------------------
void D3DOBJ_SetDraw( D3DOBJ* p_draw, BOOL flag )
{
	p_draw->draw_flg = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��t���O�擾
 *
 *	@param	cp_draw		���[�N
 *
 *	@retval	TRUE	�\��
 *	@retval	FALSE	��\��
 */
//-----------------------------------------------------------------------------
BOOL D3DOBJ_GetDraw( const D3DOBJ* cp_draw )
{
	return cp_draw->draw_flg;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�ݒ�
 *	
 *	@param	p_draw		���[�N
 *	@param	x			�����W
 *	@param	y			�����W
 *	@param	z			�����W
 *	
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_SetMatrix( D3DOBJ* p_draw, fx32 x, fx32 y, fx32 z )
{
	p_draw->matrix.x = x;
	p_draw->matrix.y = y;
	p_draw->matrix.z = z;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�擾
 *
 *	@param	p_draw	���[�N
 *	@param	p_x		�i�[��
 *	@param	p_y		�i�[��
 *	@param	p_z		�i�[��
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_GetMatrix( const D3DOBJ* cp_draw, fx32* p_x, fx32* p_y, fx32* p_z )
{
	*p_x = cp_draw->matrix.x;
	*p_y = cp_draw->matrix.y;
	*p_z = cp_draw->matrix.z;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�k�l�ݒ�
 *
 *	@param	p_draw	�`�惏�[�N
 *	@param	x		���g�k�l
 *	@param	y		���g�k�l
 *	@param	z 		���g�k�l
 *
 *	@return	noen
 */
//-----------------------------------------------------------------------------
void D3DOBJ_SetScale( D3DOBJ* p_draw, fx32 x, fx32 y, fx32 z )
{
	p_draw->scale.x = x;
	p_draw->scale.y = y;
	p_draw->scale.z = z;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�k�l���擾
 *
 *	@param	p_draw	�`�惏�[�N
 *	@param	p_x		�����W�擾��
 *	@param	p_y		�����W�擾��
 *	@param	p_z		�����W�擾��
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void D3DOBJ_GetScale( const D3DOBJ* cp_draw, fx32* p_x, fx32* p_y, fx32* p_z )
{
	*p_x = cp_draw->scale.x;
	*p_y = cp_draw->scale.y;
	*p_z = cp_draw->scale.z;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�e�ݒ�
 *
 *	@param	p_draw	�`�惏�[�N
 *	@param	rota	��]�p
 *	@param	way		����
 *
 *	@return	none
 *	way
 *		D3DOBJ_ROTA_WAY_X		X����]
 *		D3DOBJ_ROTA_WAY_Y		Y����]
 *		D3DOBJ_ROTA_WAY_Z		Z����]
 */
//-----------------------------------------------------------------------------
void D3DOBJ_SetRota( D3DOBJ* p_draw, u16 rota, u32 way )
{
	p_draw->rota[ way ] = rota;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�惏�[�N	��]�p�擾
 *	
 *	@param	p_draw	�`�惏�[�N
 *	@param	way		����
 *
 *	@return	��]�p
 *	way
 *		D3DOBJ_ROTA_WAY_X		X����]
 *		D3DOBJ_ROTA_WAY_Y		Y����]
 *		D3DOBJ_ROTA_WAY_Z		Z����]
 */
//-----------------------------------------------------------------------------
u16 D3DOBJ_GetRota( const D3DOBJ* cp_draw, u32 way )
{
	return cp_draw->rota[ way ];
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���ǂݍ���Core�֐�
 *
 *	@param	p_anm		�A�j�����[�N
 *	@param	cp_mdl		���f�����[�N
 *	@param	p_data		�A�j���f�[�^�o�b�t�@
 *	@param	pallocator	�A���P�[�^
 */
//-----------------------------------------------------------------------------
static void D3DOBJ_AnmDataLoad_Core( D3DOBJ_ANM* p_anm, const D3DOBJ_MDL* cp_mdl, void* p_data, NNSFndAllocator* pallocator )
{
	// �A�j���ް��ǂݍ���
	p_anm->pResAnm = p_data;

	//���\�[�X�擾
	p_anm->pOneResAnm = NNS_G3dGetAnmByIdx(p_anm->pResAnm,0);

	// �A�j���I�u�W�F�̃������m��
	p_anm->pAnmObj = NNS_G3dAllocAnmObj( 
			pallocator,
			p_anm->pOneResAnm,
			cp_mdl->pModel );

	// �A�j���I�u�W�F������
	NNS_G3dAnmObjInit(
		p_anm->pAnmObj,
		p_anm->pOneResAnm,
		cp_mdl->pModel,
		cp_mdl->pMdlTex );
}

//----------------------------------------------------------------------------
/**
 *	@brief	3D���f���ǂݍ��݋��ʕ���
 *
 *	@param	p_mdl	���f�����[�N
 */
//-----------------------------------------------------------------------------
static void D3DOBJ_MdlLoadComm( D3DOBJ_MDL* p_mdl )
{
	// ���\�[�X�ǂݍ��ݍς݂ł���K�v������
	GF_ASSERT( p_mdl->pResMdl );
	
	// ���f���ް��擾
	p_mdl->pModelSet	= NNS_G3dGetMdlSet( p_mdl->pResMdl );
	p_mdl->pModel		= NNS_G3dGetMdlByIdx( p_mdl->pModelSet, 0 );
	p_mdl->pMdlTex		= NNS_G3dGetTex( p_mdl->pResMdl );
	
	if( p_mdl->pMdlTex ){
		// �e�N�X�`���f�[�^�]��
		VWaitTCB_Add( D3DOBJ_MdlTransTcb, p_mdl, D3DOBJ_TEXTRANS_TCB_PRI );
	}
}

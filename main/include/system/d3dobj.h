//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		d3dobj.h
 *	@brief		�t�B�[���h�RD�I�u�W�F�N�g�ȈՍ쐬
 *	@author		tomoya takahashi
 *	@data		2006.04.25
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __D3DOBJ_H__
#define __D3DOBJ_H__

#include <nnsys.h>
#include "system/arc_util.h"

#undef GLOBAL
#ifdef	__D3DOBJ_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *	�y�g�p��̒��Ӂz
 *		�Ensb�`��2�ȏ�̂��̂��������ē���邱�Ƃ��o���܂��B
 *		�@���������̃V�X�e���ł́A
 *		�@���̒��̃C���f�b�N�X0�Ԗڂ̂��̂������K�p���܂��B
 *
 *		�E���k�ɔ�Ή��ł��B
 */
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
//	��]�����萔
//=====================================
enum{
	D3DOBJ_ROTA_WAY_X,
	D3DOBJ_ROTA_WAY_Y,
	D3DOBJ_ROTA_WAY_Z,
	D3DOBJ_ROTA_WAY_NUM,
};

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
//	�RD�I�u�W�F�N�g
//	���f���@�e�N�X�`��
//	��ނɂP�ł悢���̂ł��B
//=====================================
typedef struct {
	void*					pResMdl;		// ���f���ް�
	NNSG3dResMdlSet*		pModelSet;		// ���f���Z�b�g
	NNSG3dResMdl*			pModel;			// ���f�����\�[�X
	NNSG3dResTex*			pMdlTex;		// ���f���ɓ\��t����e�N�X�`��
} D3DOBJ_MDL;

//-------------------------------------
//	�RD�I�u�W�F�N�g
//	�A�j��
//	��ނɂP�ł悢���̂ł��B
//=====================================
typedef struct {
	void*					pResAnm;		// �A�j�����\�[�X
	void*					pOneResAnm;		// 1�؂��������\�[�X
	NNSG3dAnmObj*			pAnmObj;			// �A�j���[�V�����I�u�W�F
	fx32 frame;
	BOOL res_copy;
} D3DOBJ_ANM;

//-------------------------------------
//	�RD�`��I�u�W�F�N�g
//	�o�������I�u�W�F�N�g�̐����K�v�Ȃ��̂ł��B
//=====================================
typedef struct {
	NNSG3dRenderObj			render;		// �����_�[�I�u�W�F�N�g
	VecFx32 matrix;	// ���W
	VecFx32 scale;	// �g�k
	BOOL	draw_flg;	// �`��t���O
	u16		rota[ D3DOBJ_ROTA_WAY_NUM ];// ��]�pX
	u16		dummy;
} D3DOBJ;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
// ���f��
GLOBAL void D3DOBJ_MdlLoad( D3DOBJ_MDL* p_mdl, u32 arc_idx, u32 data_idx, u32 heap );
GLOBAL void D3DOBJ_MdlLoadH( D3DOBJ_MDL* p_mdl, ARCHANDLE* p_handle, u32 data_idx, u32 heap );
GLOBAL void D3DOBJ_MdlDelete( D3DOBJ_MDL* p_mdl );

// �A�j��
GLOBAL void D3DOBJ_AnmLoad( D3DOBJ_ANM* p_anm, const D3DOBJ_MDL* cp_mdl, u32 arc_idx, u32 data_idx, u32 heap, NNSFndAllocator* pallocator );
GLOBAL void D3DOBJ_AnmLoadH( D3DOBJ_ANM* p_anm, const D3DOBJ_MDL* cp_mdl, ARCHANDLE* p_handle, u32 data_idx, u32 heap, NNSFndAllocator* pallocator );
GLOBAL void D3DOBJ_AnmLoad_Data( D3DOBJ_ANM* p_anm, const D3DOBJ_MDL* cp_mdl, void* p_data, NNSFndAllocator* pallocator );
GLOBAL void D3DOBJ_AnmDelete( D3DOBJ_ANM* p_anm, NNSFndAllocator* pallocator );
GLOBAL BOOL D3DOBJ_AnmNoLoop( D3DOBJ_ANM* p_anm, fx32 add );
GLOBAL void D3DOBJ_AnmLoop( D3DOBJ_ANM* p_anm, fx32 add );
GLOBAL void D3DOBJ_AnmSet( D3DOBJ_ANM* p_anm, fx32 num );
GLOBAL fx32 D3DOBJ_AnmGet( const D3DOBJ_ANM* cp_anm );
GLOBAL fx32 D3DOBJ_AnmGetFrameNum( const D3DOBJ_ANM* cp_anm );

// �`��I�u�W�F
GLOBAL void D3DOBJ_Init( D3DOBJ* p_draw, D3DOBJ_MDL* cp_mdl );
GLOBAL void D3DOBJ_AddAnm( D3DOBJ* p_draw, D3DOBJ_ANM* p_anm );
GLOBAL void D3DOBJ_DelAnm( D3DOBJ* p_draw, D3DOBJ_ANM* p_anm );
GLOBAL void D3DOBJ_Draw( D3DOBJ* p_draw );
GLOBAL void D3DOBJ_DrawRMtx( D3DOBJ* p_draw, const MtxFx33* cp_mtx );
GLOBAL void D3DOBJ_SetDraw( D3DOBJ* p_draw, BOOL flag );
GLOBAL BOOL D3DOBJ_GetDraw( const D3DOBJ* cp_draw );
GLOBAL void D3DOBJ_SetMatrix( D3DOBJ* p_draw, fx32 x, fx32 y, fx32 z );
GLOBAL void D3DOBJ_GetMatrix( const D3DOBJ* cp_draw, fx32* p_x, fx32* p_y, fx32* p_z );
GLOBAL void D3DOBJ_SetScale( D3DOBJ* p_draw, fx32 x, fx32 y, fx32 z );
GLOBAL void D3DOBJ_GetScale( const D3DOBJ* cp_draw, fx32* p_x, fx32* p_y, fx32* p_z );
GLOBAL void D3DOBJ_SetRota( D3DOBJ* p_draw, u16 rota, u32 way );
GLOBAL u16 D3DOBJ_GetRota( const D3DOBJ* cp_draw, u32 way );



#undef	GLOBAL
#endif		// __D3DOBJ_H__


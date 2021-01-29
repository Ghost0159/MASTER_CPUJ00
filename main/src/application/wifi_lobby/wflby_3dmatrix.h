//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_3dmatrix.h
 *	@brief		�L�ꕔ��	�RD���W�Ǘ��֐�
 *	@author		tomoya takahashi
 *	@data		2007.11.13
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_3DMATRIX_H__
#define __WFLBY_3DMATRIX_H__

#include "system/d3dobj.h"
#include "system/arc_util.h"

#include "application/wifi_2dmap/wf2dmap_common.h"


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�O���b�h�RD�T�C�Y
//=====================================
#define WFLBY_3DMATRIX_GRID_SIZ		( FX32_ONE )
#define WFLBY_3DMATRIX_GRID_GRIDSIZ	( WF2DMAP_GRID_SIZ*FX32_ONE )
#define WFLBY_3DMATRIX_GRID_GRIDSIZ_HALF	( (WF2DMAP_GRID_SIZ/2)*FX32_ONE )

//-------------------------------------
///	���̈ʒu
//=====================================
#define WFLBY_3DMATRIX_FLOOR_MAT	( FX32_CONST(0) )

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
extern void WFLBY_3DMATRIX_GetPosVec( const WF2DMAP_POS* cp_pos, VecFx32* p_vec );
extern void WFLBY_3DMATRIX_GetVecPos( const VecFx32* cp_vec, WF2DMAP_POS* p_pos );


// �ėp�֐�
// �e�N�X�`���ǂݍ���	�e�N�X�`���]����j���o�[�W����
extern  void WFLBY_3DMAPOBJ_TEX_LoatCutTex( void** pp_in, ARCHANDLE* p_handle, u32 data_idx, u32 gheapID );
extern BOOL WFLBY_3DMAPOBJ_MDL_BOXCheck( const D3DOBJ_MDL* cp_mdl, const D3DOBJ* cp_obj );




#endif		// __WFLBY_3DMATRIX_H__


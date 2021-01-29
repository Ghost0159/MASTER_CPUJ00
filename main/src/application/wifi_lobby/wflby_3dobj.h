//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_3dobj.h
 *	@brief		�RD�r���{�[�h�A�N�^�[�I�u�W�F�Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.11.01
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_3DOBJ_H__
#define __WFLBY_3DOBJ_H__

#include "application/wifi_2dmap/wf2dmap_obj.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�A�j���萔
//=====================================
typedef enum {
	WFLBY_3DOBJ_ANM_ROTA,
	WFLBY_3DOBJ_ANM_JUMP,
	WFLBY_3DOBJ_ANM_BATABATA,
	WFLBY_3DOBJ_ANM_NUM,
} WFLBY_3DOBJ_ANMTYPE;

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�V�X�e���\����
//=====================================
typedef struct _WFLBY_3DOBJSYS WFLBY_3DOBJSYS;

//-------------------------------------
///	���[�N�\����
//=====================================
typedef struct _WFLBY_3DOBJWK WFLBY_3DOBJWK;



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
// �V�X�e���Ǘ�
extern WFLBY_3DOBJSYS* WFLBY_3DOBJSYS_Init( u32 objnum, u32 hero_sex, u32 heapID, u32 gheapID );
extern void WFLBY_3DOBJSYS_Exit( WFLBY_3DOBJSYS* p_sys );
extern void WFLBY_3DOBJSYS_Updata( WFLBY_3DOBJSYS* p_sys );
extern void WFLBY_3DOBJSYS_Draw( WFLBY_3DOBJSYS* p_sys );
extern void WFLBY_3DOBJSYS_VBlank( WFLBY_3DOBJSYS* p_sys );

// �o����g���[�i�^�C�v�Ȃ̂��`�F�b�N
extern BOOL WFLBY_3DOBJSYS_CheckTrType( u32 trtype );

// �e����
extern void WFLBY_3DOBJSYS_SetShadowAlpha( WFLBY_3DOBJSYS* p_sys, u32 alpha );
extern u32 WFLBY_3DOBJSYS_GetShadowAlpha( const WFLBY_3DOBJSYS* cp_sys );

// ���[�N�Ǘ�
extern WFLBY_3DOBJWK* WFLBY_3DOBJWK_New( WFLBY_3DOBJSYS* p_sys, const WF2DMAP_OBJWK* cp_objwk );
extern void WFLBY_3DOBJWK_Del( WFLBY_3DOBJWK* p_wk );

// �X�V�t���O
extern void WFLBY_3DOBJWK_SetUpdata( WFLBY_3DOBJWK* p_wk, BOOL updata );
extern BOOL WFLBY_3DOBJWK_GetUpdata( const WFLBY_3DOBJWK* cp_wk );

// ���C�g
extern void WFLBY_3DOBJWK_SetLight( WFLBY_3DOBJWK* p_wk, u32 light_msk );

// �J�����O�t���O�擾
extern BOOL WFLBY_3DOBJWK_GetCullingFlag( const WFLBY_3DOBJWK* cp_wk );

// �X�V�t���O��OFF�ɂ�����̓���
// ���W
extern void WFLBY_3DOBJWK_SetMatrix( WFLBY_3DOBJWK* p_wk, const WF2DMAP_POS* cp_pos );
extern void WFLBY_3DOBJWK_GetMatrix( const WFLBY_3DOBJWK* cp_wk, WF2DMAP_POS* p_pos );
extern void WFLBY_3DOBJWK_Set3DMatrix( WFLBY_3DOBJWK* p_wk, const VecFx32* cp_vec );
extern void WFLBY_3DOBJWK_Get3DMatrix( const WFLBY_3DOBJWK* cp_wk, VecFx32* p_vec );

// ����
extern void WFLBY_3DOBJWK_SetWay( WFLBY_3DOBJWK* p_wk, WF2DMAP_WAY way );

// �A�j��
extern void WFLBY_3DOBJWK_StartAnm( WFLBY_3DOBJWK* p_wk, WFLBY_3DOBJ_ANMTYPE anm );
extern void WFLBY_3DOBJWK_EndAnm( WFLBY_3DOBJWK* p_wk );
extern void WFLBY_3DOBJWK_SetAnmSpeed( WFLBY_3DOBJWK* p_wk, u8 speed );

// �\���E��\��
extern void WFLBY_3DOBJWK_SetDrawFlag( WFLBY_3DOBJWK* p_wk, BOOL flag );
extern BOOL WFLBY_3DOBJWK_GetDrawFlag( const WFLBY_3DOBJWK* cp_wk );


#endif		// __WFLBY_3DOBJ_H__


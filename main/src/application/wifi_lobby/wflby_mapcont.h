//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_mapcont.h
 *	@brief		�}�b�v�Ǘ��V�X�e��
 *	@author		tomoya	takahashi
 *	@data		2007.11.13
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_MAPCONT_H__
#define __WFLBY_MAPCONT_H__

#include "application/wifi_2dmap/wf2dmap_map.h"
#include "map_conv/wflby_mapdata.h"

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
//-------------------------------------
///	�}�b�v�V�X�e��������
//=====================================
typedef struct _WFLBY_MAPCONT WFLBY_MAPCONT;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
// �V�X�e���쐬
extern WFLBY_MAPCONT* WFLBY_MAPCONT_Init( u32 heapID );
extern void WFLBY_MAPCONT_Exit( WFLBY_MAPCONT* p_sys );

// �}�b�v�̑傫���擾
extern u16 WFLBY_MAPCONT_GridXGet( const WFLBY_MAPCONT* cp_sys );
extern u16 WFLBY_MAPCONT_GridYGet( const WFLBY_MAPCONT* cp_sys );

// �}�b�v�f�[�^�擾
extern WF2DMAP_MAP WFLBY_MAPCONT_DataGet( const WFLBY_MAPCONT* cp_sys, u16 xgrid, u16 ygrid );
extern BOOL WFLBY_MAPCONT_HitGet( const WFLBY_MAPCONT* cp_sys, u16 xgrid, u16 ygrid );
extern u32 WFLBY_MAPCONT_ObjIDGet( const WFLBY_MAPCONT* cp_sys, u16 xgrid, u16 ygrid );
extern u32 WFLBY_MAPCONT_EventGet( const WFLBY_MAPCONT* cp_sys, u16 xgrid, u16 ygrid );

// �}�b�v�f�[�^����
extern BOOL WFLBY_MAPCONT_SarchObjID( const WFLBY_MAPCONT* cp_sys, u32 objid, u16* p_gridx, u16* p_gridy, u32 no );
extern u32 WFLBY_MAPCONT_CountObjID( const WFLBY_MAPCONT* cp_sys, u32 objid );

// �C�x���g�N���^�C�v�`�F�b�N
extern BOOL WFLBY_MAPCONT_Event_CheckMount( u32 event );
extern BOOL WFLBY_MAPCONT_Event_CheckFrontKey( u32 event );
extern BOOL WFLBY_MAPCONT_Event_CheckFrontDecide( u32 event );
extern BOOL WFLBY_MAPCONT_Event_CheckFrontDouzou( u32 event );



//  �o���邾������Ȃ��ق��������֐�
extern const WF2DMAP_MAPSYS* WFLBY_MAPCONT_GetMapCoreSys( const WFLBY_MAPCONT* cp_sys );

#endif		// __WFLBY_MAPCONT_H__


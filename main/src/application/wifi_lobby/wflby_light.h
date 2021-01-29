//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_light.h
 *	@brief		���C�g�R���g���[��
 *	@author		tomoya takahashi
 *	@data		2008.01.25
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_LIGHT_H__
#define __WFLBY_LIGHT_H__

#include "wflby_def.h"

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
///	���C�g�V�X�e��
//=====================================
typedef struct _WFLBY_LIGHTWK WFLBY_LIGHTWK;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

extern WFLBY_LIGHTWK* WFLBY_LIGHT_Init( u32 heapID, WFLBY_LIGHT_NEON_ROOMTYPE roomtype, WFLBY_LIGHT_NEON_FLOORTYPE floor, WFLBY_LIGHT_NEON_MONUTYPE monu );
extern void WFLBY_LIGHT_Exit( WFLBY_LIGHTWK* p_wk );
extern void WFLBY_LIGHT_Main( WFLBY_LIGHTWK* p_wk );
extern void WFLBY_LIGHT_SetRoom( WFLBY_LIGHTWK* p_wk, WFLBY_LIGHT_NEON_ROOMTYPE roomtype );
extern void WFLBY_LIGHT_SetFloor( WFLBY_LIGHTWK* p_wk, WFLBY_LIGHT_NEON_FLOORTYPE floor );
extern void WFLBY_LIGHT_SetMonu( WFLBY_LIGHTWK* p_wk, WFLBY_LIGHT_NEON_MONUTYPE monu );
#endif		// __WFLBY_LIGHT_H__


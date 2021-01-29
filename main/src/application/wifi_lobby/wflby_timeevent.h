//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_timeevent.h
 *	@brief		�^�C���C�x���g
 *	@author		tomoya takahshi
 *	@data		2008.01.26
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_TIMEEVENT_H__
#define __WFLBY_TIMEEVENT_H__

#include "wflby_room_def.h"
#include "wflby_float.h"

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
///	�^�C���C�x���g���[�N
//=====================================
typedef struct _WFLBY_TIMEEVENT WFLBY_TIMEEVENT;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
extern WFLBY_TIMEEVENT* WFLBY_TIMEEVENT_Init( u32 heapID, WFLBY_ROOMWK* p_rmwk );
extern void WFLBY_TIMEEVENT_Exit( WFLBY_TIMEEVENT* p_wk );
extern void WFLBY_TIMEEVENT_Main( WFLBY_TIMEEVENT* p_wk );

// �t���[�g
extern BOOL WFLBY_TIMEEVENT_FLOAT_CheckAnm( const WFLBY_TIMEEVENT* cp_wk, u32 floatidx, WFLBY_FLOAT_ANM_TYPE anmidx );

// �ԉ�SE��~
extern void WFLBY_TIMEEVENT_FIRE_StopSe( WFLBY_TIMEEVENT* p_wk );

#endif		// __WFLBY_TIMEEVENT_H__


//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_float.h
 *	@brief		�t���[�g�Ǘ��V�X�e��	(SYSTEM�̃t���[�g�f�[�^��`��ɔ��f������V�X�e��)
 *	@author		tomoya takahashi
 *	@data		2008.01.29
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_FLOAT_H__
#define __WFLBY_FLOAT_H__

#include "wflby_room_def.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�A�j����ޒ萔
//=====================================
typedef enum {
	WFLBY_FLOAT_ANM_SOUND,	// ���A�j��
	WFLBY_FLOAT_ANM_SHAKE,	// ���A�j��
	WFLBY_FLOAT_ANM_BODY,	// ���̃A�j��
	WFLBY_FLOAT_ANM_NUM,
} WFLBY_FLOAT_ANM_TYPE;

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�t���[�g�Ǘ����[�N
//=====================================
typedef struct _WFLBY_FLOAT_CONT WFLBY_FLOAT_CONT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
extern WFLBY_FLOAT_CONT* WFLBY_FLOAT_CONT_Init( WFLBY_ROOMWK* p_rmwk, u32 heapID );
extern void WFLBY_FLOAT_CONT_Exit( WFLBY_FLOAT_CONT* p_sys );
extern void WFLBY_FLOAT_CONT_Main( WFLBY_FLOAT_CONT* p_sys );

extern BOOL WFLBY_FLOAT_CONT_CheckAnm( const WFLBY_FLOAT_CONT* cp_sys, u32 floatidx, WFLBY_FLOAT_ANM_TYPE anmidx );

#endif		// __WFLBY_FLOAT_H__


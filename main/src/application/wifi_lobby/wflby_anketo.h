//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_anketo.h
 *	@brief		�A���P�[�g���
 *	@author		tomoya takahashi
 *	@data		2008.05.29
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_ANKETO_H__
#define __WFLBY_ANKETO_H__


#include "savedata/savedata_def.h"

#include "system/procsys.h"

#include "application/wifi_lobby/wflby_def.h"
#include "application/wifi_lobby/wflby_system_def.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	����^�C�v
//=====================================
typedef enum{
	ANKETO_MOVE_TYPE_INPUT,	// ����
	ANKETO_MOVE_TYPE_VIEW,	// ������

	ANKETO_MOVE_TYPE_NUM,	// ��
} ANKETO_MOVE_TYPE;

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�A���P�[�g�f�[�^
//=====================================
typedef struct {
	SAVEDATA*				p_save;		// �Z�[�u�f�[�^
	WFLBY_SYSTEM*			p_system;
	ANKETO_MOVE_TYPE		move_type;	// ����^�C�v
} ANKETO_PARAM;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

extern PROC_RESULT ANKETO_Init(PROC* p_proc, int* p_seq);
extern PROC_RESULT ANKETO_Main(PROC* p_proc, int* p_seq);
extern PROC_RESULT ANKETO_Exit(PROC* p_proc, int* p_seq);

#endif		// __WFLBY_ANKETO_H__


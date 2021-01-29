//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		worldtimer.h
 *	@brief		���E���v
 *	@author		tomoya takahashi
 *	@data		2007.09.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WORLDTIMER_H__
#define __WORLDTIMER_H__

#include "savedata/savedata_def.h"
#include "application/wifi_lobby/wflby_def.h"
#include "system/procsys.h"
#include "application/wifi_lobby/wflby_system_def.h"

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
///	���E���v�f�[�^
//=====================================
typedef struct {
	const WFLBY_WLDTIMER*	cp_data;	// �\���f�[�^
	SAVEDATA*				p_save;		// �Z�[�u�f�[�^
	WFLBY_TIME				worldtime;	// ���E����
	WFLBY_SYSTEM*			p_system;
} WLDTIMER_PARAM;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
extern PROC_RESULT WLDTIMER_Init(PROC* p_proc, int* p_seq);
extern PROC_RESULT WLDTIMER_Main(PROC* p_proc, int* p_seq);
extern PROC_RESULT WLDTIMER_Exit(PROC* p_proc, int* p_seq);

#ifdef  PM_DEBUG
extern PROC_RESULT WLDTIMER_DebugInit(PROC* p_proc, int* p_seq);
extern PROC_RESULT WLDTIMER_DebugExit(PROC* p_proc, int* p_seq);
#endif


#endif		// __WORLDTIMER_H__


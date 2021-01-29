//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_connect.h
 *	@brief		Wi-Fi���r�[�ɐڑ�	�ؒf�����v���b�N
 *	@author		tomoya takahashi
 *	@data		2007.12.12
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_CONNECT_H__
#define __WFLBY_CONNECT_H__

#include "system/procsys.h"

#include "savedata/savedata_def.h"

#include "field/wflby_counter.h"

#include "wflby_system_def.h"

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
///	�p�����[�^
//=====================================
typedef struct {
	SAVEDATA* p_save;
	WFLBY_SYSTEM* p_system;	
	BOOL	check_skip;
	WFLBY_COUNTER_TIME* p_wflby_counter;	// WiFi�Ђ�Γ������Ԋi�[��

	// ���Œ��ɓ������Ȃ�s�q�t�d������
	BOOL enter;

} WFLBY_CONNECT_PARAM;

//-------------------------------------
///	�ގ��p�����[�^
//=====================================
typedef struct {
	SAVEDATA* p_save;
	WFLBY_SYSTEM* p_system;	
	BOOL timeout;

} WFLBY_DISCONNECT_PARAM;



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

extern PROC_RESULT WFLBY_CONNECT_Init(PROC* p_proc, int* p_seq);
extern PROC_RESULT WFLBY_CONNECT_Main(PROC* p_proc, int* p_seq);
extern PROC_RESULT WFLBY_CONNECT_Exit(PROC* p_proc, int* p_seq);

extern PROC_RESULT WFLBY_DISCONNECT_Init(PROC* p_proc, int* p_seq);
extern PROC_RESULT WFLBY_DISCONNECT_Main(PROC* p_proc, int* p_seq);
extern PROC_RESULT WFLBY_DISCONNECT_Exit(PROC* p_proc, int* p_seq);


#endif		// __WFLBY_CONNECT_H__


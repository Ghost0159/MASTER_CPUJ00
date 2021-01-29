//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		bucket.h
 *	@brief		�o�P�b�g�Q�[��
 *	@author		tomoya takahashi
 *	@data		2007.06.19
 *															
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __BUCKET_H__
#define __BUCKET_H__

#include "savedata/savedata_def.h"

#include "system/procsys.h"

#include "application/wifi_lobby/minigame_tool.h"


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
///	�o�P�b�g�Q�[���v���Z�X���[�N
//=====================================
typedef struct {
	WFLBY_MINIGAME_WK	lobby_wk;		//  �����ɃK�W�F�b�g�̏�Ԃ������Ă��܂��B�Q�[����͂��̒l����{���ɐݒ肵�܂��B
	SAVEDATA*		p_save;
	u8 vchat;
	u8 wifi_lobby;
	u8 pad[2];
} BUCKET_PROC_WORK;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

extern PROC_RESULT BucketProc_Init( PROC * p_proc, int * p_seq );
extern PROC_RESULT BucketProc_Main( PROC* p_proc, int* p_seq );
extern PROC_RESULT BucketProc_End( PROC* p_proc, int* p_seq );


#endif		// __BUCKET_H__


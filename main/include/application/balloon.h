//==============================================================================
/**
 * @file	balloon.h
 * @brief	���D����̃w�b�_
 * @author	matsuda
 * @date	2007.11.06(��)
 */
//==============================================================================
#ifndef __BALLOON_H__
#define __BALLOON_H__

// �Ƃ肠���������ɓ���܂��A���c����̏���������Ȃ��Ȃ�����Ă��������B
#include "application/wifi_lobby/minigame_tool.h"

//==============================================================================
//	�\���̒�`
//==============================================================================
//--------------------------------------------------------------
/**
 *	���D����Q�[���v���Z�X���[�N(parent_work)
 */
//--------------------------------------------------------------
typedef struct {
	WFLBY_MINIGAME_WK lobby_wk;
	SAVEDATA* p_save;
	u8 vchat;
	u8 wifi_lobby;
	u8 pad[2];
	
#ifdef PM_DEBUG
	int debug_offline;		///<TRUE:�f�o�b�O�p�̃I�t���C�����[�h
#endif
}BALLOON_PROC_WORK;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern PROC_RESULT BalloonProc_Init( PROC * proc, int * seq );
extern PROC_RESULT BalloonProc_Main( PROC * proc, int * seq );
extern PROC_RESULT BalloonProc_End(PROC *proc, int *seq);


#endif		// __BALLOON_H__


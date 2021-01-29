//==============================================================================
/**
 * @file	balance_ball.h
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2007.09.25(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#ifndef __BALANCE_BALL_H__
#define __BALANCE_BALL_H__

#include "system/procsys.h"
#include "savedata/savedata_local.h"
#include "application/wifi_lobby/minigame_tool.h"

typedef struct {
	WFLBY_MINIGAME_WK lobby_wk;

//	u8 voice_chat;
	SAVEDATA*	p_save;
	BOOL		wifi_lobby;
	u32			vchat;			///< voice_chat
	
} BB_PROC_WORK;


extern PROC_RESULT BalanceBallProc_Init( PROC* proc, int* seq );
extern PROC_RESULT BalanceBallProc_Main( PROC* proc, int* seq );
extern PROC_RESULT BalanceBallProc_Exit( PROC* proc, int* seq );


#endif		// __BUCKET_H__


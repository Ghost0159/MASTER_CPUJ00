//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wifi_note_snd.h
 *	@brief		�F�B�蒠�@�T�E���h
 *	@author		tomoya takahshi
 *	@data		2007.07.24
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_NOTE_SND_H__
#define __WIFI_NOTE_SND_H__

#include "system/snd_tool.h"



// SE�p��`
//#define WIFINOTE_MOVE_SE		(SEQ_SE_DP_SELECT)
#define WIFINOTE_MOVE_SE		(SEQ_SE_DP_BUTTON3)

#define WIFINOTE_MENU_MOVE_SE	(SEQ_SE_DP_SELECT)	// ���j���[�̈ړ���

//#define WIFINOTE_DECIDE_SE	(SEQ_SE_DP_SELECT)
//#define WIFINOTE_DECIDE_SE	(SEQ_SE_DP_BUTTON3)
#define WIFINOTE_DECIDE_SE		(SEQ_SE_DP_DECIDE)

#define WIFINOTE_MENU_DECIDE_SE	(SEQ_SE_DP_SELECT)	// ���j���[�̌���

//#define WIFINOTE_BS_SE		(SEQ_SE_DP_SELECT)
//#define WIFINOTE_BS_SE		(SEQ_SE_DP_BUTTON3)
#define WIFINOTE_BS_SE			(SEQ_SE_DP_DECIDE)

#define WIFINOTE_MENU_BS_SE		(SEQ_SE_DP_SELECT)	// ���j���[�̃L�����Z��

#define WIFINOTE_CLEAN_SE		(SEQ_SE_DP_UG_025)	// �����S���ŏ�����

//#define WIFINOTE_SCRLL_SE		(SEQ_SE_DP_BOX02)	// �X�N���[����
#define WIFINOTE_SCRLL_SE		(SEQ_SE_DP_SELECT5)	// �X�N���[����

#endif		// __WIFI_NOTE_SND_H__


//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		lobby_news_snd.h
 *	@brief		WiFi���r�[�j���[�X	��
 *	@author		tomoya takahashi
 *	@data		2007.10.23
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __LOBBY_NEWS_SND_H__
#define __LOBBY_NEWS_SND_H__

#include "system/snd_tool.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//#define NEWSDRAW_NEWTOPIC	( SEQ_SE_DP_DECIDE )		// �V�����g�s�b�N
//#define NEWSDRAW_NEWTOPIC	( SEQ_SE_DP_PIRORIRO )		// �V�����g�s�b�N
#define NEWSDRAW_NEWTOPIC	( SEQ_SE_DP_PIRORIRO2 )		// �V�����g�s�b�N

#define NEWSDRAW_NEWTIME	( SEQ_SE_DP_DECIDE )		// ���ԍX�V
//#define NEWSDRAW_NEWTIME	( SEQ_SE_DP_DENSI01 )		// ���ԍX�V
//#define NEWSDRAW_NEWTIME	( SEQ_SE_DP_DENSI01 )		// ���ԍX�V

//#define NEWSDRAW_NEWPLAYER	( SEQ_SE_DP_DECIDE )	// �l���������A�A����
//#define NEWSDRAW_NEWPLAYER	( SEQ_SE_DP_TARARARAN )	// �l���������A�A����(��Ȃ�)
//#define NEWSDRAW_NEWPLAYER	( SEQ_SE_DP_DECIDE )		// �l���������A�A����
//
//���̉�ʂƓ�����͂��邽�߂ɕύX���܂���(08.03..07)
//#define NEWSDRAW_NEWPLAYER	( SEQ_SE_DP_BUTTON3 )		// �l���������A�A����
#define NEWSDRAW_NEWPLAYER	( SEQ_SE_DP_BUTTON9 )		// �l���������A�A����


#endif		// __LOBBY_NEWS_SND_H__



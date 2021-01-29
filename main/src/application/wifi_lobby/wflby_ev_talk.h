//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_ev_talk.h
 *	@brief		��b�C�x���g
 *	@author		tomoya takahashi
 *	@data		2007.12.18
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_EV_TALK_H__
#define __WFLBY_EV_TALK_H__

#include "wflby_event.h"

#ifdef PM_DEBUG
//#define WFLBY_DEBUG_TALK_ALLWORLD	// �g�s�b�N�O�̌Z����񂪐��E�̈��A�𗬂�
#endif

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

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------


// ��b�͂Ȃ�������
extern BOOL WFLBY_EV_TALK_StartA( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );

// ��b�͂Ȃ�������ꂽ
extern BOOL WFLBY_EV_TALK_StartB( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );

// �g�s�b�N��b
extern BOOL WFLBY_EV_DEF_PlayerA_SWTOPIC_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );

// �V�т𐄑E����l
extern BOOL WFLBY_EV_DEF_PlayerA_SWTOPIC_PLAYED_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );



#ifdef WFLBY_DEBUG_TALK_ALLWORLD
extern BOOL WFLBY_EV_TALK_StartA_AllWorld( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
#endif

#endif		// __WFLBY_EV_TALK_H__


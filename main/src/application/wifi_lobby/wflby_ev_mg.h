//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_ev_mg.h
 *	@brief		�~�j�Q�[���C�x���g
 *	@author		tomoya takahashi
 *	@data		2008.01.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_EV_MG_H__
#define __WFLBY_EV_MG_H__

#include "wflby_event.h"

#include "msgdata/msg_wifi_h_info.h"
#include "msgdata/msg_wifi_hiroba.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	in_ok�ɓ����Ă���l
//=====================================
enum{
	WFLBY_EV_MG_RET_NG_NUM,			// �l���������ς��œ���Ȃ�
	WFLBY_EV_MG_RET_NG_PLAYING,		// �Q�[�����Ȃ̂œ���Ȃ�
	WFLBY_EV_MG_RET_NG_TIME,		// �~�j�Q�[����I�����Ă���̂œ���Ȃ�
	WFLBY_EV_MG_RET_NG_MIN,			// �K�v�Ȑl���������Ȃ�����
	WFLBY_EV_MG_RET_NG_DISCON,		// �N�����ؒf����
	WFLBY_EV_MG_RET_NG_MY,			// ����f����
	WFLBY_EV_MG_RET_NG_BCAN,		// BCancel����
	WFLBY_EV_MG_RET_NG_LOCK,		// �ȑO�e�̂Ƃ���BCancel�����̂ŎQ�����b�N��
	WFLBY_EV_MG_RET_OK,
};
//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�~�j�Q�[���Q���p�����[�^
//=====================================
typedef struct {
	u8 in_ok;			// ��W�ɎQ���ł�����
	u8 mg_type;			// DWC�~�j�Q�[���^�C�v
	u8 wflby_mg_type;	// ���r�[���~�j�Q�[���^�C�v
	u8 wflby_mg_status;	// �Q����ɐݒ肷��X�e�[�^�X
} WFLBY_EV_MG_PARAM;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
extern BOOL WFLBY_EV_MG_Start( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
extern BOOL WFLBY_EV_MG_End( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );
extern BOOL WFLBY_EV_DEF_PlayerA_MINIGAME_Before( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );

#endif		// __WFLBY_EV_MG_H__


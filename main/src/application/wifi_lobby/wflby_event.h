//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_event.h
 *	@brief		�C�x���g�Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.11.26
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_EVENT_H__
#define __WFLBY_EVENT_H__

#include "wflby_event_def.h"
#include "wflby_room_def.h"

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
///	�C�x���g�^�錾
//	
//	p_wk	���̃C�x���g�̃��[�N
//	p_rmwk	�����̑S�f�[�^
//	plno	�ΏۂƂȂ�v���C���[No�iGlobal�C�x���g�͏�ɂO�j
//
//	BOOL	TRUE:�I��	FALSE:���s
//
//	TRUE��Ԃ��ƁA�ǂ̃^�C�~���O�ŌĂ΂ꂽ�֐��ł���A�C�x���g���ƏI�����܂��B
//=====================================
typedef BOOL (*pEVENT_FUNC)( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno );


//-------------------------------------
///	�C�x���g�ݒ胏�[�N
// �g�p���Ȃ��^�C�~���O�̊֐��|�C���^�ϐ��ɂ�NULL������Ă�������
//=====================================
typedef struct {
	pEVENT_FUNC p_before;		// �I�u�W�F���N�G�X�g�ݒ�O
	pEVENT_FUNC	p_after;		// �I�u�W�F���N�G�X�g�ݒ��
} WFLBY_EVENT_DATA;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
// �C�x���g�V�X�e���Ǘ�
//=====================================
extern WFLBY_EVENT* WFLBY_EVENT_Init( WFLBY_ROOMWK* p_rmwk, u32 heapID );
extern void WFLBY_EVENT_Exit( WFLBY_EVENT* p_sys );

//-------------------------------------
// �e���s�֐�
//=====================================
extern void WFLBY_EVENT_MainBefore( WFLBY_EVENT* p_sys );	// �I�u�W�F���N�G�X�g�ݒ�O
extern void WFLBY_EVENT_MainAfter( WFLBY_EVENT* p_sys );	// �I�u�W�F���N�G�X�g�ݒ��

//-------------------------------------
// ���[�N�֐�
//=====================================
extern void* WFLBY_EVENTWK_GetParam( WFLBY_EVENTWK* p_wk );
extern void* WFLBY_EVENTWK_AllocWk( WFLBY_EVENTWK* p_wk, u32 size );
extern void WFLBY_EVENTWK_DeleteWk( WFLBY_EVENTWK* p_wk );
extern void* WFLBY_EVENTWK_GetWk( WFLBY_EVENTWK* p_wk );
extern u32 WFLBY_EVENTWK_GetSeq( const WFLBY_EVENTWK* cp_wk );
extern void WFLBY_EVENTWK_SetSeq( WFLBY_EVENTWK* p_wk, u32 seq );
extern void WFLBY_EVENTWK_AddSeq( WFLBY_EVENTWK* p_wk );


//-------------------------------------
// �ʃC�x���g�Ǘ�
//=====================================
// �㏑��	��ɂ��̊֐����ĂԂ悤�ɂȂ�܂��B�i�풓�C�x���g�j
extern void WFLBY_EVENT_SetPrivateEvent( WFLBY_EVENT* p_sys, u32 plno, const WFLBY_EVENT_DATA* cp_data, void* p_param );
// ���荞�݃C�x���g	Wait�֐���TRUE���ς��Ă���܂ł��̊֐��̒��g�����s���܂��B
// ���s��́A���̑O�ɐݒ肳��Ă����֐��i�풓�C�x���g�j���Ăт܂�
// ���̊֐���ǂ񂾌�A�V�[�P���X�Ȃǂ͐ݒ肵���C�x���g�̂��̂ɂȂ��Ă��܂��܂��B���ӂ��Ă�������
extern void WFLBY_EVENT_SetPrivateIntrEvent( WFLBY_EVENT* p_sys, u32 plno, const WFLBY_EVENT_DATA* cp_data, void* p_param );
extern BOOL WFLBY_EVENT_WaitPrivateIntrEvent( const WFLBY_EVENT* cp_sys, u32 plno );

#ifdef PM_DEBUG
// �f�o�b�N
extern void WFLBY_EVENT_SetPrivateEvent_DEBUG( WFLBY_EVENT* p_sys, u32 plno, const WFLBY_EVENT_DATA* cp_data, void* p_param, u32 line );
extern void WFLBY_EVENT_SetPrivateIntrEvent_DEBUG( WFLBY_EVENT* p_sys, u32 plno, const WFLBY_EVENT_DATA* cp_data, void* p_param, u32 line );

#define WFLBY_EVENT_SET_PRIVATE_EVENT( p_sys, plno, cp_data, p_param )		WFLBY_EVENT_SetPrivateEvent_DEBUG( p_sys, plno, cp_data, p_param, __LINE__ )
#define WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_sys, plno, cp_data, p_param )		WFLBY_EVENT_SetPrivateIntrEvent_DEBUG( p_sys, plno, cp_data, p_param, __LINE__ )

#else

//�@ �����[�X
#define WFLBY_EVENT_SET_PRIVATE_EVENT( p_sys, plno, cp_data, p_param )		WFLBY_EVENT_SetPrivateEvent( p_sys, plno, cp_data, p_param )
#define WFLBY_EVENT_SET_PRIVATE_INTR_EVENT( p_sys, plno, cp_data, p_param )		WFLBY_EVENT_SetPrivateIntrEvent( p_sys, plno, cp_data, p_param )

#endif

//  ����ݒ�
//  �C�x���g���~�������l���ӔC�������āA�Đ������邱�ƁI
extern void WFLBY_EVENT_SetPrivateEventMove( WFLBY_EVENT* p_sys, u32 plno, BOOL move );



//-------------------------------------
// �S�̃C�x���g�Ǘ�
// ���s��������S�̂̓�����~���āA���̏������s���܂��B
//=====================================
extern void WFLBY_EVENT_SetGlobalEvent( WFLBY_EVENT* p_sys, const WFLBY_EVENT_DATA* cp_data, void* p_param );
extern BOOL WFLBY_EVENT_WaitGlobalEvent( const WFLBY_EVENT* cp_sys );


#endif		// __WFLBY_EVENT_H__


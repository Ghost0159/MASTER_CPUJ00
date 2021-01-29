//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_client.h
 *	@brief		�N���C�A���g�V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.02.22
 *
 *	�������`��PROC��
 *	���̑�PROC�ɕ��򂷂鏈�����Ǘ�
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WBR_CLIENT_H__
#define __WBR_CLIENT_H__

#include "wbr_common.h"
#include "include/field/field.h"

#undef GLOBAL
#ifdef	__WBR_CLIENT_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	���
//=====================================
enum {
	WBR_CLIENT_SEQ_START_WAIT,		// �e����̊J�n���ߑ҂�
	WBR_CLIENT_SEQ_COMMDATA_SEND,	// ���ʃf�[�^���M
	WBR_CLIENT_SEQ_COMMDATA_WAIT,	// ���ʃf�[�^��M�҂�
	WBR_CLIENT_SEQ_WALK_START,
	WBR_CLIENT_SEQ_WALK_ENDWAIT,
	WBR_CLIENT_SEQ_TRCARD_START,
	WBR_CLIENT_SEQ_TRCARD_ENDWAIT,
	WBR_CLIENT_SEQ_END,
} ;
//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�N���C�A���g���[�N
//=====================================
typedef struct _WBR_CLIENT WBR_CLIENT;

//-------------------------------------
/// �N���C�A���g�V�X�e���������f�[�^
//=====================================
typedef struct {
	u32 netid;
	const WBR_MYSTATUS* cp_mystatus;
	FIELDSYS_WORK* p_fsys;
} WBR_CLIENT_INIT;



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
GLOBAL WBR_CLIENT* WBR_ClientInit( const WBR_CLIENT_INIT* p_init, u32 heapID );
GLOBAL void WBR_ClientExit( WBR_CLIENT* p_sys );

GLOBAL BOOL WBR_ClientMain( WBR_CLIENT* p_sys );

GLOBAL void WBR_ClientSeqSet( WBR_CLIENT* p_sys, u32 seq );
GLOBAL void WBR_ClientCommDataSet( WBR_CLIENT* p_sys, const WBR_COMM_COMMON* cp_data );
GLOBAL void WBR_ClientGameDataSet( WBR_CLIENT* p_sys, const WBR_OYA_COMM_GAME* cp_data );
GLOBAL void WBR_ClientTalkDataSet( WBR_CLIENT* p_sys, const WBR_KO_COMM_TALK* cp_data );
GLOBAL void WBR_ClientGameEndSet( WBR_CLIENT* p_sys );

GLOBAL u8* WBR_ClientCommDataBuffPtrGet( WBR_CLIENT* p_sys, u32 netid );

#undef	GLOBAL
#endif		// __WBR_CLIENT_H__


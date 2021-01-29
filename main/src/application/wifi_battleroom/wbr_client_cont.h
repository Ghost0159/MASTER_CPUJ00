//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_client_cont.h
 *	@brief		�N���C�A���g�I�u�W�F�N�g�Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.04.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WBR_CLIENT_CONT_H__
#define __WBR_CLIENT_CONT_H__

#include "system/procsys.h"
#include "wbr_client_local.h"
#include "wbr_common.h"

#undef GLOBAL
#ifdef	__WBR_CLIENT_CONT_H_GLOBAL
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
///	��ɕԂ����̃V�[�P���X�t���O
//=====================================
typedef enum {
	WBR_CLIENTCONT_RET_NONE,
	WBR_CLIENTCONT_RET_END,
	WBR_CLIENTCONT_RET_TRCARD,
} WBR_CLIENTCONT_RETTYPE;

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

GLOBAL PROC_RESULT WBR_ClientCont_Init( PROC* p_proc, int* p_seq );
GLOBAL PROC_RESULT WBR_ClientCont_Main( PROC* p_proc, int* p_seq );
GLOBAL PROC_RESULT WBR_ClientCont_End( PROC* p_proc, int* p_seq );

GLOBAL void WBR_ClientCont_SetOyaGameData( void* p_procw, const WBR_OYA_COMM_GAME* cp_oyadata );
GLOBAL void WBR_ClientCont_SetTalkData( void* p_procw, const WBR_KO_COMM_TALK* cp_talkdata );
GLOBAL void WBR_ClientCont_SetEndFlag( void* p_procw );

#undef	GLOBAL
#endif		// __WBR_CLIENT_CONT_H__


//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_cmdq.h
 *	@brief		�R�}���h�L���[
 *	@author		tomoya takahashi
 *	@data		2007.03.28
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WF2DMAP_CMDQ_H__
#define __WF2DMAP_CMDQ_H__

#include "application/wifi_2dmap/wf2dmap_common.h"

#undef GLOBAL
#ifdef	__WF2DMAP_CMDQ_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *		�A�N�V�����R�}���h	�L���[
 *		�i��������֗��Ȃ̂ō쐬���܂����j
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�A�N�V�����R�}���h	�L���[
//=====================================
typedef struct _WF2DMAP_ACTCMDQ WF2DMAP_ACTCMDQ;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//�@�V�X�e�����[�N
GLOBAL WF2DMAP_ACTCMDQ* WF2DMAP_ACTCMDQSysInit( u32 buffnum, u32 heapID );
GLOBAL void WF2DMAP_ACTCMDQSysExit( WF2DMAP_ACTCMDQ* p_sys );

// �A�N�V�����R�}���h��ݒ�
GLOBAL void WF2DMAP_ACTCMDQSysCmdPush( WF2DMAP_ACTCMDQ* p_sys, const WF2DMAP_ACTCMD* cp_buff );
// �R�}���h�f�[�^�擾 
GLOBAL BOOL WF2DMAP_ACTCMDQSysCmdPop( WF2DMAP_ACTCMDQ* p_sys, WF2DMAP_ACTCMD* p_cmd );
// �o�b�t�@���擾
GLOBAL u32 WF2DMAP_ACTCMDQSysBuffNumGet( const WF2DMAP_ACTCMDQ* cp_sys );



//-----------------------------------------------------------------------------
/**
 *		���N�G�X�g�R�}���h�L���[
 *		�i��������֗��Ȃ̂ō쐬���܂����j
 */
//-----------------------------------------------------------------------------

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
///	���N�G�X�g�R�}���h�L���[
//=====================================
typedef struct _WF2DMAP_REQCMDQ WF2DMAP_REQCMDQ;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//�@�V�X�e�����[�N
GLOBAL WF2DMAP_REQCMDQ* WF2DMAP_REQCMDQSysInit( u32 buffnum, u32 heapID );
GLOBAL void WF2DMAP_REQCMDQSysExit( WF2DMAP_REQCMDQ* p_sys );

// ���N�G�X�g�R�}���h��ݒ�
GLOBAL void WF2DMAP_REQCMDQSysCmdPush( WF2DMAP_REQCMDQ* p_sys, const WF2DMAP_REQCMD* cp_cmd );
// �R�}���h�f�[�^�擾 
GLOBAL BOOL WF2DMAP_REQCMDQSysCmdPop( WF2DMAP_REQCMDQ* p_sys, WF2DMAP_REQCMD* p_cmd );

// �o�b�t�@���擾
GLOBAL u32 WF2DMAP_REQCMDQSysBuffNumGet( const WF2DMAP_REQCMDQ* cp_sys );

#undef	GLOBAL
#endif		// __WF2DMAP_CMDQ_H__


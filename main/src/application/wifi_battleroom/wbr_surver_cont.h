//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_surver_cont.h
 *	@brief		�T�[�o�[�@�I�u�W�F�N�g�Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.04.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WBR_SURVER_CONT_H__
#define __WBR_SURVER_CONT_H__

#include "wbr_common.h"

#undef GLOBAL
#ifdef	__WBR_SURVER_CONT_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
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
//-------------------------------------
///	�T�[�o�[�I�u�W�F�N�g�Ǘ�
//=====================================
typedef struct _WBR_SURVERCONT WBR_SURVERCONT;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
// �V�X�e������
GLOBAL WBR_SURVERCONT* WBR_SurverContInit( u32 objnum, u32 heapID );
GLOBAL void WBR_SurverContExit( WBR_SURVERCONT* p_sys );
GLOBAL BOOL WBR_SurverContMain( WBR_SURVERCONT* p_sys );

// �R�}���h�ݒ�
GLOBAL void WBR_SurverContNowStatusActCmdSet( WBR_SURVERCONT* p_sys );
GLOBAL void WBR_SurverContReqCmdSet( WBR_SURVERCONT* p_sys, const WF2DMAP_REQCMD* cp_req );
GLOBAL void WBR_SurverContTalkDataSet( WBR_SURVERCONT* p_sys, u32 aid, const WBR_TALK_DATA* cp_data );
GLOBAL void WBR_SurverContTalkReqDataSet( WBR_SURVERCONT* p_sys, const WBR_TALK_REQ* cp_data );

// �ʐM�f�[�^�擾
GLOBAL BOOL WBR_SurverContSendDataPop( WBR_SURVERCONT* p_sys, WBR_OBJ_DATA* p_buff );


#undef	GLOBAL
#endif		// __WBR_SURVER_CONT_H__


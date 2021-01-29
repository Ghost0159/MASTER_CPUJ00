//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_surver.h
 *	@brief		�T�[�o�[�V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.02.21
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WBR_SURVER_H__
#define __WBR_SURVER_H__

#undef GLOBAL
#ifdef	__WBR_SURVER_H_GLOBAL
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
///	�V�[�P���X
//=====================================
enum {
	WBR_SURVER_SEQ_START_SEND,
	WBR_SURVER_SEQ_GETCOMMON_DATA,
	WBR_SURVER_SEQ_MAIN_START,
	WBR_SURVER_SEQ_MAIN,
	WBR_SURVER_SEQ_END_SEND,
	WBR_SURVER_SEQ_END_WAIT,
	WBR_SURVER_SEQ_NUM,
} ;

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�T�[�o�[���[�N
//=====================================
typedef struct _WBR_SURVER WBR_SURVER;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
GLOBAL WBR_SURVER* WBR_SurverInit( u32 heap );
GLOBAL void WBR_SurverExit( WBR_SURVER* p_sys );

// �Ǘ����ĕK�v�Ȃ�R�}���h�𑗐M
GLOBAL void WBR_SurverMain( WBR_SURVER* p_sys );

// �f�[�^�ݒ�֐�
GLOBAL void WBR_SurverSeqSet( WBR_SURVER* p_sys, u32 seq );
GLOBAL u32 WBR_SurverSeqGet( const WBR_SURVER* cp_sys );
GLOBAL void WBR_SurverKoGameDataSet( WBR_SURVER* p_sys, const WBR_KO_COMM_GAME* cp_ko, u32 id );
GLOBAL void WBR_SurverKoTalkDataSet( WBR_SURVER* p_sys, const WBR_KO_COMM_TALK* cp_ko, u32 id );
GLOBAL void WBR_SurverKoTalkReqSet( WBR_SURVER* p_sys, const WBR_TALK_REQ* cp_ko, u32 id );

GLOBAL BOOL WBR_SurverKoCommDataRecv( WBR_SURVER* p_sys, u32 id );

// ���̏�Ԃ��q�ɋ����]��
GLOBAL void WBR_SurverDataSend( WBR_SURVER* p_sys );


#undef	GLOBAL
#endif		// __WBR_SURVER_H__


//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		bct_surver.h
 *	@brief		�T�[�o�[
 *	@author		tomoya takahashi
 *	@data		2007.06.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __BCT_SURVER_H__
#define __BCT_SURVER_H__

#include "bct_common.h"

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
///	�T�[�o�[�V�X�e��
//=====================================
typedef struct _BCT_SURVER	BCT_SURVER;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
extern BCT_SURVER* BCT_SURVER_Init( u32 heapID, u32 timeover, u32 comm_num, const BCT_GAMEDATA* cp_gamedata );
extern void BCT_SURVER_Delete( BCT_SURVER* p_wk );

// ���C���֐�
// FALSE���A���Ă�����I��
// �I�����߂ƃX�R�A���݂�Ȃɑ���
extern BOOL BCT_SURVER_Main( BCT_SURVER* p_wk );

// �Q�[�����x���̕ύX���`�F�b�N
extern BOOL BCT_SURVER_CheckGameLevelChange( const BCT_SURVER* cp_wk );	// �ύX�����������H
extern void BCT_SURVER_ClearGameLevelChange( BCT_SURVER* p_wk );		// �t���O���N���A
extern u32	BCT_SURVER_GetGameLevel( const BCT_SURVER* cp_wk );			// ���̃��x�����擾

// �؂̎�������������ݒ�
extern void BCT_SURVER_SetNutData( BCT_SURVER* p_wk, const BCT_NUT_COMM* cp_data, u32 plno );

//  �݂�Ȃ̃X�R�A�����W
extern void BCT_SURVER_ScoreSet( BCT_SURVER* p_wk, u32 score, u32 plno );
extern BOOL BCT_SURVER_ScoreAllUserGetCheck( const BCT_SURVER* cp_wk );
extern void BCT_SURVER_ScoreGet( BCT_SURVER* p_wk, BCT_SCORE_COMM* p_data );

// �J�E���g�_�E����i�߂邩�ݒ�
extern void BCT_SURVER_SetCountDown( BCT_SURVER* p_wk, BOOL flag );

#endif		// __BCT_SURVER_H__


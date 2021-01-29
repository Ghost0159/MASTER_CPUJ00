//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wifi_hiroba_save.h
 *	@brief		Wi-Fi�Ђ��	�Z�[�u�f�[�^
 *	@author	
 *	@data		2008.05.22
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_HIROBA_SAVE_H__
#define __WIFI_HIROBA_SAVE_H__

// savedata
#include "savedata/savedata_def.h"	//SAVEDATA�Q�Ƃ̂���
#include "savedata/savedata.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define WFLBY_SAVEDATA_QUESTION_NONE	( 0xffffffff )	// �����A���P�[�g�ʂ��ԍ�

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�Z�[�u�f�[�^
//=====================================
typedef struct _WFLBY_SAVEDATA WFLBY_SAVEDATA;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
// �Z�[�u�f�[�^�V�X�e�����ˑ�����֐�
extern int SAVEDATA_WFLBY_GetWorkSize( void );
extern void SAVEDATA_WFLBY_InitWork( WFLBY_SAVEDATA* p_wk );
extern WFLBY_SAVEDATA* SAVEDATA_WFLBY_AllocWork( u32 heapID );

// �Z�[�u�f�[�^���[�N�擾
extern WFLBY_SAVEDATA* SAVEDATA_WFLBY_GetWork( SAVEDATA* p_sv );

// �f�[�^�擾
extern u32 SAVEDATA_WFLBY_GetQuestionNumber( const WFLBY_SAVEDATA* cp_wk );
extern u32 SAVEDATA_WFLBY_GetMyAnswer( const WFLBY_SAVEDATA* cp_wk );

// �f�[�^�ݒ�
extern void SAVEDATA_WFLBY_SetQuestionNumber( WFLBY_SAVEDATA* p_wk, u32 number );
extern void SAVEDATA_WFLBY_SetMyAnswer( WFLBY_SAVEDATA* p_wk, u32 answer );


#endif		// __WIFI_HIROBA_SAVE_H__


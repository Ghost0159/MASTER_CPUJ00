//==============================================================================
/**
 * @file	codein.h
 * @brief	�������̓C���^�[�t�F�[�X
 * @author	goto
 * @date	2007.07.11(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#ifndef __CODEIN_H__
#define __CODEIN_H__

#include "gflib/strbuf.h"
#include "savedata/config.h"
#include "gflib/button_man.h"

#define CODE_BLOCK_MAX		( 3 )		///< ���̓u���b�N�ő吔

// -----------------------------------------
//
//	�Ăяo�����ŊǗ����郏�[�N
//
// -----------------------------------------
typedef struct {
	
//	int			heap_id;					///< �g�p����HEAPID
	int			word_len;					///< ���͕�����	
	int			block[ CODE_BLOCK_MAX + 1 ];///< ���̓u���b�N�@xx-xxxx-xxx �Ƃ�
	
	int			end_state;					///< �I�����̏��
	STRBUF*		strbuf;						///< �󗓂Ȃ�o�b�t�@�����łȂ���΃f�t�H���g�l
	
	CONFIG*		cfg;						///< window_type�擾�p
	
} CODEIN_PARAM;

extern const PROC_DATA CodeInput_ProcData;

extern CODEIN_PARAM*	CodeInput_ParamCreate( int heap_id, int word_len, int block[], CONFIG* cfg );
extern void				CodeInput_ParamDelete( CODEIN_PARAM* codein_param );


//--------------------------------------------------------------
/**
 * @brief	xxxx-xxxx-xxxx �̃u���b�N��`�����
 *
 * @param	block[]	
 *
 * @retval	static inline	
 *
 */
//--------------------------------------------------------------
static inline void CodeIn_BlockDataMake_4_4_4( int block[] )
{
#if 1
	block[ 0 ] = 4;
	block[ 1 ] = 4;
	block[ 2 ] = 4;
#else
	block[ 0 ] = 2;
	block[ 1 ] = 5;
	block[ 2 ] = 5;
#endif
}


//--------------------------------------------------------------
/**
 * @brief	xx-xxxxx-xxxxx �̃u���b�N��`�����
 *
 * @param	block[]	
 *
 * @retval	static inline	
 *
 */
//--------------------------------------------------------------
static inline void CodeIn_BlockDataMake_2_5_5( int block[] )
{
	block[ 0 ] = 2;
	block[ 1 ] = 5;
	block[ 2 ] = 5;
}

#endif	///< __CODEIN_H__

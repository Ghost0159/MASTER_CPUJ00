//=============================================================================================
/**
 * @file	strbuf_family.h
 * @brief	�ėp������o�b�t�@�^�I�u�W�F�N�g�i�����ȕ����z����������W���[���̂�include���j
 * @author	taya
 * @date	2005.11.14
 */
//=============================================================================================
#ifndef __STRBUF_FAMILY_H__
#define __STRBUF_FAMILY_H__

#include "strbuf.h"


//------------------------------------------------------------------
/**
 * ���̕�����iEOM�I�[�j���o�b�t�@�I�u�W�F�N�g�ɃZ�b�g����
 *
 * @param   strbuf		[out] �o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^
 * @param   sz			[in]  EOM �ŏI��镶���z��
 *
 */
//------------------------------------------------------------------
extern void STRBUF_SetStringCode( STRBUF* strbuf, const STRCODE* sz );


//------------------------------------------------------------------
/**
 * ���̕�����i�����w��j���o�b�t�@�I�u�W�F�N�g�ɃZ�b�g����
 *
 * @param   strbuf		[out] �o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^
 * @param   str			[in]  �����z��̐擪�|�C���^
 * @param   len			[in]  �Z�b�g���镶�����iEOM���܂ށj
 *
 */
//------------------------------------------------------------------
extern void STRBUF_SetStringCodeOrderLength( STRBUF* strbuf, const STRCODE* str, u32 len );


//------------------------------------------------------------------
/**
 * �o�b�t�@���琶�̕����z����R�s�[����
 *
 * @param   strbuf		[in]  �o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^
 * @param   ary			[out] �R�s�[��z��
 * @param   arysize		[in]  �R�s�[��z��̗v�f��
 *
 */
//------------------------------------------------------------------
extern void STRBUF_GetStringCode( const STRBUF* strbuf, STRCODE* ary, u32 arysize );







//==============================================================================================
// �ȉ��̊֐��͋��𓾂��ɌĂяo���Ă͂����܂���I�I
//==============================================================================================

//------------------------------------------------------------------
/**
 * �o�b�t�@�I�u�W�F�N�g������镶���z��̃A�h���X��Ԃ�
 * �����̊֐����Ăяo���ӏ��͒�����B���Ԃ񕶎��o�͌n�̂݁B
 *
 * @param   strbuf				[in] �o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^
 *
 * @retval  const STRCODE*		�����z��̃A�h���X
 */
//------------------------------------------------------------------
extern const STRCODE* STRBUF_GetStringCodePointer( const STRBUF* strbuf );


//------------------------------------------------------------------
/**
// ������̘A������y�y �����g�p�֎~ �z�z
 *
 * @param   dst		[out] �A�������o�b�t�@�I�u�W�F�N�g
 * @param   src		[in]  �A�����镶��������o�b�t�@�I�u�W�F�N�g
 *
 */
//------------------------------------------------------------------
extern void STRBUF_AddStr( STRBUF* dst, const STRBUF* src );


//------------------------------------------------------------------
/**
 * �P�����A���y�y �����g�p�֎~ �z�z
 *
 * @param   dst			�A�������o�b�t�@�I�u�W�F�N�g
 * @param   code		�A�����镶���R�[�h
 *
 */
//------------------------------------------------------------------
extern void STRBUF_AddChar( STRBUF* dst, STRCODE code );


// ----------------------------------------------------------------------------
// localize_spec_mark(LANG_ALL) imatake 2006/12/07
// ���k���ꂽ������𔻕ʁE�W�J����֐���ǉ�

BOOL STRBUF_IsCompressed(STRBUF *strbuf);
void STRBUF_AddCompStr(STRBUF *dst, STRBUF *src);


#endif
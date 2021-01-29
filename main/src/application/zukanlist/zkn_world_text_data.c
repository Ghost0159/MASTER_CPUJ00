//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		zkn_world_text_data.c
 *	@brief		�}�Ӑ��E�e�L�X�g�f�[�^�@�ƃQ�[�������R�[�h�������N������f�[�^
 *	@author		tomoya takahashi	
 *	@data		2006.03.01
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "system.h"
#include "assert.h"
#include <string.h>

//#include "src/application/zukanlist/zkn_worldtext.h"

#define	__ZKN_WORLD_TEXT_DATA_H_GLOBAL
#include "include/application/zukanlist/zkn_world_text_data.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
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

//-----------------------------------------------------------------------------
/**
 *					�O���[�o���f�[�^
 */
//-----------------------------------------------------------------------------

//-------------------------------------
//	���R�[�h�ƃ|�P�����O����e�L�X�g�p���R�[�h
//	�Ƃ̑Ή��f�[�^
//=====================================
static const u8 ZKN_WORLD_TEXT_Idx[ ZKN_WORLD_TEXT_NUM ] = {
	LANG_JAPAN,
	LANG_ENGLISH,
	LANG_FRANCE,
	LANG_GERMANY,
	LANG_ITALY,
	LANG_SPAIN
};

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	���R�[�h����}�ӊO����e�L�X�g�R�[�h���擾
 *
 *	@param	lang_code	���R�[�h
 *
 *	@retval	�O����e�L�X�g�p���R�[�h	
 *	@retval	ZKN_WORLD_TEXT_NUM�̎��͊O����e�L�X�g�̖������R�[�h
 */
//-----------------------------------------------------------------------------
int ZKN_WT_GetLANG_Code_ZKN_WORLD_TEXT_Code( int lang_code )
{
	int i;

	for( i=0; i<ZKN_WORLD_TEXT_NUM; i++ ){

		if( lang_code == ZKN_WORLD_TEXT_Idx[i] ){
			break;
		}
	}
	
	return i;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�ӊO����e�L�X�g�R�[�h���獑�R�[�h���擾
 *
 *	@param	world_text_code	�}�ӊO����e�L�X�g�R�[�h
 *
 *	@retval	���R�[�h	
 */
//-----------------------------------------------------------------------------
int ZKN_WT_GetZKN_WORLD_TEXT_Code_LANG_Code( int world_text_code )
{
	GF_ASSERT( world_text_code < ZKN_WORLD_TEXT_NUM );
	return ZKN_WORLD_TEXT_Idx[ world_text_code ];
}


#if 0
//----------------------------------------------------------------------------
/**
 *	@brief	�����X�^�[�i���o�[���O����e�L�X�g�z��C���f�b�N�X�ɕύX
 *
 *	@param	monsno	�����X�^�[�i���o�[
 *
 *	@retval	�O����e�L�X�g�z��C���f�b�N�X
 *	@retval	ZKN_WORLD_TEXT_POKE_NUM�̂Ƃ��͎�����̃f�[�^�����Ȃ��|�P����
 */
//-----------------------------------------------------------------------------
int ZKN_WT_GetMonsNo_TEXTVERPokeNum( int monsno )
{
	int i;

	for( i=0; i<ZKN_WORLD_TEXT_POKE_NUM; i++ ){

		if( monsno == TEXTVER_PokeNum[i] ){
			break;
		}
	}
	
	return i;
}
#endif
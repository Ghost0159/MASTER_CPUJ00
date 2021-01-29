//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		zkn_data_work.c
 *	@brief		�}�Ӄf�[�^���[�N
 *	@author		tomoya takahashi
 *	@data		2006.07.13
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "system.h"
#include "assert.h"
#include <string.h>

#include "include/application/zukanlist/zkn_glbdata.h"

#define __ZKN_DATA_WORK_H_GLOBAL
#include "include/application/zukanlist/zkn_data_work.h"

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
//-------------------------------------
//	�}�ӕێ��f�[�^���[�N
//=====================================
typedef struct _ZKN_DATA_WORK{
	u32 list_monsno;	// ���X�g�̃����X�^�[�i���o�[
	u32 zkn_mode;	// ���X�g�̃����X�^�[�i���o�[
} ;



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�m��
 *
 *	@param	heapID	�q�[�v
 */
//-----------------------------------------------------------------------------
ZKN_DATA_WORK* ZKN_DW_Alloc( u32 heapID )
{
	ZKN_DATA_WORK* p_zkn;
	p_zkn = sys_AllocMemory( heapID, sizeof(ZKN_DATA_WORK) );
	memset( p_zkn, 0, sizeof(ZKN_DATA_WORK) );
	p_zkn->zkn_mode = ZKN_MODE_NAZO;
	return p_zkn;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�j��
 *
 *	@param	p_zkn	�}�Ӄ��[�N
 */
//-----------------------------------------------------------------------------
void ZKN_DW_Free( ZKN_DATA_WORK* p_zkn )
{
	sys_FreeMemoryEz( p_zkn );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ێ����Ă��郊�X�g�����X�^�[�i���o�[�擾
 *
 *	@param	cp_zkn	�}�Ӄ��[�N
 *
 *	@return	�����X�^�[�i���o�[
 */
//-----------------------------------------------------------------------------
u32 ZKN_DW_GetListMonsNo( const ZKN_DATA_WORK* cp_zkn )
{
	return cp_zkn->list_monsno;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ێ����郂���X�^�[�i���o�[���擾
 *
 *	@param	p_zkn	�}�Ӄ��[�N
 *	@param	monsno	�����X�^�[�i���o�[
 */
//-----------------------------------------------------------------------------
void ZKN_DW_SetListMonsNo( ZKN_DATA_WORK* p_zkn, u32 monsno )
{
	p_zkn->list_monsno = monsno;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}��Ӱ��
 *
 *	@param	cp_zkn	�}�Ӄ��[�N
 */
//-----------------------------------------------------------------------------
u32 ZKN_DW_GetZknMode( const ZKN_DATA_WORK* cp_zkn )
{
	return cp_zkn->zkn_mode;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}��Ӱ�ސݒ�
 *	
 *	@param	p_zkn		�}�Ӄ��[�N
 *	@param	zkn_mode 
	ZKN_MODE_SHINOH,	// �V���I�E�}��
	ZKN_MODE_ZENKOKU,	// �S���}��
	ZKN_MODE_NAZO,		// �s��	�i�܂�1����}�ӂ��J���Ă��Ȃ��Ƃ��j
 */
//-----------------------------------------------------------------------------
void ZKN_DW_SetZknMode( ZKN_DATA_WORK* p_zkn, u32 zkn_mode )
{
	p_zkn->zkn_mode = zkn_mode;
}
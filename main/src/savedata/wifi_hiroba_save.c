//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wifi_hiroba_save.c
 *	@brief		Wi-Fi�Ђ�΁@�Z�[�u�f�[�^
 *	@author		tomoya takahashi
 *	@data		2008.05.22
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "savedata/wifi_hiroba_save.h"

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
///	�Z�[�u�f�[�^
//=====================================
typedef struct _WFLBY_SAVEDATA {
	u32	question_number;
	u32	answer;
}WFLBY_SAVEDATA;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�T�C�Y�擾
 */
//-----------------------------------------------------------------------------
int SAVEDATA_WFLBY_GetWorkSize( void )
{
	return sizeof(WFLBY_SAVEDATA);
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�̏�����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void SAVEDATA_WFLBY_InitWork( WFLBY_SAVEDATA* p_wk )
{
	MI_CpuFill32( p_wk, 0, SAVEDATA_WFLBY_GetWorkSize() );

	p_wk->question_number = WFLBY_SAVEDATA_QUESTION_NONE;
	
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_WFHIROBA)
	SVLD_SetCrc(GMDATA_ID_WFHIROBA);
#endif //CRC_LOADCHECK
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N���������m��
 *
 *	@param	heapID	�q�[�v
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
WFLBY_SAVEDATA* SAVEDATA_WFLBY_AllocWork( u32 heapID )
{
	WFLBY_SAVEDATA* p_wk;

	p_wk = sys_AllocMemory( heapID, SAVEDATA_WFLBY_GetWorkSize() );

	SAVEDATA_WFLBY_InitWork( p_wk );

	return p_wk;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�Z�[�u�f�[�^���烏�[�N���擾
 *
 *	@param	p_sv 
 */
//-----------------------------------------------------------------------------
WFLBY_SAVEDATA* SAVEDATA_WFLBY_GetWork( SAVEDATA* p_sv )
{
	WFLBY_SAVEDATA* p_wk;

	p_wk = SaveData_Get( p_sv, GMDATA_ID_WFHIROBA );
	
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_WFHIROBA)
	SVLD_CheckCrc(GMDATA_ID_WFHIROBA);
#endif //CRC_LOADCHECK
	return p_wk;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�ʂ��i���o�[�擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�ʂ��i���o�[
 */
//-----------------------------------------------------------------------------
u32 SAVEDATA_WFLBY_GetQuestionNumber( const WFLBY_SAVEDATA* cp_wk )
{
	return cp_wk->question_number;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�񓚓��e�擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�񓚓��e
 */
//-----------------------------------------------------------------------------
u32 SAVEDATA_WFLBY_GetMyAnswer( const WFLBY_SAVEDATA* cp_wk )
{
	return cp_wk->answer;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�ʂ��i���o�[�̐ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	number		�A���P�[�g�ʂ��i���o�[
 */
//-----------------------------------------------------------------------------
void SAVEDATA_WFLBY_SetQuestionNumber( WFLBY_SAVEDATA* p_wk, u32 number )
{
	p_wk->question_number = number;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_WFHIROBA)
	SVLD_CheckCrc(GMDATA_ID_WFHIROBA);
#endif //CRC_LOADCHECK
}

//----------------------------------------------------------------------------
/**
 *	@brief	�񓚓��e��ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	answer	��
 */
//-----------------------------------------------------------------------------
void SAVEDATA_WFLBY_SetMyAnswer( WFLBY_SAVEDATA* p_wk, u32 answer )
{
	p_wk->answer = answer;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_WFHIROBA)
	SVLD_CheckCrc(GMDATA_ID_WFHIROBA);
#endif //CRC_LOADCHECK
}


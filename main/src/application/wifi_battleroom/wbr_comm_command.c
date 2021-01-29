//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_comm_command.c
 *	@brief		wifi	�o�g�����[���ʐM�R�}���h
 *	@author		tomoya takahashi
 *	@data		2007.02.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "communication/communication.h"
#include "wbr_comm_command_func.h"

#define __WBR_COMM_COMMAND_H_GLOBAL
#include "wbr_comm_command.h"

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
 *			�ʐM�f�[�^
 */
//-----------------------------------------------------------------------------
static const CommPacketTbl _CommWbrPacketTbl[] = {
	//-------------------------------------
	//�@�Q�[���p
	//=====================================
	// �����p	
	{ WBR_CNM_GameSysStart, WBR_CNM_ZeroSizeGet, NULL },		///< OYA->KO ��������
	{ WBR_CNM_GameSysGameStart, WBR_CNM_ZeroSizeGet, NULL },		///< OYA->KO �Q�[���J�n
	{ WBR_CNM_GameSysEnd,	WBR_CNM_ZeroSizeGet, NULL },		///< OYA->KO �I�� 	
	// �f�[�^��M�p
	{ WBR_CNM_KoCommonRecv,		WBR_CNM_KoCommonSizeGet, WBR_CNM_KoCommRecvBuffGet },		///< KO->OYA ���ʃf�[�^
	{ WBR_CNM_KoGameDataRecv,	WBR_CNM_KoGameDataSizeGet, NULL },		///< KO->OYA �Q�[���R�}���h
	{ WBR_CNM_OyaGameDataRecv,	WBR_CNM_OyaGameDataSizeGet, NULL },		///< OYA->KO �Q�[����ԃf�[�^
	{ WBR_CNM_KoOyaDataGetRecv,	WBR_CNM_ZeroSizeGet, NULL },		///< KO->OYA ���݂̃Q�[����ԋ����擾
	{ WBR_CNM_KoTalkDataRecv,	WBR_CNM_KoTalkDataSizeGet, NULL },		///< KO->OYA �b�������f�[�^
	{ WBR_CNM_KoTalkReqRecv,	WBR_CNM_KoTalkReqSizeGet, NULL },		///< KO->OYA �b�������f�[�^
};


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief		�R�}���h�e�[�u�����擾
 */
//-----------------------------------------------------------------------------
const CommPacketTbl* Wbr_CommCommandTclGet( void )
{
	return _CommWbrPacketTbl;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�}���h�e�[�u���v�f�����擾
 */
//-----------------------------------------------------------------------------
int Wbr_CommCommandTblNumGet( void )
{
    return sizeof(_CommWbrPacketTbl)/sizeof(CommPacketTbl);
}


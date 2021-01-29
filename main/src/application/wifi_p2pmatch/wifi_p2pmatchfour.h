//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_p2pmatchfour.h
 *	@brief		wifi�S�l�}�b�`���O��p���
 *	@author		tomoya takahashi
 *	@data		2007.05.22
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_P2PMATCHFOUR_H__
#define __WIFI_P2PMATCHFOUR_H__

#include "system/procsys.h"
#include "application/wifi_p2pmatch_def.h"

#undef GLOBAL
#ifdef	__WIFI_P2PMATCHFOUR_H_GLOBAL
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
///	�}�b�`���O�^�C�v
//=====================================
enum {
	WFP2PMF_TYPE_POFIN,				// �|�t�B��
	WFP2PMF_TYPE_BUCKET,			// ���܂���
	WFP2PMF_TYPE_BALANCE_BALL,		// ���܂̂�	
	WFP2PMF_TYPE_BALLOON,			// �ӂ�������
	WFP2PMF_TYPE_NUM,
} ;

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�}�b�`���O���e�@�\����
//=====================================
typedef struct {
	u8 type;		// �}�b�`���O�^�C�v
	u8 comm_min;	// �ʐM�Œ�l��
	u8 comm_max;	// �ʐM�ő�l��

	u8 result;		// ����ɏI�������̂��A�}�b�`���O��ʂɖ߂�̂�
	u8 vchat;		// VCHAT ON/OFF
	u8 pad[3];
	
	SAVEDATA*  p_savedata;

} WFP2PMF_INIT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
GLOBAL PROC_RESULT WifiP2PMatchFourProc_Init( PROC * proc, int * seq );
GLOBAL PROC_RESULT WifiP2PMatchFourProc_Main( PROC * proc, int * seq );
GLOBAL PROC_RESULT WifiP2PMatchFourProc_End( PROC * proc, int * seq );


#undef	GLOBAL
#endif		// __WIFI_P2PMATCHFOUR_H__


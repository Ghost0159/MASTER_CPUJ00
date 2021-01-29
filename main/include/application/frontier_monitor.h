//==============================================================================================
/**
 * @file	frontier_monitor.h
 * @brief	�u�o�g���t�@�N�g���[�v���j�^�[
 * @author	Satoshi Nohara
 * @date	2007.04.24
 */
//==============================================================================================
#ifndef _FRONTIER_MONITOR_H_
#define _FRONTIER_MONITOR_H_

#include "system/window.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================


//==============================================================================================
//
//	�\����
//
//==============================================================================================
typedef struct{
	SAVEDATA* sv;
	u8	type;							//�V���O���A�_�u���A�}���`�AWIFI
	u8	fr_no;							//�{�݃i���o�[
	u16 monsno;							//�����X�^�[�i���o�[
}FRONTIER_MONITOR_CALL;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
//extern const PROC_DATA FrontierProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT FrontierMonitorProc_Init( PROC * proc, int * seq );
extern PROC_RESULT FrontierMonitorProc_Main( PROC * proc, int * seq );
extern PROC_RESULT FrontierMonitorProc_End( PROC * proc, int * seq );


#endif //_FRONTIER_MONITOR_H_



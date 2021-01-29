//==============================================================================
/**
 * @file	footprint_main.h
 * @brief	���Ճ{�[�h�̃��C���w�b�_
 * @author	matsuda
 * @date	2008.01.19(�y)
 */
//==============================================================================
#ifndef __FOOTPRINT_MAIN_H__
#define __FOOTPRINT_MAIN_H__

#include "application/wifi_lobby/wflby_system_def.h"

//==============================================================================
//	�萔��`
//==============================================================================
///�{�[�h�̎��
enum{
	FOOTPRINT_BOARD_TYPE_WHITE,		///<���{�[�h
	FOOTPRINT_BOARD_TYPE_BLACK,		///<���{�[�h

	FOOTPRINT_BOARD_TYPE_MAX,
};


//==============================================================================
//	�\���̒�`
//==============================================================================
typedef struct{
	WFLBY_SYSTEM *wflby_sys;		///<WIFI���r�[�V�X�e���ւ̃|�C���^
	u8 board_type;					///<�{�[�h�̎��(FOOTPRINT_BOARD_TYPE_???)
	
	u8 padding[3];
	
#ifdef PM_DEBUG
	SAVEDATA *debug_sv;
#endif
}FOOTPRINT_PARAM;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern PROC_RESULT FootPrintProc_Init( PROC * proc, int * seq );
extern PROC_RESULT FootPrintProc_Main( PROC * proc, int * seq );
extern PROC_RESULT FootPrintProc_End( PROC * proc, int * seq );


#endif	//__FOOTPRINT_MAIN_H__


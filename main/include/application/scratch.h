//==============================================================================================
/**
 * @file	scratch.h
 * @brief	�u�X�N���b�`�v�Ăяo��
 * @author	Satoshi Nohara
 * @date	2007.12.11
 */
//==============================================================================================
#ifndef _SCRATCH_H_
#define _SCRATCH_H_

#include "system/window.h"
#include "../../src/application/scratch/scratch_sys.h"


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
	u16 dummy2;

	u16 item_no[SCRATCH_CARD_NUM];		//�A�C�e���i���o�[
	u16 item_num[SCRATCH_CARD_NUM];		//�A�C�e����

	u16 ret_work;						//�߂�l�i�[���[�N
	u16	dummy3;
}SCRATCH_CALL_WORK;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
//extern const PROC_DATA FrontierProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT ScratchProc_Init( PROC * proc, int * seq );
extern PROC_RESULT ScratchProc_Main( PROC * proc, int * seq );
extern PROC_RESULT ScratchProc_End( PROC * proc, int * seq );


#endif //_SCRATCH_H_



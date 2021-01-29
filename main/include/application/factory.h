//==============================================================================================
/**
 * @file	factory.h
 * @brief	�u�o�g���t�@�N�g���[�v�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.03.15
 */
//==============================================================================================
#ifndef _FACTORY_H_
#define _FACTORY_H_

#include "../../src/frontier/factory_def.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================


//==============================================================================================
//
//	�t�@�N�g���[��ʌĂяo���@�\���̐錾
//
//==============================================================================================
typedef struct{
	SAVEDATA* sv;

	u8	type; 
	u8	level;
	u8	mode; 
	u8	dummy;

	POKEPARTY* p_m_party;							//����
	POKEPARTY* p_e_party;							//�G
	u16 ret_work[FACTORY_RET_WORK_MAX];				//�߂�l�i�[���[�N
	void* p_work;									//FACTORY_SCRWORK(bf_scr_wk�ۑ�)

	u32 dummy_work;
}FACTORY_CALL_WORK;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
//extern const PROC_DATA FactoryProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT FactoryProc_Init( PROC * proc, int * seq );
extern PROC_RESULT FactoryProc_Main( PROC * proc, int * seq );
extern PROC_RESULT FactoryProc_End( PROC * proc, int * seq );


#endif //_FACTORY_H_



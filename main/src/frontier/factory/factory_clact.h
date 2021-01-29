//==============================================================================================
/**
 * @file	factory_clact.h
 * @brief	�u�o�g���t�@�N�g���[�v�Z���A�N�^�[
 * @author	Satoshi Nohara
 * @date	2007.11.15
 */
//==============================================================================================
#ifndef _FACTORY_CLACT_H_
#define _FACTORY_CLACT_H_

#include "common.h"
#include "system/clact_tool.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//CellActor�ɏ��������郊�\�[�X�}�l�[�W���̎�ނ̐��i���}���`�Z���E�}���`�Z���A�j���͎g�p���Ȃ��j
#define FACTORY_RESOURCE_NUM		(4)

enum{
	DISP_MAIN	= 0,		//����
	DISP_SUB	= 1,		//�����
};


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
typedef struct{
	CLACT_SET_PTR 			ClactSet;							//�Z���A�N�^�[�Z�b�g
	CLACT_U_EASYRENDER_DATA	RendData;							//�ȈՃ����_�[�f�[�^
	CLACT_U_RES_MANAGER_PTR	ResMan[FACTORY_RESOURCE_NUM];		//���\�[�X�}�l�[�W��
	//res�I�u�W�Ftbl(�㉺���)
	CLACT_U_RES_OBJ_PTR 	ResObjTbl[FACTORY_RES_OBJ_MAX][FACTORY_RESOURCE_NUM];
}FACTORY_CLACT;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
extern void					FactoryClact_InitCellActor( FACTORY_CLACT* wk );
extern CLACT_WORK_PTR FactoryClact_SetActor( FACTORY_CLACT* wk, u32 data_no, u32 anm_no, u32 pri, int bg_pri, u8 disp );
extern void					FactoryClact_DeleteCellObject( FACTORY_CLACT* wk );


#endif //_FACTORY_CLACT_H_



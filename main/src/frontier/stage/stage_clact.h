//==============================================================================================
/**
 * @file	stage_clact.h
 * @brief	�u�o�g���X�e�[�W�v�Z���A�N�^�[
 * @author	Satoshi Nohara
 * @date	07.06.08
 */
//==============================================================================================
#ifndef _STAGE_CLACT_H_
#define _STAGE_CLACT_H_

#include "common.h"
#include "poketool/poke_tool.h"
#include "system/clact_tool.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//CellActor�ɏ��������郊�\�[�X�}�l�[�W���̎�ނ̐��i���}���`�Z���E�}���`�Z���A�j���͎g�p���Ȃ��j
#define STAGE_RESOURCE_NUM		(4)

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
	CLACT_U_RES_MANAGER_PTR	ResMan[STAGE_RESOURCE_NUM];		//���\�[�X�}�l�[�W��
	//res�I�u�W�Ftbl(�㉺���)
	CLACT_U_RES_OBJ_PTR 	ResObjTbl[STAGE_RES_OBJ_MAX][STAGE_RESOURCE_NUM];
}STAGE_CLACT;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
//extern void					StageClact_InitCellActor( STAGE_CLACT* wk );
extern void					StageClact_InitCellActor( STAGE_CLACT* wk, POKEMON_PARAM* pp );
extern CLACT_WORK_PTR StageClact_SetActor( STAGE_CLACT* wk, u32 data_no, u32 anm_no, u32 pri, u8 disp );
extern void					StageClact_DeleteCellObject( STAGE_CLACT* wk );


#endif //_STAGE_CLACT_H_



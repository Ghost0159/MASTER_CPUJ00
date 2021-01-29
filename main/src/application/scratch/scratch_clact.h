//==============================================================================================
/**
 * @file	scratch_clact.h
 * @brief	�u�X�N���b�`�v�Z���A�N�^�[
 * @author	Satoshi Nohara
 * @date	07.12.11
 */
//==============================================================================================
#ifndef _SCRATCH_CLACT_H_
#define _SCRATCH_CLACT_H_

#include "common.h"
#include "poketool/poke_tool.h"
#include "system/clact_tool.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//CellActor�ɏ��������郊�\�[�X�}�l�[�W���̎�ނ̐��i���}���`�Z���E�}���`�Z���A�j���͎g�p���Ȃ��j
#define SCRATCH_RESOURCE_NUM		(4)

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
	CLACT_U_RES_MANAGER_PTR	ResMan[SCRATCH_RESOURCE_NUM];		//���\�[�X�}�l�[�W��
	//res�I�u�W�Ftbl(�㉺���)
	CLACT_U_RES_OBJ_PTR 	ResObjTbl[SCRATCH_RES_OBJ_MAX][SCRATCH_RESOURCE_NUM];
}SCRATCH_CLACT;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
extern void					ScratchClact_InitCellActor( SCRATCH_CLACT* wk );
extern CLACT_WORK_PTR ScratchClact_SetActor( SCRATCH_CLACT* wk, u32 data_no, u32 anm_no, u32 bg_pri, u32 pri, u8 disp );
extern void					ScratchClact_DeleteCellObject( SCRATCH_CLACT* wk );
extern void					ScratchClact_ResButtonChg( SCRATCH_CLACT* wk );
extern void					ScratchClact_ButtonResourcePalChg( SCRATCH_CLACT* wk, int no );


#endif //_SCRATCH_CLACT_H_



//==============================================================================================
/**
 * @file	castle_clact.h
 * @brief	�u�o�g���L���b�X���v�Z���A�N�^�[
 * @author	Satoshi Nohara
 * @date	07.07.05
 */
//==============================================================================================
#ifndef _CASTLE_CLACT_H_
#define _CASTLE_CLACT_H_

#include "common.h"
#include "poketool/poke_tool.h"
#include "system/clact_tool.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//CellActor�ɏ��������郊�\�[�X�}�l�[�W���̎�ނ̐��i���}���`�Z���E�}���`�Z���A�j���͎g�p���Ȃ��j
#define CASTLE_RESOURCE_NUM		(4)

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
	CLACT_U_RES_MANAGER_PTR	ResMan[CASTLE_RESOURCE_NUM];		//���\�[�X�}�l�[�W��
	CLACT_U_RES_OBJ_PTR 	ResObjTbl[RES_OBJ_MAX][CASTLE_RESOURCE_NUM];//res�I�u�W�Ftbl(�㉺���)
}CASTLE_CLACT;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
//extern void		CastleClact_InitCellActor( CASTLE_CLACT* wk );
extern void			CastleClact_InitCellActor( CASTLE_CLACT* wk, POKEPARTY* party, u8 type );
extern CLACT_WORK_PTR CastleClact_SetActor( CASTLE_CLACT* wk, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, u32 pri, int bg_pri, u8 disp );
extern void			CastleClact_DeleteCellObject( CASTLE_CLACT* wk );

//�A�C�e���A�C�R��
extern void			CastleClact_ItemIconCharChange( CASTLE_CLACT* wk, u16 item );
extern void			CastleClact_ItemIconPlttChange( CASTLE_CLACT* wk, u16 item );


#endif //_CASTLE_CLACT_H_



//==============================================================================================
/**
 * @file	roulette_clact.h
 * @brief	�u�o�g�����[���b�g�v�Z���A�N�^�[
 * @author	Satoshi Nohara
 * @date	07.09.05
 */
//==============================================================================================
#ifndef _ROULETTE_CLACT_H_
#define _ROULETTE_CLACT_H_

#include "common.h"
#include "poketool/poke_tool.h"
#include "system/clact_tool.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//CellActor�ɏ��������郊�\�[�X�}�l�[�W���̎�ނ̐��i���}���`�Z���E�}���`�Z���A�j���͎g�p���Ȃ��j
#define ROULETTE_RESOURCE_NUM		(4)

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
	CLACT_U_RES_MANAGER_PTR	ResMan[ROULETTE_RESOURCE_NUM];		//���\�[�X�}�l�[�W��
	CLACT_U_RES_OBJ_PTR 	ResObjTbl[ROULETTE_RES_OBJ_MAX][ROULETTE_RESOURCE_NUM];//res�I�u�W�Ftbl(�㉺���)
}ROULETTE_CLACT;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
//extern void		RouletteClact_InitCellActor( ROULETTE_CLACT* wk );
extern void			RouletteClact_InitCellActor( ROULETTE_CLACT* wk, POKEPARTY* m_party, POKEPARTY* e_party, u8 type );
extern CLACT_WORK_PTR RouletteClact_SetActor( ROULETTE_CLACT* wk, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, u32 pri, int bg_pri, u8 disp );
extern void			RouletteClact_DeleteCellObject( ROULETTE_CLACT* wk );

//�A�C�e���A�C�R��
extern void			RouletteClact_ItemIconCharChange( ROULETTE_CLACT* wk, u16 item );
extern void			RouletteClact_ItemIconPlttChange( ROULETTE_CLACT* wk, u16 item );


#endif //_ROULETTE_CLACT_H_



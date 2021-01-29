//==============================================================================================
/**
 * @file	stage_obj.h
 * @brief	�u�o�g���X�e�[�W�vOBJ(�J�[�\���A�A�C�R��)
 * @author	Satoshi Nohara
 * @date	07.06.08
 */
//==============================================================================================
#ifndef _STAGE_OBJ_H_
#define _STAGE_OBJ_H_

#include "system/clact_tool.h"
#include "stage_sys.h"


//==============================================================================================
//
//	�J�[�\��OBJ���[�N�ւ̕s���S�^�|�C���^
//
//==============================================================================================
typedef	struct _STAGE_OBJ STAGE_OBJ;


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�J�[�\���ړ��^�C�v
enum{
	OBJ_H_MODE = 0,
	OBJ_V_MODE = 1,
	OBJ_HV_MODE = 2,
};


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
extern STAGE_OBJ* StageObj_Create( STAGE_CLACT* stage_clact, u32 data_no, u32 obj_no, u16 x, u16 y, const u8* anm_tbl );
extern void* StageObj_Delete( STAGE_OBJ* wk );
extern void StageObj_Vanish( STAGE_OBJ* wk, int flag );
extern void StageObj_Pause( STAGE_OBJ* wk, int flag );
extern void StageObj_SetObjPos( STAGE_OBJ* wk, u16 x, u16 y );
extern void StageObj_AnmChg( STAGE_OBJ* wk, u32 num );
extern void StageObj_IconPalChg( STAGE_OBJ* wk, POKEMON_PARAM* pp );


#endif //_STAGE_OBJ_H_



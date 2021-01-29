//==============================================================================================
/**
 * @file	castle_obj.h
 * @brief	�u�o�g���L���b�X���vOBJ(�J�[�\���A�A�C�R��)
 * @author	Satoshi Nohara
 * @date	07.07.05
 */
//==============================================================================================
#ifndef _CASTLE_OBJ_H_
#define _CASTLE_OBJ_H_

#include "system/clact_tool.h"
#include "castle_sys.h"


//==============================================================================================
//
//	�J�[�\��OBJ���[�N�ւ̕s���S�^�|�C���^
//
//==============================================================================================
typedef	struct _CASTLE_OBJ CASTLE_OBJ;


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

//BG�v���C�I���e�B
enum{
	CASTLE_BG_PRI_BACK = 3,
	CASTLE_BG_PRI_LOW = 2,
	CASTLE_BG_PRI_NORMAL = 1,
	CASTLE_BG_PRI_HIGH = 0,
};


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
extern CASTLE_OBJ* CastleObj_Create( CASTLE_CLACT* castle_clact, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, s16 x, s16 y, int bg_pri, const u8* anm_tbl );
extern void* CastleObj_Delete( CASTLE_OBJ* wk );
extern void CastleObj_Vanish( CASTLE_OBJ* wk, int flag );
extern u16 CastleObj_GetObjX( CASTLE_OBJ* wk );
extern u16 CastleObj_GetObjY( CASTLE_OBJ* wk );
extern void CastleObj_SetObjPos( CASTLE_OBJ* wk, u16 x, u16 y );
extern void CastleObj_AnmChg( CASTLE_OBJ* wk, u32 num );
extern void CastleObj_IconPalChg( CASTLE_OBJ* wk, POKEMON_PARAM* pp );
extern u16 CastleObj_GetAnmFrame( CASTLE_OBJ* wk );
extern void CastleObj_SetInitPosOffset( CASTLE_OBJ* wk, int offset_x, int offset_y );
extern void CastleObj_PokeIconAnmChg( CASTLE_OBJ* wk, u8 anm );
extern void CastleObj_PokeIconPosSet( CASTLE_OBJ* wk, u8 flag );
extern BOOL CastleObj_AnmActiveCheck( CASTLE_OBJ* wk );
extern void CastleObj_PriorityChg( CASTLE_OBJ* wk, u8 pri );
extern void CastleObj_SetInitXY( CASTLE_OBJ* wk, s16 x, s16 y );
extern void CastleObj_RankUpEff( CASTLE_OBJ* wk, u16 x, u16 y );


#endif //_CASTLE_OBJ_H_



//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_3dmapobj_cont.h
 *	@brief		�}�b�v�I�u�W�F�z�u�Ǘ�
 *	@author		tomoya takahashi
 *	@data		2007.11.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_3DMAPOBJ_CONT_H__
#define __WFLBY_3DMAPOBJ_CONT_H__

#include "wflby_def.h"
#include "wflby_3dmapobj.h"
#include "wflby_mapcont.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�z�u�t���[�g�o�^�ő吔
//=====================================
#define WFLBY_3DMAPOBJ_CONT_MAPOBJFLOAT_MAX		( WFLBY_FLOAT_MAX )

//-------------------------------------
///	�t���[�g���ȍő吔
//=====================================
#define WFLBY_3DMAPOBJ_CONT_MAPOBJFLOAT_SHEET_MAX	( WFLBY_FLOAT_ON_NUM )

//-------------------------------------
///	�������v�A�j���萔
//=====================================
typedef enum {
	WFLBY_3DMAPOBJ_CONT_LAMP_NONE,	// �����o�Ă��Ȃ����
	WFLBY_3DMAPOBJ_CONT_LAMP_ON,	// NONE��������o����Ԃɂ���		���̌���A�j���ɂȂ�܂�
	WFLBY_3DMAPOBJ_CONT_LAMP_OFF,	// �����o�Ă����Ԃ�����������B	���̌�NONE��ԂɂȂ�܂��B
	WFLBY_3DMAPOBJ_CONT_LAMP_YURE,	// ���A�j��
} WFLBY_3DMAPOBJ_CONT_LAMP_ANM;



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�V�X�e���\����
//=====================================
typedef struct _WFLBY_3DMAPOBJ_CONT WFLBY_3DMAPOBJ_CONT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
extern WFLBY_3DMAPOBJ_CONT* WFLBY_3DMAPOBJCONT_Init( WFLBY_SEASON_TYPE season, WFLBY_ROOM_TYPE room, const WFLBY_MAPCONT* cp_map, u32 heapID, u32 gheapID );
extern void WFLBY_3DMAPOBJCONT_Exit( WFLBY_3DMAPOBJ_CONT* p_sys );
extern void WFLBY_3DMAPOBJCONT_Main( WFLBY_3DMAPOBJ_CONT* p_sys );
extern void WFLBY_3DMAPOBJCONT_Draw( WFLBY_3DMAPOBJ_CONT* p_sys );
extern void WFLBY_3DMAPOBJCONT_VBlank( WFLBY_3DMAPOBJ_CONT* p_sys );


// �p�\�R���A�t���[�g�Ȃǂɑ΂��鑀��֐���ǉ����Ă���

// �t���[�g
extern void WFLBY_3DMAPOBJCONT_FLOAT_GetPos( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx, VecFx32* p_vec );
extern void WFLBY_3DMAPOBJCONT_FLOAT_SetPos( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, fx32 x );
extern void WFLBY_3DMAPOBJCONT_FLOAT_SetOfsPos( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, const VecFx32* cp_vec );
extern void WFLBY_3DMAPOBJCONT_FLOAT_GetSheetPos( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx, u32 no, VecFx32* p_vec );
extern BOOL WFLBY_3DMAPOBJCONT_FLOAT_SetAnmSound( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx );
extern BOOL WFLBY_3DMAPOBJCONT_FLOAT_SetAnmBody( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx );
extern void WFLBY_3DMAPOBJCONT_FLOAT_SetDraw( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, BOOL flag );
extern BOOL WFLBY_3DMAPOBJCONT_FLOAT_GetDraw( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx );
extern void WFLBY_3DMAPOBJCONT_FLOAT_SetRot( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, u16 x );
extern BOOL WFLBY_3DMAPOBJCONT_FLOAT_CheckAnmSound( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx );
extern BOOL WFLBY_3DMAPOBJCONT_FLOAT_CheckAnmBody( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx );

// �}�b�v
extern void WFLBY_3DMAPOBJCONT_MAP_OnPoll( WFLBY_3DMAPOBJ_CONT* p_sys );
extern void WFLBY_3DMAPOBJCONT_MAP_OffPoll( WFLBY_3DMAPOBJ_CONT* p_sys );


// ����
extern void WFLBY_3DMAPOBJCONT_MAP_StartDouzouAnm( WFLBY_3DMAPOBJ_CONT* p_sys, u8 gridx, u8 gridy );
extern void WFLBY_3DMAPOBJCONT_MAP_SetDouzouFire( WFLBY_3DMAPOBJ_CONT* p_sys, BOOL flag );
extern void WFLBY_3DMAPOBJCONT_MAP_SetDouzouBigFire( WFLBY_3DMAPOBJ_CONT* p_sys, fx32 speed );
extern void WFLBY_3DMAPOBJCONT_MAP_OffDouzouLight( WFLBY_3DMAPOBJ_CONT* p_sys );

// �����C�g
extern void WFLBY_3DMAPOBJCONT_MAP_SetFloorLight( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_3DMAPOBJ_CONT_LAMP_ANM anm );
extern BOOL WFLBY_3DMAPOBJCONT_MAP_CheckFloorLightYure( const WFLBY_3DMAPOBJ_CONT* cp_sys );
extern void WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFire( WFLBY_3DMAPOBJ_CONT* p_sys, BOOL  flag, fx32 speed );
extern BOOL WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFireIdx( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, BOOL  flag,  fx32 speed );
extern void WFLBY_3DMAPOBJCONT_MAP_SetFloorLightBigFire( WFLBY_3DMAPOBJ_CONT* p_sys, fx32 speed );

// �~�j�Q�[���A�j��
extern void WFLBY_3DMAPOBJCONT_MAP_SetMGAnm( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_GAMETYPE game, u32 num, BOOL recruit, BOOL play, BOOL light );


#endif		// __WFLBY_3DMAPOBJ_CONT_H__


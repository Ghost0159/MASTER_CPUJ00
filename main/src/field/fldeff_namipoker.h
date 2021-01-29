//==============================================================================
/**
 * @file	fldeff_tw_namipoker.h
 * @brief	�t�B�[���hOBJ�g���|�P�����@�j�ꂽ���E
 * @author	kagaya
 * @data	05.07.13
 */
//==============================================================================
#ifndef FLDEFF_NAMIPOKER_H_FILE
#define FLDEFF_NAMIPOKER_H_FILE

#include "fieldobj.h"
#include "field_effect.h"
#include "player.h"

//==============================================================================
//	define
//==============================================================================

//==============================================================================
//	typedef
//==============================================================================
//--------------------------------------------------------------
///	�X�e�[�^�X�r�b�g 32bit
//--------------------------------------------------------------
typedef enum
{
	NPOKER_BIT_VANISH			= 1 << 0, ///<��\��
	NPOKER_BIT_JOINT			= 1 << 1, ///<�ڑ��L��
	NPOKER_BIT_ROTATE_OFF		= 1 << 2, ///<���]�I�t
	NPOKER_BIT_POS_OFF			= 1 << 3, ///<���W�␳�I�t
	NPOKER_BIT_ANGLE_REV_OFF	= 1 << 4, ///<���]�I�t
	NPOKER_BIT_SHAKE_OUTSIDE	= 1 << 5, ///<�O���w��h����̗p
	NPOKER_BIT_POKE_OFFS_OFF	= 1 << 6, ///<�g���|�P�����I�t�Z�b�g�I�t
}NPOKER_BIT;

//==============================================================================
//	�O���Q��
//==============================================================================
extern void * FE_NamiPokeR_Init( FE_SYS *fes );
extern void FE_NamiPokeR_Delete( void *work );

extern EOA_PTR FE_FldOBJNamiPokeRSet( PLAYER_STATE_PTR jiki,
	int gx, int gy, int gz, int dir, int joint, HEROTWTYPE tw );

extern ROTATE * FE_FldOBJNamiPokeR_RotatePtrGet( EOA_PTR eoa );
extern void FE_FldOBJNamiPokeR_RotateDirInit(
		EOA_PTR eoa, int dir, HEROTWTYPE tw_type );
extern void FE_FldOBJNamiPokeR_StatusBitON( EOA_PTR eoa, NPOKER_BIT bit );
extern void FE_FldOBJNamiPokeR_StatusBitOFF( EOA_PTR eoa, NPOKER_BIT bit );
extern VecFx32 * FE_FldOBJNamiPokeR_ShakeOffsetPtrGet( EOA_PTR eoa );
extern VecFx32 * FE_FldOBJNamiPokeR_PokeOffsetPtrGet( EOA_PTR eoa );
extern void FE_FldOBJNamiPokeR_AngleRevSet( EOA_PTR eoa, u16 angle );

#endif //FLDEFF_NAMIPOKER_H_FILE

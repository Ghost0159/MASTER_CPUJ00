//==============================================================================
/**
 * @file	fldeff_kemuri.h
 * @brief	�t�B�[���hOBJ�y��
 * @author	kagaya
 * @data	05.07.13
 */
//==============================================================================
#ifndef FLDEFF_KEMURI_H_FILE
#define FLDEFF_KEMURI_H_FILE

#include "eoa.h"
#include "fieldobj.h"
#include "field_effect.h"

//==============================================================================
//	typedef
//==============================================================================
//--------------------------------------------------------------
///	������
//--------------------------------------------------------------
typedef enum
{
	FE_KEMURI_TYPE_FLAT = 0,	//����
	FE_KEMURI_TYPE_LEFTG,		//���n��
	FE_KEMURI_TYPE_RIGHTG,		//�E�n��
	FE_KEMURI_TYPE_ROOFG,		//�V���
	FE_KEMURI_TYPE_MAX,
}FE_KEMURI_TYPE;

//==============================================================================
//	�O���Q��
//==============================================================================
extern void * FE_Kemuri_Init( FE_SYS *fes );
extern void FE_Kemuri_Delete( void *work );
extern void FE_Kemuri_TornWorldON( FE_SYS *fes );

extern void FE_FldOBJKemuri_Add( FIELD_OBJ_PTR fldobj );
extern void FE_FldOBJKemuriTornWorld_Add( FIELD_OBJ_PTR fldobj, FE_KEMURI_TYPE type );

#endif //FLDEFF_KEMURI_H_FILE

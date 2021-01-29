//******************************************************************************
/**
 * 
 * @file	fldeff_ripple.c
 * @brief	�t�B�[���hOBJ�@���g��
 * @author	kagaya
 * @data	05.07.13
 *
 */
//******************************************************************************
#include "common.h"
#include "fieldsys.h"
#include "field_effect.h"
#include "fieldobj.h"
#include "fldeff_ripple.h"

//==============================================================================
//	define
//==============================================================================
//#define DEBUG_RIPPLE_WRITE_OFF	//��`�Ő��g��`��OFF

#define RIPPLE_DRAW_Y_OFFSET (-FX32_ONE*(8))		///<���g��`��I�t�Z�b�gY��
#define RIPPLE_DRAW_Z_OFFSET (FX32_ONE*(1))

#define RIPPLE_ON_FRAME (30)
#define RIPPLE_OFF_FRAME (RIPPLE_ON_FRAME+30)

//==============================================================================
//	typedef struct
//==============================================================================
//--------------------------------------------------------------
///	FE_RIPPLE�^
//--------------------------------------------------------------
typedef struct _TAG_FE_RIPPLE * FE_RIPPLE_PTR;

//--------------------------------------------------------------
///	FE_RIPPLE�\����
//--------------------------------------------------------------
typedef struct _TAG_FE_RIPPLE
{
	FE_SYS *fes;
	FRO_MDL m_rmdl;
	FRO_ANM m_ranm;
}FE_RIPPLE;

#define FE_RIPPLE_SIZE (sizeof(FE_RIPPLE)) ///<FE_RIPPLE�T�C�Y

//--------------------------------------------------------------
///	RIPPLE_ADD_H�\����
//--------------------------------------------------------------
typedef struct
{
	FE_SYS *fes;					///<FE_SYS_PTR
	FE_RIPPLE_PTR ripple;		///<FE_RIPPLE_PTR
}RIPPLE_ADD_H;

#define RIPPLE_ADD_H_SIZE (sizeof(RIPPLE_ADD_H)) ///<RIPPLE_ADD_H�T�C�Y

//--------------------------------------------------------------
///	RIPPLE_WORK�\����
//--------------------------------------------------------------
typedef struct
{
	FRO_ANM ranm;
	FRO_OBJ robj;
	RIPPLE_ADD_H head;						///<�ǉ�����RIPPLE_ADD_H
}RIPPLE_WORK;

#define RIPPLE_WORK_SIZE (sizeof(RIPPLE_WORK))	///<RIPPLE_WORK�T�C�Y

//==============================================================================
//	�v���g�^�C�v
//==============================================================================
static void Ripple_GraphicInit( FE_RIPPLE_PTR ripple );
static void Ripple_GraphicDelete( FE_RIPPLE_PTR ripple );

static const EOA_H_NPP DATA_EoaH_Ripple;

//==============================================================================
//	���g��@�V�X�e��
//==============================================================================
//--------------------------------------------------------------
/**
 * ���g�䏉����
 * @param	fes		FE_SYS_PTR
 * @retval	FE_RIPPLE_PTR	FE_RIPPLE_PTR
 */
//--------------------------------------------------------------
void * FE_Ripple_Init( FE_SYS *fes )
{
	FE_RIPPLE_PTR ripple;
	
	ripple = FE_AllocClearMemory( fes, FE_RIPPLE_SIZE, ALLOC_FR, 0 );
	ripple->fes = fes;
	
	Ripple_GraphicInit( ripple );
	return( ripple );
}

//--------------------------------------------------------------
/**
 * ���g��폜
 * @param	ripple		FE_RIPPLE_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_Ripple_Delete( void *work )
{
	FE_RIPPLE_PTR ripple = work;
	Ripple_GraphicDelete( ripple );
	FE_FreeMemory( ripple );
}

//==============================================================================
//	���g��@�O���t�B�b�N
//==============================================================================
//--------------------------------------------------------------
/**
 * ���g�� �O���t�B�b�N������
 * @param	ripple	FE_RIPPLE_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
static void Ripple_GraphicInit( FE_RIPPLE_PTR rip )
{
	FE_FROMdl_ResSetArcLoadTexTrans(
		rip->fes, &rip->m_rmdl, 0, NARC_fldeff_ripple_nsbmd, ALLOC_FR );
	//���C�����\�[�X�̂݃��[�h
	FE_FROAnm_AnmResSetArcLoad(
		rip->fes, &rip->m_ranm, 0, NARC_fldeff_ripple_nsbtp, ALLOC_FR );
}

//--------------------------------------------------------------
/**
 * ���g�� �O���t�B�b�N�폜
 * @param	ripple	FE_RIPPLE_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
static void Ripple_GraphicDelete( FE_RIPPLE_PTR rip )
{
	FRO_MDL_DeleteAll( &rip->m_rmdl );
	FRO_ANM_DeleteAll( &rip->m_ranm );
}

//==============================================================================
//	���g��@EOA
//==============================================================================
//--------------------------------------------------------------
/**
 * ���g��ǉ�
 * @param	fldobj	�ΏۂƂȂ�FIELD_OBJ_PTR
 * @param	gx		�\������O���b�hX���W
 * @param	gz		�\������O���b�hZ���W
 * @retval	EOA_PTR	���g��EOA_PTR
 */
//--------------------------------------------------------------
void FE_FldOBJRippleSet( FIELD_OBJ_PTR fldobj, int gx, int gy, int gz )
{
	int pri,param;
	RIPPLE_ADD_H head;
	VecFx32 mtx;
	
	head.fes = FE_FieldOBJ_FE_SYS_Get( fldobj );
	head.ripple = FE_EffectWorkGet( head.fes, FE_FLD_RIPPLE );
	
	FieldOBJTool_GridCenterPosGet( gx, gz, &mtx );
	mtx.z += H_GRID_SIZE( gy ) + RIPPLE_DRAW_Z_OFFSET;
	
	mtx.y = H_GRID_SIZE_FX32( FieldOBJ_OldPosGY_Get(fldobj) ) + RIPPLE_DRAW_Y_OFFSET;
	
	param = 0;
	pri = FieldOBJ_TCBPriGet( fldobj, FLDOBJ_TCBPRI_OFFS_AFTER );
	FE_EoaAddNpp( head.fes, &DATA_EoaH_Ripple, &mtx, param, &head,pri );
}

//--------------------------------------------------------------
/**
 * EOA ���g��@������
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	int		TRUE=����I���BFALSE=�ُ�I��
 */
//--------------------------------------------------------------
static int EoaRipple_Init( EOA_PTR eoa, void *wk )
{
	RIPPLE_WORK *work = wk;
	const RIPPLE_ADD_H *head = EOA_AddPtrGet( eoa );
	
	work->head = *head;
	FE_FROAnm_AnmResObjInitSame( work->head.fes, &work->ranm,
			&work->head.ripple->m_rmdl, &work->head.ripple->m_ranm, 0 );
	FRO_OBJ_InitAll( &work->robj, &work->head.ripple->m_rmdl, &work->ranm );
	return( TRUE );
}

//--------------------------------------------------------------
/**
 * EOA ���g��@�폜
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaRipple_Delete( EOA_PTR eoa, void *wk )
{
	RIPPLE_WORK *work = wk;
	FRO_ANM_DeleteAll( &work->ranm );
}

//--------------------------------------------------------------
/**
 * EOA ���g��@����
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaRipple_Move( EOA_PTR eoa, void *wk )
{
	RIPPLE_WORK *work = wk;
	
	if( FRO_ANM_AnmEndCheck(&work->ranm) == TRUE ){
		FE_EoaDelete( eoa );
		return;
	}
	
	FRO_ANM_Play( &work->ranm, FX32_ONE, ANMLOOP_OFF );
}

//--------------------------------------------------------------
/**
 * EOA ���g��@�`��
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaRipple_Draw( EOA_PTR eoa, void *wk )
{
	VecFx32 pos;
	RIPPLE_WORK *work = wk;
	EOA_MatrixGet( eoa, &pos );
	FRO_OBJ_DrawPos( &work->robj, &pos );
}

//==============================================================================
//	data
//==============================================================================
//--------------------------------------------------------------
///	���g��EOA_H
//--------------------------------------------------------------
static const EOA_H_NPP DATA_EoaH_Ripple =
{
	RIPPLE_WORK_SIZE,
	EoaRipple_Init,
	EoaRipple_Delete,
	EoaRipple_Move,
	EoaRipple_Draw,
};


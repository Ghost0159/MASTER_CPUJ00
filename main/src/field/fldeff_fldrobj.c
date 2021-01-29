//******************************************************************************
/**
 * @file	fldeff_fldrobj.c
 * @brief	�t�B�[���h�G�t�F�N�g�@�t�B�[���h�����_�����OOBJ�\��
 * @author	kagaya
 * @data	05.07.13
 */
//******************************************************************************
#include "common.h"
#include "fieldsys.h"
#include "field_effect.h"
#include "fieldobj.h"

#include "fldeff_fldrobj.h"

//==============================================================================
//	define
//==============================================================================

//==============================================================================
//	typedef struct
//==============================================================================
//--------------------------------------------------------------
///	FRO_CODE
//--------------------------------------------------------------
typedef struct
{
	u32 code;
	FRO_MDL rmdl;
	FRO_OBJ robj;
}MDLDATA;

//--------------------------------------------------------------
///	FE_FLDROBJ
//--------------------------------------------------------------
typedef struct
{
	FE_SYS *fes;			///<FE_SYS
	u32 obj_max;			///<OBJ�ő吔
	MDLDATA *mdldata;		///<�R�[�h�ʃ����_�i�[
}FE_FLDROBJ;

//--------------------------------------------------------------
///	FLDROBJ_H
//--------------------------------------------------------------
typedef struct
{
	u32 code;
	FE_SYS *fes;
	CONST_FIELD_OBJ_PTR fldobj;
}FLDROBJ_H;

//--------------------------------------------------------------
///	EOA_FLDROBJ_WORK
//--------------------------------------------------------------
typedef struct
{
	int seq_no;
	int obj_id;
	int zone_id;
	VecFx32 offs;
	FRO_OBJ *robj;
	FLDROBJ_H head;
}EOA_FLDROBJ_WORK;

//--------------------------------------------------------------
///	CODEARCIDX
//--------------------------------------------------------------
typedef struct
{
	u32 code;
	u32 arc_idx;
}CODEARCIDX;

//==============================================================================
//	�v���g�^�C�v
//==============================================================================
static void MdlDataInit( MDLDATA *mdat, u32 code, u32 arc_idx, FE_SYS *fes );
static void MdlDataDelete( MDLDATA *mdat );
static void MdlDataDeleteAll( FE_FLDROBJ *frobj );
static MDLDATA * MdlDataSearch( FE_FLDROBJ *frobj, u32 code );

static const EOA_H_NPP DATA_EoaH_FldRObj;

const CODEARCIDX DATA_FieldOBJRenderOBJArcIdx[];

//==============================================================================
//	data
//==============================================================================
//--------------------------------------------------------------
///	�����_OBJ�g�p�\���R�[�h
//--------------------------------------------------------------
static const CODEARCIDX DATA_FieldOBJRenderOBJArcIdx[] =
{
	{ BOARD_A, NARC_fldeff_board_a_nsbmd },
	{ BOARD_B, NARC_fldeff_board_b_nsbmd },
	{ BOARD_C, NARC_fldeff_board_c_nsbmd },
	{ BOARD_D, NARC_fldeff_board_d_nsbmd },
	{ BOARD_E, NARC_fldeff_board_e_nsbmd },
	{ BOARD_F, NARC_fldeff_board_f_nsbmd },
	{ BOOK, NARC_fldeff_book_nsbmd },
	{ DOOR2, NARC_fldeff_door2_nsbmd },
	{ ROTOMWALL, NARC_fldeff_rotomwall_nsbmd },
};

//==============================================================================
//	FE_FLDROBJ�@�V�X�e��
//==============================================================================
//--------------------------------------------------------------
/**
 * �t�B�[���h�����_�����OOBJ������
 * @param	fes		FE_SYS_PTR
 * @retval	FE_FLDROBJ
 */
//--------------------------------------------------------------
void * FE_FldROBJ_Init( FE_SYS *fes )
{
	FE_FLDROBJ *frobj;
	frobj = FE_AllocClearMemory( fes, sizeof(FE_FLDROBJ), ALLOC_FR, 0 );
	frobj->fes = fes;
	return( frobj );
}

//--------------------------------------------------------------
/**
 * �t�B�[���h�����_�����OOBJ�폜
 * @param	work	FE_FLDROBJ
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_FldROBJ_Delete( void *work )
{
	FE_FLDROBJ *frobj = work;
	MdlDataDeleteAll( frobj );
	if( frobj->mdldata != NULL ){ FE_FreeMemory(frobj->mdldata); }
	FE_FreeMemory( frobj );
}

//==============================================================================
//	�����_OBJ�@�O���t�B�b�N
//==============================================================================
//--------------------------------------------------------------
/**
 * ���f���Z�b�g
 * @param	mdat	MDLDATA
 * @param	arc_idx	�A�[�J�C�u�C���f�b�N�X
 * @retval
 */
//--------------------------------------------------------------
static void MdlDataInit( MDLDATA *mdat, u32 code, u32 arc_idx, FE_SYS *fes )
{
	mdat->code = code;
	FE_FROMdl_ResSetArcLoadTexTrans( fes, &mdat->rmdl, 0, arc_idx, ALLOC_FR );
	FRO_OBJ_InitInMdl( &mdat->robj, &mdat->rmdl );
}

//--------------------------------------------------------------
/**
 * ���f���폜
 * @param	mdat	MDLDATA
 * @retval	nothing
 */
//--------------------------------------------------------------
static void MdlDataDelete( MDLDATA *mdat )
{
	mdat->code = OBJCODEMAX;
	FRO_MDL_DeleteAll( &mdat->rmdl );
}

//--------------------------------------------------------------
/**
 * ���f���S�폜
 * @param	frobj	FE_FLDROBJ
 * @retval	nothing
 */
//--------------------------------------------------------------
static void MdlDataDeleteAll( FE_FLDROBJ *frobj )
{
	u32 i = 0, max = frobj->obj_max;
	MDLDATA *mdata = frobj->mdldata;
	while(i<max){if(mdata->code!=OBJCODEMAX){MdlDataDelete(mdata);}mdata++;i++;}
}

//--------------------------------------------------------------
/**
 * MDLDATA����
 * @param	frobj		FE_FLDROBJ
 * @param	code		�����R�[�h
 * @retval	MDLDATA		NULL=��v�Ȃ�	
 */
//--------------------------------------------------------------
static MDLDATA * MdlDataSearch( FE_FLDROBJ *frobj, u32 code )
{
	u32 max = frobj->obj_max;
	MDLDATA *mdata = frobj->mdldata;
	do{ if(mdata->code==code){ return(mdata); } mdata++; max--; }while( max );
	return( NULL );
}

//--------------------------------------------------------------
/**
 * ���f���f�[�^�̏�����
 * @paraum	
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_FldROBJ_MdlWorkInit( FE_SYS *fes, u32 max )
{
	MDLDATA *mdata;
	FE_FLDROBJ *frobj = FE_EffectWorkGet( fes, FE_FLD_FLDROBJ );
	GF_ASSERT( max );
	frobj->obj_max = max;
	mdata = FE_AllocClearMemory( fes, sizeof(MDLDATA)*max, ALLOC_FR, 0 );
	frobj->mdldata = mdata;
	do{ mdata->code = OBJCODEMAX; mdata++; max--; }while( max );
}

//--------------------------------------------------------------
/**
 * ���f���f�[�^�̓o�^
 * @param	fes		FE_SYS
 * @param	code	�o�^�R�[�h
 * @param	arc_idx	�A�[�J�C�u�C���f�b�N�X
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_FldROBJ_MdlAdd( FE_SYS *fes, u32 code, u32 arc_idx )
{
	MDLDATA *mdata;
	FE_FLDROBJ *frobj = FE_EffectWorkGet( fes, FE_FLD_FLDROBJ );
	mdata = MdlDataSearch( frobj, code );
	if( mdata != NULL ){ return; }
	mdata = MdlDataSearch( frobj, OBJCODEMAX );
	if( mdata == NULL ){ GF_ASSERT(0); return; }
	MdlDataInit( mdata, code, arc_idx, fes );
}

//--------------------------------------------------------------
/**
 * �R�[�h����FRO_MDL�擾
 * @param	frobj	FE_FLDROBJ
 * @param	code	�\���R�[�h
 * @retval	FRO_MDL
 */
//--------------------------------------------------------------
FRO_MDL * FE_FldROBJ_FROMdlSearch( FE_SYS *fes, u32 code )
{
	FE_FLDROBJ *frobj = FE_EffectWorkGet( fes, FE_FLD_FLDROBJ );
	MDLDATA *mdata = MdlDataSearch( frobj, code );
	GF_ASSERT( mdata != NULL );
	return( &mdata->rmdl );
}

//--------------------------------------------------------------
/**
 * �R�[�h����FRO_OBJ�擾
 * @param	frobj	FE_FLDROBJ
 * @param	code	�\���R�[�h
 * @retval	FRO_OBJ
 */
//--------------------------------------------------------------
FRO_OBJ * FE_FldROBJ_FROObjSearch( FE_SYS *fes, u32 code )
{
	FE_FLDROBJ *frobj = FE_EffectWorkGet( fes, FE_FLD_FLDROBJ );
	MDLDATA *mdata = MdlDataSearch( frobj, code );
	GF_ASSERT( mdata != NULL );
	return( &mdata->robj );
}

//==============================================================================
//	�t�B�[���hOBJ �����_�\���@EOA
//==============================================================================
//--------------------------------------------------------------
/**
 * �t�B�[���hOBJ�����_�ǉ�
 * @param	fldobj	FIELD_OBJ_PTR
 * @param	offs	�\���I�t�Z�b�g
 * @retval	eoa		EOA_PTR
 */
//--------------------------------------------------------------
EOA_PTR FE_FldROBJ_Add( CONST_FIELD_OBJ_PTR fldobj, const VecFx32 *offs )
{
	int pri;
	EOA_PTR eoa;
	FLDROBJ_H head;
	FE_SYS *fes = FE_FieldOBJ_FE_SYS_Get( fldobj );
	
	head.code = FieldOBJ_OBJCodeGet( fldobj );
	head.fes = fes;
	head.fldobj = fldobj;
	
	pri = FieldOBJ_TCBPriGet( fldobj, FLDOBJ_TCBPRI_OFFS_AFTER );
	eoa = FE_EoaAddNpp( fes, &DATA_EoaH_FldRObj, offs, 0, &head, pri );
	return( eoa );
}

//--------------------------------------------------------------
/**
 * EOA �t�B�[���hOBJ�����_�\���@������
 * @param	eoa	EOA_PTR
 * @param	wk	eoa work *
 * @retval	int TRUE=����I���BFALSE=�ُ�I��
 */
//--------------------------------------------------------------
static int EoaFldRObj_Init( EOA_PTR eoa, void *wk )
{
	EOA_FLDROBJ_WORK *work = wk;
	const FLDROBJ_H *head;
	
	head = EOA_AddPtrGet( eoa );
	work->head = *head;
	work->obj_id = FieldOBJ_OBJIDGet( work->head.fldobj );
	work->robj = FE_FldROBJ_FROObjSearch( work->head.fes, work->head.code );
	
	if( FieldOBJ_StatusBitCheck_Alies(work->head.fldobj) == TRUE ){
		work->zone_id = FieldOBJ_ZoneIDGetAlies( work->head.fldobj );
	}else{
		work->zone_id = FieldOBJ_ZoneIDGet( work->head.fldobj );
	}
	
	EOA_MatrixGet( eoa, &work->offs );
	return( TRUE );
}

//--------------------------------------------------------------
/**
 * EOA �t�B�[���hOBJ�����_�\���@�폜
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaFldRObj_Delete( EOA_PTR eoa, void *wk )
{
}

//--------------------------------------------------------------
/**
 * EOA �t�B�[���hOBJ�����_�\���@����
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaFldRObj_Move( EOA_PTR eoa, void *wk )
{
	EOA_FLDROBJ_WORK *work = wk;
	CONST_FIELD_OBJ_PTR fldobj = work->head.fldobj;
	
	if( FieldOBJ_CheckSameID(fldobj,work->obj_id,work->zone_id) == FALSE ){
		FE_EoaDelete( eoa );
		return;
	}
	
	{
		VecFx32 mtx,d_mtx;
		FieldOBJ_VecPosGet( fldobj, &mtx );
		FieldOBJ_VecDrawOffsGet( fldobj, &d_mtx );
		mtx.x += d_mtx.x + work->offs.x;
		mtx.y += d_mtx.y + work->offs.y;
		mtx.z += d_mtx.z + work->offs.z;
		EOA_MatrixSet( eoa, &mtx );
	}
}

//--------------------------------------------------------------
/**
 * EOA �t�B�[���hOBJ�����_�\���@�`��
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaFldRObj_Draw( EOA_PTR eoa, void *wk )
{
	EOA_FLDROBJ_WORK *work = wk;
	VecFx32 pos;
	EOA_MatrixGet( eoa, &pos );
	FRO_OBJ_DrawPos( work->robj, &pos );
}

//--------------------------------------------------------------
///	EOA_H
//--------------------------------------------------------------
static const EOA_H_NPP DATA_EoaH_FldRObj =
{
	sizeof(EOA_FLDROBJ_WORK),
	EoaFldRObj_Init,
	EoaFldRObj_Delete,
	EoaFldRObj_Move,
	EoaFldRObj_Draw,
};

//==============================================================================
//	�t�B�[���hOBJ�@�t�B�[���h�G�t�F�N�g�����_�����OOBJ�\��
//==============================================================================
//--------------------------------------------------------------
/**
 * �t�B�[���hOBJ �t�B�[���h�G�t�F�N�g�g�p�����_�����OOBJ������
 * @param	fes		FE_SYS
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_FieldRenderOBJInit( FE_SYS *fes )
{
	const CODEARCIDX *idx = DATA_FieldOBJRenderOBJArcIdx;
	u32 max = NELEMS( DATA_FieldOBJRenderOBJArcIdx );
	FE_FldROBJ_MdlWorkInit( fes, max );
	OS_Printf( "field effect robj max = %d\n", max );
	
	do{
		FE_FldROBJ_MdlAdd( fes, idx->code, idx->arc_idx );
		idx++; max--;
	}while( max );
}


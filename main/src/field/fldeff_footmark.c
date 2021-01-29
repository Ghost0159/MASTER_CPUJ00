//******************************************************************************
/**
 * 
 * @file	fldeff_footmark.c
 * @brief	�t�B�[���hOBJ����
 * @author	kagaya
 * @data	05.07.13
 *
 */
//******************************************************************************
#include "common.h"
#include "fieldsys.h"
#include "field_effect.h"
#include "fieldobj.h"
#include "fldeff_footmark.h"

//==============================================================================
//	define
//==============================================================================
#define FOOTMARK_SNOWCYCLE_OFF	///<��`�ŉe��̎��]�ԑ��Ղ𖳌���

#define FOOTMARK_VANISH_START_FRAME (16)			///<�_�ŊJ�n�t���[��(�ԗ�40)
#define FOOTMARK_VANISH_END_FRAME (28)				///<�_�ŏI���t���[��(�ԗ�56)

//--------------------------------------------------------------
///	���Վ���
//--------------------------------------------------------------
enum
{
	FOOTMARK_WALK_UP = 0,							///<���Ձ@��
	FOOTMARK_WALK_DOWN,								///<���Ձ@��
	FOOTMARK_WALK_LEFT,								///<���Ձ@��
	FOOTMARK_WALK_RIGHT,							///<���Ձ@�E
	FOOTMARK_CYCLE_UD,								///<���]�ԑ��� �㉺
	FOOTMARK_CYCLE_LR,								///<���]�ԑ��� ���E
	FOOTMARK_CYCLE_UL,								///<���]�ԑ��� �㍶
	FOOTMARK_CYCLE_UR,								///<���]�ԑ��� ��E
	FOOTMARK_CYCLE_DL,								///<���]�ԑ��� ����
	FOOTMARK_CYCLE_DR,								///<���]�ԑ��� ���E
	FOOTMARK_MAX,									///<���Սő吔
};

#define FOOTMARK_CYCLE_START (FOOTMARK_CYCLE_UD)
#define FOOTMARK_MAX_CYCLE (FOOTMARK_MAX-FOOTMARK_CYCLE_START)

//--------------------------------------------------------------
///	���Ճ^�C�v	�����_OBJ�̎�ނƕ\���ʒu�ɉe������
//--------------------------------------------------------------
typedef enum
{
	FOOTMARKTYPE_NORMAL = 0,		///<���Ճ^�C�v�@�Q��
	FOOTMARKTYPE_CYCLE,				///<���Ճ^�C�v�@���]��
	FOOTMARKTYPE_SNOW,				///<���Ճ^�C�v�@��
	FOOTMARKTYPE_SNOWDEEP,			///<���Ճ^�C�v�@�[����
	FOOTMARKTYPE_SHADOWSNOW,		///<���Ճ^�C�v�@�e�\����@��
	FOOTMARKTYPE_SHADOWSNOW_CYCLE,	///<���Ճ^�C�v�@�e�\����@���]��
}FOOTMARKTYPE;

//==============================================================================
//	typedef struct
//==============================================================================
//--------------------------------------------------------------
///	FE_FOOTMARK�^
//--------------------------------------------------------------
typedef struct _TAG_FE_FOOTMARK * FE_FOOTMARK_PTR;

//--------------------------------------------------------------
///	FE_FOOTMARK�\����
//--------------------------------------------------------------
typedef struct _TAG_FE_FOOTMARK
{
	FE_SYS *fes;
	FRO_MDL rmdl_gnd[FOOTMARK_MAX];
	FRO_MDL rmdl_snow[FOOTMARK_MAX];
	#ifndef FOOTMARK_SNOWCYCLE_OFF
	FRO_MDL rmdl_ssnow_c[FOOTMARK_MAX];
	#endif
	
	FRO_OBJ robj_gnd[FOOTMARK_MAX];
	FRO_OBJ robj_snow[FOOTMARK_MAX];
	#ifndef FOOTMARK_SNOWCYCLE_OFF
	FRO_OBJ robj_ssnow_c[FOOTMARK_MAX_CYCLE];
	#endif
}FE_FOOTMARK;

#define FE_FOOTMARK_SIZE (sizeof(FE_FOOTMARK)) ///<FE_FOOTMARK�T�C�Y

//--------------------------------------------------------------
///	FOOTMARK_ADD_H�\����
//--------------------------------------------------------------
typedef struct
{
	FOOTMARKTYPE type;
	FRO_MDL *rmdl;
	FRO_OBJ *robj;
}FOOTMARK_ADD_H;

//--------------------------------------------------------------
///	FOOTMARK_WORK�\����
//--------------------------------------------------------------
typedef struct
{
	int seq_no;
	int frame;
	int vanish_sw;
	int alpha;
	FOOTMARKTYPE type;
	FRO_MDL *rmdl;
	FRO_OBJ *robj;
}FOOTMARK_WORK;

#define FOOTMARK_WORK_SIZE (sizeof(FOOTMARK_WORK))

//==============================================================================
//	�v���g�^�C�v
//==============================================================================
static void FootMark_GraphicInit( FE_FOOTMARK_PTR fm );
static void FootMark_GraphicDelete( FE_FOOTMARK_PTR fm );

static FRO_OBJ * FootMark_TypeObjGet(
		FE_FOOTMARK_PTR fm, FOOTMARKTYPE type, int now_dir, int old_dir );
static FRO_MDL * FootMark_TypeMdlGet(
		FE_FOOTMARK_PTR fm, FOOTMARKTYPE type, int now_dir, int old_dir );

static EOA_PTR FldOBJFootMark_AddMain( CONST_FIELD_OBJ_PTR fldobj, FOOTMARKTYPE type );

static const u32 DATA_3DModelArcID_FootMark[FOOTMARK_MAX];
static const u32 DATA_3DModelArcID_FootMarkSnow[FOOTMARK_MAX];

#ifndef FOOTMARK_SNOWCYCLE_OFF
static const u32 DATA_3DModelArcID_FootMarkShadowSnowCycle[FOOTMARK_MAX_CYCLE];
#endif
static const int DATA_FootMarkCycleDirTbl[DIR_4_MAX][DIR_4_MAX];
static const EOA_H_NPP DATA_EoaH_FootMark;

//==============================================================================
//	���Ձ@�V�X�e��
//==============================================================================
//--------------------------------------------------------------
/**
 * ���Տ�����
 * @param	fes		FE_SYS_PTR
 * @retval	FE_FOOTMARK_PTR		FE_FOOTMARK_PTR
 */
//--------------------------------------------------------------
void * FE_FootMark_Init( FE_SYS *fes )
{
	FE_FOOTMARK_PTR fm;
	
	fm = FE_AllocClearMemory( fes, FE_FOOTMARK_SIZE, ALLOC_FR, 0 );
	fm->fes = fes;
	
	FootMark_GraphicInit( fm );
	return( fm );
}

//--------------------------------------------------------------
/**
 * ���Ս폜
 * @param	fm		FE_FOOTMARK_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_FootMark_Delete( void *work )
{
	FE_FOOTMARK_PTR fm = work;
	FootMark_GraphicDelete( fm );
	FE_FreeMemory( fm );
}

//==============================================================================
//	���Ձ@�O���t�B�b�N
//==============================================================================
//--------------------------------------------------------------
/**
 * ���� �O���t�B�b�N������
 * @param	fm		FE_FOOTMARK_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
static void FootMark_GraphicInit( FE_FOOTMARK_PTR fm )
{
	u32 i,idx;
	
	for( i = 0; i < FOOTMARK_MAX; i++ ){
		idx = DATA_3DModelArcID_FootMark[i];
		FE_FROMdl_ResSetArcLoadTexTrans(
			fm->fes, &fm->rmdl_gnd[i], 0, idx, ALLOC_FR );
		FRO_OBJ_InitInMdl( &fm->robj_gnd[i], &fm->rmdl_gnd[i] );
		
		idx = DATA_3DModelArcID_FootMarkSnow[i];
		FE_FROMdl_ResSetArcLoadTexTrans(
			fm->fes, &fm->rmdl_snow[i], 0, idx, ALLOC_FR );
		FRO_OBJ_InitInMdl( &fm->robj_snow[i], &fm->rmdl_snow[i] );
	}
	
	#ifdef DEBUG_ONLY_FOR_kagaya
	{
		u32 free = sys_GetHeapFreeSize( HEAPID_FIELD );
		OS_Printf( "FldEff FootMark0 FIELD HEAP FreeSize %xH\n", free );
	}
	#endif
	
	#ifndef FOOTMARK_SNOWCYCLE_OFF
	for( i = 0; i < FOOTMARK_MAX_CYCLE; i++ ){
		idx = DATA_3DModelArcID_FootMarkShadowSnowCycle[i];
		FE_FROMdl_ResSetArcLoadTexTrans(
			fm->fes, &fm->rmdl_ssnow_c[i], 0, idx, ALLOC_FR );
		FRO_OBJ_InitInMdl( &fm->robj_ssnow_c[i], &fm->rmdl_ssnow_c[i] );
	}
	#endif
	
	#ifdef DEBUG_ONLY_FOR_kagaya
	{
		u32 free = sys_GetHeapFreeSize( HEAPID_FIELD );
		OS_Printf( "FldEff FootMark1 FIELD HEAP FreeSize %xH\n", free );
	}
	#endif
}

//--------------------------------------------------------------
/**
 * ���Ձ@�O���t�B�b�N�폜
 * @param	fm		FE_FOOTMARK_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
static void FootMark_GraphicDelete( FE_FOOTMARK_PTR fm )
{
	int i;
	
	for( i = 0; i < FOOTMARK_MAX; i++ ){
		FRO_MDL_DeleteAll( &fm->rmdl_gnd[i] );
		FRO_MDL_DeleteAll( &fm->rmdl_snow[i] );
	}
	
	#ifndef FOOTMARK_SNOWCYCLE_OFF
	for( i = 0; i < FOOTMARK_MAX_CYCLE; i++ ){
		FRO_MDL_DeleteAll( &fm->rmdl_ssnow_c[i] );
	}
	#endif
}

//--------------------------------------------------------------
/**
 * �w��^�C�v�̃����_�擾
 * @param	fm		FE_FOOTMARK_PTR
 * @param	type	FOOTMARK_TYPE_NORMAL��
 * @param	now_dir	���݂̌��� DIR_UP��
 * @param	old_dir	�ߋ��̌��� DIR_UP��
 * @param	attr	FOOTMARK_ATTR
 * @retval	NNSG3dRenderObj �w��̃����_
 */
//--------------------------------------------------------------
static FRO_OBJ * FootMark_TypeObjGet(
		FE_FOOTMARK_PTR fm, FOOTMARKTYPE type, int now_dir, int old_dir )
{
	int no;
	FRO_OBJ *robj = NULL;
	
	switch( type ){
	case FOOTMARKTYPE_NORMAL:
		no = FOOTMARK_WALK_UP + now_dir;
		robj = &fm->robj_gnd[no];
		break;
	case FOOTMARKTYPE_CYCLE:
		no = DATA_FootMarkCycleDirTbl[old_dir][now_dir];
		robj = &fm->robj_gnd[no];
		break;
	case FOOTMARKTYPE_SNOW:
	case FOOTMARKTYPE_SHADOWSNOW:
		no = FOOTMARK_WALK_UP + now_dir;
		robj = &fm->robj_snow[no];
		break;
	case FOOTMARKTYPE_SNOWDEEP:
		no = DATA_FootMarkCycleDirTbl[old_dir][now_dir];
		robj = &fm->robj_snow[no];
		break;
	#ifndef FOOTMARK_SNOWCYCLE_OFF
	case FOOTMARKTYPE_SHADOWSNOW_CYCLE:
		no = DATA_FootMarkCycleDirTbl[old_dir][now_dir];
		no -= FOOTMARK_CYCLE_START;
		GF_ASSERT( no < FOOTMARK_MAX_CYCLE );
		robj = &fm->robj_ssnow_c[no];
		break;
	#endif
	}
	
	GF_ASSERT( robj != NULL );
	return( robj );
}

//--------------------------------------------------------------
/**
 * �w��^�C�v�̃��f���擾
 * @param	fm		FE_FOOTMARK_PTR
 * @param	type	FOOTMARK_TYPE_NORMAL��
 * @param	now_dir	���݂̌��� DIR_UP��
 * @param	old_dir	�ߋ��̌��� DIR_UP��
 * @param	attr	FOOTMARK_ATTR
 * @retval	NNSG3dMdl �w��̃����_
 */
//--------------------------------------------------------------
static FRO_MDL * FootMark_TypeMdlGet(
		FE_FOOTMARK_PTR fm, FOOTMARKTYPE type, int now_dir, int old_dir )
{
	int no = 0;
	FRO_MDL *rmdl = NULL;
	
	switch( type ){
	case FOOTMARKTYPE_NORMAL:
		no = FOOTMARK_WALK_UP + now_dir;
		rmdl = &fm->rmdl_gnd[no];
		break;
	case FOOTMARKTYPE_CYCLE:
		no = DATA_FootMarkCycleDirTbl[old_dir][now_dir];
		rmdl = &fm->rmdl_gnd[no];
		break;
	case FOOTMARKTYPE_SNOW:
	case FOOTMARKTYPE_SHADOWSNOW:
		no = FOOTMARK_WALK_UP + now_dir;
		rmdl = &fm->rmdl_snow[no];
		break;
	case FOOTMARKTYPE_SNOWDEEP:
		no = DATA_FootMarkCycleDirTbl[old_dir][now_dir];
		rmdl = &fm->rmdl_snow[no];
		break;
	#ifndef FOOTMARK_SNOWCYCLE_OFF
	case FOOTMARKTYPE_SHADOWSNOW_CYCLE:
		no = DATA_FootMarkCycleDirTbl[old_dir][now_dir];
		no -= FOOTMARK_CYCLE_START;
		GF_ASSERT( no < FOOTMARK_MAX_CYCLE );
		rmdl = &fm->rmdl_ssnow_c[no];
		break;
	#endif
	}
	
	GF_ASSERT( rmdl != NULL );
	return( rmdl );
}

//==============================================================================
//	���Ձ@EOA
//==============================================================================
//--------------------------------------------------------------
/**
 * �t�B�[���hOBJ �Q�����Ւǉ�
 * @param	fldobj	FIELD_OBJ_PTR
 * @retval	eoa		EOA_PTR
 */
//--------------------------------------------------------------
EOA_PTR FE_FldOBJFootMarkNormal_Add( CONST_FIELD_OBJ_PTR fldobj )
{
	return( FldOBJFootMark_AddMain(fldobj,FOOTMARKTYPE_NORMAL) );
}

//--------------------------------------------------------------
/**
 * �t�B�[���hOBJ ���]�ԑ��Ւǉ�
 * @param	fldobj	FIELD_OBJ_PTR
 * @retval	eoa		EOA_PTR
 */
//--------------------------------------------------------------
EOA_PTR FE_FldOBJFootMarkCycle_Add( CONST_FIELD_OBJ_PTR fldobj )
{
	return( FldOBJFootMark_AddMain(fldobj,FOOTMARKTYPE_CYCLE) );
}

//--------------------------------------------------------------
/**
 * �t�B�[���hOBJ �ᑫ�Ւǉ�
 * @param	fldobj	FIELD_OBJ_PTR
 * @retval	eoa		EOA_PTR
 */
//--------------------------------------------------------------
EOA_PTR FE_FldOBJFootMarkSnow_Add( CONST_FIELD_OBJ_PTR fldobj )
{
	return( FldOBJFootMark_AddMain(fldobj,FOOTMARKTYPE_SNOW) );
}

//--------------------------------------------------------------
/**
 * �t�B�[���hOBJ �[���ᑫ�Ւǉ�
 * @param	fldobj	FIELD_OBJ_PTR
 * @retval	eoa		EOA_PTR
 */
//--------------------------------------------------------------
EOA_PTR FE_FldOBJFootMarkSnowDeep_Add( CONST_FIELD_OBJ_PTR fldobj )
{
	return( FldOBJFootMark_AddMain(fldobj,FOOTMARKTYPE_SNOWDEEP) );
}

//--------------------------------------------------------------
/**
 * �t�B�[���hOBJ �e�\���t���ᑫ�Ւǉ�
 * @param	fldobj	FIELD_OBJ_PTR
 * @retval	eoa		EOA_PTR
 */
//--------------------------------------------------------------
EOA_PTR FE_FldOBJFootMarkShadowSnow_Add( CONST_FIELD_OBJ_PTR fldobj )
{
	return( FldOBJFootMark_AddMain(fldobj,FOOTMARKTYPE_SHADOWSNOW) );
}

//--------------------------------------------------------------
/**
 * �t�B�[���hOBJ �e�\���t���Ꭹ�]�ԑ��Ւǉ�
 * @param	fldobj	FIELD_OBJ_PTR
 * @retval	eoa		EOA_PTR
 */
//--------------------------------------------------------------
EOA_PTR FE_FldOBJFootMarkShadowSnowCycle_Add( CONST_FIELD_OBJ_PTR fldobj )
{
	#ifndef FOOTMARK_SNOWCYCLE_OFF
	return( FldOBJFootMark_AddMain(fldobj,FOOTMARKTYPE_SHADOWSNOW_CYCLE) );
	#else
	return( NULL );
	#endif
}

//--------------------------------------------------------------
/**
 * �t�B�[���hOBJ���Ւǉ��@���C��
 * @param	fldobj	FIELD_OBJ_PTR
 * @param	attr	FOOTMARK_ATTR
 * @retval	eoa		EOA_PTR
 */
//--------------------------------------------------------------
static EOA_PTR FldOBJFootMark_AddMain( CONST_FIELD_OBJ_PTR fldobj, FOOTMARKTYPE type )
{
	VecFx32 mtx;
	EOA_PTR eoa;
	FOOTMARK_ADD_H head;
	int gx = FieldOBJ_OldPosGX_Get( fldobj );
	int gz = FieldOBJ_OldPosGZ_Get( fldobj );
	fx32 y = FieldOBJ_VecPosYGet( fldobj );
	int now_dir = FieldOBJ_DirDispGet( fldobj );
	int old_dir = FieldOBJ_DirDispOldGet( fldobj );
	int pri = FieldOBJ_TCBPriGet( fldobj, FLDOBJ_TCBPRI_OFFS_AFTER );
	FE_SYS *fes = FE_FieldOBJ_FE_SYS_Get( fldobj );
	FE_FOOTMARK_PTR fm = FE_EffectWorkGet( fes, FE_FLD_FOOTMARK );
	
	head.type = type;
	head.robj = FootMark_TypeObjGet( fm, type, now_dir, old_dir );
	head.rmdl = FootMark_TypeMdlGet( fm, type, now_dir, old_dir );
	
	FieldOBJTool_GridCenterPosGet( gx, gz, &mtx );
	mtx.y = y;
	
	switch( type ){
	case FOOTMARKTYPE_NORMAL:
	case FOOTMARKTYPE_CYCLE:
	case FOOTMARKTYPE_SHADOWSNOW:
	case FOOTMARKTYPE_SHADOWSNOW_CYCLE:
		mtx.y += NUM_FX32( -8 );
			
		if( type == FOOTMARK_WALK_LEFT || type == FOOTMARK_WALK_RIGHT ){
			mtx.z += NUM_FX32( -2 );
		}else{
			mtx.z += NUM_FX32( 1 );
		}
		
		break;
	case FOOTMARKTYPE_SNOW:
	case FOOTMARKTYPE_SNOWDEEP:
		mtx.y += NUM_FX32( -10 );
			
		if( type == FOOTMARK_WALK_LEFT || type == FOOTMARK_WALK_RIGHT ){
			mtx.z += NUM_FX32( -2 );
		}else{
			mtx.z += NUM_FX32( 2 );
		}
	}
	
	eoa = FE_EoaAddNpp( fes, &DATA_EoaH_FootMark, &mtx, 0, &head, pri );
	return( eoa );
}

//--------------------------------------------------------------
/**
 * EOA ���Ձ@������
 * @param	eoa	EOA_PTR
 * @param	wk	eoa work *
 * @retval	int TRUE=����I���BFALSE=�ُ�I��
 */
//--------------------------------------------------------------
static int EoaFootMark_Init( EOA_PTR eoa, void *wk )
{
	VecFx32 mtx;
	FOOTMARK_WORK *work;
	const FOOTMARK_ADD_H *head;
	
	work = wk;
	work->type = EOA_AddParamGet( eoa );
	
	head = EOA_AddPtrGet( eoa );
	work->robj = head->robj;
	work->rmdl = head->rmdl;
	work->alpha = 31;
	work->type = head->type;
	
	return( TRUE );
}

//--------------------------------------------------------------
/**
 * EOA ���Ձ@�폜
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaFootMark_Delete( EOA_PTR eoa, void *wk )
{
}

//--------------------------------------------------------------
/**
 * EOA ���Ձ@����
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaFootMark_Move( EOA_PTR eoa, void *wk )
{
	FOOTMARK_WORK *work;
	
	work = wk;
	
	switch( work->seq_no ){
	case 0:
		work->frame++;
		
		if( work->frame >= FOOTMARK_VANISH_START_FRAME ){
			work->seq_no++;
		}
		
		break;
	case 1:
		work->alpha -= 2;
		
		if( work->alpha < 0 ){
			FE_EoaDelete( eoa );
			return;
		}
	}
}

//--------------------------------------------------------------
/**
 * EOA ���Ձ@�`��
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaFootMark_Draw( EOA_PTR eoa, void *wk )
{
	FOOTMARK_WORK *work = wk;
	
	if( work->vanish_sw == FALSE ){
		VecFx32 pos;
		EOA_MatrixGet( eoa, &pos );
		NNS_G3dMdlUseMdlAlpha( work->rmdl->pResMdl );
		NNS_G3dMdlSetMdlAlphaAll( work->rmdl->pResMdl, work->alpha );
		FRO_OBJ_DrawPos( work->robj, &pos );
	}
}

//==============================================================================
//	data
//==============================================================================
//--------------------------------------------------------------
///	����imd�e�[�u���@���т�FOOTMARK_WALK_UP���̒l�Ɉ�v
//--------------------------------------------------------------
static const u32 DATA_3DModelArcID_FootMark[FOOTMARK_MAX] =
{
	NARC_fldeff_f_mark_u_nsbmd,
	NARC_fldeff_f_mark_d_nsbmd,
	NARC_fldeff_f_mark_l_nsbmd,
	NARC_fldeff_f_mark_r_nsbmd,
	NARC_fldeff_c_mark_u_nsbmd,
	NARC_fldeff_c_mark_l_nsbmd,
	NARC_fldeff_c_mark_ul_nsbmd,
	NARC_fldeff_c_mark_ur_nsbmd,
	NARC_fldeff_c_mark_dl_nsbmd,
	NARC_fldeff_c_mark_dr_nsbmd,
};

//--------------------------------------------------------------
///	����imd�e�[�u���@���т�FOOTMARK_WALK_UP���̒l�Ɉ�v�@��
//--------------------------------------------------------------
static const u32 DATA_3DModelArcID_FootMarkSnow[FOOTMARK_MAX] =
{
	NARC_fldeff_nf_mark_u_nsbmd,
	NARC_fldeff_nf_mark_d_nsbmd,
	NARC_fldeff_nf_mark_l_nsbmd,
	NARC_fldeff_nf_mark_r_nsbmd,
	NARC_fldeff_nc_mark_u_nsbmd,
	NARC_fldeff_nc_mark_l_nsbmd,
	NARC_fldeff_nc_mark_ul_nsbmd,
	NARC_fldeff_nc_mark_ur_nsbmd,
	NARC_fldeff_nc_mark_dl_nsbmd,
	NARC_fldeff_nc_mark_dr_nsbmd,
};

//--------------------------------------------------------------
///	����imd�e�[�u���@���т�FOOTMARK_WALK_UP���̒l�Ɉ�v�@�e�\����@���]��
//--------------------------------------------------------------
#ifndef FOOTMARK_SNOWCYCLE_OFF
static const u32 DATA_3DModelArcID_FootMarkShadowSnowCycle[FOOTMARK_MAX_CYCLE] =
{
	NARC_fldeff_sc_mark_u_nsbmd,
	NARC_fldeff_sc_mark_l_nsbmd,
	NARC_fldeff_sc_mark_ul_nsbmd,
	NARC_fldeff_sc_mark_ur_nsbmd,
	NARC_fldeff_sc_mark_dl_nsbmd,
	NARC_fldeff_sc_mark_dr_nsbmd,
};
#endif

//--------------------------------------------------------------
///	���]�ԑ��Ճe�[�u�� [�ߋ�����][���ݕ���]
//--------------------------------------------------------------
static const int DATA_FootMarkCycleDirTbl[DIR_4_MAX][DIR_4_MAX] =
{
	{FOOTMARK_CYCLE_UD,FOOTMARK_CYCLE_UD,FOOTMARK_CYCLE_UR,FOOTMARK_CYCLE_UL},
	{FOOTMARK_CYCLE_UD,FOOTMARK_CYCLE_UD,FOOTMARK_CYCLE_DR,FOOTMARK_CYCLE_DL},
	{FOOTMARK_CYCLE_DL,FOOTMARK_CYCLE_UL,FOOTMARK_CYCLE_LR,FOOTMARK_CYCLE_LR},
	{FOOTMARK_CYCLE_DR,FOOTMARK_CYCLE_UR,FOOTMARK_CYCLE_LR,FOOTMARK_CYCLE_LR},
};

//--------------------------------------------------------------
///	����EOA_H
//--------------------------------------------------------------
static const EOA_H_NPP DATA_EoaH_FootMark =
{
	FOOTMARK_WORK_SIZE,
	EoaFootMark_Init,
	EoaFootMark_Delete,
	EoaFootMark_Move,
	EoaFootMark_Draw,
};


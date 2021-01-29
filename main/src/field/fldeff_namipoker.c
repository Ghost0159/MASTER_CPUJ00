//******************************************************************************
/**
 * 
 * @file	fldeff_namipoker.c
 * @brief	�t�B�[���hOBJ�@���@�g���|�P�����@��]����t���@���@��p
 * @author	kagaya
 * @data	05.07.13
 *
 */
//******************************************************************************
#include "common.h"
#include "fieldsys.h"
#include "field_effect.h"
#include "fieldobj.h"
#include "fldeff_namipoker.h"

//==============================================================================
//	define
//==============================================================================
#ifdef DEBUG_ONLY_FOR_kagaya
//#define DEBUG_MDL_ROTATE	//��`�ŉ�]�e�X�g
//#define DEBUG_MDL_MOVE	//��`�ō��W�ړ�
//#define DEBUG_MDL_REVO	//��`�Ō��]�e�X�g
#endif

//�V��ʂł̃I�t�Z�b�g
#define NAMIPOKER_Y_GRID_OFFS_FX32_ROOF (FX32_ONE*40)
#define NAMIPOKER_RIDE_Y_OFFSET_ROOF (FX32_ONE*7)

///<�����h�ꕝ
#define NAMIPOKER_SHAKE_VALUE (0x0400)
#define NAMIPOKER_SHAKE_MAX (FX32_ONE*4)

#define NAMIPOKER_ON_FRAME (30)
#define NAMIPOKER_OFF_FRAME (NAMIPOKE_ON_FRAME+30)

//--------------------------------------------------------------
///	���a�T�C�Y
//--------------------------------------------------------------
#define NPOKER_ANGLE_REV_SIZE (20)

#define NPOKER_POS_X_DEF_OFFSET (NUM_FX32(0))
#define NPOKER_POS_Y_DEF_OFFSET (NUM_FX32(+8))
#define NPOKER_POS_Z_DEF_OFFSET (NUM_FX32(0))

#define NPOKER_POS_X_OFFSET_FLAT (NUM_FX32(1)/4)
#define NPOKER_POS_Y_OFFSET_FLAT (NUM_FX32(6))
#define NPOKER_POS_Z_OFFSET_FLAT (NUM_FX32(-5))
#define NPOKER_POS_Z_OFFSET_ROOF (NUM_FX32(6))

//==============================================================================
//	typedef struct
//==============================================================================
//--------------------------------------------------------------
///	FE_NAMIPOKER�^
//--------------------------------------------------------------
typedef struct _TAG_FE_NAMIPOKER * FE_NAMIPOKER_PTR;

//--------------------------------------------------------------
///	FE_NAMIPOKER�\����
//--------------------------------------------------------------
typedef struct _TAG_FE_NAMIPOKER
{
	FE_SYS *fes;
	FRO_MDL rmdl;
	FRO_OBJ robj;
}FE_NAMIPOKER;

///FE_NAMIPOKE�T�C�Y
#define FE_NAMIPOKER_SIZE (sizeof(FE_NAMIPOKER))

//--------------------------------------------------------------
///	NAMIPOKER_ADD_H�\����
//--------------------------------------------------------------
typedef struct
{
	int init_dir;					///<�\������
	HEROTWTYPE tw_type;				///<�n�`�^�C�v
	
	FE_SYS *fes;					///<FE_SYS_PTR
	FE_NAMIPOKER_PTR namipoke;		///<FE_NAMIPOKER_PTR
	FIELD_OBJ_PTR fldobj;			///<FIELD_OBJ_PTR
	PLAYER_STATE_PTR jiki;			///<PLAYER_STATE_PTR
}NAMIPOKER_ADD_H;

///NAMIPOKER_ADD_H�T�C�Y
#define NAMIPOKER_ADD_H_SIZE (sizeof(NAMIPOKER_ADD_H))

//--------------------------------------------------------------
///	NAMIPOKER_WORK�\����
//--------------------------------------------------------------
typedef struct
{
	u32 status_bit;					///<�X�e�[�^�X�r�b�g
	s16 dir;						///<����
	u16 frame;						///<�\���t���[��
	u16 angle_rev;					//270
	u16 angle_rev_size;				//20
	ROTATE rotate;
	VecFx32 shake_outside;
	VecFx32 offs_outside;
	fx32 shake_offs;							///<�h��
	fx32 shake_value;							///<�h���
	
	HEROTWTYPE tw_type;							///<HEROTWTYPE
	NAMIPOKER_ADD_H head;						///<�ǉ�����NAMIPOKER_ADD_H
	
	#ifdef DEBUG_MDL_MOVE
	VecFx32 debug_offs;
	#endif
	
	#ifdef DEBUG_MDL_REVO
	VecFx32 debug_rev_offs;
	#endif
}NAMIPOKER_WORK;

///NAMIPOKER_WORK�T�C�Y
#define NAMIPOKER_WORK_SIZE (sizeof(NAMIPOKER_WORK))

//==============================================================================
//	�v���g�^�C�v
//==============================================================================
static void NamiPokeR_GraphicInit( FE_NAMIPOKER_PTR namipoke );
static void NamiPokeR_GraphicDelete( FE_NAMIPOKER_PTR namipoke );

//parts
static void npoke_rotate_set( HEROTWTYPE tw, int dir, ROTATE *rot );

//data
static const EOA_H_NPP DATA_EoaH_NamiPokeR;
static const VecFx32 DATA_TwDirPokeOffsetTbl[HEROTWTYPE_MAX];
static const VecFx32 DATA_TwDirJointJikiOffsetTbl[HEROTWTYPE_MAX];
static const VecFx32 DATA_TwDirJointPokeOffsetTbl[HEROTWTYPE_MAX];
static const u32 DATA_TwJointAngleRevTbl[HEROTWTYPE_MAX];
static const ROTATE DATA_TwDirRotateTbl[HEROTWTYPE_MAX][DIR_4_MAX];

//debug
#ifdef DEBUG_MDL_ROTATE
static void debug_mdl_rotate( EOA_PTR eoa, NAMIPOKER_WORK *work );
#endif

#ifdef DEBUG_MDL_MOVE
static void debug_mdl_move( EOA_PTR eoa, NAMIPOKER_WORK *work );
#endif

#ifdef DEBUG_MDL_REVO
static void debug_mdl_revo_angle( EOA_PTR eoa, NAMIPOKER_WORK *work );
static void debug_mdl_revo_pos(
	EOA_PTR eoa, NAMIPOKER_WORK *work, VecFx32 *poswk );
#endif

//--------------------------------------------------------------
/// �X�e�[�^�X�r�b�g�}�N��
//--------------------------------------------------------------
#define NPOKER_BIT_CHECK(w,b) ((w)->status_bit&(b))

//==============================================================================
//	�g���|�P�����@�V�X�e��
//==============================================================================
//--------------------------------------------------------------
/**
 * �g���|�P����������
 * @param	fes		FE_SYS_PTR
 * @retval	FE_NAMIPOKER_PTR	FE_NAMIPOKER_PTR
 */
//--------------------------------------------------------------
void * FE_NamiPokeR_Init( FE_SYS *fes )
{
	FE_NAMIPOKER_PTR namipoke;
	
	namipoke = FE_AllocClearMemory( fes, FE_NAMIPOKER_SIZE, ALLOC_FR, 0 );
	namipoke->fes = fes;
	
	NamiPokeR_GraphicInit( namipoke );
	return( namipoke );
}

//--------------------------------------------------------------
/**
 * �g���|�P�����폜
 * @param	namipoke		FE_NAMIPOKER_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_NamiPokeR_Delete( void *work )
{
	FE_NAMIPOKER_PTR namipoke = work;
	NamiPokeR_GraphicDelete( namipoke );
	FE_FreeMemory( namipoke );
}

//==============================================================================
//	�g���|�P�����@�O���t�B�b�N
//==============================================================================
//--------------------------------------------------------------
/**
 * �g���|�P���� �O���t�B�b�N������
 * @param	namipoke	FE_NAMIPOKER_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
static void NamiPokeR_GraphicInit( FE_NAMIPOKER_PTR namipoke )
{
	FE_FROMdl_ResSetArcLoadTexTrans(
		namipoke->fes, &namipoke->rmdl, 0,
		NARC_fldeff_sea_ride_nsbmd, ALLOC_FR );
	FRO_OBJ_InitInMdl( &namipoke->robj, &namipoke->rmdl );
}

//--------------------------------------------------------------
/**
 * �g���|�P���� �O���t�B�b�N�폜
 * @param	namipoke	FE_NAMIPOKER_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
static void NamiPokeR_GraphicDelete( FE_NAMIPOKER_PTR namipoke )
{
	FRO_MDL_DeleteAll( &namipoke->rmdl );
}

//==============================================================================
//	�g���|�P�����@EOA
//==============================================================================
//--------------------------------------------------------------
/**
 * �g���|�P�����ǉ�
 * @param	jiki	�ΏۂƂȂ�PLAYER_STATE_PTR
 * @param	gx		�\������O���b�hX���W
 * @param	gy		�\������O���b�hY���W(�n�ʃO���b�h�P��
 * @param	gz		�\������O���b�hZ���W
 * @param	dir		�\���������
 * @param	HEROTWTYPE HEROTWTYPE
 * @param	joint	TRUE=fldobj���W�ɐڑ��@FALSE=�ڑ����Ȃ�
 * @retval	EOA_PTR	�g���|�P����EOA_PTR
 */
//--------------------------------------------------------------
EOA_PTR FE_FldOBJNamiPokeRSet( PLAYER_STATE_PTR jiki,
	int gx, int gy, int gz, int dir, int joint, HEROTWTYPE tw_type )
{
	int pri;
	EOA_PTR eoa;
	const VecFx32 *offs;
	VecFx32 mtx = {0,0,0};
	NAMIPOKER_ADD_H head;
	FIELD_OBJ_PTR fldobj;
	
	fldobj = Player_FieldOBJGet( jiki );
	
	head.init_dir = dir;
	head.tw_type = tw_type;
	head.fes = FE_FieldOBJ_FE_SYS_Get( fldobj );
	head.namipoke = FE_EffectWorkGet( head.fes, FE_FLD_NAMIPOKER );
	head.fldobj = fldobj;
	head.jiki = jiki;
	
	if( joint == FALSE ){
		FIELDSYS_WORK *fsys = FieldOBJ_FieldSysWorkGet( fldobj );
		offs = &DATA_TwDirPokeOffsetTbl[tw_type];
		mtx.x = GRID_SIZE_FX32( gx ) + offs->x;
		mtx.y = GRID_SIZE_FX32( gy ) + offs->y;
		mtx.z = GRID_SIZE_FX32( gz ) + offs->z;
	}else{
		offs = &DATA_TwDirJointJikiOffsetTbl[tw_type];
		FieldOBJ_VecPosGet( fldobj, &mtx );
		FieldOBJ_VecDrawOffsOutSideSet( fldobj, offs );
	}
	
	pri = FieldOBJ_TCBPriGet( fldobj, FLDOBJ_TCBPRI_OFFS_AFTER );
	eoa = FE_EoaAddNpp(
		head.fes, &DATA_EoaH_NamiPokeR, &mtx, joint, &head, pri );
	
	return( eoa );
}

//--------------------------------------------------------------
/**
 * EOA �g���|�P�����@������
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	int		TRUE=����I���BFALSE=�ُ�I��
 */
//--------------------------------------------------------------
static int EoaNamiPokeR_Init( EOA_PTR eoa, void *wk )
{
	NAMIPOKER_WORK *work = wk;
	const NAMIPOKER_ADD_H *head = EOA_AddPtrGet( eoa );
	FIELD_OBJ_PTR fldobj = head->fldobj;
	
	work->head = *head;
	work->dir = head->init_dir;
	work->shake_offs = FX32_ONE;
	work->shake_value = NAMIPOKER_SHAKE_VALUE;
	work->tw_type = head->tw_type;
	npoke_rotate_set( work->tw_type, work->dir, &work->rotate );
	work->angle_rev = DATA_TwJointAngleRevTbl[work->tw_type];
	work->angle_rev_size = NPOKER_ANGLE_REV_SIZE;
	
	if( EOA_AddParamGet(eoa) == TRUE ){
		work->status_bit |= NPOKER_BIT_JOINT;
	}
	
	return( TRUE );
}

//--------------------------------------------------------------
/**
 * EOA �g���|�P�����@�폜
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaNamiPokeR_Delete( EOA_PTR eoa, void *wk )
{
	NAMIPOKER_WORK *work = wk;
	FIELD_OBJ_PTR fldobj = work->head.fldobj;
	VecFx32 offs = { 0, 0, 0 };
	FieldOBJ_VecDrawOffsOutSideSet( fldobj, &offs );
}

//--------------------------------------------------------------
/**
 * EOA �g���|�P�����@����
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaNamiPokeR_Move( EOA_PTR eoa, void *wk )
{
	NAMIPOKER_WORK *work = wk;
	FIELD_OBJ_PTR fldobj = work->head.fldobj;
	PLAYER_STATE_PTR jiki = work->head.jiki;
	
	if( NPOKER_BIT_CHECK(work,NPOKER_BIT_JOINT) == 0 ){
		#ifdef DEBUG_MDL_ROTATE
		debug_mdl_rotate( eoa, work );
		#endif
		
		#ifdef DEBUG_MDL_MOVE
		debug_mdl_move( eoa, work );
		#endif
		
		return;
	}
	
	work->dir = FieldOBJ_DirMoveGet( fldobj );
	work->tw_type = Player_MoveBitCheck_TWorld( jiki );
	GF_ASSERT( work->dir != DIR_NOT );
	
	{													//�h�ꕝ
		work->shake_offs += work->shake_value;
		
		if( work->shake_offs >= NAMIPOKER_SHAKE_MAX ){
			work->shake_offs = NAMIPOKER_SHAKE_MAX;
			work->shake_value = -work->shake_value;
		}else if( work->shake_offs <= FX32_ONE ){
			work->shake_offs = FX32_ONE;
			work->shake_value = -work->shake_value;
		}
	}
	
	if( NPOKER_BIT_CHECK(work,NPOKER_BIT_ROTATE_OFF) == 0 ){
		npoke_rotate_set( work->tw_type, work->dir, &work->rotate );
	}
	
	if( NPOKER_BIT_CHECK(work,NPOKER_BIT_ANGLE_REV_OFF) == 0 ){
		#ifndef DEBUG_MDL_REVO
		work->angle_rev = DATA_TwJointAngleRevTbl[work->tw_type];
		#endif
	}
	
	if( NPOKER_BIT_CHECK(work,NPOKER_BIT_POS_OFF) == 0 ){
		VecFx32 poswk;
		poswk = DATA_TwDirJointJikiOffsetTbl[work->tw_type];
		
		if( NPOKER_BIT_CHECK(work,NPOKER_BIT_SHAKE_OUTSIDE) == 0 ){
			poswk.y += work->shake_offs;
		}else{
			poswk.x += work->shake_outside.x;
			poswk.y += work->shake_outside.y;
			poswk.z += work->shake_outside.z;
		}
		
		FieldOBJ_VecDrawOffsOutSideSet( fldobj, &poswk );
		
		#ifdef DEBUG_MDL_REVO
		debug_mdl_revo_angle( eoa, work );
		#endif
		
		FieldOBJ_VecPosGet( fldobj, &poswk );
		
		{
			const VecFx32 *offs;
			
			if( NPOKER_BIT_CHECK(work,NPOKER_BIT_POKE_OFFS_OFF) == 0 ){
				offs = &DATA_TwDirJointPokeOffsetTbl[work->tw_type];
			}else{
				offs = &work->offs_outside;
			}
			
			poswk.x += (NPOKER_POS_X_DEF_OFFSET + offs->x) +
				Cos360(work->angle_rev) * work->angle_rev_size;
			poswk.y += (NPOKER_POS_Y_DEF_OFFSET + offs->y) +
				Sin360(work->angle_rev) * work->angle_rev_size;
			poswk.z += (NPOKER_POS_Z_DEF_OFFSET + offs->z);
		}
		
		if( NPOKER_BIT_CHECK(work,NPOKER_BIT_SHAKE_OUTSIDE) == 0 ){
			poswk.y += work->shake_offs;
		}else{
			poswk.x += work->shake_outside.x;
			poswk.y += work->shake_outside.y;
			poswk.z += work->shake_outside.z;
		}
		
		#ifdef DEBUG_MDL_REVO
		debug_mdl_revo_pos( eoa, work, &poswk );
		#endif
		
		EOA_MatrixSet( eoa, &poswk );
	}
}

//--------------------------------------------------------------
/**
 * EOA �g���|�P�����@�`��
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaNamiPokeR_Draw( EOA_PTR eoa, void *wk )
{
	NAMIPOKER_WORK *work = wk;
	
	if( NPOKER_BIT_CHECK(work,NPOKER_BIT_VANISH) == 0 ){
		VecFx32 pos;
		FRO_OBJ *robj = &work->head.namipoke->robj;
		EOA_MatrixGet( eoa, &pos );
		
		#ifdef DEBUG_MDL_MOVE
		pos.x += work->debug_offs.x;
		pos.y += work->debug_offs.y;
		pos.z += work->debug_offs.z;
		#endif
		
		FRO_OBJ_DrawRotate( robj, &pos, &work->rotate );
		
	}
	
	work->frame++;
}

//==============================================================================
//	�g���|�P�����A�N�^�[�p�[�c
//==============================================================================
//--------------------------------------------------------------
/**
 * �j�ꂽ�n�`�ƕ��������]�p�x���Z�b�g
 * @param	tw		HEROTWTYPE
 * @param	dir		DIR_UP��
 * @param	rotate	�p�x�i�[��
 * @retval	nothing
 */
//--------------------------------------------------------------
static void npoke_rotate_set( HEROTWTYPE tw, int dir, ROTATE *rot )
{
	const ROTATE *rot_tbl;
	GF_ASSERT( (u32)tw < HEROTWTYPE_MAX );
	rot_tbl = &DATA_TwDirRotateTbl[tw][dir];
	*rot = *rot_tbl;
}

//--------------------------------------------------------------
/**
 * ���]�p�x�|�C���^�擾
 * @param	eoa		�g���|�P����EOA_PTR
 * @retval	ROTATE*
 */
//--------------------------------------------------------------
ROTATE * FE_FldOBJNamiPokeR_RotatePtrGet( EOA_PTR eoa )
{
	NAMIPOKER_WORK *work = EOA_LocalWorkGet( eoa );
	return( &work->rotate );
}

//--------------------------------------------------------------
/**
 * �w������Ŏ��]�p�x������
 * @param	eoa		�g���|�P����EOA_PTR
 * @retval	ROTATE*
 */
//--------------------------------------------------------------
void FE_FldOBJNamiPokeR_RotateDirInit(
		EOA_PTR eoa, int dir, HEROTWTYPE tw_type )
{
	NAMIPOKER_WORK *work = EOA_LocalWorkGet( eoa );
	npoke_rotate_set( tw_type, dir, &work->rotate );
}

//--------------------------------------------------------------
/**
 * �g���|�P�����@�X�e�[�^�X�r�b�g�𗧂Ă�
 * @param	eoa	�g���|�P����EOA_PTR
 * @param	bit	NPOKER_BIT
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_FldOBJNamiPokeR_StatusBitON( EOA_PTR eoa, NPOKER_BIT bit )
{
	NAMIPOKER_WORK *work = EOA_LocalWorkGet( eoa );
	work->status_bit |= bit;
}

//--------------------------------------------------------------
/**
 * �g���|�P�����@�X�e�[�^�X�r�b�g�����낷
 * @param	eoa	�g���|�P����EOA_PTR
 * @param	bit	NPOKER_BIT
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_FldOBJNamiPokeR_StatusBitOFF( EOA_PTR eoa, NPOKER_BIT bit )
{
	NAMIPOKER_WORK *work = EOA_LocalWorkGet( eoa );
	work->status_bit &= ~bit;
}

//--------------------------------------------------------------
/**
 * �g���|�P�����@�O���w����W�h��I�t�Z�b�g�|�C���^�擾
 * @param	eoa	�g���|�P����EOA_PTR
 * @retval	VecFx32 *
 */
//--------------------------------------------------------------
VecFx32 * FE_FldOBJNamiPokeR_ShakeOffsetPtrGet( EOA_PTR eoa )
{
	NAMIPOKER_WORK *work = EOA_LocalWorkGet( eoa );
	return( &work->shake_outside );
}

//--------------------------------------------------------------
/**
 * �g���|�P�����@�O���w����W�g���|�P�����I�t�Z�b�g�|�C���^�擾
 * @param	eoa	�g���|�P����EOA_PTR
 * @retval	VecFx32 *
 */
//--------------------------------------------------------------
VecFx32 * FE_FldOBJNamiPokeR_PokeOffsetPtrGet( EOA_PTR eoa )
{
	NAMIPOKER_WORK *work = EOA_LocalWorkGet( eoa );
	return( &work->offs_outside );
}

//--------------------------------------------------------------
/**
 * �g���|�P�����@���]�p�x�Z�b�g
 * @param	eoa	�g���|�P����EOA_PTR
 * @param	angle	�p�x 360
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_FldOBJNamiPokeR_AngleRevSet( EOA_PTR eoa, u16 angle )
{
	NAMIPOKER_WORK *work = EOA_LocalWorkGet( eoa );
	work->angle_rev = angle;
}

//==============================================================================
//	data
//==============================================================================
//--------------------------------------------------------------
///	�g���|�P����EOA_H
//--------------------------------------------------------------
static const EOA_H_NPP DATA_EoaH_NamiPokeR =
{
	NAMIPOKER_WORK_SIZE,
	EoaNamiPokeR_Init,
	EoaNamiPokeR_Delete,
	EoaNamiPokeR_Move,
	EoaNamiPokeR_Draw,
};

//--------------------------------------------------------------
///	��ڑ����̒n�`�ʃ|�P�����I�t�Z�b�g���W
//--------------------------------------------------------------
static const VecFx32 DATA_TwDirPokeOffsetTbl[HEROTWTYPE_MAX] =
{
	//non
	{FLDOBJ_VEC_X_GRID_OFFS_FX32,0,FLDOBJ_VEC_Z_GRID_OFFS_FX32},
	//on
	{FLDOBJ_VEC_X_GRID_OFFS_FX32,0,FLDOBJ_VEC_Z_GRID_OFFS_FX32},
	//ground
	{FLDOBJ_VEC_X_GRID_OFFS_FX32,0,FLDOBJ_VEC_Z_GRID_OFFS_FX32},
	//wall left
	{FLDOBJ_VEC_X_GRID_OFFS_FX32,0,FLDOBJ_VEC_Z_GRID_OFFS_FX32},
	//wall right
	{FLDOBJ_VEC_X_GRID_OFFS_FX32,0,FLDOBJ_VEC_Z_GRID_OFFS_FX32},
	//roof
	{
		FLDOBJ_VEC_X_GRID_OFFS_FX32,
		NAMIPOKER_Y_GRID_OFFS_FX32_ROOF,
		FLDOBJ_VEC_Z_GRID_OFFS_FX32
	},
};

//--------------------------------------------------------------
///	�n�`�ʎ��@�I�t�Z�b�g���W
//--------------------------------------------------------------
static const VecFx32 DATA_TwDirJointJikiOffsetTbl[HEROTWTYPE_MAX] =
{
	//non
	{0,0,0},
	//on
	{0,0,0},
	//ground
	{0,0,0},
	//wall left
	{0,0,0},
	//wall right
	{0,0,0},
	//roof
	{0,0,0},
};

//--------------------------------------------------------------
///	�n�`�ʃ|�P�����I�t�Z�b�g���W
//--------------------------------------------------------------
static const VecFx32 DATA_TwDirJointPokeOffsetTbl[HEROTWTYPE_MAX] =
{
	//non
	{NPOKER_POS_X_OFFSET_FLAT,NPOKER_POS_Y_OFFSET_FLAT,NPOKER_POS_Z_OFFSET_FLAT},
	//on
	{NPOKER_POS_X_OFFSET_FLAT,NPOKER_POS_Y_OFFSET_FLAT,NPOKER_POS_Z_OFFSET_FLAT},
	//ground
	{NPOKER_POS_X_OFFSET_FLAT,NPOKER_POS_Y_OFFSET_FLAT,NPOKER_POS_Z_OFFSET_FLAT},
	//wall left
	{0,0,0},
	//wall right
	{0,0,0},
	//roof
	{0,0,NPOKER_POS_Z_OFFSET_ROOF},
};

//--------------------------------------------------------------
///	�n�`�ʌ��]�p�x
//--------------------------------------------------------------
static const u32 DATA_TwJointAngleRevTbl[HEROTWTYPE_MAX] =
{
	270, //non
	270, //on
	270, //ground
	0, //wall left
	0, //wall right
	90, //roof
};

//--------------------------------------------------------------
///	�W���C���g���̒n�`�ʐڑ����W
//--------------------------------------------------------------
#if 0
static const VecFx32 DATA_TwDirJointPokeOffsetTbl[HEROTWTYPE_MAX] =
{
	//non
	{0,0,0},
	//on
	{0,0,0},
	//ground
	{0,0,0},
	//wall left
	{0,0,0},
	//wall right
	{0,0,0},
	//roof
	{0,NAMIPOKER_Y_GRID_OFFS_FX32_ROOF,0},
};
#endif

//--------------------------------------------------------------
///	�n�`�������ʎ��]�p�x
//--------------------------------------------------------------
static const ROTATE DATA_TwDirRotateTbl[HEROTWTYPE_MAX][DIR_4_MAX] =
{
	{{0,180,0},{0,0,0},{0,270,0},{0,90,0}}, //non
	{{0,180,0},{0,0,0},{0,270,0},{0,90,0}}, //on
	{{0,180,0},{0,0,0},{0,270,0},{0,90,0}}, //ground
	{{0,180,0},{0,0,0},{0,270,0},{0,90,0}}, //wall left  �o��V�[������
	{{0,180,0},{0,0,0},{0,270,0},{0,90,0}}, //wall right �o��V�[������
	{{180,180,0},{180,0,0},{180,90,0},{180,270,0}}, //roof
};

//==============================================================================
//	debug
//==============================================================================
#define PADCNT(a) (sys.cont&(a))
#define PADTRG(a) (sys.trg&(a))

#ifdef DEBUG_MDL_ROTATE
static void debug_mdl_rotate( EOA_PTR eoa, NAMIPOKER_WORK *work )
{
	if( PADCNT(PAD_BUTTON_L) == 0 ){
		return;
	}
	
	{
		ROTATE *rot = &work->rotate;
		ROTATE old = *rot;
		
		if( PADCNT(PAD_BUTTON_A) ){
			if( PADCNT(PAD_KEY_UP) ){
				rot->z -= 2;
			}
			
			if( PADCNT(PAD_KEY_DOWN) ){
				rot->z += 2;
			}
		}else{
			if( PADCNT(PAD_KEY_UP) ){
				rot->y -= 2;
			}
			
			if( PADCNT(PAD_KEY_DOWN) ){
				rot->y += 2;
			}
			
			if( PADCNT(PAD_KEY_LEFT) ){
				rot->x -= 2;
			}
			
			if( PADCNT(PAD_KEY_RIGHT) ){
				rot->x += 2;
			}
		}
		
		rot->x %= 360;
		rot->y %= 360;
		rot->z %= 360;
		
		if( PADTRG(PAD_BUTTON_B) ||
			old.x != rot->x || old.y != rot->y || old.z != rot->z ){
			OS_Printf( "�g����] X=%d,Y=%d,Z=%d\n",
					rot->x, rot->y, rot->z );
		}
	}
}
#endif

#ifdef DEBUG_MDL_MOVE
static void debug_mdl_move( EOA_PTR eoa, NAMIPOKER_WORK *work )
{
	{
		VecFx32 *offs = &work->debug_offs;
		VecFx32 old = *offs;
		
		if( PADCNT(PAD_BUTTON_A) ){
			if( PADCNT(PAD_KEY_UP) ){
				offs->z -= FX32_ONE;
			}
			
			if( PADCNT(PAD_KEY_DOWN) ){
				offs->z += FX32_ONE;
			}
		}else{
			if( PADCNT(PAD_KEY_UP) ){
				offs->y -= FX32_ONE;
			}
			
			if( PADCNT(PAD_KEY_DOWN) ){
				offs->y += FX32_ONE;
			}
			
			if( PADCNT(PAD_KEY_LEFT) ){
				offs->x -= FX32_ONE;
			}
			
			if( PADCNT(PAD_KEY_RIGHT) ){
				offs->x += FX32_ONE;
			}
		}
		
		if( PADTRG(PAD_BUTTON_B) ||
			old.x != offs->x || old.y != offs->y || old.z != offs->z ){
			OS_Printf( "�g�����W X=%d,Y=%d,Z=%d\n",
					FX32_NUM(offs->x), FX32_NUM(offs->y), FX32_NUM(offs->z) );
		}
	}
}
#endif

#ifdef DEBUG_MDL_REVO
static void debug_mdl_revo_angle( EOA_PTR eoa, NAMIPOKER_WORK *work )
{
	if( (sys.cont & PAD_BUTTON_X)  ){
		if( (sys.cont & PAD_BUTTON_L) ){
			work->angle_rev -= 2;
		}else if( (sys.cont & PAD_BUTTON_R) ){
			work->angle_rev += 2;
		}
		
		while( (s16)work->angle_rev < 0 ){
			work->angle_rev += 360;
		}
		
		work->angle_rev %= 360;
	}
}

static void debug_mdl_revo_pos(
	EOA_PTR eoa, NAMIPOKER_WORK *work, VecFx32 *poswk )
{
	if( (sys.cont & PAD_BUTTON_A) ){
		if( (sys.cont & PAD_BUTTON_L) ){
			work->debug_rev_offs.z -= NUM_FX32(1);
		}else if( (sys.cont & PAD_BUTTON_R) ){
			work->debug_rev_offs.z += NUM_FX32(1);
		}
	}else if( (sys.cont & PAD_BUTTON_B) ){
		if( (sys.cont & PAD_BUTTON_L) ){
			work->debug_rev_offs.y -= NUM_FX32(1);
		}else if( (sys.cont & PAD_BUTTON_R) ){
			work->debug_rev_offs.y += NUM_FX32(1);
		}
	}
	
	poswk->x += work->debug_rev_offs.x;
	poswk->y += work->debug_rev_offs.y;
	poswk->z += work->debug_rev_offs.z;
	
	if( (sys.trg & PAD_BUTTON_SELECT) ){
		OS_Printf( "���{�����[�V���� X=%d,Y=%d,Z=%d,ANGLE=%d\n",
			FX32_NUM(work->debug_rev_offs.x),
			FX32_NUM(work->debug_rev_offs.y),
			FX32_NUM(work->debug_rev_offs.z),
			work->angle_rev );
	}
}
#endif

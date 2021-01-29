//******************************************************************************
/**
 * 
 * @file	field_rdobj.c
 * @brief	�t�B�[���h�@�����_�����OOBJ
 * @author	kagaya
 * @data	05.07.13
 */
//******************************************************************************
#include "common.h"
#include "field_rdobj.h"

//==============================================================================
//	define
//==============================================================================
//--------------------------------------------------------------
//	anime
//--------------------------------------------------------------
#define ANMBIT_NON		(0)
#define ANMBIT_RES_MAIN	(1<<0)
#define ANMBIT_ANM_END	(1<<1)

#define BITCHK(a,b) ((a)&(b))
#define BITFLIP(b) (~(b))

//--------------------------------------------------------------
//	debug
//--------------------------------------------------------------
//#define DEBUG_MDL_VINTR_BUG_PASS	//��`�Ń��f���]���^�X�N�o�O�����

//==============================================================================
//	typedef
//==============================================================================

//==============================================================================
//	proto
//==============================================================================
static void FROAnm_AnmResSet( FRO_ANM *ranm, void *pRes, u32 idx );

#ifdef DEBUG_FRO_VINTR_COUNT
int DEBUG_FroVIntrCount;
#endif

//--------------------------------------------------------------
///	data
//--------------------------------------------------------------
static const VecFx32 DATA_OneScaleFx32 =
{FX32_ONE,FX32_ONE,FX32_ONE};
static const MtxFx33 DATA_ZeroRotateFx33 =
{FX32_ONE,0,0,0,FX32_ONE,0,0,0,FX32_ONE};

//==============================================================================
//	FRO_MDL
//==============================================================================
//--------------------------------------------------------------
/**
 * FRO_MDL	�N���A
 * @param	rmdl	FRO_MDL
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_MDL_Clear( FRO_MDL *rmdl )
{
	memset( rmdl, 0, sizeof(FRO_MDL) );
}

//--------------------------------------------------------------
/**
 * ���\�[�X�t�@�C���w�b�_�[����FRO_MDL�ݒ�
 * �w�b�_�[�����烂�f���A�e�N�X�`�������o���B
 * @param	rmdl	FRO_MDL
 * @param	head	���\�[�X�t�@�C���f�[�^
 * @param	idx	�擾���郂�f���̃C���f�b�N�X
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_MDL_ResSetFileHeader( FRO_MDL *rmdl, NNSG3dResFileHeader *head, u32 idx )
{
	FRO_MDL_Clear( rmdl );
	rmdl->pResFileHeader = head;
	rmdl->bind_tex_flag = FALSE;
	rmdl->pResMdlSet = NNS_G3dGetMdlSet( head );
	rmdl->pResMdl = NNS_G3dGetMdlByIdx( rmdl->pResMdlSet, idx );
	rmdl->pResTex = NNS_G3dGetTex( head );
}

//--------------------------------------------------------------
/**
 * �A�[�J�C�u����imd�t�@�C���w�b�_�[�����[�h��A
 * �w�b�_�[�����烂�f���A�e�N�X�`�������o���B
 * @param	rmdl	FRO_MDL
 * @param	idx		�擾���郂�f���̃C���f�b�N�X
 * @param	handle	�A�[�J�C�u�n���h��
 * @param	arc_idx	�A�[�J�C�u��imd�t�@�C���w�b�_�[�C���f�b�N�X
 * @param	heap_id	�q�[�vID
 * @param	alloc_type	ALLOC_TYPE
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_MDL_ResSetArcLoad( FRO_MDL *rmdl, u32 idx,
		ARCHANDLE *handle, u32 arc_idx, u32 heap_id, ALLOC_TYPE alloc_type )
{
	NNSG3dResFileHeader *head =
		ArcUtil_HDL_Load( handle, arc_idx, FALSE, heap_id, alloc_type );
	FRO_MDL_ResSetFileHeader( rmdl, head, idx );
}

//--------------------------------------------------------------
/**
 * �e�N�X�`�� VRAM�֓]�������f���ƃo�C���h
 * @param	rmdl	FRO_MDL
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_MDL_TexTransBind( FRO_MDL *rmdl )
{
	LoadVRAMTexture( rmdl->pResTex );
	NNS_G3dBindMdlSet( rmdl->pResMdlSet, rmdl->pResTex );
	rmdl->bind_tex_flag = TRUE;
}

//--------------------------------------------------------------
/**
 * �e�N�X�`�� VRAM�֓]�������f���ƃo�C���h TCB
 * @param	rmdl	FRO_MDL
 * @retval	nothing
 */
//--------------------------------------------------------------
static void VIntrTCB_MDL_TexTransBind( TCB_PTR tcb, void *wk )
{
	FRO_MDL *rmdl = wk;
	FRO_MDL_TexTransBind( rmdl );
	TCB_Delete( tcb );
	
	#ifdef DEBUG_FRO_VINTR_COUNT
	DEBUG_FroVIntrCount--;
	
	if( DEBUG_FroVIntrCount < 0 ){
		OS_Printf( "FRO_MDL VIntrTCB - ERROR\n" );
		GF_ASSERT( 0 );
	}
	
	#ifdef DEBUG_FRO_VINTR_COUNT_PRINT	
	OS_Printf( "FRO_MDL VIntrTCB CountDown = %d\n", DEBUG_FroVIntrCount );
	#endif
	
	#endif
}

//--------------------------------------------------------------
/**
 * �e�N�X�`���]��VBlank�^�X�N��ǉ�
 * VBlank����FRO_MDL_TexTransBind()�����s���������ł���^�X�N�B
 * @param	rmdl	FRO_MDL
 * @retval	nothing
 */
//--------------------------------------------------------------
#ifndef DEBUG_MDL_VINTR_BUG_PASS
void FRO_MDL_TexTransBindVTaskAdd( FRO_MDL *rmdl )
{
	#ifdef DEBUG_FRO_VINTR_COUNT
	{
		DEBUG_FroVIntrCount++;
	}
	#endif
	
	{
		TCB_PTR tcb = VIntrTCB_Add(
				VIntrTCB_MDL_TexTransBind, rmdl, VTCBPRI_FRO_TEXBIND );
		GF_ASSERT( tcb != NULL && "VIntrTCB_Add() TCB ADD NULL ERROR\n" );
	}
}
#else
void FRO_MDL_TexTransBindVTaskAdd( FRO_MDL *rmdl )
{
	#ifdef DEBUG_FRO_VINTR_COUNT
	{
		DEBUG_FroVIntrCount++;
	}
	#endif
	
	{
		TCB_PTR tcb = VIntrTCB_Add(
				VIntrTCB_MDL_TexTransBind, rmdl, VTCBPRI_FRO_TEXBIND );
	
		if( tcb == NULL ){
			FRO_MDL_TexTransBind( rmdl );
		
			#ifdef DEBUG_FRO_VINTR_COUNT
			OS_Printf( "FRO_MDL VIntrTCB Count = %d ", DEBUG_FroVIntrCount );
			#endif
		
			OS_Printf( "VIntrTCB_Add() TCB ADD NULL ERROR\n" );
		
			#ifdef DEBUG_FRO_VINTR_COUNT_ASSERT
			GF_ASSERT( 0 );
			#endif
		}
	}
	
	#ifdef DEBUG_FRO_VINTR_COUNT_PRINT	
	OS_Printf( "FRO_MDL VIntrTCB CountUp = %d\n", DEBUG_FroVIntrCount );
	#endif
}
#endif

//--------------------------------------------------------------
/**
 * FRO_MDL ���\�[�X�t�@�C���J��
 * @param	rmdl	FRO_MDL
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_MDL_ResHeaderFree( FRO_MDL *rmdl )
{
	if( rmdl->pResFileHeader ){
		sys_FreeMemoryEz( rmdl->pResFileHeader );
	}
}

//--------------------------------------------------------------
/**
 * FRO_MDL�@�e�N�X�`���J��
 * @param	rmdl	FRO_MDL
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_MDL_TexFree( FRO_MDL *rmdl )
{
	if( rmdl->pResTex != NULL ){
		NNSG3dTexKey texKey;
		NNSG3dTexKey tex4x4Key;
		NNSG3dPlttKey plttKey;
		
		NNS_G3dTexReleaseTexKey( rmdl->pResTex, &texKey, &tex4x4Key );
		NNS_GfdFreeTexVram( texKey );
		NNS_GfdFreeTexVram( tex4x4Key );
		
		plttKey = NNS_G3dPlttReleasePlttKey( rmdl->pResTex );
		NNS_GfdFreePlttVram( plttKey );
		rmdl->pResTex = NULL;
	}
}

//--------------------------------------------------------------
/**
 * FRO_MDL ���\�[�X�t�@�C���J�����e�N�X�`���J�������[�N�N���A
 * @param	rmdl	FRO_MDL
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_MDL_DeleteAll( FRO_MDL *rmdl )
{
	FRO_MDL_TexFree( rmdl );
	FRO_MDL_ResHeaderFree( rmdl );
	FRO_MDL_Clear( rmdl );
}

//--------------------------------------------------------------
/**
 * FRO_MDL	�e�N�X�`���o�C���h�`�F�b�N
 * @param	rmdl
 * @retval	BOOL	TRUE=�o�C���h�ς�
 */
//--------------------------------------------------------------
BOOL FRO_MDL_TexBindCheck( FRO_MDL *rmdl )
{
	return( rmdl->bind_tex_flag );
}

//==============================================================================
//	FRO_ANM
//==============================================================================
//--------------------------------------------------------------
/**
 * FRO_ANM �N���A
 * @param	ranm	FRO_ANM
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_Clear( FRO_ANM *ranm )
{
	memset( ranm, 0, sizeof(FRO_ANM) );
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X���Z�b�g�@���\�[�X���C��
 * @param	ranm	FRO_ANM
 * @param	pRes	�A�j�����\�[�X
 * @param	idx		�Q�ƃ��\�[�X�C���f�b�N�X
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmResSetMain( FRO_ANM *ranm, void *pRes, u32 idx )
{
	FROAnm_AnmResSet( ranm, pRes, idx );
	ranm->status_bit |= ANMBIT_RES_MAIN;
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X���Z�b�g�@���\�[�X���p
 * @param	ranm	FRO_ANM
 * @param	pRes	�A�j�����\�[�X
 * @param	idx		�Q�ƃ��\�[�X�C���f�b�N�X
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmResSetSub( FRO_ANM *ranm, void *pRes, u32 idx )
{
	FROAnm_AnmResSet( ranm, pRes, idx );
	ranm->status_bit &= BITFLIP( ANMBIT_RES_MAIN );
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X���Z�b�g�B
 * ���ɃZ�b�g�ς�FRO_ANM���̃��\�[�X���p��FRO_ANM_AnmResSetSub()
 * @param	ranm	FRO_ANM
 * @param	m_ranm	���\�[�X�Z�b�g�ς�FRO_ANM
 * @param	idx		�Q�ƃ��\�[�X�C���f�b�N�X
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmResSetSame( FRO_ANM *ranm, FRO_ANM *m_ranm, u32 idx )
{
	void *pRes = m_ranm->pResAnm;
	FRO_ANM_AnmResSetSub( ranm, pRes, idx );
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X���A�[�J�C�u����ǂݍ��݁AFRO_ANM_AnmResSetMain()�ŃZ�b�g
 * @param	ranm	FRO_ANM
 * @param	idx		�擾����A�j�����\�[�X�C���f�b�N�X
 * @param	handle	�A�[�J�C�u�n���h��
 * @param	arc_idx	�A�[�J�C�u���A�j�����\�[�X�C���f�b�N�X
 * @param	heap_id	�q�[�vID
 * @param	alloc_type	ALLOC_TYPE
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmResSetArcLoad( FRO_ANM *ranm, u32 idx,
		ARCHANDLE *handle, u32 arc_idx, u32 heap_id, ALLOC_TYPE alloc_type )
{
	void *pRes = 
		ArcUtil_HDL_Load( handle, arc_idx, FALSE, heap_id, alloc_type );
	FRO_ANM_AnmResSetMain( ranm, pRes, idx );
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X�Z�b�g
 * @param	ranm	FRO_ANM
 * @param	pRes	�A�j�����\�[�X
 * @param	idx		�Q�ƃ��\�[�X�C���f�b�N�X
 * @retval
 */
//--------------------------------------------------------------
static void FROAnm_AnmResSet( FRO_ANM *ranm, void *pRes, u32 idx )
{
	FRO_ANM_Clear( ranm );
	ranm->pResAnm = pRes;
	ranm->pResAnmIdx = NNS_G3dGetAnmByIdx( ranm->pResAnm, 0 );
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X�J���B
 * FRO_ANM_AnmResSetMain()�Ŋm�ۂ����ꍇ�A���\�[�X���𕹂��ĊJ��
 * @param	ranm	FRO_ANM
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmResFree( FRO_ANM *ranm )
{
	if( (ranm->status_bit&ANMBIT_RES_MAIN) ){
		sys_FreeMemoryEz( ranm->pResAnm );
		ranm->status_bit &= BITFLIP( ANMBIT_RES_MAIN );
	}
	
	ranm->pResAnm = NULL;
	ranm->pResAnmIdx = NULL;
}

//--------------------------------------------------------------
/**
 * �I�u�W�F���\�[�X�m��
 * @param	ranm	FRO_ANM
 * @param	pMdl	NNSG3dResMdl
 * @param	heap_id	�q�[�vID
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AllocAnmObj( FRO_ANM *ranm, const NNSG3dResMdl *pMdl, u32 heap_id )
{
	sys_InitAllocator( &ranm->Allocator, heap_id, 4 );
	ranm->pAnmObj = NNS_G3dAllocAnmObj( &ranm->Allocator, ranm->pResAnmIdx, pMdl );
	GF_ASSERT( ranm->pAnmObj != NULL );
}

//--------------------------------------------------------------
/**
 * �I�u�W�F���\�[�X�m�� rmdl���̃��f�����g�p�����\�[�X�m��
 * @param	ranm	FRO_ANM
 * @param	rmdl	FRO_MDL
 * @param	heap_id	�q�[�vID
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AllocAnmObjInMdl( FRO_ANM *ranm, const FRO_MDL *rmdl, u32 heap_id )
{
	FRO_ANM_AllocAnmObj( ranm, rmdl->pResMdl, heap_id );
}

//--------------------------------------------------------------
/**
 * �I�u�W�F���\�[�X������
 * @param	ranm	FRO_ANM
 * @param	pResMdl	NNSG3dResMdl
 * @param	pResTex NNSG3dResTex
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmObjInit( FRO_ANM *ranm,
	const NNSG3dResMdl* pResMdl, const NNSG3dResTex* pResTex )
{
	NNS_G3dAnmObjInit( ranm->pAnmObj, ranm->pResAnmIdx, pResMdl, pResTex );
}

//--------------------------------------------------------------
/**
 * �I�u�W�F���\�[�X������ FRO_MDL�����\�[�X�g�p
 * @param	ranm	FRO_ANM
 * @param	rmdl	FRO_MDL
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmObjInitInMdl( FRO_ANM *ranm, const FRO_MDL *rmdl )
{
	FRO_ANM_AnmObjInit( ranm, rmdl->pResMdl, rmdl->pResTex );
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X���I�u�W�F���\�[�X�̈ꊇ�������B���\�[�X���C��
 * FRO_ANM_AnmResSetMain()�Ń��\�[�X���Z�b�g��
 * FRO_ANM_AllocAnmObjInMdlAlloc()�ŃA���P�[�^�쐬��ɃI�u�W�F���\�[�X�Z�b�g��
 * FRO_ANM_AnmObjInitInMdl()�ŃI�u�W�F�Ƀ��f�����Z�b�g
 * @param	ranm	FRO_ANM
 * @param	rmdl	FRO_MDL
 * @param	pRes	�A�j�����\�[�X
 * @param	idx		�Q�ƃ��\�[�X�C���f�b�N�X
 * @param	heap_id	�q�[�vID
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmResObjInitMain( FRO_ANM *ranm, const FRO_MDL *rmdl,
		void *pRes, u32 idx, u32 heap_id )
{
	FRO_ANM_AnmResSetMain( ranm, pRes, idx );
	FRO_ANM_AllocAnmObjInMdl( ranm, rmdl, heap_id );
	FRO_ANM_AnmObjInitInMdl( ranm, rmdl );
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X���I�u�W�F���\�[�X�̈ꊇ�������B���\�[�X�T�u
 * FRO_ANM_AnmResSetSub()�Ń��\�[�X���Z�b�g��
 * FRO_ANM_AllocAnmObjInMdlAlloc()�ŃA���P�[�^�쐬��ɃI�u�W�F���\�[�X�Z�b�g��
 * FRO_ANM_AnmObjInitInMdl()�ŃI�u�W�F�Ƀ��f�����Z�b�g
 * @param	ranm	FRO_ANM
 * @param	rmdl	FRO_MDL
 * @param	pRes	�A�j�����\�[�X
 * @param	idx		�Q�ƃ��\�[�X�C���f�b�N�X
 * @param	heap_id	�q�[�vID
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmResObjInitSub( FRO_ANM *ranm, const FRO_MDL *rmdl,
		void *pRes, u32 idx, u32 heap_id )
{
	FRO_ANM_AnmResSetSub( ranm, pRes, idx );
	FRO_ANM_AllocAnmObjInMdl( ranm, rmdl, heap_id );
	FRO_ANM_AnmObjInitInMdl( ranm, rmdl );
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X���I�u�W�F���\�[�X�̈ꊇ�������B���\�[�X�T�u
 * FRO_ANM_AnmResSetSame()�Ń��\�[�X���Z�b�g��
 * FRO_ANM_AllocAnmObjInMdlAlloc()�ŃA���P�[�^�쐬��ɃI�u�W�F���\�[�X�Z�b�g��
 * FRO_ANM_AnmObjInitInMdl()�ŃI�u�W�F�Ƀ��f�����Z�b�g
 * @param	ranm	FRO_ANM
 * @param	rmdl	FRO_MDL
 * @param	m_ranm	���\�[�X�Z�b�g�ς�FRO_ANM
 * @param	idx		�Q�ƃ��\�[�X�C���f�b�N�X
 * @param	heap_id	�q�[�vID
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmResObjInitSame( FRO_ANM *ranm, const FRO_MDL *rmdl,
		FRO_ANM *m_ranm, u32 idx, u32 heap_id )
{
	FRO_ANM_AnmResSetSame( ranm, m_ranm, idx );
	FRO_ANM_AllocAnmObjInMdl( ranm, rmdl, heap_id );
	FRO_ANM_AnmObjInitInMdl( ranm, rmdl );
}

//--------------------------------------------------------------
/**
 * �I�u�W�F���\�[�X�J��
 * @param	ranm	FRO_ANM
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_AnmObjFree( FRO_ANM *ranm )
{
	if( ranm->pAnmObj ){
		NNS_G3dFreeAnmObj( &ranm->Allocator, ranm->pAnmObj );
		ranm->pAnmObj = NULL;
	}
}

//--------------------------------------------------------------
/**
 * �A�j�����\�[�X���I�u�W�F���\�[�X�J���ƃ��[�N�N���A�B
 * FRO_ANM_AnmObjFree()��FRO_ANM_AnmResFree()���Ăт��A���[�N�N���A
 * @param	ranm	FRO_ANM
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_DeleteAll( FRO_ANM *ranm )
{
	FRO_ANM_AnmObjFree( ranm );
	FRO_ANM_AnmResFree( ranm );
	FRO_ANM_Clear( ranm );
}

//		MtxFx33 rot = { FX32_ONE, 0,0,0, FX32_ONE, 0,0,0,FX32_ONE};

//--------------------------------------------------------------
/**
 * �A�j���Đ�
 * @param	ranm	FRO_ANM
 * @param	speed	�Đ����x -speed=�t�Đ�
 * @param	loop	LOOPTYPE
 * @retval	BOOL	TRUE=�I�[�܂ōĐ� -spped=�擪�܂ōĐ�
 */
//--------------------------------------------------------------
BOOL FRO_ANM_Play( FRO_ANM *ranm, fx32 speed, ANMLOOPTYPE loop )
{
	u32 ret = FALSE;
	fx32 end = NNS_G3dAnmObjGetNumFrame( ranm->pAnmObj );
	
	ranm->frame += speed;
	
	if( speed > 0 ){
		if( ranm->frame >= end ){
			ret = TRUE;
			
			if( loop == ANMLOOP_ON ){
				ranm->frame -= end;
			}else{
				ranm->frame = end;
			}
		}
	}else{
		if( ranm->frame <= 0 ){
			ret = TRUE;
			
			if( loop == ANMLOOP_ON ){
				ranm->frame += end;
			}else{
				ranm->frame = 0;
			}
		}
	}
	
	NNS_G3dAnmObjSetFrame( ranm->pAnmObj, ranm->frame );
	
	if( ret == TRUE ){
		ranm->status_bit |= ANMBIT_ANM_END;
	}else{
		ranm->status_bit &= BITFLIP( ANMBIT_ANM_END );
	}
	
	return( ret );
}

//--------------------------------------------------------------
/**
 * �Đ��t���[���Z�b�g
 * @param	ranm	FRO_ANM
 * @param	frame	�t���[����
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_ANM_FrameSet( FRO_ANM *ranm, fx32 frame )
{
	ranm->frame = frame;
}

//--------------------------------------------------------------
/**
 * ���݂̍Đ��t���[���擾
 * @param	ranm	FRO_ANM
 * @retval	fx32	�t���[����
 */
//--------------------------------------------------------------
fx32 FRO_ANM_FrameGet( const FRO_ANM *ranm )
{
	return( ranm->frame );
}

//--------------------------------------------------------------
/**
 * �A�j���̍ő�t���[�����擾
 * @param	ranm	FRO_ANM
 * @retval	fx32	�t���[����
 */
//--------------------------------------------------------------
fx32 FRO_ANM_MaxFrameGet( const FRO_ANM *ranm )
{
	GF_ASSERT( ranm->pAnmObj != NULL );
	{
		fx32 end = NNS_G3dAnmObjGetNumFrame( ranm->pAnmObj );
		return( end );
	}
}

//--------------------------------------------------------------
/**
 * �A�j���I���t���O�`�F�b�N
 * @param	ranm	FRO_ANM
 * @retval	BOOL	TRUE=�A�j���I��
 */
//--------------------------------------------------------------
BOOL FRO_ANM_AnmEndCheck( const FRO_ANM *ranm )
{
	if( (ranm->status_bit&ANMBIT_ANM_END) ){
		return( TRUE );
	}
	return( FALSE );
}

//--------------------------------------------------------------
/**
 * FRO_ANM �A�j�����\�[�X��Ԃ�
 * @param	ranm FRO_ANM
 * @param	pResAnmData
 * @retval	nothing
 */
//--------------------------------------------------------------
void * FRO_ANM_ResAnmGet( FRO_ANM *ranm )
{
	return( ranm->pResAnm );
}

//==============================================================================
//	FRO_OBJ
//==============================================================================
//--------------------------------------------------------------
/**
 * FRO_OBJ ���[�N�N���A
 * @param	robj	FRO_OBJ
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_Clear( FRO_OBJ *robj )
{
	memset( robj, 0, sizeof(FRO_OBJ) );
}

//--------------------------------------------------------------
/**
 * �����_�����OOBJ������
 * @param	robj	FRO_OBJ
 * @param	pResMdl	NNSG3dResMdl
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_Init( FRO_OBJ *robj, NNSG3dResMdl *pResMdl )
{
	FRO_OBJ_Clear( robj );
	NNS_G3dRenderObjInit( &robj->RenderObj, pResMdl );
#if 0	
	robj->scale.x = FX32_ONE;
	robj->scale.y = FX32_ONE;
	robj->scale.z = FX32_ONE;
	MTX_Identity33( &robj->rotate );
#endif
}

//--------------------------------------------------------------
/**
 * �����_�����OOBJ������ FRO_MDL�����f���ƃ����N
 * @param	robj	FRO_OBJ
 * @param	rmdl	FRO_MDL
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_InitInMdl( FRO_OBJ *robj, FRO_MDL *rmdl )
{
	FRO_OBJ_Init( robj, rmdl->pResMdl );
}

//--------------------------------------------------------------
/**
 * �A�j���ǉ�
 * @param	robj	FRO_OBJ
 * @param	pAnmObj	NNSG3dAnmObj
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_AddAnm( FRO_OBJ *robj, NNSG3dAnmObj *pAnmObj )
{
	NNS_G3dRenderObjAddAnmObj( &robj->RenderObj, pAnmObj );
}

//--------------------------------------------------------------
/**
 * �A�j���폜
 * @param	robj	FRO_OBJ
 * @param	pAnmObj	NNSG3dAnmObj
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_DelAnm( FRO_OBJ *robj, NNSG3dAnmObj *pAnmObj )
{
	NNS_G3dRenderObjRemoveAnmObj( &robj->RenderObj, pAnmObj );
}

//--------------------------------------------------------------
/**
 * �A�j���ǉ��@FRO_ANM���A�j���I�u�W�F�g�p
 * @param	robj	FRO_OBJ
 * @param	ranm	FRO_ANM
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_AddAnmInAnm( FRO_OBJ *robj, FRO_ANM *ranm )
{
	FRO_OBJ_AddAnm( robj, ranm->pAnmObj );
}

//--------------------------------------------------------------
/**
 * �A�j���폜�@FRO_ANM���A�j���I�u�W�F�g�p
 * @param	robj	FRO_OBJ
 * @param	ranm	FRO_ANM
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_DelAnmInAnm( FRO_OBJ *robj, FRO_ANM *ranm )
{
	FRO_OBJ_DelAnm( robj, ranm->pAnmObj );
}

//--------------------------------------------------------------
/**
 * OBJ���������A�j���ǉ��BFRO_MDL,FRO_ANM�����\�[�X���Q�Ƃ�
 * FRO_OBJ_InitInMdl(),FRO_OBJ_AddAnmInAnm()
 * @param	robj	FRO_OBJ
 * @param	rmdl	FRO_MDL
 * @param	ranm	FRO_ANM
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_InitAll( FRO_OBJ *robj, FRO_MDL *rmdl, FRO_ANM *ranm )
{
	FRO_OBJ_InitInMdl( robj, rmdl );
	FRO_OBJ_AddAnmInAnm( robj, ranm );
}

//--------------------------------------------------------------
/**
 * OBJ�`��
 * @param	robj	FRO_OBJ
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_Draw( FRO_OBJ *robj,
	const VecFx32 *pos, const VecFx32 *scale, const MtxFx33 *rotate )
{
	simple_3DModelDraw( &robj->RenderObj, pos, rotate, scale );
}

//--------------------------------------------------------------
/**
 * OBJ�`��@���W�w��̂�
 * @param	robj	FRO_OBJ
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_DrawPos( FRO_OBJ *robj, const VecFx32 *pos )
{
	FRO_OBJ_Draw( robj, pos, &DATA_OneScaleFx32, &DATA_ZeroRotateFx33 );
}

#if 0
void FRO_OBJ_DrawPos( FRO_OBJ *robj, VecFx32 *pos )
{
	VecFx32 sc = { FX32_ONE, FX32_ONE, FX32_ONE }; 
#if 0
	MtxFx33 rot = { FX32_ONE, 0,0,0, FX32_ONE, 0,0,0,FX32_ONE};
#elif 0
	MtxFx33 rot,calc_rot;
	MTX_Identity33( &rot );
	MTX_RotX33( &calc_rot, FX_SinIdx(0), FX_CosIdx(0) );
	MTX_Concat33( &calc_rot, &rot, &rot );
	MTX_RotY33( &calc_rot, FX_SinIdx(0), FX_CosIdx(0) );
	MTX_Concat33( &calc_rot, &rot, &rot );
	MTX_RotZ33( &calc_rot, FX_SinIdx(0), FX_CosIdx(0) );
	MTX_Concat33( &calc_rot, &rot, &rot );
#else
	MtxFx33 rot;
//	Rot360_to_MtxFx33( &rot, 0, 0, 0 );
	MTX_Identity33( &rot );
#endif
	FRO_OBJ_Draw( robj, pos, &sc, &rot );
}
#endif

//--------------------------------------------------------------
/**
 * OBJ�`��@�g�k���A�s��p�x�w��
 * @param	robj	FRO_OBJ
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_DrawScaleRotate( FRO_OBJ *robj,
	const VecFx32 *pos, const VecFx32 *scale, const ROTATE *rotate )
{
	MtxFx33 rot;
	Rot360_to_MtxFx33( &rot, rotate->x, rotate->y, rotate->z );
	FRO_OBJ_Draw( robj, pos, scale, &rot );
}

//--------------------------------------------------------------
/**
 * OBJ�`��@��]�s��w��
 * @param	robj	FRO_OBJ
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_DrawRotate33(
	FRO_OBJ *robj, const VecFx32 *pos, const MtxFx33 *rotate )
{
	FRO_OBJ_Draw( robj, pos, &DATA_OneScaleFx32, rotate );
}

//--------------------------------------------------------------
/**
 * OBJ�`��@�s��p�x�w��
 * @param	robj	FRO_OBJ
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_DrawRotate(
	FRO_OBJ *robj, const VecFx32 *pos, const ROTATE *rotate )
{
	MtxFx33 rot;
	Rot360_to_MtxFx33( &rot, rotate->x, rotate->y, rotate->z );
	FRO_OBJ_Draw( robj, pos, &DATA_OneScaleFx32, &rot );
}

#if 0
void FRO_OBJ_DrawRotate( FRO_OBJ *robj, VecFx32 *pos, VecFx32 *scale, const ROTATE *rotate )
{
	MtxFx33 rot;
	Rot360_to_MtxFx33( &rot, rotate->x, rotate->y, rotate->z );
	FRO_OBJ_Draw( robj, pos, scale, &rot );
}
#endif

//--------------------------------------------------------------
/**
 * OBJ�`��
 * @param	robj	FRO_OBJ
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_Draw1Mat1Shape( FRO_OBJ *robj, const VecFx32 *pos, const VecFx32 *scale, const MtxFx33 *rotate )
{
	simple_3DModelDraw_1mat1shape( &robj->RenderObj, pos, rotate, scale );
}

//==============================================================================
//	�p�[�c
//==============================================================================
#if 0
//--------------------------------------------------------------
/**
 * ��]�p�x0-360�����]�s��Z�b�g
 * @param	robj	FRO_OBJ
 * @retval
 */
//--------------------------------------------------------------
void FRO_RotateMtx( MtxFx33 *, u32 x, u32 y, u32 z )
{
	MTX_Identity33( rotate );
	Rot360_to_MtxFx33( rotate, x, y, z );
}
#endif

//==============================================================================
//	debug
//==============================================================================
//----
#if 0
//----
//--------------------------------------------------------------
/**
 * VecFx32�^�̕\�����W�|�C���^�擾
 * @param	robj	FRO_OBJ
 * @retval	VecFx32*
 */
//--------------------------------------------------------------
VecFx32 * FRO_OBJ_pMatrixGet( FRO_OBJ *robj )
{
	return( &robj->matrix );
}

//--------------------------------------------------------------
/**
 * �\�����W�Z�b�g
 * @param	robbj	FRO_OBJ
 * @param	mtx		�\�����W
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_MatrixSet( FRO_OBJ *robj, const VecFx32 *mtx )
{
	robj->matrix = *mtx;
}

//--------------------------------------------------------------
/**
 * ��\���t���O�`�F�b�N
 * @param	robj	FRO_OBJ
 * @retval	flag	TRUE=��\��
 */
//--------------------------------------------------------------
BOOL FRO_OBJ_VanishCheck( const FRO_OBJ *robj )
{
	return( robj->vanish );
}

//--------------------------------------------------------------
/**
 * ��\���t���O�Z�b�g
 * @param	robj	FRO_OBJ
 * @param	flag	TRUE=��\��
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_VanishSet( FRO_OBJ *robj, BOOL flag )
{
	robj->vanish = flag;
}

//--------------------------------------------------------------
/**
 * �g�k���|�C���^�擾
 * @param	robj	FRO_OBJ
 * @retval	VecFx32*
 */
//--------------------------------------------------------------
VecFx32 * FRO_OBJ_pScaleGet( FRO_OBJ *robj )
{
	return( &robj->scale );
}

//--------------------------------------------------------------
/**
 * �g�k���Z�b�g
 * @param	robj	FRO_OBJ
 * @param	scale
 * @retval	nothing
 */
//--------------------------------------------------------------
void FRO_OBJ_ScaleSet( FRO_OBJ *robj, const VecFx32 *scale )
{
	robj->scale = *scale;
}

//--------------------------------------------------------------
/**
 * ��]�s��|�C���^�擾
 * @param	robj	FRO_OBJ
 * @retval	ROTET
 */
//--------------------------------------------------------------
MtxFx33 * FRO_OBJ_pRotateGet( FRO_OBJ *robj )
{
	return( &robj->rotate );
}

//--------------------------------------------------------------
/**
 * ��]�s��Z�b�g
 * @param	robj	FRO_OBJ
 * @retval
 */
//--------------------------------------------------------------
void FRO_OBJ_RotateSet( FRO_OBJ *robj, const MtxFx33 *rotate )
{
	robj->rotate = *rotate;
}

//--------------------------------------------------------------
/**
 * ��]�p�x0-360�����]�s��Z�b�g
 * @param	robj	FRO_OBJ
 * @retval
 */
//--------------------------------------------------------------
void FRO_OBJ_Rotate360Set( FRO_OBJ *robj, const ROTATE *r360 )
{
	MtxFx33 rotate;
	MTX_Identity33( &rotate );
	Rot360_to_MtxFx33( &rotate, r360->x, r360->y, r360->z );
	robj->rotate = rotate;
}
//----
#endif
//----

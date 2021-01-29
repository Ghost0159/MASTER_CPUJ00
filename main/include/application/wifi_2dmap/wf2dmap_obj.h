//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_obj.h
 *	@brief		�I�u�W�F�N�g�f�[�^�Ǘ�
 *	@author		tomoya takahashi
 *	@data		2007.03.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WF2DMAP_OBJ_H__
#define __WF2DMAP_OBJ_H__

#include "application/wifi_2dmap/wf2dmap_common.h"
#include "application/wifi_2dmap/wf2dmap_objst.h"

#undef GLOBAL
#ifdef	__WF2DMAP_OBJ_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	��Ԓ萔
//=====================================
typedef enum {
	WF2DMAP_OBJST_NONE = WF2DMAP_OBJST_C_NONE,
	WF2DMAP_OBJST_TURN = WF2DMAP_OBJST_C_TURN,
	WF2DMAP_OBJST_WALK = WF2DMAP_OBJST_C_WALK,
	WF2DMAP_OBJST_RUN =  WF2DMAP_OBJST_C_RUN,
	WF2DMAP_OBJST_BUSY = WF2DMAP_OBJST_C_BUSY,
	WF2DMAP_OBJST_WALLWALK = WF2DMAP_OBJST_C_WALLWALK,
	WF2DMAP_OBJST_SLOWWALK = WF2DMAP_OBJST_C_SLOWWALK,
	WF2DMAP_OBJST_WALK4	= WF2DMAP_OBJST_C_WALK4,
	WF2DMAP_OBJST_STAYWALK2 = WF2DMAP_OBJST_C_STAYWALK2,
	WF2DMAP_OBJST_STAYWALK4 = WF2DMAP_OBJST_C_STAYWALK4,
	WF2DMAP_OBJST_STAYWALK8 = WF2DMAP_OBJST_C_STAYWALK8,
	WF2DMAP_OBJST_STAYWALK16 = WF2DMAP_OBJST_C_STAYWALK16,
	WF2DMAP_OBJST_NUM =  WF2DMAP_OBJST_C_NUM,
} WF2DMAP_OBJST;

//-------------------------------------
///	���[�N�̒l�萔
//=====================================
typedef enum {
	WF2DMAP_OBJPM_X,		// x���W
	WF2DMAP_OBJPM_Y,		// y���W
	WF2DMAP_OBJPM_LX,		// �O��x���W
	WF2DMAP_OBJPM_LY,		// �O��y���W
	WF2DMAP_OBJPM_PLID,		// �v���C���[ID
	WF2DMAP_OBJPM_ST,		// ���
	WF2DMAP_OBJPM_WAY,		// ����
	WF2DMAP_OBJPM_CHARA,	// �L�����N�^�[
	WF2DMAP_OBJPM_FRAME,	// ����t���[��
	WF2DMAP_OBJPM_ENDFRAME,	// �I������t���[��
	WF2DMAP_OBJPM_STFRAME,	// ���̏�ԂɂȂ��ĉ��t���[�������Ă��邩
} WF2DMAP_OBJPARAM;


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�I�u�W�F�N�g���[�N
//=====================================
typedef struct _WF2DMAP_OBJWK WF2DMAP_OBJWK;

//-------------------------------------
///	�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
//=====================================
typedef struct _WF2DMAP_OBJSYS WF2DMAP_OBJSYS;


//-------------------------------------
///	�I�u�W�F�N�g�o�^�f�[�^
//=====================================
typedef struct {
	s16 x;		// x���W
	s16 y;		// y���W
	u16 playid;	// �v���C���[����ID	�iAID�Ȃǂ��w��j
	u16 status;	// ���������(WF2DMAP_OBJST)
	u16 way;		// ����(WF2DMAP_WAY)
	u16 charaid;// �L�����N�^�[ID
} WF2DMAP_OBJDATA;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

// �V�X�e���Ǘ�
GLOBAL WF2DMAP_OBJSYS* WF2DMAP_OBJSysInit( u32 objnum, u32 heapID );
GLOBAL void WF2DMAP_OBJSysExit( WF2DMAP_OBJSYS* p_sys );

// ���상�C��
GLOBAL void WF2DMAP_OBJSysCmdSet( WF2DMAP_OBJSYS* p_objsys, const WF2DMAP_ACTCMD* cp_cmd );
GLOBAL void WF2DMAP_OBJSysMain( WF2DMAP_OBJSYS* p_objsys );


GLOBAL u32 WF2DMAP_OBJSysObjNumGet( const WF2DMAP_OBJSYS* cp_sys );

// ���[�N�쐬�j��
GLOBAL WF2DMAP_OBJWK* WF2DMAP_OBJWkNew( WF2DMAP_OBJSYS* p_sys, const WF2DMAP_OBJDATA* cp_data );
GLOBAL void WF2DMAP_OBJWkDel( WF2DMAP_OBJWK* p_wk );


// �v���C���[ID�̃I�u�W�F�N�g�擾
GLOBAL WF2DMAP_OBJWK* WF2DMAP_OBJWkGet( WF2DMAP_OBJSYS* p_sys, u16 playid );
GLOBAL WF2DMAP_OBJWK* WF2DMAP_OBJWkIdxGet( WF2DMAP_OBJSYS* p_sys, u16 idx );
GLOBAL const WF2DMAP_OBJWK* WF2DMAP_OBJWkConstGet( const WF2DMAP_OBJSYS* cp_sys, u16 playid );
GLOBAL const WF2DMAP_OBJWK* WF2DMAP_OBJWkConstIdxGet( const WF2DMAP_OBJSYS* cp_sys, u16 idx );

// �f�[�^�擾�ݒ�
GLOBAL s32 WF2DMAP_OBJWkDataGet( const WF2DMAP_OBJWK* cp_wk, WF2DMAP_OBJPARAM id );
GLOBAL void WF2DMAP_OBJWkDataSet( WF2DMAP_OBJWK* p_wk, WF2DMAP_OBJPARAM id, s32 data );
GLOBAL void WF2DMAP_OBJWkDataAdd( WF2DMAP_OBJWK* p_wk, WF2DMAP_OBJPARAM id, s32 data );
GLOBAL WF2DMAP_POS WF2DMAP_OBJWkMatrixGet( const WF2DMAP_OBJWK* cp_wk );
GLOBAL WF2DMAP_POS WF2DMAP_OBJWkLastMatrixGet( const WF2DMAP_OBJWK* cp_wk );
GLOBAL WF2DMAP_POS WF2DMAP_OBJWkFrameMatrixGet( const WF2DMAP_OBJWK* cp_wk );
GLOBAL void WF2DMAP_OBJWkMatrixSet( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos );
GLOBAL void WF2DMAP_OBJWkLastMatrixSet( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos );

// �����蔻��֌W
GLOBAL const WF2DMAP_OBJWK* WF2DMAP_OBJSysHitCheck( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_OBJSYS* cp_objsys, WF2DMAP_WAY way );
GLOBAL const WF2DMAP_OBJWK* WF2DMAP_OBJSysPosHitCheck( const WF2DMAP_OBJSYS* cp_objsys, WF2DMAP_POS pos );


// �c�[����
GLOBAL WF2DMAP_POS WF2DMAP_OBJToolWayPosGet( WF2DMAP_POS pos, WF2DMAP_WAY way );
GLOBAL WF2DMAP_WAY WF2DMPA_OBJToolRetWayGet( WF2DMAP_WAY way );

#undef	GLOBAL
#endif		// __WF2DMAP_OBJ_H__


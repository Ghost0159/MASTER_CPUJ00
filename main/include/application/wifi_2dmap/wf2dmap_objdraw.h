//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_objdraw.h
 *	@brief		�I�u�W�F�N�g�f�[�^�\�����W���[��
 *	@author		tomoya takahashi
 *	@data		2007.03.16
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WF2DMAP_OBJDRAW_H__
#define __WF2DMAP_OBJDRAW_H__

#include "application/wifi_2dmap/wf2dmap_common.h"
#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wifi_2dchar.h"

#undef GLOBAL
#ifdef	__WF2DMAP_OBJDRAW_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�\���V�X�e�����[�N
//=====================================
typedef struct _WF2DMAP_OBJDRAWSYS WF2DMAP_OBJDRAWSYS;

//-------------------------------------
///	�\�����[�N
//=====================================
typedef struct _WF2DMAP_OBJDRAWWK WF2DMAP_OBJDRAWWK;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

// �V�X�e������
// Init_Shadow�A�̃p���b�g���\�[�X�͎�l���̂��̂��g�p���邽�߁A���������Ɏ�l���̃f�[�^�͓ǂݍ��݂��s���܂�
GLOBAL WF2DMAP_OBJDRAWSYS* WF2DMAP_OBJDrawSysInit( CLACT_SET_PTR p_clset, PALETTE_FADE_PTR p_pfd, u32 objnum, u32 draw_type, u32 heapID );
GLOBAL WF2DMAP_OBJDRAWSYS* WF2DMAP_OBJDrawSysInit_Shadow( CLACT_SET_PTR p_clset, PALETTE_FADE_PTR p_pfd, u32 objnum, u32 hero_charid, WF_2DC_MOVETYPE hero_movetype,  u32 draw_type,u32 heapID );
GLOBAL void WF2DMAP_OBJDrawSysExit( WF2DMAP_OBJDRAWSYS* p_sys );

// �I�u�W�F�N�g�o�^��{�ݒ�ύX
GLOBAL void WF2DMAP_OBJDrawSysDefBgPriSet( WF2DMAP_OBJDRAWSYS* p_sys, u32 bg_pri );
GLOBAL u32 WF2DMAP_OBJDrawSysDefBgPriGet( const WF2DMAP_OBJDRAWSYS* cp_sys );

// �O���t�B�b�N�f�[�^�̓o�^
// WF_2DC_MOVETYPE��include/applicaton/wifi_2dmap/wifi_2dchar.h�ɒ�`����Ă��܂�
GLOBAL void WF2DMAP_OBJDrawSysResSet( WF2DMAP_OBJDRAWSYS* p_sys, u32 charaid, WF_2DC_MOVETYPE movetype, u32 heapID );
GLOBAL void WF2DMAP_OBJDrawSysResDel( WF2DMAP_OBJDRAWSYS* p_sys, u32 charaid );
GLOBAL BOOL WF2DMAP_OBJDrawSysResCheck( const WF2DMAP_OBJDRAWSYS* cp_sys, u32 charaid );
GLOBAL void WF2DMAP_OBJDrawSysAllResDel( WF2DMAP_OBJDRAWSYS* p_sys );

// ���j�I���O���t�B�b�N�o�^
// ���j�I�����\�[�X�́Aboy1 boy3 man3 badman explore fighter gorggeousm mystery girl1 girl2 woman2 woman3 idol lady cowgirl gorggeousw �̂P�U��
GLOBAL void WF2DMAP_OBJDrawSysUniResSet( WF2DMAP_OBJDRAWSYS* p_sys, WF_2DC_MOVETYPE movetype, u32 heap );
GLOBAL void WF2DMAP_OBJDrawSysUniResDel( WF2DMAP_OBJDRAWSYS* p_sys );

// �I�u�W�F�N�g�o�^
GLOBAL WF2DMAP_OBJDRAWWK* WF2DMAP_OBJDrawWkNew( WF2DMAP_OBJDRAWSYS* p_sys, const WF2DMAP_OBJWK* cp_objwk, BOOL hero, u32 heapID );
GLOBAL void WF2DMAP_OBJDrawWkDel( WF2DMAP_OBJDRAWWK* p_wk );

// �I�u�W�F�N�g�\���f�[�^�X�V	�ʏ�͂���ł悢
GLOBAL void WF2DMAP_OBJDrawSysUpdata( WF2DMAP_OBJDRAWSYS* p_sys );
// �I�u�W�F�N�g�\���f�[�^�X�ɍX�V
GLOBAL void WF2DMAP_OBJDrawWkUpdata( WF2DMAP_OBJDRAWWK* p_wk );

// �I�u�W�F�N�g�X�V��ONOFF�t���O�ݒ�@�擾
GLOBAL void WF2DMAP_OBJDrawWkUpdataFlagSet( WF2DMAP_OBJDRAWWK* p_wk, BOOL flag );
GLOBAL BOOL WF2DMAP_OBJDrawWkUpdataFlagGet( const WF2DMAP_OBJDRAWWK* cp_wk );

// ����G�t�F�N�g����
// WF2DMAP_OBJDrawWkUpdataFlagSet�ŃI�u�W�F�N�g�X�V��OFF���Ă����Ȃ��ƈӖ�������܂���
// AnimeEnd�����s����܂ł����Ɠ��삵�܂�
GLOBAL void WF2DMAP_OBJDrawWkKuruAnimeStart( WF2DMAP_OBJDRAWWK* p_wk );
GLOBAL void WF2DMAP_OBJDrawWkKuruAnimeMain( WF2DMAP_OBJDRAWWK* p_wk );
GLOBAL void WF2DMAP_OBJDrawWkKuruAnimeEnd( WF2DMAP_OBJDRAWWK* p_wk );

// ���W��ݒ�
// WF2DMAP_OBJDrawWkUpdataFlagSet�ŃI�u�W�F�N�g�X�V��OFF���Ă����Ȃ��ƈӖ�������܂���
GLOBAL WF2DMAP_POS WF2DMAP_OBJDrawWkMatrixGet( const WF2DMAP_OBJDRAWWK* cp_wk );
GLOBAL void WF2DMAP_OBJDrawWkMatrixSet( WF2DMAP_OBJDRAWWK* p_wk, WF2DMAP_POS pos );

// �\���E��\��
GLOBAL BOOL WF2DMAP_OBJDrawWkDrawFlagGet( const WF2DMAP_OBJDRAWWK* cp_wk );
GLOBAL void WF2DMAP_OBJDrawWkDrawFlagSet( WF2DMAP_OBJDRAWWK* p_wk, BOOL flag );

// �\���D�揇�ʂ̎擾
GLOBAL u16 WF2DMAP_OBJDrawWkDrawPriGet( const WF2DMAP_OBJDRAWWK* cp_wk );

// �\����Ԃ̐ݒ�
// WF2DMAP_OBJDrawWkUpdataFlagSet�ŃI�u�W�F�N�g�X�V��OFF���Ă����Ȃ��ƈӖ�������܂���
GLOBAL void WF2DMAP_OBJDrawWkWaySet( WF2DMAP_OBJDRAWWK* p_wk, WF2DMAP_WAY way );

// �\���D�揇�ʎ擾
GLOBAL u32 WF2DMAP_OBJDrawWkDrawPriCalc( s16 y, BOOL hero );

// �p���b�g�i���o�[�̎擾
GLOBAL u32 WF2DMAP_OBJDrawWkPaletteNoGet( const WF2DMAP_OBJDRAWWK* cp_wk );

// �A�̍��W��ݒ�
GLOBAL void WF2DMAP_OBJDrawWkShadowMatrixSet( WF2DMAP_OBJDRAWWK* p_wk, WF2DMAP_POS pos );



#undef	GLOBAL
#endif		// __WF2DMAP_OBJDRAW_H__


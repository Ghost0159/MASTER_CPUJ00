//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_scrdraw.h
 *	@brief		�X�N���[���`��ݒ菈��
 *	@author		tomoya takahshi
 *	@data		2007.03.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WF2DMAP_SCRDRAW_H__
#define __WF2DMAP_SCRDRAW_H__

#include "application/wifi_2dmap/wf2dmap_common.h"
#include "application/wifi_2dmap/wf2dmap_map.h"
#include "application/wifi_2dmap/wf2dmap_scroll.h"
#include "system/clact_util.h"
#include "gflib/bg_system.h"

#undef GLOBAL
#ifdef	__WF2DMAP_SCRDRAW_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif


//-----------------------------------------------------------------------------
/**
 *			�g�p����
 *
 *			�a�f��
 *			�P�F�X�N���[���o�b�t�@��0x1000�T�C�Y�g�p���܂��B
 *			�Q�FVBlank����GF_BGL_VBlankFunc(...)��K���Ă�ł�������
 *			�n�`�l
 *			�P�Fclact_util�̃f�t�H���g�����_���[�ŏ��������Ă���
 */
//-----------------------------------------------------------------------------




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
///	�X�N���[���`�惏�[�N
//=====================================
typedef struct _WF2DMAP_SCRDRAW WF2DMAP_SCRDRAW;

//-------------------------------------
///	�X�N���[���`��V�X�e���������f�[�^
//	�X�N���[��������T�[�t�F�[�X�ݒ�
//	�X�N���[������������a�f�ʃR���g���[���ݒ�
//=====================================
typedef struct {
	u8	surface_type;	// �T�[�t�F�[�X�ʁiCLACT_U_EASYRENDER_SURFACE_MAIN/CLACT_U_EASYRENDER_SURFACE_SUB�j
	u8	bg_frame;		// BG�t���[����
	u8	colorMode;		// �J���[���[�h(GX_BG_COLORMODE_16�Ȃ�)
	u8	screenBase;		// �X�N���[���x�[�X(GX_BG_SCRBASE_0xe000�Ȃ�)
	u8	charbase;		// �L�����N�^�x�[�X(GX_BG_CHARBASE_0x00000�Ȃ�)
    u8	bgExtPltt;		//�a�f�g���p���b�g�X���b�g�I��(GX_BG_EXTPLTT_01�Ȃ�)
	u8	priority;		//�\���v���C�I���e�B�[
	u8	mosaic;			//���U�C�N�ݒ�
	u8	arcid_scrn;		// �ǂݍ��ރX�N���[���f�[�^�̃A�[�J�C�uID
	u8	dataid_scrn;	// �ǂݍ��ރX�N���[���f�[�^�̃f�[�^ID
	u8	arcdata_comp;	// �ǂݍ��ރX�N���[���f�[�^�����k����Ă��邩
} WF2DMAP_SCRDRAWINIT;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
// �V�X�e���쐬
GLOBAL WF2DMAP_SCRDRAW* WF2DMAP_SCRDrawSysInit( CLACT_U_EASYRENDER_DATA* p_render, GF_BGL_INI* p_bgl, const WF2DMAP_SCRDRAWINIT* cp_init, u32 heapID );
GLOBAL void WF2DMAP_SCRDrawSysExit( WF2DMAP_SCRDRAW* p_sys );

// �X�N���[�����f
GLOBAL void WF2DMAP_SCRDrawSysMain( WF2DMAP_SCRDRAW* p_sys, const WF2DMAP_SCROLL* cp_scroll );

#undef	GLOBAL
#endif		// __WF2DMAP_SCRDRAW_H__


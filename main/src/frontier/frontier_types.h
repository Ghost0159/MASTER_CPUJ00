//==============================================================================
/**
 * @file	frontier_types.h
 * @brief	�t�����e�B�A�֘A�̕s��`�|�C���^�ȂǁA�ǂ��ł��g�p����悤�Ȃ��̂̒�`
 * @author	matsuda
 * @date	2007.04.06(��)
 */
//==============================================================================
#ifndef __FRONTIER_TYPES_H__
#define __FRONTIER_TYPES_H__

#include "system/clact_tool.h"
#include "fs_usescript.h"


//==============================================================================
//	�萔��`
//==============================================================================
///	�a�f�p���b�g��` 2007.04.06(��)
//
//	0 �` 10  : �t�B�[���h�}�b�v�a�f�p
//  11      : ���b�Z�[�W�E�C���h�E
//  12      : ���j���[�E�C���h�E
//  13      : ���b�Z�[�W�t�H���g
//  14      : �V�X�e���t�H���g
//	15		: ���g�p�i���[�J���C�Y�p�j
#define FFD_FIELD_PAL_START  (0)			//  �t�B�[���h�}�b�vBG�p���b�g�J�n
#define FFD_FIELD_PAL_END    (10)			//  �t�B�[���h�}�b�vBG�p���b�g�I��
#define FFD_MESFRAME_PAL     ( 11 )         //  ���b�Z�[�W�E�C���h�E
#define FFD_MENUFRAME_PAL    ( 12 )         //  ���j���[�E�C���h�E
#define FFD_MESFONT_PAL      ( 13 )         //  ���b�Z�[�W�t�H���g
#define FFD_SYSFONT_PAL	     ( 14 )         //  �V�X�e���t�H���g
#define FFD_LOCALIZE_PAL     ( 15 )         //	���g�p�i���[�J���C�Y�p�j

#define FFD_FIELD_PAL_SIZE	((FFD_FIELD_PAL_END - FFD_FIELD_PAL_START + 1) * 0x20)

/*********************************************************************************************
	���C����ʂ�CGX����U��		2007.04.06(��)

		�E�B���h�E�g	�F	409 - 511
			��b�A���j���[�A�n���A�Ŕ�

		BMP�E�B���h�E�P	�F	297 - 408
			��b

		BMP�E�B���h�E�Q	�F	0 - 296
			���j���[�i�ő�j�A�͂�/������

*********************************************************************************************/

/*********************************************************************************************
	�E�B���h�E�g
*********************************************************************************************/
// ��b�E�B���h�E�L����
#define	FR_TALK_WIN_CGX_SIZE	( 18+12 )
#define	FR_TALK_WIN_CGX_NUM		( 1024 - FR_TALK_WIN_CGX_SIZE )
#define	FR_TALK_WIN_PAL			( FFD_MESFRAME_PAL )

// ���j���[�E�B���h�E�L����
#define	FR_MENU_WIN_CGX_SIZE	( 9 )
#define	FR_MENU_WIN_CGX_NUM	( FR_TALK_WIN_CGX_NUM - FR_MENU_WIN_CGX_SIZE )
#define	FR_MENU_WIN_PAL		( FFD_MENUFRAME_PAL )

/*********************************************************************************************
	BMP�E�B���h�E
*********************************************************************************************/
// ��b�E�B���h�E�i���C���j
#define	FFD_MSG_WIN_PX		( 2 )
#define	FFD_MSG_WIN_PY		( 19 )
#define	FFD_MSG_WIN_SX		( 27 )
#define	FFD_MSG_WIN_SY		( 4 )
#define	FFD_MSG_WIN_PAL		( FFD_MESFONT_PAL )
#define	FFD_MSG_WIN_CGX		( FR_MENU_WIN_CGX_NUM - ( FFD_MSG_WIN_SX * FFD_MSG_WIN_SY ) )

// ���j���[�E�B���h�E�i���C���j
#define	FFD_MENU_WIN_PX		( 20 )
#define	FFD_MENU_WIN_PY		( 1 )
#define	FFD_MENU_WIN_SX		( 11 )
#define	FFD_MENU_WIN_SY		( 22 )
#define	FFD_MENU_WIN_PAL	( FFD_SYSFONT_PAL )
#define	FFD_MENU_WIN_CGX	( FFD_MSG_WIN_CGX - ( FFD_MENU_WIN_SX * FFD_MENU_WIN_SY ) )

// �͂�/�������E�B���h�E�i���C���j�i���j���[�Ɠ����ʒu�i���j���[��菬�����j�j
#define	FFD_YESNO_WIN_PX	( 25 )
#define	FFD_YESNO_WIN_PY	( 13 )
#define	FFD_YESNO_WIN_SX	( 6 )
#define	FFD_YESNO_WIN_SY	( 4 )
#define	FFD_YESNO_WIN_PAL	( FFD_SYSFONT_PAL )
#define	FFD_YESNO_WIN_CGX	( FFD_MSG_WIN_CGX - ( FFD_YESNO_WIN_SX * FFD_YESNO_WIN_SY ) )

// ���O�{CP�E�B���h�E(�v���C���[�p)
#define	FFD_CP_WIN_PX		( 1 )
#define	FFD_CP_WIN_PY		( 1 )
#define	FFD_CP_WIN_SX		( 10 )
#define	FFD_CP_WIN_SY		( 4 )
#define	FFD_CP_WIN_PAL		( FFD_SYSFONT_PAL )
#define	FFD_CP_WIN_CGX		( FFD_MENU_WIN_CGX - ( FFD_CP_WIN_SX * FFD_CP_WIN_SY ) )

// ���O�{CP�E�B���h�E(�p�[�g�i�[�p)
#define	FFD_CP2_WIN_PX		( 21 )
#define	FFD_CP2_WIN_PY		( 1 )
#define	FFD_CP2_WIN_SX		( FFD_CP_WIN_SX )
#define	FFD_CP2_WIN_SY		( FFD_CP_WIN_SY )
#define	FFD_CP2_WIN_PAL		( FFD_SYSFONT_PAL )
#define	FFD_CP2_WIN_CGX		( FFD_CP_WIN_CGX - ( FFD_CP2_WIN_SX * FFD_CP2_WIN_SY ) )

// BMP�E�B���h�E �t���[�J�n�ʒu
#define FFD_FREE_CGX		(1)


//==============================================================================
//	�^��`
//==============================================================================
///FS_SYSTEM�̕s��`�|�C���^
typedef struct _FRONTIERMAP_PROC_WORK * FMAP_PTR;

///FRONTIER_MAIN_WORK�̕s��`�|�C���^
typedef struct _FRONTIER_MAIN_WORK * FMAIN_PTR;

//typedef struct _TAG_FIELD_OBJ * TARGET_OBJ_PTR;
typedef u32 * TARGET_OBJ_PTR;	//�^�[�Q�b�gOBJ�̌^���܂�����Ȃ̂łƂ肠����u32�Œ�`���Ă���

///FS_SYSTEM�̕s��`�|�C���^
typedef struct _FS_SYSTEM * FSS_PTR;

///FS_SYSTEM�̕s��`�|�C���^
typedef struct _FSEV_WIN * FSEVWIN_PTR;


//==============================================================================
//	�\���̒�`
//==============================================================================
///�z�u���A�N�^�[�̃V�X�e�������Ŏg���T�u�\���́B�A�N�^�[�p�����[�^�֘A
typedef struct{
	s16 x;				///<X���W
	s16 y;				///<Y���W

	u8 act_id;			///<�A�N�^�[ID
	u8 anm_seq;			///<���݂̃A�j���[�V�����V�[�P���XNo
	
	u16 anm_frame:13;	///<���݂̃A�j���[�V�����t���[����
	u16 anm_stop:1;		///<�A�j����~�t���O
	u16 visible:1;		///<�\���E��\���ݒ�
	u16 occ:1;			///<���̗̈�̃p�����[�^�̗L���E�����t���O(FALSE���g���Ă��Ȃ�)
}PUSH_ARTICLE_ACT_PARAM;

///�z�u���A�N�^�[�̃V�X�e���\����
typedef struct{
	u16 resource_id[ACTWORK_RESOURCE_MAX];	///<�X�N���v�g��Ő��������A�N�^�[���\�[�X��ID�ۑ��ꏊ
//	u32 anmbit;				///<�A�j������̗L��(1�r�b�g�P�ʂŊǗ�)

	PUSH_ARTICLE_ACT_PARAM act_param[ACTWORK_MAX];	///<�A�N�^�[�p�����[�^
	//CATS_ACT_PTR act[ACTWORK_MAX];///<�X�N���v�g��Ő������ĊǗ�����ꍇ�̃|�C���^�ۑ��ꏊ
}PUSH_ARTICLE_WORK;


#endif	//__FRONTIER_TYPES_H__

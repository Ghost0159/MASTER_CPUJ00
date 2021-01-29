//==============================================================================
/**
 * @file	d_matsu.h
 * @brief	���c�f�o�b�O�p�t�@�C���̃w�b�_
 * @author	matsuda
 * @date	2007.04.18(��)
 */
//==============================================================================
#ifndef __D_MATSU_H__
#define __D_MATSU_H__

//==============================================================================
//	�萔��`
//==============================================================================
enum{
	DMATSU_PROC_FLAG_BALLOON,
	DMATSU_PROC_FLAG_GDS,
	DMATSU_PROC_FLAG_GDS_RECORDER,
	DMATSU_PROC_FLAG_FOOTPRINT,
};

///�R���e�X�g�f�o�b�O�@�\�Ăяo���̗L�������	0=�����A1=�L��
#define DEBUG_MATSU_CONTEST		(0)

//==============================================================================
//	�\���̒�`
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���c�f�o�b�O�p�\����
 */
//--------------------------------------------------------------
typedef struct{
	FIELDSYS_WORK *fsys;
	BMPLIST_WORK *lw;
	BMP_MENULIST_DATA *listmenu;
	GF_BGL_BMPWIN bmpwin;
	s16 wait;
	u8 main_seq;
	u8 proc_flag;
	u8 proc_debug_mode;
}DMATSU_WORK;

typedef struct{
	int seq;
	int debug_mode;
	u16 rank;
	u16 type;
}EV_ACTING_WORK;

typedef struct{
	CONTEST_SYSTEM *consys;		///<��check�@�����ȓ�����E�o�������܂�܂ł̎b��
	EV_ACTING_WORK *eaw;
	int seq;
	int local_seq;
	int debug_mode;
	u16 rank;
	u16 type;
}EV_SUB_ACTING_WORK;

#endif	//__D_MATSU_H__

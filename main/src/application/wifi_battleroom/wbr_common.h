//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_common.h
 *	@brief		wifi	�o�g�����[���@���ʃw�b�_�[
 *	@author		tomoya takahashi
 *	@data		2007.02.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WBR_COMMON_H__
#define __WBR_COMMON_H__

#include "common.h"
#include "communication/comm_def.h"
#include "communication/comm_system.h"
#include "communication/comm_info.h"
#include "communication/comm_tool.h"
#include "wbr_comm_command.h"
#include "savedata/mystatus.h"
#include "application/trainer_card.h"

#include "include/application/wifi_2dmap/wf2dmap_common.h"

#undef GLOBAL
#ifdef	__WBR_COMMON_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define WBR_COMM_NUM		(2)	// �P�x�ɂȂ���l��
#define WBR_COMM_OYA_AID	(0)	// �e��AID
#define WBR_COMM_MYSTATUSSIZE	(32)	// mystatus�̃T�C�Y

// �O���b�h�f�[�^
#define WBR_COMM_GRID_SIZ	( 16 )	// 1�O���b�h�T�C�Y


//-------------------------------------
///	�s���t���O
//=====================================
typedef enum {
	WBR_MOVE_NONE,	// �����Ȃ��i�L�[���͑҂��j
	WBR_MOVE_TURN,	// �U�����
	WBR_MOVE_WALK,	// ����
	WBR_MOVE_TALK,	// �b������
	WBR_MOVE_RUN,	// ����
	WBR_MOVE_NUM
} WBR_MOVE_FLAG;

//-------------------------------------
///	�s���t���O
//=====================================
typedef enum {
	WBR_WAY_TOP,
	WBR_WAY_BOTTOM,
	WBR_WAY_LEFT,
	WBR_WAY_RIGHT,
	WBR_WAY_NUM
} WBR_WAY_FLAG;

//-------------------------------------
///	�b������TALK���[�h
//=====================================
typedef enum {
	WBR_TALK_NONE,	// �b�������֌W�Ȃ�
	WBR_TALK_OYA,	// �b�������֌W�̐e
	WBR_TALK_KO,	// �b�������֌W�̎q
} WBR_TALK_FLAG;

//-------------------------------------
///	�b���������N�G�X�g�t���O
//=====================================
typedef enum {
	WBR_TALK_REQ_START,
	WBR_TALK_REQ_END,
} WBR_TALK_REQ_FLAG;


//-------------------------------------
///	�b�V�[�P���X
//=====================================
typedef enum {
	WBR_TALK_SEQ_START,			// �b�������J�n
	WBR_TALK_SEQ_NEXT,			// �b���������́H
	WBR_TALK_SEQ_END,			// �b�������I��
	WBR_TALK_SEQ_EXIT,			// �b���������甲����
	WBR_TALK_SEQ_TRCARD_REQ,	// �g���[�i�[�J�[�h���N�G�X�g
	WBR_TALK_SEQ_TRCARD_START,	// �g���[�i�[�J�[�h�J�n
	WBR_TALK_SEQ_TRCARD_ENDWAIT,// �g���[�i�[�J�[�h�I���҂�
	WBR_TALK_SEQ_TRCARD_END,	// �g���[�i�[�J�[�h�I��
	WBR_TALK_SEQ_NUM	
} WBR_TALK_SEQ;



#define WBR_MOVEWAIT_TURN	(2)	// �U���������V���N��
#define WBR_MOVEWAIT_WALK	(8)	// ��������V���N��
#define WBR_MOVEWAIT_RUN	(4)	// ���蓮��V���N��

//-------------------------------------
///	�ʐM�����萔
//=====================================
enum {
	WBR_COMM_SYNC_START,
	WBR_COMM_SYNC_END,
} ;


//  �R�}���h�L���[�̃o�b�t�@��
#define WBR_CMDQ_BUFFNUM	(16)

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�b���������N�G�X�g
//=====================================
typedef struct {
	u16 flag;		// �b�������J�nor�I��	WBR_TALK_REQ_FLAG
	u16 netid;		// ������NETID
} WBR_TALK_REQ;


//-------------------------------------
///	�b�f�[�^
//=====================================
typedef struct {
	u8	netid;			// ������ID
	u8	talk_mode;		// �b���[�h�i�͂Ȃ��������̂��@������ꂽ�̂��j
	u8	talk_playid;	// �b�������Ă���v���C���[ID
	u8	talk_seq;		// �b�������V�[�P���X
} WBR_TALK_DATA;

//-------------------------------------
///	�I�u�W�F�N�g�f�[�^
//=====================================
typedef struct {
	WF2DMAP_ACTCMD act;
	WBR_TALK_DATA talk_data;
} WBR_OBJ_DATA;

#if 0
//-------------------------------------
///	�I�u�W�F�N�g�f�[�^
//=====================================
typedef struct {
	// ���W
	s16 x;
	s16 y;
	u8 z;
	u8 playid;

	// ����
	u8	move_flag;		// ����
	u8	way_flag;		// ����
	u8	talk_mode;		// �b���[�h�i�͂Ȃ��������̂��@������ꂽ�̂��j
	u8	talk_playid;	// �b�������Ă���v���C���[ID
	u8	talk_seq;		// �b�������V�[�P���X

	u8	pad[1];
} WBR_OBJ_DATA;
#endif

//-------------------------------------
///	 �q�ʐM�f�[�^
//=====================================
typedef struct {
	WF2DMAP_REQCMD req;
} WBR_KO_COMM_GAME;

#if 0
//-------------------------------------
///	 �q�ʐM�f�[�^
//=====================================
typedef struct {
	u8	move_flag;
	u8	way_flag;
	u8	pad_0[2];
} WBR_KO_COMM_GAME;
#endif

//-------------------------------------
///	�b�q�ʐM�f�[�^
//=====================================
typedef struct {
	WBR_TALK_DATA talk_data;
} WBR_KO_COMM_TALK;


//-------------------------------------
///	 �e�ʐM�f�[�^
//=====================================
typedef struct {
	WBR_OBJ_DATA	obj_data;
} WBR_OYA_COMM_GAME;

//-------------------------------------
///	�ʐM��{���
//	�����̊�{�I�ȏ��
//	MYSTATUS�ȂǂȂ�
//=====================================
typedef struct {
	TR_CARD_DATA tr_card;
} WBR_COMM_COMMON;


//-------------------------------------
///	MYSTATUS�f�[�^
//=====================================
typedef struct {
	const MYSTATUS* cp_mystatus[ WBR_COMM_NUM ];
} WBR_MYSTATUS;





//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
#undef	GLOBAL
#endif		// __WBR_COMMON_H__


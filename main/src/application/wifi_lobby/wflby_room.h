//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_room.h
 *	@brief		WiFi���r�[	�����V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.11.01
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_ROOM_H__
#define __WFLBY_ROOM_H__

#include "savedata/savedata_def.h"
#include "application/wifi_lobby/wflby_def.h"
#include "system/procsys.h"
#include "application/wifi_lobby/wflby_system_def.h"


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�����߂�l
//=====================================
typedef enum {
	WFLBY_ROOM_RET_ROOM_OUT,			// �o������o��
	WFLBY_ROOM_RET_ROOM_WORLDTIMER,		// ���E���v��
	WFLBY_ROOM_RET_ROOM_TOPIC,			// �g�s�b�N��
	WFLBY_ROOM_RET_ROOM_BALLSLOW,		// �ʓ���
	WFLBY_ROOM_RET_ROOM_BALANCE,		// �ʏ��
	WFLBY_ROOM_RET_ROOM_BALLOON,		// ���D����
	WFLBY_ROOM_RET_ROOM_FOOT1,			// ���Ճ{�[�h��
	WFLBY_ROOM_RET_ROOM_FOOT2,			// ���Ճ{�[�h��
	WFLBY_ROOM_RET_ROOM_TIMEOUT,		// �������I�����

	WFLBY_ROOM_RET_ROOM_ANKETO_INPUT,	// �A���P�[�g�@����
	WFLBY_ROOM_RET_ROOM_ANKETO_OUTPUT,	// �A���P�[�g�@�o��

#ifdef PM_DEBUG
	WFLBY_ROOM_RET_ROOM_RETURN,			// �����ɂ�����x�߂�
#endif
} WFLBY_ROOM_RET;



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	WiFi���r�[�����@�Z�[�u�f�[�^
//=====================================
typedef struct {
	u16	pl_gridx;	// �v���C���[�o��O���b�h��
	u16	pl_gridy;	// �v���C���[�o��O���b�h��
	u16 pl_way;		// �v���C���[�o�����
	u16 pl_inevno;	// �v���C���[����C�x���g�i���o�[
	u16	minigame;	// �V�тɍs���i�s�����j�~�j�Q�[���i���o�[
	u16	pad;
} WFLBY_ROOM_SAVE;


//-------------------------------------
///	�p�����[�^�\����
//=====================================
typedef struct {
	// �����ƈꏏ�̃f�[�^
	SAVEDATA*			p_save;
	WFLBY_SEASON_TYPE	season;
	WFLBY_ROOM_TYPE		room;

	// ���������Ŏg�p����ۑ��f�[�^
	WFLBY_ROOM_SAVE		save;	// �����l�͑S���O�ɂ��Ă�������
	
	// room�V�X�e�������ɕԓ�����f�[�^
	WFLBY_ROOM_RET		ret;

	// �����炭��{���Ȃ�
	WFLBY_SYSTEM*		p_system;	// LOBBY���
} WFLBY_ROOM_PARAM;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
extern PROC_RESULT WFLBY_ROOM_Init(PROC* p_proc, int* p_seq);
extern PROC_RESULT WFLBY_ROOM_Main(PROC* p_proc, int* p_seq);
extern PROC_RESULT WFLBY_ROOM_Exit(PROC* p_proc, int* p_seq);

#ifdef PM_DEBUG
extern PROC_RESULT WFLBY_ROOM_InitDebug(PROC* p_proc, int* p_seq);
extern PROC_RESULT WFLBY_ROOM_ExitDebug(PROC* p_proc, int* p_seq);
#endif

#endif		// __WFLBY_ROOM_H__


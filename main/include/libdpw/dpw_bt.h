/*---------------------------------------------------------------------------*
  Project:  DP WiFi Library
  File:     dpw_bt.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*! @file
	@brief	DP WiFi Battle Tower ���C�u����
	
	@author	kitayama(kitayama_shigetoshi@nintendo.co.jp)
	@author	Yamaguchi Ryo(yamaguchi_ryo@nintendo.co.jp)
	
*/

#ifndef DPW_BT_H_
#define DPW_BT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _NITRO
	#include <dwc.h>
#else
	#include <nitro.h>
	#include <dwc.h>
#endif

#include "include/libdpw/dpw_common.h"
/*-----------------------------------------------------------------------*
					�^�E�萔�錾
 *-----------------------------------------------------------------------*/

//! �����N�̐�
#define DPW_BT_RANK_NUM			10

//! ���[�����̍ő�l
#define DPW_BT_ROOM_NUM_MAX		100

//! Dpw_Bt_Callback �̌��ʂ��G���[�ƂȂ�ꍇ�̎��
typedef enum {
	DPW_BT_ERROR_SERVER_FULL = -1,		//!< �T�[�o�[�e�ʃI�[�o�[
	DPW_BT_ERROR_SERVER_TIMEOUT = -2,	//!< �T�[�o�[����̉������Ȃ�
	DPW_BT_ERROR_ILLIGAL_REQUEST = -3,	//!< �T�[�o�ւ̕s���ȗv���B�s���ȕ����ԍ����w�肵�Ă��܂�
	DPW_BT_ERROR_CANCEL	= -4,			//!< ������ Dpw_Bt_CancelAsync() �ɂ���ăL�����Z�����ꂽ
	DPW_BT_ERROR_FATAL = -5,			//!< �ʐM�v���I�G���[�B�d���̍ē������K�v�ł�
	DPW_BT_ERROR_DISCONNECTED = -6,		//!< �ʐM�s�\�G���[�B���C�u�����̏��������K�v�ł��B
	DPW_BT_ERROR_FAILURE = -7			//!< �ʐM���s�G���[�B���g���C���Ă�������
} DpwBtError;

//! �T�[�o�[�X�e�[�^�X
typedef enum {
	DPW_BT_STATUS_SERVER_OK,				//!< ����
	DPW_BT_STATUS_SERVER_STOP_SERVICE,		//!< �T�[�r�X�ꎞ��~��
	DPW_BT_STATUS_SERVER_FULL				//!< �T�[�o�[�e�ʃI�[�o�[
} DpwBtServerStatus;

//! �|�P�����̃f�[�^�B�f�[�^�̓��e�͌��m���Ȃ�
typedef struct {
	u8 data[56];		//!< �|�P�����̃f�[�^
} Dpw_Bt_PokemonData;

//! ���[�_�[�f�[�^�\����
typedef struct {
	u16 playerName[8];		//!< �v���C���[��
	u8 versionCode;			//!< �o�[�W�����R�[�h
	u8 langCode;			//!< ����R�[�h
	u8 countryCode;			//!< �Z��ł��鍑�R�[�h
	u8 localCode;			//!< �Z��ł���n���R�[�h
	u8 playerId[4];			//!< �v���C���[ID
	s8 leaderMessage[8];	//!< ���[�_�[���b�Z�[�W
	union{
		struct{
			u8 ngname_f	:1;	//!< NG�l�[���t���O
			u8 gender	:1;	//!< �v���C���[�̐��ʃt���O
			u8			:6;	// ���܂�6bit
		};
		u8	  flags;
	};
	u8 padding;				// �p�f�B���O
} Dpw_Bt_Leader;

//! �v���C���[�f�[�^�\����
typedef struct {
	Dpw_Bt_PokemonData pokemon[3];	//!< �|�P�����f�[�^
	u16 playerName[8];		//!< �v���C���[��
	u8 versionCode;			//!< �o�[�W�����R�[�h
	u8 langCode;			//!< ����R�[�h
	u8 countryCode;			//!< �Z��ł��鍑�R�[�h
	u8 localCode;			//!< �Z��ł���n���R�[�h
	u8 playerId[4];			//!< �v���C���[ID
	s8 leaderMessage[8];	//!< ���[�_�[���b�Z�[�W
	union{
		struct{
			u8 ngname_f	:1;	//!< NG�l�[���t���O
			u8 gender	:1;	//!< �v���C���[�̐��ʃt���O
			u8			:6;	// ���܂�6bit
		};
		u8	  flags;
	};
	u8 trainerType;			//!< �g���[�i�[�^�C�v
	s8 message[24];			//!< ���b�Z�[�W
	u16 result;				//!< ����
} Dpw_Bt_Player;

//! ���[���f�[�^�\����
typedef struct {
	Dpw_Bt_Player player[7];	//!< �v���C���[�f�[�^
	Dpw_Bt_Leader leader[30];	//!< ���[�_�[�f�[�^
} Dpw_Bt_Room;


/*-----------------------------------------------------------------------*
					�O���[�o���ϐ���`
 *-----------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*
					�֐��O���錾
 *-----------------------------------------------------------------------*/

extern void Dpw_Bt_Init(s32 pid, u64 friend_key);
extern void Dpw_Bt_Main(void);
extern void Dpw_Bt_End(void);
extern BOOL Dpw_Bt_IsAsyncEnd(void);
extern s32 Dpw_Bt_GetAsyncResult(void);
extern void Dpw_Bt_GetRoomNumAsync(s32 rank);
extern void Dpw_Bt_DownloadRoomAsync(s32 rank, s32 roomNo, Dpw_Bt_Room* roomData);
extern void Dpw_Bt_UploadPlayerAsync(s32 rank, s32 roomNo, s32 win, const Dpw_Bt_Player* player);
extern void Dpw_Bt_CancelAsync(void);
extern void Dpw_Bt_GetServerStateAsync(void);
extern void Dpw_Bt_SetProfileAsync(const Dpw_Common_Profile* data, Dpw_Common_ProfileResult* result);

BOOL Dpw_Bt_Db_InitServer(void);
BOOL Dpw_Bt_Db_UpdateServer(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // DPW_BT_H_
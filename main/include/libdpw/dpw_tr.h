/*---------------------------------------------------------------------------*
  Project:  DP WiFi Library
  File:     dpw_tr.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*! @file
	@brief	DP WiFi Trade ���C�u����
	
	@author	kitayama(kitayama_shigetoshi@nintendo.co.jp)
	@author	Yamaguchi Ryo(yamaguchi_ryo@nintendo.co.jp)
	
*/

#ifndef DPW_TR_H_
#define DPW_TR_H_

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

//! ���O�̃T�C�Y
#define DPW_TR_NAME_SIZE 8

//! Dpw_Tr_DownloadMatchDataAsync() �Ŏ擾�\�ȃf�[�^�̍ő�l
#define DPW_TR_DOWNLOADMATCHDATA_MAX	7

//! Dpw_Tr_Callback �̌��ʂ��G���[�ƂȂ�ꍇ�̎��
typedef enum {
	DPW_TR_ERROR_SERVER_FULL = -1,		//!< �T�[�o�[�e�ʃI�[�o�[
	DPW_TR_ERROR_SERVER_TIMEOUT = -2,	//!< �T�[�o�[����̉������Ȃ�
	DPW_TR_ERROR_NO_DATA = -3,			//!< �T�[�o�Ƀf�[�^�����݂��Ȃ��B�f�[�^�𕜌����Ă��������B
	DPW_TR_ERROR_DATA_TIMEOUT = -4,		//!< �a�����f�[�^�̃^�C���A�E�g�B�^�}�S��߂��Ă��������B
	DPW_TR_ERROR_ILLIGAL_REQUEST = -5,	//!< �T�[�o�ւ̕s���ȗv���B���ɗa���Ă���̂ɗa���悤�Ƃ����Ƃ��ȂǁB
	DPW_TR_ERROR_ILLEGAL_DATA = -6,		//!< �A�b�v���[�h���ꂽ�f�[�^�����Ă���B
	DPW_TR_ERROR_CHEAT_DATA = -7,		//!< �A�b�v���[�h���ꂽ�f�[�^���s��
	DPW_TR_ERROR_NG_POKEMON_NAME = -8,	//!< �A�b�v���[�h���ꂽ�|�P�����̖��O��NG���[�h���܂�ł���
	DPW_TR_ERROR_NG_PARENT_NAME = -9,	//!< �A�b�v���[�h���ꂽ�|�P�����̐e�̖��O��NG���[�h���܂�ł���
	DPW_TR_ERROR_NG_MAIL_NAME = -10,	//!< �A�b�v���[�h���ꂽ���[���̖��O��NG���[�h���܂�ł���
	DPW_TR_ERROR_NG_OWNER_NAME = -11,	//!< �A�b�v���[�h���ꂽ��l������NG���[�h���܂�ł���
	DPW_TR_ERROR_CANCEL	= -12,			//!< ������ Dpw_Tr_CancelAsync() �ɂ���ăL�����Z�����ꂽ�B
	DPW_TR_ERROR_FATAL = -13,			//!< �ʐM�v���I�G���[�B�d���̍ē������K�v�ł�
	DPW_TR_ERROR_DISCONNECTED = -14,	//!< �ʐM�s�\�G���[�B���C�u�����̏��������K�v�ł��B
	DPW_TR_ERROR_FAILURE = -15			//!< �ʐM���s�G���[�B���g���C���Ă�������
} DpwTrError;

//! �T�[�o�[�X�e�[�^�X
typedef enum {
	DPW_TR_STATUS_SERVER_OK,				//!< ����
	DPW_TR_STATUS_SERVER_STOP_SERVICE,		//!< �T�[�r�X�ꎞ��~��
	DPW_TR_STATUS_SERVER_FULL				//!< �T�[�o�[�e�ʃI�[�o�[
} DpwTrServerStatus;

//! �����\����
typedef struct {
	u16 year;	//!< �N
	u8 month;	//!< ��
	u8 day;		//!< ��
	u8 hour;	//!< ��
	u8 minutes;	//!< ��
	u8 sec;		//!< �b
	u8 padding;	// �p�f�B���O
} Dpw_Tr_Date;

//! �|�P�����̐���
typedef enum {
	DPW_TR_GENDER_MALE = 1,		//!< �I�X
	DPW_TR_GENDER_FEMALE = 2,	//!< ���X
	DPW_TR_GENDER_NONE = 3		//!< ���ʂȂ��B�������ɐ��ʂ���Ȃ��悤�ɂ��邽�߂ɂ́A���̒l���w�肵�Ă��������B
} Dpw_Tr_PokemonGender;

//! �|�P�����f�[�^�̃T�[�o�[�����p�\����
typedef struct {
	s16 characterNo;	//!< �L�����N�^�ԍ�
	s8 gender;			//!< ���ʁB Dpw_Tr_PokemonGender �̒l�ɂȂ�B
	s8 level;			//!< ���x���B
} Dpw_Tr_PokemonDataSimple;

//! �|�P�����f�[�^�̃T�[�o�[���������\����
typedef struct {
	s16 characterNo;	//!< �L�����N�^�ԍ�
	s8 gender;			//!< ���ʁB Dpw_Tr_PokemonGender �̒l�ɂȂ�B
	s8 level_min;		//!< ���x���̉����B0�͎w��Ȃ��A1�`100�͂��̃��x���ȏ�̃|�P�����Ƃ����Ӗ��ɂȂ�B
	s8 level_max;		//!< ���x���̏���B0�͎w��Ȃ��A1�`100�͂��̃��x���ȉ��̃|�P�����Ƃ����Ӗ��ɂȂ�B
	s8 padding;			// �p�f�B���O
} Dpw_Tr_PokemonSearchData;

//! ���R�[�h�̏�����ǉ������|�P�����f�[�^�̃T�[�o�[���������\����
typedef struct {
	s16 characterNo;	//!< �L�����N�^�ԍ�
	s8 gender;			//!< ���ʁB Dpw_Tr_PokemonGender �̒l�ɂȂ�B
	s8 level_min;		//!< ���x���̉����B0�͎w��Ȃ��A1�`100�͂��̃��x���ȏ�̃|�P�����Ƃ����Ӗ��ɂȂ�B
	s8 level_max;		//!< ���x���̏���B0�͎w��Ȃ��A1�`100�͂��̃��x���ȉ��̃|�P�����Ƃ����Ӗ��ɂȂ�B
	s8 padding;			// �݊����̂��߂̃p�f�B���O
	s8 maxNum;			//!< ��������|�P�����̍ő吔[1�`7]
    u8 countryCode;     //!< �����ʂ��̍��R�[�h�B0�͏����w�薳���B���{(103)���w�肷��ƁA�T�[�o���countryCode��103�܂���0�̃f�[�^�������B���̑��̍��͂��̐��l�̃f�[�^�̂݌���
} Dpw_Tr_PokemonSearchDataEx;

//! �|�P�����̃f�[�^�B�f�[�^�̓��e�͌��m���Ȃ�
typedef struct {
	u8 data[236];		//!< �|�P�����̃f�[�^
} Dpw_Tr_PokemonData;

//! Dpw_Tr���C�u�����ŃT�[�o�[�Ƃ���肳��郁�C���f�[�^�\����
typedef struct {
	Dpw_Tr_PokemonData postData;			//!< �u�a����v�|�P�����̏ڍ׃f�[�^
	Dpw_Tr_PokemonDataSimple postSimple;	//!< �u�a����v�|�P�����̊ȗ��f�[�^�i�T�[�o�[�����p�j
	Dpw_Tr_PokemonSearchData wantSimple;	//!< �u�ق����v�|�P�����̊ȗ��f�[�^�i�T�[�o�[�����p�j
	u8 gender;								//!< ��l���̐���
	u8 padding;								// �p�f�B���O
	Dpw_Tr_Date postDate;					//!< �a���������i�T�[�o�[�ɂăZ�b�g�j
	Dpw_Tr_Date tradeDate;					//!< �������������i�T�[�o�[�ɂăZ�b�g�j
	s32 id;									//!< ���̃f�[�^��ID�i�T�[�o�[�ɂăZ�b�g�j
	u16 name[DPW_TR_NAME_SIZE];				//!< ��l����
	u16 trainerID;							//!< �g���[�i�[��ID
	u8 countryCode;							//!< �Z��ł��鍑�R�[�h
	u8 localCode;							//!< �Z��ł���n���R�[�h
	u8 trainerType;							//!< �g���[�i�[�^�C�v
	s8 isTrade;								//!< �����ς݃t���O�i�T�[�o�[�ɂăZ�b�g�j
	u8 versionCode;							//!< �o�[�W�����R�[�h
	u8 langCode;							//!< ����R�[�h
} Dpw_Tr_Data;


/*-----------------------------------------------------------------------*
					�O���[�o���ϐ���`
 *-----------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*
					�֐��O���錾
 *-----------------------------------------------------------------------*/

extern void Dpw_Tr_Init(s32 pid, u64 friend_key);
extern void Dpw_Tr_Main(void);
extern void Dpw_Tr_End(void);
extern BOOL Dpw_Tr_IsAsyncEnd(void);
extern s32 Dpw_Tr_GetAsyncResult(void);
extern void Dpw_Tr_UploadAsync(const Dpw_Tr_Data* data);
extern void Dpw_Tr_UploadFinishAsync(void);
extern void Dpw_Tr_DownloadAsync(Dpw_Tr_Data* data);
extern void Dpw_Tr_GetUploadResultAsync(Dpw_Tr_Data* data);
extern void Dpw_Tr_DeleteAsync(void);
extern void Dpw_Tr_ReturnAsync(void);
extern void Dpw_Tr_CancelAsync(void);
extern void Dpw_Tr_DownloadMatchDataAsync(const Dpw_Tr_PokemonSearchData* searchData, s32 maxNum, Dpw_Tr_Data* downloadData);
extern void Dpw_Tr_DownloadMatchDataExAsync(const Dpw_Tr_PokemonSearchDataEx* searchData, Dpw_Tr_Data* downloadData);
extern void Dpw_Tr_TradeAsync(s32 id, const Dpw_Tr_Data* uploadData, Dpw_Tr_Data* downloadData);
extern void Dpw_Tr_TradeFinishAsync(void);
extern void Dpw_Tr_GetServerStateAsync(void);
extern void Dpw_Tr_SetProfileAsync(const Dpw_Common_Profile* data, Dpw_Common_ProfileResult* result);

BOOL Dpw_Tr_Db_InitServer(void);
BOOL Dpw_Tr_Db_UpdateServer(void);
BOOL Dpw_Tr_Db_ShuffleServerData(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // DPW_TR_H_
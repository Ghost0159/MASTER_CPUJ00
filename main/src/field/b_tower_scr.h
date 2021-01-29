/**
 *	@file	btower_scr.h
 *	@brief	�o�g���^���[�@�X�N���v�g�n
 *	@author	Miyuki Iwasawa
 *	@date	06.04.17
 */

#ifndef __H_B_TOWER_SCR_H__
#define __H_B_TOWER_SCR_H__

#ifdef GLOBAL
#undef GLOBAL
#endif

#ifdef __B_TOWER_SCR_H_GLOBAL
#define GLOBAL	/***/
#else
#define GLOBAL	extern
#endif

#include "b_tower_scr_def.h"
#include "tower_scr_common.h"

///�X�N���v�g�R�}���h����Ăяo�����[�`���Q
///////////////////////////////////////////////////////
/**
 *	@brief	�v���C���[�h�ʂ̃����o�[�����擾����
 */
GLOBAL u16	TowerScrTools_GetMemberNum(u16 mode);

/**
 *	@brief	�o�g���^���[�@�Q���\�ȃ|�P�������̃`�F�b�N
 *
 *	@param	num			�Q���ɕK�v�ȃ|�P������
 *	@param	item_flag	�A�C�e���`�F�b�N���邩�t���O
 *
 *	@retval	true	�Q���\
 *	@retval	false	�Q���s��
 */
//GLOBAL BOOL TowerScrTools_CheckEntryPokeNum(u16 num,SAVEDATA *savedata);
GLOBAL BOOL TowerScrTools_CheckEntryPokeNum(u16 num,SAVEDATA *savedata,u8 item_flag);

/**
 *	@brief	���Z�b�g���ă^�C�g���ɖ߂�
 */
GLOBAL void TowerScrTools_SystemReset(void);

/**
 *	@brief	new game���Ƀv���C�f�[�^���N���A����
 */
GLOBAL void TowerScrTools_ClearPlaySaveData(BTLTOWER_PLAYWORK* playSave);

/**
 * @brief	�Z�[�u�f�[�^���L�����ǂ����Ԃ�
 */
GLOBAL BOOL TowerScrTools_IsSaveDataEnable(BTLTOWER_PLAYWORK* playSave);

/**
 *	@brief	�E�o�p�Ɍ��݂̃��P�[�V�������L������
 */
GLOBAL void TowerScrTools_PushNowLocation(FIELDSYS_WORK* fsys);

/**
 *	@brief	�Q�[�g����o�鎞�ɓ���ڑ�������ɖ߂�
 */
GLOBAL void TowerScrTools_PopNowLocation(FIELDSYS_WORK* fsys);

/**
 *	@brief	�������Q�[�g�̈ʒu���擾����
 */
GLOBAL void TowerScrTools_GetGateLocation(FIELDSYS_WORK* fsys,u16* x,u16* y);

/**
 *	@brief	�v���C���[�h�ʃ��R�[�h�Z�[�u�f�[�^�A������Ԃ�
 */
GLOBAL u16	TowerScrTools_GetRenshouRecord(SAVEDATA* sv,u16 play_mode);

/**
 *	@brief	�X�R�A�f�[�^�̃t���O��Ԃ�
 */
GLOBAL u16 TowerScrTools_GetScoreFlags(SAVEDATA* sv,BTWR_SFLAG_ID flagid);

/**
 *	@brief	Wifi���т̖��A�b�v���[�h�t���O�𐧌䂷��
 */
GLOBAL void TowerScrTools_SetWifiUploadFlag(SAVEDATA* sv,u8 flag);

/**
 *	@brief	WiFi�v���C���т̖��A�b�v���[�h�t���O���擾����
 */
GLOBAL u16 TowerScrTools_GetWifiUploadFlag(SAVEDATA* sv);

/**
 *	@brief	�Z�[�u�����Ɏ~�߂��Ƃ��̃G���[����
 *
 *	@return	�v���C���Ă������[�h��Ԃ�
 */
GLOBAL u16 TowerScrTools_SetNGScore(SAVEDATA* savedata);

/**
 *	@brief	WIFI�̃v���C���[DL�f�[�^�����邩�ǂ����`�F�b�N
 */
GLOBAL u16	TowerScrTools_IsPlayerDataEnable(SAVEDATA* sv);


//*****************************************************
//=====================================================
//���[�N�̏������Ɖ��
//=====================================================
//*****************************************************
/**
 *	@brief	fsys���̃o�g���^���[���䃏�[�N�|�C���^������������
 *			�o�g���^���[�J�n���ɕK���Ăяo��
 */
GLOBAL void TowerScr_WorkClear(BTOWER_SCRWORK** pp);

/**
 *	@brief	���[�N�G���A���擾���ď���������
 *
 *	@param	savedata	�Z�[�u�f�[�^�ւ̃|�C���^
 *	@param	init		���������[�h BTWR_PLAY_NEW:���߂���ABTWR_PLAY_CONTINE:��������
 *	@param	playmode	�v���C���[�h�w�� BTWR_MODE_�`
 *	
 *	�����Ȃ炸 TowerScr_WorkRelease()�ŗ̈���J�����邱��
 */
GLOBAL BTOWER_SCRWORK* TowerScr_WorkInit(SAVEDATA* savedata,u16 init,u16 playmode);

/**
 *	@brief	���[�N�G���A���J������
 */
GLOBAL void TowerScr_WorkRelease(BTOWER_SCRWORK* wk);


/**
 *	@brief	�o�g���^���[�p�|�P�����I���ďo��
 */
GLOBAL void TowerScr_SelectPoke(BTOWER_SCRWORK* wk,
				GMEVENT_CONTROL *ev_work,void** proc_wk);

/**
 *	@brief	�I�������|�P�������擾
 *	
 *	@retval	TRUE	�I������
 *	@retval	FALSE	�I�������ɂ�߂�
 */
GLOBAL BOOL TowerScr_GetEntryPoke(BTOWER_SCRWORK* wk,void** app_work,SAVEDATA* sv);

/**
 *	@brief	�Q���w�肵���莝���|�P�����̏����`�F�b�N
 *
 *	@retval	0	�Q��OK
 *	@retval	1	�����|�P����������
 *	@retval 2	�����A�C�e���������Ă���
 */
GLOBAL int TowerScr_CheckEntryPoke(BTOWER_SCRWORK* wk,SAVEDATA* savedata);

/**
 *	@brief�@�ΐ�g���[�i�[No���I
 */
GLOBAL void TowerScr_BtlTrainerNoSet(BTOWER_SCRWORK* wk,SAVEDATA* sv);

/**
 *	@brief	���݂̃��E���h�����X�N���v�g���[�N�Ɏ擾����
 */
GLOBAL u16	TowerScr_GetNowRound(BTOWER_SCRWORK* wk);

/**
 *	@brief	���E���h�����C���N�������g
 */
GLOBAL u16	TowerScr_IncRound(BTOWER_SCRWORK* wk);

/**
 *	@brief	7�A�����Ă��邩�ǂ����`�F�b�N
 */
GLOBAL BOOL TowerScr_IsClear(BTOWER_SCRWORK* wk);

/**
 *	@brief	���݂̘A�������擾����
 */
GLOBAL u16 TowerScr_GetRenshouCount(BTOWER_SCRWORK* wk);

/**
 *	@brief	�s�폈��	
 */
GLOBAL void TowerScr_SetLoseScore(BTOWER_SCRWORK* wk,SAVEDATA* savedata);

/**
 *	@brief	�N���A����
 */
GLOBAL void TowerScr_SetClearScore(BTOWER_SCRWORK* wk,SAVEDATA* savedata, FNOTE_DATA* fnote);


/**
 *	@brief	�x�ނƂ��Ɍ��݂̃v���C�󋵂��Z�[�u�ɏ����o��
 */
GLOBAL void TowerScr_SaveRestPlayData(BTOWER_SCRWORK* wk);


/**
 *	@brief	�ΐ�g���[�i�[���I
 */
GLOBAL void TowerScr_ChoiceBtlPartner(BTOWER_SCRWORK* wk,SAVEDATA* sv);
/**
 *	@brief	AI�}���`�y�A�|�P�������I
 */
GLOBAL void TowerScr_ChoiceBtlSeven(BTOWER_SCRWORK* wk);

/**
 *	@brief	�ΐ�g���[�i�[OBJ�R�[�h�擾
 */
GLOBAL u16 TowerScr_GetEnemyObj(BTOWER_SCRWORK* wk,u16 idx);

/**
 *	@brief	�퓬�Ăяo��
 */
GLOBAL void TowerScr_LocalBattleCall(GMEVENT_CONTROL* event,BTOWER_SCRWORK* wk,BOOL* win_flag);

/**
 *	@brief	���݂̃v���C���[�h��Ԃ�
 */
GLOBAL u16	TowerScr_GetPlayMode(BTOWER_SCRWORK* wk);

/**
 *	@brief	���[�_�[�N���A�t���O�𗧂Ă�
 */
GLOBAL void TowerScr_SetLeaderClearFlag(BTOWER_SCRWORK* wk,u16 mode);

/**
 *	@brief	���[�_�[�N���A�t���O���擾����
 */
GLOBAL	u16	TowerScr_GetLeaderClearFlag(BTOWER_SCRWORK* wk);

/**
 *	@brief	�^���[�N���A���Ƀo�g���|�C���g�����Z����
 *
 *	@return	�V���Ɏ擾�����o�g���|�C���g
 */
GLOBAL u16	TowerScr_AddBtlPoint(BTOWER_SCRWORK* wk);

/**
 *	@brief	�V���O���ŘA���������̂��J���̃g���t�B�[��������t���O�𗧂ĂĂ���
 */
GLOBAL	u16	TowerScr_GoodsFlagSet(BTOWER_SCRWORK* wk,SAVEDATA* sv);

/**
 *	@brief	���݂�WIFI�����N�𑀍삵�ĕԂ�
 */
GLOBAL u16	TowerScr_SetWifiRank(BTOWER_SCRWORK* wk,SAVEDATA* sv,u8 mode);


/**
 *	@brief	���[�_�[��|�������J�����{����������
 */
GLOBAL u16 TowerScr_LeaderRibbonSet(BTOWER_SCRWORK* wk,SAVEDATA* sv);

/**
 *	@brief	�A���̂��J�����{����������	
 */
GLOBAL u16 TowerScr_RenshouRibbonSet(BTOWER_SCRWORK* wk,SAVEDATA* sv);

/**
 *	@biref	�v���C�����_���V�[�h���X�V����
 */
GLOBAL u16 TowerScr_PlayRandUpdate(BTOWER_SCRWORK* wk,SAVEDATA* sv);


//=============================================================================================
//
//	�t�����e�B�A�ƃt�B�[���h�ŋ��ʂŎg�p������̂��ړ�
//
//=============================================================================================

//---------------------------------------------------------------------------------------------
/**
 *	�o�g���^���[�g���[�i�[�̎����|�P�����̃p���[���������肷��
 *
 * @param	tr_no	�g���[�i�[�i���o�[
 *
 * @return	�p���[����
 *
 * b_tower_fld.c��b_tower_scr�Ɉړ�
 */
//---------------------------------------------------------------------------------------------
GLOBAL u8	BattleTowerPowRndGet(u16 tr_no);

//---------------------------------------------------------------------------------------------
/**
 * @brief	�^���[�v���C�X�V�����_���V�[�h�擾 ���E���h�X�V���ɌĂ�
 *
 * b_tower_fld.c��b_tower_scr�Ɉړ�
 */
//---------------------------------------------------------------------------------------------
GLOBAL u16	btower_rand(BTOWER_SCRWORK* wk);


#endif	//__H_BTOWER_SCR_H__


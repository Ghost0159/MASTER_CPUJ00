//==============================================================================
/**
 * @file	frontier_savedata_local.h
 * @brief	�t�����e�B�A�Z�[�u�f�[�^
 * @author	matsuda
 * @date	2007.04.26(��)
 */
//==============================================================================
#ifndef __FRONTIER_SAVEDATA_LOCAL_H__
#define __FRONTIER_SAVEDATA_LOCAL_H__

#include "b_tower_local.h"
#include "factory_savedata_local.h"
#include "stage_savedata_local.h"
#include "castle_savedata_local.h"
#include "roulette_savedata_local.h"
#include "savedata/wifilist.h"	//WIFILIST_FRIEND_MAX


//==============================================================================
//	�\���̒�`
//==============================================================================

//--------------------------------------------------------------
/**
 *	�t�����e�B�A�{�݂̃��R�[�h�f�[�^�\����
 */
//--------------------------------------------------------------
typedef struct _FRONTIER_RECORD_SAVEWORK{
	u16 record[FRID_MAX];
	u16 wififriend_record[WIFILIST_FRIEND_MAX][FRID_WIFI_MAX - FRID_MAX];//�F�B�蒠���Ɏ����R�[�h
}FRONTIER_RECORD_SAVEWORK;


//--------------------------------------------------------------
/**
 *	�t�����e�B�A�Z�[�u���[�N
 */
//--------------------------------------------------------------
struct _FRONTIER_SAVEWORK{
	
	//--------------------------------------------------------------
	//	�e�{�݂̃��R�[�h�f�[�^
	//--------------------------------------------------------------
	FRONTIER_RECORD_SAVEWORK frontier_record;
	
	
	//--------------------------------------------------------------
	//	�v���C�f�[�^(����)
	//--------------------------------------------------------------
	union{
		struct _BTLTOWER_PLAYWORK	play_tower;		//�o�g���^���[
		struct _FACTORYDATA			play_factory;	//�t�@�N�g���[
		struct _STAGEDATA			play_stage;		//�X�e�[�W
		struct _CASTLEDATA			play_castle;	//�L���b�X��
		struct _ROULETTEDATA		play_roulette;	//���[���b�g
	//	FACTORY_PLAYWORK play_factory;		//�t�@�N�g���[
	};
	
	//--------------------------------------------------------------
	//	�ʃf�[�^
	//--------------------------------------------------------------
	//�o�g���^���[
	struct{
		struct _BTLTOWER_SCOREWORK	score;			//�X�R�A�f�[�^
		struct _BTLTOWER_PLAYER_MSG	player_msg;		//WiFi �v���C���[���b�Z�[�W�f�[�^
		struct _BTLTOWER_WIFI_DATA	wifi;			//WiFi �f�[�^
	}tower;
	
	//�o�g���t�@�N�g���[
	struct{
		struct _FACTORYSCORE score;					//�X�R�A�f�[�^
	}factory;

	//�o�g���X�e�[�W
	struct{
		struct _STAGESCORE score;					//�X�R�A�f�[�^
	}stage;

	//�o�g���L���b�X��
	struct{
		struct _CASTLESCORE score;					//�X�R�A�f�[�^
	}castle;

	//�o�g�����[���b�g
	struct{
		struct _ROULETTESCORE score;				//�X�R�A�f�[�^
	}roulette;

	//�t�@�N�g���[
//	struct{
//		u32 test;
//	}factory;
};




#endif	//__FRONTIER_SAVEDATA_LOCAL_H__

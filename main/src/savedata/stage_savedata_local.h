//==============================================================================================
/**
 * @file	stage_savedata_local.h
 * @brief	�u�o�g���X�e�[�W�v�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.06.07
 */
//==============================================================================================
#ifndef	__STAGE_SAVEDATA_LOCAL_H__
#define	__STAGE_SAVEDATA_LOCAL_H__

#include "..\frontier\stage_def.h"

//============================================================================================
//
//	�v���C�f�[�^
//
//============================================================================================
struct _STAGEDATA{
	u8	type		:3;										//�V���O���A�_�u���A�}���`�Awifi�}���`
	u8	save_flag	:1;										//�Z�[�u�ς݂��ǂ����t���O
	u8	dmy99		:4;
	u8	round;												//�����l�ځH

	//�Q�����Ă���莝���|�P�����̈ʒu���
	u8 mine_poke_pos[STAGE_ENTRY_POKE_MAX];

	//�G�g���[�i�[�f�[�^
	u16 tr_index[STAGE_LAP_MULTI_ENEMY_MAX];				//�g���[�i�[index�e�[�u��[18*2]

	//�G�|�P�����f�[�^�����Ȃ��悤�ɕۑ�
	u16 enemy_poke_index[STAGE_LAP_MULTI_ENEMY_MAX];		//�|�P����index�e�[�u��
};


//============================================================================================
//
//	���уf�[�^
//
//============================================================================================
struct _STAGESCORE{
	u8	clear_flag;											//7�A��(�N���A)�������t���O(�r�b�g)
	u8	dmy;
	u16 dummy;

	//�^�C�v���Ƃ̃��x��(�����N)
	//u8 type_level[STAGE_TYPE_MAX][STAGE_TR_TYPE_MAX/2];	//[4][18/2] 4bit
};


#endif	__STAGE_SAVEDATA_LOCAL_H__



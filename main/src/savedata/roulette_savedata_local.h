//==============================================================================================
/**
 * @file	roulette_savedata_local.h
 * @brief	�u�o�g�����[���b�g�v�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.09.05
 */
//==============================================================================================
#ifndef	__ROULETTE_SAVEDATA_LOCAL_H__
#define	__ROULETTE_SAVEDATA_LOCAL_H__

#include "..\frontier\roulette_def.h"

//============================================================================================
//
//	�v���C�f�[�^
//
//============================================================================================
struct _ROULETTEDATA{
	u8	type		:3;									//�V���O���A�_�u���A�}���`�Awifi�}���`
	u8	save_flag	:1;									//�Z�[�u�ς݂��ǂ����t���O
	u8	random_flag	:1;									//�J�[�\���̃����_���ړ�on�t���O
	u8	dmy99		:3;
	u8	round;											//�����l�ځH
	u8	csr_speed_level;								//�J�[�\���̃X�s�[�h���x��

	//�Q�����Ă���莝���|�P�����̈ʒu���
	u8 mine_poke_pos[ROULETTE_ENTRY_POKE_MAX];			//

	//�莝���|�P����
	u16 item[ROULETTE_MINE_POKE_MAX];					//����

	//�G�g���[�i�[�f�[�^
	u16 tr_index[ROULETTE_LAP_MULTI_ENEMY_MAX];			//�g���[�i�[index�e�[�u��[7*2]

	//�G�|�P�����f�[�^�����Ȃ��悤�ɕۑ�
	u16 enemy_poke_index[ROULETTE_LAP_MULTI_ENEMY_MAX];	//�|�P����index�e�[�u��

	u16 dummy;											//4byte���E
};


//============================================================================================
//
//	���уf�[�^
//
//============================================================================================
struct _ROULETTESCORE{
	//��WIFI�ȊO�̃o�g���^�C�v�Ŏg�p���Ă���
	u8	clear_flag;										//7�A��(�N���A)�������t���O(�r�b�g)
	u8	dmy[3];
};


#endif	__ROULETTE_SAVEDATA_LOCAL_H__



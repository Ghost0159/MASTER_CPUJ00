//==============================================================================================
/**
 * @file	factory_savedata_local.h
 * @brief	�u�o�g���t�@�N�g���[�v�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.03.28
 */
//==============================================================================================
#ifndef	__FACTORY_SAVEDATA_LOCAL_H__
#define	__FACTORY_SAVEDATA_LOCAL_H__

#include "..\frontier\factory_def.h"

//============================================================================================
//
//	�v���C�f�[�^
//
//============================================================================================
struct _FACTORYDATA{
	u8	level		:1;										//LV50�A�I�[�v��
	u8	type		:3;										//�V���O���A�_�u���A�}���`�Awifi�}���`
	u8	save_flag	:1;										//�Z�[�u�ς݂��ǂ����t���O
	u8	dmy99		:3;
	u8	round;												//�����l�ځH
	u16	dummy;

	//�G�g���[�i�[�f�[�^
	u16 tr_index[FACTORY_LAP_MULTI_ENEMY_MAX];				//�g���[�i�[index�e�[�u��

	//�莝���̃f�[�^
	u16 temoti_poke_index[FACTORY_MINE_POKE_MAX];			//�|�P����index�e�[�u��
	u8	temoti_pow_rnd[FACTORY_MINE_POKE_MAX];				//�|�P������pow����
	u32 temoti_personal_rnd[FACTORY_MINE_POKE_MAX];			//�|�P�����̌�����

	//�G�̃f�[�^
	u16 enemy_poke_index[FACTORY_ENEMY_POKE_MAX];			//�|�P����index�e�[�u��
	u8	enemy_pow_rnd[FACTORY_ENEMY_POKE_MAX];				//�|�P������pow����
	u32 enemy_personal_rnd[FACTORY_ENEMY_POKE_MAX];			//�|�P�����̌�����
};


//============================================================================================
//
//	���уf�[�^
//
//============================================================================================
struct _FACTORYSCORE{
	//��WIFI�ȊO�̃o�g���^�C�v�Ŏg�p���Ă���
	u8	clear_flag;											//7�A��(�N���A)�������t���O(�r�b�g)
	u8	dmy[3];
};


#endif	__FACTORY_SAVEDATA_LOCAL_H__



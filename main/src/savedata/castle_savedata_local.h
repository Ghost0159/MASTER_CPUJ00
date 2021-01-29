//==============================================================================================
/**
 * @file	castle_savedata_local.h
 * @brief	�u�o�g���L���b�X���v�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.07.03
 */
//==============================================================================================
#ifndef	__CASTLE_SAVEDATA_LOCAL_H__
#define	__CASTLE_SAVEDATA_LOCAL_H__

#include "..\frontier\castle_def.h"

//============================================================================================
//
//	�v���C�f�[�^
//
//============================================================================================
struct _CASTLEDATA{
	u8	type		:3;									//�V���O���A�_�u���A�}���`�Awifi�}���`
	u8	save_flag	:1;									//�Z�[�u�ς݂��ǂ����t���O
	u8	dmy99		:4;
	u8	round;											//�����l�ځH
	u8	dmy89;

	//�Q�����Ă���莝���|�P�����̈ʒu���
	u8 mine_poke_pos[3];

	//�G�g���[�i�[�f�[�^
	u16 tr_index[CASTLE_LAP_MULTI_ENEMY_MAX];			//�g���[�i�[index�e�[�u��[7*2]

	//�莝���|�P����
#if 0
	u16	hp[CASTLE_MINE_POKE_MAX];						//HP					//2*4	=8
	u16	pp[CASTLE_MINE_POKE_MAX][4];					//�Z4��PP				//2*4x4	=32
	u8	condition[CASTLE_MINE_POKE_MAX];				//�ŁA��ჂȂǂ̏��	//1*4	=4
	u16 item[CASTLE_MINE_POKE_MAX];						//����					//2*4   =8 = 52
#else
	//08.05.28 condition��u32�K�v�Ȃ̂ŁApp�����炵�đΏ�
	u16	hp[CASTLE_MINE_POKE_MAX];						//HP					//2*4	=8
	u8	pp[CASTLE_MINE_POKE_MAX][4];					//�Z4��PP				//1*4*4	=16
	u32	condition[CASTLE_MINE_POKE_MAX];				//�ŁA��ჂȂǂ̏��	//4*4	=16
	//u8	dmy35[4];									//�]��					//1*4	=4
	u16 item[CASTLE_MINE_POKE_MAX];						//����					//2*4   =8 = 52
#endif

	//�G�|�P�����f�[�^�����Ȃ��悤�ɕۑ�
	u16 enemy_poke_index[CASTLE_LAP_MULTI_ENEMY_MAX];	//�|�P����index�e�[�u��

	u16 dummy;											//4byte���E				//116
};


//============================================================================================
//
//	���уf�[�^
//
//============================================================================================
struct _CASTLESCORE{
	//��WIFI�ȊO�̃o�g���^�C�v�Ŏg�p���Ă���
	u8	clear_flag;										//7�A��(�N���A)�������t���O(�r�b�g)

	u8	guide_flag;										//�����󂯂���
	u8	dmy[2];
};


#endif	__CASTLE_SAVEDATA_LOCAL_H__



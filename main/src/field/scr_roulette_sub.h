//============================================================================================
/**
 * @file	scr_roulette_sub.h
 * @bfief	�X�N���v�g�R�}���h�F�o�g�����[���b�g
 * @author	Satoshi Nohara
 * @date	07.09.06
 *
 * �X�N���v�g�֘A�Ƃ͐؂藣������������u��
 */
//============================================================================================
#ifndef SCR_ROULETTE_SUB_H
#define SCR_ROULETTE_SUB_H

#include "../frontier/roulette_def.h"

//============================================================================================
//
//	�ʐM�p���[�N
//
//============================================================================================
typedef struct{
	u8	seq;
	u8	recieve_count;
	u16 dummy;

	u16 mine_monsno[ROULETTE_COMM_POKE_NUM];			//�����̃|�P�����i���o�[�i�[
	u16 pair_monsno[ROULETTE_COMM_POKE_NUM];			//����̃|�P�����i���o�[�i�[

	u16* ret_wk;
}FLDSCR_ROULETTE_COMM;


//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern void CommFldRouletteRecvMonsNo(int id_no,int size,void *pData,void *work);


#endif	/* SCR_ROULETTE_SUB_H */




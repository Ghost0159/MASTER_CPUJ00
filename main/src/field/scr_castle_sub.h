//============================================================================================
/**
 * @file	scr_castle_sub.h
 * @bfief	�X�N���v�g�R�}���h�F�o�g���X�e�[�W
 * @author	Satoshi Nohara
 * @date	07.07.24
 *
 * �X�N���v�g�֘A�Ƃ͐؂藣������������u��
 */
//============================================================================================
#ifndef SCR_CASTLE_SUB_H
#define SCR_CASTLE_SUB_H

#include "../frontier/castle_def.h"

//============================================================================================
//
//	�ʐM�p���[�N
//
//============================================================================================
typedef struct{
	u8	seq;
	u8	recieve_count;
	u16 dummy;

	u16 mine_monsno[CASTLE_COMM_POKE_NUM];			//�����̃|�P�����i���o�[�i�[
	u16 pair_monsno[CASTLE_COMM_POKE_NUM];			//����̃|�P�����i���o�[�i�[

	u16* ret_wk;
}FLDSCR_CASTLE_COMM;


//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern void CommFldCastleRecvMonsNo(int id_no,int size,void *pData,void *work);


#endif	/* SCR_CASTLE_SUB_H */




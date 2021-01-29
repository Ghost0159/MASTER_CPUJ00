//============================================================================================
/**
 * @file	scr_stage_sub.h
 * @bfief	�X�N���v�g�R�}���h�F�o�g���X�e�[�W
 * @author	Satoshi Nohara
 * @date	06.06.13
 *
 * �X�N���v�g�֘A�Ƃ͐؂藣������������u��
 */
//============================================================================================
#ifndef SCR_STAGE_SUB_H
#define SCR_STAGE_SUB_H

#include "savedata/stage_savedata.h"

//============================================================================================
//
//	�ʐM�p���[�N
//
//============================================================================================
typedef struct{
	u8	seq;
	u8	recieve_count;
	u16 dummy;

	u16 mine_monsno;			//�����̃|�P�����i���o�[�i�[
	u16 pair_monsno;			//����̃|�P�����i���o�[�i�[

	u16* ret_wk;
}FLDSCR_STAGE_COMM;


//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern void CommFldStageRecvMonsNo(int id_no,int size,void *pData,void *work);
extern void BattleStageSetNewChallenge( SAVEDATA* sv, STAGESCORE* wk, u8 type );


#endif	/* SCR_STAGE_SUB_H */



//==============================================================================================
/**
 * @file	roulette.h
 * @brief	�u�o�g�����[���b�g�v�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.09.05
 */
//==============================================================================================
#ifndef _ROULETTE_H_
#define _ROULETTE_H_

#include "../../src/frontier/roulette_def.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================


//==============================================================================================
//
//	���[���b�g��ʌĂяo���@�\���̐錾
//
//==============================================================================================
typedef struct{
	SAVEDATA* sv;

	u8	type; 
	u8	level;
	u8	mode; 
	u8	point;

	int* p_weather;									//�V��
	u8*	p_csr_speed_level;							//�J�[�\���X�s�[�h���x��
	u8*	p_decide_ev_no;								//���肵���C�x���g�i���o�[
	u16* p_rand_pos;								//���ʂ̃����_��

	u16	pair_rensyou;
	u16	dummy2;

	u16	rensyou;
	u16	lap;

	//�G�g���[�i�[��ʂ̏�񂪌��J���ꂽ���t���O
	u8	enemy_temoti_flag[ROULETTE_ENEMY_POKE_MAX];
	u8	enemy_level_flag[ROULETTE_ENEMY_POKE_MAX];
	u8	enemy_tuyosa_flag[ROULETTE_ENEMY_POKE_MAX];
	u8	enemy_waza_flag[ROULETTE_ENEMY_POKE_MAX];

	POKEPARTY* p_m_party;							//����
	POKEPARTY* p_e_party;							//�G
//	u16 ret_work[ROULETTE_RET_WORK_MAX];				//�߂�l�i�[���[�N
	u16 ret_work;									//�߂�l�i�[���[�N
	void* p_work;									//ROULETTE_SCRWORK(bf_scr_wk�ۑ�)

	u16 pair_cp;									//�p�[�g�i�[��CP

	u8	random_flag;								//�J�[�\�������_���ړ�on�t���O
	u8	dummy_work;
	u16	dummy_work2;
}ROULETTE_CALL_WORK;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
//extern const PROC_DATA RouletteProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT RouletteProc_Init( PROC * proc, int * seq );
extern PROC_RESULT RouletteProc_Main( PROC * proc, int * seq );
extern PROC_RESULT RouletteProc_End( PROC * proc, int * seq );


#endif //_ROULETTE_H_



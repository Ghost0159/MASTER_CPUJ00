//==============================================================================================
/**
 * @file	castle.h
 * @brief	�u�o�g���t�@�N�g���[�v�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.07.04
 */
//==============================================================================================
#ifndef _CASTLE_H_
#define _CASTLE_H_

#include "../../src/frontier/castle_def.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================


//==============================================================================================
//
//	�t�@�N�g���[��ʌĂяo���@�\���̐錾
//
//==============================================================================================
typedef struct{
	SAVEDATA* sv;

	u8	type; 
	u8	level;
	u8	mode; 
	u8	dummy;

	//�G�g���[�i�[��ʂ̏�񂪌��J���ꂽ���t���O
	u8	enemy_temoti_flag[CASTLE_ENEMY_POKE_MAX];
	u8	enemy_level_flag[CASTLE_ENEMY_POKE_MAX];
	u8	enemy_tuyosa_flag[CASTLE_ENEMY_POKE_MAX];
	u8	enemy_waza_flag[CASTLE_ENEMY_POKE_MAX];

	POKEPARTY* p_m_party;							//����
	POKEPARTY* p_e_party;							//�G
	u16 ret_work;									//�߂�l�i�[���[�N
	void* p_work;									//CASTLE_SCRWORK(bf_scr_wk�ۑ�)

	u16 pair_cp;									//�p�[�g�i�[��CP
	u16 dummy3;

	u32 dummy_work;
}CASTLE_CALL_WORK;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
//extern const PROC_DATA CastleProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT CastleMineProc_Init( PROC * proc, int * seq );
extern PROC_RESULT CastleMineProc_Main( PROC * proc, int * seq );
extern PROC_RESULT CastleMineProc_End( PROC * proc, int * seq );
extern PROC_RESULT CastleEnemyProc_Init( PROC * proc, int * seq );
extern PROC_RESULT CastleEnemyProc_Main( PROC * proc, int * seq );
extern PROC_RESULT CastleEnemyProc_End( PROC * proc, int * seq );
extern PROC_RESULT CastleRankProc_Init( PROC * proc, int * seq );
extern PROC_RESULT CastleRankProc_Main( PROC * proc, int * seq );
extern PROC_RESULT CastleRankProc_End( PROC * proc, int * seq );


#endif //_CASTLE_H_



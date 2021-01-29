//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		bct_common.h
 *	@brief		���ʃw�b�_
 *	@author		tomoya takahashi
 *	@data		2007.06.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __BCT_COMMON_H__
#define __BCT_COMMON_H__

#include "savedata/mystatus.h"

// �f�o�b�N�萔
#ifdef PM_DEBUG
//#define BCT_DEBUG_AUTOSLOW		// ��������
//#define BCT_DEBUG_NOT_TIMECOUNT	// ���Ԑ����Ȃ�
//
//#define BCT_DEBUG			// OS_PRINT	�؂̎���
//#define BCT_DEBUG_HITZONE	// �����蔻��]�[���̕\��
//#define BCT_DEBUG_TIMESELECT // ���ԑI��mode
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define BCT_PLAYER_NUM	(4)		// �v���C���[��
#define BCT_SCORE_MAX	(99999)	// �X�R�A�ő�l
#define BCT_MIDDLE_SCORE_SEND_TIMING	( 30 )	// �`�V���N�ɂP�񑗐M����
#define BCT_SPECIAL_BALL_COUNT	(5)	// �X�y�V�����{�[���ɂȂ�J�E���g
#define BCT_NUTS_COUNT_START_BONUS_NUM	(3)	// �؂̎��J�E���^���o����

// ���A�Q�[�����
#define BCT_GAMETYPE_PAR_MAX		(100)
#define BCT_GAMETYPE_NORAL_PAR		(75)		// �ʏ�
#define BCT_GAMETYPE_ROT_RET_PAR	(15)		// �t��]�p�[�Z���e�[�W
#define BCT_GAMETYPE_SCA_RET_PAR	(10)		// �X�P�[���p�[�Z���e�[�W

// �ʐM�����p
enum{
	BCT_SYNCID_NONE,
	BCT_SYNCID_GAMESTART,
	BCT_SYNCID_CLIENT_TOUCHPEN_END,
	BCT_SYNCID_END,		// �I������
	BCT_SYNCID_ERR_END,	// �G���[�`�F�b�N�I������
};

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�Q�[���O�ɑ��M����Q�[���̏��
//=====================================
typedef struct {
	u8 scale_rev;	// �g�嗦�̔��]
	u8 rota_rev;	// ��]�����̔��]
	u8 pad0;
	u8 pad1;
} BCT_GAME_TYPE_WK;


//-------------------------------------
/// �؂̎��ʐM
//=====================================
typedef struct{
	u16 pl_no;		// �v���C���[NO
	u16	in_flag;	// �؂̎����}���m�[���ɓ�������	4
	VecFx16 way;	// ����							
	VecFx16 mat;	// ���W
	fx16	power;	// �p���[						8
	s8 bonus;		//	�{�[�i�X					12
	u8 special;		// �X�y�V�����{�[��
} BCT_NUT_COMM;


//-------------------------------------
///	�݂�Ȃ̃X�R�A
//=====================================
typedef struct {
	u32 score[ BCT_PLAYER_NUM ];
} BCT_SCORE_COMM;



//-------------------------------------
/// �Q�[���\���f�[�^1�e�[�u���f�[�^
//=====================================
typedef struct {
	s32	nuts_num;		// ���̃e�[�u�����Q�Ƃ���͈̖͂؂̎��̐�
	s32	rota_speed;		// ��]�X�s�[�h
	s32 camera_ta_y;	// �J�����^�[�Q�b�gY���W
	s32	camera_an_x;	// �J�����A���O����
	s32 pos_z;			// ���ʒu
	s32 size;			// �T�C�Y
} BCT_GAMEDATA_ONE;

//-------------------------------------
///	�Q�[���\���f�[�^	
//=====================================
typedef struct {
	u32 tblnum;
	BCT_GAMEDATA_ONE* p_tbl;
	const MYSTATUS* cp_status[ BCT_PLAYER_NUM ];
	u32				vip[ BCT_PLAYER_NUM ];
} BCT_GAMEDATA;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
#endif		// __BCT_COMMON_H__


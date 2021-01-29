//==============================================================================
/**
 * @file	bb_game.h
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2007.10.19(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================

#ifndef __BB_GAME_H__
#define __BB_GAME_H__

#ifdef PM_DEBUG

///< �L���ŋʂ��]����Ȃ�
#define BB_SW_GAME_UPDATE_OFF

///< �L���ŃZ�b�g�A�b�v����
//#define BB_GAME_SETUP_ON

///< �L���ŃG���h���X
//#define BB_GAME_ENDLESS

///< �L���ŃG�t�F�N�g����
//#define BB_EFFECT_ON

///< �L���ŏ�Ƀ��A�Q�[��
///#define BB_REA_GAME_ON

#endif


/// �Q�[�������ő���ł���p�����[�^

///< �ʂ̌X���X�V�^�C�~���O ( 30�� x �b�� �Ȃ̂ŁA30*2�Ȃ�Q�b�ɂP�� )
#define BB_ROOL_UPDATE_LV1	( 30 * 4 )					///< ��]�̍X�V�ڰ�
#define BB_ROOL_UPDATE_LV2	( 30 * 3 )					///< ��]�̍X�V�ڰ�
#define BB_ROOL_UPDATE_LV3	( 30 * 2 )				///< ��]�̍X�V�ڰ�
#define BB_ROOL_UPDATE_LV4	( 30 * 1.5 )					///< ��]�̍X�V�ڰ�

///< �p���[  �ŏ��l��19�@�ő��50�������I�ł��B 
///< �m���� ���ꂪ�N����m���Ȃ̂ő����ĂP�O�O�ɂȂ�悤��
#define LV1_0_X				( 19 )		///< �p���[
#define LV1_1_X				( 19 )
#define LV1_2_X				( 19 )
#define LV1_3_X				( 19 )
#define LV1_0_P				( 45 )		///< �m��
#define LV1_1_P				( 45 )
#define LV1_2_P				(  5 )
#define LV1_3_P				(  5 )

#define LV2_0_X				( 20 )		///< �p���[
#define LV2_1_X				( 20 )
#define LV2_2_X				( 25 )
#define LV2_3_X				( 25 )
#define LV2_0_P				( 30 )		///< �m��
#define LV2_1_P				( 50 )
#define LV2_2_P				( 20 )
#define LV2_3_P				( 10 )

#define LV3_0_X				( 20 )		///< �p���[
#define LV3_1_X				( 30 )
#define LV3_2_X				( 35 )
#define LV3_3_X				( 40 )
#define LV3_0_P				( 18 )		///< �m��
#define LV3_1_P				( 40 )
#define LV3_2_P				( 30 )
#define LV3_3_P				( 12 )

#define LV4_0_X				( 20 )		///< �p���[
#define LV4_1_X				( 30 )
#define LV4_2_X				( 35 )
#define LV4_3_X				( 45 )
#define LV4_0_P				( 15 )		///< �m��
#define LV4_1_P				( 30 )
#define LV4_2_P				( 40 )
#define LV4_3_P				( 15 )

#define BB_POINT_FALL		( FX32_CONST(  45 ) )		///< �����銄���@POINT_Y����̊���

/// -----�����܂�----------------------------------------







#define BB_REA_GAME			( 100 )					///< ������
#define BB_PLAYER_NUM		( 4 )					///< �v���C���[�l��
#define BB_SCORE_UP			( 100 )					///< �X�R�A���{
#define BB_SCORE_MAX		( 100000 )				///< �X�R�A�̍ő�
#define BB_SCORE_MIN		(   4500 )				///< �X�R�A�̍ŏ�
#define BB_SCORE_MIN_CHK	( 1 )
#define BB_SCORE_SET_MIN	( 0 )

///< �X�R�A�̌v�Z
#define BB_P_TO_SCORE( n )	( ( n / 10 ) * BB_SCORE_UP )
#define BB_P_LV1_BASE		(    1 )			///< �P�t���ӂ�̃X�R�A
#define BB_P_LV2_BASE		(    2 )
#define BB_P_LV3_BASE		(    3 )
#define BB_P_LV4_BASE		(    4 )
#define BB_P_LV1_BONUS		(  700 )			///< ���x���A�b�v�{�[�i�X
#define BB_P_LV2_BONUS		( 1400 )
#define BB_P_LV3_BONUS		( 2100 )
#define BB_P_LV4_BONUS		( 2800 )

///< �Q�[�����Ԃƃ��x���A�b�v
#define BB_GAME_TIMER		( ( 30 * 40 ) - 0 )	///< �Q�[������
#define BB_GAME_LV_1UP		( ( 30 *  0 ) - 1 )	///< Lv1 �ɂȂ鎞��
#define BB_GAME_LV_2UP		( ( 30 * 10 ) - 1 )	///< Lv2 �ɂȂ鎞��
#define BB_GAME_LV_3UP		( ( 30 * 20 ) - 1 )	///< Lv3 �ɂȂ鎞��
#define BB_GAME_LV_4UP		( ( 30 * 30 ) - 1 )	///< Lv4 �ɂȂ鎞��
#define BB_GAME_LV_MAX		( ( 30 * 40 ) - 1 )	///< Lv5 �ɂȂ鎞��
#define BB_GANE_LEVEL_MAX	( 4 )

#define BB_RECOVER_WAIT		( ( 30 * 3 ) )		///< �����Ă���߂�܂�
#define BB_JUMP_BALL_WAIT	( 30 )				///< �ʂɏ��܂ł�wait

///< �V���N���̉��o
#define BB_FEVER_COUNT		( ( 30 * 5 ) -1 )	///< �t�B�[�o�[LV�A�b�v���鎞��
//#define BB_FEVER_LEVEL_MAX	( 7 )				///< �ő�LV
#define BB_FEVER_LEVEL_MAX	( 8 )				///< �ő�LV

///< �l�I�ȉ��o
#define BB_MY_EFF_1			( (  4 * 30 ) - 1 )	///< ���C�g
#define BB_MY_EFF_2			( (  9 * 30 ) - 1 )	///< ����x2
#define BB_MY_EFF_3			( ( 14 * 30 ) - 1 )	///< ���ӂԂ�
#define BB_MY_EFF_4			( ( 19 * 30 ) - 1 )	///< ����x4
#define BB_MY_EFF_5			( ( 24 * 30 ) - 1 )	///< �ԉ�

#define BB_SURFACE_LIMIT	( 192 + 32 )
#define BB_SURFACE_Y		( 192 + 64 )
#define BB_SURFACE_Y_FX		( ( 192 + 64 ) << FX32_SHIFT )



#define BB_CONST_INI_FRAME	( 8 )						///< ���ʂœ����ڰ�
#define BB_CONST_ROLL_FRAME	( BB_CONST_INI_FRAME + 2 )	///< ���ʂœ����ڰ�
#define BB_ACCEL_ROLL_FRAME	( 12 )						///< ���������ڰ�


///< �␳�W���Ƃ�
#define BB_COEFFICIENT_DX		( ( ( 1.33 ) / 256 ) )		///< �ړ��ʕ␳�l
#define BB_COEFFICIENT_DY		( ( ( 1.00 ) / 192 ) )		///< �ړ��ʕ␳�l
#define BB_COEFFICIENT_A		( 1.00 )					///< �ړ��ʂ̌W�� ���̒l��傫������ƈړ��ʂ�������
#define BB_COEFFICIENT_AB		( 2.35 * 0.80 )				///< ����ɊO������t������l�i�{�[���j
#define BB_COEFFICIENT_AB_PEN	( 2.50 * 0.80 )				///< ����ɊO������t������l�i�{�[���j
#define BB_COEFFICIENT_AM		( 0.22 )					///< ����ɊO������t������l�i�}�l�l �y���ł̈ړ��j
#define BB_COEFFICIENT_AMI		( 0.80 )					///< ����ɊO������t������l�i�}�l�l �{�[���ł̈ړ��j
#define BB_COEFFICIENT_AMB		( 0.80 )					///< ����ɊO������t������l�i�}�l�l �{�[���ł̈ړ��j
#define BB_COEFFICIENT_POW		( 1 )						///< 1�h�b�g�ł͌v�Z���ʂ̔��f���ł��Ȃ��̂ł��{���Ĉ���

#define BB_COEFFICIENT_PEN( n )	( ( 100 - ( n >> FX32_SHIFT ) ) / 40 )


///< ������W
#define BB_POINT_Y			( FX32_CONST( 100 ) )		///< ���̐��l�͕ω����Ȃ�


#define BB_POINT_1S			( FX32_CONST(  75 ) )		///< �����銄���@POINT_Y����̊���
#define BB_POINT_1E			( FX32_CONST(  61 ) )		///< �����銄���@POINT_Y����̊���
#define BB_POINT_2S			( FX32_CONST(  60 ) )			///< �����銄���@POINT_Y����̊���
#define BB_POINT_2E			( FX32_CONST(  46 )  )				///< �����銄���@POINT_Y����̊���


///< �Q�[���f�[�^
#define LVA_DEFAUT_POW		( 20 )		///< �ŏ��ɂ������



#endif

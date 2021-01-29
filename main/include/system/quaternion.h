//==============================================================================
/**
 * @file	quaternion.h
 * @brief	�N�H�[�^�j�I��
 * @author	goto
 * @date	2007.10.09(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

// -----------------------------------------
//
//	�N�H�[�^�j�I��
//
// -----------------------------------------
typedef union {
	
	struct {		
		f32	wr;		///< ����
		f32	xi;		///< ����
		f32	yi;		///< ����
		f32	zi;		///< ����
	};
	f32 q[ 4 ];
	
} QUATERNION;


// -----------------------------------------
//
//	�N�H�[�^�j�I���}�g���b�N�X ( 4x4 )
//
// -----------------------------------------
typedef union {
	
	struct {
		f32 _00, _01, _02, _03;
        f32 _10, _11, _12, _13;
        f32 _20, _21, _22, _23;
        f32 _30, _31, _32, _33;
	};
	f32 m[ 4 ][ 4 ];
	f32 a[ 16 ];
	
} QUATERNION_MTX44;




// �N�H�[�^�j�I���̏�����
extern void Quaternion_Identity( QUATERNION* q );

// �N�H�[�^�j�I���̃R�s�[
extern void Quaternion_Copy( QUATERNION* q1, QUATERNION* q2 );

// �N�H�[�^�j�I���̐�
extern void Quaternion_Mul( QUATERNION* q, const QUATERNION* q1, const QUATERNION* q2 );

// �N�H�[�^�j�I���̉��Z
extern void Quaternion_Add( QUATERNION* q, const QUATERNION* q1, const QUATERNION* q2 );

// �N�H�[�^�j�I���̌��Z
extern void Quaternion_Sub( QUATERNION* q, const QUATERNION* q1, const QUATERNION* q2 );

// �N�H�[�^�j�I���Ǝ����̏��Z
extern void Quaternion_DivReal( QUATERNION *ans, const QUATERNION *qt, f32 s );

// �N�H�[�^�j�I���̃m���� || a || ^ 2
extern f32  Quaternion_GetNormSqrt( const QUATERNION* q );

// �N�H�[�^�j�I���̃m���� || a ||
extern f32  Quaternion_GetNorm( const QUATERNION* q );

// �N�H�[�^�j�I�����}�g���b�N�X�ɐݒ�
extern void Quaternion_SetMtx44( QUATERNION_MTX44* qmtx, const QUATERNION* q );

// �N�H�[�^�j�I���}�g���b�N�X��fx32�^4x4�}�g���b�N�X�ɕϊ�
extern void Quaternion_SetMtx44_to_MtxFx44( const QUATERNION_MTX44* qmtx, MtxFx44* mtx );

// �N�H�[�^�j�I���}�g���b�N�X��fx32�^4x3�}�g���b�N�X�ɕϊ�
extern void Quaternion_SetMtx44_to_MtxFx43( const QUATERNION_MTX44* qmtx, MtxFx43* mtx );

// �N�H�[�^�j�I���̕ێ����Ă����]��
extern u16  Quaternion_GetRotation( const QUATERNION* q );

#endif

//==============================================================================
/**
 * @file	quaternion.c
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2007.10.09(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#include "system/quaternion.h"

//--------------------------------------------------------------
/**
 * @brief	�P�ʃN�H�[�^�j�I���쐬
 *
 * @param	q	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Quaternion_Identity( QUATERNION* q )
{
	q->wr = 1.0;
	q->xi = 0;
	q->yi = 0;
	q->zi = 0;
}


//--------------------------------------------------------------
/**
 * @brief	�N�H�[�^�j�I���̃R�s�[ ( ������Ă邾�� )
 *
 * @param	q1	
 * @param	q2	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Quaternion_Copy( QUATERNION* q1, QUATERNION* q2 )
{
	*q1 = *q2;
}


//--------------------------------------------------------------
/**
 * @brief	�N�H�[�^�j�I���̐�
 *
 * @param	q		�i�[��
 * @param	q1		�N�H�[�^�j�I��
 * @param	q2		�N�H�[�^�j�I��
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Quaternion_Mul( QUATERNION* q, const QUATERNION* q1, const QUATERNION* q2 )
{
	q->wr = ( q1->wr * q2->wr ) - ( q1->xi * q2->xi ) - ( q1->yi * q2->yi ) - ( q1->zi * q2->zi );
	q->xi = ( q1->wr * q2->xi ) + ( q1->xi * q2->wr ) + ( q1->yi * q2->zi ) - ( q1->zi * q2->yi );
	q->yi = ( q1->wr * q2->yi ) - ( q1->xi * q2->zi ) + ( q1->yi * q2->wr ) + ( q1->zi * q2->xi );
	q->zi = ( q1->wr * q2->zi ) + ( q1->xi * q2->yi ) - ( q1->yi * q2->xi ) + ( q1->zi * q2->wr );
}


//--------------------------------------------------------------
/**
 * @brief	�N�H�[�^�j�I���̉��Z
 *
 * @param	q	
 * @param	q1	
 * @param	q2	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Quaternion_Add( QUATERNION* q, const QUATERNION* q1, const QUATERNION* q2 )
{
	q->wr = q1->wr + q2->wr;
	q->xi = q1->xi + q2->xi;
	q->yi = q1->yi + q2->yi;
	q->zi = q1->zi + q2->zi;
}


//--------------------------------------------------------------
/**
 * @brief	�N�H�[�^�j�I���̌��Z
 *
 * @param	q	
 * @param	q1	
 * @param	q2	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Quaternion_Sub( QUATERNION* q, const QUATERNION* q1, const QUATERNION* q2 )
{
	q->wr = q1->wr - q2->wr;
	q->xi = q1->xi - q2->xi;
	q->yi = q1->yi - q2->yi;
	q->zi = q1->zi - q2->zi;
}


//--------------------------------------------------------------
/**
 * @brief	�N�H�[�^�j�I���̃m���� || a || ^ 2
 *
 * @param	q	
 *
 * @retval	f32	
 *
 */
//--------------------------------------------------------------
f32 Quaternion_GetNormSqrt( const QUATERNION* q )
{
	return ( q->wr * q->wr ) + ( q->xi + q->xi ) + ( q->yi + q->yi ) + ( q->zi + q->zi );
}


//--------------------------------------------------------------
/**
 * @brief	�N�H�[�^�j�I���̃m���� || a ||
 *
 * @param	q	
 *
 * @retval	f32	
 *
 */
//--------------------------------------------------------------
f32 Quaternion_GetNorm( const QUATERNION* q )
{
	fx32 n;
	
	n = FX_F32_TO_FX32( Quaternion_GetNormSqrt( q ) );
	
	return FX_FX32_TO_F32( FX_Sqrt( n ) );
}


//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	ans	
 * @param	qt	
 * @param	s	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Quaternion_DivReal( QUATERNION *ans, const QUATERNION *qt, f32 s )
{
	ans->wr = qt->wr / s;
	ans->xi = qt->xi / s;
	ans->yi = qt->yi / s;
	ans->zi = qt->zi / s;
}


//--------------------------------------------------------------
/**
 * @brief	�N�H�[�^�j�I�����}�g���b�N�X�ɐݒ�
 *
 * @param	qmtx			�}�g���b�N�X�i�[��
 * @param	quaternion		�N�H�[�^�j�I��
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Quaternion_SetMtx44( QUATERNION_MTX44* qmtx, const QUATERNION* q )
{
	f32 x2 = q->xi * q->xi * 2.0;
	f32 y2 = q->yi * q->yi * 2.0;
	f32 z2 = q->zi * q->zi * 2.0;
	f32 xy = q->xi * q->yi * 2.0;
	f32 yz = q->yi * q->zi * 2.0;
	f32 zx = q->zi * q->xi * 2.0;
	f32 xw = q->xi * q->wr * 2.0;
	f32 yw = q->yi * q->wr * 2.0;
	f32 zw = q->zi * q->wr * 2.0;
	
	qmtx->_00 = 1.0 - y2 - z2;
	qmtx->_01 = xy + zw;
	qmtx->_02 = zx - yw;
	qmtx->_03 = 0.0;
	qmtx->_10 = xy - zw;
	qmtx->_11 = 1.0 - z2 - x2;
	qmtx->_12 = yz + xw;
	qmtx->_13 = 0.0;
	qmtx->_20 = zx + yw;
	qmtx->_21 = yz - xw;
	qmtx->_22 = 1.0 - x2 - y2;
	qmtx->_23 = 0.0;
	qmtx->_30 = 0.0;
	qmtx->_31 = 0.0;
	qmtx->_32 = 0.0;
	qmtx->_33 = 1.0;
}


//--------------------------------------------------------------
/**
 * @brief	�N�H�[�^�j�I���ō쐬�����}�g���b�N�X��fx32�^��4x4�}�g���b�N�X�ɕϊ�
 *
 * @param	qmtx	�N�H�[�^�j�I���}�g���b�N�X
 * @param	mtx		�i�[��
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Quaternion_SetMtx44_to_MtxFx44( const QUATERNION_MTX44* qmtx, MtxFx44* mtx )
{
	mtx->_00 = FX32_CONST( qmtx->_00 );
	mtx->_01 = FX32_CONST( qmtx->_01 );
	mtx->_02 = FX32_CONST( qmtx->_02 );
	mtx->_03 = FX32_CONST( qmtx->_03 );
	mtx->_10 = FX32_CONST( qmtx->_10 );
	mtx->_11 = FX32_CONST( qmtx->_11 );
	mtx->_12 = FX32_CONST( qmtx->_12 );
	mtx->_13 = FX32_CONST( qmtx->_13 );
	mtx->_20 = FX32_CONST( qmtx->_20 );
	mtx->_21 = FX32_CONST( qmtx->_21 );
	mtx->_22 = FX32_CONST( qmtx->_22 );
	mtx->_23 = FX32_CONST( qmtx->_23 );
	mtx->_30 = FX32_CONST( qmtx->_30 );
	mtx->_31 = FX32_CONST( qmtx->_31 );
	mtx->_32 = FX32_CONST( qmtx->_32 );
	mtx->_33 = FX32_CONST( qmtx->_33 );
}


//--------------------------------------------------------------
/**
 * @brief	�N�H�[�^�j�I���ō쐬�����}�g���b�N�X��fx32�^��4x3�}�g���b�N�X�ɕϊ�
 *
 * @param	qmtx	
 * @param	mtx	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Quaternion_SetMtx44_to_MtxFx43( const QUATERNION_MTX44* qmtx, MtxFx43* mtx )
{
	MtxFx44 mtx44;
	
	Quaternion_SetMtx44_to_MtxFx44( qmtx, &mtx44 );
	
	MTX_Copy44To43( &mtx44, mtx );
}


//--------------------------------------------------------------
/**
 * @brief	��]�ʂ��擾 ���W�A��
 *
 * @param	q	
 *
 * @retval	u16	
 *
 */
//--------------------------------------------------------------
u16 Quaternion_GetRotation( const QUATERNION* q )
{
	u16 rad;
	fx32 x = FX32_CONST( q->wr );
	
	rad = FX_AcosIdx( x ) * 2;
	
	return rad;
}


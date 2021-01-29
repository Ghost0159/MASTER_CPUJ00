//==============================================================================
/**
 * @file	br_sys_def.h
 * @brief	�o�g�����R�[�_�[
 * @author	matsuda
 * @date	2008.01.18(��)
 *
 * �A�Z���u���t�@�C�������include����Ė��Ȃ����̂�����`
 *
 */
//==============================================================================
#ifndef __BR_SYS_DEF_H__
#define __BR_SYS_DEF_H__

//==============================================================================
//	�萔��`
//==============================================================================
// -------------------------------------
//
//	���[�h
//
// -------------------------------------
#define BR_MODE_BROWSE		( 0 )		///< �u���E�U
#define BR_MODE_GDS			( 1 )		///< GDS
#define BR_MODE_GDS_BV		( 2 )		///< GDS�o�g���r�f�I
#define BR_MODE_GDS_BV_RANK	( 3 )		///< GDS�o�g���r�f�I�����L���O
#define BR_MODE_GDS_RANK	( 4 )		///< GDS�g���[�i�[�����L���O
#define BR_MODE_GDS_DRESS	( 5 )		///< GDS�h���X�A�b�v�V���b�g
#define BR_MODE_GDS_BOX		( 6 )		///< GDS�{�b�N�X�V���b�g

#define BR_MODE_MAX			( BR_MODE_GDS_BOX + 1 )



#endif	//__BR_SYS_DEF_H__

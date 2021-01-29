/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d
  File:     cgtool.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#ifndef NNSG3D_CGTOOL_H_
#define NNSG3D_CGTOOL_H_

#include <nnsys/g3d/config.h>
#include <nnsys/g3d/anm.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    NNSG3dGetJointScale

    �W���C���g�̃X�P�[���v�Z���s���֐��ւ̃|�C���^
    CG�c�[�����ɈႢ�����邽�ߕ������Ă����B
*---------------------------------------------------------------------------*/
typedef void (*NNSG3dGetJointScale)(NNSG3dJntAnmResult* pResult,
                                    const fx32* p,
                                    const u8* cmd,
                                    u32 srtflag);

/*---------------------------------------------------------------------------*
    NNSG3dSendJointSRT

    �W���C���g�s����W�I���g���G���W���ɐݒ肷��B
    �Ă΂�鎞�_�ł̍s�񃂁[�h��Position/Vector���[�h�ɂȂ��Ă���K�v������B
    �܂��A���H�Ώۂ̍s��̓J�����g�s��ɓ����Ă���K�v������B
 *---------------------------------------------------------------------------*/
typedef void (*NNSG3dSendJointSRT)(const NNSG3dJntAnmResult*);


/*---------------------------------------------------------------------------*
    NNSG3dSendTexSRT

    �e�N�X�`��SRT�s����W�I���g���G���W���ɐݒ肷��B
    ���s��͍s�񃂁[�h��Position/Vector���[�h�ɂȂ��Ă���B
 *---------------------------------------------------------------------------*/
typedef void (*NNSG3dSendTexSRT)(const NNSG3dMatAnmResult*);


/*---------------------------------------------------------------------------*
    NNS_G3dSendJointSRT_FuncArray

    NNSG3dScalingRule(<model_info>::scaling_rule)�̒l�ɑΉ�����
    �֐��|�C���^�̔z��B�W���C���g�s��𑗐M����ꍇ�AG3D�����
    ���̊֐��|�C���^�x�N�^��ʂ��ăA�N�Z�X���Ȃ���΂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
extern NNSG3dGetJointScale NNS_G3dGetJointScale_FuncArray[NNS_G3D_FUNC_SENDJOINTSRT_MAX];
extern NNSG3dSendJointSRT NNS_G3dSendJointSRT_FuncArray[NNS_G3D_FUNC_SENDJOINTSRT_MAX];


/*---------------------------------------------------------------------------*
    NNS_G3dSendTexSRT_FuncArray

    NNSG3dTexMtxMode(<model_info>::tex_matrix_mode)�̒l�ɑΉ�����
    �֐��|�C���^�̔z��B�e�N�X�`���s��𑗐M����ꍇ�AG3D�����
    ���̊֐��|�C���^�x�N�^��ʂ��ăA�N�Z�X���Ȃ���΂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
extern NNSG3dSendTexSRT   NNS_G3dSendTexSRT_FuncArray[NNS_G3D_FUNC_SENDTEXSRT_MAX];


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif

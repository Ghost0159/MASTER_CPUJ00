/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d - src - cgtool
  File:     basic.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.11 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/cgtool/basic.h>
#include <nnsys/g3d/gecom.h>
#include <nnsys/g3d/binres/res_struct.h>


/*---------------------------------------------------------------------------*
    NNSi_G3dSendJointSRTBasic

    �W���C���g�s����W�I���g���G���W���ɐݒ肷��B
    �ʏ�ANNS_G3dSendJointSRT_FuncArray�Ƀ|�C���^���i�[����Ă��āA
    NNS_G3D_SCALINGRULE_STANDARD(<model_info>::scaling_rule��standard�̏ꍇ)
    ���w�肳��Ă����ꍇ�ɌĂяo�����悤�ɂȂ��Ă���B

    �܂��A�Ăяo�����ɂ́APosition/Vector���[�h�ł���A���H�Ώۂ̍s��
    �J�����g�s��ɓ����Ă���K�v������B
 *---------------------------------------------------------------------------*/
void NNSi_G3dSendJointSRTBasic(const NNSG3dJntAnmResult* result)
{
    NNS_G3D_NULL_ASSERT(result);

    // ���̎��_��
    // �s�񃂁[�h��Position/Vector���[�h�łȂ���΂Ȃ�Ȃ��B
    // ���H�Ώۂ̍s�񂪃J�����g�s��ɂȂ��Ă��Ȃ���΂Ȃ�Ȃ��B
    if (!(result->flag & NNS_G3D_JNTANM_RESULTFLAG_TRANS_ZERO))
    {
        if (!(result->flag & NNS_G3D_JNTANM_RESULTFLAG_ROT_ZERO))
        {
            // HACK ALLERT
            // rot��trans�͘A�����Ă���Ƃ������Ƃ��O��
            // �܂�A�R�[�h��NNSG3dJntAnmResult�̃����o�̏��ԂɈˑ����Ă���B
            NNS_G3dGeBufferOP_N(G3OP_MTX_MULT_4x3,
                                (u32*)&result->rot._00,
                                G3OP_MTX_MULT_4x3_NPARAMS);
        }
        else
        {
            // Position/Vector���[�h�͒x�����A
            // ���[�h��؂�ւ��邽�߂̃R�}���h���M�̂��߂�
            // CPU��]�v�Ɏg���͖̂]�܂����Ȃ��B
            NNS_G3dGeBufferOP_N(G3OP_MTX_TRANS,
                                (u32*)&result->trans.x,
                                G3OP_MTX_TRANS_NPARAMS);
        }
    }
    else
    {
        if (!(result->flag & NNS_G3D_JNTANM_RESULTFLAG_ROT_ZERO))
        {
            NNS_G3dGeBufferOP_N(G3OP_MTX_MULT_3x3,
                                (u32*)&result->rot._00,
                                G3OP_MTX_MULT_3x3_NPARAMS);
        }
    }

    if (!(result->flag & NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE))
    {
        NNS_G3dGeBufferOP_N(G3OP_MTX_SCALE,
                            (u32*)&result->scale.x,
                            G3OP_MTX_SCALE_NPARAMS);
    }
}


/*---------------------------------------------------------------------------*
    NNSi_G3dGetJointScaleBasic

    �������ʂ̃X�P�[�����O���s���܂��B*(p + 3), *(p + 4), *(p + 5)�͎g�p���܂���
 *---------------------------------------------------------------------------*/
void NNSi_G3dGetJointScaleBasic(NNSG3dJntAnmResult* pResult,
                                const fx32* p,
                                const u8*,
                                u32 srtflag)
{
    // NOTICE:
    // cmd��NULL�ł��悢
    // NNS_G3D_SRTFLAG_SCALE_ONE��ON�Ȃ�p��NULL�ł��悢

    NNS_G3D_NULL_ASSERT(pResult);
    
    if (srtflag & NNS_G3D_SRTFLAG_SCALE_ONE)
    {
        pResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE;
    }
    else
    {
        NNS_G3D_NULL_ASSERT(p);

        pResult->scale.x = *(p + 0);
        pResult->scale.y = *(p + 1);
        pResult->scale.z = *(p + 2);
    }

    // �g�p���Ȃ����Z�b�g���Ă����̂�����(�u�����h�Ƃ̌��ˍ���)
    pResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_SCALEEX0_ONE |
                     NNS_G3D_JNTANM_RESULTFLAG_SCALEEX1_ONE;
}


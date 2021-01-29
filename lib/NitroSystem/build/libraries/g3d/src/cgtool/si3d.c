/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d - src - cgtool
  File:     si3d.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.19 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/cgtool/si3d.h>

#ifndef NNS_G3D_SI3D_DISABLE
#include <nnsys/g3d/kernel.h>
#include <nnsys/g3d/gecom.h>
#include <nnsys/g3d/sbc.h>
#include <nnsys/g3d/util.h>

#include <nitro/code32.h>

/*---------------------------------------------------------------------------*
    NNSi_G3dSendJointSRTSi3d

    �W���C���g�s����W�I���g���G���W���ɐݒ肷��B
    �ʏ�ANNS_G3dSendJointSRT_FuncArray�Ƀ|�C���^���i�[����Ă��āA
    NNS_G3D_SCALINGRULE_SI3D(<model_info>::scaling_rule��si3d�̏ꍇ)
    ���w�肳��Ă����ꍇ�ɌĂяo�����悤�ɂȂ��Ă���B

    �܂��A�Ăяo�����ɂ́APosition/Vector���[�h�ł���A���H�Ώۂ̍s��
    �J�����g�s��ɓ����Ă���K�v������B
 *---------------------------------------------------------------------------*/
void NNSi_G3dSendJointSRTSi3d(const NNSG3dJntAnmResult* result)
{
    BOOL rtFlag = FALSE;
    // ���̎��_��
    // �s�񃂁[�h��Position/Vector���[�h�łȂ���΂Ȃ�Ȃ��B
    // ���H�Ώۂ̍s�񂪃J�����g�s��ɂȂ��Ă��Ȃ���΂Ȃ�Ȃ��B

    // �v����Ɉȉ��̂悤�Ȍv�Z�����Ă���
    // a(�e)->b(�q)->c(��)�Ƃ���ƁA
    // Sc * (SbSa) * Rc * T'c * inv(SbSa) * Sb * Sa * Rb * T'b * inv(Sa) * Sa * Ra * T'a

    u32 flagScaleEx = result->flag & (u32)(NNS_G3D_JNTANM_RESULTFLAG_SCALEEX0_ONE |
                                           NNS_G3D_JNTANM_RESULTFLAG_SCALEEX1_ONE);

    //
    // ScaleEx0�ɂ̓X�P�[����ݏ悵�Ă����������A
    // ScaleEx1�ɂ̓X�P�[���̋t����ݏ悵�Ă��������������Ă���B
    // ����Ăǂ��炩�Е�������1�Ƃ������Ƃ͂Ȃ��B
    //
    NNS_G3D_ASSERT(flagScaleEx == 0 ||
                   flagScaleEx == (NNS_G3D_JNTANM_RESULTFLAG_SCALEEX0_ONE |
                                   NNS_G3D_JNTANM_RESULTFLAG_SCALEEX1_ONE));

    if (!flagScaleEx)
    {
        // �e�܂ł̋t���X�P�[����1�łȂ��ꍇ�̓X�P�[�����O���s��
        NNS_G3dGeBufferOP_N(G3OP_MTX_SCALE,
                            (u32*)&result->scaleEx1.x,
                            G3OP_MTX_SCALE_NPARAMS);
    }

    if (!(result->flag & NNS_G3D_JNTANM_RESULTFLAG_TRANS_ZERO))
    {
        // Trans��0�łȂ��ꍇ
        if (!flagScaleEx)
        {
            // �e�܂ł̋t���X�P�[����1�łȂ��ꍇ�Atranslation�ɏ���̏�Z���s��
            VecFx32 tmp;
            tmp.x = (fx32)((fx64)result->trans.x * result->scaleEx0.x >> FX32_SHIFT);
            tmp.y = (fx32)((fx64)result->trans.y * result->scaleEx0.y >> FX32_SHIFT);
            tmp.z = (fx32)((fx64)result->trans.z * result->scaleEx0.z >> FX32_SHIFT);
            
            NNS_G3dGeBufferOP_N(G3OP_MTX_TRANS,
                                (u32*)&tmp.x,
                                G3OP_MTX_TRANS_NPARAMS);
        }
        else
        {
            rtFlag = TRUE;
        }
    }

    if (!(result->flag & NNS_G3D_JNTANM_RESULTFLAG_ROT_ZERO))
    {
        // ��]�s�񂪒P�ʍs��łȂ��ꍇ�A��]�s�����Z����B
        if (rtFlag)
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
            NNS_G3dGeBufferOP_N(G3OP_MTX_MULT_3x3,
                                (u32*)&result->rot._00,
                                G3OP_MTX_MULT_3x3_NPARAMS);
        }
    }
    else
    {
        if (rtFlag)
        {
            // �e�܂ł̋t���X�P�[����1�̏ꍇ�A���̂܂�translation�𑗂�
            NNS_G3dGeBufferOP_N(G3OP_MTX_TRANS,
                                (u32*)&result->trans.x,
                                G3OP_MTX_TRANS_NPARAMS);
        }
    }

    if (!flagScaleEx)
    {
        // �e�܂ł̃X�P�[����1�łȂ��ꍇ�̓X�P�[�����O���s��
        NNS_G3dGeBufferOP_N(G3OP_MTX_SCALE,
                            (u32*)&result->scaleEx0.x,
                            G3OP_MTX_SCALE_NPARAMS);
    }

    if (!(result->flag & NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE))
    {
        // ���g�̃X�P�[����1�łȂ��ꍇ�̓X�P�[�����O���s��
        NNS_G3dGeBufferOP_N(G3OP_MTX_SCALE,
                            (u32*)&result->scale.x,
                            G3OP_MTX_SCALE_NPARAMS);
    }
}


/*---------------------------------------------------------------------------*
    NNSi_G3dGetJointScaleSi3d

    Si3d��Classic Scale Off���l�������X�P�[�����O���s���܂��B
    pResult->scaleEx0�ɂ͐e�m�[�h�܂ł̗ݐσX�P�[���l�x�N�g��,
    pResult->scaleEx1�ɂ͐e�m�[�h�܂ł̗ݐϋt���X�P�[���l�x�N�g��������܂��B
    NNS_G3dRSOnGlb.scaleCache���ɂ͎��g�̗ݐσX�P�[���l�x�N�g���E
    �ݐϋt���X�P�[���l������܂��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dGetJointScaleSi3d(NNSG3dJntAnmResult* pResult,
                          const fx32* p,
                          const u8* cmd,
                          u32 srtflag)
{
    //
    // srtflag��NNSG3dResNodeData::flag�ł���B
    // cmd�͌��ݏ������Ă���SBC���w��(NODEDESC�R�}���h)
    //

    //
    // ScaleEx0, ScaleEx1�ɂ͐e�܂ł̗ݐσX�P�[���l������
    //
    u32 nodeID;
    u32 parentID;
    NNS_G3D_NULL_ASSERT(pResult);
    NNS_G3D_NULL_ASSERT(cmd);
    NNS_G3D_NULL_ASSERT(p);
    NNS_G3D_NULL_ASSERT(NNS_G3dRS);
    
    nodeID = *(cmd + 1);
    parentID = *(cmd + 2);

    if (srtflag & NNS_G3D_SRTFLAG_SCALE_ONE)
    {
        // ���g�̃X�P�[���l��1�Ȃ̂Ńt���O�Z�b�g
        pResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE;

        if (NNSi_G3dBitVecCheck(&NNS_G3dRS->isScaleCacheOne[0], parentID))
        {
            // �ݐσX�P�[���l��1�̂܂�
            NNSi_G3dBitVecSet(&NNS_G3dRS->isScaleCacheOne[0], nodeID);

            pResult->flag |= (NNS_G3D_JNTANM_RESULTFLAG_SCALEEX0_ONE |
                              NNS_G3D_JNTANM_RESULTFLAG_SCALEEX1_ONE);
        }
        else
        {
            // �e�̗ݐσX�P�[���l�������z��
            MI_CpuCopy32(&NNS_G3dRSOnGlb.scaleCache[parentID],
                         &NNS_G3dRSOnGlb.scaleCache[nodeID],
                         sizeof(VecFx32) + sizeof(VecFx32));

            MI_CpuCopy32(&NNS_G3dRSOnGlb.scaleCache[parentID],
                         &pResult->scaleEx0,
                         sizeof(VecFx32) + sizeof(VecFx32));
        }
    }
    else
    {
        // ���g�̃X�P�[�����Z�b�g
        pResult->scale.x = *(p + 0);
        pResult->scale.y = *(p + 1);
        pResult->scale.z = *(p + 2);

        if (NNSi_G3dBitVecCheck(&NNS_G3dRS->isScaleCacheOne[0], parentID))
        {
            // ���g�̃X�P�[���l���ݐσX�P�[���l�ɂȂ�
            // NNSG3dResNodeData��Sx,Sy,Sz,InvSx,InvSy,InvSz���R�s�[���Ă���B
            MI_CpuCopy32(p,
                         &NNS_G3dRSOnGlb.scaleCache[nodeID],
                         sizeof(VecFx32) + sizeof(VecFx32));

            // NOTICE
            // nodeID=0��scaleOne�t���O�͎��O�ɃZ�b�g����Ă��܂��B
            // ����́A���m�[�h�̐e�m�[�h��scaleOne�ł��邱�Ƃ��������߂̂��̂ŁA
            // ���m�[�h��scaleOne�łȂ��ꍇ�̓��Z�b�g���Ȃ��Ă͂����܂���B
            // �Ȃ��A���̃R�[�h��if�̊O�ɏo���̂͊ԈႢ�ł��B
            NNSi_G3dBitVecReset(&NNS_G3dRS->isScaleCacheOne[0], nodeID);

            // �e�܂ł̗ݐσX�P�[���l��1�Ȃ̂Ńt���O���Z�b�g
            pResult->flag |= (NNS_G3D_JNTANM_RESULTFLAG_SCALEEX0_ONE |
                              NNS_G3D_JNTANM_RESULTFLAG_SCALEEX1_ONE);
        }
        else
        {
            // NOTICE
            // nodeID=0��scaleOne�t���O�͎��O�ɃZ�b�g����Ă��܂��B
            // ����́A���m�[�h�̐e�m�[�h��scaleOne�ł��邱�Ƃ��������߂̂��̂ŁA
            // ���m�[�h��scaleOne�łȂ��ꍇ�̓��Z�b�g���Ȃ��Ă͂����܂���B
            // �Ȃ��A�R�[�h��if�̊O�ɏo���̂͊ԈႢ�ł��B
            NNSi_G3dBitVecReset(&NNS_G3dRS->isScaleCacheOne[0], nodeID);

            // �e�̗ݐσX�P�[���l�Ɏ��g�̃X�P�[���l�������Ď��g�̗ݐσX�P�[���l�𓾂�
            NNS_G3dRSOnGlb.scaleCache[nodeID].s.x =
                (fx32)((fx64)(*(p + 0)) * NNS_G3dRSOnGlb.scaleCache[parentID].s.x >> FX32_SHIFT);
            NNS_G3dRSOnGlb.scaleCache[nodeID].s.y =
                (fx32)((fx64)(*(p + 1)) * NNS_G3dRSOnGlb.scaleCache[parentID].s.y >> FX32_SHIFT);
            NNS_G3dRSOnGlb.scaleCache[nodeID].s.z =
                (fx32)((fx64)(*(p + 2)) * NNS_G3dRSOnGlb.scaleCache[parentID].s.z >> FX32_SHIFT);
            
            NNS_G3dRSOnGlb.scaleCache[nodeID].inv.x =
                (fx32)((fx64)(*(p + 3)) * NNS_G3dRSOnGlb.scaleCache[parentID].inv.x >> FX32_SHIFT);
            NNS_G3dRSOnGlb.scaleCache[nodeID].inv.y =
                (fx32)((fx64)(*(p + 4)) * NNS_G3dRSOnGlb.scaleCache[parentID].inv.y >> FX32_SHIFT);
            NNS_G3dRSOnGlb.scaleCache[nodeID].inv.z =
                (fx32)((fx64)(*(p + 5)) * NNS_G3dRSOnGlb.scaleCache[parentID].inv.z >> FX32_SHIFT);

            // �e�܂ł̗ݐσX�P�[���l���Z�b�g
            MI_CpuCopy32(&NNS_G3dRSOnGlb.scaleCache[parentID],
                         &pResult->scaleEx0,
                         sizeof(VecFx32) + sizeof(VecFx32));
        }
    }
}


/*---------------------------------------------------------------------------*
    NNSi_G3dSendTexSRTSi3d

    SoftImage3D�ō쐬���ꂽ���f���f�[�^�̃e�N�X�`���s���ݒ肵�܂��B
 *---------------------------------------------------------------------------*/
void NNSi_G3dSendTexSRTSi3d(const NNSG3dMatAnmResult* anm)
{
    struct
    {
        u32     cmd;
        u32     mode_tex;
        MtxFx43 m;
        u32     mode_pos_vec;
    }
    data;

    NNS_G3D_NULL_ASSERT(anm);
    NNS_G3D_ASSERT(anm->flag & (NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SET |
                                NNS_G3D_MATANM_RESULTFLAG_TEXMTX_MULT));

    if (anm->flag & NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SET)
    {
        data.cmd = GX_PACK_OP(G3OP_MTX_MODE, G3OP_MTX_LOAD_4x3, G3OP_MTX_MODE, G3OP_NOP);
    }
    else
    {
        data.cmd = GX_PACK_OP(G3OP_MTX_MODE, G3OP_MTX_MULT_4x3, G3OP_MTX_MODE, G3OP_NOP);
    }

    data.mode_tex     = GX_MTXMODE_TEXTURE;
    data.mode_pos_vec = GX_MTXMODE_POSITION_VECTOR;

    data.m._01 = data.m._02 = data.m._10 = data.m._12 = data.m._20 =
    data.m._21 = data.m._22 = data.m._32 = 0;

    if (anm->flag & NNS_G3D_MATANM_RESULTFLAG_TEXMTX_TRANSZERO)
    {
        data.m._30 = 0;
        data.m._31 = 0;

        if (anm->flag & NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SCALEONE)
        {
            data.m._00 = FX32_ONE;
            data.m._11 = FX32_ONE;
        }
        else
        {
            data.m._00 = anm->scaleS;
            data.m._11 = anm->scaleT;
        }
    }
    else if (anm->flag & NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SCALEONE)
    {
        data.m._30 = -(anm->transS << 4) * anm->origWidth;
        data.m._31 = -(anm->transT << 4) * anm->origHeight;
        data.m._00 = FX32_ONE;
        data.m._11 = FX32_ONE;
    }
    else
    {
        data.m._30 = -(fx32)((fx64)anm->scaleS * anm->transS >> (FX32_SHIFT - 4)) * anm->origWidth;
        data.m._31 = -(fx32)((fx64)anm->scaleT * anm->transT >> (FX32_SHIFT - 4)) * anm->origHeight;
        data.m._00 = anm->scaleS;
        data.m._11 = anm->scaleT;
    }

    if (anm->magW != FX32_ONE)
    {
        data.m._00 = (fx32)((fx64)anm->magW * data.m._00 >> FX32_SHIFT);
        data.m._30 = (fx32)((fx64)anm->magW * data.m._30 >> FX32_SHIFT);

    }

    if (anm->magH != FX32_ONE)
    {
        data.m._11 = (fx32)((fx64)anm->magH * data.m._11 >> FX32_SHIFT);
        data.m._31 = (fx32)((fx64)anm->magH * data.m._31 >> FX32_SHIFT);
    }

    NNS_G3dGeBufferData_N((u32*)&data.cmd, sizeof(data) / 4);
}

#endif // NNS_G3D_SI3D_DISABLE



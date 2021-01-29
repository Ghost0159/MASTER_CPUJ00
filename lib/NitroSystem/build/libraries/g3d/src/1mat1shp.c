/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d
  File:     1mat1shp.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/1mat1shp.h>
#include <nnsys/g3d/binres/res_struct_accessor.h>
#include <nnsys/g3d/gecom.h>
#include <nnsys/g3d/cgtool.h>


/*---------------------------------------------------------------------------*
    NNS_G3dDraw1Mat1Shp

    matID��shpID���w�肵�ĕ`�悵�܂��B�P���ȃ��f����`�悷��̂��߂̊֐��ł��B
    sendMat��FALSE�ɂ���ƃ}�e���A���̑��M���ȗ����܂��B
    �g�p���@�ɂ��ẮA1mat1shp�T���v�����Q�Ƃ��Ă��������B
 *---------------------------------------------------------------------------*/
void
NNS_G3dDraw1Mat1Shp(const NNSG3dResMdl* pResMdl,
                    u32 matID,
                    u32 shpID,
                    BOOL sendMat)
{
    NNS_G3D_NULL_ASSERT(pResMdl);

    if (pResMdl->info.posScale != FX32_ONE)
    {
        // pos_scale�ɂ��X�P�[���␳���K�v�ȏꍇ
        NNS_G3dGeScale(pResMdl->info.posScale,
                       pResMdl->info.posScale,
                       pResMdl->info.posScale);
    }

    if (sendMat)
    {
        NNS_G3D_ASSERT(matID < pResMdl->info.numMat);
        if (matID < pResMdl->info.numMat)
        {
            //
            // �}�e���A���𑗐M����ꍇ:
            // �E�}�e���A���A�j���[�V�����̍Đ��͕s�\�ł��i���f���f�[�^�Ɋi�[����Ă���}�e���A���f�[�^��
            //   ����������Γ��l�̌��ʂ͏o���܂��j�B
            // �ENNS_G3dGlb�\���̂Ɋi�[����Ă���A�O���[�o���̃}�e���A�����͔��f����܂���B���f���f�[�^��
            //   �i�[����Ă���}�e���A���f�[�^�����f����܂��B
            //
            const NNSG3dResMatData* mat;
            u32 cmd[7];
            
            {
                // NNSG3dResMatData�ւ̃|�C���^���擾
                const NNSG3dResMat* p = NNS_G3dGetMat(pResMdl);
                NNS_G3D_NULL_ASSERT(p);
                mat = NNS_G3dGetMatDataByIdx(p, matID);
                NNS_G3D_NULL_ASSERT(mat);
            }

            // �����Ȃ�`�悹���߂�B
            if (!(mat->polyAttr & REG_G3_POLYGON_ATTR_ALPHA_MASK))
                return;

            // G3_MaterialColorDiffAmb(mat->diffAmb);
            // G3_MaterialColorSpecEmi(mat->specEmi);
            // G3_PolygonAttr(...);
            cmd[0] = GX_PACK_OP(G3OP_DIF_AMB, G3OP_SPE_EMI, G3OP_POLYGON_ATTR, G3OP_NOP);
            cmd[1] = mat->diffAmb;
            cmd[2] = mat->specEmi;
            cmd[3] = mat->polyAttr;

            if (mat->flag & NNS_G3D_MATFLAG_WIREFRAME)
            {
                // ���C���[�t���[���\���̏ꍇ�̓W�I���g���G���W���ɑ��郿��0�ɂȂ�
                // ALPHA�̃r�b�g��0�ɂ��Ă����B
                cmd[3] &= ~REG_G3_POLYGON_ATTR_ALPHA_MASK;
            }

            // G3_TexImageParam(...);
            // G3_TexPlttBase(...);
            cmd[4] = GX_PACK_OP(G3OP_TEXIMAGE_PARAM, G3OP_TEXPLTT_BASE, G3OP_NOP, G3OP_NOP);
            cmd[5] = mat->texImageParam;
            cmd[6] = mat->texPlttBase;

            // �܂Ƃ߂�FIFO�ɑ��M
            NNS_G3dGeBufferData_N(&cmd[0], 7);

            if (mat->flag & NNS_G3D_MATFLAG_TEXMTX_USE)
            {
                // �e�N�X�`���s����Z�b�g
                NNSG3dSendTexSRT func;
                NNSG3dMatAnmResult dummy;

                // NNSG3dResMatData�̌���SRT�f�[�^��mat->flag�̒l�ɂ���ē����Ă���
                const u8* p = (const u8*)mat + sizeof(NNSG3dResMatData);
                
                dummy.flag = NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SET;
                dummy.origWidth = mat->origWidth;
                dummy.origHeight = mat->origHeight;
                dummy.magW = mat->magW;
                dummy.magH = mat->magH;

                //
                // �ȉ�NNSG3dResMatData�㕔�̃f�[�^(SRT)���t���O�̒l�����Ȃ���擾���Ă���
                //

                // �e�N�X�`���s���Scale�ݒ�
                if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_SCALEONE))
                {
                    const fx32* p_fx32 = (const fx32*)p;

                    dummy.scaleS = *(p_fx32 + 0);
                    dummy.scaleT = *(p_fx32 + 1);
                    p += 2 * sizeof(fx32);
                }
                else
                {
                    dummy.flag |= NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SCALEONE;
                }

                // �e�N�X�`���s���Rotation�ݒ�
                if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_ROTZERO))
                {
                    const fx16* p_fx16 = (const fx16*)p;

                    dummy.sinR = *(p_fx16 + 0);
                    dummy.cosR = *(p_fx16 + 1);
                    p += 2 * sizeof(fx16);
                }
                else
                {
                    dummy.flag |= NNS_G3D_MATANM_RESULTFLAG_TEXMTX_ROTZERO;
                }

                // �e�N�X�`���s���Translation�ݒ�
                if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_TRANSZERO))
                {
                    const fx32* p_fx32 = (const fx32*)p;

                    dummy.transS = *(p_fx32 + 0);
                    dummy.transT = *(p_fx32 + 1);
                }
                else
                {
                    dummy.flag |= NNS_G3D_MATANM_RESULTFLAG_TEXMTX_TRANSZERO;
                }

                func = NNS_G3dSendTexSRT_FuncArray[pResMdl->info.texMtxMode];
                NNS_G3D_NULL_ASSERT(func);
                if (func)
                {
                    // �e�N�X�`���s����W�I���g���G���W���ɐݒ�
                    // ���f���쐬�ɗ��p���ꂽCG�c�[�����̐ݒ���s��
                    (*func)(&dummy);
                }
            }
        }
    }

    NNS_G3D_ASSERT(shpID < pResMdl->info.numShp);
    if (shpID < pResMdl->info.numShp)
    {
        // Shp�𑗐M
        const NNSG3dResShp* p;
        const NNSG3dResShpData* shp;
        
        p = NNS_G3dGetShp(pResMdl);
        NNS_G3D_NULL_ASSERT(p);
        shp = NNS_G3dGetShpDataByIdx(p, shpID);
        NNS_G3D_NULL_ASSERT(shp);

        NNS_G3dGeSendDL(NNS_G3dGetShpDLPtr(shp), NNS_G3dGetShpDLSize(shp));
    }

    if (pResMdl->info.invPosScale != FX32_ONE)
    {
        // pos_scale�ɂ��X�P�[���␳���������Ă���ꍇ�͌��ɖ߂�
        NNS_G3dGeScale(pResMdl->info.invPosScale,
                       pResMdl->info.invPosScale,
                       pResMdl->info.invPosScale);
    }
}









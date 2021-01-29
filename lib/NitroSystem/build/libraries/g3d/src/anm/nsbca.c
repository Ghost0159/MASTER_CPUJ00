/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d - src - anm
  File:     nsbca.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.29 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/anm/nsbca.h>
#include <nnsys/g3d/sbc.h>
#include <nnsys/g3d/binres/res_struct_accessor.h>

#ifndef NNS_G3D_NSBCA_DISABLE
/*---------------------------------------------------------------------------*
    NNSi_G3dAnmObjInitNsBca

    NNSG3dAnmObj��.nsbca���\�[�X�p�ɃC�j�V�����C�Y���܂��B
    NNS_G3dInitAnmObj����Ăяo����܂��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dAnmObjInitNsBca(NNSG3dAnmObj* pAnmObj,
                        void* pResAnm,
                        const NNSG3dResMdl* pResMdl)
{
    u32 i;
    u16* ofsArray;
    NNSG3dResJntAnm* jntAnm;
    const NNSG3dResNodeInfo* jnt;

    NNS_G3D_NULL_ASSERT(pAnmObj);
    NNS_G3D_NULL_ASSERT(pResAnm);
    NNS_G3D_NULL_ASSERT(pResMdl);

    pAnmObj->resAnm = pResAnm;
    jntAnm = (NNSG3dResJntAnm*)pResAnm;
    jnt = NNS_G3dGetNodeInfo(pResMdl);
    pAnmObj->funcAnm = NNS_G3dFuncAnmJntNsBcaDefault;
    pAnmObj->numMapData = pResMdl->info.numNode;

    
    // �܂���mapData���[���N���A
    MI_CpuClear16(&pAnmObj->mapData[0], sizeof(u16) * pAnmObj->numMapData);

    //
    // NNSG3dJntAnmSRTTag���Ƀ��f���̃m�[�hID���i�[����Ă���B
    //
    ofsArray = (u16*)((u8*)jntAnm + sizeof(NNSG3dResJntAnm));

    // �A�j���[�V�����I�u�W�F�N�g�Ŋm�ۂ��ꂽmapData�z��̃T�C�Y�𒴂��Ȃ����`�F�b�N
    NNS_G3D_ASSERT(jntAnm->numNode <= pAnmObj->numMapData);
    
    for (i = 0; i < jntAnm->numNode; ++i)
    {
        // .nsbca�̏ꍇ�́A���݃��\�[�X�̃C���f�b�N�X�ƃ��f���̃m�[�hID����v����悤�ɂȂ��Ă���.
        NNSG3dResJntAnmSRTTag* pTag =
            (NNSG3dResJntAnmSRTTag*)((u8*)jntAnm + ofsArray[i]);
        pAnmObj->mapData[i] = (u16)((pTag->tag >> NNS_G3D_JNTANM_SRTINFO_NODE_SHIFT) |
                                    NNS_G3D_ANMOBJ_MAPDATA_EXIST);
    }
}


static void
getJntSRTAnmResult_(const NNSG3dResJntAnm* pJntAnm, 
                    u32                    dataIdx, 
                    fx32                   Frame,
                    NNSG3dJntAnmResult*    pResult);

static void
getTransData_(fx32* pVal,
              fx32 Frame,
              const u32* pData,
              const NNSG3dResJntAnm* pJntAnm);

static void
getTransDataEx_(fx32* pVal,
                fx32 Frame,
                const u32* pData,
                const NNSG3dResJntAnm* pJntAnm);

static void
getScaleData_(fx32* s_invs,
              fx32 Frame,
              const u32* pData,
              const NNSG3dResJntAnm* pJntAnm);

static void
getScaleDataEx_(fx32* s_invs,
                fx32 Frame,
                const u32* pData,
                const NNSG3dResJntAnm* pJntAnm);


static void
getRotData_(MtxFx33* pRot,
            fx32 Frame,
            const u32* pData,
            const NNSG3dResJntAnm* pJntAnm);

static void
getRotDataEx_(MtxFx33* pRot,
              fx32 Frame,
              const u32* pData,
              const NNSG3dResJntAnm* pJntAnm);

static BOOL
getRotDataByIdx_(MtxFx33* pRot,
                 const void* pArrayRot3,
                 const void* pArrayRot5,
                 NNSG3dJntAnmRIdx info);


/*---------------------------------------------------------------------------*
    NNSi_G3dAnmCalcNsBca

    pResult: �W���C���g�A�j���[�V�����̌��ʂ��i�[���܂��B
    pAnmObj:
    dataIdx: ���\�[�X���f�[�^�̊i�[�ꏊ�������C���f�b�N�X�ł�
 *---------------------------------------------------------------------------*/
void NNSi_G3dAnmCalcNsBca(NNSG3dJntAnmResult* pResult,
                          const NNSG3dAnmObj* pAnmObj,
                          u32 dataIdx)
{
    NNS_G3D_NULL_ASSERT(pResult);
    NNS_G3D_NULL_ASSERT(pAnmObj);

    //
    // �W���C���g�A�j���[�V�����̎������@
    // 
    // ���̊֐����̂ł́A
    // pResult->flag,
    // pResult->trans,
    // pResult->rot,
    // ��ݒ肷�邱�ƂɂȂ�܂��B
    // pResult->flag�Ɋւ��Ă�
    // NNS_G3D_JNTANM_RESULTFLAG_ROT_ZERO
    // NNS_G3D_JNTANM_RESULTFLAG_TRANS_ZERO
    // NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE
    // �����ꂼ��A��]���Ȃ��Ƃ��A�ړ����Ȃ��Ƃ��ɃZ�b�g���Ă����Ă��������B
    //
    // pResult->scale, pResult->scaleEx0, pResult->scaleEx1�Ɋւ��ẮA
    // ������p�ӂ��āA(*NNS_G3dRS->funcJntScale)���R�[�����邱�ƂŌv�Z�ł��܂��B
    // pResult->flag�̎c��̃t���O�������Őݒ肳��܂��B
    // CG�c�[�����ɕʂ̌v�Z�����Ȃ��Ă͂Ȃ�Ȃ��̂ŁA�����Ȃ��Ă܂��B��̓I�ɂ́A
    // NNS_G3dGetJointScaleBasic,
    // NNS_G3dGetJointScaleMaya,
    // NNS_G3dGetJointScaleSi3d
    // �̂ǂꂩ���Ă΂�܂��B
    // �����́A
    // void NNS_G3dGetJointScaleBasic(NNSG3dJntAnmResult* pResult,
    //                                const fx32* p,
    //                                const u8* cmd,
    //                                u32 srtflag)
    // �Ȋ����ɂȂ��Ă��āApResult�����H����悤�ɂȂ��Ă��܂��B
    // 2�Ԗڈȍ~�̈����ɂ��Đ������Ă����܂��B
    // p�́A�W���C���g�Ɋւ���X�P�[���ƃX�P�[���̋t����������6���[�h�̔z��ɂȂ�܂��B
    // cmd�ɂ�NNS_G3dRS->c�����܂��B
    // srtflag�ɂ́ANNSG3dSRTFlag(res_struct.h�Œ�`)�ɍ��v����悤��
    // �t���O���\�����ē���Ă��������B
    
    {
        fx32 frame;
        NNSG3dResJntAnm* anm = (NNSG3dResJntAnm*)(pAnmObj->resAnm);

        if (pAnmObj->frame >= (anm->numFrame << FX32_SHIFT))
            frame = (anm->numFrame << FX32_SHIFT) - 1;
        else if (pAnmObj->frame < 0)
            frame = 0;
        else
            frame = pAnmObj->frame;
        
        //
        // �W���C���g�A�j�����\�[�XpAnm��dataIdx�Ԗڂ̃��\�[�X��frame�ڂ�
        // ���ʂ�result�ɓ����
        //

        getJntSRTAnmResult_(anm,
                            dataIdx,
                            frame,
                            pResult);

    }
}


/*---------------------------------------------------------------------------*
    vecCross_

    ��]�s��̏ꍇ�A��Βl��FX32_ONE�ȏ�ɂȂ�Ȃ��̂ŁA
    �O�ς�fx64�ɃL���X�g����K�v���Ȃ��B
 *---------------------------------------------------------------------------*/
#include <nitro/code32.h>
static NNS_G3D_INLINE void
vecCross_(const VecFx32 * a, const VecFx32 * b, VecFx32 * axb)
{
    fx32 x, y, z;
    NNS_G3D_NULL_ASSERT(a);
    NNS_G3D_NULL_ASSERT(b);
    NNS_G3D_NULL_ASSERT(axb);

    x = (a->y * b->z - a->z * b->y) >> FX32_SHIFT;
    y = (a->z * b->x - a->x * b->z) >> FX32_SHIFT;
    z = (a->x * b->y - a->y * b->x) >> FX32_SHIFT;

    axb->x = x;
    axb->y = y;
    axb->z = z;
}
#include <nitro/codereset.h>


/*---------------------------------------------------------------------------*
    getMdlTrans_

    ���f����translation���g�p����
 *---------------------------------------------------------------------------*/
static void 
getMdlTrans_(NNSG3dJntAnmResult* pResult)
{
    u32 idxNode;
    const NNSG3dResNodeData* pNd;
    NNS_G3D_NULL_ASSERT(NNS_G3dRS);

    // HACK ALLERT
    // NODEDESC�R�}���h���Ȃ̂ł�������f�[�^���Ƃ��Ă���
    NNS_G3D_ASSERT(NNS_G3D_GET_SBCCMD(*NNS_G3dRS->c) == NNS_G3D_SBC_NODEDESC);
    idxNode = *(NNS_G3dRS->c + 1);
    pNd = NNS_G3dGetNodeDataByIdx(NNS_G3dRS->pResNodeInfo, idxNode);
    
    // Translation
    if (pNd->flag & NNS_G3D_SRTFLAG_TRANS_ZERO)
    {
        // pResult->trans�ɏ������ޑ���Ƀt���O�𗧂Ă邾���ɂ���
        // �������A�N�Z�X�Ƃ��̌�̏����R�X�g��}������B
        pResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_TRANS_ZERO;
    }
    else
    {
        // NNSG3dResNodeData��flag�̒l�ɏ]���Ė����Ƀf�[�^�������Ă���
        // res_struct.h����NNSG3dResNodeData�̒�`���Q�Ƃ̂���
        const fx32* p_fx32 = (const fx32*)((const u8*)pNd + sizeof(NNSG3dResNodeData));
        
        pResult->trans.x = *(p_fx32 + 0);
        pResult->trans.y = *(p_fx32 + 1);
        pResult->trans.z = *(p_fx32 + 2);
    }
}


/*---------------------------------------------------------------------------*
    getMdlScale_

    ���f����scale���g�p����
 *---------------------------------------------------------------------------*/
static void
getMdlScale_(NNSG3dJntAnmResult* pResult)
{
    u32 idxNode;
    const NNSG3dResNodeData* pNd;
    const u8* p;
    NNS_G3D_NULL_ASSERT(NNS_G3dRS);
    NNS_G3D_NULL_ASSERT(NNS_G3dRS->funcJntScale);

    // HACK ALLERT
    // NODEDESC�R�}���h���Ȃ̂ł�������f�[�^���Ƃ��Ă���
    NNS_G3D_ASSERT(NNS_G3D_GET_SBCCMD(*NNS_G3dRS->c) == NNS_G3D_SBC_NODEDESC);
    idxNode = *(NNS_G3dRS->c + 1);

    // NNSG3dResNodeData��flag�̒l�ɏ]���Ė����Ƀf�[�^�������Ă���
    // res_struct.h����NNSG3dResNodeData�̒�`���Q�Ƃ̂���
    pNd = NNS_G3dGetNodeDataByIdx(NNS_G3dRS->pResNodeInfo, idxNode);
    p = (const u8*)pNd + sizeof(*pNd);
    
    // Translation
    if (!(pNd->flag & NNS_G3D_SRTFLAG_TRANS_ZERO))
    {
        // Tx, Ty, Tz���X�L�b�v����
        p += 3 * sizeof(fx32);
    }

    // Rotation
    if (!(pNd->flag & NNS_G3D_SRTFLAG_ROT_ZERO))
    {
        // ��]�f�[�^���X�L�b�v����
        if (pNd->flag & NNS_G3D_SRTFLAG_PIVOT_EXIST)
        {
            // �s�{�b�g�`���Ȃ̂�A, B���X�L�b�v����
            p += 2 * sizeof(fx16);
        }
        else
        {
            // _01, _02, ...., ���X�L�b�v����
            p += 8 * sizeof(fx16);
        }
    }

    // Scale
    (*NNS_G3dRS->funcJntScale)(pResult, (const fx32*)p, NNS_G3dRS->c, pNd->flag);
}


// Rot3����f�[�^�������Ă���
// ��0�A��pivot�̗v�f�̈ʒu���i�[����Ă���
// �z��̍ŏ���index��pivot�̈ʒu�ƂȂ�B
static const u8 pivotUtil_[9][4] =
{
    {4, 5, 7, 8},
    {3, 5, 6, 8},
    {3, 4, 6, 7},

    {1, 2, 7, 8},
    {0, 2, 6, 8},
    {0, 1, 6, 7},

    {1, 2, 4, 5},
    {0, 2, 3, 5},
    {0, 1, 3, 4}
};


/*---------------------------------------------------------------------------*
    getMdlRot_

    ���f����rotation���g�p����
 *---------------------------------------------------------------------------*/
static void
getMdlRot_(NNSG3dJntAnmResult* pResult)
{
    u32 idxNode;
    const NNSG3dResNodeData* pNd;
    const u8* p;
    NNS_G3D_NULL_ASSERT(NNS_G3dRS);

    // HACK ALLERT
    // NODEDESC�R�}���h���Ȃ̂ł�������f�[�^���Ƃ��Ă���
    NNS_G3D_ASSERT(NNS_G3D_GET_SBCCMD(*NNS_G3dRS->c) == NNS_G3D_SBC_NODEDESC);
    idxNode = *(NNS_G3dRS->c + 1);

    // NNSG3dResNodeData��flag�̒l�ɏ]���Ė����Ƀf�[�^�������Ă���
    // res_struct.h����NNSG3dResNodeData�̒�`���Q�Ƃ̂���
    pNd = NNS_G3dGetNodeDataByIdx(NNS_G3dRS->pResNodeInfo, idxNode);
    p = (const u8*)pNd + sizeof(*pNd);
    
    // Translation
    if (!(pNd->flag & NNS_G3D_SRTFLAG_TRANS_ZERO))
    {
        // Tx, Ty, Tz���X�L�b�v����
        p += 3 * sizeof(fx32);
    }

    // Rotation
    if (!(pNd->flag & NNS_G3D_SRTFLAG_ROT_ZERO))
    {
        if (pNd->flag & NNS_G3D_SRTFLAG_PIVOT_EXIST)
        {
            // ���k����Ă���ꍇ(��Ɉꎲ��])��
            // ���k���ꂽ�s������ɖ߂�
            fx32 A = *(fx16*)(p + 0);
            fx32 B = *(fx16*)(p + sizeof(fx16));
            u32 idxPivot = (u32)( (pNd->flag & NNS_G3D_SRTFLAG_IDXPIVOT_MASK) >> 
                                            NNS_G3D_SRTFLAG_IDXPIVOT_SHIFT );
            // anmResult.rot���N���A
            MI_Zero36B(&pResult->rot);
            
            pResult->rot.a[idxPivot] =
                (pNd->flag & NNS_G3D_SRTFLAG_PIVOT_MINUS) ?
                    -FX32_ONE :
                    FX32_ONE;
            
            pResult->rot.a[pivotUtil_[idxPivot][0]] = A;
            pResult->rot.a[pivotUtil_[idxPivot][1]] = B;

            pResult->rot.a[pivotUtil_[idxPivot][2]] =
                (pNd->flag & NNS_G3D_SRTFLAG_SIGN_REVC) ? -B : B;

            pResult->rot.a[pivotUtil_[idxPivot][3]] =
                (pNd->flag & NNS_G3D_SRTFLAG_SIGN_REVD) ? -A : A;
        }
        else
        {
            // NOTICE:
            // �������R�s�[API�ɒu�������Ȃ�����
            // fx16����fx32�ւ̈Öق̃L���X�g���s���Ă��邩��

            const fx16* pp = (const fx16*)p;

            // pResult��3x3��]�s��Ƀf�[�^���Z�b�g���Ă����B
            pResult->rot.a[0] = pNd->_00;
            pResult->rot.a[1] = *(pp + 0);
            pResult->rot.a[2] = *(pp + 1);
            pResult->rot.a[3] = *(pp + 2);
            pResult->rot.a[4] = *(pp + 3);
            pResult->rot.a[5] = *(pp + 4);
            pResult->rot.a[6] = *(pp + 5);
            pResult->rot.a[7] = *(pp + 6);
            pResult->rot.a[8] = *(pp + 7);
        }
    }
    else
    {
        // pResult->rot�֏������ޑ���Ƀt���O�𗧂Ă邾���ɂ���
        // �������A�N�Z�X�Ƃ��̌�̏����R�X�g��}������B
        pResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_ROT_ZERO;
    }
}


/*---------------------------------------------------------------------------*
    getJntSRTAnmResult_

    pResult��rotation, translation, flag���Z�b�g
    pS_invS�ɃX�P�[���ƃX�P�[���̋t�����Z�b�g
 *---------------------------------------------------------------------------*/
static void
getJntSRTAnmResult_(const NNSG3dResJntAnm* pJntAnm, 
                    u32                    dataIdx, 
                    fx32                   Frame,
                    NNSG3dJntAnmResult*    pResult)
{
    NNSG3dResJntAnmSRTTag* pAnmSRTTag;
    NNSG3dJntAnmSRTTag     tag;
    const u32*             pData;
    BOOL                   IsDecimalFrame;
    fx32                   pS_invS[6]; // �|�C���^�������烍�[�J���z��ɕύX

    // ���͌���
    NNS_G3D_NULL_ASSERT(pJntAnm);
    NNS_G3D_NULL_ASSERT(pResult);
    NNS_G3D_ASSERT(dataIdx < pJntAnm->numNode);
    NNS_G3D_ASSERT(Frame < (pJntAnm->numFrame << FX32_SHIFT));
    NNS_G3D_ASSERT(pJntAnm->anmHeader.category0 == 'J' &&
                   pJntAnm->anmHeader.category1 == 'CA');

    {
        const u16* ofsTag = (const u16*)((u8*) pJntAnm + sizeof(NNSG3dResJntAnm));
        // �A�j�����\�[�X����dataIdx�Ԗڂ̃f�[�^��SRT�^�O���擾
        pAnmSRTTag = (NNSG3dResJntAnmSRTTag*)((u8*) pJntAnm + ofsTag[dataIdx]);

        tag = (NNSG3dJntAnmSRTTag)pAnmSRTTag->tag;
    }

    // �A�j���[�V�����f�[�^�����݂��Ȃ��ꍇ���`�F�b�N
    if (tag & NNS_G3D_JNTANM_SRTINFO_IDENTITY)
    {
        pResult->flag = (NNSG3dJntAnmResultFlag)(NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE |
                                                 NNS_G3D_JNTANM_RESULTFLAG_ROT_ZERO  |
                                                 NNS_G3D_JNTANM_RESULTFLAG_TRANS_ZERO);
        goto GET_JOINTSCALE;
    }

    // pAnmSRTTag�ɑ����̈��pAnmSRTTag�̒l�ɏ]�����ϒ��̃f�[�^���i�[����Ă���
    pData = (const u32*)((const u8*)pAnmSRTTag + sizeof(NNSG3dResJntAnmSRTTag));

    IsDecimalFrame = (BOOL)((Frame & (FX32_ONE - 1)) &&
                            (pJntAnm->flag & NNS_G3D_JNTANM_OPTION_INTERPOLATION));

    // �܂����Z�b�g
    pResult->flag = (NNSG3dJntAnmResultFlag) 0;

    //
    // res_struct.h��NNSG3dResJntAnmSRT�ɂ��ẴR�����g���Q�Ƃ̂���
    //
    
    //
    // Translation�̎擾
    //
    if (!(tag & (NNS_G3D_JNTANM_SRTINFO_IDENTITY_T | NNS_G3D_JNTANM_SRTINFO_BASE_T)))
    {
        // TX
        if (!(tag & NNS_G3D_JNTANM_SRTINFO_CONST_TX))
        {
            // TX�����o���֐��Ăяo��
            // �f�[�^�̃I�t�Z�b�g��pAnmSRTTag����̂��̂ɂȂ�
            if (IsDecimalFrame)
            {
                getTransDataEx_(&pResult->trans.x,
                                Frame,
                                pData,
                                pJntAnm);
            }
            else
            {
                getTransData_(&pResult->trans.x,
                              Frame,
                              pData,
                              pJntAnm);
            }

            // NNSG3dJntAnmTInfo�Ɣz��ւ̃I�t�Z�b�g�f�[�^�̕�
            pData += 2;
        }
        else
        {
            pResult->trans.x = *(fx32*)pData;

            // �萔�f�[�^�̕�
            pData += 1;
        }

        // TY
        if (!(tag & NNS_G3D_JNTANM_SRTINFO_CONST_TY))
        {
            // TY�����o���֐��Ăяo��
            // �f�[�^�̃I�t�Z�b�g��pAnmSRTTag����̂��̂ɂȂ�
            if (IsDecimalFrame)
            {
                getTransDataEx_(&pResult->trans.y,
                                Frame,
                                pData,
                                pJntAnm);
            }
            else
            {
                getTransData_(&pResult->trans.y,
                              Frame,
                              pData,
                              pJntAnm);
            }

            // NNSG3dJntAnmTInfo�Ɣz��ւ̃I�t�Z�b�g�f�[�^�̕�
            pData += 2;
        }
        else
        {
            pResult->trans.y = *(fx32*)pData;

            // �萔�f�[�^�̕�
            pData += 1;
        }

        // TZ
        if (!(tag & NNS_G3D_JNTANM_SRTINFO_CONST_TZ))
        {
            // TZ�����o���֐��Ăяo��
            // �f�[�^�̃I�t�Z�b�g��pAnmSRTTag����̂��̂ɂȂ�
            if (IsDecimalFrame)
            {
                getTransDataEx_(&pResult->trans.z,
                                Frame,
                                pData,
                                pJntAnm);
            }
            else
            {
                getTransData_(&pResult->trans.z,
                              Frame,
                              pData,
                              pJntAnm);
            }

            // NNSG3dJntAnmTInfo�Ɣz��ւ̃I�t�Z�b�g�f�[�^�̕�
            pData += 2;
        }
        else
        {
            pResult->trans.z = *(fx32*)pData;

            // �萔�f�[�^�̕�
            pData += 1;
        }
    }
    else
    {
        if (tag & NNS_G3D_JNTANM_SRTINFO_IDENTITY_T)
        {
            // Trans = (0, 0, 0)
            pResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_TRANS_ZERO;

            // �㑱�f�[�^�͑��݂��Ȃ��̂�pData��i�߂�K�v�͂Ȃ�
        }
        else
        {
            // ���f����Trans���擾
            getMdlTrans_(pResult);
        }
    }

    //
    // Rotation�̎擾
    //
    if (!(tag & (NNS_G3D_JNTANM_SRTINFO_IDENTITY_R | NNS_G3D_JNTANM_SRTINFO_BASE_R)))
    {
        if (!(tag & NNS_G3D_JNTANM_SRTINFO_CONST_R))
        {
            // R�����o���֐��Ăяo��
            // �f�[�^�̃I�t�Z�b�g��pJntAnm->ofsRot3,
            // �܂���pJntAnm->ofsRot5����̂��̂ɂȂ�
            if (IsDecimalFrame)
            {
                getRotDataEx_(&pResult->rot,
                              Frame,
                              pData,
                              pJntAnm);
            }
            else
            {
                getRotData_(&pResult->rot,
                            Frame,
                            pData,
                            pJntAnm);
            }

            // NNSG3dJntAnmRInfo�Ɣz��ւ̃I�t�Z�b�g�f�[�^�̕�
            pData += 2;
        }
        else
        {
            // const��R�����o���R�[�h
            if (getRotDataByIdx_(&pResult->rot,
                                 (void*)((u8*)pJntAnm + pJntAnm->ofsRot3),
                                 (void*)((u8*)pJntAnm + pJntAnm->ofsRot5),
                                 (NNSG3dJntAnmRIdx)*pData))
            {
                vecCross_((const VecFx32*)&pResult->rot._00,
                          (const VecFx32*)&pResult->rot._10,
                          (VecFx32*)&pResult->rot._20);
            }

            // �萔�f�[�^�̕�
            pData += 1;
        }
    }
    else
    {
        if (tag & NNS_G3D_JNTANM_SRTINFO_IDENTITY_R)
        {
            // Rot = Identity
            pResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_ROT_ZERO;

            // �㑱�f�[�^�͑��݂��Ȃ��̂�pData��i�߂�K�v�͂Ȃ�
        }
        else
        {
            // ���f����Rot���擾
            getMdlRot_(pResult);
        }
    }

    //
    // Scale�̎擾
    //
    if (!(tag & (NNS_G3D_JNTANM_SRTINFO_IDENTITY_S | NNS_G3D_JNTANM_SRTINFO_BASE_S)))
    {
        if (!(tag & NNS_G3D_JNTANM_SRTINFO_CONST_SX))
        {
            // SX�����o���֐��Ăяo��
            // �f�[�^�̃I�t�Z�b�g��pAnmSRTTag����̂��̂ɂȂ�B
            fx32 sx_invsx[2];
            if (IsDecimalFrame)
            {
                getScaleDataEx_(&sx_invsx[0],
                                Frame,
                                pData,
                                pJntAnm);
            }
            else
            {
                getScaleData_(&sx_invsx[0],
                              Frame,
                              pData,
                              pJntAnm);
            }
            *(pS_invS + 0) = sx_invsx[0];
            *(pS_invS + 3) = sx_invsx[1];
        }
        else
        {
            const fx32* p_fx32 = (const fx32*)pData;

            *(pS_invS + 0) = *(p_fx32 + 0);
            *(pS_invS + 3) = *(p_fx32 + 1);
        }

        // NNSG3dJntAnmSInfo�Ɣz��ւ̃I�t�Z�b�g�f�[�^�̕�
        pData += 2;

        if (!(tag & NNS_G3D_JNTANM_SRTINFO_CONST_SY))
        {
            // SY�����o���֐��Ăяo��
            // �f�[�^�̃I�t�Z�b�g��pAnmSRTTag����̂��̂ɂȂ�B
            fx32 sy_invsy[2];
            if (IsDecimalFrame)
            {
                getScaleDataEx_(&sy_invsy[0],
                                Frame,
                                pData,
                                pJntAnm);
            }
            else
            {
                getScaleData_(&sy_invsy[0],
                              Frame,
                              pData,
                              pJntAnm);
            }
            *(pS_invS + 1) = sy_invsy[0];
            *(pS_invS + 4) = sy_invsy[1];
        }
        else
        {
            const fx32* p_fx32 = (const fx32*)pData;

            *(pS_invS + 1) = *(p_fx32 + 0);
            *(pS_invS + 4) = *(p_fx32 + 1);

        }

        // NNSG3dJntAnmSInfo�Ɣz��ւ̃I�t�Z�b�g�f�[�^�̕�
        pData += 2;

        if (!(tag & NNS_G3D_JNTANM_SRTINFO_CONST_SZ))
        {
            // SZ�����o���֐��Ăяo��
            // �f�[�^�̃I�t�Z�b�g��pAnmSRTTag����̂��̂ɂȂ�B
            fx32 sz_invsz[2];
            if (IsDecimalFrame)
            {
                getScaleDataEx_(&sz_invsz[0],
                                Frame,
                                pData,
                                pJntAnm);
            }
            else
            {
                getScaleData_(&sz_invsz[0],
                              Frame,
                              pData,
                              pJntAnm);
            }
            *(pS_invS + 2) = sz_invsz[0];
            *(pS_invS + 5) = sz_invsz[1];
        }
        else
        {
            const fx32* p_fx32 = (const fx32*)pData;

            *(pS_invS + 2) = *(p_fx32 + 0);
            *(pS_invS + 5) = *(p_fx32 + 1);
        }

        // NNSG3dJntAnmSInfo�Ɣz��ւ̃I�t�Z�b�g�f�[�^�̕�
        pData += 2;
    }
    else
    {
        if (tag & NNS_G3D_JNTANM_SRTINFO_IDENTITY_S)
        {
            // Scale = (1, 1, 1)
            pResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE;

            // �㑱�f�[�^�͑��݂��Ȃ��̂�pData��i�߂�K�v�͂Ȃ�
        }
        else
        {
            // ���f����Scale���擾
            getMdlScale_(pResult);

            // getMdlScale_������NNSG3dJntAnmResult�̃X�P�[�����O
            // ����ݒ肵�Ă���̂Ń��^�[������
            return;
        }
    }
GET_JOINTSCALE:
    //
    // NNSG3dJntAnmResult�̃X�P�[�����O����ݒ肷��
    //
    (*NNS_G3dRS->funcJntScale)(
        pResult, 
        pS_invS,
        NNS_G3dRS->c,
        ((pResult->flag & NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE) ?
                NNS_G3D_SRTFLAG_SCALE_ONE :
                0) // �t���O�͕ϊ�����K�v������
    );
}


/*---------------------------------------------------------------------------*
    getTransData_

    translation�f�[�^���擾���A*pVal�ɓ����
 *---------------------------------------------------------------------------*/
static void
getTransData_(fx32* pVal,
              fx32 Frame,
              const u32* pData,
              const NNSG3dResJntAnm* pJntAnm)
{
    u32 frame = (u32)FX_Whole(Frame);
    const void* pArray = (const void*)((const u8*)pJntAnm + *(pData + 1));
    NNSG3dJntAnmTInfo info = (NNSG3dJntAnmTInfo)*pData;
    u32 last_interp;
    u32 idx;
    u32 idx_sub;

    NNS_G3D_NULL_ASSERT(pVal);
    NNS_G3D_NULL_ASSERT(pArray);

    if (!(info & NNS_G3D_JNTANM_TINFO_STEP_MASK))
    {
        // NNS_G3D_JNTANM_TINFO_STEP1���I������Ă���
        idx = frame;
        goto TRANS_NONINTERP;
    }

    // last_interp�ȍ~��1�R�}���f�[�^�������Ă���
    // last_interp��2�̔{����4�̔{���ł���B
    last_interp = ((u32)info & NNS_G3D_JNTANM_TINFO_LAST_INTERP_MASK) >>
                                    NNS_G3D_JNTANM_TINFO_LAST_INTERP_SHIFT;

    if (info & NNS_G3D_JNTANM_TINFO_STEP_2)
    {
        // NNS_G3D_JNTANM_TINFO_STEP_2���I������Ă���
        if (frame & 1)
        {
            if (frame > last_interp)
            {
                // �ŏI�t���[���ȊO���肦�Ȃ�
                idx = (last_interp >> 1) + 1;
                goto TRANS_NONINTERP;
            }
            else
            {
                // ��ōŏI�t���[���łȂ��̂�50:50�̕�ԏ���������B
                idx = frame >> 1;
                goto TRANS_INTERP_2;
            }
        }
        else
        {
            // �����t���[���Ȃ̂ŕ�ԏ����͂���Ȃ�
            idx = frame >> 1;
            goto TRANS_NONINTERP;
        }
    }
    else
    {
        // NNS_G3D_JNTANM_TINFO_STEP_4���I������Ă���
        if (frame & 3)
        {
            if (frame > last_interp)
            {
                idx = (last_interp >> 2) + (frame & 3);
                goto TRANS_NONINTERP;
            }

            // ��ԏ�������
            if (frame & 1)
            {
                fx32 v, v_sub;
                if (frame & 2)
                {
                    // 3:1�̈ʒu�ŕ��
                    idx_sub = (frame >> 2);
                    idx = idx_sub + 1;
                }
                else
                {
                    // 1:3�̈ʒu�ŕ��
                    idx = (frame >> 2);
                    idx_sub = idx + 1;
                }
                
                // 1:3, 3:1�̏ꍇ�̕��
                if (info & NNS_G3D_JNTANM_TINFO_FX16ARRAY)
                {
                    const fx16* p_fx16 = (const fx16*)pArray;

                    v = *(p_fx16 + idx);
                    v_sub = *(p_fx16 + idx_sub);
                    *pVal = (v + v + v + v_sub) >> 2; // v:v_sub��3:1�Ńu�����h
                }
                else
                {
                    const fx32* p_fx32 = (const fx32*)pArray;

                    v = *(p_fx32 + idx);
                    v_sub = *(p_fx32 + idx_sub);
                    // v:v_sub��3:1�Ńu�����h�B�I�[�o�[�t���[������邽��fx64�Ōv�Z
                    *pVal = (fx32)(((fx64)v + v + v + v_sub) >> 2);
                }
                return;
            }
            else
            {
                // 50:50�̕�ԂɂȂ�
                idx = frame >> 2;
                goto TRANS_INTERP_2;
            }
        }
        else
        {
            // �t���[���͒��x4�̔{���ɂȂ��Ă���B
            idx = frame >> 2;
            goto TRANS_NONINTERP;
        }
    }
    NNS_G3D_ASSERT(0);
TRANS_INTERP_2:
    if (info & NNS_G3D_JNTANM_TINFO_FX16ARRAY)
    {
        const fx16* p_fx16 = (const fx16*)pArray;

        *pVal = (*(p_fx16 + idx) + *(p_fx16 + idx + 1)) >> 1;
    }
    else
    {
        const fx32* p_fx32 = (const fx32*)pArray;

        fx32 v1 = *(p_fx32 + idx) >> 1;
        fx32 v2 = *(p_fx32 + idx + 1) >> 1;
        *pVal = v1 + v2;
    }
    return;
TRANS_NONINTERP:
    if (info & NNS_G3D_JNTANM_TINFO_FX16ARRAY)
    {
        *pVal = *((const fx16*)pArray + idx);
    }
    else
    {
        *pVal = *((const fx32*)pArray + idx);
    }
    return;
}


/*---------------------------------------------------------------------------*
    getTransDataEx_

    translation�f�[�^���擾���A*pVal�ɓ����B
    �����_�ȉ��̏ꍇ�͕�Ԃ��s���B
 *---------------------------------------------------------------------------*/
static void
getTransDataEx_(fx32* pVal,
                fx32 Frame,
                const u32* pData,
                const NNSG3dResJntAnm* pJntAnm)
{
    // ���̊֐��ɂ́A
    // 0 < Frame < numFrame�ƂȂ�悤��Frame�������Ȃ��悤�ɂȂ��Ă��܂��B
    const void* pArray = (const void*)((const u8*)pJntAnm + *(pData + 1));
    NNSG3dJntAnmTInfo info = (NNSG3dJntAnmTInfo)*pData;

    u32 last_interp;
    u32 idx;
    fx32 remainder;
    int step;
    u32 step_shift;
    u32 frame;

    NNS_G3D_NULL_ASSERT(pVal);
    NNS_G3D_NULL_ASSERT(pArray);

    frame = (u32)FX_Whole(Frame);

    if (frame == pJntAnm->numFrame - 1)
    {
        //
        // Frame��numFrame - 1��numFrame�̊Ԃ̏ꍇ
        // ���傤�ǍŏI�t���[������ŏ��̃t���[���̊Ԃɂ���ꍇ
        //

        //
        // �ŏ��ɍŏI�t���[���f�[�^�̃C���f�b�N�X���擾
        //
        if (!(info & NNS_G3D_JNTANM_TINFO_STEP_MASK))
        {
            idx = frame;
        }
        else if (info & NNS_G3D_JNTANM_TINFO_STEP_2)
        {
            idx = (frame >> 1) + (frame & 1);
        }
        else
        {
            idx = (frame >> 2) + (frame & 3);
        }

        //
        // ��O���邩�ŏI�̃f�[�^��Ԃ����̓t���O�Ō��܂��Ă���B
        //
        if (pJntAnm->flag & NNS_G3D_JNTANM_OPTION_END_TO_START_INTERPOLATION)
        {
            // ��O����ꍇ�͍ŏI�f�[�^�ƍŏ��̃f�[�^�ōs��(���[�v)
            fx32 v0, v1;
            remainder = Frame & (FX32_ONE - 1);

            if (info & NNS_G3D_JNTANM_TINFO_FX16ARRAY)
            {
                const fx16* p_fx16 = (const fx16*)pArray;

                v0 = *(p_fx16 + idx);
                v1 = *p_fx16;
            }
            else
            {
                const fx32* p_fx32 = (const fx32*)pArray;

                v0 = *(p_fx32 + idx);
                v1 = *p_fx32;
            }

            *pVal = v0 + (((v1 - v0) * remainder) >> FX32_SHIFT);
            return;
        }
        else
        {
            // �ŏI�̃f�[�^��Ԃ��ꍇ
            if (info & NNS_G3D_JNTANM_TINFO_FX16ARRAY)
            {
                *pVal = *((const fx16*)pArray + idx);
            }
            else
            {
                *pVal = *((const fx32*)pArray + idx);
            }
            return;
        }
    }

    if (!(info & NNS_G3D_JNTANM_TINFO_STEP_MASK))
    {
        // NNS_G3D_JNTANM_TINFO_STEP1���I������Ă���
        goto TRANS_EX_0;
    }

    // last_interp�ȍ~��1�R�}���f�[�^�������Ă���
    // last_interp��2�̔{����4�̔{���ł���B
    last_interp = ((u32)info & NNS_G3D_JNTANM_TINFO_LAST_INTERP_MASK) >>
                                    NNS_G3D_JNTANM_TINFO_LAST_INTERP_SHIFT;
    
    if (info & NNS_G3D_JNTANM_TINFO_STEP_2)
    {
        // NNS_G3D_JNTANM_TINFO_STEP_2���I������Ă���
        // �������܂܂��̂͐؂�̂Ă�ꂽ�f�[�^���l�����邩��
        if (frame >= last_interp)
        {
            // �ŏI�t���[���ȊO���肦�Ȃ�
            idx = (last_interp >> 1);
            goto TRANS_EX_0_1;
        }
        else
        {
            idx = frame >> 1;
            remainder = Frame & (FX32_ONE * 2 - 1);
            step = 2;
            step_shift = 1;
            goto TRANS_EX;
        }
    }
    else
    {
        // NNS_G3D_JNTANM_TINFO_STEP_4���I������Ă���
        if (frame >= last_interp)
        {
            // ���̊֐����Ăяo���������ɂ��Aframe���ŏI�f�[�^�ł��邱�Ƃ͂Ȃ�
            // �̂ňȉ��̃R�[�h�ŋ��E���͂ݏo�邱�Ƃ͂Ȃ��B
            idx = (frame >> 2) + (frame & 3);
            goto TRANS_EX_0_1;
        }
        else
        {
            idx = frame >> 2;
            remainder = Frame & (FX32_ONE * 4 - 1);
            step = 4;
            step_shift = 2;
            goto TRANS_EX;
        }
    }
    NNS_G3D_ASSERT(0);
TRANS_EX_0:
    idx = (u32)frame;
TRANS_EX_0_1:
    remainder = Frame & (FX32_ONE - 1);
    step = 1;
    step_shift = 0;
TRANS_EX:
    {
        fx32 v0, v1;
        if (info & NNS_G3D_JNTANM_TINFO_FX16ARRAY)
        {
            const fx16* p_fx16 = (const fx16*)pArray;

            v0 = *(p_fx16 + idx);
            v1 = *(p_fx16 + idx + 1);
        }
        else
        {
            const fx32* p_fx32 = (const fx32*)pArray;

            v0 = *(p_fx32 + idx);
            v1 = *(p_fx32 + idx + 1);
        }

        *pVal = ((v0 * step) + (((v1 - v0) * remainder) >> FX32_SHIFT)) >> step_shift;
    }
    return;
}


/*---------------------------------------------------------------------------*
    getScaleData_

    scale�Ƃ��̋t�����擾���As_invs[0], s_invs[1]�ɓ����
 *---------------------------------------------------------------------------*/
static void
getScaleData_(fx32* s_invs,
              fx32 Frame,
              const u32* pData,
              const NNSG3dResJntAnm* pJntAnm)
{
    u32 frame = (u32)FX_Whole(Frame);
    const void* pArray = (const void*)((u8*)pJntAnm + *(pData + 1));
    NNSG3dJntAnmSInfo info = (NNSG3dJntAnmSInfo)*pData;
    u32 last_interp;
    u32 idx;
    u32 idx_sub;

    NNS_G3D_NULL_ASSERT(s_invs);
    NNS_G3D_NULL_ASSERT(pArray);

    if (!(info & NNS_G3D_JNTANM_SINFO_STEP_MASK))
    {
        // NNS_G3D_JNTANM_SINFO_STEP_1���I������Ă���
        idx = frame;
        goto SCALE_NONINTERP;
    }

    // last_interp�ȍ~��1�R�}���f�[�^�������Ă���
    // last_interp��2�̔{����4�̔{���ł���B
    last_interp = ((u32)info & NNS_G3D_JNTANM_SINFO_LAST_INTERP_MASK) >>
                                    NNS_G3D_JNTANM_SINFO_LAST_INTERP_SHIFT;

    if (info & NNS_G3D_JNTANM_SINFO_STEP_2)
    {
        // NNS_G3D_JNTANM_SINFO_STEP_2���I������Ă���
        if (frame & 1)
        {
            if (frame > last_interp)
            {
                // �ŏI�t���[���ȊO���肦�Ȃ�
                idx = (last_interp >> 1) + 1;
                goto SCALE_NONINTERP;
            }
            else
            {
                // ��ōŏI�t���[���łȂ��̂�50:50�̕�ԏ���������B
                idx = frame >> 1;
                goto SCALE_INTERP_2;
            }
        }
        else
        {
            // �����t���[���Ȃ̂ŕ�ԏ����͂���Ȃ�
            idx = frame >> 1;
            goto SCALE_NONINTERP;
        }
    }
    else
    {
        // NNS_G3D_JNTANM_TINFO_STEP_4���I������Ă���
        if (frame & 3)
        {
            if (frame > last_interp)
            {
                // last_interp�ȍ~�͕�ԏ����Ȃ�
                idx = (last_interp >> 2) + (frame & 3);
                goto SCALE_NONINTERP;
            }

            // ��ԏ�������
            if (frame & 1)
            {
                fx32 v, v_sub;
                if (frame & 2)
                {
                    // 3:1�̈ʒu�ŕ��
                    idx_sub = (frame >> 2);
                    idx = idx_sub + 1;
                }
                else
                {
                    // 1:3�̈ʒu�ŕ��
                    idx = (frame >> 2);
                    idx_sub = idx + 1;
                }
                
                if (info & NNS_G3D_JNTANM_SINFO_FX16ARRAY)
                {
                    const fx16* p_fx16 = (const fx16*)pArray;

                    // scale�̌v�Z
                    v = *(p_fx16 + 2 * idx);
                    v_sub = *(p_fx16 + 2 * idx_sub);
                    s_invs[0] = (v + (v << 1) + v_sub) >> 2; // v:v_sub��3:1�Ńu�����h

                    // inverse scale�̌v�Z
                    v = *(p_fx16 + 2 * idx + 1);
                    v_sub = *(p_fx16 + 2 * idx_sub + 1);
                    s_invs[1] = (v + (v << 1) + v_sub) >> 2; // v:v_sub��3:1�Ńu�����h
                }
                else
                {
                    const fx32* p_fx32 = (const fx32*)pArray;

                    // scale�̌v�Z
                    v = *(p_fx32 + 2 * idx);
                    v_sub = *(p_fx32 + 2 * idx_sub);
                    // v:v_sub��3:1�Ńu�����h�B�I�[�o�[�t���[������邽��fx64�Ōv�Z
                    s_invs[0] = (fx32)(((fx64)v + v + v + v_sub) >> 2);

                    // inverse scale�̌v�Z
                    v = *(p_fx32 + 2 * idx + 1);
                    v_sub = *(p_fx32 + 2 * idx_sub + 1);
                    // v:v_sub��3:1�Ńu�����h�B�I�[�o�[�t���[������邽��fx64�Ōv�Z
                    s_invs[1] = (fx32)(((fx64)v + v + v + v_sub) >> 2);
                }
                return;
            }
            else
            {
                // 50:50�̕�ԂɂȂ�
                idx = frame >> 2;
                goto SCALE_INTERP_2;
            }
        }
        else
        {
            // �t���[���͒��x4�̔{���ɂȂ��Ă���B
            idx = frame >> 2;
            goto SCALE_NONINTERP;
        }
    }
SCALE_NONINTERP:
    if (info & NNS_G3D_JNTANM_SINFO_FX16ARRAY)
    {
        const fx16* p_fx16 = (const fx16*)pArray;

        s_invs[0] = *(p_fx16 + 2 * idx);
        s_invs[1] = *(p_fx16 + 2 * idx + 1);
    }
    else
    {
        const fx32* p_fx32 = (const fx32*)pArray;

        s_invs[0] = *(p_fx32 + 2 * idx);
        s_invs[1] = *(p_fx32 + 2 * idx + 1);
    }
    return;
SCALE_INTERP_2:
    if (info & NNS_G3D_JNTANM_SINFO_FX16ARRAY)
    {
        const fx16* p_fx16 = (const fx16*)pArray;

        s_invs[0] = (*(p_fx16 + 2 * idx) + *(p_fx16 + 2 * idx + 2)) >> 1;
        s_invs[1] = (*(p_fx16 + 2 * idx + 1) + *(p_fx16 + 2 * idx + 3)) >> 1;
    }
    else
    {
        const fx32* p_fx32 = (const fx32*)pArray;

        s_invs[0] = (*(p_fx32 + 2 * idx) + (*(p_fx32 + 2 * idx + 2))) >> 1;
        s_invs[1] = (*(p_fx32 + 2 * idx + 1) + (*(p_fx32 + 2 * idx + 3))) >> 1;
    }
    return;
}


/*---------------------------------------------------------------------------*
    getScaleDataEx_

    scale�Ƃ��̋t�����擾���As_invs[0], s_invs[1]�ɓ����
    �����_�ȉ��̏ꍇ�͕�Ԃ��s���B
 *---------------------------------------------------------------------------*/
static void
getScaleDataEx_(fx32* s_invs,
                fx32 Frame,
                const u32* pData,
                const NNSG3dResJntAnm* pJntAnm)
{
    const void* pArray = (const void*)((const u8*)pJntAnm + *(pData + 1));
    NNSG3dJntAnmSInfo info = (NNSG3dJntAnmSInfo)*pData;
    u32 last_interp;
    u32 idx0, idx1;
    fx32 remainder;
    int step;
    u32 step_shift;
    u32 frame;

    frame = (u32)FX_Whole(Frame);

    if (frame == pJntAnm->numFrame - 1)
    {
        //
        // Frame��numFrame - 1��numFrame�̊Ԃ̏ꍇ
        // ���傤�ǍŏI�t���[������ŏ��̃t���[���̊Ԃɂ���ꍇ
        //

        //
        // �ŏ��ɍŏI�t���[���f�[�^�̃C���f�b�N�X���擾
        //
        if (!(info & NNS_G3D_JNTANM_SINFO_STEP_MASK))
        {
            idx0 = frame;
        }
        else if (info & NNS_G3D_JNTANM_SINFO_STEP_2)
        {
            idx0 = (frame >> 1) + (frame & 1);
        }
        else
        {
            idx0 = (frame >> 2) + (frame & 3);
        }

        //
        // ��O���邩�ŏI�̃f�[�^��Ԃ����̓t���O�Ō��܂��Ă���B
        //
        if (pJntAnm->flag & NNS_G3D_JNTANM_OPTION_END_TO_START_INTERPOLATION)
        {
            idx1 = 0;
            goto SCALE_EX_0_1;
        }
        else
        {
            if (info & NNS_G3D_JNTANM_SINFO_FX16ARRAY)
            {
                const fx16* p_fx16 = (const fx16*)pArray;

                s_invs[0] = *(p_fx16 + 2 * idx0);
                s_invs[1] = *(p_fx16 + 2 * idx0 + 1);
            }
            else
            {
                const fx32* p_fx32 = (const fx32*)pArray;

                s_invs[0] = *(p_fx32 + 2 * idx0);
                s_invs[1] = *(p_fx32 + 2 * idx0 + 1);
            }
            return;
        }
    }

    if (!(info & NNS_G3D_JNTANM_SINFO_STEP_MASK))
    {
        // NNS_G3D_JNTANM_SINFO_STEP_1���I������Ă���
        goto SCALE_EX_0;
    }

    last_interp = ((u32)info & NNS_G3D_JNTANM_SINFO_LAST_INTERP_MASK) >>
                                    NNS_G3D_JNTANM_SINFO_LAST_INTERP_SHIFT;

    if (info & NNS_G3D_JNTANM_SINFO_STEP_2)
    {
        // NNS_G3D_JNTANM_SINFO_STEP_2���I������Ă���
        // �������܂܂��̂͐؂�̂Ă�ꂽ�f�[�^���l�����邩��
        if (frame >= last_interp)
        {
            // �ŏI�t���[���ȊO���肦�Ȃ�
            idx0 = (last_interp >> 1);
            idx1 = idx0 + 1;
            goto SCALE_EX_0_1;
        }
        else
        {
            idx0 = frame >> 1;
            idx1 = idx0 + 1;
            remainder = Frame & (FX32_ONE * 2 - 1);
            step = 2;
            step_shift = 1;
            goto SCALE_EX;
        }
    }
    else
    {
        // NNS_G3D_JNTANM_SINFO_STEP_4���I������Ă���
        if (frame >= last_interp)
        {
            // ���̊֐����Ăяo���������ɂ��Aframe���ŏI�f�[�^�ł��邱�Ƃ͂Ȃ�
            // �̂ňȉ��̃R�[�h�ŋ��E���͂ݏo�邱�Ƃ͂Ȃ��B
            idx0 = (frame >> 2) + (frame & 3);
            idx1 = idx0 + 1;
            goto SCALE_EX_0_1;
        }
        else
        {
            idx0 = frame >> 2;
            idx1 = idx0 + 1;
            remainder = Frame & (FX32_ONE * 4 - 1);
            step = 4;
            step_shift = 2;
            goto SCALE_EX;
        }
    }
    NNS_G3D_ASSERT(0);
SCALE_EX_0:
    idx0 = (u32)frame;
    idx1 = idx0 + 1;
SCALE_EX_0_1:
    remainder = Frame & (FX32_ONE - 1);
    step = 1;
    step_shift = 0;
SCALE_EX:
    {
        fx32 s0, s1;
        fx32 inv0, inv1;

        if (info & NNS_G3D_JNTANM_SINFO_FX16ARRAY)
        {
            const fx16* p_fx16 = (const fx16*)pArray;

            s0   = *(p_fx16 + 2 * idx0);
            inv0 = *(p_fx16 + 2 * idx0 + 1);

            s1   = *(p_fx16 + 2 * idx1);
            inv1 = *(p_fx16 + 2 * idx1 + 1);
        }
        else
        {
            const fx32* p_fx32 = (const fx32*)pArray;

            s0   = *(p_fx32 + 2 * idx0);
            inv0 = *(p_fx32 + 2 * idx0 + 1);

            s1   = *(p_fx32 + 2 * idx1);
            inv1 = *(p_fx32 + 2 * idx1 + 1);
        }

        s_invs[0] = ((s0 * step) + (((s1 - s0) * remainder) >> FX32_SHIFT)) >> step_shift;
        s_invs[1] = ((inv0 * step) + (((inv1 - inv0) * remainder) >> FX32_SHIFT)) >> step_shift;
    }

    return;
}



// �t���[���Ԃ̕�ԏ������K�v�ƂȂ�ꍇ�ɁAG3D_NORMALIZE_ROT_MTX�� 
// �����ɂȂ��Ă���ƒP�Ȃ�s��̐��`��Ԃ����s�Ȃ�Ȃ����߁A�L�[�t���[���Ԃ�
// ��]�p���傫���ꍇ�ɂ̓��f���̌`�󂪘c�ޏꍇ������܂��B
// G3D_NORMALIZE_ROT_MTX���L���ł���ꍇ�ɂ́A���K���������s�Ȃ�����
// ���f���̘c�݂�}���邱�Ƃ��ł��܂��B
#define G3D_NORMALIZE_ROT_MTX

#ifdef G3D_NORMALIZE_ROT_MTX
#define ROT_FILTER_SHIFT    0
#else
#define ROT_FILTER_SHIFT    1
#endif

/*---------------------------------------------------------------------------*
    getRotData_

    ��]�s����擾���A*pRot�ɃZ�b�g����
 *---------------------------------------------------------------------------*/
static void
getRotData_(MtxFx33* pRot,
            fx32 Frame,
            const u32* pData,
            const NNSG3dResJntAnm* pJntAnm)
{
    u32 frame = (u32)FX_Whole(Frame);
    const void* pArray = (const void*)((const u8*)pJntAnm + *(pData + 1));
    const void* pArrayRot3 = (const void*)((const u8*)pJntAnm + pJntAnm->ofsRot3);
    const void* pArrayRot5 = (const void*)((const u8*)pJntAnm + pJntAnm->ofsRot5);
    NNSG3dJntAnmRInfo info = (NNSG3dJntAnmRInfo)*pData;
    const u16* p = (const u16*)pArray;

    u32 last_interp;
    u32 idx;
    u32 idx_sub;

    if (!(info & NNS_G3D_JNTANM_RINFO_STEP_MASK))
    {
        // NNS_G3D_JNTANM_RINFO_STEP_1���I������Ă���
        idx = frame;
        goto ROT_NONINTERP;
    }

    last_interp = ((u32)info & NNS_G3D_JNTANM_RINFO_LAST_INTERP_MASK) >>
                                    NNS_G3D_JNTANM_RINFO_LAST_INTERP_SHIFT;

    if (info & NNS_G3D_JNTANM_RINFO_STEP_2)
    {
        // NNS_G3D_JNTANM_RINFO_STEP_2���I������Ă���
        if (frame & 1)
        {
            if (frame > last_interp)
            {
                // �ŏI�t���[���ȊO���肦�Ȃ�
                idx = (last_interp >> 1) + 1;
                goto ROT_NONINTERP;
            }
            else
            {
                // ��ōŏI�t���[���łȂ��̂�50:50�̕�ԏ���������B
                idx = frame >> 1;
                goto ROT_INTERP_2;
            }
        }
        else
        {
            // �����t���[���Ȃ̂ŕ�ԏ����͂���Ȃ�
            idx = frame >> 1;
            goto ROT_NONINTERP;
        }
    }
    else
    {
        // NNS_G3D_JNTANM_RINFO_STEP_4���I������Ă���
        if (frame & 3)
        {
            if (frame > last_interp)
            {
                idx = (last_interp >> 2) + (frame & 3);
                goto ROT_NONINTERP;
            }

            // ��ԏ�������
            if (frame & 1)
            {
                MtxFx33 tmp;
                BOOL    doCross = FALSE;
                if (frame & 2)
                {
                    // 3:1�̈ʒu�ŕ��
                    idx_sub = (frame >> 2);
                    idx = idx_sub + 1;
                }
                else
                {
                    // 1:3�̈ʒu�ŕ��
                    idx = (frame >> 2);
                    idx_sub = idx + 1;
                }
                doCross |= getRotDataByIdx_(pRot,
                                            pArrayRot3,
                                            pArrayRot5,
                                            (NNSG3dJntAnmRIdx)p[idx]);
                doCross |= getRotDataByIdx_(&tmp,
                                            pArrayRot3,
                                            pArrayRot5,
                                            (NNSG3dJntAnmRIdx)p[idx_sub]);
                pRot->_00 = (pRot->_00 * 3 + tmp._00) >> (2 * ROT_FILTER_SHIFT);
                pRot->_01 = (pRot->_01 * 3 + tmp._01) >> (2 * ROT_FILTER_SHIFT);
                pRot->_02 = (pRot->_02 * 3 + tmp._02) >> (2 * ROT_FILTER_SHIFT);
                pRot->_10 = (pRot->_10 * 3 + tmp._10) >> (2 * ROT_FILTER_SHIFT);
                pRot->_11 = (pRot->_11 * 3 + tmp._11) >> (2 * ROT_FILTER_SHIFT);
                pRot->_12 = (pRot->_12 * 3 + tmp._12) >> (2 * ROT_FILTER_SHIFT);
                
#ifdef G3D_NORMALIZE_ROT_MTX
                VEC_Normalize( (VecFx32*)(&pRot->_00), (VecFx32*)(&pRot->_00) );
                VEC_Normalize( (VecFx32*)(&pRot->_10), (VecFx32*)(&pRot->_10) );
#endif
                
                if (!doCross)
                {
                    // �����Ƃ�Rot3�`���̃f�[�^�������ꍇ��
                    // �O�ς̌v�Z������邱�Ƃɂ���
                    pRot->_20 = (pRot->_20 * 3 + tmp._20) >> (2 * ROT_FILTER_SHIFT);
                    pRot->_21 = (pRot->_21 * 3 + tmp._21) >> (2 * ROT_FILTER_SHIFT);
                    pRot->_22 = (pRot->_22 * 3 + tmp._22) >> (2 * ROT_FILTER_SHIFT);

#ifdef G3D_NORMALIZE_ROT_MTX
                    VEC_Normalize( (VecFx32*)(&pRot->_20), (VecFx32*)(&pRot->_20) );
#endif
                }
                else
                {
                    vecCross_((const VecFx32*)&pRot->_00,
                              (const VecFx32*)&pRot->_10,
                              (VecFx32*)&pRot->_20);
                }
                return;
            }
            else
            {
                // 50:50�̕�ԂɂȂ�
                idx = frame >> 2;
                goto ROT_INTERP_2;
            }
        }
        else
        {
            idx = frame >> 2;
            goto ROT_NONINTERP;
        }
    }
ROT_INTERP_2:
    {
        MtxFx33 tmp;
        BOOL    doCross = FALSE;
        doCross |= getRotDataByIdx_(pRot,
                                    pArrayRot3,
                                    pArrayRot5,
                                    (NNSG3dJntAnmRIdx)p[idx]);
        doCross |= getRotDataByIdx_(&tmp,
                                    pArrayRot3,
                                    pArrayRot5,
                                    (NNSG3dJntAnmRIdx)p[idx + 1]);
        pRot->_00 = (pRot->_00 + tmp._00) >> ROT_FILTER_SHIFT;
        pRot->_01 = (pRot->_01 + tmp._01) >> ROT_FILTER_SHIFT;
        pRot->_02 = (pRot->_02 + tmp._02) >> ROT_FILTER_SHIFT;
        pRot->_10 = (pRot->_10 + tmp._10) >> ROT_FILTER_SHIFT;
        pRot->_11 = (pRot->_11 + tmp._11) >> ROT_FILTER_SHIFT;
        pRot->_12 = (pRot->_12 + tmp._12) >> ROT_FILTER_SHIFT;
        
#ifdef G3D_NORMALIZE_ROT_MTX
        VEC_Normalize( (VecFx32*)(&pRot->_00), (VecFx32*)(&pRot->_00) );
        VEC_Normalize( (VecFx32*)(&pRot->_10), (VecFx32*)(&pRot->_10) );
#endif
        
        if (!doCross)
        {
            pRot->_20 = (pRot->_20 + tmp._20) >> ROT_FILTER_SHIFT;
            pRot->_21 = (pRot->_21 + tmp._21) >> ROT_FILTER_SHIFT;
            pRot->_22 = (pRot->_22 + tmp._22) >> ROT_FILTER_SHIFT;
            
#ifdef G3D_NORMALIZE_ROT_MTX
            VEC_Normalize( (VecFx32*)(&pRot->_20), (VecFx32*)(&pRot->_20) );
#endif
        }
        else
        {
            vecCross_((const VecFx32*)&pRot->_00,
                      (const VecFx32*)&pRot->_10,
                      (VecFx32*)&pRot->_20);
        }
        return;
    }
ROT_NONINTERP:
    if (getRotDataByIdx_(pRot,
                         pArrayRot3,
                         pArrayRot5,
                         (NNSG3dJntAnmRIdx)p[idx]))
    {
        vecCross_((const VecFx32*)&pRot->_00,
                  (const VecFx32*)&pRot->_10,
                  (VecFx32*)&pRot->_20);
    }
    else
    {
#ifdef G3D_NORMALIZE_ROT_MTX
        VEC_Normalize( (VecFx32*)(&pRot->_20), (VecFx32*)(&pRot->_20) );
#endif
    }
    return;
}


/*---------------------------------------------------------------------------*
    getRotData_

    ��]�s����擾���A*pRot�ɃZ�b�g����B
    �����_�ȉ��̏ꍇ�͕�Ԃ��s���B
 *---------------------------------------------------------------------------*/
static void
getRotDataEx_(MtxFx33* pRot,
              fx32 Frame,
              const u32* pData,
              const NNSG3dResJntAnm* pJntAnm)
{
    // ���̊֐��ɂ́A
    // 0 < Frame < numFrame�ƂȂ�悤��Frame�������Ȃ��悤�ɂȂ��Ă��܂��B

    const void* pArray = (const void*)((const u8*)pJntAnm + *(pData + 1));
    const void* pArrayRot3 = (const void*)((const u8*)pJntAnm + pJntAnm->ofsRot3);
    const void* pArrayRot5 = (const void*)((const u8*)pJntAnm + pJntAnm->ofsRot5);
    NNSG3dJntAnmRInfo info = (NNSG3dJntAnmRInfo)*pData;

    u32 last_interp;
    u32 idx0, idx1;
    fx32 remainder;
    int step;
    u32 step_shift;
    u32 frame;
    const u16* p = (const u16*)pArray;

    frame = (u32)FX_Whole(Frame);

    if (frame == pJntAnm->numFrame - 1)
    {
        //
        // Frame��numFrame - 1��numFrame�̊Ԃ̏ꍇ
        // ���傤�ǍŏI�t���[������ŏ��̃t���[���̊Ԃɂ���ꍇ
        //

        //
        // �ŏ��ɍŏI�t���[���f�[�^�̃C���f�b�N�X���擾
        //
        if (!(info & NNS_G3D_JNTANM_RINFO_STEP_MASK))
        {
            idx0 = frame;
        }
        else if (info & NNS_G3D_JNTANM_RINFO_STEP_2)
        {
            idx0 = (frame >> 1) + (frame & 1);
        }
        else
        {
            idx0 = (frame >> 2) + (frame & 3);
        }

        //
        // ��O���邩�ŏI�̃f�[�^��Ԃ����̓t���O�Ō��܂��Ă���B
        //
        if (pJntAnm->flag & NNS_G3D_JNTANM_OPTION_END_TO_START_INTERPOLATION)
        {
            // ��O����ꍇ�͍ŏI�f�[�^�ƍŏ��̃f�[�^�ōs��(���[�v)
            idx1 = 0;
            goto ROT_EX_0_1;
        }
        else
        {
            // �ŏI�̃f�[�^��Ԃ��ꍇ
            if (getRotDataByIdx_(pRot,
                                 pArrayRot3,
                                 pArrayRot5,
                                 (NNSG3dJntAnmRIdx)p[idx0]))
            {
                vecCross_((const VecFx32*)&pRot->_00,
                          (const VecFx32*)&pRot->_10,
                          (VecFx32*)&pRot->_20);
            }
            else
            {
#ifdef G3D_NORMALIZE_ROT_MTX
                VEC_Normalize( (VecFx32*)(&pRot->_20), (VecFx32*)(&pRot->_20) );
#endif
            }
            return;
        }
    }

    if (!(info & NNS_G3D_JNTANM_RINFO_STEP_MASK))
    {
        // NNS_G3D_JNTANM_RINFO_STEP_1���I������Ă���
        goto ROT_EX_0;
    }

    last_interp = ((u32)info & NNS_G3D_JNTANM_RINFO_LAST_INTERP_MASK) >>
                                    NNS_G3D_JNTANM_RINFO_LAST_INTERP_SHIFT;

    if (info & NNS_G3D_JNTANM_RINFO_STEP_2)
    {
        // NNS_G3D_JNTANM_RINFO_STEP_2���I������Ă���
        // �������܂܂��̂͐؂�̂Ă�ꂽ�f�[�^���l�����邩��
        if (frame >= last_interp)
        {
            // �ŏI�t���[���ȊO���肦�Ȃ�
            idx0 = (last_interp >> 1);
            idx1 = idx0 + 1;
            goto ROT_EX_0_1;
        }
        else
        {
            idx0 = frame >> 1;
            idx1 = idx0 + 1;
            remainder = Frame & (FX32_ONE * 2 - 1);
            step = 2;
            step_shift = 1;
            goto ROT_EX;
        }
    }
    else
    {
        // NNS_G3D_JNTANM_RINFO_STEP_4���I������Ă���
        if (frame >= last_interp)
        {
            // ���̊֐����Ăяo���������ɂ��Aframe���ŏI�f�[�^�ł��邱�Ƃ͂Ȃ�
            // �̂ňȉ��̃R�[�h�ŋ��E���͂ݏo�邱�Ƃ͂Ȃ��B
            idx0 = (frame >> 2) + (frame & 3);
            idx1 = idx0 + 1;
            goto ROT_EX_0_1;
        }
        else
        {
            idx0 = frame >> 2;
            idx1 = idx0 + 1;
            remainder = Frame & (FX32_ONE * 4 - 1);
            step = 4;
            step_shift = 2;
            goto ROT_EX;
        }
    }
    NNS_G3D_ASSERT(0);
ROT_EX_0:
    idx0 = (u32)frame;
    idx1 = idx0 + 1;
ROT_EX_0_1:
    remainder = Frame & (FX32_ONE - 1);
    step = 1;
    step_shift = 0;
ROT_EX:
    {
        MtxFx33 r0, r1;
        BOOL doCross = FALSE;
        doCross |= getRotDataByIdx_(&r0,
                                    pArrayRot3,
                                    pArrayRot5,
                                    (NNSG3dJntAnmRIdx)p[idx0]);
        doCross |= getRotDataByIdx_(&r1,
                                    pArrayRot3,
                                    pArrayRot5,
                                    (NNSG3dJntAnmRIdx)p[idx1]);

        pRot->_00 = ((r0._00 * step) + (((r1._00 - r0._00) * remainder) >> FX32_SHIFT)) >> (step_shift * ROT_FILTER_SHIFT);
        pRot->_01 = ((r0._01 * step) + (((r1._01 - r0._01) * remainder) >> FX32_SHIFT)) >> (step_shift * ROT_FILTER_SHIFT);
        pRot->_02 = ((r0._02 * step) + (((r1._02 - r0._02) * remainder) >> FX32_SHIFT)) >> (step_shift * ROT_FILTER_SHIFT);
        pRot->_10 = ((r0._10 * step) + (((r1._10 - r0._10) * remainder) >> FX32_SHIFT)) >> (step_shift * ROT_FILTER_SHIFT);
        pRot->_11 = ((r0._11 * step) + (((r1._11 - r0._11) * remainder) >> FX32_SHIFT)) >> (step_shift * ROT_FILTER_SHIFT);
        pRot->_12 = ((r0._12 * step) + (((r1._12 - r0._12) * remainder) >> FX32_SHIFT)) >> (step_shift * ROT_FILTER_SHIFT);

#ifdef G3D_NORMALIZE_ROT_MTX
        VEC_Normalize( (VecFx32*)(&pRot->_00), (VecFx32*)(&pRot->_00) );
        VEC_Normalize( (VecFx32*)(&pRot->_10), (VecFx32*)(&pRot->_10) );
#endif

        if (!doCross)
        {
            // �����Ƃ�Rot3�`���̃f�[�^�������ꍇ��
            // �O�ς̌v�Z������邱�Ƃɂ���
            pRot->_20 = ((r0._20 * step) + (((r1._20 - r0._20) * remainder) >> FX32_SHIFT)) >> (step_shift * ROT_FILTER_SHIFT);
            pRot->_21 = ((r0._21 * step) + (((r1._21 - r0._21) * remainder) >> FX32_SHIFT)) >> (step_shift * ROT_FILTER_SHIFT);
            pRot->_22 = ((r0._22 * step) + (((r1._22 - r0._22) * remainder) >> FX32_SHIFT)) >> (step_shift * ROT_FILTER_SHIFT);

#ifdef G3D_NORMALIZE_ROT_MTX
            VEC_Normalize( (VecFx32*)(&pRot->_20), (VecFx32*)(&pRot->_20) );
#endif
        }
        else
        {
            vecCross_((const VecFx32*)&pRot->_00,
                      (const VecFx32*)&pRot->_10,
                      (VecFx32*)&pRot->_20);
        }
        return;
    }
}


/*---------------------------------------------------------------------------*
    getRotDataByIdx_

    �C���f�b�N�X����Rot3,Rot5�����ꂩ�̃f�[�^�����o���ApRot�Ɋi�[����B
 *---------------------------------------------------------------------------*/
static BOOL
getRotDataByIdx_(MtxFx33* pRot,
                 const void* pArrayRot3,
                 const void* pArrayRot5,
                 NNSG3dJntAnmRIdx info)
{
    if (info & NNS_G3D_JNTANM_RIDX_PIVOT)
    {
        const fx16* data;
        fx32 A, B;
        u32 idxPivot;

        pRot->_00 = pRot->_01 = pRot->_02 =
        pRot->_10 = pRot->_11 = pRot->_12 =
        pRot->_20 = pRot->_21 = pRot->_22 = 0;

        // data[0] NNSG3dJntAnmPivotInfo�̃t���O�f�[�^
        // data[1], data[2] �s��̔�0�v�f
        data = (const fx16*)pArrayRot3 + ((info & NNS_G3D_JNTANM_RIDX_IDXDATA_MASK) * 3);
        A = *(data + 1);
        B = *(data + 2);
        idxPivot = (u32)( (*data & NNS_G3D_JNTANM_PIVOTINFO_IDXPIVOT_MASK) >>
                                 NNS_G3D_JNTANM_PIVOT_INFO_IDXPIVOT_SHIFT );

        pRot->a[idxPivot] =
            (*data & NNS_G3D_JNTANM_PIVOTINFO_MINUS) ? -FX32_ONE : FX32_ONE;

        pRot->a[pivotUtil_[idxPivot][0]] = A;
        pRot->a[pivotUtil_[idxPivot][1]] = B;

        pRot->a[pivotUtil_[idxPivot][2]] =
            (*data & NNS_G3D_JNTANM_PIVOTINFO_SIGN_REVC) ? -B : B;
        pRot->a[pivotUtil_[idxPivot][3]] =
            (*data & NNS_G3D_JNTANM_PIVOTINFO_SIGN_REVD) ? -A : A;

        // 3�s�ڂ܂Ŗ��܂��Ă�
        return FALSE;
    }
    else
    {
        // Rot5����f�[�^�������Ă���
        const fx16* data = (fx16*)pArrayRot5 +
                           ((info & NNS_G3D_JNTANM_RIDX_IDXDATA_MASK) * 5);
        fx16 _12;

        _12 = (fx16)(data[4] & 7);
        pRot->_11 = (data[4] >> 3);

        _12 = (fx16)((_12 << 3) | (data[0] & 7));
        pRot->_00 = (data[0] >> 3);

        _12 = (fx16)((_12 << 3) | (data[1] & 7));
        pRot->_01 = (data[1] >> 3);

        _12 = (fx16)((_12 << 3) | (data[2] & 7));
        pRot->_02 = (data[2] >> 3);

        _12 = (fx16)((_12 << 3) | (data[3] & 7));
        pRot->_10 = (data[3] >> 3);

        // �����g�������悤�ɃV�t�g�����l��fx16�ɃL���X�g���Ă���
        // fx32�ɈÖكL���X�g����
        pRot->_12 = ((fx16)(_12 << 3) >> 3);

        // 3�s�ڂ͊O�ςŋ��߂Ȃ���΂Ȃ�Ȃ�
        return TRUE;
    }
}

#endif // NNS_G3D_NSBCA_DISABLE

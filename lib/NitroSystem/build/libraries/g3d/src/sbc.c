/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d
  File:     sbc.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.55 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/sbc.h>

#include <nnsys/g3d/kernel.h>
#include <nnsys/g3d/binres/res_struct.h>
#include <nnsys/g3d/glbstate.h>
#include <nnsys/g3d/gecom.h>
#include <nnsys/g3d/binres/res_struct_accessor.h>
#include <nnsys/g3d/util.h>


static void
G3dDrawInternal_Loop_(NNSG3dRS* pRS)
{
    //
    // Loop for SBC execution
    //
    do
    {
        pRS->flag &= ~NNS_G3D_RSFLAG_SKIP;
        NNS_G3D_ASSERTMSG(NNS_G3dFuncSbcTable[NNS_G3D_GET_SBCCMD(*pRS->c)],
                          "SBC command not found / disabled.");

        (*NNS_G3dFuncSbcTable[NNS_G3D_GET_SBCCMD(*pRS->c)])
                                (pRS, NNS_G3D_GET_SBCFLG(*pRS->c));
    }
    while(!(pRS->flag & NNS_G3D_RSFLAG_RETURN));
}


static void
G3dDrawInternal_(NNSG3dRS* pRS, NNSG3dRenderObj* pRenderObj)
{
    NNS_G3D_NULL_ASSERT(pRS);
    NNS_G3D_NULL_ASSERT(pRenderObj);
  
    //
    // Init
    //
    // NOTICE:
    // isScaleCacheOne�̍ŏ��̃r�b�g�͗��ĂĂ�������
    // �������Ȃ��ƁASi3d��classic scale off�̏ꍇ�ɂ��������Ȃ�
    // (���m�[�h�̐e�̃X�P�[����1�Ƃ��Čv�Z�����邽�߂Ƀr�b�g�𗧂Ă�)
    //
    MI_CpuClearFast(pRS, sizeof(*pRS));
    pRS->isScaleCacheOne[0] = 1; // �G���f�B�A���ˑ�

    //
    // NNS_G3D_RSFLAG_NODE_VISIBLE�̓Z�b�g���Ă����̂�����
    // ���[�U�[��`��SBC�ł����Ȃ�MAT�R�}���h�����s�����悤��
    // �ꍇ�ɔ�����B
    //
    pRS->flag = NNS_G3D_RSFLAG_NODE_VISIBLE;

    //
    // ���߂���SBC�̑I��
    // pRenderObj->ptrUserSbc�Ƀ|�C���^��ݒ肷�邱�Ƃ�
    // ���߂���SBC��ύX���邱�Ƃ��ł���B
    //
    if (pRenderObj->ptrUserSbc)
    {
        pRS->c = pRenderObj->ptrUserSbc;
    }
    else
    {
        pRS->c = (u8*)pRenderObj->resMdl + pRenderObj->resMdl->ofsSbc;
    }

    //
    // pRenderObj�̐ݒ�
    //
    pRS->pRenderObj   = pRenderObj;

    //
    // pRenderObj->resMdl�̒��ŌJ��Ԃ��g�������L���b�V�����Ă���
    //
    pRS->pResNodeInfo = NNS_G3dGetNodeInfo(pRenderObj->resMdl);
    pRS->pResMat      = NNS_G3dGetMat(pRenderObj->resMdl);
    pRS->pResShp      = NNS_G3dGetShp(pRenderObj->resMdl);
    pRS->funcJntScale = NNS_G3dGetJointScale_FuncArray[pRenderObj->resMdl->info.scalingRule];
    pRS->funcJntMtx   = NNS_G3dSendJointSRT_FuncArray[pRenderObj->resMdl->info.scalingRule];
    pRS->funcTexMtx   = NNS_G3dSendTexSRT_FuncArray[pRenderObj->resMdl->info.texMtxMode];
    pRS->posScale     = pRenderObj->resMdl->info.posScale;
    pRS->invPosScale  = pRenderObj->resMdl->info.invPosScale;

    //
    // �R�[���o�b�N�̐ݒ�
    //
    if (pRenderObj->cbFunc && pRenderObj->cbCmd < NNS_G3D_SBC_COMMAND_NUM)
    {
        NNS_G3D_SBC_CALLBACK_TIMING_ASSERT(pRenderObj->cbTiming);
        pRS->cbVecFunc[pRenderObj->cbCmd] = pRenderObj->cbFunc;
        pRS->cbVecTiming[pRenderObj->cbCmd] = pRenderObj->cbTiming;
    }

    if (pRenderObj->flag & NNS_G3D_RENDEROBJ_FLAG_RECORD)
    {
        pRS->flag |= NNS_G3D_RSFLAG_OPT_RECORD;
    }

    if (pRenderObj->flag & NNS_G3D_RENDEROBJ_FLAG_NOGECMD)
    {
        pRS->flag |= NNS_G3D_RSFLAG_OPT_NOGECMD;
    }

    if (pRenderObj->flag & NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_DRAW)
    {
        pRS->flag |= NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW;
    }

    if (pRenderObj->flag & NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_MTXCALC)
    {
        pRS->flag |= NNS_G3D_RSFLAG_OPT_SKIP_SBCMTXCALC;
    }

    if (pRenderObj->cbInitFunc)
    {
        (*pRenderObj->cbInitFunc)(pRS);
    }

    G3dDrawInternal_Loop_(pRS);

    //
    // �N���[���A�b�v
    //
    // Asserts for notifying 'DO NOT MODIFY THEM'
    NNS_G3D_ASSERT(pRS->pRenderObj == pRenderObj);
    NNS_G3D_ASSERT(pRS == NNS_G3dRS);

    // �g���₷�����邽�߁A���R�[�h�����ꍇ�A�����I�Ƀ��R�[�h�t���O�����Z�b�g����B
    // ���ɌĂԂƂ��͍Đ����[�h�ɂȂ��Ă���B
    pRenderObj->flag &= ~NNS_G3D_RENDEROBJ_FLAG_RECORD; 
}


/*---------------------------------------------------------------------------*
    updateHintVec_

    pAnmObj�œo�^����Ă��郊�\�[�XID�ɑΉ�����r�b�g��ON�ɂ��Ă���
 *---------------------------------------------------------------------------*/
static void
updateHintVec_(u32* pVec, const NNSG3dAnmObj* pAnmObj)
{
    const NNSG3dAnmObj* p = pAnmObj;
    while(p)
    {
        int i;
        for (i = 0; i < p->numMapData; ++i)
        {
            if (p->mapData[i] & NNS_G3D_ANMOBJ_MAPDATA_EXIST)
            {
                pVec[i >> 5] |= 1 << (i & 31);
            }
        }
        p = p->next;
    }
}



/*---------------------------------------------------------------------------*
    NNS_G3dDraw

    ���f����`�悷��B�A�j���[�V�������̐ݒ�͎��O��pRenderObj�𑀍삷�邱�Ƃ�
    ����čs���B
 *---------------------------------------------------------------------------*/
void
NNS_G3dDraw(NNSG3dRenderObj* pRenderObj)
{
    // NOTICE
    // NOT REENTRANT
    NNS_G3D_NULL_ASSERT(pRenderObj);
    NNS_G3D_NULL_ASSERT(pRenderObj->resMdl);
    NNS_G3D_ASSERTMSG(pRenderObj->resMdl->info.numMat <= NNS_G3D_SIZE_MAT_MAX,
                      "numMaterial exceeds NNS_G3D_SIZE_MAT_MAX");
    NNS_G3D_ASSERTMSG(pRenderObj->resMdl->info.numNode <= NNS_G3D_SIZE_JNT_MAX,
                      "numNode exceeds NNS_G3D_SIZE_JNT_MAX");
    NNS_G3D_ASSERTMSG(pRenderObj->resMdl->info.numShp <= NNS_G3D_SIZE_SHP_MAX,
                      "numShape exceeds NNS_G3D_SIZE_SHP_MAX");

    if (NNS_G3dRenderObjTestFlag(pRenderObj, NNS_G3D_RENDEROBJ_FLAG_HINT_OBSOLETE))
    {
        // hint�p�r�b�g�x�N�g�����������Ȃ��̂ŃA�b�v�f�[�g����
        MI_CpuClear32(&pRenderObj->hintMatAnmExist[0], sizeof(u32) * (NNS_G3D_SIZE_MAT_MAX / 32));
        MI_CpuClear32(&pRenderObj->hintJntAnmExist[0], sizeof(u32) * (NNS_G3D_SIZE_JNT_MAX / 32));
        MI_CpuClear32(&pRenderObj->hintVisAnmExist[0], sizeof(u32) * (NNS_G3D_SIZE_JNT_MAX / 32));

        if (pRenderObj->anmMat)
            updateHintVec_(&pRenderObj->hintMatAnmExist[0], pRenderObj->anmMat);
        if (pRenderObj->anmJnt)
            updateHintVec_(&pRenderObj->hintJntAnmExist[0], pRenderObj->anmJnt);
        if (pRenderObj->anmVis)
            updateHintVec_(&pRenderObj->hintVisAnmExist[0], pRenderObj->anmVis);

        NNS_G3dRenderObjResetFlag(pRenderObj, NNS_G3D_RENDEROBJ_FLAG_HINT_OBSOLETE);
    }

    if (NNS_G3dRS)
    {
        G3dDrawInternal_(NNS_G3dRS, pRenderObj);
    }
    else
    {
        // SBC�����^�C���\���̗̈悪�m�ۂ���Ă��Ȃ��ꍇ�̓��[�J���X�^�b�N�ɂƂ�
        NNSG3dRS rs;
        NNS_G3dRS = &rs;
        G3dDrawInternal_(&rs, pRenderObj);
        NNS_G3dRS = NULL;
    }
}



//
// �R�[���o�b�N����R�[�h�̃R�[�f�B���O�ւ��钍�ӊ��N
// NOTICE:
// �R�[���o�b�N�֐������ŃR�[���o�b�N���������[�U�[�ɂ���ď����������邱�Ƃ�
// ���邱�Ƃɒ��ӂ��Ă��������B�܂�A�R�[���o�b�N�����邩�ǂ����̔����
// auto�ϐ��Ɋi�[���āA���̃R�[���o�b�N����ɗ��p���Ă͂����܂���B
//

/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_NOP

    mnemonic:   NNS_G3D_SBC_NOP
    operands:   none
    callbacks:  A/B/C

    �������
    �������Ȃ��B

    �t�L
    �R�[���o�b�N�͗L���ł��邪�A�^�C�~���O�̎w��͖��Ӗ��ł���B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_NOP(NNSG3dRS* rs, u32)
{
    NNS_G3D_NULL_ASSERT(rs);

#if !defined(NNS_G3D_SBC_CALLBACK_TIMING_A_DISABLE) || \
    !defined(NNS_G3D_SBC_CALLBACK_TIMING_B_DISABLE) || \
    !defined(NNS_G3D_SBC_CALLBACK_TIMING_C_DISABLE)
    
    if (rs->cbVecFunc[NNS_G3D_SBC_NOP])
    {
        (*rs->cbVecFunc[NNS_G3D_SBC_NOP])(rs);
    }
#endif

    rs->c++;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_RET

    mnemonic:   NNS_G3D_SBC_RET
    operands:   none
    callbacks:  A/B/C

    �������
    NNS_G3D_FUNCSBC_STATUS_RETURN��Ԃ��āASbc�̎��s���I����.

    �t�L
    SBC�̍Ō�ɂ���R�}���h�B
    �R�[���o�b�N�͗L���ł��邪�A�^�C�~���O�̎w��͖��Ӗ��ł���B
    �Ȃ��A���̖��߂�NNS_G3dRS->c���C���N�������g���Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_RET(NNSG3dRS* rs, u32)
{
    NNS_G3D_NULL_ASSERT(rs);

#if !defined(NNS_G3D_SBC_CALLBACK_TIMING_A_DISABLE) || \
    !defined(NNS_G3D_SBC_CALLBACK_TIMING_B_DISABLE) || \
    !defined(NNS_G3D_SBC_CALLBACK_TIMING_C_DISABLE)

    if (rs->cbVecFunc[NNS_G3D_SBC_RET])
    {
        (*rs->cbVecFunc[NNS_G3D_SBC_RET])(rs);
    }
#endif

    rs->flag |= NNS_G3D_RSFLAG_RETURN;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_NODE

    mnemonic:   NNS_G3D_SBC_NODE
    operands:   nodeID
                flags(1byte)
                    visible if 0x01 is on
    callbacks:  A: before
                B: none
                C: after
    
    �������
    �E TIMING_A�̃R�[���o�b�N���Ă�
    �E �r�W�r���e�B�A�j���[�V�����̎��s�O�`�F�b�N���s��
       �r�W�r���e�B�A�j���[�V���������݂���ꍇ�F
           �r�W�r���e�B�A�j���[�V�����u�����h�֐��̌���(�Ԃ�l�ł͂Ȃ�)�ɏ]����
           NNS_G3D_RSFLAG_NODE_VISIBLE�r�b�g���Z�b�g
       �Ȃ��ꍇ�F
           flags���Q�Ƃ��āANNS_G3D_RSFLAG_NODE_VISIBLE�r�b�g���Z�b�g
    �E TIMING_B�̃R�[���o�b�N���Ă�
    �E rs->c�����Z

    �t�L
    NNS_G3D_RSFLAG_NODE_VISIBLE�r�b�g�́A���̊֐����ł̂ݕύX�����B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_NODE(NNSG3dRS* rs, u32)
{
    NNS_G3D_NULL_ASSERT(rs);

    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW))
    {
        BOOL                    cbFlag;
        NNSG3dSbcCallBackTiming cbTiming;
        u32                     curNode;

        curNode = rs->currentNode = *(rs->c + 1);
        rs->flag |= NNS_G3D_RSFLAG_CURRENT_NODE_VALID;
        rs->pVisAnmResult = &rs->tmpVisAnmResult;

        // �R�[���o�b�N�`
        // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
        // rs->pVisAnmResult�𑀍삷�邱�Ƃ�
        // �r�W�r���e�B�v�Z�������邱�Ƃ��ł���
        cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_NODE, &cbTiming);

        if (!cbFlag)
        {
            NNS_G3D_NULL_ASSERT(rs->pRenderObj->funcBlendVis);

            if (rs->pRenderObj->anmVis                                            &&
                NNSi_G3dBitVecCheck(&rs->pRenderObj->hintVisAnmExist[0], curNode) &&
                (*rs->pRenderObj->funcBlendVis)(rs->pVisAnmResult, rs->pRenderObj->anmVis, curNode))
            {
                // AnmObj�̃��X�g���ݒ肳��Ă��āA
                // �r�W�r���e�B�A�j���[�V�����Ɋւ���q���g�x�N�g���̃r�b�g��ON�ɂȂ��Ă���
                // �r�W�r���e�B�A�j���[�V�����u�����_���Ŏ��ۂɊY������r�W�r���e�B�A�j���[�V�������������ꍇ
                ;
            }
            else
            {
                // �r�W�r���e�B�A�j���[�V�������Ȃ��ꍇ�A
                // �I�y�����h�̃f�[�^���画�f����B
                rs->pVisAnmResult->isVisible = *(rs->c + 2) & 1;
            }
        }
        
        // �R�[���o�b�N�a
        // �v�Z���ꂽ�r�W�r���e�B��rs->pVisAnmResult�𑀍삷�邱�Ƃ�
        // ���ς��邱�Ƃ��ł���B
        cbFlag = NNSi_G3dCallBackCheck_B(rs, NNS_G3D_SBC_NODE, &cbTiming);

        if (!cbFlag)
        {
            if (rs->pVisAnmResult->isVisible)
            {
                rs->flag |= NNS_G3D_RSFLAG_NODE_VISIBLE;
            }
            else
            {
                rs->flag &= ~NNS_G3D_RSFLAG_NODE_VISIBLE;
            }
        }

        // �R�[���o�b�N�b
        // ���̖��߂̑O�ɉ��炩�̏�����}�����邱�Ƃ��ł���B
        (void)NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_NODE, cbTiming);
    }

    rs->c += 3;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_MTX

    mnemonic:   NNS_G3D_SBC_MTX
    operands:   idxMtx
    callbacks:  A: before
                B: none
                C: after

    �������
    �E NNS_G3D_RSFLAG_NODE_VISIBLE��OFF�Ȃ�΂��̖��ߑS�̂��X�L�b�v
    �E TIMING_A�̃R�[���o�b�N���Ă�
    �E �s��X�^�b�N����J�����g�s��Ƀ��[�h
    �E TIMING_B�̃R�[���o�b�N���Ă�
    �E rs->c�����Z

    �t�L
    NNS_G3D_RSFLAG_NODE_VISIBLE��OFF�̏ꍇ�̓R�[���o�b�N���܂߂ăX�L�b�v�����B
    �J�n�O�ɁAPOSITION_VECTOR���[�h�łȂ��Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_MTX(NNSG3dRS* rs, u32)
{
    NNS_G3D_NULL_ASSERT(rs);

    // MTX�R�}���h��Draw�J�e�S���ɓ���
    // MAT,SHP�̑O�ɐ��������R�}���h������
    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW) &&
        (rs->flag & NNS_G3D_RSFLAG_NODE_VISIBLE))
    {
        BOOL                    cbFlag;
        NNSG3dSbcCallBackTiming cbTiming;

        // �R�[���o�b�N�`
        // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
        // �s��̃��X�g�A�����u�������邱�Ƃ��ł���B
        cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_MTX, &cbTiming);

        if (!cbFlag)
        {
            u32 arg = *(rs->c + 1);
            NNS_G3D_ASSERT(arg < 31);

            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
            {
                NNS_G3dGeBufferOP_N(G3OP_MTX_RESTORE,
                                    &arg,
                                    G3OP_MTX_RESTORE_NPARAMS);
            }
        }

        // �R�[���o�b�N�b
        // ���̖��߂̑O�ɉ��炩�̏�����}�����邱�Ƃ��ł���B
        (void)NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_MTX, cbTiming);
    }

    rs->c += 2;
}


void
NNSi_G3dFuncSbc_MAT_InternalDefault(NNSG3dRS* rs,
                                    u32 opt,
                                    const NNSG3dResMatData* mat,
                                    u32 idxMat)
{
    static const u32 matColorMask_[8] =
    {
        0x00000000,
        0x00007fff,
        0x7fff0000,
        0x7fff7fff,
        0x00008000,
        0x0000ffff,
        0x7fff8000,
        0x7fffffff
    };
    BOOL cbFlag;
    NNSG3dSbcCallBackTiming cbTiming;

    NNS_G3D_NULL_ASSERT(rs);
    NNS_G3D_NULL_ASSERT(mat);

    rs->currentMat = (u8)idxMat;
    rs->flag |= NNS_G3D_RSFLAG_CURRENT_MAT_VALID;
    // �R�[���o�b�N���Ă΂��ꍇ�Ɏg�p�����̈���w�肵�Ă����Ȃ��Ă͂Ȃ�Ȃ� 
    rs->pMatAnmResult = &rs->tmpMatAnmResult;

    // �R�[���o�b�N�`
    // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
    // �}�e���A���ݒ�R�[�h��u�������邱�Ƃ��ł���B
    cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_MAT, &cbTiming);

    if (!cbFlag)
    {
        NNSG3dMatAnmResult* pResult;

        //
        // �v�Z���ʂ̃o�b�t�@���g�����ǂ��������肷��B
        //
        if (rs->pRenderObj->recMatAnm &&
            !(rs->flag & NNS_G3D_RSFLAG_OPT_RECORD))
        {
            // �o�b�t�@���g�p�ł���ꍇ
            pResult = (rs->pRenderObj->recMatAnm + idxMat);
        }
        else
        {
            //
            // ����if�u���b�N�ł́A�}�e���A���f�[�^�̎擾��v�Z���s���A
            // pResult�ɓK�؂ȃf�[�^��ݒ肷��B
            //
            if ((opt == NNS_G3D_SBCFLG_001 || opt == NNS_G3D_SBCFLG_010) &&
                NNSi_G3dBitVecCheck(&rs->isMatCached[0], idxMat))
            {
                // MAT�L���b�V���ς݃t���O�������Ă���ꍇ
                if (rs->pRenderObj->recMatAnm)
                {
                    // �ȑO�̌v�Z���ʂ��o�b�t�@�ɋL�^����Ă���̂ł���������o��
                    pResult = (rs->pRenderObj->recMatAnm + idxMat);
                }
                else
                {
                    // �ȑO�̌v�Z���ʂ��L���b�V���ɋL�^����Ă���̂ł���������o��
                    pResult = &NNS_G3dRSOnGlb.matCache[idxMat];
                }
            }
            else
            {
                if (rs->pRenderObj->recMatAnm)
                {
                    // MAT�L���b�V���ς݃t���O�𗧂Ă�
                    NNSi_G3dBitVecSet(&rs->isMatCached[0], idxMat);

                    // �o�b�t�@������ꍇ�͖��߃I�v�V�����Ɋւ�炸�o�b�t�@�ɋL�^
                    pResult = (rs->pRenderObj->recMatAnm + idxMat);
                }
                else if (opt == NNS_G3D_SBCFLG_010)
                {
                    // MAT�L���b�V���ς݃t���O�𗧂Ă�
                    NNSi_G3dBitVecSet(&rs->isMatCached[0], idxMat);

                    // �������ݐ��MAT�L���b�V���ɐݒ�
                    pResult = &NNS_G3dRSOnGlb.matCache[idxMat];
                }
                else
                {
                    // NNSG3dRS��̗̈���g�p
                    pResult = &rs->tmpMatAnmResult;
                }

                //
                // ���f���f�[�^�̃}�e���A�������Z�b�g
                // G3dGlb�̃f�t�H���g�ƍ�������
                //
                {
                    pResult->flag           = (NNSG3dMatAnmResultFlag) 0;
                    if (NNS_G3dGetMatDataByIdx(rs->pResMat, idxMat)->flag & NNS_G3D_MATFLAG_WIREFRAME)
                    {
                        pResult->flag |= NNS_G3D_MATANM_RESULTFLAG_WIREFRAME;
                    }
                }
                {
                    // Diffuse & Ambient & VtxColor
                    u32 mask = matColorMask_[(mat->flag >> 6) & 7];
                    pResult->prmMatColor0 = (NNS_G3dGlb.prmMatColor0 & ~mask) |
                                            (mat->diffAmb & mask);
                }

                {
                    // Specular & Emission & Shininess
                    u32 mask = matColorMask_[(mat->flag >> 9) & 7];
                    pResult->prmMatColor1 = (NNS_G3dGlb.prmMatColor1 & ~mask) |
                                            (mat->specEmi & mask);
                }

                pResult->prmPolygonAttr = (NNS_G3dGlb.prmPolygonAttr & ~mat->polyAttrMask) |
                                          (mat->polyAttr & mat->polyAttrMask);

                pResult->prmTexImage = mat->texImageParam;
                pResult->prmTexPltt  = mat->texPlttBase;

                // ���f���Ƀe�N�X�`���s�񂪑��݂���ꍇ�́A
                // �e�N�X�`���s������f������Z�b�g
                if (mat->flag & NNS_G3D_MATFLAG_TEXMTX_USE)
                {
                    const u8* p = (const u8*)mat + sizeof(NNSG3dResMatData);

                    if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_SCALEONE))
                    {
                        const fx32* p_fx32 = (const fx32*)p;

                        pResult->scaleS = *(p_fx32 + 0);
                        pResult->scaleT = *(p_fx32 + 1);
                        p += 2 * sizeof(fx32);
                    }
                    else
                    {
                        pResult->flag |= NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SCALEONE;
                    }

                    if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_ROTZERO))
                    {
                        const fx16* p_fx16 = (const fx16*)p;

                        pResult->sinR = *(p_fx16 + 0);
                        pResult->cosR = *(p_fx16 + 1);
                        p += 2 * sizeof(fx16);
                    }
                    else
                    {
                        pResult->flag |= NNS_G3D_MATANM_RESULTFLAG_TEXMTX_ROTZERO;
                    }

                    if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_TRANSZERO))
                    {
                        const fx32* p_fx32 = (const fx32*)p;

                        pResult->transS = *(p_fx32 + 0);
                        pResult->transT = *(p_fx32 + 1);
                    }
                    else
                    {
                        pResult->flag |= NNS_G3D_MATANM_RESULTFLAG_TEXMTX_TRANSZERO;
                    }

                    pResult->flag |= NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SET;
                }

                NNS_G3D_NULL_ASSERT(rs->pRenderObj->funcBlendMat);

                // �}�e���A���A�j���[�V�����̃`�F�b�N
                if (rs->pRenderObj->anmMat &&
                    NNSi_G3dBitVecCheck(&rs->pRenderObj->hintMatAnmExist[0], idxMat))
                {
                    // �}�e���A���A�j���[�V�����̌v�Z�ɂ��ApResult��ύX���邩������Ȃ�
                    (void)(*rs->pRenderObj->funcBlendMat)(pResult, rs->pRenderObj->anmMat, idxMat);
                }

                //
                // ���X�e�N�X�`���s�񂪂���ꍇ��A�j���[�V�����ŕt������Ă���ꍇ��
                // �e�N�X�`���̕�����񓙂�ǉ�����B
                //
                if ( pResult->flag & (NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SET |
                                      NNS_G3D_MATANM_RESULTFLAG_TEXMTX_MULT))
                {
                    pResult->origWidth  = mat->origWidth;
                    pResult->origHeight = mat->origHeight;
                    pResult->magW = mat->magW;
                    pResult->magH = mat->magH;
                }
            }
        }
        rs->pMatAnmResult = pResult;
    }

    // �R�[���o�b�N�a
    // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
    // �}�e���A���̑��M�����u�������邱�Ƃ��ł���B
    cbFlag = NNSi_G3dCallBackCheck_B(rs, NNS_G3D_SBC_MAT, &cbTiming);

    if (!cbFlag)
    {
        NNSG3dMatAnmResult* pResult;

        NNS_G3D_NULL_ASSERT(rs->pMatAnmResult);
        pResult = rs->pMatAnmResult;

        // �����}�e���A�����ۂ��̃`�F�b�N
        if (pResult->prmPolygonAttr & REG_G3_POLYGON_ATTR_ALPHA_MASK)
        {
            // �����łȂ������C���[�t���[���\���̏ꍇ�́A
            // �W�I���g���R�}���h�̑��M
            
            if (pResult->flag & NNS_G3D_MATANM_RESULTFLAG_WIREFRAME)
            {
                // ���C���[�t���[���̏ꍇ�̓W�I���g���G���W���ɑ��郿��0�ɂȂ�
                pResult->prmPolygonAttr &= ~REG_G3_POLYGON_ATTR_ALPHA_MASK;
            }

            // �����t���O��OFF
            rs->flag &= ~NNS_G3D_RSFLAG_MAT_TRANSPARENT;

            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
            {
                // �X�^�b�N�Ƀv�b�V�����ē]��
                u32 cmd[7];

                cmd[0] = GX_PACK_OP(G3OP_DIF_AMB, G3OP_SPE_EMI, G3OP_POLYGON_ATTR, G3OP_NOP);
                cmd[1] = pResult->prmMatColor0;
                cmd[2] = pResult->prmMatColor1;
                cmd[3] = pResult->prmPolygonAttr;
                cmd[4] = GX_PACK_OP(G3OP_TEXIMAGE_PARAM, G3OP_TEXPLTT_BASE, G3OP_NOP, G3OP_NOP);
                cmd[5] = pResult->prmTexImage;
                cmd[6] = pResult->prmTexPltt;

                NNS_G3dGeBufferData_N(&cmd[0], 7);

                if (pResult->flag & (NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SET |
                                     NNS_G3D_MATANM_RESULTFLAG_TEXMTX_MULT))
                {

                    // �e�N�X�`���s��̑��M
                    NNS_G3D_NULL_ASSERT(rs->funcTexMtx);
                    (*rs->funcTexMtx)(pResult);
                }
            }
        }
        else
        {
            // �����t���O���n�m
            rs->flag |= NNS_G3D_RSFLAG_MAT_TRANSPARENT;
        }
    }

    // �R�[���o�b�N�b
    // ���̖��߂̑O�ɉ��炩�̏�����}�����邱�Ƃ��ł���B
    (void) NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_MAT, cbTiming);
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_MAT

    mnemonic:   NNS_G3D_SBC_MAT([000], [001], [010])
    operands:   idxMat
    callbacks:  A: before
                B: during
                C: after

    �������
    �E NNS_G3D_RSFLAG_NODE_VISIBLE��OFF�Ȃ�΂��̖��ߑS�̂��X�L�b�v
    �E TIMING_A�̃R�[���o�b�N���Ă�
    �E �}�e���A�����̎擾/�v�Z���s��
       opt�ɂ����MAT�L���b�V���֘A�̓��삪�قȂ�
       rs->isMatCached��idxMat�Ԗڂ̃r�b�g�������Ă���΁AMAT�L���b�V����
       �}�e���A����񂪃L���b�V������Ă��āA���p�ł���B

       [000] MAT�L���b�V���͈�؎Q�Ƃ��Ȃ�
       [001] MAT�L���b�V���ւ̓ǂݏ������s��
       [010] MAT�L���b�V����ǂ�

       NNS_G3D_RSFLAG_MAT_TRANSPARENT�̐ݒ���s��
       �Z�b�g���ꂽ�Ȃ�Ars->c�̉��Z�܂ŏ������X�L�b�v����B
    �E TIMING_B�̃R�[���o�b�N���Ă�
    �E �}�e���A�������W�I���g���G���W���֑��M
    �E TIMING_C�̃R�[���o�b�N���Ă�
    �E rs->c�����Z

    �t�L
    [000],[001],[010]���ǂ��g�������邩�̓R���o�[�g���Ɍ��肳���B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_MAT(NNSG3dRS* rs, u32 opt)
{
    NNS_G3D_NULL_ASSERT(rs);

    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW))
    {
        u32 idxMat;
        idxMat = *(rs->c + 1);

        // VISIBLE�łȂ��ꍇ�����݊��ɓ���MatID��Ԃł���ꍇ�͖��߂��X�L�b�v
        if ((rs->flag & NNS_G3D_RSFLAG_NODE_VISIBLE) ||
            !((rs->flag & NNS_G3D_RSFLAG_CURRENT_MAT_VALID) &&
            (idxMat == rs->currentMat)))
        {
            const NNSG3dResMatData* mat =
                NNS_G3dGetMatDataByIdx(rs->pResMat, idxMat);

            NNS_G3D_NULL_ASSERT(NNS_G3dFuncSbcMatTable[mat->itemTag]);
            (*NNS_G3dFuncSbcMatTable[mat->itemTag])(rs, opt, mat, idxMat);
        }
    }
    rs->c += 2;
}


void
NNSi_G3dFuncSbc_SHP_InternalDefault(NNSG3dRS* rs,
                                    u32,
                                    const NNSG3dResShpData* shp,
                                    u32)
{
    BOOL cbFlag;
    NNSG3dSbcCallBackTiming cbTiming;
    NNS_G3D_NULL_ASSERT(rs);
    NNS_G3D_NULL_ASSERT(shp);

    // �R�[���o�b�N�`
    // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
    // �f�B�X�v���C���X�g�̑��M�����u�������邱�Ƃ��ł���
    cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_SHP, &cbTiming);

    if (!cbFlag)
    {
        if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
        {
            NNS_G3dGeSendDL(NNS_G3dGetShpDLPtr(shp), NNS_G3dGetShpDLSize(shp));
        }
    }

    // �R�[���o�b�N�a
    (void) NNSi_G3dCallBackCheck_B(rs, NNS_G3D_SBC_SHP, &cbTiming);

    // �X�e�[�g�̕��A
    // �����I�ɏ�ԏ������̏��������邩������Ȃ�

    // �R�[���o�b�N�b
    // ���̖��߂̑O�ɉ��炩�̏�����}�����邱�Ƃ��ł���B
    (void) NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_SHP, cbTiming);
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_SHP

    mnemonic:   NNS_G3D_SBC_SHP
    operands:   idxShp
    callbacks:  A: before
                B: during
                C: after

    �������
    �E NNS_G3D_RSFLAG_NODE_VISIBLE��OFF�A����NNS_G3D_RSFLAG_MAT_TRANSPARENT��ON
       �Ȃ�΂��̖��ߑS�̂��X�L�b�v
    �E TIMING_A�̃R�[���o�b�N���Ă�
    �E �f�B�X�v���C���X�g���R�[��
    �E TIMING_B�̃R�[���o�b�N���Ă�
    �E �f�B�X�v���C���X�g���ύX�����X�e�[�g�𕜋A����
    �E TIMING_C�̃R�[���o�b�N���Ă�
    �E rs->c�̉��Z

    �t�L
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_SHP(NNSG3dRS* rs, u32 opt)
{
    NNS_G3D_NULL_ASSERT(rs);

    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW))
    {
        if ((rs->flag & NNS_G3D_RSFLAG_NODE_VISIBLE) &&
            !(rs->flag & NNS_G3D_RSFLAG_MAT_TRANSPARENT))
        {
            u32 idxShp = *(rs->c + 1);
            const NNSG3dResShpData* shp =
                NNS_G3dGetShpDataByIdx(rs->pResShp, idxShp);

            NNS_G3D_NULL_ASSERT(NNS_G3dFuncSbcShpTable[shp->itemTag]);
            (*NNS_G3dFuncSbcShpTable[shp->itemTag])(rs, opt, shp, idxShp);
        }
    }
    rs->c += 2;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_NODEDESC([000], [001], [010], [011])

    mnemonic:   NNS_G3D_SBC_NODEDESC
    operands:   [000]: idxNode, idxNodeParent, flags
                [001]: idxNode, idxNodeParent, flags, idxMtxDest
                [010]: idxNode, idxNodeParent, flags, idxMtxSrc
                [011]: idxNode, idxNodeParent, flags, idxMtxDest, idxMtxSrc
                0 <= idxMtxSrc, idxMtxDest <= 30
                x = 0 --> checks visibility anm and change currentNode
                x = 1 --> no
    callbacks:  A: before
                B: during
                C: after

    �������
    �E [010], [011]�Ȃ�΁AidxMtxSrc���J�����g�s��Ƀ��X�g�A����
    �E TIMING_A�̃R�[���o�b�N���Ă�
    �E �J�����g�s��ɑ΂��A���f��/�W���C���g�A�j���[�V�����̍s�����Z����
    �E TIMING_B�̃R�[���o�b�N���Ă�
    �E [001], [011]�Ȃ�΁AidxMtxDest�ɃJ�����g�s����X�g�A����
    �E TIMING_C�̃R�[���o�b�N���Ă�
    �E rs->c�̉��Z

    �t�L
    NNS_G3dFuncSbc_NODE�̂悤��NODE��Ԃ�ύX�����邱�Ƃ͂Ȃ�
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_NODEDESC(NNSG3dRS* rs, u32 opt)
{
    u32 cmdLen = 4;
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
    BOOL cbFlag;
    NNSG3dSbcCallBackTiming cbTiming;
    u32 idxNode = *(rs->c + 1);
    rs->currentNodeDesc = (u8)idxNode;
    rs->flag |= NNS_G3D_RSFLAG_CURRENT_NODEDESC_VALID;

    NNS_G3D_NULL_ASSERT(rs);

    if (rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCMTXCALC)
    {
        if (opt == NNS_G3D_SBCFLG_010 ||
            opt == NNS_G3D_SBCFLG_011)
        {
            ++cmdLen;
        }


        if (opt == NNS_G3D_SBCFLG_001 ||
            opt == NNS_G3D_SBCFLG_011)
        {
            ++cmdLen;
#if 1
            // ��ɂ��̂܂܍s����g���\��������̂ŁA
            // �X�g�A��������X�g�A�Œu�������Ă����K�v������
            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
            {
                u32 idxMtxDest = *(rs->c + 4);
                NNS_G3D_ASSERT(idxMtxDest < 31);

                NNS_G3dGeBufferOP_N(G3OP_MTX_RESTORE,
                                    &idxMtxDest,
                                    G3OP_MTX_RESTORE_NPARAMS);
            }
#endif
        }
        rs->c += cmdLen;
        return;
    }

    {
        if (opt == NNS_G3D_SBCFLG_010 ||
            opt == NNS_G3D_SBCFLG_011)
        {
            u32 idxMtxSrc;

            ++cmdLen;
            if (opt == NNS_G3D_SBCFLG_010)
            {
                idxMtxSrc = *(rs->c + 4);
            }
            else
            {
                idxMtxSrc = *(rs->c + 5);
            }
            NNS_G3D_ASSERT(idxMtxSrc < 31);

            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
            {
                NNS_G3dGeBufferOP_N(G3OP_MTX_RESTORE,
                                    &idxMtxSrc,
                                    G3OP_MTX_RESTORE_NPARAMS);
            }
        }
    }

    // �R�[���o�b�N���Ă΂��ꍇ�Ɏg�p�����̈���w�肵�Ă����Ȃ��Ă͂Ȃ�Ȃ� 
    rs->pJntAnmResult = &rs->tmpJntAnmResult;

    // �R�[���o�b�N�`
    // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
    // �W���C���gSRT�f�[�^�̎擾�����u�������邱�Ƃ��ł���B
    cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_NODEDESC, &cbTiming);

    if (!cbFlag)
    {
        NNSG3dJntAnmResult* pAnmResult;
        BOOL                isUseRecordData;
//        u32 idxNodeParent = *(rs->c + 2);
//        u32 flags         = *(rs->c + 3);

        //
        // �v�Z���ʂ̃o�b�t�@���g�����ǂ��������肷��B
        //
        if (rs->pRenderObj->recJntAnm)
        {
            // �O���o�b�t�@���|�C���g����
            pAnmResult = (rs->pRenderObj->recJntAnm + idxNode);
            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_RECORD))
            {
                // �O���o�b�t�@�̃f�[�^���Đ�����B
                isUseRecordData = TRUE;
            }
            else
            {
                // �O���o�b�t�@�Ƀf�[�^���L�^����B
                isUseRecordData = FALSE;
            }
        }
        else
        {
            // �֐������̃o�b�t�@���e���|�����Ŏg�p
            isUseRecordData = FALSE;
            pAnmResult = &rs->tmpJntAnmResult;
        }

        if (!isUseRecordData)
        {
            // �v�Z�̑O��NNSG3dJntAnmResult::flag�����͂��炩���߃N���A���Ă����K�v������B
            pAnmResult->flag = (NNSG3dJntAnmResultFlag) 0;

            NNS_G3D_NULL_ASSERT(rs->pRenderObj->funcBlendJnt);
            if (rs->pRenderObj->anmJnt &&
                (*rs->pRenderObj->funcBlendJnt)(pAnmResult, rs->pRenderObj->anmJnt, idxNode))
            {
                //
                // �W���C���g�A�j���[�V�����̃��\�[�X���g�p����anmResult�������Ă���B
                //
                ;
            }
            else
            {
                //
                // �Î~���f���̃��\�[�X���g�p����anmResult�𓾂�
                //
                const NNSG3dResNodeData* pNd =
                    NNS_G3dGetNodeDataByIdx(rs->pResNodeInfo, idxNode);
                const u8* p = (const u8*)pNd + sizeof(NNSG3dResNodeData);

                //
                // Translation
                //
                if (pNd->flag & NNS_G3D_SRTFLAG_TRANS_ZERO)
                {
                    pAnmResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_TRANS_ZERO;
                }
                else
                {
                    const fx32* p_fx32 = (const fx32*)p;

                    pAnmResult->trans.x = *(p_fx32 + 0);
                    pAnmResult->trans.y = *(p_fx32 + 1);
                    pAnmResult->trans.z = *(p_fx32 + 2);
                    p += 3 * sizeof(fx32);
                }

                //
                // Rotation
                //
                if (pNd->flag & NNS_G3D_SRTFLAG_ROT_ZERO)
                {
                    pAnmResult->flag |= NNS_G3D_JNTANM_RESULTFLAG_ROT_ZERO;
                }
                else
                {
                    if (pNd->flag & NNS_G3D_SRTFLAG_PIVOT_EXIST)
                    {
                        //
                        // ���k����Ă���ꍇ(��Ɉꎲ��])��
                        // ���k���ꂽ�s������ɖ߂�
                        //
                        fx32 A = *(fx16*)(p + 0);
                        fx32 B = *(fx16*)(p + 2);
                        u32 idxPivot = (u32)( (pNd->flag & NNS_G3D_SRTFLAG_IDXPIVOT_MASK) >> 
                                                        NNS_G3D_SRTFLAG_IDXPIVOT_SHIFT );
                        // anmResult.rot���N���A
                        MI_Zero36B(&pAnmResult->rot);
                        
                        pAnmResult->rot.a[idxPivot] =
                            (pNd->flag & NNS_G3D_SRTFLAG_PIVOT_MINUS) ?
                                -FX32_ONE :
                                FX32_ONE;
                        
                        pAnmResult->rot.a[pivotUtil_[idxPivot][0]] = A;
                        pAnmResult->rot.a[pivotUtil_[idxPivot][1]] = B;

                        pAnmResult->rot.a[pivotUtil_[idxPivot][2]] =
                            (pNd->flag & NNS_G3D_SRTFLAG_SIGN_REVC) ? -B : B;

                        pAnmResult->rot.a[pivotUtil_[idxPivot][3]] =
                            (pNd->flag & NNS_G3D_SRTFLAG_SIGN_REVD) ? -A : A;

                        p += 2 * sizeof(fx16);
                    }
                    else
                    {
                        // NOTICE:
                        // �������R�s�[API�ɒu�������Ȃ�����
                        // fx16����fx32�ւ̈Öق̃L���X�g���s���Ă��邩��

                        const fx16* pp = (const fx16*)p;
                        pAnmResult->rot.a[0] = pNd->_00;
                        pAnmResult->rot.a[1] = *(pp + 0);
                        pAnmResult->rot.a[2] = *(pp + 1);
                        pAnmResult->rot.a[3] = *(pp + 2);
                        pAnmResult->rot.a[4] = *(pp + 3);
                        pAnmResult->rot.a[5] = *(pp + 4);
                        pAnmResult->rot.a[6] = *(pp + 5);
                        pAnmResult->rot.a[7] = *(pp + 6);
                        pAnmResult->rot.a[8] = *(pp + 7);

                        p += 8 * sizeof(fx16);
                    }
                }

                //
                // Scale
                //

                //
                // NOTICE:
                // MayaSSC��Si3d��classic scale off�̏ꍇ�ɂ�
                // NNS_G3D_SRTFLAG_SCALE_ONE�̂Ƃ��ɂ��t���O�𗧂Ă�ȊO��
                // �v�Z������������B
                //
                NNS_G3D_NULL_ASSERT(rs->funcJntScale);
                (*rs->funcJntScale)(pAnmResult, (fx32*)p, rs->c, pNd->flag);
            }
        }
        rs->pJntAnmResult = pAnmResult;
    }

    // �R�[���o�b�N�a
    // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
    // �W�I���g���G���W���ւ̑��M�����u�������邱�Ƃ��ł���B
    cbFlag = NNSi_G3dCallBackCheck_B(rs, NNS_G3D_SBC_NODEDESC, &cbTiming);

    if (!cbFlag)
    {
        //
        // �W�I���g���G���W���ւ̑��M
        //
        if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
        {
            NNS_G3D_NULL_ASSERT(rs->pJntAnmResult);
            NNS_G3D_NULL_ASSERT(rs->funcJntMtx);

            (*rs->funcJntMtx)(rs->pJntAnmResult);
        }
    }

    rs->pJntAnmResult = NULL;

    // �R�[���o�b�N�b
    // ���̖��߂̑O�ɉ��炩�̏�����}�����邱�Ƃ��ł���B
    // �܂��ANNS_G3D_RSFLAG_SKIP���I���ɂ��邱�Ƃɂ����
    // �J�����g�s��̃X�g�A�����u�������邱�Ƃ��ł���
    cbFlag = NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_NODEDESC, cbTiming);

    if (opt == NNS_G3D_SBCFLG_001 ||
        opt == NNS_G3D_SBCFLG_011)
    {
        ++cmdLen;

        if (!cbFlag)
        {
            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
            {
                u32 idxMtxDest = *(rs->c + 4);
                NNS_G3D_ASSERT(idxMtxDest < 31);

                NNS_G3dGeBufferOP_N(G3OP_MTX_STORE,
                                    &idxMtxDest,
                                    G3OP_MTX_STORE_NPARAMS);
            }
        }
    }

    rs->c += cmdLen;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_BB([000], [001], [010], [011])

    mnemonic:   NNS_G3D_SBC_BB
    operands:   [000]: idxNode
                [001]: idxNode, idxMtxDest
                [010]: idxNode, idxMtxSrc
                [011]: idxNode, idxMtxDest, idxMtxSrc
                0 <= idxMtxSrc, idxMtxDest <= 30
    callbacks:  A: before
                B: during
                C: after

    �������
    �E [010], [011]�Ȃ�΁AidxMtxSrc���J�����g�s��Ƀ��X�g�A����
    �E TIMING_A�̃R�[���o�b�N���Ă�
    �E �ˉe�s����v�b�V�����A�P�ʍs����Z�b�g�B
    �E �W�I���g���G���W�����~�܂�܂ő҂��A�J�����g�s������o���B
    �E CPU�Ńr���{�[�h�s��ɂȂ�悤���H���A�J�����g�s��Ɋi�[
    �E TIMING_B�̃R�[���o�b�N���Ă�
    �E [001], [011]�Ȃ�΁AidxMtxDest�ɃJ�����g�s����X�g�A����
    �E TIMING_C�̃R�[���o�b�N���Ă�
    �E rs->c�̉��Z

    �t�L
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_BB(NNSG3dRS* rs, u32 opt)
{
    //
    // �u�J�����̎ˉe���ʁv�ɕ��s�ȃr���{�[�h�̕\��
    //
    u32 cmdLen = 2;

    static u32 bbcmd1[] =
    {
        GX_PACK_OP(G3OP_MTX_POP, G3OP_MTX_MODE, G3OP_MTX_LOAD_4x3, G3OP_MTX_SCALE),
        1,
        GX_MTXMODE_POSITION_VECTOR,
        FX32_ONE, 0, 0, 
        0, FX32_ONE, 0,
        0, 0, FX32_ONE,
        0, 0, 0,   // �����͉�(Trans)
        0, 0, 0    // �����͉�(Scale)
    };
    
    VecFx32* trans = (VecFx32*)&bbcmd1[12];
    VecFx32* scale = (VecFx32*)&bbcmd1[15];
    MtxFx44 m;
    BOOL cbFlag;
    NNSG3dSbcCallBackTiming cbTiming;

    NNS_G3D_NULL_ASSERT(rs);

    if (rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW)
    {
        // �r���{�[�h��Draw�J�e�S���ɓ���
        if (opt == NNS_G3D_SBCFLG_010 ||
            opt == NNS_G3D_SBCFLG_011)
        {
            ++cmdLen;
        }
        if (opt == NNS_G3D_SBCFLG_001 ||
            opt == NNS_G3D_SBCFLG_011)
        {
            ++cmdLen;
        }
        rs->c += cmdLen;
        return;
    }

    {
        if (opt == NNS_G3D_SBCFLG_010 ||
            opt == NNS_G3D_SBCFLG_011)
        {
            ++cmdLen;

            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
            {
                u32 idxMtxSrc;
                if (opt == NNS_G3D_SBCFLG_010)
                {
                    idxMtxSrc = *(rs->c + 2);
                }
                else
                {
                    idxMtxSrc = *(rs->c + 3);
                }
                NNS_G3D_ASSERT(idxMtxSrc < 31);

                NNS_G3dGeBufferOP_N(G3OP_MTX_RESTORE,
                                    &idxMtxSrc,
                                    1);
            }
        }
    }

    // �R�[���o�b�N�`
    // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
    // �r���{�[�h�s��̐ݒ蓮���u�������邱�Ƃ��ł���B
    cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_BB, &cbTiming);

    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD) &&
        (!cbFlag))
    {
        // �o�b�t�@�̃t���b�V��
        NNS_G3dGeFlushBuffer();

        //
        // ���̃R�[���o�b�N�|�C���g�܂ŃC�~�f�B�G�C�g���M�\.
        //

        // �R�}���h�]��:
        // PROJ���[�h�ɕύX
        // �ˉe�s���ޔ�
        // �P�ʍs����Z�b�g
        reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_MODE, G3OP_MTX_PUSH, G3OP_MTX_IDENTITY, G3OP_NOP);
        reg_G3X_GXFIFO = (u32)GX_MTXMODE_PROJECTION;
        reg_G3X_GXFIFO = 0; // 2004/08/26 geometry fifo glitch

        // �X�ɃW�I���g���G���W���̒�~��҂�
        // �J�����g�s��̎擾
        while (G3X_GetClipMtx(&m))
            ;

        if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_WVP)
        {
            // Projection�s��ɃJ�����s�񂪂������Ă���ꍇ�́A
            // ���߂�SRT�t�J�����s�����납�炩���Ȃ���΂Ȃ�Ȃ��B
            const MtxFx43* cam = NNS_G3dGlbGetSrtCameraMtx();
            MtxFx44 tmp;

            MTX_Copy43To44(cam, &tmp);
            MTX_Concat44(&m, &tmp, &m);
        }
        else if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_VP)
        {
            const MtxFx43* cam = NNS_G3dGlbGetCameraMtx();
            MtxFx44 tmp;

            MTX_Copy43To44(cam, &tmp);
            MTX_Concat44(&m, &tmp, &m);
        }

        // �r���{�[�h�s��̌v�Z
        trans->x = m._30;
        trans->y = m._31;
        trans->z = m._32;

        scale->x = VEC_Mag((VecFx32*)&m._00);
        scale->y = VEC_Mag((VecFx32*)&m._10);
        scale->z = VEC_Mag((VecFx32*)&m._20);

        if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_WVP)
        {
            // �ˉe�s���POP
            // POS_VEC�ɕ��A
            // SRT�t�J�����̋t�s����Z�b�g
            reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_POP, G3OP_MTX_MODE, G3OP_MTX_LOAD_4x3, G3OP_NOP);
            MI_CpuSend32(&bbcmd1[1],
                         &reg_G3X_GXFIFO,
                         2 * sizeof(u32));
            MI_CpuSend32(NNS_G3dGlbGetInvSrtCameraMtx(),
                         &reg_G3X_GXFIFO,
                         G3OP_MTX_LOAD_4x3_NPARAMS * sizeof(u32));

            // �J�����g�s��ɂ�����
            // �v�Z�����X�P�[����������
            reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_MULT_4x3, G3OP_MTX_SCALE, G3OP_NOP, G3OP_NOP);
            MI_CpuSend32(&bbcmd1[3],
                         &reg_G3X_GXFIFO,
                         sizeof(MtxFx43) + sizeof(VecFx32));
        }
        else if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_VP)
        {
            // �ˉe�s���POP
            // POS_VEC�ɕ��A
            // �J�����̋t�s����Z�b�g
            reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_POP, G3OP_MTX_MODE, G3OP_MTX_LOAD_4x3, G3OP_NOP);
            MI_CpuSend32(&bbcmd1[1],
                         &reg_G3X_GXFIFO,
                         2 * sizeof(u32));
            MI_CpuSend32(NNS_G3dGlbGetInvCameraMtx(),
                         &reg_G3X_GXFIFO,
                         G3OP_MTX_LOAD_4x3_NPARAMS * sizeof(u32));

            // �J�����g�s��ɂ�����
            // �v�Z�����X�P�[����������
            reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_MULT_4x3, G3OP_MTX_SCALE, G3OP_NOP, G3OP_NOP);
            MI_CpuSend32(&bbcmd1[3],
                         &reg_G3X_GXFIFO,
                         sizeof(MtxFx43) + sizeof(VecFx32));
        }
        else
        {
            // �ˉe�s���POP
            // POS_VEC�ɕ��A
            // �J�����g�s��Ɋi�[
            // �v�Z�����X�P�[����������
            MI_CpuSend32(&bbcmd1[0],
                         &reg_G3X_GXFIFO,
                         18 * sizeof(u32));
        }
    }

    // �R�[���o�b�N�b
    // ���̖��߂̑O�ɉ��炩�̏�����}�����邱�Ƃ��ł���B
    // �܂��ANNS_G3D_RSFLAG_SKIP���I���ɂ��邱�Ƃɂ����
    // �J�����g�s��̃X�g�A�����u�������邱�Ƃ��ł���
    cbFlag = NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_BB, cbTiming);

    if (opt == NNS_G3D_SBCFLG_001 ||
        opt == NNS_G3D_SBCFLG_011)
    {
        ++cmdLen;

        if (!cbFlag)
        {
            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
            {
                u32 idxMtxDest;
                idxMtxDest = *(rs->c + 2);

                NNS_G3D_ASSERT(idxMtxDest < 31);

                // �����ł̓C�~�f�B�G�C�g���M�\�ł͂Ȃ���������Ȃ�
                NNS_G3dGeBufferOP_N(G3OP_MTX_STORE,
                                    &idxMtxDest,
                                    G3OP_MTX_STORE_NPARAMS);
            }
        }
    }

    rs->c += cmdLen;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_BBY([000], [001], [010], [011])

    mnemonic:   NNS_G3D_SBC_BBY
    operands:   [000]: idxNode
                [001]: idxNode, idxMtxDest
                [010]: idxNode, idxMtxSrc
                [011]: idxNode, idxMtxDest, idxMtxSrc
                0 <= idxMtxSrc, idxMtxDest <= 30
    callbacks:  A: before
                B: during
                C: after

    �������
    �E [010], [011]�Ȃ�΁AidxMtxSrc���J�����g�s��Ƀ��X�g�A����
    �E TIMING_A�̃R�[���o�b�N���Ă�
    �E �ˉe�s����v�b�V�����A�P�ʍs����Z�b�g�B
    �E �W�I���g���G���W�����~�܂�܂ő҂��A�J�����g�s������o���B
    �E CPU��Y���r���{�[�h�s��ɂȂ�悤���H���A�J�����g�s��Ɋi�[
    �E TIMING_B�̃R�[���o�b�N���Ă�
    �E [001], [011]�Ȃ�΁AidxMtxDest�ɃJ�����g�s����X�g�A����
    �E TIMING_C�̃R�[���o�b�N���Ă�
    �E rs->c�̉��Z

    �t�L
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_BBY(NNSG3dRS* rs, u32 opt)
{
    u32 cmdLen = 2;
    MtxFx44 m;

    static u32 bbcmd1[] =
    {
        GX_PACK_OP(G3OP_MTX_POP, G3OP_MTX_MODE, G3OP_MTX_LOAD_4x3, G3OP_MTX_SCALE),
        1,
        GX_MTXMODE_POSITION_VECTOR,
        FX32_ONE, 0, 0, // �����͉�(4x3Mtx)
        0, FX32_ONE, 0,
        0, 0, FX32_ONE,
        0, 0, 0,   
        0, 0, 0    // �����͉�(Scale)
    };
    VecFx32* trans = (VecFx32*)&bbcmd1[12];
    VecFx32* scale = (VecFx32*)&bbcmd1[15];
    MtxFx43* mtx   = (MtxFx43*)&bbcmd1[3];
    BOOL cbFlag;
    NNSG3dSbcCallBackTiming cbTiming;

    NNS_G3D_NULL_ASSERT(rs);

    if (rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW)
    {
        // �r���{�[�h��Draw�J�e�S���ɓ���
        if (opt == NNS_G3D_SBCFLG_010 ||
            opt == NNS_G3D_SBCFLG_011)
        {
            ++cmdLen;
        }
        if (opt == NNS_G3D_SBCFLG_001 ||
            opt == NNS_G3D_SBCFLG_011)
        {
            ++cmdLen;
        }
        rs->c += cmdLen;
        return;
    }

    {
        if (opt == NNS_G3D_SBCFLG_010 ||
            opt == NNS_G3D_SBCFLG_011)
        {
            ++cmdLen;
            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
            {
                u32 idxMtxSrc;
                if (opt == NNS_G3D_SBCFLG_010)
                {
                    idxMtxSrc = *(rs->c + 2);
                }
                else
                {
                    idxMtxSrc = *(rs->c + 3);
                }
                NNS_G3D_ASSERT(idxMtxSrc < 31);
                NNS_G3dGeBufferOP_N(G3OP_MTX_RESTORE,
                                    &idxMtxSrc,
                                    1);
            }
        }
    }

    // �R�[���o�b�N�`
    // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
    // �r���{�[�h�s��̐ݒ蓮���u�������邱�Ƃ��ł���
    cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_BBY, &cbTiming);

    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD) &&
        (!cbFlag))
    {
        // �o�b�t�@�̃t���b�V��
        NNS_G3dGeFlushBuffer();

        //
        // ���̃R�[���o�b�N�|�C���g�܂ŃC�~�f�B�G�C�g���M�\.
        //

        // �R�}���h�]��:
        // PROJ���[�h�ɕύX
        // �ˉe�s���ޔ�
        // �P�ʍs����Z�b�g
        reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_MODE, G3OP_MTX_PUSH, G3OP_MTX_IDENTITY, G3OP_NOP);
        reg_G3X_GXFIFO = (u32)GX_MTXMODE_PROJECTION;
        reg_G3X_GXFIFO = 0; // 2004/08/26 geometry fifo glitch

        // �X�ɃW�I���g���G���W���̒�~��҂�
        // �J�����g�s��(�N���b�v�s��)�̎擾
        while (G3X_GetClipMtx(&m))
            ;

        if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_WVP)
        {
            // Projection�s��ɃJ�����s�񂪂������Ă���ꍇ�́A
            // ���߂�SRT�t�J�����s�����납�炩���Ȃ���΂Ȃ�Ȃ��B
            const MtxFx43* cam = NNS_G3dGlbGetSrtCameraMtx();
            MtxFx44 tmp;

            MTX_Copy43To44(cam, &tmp);
            MTX_Concat44(&m, &tmp, &m);
        }
        else if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_VP)
        {
            // Projection�s��ɃJ�����s�񂪂������Ă���ꍇ�́A
            // ���߂ăJ�����s�����납�炩���Ȃ���΂Ȃ�Ȃ��B
            const MtxFx43* cam = NNS_G3dGlbGetCameraMtx();
            MtxFx44 tmp;

            MTX_Copy43To44(cam, &tmp);
            MTX_Concat44(&m, &tmp, &m);
        }

        // �r���{�[�h�s��̌v�Z

        // 1: translation�͗��p
        trans->x = m._30;
        trans->y = m._31;
        trans->z = m._32;

        // 2: scale�͊e�s�̃x�N�g���̑傫���ŋߎ�
        scale->x = VEC_Mag((VecFx32*)&m._00);
        scale->y = VEC_Mag((VecFx32*)&m._10);
        scale->z = VEC_Mag((VecFx32*)&m._20);

        // 3: ��]�s��͌��ʂƂ���X���ɑ΂����]�s��ɂȂ�̂�
        //    ����炵���l��ݒ肷��
        if (m._11 != 0 || m._12 != 0)
        {
            VEC_Normalize((VecFx32*)&m._10, (VecFx32*)&mtx->_10);

            mtx->_21 = -mtx->_12;
            mtx->_22 = mtx->_11;
        }
        else
        {
            VEC_Normalize((VecFx32*)&m._20, (VecFx32*)&mtx->_20);
            
            mtx->_12 = -mtx->_21;
            mtx->_11 = mtx->_22;
        }

        if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_WVP)
        {
            // �ˉe�s���POP
            // POS_VEC�ɕ��A
            // SRT���J�����̋t�s����Z�b�g
            reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_POP, G3OP_MTX_MODE, G3OP_MTX_LOAD_4x3, G3OP_NOP);
            MI_CpuSend32(&bbcmd1[1],
                         &reg_G3X_GXFIFO,
                         2 * sizeof(u32));
            MI_CpuSend32(NNS_G3dGlbGetInvSrtCameraMtx(),
                         &reg_G3X_GXFIFO,
                         G3OP_MTX_LOAD_4x3_NPARAMS * sizeof(u32));
            
            // �J�����g�s��ɂ�����
            // �v�Z�����X�P�[����������        
            reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_MULT_4x3, G3OP_MTX_SCALE, G3OP_NOP, G3OP_NOP);
            MI_CpuSend32(&bbcmd1[3],
                         &reg_G3X_GXFIFO,
                         sizeof(MtxFx43) + sizeof(VecFx32));
        }
        else if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_VP)
        {
            // �ˉe�s���POP
            // POS_VEC�ɕ��A
            // �J�����̋t�s����Z�b�g
            reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_POP, G3OP_MTX_MODE, G3OP_MTX_LOAD_4x3, G3OP_NOP);
            MI_CpuSend32(&bbcmd1[1],
                         &reg_G3X_GXFIFO,
                         2 * sizeof(u32));
            MI_CpuSend32(NNS_G3dGlbGetInvCameraMtx(),
                         &reg_G3X_GXFIFO,
                         G3OP_MTX_LOAD_4x3_NPARAMS * sizeof(u32));
            
            // �J�����g�s��ɂ�����
            // �v�Z�����X�P�[����������        
            reg_G3X_GXFIFO = GX_PACK_OP(G3OP_MTX_MULT_4x3, G3OP_MTX_SCALE, G3OP_NOP, G3OP_NOP);
            MI_CpuSend32(&bbcmd1[3],
                         &reg_G3X_GXFIFO,
                         sizeof(MtxFx43) + sizeof(VecFx32));
        }
        else
        {
            // �ˉe�s���POP
            // POS_VEC�ɕ��A
            // �J�����g�s��Ɋi�[
            // �v�Z�����X�P�[����������
            MI_CpuSend32(&bbcmd1[0], &reg_G3X_GXFIFO, 18 * sizeof(u32));
        }
    }

    // �R�[���o�b�N�b
    // ���̖��߂̑O�ɉ��炩�̏�����}�����邱�Ƃ��ł���B
    // �܂��ANNS_G3D_RSFLAG_SKIP���I���ɂ��邱�Ƃɂ����
    // �J�����g�s��̃X�g�A�����u�������邱�Ƃ��ł���
    cbFlag = NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_BBY, cbTiming);

    if (opt == NNS_G3D_SBCFLG_001 ||
        opt == NNS_G3D_SBCFLG_011)
    {
        ++cmdLen;

        if (!cbFlag)
        {
            if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD))
            {
                u32 idxMtxDest;
                idxMtxDest = *(rs->c + 2);

                NNS_G3D_ASSERT(idxMtxDest < 31);
                // �����ł̓C�~�f�B�G�C�g���M�\�ł͂Ȃ���������Ȃ�
                NNS_G3dGeBufferOP_N(G3OP_MTX_STORE,
                                    &idxMtxDest,
                                    G3OP_MTX_STORE_NPARAMS);
            }
        }
    }

    rs->c += cmdLen;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_NODEMIX

    mnemonic:   NNS_G3D_SBC_NODEMIX
    operands:   idxMtxDest, numMtx, idxStack1, idxMtx1, ratio1, ... ,
                                    idxStackN, idxMtxN, ratioN
    callbacks:  none

    �������
    �Esum(ratio * InvM(InvN) * AnmM)���v�Z���āA�s��X�^�b�N��idxMtxDest�Ɋi�[���܂��B
      �܂�A���_���t�s��(InvM, InvN)�ɂ���Ċe�W���C���g�̃��[�J�����W�ɖ߂�
      �A�j���[�V�����s���������Ƃ���������s���s����u�����h���Ă��܂��B
    �E���f�����O�ϊ��s��̋t�s��(InvM, InvN)�̓��f�����\�[�X�Ɋi�[����Ă��܂��B
      g3dcvtr�ɂ��R���o�[�g�̍ۂɌv�Z����Ă��܂��B
    �E�s��̏�Z�̓W�I���g���G���W���𗘗p���čs���Ă��܂��B
    �E�E�F�C�e�b�h�G���x���[�v���������Ă��钸�_�̓O���[�o�����W�Ŋi�[����Ă��܂��B

    �t�L
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_NODEMIX(NNSG3dRS* rs, u32)
{
    fx64 w = 0;
    const NNSG3dResEvpMtx* evpMtx =
        (const NNSG3dResEvpMtx*)((u8*)rs->pRenderObj->resMdl +
                                    rs->pRenderObj->resMdl->ofsEvpMtx);
    u32 numMtx = *(rs->c + 2);
    u8* p = rs->c + 3;

    NNS_G3D_ASSERT(numMtx >= 2);

#if (NNS_G3D_USE_EVPCACHE)
    {
        u32 i;
        struct
        {
            MtxFx43 M;
            MtxFx33 N;
        } sum;
        MtxFx44* pX;
        MtxFx33* pY;

        MI_CpuClearFast(&sum, sizeof(sum));
        NNS_G3dGeFlushBuffer();

        G3_MtxMode(GX_MTXMODE_PROJECTION);
        G3_StoreMtx(1);
        G3_Identity();
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        for (i = 0; i < numMtx; ++i)
        {
            u32 idxJnt = *(p + 1);
            BOOL evpCached = NNSi_G3dBitVecCheck(&rs->isEvpCached[0], idxJnt);

            pX = &NNS_G3dRSOnGlb.evpCache[idxJnt].M;
            if (!evpCached)
            {
                NNSi_G3dBitVecSet(&rs->isEvpCached[0], idxJnt);

                G3_RestoreMtx(*p);
                G3_MtxMode(GX_MTXMODE_POSITION);
                G3_MultMtx43(&evpMtx[idxJnt].invM);
            }

            if (i != 0)
            {
                sum.N.m[0][0] += (w * pY->m[0][0]) >> FX32_SHIFT;
                sum.N.m[0][1] += (w * pY->m[0][1]) >> FX32_SHIFT;
                sum.N.m[0][2] += (w * pY->m[0][2]) >> FX32_SHIFT;

                sum.N.m[1][0] += (w * pY->m[1][0]) >> FX32_SHIFT;
                sum.N.m[1][1] += (w * pY->m[1][1]) >> FX32_SHIFT;
                sum.N.m[1][2] += (w * pY->m[1][2]) >> FX32_SHIFT;

                sum.N.m[2][0] += (w * pY->m[2][0]) >> FX32_SHIFT;
                sum.N.m[2][1] += (w * pY->m[2][1]) >> FX32_SHIFT;
                sum.N.m[2][2] += (w * pY->m[2][2]) >> FX32_SHIFT;
            }

            if (!evpCached)
            {
                while (G3X_GetClipMtx(pX))
                    ;
                G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
                G3_MultMtx33(&evpMtx[idxJnt].invN);
            }

            w = *(p + 2) << 4;

            sum.M.m[0][0] += (w * pX->m[0][0]) >> FX32_SHIFT;
            sum.M.m[0][1] += (w * pX->m[0][1]) >> FX32_SHIFT;
            sum.M.m[0][2] += (w * pX->m[0][2]) >> FX32_SHIFT;

            sum.M.m[1][0] += (w * pX->m[1][0]) >> FX32_SHIFT;
            sum.M.m[1][1] += (w * pX->m[1][1]) >> FX32_SHIFT;
            sum.M.m[1][2] += (w * pX->m[1][2]) >> FX32_SHIFT;

            sum.M.m[2][0] += (w * pX->m[2][0]) >> FX32_SHIFT;
            sum.M.m[2][1] += (w * pX->m[2][1]) >> FX32_SHIFT;
            sum.M.m[2][2] += (w * pX->m[2][2]) >> FX32_SHIFT;

            sum.M.m[3][0] += (w * pX->m[3][0]) >> FX32_SHIFT;
            sum.M.m[3][1] += (w * pX->m[3][1]) >> FX32_SHIFT;
            sum.M.m[3][2] += (w * pX->m[3][2]) >> FX32_SHIFT;

            p += 3;
            pY = &NNS_G3dRSOnGlb.evpCache[idxJnt].N;

            if (!evpCached)
            {
                while (G3X_GetVectorMtx(pY))
                    ;
            }
        }
        sum.N.m[0][0] += (w * pY->m[0][0]) >> FX32_SHIFT;
        sum.N.m[0][1] += (w * pY->m[0][1]) >> FX32_SHIFT;
        sum.N.m[0][2] += (w * pY->m[0][2]) >> FX32_SHIFT;

        sum.N.m[1][0] += (w * pY->m[1][0]) >> FX32_SHIFT;
        sum.N.m[1][1] += (w * pY->m[1][1]) >> FX32_SHIFT;
        sum.N.m[1][2] += (w * pY->m[1][2]) >> FX32_SHIFT;

        sum.N.m[2][0] += (w * pY->m[2][0]) >> FX32_SHIFT;
        sum.N.m[2][1] += (w * pY->m[2][1]) >> FX32_SHIFT;
        sum.N.m[2][2] += (w * pY->m[2][2]) >> FX32_SHIFT;

        G3_LoadMtx43((const MtxFx43*)&sum.N); // �㏑�������̂ŃS�~�f�[�^���������Ă��悢
        G3_MtxMode(GX_MTXMODE_POSITION);
        G3_LoadMtx43(&sum.M);
        G3_MtxMode(GX_MTXMODE_PROJECTION);
        G3_RestoreMtx(1);
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    }
#else
    {
        u32 i;
        struct
        {
            MtxFx43 M;
            MtxFx33 N;
        } sum;
        MtxFx44 X;
        MtxFx33 Y;

        MI_CpuClearFast(&sum, sizeof(sum));
        NNS_G3dGeFlushBuffer();

        G3_MtxMode(GX_MTXMODE_PROJECTION);
        G3_StoreMtx(1);
        G3_Identity();
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        for (i = 0; i < numMtx; ++i)
        {
            u32 idxJnt = *(p + 1);
            
            G3_RestoreMtx(*p);
            G3_MtxMode(GX_MTXMODE_POSITION);
            G3_MultMtx43(&evpMtx[idxJnt].invM);

            if (i != 0)
            {
                sum.N.m[0][0] += (w * Y.m[0][0]) >> FX32_SHIFT;
                sum.N.m[0][1] += (w * Y.m[0][1]) >> FX32_SHIFT;
                sum.N.m[0][2] += (w * Y.m[0][2]) >> FX32_SHIFT;

                sum.N.m[1][0] += (w * Y.m[1][0]) >> FX32_SHIFT;
                sum.N.m[1][1] += (w * Y.m[1][1]) >> FX32_SHIFT;
                sum.N.m[1][2] += (w * Y.m[1][2]) >> FX32_SHIFT;

                sum.N.m[2][0] += (w * Y.m[2][0]) >> FX32_SHIFT;
                sum.N.m[2][1] += (w * Y.m[2][1]) >> FX32_SHIFT;
                sum.N.m[2][2] += (w * Y.m[2][2]) >> FX32_SHIFT;
            }

            while (G3X_GetClipMtx(&X))
                ;
            G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
            G3_MultMtx33(&evpMtx[idxJnt].invN);

            w = *(p + 2) << 4;
            sum.M.m[0][0] += (w * X.m[0][0]) >> FX32_SHIFT;
            sum.M.m[0][1] += (w * X.m[0][1]) >> FX32_SHIFT;
            sum.M.m[0][2] += (w * X.m[0][2]) >> FX32_SHIFT;

            sum.M.m[1][0] += (w * X.m[1][0]) >> FX32_SHIFT;
            sum.M.m[1][1] += (w * X.m[1][1]) >> FX32_SHIFT;
            sum.M.m[1][2] += (w * X.m[1][2]) >> FX32_SHIFT;

            sum.M.m[2][0] += (w * X.m[2][0]) >> FX32_SHIFT;
            sum.M.m[2][1] += (w * X.m[2][1]) >> FX32_SHIFT;
            sum.M.m[2][2] += (w * X.m[2][2]) >> FX32_SHIFT;

            sum.M.m[3][0] += (w * X.m[3][0]) >> FX32_SHIFT;
            sum.M.m[3][1] += (w * X.m[3][1]) >> FX32_SHIFT;
            sum.M.m[3][2] += (w * X.m[3][2]) >> FX32_SHIFT;
            p += 3;

            while (G3X_GetVectorMtx(&Y))
                ;
        }
        sum.N.m[0][0] += (w * Y.m[0][0]) >> FX32_SHIFT;
        sum.N.m[0][1] += (w * Y.m[0][1]) >> FX32_SHIFT;
        sum.N.m[0][2] += (w * Y.m[0][2]) >> FX32_SHIFT;

        sum.N.m[1][0] += (w * Y.m[1][0]) >> FX32_SHIFT;
        sum.N.m[1][1] += (w * Y.m[1][1]) >> FX32_SHIFT;
        sum.N.m[1][2] += (w * Y.m[1][2]) >> FX32_SHIFT;

        sum.N.m[2][0] += (w * Y.m[2][0]) >> FX32_SHIFT;
        sum.N.m[2][1] += (w * Y.m[2][1]) >> FX32_SHIFT;
        sum.N.m[2][2] += (w * Y.m[2][2]) >> FX32_SHIFT;

        G3_LoadMtx43((const MtxFx43*)&sum.N); // �㏑�������̂ŃS�~�f�[�^���������Ă��悢
        G3_MtxMode(GX_MTXMODE_POSITION);
        G3_LoadMtx43(&sum.M);
        G3_MtxMode(GX_MTXMODE_PROJECTION);
        G3_RestoreMtx(1);
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    }
#endif
    G3_StoreMtx(*(rs->c + 1));
    rs->c += 3 + *(rs->c + 2) * 3;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_CALLDL

    mnemonic:   NNS_G3D_SBC_CALLDL
    operands:   rel_addr(1 word), size(1 word)
    callbacks:  A: before
                B: none
                C: after
    �������
    �E TIMING_A�̃R�[���o�b�N���Ă�
    �E �f�B�X�v���C���X�g�̓]�����s��
    �E TIMING_C�̃R�[���o�b�N���Ă�
    �E rs->c�̉��Z

    �t�L
    �ŏ���NODEDESC�ň�ʂ�s���ݒ肵�āACALLDL���ĂԂ��ƂŒP���ȃ��f����������
    �`�悷��悤�ȃR���o�[�g���@�����������ꍇ�ɁA���炭���̖��߂��K�v�ɂȂ�B
    ���[�U�[�ɂ��n�b�N�̂��߂̎�i�̂P�ł�����B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_CALLDL(NNSG3dRS* rs, u32)
{
    BOOL cbFlag;
    NNSG3dSbcCallBackTiming cbTiming;

    NNS_G3D_NULL_ASSERT(rs);

    // �R�[���o�b�N�`
    // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
    // �f�B�X�v���C���X�g�̑��M�����u�������邱�Ƃ��ł���B
    cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_CALLDL, &cbTiming);

    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD) &&
        (!cbFlag))
    {
        u32 rel_addr;
        u32 size;
        rel_addr = (u32)( (*(rs->c + 1) << 0 ) |
                          (*(rs->c + 2) << 8 ) |
                          (*(rs->c + 3) << 16) |
                          (*(rs->c + 4) << 24) );

        size = (u32)( (*(rs->c + 5) << 0 ) |
                      (*(rs->c + 6) << 8 ) |
                      (*(rs->c + 7) << 16) |
                      (*(rs->c + 8) << 24) );

        NNS_G3dGeSendDL(rs->c + rel_addr, size);
    }

    // �R�[���o�b�N�b
    // ���̖��߂̑O�ɉ��炩�̏�����}�����邱�Ƃ��ł���B
    (void) NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_CALLDL, cbTiming);

    rs->c += 1 + sizeof(u32) + sizeof(u32);
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_POSSCALE([000], [001])

    mnemonic:   NNS_G3D_SBC_POSSCALE
    operands:   none
    callbacks:  A: none
                B: none
                C: none

    �������
    �E �J�����g�s��ɃX�P�[�����O�s���������
       [000] ���f���f�[�^����posScale��v�f�Ƃ���X�P�[�����O�s���������
       [001] ���f���f�[�^����invPosScale��v�f�Ƃ���X�P�[�����O�s���������
    �E rs->c�̉��Z

    �t�L
    imd�t�@�C����pos_scale��0�łȂ��ꍇ�A���̃R�}���h���o�͂���Ă���B
    �`��̒��O�ɔz�u�����K�v������B
    ���[�U�[����ӎ��ł���K�v�͂Ȃ��Ǝv����̂ŃR�[���o�b�N��u���Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_POSSCALE(NNSG3dRS* rs, u32 opt)
{
    VecFx32 s;
    NNS_G3D_NULL_ASSERT(rs);
    NNS_G3D_ASSERT(opt == NNS_G3D_SBCFLG_000 ||
                   opt == NNS_G3D_SBCFLG_001);

    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD) &&
        !(rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW))
    {
        if (opt == NNS_G3D_SBCFLG_000)
        {
            s.x = s.y = s.z = rs->posScale;
        }
        else
        {
            // NNS_G3D_SBCFLG_001
            s.x = s.y = s.z = rs->invPosScale;
        }
        NNS_G3dGeBufferOP_N(G3OP_MTX_SCALE,
                            (u32*)&s.x,
                            G3OP_MTX_SCALE_NPARAMS);
    }

    rs->c += 1;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_ENVMAP

    mnemonic:   NNS_G3D_SBC_ENVMAP
    operands:   idxMat, flags(�\��)
    callbacks:  A: �@��->�e�N�X�`�����W�}�b�s���O�s��̃J�X�^�}�C�Y
                B: effect_mtx�ݒ�̃J�X�^�}�C�Y
                C: �@���ϊ��̃J�X�^�}�C�Y

    �������
    nrm * (C: �@���̕ϊ��s��) * (B: .imd�ɐݒ肳�ꂽ�G�t�F�N�g�s��) * 
          (A: �}�b�s���O�s��) * (material�Őݒ肳�ꂽ�e�N�X�`���s��)
    �Ƃ����v�Z���s���܂��BA, B, C�̒l�́A�R�[���o�b�N��ݒ肷�邱�Ƃɂ��A
    �J�X�^�}�C�Y���邱�Ƃ��ł��܂��B�f�t�H���g�̏�Ԃł́A�\�ʂƗ��ʂɂ��ꂼ��
    �P�����̃e�N�X�`���C���[�W��\��t����悤�ȃ}�b�s���O���s���܂��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_ENVMAP(NNSG3dRS* rs, u32)
{
    NNS_G3D_NULL_ASSERT(rs);

    // VISIBLE�łȂ��ꍇ�͖��߂��X�L�b�v
    // ����MatID��Ԃł��m�[�h���Ⴆ�Όv�Z���Ȃ����K�v������
    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW) &&
        (rs->flag & NNS_G3D_RSFLAG_NODE_VISIBLE))
    {
        BOOL cbFlag;
        NNSG3dSbcCallBackTiming cbTiming;

        // �e�N�X�`��SRT�A�j����t���Ă���ꍇ�A
        // TEXGEN��GX_TEXGEN_TEXCOORD�ɂȂ��Ă��܂��̂ŁA
        // GX_TEXGEN_NORMAL�ɐݒ肵�Ȃ����B
        if ((rs->pMatAnmResult->prmTexImage & REG_G3_TEXIMAGE_PARAM_TGEN_MASK) !=
                (GX_TEXGEN_NORMAL << REG_G3_TEXIMAGE_PARAM_TGEN_SHIFT))
        {
            static u32 cmd[] =
            {
                G3OP_TEXIMAGE_PARAM,
                0
            };
            rs->pMatAnmResult->prmTexImage &= ~REG_G3_TEXIMAGE_PARAM_TGEN_MASK;
            rs->pMatAnmResult->prmTexImage |= GX_TEXGEN_NORMAL << REG_G3_TEXIMAGE_PARAM_TGEN_SHIFT;

            cmd[1] = rs->pMatAnmResult->prmTexImage;
            NNS_G3dGeBufferData_N(&cmd[0], 2);
        }

        // �e�N�X�`���s��𑀍�Ώۂɂ���B
        NNS_G3dGeMtxMode(GX_MTXMODE_TEXTURE);

        // �R�[���o�b�N�`
        // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
        // qMtx�̐ݒ��u�������邱�Ƃ��ł���B
        cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_ENVMAP, &cbTiming);
        if (!cbFlag)
        {
            // callback A�ŃJ�X�^�}�C�Y�ł��鏈��
            s32 width, height;
            width = (s32)rs->pMatAnmResult->origWidth;
            height =(s32)rs->pMatAnmResult->origHeight;

            // NOTICE:
            // �ϊ����S,T���W��q�Ŋ����Ȃ�����Paraboloid Mapping���������邱�Ƃ͂ł��Ȃ��B
            {
                //     0.5   0   0   0
                // m =  0  -0.5  0   0
                //      0    0   1   0
                //     0.5  0.5  0   1

                // �\�ʂƗ��ʂɂP�����e�N�X�`�����͂�悤�ȃ}�b�s���O
                NNS_G3dGeScale(width << (FX32_SHIFT + 3), -height << (FX32_SHIFT + 3), FX32_ONE << 4);
                NNS_G3dGeTexCoord(width << (FX32_SHIFT - 1), height << (FX32_SHIFT - 1));
            }
        }

        // �R�[���o�b�N�a
        // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
        // effect_mtx�̐ݒ��u�������邱�Ƃ��ł���B
        cbFlag = NNSi_G3dCallBackCheck_B(rs, NNS_G3D_SBC_ENVMAP, &cbTiming);
        if (!cbFlag)
        {
            u32 idxMat = *(rs->c + 1);
            const NNSG3dResMatData* mat =
                NNS_G3dGetMatDataByIdx(rs->pResMat, idxMat);

            // NITRO���ԃt�@�C������effect_mtx�����݂��Ă���ꍇ�́A
            // effect_mtx��������
            if (mat->flag & NNS_G3D_MATFLAG_EFFECTMTX)
            {
                // NITRO���ԃt�@�C������effect_mtx�����݂��Ă���ꍇ
                const MtxFx44* effect_mtx;
                const u8* p = (const u8*)mat + sizeof(NNSG3dResMatData);

                if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_SCALEONE))
                {
                    p += sizeof(fx32) + sizeof(fx32); // scaleS, scaleT
                }

                if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_ROTZERO))
                {
                    p += sizeof(fx16) + sizeof(fx16); // rotSin, rotCos
                }

                if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_TRANSZERO))
                {
                    p += sizeof(fx32) + sizeof(fx32); // transS, transT
                }

                effect_mtx = (const MtxFx44*)p;
                NNS_G3dGeMultMtx44(effect_mtx);
            }
        }

        // �R�[���o�b�N�b
        // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
        // ���͖@���x�N�g���̕ϊ��s���u�������邱�Ƃ��ł���B
        cbFlag = NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_ENVMAP, cbTiming);
        if (!cbFlag)
        {
            MtxFx33 n;
            NNS_G3dGeMtxMode(GX_MTXMODE_POSITION_VECTOR);
            NNS_G3dGetCurrentMtx(NULL, &n);
            NNS_G3dGeMtxMode(GX_MTXMODE_TEXTURE);

            //
            // ���͖@�������[���h���W�n�̕����ɕϊ�����B
            // �Ȃ��A���͂����@���x�N�g���́A�W�I���g���R�}���h�̕����Ɠ���(�W���C���g���W�n)�B
            //
            if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_WVP)
            {
                // NNS_G3dGlbFlushWVP�̂Ƃ��́A
                // �W���C���g���W�n->���[�J�����W�n->�r���[���W�n
                // �̕ϊ����s���B
                NNS_G3dGeMultMtx33((const MtxFx33*)NNS_G3dGlbGetCameraMtx()); // ���[���h���W�n -> �r���[���W�n
                NNS_G3dGeMultMtx33(NNS_G3dGlbGetBaseRot());                   // ���[�J�����W�n -> ���[���h���W�n
                NNS_G3dGeMultMtx33(&n);                                       // �W���C���g���W�n -> ���[�J�����W�n

            }
            else if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_VP)
            {
                // NNS_G3dGlbFlushVP�̂Ƃ���
                // ���̂܂܂Ń��[���h���W�n�̕����x�N�g���ɂȂ�B
                NNS_G3dGeMultMtx33((const MtxFx33*)NNS_G3dGlbGetCameraMtx()); // ���[���h���W�n -> �r���[���W�n
                NNS_G3dGeMultMtx33(&n);                                       // �W���C���g���W�n -> ���[���h���W�n
            }
            else
            {
                NNS_G3dGeMultMtx33(&n);                            // �W���C���g���W�n -> �r���[���W�n
            }
        }

        // ����Ώۍs������ɖ߂�
        NNS_G3dGeMtxMode(GX_MTXMODE_POSITION_VECTOR);
    }
    rs->c += 3;
}


/*---------------------------------------------------------------------------*
    NNSi_G3dFuncSbc_PRJMAP

    mnemonic:   NNS_G3D_SBC_PRJMAP([000], [001], [010])
    operands:   idxMat
    callbacks:  A: ���_���W->�e�N�X�`�����W�}�b�s���O�s��̃J�X�^�}�C�Y
                B: effect_mtx�ݒ�̃J�X�^�}�C�Y
                C: ���_���W�ϊ��̃J�X�^�}�C�Y

    �������
    �v�Z�����e�N�X�`���s�񂩂�l�����o���āATexCoord�ɐݒ肷��K�v������̂ŁA
    ���o���̂��\�Ȉʒu���W�s���p���Čv�Z���s���܂��B
    ���̌���o���āA�e�N�X�`���s���TexCoord�ɐݒ肵�܂��B
    �Ȃ��AMAT�R�}���h�Őݒ肵���e�N�X�`���s��͏㏑�������̂Ŗ����ɂȂ�܂��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dFuncSbc_PRJMAP(NNSG3dRS* rs, u32 )
{
    //
    // �e�N�X�`���s��̈ꕔ��TexCoord��ݒ肷��K�v������̂ŁA
    // NNS_G3D_MTXSTACK_SYS���g�p���Čv�Z���Ă�����o���B
    //

    NNS_G3D_NULL_ASSERT(rs);

    // VISIBLE�łȂ��ꍇ�͖��߂��X�L�b�v
    // ����MatID��Ԃł��m�[�h���Ⴆ�Όv�Z���Ȃ����K�v������
    if (!(rs->flag & NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW) &&
        (rs->flag & NNS_G3D_RSFLAG_NODE_VISIBLE))
    {
        BOOL cbFlag;
        NNSG3dSbcCallBackTiming cbTiming;

        // ��Ŏg�p����J�����g�ʒu���W�s������o���B
        MtxFx43 m;
        NNS_G3dGetCurrentMtx(&m, NULL);
        NNS_G3dGeStoreMtx(NNS_G3D_MTXSTACK_SYS);

        // �e�N�X�`��SRT�A�j����t���Ă���ꍇ�A
        // TEXGEN��GX_TEXGEN_TEXCOORD�ɂȂ��Ă��܂��̂ŁA
        // GX_TEXGEN_VERTEX�ɐݒ肵�Ȃ����B
        if ((rs->pMatAnmResult->prmTexImage & REG_G3_TEXIMAGE_PARAM_TGEN_MASK) !=
                (GX_TEXGEN_VERTEX << REG_G3_TEXIMAGE_PARAM_TGEN_SHIFT))
        {
            static u32 cmd[] =
            {
                G3OP_TEXIMAGE_PARAM,
                0
            };
            rs->pMatAnmResult->prmTexImage &= ~REG_G3_TEXIMAGE_PARAM_TGEN_MASK;
            rs->pMatAnmResult->prmTexImage |= GX_TEXGEN_VERTEX << REG_G3_TEXIMAGE_PARAM_TGEN_SHIFT;

            cmd[1] = rs->pMatAnmResult->prmTexImage;
            NNS_G3dGeBufferData_N(&cmd[0], 2);
        }

        // �R�[���o�b�N�`
        // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
        // qMtx�̐ݒ��u�������邱�Ƃ��ł���B
        cbFlag = NNSi_G3dCallBackCheck_A(rs, NNS_G3D_SBC_PRJMAP, &cbTiming);
        if (!cbFlag)
        {
            // callback A�ŃJ�X�^�}�C�Y�ł��鏈��
            s32 width, height;
            width = (s32)rs->pMatAnmResult->origWidth;
            height = (s32)rs->pMatAnmResult->origHeight;

            {
                static MtxFx44 mtx = {
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, FX32_ONE << 4, 0,
                    0, 0, 0, FX32_ONE << 4
                };

                mtx._00 = width << (FX32_SHIFT + 3);
                mtx._11 = -height << (FX32_SHIFT + 3);
                mtx._30 = width << (FX32_SHIFT + 3);
                mtx._31 = height << (FX32_SHIFT + 3);

                //        0.5   0   0   0
                // mtx =   0  -0.5  0   0
                //         0    0   1   0
                //        0.5  0.5  0   1
                NNS_G3dGeLoadMtx44(&mtx);
            }
        }

        // �R�[���o�b�N�a
        // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
        // effect_mtx�̐ݒ��u�������邱�Ƃ��ł���B
        cbFlag = NNSi_G3dCallBackCheck_B(rs, NNS_G3D_SBC_PRJMAP, &cbTiming);
        if (!cbFlag)
        {
            u32 idxMat = *(rs->c + 1);
            const NNSG3dResMatData* mat =
                NNS_G3dGetMatDataByIdx(rs->pResMat, idxMat);

            // NITRO���ԃt�@�C������effect_mtx�����݂��Ă���ꍇ�́A
            // effect_mtx��������
            if (mat->flag & NNS_G3D_MATFLAG_EFFECTMTX)
            {
                // NITRO���ԃt�@�C������effect_mtx�����݂��Ă���ꍇ
                const MtxFx44* effect_mtx;
                const u8* p = (const u8*)mat + sizeof(NNSG3dResMatData);

                if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_SCALEONE))
                {
                    p += sizeof(fx32) + sizeof(fx32); // scaleS, scaleT
                }

                if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_ROTZERO))
                {
                    p += sizeof(fx16) + sizeof(fx16); // rotSin, rotCos
                }

                if (!(mat->flag & NNS_G3D_MATFLAG_TEXMTX_TRANSZERO))
                {
                    p += sizeof(fx32) + sizeof(fx32); // transS, transT
                }

                effect_mtx = (const MtxFx44*)p;
                NNS_G3dGeMultMtx44(effect_mtx);
            }
        }

        // �R�[���o�b�N�b
        // ������NNS_G3D_RSFLAG_SKIP���Z�b�g�����ꍇ�A
        // ���͖@���x�N�g���̕ϊ��s���u�������邱�Ƃ��ł���B
        cbFlag = NNSi_G3dCallBackCheck_C(rs, NNS_G3D_SBC_PRJMAP, cbTiming);
        if (!cbFlag)
        {
            MtxFx44 tex_mtx;

            //
            // ���͍��W�����[���h���W�n�̍��W�ɕϊ�����B
            // �Ȃ��A���͂����@���x�N�g���́A�W�I���g���R�}���h�̕����Ɠ���(�W���C���g���W�n)�B
            //
            if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_WVP)
            {
                // NNS_G3dGlbFlushWVP�̂Ƃ��́A
                // �W���C���g���W�n->���[�J�����W�n->���[���h���W�n
                // �̕ϊ����s���B
                NNS_G3dGeTranslateVec(NNS_G3dGlbGetBaseTrans());
                NNS_G3dGeMultMtx33(NNS_G3dGlbGetBaseRot());  // ���[�J�����W�n -> ���[���h���W�n
                NNS_G3dGeMultMtx43(&m);                      // �W���C���g���W�n -> ���[�J�����W�n

            }
            else if (NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_VP)
            {
                // NNS_G3dGlbFlushVP�̂Ƃ���
                // ���̂܂܂Ń��[���h���W�n�̕����x�N�g���ɂȂ�B
                NNS_G3dGeMultMtx43(&m);                      // �W���C���g���W�n -> ���[���h���W�n
            }
            else
            {
                // NNS_G3dGlbFlushP�̂Ƃ��́A
                // �W���C���g���W�n->�J�������W�n->���[���h���W�n
                // �̕ϊ����s���B
                NNS_G3dGeMultMtx43(NNS_G3dGlbGetInvV()); // �J�������W�n -> ���[���h���W�n
                NNS_G3dGeMultMtx43(&m);                            // �W���C���g���W�n -> �J�������W�n
            }

            //
            // �ʒu���W�s��X�^�b�N����v�Z�����e�N�X�`���s���ǂݖ߂�
            //
            {
                NNS_G3dGeFlushBuffer();

                G3_MtxMode(GX_MTXMODE_PROJECTION);
                G3_PushMtx();
                G3_Identity();

                while(G3X_GetClipMtx(&tex_mtx))
                    ;

                G3_PopMtx(1);
                G3_MtxMode(GX_MTXMODE_TEXTURE);
            }

            NNS_G3dGeLoadMtx44(&tex_mtx);
            NNS_G3dGeTexCoord(tex_mtx._30 >> 4, tex_mtx._31 >> 4);
        }

        // ����Ώۍs������ɖ߂�
        NNS_G3dGeMtxMode(GX_MTXMODE_POSITION_VECTOR);
        NNS_G3dGeRestoreMtx(NNS_G3D_MTXSTACK_SYS);
    }
    rs->c += 3;
}




////////////////////////////////////////////////////////////////////////////////
//
// �O���[�o���ϐ�
//

/*---------------------------------------------------------------------------*
    NNS_G3dRS

    �����_�����O���Ƀv���O�������ێ�������(�X�e�[�g�}�V���̃X�e�[�g)��ێ�����
    �\���̂ւ̃|�C���^�B�\���̗̂̈�́ANNS_G3dDraw�ɂ����ăX�^�b�N�̈�(DTCM)��
    �m�ۂ����BNNS_G3dRS�́ANNS_G3dDraw�I������NULL�ɃN���[���A�b�v�����B
 *---------------------------------------------------------------------------*/
NNSG3dRS* NNS_G3dRS = NULL;


/*---------------------------------------------------------------------------*
    NNS_G3dRSOnGlb

    �����_�����O���Ƀv���O�������ێ�������(�X�e�[�g�}�V���̃X�e�[�g)�ŁA
    �T�C�Y���傫�����߂ɃX�^�b�N�̈�ɒu���Ȃ����́B
 *---------------------------------------------------------------------------*/
NNSG3dRSOnGlb NNS_G3dRSOnGlb;


/*---------------------------------------------------------------------------*
    NNS_G3dFuncSbcTable

    SBC�R�[�h�̃n���h�����o�^����Ă���B
    const�ł͂Ȃ��̂̓��[�U�[�̏����������\�ɂ��邽�߂ł���B
 *---------------------------------------------------------------------------*/
NNSG3dFuncSbc NNS_G3dFuncSbcTable[NNS_G3D_SBC_COMMAND_NUM] =
{
    &NNSi_G3dFuncSbc_NOP,
    &NNSi_G3dFuncSbc_RET,
    &NNSi_G3dFuncSbc_NODE,
    &NNSi_G3dFuncSbc_MTX,
    &NNSi_G3dFuncSbc_MAT,
    &NNSi_G3dFuncSbc_SHP,
    &NNSi_G3dFuncSbc_NODEDESC,
    &NNSi_G3dFuncSbc_BB,

    &NNSi_G3dFuncSbc_BBY,
    &NNSi_G3dFuncSbc_NODEMIX,
    &NNSi_G3dFuncSbc_CALLDL,
    &NNSi_G3dFuncSbc_POSSCALE,
    &NNSi_G3dFuncSbc_ENVMAP,
    &NNSi_G3dFuncSbc_PRJMAP
};


/*---------------------------------------------------------------------------*
    NNS_G3dFuncSbcShpTable

    �X�̃V�F�C�v�f�[�^�̐擪�ɂ���^�O�͂��̃e�[�u���ւ̃C���f�b�N�X�ɂȂ��Ă���B
    �e�[�u���ɃV�F�C�v�f�[�^�̎�ޕʂ̃n���h����o�^���Ă����B
 *---------------------------------------------------------------------------*/
NNSG3dFuncSbc_ShpInternal NNS_G3dFuncSbcShpTable[NNS_G3D_SIZE_SHP_VTBL_NUM] =
{
    &NNSi_G3dFuncSbc_SHP_InternalDefault
};


/*---------------------------------------------------------------------------*
    NNS_G3dFuncSbcMatTable

    �X�̃}�e���A���f�[�^�̐擪�ɂ���^�O�͂��̃e�[�u���ւ̃C���f�b�N�X�ɂȂ��Ă���B
    �e�[�u���Ƀ}�e���A���f�[�^�̎�ޕʂ̃n���h����o�^���Ă����B
 *---------------------------------------------------------------------------*/
NNSG3dFuncSbc_MatInternal NNS_G3dFuncSbcMatTable[NNS_G3D_SIZE_MAT_VTBL_NUM] =
{
    &NNSi_G3dFuncSbc_MAT_InternalDefault
};

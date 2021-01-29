/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d
  File:     sbc_inline.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.14 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#ifndef NNSG3D_SBC_INLINE_H_
#define NNSG3D_SBC_INLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

//
// �R�[���o�b�N�`�F�b�N
//
#ifndef NNS_G3D_SBC_CALLBACK_TIMING_A_DISABLE
NNS_G3D_INLINE BOOL
NNSi_G3dCallBackCheck_A(NNSG3dRS* rs, u8 cmd, NNSG3dSbcCallBackTiming* pTiming)
{
    // �ŏ��ɌĂяo�����R�[���o�b�N�Ȃ̂�*pTiming��ݒ肷��
    *pTiming = NNSi_CheckPossibilityOfCallBack(rs, cmd);
    if (*pTiming == NNS_G3D_SBC_CALLBACK_TIMING_A)
    {
        rs->flag &= ~NNS_G3D_RSFLAG_SKIP;
        (*rs->cbVecFunc[cmd])(rs);
        // �R�[���o�b�N�����Őݒ肪�ύX����Ă���ꍇ������̂ōă`�F�b�N
        *pTiming = NNSi_CheckPossibilityOfCallBack(rs, cmd);
        return (BOOL)(rs->flag & NNS_G3D_RSFLAG_SKIP);
    }
    else
    {
        return FALSE;
    }
}
#else
NNS_G3D_INLINE BOOL
NNSi_G3dCallBackCheck_A(NNSG3dRS* rs, u8 cmd, NNSG3dSbcCallBackTiming* pTiming)
{
    // �ŏ��ɌĂяo�����R�[���o�b�N�Ȃ̂�*pTiming��ݒ肷��
    *pTiming = NNSi_CheckPossibilityOfCallBack(rs, cmd);
    return FALSE;
}
#endif


#ifndef NNS_G3D_SBC_CALLBACK_TIMING_B_DISABLE
NNS_G3D_INLINE BOOL
NNSi_G3dCallBackCheck_B(NNSG3dRS* rs, u8 cmd, NNSG3dSbcCallBackTiming* pTiming)
{
    if (*pTiming == NNS_G3D_SBC_CALLBACK_TIMING_B)
    {
        rs->flag &= ~NNS_G3D_RSFLAG_SKIP;
        (*rs->cbVecFunc[cmd])(rs);
        // �R�[���o�b�N�����Őݒ肪�ύX����Ă���ꍇ������̂ōă`�F�b�N
        *pTiming = NNSi_CheckPossibilityOfCallBack(rs, cmd);
        return (BOOL)(rs->flag & NNS_G3D_RSFLAG_SKIP);
    }
    else
    {
        return FALSE;
    }
}
#else
NNS_G3D_INLINE BOOL
NNSi_G3dCallBackCheck_B(NNSG3dRS*, u8, NNSG3dSbcCallBackTiming*) { return FALSE; }
#endif


#ifndef NNS_G3D_SBC_CALLBACK_TIMING_C_DISABLE
NNS_G3D_INLINE BOOL
NNSi_G3dCallBackCheck_C(NNSG3dRS* rs, u8 cmd, NNSG3dSbcCallBackTiming timing)
{
    if (timing == NNS_G3D_SBC_CALLBACK_TIMING_C)
    {
        rs->flag &= ~NNS_G3D_RSFLAG_SKIP;
        (*rs->cbVecFunc[cmd])(rs);
        // �Ō�̃R�[���o�b�N�Ȃ̂ōă`�F�b�N�̕K�v�͂Ȃ�
        return (BOOL)(rs->flag & NNS_G3D_RSFLAG_SKIP);
    }
    else
    {
        return FALSE;
    }
}
#else
NNS_G3D_INLINE BOOL
NNSi_G3dCallBackCheck_C(NNSG3dRS*, u8, NNSG3dSbcCallBackTiming) { return FALSE; }
#endif

//
//
//
NNS_G3D_INLINE NNSG3dSbcCallBackTiming
NNSi_CheckPossibilityOfCallBack(NNSG3dRS* rs, u8 cmd)
{
    if (rs->cbVecFunc[cmd])
    {
        return (NNSG3dSbcCallBackTiming)rs->cbVecTiming[cmd];        
    }
    else
    {
        return NNS_G3D_SBC_CALLBACK_TIMING_NONE;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSSetCallBack

    NNSG3dRS�\���̂�SBC�R�}���hcmd�p�ɐݒ�ł���R�[���o�b�N�֐���ݒ肷��B
    NNS_G3dRenderObjSetInitFunc�Őݒ�ł���C�j�V�����C�Y�֐����Ŏg�p�����̂����ʁB
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRSSetCallBack(NNSG3dRS* rs,
                     NNSG3dSbcCallBackFunc func,
                     u8 cmd,
                     NNSG3dSbcCallBackTiming timing)
{
    NNS_G3D_NULL_ASSERT(rs);
    NNS_G3D_SBC_CALLBACK_TIMING_ASSERT(timing);
    NNS_G3D_ASSERT(cmd < NNS_G3D_SBC_COMMAND_NUM);

    rs->cbVecFunc[cmd] = func;
    rs->cbVecTiming[cmd] = timing;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSResetCallBack

    NNSG3dRS�\���̂�cmd�p�R�[���o�b�N�֐�����������B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRSResetCallBack(NNSG3dRS* rs, u8 cmd)
{
    NNS_G3D_NULL_ASSERT(rs);
    NNS_G3D_ASSERT(cmd < NNS_G3D_SBC_COMMAND_NUM);

    rs->cbVecFunc[cmd] = NULL;
    rs->cbVecTiming[cmd] = (u8)NNS_G3D_SBC_CALLBACK_TIMING_NONE;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetRenderObj

    NNSG3dRS�\���̂��ێ�����NNSG3dRenderObj�\���̂ւ̃|�C���^��Ԃ�
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE NNSG3dRenderObj*
NNS_G3dRSGetRenderObj(NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    return rs->pRenderObj;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetMatAnmResult

    NNSG3dRS�\���̂��ێ�����NNSG3dMatAnmResult�\���̂ւ̃|�C���^��Ԃ�
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE NNSG3dMatAnmResult*
NNS_G3dRSGetMatAnmResult(NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    return rs->pMatAnmResult;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetJntAnmResult

    NNSG3dRS�\���̂��ێ�����NNSG3dJntAnmResult�\���̂ւ̃|�C���^��Ԃ�
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE NNSG3dJntAnmResult*
NNS_G3dRSGetJntAnmResult(NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    return rs->pJntAnmResult;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetJntAnmResult

    NNSG3dRS�\���̂��ێ�����NNSG3dJntAnmResult�\���̂ւ̃|�C���^��Ԃ�
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE NNSG3dVisAnmResult*
NNS_G3dRSGetVisAnmResult(NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    return rs->pVisAnmResult;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetSbcPtr

    NNSG3dRS�\���̂��ێ����錻�ݎ��s����SBC���߂ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE u8*
NNS_G3dRSGetSbcPtr(NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    return rs->c;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSSetFlag

    NNSG3dRS�\���̂��ێ�����t���O���Z�b�g���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRSSetFlag(NNSG3dRS* rs, NNSG3dRSFlag flag)
{
    NNS_G3D_NULL_ASSERT(rs);
    rs->flag |= flag;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSResetFlag

    NNSG3dRS�\���̂��ێ�����t���O�����Z�b�g���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRSResetFlag(NNSG3dRS* rs, NNSG3dRSFlag flag)
{
    NNS_G3D_NULL_ASSERT(rs);
    rs->flag &= ~flag;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetCurrentMatID

    ����NNSG3dRS�\���̂ɐݒ肳��Ă���}�e���A��ID���擾���܂��B
    �}�e���A��ID���ݒ肳��Ă��Ȃ��ꍇ��-1��Ԃ��܂��B

    �}�e���A��ID��SBC��MAT�R�}���h�Őݒ肳��A����MAT�R�}���h���I�[�o�[���C�h����
    ���A�R�[���o�b�N�֐��ɂ���ĕύX�����܂Œl��ێ����Â��܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE int
NNS_G3dRSGetCurrentMatID(const NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    if (rs->flag & NNS_G3D_RSFLAG_CURRENT_MAT_VALID)
    {
        return rs->currentMat;
    }
    else
    {
        return -1;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetCurrentNodeID

    ����NNSG3dRS�\���̂ɐݒ肳��Ă���m�[�hID���擾���܂��B
    �m�[�hID���ݒ肳��Ă��Ȃ��ꍇ��-1��Ԃ��܂��B

    �m�[�hID��SBC��NODE�R�}���h�Őݒ肳��A����NODE�R�}���h���I�[�o�[���C�h���邩�A
    �R�[���o�b�N�֐��ɂ���ĕύX�����܂Œl��ێ����Â��܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE int
NNS_G3dRSGetCurrentNodeID(const NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    if (rs->flag & NNS_G3D_RSFLAG_CURRENT_NODE_VALID)
    {
        return rs->currentNode;
    }
    else
    {
        return -1;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetCurrentNodeDescID

    ����NNSG3dRS�\���̂ɐݒ肳��Ă���NodeDescID���擾���܂��B
    NodeDescID���ݒ肳��Ă��Ȃ��ꍇ��-1��Ԃ��܂��B

    NodeDescID��SBC��NODEDESC�R�}���h�Őݒ肳��A����NODEDESC�R�}���h��
    �I�[�o�[���C�h���邩�A�R�[���o�b�N�֐��ɂ���ĕύX�����܂Œl��ێ�
    ���Â��܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE int
NNS_G3dRSGetCurrentNodeDescID(const NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    if (rs->flag & NNS_G3D_RSFLAG_CURRENT_NODEDESC_VALID)
    {
        return rs->currentNodeDesc;
    }
    else
    {
        return -1;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetPosScale

    NNSG3dRS�\���̂ɃL���b�V������Ă���A���_���W�ɑ΂��ď悴���X�P�[���l��
    �擾���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE fx32
NNS_G3dRSGetPosScale(const NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    return rs->posScale;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRSGetInvPosScale

    NNSG3dRS�\���̂ɃL���b�V������Ă���A���_���W�ɑ΂��ď悴���X�P�[���l��
    �t�����擾���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE fx32
NNS_G3dRSGetInvPosScale(const NNSG3dRS* rs)
{
    NNS_G3D_NULL_ASSERT(rs);
    return rs->invPosScale;
}


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif

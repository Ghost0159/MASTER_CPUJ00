/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d
  File:     sbc.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.24 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#ifndef NNSG3D_SBC_H_
#define NNSG3D_SBC_H_

#include <nnsys/g3d/config.h>
#include <nnsys/g3d/binres/res_struct.h>
#include <nnsys/g3d/kernel.h>
#include <nnsys/g3d/anm.h>
#include <nnsys/g3d/cgtool.h>

#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////////////////////////////////////////
//
// �\���̒�`�y��typedef
//

/*---------------------------------------------------------------------------*
    NNSG3dRSFlag

    NNSG3dRS���ɂ���1�r�b�g�t���O�̏W���B
    NNS_G3dDraw��SBC�R�}���h�֐����ŃZ�b�g���ꂽ��Q�Ƃ��ꂽ�肷��B

    NNS_G3D_RSFLAG_SKIP���R�[���o�b�N���Őݒ肷��ƁA����SBC���߂̍Ō�܂ŁA
    �R�[���o�b�N�̔���ȊO�̓�����X�L�b�v����悤�ɂȂ��Ă���B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_RSFLAG_NODE_VISIBLE           = 0x00000001,    // NODE�R�}���h��ON/OFF�����
    NNS_G3D_RSFLAG_MAT_TRANSPARENT        = 0x00000002,    // MAT�R�}���h��ON/OFF�����
    NNS_G3D_RSFLAG_CURRENT_NODE_VALID     = 0x00000004,    // NODE�R�}���h��ON�����
    NNS_G3D_RSFLAG_CURRENT_MAT_VALID      = 0x00000008,    // MAT�R�}���h��ON�����
    NNS_G3D_RSFLAG_CURRENT_NODEDESC_VALID = 0x00000010,
    NNS_G3D_RSFLAG_RETURN                 = 0x00000020,    // RET�R�}���h��ON�����
    NNS_G3D_RSFLAG_SKIP                   = 0x00000040,    // �R�[���o�b�N�p���[�U�[�t���O(�R�}���h�̓��̎��̃R�[���o�b�N�܂ŃX�L�b�v)

    // �ȉ��͎��s�O��NNSG3dRenderObj�ɂ���Č��肳���l
    NNS_G3D_RSFLAG_OPT_RECORD             = 0x00000080,    // NNSG3dRenderObj��recJntAnm, recMatAnm�Ɍv�Z���ʂ��L�^���܂��B
    NNS_G3D_RSFLAG_OPT_NOGECMD            = 0x00000100,    // �W�I���g���R�}���h�𑗐M���܂���B
    NNS_G3D_RSFLAG_OPT_SKIP_SBCDRAW       = 0x00000200,    // Draw�nSBC�R�}���h���X�L�b�v���܂��B�R�[���o�b�N���X�L�b�v���܂��B
    NNS_G3D_RSFLAG_OPT_SKIP_SBCMTXCALC    = 0x00000400     // MtxCalc�nSBC�R�}���h���X�L�b�v���܂��B�R�[���o�b�N���X�L�b�v���܂��B

    // Draw�nSBC�R�}���h�ꗗ
    // NODE         --- NNSi_G3dFuncSbc_NODE
    // MTX          --- NNSi_G3dFuncSbc_MTX
    // MAT          --- NNSi_G3dFuncSbc_MAT
    // SHP          --- NNSi_G3dFuncSbc_SHP
    // NODEDESC_BB  --- NNSi_G3dFuncSbc_BB
    // NODEDESC_BBY --- NNSi_G3dFuncSbc_BBY
    // POSSCALE     --- NNSi_G3dFuncSbc_POSSCALE

    // MtxCalc�nSBC�R�}���h�ꗗ
    // NODEDESC     --- NNSi_G3dFuncSbc_NODEDESC
}
NNSG3dRSFlag;


/*---------------------------------------------------------------------------*
    NNSG3dRS

    �`��G���W���̏�Ԃ��i�[����̂Ɏg�p�B�ʏ��DTCM���g�p���邽�߃X�^�b�N��
    �m�ۂ���邱�Ƃɒ��ӁB�`�撆�ɂ��Ƃ肷��ϐ��Ƃ����i�[�BNNS_G3dRS�Ƀ|�C���^��
    �i�[����Ă���B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dRS_
{
    // ��{���
    u8*                               c;                   // ���ݎQ�Ƃ��Ă���SBC���߂ւ̃|�C���^
    NNSG3dRenderObj*                  pRenderObj;
    u32                               flag;                // NNSG3dRSFlag 

    // �R�[���o�b�N�֐��x�N�^
    NNSG3dSbcCallBackFunc             cbVecFunc[NNS_G3D_SBC_COMMAND_NUM];   // �R�[���o�b�N�֐��̃x�N�^
    u8                                cbVecTiming[NNS_G3D_SBC_COMMAND_NUM]; // NNSG3dSbcCallBackTiming�̃x�N�^

    // flag��NNS_G3D_RSFLAG_CURRENT_NODE_VALID��ON�̂Ƃ��A���O�ɌĂ΂ꂽ(���ݎ��s����)
    // NODE�R�}���h��nodeID���i�[����Ă���B
    u8                                currentNode;

    // flag��NNS_G3D_RSFLAG_CURRENT_MAT_VALID��ON�̂Ƃ��A���O�ɌĂ΂ꂽ(���ݎ��s����)
    // MAT�R�}���h�̃}�e���A��ID���i�[����Ă���B
    // MAT�R�}���h�Ŏw�肳��Ă���}�e���A��ID�������ꍇ�A�}�e���A������
    // ���M���ȗ�����̂ŁA�R�[���o�b�N���Ń}�e���A���f�[�^��ύX�����ꍇ�́A
    // NNS_G3D_RSFLAG_CURRENT_MAT_VALID��OFF�ɂ���K�v������B
    u8                                currentMat;

    // flag��NNS_G3D_RSFLAG_CURRENT_NODEDESC_VALID��ON�̂Ƃ��A���O�ɌĂ΂ꂽ(���ݎ��s����)
    // NODEDESC�R�}���h�̃m�[�hID���i�[����Ă���B
    u8                                currentNodeDesc;

    u8                                dummy_;
    // NNSG3dMatAnmResult,NNSG3dJntAnmResult,NNSG3dVisAnmResult���v�Z���ł���ꍇ�ɂ�
    // ���̃����o�Ƀ|�C���^���i�[����Ă���B
    // �R�[���o�b�N�Ōv�Z���ʂ�ύX���邱�Ƃ��ł���悤�ɂȂ��Ă���B

    // �ŏ���MAT�R�}���h���Ăяo�����܂ł�NULL
    // ���̌�͒��O�Ɏ��s���ꂽMAT�R�}���h�ɂ��v�Z���ʂ��i�[
    NNSG3dMatAnmResult*               pMatAnmResult;

    // �ŏ���NODEDESC�R�}���h���Ăяo�����܂ł�NULL
    // ���̌�͒��O�Ɏ��s���ꂽNODEDESC�R�}���h�ɂ��v�Z���ʂ��i�[
    NNSG3dJntAnmResult*               pJntAnmResult;

    // �ŏ���NODE�R�}���h���Ăяo�����܂ł�NULL
    // ���̌�͒��O�Ɏ��s���ꂽNODE�R�}���h�ɂ��v�Z���ʂ��i�[
    NNSG3dVisAnmResult*               pVisAnmResult;

    // �}�e���A���f�[�^�����ɃL���b�V������Ă���ꍇ�̓r�b�g�������Ă���B
    // �f�[�^�{�̂�NNSG3dRSOnHeap��matCache�Ɋi�[����Ă���B
    u32                               isMatCached[NNS_G3D_SIZE_MAT_MAX / 32];

    // �X�P�[���֘A�f�[�^���i�[����ꍇ�A�f�[�^��(1.0, 1.0, 1.0)�ł���ꍇ�́A
    // �f�[�^���i�[�������ɑΉ�����r�b�g��ON�ɂ��Ă������Ƃő�p����B
    u32                               isScaleCacheOne[NNS_G3D_SIZE_JNT_MAX / 32];

#if (NNS_G3D_USE_EVPCACHE)
    u32                               isEvpCached[NNS_G3D_SIZE_JNT_MAX / 32];
#endif

    // resMdl�̒��ŌJ��Ԃ��g�������L���b�V�����Ă���
    // ���[�U�[���G��Ȃ���΂����Ȃ����Ƃ͂Ȃ��B
    const NNSG3dResNodeInfo*          pResNodeInfo;
    const NNSG3dResMat*               pResMat;
    const NNSG3dResShp*               pResShp;
    fx32                              posScale;
    fx32                              invPosScale;
    NNSG3dGetJointScale               funcJntScale;
    NNSG3dSendJointSRT                funcJntMtx;
    NNSG3dSendTexSRT                  funcTexMtx;

    NNSG3dMatAnmResult                tmpMatAnmResult;
    NNSG3dJntAnmResult                tmpJntAnmResult;
    NNSG3dVisAnmResult                tmpVisAnmResult;
}
NNSG3dRS;


/*---------------------------------------------------------------------------*
    NNSG3dRSOnGlb

    �`��G���W���̏�Ԃ̂����A�T�C�Y���傫�����߃X�^�b�N�ɓ����̂ɂ͓K���Ȃ�
    ���̂��i�[�B�O���[�o���ϐ���NNS_G3dRSOnGlb���g�p����B
    �I�n���C���������Ɋm�ۂ��ꂽ�܂܂̏�Ԃł���B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dRSOnGlb_
{
    //
    // Sbc��MAT���߂ɂ��L���b�V���O���ꂽ�}�e���A�����
    // NNSG3dRS��isMatChached�̑Ή��r�b�g��ON�ł���ꍇ�Ƀf�[�^�͗L���ł���B
    //
    struct NNSG3dMatAnmResult_ matCache[NNS_G3D_SIZE_MAT_MAX];

    //
    // Maya��Segment Scale Compensate, SI3D��Classic Scale Off�̏ꍇ��
    // �W���C���g�ɑΉ������X�P�[���֘A�f�[�^���i�[���邽�߂ɑ��݂���B
    // Sbc��NODEDESC���߂ɂ�菑�����܂��B
    // Maya��SSC�̏ꍇ:
    // NODEDESC�̃t���O�����ŁANNS_G3D_SBC_NODEDESC_FLAG_MAYASSC_PARENT
    // ��ON�̂Ƃ��A�X�P�[���̋t��������B
    //
    // SI3D��Classic Scale Off�̏ꍇ:
    // NNS_G3D_SCALINGRULE_SI3D���w�肳��Ă���
    // �ꍇ�ɐe�܂ł̃X�P�[���̗ݏ悪�i�[����Ă���B
    // 
    // ��������ANNSG3dRS��isScaleCacheOne�̑Ή�����r�b�g��OFF�̏ꍇ
    // (�X�P�[����1.0�łȂ��Ƃ�)�̂ݒl���i�[����Ă���
    //
    struct
    {
        VecFx32 s;
        VecFx32 inv;
    }
    scaleCache[NNS_G3D_SIZE_JNT_MAX];

#if (NNS_G3D_USE_EVPCACHE)
    struct
    {
        MtxFx44 M;
        MtxFx33 N;
    }
    evpCache[NNS_G3D_SIZE_JNT_MAX];
#endif
}
NNSG3dRSOnGlb;


/*---------------------------------------------------------------------------*
    NNSG3dFuncSbc

    Sbc�R�}���h�֐��̃|�C���^�^
 *---------------------------------------------------------------------------*/
typedef void (*NNSG3dFuncSbc)(NNSG3dRS*, u32);


/*---------------------------------------------------------------------------*
    NNSG3dFuncSbc_[Mat|Shp]Internal

    Sbc��MAT/SHP�R�}���h�����֐��̃|�C���^�^
 *---------------------------------------------------------------------------*/
typedef void (*NNSG3dFuncSbc_MatInternal)(NNSG3dRS*, u32, const NNSG3dResMatData*, u32);
typedef void (*NNSG3dFuncSbc_ShpInternal)(NNSG3dRS*, u32, const NNSG3dResShpData*, u32);


/*---------------------------------------------------------------------------*
    g3dcvtr����������R�[�h�Ŏg�p���Ȃ��s��X�^�b�N�C���f�b�N�X�̒�`
    NNS_G3D_MTXSTACK_SYS��G3D�����ŗ\�񂳂�Ă��āA
    NNS_G3D_MTXSTACK_USER�̓��[�U�[���g�p���邱�Ƃ��ł���B
 *---------------------------------------------------------------------------*/

#define NNS_G3D_MTXSTACK_SYS  (30)
#define NNS_G3D_MTXSTACK_USER (29)


////////////////////////////////////////////////////////////////////////////////
//
// �֐��̐錾
//

//
// NNS_G3dRS�ɑ΂���A�N�Z�T
//
NNS_G3D_INLINE void NNS_G3dRSSetCallBack(NNSG3dRS* rs, NNSG3dSbcCallBackFunc func, u8 cmd, NNSG3dSbcCallBackTiming timing);
NNS_G3D_INLINE void NNS_G3dRSResetCallBack(NNSG3dRS* rs, u8 cmd);
NNS_G3D_INLINE NNSG3dRenderObj* NNS_G3dRSGetRenderObj(NNSG3dRS* rs);
NNS_G3D_INLINE NNSG3dMatAnmResult* NNS_G3dRSGetMatAnmResult(NNSG3dRS* rs);
NNS_G3D_INLINE NNSG3dJntAnmResult* NNS_G3dRSGetJntAnmResult(NNSG3dRS* rs);
NNS_G3D_INLINE NNSG3dVisAnmResult* NNS_G3dRSGetVisAnmResult(NNSG3dRS* rs);
NNS_G3D_INLINE u8* NNS_G3dRSGetSbcPtr(NNSG3dRS* rs);
NNS_G3D_INLINE void NNS_G3dRSSetFlag(NNSG3dRS* rs, NNSG3dRSFlag flag);
NNS_G3D_INLINE void NNS_G3dRSResetFlag(NNSG3dRS* rs, NNSG3dRSFlag flag);
NNS_G3D_INLINE int NNS_G3dRSGetCurrentMatID(const NNSG3dRS* rs);
NNS_G3D_INLINE int NNS_G3dRSGetCurrentNodeID(const NNSG3dRS* rs);
NNS_G3D_INLINE int NNS_G3dRSGetCurrentNodeDescID(const NNSG3dRS* rs);
NNS_G3D_INLINE fx32 NNS_G3dRSGetPosScale(const NNSG3dRS* rs);
NNS_G3D_INLINE fx32 NNS_G3dRSGetInvPosScale(const NNSG3dRS* rs);



//
// �`��
//
void NNS_G3dDraw(struct NNSG3dRenderObj_* pRenderObj);

//
// SBC instructions
//
void NNSi_G3dFuncSbc_NOP(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_RET(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_NODE(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_MTX(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_MAT(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_SHP(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_NODEDESC(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_BB(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_BBY(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_NODEMIX(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_CALLDL(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_POSSCALE(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_ENVMAP(NNSG3dRS*, u32);
void NNSi_G3dFuncSbc_PRJMAP(NNSG3dRS*, u32);


// MAT/SHP��itemTag��0�̂Ƃ��̓���
void NNSi_G3dFuncSbc_SHP_InternalDefault(NNSG3dRS* rs,
                                        u32 opt,
                                        const NNSG3dResShpData* shp,
                                        u32 idxShp);

void NNSi_G3dFuncSbc_MAT_InternalDefault(NNSG3dRS* rs,
                                        u32 opt,
                                        const NNSG3dResMatData* mat,
                                        u32 idxMat);

//
// �R�[���o�b�N�̃`�F�b�N 
//
NNS_G3D_INLINE BOOL NNSi_G3dCallBackCheck_A(NNSG3dRS* rs, u8 cmd, NNSG3dSbcCallBackTiming* pTiming);
NNS_G3D_INLINE BOOL NNSi_G3dCallBackCheck_B(NNSG3dRS* rs, u8 cmd, NNSG3dSbcCallBackTiming* pTiming);
NNS_G3D_INLINE BOOL NNSi_G3dCallBackCheck_C(NNSG3dRS* rs, u8 cmd, NNSG3dSbcCallBackTiming timing);
NNS_G3D_INLINE NNSG3dSbcCallBackTiming NNSi_CheckPossibilityOfCallBack(NNSG3dRS* rs, u8 cmd);

////////////////////////////////////////////////////////////////////////////////
//
// �O���[�o���ϐ�
//
extern NNSG3dFuncSbc NNS_G3dFuncSbcTable[NNS_G3D_SBC_COMMAND_NUM];
extern NNSG3dFuncSbc_MatInternal NNS_G3dFuncSbcMatTable[NNS_G3D_SIZE_MAT_VTBL_NUM];
extern NNSG3dFuncSbc_ShpInternal NNS_G3dFuncSbcShpTable[NNS_G3D_SIZE_SHP_VTBL_NUM];
extern NNSG3dRS* NNS_G3dRS;
extern NNSG3dRSOnGlb NNS_G3dRSOnGlb;



#ifdef __cplusplus
}/* extern "C" */
#endif

#include <nnsys/g3d/sbc_inline.h>

#endif

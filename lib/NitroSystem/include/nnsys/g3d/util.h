/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d
  File:     util.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.20 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#ifndef NNSG3D_UTIL_H_
#define NNSG3D_UTIL_H_

#include <nnsys/g3d/config.h>
#include <nnsys/g3d/binres/res_struct.h>
#include <nnsys/g3d/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
// C++�̏ꍇ�������Ȃ���16�����ڂ��g�p�ł��Ȃ�����
#define NNS_G3D_UTIL_RESNAME_LEN 17
#else
#define NNS_G3D_UTIL_RESNAME_LEN 16
#endif


/*---------------------------------------------------------------------------*
    NNSG3dUtilResName

    ���\�[�X�����v���O�������Œ�`����Ƃ��Ɏg�p����鋤�p��
    NNS_G3D_DEFRESNAME��ʂ��Ďg�p����̂��悢�B
    C++�ł�16������t�g����悤�ɂ��邽�߂ɑ��݂���B
 *---------------------------------------------------------------------------*/
typedef union
{
    char _0[NNS_G3D_UTIL_RESNAME_LEN];
    struct
    {
        NNSG3dResName resName;
#ifdef __cplusplus
        u32           _1;
#endif
    };
}
NNSG3dUtilResName;

/*---------------------------------------------------------------------------*
    NNS_G3D_DEFRESNAME

    �v���O�����Ń��\�[�X�����g�p����ꍇ�Ɏg�p����}�N���B
    C++�ł�16���������ς��g����B

    WARNING
    �R���p�C���̎�����static�ȗ̈�͂��炩����0�ŃN���A�����悤�ɂȂ���
    ���Ȃ��Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
#define NNS_G3D_DEFRESNAME(var, str) \
    static const NNSG3dUtilResName var ATTRIBUTE_ALIGN(4) = { str }


//
// �����񃊃e�������g�p���Č�������ꍇ�̃}�N��
// NOTICE:
// �C�����C���֐������Ă͂����Ȃ�
//
#define NNS_G3D_GET_JNTID(pMdl, pJntID, literal)                                          \
    do {                                                                                  \
        static const char name[NNS_G3D_UTIL_RESNAME_LEN] ATTRIBUTE_ALIGN(4) = literal;    \
        *pJntID = NNS_G3dGetNodeIdxByName(NNS_G3dGetNodeInfo(pMdl), (NNSG3dResName*)name); \
    } while(0)

#define NNS_G3D_GET_MATID(pMdl, pMatID, literal)                                          \
    do {                                                                                  \
        static const char name[NNS_G3D_UTIL_RESNAME_LEN] ATTRIBUTE_ALIGN(4) = literal;    \
        *pMatID = NNS_G3dGetMatIdxByName(NNS_G3dGetMat(pMdl), (NNSG3dResName*)name);      \
    } while(0)

#define NNS_G3D_GET_SHPID(pMdl, pShpID, literal)                                          \
    do {                                                                                  \
        static const char name[NNS_G3D_UTIL_RESNAME_LEN] ATTRIBUTE_ALIGN(4) = literal;    \
        *pShpID = NNS_G3dGetShpIdxByName(NNS_G3dGetShp(pMdl), (NNSG3dResName*)name);      \
    } while(0)

#define NNS_G3D_GET_MDL(pMdlSet, pMdl, literal)                                           \
    do {                                                                                  \
        static const char name[NNS_G3D_UTIL_RESNAME_LEN] ATTRIBUTE_ALIGN(4) = literal;    \
        pMdl = NNS_G3dGetMdlByName(pMdlSet, (NNSG3dResName*)name);                        \
    } while(0)


//
// �A�j���[�V�������\�[�X�t�@�C������A�j���[�V�������\�[�X���擾
//
#define NNS_G3D_GET_ANM(pRes, pResAnm, literal)                                           \
    do {                                                                                  \
        static const char name[NNS_G3D_UTIL_RESNAME_LEN] ATTRIBUTE_ALIGN(4) = literal;    \
        pResAnm = NNS_G3dGetAnmByName(pRes, pName);                                       \
    } while(0)


//
// �r�b�g�x�N�g��
//
NNS_G3D_INLINE BOOL NNSi_G3dBitVecCheck(const u32* vec, u32 idx);
NNS_G3D_INLINE void NNSi_G3dBitVecSet(u32* vec, u32 idx);
NNS_G3D_INLINE void NNSi_G3dBitVecReset(u32* vec, u32 idx);


//
// �s��X�^�b�N����̎擾�ƕύX
//
void NNS_G3dGetCurrentMtx(MtxFx43* m, MtxFx33* n);
BOOL NNS_G3dGetResultMtx(const NNSG3dRenderObj* pRenderObj,
                         MtxFx43* pos,
                         MtxFx33* nrm,
                         u32 nodeID);
BOOL NNS_G3dSetResultMtx(const NNSG3dRenderObj* pRenderObj,
                         const MtxFx43* pos,
                         const MtxFx33* nrm,
                         u32 nodeID);


//
// �f�t�H���g�̃C�j�V�����C�Y
//
void NNS_G3dInit(void);

//
// SBC��̓��[�e�B���e�B
//
int NNS_G3dGetSbcCmdLen(const u8* c);
const u8* NNS_G3dSearchSbcCmd(const u8* c, u8 cmd);
const u8* NNS_G3dGetParentNodeID(int* parentID, const u8* sbc, u32 nodeID);
int NNS_G3dGetChildNodeIDList(u8* idList, const u8* sbc, u32 nodeID);

//
// ���\�[�X�Z�b�g�A�b�v�E�I���������[�e�B���e�B
//
BOOL NNS_G3dResDefaultSetup(void* pResData);
void NNS_G3dResDefaultRelease(void* pResData);

//
// ���W�ϊ����[�e�B���e�B
//
int NNS_G3dLocalOriginToScrPos(int* px, int* py);
int NNS_G3dWorldPosToScrPos(const VecFx32* pWorld, int* px, int* py);
int NNS_G3dScrPosToWorldLine(int px, int py, VecFx32* pNear, VecFx32* pFar);


#ifdef __cplusplus
}/* extern "C" */
#endif

#include <nnsys/g3d/util_inline.h>

#endif

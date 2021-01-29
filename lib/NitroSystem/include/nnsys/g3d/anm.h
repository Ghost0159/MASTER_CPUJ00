/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d
  File:     anm.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.16 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#ifndef NNSG3D_ANM_H_
#define NNSG3D_ANM_H_

#include <nnsys/g3d/config.h>
#include <nnsys/g3d/binres/res_struct.h>
#include <nnsys/g3d/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    �A�j���[�V�����̓���T��:

    matID, nodeID�Ɋւ��ăA�j���[�V�����������s���ꍇ�A
    SBC�C���^�v���^����NNSG3dRenderObj�ɓo�^����Ă���Blend�֐����Ă΂��B
    NNSG3d[Mat|Jnt]AnmResult*�ɂ́A����(���e�ɂ�BlendMat���ӔC�𕉂�)��
    �i�[����邱�ƂɂȂ�ANNSG3dAnmObj*��NNSG3dRenderObj�̃f�[�^�����o�ł���A
    �A�j���[�V�����I�u�W�F�N�g�̃��X�g�ł���B
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
//
// �\���̒�`�y��typedef
//


struct NNSG3dResMdl_;
/*---------------------------------------------------------------------------*
    NNSG3dAnimInitFunc

    NNSG3dAnmObj������������֐��̌^�Bvoid*�̓A�j���[�V�������\�[�X�ւ̃|�C���^�B
    �A�j���[�V�������\�[�X�̎�ނɂ����NNSG3dAnmObj�̏��������@�͈قȂ邽�߁B
 *---------------------------------------------------------------------------*/
typedef void (*NNSG3dAnimInitFunc)(NNSG3dAnmObj*,
                                   void*,
                                   const NNSG3dResMdl*);






/*---------------------------------------------------------------------------*
    NNSG3dAnmObjInitFunc

    category0��category1��NNSG3dResAnmHeader�̂��̂Ɠ����ł���B
 *---------------------------------------------------------------------------*/
typedef struct
{
    u8                  category0;
    u8                  dummy_;
    u16                 category1;
    NNSG3dAnimInitFunc  func;
}
NNSG3dAnmObjInitFunc;


/*---------------------------------------------------------------------------*
    �}�e���A���A�j���[�V�����̓���ɂ���

    �}�e���A���A�j���[�V�����̌v�Z��NNSG3dMatAnmResult�Ɏ��X�ƒl���㏑�����Ă����A
    �Ō�ɃW�I���g���G���W���Ƀf�[�^�𑗐M���邱�Ƃōs����B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SCALEONE  = 0x00000001,
    NNS_G3D_MATANM_RESULTFLAG_TEXMTX_ROTZERO   = 0x00000002,
    NNS_G3D_MATANM_RESULTFLAG_TEXMTX_TRANSZERO = 0x00000004,

    NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SET       = 0x00000008,
    NNS_G3D_MATANM_RESULTFLAG_TEXMTX_MULT      = 0x00000010,
    NNS_G3D_MATANM_RESULTFLAG_WIREFRAME        = 0x00000020
}
NNSG3dMatAnmResultFlag;


/*---------------------------------------------------------------------------*
    NNSG3dMatAnmResult

    �v�Z���ꂽ�}�e���A�������i�[���邽�߂̍\���́B
    SBC��MAT�R�}���h�����Ōv�Z�����B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dMatAnmResult_
{
    NNSG3dMatAnmResultFlag flag;
    u32                    prmMatColor0;
    u32                    prmMatColor1;
    u32                    prmPolygonAttr;
    u32                    prmTexImage;
    u32                    prmTexPltt;

    // flag��SCALEONE/ROTZERO/TRANSZERO�̏ꍇ��
    // �Ή�����l�̓Z�b�g����Ă��Ȃ��B
    fx32                   scaleS, scaleT;
    fx16                   sinR, cosR;
    fx32                   transS, transT;

    u16                    origWidth, origHeight;
    fx32                   magW, magH;
}
NNSG3dMatAnmResult;


/*---------------------------------------------------------------------------*
    �W���C���g�A�j���[�V�����̓���ɂ���

    �N�H�[�^�j�I���̓T�|�[�g����Ă��Ȃ��B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE      = 0x00000001,
    NNS_G3D_JNTANM_RESULTFLAG_ROT_ZERO       = 0x00000002,
    NNS_G3D_JNTANM_RESULTFLAG_TRANS_ZERO     = 0x00000004,
    NNS_G3D_JNTANM_RESULTFLAG_SCALEEX0_ONE   = 0x00000008,
    NNS_G3D_JNTANM_RESULTFLAG_SCALEEX1_ONE   = 0x00000010,
    NNS_G3D_JNTANM_RESULTFLAG_MAYA_SSC       = 0x00000020
//    NNS_G3D_JNTANM_RESULTFLAG_ROT_QUATERNION = 0x00000040
}
NNSG3dJntAnmResultFlag;


/*---------------------------------------------------------------------------*
    NNSG3dJntAnmResult

    �v�Z���ꂽ�m�[�h�����i�[���邽�߂̍\���́B
    SBC��NODEDESC�R�}���h�����Ōv�Z�����B
    scaleEx0��scaleEx1��Maya��SSC,Si3d��Classic Scale off�̏ꍇ��
    �t���I�ȃX�P�[�������i�[���邽�߂Ɏg�p�����B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dJntAnmResult_
{
    NNSG3dJntAnmResultFlag flag;
    VecFx32                scale;
    VecFx32                scaleEx0;
    VecFx32                scaleEx1;
    MtxFx33                rot;
    VecFx32                trans;
}
NNSG3dJntAnmResult;


/*---------------------------------------------------------------------------*
    �r�W�r���e�B�A�j���[�V�����̓���ɂ���

    �W���C���g���ۂ��ƌ������茩���Ȃ��Ȃ����肵�܂��B
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    NNSG3dVisAnmResult

    �v�Z���ꂽ�r�W�r���e�B�����i�[���邽�߂̍\���́B
    SBC��NODE�R�}���h�����Ōv�Z�����B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dVisAnmResult_
{
    BOOL isVisible;
}
NNSG3dVisAnmResult;


// �}�e���A���A�j���[�V�����v�Z�֐�
typedef void (*NNSG3dFuncAnmMat)(NNSG3dMatAnmResult*,
                                 const NNSG3dAnmObj*,
                                 u32);

// �W���C���g�A�j���[�V�����v�Z�֐�
typedef void (*NNSG3dFuncAnmJnt)(NNSG3dJntAnmResult*,
                                 const NNSG3dAnmObj*,
                                 u32);

// �r�W�r���e�B�A�j���[�V�����v�Z�֐�
typedef void (*NNSG3dFuncAnmVis)(NNSG3dVisAnmResult*,
                                 const NNSG3dAnmObj*,
                                 u32);


////////////////////////////////////////////////////////////////////////////////
//
// �֐��̐錾
//

//
// �}�e���A���A�j���[�V�����u�����h�֐��̃f�t�H���g
// �P���ɁA��Ōv�Z���ꂽ�A�j���[�V�������ʂ��D�悷��B
//
BOOL NNSi_G3dAnmBlendMat(NNSG3dMatAnmResult* pResult,
                         const NNSG3dAnmObj* pAnmObj,
                         u32 matID);

BOOL NNSi_G3dAnmBlendJnt(NNSG3dJntAnmResult*,
                         const NNSG3dAnmObj*,
                         u32);

BOOL NNSi_G3dAnmBlendVis(NNSG3dVisAnmResult*,
                         const NNSG3dAnmObj*,
                         u32);

////////////////////////////////////////////////////////////////////////////////
//
// �O���[�o���ϐ�
//

//
// �f�t�H���g�̃A�j���[�V�����u�����h�֐��ւ̃|�C���^
// NNS_G3dRenderObjInit�ɂ�����NNSG3dRenderObj�������ݒ肷�邽�߂Ɏg�p�����B
//
extern NNSG3dFuncAnmBlendMat NNS_G3dFuncBlendMatDefault;
extern NNSG3dFuncAnmBlendJnt NNS_G3dFuncBlendJntDefault;
extern NNSG3dFuncAnmBlendVis NNS_G3dFuncBlendVisDefault;

//
// �f�t�H���g�̃A�j���[�V�����v�Z�֐��ւ̃|�C���^
// NNS_G3dAnmObjInit�ɂ�����NNSG3dAnmObj�������ݒ肷�邽�߂Ɏg�p�����B
//
extern NNSG3dFuncAnmMat NNS_G3dFuncAnmMatNsBmaDefault;
extern NNSG3dFuncAnmMat NNS_G3dFuncAnmMatNsBtpDefault;
extern NNSG3dFuncAnmMat NNS_G3dFuncAnmMatNsBtaDefault;
extern NNSG3dFuncAnmJnt NNS_G3dFuncAnmJntNsBcaDefault;
extern NNSG3dFuncAnmVis NNS_G3dFuncAnmVisNsBvaDefault;

extern u32 NNS_G3dAnmFmtNum;
extern NNSG3dAnmObjInitFunc NNS_G3dAnmObjInitFuncArray[NNS_G3D_ANMFMT_MAX];


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif

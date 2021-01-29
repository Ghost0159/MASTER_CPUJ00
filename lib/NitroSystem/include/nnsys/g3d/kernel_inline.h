/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d
  File:     kernel_inline.h

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

#ifndef NNSG3D_KERNEL_INLINE_H_
#define NNSG3D_KERNEL_INLINE_H_
//
// DO NOT INCLUDE THIS FILE DIRECTLY
//

#include <nnsys/g3d/config.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// inline functions for NNSG3dAnmObj
//

/*---------------------------------------------------------------------------
    NNS_G3dAnmObjSetFrame

    �A�j���[�V�����I�u�W�F�N�g�̍Đ��t���[����ݒ肵�܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dAnmObjSetFrame(NNSG3dAnmObj* pAnmObj, fx32 frame)
{
    NNS_G3D_NULL_ASSERT(pAnmObj);

    pAnmObj->frame = frame;
}


/*---------------------------------------------------------------------------
    NNS_G3dAnmObjSetBlendRatio

    �A�j���[�V�����I�u�W�F�N�g�Ƀu�����h��(0����FX32_ONE�܂�)��ݒ肵�܂��B
    �A�j���[�V�����u�����h�֐����Q�Ƃ��܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dAnmObjSetBlendRatio(NNSG3dAnmObj* pAnmObj, fx32 ratio)
{
    NNS_G3D_NULL_ASSERT(pAnmObj);

    pAnmObj->ratio = ratio;
}


/*---------------------------------------------------------------------------
    NNS_G3dAnmObjGetNumFrame

    �A�j���[�V�����̃t���[�������擾���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE fx32
NNS_G3dAnmObjGetNumFrame(const NNSG3dAnmObj* pAnmObj)
{
    const NNSG3dResAnmCommon* p;
    NNS_G3D_NULL_ASSERT(pAnmObj);
    NNS_G3D_ASSERT(NNSi_G3dIsValidAnmRes(pAnmObj->resAnm));

    p = (const NNSG3dResAnmCommon*) pAnmObj->resAnm;
    return p->numFrame * FX32_ONE;
}





//
// inline functions for NNSG3dRenderObj
//

/*---------------------------------------------------------------------------
    NNS_G3dRenderObjSetFlag

    �����_�����O�I�u�W�F�N�g�̎��s����t���O�Ŏw�肳�ꂽ���̂��Z�b�g���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRenderObjSetFlag(NNSG3dRenderObj* pRenderObj, NNSG3dRenderObjFlag flag)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);

    pRenderObj->flag |= flag;
}


/*---------------------------------------------------------------------------
    NNS_G3dRenderObjResetFlag

    �����_�����O�I�u�W�F�N�g�̎��s����t���O�Ŏw�肳�ꂽ���̂����Z�b�g���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRenderObjResetFlag(NNSG3dRenderObj* pRenderObj, NNSG3dRenderObjFlag flag)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);

    pRenderObj->flag &= ~flag;
}


/*---------------------------------------------------------------------------
    NNS_G3dRenderObjTestFlag

    �����_�����O�I�u�W�F�N�g�̎��s����t���O���Z�b�g����Ă��邩�ǂ����e�X�g���܂��B
    �w�肵���t���O���S�ăZ�b�g����Ă���ꍇ��TRUE��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE BOOL
NNS_G3dRenderObjTestFlag(const NNSG3dRenderObj* pRenderObj, NNSG3dRenderObjFlag flag)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);

    return (pRenderObj->flag & flag) == flag;
}


/*---------------------------------------------------------------------------
    NNS_G3dRenderObjSetUserSbc

    ���[�U�[��`��SBC�������_�����O�I�u�W�F�N�g�ɐݒ肵�܂��B
    �Ԃ�l�͂���܂Őݒ肳��Ă������[�U�[��`��SBC�ł��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE u8*
NNS_G3dRenderObjSetUserSbc(NNSG3dRenderObj* pRenderObj, u8* sbc)
{
    u8* rval;
    NNS_G3D_NULL_ASSERT(pRenderObj);
    // sbc can be NULL

    rval = pRenderObj->ptrUserSbc;
    pRenderObj->ptrUserSbc = sbc;
    return rval;
}


/*---------------------------------------------------------------------------
    NNS_G3dRenderObjSetJntAnmBuffer

    �W���C���g�̌v�Z���ʂ��L�^�^�Đ�����o�b�t�@�������_�����O�I�u�W�F�N�g��
    �ݒ肵�܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRenderObjSetJntAnmBuffer(NNSG3dRenderObj* pRenderObj,
                                struct NNSG3dJntAnmResult_* buf)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);
    NNS_G3D_NULL_ASSERT(buf);

//    pRenderObj->flag |= NNS_G3D_RENDEROBJ_FLAG_RECORD;
    pRenderObj->recJntAnm = buf;
}


/*---------------------------------------------------------------------------
    NNS_G3dRenderObjSetMatAnmBuffer

    �}�e���A���̌v�Z���ʂ��L�^�^�Đ�����o�b�t�@�������_�����O�I�u�W�F�N�g��
    �ݒ肵�܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRenderObjSetMatAnmBuffer(NNSG3dRenderObj* pRenderObj,
                                struct NNSG3dMatAnmResult_* buf)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);
    NNS_G3D_NULL_ASSERT(buf);

//    pRenderObj->flag |= NNS_G3D_RENDEROBJ_FLAG_RECORD;
    pRenderObj->recMatAnm = buf;
}


/*---------------------------------------------------------------------------
    NNS_G3dRenderObjReleaseJntAnmBuffer

    �W���C���g�̌v�Z���ʂ��L�^�^�Đ�����o�b�t�@�������_�����O�I�u�W�F�N�g����
    ���O���܂��B�Ԃ�l�́A���O���ꂽ�o�b�t�@�ւ̃|�C���^�ł��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE struct NNSG3dJntAnmResult_*
NNS_G3dRenderObjReleaseJntAnmBuffer(NNSG3dRenderObj* pRenderObj)
{
    struct NNSG3dJntAnmResult_* rval;
    NNS_G3D_NULL_ASSERT(pRenderObj);

    // ���߉���o�b�t�@�������Ȃ��Ȃ����ꍇ��RECORD�t���O�����Z�b�g���Ă���
    if (!pRenderObj->recMatAnm)
        pRenderObj->flag &= ~NNS_G3D_RENDEROBJ_FLAG_RECORD;
    rval = pRenderObj->recJntAnm;
    pRenderObj->recJntAnm = NULL;
    return rval;
}


/*---------------------------------------------------------------------------
    NNS_G3dRenderObjReleaseMatAnmBuffer

    �}�e���A���̌v�Z���ʂ��L�^�^�Đ�����o�b�t�@�������_�����O�I�u�W�F�N�g����
    ���O���܂��B�Ԃ�l�́A���O���ꂽ�o�b�t�@�ւ̃|�C���^�ł��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE struct NNSG3dMatAnmResult_*
NNS_G3dRenderObjReleaseMatAnmBuffer(NNSG3dRenderObj* pRenderObj)
{
    struct NNSG3dMatAnmResult_* rval;
    NNS_G3D_NULL_ASSERT(pRenderObj);

    // ���߉���o�b�t�@�������Ȃ��Ȃ����ꍇ��RECORD�t���O�����Z�b�g���Ă���
    if (!pRenderObj->recJntAnm)
        pRenderObj->flag &= ~NNS_G3D_RENDEROBJ_FLAG_RECORD;
    rval = pRenderObj->recMatAnm;
    pRenderObj->recMatAnm = NULL;
    return rval;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRenderObjSetUserPtr

    �����_�����O�I�u�W�F�N�g�Ƀ��[�U�[���R�[���o�b�N�Ŏg�p�ł���̈�ւ�
    �|�C���^��ݒ肵�܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void*
NNS_G3dRenderObjSetUserPtr(NNSG3dRenderObj* pRenderObj, void* ptr)
{
    void* rval = pRenderObj->ptrUser;
    pRenderObj->ptrUser = ptr;
    return rval;
}


/*---------------------------------------------------------------------------*
    NNSG3dRenderObjGetResMdl

    �����_�����O�I�u�W�F�N�g���ێ����郂�f�����\�[�X�ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE NNSG3dResMdl*
NNS_G3dRenderObjGetResMdl(NNSG3dRenderObj* pRenderObj)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);
    return pRenderObj->resMdl;
}


/*---------------------------------------------------------------------------*
    NNSG3dRenderObjSetBlendFuncMat

    �����_�����O�I�u�W�F�N�g��(�Ǝ���)�}�e���A���u�����h�֐���ݒ肵�܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRenderObjSetBlendFuncMat(NNSG3dRenderObj* pRenderObj,
                                NNSG3dFuncAnmBlendMat func)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);
    NNS_G3D_NULL_ASSERT(func);

    pRenderObj->funcBlendMat = func;
}


/*---------------------------------------------------------------------------*
    NNSG3dRenderObjSetBlendFuncJnt

    �����_�����O�I�u�W�F�N�g��(�Ǝ���)�W���C���g�u�����h�֐���ݒ肵�܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRenderObjSetBlendFuncJnt(NNSG3dRenderObj* pRenderObj,
                                NNSG3dFuncAnmBlendJnt func)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);
    NNS_G3D_NULL_ASSERT(func);

    pRenderObj->funcBlendJnt = func;
}


/*---------------------------------------------------------------------------*
    NNSG3dRenderObjSetBlendFuncVis

    �����_�����O�I�u�W�F�N�g��(�Ǝ���)�r�W�r���e�B�u�����h�֐���ݒ肵�܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dRenderObjSetBlendFuncVis(NNSG3dRenderObj* pRenderObj,
                                NNSG3dFuncAnmBlendVis func)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);
    NNS_G3D_NULL_ASSERT(func);

    pRenderObj->funcBlendVis = func;
}



//
// Misc inline functions
//




#ifdef __cplusplus
}
#endif

#endif

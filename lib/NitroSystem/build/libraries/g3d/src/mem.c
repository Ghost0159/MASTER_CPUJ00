/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d
  File:     mem.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/mem.h>


/*---------------------------------------------------------------------------*
    NNS_G3dAllocRenderObj

    NNSG3dRenderObj�̂��߂̗̈���q�[�v�̈悩��m�ۂ��܂��B
    �C�j�V�����C�Y�͕ʓrNNS_G3dRenderObjInit�ōs���Ă��������B
 *---------------------------------------------------------------------------*/
NNSG3dRenderObj*
NNS_G3dAllocRenderObj(NNSFndAllocator* pAlloc)
{
    NNS_G3D_NULL_ASSERT(pAlloc);

    return (NNSG3dRenderObj*)
           NNS_FndAllocFromAllocator(pAlloc, sizeof(NNSG3dRenderObj));
}


/*---------------------------------------------------------------------------*
    NNS_G3dFreeRenderObj

    NNSG3dRenderObj�̗̈��������܂��B
    NNSG3dRenderObj���ێ����Ă���|�C���^���Q�Ƃ���̈�͉�����܂���B
    �ʓr���[�U�[���������K�v������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dFreeRenderObj(NNSFndAllocator* pAlloc,
                     NNSG3dRenderObj* pRenderObj)
{
    NNS_G3D_NULL_ASSERT(pAlloc);

    NNS_FndFreeToAllocator(pAlloc, pRenderObj);
}


/*---------------------------------------------------------------------------*
    NNS_G3dAllocAnmObj

    NNSG3dAnmObj�̂��߂̗̈���q�[�v�̈悩��m�ۂ��܂��B
    �C�j�V�����C�Y�͕ʓrNNS_G3dAnmObjInit�ōs���Ă��������B
 *---------------------------------------------------------------------------*/
NNSG3dAnmObj*
NNS_G3dAllocAnmObj(NNSFndAllocator* pAlloc,
                   const void* pAnm,
                   const NNSG3dResMdl* pMdl)
{
    u32 sz;
    NNS_G3D_NULL_ASSERT(pAlloc);
    NNS_G3D_NULL_ASSERT(pAnm);
    NNS_G3D_NULL_ASSERT(pMdl);

    sz = NNS_G3dAnmObjCalcSizeRequired(pAnm, pMdl);
    return (NNSG3dAnmObj*) NNS_FndAllocFromAllocator(pAlloc, sz);
}


/*---------------------------------------------------------------------------*
    NNS_G3dFreeAnmObj

    NNSG3dAnmObj�̗̈��������܂��B
    NNSG3dAnmObj���ێ����Ă���|�C���^���Q�Ƃ���̈�͉�����܂���B
    �ʓr���[�U�[���������K�v������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dFreeAnmObj(NNSFndAllocator* pAlloc,
                  NNSG3dAnmObj* pAnmObj)
{
    NNS_G3D_NULL_ASSERT(pAlloc);

    NNS_FndFreeToAllocator(pAlloc, pAnmObj);
}


/*---------------------------------------------------------------------------*
    NNS_G3dAllocRecBufferJnt

    NNSG3dRenderObj���ێ����邱�Ƃ��ł���A�W���C���g�̌v�Z���ʂ�ێ����邽�߂�
    �o�b�t�@�̈���m�ۂ��܂��B�̈�̑傫����NNSG3dResMdl�ɑ��݂���W���C���g�̐�
    �ɂ���Č��܂�܂��B
 *---------------------------------------------------------------------------*/
NNSG3dJntAnmResult*
NNS_G3dAllocRecBufferJnt(NNSFndAllocator* pAlloc,
                         const NNSG3dResMdl* pResMdl)
{
    u32 numJnt;
    NNS_G3D_NULL_ASSERT(pAlloc);
    NNS_G3D_NULL_ASSERT(pResMdl);

    numJnt = pResMdl->info.numNode;

    return (NNSG3dJntAnmResult*)
            NNS_FndAllocFromAllocator(pAlloc,
                                      NNS_G3D_RENDEROBJ_JNTBUFFER_SIZE(numJnt));
}


/*---------------------------------------------------------------------------*
    NNS_G3dFreeRecBufferJnt

    �W���C���g�̌v�Z���ʂ�ێ����邽�߂̃o�b�t�@�̈��������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dFreeRecBufferJnt(NNSFndAllocator* pAlloc,
                        NNSG3dJntAnmResult* pRecBuf)
{
    NNS_G3D_NULL_ASSERT(pAlloc);

    NNS_FndFreeToAllocator(pAlloc, pRecBuf);
}


/*---------------------------------------------------------------------------*
    NNS_G3dAllocRecBufferMat

    NNSG3dRenderObj���ێ����邱�Ƃ��ł���A�}�e���A���̌v�Z���ʂ�ێ����邽�߂�
    �o�b�t�@�̈���m�ۂ��܂��B�̈�̑傫����NNSG3dResMdl�ɑ��݂���}�e���A���̐�
    �ɂ���Č��܂�܂��B
 *---------------------------------------------------------------------------*/
NNSG3dMatAnmResult*
NNS_G3dAllocRecBufferMat(NNSFndAllocator* pAlloc,
                         const NNSG3dResMdl* pResMdl)
{
    u32 numMat;
    NNS_G3D_NULL_ASSERT(pAlloc);
    NNS_G3D_NULL_ASSERT(pResMdl);   

    numMat = pResMdl->info.numMat;

    return (NNSG3dMatAnmResult*)
            NNS_FndAllocFromAllocator(pAlloc,
                                      NNS_G3D_RENDEROBJ_MATBUFFER_SIZE(numMat));
}


/*---------------------------------------------------------------------------*
    NNS_G3dFreeRecBufferMat

    �}�e���A���̌v�Z���ʂ�ێ����邽�߂̃o�b�t�@�̈��������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dFreeRecBufferMat(NNSFndAllocator* pAlloc,
                        NNSG3dMatAnmResult* pRecBuf)
{
    NNS_G3D_NULL_ASSERT(pAlloc);

    NNS_FndFreeToAllocator(pAlloc, pRecBuf);
}

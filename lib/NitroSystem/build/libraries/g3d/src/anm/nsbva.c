/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d - src - anm
  File:     nsbva.c

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

#include <nnsys/g3d/anm/nsbva.h>
#include <nnsys/g3d/binres/res_struct_accessor.h>

#ifndef NNS_G3D_NSBVA_DISABLE
/*---------------------------------------------------------------------------*
    NNSi_G3dAnmObjInitNsBva

    NNSG3dAnmObj��.nsbva���\�[�X�p�ɃC�j�V�����C�Y���܂��B
    NNS_G3dInitAnmObj����Ăяo����܂��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dAnmObjInitNsBva(NNSG3dAnmObj* pAnmObj,
                        void* pResAnm,
                        const NNSG3dResMdl* pResMdl)
{
    u32 i;
    NNSG3dResVisAnm* visAnm;
    const NNSG3dResNodeInfo* jnt;

    visAnm = (NNSG3dResVisAnm*)pResAnm;
    jnt = NNS_G3dGetNodeInfo(pResMdl);
    pAnmObj->funcAnm = (void*) NNS_G3dFuncAnmVisNsBvaDefault;
    pAnmObj->numMapData = pResMdl->info.numNode;
   
    pAnmObj->resAnm = (void*)visAnm;
    
    //
    // �r�W�r���e�B�A�j���[�V�����̏ꍇ�͑S�Ẵm�[�h�ɑ΂��Ē�`����Ă���B
    //
    for (i = 0; i < pAnmObj->numMapData; ++i)
    {
        pAnmObj->mapData[i] = (u16)(i | NNS_G3D_ANMOBJ_MAPDATA_EXIST);
    }
}


/*---------------------------------------------------------------------------*
    NNSi_G3dAnmCalcNsBva

    pResult: �r�W�r���e�B�A�j���[�V�����̌��ʂ��i�[���܂��B
    pAnmObj:
    dataIdx: ���\�[�X���f�[�^�̊i�[�ꏊ�������C���f�b�N�X�ł�

 *---------------------------------------------------------------------------*/
void NNSi_G3dAnmCalcNsBva(NNSG3dVisAnmResult* pResult,
                          const NNSG3dAnmObj* pAnmObj,
                          u32 dataIdx)
{
    NNS_G3D_NULL_ASSERT(pResult);
    NNS_G3D_NULL_ASSERT(pAnmObj);
    NNS_G3D_NULL_ASSERT(pAnmObj->resAnm);

    {
        u32 frame = (u32)FX_Whole(pAnmObj->frame);
        const NNSG3dResVisAnm* pVisAnm = pAnmObj->resAnm;
        u32 pos = frame * pVisAnm->numNode + dataIdx;
        u32 n = pos >> 5;
        u32 mask = 1U << (pos & 0x1f);

        pResult->isVisible = (BOOL)(pVisAnm->visData[n] & mask);
    }
}

#endif // NNS_G3D_NSBVA_DISABLE



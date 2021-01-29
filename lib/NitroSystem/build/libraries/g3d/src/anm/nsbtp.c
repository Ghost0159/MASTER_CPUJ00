/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d - src - anm
  File:     nsbtp.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.21 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/anm/nsbtp.h>
#include <nnsys/g3d/binres/res_struct_accessor.h>

#ifndef NNS_G3D_NSBTP_DISABLE
/*---------------------------------------------------------------------------*
    NNSi_G3dAnmObjInitNsBtp

    NNSG3dAnmObj��.nsbtp���\�[�X�p�ɃC�j�V�����C�Y���܂��B
    NNS_G3dInitAnmObj����Ăяo����܂��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dAnmObjInitNsBtp(NNSG3dAnmObj*       pAnmObj,
                        void*               pResAnm,
                        const NNSG3dResMdl* pResMdl)
{
    u32 i;
    NNSG3dResTexPatAnm* tpAnm = (NNSG3dResTexPatAnm*)pResAnm;
    const NNSG3dResMat* mat = NNS_G3dGetMat(pResMdl);

    NNS_G3D_NULL_ASSERT(pAnmObj->resTex);

    pAnmObj->funcAnm    = (void*) NNS_G3dFuncAnmMatNsBtpDefault;
    pAnmObj->numMapData = pResMdl->info.numMat;
    pAnmObj->resAnm     = tpAnm;

    // �܂���mapData���[���N���A
    MI_CpuClear16(&pAnmObj->mapData[0], sizeof(u16) * pAnmObj->numMapData);

    for (i = 0; i < tpAnm->dict.numEntry; ++i)
    {
        const NNSG3dResName* name = NNS_G3dGetResNameByIdx(&tpAnm->dict, i);
        int idx = NNS_G3dGetMatIdxByName(mat, name);
        if (!(idx < 0))
        {
            // ���\�[�XID i�ɑΉ����郊�\�[�X�����݂���ꍇ�́A
            // �}�e���A��ID idx�Ƀ��\�[�XID i���֘A�t����B
            pAnmObj->mapData[idx] = (u16)(i | NNS_G3D_ANMOBJ_MAPDATA_EXIST);
        }
    }
}


/*---------------------------------------------------------------------------*
    SetTexParamaters_

    pResult �� �e�N�X�`���֘A������ݒ肵�܂�
 *---------------------------------------------------------------------------*/
static void SetTexParamaters_
( 
    const NNSG3dResTex*     pTex, 
    const NNSG3dResName*    pTexName, 
    NNSG3dMatAnmResult*     pResult 
)
{
    NNS_G3D_NULL_ASSERT( pTex );
    NNS_G3D_NULL_ASSERT( pTexName );
    NNS_G3D_NULL_ASSERT( pResult );
    NNS_G3D_ASSERTMSG((pTex->texInfo.vramKey != 0 || pTex->texInfo.sizeTex == 0),
                      "No texture key assigned");
    NNS_G3D_ASSERTMSG((pTex->tex4x4Info.vramKey != 0 || pTex->tex4x4Info.sizeTex == 0),
                      "No texture(4x4) key assigned");
    {
        //
        // ���O����e�N�X�`���f�[�^�������������܂�
        //
        const NNSG3dResDictTexData* pData = NNS_G3dGetTexDataByName( pTex, pTexName );        
        NNS_G3D_NULL_ASSERT( pData );
        
        {
            const u32 vramOffset = (( pData->texImageParam & REG_G3_TEXIMAGE_PARAM_TEXFMT_MASK) !=
                                                (GX_TEXFMT_COMP4x4 << REG_G3_TEXIMAGE_PARAM_TEXFMT_SHIFT)) ?
                                    NNS_GfdGetTexKeyAddr(pTex->texInfo.vramKey) >> NNS_GFD_TEXKEY_ADDR_SHIFT :    // 4x4�ȊO�̃e�N�X�`��
                                    NNS_GfdGetTexKeyAddr(pTex->tex4x4Info.vramKey) >> NNS_GFD_TEXKEY_ADDR_SHIFT;  // 4x4�e�N�X�`��

            // flip��repeat��texgen���c�����Z�b�g���Ă��獇������B
            // �}�e���A���������Ă���f�[�^�͎c���Ă����Ȃ��Ă͂Ȃ�Ȃ��B
            pResult->prmTexImage &= REG_G3_TEXIMAGE_PARAM_TGEN_MASK |
                                    REG_G3_TEXIMAGE_PARAM_FT_MASK | REG_G3_TEXIMAGE_PARAM_FS_MASK |
                                    REG_G3_TEXIMAGE_PARAM_RT_MASK | REG_G3_TEXIMAGE_PARAM_RS_MASK;
            pResult->prmTexImage |= pData->texImageParam + vramOffset;
            
            pResult->origWidth  = (u16)(( pData->extraParam & NNS_G3D_TEXIMAGE_PARAMEX_ORIGW_MASK ) >> 
                                    NNS_G3D_TEXIMAGE_PARAMEX_ORIGW_SHIFT);
            pResult->origHeight = (u16)(( pData->extraParam & NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_MASK ) >> 
                                    NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_SHIFT);
            
            //
            // �e�N�X�`���s��̌v�Z�ɕK�v�ȏc���̔{�����Z�o���܂��B
            //
            {
                const s32 w = (s32)(((pData->extraParam) & NNS_G3D_TEXIMAGE_PARAMEX_ORIGW_MASK) >> NNS_G3D_TEXIMAGE_PARAMEX_ORIGW_SHIFT);
                const s32 h = (s32)(((pData->extraParam) & NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_MASK) >> NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_SHIFT);
                                
                pResult->magW = ( w != pResult->origWidth ) ?
                                FX_Div( w << FX32_SHIFT, pResult->origWidth << FX32_SHIFT) :
                                FX32_ONE;
                pResult->magH = ( h != pResult->origHeight ) ?
                                FX_Div( h << FX32_SHIFT, pResult->origHeight << FX32_SHIFT) :
                                FX32_ONE;
            }
        }
    }
}


/*---------------------------------------------------------------------------*
    SetPlttParamaters_

    pResult �� �p���b�g�֘A������ݒ肵�܂�
 *---------------------------------------------------------------------------*/
static void SetPlttParamaters_
( 
    const NNSG3dResTex*     pTex, 
    const NNSG3dResName*    pPlttName, 
    NNSG3dMatAnmResult*     pResult 
)
{
    NNS_G3D_NULL_ASSERT(pTex);
    NNS_G3D_NULL_ASSERT(pPlttName);
    NNS_G3D_NULL_ASSERT(pResult);
    NNS_G3D_ASSERTMSG((pTex->plttInfo.vramKey != 0 || pTex->plttInfo.sizePltt == 0),
         "No palette key assigned");
    
    {
        // �e�N�X�`���u���b�N����p���b�g���ɑΉ�����f�[�^�t�B�[���h�𓾂�
        const NNSG3dResDictPlttData* pPlttData = NNS_G3dGetPlttDataByName(pTex, pPlttName);
        u16 plttBase    = pPlttData->offset;
        u16 vramOffset  = (u16)(NNS_GfdGetTexKeyAddr(pTex->plttInfo.vramKey) >> NNS_GFD_TEXKEY_ADDR_SHIFT);
        
        NNS_G3D_NULL_ASSERT(pPlttData);    
        
        
        // 4colors�Ȃ�r�b�g�������Ă���
        if (!(pPlttData->flag & 1))
        {
            // 4colors�ȊO�̏ꍇ��4bit�V�t�g�ɂȂ�B
            // 4colors�̏ꍇ��3bit�V�t�g�Ȃ̂ł��̂܂�
            plttBase >>= 1;
            vramOffset >>= 1;
        }
        
        //
        // �A�h���X��ݒ肷��
        //
        pResult->prmTexPltt = (u32)(plttBase + vramOffset);
    }
}


/*---------------------------------------------------------------------------*
    NNSi_G3dAnmCalcNsBtp

    pResult: �}�e���A���A�j���[�V�����̌��ʂ��i�[���܂��B
    pAnmObj:
    dataIdx: ���\�[�X���f�[�^�̊i�[�ꏊ�������C���f�b�N�X�ł�
 *---------------------------------------------------------------------------*/
void NNSi_G3dAnmCalcNsBtp(NNSG3dMatAnmResult* pResult,
                          const NNSG3dAnmObj* pAnmObj,
                          u32 dataIdx)
{
    NNS_G3D_NULL_ASSERT(pResult);
    NNS_G3D_NULL_ASSERT(pAnmObj);
    NNS_G3D_NULL_ASSERT(pAnmObj->resTex);
    
    {
        //
        // ���݂̃t���[���ɑΉ�����e�N�X�`���p�^���[��FV���擾���܂�
        //
        const NNSG3dResTexPatAnm*     pPatAnm 
            = (const NNSG3dResTexPatAnm*)pAnmObj->resAnm;
            
        const NNSG3dResTexPatAnmFV*   pTexFV 
            = NNSi_G3dGetTexPatAnmFV( pPatAnm, 
                                     (u16)dataIdx, 
                                     (u16)FX_Whole( pAnmObj->frame ) );
        NNS_G3D_NULL_ASSERT(pTexFV);
        
        //
        // pResult �� �e�N�X�`���֘A������ݒ肵�܂�
        //
        SetTexParamaters_( pAnmObj->resTex, 
                           NNSi_G3dGetTexPatAnmTexNameByIdx( pPatAnm, pTexFV->idTex ),
                           pResult ); 
        //                   
        // pResult �� �p���b�g�֘A������ݒ肵�܂�
        //
        
        //
        // ���݂̃R���o�[�^�̎����ł̓_�C���N�g�e�N�X�`���t�H�[�}�b�g�Q�Ǝ��ɂ�
        // pTexFV->idPltt �����o�� 255 ���������܂��BpTexFV->idPltt��255���ݒ肳�ꂽ�ꍇ���C�u������
        // �p���b�g�ݒ菈�����X�L�b�v����K�v������܂��B
        //
        if( pTexFV->idPltt != 255 )
        {
            SetPlttParamaters_( pAnmObj->resTex, 
                                NNSi_G3dGetTexPatAnmPlttNameByIdx( pPatAnm, pTexFV->idPltt ),
                                pResult ); 
        }                                     
    }
}

#endif // NNS_G3D_NSBTP_DISABLE

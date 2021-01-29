/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d - src - anm
  File:     nsbta.c

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

#include <nnsys/g3d/anm/nsbta.h>
#include <nnsys/g3d/binres/res_struct_accessor.h>

#ifndef NNS_G3D_NSBTA_DISABLE
//---------------------------------------------------------------------
static fx32
GetTexSRTAnmVectorVal_(const NNSG3dResTexSRTAnm* pTexAnm, 
                       u32 info, 
                       u32 data, 
                       u32 frame)
{
    u32 idx, idx_sub;
    u32 last_interp;
    const void* pDataHead;

    if( info & NNS_G3D_TEXSRTANM_ELEM_CONST )
    {
        return (fx32)data;
    }

    pDataHead = (const void*)((u8*)pTexAnm + data);

    if (!(info & NNS_G3D_TEXSRTANM_ELEM_STEP_MASK))
    {
        // NNS_G3D_TEXSRTANM_ELEM_STEP_1���I������Ă���
        idx = frame;
        goto TEXSRT_VAL_NONINTERP;
    }

    // last_interp�ȍ~��1�R�}���f�[�^�������Ă���
    // last_interp��2�̔{����4�̔{���ł���B
    last_interp = (NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_MASK & info) >>
                                NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_SHIFT;

    if (info & NNS_G3D_TEXSRTANM_ELEM_STEP_2)
    {
        // NNS_G3D_TEXSRTANM_ELEM_STEP_2���I������Ă���
        if (frame & 1)
        {
            if (frame > last_interp)
            {
                // �ŏI�t���[���ȊO���肦�Ȃ�
                idx = (last_interp >> 1) + 1;
                goto TEXSRT_VAL_NONINTERP;
            }
            else
            {
                // ��ōŏI�t���[���łȂ��̂�50:50�̕�ԏ���������B
                idx = frame >> 1;
                goto TEXSRT_VAL_INTERP_2;
            }
        }
        else
        {
            // �����t���[���Ȃ̂ŕ�ԏ����͂���Ȃ�
            idx = frame >> 1;
            goto TEXSRT_VAL_NONINTERP;
        }
    }
    else
    {
        // NNS_G3D_TEXSRTANM_ELEM_STEP_4���I������Ă���
        if (frame & 3)
        {
            if (frame > last_interp)
            {
                // last_interp�ȍ~�͕�ԏ����Ȃ�
                idx = (last_interp >> 2) + (frame & 3);
                goto TEXSRT_VAL_NONINTERP;
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

                if (info & NNS_G3D_TEXSRTANM_ELEM_FX16)
                {
                    v = *((const fx16*)pDataHead + idx);
                    v_sub = *((const fx16*)pDataHead + idx_sub);
                }
                else
                {
                    v = *((const fx32*)pDataHead + idx);
                    v_sub = *((const fx32*)pDataHead + idx_sub);
                }
                return (v + v + v + v_sub) >> 2;
            }
            else
            {
                // 50:50�̕�ԂɂȂ�
                idx = frame >> 2;
                goto TEXSRT_VAL_INTERP_2;
            }
        }
        else
        {
            // �t���[���͒��x4�̔{���ɂȂ��Ă���
            idx = frame >> 2;
            goto TEXSRT_VAL_NONINTERP;
        }
    }
    NNS_G3D_ASSERT(0);
TEXSRT_VAL_NONINTERP:
    if (info & NNS_G3D_TEXSRTANM_ELEM_FX16)
    {
        return *((const fx16*)pDataHead + idx);
    }
    else
    {
        return *((const fx32*)pDataHead + idx);
    }
TEXSRT_VAL_INTERP_2:
    {
        fx32 v0, v1;
        if (info & NNS_G3D_TEXSRTANM_ELEM_FX16)
        {
            v0 = *((const fx16*)pDataHead + idx);
            v1 = *((const fx16*)pDataHead + idx + 1);
        }
        else
        {
            v0 = *((const fx32*)pDataHead + idx);
            v1 = *((const fx32*)pDataHead + idx + 1);
        }
        return ((v0 + v1) >> 1);
    }
}


//---------------------------------------------------------------------
static u32
GetTexSRTAnmSinCosVal_(const NNSG3dResTexSRTAnm* pTexAnm, 
                       u32 info,
                       u32 data,
                       u32 frame)
{
    u32 idx, idx_sub;
    u32 last_interp;
    const void* pDataHead;

    NNS_G3D_ASSERT(!(info & NNS_G3D_TEXSRTANM_ELEM_FX16));

    if(info & NNS_G3D_TEXSRTANM_ELEM_CONST)
    {
        return data;
    }

    pDataHead = (const void*)((u8*)pTexAnm + data);

    if (!(info & NNS_G3D_TEXSRTANM_ELEM_STEP_MASK))
    {
        // NNS_G3D_TEXSRTANM_ELEM_STEP_1���I������Ă���
        idx = frame;
        goto TEXSRT_SINCOS_NONINTERP;
    }

    // last_interp�ȍ~��1�R�}���f�[�^�������Ă���
    // last_interp��2�̔{����4�̔{���ł���B
    last_interp = (NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_MASK & info) >>
                                NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_SHIFT;

    if (info & NNS_G3D_TEXSRTANM_ELEM_STEP_2)
    {
        // NNS_G3D_TEXSRTANM_ELEM_STEP_2���I������Ă���
        if (frame & 1)
        {
            if (frame > last_interp)
            {
                // �ŏI�t���[���ȊO���肦�Ȃ�
                idx = (last_interp >> 1) + 1;
                goto TEXSRT_SINCOS_NONINTERP;
            }
            else
            {
                // ��ōŏI�t���[���łȂ��̂�50:50�̕�ԏ���������B
                idx = frame >> 1;
                goto TEXSRT_SINCOS_INTERP_2;
            }
        }
        else
        {
            // �����t���[���Ȃ̂ŕ�ԏ����͂���Ȃ�
            idx = frame >> 1;
            goto TEXSRT_SINCOS_NONINTERP;
        }
    }
    else
    {
        // NNS_G3D_TEXSRTANM_ELEM_STEP_4���I������Ă���
        if (frame & 3)
        {
            if (frame > last_interp)
            {
                // last_interp�ȍ~�͕�ԏ����Ȃ�
                idx = (last_interp >> 2) + (frame & 3);
                goto TEXSRT_SINCOS_NONINTERP;
            }

            // ��ԏ�������
            if (frame & 1)
            {
                fx32 s, s_sub;
                fx32 c, c_sub;

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

                s = *((const fx16*)pDataHead + 2 * idx);
                c = *((const fx16*)pDataHead + 2 * idx + 1);
                s_sub = *((const fx16*)pDataHead + 2 * idx_sub);
                c_sub = *((const fx16*)pDataHead + 2 * idx_sub + 1);

                s = (s + s + s + s_sub) >> 2;
                c = (c + c + c + c_sub) >> 2;
                return (u32)((s & 0xffff) | (c << 16));
            }
            else
            {
                // 50:50�̕�ԂɂȂ�
                idx = frame >> 2;
                goto TEXSRT_SINCOS_INTERP_2;
            }
        }
        else
        {
            // �t���[���͒��x4�̔{���ɂȂ��Ă���
            idx = frame >> 2;
            goto TEXSRT_SINCOS_NONINTERP;
        }
    }
    NNS_G3D_ASSERT(0);
TEXSRT_SINCOS_NONINTERP:
    return *((const u32*)pDataHead + idx);
TEXSRT_SINCOS_INTERP_2:
    {
        fx32 s0, s1;
        fx32 c0, c1;
        s0 = *((const fx16*)pDataHead + 2 * idx);
        c0 = *((const fx16*)pDataHead + 2 * idx + 1);

        s1 = *((const fx16*)pDataHead + 2 * idx + 2);
        c1 = *((const fx16*)pDataHead + 2 * idx + 3);

        return (u32)((((s0 + s1) >> 1) & 0xffff) | (((c0 + c1) >> 1) << 16));
    }
}


static void 
GetTexSRTAnm_(const NNSG3dResTexSRTAnm* pTexAnm, 
              u16                       idx, 
              u32                       frame, 
              NNSG3dMatAnmResult*       pResult)
{
    NNS_G3D_ASSERT( idx < pTexAnm->dict.numEntry );
    NNS_G3D_ASSERT( frame < pTexAnm->numFrame );
    NNS_G3D_ASSERT( (pTexAnm->anmHeader.category0 == 'M') && (pTexAnm->anmHeader.category1 == 'TA') );
    
    {
        const NNSG3dResDictTexSRTAnmData* pAnmData = 
            (const NNSG3dResDictTexSRTAnmData*)NNS_G3dGetResDataByIdx( &pTexAnm->dict, idx );
        NNSG3dMatAnmResultFlag flag = pResult->flag;
        
        //
        // Trans
        //
        {
            fx32 transS, transT;
            
            transS = GetTexSRTAnmVectorVal_(pTexAnm,
                                            pAnmData->transS,
                                            pAnmData->transSEx,
                                            frame);
            transT = GetTexSRTAnmVectorVal_(pTexAnm,
                                            pAnmData->transT,
                                            pAnmData->transTEx,
                                            frame);
            //
            // �f�[�^�̓������L�q���Ă���A�t���O�l��ݒ肷��B
            // ���̐ݒ�ɂ���āA��ɍs���A�e�N�X�`���s��v�Z�̃R�X�g���팸���邱�Ƃ��\�ƂȂ�B
            // 
            if (transS == 0 && transT == 0 )
            {
                flag |=  NNS_G3D_MATANM_RESULTFLAG_TEXMTX_TRANSZERO;
            }
            else
            {
                flag &= ~NNS_G3D_MATANM_RESULTFLAG_TEXMTX_TRANSZERO;
                pResult->transS = transS;
                pResult->transT = transT;
            }
        }
        
        //
        // Rotation
        //
        {
            
            u32 data= GetTexSRTAnmSinCosVal_(pTexAnm, 
                                             pAnmData->rot,    
                                             pAnmData->rotEx,    
                                             frame); 

            //
            // �f�[�^�̓������L�q���Ă���A�t���O�l��ݒ肷��B
            //
            if (data == ((FX32_ONE << 16) | 0)) // sin = 0, cos = 1
            {
                flag |=  NNS_G3D_MATANM_RESULTFLAG_TEXMTX_ROTZERO;
            }
            else
            {
                pResult->sinR = (fx16)(data & 0x0000FFFF);
                pResult->cosR = (fx16)(data >> 16);
                flag &= ~NNS_G3D_MATANM_RESULTFLAG_TEXMTX_ROTZERO;
            }
        }
        
        //
        // Scale
        //
        {        
            fx32 scaleS, scaleT;

            scaleS = GetTexSRTAnmVectorVal_(pTexAnm,
                                            pAnmData->scaleS,
                                            pAnmData->scaleSEx,
                                            frame);
            scaleT = GetTexSRTAnmVectorVal_(pTexAnm,
                                            pAnmData->scaleT,
                                            pAnmData->scaleTEx,
                                            frame);    
           
            //
            // �f�[�^�̓������L�q���Ă���A�t���O�l��ݒ肷��B
            //
            if (scaleS == FX32_ONE && scaleT == FX32_ONE )
            {
                flag |=  NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SCALEONE;
            }
            else
            {
                flag &= ~NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SCALEONE;
                pResult->scaleS = scaleS;
                pResult->scaleT = scaleT;
            }
        }

        pResult->flag = flag;
    }
}


/*---------------------------------------------------------------------------*
    NNSi_G3dAnmObjInitNsBta

    NNSG3dAnmObj��.nsbta���\�[�X�p�ɃC�j�V�����C�Y���܂��B
    NNS_G3dInitAnmObj����Ăяo����܂��B
 *---------------------------------------------------------------------------*/
void
NNSi_G3dAnmObjInitNsBta(NNSG3dAnmObj* pAnmObj,
                        void* pResAnm,
                        const NNSG3dResMdl* pResMdl)
{
    u32 i;
    NNSG3dResTexSRTAnm* srtAnm;
    const NNSG3dResMat* mat;

    NNS_G3D_NULL_ASSERT(pAnmObj);
    NNS_G3D_NULL_ASSERT(pResAnm);
    NNS_G3D_NULL_ASSERT(pResMdl);

    srtAnm = (NNSG3dResTexSRTAnm*)pResAnm;
    mat = NNS_G3dGetMat(pResMdl);
    pAnmObj->funcAnm = (void*) NNS_G3dFuncAnmMatNsBtaDefault;
    pAnmObj->numMapData = pResMdl->info.numMat;

    // �܂���mapData���[���N���A
    MI_CpuClear16(&pAnmObj->mapData[0], sizeof(u16) * pAnmObj->numMapData);
    
    for (i = 0; i < srtAnm->dict.numEntry; ++i)
    {
        const NNSG3dResName* name = NNS_G3dGetResNameByIdx(&srtAnm->dict, i);
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
    NNSi_G3dAnmCalcNsBta

    pResult: �}�e���A���A�j���[�V�����̌��ʂ��i�[���܂��B
    pAnmObj: �A�j���[�V�����I�u�W�F�N�g�ւ̃|�C���^
    dataIdx: ���\�[�X���f�[�^�̊i�[�ꏊ�������C���f�b�N�X�ł�
 *---------------------------------------------------------------------------*/
void NNSi_G3dAnmCalcNsBta(NNSG3dMatAnmResult* pResult,
                          const NNSG3dAnmObj* pAnmObj,
                          u32 dataIdx)
{
    NNS_G3D_NULL_ASSERT(pResult);
    NNS_G3D_NULL_ASSERT(pAnmObj);

    {
        const NNSG3dResTexSRTAnm* pTexAnm 
                = (const NNSG3dResTexSRTAnm*)pAnmObj->resAnm;
                        
        GetTexSRTAnm_( pTexAnm,
                       (u16)dataIdx, 
                       (u32)FX_Whole( pAnmObj->frame ),
                       pResult );
        //
        // Texture SRT �g�p���ɂ͋����I�Ƀe�N�X�`�����W�������[�h��
        // GX_TEXGEN_TEXCOORD �ɐݒ肵�܂��B
        //
        pResult->prmTexImage &= ~REG_G3_TEXIMAGE_PARAM_TGEN_MASK;
        pResult->prmTexImage |= GX_TEXGEN_TEXCOORD << REG_G3_TEXIMAGE_PARAM_TGEN_SHIFT;
        
        //
        // �{�t���O��ݒ肵�Ȃ��ƁATexture�s�񂪃O���t�B�b�N�X�G���W���ɑ��M����Ȃ��B
        //
        pResult->flag |= NNS_G3D_MATANM_RESULTFLAG_TEXMTX_SET;        
        
    }
}

#endif // NNS_G3D_NSBTA_DISABLE

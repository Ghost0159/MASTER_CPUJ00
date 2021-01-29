/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d
  File:     kernel.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.35 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/kernel.h>
#include <nnsys/g3d/anm.h>
#include <nnsys/g3d/binres/res_struct.h>
#include <nnsys/g3d/binres/res_struct_accessor.h>


/*---------------------------------------------------------------------------*
    NNS_G3dAnmObjCalcSizeRequired

    �A�j���[�V�������\�[�X�A���f�����\�[�X�ɑΉ�����NNSG3dAnmObj�ɕK�v��
    �������T�C�Y��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u32
NNS_G3dAnmObjCalcSizeRequired(const void* pAnm,
                              const NNSG3dResMdl* pMdl)
{
    const NNSG3dResAnmHeader* hdr;
    NNS_G3D_NULL_ASSERT(pAnm);
    NNS_G3D_NULL_ASSERT(pMdl);
    if (!pAnm || !pMdl)
        return 0;

    hdr = (const NNSG3dResAnmHeader*)pAnm;
    switch(hdr->category0)
    {
    case 'M': // material animation
        return NNS_G3D_ANMOBJ_SIZE_MATANM(pMdl);
        break;
    case 'J': // character(joint) animation
    case 'V': // visibility animation
        return NNS_G3D_ANMOBJ_SIZE_JNTANM(pMdl);
        break;
    default:
        NNS_G3D_ASSERT(0);
        return 0;
        break;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dInitAnmObj

    pAnmObj��pResAnm, pResMdl�p�ɏ��������܂��B

    NOTICE:
    pAnmObj�͍Œ�ANNS_G3dAnmObjCalcSizeRequired(pResAnm, pResMdl)�̃T�C�Y��
    �����Ă���K�v������B
 *---------------------------------------------------------------------------*/
void
NNS_G3dAnmObjInit(NNSG3dAnmObj* pAnmObj,
                  void* pResAnm,
                  const NNSG3dResMdl* pResMdl,
                  const NNSG3dResTex* pResTex)
{
    // �v���O���}����`����A�j���[�V�����̏ꍇ���\�[�X���Ȃ��ꍇ�����邪�A
    // ���̏ꍇ�̓w�b�_�����������U�����ē���Ă��܂��΂悢�B
    const NNSG3dResAnmHeader* hdr;
    u32 i;
    NNS_G3D_NULL_ASSERT(pAnmObj);
    NNS_G3D_NULL_ASSERT(pResAnm);
    NNS_G3D_NULL_ASSERT(pResMdl);
    // resTex�̓e�N�X�`���p�^�[���A�j���[�V�����ȊO�ł�NULL�ł��悢�B
    NNS_G3D_ASSERT(NNS_G3dAnmFmtNum <= NNS_G3D_ANMFMT_MAX);

    pAnmObj->frame = 0;
    pAnmObj->resAnm = (void*)pResAnm;

    // �A�j���[�V�����֐��̐ݒ肪����B
    // �����category1�ɂ���Č��肳���
    pAnmObj->next = NULL;
    pAnmObj->priority = 127;
    pAnmObj->ratio = FX32_ONE;
    pAnmObj->resTex = pResTex;
    pAnmObj->numMapData = 0; // �A�j���[�V���������}�N��(NNS_G3D_NS***_DISABLE)�Ή��p
    pAnmObj->funcAnm = NULL; // �A�j���[�V���������}�N��(NNS_G3D_NS***_DISABLE)�Ή��p

    hdr = (const NNSG3dResAnmHeader*)pResAnm;

    for (i = 0; i < NNS_G3dAnmFmtNum; ++i)
    {
        if (NNS_G3dAnmObjInitFuncArray[i].category0 == hdr->category0 &&
            NNS_G3dAnmObjInitFuncArray[i].category1 == hdr->category1)
        {
            if (NNS_G3dAnmObjInitFuncArray[i].func)
            {
                (*NNS_G3dAnmObjInitFuncArray[i].func)(pAnmObj, pResAnm, pResMdl);
            }
            break;
        }
    }
    NNS_G3D_ASSERT(i != NNS_G3dAnmFmtNum);
}


/*---------------------------------------------------------------------------
    NNS_G3dAnmObjEnableID

    JntID,����MatID�ɑΉ������A�j���[�V�������Đ�����悤�ɂ��܂��B
    (�f�t�H���g)
 *---------------------------------------------------------------------------*/
void
NNS_G3dAnmObjEnableID(NNSG3dAnmObj* pAnmObj, int id)
{
    NNS_G3D_NULL_ASSERT(pAnmObj);

    if (id >= 0 && id < pAnmObj->numMapData &&
        (pAnmObj->mapData[id] & NNS_G3D_ANMOBJ_MAPDATA_EXIST))
        pAnmObj->mapData[id] &= ~NNS_G3D_ANMOBJ_MAPDATA_DISABLED;
}


/*---------------------------------------------------------------------------
    NNS_G3dAnmObjEnableID

    JntID,����MatID�ɑΉ������A�j���[�V�������Đ����Ȃ��悤�ɂ��܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dAnmObjDisableID(NNSG3dAnmObj* pAnmObj, int id)
{
    NNS_G3D_NULL_ASSERT(pAnmObj);

    if (id >= 0 && id < pAnmObj->numMapData &&
        (pAnmObj->mapData[id] & NNS_G3D_ANMOBJ_MAPDATA_EXIST))
        pAnmObj->mapData[id] |= NNS_G3D_ANMOBJ_MAPDATA_DISABLED;
}



/*---------------------------------------------------------------------------*
    NNS_G3dRenderObjInit

    NNSG3dRenderObj�����������܂��BpResMdl��NULL�ł�OK�ł��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dRenderObjInit(NNSG3dRenderObj* pRenderObj,
                     NNSG3dResMdl* pResMdl)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);
    // pResMdl can be NULL

    MI_CpuClear32(pRenderObj, sizeof(NNSG3dRenderObj));

    // �u�����h�̃f�t�H���g�n���h����ݒ�
    pRenderObj->funcBlendMat = NNS_G3dFuncBlendMatDefault;
    pRenderObj->funcBlendJnt = NNS_G3dFuncBlendJntDefault;
    pRenderObj->funcBlendVis = NNS_G3dFuncBlendVisDefault;

    pRenderObj->resMdl = pResMdl;
}


/*---------------------------------------------------------------------------*
    addLink_

    item�����Ƀ��X�g�ɂȂ��Ă���ꍇ�ɂ��Ή����Ă��邪�A���̃��X�g���\�[�g
    ����Ă��Ȃ��ꍇ�͐������Ȃ����ԂɂȂ�ꍇ������
 *---------------------------------------------------------------------------*/
static void
addLink_(NNSG3dAnmObj** l, NNSG3dAnmObj* item)
{
#ifdef NITRO_DEBUG
    // item��l���ɂ��łɂ���ꍇ�̓G���[
    {
        NNSG3dAnmObj* p = *l;
        while(p)
        {
            NNS_G3D_ASSERT(p != item);
            p = p->next;
        }
    }
#endif
    if (!(*l))
    {
        // ���X�g�����݂��Ȃ���΍ŏ��̗v�f�ɂȂ�B
        *l = item;
    }
    else if (!((*l)->next))
    {
        // ���X�g�̗v�f���P�̏ꍇ
        if ((*l)->priority > item->priority)
        {
            // item�̖����Ƀ��X�g���q����
            NNSG3dAnmObj* p = item;
            while(p->next)
            {
                p = p->next;
            }
            p->next = (*l);
            *l = item;
        }
        else
        {
            // �������X�g(�������v�f�͂P��)�̖����Ɍq����
            (*l)->next = item;
        }
    }
    else
    {
        NNSG3dAnmObj* p = *l;
        NNSG3dAnmObj* x = (*l)->next;

        while(x)
        {
            if (x->priority >= item->priority)
            {
                // �������X�g�ɑ}��
                NNSG3dAnmObj* pp = item;
                while(pp->next)
                {
                    pp = pp->next;
                }
                p->next = item;
                pp->next = x;
                return;
            }
            p = x;
            x = x->next;
        }
        // �������X�g�̖����Ɍq����
        p->next = item;
    }
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
    NNS_G3dRenderObjAddAnm

    pRenderObj��pAnmObj��ǉ�����BpAnmObj�̃w�b�_�������āA
    �K�؂ȃ��X�g�ɒǉ�����B
 *---------------------------------------------------------------------------*/
void
NNS_G3dRenderObjAddAnmObj(NNSG3dRenderObj* pRenderObj,
                          NNSG3dAnmObj* pAnmObj)
{
    const NNSG3dResAnmHeader* hdr;
    NNS_G3D_NULL_ASSERT(pRenderObj);
    NNS_G3D_NULL_ASSERT(pAnmObj);
    NNS_G3D_NULL_ASSERT(pAnmObj->resAnm);

    if (pAnmObj && pAnmObj->resAnm)
    {
        hdr = (const NNSG3dResAnmHeader*)pAnmObj->resAnm;

        switch(hdr->category0)
        {
        case 'M': // material animation
            updateHintVec_(&pRenderObj->hintMatAnmExist[0], pAnmObj);
            addLink_(&pRenderObj->anmMat, pAnmObj);
            break;
        case 'J': // character(joint) animation
            updateHintVec_(&pRenderObj->hintJntAnmExist[0], pAnmObj);
            addLink_(&pRenderObj->anmJnt, pAnmObj);
            break;
        case 'V': // visibility animation
            updateHintVec_(&pRenderObj->hintVisAnmExist[0], pAnmObj);
            addLink_(&pRenderObj->anmVis, pAnmObj);
            break;
        default:
            NNS_G3D_ASSERT(0);
            break;
        }
    }
}


/*---------------------------------------------------------------------------*
    removeLink_

    item�����X�g�����菜���܂��B���X�g����item���������ꍇ�ɂ�TRUE
    �Ȃ������ꍇ�ɂ�FALSE��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL
removeLink_(NNSG3dAnmObj** l, NNSG3dAnmObj* item)
{
    NNSG3dAnmObj* p;
    NNSG3dAnmObj* pp;

    if (!*l)
    {
        // AnmObj������Ȃ��ꍇ
        return FALSE;
    }
    
    if (*l == item)
    {
        // �ŏ���AnmObj����菜�����ꍇ
        *l = (*l)->next;
        item->next = NULL;
        return TRUE;
    }

    // ���X�g������
    p = (*l)->next;
    pp = (*l);
    while(p)
    {
        if (p == item)
        {
            pp->next = p->next;
            p->next = NULL;

            return TRUE;
        }
        pp = p;
        p = p->next;
    }
    return FALSE;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRenderObjRemoveAnmObj

    pRenderObj����pAnmObj���폜���܂��B
    �������A�q���g�p�̃r�b�g�x�N�g���͕ύX����܂���B
    �X�V�́ANNS_G3dDraw����NNS_G3D_RENDEROBJ_FLAG_HINT_OBSOLETE
    ���`�F�b�N����Ă���ꍇ�ɍs���܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dRenderObjRemoveAnmObj(NNSG3dRenderObj* pRenderObj,
                             NNSG3dAnmObj* pAnmObj)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);
    NNS_G3D_NULL_ASSERT(pAnmObj);

    if (removeLink_(&pRenderObj->anmMat, pAnmObj) ||
        removeLink_(&pRenderObj->anmJnt, pAnmObj) ||
        removeLink_(&pRenderObj->anmVis, pAnmObj))
    {
        NNS_G3dRenderObjSetFlag(pRenderObj, NNS_G3D_RENDEROBJ_FLAG_HINT_OBSOLETE);
        return;
    }
    
    // pAnmObj��pRenderObj�Ɍq�����Ă��Ȃ�����
    NNS_G3D_WARNING(0, "An AnmObj was not removed in NNS_G3dRenderObjRemoveAnmObj");
}


/*---------------------------------------------------------------------------*
    NNS_G3dRenderObjSetCallBack

    �`�揈�����ɍs���R�[���o�b�N������o�^���܂��B
    cmd�̃R�}���h�����s�����Ƃ��ŁA
    timing�Ŏw��ł���(3��ޑI�ׂ�)�ꏊ�ŁA�R�[���o�b�N���Ă΂�܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dRenderObjSetCallBack(NNSG3dRenderObj* pRenderObj,
                            NNSG3dSbcCallBackFunc func,
                            u8*,          // NOTICE:�R�[���o�b�N�̃A�h���X�w��͔p�~�ɂȂ�܂���
                            u8 cmd,
                            NNSG3dSbcCallBackTiming timing)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);

    pRenderObj->cbFunc = func;
    pRenderObj->cbCmd = cmd;
    pRenderObj->cbTiming = (u8)timing;
}



/*---------------------------------------------------------------------------*
    NNS_G3dRenderObjResetCallBack

    �`�揈�����ɍs���R�[���o�b�N�������������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dRenderObjResetCallBack(NNSG3dRenderObj* pRenderObj)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);

    pRenderObj->cbFunc = NULL;
    pRenderObj->cbCmd = 0;
    pRenderObj->cbTiming = 0;
}


/*---------------------------------------------------------------------------*
    NNS_G3dRenderObjSetInitFunc

    �`�揈�����O�ɌĂ΂��R�[���o�b�N��ݒ肵�܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dRenderObjSetInitFunc(NNSG3dRenderObj* pRenderObj,
                            NNSG3dSbcCallBackFunc func)
{
    NNS_G3D_NULL_ASSERT(pRenderObj);
    pRenderObj->cbInitFunc = func;
}


////////////////////////////////////////////////////////////////////////////////
//
// ���f��<->�e�N�X�`���ԃR�[�h
//
//

//
// Texture / 4x4Texture / Pltt�̃��C�t�T�C�N��
//
// 1 GetRequiredSize�ŃT�C�Y�����߂�
// 2 texKey�܂���plttKey���擾(VRAM�}�l�[�W���̃R�[�h�͌Ăяo���Ȃ�)
// 3 SetTexKey�܂���SetPlttKey�ŃL�[���Z�b�g
// 4 �e�N�X�`���^�p���b�g�f�[�^��VRAM�Ƀ��[�h
// 5 �`�悵�������f�����\�[�X�Ƀe�N�X�`���^�p���b�g�f�[�^���o�C���h
// 6 ���f�����\�[�X���g���ĕ`��
//(7 ���f�����\�[�X���ʂ̃e�N�X�`���Z�b�g�𗘗p����ꍇ�̓A���o�C���h)
// 8 �ǂ̃��f��������g�p���Ȃ��̂Ȃ烊���[�X(VRAM�}�l�[�W���̃R�[�h�͌Ăяo���Ȃ�)
//
// 4,5�͏��s��
//

/*---------------------------------------------------------------------------*
    NNS_G3dTexGetRequiredSize

    �e�N�X�`���u���b�N������4x4���k�e�N�X�`���ȊO�̃e�N�X�`���̃T�C�Y��Ԃ��܂�
 *---------------------------------------------------------------------------*/
u32
NNS_G3dTexGetRequiredSize(const NNSG3dResTex* pTex)
{
    NNS_G3D_NULL_ASSERT(pTex);

    if (pTex)
    {
        return (u32)(pTex->texInfo.sizeTex << 3);
    }
    else
    {
        return 0;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dTex4x4GetRequiredSize

    �e�N�X�`���u���b�N������4x4���k�e�N�X�`���̃T�C�Y��Ԃ��܂�
 *---------------------------------------------------------------------------*/
u32
NNS_G3dTex4x4GetRequiredSize(const NNSG3dResTex* pTex)
{
    NNS_G3D_NULL_ASSERT(pTex);

    if (pTex)
    {
        return (u32)(pTex->tex4x4Info.sizeTex << 3);
    }
    else
    {
        return 0;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dTexSetTexKey

    �e�N�X�`���u���b�N���̃e�N�X�`���փe�N�X�`���L�[�̊��蓖�Ă��s���܂�
 *---------------------------------------------------------------------------*/
void
NNS_G3dTexSetTexKey(NNSG3dResTex* pTex,
                    NNSG3dTexKey texKey,
                    NNSG3dTexKey tex4x4Key)
{
    if (texKey > 0)
    {
        NNS_G3D_NULL_ASSERT(pTex);
        NNS_G3D_ASSERTMSG(!(pTex->texInfo.flag & NNS_G3D_RESTEX_LOADED),
                        "NNS_G3dTexSetTexKey: Tex already loaded.");
        NNS_G3D_ASSERTMSG((pTex->texInfo.sizeTex << 3) <= NNS_GfdGetTexKeySize(texKey),
                        "NNS_G3dTexSetTexKey: texKey size not enough, "
                        "0x%05x bytes required.", pTex->texInfo.sizeTex << 3);
        pTex->texInfo.vramKey = texKey;
    }

    if (tex4x4Key > 0)
    {
        NNS_G3D_ASSERTMSG(!(pTex->tex4x4Info.flag & NNS_G3D_RESTEX4x4_LOADED),
                        "NNS_G3dTexSetTexKey(4x4): Tex already loaded.");
        NNS_G3D_ASSERTMSG(tex4x4Key & 0x80000000,
                        "NNS_G3dTexSetTexKey(4x4): texKey is not for 4x4comp textures");
        NNS_G3D_ASSERTMSG((pTex->tex4x4Info.sizeTex << 3) <= NNS_GfdGetTexKeySize(tex4x4Key),
                        "NNS_G3dTexSetTexKey(4x4): texKey size not enough, "
                        "0x%05x bytes required.", pTex->tex4x4Info.sizeTex << 3);
        pTex->tex4x4Info.vramKey = tex4x4Key;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dTexLoad

    pTex���̃e�N�X�`�����e�N�X�`���L�[�������ꏊ�Ƀ��[�h���܂��B
    exec_begin_end��TRUE�Ɏw�肷��ƁA
    ������VRAM�o���N�̐؂�ւ����s���܂��BFALSE�̏ꍇ�́A���[�U�[�����O�Ǝ����
    VRAM�o���N�̐؂�ւ����s���K�v������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dTexLoad(NNSG3dResTex* pTex, BOOL exec_begin_end)
{
    NNS_G3D_NULL_ASSERT(pTex);
    NNS_G3D_WARNING(!(pTex->texInfo.flag & NNS_G3D_RESTEX_LOADED),
                    "NNS_G3dTexLoad: texture already loaded");
    NNS_G3D_WARNING(!(pTex->tex4x4Info.flag & NNS_G3D_RESTEX4x4_LOADED),
                    "NNS_G3dTex4x4Load: texture already loaded");

    if (exec_begin_end)
    {
        // VRAM���o���N�؂�ւ����ă��C���������̃A�h���X��ԂɎ����Ă���B
        GX_BeginLoadTex();
    }

    {
        //
        // �ʏ�̃e�N�X�`�������[�h����
        //

        u32 sz;
        const void* pData;
        u32 from;

        sz = (u32)pTex->texInfo.sizeTex << 3;
        if (sz > 0) // �e�N�X�`���f�[�^�����݂��Ȃ��ꍇ��sz == 0�ɂȂ�B
        {
            NNS_G3D_ASSERT(pTex->texInfo.vramKey != 0);
            NNS_G3D_ASSERTMSG(sz <= NNS_GfdGetTexKeySize(pTex->texInfo.vramKey),
                              "NNS_G3dTexLoad: texKey size not enough, "
                              "0x%05x bytes required.", sz);
            
            pData = NNS_G3dGetTexData(pTex);
            from = NNS_GfdGetTexKeyAddr(pTex->texInfo.vramKey);

            GX_LoadTex(pData, from, sz);
    
            pTex->texInfo.flag |= NNS_G3D_RESTEX_LOADED;
        }
    }

    {
        //
        // 4x4���k�e�N�X�`�������[�h����
        //

        u32 sz;
        const void* pData;
        const void* pDataPlttIdx;
        u32 from;

        sz = (u32)pTex->tex4x4Info.sizeTex << 3;

        if (sz > 0) // �e�N�X�`���f�[�^�����݂��Ȃ��ꍇ��sz == 0�ɂȂ�B
        {
            NNS_G3D_ASSERT(pTex->tex4x4Info.sizeTex != 0);
            NNS_G3D_ASSERTMSG(NNS_GfdGetTexKey4x4Flag(pTex->tex4x4Info.vramKey),
                              "NNS_G3dTexLoad(4x4): texKey is not for 4x4comp textures");
            NNS_G3D_ASSERTMSG(sz <= NNS_GfdGetTexKeySize(pTex->tex4x4Info.vramKey),
                              "NNS_G3dTexLoad(4x4): texKey size not enough, "
                              "0x%05x bytes required.", sz);

            pData = NNS_G3dGetTex4x4Data(pTex);
            pDataPlttIdx = NNS_G3dGetTex4x4PlttIdxData(pTex);
            from = NNS_GfdGetTexKeyAddr(pTex->tex4x4Info.vramKey);

            GX_LoadTex(pData, from, sz);
            GX_LoadTex(pDataPlttIdx, GX_COMP4x4_PLTT_IDX(from), sz >> 1);
    
            pTex->tex4x4Info.flag |= NNS_G3D_RESTEX4x4_LOADED;
        }
    }

    if (exec_begin_end)
    {
        // �o���N�؂�ւ��������̂����ɖ߂�
        GX_EndLoadTex();
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dTexReleaseTexKey

    �e�N�X�`���u���b�N���̃e�N�X�`���L�[���蓖�Ă��������܂��B
    �L�[�͕Ԃ�l�Ƃ��ă��[�U�[�ɓn����܂��B
    VRAM�̃e�N�X�`���̈悻�̂��̂̉���͍s���܂���B
    ���[�U�[���L�[�𗘗p���ĉ������K�v������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dTexReleaseTexKey(NNSG3dResTex* pTex,
                        NNSG3dTexKey* texKey,
                        NNSG3dTexKey* tex4x4Key)
{
    NNS_G3D_NULL_ASSERT(pTex);
    NNS_G3D_NULL_ASSERT(texKey);
    NNS_G3D_NULL_ASSERT(tex4x4Key);

    if (texKey)
    {
        pTex->texInfo.flag &= ~NNS_G3D_RESTEX_LOADED;
        *texKey = pTex->texInfo.vramKey;
        pTex->texInfo.vramKey = 0;
    }

    if (tex4x4Key)
    {
        pTex->tex4x4Info.flag &= ~NNS_G3D_RESTEX4x4_LOADED;
        *tex4x4Key = pTex->tex4x4Info.vramKey;
        pTex->tex4x4Info.vramKey = 0;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dPlttGetRequiredSize

    �e�N�X�`���u���b�N�����p���b�g�̃T�C�Y��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
u32
NNS_G3dPlttGetRequiredSize(const NNSG3dResTex* pTex)
{
    NNS_G3D_NULL_ASSERT(pTex);

    if (pTex)
    {
        return (u32)(pTex->plttInfo.sizePltt << 3);
    }
    else
    {
        return 0;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dPlttSetPlttKey

    �e�N�X�`���u���b�N���̃p���b�g�փp���b�g�L�[�̊��蓖�Ă��s���܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dPlttSetPlttKey(NNSG3dResTex* pTex, NNSG3dPlttKey plttKey)
{
    NNS_G3D_NULL_ASSERT(pTex);
    NNS_G3D_ASSERTMSG(!(pTex->plttInfo.flag & NNS_G3D_RESPLTT_LOADED),
                      "NNS_G3dPlttSetPlttKey: Palette already loaded.");
    NNS_G3D_ASSERTMSG(!(pTex->plttInfo.sizePltt > (plttKey >> 16)),
                      "NNS_G3dPlttSetPlttKey: plttKey size not enough, "
                      "0x%05x bytes required.", pTex->plttInfo.sizePltt << 3);

    pTex->plttInfo.vramKey = plttKey;
}


/*---------------------------------------------------------------------------*
    NNS_G3dPlttLoad

    pTex���̃p���b�g���p���b�g�L�[�������ꏊ�Ƀ��[�h���܂��B
    exec_begin_end��TRUE���w�肷���
    ������VRAM�o���N�̐؂�ւ����s���܂��BFALSE�̏ꍇ�́A���[�U�[�����O�Ǝ����
    VRAM�o���N�̐؂�ւ����s���K�v������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dPlttLoad(NNSG3dResTex* pTex, BOOL exec_begin_end)
{
    NNS_G3D_NULL_ASSERT(pTex);
    NNS_G3D_WARNING(!(pTex->plttInfo.flag & NNS_G3D_RESPLTT_LOADED),
                    "NNS_G3dPlttLoad: texture already loaded");

    if (exec_begin_end)
    {
        // VRAM���o���N�؂�ւ����ă��C���������̃A�h���X��ԂɎ����Ă���B
        GX_BeginLoadTexPltt();
    }

    {
        u32 sz;
        const void* pData;
        u32 from;

        sz = (u32)pTex->plttInfo.sizePltt << 3;
        pData = NNS_G3dGetPlttData(pTex);
        from = NNS_GfdGetTexKeyAddr(pTex->plttInfo.vramKey);

        GX_LoadTexPltt(pData, from, sz);
    
        pTex->plttInfo.flag |= NNS_G3D_RESPLTT_LOADED;
    }

    if (exec_begin_end)
    {
        // �o���N�؂�ւ��������̂����ɖ߂�
        GX_EndLoadTexPltt();
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dPlttReleasePlttKey

    �e�N�X�`���u���b�N���̃p���b�g�u���b�N�ւ̃p���b�g�L�[���蓖�Ă��������܂��B
    �L�[�͕Ԃ�l�Ƃ��ă��[�U�[�ɓn����܂��B
    VRAM�̃p���b�g�̈悻�̂��̂̉���͍s���܂���B
    ���[�U�[���L�[�𗘗p���ĉ������K�v������܂��B
 *---------------------------------------------------------------------------*/
NNSG3dPlttKey
NNS_G3dPlttReleasePlttKey(NNSG3dResTex* pTex)
{
    u32 rval;
    NNS_G3D_NULL_ASSERT(pTex);

    pTex->plttInfo.flag &= ~NNS_G3D_RESPLTT_LOADED;
    
    rval = pTex->plttInfo.vramKey;
    pTex->plttInfo.vramKey = 0;
    return rval;
}


static void
bindMdlTex_Internal_(NNSG3dResMat* pMat,
                     NNSG3dResDictTexToMatIdxData* pBindData,
                     const NNSG3dResTex* pTex,
                     const NNSG3dResDictTexData* pTexData)
{
    u8* base = (u8*)pMat + pBindData->offset;
    u32 vramOffset;
    u32 j;

    NNS_G3D_ASSERTMSG((pTex->texInfo.vramKey != 0 || pTex->texInfo.sizeTex == 0),
                      "No texture key assigned");
    NNS_G3D_ASSERTMSG((pTex->tex4x4Info.vramKey != 0 || pTex->tex4x4Info.sizeTex == 0),
                      "No texture(4x4) key assigned");

    if ((pTexData->texImageParam & REG_G3_TEXIMAGE_PARAM_TEXFMT_MASK) !=
                    (GX_TEXFMT_COMP4x4 << REG_G3_TEXIMAGE_PARAM_TEXFMT_SHIFT))
    {
        // 4x4�ȊO�̃e�N�X�`��
        vramOffset = NNS_GfdGetTexKeyAddr(pTex->texInfo.vramKey)
                            >> NNS_GFD_TEXKEY_ADDR_SHIFT;
    }
    else
    {
        // 4x4�e�N�X�`��
        vramOffset = NNS_GfdGetTexKeyAddr(pTex->tex4x4Info.vramKey)
                            >> NNS_GFD_TEXKEY_ADDR_SHIFT;
    }

    for (j = 0; j < pBindData->numIdx; ++j)
    {
        s32 w, h;

        // �ematData�Ƀe�N�X�`�������Z�b�g�A�b�v���Ă����B
        NNSG3dResMatData* matData = NNS_G3dGetMatDataByIdx(pMat, *(base + j));

        matData->texImageParam |= (pTexData->texImageParam + vramOffset);
        w = (s32)(((pTexData->extraParam) & NNS_G3D_TEXIMAGE_PARAMEX_ORIGW_MASK) >> NNS_G3D_TEXIMAGE_PARAMEX_ORIGW_SHIFT);
        h = (s32)(((pTexData->extraParam) & NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_MASK) >> NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_SHIFT);

        matData->magW = (w != matData->origWidth) ?
                        FX_Div(w << FX32_SHIFT, matData->origWidth << FX32_SHIFT) :
                        FX32_ONE;
        matData->magH = (h != matData->origHeight) ?
                        FX_Div(h << FX32_SHIFT, matData->origHeight << FX32_SHIFT) :
                        FX32_ONE;
    }

    // �o�C���h��Ԃɂ���B
    pBindData->flag |= 1;
}

static void
releaseMdlTex_Internal_(NNSG3dResMat* pMat,
                        NNSG3dResDictTexToMatIdxData* pData)
{
    // �o�C���h��Ԃ̏ꍇ�����[�X����
    u8* base = (u8*)pMat + pData->offset;
    u32 j;

    for (j = 0; j < pData->numIdx; ++j)
    {
        // �ematData�Ƀe�N�X�`�������Z�b�g�A�b�v���Ă����B
        NNSG3dResMatData* matData = NNS_G3dGetMatDataByIdx(pMat, *(base + j));

        // flip��repeat��texgen���c�����Z�b�g
        matData->texImageParam &= REG_G3_TEXIMAGE_PARAM_TGEN_MASK |
                                  REG_G3_TEXIMAGE_PARAM_FT_MASK | REG_G3_TEXIMAGE_PARAM_FS_MASK |
                                  REG_G3_TEXIMAGE_PARAM_RT_MASK | REG_G3_TEXIMAGE_PARAM_RS_MASK;
        matData->magH = matData->magW = FX32_ONE;
    }

    // �A���o�C���h��Ԃɂ���
    pData->flag &= ~1;
}


/*---------------------------------------------------------------------------*
    NNS_G3dBindMdlTex

    ���f�����ŁA�e�N�X�`�����Ɋ֘A�t�����Ă���}�e���A���̃e�N�X�`���G���g����
    �e�N�X�`���u���b�N���̓����̃e�N�X�`���������e�N�X�`���f�[�^�Ɋ֘A�t���܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dBindMdlTex(NNSG3dResMdl* pMdl, const NNSG3dResTex* pTex)
{
    NNSG3dResMat*  mat;
    NNSG3dResDict* dictTex;
    u32 i;
    BOOL result = TRUE;

    NNS_G3D_NULL_ASSERT(pMdl);
    NNS_G3D_NULL_ASSERT(pTex);

    mat     = NNS_G3dGetMat(pMdl);
    dictTex = (NNSG3dResDict*)((u8*)mat + mat->ofsDictTexToMatList);
    
    // ���f�����\�[�X�̃e�N�X�`����->�}�e���A���C���f�b�N�X�񎫏�����
    // ���ꂼ��ɑ΂��ă��[�v����
    for (i = 0; i < dictTex->numEntry; ++i)
    {
        const NNSG3dResName* name = NNS_G3dGetResNameByIdx(dictTex, i);

        // �e�N�X�`���u���b�N���̓����G���g��������
        const NNSG3dResDictTexData* texData = NNS_G3dGetTexDataByName(pTex, name);

        if (texData)
        {
            // ���݂��Ă����i�Ԗڂ̃e�N�X�`���Ɋւ���o�C���h�f�[�^���擾
            NNSG3dResDictTexToMatIdxData* data =
                (NNSG3dResDictTexToMatIdxData*) NNS_G3dGetResDataByIdx(dictTex, i);
        
            if (!(data->flag & 1))
            {
                // �o�C���h��ԂłȂ��ꍇ�A�o�C���h����
                bindMdlTex_Internal_(mat, data, pTex, texData);
            }
        }
        else
        {
            result = FALSE;
        }
    }
    return result;
}


/*---------------------------------------------------------------------------*
    NNS_G3dBindMdlTexEx

    ���f�����ŁApResName�Ƃ����e�N�X�`�����Ɋ֘A�t�����Ă���}�e���A����
    �e�N�X�`���G���g�����e�N�X�`���u���b�N���̖��OpResName�����e�N�X�`��
    �Ɋ֘A�t���܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dBindMdlTexEx(NNSG3dResMdl* pMdl,
                    const NNSG3dResTex* pTex,
                    const NNSG3dResName* pResName)
{
    NNSG3dResMat* mat;
    NNSG3dResDict* dictTex;
    const NNSG3dResDictTexData* texData;
    NNSG3dResDictTexToMatIdxData* data;

    NNS_G3D_NULL_ASSERT(pMdl);
    NNS_G3D_NULL_ASSERT(pTex);
    NNS_G3D_NULL_ASSERT(pResName);

    mat     = NNS_G3dGetMat(pMdl);
    dictTex = (NNSG3dResDict*)((u8*)mat + mat->ofsDictTexToMatList);

    // �e�N�X�`���u���b�N���̎���������
    texData = NNS_G3dGetTexDataByName(pTex, pResName);

    if (texData)
    {
        // ���f���f�[�^���̃e�N�X�`��->�}�e���A���C���f�b�N�X��̎���������
        data = (NNSG3dResDictTexToMatIdxData*)
                    NNS_G3dGetResDataByName(dictTex, pResName);

        if (data && !(data->flag & 1))
        {
            // ���f���ƃe�N�X�`���̗�����pResName�ɑΉ�����f�[�^�������āA
            // �o�C���h��ԂłȂ��ꍇ�A�o�C���h����
            bindMdlTex_Internal_(mat, data, pTex, texData);
            return TRUE;
        }
    }
    return FALSE;
}


/*---------------------------------------------------------------------------*
    NNS_G3dForceBindMdlTex

    ���f�����Ńe�N�X�`����->�}�e���A�����X�g������texToMatListIdx�Ԗڂ̃G���g��
    �Ɋi�[����Ă���}�e���A���̃e�N�X�`���G���g���ƃe�N�X�`���u���b�N����
    texIdx�Ԗڂ̃e�N�X�`���ɋ����I�Ɋ֘A�t���܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dForceBindMdlTex(NNSG3dResMdl* pMdl,
                       const NNSG3dResTex* pTex,
                       u32 texToMatListIdx,
                       u32 texIdx)
{
    NNSG3dResMat* mat;
    NNSG3dResDict* dictTex;
    const NNSG3dResDictTexData* texData;
    NNSG3dResDictTexToMatIdxData* data;

    NNS_G3D_NULL_ASSERT(pMdl);
    NNS_G3D_NULL_ASSERT(pTex);

    mat     = NNS_G3dGetMat(pMdl);
    dictTex = (NNSG3dResDict*)((u8*)mat + mat->ofsDictTexToMatList);

    // �e�N�X�`���u���b�N���̃e�N�X�`���f�[�^���C���f�b�N�X�����Ŏ擾
    texData = NNS_G3dGetTexDataByIdx(pTex, texIdx);

    // ���f���f�[�^���̃e�N�X�`��->�}�e���A���C���f�b�N�X��̎���������
    data = (NNSG3dResDictTexToMatIdxData*)
                NNS_G3dGetResDataByIdx(dictTex, texToMatListIdx);

    if (data)
    {
        // pResName�̖��O�����e�N�X�`�����}�e���A���ɑ��݂���΋����I�Ƀo�C���h����B
        bindMdlTex_Internal_(mat, data, pTex, texData);
        return TRUE;
    }
    return FALSE;
}


/*---------------------------------------------------------------------------*
    NNS_G3dReleaseMdlTex

    ���f�����ŁA�e�N�X�`���Ɋ֘A�t�����Ă���}�e���A���̃e�N�X�`���G���g��
    �̊֘A�t�����������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dReleaseMdlTex(NNSG3dResMdl* pMdl)
{
    NNSG3dResMat*  mat;
    NNSG3dResDict* dictTex;
    u32 i;

    NNS_G3D_NULL_ASSERT(pMdl);

    mat      = NNS_G3dGetMat(pMdl);
    dictTex  = (NNSG3dResDict*)((u8*)mat + mat->ofsDictTexToMatList);
    
    // ���f�����\�[�X�̃e�N�X�`����->�}�e���A���C���f�b�N�X�񎫏�����
    // ���ꂼ��ɑ΂��ă��[�v����
    for (i = 0; i < dictTex->numEntry; ++i)
    {
        NNSG3dResDictTexToMatIdxData* data =
            (NNSG3dResDictTexToMatIdxData*) NNS_G3dGetResDataByIdx(dictTex, i);

        if (data->flag & 1)
        {
            // �o�C���h����Ă����ԂȂ�΃����[�X
            releaseMdlTex_Internal_(mat, data);
        }
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dReleaseMdlTexEx

    ���f�����ŁA���OpResName�Ƃ������O�����e�N�X�`���Ɋ֘A�t�����Ă���
    �}�e���A���̃e�N�X�`���G���g���̊֘A�t�����������܂��B
 *---------------------------------------------------------------------------*/
BOOL NNS_G3dReleaseMdlTexEx(NNSG3dResMdl* pMdl, const NNSG3dResName* pResName)
{
    NNSG3dResMat*  mat;
    NNSG3dResDict* dictTex;
    NNSG3dResDictTexToMatIdxData* data;

    NNS_G3D_NULL_ASSERT(pMdl);
    NNS_G3D_NULL_ASSERT(pResName);

    mat      = NNS_G3dGetMat(pMdl);
    dictTex  = (NNSG3dResDict*)((u8*)mat + mat->ofsDictTexToMatList);
    data     = (NNSG3dResDictTexToMatIdxData*)NNS_G3dGetResDataByName(dictTex, pResName);

    if (data && (data->flag & 1))
    {
        // �o�C���h����Ă����ԂȂ�΃����[�X
        releaseMdlTex_Internal_(mat, data);
        return TRUE;
    }
    return FALSE;
}


static void
bindMdlPltt_Internal_(NNSG3dResMat* pMat,
                      NNSG3dResDictPlttToMatIdxData* pBindData,
                      const NNSG3dResTex* pTex,
                      const NNSG3dResDictPlttData* pPlttData)
{
    // �o�C���h��ԂłȂ��ꍇ�A�o�C���h����
    u8* base = (u8*)pMat + pBindData->offset;
    u16 plttBase = pPlttData->offset;
    u16 vramOffset = (u16)(NNS_GfdGetTexKeyAddr(pTex->plttInfo.vramKey) >> NNS_GFD_TEXKEY_ADDR_SHIFT);
    u32 j;

    NNS_G3D_ASSERTMSG((pTex->plttInfo.vramKey != 0 || pTex->plttInfo.sizePltt == 0),
                       "No palette key assigned");
    
    // 4colors�Ȃ�r�b�g�������Ă���
    if (!(pPlttData->flag & 1))
    {
        // 4colors�ȊO�̏ꍇ��4bit�V�t�g�ɂȂ�B
        // 4colors�̏ꍇ��3bit�V�t�g�Ȃ̂ł��̂܂�
        plttBase >>= 1;
        vramOffset >>= 1;
    }

    for (j = 0; j < pBindData->numIdx; ++j)
    {
        // �ematData�Ƀp���b�g�����Z�b�g�A�b�v���Ă����B
        NNSG3dResMatData* matData = NNS_G3dGetMatDataByIdx(pMat, *(base + j));
        matData->texPlttBase = (u16)(plttBase + vramOffset);
    }

    // �o�C���h��Ԃɂ���
    pBindData->flag |= 1;
}


/*---------------------------------------------------------------------------*
    NNS_G3dBindMdlPltt

    ���f�����ŁA�p���b�g���Ɋ֘A�t�����Ă���}�e���A���̃p���b�g�G���g����
    �e�N�X�`���u���b�N���̓����̃p���b�g�������p���b�g�f�[�^�Ɋ֘A�t���܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dBindMdlPltt(NNSG3dResMdl* pMdl, const NNSG3dResTex* pTex)
{
    NNSG3dResMat*  mat;
    NNSG3dResDict* dictPltt;
    u32 i;
    BOOL result = TRUE;

    NNS_G3D_NULL_ASSERT(pMdl);
    NNS_G3D_NULL_ASSERT(pTex);

    mat      = NNS_G3dGetMat(pMdl);
    dictPltt = (NNSG3dResDict*)((u8*)mat + mat->ofsDictPlttToMatList);

    for (i = 0; i < dictPltt->numEntry; ++i)
    {
        // ���f�����̃p���b�g��->�}�e���A���C���f�b�N�X�񎫏�����p���b�g�����Ƃ�
        const NNSG3dResName* name = NNS_G3dGetResNameByIdx(dictPltt, i);
        
        // �e�N�X�`���u���b�N����p���b�g���ɑΉ�����f�[�^�t�B�[���h�𓾂�
        const NNSG3dResDictPlttData* plttData = NNS_G3dGetPlttDataByName(pTex, name);

        if (plttData)
        {
            // �e�N�X�`���u���b�N�Ƀp���b�g��������
            NNSG3dResDictPlttToMatIdxData* data;
            data = (NNSG3dResDictPlttToMatIdxData*) NNS_G3dGetResDataByIdx(dictPltt, i);

            if (!(data->flag & 1))
            {
                bindMdlPltt_Internal_(mat, data, pTex, plttData);
            }
        }
        else
        {
            result = FALSE;
        }
    }
    return result;
}


/*---------------------------------------------------------------------------*
    NNS_G3dBindMdlPlttEx

    ���f�����ŁApResName�Ƃ����p���b�g���Ɋ֘A�t�����Ă���}�e���A����
    �p���b�g�G���g�����e�N�X�`���u���b�N���̖��OpResName�����p���b�g
    �Ɋ֘A�t���܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dBindMdlPlttEx(NNSG3dResMdl* pMdl,
                     const NNSG3dResTex* pTex,
                     const NNSG3dResName* pResName)
{
    NNSG3dResMat*  mat;
    NNSG3dResDict* dictPltt;
    const NNSG3dResDictPlttData* plttData;
    NNSG3dResDictPlttToMatIdxData* data;

    NNS_G3D_NULL_ASSERT(pMdl);
    NNS_G3D_NULL_ASSERT(pTex);
    NNS_G3D_NULL_ASSERT(pResName);

    mat      = NNS_G3dGetMat(pMdl);
    dictPltt = (NNSG3dResDict*)((u8*)mat + mat->ofsDictPlttToMatList);

    // �e�N�X�`���u���b�N����p���b�g���ɑΉ�����f�[�^�t�B�[���h�𓾂�
    plttData = NNS_G3dGetPlttDataByName(pTex, pResName);

    if (plttData)
    {
        // �e�N�X�`���u���b�N�Ƀp���b�g��������
        data = (NNSG3dResDictPlttToMatIdxData*)
                    NNS_G3dGetResDataByName(dictPltt, pResName);
        
        if (data && !(data->flag & 1))
        {
            bindMdlPltt_Internal_(mat, data, pTex, plttData);
            return TRUE;
        }
    }
    return FALSE;
}


/*---------------------------------------------------------------------------*
    NNS_G3dForceBindMdlPltt

    ���f�����Ńp���b�g��->�}�e���A�����X�g������plttToMatListIdx�Ԗڂ̃G���g��
    �Ɋi�[����Ă���}�e���A���̃p���b�g�G���g���ƃe�N�X�`���u���b�N����
    plttIdx�Ԗڂ̃p���b�g�ɋ����I�Ɋ֘A�t���܂��B
 *---------------------------------------------------------------------------*/
BOOL NNS_G3dForceBindMdlPltt(NNSG3dResMdl* pMdl,
                             const NNSG3dResTex* pTex,
                             u32 plttToMatListIdx,
                             u32 plttIdx)
{
    NNSG3dResMat*  mat;
    NNSG3dResDict* dictPltt;
    const NNSG3dResDictPlttData* plttData;
    NNSG3dResDictPlttToMatIdxData* data;

    NNS_G3D_NULL_ASSERT(pMdl);
    NNS_G3D_NULL_ASSERT(pTex);

    mat      = NNS_G3dGetMat(pMdl);
    dictPltt = (NNSG3dResDict*)((u8*)mat + mat->ofsDictPlttToMatList);
    plttData = NNS_G3dGetPlttDataByIdx(pTex, plttIdx);

    data = (NNSG3dResDictPlttToMatIdxData*)
                NNS_G3dGetResDataByIdx(dictPltt, plttToMatListIdx);
        
    if (data && !(data->flag & 1))
    {
        bindMdlPltt_Internal_(mat, data, pTex, plttData);
        return TRUE;
    }
    return FALSE;
}



/*---------------------------------------------------------------------------*
    NNS_G3dReleaseMdlPltt

    ���f�����ŁA�p���b�g�Ɋ֘A�t�����Ă���}�e���A���̃p���b�g�G���g��
    �̊֘A�t�����������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dReleaseMdlPltt(NNSG3dResMdl* pMdl)
{
    NNSG3dResMat*  mat;
    NNSG3dResDict* dictPltt;
    u32 i;

    NNS_G3D_NULL_ASSERT(pMdl);

    mat      = NNS_G3dGetMat(pMdl);
    dictPltt = (NNSG3dResDict*)((u8*)mat + mat->ofsDictPlttToMatList);
    for (i = 0; i < dictPltt->numEntry; ++i)
    {
        NNSG3dResDictPlttToMatIdxData* data =
            (NNSG3dResDictPlttToMatIdxData*) NNS_G3dGetResDataByIdx(dictPltt, i);
         
        if (data->flag & 1)
        {
            // �o�C���h��Ԃ̏ꍇ�����[�X����
#if 0
            u32 j;
            // �t���O���삾���łn�j�Ȃ̂Ŏ��s���Ȃ�
            u8* base = (u8*)mat + data->offset;

            for (j = 0; j < data->numIdx; ++j)
            {
                // �ematData�����Z�b�g����.
                NNSG3dResMatData* matData = NNS_G3dGetMatDataByIdx(mat, *(base + j));
                matData->texPlttBase = 0;
            }
#endif
            // �A���o�C���h�ɂ���B
            data->flag &= ~1;
        }
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dReleaseMdlPlttEx

    ���f�����ŁA���OpResName�Ƃ������O�����p���b�g�Ɋ֘A�t�����Ă���
    �}�e���A���̃p���b�g�G���g���̊֘A�t�����������܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dReleaseMdlPlttEx(NNSG3dResMdl* pMdl, const NNSG3dResName* pResName)
{
    NNSG3dResMat*  mat;
    NNSG3dResDict* dictPltt;
    NNSG3dResDictPlttToMatIdxData* data;

    NNS_G3D_NULL_ASSERT(pMdl);
    NNS_G3D_NULL_ASSERT(pResName);

    mat      = NNS_G3dGetMat(pMdl);
    dictPltt = (NNSG3dResDict*)((u8*)mat + mat->ofsDictPlttToMatList);
    data = (NNSG3dResDictPlttToMatIdxData*)
                NNS_G3dGetResDataByName(dictPltt, pResName);

    if (data && (data->flag & 1))
    {
        // �o�C���h��Ԃ̏ꍇ�����[�X����
        data->flag &= ~1;
        return TRUE;
    }
    return FALSE;
}


/*---------------------------------------------------------------------------*
    NNS_G3dBindMdlSet

    �e���f�����e�N�X�`���^�p���b�g�Ɋ֘A�t���܂��B
    �e�N�X�`����TexKey, PlttKey���Z�b�g����Ă���K�v������܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dBindMdlSet(NNSG3dResMdlSet* pMdlSet, const NNSG3dResTex* pTex)
{
    u32 i;
    BOOL result = TRUE;

    NNS_G3D_NULL_ASSERT(pMdlSet);
    NNS_G3D_NULL_ASSERT(pTex);

    for (i = 0; i < pMdlSet->dict.numEntry; ++i)
    {
        NNSG3dResMdl* mdl = NNS_G3dGetMdlByIdx(pMdlSet, i);
        NNS_G3D_NULL_ASSERT(mdl);

        result &= NNS_G3dBindMdlTex(mdl, pTex);
        result &= NNS_G3dBindMdlPltt(mdl, pTex);
    }
    return result;
}


/*---------------------------------------------------------------------------*
    NNS_G3dReleaseMdlSet

    �e���f������e�N�X�`���^�p���b�g�ւ̊֘A�t�����O���܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dReleaseMdlSet(NNSG3dResMdlSet* pMdlSet)
{
    u32 i;
    NNS_G3D_NULL_ASSERT(pMdlSet);

    for (i = 0; i < pMdlSet->dict.numEntry; ++i)
    {
        NNSG3dResMdl* mdl = NNS_G3dGetMdlByIdx(pMdlSet, i);
        NNS_G3D_NULL_ASSERT(mdl);

        NNS_G3dReleaseMdlTex(mdl);
        NNS_G3dReleaseMdlPltt(mdl);
    }
}

/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d
  File:     util.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.26 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/util.h>
#include <nnsys/g3d/gecom.h>
#include <nnsys/g3d/binres/res_struct_accessor.h>
#include <nnsys/g3d/anm.h>
#include <nnsys/g3d/glbstate.h>


/*---------------------------------------------------------------------------*
    NNS_G3dGetCurrentMtx

    �J�����g�ʒu���W�s��ƃJ�����g�����x�N�g���s����擾���܂��B
    ������NULL�̏ꍇ�͂��̍s��̎擾���ȗ����܂��B
    ���s���Ɏˉe�s��̍s��X�^�b�N��j�󂵂܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGetCurrentMtx(MtxFx43* m, MtxFx33* n)
{
    NNS_G3dGeFlushBuffer();

    // Projection�s���Identity�ɂ���
    // �N���b�v�s��ƃx�N�^�s������o���ƁA
    // �J�����g�ʒu���W�s��ƃJ�����g�����x�N�g���s������o����B
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_PushMtx();
    G3_Identity();

    if (m)
    {
        MtxFx44 tmp;
        while(G3X_GetClipMtx(&tmp))
            ;
        MTX_Copy44To43(&tmp, m);
    }

    if (n)
    {
        while(G3X_GetVectorMtx(n))
            ;
    }

    // ����Projection�s��𕜋A���APosition/Vector�s�񃂁[�h�ɂ���
    G3_PopMtx(1);
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGetResultMtx

    NNS_G3dDraw�̎��s���nodeID�ɑΉ�����s�񂪍s��X�^�b�N�Ɏc���Ă���ꍇ�́A
    ���̍s����s��X�^�b�N������o���܂��B
    �s�񂪎��o�����ꍇ�ɂ�TRUE, ���o���Ȃ������ꍇ�ɂ�FALSE���Ԃ�܂��B
    TRUE�̏ꍇ�A�s�񃂁[�h��Position/Vector�s�񃂁[�h�ɕύX����Ă��܂��B
    pos, nrm��NULL�łȂ��ꍇ�ɃZ�b�g����܂��B
    �Ȃ��A������s��ɂ�pos_scale�͂������Ă��Ȃ����Ƃɂ����ӂ��������B
    (.imd���ԃt�@�C����<model_info>�Ŏw�肳��鑮���ŁA���_���W���X�P�[�����O
     ���Ă���BNNSG3dResMdlInfo::posScale��fx32�^�Ŋi�[����Ă���B)

    �g�p�@�ƒ���:
    g3cvtr��-s�I�v�V���������ăR���o�[�g�����ꍇ�A�W���C���g�s��͑S��
    �s��X�^�b�N�ɃX�g�A����Ă��܂��B
    �܂��ANNSG3dRenderObj::flag��NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_DRAW��ݒ肵��
    NNS_G3dDraw���R�[�������ꍇ�A�`����X�L�b�v���邱�Ƃ��ł��A
    NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_MTXCALC��ݒ肵��NNS_G3dDraw���R�[������
    �ꍇ�A�s��v�Z���X�L�b�v���邱�Ƃ��ł��܂��B

    ���̋@�\�𗘗p����ƁA
    �E�ŏ��ɍs����s��X�^�b�N�ɃZ�b�g���Ă���
    �E�s��X�^�b�N�ɃZ�b�g���ꂽ�s������H
    �E���H���ꂽ�s���p���ĕ`��
    �Ƃ������v���O���~���O���\�ɂȂ�܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dGetResultMtx(const NNSG3dRenderObj* pRenderObj,
                    MtxFx43* pos,
                    MtxFx33* nrm,
                    u32 nodeID)
{
    const NNSG3dResNodeData* nd;
    u32 stackID;

    // pos, nrm��NULL�ł�OK
    NNS_G3D_NULL_ASSERT(pRenderObj);

    nd = NNS_G3dGetNodeDataByIdx(&pRenderObj->resMdl->nodeInfo, nodeID);
    stackID = (nd->flag & (u32)NNS_G3D_SRTFLAG_IDXMTXSTACK_MASK)
                                >> NNS_G3D_SRTFLAG_IDXMTXSTACK_SHIFT;

    if (stackID != 31)
    {
        NNS_G3dGeRestoreMtx((int)stackID);
        if (pos || nrm)
        {
            NNS_G3dGetCurrentMtx(pos, nrm);
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dSetResultMtx

    NNS_G3dDraw�̎��s���nodeID�ɑΉ�����s�񂪍s��X�^�b�N�Ɏc��悤�Ȑݒ��
    �Ȃ��Ă���ꍇ�́A�s��*pos, *nrm���X�^�b�N�̊Y���ʒu�ɑ�����܂��B
    ����ł����ꍇ��TRUE,�ł��Ȃ������ꍇ��FALSE���Ԃ�܂��B
    TRUE�̏ꍇ�A�s�񃂁[�h��Position/Vector�s�񃂁[�h�ɕύX����Ă��܂��B
    nrm��NULL�ł����܂��܂��񂪁Apos��NULL�ł����Ă͂����܂���B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dSetResultMtx(const NNSG3dRenderObj* pRenderObj,
                    const MtxFx43* pos,
                    const MtxFx33* nrm,
                    u32 nodeID)
{
    const NNSG3dResNodeData* nd;
    u32 stackID;

    NNS_G3D_NULL_ASSERT(pRenderObj);
    NNS_G3D_NULL_ASSERT(pos); // nrm��NULL�ł��悢

    nd = NNS_G3dGetNodeDataByIdx(&pRenderObj->resMdl->nodeInfo, nodeID);
    stackID = (nd->flag & (u32)NNS_G3D_SRTFLAG_IDXMTXSTACK_MASK)
                                >> NNS_G3D_SRTFLAG_IDXMTXSTACK_SHIFT;

    if (stackID != 31)
    {
        // �o�b�t�@���t���b�V��
        NNS_G3dGeFlushBuffer();

        // �ȍ~�̓C�~�f�B�G�C�g�ő��M�\.
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        if (nrm)
        {
            // ���[�h��4x3�s�񂵂��ł��Ȃ�
            reg_G3X_GXFIFO = G3OP_MTX_LOAD_4x3;
            MI_CpuSend32(&nrm->_00, &reg_G3X_GXFIFO, sizeof(*nrm));
            reg_G3X_GXFIFO = 0;
            reg_G3X_GXFIFO = 0;
            reg_G3X_GXFIFO = 0;

            G3_MtxMode(GX_MTXMODE_POSITION);
            G3_LoadMtx43(pos);
            G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
        }
        else
        {
            G3_LoadMtx43(pos);
        }

        G3_StoreMtx((int)stackID);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dInit

    �f�t�H���g�̃C�j�V�����C�Y���s���܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dInit(void)
{
    G3X_Init();

    //
    // G3D�����ŕێ����Ă���O���[�o�����(�J�����s��E�ˉe�s��)�̃C�j�V�����C�Y
    //
    NNS_G3dGlbInit();

    //
    // GX_FIFOINTR_COND_DISABLE�łȂ���΂悢���A
    // GX_FIFOINTR_COND_UNDERHALF���GX_FIFOINTR_COND_EMPTY�̂ق����悢�悤��
    //
    G3X_SetFifoIntrCond(GX_FIFOINTR_COND_EMPTY);
}


/*---------------------------------------------------------------------------*
    NNS_G3dSbcCmdLen

    SBC�R�}���h�̒���(�o�C�g��)��\���e�[�u���ł��B
    -1�̏ꍇ�͑Ή�����SBC�R�}���h�͑��݂����A0�̏ꍇ�̓R�}���h�͉ϒ��ł��B
 *---------------------------------------------------------------------------*/
const s8 NNS_G3dSbcCmdLen[256] = 
{
    // -1�̏ꍇ�͑��݂���
    // 0�̏ꍇ�͕s�蒷
     1,  1,  3,  2,  2,  2,  4,  2,  2,  0,  9,  1,  3,  3, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1,  2, -1,  5,  3,  3,  0, -1,  1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1,  2, -1,  5,  3,  3, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1,  6,  4,  4, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};


/*---------------------------------------------------------------------------*
    NNS_G3dGetSbcCmdLen

    SBC�R�}���h*c�̒�����Ԃ��܂��B
    *c��SBC�R�}���h�łȂ��ꍇ��-1��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
int
NNS_G3dGetSbcCmdLen(const u8* c)
{
    int cmdLen;
    NNS_G3D_NULL_ASSERT(c);

    cmdLen = NNS_G3dSbcCmdLen[*c];

    if (cmdLen < 0)
    {
        return -1;
    }
    else if (cmdLen == 0)
    {
        if (*c == NNS_G3D_SBC_NODEMIX)
        {
            return *(c + 2) * 3 + 3;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return cmdLen;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dSearchSbcCmd

    SBC�R�}���h(cmd)��SBC�R�[�h��(c)�̒����猩���܂��B
    ���������ꍇ��SBC�R�}���h���̃|�C���^��Ԃ��܂��B
    ������Ȃ������ꍇ��NULL��Ԃ��܂��B
    SBC�R�}���h���̃t���O���͖�������܂��B
 *---------------------------------------------------------------------------*/
const u8*
NNS_G3dSearchSbcCmd(const u8* c, u8 cmd)
{
    int x;
    NNS_G3D_NULL_ASSERT(c);
    NNS_G3D_ASSERTMSG(NNS_G3dSbcCmdLen[cmd] >= 0, "unknown command");

    cmd &= NNS_G3D_SBCCMD_MASK;
    while((x = (*c & NNS_G3D_SBCCMD_MASK)) != NNS_G3D_SBC_RET)
    {
        if (cmd == x)
        {
            return c;
        }
        else
        {
            int cmdLen = NNS_G3dGetSbcCmdLen(c);
            NNS_G3D_ASSERTMSG(cmdLen > 0, "unknown command");
            c += cmdLen;
        }
    }
    return NULL;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGetParentNodeID

    nodeID�̐e�ɂȂ�m�[�h��ID��*parentID�Ɋi�[���܂��B
    �Ԃ�l�́AnodeID��ݒ肵�Ă���NODEDESC���߂ւ̃|�C���^�ł��B
    ������Ȃ������ꍇ��NULL��Ԃ��A*parentID���ύX����܂���B
 *---------------------------------------------------------------------------*/
const u8*
NNS_G3dGetParentNodeID(int* parentID, const u8* sbc, u32 nodeID)
{
    const u8* tmp = sbc;

    NNS_G3D_NULL_ASSERT(parentID);
    NNS_G3D_NULL_ASSERT(sbc);
    NNS_G3D_ASSERTMSG(nodeID < NNS_G3D_SIZE_JNT_MAX,
                      "specified nodeID is illegal");

    while((tmp = NNS_G3dSearchSbcCmd(tmp, NNS_G3D_SBC_NODEDESC)) != 0)
    {
        // ���g�̃m�[�hID�Ƃ̈�v����
        if (*(tmp + 1) == nodeID)
        {
            // �e���擾
            // �q�ɑ΂��Đe����������悤�ȕs����SBC�ɂ͑Ή����Ă��Ȃ�
            *parentID = *(tmp + 2);
            return tmp;
        }

        {
            int cmdLen = NNS_G3dGetSbcCmdLen(tmp);
            NNS_G3D_ASSERTMSG(cmdLen > 0, "unknown command");
            tmp += cmdLen;
        }
    }
    return NULL;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGetChildNodeIDList

    nodeID�̎q�ɂȂ�m�[�hID�̃��X�g��idList�ɍ쐬���܂��B
    idList�͏\���ȗ̈悪�m�ۂ���Ă���K�v������܂��B
    �Ԃ�l�͎q�m�[�h�̐��ł��B
    �݂���Ȃ������ꍇ��0��Ԃ��AidList�ɂ͉����������܂�܂���B
 *---------------------------------------------------------------------------*/
int
NNS_G3dGetChildNodeIDList(u8* idList, const u8* sbc, u32 nodeID)
{
    const u8* tmp = sbc;
    int num = 0;

    NNS_G3D_NULL_ASSERT(idList);
    NNS_G3D_ASSERTMSG(nodeID < NNS_G3D_SIZE_JNT_MAX,
                      "specified nodeID is illegal");

    while((tmp = NNS_G3dSearchSbcCmd(tmp, NNS_G3D_SBC_NODEDESC)) != 0)
    {
        // �e�m�[�hID�̈�v����
        if (*(tmp + 2) == nodeID && *(tmp + 2) != *(tmp + 1))
        {
            // �ǉ�
            *(idList + num++) = *(tmp + 1);
        }

        {
            int cmdLen = NNS_G3dGetSbcCmdLen(tmp);
            NNS_G3D_ASSERTMSG(cmdLen > 0, "unknown command");
            tmp += cmdLen;
        }
    }

    return num;
}


/*---------------------------------------------------------------------------*
    NNS_G3dResDefaultSetup

    G3D�̃��\�[�X*pData���Z�b�g�A�b�v���܂��B�f�[�^�t�H�[�}�b�g�ɂ��
    ���삪���ƂȂ�܂��B

    .nsbmd�t�@�C���̏ꍇ
        �E�e�N�X�`���ƃp���b�g�p�̗̈���m��
        �E�e�N�X�`���ƃp���b�g��VRAM�Ƀ��[�h
        �E�t�@�C�����̊e���f���Ƀe�N�X�`�����o�C���h

    .nsbtx�t�@�C���̏ꍇ
        �E�e�N�X�`���ƃp���b�g�p�̗̈���m��
        �E�e�N�X�`���ƃp���b�g��VRAM�Ƀ��[�h

    ���̑�
        �Ȃɂ����܂���B

    �e�N�X�`���ƃp���b�g��VRAM�Ƀ��[�h����ۂ�DMA�]�����g�p���邽�߁A
    ���̊֐����ĂԑO�Ƀe�N�X�`���ƃp���b�g�̃f�[�^�L���b�V����
    ���C���������Ƀ��C�g�o�b�N���Ă����K�v������܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dResDefaultSetup(void* pResData)
{
    u8* binFile = (u8*)pResData;
    BOOL failed = FALSE;

    NNS_G3D_NULL_ASSERT(pResData);

    switch(*(u32*)&binFile[0])
    {
    case NNS_G3D_SIGNATURE_NSBTX: // BTX0
    case NNS_G3D_SIGNATURE_NSBMD: // BMD0
        {
            NNSG3dResTex* tex;
            u32 szTex, szTex4x4, szPltt;
            BOOL sucTex    = TRUE;
            BOOL sucTex4x4 = TRUE;
            BOOL sucPltt   = TRUE;
            NNSG3dTexKey keyTex;
            NNSG3dTexKey keyTex4x4;
            NNSG3dPlttKey keyPltt;

            tex = NNS_G3dGetTex((NNSG3dResFileHeader*) pResData);
            if (tex)
            {
                // �K�v�ȃT�C�Y���擾
                szTex    = NNS_G3dTexGetRequiredSize(tex);
                szTex4x4 = NNS_G3dTex4x4GetRequiredSize(tex);
                szPltt   = NNS_G3dPlttGetRequiredSize(tex);

                if (szTex > 0)
                {
                    // ���݂���΃e�N�X�`���C���[�W�X���b�g�Ɋm��
                    keyTex = NNS_GfdAllocTexVram(szTex, FALSE, 0);
                    if (keyTex == NNS_GFD_ALLOC_ERROR_TEXKEY)
                    {
                        sucTex = FALSE;
                    }
                }
                else
                {
                    keyTex = 0;
                }

                if (szTex4x4 > 0)
                {
                    // ���݂���΃e�N�X�`���C���[�W�X���b�g�Ɋm��
                    keyTex4x4 = NNS_GfdAllocTexVram(szTex4x4, TRUE, 0);
                    if (keyTex4x4 == NNS_GFD_ALLOC_ERROR_TEXKEY)
                    {
                        sucTex4x4 = FALSE;
                    }
                }
                else
                {
                    keyTex4x4 = 0;
                }

                if (szPltt > 0)
                {
                    // ���݂���΃e�N�X�`���p���b�g�X���b�g�Ɋm��
                    keyPltt = 
                        NNS_GfdAllocPlttVram(szPltt,
                                            tex->tex4x4Info.flag & NNS_G3D_RESPLTT_USEPLTT4,
                                            0);
                    if (keyPltt == NNS_GFD_ALLOC_ERROR_PLTTKEY)
                    {
                        sucPltt = FALSE;
                    }
                }
                else
                {
                    keyPltt = 0;
                }

                if (!sucTex || !sucTex4x4 || !sucPltt)
                {
                    // ���s���̃��[���o�b�N����
                    int status;

                    if (sucPltt)
                    {
                        status = NNS_GfdFreePlttVram(keyPltt);
                        NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreePlttVram failed");
                    }

                    if (sucTex4x4)
                    {
                        status = NNS_GfdFreeTexVram(keyTex4x4);
                        NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreeTexVram failed");
                    }

                    if (sucTex)
                    {
                        status = NNS_GfdFreeTexVram(keyTex);
                        NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreeTexVram failed");
                    }

                    return FALSE;
                }

                // �L�[�̃A�T�C��
                NNS_G3dTexSetTexKey(tex, keyTex, keyTex4x4);
                NNS_G3dPlttSetPlttKey(tex, keyPltt);

                // VRAM�ւ̃��[�h
                NNS_G3dTexLoad(tex, TRUE);
                NNS_G3dPlttLoad(tex, TRUE);
            }

            if (*(u32*)&binFile[0] == NNS_G3D_SIGNATURE_NSBMD)
            {
                // NSBMD�̏ꍇ�͑S�Ẵ��f�����o�C���h����
                NNSG3dResMdlSet* mdlSet = NNS_G3dGetMdlSet((NNSG3dResFileHeader*) pResData);
                NNS_G3D_NULL_ASSERT(mdlSet);

                if (tex)
                {
                    // ���f���Z�b�g�̃o�C���h
                    (void)NNS_G3dBindMdlSet(mdlSet, tex);
                }
            }
        }
        return TRUE;
        break;
    case NNS_G3D_SIGNATURE_NSBCA: // BCA0
    case NNS_G3D_SIGNATURE_NSBVA: // BVA0
    case NNS_G3D_SIGNATURE_NSBMA: // BMA0
    case NNS_G3D_SIGNATURE_NSBTP: // BTP0
    case NNS_G3D_SIGNATURE_NSBTA: // BTA0
        return TRUE;
        break;
    default:
        NNS_G3D_ASSERTMSG(1==0, "unknown file signature: '%c%c%c%c' found.\n",
                                binFile[0], binFile[1], binFile[2], binFile[3]);
        return FALSE;
        break;
    };
}


/*---------------------------------------------------------------------------*
    NNS_G3dResDefaultRelease

    G3D�̃��\�[�X*pData�̗̈���J������O�ɒʏ�s����������s���܂��B
    �f�[�^�t�H�[�}�b�g�ɂ�蓮�삪���ƂȂ�܂��B

    .nsbmd�t�@�C���̏ꍇ
        �E�e�N�X�`���ƃp���b�g�p�̗̈���J��

    .nsbtx�t�@�C���̏ꍇ
        �E�e�N�X�`���ƃp���b�g�p�̗̈���J��

    ���̑�
        �Ȃɂ����܂���B

    NOTICE:
    pResData�̃������̈悻�̂��̂̉���̓��[�U�[���s���K�v������܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dResDefaultRelease(void* pResData)
{
    u8* binFile = (u8*)pResData;
    BOOL failed = FALSE;

    NNS_G3D_NULL_ASSERT(pResData);

    switch(*(u32*)&binFile[0])
    {
    case NNS_G3D_SIGNATURE_NSBMD: // BMD0
        {
            NNSG3dResTex* tex;
            NNSG3dResMdlSet* mdlSet = NNS_G3dGetMdlSet(pResData);
            NNS_G3D_NULL_ASSERT(mdlSet);
            tex = NNS_G3dGetTex((NNSG3dResFileHeader*) pResData);

            if (tex)
            {
                // �S�Ẵ��f���������[�X
                NNS_G3dReleaseMdlSet(mdlSet);
            }
        }
        // don't break
    case NNS_G3D_SIGNATURE_NSBTX: // BTX0
        {
            NNSG3dResTex* tex;
            NNSG3dPlttKey plttKey;
            NNSG3dTexKey  texKey, tex4x4Key;
            int status;
            tex = NNS_G3dGetTex((NNSG3dResFileHeader*) pResData);

            if (tex)
            {
                // �L�[���e�N�X�`���u���b�N����؂藣��
                plttKey   = NNS_G3dPlttReleasePlttKey(tex);
                NNS_G3dTexReleaseTexKey(tex, &texKey, &tex4x4Key);

                if (plttKey > 0)
                {
                    // �e�N�X�`���p���b�g�X���b�g���Ɋm�ۂ���Ă���Ή��
                    status = NNS_GfdFreePlttVram(plttKey);
                    NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreePlttVram failed");
                }

                if (tex4x4Key > 0)
                {
                    // �e�N�X�`���C���[�W�X���b�g���Ɋm�ۂ���Ă���Ή��
                    status = NNS_GfdFreeTexVram(tex4x4Key);
                    NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreeTexVram failed");
                }

                if (texKey > 0)
                {
                    // �e�N�X�`���C���[�W�X���b�g���Ɋm�ۂ���Ă���Ή��
                    status = NNS_GfdFreeTexVram(texKey);
                    NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreeTexVram failed");
                }
            }
        }
        break;
    case NNS_G3D_SIGNATURE_NSBCA: // BCA0
    case NNS_G3D_SIGNATURE_NSBVA: // BVA0
    case NNS_G3D_SIGNATURE_NSBMA: // BMA0
    case NNS_G3D_SIGNATURE_NSBTP: // BTP0
    case NNS_G3D_SIGNATURE_NSBTA: // BTA0
        break;
    default:
        NNS_G3D_ASSERTMSG(1==0, "unknown file signature: '%c%c%c%c' found.\n",
                                binFile[0], binFile[1], binFile[2], binFile[3]);
        break;
    };
}


/*---------------------------------------------------------------------------*
    NNS_G3dLocalOriginToScrPos

    ���[�J�����W�n�̌��_�̃X�N���[����̈ʒu�����߂�B
    �J�����g�ʒu���W�s��ƃJ�����g�ˉe�s�񂪓K�؂ɐݒ肳��Ă���K�v������܂��B

    �Ԃ�l��0�Ȃ��*px, *py�ɃX�N���[����̍��W���i�[����Ă��܂��B
    �Ԃ�l��-1�Ȃ�Ή�ʊO�ł����A*px, *py�ɕ����̖ڈ��ƂȂ���W���i�[����Ă��܂��B
 *---------------------------------------------------------------------------*/
int
NNS_G3dLocalOriginToScrPos(int* px, int* py)
{
    // �J�����g�ʒu���W�s��ƃJ�����g�ˉe�ϊ��s��ŁA
    // ���[�J�����W�n����N���b�v���W�n�ւ̕ϊ����ł���悤��
    // �ݒ肵�Ă���Ăяo���B
    
    VecFx32 vec;
    fx32 w;
    fx64c invW;
    int x1, y1, x2, y2;
    int dx, dy;
    int rval;

    NNS_G3D_NULL_ASSERT(px);
    NNS_G3D_NULL_ASSERT(py);

    // ���[�J�����W(0, 0, 0)�ɑ΂���N���b�v���W�����߂�B
    NNS_G3dGePositionTest(0, 0, 0);
    NNS_G3dGeFlushBuffer();
    while(G3X_GetPositionTestResult(&vec, &w))
        ;

    invW = FX_InvFx64c(w);

    // ���K���X�N���[�����W�n�ɕϊ�
    vec.x = (FX_Mul32x64c(vec.x, invW) + FX32_ONE) / 2;
    vec.y = (FX_Mul32x64c(vec.y, invW) + FX32_ONE) / 2;

    // ���[�J�����W�̌��_�����K���X�N���[�����W�n�ɕϊ�����Ă���
    if (vec.x < 0 || vec.y < 0 || vec.x > FX32_ONE || vec.y > FX32_ONE)
    {
        // ��ʊO�̏ꍇ
        rval = -1;
    }
    else
    {
        rval = 0;
    }

    NNS_G3dGlbGetViewPort(&x1, &y1, &x2, &y2);
    dx = x2 - x1;
    dy = y2 - y1;

    // �X��BG�X�N���[�����W�n�ɕϊ�
    // BG�̐����ړ����������Ă���ꍇ�́A�X�Ɍ��ʂ�␳����K�v������B
    *px = x1 + ((vec.x * dx + FX32_HALF) >> FX32_SHIFT);
    *py = 191 - y1 - ((vec.y * dy + FX32_HALF) >> FX32_SHIFT);

    
    return rval;
}


/*---------------------------------------------------------------------------*
    NNS_G3dWorldPosToScrPos

    ���[���h���W����X�N���[����̈ʒu�����߂�B
    NNS_G3dGlb�\���̂ɃJ�����s��Ǝˉe�s�񂪐ݒ肳��Ă���K�v������܂��B

    �Ԃ�l��0�Ȃ��*px, *py�ɃX�N���[����̍��W���i�[����Ă��܂��B
    �Ԃ�l��-1�Ȃ�Ή�ʊO�ł����A*px, *py�ɕ����̖ڈ��ƂȂ���W���i�[����Ă��܂��B
 *---------------------------------------------------------------------------*/
int
NNS_G3dWorldPosToScrPos(const VecFx32* pWorld, int* px, int* py)
{
    // ���͂̃��[���h���W�Ƀr���[�C���O�ϊ���
    // �ˉe�ϊ���������

    const MtxFx44* proj;
    const MtxFx43* camera;
    VecFx32 tmp;
    VecFx32 vec;
    fx32 w;
    fx64c invW;
    int x1, y1, x2, y2;
    int dx, dy;
    int rval;

    NNS_G3D_NULL_ASSERT(pWorld);
    NNS_G3D_NULL_ASSERT(px);
    NNS_G3D_NULL_ASSERT(py);

    proj = NNS_G3dGlbGetProjectionMtx();
    camera = NNS_G3dGlbGetCameraMtx();

    MTX_MultVec43(pWorld, camera, &tmp);

    w = (fx32)(((fx64)tmp.x * proj->_03 +
                (fx64)tmp.y * proj->_13 +
                (fx64)tmp.z * proj->_23) >> FX32_SHIFT);
    w += proj->_33;

    FX_InvAsync(w);

    vec.x = (fx32)(((fx64)tmp.x * proj->_00 +
                    (fx64)tmp.y * proj->_10 +
                    (fx64)tmp.z * proj->_20) >> FX32_SHIFT);
    vec.x += proj->_30;

    vec.y = (fx32)(((fx64)tmp.x * proj->_01 +
                    (fx64)tmp.y * proj->_11 +
                    (fx64)tmp.z * proj->_21) >> FX32_SHIFT);
    vec.y += proj->_31;

    invW = FX_GetInvResultFx64c();

    // ���K���X�N���[�����W�n�ɕϊ�
    vec.x = (FX_Mul32x64c(vec.x, invW) + FX32_ONE) / 2;
    vec.y = (FX_Mul32x64c(vec.y, invW) + FX32_ONE) / 2;

    // ���[�J�����W�̌��_�����K���X�N���[�����W�n�ɕϊ�����Ă���
    if (vec.x < 0 || vec.y < 0 || vec.x > FX32_ONE || vec.y > FX32_ONE)
    {
        // ��ʊO�̏ꍇ
        rval = -1;
    }
    else
    {
        rval = 0;
    }

    NNS_G3dGlbGetViewPort(&x1, &y1, &x2, &y2);
    dx = x2 - x1;
    dy = y2 - y1;

    // �X��BG�X�N���[�����W�n�ɕϊ�
    // BG�̐����ړ����������Ă���ꍇ�́A�X�Ɍ��ʂ�␳����K�v������B
    *px = x1 + ((vec.x * dx + FX32_HALF) >> FX32_SHIFT);
    *py = 191 - y1 - ((vec.y * dy + FX32_HALF) >> FX32_SHIFT);
    
    return rval;
}


/*---------------------------------------------------------------------------*
    NNS_G3dScrPosToWorldLine

    �X�N���[����̈ʒu�ɑΉ�����Near�N���b�v�ʂ�Far�N���b�v�ʏ�̓_��
    ���[���h���W�n�ŕԂ��܂��B
    pFar��NULL�̏ꍇ�́AFar�N���b�v�ʏ�̓_�̌v�Z�͏ȗ�����܂��B

    �Ԃ�l��0�Ȃ�΁A�X�N���[����̓_�̓r���[�|�[�g���ł��B
    �Ԃ�l��-1�Ȃ�΁A�X�N���[����̓_�̓r���[�|�[�g�O�ł��B
 *---------------------------------------------------------------------------*/
int
NNS_G3dScrPosToWorldLine(int px, int py, VecFx32* pNear, VecFx32* pFar)
{
    int rval;
    int x1, y1, x2, y2;
    int dx, dy;
    fx32 x, y;
    const MtxFx44* m;
    VecFx32 vNear, vFar;
    fx64c invWNear, invWFar;
    fx32 wNear, wFar;

    NNS_G3D_NULL_ASSERT(pNear);

    // �a�f�X�N���[�����W�n���琳�K���X�N���[�����W�n�ɕϊ�
    NNS_G3dGlbGetViewPort(&x1, &y1, &x2, &y2);
    dx = x2 - x1;
    dy = y2 - y1;

    x = FX_Div((px - x1) << FX32_SHIFT, dx << FX32_SHIFT);
    y = FX_Div((py + y1 - 191) << FX32_SHIFT, -dy << FX32_SHIFT);

    if (x < 0 || y < 0 || x > FX32_ONE || y > FX32_ONE)
    {
        rval = -1;
    }
    else
    {
        rval = 0;
    }

    // +-1�̗����̂ɂȂ�B
    x = (x - FX32_HALF) * 2;
    y = (y - FX32_HALF) * 2;

    // �ˉe�s��ƃJ�����s����������s��̋t�s����擾����B
    m = NNS_G3dGlbGetInvVP();

    // NEAR�ʏ�̓_��(x, y, -FX32_ONE, FX32_ONE)
    // FAR�ʏ�̓_�� (x, y,  FX32_ONE, FX32_ONE)
    // �t�s��������ă��[���h���W�n�ł̓_�����߂�B
    wNear   = m->_33 + (fx32)(((fx64)x * m->_03 + (fx64)y * m->_13) >> FX32_SHIFT);
    FX_InvAsync(wNear - m->_23);

    vNear.x = m->_30 + (fx32)(((fx64)x * m->_00 + (fx64)y * m->_10) >> FX32_SHIFT);
    vNear.y = m->_31 + (fx32)(((fx64)x * m->_01 + (fx64)y * m->_11) >> FX32_SHIFT);
    vNear.z = m->_32 + (fx32)(((fx64)x * m->_02 + (fx64)y * m->_12) >> FX32_SHIFT);

    if (pFar)
    {
        vFar.x = vNear.x + m->_20;
        vFar.y = vNear.y + m->_21;
        vFar.z = vNear.z + m->_22;
        wFar   = wNear + m->_23;
    }

    vNear.x -= m->_20;
    vNear.y -= m->_21;
    vNear.z -= m->_22;

    invWNear = FX_GetInvResultFx64c();
    if (pFar)
        FX_InvAsync(wFar);
    
    pNear->x = FX_Mul32x64c(vNear.x, invWNear);
    pNear->y = FX_Mul32x64c(vNear.y, invWNear);
    pNear->z = FX_Mul32x64c(vNear.z, invWNear);

    if (pFar)
    {
        invWFar = FX_GetInvResultFx64c();

        pFar->x = FX_Mul32x64c(vFar.x, invWFar);
        pFar->y = FX_Mul32x64c(vFar.y, invWFar);
        pFar->z = FX_Mul32x64c(vFar.z, invWFar);
    }

    return rval;
}

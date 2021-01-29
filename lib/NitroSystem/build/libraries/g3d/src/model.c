/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d
  File:     model.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.12 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/model.h>


/*---------------------------------------------------------------------------*
    NNSi_G3dModifyMatFlag

    ���f�����̑S�Ẵ}�e���A���̃t���O�ɑ΂���flag�Ŏw�肵���t���O��ON/OFF����.
 *---------------------------------------------------------------------------*/
void
NNSi_G3dModifyMatFlag(NNSG3dResMdl* pMdl, BOOL isOn, NNSG3dMatFlag flag)
{
    u32 numMat;
    u32 i;
    NNSG3dResMat* mat;
    NNS_G3D_NULL_ASSERT(pMdl);

    numMat = pMdl->info.numMat;
    mat = NNS_G3dGetMat(pMdl);

    for (i = 0; i < numMat; ++i)
    {
        NNSG3dResMatData* data = NNS_G3dGetMatDataByIdx(mat, i);
        NNS_G3D_NULL_ASSERT(data);

        if (isOn)
        {
            data->flag |= flag;
        }
        else
        {
            data->flag &= ~flag;
        }
    }
}


/*---------------------------------------------------------------------------*
    NNSi_G3dModifyPolygonAttrMask

    ���f�����̑S�Ẵ}�e���A����PolygonAttr�ɑ΂���mask��ON/OFF����
 *---------------------------------------------------------------------------*/
void
NNSi_G3dModifyPolygonAttrMask(NNSG3dResMdl* pMdl, BOOL isOn, u32 mask)
{
    u32 numMat;
    u32 i;
    NNSG3dResMat* mat;
    NNS_G3D_NULL_ASSERT(pMdl);

    numMat = pMdl->info.numMat;
    mat = NNS_G3dGetMat(pMdl);

    for (i = 0; i < numMat; ++i)
    {
        NNSG3dResMatData* data = NNS_G3dGetMatDataByIdx(mat, i);
        NNS_G3D_NULL_ASSERT(data);

        if (isOn)
        {
            data->polyAttrMask |= mask;
        }
        else
        {
            data->polyAttrMask &= ~mask;
        }
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlDiff

    ���f�����\�[�X����matID�Ԗڂ̃}�e���A����diffuse��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlDiff(NNSG3dResMdl* pMdl, u32 matID, GXRgb col)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);
    GXRGB_ASSERT(col);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlDiff is available only to standard(default) materials");

    data->diffAmb = (data->diffAmb & ~(REG_G3_DIF_AMB_DIFFUSE_RED_MASK | REG_G3_DIF_AMB_DIFFUSE_GREEN_MASK | REG_G3_DIF_AMB_DIFFUSE_BLUE_MASK)) |
                    (col << REG_G3_DIF_AMB_DIFFUSE_RED_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlAmb

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A����ambient��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlAmb(NNSG3dResMdl* pMdl, u32 matID, GXRgb col)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);
    GXRGB_ASSERT(col);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlAmb is available only to standard(default) materials");

    data->diffAmb = (data->diffAmb & ~(REG_G3_DIF_AMB_AMBIENT_RED_MASK | REG_G3_DIF_AMB_AMBIENT_GREEN_MASK | REG_G3_DIF_AMB_AMBIENT_BLUE_MASK)) |
                    (col << REG_G3_DIF_AMB_AMBIENT_RED_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlSpec

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A����specular��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlSpec(NNSG3dResMdl* pMdl, u32 matID, GXRgb col)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);
    GXRGB_ASSERT(col);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlSpec is available only to standard(default) materials");

    data->specEmi = (data->specEmi & ~(REG_G3_SPE_EMI_SPECULAR_RED_MASK | REG_G3_SPE_EMI_SPECULAR_GREEN_MASK | REG_G3_SPE_EMI_SPECULAR_BLUE_MASK)) |
                    (col << REG_G3_SPE_EMI_SPECULAR_RED_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlEmi

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A����emission��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlEmi(NNSG3dResMdl* pMdl, u32 matID, GXRgb col)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);
    GXRGB_ASSERT(col);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlEmi is available only to standard(default) materials");

    data->specEmi = (data->specEmi & ~(REG_G3_SPE_EMI_EMISSION_RED_MASK | REG_G3_SPE_EMI_EMISSION_GREEN_MASK | REG_G3_SPE_EMI_EMISSION_BLUE_MASK)) |
                    (col << REG_G3_SPE_EMI_EMISSION_RED_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlLightEnableFlag

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃��C�g�C�l�[�u���t���O��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlLightEnableFlag(NNSG3dResMdl* pMdl, u32 matID, int light)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);
    GX_LIGHTMASK_ASSERT(light);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlLightEnableFlag is available only to standard(default) materials");

    data->polyAttr = (data->polyAttr & ~REG_G3_POLYGON_ATTR_LE_MASK) |
                     (light << REG_G3_POLYGON_ATTR_LE_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlPolygonMode

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃|���S�����[�h��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlPolygonMode(NNSG3dResMdl* pMdl, u32 matID, GXPolygonMode polyMode)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);
    GX_POLYGONMODE_ASSERT(polyMode);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlPolygonMode is available only to standard(default) materials");

    data->polyAttr = (data->polyAttr & ~REG_G3_POLYGON_ATTR_PM_MASK) |
                     (polyMode << REG_G3_POLYGON_ATTR_PM_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlCullMode

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃J�����[�h��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlCullMode(NNSG3dResMdl* pMdl, u32 matID, GXCull cullMode)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);
    GX_CULL_ASSERT(cullMode);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlCullMode is available only to standard(default) materials");

    data->polyAttr = (data->polyAttr & ~(REG_G3_POLYGON_ATTR_BK_MASK | REG_G3_POLYGON_ATTR_FR_MASK)) |
                     (cullMode << REG_G3_POLYGON_ATTR_BK_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlPolygonID

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃|���S���h�c��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlPolygonID(NNSG3dResMdl* pMdl, u32 matID, int polygonID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);
    GX_POLYGON_ATTR_POLYGONID_ASSERT(polygonID);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlPolygonID is available only to standard(default) materials");

    data->polyAttr = (data->polyAttr & ~REG_G3_POLYGON_ATTR_ID_MASK) |
                     (polygonID << REG_G3_POLYGON_ATTR_ID_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlAlpha

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃���ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlAlpha(NNSG3dResMdl* pMdl, u32 matID, int alpha)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);
    GX_POLYGON_ATTR_ALPHA_ASSERT(alpha);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlAlpha is available only to standard(default) materials");

    data->polyAttr = (data->polyAttr & ~REG_G3_POLYGON_ATTR_ALPHA_MASK) |
                     (alpha << REG_G3_POLYGON_ATTR_ALPHA_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetFogEnableFlag

    ���f�����\�[�X��matID�Ԗڂ̃t�H�O�C�l�[�u���t���O��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlFogEnableFlag(NNSG3dResMdl* pMdl, u32 matID, BOOL flag)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlFogEnableFlag is available only to standard(default) materials");

    if (flag)
        data->polyAttr |= REG_G3_POLYGON_ATTR_FE_MASK;
    else
        data->polyAttr &= ~REG_G3_POLYGON_ATTR_FE_MASK;
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetDepthTestCond

    ���f�����\�[�X��matID�Ԗڂ̃f�v�X�e�X�g������ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlDepthTestCond(NNSG3dResMdl* pMdl, u32 matID, BOOL flag)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlDepthTestCond is available only to standard(default) materials");

    if (flag)
        data->polyAttr |= REG_G3_POLYGON_ATTR_DT_MASK;
    else
        data->polyAttr &= ~REG_G3_POLYGON_ATTR_DT_MASK;
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdl1Dot

    ���f�����\�[�X��matID�Ԗڂ̂P�h�b�g�|���S���\���w���ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdl1Dot(NNSG3dResMdl* pMdl, u32 matID, BOOL flag)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdl1Dot is available only to standard(default) materials");

    if (flag)
        data->polyAttr |= REG_G3_POLYGON_ATTR_D1_MASK;
    else
        data->polyAttr &= ~REG_G3_POLYGON_ATTR_D1_MASK;
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlFarClip

    ���f�����\�[�X��matID�Ԗڂ�FAR�ʌ����|���S���\���w���ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlFarClip(NNSG3dResMdl* pMdl, u32 matID, BOOL flag)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlFarClip is available only to standard(default) materials");

    if (flag)
        data->polyAttr |= REG_G3_POLYGON_ATTR_FC_MASK;
    else
        data->polyAttr &= ~REG_G3_POLYGON_ATTR_FC_MASK;
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlXLDepthUpdate

    ���f�����\�[�X��matID�Ԗڂ̔������|���S���̃f�v�X�l�X�V�C�l�[�u���t���O��ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlXLDepthUpdate(NNSG3dResMdl* pMdl, u32 matID, BOOL flag)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);
    NNS_G3D_ASSERTMSG(data->itemTag == NNS_G3D_ITEMTAG_MAT_STANDARD,
                      "NNS_G3dMdlSetMdlXLDepthUpdate is available only to standard(default) materials");

    if (flag)
        data->polyAttr |= REG_G3_POLYGON_ATTR_XL_MASK;
    else
        data->polyAttr &= ~REG_G3_POLYGON_ATTR_XL_MASK;
}

/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlDiff

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A����diffuse�𓾂�
 *---------------------------------------------------------------------------*/
GXRgb
NNS_G3dMdlGetMdlDiff(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (GXRgb)((data->diffAmb & (REG_G3_DIF_AMB_DIFFUSE_RED_MASK | REG_G3_DIF_AMB_DIFFUSE_GREEN_MASK | REG_G3_DIF_AMB_DIFFUSE_BLUE_MASK))
                >> REG_G3_DIF_AMB_DIFFUSE_RED_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlAmb

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A����ambient�𓾂�
 *---------------------------------------------------------------------------*/
GXRgb
NNS_G3dMdlGetMdlAmb(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (GXRgb)((data->diffAmb & (REG_G3_DIF_AMB_AMBIENT_RED_MASK | REG_G3_DIF_AMB_AMBIENT_GREEN_MASK | REG_G3_DIF_AMB_AMBIENT_BLUE_MASK))
                >> REG_G3_DIF_AMB_AMBIENT_RED_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlSpec

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A����specular�𓾂�
 *---------------------------------------------------------------------------*/
GXRgb
NNS_G3dMdlGetMdlSpec(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (GXRgb)((data->specEmi & (REG_G3_SPE_EMI_SPECULAR_RED_MASK | REG_G3_SPE_EMI_SPECULAR_GREEN_MASK | REG_G3_SPE_EMI_SPECULAR_BLUE_MASK))
                >> REG_G3_SPE_EMI_SPECULAR_RED_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlEmi

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A����emission�𓾂�
 *---------------------------------------------------------------------------*/
GXRgb
NNS_G3dMdlGetMdlEmi(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (GXRgb)((data->specEmi & (REG_G3_SPE_EMI_EMISSION_RED_MASK | REG_G3_SPE_EMI_EMISSION_GREEN_MASK | REG_G3_SPE_EMI_EMISSION_BLUE_MASK))
                >> REG_G3_SPE_EMI_EMISSION_RED_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlLightEnableFlag

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃��C�g�C�l�[�u���t���O�𓾂�
 *---------------------------------------------------------------------------*/
int
NNS_G3dMdlGetMdlLightEnableFlag(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (int)( (data->polyAttr & REG_G3_POLYGON_ATTR_LE_MASK)
                    >> REG_G3_POLYGON_ATTR_LE_SHIFT );
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlPolygonMode

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃|���S�����[�h�𓾂�
 *---------------------------------------------------------------------------*/
GXPolygonMode
NNS_G3dMdlGetMdlPolygonMode(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (GXPolygonMode)((data->polyAttr & REG_G3_POLYGON_ATTR_PM_MASK)
                                >> REG_G3_POLYGON_ATTR_PM_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlCullMode

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃J�����[�h�𓾂�
 *---------------------------------------------------------------------------*/
GXCull
NNS_G3dMdlGetMdlCullMode(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (GXCull)((data->polyAttr & (REG_G3_POLYGON_ATTR_BK_MASK | REG_G3_POLYGON_ATTR_FR_MASK))
                                >> REG_G3_POLYGON_ATTR_BK_SHIFT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlPolygonID

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃|���S���h�c�𓾂�
 *---------------------------------------------------------------------------*/
int
NNS_G3dMdlGetMdlPolygonID(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (int)( (data->polyAttr & REG_G3_POLYGON_ATTR_ID_MASK)
                                >> REG_G3_POLYGON_ATTR_ID_SHIFT );
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlAlpha

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃��𓾂�
 *---------------------------------------------------------------------------*/
int
NNS_G3dMdlGetMdlAlpha(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (int)( (data->polyAttr & REG_G3_POLYGON_ATTR_ALPHA_MASK)
                                >> REG_G3_POLYGON_ATTR_ALPHA_SHIFT );
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlFogEnableFlag

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃t�H�O�C�l�[�u���t���O�𓾂�
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dMdlGetMdlFogEnableFlag(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (int)( (data->polyAttr & REG_G3_POLYGON_ATTR_FE_MASK) );
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlDepthTestCond

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̃f�v�X�e�X�g�����𓾂�
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dMdlGetMdlDepthTestCond(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (int)( (data->polyAttr & REG_G3_POLYGON_ATTR_DT_MASK) );
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdl1Dot

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̂P�h�b�g�|���S���\���w��𓾂�
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dMdlGetMdl1Dot(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (int)( (data->polyAttr & REG_G3_POLYGON_ATTR_D1_MASK) );
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlFarClip

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A����FAR�ʌ����|���S���\���w��𓾂�
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dMdlGetMdlFarClip(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (int)( (data->polyAttr & REG_G3_POLYGON_ATTR_FC_MASK) );
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlGetMdlXLDepthUpdate

    ���f�����\�[�X��matID�Ԗڂ̃}�e���A���̔������|���S���̃f�v�X�l�X�V�C�l�[�u��
    �t���O�𓾂�
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dMdlGetMdlXLDepthUpdate(const NNSG3dResMdl* pMdl, u32 matID)
{
    NNSG3dResMatData* data;
    NNS_G3D_NULL_ASSERT(pMdl);

    data = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(pMdl), matID);
    NNS_G3D_NULL_ASSERT(data);

    return (int)( (data->polyAttr & REG_G3_POLYGON_ATTR_XL_MASK) );
}

/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlDiffAll

    ���f�����\�[�X���̃}�e���A����diffuse�S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlDiffAll(NNSG3dResMdl* pMdl, GXRgb col)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlDiff(pMdl, matID, col);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlAmbAll

    ���f�����\�[�X���̃}�e���A����ambient�S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlAmbAll(NNSG3dResMdl* pMdl, GXRgb col)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlAmb(pMdl, matID, col);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlSpecAll

    ���f�����\�[�X���̃}�e���A����specular�S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlSpecAll(NNSG3dResMdl* pMdl, GXRgb col)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlSpec(pMdl, matID, col);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlEmiAll

    ���f�����\�[�X���̃}�e���A����emission�S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlEmiAll(NNSG3dResMdl* pMdl, GXRgb col)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlEmi(pMdl, matID, col);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlLightEnableFlagAll

    ���f�����\�[�X���̃}�e���A���̃��C�g�C�l�[�u���t���O�S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlLightEnableFlagAll(NNSG3dResMdl* pMdl, int light)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlLightEnableFlag(pMdl, matID, light);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlPolygonModeAll

    ���f�����\�[�X���̃}�e���A���̃|���S�����[�h�S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlPolygonModeAll(NNSG3dResMdl* pMdl, GXPolygonMode polyMode)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlPolygonMode(pMdl, matID, polyMode);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlCullModeAll

    ���f�����\�[�X���̃}�e���A���̃J�����[�h�S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlCullModeAll(NNSG3dResMdl* pMdl, GXCull cullMode)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlCullMode(pMdl, matID, cullMode);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlPolygonIDAll

    ���f�����\�[�X���̃}�e���A���̃|���S���h�c�S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlPolygonIDAll(NNSG3dResMdl* pMdl, int polygonID)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlPolygonID(pMdl, matID, polygonID);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlAlphaAll

    ���f�����\�[�X���̃}�e���A���̃��S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlAlphaAll(NNSG3dResMdl* pMdl, int alpha)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlAlpha(pMdl, matID, alpha);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlFogEnableFlagAll

    ���f�����\�[�X���̃}�e���A���̃t�H�O�C�l�[�u���t���O�S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlFogEnableFlagAll(NNSG3dResMdl* pMdl, BOOL flag)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlFogEnableFlag(pMdl, matID, flag);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlDepthTestCondAll

    ���f�����\�[�X���̃}�e���A���̃f�v�X�e�X�g�����S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlDepthTestCondAll(NNSG3dResMdl* pMdl, BOOL flag)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlDepthTestCond(pMdl, matID, flag);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdl1DotAll

    ���f�����\�[�X���̃}�e���A���̂P�h�b�g�|���S���\���w��S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdl1DotAll(NNSG3dResMdl* pMdl, BOOL flag)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdl1Dot(pMdl, matID, flag);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetFarClipAll

    ���f�����\�[�X���̃}�e���A����FAR�ʌ����|���S���\���w��S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlFarClipAll(NNSG3dResMdl* pMdl, BOOL flag)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlFarClip(pMdl, matID, flag);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlSetMdlXLDepthUpdateAll

    ���f�����\�[�X���̃}�e���A���̔������|���S���̃f�v�X�l�X�V�C�l�[�u���t���O
    �S�Ă�ݒ肷��
 *---------------------------------------------------------------------------*/
void
NNS_G3dMdlSetMdlXLDepthUpdateAll(NNSG3dResMdl* pMdl, BOOL flag)
{
    u32 matID;
    for (matID = 0; matID < pMdl->info.numMat; ++matID)
    {
        NNS_G3dMdlSetMdlXLDepthUpdate(pMdl, matID, flag);
    }
}




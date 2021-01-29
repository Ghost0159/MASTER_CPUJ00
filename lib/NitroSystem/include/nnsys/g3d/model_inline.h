/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d
  File:     model_inline.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#ifndef NNSG3D_MODEL_INLINE_
#define NNSG3D_MODEL_INLINE_

//
// DO NOT INCLUDE THIS FILE DIRECTLY
//

#ifdef __cplusplus
extern "C" {
#endif

void NNSi_G3dModifyMatFlag(NNSG3dResMdl* pMdl, BOOL isOn, NNSG3dMatFlag flag);
void NNSi_G3dModifyPolygonAttrMask(NNSG3dResMdl* pMdl, BOOL isOn, u32 mask);

//
// To Use Global data
//

/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbDiff

    NNS_G3dGlbMaterialColorDiffAmb�Őݒ肵��diffuse�����f�����̃}�e���A����
    diffuse�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbDiff(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyMatFlag(pMdl,
                          FALSE,
                          NNS_G3D_MATFLAG_DIFFUSE);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbAmb

    NNS_G3dGlbMaterialColorDiffAmb�Őݒ肵��ambient�����f�����̃}�e���A����
    ambient�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbAmb(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyMatFlag(pMdl,
                          FALSE,
                          NNS_G3D_MATFLAG_AMBIENT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbSpec

    NNS_G3dGlbMaterialColorSpecEmi�Őݒ肵��specular�����f�����̃}�e���A����
    specular�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbSpec(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyMatFlag(pMdl,
                          FALSE,
                          NNS_G3D_MATFLAG_SPECULAR);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbEmi

    NNS_G3dGlbMaterialColorSpecEmi�Őݒ肵��emission�����f�����̃}�e���A����
    emission�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbEmi(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyMatFlag(pMdl,
                          FALSE,
                          NNS_G3D_MATFLAG_EMISSION);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbLightEnableFlag

    NNS_G3dGlbPolygonAttr�Őݒ肵�����C�g�C�l�[�u���t���O�����f�����̃}�e���A����
    ���C�g�C�l�[�u���t���O�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbLightEnableFlag(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_LE_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbPolygonMode

    NNS_G3dGlbPolygonAttr�Őݒ肵���|���S�����[�h�����f�����̃}�e���A����
    �|���S�����[�h�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbPolygonMode(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_PM_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbCullMode

    NNS_G3dGlbPolygonAttr�Őݒ肵���J�����[�h�����f�����̃}�e���A����
    �J�����[�h�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbCullMode(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_BK_MASK |
                                  REG_G3_POLYGON_ATTR_FR_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbPolygonID

    NNS_G3dGlbPolygonAttr�Őݒ肵���|���S��ID�����f�����̃}�e���A����
    �|���S��ID�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbPolygonID(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_ID_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbAlpha

    NNS_G3dGlbPolygonAttr�Őݒ肵�����l�����f�����̃}�e���A����
    ���l�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbAlpha(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_ALPHA_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbFogEnableFlag

    NNS_G3dGlbPolygonAttr�Őݒ肵���t�H�O�C�l�[�u���t���O�����f�����̃}�e���A����
    �t�H�O�C�l�[�u���t���O�Ƃ��Ďg�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbFogEnableFlag(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_FE_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbDepthTestCond

    NNS_G3dGlbPolygonAttr�Őݒ肵���f�v�X�e�X�g���������f�����̃}�e���A����
    �f�v�X�e�X�g�����Ƃ��Ďg�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbDepthTestCond(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_DT_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlb1Dot

    NNS_G3dGlbPolygonAttr�Őݒ肵���P�h�b�g�|���S���\���w������f�����̃}�e���A����
    �P�h�b�g�|���S���\���w��Ƃ��Ďg�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlb1Dot(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_D1_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbFarClip

    NNS_G3dGlbPolygonAttr�Őݒ肵��FAR�ʌ����|���S���\���w������f�����̃}�e���A����
    FAR�ʌ����|���S���\���w��Ƃ��Ďg�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbFarClip(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_FC_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseGlbXLDepthUpdate

    NNS_G3dGlbPolygonAttr�Őݒ肵���������|���S���̃f�v�X�l�X�V�C�l�[�u���t���O��
    ���f�����̃}�e���A���̔������|���S���̃f�v�X�l�X�V�C�l�[�u���t���O�Ƃ��Ďg�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseGlbXLDepthUpdate(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  FALSE,
                                  REG_G3_POLYGON_ATTR_XL_MASK);
}


//
// To Use Model's Data
//

/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlDiff

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A����diffuse���g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlDiff(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyMatFlag(pMdl,
                          TRUE,
                          NNS_G3D_MATFLAG_DIFFUSE);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlAmb

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A����ambient���g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlAmb(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyMatFlag(pMdl,
                          TRUE,
                          NNS_G3D_MATFLAG_AMBIENT);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlSpec

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A����specular���g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlSpec(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyMatFlag(pMdl,
                          TRUE,
                          NNS_G3D_MATFLAG_SPECULAR);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlEmi

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A����emission���g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlEmi(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyMatFlag(pMdl,
                          TRUE,
                          NNS_G3D_MATFLAG_EMISSION);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlLightEnableFlag

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A���̃��C�g�C�l�[�u���t���O���g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlLightEnableFlag(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_LE_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlPolygonMode

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A���̃|���S�����[�h���g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlPolygonMode(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_PM_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlCullMode

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A���̃J�����[�h���g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlCullMode(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_BK_MASK |
                                  REG_G3_POLYGON_ATTR_FR_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlPolygonID

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A���̃|���S��ID���g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlPolygonID(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_ID_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlAlpha

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A���̃��l���g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlAlpha(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_ALPHA_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlFogEnableFlag

    ���f�����\�[�X���Őݒ肳��Ă���X�̃}�e���A���̃t�H�O�C�l�[�u���t���O��
    �g�p���܂��B
    (.nsbmd�`���̃t�@�C���̃f�t�H���g�ݒ�ł�)
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlFogEnableFlag(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_FE_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlDepthTestCond

    ���f�����\�[�X���Őݒ肳��Ă���X�̃f�v�X�e�X�g�������g�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlDepthTestCond(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_DT_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdl1Dot

    ���f�����\�[�X���Őݒ肳��Ă���X�̂P�h�b�g�|���S���\���w����g�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdl1Dot(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_D1_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlFarClip

    ���f�����\�[�X���Őݒ肳��Ă���X��FAR�ʌ����|���S���\���w����g�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlFarClip(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_FC_MASK);
}


/*---------------------------------------------------------------------------*
    NNS_G3dMdlUseMdlXLDepthUpdate

    ���f�����\�[�X���Őݒ肳��Ă���X�̔������|���S���̃f�v�X�l�X�V�C�l�[�u���t���O
    ���g�p���܂��B
 *---------------------------------------------------------------------------*/
NNS_G3D_INLINE void
NNS_G3dMdlUseMdlXLDepthUpdate(NNSG3dResMdl* pMdl)
{
    NNSi_G3dModifyPolygonAttrMask(pMdl,
                                  TRUE,
                                  REG_G3_POLYGON_ATTR_XL_MASK);
}





#ifdef __cplusplus
}
#endif

#endif

/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d
  File:     glbstate.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.22 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/glbstate.h>
#include <nnsys/g3d/gecom.h>

//
//
// Functions
//
//

/*---------------------------------------------------------------------------*
    NNS_G3dGlbInit

    NNS_G3dGlb�\���̂̃C�j�V�����C�Y
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbInit(void)
{
    // �萔�̐ݒ�
    NNS_G3dGlb.cmd0 = GX_PACK_OP(G3OP_MTX_MODE, G3OP_MTX_LOAD_4x4, G3OP_MTX_MODE, G3OP_MTX_LOAD_4x3);
    NNS_G3dGlb.mtxmode_proj   = GX_MTXMODE_PROJECTION;
    NNS_G3dGlb.mtxmode_posvec = GX_MTXMODE_POSITION_VECTOR;

    NNS_G3dGlb.cmd1 = GX_PACK_OP(G3OP_LIGHT_VECTOR, G3OP_LIGHT_VECTOR, G3OP_LIGHT_VECTOR, G3OP_LIGHT_VECTOR);

    NNS_G3dGlb.cmd2 = GX_PACK_OP(G3OP_DIF_AMB, G3OP_SPE_EMI, G3OP_POLYGON_ATTR, G3OP_VIEWPORT);

    NNS_G3dGlb.cmd3 = GX_PACK_OP(G3OP_LIGHT_COLOR, G3OP_LIGHT_COLOR, G3OP_LIGHT_COLOR, G3OP_LIGHT_COLOR);

    NNS_G3dGlb.cmd4 = GX_PACK_OP(G3OP_MTX_MULT_4x3, G3OP_MTX_SCALE, G3OP_TEXIMAGE_PARAM, G3OP_NOP);
        
    // ���ꂼ���P�ʍs��ɃZ�b�g
    MTX_Identity43(&NNS_G3dGlb.cameraMtx);
    MTX_Identity44(&NNS_G3dGlb.projMtx);

    // ���C�g�x�N�g���́A�s���łȂ��l�ɐݒ�
    NNS_G3dGlb.lightVec[0] = GX_PACK_LIGHTVECTOR_PARAM(0, -FX16_SQRT1_3, -FX16_SQRT1_3, -FX16_SQRT1_3);
    NNS_G3dGlb.lightVec[1] = GX_PACK_LIGHTVECTOR_PARAM(1, -FX16_ONE, 0, 0);
    NNS_G3dGlb.lightVec[2] = GX_PACK_LIGHTVECTOR_PARAM(2,  FX16_ONE - 1, 0, 0);
    NNS_G3dGlb.lightVec[3] = GX_PACK_LIGHTVECTOR_PARAM(3, 0, -FX16_ONE, 0);

    // �ݒ�}�e���A���J���[�͕ύX�̉\������
    NNS_G3dGlb.prmMatColor0 = GX_PACK_DIFFAMB_PARAM(GX_RGB(16, 16, 16),
                                                    GX_RGB(16, 16, 16),
                                                    TRUE);
    NNS_G3dGlb.prmMatColor1 = GX_PACK_SPECEMI_PARAM(GX_RGB(16, 16, 16),
                                                    GX_RGB(16, 16, 16),
                                                    TRUE);

    // PolygonAttr�͖���Ȑݒ�ɂ��Ă���
    NNS_G3dGlb.prmPolygonAttr = GX_PACK_POLYGONATTR_PARAM(0xf,  // ���C�gON
                                                          GX_POLYGONMODE_MODULATE,
                                                          GX_CULL_BACK,
                                                          0,  // PolygonID
                                                          31, // Alpha
                                                          GX_POLYGON_ATTR_MISC_NONE);

    NNS_G3dGlb.prmViewPort = GX_PACK_VIEWPORT_PARAM(0, 0, 255, 191);
    
    NNS_G3dGlb.lightColor[0] = GX_PACK_LIGHTCOLOR_PARAM(0, GX_RGB(31, 31, 31));
    NNS_G3dGlb.lightColor[1] = GX_PACK_LIGHTCOLOR_PARAM(1, GX_RGB(31,  0,  0));
    NNS_G3dGlb.lightColor[2] = GX_PACK_LIGHTCOLOR_PARAM(2, GX_RGB( 0, 31,  0));
    NNS_G3dGlb.lightColor[3] = GX_PACK_LIGHTCOLOR_PARAM(3, GX_RGB( 0,  0, 31));

    // Base SRT
    NNS_G3dGlb.prmBaseTrans.x = 0;
    NNS_G3dGlb.prmBaseTrans.y = 0;
    NNS_G3dGlb.prmBaseTrans.z = 0;

    MTX_Identity33(&NNS_G3dGlb.prmBaseRot);

    NNS_G3dGlb.prmBaseScale.x = FX32_ONE;
    NNS_G3dGlb.prmBaseScale.y = FX32_ONE;
    NNS_G3dGlb.prmBaseScale.z = FX32_ONE;
    NNS_G3dGlb.prmTexImageParam = 0;

    NNS_G3dGlb.flag = (NNSG3dGlbFlag) 0;

    NNS_G3dGlb.camPos.x = NNS_G3dGlb.camPos.y = NNS_G3dGlb.camPos.z = 0;
    NNS_G3dGlb.camUp.x = NNS_G3dGlb.camUp.z = 0;
    NNS_G3dGlb.camUp.y = FX32_ONE;
    NNS_G3dGlb.camTarget.x = NNS_G3dGlb.camTarget.y = 0;
    NNS_G3dGlb.camTarget.z = -FX32_ONE;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbFlushP

    NNSG3dGlb�̓��e���W�I���g���G���W���ɔ��f������B
    �J�����g�ˉe�s��ɂ͎ˉe�ϊ��s�񂪐ݒ肳���B
    �J�����g�ʒu���W�s��ɂ̓J�����s��ƃ��[���h�ϊ��s��������������̂��ݒ肳���B

    Color���𑗂�̂́A�����Ȃ�Sbc��SHP�R�}���h������悤�ȏꍇ�ɔ����Ă���B
    �R�}���h���������ꂽ��́APOSITION/VECTOR���[�h�ɂȂ��Ă���B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbFlushP(void)
{
    static const u32 sz = (sizeof(NNS_G3dGlb.cmd0)              +
                           sizeof(NNS_G3dGlb.mtxmode_proj)      +
                           sizeof(NNS_G3dGlb.projMtx)           +
                           sizeof(NNS_G3dGlb.mtxmode_posvec)    +
                           sizeof(NNS_G3dGlb.cameraMtx)         +
                           sizeof(NNS_G3dGlb.cmd1)              +
                           sizeof(NNS_G3dGlb.lightVec[0]) * 4   +
                           sizeof(NNS_G3dGlb.cmd2)              +
                           sizeof(NNS_G3dGlb.prmMatColor0)      +
                           sizeof(NNS_G3dGlb.prmMatColor1)      +
                           sizeof(NNS_G3dGlb.prmPolygonAttr)    +
                           sizeof(NNS_G3dGlb.prmViewPort)       +
                           sizeof(NNS_G3dGlb.cmd4)              +
                           sizeof(NNS_G3dGlb.prmBaseTrans)      +
                           sizeof(NNS_G3dGlb.prmBaseRot)        +
                           sizeof(NNS_G3dGlb.prmBaseScale)      +
                           sizeof(NNS_G3dGlb.prmTexImageParam)  +
                           sizeof(NNS_G3dGlb.cmd3)              +
                           sizeof(NNS_G3dGlb.lightColor[0]) * 4) / 4;

    NNS_G3D_ASSERT(NNS_G3dGlb.cmd0 == ((G3OP_MTX_MODE << 0)     |
                                       (G3OP_MTX_LOAD_4x4 << 8) |
                                       (G3OP_MTX_MODE << 16)    |
                                       (G3OP_MTX_LOAD_4x3 << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.mtxmode_proj == GX_MTXMODE_PROJECTION);
    NNS_G3D_ASSERT(NNS_G3dGlb.mtxmode_posvec == GX_MTXMODE_POSITION_VECTOR);
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd1 == ((G3OP_LIGHT_VECTOR << 0)  |
                                       (G3OP_LIGHT_VECTOR << 8)  |
                                       (G3OP_LIGHT_VECTOR << 16) |
                                       (G3OP_LIGHT_VECTOR << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd2 == ((G3OP_DIF_AMB << 0)       |
                                       (G3OP_SPE_EMI << 8)       |
                                       (G3OP_POLYGON_ATTR << 16) |
                                       (G3OP_VIEWPORT << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd3 == ((G3OP_LIGHT_COLOR << 0)  |
                                       (G3OP_LIGHT_COLOR << 8)  |
                                       (G3OP_LIGHT_COLOR << 16) |
                                       (G3OP_LIGHT_COLOR << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd4 == ((G3OP_MTX_MULT_4x3 << 0) |
                                       (G3OP_MTX_SCALE << 8)    |
                                       (G3OP_TEXIMAGE_PARAM << 16)));

    NNS_G3dGeBufferData_N((u32*)&NNS_G3dGlb.cmd0, sz);

    NNS_G3dGlb.flag &= ~NNS_G3D_GLB_FLAG_FLUSH_WVP;
    NNS_G3dGlb.flag &= ~NNS_G3D_GLB_FLAG_FLUSH_VP;
    //
    // �R�}���h�̏������POSITION/VECTOR���[�h�ɂȂ��Ă���B
    //
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbFlushVP

    NNSG3dGlb�̓��e���W�I���g���G���W���ɔ��f������B
    �J�����g�ˉe�s��ɂ͎ˉe�ϊ��s��ƃJ�����s��������������̂��ݒ肳���B
    �J�����g�ʒu���W�s��ɂ̓��[���h�ϊ��s�񂪐ݒ肳���B

    Color���𑗂�̂́A�����Ȃ�Sbc��SHP�R�}���h������悤�ȏꍇ�ɔ����Ă���B
    �R�}���h���������ꂽ��́APOSITION/VECTOR���[�h�ɂȂ��Ă���B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbFlushVP(void)
{
    static const u32 sz = (sizeof(NNS_G3dGlb.cmd1)              +
                           sizeof(NNS_G3dGlb.lightVec[0]) * 4   +
                           sizeof(NNS_G3dGlb.cmd2)              +
                           sizeof(NNS_G3dGlb.prmMatColor0)      +
                           sizeof(NNS_G3dGlb.prmMatColor1)      +
                           sizeof(NNS_G3dGlb.prmPolygonAttr)    +
                           sizeof(NNS_G3dGlb.prmViewPort)       +
                           sizeof(NNS_G3dGlb.cmd3)              +
                           sizeof(NNS_G3dGlb.lightColor[0]) * 4 +
                           sizeof(NNS_G3dGlb.cmd4)              +
                           sizeof(NNS_G3dGlb.prmBaseRot)        +
                           sizeof(NNS_G3dGlb.prmBaseTrans)      +
                           sizeof(NNS_G3dGlb.prmBaseScale)      +
                           sizeof(NNS_G3dGlb.prmTexImageParam)) / 4;

    NNS_G3D_ASSERT(NNS_G3dGlb.cmd0 == ((G3OP_MTX_MODE << 0)     |
                                       (G3OP_MTX_LOAD_4x4 << 8) |
                                       (G3OP_MTX_MODE << 16)    |
                                       (G3OP_MTX_LOAD_4x3 << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.mtxmode_proj == GX_MTXMODE_PROJECTION);
    NNS_G3D_ASSERT(NNS_G3dGlb.mtxmode_posvec == GX_MTXMODE_POSITION_VECTOR);
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd1 == ((G3OP_LIGHT_VECTOR << 0)  |
                                       (G3OP_LIGHT_VECTOR << 8)  |
                                       (G3OP_LIGHT_VECTOR << 16) |
                                       (G3OP_LIGHT_VECTOR << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd2 == ((G3OP_DIF_AMB << 0)       |
                                       (G3OP_SPE_EMI << 8)       |
                                       (G3OP_POLYGON_ATTR << 16) |
                                       (G3OP_VIEWPORT << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd3 == ((G3OP_LIGHT_COLOR << 0)  |
                                       (G3OP_LIGHT_COLOR << 8)  |
                                       (G3OP_LIGHT_COLOR << 16) |
                                       (G3OP_LIGHT_COLOR << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd4 == ((G3OP_MTX_MULT_4x3 << 0) |
                                       (G3OP_MTX_SCALE << 8)    |
                                       (G3OP_TEXIMAGE_PARAM << 16)));

    // Projection�s�񃂁[�h�ɂ���Projection�s���ݒ�
    NNS_G3dGeBufferOP_N(GX_PACK_OP(G3OP_MTX_MODE, G3OP_MTX_LOAD_4x4, G3OP_NOP, G3OP_NOP),
                        (u32*)&NNS_G3dGlb.mtxmode_proj,
                        (sizeof(NNS_G3dGlb.mtxmode_proj) +
                         sizeof(NNS_G3dGlb.projMtx)) / 4);    

    // Projection�s��ɃJ�����s���������
    NNS_G3dGeBufferOP_N((G3OP_MTX_MULT_4x3 << 0),
                        (u32*)&NNS_G3dGlb.cameraMtx,
                        sizeof(NNS_G3dGlb.cameraMtx) / 4);

    NNS_G3dGeMtxMode(GX_MTXMODE_POSITION_VECTOR);
    NNS_G3dGeBufferOP_N(G3OP_MTX_IDENTITY, (u32*)&NNS_G3dGlb.cmd1, sz);

    NNS_G3dGlb.flag &= ~NNS_G3D_GLB_FLAG_FLUSH_WVP;
    NNS_G3dGlb.flag |=  NNS_G3D_GLB_FLAG_FLUSH_VP;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbFlushWVP

    NNSG3dGlb�̓��e���W�I���g���G���W���ɔ��f������B
    �J�����g�ˉe�s��ɂ͎ˉe�ϊ��s��ƃJ�����s��ƃ��[���h�ϊ��s��������������̂��ݒ肳���B
    �J�����g�ʒu���W�s��ɂ͒P�ʍs�񂪐ݒ肳���B

    Color���𑗂�̂́A�����Ȃ�Sbc��SHP�R�}���h������悤�ȏꍇ�ɔ����Ă���B
    �R�}���h���������ꂽ��́APOSITION/VECTOR���[�h�ɂȂ��Ă���B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbFlushWVP(void)
{
    static const u32 sz = (sizeof(NNS_G3dGlb.cmd1)              +
                           sizeof(NNS_G3dGlb.lightVec[0]) * 4   +
                           sizeof(NNS_G3dGlb.cmd2)              +
                           sizeof(NNS_G3dGlb.prmMatColor0)      +
                           sizeof(NNS_G3dGlb.prmMatColor1)      +
                           sizeof(NNS_G3dGlb.prmPolygonAttr)    +
                           sizeof(NNS_G3dGlb.prmViewPort)       +
                           sizeof(NNS_G3dGlb.cmd3)              +
                           sizeof(NNS_G3dGlb.lightColor[0]) * 4) / 4;

    NNS_G3D_ASSERT(NNS_G3dGlb.cmd0 == ((G3OP_MTX_MODE << 0)     |
                                       (G3OP_MTX_LOAD_4x4 << 8) |
                                       (G3OP_MTX_MODE << 16)    |
                                       (G3OP_MTX_LOAD_4x3 << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.mtxmode_proj == GX_MTXMODE_PROJECTION);
    NNS_G3D_ASSERT(NNS_G3dGlb.mtxmode_posvec == GX_MTXMODE_POSITION_VECTOR);
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd1 == ((G3OP_LIGHT_VECTOR << 0)  |
                                       (G3OP_LIGHT_VECTOR << 8)  |
                                       (G3OP_LIGHT_VECTOR << 16) |
                                       (G3OP_LIGHT_VECTOR << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd2 == ((G3OP_DIF_AMB << 0)       |
                                       (G3OP_SPE_EMI << 8)       |
                                       (G3OP_POLYGON_ATTR << 16) |
                                       (G3OP_VIEWPORT << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd3 == ((G3OP_LIGHT_COLOR << 0)  |
                                       (G3OP_LIGHT_COLOR << 8)  |
                                       (G3OP_LIGHT_COLOR << 16) |
                                       (G3OP_LIGHT_COLOR << 24)));
    NNS_G3D_ASSERT(NNS_G3dGlb.cmd4 == ((G3OP_MTX_MULT_4x3 << 0) |
                                       (G3OP_MTX_SCALE << 8)    |
                                       (G3OP_TEXIMAGE_PARAM << 16)));


    // Projection�s�񃂁[�h�ɂ���Projection�s���ݒ�
    NNS_G3dGeBufferOP_N(GX_PACK_OP(G3OP_MTX_MODE, G3OP_MTX_LOAD_4x4, G3OP_NOP, G3OP_NOP),
                        (u32*)&NNS_G3dGlb.mtxmode_proj,
                        (sizeof(NNS_G3dGlb.mtxmode_proj) +
                         sizeof(NNS_G3dGlb.projMtx)) / 4);

    // Projection�s��ɃJ�����s���������
    NNS_G3dGeBufferOP_N((G3OP_MTX_MULT_4x3 << 0),
                        (u32*)&NNS_G3dGlb.cameraMtx,
                        sizeof(NNS_G3dGlb.cameraMtx) / 4);

    // Projection�s��Ƀx�[�X�s���������
    NNS_G3dGeBufferOP_N(GX_PACK_OP(G3OP_MTX_MULT_4x3,G3OP_MTX_SCALE, G3OP_NOP, G3OP_NOP),
                        (u32*)&NNS_G3dGlb.prmBaseRot,
                        (sizeof(NNS_G3dGlb.prmBaseRot) +
                         sizeof(NNS_G3dGlb.prmBaseTrans)   +
                         sizeof(NNS_G3dGlb.prmBaseScale)) / 4);

    NNS_G3dGeMtxMode(GX_MTXMODE_POSITION_VECTOR);

    // ���C�g�x�N�g���ɂ̓x�[�X��]�s��̋t�s���������(���[���h���烍�[�J���ɕϊ��j
    {
        MtxFx43 inv;
        int result;
        result = MTX_Inverse33(&NNS_G3dGlb.prmBaseRot ,(MtxFx33*)&inv);
        NNS_G3D_ASSERT(!result);
        NNS_G3dGeLoadMtx43(&inv);
    }

    NNS_G3dGeBufferData_N((u32*)&NNS_G3dGlb.cmd1, sz);

    NNS_G3dGeIdentity();

    NNS_G3dGeBufferOP_N(G3OP_TEXIMAGE_PARAM, &NNS_G3dGlb.prmTexImageParam, 1);

    NNS_G3dGlb.flag |= NNS_G3D_GLB_FLAG_FLUSH_WVP;
    NNS_G3dGlb.flag &= ~NNS_G3D_GLB_FLAG_FLUSH_VP;

    //
    // �R�}���h�̏������POSITION/VECTOR���[�h�ɂȂ��Ă���B
    //
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbSetBaseTrans

    NNS_G3dGlb�̃x�[�X�s��i���[���h�ϊ��s��j��Translation��ݒ肷��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbSetBaseTrans(const VecFx32* pTrans)
{
    NNS_G3D_NULL_ASSERT(pTrans);

    if (pTrans)
    {
        NNS_G3dGlb.prmBaseTrans = *pTrans;
        NNS_G3dGlb.flag &= ~(NNS_G3D_GLB_FLAG_INVBASE_UPTODATE       |
                             NNS_G3D_GLB_FLAG_INVBASECAMERA_UPTODATE |
                             NNS_G3D_GLB_FLAG_BASECAMERA_UPTODATE);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbSetBaseScale

    NNS_G3dGlb�̃x�[�X�s��i���[���h�ϊ��s��j��Scale��ݒ肷��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbSetBaseScale(const VecFx32* pScale)
{
    NNS_G3D_NULL_ASSERT(pScale);

    if (pScale)
    {
        NNS_G3dGlb.prmBaseScale = *pScale;
        NNS_G3dGlb.flag &= ~(NNS_G3D_GLB_FLAG_INVBASE_UPTODATE      |
                            NNS_G3D_GLB_FLAG_INVBASECAMERA_UPTODATE |
                            NNS_G3D_GLB_FLAG_BASECAMERA_UPTODATE);
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbLightVector

    NNS_G3dGlb�̃��C�g�x�N�g����ύX����B
    �W�I���g���G���W���ɂ́ANNS_G3dGlbFlush���ĂԂ܂Ŕ��f����Ȃ��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbLightVector(GXLightId lightID,
                      fx16 x,
                      fx16 y,
                      fx16 z)
{
    GX_LIGHTID_ASSERT(lightID);

    NNS_G3D_ASSERT(x >= -FX16_ONE && x < FX16_ONE);
    NNS_G3D_ASSERT(y >= -FX16_ONE && y < FX16_ONE);
    NNS_G3D_ASSERT(z >= -FX16_ONE && z < FX16_ONE);

    NNS_G3dGlb.lightVec[lightID] = GX_PACK_LIGHTVECTOR_PARAM(lightID, x, y, z);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbLightColor

    NNS_G3dGlb�̃��C�g�J���[��ݒ肷��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbLightColor(GXLightId lightID, GXRgb rgb)
{
    GX_LIGHTID_ASSERT(lightID);
    GXRGB_ASSERT(rgb);

    NNS_G3dGlb.lightColor[lightID] = GX_PACK_LIGHTCOLOR_PARAM(lightID, rgb);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbMaterialColorDiffAmb

    NNS_G3dGlb��diffuse��ambient��ݒ肷��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbMaterialColorDiffAmb(GXRgb diffuse,
                               GXRgb ambient,
                               BOOL  IsSetVtxColor)
{
    GXRGB_ASSERT(diffuse);
    GXRGB_ASSERT(ambient);

    NNS_G3dGlb.prmMatColor0 = GX_PACK_DIFFAMB_PARAM(diffuse, ambient, IsSetVtxColor);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbMaterialColorSpecEmi

    NNS_G3dGlb��specular��emission��ݒ肷��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbMaterialColorSpecEmi(GXRgb specular,
                               GXRgb emission,
                               BOOL IsShininess)
{
    GXRGB_ASSERT(specular);
    GXRGB_ASSERT(emission);

    NNS_G3dGlb.prmMatColor1 = GX_PACK_SPECEMI_PARAM(specular, emission, IsShininess);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbPolygonAttr

    NNS_G3dGlb��PolygonAttr��ݒ肷��B
 *---------------------------------------------------------------------------*/
void 
NNS_G3dGlbPolygonAttr(int light,
                      GXPolygonMode polyMode,
                      GXCull cullMode,
                      int polygonID,
                      int alpha,
                      int misc // GXPolygonAttrMisc
                      )
{
    GX_LIGHTMASK_ASSERT(light);
    GX_POLYGONMODE_ASSERT(polyMode);
    GX_CULL_ASSERT(cullMode);
    GX_POLYGON_ATTR_POLYGONID_ASSERT(polygonID);
    GX_POLYGON_ATTR_ALPHA_ASSERT(alpha);

    NNS_G3dGlb.prmPolygonAttr = GX_PACK_POLYGONATTR_PARAM(light,
                                                          polyMode,
                                                          cullMode,
                                                          polygonID,
                                                          alpha,
                                                          misc);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbGetInvW

    ���f�����O�s��̋t�s��ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
const MtxFx43*
NNS_G3dGlbGetInvW(void)
{
    if (!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_INVBASE_UPTODATE))
    {
        MtxFx43 tmp;
        s32 result;
        MTX_ScaleApply43((MtxFx43*)&NNS_G3dGlb.prmBaseRot,
                         &tmp,
                         NNS_G3dGlb.prmBaseScale.x,
                         NNS_G3dGlb.prmBaseScale.y,
                         NNS_G3dGlb.prmBaseScale.z);
        result = MTX_Inverse43(&tmp, &NNS_G3dGlb.invBaseMtx);
        NNS_G3D_ASSERT(!result);
        NNS_G3dGlb.flag |= NNS_G3D_GLB_FLAG_INVBASE_UPTODATE;
    }
    return &NNS_G3dGlb.invBaseMtx;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbGetInvV

    �J�����s��̋t�s��ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
const MtxFx43*
NNS_G3dGlbGetInvV(void)
{
    if (!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_INVCAMERA_UPTODATE))
    {
        s32 result = MTX_Inverse43(&NNS_G3dGlb.cameraMtx, &NNS_G3dGlb.invCameraMtx);
        NNS_G3D_ASSERT(!result);
        NNS_G3dGlb.flag |= NNS_G3D_GLB_FLAG_INVCAMERA_UPTODATE;
    }
    return &NNS_G3dGlb.invCameraMtx;
}


static int mtx_inverse44(const MtxFx44 * pSrc, MtxFx44 * pDst)
{
    MtxFx44 gjm;
    int i, j, k;
    fx64c ww;
    fx32 w;

    SDK_NULL_ASSERT(pSrc);
    SDK_NULL_ASSERT(pDst);

    MTX_Copy44(pSrc, &gjm);
    MTX_Identity44(pDst);

    for (i = 0; i < 4; ++i)
    {
        fx32 max = 0;
        int swp = i;

        // ---- partial pivoting -----
        for( k = i ; k < 4 ; k++ )
        {
            fx32 ftmp = (gjm.m[k][i] < 0) ? -gjm.m[k][i] : gjm.m[k][i];

            if ( ftmp > max )
            {
                max = ftmp;
                swp = k;
            }
        }

        // check singular matrix
        //(or can't solve inverse matrix with this algorithm)
        if ( max == 0 )
        {
            return -1;
        }

        // swap row
        if( swp != i )
        {
            for ( k = 0 ; k < 4 ; k++ )
            {
                fx32 tmp;
                tmp = gjm.m[i][k];
                gjm.m[i][k] = gjm.m[swp][k];
                gjm.m[swp][k] = tmp;

                tmp = pDst->m[i][k];
                pDst->m[i][k] = pDst->m[swp][k];
                pDst->m[swp][k] = tmp;
            }
        }

        // ---- pivoting end ----
        ww = FX_InvFx64c(gjm.m[i][i]);
        for ( j = 0 ; j < 4 ; ++j )
        {
            gjm.m[i][j] = FX_Mul32x64c(gjm.m[i][j], ww);
            pDst->m[i][j] = FX_Mul32x64c(pDst->m[i][j], ww);
        }
        
        for ( k = 0 ; k < 4 ; ++k )
        {
            if ( k == i )
                continue;
            
            w = gjm.m[k][i];
            for ( j = 0 ; j < 4 ; ++j )
            {
                gjm.m[k][j]   -= (fx32)(((fx64)w * gjm.m[i][j]) >> FX32_SHIFT);
                pDst->m[k][j] -= (fx32)(((fx64)w * pDst->m[i][j]) >> FX32_SHIFT);
            }
        }
    }
    return 0;
}




/*---------------------------------------------------------------------------*
    NNS_G3dGlbGetInvP

    �ˉe�ϊ��s��̋t�s��ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
const MtxFx44*
NNS_G3dGlbGetInvP(void)
{
    if (!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_INVPROJ_UPTODATE))
    {
        s32 result = mtx_inverse44(&NNS_G3dGlb.projMtx, &NNS_G3dGlb.invProjMtx);
        NNS_G3D_ASSERT(!result);
        NNS_G3dGlb.flag |= NNS_G3D_GLB_FLAG_INVPROJ_UPTODATE;
    }
    return &NNS_G3dGlb.invProjMtx;
}



static void
calcSrtCameraMtx_(void)
{
    s32 result;

    MTX_Concat43((MtxFx43*)&NNS_G3dGlb.prmBaseRot,
                 &NNS_G3dGlb.cameraMtx,
                 &NNS_G3dGlb.srtCameraMtx);
    MTX_ScaleApply43(&NNS_G3dGlb.srtCameraMtx,
                     &NNS_G3dGlb.srtCameraMtx,
                     NNS_G3dGlb.prmBaseScale.x,
                     NNS_G3dGlb.prmBaseScale.y,
                     NNS_G3dGlb.prmBaseScale.z);
    result = MTX_Inverse43(&NNS_G3dGlb.srtCameraMtx,
                           &NNS_G3dGlb.invSrtCameraMtx);
    NNS_G3D_ASSERT(!result);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbGetWV

    �J�����s���BaseSRT�s����������s��ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
const MtxFx43*
NNS_G3dGlbGetWV(void)
{
    if (!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_SRTCAMERA_UPTODATE))
    {
        calcSrtCameraMtx_();
        NNS_G3dGlb.flag |= NNS_G3D_GLB_FLAG_SRTCAMERA_UPTODATE;
    }
    
    return &NNS_G3dGlb.srtCameraMtx;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGlbGetInvWV

    �J�����s���BaseSRT�s����������s��̋t�s��ւ̃|�C���^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
const MtxFx43*
NNS_G3dGlbGetInvWV(void)
{
    if (!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_SRTCAMERA_UPTODATE))
    {
        calcSrtCameraMtx_();
        NNS_G3dGlb.flag |= NNS_G3D_GLB_FLAG_SRTCAMERA_UPTODATE;
    }
    
    return &NNS_G3dGlb.invSrtCameraMtx;
}


const MtxFx44*
NNS_G3dGlbGetInvVP(void)
{
    if (!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_INVCAMERAPROJ_UPTODATE))
    {
        MtxFx44 tmp;
        const MtxFx43* invCam  = NNS_G3dGlbGetInvV();
        const MtxFx44* invProj = NNS_G3dGlbGetInvP();
        
        MTX_Copy43To44(invCam, &tmp);
        MTX_Concat44(invProj, &tmp, &NNS_G3dGlb.invCameraProjMtx);

        NNS_G3dGlb.flag |= NNS_G3D_GLB_FLAG_INVCAMERAPROJ_UPTODATE;
    }
    return &NNS_G3dGlb.invCameraProjMtx;
}




/*---------------------------------------------------------------------------*
    NNS_G3dGlbGetViewPort

    NNS_G3dGlb�ɐݒ肵�Ă���r���[�|�[�g���W����肾���܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGlbGetViewPort(int* px1, int* py1, int* px2, int* py2)
{
    if (px1)
        *px1 = (int)( NNS_G3dGlb.prmViewPort & 0xff );
    if (py1)
        *py1 = (int)( (NNS_G3dGlb.prmViewPort >> 8) & 0xff );
    if (px2)
        *px2 = (int)( (NNS_G3dGlb.prmViewPort >> 16) & 0xff );
    if (py2)
        *py2 = (int)( (NNS_G3dGlb.prmViewPort >> 24) & 0xff );
}







////////////////////////////////////////////////////////////////////////////////
//
// �O���[�o���ϐ�
//

/*---------------------------------------------------------------------------*
    NNS_G3dGlb

    �J�����E���C�g����G3D�Ŏg�p����O���[�o����Ԃ��i�[���邽�߂̍\���́B
 *---------------------------------------------------------------------------*/
NNSG3dGlb NNS_G3dGlb;


/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_PolAttr_FARClip
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 10:34:33  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.8  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.7  2004/11/02 08:23:24  takano_makoto
  fix comment.

  Revision 1.6  2004/09/14 01:50:40  yasu
  Fix backslashs at end of line.

  Revision 1.5  2004/05/27 09:24:52  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:01:25  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:09:15  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that set GX_POLYGON_ATTR_MISC_FAR_CLIPP in G3_PolygonAttr().
//
// This sample displays two cubes.
// The left one is normal setting and the right one is used far clipping.
// If polygon cross the far face, the right one is clipped.
//
// USAGE:
//   UP, DOWN  : Move z position of the object.
//   START     : Reset position of the object.
//
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�����̂̃��f���f�[�^
//---------------------------------------------------------------------------
/*  */
/* ���_�f�[�^ */
static s16 s_Vertex[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};

/* �@���f�[�^ */
static VecFx10 s_Normal[6] = {
    GX_VECFX10(0, 0, FX32_ONE - 1),
    GX_VECFX10(0, FX32_ONE - 1, 0),
    GX_VECFX10(FX32_ONE - 1, 0, 0),
    GX_VECFX10(0, 0, -FX32_ONE + 1),
    GX_VECFX10(0, -FX32_ONE + 1, 0),
    GX_VECFX10(-FX32_ONE + 1, 0, 0)
};

/* �e�N�X�`�����W */
static GXSt s_TextureCoord[] = {
    GX_ST(0, 0),
    GX_ST(0, 64 * FX32_ONE),
    GX_ST(64 * FX32_ONE, 0),
    GX_ST(64 * FX32_ONE, 64 * FX32_ONE)
};

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@���_���W��ݒ�
//  �����F
//  �@�@�@�w��̒��_�f�[�^�Œ��_���W��ݒ肷��B
//  ���́F
//        i_idx : ���_�f�[�^�̂h�c
//---------------------------------------------------------------------------
inline void Vertex(int i_idx)
{
    G3_Vtx(s_Vertex[i_idx * 3],        // ���_���W��ݒ�
           s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�@���̐ݒ�
//  �����F
//  �@�@�@
//  ���́F
//        i_idx : �@���f�[�^�̂h�c
//---------------------------------------------------------------------------
inline void Normal(int i_idx)
{
    G3_Direct1(G3OP_NORMAL, s_Normal[i_idx]);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�e�N�X�`�����W�̐ݒ�
//  �����F
//
//  ���́F
//        i_idx : �e�N�X�`���f�[�^�̂h�c
//---------------------------------------------------------------------------
inline void TextureCoord(int i_idx)
{
    G3_Direct1(G3OP_TEXCOORD, s_TextureCoord[i_idx]);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�����̂̕`��
//---------------------------------------------------------------------------
static void DrawCube(void)
{
    G3_Begin(GX_BEGIN_QUADS);          // ���_���X�g�̊J�n(�l�p�`�|���S���ŕ`��)
    {
        TextureCoord(1);
        Normal(0);
        Vertex(2);
        TextureCoord(0);
        Normal(0);
        Vertex(0);
        TextureCoord(2);
        Normal(0);
        Vertex(4);
        TextureCoord(3);
        Normal(0);
        Vertex(6);

        TextureCoord(1);
        Normal(3);
        Vertex(7);
        TextureCoord(0);
        Normal(3);
        Vertex(5);
        TextureCoord(2);
        Normal(3);
        Vertex(1);
        TextureCoord(3);
        Normal(3);
        Vertex(3);

        TextureCoord(1);
        Normal(5);
        Vertex(6);
        TextureCoord(0);
        Normal(5);
        Vertex(4);
        TextureCoord(2);
        Normal(5);
        Vertex(5);
        TextureCoord(3);
        Normal(5);
        Vertex(7);

        TextureCoord(1);
        Normal(2);
        Vertex(3);
        TextureCoord(0);
        Normal(2);
        Vertex(1);
        TextureCoord(2);
        Normal(2);
        Vertex(0);
        TextureCoord(3);
        Normal(2);
        Vertex(2);

        TextureCoord(1);
        Normal(1);
        Vertex(5);
        TextureCoord(0);
        Normal(1);
        Vertex(4);
        TextureCoord(2);
        Normal(1);
        Vertex(0);
        TextureCoord(3);
        Normal(1);
        Vertex(1);

        TextureCoord(1);
        Normal(4);
        Vertex(6);
        TextureCoord(0);
        Normal(4);
        Vertex(7);
        TextureCoord(2);
        Normal(4);
        Vertex(3);
        TextureCoord(3);
        Normal(4);
        Vertex(2);
    }

    G3_End();                          // ���_���X�g�̏I��
}


//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�u�u�����N���荞�ݏ���
//  �����F
//  �@�@�@�u�u�����N���荞�݂��s��ꂽ���Ƃ��m�F���邽�߂̃t���O�𗧂Ă܂��B
//
//        ���ʏ������iDEMOInitCommon()�j�ŁA
//        �E�h�q�p���荞�݂̑I��(OS_SetIrqMask())�B
//        �E�h�q�p���荞�݂��s�����̊֐��̓o�^(OS_SetIRQFunction())�B
//        �E�h�q�p���荞�݂̋��̐ݒ�(OS_EnableIrq())�B
//        ���s���邱�ƂŁA���̊֐����u�u�����N�̂Ƃ��ɌĂ΂�܂��B
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // VBlank ���荞�݂̊m�F�t���O��ݒ�
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@G3_PolygonAttr()��FAR�ʌ����|���S���̃N���b�s���O
//  �����F
//          �����ɕ\������Ă��闧���̂́AFAR�ʃN���b�s���O���K�p����Ă��Ȃ�
//          �\���ŁA�E���ɕ\������Ă��闧���̂�FAR�ʃN���b�s���O���K�p�����B
//          ���̐ς�far�l���A�ʏ�̎��̐ϐݒ�l�ɔ�ׂĔ��ɏ������Ȃ��Ă���B
//          ����́A�N���b�s���O�������s���Ă���̂��ԋ߂Ɍ��邽�߂ł���B
//  ����F
//        UP,DOWN: �I�u�W�F�N�g��Z���W�ʒu�𑀍삷��B
//        START  : �����ʒu�ɖ߂��B
//---------------------------------------------------------------------------
void NitroMain(void)
{
    int     pos_z = -5;
    u16     rotate = 0;
    u32     texAddr = 0x01000;         // �e�N�X�`���C���[�W�E�X���b�g�̃A�h���X

    //---------------------
    // ������(IRQ�̋��A�u�q�`�l�̏������A�a�f�O���R�c���[�h�Ŏg�p)
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    /* �e�N�X�`���C���[�W���e�N�X�`���C���[�W�E�X���b�g�ւ̓ǂݍ��� */
    GX_BeginLoadTex();                 // �X���b�g���蓖�Ẵo���N��LCDC�������Ƀ}�b�v
    {
        GX_LoadTex((void *)&tex_32768_64x64[0], // �ǂݍ��݌��̃|�C���^�[
                   texAddr,            // �ǂݍ��ݐ�X���b�g�̃A�h���X
                   8192);              // �ǂݍ��݃T�C�Y
    }
    GX_EndLoadTex();                   // LCDC�Ƀ}�b�v�����X���b�g�����ɖ߂�

    /* �����ˉe�̐ݒ� */
    {
        fx32    right = FX32_ONE;
        fx32    top = FX32_ONE * 3 / 4;
        fx32    near = FX32_ONE;
        fx32    far = FX32_ONE * 6;
        // �����ˉe�̐ݒ�
        G3_Perspective(FX32_SIN30,     // Sine   FOVY
                       FX32_COS30,     // Cosine FOVY
                       FX32_ONE * 4 / 3,        // Aspect
                       near,           // Near
                       far,            // Far
                       NULL);

        G3_StoreMtx(0);                // �s����X�^�b�N�O�ԂɊi�[
    }

    DEMOStartDisplay();

    //---------------------
    //  ���C�����[�v
    //---------------------
    while (1)
    {
        G3X_Reset();
        rotate += 256;

        /* �p�b�h���͏��� */
        DEMOReadKey();

        if (DEMO_IS_TRIG(PAD_KEY_DOWN))
        {
            if (++pos_z > -3)
            {
                pos_z = -3;
            }
            OS_Printf("Pos_Z=%d\n", pos_z);
        }
        else if (DEMO_IS_TRIG(PAD_KEY_UP))
        {
            if (--pos_z < -7)
            {
                pos_z = -7;
            }
            OS_Printf("Pos_Z=%d\n", pos_z);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            pos_z = -5;
            OS_Printf("Pos_Z=%d\n", pos_z);
        }

        /* �J�����̐ݒ� */
        {
            VecFx32 Eye = { 0, 0, FX32_ONE };   // ���_�ʒu
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // ���_�ݒ�
        }

        /* ���C�g�̐ݒ�(���C�g�̐F�ƌ�����ݒ�) */
        {
            G3_LightVector(GX_LIGHTID_0, 0, 0, -FX32_ONE + 1);
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // �s���Texture���[�h�ɐݒ�
        G3_Identity();                 // ���݂̍s���P�ʍs��ɏ�����
        // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        G3_PushMtx();                  // �s����X�^�b�N�֓����

        /* �E�\�������̂̉�]�v�Z�ƈʒu�ݒ� */
        {
            fx16    s = FX_SinIdx(rotate);
            fx16    c = FX_CosIdx(rotate);

            G3_Translate(FX32_ONE * 2, 0, pos_z * FX32_ONE);
            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        /* �E�\�������̂̕`��ݒ� */
        // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // �g�U���ːF
                                GX_RGB(16, 16, 16),     // �����ːF
                                TRUE); // ���_�J���[��ݒ�
        // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // ���ʔ��ːF
                                GX_RGB(0, 0, 0),        // ���ˌ��F
                                FALSE); // ���ʔ��˂𖢎g�p

        // �e�N�X�`���̃p�����[�^��ݒ�
        G3_TexImageParam(GX_TEXFMT_DIRECT,      // �_�C���N�g�e�N�X�`�����g�p
                         GX_TEXGEN_TEXCOORD,    // �e�N�X�`�����W�ϊ����[�h�̐ݒ�
                         GX_TEXSIZE_S64,        // 64 �e�N�X�`�����T�C�Y
                         GX_TEXSIZE_T64,        // 64 �e�N�X�`�����T�C�Y
                         GX_TEXREPEAT_NONE,     // ���s�[�g����
                         GX_TEXFLIP_NONE,       // �t���b�v����
                         GX_TEXPLTTCOLOR0_USE,  // �p���b�g�̃J���[0�ݒ�l���L��
                         texAddr);     // �e�N�X�`���̃A�h���X
        // �|���S�������̐ݒ�
        G3_PolygonAttr(GX_LIGHTMASK_0, // ���C�g�O���I��
                       GX_POLYGONMODE_MODULATE, // ���W�����[�V�������[�h
                       GX_CULL_NONE,   // �J�����O���s��Ȃ�
                       0,              // �|���S���h�c �O
                       31,             // �A���t�@�l
                       GX_POLYGON_ATTR_MISC_FAR_CLIPPING);

        DrawCube();                    // �����̂̕`��

        G3_PopMtx(1);

        G3_PushMtx();
        /* ���\�������̂̉�]�v�Z�ƈʒu�ݒ� */
        {
            fx16    s = FX_SinIdx(rotate);
            fx16    c = FX_CosIdx(rotate);

            G3_Translate(-FX32_ONE * 2, 0, pos_z * FX32_ONE);

            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // �s���Texture���[�h�ɐݒ�
        G3_Identity();                 // ���݂̍s���P�ʍs��ɏ�����
        // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        /* ���\�������̂̕`��ݒ� */
        // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // �g�U���ːF
                                GX_RGB(16, 16, 16),     // �����ːF
                                TRUE); // ���_�J���[��ݒ�
        // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // ���ʔ��ːF
                                GX_RGB(0, 0, 0),        // ���ˌ��F
                                FALSE); // ���ʔ��˂𖢎g�p
        // �e�N�X�`���̃p�����[�^��ݒ�
        G3_TexImageParam(GX_TEXFMT_DIRECT,      // �_�C���N�g�e�N�X�`�����g�p
                         GX_TEXGEN_TEXCOORD,    // �e�N�X�`�����W�ϊ����[�h�̐ݒ�
                         GX_TEXSIZE_S64,        // 64 �e�N�X�`�����T�C�Y
                         GX_TEXSIZE_T64,        // 64 �e�N�X�`�����T�C�Y
                         GX_TEXREPEAT_NONE,     // ���s�[�g����
                         GX_TEXFLIP_NONE,       // �t���b�v����
                         GX_TEXPLTTCOLOR0_USE,  // �p���b�g�̃J���[0�ݒ�l���L��
                         texAddr);     // �e�N�X�`���̃A�h���X
        G3_PolygonAttr(GX_LIGHTMASK_0, // ���C�g�O���I��
                       GX_POLYGONMODE_MODULATE, // ���W�����[�V�������[�h
                       GX_CULL_NONE,   // �J���o�b�N���s��Ȃ�
                       0,              // �|���S���h�c �O
                       31,             // �A���t�@�l
                       GX_POLYGON_ATTR_MISC_NONE);

        DrawCube();                    // �����̂̕`��

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        /* VBlank�҂� */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x9F1292DC);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}

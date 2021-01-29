/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_PolAttr_1Dot
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.9  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 10:33:52  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.7  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.6  2004/07/27 08:02:40  yasu
  Avoid warning of padding

  Revision 1.5  2004/05/27 09:24:21  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:00:17  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:07:03  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that control the display of 1dot polygon by depth value.
// 
// This sample displays three cubes, and change scale and depth by pad.
// If cubes looks less than 1dot, set visible or unvisible by press A.
// 
// USAGE:
//   UP, DOWN   : Change the limit depth value that 1dot polygon is visible.
//   L, R       : Change the scale of object.
//   A          : Switch ON/OFF that control visible of 1dot polygon.
//   
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�����̂̃��f���f�[�^
//---------------------------------------------------------------------------
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
    G3_Vtx(s_Vertex[i_idx * 3], s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@���_�J���[��ݒ�
//---------------------------------------------------------------------------
inline void Color(void)
{
    G3_Color(GX_RGB(31, 31, 31));
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
//        �����̂̕`����̌^
//  �����F
//---------------------------------------------------------------------------
#pragma  warn_padding off
typedef struct
{
    int     trg;
    fx32    scale;
    fx32    depth;
    u32     texAddr;
    u16     rotate;
    // [PADDING 2 BYTE HERE]
}
cubu_c;
#pragma  warn_padding reset


//---------------------------------------------------------------------------
//  �T���F
//        �����̂̕`��
//  �����F
//
//  ���́F
//        *p : �����̏��̃|�C���^�[
//        x  : �w�ʒu
//        y  : �x�ʒu
//        z  : �y�ʒu
//---------------------------------------------------------------------------
static void Cube(cubu_c * p, fx32 x, fx32 y, fx32 z)
{
    G3_PushMtx();

    /* �����̂̉�]�ƈʒu�ݒ� */
    {
        fx16    s = FX_SinIdx(p->rotate);
        fx16    c = FX_CosIdx(p->rotate);

        G3_Translate(x, y, z);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);

        G3_Scale(p->scale, p->scale, p->scale);
    }

    /* �����̂̕`��ݒ� */
    // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // �g�U���ːF
                            GX_RGB(16, 16, 16), // �����ːF
                            TRUE);     // ���_�J���[��ݒ�
    // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // ���ʔ��ːF
                            GX_RGB(0, 0, 0),    // ���ˌ��F
                            FALSE);    // ���ʔ��˂𖢎g�p
    // �|���S�������̐ݒ�

    // �e�N�X�`���̃p�����[�^��ݒ�
    G3_TexImageParam(GX_TEXFMT_DIRECT, // �_�C���N�g�e�N�X�`�����g�p
                     GX_TEXGEN_TEXCOORD,        // �e�N�X�`�����W�ϊ����[�h�̐ݒ�
                     GX_TEXSIZE_S64,   // 64 �e�N�X�`�����T�C�Y
                     GX_TEXSIZE_T64,   // 64 �e�N�X�`�����T�C�Y
                     GX_TEXREPEAT_NONE, // ���s�[�g����
                     GX_TEXFLIP_NONE,  // �t���b�v����
                     GX_TEXPLTTCOLOR0_USE,      // �p���b�g�̃J���[0�ݒ�l���L��
                     p->texAddr);      // �e�N�X�`���̃A�h���X

    {
        int     attr;

        if (p->trg)
        {
            attr = GX_POLYGON_ATTR_MISC_DISP_1DOT;
        }
        else
        {
            attr = GX_POLYGON_ATTR_MISC_NONE;
        }
        // �|���S�������̐ݒ�
        G3_PolygonAttr(GX_LIGHTMASK_NONE,       // ���C�g�𔽉f���Ȃ�
                       GX_POLYGONMODE_MODULATE, // ���W�����[�V�������[�h
                       GX_CULL_NONE,   // �J�����O����
                       0,              // �|���S��ID 0
                       31,             // �A���t�@�l
                       attr);          // 1�h�b�g�|���S���̕\���ݒ�
    }
    // 
    G3X_SetDisp1DotDepth(p->depth);    // 1�h�b�g�|���S���\�����E�f�v�X�l

    G3_Begin(GX_BEGIN_QUADS);          // ���_���X�g�̊J�n(�l�p�`�|���S���ł̕`��)
    {
        TextureCoord(1);
        Color();
        Vertex(2);
        TextureCoord(0);
        Color();
        Vertex(0);
        TextureCoord(2);
        Color();
        Vertex(4);
        TextureCoord(3);
        Color();
        Vertex(6);

        TextureCoord(1);
        Color();
        Vertex(7);
        TextureCoord(0);
        Color();
        Vertex(5);
        TextureCoord(2);
        Color();
        Vertex(1);
        TextureCoord(3);
        Color();
        Vertex(3);

        TextureCoord(1);
        Color();
        Vertex(6);
        TextureCoord(0);
        Color();
        Vertex(4);
        TextureCoord(2);
        Color();
        Vertex(5);
        TextureCoord(3);
        Color();
        Vertex(7);

        TextureCoord(1);
        Color();
        Vertex(3);
        TextureCoord(0);
        Color();
        Vertex(1);
        TextureCoord(2);
        Color();
        Vertex(0);
        TextureCoord(3);
        Color();
        Vertex(2);

        TextureCoord(1);
        Color();
        Vertex(5);
        TextureCoord(0);
        Color();
        Vertex(4);
        TextureCoord(2);
        Color();
        Vertex(0);
        TextureCoord(3);
        Color();
        Vertex(1);

        TextureCoord(1);
        Color();
        Vertex(6);
        TextureCoord(0);
        Color();
        Vertex(7);
        TextureCoord(2);
        Color();
        Vertex(3);
        TextureCoord(3);
        Color();
        Vertex(2);
    }
    G3_End();                          // ���_���X�g�̏I��

    G3_PopMtx(1);
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
//        G3_PolygonAttr()��GX_POLYGON_ATTR_MISC_DISP_1DOT�ݒ�
//�@�@�@�@(�f�v�X�l�ɂ��P�h�b�g�|���S���̕\���}��)
//  �����F
//        �f�v�X�l�ɂ���ĂP�h�b�g�|���S���̕\���}�����s���B
//
//        1. �T���v�����n�܂�����BUTTON_L�������ăI�u�W�F�N�g�̃X�P�[���l��0��
//         ����B
//        2. �X�P�[���l��0�ɂȂ�A3�̃I�u�W�F�N�g��1�h�b�g�ŕ\������Ă���B
//        3. �\���L�[�̏㉺�������ăf�v�X�l�𑝌������邱�Ƃɂ�艜�ɕ\�������
//         ���鍶���̃I�u�W�F�N�g���珇�ɓ_�ŕ\�������B
//  ����F
//        BUTTON_L,R: �I�u�W�F�N�g�̃X�P�[���l
//        UP,DOWN   : �I�u�W�F�N�g�̃f�v�X�l�̑���
//        START     : �����l�֖߂�
//        SELECT    : �|�W�V�����e�X�g�ŁA�f�v�X�l���߂��l�ɍX�V����
//---------------------------------------------------------------------------
void NitroMain(void)
{
    cubu_c  cubu;

    cubu.trg = 1;
    cubu.scale = FX32_ONE;
    cubu.depth = 16384;
    cubu.texAddr = 0x01000;            // �e�N�X�`���C���[�W�E�X���b�g�̃A�h���X
    cubu.rotate = 0;

    /*
     * ������(IRQ�̋��A�u�q�`�l�̏������A�a�f�O���R�c���[�h�Ŏg�p)
     */
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    /* �e�N�X�`���C���[�W���e�N�X�`���C���[�W�E�X���b�g�ւ̓ǂݍ��� */
    GX_BeginLoadTex();                 // �X���b�g���蓖�Ẵo���N��LCDC�������Ƀ}�b�v
    {
        GX_LoadTex((void *)&tex_32768_64x64[0], // �ǂݍ��݌��̃|�C���^�[
                   cubu.texAddr,       // �ǂݍ��ݐ�X���b�g�̃A�h���X
                   8192);              // �ǂݍ��݃T�C�Y
    }
    GX_EndLoadTex();                   // LCDC�Ƀ}�b�v�����X���b�g�����ɖ߂�

    DEMOStartDisplay();

    while (1)
    {
        G3X_Reset();
        cubu.rotate += 256;

        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] =
                { {PAD_BUTTON_L, 80}, {PAD_KEY_DOWN, 8}, {PAD_BUTTON_A, 1}, {0, 0} };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            cubu.scale += 64;
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_L))
        {
            cubu.scale -= 64;
            if (cubu.scale < 0)
            {
                cubu.scale = 0;
            }
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            cubu.depth += 256;
            if (cubu.depth > 0xffffff)
            {
                cubu.depth = 0xffffff;
            }
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            cubu.depth -= 256;
            if (cubu.depth < 0)
            {
                cubu.depth = 0;
            }
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_A))
        {
            cubu.trg = (cubu.trg + 1) & 0x01;
            OS_Printf("PolygonAttr_1DotPoly=%s\n", (cubu.trg ? "ON" : "OFF"));
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            cubu.scale = FX32_ONE;
            cubu.depth = 16384;
            OS_Printf("Scale=%d Depth=%d\n", cubu.scale, cubu.depth);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_SELECT))
        {
            fx16    s = FX_SinIdx(cubu.rotate);
            fx16    c = FX_CosIdx(cubu.rotate);

            G3_Translate(-FX32_ONE, 0, -4 * FX32_ONE);

            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);

            G3_Scale(cubu.scale, cubu.scale, cubu.scale);

            {
                VecFx32 m;
                fx32    w;

                G3_PositionTest(FX16_ONE, FX16_ONE, FX16_ONE);
                while (G3X_GetPositionTestResult(&m, &w))
                {
                    // �|�W�V�����e�X�g�҂�
                }

                OS_Printf("Position Test : Pos(%d, %d, %d) W(%d)\n", m.x, m.y, m.z, w);
                cubu.depth = w;
            }
        }

        /* �J�����̐ݒ� */
        {
            VecFx32 Eye = { 0, 0, FX32_ONE };   // Eye position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // ���_�ݒ�
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // �s���Texture���[�h�ɐݒ�
        G3_Identity();                 // ���݂̍s���P�ʍs��ɏ�����
        // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        /* �����̂̕`�� */
        Cube(&cubu, FX32_ONE, 0, -2 * FX32_ONE);
        Cube(&cubu, 0, 0, -3 * FX32_ONE);
        Cube(&cubu, -FX32_ONE, 0, -4 * FX32_ONE);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        /* �u�a���������҂� */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(150, 0xA00DF599);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

    }
}

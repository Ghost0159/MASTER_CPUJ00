/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Pol_Vertex_Source
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2006/01/18 02:11:26  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 10:39:24  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.8  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.7  2004/11/02 08:23:12  takano_makoto
  fix comment.

  Revision 1.6  2004/05/27 09:24:07  takano_makoto
  Add Autotest code.

  Revision 1.5  2004/04/23 07:23:36  yada
  only fix comment

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:00:09  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:06:20  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that call G3_TexImageParam() with GX_TEXGEN_VERTEX mode.
// 
// This sample displays a cube having a constant viewpoint texture.
//---------------------------------------------------------------------------


#include <nitro.h>
#include "DEMO.h"
#include "data.h"

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�����̂̃��f���f�[�^
//---------------------------------------------------------------------------
/* �e�N�X�`���s�� */
static MtxFx44 s_TextureMtx = {
    32 * FX32_ONE, 0, 0, 0,
    0, -32 * FX32_ONE, 0, 0,
    0, 0, FX32_ONE, 0,
    0, 0, 0, FX32_ONE
};
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
static VecFx10 s_Normal[8] = {
    GX_VECFX10(FX32_SQRT1_3, FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(FX32_SQRT1_3, FX32_SQRT1_3, -FX32_SQRT1_3),
    GX_VECFX10(FX32_SQRT1_3, -FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(FX32_SQRT1_3, -FX32_SQRT1_3, -FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, FX32_SQRT1_3, -FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, -FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, -FX32_SQRT1_3, -FX32_SQRT1_3)
};

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@���_���W��ݒ�
//  �����F
//  �@�@�@�w��̒��_�f�[�^�Œ��_���W��ݒ肷��B
//  ���́F
//        idx : ���_�f�[�^�̂h�c
//---------------------------------------------------------------------------
inline void Vertex(int idx)
{
    G3_Vtx(s_Vertex[idx * 3], s_Vertex[idx * 3 + 1], s_Vertex[idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�@���̐ݒ�
//  �����F
//  �@�@�@
//  ���́F
//        idx : �@���f�[�^�̂h�c
//---------------------------------------------------------------------------
inline void Normal(int idx)
{
    G3_Direct1(G3OP_NORMAL, s_Normal[idx]);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�����̖̂ʐ���
//  �����F
//  �@�@�@�����̂̂P�ʂ𐶐�����B
//  ���́F
//        idx0�`idx3 : �\�����钸�_�f�[�^�̂h�c
//---------------------------------------------------------------------------
static void Quad(int idx0, int idx1, int idx2, int idx3)
{
    Normal(idx0);
    Vertex(idx0);
    Normal(idx1);
    Vertex(idx1);
    Normal(idx2);
    Vertex(idx2);
    Normal(idx3);
    Vertex(idx3);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�����̂̕`��i�@�������j
//  �����F
//  �@�@�@
//---------------------------------------------------------------------------
static void DrawCube(void)
{
    G3_Begin(GX_BEGIN_QUADS);          // ���_���X�g�̊J�n(�l�p�`�|���S���ŕ`��
    {
        Quad(2, 0, 4, 6);
        Quad(7, 5, 1, 3);
        Quad(6, 4, 5, 7);
        Quad(3, 1, 0, 2);
        Quad(5, 4, 0, 1);
        Quad(6, 7, 3, 2);
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
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt
}

//---------------------------------------------------------------------------
//  �T���F
//        G3_TexImageParam()��GX_TEXGEN_VERTEX�ݒ�i���_�Œ�e�N�X�`���̕\���j
//  �����F
//        G3_TexImageParam()��GX_TEXGEN_VERTEX�ݒ���s���A�����̂Ƀe�N�X�`����
//      ����B���ʁA�����̂̉�]�Ɋւ�炸���_�ɑ΂��ČŒ肵�Č�����e�N�X�`��
//      ��������B
//
//        �e�N�X�`���s��ɑ���ꂽ���_�x�N�g�����A���[�J���ɂ��邱�Ƃɒ��ӁB
//        �]���āA�K�؂ȉ�]�s���O�����đ��₳�Ȃ���΂Ȃ�Ȃ��B
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u32     myTexAddr = 0x1000;        // �e�N�X�`���C���[�W�E�X���b�g�̃A�h���X
    u32     myTexPlttAddr = 0x1000;    // �e�N�X�`���p���b�g�E�X���b�g�̃A�h���X
    u16     Rotate = 0;

    //---------------------
    // ������(IRQ�̋��A�u�q�`�l�̏������A�a�f�O���R�c���[�h�Ŏg�p)
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    /* �e�N�X�`���C���[�W���e�N�X�`���C���[�W�E�X���b�g�֓ǂݍ��� */
    GX_BeginLoadTex();                 // �X���b�g���蓖�Ẵo���N��LCDC�������Ƀ}�b�v
    {
        GX_LoadTex((void *)&tex_4plett64x64[0], // �ǂݍ��݌��̃|�C���^�[
                   myTexAddr,          // �ǂݍ��ݐ�̃X���b�g�̃A�h���X
                   1024);              // �ǂݍ��݃T�C�Y
    }
    GX_EndLoadTex();                   // LCDC�Ƀ}�b�v�����X���b�g�����ɖ߂�

    /* �e�N�X�`���p���b�g�̃e�N�X�`���p���b�g�E�X���b�g�ւ̓ǂݍ��� */
    GX_BeginLoadTexPltt();             // �X���b�g���蓖�Ẵo���N��LCDC�������Ƀ}�b�v
    {
        GX_LoadTexPltt((void *)&pal_4plett[0],  // �ǂݍ��݌��̃|�C���^�[
                       myTexPlttAddr,  // �ǂݍ��ݐ�p���b�g�E�X���b�g�̃A�h���X
                       8);             // �ǂݍ��݃T�C�Y
    }
    GX_EndLoadTexPltt();               // LCDC�Ƀ}�b�v�����X���b�g�����ɖ߂�

    DEMOStartDisplay();

    //---------------------
    //  ���C�����[�v
    //---------------------
    while (1)
    {
        MtxFx43 camera;

        G3X_Reset();
        Rotate += 256;

        /* �J�����̐ݒ� */
        {
            VecFx32 Eye = { 0, 0, 10 * FX32_ONE };      // Eye position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up
            G3_LookAt(&Eye, &vUp, &at, &camera);
        }

        /* ���C�g�̐ݒ�
           (LightVector�R�}���h�̒���ɁA���C�g�s�񂪌��݂̍s��ɂ���ĕϊ���
           ��Ă��邱�Ƃɒ���) */
        {
            G3_LightVector(GX_LIGHTID_0, 0, -FX32_ONE + 1, 0);
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
        }

        G3_PushMtx();

        /* �����̂̉�]�v�Z�ƈʒu�ݒ� */
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_Scale(FX32_ONE * 3, FX32_ONE * 3, FX32_ONE * 3);
            G3_RotZ(s, c);
            G3_RotY(s, c);
            G3_RotX(s, c);
        }

        /* �����̂̕`��ݒ� */
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            /* �����̂ւ̊��}�b�s���O�̐ݒ�
               ���݂̃e�N�X�`���s��ɐݒ肳���O�ɁA�����̍s���(16,16,16)��
               �X�P�[������Ă���B
               G3_MtxMode(GX_MTXMODE_TEXTURE)��G3_LoadMtx44()���g�p����΁A�s
               ��������ŃX�P�[�����Ȃ���΂Ȃ�Ȃ��B */

            // �s����e�N�X�`�����[�h�ɂ��ăe�N�X�`���s���ݒ�
            G3_LoadTexMtxEnv(&s_TextureMtx);

            /* �����ŃJ�����s��𑝂₹�΁A�������@�Ńe�N�X�`����������B
               �ʏ�A�����ł̍s��͊����̕����̎w����s���B */
            // ���݂̍s��ɂR�~�R�̍s�����Z����
            G3_MultMtx33((MtxFx33 *)&camera);   // ���s��ɃJ�����s�����Z

            G3_RotZ(s, c);
            G3_RotY(s, c);
            G3_RotX(s, c);

            /* �����̂֒���e�N�X�`���̐ݒ� */
            // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
            G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

            // �e�N�X�`�����W�̐ݒ�
            G3_TexCoord(32 * FX32_ONE, 32 * FX32_ONE);  // 64x64�e�N�X�`���̒��S���w��
        }

        // �e�N�X�`���̃p�����[�^��ݒ�
        G3_TexImageParam(GX_TEXFMT_PLTT4,       // 4�F�p���b�g�e�N�X�`��
                         GX_TEXGEN_VERTEX,      // �o�[�e�b�N�X�\�[�X
                         GX_TEXSIZE_S64,        // 64 �e�N�X�`�����T�C�Y
                         GX_TEXSIZE_T64,        // 64 �e�N�X�`�����T�C�Y
                         GX_TEXREPEAT_NONE,     // ���s�[�g����
                         GX_TEXFLIP_NONE,       // �t���b�v����
                         GX_TEXPLTTCOLOR0_USE,  // �p���b�g�̃J���[0�ݒ�l���L��
                         myTexAddr);   // �e�N�X�`���̃A�h���X

        // �e�N�X�`���p���b�g�̃A�h���X��ݒ�
        G3_TexPlttBase(myTexPlttAddr,  // �e�N�X�`���p���b�g�̃A�h���X
                       GX_TEXFMT_PLTT4);        // 4�F�p���b�g�e�N�X�`���ɓK�p

        // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // �g�U���ːF
                                GX_RGB(16, 16, 16),     // �����ːF
                                TRUE); // �g�U���ːF�𒸓_�J���[�Ƃ��ăZ�b�g

        // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // ���ʔ��ːF
                                GX_RGB(0, 0, 0),        // ���ˌ��F
                                FALSE); // ���ʔ��˂𖢎g�p

        // �|���S�������̐ݒ�
        G3_PolygonAttr(GX_LIGHTMASK_0, // ���C�g�O�𔽉f
                       GX_POLYGONMODE_MODULATE, // ���W�����[�V�������[�h
                       GX_CULL_BACK,   // �J���o�b�N���s��
                       0,              // �|���S���h�c �O
                       31,             // �A���t�@�l
                       0);

        DrawCube();                    // �����̂̕`��

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
        /* �u�u�����N�҂� */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xB561C3C8);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}

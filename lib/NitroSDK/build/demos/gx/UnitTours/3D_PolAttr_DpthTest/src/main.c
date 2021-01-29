/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_PolAttr_DpthTest
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.8  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.7  2005/11/21 10:34:05  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.6  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.5  2004/05/27 09:24:32  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:00:23  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:07:48  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


//---------------------------------------------------------------------------
// A sample that set GX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL in G3_PolygonAttr().
// 
// This sample displays two square.  If switch is ON, the right one is drawn
// only when the polygon's depth value equals the value in depth buffer.
// 
// USAGE:
//   START      : Switch ON/OFF of GX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL
//   
//---------------------------------------------------------------------------


#include <nitro.h>
#include "DEMO.h"
#include "data.h"

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@��`�̃��f���f�[�^
//---------------------------------------------------------------------------
/* ���_�f�[�^ */
static s16 s_Vertex[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
};
/* �@���f�[�^ */
static VecFx10 s_Normal = GX_VECFX10(0, 0, FX32_ONE - 1);
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
//        i_idx : ���_�f�[�^��ID
//---------------------------------------------------------------------------
inline void Vertex(int i_idx)
{
    G3_Vtx(s_Vertex[i_idx * 3], s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�@���̐ݒ�
//  �����F
//  �@�@�@
//---------------------------------------------------------------------------
inline void Normal(void)
{
    G3_Direct1(G3OP_NORMAL, s_Normal);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�e�N�X�`�����W�̐ݒ�
//  �����F
//        
//  ���́F
//        i_idx : �e�N�X�`���f�[�^��ID
//---------------------------------------------------------------------------
inline void TextureCoord(int i_idx)
{
    G3_Direct1(G3OP_TEXCOORD, s_TextureCoord[i_idx]);
}


//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@��`�̕`��
//  �����F
//        
//---------------------------------------------------------------------------
static void DrawRectangle(void)
{
    G3_Begin(GX_BEGIN_QUADS);          // ���_���X�g�̊J�n(�l�p�`�|���S���ŕ`��)
    {
        TextureCoord(1);
        Normal();
        Vertex(1);
        TextureCoord(3);
        Normal();
        Vertex(3);
        TextureCoord(2);
        Normal();
        Vertex(2);
        TextureCoord(0);
        Normal();
        Vertex(0);
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
//        �EIRQ���荞�݂̑I��(OS_SetIrqMask())�B
//        �EIRQ���荞�݂��s�����̊֐��̓o�^(OS_SetIRQFunction())�B
//        �EIRQ���荞�݂̋��̐ݒ�(OS_EnableIrq())�B
//        ���s���邱�ƂŁA���̊֐����u�u�����N�̂Ƃ��ɌĂ΂�܂��B
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // VBlank ���荞�݂̊m�F�t���O��ݒ�
}

//---------------------------------------------------------------------------
//  �T���F
//  �@    G3_PolygonAttr()��GX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL�ݒ�
//  �����F
//�@�@�@�@�|���S���̃f�v�X�l���f�v�X�o�b�t�@�̃f�v�X�l�Ɠ������ꍇ�ɕ`�悷��B
//              
//        �����ɕ\������Ă����`�͒ʏ�\���ŁA�E���ɕ\������Ă����`���f�v
//      �X�Ώۂ̃|���S���B���ʂ��n�m�ɂ���ƁA����`�ƃf�v�X�l�������ɂȂ�E��
//      ��`�̏d�Ȃ����ӏ�������`�̏�ɕ\�������B
//  ����F
//        START: ���ʂ̂n�m/�n�e�e�̐؂�ւ��X�C�b�`
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u32     texAddr = 0x00000;         // �e�N�X�`���C���[�W�E�X���b�g�̃A�h���X
    unsigned int count = 0;
    BOOL    trg = 0;
    u16     rotate = 0;

    //---------------------
    // ������(IRQ�̋��A�u�q�`�l�̏������ABG0��3D���[�h�Ŏg�p)
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

    DEMOStartDisplay();

    OS_Printf("mssg%d:PolygonAttr_DepthTest=%s\n", count++, (trg ? "ON" : "OFF"));

    //---------------------
    //  ���C�����[�v
    //---------------------
    while (1)
    {
        G3X_Reset();
        rotate += 128;

        /* �p�b�h���� */
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] = { {0, 20}, {PAD_BUTTON_START, 20}, {0, 0} };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            trg = (trg + 1) & 0x01;
            OS_Printf("mssg%d:PolygonAttr_DepthTest=%s\n", count++, (trg ? "ON" : "OFF"));
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
            G3_LightVector(GX_LIGHTID_0, 0, 0, -FX32_ONE + 1);  // ���C�g�̌���
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));    // ���C�g�̐F
        }

        /* ���\����`�̕`��ݒ� */
        G3_MtxMode(GX_MTXMODE_TEXTURE);
        G3_Identity();
        // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        G3_PushMtx();                  // �s����X�^�b�N�֓����

        /* ���\����`�̉�]�v�Z�ƈʒu�ݒ� */
        {
            fx16    s = FX_SinIdx(rotate);
            fx16    c = FX_CosIdx(rotate);

            G3_Translate(-FX32_ONE / 2, 0, -3 * FX32_ONE);
            G3_RotZ(s, c);
        }

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
        G3_PolygonAttr(GX_LIGHTMASK_0, // ���C�g0�𔽉f
                       GX_POLYGONMODE_MODULATE, // ���W�����[�V�������[�h
                       GX_CULL_NONE,   // �J�����s��Ȃ�
                       0,              // �|���S��ID 0
                       31,             // �A���t�@�l
                       GX_POLYGON_ATTR_MISC_NONE);

        DrawRectangle();               // ��`�̕`��

        G3_PopMtx(1);

        G3_PushMtx();

        /* �E�\����`�̉�]�v�Z�ƈʒu�ݒ� */
        {
            fx16    s = FX_SinIdx(rotate);
            fx16    c = FX_CosIdx(rotate);

            G3_Translate(FX32_ONE / 2, 0, -3 * FX32_ONE);
            G3_RotZ(s, c);
        }
        /* �E�\����`�̕`��ݒ� */
        G3_MtxMode(GX_MTXMODE_TEXTURE); // �s���Texture���[�h�ɐݒ�
        G3_Identity();                 // ���݂̍s���P�ʍs��ɏ�����
        // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
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

        // �E�\����`�|���S���̃f�v�X�ύX�ݒ�
        {
            int     attr;

            if (trg)
            {
                attr = GX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL;
            }
            else
            {
                attr = GX_POLYGON_ATTR_MISC_NONE;
            }
            // �|���S�������̐ݒ�
            G3_PolygonAttr(GX_LIGHTMASK_0,      // ���C�g0���I��
                           GX_POLYGONMODE_MODULATE,     // ���W�����[�V�������[�h
                           GX_CULL_NONE,        // �J�����O���s��Ȃ�
                           0,          // �|���S��ID 0
                           31,         // �A���t�@�l
                           attr);
        }

        DrawRectangle();               // ��`�̕`��

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        OS_WaitVBlankIntr();           // VBlank�҂�

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x43CB4676);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

    }
}

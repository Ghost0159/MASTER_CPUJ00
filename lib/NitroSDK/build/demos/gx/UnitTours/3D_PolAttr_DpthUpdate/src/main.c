/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_PolAttr_DpthUpdate
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

  Revision 1.8  2005/11/21 10:34:19  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.7  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.6  2004/11/02 08:23:17  takano_makoto
  fix comment.

  Revision 1.5  2004/05/27 09:24:41  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.2  2004/03/15 02:00:32  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:08:29  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that set GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE in G3_PolygonAttr().
// The depth buffer is updated when the transparent polygon is drawn.
// 
// This sample displays a transparent cube and pole. If switch is ON, 
// the cube is drawn with GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE parameter.
// 
// USAGE:
//   UP, DOWN   : Change alpha blend parameter.
//   START      : Switch ON/OFF of GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE.
//   
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�����̂̃��f���f�[�^
//---------------------------------------------------------------------------
/* ���_�f�[�^ */
static const s16 s_Vertex[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};

//---------------------------------------------------------------------------
//  �@�@�@���_���W��ݒ�
//  �����F
//  �@�@�@�w��̒��_�f�[�^�Œ��_���W��ݒ肷��B
//  ���́F
//        i_idx : ���_�f�[�^�̂h�c
//---------------------------------------------------------------------------
static void Vertex(int i_idx)
{
    G3_Vtx(s_Vertex[i_idx * 3], s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  �T���F
//  �@�@�@�����̖̂ʐ���
//  �����F
//  �@�@�@�����̂̂P�ʂ𐶐�����B
//  ���́F
//        i_idx0�`i_id3 : �\�����钸�_�f�[�^�̂h�c
//---------------------------------------------------------------------------
inline void Quad(int i_idx0, int i_idx1, int i_idx2, int i_idx3)
{
    Vertex(i_idx0);
    Vertex(i_idx1);
    Vertex(i_idx2);
    Vertex(i_idx3);
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
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // VBlank ���荞�݂̊m�F�t���O��ݒ�
}

//---------------------------------------------------------------------------
//  �T���F
//        G3_PolygonAttr()��GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE�ݒ�
//       �i�������|���S���`�掞�̃f�v�X�o�b�t�@�̍X�V�j
//  �����F
//        �ʏ�\���̋�`�̎��͂��A�f�v�X�e�X�g�Ώۂ̃t�H�O���|�����Ă��锼��
//      �������̂����񂷂�B
//        �f�v�X�l�X�V���n�e�e�ł���΁A�����������̂ɂ̓t�H�O�̉e�������f����
//      �Ȃ��B�f�v�X�l�X�V���n�m�ɂ���΃t�H�O�̉e�������f�����B
//  ����F
//  �@    START  : �f�v�X�l�X�V�̂n�m/�n�e�e�̐؂�ւ�
//   �@ �@UP,DOWN: �I�u�W�F�N�g�̃��l�̑���
//---------------------------------------------------------------------------
void NitroMain(void)
{
    unsigned int count = 0;
    int     alpha = 0x0a;
    BOOL    trg = 1;
    u16     Rotate = 0;

    //---------------------
    // ������(IRQ�̋��A�u�q�`�l�̏������A�a�f�O���R�c���[�h�Ŏg�p)
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    /* �t�H�O�ݒ� */
    {
        u32     fog_table[8];
        int     i;

        // �N���A�J���[�̐ݒ�
        G3X_SetClearColor(GX_RGB(0, 0, 0),      // �N���A�J���[
                          0,           // �N���A�A���t�@�l
                          0x7fff,      // �N���A�f�v�X�l
                          63,          // �N���A�|���S��ID
                          TRUE);       // �t�H�O���s��
        // �t�H�O�����̐ݒ�
        G3X_SetFog(TRUE,               // �t�H�O��L����
                   GX_FOGBLEND_COLOR_ALPHA,     // �J���[�ƃA���t�@�Ƀt�H�O
                   GX_FOGSLOPE_0x2000, // �t�H�O���z�̐ݒ�
                   0x5800);            // �t�H�O�v�Z�f�v�X�l
        G3X_SetFogColor(GX_RGB(31, 31, 31), 0); // �t�H�O�J���[�̐ݒ�

        // �t�H�O�e�[�u���̐ݒ�(�l���傫���قǔZ���t�H�O�ɂȂ�)
        for (i = 0; i < 8; i++)
        {
            fog_table[i] = (u32)(((i * 16) << 0) |
                                 ((i * 16 + 4) << 8) |
                                 ((i * 16 + 8) << 16) | ((i * 16 + 12) << 24));
        }
        G3X_SetFogTable(&fog_table[0]);
    }

    G3X_AlphaBlend(TRUE);              // �A���t�@�u�����f�B���O������

    /* �����_�����O�G���W���̎Q�ƃf�[�^�Q���X���b�v
       (�y�o�b�t�@���[�h�Ƃv�o�b�t�@���[�h�ŁA
       �f�v�X�l���قȂ邱�Ƃɒ��ӁB) */
    G3_SwapBuffers(GX_SORTMODE_AUTO,   // �����\�[�g���[�h
                   GX_BUFFERMODE_Z);   // �y�l�ɂ��o�b�t�@�����O���[�h

    DEMOStartDisplay();

    OS_Printf("mssg%d:PolygonAttr_DepthUpdate=%s\n", count++, (trg ? "ON" : "OFF"));

    //---------------------
    //  ���C�����[�v
    //---------------------
    while (1)
    {
        G3X_Reset();
        Rotate += 256;

        /* �p�b�h���̎擾�Ƒ���ݒ� */
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] = { {PAD_KEY_UP, 10}, {0, 1}, {PAD_KEY_UP, 10}, {0, 1},
            {PAD_KEY_UP, 10}, {0, 1}, {PAD_KEY_UP, 10}, {0, 0}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_TRIG(PAD_KEY_UP))
        {
            if (++alpha > 31)
            {
                alpha = 31;
            }
            OS_Printf("mssg%d:alpha=%d\n", count++, alpha);
        }
        else if (DEMO_IS_TRIG(PAD_KEY_DOWN))
        {
            if (--alpha < 0)
            {
                alpha = 0;
            }
            OS_Printf("mssg%d:alpha=%d\n", count++, alpha);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            trg = (trg + 1) & 0x01;
            OS_Printf("mssg%d:PolygonAttr_DepthUpdate=%s\n", count++, (trg ? "ON" : "OFF"));
        }

        /* �J�����̐ݒ� */
        {
            VecFx32 Eye = { 0, 0, FX32_ONE * 8 };       // ���_�ʒu
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // ���_�ݒ�
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // �s���Texture���[�h�ɐݒ�
        G3_Identity();                 // ���݂̍s���P�ʍs��ɏ�����
        // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        G3_PushMtx();                  // �s����X�^�b�N�֓����

        /* �����̂̉�]�v�Z�ƈʒu�ݒ� */
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_RotY(s, c);
            G3_Translate(0, 0, 5 * FX32_ONE);
            G3_RotX(s, c);
            G3_RotZ(s, c);
        }

        /* �����̂̕`��ݒ�i�t�H�O�u�����h���s���j */
        // �����̊g�U���ːF�Ɗ����ːF��ݒ�
        G3_MaterialColorDiffAmb(GX_RGB(31, 0, 0),       // �g�U���ːF
                                GX_RGB(16, 16, 16),     // �����ːF
                                TRUE); // ���_�J���[��ݒ�
        // �����̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // ���ʔ��ːF
                                GX_RGB(0, 0, 0),        // ���ˌ��F
                                FALSE); // ���ʔ��˂𖢎g�p

        // �����̂̃|���S�������i�A���t�@�ƃf�v�X�j�̐ݒ�
        {
            int     attr;

            if (trg)
            {
                attr = GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE;
            }
            else
            {
                attr = GX_POLYGON_ATTR_MISC_NONE;
            }
            attr |= GX_POLYGON_ATTR_MISC_FOG;
            // �|���S�������̐ݒ�
            G3_PolygonAttr(GX_LIGHTMASK_NONE,   // ���C�g�̉e���Ȃ�
                           GX_POLYGONMODE_MODULATE,     // ���W�����[�V�������[�h
                           GX_CULL_BACK,        // �J���o�b�N���s��
                           0,          // �|���S���h�c �O
                           alpha,      // �A���t�@�l�̐ݒ�
                           attr);
        }

        DrawCube();                    // �����̂̕`��

        G3_PopMtx(1);

        /* ��`�̕`��ݒ�i�t�H�O�u�����h���s��Ȃ��j */
        G3_PushMtx();
        // �����̊g�U���ːF�Ɗ����ːF��ݒ�
        G3_MaterialColorDiffAmb(GX_RGB(0, 0, 31),       // �g�U���ːF
                                GX_RGB(16, 16, 16),     // �����ːF
                                TRUE); // ���_�J���[��ݒ�
        // �����̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // ���ʔ��ːF
                                GX_RGB(0, 0, 0),        // ���ˌ��F
                                FALSE); // ���ʔ��˂𖢎g�p
        // �|���S�������̐ݒ�
        G3_PolygonAttr(GX_LIGHTMASK_NONE,       // ���C�g�̉e���Ȃ�
                       GX_POLYGONMODE_MODULATE, // ���W�����[�V�������[�h
                       GX_CULL_BACK,   // �J���o�b�N���s��
                       0,              // �|���S���h�c �O
                       31,             // �A���t�@�l
                       0);

        G3_Begin(GX_BEGIN_QUADS);      // ���_���X�g�̊J�n(�l�p�`�|���S���ŕ`��)
        {
            G3_Vtx(FX32_ONE << 2, -FX32_ONE << 2, 0);
            G3_Vtx(FX32_ONE << 2, FX32_ONE << 2, 0);
            G3_Vtx(-FX32_ONE, FX32_ONE << 2, 0);
            G3_Vtx(-FX32_ONE, -FX32_ONE << 2, 0);
        }
        G3_End();                      // ���_���X�g�̏I��

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);

        /* �u�u�����N�҂� */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x2A787000);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}

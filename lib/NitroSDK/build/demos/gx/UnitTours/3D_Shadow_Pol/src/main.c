/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Shadow_Pol
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.9  2006/01/18 02:11:26  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 10:39:35  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.7  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.6  2004/11/02 08:23:30  takano_makoto
  fix comment.

  Revision 1.5  2004/05/27 09:25:01  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 02:01:34  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 09:09:37  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


//---------------------------------------------------------------------------
// A sample that use shadow polygon.
// 
// This sample displays a sphere and ground.
// On the ground, the sphere's shadow is drawn.
//   
//---------------------------------------------------------------------------


#include <nitro.h>
#include "DEMO.h"
#include "data.h"

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
//        �V���h�E�|���S�����g�p���ĉe��\��
//  �����F
//        1. �����n�ʂɎʂ��e����邽�߂ɉ~����p�ӂ���A���̎��A�~���͓�������
//         ��p�ӂ���B
//        2. �}�X�N�p��PolygonAttr�̃|���S���������V���h�E�|���S���ł���~����
//         �p�ӂ��A���ʂ̂ݕ`�悷����̂Ƃ���B
//        3. ���ɁA����̂Ɠ������W�ɁA�`��p��PolygonAttr�̃|���S���������V��
//         �h�E�|���S���ł���~����p�ӂ��A�\�ʂ̂ݕ`�悷����̂Ƃ���B
//           ���̎��A�`��p�̉~���ɂ͒��_�J���[��^���Ă����B
//        4. ���̉~����n�ʂƂȂ�|���S���Ɍ���������ƁA�~���ƒn�ʂƂ���������
//         ���镔���ɉe���\�������B
//           ���̎��\�������e�̐F�͕`��p�̃V���h�E�|���S���̃J���[���g�p����
//         ��B
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u16     Rotate = 0;                // for rotating sphear(0-65535)

    //---------------------
    // ������(IRQ�̋��A�u�q�`�l�̏������A�a�f�O���R�c���[�h�Ŏg�p)
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    G3X_AlphaBlend(TRUE);              // �A���t�@�u�����h������

    DEMOStartDisplay();

    //---------------------
    //  ���C�����[�v
    //---------------------
    while (1)
    {
        G3X_Reset();
        Rotate += 256;

        /* �J�����̐ݒ� */
        {
            VecFx32 Eye = { 0, 15 * FX32_ONE, -15 * FX32_ONE }; // ���_�ʒu
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // ���_�ݒ�
        }

        /* ���C�g�̐ݒ�(���C�g�̐F�ƌ�����ݒ�) */
        {
            G3_LightVector(GX_LIGHTID_0, 0, -FX32_ONE + 1, 0);
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // �s���Texture���[�h�ɐݒ�
        G3_Identity();                 // ���݂̍s���P�ʍs��ɏ�����
        // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        /* �n�`�̕`��ݒ� */
        G3_PushMtx();
        // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // �g�U���ːF
                                GX_RGB(16, 16, 16),     // �����ːF
                                FALSE); // ���_�J���[�͎g�p���Ȃ�

        // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // ���ʔ��ːF
                                GX_RGB(0, 0, 0),        // ���ˌ��F
                                FALSE); // ���ʔ��˂𖢎g�p
        // �e�N�X�`���̃p�����[�^��ݒ�
        G3_TexImageParam(GX_TEXFMT_NONE,        // �e�N�X�`���͎g�p���Ȃ�
                         GX_TEXGEN_NONE,
                         GX_TEXSIZE_S8,
                         GX_TEXSIZE_T8,
                         GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
        // �|���S�������̐ݒ�
        G3_PolygonAttr(GX_LIGHTMASK_0, // ���C�g�O���I��
                       GX_POLYGONMODE_MODULATE, // ���W�����[�V�������[�h
                       GX_CULL_NONE,   // �J���o�b�N���s��Ȃ�
                       2,              // �|���S���h�c �Q
                       31,             // �A���t�@�l
                       GX_POLYGON_ATTR_MISC_NONE);

        G3_Scale(10 * FX32_ONE, 0, 10 * FX32_ONE);

        G3_Begin(GX_BEGIN_QUADS);      // ���_���X�g�̊J�n(�l�p�`�|���S���ł̕`��)
        G3_Normal(0, 1 << 9, 0);
        G3_Vtx(FX16_ONE, 0, FX16_ONE);
        G3_Vtx(FX16_ONE, 0, -FX16_ONE);
        G3_Vtx(-FX16_ONE, 0, -FX16_ONE);
        G3_Vtx(-FX16_ONE, 0, FX16_ONE);
        G3_End();                      // ���_���X�g�̏I��

        G3_PopMtx(1);


        G3_PushMtx();
        /* ���̉�]�v�Z�ƈʒu�ݒ� */
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_RotY(s, c);
            G3_Translate(0, 5 * FX32_ONE, -5 * FX32_ONE);
            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        /* ���̕`��ݒ� */
        // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // �g�U���ːF
                                GX_RGB(16, 16, 16),     // �����ːF
                                FALSE); // ���_�J���[�͎g�p���Ȃ�

        // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // ���ʔ��ːF
                                GX_RGB(0, 0, 0),        // ���ˌ��F
                                FALSE); // ���ʔ��˂𖢎g�p
        // �e�N�X�`���̃p�����[�^��ݒ�
        G3_TexImageParam(GX_TEXFMT_NONE,        // �e�N�X�`���͎g�p���Ȃ�
                         GX_TEXGEN_NONE,
                         GX_TEXSIZE_S8,
                         GX_TEXSIZE_T8,
                         GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
        // �|���S�������̐ݒ�
        G3_PolygonAttr(GX_LIGHTMASK_0, // ���C�g�O���I��
                       GX_POLYGONMODE_MODULATE, // ���W�����[�V�������[�h
                       GX_CULL_BACK,   // �J���o�b�N���s��
                       1,              // �|���S���h�c �P
                       31,             // �A���t�@�l
                       GX_POLYGON_ATTR_MISC_NONE);

        G3_Begin(GX_BEGIN_TRIANGLES);  // ���_���X�g�̊J�n(�O�p�`�|���S���ŕ`��)
        {
            MI_SendGXCommand(3, sphere1, sphere_size);
        }
        G3_End();                      // ���_���X�g�̏I��

        G3_PopMtx(1);

        /* �}�X�N�p�V���h�E�|���S���̈ʒu�ݒ� */
        G3_PushMtx();
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_RotY(s, c);
            G3_Translate(0, 0, -5 * FX32_ONE);
        }

        /* �}�X�N�p�V���h�E�|���S���̕`��ݒ� */
        // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
        G3_MaterialColorDiffAmb(GX_RGB(0, 0, 0),        // �g�U���ːF
                                GX_RGB(0, 0, 0),        // �����ːF
                                FALSE); // ���_�J���[���g�p���Ȃ�
        // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
        G3_MaterialColorSpecEmi(GX_RGB(0, 0, 0),        // ���ʔ��ːF
                                GX_RGB(0, 0, 0),        // ���ˌ��F
                                FALSE); // ���ʔ��˂𖢎g�p
        // �e�N�X�`���̃p�����[�^��ݒ�
        G3_TexImageParam(GX_TEXFMT_NONE,        // �e�N�X�`���͎g�p���Ȃ�
                         GX_TEXGEN_NONE,
                         GX_TEXSIZE_S8,
                         GX_TEXSIZE_T8,
                         GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
        // �|���S�������̐ݒ�
        G3_PolygonAttr(GX_LIGHTMASK_0, // ���C�g�𔽉f���Ȃ�
                       GX_POLYGONMODE_SHADOW,   // �V���h�E�|���S�����[�h
                       GX_CULL_FRONT,  // �J���t�����g���s��
                       0,              // �|���S���h�c �O
                       0x08,           // �A���t�@�l
                       GX_POLYGON_ATTR_MISC_NONE);

        // �}�X�N�i�~���j�̕`��
        G3_Begin(GX_BEGIN_TRIANGLES);  // ���_���X�g�̊J�n(�O�p�`�|���S���ŕ`��)
        {
            MI_SendGXCommand(3, cylinder1, cylinder_size);
        }
        G3_End();                      // ���_���X�g�̏I��

        /* �`��p�V���h�E�|���S���̕`��ݒ� */
        // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
        G3_MaterialColorDiffAmb(GX_RGB(0, 0, 0),        // �g�U���ːF
                                GX_RGB(0, 0, 0),        // �����ːF
                                TRUE); // ���_�J���[���g�p
        // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
        G3_MaterialColorSpecEmi(GX_RGB(0, 0, 0),        // ���ʔ��ːF
                                GX_RGB(0, 0, 0),        // ���ˌ��F
                                FALSE); // ���ʔ��˂𖢎g�p
        // �e�N�X�`���̃p�����[�^��ݒ�
        G3_TexImageParam(GX_TEXFMT_NONE,        // �e�N�X�`���͎g�p���Ȃ�
                         GX_TEXGEN_NONE,
                         GX_TEXSIZE_S8,
                         GX_TEXSIZE_T8,
                         GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
        // �|���S�������̐ݒ�
        G3_PolygonAttr(GX_LIGHTMASK_0, // ���C�g�𔽉f���Ȃ�
                       GX_POLYGONMODE_SHADOW,   // �V���h�E�|���S�����[�h
                       GX_CULL_BACK,   // �J���o�b�N���s��
                       1,              // �|���S���h�c �P
                       0x08,           // �A���t�@�l
                       GX_POLYGON_ATTR_MISC_NONE);      // 

        // �V���h�E�i�~���j�̕`��
        G3_Begin(GX_BEGIN_TRIANGLES);  // ���_���X�g�̊J�n(�O�p�`�|���S���ŕ`��)
        {
            MI_SendGXCommand(3, cylinder1, cylinder_size);
        }
        G3_End();                      // ���_���X�g�̏I��

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W);

        /* �u�u�����N�҂� */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x38FFDE1A);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}

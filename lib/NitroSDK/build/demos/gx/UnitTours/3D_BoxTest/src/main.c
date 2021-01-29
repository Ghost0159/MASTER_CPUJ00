/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_BoxTest
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

  Revision 1.8  2005/11/21 10:33:02  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.7  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.6  2004/07/29 06:50:14  takano_makoto
  Add PolygonAttr setting before BoxTest.

  Revision 1.5  2004/05/27 09:23:45  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.2  2004/03/15 01:58:41  takano_makoto
  modify comments and programs a little.

  Revision 1.1  2004/03/01 09:04:35  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use Box test.
// 
// This sample displays a cube and a rolling sphere.
// If the sphere contact with the cube, it is displayed normaly.
// The other case it is diplayed by wire frame.
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
//        �{�b�N�X�e�X�g���s��
//  �����F
//        ��]���郏�C���[�t���[���̋��̂����͂��͂ޗ����̖̂ʂɐڐG����ƁA
//      �{�b�N�X�e�X�g�̌��ʒʏ�`�悳���B
//        
//        1. ���ˉe�ŋ�Ԓ���BoxTest�ŃJ�����O�������Ԃ�ݒ肷��B
//        2. ���ˉe�Őݒ肳�ꂽ�J�����g�N���b�v���W�s��ɑ΂��āA���Ɠ��T�C�Y��
//         �����̂��g����BoxTest���s���B
// �@�@�@ 3. ���̌�A�����ˉe�ɕϊ����ă��C���[�t���[���Ő�قǐݒ肵���J�����O
//         ��Ԃ�`�悷��B
//        4. �����ċ��̕`��̍ۂɐ旧���čs����BoxTest�̌��ʂ����ɒʏ�`���
//�@�@     ���C���[�t���[���`���؂�ւ���B
//---------------------------------------------------------------------------
void NitroMain(void)
{
    u16     Rotate = 0;                // for rotating cubes(0-65535)

    //---------------------
    // ������
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();

    G3X_Init();                        // �R�c�O���t�B�b�N�̏�����
    G3X_InitTable();                   // �O���t�B�b�N�e�[�u���̏�����
    G3X_InitMtxStack();                // �s��X�^�b�N�̏�����

    /* �R�c�O���t�B�b�N�̐ݒ� */
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);       // �a�f�̂O�Ԃ��g�p

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // �Q�c�^�R�c���[�h
                       GX_BGMODE_4,    // BGMODE���S�ɐݒ�
                       GX_BG0_AS_3D);  // �a�f�̂O���R�c�Ƃ��Ďg�p
    G2_SetBG0Priority(0);              // �a�f�O�Ԃ̕\���D��x���g�b�v��

    G3X_SetShading(GX_SHADING_HIGHLIGHT);       // �V�F�[�f�B���O���[�h�̐ݒ�
    G3X_AntiAlias(TRUE);               // �A���`�G�C���A�X��L��

    G3_SwapBuffers(GX_SORTMODE_AUTO,   // �X���b�v�o�b�t�@�̐ݒ�
                   GX_BUFFERMODE_W);
    // �N���A�J���[�̐ݒ�
    G3X_SetClearColor(GX_RGB(0, 0, 0), // clear color
                      0,               // clear alpha
                      0x7fff,          // clear depth
                      63,              // clear polygon ID
                      FALSE);          // fog

    G3_ViewPort(0, 0, 255, 191);       // �r���[�|�[�g�̐ݒ�

    DEMOStartDisplay();

    //---------------------
    //  ���C�����[�v
    //---------------------
    while (1)
    {
        int     sphere_alpha;

        G3X_Reset();
        Rotate += 256;

        /* ���ˉe�̐ݒ� */
        {
            fx32    pos1_x = -((5 * FX32_ONE) / 2);
            fx32    pos1_y = ((5 * FX32_ONE) / 2);
            fx32    pos2_x = ((5 * FX32_ONE) / 2);
            fx32    pos2_y = -((5 * FX32_ONE) / 2);
            fx32    near = (5 * FX32_ONE) / 2;
            fx32    far = ((5 * FX32_ONE) / 2) + (5 * FX32_ONE);

            G3_MtxMode(GX_MTXMODE_PROJECTION);  // �s���Projection���[�h�ɐݒ�
            G3_Identity();             // ���݂̍s���P�ʍs��ɏ�����
            // ���ˉe�̐ݒ�
            G3_Ortho(pos1_y,           // up    pos Y
                     pos2_y,           // down  pos Y
                     pos1_x,           // left  pos X
                     pos2_x,           // right pos X
                     near,             // near
                     far,              // far
                     NULL);

            G3_StoreMtx(0);
        }

        /* �{�b�N�X�e�X�g */
        {
            G3_MtxMode(GX_MTXMODE_TEXTURE);     // �s���Texture���[�h�ɐݒ�
            G3_Identity();             // ���݂̍s���P�ʍs��ɏ�����
            // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
            G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
            G3_PushMtx();
            {
                GXBoxTestParam box;
                fx16    s = FX_SinIdx(Rotate);
                fx16    c = FX_CosIdx(Rotate);
                s32     result;

                G3_Translate(0, 0, -11 * FX32_ONE);
                G3_RotY(s, c);
                G3_Translate(0, 0, 6 * FX32_ONE);
                G3_RotX(s, c);
                G3_RotY(s, c);
                G3_RotZ(s, c);

                // BoxTest�̑O�ɂ�FAR�N���b�v�y��1�h�b�g�|���S�������_�����O�t���O��
                // ON�ɐݒ肷��
                G3_PolygonAttr(GX_LIGHTMASK_0,
                               GX_POLYGONMODE_MODULATE,
                               GX_CULL_NONE,
                               0,
                               0,
                               GX_POLYGON_ATTR_MISC_FAR_CLIPPING | GX_POLYGON_ATTR_MISC_DISP_1DOT);
                G3_Begin(GX_BEGIN_TRIANGLES);
                G3_End();

                box.x = -7200;
                box.y = -7200;
                box.z = -7200;
                box.width = 7200 * 2;
                box.height = 7200 * 2;
                box.depth = 7200 * 2;
                G3_BoxTest(&box);      // �{�b�N�X�e�X�g���s��

                while (G3X_GetBoxTestResult(&result) != 0)
                {
                    // �{�b�N�X�e�X�g�̌��ʑ҂����[�v
                    ;
                }
                sphere_alpha = (result ? 31 : 0);
                OS_Printf("result %d\n", result);
            }
            G3_PopMtx(1);
        }

        /* �����ˉe�̐ݒ� */
        G3_MtxMode(GX_MTXMODE_PROJECTION);      // �s���Projection���[�h�ɐݒ�
        G3_Identity();                 // ���݂̍s���P�ʍs��ɏ�����
        {
            fx32    right = FX32_ONE;
            fx32    top = FX32_ONE * 3 / 4;
            fx32    near = FX32_ONE;
            fx32    far = FX32_ONE * 400;
            // �����ˉe�̐ݒ�
            G3_Perspective(FX32_SIN30, // Sine   FOVY
                           FX32_COS30, // Cosine FOVY
                           FX32_ONE * 4 / 3,    // Aspect
                           near,       // Near
                           far,        // Far
                           NULL);

            G3_StoreMtx(0);
        }

        /* �J�����̐ݒ� */
        {
            VecFx32 Eye = { 0, 8 * FX32_ONE, FX32_ONE };        // ���_�ʒu
            VecFx32 at = { 0, 0, -5 * FX32_ONE };       // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);   // ���_�ݒ� 
        }

        /* ���C�g�̐ݒ� */
        {
            G3_LightVector(GX_LIGHTID_0, 0, -FX32_ONE + 1, 0);  // ���C�g�̐F
            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));    // ���C�g�̌���
        }

        G3_MtxMode(GX_MTXMODE_TEXTURE); // �s���Texture���[�h�ɐݒ�
        G3_Identity();                 // ���݂̍s���P�ʍs��ɏ�����
        // �s���Position�EVector �����ݒ胂�[�h�ɐݒ�
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        /* �{�b�N�X�̐����ƕ`�� */
        {
            G3_PushMtx();
            G3_Translate(0, 0, -5 * FX32_ONE);  // �{�b�N�X�̈ʒu�ݒ�
            // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
            G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // �g�U���ːF
                                    GX_RGB(16, 16, 16), // �����ːF
                                    FALSE);     // ���_�J���[���Z�b�g���Ȃ�
            // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
            G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // ���ʔ��ːF
                                    GX_RGB(0, 0, 0),    // ���ˌ��F
                                    FALSE);     // ���ʔ��˃e�[�u���𖢎g�p
            // �e�N�X�`���̃p�����[�^��ݒ�
            G3_TexImageParam(GX_TEXFMT_NONE,    // �e�N�X�`���Ȃ�
                             GX_TEXGEN_NONE,    // �e�N�X�`�����W�ϊ��Ȃ�
                             GX_TEXSIZE_S8,
                             GX_TEXSIZE_T8,
                             GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
            // �|���S�������̐ݒ�
            G3_PolygonAttr(GX_LIGHTMASK_0,      // ���C�g�O���I��
                           GX_POLYGONMODE_MODULATE,     // ���W�����[�V�������[�h
                           GX_CULL_NONE,        // �J�����O����
                           1,          // �|���S���h�c �P
                           0,          // �A���t�@�l
                           GX_POLYGON_ATTR_MISC_NONE);
            G3_Begin(GX_BEGIN_QUADS);  // ���_���X�g�̊J�n
            // (�l�p�`�|���S���ł̕`��錾)
            {
                G3_Normal(0, 1 << 9, 0);
                G3_Vtx(((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));

                G3_Normal(0, -1 << 9, 0);
                G3_Vtx(((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));

                G3_Normal(1 << 9, 0, 0);
                G3_Vtx(((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));

                G3_Normal(-1 << 9, 0, 0);
                G3_Vtx(-((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), -((5 * FX16_ONE) / 2));
                G3_Vtx(-((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2), ((5 * FX16_ONE) / 2));
            }
            G3_End();                  // ���_���X�g�̏I��
            G3_PopMtx(1);
        }

        /* ���̐����ƕ`�� */
        {
            G3_PushMtx();              // �s����X�^�b�N��
            {                          // �ړ��ʒu�̌v�Z
                fx16    s = FX_SinIdx(Rotate);
                fx16    c = FX_CosIdx(Rotate);

                G3_Translate(0, 0, -11 * FX32_ONE);
                G3_RotY(s, c);
                G3_Translate(0, 0, 6 * FX32_ONE);
                G3_RotX(s, c);
                G3_RotY(s, c);
                G3_RotZ(s, c);
            }
            // �ގ��̊g�U���ːF�Ɗ����ːF��ݒ�
            G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // �g�U���ːF
                                    GX_RGB(16, 16, 16), // �����ːF
                                    FALSE);     // ���_�J���[���Z�b�g���Ȃ�
            // �ގ��̋��ʔ��ːF�ƕ��ˌ��F��ݒ�
            G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // ���ʔ��ːF
                                    GX_RGB(0, 0, 0),    // ���ˌ��F
                                    FALSE);     // ���ʔ��˃e�[�u�����g�p����
            // �e�N�X�`���̃p�����[�^��ݒ�
            G3_TexImageParam(GX_TEXFMT_NONE,    // �e�N�X�`���Ȃ�
                             GX_TEXGEN_NONE,    // �e�N�X�`�����W�ϊ��Ȃ�
                             GX_TEXSIZE_S8,
                             GX_TEXSIZE_T8,
                             GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
            // �|���S���֘A�����l��ݒ�
            G3_PolygonAttr(GX_LIGHTMASK_0,      // ���C�g�O���I��
                           GX_POLYGONMODE_MODULATE,     // ���W�����[�V�������[�h
                           GX_CULL_NONE,        // �J�����O����
                           1,          // �|���S���h�c �P
                           sphere_alpha,        // �A���t�@�l
                           GX_POLYGON_ATTR_MISC_NONE);

            OS_Printf("sphere_alpha=%d\n", sphere_alpha);
            // ���̂̕`��
            G3_Begin(GX_BEGIN_TRIANGLES);       /* ���_���X�g�̊J�n
                                                   (�O�p�`�|���S���ł̕`��錾) */
            {
                MI_SendGXCommand(3, sphere1, sphere_size);
            }
            G3_End();                  // ���_���X�g�̏I��
            G3_PopMtx(1);
        }

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        /* VBlank�҂� */
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x1959EE87);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

    }
}

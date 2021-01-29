/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_OBJWindow
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

  Revision 1.7  2005/11/21 10:32:04  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.6  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.5  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.4  2004/05/27 09:22:11  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 09:03:11  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use OBJ window.
// 
// This sample displays a sea view through a OBJ window.
// 
// USAGE:
//   UP, DOWN, LEFT, RIGHT      : Move the OBJ window
//   
// HOWTO:
// 1. Set up OBJ window by GX_SetVisibleWnd(GX_WNDMASK_OW)
// 2. Set inside and outside of window by G2_SetWndOutsidePlane() and 
//    G2_SetWndOBJInsidePlane.
// 3. Transfer the OBJ data by GX_LoadOBJ().
// 4. Set GX_OAM_MODE_OBJWND to OAM attribute to use the OBJ window
//---------------------------------------------------------------------------


#include <nitro.h>
#include "DEMO.h"
#include "data.h"
//---------------------------------------------------------------------------
//  �T���F
//        �n�`�l�o�b�t�@�̈�
//---------------------------------------------------------------------------
static GXOamAttr s_OAMBuffer[128];

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
//        �n�a�i�E�B���h�E�̕\��
//  �����F
//        �w�i�̂a�f���A�~�`�̂n�a�i�E�B���h�E��ʂ��ĕ\������B�n�a�i�E�B��
//      �h�E�̊O���͍��ŕ\�������B  
//        �p�b�h����ɂ���āA�n�a�i�E�B���h�E���ړ����邱�Ƃ��\�B
//  ����F
//        UP, DOWN: �n�a�i�E�B���h�E�̈ړ��B
//---------------------------------------------------------------------------
void NitroMain()
{
    int     pos_x = 0, pos_y = 0;

    //---------------------
    // ������
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();

    /* �\��������n�`�l�Ƃa�f�̑I�� */
    GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);

    /* �n�`�l�̐ݒ� */
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);        // VRAM-A���g�p
    // �L�����N�^OBJ�̃}�b�s���O���[�h��ݒ�
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);      // �Q�����}�b�s���O

    /* �a�f�̐ݒ� */
    GX_SetBankForBG(GX_VRAM_BG_128_B); // VRAM-B���g�p
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // �Q�c�^�R�c���[�h
                       GX_BGMODE_0,    // 
                       GX_BG0_AS_2D);  // �Q�c�\��
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,        // �X�N���[���T�C�Y256�~256
                     GX_BG_COLORMODE_256,       // �Q�T�U�F
                     GX_BG_SCRBASE_0x0000,      // �X�N���[���I�t�Z�b�g 0x0000
                     GX_BG_CHARBASE_0x04000,    // �L�����N�^�[�I�t�Z�b�g0x04000
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);              // �a�f�O�Ԃ̕\���D��x���g�b�v��
    G2_BG0Mosaic(FALSE);               // ���U�C�N���ʂ��s��Ȃ�

    /* �E�B���h�E�ݒ� */
    GX_SetVisibleWnd(GX_WNDMASK_OW);   // �n�a�i�E�B���h�E��L��
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE,        // �E�B���h�E�̊O���𖳌�
                          FALSE);
    G2_SetWndOBJInsidePlane(GX_WND_PLANEMASK_BG0,       // �E�B���h�E�̓�����BG0�ɐݒ�
                            FALSE);

    /* �n�a�i�L�����N�^�[�f�[�^�[�̓ǂݍ��� */
    GX_LoadOBJ(d_64_256_obj_schDT,     // OBJ�L�����f�[�^(���̌`)�ǂݍ���
               0, sizeof(d_64_256_obj_schDT));
    /* �a�f�L�����N�^�[�f�[�^�[�ƃp���b�g�f�[�^�ǂݍ��� */
    GX_LoadBG0Char(d_natsunoumi_schDT, // BG0�̃L�����f�[�^�̓ǂݍ���
                   0, sizeof(d_natsunoumi_schDT));
    GX_LoadBGPltt(d_natsunoumi_sclDT,  // BG�p���b�g�f�[�^�̓ǂݍ���
                  0, sizeof(d_natsunoumi_sclDT));
    GX_LoadBG0Scr(d_natsunoumi_sscDT,  // BG0�̃X�N���[���f�[�^�̓ǂݍ���
                  0, sizeof(d_natsunoumi_sscDT));

    DEMOStartDisplay();

    //---------------------
    //  ���C�����[�v
    //---------------------
    while (1)
    {
        /* �p�b�h���̓ǂݍ��� */
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] =
                { {PAD_BUTTON_A, 10}, {PAD_BUTTON_L, 10}, {PAD_KEY_DOWN | PAD_KEY_RIGHT, 20}, {0,
                                                                                               0}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            if (++pos_y > 191)
            {
                pos_y = 191;
            }
        }
        else if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            if (--pos_y < 0)
            {
                pos_y = 0;
            }
        }
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
        {
            if (++pos_x > 255)
            {
                pos_x = 255;
            }
        }
        else if (DEMO_IS_PRESS(PAD_KEY_LEFT))
        {
            if (--pos_x < 0)
            {
                pos_x = 0;
            }
        }

        /* �n�a�i�E�B���h�E�̂n�`�l�A�g���r���[�g��ݒ� */
        G2_SetOBJAttr(&s_OAMBuffer[0], // �g�p����n�`�l�̃|�C���^�[
                      pos_x,           // �w�ʒu
                      pos_y,           // �x�ʒu
                      0,               // �D�揇��
                      GX_OAM_MODE_OBJWND,       // �n�a�i�E�B���h�E���[�h�ɐݒ�
                      FALSE,           // ���U�C�N����
                      GX_OAM_EFFECT_NONE,       // ������ʖ���
                      GX_OAM_SHAPE_64x64,       // �n�a�i�̑傫��
                      GX_OAM_COLOR_256, // �Q�T�U�F
                      0, 0, 0);

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xAE1F20B9);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        /* �L���b�V�������C���������[�ɓf���o���A���������� */
        DC_FlushRange(s_OAMBuffer, sizeof(s_OAMBuffer));
        /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
        // DC_WaitWriteBufferEmpty();

        /* �u�a���������҂� */
        OS_WaitVBlankIntr();

        /* �n�`�l�֓]�� */
        GX_LoadOAM(s_OAMBuffer,        // OAM�o�b�t�@��OAM�֓]��
                   0, sizeof(s_OAMBuffer));
        MI_DmaFill32(3,                // OAM�o�b�t�@���N���A�[
                     s_OAMBuffer, 192, sizeof(s_OAMBuffer));
    }
}

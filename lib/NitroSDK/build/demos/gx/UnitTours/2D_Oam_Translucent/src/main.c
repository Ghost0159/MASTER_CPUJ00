/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_Translucent
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

  Revision 1.7  2005/11/21 10:32:19  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.6  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.5  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.4  2004/05/27 09:22:22  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 09:04:12  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use OBJ with alpha blending effect.
// 
// This sample displays two spheres on the display.
// One of the sphere is transparent.
// 
// USAGE:
//   UP, DOWN   : Change rate of transparent.
//   
// HOWTO:
// 1. Set GX_OAM_MODE_XLU to OAM attribute to use transparent OBJ.
// 2. Set transparent object by G2_SetBlendAlpha().
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
//        �������n�a�i�̕\��
//  �����F
//        �������̂n�a�i�L�����N�^�[��\������B
//        �����ɕ\������Ă���OBJ�͒ʏ�\���ŁA�E���ɕ\������Ă���OBJ�͔�����
//      OBJ�B
//  ����F
//        UP,DOWN: ������OBJ�̃u�����f�B���O�W����ύX�ł���B
//---------------------------------------------------------------------------
void NitroMain()
{
    int     eva = 12;

    //---------------------
    // ������
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplayOBJOnly();

    /* �L�����N�^�[�f�[�^�[�ƃp���b�g�f�[�^�ǂݍ��� */
    GX_LoadOBJ(d_64_256_obj_schDT, 0, sizeof(d_64_256_obj_schDT));
    GX_LoadOBJPltt(d_64_256_obj_sclDT, 0, sizeof(d_64_256_obj_sclDT));

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
            const EXTKeys keys[8] = { {PAD_KEY_DOWN, 5}, {0, 0} };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            if (++eva > 0x1f)
            {
                eva = 0x1f;
            }
            OS_Printf("eva=%d\n", eva);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            if (--eva < 0x00)
            {
                eva = 0x00;
            }
            OS_Printf("eva=%d\n", eva);
        }

        /* �ʏ�n�a�i�̂n�`�l�A�g���r���[�g��ݒ� */
        G2_SetOBJAttr(&s_OAMBuffer[0], // �P�Ԗڂ̂n�`�l���g�p
                      0,               // �w�ʒu
                      0,               // �x�ʒu
                      0,               // �D�揇��
                      GX_OAM_MODE_NORMAL,       // �ʏ�OBJ
                      FALSE,           // ���U�C�N����
                      GX_OAM_EFFECT_NONE,       // ������ʖ���
                      GX_OAM_SHAPE_64x64,       // �n�a�i�̑傫��
                      GX_OAM_COLOR_256, // �Q�T�U�F
                      0,               // �L������
                      0, 0);

        /* �������n�a�i�̂n�`�l�A�g���r���[�g��ݒ� */
        G2_SetOBJAttr(&s_OAMBuffer[1], // �Q�Ԗڂ̂n�`�l���g�p
                      64,              // �w�ʒu
                      0,               // �x�ʒu
                      0,               // �D�揇��
                      GX_OAM_MODE_XLU, // ������OBJ
                      FALSE,           // ���U�C�N����
                      GX_OAM_EFFECT_NONE,       // ������ʖ���
                      GX_OAM_SHAPE_64x64,       // �n�a�i�̑傫��
                      GX_OAM_COLOR_256, // �Q�T�U�F
                      0,               // �L������
                      0, 0);
        /* ���u�����f�B���O�ݒ� */
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE,       // ���Ώۖʂ͎w��Ȃ�
                         GX_BLEND_PLANEMASK_BD, // ���Ώۖʂ̓o�b�N�h���b�v
                         eva,          // ���Ώۖʂւ̃u�����h�W���̐ݒ�
                         0);           // 

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x80C7021F);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        /* �L���b�V�������C���������[�ɓf���o�� */
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

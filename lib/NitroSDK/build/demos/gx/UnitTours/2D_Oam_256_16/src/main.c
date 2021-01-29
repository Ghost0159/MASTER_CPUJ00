/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_256_16
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.11  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.10  2005/11/21 10:30:18  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.9  2005/07/22 12:53:44  kitase_hirotake
  �����Y��Ă����f�N�������g������������

  Revision 1.8  2005/07/22 12:38:33  kitase_hirotake
  FINALROM�ŏ\���L�[�������Ȃ����Ƃ̏C��

  Revision 1.7  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.6  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.5  2004/06/02 13:08:56  takano_makoto
  Change EXTPLTT enum name.

  Revision 1.4  2004/05/27 09:21:38  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.1  2004/03/01 09:05:36  takano_makoto
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that use extended 256x16 color palette in OBJ:
// 
// This sample displays some objects on the display with extended 256x16 color 
// palette.
// 
// USAGE:
//   UP, DOWN, LEFT, RIGHT      : Slide the OBJs
//   A, B                       : Scaling the OBJs
//   L, R                       : Rotate the OBJs
//   SELECT                     : Switch palette Number of the OBJs
//   START                      : Initialize status of the OBJs
//   
// HOWTO:
// 1. Allocate VRAM to extended palette by GX_SetBankForOBJExtPltt().
// 2. Transfer the palette data by GX_BeginLoadOBJExtPltt(), GX_LoadOBJExtPltt()
//    and GX_EndLoadOBJExtPltt().
// 3. Set GX_OAM_COLOR_256 to OAM attribute to use the extended palette.
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
//        �A�t�B���n�a�i��256�F�~16�g���p���b�g�L�����N�^��\��
//  �����F
//�@�@�@�@256�F�~16�g���p���b�g�n�a�i���A�A�t�B���g���i��]�g��k���j���s����
//      �\������B
//  ����F
//        UP,DOWN    : �\���ʒu�𑀍�
//        BUTTON_A,B : �g�k
//        BUTTON_L,R : ��]
//        SELECT     : �p���b�g�̐؂�ւ�
//        START      : �ݒ�l��������
//---------------------------------------------------------------------------
void NitroMain()
{
    int     base_pos_x = 0, base_pos_y = 0;
    int     base_pal = 0;
    int     base_pat = 0;
    fx32    scale = FX32_ONE;
    u16     rotate = 0;

    //---------------------
    // ������
    //---------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplayOBJOnly();

    /* VRAM�o���N��OBJ�g���p���b�g�Ɋ��蓖�Ă� */
    GX_SetBankForOBJExtPltt(GX_VRAM_OBJEXTPLTT_0_F);

    /* �L�����N�^�[�f�[�^�[�ƃp���b�g�f�[�^�[�̓ǂݍ��� */
    GX_LoadOBJ(bitmapOBJ_256color_icg, // OBJ�f�[�^��VRAM��A�]��
               0, sizeof(bitmapOBJ_256color_icg));
    {
        GX_BeginLoadOBJExtPltt();      // �p���b�g�f�[�^�̓]������
        GX_LoadOBJExtPltt(bitmapOBJ_256color_icl,       // �p���b�g�f�[�^��VRAM�ɓ]��
                          0, sizeof(bitmapOBJ_256color_icl));
        GX_EndLoadOBJExtPltt();        // �p���b�g�f�[�^�̓]������
    }

    DEMOStartDisplay();

    //---------------------
    //  ���C�����[�v
    //---------------------
    while (1)
    {
        int     i, j;
        int     pos_x = base_pos_x;
        int     pos_y = base_pos_y;
        int     palette = base_pal;

        /* �p�b�h���̎擾�Ƒ���ݒ� */
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] =
                { {PAD_BUTTON_A, 10}, {PAD_BUTTON_SELECT, 1}, {PAD_BUTTON_L, 10},
            {PAD_KEY_UP | PAD_KEY_RIGHT, 20}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            --base_pos_y;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            ++base_pos_y;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
        {
            ++base_pos_x;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_KEY_LEFT))
        {
            --base_pos_x;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_L))
        {
            rotate -= 512;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            rotate += 512;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_A))
        {
            scale += 128;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_B))
        {
            scale -= 128;
            if (scale < 0)
            {
                scale = 0;
            }
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_SELECT))
        {
            if (++base_pal > 15)
            {
                base_pal = 0;
            }
            OS_Printf("base_pal=%d\n", base_pal);
        }

        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            base_pal = 0;
            base_pos_x = 0;
            base_pos_y = 0;
            rotate = 0;
            scale = FX32_ONE;
            OS_Printf("pos=%d:%d rotate=%d scale=%d\n", base_pos_x, base_pos_y, rotate, scale);
        }

        /* �A�t�B���ϊ��}�g���b�N�X�̐����Ɛݒ� */
        {
            MtxFx22 mtx;
            fx16    sinVal = FX_SinIdx(rotate);
            fx16    cosVal = FX_CosIdx(rotate);
            fx32    rScale = FX_Inv(scale);
            GXOamAffine *oamBuffp = (GXOamAffine *)&s_OAMBuffer[0];

            mtx._00 = (fx32)((cosVal * rScale) >> FX32_SHIFT);
            mtx._01 = (fx32)((sinVal * rScale) >> FX32_SHIFT);
            mtx._10 = -(fx32)((sinVal * rScale) >> FX32_SHIFT);
            mtx._11 = (fx32)((cosVal * rScale) >> FX32_SHIFT);
            // �n�a�i�̃A�t�B���ϊ���ݒ�
            G2_SetOBJAffine(oamBuffp,  // OAM�o�b�t�@�̃|�C���^
                            &mtx);     // �A�t�B���ϊ��p2x2�s��
        }

        /* �n�`�l�̐ݒ�(16��32�~32�h�b�g�̂n�a�i��ݒ�) */
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                int     count = (i * 4) + j;
                int     pattern = ((count / 4) * 0x80) + ((count % 4) * 0x08);

                /* �n�a�i�̂n�`�l�A�g���r���[�g��ݒ� */
                G2_SetOBJAttr(&s_OAMBuffer[count],      // �n�`�l�̏ꏊ���w��
                              pos_x,   // �w�ʒu
                              pos_y,   // �x�ʒu
                              0,       // �D�揇��
                              GX_OAM_MODE_NORMAL,       // �ʏ�OBJ
                              FALSE,   // ���U�C�N����
                              GX_OAM_EFFECT_AFFINE,     // �A�t�B������
                              GX_OAM_SHAPE_32x32,       // �n�a�i�̑傫��
                              GX_OAM_COLOR_256, // �Q�T�U�F
                              pattern, // �L������
                              palette, // �J���[�p���b�g
                              0);
                pos_x += 32;
                if (++palette > 15)
                {
                    palette = 0;
                }
            }
            pos_x = base_pos_x;
            pos_y += 32;
        }

        /* �L���b�V�������C���������[�ɓf���o�� */
        DC_FlushRange(s_OAMBuffer, sizeof(s_OAMBuffer));
        /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
        // DC_WaitWriteBufferEmpty();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x8B9FC4A0);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        /* �u�a���������҂� */
        OS_WaitVBlankIntr();

        /* �n�`�l�֓]�� */
        GX_LoadOAM(s_OAMBuffer,        // OAM�o�b�t�@��OAM�֓]��
                   0, sizeof(s_OAMBuffer));
        MI_DmaFill32(3,                // OAM�o�b�t�@���N���A�[
                     s_OAMBuffer, 192, sizeof(s_OAMBuffer));
    }
}

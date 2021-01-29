/*---------------------------------------------------------------------------*
  Project:  NitroSDK - PAD - demos - padRead
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.30  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.29  2005/11/21 10:57:05  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.28  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.27  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.26  2004/06/11 11:24:06  terui
  Add Objs that show status of DEBUG_BUTTON and DETECT_FOLD.

  Revision 1.25  2004/04/15 06:51:31  yada
  only arrange comment

  Revision 1.24  2004/03/08 08:55:47  yada
  fix top comment

  Revision 1.23  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.22  2004/02/18 09:00:48  yada
  X,Y �{�^���\���ǉ�

  Revision 1.21  2004/02/10 05:48:12  yasu
  delete macro CODE32

  Revision 1.20  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.19  2004/01/29 08:07:40  yada
  GX_DispOn() �ǉ�


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include "data.h"

static GXOamAttr oamBak[128];

void    ObjSet(int objNo, int x, int y, int charNo, int paletteNo);
void    VBlankIntr(void);

//---- DMA �ԍ�
#define DMA_NO   3

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    //================ ������
    //---- OS ������
    OS_Init();

    //---- GX ������
    GX_Init();

    //================ �ݒ�
    //---- �d���S�ăI��
    GX_SetPower(GX_POWER_ALL);

    //----  V�u�����N��������
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();

    //---- V�u�����N�����ݒ�
    (void)GX_VBlankIntr(TRUE);

    //---- VRAM �N���A
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();

    //---- OAM�ƃp���b�g�N���A
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);


    //---- �o���N�`���n�a�i��
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);

    //---- �O���t�B�b�N�X�\�����[�h�ɂ���
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);

    //---- OBJ�̕\���̂�ON
    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);

    //---- 32K�o�C�g��OBJ��2D�}�b�v���[�h�Ŏg�p
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);

    //---- �f�[�^���[�h
    MI_DmaCopy32(DMA_NO, sampleCharData, (void *)HW_OBJ_VRAM, sizeof(sampleCharData));
    MI_DmaCopy32(DMA_NO, samplePlttData, (void *)HW_OBJ_PLTT, sizeof(samplePlttData));

    //---- ��\��OBJ�͉�ʊO�ֈړ�
    MI_DmaFill32(DMA_NO, oamBak, 0xc0, sizeof(oamBak));

    //---- �\���J�n
    OS_WaitVBlankIntr();
    GX_DispOn();

    //================ ���C�����[�v
    while (1)
    {
        u16     keyData;

        //---- V�u�����N�����I���҂�
        OS_WaitVBlankIntr();

        //---- �p�b�h�f�[�^�ǂݍ���
        keyData = PAD_Read();

        //---- �p�b�h���� OBJ �ŕ\��
        ObjSet(0, 200, 90, 'A', (keyData & PAD_BUTTON_A) ? 1 : 2);
        ObjSet(1, 180, 95, 'B', (keyData & PAD_BUTTON_B) ? 1 : 2);

        ObjSet(2, 60, 50, 'L', (keyData & PAD_BUTTON_L) ? 1 : 2);
        ObjSet(3, 180, 50, 'R', (keyData & PAD_BUTTON_R) ? 1 : 2);

        ObjSet(4, 60, 80, 'U', (keyData & PAD_KEY_UP) ? 1 : 2);
        ObjSet(5, 60, 100, 'D', (keyData & PAD_KEY_DOWN) ? 1 : 2);
        ObjSet(6, 50, 90, 'L', (keyData & PAD_KEY_LEFT) ? 1 : 2);
        ObjSet(7, 70, 90, 'R', (keyData & PAD_KEY_RIGHT) ? 1 : 2);

        ObjSet(8, 130, 95, 'S', (keyData & PAD_BUTTON_START) ? 1 : 2);
        ObjSet(9, 110, 95, 'S', (keyData & PAD_BUTTON_SELECT) ? 1 : 2);

        ObjSet(10, 200, 75, 'X', (keyData & PAD_BUTTON_X) ? 1 : 2);
        ObjSet(11, 180, 80, 'Y', (keyData & PAD_BUTTON_Y) ? 1 : 2);

        //---- �f�o�b�O�{�^������ OBJ �ŕ\��
        ObjSet(12, 200, 120, 'D', (keyData & PAD_BUTTON_DEBUG) ? 1 : 2);
        //---- �܂��݌��m��Ԃ� OBJ �ŕ\��
        ObjSet(13, 120, 30, 'F', (PAD_DetectFold())? 1 : 2);
    }
}

//--------------------------------------------------------------------------------
//  OBJ �Z�b�g
//
void ObjSet(int objNo, int x, int y, int charNo, int paletteNo)
{
    G2_SetOBJAttr((GXOamAttr *)&oamBak[objNo],
                  x,
                  y,
                  0,
                  GX_OAM_MODE_NORMAL,
                  FALSE,
                  GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_16, charNo, paletteNo, 0);
}


//--------------------------------------------------------------------------------
//    �u�u�����N���荞�ݏ���
//
void VBlankIntr(void)
{
    //---- �n�`�l�̍X�V
    DC_FlushRange(oamBak, sizeof(oamBak));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    MI_DmaCopy32(DMA_NO, oamBak, (void *)HW_OAM, sizeof(oamBak));

    //---- ���荞�݃`�F�b�N�t���O
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*====== End of main.c ======*/

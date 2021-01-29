/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - timer-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.18  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.17  2005/11/21 10:56:23  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.16  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.15  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.14  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.13  2004/03/30 08:38:22  yada
  ���C��

  Revision 1.12  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.11  2004/02/10 05:48:12  yasu
  delete macro CODE32

  Revision 1.10  2004/02/05 10:50:13  yada
  timer�Đݒ菈��������悤�ɂ���

  Revision 1.9  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.8  2004/02/02 05:34:12  yada
  OS_SetIrqMask()��OS_EnableIrqMask() ��

  Revision 1.7  2004/01/29 08:07:40  yada
  GX_DispOn() �ǉ�

  Revision 1.6  2004/01/18 07:29:23  yada
  GX�֐��̖��̕ύX�ɔ����C��

  Revision 1.5  2004/01/18 06:47:24  yada
  OS_Init �ǉ�

  Revision 1.4  2004/01/14 11:25:47  nishida_kenji
  add G2_OBJMosaic,
  remove GXOamMosaic

  Revision 1.3  2004/01/09 10:53:49  yada
  �����݃`�F�b�N�t���O�̃Z�b�g�֐���������

  Revision 1.2  2004/01/08 12:02:24  yada
  OAM�f�[�^�]���O�ɃL���b�V���t���b�V������悤�ɂ���

  Revision 1.1  2004/01/08 06:32:45  yada
  ����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include "data.h"

static GXOamAttr oamBak[128];

void    VBlankIntr(void);
void    ObjSet(int objNo, int x, int y, int charNo, int paletteNo);
void    TimerSet(void);
void    TimerIntr(void);

static volatile u32 timerCnt = 0;

//---- OBJ �\���p�� 0�`9,A�`F �̃R�[�h��Ԃ�
inline int ObjChar(u32 cnt, int shift)
{
    u32     d = (cnt >> shift) & 0xf;
    return (int)((d < 10) ? '0' + d : 'A' + d - 10);
}

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
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);        // �o���N�`���n�a�i�Ɋ��蓖�Ă�

    //---- �O���t�B�b�N�X�\�����[�h�ɂ���
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);

    //---- OBJ�̕\���̂�ON
    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);

    //---- 32K�o�C�g��OBJ��2D�}�b�v���[�h�Ŏg�p
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);


    //---- �f�[�^���[�h
    MI_DmaCopy32(3, sampleCharData, (void *)HW_OBJ_VRAM, sizeof(sampleCharData));
    MI_DmaCopy32(3, samplePlttData, (void *)HW_OBJ_PLTT, sizeof(samplePlttData));

    //---- ��\��OBJ�͉�ʊO�ֈړ�
    MI_DmaFill32(3, oamBak, 0xc0, sizeof(oamBak));

    //---- �\���J�n
    OS_WaitVBlankIntr();
    GX_DispOn();

    //================ ���C�����[�v
    //---- �^�C�}�����ݐݒ�
    TimerSet();

    while (1)
    {
        u16     keyData;

        //---- V�u�����N�����I���҂�
        OS_WaitVBlankIntr();

        //---- �p�b�h�f�[�^�ǂݍ���
        keyData = PAD_Read();

        //---- �J�E���^�l�\��
        ObjSet(0, 50, 100, ObjChar(timerCnt, 28), 2);
        ObjSet(1, 60, 100, ObjChar(timerCnt, 24), 2);
        ObjSet(2, 70, 100, ObjChar(timerCnt, 20), 2);
        ObjSet(3, 80, 100, ObjChar(timerCnt, 16), 2);
        ObjSet(4, 90, 100, ObjChar(timerCnt, 12), 2);
        ObjSet(5, 100, 100, ObjChar(timerCnt, 8), 2);
        ObjSet(6, 110, 100, ObjChar(timerCnt, 4), 2);
        ObjSet(7, 120, 100, ObjChar(timerCnt, 0), 2);

        //---- �{�^���Ń^�C�}�����݋��^�֎~
        if (keyData & PAD_BUTTON_A)
        {
            (void)OS_EnableIrqMask(OS_IE_TIMER0);
        }
        if (keyData & PAD_BUTTON_B)
        {
            (void)OS_DisableIrqMask(OS_IE_TIMER0);
        }
    }
}

//--------------------------------------------------------------------------------
//      OBJ �Z�b�g
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
    MI_DmaCopy32(3, oamBak, (void *)HW_OAM, sizeof(oamBak));

    //---- ���荞�݃`�F�b�N�t���O
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//--------------------------------------------------------------------------------
//              �^�C�}�ݒ�
//
void TimerSet(void)
{
    //---- �^�C�}�O�̊����݃��[�`���ݒ�
    OS_SetIrqFunction(OS_IE_TIMER0, TimerIntr);

    //---- �^�C�}�O�����ݗL����
    (void)OS_EnableIrqMask(OS_IE_TIMER0);

    //---- �^�C�}�O�X�^�[�g
    OS_StartTimer(OS_TIMER_0, 1000, OS_TIMER_PRESCALER_1024);
}

//--------------------------------------------------------------------------------
//              �^�C�}�����ݏ���
//
void TimerIntr(void)
{
    timerCnt = timerCnt + 1;

    //---- ���荞�݃`�F�b�N�t���O
    OS_SetIrqCheckFlag(OS_IE_TIMER0);

    //---- ��U�^�C�}�O����
    OS_StopTimer(OS_TIMER_0);

    //---- �^�C�}�Đݒ�
    TimerSet();
}

/*====== End of main.c ======*/
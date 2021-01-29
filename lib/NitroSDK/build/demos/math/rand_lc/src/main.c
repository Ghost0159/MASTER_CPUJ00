/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MATH - demos
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.3  2005/11/21 10:48:03  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.2  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.1  2004/12/14 13:33:01  takano_makoto
  FX����MATH�ֈړ�

  Revision 1.1  2004/12/13 07:54:18  takano_makoto
  RND����FX�ֈړ�

  Revision 1.1  2004/12/09 12:45:20  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  ���`�����@�ɂ�闐���l�擾�f��
 *---------------------------------------------------------------------------*/

#include    <nitro.h>



static void VBlankIntr(void);
static void KeyRead(void);
static void DisplayInit();
static void PutDot(u16 x, u16 y, u16 col);

/*---------------------------------------------------------------------------*
    �ϐ���`
 *---------------------------------------------------------------------------*/
static struct
{
    u16     trig;
    u16     press;
}
gKey;                                  // �L�[����



/*---------------------------------------------------------------------------*
    �֐���`
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �������y�у��C�����[�v�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    MATHRandContext16 rnd;
    u16     init_rnd = 0;

    // �e�평����
    OS_Init();

    DisplayInit();

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    KeyRead();

    OS_TPrintf("press any button\n");

    // ���C�����[�v
    while (TRUE)
    {
        static u16 col = 0;
        static u16 *pVram = (u16 *)HW_LCDC_VRAM_C;
        static u16 *pbVram = (u16 *)(HW_LCDC_VRAM_C + 0x18000 - 2);

        // �u�u�����N�҂�
        OS_WaitVBlankIntr();

        // �L�[���͏��擾
        KeyRead();

        if (!init_rnd)
        {
            if (gKey.trig)
            {
                // �������������A�����ł�VBlank�J�E���^�̒l����Ƃ��Ďg�p
                MATH_InitRand16(&rnd, OS_GetVBlankCount());
                init_rnd = 1;
            }
            continue;
        }

        {
            u16     x, y;
            u32     i;

            for (i = 0; i < 0x400; i++)
            {
                // �������擾
                x = (u16)MATH_Rand16(&rnd, HW_LCD_WIDTH);
                y = (u16)MATH_Rand16(&rnd, HW_LCD_HEIGHT);

                PutDot(x, y, col);
            }
        }

        // ���ׂĔ��^��
        while (*pVram == col)
        {
            pVram++;
            if (pVram > pbVram)
            {
                pVram = (u16 *)HW_LCDC_VRAM_C;
                pbVram = (u16 *)(HW_LCDC_VRAM_C + 0x18000 - 2);
                col ^= 0x7FFF;
            }
        }
        while (*pbVram == col)
        {
            pbVram--;
            if (pVram > pbVram)
            {
                pVram = (u16 *)HW_LCDC_VRAM_C;
                pbVram = (u16 *)(HW_LCDC_VRAM_C + 0x18000 - 2);
                col ^= 0x7FFF;
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �u�u�����N�����݃x�N�g���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // IRQ �`�F�b�N�t���O���Z�b�g
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  �L�[���͏���ҏW����B
                �����g���K�A�����g���K�A�����p�����s�[�g�����o�B

  Arguments:    pKey  - �ҏW����L�[���͏��\���́B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void KeyRead(void)
{
    u16     readData = PAD_Read();
    gKey.trig = (u16)(readData & (readData ^ gKey.press));
    gKey.press = readData;
}


/*---------------------------------------------------------------------------*
  Name:         DisplayInit

  Description:  Graphics Initialization

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DisplayInit()
{

    GX_Init();
    FX_Init();

    GX_DispOff();
    GXS_DispOff();

    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request
    (void)OS_EnableIrq();


    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);   // clear OAM
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);     // clear the standard palette

    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);     // clear OAM
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);       // clear the standard palette
    MI_DmaFill32(3, (void *)HW_LCDC_VRAM_C, 0x7FFF7FFF, 256 * 192 * sizeof(u16));


    GX_SetBankForOBJ(GX_VRAM_OBJ_256_AB);       // Set VRAM-A,B for OBJ

    GX_SetGraphicsMode(GX_DISPMODE_VRAM_C,      // VRAM mode.
                       (GXBGMode)0,    // dummy
                       (GXBG0As)0);    // dummy

    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);       // make OBJ visible
    GX_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_1D_128K);   // 2D mapping OBJ

    OS_WaitVBlankIntr();               // Waiting the end of VBlank interrupt
    GX_DispOn();

}


/*---------------------------------------------------------------------------*
  Name:         PutDot

  Description:  Draw a Dot in VRAM for LCDC.

  Arguments:    x   - position X.
                y   - position Y.
                col - DotColor.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void PutDot(u16 x, u16 y, u16 col)
{
    if (x >= HW_LCD_WIDTH || y >= HW_LCD_HEIGHT)
    {
        return;
    }
    *(u16 *)(HW_LCDC_VRAM_C + y * 256 * 2 + x * 2) = col;
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

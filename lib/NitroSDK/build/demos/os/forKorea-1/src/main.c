/*---------------------------------------------------------------------------*
  Project:  NitroSDK - demos - OS - forKorea-1
  File:     main.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.1  2006/10/19 01:32:30  okubata_ryoma
  �T���v���f��forKorea-1�̒ǉ�


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/rtc.h>

#include    "font.h"

/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void);
static void ClearString(void);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static OSOwnerInfo gOwnerInfo;
static u16 gScreen[32 * 32];           // ���z�X�N���[��


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �������y�у��C�����[�v�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // �e�평����
    OS_Init();

    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    // �\���ݒ菉����
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    // ������\���p�ɂQ�c�\���ݒ�
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,      // SCR �x�[�X�u���b�N 31
                     GX_BG_CHARBASE_0x00000,    // CHR �x�[�X�u���b�N 0
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    G2_SetBG0Offset(0, 0);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    MI_CpuClearFast(gScreen, sizeof(gScreen));
    DC_StoreRange(gScreen, sizeof(gScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // �����ݐݒ�
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    // LCD�\���J�n
    GX_DispOn();

    // �f�o�b�O������o��
    OS_Printf("forKorea-1 test started.\n");

    //****************************************
    // �I�[�i�[���擾
    OS_GetOwnerInfo(&gOwnerInfo);
    //****************************************

    // ���C�����[�v
    {
        s32     frame;

        for (frame = 0; TRUE; frame++)
        {
            // �X�N���[���N���A
            ClearString();
            
            // �����\��
            PrintString(4, 4, 0x0, "Frame : %d", frame);
            PrintString(4, 8, 0x3, "LanguageCode : %d", gOwnerInfo.language);

            // �u�u�����N�҂�
            OS_WaitVBlankIntr();
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
    // ���z�X�N���[����VRAM�ɔ��f
    DC_StoreRange(gScreen, sizeof(gScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // IRQ �`�F�b�N�t���O���Z�b�g
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  ���z�X�N���[�����N���A����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ClearString(void)
{
    MI_CpuClearFast(gScreen, sizeof(gScreen));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  ���z�X�N���[���ɕ������z�u����B�������32�����܂ŁB

  Arguments:    x       - ������̐擪��z�u���� x ���W( �~ 8 �h�b�g )�B
                y       - ������̐擪��z�u���� y ���W( �~ 8 �h�b�g )�B
                palette - �����̐F���p���b�g�ԍ��Ŏw��B
                text    - �z�u���镶����B�I�[������NULL�B
                ...     - ���z�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...)
{
    va_list vlist;
    char    temp[32 + 2];
    s32     i;

    va_start(vlist, text);
    (void)vsnprintf(temp, 33, text, vlist);
    va_end(vlist);

    *(u16 *)(&temp[32]) = 0x0000;
    for (i = 0;; i++)
    {
        if (temp[i] == 0x00)
        {
            break;
        }
        gScreen[((y * 32) + x + i) % (32 * 32)] = (u16)((palette << 12) | temp[i]);
    }
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - shared
  File:     screen.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: screen.c,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/06 00:23:20  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#include "screen.h"
#include "output.h"

/*****************************************************************************/

/* variable */
static u16  main_text_scr[32][32] ATTRIBUTE_ALIGN(32);
static BOOL main_text_modified;

/*****************************************************************************/

/* function */

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V �u�����N �R�[���o�b�N.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
    FlushScreen();
    FlushDebugOutput();
}

/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  �\���ݒ菉����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitScreen(void)
{
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    /* �����������N���A */
    main_text_modified = FALSE;
    MI_CpuClearFast(main_text_scr, sizeof(main_text_scr));

    /* VRAM �N���A */
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void*)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void*)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void*)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void*)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void*)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    /*
     * VRAM-A  = main-BG
     * VRAM-HI = sub-BG
     */
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_48_HI);

    /*
     * ���C�����:
     *   BG0 = text-BG
     */
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x00000,
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    G2_SetBG0Offset(0, 0);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    /*
     * �T�u���:
     *   BG0 = text-BG
     */
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xb800, GX_BG_CHARBASE_0x00000,
                      GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    G2S_BG0Mosaic(FALSE);
    G2S_SetBG0Offset(0, 0);
    GXS_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GXS_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    /* �\���L���� */
    GX_DispOn();
    GXS_DispOn();

    /* V�u�����N�ݒ� */
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    /* ��ʂ̏㉺�؂�ւ� */
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
}

/*---------------------------------------------------------------------------*
  Name:         ClearScreen

  Description:  �e�L�X�g�X�N���[���̏���.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ClearScreen(void)
{
    MI_CpuClearFast(main_text_scr, sizeof(main_text_scr));
    main_text_modified = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         PadkeyClearScreen

  Description:  Padkey���g�p���镔���̃e�L�X�g�X�N���[���̏���.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PadkeyClearScreen(void)
{
    MI_CpuClearFast(main_text_scr[0], sizeof(main_text_scr[0]));
    MI_CpuClearFast(main_text_scr[4], sizeof(main_text_scr[4]));
    MI_CpuClearFast(main_text_scr[23], sizeof(main_text_scr[23]));
    main_text_modified = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  �e�L�X�g�X�N���[���ɕ������\��.

  Arguments:    x             �O���b�h�P�ʂ� X ���W
                y             �O���b�h�P�ʂ� Y ���W
                palette       �p���b�g�C���f�b�N�X
                format        ����������

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PrintString(int x, int y, int palette, const char* format, ...)
{
    va_list vlist;
    char    temp[32 + 1];
    u16*    dst;
    int     i;

    va_start(vlist, format);
    (void)OS_VSNPrintf(temp, sizeof(temp), format, vlist);
    va_end(vlist);

    dst = main_text_scr[y];
    for (i = 0; (x + i < 32) && temp[i]; ++i)
    {
        dst[x + i] = (u16) ((palette << 12) | (u8) temp[i]);
    }

    main_text_modified = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         PrintDebugString

  Description:  �T�u��ʃe�L�X�g�X�N���[���Ƀf�o�b�O�������\��.

  Arguments:    s             �o�͕�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PrintDebugString(const char* s, ...)
{
    char    tmp[256];
    va_list va;
    va_start(va, s);
    (void)OS_VSNPrintf(tmp, sizeof(tmp), s, va);
    va_end(va);
    DebugOutput(tmp);
}

/*---------------------------------------------------------------------------*
  Name:         FlushScreen

  Description:  �e�L�X�g�X�N���[�������̏��VRAM�֔��f.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FlushScreen(void)
{
    OSIntrMode  bak_cpsr = OS_DisableInterrupts();
    BOOL        modified = main_text_modified;
    if (modified)
    {
        //PrintString(0, 0, COLOR_WHITE, "%s", GetTestName());
        main_text_modified = FALSE;
    }
    (void)OS_RestoreInterrupts(bak_cpsr);

    /* �o�b�N�O���E���h�ł̎����X�V���l��, �����Ŋ��荞�݂͋֎~���Ȃ� */
    if (modified)
    {
        DC_FlushRange(main_text_scr, sizeof(main_text_scr));
        GX_LoadBG0Scr(main_text_scr, 0, sizeof(main_text_scr));
    }
}

/*---------------------------------------------------------------------------*
  Name:         ShowNavigater

  Description:  �e�L�X�g�X�N���[���̍ŉ��s�ɁA�I�y���[�^��U�����郁�b�Z�[�W
                ��\������B���t���[���Ăяo�����Ƃɂ��A�_�ŕ\�������B

  Arguments:    text    -   �\�����郁�b�Z�[�W�e�L�X�g�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ShowNavigater(const char* text)
{
    char    temp[33] = { "                                " };
    s32     i;

    for (i = 0; i < 31; i++)
    {
        if (text[i] == 0x00)
        {
            break;
        }

        temp[i + 1] = text[i];
    }

    PrintString(0, 22, GetFlickerColor(), temp);
}

/*---------------------------------------------------------------------------*
  Name:         GetFlickerColor

  Description:  �_�ŗp�̕�����F���擾����B

  Arguments:    None.

  Returns:      int     -   �p���b�g�ԍ�
 *---------------------------------------------------------------------------*/
int GetFlickerColor(void)
{
    return (int)(5 + ((OS_GetVBlankCount() / 30) % 2));
}

/*---------------------------------------------------------------------------*
  Name:         PrintStep

  Description:  �e�L�X�g�X�N���[���̎w��ʒu�ɁA�菇�������\���B
                �������ƍs���ɉ����ēK�X���s����B

  Arguments:    x       -   �\�����_ x ���W�B
                y       -   �\�����_ y ���W�B
                palette -   �\���p���b�g�F�B (COLOR_*)
                step    -   ���݂̃X�e�b�v�ԍ��B
                comment -   �e�X�g�菇�������R�����g������B

  Returns:      �\���ɗv�����s��.
 *---------------------------------------------------------------------------*/
int PrintStep(int x, int y, int palette, int step, const char* comment)
{
    static const char   blank[32 + 1] = "                                ";
    char        c;
    int         len, base;
    const int   oy = y;
    const int   limit = 29 - x - 4;

    for (; y < 23; ++y)
    {
        if (y == oy)
        {
            PrintString(x, y, palette, " %d. ", step);
        }
        else
        {
            PrintString(x, y, palette, "    ");
        }

        /* 1 �s�ɕ\���\�ȕ����������� */
        len = 0;
        base = 0;
        for (;;)
        {
            /* �� */
            while (((c = comment[len]) != '\0') && ((c == ' ') || (c == '\t')))
            {
                ++len;
            }

            if (base == 0)
            {
                comment += len;
                len = 0;
            }

            /* �g�[�N�� */
            while (((c = comment[len]) != '\0') && ((c != ' ') && (c != '\t')))
            {
                ++len;
            }

            if (len > limit)
            {
                break;
            }

            base = len;
            if (c == '\0')
            {
                break;
            }
        }

        PrintString(x + 4, y, palette, "%.*s%.*s", base, comment, limit - base, blank);
        comment += base;
        if (comment[0] == '\0')
        {
            break;
        }
    }

    return y - oy + 1;
}

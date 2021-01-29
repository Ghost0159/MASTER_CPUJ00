/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     util.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: util.c,v $
  Revision 1.3  2005/11/21 10:48:24  kitase_hirotake
  OS_InitPrintServer �̍폜

  Revision 1.2  2005/06/27 11:10:11  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.1  2005/06/23 09:05:57  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include    "util.h"


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/

static u16 gScreen[32 * 32];           /* ���z�X�N���[�� */
static u16 repeat_count[12];           /* �L�[���s�[�g */
static char recent_output[24][32 + 1];


/*---------------------------------------------------------------------------*
  Name:         InitializeAllocateSystem

  Description:  ���C����������̃A���[�i�ɂă����������ăV�X�e����������.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}


/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �u�u�����N�����݃x�N�g���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    /* ���z�X�N���[����VRAM�ɔ��f */
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    /* IRQ �`�F�b�N�t���O���Z�b�g */
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}


/*---------------------------------------------------------------------------*
  Name:         UTIL_Init

  Description:  �T���v���̃t���[�����[�N������.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void UTIL_Init(void)
{
    /* �e�평���� */
    OS_Init();
    OS_InitTick();

    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    /* �\���ݒ菉���� */
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    /* ������\���p�ɂQ�c�\���ݒ� */
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                     GX_BG_COLORMODE_16,
                     GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    /* �����ݐݒ� */
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    /* ���������� */
    InitializeAllocateSystem();
}


/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  �L�[�f�[�^�̎擾.

  Arguments:    pKey : �L�[�����i�[����A�h���X

  Returns:      None.
 *---------------------------------------------------------------------------*/
void KeyRead(KeyInfo * pKey)
{
    enum
    {
        KEY_BIT_MAX = 12,
        KEY_REPEAT_START = 25,         /* �L�[���s�[�g�J�n�܂ł̃t���[���� */
        KEY_REPEAT_SPAN = 10           /* �L�[���s�[�g�̊Ԋu�t���[���� */
    };

    int     i;
    const u16 r = PAD_Read();
    pKey->trg = 0x0000;
    pKey->up = 0x0000;
    pKey->rep = 0x0000;

    for (i = 0; i < KEY_BIT_MAX; ++i)
    {
        const u16 bit = (u16)(1 << i);
        if ((r & bit) != 0)
        {
            if (!(pKey->cnt & bit))
            {
                pKey->trg |= bit;
                repeat_count[i] = 1;
            }
            else if (repeat_count[i] > KEY_REPEAT_START)
            {
                pKey->rep |= bit;
                repeat_count[i] = KEY_REPEAT_START - KEY_REPEAT_SPAN;
            }
            else
            {
                ++repeat_count[i];
            }
        }
        else if ((pKey->cnt & bit) != 0)
        {
            pKey->up |= bit;
        }
    }
    pKey->cnt = r;
}

/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  �\���p���z�X�N���[���̏�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ClearString(void)
{
    MI_CpuClearFast(gScreen, sizeof(gScreen));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  ���z�X�N���[����32�����ȓ��̕������\��.

  Arguments:    x       - ������̐擪��z�u���� x ���W / 8 dot
                y       - ������̐擪��z�u���� y ���W / 8 dot
                palette - �����F���w�肷��p���b�g�ԍ�
                text    - �\��������
                ...     - �㑱�̉ψ���

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PrintString(int x, int y, int palette, const char *text, ...)
{
    va_list vlist;
    static char temp[64 + 2];
    u8      d;
    int     i;

    va_start(vlist, text);
    (void)OS_VSNPrintf(temp, sizeof(temp) - 1, text, vlist);
    va_end(vlist);
    *(u16 *)&temp[32] = 0x0000;
    for (i = 0; (d = MI_ReadByte(temp + i)) != '\0'; ++i)
    {
        if ((d == '\r') || (d == '\n'))
            break;
        gScreen[((y * 32) + x + i) % (32 * 32)] = (u16)((palette << 12) | d);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  ���z�X�N���[���ɕ\������Ă��镶����̐F��ύX.

  Arguments:    x       - �F�ύX���J�n���� x ���W / 8 dot
                y       - �F�ύX���J�n���� y ���W / 8 dot
                length  - �F�ύX���镶����
                palette - �����F���w�肷��p���b�g�ԍ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ColorString(int x, int y, int length, int palette)
{
    int     i;
    for (i = 0; i < length; ++i)
    {
        const int index = ((y * 32) + x + i) % (32 * 32);
        gScreen[index] = (u16)((gScreen[index] & 0x0FFF) | (palette << 12));
    }
}

/*---------------------------------------------------------------------------*
  Name:         ClearLine

  Description:  ������`���������.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ClearLine(void)
{
    MI_CpuClear8(recent_output, sizeof(recent_output));
}

/*---------------------------------------------------------------------------*
  Name:         PrintLine

  Description:  �w�胉�C����1 �t���[���ȏ�c��s�����ȕ������`��.

  Arguments:    x          �\���ʒu x �O���b�h. (8 pixel �P��)
                y          �\���ʒu y �O���b�h. (8 pixel �P��)
                text       �㑱�̉ψ������󂯂鏑��������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PrintLine(int x, int y, const char *text, ...)
{
    va_list va;
    va_start(va, text);
    MI_CpuFill8(recent_output[y], ' ', sizeof(recent_output[y]));
    (void)OS_VSNPrintf(&recent_output[y][x], (size_t) (32 - x), text, va);
    va_end(va);
}

/*---------------------------------------------------------------------------*
  Name:         FlushLine

  Description:  �ʏ�̕�����`��� PrintLine() �̕s������������㏑��.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FlushLine(void)
{
    int     y;
    for (y = 0; y < 24; ++y)
    {
        PrintString(0, y, COLOR_WHITE, recent_output[y]);
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

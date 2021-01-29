/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-PowerSave
  File:     dispfunc.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dispfunc.c,v $
  Revision 1.3  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.2  2005/06/27 11:10:13  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.1  2005/04/28 07:15:24  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include "dispfunc.h"


/*****************************************************************************/
/* constant */

/* �e��`��p�萔�z�� */
extern const u32 sampleCharData[8 * 0xe0];
extern const u16 samplePlttData[16][16];


/*****************************************************************************/
/* variable */

/* V�u�����N�]���p�̉��z BG �X�N���[�� */
static u16 g_screen[2][32 * 32] ATTRIBUTE_ALIGN(32);

/* ���݂̕`��Ώ� */
static u16 *draw_target;

/* ���b�Z�[�W���O�̍ŐV�s�ԍ� */
static int cur_line = 0;


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         BgiGetTargetScreen

  Description:  ���C��/�T�u��� BG �X�N���[���ւ̃|�C���^���擾.

  Arguments:    is_lcd_main      ���C����� �Ȃ� TRUE, �T�u��ʂȂ� FALSE.

  Returns:      BG �X�N���[���ւ̃|�C���^.
 *---------------------------------------------------------------------------*/
static  inline u16 *BgiGetTargetScreen(BOOL is_lcd_main)
{
    return g_screen[!is_lcd_main];
}

/*---------------------------------------------------------------------------*
  Name:         BgiSetTargetScreen

  Description:  �������\�����郁�C��/�T�u��� BG �X�N���[����؂�ւ�.

  Arguments:    is_lcd_main      ���C����� �Ȃ� TRUE, �T�u��ʂȂ� FALSE.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static  inline void BgiSetTargetScreen(BOOL is_lcd_main)
{
    draw_target = BgiGetTargetScreen(is_lcd_main);
}

/*---------------------------------------------------------------------------*
  Name:         BgInit

  Description:  BG �𕶎���\���p�ɏ�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgInit(void)
{
    /* �����ϐ��̏����� */
    cur_line = 0;
    MI_CpuClearFast(g_screen, sizeof(g_screen));
    DC_StoreRange(g_screen, sizeof(g_screen));
    DC_WaitWriteBufferEmpty();
    draw_target = BgiGetTargetScreen(TRUE);

    /*
     * ���C�� LCD �ݒ�.
     * BG0 : ASCII �����̕\���Ɏg�p.
     *   -256x256x16 �e�L�X�gBG.
     *   -�X�N���[���x�[�X 30(0F000-0F800).
     *   -�L�����N�^�x�[�X  0(00000-04000).
     */
    GX_SetBankForBG(GX_VRAM_BG_64_E);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                     GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_SetBG0Offset(0, 0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_LoadBG0Scr(g_screen[0], 0, sizeof(g_screen[0]));
    GX_LoadBG0Char(sampleCharData, 0, sizeof(sampleCharData));
    GX_LoadBGPltt(samplePlttData, 0, sizeof(samplePlttData));
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);

    /*
     * �T�u LCD �ݒ�.
     * BG0 : ASCII �����̕\���Ɏg�p.
     *   -256x256x16 �e�L�X�gBG.
     *   -�X�N���[���x�[�X 31(0F800-10000).
     *   -�L�����N�^�x�[�X  4(10000-14000).
     */
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                      GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    G2S_SetBG0Offset(0, 0);
    G2S_BG0Mosaic(FALSE);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_LoadBG0Scr(g_screen[1], 0, sizeof(g_screen[1]));
    GXS_LoadBG0Char(sampleCharData, 0, sizeof(sampleCharData));
    GXS_LoadBGPltt(samplePlttData, 0, sizeof(samplePlttData));
    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);
}

/*---------------------------------------------------------------------------*
  Name:         BgClear

  Description:  BG ���C����ʃX�N���[����S�ăL�����N�^ 0 �ŏ�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgClear(void)
{
    MI_CpuClearFast(g_screen[0], sizeof(g_screen[0]));
}

/*---------------------------------------------------------------------------*
  Name:         BgUpdate

  Description:  BG �X�N���[�������������ɔ��f.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgUpdate(void)
{
    /* BG �����������ɔ��f */
    DC_FlushRange(g_screen, sizeof(g_screen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(g_screen[0], 0, sizeof(g_screen[0]));
    GXS_LoadBG0Scr(g_screen[1], 0, sizeof(g_screen[1]));
}

/*---------------------------------------------------------------------------*
  Name:         BgPutString

  Description:  BG �̎w��O���b�h���牡������ ASCII �������\��.

  Arguments:    x                �\�����鍶�� x �O���b�h. (8pixel�P��)
                y                �\�����鍶�� y �O���b�h. (8pixel�P��)
                pal              �p���b�g�ԍ�. (0-15)
                str              �\������ ASCII ������.
                len              �\�����镶����.
                                 str ����������Z���ʒu�� NUL ���܂ޏꍇ
                                 ���̈ʒu�܂ł̒������̗p�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgPutString(int x, int y, int pal, const char *str, int len)
{
    u16    *const dst = draw_target;
    x += y * 32;
    str -= x, len += x;
    for (; str[x] && (x < len); ++x)
    {
        dst[x & ((32 * 32) - 1)] = (u16)((pal << 12) | (u8)str[x]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         BgPrintf

  Description:  BG �̎w��O���b�h���牡�����ɏ����t�� ASCII �������\��.

  Arguments:    x                �\�����鍶�� x �O���b�h. (8pixel�P��)
                y                �\�����鍶�� y �O���b�h. (8pixel�P��)
                pal              �p���b�g�ԍ�. (0-15)
                str              �\������ ASCII ������.
                                 �Ή������� OS_VSNPrintf() �ɏ�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgPrintf(int x, int y, int pal, const char *str, ...)
{
    char    tmp[32 + 1];
    va_list vlist;
    va_start(vlist, str);
    (void)OS_VSNPrintf(tmp, sizeof(tmp), str, vlist);
    va_end(vlist);
    BgPutString(x, y, pal, tmp, sizeof(tmp));
}

/*---------------------------------------------------------------------------*
  Name:         BgSetMessage

  Description:  ���C��/�T�u��ʗ����� (4, 22) �̈ʒu�ɕ�����\��.

  Arguments:    pal              �p���b�g�ԍ�. (0-15)
                str              �\������ ASCII ������.
                                 �Ή������� OS_VSNPrintf() �ɏ�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgSetMessage(int pal, const char *str, ...)
{
    char    tmp[32 + 1];
    va_list vlist;
    va_start(vlist, str);
    (void)OS_VSNPrintf(tmp, sizeof(tmp), str, vlist);
    va_end(vlist);

    /* ���C����� (4, 22) �ɕ�����\�� */
    BgPutString(4, 22, pal, "                            ", 28);
    BgPutString(4, 22, pal, tmp, 28);
    /* �T�u��ʂ̍ŐV�s�ɕ������ǉ��\�� */
    {
        /* �K�v�ɉ����ăX�N���[�� */
        const int BG_LINES = 24;
        if (cur_line == BG_LINES)
        {
            u16    *const sub_screen = BgiGetTargetScreen(FALSE);
            MI_CpuCopy16(&sub_screen[32 * 1], sub_screen, sizeof(u16) * 32 * (BG_LINES - 1));
            MI_CpuClear16(&sub_screen[32 * (BG_LINES - 1)], sizeof(u16) * 32 * 1);
            --cur_line;
        }
        /* �ꎞ�I�ɏo�͐��؂�ւ��� */
        BgiSetTargetScreen(FALSE);
        BgPutString(0, cur_line, pal, tmp, sizeof(tmp));
        BgiSetTargetScreen(TRUE);
        if (cur_line < BG_LINES)
            ++cur_line;
    }
}

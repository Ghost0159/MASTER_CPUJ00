/*---------------------------------------------------------------------------*
  Project:  NitroSDK - VIB - demos - pulse_vib
  File:     char_data.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: char_data.c,v $
  Revision 1.1  2006/03/02 10:08:34  okubata_ryoma
  ƒTƒ“ƒvƒ‹ƒfƒ‚pulse_vib‚Ì’Ç‰Á


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
 /*! @file
    @brief      Debug print

    @author     kitayama

    @version 1.00       initial release.
  */

#include <char_data.h>

/*-----------------------------------------------------------------------*
                    Function prototype
 *-----------------------------------------------------------------------*/

//--- Auto Function Prototype --- Don't comment here.
void    dp_init(void);
void    dp_set_pltt(s32 pltt_no);
void    dp_flush(void);
void    dp_write(char *str, LcdScreen scr, s32 h_pos, s32 v_pos);
void    dp_clear_scr(LcdScreen scr);
void    dp_clear(LcdScreen scr, s32 h_pos, s32 v_pos, u32 num);
void    dp_write_byte(u8 num, LcdScreen scr, s32 h_pos, s32 v_pos);
void    dp_write_dec(s32 num, LcdScreen scr, s32 h_pos, s32 v_pos);
void    dp_write_hex(s32 num, LcdScreen scr, s32 h_pos, s32 v_pos);
void    dp_write_byte_arr(u8 *arr, u32 length, LcdScreen scr, s32 h_pos, s32 v_pos);
static void BgScSet(char *Srcp, u16 *Destp, u8 PlttNo);
//--- End of Auto Function Prototype

/*-----------------------------------------------------------------------*
                    Variable
 *-----------------------------------------------------------------------*/

static s32 pltt;
static u16 scr_buf_main[SCREEN_SIZE];
static u16 scr_buf_sub[SCREEN_SIZE];

/*-----------------------------------------------------------------------*
                    Global Function Definition
 *-----------------------------------------------------------------------*/

void dp_init(void)
{

    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    GX_SetBankForBG(GX_VRAM_BG_128_A);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);

    GXS_SetGraphicsMode(GX_BGMODE_0);

    G2_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256,
                     GX_BG_COLORMODE_16,
                     GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01);

    G2S_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256,
                      GX_BG_COLORMODE_16,
                      GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01);

    G2_SetBG1Priority(0);
    G2S_SetBG1Priority(0);

    G2_BG1Mosaic(FALSE);
    G2S_BG1Mosaic(FALSE);

    GX_LoadBG1Char(CharData_Sample, 0, sizeof(CharData_Sample));
    GX_LoadBGPltt(PlttData_Sample, 0, sizeof(PlttData_Sample));

    GXS_LoadBG1Char(CharData_Sample, 0, sizeof(CharData_Sample));
    GXS_LoadBGPltt(PlttData_Sample, 0, sizeof(PlttData_Sample));

    MI_CpuClearFast(scr_buf_main, sizeof(scr_buf_main));
    MI_CpuClearFast(scr_buf_sub, sizeof(scr_buf_sub));

    dp_flush();

    GX_SetVisiblePlane(GX_PLANEMASK_BG1);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG1);
}

void dp_set_pltt(s32 pltt_no)
{
    pltt = pltt_no;
}

void dp_flush(void)
{

    DC_FlushRange(scr_buf_main, sizeof(scr_buf_main));
    DC_FlushRange(scr_buf_sub, sizeof(scr_buf_sub));

    GX_LoadBG1Scr(scr_buf_main, 0, sizeof(scr_buf_main));
    GXS_LoadBG1Scr(scr_buf_sub, 0, sizeof(scr_buf_sub));
}

void dp_write(char *str, LcdScreen scr, s32 h_pos, s32 v_pos)
{

    switch (scr)
    {
    case LCD_BOTTOM:
        BgScSet(str, &scr_buf_main[32 * v_pos + h_pos], 0);
        break;
    case LCD_TOP:
        BgScSet(str, &scr_buf_sub[32 * v_pos + h_pos], 0);
        break;
    }
}

void dp_clear_scr(LcdScreen scr)
{

    switch (scr)
    {
    case LCD_BOTTOM:
        MI_CpuClearFast(scr_buf_main, sizeof(scr_buf_main));
        break;
    case LCD_TOP:
        MI_CpuClearFast(scr_buf_sub, sizeof(scr_buf_sub));
        break;
    }
}

void dp_clear(LcdScreen scr, s32 h_pos, s32 v_pos, u32 num)
{

    switch (scr)
    {
    case LCD_BOTTOM:
        MI_CpuClear8(&scr_buf_main[32 * v_pos + h_pos], num * 2);
        break;
    case LCD_TOP:
        MI_CpuClear8(&scr_buf_sub[32 * v_pos + h_pos], num * 2);
        break;
    }
}

void dp_write_byte(u8 num, LcdScreen scr, s32 h_pos, s32 v_pos)
{

    char    str[3];

    str[2] = 0;
    str[1] = "0123456789ABCDEF"[num & 0xf];
    str[0] = "0123456789ABCDEF"[(num >> 4) & 0xf];

    switch (scr)
    {
    case LCD_BOTTOM:
        BgScSet(str, &scr_buf_main[32 * v_pos + h_pos], 0);
        break;
    case LCD_TOP:
        BgScSet(str, &scr_buf_sub[32 * v_pos + h_pos], 0);
        break;
    }
}

void dp_write_dec(s32 num, LcdScreen scr, s32 h_pos, s32 v_pos)
{

    char    buf[64];

    (void)OS_SNPrintf(buf, 64, "%d", num);

    dp_write(buf, scr, h_pos, v_pos);
}

void dp_write_hex(s32 num, LcdScreen scr, s32 h_pos, s32 v_pos)
{

    char    buf[64];

    (void)OS_SNPrintf(buf, 64, "%x", num);

    dp_write(buf, scr, h_pos, v_pos);
}

void dp_write_byte_arr(u8 *arr, u32 length, LcdScreen scr, s32 h_pos, s32 v_pos)
{

    s32     i;
    s32     pltt_temp = pltt;

    for (i = 0; i < length; i++)
    {
        pltt = i & 1;
        dp_write_byte(arr[i], scr, h_pos + i * 2, v_pos);
    }
    pltt = pltt_temp;
}

static void BgScSet(char *Srcp, u16 *Destp, u8 PlttNo)
{
    PlttNo = (u8)pltt;

    while (*Srcp != '\0')
        *Destp++ = (u16)(PlttNo << 12 | *Srcp++);
}

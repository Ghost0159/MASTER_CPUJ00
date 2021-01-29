/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CTRDG - demos - pulse_vib
  File:     char_data.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: char_data.h,v $
  Revision 1.1  2006/03/02 23:40:20  okubata_ryoma
  char_data.hÇÃí«â¡


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
 /*! @file
    @brief      Debug print

    @author     kitayama

    @version 1.00       initial release.
  */

#ifndef _CHAR_DATA_H
#define _CHAR_DATA_H

#include <nitro.h>

#define SCREEN_SIZE	(32 * 24)

typedef enum
{
    LCD_BOTTOM,
    LCD_TOP
}
LcdScreen;

/*-------------------------- ÉfÅ[É^  -------------------------------*/

//--- Auto Function Prototype --- Don't comment here.
extern void dp_init(void);
extern void dp_set_pltt(s32 pltt_no);
extern void dp_flush(void);
extern void dp_write(char *str, LcdScreen scr, s32 h_pos, s32 v_pos);
extern void dp_clear_scr(LcdScreen scr);
extern void dp_clear(LcdScreen scr, s32 h_pos, s32 v_pos, u32 num);
extern void dp_write_byte(u8 num, LcdScreen scr, s32 h_pos, s32 v_pos);
extern void dp_write_dec(s32 num, LcdScreen scr, s32 h_pos, s32 v_pos);
extern void dp_write_hex(s32 num, LcdScreen scr, s32 h_pos, s32 v_pos);
extern void dp_write_byte_arr(u8 *arr, u32 length, LcdScreen scr, s32 h_pos, s32 v_pos);

extern const u16 PlttData_Sample[16][16];
extern const u32 CharData_Sample[8 * 0x100];
//--- End of Auto Function Prototype

#endif /* CHAR_DATA_H */

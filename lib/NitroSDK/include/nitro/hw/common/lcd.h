/*---------------------------------------------------------------------------*
  Project:  NitroSDK - HW - include
  File:     lcd.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: lcd.h,v $
  Revision 1.4  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.3  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.2  2004/11/01 09:14:15  seiki_masashi
  �֐��}�N���̐��l�͈͈��ɑΏ�

  Revision 1.1  2004/10/20 06:17:17  terui
  Initial upload.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef NITRO_HW_COMMON_LCD_H_
#define NITRO_HW_COMMON_LCD_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
// �����h�b�g��
#define HW_LCD_WIDTH                256 // �\����ʕ��h�b�g��
#define HW_LCD_HBLANK               99 // H�u�����N���ԃh�b�g��
#define HW_LCD_COLUMNS              ( HW_LCD_WIDTH + HW_LCD_HBLANK )

// �������C����
#define HW_LCD_HEIGHT               192 // �\����ʍ����C����
#define HW_LCD_VBLANK               71 // V�u�����N���ԃ��C����
#define HW_LCD_LINES                ( HW_LCD_HEIGHT + HW_LCD_VBLANK )

// LCD�N���b�N
#define HW_LCD_CLOCK                33513982    // 33.513982MHz
#define HW_LCD_CLOCK_PER_DOT        6  // LCD�N���b�N 6����

// H�T�C�N��( 63.5556us )
#define HW_LCD_H_CYCLE_NS           ((u32)( 1000000000ULL * HW_LCD_COLUMNS * HW_LCD_CLOCK_PER_DOT / HW_LCD_CLOCK ))
#define HW_LCD_H_CYCLE_US           ((u32)(    1000000ULL * HW_LCD_COLUMNS * HW_LCD_CLOCK_PER_DOT / HW_LCD_CLOCK ))
#define HW_LCD_H_CYCLE_MS           ((u32)(       1000ULL * HW_LCD_COLUMNS * HW_LCD_CLOCK_PER_DOT / HW_LCD_CLOCK )

// V�T�C�N��( 16.7151ms )
#define HW_LCD_V_CYCLE_NS           ((u32)( 1000000000ULL * HW_LCD_LINES * HW_LCD_COLUMNS * HW_LCD_CLOCK_PER_DOT / HW_LCD_CLOCK ))
#define HW_LCD_V_CYCLE_US           ((u32)(    1000000ULL * HW_LCD_LINES * HW_LCD_COLUMNS * HW_LCD_CLOCK_PER_DOT / HW_LCD_CLOCK ))
#define HW_LCD_V_CYCLE_MS           ((u32)(       1000ULL * HW_LCD_LINES * HW_LCD_COLUMNS * HW_LCD_CLOCK_PER_DOT / HW_LCD_CLOCK ))

// n���C�������ɗv���鎞��
#define HW_LCD_LINES_CYCLE_NS(n)    ((u32)( 1000000000ULL * (n) * HW_LCD_COLUMNS * HW_LCD_CLOCK_PER_DOT / HW_LCD_CLOCK ))
#define HW_LCD_LINES_CYCLE_US(n)    ((u32)(    1000000ULL * (n) * HW_LCD_COLUMNS * HW_LCD_CLOCK_PER_DOT / HW_LCD_CLOCK ))
#define HW_LCD_LINES_CYCLE_MS(n)    ((u32)(       1000ULL * (n) * HW_LCD_COLUMNS * HW_LCD_CLOCK_PER_DOT / HW_LCD_CLOCK ))


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* NITRO_HW_COMMON_LCD_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

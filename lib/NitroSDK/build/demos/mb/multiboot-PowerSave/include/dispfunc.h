/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-PowerSave
  File:     dispfunc.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dispfunc.h,v $
  Revision 1.2  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/28 07:15:17  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#if !defined(NITROSDK_DEMOS_MB_MULTIBOOT_POWERSAVE_DISPFUNC_H_)
#define NITROSDK_DEMOS_MB_MULTIBOOT_POWERSAVE_DISPFUNC_H_


#include <nitro.h>


/*****************************************************************************/
/* constant */

/* �e�L�X�g�p�p���b�g�J���[ */
enum
{
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    PURPLE,
    LIGHT_GREEN,
    HI_YELLOW,
    HI_CYAN,
    HI_WHITE,
    HI_BLUE,
    HI_PURPLE,
    HI_LIGHT_GREEN,
    BROWN,
    HI_BROWN,
    COLOR_NUM
};


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         BgInit

  Description:  BG �𕶎���\���p�ɏ�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    BgInit(void);

/*---------------------------------------------------------------------------*
  Name:         BgClear

  Description:  BG ���C����ʃX�N���[����S�ăL�����N�^ 0 �ŏ�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    BgClear(void);

/*---------------------------------------------------------------------------*
  Name:         BgUpdate

  Description:  BG �X�N���[�������������ɔ��f.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    BgUpdate(void);

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
void    BgPutString(int x, int y, int pal, const char *str, int len);

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
void    BgPrintf(int x, int y, int pal, const char *str, ...);

/*---------------------------------------------------------------------------*
  Name:         BgPutChar

  Description:  BG �̎w��O���b�h�ɕ�����\��.

  Arguments:    x                �\�����鍶�� x �O���b�h. (8pixel�P��)
                y                �\�����鍶�� y �O���b�h. (8pixel�P��)
                pal              �p���b�g�ԍ�. (0-15)
                c                �\������ BG �L�����N�^�ԍ�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static  inline void BgPutChar(int x, int y, int pal, int c)
{
    const char tmp = (char)c;
    BgPutString(x, y, pal, &tmp, 1);
}

/*---------------------------------------------------------------------------*
  Name:         BgSetMessage

  Description:  ���C��/�T�u��ʗ����� (4, 22) �̈ʒu�ɕ�����\��.

  Arguments:    pal              �p���b�g�ԍ�. (0-15)
                str              �\������ ASCII ������.
                                 �Ή������� OS_VSNPrintf() �ɏ�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    BgSetMessage(int pal, const char *str, ...);


#endif /* NITROSDK_DEMOS_MB_MULTIBOOT_POWERSAVE_DISPFUNC_H_ */

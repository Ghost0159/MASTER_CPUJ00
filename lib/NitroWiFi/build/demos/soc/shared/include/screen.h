/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - shared
  File:     screen.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: screen.h,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/06 00:23:35  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if !defined(NITROWIFI_SOC_SHARED_SCREEN_H_)
#define NITROWIFI_SOC_SHARED_SCREEN_H_

#include <nitro.h>

#ifdef __cplusplus

extern "C"
{
#endif

/*****************************************************************************/

/* constant */
extern const u32    d_CharData[8 * 256];
extern const u32    d_PaletteData[8 * 16];

#define COLOR_WHITE             0
#define COLOR_RED               1
#define COLOR_GREEN             2
#define COLOR_BLUE              3
#define COLOR_YELLOW            4
#define COLOR_PURPLE            5
#define COLOR_LIGHT_BLUE        6
#define COLOR_DARK_RED          7
#define COLOR_DARK_GREEN        8
#define COLOR_DARK_BLUE         9
#define COLOR_DARK_YELLOW       10
#define COLOR_DARK_PURPLE       11
#define COLOR_DARK_LIGHT_BLUE   12
#define COLOR_GRAY              13
#define COLOR_DARK_GRAY         14
#define COLOR_DARK_BLACK        15

/*****************************************************************************/

/* function */

/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  �\�������̏�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    InitScreen(void);

/*---------------------------------------------------------------------------*
  Name:         ClearScreen

  Description:  �e�L�X�g�X�N���[���̏���.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ClearScreen(void);

/*---------------------------------------------------------------------------*
  Name:         PadkeyClearScreen

  Description:  Padkey���g�p���镔���̃e�L�X�g�X�N���[���̏���.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PadkeyClearScreen(void);

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  �e�L�X�g�X�N���[���ɕ������\��.

  Arguments:    x             �O���b�h�P�ʂ� X ���W
                y             �O���b�h�P�ʂ� Y ���W
                palette       �p���b�g�C���f�b�N�X
                format        ����������

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PrintString(int x, int y, int palette, const char* format, ...);

/*---------------------------------------------------------------------------*
  Name:         PrintDebugString

  Description:  �T�u��ʃe�L�X�g�X�N���[���Ƀf�o�b�O�������\��.

  Arguments:    s             �o�͕�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PrintDebugString(const char* s, ...);

/*---------------------------------------------------------------------------*
  Name:         FlushScreen

  Description:  �e�L�X�g�X�N���[�������̏��VRAM�֔��f.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FlushScreen(void);

/*---------------------------------------------------------------------------*
  Name:         GetFlickerColor

  Description:  �_�ŗp�̕�����F���擾����B

  Arguments:    None.

  Returns:      int     -   �p���b�g�ԍ�
 *---------------------------------------------------------------------------*/
int     GetFlickerColor(void);

/*---------------------------------------------------------------------------*
  Name:         ShowNavigater

  Description:  �e�L�X�g�X�N���[���̍ŉ��s�ɁA�I�y���[�^��U�����郁�b�Z�[�W
                ��\������B���t���[���Ăяo�����Ƃɂ��A�_�ŕ\�������B

  Arguments:    text    -   �\�����郁�b�Z�[�W�e�L�X�g�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ShowNavigater(const char* text);

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
int     PrintStep(int x, int y, int palette, int step, const char* comment);

#ifdef __cplusplus

} /* extern "C" */
#endif

#endif /* NITROWIFI_SOC_SHARED_SCREEN_H_ */

/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - shared
  File:     padkeyboard.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: padkeyboard.h,v $
  Revision 1.3  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2006/01/10 04:21:17  kitase_hirotake
  PadKeySetString, PadKeyGetInputString �̒ǉ�

  Revision 1.1  2006/01/06 00:23:35  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if !defined(NITROWIFI_SOC_SHARED_PAD_KEYBOARD_H_)
#define NITROWIFI_SOC_SHARED_PAD_KEYBOARD_H_

#include <nitro.h>

#include <stdlib.h>

#include "output.h"
#include "screen.h"

#ifdef __cplusplus

extern "C"
{
#endif

/*****************************************************************************/

/* constant */
#define INPUT_MAX   256

/* �L�[�{�[�h�Ŏg�p���Ă���萔 */
#define KEY_UP              (-1)
#define KEY_DOWN            (-2)
#define KEY_LEFT            (-3)
#define KEY_RIGHT           (-4)
#define KEY_BACKSPACE       (-5)
#define KEY_SPACE           (-6)
#define KEY_RETURN          (-7)
#define KEY_DONT_GET        (-128)

#define DIRECT_MODE         1
#define INDIRECT_MODE       2

#define ARABIC_NUMERALS     1
#define CAPITAL_LETTER      2
#define SMALL_LETTER        3
#define CHARACTOR_SYMBOL    4
#define MAX_CHARACTOR_MODE  4

/* PadKeyGet* �œ���ꂽ�f�[�^�� Indirect Mode �ł��邩�̃`�F�b�N�p�}�N�� */
#define IndirectCheck(charactor)    (charactor > KEY_UP ? 1 : 0)

/*---------------------------------------------------------------------------*
  Name:         KeyboardValid, KeyboardInvalid

  Description:  �L�[�{�[�h���̗͂L���A������؂�ւ���.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void    KeyboardValid();
void    KeyboardInvalid();

/*---------------------------------------------------------------------------*
  Name:         PadKeySetString

  Description:  �w�蕶��������݂̓��͂ɃZ�b�g����.

  Arguments:    string - �Z�b�g���镶����.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void    PadKeySetString(char string[]);

/*---------------------------------------------------------------------------*
  Name:         PadKeyGetInputString

  Description:  ���������͂����āA������󂯎��.

  Arguments:    string - �Z�b�g���镶����.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void    PadKeyGetInputString(char string[], int size);

/*---------------------------------------------------------------------------*
  Name:         PadKeyGetcharAsync

  Description:  �p�b�h���̓f�[�^���ꕶ������(�񓯊�).

  Arguments:    None.

  Returns:      �����f�[�^.
 *---------------------------------------------------------------------------*/
char    PadKeyGetcharAsync(void);

/*---------------------------------------------------------------------------*
  Name:         PadKeyGetchar

  Description:  �p�b�h���̓f�[�^���ꕶ������(����).

  Arguments:    None.

  Returns:      �����f�[�^.
 *---------------------------------------------------------------------------*/
char    PadKeyGetchar(void);

/*---------------------------------------------------------------------------*
  Name:         WakeupKeyboardThread

  Description:  �L�[�{�[�h�X���b�h���N����.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void    WakeupKeyboardThread(void);

/*---------------------------------------------------------------------------*
  Name:         InitKeyboardThread

  Description:  �L�[�{�[�h�X���b�h���쐬.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void    InitKeyboardThread(void);

#ifdef __cplusplus

} /* extern "C" */
#endif

#endif /* NITROWIFI_SOC_SHARED_PAD_KEYBOARD_H_ */

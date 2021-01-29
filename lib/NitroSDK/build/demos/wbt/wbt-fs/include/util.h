/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs
  File:     util.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: util.h,v $
  Revision 1.4  2006/01/18 02:12:38  kitase_hirotake
  do-indent

  Revision 1.3  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.2  2004/10/25 10:18:42  yosizaki
  add color definitions.

  Revision 1.1  2004/09/03 01:11:01  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#if	!defined(NITRO_DEMOS_WBT_WBT_FS_UTIL_H_)
#define	NITRO_DEMOS_WBT_WBT_FS_UTIL_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include	<nitro/types.h>

/*
 * �T���v���̃t���[�����[�N�S�ʂ��������郋�[�`���ł�.
 * �T���v�����񎦂���f�����X�g���[�V�����Ƃ͒��ڊ֌W����܂���.
 */


/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/

/* ASCII �t�H���g�f�[�^ */
extern const u32 d_CharData[8 * 256];
extern const u32 d_PaletteData[8 * 16];

/* �J���[�p���b�g */
#define	COLOR_BLACK                0x00
#define	COLOR_RED                  0x01
#define	COLOR_GREEN                0x02
#define	COLOR_BLUE                 0x03
#define	COLOR_YELLOW               0x04
#define	COLOR_PURPLE               0x05
#define	COLOR_LIGHT_BLUE           0x06
#define	COLOR_DARK_RED             0x07
#define	COLOR_DARK_GREEN           0x08
#define	COLOR_DARK_BLUE            0x09
#define	COLOR_DARK_YELLOW          0x0A
#define	COLOR_DARK_PURPLE          0x0B
#define	COLOR_DARK_LIGHT_BLUE      0x0C
#define	COLOR_GRAY                 0x0D
#define	COLOR_DARK_GRAY            0x0E
#define	COLOR_WHITE                0x0F


/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/

/* �L�[���͏�� */
typedef struct KeyInfo
{
    u16     cnt;                       /* �����H���͒l */
    u16     trg;                       /* �����g���K���� */
    u16     up;                        /* �����g���K���� */
    u16     rep;                       /* �����ێ����s�[�g���� */

}
KeyInfo;


/*---------------------------------------------------------------------------*
    �֐��錾
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         UTIL_Init

  Description:  �T���v���̃t���[�����[�N������.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    UTIL_Init(void);


/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  �L�[�f�[�^�̎擾.

  Arguments:    pKey : �L�[�����i�[����A�h���X

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    KeyRead(KeyInfo * pKey);


/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  �\���p���z�X�N���[���̏�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ClearString(void);


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
void    PrintString(int x, int y, int palette, const char *text, ...);


/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  ���z�X�N���[���ɕ\������Ă��镶����̐F��ύX.

  Arguments:    x       - �F�ύX���J�n���� x ���W / 8 dot
                y       - �F�ύX���J�n���� y ���W / 8 dot
                length  - �F�ύX���镶����
                palette - �����F���w�肷��p���b�g�ԍ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ColorString(int x, int y, int length, int palette);


/*---------------------------------------------------------------------------*
  Name:         ClearLine

  Description:  ������`���������.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ClearLine(void);


/*---------------------------------------------------------------------------*
  Name:         PrintLine

  Description:  �w�胉�C���� 1 �t���[���ȏ�c��s�����ȕ������`��.

  Arguments:    x          �\���ʒu x �O���b�h. (8 pixel �P��)
                y          �\���ʒu y �O���b�h. (8 pixel �P��)
                text       �㑱�̉ψ������󂯂鏑��������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PrintLine(int x, int y, const char *text, ...);


/*---------------------------------------------------------------------------*
  Name:         FlushLine

  Description:  �ʏ�̕�����`��� PrintLine() �̕s������������㏑��.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FlushLine(void);


/*===========================================================================*/

#ifdef	__cplusplus
}          /* extern "C" */
#endif

#endif /* NITRO_DEMOS_WBT_WBT_FS_UTIL_H_ */


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - include
  File:     ownerInfo.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ownerInfo.h,v $
  Revision 1.17  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.16  2006/10/25 04:46:45  kitase_hirotake
  OSOwnerInfo �̕ύX

  Revision 1.15  2006/10/17 05:27:43  okubata_ryoma
  �؍���IPL�ɑΉ�

  Revision 1.14  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.13  2005/05/14 03:03:13  terui
  SDK_NVRAM_FORMAT��300�ȏ�̏ꍇ�̂ݒ�����̌���R�[�h����`�����悤�C��

  Revision 1.12  2005/04/26 08:01:54  terui
  Fix comment

  Revision 1.11  2005/04/18 12:28:10  terui
  OSi_CheckOwnerInfo�֐����폜

  Revision 1.10  2005/04/12 10:49:15  terui
  ������Ή��ׂ̈ɉ���

  Revision 1.9  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.8  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.7  2004/10/07 10:19:39  yosizaki
  add favorite color API.

  Revision 1.6  2004/09/13 05:49:56  yasu
  merged with NitroSDK_2_00pr6_branch

  Revision 1.5.2.1  2004/09/13 05:43:02  yasu
  Support SDK_NVRAM_FORMAT

  Revision 1.5  2004/09/03 08:43:07  terui
  Only fix comment.

  Revision 1.4  2004/09/03 08:00:53  terui
  OS_GetOwnerRtcOffset�֐���ǉ��B

  Revision 1.3  2004/09/03 04:35:21  yasu
  Add "#include <nitro/spec.h>"

  Revision 1.2  2004/09/03 04:29:15  terui
  NVRAM�����[�U�[���Ɋւ���\���̒�`�����ɔ����C���B

  Revision 1.1  2004/09/02 06:20:49  terui
  Initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_OS_COMMON_OWNERINFO_H_
#define NITRO_OS_COMMON_OWNERINFO_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include    <nitro/types.h>
#include    <nitro/spec.h>
#include    <nitro/spi/common/config.h>
#include    <nitro/hw/common/mmap_shared.h>
#include    <nitro/mi/memory.h>

#include <nitro/gx/gxcommon.h>



/* �{�̏�� : �u���C�ɓ���̐F�v��` (0-15) */
#define	OS_FAVORITE_COLOR_GRAY         0x00
#define	OS_FAVORITE_COLOR_BROWN        0x01
#define	OS_FAVORITE_COLOR_RED          0x02
#define	OS_FAVORITE_COLOR_PINK         0x03
#define	OS_FAVORITE_COLOR_ORANGE       0x04
#define	OS_FAVORITE_COLOR_YELLOW       0x05
#define	OS_FAVORITE_COLOR_LIME_GREEN   0x06
#define	OS_FAVORITE_COLOR_GREEN        0x07
#define	OS_FAVORITE_COLOR_DARK_GREEN   0x08
#define	OS_FAVORITE_COLOR_SEA_GREEN    0x09
#define	OS_FAVORITE_COLOR_TURQUOISE    0x0A
#define	OS_FAVORITE_COLOR_BLUE         0x0B
#define	OS_FAVORITE_COLOR_DARK_BLUE    0x0C
#define	OS_FAVORITE_COLOR_PURPLE       0x0D
#define	OS_FAVORITE_COLOR_VIOLET       0x0E
#define	OS_FAVORITE_COLOR_MAGENTA      0x0F
#define	OS_FAVORITE_COLOR_BITMASK      0x0F
#define	OS_FAVORITE_COLOR_MAX          0x10

/* �{�̏�� : �u���C�ɓ���̐F�v�萔 (GXRgb) */
#define	OS_FAVORITE_COLOR_VALUE_GRAY         GX_RGB(12,16,19)
#define	OS_FAVORITE_COLOR_VALUE_BROWN        GX_RGB(23, 9, 0)
#define	OS_FAVORITE_COLOR_VALUE_RED          GX_RGB(31, 0, 3)
#define	OS_FAVORITE_COLOR_VALUE_PINK         GX_RGB(31,17,31)
#define	OS_FAVORITE_COLOR_VALUE_ORANGE       GX_RGB(31,18, 0)
#define	OS_FAVORITE_COLOR_VALUE_YELLOW       GX_RGB(30,28, 0)
#define	OS_FAVORITE_COLOR_VALUE_LIME_GREEN   GX_RGB(21,31, 0)
#define	OS_FAVORITE_COLOR_VALUE_GREEN        GX_RGB( 0,31, 0)
#define	OS_FAVORITE_COLOR_VALUE_DARK_GREEN   GX_RGB( 0,20, 7)
#define	OS_FAVORITE_COLOR_VALUE_SEA_GREEN    GX_RGB( 9,27,17)
#define	OS_FAVORITE_COLOR_VALUE_TURQUOISE    GX_RGB( 6,23,30)
#define	OS_FAVORITE_COLOR_VALUE_BLUE         GX_RGB( 0,11,30)
#define	OS_FAVORITE_COLOR_VALUE_DARK_BLUE    GX_RGB( 0, 0,18)
#define	OS_FAVORITE_COLOR_VALUE_PURPLE       GX_RGB(17, 0,26)
#define	OS_FAVORITE_COLOR_VALUE_VIOLET       GX_RGB(26, 0,29)
#define	OS_FAVORITE_COLOR_VALUE_MAGENTA      GX_RGB(31, 0,18)


#ifdef  SDK_TS
#if ( SDK_TS_VERSION >= 200 || SDK_NVRAM_FORMAT >= 100 )

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
// ����R�[�h
typedef enum OSLanguage
{
    OS_LANGUAGE_JAPANESE = NVRAM_CONFIG_LANG_JAPANESE,  // 0: ���{��
    OS_LANGUAGE_ENGLISH = NVRAM_CONFIG_LANG_ENGLISH,    // 1: �p��
    OS_LANGUAGE_FRENCH = NVRAM_CONFIG_LANG_FRENCH,      // 2: �t�����X��
    OS_LANGUAGE_GERMAN = NVRAM_CONFIG_LANG_GERMAN,      // 3: �h�C�c��
    OS_LANGUAGE_ITALIAN = NVRAM_CONFIG_LANG_ITALIAN,    // 4: �C�^���A��
    OS_LANGUAGE_SPANISH = NVRAM_CONFIG_LANG_SPANISH,    // 5: �X�y�C����
#if ( SDK_NVRAM_FORMAT >= 300 )
    OS_LANGUAGE_CHINESE = NVRAM_CONFIG_LANG_CHINESE,    // 6: ������
    OS_LANGUAGE_HANGUL = NVRAM_CONFIG_LANG_HANGUL,      // 7: �؍���
#endif
    OS_LANGUAGE_CODE_MAX = NVRAM_CONFIG_LANG_CODE_MAX
}
OSLanguage;


// ������ő咷
#define     OS_OWNERINFO_NICKNAME_MAX       NVRAM_CONFIG_NICKNAME_LENGTH
#define     OS_OWNERINFO_COMMENT_MAX        NVRAM_CONFIG_COMMENT_LENGTH


/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
// �a�������
typedef struct OSBirthday
{
    u8      month;                     // ��( 1 �` 12 )
    u8      day;                       // ��( 1 �` 31 )

}
OSBirthday;

// �I�[�i�[���
typedef struct OSOwnerInfo
{
    u8      language;                  // ����R�[�h
    u8      favoriteColor;             // ���C�ɓ���̐F ( 0 �` 15 )
    OSBirthday birthday;               // �a����
    u16     nickName[OS_OWNERINFO_NICKNAME_MAX + 1];
    // �j�b�N�l�[�� (Unicode(UTF16)�ōő�10�����A�I�[�R�[�h�Ȃ�)
    u16     nickNameLength;            // �j�b�N�l�[��������( 0 �` 10 )
    u16     comment[OS_OWNERINFO_COMMENT_MAX + 1];
    // �R�����g (Unicode(UTF16)�ōő�26�����A�I�[�R�[�h�Ȃ�)
    u16     commentLength;             // �R�����g������( 0 �` 26 )

}
OSOwnerInfo;


/*---------------------------------------------------------------------------*
    �֐���`
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         OS_GetOwnerInfo

  Description:  �I�[�i�[�����擾����B

  Arguments:    info    -   �I�[�i�[�����擾����o�b�t�@�ւ̃|�C���^�B
                            ���̃o�b�t�@�Ƀf�[�^���������܂��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    OS_GetOwnerInfo(OSOwnerInfo *info);

#endif

/*---------------------------------------------------------------------------*
  Name:         OS_GetOwnerRtcOffset

  Description:  �I�[�i�[��RTC�ݒ��ύX����x�ɂ��̒l�ɉ����đ�������I�t�Z�b�g
                �l(�P�ʂ͕b)���擾����B
                �Đݒ蒼�O��RTC�������疢���ɐi�߂�ꂽ�ꍇ�͐i�߂����Ԃ����Z
                �����B�ߋ��ɖ߂��ꂽ�ꍇ�͖߂������Ԃ����Z�����B

  Arguments:    None.

  Returns:      s64 -   RTC�ݒ�I�t�Z�b�g�l��Ԃ��B
 *---------------------------------------------------------------------------*/
s64     OS_GetOwnerRtcOffset(void);

#endif

/*---------------------------------------------------------------------------*
  Name:         OS_GetMacAddress

  Description:  �����n�[�h�E�F�A��MAC�A�h���X���擾����B

  Arguments:    macAddress  -   MAC�A�h���X���擾����o�b�t�@�ւ̃|�C���^�B
                                ���̃o�b�t�@��6�o�C�g�̃f�[�^���������܂��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    OS_GetMacAddress(u8 *macAddress);

/*---------------------------------------------------------------------------*
  Name:         OS_GetFavoriteColorTable

  Description:  OSOwnerInfo �\���̂̃����o favoriteColor �̔ԍ��ɑΉ�����
                �u���C�ɓ���̐F�v�̔z��ւ̃|�C���^���擾���܂��B

  Arguments:    None.

  Returns:      �Ή�����F�l���i�[���� GXRgb �^�̒萔�z��ւ̃|�C���^.
 *---------------------------------------------------------------------------*/
const GXRgb *OS_GetFavoriteColorTable(void);

/*---------------------------------------------------------------------------*
  Name:         OS_GetMacAddress

  Description:  �w��̔ԍ��ɑΉ�����u���C�ɓ���̐F�v���擾���܂��B

  Arguments:    index  -   �w�肷��u���C�ɓ���̐F�v�̔ԍ�.
                           ���̃C���f�b�N�X�́u���C�ɓ���̐F�v�����ł���
                           16 �F(4bit)�Ń}�X�N����܂��B

  Returns:      �w�肵���ԍ��ɑΉ����� GXRgb �^�̐F�l.
 *---------------------------------------------------------------------------*/
static inline GXRgb OS_GetFavoriteColor(int index)
{
    return OS_GetFavoriteColorTable()[index & OS_FAVORITE_COLOR_BITMASK];
}

/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* NITRO_OS_COMMON_OWNERINFO_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

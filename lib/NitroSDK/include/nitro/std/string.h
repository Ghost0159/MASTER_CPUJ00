/*---------------------------------------------------------------------------*
  Project:  NitroSDK - STD - include
  File:     string.h

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: string.h,v $
  Revision 1.14  2007/04/24 12:05:14  yosizaki
  update copyright.

  Revision 1.13  2007/04/20 09:45:52  yosizaki
  add STD_TSPrintf.

  Revision 1.12  2006/06/06 09:09:37  kitase_hirotake
  STD_CopyLStringZeroFill �֐��̕Ԃ�l�ύX

  Revision 1.10  2006/01/18 02:12:29  kitase_hirotake
  do-indent

  Revision 1.9  2005/10/21 08:44:16  yosizaki
  add STD_TSScanf().

  Revision 1.8  2005/09/27 05:19:39  yada
  just arrange src list

  Revision 1.7  2005/09/26 08:48:53  okubata_ryoma
  STD_CompareNString�ASTD_CompareLString�̒ǉ�

  Revision 1.6  2005/09/26 05:06:24  okubata_ryoma
  �C��

  Revision 1.5  2005/09/26 04:55:42  okubata_ryoma
  STD_CompareLString�֐��̒ǉ�

  Revision 1.4  2005/09/06 01:25:40  yasu
  STD_GetStringLength �̕Ԃ�l�̌^�� int ��

  Revision 1.3  2005/09/05 01:20:47  yada
  add some functions

  Revision 1.2  2005/08/24 02:51:22  yada
  add STD_SearchStringInString()

  Revision 1.1  2005/08/19 11:01:05  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_STD_STRING_H_
#define NITRO_STD_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>

//---- aliases
#define STD_StrCpy          STD_CopyString
#define STD_StrLCpy         STD_CopyLString
#define STD_StrStr          STD_SearchString
#define STD_StrLen          STD_GetStringLength
#define STD_StrCat          STD_ConcatenateString
#define STD_StrCmp          STD_CompareString
#define STD_StrNCmp         STD_CompareNString
#define STD_StrLCmp         STD_CompareLString

/*---------------------------------------------------------------------------*
  Name:         STD_CopyString

  Description:  same to strcpy

  Arguments:    destp : destination pointer
                srcp  : src pointer

  Returns:      pointer to destination
 *---------------------------------------------------------------------------*/
extern char *STD_CopyString(char *destp, const char *srcp);

/*---------------------------------------------------------------------------*
  Name:         STD_CopyLStringZeroFill

  Description:  do not correspond with strlcpy

  Arguments:    destp : destination pointer
                srcp  : src pointer
                n     : copy size + 1

  Returns:      size of src
 *---------------------------------------------------------------------------*/
extern int STD_CopyLStringZeroFill(char *destp, const char *srcp, int n);

/*---------------------------------------------------------------------------*
  Name:         STD_CopyLString

  Description:  same to strlcpy

  Arguments:    destp : destination pointer
                srcp  : src pointer
                siz   : copy size + 1

  Returns:      size of src
 *---------------------------------------------------------------------------*/
extern int STD_CopyLString(char *destp, const char *srcp, int siz);

/*---------------------------------------------------------------------------*
  Name:         STD_SearchString

  Description:  same to strstr

  Arguments:    srcp : src string
                str  : string to search from src pointer

  Returns:      pointer to destination
 *---------------------------------------------------------------------------*/
extern char *STD_SearchString(const char *srcp, const char *str);

/*---------------------------------------------------------------------------*
  Name:         STD_GetStringLength

  Description:  get string length. same to strlen

  Arguments:    str : string

  Returns:      string length
 *---------------------------------------------------------------------------*/
extern int STD_GetStringLength(const char *str);

/*---------------------------------------------------------------------------*
  Name:         STD_ConcatenateString

  Description:  concatenate strings. same to strcat

  Arguments:    str1 : original string
                str2 : string to concatenate

  Returns:      concatenated string
 *---------------------------------------------------------------------------*/
extern char *STD_ConcatenateString(char *str1, const char *str2);

/*---------------------------------------------------------------------------*
  Name:         STD_CompareString

  Description:  compare strings. same to strcmp

  Arguments:    str1, str2 : strings

  Returns:      0 if same
 *---------------------------------------------------------------------------*/
extern int STD_CompareString(const char *str1, const char *str2);

/*---------------------------------------------------------------------------*
  Name:         STD_CompareNString

  Description:  same to strncmp

  Arguments:    str1, str2 : strings
                len        : max length

  Returns:      0 if same
 *---------------------------------------------------------------------------*/
extern int STD_CompareNString(const char *str1, const char *str2, int len);

/*---------------------------------------------------------------------------*
  Name:         STD_CompareLString

  Description:  same to strlcmp

  Arguments:    str1, str2 : strings

  Returns:      0 if same
 *---------------------------------------------------------------------------*/
extern int STD_CompareLString(const char *str1, const char *str2);

/*---------------------------------------------------------------------------*
  Name:         STD_TSScanf

  Description:  �ȃT�C�Y��ړI�Ƃ��� sscanf.
                ��{�I�ȏ����w�� "%(*?)([lh]{,2})([diouxXpn])" �ɑΉ�.

  Arguments:    src           ���͕�����
                fmt           �������䕶����

  Returns:      ������ꂽ�l�̑���.
                ������ł̏I�[���邢�͕s�������o����� -1.
 *---------------------------------------------------------------------------*/
extern int STD_TSScanf(const char *src, const char *fmt, ...);

/*---------------------------------------------------------------------------*
  Name:         STD_TVSScanf

  Description:  STD_TSScanf �� va_list �Ή���.
                ��{�I�ȏ����w�� "%(*?)([lh]{,2})[diouxX]" �ɑΉ�.

  Arguments:    src           ���͕�����
                fmt           �������䕶����
                vlist         �p�����[�^

  Returns:      ������ꂽ�l�̑���.
                ������ł̏I�[���邢�͕s�������o����� -1.
 *---------------------------------------------------------------------------*/
extern int STD_TVSScanf(const char *src, const char *fmt, va_list vlist);

/*---------------------------------------------------------------------------*
  Name:         STD_TSPrintf

  Description:  �����̌`���ȊO�� STD_TVSNPrintf �Ɠ���.

  Arguments:    dst           ���ʂ��i�[����o�b�t�@
                fmt           �������䕶����

  Returns:      STD_VSNPrintf �Ɠ���.
 *---------------------------------------------------------------------------*/
extern int STD_TSPrintf(char *dst, const char *fmt, ...);

/*---------------------------------------------------------------------------*
  Name:         STD_TVSPrintf

  Description:  �����̌`���ȊO�� STD_TVSNPrintf �Ɠ���.

  Arguments:    dst           ���ʂ��i�[����o�b�t�@
                fmt           �������䕶����
                vlist         �p�����[�^

  Returns:      STD_VSNPrintf �Ɠ���.
 *---------------------------------------------------------------------------*/
extern int STD_TVSPrintf(char *dst, const char *fmt, va_list vlist);

/*---------------------------------------------------------------------------*
  Name:         STD_TSNPrintf

  Description:  �����̌`���ȊO�� STD_TVSNPrintf �Ɠ���.

  Arguments:    dst           ���ʂ��i�[����o�b�t�@
                len           �o�b�t�@��
                fmt           �������䕶����
 
  Returns:      STD_VSNPrintf �Ɠ���.
 *---------------------------------------------------------------------------*/
extern int STD_TSNPrintf(char *dst, size_t len, const char *fmt, ...);

/*---------------------------------------------------------------------------*
  Name:         STD_TVSNPrintf

  Description:  �ȃT�C�Y��ړI�Ƃ��� sprintf.
                ��{�I�ȏ����w��ɑΉ�.
                %([-+# ]?)([0-9]*)(\.?)([0-9]*)([l|ll|h||hh]?)([diouxXpncs%])

  Note:         CodeWarrior �� MSL sprintf() �̋����ɂ��킹��
                '+' �� '#' �͖����ɂ��Ă���.
                { // exsample
                  char buf[5];
                  sprintf(buf, "%-i\n", 45);  // "45"  (OK)
                  sprintf(buf, "%0i\n", 45);  // "45"  (OK)
                  sprintf(buf, "% i\n", 45);  // " 45" (OK)
                  sprintf(buf, "%+i\n", 45);  // "%+i" ("+45" expected)
                  sprintf(buf, "%#x\n", 45);  // "%#x" ("0x2d" expected)
                  // but, this works correctly!
                  sprintf(buf, "% +i\n", 45); // "+45" (OK)
                }

  Arguments:    dst           ���ʂ��i�[����o�b�t�@
                len           �o�b�t�@��
                fmt           �������䕶����
                vlist         �p�����[�^

  Returns:      ����������𐳂����o�͂����ꍇ�̕�������Ԃ�. ('\0'���܂܂Ȃ�)
                �o�b�t�@�T�C�Y���[���Ȃ�S�Ă̕������o�͂��ďI�[��t�^����.
                �o�b�t�@�T�C�Y���s���Ȃ�؂�l�߂� dst[len-1] ���I�[�Ƃ���.
                len �� 0 �̏ꍇ�͉������Ȃ�.

 *---------------------------------------------------------------------------*/
extern int STD_TVSNPrintf(char *dst, size_t len, const char *fmt, va_list vlist);


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_STD_STRING_H_ */
#endif

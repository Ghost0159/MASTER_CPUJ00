/*---------------------------------------------------------------------------*
  Project:  NitroSDK - libraries - STD
  File:     std_string.c

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: std_string.c,v $
  Revision 1.20  2007/02/20 00:28:09  kitase_hirotake
  indent source

  Revision 1.19  2006/11/16 11:19:19  yosizaki
  small change for unspecified charclass-range formats.

  Revision 1.18  2006/11/16 10:40:24  yosizaki
  support some extra format of character-class in Scanf

  Revision 1.17  2006/10/17 04:56:45  kitase_hirotake
  STD_CopyLString, STD_CopyLStringZeroFill �̕Ԃ�l�C��

  Revision 1.16  2006/09/08 09:46:07  yosizaki
  support character-class.

  Revision 1.15  2006/06/06 09:10:31  kitase_hirotake
  STD_CopyLStringZeroFill �֐��̕Ԃ�l�ύX

  Revision 1.13  2006/01/18 02:12:29  kitase_hirotake
  do-indent

  Revision 1.12  2005/10/25 09:34:59  yosizaki
  support 'c' and 's' in sscanf().

  Revision 1.11  2005/10/21 10:13:54  yosizaki
  fix bugs.

  Revision 1.10  2005/10/21 08:44:05  yosizaki
  add STD_TSScanf().

  Revision 1.9  2005/09/27 05:18:38  yada
  just arrange src list

  Revision 1.8  2005/09/26 08:49:35  okubata_ryoma
  STD_CompareNString�ASTD_CompareLString�̒ǉ�

  Revision 1.7  2005/09/26 05:06:40  okubata_ryoma
  �C��

  Revision 1.6  2005/09/26 04:54:50  okubata_ryoma
  STD_CompareLString�֐��̒ǉ�

  Revision 1.5  2005/09/06 01:25:23  yasu
  STD_GetStringLength �̕Ԃ�l�̌^�� int ��

  Revision 1.4  2005/09/05 01:20:11  yada
  add some functions

  Revision 1.3  2005/08/25 00:17:17  yada
  fix SDK_ASSERT

  Revision 1.2  2005/08/24 02:50:50  yada
  add STD_SearchStringInString()

  Revision 1.1  2005/08/19 11:04:28  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

/*---------------------------------------------------------------------------*
  Name:         STD_CopyString

  Description:  same to strcpy

  Arguments:    destp : destination pointer
                srcp  : src pointer

  Returns:      pointer to destination
 *---------------------------------------------------------------------------*/
char   *STD_CopyString(char *destp, const char *srcp)
{
    char   *retval = destp;

    SDK_ASSERT(destp && srcp);

    while (*srcp)
    {
        *destp++ = (char)*srcp++;
    }
    *destp = '\0';

    return retval;
}

/*---------------------------------------------------------------------------*
  Name:         STD_CopyLStringZeroFill

  Description:  do not correspond with strlcpy

  Arguments:    destp : destination pointer
                srcp  : src pointer
                n     : copy size + 1

  Returns:      size of src
 *---------------------------------------------------------------------------*/
int STD_CopyLStringZeroFill(char *destp, const char *srcp, int n)
{
    int     i;
    const char *s = srcp;

    SDK_ASSERT(destp && srcp && n >= 0);

    for (i = 0; i < n - 1; i++)
    {
        destp[i] = (char)*s;
        if (*s)
        {
            s++;
        }
    }
    destp[i] = '\0';

    return STD_GetStringLength(srcp);
}

/*---------------------------------------------------------------------------*
  Name:         STD_CopyLString

  Description:  same to strlcpy

  Arguments:    destp : destination pointer
                srcp  : src pointer
                siz   : copy size + 1

  Returns:      size of src
 *---------------------------------------------------------------------------*/
int STD_CopyLString(char *destp, const char *srcp, int siz)
{
    int     i;
    const char *s = srcp;

    SDK_ASSERT(destp && srcp && siz >= 0);

    for (i = 0; i < siz - 1; i++)
    {
        destp[i] = (char)*s;
        if (*s)
        {
            s++;
        }
        else
        {
            break;
        }
    }

    if ((i >= siz - 1) && (siz != 0))
    {
        destp[i] = '\0';
    }

    return STD_GetStringLength(srcp);
}

/*---------------------------------------------------------------------------*
  Name:         STD_SearchString

  Description:  same to strstr

  Arguments:    srcp : src string
                str  : string to search from src pointer

  Returns:      pointer to destination
 *---------------------------------------------------------------------------*/
char   *STD_SearchString(const char *srcp, const char *str)
{
    int     i;
    int     n;

    SDK_ASSERT(srcp && str);

    for (i = 0; srcp[i]; i++)
    {
        n = 0;
        while (str[n] && srcp[i + n] == str[n])
        {
            n++;
        }

        if (str[n] == '\0')
        {
            return (char *)&srcp[i];
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         STD_GetStringLength

  Description:  get string length. same to strlen

  Arguments:    str : string

  Returns:      string length
 *---------------------------------------------------------------------------*/
int STD_GetStringLength(const char *str)
{
    int     n = 0;
    while (str[n])
    {
        n++;
    }
    return n;
}

/*---------------------------------------------------------------------------*
  Name:         STD_ConcatenateString

  Description:  concatenate strings. same to strcat

  Arguments:    str1 : original string
                str2 : string to concatenate

  Returns:      concatenated string
 *---------------------------------------------------------------------------*/
char   *STD_ConcatenateString(char *str1, const char *str2)
{
    int     n = STD_GetStringLength(str1);
    (void)STD_CopyString(&str1[n], str2);
    return str1;
}

/*---------------------------------------------------------------------------*
  Name:         STD_CompareString

  Description:  compare strings. same to strcmp

  Arguments:    str1, str2 : strings

  Returns:      0 if same
 *---------------------------------------------------------------------------*/
int STD_CompareString(const char *str1, const char *str2)
{
    while (*str1 == *str2 && *str1)
    {
        str1++;
        str2++;
    }
    return (*str1 - *str2);
}

/*---------------------------------------------------------------------------*
  Name:         STD_CompareNString

  Description:  same to strncmp

  Arguments:    str1, str2 : strings
                len        : max length

  Returns:      0 if same
 *---------------------------------------------------------------------------*/
int STD_CompareNString(const char *str1, const char *str2, int len)
{
    if (len)
    {
        int     i;
        for (i = 0; i < len; ++i)
        {
            int     c = (int)(MI_ReadByte(str1 + i));
            int     d = (int)(MI_ReadByte(str2 + i));
            if (c != d)
            {
                return (int)(c - d);
            }
        }
    }
    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         STD_CompareLString

  Description:  same to strlcmp

  Arguments:    str1, str2 : strings

  Returns:      0 if same
 *---------------------------------------------------------------------------*/
int STD_CompareLString(const char *str1, const char *str2)
{
    return STD_CompareNString(str1, str2, STD_GetStringLength(str2));
}

/*---------------------------------------------------------------------------*
  Name:         STD_TSScanf

  Description:  �ȃT�C�Y��ړI�Ƃ��� sscanf.
                ��{�I�ȏ����w�� "%(*?)([lh]{,2})([diouxXpn])" �ɑΉ�.

  Arguments:    src           ���͕�����
                fmt           �������䕶����

  Returns:      ������ꂽ�l�̑���.
                ������ł̏I�[���邢�͕s�������o����� -1.
 *---------------------------------------------------------------------------*/
SDK_WEAK_SYMBOL int STD_TSScanf(const char *src, const char *fmt, ...)
{
    int     ret;
    va_list va;
    va_start(va, fmt);
    ret = STD_TVSScanf(src, fmt, va);
    va_end(va);
    return ret;
}

/*---------------------------------------------------------------------------*
  Name:         STDi_IsSpace

  Description:  �w�蕶�����󔒗ޕ���������.

  Arguments:    c             ���肷�镶��

  Returns:      �󔒗ޕ����ł���� TRUE.
 *---------------------------------------------------------------------------*/
static BOOL STDi_IsSpace(int c)
{
    return (c == '\f') || (c == '\n') || (c == '\r') || (c == '\t') || (c == '\v') || (c == ' ');
}

/*---------------------------------------------------------------------------*
  Name:         STDi_GetBitset

  Description:  �r�b�g�Z�b�g�̎w��ʒu�̓��e���擾.

  Arguments:    bitet         �r�b�g�Z�b�g�z��
                i             �����擾����ʒu

  Returns:      ON �Ȃ� TRUE, OFF �Ȃ� FALSE.
 *---------------------------------------------------------------------------*/
inline static u32 STDi_GetBitset(const u32 *bitset, u32 i)
{
    const u32 pos = (u32)(i >> 5UL);
    const u32 bit = (u32)(i & 31UL);
    return ((bitset[pos] >> bit) & 1UL);
}

/*---------------------------------------------------------------------------*
  Name:         STDi_SetBitset

  Description:  �r�b�g�Z�b�g�̎w��ʒu��ON�ɐݒ�.

  Arguments:    bitet         �r�b�g�Z�b�g�z��
                i             ON�ɂ���ʒu

  Returns:      None.
 *---------------------------------------------------------------------------*/
inline static void STDi_SetBitset(u32 *bitset, u32 i)
{
    const u32 pos = (i >> 5UL);
    const u32 bit = (i & 31UL);
    bitset[pos] |= (1UL << bit);
}

/*---------------------------------------------------------------------------*
  Name:         STDi_FillBitset

  Description:  �r�b�g�Z�b�g�̎w��͈� [a,b) ��ON�ɐݒ�.

  Arguments:    bitet         �r�b�g�Z�b�g�z��
                a             �J�n�ʒu
                b             �I���ʒu

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void STDi_FillBitset(u32 *bitset, u32 a, u32 b)
{
    while (a < b)
    {
        const u32 pos = (u32)(a >> 5UL);
        const u32 bit = (u32)(a & 31UL);
        u32     mask = (~0UL << bit);
        a = (pos + 1UL) * 32UL;
        if (a > b)
        {
            mask &= (u32)((1UL << (b & 31UL)) - 1UL);
        }
        bitset[pos] |= mask;
    }
}

/*---------------------------------------------------------------------------*
  Name:         STD_TVSScanf

  Description:  STD_TSScanf �� va_list �Ή���.
                ��{�I�ȏ����w��ɑΉ�.

  Arguments:    src           ���͕�����
                fmt           �������䕶����
                vlist         �p�����[�^

  Returns:      ������ꂽ�l�̑���.
                ������ł̏I�[���邢�͕s�������o����� -1.
 *---------------------------------------------------------------------------*/
SDK_WEAK_SYMBOL int STD_TVSScanf(const char *src, const char *fmt, va_list vlist)
{
    /* �ƍ�������̋N�_ (%n �̂��߂ɕK�v) */
    const char *src_base = src;
    /* �����Ɉ�x�ł��}�b�`�������ǂ���. (�Ԃ�l -1 �ɍ�p) */
    BOOL    matched = FALSE;
    /* ��������s���ꂽ��. (�Ԃ�l >= 0 �ɍ�p) */
    int     stored = 0;

    /* ����������� 1 ��������� */
    while (*fmt)
    {
        char    c = *fmt;
        /* �󔒕����Ȃ珑���E�ƍ��Ƃ��X�L�b�v */
        if (STDi_IsSpace(c))
        {
            while (STDi_IsSpace(*fmt))
            {
                ++fmt;
            }
            while (STDi_IsSpace(*src))
            {
                ++src;
            }
            continue;
        }
        /* �ʏ핶���Ȃ�Ό����Ɉ�v���Ȃ���΂Ȃ�Ȃ� */
        else if (c != '%')
        {
            if (c != *src)
            {
                break;
            }
            /* SHIFT-JIS�̏ꍇ�͌㑱�o�C�g������  */
            if ((unsigned int)(((unsigned char)c ^ 0x20) - 0xA1) < 0x3C)
            {
                if (c != *++src)
                {
                    break;
                }
            }
            ++src;
            ++fmt;
        }
        /* �G�X�P�[�v�ɂ�� '%' �̔���Ȃ�P�ɔ�r */
        else if (fmt[1] == '%')
        {
            if (c != *src)
            {
                break;
            }
            fmt += 2;
        }
        /* �ϊ��w��Ȃ�Ώ�����͊J�n */
        else
        {
            enum
            {
                flag_plus = 000002,    /* '+' */
                flag_minus = 000010,   /* '-' */
                flag_l1 = 000040,      /* "l" */
                flag_h1 = 000100,      /* "h" */
                flag_l2 = 000200,      /* "ll" */
                flag_h2 = 000400,      /* "hh" */
                flag_unsigned = 010000, /* 'o', 'u', ... */
                flag_ignored = 020000, /* '*' */
                flag_end
            };
            u64     val = 0;
            int     flag = 0, width = 0, radix = 10;
            int     digit;

            /* ����}�~�t���O */
            c = *++fmt;
            if (c == '*')
            {
                flag |= flag_ignored;
                c = *++fmt;
            }
            /* ���w�� */
            while ((c >= '0') && (c <= '9'))
            {
                width = (width * 10) + c - '0';
                c = *++fmt;
            }
            /* �ϊ��C�� */
            switch (c)
            {
            case 'h':
                c = *++fmt;
                if (c != 'h')
                {
                    flag |= flag_h1;
                }
                else
                    flag |= flag_h2, c = *++fmt;
                break;
            case 'l':
                c = *++fmt;
                if (c != 'l')
                    flag |= flag_l1;
                else
                    flag |= flag_l2, c = *++fmt;
                break;
            }
            /* �ϊ��^ */
            switch (c)
            {
            case 'd':                 /* �����t10�i�� */
                radix = 10;
                goto get_integer;
            case 'i':                 /* �����t8/10/16�i�� */
                radix = -1;
                goto get_integer;
            case 'o':                 /* �����t8�i�� */
                radix = 8;
                goto get_integer;
            case 'u':                 /* ������10�i�� */
                radix = 10;
                flag |= flag_unsigned;
                goto get_integer;
            case 'X':                 /* ������16�i�� */
            case 'x':                 /* ������16�i�� */
            case 'p':                 /* �|�C���^�ϊ�(������16�i��) */
                radix = 16;
                flag |= flag_unsigned;
                goto get_integer;
            case 's':                 /* ��󔒕����܂ł̕����� */
            case 'c':                 /* �w�蕝�݂̂̕����� */
                goto store_string;

            case '[':                 /* �����N���X */
                goto store_char_class;

            case 'n':                 /* �}�b�`�ʒu�̊i�[ */
                /* ����񐔂ɂ͊�^���Ȃ��̂ł����Ō����Ă��� */
                if (!(flag & flag_ignored))
                {
                    --stored;
                }
                val = src - src_base;
                c = *++fmt;
                goto store_integer;
            }

          invalid:
            /* ��͂��c�O�Ȍ��ʂɏI����� */
            break;

          get_integer:
            /* �����̓��͏��� */
            ++fmt;
            c = *src;
            /* �󔒂̃X�L�b�v */
            while (STDi_IsSpace(c))
            {
                c = *++src;
            }
            /* �����̎擾 */
            for (;; c = *++src)
            {
                if (c == '+')
                {
                    flag |= flag_plus;
                }
                else if (c == '-')
                {
                    /* �������^�ł� '-' �͕s�� */
                    if ((flag & flag_unsigned) != 0)
                    {
                        /*
                         * ...�Ƃ݂Ȃ��̂��������͂�����,
                         * gcc �� CW �ł͂���𖳎����Ă���̂�
                         * ����ł� SDK ������ɒǐ�.
                         */
                        //goto invalid;
                    }
                    flag |= flag_minus;
                }
                else
                {
                    break;
                }
            }
            /* �����̎������� (%i) */
            if (radix == -1)
            {
                if (c != '0')
                {
                    radix = 10;
                }
                else if ((src[1] == 'x') || (src[1] == 'X'))
                {
                    radix = 16;
                }
                else
                {
                    radix = 8;
                }
            }
            /* 16�i���Ȃ� "^(0[xX])?" ���X�L�b�v */
            if ((radix == 16) && (c == '0') && ((src[1] == 'x') || (src[1] == 'X')))
            {
                src += 2;
                c = *src;
            }
            /* �����擾 */
            if (width == 0)
            {
                width = 0x7FFFFFFF;
            }
            for (digit = 0; digit < width; ++digit)
            {
                u32     d = (u32)(c - '0');
                if (d >= 10)
                {
                    d -= (u32)('a' - '0');
                    if (d < 6)
                    {
                        d += 10;
                    }
                    else
                    {
                        d -= (u32)('A' - 'a');
                        if (d < 6)
                        {
                            d += 10;
                        }
                    }
                }
                if (d >= radix)
                {
                    break;
                }
                c = *++src;
                val = val * radix + d;
            }
            /* �����͕͂s�� */
            if (digit == 0)
            {
                break;
            }
            /* �������� */
            if (flag & flag_minus)
            {
                val = (u64)(val * -1);
            }
            /* ���Ȃ��Ƃ��ϊ��͐������� */
            matched = TRUE;
          store_integer:
            /* ��� */
            if (!(flag & flag_ignored))
            {
                ++stored;
                if (flag & flag_h2)
                {
                    *va_arg(vlist, u8 *) = (u8)val;
                }
                else if (flag & flag_h1)
                {
                    *va_arg(vlist, u16 *) = (u16)val;
                }
                else if (flag & flag_l2)
                {
                    *va_arg(vlist, u64 *) = (u64)val;
                }
                else
                {
                    *va_arg(vlist, u32 *) = (u32)val;
                }
            }
            continue;

          store_string:
            /* ������̑������ */
            {
                char   *dst = NULL;
                ++fmt;
                /* ���̎��_�ŏ��Ȃ��Ƃ��ϊ��͐������� */
                matched = TRUE;
                if (!(flag & flag_ignored))
                {
                    ++stored;
                    dst = va_arg(vlist, char *);
                }
                /*
                 * s �Ȃ�󔒂܂Ō������� '\0' ��ǉ�
                 * c �Ȃ�w�蕝�܂Ō���
                 */
                if (c == 's')
                {
                    if (width == 0)
                    {
                        width = 0x7FFFFFFF;
                    }
                    for (c = *src; STDi_IsSpace(c); c = *++src)
                    {
                    }
                    for (; c && !STDi_IsSpace(c) && (width > 0); --width, c = *++src)
                    {
                        if (dst)
                        {
                            *dst++ = c;
                        }
                    }
                    if (dst)
                    {
                        *dst++ = '\0';
                    }
                }
                else
                {
                    if (width == 0)
                    {
                        width = 1;
                    }
                    for (c = *src; c && (width > 0); --width, c = *++src)
                    {
                        if (dst)
                        {
                            *dst++ = c;
                        }
                    }
                    if (dst && (width > 0))
                    {
                        *dst++ = '\0';
                    }
                }
            }
            continue;

          store_char_class:
            ++fmt;
            /* �����N���X�̑������ */
            {
                char   *dst = NULL;

                u32     bitset[256 / (8 * sizeof(u32))];
                u32     matchcond = 1;
                u32     from = 0;
                BOOL    in_range = FALSE;
                MI_CpuFill32(bitset, 0x00000000UL, sizeof(bitset));
                if (*fmt == '^')
                {
                    matchcond = 0;
                    ++fmt;
                }
                /* �s���G�X�P�[�v */
                if (*fmt == ']')
                {
                    STDi_SetBitset(bitset, (u8)*fmt);
                    ++fmt;
                }
                /* �����N���X�̉�� */
                for (;; ++fmt)
                {
                    /* �I�[�̌��o */
                    if (!*fmt || (*fmt == ']'))
                    {
                        /* �͈͎w�蒆�̏I�[�͒P�̂̕����Ƃ��Ĉ��� */
                        if (in_range)
                        {
                            STDi_SetBitset(bitset, from);
                            STDi_SetBitset(bitset, (u32)'-');
                        }
                        if (*fmt == ']')
                        {
                            ++fmt;
                        }
                        break;
                    }
                    /* �P�������邢�͔͈͎w��̎n�[ */
                    else if (!in_range)
                    {
                        /* �͈͎w��̊J�n */
                        if ((from != 0) && (*fmt == '-'))
                        {
                            in_range = TRUE;
                        }
                        else
                        {
                            STDi_SetBitset(bitset, (u8)*fmt);
                            from = (u8)*fmt;
                        }
                    }
                    /* �͈͎w��̏I�[ */
                    else
                    {
                        u32     to = (u8)*fmt;
                        /* �s���Ȕ͈͎w��͕ʌ̕����w��Ƃ��Ĉ��� */
                        if (from > to)
                        {
                            STDi_SetBitset(bitset, from);
                            STDi_SetBitset(bitset, (u32)'-');
                            STDi_SetBitset(bitset, to);
                        }
                        /* �I�[�̕������͈͂Ɋ܂�ł܂Ƃ߂Đݒ� */
                        else
                        {
                            STDi_FillBitset(bitset, from, to + 1UL);
                        }
                        in_range = FALSE;
                        from = 0;
                    }
                }
                /* �����N���X�ƕ�����̏ƍ� */
                /* ���̎��_�ŏ��Ȃ��Ƃ��ϊ��͐������� */
                matched = TRUE;
                if (!(flag & flag_ignored))
                {
                    ++stored;
                    dst = va_arg(vlist, char *);
                }
                if (width == 0)
                {
                    width = 0x7FFFFFFF;
                }
                for (c = *src; c && (width > 0); --width, c = *++src)
                {
                    if (STDi_GetBitset(bitset, (u8)c) != matchcond)
                    {
                        break;
                    }
                    if (dst)
                    {
                        *dst++ = c;
                    }
                }
                if (dst)
                {
                    *dst++ = '\0';
                }
            }
            continue;

        }
    }

    return (*src || matched) ? stored : -1;

}

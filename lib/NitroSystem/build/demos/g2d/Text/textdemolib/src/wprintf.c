/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib
  File:     wprintf.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/

#include "g2d_textdemolib.h"

/* inner function for sized-buffer output */
typedef struct dst_string_tag {
    size_t len;
    wchar_t *cur;
    wchar_t *base;
} dst_string;

static void string_put_char(dst_string *p, wchar_t c)
{
    if(p->len > 0)  *p->cur = c, --p->len;
    ++p->cur;
}

static void string_fill_char(dst_string *p, wchar_t c, int n)
{
    if(n > 0) {
        size_t  i, k = p->len;
        if(k > (size_t)n)   k = (size_t)n;
        for( i = 0 ; i < k ; ++i )
            p->cur[i] = c;
        p->len -= k;
        p->cur += n;
    }
}

static void string_put_string(dst_string *p, const wchar_t *s, int n)
{
    if(n > 0) {
        size_t  i, k = p->len;
        if(k > (size_t)n)   k = (size_t)n;
        for( i = 0 ; i < k ; ++i )
            p->cur[i] = s[i];
        p->len -= k;
        p->cur += n;
    }
}

// based on OS_VSNPrintf

/*---------------------------------------------------------------------------*
  Name:         OS_VSNPrintf

  Description:  small-size vsnprintf which is similar to 'vsnprintf'
                without following supports.
                * CodeWarrior Extensions (#s)
                * MSL AltiVec Extensions (v, vh, vl, hv, lv, @)
                * indexed argments (%m$, *m$)
                * floating-point
                * wchar_t

  Note:         '+' and '#' do not work, MSL's sprintf().
                to keep same result, they are no implement.
                { // exsample
                  wchar_t buf[5];
                  sprintf(buf, "%-i\n", 45);  // "45"  (OK)
                  sprintf(buf, "%0i\n", 45);  // "45"  (OK)
                  sprintf(buf, "% i\n", 45);  // " 45" (OK)
                  sprintf(buf, "%+i\n", 45);  // "%+i" ("+45" expected)
                  sprintf(buf, "%#x\n", 45);  // "%#x" ("0x2d" expected)
                  // but, this works correctly!
                  sprintf(buf, "% +i\n", 45); // "+45" (OK)
                }

  Arguments:    dst   : destination buffer.
                len   : destination buffer size.
                fmt   : format string.
                vlist : parameters.

  Returns:      length of the generated string. (except '\0')
                if(result < len),
                  put NUL in dst[result].
                else if(len > 0),
                  put NUL in dst[len - 1].
                else,
                  do nothing.
 *---------------------------------------------------------------------------*/
int NNSi_G2dVSNWPrintf(wchar_t *dst, size_t len, const wchar_t *fmt, va_list vlist)
{
    wchar_t    buf[24];
    int n_buf;
    wchar_t    prefix[2];
    int n_prefix;

    const wchar_t  *s = fmt;

    dst_string  str;
    str.len = len, str.cur = str.base = dst;

    while(*s) {
        if(*s != L'%') {
            /* normal character */
            string_put_char(&str, *s++);
        } else {
            /* output with format */
            enum {
                flag_blank    = 000001, /* L' ' */
                flag_plus     = 000002, /* L'+' */
                flag_sharp    = 000004, /* L'#' */
                flag_minus    = 000010, /* L'-' */
                flag_zero     = 000020, /* L'0' */
                flag_l1       = 000040, /* "l" */
                flag_h1       = 000100, /* "h" */
                flag_l2       = 000200, /* "ll" */
                flag_h2       = 000400, /* "hh" */
                flag_unsigned = 010000, /* L'o', L'u', ... */
                flag_end } ;
            int flag = 0, width = 0, precision = -1, radix = 10;
            wchar_t    hex_char = L'a' - 10;
            const wchar_t  *p_start = s;
            /* flags */
            for( ; ; ) {
                switch(*++s) {
                case L'+':if(s[-1] != L' ') break;
                         flag |= flag_plus; continue;
                case L' ':flag |= flag_blank;continue;
                case L'-':flag |= flag_minus;continue;
                case L'0':flag |= flag_zero; continue;
                }
                break;
            }
            /* width */
            if(*s == L'*') {
                ++s, width = va_arg(vlist, int);
                if(width < 0)
                    width = -width, flag |= flag_minus;
            } else {
                while((*s >= L'0') && (*s <= L'9'))
                    width = (width * 10) + *s++ - L'0';
            }
            /* precision */
            if(*s == L'.') {
                ++s, precision = 0;
                if(*s == L'*') {
                    ++s, precision = va_arg(vlist, int);
                    if(precision < 0)   precision = -1;
                } else {
                    while((*s >= L'0') && (*s <= L'9'))
                        precision = (precision * 10) + *s++ - L'0';
                }
            }
            /* option */
            switch(*s) {
            case L'h':
                if(*++s != L'h') flag |= flag_h1;
                else    ++s, flag |= flag_h2;
                break;
            case L'l':
                if(*++s != L'l') flag |= flag_l1;
                else    ++s, flag |= flag_l2;
                break;
            }

            /* type */
            switch(*s) {
            case L'd':   /* signed decimal */
            case L'i':   /* signed decimal */
                goto    put_integer;
            case L'o':   /* unsigned octal */
                radix = 8;
                flag |= flag_unsigned;
                goto    put_integer;
            case L'u':   /* unsigned decimal */
                flag |= flag_unsigned;
                goto    put_integer;
            case L'X':   /* unsigned hexadecimal */
                hex_char = L'A' - 10;
                goto    put_hexadecimal;
            case L'x':   /* unsigned hexadecimal */
                goto    put_hexadecimal;
            case L'p':   /* pointer */
                /* equal to code warrior */
                flag |= flag_sharp;
                precision = 8;
                goto    put_hexadecimal;

            case L'c':   /* character */
                if(precision >= 0)
                    goto    put_invalid;
                {
                    int c = va_arg(vlist, int);
                    width -= 1;
                    if(flag & flag_minus) {
                        string_put_char(&str, (wchar_t)c);
                        string_fill_char(&str, L' ', width);
                    } else {
                        wchar_t    pad = (wchar_t)(
                            (flag & flag_zero) ? L'0' : L' ');
                        string_fill_char(&str, pad, width);
                        string_put_char(&str, (wchar_t)c);
                    }
                    ++s;
                }
                break;

            case L's':   /* string */
                {
                    int n_buf = 0;
                    const wchar_t  *p_buf = va_arg(vlist, const wchar_t*);
                    if(precision < 0) {
                        while(p_buf[n_buf])
                            ++n_buf;
                    } else {
                        while((n_buf < precision) && p_buf[n_buf])
                            ++n_buf;
                    }
                    width -= n_buf;
                    if(flag & flag_minus) {
                        string_put_string(&str, p_buf, n_buf);
                        string_fill_char(&str, L' ', width);
                    } else {
                        wchar_t    pad = (wchar_t)(
                            (flag & flag_zero) ? L'0' : L' ');
                        string_fill_char(&str, pad, width);
                        string_put_string(&str, p_buf, n_buf);
                    }
                    ++s;
                }
                break;

            case L'n':   /* store the number of output */
                {
                    int pos = str.cur - str.base;
                    if(flag & flag_h2)
                        ;
                    else if(flag & flag_h1)
                        *va_arg(vlist, signed short*) = (signed short)pos;
                    else if(flag & flag_l2)
                        *va_arg(vlist, u64*) = (u64)pos;
                    else
                        *va_arg(vlist, signed int*) = (signed int)pos;
                }
                ++s;
                break;

            case L'%':   /* output L'%' */
                if(p_start + 1 != s)
                    goto    put_invalid;
                string_put_char(&str, *s++);
                break;

            default:    /* invalid type */
                goto    put_invalid;

put_invalid:
                string_put_string(&str, p_start, s - p_start);
                break;

put_hexadecimal:
                radix = 16;
                flag |= flag_unsigned;
put_integer:
                {
                    u64 val = 0;
                    n_prefix = 0;

                    if(flag & flag_minus)
                        flag &= ~flag_zero;
                    if(precision < 0)   precision = 1;
                    else    flag &= ~flag_zero;

                    if(flag & flag_unsigned) {
                        if(flag & flag_h2)
                            val = va_arg(vlist, wchar_t);
                        else if(flag & flag_h1)
                            val = va_arg(vlist, unsigned short);
                        else if(flag & flag_l2)
                            val = va_arg(vlist, u64);
                        else
                            val = va_arg(vlist, unsigned long);
                        flag &= ~(flag_plus | flag_blank);
                        if(flag & flag_sharp) {
                            if(radix == 16) {
                                if(val != 0) {
                                    prefix[0] = (wchar_t)(hex_char + (10 + L'x' - L'a'));
                                    prefix[1] = L'0';
                                    n_prefix = 2;
                                }
                            } else if(radix == 8) {
                                prefix[0] = L'0';
                                n_prefix = 1;
                            }
                        }
                    } else {
                        if(flag & flag_h2)
                            val = va_arg(vlist, wchar_t);
                        else if(flag & flag_h1)
                            val = va_arg(vlist, short);
                        else if(flag & flag_l2)
                            val = va_arg(vlist, u64);
                        else
                            val = va_arg(vlist, long);
                        if((val >> 32) & 0x80000000) {
                            val = ~val + 1;
                            prefix[0] = L'-';
                            n_prefix = 1;
                        } else {
                            if(val || precision) {
                                if(flag & flag_plus) {
                                    prefix[0] = L'+';
                                    n_prefix = 1;
                                } else
                                if(flag & flag_blank) {
                                    prefix[0] = L' ';
                                    n_prefix = 1;
                                }
                            }
                        }
                    }
                    n_buf = 0;
                    while(val != 0) {
                        int d = (int)(val % radix);
                        val /= radix;
                        buf[n_buf++] = (wchar_t)((d < 10) ?
                            (d + L'0') : (d + hex_char));
                    }
                    if((n_prefix > 0) && (prefix[0] == L'0')) {
                        n_prefix = 0;
                        buf[n_buf++] = L'0';
                    }
                }
                goto    put_to_stream;

put_to_stream:
                {
                    int n_pad = precision - n_buf;
                    if(flag & flag_zero) {
                        if(n_pad < width - n_buf - n_prefix)
                            n_pad = width - n_buf - n_prefix;
                    }
                    if(n_pad > 0)   width -= n_pad;
                    width -= n_prefix + n_buf;
                    if(!(flag & flag_minus))
                        string_fill_char(&str, L' ', width);
                    while(n_prefix > 0)
                        string_put_char(&str, prefix[--n_prefix]);
                    string_fill_char(&str, L'0', n_pad);
                    while(n_buf > 0)
                        string_put_char(&str, buf[--n_buf]);
                    if(flag & flag_minus)
                        string_fill_char(&str, L' ', width);
                    ++s;
                }
                break;
            }
        }
    }

    if(str.len > 0)
        *str.cur = '\0';
    else if(len > 0)
        str.base[len - 1] = '\0';
    return  str.cur - str.base;
}

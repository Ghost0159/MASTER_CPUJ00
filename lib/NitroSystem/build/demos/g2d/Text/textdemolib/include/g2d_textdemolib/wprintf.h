/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib - include - g2d_textdemolib
  File:     wprintf.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/
#ifndef WPRINTF_H_
#define WPRINTF_H_


#ifdef __cplusplus
extern "C" {
#endif


int NNSi_G2dVSNWPrintf(wchar_t *dst, size_t len, const wchar_t *fmt, va_list vlist);


/*---------------------------------------------------------------------------*
  Name:         OS_SNPrintf

  Description:  equal to 'OS_VSNPrintf' except argument style.

  Arguments:    dst   : destination buffer.
                len   : destination buffer size.
                fmt   : format string.
 
  Returns:      length of the generated string. (except '\0')
                if(result < len),
                  put NUL in dst[result].
                else if(len > 0),
                  put NUL in dst[len - 1].
                else,
                  do nothing.
 *---------------------------------------------------------------------------*/
static inline int NNSi_G2dSNWPrintf(wchar_t *dst, size_t len, const wchar_t *fmt, ...)
{
    int ret;
    va_list va;
    va_start(va, fmt);
    ret = NNSi_G2dVSNWPrintf(dst, len, fmt, va);
    va_end(va);
    return  ret;
}

/*---------------------------------------------------------------------------*
  Name:         OS_VSPrintf

  Description:  equal to 'OS_VSNPrintf' except buffer size argument.

  Arguments:    dst   : destination buffer.
                fmt   : format string.
                vlist : parameters.

  Returns:      length of the generated string.
 *---------------------------------------------------------------------------*/
static inline int NNSi_G2dVSWPrintf(wchar_t *dst, const wchar_t *fmt, va_list vlist)
{
    return  NNSi_G2dVSNWPrintf(dst, 0x7FFFFFFF, fmt, vlist);
}

/*---------------------------------------------------------------------------*
  Name:         OS_SPrintf

  Description:  equal to 'OS_VSPrintf' except argument style.

  Arguments:    dst   : destination buffer.
                fmt   : format string.

  Returns:      length of the generated string.
 *---------------------------------------------------------------------------*/
static inline int NNSi_G2dSWPrintf(wchar_t *dst, const wchar_t *fmt, ...)
{
    int ret;
    va_list va;
    va_start(va, fmt);
    ret = NNSi_G2dVSWPrintf(dst, fmt, va);
    va_end(va);
    return  ret;
}




#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // WPRINTF_H_


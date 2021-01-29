/*---------------------------------------------------------------------------*
  Project:  NitroSDK - libraries - STD
  File:     std_unicode.c

  Copyright 2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: std_unicode.c,v $
  Revision 1.4  2007/02/20 00:28:09  kitase_hirotake
  indent source

  Revision 1.3  2006/08/10 07:53:08  yosizaki
  add STDResult and change API interface.

  Revision 1.2  2006/07/24 12:28:08  yosizaki
  beautify indent.
  change of STD-API interface.

  Revision 1.1  2006/07/05 07:43:43  kitase_hirotake
  std �� sjis <-> unicode �ϊ����C�u�����̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#include <nitro/std/unicode.h>
#include "sjis2unicode.h"
#include "unicode2sjis.h"


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         STD_ConvertStringSjisToUnicode

  Description:  ShiftJIS�������Unicode������ɕϊ�.

  Arguments:    dst               �ϊ���o�b�t�@.
                                  NULL ���w�肷��Ɗi�[�����͖��������.
                dst_len           �ϊ���o�b�t�@�̍ő啶�������i�[���ēn��,
                                  ���ۂɊi�[���ꂽ���������󂯎��|�C���^.
                                  NULL ��^�����ꍇ�͖��������.
                src               �ϊ����o�b�t�@.
                src_len           �ϊ����ׂ��ő啶�������i�[���ēn��,
                                  ���ۂɕϊ����ꂽ���������󂯎��|�C���^.
                                  ���̎w�����������I�[�̈ʒu���D�悳���.
                                  ���̒l���i�[���ēn���� NULL ��^�����ꍇ��
                                  �I�[�ʒu�܂ł̕��������w�肵���Ƃ݂Ȃ����.
                callback          �ϊ��ł��Ȃ����������ꂽ���ɌĂ΂��R�[���o�b�N.
                                  NULL���w�肵���ꍇ, �ϊ��ł��Ȃ������̈ʒu��
                                  �ϊ��������I������.

  Returns:      �ϊ������̌���.
 *---------------------------------------------------------------------------*/
STDResult STD_ConvertStringSjisToUnicode(u16 *dst, int *dst_len,
                                         const char *src, int *src_len,
                                         STDConvertUnicodeCallback callback)
{
    STDResult retval = STD_RESULT_SUCCESS;
    if (!src)
    {
        retval = STD_RESULT_INVALID_PARAM;
    }
    else
    {
        int     src_pos = 0;
        int     dst_pos = 0;
        int     src_max = (src_len && (*src_len >= 0)) ? *src_len : 0x7FFFFFFF;
        int     dst_max = (dst && dst_len && (*dst_len >= 0)) ? *dst_len : 0x7FFFFFFF;

        /* �����ꂩ�̃o�b�t�@�����I�[�ɒB����܂�1���������� */
        while ((dst_pos < dst_max) && (src_pos < src_max))
        {
            u16     dst_tmp[4];
            int     dst_count = 0;
            int     src_count;
            u32     c1 = (u8)src[src_pos];
            /* �����񒷂����I�[�̌��o��D�悷�� */
            if (!c1)
            {
                break;
            }
            /* ASCII [00, 7E] */
            else if (c1 <= 0x7E)
            {
                dst_tmp[0] = (u16)c1;
                src_count = 1;
                dst_count = 1;
            }
            /* ���p�J�i [A1, DF] */
            else if ((c1 >= 0xA1) && (c1 <= 0xDF))
            {
                dst_tmp[0] = (u16)(c1 + (0xFF61 - 0xA1));
                src_count = 1;
                dst_count = 1;
            }
            /* ShiftJIS */
            else if (STD_IsSjisCharacter(&src[src_pos]))
            {
                src_count = 2;
                if (src_pos + src_count <= src_max)
                {
                    /*
                     * �ϊ��e�[�u���͈̔͂�
                     * ��1�o�C�g { 81 - 9F, E0 - FC }, ��2�o�C�g { 40 - FF }.
                     * �e�[�u���v�Z���ɏ��Z�������������̂���
                     * ��2�o�C�g { 7F, FD, FE, FF } �͏ȗ������c���Ă���.
                     */
                    u32     c2 = (u8)src[src_pos + 1];
                    c1 -= 0x81 + ((c1 >= 0xE0) ? (0xE0 - 0xA0) : 0);
                    dst_tmp[0] = sjis2unicode_array[c1 * 0xC0 + (c2 - 0x40)];
                    dst_count = (dst_tmp[0] ? 1 : 0);
                }
            }
            /* �ϊ��ł��Ȃ����������ꂽ��R�[���o�b�N���Ăяo�� */
            if (dst_count == 0)
            {
                if (!callback)
                {
                    retval = STD_RESULT_CONVERSION_FAILED;
                }
                else
                {
                    src_count = src_max - src_pos;
                    dst_count = sizeof(dst_tmp) / sizeof(*dst_tmp);
                    retval = (*callback) (dst_tmp, &dst_count, &src[src_pos], &src_count);
                }
                if (retval != STD_RESULT_SUCCESS)
                {
                    break;
                }
            }
            /* �ϊ����܂��͕ϊ��悪�I�[�𒴂���Ȃ炱���ŏI�� */
            if ((src_pos + src_count > src_max) || (dst_pos + dst_count > dst_max))
            {
                break;
            }
            /* �o�b�t�@���L���Ȃ�Εϊ����ʂ��i�[ */
            if (dst)
            {
                int     i;
                for (i = 0; i < dst_count; ++i)
                {
                    MI_StoreLE16(&dst[dst_pos + i], (u16)dst_tmp[i]);
                }
            }
            src_pos += src_count;
            dst_pos += dst_count;
        }
        /* �ϊ���������Ԃ� */
        if (src_len)
        {
            *src_len = src_pos;
        }
        if (dst_len)
        {
            *dst_len = dst_pos;
        }
    }
    return retval;
}

/*---------------------------------------------------------------------------*
  Name:         STD_ConvertStringUnicodeToSjis

  Description:  Unicode�������ShiftJIS������ɕϊ�

  Arguments:    dst               �ϊ���o�b�t�@.
                                  NULL ���w�肷��Ɗi�[�����͖��������.
                dst_len           �ϊ���o�b�t�@�̍ő啶�������i�[���ēn��,
                                  ���ۂɊi�[���ꂽ���������󂯎��|�C���^.
                                  NULL ��^�����ꍇ�͖��������.
                src               �ϊ����o�b�t�@.
                src_len           �ϊ����ׂ��ő啶�������i�[���ēn��,
                                  ���ۂɕϊ����ꂽ���������󂯎��|�C���^.
                                  ���̎w�����������I�[�̈ʒu���D�悳���.
                                  ���̒l���i�[���ēn���� NULL ��^�����ꍇ��
                                  �I�[�ʒu�܂ł̕��������w�肵���Ƃ݂Ȃ����.
                callback          �ϊ��ł��Ȃ����������ꂽ���ɌĂ΂��R�[���o�b�N.
                                  NULL���w�肵���ꍇ, �ϊ��ł��Ȃ������̈ʒu��
                                  �ϊ��������I������.

  Returns:      �ϊ������̌���.
 *---------------------------------------------------------------------------*/
STDResult STD_ConvertStringUnicodeToSjis(char *dst, int *dst_len,
                                         const u16 *src, int *src_len,
                                         STDConvertSjisCallback callback)
{
    STDResult retval = STD_RESULT_SUCCESS;
    if (!src)
    {
        retval = STD_RESULT_INVALID_PARAM;
    }
    else
    {
        int     src_pos = 0;
        int     dst_pos = 0;
        int     src_max = (src_len && (*src_len >= 0)) ? *src_len : 0x7FFFFFFF;
        int     dst_max = (dst && dst_len && (*dst_len >= 0)) ? *dst_len : 0x7FFFFFFF;
        /* �����ꂩ�̃o�b�t�@�����I�[�ɒB����܂�1���������� */
        while ((dst_pos < dst_max) && (src_pos < src_max))
        {
            char    dst_tmp[4];
            int     dst_count = 0;
            int     src_count = 1;
            u32     w = MI_LoadLE16(&src[src_pos]);
            /* �����񒷂����I�[�̌��o��D�悷�� */
            if (!w)
            {
                break;
            }
            /* �O�� (���p�̈�) */
            else if ((w >= 0xE000) && (w < 0xF8FF))
            {
                const u32 sjis_page = 188UL;
                const u32 offset = w - 0xE000;
                u32     c1 = offset / sjis_page;
                u32     c2 = offset - c1 * sjis_page;
                dst_tmp[0] = (char)(c1 + 0xF0);
                dst_tmp[1] = (char)(c2 + ((c2 < 0x3F) ? 0x40 : 0x41));
                dst_count = 2;
            }
            else
            {
                /*
                 * �ϊ��e�[�u���͈̔͂�
                 * [0000-0480), [2000-2680), [3000-3400), [4E00-9FA8), [F928-FFE6).
                 * �P����, 0000�Ŗ��߂�ꂽ�̈��傫�����ɍ폜���ċl�߂Ă���.
                 */
				/* *INDENT-OFF* */
                static const int table[][2] =
                {
                    {0x0000,0x0480 - 0x0000,},
                    {0x2000,0x2680 - 0x2000,},
                    {0x3000,0x3400 - 0x3000,},
                    {0x4E00,0x9FA8 - 0x4E00,},
                    {0xF928,0xFFE6 - 0xF928,},
                };
				enum { table_max = sizeof(table) / (sizeof(int) * 2) };
				/* *INDENT-ON* */
                int     i;
                int     index = 0;
                for (i = 0; i < table_max; ++i)
                {
                    const int offset = (int)(w - table[i][0]);
                    /* �����Ȕ͈� */
                    if (offset < 0)
                    {
                        break;
                    }
                    /* �L���Ȕ͈� */
                    else if (offset < table[i][1])
                    {
                        index += offset;
                        dst_tmp[0] = (char)unicode2sjis_array[index * 2 + 0];
                        if (dst_tmp[0])
                        {
                            dst_tmp[1] = (char)unicode2sjis_array[index * 2 + 1];
                            dst_count = dst_tmp[1] ? 2 : 1;
                        }
                        break;
                    }
                    /* ����ȏ�͈̔� */
                    else
                    {
                        index += table[i][1];
                    }
                }
            }
            /* �ϊ��ł��Ȃ����������ꂽ��R�[���o�b�N���Ăяo�� */
            if (dst_count == 0)
            {
                if (!callback)
                {
                    retval = STD_RESULT_CONVERSION_FAILED;
                }
                else
                {
                    src_count = src_max - src_pos;
                    dst_count = sizeof(dst_tmp) / sizeof(*dst_tmp);
                    retval = (*callback) (dst_tmp, &dst_count, &src[src_pos], &src_count);
                }
                if (retval != STD_RESULT_SUCCESS)
                {
                    break;
                }
            }
            /* �ϊ����܂��͕ϊ��悪�I�[�𒴂���Ȃ炱���ŏI�� */
            if ((src_pos + src_count > src_max) || (dst_pos + dst_count > dst_max))
            {
                break;
            }
            /* �o�b�t�@���L���Ȃ�Εϊ����ʂ��i�[ */
            if (dst)
            {
                int     i;
                for (i = 0; i < dst_count; ++i)
                {
                    MI_StoreLE8(&dst[dst_pos + i], (u8)dst_tmp[i]);
                }
            }
            src_pos += src_count;
            dst_pos += dst_count;
        }
        /* �ϊ���������Ԃ� */
        if (src_len)
        {
            *src_len = src_pos;
        }
        if (dst_len)
        {
            *dst_len = dst_pos;
        }
    }
    return retval;
}

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - STD - include
  File:     unicode.h

  Copyright 2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: unicode.h,v $
  Revision 1.4  2007/02/20 00:28:09  kitase_hirotake
  indent source

  Revision 1.3  2006/08/10 07:52:49  yosizaki
  add STDResult and change API interface.

  Revision 1.2  2006/07/24 13:16:42  yosizaki
  beautify indent.
  change of STD-API interface.

  Revision 1.1  2006/07/05 07:42:51  kitase_hirotake
  std �� sjis <-> unicode �ϊ����C�u�����̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_STD_UNICODE_H_
#define NITRO_STD_UNICODE_H_

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************/
/* constant */

typedef enum STDResult
{
    STD_RESULT_SUCCESS,
    STD_RESULT_ERROR,
    STD_RESULT_INVALID_PARAM,
    STD_RESULT_CONVERSION_FAILED
}
STDResult;

/*****************************************************************************/
/* declaration */

//---- ConvertString callback
typedef STDResult(*STDConvertUnicodeCallback) (u16 *dst, int *dst_len, const char *src,
                                               int *src_len);
typedef STDResult(*STDConvertSjisCallback) (char *dst, int *dst_len, const u16 *src, int *src_len);


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         STD_IsSjisLeadByte

  Description:  �w�肵��������ShiftJIS�̐�s�o�C�g�ł��邩����.

  Arguments:    c                 ���肷�镶��.

  Returns:      ShiftJIS�̐�s�o�C�g�Ȃ�TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL STD_IsSjisLeadByte(int c)
{
    return ((unsigned int)((((u8)c) ^ 0x20) - 0xA1) < 0x3C);
}

/*---------------------------------------------------------------------------*
  Name:         STD_IsSjisTrailByte

  Description:  �w�肵��������ShiftJIS�̌㑱�o�C�g�ł��邩����.

  Arguments:    c                 ���肷�镶��.

  Returns:      ShiftJIS�̌㑱�o�C�g�Ȃ�TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL STD_IsSjisTrailByte(int c)
{
    return (c != 0x7F) && ((u8)(c - 0x40) <= (0xFC - 0x40));
}

/*---------------------------------------------------------------------------*
  Name:         STD_IsSjisCharacter

  Description:  �w�肵��������ShiftJIS��1�����ł��邩����.

  Arguments:    s                 ���肷�镶����.

  Returns:      ShiftJIS��1�����Ȃ�TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL STD_IsSjisCharacter(const char *s)
{
    return STD_IsSjisLeadByte(s[0]) && STD_IsSjisTrailByte(s[1]);
}

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
                                         STDConvertUnicodeCallback callback);

/*---------------------------------------------------------------------------*
  Name:         STD_ConvertCharSjisToUnicode.

  Description:  ShiftJIS������Unicode�����ɕϊ�.

  Arguments:    dst               �ϊ���o�b�t�@.
                src               �ϊ���������.

  Returns:      �ϊ�����������.
                �ϊ��Ɏ��s�����ꍇ�� -1.
 *---------------------------------------------------------------------------*/
SDK_INLINE int STD_ConvertCharSjisToUnicode(u16 *dst, const char *src)
{
    int     src_len = STD_IsSjisCharacter(src) ? 2 : 1;
    int     dst_len = 1;
    STDResult ret = STD_ConvertStringSjisToUnicode(dst, &dst_len, src, &src_len, NULL);
    return (ret == STD_RESULT_SUCCESS) ? dst_len : -1;
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
                                         STDConvertSjisCallback callback);

/*---------------------------------------------------------------------------*
  Name:         STD_ConvertCharUnicodeToSjis

  Description:  Unicode�������ShiftJIS������ɕϊ�.

  Arguments:    dst               �ϊ���o�b�t�@.
                                  2�o�C�g���m�ۂ���K�v������.
                src               �ϊ���������.

  Returns:      �ϊ������o�C�g��.
                �ϊ��Ɏ��s�����ꍇ�� -1.
 *---------------------------------------------------------------------------*/
SDK_INLINE int STD_ConvertCharUnicodeToSjis(char *dst, u16 src)
{
    int     src_len = 1;
    int     dst_len = 2;
    STDResult ret = STD_ConvertStringUnicodeToSjis(dst, &dst_len, &src, &src_len, NULL);
    return (ret == STD_RESULT_SUCCESS) ? dst_len : -1;
}


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_STD_UNICODE_H_ */
#endif

/*---------------------------------------------------------------------------*
# Project:  NitroSDK - libraries - utest
  File:     utest_assert.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: utest_assert.c,v $
  Revision 1.2  2006/01/18 02:12:38  kitase_hirotake
  do-indent

  Revision 1.1  2005/06/24 02:19:25  yasu
  ���j�b�g�e�X�g�̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitro/utest.h>

/*---------------------------------------------------------------------------*
  �ʏ�� ASSERT
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         UTi_Assert
  Description:  exp_val �̒l����0�Ȃ� NG ���b�Z�[�W���o�͂���
  Arguments:    file:    __FILE__ �\�[�X�t�@�C����
                line:    __LINE__ �\�[�X�t�@�C�����C����
                exp_str: �\�����郁�b�Z�[�W�t�H�[�}�b�g
                exp_val: �\������l
  Returns:      None.
 *---------------------------------------------------------------------------*/
void UTi_Assert(const char *file, int line, const char *exp_str, int exp_val)
{
    // val ���O�Ȃ� NG �Ƃ���
    if (!exp_val)
    {
        UTi_SetResult(UT_RESULT_NG);
        UTi_SetMessage(file, line, "UT_Assert: %s[=%d] is not TRUE\n", exp_str, exp_val);
    }

    // val ����O�ł���A����܂ł̃e�X�g�Ɏ��s���Ă��Ȃ���� OK �Ƃ���
    else if (UTi_GetResult() != UT_RESULT_NG)
    {
        UTi_SetResult(UT_RESULT_OK);
    }
    return;
}

/*---------------------------------------------------------------------------*
  Name:         UTi_AssertEq
  Description:  exp_val1 �̒l�� exp_val2 ���������Ȃ���� NG ���b�Z�[�W���o�͂���
  Arguments:    file:    __FILE__ �\�[�X�t�@�C����
                line:    __LINE__ �\�[�X�t�@�C�����C����
                exp_str1: ��r����l1 �̃e�L�X�g
                exp_val1: ��r����l1
                exp_str2: ��r����l2 �̃e�L�X�g
                exp_val2: ��r����l2
  Returns:      None.
 *---------------------------------------------------------------------------*/
void UTi_AssertEq(const char *file, int line, const char *exp_str1, int exp_val1,
                  const char *exp_str2, int exp_val2)
{
    // val1 �� val2 ���������Ȃ��Ȃ� NG �Ƃ���
    if (exp_val1 != exp_val2)
    {
        UTi_SetResult(UT_RESULT_NG);
        UTi_SetMessage(file, line, "UT_AssertEq: %s[=%d] is not equal to %s[=%d]\n", exp_str1,
                       exp_val1, exp_str2, exp_val2);
    }

    // val1 �� val2 �����������A����܂ł̃e�X�g�Ɏ��s���Ă��Ȃ���� OK �Ƃ���
    else if (UTi_GetResult() != UT_RESULT_NG)
    {
        UTi_SetResult(UT_RESULT_OK);
    }
    return;
}


/*---------------------------------------------------------------------------*
  Name:         UTi_AssertNe
  Description:  exp_val1 �̒l�� exp_val2 ����������� NG ���b�Z�[�W���o�͂���
  Arguments:    file:    __FILE__ �\�[�X�t�@�C����
                line:    __LINE__ �\�[�X�t�@�C�����C����
                exp_str1: ��r����l1 �̃e�L�X�g
                exp_val1: ��r����l1
                exp_str2: ��r����l2 �̃e�L�X�g
                exp_val2: ��r����l2
  Returns:      None.
 *---------------------------------------------------------------------------*/
void UTi_AssertNe(const char *file, int line, const char *exp_str1, int exp_val1,
                  const char *exp_str2, int exp_val2)
{
    // val1 �� val2 ���������Ȃ� NG �Ƃ���
    if (exp_val1 == exp_val2)
    {
        UTi_SetResult(UT_RESULT_NG);
        UTi_SetMessage(file, line, "UT_AssertNe: %s[=%d] is equal to %s[=%d]\n", exp_str1, exp_val1,
                       exp_str2, exp_val2);
    }

    // val1 �� val2 ���������Ȃ��A������܂ł̃e�X�g�Ɏ��s���Ă��Ȃ���� OK �Ƃ���
    else if (UTi_GetResult() != UT_RESULT_NG)
    {
        UTi_SetResult(UT_RESULT_OK);
    }
    return;
}


/*---------------------------------------------------------------------------*
  Name:         UTi_AssertMemEq
  
  Description:  exp_len �o�C�g�̃������u���b�N exp_ptr1 �� exp_ptr2 ��
                �������Ȃ���� NG ���b�Z�[�W���o�͂���
  
  Arguments:    file:    __FILE__ �\�[�X�t�@�C����
                line:    __LINE__ �\�[�X�t�@�C�����C����
                exp_str1: ��r����u���b�N1 �̃e�L�X�g�\��
                exp_ptr1: ��r����u���b�N1
                exp_str2: ��r����u���b�N2 �̃e�L�X�g�\��
                exp_ptr2: ��r����u���b�N2
                exp_len : ��r����u���b�N�̃T�C�Y
  Returns:      None.
 *---------------------------------------------------------------------------*/
void UTi_AssertMemEq(const char *file, int line,
                     const char *exp_str1, const void *exp_ptr1,
                     const char *exp_str2, const void *exp_ptr2, int exp_len)
{
    // exp_ptr1 �� exp_ptr2 �� exp_len �o�C�g�������������Ȃ���� NG �Ƃ���
    if (UTi_MemCmp(exp_ptr1, exp_ptr2, exp_len))
    {
        UTi_SetResult(UT_RESULT_NG);
        UTi_SetMessage(file, line, "UT_AssertMemEq: %s is not equal to %s (size=%d)\n", exp_str1,
                       exp_str2, exp_len);
    }

    // val1 �� val2 ���������A������܂ł̃e�X�g�Ɏ��s���Ă��Ȃ���� OK �Ƃ���
    else if (UTi_GetResult() != UT_RESULT_NG)
    {
        UTi_SetResult(UT_RESULT_OK);
    }
    return;
}


/*---------------------------------------------------------------------------*
  Name:         UTi_AssertMemNe
  
  Description:  exp_len �o�C�g�̃������u���b�N exp_ptr1 �� exp_ptr2 ��
                ��������� NG ���b�Z�[�W���o�͂���
  
  Arguments:    file:    __FILE__ �\�[�X�t�@�C����
                line:    __LINE__ �\�[�X�t�@�C�����C����
                exp_str1: ��r����u���b�N1 �̃e�L�X�g�\��
                exp_ptr1: ��r����u���b�N1
                exp_str2: ��r����u���b�N2 �̃e�L�X�g�\��
                exp_ptr2: ��r����u���b�N2
                exp_len : ��r����u���b�N�̃T�C�Y
  Returns:      None.
 *---------------------------------------------------------------------------*/
void UTi_AssertMemNe(const char *file, int line,
                     const char *exp_str1, const void *exp_ptr1,
                     const char *exp_str2, const void *exp_ptr2, int exp_len)
{
    // exp_ptr1 �� exp_ptr2 �� exp_len �o�C�g��������������� NG �Ƃ���
    if (!UTi_MemCmp(exp_ptr1, exp_ptr2, exp_len))
    {
        UTi_SetResult(UT_RESULT_NG);
        UTi_SetMessage(file, line, "UT_AssertMemNe: %s is equal to %s (size=%d)\n", exp_str1,
                       exp_str2, exp_len);
    }

    // val1 �� val2 ���������Ȃ��A������܂ł̃e�X�g�Ɏ��s���Ă��Ȃ���� OK �Ƃ���
    else if (UTi_GetResult() != UT_RESULT_NG)
    {
        UTi_SetResult(UT_RESULT_OK);
    }
    return;
}


/*---------------------------------------------------------------------------*
  ASSERT �@�\�� ASSERT
 *---------------------------------------------------------------------------*/

OSContext UTi_AssertContext;

/*---------------------------------------------------------------------------*
  Name:         UTi_Terminate
  Description:  ��~�����������ւ���. ASSERT ��Ɍ��ɖ߂�
  Arguments:    None.
  Returns:      None.
 *---------------------------------------------------------------------------*/
#ifdef SDK_DEBUG
void UTi_Terminate(void)
{
    OS_LoadContext(&UTi_AssertContext);
}
#endif


/*---------------------------------------------------------------------------*
  Name:         UTi_AssertAsserted
  Description:  Assert ���������Ă��Ȃ��Ƃ��̃G���[���b�Z�[�W���o�͂���
  Arguments:    file:    __FILE__ �\�[�X�t�@�C����
                line:    __LINE__ �\�[�X�t�@�C�����C����
                exp_str:  �\�����郁�b�Z�[�W�t�H�[�}�b�g
                asserted:�A�T�[�g���N�����Ă����� 1 �����Ȃ��� 0
  Returns:      None.
 *---------------------------------------------------------------------------*/
void UTi_AssertAsserted(const char *file, int line, const char *exp_str, int asserted)
{
    // �A�T�[�g���N����Ȃ��Ȃ� NG �Ƃ���
    if (!asserted)
    {
        UTi_SetResult(UT_RESULT_NG);
        UTi_SetMessage(file, line, "UT_AssertAsserted: %s not asserted\n", exp_str);
    }

    // �A�T�[�g���N�����Ă���A������܂ł̃e�X�g�Ɏ��s���Ă��Ȃ���� OK �Ƃ���
    else if (UTi_GetResult() != UT_RESULT_NG)
    {
        UTi_SetResult(UT_RESULT_OK);
    }
    return;
}


/*---------------------------------------------------------------------------*
  Name:         UTi_AssertNotAsserted
  Description:  Assert ���������Ă���Ƃ��̃G���[���b�Z�[�W���o�͂���
  Arguments:    file:    __FILE__ �\�[�X�t�@�C����
                line:    __LINE__ �\�[�X�t�@�C�����C����
                exp_str:  �\�����郁�b�Z�[�W�t�H�[�}�b�g
                asserted:�A�T�[�g���N�����Ă����� 1 �����Ȃ��� 0
  Returns:      None.
 *---------------------------------------------------------------------------*/
void UTi_AssertNotAsserted(const char *file, int line, const char *exp_str, int asserted)
{
    // �A�T�[�g���N�����Ȃ� NG �Ƃ���
    if (asserted)
    {
        UTi_SetResult(UT_RESULT_NG);
        UTi_SetMessage(file, line, "UT_AssertNotAsserted: %s asserted\n", exp_str);
    }

    // �A�T�[�g���N�����Ă��Ȃ��A������܂ł̃e�X�g�Ɏ��s���Ă��Ȃ���� OK �Ƃ���
    else if (UTi_GetResult() != UT_RESULT_NG)
    {
        UTi_SetResult(UT_RESULT_OK);
    }
    return;
}


/*---------------------------------------------------------------------------*
  Name:         UTi_MemCmp
  
  Description:  len �o�C�g�̃������u���b�N ptr1 �� ptr2 ���r����
  
  Arguments:    ptr1: ��r����u���b�N1
                ptr2: ��r����u���b�N2
                len : ��r����u���b�N�̃T�C�Y
  
  Returns:      0    : ������.
                0�ȊO: �������Ȃ�
 *---------------------------------------------------------------------------*/
int UTi_MemCmp(const void *ptr1, const void *ptr2, int len)
{
    int     i;

    for (i = 0; i < len; i++)
    {
        if (((unsigned char *)ptr1)[i] != ((unsigned char *)ptr2)[i])
        {
            return 1;
        }
    }
    return 0;
}

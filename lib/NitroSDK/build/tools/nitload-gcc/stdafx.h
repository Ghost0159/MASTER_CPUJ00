/*---------------------------------------------------------------------------*
  Project:  NitroSDK - Tools - nitro-gcc
  File:     stdafx.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: stdafx.h,v $
  Revision 1.2  2006/01/18 02:12:29  kitase_hirotake
  do-indent

  Revision 1.1  2005/06/21 06:22:33  yasu
  ���ō쐬

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "stdafx_orig.h"
#include "nitrodll.h"

//
// �R���\�[�����b�Z�[�W�\���̗}��
//
#define  printf(...)		((void)0)

/*---------------------------------------------------------------------------*
  Description:  dll.GetDebugPrint �̃n�b�N
                Makefile ���� dll.GetDebugPrint �� dll_GetDebugPrint �ɒu����
                ������������֕��򂳂���D
                �����X�g���[���� -1 ������Ă����炻���Ńv���O�������I������
                -1 �̎��̕��������^�[���o�����[�ƂȂ�
 *---------------------------------------------------------------------------*/
BOOL    dll_GetDebugPrint(CNITRODLL * dll, NITROArch arch, char *string, INT * string_lenp,
                          INT string_max);
BOOL dll_GetDebugPrint(CNITRODLL * dll, NITROArch arch, char *string, INT * string_lenp,
                       INT string_max)
{
    BOOL    result = dll->GetDebugPrint(arch, string, string_lenp, string_max);
    int     retval;
    int     i;
    static BOOL isexit = FALSE;

    if (result && *string_lenp)
    {
        for (i = 0; i < *string_lenp; i++)
        {
            //
            // �ȑO�ɏI���g�[�N�����󂯎���Ă��邩�ǂ������肵�A
            // �󂯎���Ă���Ȃ�I���������s�Ȃ��D
            //
            if (isexit)
            {
                if (i > 0)             // �R���\�[���o�b�t�@�̃t���b�V��
                {
                    fputs(string, stdout);
                }
                retval = string[i] - '0';
                dll->Close();
                exit(retval);
            }

            //
            // �I���g�[�N�����ǂ����̔���
            //    �I���Ȃ玟�̒l���A�v���P�[�V�����̕Ԓl�ƂȂ�̂�
            //    �t���O�𗧂Ă�D
            //    �킴�킴�t���O�𗧂Ă�̂͏I���g�[�N���ƕԒl�� 2 ��
            //    �o�b�t�@�ɂ܂������đ����Ă���ꍇ�̑Ώ��D
            //
            if (string[i] == (char)-1)
            {
                string[i] = '\0';
                isexit = TRUE;
            }
        }
    }
    return result;
}

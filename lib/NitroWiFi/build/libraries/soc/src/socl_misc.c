/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_misc.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_misc.c,v $
  Revision 1.3  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/08/18 13:19:05  yasu
  Roundup4 �֐��̈ړ�

  Revision 1.1  2005/08/07 13:07:59  yasu
  �G���[�R�[�h�̖��̂̕\��

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/soc.h>

struct errortable
{
    int code;
    const char*     name;
};

static struct errortable    errtable[] =
{
#include "errtable.h"
};

/*---------------------------------------------------------------------------*
  Name:         SOCL_GetErrName

  Description:  SOCL_EXXXX �̃G���[�R�[�h�����擾����

  Arguments:    �G���[�R�[�h
  
  Returns:      �G���[�R�[�h�� Enum ��
 *---------------------------------------------------------------------------*/
const char* SOCL_GetErrName(int errcode)
{
    int i;

    for (i = 0; i < sizeof(errtable) / sizeof(errtable[0]); i++)
    {
        if (errtable[i].code == errcode)
        {
            return errtable[i].name;
        }
    }

    return "";
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ROUNDUP4/SOCLi_RoundUp4
  
  Description:  4 �̔{���ɐ؂�グ��
                �R�[�h�T�C�Y�����炷���߂Ƀ}�N���Ɗ֐���2��p�ӂ��Ă���
                size ���萔�̏ꍇ�̓}�N���̕����g���R���p�C���̍œK������
                �҂�������悢
  
  Arguments:    size     �؂�グ�����l
  
  Returns:      size �� 4 �ɐ؂�グ����
 *---------------------------------------------------------------------------*/
u32 SOCLi_RoundUp4(u32 size)
{
    return SOCLi_ROUNDUP4(size);
}

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - tools - defval
  File:     get_defval.c

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: get_defval.c,v $
  Revision 1.5  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.4  2007/02/07 01:33:34  yasu
  ����N�x�C��

  Revision 1.3  2007/02/01 11:24:22  yasu
  binmode/textmode �Ή�

  Revision 1.2  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.1  2005/06/20 07:07:15  yasu
  add defval

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "defval.h"

/*---------------------------------------------------------------------------*
  Name:         get_dvalue_listptr
  
  Description:  ��`���ɑΉ����郊�X�g�ւ̃|�C���^���擾����
  
  Arguments:    name        �ϐ��̖��O
  
  Returns:      ��`���X�g�ւ̃|�C���^
 *---------------------------------------------------------------------------*/
tDefineValue *get_dvalue_listptr(const char *name)
{
    tDefineValue *t;

    for (t = gDefineValueTop; t; t = t->next)
    {
        if (!strcmp(t->name, name))
        {
            return t;
        }
    }
    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         get_dvalue
  
  Description:  ��`���̒l���擾����B
                �܂���`�l���X�g�����������A�Ȃ���Ί��ϐ�����������
                ����ł��Ȃ���� NULL ��Ԃ��B
  
  Arguments:    name        �ϐ��̖��O
  
  Returns:      �l������ւ̃|�C���^(free �s�v)
                NULL: �Ή�����ϐ����Ȃ�
 *---------------------------------------------------------------------------*/
static char *get_dvalue(const char *name)
{
    tDefineValue *t = get_dvalue_listptr(name);

    return t ? t->value : getenv(name);
}

/*---------------------------------------------------------------------------*
  Name:         modify_dvalue
  
  Description:  �ϐ��C���q :r,:t,:e,:h �ɂ��ϊ��������󂯂���̒�`�l��
                �������ĕԂ��B
  
  Arguments:    value     �C�������l
                modifier  �C���q 'r', 't', 'e', 'h' �̂ǂꂩ
  
  Returns:      �ϊ��l������ւ̃|�C���^(free �K�v)
 *---------------------------------------------------------------------------*/
static char *modify_dvalue(const char *value, char modifier)
{
    char   *value_dir;
    char   *value_base;
    char   *value_ext;
    char   *modified_value;

    if (value == NULL)
        return NULL;
    if (modifier == '\0')
        return StrDup(value);

    //
    // �C���q�̏���
    //   ���͒l���t�@�C�����̍\���v�f�ɕ������C���q�ɉ����čč\������
    //
    UnpackFileName(value, &value_dir, &value_base, &value_ext);

    switch (modifier)
    {
    case 'h':                         // �Ō�� /, \ ���̃f�B���N�g����
        modified_value = StrDup(value_dir);
        break;

    case 't':                         // �f�B���N�g�������̃t�@�C����
        modified_value = StrCatDup(value_base, value_ext);
        break;

    case 'r':                         // �t���p�X����g���q�𖳂�������
        modified_value = StrCatDup(value_dir, value_base);
        break;

    case 'e':                         // File extension
        modified_value = StrDup(value_ext);
        break;

    default:                          // Unknown
        fprintf(stderr, "Unknown modifier ':%c'... Ignored.\n", modifier);
        modified_value = StrDup(value);
        break;
    }

    free(value_dir);
    free(value_base);
    free(value_ext);

    return modified_value;
}

/*---------------------------------------------------------------------------*
  Name:         get_modified_dvalue
  
  Description:  ��`�l���X�g���玝���Ă����l�ɁA
                �C���q :r,:t,:e,:h �ɂ��ϊ������������A�������ĕԂ��B
  
  Arguments:    name      ��`��
  
  Returns:      �ϊ��l������ւ̃|�C���^(free �K�v)
 *---------------------------------------------------------------------------*/
static char *get_modified_dvalue(const char *name)
{
    char   *name_copy = StrDup(name);
    int     name_len = strlen(name_copy);
    char    modifier = '\0';
    char   *value;

    // �C���q�̎��o��
    if (name_len > 2 && name_copy[name_len - 2] == ':')
    {
        name_copy[name_len - 2] = '\0';
        modifier = name_copy[name_len - 1];
    }

    // �l�擾
    value = modify_dvalue(get_dvalue(name_copy), modifier);
    free(name_copy);

    DebugPrintf("REFERED(%s)=[%s]\n", name, value ? value : "(NULL)");
    return value;
}

/*---------------------------------------------------------------------------*
  Name:         put_modified_dvalue
  
  Description:  ��������t�@�C���֏o�͂���
                ���̍ۂ� $(XXXXX) ��W�J����
  
  Arguments:    fp   �o�̓t�@�C����
                str  ������
  
  Returns:      TRUE ����
 *---------------------------------------------------------------------------*/
BOOL puts_modified_dvalue(FILE * fp, const char *str)
{
    const char *str_name;
    int     str_name_len;
    char   *name;
    char   *value;
    int     result;

    while ('\0' != *str)
    {
        //
        // $(XXX) ��T���A����ΑΉ�����l���o�͂���
        //
        if ('$' == *str)
        {
            if ('$' == *(str + 1))
            {
                str++;                 // $$ �Ȃ� $ �ɕϊ�����
            }
            else if ('(' == *(str + 1))
            {
                str_name = str + 2;
                str_name_len = 0;
                while ('\0' != str_name[str_name_len])
                {
                    if (')' == str_name[str_name_len])
                    {
                        name = StrNDup(str_name, str_name_len);
                        value = get_modified_dvalue(name);
                        free(name);

                        if (NULL != value)
                        {
                            result = fputs(value, fp);
                            free(value);
                            if (result == EOF)
                            {
                                return FALSE;
                            }
                        }
                        str = str_name + str_name_len;
                        goto skipout;
                    }
                    str_name_len++;
                }
            }
        }

        //
        // $(XXX) �łȂ��Ȃ�ꕶ�����o�͂���
        //
        if (EOF == fputc(*str, fp))
        {
            return FALSE;
        }
      skipout:
        str++;
    }
    return TRUE;
}

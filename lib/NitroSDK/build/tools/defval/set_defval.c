/*---------------------------------------------------------------------------*
  Project:  NitroSDK - tools - defval
  File:     set_defval.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: set_defval.c,v $
  Revision 1.2  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.1  2005/06/20 07:07:15  yasu
  add defval

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "defval.h"

static BOOL add_dvalue(const char *name, const char *value);
static char *clip_whitespace(char *str);


//---------------------------------------------------------------------------
//  ��`�l�ێ����X�g
//---------------------------------------------------------------------------
tDefineValue *gDefineValueTop = NULL;

/*---------------------------------------------------------------------------*
  Name:         add_dvalue
  
  Description:  ��`�l���X�g�ɐV�����l��ǉ�����
  
  Arguments:    name  �ϐ��̖��O
                value �ϐ��̒l
  
                name  �� NULL ���邢�� "" �Ȃ牽�������ɏI��(�Ԓl FALSE)
                value �� NULL �Ȃ� "" ����`����Ă���Ƃ݂Ȃ��B
  
  Returns:      TRUE   �V�K�ɒǉ�����
                FALSE  ���ɓ����̒l���o�^�ς�(��̂��̂ɏ㏑�������)
 *---------------------------------------------------------------------------*/
static BOOL add_dvalue(const char *name, const char *value)
{
    tDefineValue *t;

    //
    // NULL �̂Ƃ��̑Ώ�
    //   name  �� NULL ���邢�� "" �Ȃ牽�������ɏI��(�Ԓl TRUE)
    //   value �� NULL �Ȃ� "" ����`����Ă���Ƃ݂Ȃ��B
    //
    if (name == NULL || name[0] == '\0')
    {
        return TRUE;
    }
    if (value == NULL)
    {
        value = (const char *)"";
    }

    DebugPrintf("DEFINE:$(%s)=\"%s\"\n", name, value);

    // �����̒�`��������ΐV�����l�ƍ����ւ���
    if (NULL != (t = get_dvalue_listptr(name)))
    {
        if (t->value)
            free(t->value);
        t->value = StrDup(value);
        return FALSE;
    }

    // �V������`���Ȃ烊�X�g���������o�^����
    t = Calloc(sizeof(tDefineValue));
    t->name = StrDup(name);
    t->value = StrDup(value);
    t->next = gDefineValueTop;
    gDefineValueTop = t;

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         add_dvalue_by_equality
  
  Description:  ��`�l���X�g�ɓ����ŐV�����l��ǉ�����
  
  Arguments:    equality ���� "DEFINE=VALUE"
  
                equality �� NULL �܂��� "" �̂Ƃ��͉������܂���B
  
  Returns:      TRUE   �V�K�ɒǉ�����
                FALSE  ���ɓ����̒l���o�^�ς�(��̂��̂ɏ㏑�������)
 *---------------------------------------------------------------------------*/
BOOL add_dvalue_by_equality(const char *equality)
{
    char   *name;
    char   *value;
    char   *scratch;
    BOOL    result;

    //
    //  ������ύX���\�ȗ̈�ɃR�s�[�� '=' �̑O��� name �� value �ɕ�����
    //
    scratch = StrDup(equality);
    name = scratch;
    value = scratch;
    while ('\0' != *value)
    {
        if ('=' == *value)
        {
            *value = '\0';             // '=' �̈ʒu�� '\0' �����邱�Ƃŕ���
            value++;
            break;
        }
        value++;
    }
    name = clip_whitespace(name);      // �O��̋󔒂��폜����
    value = clip_whitespace(value);

    //
    //  �l��o�^����
    //
    result = add_dvalue(name, value);
    free(scratch);

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         add_dvalue_from_file
  
  Description:  ��`�l���X�g�Ƀt�@�C�����̓�����ǉ�����
  
  Arguments:    filename �t�@�C����
                "DEFINE=VALUE" �̗���
  
  Returns:      TRUE   ��������
                FALSE  ���ɓ����̒l���o�^�ς�(��̂��̂ɏ㏑�������)
 *---------------------------------------------------------------------------*/
BOOL add_dvalue_from_file(const char *filename)
{
    FILE   *fp;
    char   *buffer;
    int     buffer_size;
    int     line_num;

    if (filename[0] == '-' && filename[1] == '\0')
    {
        fp = stdin;
    }
    else if (NULL == (fp = fopen(filename, "r")))
    {
        fprintf(stderr, "Cannot open file \"%s\".\n", filename);
        return FALSE;
    }

    //
    // �t�@�C������s���ǂݍ��݁A�ϐ��o�^����
    //
    buffer = NULL;
    buffer_size = line_num = 0;
    while (NULL != Fgets(&buffer, &buffer_size, fp))
    {
        line_num++;
        if (!add_dvalue_by_equality(buffer))
        {
            fprintf(stderr, "line %d: found same entry: %s\n", line_num, buffer);
        }
    }

    if (fp != stdin)
        fclose(fp);

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         chip_whitespace
  
  Description:  ������O��̋󔒂��폜����
  
  Arguments:    str    �����Ώە�����̐擪
  
                ���� str �����͏㏑�������̂� const �̈�ł͓��삵�Ȃ�
  
  Returns:      �V�����擪�ʒu
 *---------------------------------------------------------------------------*/
static char *clip_whitespace(char *str)
{
    int     n;

    // ��̋󔒂� '\0' �𖄂߂�
    for (n = strlen(str) - 1; n >= 0; n--)
    {
        if (!isspace(str[n]))
        {
            str[n + 1] = '\0';
            break;
        }
    }

    // �O�̋󔒂��X�L�b�v���A�擪�̕����̃|�C���^��Ԃ�
    for (n = 0; str[n] != '\0'; n++)
    {
        if (!isspace(str[n]))
        {
            break;
        }
    }
    return str + n;
}

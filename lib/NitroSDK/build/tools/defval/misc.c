/*---------------------------------------------------------------------------*
  Project:  NitroSDK - tools - defval
  File:     misc.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: misc.c,v $
  Revision 1.2  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.1  2005/06/20 07:07:15  yasu
  add defval

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "misc.h"

/*---------------------------------------------------------------------------*
  Name:         StrDup

  Description:  strdup �̃G���[�n���h�����O��
  
  Arguments:    str     �R�s�[���镶����
  
  Returns:      �R�s�[���ꂽ������
 *---------------------------------------------------------------------------*/
char   *StrDup(const char *str)
{
    char   *cp;

    if (NULL == (cp = strdup(str ? str : "")))
    {
        fprintf(stderr, "Error: No memory.");
        exit(2);
    }
    return cp;
}

/*---------------------------------------------------------------------------*
  Name:         StrNDup

  Description:  �������w����� strdup �̃G���[�n���h�����O��
  
  Arguments:    str     �R�s�[���镶����
                len     �R�s�[���镶����('\0'���܂܂Ȃ�)
  
  Returns:      �R�s�[���ꂽ������
 *---------------------------------------------------------------------------*/
char   *StrNDup(const char *str, int len)
{
    char   *cp;

    cp = Calloc(len + 1);
    if (str && len)
        (void)strncpy(cp, str, len);
    return cp;

    // malloc �� len+1 �o�C�g���̈���m�ۂ��Ă���̂�
    // strncpy ��� '\0' �̕ʓr�ǉ��͕K�v�Ȃ��D
    // �܂� str �� NULL �̂Ƃ����Ή��ł���
}

/*---------------------------------------------------------------------------*
  Name:         StrCatDup

  Description:  �������A���������̂��q�[�v�̈悩��m�ۂ���
  
  Arguments:    str1    ������1 ==NULL �̎��� "" �Ƃ��Ĉ�����
                str2    ������2 ==NULL �̎��� "" �Ƃ��Ĉ�����
  
  Returns:      �A������������
 *---------------------------------------------------------------------------*/
char   *StrCatDup(const char *str1, const char *str2)
{
    int     len1, len2;
    char   *cp;

    len1 = str1 ? strlen(str1) : 0;
    len2 = str2 ? strlen(str2) : 0;

    cp = Calloc(len1 + len2 + 1);

    if (str1)
        (void)strcpy(cp, str1);
    if (str2)
        (void)strcat(cp, str2);

    return cp;
}

/*---------------------------------------------------------------------------*
  Name:         Calloc

  Description:  calloc �̃G���[�n���h�����O��(�����͈�ɂȂ��Ă���)
  
  Arguments:    size    �m�ۂ���̈撷
  
  Returns:      �m�ۂ��ꂽ�̈�
 *---------------------------------------------------------------------------*/
void   *Calloc(int size)
{
    void   *cp;

    if (NULL == (cp = calloc(1, size)))
    {
        fprintf(stderr, "Error: No memory.");
        exit(2);
    }
    return cp;
}

/*---------------------------------------------------------------------------*
  Name:         Free

  Description:  free �� NULL �n���h�����O��
                �ϐ� *ptr �� NULL �N���A�����
  
  Arguments:    ptr  free ����̈�
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void Free(void **ptr)
{
    if (NULL != *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
    return;
}

/*---------------------------------------------------------------------------*
  Name:         Realloc

  Description:  realloc �̃G���[�n���h�����O��
  
  Arguments:    buffer  ���̃T�C�Y
                size    �m�ۂ���̈撷
  
  Returns:      �m�ۂ��ꂽ�̈�
 *---------------------------------------------------------------------------*/
void   *Realloc(void *buffer, int size)
{
    void   *cp;

    cp = Calloc(size);
    if (buffer)
    {
        strcpy(cp, buffer);
        free(buffer);
    }
    return cp;
}

/*---------------------------------------------------------------------------*
  Name:         Fopen

  Description:  fopen �� stdin/stdout ���l����������
                '-' ���w�肵���Ƃ��ɃA�N�Z�X���[�h 'r' �̂Ƃ� stdin  ��
                                    �A�N�Z�X���[�h 'w' �̂Ƃ� stdout �ɐݒ�
  
  Arguments:    name   �t�@�C���o�b�t�@�ւ̃|�C���^(�ւ̃|�C���^)
                mode   �t�@�C���o�b�t�@�̃T�C�Y(�ւ̃|�C���^)
  
  Returns:      fp  �t�@�C���|�C���^
 *---------------------------------------------------------------------------*/
FILE   *Fopen(const char *filename, const char *mode)
{
    FILE   *fp;

    DebugPrintf("filename=[%s] mode=[%s]\n", filename, mode);

    if ('-' == filename[0] && '\0' == filename[1])
    {
        fp = (mode[0] == 'r') ? stdin : stdout;
    }
    else
    {
        if (NULL == (fp = fopen(filename, mode)))
        {
            fprintf(stderr, "Error: cannot open %s\n", filename);
            return NULL;
        }
    }

    DebugPrintf("fp=[%x] stdin=[%x] stdout=[%x]\n", fp, stdin, stdout);

    return fp;
}

/*---------------------------------------------------------------------------*
  Name:         Fclose

  Description:  fclose �� stdin/stdout ���l����������
                NULL, stdin, stdout, stderr �Ȃ�N���[�Y���Ȃ�
  
  Arguments:    fp  �t�@�C���|�C���^
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void Fclose(FILE * fp)
{
    if (fp != NULL && fp != stdin && fp != stdout && fp != stderr)
    {
        fclose(fp);
    }
    return;
}

/*---------------------------------------------------------------------------*
  Name:         Fgets

  Description:  �t�@�C������s�ÂǂށB
                �ʏ�� fgets �ƈقȂ�A���I�ȃo�b�t�@�T�C�Y�̃R���g���[����
                ����Ċm���Ɉ�s����Ǎ���
  
  Arguments:    *buffer   �t�@�C���o�b�t�@�ւ̃|�C���^(�ւ̃|�C���^)
                *size     �t�@�C���o�b�t�@�̃T�C�Y(�ւ̃|�C���^)
                fp        �t�@�C��(=NULL �Ȃ� stdin ����ǂ�)
  
      buffer, size �� Fgets �����ɂ����ĕK�v�ɉ��� realloc ����܂��B
      ������ size ��^����̂� buffer ���ė��p���Aalloc/free ������
      �ߖ񂷂邽�߂ł��B
      �ŏ��� *buffer �� NULL �����ČĂяo�����Ƃ�z�肵�Ă��܂��B
  
  Returns:      ���������� *buffer ���A�t�@�C���̏I�[�ȂǂŎ��s������ NULL
 *---------------------------------------------------------------------------*/
char   *Fgets(char **pbuffer, int *pbuffer_size, FILE * fp)
{
    char   *buffer;
    int     buffer_size;
    int     buffer_gets;

    assert(pbuffer);
    assert(pbuffer_size);

    buffer = *pbuffer;
    buffer_size = *pbuffer_size;

    if (buffer == NULL || buffer_size == 0)
    {
        buffer_size = DEFAULT_LINE_BUFFER_SIZE;
        buffer = Calloc(buffer_size);
    }

    buffer_gets = 0;

    while (NULL != fgets(buffer + buffer_gets, buffer_size - buffer_gets, fp))
    {
        //
        //  �ǂݍ��񂾍s���s���ɒB���Ă���Ȃ�A��`�l��o�^����
        //    �s���̔���́Afgets �ɂ��ǂݍ��݂ɂ�����
        //      1) �o�b�t�@�̈�̍Ō�ڈ�t�܂Ńf�[�^���ǂݍ��܂�Ȃ������Ƃ�
        //      2) ��t�܂Ńf�[�^�͓ǂ܂ꂽ�������ōs�����x�I����Ă���Ƃ�
        //
        buffer_gets = strlen(buffer);
        if (buffer_gets < buffer_size - 1 || buffer[buffer_gets - 1] == '\n')
        {
            *pbuffer = buffer;
            *pbuffer_size = buffer_size;
            return buffer;
        }
        //
        //  �ǂݍ��񂾍s���s���ɂ܂ŒB���Ă��Ȃ��ꍇ�́A�o�b�t�@���Q�{�Ɋg����
        //  ������ǂݐi��
        //
        else
        {
            buffer_size *= 2;
            buffer = Realloc(buffer, buffer_size);
        }
    }

    // �t�@�C���I�����̏���
    free(*pbuffer);
    *pbuffer = NULL;
    *pbuffer_size = 0;
    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         UnpackFileName

  Description:  �t�@�C��������͂��p�[�c��
                (�f�B���N�g�����A�t�@�C���劲���A�g���q)�ɕ�������
  
  Arguments:    path   �t���p�X��
                dir    �f�B���N�g�� / �Ȃǂ��܂�
                base   �t�@�C���劲
                ext    �g���q '.' ���܂�
  
                * dir,base,ext �̓q�[�v����m�ۂ��Ă���̂� free ���K�v
                * dir,base,ext �� NULL �̂Ƃ��͒l�̑���͂��Ȃ�
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void UnpackFileName(const char *path, char **dir, char **base, char **ext)
{
    int     i, base_top, ext_top;
    int     path_len = strlen(path);

    // �ŏI�� '.' �̈ʒu�ƃp�X��؂�̈ʒu����������
    ext_top = path_len;
    base_top = 0;

    for (i = path_len - 1; i >= 0; i--)
    {
        if (path[i] == '.' && ext_top == path_len)
        {
            ext_top = i;
        }
        if (path[i] == '/' || path[i] == '\\' || path[i] == ':')
        {
            base_top = i + 1;
            break;
        }
    }

    // . �� .. �̏��� (���̏ꍇ base �� "." or "..", �g���q�͂Ȃ�)
    if (!strcmp(path + base_top, ".") || !strcmp(path + base_top, ".."))
    {
        ext_top = path_len;
    }

    // �������
    if (dir)
        *dir = StrNDup(path, base_top);
    if (base)
        *base = StrNDup(path + base_top, ext_top - base_top);
    if (ext)
        *ext = StrDup(path + ext_top);
    return;
}

/*---------------------------------------------------------------------------*
  Name:         SetDebugMode
  
  Description:  �f�o�b�O���[�h�̐ݒ�
  
  Arguments:    mode   ���[�h
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
static BOOL sDebugMode = FALSE;

void SetDebugMode(BOOL mode)
{
    sDebugMode = mode;
}

/*---------------------------------------------------------------------------*
  Name:         DebugPrintf
  
  Description:  �f�o�b�O�v�����g
  
  Arguments:    fmt   Printf �̃t�H�[�}�b�g
                ...   �v�����g����ϐ�
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void DebugPrintf(const char *fmt, ...)
{
    va_list va;

    if (sDebugMode)
    {
        va_start(va, fmt);
        vfprintf(stderr, fmt, va);
        va_end(va);
    }
}

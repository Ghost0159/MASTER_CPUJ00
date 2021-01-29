/*---------------------------------------------------------------------------*
  Project:  NitroSDK - bin2obj
  File:     section.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: section.c,v $
  Revision 1.5  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.4  2005/06/14 00:57:07  yasu
  -s �I�v�V�����̒ǉ�

  Revision 1.3  2005/06/13 08:52:26  yasu
  %f ���V���{�����ɑΉ�

  Revision 1.2  2005/06/13 02:56:34  yasu
  �I�v�V�����̏C���A�e�X�g�쐬�ɂ�铮��̊m�F

  Revision 1.1  2005/06/10 12:55:43  yasu
  1st ���t�@�N�^�����O

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "bin2obj.h"

static s32 read_datasec(Object * obj, const char *filename);
static u32 section_add_string(Section * s, const char *string);

/*---------------------------------------------------------------------------*
  Name:         add_datasec
  
  Description:  �f�[�^�Z�N�V������ǉ�����

  Arguments:    obj                   �I�u�W�F�N�g�ւ̃|�C���^
                section_rodata        �ǂݍ��ݐ�p�̃f�[�^�Z�N�V������
                section_rwdata        �ǂݏ����\�ȃf�[�^�Z�N�V������
                symbol_format_begin   �V���{����(%f �ϊ��O)-�擪
                symbol_format_end     �V���{����(%f �ϊ��O)-�Ō��
                filename              ���̓o�C�i���t�@�C����
                writable              TRUE .data  FALSE .rodata
                align                 �A���C�������g
  Returns:      TRUE ����  FALSE ���s
 *---------------------------------------------------------------------------*/
BOOL add_datasec(Object * obj,
                 const char *section_rodata, const char *section_rwdata,
                 const char *symbol_format_begin, const char *symbol_format_end,
                 const char *filename, BOOL writable, u32 align)
{
    s32     n;
    DataSection *d;
    char   *symbol_begin;              // �V���{����(%f �ϊ���)-�擪
    char   *symbol_end;                // �V���{����(%f �ϊ���)-�Ō��

    //
    //  �f�[�^�Z�N�V�����ǂݍ���
    //
    if (0 > (n = read_datasec(obj, filename)))
    {
        return FALSE;
    }

    //
    //  �f�[�^�Z�N�V�����̓o�^
    //
    d = &obj->data[n];
    if (writable)                      // .rodata or .data �̏���
    {
        d->index =
            add_section(obj, section_rwdata, SHT_PROGBITS, SHF_WRITE | SHF_ALLOC, d->section.size,
                        align);
    }
    else
    {
        d->index =
            add_section(obj, section_rodata, SHT_PROGBITS, SHF_ALLOC, d->section.size, align);
    }

    //
    //  �V���{���̓o�^
    //
    symbol_begin = create_symbol_string(filename, symbol_format_begin);
    symbol_end = create_symbol_string(filename, symbol_format_end);
    (void)add_symbol(obj, symbol_begin, 0, d->section.size, d->index);
    (void)add_symbol(obj, symbol_end, d->section.size, 0, d->index);
    free(symbol_begin);
    free(symbol_end);

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         read_datasec
  
  Description:  �f�[�^�Z�N�V������ǂݍ���

  Arguments:    obj        �I�u�W�F�N�g�ւ̃|�C���^
                filename   ���̓o�C�i���t�@�C����
  
  Returns:      TRUE ����  FALSE ���s
 *---------------------------------------------------------------------------*/
static s32 read_datasec(Object * obj, const char *filename)
{
    struct stat st;
    FILE   *fp;
    u32     n = obj->num_data;
    Section *s = &obj->data[n].section;

    //
    //  data section �̗̈���q�[�v������̓t�@�C���T�C�Y�������m�ۂ��A
    //  �����֓��̓t�@�C�� filename ��ǂݍ���
    //
    if (stat(filename, &st) || !S_ISREG(st.st_mode) || st.st_size < 0
        || NULL == (fp = fopen(filename, "rb")))
    {
        fprintf(stderr, "Error: Cannot open file %s.\n", filename);
        return -1;
    }

    s->size = st.st_size;

    if (NULL == (s->ptr = malloc(s->size)))
    {
        fprintf(stderr, "Error: No memory.\n");
        fclose(fp);
        return -1;
    }

    if (s->size != fread(s->ptr, sizeof(u8), s->size, fp))
    {
        fprintf(stderr, "Error: Cannot read file %s.\n", filename);
        free(s->ptr);
        fclose(fp);
        return -1;
    }
    fclose(fp);

    obj->num_data = n + 1;
    return n;
}

/*---------------------------------------------------------------------------*
  Name:         add_section_name

  Description:  �Z�N�V�������̒ǉ�

  Arguments:    obj      �I�u�W�F�N�g
                name     �Z�N�V������
  
  Returns:      �ǉ������Z�N�V�������̃Z�N�V�������e�[�u���ł̐擪�ʒu
 *---------------------------------------------------------------------------*/
u32 add_section_name(Object * obj, const char *name)
{
    // �Z�N�V�������ւ̓o�^
    u32     pos = section_add_string(&obj->section_name, name);

    // �Z�N�V�������(�T�C�Y)�̍X�V
    if (obj->header.e_shstrndx > 0)
    {
        obj->section[obj->header.e_shstrndx].sh_size = obj->section_name.size;
    }
    return pos;
}

/*---------------------------------------------------------------------------*
  Name:         add_section
  
  Description:  �Z�N�V������ǉ�

  Arguments:    obj     �I�u�W�F�N�g�ւ̃|�C���^
                name    �V���{����(����NULL�Ȃ� NULL SECTION �ɂ���)
                type    SHT_*
                flags   SHF_*
                size    �Z�N�V�����̃T�C�Y
                align   �Z�N�V�����̃A���C�������g
  
  Returns:      �o�^����index
 *---------------------------------------------------------------------------*/
u32 add_section(Object * obj, const char *name, u32 type, u32 flags, u32 size, u32 align)
{
    ELF32_SectionHeader *s;
    u32     n;

    n = obj->header.e_shnum;
    s = &obj->section[n];

    if (name)
    {
        s->sh_name = add_section_name(obj, name);
        s->sh_type = type;
        s->sh_flags = flags;
        s->sh_addr = 0;
        s->sh_offset = 0;
        s->sh_size = size;
        s->sh_link = 0;
        s->sh_info = 0;
        s->sh_addralign = align;
        s->sh_entsize = 0;
    }
    else
    {
        (void)add_section_name(obj, "");
        memset(s, 0, sizeof(ELF32_SectionHeader));
    }
    obj->header.e_shnum = n + 1;

    return n;
}

/*---------------------------------------------------------------------------*
  Name:         add_symbol_name

  Description:  �V���{�����̒ǉ�

  Arguments:    obj      �I�u�W�F�N�g
                name     �V���{����
  
  Returns:      �ǉ������V���{�����̃Z�N�V�������e�[�u���ł̐擪�ʒu
 *---------------------------------------------------------------------------*/
u32 add_symbol_name(Object * obj, const char *name)
{
    u32     pos;

    // �Z�N�V�������ւ̓o�^
    // name �� NULL �̏ꍇ�� "" �Ƃ��Ĉ�����
    pos = section_add_string(&obj->symbol_name, name ? name : "");

    // �Z�N�V�������(�T�C�Y)�̍X�V
    if (obj->symbol_name_index > 0)
    {
        obj->section[obj->symbol_name_index].sh_size = obj->symbol_name.size;
    }
    return pos;
}

/*---------------------------------------------------------------------------*
  Name:         add_symbol

  Description:  �V���{����ǉ�����

  Arguments:    obj		�I�u�W�F�N�g�ւ̃|�C���^
                symbol		�V���{����(����NULL�Ȃ� NULL SYMBOL �ɂ���)
                value		�V���{���̒l
                size		�V���{���̃T�C�Y
                section		�֘A�Z�N�V����
  
  Returns:      �o�^���� index
 *---------------------------------------------------------------------------*/
u32 add_symbol(Object * obj, const char *symbol, u32 value, u32 size, u32 section)
{
    ELF32_SectionHeader *symtab = &obj->section[obj->symbol_index];
    u32     n = symtab->sh_info + 1;
    ELF32_Symbol *l = &obj->symbol[n];

    if (obj->symbol_index > 0)
    {
        if (symbol)
        {
            // C ����Ȃǂ̃V���{���̏ꍇ�͒ʏ�̊֐����̑O�� '_' ������
            l->st_name = add_symbol_name(obj, symbol);
            l->st_value = value;
            l->st_size = size;
            l->st_info = ELF32_ST_INFO(STB_GLOBAL, STT_OBJECT);
            l->st_other = 0;
            l->st_shndx = section;
        }
        else
        {
            (void)add_symbol_name(obj, NULL);
            memset(l, 0, sizeof(ELF32_Symbol));
        }

        // �V���{���e�[�u���X�V
        symtab->sh_info = n;
        symtab->sh_size = (n + 1) * symtab->sh_entsize;
    }
    else
    {
        fprintf(stderr, "Warning: no symbol section. [%s] is skipped.\n", symbol);
    }
    return n;
}

/*---------------------------------------------------------------------------*
  Name:         section_add_string

  Description:  �Z�N�V�����ւ̕�����̒ǉ�

  Arguments:    section  �Z�N�V����
                string   ������
  
  Returns:      �ǉ�����������̃e�[�u���ł̐擪�ʒu(=�Ăяo���O�̃T�C�Y)
 *---------------------------------------------------------------------------*/
static u32 section_add_string(Section * s, const char *string)
{
    int     new_size = s->size + strlen(string) + 1;
    u8     *new_ptr;
    int     size;

    if (NULL == (new_ptr = malloc(new_size)))
    {
        fprintf(stderr, "Error: No memory.\n");
        exit(1);
    }
    memcpy(new_ptr, s->ptr, s->size);
    strcpy(new_ptr + s->size, string);
    free(s->ptr);

    size = s->size;
    s->ptr = new_ptr;
    s->size = new_size;

    return size;
}

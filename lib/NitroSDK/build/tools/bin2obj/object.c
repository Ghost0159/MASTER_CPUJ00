/*---------------------------------------------------------------------------*
  Project:  NitroSDK - bin2obj
  File:     object.c

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: object.c,v $
  Revision 1.4  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.3  2006/07/21 07:57:19  yasu
  PowerPC �Ή�

  Revision 1.2  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.1  2005/06/10 12:55:43  yasu
  1st ���t�@�N�^�����O

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "bin2obj.h"

static void header_init(ELF32_ElfHeader * h, u16 machine, u8 endian);
/*---------------------------------------------------------------------------*
  Name:         object_init

  Description:  �I�u�W�F�N�g��������������

  Arguments:    obj    �I�u�W�F�N�g
  
  Returns:      TRUE ����  FALSE ���s
 *---------------------------------------------------------------------------*/
void object_init(Object * obj, u16 machine, u8 endian)
{
    ELF32_SectionHeader *s;
    ELF32_ElfHeader *h;

    //-----------------------------------------------------------------------
    // �̈� ZERO ������
    //-----------------------------------------------------------------------
    memset(obj, 0, sizeof(Object));

    //-----------------------------------------------------------------------
    // Elf Header �̏�����
    //-----------------------------------------------------------------------
    h = &obj->header;
    header_init(h, machine, endian);

    //-----------------------------------------------------------------------
    // �f�t�H���g�Z�N�V�����̓o�^
    //-----------------------------------------------------------------------

    // index=0 �̃Z�N�V�����̓_�~�[=NULL
    (void)add_section(obj, NULL, SHT_NULL, 0, 0, 0);

    // index=1 �Z�N�V�������e�[�u��
    h->e_shstrndx = add_section(obj, ".shstrtab", SHT_STRTAB, SHF_NULL, obj->section_name.size, 0);

    // index=2 �V���{�����\����
    obj->symbol_index = add_section(obj, ".symtab", SHT_SYMTAB, SHF_NULL, 0, 4);

    // index=3 �V���{�����e�[�u��
    obj->symbol_name_index =
        add_section(obj, ".strtab", SHT_STRTAB, SHF_NULL, obj->symbol_name.size, 1);

    // �V���{�����̐ݒ�ǉ�
    s = &obj->section[obj->symbol_index];
    s->sh_link = obj->symbol_name_index;        // symbol ���e�[�u���̃����N
    s->sh_info = -1;                   // symbol �̍ŏI index �l������
    s->sh_entsize = sizeof(ELF32_Symbol);       // symbol �G���g���̃T�C�Y

    //-----------------------------------------------------------------------
    // �V���{���e�[�u���̏�����
    //-----------------------------------------------------------------------

    // index=0 NULL �V���{����o�^
    (void)add_symbol(obj, NULL, 0, 0, 0);
}

/*---------------------------------------------------------------------------*
  Name:         header_init

  Description:  ELF �w�b�_�����쐬����

  Arguments:    elfHeader  ELF �w�b�_���
                machine    Machine �R�[�h
                              EM_ARM // ARM
                              EM_PPC // PPC
                endian     ELF �̃G���f�B�A��
                              ELFDATA2LSB // Little Endian
                              ELFDATA2MSB // Big    Endian
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
static void header_init(ELF32_ElfHeader * h, u16 machine, u8 endian)
{
    memset(h, 0, sizeof(ELF32_ElfHeader));
    h->e_ident[0] = ELFMAG0;           // 0x7f
    h->e_ident[1] = ELFMAG1;           // 'E'
    h->e_ident[2] = ELFMAG2;           // 'L'
    h->e_ident[3] = ELFMAG3;           // 'F'
    h->e_ident[4] = ELFCLASS32;        // 32bit
    h->e_ident[5] = endian;            // ELFDATA2MSB/ELFDATA2LSB
    h->e_ident[6] = EV_CURRENT;        // CURRENT
    h->e_type = ET_REL;
    h->e_machine = machine;
    h->e_version = EV_CURRENT;
    h->e_entry = 0;
    h->e_phoff = 0;
    h->e_shoff = 0;                    // ���ƂŐ������l�𖄂߂�
    h->e_flags = 0;
    h->e_ehsize = sizeof(ELF32_ElfHeader);
    h->e_phentsize = 0;
    h->e_phnum = 0;
    h->e_shentsize = sizeof(ELF32_SectionHeader);
    h->e_shnum = 0;                    // �Z�N�V������������x�� +1
    h->e_shstrndx = 0;                 // ���ƂŐ������l�𖄂߂�

    return;
}

/*---------------------------------------------------------------------------*
  Name:         map_section

  Description:  �Z�N�V�����̈ʒu������������

  Arguments:    obj    �I�u�W�F�N�g
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void map_section(Object * obj)
{
    ELF32_ElfHeader *h;
    ELF32_SectionHeader *s;
    u32     offset;
    int     i;

    // ELF HEADER/SECTION HEADER �̈ʒu�v�Z
    h = &obj->header;
    offset = h->e_ehsize;
    h->e_shoff = roundup(offset, 4);
    offset = h->e_shoff + h->e_shentsize * h->e_shnum;

    // �Z�N�V�����̈ʒu�v�Z
    for (i = 1; i < obj->header.e_shnum; i++)
    {
        s = &obj->section[i];
        s->sh_offset = roundup(offset, s->sh_addralign);
        offset = s->sh_offset + s->sh_size;
    }
    return;
}

/*---------------------------------------------------------------------------*
  Name:         roundup

  Description:  �����̐؂�グ

  Arguments:    val       �l
                align     ���E�l
  
  Returns:      ����
 *---------------------------------------------------------------------------*/
u32 roundup(u32 val, u32 align)
{
    if (align > 1)
    {
        u32     n = val % align;

        if (n > 0)
        {
            val += (align - n);
        }
    }
    return val;
}


/*---------------------------------------------------------------------------*
  Name:         conv_to_big_endian

  Description:  BinEndian �ւ̕ϊ�

  Arguments:    src  �ϊ��O
                dest �ϊ���
  
  Returns:      ����
 *---------------------------------------------------------------------------*/
static u32 Be32(u32 x)
{
    return (u32)(((x) >> 24) & 0x000000ff) | (((x) >> 8) & 0x0000ff00) |
        (((x) << 8) & 0x00ff0000) | (((x) << 24) & 0xff000000);
}

static u16 Be16(u16 x)
{
    return (u16)(((x) >> 8) & 0x00ff) | (((x) << 8) & 0xff00);
}

static void conv_to_big_endian_header(const ELF32_ElfHeader * src, ELF32_ElfHeader * dest)
{
    memcpy(dest->e_ident, src->e_ident, EI_NIDENT);
    dest->e_type = Be16(src->e_type);
    dest->e_machine = Be16(src->e_machine);
    dest->e_version = Be32(src->e_version);
    dest->e_entry = Be32(src->e_entry);
    dest->e_phoff = Be32(src->e_phoff);
    dest->e_shoff = Be32(src->e_shoff);
    dest->e_flags = Be32(src->e_flags);
    dest->e_ehsize = Be16(src->e_ehsize);
    dest->e_phentsize = Be16(src->e_phentsize);
    dest->e_phnum = Be16(src->e_phnum);
    dest->e_shentsize = Be16(src->e_shentsize);
    dest->e_shnum = Be16(src->e_shnum);
    dest->e_shstrndx = Be16(src->e_shstrndx);
}

static void conv_to_big_endian_section(const ELF32_SectionHeader * src, ELF32_SectionHeader * dest)
{
    dest->sh_name = Be32(src->sh_name);
    dest->sh_type = Be32(src->sh_type);
    dest->sh_flags = Be32(src->sh_flags);
    dest->sh_addr = Be32(src->sh_addr);
    dest->sh_offset = Be32(src->sh_offset);
    dest->sh_size = Be32(src->sh_size);
    dest->sh_link = Be32(src->sh_link);
    dest->sh_info = Be32(src->sh_info);
    dest->sh_addralign = Be32(src->sh_addralign);
    dest->sh_entsize = Be32(src->sh_entsize);
}

static void conv_to_big_endian_symbol(const ELF32_Symbol * src, ELF32_Symbol * dest)
{
    dest->st_name = Be32(src->st_name);
    dest->st_value = Be32(src->st_value);
    dest->st_size = Be32(src->st_size);
    dest->st_info = src->st_info;
    dest->st_other = src->st_other;
    dest->st_shndx = Be16(src->st_shndx);
}

void conv_to_big_endian(const Object * src, Object * dest)
{
    int     i, n;

    conv_to_big_endian_header(&src->header, &dest->header);

    for (i = 0; i < src->header.e_shnum; i++)
    {
        conv_to_big_endian_section(&src->section[i], &dest->section[i]);
    }

    n = src->section[src->symbol_index].sh_size / sizeof(src->symbol[0]);
    for (i = 0; i < n; i++)
    {
        conv_to_big_endian_symbol(&src->symbol[i], &dest->symbol[i]);
    }
}

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_fileinfo.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_fileinfo.c,v $
  Revision 1.36  2007/07/31 00:58:42  yosizaki
  update copyright

  Revision 1.35  2007/07/30 03:32:38  yosizaki
  fix about MB_RegisterFile

  Revision 1.34  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.33  2005/11/07 01:21:50  okubata_ryoma
  SDK_STATIC_ASSERT����SDK_COMPILER_ASSERT�ɕύX

  Revision 1.32  2005/05/20 03:10:01  yosizaki
  support MBGameRegistry::userParam.

  Revision 1.31  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.30  2005/02/21 00:39:34  yosizaki
  replace prefix MBw to MBi.

  Revision 1.29  2005/02/18 11:56:03  yosizaki
  fix around hidden warnings.

  Revision 1.28  2004/12/07 10:20:37  yosizaki
  fix to reload cache from correct archive. (not only "rom")

  Revision 1.27  2004/11/26 09:16:42  yosizaki
  fix MB_ReadSegment (around Debugger's patch-code)

  Revision 1.26  2004/11/22 07:39:07  yosizaki
  fix MB_ReadSegment (around auth-code)

  Revision 1.25  2004/11/11 11:49:06  yosizaki
  fix MB_ReadSegment around cache-mode and auth-code.
  add task-thread system.

  Revision 1.24  2004/11/10 13:16:27  yosizaki
  change MB_ReadSegment to use cache-system.

  Revision 1.23  2004/10/06 05:47:58  sato_masaki
  small fix

  Revision 1.22  2004/09/29 07:15:56  yosizaki
  fix MB_ReadSegment. (add size of auth-code)

  Revision 1.21  2004/09/28 00:13:18  sato_masaki
  bug fix

  Revision 1.20  2004/09/27 11:24:02  sato_masaki
  - IsAbleToLoad() �֐��ǉ��B
  - MB_RegisterFile�ɂāABlock���e�[�u���쐬�Ɏ��s���Ă��AGameInfo��Beacon�ɓo�^����Ă����̂��C���B

  Revision 1.19  2004/09/25 12:11:53  sato_masaki
  MBi_IsAbleToRecv()��ǉ��B(IsAbleToRecv()�����l�[�����Amb_child.c����ړ��B)

  Revision 1.18  2004/09/25 05:30:32  sato_masaki
  ���C�����[�v�ɂ����ČĂ΂꓾��API���s���ɁA���荞�݋֎~�����鏈����ǉ��B

  Revision 1.17  2004/09/22 09:41:50  sato_masaki
  MBi_get_blockinfo()�ɁA��������ǉ��B

  Revision 1.16  2004/09/20 13:12:20  sato_masaki
  MBi_get_blockinfo()�̕Ԃ�l��BOOL�l�ɕύX�B

  Revision 1.15  2004/09/17 11:37:17  sato_masaki
  MB_RegisterFile() �ɂ����āAMB���J�n����Ă��Ȃ�������AFALSE��Ԃ��悤�ɂ����B

  Revision 1.14  2004/09/16 12:50:37  sato_masaki
  - mb.h�ɂāAMBi_***�Œ�`���Ă����֐��A�^���AMB_***�ɕύX�B
  - MB_UnregisterFile�̕Ԃ�l���A�o�^�����Z�O�����g�o�b�t�@�ւ̃|�C���^�ɕύX�B

  Revision 1.13  2004/09/16 08:44:21  miya
  �t�@�C���X�V�p�����o�ǉ�

  Revision 1.12  2004/09/15 06:51:59  sato_masaki
  WBT��`�ɓZ��鏈�����폜�B

  Revision 1.11  2004/09/15 06:25:29  sato_masaki
  MB_COMM_P_SENDLEN, MB_COMM_C_SENDLEN �̒�`�Ɉˑ����Ă���������ϐ����B

  Revision 1.10  2004/09/14 15:38:45  yosizaki
  fix MB_ReadSegment

  Revision 1.9  2004/09/11 11:20:48  sato_masaki
  change mbc to pPwork

  Revision 1.8  2004/09/10 03:24:14  yosizaki
  unify file-access to register file.

  Revision 1.7  2004/09/08 11:09:46  miya
  small fix.

  Revision 1.6  2004/09/07 04:38:17  sato_masaki
  - const�l�AdefaultLoadSegList��MBi_defaultLoadSegList�Ƀ��l�[�����A��static���B
  - MB_BlockInfo�̗v�f�ǉ��ɑΉ��B

  Revision 1.5  2004/09/06 11:51:52  sato_masaki
  GameInfo�ւ�fileNo�̐ݒ���AMB_RegisterFile()���ōs���悤�ɕύX�B

  Revision 1.4  2004/09/06 06:23:03  yosizaki
  fix around MBi_ReadSegmentHeader().

  Revision 1.3  2004/09/06 03:29:21  yosizaki
  -add routine about reading segment header.
  -change MB_RegisterFile() in order to send own program too.

  Revision 1.2  2004/09/04 11:03:18  sato_masaki
  ROM_HEADER_SIZE_FULL�̒�`��mb_fileinfo.���ֈړ��B

  Revision 1.1  2004/09/03 07:04:36  sato_masaki
  �t�@�C�����@�\�ʂɕ����B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include "mb_private.h"

// definition

/*
 * �L���b�V���y�[�W�P��.
 * �ŏ��͐e�@�w�b�_������ 16kB �ɂ��Ă�����,
 * ���E���܂������ꍇ�ɖ�肪����̂� 17kB �ɑ���.
 */
#define	MB_CACHE_PAGE_SIZE	(17 * 1024)

/* MB_ReadSegment() ���v�o�b�t�@��`�l�̐��������� */
SDK_COMPILER_ASSERT(ROM_HEADER_SIZE_FULL +
                    MB_AUTHCODE_SIZE +
                    sizeof(MBiCacheList) +
                    sizeof(u32) * 4 + MB_CACHE_PAGE_SIZE * 3 <= MB_SEGMENT_BUFFER_MIN);

// constant variables

/* 
    �f�t�H���g�̃Z�O�����g���X�g
    �Z�O�����g�̃^�C�v����ъi�[���́A���̌`�ŌŒ�Ƃ���B
 */
const MbSegmentType MBi_defaultLoadSegList[MB_DL_SEGMENT_NUM] = {
    MB_SEG_ROMHEADER,                  /* ROM HEADER  */
    MB_SEG_ARM9STATIC,                 /* ARM9 static */
    MB_SEG_ARM7STATIC,                 /* ARM7 static */
};


/* �Z�O�����g�̃w�b�_�̈� */
#define	MB_SEGMENT_HEADER_MIN	0x4000
#define	MB_SEGMENT_HEADER_MAX	0x8000


/* �L���ȃZ�O�����g�w�b�_�̈�. (�����ςɂ���ꍇ�͐ݒ�֐���p��) */
static const CARDRomRegion mbi_seg_header_default[] = {
    {0x4000, 0x1000,},
    {0x7000, 0x1000,},
    {0, 0},
};
static const CARDRomRegion *mbi_seg_header = mbi_seg_header_default;

/* �Z�O�����g�̃w�b�_�������[�h����ۂɕK�v�ȏ�� */
typedef struct
{
    u32     rom_src;
    u32     mem_src;
    u32     mem_dst;
    u32     len;
}
MBiSegmentHeaderInfo;


// ----------------------------------------------------------------------------
// static funciton's prototypes

static void MBi_MakeDownloadFileInfo(MbDownloadFileInfoHeader * mbdlFileInfop, const void *buf);
static void MBi_SetSegmentInfo(const RomHeader * mbRomHeaderp, const MbSegmentType * loadSegListp,
                               MbSegmentInfo * seg_infop, u32 *child_recv_buff_addr);
static void MBi_ReadSegmentHeader(const MBiSegmentHeaderInfo * p, u32 top, u32 bottom, BOOL clean);
static BOOL IsAbleToLoad(u8 segment_no, u32 address, u32 size);

/*---------------------------------------------------------------------------*
  Name:         MB_GetSegmentLength

  Description:  �w�肳�ꂽ�o�C�i���t�@�C���ɕK�v�ȃZ�O�����g�o�b�t�@�����擾

  Arguments:    file : �o�C�i���t�@�C�����w�� FSFile �\����
                       NULL �̏ꍇ, ���g���g�p����. (�e�q���p�o�C�i��)
  
  Returns:      ����ɃT�C�Y���擾�ł���ΐ��̒l��, �����łȂ���� 0
 *---------------------------------------------------------------------------*/

u32 MB_GetSegmentLength(FSFile *file)
{
    enum
    { ROM_HEADER_SIZE_SMALL = 0x60 };
    u8      rom[ROM_HEADER_SIZE_SMALL];
    const RomHeader *p = NULL;

    u32     ret = 0;

    SDK_ASSERT(!file || FS_IsFile(file));

    /* �P�̂̃o�C�i���t�@�C�����w�� */
    if (file)
    {
        /* ���݂̈ʒu���L�� */
        const u32 bak_pos = FS_GetPosition(file);
        /* �Ƃ肠���� ROM �w�b�_���ǂݍ��� */
        if (FS_ReadFile(file, &rom, sizeof(rom)) >= sizeof(rom))
        {
            p = (const RomHeader *)rom;
        }
        /* �ŏ��̈ʒu�𕜋A */
        (void)FS_SeekFile(file, (int)bak_pos, FS_SEEK_SET);
    }
    /* �e�q���p�o�C�i�� */
    else
    {
        p = (const RomHeader *)HW_ROM_HEADER_BUF;
    }
    if (p)
    {
        ret = (u32)(ROM_HEADER_SIZE_FULL +      /* ROM �w�b�_ */
                    MB_AUTHCODE_SIZE + /* auth �R�[�h */
                    sizeof(MBiCacheList) +      /* �L���b�V���\���� */
                    sizeof(u32) * 4 +  /* segment - CARD �}�b�s���O */
                    p->main_size +     /* ARM9 static �Z�O�����g */
                    p->sub_size        /* ARM7 static �Z�O�����g */
            );
        if (ret < MB_SEGMENT_BUFFER_MIN)
            ret = MB_SEGMENT_BUFFER_MIN;
    }
    return ret;
}

/*---------------------------------------------------------------------------*
  Name:         MB_ReadSegment

  Description:  �w�肳�ꂽ�o�C�i���t�@�C������K�v�ȃZ�O�����g�f�[�^��ǂݏo��.

  Arguments:    file : �o�C�i���t�@�C�����w�� FSFile �\����
                       NULL �̏ꍇ, ���g���g�p����. (�e�q���p�o�C�i��)
                buf : �Z�O�����g�f�[�^��ǂݏo���o�b�t�@
				len : buf �̃T�C�Y
  
  Returns:      ����ɃZ�O�����g�f�[�^��ǂݏo����� TRUE, �����łȂ���� FALSE
 *---------------------------------------------------------------------------*/

BOOL MB_ReadSegment(FSFile *file, void *buf, u32 len)
{
    BOOL    ret = FALSE;
    SDK_ASSERT(!file || FS_IsFile(file));

    /*
     * �w�肳�ꂽ�o�b�t�@���ȉ��̃t�H�[�}�b�g�Ő؂�o��.
     *
     * [RomHeader]
     * [AuthCode]
     * [CacheList]
     * [CardMapping]
     * [ARM9 static]
     * [ARM7 static]
     *
     * �Ō�� 2 �̃Z�O�����g��, �T�C�Y���s�����Ă����
     * �L���b�V�������ŉ��z���������������đΉ�.
     */

    if (len >= ROM_HEADER_SIZE_FULL + 4)
    {

        BOOL    is_own_binary = FALSE;

        u32     rest = len;
        u8     *dst = (u8 *)buf;
        u32     bak_pos;
        FSFile  own_bin[1];
        u32     own_size;
        RomHeader *p_rom;

        MBiCacheList *p_cache = NULL;
        u32    *p_map = NULL;

        p_rom = (RomHeader *) dst;
        dst += ROM_HEADER_SIZE_FULL, rest -= ROM_HEADER_SIZE_FULL;

        /* �ʏ�̃}���`�u�[�g */
        if (file)
        {
            bak_pos = FS_GetPosition(file);
            if (FS_ReadFile(file, p_rom, ROM_HEADER_SIZE_FULL) < ROM_HEADER_SIZE_FULL)
                rest = 0;
            own_size = p_rom->own_size;
            if (!own_size)
                own_size = 16 * 1024 * 1024;
        }
        /* �N���[���u�[�g */
        else
        {
            const RomHeader *mem_rom = (RomHeader *) HW_ROM_HEADER_BUF;
            own_size = mem_rom->own_size;
            if (!own_size)
                own_size = 16 * 1024 * 1024;
            is_own_binary = TRUE;
            /* ���g��\���t�@�C���𒼐ڊJ�� */
            FS_InitFile(own_bin);
            (void)FS_OpenFileDirect(own_bin,
                                    FS_FindArchive("rom", 3), 0, (u32)(own_size + MB_AUTHCODE_SIZE),
                                    (u32)~0);
            file = own_bin;
            bak_pos = FS_GetPosition(file);
            /* �Ƃ肠���� ROM �w�b�_���R�s�[ */
            MI_CpuCopy8(mem_rom, p_rom, ROM_HEADER_SIZE_FULL);
            /* �ϊ����ꂽ������� */
            *(u32 *)((u32)p_rom + 0x60) |= 0x00406000;
        }

        /* MB_AUTHCODE_SIZE�� �̃o�C�i���l��ǂݍ��� */
        if (rest >= MB_AUTHCODE_SIZE)
        {
            (void)FS_SeekFile(file, (int)(bak_pos + own_size), FS_SEEK_SET);
            (void)FS_ReadFile(file, dst, MB_AUTHCODE_SIZE);
            dst += MB_AUTHCODE_SIZE, rest -= MB_AUTHCODE_SIZE;
        }
        else
        {
            rest = 0;
        }

        /* �L���b�V�����X�g�\���̂��m�� */
        if (rest >= sizeof(MBiCacheList))
        {
            p_cache = (MBiCacheList *) dst;
            MBi_InitCache(p_cache);
            dst += sizeof(MBiCacheList), rest -= sizeof(MBiCacheList);
            /* ROM �w�b�_���ŏ��̃L���b�V���ɓo�^ */
            MBi_AttachCacheBuffer(p_cache, 0, ROM_HEADER_SIZE_FULL, p_rom, MB_CACHE_STATE_LOCKED);
            /* �ΏۃA�[�J�C�u�����L��. (�x�����[�h���Ɏw��) */
            {
                FSArchive *p_arc = FS_GetAttachedArchive(file);
                int     i = 0;
                while ((i < FS_ARCHIVE_NAME_LEN_MAX) && p_arc->name.ptr[i])
                    ++i;
                MI_CpuCopy8(p_arc->name.ptr, p_cache->arc_name, (u32)i);
                p_cache->arc_name_len = (u32)i;
                p_cache->arc_pointer = p_arc;
            }
        }
        else
        {
            rest = 0;
        }

        /* segment-CARD �}�b�s���O�̈���m�� */
        if (rest >= sizeof(u32) * 4)
        {
            p_map = (u32 *)dst;
            p_map[0] = 0;              /* ROM �w�b�_ */
            p_map[1] = FS_GetFileImageTop(file) + bak_pos + p_rom->main_rom_offset;     /* ARM9 static */
            p_map[2] = FS_GetFileImageTop(file) + bak_pos + p_rom->sub_rom_offset;      /* ARM7 static */
            dst += sizeof(u32) * 4, rest -= sizeof(u32) * 4;
        }
        else
        {
            rest = 0;
        }

        /* �e�Z�O�����g�ǂݍ��� */
        if (rest >= p_rom->main_size + p_rom->sub_size)
        {
            /* �[���ɑS�Ĉꊇ���[�h�ł���]�T������ꍇ */
            const u32 base = FS_GetFileImageTop(file);
            (void)FS_SeekFile(file, (int)(p_map[1] - base), FS_SEEK_SET);
            (void)FS_ReadFile(file, dst, (int)p_rom->main_size);
            MBi_AttachCacheBuffer(p_cache, p_map[1], p_rom->main_size, dst, MB_CACHE_STATE_LOCKED);
            dst += p_rom->main_size, rest -= p_rom->main_size;
            (void)FS_SeekFile(file, (int)(p_map[2] - base), FS_SEEK_SET);
            (void)FS_ReadFile(file, dst, (int)p_rom->sub_size);
            MBi_AttachCacheBuffer(p_cache, p_map[2], p_rom->sub_size, dst, MB_CACHE_STATE_LOCKED);
            dst += p_rom->sub_size, rest -= p_rom->sub_size;
            /* �������� */
            ret = TRUE;
        }
        else if (rest >= MB_CACHE_PAGE_SIZE * 3)
        {
            /* 3 ���̃L���b�V�������őΉ��ł��镪�̗]�T������ꍇ */
            const u32 base = FS_GetFileImageTop(file);
            u32     offset = p_map[1];
            /* 1 ���� (�Œ�) */
            (void)FS_SeekFile(file, (int)(offset - base), FS_SEEK_SET);
            (void)FS_ReadFile(file, dst, MB_CACHE_PAGE_SIZE);
            MBi_AttachCacheBuffer(p_cache, offset, MB_CACHE_PAGE_SIZE, dst, MB_CACHE_STATE_LOCKED);
            dst += MB_CACHE_PAGE_SIZE, rest -= MB_CACHE_PAGE_SIZE;
            offset += MB_CACHE_PAGE_SIZE;
            /* 2 ���� */
            (void)FS_SeekFile(file, (int)(offset - base), FS_SEEK_SET);
            (void)FS_ReadFile(file, dst, MB_CACHE_PAGE_SIZE);
            MBi_AttachCacheBuffer(p_cache, offset, MB_CACHE_PAGE_SIZE, dst, MB_CACHE_STATE_READY);
            dst += MB_CACHE_PAGE_SIZE, rest -= MB_CACHE_PAGE_SIZE;
            offset += MB_CACHE_PAGE_SIZE;
            /* 3 ���� */
            (void)FS_SeekFile(file, (int)(offset - base), FS_SEEK_SET);
            (void)FS_ReadFile(file, dst, MB_CACHE_PAGE_SIZE);
            MBi_AttachCacheBuffer(p_cache, offset, MB_CACHE_PAGE_SIZE, dst, MB_CACHE_STATE_READY);
            dst += MB_CACHE_PAGE_SIZE, rest -= MB_CACHE_PAGE_SIZE;
            /* �������� */
            ret = TRUE;
        }

        /* �ŏ��̈ʒu�𕜋A */
        (void)FS_SeekFile(file, (int)bak_pos, FS_SEEK_SET);

        /* �N���[���u�[�g */
        if (is_own_binary)
        {
            (void)FS_CloseFile(own_bin);
            if (ret)
            {
                const CARDRomRegion *p_header = mbi_seg_header;
                MBiSegmentHeaderInfo info;
                info.rom_src = p_rom->main_rom_offset;
                info.mem_src = (u32)((u32)p_rom->main_ram_address - p_rom->main_rom_offset);
                info.mem_dst = (u32)((u32)p_cache->list[1].ptr - p_rom->main_rom_offset);
                info.len = len;
                /* �ŏ��ɊY���̈�������� */
                MBi_ReadSegmentHeader(&info, MB_SEGMENT_HEADER_MIN, MB_SEGMENT_HEADER_MAX, TRUE);
                /* �w�b�_�������]�� */
                for (; p_header->length != 0; ++p_header)
                {
                    MBi_ReadSegmentHeader(&info,
                                          p_header->offset,
                                          (u32)(p_header->offset + p_header->length), FALSE);
                }
                {
                    /* �f�o�b�K�� autoload �p�b�`�����𕜌� */
                    extern u32 _start_AutoloadDoneCallback[1];
                    u8     *dst = (u8 *)p_cache->list[1].ptr;
                    dst += ((u32)&_start_AutoloadDoneCallback - (u32)p_rom->main_ram_address);
                    *(u32 *)dst = 0xE12FFF1E;   /* asm { bx, lr } (always code32) */
                }
            }
        }

        if (ret)
        {
            DC_FlushRange(buf, len);
        }
    }

    return ret;
}

/*
 * �Z�O�����g�̃w�b�_����ǂݍ���.
 */
static void MBi_ReadSegmentHeader(const MBiSegmentHeaderInfo * p, u32 top, u32 bottom, BOOL clean)
{
    /* �w�b�_�͈͂̕␳ */
    if (top < MB_SEGMENT_HEADER_MIN)
        top = MB_SEGMENT_HEADER_MIN;
    if (bottom > MB_SEGMENT_HEADER_MAX)
        bottom = MB_SEGMENT_HEADER_MAX;
    /* ���]���̈�̕␳ */
    if (top < p->rom_src)
        top = p->rom_src;
    if (bottom > p->rom_src + p->len)
        bottom = p->rom_src + p->len;
    /* �̈悪�L���Ȃ�]�� */
    if (top < bottom)
    {
        if (clean)
        {                              /* 0 ������ */
            MI_CpuClear8((void *)(p->mem_dst + top), (u32)(bottom - top));
        }
        else
        {                              /* �]�� */
            MI_CpuCopy8((const void *)(p->mem_src + top),
                        (void *)(p->mem_dst + top), (u32)(bottom - top));
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         MB_RegisterFile

  Description:  �e�@�ɂ�Download���̃t�@�C����o�^����

  Arguments:    game_reg : �Q�[���o�^���
                buf : ���ł� MB_ReadSegment �Ń��[�h�ς݂̃Z�O�����g�o�b�t�@
  
  Returns:      
 *---------------------------------------------------------------------------*/

BOOL MB_RegisterFile(const MBGameRegistry *game_reg, const void *buf)
{
    MbDownloadFileInfoHeader *mdfi;
    static u8 update = 0;
    u8     *appname = (u8 *)game_reg->gameNamep;
    u8      fileID = 0xff, i;

    OSIntrMode enabled = OS_DisableInterrupts();        /* ���荞�݋֎~ */

    /* MB���J�n����Ă��Ȃ�������AFALSE��Ԃ��D */
    if (!MBi_IsStarted())
    {
        OS_TWarning("MB has not started yet. Cannot Register File\n");
        (void)OS_RestoreInterrupts(enabled);    /* ���荞�݋֎~���� */
        return FALSE;
    }

    if (pPwork->file_num + 1 > MB_MAX_FILE)
    {
        OS_TWarning("The number of registered files has exceeded.\n");
        (void)OS_RestoreInterrupts(enabled);    /* ���荞�݋֎~���� */
        return FALSE;
    }

    /* ��fileinfo��T�� */
    for (i = 0; i < MB_MAX_FILE; i++)
    {
        /* ����GameRegistry�����ɓo�^����Ă�����AFALSE��Ԃ� */
        if (pPwork->fileinfo[i].game_reg == (MBGameRegistry *)game_reg)
        {
            MB_DEBUG_OUTPUT("Already Registered \"%s\"\n", game_reg->gameNamep);
            (void)OS_RestoreInterrupts(enabled);        /* ���荞�݋֎~���� */
            return FALSE;
        }

        if (pPwork->fileinfo[i].active == 0)
        {
            fileID = i;
            break;
        }
    }

    /* ��fileinfo��������Ȃ�������A�ُ�I�� */
    if (i == MB_MAX_FILE)
    {
        OS_TWarning("Too many Files! \"%s\"\n", game_reg->gameNamep);
        (void)OS_RestoreInterrupts(enabled);    /* ���荞�݋֎~���� */
        return FALSE;
    }


    // GameRegistry�̃|�C���^��o�^
    pPwork->fileinfo[fileID].game_reg = (MBGameRegistry *)game_reg;

    // DownloadFileInfo�ւ̃|�C���^�Z�b�g
    mdfi = &pPwork->fileinfo[fileID].dl_fileinfo.header;

    // MBDownloadFileInfo �̐���
    MBi_MakeDownloadFileInfo(mdfi, buf);
    /* ���[�U��`�g���p�����[�^�̃R�s�[ */
    MI_CpuCopy8(game_reg->userParam,
                ((MBDownloadFileInfo *) mdfi)->reserved, HW_DOWNLOAD_PARAMETER_SIZE);

    // Block���e�[�u���̐���
    if (FALSE == MBi_MakeBlockInfoTable(&pPwork->fileinfo[fileID].blockinfo_table, mdfi))
    {
        OS_TWarning("Cannot make block information!\n");
        (void)OS_RestoreInterrupts(enabled);    /* ���荞�݋֎~���� */
        return FALSE;
    }

    // MbGameInfo�̐���
    MBi_MakeGameInfo(&pPwork->fileinfo[fileID].game_info, game_reg, &pPwork->common.user);

    /* �t�@�C���ԍ���GameInfo�ɃZ�b�g */
    pPwork->fileinfo[fileID].game_info.fileNo = fileID;

    /* GameInfo ��Beacon���X�g�ɓo�^����B */
    MB_AddGameInfo(&pPwork->fileinfo[fileID].game_info);

    /* �t�@�C���X�V�ԍ���GameInfo�ɃZ�b�g */
    pPwork->fileinfo[fileID].game_info.seqNoFixed = update++;

    pPwork->fileinfo[fileID].gameinfo_child_bmp = MB_GAMEINFO_PARENT_FLAG;

    pPwork->fileinfo[fileID].src_adr = (u32 *)buf;

    /* �L���b�V���p�̊e��|�C���^��ۑ� */
    pPwork->fileinfo[fileID].cache_list = (MBiCacheList *)
        ((u32)buf + ROM_HEADER_SIZE_FULL + MB_AUTHCODE_SIZE);
    pPwork->fileinfo[fileID].card_mapping = (u32 *)
        ((u32)buf + ROM_HEADER_SIZE_FULL + MB_AUTHCODE_SIZE + sizeof(MBiCacheList));

    /* �^�X�N�X���b�h���K�v�ɂȂ�����N�� */
    if (pPwork->fileinfo[fileID].cache_list->list[3].state != MB_CACHE_STATE_EMPTY)
    {
        if (!MBi_IsTaskAvailable())
        {
            MBi_InitTaskInfo(&pPwork->cur_task);
            MBi_InitTaskThread(pPwork->task_work, sizeof(pPwork->task_work));
        }
    }

    /* �o�^�t�@�C�����A�N�e�B�u�ɂ���B */
    pPwork->fileinfo[fileID].active = 1;

    // file���̃C���N�������g
    pPwork->file_num++;

    MB_DEBUG_OUTPUT("Register Game \"%s\"\n", game_reg->gameNamep);

    (void)OS_RestoreInterrupts(enabled);        /* ���荞�݋֎~���� */

    return TRUE;

}

/*---------------------------------------------------------------------------*
  Name:         MB_UnregisterFile

  Description:  �w�肵���t�@�C�����_�E�����[�h���X�g����폜

  Arguments:    game_reg : �Q�[���o�^���
  
  Returns:      
 *---------------------------------------------------------------------------*/

/* �w�肵���t�@�C�����_�E�����[�h���X�g����폜 */
void   *MB_UnregisterFile(const MBGameRegistry *game_reg)
{
    u8      fileID, i;
    void   *ret_bufp;
    OSIntrMode enabled = OS_DisableInterrupts();        /* ���荞�݋֎~ */

    /* GameRegistry�ƈ�v����fileinfo��T�� */
    for (i = 0; i < MB_MAX_FILE; i++)
    {
        if (pPwork->fileinfo[i].game_reg == (MBGameRegistry *)game_reg)
        {
            fileID = i;
            break;
        }
    }

    if (fileID != pPwork->fileinfo[fileID].game_info.fileNo)
    {
        OS_TWarning("Registerd File ID does not correspond with File ID in Registry List.\n");
        (void)OS_RestoreInterrupts(enabled);    /* ���荞�݋֎~���� */
        return NULL;
    }

    /* ��v����fileinfo��������Ȃ�������A�ُ�I�� */
    if (i == MB_MAX_FILE)
    {
        OS_TWarning("Cannot find corresponding GameRegistry\n");
        (void)OS_RestoreInterrupts(enabled);    /* ���荞�݋֎~���� */
        return NULL;
    }

    /* GameInfo����폜 */
    if (FALSE == MB_DeleteGameInfo(&pPwork->fileinfo[fileID].game_info))
    {
        OS_TWarning("Cannot delete GameInfo %s\n",
                    pPwork->fileinfo[fileID].game_info.fixed.gameName);
        (void)OS_RestoreInterrupts(enabled);    /* ���荞�݋֎~���� */
        return NULL;
    }

    ret_bufp = pPwork->fileinfo[fileID].src_adr;

    pPwork->fileinfo[fileID].active = 0;

    /* �Ώۂ�fileinfo���폜 */
    MI_CpuClear16(&pPwork->fileinfo[fileID], sizeof(pPwork->fileinfo[0]));

    pPwork->file_num--;

    MB_DEBUG_OUTPUT("Delete Game \"%s\"\n", game_reg->gameNamep);

    (void)OS_RestoreInterrupts(enabled);        /* ���荞�݋֎~���� */

    return ret_bufp;
}

/*---------------------------------------------------------------------------*
  Name:         MBi_MakeDownloadFileInfo

  Description:  �w�肳�ꂽ�}���`�u�[�g�Q�[���o�L����A�_�E�����[�h�t�@�C�������\�z����B

  Arguments:    mbdlFileInfop - �\�z�����MbDownloadFileInfo�ւ̃|�C���^
                buf           - File�C���[�W�o�b�t�@�ւ̃|�C���^
  
  Returns:      None.
 *---------------------------------------------------------------------------*/

static void MBi_MakeDownloadFileInfo(MbDownloadFileInfoHeader * mbdlFileInfop, const void *buf)
{
    const RomHeader *mbRomHeaderp;
    const MbSegmentType *loadSegListp;
    MbSegmentInfo *seg_infop;
    int     seg_num;
    const u16 *auth_code;
    u32     child_recv_buff_addr = MB_ARM7_STATIC_RECV_BUFFER;

    mbRomHeaderp = (const RomHeader *)buf;
    auth_code = (const u16 *)((u32)buf + ROM_HEADER_SIZE_FULL);

    /*
       ��ROM�w�b�_�̃`�F�b�N�����āAROM�o�C�i�����ǂ����𔻒肵���������������H
     */

    // �}���`�u�[�g�Q�[���������ƂɁA�����_�E�����[�h�t�@�C�������\�z����B
    mbdlFileInfop->arm9EntryAddr = (u32)mbRomHeaderp->main_entry_address;
    mbdlFileInfop->arm7EntryAddr = (u32)mbRomHeaderp->sub_entry_address;
    seg_infop = (MbSegmentInfo *) (mbdlFileInfop + 1);
    loadSegListp = MBi_defaultLoadSegList;
    MB_DEBUG_OUTPUT("\t<segment list>         recv_adr load_adr     size  rom_adr (base)\n");

    /*
     * �Z�O�����g�� static �ɂ�������ł܂Ƃ܂����̂őS�Ĕj��.
     * overlay : Load/Unload ���̊Ǘ����ł��Ȃ��̂ŕs�\.
     * file    : �e�ʂȂǂ̎d�l�����肵�ă��t�@�����X�ɂ���K�v����������I�ɕs�\.
     */
    for (seg_num = 0; seg_num < MB_DL_SEGMENT_NUM; seg_num++)
    {
        /* �e�Z�O�����g�̃��[�h�A�h���X����уT�C�Y���擾���� */
        MB_DEBUG_OUTPUT("\t SEG%2d : ", seg_num);
        MBi_SetSegmentInfo(mbRomHeaderp, loadSegListp, seg_infop, &child_recv_buff_addr);
        seg_infop++;
        loadSegListp++;
    }

    /* �F�ؗp�R�[�h��MBDownloadFileInfo���ɃZ�b�g���� */
    {
        MBDownloadFileInfo *pMdfi = (MBDownloadFileInfo *) mbdlFileInfop;
        MI_CpuCopy8(auth_code, &pMdfi->auth_code[0], MB_AUTHCODE_SIZE);
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBi_SetSegmentInfo

  Description:  �Z�O�����g���̃Z�b�g

  Arguments:    mbRomHeaderp
                loadSegListp
				seg_infop
				child_recv_buffer

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBi_SetSegmentInfo(const RomHeader * mbRomHeaderp,
                               const MbSegmentType * loadSegListp,
                               MbSegmentInfo * seg_infop, u32 *child_recv_buff_addr)
{
    CARDRomRegion *romRegp;

    /* �e�Z�O�����g�̃��[�h�A�h���X����уT�C�Y���擾���� */

    switch (*loadSegListp)
    {
    case MB_SEG_ARM9STATIC:
        romRegp = (CARDRomRegion *)(&mbRomHeaderp->main_ram_address);

        if (((u32)romRegp->offset >= MB_LOAD_AREA_LO)
            && ((u32)romRegp->offset < MB_LOAD_AREA_HI)
            && (((u32)romRegp->offset + romRegp->length) <= MB_LOAD_AREA_HI))
        {

            seg_infop->size = romRegp->length;
            seg_infop->load_addr = (u32)romRegp->offset;
            /* �����k�W�J���l������ƁA�����I�ɂ́Aload_addr == recv_addr�ł̓_�� */
            seg_infop->recv_addr = seg_infop->load_addr;
            seg_infop->target = MI_PROCESSOR_ARM9;
            MB_DEBUG_OUTPUT("arm9 static :");
        }
        else
        {
            // ���[�h�v���O�����̃A�h���X�������̓A�h���X�{�T�C�Y�����[�h�\�G���A�𒴂��Ă���
            OS_Panic("ARM9 boot code out of the load area. : addr = %x  size = %x\n",
                     (u32)romRegp->offset, romRegp->length);
        }
        break;

    case MB_SEG_ARM7STATIC:
        {
            BOOL    error_flag = FALSE;
            BOOL    reload_flag = FALSE;
            u32     load_last_addr;

            romRegp = (CARDRomRegion *)(&mbRomHeaderp->sub_ram_address);
            load_last_addr = (u32)((u32)romRegp->offset + romRegp->length);

            // �z�u�A�h���X�A�T�C�Y�̔���
            /* �z�u�擪�A�h���X�ɂ���(���C���������ɔz�u�̏ꍇ) */
            if (((u32)romRegp->offset >= MB_LOAD_AREA_LO)
                && ((u32)romRegp->offset < MB_BSSDESC_ADDRESS))
            {
                /* �z�u�A�h���X�Ō���ɂ��� */
                if (load_last_addr <= MB_ARM7_STATIC_LOAD_AREA_HI)      // �Ĕz�u�̕K�v�̂Ȃ��ꍇ�͂��̂܂܃Z�b�g
                {
                }
                else if ((load_last_addr < MB_BSSDESC_ADDRESS)
                         && (romRegp->length <= MB_ARM7_STATIC_RECV_BUFFER_SIZE))
                {                      // �Ĕz�u���K�v�ȏꍇ�́A���̂悤�ɁB
                    reload_flag = TRUE;
                }
                else
                {
                    error_flag = TRUE;
                }
            }
            else
                /* �z�u�擪�A�h���X�ɂ���(WRAM�ɔz�u�̏ꍇ) */
            if (((u32)romRegp->offset >= HW_WRAM)
                    && ((u32)romRegp->offset < HW_WRAM + MB_ARM7_STATIC_LOAD_WRAM_MAX_SIZE))
            {

                if (load_last_addr <= (HW_WRAM + MB_ARM7_STATIC_LOAD_WRAM_MAX_SIZE))
                {                      // �Ĕz�u�Ƃ���B
                    reload_flag = TRUE;
                }
                else
                {
                    error_flag = TRUE;
                }
            }
            else
            {                          // ����ȊO�̗̈�̎��̓G���[
                error_flag = TRUE;
            }

            // �G���[����
            if (error_flag == TRUE)
            {
                OS_Panic("ARM7 boot code out of the load area. : addr = %x  size = %x\n",
                         (u32)romRegp->offset, romRegp->length);
            }

            // �Z�O�����g���Z�b�g
            {
                seg_infop->size = romRegp->length;
                seg_infop->load_addr = (u32)romRegp->offset;
                if (!reload_flag)
                {
                    seg_infop->recv_addr = seg_infop->load_addr;
                }
                else
                {
                    /*
                     * ARM7�Ŏ��s�A�h���X���Ĕz�u���K�v�ȗ̈�Ȃ�A
                     * ��M�A�h���X�����C����������M�o�b�t�@�ɐݒ肷��B
                     */
                    seg_infop->recv_addr = *child_recv_buff_addr;
                    *child_recv_buff_addr += seg_infop->size;
                }
            }

            seg_infop->target = MI_PROCESSOR_ARM7;
            MB_DEBUG_OUTPUT("arm7 static :");
        }
        break;

    case MB_SEG_ROMHEADER:
        seg_infop->size = ROM_HEADER_SIZE_FULL;
        seg_infop->load_addr = (u32)MB_ROM_HEADER_ADDRESS;
        seg_infop->recv_addr = seg_infop->load_addr;
        // ROMHEADER��target��MI_PROCESSOR_ARM9�Ƃ���
        seg_infop->target = MI_PROCESSOR_ARM9;
        MB_DEBUG_OUTPUT("rom header  :");
        break;
    }

    MB_DEBUG_OUTPUT(" %8x %8x %8x %8x\n",
                    seg_infop->recv_addr, seg_infop->load_addr,
                    seg_infop->size,
                    (*loadSegListp == MB_SEG_ROMHEADER) ? 0 : *((u32 *)romRegp - 2));

}

/*	----------------------------------------------------------------------------

	�u���b�N��񐧌�֐�(�e�@�p)

	----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         MBi_MakeBlockInfoTable

  Description:  Block���擾�̂��߂̃e�[�u�����쐬

  Arguments:    

  Returns:      
 *---------------------------------------------------------------------------*/


BOOL MBi_MakeBlockInfoTable(MB_BlockInfoTable * table, MbDownloadFileInfoHeader * mdfi)
{
    u16    *seg_headB = table->seg_head_blockno;
    u32    *seg_src = table->seg_src_offset;
    u8      i;
    u32     src_ofs = 0;

    if (!mdfi)
        return FALSE;

    for (i = 0; i < MB_DL_SEGMENT_NUM; ++i)
    {
        MbSegmentInfo *si = MBi_GetSegmentInfo(mdfi, i);
        seg_src[i] = src_ofs;
        src_ofs += si->size;
    }

    seg_headB[0] = 0;                  // Segment 0

    for (i = 0; i < MB_DL_SEGMENT_NUM; ++i)
    {
        MbSegmentInfo *si = MBi_GetSegmentInfo(mdfi, i);
        u16     next_block_head =
            (u16)(seg_headB[i] + (u16)((si->size + mbc->block_size_max - 1) / mbc->block_size_max));

        if (FALSE == IsAbleToLoad(i, si->load_addr, si->size))
        {
            return FALSE;
        }

        if (i < MB_DL_SEGMENT_NUM - 1)
        {
            seg_headB[i + 1] = next_block_head;
        }
        else
        {
            table->block_num = next_block_head;
        }

    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MBi_get_blockinfo

  Description:  Block���̎擾

  Arguments:    

  Returns:      
 *---------------------------------------------------------------------------*/

BOOL MBi_get_blockinfo(MB_BlockInfo * bi, MB_BlockInfoTable * table,
                       u32 block, MbDownloadFileInfoHeader * mdfi)
{
    s8      i;
    u32     seg_block, block_adr_offset;

    if (block >= table->block_num)
    {
        return FALSE;
    }

    for (i = MB_DL_SEGMENT_NUM - 1; i >= 0; i--)
    {
        if (block >= table->seg_head_blockno[i])
        {
            break;
        }
    }

    if (i < 0)
        return FALSE;

    seg_block = block - table->seg_head_blockno[i];
    block_adr_offset = seg_block * mbc->block_size_max;

    {
        MbSegmentInfo *si = MBi_GetSegmentInfo(mdfi, i);
        bi->size = si->size - block_adr_offset;
        if (bi->size > mbc->block_size_max)
            bi->size = (u32)mbc->block_size_max;
        bi->offset = (u32)(block_adr_offset + table->seg_src_offset[i]);
        bi->child_address = (u32)(block_adr_offset + (u32)(si->recv_addr));
        bi->segment_no = (u8)i;
    }

    MB_DEBUG_OUTPUT("blockNo:%d \n", block);
    MB_DEBUG_OUTPUT("Segment %d [block:%d offset(%08x) destination(%08x) size(%04x)]\n",
                    i, seg_block, bi->offset, bi->child_address, bi->size);

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         MBi_get_blocknum

  Description:  Block�����̎擾

  Arguments:    

  Returns:      
 *---------------------------------------------------------------------------*/

u16 MBi_get_blocknum(MB_BlockInfoTable * table)
{
    SDK_ASSERT(table != 0);
    return table->block_num;
}


/*---------------------------------------------------------------------------*
  Name:         MBi_IsAbleToRecv

  Description:  ��M�A�h���X�̐������`�F�b�N

  Arguments:    

  Returns:      
 *---------------------------------------------------------------------------*/

BOOL MBi_IsAbleToRecv(u8 segment_no, u32 address, u32 size)
{
    MbSegmentType seg_type;

    SDK_ASSERT(segment_no < MB_DL_SEGMENT_NUM);

    seg_type = MBi_defaultLoadSegList[segment_no];

    switch (seg_type)
    {
    case MB_SEG_ROMHEADER:
        if (address >= MB_ROM_HEADER_ADDRESS
            && address + size <= MB_ROM_HEADER_ADDRESS + ROM_HEADER_SIZE_FULL)
        {
            return TRUE;
        }
        break;

    case MB_SEG_ARM9STATIC:
        if (address >= MB_LOAD_AREA_LO && address + size <= MB_LOAD_AREA_HI)
        {
            return TRUE;
        }
        break;

    case MB_SEG_ARM7STATIC:
        /* wram mapping */
        if (address >= MB_ARM7_STATIC_RECV_BUFFER
            && address + size <= MB_ARM7_STATIC_RECV_BUFFER_END)
        {
            return TRUE;
        }
        else
            /* main memory mapping */
        if (address >= MB_LOAD_AREA_LO && address + size <= MB_ARM7_STATIC_RECV_BUFFER_END)
        {
            return TRUE;
        }
        break;

    default:
        return FALSE;
    }

    return FALSE;
}

static BOOL IsAbleToLoad(u8 segment_no, u32 address, u32 size)
{
    MbSegmentType seg_type;

    SDK_ASSERT(segment_no < MB_DL_SEGMENT_NUM);

    seg_type = MBi_defaultLoadSegList[segment_no];

    switch (seg_type)
    {
    case MB_SEG_ROMHEADER:
    case MB_SEG_ARM9STATIC:
        return MBi_IsAbleToRecv(segment_no, address, size);

    case MB_SEG_ARM7STATIC:
        /* main memory */
        if (address >= MB_LOAD_AREA_LO && address < MB_BSSDESC_ADDRESS)
        {
            u32     end_adr = address + size;

            if (MB_ARM7_STATIC_RECV_BUFFER_END > address
                && MB_ARM7_STATIC_RECV_BUFFER_END < end_adr)

            {
                return FALSE;          // NG
            }
            else if (end_adr <= MB_ARM7_STATIC_LOAD_AREA_HI)
            {
                return TRUE;           // OK
            }
            else if (end_adr < MB_BSSDESC_ADDRESS && size <= MB_ARM7_STATIC_RECV_BUFFER_SIZE)
            {
                return TRUE;           // OK (needs relocate)
            }
            else
            {
                return FALSE;          // NG
            }
        }
        else
            /* wram */
        if (address >= HW_WRAM && address < HW_WRAM + MB_ARM7_STATIC_LOAD_WRAM_MAX_SIZE)
        {
            u32     end_adr = address + size;
            if (end_adr <= (HW_WRAM + MB_ARM7_STATIC_LOAD_WRAM_MAX_SIZE))
            {
                return TRUE;           // OK
            }
            else
            {
                return FALSE;          // NG
            }
        }
        break;

    default:
        return FALSE;
    }

    return FALSE;
}

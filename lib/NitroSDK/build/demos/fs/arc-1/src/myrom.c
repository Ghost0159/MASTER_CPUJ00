/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - libraries
  File:     fs_rom.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: myrom.c,v $
  Revision 1.6  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.5  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.4  2005/02/18 08:57:08  yosizaki
  fix around hidden warnings.

  Revision 1.3  2004/07/23 08:19:19  yosizaki
  add explicit cast to OS_LockCard/OS_UnlockCard.

  Revision 1.2  2004/07/19 11:44:28  yosizaki
  change callback routine.

  Revision 1.1  2004/07/09 01:00:17  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/fs.h>
#include <nitro/card.h>
#include "myrom.h"


/********************************************************************/
/* �֐� */

/* �񓯊��̃y�[�W�P�� CARD ���������������Ƃ��̏��� */
static void MyRom_OnReadDone(void *p_arc)
{
    /* �A�[�J�C�u�֊����ʒm */
    FS_NotifyArchiveAsyncEnd((FSArchive *)p_arc, FS_RESULT_SUCCESS);
}

/* ���[�h�R�[���o�b�N */
static FSResult MyRom_ReadCallback(FSArchive *p_arc, void *dst, u32 src, u32 len)
{
    MyRomArchive *const p_rom = (MyRomArchive *) p_arc;
    CARD_ReadRomAsync(p_rom->default_dma_no,
                      (const void *)(FS_GetArchiveBase(p_arc) + src), dst, len,
                      MyRom_OnReadDone, p_arc);
    return FS_RESULT_PROC_ASYNC;
}

/*
 * ���C�g�R�[���o�b�N
 * ���[�U�v���V�[�W���� FS_RESULT_UNSUPPORTED ��Ԃ��̂�
 * ��{�I�ɂ͌Ă΂꓾�Ȃ�.
*/
static FSResult MyRom_WriteDummyCallback(FSArchive *p_arc, const void *src, u32 dst, u32 len)
{
    (void)p_arc;
    (void)src;
    (void)dst;
    (void)len;
    return FS_RESULT_FAILURE;
}

/*
 * ���[�U�v���V�[�W��.
 * �ŏ��̃R�}���h�J�n�O����Ō�̃R�}���h������܂ŃJ�[�h�����b�N.
 * �܂�, ���C�g����̓T�|�[�g�O�Ƃ��ĉ�������.
 */
static FSResult MyRom_ArchiveProc(FSFile *p_file, FSCommandType cmd)
{
    MyRomArchive *const p_rom = (MyRomArchive *) FS_GetAttachedArchive(p_file);

    switch (cmd)
    {

    case FS_COMMAND_ACTIVATE:
        CARD_LockRom((u16)p_rom->card_lock_id);
        return FS_RESULT_SUCCESS;

    case FS_COMMAND_IDLE:
        CARD_UnlockRom((u16)p_rom->card_lock_id);
        return FS_RESULT_SUCCESS;

    case FS_COMMAND_WRITEFILE:
        return FS_RESULT_UNSUPPORTED;

    default:
        return FS_RESULT_PROC_UNKNOWN;

    }

}


/* ���� ROM �A�[�J�C�u�̏����� */
void MyRom_Init(MyRomArchive * p_rom)
{
    /* �A�[�J�C�u�������� */
    FS_InitArchive(p_rom->arc);
    /* �f�t�H���g DMA �������� */
    p_rom->default_dma_no = FS_DMA_NOT_USE;
    /* �J�[�h���b�N�p�� ID ���m�� */
    p_rom->card_lock_id = (u32)OS_GetLockID();

    /* �J�[�h���W���[�����g�p����̂�, �O�̂��ߏ�ɏ������Ăяo�� */
    CARD_Init();
}


/* DMA �̎w�肨��уA�[�J�C�u�̓o�^ / ���[�h */
BOOL MyRom_Create(MyRomArchive * p_rom,
                  const char *name, int name_len,
                  u32 base, const CARDRomRegion *fnt, const CARDRomRegion *fat)
{
    /* �w��̖��O�ŃA�[�J�C�u��o�^ */
    if (!FS_RegisterArchiveName(p_rom->arc, name, (u32)name_len))
    {
        OS_Panic("error! FS_RegisterArchiveName(%*s) failed.\n", name_len, name);
        return FALSE;
    }

    /* �K�v�ȃ��[�U�v���V�[�W����ݒ� */
    FS_SetArchiveProc(p_rom->arc, MyRom_ArchiveProc,
                      FS_ARCHIVE_PROC_WRITEFILE | FS_ARCHIVE_PROC_ACTIVATE | FS_ARCHIVE_PROC_IDLE);
    /* ���ۂɃA�[�J�C�u�����[�h */
    if (!FS_LoadArchive(p_rom->arc, base,
                        fat->offset, fat->length, fnt->offset, fnt->length,
                        MyRom_ReadCallback, MyRom_WriteDummyCallback))
    {
        OS_Panic("error! FS_LoadArchive() failed.\n");
        return FALSE;
    }
    /* ���[�h���� */
    return TRUE;
}

/* ���݂̃f�t�H���g DMA ���擾 */
u32 MyRom_GetDefaultDMA(const MyRomArchive * p_rom)
{
    return p_rom->default_dma_no;
}

/* �f�t�H���g DMA ��ݒ� */
u32 MyRom_SetDefaultDMA(MyRomArchive * p_rom, u32 dma_no)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();
    u32     bak_dma_no = p_rom->default_dma_no;

    BOOL    bak_stat = FS_SuspendArchive(p_rom->arc);

    p_rom->default_dma_no = dma_no;

    if (bak_stat)
        (void)FS_ResumeArchive(p_rom->arc);

    (void)OS_RestoreInterrupts(bak_psr);
    return bak_dma_no;
}

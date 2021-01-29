/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - include
  File:     rom.h

  Copyright 2004-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: rom.h,v $
  Revision 1.17  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.16  2005/11/29 01:21:00  yosizaki
  fix comments and indent.

  Revision 1.15  2005/03/18 06:52:57  yosizaki
  add CARDRomHeader.

  Revision 1.14  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.13  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.12  2004/12/08 08:53:06  yosizaki
  delete CARDi_ReadRomID()
  add comments.

  Revision 1.11  2004/11/02 10:07:30  yosizaki
  fix comment.

  Revision 1.10  2004/09/06 13:44:31  yosizaki
  move CARD_(Lock/Unlock)Backup

  Revision 1.9  2004/09/03 07:19:12  yosizaki
  fix include directives.

  Revision 1.8  2004/09/03 06:50:43  yosizaki
  add CARD_GetRomRegionOVT.

  Revision 1.7  2004/07/23 08:32:14  yosizaki
  small fix.

  Revision 1.6  2004/07/19 13:10:47  yosizaki
  change interface.

  Revision 1.5  2004/07/10 10:20:47  yosizaki
  add CARD_LockBackup()

  Revision 1.4  2004/07/09 08:40:13  yosizaki
  change implement of synchronous mode.

  Revision 1.3  2004/07/08 13:47:42  yosizaki
  change CARD_ReadRom (for fs_rom)

  Revision 1.2  2004/07/05 08:10:41  yosizaki
  add comment.
  include card/common.h

  Revision 1.1  2004/06/28 01:55:02  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#if	!defined(NITRO_CARD_ROM_H_)
#define NITRO_CARD_ROM_H_


#ifdef __cplusplus
extern "C"
{
#endif


#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/memorymap.h>
#include <nitro/mi/dma.h>
#include <nitro/mi/exMemory.h>
#include <nitro/os.h>

#include <nitro/card/common.h>


/*****************************************************************************/
/* declarations */

/* ROM �w�b�_ �̈���\���� */
typedef struct
{
    u32     offset;
    u32     length;
}
CARDRomRegion;

/* ROM �w�b�_ �\���� */
typedef struct
{

    /* 0x000-0x020 [�V�X�e���\��̈�] */
    char    game_name[12];             /* �\�t�g�^�C�g���� */
    u32     game_code;                 /* �C�j�V�����R�[�h */
    u16     maker_code;                /* ���[�J�[�R�[�h */
    u8      product_id;                /* �{�̃R�[�h */
    u8      device_type;               /* �f�o�C�X�^�C�v */
    u8      device_size;               /* �f�o�C�X�e�� */
    u8      reserved_A[9];             /* �V�X�e���\�� A */
    u8      game_version;              /* �\�t�g�o�[�W���� */
    u8      property;                  /* �����g�p�t���O */

    /* 0x020-0x040 [�풓���W���[���p�p�����[�^] */
    void   *main_rom_offset;           /* ARM9 �]���� ROM �I�t�Z�b�g */
    void   *main_entry_address;        /* ARM9 ���s�J�n�A�h���X(������) */
    void   *main_ram_address;          /* ARM9 �]���� RAM �I�t�Z�b�g */
    u32     main_size;                 /* ARM9 �]���T�C�Y */
    void   *sub_rom_offset;            /* ARM7 �]���� ROM �I�t�Z�b�g */
    void   *sub_entry_address;         /* ARM7 ���s�J�n�A�h���X(������) */
    void   *sub_ram_address;           /* ARM7 �]���� RAM �I�t�Z�b�g */
    u32     sub_size;                  /* ARM7 �]���T�C�Y */

    /* 0x040-0x050 [�t�@�C���e�[�u���p�p�����[�^] */
    CARDRomRegion fnt;                 /* �t�@�C���l�[���e�[�u�� */
    CARDRomRegion fat;                 /* �t�@�C���A���P�[�V�����e�[�u�� */

    /* 0x050-0x060 [�I�[�o�[���C�w�b�_�e�[�u���p�p�����[�^] */
    CARDRomRegion main_ovt;            /* ARM9 �I�[�o�[���C�w�b�_�e�[�u�� */
    CARDRomRegion sub_ovt;             /* ARM7 �I�[�o�[���C�w�b�_�e�[�u�� */

    /* 0x060-0x070 [�V�X�e���\��̈�] */
    u8      rom_param_A[8];            /* �}�X�N ROM �R���g���[���p�����[�^ A */
    u32     banner_offset;             /* �o�i�[�t�@�C�� ROM �I�t�Z�b�g */
    u16     secure_crc;                /* �Z�L���A�̈� CRC */
    u8      rom_param_B[2];            /* �}�X�N ROM �R���g���[���p�����[�^ B */

    /* 0x070-0x078 [�I�[�g���[�h�p�����[�^] */
    void   *main_autoload_done;        /* ARM9 �I�[�g���[�h�t�b�N�A�h���X */
    void   *sub_autoload_done;         /* ARM7 �I�[�g���[�h�t�b�N�A�h���X */

    /* 0x078-0x0C0 [�V�X�e���\��̈�] */
    u8      rom_param_C[8];            /* �}�X�N ROM �R���g���[���p�����[�^ C */
    u32     rom_size;                  /* �A�v���P�[�V�����ŏI ROM �I�t�Z�b�g */
    u32     header_size;               /* ROM �w�b�_�T�C�Y */
    u8      reserved_B[0x38];          /* �V�X�e���\�� B */

    /* 0x0C0-0x160 [�V�X�e���\��̈�] */
    u8      logo_data[0x9C];           /* NINTENDO ���S�C���[�W�f�[�^ */
    u16     logo_crc;                  /* NINTENDO ���S CRC */
    u16     header_crc;                /* ROM ���o�^�f�[�^ CRC */

}
CARDRomHeader;


/*****************************************************************************/
/* constants */

/* ROM �G���A�E�T�C�Y */
#define CARD_ROM_PAGE_SIZE	512


/*****************************************************************************/
/* functions */

/*---------------------------------------------------------------------------*
  Name:         CARD_GetRomRegionFNT

  Description:  ROM �w�b�_�� FNT �̈�����擾����.

  Arguments:    None.

  Returns:      ROM �w�b�_�� FNT �̈��� �ւ̃|�C���^.
 *---------------------------------------------------------------------------*/
static inline const CARDRomRegion *CARD_GetRomRegionFNT(void)
{
    return (const CARDRomRegion *)((const u8 *)HW_ROM_HEADER_BUF + 0x40);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_GetRomRegionFAT

  Description:  ROM �w�b�_�� FAT �̈�����擾����.

  Arguments:    None.

  Returns:      ROM �w�b�_�� FAT �̈��� �ւ̃|�C���^.
 *---------------------------------------------------------------------------*/
static inline const CARDRomRegion *CARD_GetRomRegionFAT(void)
{
    return (const CARDRomRegion *)((const u8 *)HW_ROM_HEADER_BUF + 0x48);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_GetRomRegionOVT

  Description:  ROM �w�b�_�� OVT �̈�����擾����.

  Arguments:    None.

  Returns:      ROM �w�b�_�� OVT �̈��� �ւ̃|�C���^.
 *---------------------------------------------------------------------------*/
static inline const CARDRomRegion *CARD_GetRomRegionOVT(MIProcessor target)
{
    return (target == MI_PROCESSOR_ARM9) ?
        (const CARDRomRegion *)((const u8 *)HW_ROM_HEADER_BUF + 0x50) :
        (const CARDRomRegion *)((const u8 *)HW_ROM_HEADER_BUF + 0x58);
}

/*
 *	�ȉ��̊֐��� CARD ��f�o�C�X�ւ̒ᐅ���A�N�Z�X��񋟂��܂�.
 *	�����̎g�p�ɍۂ��Ă͕K������o�X�̐ݒ�����ȊǗ����Ă�������.
 *
 *	TEG ���J�[�g���b�W�ł̊����ɂ����Ă�
 *	�A�N�Z�X�Ώۂ�����I�ɃJ�[�g���b�W�֕ύX���܂��̂�,
 *	��������������Ċ�{�I�ɓ��l�ȏ������\�ł�.
 *
 *	CARD_LockRom() / CARD_UnlockRom() /
 *	CARD_LockBackup() / CARD_UnlockBackup() ��
 *	OS_LockCard() �܂��� OS_LockCartridge() �̂ǂ����
 *	�g�p���ׂ������I�ɔ��f���Ă�����Ăяo���܂�.
 *	�܂�, ROM �ƃo�b�N�A�b�v�̗����𓯎��ɃA�N�Z�X���Ȃ��悤
 *	�����Ŕr�����䂪�s���܂�.
 */

#if	defined(SDK_TEG)
static inline BOOL CARDi_IsTrueRom(void)
{
    return (OS_GetConsoleType() & OS_CONSOLE_DEV_CARD) != 0;
}
#endif


/*---------------------------------------------------------------------------*
  Name:         CARD_LockRom

  Description:  lock the CARD-bus before ROM operation.

  Arguments:    lock id.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARD_LockRom(u16 lock_id);

/*---------------------------------------------------------------------------*
  Name:         CARD_UnlockRom

  Description:  unlock the CARD-bus after ROM operation.

  Arguments:    lock id which is used by CARD_LockRom().

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARD_UnlockRom(u16 lock_id);

/*
 *	�ȉ��� ROM �֓ǂݍ��݃A�N�Z�X���铯�� / �񓯊��֐��ł�.
 *	�֐����ł̃n�[�h�E�F�A����͈ȉ��̂Ƃ���ł�.
 *
 *	�EDMA ���w�肳��Ă���� MI_StopDma() �Ŗ����I�ɒ�~���Ă���g�p����
 *	�EDMA ��͈͊O�Ɏw�肷��� CPU �]�����g�p����
 *	�E�O��̃A�N�Z�X���������Ă��Ȃ���Ί����܂Ńu���b�L���O����
 *
 *	�J�[�h�]�����x���̂��͍̂ő� 5.96[MB/s] �ł���,
 *	���ۂ� API �ł͂����炩�̃I�[�o�[�w�b�h���܂݂܂�.
 *	CPU �ɂ�铯���]���̏ꍇ�͂ق� 100% �̑��x�������ł��܂�.
 *	DMA ���g�p�����񓯊��]���̏ꍇ�� 90 �` 95%  �ƂȂ�܂���,
 *	�]�����̑����̎��Ԃ� CPU ������Ɏ��s�ł��܂�.
 *	�����, ���̍ő�l�͓]���������ŗǂ̏ꍇ�Ɍ��肳��܂�.
 */
/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadRom

  Description:  ROM ���[�h�̊�{�֐�

  Arguments:    dma        �g�p���� DMA �`�����l��
                src        �]�����I�t�Z�b�g
                dst        �]���惁�����A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊����[�h���w�肷��Ȃ� TRUE

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_ReadRom(u32 dma,
                      const void *src, void *dst, u32 len,
                      MIDmaCallback callback, void *arg, BOOL is_async);

/*---------------------------------------------------------------------------*
  Name:         CARD_TryWaitRomAsync

  Description:  check if asynchronous ROM operation has been completed.

  Arguments:    None.

  Returns:      if operation has been completed, TRUE.
 *---------------------------------------------------------------------------*/
BOOL    CARD_TryWaitRomAsync(void);

/*---------------------------------------------------------------------------*
  Name:         CARD_WaitRomAsync

  Description:  wait for completion of current asynchronous ROM operation.

  Arguments:    None.

  Returns:      if the newest operation is successful, TRUE.
 *---------------------------------------------------------------------------*/
void    CARD_WaitRomAsync(void);

/*---------------------------------------------------------------------------*
  Name:         CARD_ReadRomAsync

  Description:  �񓯊� ROM ���[�h

  Arguments:    dma        �g�p���� DMA �`�����l��
                src        �]�����I�t�Z�b�g
                dst        �]���惁�����A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void CARD_ReadRomAsync(u32 dma,
                                     const void *src, void *dst, u32 len,
                                     MIDmaCallback callback, void *arg)
{
    CARDi_ReadRom(dma, src, dst, len, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ReadRom

  Description:  ���� ROM ���[�h

  Arguments:    dma        �g�p���� DMA �`�����l��
                src        �]�����I�t�Z�b�g
                dst        �]���惁�����A�h���X
                len        �]���T�C�Y

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void CARD_ReadRom(u32 dma, const void *src, void *dst, u32 len)
{
    CARDi_ReadRom(dma, src, dst, len, NULL, NULL, FALSE);
}


/* internal function */
u32     CARDi_ReadRomID(void);


/* I/O processror ************************************************************/

#if	defined(SDK_TEG) && defined(SDK_ARM7)

void    CARDi_CreatePxiRecvThread(u32 priority);

#endif /* defined(SDK_TEG) && defined(SDK_ARM7) */


#ifdef __cplusplus
} /* extern "C" */
#endif


/* NITRO_CARD_ROM_H_ */
#endif

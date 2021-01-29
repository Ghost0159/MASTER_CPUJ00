/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - libraries
  File:     card_rom.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/

#if !defined(NITRO_LIBRARIES_CARD_ROM_H__)
#define NITRO_LIBRARIES_CARD_ROM_H__


#include <nitro.h>
#include "../include/card_common.h"

#if defined(SDK_TEG)
#define SDK_ARM7_READROM_SUPPORT
#endif

/*****************************************************************************/
/* macro */


/*****************************************************************************/
/* constant */

/* �J�[�h���䃌�W�X�^ */
#define REG_CARD_MASTER_CNT (HW_REG_BASE + 0x1A1)
#define REG_CARDCNT         (HW_REG_BASE + 0x1A4)
#define REG_CARD_CMD        (HW_REG_BASE + 0x1A8)
#define REG_CARD_DATA       (HW_REG_BASE + 0x100010)

/* REG_CARD_MASTER_CNT �ݒ�r�b�g */
#define CARDMST_SEL_ROM     0x00
#define CARDMST_IF_ENABLE   0x40
#define CARDMST_ENABLE      0x80

/* �J�[�h�R�}���h�t�H�[�}�b�g (1 �y�[�W�ȊO�̓n�[�h�I�ɔ�Ή�) */
#define CARD_DATA_READY     0x00800000
#define CARD_COMMAND_PAGE   0x01000000
#define CARD_COMMAND_ID     0x07000000
#define CARD_COMMAND_MASK   0x07000000
#define CARD_RESET_HI       0x20000000
#define CARD_RESET_LO       0x00000000
#define CARD_ACCESS_MODE    0x40000000
#define CARD_READ_MODE      0x00000000
#define CARD_WRITE_MODE     0x40000000
#define CARD_START          0x80000000
#define CARD_LATENCY1_MASK  0x00001FFF

#define MROMOP_G_READ_ID    0xB8000000
#define MROMOP_G_READ_PAGE  0xB7000000


/*****************************************************************************/
/* declaration */

/* CARD / CARTRIDGE / PXI �S���ʂ̏�ԊǗ��\���� */
typedef struct CARDRomStat
{
    /* ROM �A�N�Z�X�p�p�����[�^ */
    void    (*read_func) (struct CARDRomStat *);
    u32     ctrl;                      /* CARD �ŗL : �R���g���[���t���O */
    u8     *cache_page;                /* ���݂̃L���b�V���y�[�W */

    /* PXI ��M�҂� */
    u32     dummy[5];
    /* 32 BYTE alignment */

    /* �Ō�ɓ]�������y�[�W�̃L���b�V�� (PXI / CARD) */
    u8      cache_buf[CARD_ROM_PAGE_SIZE];

}
CARDRomStat;


SDK_COMPILER_ASSERT(sizeof(CARDRomStat) % 32 == 0);


/*****************************************************************************/
/* variable */

extern CARDRomStat rom_stat;
extern u32 cardi_rom_header_addr;

/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         CARDi_IsInTcm

  Description:  �w�胁�����̈悪�ꕔ�ł� TCM �͈͂ɊY�����邩����.

  Arguments:    buf        �������A�h���X�擪
                len        �������T�C�Y

  Returns:      �ꕔ�ł� TCM �͈͂ɊY������� TRUE, �����łȂ���� FALSE.
 *---------------------------------------------------------------------------*/
static inline BOOL CARDi_IsInTcm(u32 buf, u32 len)
{
#if defined(SDK_ARM9)
    const u32 i = OS_GetITCMAddress();
    const u32 d = OS_GetDTCMAddress();
    return
        ((i < buf + len) && (i + HW_ITCM_SIZE > buf)) ||
        ((d < buf + len) && (d + HW_DTCM_SIZE > buf));
#else
#pragma unused(buf)
#pragma unused(len)
    return FALSE;
#endif
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_GetRomFlag

  Description:  �J�[�h�R�}���h�R���g���[���p�����[�^���擾.

  Arguments:    flag       �J�[�h�f�o�C�X�֔��s����R�}���h�̃^�C�v
                           (CARD_COMMAND_PAGE �� CARD_COMMAND_ID)

  Returns:      �J�[�h�R�}���h�R���g���[���p�����[�^.
 *---------------------------------------------------------------------------*/
static inline u32 CARDi_GetRomFlag(u32 flag)
{
    const u32 rom_ctrl = *(vu32 *)(cardi_rom_header_addr + 0x60);
    return (u32)((rom_ctrl & ~CARD_COMMAND_MASK) | flag |
                 CARD_READ_MODE | CARD_START | CARD_RESET_HI);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CheckPulledOutCore

  Description:  �J�[�h�������o�֐��̃��C������.
                �J�[�h�o�X�̓��b�N����Ă���K�v������.

  Arguments:    id            �J�[�h����ǂݏo���ꂽ ROM-ID

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_CheckPulledOutCore(u32 id);


#if	defined(SDK_TS) || defined(SDK_ARM7)

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadRomIDCore

  Description:  �J�[�h ID �̓ǂݏo��.

  Arguments:    None.

  Returns:      �J�[�h ID.
 *---------------------------------------------------------------------------*/
u32     CARDi_ReadRomIDCore(void);

#endif

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadEnd

  Description:  ROM �A�N�Z�X�I������.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void CARDi_ReadEnd(void)
{
    CARDiCommon *const p = &cardi_common;
#ifdef SDK_ARM9
    /* �����ŃJ�[�h�������o���s��. (���O�̔r�������͕s�v) */
    CARDi_CheckPulledOutCore(CARDi_ReadRomIDCore());
#endif
    p->cmd->result = CARD_RESULT_SUCCESS;
    CARDi_EndTask(p, TRUE);
}


#if	defined(SDK_ARM7)

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadRomCore

  Description:  ARM7 ����̃J�[�h�A�N�Z�X.

  Arguments:    src        �]�����I�t�Z�b�g
                src        �]�����������A�h���X
                src        �]���T�C�Y

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_ReadRomCore(const void *src, void *dst, u32 len);

#endif


/*****************************************************************************
 * ���ˑ�����
 *
 *   TEG ���ɂ����� CARD �A�N�Z�X�͕��G�ȏ����̂��ƂɎ�������Ă���.
 *   �܂�, ���s�����̂� CARD �łȂ��ꍇ�� CARTRIDGE �ő�ւ����.
 *   CARD �Ή��ł� TEG �ŗL�d�l��� ARM7 �����A�N�Z�X�ł��Ȃ�����
 *   ARM9 �����  PXI �o�R�� ARM7 �փ��N�G�X�g�𑗂�.
 *
 *****************************************************************************/


/*---------------------------------------------------------------------------*
  Name:         CARDi_GetRomAccessor

  Description:  ��������ƂɈقȂ�J�[�h�A�N�Z�X�֐��ւ̃|�C���^��Ԃ�.

  Arguments:    None.

  Returns:      �ȉ��̂����ꂩ.
                CARDi_ReadCard(), CARDi_ReadPxi(), CARDi_ReadCartridge().
 *---------------------------------------------------------------------------*/
void    (*CARDi_GetRomAccessor(void)) (CARDRomStat *);

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadCard

  Description:  �ݒ肳�ꂽ���e�Ɋ�Â��ăJ�[�h�A�N�Z�X�𒼐ڎ��s.

  Arguments:    p          �A�N�Z�X���e���ݒ肳�ꂽ�\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_ReadCard(CARDRomStat * p);

/*---------------------------------------------------------------------------*
  Name:         CARDi_TryReadCardDma

  Description:  �ݒ肳�ꂽ���e�� DMA �ŃJ�[�h�A�N�Z�X�\�ȏ����𖞂�����
                �񓯊� DMA ���J�n����.

  Arguments:    p          �A�N�Z�X���e���ݒ肳�ꂽ�\����

  Returns:      �񓯊� DMA ���J�n�����ꍇ�� TRUE, �����łȂ��ꍇ�� FALSE.
 *---------------------------------------------------------------------------*/
BOOL    CARDi_TryReadCardDma(CARDRomStat * p);


#if defined(SDK_TEG)

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadCartridge

  Description:  �ݒ肳�ꂽ���e�Ɋ�Â��ăJ�[�g���b�W�A�N�Z�X�����s.

  Arguments:    p          �A�N�Z�X���e���ݒ肳�ꂽ�\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_ReadCartridge(CARDRomStat * p);

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadPxi

  Description:  �ݒ肳�ꂽ���e�Ɋ�Â��ăJ�[�h�A�N�Z�X�� PXI �o�R�Ŏ��s.

  Arguments:    p          �A�N�Z�X���e���ݒ肳�ꂽ�\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARDi_ReadPxi(CARDRomStat * p);

#endif /* defined(SDK_TEG) */


#endif /* NITRO_LIBRARIES_CARD_ROM_H__ */


/*---------------------------------------------------------------------------*
  $Log: card_rom.h,v $
  Revision 1.7  2006/08/23 00:39:45  yosizaki
  add cardi_rom_header_addr.

  Revision 1.6  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.5  2005/11/09 07:38:50  adachi_hiroaki
  ARM7����ROM�ǂݍ��݂𖳌���

  Revision 1.4  2005/11/07 01:09:47  okubata_ryoma
  SDK_ASSERT_ON_COMPILE����SDK_COMPILER_ASSERT�ɕύX

  Revision 1.3  2005/10/25 01:14:19  yosizaki
  small fix for ARM7.

  Revision 1.2  2005/10/24 10:49:59  yosizaki
  add pulledout-check in CARDi_ReadEnd().

  Revision 1.1  2005/05/12 11:14:59  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

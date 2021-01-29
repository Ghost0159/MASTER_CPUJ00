/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - libraries
  File:     card_spi.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: card_spi.c,v $
  Revision 1.34  2007/11/13 04:22:01  yosizaki
  support erase-SUBSECTOR.

  Revision 1.33  2007/10/04 13:11:37  yosizaki
  add test commands.

  Revision 1.32  2007/09/14 02:57:16  yosizaki
  add CARD_BACKUP_TYPE_FLASH_64MBITS_EX

  Revision 1.31  2007/07/04 08:07:01  yosizaki
  add FLASH 32Mb.

  Revision 1.30  2007/02/20 00:28:12  kitase_hirotake
  indent source

  Revision 1.29  2006/11/16 04:32:05  yosizaki
  change spec of FLASH16M.

  Revision 1.28  2006/05/15 08:51:00  yosizaki
  add status check for FLASH16M/64M

  Revision 1.27  2006/04/28 07:43:27  yosizaki
  add DeviceCaps.

  Revision 1.26  2006/04/10 04:13:50  yosizaki
  add comments.
  small fix.

  Revision 1.25  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.24  2005/11/30 04:03:17  yosizaki
  fix warning.

  Revision 1.23  2005/11/28 02:10:59  yosizaki
  fix about timeout check.

  Revision 1.22  2005/10/13 02:06:59  yosizaki
  change WriteStatus.

  Revision 1.21  2005/09/30 01:09:01  yosizaki
  change CARDi_InitStatusRegister().

  Revision 1.20  2005/09/30 00:51:25  yosizaki
  fix about sector-size.
  change spceification of FLASH 4Mb/8Mb.

  Revision 1.19  2005/09/02 07:35:55  yosizaki
  change CARDi_WaitPrevCommand() to verify NO_RESPONSE.

  Revision 1.18  2005/08/04 15:02:08  yosizaki
  add CARDi_InitStatusRegister().

  Revision 1.17  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.16  2005/02/18 11:20:29  yosizaki
  fix around hidden warnings.

  Revision 1.15  2005/01/31 07:23:26  yosizaki
  add FLASH 4Mb/8Mb.

  Revision 1.14  2005/01/18 09:35:48  yosizaki
  fix page size of EEPROM512kb.

  Revision 1.13  2005/01/14 08:49:08  yosizaki
  remove waste WriteEnable before read-command.
  fix addressing-width (from 3 to 2).

  Revision 1.12  2004/12/13 05:07:42  yosizaki
  add implement of FRAM64kb.

  Revision 1.11  2004/12/08 12:41:58  yosizaki
  remove cardi_device and change CARDi_IdentifyBackupCore.

  Revision 1.10  2004/09/10 10:33:26  yosizaki
  set CARDi_WaitPrevCommand timeout 50ms.

  Revision 1.9  2004/09/06 09:20:49  yosizaki
  fix backup access error at 1st time.

  Revision 1.8  2004/09/02 11:54:42  yosizaki
  fix CARD include directives.

  Revision 1.7  2004/08/23 10:38:10  yosizaki
  add write-command.
  add retry count on writing timeout.

  Revision 1.6  2004/08/18 08:19:02  yosizaki
  change CARDBackupType format.

  Revision 1.5  2004/07/28 13:18:06  yosizaki
  asynchronous mode support.

  Revision 1.4  2004/07/23 08:28:42  yosizaki
  change backup-routine to ARM7 only.

  Revision 1.3  2004/07/19 13:15:42  yosizaki
  add CARDi_ProgramBackupCore.

  Revision 1.2  2004/07/05 08:37:21  yosizaki
  change CardBackupType to CARDBackupType.

  Revision 1.1  2004/06/28 01:52:48  yosizaki
  (none)

 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "../include/card_common.h"
#include "../include/card_spi.h"

/* CARD-SPI �̐��� */


/******************************************************************************/
/* �萔 */

/* reg_MI_MCCNT0 �e�r�b�g*/

#define MCCNT0_SPI_CLK_MASK	0x0003 /* �{�[���[�g�ݒ�}�X�N */
#define MCCNT0_SPI_CLK_4M	0x0000 /* �{�[���[�g 4.19MHz */
#define MCCNT0_SPI_CLK_2M	0x0001 /* �{�[���[�g 2.10MHz */
#define MCCNT0_SPI_CLK_1M	0x0002 /* �{�[���[�g 1.05MHz */
#define MCCNT0_SPI_CLK_524K	0x0003 /* �{�[���[�g 524kHz */
#define MCCNT0_SPI_BUSY		0x0080 /* SPI �r�W�[�t���O */
#define	MMCNT0_SEL_MASK		0x2000 /* CARD ROM / SPI �I���}�X�N */
#define	MMCNT0_SEL_CARD		0x0000 /* CARD ROM �I�� */
#define	MMCNT0_SEL_SPI		0x2000 /* CARD SPI �I�� */
#define MCCNT0_INT_MASK		0x4000 /* �]���������荞�݃}�X�N */
#define MCCNT0_INT_ON		0x4000 /* �]���������荞�ݗL�� */
#define MCCNT0_INT_OFF		0x0000 /* �]���������荞�ݖ��� */
#define MCCNT0_MASTER_MASK	0x8000 /* CARD �}�X�^�[�}�X�N */
#define MCCNT0_MASTER_ON	0x8000 /* CARD �}�X�^�[ ON */
#define MCCNT0_MASTER_OFF	0x0000 /* CARD �}�X�^�[ OFF */


/******************************************************************************/
/* �ϐ� */

typedef struct
{                                      /* SPI �����Ǘ��p�����[�^. */
    u32     rest_comm;                 /* �S���M�o�C�g���̎c��. */
    u32     src;                       /* �]���� */
    u32     dst;                       /* �]���� */
    BOOL    cmp;                       /* ��r���� */
}
CARDiParam;

static CARDiParam cardi_param;


/******************************************************************************/
/* �֐� */

static BOOL CARDi_CommandCheckBusy(void);
static void CARDi_CommArray(const void *src, void *dst, u32 n, void (*func) (CARDiParam *));
static void CARDi_CommReadCore(CARDiParam * p);
static void CARDi_CommWriteCore(CARDiParam * p);
static void CARDi_CommVerifyCore(CARDiParam * p);

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommArrayRead

  Description:  ���[�h�R�}���h�̌㑱�ǂݏo������.

  Arguments:    dst               �ǂݏo���惁����.
                len               �ǂݏo���T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARDi_CommArrayRead(void *dst, u32 len)
{
    CARDi_CommArray(NULL, dst, len, CARDi_CommReadCore);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommArrayWrite

  Description:  ���C�g�R�}���h�̌㑱�������ݏ���.

  Arguments:    dst               �������݌�������.
                len               �������݃T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARDi_CommArrayWrite(const void *src, u32 len)
{
    CARDi_CommArray(src, NULL, len, CARDi_CommWriteCore);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommArrayVerify

  Description:  (��r�̂��߂�)���[�h�R�}���h�̌㑱��r����.

  Arguments:    src               ��r��������.
                len               ��r�T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARDi_CommArrayVerify(const void *src, u32 len)
{
    CARDi_CommArray(src, NULL, len, CARDi_CommVerifyCore);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_EnableSpi

  Description:  CARD-SPI ��L���ɂ���.

  Arguments:    cont              �A���]���t���O. (CSPI_CONTINUOUS_ON or OFF)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARDi_EnableSpi(u32 cont)
{
    /*
     * �����N���b�N�X�s�[�h���x���f�o�C�X���o�ꂵ����
     * MCCNT0_SPI_CLK_4M ���v���p�e�B�z��ɒǉ����ē��I�ύX.
     */
    const u16 ctrl = (u16)(MCCNT0_MASTER_ON |
                           MCCNT0_INT_OFF | MMCNT0_SEL_SPI | MCCNT0_SPI_CLK_4M | cont);
    reg_MI_MCCNT0 = ctrl;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_WaitBusy

  Description:  CARD-SPI �̊�����҂�.

  Arguments:    None

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARDi_WaitBusy(void)
{
    while ((reg_MI_MCCNT0 & MCCNT0_SPI_BUSY) != 0)
    {
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommandBegin

  Description:  �R�}���h���s�J�n�̐錾.

  Arguments:    len               ���s����R�}���h��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_CommandBegin(int len)
{
    cardi_param.rest_comm = (u32)len;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommandEnd

  Description:  �R�}���h���M��������.

  Arguments:    force_wait     �����E�F�C�g���� [ms]
                timeout        �^�C���A�E�g���� [ms]

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_CommandEnd(u32 force_wait, u32 timeout)
{
    if (force_wait + timeout > 0)
    {
        /*
         * �����E�F�C�g���Ԃ�����΂��̕������ҋ@.
         * �ŏ��̘b�ƈ���� FLASH ���E�F�C�g���������ꂽ.
         */
        if (force_wait > 0)
        {
            OS_Sleep(force_wait);
        }
        if (timeout > 0)
        {
            /*
             * PageWrite �R�}���h�̂݁u�O�����������E�F�C�g�v
             * �Ȃ̂ł��̂悤�ȓ���ȃ��[�v�ɂȂ�.
             */
            int     rest = (int)(timeout - force_wait);
            while (!CARDi_CommandCheckBusy() && (rest > 0))
            {
                int     interval = (rest < 5) ? rest : 5;
                OS_Sleep((u32)interval);
                rest -= interval;
            }
        }
        /*
         * ���̑��̃R�}���h�͎w�莞�ԃE�F�C�g�ς݂Ȃ̂�
         * ReadStatus �� 1 �񂾂��ŗǂ�.
         */
        /* �����܂ł��ăr�W�[�Ȃ�^�C���A�E�g */
        if (!CARDi_CommandCheckBusy())
        {
            cardi_common.cmd->result = CARD_RESULT_TIMEOUT;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommandReadStatus

  Description:  ���[�h�X�e�[�^�X

  Arguments:    None

  Returns:      �X�e�[�^�X�l
 *---------------------------------------------------------------------------*/
u8 CARDi_CommandReadStatus(void)
{
    const u8    buf = COMM_READ_STATUS;
    u8          dst;
    CARDi_CommandBegin(1 + 1);
    CARDi_CommArrayWrite(&buf, 1);
    CARDi_CommArrayRead(&dst, 1);
    CARDi_CommandEnd(0, 0);
    return dst;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommandCheckBusy

  Description:  �f�o�C�X�̃r�W�[���������ꂽ�����[�h�X�e�[�^�X�R�}���h�Ŕ���.

  Arguments:    None

  Returns:      �r�W�[�łȂ���� TRUE.
 *---------------------------------------------------------------------------*/
static BOOL CARDi_CommandCheckBusy(void)
{
    return ((CARDi_CommandReadStatus() & COMM_STATUS_WIP_BIT) == 0);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_WaitPrevCommand

  Description:  �R�}���h���s�O�̃r�W�[�`�F�b�N.
                �O��̃R�}���h�ɂ���ăr�W�[��Ԃł���� 50[ms] �܂ő҂�.
                (���C�u�����̎����ƃJ�[�h�̐ڐG������Ȍ���ʏ킠�肦�Ȃ�)

  Arguments:    None.

  Returns:      �R�}���h�����������s�\�ł���� TRUE.
 *---------------------------------------------------------------------------*/
static BOOL CARDi_WaitPrevCommand(void)
{
    CARDi_CommandEnd(0, 50);
    /* �����Ń^�C���A�E�g�̏ꍇ�͖����� */
    if (cardi_common.cmd->result == CARD_RESULT_TIMEOUT)
    {
        cardi_common.cmd->result = CARD_RESULT_NO_RESPONSE;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommArray

  Description:  �R�}���h���s�̋��ʏ���.

  Arguments:    src               �������̃�����. (�s�g�p�Ȃ�NULL)
                dst               ������̃�����. (�s�g�p�Ȃ�NULL)
                len               �����T�C�Y.
                func              �������e.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_CommArray(const void *src, void *dst, u32 len, void (*func) (CARDiParam *))
{
    CARDiParam *const p = &cardi_param;
    p->src = (u32)src;
    p->dst = (u32)dst;
    CARDi_EnableSpi(CSPI_CONTINUOUS_ON);
    for (; len > 0; --len)
    {
        if (!--p->rest_comm)
        {
            CARDi_EnableSpi(CSPI_CONTINUOUS_OFF);
        }
        CARDi_WaitBusy();
        (*func) (p);
    }
    if (!p->rest_comm)
    {
        reg_MI_MCCNT0 = (u16)(MCCNT0_MASTER_OFF | MCCNT0_INT_OFF);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommReadCore

  Description:  1�o�C�g���[�h����.

  Arguments:    p                 �R�}���h�p�����[�^.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_CommReadCore(CARDiParam * p)
{
    reg_MI_MCD0 = 0;
    CARDi_WaitBusy();
    MI_WriteByte((void *)p->dst, (u8)reg_MI_MCD0);
    ++p->dst;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommWriteCore

  Description:  1�o�C�g���C�g����.

  Arguments:    p                 �R�}���h�p�����[�^.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_CommWriteCore(CARDiParam * p)
{
    vu16    tmp;
    reg_MI_MCD0 = (u16)MI_ReadByte((void *)p->src);
    ++p->src;
    CARDi_WaitBusy();
    tmp = reg_MI_MCD0;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CommVerifyCore

  Description:  1�o�C�g��r����.

  Arguments:    p                 �R�}���h�p�����[�^.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_CommVerifyCore(CARDiParam * p)
{
    reg_MI_MCD0 = 0;
    CARDi_WaitBusy();
    /*
     * ���[�h���Ĉ�v���Ȃ���Β��f.
     * �������A���]���𔲂��Ȃ���΂����Ȃ��̂�
     * ���Ȃ��Ƃ� 1 �񂾂��]�v�ɓǂޕK�v������.
     */
    if ((u8)reg_MI_MCD0 != MI_ReadByte((void *)p->src))
    {
        p->cmp = FALSE;
        if (p->rest_comm > 1)
        {
            p->rest_comm = 1;
        }
    }
    ++p->src;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_WriteEnable

  Description:  �f�o�C�X�̏������ݑ���L����. (Write �n�R�}���h�̑O�ɖ���K�v)

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_WriteEnable(void)
{
    static const u8 arg[1] = { COMM_WRITE_ENABLE, };
    CARDi_CommandBegin(sizeof(arg));
    CARDi_CommArrayWrite(arg, sizeof(arg));
    CARDi_CommandEnd(0, 0);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_SendSpiAddressingCommand

  Description:  �A�h���X�w��R�}���h�̐ݒ�.

  Arguments:    addr              �{���ΏۂƂȂ�f�o�C�X��̃A�h���X.
                mode              ���s����R�}���h.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_SendSpiAddressingCommand(u32 addr, u32 mode)
{
    const u32 width = cardi_common.cmd->spec.addr_width;
    u32     addr_cmd;
    switch (width)
    {
    case 1:
        /* 4kbit �f�o�C�X�� [A:8] ���R�}���h�̈ꕔ�ɂȂ� */
        addr_cmd = (u32)(mode | ((addr >> 5) & 0x8) | ((addr & 0xFF) << 8));
        break;
    case 2:
        addr_cmd = (u32)(mode | (addr & 0xFF00) | ((addr & 0xFF) << 16));
        break;
    case 3:
        addr_cmd = (u32)(mode |
                         ((addr >> 8) & 0xFF00) | ((addr & 0xFF00) << 8) | ((addr & 0xFF) << 24));
        break;
    default:
        SDK_ASSERT(FALSE);
        break;
    }
    CARDi_CommArrayWrite(&addr_cmd, (u32)(1 + width));
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_InitStatusRegister

  Description:  FRAM/EEPROM �̏ꍇ, �N�����ɃX�e�[�^�X���W�X�^�̕␳�������s��.
                �EFRAM �͓d���������Ƀ��C�g�v���e�N�g�̕ω����N���肤�邽��.
                �EEEPROM �͔[�����ɏ����l�s�������肤�邽��.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_InitStatusRegister(void)
{
    /*
     * �X�e�[�^�X���W�X�^���ُ�ȏ����l����肤��f�o�C�X�ɑ΂��Ă�
     * ����g�p���ɓK�X�␳����.
     */
    const u8 stat = cardi_common.cmd->spec.initial_status;
    if (stat != 0xFF)
    {
        static BOOL status_checked = FALSE;
        if (!status_checked)
        {
            if (CARDi_CommandReadStatus() != stat)
            {
                CARDi_SetWriteProtectCore(stat);
            }
            status_checked = TRUE;
        }
    }
}


/********************************************************************/
/*
 * �������ڏ���.
 * ���̒i�K�ł͂��łɔr���⃊�N�G�X�g�����S�Ċ������Ă���.
 * �T�C�Y�̐����ɂ��Ă����łɍl���ς݂̂��̂Ƃ���.
 */

/*---------------------------------------------------------------------------*
  Name:         CARDi_IdentifyBackupCore

  Description:  �f�o�C�X�^�C�v����.

  Arguments:    type              ���肷��f�o�C�X�^�C�v.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_IdentifyBackupCore(CARDBackupType type)
{
    /*
     * �擾�����p�����[�^�� CARDiCommandArg �֕ۑ�.
     * ����͍ŏI�I�Ƀe�[�u����p�~���邱�ƂŊ�������.
     */
    {
        CARDiCommandArg *const p = cardi_common.cmd;

        /* �܂��S�p�����[�^���N���A���� NOT_USE ��Ԃɂ��� */
        MI_CpuFill8(&p->spec, 0, sizeof(p->spec));
        p->type = type;
        p->spec.caps = (CARD_BACKUP_CAPS_AVAILABLE | CARD_BACKUP_CAPS_READ_STATUS);
        if (type != CARD_BACKUP_TYPE_NOT_USE)
        {
            /*
             * �f�o�C�X�^�C�v, ���e��, �x���_�� type ����擾�\.
             * �x���_�ԍ���, ����^�C�v�ŕ������[�J�[���̗p����
             * ���s��Ȃǂ̗��R�ŋ�ʂ���K�v���������ꍇ�ȊO 0.
             */
            const u32 size = (u32)(1 << ((type >> CARD_BACKUP_TYPE_SIZEBIT_SHIFT) &
                                         CARD_BACKUP_TYPE_SIZEBIT_MASK));
            const int device =
                ((type >> CARD_BACKUP_TYPE_DEVICE_SHIFT) & CARD_BACKUP_TYPE_DEVICE_MASK);
            const int vender =
                ((type >> CARD_BACKUP_TYPE_VENDER_SHIFT) & CARD_BACKUP_TYPE_VENDER_MASK);

            p->spec.total_size = size;
            /* �X�e�[�^�X���W�X�^�̕␳���s�v�Ȃ� 0xFF. (���ꂪ�ʏ�) */
            p->spec.initial_status = 0xFF;
            if (device == CARD_BACKUP_TYPE_DEVICE_EEPROM)
            {
                switch (size)
                {
                default:
                    goto invalid_type;
                case 0x000200:        // CARD_BACKUP_TYPE_EEPROM_4KBITS
                    p->spec.page_size = 0x10;
                    p->spec.addr_width = 1;
                    p->spec.program_page = 5;
                    p->spec.initial_status = 0xF0;
                    break;
                case 0x002000:        // CARD_BACKUP_TYPE_EEPROM_64KBITS
                    p->spec.page_size = 0x0020;
                    p->spec.addr_width = 2;
                    p->spec.program_page = 5;
                    p->spec.initial_status = 0x00;
                    break;
                case 0x010000:        // CARD_BACKUP_TYPE_EEPROM_512KBITS
                    p->spec.page_size = 0x0080;
                    p->spec.addr_width = 2;
                    p->spec.program_page = 10;
                    p->spec.initial_status = 0x00;
                    break;
/*
				case 0x020000:	// CARD_BACKUP_TYPE_EEPROM_1MBITS
					p->spec.page_size = 0x0080;
					p->spec.addr_width = 3;
					p->spec.program_page = 10;
					break;
*/
                }
                p->spec.sect_size = p->spec.page_size;
                p->spec.caps |= CARD_BACKUP_CAPS_READ;
                p->spec.caps |= CARD_BACKUP_CAPS_PROGRAM;
                p->spec.caps |= CARD_BACKUP_CAPS_VERIFY;
                p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
            }
            else if (device == CARD_BACKUP_TYPE_DEVICE_FLASH)
            {
                switch (size)
                {
                default:
                    goto invalid_type;
                case 0x040000:        // CARD_BACKUP_TYPE_FLASH_2MBITS
                case 0x080000:        // CARD_BACKUP_TYPE_FLASH_4MBITS
                case 0x100000:        // CARD_BACKUP_TYPE_FLASH_8MBITS
                    p->spec.write_page = 25;
                    p->spec.write_page_total = 300;
                    p->spec.erase_page = 300;
                    p->spec.erase_sector = 5000;
                    p->spec.caps |= CARD_BACKUP_CAPS_WRITE;
                    p->spec.caps |= CARD_BACKUP_CAPS_ERASE_PAGE;
                    break;
                case 0x200000:        // CARD_BACKUP_TYPE_FLASH_16MBITS
                    p->spec.write_page = 23;
                    p->spec.write_page_total = 300;
                    p->spec.erase_sector = 500;
                    p->spec.erase_sector_total = 5000;
                    p->spec.erase_chip = 10000;
                    p->spec.erase_chip_total = 60000;
                    p->spec.initial_status = 0x00;
                    p->spec.caps |= CARD_BACKUP_CAPS_WRITE;
                    p->spec.caps |= CARD_BACKUP_CAPS_ERASE_PAGE;
                    p->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                    p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
                    break;
                case 0x400000:        // CARD_BACKUP_TYPE_FLASH_32MBITS
                    p->spec.erase_sector = 600;
                    p->spec.erase_sector_total = 3000;
                    p->spec.erase_subsector = 70;
                    p->spec.erase_subsector_total = 150;
                    p->spec.erase_chip = 23000;
                    p->spec.erase_chip_total = 800000;
                    p->spec.initial_status = 0x00;
                    p->spec.subsect_size = 0x1000;
                    p->spec.caps |= CARD_BACKUP_CAPS_ERASE_SUBSECTOR;
                    p->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                    p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
                    break;
                case 0x800000:
                    if (vender == 0)  // CARD_BACKUP_TYPE_FLASH_64MBITS
                    {
                        p->spec.erase_sector = 1000;
                        p->spec.erase_sector_total = 3000;
                        p->spec.erase_chip = 68000;
                        p->spec.erase_chip_total = 160000;
                        p->spec.initial_status = 0x00;
                        p->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                        p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
                    }
                    else if (vender == 1)   // CARD_BACKUP_TYPE_FLASH_64MBITS_EX
                    {
                        p->spec.erase_sector = 1000;
                        p->spec.erase_sector_total = 3000;
                        p->spec.erase_chip = 68000;
                        p->spec.erase_chip_total = 160000;
                        p->spec.initial_status = 0x84;
                        p->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                        p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
                    }
                    break;
                }
                p->spec.sect_size = 0x010000;
                p->spec.page_size = 0x0100;
                p->spec.addr_width = 3;
                p->spec.program_page = 5;
                p->spec.caps |= CARD_BACKUP_CAPS_READ;
                p->spec.caps |= CARD_BACKUP_CAPS_PROGRAM;
                p->spec.caps |= CARD_BACKUP_CAPS_VERIFY;
                p->spec.caps |= CARD_BACKUP_CAPS_ERASE_SECTOR;
            }
            else if (device == CARD_BACKUP_TYPE_DEVICE_FRAM)
            {
                switch (size)
                {
                default:
                    goto invalid_type;
                case 0x002000:        // #CARD_BACKUP_TYPE_FRAM_64KBITS
                case 0x008000:        // #CARD_BACKUP_TYPE_FRAM_256KBITS
                    break;
                }
                p->spec.page_size = size;
                p->spec.sect_size = size;
                p->spec.addr_width = 2;
                p->spec.initial_status = 0x00;
                p->spec.caps |= CARD_BACKUP_CAPS_READ;
                p->spec.caps |= CARD_BACKUP_CAPS_PROGRAM;
                p->spec.caps |= CARD_BACKUP_CAPS_VERIFY;
                p->spec.caps |= CARD_BACKUP_CAPS_WRITE_STATUS;
            }
            else
            {
              invalid_type:
                p->type = CARD_BACKUP_TYPE_NOT_USE;
                p->spec.total_size = 0;
                cardi_common.cmd->result = CARD_RESULT_UNSUPPORTED;
                return;
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadBackupCore

  Description:  �f�o�C�X�ւ̃��[�h�R�}���h�S��.

  Arguments:    src               �ǂݏo�����̃f�o�C�X�I�t�Z�b�g.
                dst               �ǂݏo����̃������A�h���X.
                len               �ǂݏo���T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_ReadBackupCore(u32 src, void *dst, u32 len)
{
    if (CARDi_WaitPrevCommand())
    {
        CARDiCommandArg *const cmd = cardi_common.cmd;
        /* ���[�h�Ɋւ��Ă̓y�[�W���E�̐����������̂ňꊇ���� */
        CARDi_CommandBegin((int)(1 + cmd->spec.addr_width + len));
        CARDi_SendSpiAddressingCommand(src, COMM_READ_ARRAY);
        CARDi_CommArrayRead(dst, len);
        CARDi_CommandEnd(0, 0);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_ProgramBackupCore

  Description:  �f�o�C�X�ւ̃v���O����(����������������)�R�}���h�S��.

  Arguments:    dst               �������ݐ�̃f�o�C�X�I�t�Z�b�g.
                src               �������݌��̃������A�h���X.
                len               �������݃T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_ProgramBackupCore(u32 dst, const void *src, u32 len)
{
    if (CARDi_WaitPrevCommand())
    {
        CARDiCommandArg *const cmd = cardi_common.cmd;
        /* ���C�g�̓y�[�W���E���܂����Ȃ��悤�ɕ������� */
        const u32 page = cmd->spec.page_size;
        while (len > 0)
        {
            const u32 mod = (u32)(dst & (page - 1));
            u32     size = page - mod;
            if (size > len)
            {
                size = len;
            }
            CARDi_WriteEnable();
            CARDi_CommandBegin((int)(1 + cmd->spec.addr_width + size));
            CARDi_SendSpiAddressingCommand(dst, COMM_PROGRAM_PAGE);
            CARDi_CommArrayWrite(src, size);
            CARDi_CommandEnd(cmd->spec.program_page, 0);
            if (cmd->result != CARD_RESULT_SUCCESS)
            {
                break;
            }
            src = (const void *)((u32)src + size);
            dst += size;
            len -= size;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_WriteBackupCore

  Description:  �f�o�C�X�ւ̃��C�g(���� + �v���O����)�R�}���h�S��.

  Arguments:    dst               �������ݐ�̃f�o�C�X�I�t�Z�b�g.
                src               �������݌��̃������A�h���X.
                len               �������݃T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_WriteBackupCore(u32 dst, const void *src, u32 len)
{
    if (CARDi_WaitPrevCommand())
    {
        CARDiCommandArg *const cmd = cardi_common.cmd;
        /* ���C�g�̓y�[�W���E���܂����Ȃ��悤�ɕ������� */
        const u32 page = cmd->spec.page_size;
        while (len > 0)
        {
            const u32 mod = (u32)(dst & (page - 1));
            u32     size = page - mod;
            if (size > len)
            {
                size = len;
            }
            CARDi_WriteEnable();
            CARDi_CommandBegin((int)(1 + cmd->spec.addr_width + size));
            CARDi_SendSpiAddressingCommand(dst, COMM_PAGE_WRITE);
            CARDi_CommArrayWrite(src, size);
            CARDi_CommandEnd(cmd->spec.write_page, cmd->spec.write_page_total);
            if (cmd->result != CARD_RESULT_SUCCESS)
            {
                break;
            }
            src = (const void *)((u32)src + size);
            dst += size;
            len -= size;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_VerifyBackupCore

  Description:  �f�o�C�X�ւ̃x���t�@�C(���ۂ̓��[�h + ��r����)�R�}���h�S��.

  Arguments:    dst               ��r��̃f�o�C�X�I�t�Z�b�g.
                src               ��r���̃������A�h���X.
                len               ��r�T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_VerifyBackupCore(u32 dst, const void *src, u32 len)
{
    if (CARDi_WaitPrevCommand())
    {
        CARDiCommandArg *const cmd = cardi_common.cmd;
        /* ���[�h�Ɋւ��Ă̓y�[�W���E�̐����������̂ňꊇ���� */
        cardi_param.cmp = TRUE;
        CARDi_CommandBegin((int)(1 + cmd->spec.addr_width + len));
        CARDi_SendSpiAddressingCommand(dst, COMM_READ_ARRAY);
        CARDi_CommArrayVerify(src, len);
        CARDi_CommandEnd(0, 0);
        if ((cmd->result == CARD_RESULT_SUCCESS) && !cardi_param.cmp)
        {
            cmd->result = CARD_RESULT_FAILURE;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_EraseBackupSectorCore

  Description:  �f�o�C�X�ւ̃Z�N�^�����R�}���h�S��.

  Arguments:    dst               ������̃f�o�C�X�I�t�Z�b�g.
                len               �����T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_EraseBackupSectorCore(u32 dst, u32 len)
{
    CARDiCommandArg *const cmd = cardi_common.cmd;
    const u32 sector = cmd->spec.sect_size;
    /* �����͈͂��Z�N�^�̐����{�ɐ������Ă��Ȃ��ꍇ�͏��������Ȃ� */
    if (((dst | len) & (sector - 1)) != 0)
    {
        cmd->result = CARD_RESULT_INVALID_PARAM;
    }
    else if (CARDi_WaitPrevCommand())
    {
        /* �Z�N�^���E�P�ʂŏ��� */
        while (len > 0)
        {
            CARDi_WriteEnable();
            CARDi_CommandBegin((int)(1 + cmd->spec.addr_width + 0));
            CARDi_SendSpiAddressingCommand(dst, COMM_SECTOR_ERASE);
            CARDi_CommandEnd(cmd->spec.erase_sector, cmd->spec.erase_sector_total);
            if (cmd->result != CARD_RESULT_SUCCESS)
            {
                break;
            }
            dst += sector;
            len -= sector;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_EraseBackupSubSectorCore

  Description:  �f�o�C�X�ւ̃T�u�Z�N�^�����R�}���h�S��.

  Arguments:    dst               ������̃f�o�C�X�I�t�Z�b�g.
                len               �����T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_EraseBackupSubSectorCore(u32 dst, u32 len)
{
    CARDiCommandArg *const cmd = cardi_common.cmd;
    const u32 sector = cmd->spec.subsect_size;
    /* �����͈͂��T�u�Z�N�^�̐����{�ɐ������Ă��Ȃ��ꍇ�͏��������Ȃ� */
    if (((dst | len) & (sector - 1)) != 0)
    {
        cmd->result = CARD_RESULT_INVALID_PARAM;
    }
    else if (CARDi_WaitPrevCommand())
    {
        /* �Z�N�^���E�P�ʂŏ��� */
        while (len > 0)
        {
            CARDi_WriteEnable();
            CARDi_CommandBegin((int)(1 + cmd->spec.addr_width + 0));
            CARDi_SendSpiAddressingCommand(dst, COMM_SUBSECTOR_ERASE);
            CARDi_CommandEnd(cmd->spec.erase_subsector, cmd->spec.erase_subsector_total);
            if (cmd->result != CARD_RESULT_SUCCESS)
            {
                break;
            }
            dst += sector;
            len -= sector;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_EraseChipCore

  Description:  �f�o�C�X�ւ̃`�b�v�����R�}���h�S��.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_EraseChipCore(void)
{
    if (CARDi_WaitPrevCommand())
    {
        CARDiCommandArg *const cmd = cardi_common.cmd;
        static const u8 arg[1] = { COMM_CHIP_ERASE, };
        CARDi_WriteEnable();
        CARDi_CommandBegin(sizeof(arg));
        CARDi_CommArrayWrite(arg, sizeof(arg));
        CARDi_CommandEnd(cmd->spec.erase_chip, cmd->spec.erase_chip_total);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_SetWriteProtectCore

  Description:  �f�o�C�X�ւ̃��C�g�v���e�N�g�R�}���h�S��.

  Arguments:    stat              �ݒ肷��v���e�N�g�t���O.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_SetWriteProtectCore(u16 stat)
{
    if (CARDi_WaitPrevCommand())
    {
        CARDiCommandArg *const cmd = cardi_common.cmd;
        int     retry_count = 10;
        u8      arg[2];
        arg[0] = COMM_WRITE_STATUS;
        arg[1] = (u8)stat;
        do
        {
            CARDi_WriteEnable();
            CARDi_CommandBegin(1 + 1);
            CARDi_CommArrayWrite(&arg, sizeof(arg));
            CARDi_CommandEnd(5, 0);
        }
        while ((cmd->result == CARD_RESULT_TIMEOUT) && (--retry_count > 0));
    }
}


#if	0

/********************************************************************/
/********************************************************************/
/* ���ؒ��f�o�C�X�ŗL�R�}���h ***************************************/
/********************************************************************/
/********************************************************************/


/* ID ���[�h */
static void CARDi_ReadIdCore(void)
{
    /*
     * EEPROM, FRAM �ɂ͂��̃R�}���h������.
     * ST �� 2Mbit FLASH �ɂ����̃R�}���h������?
     * �Ή��O�̃R�}���h�o�C�g���o���̓����`����������Ȃ�.
     * �Ƃ����, ���̃R�}���h�� 2Mbit ���͏�Ɏg�p�s�Ƃ��邩
     * CARD_BACKUP_TYPE_FLASH_2MBITS_SANYO �Ȃǂƍו������邩.
     * ������ɂ��悻���܂Ŏ��O�ɋ敪���Ă���̂�
     * ID ���K�v�ɂȂ�V�[��������̂��낤��...?
     * �]�͂�����Γ����Ő���������Ɏg�p�\��.
     */
    cardi_common.cmd->result = CARD_RESULT_UNSUPPORTED;
}

/* 2M FLASH �̂ݎg�p�\�ȃR�}���h **********************************/

/* �y�[�W���� (FLASH) */
static void CARDi_ErasePageCore(u32 dst)
{
    CARDi_WriteEnable();
    CARDi_CommandBegin(1 + cardi_common.cmd->spec.addr_width + 0);
    CARDi_SendSpiAddressingCommand(dst, COMM_PAGE_ERASE);
    CARDi_CommandEnd(cardi_common.cmd->spec.erase_page, 0);
}


#endif

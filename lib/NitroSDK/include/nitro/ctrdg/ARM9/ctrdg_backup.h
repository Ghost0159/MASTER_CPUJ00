/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CTRDG - include
  File:     ctrdg_backup.h

  Copyright 2001,2003,2004,2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ctrdg_backup.h,v $
  Revision 1.4  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.3  2006/04/07 07:56:11  okubata_ryoma
  ���ӃR�����g�ǉ�

  Revision 1.2  2006/04/07 03:29:03  okubata_ryoma
  �R�����g�̏C��

  Revision 1.1  2006/04/05 10:34:15  okubata_ryoma
  AGB�o�b�N�A�b�v���C�u������SDK���^�̂��߂̕ύX


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_CTRDG_BACKUP_H_
#define NITRO_CTRDG_BACKUP_H_

#include <nitro.h>
#include "ctrdg_task.h"
#include "ctrdg_flash.h"
#include "ctrdg_sram.h"

// define data----------------------------------
#define CTRDG_BACKUP_PHASE_VERIFY           0x0000
#define CTRDG_BACKUP_PHASE_PROGRAM          0x0001
#define CTRDG_BACKUP_PHASE_SECTOR_ERASE     0x0002
#define CTRDG_BACKUP_PHASE_CHIP_ERASE       0x0003
#define CTRDG_BACKUP_PHASE_VERIFY_ERASE     0x0004
#define CTRDG_BACKUP_PHASE_PARAMETER_CHECK  0x00ff

#define CTRDG_BACKUP_RESULT_OK              0x0000
#define CTRDG_BACKUP_RESULT_ERROR           0x8000
#define CTRDG_BACKUP_RESULT_TIMEOUT         0x4000
#define CTRDG_BACKUP_RESULT_Q5TIMEOUT       0x2000
#define CTRDG_BACKUP_RESULT_PULLEDOUT       0x1000

/* �f�o�b�O�p */
//#ifndef   __FLASH_DEBUG
#define CTRDG_BACKUP_VENDER_ID_ADR      (CTRDG_AGB_FLASH_ADR+0x00000000)
#define CTRDG_BACKUP_DEVICE_ID_ADR      (CTRDG_AGB_FLASH_ADR+0x00000001)
#define CTRDG_BACKUP_COM_ADR1           (CTRDG_AGB_FLASH_ADR+0x00005555)
#define CTRDG_BACKUP_COM_ADR2           (CTRDG_AGB_FLASH_ADR+0x00002aaa)
//#else
//#define   VENDER_ID_ADR       0x02003ffc
//#define   DEVICE_ID_ADR       0x02003ffd
//#define   COM_ADR1            0x02003ffe
//#define COM_ADR2          0x02003fff
//#endif

#define CTRDG_BACKUP_MEGA_512K_ID       0x1cc2
#define CTRDG_BACKUP_MEGA_1M_ID         0x09c2
//#define   CTRDG_BACKUP_MEGA_1M_ID         0x1cc2

// data type declaration------------------------
typedef struct CTRDGiFlashTypePlusTag
{
    u16     (*CTRDGi_WriteAgbFlashSector) (u16 secNo, u8 *src);
    u16     (*CTRDGi_EraseAgbFlashChip) (void);
    u16     (*CTRDGi_EraseAgbFlashSector) (u16 secNo);
    void    (*CTRDGi_WriteAgbFlashSectorAsync) (u16 secNo, u8 *src, CTRDG_TASK_FUNC callback);
    void    (*CTRDGi_EraseAgbFlashChipAsync) (CTRDG_TASK_FUNC callback);
    void    (*CTRDGi_EraseAgbFlashSectorAsync) (u16 secNo, CTRDG_TASK_FUNC callback);
    u16     (*CTRDGi_PollingSR) (u16 phase, u8 *adr, u16 lastData);
    const u16 (*maxtime);
    CTRDGFlashType type;
}
CTRDGiFlashTypePlus;

typedef enum
{
    CTRDG_BACKUP_TYPE_FLASH_512K,      /* 512k�t���b�V�� */
    CTRDG_BACKUP_TYPE_FLASH_1M,        /* 1M�t���b�V�� */
    CTRDG_BACKUP_TYPE_SRAM             /* 256kSRAM */
}
CTRDGBackupType;

// extern variables declaration----------------------
extern u16 ctrdg_flash_remainder;
extern const u16 (*ctrdgi_fl_maxtime);
extern u16 (*CTRDGi_PollingSR) (u16 phase, u8 *adr, u16 lastData);

// function declaration-------------------------
extern const CTRDGFlashType *AgbFlash;
extern u16 (*CTRDGi_WriteAgbFlashSector) (u16 secNo, u8 *src);
extern u16 (*CTRDGi_EraseAgbFlashChip) (void);
extern u16 (*CTRDGi_EraseAgbFlashSector) (u16 secNo);
extern void (*CTRDGi_WriteAgbFlashSectorAsync) (u16 secNo, u8 *src, CTRDG_TASK_FUNC callback);
extern void (*CTRDGi_EraseAgbFlashChipAsync) (CTRDG_TASK_FUNC callback);
extern void (*CTRDGi_EraseAgbFlashSectorAsync) (u16 secNo, CTRDG_TASK_FUNC callback);
extern u16 (*pollingSR) (u16 phase, u8 *adr, u16 lastData);

/*------------------------------------------------------------------*/
/*          �f�o�C�X�̎��ʂ���я����ݒ�                            */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_IdentifyAgbBackup
                
                �Ή�����AGB���̊֐��Fextern u16 IdentifyFlash_512K()
                �Ή�����AGB���̊֐��Fextern u16 IdentifyFlash_1M()
                
  Description:  NITRO-CTRDG �ɓ��ڂ���Ă���o�b�N�A�b�v�f�o�C�X���w�肵�܂��B
                �o�b�N�A�b�v�f�o�C�X���t���b�V���̏ꍇ�̓t���b�V���� ID ��ǂݏo���A
                �ǂ̃t���b�V�����J�[�g���b�W�ɓ��ڂ���Ă��邩�����ʂ��āA
                �t���b�V���̗e�ʂ�Z�N�^�T�C�Y�̎擾�A�A�N�Z�X�X�s�[�h�̐ݒ�A
                �X�ɑΉ�����t���b�V���p�̊e�A�N�Z�X�֐��̃Z�b�g���s���܂��B
                �擾�����t���b�V���̃f�[�^�̓O���[�o���ϐ� CTRDGFlashType *AgbFlash �ŎQ�Ƃ��邱�Ƃ��ł��܂��B
                (CTRDGFlashType�̏ڍׂ͏�L��`�����Q�ƁB)
                
                �{�֐��̓o�b�N�A�b�v�f�o�C�X�ɃA�N�Z�X������ȑO�i���[�h���܂߂āj�ɂP��R�[������K�v������܂��B
                �f�o�C�X�����ʂł��Ȃ������ꍇ�̓G���[��Ԃ��A�ȉ��̃A�N�Z�X�֐��͎g�p�s�ƂȂ�܂��B
                
                �A�N�Z�X�T�C�N���̐ݒ�͊֐����ōs���܂��̂ŁA���O�ɍs���K�v�͂���܂���B
                ���̊֐��ł̓^�C���A�E�g����p�Ƀ`�b�N���g�p���܂��̂Ŏ��O�� 
                OS_InitTick �֐����Ă�ł����K�v������܂��B

                �����̊֐����ł͈����ԑS�Ă̊��荞�݂��֎~����A�J�[�g���b�W�o�X�����b�N����܂��̂Œ��ӂ��ĉ������B
                ���ɁA�{�֐��R�[�����̓_�C���N�g�T�E���h����тu�E�g�u�����N�����A�\�������A
                �J�[�g���b�W���N�G�X�g���̓���̃^�C�~���O�Ŏ����N������c�l�`�͎g�p���Ȃ��ŉ������B
                
                �܂�������CTRDG_BACKUP_TYPE_FLASH_512K�܂��́ACTRDG_BACKUP_TYPE_FLASH_1M��^�����ꍇ�A
                �f�o�C�X�ւ̏������ݓ��삪�������܂��̂ŁA�^��������ƈقȂ��ނ̃f�o�C�X��NITRO-CTRDG�ɓ��ڂ���Ă����
                �o�b�N�A�b�v�f�[�^���j�󂳂�邱�Ƃ�����܂��̂Œ��ӂ��Ă��������B


  Arguments:    type  : NITRO-CTRDG�ɓ��ڂ���Ă���o�b�N�A�b�v�f�o�C�X�̎��

  Returns:      0     : ����I��
                0�ȊO : ���ʃG���[
 *---------------------------------------------------------------------------*/
extern u16 CTRDG_IdentifyAgbBackup(CTRDGBackupType type);

#endif // NITRO_CTRDG_BACKUP_H_

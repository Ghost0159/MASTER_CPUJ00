/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CTRDG - include
  File:     ctrdg_flash.h

  Copyright 2003,2004,2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ctrdg_flash.h,v $
  Revision 1.3  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.2  2006/04/07 03:28:43  okubata_ryoma
  �R�����g�̏C��

  Revision 1.1  2006/04/05 10:34:15  okubata_ryoma
  AGB�o�b�N�A�b�v���C�u������SDK���^�̂��߂̕ύX


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_CTRDG_FLASH_H_
#define NITRO_CTRDG_FLASH_H_

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif

/* definition ****************************************************************/

/* �t���b�V���擪�A�h���X */
#define CTRDG_AGB_FLASH_ADR           0x0A000000

/* CTRDG_WriteAndVerifyAgbFlash�֐����g���C�񐔏�� */
#define CTRDG_AGB_FLASH_RETRY_MAX     3


typedef struct CTRDGiFlashSectorTag
{
    u32     size;                      /* �Z�N�^�T�C�Y */
    u16     shift;                     /* �Z�N�^�T�C�Y���V�t�g���Ɋ��Z�����l */
    u16     count;                     /* �Z�N�^�� */
    u16     top;                       /* �g�p�\�擪�Z�N�^No. */
    u8      reserved[2];               /* �p�f�B���O */
}
CTRDGiFlashSector;

typedef struct CTRDGFlashTypeTag
{
    u32     romSize;                   /* ROM�T�C�Y */
    CTRDGiFlashSector sector;          /* �Z�N�^���(����LCTRDGiFlashSector�Q�ƁB) */
    MICartridgeRamCycle agbWait[2];    /* AGB�J�[�g���b�W�o�XRAM�̈��read/write�E�F�C�g�l */
    u16     makerID;                   /* �x���_ID */
    u16     deviceID;                  /* �f�o�C�XID */
}
CTRDGFlashType;

extern void CTRDGi_SetFlashBankMx(u16 bank);
extern u16 CTRDGi_ReadFlashID(void);
extern void StartFlashTimer(u16 phase);
extern void CheckFlashTimer(void);
extern void CTRDGi_SetFlashBankMx(u16 bank);
extern u16 CTRDGi_PollingSR512kCOMMON(u16 phase, u8 *adr, u16 lastData);
extern u16 CTRDGi_PollingSR1MCOMMON(u16 phase, u8 *adr, u16 lastData);

/*------------------------------------------------------------------*/
/*          �O���[�o���ϐ�                                          */
/*------------------------------------------------------------------*/

/*
 * �t���b�V���̍\�����e�������f�[�^�ւ̃|�C���^�B
 * �i�ڍׂ͏�L�f�[�^��`���Q��)
 */
extern const CTRDGFlashType *AgbFlash;

/*
 * �t���b�V�����C�g���̐i���������o�����[�^
 * ��ctrdg_flash_remainder�́A�A�g�������t���b�V���ł�128���������A
 *   ���̑��̃f�o�C�X�ł�1���������܂��B
 */
extern u16 ctrdg_flash_remainder;

/*------------------------------------------------------------------*/
/*          �f�[�^�ǂݏo��                                          */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_ReadAgbFlash
                CTRDG_ReadAgbFlashAsync
  
                �Ή�����AGB���̊֐��Fextern void ReadFlash(u16 secNo,u32 offset,u8 *dst,u32 size)

  Description:  �t���b�V���̑ΏۃZ�N�^�����ɂ����� offset �o�C�g��̃A�h���X����A
                size �o�C�g���̃f�[�^�����[�N�� dst �A�h���X�ȍ~�ɓǂݏo���܂��B
                
                �A�N�Z�X�T�C�N���̐ݒ�͊֐����ōs���܂��̂ŁA���O�ɍs���K�v�͂���܂���B

                �����̊֐����ł͈����ԃJ�[�g���b�W�o�X�����b�N����܂��̂Œ��ӂ��ĉ������B

                
  Arguments:    secNo    : �ΏۃZ�N�^��
                offset   : �Z�N�^���̃o�C�g�P�ʂ̃I�t�Z�b�g
                dst      : ���[�h�����f�[�^���i�[���郏�[�N�̈�̃A�h���X
                size     : �o�C�g�P�ʂł̃��[�h�T�C�Y
                callback : Read�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      None.
 *---------------------------------------------------------------------------*/
extern void CTRDG_ReadAgbFlash(u16 sec_num, u32 offset, u8 *dst, u32 size);
extern void CTRDG_ReadAgbFlashAsync(u16 sec_num, u32 offset, u8 *dst, u32 size,
                                    CTRDG_TASK_FUNC callback);


/*------------------------------------------------------------------*/
/*          �`�b�v����                                              */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_EraseAgbFlashChip
                CTRDG_EraseAgbFlashChipAsync
                
                �Ή�����AGB���̊֐��Fextern u16 (*EraseFlashChip)()

  Description:  �`�b�v�S�̂����S�ɃC���[�X���܂��B
  
                �A�N�Z�X�T�C�N���̐ݒ�͊֐����ōs���܂��̂ŁA���O�ɍs���K�v�͂���܂���B
                �܂��A���̊֐��ł̓^�C���A�E�g����p�Ƀ`�b�N���g�p���܂��̂Ŏ��O�� 
                OS_InitTick �֐����Ă�ł����K�v������܂��B
                
                �܂��A�񓯊��łł́A�{���[�`���̌Ăяo����ɕԂ��Ă���R�[���o�b�N�֐��̈����ł���
                �\���� CTRDGTaskInfo �̃����o result ���Q�Ƃ��邱�ƂŁA Erase�����ɐ��������̂���m�邱�Ƃ��ł��܂��B

                �����̊֐����ł͈����ԑS�Ă̊��荞�݂��֎~����A�J�[�g���b�W�o�X�����b�N����܂��̂Œ��ӂ��ĉ������B
                ���ɁA�{�֐��R�[�����̓_�C���N�g�T�E���h����тu�E�g�u�����N�����A�\�������A
                �J�[�g���b�W���N�G�X�g���̓���̃^�C�~���O�Ŏ����N������c�l�`�͎g�p���Ȃ��ŉ������B

                
  Arguments:    callback : Erase�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      0     : ����I��       (�����֐��̏ꍇ�̂�)
                0�ȊO : �C���[�X�G���[
 *---------------------------------------------------------------------------*/
extern u16 CTRDG_EraseAgbFlashChip(void);
extern void CTRDG_EraseAgbFlashChipAsync(CTRDG_TASK_FUNC callback);


/*------------------------------------------------------------------*/
/*          �Z�N�^����                                              */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_EraseAgbFlashSector
                CTRDG_EraseAgbFlashSectorAsync
                
                �Ή�����AGB���̊֐��Fextern u16 (*EraseFlashSector)(u16 secNo)

  Description:  �ΏۃZ�N�^ �� �̃Z�N�^���C���[�X���܂��B
                ���̃��[�`���͏������݃��[�`���̒��ŃR�[������邽�߁A
                �ʏ�͏������ݑO�ɂ��̃��[�`�����R�[������K�v�͂���܂���B
                �ΏۃZ�N�^�����͈͊O�̎��̓G���[��Ԃ��܂��B
                
                �܂��A�񓯊��łł́A�{���[�`���̌Ăяo����ɕԂ��Ă���R�[���o�b�N�֐��̈����ł���
                �\���� CTRDGTaskInfo �̃����o result ���Q�Ƃ��邱�ƂŁA Erase�����ɐ��������̂���m�邱�Ƃ��ł��܂��B
                
                �A�N�Z�X�T�C�N���̐ݒ�͊֐����ōs���܂��̂ŁA���O�ɍs���K�v�͂���܂���B
                �܂��A���̊֐��ł̓^�C���A�E�g����p�Ƀ`�b�N���g�p���܂��̂Ŏ��O�� 
                OS_InitTick �֐����Ă�ł����K�v������܂��B

                �����̊֐����ł͈����ԑS�Ă̊��荞�݂��֎~����A�J�[�g���b�W�o�X�����b�N����܂��̂Œ��ӂ��ĉ������B
                ���ɁA�{�֐��R�[�����̓_�C���N�g�T�E���h����тu�E�g�u�����N�����A�\�������A
                �J�[�g���b�W���N�G�X�g���̓���̃^�C�~���O�Ŏ����N������c�l�`�͎g�p���Ȃ��ŉ������B

                
  Arguments:    sec_num   : �ΏۃZ�N�^��
                callback  : Erase�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      0     : ����I��       (�����֐��̏ꍇ�̂�)
                0�ȊO : �C���[�X�G���[
 *---------------------------------------------------------------------------*/
extern u16 CTRDG_EraseAgbFlashSector(u16 sec_num);
extern void CTRDG_EraseAgbFlashSectorAsync(u16 sec_num, CTRDG_TASK_FUNC callback);


/*------------------------------------------------------------------*/
/*          �Z�N�^�P�ʃf�[�^��������                                */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_WriteAgbFlashSector
                CTRDG_WriteAgbFlashSectorAsync
                
                �Ή�����AGB���̊֐��Fextern u16 (*ProgramFlashSector)(u16 secNo,u8 *src)

  Description:  src�A�h���X����P�Z�N�^��(4kbyte)�̃f�[�^��ΏۃZ�N�^���ɏ������݂܂��B
                �{���[�`�����ŏ�L��CTRDG_EraseAgbFlashSector���R�[�����A�Z�N�^���������Ă��珑�����݂��s���܂��B
                �ΏۃZ�N�^�����͈͊O�̎��̓G���[��Ԃ��܂��B
                �{���[�`���̎��s���A�O���[�o���ϐ�ctrdg_flash_remainder���Q�Ƃ��邱�ƂŁA�c�o�C�g����m�邱�Ƃ��ł��܂��B
                
                �܂��A�񓯊��łł́A�{���[�`���̌Ăяo����ɕԂ��Ă���R�[���o�b�N�֐��̈����ł���
                �\���� CTRDGTaskInfo �̃����o result ���Q�Ƃ��邱�ƂŁA Write�����ɐ��������̂���m�邱�Ƃ��ł��܂��B
                
                �A�N�Z�X�T�C�N���̐ݒ�͊֐����ōs���܂��̂ŁA���O�ɍs���K�v�͂���܂���B
                �܂��A���̊֐��ł̓^�C���A�E�g����p�Ƀ`�b�N���g�p���܂��̂Ŏ��O�� 
                OS_InitTick �֐����Ă�ł����K�v������܂��B

                �����̊֐����ł͈����ԑS�Ă̊��荞�݂��֎~����A�J�[�g���b�W�o�X�����b�N����܂��̂Œ��ӂ��ĉ������B
                ���ɁA�{�֐��R�[�����̓_�C���N�g�T�E���h����тu�E�g�u�����N�����A�\�������A
                �J�[�g���b�W���N�G�X�g���̓���̃^�C�~���O�Ŏ����N������c�l�`�͎g�p���Ȃ��ŉ������B

                
  Arguments:    sec_num   : �ΏۃZ�N�^��
                src       : �������݌��A�h���X
                callback  : Write�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      0     : ����I��       (�����֐��̏ꍇ�̂�)
                0�ȊO : ���C�g�G���[
 *---------------------------------------------------------------------------*/
extern u16 CTRDG_WriteAgbFlashSector(u16 sec_num, u8 *src);
extern void CTRDG_WriteAgbFlashSectorAsync(u16 sec_num, u8 *src, CTRDG_TASK_FUNC callback);


/*------------------------------------------------------------------*/
/*          �o�C�g�P�ʃf�[�^�x���t�@�C                              */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_VerifyAgbFlash
                CTRDG_VerifyAgbFlashAsync
                
                �Ή�����AGB���̊֐��Fextern u32 VerifyFlash(u16 secNo,u8 *src,u32 size)

  Description:  src�A�h���X����̃f�[�^�ƃt���b�V���̑ΏۃZ�N�^���̃f�[�^��size�o�C�g���x���t�@�C���܂��B
                �{�֐��́A�x���t�@�C������ɏI�������Ȃ�΂O��Ԃ��A�x���t�@�C�G���[���������Ȃ�΃G���[�̔��������A�h���X��Ԃ��܂��B
                �܂��A�񓯊��łł́A�{���[�`���̌Ăяo����ɕԂ��Ă���R�[���o�b�N�֐��̈����ł���
                �\���� CTRDGTaskInfo �̃����o result ���Q�Ƃ��邱�ƂŁA Verify�����ɐ��������̂���m�邱�Ƃ��ł��܂��B
                
  Arguments:    sec_num   : �ΏۃZ�N�^��
                src       : �x���t�@�C���A�h���X�i�I���W�i���f�[�^�j
                size      : �x���t�@�C�T�C�Y(byte)
                callback  : Verify�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      0     : ����I��                                                (�����֐��̏ꍇ�̂�)
                0�ȊO : �x���t�@�C�G���[���̓f�o�C�X���G���[�A�h���X��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
extern u32 CTRDG_VerifyAgbFlash(u16 sec_num, u8 *src, u32 size);
extern void CTRDG_VerifyAgbFlashAsync(u16 sec_num, u8 *src, u32 size, CTRDG_TASK_FUNC callback);


/*------------------------------------------------------------------*/
/*          �Z�N�^�P�ʃf�[�^�������݁��o�C�g�P�ʃx���t�@�C          */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_WriteAndVerifyAgbFlash
                CTRDG_WriteAndVerifyAgbFlashAsync
                
                �Ή�����AGB���̊֐��Fextern u32 ProgramFlashSectorEx2(u16 secNo,u8 *src,u32 verifySize)

  Description:  �{�֐��́A������ CTRDG_WriteAgbFlashSector �֐��ŏ������݂��s������
                CTRDG_VerifyAgbFlash �֐��� verifysize �o�C�g���x���t�@�C���s���܂��B
                �G���[�̏ꍇ�͍ő��CTRDG_AGB_FLASH_RETRY_MAX(ctrdg_flash.h�Œ�`�j�񃊃g���C���s���܂��B
                
                ���{�֐��ł́A���C�g�G���[����32bit����16bit�ŃG���[�R�[�h��Ԃ��܂����A
                �x���t�@�C�G���[����32bit�̃f�o�C�X���G���[�A�h���X��Ԃ��܂��̂ŁA
                �G���[�R�[�h�m�F�̍ۂ͂����Ӊ������B
                �܂��A�񓯊��łł́A�{���[�`���̌Ăяo����ɕԂ��Ă���R�[���o�b�N�֐��̈����ł���
                �\���� CTRDGTaskInfo �̃����o result ���Q�Ƃ��邱�ƂŁA WriteAndVerify�����ɐ��������̂���m�邱�Ƃ��ł��܂��B
                
                �A�N�Z�X�T�C�N���̐ݒ�͊֐����ōs���܂��̂ŁA���O�ɍs���K�v�͂���܂���B
                �܂��A���̊֐��ł̓^�C���A�E�g����p�Ƀ`�b�N���g�p���܂��̂Ŏ��O�� 
                OS_InitTick �֐����Ă�ł����K�v������܂��B

                �����̊֐����ł͈����ԑS�Ă̊��荞�݂��֎~����A�J�[�g���b�W�o�X�����b�N����܂��̂Œ��ӂ��ĉ������B
                ���ɁA�{�֐��R�[�����̓_�C���N�g�T�E���h����тu�E�g�u�����N�����A�\�������A
                �J�[�g���b�W���N�G�X�g���̓���̃^�C�~���O�Ŏ����N������c�l�`�͎g�p���Ȃ��ŉ������B

                
  Arguments:    sec_num    : �ΏۃZ�N�^��
                src        : �������݌��A�h���X
                verifysize : �x���t�@�C�T�C�Y�ibyte�j
                callback   : WriteAndVerify�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      0     : ����I��                                                (�����֐��̏ꍇ�̂�)
                0�ȊO : �x���t�@�C�G���[���̓f�o�C�X���G���[�A�h���X��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
extern u32 CTRDG_WriteAndVerifyAgbFlash(u16 sec_num, u8 *src, u32 verifysize);
extern void CTRDG_WriteAndVerifyAgbFlashAsync(u16 sec_num, u8 *src, u32 verifysize,
                                              CTRDG_TASK_FUNC callback);


#ifdef __cplusplus
}      /* extern "C" */
#endif


#endif /* NITRO_CTRDG_FLASH_H_ */

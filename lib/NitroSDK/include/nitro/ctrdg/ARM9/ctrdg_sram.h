/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CTRDG - include
  File:     ctrdg_sram.h

  Copyright 2003,2004,2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ctrdg_sram.h,v $
  Revision 1.3  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.2  2006/04/07 03:28:23  okubata_ryoma
  �R�����g�̏C��

  Revision 1.1  2006/04/05 10:34:15  okubata_ryoma
  AGB�o�b�N�A�b�v���C�u������SDK���^�̂��߂̕ύX


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITRO_CTRDG_SRAM_H_
#define NITRO_CTRDG_SRAM_H_

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif

/* �f�o�b�O�p */
//#ifndef __SRAM_DEBUG
#define CTRDG_AGB_SRAM_ADR          0x0A000000  /* SRAM�擪�A�h���X */
#define CTRDG_AGB_SRAM_SIZE_256K    0x00008000  /* 256KSRAM�T�C�Y */
#define CTRDG_AGB_SRAM_SIZE_512K    0x00010000  /* 512KSRAM�T�C�Y */
//#else
//#define CTRDG_AGB_SRAM_ADR        0x02018000
//#define CTRDG_AGB_SRAM_SIZE_256K    0x00000400
//#define CTRDG_AGB_SRAM_SIZE_512K    0x00000800
//#endif

/* CTRDG_WriteAgbSramEx�֐����g���C�񐔏�� */
#define CTRDG_AGB_SRAM_RETRY_MAX    3


/*------------------------------------------------------------------
--------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*          �f�[�^�ǂݏo��                                          */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_ReadAgbSram
                CTRDG_ReadAgbSramAsync
  
                �Ή�����AGB���̊֐��Fextern void ReadSram(u8 *src,u8 *dst,u32 size)

  Description:  �����Ŏw�肳�ꂽSRAM�A�h���X����Asize�o�C�g�̃f�[�^�����[�N��dst
                �A�h���X�ȍ~�ɓǂݏo���܂��B
                
  Arguments:    src       : ���[�h����SRAM�̃A�h���X�iAGB�������}�b�v��̃A�h���X�j
                dst       : ���[�h�����f�[�^���i�[���郏�[�N�̈�̃A�h���X�iAGB�������}�b�v��̃A�h���X�j
                size      : �o�C�g�P�ʂł̃��[�h�T�C�Y
                callback  : Read�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      None.
 *---------------------------------------------------------------------------*/
extern void CTRDG_ReadAgbSram(u32 src, void *dst, u32 size);
extern void CTRDG_ReadAgbSramAsync(u32 src, void *dst, u32 size, CTRDG_TASK_FUNC callback);


/*------------------------------------------------------------------*/
/*          �f�[�^��������                                          */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_WriteAgbSram
                CTRDG_WriteAgbSramAsync
                
                �Ή�����AGB���̊֐��Fextern void WriteSram(u8 *dst,u8 *src,u32 size)

  Description:  �����Ŏw�肳�ꂽ���[�N�̈�A�h���X����Asize�o�C�g�̃f�[�^��SRAM��
                dst�A�h���X�ȍ~�ɏ������݂܂��B
                
  Arguments:    dst       : ���C�g���SRAM�̃A�h���X�iAGB�������}�b�v��̃A�h���X�j
                src       : ���C�g���̃��[�N�̈�̃A�h���X
                size      : �o�C�g�P�ʂł̃��C�g�T�C�Y
                callback  : Write�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      None.
 *---------------------------------------------------------------------------*/
extern void CTRDG_WriteAgbSram(u32 dst, const void *src, u32 size);
extern void CTRDG_WriteAgbSramAsync(u32 dst, const void *src, u32 size, CTRDG_TASK_FUNC callback);


/*------------------------------------------------------------------*/
/*          �f�[�^�x���t�@�C                                        */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_VerifyAgbSram
                CTRDG_VerifyAgbSramAsync
                
                �Ή�����AGB���̊֐��Fextern u32 VerifySram(u8 *tgt,u8 *src,u32 size)

  Description:  ���[�N�̈��src�A�h���X����̃f�[�^��SRAM��tgt�A�h���X�̃f�[�^��size�o�C�g���x���t�@�C���܂��B
                �{�֐��́A�x���t�@�C������ɏI�������Ȃ�΂O��Ԃ��A�x���t�@�C�G���[
                ���������Ȃ�΃G���[�̔��������A�h���X��Ԃ��܂��B
                �܂��A�񓯊��łł́A�{���[�`���̌Ăяo����ɕԂ��Ă���R�[���o�b�N�֐��̈����ł���
                �\���� CTRDGTaskInfo �̃����o result ���Q�Ƃ��邱�ƂŁA Verify�����ɐ��������̂���m�邱�Ƃ��ł��܂��B
                
  Arguments:    tgt       : �x���t�@�C��SRAM�A�h���X�ւ̃|�C���^ (�������ݐ�̃f�[�^�AAGB�������}�b�v��̃A�h���X�j
                src       : �x���t�@�C�����[�N�̈�A�h���X�ւ̃|�C���^ �i�I���W�i���̃f�[�^�j
                size      : �o�C�g�P�ʂł̃x���t�@�C�T�C�Y
                callback  : Verify�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      0     : ����I��
                0�ȊO : �x���t�@�C�G���[���̓f�o�C�X���G���[�A�h���X��Ԃ��܂��B(�����֐��̏ꍇ�̂�)
 *---------------------------------------------------------------------------*/
extern u32 CTRDG_VerifyAgbSram(u32 tgt, const void *src, u32 size);
extern void CTRDG_VerifyAgbSramAsync(u32 tgt, const void *src, u32 size, CTRDG_TASK_FUNC callback);


/*------------------------------------------------------------------*/
/*          �f�[�^�������݁��x���t�@�C                              */
/*------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         CTRDG_WriteAndVerifyAgbSram
                CTRDG_WriteAndVerifyAgbSramAsync
                
                �Ή�����AGB���̊֐��Fextern u32 WriteSramEx(u8 *dst, u8 *src, u32 size)

  Description:  ������CTRDG_WriteAgbSram�ŏ������݂��s������CTRDG_VerifyAgbSram�Ńx���t�@�C���s���A
                �G���[�̏ꍇ�͍ő��CTRDG_AGB_SRAM_RETRY_MAX�ictrdg_sram.h�ɂĒ�`�j�񃊃g���C���s���܂��B
                
                �܂��A�񓯊��łł́A�{���[�`���̌Ăяo����ɕԂ��Ă���R�[���o�b�N�֐��̈����ł���
                �\���� CTRDGTaskInfo �̃����o result ���Q�Ƃ��邱�ƂŁA WriteAndVerify�����ɐ��������̂���m�邱�Ƃ��ł��܂��B
                
  Arguments:    dst       : ���C�g���SRAM�̃A�h���X�iAGB�������}�b�v��̃A�h���X�j
                src       : ���C�g���̃��[�N�̈�̃A�h���X
                size      : �o�C�g�P�ʂł̃��C�g/�x���t�@�C�T�C�Y
                callback  : WritrAndVerify�����I�����ɌĂяo�����R�[���o�b�N�֐�(�񓯊��֐��̏ꍇ�̂�)

  Returns:      0     : ����I��
                0�ȊO : �x���t�@�C�G���[���̓f�o�C�X���G���[�A�h���X��Ԃ��܂��B(�����֐��̏ꍇ�̂�)
 *---------------------------------------------------------------------------*/
extern u32 CTRDG_WriteAndVerifyAgbSram(u32 dst, const void *src, u32 size);
extern void CTRDG_WriteAndVerifyAgbSramAsync(u32 dst, const void *src, u32 size,
                                             CTRDG_TASK_FUNC callback);


#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif /* NITRO_CTRDG_SRAM_H_ */

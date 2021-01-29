/*---------------------------------------------------------------------------*
  Project:  NitroSDK - RTC - include
  File:     fifo.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: fifo.h,v $
  Revision 1.6  2005/02/28 05:26:08  yosizaki
  do-indent.

  Revision 1.5  2004/09/14 01:59:57  yasu
  Fix backslashs at end of line.

  Revision 1.4  2004/05/31 08:42:49  terui
  �A���[�������ݗpRESULT���폜���ACOMMAND�ɒǉ��B

  Revision 1.3  2004/05/17 11:02:51  terui
  �������ʂ�FATAL_ERROR��ǉ�

  Revision 1.2  2004/05/17 08:43:34  terui
  �������ʂ�BUSY��ǉ�

  Revision 1.1  2004/05/12 02:27:41  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_RTC_COMMON_FIFO_H_
#define NITRO_RTC_COMMON_FIFO_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
// �v���g�R���֘A��`
#define RTC_PXI_COMMAND_MASK                0x00007f00  // �R�}���h�i�[�����̃}�X�N
#define RTC_PXI_COMMAND_SHIFT               8   // �R�}���h�i�[�����̈ʒu
#define RTC_PXI_RESULT_BIT_MASK             0x00008000  // PXI�̉��������������̃}�X�N
#define RTC_PXI_RESULT_MASK                 0x000000ff  // �������ʊi�[�����̃}�X�N
#define RTC_PXI_RESULT_SHIFT                0   // �������ʊi�[�����̈ʒu

// PXI�R�}���h��`
#define RTC_PXI_COMMAND_RESET               0x00        // ���Z�b�g
#define RTC_PXI_COMMAND_SET_HOUR_FORMAT     0x01        // ���ԕ\�L�t�H�[�}�b�g�ݒ�
#define RTC_PXI_COMMAND_READ_DATETIME       0x10        // ���t�E�����ǂݏo��
#define RTC_PXI_COMMAND_READ_DATE           0x11        // ���t�ǂݏo��
#define RTC_PXI_COMMAND_READ_TIME           0x12        // �����ǂݏo��
#define RTC_PXI_COMMAND_READ_PULSE          0x13        // �p���X�o�͐ݒ�l�ǂݏo��
#define RTC_PXI_COMMAND_READ_ALARM1         0x14        // �A���[���P�ݒ�l�ǂݏo��
#define RTC_PXI_COMMAND_READ_ALARM2         0x15        // �A���[���Q�ݒ�l�ǂݏo��
#define RTC_PXI_COMMAND_READ_STATUS1        0x16        // �X�e�[�^�X�P���W�X�^�ǂݏo��
#define RTC_PXI_COMMAND_READ_STATUS2        0x17        // �X�e�[�^�X�Q���W�X�^�ǂݏo��
#define RTC_PXI_COMMAND_READ_ADJUST         0x18        // ���g����튄���݃��W�X�^�ǂݏo��
#define RTC_PXI_COMMAND_READ_FREE           0x19        // �t���[���W�X�^�ǂݏo��
#define RTC_PXI_COMMAND_WRITE_DATETIME      0x20        // ���t�E������������
#define RTC_PXI_COMMAND_WRITE_DATE          0x21        // ���t��������
#define RTC_PXI_COMMAND_WRITE_TIME          0x22        // ������������
#define RTC_PXI_COMMAND_WRITE_PULSE         0x23        // �p���X�o�͐ݒ�l��������
#define RTC_PXI_COMMAND_WRITE_ALARM1        0x24        // �A���[���P�ݒ�l��������
#define RTC_PXI_COMMAND_WRITE_ALARM2        0x25        // �A���[���Q�ݒ�l��������
#define RTC_PXI_COMMAND_WRITE_STATUS1       0x26        // �X�e�[�^�X�P���W�X�^��������
#define RTC_PXI_COMMAND_WRITE_STATUS2       0x27        // �X�e�[�^�X�Q���W�X�^��������
#define RTC_PXI_COMMAND_WRITE_ADJUST        0x28        // ���g����튄���݃��W�X�^��������
#define RTC_PXI_COMMAND_WRITE_FREE          0x29        // �t���[���W�X�^��������
#define RTC_PXI_COMMAND_INTERRUPT           0x30        // �A���[�������ݔ����ʒB�p

// ������`
typedef enum RTCPxiResult
{
    RTC_PXI_RESULT_SUCCESS = 0,        // ��������.
    RTC_PXI_RESULT_INVALID_COMMAND,    // �s����PXI�R�}���h.
    RTC_PXI_RESULT_ILLEGAL_STATUS,     // RTC�̏�Ԃɂ�菈�������s�s�\.
    RTC_PXI_RESULT_BUSY,               // ���̃��N�G�X�g�����s��.
    RTC_PXI_RESULT_FATAL_ERROR,        // ���̑����炩�̌����ŏ����Ɏ��s.
    RTC_PXI_RESULT_MAX
}
RTCPxiResult;


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* NITRO_RTC_COMMON_FIFO_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

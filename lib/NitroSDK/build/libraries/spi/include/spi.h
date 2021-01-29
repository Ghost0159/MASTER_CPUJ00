/*---------------------------------------------------------------------------*
  Project:  NitroSDK - libraries - spi
  File:     spi.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: spi.h,v $
  Revision 1.20  2007/04/12 00:08:39  yasu
  ����N�x�C��

  Revision 1.19  2007/04/03 07:33:07  terui
  �p���[�_�E���R�}���h���g�p�֎~�ɕύX

  Revision 1.18  2006/01/18 02:12:29  kitase_hirotake
  do-indent

  Revision 1.17  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.16  2004/12/29 02:05:04  takano_makoto
  SetStability�֐���retry�p�����[�^��p�~

  Revision 1.15  2004/10/07 06:49:34  terui
  NVRAM�ɂ��ĕʃ��[�J�[���̏ꍇ�̃R�}���h�ǉ��ɔ����C���B

  Revision 1.14  2004/07/31 08:08:46  terui
  �s�v�ȃR�[�h�̍폜

  Revision 1.13  2004/06/01 04:45:59  terui
  �}�C�N�P�̃T���v�����O��inline�֐�����buf�w����폜

  Revision 1.12  2004/05/31 08:54:50  terui
  �C�����C���֐��ȊO���O���[�o��include����type.h�Ɉړ��B

  Revision 1.11  2004/05/28 08:51:44  terui
  nitro/include�ȉ��̃w�b�_�t�@�C���ɒ�`��؂�o���B
  inline�֐���ARM9�����[�J���֐����ł���΃t�@�C�����̂��폜�\��B

  Revision 1.10  2004/05/25 00:58:48  terui
  SPI�e�f�o�C�X�p���C�u�����ו����ɔ����C��

  Revision 1.8  2004/05/12 05:28:24  terui
  SPI_Init�̐錾��ύX

  Revision 1.7  2004/04/29 10:28:43  terui
  �f�o�C�X�^�C�v��ARM7����̔r���A�N�Z�X���l�������^�C�v��ǉ�

  Revision 1.6  2004/04/27 12:05:59  terui
  �}�C�N�T���v�����O�ŐV�ۑ��A�h���X�擾I/F��ǉ�

  Revision 1.5  2004/04/23 02:27:35  takano_makoto
  Change Default Stability Range

  Revision 1.4  2004/04/15 13:55:41  takano_makoto
  Chang default stability parameter

  Revision 1.3  2004/04/14 06:26:46  terui
  SPI���C�u�����̃\�[�X�����ɔ����X�V

  Revision 1.2  2004/04/09 02:59:15  terui
  Correct misspell.

  Revision 1.1  2004/04/05 04:46:32  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef LIBRARIES_SPI_H_
#define LIBRARIES_SPI_H_

#include    <nitro/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
    �C�����C���֐���`
    ARM9�p�Ɏb��I�ɒ�`�B
    �{����ARM9�����C�u��������static�֐��Ƃ��Ē�`�����ׂ��B
 *---------------------------------------------------------------------------*/
#ifdef  SDK_ARM9

/*---------------------------------------------------------------------------*
  Name:         SPI_TpSamplingNow

  Description:  �^�b�`�p�l�������T���v�����O����B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_TpSamplingNow(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_TP_SAMPLING << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_TpAutoSamplingOn

  Description:  �^�b�`�p�l���̎����T���v�����O���J�n����B

  Arguments:    vCount -    �T���v�����O���s��V�J�E���g�B
                            1�t���[���ɕ�����T���v�����O����ꍇ�A������
                            �N�_��1�t���[���������������B
                frequency - 1�t���[���ɉ���T���v�����O���邩�̕p�x�B

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_TpAutoSamplingOn(u16 vCount, u8 frequency)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_TP_AUTO_ON << 8) | (u32)frequency, 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_END_BIT | (1 << SPI_PXI_INDEX_SHIFT) | (u32)vCount, 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_TpAutoSamplingOff

  Description:  �^�b�`�p�l���̎����T���v�����O���~����B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_TpAutoSamplingOff(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_TP_AUTO_OFF << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_TpSetupStability

  Description:  �T���v�����O�ɂ�������蔻��p�����[�^��ݒ肷��B

  Arguments:    range - �A�������T���v�����O�ɂ����āA���o�d�������肵���Ƃ݂Ȃ��덷�B
                        �Ȃ��A���o�l��12bit�� 0 �` 4095�B

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_TpSetupStability(u16 range)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_TP_SETUP_STABILITY << 8) | (u32)range, 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramWriteEnable

  Description:  NVRAM�Ɂu�������݋��v���߂𔭍s����B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramWriteEnable(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_NVRAM_WREN << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramWriteDisable

  Description:  NVRAM�Ɂu�������݋֎~�v���߂𔭍s����B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramWriteDisable(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_NVRAM_WRDI << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramReadStatusRegister

  Description:  NVRAM�Ɂu�X�e�[�^�X���W�X�^�ǂݏo���v���߂𔭍s����B

  Arguments:    pData - �ǂݏo�����l���i�[����ϐ��ւ̃|�C���^�B
                        �l��ARM7�����ڏ����o���̂ŃL���b�V���ɒ��ӁB

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramReadStatusRegister(u8 *pData)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_NVRAM_RDSR << 8) | ((u32)pData >> 24), 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (1 << SPI_PXI_INDEX_SHIFT) | (((u32)pData >> 8) & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet [2]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_END_BIT |
                               (2 << SPI_PXI_INDEX_SHIFT) | (((u32)pData << 8) & 0x0000ff00), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramReadDataBytes

  Description:  NVRAM�Ɂu�ǂݏo���v���߂𔭍s����B

  Arguments:    address - NVRAM��̓ǂݏo���J�n�A�h���X�B24bit�̂ݗL���B
                size -    �A�����ēǂݏo���o�C�g���B
                pData -   �ǂݏo�����l���i�[����z��B
                          �l��ARM7�����ڏ����o���̂ŃL���b�V���ɒ��ӁB

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramReadDataBytes(u32 address, u32 size, u8 *pData)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_NVRAM_READ << 8) |
                               ((address >> 16) & 0x000000ff), 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (1 << SPI_PXI_INDEX_SHIFT) | (address & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet [2]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM, (2 << SPI_PXI_INDEX_SHIFT) | (size >> 16), 0))
    {
        return FALSE;
    }

    // Send packet [3]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (3 << SPI_PXI_INDEX_SHIFT) | (size & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet [4]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (4 << SPI_PXI_INDEX_SHIFT) | ((u32)pData >> 16), 0))
    {
        return FALSE;
    }

    // Send packet [5]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_END_BIT |
                               (5 << SPI_PXI_INDEX_SHIFT) | ((u32)pData & 0x0000ffff), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramReadHigherSpeed

  Description:  NVRAM�Ɂu�����ǂݏo���v���߂𔭍s����B
                �u�����ǂݏo���v�͖��ߔ��s��A�f�[�^�ǂݏo���O��8bit�̃_�~�[��
                �N���b�N�����ނ��Ƃ�25MHz�܂ł̃N���b�N�œǂݏo���\�Ƃ�������
                �����ASPI�̃N���b�N�͍ō�4MHz�܂łȂ̂ŁA�g�p����Ӗ��͂Ȃ��B
                ���Ȃ݂ɕ��ʂ́u�ǂݏo���v���߂͍ō�20MHz�܂ł̃N���b�N�ɑΉ��B
                ��L�̓f�o�C�X��M45PE40�̏ꍇ�ŁALE25FW203T�̏ꍇ�́u�ǂݏo���v
                �u�����ǂݏo���v����30MHz�B

  Arguments:    address - NVRAM��̓ǂݏo���J�n�A�h���X�B24bit�̂ݗL���B
                size -    �A�����ēǂݏo���o�C�g���B
                pData -   �ǂݏo�����l���i�[����z��B
                          �l��ARM7�����ڏ����o���̂ŃL���b�V���ɒ��ӁB

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramReadHigherSpeed(u32 address, u32 size, u8 *pData)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_NVRAM_FAST_READ << 8) |
                               ((address >> 16) & 0x000000ff), 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (1 << SPI_PXI_INDEX_SHIFT) | (address & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet [2]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM, (2 << SPI_PXI_INDEX_SHIFT) | (size >> 16), 0))
    {
        return FALSE;
    }

    // Send packet [3]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (3 << SPI_PXI_INDEX_SHIFT) | (size & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet [4]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (4 << SPI_PXI_INDEX_SHIFT) | ((u32)pData >> 16), 0))
    {
        return FALSE;
    }

    // Send packet [5]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_END_BIT |
                               (5 << SPI_PXI_INDEX_SHIFT) | ((u32)pData & 0x0000ffff), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramPageWrite

  Description:  NVRAM�Ɂu�y�[�W�������݁v���߂𔭍s����B
                NVRAM�̓����ł́A�u�y�[�W�����v�Ɓu�y�[�W��������(�����t)�v��
                �A�����čs����B

  Arguments:    address - NVRAM��̏������݊J�n�A�h���X�B24bit�̂ݗL���B
                size -    �A�����ď������ރo�C�g���B
                          address + size ���y�[�W���E(256�o�C�g)���z����Ɖz����
                          ���̃f�[�^�͖��������B
                pData -   �������ޒl���i�[����Ă���z��B
                          ARM7�����ړǂݏo���̂ŁA�L���b�V�����烁�������̂�
                          �m���ɏ����o���Ă����K�v������B

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramPageWrite(u32 address, u16 size, const u8 *pData)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_NVRAM_PW << 8) | ((address >> 16) & 0x000000ff), 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (1 << SPI_PXI_INDEX_SHIFT) | (address & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet [2]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM, (2 << SPI_PXI_INDEX_SHIFT) | (u32)size, 0))
    {
        return FALSE;
    }

    // Send packet [3]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (3 << SPI_PXI_INDEX_SHIFT) | ((u32)pData >> 16), 0))
    {
        return FALSE;
    }

    // Send packet [4]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_END_BIT |
                               (4 << SPI_PXI_INDEX_SHIFT) | ((u32)pData & 0x0000ffff), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramPageWrite

  Description:  NVRAM�Ɂu�y�[�W��������(�����t)�v���߂𔭍s����B
                ���ʂ́u�y�[�W�������݁v��舳�|�I�ɑ����������߂邪�A�r�b�g��
                �����鑀�삵���ł��Ȃ��B'0'�̃r�b�g��'1'����������ł�'0'�̂܂܁B

  Arguments:    address - NVRAM��̏������݊J�n�A�h���X�B24bit�̂ݗL���B
                size -    �A�����ď������ރo�C�g���B
                          address + size ���y�[�W���E(256�o�C�g)���z����Ɖz����
                          ���̃f�[�^�͖��������B
                pData -   �������ޒl���i�[����Ă���z��B
                          ARM7�����ړǂݏo���̂ŁA�L���b�V�����烁�������̂�
                          �m���ɏ����o���Ă����K�v������B

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramPageProgram(u32 address, u16 size, const u8 *pData)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_NVRAM_PP << 8) | ((address >> 16) & 0x000000ff), 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (1 << SPI_PXI_INDEX_SHIFT) | (address & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet [2]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM, (2 << SPI_PXI_INDEX_SHIFT) | (u32)size, 0))
    {
        return FALSE;
    }

    // Send packet [3]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (3 << SPI_PXI_INDEX_SHIFT) | ((u32)pData >> 16), 0))
    {
        return FALSE;
    }

    // Send packet [4]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_END_BIT |
                               (4 << SPI_PXI_INDEX_SHIFT) | ((u32)pData & 0x0000ffff), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramPageErase

  Description:  NVRAM�Ɂu�y�[�W�����v���߂𔭍s����B
                �������ꂽ�y�[�W�̓r�b�g���S��'1'�ɂȂ�B

  Arguments:    address - NVRAM��̏�������y�[�W�̃A�h���X�B24bit�̂ݗL���B
                          ���̃A�h���X���܂܂��y�[�W��256byte���S�ď��������B
                          NVRAM�����ł̓A�h���X�̉���8bit���N���A���ꂽ�A�h���X��
                          �����ΏۃA�h���X�ƂȂ�A�Ǝv����B

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramPageErase(u32 address)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_NVRAM_PE << 8) | ((address >> 16) & 0x000000ff), 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_END_BIT |
                               (1 << SPI_PXI_INDEX_SHIFT) | (address & 0x0000ffff), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramSectorErase

  Description:  NVRAM�Ɂu�Z�N�^�����v���߂𔭍s����B
                �������ꂽ�Z�N�^�̓r�b�g���S��'1'�ɂȂ�B

  Arguments:    address - NVRAM��̏�������Z�N�^�̃A�h���X�B24bit�̂ݗL���B
                          ���̃A�h���X���܂܂��Z�N�^��64Kbyte���S�ď��������B
                          NVRAM�����ł̓A�h���X�̉���16bit���N���A���ꂽ�A�h���X��
                          �����ΏۃA�h���X�ƂȂ�A�Ǝv����B

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramSectorErase(u32 address)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_NVRAM_SE << 8) | ((address >> 16) & 0x000000ff), 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_END_BIT |
                               (1 << SPI_PXI_INDEX_SHIFT) | (address & 0x0000ffff), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/* �V�^�ԃf�o�C�X�����ɔ����g�p�֎~ (2007/4/3 terui@nintendo.co.jp) */
#if 0
/*---------------------------------------------------------------------------*
  Name:         SPI_NvramPowerDown

  Description:  NVRAM�Ɂu�ȓd�́v���߂𔭍s����B
                �u�ȓd�͂��畜�A�v���߈ȊO���󂯕t���Ȃ��Ȃ�B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramPowerDown(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_NVRAM_DP << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramReleasePowerDown

  Description:  NVRAM�Ɂu�ȓd�͂��畜�A�v���߂𔭍s����B
                �ȓd�̓��[�h�ɂȂ��Ă��Ȃ����͖��������B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramReleasePowerDown(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_NVRAM_RDP << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}
#endif

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramChipErase

  Description:  NVRAM�Ɂu�`�b�v�����v���߂𔭍s����B
                �f�o�C�X��LE25FW203T�̏ꍇ�ɗL���B
                �f�o�C�X��M45PE40�̏ꍇ�͖��������B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramChipErase(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_NVRAM_CE << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramReadSiliconId

  Description:  NVRAM�Ɂu�V���R��ID�ǂݏo���v���߂𔭍s����B
                �f�o�C�X��LE25FW203T�̏ꍇ�ɗL���B
                �f�o�C�X��M45PE40�̏ꍇ�͖��������B

  Arguments:    pData - �ǂݏo�����V���R��ID���i�[����2�o�C�g�̃o�b�t�@���w��
                        ����B�l��ARM7�����ڏ����o���̂ŃL���b�V���ɒ��ӁB

  Returns:      BOOL  - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                        ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramReadSiliconId(u8 *pData)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_NVRAM_RSI << 8) | ((u32)pData >> 24), 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               (1 << SPI_PXI_INDEX_SHIFT) | (((u32)pData >> 8) & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet [2]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_END_BIT |
                               (2 << SPI_PXI_INDEX_SHIFT) | (((u32)pData << 8) & 0x0000ff00), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_NvramSoftwareReset

  Description:  NVRAM�Ɂu�\�t�g�E�F�A���Z�b�g�v���߂𔭍s����B
                �f�o�C�X��LE25FW203T�̏ꍇ�ɗL���B
                �f�o�C�X��M45PE40�̏ꍇ�͖��������B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_NvramSoftwareReset(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_NVRAM,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_NVRAM_SR << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_MicSamplingNow

  Description:  �}�C�N�����T���v�����O����B

  Arguments:    type  - �T���v�����O�^�C�v( 0: 8�r�b�g , 1: 12�r�b�g )
                pData - �T���v�����O���ʂ��i�[���郁�����̃A�h���X�B
                   -> �폜

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_MicSamplingNow(u8 type)
{
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_MIC_SAMPLING << 8) | (u32)type, 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_MicAutoSamplingOn

  Description:  �}�C�N�̎����T���v�����O���J�n����B

  Arguments:    pData    - �T���v�����O�����f�[�^���i�[����o�b�t�@�̃A�h���X�B
                size     - �o�b�t�@�̃T�C�Y�B�o�C�g�P�ʂŎw��B
                span     - �T���v�����O�Ԋu(ARM7��CPU�N���b�N�Ŏw��)�B
                           �^�C�}�[�̐�����A16�r�b�g�~1or64or256or1024 �̐���
                           �������m�ɂ͐ݒ�ł��Ȃ��B�[���r�b�g�͐؂�̂Ă���B
                middle   - �r��������Ԃ��T���v�����O�񐔁B
                           -> �폜
                adMode   - AD�ϊ��̃r�b�g��( 8 or 12 )���w��B
                loopMode - �����T���v�����O�̃��[�v����ہB
                correct  - �����T���v�����O����Tick�␳�ہB

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_MicAutoSamplingOn(void *pData, u32 size, u32 span,
//    u32     middle ,
                                         u8 adMode, BOOL loopMode, BOOL correct)
{
    u8      temp;

    // �t���O�ނ�"type"�ϐ��ɂ܂Ƃ߂�
    switch (adMode)                    // AD�ϊ��r�b�g���w��
    {
    case SPI_MIC_SAMPLING_TYPE_8BIT:
    case SPI_MIC_SAMPLING_TYPE_12BIT:
        temp = adMode;
        break;
    default:
        return FALSE;
    }
    if (loopMode)                      // �A���T���v�����O�̃��[�v�w��
    {
        temp |= (u8)SPI_MIC_SAMPLING_TYPE_LOOP_ON;
    }
    if (correct)                       // Tick�덷�ɂ��␳���W�b�N�K�p�w��
    {
        temp |= (u8)SPI_MIC_SAMPLING_TYPE_CORRECT_ON;
    }

    // Send packet[0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_START_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_MIC_AUTO_ON << 8) | (u32)temp, 0))
    {
        return FALSE;
    }

    // Send packet[1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               (1 << SPI_PXI_INDEX_SHIFT) | ((u32)pData >> 16), 0))
    {
        return FALSE;
    }

    // Send packet[2]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               (2 << SPI_PXI_INDEX_SHIFT) | ((u32)pData & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet[3]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (3 << SPI_PXI_INDEX_SHIFT) | (size >> 16), 0))
    {
        return FALSE;
    }

    // Send packet[4]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               (4 << SPI_PXI_INDEX_SHIFT) | (size & 0x0000ffff), 0))
    {
        return FALSE;
    }

    // Send packet[5]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (5 << SPI_PXI_INDEX_SHIFT) | (span >> 16), 0))
    {
        return FALSE;
    }

    // Send packet[6]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_END_BIT |
                               (6 << SPI_PXI_INDEX_SHIFT) | (span & 0x0000ffff), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SPI_MicAutoSamplingOff

  Description:  �}�C�N�̎����T���v�����O���~����B
                �J�n���Ƀ��[�v�w�肵�Ȃ������ꍇ�̓o�b�t�@�����܂������_��
                �����I�ɒ�~����B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL SPI_MicAutoSamplingOff(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_START_BIT |
                               SPI_PXI_END_BIT |
                               (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_MIC_AUTO_OFF << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

#endif /* SDK_ARM9 */

/*===========================================================================*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LIBRARIES_SPI_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

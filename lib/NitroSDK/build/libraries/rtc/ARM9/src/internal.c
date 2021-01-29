/*---------------------------------------------------------------------------*
  Project:  NitroSDK - RTC - libraries
  File:     internal.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: internal.c,v $
  Revision 1.2  2005/02/28 05:26:08  yosizaki
  do-indent.

  Revision 1.1  2004/05/12 02:36:08  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include	<nitro/rtc/ARM9/api.h>
#include	<nitro/pxi.h>


/*---------------------------------------------------------------------------*
	�����֐���`
 *---------------------------------------------------------------------------*/
static BOOL RtcSendPxiCommand(u32 command);


/*---------------------------------------------------------------------------*
  Name:         RTCi_ResetAsync

  Description:  RTC�̃��Z�b�g��񓯊��ōs���B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ResetAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_RESET);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_SetHourFormatAsync

  Description:  �����t�H�[�}�b�g��񓯊��ŕύX����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���status1�ň�����ݒ肷��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_SetHourFormatAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_SET_HOUR_FORMAT);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawDateTimeAsync

  Description:  �����H�̓��t�E�����f�[�^��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawDateTimeAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_DATETIME);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawDateTimeAsync

  Description:  �����H�̓��t�E�����f�[�^��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawDateTimeAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_DATETIME);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawDateAsync

  Description:  �����H�̓��t�f�[�^��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawDateAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_DATE);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawDateAsync

  Description:  �����H�̓��t�f�[�^��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawDateAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_DATE);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawTimeAsync

  Description:  �����H�̎����f�[�^��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawTimeAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_TIME);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawTimeAsync

  Description:  �����H�̎����f�[�^��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawTimeAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_TIME);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawPulseAsync

  Description:  �����H�̎��g����튄���ݐݒ�l��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawPulseAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_PULSE);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawPulseAsync

  Description:  �����H�̎��g����튄���ݐݒ�l��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawPulseAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_PULSE);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawAlarm1Async

  Description:  �����H�̃A���[���P�����ݐݒ�l��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawAlarm1Async(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_ALARM1);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawAlarm1Async

  Description:  �����H�̃A���[���P�����ݐݒ�l��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawAlarm1Async(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_ALARM1);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawAlarm2Async

  Description:  �����H�̃A���[���Q�����ݐݒ�l��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawAlarm2Async(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_ALARM2);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawAlarm2Async

  Description:  �����H�̃A���[���Q�����ݐݒ�l��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawAlarm2Async(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_ALARM2);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawStatus1Async

  Description:  �����H�̃X�e�[�^�X�P���W�X�^�ݒ�l��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawStatus1Async(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_STATUS1);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawStatus1Async

  Description:  �����H�̃X�e�[�^�X�P���W�X�^�ݒ�l��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawStatus1Async(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_STATUS1);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawStatus2Async

  Description:  �����H�̃X�e�[�^�X�Q���W�X�^�ݒ�l��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawStatus2Async(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_STATUS2);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawStatus2Async

  Description:  �����H�̃X�e�[�^�X�Q���W�X�^�ݒ�l��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawStatus2Async(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_STATUS2);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawAdjustAsync

  Description:  �����H�̃N���b�N�����p���W�X�^�ݒ�l��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawAdjustAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_ADJUST);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawAdjustAsync

  Description:  �����H�̃N���b�N�����p���W�X�^�ݒ�l��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawAdjustAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_ADJUST);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ReadRawFreeAsync

  Description:  �����H�̃t���[���W�X�^�ݒ�l��񓯊��Ŏ擾����B
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                �f�[�^��OS_GetSystemWork()->real_time_clock�Ɋi�[�����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_ReadRawFreeAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_FREE);
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_WriteRawFreeAsync

  Description:  �����H�̃t���[���W�X�^�ݒ�l��񓯊��Ńf�o�C�X�ɏ������ށB
                ARM7����̉�����PXI�R�[���o�b�N(tag:RTC)�ɂĒʒm�����B
                OS_GetSystemWork()->real_time_clock�ɂ���f�[�^���������܂��B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL RTCi_WriteRawFreeAsync(void)
{
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_FREE);
}

/*---------------------------------------------------------------------------*
  Name:         RtcSendPxiCommand

  Description:  �w��R�}���h��PXI�o�R��ARM7�ɑ��M����B

  Arguments:    None.

  Returns:      BOOL     - PXI�ɑ΂��đ��M�����������ꍇTRUE���A
                           PXI�ɂ�鑗�M�Ɏ��s�����ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL RtcSendPxiCommand(u32 command)
{
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_RTC,
                               ((command << RTC_PXI_COMMAND_SHIFT) & RTC_PXI_COMMAND_MASK), 0))
    {
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

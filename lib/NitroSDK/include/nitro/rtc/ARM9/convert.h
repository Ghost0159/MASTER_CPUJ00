/*---------------------------------------------------------------------------*
  Project:  NitroSDK - RTC - include
  File:     convert.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: convert.h,v $
  Revision 1.4  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.3  2005/09/30 00:25:46  yada
  add  RTC_GetDayOfWeek()

  Revision 1.2  2005/05/18 02:53:17  seiki_masashi
  RTC_ConvertDateToDay, DayToDate �����J

  Revision 1.1  2005/05/17 13:01:45  seiki_masashi
  �ϊ��֐��̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_RTC_ARM9_CONVERT_H_
#define NITRO_RTC_ARM9_CONVERT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include <nitro/types.h>

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    �֐��錾
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         RTC_ConvertDateToDay

  Description:  RTCDate �^�̓��t�f�[�^��2000�N����̒ʎZ�����ɕϊ�����

  Arguments:    date      - ���t�f�[�^�ւ̃|�C���^.

  Returns:      2000�N1��1������̒ʎZ����.
 *---------------------------------------------------------------------------*/
s32     RTC_ConvertDateToDay(const RTCDate *date);

/*---------------------------------------------------------------------------*
  Name:         RTCi_ConvertTimeToSecond

  Description:  RTCTime �^�̎����f�[�^��0������̒ʎZ�b���ɕϊ�����

  Arguments:    time      - �����f�[�^�ւ̃|�C���^.

  Returns:      0��0������̒ʎZ�b��.
 *---------------------------------------------------------------------------*/
s32     RTCi_ConvertTimeToSecond(const RTCTime *time);

/*---------------------------------------------------------------------------*
  Name:         RTC_ConvertDateTimeToSecond

  Description:  RTCDate �^�� RTCTime �^�̓����f�[�^��ʎZ�b���ɕϊ�����

  Arguments:    date      - ���t�f�[�^�ւ̃|�C���^.
                time      - �����f�[�^�ւ̃|�C���^.

  Returns:      ���[�J�����Ԃɂ�����2000�N1��1��0��0������̒ʎZ�b��.
 *---------------------------------------------------------------------------*/
s64     RTC_ConvertDateTimeToSecond(const RTCDate *date, const RTCTime *time);

/*---------------------------------------------------------------------------*
  Name:         RTC_ConvertDayToDate

  Description:  2000�N����̒ʎZ������ RTCDate �^�̓��t�f�[�^�ɕϊ�����

  Arguments:    date      - �ۑ���̓��t�f�[�^�̃|�C���^.
                day       - 2000�N1��1������̒ʎZ����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    RTC_ConvertDayToDate(RTCDate *date, s32 day);

/*---------------------------------------------------------------------------*
  Name:         RTCi_ConvertSecondToTime

  Description:  0������̒ʎZ�b���� RTCTime �^�̎����f�[�^�ɕϊ�����

  Arguments:    time      - �ۑ���̓��t�f�[�^�̃|�C���^.
                sec       - 0��0������̒ʎZ�b��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    RTCi_ConvertSecondToTime(RTCTime *time, s32 sec);

/*---------------------------------------------------------------------------*
  Name:         RTC_ConvertDateTimeToSecond

  Description:  �ʎZ�b���� RTCDate �^�� RTCTime �^�̓����f�[�^��ϊ�����

  Arguments:    date      - �ۑ���̓��t�f�[�^�ւ̃|�C���^.
                time      - �ۑ���̎����f�[�^�ւ̃|�C���^.
                sec       - 2000�N1��1��0��0������̒ʎZ�b��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    RTC_ConvertSecondToDateTime(RTCDate *date, RTCTime *time, s64 sec);

/*---------------------------------------------------------------------------*
  Name:         RTC_GetDayOfWeek

  Description:  RTCDate �^�f�[�^����A�j�����v�Z���ĕԂ��B
                (RTCDate �̒��̗j�����g�p�����Ɍv�Z�ɂ���ċ��߂�)

  Arguments:    date      - ���t�f�[�^�ւ̃|�C���^.

  Returns:      �j��      RTC_WEEK_xxxx
 *---------------------------------------------------------------------------*/
RTCWeek RTC_GetDayOfWeek(RTCDate *date);

/*===========================================================================*/

#ifdef __cplusplus
}          /* extern "C" */
#endif

#endif /* NITRO_RTC_ARM9_CONVERT_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

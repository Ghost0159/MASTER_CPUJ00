/*---------------------------------------------------------------------------*
  Project:  NitroSDK - RTC - libraries
  File:     convert.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: convert.c,v $
  Revision 1.6  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.5  2005/10/04 00:29:01  yada
  suppressed warning

  Revision 1.4  2005/09/30 00:26:08  yada
  add  RTC_GetDayOfWeek()

  Revision 1.3  2005/05/18 02:01:37  seiki_masashi
  RTC_ConvertDateToDay, DayToDate �����J�֐��ɕύX

  Revision 1.2  2005/05/18 00:40:07  seiki_masashi
  �x���ɑΉ�

  Revision 1.1  2005/05/17 12:53:54  seiki_masashi
  �ϊ��֐��̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro/types.h>
#include    <nitro/rtc.h>


/*---------------------------------------------------------------------------*
    �ÓI�ϐ���`
 *---------------------------------------------------------------------------*/
static s32 sDayOfYear[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };


/*---------------------------------------------------------------------------*
    �����֐��錾
 *---------------------------------------------------------------------------*/

// �ȈՂ��邤�N�̔��� (���邤�N�Ȃ� TRUE�j
// �� RTC �̂Ƃ肤��͈͂� 2000 �` 2099 �N�Ɍ��肷��B
static inline BOOL RTCi_IsLeapYear(u32 year)
{
    // ���邤�N�́A�u4�Ŋ���؂� ���� 100�Ŋ���؂�Ȃ��N�v�܂��́u400�Ŋ���؂��N�v
    return !((year & 0x03));
}

/*---------------------------------------------------------------------------*
    �֐���`
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         RTC_ConvertDateToDay

  Description:  RTCDate �^�̓��t�f�[�^��2000�N����̒ʎZ�����ɕϊ�����

  Arguments:    date      - ���t�f�[�^�ւ̃|�C���^.

  Returns:      2000�N1��1������̒ʎZ����.
 *---------------------------------------------------------------------------*/
s32 RTC_ConvertDateToDay(const RTCDate *date)
{
    s32     dayNum;

    if (date->year >= 100
        || (date->month < 1) || (date->month > 12)
        || (date->day < 1) || (date->day > 31)
        || (date->week >= RTC_WEEK_MAX) || (date->month < 1) || (date->month > 12))
    {
        return -1;
    }

    // ���A��������Ɋ��Z
    dayNum = (s32)(date->day - 1);
    dayNum += sDayOfYear[date->month - 1];
    if ((date->month >= 3) && RTCi_IsLeapYear(date->year))
    {
        // �[�N�ł́A3���ȍ~��1�����₷
        dayNum++;
    }

    // �N������Ɋ��Z
    dayNum += date->year * 365;
    // �w��N-1�܂ł̂��邤�N�̌����Z�o���āA���̓��������Z�B
    // date->year �� 0 �` 99 �܂łȂ̂ŁA�[�N�̌v�Z�͊ȈՔ�
    dayNum += (date->year + 3) / 4;

    return dayNum;
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ConvertTimeToSecond

  Description:  RTCTime �^�̎����f�[�^��0������̒ʎZ�b���ɕϊ�����

  Arguments:    time      - �����f�[�^�ւ̃|�C���^.

  Returns:      0��0������̒ʎZ�b��.
 *---------------------------------------------------------------------------*/
s32 RTCi_ConvertTimeToSecond(const RTCTime *time)
{
    return (s32)((time->hour * 60 + time->minute) * 60 + time->second);
}

/*---------------------------------------------------------------------------*
  Name:         RTC_ConvertDateTimeToSecond

  Description:  RTCDate �^�� RTCTime �^�̓����f�[�^��ʎZ�b���ɕϊ�����

  Arguments:    date      - ���t�f�[�^�ւ̃|�C���^.
                time      - �����f�[�^�ւ̃|�C���^.

  Returns:      ���[�J�����Ԃɂ�����2000�N1��1��0��0������̒ʎZ�b��.
 *---------------------------------------------------------------------------*/
s64 RTC_ConvertDateTimeToSecond(const RTCDate *date, const RTCTime *time)
{
    s32     day, sec;
    day = RTC_ConvertDateToDay(date);
    if (day == -1)
    {
        return -1;
    }
    sec = RTCi_ConvertTimeToSecond(time);
    if (sec == -1)
    {
        return -1;
    }
    return ((s64)day) * (60 * 60 * 24) + sec;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_ConvertDayToDate

  Description:  2000�N����̒ʎZ������ RTCDate �^�̓��t�f�[�^�ɕϊ�����

  Arguments:    date      - �ۑ���̓��t�f�[�^�̃|�C���^.
                day       - 2000�N1��1������̒ʎZ����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void RTC_ConvertDayToDate(RTCDate *date, s32 day)
{
    u32     year;
    s32     month;

    if (day < 0)
    {
        day = 0;
    }
    if (day > 36524)
    {
        day = 36524;
    }

    // 2000�N1��1���͓y�j��
    date->week = (RTCWeek)((day + 6) % 7);

    for (year = 0; year < 99; year++)
    {
        s32     prev = day;
        day -= (RTCi_IsLeapYear(year)) ? 366 : 365;
        if (day < 0)
        {
            day = prev;
            break;
        }
    }
    if (day > 365)
    {
        day = 365;
    }

    date->year = year;

    if (RTCi_IsLeapYear(year))
    {
        if (day < 31 + 29)
        {
            if (day < 31)
            {
                month = 1;
            }
            else
            {
                month = 2;
                day -= 31;
            }
            date->month = (u32)month;
            date->day = (u32)(day + 1);
            return;
        }
        else
        {
            day--;
        }
    }

    for (month = 11; month >= 0; month--)
    {
        if (day >= sDayOfYear[month])
        {
            date->month = (u32)(month + 1);
            date->day = (u32)(day - sDayOfYear[month] + 1);
            return;
        }
    }

    SDK_ASSERT("Internal Error.");
}

/*---------------------------------------------------------------------------*
  Name:         RTCi_ConvertSecondToTime

  Description:  0������̒ʎZ�b���� RTCTime �^�̎����f�[�^�ɕϊ�����

  Arguments:    time      - �ۑ���̓��t�f�[�^�̃|�C���^.
                sec       - 0��0������̒ʎZ�b��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void RTCi_ConvertSecondToTime(RTCTime *time, s32 sec)
{
    if (sec < 0)
    {
        sec = 0;
    }
    if (sec > 86399)
    {
        sec = 86399;
    }

    time->second = (u32)(sec % 60);
    sec /= 60;
    time->minute = (u32)(sec % 60);
    sec /= 60;
    time->hour = (u32)sec;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_ConvertDateTimeToSecond

  Description:  �ʎZ�b���� RTCDate �^�� RTCTime �^�̓����f�[�^��ϊ�����

  Arguments:    date      - �ۑ���̓��t�f�[�^�ւ̃|�C���^.
                time      - �ۑ���̎����f�[�^�ւ̃|�C���^.
                sec       - 2000�N1��1��0��0������̒ʎZ�b��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void RTC_ConvertSecondToDateTime(RTCDate *date, RTCTime *time, s64 sec)
{
    if (sec < 0)
    {
        sec = 0;
    }
    else if (sec > 3155759999)
    {
        sec = 3155759999;
    }
    RTCi_ConvertSecondToTime(time, (s32)(sec % 86400));
    RTC_ConvertDayToDate(date, (s32)(sec / 86400));
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetDayOfWeek

  Description:  RTCDate �^�f�[�^����A�j�����v�Z���ĕԂ��B
                (RTCDate �̒��̗j�����g�p�����Ɍv�Z�ɂ���ċ��߂�)

  Arguments:    date      - ���t�f�[�^�ւ̃|�C���^.

  Returns:      �j��      RTC_WEEK_xxxx
 *---------------------------------------------------------------------------*/
RTCWeek RTC_GetDayOfWeek(RTCDate *date)
{
    int     cent;
    int     year = (int)(2000 + date->year);
    int     month = (int)date->month;
    int     day = (int)date->day;

    month -= 2;
    if (month < 1)
    {
        month += 12;
        --year;
    }

    cent = year / 100;
    year %= 100;
    return (RTCWeek)(((26 * month - 2) / 10 + day + year + year / 4 + cent / 4 + 5 * cent) % 7);
    // RTC_WEEK_xxxx �͓��A���A�c�y��0�`6�Œ�`����Ă��邱�Ƃ𗘗p
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

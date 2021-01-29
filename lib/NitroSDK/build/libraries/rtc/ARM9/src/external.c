/*---------------------------------------------------------------------------*
  Project:  NitroSDK - RTC - libraries
  File:     external.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: external.c,v $
  Revision 1.18  2007/09/24 23:49:43  yosizaki
  update copyright date.

  Revision 1.17  2007/08/06 00:49:44  okubata_ryoma
  small fix

  Revision 1.16  2007/07/31 02:15:14  tokunaga_eiji
  RTC_GetDateTimeExByTick �֐��ƕt����������֐���ǉ��B

  Revision 1.15  2006/07/05 09:12:56  yosizaki
  fix about include header.

  Revision 1.14  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.13  2005/09/30 00:27:04  yada
  calculate day of week instead of reading rtc

  Revision 1.12  2005/02/28 05:26:07  yosizaki
  do-indent.

  Revision 1.11  2004/11/02 04:13:31  terui
  �R�����g�C���B

  Revision 1.10  2004/08/19 13:43:01  terui
  HourFormat�֘A�̒�`���폜�B

  Revision 1.9  2004/08/19 12:50:36  terui
  24���ԕ\�L�Œ艻�ɔ����C���B

  Revision 1.8  2004/08/19 07:49:37  terui
  RTC_GetAlarmStatusAsync�֐�����"Adjust"�ł̏㏑���ҏW�����̏�ԂɏC���B

  Revision 1.7  2004/08/04 05:31:48  miya
  �H��ݒ�p�Ɋ֐��ǉ�

  Revision 1.6  2004/07/01 03:16:52  yosizaki
  change around RTCHourFormat to internal use.

  Revision 1.5  2004/05/31 08:44:45  terui
  PXI�R�[���o�b�N�֐��̃��W�b�N��ύX�B

  Revision 1.4  2004/05/18 01:37:33  terui
  ARM7�Ƃ�PXI�̓����@�\�ɑΉ�

  Revision 1.3  2004/05/18 01:15:12  terui
  ARM7�Ƃ�PXI�̓����@�\�ɑΉ�

  Revision 1.2  2004/05/17 11:05:20  terui
  PXI�������ʂ�API��p�������ʂɉ����B
  �񓯊��֐���BOOL���珈�����ʎ�ʂ�Ԃ��悤�ɕύX�B

  Revision 1.1  2004/05/12 02:36:08  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro/os.h>
#include <nitro/rtc.h>


/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
// �񓯊��֐��r�������p���b�N��`
typedef enum RTCLock
{
    RTC_LOCK_OFF = 0,                  // ���b�N�J�����
    RTC_LOCK_ON,                       // ���b�N�{�����
    RTC_LOCK_MAX
}
RTCLock;

// �A���R�}���h���M���܂ޏ����p�̃V�[�P���X��`
typedef enum RTCSequence
{
    RTC_SEQ_GET_DATE = 0,              // ���t�擾�V�[�P���X
    RTC_SEQ_GET_TIME,                  // �����擾�V�[�P���X
    RTC_SEQ_GET_DATETIME,              // ���t�E�����擾�V�[�P���X
    RTC_SEQ_SET_DATE,                  // ���t�ݒ�V�[�P���X
    RTC_SEQ_SET_TIME,                  // �����ݒ�V�[�P���X
    RTC_SEQ_SET_DATETIME,              // ���t�E�����ݒ�V�[�P���X
    RTC_SEQ_GET_ALARM1_STATUS,         // �A���[���P��Ԏ擾�V�[�P���X
    RTC_SEQ_GET_ALARM2_STATUS,         // �A���[���Q��Ԏ擾�V�[�P���X
    RTC_SEQ_GET_ALARM_PARAM,           // �A���[���ݒ�l�擾�V�[�P���X
    RTC_SEQ_SET_ALARM1_STATUS,         // �A���[���P��ԕύX�V�[�P���X
    RTC_SEQ_SET_ALARM2_STATUS,         // �A���[���Q��ԕύX�V�[�P���X
    RTC_SEQ_SET_ALARM1_PARAM,          // �A���[���P�ݒ�l�ύX�V�[�P���X
    RTC_SEQ_SET_ALARM2_PARAM,          // �A���[���Q�ݒ�l�ύX�V�[�P���X
    RTC_SEQ_SET_HOUR_FORMAT,           // ���ԕ\�L�t�H�[�}�b�g�ύX�V�[�P���X
    RTC_SEQ_SET_REG_STATUS2,           // �X�e�[�^�X�Q���W�X�^�������݃V�[�P���X
    RTC_SEQ_SET_REG_ADJUST,            // �A�W���X�g���W�X�^�������݃V�[�P���X
    RTC_SEQ_MAX
}
RTCSequence;

// ���[�N�p�\����
typedef struct RTCWork
{
    u32     lock;                      // �r�����b�N
    RTCCallback callback;              // �񓯊��֐��R�[���o�b�N�ޔ�p
    void   *buffer[2];                 // �񓯊��֐��p�����[�^�ޔ�p
    void   *callbackArg;               // �R�[���o�b�N�֐��̈����ۑ��p
    u32     sequence;                  // �A���������[�h�Ǘ��p
    u32     index;                     // �A�������̏󋵊Ǘ��p
    RTCInterrupt interrupt;            // �A���[���ʒm���̌Ăяo���֐��ޔ�p
    RTCResult commonResult;            // �񓯊��֐��̏������ʑޔ�p

}
RTCWork;

/*---------------------------------------------------------------------------*
    �ÓI�ϐ���`
 *---------------------------------------------------------------------------*/
static u16 rtcInitialized;             // �������m�F�t���O
static RTCWork rtcWork;                // ���[�N�ϐ����܂Ƃ߂��\����
static u16     rtcTickInitialized;     // �`�b�N�������m�F�t���O
static OSTick  rtcInitialTotalTicks;   // �`�b�N���������ɕۑ������ 2000/01/01
                                       // 00:00:00 ����̑��`�b�N���B

/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void RtcCommonCallback(PXIFifoTag tag, u32 data, BOOL err);
static u32 RtcBCD2HEX(u32 bcd);
static u32 RtcHEX2BCD(u32 hex);
static BOOL RtcCheckAlarmParam(const RTCAlarmParam *param);
static RTCRawAlarm RtcMakeAlarmParam(const RTCAlarmParam *param);
static BOOL RtcCheckDate(const RTCDate *date, RTCRawDate *raw);
static BOOL RtcCheckTime(const RTCTime *time, RTCRawTime *raw);
static void RtcGetResultCallback(RTCResult result, void *arg);
static void RtcWaitBusy(void);
static RTCResult RtcTickInit(void);
static void RtcConvertTickToDateTimeEx(RTCDate * date, RTCTimeEx *time, OSTick tick);

/*---------------------------------------------------------------------------*
  Name:         RTC_Init

  Description:  RTC���C�u����������������B
       Notice:  �R���|�[�l���g���̏������ɂ���ēd�������`�F�b�N���s����B
                ���̍ہARTC�ւ̓d����������U��~����Ă����ꍇ��RTC������Ԃ�
                ���Z�b�g����A2000/01/01/00:00:00(12���ԕ\�L)0�j��(���j��)�ɂȂ�B
                �A���[���ݒ�l��0�N���A�����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void RTC_Init(void)
{
    // ���������m�F
    if (rtcInitialized)
    {
        return;
    }
    rtcInitialized = 1;

    // ���[�N�p�ϐ�������
    rtcWork.lock = RTC_LOCK_OFF;
    rtcWork.callback = NULL;
    rtcWork.interrupt = NULL;
    rtcWork.buffer[0] = NULL;
    rtcWork.buffer[1] = NULL;

    // ARM7��RTC���C�u�������J�n�����܂ő҂�
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_RTC, PXI_PROC_ARM7))
    {
    }

    // PXI�R�[���o�b�N�֐���ݒ�
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_RTC, RtcCommonCallback);
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetDateAsync

  Description:  RTC������t�f�[�^��񓯊��œǂݏo���B

  Arguments:    date      - ���t�f�[�^���i�[����o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_GetDateAsync(RTCDate *date, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_ASSERT(date != NULL);
    SDK_ASSERT(callback != NULL);

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // ���t�ǂݏo���R�}���h�𑗐M
    rtcWork.sequence = RTC_SEQ_GET_DATE;
    rtcWork.index = 0;
    rtcWork.buffer[0] = (void *)date;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_ReadRawDateAsync())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetDate

  Description:  RTC������t�f�[�^��ǂݏo���B

  Arguments:    date      - ���t�f�[�^���i�[����o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_GetDate(RTCDate *date)
{
    rtcWork.commonResult = RTC_GetDateAsync(date, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetTimeAsync

  Description:  RTC���玞���f�[�^��񓯊��œǂݏo���B

  Arguments:    time      - �����f�[�^���i�[����o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_GetTimeAsync(RTCTime *time, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(time);
    SDK_NULL_ASSERT(callback);

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // �����ǂݏo���R�}���h�𑗐M
    rtcWork.sequence = RTC_SEQ_GET_TIME;
    rtcWork.index = 0;
    rtcWork.buffer[0] = (void *)time;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_ReadRawTimeAsync())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetTime

  Description:  RTC���玞���f�[�^��ǂݏo���B

  Arguments:    time      - �����f�[�^���i�[����o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_GetTime(RTCTime *time)
{
    rtcWork.commonResult = RTC_GetTimeAsync(time, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetDateTimeAsync

  Description:  RTC������t�E�����f�[�^��񓯊��œǂݏo���B

  Arguments:    date      - ���t�f�[�^���i�[����o�b�t�@���w��B
                time      - �����f�[�^���i�[����o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_GetDateTimeAsync(RTCDate *date, RTCTime *time, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(date);
    SDK_NULL_ASSERT(time);
    SDK_NULL_ASSERT(callback);

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // �����ǂݏo���R�}���h�𑗐M
    rtcWork.sequence = RTC_SEQ_GET_DATETIME;
    rtcWork.index = 0;
    rtcWork.buffer[0] = (void *)date;
    rtcWork.buffer[1] = (void *)time;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_ReadRawDateTimeAsync())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetDateTime

  Description:  RTC������t�E�����f�[�^��ǂݏo���B

  Arguments:    date      - ���t�f�[�^���i�[����o�b�t�@���w��B
                time      - �����f�[�^���i�[����o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_GetDateTime(RTCDate *date, RTCTime *time)
{
    rtcWork.commonResult = RTC_GetDateTimeAsync(date, time, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetDateTimeExByTick

  Description:  Gets date and time by coverting total CPU ticks progressed
                since 2000/01/01/00:00:00. The total ticks are calculted by
                adding current ticks and initial total ticks. The initial
                ticks are calculated and saved at the first call. OS_InitTick
                must be called before calling this function.

  Arguments:    date      - buffer for storing RTCDate
                time      - buffer for storing RTCTimeEx

  Returns:      RTCResult - first call: result of initial RTC_GetDateTime
                            second call~: RTC_RESULT_SUCCESS
 *---------------------------------------------------------------------------*/
RTCResult RTC_GetDateTimeExByTick(RTCDate *date, RTCTimeEx *time)
{
    RTCResult result;
    OSTick totalTicks;
    
    if(!rtcTickInitialized){
        result = RtcTickInit();
    }else{
        result = RTC_RESULT_SUCCESS;
    }

    SDK_ASSERT(OS_IsTickAvailable());

    // culcurate total ticks progressed since 2000/01/01/00:00:00
    totalTicks = OS_GetTick() + rtcInitialTotalTicks;

    RtcConvertTickToDateTimeEx(date, time, totalTicks);
    
    return result;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetDateAsync

  Description:  RTC�ɓ��t�f�[�^��񓯊��ŏ������ށB

  Arguments:    date      - ���t�f�[�^���i�[����Ă���o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_SetDateAsync(const RTCDate *date, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(date);
    SDK_NULL_ASSERT(callback);

    // �Z�b�g������t���m�F�A�ҏW
    if (!RtcCheckDate(date, &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.date)))
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // ���t�������݃R�}���h�𑗐M
    rtcWork.sequence = RTC_SEQ_SET_DATE;
    rtcWork.index = 0;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_WriteRawDateAsync())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetDate

  Description:  RTC�ɓ��t�f�[�^���������ށB

  Arguments:    date      - ���t�f�[�^���i�[����Ă���o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_SetDate(const RTCDate *date)
{
    rtcWork.commonResult = RTC_SetDateAsync(date, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetTimeAsync

  Description:  RTC�Ɏ����f�[�^��񓯊��ŏ������ށB

  Arguments:    time      - �����f�[�^���i�[����Ă���o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_SetTimeAsync(const RTCTime *time, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(time);
    SDK_NULL_ASSERT(callback);

    // �Z�b�g���鎞�����m�F�A�ҏW
    if (!RtcCheckTime(time, &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.time)))
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // �����������݃R�}���h�𑗐M
    rtcWork.sequence = RTC_SEQ_SET_TIME;
    rtcWork.index = 0;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_WriteRawTimeAsync())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetTime

  Description:  RTC�Ɏ����f�[�^���������ށB

  Arguments:    time      - �����f�[�^���i�[����Ă���o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̌��ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_SetTime(const RTCTime *time)
{
    rtcWork.commonResult = RTC_SetTimeAsync(time, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetDateTimeAsync

  Description:  RTC�ɓ��t�E�����f�[�^��񓯊��ŏ������ށB

  Arguments:    date      - ���t�f�[�^���i�[����Ă���o�b�t�@���w��B
                time      - �����f�[�^���i�[����Ă���o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult
RTC_SetDateTimeAsync(const RTCDate *date, const RTCTime *time, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(date);
    SDK_NULL_ASSERT(time);
    SDK_NULL_ASSERT(callback);

    // �Z�b�g������t�E�������m�F�A�ҏW
    if (!RtcCheckDate(date, &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.date)))
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }
    if (!RtcCheckTime(time, &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.time)))
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // ���t�E�����������݃R�}���h�𑗐M
    rtcWork.sequence = RTC_SEQ_SET_DATETIME;
    rtcWork.index = 0;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_WriteRawDateTimeAsync())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetDateTime

  Description:  RTC�ɓ��t�E�����f�[�^���������ށB

  Arguments:    date      - ���t�f�[�^���i�[����Ă���o�b�t�@���w��B
                time      - �����f�[�^���i�[����Ă���o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_SetDateTime(const RTCDate *date, const RTCTime *time)
{
    rtcWork.commonResult = RTC_SetDateTimeAsync(date, time, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}




/*---------------------------------------------------------------------------*
  Name:         RTC_SetRegStatus2Async

  Description:  RTC�X�e�[�^�X�Q���W�X�^�Ƀf�[�^����������

  Arguments:    status2   - �X�e�[�^�X�Q���W�X�^���e���i�[����Ă���o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTCi_SetRegStatus2Async(const RTCRawStatus2 *status2, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(status2);
    SDK_NULL_ASSERT(callback);

    ((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2.intr_mode = status2->intr_mode;
    ((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2.intr2_mode =
        status2->intr2_mode;
    ((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2.test = status2->test;

    /* �p�����[�^�`�F�b�N���� */
    // return RTC_RESULT_ILLEGAL_PARAMETER;


    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // �X�e�[�^�X�Q���W�X�^�������݃R�}���h�𑗐M
    rtcWork.sequence = RTC_SEQ_SET_REG_STATUS2;
    rtcWork.index = 0;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_WriteRawStatus2Async())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetRegStatus2

  Description:  RTC�X�e�[�^�X�Q���W�X�^�Ƀf�[�^����������

  Arguments:    status2   - �X�e�[�^�X�Q���W�X�^���e���i�[����Ă���o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTCi_SetRegStatus2(const RTCRawStatus2 *status2)
{
    rtcWork.commonResult = RTCi_SetRegStatus2Async(status2, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}


/*---------------------------------------------------------------------------*
  Name:         RTC_SetRegAdjustAsync

  Description:  RTC�A�W���X�g���W�X�^�Ƀf�[�^����������

  Arguments:    adjust    - �A�W���X�g���W�X�^���e���i�[����Ă���o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTCi_SetRegAdjustAsync(const RTCRawAdjust *adjust, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(adjust);
    SDK_NULL_ASSERT(callback);

    ((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.adjust.adjust = adjust->adjust;

    /* �p�����[�^�`�F�b�N���� */
    // return RTC_RESULT_ILLEGAL_PARAMETER;

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // �X�e�[�^�X�Q���W�X�^�������݃R�}���h�𑗐M
    rtcWork.sequence = RTC_SEQ_SET_REG_ADJUST;
    rtcWork.index = 0;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_WriteRawAdjustAsync())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}


/*---------------------------------------------------------------------------*
  Name:         RTC_SetRegAdjust

  Description:  RTC�A�W���X�g���W�X�^�Ƀf�[�^����������

  Arguments:    status2   - �A�W���X�g���W�X�^���e���i�[����Ă���o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTCi_SetRegAdjust(const RTCRawAdjust *Adjust)
{
    rtcWork.commonResult = RTCi_SetRegAdjustAsync(Adjust, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}



/*---------------------------------------------------------------------------*
  Name:         RTC_GetAlarmStatusAsync

  Description:  RTC����A���[����ON/OFF��Ԃ�񓯊��œǂݏo���B

  Arguments:    chan      - �A���[���̃`�����l�����w��B
                status    - �A���[����Ԃ��i�[����o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult
RTC_GetAlarmStatusAsync(RTCAlarmChan chan, RTCAlarmStatus *status, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(status);
    SDK_NULL_ASSERT(callback);

    // �p�����[�^���m�F
    if (chan >= RTC_ALARM_CHAN_MAX)
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }
    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // �X�e�[�^�X�Q���W�X�^�ǂݏo���R�}���h�𑗐M
    switch (chan)
    {
    case RTC_ALARM_CHAN_1:
        rtcWork.sequence = RTC_SEQ_GET_ALARM1_STATUS;
        break;
    case RTC_ALARM_CHAN_2:
        rtcWork.sequence = RTC_SEQ_GET_ALARM2_STATUS;
        break;
    }
    rtcWork.index = 0;
    rtcWork.buffer[0] = (void *)status;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_ReadRawStatus2Async())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetAlarmStatus

  Description:  RTC����A���[����ON/OFF��Ԃ�ǂݏo���B

  Arguments:    chan      - �A���[���̃`�����l�����w��B
                status    - �A���[����Ԃ��i�[����o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_GetAlarmStatus(RTCAlarmChan chan, RTCAlarmStatus *status)
{
    rtcWork.commonResult = RTC_GetAlarmStatusAsync(chan, status, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetAlarmParamAsync

  Description:  RTC����A���[���̐ݒ�l��񓯊��œǂݏo���B

  Arguments:    chan      - �A���[���̃`�����l�����w��B
                param     - �A���[���ݒ�l���i�[����o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult
RTC_GetAlarmParamAsync(RTCAlarmChan chan, RTCAlarmParam *param, RTCCallback callback, void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(param);
    SDK_NULL_ASSERT(callback);

    // �p�����[�^�m�F
    if (chan >= RTC_ALARM_CHAN_MAX)
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // �A���[���Por�Q�ݒ�l�ǂݏo���R�}���h�𑗐M
    rtcWork.sequence = RTC_SEQ_GET_ALARM_PARAM;
    rtcWork.index = 0;
    rtcWork.buffer[0] = (void *)param;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (chan == RTC_ALARM_CHAN_1)
    {
        if (RTCi_ReadRawAlarm1Async())
        {
            return RTC_RESULT_SUCCESS;
        }
        else
        {
            return RTC_RESULT_SEND_ERROR;
        }
    }
    if (RTCi_ReadRawAlarm2Async())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_GetAlarmParam

  Description:  RTC����A���[���̐ݒ�l��ǂݏo���B

  Arguments:    chan      - �A���[���̃`�����l�����w��B
                param     - �A���[���ݒ�l���i�[����o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_GetAlarmParam(RTCAlarmChan chan, RTCAlarmParam *param)
{
    rtcWork.commonResult = RTC_GetAlarmParamAsync(chan, param, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetAlarmInterrupt

  Description:  �A���[�������ݔ������̃R�[���o�b�N�֐���ݒ肷��B

  Arguments:    interrupt - �R�[���o�b�N�֐����w��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void RTC_SetAlarmInterrupt(RTCInterrupt interrupt)
{
    rtcWork.interrupt = interrupt;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetAlarmStatusAsync

  Description:  RTC�ɃA���[����Ԃ�񓯊��ŏ������ށB

  Arguments:    chan      - �A���[���̃`�����l�����w��B
                status    - �A���[����Ԃ��i�[����Ă���o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult
RTC_SetAlarmStatusAsync(RTCAlarmChan chan, const RTCAlarmStatus *status, RTCCallback callback,
                        void *arg)
{
    OSIntrMode enabled;

    SDK_NULL_ASSERT(status);
    SDK_NULL_ASSERT(callback);

    // �p�����[�^�m�F
    if (chan >= RTC_ALARM_CHAN_MAX)
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }
    if (*status > RTC_ALARM_STATUS_ON)
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // �X�e�[�^�X�Q���W�X�^�ǂݏo���R�}���h�𑗐M
    switch (chan)
    {
    case RTC_ALARM_CHAN_1:
        rtcWork.sequence = RTC_SEQ_SET_ALARM1_STATUS;
        break;
    case RTC_ALARM_CHAN_2:
        rtcWork.sequence = RTC_SEQ_SET_ALARM2_STATUS;
        break;
    }
    rtcWork.index = 0;
    rtcWork.buffer[0] = (void *)status;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_ReadRawStatus2Async())
    {
        return RTC_RESULT_SUCCESS;
    }
    else
    {
        return RTC_RESULT_SEND_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetAlarmStatus

  Description:  RTC�ɃA���[����Ԃ��������ށB

  Arguments:    chan      - �A���[���̃`�����l�����w��B
                status    - �A���[����Ԃ��i�[����Ă���o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_SetAlarmStatus(RTCAlarmChan chan, const RTCAlarmStatus *status)
{
    rtcWork.commonResult = RTC_SetAlarmStatusAsync(chan, status, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetAlarmParamAsync

  Description:  RTC�ɃA���[���ݒ��񓯊��ŏ������ށB
       Notice:  RTC�̃A���[����Ԃ�ON�ɂȂ��Ă��Ȃ��ꍇ�A�f�o�C�X���Ń��C�g��
                �󂯕t���Ă���Ȃ��̂ŁA�������݂͎��s����B

  Arguments:    chan      - �A���[���̃`�����l�����w��B
                param     - �A���[���ݒ�l���i�[����Ă���o�b�t�@���w��B
                callback  - �񓯊����������������ۂɌĂяo���֐����w��B
                arg       - �R�[���o�b�N�֐��Ăяo�����̈������w��B

  Returns:      RTCResult - �񓯊��f�o�C�X����J�n�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult
RTC_SetAlarmParamAsync(RTCAlarmChan chan, const RTCAlarmParam *param, RTCCallback callback,
                       void *arg)
{
    OSIntrMode enabled;
    RTCRawAlarm *pAlarm = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.alarm);
    BOOL    result = FALSE;

    SDK_NULL_ASSERT(param);
    SDK_NULL_ASSERT(callback);

    // �p�����[�^���m�F
    if (chan >= RTC_ALARM_CHAN_MAX)
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }
    if (!RtcCheckAlarmParam(param))
    {
        return RTC_RESULT_ILLEGAL_PARAMETER;
    }

    // ���b�N�m�F
    enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF)
    {
        (void)OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    (void)OS_RestoreInterrupts(enabled);

    // �ݒ肷��f�[�^��ҏW
    rtcWork.index = 0;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    ((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.alarm = RtcMakeAlarmParam(param);
    // �A���[���ԍ��ɂ�著�M�R�}���h��U�蕪��
    switch (chan)
    {
    case RTC_ALARM_CHAN_1:
        // �A���[���P���W�X�^�������݃R�}���h�𑗐M
        rtcWork.sequence = RTC_SEQ_SET_ALARM1_PARAM;
        result = RTCi_WriteRawAlarm1Async();
        break;
    case RTC_ALARM_CHAN_2:
        // �A���[���Q���W�X�^�������݃R�}���h�𑗐M
        rtcWork.sequence = RTC_SEQ_SET_ALARM2_PARAM;
        result = RTCi_WriteRawAlarm2Async();
        break;
    }
    if (result)
    {
        return RTC_RESULT_SUCCESS;
    }
    return RTC_RESULT_SEND_ERROR;
}

/*---------------------------------------------------------------------------*
  Name:         RTC_SetAlarmParam

  Description:  RTC�ɃA���[���ݒ�l���������ށB

  Arguments:    chan      - �A���[���̃`�����l�����w��B
                param     - �A���[���ݒ�l���i�[����Ă���o�b�t�@���w��B

  Returns:      RTCResult - �f�o�C�X����̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
RTCResult RTC_SetAlarmParam(RTCAlarmChan chan, const RTCAlarmParam *param)
{
    rtcWork.commonResult = RTC_SetAlarmParamAsync(chan, param, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS)
    {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

/*---------------------------------------------------------------------------*
  Name:         RtcCommonCallback

  Description:  �񓯊�RTC�֐��p�̋��ʃR�[���o�b�N�֐��B

  Arguments:    tag -  PXI tag which show message type.
                data - message from ARM7.
                err -  PXI transfer error flag.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void RtcCommonCallback(PXIFifoTag tag, u32 data, BOOL err)
{
#pragma unused( tag )

    RTCResult result;
    RTCPxiResult pxiresult;
    u8      command;
    RTCCallback cb;

    // PXI�ʐM�G���[���m�F
    if (err)
    {
        // �V�[�P���X�������I��
        if (rtcWork.index)
        {
            rtcWork.index = 0;
        }
        if (rtcWork.lock != RTC_LOCK_OFF)
        {
            rtcWork.lock = RTC_LOCK_OFF;
        }
        if (rtcWork.callback)
        {
            cb = rtcWork.callback;
            rtcWork.callback = NULL;
            cb(RTC_RESULT_FATAL_ERROR, rtcWork.callbackArg);
        }
        return;
    }

    // ��M�f�[�^�����
    command = (u8)((data & RTC_PXI_COMMAND_MASK) >> RTC_PXI_COMMAND_SHIFT);
    pxiresult = (RTCPxiResult)((data & RTC_PXI_RESULT_MASK) >> RTC_PXI_RESULT_SHIFT);

    // �A���[�������݂��m�F
    if (command == RTC_PXI_COMMAND_INTERRUPT)
    {
        // pxiresult�ɂăA���[���Por�Q�𔻒f�\�����A
        // �R�[���o�b�N�𓝈ꂵ�Ă���̂łǂ���ł��邩�͖�������B
        if (rtcWork.interrupt)
        {
            // �A���[�������ݒʒm���R�[���o�b�N
            rtcWork.interrupt();
        }
        return;
    }

    // �������������Ȃ�A������Ԃɉ����Ċe�펖�㏈��
    if (pxiresult == RTC_PXI_RESULT_SUCCESS)
    {
        result = RTC_RESULT_SUCCESS;
        switch (rtcWork.sequence)
        {
            // ���t�擾�V�[�P���X
        case RTC_SEQ_GET_DATE:
            {
                RTCDate *pDst = (RTCDate *)(rtcWork.buffer[0]);
                RTCRawDate *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.date);

                pDst->year = RtcBCD2HEX(pSrc->year);
                pDst->month = RtcBCD2HEX(pSrc->month);
                pDst->day = RtcBCD2HEX(pSrc->day);
                pDst->week = RTC_GetDayOfWeek(pDst);
            }
            break;
            // �����擾�V�[�P���X
        case RTC_SEQ_GET_TIME:
            {
                RTCTime *pDst = (RTCTime *)(rtcWork.buffer[0]);
                RTCRawTime *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.time);

                pDst->hour = RtcBCD2HEX(pSrc->hour);
                pDst->minute = RtcBCD2HEX(pSrc->minute);
                pDst->second = RtcBCD2HEX(pSrc->second);
            }
            break;
            // ���t�E�����擾�V�[�P���X
        case RTC_SEQ_GET_DATETIME:
            {
                RTCDate *pDst = (RTCDate *)(rtcWork.buffer[0]);
                RTCRawDate *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.date);

                //pDst->year =  RtcBCD2HEX( pSrc->year );   // �Ȃ����l���n����Ȃ��̂ŉ��̃R�[�h�ɕύX
                pDst->year = RtcBCD2HEX(*(u32 *)pSrc & 0x000000ff);
                pDst->month = RtcBCD2HEX(pSrc->month);
                pDst->day = RtcBCD2HEX(pSrc->day);
                pDst->week = RTC_GetDayOfWeek(pDst);
            }
            {
                RTCTime *pDst = (RTCTime *)(rtcWork.buffer[1]);
                RTCRawTime *pSrc = &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->t.time);

                pDst->hour = RtcBCD2HEX(pSrc->hour);
                pDst->minute = RtcBCD2HEX(pSrc->minute);
                pDst->second = RtcBCD2HEX(pSrc->second);
            }
            break;
            // ���t�ύX�V�[�P���X
        case RTC_SEQ_SET_DATE:
        case RTC_SEQ_SET_TIME:
        case RTC_SEQ_SET_DATETIME:
            // ���ɏ����Ȃ�
            break;
            // �A���[���P��Ԏ擾�V�[�P���X
        case RTC_SEQ_GET_ALARM1_STATUS:
            {
                RTCAlarmStatus *pDst = (RTCAlarmStatus *)(rtcWork.buffer[0]);
                RTCRawStatus2 *pSrc =
                    &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2);

                switch (pSrc->intr_mode)
                {
                case RTC_INTERRUPT_MODE_ALARM:
                    *pDst = RTC_ALARM_STATUS_ON;
                    break;
                default:
                    *pDst = RTC_ALARM_STATUS_OFF;
                }
            }
            break;
            // �A���[���Q��Ԏ擾�V�[�P���X
        case RTC_SEQ_GET_ALARM2_STATUS:
            {
                RTCAlarmStatus *pDst = (RTCAlarmStatus *)(rtcWork.buffer[0]);
                RTCRawStatus2 *pSrc =
                    &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2);

                if (pSrc->intr2_mode)
                {
                    *pDst = RTC_ALARM_STATUS_ON;
                }
                else
                {
                    *pDst = RTC_ALARM_STATUS_OFF;
                }
            }
            break;
            // �A���[���Por�Q�ݒ�l�擾�V�[�P���X
        case RTC_SEQ_GET_ALARM_PARAM:
            {
                RTCAlarmParam *pDst = (RTCAlarmParam *)(rtcWork.buffer[0]);
                RTCRawAlarm *pSrc =
                    &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.alarm);

                pDst->week = (RTCWeek)(pSrc->week);
                pDst->hour = RtcBCD2HEX(pSrc->hour);
                pDst->minute = RtcBCD2HEX(pSrc->minute);
                pDst->enable = RTC_ALARM_ENABLE_NONE;
                if (pSrc->we)
                    pDst->enable += RTC_ALARM_ENABLE_WEEK;
                if (pSrc->he)
                    pDst->enable += RTC_ALARM_ENABLE_HOUR;
                if (pSrc->me)
                    pDst->enable += RTC_ALARM_ENABLE_MINUTE;
            }
            break;
            // �A���[���P��Ԑݒ�V�[�P���X
        case RTC_SEQ_SET_ALARM1_STATUS:
            if (rtcWork.index == 0)
            {
                RTCRawStatus2 *pSrc =
                    &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2);

                // �X�e�[�^�X�Q���W�X�^�ǂݏo������
                if (*(RTCAlarmStatus *)(rtcWork.buffer[0]) == RTC_ALARM_STATUS_ON)
                {
                    // �����݋�����ꍇ
                    if (pSrc->intr_mode != RTC_INTERRUPT_MODE_ALARM)
                    {
                        // �X�e�[�^�X�Q���W�X�^��������
                        rtcWork.index++;        // ���̃V�[�P���X�ֈڍs
                        pSrc->intr_mode = RTC_INTERRUPT_MODE_ALARM;
                        if (!RTCi_WriteRawStatus2Async())
                        {
                            rtcWork.index = 0;  // �V�[�P���X�𒆒f
                            result = RTC_RESULT_SEND_ERROR;
                        }
                    }
                }
                else
                {
                    // �����݋֎~����ꍇ
                    if (pSrc->intr_mode != RTC_INTERRUPT_MODE_NONE)
                    {
                        // �X�e�[�^�X�Q���W�X�^��������
                        rtcWork.index++;        // ���̃V�[�P���X�ֈڍs
                        pSrc->intr_mode = RTC_INTERRUPT_MODE_NONE;
                        if (!RTCi_WriteRawStatus2Async())
                        {
                            rtcWork.index = 0;  // �V�[�P���X�𒆒f
                            result = RTC_RESULT_SEND_ERROR;
                        }
                    }
                }
            }
            else
            {
                // �X�e�[�^�X�Q���W�X�^�������݌���
                rtcWork.index = 0;     // �V�[�P���X�I��
            }
            break;
            // �A���[���Q��Ԑݒ�V�[�P���X
        case RTC_SEQ_SET_ALARM2_STATUS:
            if (rtcWork.index == 0)
            {
                RTCRawStatus2 *pSrc =
                    &(((RTCRawData *)(OS_GetSystemWork()->real_time_clock))->a.status2);

                // �X�e�[�^�X�Q���W�X�^�ǂݏo������
                if (*(RTCAlarmStatus *)(rtcWork.buffer[0]) == RTC_ALARM_STATUS_ON)
                {
                    // �����݋�����ꍇ
                    if (!pSrc->intr2_mode)
                    {
                        // �X�e�[�^�X�Q���W�X�^��������
                        rtcWork.index++;        // ���̃V�[�P���X�ֈڍs
                        pSrc->intr2_mode = 1;
                        if (!RTCi_WriteRawStatus2Async())
                        {
                            rtcWork.index = 0;  // �V�[�P���X�𒆒f
                            result = RTC_RESULT_SEND_ERROR;
                        }
                    }
                }
                else
                {
                    // �����݋֎~����ꍇ
                    if (pSrc->intr2_mode)
                    {
                        // �X�e�[�^�X�Q���W�X�^��������
                        rtcWork.index++;        // ���̃V�[�P���X�ֈڍs
                        pSrc->intr2_mode = 0;
                        if (!RTCi_WriteRawStatus2Async())
                        {
                            rtcWork.index = 0;  // �V�[�P���X�𒆒f
                            result = RTC_RESULT_SEND_ERROR;
                        }
                    }
                }
            }
            else
            {
                // �X�e�[�^�X�Q���W�X�^�������݌���
                rtcWork.index = 0;     // �V�[�P���X�I��
            }
            break;
            // �A���[���P�p�����[�^�ݒ�V�[�P���X
        case RTC_SEQ_SET_ALARM1_PARAM:
            // �A���[���Q�p�����[�^�ݒ�V�[�P���X
        case RTC_SEQ_SET_ALARM2_PARAM:
            // ���ԕ\�L�ύX�V�[�P���X
        case RTC_SEQ_SET_HOUR_FORMAT:
            // �X�e�[�^�X�Q���W�X�^�������݃V�[�P���X
        case RTC_SEQ_SET_REG_STATUS2:
            // �A�W���X�g���W�X�^�������݃V�[�P���X
        case RTC_SEQ_SET_REG_ADJUST:
            // ���ɏ����Ȃ�
            break;

            // ���̑���̃V�[�P���X
        default:
            result = RTC_RESULT_INVALID_COMMAND;
            rtcWork.index = 0;
        }
    }
    else
    {
        // �R�}���h�Ɏ��s�������Ԃ��ꂽ�̂ŃV�[�P���X���f
        rtcWork.index = 0;
        // PXI�ʐM������ʂ��珈�����ʂ�����
        switch (pxiresult)
        {
        case RTC_PXI_RESULT_INVALID_COMMAND:
            result = RTC_RESULT_INVALID_COMMAND;
            break;
        case RTC_PXI_RESULT_ILLEGAL_STATUS:
            result = RTC_RESULT_ILLEGAL_STATUS;
            break;
        case RTC_PXI_RESULT_BUSY:
            result = RTC_RESULT_BUSY;
            break;
        case RTC_PXI_RESULT_FATAL_ERROR:
        default:
            result = RTC_RESULT_FATAL_ERROR;
        }
    }

    // �A�������V�[�P���X�������Ȃ�΁A�V�[�P���X�̏I������
    if (rtcWork.index == 0)
    {
        // �r�����b�N�J��
        if (rtcWork.lock != RTC_LOCK_OFF)
        {
            rtcWork.lock = RTC_LOCK_OFF;
        }
        // �R�[���o�b�N�֐����Ăяo��
        if (rtcWork.callback)
        {
            cb = rtcWork.callback;
            rtcWork.callback = NULL;
            cb(result, rtcWork.callbackArg);
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         RtcBCD2HEX

  Description:  BCD�^�ŕ\�����ꂽ���l����ʓI��u32�ŕ\���������l�ɕϊ�����B

  Arguments:    bcd  - BCD�^�ŕ\�����ꂽ���l�B

  Returns:      u32  - ��ʓI��u32�ŕ\�������������l�B
                       ���̓p�����[�^��BCD�^�łȂ��ꍇ�A0��Ԃ��B
 *---------------------------------------------------------------------------*/
static u32 RtcBCD2HEX(u32 bcd)
{
    u32     hex = 0;
    s32     i;
    s32     w;

    // 0xA ~ 0xF ���ǂ��̌��ɂ��܂܂�Ă��Ȃ����Ƃ��m�F
    for (i = 0; i < 8; i++)
    {
        if (((bcd >> (i * 4)) & 0x0000000f) >= 0x0a)
        {
            return hex;                // �ϊ��𒆒f���ċ����I��"0"��Ԃ�
        }
    }

    // �ϊ����[�v
    for (i = 0, w = 1; i < 8; i++, w *= 10)
    {
        hex += (((bcd >> (i * 4)) & 0x0000000f) * w);
    }
    return hex;
}

/*---------------------------------------------------------------------------*
  Name:         RtcHEX2BCD

  Description:  ��ʓI��u32�ŕ\���������l��BCD�^�ŕ\�����ꂽ���l�ɕϊ�����B

  Arguments:    hex  - ��ʓI��u32�ŕ\���������l�B

  Returns:      u32  - BCD�^�ŕ\�������������l�B
                       ���̓p�����[�^��BCD�^�ŕ\���ł��Ȃ��ꍇ�A0��Ԃ��B
 *---------------------------------------------------------------------------*/
static u32 RtcHEX2BCD(u32 hex)
{
    u32     bcd = 0;
    s32     i;
    u32     w;

    // 99999999���z���Ă��Ȃ����Ƃ��m�F
    if (hex > 99999999)
    {
        return 0;
    }

    // �ϊ����[�v
    for (i = 0, w = hex; i < 8; i++)
    {
        bcd += ((w % 10) << (i * 4));
        w /= 10;
    }
    return bcd;
}

/*---------------------------------------------------------------------------*
  Name:         RtcCheckAlarmParam

  Description:  �A���[���ݒ�l��RTC�ɃZ�b�g���Ė��Ȃ��l���ǂ������`�F�b�N����B

  Arguments:    param  - �`�F�b�N����A���[���ݒ�l�B

  Returns:      BOOL   - �A���[���ݒ�l�Ƃ��Ė��Ȃ��ꍇ��TRUE���A���炩��
                         ��肪����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL RtcCheckAlarmParam(const RTCAlarmParam *param)
{
    if (param->week >= RTC_WEEK_MAX)
        return FALSE;
    if (param->hour >= 24)
        return FALSE;
    if (param->minute >= 60)
        return FALSE;
    if (param->enable & ~RTC_ALARM_ENABLE_ALL)
        return FALSE;
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         RtcMakeAlarmParam

  Description:  �A���[���ݒ�l��RTC�ɃZ�b�g�ł���`�ɕϊ�����B

  Arguments:    param  - �ϊ�����ΏۂƂȂ�A���[���ݒ�l�B

  Returns:      RTCRawAlarm - RTC�ɃZ�b�g�ł���`�ɕϊ����ꂽ�f�[�^�B
 *---------------------------------------------------------------------------*/
static RTCRawAlarm RtcMakeAlarmParam(const RTCAlarmParam *param)
{
    RTCRawAlarm dst;

    // �߂�l��0�N���A
    *((u32 *)(&dst)) = 0;

    // �O�̂��ߐݒ�l�̐��������m�F
    if (!RtcCheckAlarmParam(param))
    {
        return dst;
    }

    // �j���f�[�^
    dst.week = (u32)(param->week);
    // ���ԃf�[�^�A�y�ьߑO�E�ߌ�t���O
    if (param->hour >= 12)
    {
        dst.afternoon = 1;
    }
    dst.hour = RtcHEX2BCD(param->hour);
    // ���f�[�^
    dst.minute = RtcHEX2BCD(param->minute);
    // �L���t���O
    if (param->enable & RTC_ALARM_ENABLE_WEEK)
    {
        dst.we = 1;
    }
    if (param->enable & RTC_ALARM_ENABLE_HOUR)
    {
        dst.he = 1;
    }
    if (param->enable & RTC_ALARM_ENABLE_MINUTE)
    {
        dst.me = 1;
    }

    return dst;
}

/*---------------------------------------------------------------------------*
  Name:         RtcCheckDate

  Description:  ���t��RTC�ɃZ�b�g���Ė��Ȃ��l���ǂ������`�F�b�N����B
                ���Ȃ����RTC�ɃZ�b�g�ł���`�ɕҏW����B

  Arguments:    date - �`�F�b�N������t����́B
                raw  - RTC�ɃZ�b�g�ł���`�ɕҏW���ꂽ�f�[�^���o�́B

  Returns:      BOOL - �`�F�b�N���Ė�肪����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL RtcCheckDate(const RTCDate *date, RTCRawDate *raw)
{
    // �e�����o�����e�͈͓����`�F�b�N
    if (date->year >= 100)
        return FALSE;
    if ((date->month < 1) || (date->month > 12))
        return FALSE;
    if ((date->day < 1) || (date->day > 31))
        return FALSE;
    if (date->week >= RTC_WEEK_MAX)
        return FALSE;

    // ���f�[�^�̌^�ɕҏW
    //raw->year  = RtcHEX2BCD( date->year );    // �Ȃ����l���i�[����Ȃ��̂ŉ��̃R�[�h�ɕύX
    *(u32 *)raw = RtcHEX2BCD(date->year);
    raw->month = RtcHEX2BCD(date->month);
    raw->day = RtcHEX2BCD(date->day);
    raw->week = (u32)(date->week);
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         RtcCheckTime

  Description:  ������RTC�ɃZ�b�g���Ė��Ȃ��l���ǂ������`�F�b�N����B
                ���Ȃ����RTC�ɃZ�b�g�ł���`�ɕҏW����B

  Arguments:    date - �`�F�b�N���鎞������́B
                raw  - RTC�ɃZ�b�g�ł���`�ɕҏW���ꂽ�f�[�^���o�́B

  Returns:      BOOL - �`�F�b�N���Ė�肪����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL RtcCheckTime(const RTCTime *time, RTCRawTime *raw)
{
    // �e�����o�����e�͈͓����`�F�b�N
    if (time->hour >= 24)
        return FALSE;
    if (time->minute >= 60)
        return FALSE;
    if (time->second >= 60)
        return FALSE;

    // ���f�[�^�̌^�ɕҏW
    if (time->hour >= 12)
    {
        raw->afternoon = 1;
    }
    else
    {
        raw->afternoon = 0;
    }
    raw->hour = RtcHEX2BCD(time->hour);
    raw->minute = RtcHEX2BCD(time->minute);
    raw->second = RtcHEX2BCD(time->second);

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         RtcGetResultCallback

  Description:  �񓯊������̊������ɌĂяo����A�����ϐ��̏������ʂ��X�V����B

  Arguments:    result - �񓯊��֐��̏������ʁB
                arg    - �g�p���Ȃ��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void RtcGetResultCallback(RTCResult result, void *arg)
{
#pragma unused( arg )

    rtcWork.commonResult = result;
}

/*---------------------------------------------------------------------------*
  Name:         RtcConvertTickToDateTimeEx

  Description:  Converts CPU ticks to RTCDate and RTCTimeEx. The number of ticks
                are treated as total ticks progressed since 2000/01/01/00:00:00.

  Arguments:    date      - buffer for storing RTCDate
                time      - buffer for storing RTCTimeEx

  Returns:      None
 *---------------------------------------------------------------------------*/
static void RtcConvertTickToDateTimeEx(RTCDate * date, RTCTimeEx *time, OSTick tick)
{
    // store date and time converted from the seconds converted from ticks
    RTC_ConvertSecondToDateTime(date, (RTCTime*)time, (s64) OS_TicksToSeconds(tick));
    // store milliseconds independently
    time->millisecond = (s32) (OS_TicksToMilliSeconds(tick) % 1000);
}

/*---------------------------------------------------------------------------*
  Name:         RtcTickInit

  Description:  Saves initial total CPU ticks progressed since
                2000/01/01/00:00:00. The total ticks are calculated using
                current RTCDate, RTCTime and OS_GetTick().

  Arguments:    None

  Returns:      RTCResult - result of initial RTC_GetDateTime
 *---------------------------------------------------------------------------*/
static RTCResult RtcTickInit(void)
{   
    RTCDate currentDate;
    RTCTime currentTime;
    s64     totalSeconds;
    RTCResult result;
    
    SDK_ASSERT(OS_IsTickAvailable());

    // save current seconds converted from current date and time
    result = RTC_GetDateTime(&currentDate, &currentTime);
    totalSeconds = RTC_ConvertDateTimeToSecond(&currentDate, &currentTime);

    rtcInitialTotalTicks = OS_SecondsToTicks(totalSeconds) - OS_GetTick();
    rtcTickInitialized = TRUE;

    return result;
}

#include    <nitro/code32.h>
/*---------------------------------------------------------------------------*
  Name:         RtcWaitBusy

  Description:  RTC�̔񓯊����������b�N����Ă���ԑ҂B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static asm void
RtcWaitBusy( void )
{
    ldr     r12,    =rtcWork.lock
loop:
    ldr     r0,     [ r12,  #0 ]
    cmp     r0,     #RTC_LOCK_ON
    beq     loop
    bx      lr
}
#include    <nitro/codereset.h>


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

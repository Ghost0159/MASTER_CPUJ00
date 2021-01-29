/*---------------------------------------------------------------------------*
  Project:  NitroSDK - RTC - demos
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 11:02:12  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.8  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.7  2005/02/28 05:26:11  yosizaki
  do-indent.

  Revision 1.6  2004/10/06 00:27:21  terui
  Fix comment.

  Revision 1.5  2004/09/16 05:45:55  terui
  FINALROM�ł���IS-NITRO-EMULATOR�ŃA���[��������ɓ��삵�Ȃ����ӏ�����ǉ��B

  Revision 1.4  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.3  2004/08/19 11:15:50  terui
  ���ԕ\�L��24���ԕ\�L�Œ艻�ɔ��������B

  Revision 1.2  2004/05/24 02:23:33  terui
  Bug fix. (Called GX_LoadBGxxxx in IRQ before initialize.)

  Revision 1.1  2004/05/12 02:38:49  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    NOTICE: ���T���v���ɂ�RTC�̃A���[���@�\�̓�����m�F����@�\���܂܂�Ă���
            �܂����A2004/10/06���݁AIS-NITRO-DEBUGGER�\�t�g�E�F�A�̃o�[�W������
            Ver 1.11 ���O�̃o�[�W�����ł���Ɛ���ɓ��삵�Ȃ��ꍇ������܂��B
            ��̓I�ɂ͔�������ׂ��A���[�����������Ȃ��Ƃ������ۂ��N����܂��B
            ���̌��ۂ�NintendoDS�{�̂�IS-NITRO-EMULATOR�̃n�[�h�E�F�A��ł�
            �����ɋN�����Ă���܂��̂ŁA�ʎY����i�Ȃǂ̎��@�ł͖��Ȃ�����
            ���܂��B
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/rtc.h>

#include    "font.h"


/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     KEY_REPEAT_START    25     // �L�[���s�[�g�J�n�܂ł̃t���[����
#define     KEY_REPEAT_SPAN     10     // �L�[���s�[�g�̊Ԋu�t���[����


/*---------------------------------------------------------------------------*
    ������萔��`
 *---------------------------------------------------------------------------*/
// �j��
const char *StrWeek[7] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

// �A���[�����
const char *StrAlarmStatus[2] = {
    "OFF",
    "ON"
};

// �A���[���L���t���O
const char *StrAlarmEnable[8] = {
    "- - -",                           // �L���t���O�Ȃ�
    "W - -",                           // �T       �ݒ肪�L��
    "- H -",                           //    ��    �ݒ肪�L��
    "W H -",                           // �T ��    �ݒ肪�L��
    "- - M",                           //       �� �ݒ肪�L��
    "W - M",                           // �T    �� �ݒ肪�L��
    "- H M",                           //    �� �� �ݒ肪�L��
    "W H M"                            // �T �� �� �ݒ肪�L��
};


/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
// �L�[���͏��
typedef struct KeyInformation
{
    u16     cnt;                       // �����H���͒l
    u16     trg;                       // �����g���K����
    u16     up;                        // �����g���K����
    u16     rep;                       // �����ێ����s�[�g����

}
KeyInformation;

// �ҏW���[�h
typedef enum EditMode
{
    EDIT_MODE_NONE = 0,                // �ҏW�Ȃ����[�h
    EDIT_MODE_DATE_TIME,               // ���t�������ҏW���[�h
    EDIT_MODE_ALARM_1,                 // �A���[���P��ԁ��ݒ�l�ҏW���[�h
    EDIT_MODE_ALARM_2,                 // �A���[���Q��ԁ��ݒ�l�ҏW���[�h
    EDIT_MODE_MAX
}
EditMode;


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void);
static void AlarmIntrCallback(void);

static void IncValue(void);
static void DecValue(void);
static void IncSelect(void);
static void DecSelect(void);

static void KeyRead(KeyInformation * pKey);
static void ClearString(void);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
static void ColorString(s16 x, s16 y, s16 length, u8 palette);


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static u16 gScreen[32 * 32];           // ���z�X�N���[��
static KeyInformation gKey;            // �L�[����

static RTCDate gRtcDate;               // ���t
static RTCTime gRtcTime;               // ����
static RTCAlarmStatus gRtcAlarmStatus1; // �A���[���P���
static RTCAlarmParam gRtcAlarmParam1;  // �A���[���P�ݒ�l
static RTCAlarmStatus gRtcAlarmStatus2; // �A���[���Q���
static RTCAlarmParam gRtcAlarmParam2;  // �A���[���Q�ݒ�l

static EditMode gEditMode;             // �ҏW���[�h
static u32 gSelect;                    // �I���ӏ��C���f�b�N�X
static s32 gMosaic;                    // ���U�C�N�\���J�E���^


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �������y�у��C�����[�v�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // �e�평����
    OS_Init();
    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    // �\���ݒ菉����
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    // ������\���p�ɂQ�c�\���ݒ�
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,      // SCR �x�[�X�u���b�N 31
                     GX_BG_CHARBASE_0x00000,    // CHR �x�[�X�u���b�N 0
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // �����ݐݒ�
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();


    //****************************************************************
    // RTC������
    RTC_Init();
    // �A���[���������̔�ѐ�x�N�g����ݒ�
    RTC_SetAlarmInterrupt(AlarmIntrCallback);
    //****************************************************************

    // �����ϐ�������
    gEditMode = EDIT_MODE_NONE;
    gSelect = 0;
    gMosaic = 0;

    // LCD�\���J�n
    GX_DispOn();
    GXS_DispOn();

    // �f�o�b�O������o��
    OS_Printf("ARM9: RTC demo started.\n");

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    KeyRead(&gKey);

    // ���C�����[�v
    while (TRUE)
    {
        // �L�[���͏��擾
        KeyRead(&gKey);

        // �X�N���[���N���A
        ClearString();

        //****************************************************************
        // ���݂̏�Ԃ�RTC����ǂݏo��
        if (gEditMode != EDIT_MODE_DATE_TIME)
        {
            // ���t�E������ǂݏo��
            (void)RTC_GetDateTime(&gRtcDate, &gRtcTime);
        }
        if (gEditMode != EDIT_MODE_ALARM_1)
        {
            // �A���[���P��Ԃ�ǂݏo��
            (void)RTC_GetAlarmStatus(RTC_ALARM_CHAN_1, &gRtcAlarmStatus1);
            if (gRtcAlarmStatus1 == RTC_ALARM_STATUS_ON)
            {
                // �A���[���P�� ON �Ȃ�ΐݒ�l��ǂݏo��
                (void)RTC_GetAlarmParam(RTC_ALARM_CHAN_1, &gRtcAlarmParam1);
            }
        }
        if (gEditMode != EDIT_MODE_ALARM_2)
        {
            // �A���[���Q��Ԃ�ǂݏo��
            (void)RTC_GetAlarmStatus(RTC_ALARM_CHAN_2, &gRtcAlarmStatus2);
            if (gRtcAlarmStatus2 == RTC_ALARM_STATUS_ON)
            {
                // �A���[���Q�� ON �Ȃ�ΐݒ�l��ǂݏo��
                (void)RTC_GetAlarmParam(RTC_ALARM_CHAN_2, &gRtcAlarmParam2);
            }
        }
        //****************************************************************

        // ���t��������\��
        PrintString(7, 3, 0xf, "%04d/%02d/%02d/%s",
                    gRtcDate.year + 2000, gRtcDate.month, gRtcDate.day, StrWeek[gRtcDate.week]);
        PrintString(9, 5, 0xf, " %02d:%02d:%02d", gRtcTime.hour, gRtcTime.minute, gRtcTime.second);
        // ���t�������̕ҏW���[�h�̏ꍇ�A�I���ӏ��̐F��ύX
        if (gEditMode == EDIT_MODE_DATE_TIME)
        {
            switch (gSelect)
            {
            case 0:
                ColorString(7, 3, 4, 0x1);
                break;
            case 1:
                ColorString(12, 3, 2, 0x1);
                break;
            case 2:
                ColorString(15, 3, 2, 0x1);
                break;
            case 3:
                ColorString(18, 3, 9, 0x1);
                break;
            case 4:
                ColorString(10, 5, 2, 0x1);
                break;
            case 5:
                ColorString(13, 5, 2, 0x1);
                break;
            case 6:
                ColorString(16, 5, 2, 0x1);
                break;
            }
        }

        // �A���[���P��ԁ��ݒ�l��\��
        PrintString(3, 8, 0x4, "ALARM-1 %s", StrAlarmStatus[gRtcAlarmStatus1]);
        PrintString(4, 10, 0xf, "% 9s  %02d:%02d  %s",
                    StrWeek[gRtcAlarmParam1.week],
                    gRtcAlarmParam1.hour,
                    gRtcAlarmParam1.minute, StrAlarmEnable[gRtcAlarmParam1.enable]);
        // �A���[���P�̕ҏW���[�h�̏ꍇ�A�I���ӏ��̐F��ύX
        if (gEditMode == EDIT_MODE_ALARM_1)
        {
            switch (gSelect)
            {
            case 0:
                ColorString(11, 8, 6, 0x1);
                break;
            case 1:
                ColorString(4, 10, 9, 0x1);
                break;
            case 2:
                ColorString(15, 10, 2, 0x1);
                break;
            case 3:
                ColorString(18, 10, 2, 0x1);
                break;
            case 4:
                ColorString(22, 10, 5, 0x1);
                break;
            }
        }

        // �A���[���Q��ԁ��ݒ�l��\��
        PrintString(3, 12, 0x4, "ALARM-2 %s", StrAlarmStatus[gRtcAlarmStatus2]);
        PrintString(4, 14, 0xf, "% 9s  %02d:%02d  %s",
                    StrWeek[gRtcAlarmParam2.week],
                    gRtcAlarmParam2.hour,
                    gRtcAlarmParam2.minute, StrAlarmEnable[gRtcAlarmParam2.enable]);
        // �A���[���Q�ҏW���[�h�̏ꍇ�A�I���ӏ��̐F��ύX
        if (gEditMode == EDIT_MODE_ALARM_2)
        {
            switch (gSelect)
            {
            case 0:
                ColorString(11, 12, 6, 0x1);
                break;
            case 1:
                ColorString(4, 14, 9, 0x1);
                break;
            case 2:
                ColorString(15, 14, 2, 0x1);
                break;
            case 3:
                ColorString(18, 14, 2, 0x1);
                break;
            case 4:
                ColorString(22, 14, 5, 0x1);
                break;
            }
        }

        // �{�^�������������\��
        if (gEditMode == EDIT_MODE_NONE)
        {
            PrintString(1, 19, 0x3, "DOWN  > Change Date & Time.");
            PrintString(1, 20, 0x3, "LEFT  > Change Alarm1 Setting.");
            PrintString(1, 21, 0x3, "RIGHT > Change Alarm2 Setting.");
        }
        else
        {
            PrintString(1, 18, 0x3, "UP    > Increment value.");
            PrintString(1, 19, 0x3, "DOWN  > Decrement value.");
            PrintString(1, 20, 0x3, "LEFT  > Move selector left.");
            PrintString(1, 21, 0x3, "RIGHT > Move selector right.");
            PrintString(1, 22, 0x3, "A     > Commit editing.");
            PrintString(1, 23, 0x3, "B     > Cancel editing.");
        }

        // �{�^�����͑���
        if (gEditMode == EDIT_MODE_NONE)
        {
            // �� �L�[
            if ((gKey.trg | gKey.rep) & PAD_KEY_DOWN)
            {
                // ���t�������ҏW���[�h�ֈڍs
                gSelect = 0;
                gEditMode = EDIT_MODE_DATE_TIME;
            }
            // �� �L�[
            if ((gKey.trg | gKey.rep) & PAD_KEY_LEFT)
            {
                // �A���[���P��ԁ��ݒ�l�ҏW���[�h�ֈڍs
                gSelect = 0;
                gEditMode = EDIT_MODE_ALARM_1;
            }
            // �� �L�[
            if ((gKey.trg | gKey.rep) & PAD_KEY_RIGHT)
            {
                // �A���[���Q��ԁ��ݒ�l�ҏW���[�h�ֈڍs
                gSelect = 0;
                gEditMode = EDIT_MODE_ALARM_2;
            }
        }
        else                           // �ҏW���[�h���ʃL�[����
        {
            // �� �L�[
            if ((gKey.trg | gKey.rep) & PAD_KEY_UP)
            {
                IncValue();
            }
            // �� �L�[
            if ((gKey.trg | gKey.rep) & PAD_KEY_DOWN)
            {
                DecValue();
            }
            // �� �L�[
            if ((gKey.trg | gKey.rep) & PAD_KEY_LEFT)
            {
                DecSelect();
            }
            // �� �L�[
            if ((gKey.trg | gKey.rep) & PAD_KEY_RIGHT)
            {
                IncSelect();
            }
            // �` �{�^��
            if ((gKey.trg | gKey.rep) & PAD_BUTTON_A)
            {
                //****************************************************************
                // RTC�ɕύX�����ϐ�����������
                switch (gEditMode)
                {
                case EDIT_MODE_DATE_TIME:
                    // ���t�E��������������
                    (void)RTC_SetDateTime(&gRtcDate, &gRtcTime);
                    break;
                case EDIT_MODE_ALARM_1:
                    // �A���[���P�̏�Ԃ���������
                    (void)RTC_SetAlarmStatus(RTC_ALARM_CHAN_1, &gRtcAlarmStatus1);
                    if (gRtcAlarmStatus1 == RTC_ALARM_STATUS_ON)
                    {
                        // �A���[���P�� ON �Ȃ�ΐݒ�l����������
                        (void)RTC_SetAlarmParam(RTC_ALARM_CHAN_1, &gRtcAlarmParam1);
                    }
                    break;
                case EDIT_MODE_ALARM_2:
                    // �A���[���Q�̏�Ԃ���������
                    (void)RTC_SetAlarmStatus(RTC_ALARM_CHAN_2, &gRtcAlarmStatus2);
                    if (gRtcAlarmStatus2 == RTC_ALARM_STATUS_ON)
                    {
                        // �A���[���Q�� ON �Ȃ�ΐݒ�l����������
                        (void)RTC_SetAlarmParam(RTC_ALARM_CHAN_2, &gRtcAlarmParam2);
                    }
                    break;
                }
                //****************************************************************

                // �ҏW�Ȃ����[�h�ֈڍs
                gEditMode = EDIT_MODE_NONE;
            }
            // �a �{�^��
            if ((gKey.trg | gKey.rep) & PAD_BUTTON_B)
            {
                // �ҏW�Ȃ����[�h�ֈڍs
                gEditMode = EDIT_MODE_NONE;
            }
        }

        // �u�u�����N�҂�
        OS_WaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �u�u�����N�����݃x�N�g���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // ���z�X�N���[����VRAM�ɔ��f
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // ���U�C�N�\���J�E���^�ɏ]�����U�C�N�\��
    if (gMosaic)
    {
        int     w;
        int     i = 5;

        w = gMosaic / 4 % (i * 2);
        w = w - (w - ((w - i) * 2)) * (w / (i + 1));
        gMosaic--;

        G2_BG0Mosaic(TRUE);
        G2_SetBGMosaicSize(w, w);
    }
    else
    {
        G2_BG0Mosaic(FALSE);
    }

    // IRQ �`�F�b�N�t���O���Z�b�g
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         AlarmIntrCallback

  Description:  RTC�A���[���������̊����݃x�N�g���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void AlarmIntrCallback(void)
{
    // �f�o�b�O������o��
    OS_Printf("ARM9: Alarm interrupt on %04d/%02d/%02d (%s) %02d:%02d:%02d .\n",
              gRtcDate.year + 2000,
              gRtcDate.month,
              gRtcDate.day,
              StrWeek[gRtcDate.week], gRtcTime.hour, gRtcTime.minute, gRtcTime.second);
    // ���U�C�N�A�j���[�V�����J�n
    gMosaic = 300;
}

/*---------------------------------------------------------------------------*
  Name:         IncValue

  Description:  �ҏW���[�h�ɂ��A�I������Ă���l���{�P����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void IncValue(void)
{
    switch (gEditMode)
    {

        // ���t�������ҏW���[�h
    case EDIT_MODE_DATE_TIME:
        {
            RTCDate *d = &gRtcDate;
            RTCTime *t = &gRtcTime;

            switch (gSelect)
            {
            case 0:
                d->year = (d->year + 1) % 100;
                break;
            case 1:
                d->month = (d->month % 12) + 1;
                break;
            case 2:
                d->day = (d->day % 31) + 1;
                break;
            case 3:
                d->week = (RTCWeek)((d->week + 1) % RTC_WEEK_MAX);
                break;
            case 4:
                t->hour = (t->hour + 1) % 24;
                break;
            case 5:
                t->minute = (t->minute + 1) % 60;
                break;
            case 6:
                t->second = (t->second + 1) % 60;
                break;
            }
        }
        break;

        // �A���[���Por�Q�ҏW���[�h
    case EDIT_MODE_ALARM_1:
    case EDIT_MODE_ALARM_2:
        {
            RTCAlarmStatus *s;
            RTCAlarmParam *p;

            s = (gEditMode == EDIT_MODE_ALARM_1 ? &gRtcAlarmStatus1 : &gRtcAlarmStatus2);
            p = (gEditMode == EDIT_MODE_ALARM_1 ? &gRtcAlarmParam1 : &gRtcAlarmParam2);

            switch (gSelect)
            {
            case 0:
                *s = (RTCAlarmStatus)((*s + 1) % RTC_ALARM_STATUS_MAX);
                break;
            case 1:
                p->week = (RTCWeek)((p->week + 1) % RTC_WEEK_MAX);
                break;
            case 2:
                p->hour = (p->hour + 1) % 24;
                break;
            case 3:
                p->minute = (p->minute + 1) % 60;
                break;
            case 4:
                p->enable = (p->enable + 1) % (RTC_ALARM_ENABLE_ALL + 1);
                break;
            }
        }
        break;

    }
}

/*---------------------------------------------------------------------------*
  Name:         DecValue

  Description:  �ҏW���[�h�ɂ��A�I������Ă���l���|�P����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DecValue(void)
{
    switch (gEditMode)
    {

        // ���t�������ҏW���[�h
    case EDIT_MODE_DATE_TIME:
        {
            RTCDate *d = &gRtcDate;
            RTCTime *t = &gRtcTime;

            switch (gSelect)
            {
            case 0:
                d->year = (d->year + 99) % 100;
                break;
            case 1:
                d->month = ((d->month + 10) % 12) + 1;
                break;
            case 2:
                d->day = ((d->day + 29) % 31) + 1;
                break;
            case 3:
                d->week = (RTCWeek)((d->week + (RTC_WEEK_MAX - 1)) % RTC_WEEK_MAX);
                break;
            case 4:
                t->hour = (t->hour + 23) % 24;
                break;
            case 5:
                t->minute = (t->minute + 59) % 60;
                break;
            case 6:
                t->second = (t->second + 59) % 60;
                break;
            }
        }
        break;

        // �A���[���Por�Q�ҏW���[�h
    case EDIT_MODE_ALARM_1:
    case EDIT_MODE_ALARM_2:
        {
            RTCAlarmStatus *s;
            RTCAlarmParam *p;

            s = (gEditMode == EDIT_MODE_ALARM_1 ? &gRtcAlarmStatus1 : &gRtcAlarmStatus2);
            p = (gEditMode == EDIT_MODE_ALARM_1 ? &gRtcAlarmParam1 : &gRtcAlarmParam2);

            switch (gSelect)
            {
            case 0:
                *s = (RTCAlarmStatus)((*s + (RTC_ALARM_STATUS_MAX - 1)) % RTC_ALARM_STATUS_MAX);
                break;
            case 1:
                p->week = (RTCWeek)((p->week + (RTC_WEEK_MAX - 1)) % RTC_WEEK_MAX);
                break;
            case 2:
                p->hour = (p->hour + 23) % 24;
                break;
            case 3:
                p->minute = (p->minute + 59) % 60;
                break;
            case 4:
                p->enable = (p->enable + RTC_ALARM_ENABLE_ALL) % (RTC_ALARM_ENABLE_ALL + 1);
                break;
            }
        }
        break;

    }
}

/*---------------------------------------------------------------------------*
  Name:         IncSelect

  Description:  �ҏW���[�h�ɂ��A�I���C���f�b�N�X���{�P����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void IncSelect(void)
{
    switch (gEditMode)
    {

        // ���t�������ҏW���[�h
    case EDIT_MODE_DATE_TIME:
        gSelect = (gSelect + 1) % 7;
        break;

        // �A���[���Por�Q�ҏW���[�h
    case EDIT_MODE_ALARM_1:
    case EDIT_MODE_ALARM_2:
        gSelect = (gSelect + 1) % 5;
        break;

    }
}

/*---------------------------------------------------------------------------*
  Name:         DecSelect

  Description:  �ҏW���[�h�ɂ��A�I���C���f�b�N�X���|�P����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DecSelect(void)
{
    switch (gEditMode)
    {

        // ���t�������ҏW���[�h
    case EDIT_MODE_DATE_TIME:
        gSelect = (gSelect + 6) % 7;
        break;

        // �A���[���Por�Q�ҏW���[�h
    case EDIT_MODE_ALARM_1:
    case EDIT_MODE_ALARM_2:
        gSelect = (gSelect + 4) % 5;
        break;

    }
}

/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  �L�[���͏���ҏW����B
                �����g���K�A�����g���K�A�����p�����s�[�g�����o�B

  Arguments:    pKey  - �ҏW����L�[���͏��\���́B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void KeyRead(KeyInformation * pKey)
{
    static u16 repeat_count[12];
    int     i;
    u16     r;

    r = PAD_Read();
    pKey->trg = 0x0000;
    pKey->up = 0x0000;
    pKey->rep = 0x0000;

    for (i = 0; i < 12; i++)
    {
        if (r & (0x0001 << i))
        {
            if (!(pKey->cnt & (0x0001 << i)))
            {
                pKey->trg |= (0x0001 << i);     // �����g���K
                repeat_count[i] = 1;
            }
            else
            {
                if (repeat_count[i] > KEY_REPEAT_START)
                {
                    pKey->rep |= (0x0001 << i); // �����p�����s�[�g
                    repeat_count[i] = KEY_REPEAT_START - KEY_REPEAT_SPAN;
                }
                else
                {
                    repeat_count[i]++;
                }
            }
        }
        else
        {
            if (pKey->cnt & (0x0001 << i))
            {
                pKey->up |= (0x0001 << i);      // �����g���K
            }
        }
    }
    pKey->cnt = r;                     // �����H�L�[����
}

/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  ���z�X�N���[�����N���A����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ClearString(void)
{
    MI_CpuClearFast((void *)gScreen, sizeof(gScreen));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  ���z�X�N���[���ɕ������z�u����B�������32�����܂ŁB

  Arguments:    x       - ������̐擪��z�u���� x ���W( �~ 8 �h�b�g )�B
                y       - ������̐擪��z�u���� y ���W( �~ 8 �h�b�g )�B
                palette - �����̐F���p���b�g�ԍ��Ŏw��B
                text    - �z�u���镶����B�I�[������NULL�B
                ...     - ���z�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...)
{
    va_list vlist;
    char    temp[32 + 2];
    s32     i;

    va_start(vlist, text);
    (void)vsnprintf(temp, 33, text, vlist);
    va_end(vlist);

    *(u16 *)(&temp[32]) = 0x0000;
    for (i = 0;; i++)
    {
        if (temp[i] == 0x00)
        {
            break;
        }
        gScreen[((y * 32) + x + i) % (32 * 32)] = (u16)((palette << 12) | temp[i]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  ���z�X�N���[���ɔz�u����������̐F��ύX����B

  Arguments:    x       - �F�ύX���J�n���� x ���W( �~ 8 �h�b�g )�B
                y       - �F�ύX���J�n���� y ���W( �~ 8 �h�b�g )�B
                length  - �A�����ĐF�ύX���镶�����B
                palette - �����̐F���p���b�g�ԍ��Ŏw��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ColorString(s16 x, s16 y, s16 length, u8 palette)
{
    s32     i;
    u16     temp;
    s32     index;

    if (length < 0)
        return;

    for (i = 0; i < length; i++)
    {
        index = ((y * 32) + x + i) % (32 * 32);
        temp = gScreen[index];
        temp &= 0x0fff;
        temp |= (palette << 12);
        gScreen[index] = temp;
    }
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

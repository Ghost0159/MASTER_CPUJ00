/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - demos - pm-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.23  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.22  2005/11/21 11:04:18  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.21  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.20  2005/03/03 13:05:43  takano_makoto
  exclude any code which is available in specific compiler.

  Revision 1.19  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.18  2004/10/06 09:01:15  yada
  change around sleep callback

  Revision 1.17  2004/10/05 06:58:46  yada
  add sample of pre/post callback for sleep mode

  Revision 1.16  2004/09/28 05:57:28  yada
  remove unused variables and definitions.
  battery check is done automaticaly

  Revision 1.15  2004/09/01 06:14:52  yada
  remove LED setting demo

  Revision 1.14  2004/08/26 11:43:49  yada
  add changing LED pattern demo

  Revision 1.13  2004/08/23 08:18:51  yada
  add demo of LED self blink

  Revision 1.12  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.11  2004/08/20 00:21:19  yada
  remove RTCTime member 'noon'

  Revision 1.10  2004/08/19 12:12:23  yada
  add RTC alarm interrupt sample

  Revision 1.9  2004/08/11 04:05:47  yasu
  Make a code around LCD power management simple

  Revision 1.8  2004/08/10 10:59:14  yada
  LCD power interval become to be controled with VBlank counter

  Revision 1.7  2004/08/09 02:00:59  yada
  fix around PM_SetLCDPower

  Revision 1.6  2004/08/07 07:26:10  yada
  PM_CheckBattery -> PM_GetBattery,
  fix bug about checking battery

  Revision 1.5  2004/08/07 07:19:54  yada
  add L&R push to power off

  Revision 1.4  2004/08/07 01:59:51  yada
  modified much

  Revision 1.3  2004/08/06 12:44:56  yada
  change argument of PM_GoSleepMode()

  Revision 1.2  2004/08/04 00:48:23  yada
  PM_SetBackLightTop/Bottom -> PM_SetBackLight

  Revision 1.1  2004/08/03 01:54:16  yada
  first release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include  <nitro.h>
#include  "font.h"
#include  "screen.h"

//---- for battery display
char   *myBatteryStr[] = { "HI", "LO" };
#define MY_BATTERY_HI      0
#define MY_BATTERY_LO      1
#define MY_BATTERY_UNKNOWN 2

//---- for LCD switch ( next status )
#define MY_LCD_KEEP        0           // not change
#define MY_LCD_OFF         1
#define MY_LCD_ON          2

static int myLCDNext = MY_LCD_KEEP;

//---- for power save
#define MY_POWER_SAVE_PERIOD  3000     // milliseconds

//---- for battery check
#define MY_BATTERY_CHECK_INTERVAL 10   // frame

static int myBatteryStatus = MY_BATTERY_UNKNOWN;
static u32 myBatteryCount;

//---- for RTC
static RTCTime myCurrentTime;
static int myResult;

//---- alarm
static OSAlarm myAlarm;

//---- message
#define  MY_MESSAGE_SIZE   4

static OSMessage myMessageBuffer[MY_MESSAGE_SIZE];
static OSMessageQueue myMessageQueue;

//---- vcount
static u32 myVCount;

//---- sleep reason
#define MY_SLEEP_BY_CLOSE_COVER    0
#define MY_SLEEP_BY_PUSH_START     1
#define MY_SLEEP_BY_PUSH_LEFT      2
static u32 mySleepReason;

static void myInit(void);
static void myVBlankIntr(void);
static void myAlarmHandler(void *arg);

void    myPreCallback(void *arg);
void    myPostCallback(void *arg);
PMSleepCallbackInfo myPreCallbackInfo;
PMSleepCallbackInfo myPostCallbackInfo;

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    u16     preButton;
    u16     button;
    u16     trigger;

    //---------------- initialize
    myInit();
    RTC_Init();

    OS_InitMessageQueue(&myMessageQueue, &myMessageBuffer[0], MY_MESSAGE_SIZE);
    OS_CreateAlarm(&myAlarm);

    preButton = PAD_Read();            // dummy read for pressing 'A' on IPL

    //---- set SleepMode callbacks
    PM_SetSleepCallbackInfo(&myPreCallbackInfo, myPreCallback, (void *)&mySleepReason);
    PM_SetSleepCallbackInfo(&myPostCallbackInfo, myPostCallback, NULL);
    PM_AppendPreSleepCallback(&myPreCallbackInfo);
    PM_AppendPostSleepCallback(&myPostCallbackInfo);

    //---------------- main loop
    while (TRUE)
    {
        button = PAD_Read();
        trigger = (u16)((button ^ preButton) & button);
        preButton = button;

        //---- vblank count
        myVCount = OS_GetVBlankCount();

        //---- clear screen buffer
        ClearScreen();

        //---- display key description
        PrintString(3, 2, 15, "START : sleep till push");
        PrintString(3, 3, 15, "        SELECT");
        PrintString(3, 4, 15, "A     : top    back light");
        PrintString(3, 5, 15, "B     : bottom back light");
        PrintString(3, 7, 15, "UP    : power save till push");
        PrintString(3, 8, 15, "        DOWN or pass 3 secs");
        PrintString(3, 9, 15, "LEFT  : sleep till next min");
        PrintString(3, 10, 15, "        by RTC or push RIGHT");
        PrintString(3, 12, 15, "CLOSE : sleep till open");
        PrintString(3, 13, 15, "L & R : power off");

        //---- display time
        myResult = RTC_GetTime(&myCurrentTime);
        if (myResult == 0 /*no error */ )
        {
            PrintString(5, 20, 8, "%02d:%02d:%02d",
                        myCurrentTime.hour, myCurrentTime.minute, myCurrentTime.second);
        }

        //---- display battery status
        if (myBatteryCount < myVCount || myBatteryStatus == MY_BATTERY_UNKNOWN)
        {
            PMBattery battery;
            (void)PM_GetBattery(&battery);
            myBatteryStatus = (battery == PM_BATTERY_HIGH) ? MY_BATTERY_HI : MY_BATTERY_LO;

            myBatteryCount = myVCount + MY_BATTERY_CHECK_INTERVAL;
        }
        PrintString(3, 17, 15, "Battery status : %s", myBatteryStr[myBatteryStatus]);

        //---- display counter
        PrintString(18, 20, 4, "%08X", myVCount);

        //---- display LED pattern
        {
            PMLEDPattern pattern;
            (void)PM_GetLEDPattern(&pattern);
            PrintString(3, 18, 15, "LED Pattern    : %d", pattern);
        }

        //---- push START to sleep
        if (trigger & PAD_BUTTON_START)
        {
            mySleepReason = MY_SLEEP_BY_PUSH_START;
            PM_GoSleepMode(PM_TRIGGER_KEY, PM_PAD_LOGIC_AND, PAD_BUTTON_SELECT);
        }

        //---- push A to switch top backlight on/off
        if (trigger & PAD_BUTTON_A)
        {
            static int cnt = 0;
            switch (cnt % 2)
            {
            case 0:
                (void)PM_SetBackLight(PM_LCD_TOP, PM_BACKLIGHT_OFF);
                break;
            case 1:
                (void)PM_SetBackLight(PM_LCD_TOP, PM_BACKLIGHT_ON);
                break;
            }
            cnt++;
        }

        //---- push B to switch top backlight on/off
        if (trigger & PAD_BUTTON_B)
        {
            static int cnt = 0;
            switch (cnt % 2)
            {
            case 0:
                (void)PM_SetBackLight(PM_LCD_BOTTOM, PM_BACKLIGHT_OFF);
                break;
            case 1:
                (void)PM_SetBackLight(PM_LCD_BOTTOM, PM_BACKLIGHT_ON);
                break;
            }
            cnt++;
        }

        //---- push UP to go power save mode
        if (trigger & PAD_KEY_UP && PM_GetLCDPower() == PM_LCD_POWER_ON && myLCDNext == MY_LCD_KEEP)
        {
            //---- set 3 seconds alarm
            OS_CancelAlarm(&myAlarm);
            OS_SetAlarm(&myAlarm, OS_MilliSecondsToTicks(MY_POWER_SAVE_PERIOD), myAlarmHandler,
                        NULL);

            //---- turn LCD off
            myLCDNext = MY_LCD_OFF;
        }

        //---- push LEFT to sleep till next minute
        if (trigger & PAD_KEY_LEFT)
        {
            RTCAlarmStatus st = RTC_ALARM_STATUS_ON;
            RTCAlarmParam p;

            p.week = RTC_WEEK_SUNDAY;  // dymmy. no sense
            p.hour = 1;                // dummy. no sense
            p.minute = (myCurrentTime.minute == 59) ? 0 : (myCurrentTime.minute + 1);
            p.enable = RTC_ALARM_ENABLE_MINUTE;

            myResult = RTC_SetAlarmStatus(RTC_ALARM_CHAN_1, &st);
            myResult = RTC_SetAlarmParam(RTC_ALARM_CHAN_1, &p);

            mySleepReason = MY_SLEEP_BY_PUSH_LEFT;
            PM_GoSleepMode(PM_TRIGGER_KEY | PM_TRIGGER_RTC_ALARM, PM_PAD_LOGIC_AND, PAD_KEY_RIGHT);
        }

        //---- push DOWN to return from power save mode
        if (trigger & PAD_KEY_DOWN && PM_GetLCDPower() == PM_LCD_POWER_OFF
            && myLCDNext == MY_LCD_KEEP)
        {
            //---- turn LCD on
            myLCDNext = MY_LCD_ON;
            OS_CancelAlarm(&myAlarm);
        }

        //---- close cover to sleep
        if (PAD_DetectFold() == TRUE)
        {
            myLCDNext = MY_LCD_KEEP;
            mySleepReason = MY_SLEEP_BY_CLOSE_COVER;
            PM_GoSleepMode(PM_TRIGGER_COVER_OPEN | PM_TRIGGER_CARD, 0, 0);
        }

        //---- push L & R to power off
        if ((button & (PAD_BUTTON_L | PAD_BUTTON_R)) == (PAD_BUTTON_L | PAD_BUTTON_R))
        {
            (void)PM_ForceToPowerOff();
        }

        //---- switch LCD power
        if (myLCDNext != MY_LCD_KEEP)
        {
            if (myLCDNext == MY_LCD_OFF)
            {
                OS_WaitVBlankIntr();
                GX_DispOff();
                GXS_DispOff();
                OS_WaitVBlankIntr();
                if (PM_SetLCDPower(PM_LCD_POWER_OFF))
                {
                    myLCDNext = MY_LCD_KEEP;
                }
            }
            else if (myLCDNext == MY_LCD_ON)
            {
                if (PM_SetLCDPower(PM_LCD_POWER_ON))
                {
                    OS_WaitVBlankIntr();
                    GX_DispOn();
                    GXS_DispOn();
                    OS_WaitVBlankIntr();
                    myLCDNext = MY_LCD_KEEP;
                }
            }
        }

        OS_WaitVBlankIntr();
    }
}

//----------------------------------------------------------------
//  myInit
//
void myInit(void)
{
    //---- init
    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    //---- init displaying
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();

    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    //---- setting 2D for top screen
    GX_SetBankForBG(GX_VRAM_BG_128_A);

    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                     GX_BG_COLORMODE_16,
                     GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);

    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));



    //---- setting 2D for bottom screen
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);

    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                      GX_BG_COLORMODE_16,
                      GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    G2S_BG0Mosaic(FALSE);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);

    GXS_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GXS_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));


    //---- screen
    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));
    GXS_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    //---- init interrupt
    OS_SetIrqFunction(OS_IE_V_BLANK, myVBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    //---- start displaying
    GX_DispOn();
    GXS_DispOn();
}

//----------------------------------------------------------------
//  myVBlankIntr
//             vblank interrupt handler
//
static void myVBlankIntr(void)
{
    //---- upload pseudo screen to VRAM
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));
    GXS_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//----------------------------------------------------------------
//  myAlarmHandler
//             alarm interrupt handler
//
void myAlarmHandler(void *arg)
{
#pragma unused(arg)
    if (PM_GetLCDPower() == PM_LCD_POWER_OFF && myLCDNext == MY_LCD_KEEP)
    {
        myLCDNext = MY_LCD_ON;
    }
}

//================================================================
//----------------------------------------------------------------
//  myPreCallback
//
static const char *myCallbackReasonStr[] = {
    "closing cover",
    "pushing START",
    "pushing LEFT"
};

void myPreCallback(void *arg)
{
    int     reason = *(int *)arg;
    OS_Printf("Go to sleep mode by %s\n", myCallbackReasonStr[reason]);
}

//----------------------------------------------------------------
//  myPostCallback
//
void myPostCallback(void *arg)
{
#pragma unused(arg)
    OS_Printf("Return from sleep mode.\n");
}

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - alarm-2
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.15  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.14  2005/11/21 10:53:59  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.13  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.12  2004/09/30 05:08:02  yada
  add OS_InitAlarm().  to remove it was mistake

  Revision 1.11  2004/09/28 12:21:26  yada
  not call OS_InitAlarm() or OS_InitVAlarm() because OS_Init() called

  Revision 1.10  2004/06/08 00:16:25  yada
  add '#pragma unused()' for FINALROM

  Revision 1.9  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.8  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.7  2004/02/25 11:35:21  yada
  systemClock→Tick に伴う変更

  Revision 1.6  2004/02/24 00:10:09  yada
  (none)

  Revision 1.5  2004/02/10 05:47:46  yasu
  delete macro CODE32

  Revision 1.4  2004/02/09 11:54:29  yasu
  include code32.h

  Revision 1.3  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.2  2004/02/04 07:37:45  yada
  OS_GetSystemClock(), OS_SetSystemClock()を
  OS_GetTime(),OS_SetTime() へ名称変更した関連で修正

  Revision 1.1  2004/02/03 12:03:20  yada
  firstRelease

  Revision 1.1  2004/02/03 11:25:17  yada
  firstRelease


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    alarmCallback(void *arg);
void    alarmCallback2(void *arg);
void    VBlankIntr(void);


static OSAlarm alarm;
static OSAlarm alarm2;

static BOOL called = FALSE;
static BOOL alarm2_sw = FALSE;

static int pushCounter = 0;
static int periodicCounter = 0;

static u16 keyData;


#define  ALARM_COUNT     (OS_MilliSecondsToTicks(200))
#define  ALARM_COUNT_P1  (OS_MilliSecondsToTicks(400))
#define  ALARM_COUNT_P2  (OS_MilliSecondsToTicks(500))

#define  TEST_DATA       0x12345678
#define  TEST_DATA2      0x9abcdef0


//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    OS_Init();
    GX_Init();

    // ---- initialize tick system
    OS_InitTick();
    // ---- initialize alarm system
    OS_InitAlarm();

    //---- setup VBlank
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr(TRUE);

    OS_Printf("Push [A]  to start alarm.\n");
    OS_Printf("Push [UP] to start periodic alarm. ([DOWN] to stop)\n");

    while (1)
    {
        OS_WaitVBlankIntr();
        keyData = PAD_Read();

        //---- PUSH A to start alarm
        if (keyData & PAD_BUTTON_A && !called)
        {
            pushCounter++;
            called = TRUE;

            //---- setting alarm
            OS_CreateAlarm(&alarm);
            OS_SetAlarm(&alarm, ALARM_COUNT, &alarmCallback, (void *)TEST_DATA);

            OS_Printf("set alarm\n");
        }


        //---- PUSH UP to start periodic alarm
        if (keyData & PAD_KEY_UP && !alarm2_sw)
        {
            alarm2_sw = TRUE;

            //---- setting periodic alarm
            OS_CreateAlarm(&alarm2);
            OS_SetPeriodicAlarm(&alarm2, OS_GetTick() + ALARM_COUNT_P1, ALARM_COUNT_P2,
                                &alarmCallback2, (void *)TEST_DATA2);

            OS_Printf("set periodic alarm\n");
        }

        //---- PUSH DOWN to stop periodic alarm
        if (keyData & PAD_KEY_DOWN && alarm2_sw)
        {
            alarm2_sw = FALSE;

            OS_CancelAlarm(&alarm2);
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         alarmCallback

  Description:  callback for alarm interrupt

  Arguments:    arg : user setting argument

  Returns:      None
 *---------------------------------------------------------------------------*/
void alarmCallback(void *arg)
{
#ifdef SDK_FINALROM
#pragma unused( arg )
#endif
    OS_Printf(">>> called alarmCallback. arg=%x PUSH=%d\n", arg, pushCounter);
    called = FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         alarmCallback2

  Description:  callback for alarm interrupt, for periodic alarm

  Arguments:    arg : user setting argument

  Returns:      None
 *---------------------------------------------------------------------------*/
void alarmCallback2(void *arg)
{
#ifdef SDK_FINALROM
#pragma unused( arg )
#endif
    OS_Printf(">>> called alarmCallback2. arg=%x  TICK=%llx\n", arg, OS_GetTick());
    periodicCounter++;
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  VBlank interrupt handler

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void VBlankIntr(void)
{
    //---- check interrupt flag
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*====== End of main.c ======*/

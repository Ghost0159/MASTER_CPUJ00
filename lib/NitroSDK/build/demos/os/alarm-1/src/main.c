/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - alarm-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.14  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.13  2005/11/21 10:53:49  kitase_hirotake
  SVC_WaitVBlankIntr ‚ð OS_WaitVBlankIntr ‚É•ÏX

  Revision 1.12  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.11  2004/09/30 05:08:02  yada
  add OS_InitAlarm().  to remove it was mistake

  Revision 1.10  2004/09/28 12:21:26  yada
  not call OS_InitAlarm() or OS_InitVAlarm() because OS_Init() called

  Revision 1.9  2004/06/08 00:16:05  yada
  add '#pragma unused()' for FINALROM

  Revision 1.8  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.7  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.6  2004/03/04 11:16:30  yada
  only fix comment

  Revision 1.5  2004/02/25 11:35:21  yada
  systemClock¨Tick ‚É”º‚¤•ÏX

  Revision 1.4  2004/02/10 05:47:46  yasu
  delete macro CODE32

  Revision 1.3  2004/02/09 11:18:07  yasu
  include code32.h

  Revision 1.2  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.1  2004/02/03 11:25:17  yada
  firstRelease


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    VBlankIntr(void);
void    alarmCallback(void *arg);

static OSAlarm alarm;

static BOOL called = FALSE;

static int pushCounter = 0;

static u16 keyData;


#define  ALARM_COUNT     (OS_MilliSecondsToTicks(200))
#define  TEST_DATA       0x12345678


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

    OS_Printf("Push [A] to start alarm.\n");

    while (1)
    {
        OS_WaitVBlankIntr();
        keyData = PAD_Read();

        if (keyData & PAD_BUTTON_A && !called)
        {
            pushCounter++;
            called = TRUE;

            //---- setting alarm
            OS_CreateAlarm(&alarm);
            OS_SetAlarm(&alarm, ALARM_COUNT, &alarmCallback, (void *)TEST_DATA);

            OS_Printf("set alarm\n");
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
    OS_Printf(">>> called alarmCallback. arg=%x\n", arg);
    OS_Printf(">>> PUSH=%d\n", pushCounter);
    OS_Printf(">>> sp=%x\n", OSi_GetCurrentStackPointer());
    called = FALSE;
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

/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - telnet
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:23  adachi_hiroaki
  telnetデモの追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  操作方法：
    A         - Enter
    B         - BackSpace
    L         - Control
    R         - Shift (Tinykbdのみ)
    SELECT    - Escape(Meta)
    Pad       - カーソルキー
    L + ]     - 切断
    
  動作確認済み：
    NetBSD 2.1
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include <string.h>

#include "console.h"
#include "telnet.h"
#include "netconnect.h"
#include "softkbd.h"

extern const u32        d_CharData[8 * 256];
extern const u32        d_PaletteData[8 * 16];

static ConsoleHandle    consMain;

static void editString(char* buffer, int length);

static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
    ConsoleUpdate(&consMain);
}

#define SITEDEFS_APCLASS    "WiFi.LAN.1.AP.1"
#define DEFAULTHOST         "192.168.105.200"

static void editString(char* buffer, int length)
{
    u16     padCurrent = 0, padPrevious = 0, padTrigger = 0;
    char    c;
    int     readtotal = STD_StrLen(buffer);
    length -= readtotal;

    ConsolePutString(buffer);
    buffer = (char*)((int)buffer + readtotal);
    while (TRUE)
    {
        padPrevious = padCurrent;
        padCurrent = PAD_Read();
        padTrigger = (u16) ((padPrevious ^ padCurrent) & padCurrent);
        c = 0;
        if (SoftkbdRead(&c, 1) == 0)
        {
            if (padTrigger & PAD_BUTTON_A)
            {
                c = '\n';
            }
        }

        if (c > 0)
        {
            ConsolePutChar(c);
            if (c == '\n')
            {
                *buffer = '\0';
                ConsolePutChar('\r');
                break;
            }
            else if (c == '\x08' && readtotal > 0)
            {
                --readtotal;
                ++length;
                --buffer;
                *buffer = '\0';
                ConsolePutString(" \x08");
            }
            else if (length > 0)
            {
                --length;
                ++readtotal;

                *buffer = c;
                ++buffer;
                *buffer = '\0';
            }
        }

        OS_Sleep(30);
    }
}

static BOOL telnet(const char* hostName)
{
    char        kbdBuffer[256];
    u32         nReadLength = 0;
    u16         padCurrent = 0, padPrevious = 0, padTrigger = 0;
    Telnet      telnet;
    Telnet*     pTelnetInstance = &telnet;

    ConsoleClear();
    ConsolePrintf("Connecting to %s...", hostName);
    if (TelnetConnect(pTelnetInstance, hostName, 23))
    {
        ConsolePutString("Connected\n");
    }
    else
    {
        ConsolePrintf("Failed\n");
        return TRUE;
    }

    while (TRUE)
    {
        padPrevious = padCurrent;
        padCurrent = PAD_Read();
        padTrigger = (u16) ((padPrevious ^ padCurrent) & padCurrent);
        if (!TelnetMain(pTelnetInstance))
        {
            break;
        }

        if (padTrigger & PAD_BUTTON_A)
        {
            /* Enter */
            TelnetWrite(pTelnetInstance, "\n", 1);
        }

        if (padTrigger & PAD_KEY_RIGHT)
        {
            TelnetWrite(pTelnetInstance, "\x1b[C", 3);
        }

        if (padTrigger & PAD_KEY_LEFT)
        {
            TelnetWrite(pTelnetInstance, "\x1b[D", 3);
        }

        if (padTrigger & PAD_KEY_UP)
        {
            TelnetWrite(pTelnetInstance, "\x1b[A", 3);
        }

        if (padTrigger & PAD_KEY_DOWN)
        {
            TelnetWrite(pTelnetInstance, "\x1b[B", 3);
        }

        if (padTrigger & PAD_BUTTON_SELECT)
        {
            /* Escape(META key) */
            TelnetWrite(pTelnetInstance, "\x1b", 1);
        }

        nReadLength = SoftkbdRead(kbdBuffer, sizeof(kbdBuffer));
        if (nReadLength > 0)
        {
            /* ^]で終了 */
            if (STD_StrStr(kbdBuffer, "\x1d") != NULL)
            {
                break;
            }

            TelnetWrite(pTelnetInstance, kbdBuffer, (int)nReadLength);
        }

        OS_Sleep(10);
    }
    (void)SOC_Close(pTelnetInstance->socket);
    ConsolePutString("\ndisconnected\n");
    ConsoleFlush();
    return TRUE;
}

void NitroMain()
{
    //---- init
    NcGlobalInit();

    FX_Init();
    GX_Init();
    TP_Init();
    GX_DispOff();
    GXS_DispOff();

    //---- init displaying
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void*)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();

    MI_CpuFillFast((void*)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void*)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void*)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void*)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    //---- setting 2D for top screen
    GX_SetBankForBG(GX_VRAM_BG_128_A);

    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000,
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);

    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    //---- setting 2D for bottom screen
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);

    GXS_SetGraphicsMode(GX_BGMODE_0);               // BGMODE is 0
    GXS_SetVisiblePlane(GX_PLANEMASK_BG1);

    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000,
                      GX_BG_EXTPLTT_01);

    G2S_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256,   // 256pix x 256pix text
                      GX_BG_COLORMODE_16,           // use 16 colors mode
                      GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(1);
    G2S_BG0Mosaic(FALSE);
    G2S_BG1Mosaic(FALSE);

    GXS_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GXS_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    GXS_LoadBG1Char(d_CharData, 0, sizeof(d_CharData));

    GX_DispOn();
    GXS_DispOn();
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    {
        ConsoleInit(&consMain);

        SoftkbdInit(0, 0);

        ConsolePrintf("\x1b[1;1HInitializing network..");
        NcStart(SITEDEFS_APCLASS);
        ConsolePrintf("ok.\n");
    }

    ConsolePrintf("Key assign:\n A      - Enter\n B      - BackSpace\n L      - Control\n"
#ifndef WITH_KBD
    " R      - Shift\n"
#endif
    " SELECT - Escape(Meta)\n\n");

    {
        char    szHost[256];
        (void)STD_StrCpy(szHost, DEFAULTHOST);
        while (TRUE)
        {
            ConsolePrintf("Please enter hostname:\n");
            editString(szHost, sizeof(szHost));
            if (szHost[0] == '\0')
            {
                break;
            }
            (void)telnet(szHost);
        };
    }
    {
        ConsolePrintf("Exiting network...\n");
        NcFinish();
        ConsolePrintf("Program terminated.\n");
        ConsoleFlush();
        OS_Terminate();
    }
}

/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - netconnect
  File:     netconnect.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: netconnect.c,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2005/12/22 05:22:41  adachi_hiroaki
  簡易自動接続ライブラリをデモとして追加

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>

#include <sitedefs.h>
#include "wcm_control.h"
#include "netconnect.h"

/*---------------------------------------------------------------------------*
   初期化
 *---------------------------------------------------------------------------*/
static void     Heap_Setup(void);
static void*    myAlloc(u32 name, s32 size);
static void     myFree(u32 name, void* ptr, s32 size);
static WcmControlApInfo apInfo;
static s32  previousAddr = 0;

/*---------------------------------------------------------------------------*
  Name        : NcGlobalInit
  Description : 
  Arguments   : なし。
  Returns     : 
 *---------------------------------------------------------------------------*/
void NcGlobalInit()
{
    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    Heap_Setup();
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
}

/*---------------------------------------------------------------------------*
  Name        : NcStart
  Description : 
  Arguments   : apClass     - SitDefsに定義されているアクセスポイントのクラス名
  Returns     : 
 *---------------------------------------------------------------------------*/
void NcStart(const char* apClass)
{
    int result;

    SiteDefs_Init();
    if (!InitWcmApInfo(&apInfo, apClass))
    {
        OS_Panic("Invalid AP Class....");
    }

    while (WCM_GetPhase() != WCM_PHASE_NULL)
    {
        OS_Sleep(100);
    }

    InitWcmControlByApInfo(&apInfo);

    OS_TPrintf("LINK UP....");
    while (WCM_GetPhase() != WCM_PHASE_DCF)
    {
        OS_Sleep(100);
    }

    if (previousAddr != 0)
    {
        /*
           DHCPサーバに前回取得したアドレスを要求するように設定する。
           アドレスプールを使い切らないようにするために必要。
         */
        SOCL_SetRequestedIP(SOC_NtoHl(previousAddr));
    }

    {
        SOCConfig   soc_config;
        soc_config.alloc = myAlloc;
        soc_config.free = myFree;
        soc_config.flag = SOC_FLAG_DHCP;
        soc_config.mtu = 0;
        soc_config.rwin = 0;

        result = SOC_Startup(&soc_config);
    }

    if (result < 0)
    {
        OS_Panic("SOC_Startup failed (%d)", result);
    }

    OS_TPrintf("DHCP....\n");

    while (SOC_GetHostID() == 0)
    {
        OS_Sleep(100);
    }

    OS_TPrintf("IP addr = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSMyIp));
    OS_TPrintf("NetMask = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSNetMask));
    OS_TPrintf("GW addr = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSGatewayIp));
    OS_TPrintf("DNS[0]  = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSDnsIp[0]));
    OS_TPrintf("DNS[1]  = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSDnsIp[1]));
}

void NcFinish()
{
    previousAddr = SOC_GetHostID();
    (void)SOC_Cleanup();
}

/*---------------------------------------------------------------------------*
  ヒープ関連
 *---------------------------------------------------------------------------*/

// MainRAM アリーナに対して メモリ割り当てシステム初期化
static void Heap_Setup(void)
{
    void*           nstart;
    OSHeapHandle    handle;

    nstart = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetMainArenaLo(nstart);
    handle = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, handle);
}

static void* myAlloc(u32 name, s32 size)
{
#pragma unused(name)
    OSIntrMode  enable = OS_DisableInterrupts();
    void*       ptr = OS_Alloc((u32) size);
    (void)OS_RestoreInterrupts(enable);
    return ptr;
}

static void myFree(u32 name, void* ptr, s32 size)
{
#pragma unused(name, size)
    OSIntrMode  enable = OS_DisableInterrupts();
    OS_Free(ptr);
    (void)OS_RestoreInterrupts(enable);
    return;
}

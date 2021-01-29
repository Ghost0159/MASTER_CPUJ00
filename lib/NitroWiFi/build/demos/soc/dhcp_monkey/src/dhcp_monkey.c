/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - dhcp_monkey
  File:     dhcp_monkey.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dhcp_monkey.c,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2005/12/22 04:28:44  adachi_hiroaki
  dhcp_monkey��SOC��

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include "wcm_control.h"
#include <sitedefs.h>

#define CLASS_AP    "WiFi.LAN.1.AP.1"

static void         Init(void);

/*---------------------------------------------------------------------------*
   MAIN
 *---------------------------------------------------------------------------*/
static void         Heap_Setup(void);
static void*        myAlloc(u32 name, s32 size);
static void         myFree(u32 name, void* ptr, s32 size);
static SOCConfig    myConfig;

static WcmControlApInfo apInfo;

void NitroMain(void)
{
    int result;
    int n;

    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    Heap_Setup();
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    SiteDefs_Init();

    MI_CpuClear8(&myConfig, sizeof(myConfig));
    myConfig.flag = SOC_FLAG_DHCP;  // USE DHCP
    myConfig.alloc = myAlloc;
    myConfig.free = myFree;

    // requested_ip �́A����A�N�Z�X�|�C���g�ɂ����ēd���� ON/OFF ��p�ɂɍs
    // �Ȃ��Ƃ��� DHCP �� IP �̈��H���Ԃ��Ă��܂����Ƃɑ΂���΍�ƂȂ���
    // ����D��U�ڑ��ł����Ƃ��� IP ���o�b�N�A�b�v�������Ȃǂɕۑ����Ă����āA
    // ����������Ŏw�肷��D����ɂ��ēx���� IP �A�h���X���擾���邱�Ƃ��ł���D
    // �ݒ�ł���Ȃ炵�������ǂ��D
    // SOCL_SetRequestedIP(xxx.xxx.xxx.xxx);
    if (!InitWcmApInfo(&apInfo, CLASS_AP))
    {
        OS_Panic("Invalid AP Class....");
    }

    for (n = 0;; n++)
    {
        OS_TPrintf("################################ DHCP Count=%d\n", n);

        // �����ڑ�
        InitWcmControlByApInfo(&apInfo);

        // WCM ����X���b�h�̃v���C�I���e�B(=24)�������C����������
        // ���邢�� InitWcmControl() �ďo����ɁASetWcmThreadPrority()
        // �� WCM ����X���b�h�̗D����グ��D
        // ���̑��Ƀ��C���X���b�h�������ɖ����I�� WCM �֐����n���Ƃ���
        // ���@������D���ꂼ��꒷��Z������̂łǂ̎�@���̗p�����
        // �ǂ��̂��̓A�v���P�[�V�����Ɉˑ�����.
        // �X���b�h�̗D�揇���������܂܂Ȃ�ʏ�̒ʐM��ԂɂȂ����Ƃ���
        // WCM �֘A�̏������������Ƃ��ɏ�����������\��������D
        // �܂������I�� WCM �֐����n�����@���A�R�[�h�����[�v�\����
        // �Ȃ��Ă��Ȃ��Ƃ��Ȃǂ̂悤�ɒ���I�� OS_Sleep �Ȃǂ̌Ăяo����
        // �L�q���ɂ����ꍇ������D
        //(void)SetWcmThreadPriority(10);
        OS_TPrintf("LINK UP....\n");
        while (WCM_GetPhase() != WCM_PHASE_DCF)
        {
            // WCM �X���b�h�������C���X���b�h�̗D�悪�����Ƃ���
            // �����I�ɏ����𖾂��n���K�v������
            OS_Sleep(100);
        }

        OS_TPrintf("DHCP....\n");

        (void)SOC_ClearConfigError(NULL);
        result = SOC_Startup(&myConfig);

        while (SOC_GetHostID() == 0 && SOC_GetConfigError(NULL) == 0)
        {
            // WCM �X���b�h�������C���X���b�h�̗D�悪�����Ƃ���
            // �����I�ɏ����𖾂��n���K�v������
            OS_Sleep(100);
        }

        if (SOC_GetHostID() != 0)
        {
            int retry = 0;

            OS_TPrintf("IP      = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(SOCL_GetHostID()));
            OS_TPrintf("NetMask = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSNetMask));
            OS_TPrintf("GW IP   = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSGatewayIp));
            OS_TPrintf("DNS[0]  = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSDnsIp[0]));
            OS_TPrintf("DNS[1]  = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSDnsIp[1]));

            OS_TPrintf("Resolving....\n");
            {
                SOCHostEnt*     hotEnt = SOC_GetHostByName("www.nintendo.co.jp");
                if (hotEnt)
                {
                    SOCInAddr   addr;
                    addr.addr = (u32) hotEnt->addrList;
                    OS_TPrintf("Resolved IP=%s\n", SOC_InetNtoA(addr));
                }
                else
                {
                    OS_TPrintf("NO RESOLVER or NO ENTRY\n");
                }
            }
        }
        else
        {
            // ���݂� WCM �̎d�l�ł̓����N�؂�����o�ł���̂�
            // ���ۂ̃����N���؂�Ă��牽�b���o���Ă���Ȃ̂�
            // DHCP �擾�̃^�C���A�E�g�̕�����ɐ������Ă��܂��ꍇ������D
            // ����� DHCP �Ɏ��s�����Ƃ��͕K�������N�؂ꂪ��������
            // �ƁA�݂Ȃ��čĐڑ����ɂ̓����N�m������͂��߂Ă��炤�������S�D
            OS_TPrintf("NO DHCP SERVER or NO LINK....\n");

            switch (SOC_GetConfigError(NULL))
            {
            case SOC_IP_ERR_DHCP_TIMEOUT:
                OS_TPrintf("SOC_IP_ERR_DHCP_TIMEOUT\n");
                break;

            case SOC_IP_ERR_LINK_DOWN:
                OS_TPrintf("SOC_IP_ERR_LINK_DOWN\n");
                break;

            default:
                OS_TPrintf("Default???\n");
                break;
            }
        }

        OS_TPrintf("LINK DOWN....\n");
        (void)SOC_Cleanup();
        TerminateWcmControl();
        while (WCM_GetPhase() != WCM_PHASE_NULL)
        {
            // WCM �X���b�h�������C���X���b�h�̗D�悪�����Ƃ���
            // �����I�ɏ����𖾂��n���K�v������
            OS_Sleep(100);
        }
    }

    OS_TPrintf("Terminate....\n");
    OS_Terminate();
}

/*---------------------------------------------------------------------------*
  �q�[�v�֘A
 *---------------------------------------------------------------------------*/
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

/*====== End of dhcp_monkey.c ======*/

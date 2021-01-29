/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     main.c

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2007/03/09 01:16:16  kitase_hirotake
  indent source

  Revision 1.9  2006/03/15 04:44:46  adachi_hiroaki
  �\����ssl-1�ɂ���

  Revision 1.8  2006/03/15 04:14:35  adachi_hiroaki
  alloc/free must be given�̌x�����o�Ȃ��悤�ɂ���

  Revision 1.7  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.6  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.5  2005/10/25 04:19:19  terui
  SVC_WaitVBlankIntr -> OS_WaitVBlankIntr

  Revision 1.4  2005/08/09 00:59:08  seiki_masashi
  �s����'�\'�����

  Revision 1.3  2005/08/08 05:55:36  seiki_masashi
  CPS_AddRandomSeed �֐������C�����[�v���ŌĂяo���悤�ɕύX

  Revision 1.2  2005/07/29 09:49:16  seiki_masashi
  small fix

  Revision 1.1  2005/07/21 11:47:03  seiki_masashi
  ssl/ssl-1 �Ɉړ�

  Revision 1.3  2005/07/21 11:18:45  seiki_masashi
  random_seed �� RTC ����擾����悤�ɕύX

  Revision 1.2  2005/07/18 23:56:12  seiki_masashi
  �{�^�����Ƃɋ@�\���t��

  Revision 1.1  2005/07/18 08:10:26  seiki_masashi
  ssl-1 �f���̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    ��ʓI�Ȗ����A�N�Z�X�|�C���g ( AP ) �ɐڑ�������ŁACPS ���C�u������ SSL
    �@�\���g�p���� SSL �N���C�A���g�Ƃ��� SSL �T�[�o�ƒʐM���s���T���v���ł��B
    AP �̏��� NitroWiFi/build/demos/shared/ap_info.c �ɋL�q����Ă���܂��̂ŁA
    ���̏���K�X���������Ďg�p���ĉ������BBSSID�AESSID ���Œ�l�ł͂Ȃ��C�Ӓl
    �ɐݒ肷��ƁA��Ԏn�߂Ɍ������� AP �ɐڑ������݂܂��B
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include "screen.h"
#include "wcm_control.h"
#include "wcm_cpsif.h"
#include "tcp_client.h"

/*---------------------------------------------------------------------------*
    �萔 ��`
 *---------------------------------------------------------------------------*/
#define KEY_REPEAT_START    25  // �L�[���s�[�g�J�n�܂ł̃t���[����
#define KEY_REPEAT_SPAN     10  // �L�[���s�[�g�̊Ԋu�t���[����

/*---------------------------------------------------------------------------*
    �\���� ��`
 *---------------------------------------------------------------------------*/

// �L�[���͏��
typedef struct KeyInfo
{
    u16 cnt;    // �����H���͒l
    u16 trg;    // �����g���K����
    u16 up;     // �����g���K����
    u16 rep;    // �����ێ����s�[�g����
} KeyInfo;

/*---------------------------------------------------------------------------*
    �����ϐ� ��`
 *---------------------------------------------------------------------------*/

// �L�[����
static KeyInfo  gKey;

/*---------------------------------------------------------------------------*
    �����֐� �v���g�^�C�v
 *---------------------------------------------------------------------------*/
static void     ProcMain(void);
static void     VBlankIntr(void);
static void     InitInterrupts(void);
static void     InitHeap(void);
static void     ReadKey(KeyInfo* pKey);

static void     InitNetwork(void);
static void     inform_ip(void);
static void     GetPage(void);

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
    OS_InitTick();          // OS_Sleep �֐����g�p����ׂɕK�v
    OS_InitAlarm();         // OS_Sleep �֐����g�p����ׂɕK�v
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    RTC_Init();

    // ���������蓖�ď�����
    InitHeap();

    // �\���ݒ菉����
    InitScreen();

    // ���荞�ݐݒ菉����
    InitInterrupts();

    // LCD �\���J�n
    GX_DispOn();
    GXS_DispOn();

    // �f�o�b�O������o��
    OS_TPrintf("\"ssl-1\" demo started.\n");
    PrintSubScreen("\"ssl-1\" demo started.\n");

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    ReadKey(&gKey);

    InitNetwork();

    while (TRUE)
    {
        // �L�[���͏��擾
        ReadKey(&gKey);

        // ���C����ʃN���A
        ClearMainScreen();

        ProcMain();
        inform_ip();

        // 16 �t���[�����Ƃ� SSL �p�̗����̎������������
        if (OS_GetVBlankCount() % 16 == 0)
        {
            u32 data[OS_LOW_ENTROPY_DATA_SIZE/sizeof(u32)];
            OS_GetLowEntropyData(data);

            // CPS_SslAddRandomSeed �� CPS �������O�ł��Ăяo���\ 
            CPS_SslAddRandomSeed(data, sizeof(data));
        }

        // �u�u�����N�҂� ( �X���b�h�Ή� )
        OS_WaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  Name:         ProcMain

  Description:  WCM ���C�u�����̐ڑ��Ǘ���Ԃ̕\���A�y�ѓ��͂ɂ�鏈����
                �؂�ւ����s���B���C�����[�v����s�N�`���[�t���[�����Ƃ�
                �Ăяo�����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ProcMain(void)
{
    switch (WCM_GetPhase())
    {
    case WCM_PHASE_NULL:
        // ������� �������� WCM �R���g���[���X���b�h���I���������
        ((u16*)HW_PLTT)[0] = 0x7fff;
        PutMainScreen(11, 10, 0x0, "Now offline");
        PutMainScreen(7, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push A to connect");

        //if ((gKey.trg | gKey.rep) & PAD_BUTTON_A)
        {
            InitWcmControl();
        }
        break;

    case WCM_PHASE_DCF:
        // �ڑ����ꂽ���
        ((u16*)HW_PLTT)[0] = 0x7ff0;
        PutMainScreen(11, 10, 0x0, "Now online");
        PutMainScreen(6, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push A to disconnect");
        PutMainScreen(6, 13, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push B to get a page");
        if ((gKey.trg | gKey.rep) & PAD_BUTTON_A)
        {
            TerminateWcmControl();
        }
        else if ((gKey.trg | gKey.rep) & PAD_BUTTON_B)
        {
            GetPage();
        }
        break;

    case WCM_PHASE_FATAL_ERROR:
        // �����s�\�� FATAL ERROR
        ((u16*)HW_PLTT)[0] = 0x001f;
        PutMainScreen(0, 7, 0xf, "================================");
        PutMainScreen(7, 10, 0xf, "WCM FATAL ERROR !");
        PutMainScreen(2, 12, 0xf, "Turn off power supply please.");
        PutMainScreen(0, 15, 0xf, "================================");
        OS_WaitVBlankIntr();
        OS_Terminate();
        break;

    default:
        // ������
        ((u16*)HW_PLTT)[0] = 0x6318;
        PutMainScreen(10, 10, 0x0, "Now working...");
        PutMainScreen(9, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push A to stop");
        if ((gKey.trg | gKey.rep) & PAD_BUTTON_A)
        {
            TerminateWcmControl();
        }
    }

    if ((gKey.trg | gKey.rep) & PAD_BUTTON_X)
    {
        OS_DumpThreadList();
    }
}

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �u�u�����N�����݃n���h���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // �e�L�X�g�\�����X�V
    UpdateScreen();

    // IRQ �`�F�b�N�t���O���Z�b�g
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         InitInterrupts

  Description:  ���荞�ݐݒ������������B
                V �u�����N���荞�݂������A���荞�݃n���h����ݒ肷��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitInterrupts(void)
{
    // V �u�����N���荞�ݐݒ�
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);

    // ���荞�݋���
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
}

/*---------------------------------------------------------------------------*
  Name:         InitHeap

  Description:  ���C����������̃A���[�i�ɂă����������ăV�X�e��������������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitHeap(void)
{
    void*           tempLo;
    OSHeapHandle    hh;

    // ���C����������̃A���[�i�Ƀq�[�v���ЂƂ쐬
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        // �q�[�v�쐬�Ɏ��s�����ꍇ�ُ͈�I��
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

/*---------------------------------------------------------------------------*
  Name:         ReadKey

  Description:  �L�[���͏����擾���A���͏��\���̂�ҏW����B
                �����g���K�A�����g���K�A�����p�����s�[�g�g���K �����o����B

  Arguments:    pKey  - �ҏW����L�[���͏��\���̂��w�肷��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ReadKey(KeyInfo* pKey)
{
    static u16  repeat_count[12];
    int         i;
    u16         r;

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
                    repeat_count[i] = (u16) (KEY_REPEAT_START - KEY_REPEAT_SPAN);
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

    pKey->cnt = r;  // �����H�L�[����
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//  Initialize network
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
static CPSConfig    netconfig;

//
//  Callback routine called:
//
//  1) immediately after CPS_Startup() if CPS_DONOTUSE_DHCP is on
//
//      or
//
//  2) after the attempt to look for a DHCP server fails
//     if CPS_DONOTUSE_DHCP is off

//
static void dhcp_callback()
{
    OS_TPrintf("dhcp_callback() entered\n");
    CPSMyIp = CPS_MK_IPv4(192, 168, 11, 200);
    CPSNetMask = CPS_MK_IPv4(255, 255, 255, 0);
}

//
//  Returns TRUE if the link is ON

//
static BOOL link_is_on()
{
    return WCM_GetApMacAddress() != NULL;
}

static void* my_alloc(u32 size)
{
    void*       p;

    OSIntrMode  enabled = OS_DisableInterrupts();
    p = OS_Alloc(size);
    (void)OS_RestoreInterrupts(enabled);
    return p;
}

static void my_free(void* p)
{
    OSIntrMode  enabled = OS_DisableInterrupts();
    OS_Free(p);
    (void)OS_RestoreInterrupts(enabled);
}

static u8   lan_buf[16384];

static void InitNetwork()
{
    WCM_SetRecvDCFCallback(CPSi_RecvCallbackFunc);
    netconfig.dhcp_callback = dhcp_callback;
    netconfig.link_is_on = link_is_on;
    netconfig.alloc = my_alloc;
    netconfig.free = my_free;
    netconfig.lan_buf = lan_buf;
    netconfig.lan_buflen = sizeof(lan_buf);
    OS_Printf("OS_GetTick() == %lld\n", OS_GetTick());
    {
        RTCDate date;
        RTCTime time;
        s64     sec;
        (void)RTC_GetDateTime(&date, &time);
        sec = RTC_ConvertDateTimeToSecond(&date, &time);
        if (sec >= 0)
        {
            netconfig.random_seed = (u32) sec;
        }
        else
        {
            netconfig.random_seed = 0ULL;
        }
    }
    {
        u32 data[OS_LOW_ENTROPY_DATA_SIZE/sizeof(u32)];
        OS_GetLowEntropyData(data);
        CPS_SslAddRandomSeed(data, sizeof(data));
    }

    //  netconfig.mode = CPS_DONOTUSE_DHCP;
    CPS_Startup(&netconfig);
}

static void inform_ip()
{
    char        tmp[128];
    static int  have_ip = 0;

    if (have_ip)
    {
        if (CPSMyIp == 0)
        {
            have_ip = 0;    // lost IP
        }

        return;
    }
    else if (CPSMyIp == 0)
    {
        return;             // IP not yet established
    }

    have_ip = 1;
    (void)OS_SPrintf(tmp, "MAC  = %02x:%02x:%02x:%02x:%02x:%02x\n", CPSMyMac[0], CPSMyMac[1], CPSMyMac[2], CPSMyMac[3],
                     CPSMyMac[4], CPSMyMac[5]);
    PrintSubScreen(tmp);
    (void)OS_SPrintf(tmp, "ip   = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSMyIp));
    PrintSubScreen(tmp);
    (void)OS_SPrintf(tmp, "mask = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSNetMask));
    PrintSubScreen(tmp);
    (void)OS_SPrintf(tmp, "gwip = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSGatewayIp));
    PrintSubScreen(tmp);
    (void)OS_SPrintf(tmp, "DNS1 = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSDnsIp[0]));
    PrintSubScreen(tmp);
    if (CPSDnsIp[1] != 0)
    {
        (void)OS_SPrintf(tmp, "   2 = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSDnsIp[1]));
        PrintSubScreen(tmp);
    }
    (void)OS_SPrintf(tmp, "DHCP = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSDhcpServerIp));
    PrintSubScreen(tmp);
}

static void GetPage(void)
{
    // �ē��\�ł͂Ȃ�
    static HttpClientRequest    req;
    req.host = "192.168.0.10";
    req.path = "/";
#if 1
    req.port = 4433;
    req.https = TRUE;
#else
    req.port = 80;
    req.https = FALSE;
#endif
    StartHttpClient(&req);
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

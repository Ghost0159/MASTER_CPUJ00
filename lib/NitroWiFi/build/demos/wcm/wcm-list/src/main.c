/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - demos - wcm-list
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.6  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.5  2005/10/25 04:19:19  terui
  SVC_WaitVBlankIntr -> OS_WaitVBlankIntr

  Revision 1.4  2005/08/29 06:39:36  yosizaki
  add SetWcmManualConnect().

  Revision 1.3  2005/07/25 07:48:46  adachi_hiroaki
  �ꗗ���Z�b�g�{�^���ǉ�

  Revision 1.2  2005/07/22 01:07:40  adachi_hiroaki
  �f�t�H���g��BSSID��ANY�̏ꍇ�͑I�����[�h�ɂ����ڍs���Ȃ��悤�ɂ���

  Revision 1.1  2005/07/21 08:21:06  adachi_hiroaki
  �V�K�ǉ�


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    WCM ���C�u�������g�p���� ��ʓI�Ȗ����A�N�Z�X�|�C���g ( AP ) ����������
    ���X�g�A�b�v���A�ڑ�����T���v���ł��B
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include "screen.h"
#include "wcm_control.h"

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

//�f�t�H���g�̐ڑ���BSSID
static u8       gBssidDefault[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static int      gSelectedAP = 0;
static BOOL     gConnectAuto = FALSE;

/*---------------------------------------------------------------------------*
    �����֐� �v���g�^�C�v
 *---------------------------------------------------------------------------*/
static void     ProcMain(void);
static void     SelectAP(void);
static const WMBssDesc*     GetBssDesc(const u8* bssid);
static void VBlankIntr(void);
static void InitInterrupts(void);
static void InitHeap(void);
static void ReadKey(KeyInfo* pKey);

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
    OS_InitTick();  // OS_Sleep �֐����g�p����ׂɕK�v
    OS_InitAlarm(); // OS_Sleep �֐����g�p����ׂɕK�v
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

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
    OS_TPrintf("\"wcm-list\" demo started.\n");
    PrintSubScreen("\"wcm-list\" demo started.\n");

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    ReadKey(&gKey);

    while (TRUE)
    {
        // �L�[���͏��擾
        ReadKey(&gKey);

        // ���C����ʃN���A
        ClearMainScreen();

        ProcMain();

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
        PutMainScreen(4, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push any key to search AP");
        if (gKey.trg | gKey.rep)
        {
            SetWcmManualConnect(TRUE);
            InitWcmControl();
        }

        if (!WCM_CompareBssID(WCM_BSSID_ANY, gBssidDefault))
        {
            PutMainScreen(5, 23, 0x0, "Start button: auto connect");
            if (gKey.trg & PAD_BUTTON_START)
            {
                gConnectAuto = TRUE;
            }
            else
            {
                gConnectAuto = FALSE;
            }
        }
        break;

    case WCM_PHASE_SEARCH:
        // AP���������̏��
        if (gConnectAuto)
        {
            // �����ڑ����[�h
            PutMainScreen(11, 10, 0x0, "Now online");
            PutMainScreen(4, 11, 0x0, "connecting to default AP");
            PutMainScreen(3, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push any key to select AP");
            if (gKey.trg | gKey.rep)
            {
                // �L�[��������Ă����AP�I�����[�h��
                gConnectAuto = FALSE;
            }
            else
            {
                //�f�t�H���g��BssID������AP��T��
                const WMBssDesc*    pbd = GetBssDesc(gBssidDefault);
                if (pbd)
                {
                    //������ΐڑ�����
                    ConnectAP(pbd);
                }
            }
        }
        else
        {
            // AP�I�����[�h
            SelectAP();
        }
        break;

    case WCM_PHASE_DCF:
        // �ڑ����ꂽ���
        ((u16*)HW_PLTT)[0] = 0x7ff0;
        PutMainScreen(11, 10, 0x0, "Now online");
        PutMainScreen(3, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push any key to disconnect");
        if (gKey.trg | gKey.rep)
        {
            TerminateWcmControl();
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
        PutMainScreen(6, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push any key to stop");
        if (gKey.trg | gKey.rep)
        {
            TerminateWcmControl();
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         SelectAP

  Description:  ���p�ł���A�N�Z�X�|�C���g�ꗗ��\�����I������

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SelectAP(void)
{
    int     nIndex = 0;
    int     nAPCount = 0;
    char    buffer[256];
    s32     nLevel;
    BOOL    bDefault = FALSE;

    PutMainScreen(0, 0, 0xf, " bssid      + ssid/link");
    PutMainScreen(0, 23, 0xf, "A:connect, Sel:set def, St:clear");

    (void)WCM_LockApList(WCM_APLIST_LOCK);
    nAPCount = WCM_CountApList();
    if (nAPCount == 0)
    {
        //AP�����������Ȃ���Ε\���E�I�����Ȃ�
        (void)WCM_LockApList(WCM_APLIST_UNLOCK);
        return;
    }

    //�I���J�[�\���̈ړ�
    if (gKey.trg & PAD_KEY_DOWN)
    {
        ++gSelectedAP;
    }

    if (gKey.trg & PAD_KEY_UP)
    {
        --gSelectedAP;
    }

    gSelectedAP = MATH_CLAMP(gSelectedAP, 0, nAPCount - 1);
    {
        WMBssDesc*  pbdSelected = WCM_PointApList(gSelectedAP);
        if (pbdSelected)
        {
            if (gKey.trg & PAD_BUTTON_A)
            {
                ConnectAP(pbdSelected);
            }

            if (gKey.trg & PAD_BUTTON_SELECT)
            {
                MI_CpuCopy8(pbdSelected->bssid, gBssidDefault, WM_SIZE_BSSID);
            }
        }
    }

    for (nIndex = 0; nIndex < nAPCount; ++nIndex)
    {
        WMBssDesc*  pbd = WCM_PointApList(nIndex);
        bDefault = WCM_CompareBssID(gBssidDefault, pbd->bssid);
        nLevel = WCM_PointApListLinkLevel(nIndex);
        (void)OS_SPrintf(buffer, "%02X%02X%02X%02X%02X%02X%s%s/%d", pbd->bssid[0], pbd->bssid[1], pbd->bssid[2],
                         pbd->bssid[3], pbd->bssid[4], pbd->bssid[5], bDefault ? "*" : " ", pbd->ssid, nLevel);
        PutMainScreen(0, 1 + nIndex, (u8) (nIndex == gSelectedAP ? 0x1 : 0xf), buffer);
    }
    (void)WCM_LockApList(WCM_APLIST_UNLOCK);

    if (gKey.trg & PAD_BUTTON_START)
    {
        WCM_ClearApList();
    }

}

/*---------------------------------------------------------------------------*
  Name:         GetBssDesc

  Description:  �w�肳�ꂽBssID��������AP���������Ă���΂���BssDesc�𓾂�B

  Arguments:    BssDesc�𓾂���AP��BssID

  Returns:      ��������AP��BssDesc�ւ̃|�C���^
 *---------------------------------------------------------------------------*/
static const WMBssDesc* GetBssDesc(const u8* bssid)
{
    int nAPCount = 0;
    int nIndex = 0;

    (void)WCM_LockApList(WCM_APLIST_LOCK);
    nAPCount = WCM_CountApList();
    for (nIndex = 0; nIndex < nAPCount; ++nIndex)
    {
        WMBssDesc*  pbd = WCM_PointApList(nIndex);
        if (WCM_CompareBssID((u8*)bssid, pbd->bssid))
        {
            (void)WCM_LockApList(WCM_APLIST_UNLOCK);
            return pbd;
        }
    }
    (void)WCM_LockApList(WCM_APLIST_UNLOCK);
    return NULL;
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

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

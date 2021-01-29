/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - shared - demos
  File:     wcm_control.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_control.c,v $
  Revision 1.13  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.12  2005/11/02 04:13:42  terui
  Fix comment.

  Revision 1.11  2005/09/13 13:14:14  terui
  DisconnectAP�֐��̒ǉ��ɔ�������

  Revision 1.10  2005/09/08 10:42:38  adachi_hiroaki
  ���E���h�X�L�����̉񐔂��擾�ł���悤�ɂ����B�܂��R�[���o�b�N�֐����̃f�o�b�O�\������񂾂��\�������悤�ɂ����B

  Revision 1.9  2005/09/06 02:53:44  yasu
  nitroWiFi/env.h �� NitroSDK �ւ̈ړ��ɔ����C��

  Revision 1.8  2005/09/05 04:00:49  terui
  �X�L���������A�d�͊Ǘ����[�h���O������ύX�\�Ȃ悤�ɏC���B

  Revision 1.7  2005/08/29 06:39:02  yosizaki
  add SetWcmManualConnect(), ConnectAP().

  Revision 1.6  2005/08/23 01:56:35  seiki_masashi
  rename ENV_GetBinaryAndLength() to ENV_GetBinaryAndSize()

  Revision 1.5  2005/08/22 12:10:19  yasu
  SiteDefs ���Q�Ƃ���悤�ɏC��

  Revision 1.4  2005/07/27 08:36:22  terui
  Add SetWcmThreadPriority API

  Revision 1.3  2005/07/22 11:18:56  terui
  �X�^�b�N�̒���w�肷��ۂ̋L�q�̎d���𓝈�

  Revision 1.2  2005/07/22 10:36:17  terui
  �X���b�h�X�^�b�N�̒�̈ʒu�w�肪�Ԉ���Ă����o�O���C���B

  Revision 1.1  2005/07/21 08:38:16  terui
  �T���v���ŋ��L�\�ȃ\�[�X�t�@�C���Ƃ��ĐV�K�ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro/os.h>
#include <nitroWiFi.h>
#include "wcm_control.h"
#include "ap_info.h"

/*---------------------------------------------------------------------------*
    �萔 ��`
 *---------------------------------------------------------------------------*/
#define LOCAL_WCM_THREAD_STACK_SIZE     1024    // ( �ύX�� ) �傫�߂ɃX�^�b�N���m��
#define LOCAL_WCM_THREAD_PRIORITY       24      // ( �ύX�� ) ��߂̗D��x�ŏ\��
#define LOCAL_WCM_DMA_NO                3       // ( �ύX�� ) �����@�\�Ɏg�p���� DMA �ԍ�
#define LOCAL_WCM_APLIST_COUNT_MAX      22      // WCM ���C�u�������ŊǗ������� AP ��񃊃X�g�̐�
#define LOCAL_WCM_APLIST_BUF_SIZE       ((LOCAL_WCM_APLIST_COUNT_MAX * WCM_APLIST_BLOCK_SIZE) + 12)
#define LOCAL_WCM_THREAD_MSGQ_CAPACITY  3       // ���b�Z�[�W�L���[�̒i��
#define LOCAL_WCM_MSG_TERMINATE         ((void*)0)
#define LOCAL_WCM_MSG_FOUND_AP          ((void*)1)
#define LOCAL_WCM_MSG_DISCONNECTED      ((void*)2)
#define LOCAL_WCM_MSG_ERROR             ((void*)3)
#define LOCAL_WCM_MSG_CONNECT_AP        ((void*)4)
#define LOCAL_WCM_MSG_DISCONNECT        ((void*)5)

// WCM ���C�u�������샍�O�\���p ������萔
static const char*  gWcmResultText[8] =
{
    { "SUCCESS" } ,         { "FAILURE" } ,
    { "PROGRESS" } ,        { "ACCEPT" } ,
    { "REJECT" } ,          { "WMDISABLE" } ,
    { "NOT_ENOUGH_MEM" } ,  { "FATAL_ERROR" }
};
static const char*  gWcmNotifyText[10] =
{
    { "COMMON" } ,          { "STARTUP" } ,
    { "CLEANUP" } ,         { "BEGIN_SEARCH" } ,
    { "END_SEARCH" } ,      { "CONNECT" } ,
    { "DISCONNECT" } ,      { "FOUND_AP" } ,
    { "SEARCH_AROUND" } ,   { "TERMINATE" }
};

// �f�o�b�O������\���ݒ�
#define LOCAL_DEBUG_PRINT   1
#if LOCAL_DEBUG_PRINT
#define DebugPrintf OS_TPrintf
#else
#define DebugPrintf(...)    ((void)0)
#endif

/*---------------------------------------------------------------------------*
    �����ϐ� ��`
 *---------------------------------------------------------------------------*/

// �X���b�h����p
static OSThread         gWcmThread;
static u32          gWcmStack[LOCAL_WCM_THREAD_STACK_SIZE / sizeof( u32 ) ];
static OSMessageQueue   gWcmMessageQ;
static OSMessage        gWcmMsgArray[LOCAL_WCM_THREAD_MSGQ_CAPACITY];
static OSThreadQueue    gWcmThreadQ;

// WCM ���C�u��������p
static volatile BOOL    gWcmTerminateFlag;
static vs32         gWcmResult;
static void*        gWcmBuf = NULL;
static WCMConfig    gWcmConfig;
static WMBssDesc gWcmBssDesc    ATTRIBUTE_ALIGN(4);
static u32  gWcmScanType = WCM_OPTION_SCANTYPE_ACTIVE;
static u32  gWcmPowerMode = WCM_OPTION_POWER_ACTIVE;

static BOOL gWcmManualConnect = FALSE;
static u32  gWcmSearchAroundCount = 0;

static const WcmControlApInfo*  constApInfoPtr = NULL;

/*---------------------------------------------------------------------------*
    �����֐� �v���g�^�C�v
 *---------------------------------------------------------------------------*/
static void ControlThread(void* arg);
static void FreezeThread(void);
static s32  CallFunction(s16 notifyId);
static void ControlCallback(WCMNotice* arg);

/*---------------------------------------------------------------------------*
  Name:         SetWcmScanType

  Description:  AP ��T������ۂ̃X�L����������ݒ肷��B
                �f�t�H���g�ł̓A�N�e�B�u�X�L���������ƂȂ��Ă���B
                InitWcmControl() ����ɌĂяo���K�v������B

  Arguments:    type    -   WCM_OPTION_SCANTYPE_* �̒萔���w�肷��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SetWcmScanType(u32 type)
{
    switch (type)
    {
    case WCM_OPTION_SCANTYPE_PASSIVE:
        gWcmScanType = WCM_OPTION_SCANTYPE_PASSIVE;
        break;

    default:
        gWcmScanType = WCM_OPTION_SCANTYPE_ACTIVE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         SetWcmPowerMode

  Description:  AP �ƒʐM����ۂ̓d�͊Ǘ��������w�肷��B
                �f�t�H���g�ł͏펞�A�N�e�B�u�����ƂȂ��Ă���B
                InitWcmControl() ����ɌĂяo���K�v������B

  Arguments:    mode    -   WCM_OPTION_POWER_* �̒萔���w�肷��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SetWcmPowerMode(u32 mode)
{
    switch (mode)
    {
    case WCM_OPTION_POWER_SAVE:
        gWcmPowerMode = WCM_OPTION_POWER_SAVE;
        break;

    default:
        gWcmPowerMode = WCM_OPTION_POWER_ACTIVE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         SetWcmManualConnectMode

  Description:  AP�̃��X�g����蓮�őI�����Đڑ�����悤�ݒ�.
                �f�t�H���g�ł� FALSE (�����ڑ�)�ƂȂ��Ă���.
                InitWcmControl() ����ɌĂяo���K�v������.

  Arguments:    enable -   �蓮�Őڑ�����ꍇ�� TRUE�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SetWcmManualConnect(BOOL enable)
{
    gWcmManualConnect = enable;
}

/*---------------------------------------------------------------------------*
  Name:         ConnectAP

  Description:  �w�肳�ꂽBssDesc��AP�֐ڑ�����

  Arguments:    bd     -   �ڑ�����AP�������\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ConnectAP(const WMBssDesc* pbd)
{
    SDK_NULL_ASSERT(pbd);
    MI_CpuCopy8(pbd, &gWcmBssDesc, /*sizeof(WMBssDesc)*/ WCM_APLIST_SIZE);
    (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_CONNECT_AP, OS_MESSAGE_NOBLOCK);
}

/*---------------------------------------------------------------------------*
  Name:         DisconnectAP

  Description:  AP �Ƃ̐ڑ��̐ؒf�����݂�

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void DisconnectAP(void)
{
    OSIntrMode  e = OS_DisableInterrupts();
    if (WCM_GetPhase() == WCM_PHASE_DCF)
    {
        (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_DISCONNECT, OS_MESSAGE_NOBLOCK);
    }
    (void)OS_RestoreInterrupts(e);
}

/*---------------------------------------------------------------------------*
  Name:         InitWcmControl/InitWcmControlByApInfo

  Description:  �����ϐ������������AWCM �R���g���[���X���b�h���N������B
                ���ɃX���b�h���N�����Ă���ꍇ�͉����s��Ȃ��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitWcmControl(void)
{
    InitWcmControlByApInfo(&constApInfo);
}

void InitWcmControlByApInfo(const WcmControlApInfo* ptr)
{
    // �X���b�h�ϐ������ł����������ꂽ���H
    if (gWcmThread.id != 0)
    {
        // �X���b�h�͏I����Ԃɂ��邩�H
        if (OS_IsThreadTerminated(&gWcmThread) == FALSE)
        {
            return;
        }
    }

    constApInfoPtr = ptr;

    // ���b�Z�[�W�L���[��������
    OS_InitMessageQueue(&gWcmMessageQ, gWcmMsgArray, LOCAL_WCM_THREAD_MSGQ_CAPACITY);

    // �X���b�h�L���[��������
    OS_InitThreadQueue(&gWcmThreadQ);

    // �ϐ�������
    MI_CpuClearFast(&gWcmBssDesc, sizeof(WMBssDesc));

    // �X���b�h�ϐ����g���ĐV�K�ɃX���b�h���쐬
    OS_CreateThread(&gWcmThread, ControlThread, NULL, (void*)(gWcmStack + (LOCAL_WCM_THREAD_STACK_SIZE / sizeof(u32))),
                    LOCAL_WCM_THREAD_STACK_SIZE, LOCAL_WCM_THREAD_PRIORITY);

    // �쐬�����X���b�h���N��
    gWcmTerminateFlag = FALSE;
    OS_WakeupThreadDirect(&gWcmThread);
}

/*---------------------------------------------------------------------------*
  Name:         TerminateWcmControl

  Description:  WCM �R���g���[���X���b�h���I������B
                �X���b�h���N������Ă��Ȃ��ꍇ�͉����s��Ȃ��B
    NOTICE:     ���֐��̓X���b�h���I������ׂ̃g���K�ƂȂ邾���Ȃ̂ŁA�X���b�h
                ���̂��I������܂ő҂����ɖ߂�_�ɒ��ӁB
                �X���b�h���̂̏I����҂������ꍇ�́AOS_SendMessage �Ăяo����
                ������ OS_JoinThread ���Ăяo���K�v������B
                �Ȃ��AWCM_GetPhase �֐��� WCM_PHASE_NULL ��Ԃ��悤�ɂȂ��
                �X���b�h�͏I�����Ă���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void TerminateWcmControl(void)
{
    // �X���b�h�ϐ������ł����������ꂽ���H
    if (gWcmThread.id != 0)
    {
        // �X���b�h�͏I����Ԃɂ��邩�H
        if (OS_IsThreadTerminated(&gWcmThread) == FALSE)
        {
            // ���ɏI���v�����󂯕t���Ă����Ԃ��H
            if (gWcmTerminateFlag == FALSE)
            {
                gWcmTerminateFlag = TRUE;
                (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_TERMINATE, OS_MESSAGE_NOBLOCK);
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         SetWcmThreadPriority

  Description:  WCM ���C�u�����̏�ԊǗ����s���X���b�h�̗D��x��ύX����B

  Arguments:    priority    -   0 �` 31 �̃X���b�h�D��x���w�肷��B

  Returns:      u8  -   �ύX����O�ɐݒ肳��Ă����D��x��Ԃ��B
                        �Ȃ�炩�̗��R�ŗD��x�̕ύX�Ɏ��s�����ꍇ�� 0xff ��Ԃ��B
 *---------------------------------------------------------------------------*/
u8 SetWcmThreadPriority(u8 priority)
{
    u8  beforePrio = 0xff;

    if (priority < 32)
    {
        if ((gWcmThread.id != 0) && (OS_IsThreadTerminated(&gWcmThread) == FALSE))
        {
            beforePrio = (u8) OS_GetThreadPriority(&gWcmThread);
            if (FALSE == OS_SetThreadPriority(&gWcmThread, (u32) priority))
            {
                beforePrio = 0xff;
            }
        }
    }

    return beforePrio;
}

/*---------------------------------------------------------------------------*
  Name:         GetWcmSearchAroundCount

  Description:  �S�`�����l�������T�X�L������������Ԃ��B
                StartUp�����0�ɂȂ�B

  Arguments:    None.

  Returns:      ����X�L�����������B
 *---------------------------------------------------------------------------*/
u32 GetWcmSearchAroundCount(void)
{
    return gWcmSearchAroundCount;
}

/*---------------------------------------------------------------------------*
  Name:         ControlThread

  Description:  WCM ���C�u�����̏�ԊǗ����s���X���b�h�B
                gWcmTerminateFlag �ϐ��� FALSE �̊Ԃ͎w�� AP �ɐڑ����悤�Ƃ��A
                �܂��ڑ�������͂��̐ڑ����ێ����悤�Ƃ���B
                TerminateWcmControl �֐����Ă΂��� gWcmTerminateFlag �ϐ���
                TRUE �ɕω����A���̌�� WCM ���C�u�������I�����Ď��g�̃X���b�h
                ���I�����悤�Ƃ���B
                �Ȃ��AWCM ���C�u���������� FATAL ERROR �����������ꍇ�ɂ́A
                ���X���b�h�͓����Ŗ������[�v�ɓ���B

  Arguments:    arg     -   �g�p���Ȃ��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ControlThread(void* arg)
{
#pragma unused(arg)

    void*       wcmBuf;
    void*       wcmAplistBuf;
    s32         wcmResult;
    OSMessage   tempMessage;

    // WCM ���C�u�������g�p���郁�������m��
    wcmBuf = OS_Alloc(WCM_WORK_SIZE + LOCAL_WCM_APLIST_BUF_SIZE);

    if (wcmBuf == NULL)
    {
        /* ���������m�ۂł��Ȃ������ꍇ�͂��̌�̑���͕s�\�Ȃ̂ŏI�� */
        return;
    }

    wcmAplistBuf = (void*)((u32) wcmBuf + WCM_WORK_SIZE);

    // WCM ���C�u������������
    wcmResult = WCM_Init(wcmBuf, WCM_WORK_SIZE);
    DebugPrintf("WCM_Init() -> %s\n", gWcmResultText[wcmResult]);
    if (wcmResult != WCM_RESULT_SUCCESS)
    {
        /*
         * �����ŃG���[���Ԃ����ꍇ�́A���C�u�����̎g�������Ԉ���Ă���\���������B
         * ���̃X���b�h���� WCM ���C�u���������ɏ���������Ă��邱�Ƃ��l������B
         */
        OS_Free(wcmBuf);
        return;
    }

    // WCM ���C�u�����̓���ݒ�
    gWcmConfig.dmano = LOCAL_WCM_DMA_NO;
    gWcmConfig.pbdbuffer = wcmAplistBuf;
    gWcmConfig.nbdbuffer = LOCAL_WCM_APLIST_BUF_SIZE;
    gWcmConfig.nbdmode = WCM_APLIST_MODE_IGNORE;

    // �ڑ��J�n
    while (gWcmTerminateFlag == FALSE)
    {
        switch (WCM_GetPhase())
        {
        case WCM_PHASE_WAIT:
            // �����@�\�̋N��
            wcmResult = CallFunction(WCM_NOTIFY_STARTUP);
            break;

        case WCM_PHASE_IDLE:
            if (gWcmBssDesc.length == 0)
            {
                // AP �̎����T�����J�n
                WCM_ClearApList();
                wcmResult = CallFunction(WCM_NOTIFY_BEGIN_SEARCH);
            }
            else
            {
                // AP �ɐڑ�
                wcmResult = CallFunction(WCM_NOTIFY_CONNECT);
                if (wcmResult == WCM_RESULT_SUCCESS)
                {
                    // ���ɐڑ����悤�Ƃ���ۂɃT�[�`�����蒼���ׂɁA�����ŃN���A
                    MI_CpuClearFast(&gWcmBssDesc, sizeof(WMBssDesc));
                }
            }
            break;

        case WCM_PHASE_SEARCH:
            // AP �����������܂ő҂�
            (void)OS_ReceiveMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_BLOCK);

            /*
             * ���b�Z�[�W�Ƃ��ẮAWCM �̒�~�v���AAP �����ʒm�A�y�уG���[�ʒm�̂R��ނ�
             * �󂯎��\��������BAP �����ʒm�̏ꍇ�́A���̃T���v���ł͔������� AP �����Ȃ킿
             * �ڑ����ׂ� AP �Ȃ̂ŁA����ޔ�����B
             */
            if (tempMessage == LOCAL_WCM_MSG_FOUND_AP)
            {
                /* �����ڑ����[�h�Ȃ炱���Ō����I�� */
                if (!gWcmManualConnect)
                {
                    (void)WCM_LockApList(WCM_APLIST_LOCK);
                    if (WCM_CountApList() >= 1)
                    {
                        // �ŏ��Ɍ������� AP �̏��݂̂�ޔ�����B
                        MI_CpuCopyFast(WCM_PointApList(0), &gWcmBssDesc, WCM_APLIST_SIZE);
                        DebugPrintf("  BSS-ID: %02x-%02x-%02x-%02x-%02x-%02x\n", gWcmBssDesc.bssid[0],
                                    gWcmBssDesc.bssid[1], gWcmBssDesc.bssid[2], gWcmBssDesc.bssid[3],
                                    gWcmBssDesc.bssid[4], gWcmBssDesc.bssid[5]);
                        DebugPrintf("  ESS-ID: %s\n", gWcmBssDesc.ssid);
                    }
                    (void)WCM_LockApList(WCM_APLIST_UNLOCK);
                    wcmResult = CallFunction(WCM_NOTIFY_END_SEARCH);
                }
            }

            /* �ڑ��v��������΂܂��������I�� */
            else if (tempMessage == LOCAL_WCM_MSG_CONNECT_AP)
            {
                wcmResult = CallFunction(WCM_NOTIFY_END_SEARCH);
            }
            break;

        case WCM_PHASE_DCF:
            // �ؒf�ʒm�܂ő҂�
            (void)OS_ReceiveMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_BLOCK);

            /*
             * ���b�Z�[�W�Ƃ��ẮAWCM �̒�~�v���AAP ����̐ؒf�A�y�уG���[�ʒm�̂R��ނ�
             * �󂯎��\��������B���̂ǂ�̏ꍇ���󂯎�������_�ōs���ׂ������͖����B
             */
            if (tempMessage == LOCAL_WCM_MSG_DISCONNECT)
            {
                wcmResult = CallFunction(WCM_NOTIFY_DISCONNECT);
            }
            break;

        case WCM_PHASE_IRREGULAR:
            // FIFO ����ꂽ�\���������̂ŁA�����҂�
            OS_Sleep(1);

            // IRREGULAR �t�F�[�Y���ɗL���ȏ����͋�����~�̂�
            wcmResult = CallFunction(WCM_NOTIFY_TERMINATE);
            break;

        case WCM_PHASE_FATAL_ERROR:
        default:
            FreezeThread();
        }
    }

    // �I������
    while (gWcmTerminateFlag != FALSE)
    {
        switch (WCM_GetPhase())
        {
        case WCM_PHASE_WAIT:
            // ���[�v���甲����
            gWcmTerminateFlag = FALSE;
            break;

        case WCM_PHASE_IDLE:
            // �����@�\�̒�~
            wcmResult = CallFunction(WCM_NOTIFY_CLEANUP);
            break;

        case WCM_PHASE_SEARCH:
            // AP �̎����T�����~
            wcmResult = CallFunction(WCM_NOTIFY_END_SEARCH);
            break;

        case WCM_PHASE_DCF:
            // AP ����ؒf
            wcmResult = CallFunction(WCM_NOTIFY_DISCONNECT);
            break;

        case WCM_PHASE_IRREGULAR:
            // �����@�\�̋����I��
            wcmResult = CallFunction(WCM_NOTIFY_TERMINATE);
            break;

        case WCM_PHASE_FATAL_ERROR:
        default:
            FreezeThread();
        }
    }

    // WCM ���C�u�������I��
    wcmResult = WCM_Finish();
    DebugPrintf("WCM_Finish() -> %s\n", gWcmResultText[wcmResult]);
    if (wcmResult != WCM_RESULT_SUCCESS)
    {
        /*
         * �����ŃG���[���Ԃ����ꍇ�́A���C�u�����̎g�������Ԉ���Ă���\���������B
         * ���̃X���b�h���� WCM ���C�u�������I������Ă��܂��Ă��邱�Ƃ��l������B
         */
        OS_Free(wcmBuf);
        return;
    }

    // �m�ۂ��Ă��������������
    OS_Free(wcmBuf);
    return;
}

/*---------------------------------------------------------------------------*
  Name:         FreezeThread

  Description:  �X���b�h����Ăяo����A�������[�v�ɓ����Čł܂�B

  Arguments:    None.

  Returns:      ���֐�����͕Ԃ�Ȃ��B
 *---------------------------------------------------------------------------*/
static void FreezeThread(void)
{
    /*
     * FATAL ERROR �̏ꍇ�͂ǂ����悤���Ȃ��̂ŁA���Ōł܂邱�Ƃɂ���B
     * �v���I�ȃ��������j�󂳂ꂽ�AARM7�̕��ׂ��������ď���������Ȃ�
     * �Ȃ����A�X���b�h�̃X�^�b�N����ꂽ�A���̌������l������B
     * �G���h���[�U�[�ɓK�؂ȃG���[�ʒm�����Ȃ���΂Ȃ�Ȃ����A�\�t�g�I��
     * ����������@�͖����̂ŁA�G���h���[�U�[�ɓd����؂��Ă��炤�����Ȃ��B
     */
    while (TRUE)
    {
        OS_Sleep(10000);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CallFunction

  Description:  WCM �񓯊� API ���R�[�����A�񓯊���������������܂ő҂B
                �X���b�h����Ăяo����邱�Ƃ�z�肵�Ă���A�񓯊������̊�����
                �����Ńu���b�N���đ҂_�ɒ��ӁB

  Arguments:    WCM �񓯊� API �̎�ʂ� WCM_NOTIFY_* �̌`���Ŏw�肷��B
                �w��\�ȃp�����[�^�͈ȉ��̒ʂ�B
                    WCM_NOTIFY_STARTUP      : WCM_StartupAsync �����s����B
                    WCM_NOTIFY_CLEANUP      : WCM_CleanupAsync �����s����B
                    WCM_NOTIFY_BEGIN_SEARCH : WCM_SearchAsycn(�J�n) �����s����B
                    WCM_NOTIFY_END_SEARCH   : WCM_SearchAsync(��~) �����s����B
                    WCM_NOTIFY_CONNECT      : WCM_ConnectAsync �����s����B
                    WCM_NOTIFY_DISCONNECT   : WCM_DisconnectAsync �����s����B
                    WCM_NOTIFY_TERMINATE    : WCM_TerminateAsync �����s����B

  Returns:      �����I�E�񓯊��I�ȏ������ʂ�Ԃ��B
                �Ԃ����l�͈ȉ��̓��̂����ꂩ�ƂȂ�B
                    WCM_RESULT_SUCCESS      : ����������Ɋ����B
                    WCM_RESULT_FAILURE      : �����̓r���łȂ�炩�̗v���Ŏ��s�B
                    WCM_RESULT_FATAL_ERROR  : �p�����[�^���z��͈͊O
 *---------------------------------------------------------------------------*/
static s32 CallFunction(s16 notifyId)
{
    s32         wcmResult = WCM_RESULT_FATAL_ERROR;
    OSIntrMode  e = OS_DisableInterrupts();

    while (TRUE)
    {
        // ���s���ׂ� API ��U�蕪��
        switch (notifyId)
        {
        case WCM_NOTIFY_STARTUP:
            wcmResult = WCM_StartupAsync(&gWcmConfig, ControlCallback);
            DebugPrintf("WCM_StartupAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_CLEANUP:
            wcmResult = WCM_CleanupAsync();
            DebugPrintf("WCM_CleanupAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_BEGIN_SEARCH:
            /****************************************************************/

            /*  �A�N�Z�X�|�C���g��T������ۂ̏d�v�Ȑݒ���s���Ă���ӏ�    */
            wcmResult = WCM_BeginSearchAsync((void*)constApInfoPtr->bssId, (void*)constApInfoPtr->essId, (u32) (
                                                 WCM_OPTION_CHANNEL_ALL | gWcmScanType |
                                                 (gWcmManualConnect ? WCM_OPTION_ROUNDSCAN_NOTIFY : WCM_OPTION_ROUNDSCAN_IGNORE)
                                             ));

            /****************************************************************/
            DebugPrintf("WCM_BeginSearchAsync -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_END_SEARCH:
            wcmResult = WCM_EndSearchAsync();
            DebugPrintf("WCM_EndSearchAsync -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_CONNECT:
            /****************************************************************/

            /*  �A�N�Z�X�|�C���g�ɐڑ�����ۂ̏d�v�Ȑݒ���s���Ă���ӏ�    */
            wcmResult = WCM_ConnectAsync(&gWcmBssDesc, (void*)(&(constApInfoPtr->wepDesc)),
                                         gWcmPowerMode | constApInfoPtr->auth_option);

            /****************************************************************/
            DebugPrintf("WCM_ConnectAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_DISCONNECT:
            wcmResult = WCM_DisconnectAsync();
            DebugPrintf("WCM_DisconnectAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_TERMINATE:
            wcmResult = WCM_TerminateAsync();
            DebugPrintf("WCM_TerminateAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        default:
            (void)OS_RestoreInterrupts(e);
            return wcmResult;
        }

        // �����I�ȏ������ʂ��m�F
        switch (wcmResult)
        {
        case WCM_RESULT_REJECT:
            // �����҂��Ă��烊�g���C����
            OS_Sleep(1);
            continue;

        case WCM_RESULT_PROGRESS:
        case WCM_RESULT_ACCEPT:
            // �񓯊������������R�[���o�b�N�����܂ő҂�
            OS_SleepThread(&gWcmThreadQ);
            break;

        case WCM_RESULT_SUCCESS:
        case WCM_RESULT_FAILURE:
            // �����I�ɏ������������A�񓯊������͍s���Ȃ��ꍇ
            (void)OS_RestoreInterrupts(e);
            return wcmResult;

        case WCM_RESULT_WMDISABLE:
            /*
             * ARM7 ���� WM ���C�u���������삵�Ă��Ȃ��ꍇ�B
             * WCM ���g���n�߂�O�� WVR ���C�u�������g���� ARM7 �������h���C�o��
             * �N��������K�v�����邪�A����̓A�v���P�[�V�����̐ӔC�ōs���ׂ����ƁB
             */
            OS_Panic("ARM7 WM library is not ready.\n");

        case WCM_RESULT_NOT_ENOUGH_MEM: // �񓯊��֐��ɂ��̕Ԃ�l���Ԃ���邱�Ƃ͖���
        case WCM_RESULT_FATAL_ERROR:
        default:
            (void)OS_RestoreInterrupts(e);
            FreezeThread();
        }

        // �񓯊��I�ȏ������ʂ��m�F
        switch (gWcmResult)
        {
        case WCM_RESULT_SUCCESS:
        case WCM_RESULT_FAILURE:
            wcmResult = gWcmResult;
            break;

        case WCM_RESULT_PROGRESS:       // �񓯊��������ʂɂ��̏������ʂ��n����邱�Ƃ͖���
        case WCM_RESULT_ACCEPT:         // �񓯊��������ʂɂ��̏������ʂ��n����邱�Ƃ͖���
        case WCM_RESULT_REJECT:         // �񓯊��������ʂɂ��̏������ʂ��n����邱�Ƃ͖���
        case WCM_RESULT_WMDISABLE:      // �񓯊��������ʂɂ��̏������ʂ��n����邱�Ƃ͖���
        case WCM_RESULT_NOT_ENOUGH_MEM: // �񓯊��������ʂɂ��̏������ʂ��n����邱�Ƃ͖���
        case WCM_RESULT_FATAL_ERROR:
        default:
            (void)OS_RestoreInterrupts(e);
            FreezeThread();
        }

        break;
    }

    (void)OS_RestoreInterrupts(e);
    return wcmResult;
}

/*---------------------------------------------------------------------------*
  Name:         ControlCallback

  Description:  WCM �񓯊� API �̏������ʉ����A�y�� WCM ���C�u��������� �s���
                �ʒm���󂯎��R�[���o�b�N�֐��B

  Arguments:    arg     -   �ʒm��ʂ⏈�����ʂȂǂ������\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ControlCallback(WCMNotice* arg)
{
    static s16  lastNotify = -1;
    static s16  lastResult = -1;

    //�O��ƌ��ʂ��������\��
    if (lastNotify != arg->notify || lastResult != arg->result)
    {
        DebugPrintf(" cb %s %s\n", gWcmNotifyText[arg->notify], gWcmResultText[arg->result]);
        lastNotify = arg->notify;
        lastResult = arg->result;
    }

    switch (arg->notify)
    {
    case WCM_NOTIFY_STARTUP:
        gWcmSearchAroundCount = 0;

    case WCM_NOTIFY_CLEANUP:
    case WCM_NOTIFY_BEGIN_SEARCH:
    case WCM_NOTIFY_END_SEARCH:
    case WCM_NOTIFY_CONNECT:
    case WCM_NOTIFY_TERMINATE:
        // �v���� 1 �� 1 �ɑΉ�����񓯊������̌��ʉ���
        gWcmResult = arg->result;
        OS_WakeupThread(&gWcmThreadQ);
        break;

    case WCM_NOTIFY_DISCONNECT:
#ifdef SDK_THREAD_INFINITY
        if (gWcmThreadQ.head != NULL)
#else
            if (gWcmThreadQ != 0)
#endif
            {
                // �v���� 1 �� 1 �ɑΉ�����񓯊������̌��ʉ���
                gWcmResult = arg->result;
                OS_WakeupThread(&gWcmThreadQ);
            }
            else
            {
                // �X���b�h���ғ����A�������̓��b�Z�[�W�҂��Œ�~��Ԃ̏ꍇ
                if (arg->result == WCM_RESULT_SUCCESS)
                {
                    // DCF �ʐM���� AP ����ؒf���ꂽ���Ƃ������ʒm
                    (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_DISCONNECTED, OS_MESSAGE_NOBLOCK);
                }
                else
                {
                    // DCF �ʐM���ɂȂ�炩�̃G���[�������������Ƃ������ʒm
                    (void)OS_JamMessage(&gWcmMessageQ, LOCAL_WCM_MSG_ERROR, OS_MESSAGE_NOBLOCK);
                }
            }
        break;

    case WCM_NOTIFY_FOUND_AP:
        // �����T�����̔񓯊��ʒm
        {
            if (arg->result == WCM_RESULT_SUCCESS)
            {
                OSMessage   tempMessage;

                // �����T������ AP �𔭌��������Ƃ������ʒm
                if (OS_ReadMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_NOBLOCK) == FALSE)
                {
                    // ���x���ʒm�������̂Ȃ̂ŁA���b�Z�[�W�L���[����ł���ꍇ�݂̂Ɍ���
                    (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_FOUND_AP, OS_MESSAGE_NOBLOCK);
                }
            }
            else
            {
                // �����T�����ɂȂ�炩�̃G���[�������������Ƃ������ʒm
                (void)OS_JamMessage(&gWcmMessageQ, LOCAL_WCM_MSG_ERROR, OS_MESSAGE_NOBLOCK);
            }
        }
        break;

    case WCM_NOTIFY_SEARCH_AROUND:
        // �����T�����Ɏw��`�����l�����ꏄ�������Ƃ������s����ʒm
        ++gWcmSearchAroundCount;
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         InitWcmApInfo

  Description:  �A�N�Z�X�|�C���g�ڑ��p�����[�^�����ݒ�l����擾����
                ����������

  Arguments:    apinfo   �ڑ��p�����[�^
                apclass  �A�N�Z�X�|�C���g�̃N���X��

  Returns:      TRUE   ����
                FALSE  ���s
 *---------------------------------------------------------------------------*/
static BOOL myEnvCopy(const char* name, void* dest, int dest_size);
static BOOL myEnvCopy(const char* name, void* dest, int dest_size)
{
    void*   src;
    int     src_size;

    if (ENV_GetBinaryAndSize(name, &src, &src_size))
    {
        MI_CpuCopy8(src, dest, (u32) MATH_MIN(src_size, dest_size));
        return TRUE;
    }

    return FALSE;
}

BOOL InitWcmApInfo(WcmControlApInfo* apinfo, const char* apclass)
{
    BOOL    isvalid;

    ENV_SetClass(apclass);

    if (ENV_GetBOOL(".ISVALID", &isvalid) && isvalid)
    {
        MI_CpuClear8(apinfo, sizeof(WcmControlApInfo));

        if (myEnvCopy(".ESSID", apinfo->essId, sizeof(apinfo->essId)) &&
            myEnvCopy(".BSSID", apinfo->bssId, sizeof(apinfo->bssId)) && ENV_GetU32(".AUTHMODE", &apinfo->auth_option) &&
            ENV_GetU8(".WEP.MODE", &apinfo->wepDesc.mode) && ENV_GetU8(".WEP.KEYID", &apinfo->wepDesc.keyId) &&
            myEnvCopy(".WEP.KEY", apinfo->wepDesc.key, sizeof(apinfo->wepDesc.key)))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

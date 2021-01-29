/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     wcm_control.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_control.c,v $
  Revision 1.3  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.1  2005/07/21 11:47:03  seiki_masashi
  ssl/ssl-1 �Ɉړ�

  Revision 1.1  2005/07/18 08:10:26  seiki_masashi
  ssl-1 �f���̒ǉ�

  Revision 1.1  2005/07/14 10:15:49  jey-s
  *** empty log message ***

  Revision 1.2  2005/07/12 04:17:50  terui
  �Œ�� WEP �ݒ�̕\�L������
  �F�ؕ�����ݒ�ɏ]���Đ؂�ւ���悤����

  Revision 1.1  2005/07/11 12:07:09  terui
  �V�K�ǉ��B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitro/os.h>
#include <nitroWiFi.h>
#include "wcm_control.h"
#include "screen.h"

/*---------------------------------------------------------------------------*
    AP ���

    NOTICE: �����̒�`���A�ڑ����� AP �ɍ��킹�ĕҏW���ĉ������B
            �Ȃ��A���T���v���ł� BSSID�AESSID�A802.11 �F�ؕ����AWEP�Í����ݒ肪
            �������Ă��� AP �֎����I�ɐڑ����܂��B
 *---------------------------------------------------------------------------*/
static const u8*        gWcmBssid = WCM_BSSID_ANY;
static const char       gWcmEssid[] = { "000D0B14ABD2" };
static const u32        gWcmAuthOption = WCM_OPTION_AUTH_OPENSYSTEM;

#define LOCAL_WCM_WEP_MODE  WCM_WEPMODE_NONE

static const WCMWepDesc gWcmWepDesc =
{
    LOCAL_WCM_WEP_MODE ,                        // �Í������[�h
    0,                                          // ���M���Ɏg�p���� WEP �L�[ ID ( 0 �` 3 )
#if ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_40 )
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
#elif ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_104 )
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
#elif ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_128 )
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00
    }
#else /* ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_NONE ) */
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    }
#endif
};

/*---------------------------------------------------------------------------*
    �萔 ��`
 *---------------------------------------------------------------------------*/
#define LOCAL_WCM_THREAD_STACK_SIZE     1024    // ( �ύX�� ) �傫�߂ɃX�^�b�N���m��
#define LOCAL_WCM_THREAD_PRIORITY       24      // ( �ύX�� ) ��߂̗D��x�ŏ\��
#define LOCAL_WCM_DMA_NO                3       // ( �ύX�� ) �����@�\�Ɏg�p���� DMA �ԍ�
#define LOCAL_WCM_APLIST_COUNT_MAX      1       // WCM ���C�u�������ŊǗ������� AP ��񃊃X�g�̐�
#define LOCAL_WCM_APLIST_BUF_SIZE       ((LOCAL_WCM_APLIST_COUNT_MAX * WCM_APLIST_BLOCK_SIZE) + 12)
#define LOCAL_WCM_THREAD_MSGQ_CAPACITY  2       // ���b�Z�[�W�L���[�̒i��
#define LOCAL_WCM_MSG_TERMINATE         ((void*)0)
#define LOCAL_WCM_MSG_FOUND_AP          ((void*)1)
#define LOCAL_WCM_MSG_DISCONNECTED      ((void*)2)

// WCM ���C�u�������샍�O�\���p ������萔
static const char*      gWcmResultText[8] =
{
    { "SUCCESS" } ,         { "FAILURE" } ,
    { "PROGRESS" } ,        { "ACCEPT" } ,
    { "REJECT" } ,          { "WMDISABLE" } ,
    { "NOT_ENOUGH_MEM" } ,  { "FATAL_ERROR" }
};
static const char*      gWcmNotifyText[10] =
{
    { "COMMON" } ,          { "STARTUP" } ,
    { "CLEANUP" } ,         { "BEGIN_SEARCH" } ,
    { "END_SEARCH" } ,      { "CONNECT" } ,
    { "DISCONNECT" } ,      { "FOUND_AP" } ,
    { "SEARCH_AROUND" } ,   { "TERMINATE" }
};

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

/*---------------------------------------------------------------------------*
    �����֐� �v���g�^�C�v
 *---------------------------------------------------------------------------*/
static void ControlThread(void* arg);
static void FreezeThread(void);
static s32  CallFunction(s16 notifyId);
static void ControlCallback(WCMNotice* arg);

/*---------------------------------------------------------------------------*
  Name:         InitWcmControl

  Description:  �����ϐ������������AWCM �R���g���[���X���b�h���N������B
                ���ɃX���b�h���N�����Ă���ꍇ�͉����s��Ȃ��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitWcmControl(void)
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

    // ���b�Z�[�W�L���[��������
    OS_InitMessageQueue(&gWcmMessageQ, gWcmMsgArray, LOCAL_WCM_THREAD_MSGQ_CAPACITY);

    // �X���b�h�L���[��������
    OS_InitThreadQueue(&gWcmThreadQ);

    // �ϐ�������
    MI_CpuClearFast(&gWcmBssDesc, sizeof(WMBssDesc));

    // �X���b�h�ϐ����g���ĐV�K�ɃX���b�h���쐬
    OS_CreateThread(&gWcmThread, ControlThread, NULL, (void*)(gWcmStack + LOCAL_WCM_THREAD_STACK_SIZE),
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
    if (gWcmThread.id != 0)
    {
        if (OS_IsThreadTerminated(&gWcmThread) == FALSE)
        {
            if (gWcmTerminateFlag == FALSE)
            {
                gWcmTerminateFlag = TRUE;
                (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_TERMINATE, OS_MESSAGE_NOBLOCK);
            }
        }
    }
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
    PrintSubScreen("WCM_Init() -> %s\n", gWcmResultText[wcmResult]);
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
             * ���b�Z�[�W�Ƃ��ẮAWCM �̒�~�v���A�y�� AP �����ʒm�̂Q��ނ̂ݎ󂯎��
             * �\��������BAP �����ʒm�̏ꍇ�́A���̃T���v���ł͔������� AP �����Ȃ킿
             * �ڑ����ׂ� AP �Ȃ̂ŁA����ޔ�����B
             */
            if (tempMessage == LOCAL_WCM_MSG_FOUND_AP)
            {
                (void)WCM_LockApList(WCM_APLIST_LOCK);
                if (WCM_CountApList() == 1)
                {
                    // BSSID �Œ�Ȃ̂� ���������\���̂��� AP �͂P�����̂͂�
                    MI_CpuCopyFast(WCM_PointApList(0), &gWcmBssDesc, WCM_APLIST_SIZE);
                }
                (void)WCM_LockApList(WCM_APLIST_UNLOCK);
                wcmResult = CallFunction(WCM_NOTIFY_END_SEARCH);
            }
            break;

        case WCM_PHASE_DCF:
            // �ؒf�ʒm�܂ő҂�
            (void)OS_ReceiveMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_BLOCK);

            /*
             * ���b�Z�[�W�Ƃ��ẮAWCM �̒�~�v���A�y�� AP ����̐ؒf�̂Q��ނ̂ݎ󂯎��
             * �\��������B���̂ǂ���̏ꍇ���󂯎�������_�ōs���ׂ������͖����B
             */
            break;

        case WCM_PHASE_IRREGULAR:
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
    PrintSubScreen("WCM_Finish() -> %s\n", gWcmResultText[wcmResult]);
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
    s32 wcmResult = WCM_RESULT_FATAL_ERROR;

    while (TRUE)
    {
        // ���s���ׂ� API ��U�蕪��
        switch (notifyId)
        {
        case WCM_NOTIFY_STARTUP:
            wcmResult = WCM_StartupAsync(&gWcmConfig, ControlCallback);
            PrintSubScreen("WCM_StartupAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_CLEANUP:
            wcmResult = WCM_CleanupAsync();
            PrintSubScreen("WCM_CleanupAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_BEGIN_SEARCH:
            /****************************************************************/

            /*  �A�N�Z�X�|�C���g��T������ۂ̏d�v�Ȑݒ���s���Ă���ӏ�    */
            wcmResult = WCM_SearchAsync((void*)gWcmBssid, (void*)gWcmEssid,
                                        WCM_OPTION_CHANNEL_ALL | WCM_OPTION_SCANTYPE_ACTIVE | WCM_OPTION_ROUNDSCAN_IGNORE);

            /****************************************************************/
            PrintSubScreen("WCM_SearchAsync(S) -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_END_SEARCH:
            wcmResult = WCM_SearchAsync(NULL, NULL, 0);
            PrintSubScreen("WCM_SearchAsync(E) -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_CONNECT:
            /****************************************************************/

            /*  �A�N�Z�X�|�C���g�ɐڑ�����ۂ̏d�v�Ȑݒ���s���Ă���ӏ�    */
            wcmResult = WCM_ConnectAsync(&gWcmBssDesc, (void*)(&gWcmWepDesc), WCM_OPTION_POWER_ACTIVE | gWcmAuthOption);

            /****************************************************************/
            PrintSubScreen("WCM_ConnectAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_DISCONNECT:
            wcmResult = WCM_DisconnectAsync();
            PrintSubScreen("WCM_DisconnectAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_TERMINATE:
            wcmResult = WCM_TerminateAsync();
            PrintSubScreen("WCM_TerminateAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        default:
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
            FreezeThread();
        }

        break;
    }

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
    PrintSubScreen(" cb %s %s\n", gWcmNotifyText[arg->notify], gWcmResultText[arg->result]);

    switch (arg->notify)
    {
    case WCM_NOTIFY_STARTUP:
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
        if (arg->parameter[1].n == 0)
        {
            // �����I�Ȑؒf�v���� 1 �� 1 �ɑΉ����錋�ʉ���
            gWcmResult = arg->result;
            OS_WakeupThread(&gWcmThreadQ);
        }
        else
        {
            // AP ����ؒf���ꂽ���Ƃ������s����ʒm
            (void)OS_JamMessage(&gWcmMessageQ, LOCAL_WCM_MSG_DISCONNECTED, OS_MESSAGE_NOBLOCK);
        }
        break;

    case WCM_NOTIFY_FOUND_AP:
        // �����T������ AP �𔭌��������Ƃ������s����ʒm
        {
            OSMessage   tempMessage;

            // ���x���ʒm�������̂Ȃ̂ŁA���b�Z�[�W�L���[����ł���ꍇ�̂ݒʒm����
            if (OS_ReadMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_NOBLOCK) == FALSE)
            {
                (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_FOUND_AP, OS_MESSAGE_NOBLOCK);
            }
        }
        break;

    case WCM_NOTIFY_SEARCH_AROUND:
        // �����T�����Ɏw��`�����l�����ꏄ�������Ƃ������s����ʒm

        /*
         * ���̃T���v���v���O�����ł͔������邱�Ƃ͂��蓾�Ȃ��̂ŁA��������B
         */
        break;
    }
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

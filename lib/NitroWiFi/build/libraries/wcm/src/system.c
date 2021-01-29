/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - libraries
  File:     system.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: system.c,v $
  Revision 1.17  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.16  2005/11/08 01:08:02  seiki_masashi
  NITRO-SDK 3.0RC �ł̕ύX�ɑΉ�

  Revision 1.15  2005/11/01 12:08:26  seiki_masashi
  SDK_ASSERT_ON_COMPILE ����`����Ă����ꍇ�ɑΏ�

  Revision 1.14  2005/10/05 08:49:50  terui
  DCF�f�[�^��M�̓s�x�A�d�g���x��ޔ�����@�\��ǉ�

  Revision 1.13  2005/09/17 10:09:08  terui
  CAM���C�t�^�C���Ń^�C���A�E�g����@�\��ǉ��B

  Revision 1.12  2005/09/16 04:50:10  terui
  WCM_ConnectAsync�֐��ɂāAWEP���[�h���Í��������̏ꍇ�ɂ�WEP�L�[�f�[�^��S��0�̃f�[�^�ŋU������悤�΍�

  Revision 1.11  2005/09/16 04:23:14  terui
  ���[�g�Z�b�g�̕s�����ɂ��ڑ��Ɏ��s�������̃��g���C���ɏ�Ԃ��ꎞIDLE�ɖ߂��Ă��܂��s����C��

  Revision 1.10  2005/09/12 10:02:13  terui
  ���[�g�Z�b�g�̕s�����ɂ��ڑ��Ɏ��s�������̑΍��ǉ�

  Revision 1.9  2005/09/10 03:08:31  terui
  �����ʐM���֎~����Ă���{�̂̃`�F�b�N��ǉ��B
  ���[�e�B���e�B�֐���ʃt�@�C���֐؂�o���B

  Revision 1.8  2005/09/01 13:08:14  terui
  �A���[���ɂ���ă^�C�~���O�����AKeep Alive �p�P�b�g�𑗐M����@�\��ǉ�
  Connect ���s���AWM ����Ԃ���� wlStatus �t�B�[���h����ʂɓ`����@�\��ǉ�

  Revision 1.7  2005/08/09 07:58:30  terui
  WEP�L�[��ޔ���������o�b�t�@��32�o�C�g�A���C�������ʒu�ɒ���

  Revision 1.6  2005/08/08 06:13:42  terui
  AP�ɐڑ�����ۂ̃T�|�[�g���[�g�Z�b�g��1M/2M���Œ�Œǉ�����悤����

  Revision 1.5  2005/07/18 01:34:49  terui
  DCF �ʐM���ɐؒf���ꂽ�ꍇ�̒ʒm��ʂ̐ݒ�ʒu��ύX

  Revision 1.4  2005/07/15 11:32:10  terui
  WCM_SearchAsync �֐����J�n�֐��ƒ�~�֐��Q�ɕ���������B

  Revision 1.3  2005/07/12 06:25:30  terui
  WCM_SetChannelScanTime �֐���ǉ�

  Revision 1.2  2005/07/11 23:44:29  terui
  Fix comment.

  Revision 1.1  2005/07/07 10:45:52  terui
  �V�K�ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "wcm_private.h"
#include "wcm_cpsif.h"

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/

// �S�� 0xff �� ESSID
const u8    WCM_Essid_Any[WCM_ESSID_SIZE] =
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// �S�� 0xff �� BSSID
const u8    WCM_Bssid_Any[WCM_BSSID_SIZE] =
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

#if WCM_DEBUG

// �x�����p�e�L�X�g���`
static const char   wcmWarningText_AlreadyInit[] = { "WCM library is already initialized.\n" };
static const char   wcmWarningText_IllegalParam[] = { "Illegal parameter ( %s )\n" };
static const char   wcmWarningText_NotInit[] = { "WCM library is not initialized yet.\n" };
static const char   wcmWarningText_IllegalPhase[] = { "Could not accept request now. ( PHASE: %d )\n" };
static const char   wcmWarningText_InvalidWmState[] = { "Invalid state of WM library. Don't use WM or WVR library while using WCM library.\n" };
static const char   wcmWarningText_UnsuitableArm7[] = { "Unsuitable ARM7 component. Could not drive wireless module.\n" };
static const char   wcmWarningText_WirelessForbidden[] = { "Forbidden to use wireless module." };

// �񍐕��p�e�L�X�g���`
static const char   wcmReportText_WmSyncError[] = { "%s failed syncronously. ( ERRCODE: 0x%02x )\n" };
static const char   wcmReportText_WmAsyncError[] = { "%s failed asyncronously. ( ERRCODE: 0x%02x - 0x%04x - 0x%04x )\n" };
static const char   wcmReportText_WmDisconnected[] = { "%s succeeded asyncronously , but already disconnected from AP.\n" };
static const char   wcmReportText_InvalidAid[] = { "%s succeeded asyncronously , but given Auth-ID is invalid.\n" };
static const char   wcmReportText_SupportRateset[] = { "Failed association to \"%s\" because of illegal support rate set.\n  So retry with camouflage 5.5M and 11M automatically.\n" };
#endif

/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static WCMWork*     wcmw = NULL;

/*---------------------------------------------------------------------------*
    �����֐��v���g�^�C�v
 *---------------------------------------------------------------------------*/
static void         WcmConfigure(WCMConfig* config, WCMNotify notify);
static void         WcmEditScanExParam(void* bssid, void* essid, u32 option);
static void         WcmInitOption(void);
static u16          WcmGetNextScanChannel(u16 channel);
static void         WcmNotify(s16 result, void* para0, void* para1, s32 para2);
static void         WcmNotifyEx(s16 notify, s16 result, void* para0, void* para1, s32 para2);
static void         WcmSetPhase(u32 phase);
static void         WcmKeepAliveAlarm(void* arg);
static u32          WcmCountBits(u32 arg);
static u32          WcmCountLeadingZero(u32 arg);

static void         WcmWmReset(void);

static void         WcmWmcbIndication(void* arg);
static void         WcmWmcbCommon(void* arg);
static void         WcmWmcbScanEx(void* arg);
static void         WcmWmcbEndScan(void* arg);
static void         WcmWmcbConnect(void* arg);
static void         WcmWmcbDisconnect(void* arg);
static void         WcmWmcbStartDCF(void* arg);
static void         WcmWmcbEndDCF(void* arg);
static void         WcmWmcbReset(void* arg);

/* �R���p�C�����A�T�[�V���� */
#ifndef SDK_ASSERT_ON_COMPILE
#define SDK_ASSERT_ON_COMPILE(expr) extern assert_on_compile ## #__LINE__ (char a[(expr) ? 1 : -1])
#endif

/*---------------------------------------------------------------------------*
  Name:         WCM_Init

  Description:  WCM ���C�u����������������B�����֐��B

  Arguments:    buf     -   WCM ���C�u�����������I�Ɏg�p���郏�[�N�o�b�t�@�ւ�
                            �|�C���^���w�肷��B������������́AWCM_Finish �֐�
                            �ɂ���� WCM ���C�u�������I������܂ŕʂ̗p�r�ɗp��
                            �Ă͂����Ȃ��B
                len     -   ���[�N�o�b�t�@�̃T�C�Y���w�肷��BWCM_WORK_SIZE ���
                            �������T�C�Y���w�肷��Ƃ��̊֐��͎��s����B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̏������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_NOT_ENOUGH_MEM
 *---------------------------------------------------------------------------*/
s32 WCM_Init(void* buf, s32 len)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // WCM_WORK_SIZE �萔�̑Ó������R���p�C�����Ɋm�F
    SDK_ASSERT_ON_COMPILE(sizeof(WCMWork) <= WCM_WORK_SIZE);

    // �����������m�F
    if (wcmw != NULL)
    {
        WCMi_Warning(wcmWarningText_AlreadyInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;          // ���ɏ������ς�
    }

    // �p�����[�^���m�F
    if (buf == NULL)
    {
        WCMi_Warning(wcmWarningText_IllegalParam, "buf");
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;          // �w��o�b�t�@�� NULL
    }

    if ((u32) buf & 0x01f)
    {
        WCMi_Warning(wcmWarningText_IllegalParam, "buf");
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;          // �w��o�b�t�@�� 32 �o�C�g�A���C���łȂ�
    }

    if (len < sizeof(WCMWork))
    {
        WCMi_Warning(wcmWarningText_IllegalParam, "len");
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_NOT_ENOUGH_MEM;   // �w��o�b�t�@�̃T�C�Y������Ȃ�
    }

    // �e���[�N�ϐ���������
    wcmw = (WCMWork*)buf;
    wcmw->phase = WCM_PHASE_WAIT;           // �����t�F�[�Y��������
    wcmw->notifyId = WCM_NOTIFY_COMMON;     // �񓯊� API �ʒm ID ��������
    wcmw->maxScanTime = 0;                  // �Œ�X�L�������Ԃ�������
    wcmw->apListLock = WCM_APLIST_UNLOCK;   // AP ���ێ����X�g�̃��b�N��Ԃ�������
    wcmw->resetting = WCM_RESETTING_OFF;    // ���Z�b�g�d���Ăяo���Ǘ��t���O��������
    wcmw->authId = 0;                       // Auth-ID ��������
    wcmw->wlStatusOnConnectFail = 0x0000;
    WcmInitOption();                        // �I�v�V�����ݒ��������
    WCMi_InitCpsif();                       // CPS �C���^�[�t�F�[�X��������

    // �A���[����������
    if (!OS_IsTickAvailable())
    {
        OS_InitTick();
    }

    if (!OS_IsAlarmAvailable())
    {
        OS_InitAlarm();
    }

    OS_CreateAlarm(&(wcmw->keepAliveAlarm));

    // ����I��
    (void)OS_RestoreInterrupts(e);
    return WCM_RESULT_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_Finish

  Description:  WCM���C�u�������I������B�����֐��BWCM_Init�֐��ɂ���Ďw�肵��
                ���[�N�p�o�b�t�@�͉�������B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̏������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE
 *---------------------------------------------------------------------------*/
s32 WCM_Finish(void)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // �������ς݂��m�F
    if (wcmw == NULL)
    {
        WCMi_Warning(wcmWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;  // ����������Ă��Ȃ�
    }

    // �����t�F�[�Y�m�F
    if (wcmw->phase != WCM_PHASE_WAIT)
    {
        WCMi_Warning(wcmWarningText_IllegalPhase, wcmw->phase);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;  // WAIT �t�F�[�Y�łȂ�
    }

    // ���[�N�o�b�t�@�����
    wcmw = NULL;

    // ����I��
    (void)OS_RestoreInterrupts(e);
    return WCM_RESULT_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_StartupAsync

  Description:  �����@�\�̋N���V�[�P���X���J�n����B
                �����I�ɂ� WAIT ���� IDLE �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� notify �Ŏw�肵���R�[���o�b�N�֐��ɂ��񓯊��I��
                �������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_STARTUP �ƂȂ�B

  Arguments:    config  -   WCM �̓���ݒ�ƂȂ�\���̂ւ̃|�C���^���w��B
                notify  -   �񓯊��I�ȏ������ʂ�ʒm����R�[���o�b�N�֐����w��B
                            ���̃R�[���o�b�N�֐��͈Ȍ�̔񓯊��I�Ȋ֐��̌���
                            �ʒm�ɋ��ʂ��Ďg�p�����B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_WMDISABLE ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32 WCM_StartupAsync(WCMConfig* config, WCMNotify notify)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // �������ς݂��m�F
    if (wcmw == NULL)
    {
        WCMi_Warning(wcmWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;                  // ����������Ă��Ȃ�
    }

    // �����t�F�[�Y�m�F
    switch (wcmw->phase)
    {
    case WCM_PHASE_WAIT:
        WcmConfigure(config, notify);
        break;

    case WCM_PHASE_WAIT_TO_IDLE:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_PROGRESS;                 // ���ɓ������N�G�X�g�̔񓯊�������

    case WCM_PHASE_IDLE:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_SUCCESS;                  // ��ԑJ�ږڕW�t�F�[�Y�Ɉڍs�ς�

    default:
        WCMi_Warning(wcmWarningText_IllegalPhase, wcmw->phase);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;                  // �v�����󂯕t�����Ȃ��t�F�[�Y
    }

    // �����@�\�̋N���V�[�P���X���J�n
    {
        WMErrCode   wmResult;

        // WM ���C�u����������
        wmResult = WM_Init(wcmw->wmWork, (u16) wcmw->config.dmano);
        switch (wmResult)
        {
        case WM_ERRCODE_SUCCESS:
            break;

        case WM_ERRCODE_ILLEGAL_STATE:
            WCMi_Warning(wcmWarningText_InvalidWmState);
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FATAL_ERROR;          // WM ���C�u���������ɏ���������Ă���

        case WM_ERRCODE_WM_DISABLE:
            WCMi_Warning(wcmWarningText_UnsuitableArm7);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_WMDISABLE;            // ARM7 ���� WM ���C�u���������삵�Ă��Ȃ�

        case WM_ERRCODE_INVALID_PARAM:
        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FATAL_ERROR;          // �z��͈͊O�̃G���[
        }

        // �����ʐM���֎~����Ă���{�̂łȂ����Ƃ��m�F
        if (0 == WM_GetAllowedChannel())
        {
            if (WM_ERRCODE_SUCCESS != WM_Finish())
            {
                WcmSetPhase(WCM_PHASE_FATAL_ERROR);
                (void)OS_RestoreInterrupts(e);
                return WCM_RESULT_FATAL_ERROR;      // �z��͈͊O�̃G���[
            }

            WCMi_Warning(wcmWarningText_WirelessForbidden);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_WMDISABLE;            // �����ʐM��������Ă��Ȃ�
        }

        // �s����� ARM7 ����̒ʒm���󂯎��x�N�g����ݒ�
        wmResult = WM_SetIndCallback(WcmWmcbIndication);
        if (wmResult != WM_ERRCODE_SUCCESS)
        {
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FATAL_ERROR;          // �z��͈͊O�̃G���[
        }

        // �����n�[�h�E�F�A�g�p���v���𔭍s
        wmResult = WM_Enable(WcmWmcbCommon);
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            WcmSetPhase(WCM_PHASE_WAIT_TO_IDLE);    // �񓯊��I�ȃV�[�P���X�̊J�n�ɐ���
            wcmw->notifyId = WCM_NOTIFY_STARTUP;
            break;

        case WM_ERRCODE_FIFO_ERROR:
            WCMi_Printf(wcmReportText_WmSyncError, "WM_Enable", wmResult);
            WcmSetPhase(WCM_PHASE_IRREGULAR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FAILURE;              // ARM7 �ւ̗v�����s�Ɏ��s

        case WM_ERRCODE_ILLEGAL_STATE:
            WCMi_Warning(wcmWarningText_InvalidWmState);

        /* Don't break here */
        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FATAL_ERROR;          // �z��͈͊O�̃G���[
        }
    }

    // ����I��
    (void)OS_RestoreInterrupts(e);
    return WCM_RESULT_ACCEPT;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_CleanupAsync

  Description:  �����@�\�̒�~�V�[�P���X���J�n����B
                �����I�ɂ� IDLE ���� WAIT �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_CLEANUP �ƂȂ�B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32 WCM_CleanupAsync(void)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // �������ς݂��m�F
    if (wcmw == NULL)
    {
        WCMi_Warning(wcmWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;                  // ����������Ă��Ȃ�
    }

    // �����t�F�[�Y�m�F
    switch (wcmw->phase)
    {
    case WCM_PHASE_IDLE:
        break;

    case WCM_PHASE_IDLE_TO_WAIT:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_PROGRESS;                 // ���ɓ������N�G�X�g�̔񓯊�������

    case WCM_PHASE_WAIT:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_SUCCESS;                  // ��ԑJ�ږڕW�t�F�[�Y�Ɉڍs�ς�

    default:
        WCMi_Warning(wcmWarningText_IllegalPhase, wcmw->phase);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;                  // �v�����󂯕t�����Ȃ��t�F�[�Y
    }

    // �����@�\�̒�~�V�[�P���X���J�n
    {
        WMErrCode   wmResult;

        // �����n�[�h�E�F�A�̃V���b�g�_�E���v���𔭍s
        wmResult = WM_PowerOff(WcmWmcbCommon);
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            WcmSetPhase(WCM_PHASE_IDLE_TO_WAIT);    // �񓯊��I�ȃV�[�P���X�̊J�n�ɐ���
            wcmw->notifyId = WCM_NOTIFY_CLEANUP;
            break;

        case WM_ERRCODE_FIFO_ERROR:
            WCMi_Printf(wcmReportText_WmSyncError, "WM_PowerOff", wmResult);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_REJECT;               // ARM7 �ւ̗v�����s�Ɏ��s( ���g���C�\ )

        case WM_ERRCODE_ILLEGAL_STATE:
            WCMi_Warning(wcmWarningText_InvalidWmState);

        /* Don't break here */
        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FATAL_ERROR;          // �z��͈͊O�̃G���[
        }
    }

    // ����I��
    (void)OS_RestoreInterrupts(e);
    return WCM_RESULT_ACCEPT;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_SearchAsync

  Description:  AP �����T���J�n�V�[�P���X�A�������͒�~�V�[�P���X���J�n����B
                �p�����[�^�ɂ���� WCM_BeginSearchAsync �֐����Ăяo�����A
                WCM_EndSearchAsync �֐����Ăяo�����U�蕪���邾���̊֐��B

  Arguments:    bssid   -   �T������ AP �� BSSID ���w�肷��BNULL ���w�肵��
                            �ꍇ�ɂ́A�T���̒�~���w���������̂Ƃ݂Ȃ����B
                essid   -   �T������ AP �� ESSID ���w�肷��BNULL ���w�肵��
                            �ꍇ�ɂ́A�T���̒�~���w���������̂Ƃ݂Ȃ����B
                option  -   �I�v�V�����ݒ�t�H�[�}�b�g�ɏ]���� 32 �r�b�g��
                            �I�v�V�����ύX�p�ϐ����w�肷��B0 ���w�肵���ꍇ��
                            �ύX�͍s���Ȃ��B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32 WCM_SearchAsync(void* bssid, void* essid, u32 option)
{
    // �p�����[�^���m�F
    if ((bssid == NULL) || (essid == NULL))
    {
        return WCM_EndSearchAsync();
    }

    return WCM_BeginSearchAsync(bssid, essid, option);
}

/*---------------------------------------------------------------------------*
  Name:         WCM_BeginSearchAsync

  Description:  AP �����T���J�n�V�[�P���X���J�n����B���Ɏ����T����Ԃł�����
                �ꍇ�ɁA�T�������̕ύX�̂ݍs�����Ƃ��\�B
                �����I�ɂ� IDLE ���� SEARCH �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_BEGIN_SEARCH �ƂȂ�B

  Arguments:    bssid   -   �T������ AP �� BSSID ���w�肷��BNULL ��������
                            WCM_BSSID_ANY ���w�肵���ꍇ�͔C�ӂ� BSSID ������
                            AP ��T������B
                essid   -   �T������ AP �� ESSID ���w�肷��BNULL ��������
                            WCM_ESSID_ANY ���w�肵���ꍇ�͔C�ӂ� ESSID ������
                            AP ��T������B
                option  -   �I�v�V�����ݒ�t�H�[�}�b�g�ɏ]���� 32 �r�b�g��
                            �I�v�V�����ύX�p�ϐ����w�肷��B0 ���w�肵���ꍇ��
                            �ύX�͍s��Ȃ��B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32 WCM_BeginSearchAsync(void* bssid, void* essid, u32 option)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // �������ς݂��m�F
    if (wcmw == NULL)
    {
        WCMi_Warning(wcmWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;                  // ����������Ă��Ȃ�
    }

    // AP �����T���J�n�V�[�P���X���J�n�A�������͒T�������ύX
    {
        WMErrCode   wmResult;

        switch (wcmw->phase)
        {
        case WCM_PHASE_IDLE:
            break;

        case WCM_PHASE_IDLE_TO_SEARCH:
            WcmEditScanExParam(bssid, essid, option);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_PROGRESS;             // ���ɓ������N�G�X�g�̔񓯊�������

        case WCM_PHASE_SEARCH:
            WcmEditScanExParam(bssid, essid, option);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_SUCCESS;              // ��ԑJ�ږڕW�t�F�[�Y�Ɉڍs�ς�

        default:
            WCMi_Warning(wcmWarningText_IllegalPhase, wcmw->phase);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FAILURE;              // �v�����󂯕t�����Ȃ��t�F�[�Y
        }

        // �X�L�����J�n�v���𔭍s
        WcmEditScanExParam(bssid, essid, option);
        DC_InvalidateRange(wcmw->scanExParam.scanBuf, wcmw->scanExParam.scanBufSize);
        wcmw->scanCount++;
        wmResult = WM_StartScanEx(WcmWmcbScanEx, &(wcmw->scanExParam));
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            WcmSetPhase(WCM_PHASE_IDLE_TO_SEARCH);  // �񓯊��I�ȃV�[�P���X�̊J�n�ɐ���
            wcmw->notifyId = WCM_NOTIFY_BEGIN_SEARCH;
            break;

        case WM_ERRCODE_FIFO_ERROR:
            WCMi_Printf(wcmReportText_WmSyncError, "WM_StartScanEx", wmResult);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_REJECT;               // ARM7 �ւ̗v�����s�Ɏ��s( ���g���C�\ )

        case WM_ERRCODE_ILLEGAL_STATE:
            WCMi_Warning(wcmWarningText_InvalidWmState);

        /* Don't break here */
        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FATAL_ERROR;          // �z��͈͊O�̃G���[
        }
    }

    // ����I��
    (void)OS_RestoreInterrupts(e);
    return WCM_RESULT_ACCEPT;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_EndSearchAsync

  Description:  AP �����T����~�V�[�P���X���J�n����B
                �����I�ɂ� SEARCH ���� IDLE �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A���@�I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_END_SEARCH �ƂȂ�B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32 WCM_EndSearchAsync(void)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // �������ς݂��m�F
    if (wcmw == NULL)
    {
        WCMi_Warning(wcmWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;  // ����������Ă��Ȃ�
    }

    // �����t�F�[�Y�m�F
    switch (wcmw->phase)
    {
    case WCM_PHASE_SEARCH:
        WcmSetPhase(WCM_PHASE_SEARCH_TO_IDLE);
        wcmw->notifyId = WCM_NOTIFY_END_SEARCH;
        break;

    case WCM_PHASE_SEARCH_TO_IDLE:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_PROGRESS; // ���ɓ������N�G�X�g�̔񓯊�������

    case WCM_PHASE_IDLE:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_SUCCESS;  // ��ԑJ�ږڕW�t�F�[�Y�Ɉڍs�ς�

    default:
        WCMi_Warning(wcmWarningText_IllegalPhase, wcmw->phase);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;  // �v�����󂯕t�����Ȃ��t�F�[�Y
    }

    /* �X�L������~�v���̔��s�� StartScanEx �̃R�[���o�b�N���ōs���̂ŁA�����ł͍s��Ȃ� */

    // ����I��
    (void)OS_RestoreInterrupts(e);
    return WCM_RESULT_ACCEPT;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_ConnectAsync

  Description:  AP �Ƃ̖����ڑ��V�[�P���X���J�n����B
                �����I�ɂ� IDLE ���� DCF �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_CONNECT �ƂȂ�B

  Arguments:    bssDesc -   �ڑ����� AP �̖����l�b�g���[�N�����w�肷��B
                            WCM_SearchAsync �ɂ���ē���ꂽ��񂪂��̂܂܎w��
                            ����邱�Ƃ�z��B
                wepDesc -   WCMWepDesc �^�� WEP �L�[�ɂ��Í����ݒ�����w��
                            ����BNULL �̏ꍇ�́AWEP �Í����Ȃ��Ƃ����ݒ�ɂȂ�B
                option  -   �I�v�V�����ݒ�t�H�[�}�b�g�ɏ]���� 32 �r�b�g��
                            �I�v�V�����ύX�p�ϐ����w�肷��B0 ���w�肵���ꍇ��
                            �ύX�͍s���Ȃ��B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32 WCM_ConnectAsync(void* bssDesc, void* wepDesc, u32 option)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // �������ς݂��m�F
    if (wcmw == NULL)
    {
        WCMi_Warning(wcmWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;              // ����������Ă��Ȃ�
    }

    // �����t�F�[�Y�m�F
    switch (wcmw->phase)
    {
    case WCM_PHASE_IDLE:
        // �p�����[�^�m�F
        if (bssDesc == NULL)
        {
            WCMi_Warning(wcmWarningText_IllegalParam, "bssDesc");
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FAILURE;          // bssDesc ������ NULL
        }

        if (((WMBssDesc*)bssDesc)->gameInfoLength > 0)
        {
            WCMi_Warning(wcmWarningText_IllegalParam, "bssDesc");
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FAILURE;          // bssDesc ������ AP ���ł͂Ȃ� DS �e�@
        }

        if (wepDesc != NULL)
        {
            if ((((WCMWepDesc*)wepDesc)->mode >= 4) || (((WCMWepDesc*)wepDesc)->keyId >= 4))
            {
                WCMi_Warning(wcmWarningText_IllegalParam, "wepDesc");
                (void)OS_RestoreInterrupts(e);
                return WCM_RESULT_FAILURE;      // wepDesc �������z��O�̓��e
            }

            wcmw->wepDesc.mode = ((WCMWepDesc*)wepDesc)->mode;
            wcmw->wepDesc.keyId = ((WCMWepDesc*)wepDesc)->keyId;
            if (wcmw->wepDesc.mode == WCM_WEPMODE_NONE)
            {
                MI_CpuClear8(wcmw->wepDesc.key, WM_SIZE_WEPKEY);
            }
            else
            {
                MI_CpuCopy8(((WCMWepDesc*)wepDesc)->key, wcmw->wepDesc.key, WM_SIZE_WEPKEY);
            }
        }
        else
        {
            MI_CpuClear8(&(wcmw->wepDesc), sizeof(WCMWepDesc));
        }

        MI_CpuCopy8(bssDesc, &(wcmw->bssDesc), sizeof(WMBssDesc));
#if 0
        /* debug */
        {
            int     i;
            u8*     pDump = (u8 *) (&(wcmw->bssDesc));

            for (i = 0; i < 192; i++)
            {
                if (i % 16)
                {
                    OS_TPrintf(" ");
                }

                OS_TPrintf("%02X", pDump[i]);
                if ((i % 16) == 15)
                {
                    OS_TPrintf("\n");
                }
            }
        }
#endif

        // �T�|�[�g����f�[�^�]�����[�g�� NITRO �p�ɉ���
        wcmw->bssDesc.rateSet.support = (u16) (WCM_ADDITIONAL_RATESET | wcmw->bssDesc.rateSet.basic);
        (void)WCM_UpdateOption(option);
        break;

    case WCM_PHASE_IDLE_TO_DCF:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_PROGRESS;             // ���ɓ������N�G�X�g�̔񓯊�������

    case WCM_PHASE_DCF:
        (void)OS_RestoreInterrupts(e);          // ��ԑJ�ږڕW�t�F�[�Y�Ɉڍs�ς�
        return WCM_RESULT_SUCCESS;

    default:
        WCMi_Warning(wcmWarningText_IllegalPhase, wcmw->phase);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;              // �v�����󂯕t�����Ȃ��t�F�[�Y
    }

    // AP �Ƃ̖����ڑ��V�[�P���X���J�n
    {
        WMErrCode   wmResult;

        // �����t�@�[���E�F�A�̃^�C���A�E�g�ݒ�v���𔭍s
        wmResult = WM_SetLifeTime(WcmWmcbCommon, 0xffff, WCM_CAM_LIFETIME, 0xffff, 0xffff);
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            WcmSetPhase(WCM_PHASE_IDLE_TO_DCF); // �񓯊��I�ȃV�[�P���X�̊J�n�ɐ���
            wcmw->notifyId = WCM_NOTIFY_CONNECT;
            break;

        case WM_ERRCODE_FIFO_ERROR:
            WCMi_Printf(wcmReportText_WmSyncError, "WM_SetLifeTime", wmResult);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_REJECT;           // ARM7 �ւ̗v�����s�Ɏ��s( ���g���C�\ )

        case WM_ERRCODE_ILLEGAL_STATE:
            WCMi_Warning(wcmWarningText_InvalidWmState);

        /* Don't break here */
        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FATAL_ERROR;      // �z��͈͊O�̃G���[
        }
    }

    // ����I��
    (void)OS_RestoreInterrupts(e);
    return WCM_RESULT_ACCEPT;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_DisconnectAsync

  Description:  AP �Ƃ̖����ڑ���ؒf����V�[�P���X���J�n����B
                �����I�ɂ� DCF ���� IDLE �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_DISCONNECT �ƂȂ�B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32 WCM_DisconnectAsync(void)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // �������ς݂��m�F
    if (wcmw == NULL)
    {
        WCMi_Warning(wcmWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;              // ����������Ă��Ȃ�
    }

    // �����t�F�[�Y�m�F
    switch (wcmw->phase)
    {
    case WCM_PHASE_DCF:
        break;

    case WCM_PHASE_DCF_TO_IDLE:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_PROGRESS;             // ���ɓ������N�G�X�g�̔񓯊�������

    case WCM_PHASE_IDLE:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_SUCCESS;              // ��ԑJ�ږڕW�t�F�[�Y�Ɉڍs�ς�

    default:
        WCMi_Warning(wcmWarningText_IllegalPhase, wcmw->phase);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;              // �v�����󂯕t�����Ȃ��t�F�[�Y
    }

    // AP �Ƃ̖����ڑ��̐ؒf�V�[�P���X���J�n
    if (wcmw->resetting == WCM_RESETTING_ON)
    {
        /* �ؒf�ʒm���󂯂ă��Z�b�g���Ȃ̂ŁA���̃��Z�b�g�������Đؒf�v�������s���ꂽ���Ƃɂ��� */
        WcmSetPhase(WCM_PHASE_DCF_TO_IDLE);
        wcmw->notifyId = WCM_NOTIFY_DISCONNECT;
    }
    else
    {
        WMErrCode   wmResult;

        // DCF �ʐM���[�h�I���v���𔭍s
        wmResult = WM_EndDCF(WcmWmcbEndDCF);
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            WcmSetPhase(WCM_PHASE_DCF_TO_IDLE); // �񓯊��I�ȃV�[�P���X�̊J�n�ɐ���
            wcmw->notifyId = WCM_NOTIFY_DISCONNECT;
            break;

        case WM_ERRCODE_FIFO_ERROR:
            WCMi_Printf(wcmReportText_WmSyncError, "WM_EndDCF", wmResult);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_REJECT;           // ARM7 �ւ̗v�����s�Ɏ��s( ���g���C�\ )

        case WM_ERRCODE_ILLEGAL_STATE:
            WCMi_Warning(wcmWarningText_InvalidWmState);

        /* Don't break here */
        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FATAL_ERROR;      // �z��͈͊O�̃G���[
        }
    }

    // ����I��
    (void)OS_RestoreInterrupts(e);
    return WCM_RESULT_ACCEPT;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_TerminateAsync

  Description:  �����@�\�̋����I���V�[�P���X���J�n����B
                �ʂ̔񓯊����������s����Ă��Ȃ�����͂ǂ̃t�F�[�Y����ł����s
                �\�ł���A�����I�ɂ� WAIT �ւƃt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_SartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_TERMINATE �ƂȂ�B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32 WCM_TerminateAsync(void)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // �������ς݂��m�F
    if (wcmw == NULL)
    {
        WCMi_Warning(wcmWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;              // ����������Ă��Ȃ�
    }

    // �����t�F�[�Y�m�F
    switch (wcmw->phase)
    {
    case WCM_PHASE_IDLE:
    case WCM_PHASE_DCF:
    case WCM_PHASE_IRREGULAR:
        break;

    case WCM_PHASE_TERMINATING:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_PROGRESS;             // ���ɓ������N�G�X�g�̔񓯊�������

    case WCM_PHASE_WAIT:
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_SUCCESS;              // ��ԑJ�ږڕW�t�F�[�Y�Ɉڍs�ς�

    case WCM_PHASE_SEARCH:
        WcmSetPhase(WCM_PHASE_TERMINATING);
        wcmw->notifyId = WCM_NOTIFY_TERMINATE;
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_ACCEPT;               // �񓯊������v�������

    /* ���Z�b�g�v���̔��s�� StartScanEx �̃R�[���o�b�N���ōs���̂ŁA�����ł͍s��Ȃ� */
    case WCM_PHASE_WAIT_TO_IDLE:
    case WCM_PHASE_IDLE_TO_WAIT:
    case WCM_PHASE_IDLE_TO_SEARCH:
    case WCM_PHASE_SEARCH_TO_IDLE:
    case WCM_PHASE_IDLE_TO_DCF:
    case WCM_PHASE_DCF_TO_IDLE:
    case WCM_PHASE_FATAL_ERROR:
    default:
        WCMi_Warning(wcmWarningText_IllegalPhase, wcmw->phase);
        (void)OS_RestoreInterrupts(e);
        return WCM_RESULT_FAILURE;              // �v�����󂯕t�����Ȃ��t�F�[�Y
    }

    // �����@�\�̋����I���V�[�P���X���J�n����
    if (wcmw->resetting == WCM_RESETTING_ON)
    {
        /* DCF �ʐM���ɐؒf����ă��Z�b�g���Ȃ̂ŁA���̃��Z�b�g�������ċ����I���v�������s���ꂽ���Ƃɂ��� */
        WcmSetPhase(WCM_PHASE_TERMINATING);
        wcmw->notifyId = WCM_NOTIFY_TERMINATE;
    }
    else
    {
        WMStatus*   ws;
        u16         wmState;
        WMErrCode   wmResult;

        // WM ���C�u�����̓�����Ԃ𒲍�
        ws = (WMStatus*)WMi_GetStatusAddress();
        DC_InvalidateRange(ws, 2);
        wmState = ws->state;

        // WM ������Ԃɂ���~��ԂɑJ�ڂ���悤�A�����J�n
        switch (wmState)
        {
        case WM_STATE_READY:
            // WM ���C�u�����I��
            wmResult = WM_Finish();
            if (wmResult == WM_ERRCODE_SUCCESS)
            {
                WcmSetPhase(WCM_PHASE_WAIT);
                wcmw->notifyId = WCM_NOTIFY_COMMON;
                (void)OS_RestoreInterrupts(e);
                return WCM_RESULT_SUCCESS;      // �����I�� WAIT �t�F�[�Y�ւ̈ڍs������
            }
            break;

        case WM_STATE_STOP:
            // �����n�[�h�E�F�A�̎g�p�֎~�v���𔭍s
            wmResult = WM_Disable(WcmWmcbCommon);
            break;

        case WM_STATE_IDLE:
            // �����n�[�h�E�F�A�̃V���b�g�_�E���v���𔭍s
            wmResult = WM_PowerOff(WcmWmcbCommon);
            break;

        default:
            // ���Z�b�g�d���Ăяo���Ǘ��t���O���Z�b�g
            wcmw->resetting = WCM_RESETTING_ON;

            // �����ڑ���Ԃ̃��Z�b�g�v���𔭍s
            wmResult = WM_Reset(WcmWmcbReset);
        }

        // �v�����s�ɑ΂��铯���I�ȏ������ʂ��m�F
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            WcmSetPhase(WCM_PHASE_TERMINATING); // �񓯊��I�ȃV�[�P���X�̊J�n�ɐ���
            wcmw->notifyId = WCM_NOTIFY_TERMINATE;
            break;

        case WM_ERRCODE_FIFO_ERROR:
            WCMi_Printf(wcmReportText_WmSyncError, "WM_***", wmResult);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_REJECT;           // ARM7 �ւ̗v�����s�Ɏ��s( ���g���C�\ )

        case WM_ERRCODE_ILLEGAL_STATE:
            WCMi_Warning(wcmWarningText_InvalidWmState);

        /* Don't break here */
        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            (void)OS_RestoreInterrupts(e);
            return WCM_RESULT_FATAL_ERROR;      // �z��͈͊O�̃G���[
        }
    }

    // ����I��
    (void)OS_RestoreInterrupts(e);
    return WCM_RESULT_ACCEPT;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_GetPhase

  Description:  WCM ���C�u�����̓������ ( �t�F�[�Y ) ���擾����B�����֐��B

  Arguments:    None.

  Returns:      s32     -   ���݂� WCM ���C�u�����̃t�F�[�Y��Ԃ��B
 *---------------------------------------------------------------------------*/
s32 WCM_GetPhase(void)
{
    OSIntrMode  e = OS_DisableInterrupts();
    s32         phase = WCM_PHASE_NULL;

    // �������ς݂��m�F
    if (wcmw != NULL)
    {
        phase = (s32) (wcmw->phase);
    }

    (void)OS_RestoreInterrupts(e);
    return phase;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_UpdateOption

  Description:  WCM ���C�u�����̃I�v�V�����ݒ���X�V����B

  Arguments:    option  -   �I�v�V�����ݒ�t�H�[�}�b�g�ɏ]���� 32 �r�b�g��
                            �I�v�V�����ύX�p�ϐ����w�肷��B
                            0 ���w�肵���ꍇ�͉����X�V���s��Ȃ�����ɂȂ�B

  Returns:      u32     -   �ύX���s���O�̃I�v�V�����ϐ���Ԃ��B
 *---------------------------------------------------------------------------*/
u32 WCM_UpdateOption(u32 option)
{
    OSIntrMode  e = OS_DisableInterrupts();
    u32         filter = 0;
    u32         old_option = wcmw->option;

    // �������m�F
    if (wcmw == NULL)
    {
        (void)OS_RestoreInterrupts(e);
        return 0;
    }

    // �X�V���ׂ��I�v�V�����J�e�S���𒊏o���A��U�N���A���ׂ��r�b�g��ҏW����
    if (option & WCM_OPTION_TEST_CHANNEL)
    {
        filter |= WCM_OPTION_FILTER_CHANNEL;
        if (!(option & WCM_OPTION_FILTER_CHANNEL))
        {
            option |= WCM_OPTION_CHANNEL_RDC;
        }
    }

    if (option & WCM_OPTION_TEST_POWER)
    {
        filter |= WCM_OPTION_FILTER_POWER;
    }

    if (option & WCM_OPTION_TEST_AUTH)
    {
        filter |= WCM_OPTION_FILTER_AUTH;
    }

    if (option & WCM_OPTION_TEST_SCANTYPE)
    {
        filter |= WCM_OPTION_FILTER_SCANTYPE;
    }

    if (option & WCM_OPTION_TEST_ROUNDSCAN)
    {
        filter |= WCM_OPTION_FILTER_ROUNDSCAN;
    }

    // �I�v�V�����ϐ��̊e�r�b�g���X�V
    wcmw->option = (u32) ((old_option &~filter) | option);

    (void)OS_RestoreInterrupts(e);
    return old_option;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_SetChannelScanTime

  Description:  AP �̎����T�����ɂP�̃`�����l�����X�L�������鎞�Ԃ�ݒ肷��B

  Arguments:    msec    -   �P�`�����l�����X�L�������鎞�Ԃ� ms �P�ʂŎw��B
                            10 �` 1000 �܂ł̊ԂŐݒ�\�����A���͈̔͊O�̒l��
                            �w�肷��Ǝ����T���̓f�t�H���g�̐ݒ莞�ԂŒT�����s��
                            �悤�ɂȂ�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WCM_SetChannelScanTime(u16 msec)
{
    // �������m�F
    if (wcmw != NULL)
    {
        if ((msec >= 10) && (msec <= 1000))
        {
            wcmw->maxScanTime = msec;
        }
        else
        {
            wcmw->maxScanTime = 0;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WCMi_GetSystemWork

  Description:  WCM ���C�u�����������Ŏg�p���Ă��郏�[�N�o�b�t�@�ւ̃|�C���^��
                �擾���邽�߂̓����֐��B

  Arguments:    None.

  Returns:      WCMWork*    -   ���[�N�o�b�t�@�ւ̃|�C���^��Ԃ��B�������O�Ȃ�
                                �o�b�t�@�����݂��Ȃ��ꍇ�� NULL �ɂȂ�B
 *---------------------------------------------------------------------------*/
WCMWork* WCMi_GetSystemWork(void)
{
    return wcmw;
}

/*---------------------------------------------------------------------------*
  Name:         WcmConfigure

  Description:  WCM �̓�������ݒ��ҏW����B

  Arguments:    config  -   WCM �̓���ݒ�\���̂ւ̃|�C���^���w��B
                notify  -   �񓯊��I�ȏ������ʂ�ʒm����R�[���o�b�N�֐����w��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmConfigure(WCMConfig* config, WCMNotify notify)
{
    if (config == NULL)
    {
        // �ݒ肪������Ȃ��ꍇ�A�f�t�H���g�l���g�p����
        wcmw->config.dmano = 3;
        wcmw->config.pbdbuffer = NULL;
        wcmw->config.nbdbuffer = 0;
        wcmw->config.nbdmode = 0;
    }
    else
    {
        // DMA �ԍ���ޔ�
        if (config->dmano &~(0x03))
        {
            // DMA �ԍ��� 0 �` 3 �łȂ��Ƃ����Ȃ�
            WCMi_Warning(wcmWarningText_IllegalParam, "config->dmano");
        }

        wcmw->config.dmano = (config->dmano & 0x03);

        // AP ���ێ����X�g�̈�̐ݒ��ޔ�
        if ((((4 - ((u32) (config->pbdbuffer) & 0x03)) % 4) + sizeof(WCMApListHeader)) > config->nbdbuffer)
        {
            // ���X�g�Ǘ��p�w�b�_�̈悷��m�ۂł��Ȃ��T�C�Y�Ȃ̂ŁA�o�b�t�@�Ȃ��̏ꍇ�Ɠ���
            wcmw->config.pbdbuffer = NULL;
            wcmw->config.nbdbuffer = 0;
        }
        else
        {
            /*
             * �^����ꂽ�o�b�t�@�� 4 �o�C�g�A���C������Ă��Ȃ��\�������邽�߁A
             * 4 �o�C�g�A���C�����ꂽ�ʒu�ɂ��炵�A�T�C�Y�����̕��ڌ��肳����B
             */
            wcmw->config.pbdbuffer = (void*)WCM_ROUNDUP4((u32) (config->pbdbuffer));
            wcmw->config.nbdbuffer = config->nbdbuffer - (s32) ((4 - ((u32) (config->pbdbuffer) & 0x03)) % 4);
            MI_CpuClear8(wcmw->config.pbdbuffer, (u32) (wcmw->config.nbdbuffer));
        }

        wcmw->config.nbdmode = config->nbdmode;
    }

    // �ʒm�x�N�g���ޔ�
    wcmw->notify = notify;
}

/*---------------------------------------------------------------------------*
  Name:         WcmEditScanExParam

  Description:  WM ���C�u�����ɃX�L�������w������ۂ̃X�L�����ݒ�\���̂�ҏW����B

  Arguments:    bssid   -   �T������ BSSID �t�B���^�[�BWCM_BSSID_ANY �͑S�� 0xff
                            �ł��� BSSID �Ȃ̂ŁA�t�B���^�[���Ȃ��ݒ�ƂȂ�B
                essid   -   �T������ ESSID �t�B���^�[�BWCM_ESSID_ANY ���w�肷���
                            ���� 0 �� ESSID �Ƃ݂Ȃ���A�t�B���^�[���Ȃ��ݒ�ƂȂ�B
                option  -   �I�v�V�����ύX�p�ϐ��B�ύX���s���� WCM ���C�u��������
                            ���ʎg�p����� option ���ς���āA���ɂ͖߂�Ȃ��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmEditScanExParam(void* bssid, void* essid, u32 option)
{
    (void)WCM_UpdateOption(option);
    wcmw->scanExParam.scanBuf = (WMBssDesc*)wcmw->recvBuf;
    wcmw->scanExParam.scanBufSize = WM_SIZE_SCAN_EX_BUF;
    wcmw->scanExParam.channelList = (u16) ((0x0001 << WcmGetNextScanChannel(0)) >> 1);
    wcmw->scanExParam.maxChannelTime = ((wcmw->maxScanTime != 0) ? wcmw->maxScanTime : WM_GetDispersionScanPeriod());
    wcmw->scanExParam.scanType = (u16) (((wcmw->option & WCM_OPTION_MASK_SCANTYPE) == WCM_OPTION_SCANTYPE_ACTIVE) ? WM_SCANTYPE_ACTIVE : WM_SCANTYPE_PASSIVE);
    if (bssid == NULL)
    {
        MI_CpuCopy8(WCM_Bssid_Any, wcmw->scanExParam.bssid, WCM_BSSID_SIZE);
    }
    else
    {
        MI_CpuCopy8(bssid, wcmw->scanExParam.bssid, WCM_BSSID_SIZE);
    }

    //MI_CpuCopy8( bssid , wcmw->scanExParam.bssid , WCM_BSSID_SIZE );
    if ((essid == NULL) || (essid == (void*)WCM_Essid_Any))
    {
        MI_CpuCopy8(WCM_Essid_Any, wcmw->scanExParam.ssid, WCM_ESSID_SIZE);
        wcmw->scanExParam.ssidLength = 0;
    }
    else
    {
        s32 len;

        MI_CpuCopy8(essid, wcmw->scanExParam.ssid, WCM_ESSID_SIZE);
        for (len = 0; len < WCM_ESSID_SIZE; len++)
        {
            if (((u8*)essid)[len] == '\0')
            {
                break;
            }
        }

        wcmw->scanExParam.ssidLength = (u16) len;
    }

    wcmw->scanCount = 0;
}

/*---------------------------------------------------------------------------*
  Name:         WcmInitOption

  Description:  WCM ���C�u�����̃I�v�V�����ݒ�l������������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmInitOption(void)
{
    /*
     * IW �� I/O �����킹��ׂɉ��L�̂悤�ȃf�t�H���g�ݒ�ɂȂ��Ă��邪�A
     * CHANNEL �� ALL �ASCANTYPE �� ACTIVE �ł������ق����Ó��Ǝv����B
     */
    wcmw->option = WCM_OPTION_CHANNEL_RDC |
        WCM_OPTION_POWER_SAVE |
        WCM_OPTION_AUTH_OPENSYSTEM |
        WCM_OPTION_SCANTYPE_PASSIVE |
        WCM_OPTION_ROUNDSCAN_IGNORE;
}

/*---------------------------------------------------------------------------*
  Name:         WcmGetNextScanChannel

  Description:  �I�v�V�����ɐݒ肳��Ă���T�����̃`�����l�����X�g����A����
                �X�L�������ׂ��`�����l�������肷��B

  Arguments:    channel -   ����X�L���������`�����l����n���B

  Returns:      u16     -   ����X�L�������ׂ��`�����l����Ԃ��B
 *---------------------------------------------------------------------------*/
static u16 WcmGetNextScanChannel(u16 channel)
{
    s32 i;

    for (i = 0; i < 13; i++)
    {
        if (wcmw->option & (0x0001 << (((channel + i) % 13) + 1)))
        {
            break;
        }
    }

    return(u16) (((channel + i) % 13) + 1);
}

/*---------------------------------------------------------------------------*
  Name:         WcmNotify

  Description:  �񓯊��I�ȏ����̏������ʂ��R�[���o�b�N����B
                �s����ɔ�������ʒm�����̊֐���ʂ��ăR�[���o�b�N�����B
                �ʒm��ʂɂ��Ă� WCM �������ʕϐ����玩���I�ɐݒ肳��A����
                �d�������ʒm������邽�߂ɁA�ʒm����x�ɃN���A�����B

  Arguments:    result  -   �������ʂ��w�肷��B
                para0   -   �ʒm�֐��ɓn���p�����[�^[ 0 ]
                para1   -   �ʒm�֐��ɓn���p�����[�^[ 1 ]
                para2   -   �ʒm�֐��ɓn���p�����[�^[ 2 ]

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmNotify(s16 result, void* para0, void* para1, s32 para2)
{
    s16 notifyId = wcmw->notifyId;

    wcmw->notifyId = WCM_NOTIFY_COMMON;
    WcmNotifyEx(notifyId, result, para0, para1, para2);
}

/*---------------------------------------------------------------------------*
  Name:         WcmNotifyEx

  Description:  �A�v���P�[�V�����ւ̃R�[���o�b�N�ɂ��ʒm���s���B

  Arguments:    result  -   �������ʂ��w�肷��B
                para0   -   �ʒm�֐��ɓn���p�����[�^[ 0 ]
                para1   -   �ʒm�֐��ɓn���p�����[�^[ 1 ]
                para2   -   �ʒm�֐��ɓn���p�����[�^[ 2 ]

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmNotifyEx(s16 notify, s16 result, void* para0, void* para1, s32 para2)
{
    if (wcmw->notify)
    {
        WCMNotice   notice;

        notice.notify = notify;
        notice.result = result;
        notice.parameter[0].p = para0;
        notice.parameter[1].p = para1;
        notice.parameter[2].n = para2;
        wcmw->notify(&notice);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmSetPhase

  Description:  WCM ���C�u�����̓������ ( �t�F�[�Y ) ��ύX����B
                FATAL_ERROR ��Ԃ���ʂ̃t�F�[�Y�ɕύX�͂ł��Ȃ��B
                �܂��ADCF �ʐM��Ԃւ̃t�F�[�Y�ύX�y�� DCF �ʐM��Ԃ����
                �t�F�[�Y�ύX���Ď����A�L�[�v�A���C�u�p�P�b�g���M�p�A���[����
                ����𐧌䂷��B

  Arguments:    phase   -   �ύX����t�F�[�Y���w��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmSetPhase(u32 phase)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // DCF �ʐM��Ԃ���ʂ̏�ԂɈڍs����ۂɃA���[�����~
    if ((wcmw->phase == WCM_PHASE_DCF) && (phase != WCM_PHASE_DCF))
    {
        // Keep Alive �p�A���[����~
        OS_CancelAlarm(&(wcmw->keepAliveAlarm));
    }

    // FATAL ERROR ��ԂłȂ��ꍇ�͎w���ԂɕύX
    if (wcmw->phase != WCM_PHASE_FATAL_ERROR)
    {
        wcmw->phase = phase;
    }

    // DCF �ʐM��ԂɈڍs����ۂɃA���[�����J�n
    if (phase == WCM_PHASE_DCF)
    {
        // Keep Alive �p�A���[���J�n
        OS_SetAlarm(&(wcmw->keepAliveAlarm), OS_SecondsToTicks(WCM_KEEP_ALIVE_SPAN), WcmKeepAliveAlarm, NULL);
    }
    (void)OS_RestoreInterrupts(e);
}

/*---------------------------------------------------------------------------*
  Name:         WCMi_ResetKeepAliveAlarm

  Description:  �L�[�v�A���C�u NULL �p�P�b�g���M�p�A���[�������Z�b�g���A�K��
                ���Ԃ̃A���[����ݒ肵�����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WCMi_ResetKeepAliveAlarm(void)
{
    OSIntrMode  e = OS_DisableInterrupts();

    // ���߂ăA���[�����Z�b�g
    OS_CancelAlarm(&(wcmw->keepAliveAlarm));
    if (wcmw->phase == WCM_PHASE_DCF)
    {
        OS_SetAlarm(&(wcmw->keepAliveAlarm), OS_SecondsToTicks(WCM_KEEP_ALIVE_SPAN), WcmKeepAliveAlarm, NULL);
    }
    (void)OS_RestoreInterrupts(e);
}

/*---------------------------------------------------------------------------*
  Name:         WcmKeepAliveAlarm

  Description:  �L�[�v�A���C�u NULL �p�P�b�g���M�p�A���[���̃A���[���n���h���B

  Arguments:    arg     -   ���g�p�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmKeepAliveAlarm(void* arg)
{
#pragma unused(arg)

    WCMi_CpsifSendNullPacket();
    WCMi_ResetKeepAliveAlarm();
}

#include <nitro/code32.h>

/*---------------------------------------------------------------------------*
  Name:         WcmCountBits

  Description:  u32 �^�̒l�Ɋ܂܂�� 1 �ł���r�b�g�̐����擾����B

  Arguments:    arg     -   �������� u32 �^�̒l�B

  Returns:      u32     -   �����Ɋ܂܂�� 1 �r�b�g�̐���Ԃ��B0 �` 32 �̒l�B
 *---------------------------------------------------------------------------*/

static asm u32
WcmCountBits( u32 arg )
{
    mov     r1, r0
    mov     r0, #0
    mov     r3, #1
@loop:
    clz     r2, r1
    rsbs    r2, r2, #31
    bxcc    lr
    bic     r1, r1, r3, LSL r2
    add     r0, r0, #1
    b       @loop
}

/*---------------------------------------------------------------------------*
  Name:         WcmCountLeadingZero

  Description:  u32 �^�̒l�̐�s�[���J�E���g(�ŏ�ʃr�b�g���� 0 �ł���r�b�g��
                �����A�����Ă��邩)���擾����B

  Arguments:    arg     -   �������� u32 �^�̒l�B

  Returns:      u32     -   ��s�[���J�E���g��Ԃ��B0 �` 32 �̒l�B
 *---------------------------------------------------------------------------*/

static asm u32
WcmCountLeadingZero( u32 arg )
{
    clz     r0, r0
    bx      lr
}
#include <nitro/codereset.h>

/*---------------------------------------------------------------------------*
  Name:         WcmWmReset

  Description:  WM_Reset �֐��ɂ�� WM ���C�u�����̃��Z�b�g�������J�n����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmReset(void)
{
    WMErrCode   wmResult;

    if (wcmw->resetting == WCM_RESETTING_OFF)
    {
        // ���Z�b�g�d���Ăяo���Ǘ��t���O���Z�b�g
        wcmw->resetting = WCM_RESETTING_ON;

        // �����ڑ���Ԃ̃��Z�b�g�v���𔭍s
        wmResult = WM_Reset(WcmWmcbReset);
        if (wmResult != WM_ERRCODE_OPERATING)
        {
            /* ���Z�b�g�Ɏ��s�����ꍇ�͕����s�\ */
            WCMi_Printf(wcmReportText_WmSyncError, "WM_Reset", wmResult);
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);
            WcmNotify(WCM_RESULT_FATAL_ERROR, 0, 0, __LINE__);
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmWmcbIndication

  Description:  WM ���C�u��������̕s����ʒm���󂯎��n���h���B

  Arguments:    arg     -   WM ���C�u��������n�����ʒm�p�����[�^�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmcbIndication(void* arg)
{
    WMIndCallback*  cb = (WMIndCallback*)arg;

    /* WCM ���������Ɋ֌W���Ȃ��ʒm�͖������� */
    if (cb->errcode == WM_ERRCODE_FIFO_ERROR)
    {
        if ((cb->state == WM_STATECODE_FIFO_ERROR) && (cb->reason == WM_APIID_AUTO_DISCONNECT))
        {
            /*
             * �ʐM�̐��������ۂĂȂ��Ȃ�v���I�ȃn�[�h�E�F�A�G���[�� ARM7 ���Ō��m����A
             * �����I�ɐؒf���悤�Ƃ��������N�G�X�g�L���[���l�܂��Ă��Đؒf�v����\��ł��Ȃ������ꍇ
             */
            switch (wcmw->phase)
            {
            case WCM_PHASE_IDLE_TO_DCF:
                WcmSetPhase(WCM_PHASE_IRREGULAR);
                break;

            case WCM_PHASE_DCF:
            case WCM_PHASE_IRREGULAR:
                WcmWmReset();
                break;

            case WCM_PHASE_DCF_TO_IDLE:
                WcmSetPhase(WCM_PHASE_IRREGULAR);
                break;

                /* �z��O�̃t�F�[�Y�ł̒ʒm�͖������� */
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmWmcbCommon

  Description:  WM ���C�u��������̒ʒm���󂯎��n���h���B
                WM_Enable , WM_Disable , WM_PowerOn , WM_PowerOff ,
                WM_SetLifeTime , WM_SetBeaconIndication , WM_SetWEPKeyEx
                �ɑ΂��錋�ʉ������󂯎��B

  Arguments:    arg     -   WM ���C�u��������n�����ʒm�p�����[�^�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmcbCommon(void* arg)
{
    WMCallback*     cb = (WMCallback*)arg;
    WMErrCode       wmResult = WM_ERRCODE_MAX;

    switch (cb->errcode)
    {
    case WM_ERRCODE_SUCCESS:
        // ���̃X�e�b�v�Ƃ��čs���ׂ�����������
        switch (cb->apiid)
        {
        case WM_APIID_ENABLE:
            // �����n�[�h�E�F�A�ւ̓d�͋����v���𔭍s
            wmResult = WM_PowerOn(WcmWmcbCommon);
            break;

        case WM_APIID_DISABLE:
            // WM ���C�u�����I��
            wmResult = WM_Finish();
            switch (wmResult)
            {
            case WM_ERRCODE_SUCCESS:
                WcmSetPhase(WCM_PHASE_WAIT);        // �񓯊��V�[�P���X����I��
                WcmNotify(WCM_RESULT_SUCCESS, 0, 0, __LINE__);
                break;

            case WM_ERRCODE_WM_DISABLE:
                WCMi_Warning(wcmWarningText_InvalidWmState);

            /* Don't break here */
            default:
                WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
                WcmNotify(WCM_RESULT_FATAL_ERROR, 0, 0, __LINE__);
            }

            return; // ���̃X�e�b�v�͂Ȃ��̂ł����ŏI��

        case WM_APIID_POWER_ON:
            WcmSetPhase(WCM_PHASE_IDLE);    // �񓯊��V�[�P���X����I��
            WcmNotify(WCM_RESULT_SUCCESS, 0, 0, __LINE__);
            return; // ���̃X�e�b�v�͂Ȃ��̂ł����ŏI��

        case WM_APIID_POWER_OFF:
            // �����n�[�h�E�F�A�̎g�p�֎~�v���𔭍s
            wmResult = WM_Disable(WcmWmcbCommon);
            break;

        case WM_APIID_SET_LIFETIME:
            // �r�[�R������M�ʒm OFF �v���𔭍s
            wmResult = WM_SetBeaconIndication(WcmWmcbCommon, 0);
            break;

        case WM_APIID_SET_BEACON_IND:
            // WEP �Í����ݒ�v���𔭍s
            wmResult = WM_SetWEPKeyEx(WcmWmcbCommon, (u16) (wcmw->wepDesc.mode), (u16) (wcmw->wepDesc.keyId),
                                      (const u8*)(wcmw->wepDesc.key));
            break;

        case WM_APIID_SET_WEPKEY_EX:
            // AP �ւ̖����ڑ��v���𔭍s
            wmResult = WM_StartConnectEx(WcmWmcbConnect, &(wcmw->bssDesc), NULL,
                                         ((wcmw->option & WCM_OPTION_MASK_POWER) == WCM_OPTION_POWER_ACTIVE ? FALSE : TRUE),
                                         ((wcmw->option & WCM_OPTION_MASK_AUTH) == WCM_OPTION_AUTH_SHAREDKEY ? (u16) WM_AUTHMODE_SHARED_KEY :
                                                     (u16) WM_AUTHMODE_OPEN_SYSTEM));
            break;
        }

        // �����I�ȏ������ʂ��m�F
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            break;

        case WM_ERRCODE_FIFO_ERROR:
#if WCM_DEBUG
            switch (cb->apiid)
            {
            case WM_APIID_ENABLE:
                WCMi_Printf(wcmReportText_WmSyncError, "WM_PowerOn", wmResult);
                break;

            case WM_APIID_POWER_OFF:
                WCMi_Printf(wcmReportText_WmSyncError, "WM_Disable", wmResult);
                break;

            case WM_APIID_SET_LIFETIME:
                WCMi_Printf(wcmReportText_WmSyncError, "WM_SetBeaconIndication", wmResult);
                break;

            case WM_APIID_SET_BEACON_IND:
                WCMi_Printf(wcmReportText_WmSyncError, "WM_SetWEPKeyEx", wmResult);
                break;

            case WM_APIID_SET_WEPKEY_EX:
                WCMi_Printf(wcmReportText_WmSyncError, "WM_StartConnectEx", wmResult);
                break;
            }
#endif
            WcmSetPhase(WCM_PHASE_IRREGULAR);   // ARM7 �ւ̗v�����s�Ɏ��s( �V�[�P���X�r��Ń��g���C�s�\ )
            WcmNotify(WCM_RESULT_FAILURE, (wcmw->notifyId == WCM_NOTIFY_CONNECT ? &(wcmw->bssDesc) : 0), 0, __LINE__);
            break;

        case WM_ERRCODE_ILLEGAL_STATE:
            WCMi_Warning(wcmWarningText_InvalidWmState);

        /* Don't break here */
        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
            WcmNotify(WCM_RESULT_FATAL_ERROR, (wcmw->notifyId == WCM_NOTIFY_CONNECT ? &(wcmw->bssDesc) : 0), 0, __LINE__);
        }
        break;

    case WM_ERRCODE_FAILED:
#if WCM_DEBUG
        switch (cb->apiid)
        {
        case WM_APIID_ENABLE:
            WCMi_Printf(wcmReportText_WmAsyncError, "WM_Enable", cb->errcode, cb->wlCmdID, cb->wlResult);
            break;

        case WM_APIID_DISABLE:
            WCMi_Printf(wcmReportText_WmAsyncError, "WM_Disable", cb->errcode, cb->wlCmdID, cb->wlResult);
            break;

        case WM_APIID_POWER_ON:
            WCMi_Printf(wcmReportText_WmAsyncError, "WM_PowerOn", cb->errcode, cb->wlCmdID, cb->wlResult);
            break;

        case WM_APIID_POWER_OFF:
            WCMi_Printf(wcmReportText_WmAsyncError, "WM_PowerOff", cb->errcode, cb->wlCmdID, cb->wlResult);
            break;

        case WM_APIID_SET_LIFETIME:
            WCMi_Printf(wcmReportText_WmAsyncError, "WM_SetLifeTime", cb->errcode, cb->wlCmdID, cb->wlResult);
            break;

        case WM_APIID_SET_BEACON_IND:
            WCMi_Printf(wcmReportText_WmAsyncError, "WM_SetBeaconIndication", cb->errcode, cb->wlCmdID, cb->wlResult);
            break;

        case WM_APIID_SET_WEPKEY_EX:
            WCMi_Printf(wcmReportText_WmAsyncError, "WM_SetWEPKeyEx", cb->errcode, cb->wlCmdID, cb->wlResult);
            break;
        }
#endif
        WcmSetPhase(WCM_PHASE_IRREGULAR);       // ARM7 �łȂ�炩�̗��R�ŃG���[
        WcmNotify(WCM_RESULT_FAILURE, (wcmw->notifyId == WCM_NOTIFY_CONNECT ? &(wcmw->bssDesc) : 0), 0, __LINE__);
        break;

    case WM_ERRCODE_ILLEGAL_STATE:
    case WM_ERRCODE_WM_DISABLE:
        WCMi_Warning(wcmWarningText_InvalidWmState);

    /* Don't break here */
    default:
        WcmSetPhase(WCM_PHASE_FATAL_ERROR);     // �z��͈͊O�̃G���[
        WcmNotify(WCM_RESULT_FATAL_ERROR, (wcmw->notifyId == WCM_NOTIFY_CONNECT ? &(wcmw->bssDesc) : 0), 0, __LINE__);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmWmcbScanEx

  Description:  WM ���C�u��������̒ʒm���󂯎��n���h���B
                WM_StartScanEx �ɑ΂��錋�ʉ������󂯎��B

  Arguments:    arg     -   WM ���C�u��������n�����ʒm�p�����[�^�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmcbScanEx(void* arg)
{
    WMStartScanExCallback*  cb = (WMStartScanExCallback*)arg;
    WMErrCode   wmResult = WM_ERRCODE_MAX;

    switch (cb->errcode)
    {
    case WM_ERRCODE_SUCCESS:
        // �����T���J�n���̏ꍇ�́A����Ɏ����T�����[�h�ɓ��������Ƃ�ʒm
        if (wcmw->phase == WCM_PHASE_IDLE_TO_SEARCH)
        {
            WcmSetPhase(WCM_PHASE_SEARCH);

            /* ���̒ʒm���ɂĎ����T����~��v�������ꍇ���z�肳��� */
            WcmNotify(WCM_RESULT_SUCCESS, 0, 0, __LINE__);
        }

        // ���̃X�e�b�v�Ƃ��čs���ׂ�����������
        switch (wcmw->phase)
        {
        case WCM_PHASE_SEARCH:
            wcmw->notifyId = WCM_NOTIFY_FOUND_AP;
            if (cb->state == WM_STATECODE_PARENT_FOUND)
            {
                // AP ���𔭌������ꍇ�A�����̃��X�g��ҏW���A�v���P�[�V�����ɒʒm
                s32 i;

                DC_InvalidateRange(wcmw->scanExParam.scanBuf, wcmw->scanExParam.scanBufSize);
                for (i = 0; i < cb->bssDescCount; i++)
                {
                    WCMi_EntryApList(cb->bssDesc[i], cb->linkLevel[i]);

                    /*
                     * IW ��I/O �����킹��ׂɉ��L�̂悤�ȃp�����[�^�\���ɂ��Ă��邪�A
                     * cb �� i ���p�����[�^�Ɏ������ق����Ó��Ǝv����B
                     */
                    WcmNotifyEx(WCM_NOTIFY_FOUND_AP, WCM_RESULT_SUCCESS, cb->bssDesc[i], (void*)cb, __LINE__);
                }
            }

            // �w�肳�ꂽ�`�����l�����ꏄ�������𔻒�
            if ((wcmw->option & WCM_OPTION_MASK_ROUNDSCAN) == WCM_OPTION_ROUNDSCAN_NOTIFY)
            {
                u32 channels = WcmCountBits(wcmw->option & WCM_OPTION_FILTER_CHANNEL);

                if (channels > 0)
                {
                    if ((wcmw->scanCount % channels) == 0)
                    {
                        // �e�`�����l���̃X�L�������ꏄ�������Ƃ�ʒm
                        WcmNotifyEx(WCM_NOTIFY_SEARCH_AROUND, WCM_RESULT_SUCCESS, (void*)(wcmw->scanCount), 0, __LINE__);
                    }
                }
            }

            // ���̃`�����l���̃X�L�����J�n�v���𔭍s
            wcmw->scanExParam.channelList = (u16) ((0x0001 << WcmGetNextScanChannel((u16) (32 - WcmCountLeadingZero(cb->channelList)))) >> 1);
            DC_InvalidateRange(wcmw->scanExParam.scanBuf, wcmw->scanExParam.scanBufSize);
            wcmw->scanCount++;
            wmResult = WM_StartScanEx(WcmWmcbScanEx, &(wcmw->scanExParam));
            break;

        case WCM_PHASE_SEARCH_TO_IDLE:
            // �X�L������~�v���𔭍s
            wmResult = WM_EndScan(WcmWmcbEndScan);
            break;

        case WCM_PHASE_TERMINATING:
            // �����I���V�[�P���X���̏ꍇ�͂����Ń��Z�b�g
            WcmWmReset();
            return;
        }

        // �����I�ȏ������ʂ��m�F
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            break;

        case WM_ERRCODE_FIFO_ERROR:
#if WCM_DEBUG
            switch (wcmw->phase)
            {
            case WCM_PHASE_SEARCH:
                WCMi_Printf(wcmReportText_WmSyncError, "WM_StartScanEx", wmResult);
                break;

            case WCM_PHASE_SEARCH_TO_IDLE:
                WCMi_Printf(wcmReportText_WmSyncError, "WM_EndScan", wmResult);
                break;
            }
#endif
            WcmSetPhase(WCM_PHASE_IRREGULAR);   // ARM7 �ւ̗v�����s�Ɏ��s( �V�[�P���X�r��Ń��g���C�s�\ )
            WcmNotify(WCM_RESULT_FAILURE, 0, 0, __LINE__);
            break;

        case WM_ERRCODE_ILLEGAL_STATE:
            WCMi_Warning(wcmWarningText_InvalidWmState);

        /* Don't break here */
        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
            WcmNotify(WCM_RESULT_FATAL_ERROR, 0, 0, __LINE__);
        }
        break;

    case WM_ERRCODE_FAILED:
        // �X�L�����v���Ɏ��s�����ꍇ�̓��Z�b�g
        WCMi_Printf(wcmReportText_WmAsyncError, "WM_StartScanEx", cb->errcode, cb->wlCmdID, cb->wlResult);
        WcmWmReset();
        break;

    case WM_ERRCODE_ILLEGAL_STATE:
    case WM_ERRCODE_WM_DISABLE:
        WCMi_Warning(wcmWarningText_InvalidWmState);

    /* Don't break here */
    default:
        WcmSetPhase(WCM_PHASE_FATAL_ERROR);     // �z��͈͊O�̃G���[
        WcmNotify(WCM_RESULT_FATAL_ERROR, 0, 0, __LINE__);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmWmcbEndScan

  Description:  WM ���C�u��������̒ʒm���󂯎��n���h���B
                WM_EndScan �ɑ΂��錋�ʉ������󂯎��B

  Arguments:    arg     -   WM ���C�u��������n�����ʒm�p�����[�^�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmcbEndScan(void* arg)
{
    WMCallback*     cb = (WMCallback*)arg;

    switch (cb->errcode)
    {
    case WM_ERRCODE_SUCCESS:
        WcmSetPhase(WCM_PHASE_IDLE);        // �񓯊��V�[�P���X����I��
        WcmNotify(WCM_RESULT_SUCCESS, 0, 0, __LINE__);
        break;

    case WM_ERRCODE_FAILED:
        // �X�L������~�v���Ɏ��s�����ꍇ�̓��Z�b�g
        WCMi_Printf(wcmReportText_WmAsyncError, "WM_EndScan", cb->errcode, cb->wlCmdID, cb->wlResult);
        WcmWmReset();
        break;

    case WM_ERRCODE_ILLEGAL_STATE:
    case WM_ERRCODE_WM_DISABLE:
        WCMi_Warning(wcmWarningText_InvalidWmState);

    /* Don't break here */
    default:
        WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
        WcmNotify(WCM_RESULT_FATAL_ERROR, 0, 0, __LINE__);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmWmcbConnect

  Description:  WM ���C�u��������̒ʒm���󂯎��n���h���B
                WM_StartConnectEx �ɑ΂��錋�ʉ����A�y�і����ڑ���̔�ؒf�ʒm
                ���󂯎��B

  Arguments:    arg     -   WM ���C�u��������n�����ʒm�p�����[�^�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmcbConnect(void* arg)
{
    WMStartConnectCallback*     cb = (WMStartConnectCallback*)arg;
    WMErrCode   wmResult = WM_ERRCODE_MAX;

    switch (cb->errcode)
    {
    case WM_ERRCODE_SUCCESS:
        switch (cb->state)
        {
        case WM_STATECODE_DISCONNECTED:                     // ��ؒf�ʒm
        case WM_STATECODE_BEACON_LOST:                      // �ؒf���ꂽ�ꍇ�Ɠ����̏������s��
            switch (wcmw->phase)
            {
            case WCM_PHASE_DCF_TO_IDLE:
                // Auth-ID ���N���A
                wcmw->authId = 0;

            /* Don't break here */
            case WCM_PHASE_IDLE_TO_DCF:
                WcmSetPhase(WCM_PHASE_IRREGULAR);
                break;

            case WCM_PHASE_DCF:
                // Auth-ID ���N���A
                wcmw->authId = 0;

                // �ʒm��ʂ�ݒ�
                wcmw->notifyId = WCM_NOTIFY_DISCONNECT;

            /* Don't break here */
            case WCM_PHASE_IRREGULAR:
                WcmWmReset();
                break;

                /* �z��O�̃t�F�[�Y�ł̐ؒf�ʒm�͖������� */
            }
            break;

#if SDK_VERSION_MAJOR > 3 || (SDK_VERSION_MAJOR == 3 && SDK_VERSION_MINOR > 0) || \
        (SDK_VERSION_MAJOR == 3 && SDK_VERSION_MINOR == 0 && SDK_VERSION_RELSTEP >= 20100)

        case WM_STATECODE_DISCONNECTED_FROM_MYSELF:         // ��������̐ؒf�ł͐ؒf�֐����ŏ������Ă���̂ŁA�����Ȃ�
            break;
#endif

        case WM_STATECODE_CONNECT_START:                    // �ڑ��̓r���o�߂Ȃ̂ŁA�����Ȃ�
            break;

        case WM_STATECODE_CONNECTED:                        // �ڑ������ʒm
            if (wcmw->phase == WCM_PHASE_IRREGULAR)
            {
                // �ؒf���ꂽ��ɐڑ������ʒm�������ꍇ�́A���s�Ƃ݂Ȃ����Z�b�g
                WCMi_Printf(wcmReportText_WmDisconnected, "WM_StartConnectEx");
                WcmSetPhase(WCM_PHASE_IDLE_TO_DCF);         // �t�F�[�Y���㏑��
                WcmWmReset();
            }
            else
            {
                // AID �� 1 �` 2007 �܂ł͈̔͂ŗ^������͂�
                if ((WCM_AID_MIN <= cb->aid) && (cb->aid <= WCM_AID_MAX))
                {
                    // Auth-ID ��ޔ�
                    wcmw->authId = cb->aid;

                    // DCM �ʐM���[�h�J�n�v���𔭍s
                    wmResult = WM_StartDCF(WcmWmcbStartDCF, (WMDcfRecvBuf *) (wcmw->recvBuf), WCM_DCF_RECV_BUF_SIZE);
                    switch (wmResult)
                    {
                    case WM_ERRCODE_OPERATING:
                        break;

                    case WM_ERRCODE_FIFO_ERROR:
                        WCMi_Printf(wcmReportText_WmSyncError, "WM_StartDCF", wmResult);
                        WcmSetPhase(WCM_PHASE_IRREGULAR);   // ARM7 �ւ̗v�����s�Ɏ��s( �V�[�P���X�r��Ń��g���C�s�\ )
                        WcmNotify(WCM_RESULT_FAILURE, &(wcmw->bssDesc), 0, __LINE__);
                        break;

                    case WM_ERRCODE_ILLEGAL_STATE:
                        WCMi_Warning(wcmWarningText_InvalidWmState);

                    /* Don't break here */
                    default:
                        WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
                        WcmNotify(WCM_RESULT_FATAL_ERROR, &(wcmw->bssDesc), 0, __LINE__);
                    }
                }
                else
                {
                    // �z��O�� AID ���^����ꂽ�ꍇ�́A���s�Ƃ݂Ȃ����Z�b�g
                    WCMi_Printf(wcmReportText_InvalidAid, "WM_StartConnectEx");
                    WcmWmReset();
                }
            }
            break;

        default:    // �z��O�̃X�e�[�g�R�[�h
            WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
            WcmNotify(WCM_RESULT_FATAL_ERROR, &(wcmw->bssDesc), (void*)(cb->state), __LINE__);
        }
        break;

    case WM_ERRCODE_FAILED:
        wcmw->wlStatusOnConnectFail = cb->wlStatus;

    case WM_ERRCODE_NO_ENTRY:
    case WM_ERRCODE_INVALID_PARAM:
    case WM_ERRCODE_OVER_MAX_ENTRY:
        // �����ڑ��Ɏ��s�����ꍇ�̓��Z�b�g
        WCMi_Printf(wcmReportText_WmAsyncError, "WM_StartConnectEx", cb->errcode, cb->wlCmdID, cb->wlResult);
        WcmSetPhase(WCM_PHASE_IDLE_TO_DCF);     // IRREGULAR �ɂȂ��Ă���\��������̂Ńt�F�[�Y���㏑��
        WcmWmReset();
        break;

    case WM_ERRCODE_ILLEGAL_STATE:
    case WM_ERRCODE_WM_DISABLE:
        WCMi_Warning(wcmWarningText_InvalidWmState);

    /* Don't break here */
    default:
        WcmSetPhase(WCM_PHASE_FATAL_ERROR);     // �z��͈͊O�̃G���[
        WcmNotify(WCM_RESULT_FATAL_ERROR, &(wcmw->bssDesc), 0, __LINE__);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmWmcbDisconnect

  Description:  WM ���C�u��������̒ʒm���󂯎��n���h���B
                WM_Disconnect �ɑ΂��錋�ʉ������󂯎��B

  Arguments:    arg     -   WM ���C�u��������n�����ʒm�p�����[�^�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmcbDisconnect(void* arg)
{
    WMCallback*     cb = (WMCallback*)arg;

    switch (cb->errcode)
    {
    case WM_ERRCODE_SUCCESS:
        if (wcmw->phase == WCM_PHASE_IRREGULAR)
        {
            // �ؒf���ꂽ��ɖ����I�Ȑؒf�v���̊����ʒm�������ꍇ�A�O�̂��߃��Z�b�g
            WCMi_Printf(wcmReportText_WmDisconnected, "WM_Disconnect");
            WcmSetPhase(WCM_PHASE_DCF_TO_IDLE);
            WcmWmReset();
        }
        else
        {
            // Auth-ID ���N���A
            wcmw->authId = 0;

            WcmSetPhase(WCM_PHASE_IDLE);    // �񓯊��V�[�P���X����I��
            WcmNotify(WCM_RESULT_SUCCESS, &(wcmw->bssDesc), 0, __LINE__);
        }
        break;

    case WM_ERRCODE_FAILED:
    case WM_ERRCODE_ILLEGAL_STATE:          // �N���e�B�J���ȃ^�C�~���O�ŒʐM���؂ꂽ�ꍇ�ɕԂ��Ă�����
        // �����ڑ��̐ؒf�v�������s�����ꍇ�̓��Z�b�g
        WCMi_Printf(wcmReportText_WmAsyncError, "WM_Disconnect", cb->errcode, cb->wlCmdID, cb->wlResult);
        WcmSetPhase(WCM_PHASE_DCF_TO_IDLE);
        WcmWmReset();
        break;

    case WM_ERRCODE_WM_DISABLE:
        WCMi_Warning(wcmWarningText_InvalidWmState);

    /* Don't break here */
    default:
        WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
        WcmNotify(WCM_RESULT_FATAL_ERROR, &(wcmw->bssDesc), 0, __LINE__);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmWmcbStartDCF

  Description:  WM ���C�u��������̒ʒm���󂯎��n���h���B
                WM_StartDCF �ɑ΂��錋�ʉ����A�y�� DCF ���[�h�ł̃f�[�^��M�ʒm
                ���󂯎��B

  Arguments:    arg     -   WM ���C�u��������n�����ʒm�p�����[�^�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmcbStartDCF(void* arg)
{
    WMStartDCFCallback*     cb = (WMStartDCFCallback*)arg;

    switch (cb->errcode)
    {
    case WM_ERRCODE_SUCCESS:
        switch (cb->state)
        {
        case WM_STATECODE_DCF_START:
            if (wcmw->phase == WCM_PHASE_IRREGULAR)
            {
                // �ؒf���ꂽ��� DCF �J�n�����ʒm�������ꍇ�́A���s�Ƃ݂Ȃ����Z�b�g
                WCMi_Printf(wcmReportText_WmDisconnected, "WM_StartDCF");
                WcmSetPhase(WCM_PHASE_IDLE_TO_DCF); // �t�F�[�Y���㏑��
                WcmWmReset();
            }
            else
            {
                WcmSetPhase(WCM_PHASE_DCF);         // �񓯊��V�[�P���X����I��
                WcmNotify(WCM_RESULT_SUCCESS, &(wcmw->bssDesc), 0, __LINE__);
            }
            break;

        case WM_STATECODE_DCF_IND:
            // �d�g���x��ޔ�
            WCMi_ShelterRssi((u8) (cb->recvBuf->rate_rssi >> 8));

            // CPS �Ƃ̃C���^�[�t�F�[�X�� DCF ��M��ʒm
            DC_InvalidateRange(cb->recvBuf, WCM_DCF_RECV_BUF_SIZE);
            WCMi_CpsifRecvCallback(cb->recvBuf);
            break;

        default:    // �z��O�̃X�e�[�g�R�[�h
            WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
            WcmNotify(WCM_RESULT_FATAL_ERROR, &(wcmw->bssDesc), (void*)(cb->state), __LINE__);
        }
        break;

    /* StartDCF �ɂ� WM_ERRCODE_FAILED �͕Ԃ��Ă��Ȃ��̂ŏȗ� */
    case WM_ERRCODE_WM_DISABLE:
        WCMi_Warning(wcmWarningText_InvalidWmState);

    /* Don't break here */
    default:
        WcmSetPhase(WCM_PHASE_FATAL_ERROR);     // �z��͈͊O�̃G���[
        WcmNotify(WCM_RESULT_FATAL_ERROR, &(wcmw->bssDesc), 0, __LINE__);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmWmcbEndDCF

  Description:  WM ���C�u��������̒ʒm���󂯎��n���h���B
                WM_EndDCF �ɑ΂��錋�ʉ������󂯎��B

  Arguments:    arg     -   WM ���C�u��������n�����ʒm�p�����[�^�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmcbEndDCF(void* arg)
{
    WMCallback*     cb = (WMCallback*)arg;
    WMErrCode       wmResult = WM_ERRCODE_MAX;

    switch (cb->errcode)
    {
    case WM_ERRCODE_SUCCESS:
        if (wcmw->phase == WCM_PHASE_IRREGULAR)
        {
            // �ؒf���ꂽ��� DCF �I�������ʒm�������ꍇ�́A���s�Ƃ݂Ȃ����Z�b�g
            WcmSetPhase(WCM_PHASE_DCF_TO_IDLE);
            WcmWmReset();
        }
        else
        {
            // AP �Ƃ̖����ڑ��ؒf�v���𔭍s
            wmResult = WM_Disconnect(WcmWmcbDisconnect, 0);
            switch (wmResult)
            {
            case WM_ERRCODE_OPERATING:
                break;

            case WM_ERRCODE_FIFO_ERROR:
                WCMi_Printf(wcmReportText_WmSyncError, "WM_Disconnect", wmResult);
                WcmSetPhase(WCM_PHASE_IRREGULAR);   // ARM7 �ւ̗v�����s�Ɏ��s( �V�[�P���X�r��Ń��g���C�s�\ )
                WcmNotify(WCM_RESULT_FAILURE, &(wcmw->bssDesc), 0, __LINE__);
                break;

            case WM_ERRCODE_ILLEGAL_STATE:          // �N���e�B�J���ȃ^�C�~���O�ŒʐM���؂ꂽ�ꍇ
                // �ؒf���鐡�O�� AP ������ؒf���ꂽ�ꍇ�́A���s�Ƃ݂Ȃ����Z�b�g
                WCMi_Printf(wcmReportText_WmSyncError, "WM_Disconnect", wmResult);
                WcmSetPhase(WCM_PHASE_DCF_TO_IDLE);
                WcmWmReset();
                break;

            default:
                WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
                WcmNotify(WCM_RESULT_FATAL_ERROR, &(wcmw->bssDesc), 0, __LINE__);
            }
        }
        break;

    case WM_ERRCODE_FAILED:
    case WM_ERRCODE_ILLEGAL_STATE:                  // �N���e�B�J���ȃ^�C�~���O�ŒʐM���؂ꂽ�ꍇ
        // DCF �ʐM���[�h�I���Ɏ��s�����ꍇ�̓��Z�b�g
        WCMi_Printf(wcmReportText_WmAsyncError, "WM_EndDCF", cb->errcode, cb->wlCmdID, cb->wlResult);
        WcmSetPhase(WCM_PHASE_DCF_TO_IDLE);         // IRREGULAR �ɂȂ��Ă���\��������̂Ńt�F�[�Y���㏑��
        WcmWmReset();
        break;

    case WM_ERRCODE_WM_DISABLE:
        WCMi_Warning(wcmWarningText_InvalidWmState);

    /* Don't break here */
    default:
        WcmSetPhase(WCM_PHASE_FATAL_ERROR);         // �z��͈͊O�̃G���[
        WcmNotify(WCM_RESULT_FATAL_ERROR, &(wcmw->bssDesc), 0, __LINE__);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmWmcbReset

  Description:  WM ���C�u��������̒ʒm���󂯎��n���h���B
                WM_Reset �ɑ΂��錋�ʉ������󂯎��B

  Arguments:    arg     -   WM ���C�u��������n�����ʒm�p�����[�^�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmWmcbReset(void* arg)
{
    WMCallback*     cb = (WMCallback*)arg;
    WMErrCode       wmResult = WM_ERRCODE_MAX;

    switch (cb->errcode)
    {
    case WM_ERRCODE_SUCCESS:
        // ���Z�b�g�d���Ăяo���Ǘ��t���O���N���A
        wcmw->resetting = WCM_RESETTING_OFF;

        // Auth-ID ���N���A
        wcmw->authId = 0;

        switch (wcmw->phase)
        {
        case WCM_PHASE_IDLE_TO_SEARCH:  // AP �����T���J�n��
        case WCM_PHASE_SEARCH:          // AP �����T����
            // �X�L���������Ɏ��s���ă��Z�b�g���邱�ƂɂȂ����|��ʒm
            WcmSetPhase(WCM_PHASE_IDLE);
            WcmNotify(WCM_RESULT_FAILURE, 0, 0, __LINE__);
            break;

        case WCM_PHASE_SEARCH_TO_IDLE:  // AP �����T����~��
            // �v������ AP �����T����~�����������������Ƃ�ʒm
            WcmSetPhase(WCM_PHASE_IDLE);
            WcmNotify(WCM_RESULT_SUCCESS, 0, 0, __LINE__);
            break;

        case WCM_PHASE_IDLE_TO_DCF:     // �ڑ���
            // �ڑ������Ɏ��s���ă��Z�b�g���邱�ƂɂȂ����|��ʒm
            {
                u16 wlStatus = wcmw->wlStatusOnConnectFail;

                wcmw->wlStatusOnConnectFail = 0x0000;
#ifdef WCM_CAMOUFLAGE_RATESET
                /* ���[�g�Z�b�g�̕s�����ɂ��ڑ��Ɏ��s�������̑΍� */
                if (wlStatus == WCM_CONNECT_STATUSCODE_ILLEGAL_RATESET)
                {
                    if ((wcmw->bssDesc.rateSet.support & WCM_CAMOUFLAGE_RATESET) != WCM_CAMOUFLAGE_RATESET)
                    {
                        WCMi_Printf(wcmReportText_SupportRateset, wcmw->bssDesc.ssid);
                        wcmw->bssDesc.rateSet.support |= WCM_CAMOUFLAGE_RATESET;

                        // �����I�ɐڑ������g���C
                        wmResult = WM_StartConnectEx(WcmWmcbConnect, &(wcmw->bssDesc), NULL,
                                                     ((wcmw->option & WCM_OPTION_MASK_POWER) == WCM_OPTION_POWER_ACTIVE ? FALSE : TRUE),
                                                             ((wcmw->option & WCM_OPTION_MASK_AUTH) == WCM_OPTION_AUTH_SHAREDKEY ?
                                                                     (u16) WM_AUTHMODE_SHARED_KEY : (u16) WM_AUTHMODE_OPEN_SYSTEM
                                                                     ));

                        // �����I�ȏ������ʂ��m�F
                        switch (wmResult)
                        {
                        case WM_ERRCODE_OPERATING:
                            break;

                        case WM_ERRCODE_FIFO_ERROR:
                            WcmSetPhase(WCM_PHASE_IRREGULAR);   // ARM7 �ւ̗v�����s�Ɏ��s( �V�[�P���X�r��Ń��g���C�s�\ )
                            WcmNotify(WCM_RESULT_FAILURE, &(wcmw->bssDesc), (void*)wlStatus, __LINE__);
                            break;

                        case WM_ERRCODE_ILLEGAL_STATE:
                            WCMi_Warning(wcmWarningText_InvalidWmState);

                        /* Don't break here */
                        default:
                            WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
                            WcmNotify(WCM_RESULT_FATAL_ERROR, &(wcmw->bssDesc), (void*)wlStatus, __LINE__);
                        }

                        return;
                    }
                }
#endif
                WcmSetPhase(WCM_PHASE_IDLE);
                WcmNotify(WCM_RESULT_FAILURE, &(wcmw->bssDesc), (void*)wlStatus, __LINE__);
            }
            break;

        case WCM_PHASE_DCF:         // DCF �ʐM��
        case WCM_PHASE_IRREGULAR:
            // AP ����ؒf���ꂽ���Ƃ�ʒm
            WcmSetPhase(WCM_PHASE_IDLE);
            WcmNotify(WCM_RESULT_SUCCESS, &(wcmw->bssDesc), (void*)1, __LINE__);

            break;

        case WCM_PHASE_DCF_TO_IDLE: // �ؒf��
            // �v�������ؒf�����������������Ƃ�ʒm
            WcmSetPhase(WCM_PHASE_IDLE);
            WcmNotify(WCM_RESULT_SUCCESS, &(wcmw->bssDesc), 0, __LINE__);
            break;

        case WCM_PHASE_TERMINATING: // �����I����
            // ���Z�b�g�̊����ɑ����āAPHASE_WAIT �ɖ߂��悤�ɘA���I�ɏ������p��
            wmResult = WM_PowerOff(WcmWmcbCommon);
            switch (wmResult)
            {
            case WM_ERRCODE_OPERATING:
                break;

            case WM_ERRCODE_FIFO_ERROR:
                WCMi_Printf(wcmReportText_WmSyncError, "WM_Reset", wmResult);
                WcmSetPhase(WCM_PHASE_IRREGULAR);   // ARM7 �ւ̗v�����s�Ɏ��s( �V�[�P���X�r��Ń��g���C�s�\ )
                WcmNotify(WCM_RESULT_FAILURE, 0, 0, __LINE__);
                break;

            case WM_ERRCODE_ILLEGAL_STATE:
                WCMi_Warning(wcmWarningText_InvalidWmState);

            /* Don't break here */
            default:
                WcmSetPhase(WCM_PHASE_FATAL_ERROR); // �z��͈͊O�̃G���[
                WcmNotify(WCM_RESULT_FATAL_ERROR, 0, 0, __LINE__);
            }
            break;

        default:
            WcmSetPhase(WCM_PHASE_FATAL_ERROR);     // �z��͈͊O�̃G���[
            WcmNotify(WCM_RESULT_FATAL_ERROR, 0, (void*)(wcmw->phase), __LINE__);
        }
        break;

    default:
        /* ���Z�b�g�Ɏ��s�����ꍇ�͕����s�\ */
        WcmSetPhase(WCM_PHASE_FATAL_ERROR);         // �z��͈͊O�̃G���[
        WcmNotify(WCM_RESULT_FATAL_ERROR, 0, 0, __LINE__);
    }
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/

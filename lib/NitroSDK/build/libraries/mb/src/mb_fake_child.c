/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_fake_child.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_fake_child.c,v $
  Revision 1.19  2007/04/18 04:02:18  yosizaki
  add MB_FakeSetCompareGGIDCallback.

  Revision 1.18  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.17  2006/11/16 10:30:18  takano_makoto
  MB_FakeSetVerboseScanCallback()�֐���ǉ�

  Revision 1.16  2006/03/13 06:33:33  yosizaki
  add MB_FAKESCAN_PARENT_BEACON.

  Revision 1.15  2006/02/20 02:31:03  seiki_masashi
  WMGameInfo.gameNameCount_attribute �� attribute �ɖ��O�ύX

  Revision 1.14  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.13  2005/12/01 09:15:13  takano_makoto
  MB_FAKE_WORK_SIZE�̃T�C�Y�`�F�b�N��ǉ�

  Revision 1.12  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.11  2005/02/21 00:39:34  yosizaki
  replace prefix MBw to MBi.

  Revision 1.10  2005/01/11 07:41:13  takano_makoto
  fix copyright header.

  Revision 1.9  2005/01/11 02:24:33  takano_makoto
  Lock���̐e�̃f�[�^��M���ɁATimeout�J�E���^�����Z�b�g����悤�ɏC��

  Revision 1.8  2005/01/07 02:55:30  takano_makoto
  �f�o�b�O�o�͂�ǉ�

  Revision 1.7  2004/11/29 05:46:01  takano_makoto
  ����M�o�b�t�@�T�C�Y��ύX

  Revision 1.6  2004/11/29 04:53:08  takano_makoto
  WM_StartMP��sendBuffer�� WM_SetMPData��SendBuffer�ɓ����̈悪�g�p����Ă����̂��C���B

  Revision 1.5  2004/11/29 02:55:30  takano_makoto
  32byte align�s��C��

  Revision 1.4  2004/11/25 12:30:28  takano_makoto
  print debug��OFF�ɕύX

  Revision 1.3  2004/11/24 13:00:35  takano_makoto
  �G���[������ǉ�

  Revision 1.2  2004/11/23 23:51:20  takano_makoto
  FINALROM�r���h����Warning������

  Revision 1.1  2004/11/22 12:38:30  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "mb_common.h"
#include "mb_block.h"
#include "mb_child.h"
#include "mb_private.h"
#include "mb_wm_base.h"
#include "mb_fake_child.h"
#include "mb_wm.h"

#define MB_FAKE_PRINT   (0)

#if (MB_FAKE_PRINT == 1)
#define MB_FAKE_OUTPUT    OS_TPrintf
#else
#define MB_FAKE_OUTPUT( ... ) ((void)0)
#endif

#define MY_ROUND(n, a)      (((u32) (n) + (a) - 1) & ~((a) - 1))

typedef struct
{
    u32     sendBuf[WM_SIZE_CHILD_SEND_BUFFER(MB_COMM_PARENT_RECV_MAX, FALSE) / sizeof(u32)] ATTRIBUTE_ALIGN(32);       // ���M�o�b�t�@ 0x400 byte
    u32     recvBuf[WM_SIZE_CHILD_RECEIVE_BUFFER(MB_COMM_PARENT_SEND_MAX, FALSE) / sizeof(u32)] ATTRIBUTE_ALIGN(32);    // ��M�o�b�t�@ 0xF00 byte
    u32     sendDataBuf[WM_SIZE_CHILD_SEND_BUFFER(MB_COMM_PARENT_RECV_MAX, FALSE) / sizeof(u32)] ATTRIBUTE_ALIGN(32);   // ���M�f�[�^�쐬�p�o�b�t�@ 0x400 byte
    WMBssDesc bssDescBuf ATTRIBUTE_ALIGN(32);   // �e�@�X�L�����p�o�b�t�@ �v32�o�C�g�A���C��  0xC0 byte
    WMScanParam scanParam ATTRIBUTE_ALIGN(32);  // �X�L�����p�����[�^     �v32�o�C�g�A���C��  0x20 byte
    MBWMWork wmWork ATTRIBUTE_ALIGN(32);        // MP�ʐM�p���[�N�o�b�t�@�� �v32�o�C�g�A���C��
    MBUserInfo userInfo;               // ���[�U�[���
    u8      _padding1[2];
    MBFakeScanCallbackFunc scanCallback;        // �e�@�X�L�����ʒm�p�R�[���o�b�N
    MBCommCStateCallbackFunc stateCallback;     // ��Ԓʒm�p�R�[���o�b�N
    u32     ggid;                      // ��������GGID
    MbBeaconRecvStatus beaconRecvStatus;        // �r�[�R����M�X�e�[�^�X
    BOOL    scanning;                  // �X�L�������t���O
    BOOL    endScanBusy;               // �X�L�����I���������t���O
    BOOL    locking;                   // ���b�N���t���O
    BOOL    endFlag;                   // MB�̏I�������v��
    u32     targetGgid;                // �v������o�C�i����GGID
    u16     targetFileNo;              // �v������o�C�i����FileNo
    u16     c_comm_state;              // �q�@���
    WMCallbackFunc verboseScanCallback; // ����WMScan�̒ʒm�����[�U�ɒm�点��R�[���o�b�N
    MBFakeCompareGGIDCallbackFunc compareGGIDCallback; // ���[�U��`��GGID��r�R�[���o�b�N
    u8      _padding2[8];
}
MBFakeWork;

SDK_COMPILER_ASSERT(sizeof(MBFakeWork) <= MB_FAKE_WORK_SIZE);


//============================================================================
// �֐��v���g�^�C�v�錾
//============================================================================

static void MBFi_EndComplete(void);

static void MBFi_CommChangeChildState(u16 state, void *arg);
static void MBFi_SendCallback(u16 state, void *arg);
static void MBFi_ErrorCallback(u16 apiid, u16 errcode, BOOL isApiError);

/* scan�p */
static void MBFi_StateInStartScan(void);
static void MBFi_StateOutStartScanParent(void *arg);
static void MBFi_CommBeaconRecvCallback(MbBeaconMsg msg, MBGameInfoRecvList * gInfop, int index);

static void MBFi_ScanCallback(u16 state, MBGameInfoRecvList * gInfop, int index);
static void MBFi_ScanErrorCallback(u16 apiid, u16 errcode);
static void MBFi_ScanLock(u8 *macAddr);
static void MBFi_ScanUnlock(void);
static BOOL RotateChannel(void);

static void MBFi_StateInEndScan(void);
static void MBFi_StateOutEndScan(void *arg);

/* �G���g���[�����p */
static void MBFi_WMCallback(u16 type, void *arg);
static void MBFi_CommChildRecvData(WMPortRecvCallback *cb);
static void MBFi_CommChildSendData(void);

/* �f�o�b�O�o�͗p */
#if (MB_FAKE_PRINT == 1)
static void MBFi_PrintMBCallbackType(u16 type);
static void MBFi_PrintMBCommCallbacyType(u16 type);
#else
#define MBFi_PrintMBCallbackType(a)         ((void)0)
#define MBFi_PrintMBCommCallbackType(a)     ((void)0)
#endif

//============================================================================
// ���[�J���ϐ��錾
//============================================================================

static vu16 mbf_initialize;
static MBFakeWork *mbf_work;



//============================================================================
// �֐���`
//============================================================================


/*---------------------------------------------------------------------------*
  Name:         MB_FakeInit

  Description:  �t�F�C�N�}���`�u�[�g�q�@�̏������֐��B

  Arguments:    buf     �t�F�C�N�q�@�ɕK�v�ȃ��[�N�̈�ւ̃|�C���^�ł��B
                        MB_FakeGetWorkSize()�ŕK�v�ȃT�C�Y���擾�ł��܂��B
                user    �q�@�̃��[�U���ւ̃|�C���^�ł��B
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void MB_FakeInit(void *buf, const MBUserInfo *user)
{
    SDK_NULL_ASSERT(buf);
    SDK_ASSERT(((u32)buf & 0x1F) == 0); // 32�o�C�g�A���C�����`�F�b�N

    if (mbf_initialize)
    {
        OS_TWarning("MB_FakeInit multiply called\n");
        return;
    }
    mbf_initialize = 1;

    MB_FAKE_OUTPUT("MB_Fake Initialized\n");

    // �o�b�t�@�̃N���A
    MI_CpuClear8(buf, sizeof(MBFakeWork));

    mbf_work = (MBFakeWork *) buf;
    mbf_work->c_comm_state = MB_COMM_CSTATE_NONE;
    mbf_work->verboseScanCallback = NULL;
    mbf_work->compareGGIDCallback = NULL;

    MI_CpuCopy8(user, &mbf_work->userInfo, sizeof(MBUserInfo));
    // �r�[�R����M�p���[�N�o�b�t�@��ݒ�
    MBi_SetBeaconRecvStatusBuffer(&mbf_work->beaconRecvStatus);
    // �Q�[������M�X�e�[�^�X��������
    MB_InitRecvGameInfoStatus();
    // �X�L�������b�N�p�֐���ݒ�
    MBi_SetScanLockFunc(MBFi_ScanLock, MBFi_ScanUnlock);
}


/*---------------------------------------------------------------------------*
  Name:         MB_FakeEnd

  Description:  �t�F�C�N�}���`�u�[�g�q�@�̏I���֐��B

  Arguments:    None.
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void MB_FakeEnd(void)
{
    if (mbf_work->endFlag)
    {
        // ���ɏI�������ɓ����Ă���ꍇ�͉������Ȃ�
        return;
    }

    mbf_work->endFlag = TRUE;

    switch (mbf_work->c_comm_state)
    {
    case MB_COMM_CSTATE_NONE:
        // �X�L�������ł���΃X�L�������I��
        if (mbf_work->scanning)
        {
            mbf_work->scanning = FALSE;
        }
        // �X�L�����J�n�O��������EndScan������ł���Ή��������Ɋ���
        else if (!mbf_work->endScanBusy)
        {
            MBFi_EndComplete();
        }
        else
        {
            // �X�L�����I���������ł���Ί�����҂�
        }
        break;

    case MB_COMM_CSTATE_INIT_COMPLETE:
    case MB_COMM_CSTATE_CONNECT:
    case MB_COMM_CSTATE_CONNECT_FAILED:
    case MB_COMM_CSTATE_DISCONNECTED_BY_PARENT:
    case MB_COMM_CSTATE_REQ_REFUSED:
    case MB_COMM_CSTATE_ERROR:
    case MB_COMM_CSTATE_MEMBER_FULL:
        // ���Z�b�g���ďI��
        MBi_WMReset();
        break;

    case MB_COMM_CSTATE_REQ_ENABLE:
    case MB_COMM_CSTATE_DLINFO_ACCEPTED:
    case MB_COMM_CSTATE_RECV_PROCEED:
    case MB_COMM_CSTATE_RECV_COMPLETE:
        // MP���珇�Ɉ��S�ɏI��
        MBi_WMDisconnect();
        break;

    case MB_COMM_CSTATE_BOOTREQ_ACCEPTED:
        // �ؒf������҂�
        break;

    case MB_COMM_CSTATE_BOOT_READY:
    case MB_COMM_CSTATE_CANCELLED:
        // ����IDLE�X�e�[�g�ɗ����Ă���̂ŏI��
        MBFi_EndComplete();
        break;

    default:
        OS_TPanic("MB fake child is in Illegal State\n");
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBFi_EndComplete

  Description:  �t�F�C�N�q�@���C�u�����̊�������

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_EndComplete(void)
{
    mbf_initialize = 0;
    mbf_work->endFlag = 0;
    // �|�[�g�R�[���o�b�N���N���A
    MBi_WMClearCallback();

    // ���[�U�Ɋ����ʒm
    MBFi_CommChangeChildState(MB_COMM_CSTATE_FAKE_END, NULL);
}


/*---------------------------------------------------------------------------*
  Name:         MB_FakeGetWorkSize

  Description:  �t�F�C�N�q�@���C�u�����ɕK�v�ȃ��[�N�T�C�Y���擾���܂��B

  Arguments:    None.

  Returns:      �K�v�ȃ��[�N�o�b�t�@�̃T�C�Y
 *---------------------------------------------------------------------------*/
u32 MB_FakeGetWorkSize(void)
{
    return sizeof(MBFakeWork);
}



/*---------------------------------------------------------------------------*
  Name:         MB_FakeSetCStateCallback

  Description:  �t�F�C�N�q�@�̏�ԑJ�ڂ�ʒm����R�[���o�b�N�֐���ݒ肵�܂��B

  Arguments:    callback    �ݒ肷��R�[���o�b�N�֐�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MB_FakeSetCStateCallback(MBCommCStateCallbackFunc callback)
{
    OSIntrMode enabled = OS_DisableInterrupts();

    mbf_work->stateCallback = callback;

    (void)OS_RestoreInterrupts(enabled);
}


/*---------------------------------------------------------------------------*
  Name:         MBFi_CommChangeChildState

  Description:  �q�@��Ԃ̕ύX����֐��ł��B
                ��Ԃ̕ύX�Ɠ����Ƀ��[�U�v���O�����փR�[���o�b�N��Ԃ��܂��B

  Arguments:    state   �ݒ肷����
                arg     �R�[���o�b�N�ւ̈���

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_CommChangeChildState(u16 state, void *arg)
{
#pragma unused( arg )
    OSIntrMode enabled;

    static const char *const CSTATE_NAME_STRING[] = {
        "MB_COMM_CSTATE_NONE",
        "MB_COMM_CSTATE_INIT_COMPLETE",
        "MB_COMM_CSTATE_CONNECT",
        "MB_COMM_CSTATE_CONNECT_FAILED",
        "MB_COMM_CSTATE_DISCONNECTED_BY_PARENT",
        "MB_COMM_CSTATE_REQ_ENABLE",
        "MB_COMM_CSTATE_REQ_REFUSED",
        "MB_COMM_CSTATE_DLINFO_ACCEPTED",
        "MB_COMM_CSTATE_RECV_PROCEED",
        "MB_COMM_CSTATE_RECV_COMPLETE",
        "MB_COMM_CSTATE_BOOTREQ_ACCEPTED",
        "MB_COMM_CSTATE_BOOT_READY",
        "MB_COMM_CSTATE_CANCELLED",
        "MB_COMM_CSTATE_AUTHENTICATION_FAILED",
        "MB_COMM_CSTATE_MEMBER_FULL",
        "MB_COMM_CSTATE_GAMEINFO_VALIDATED",
        "MB_COMM_CSTATE_GAMEINFO_INVALIDATED",
        "MB_COMM_CSTATE_GAMEINFO_LOST",
        "MB_COMM_CSTATE_GAMEINFO_LIST_FULL",
        "MB_COMM_CSTATE_ERROR",
        "MB_COMM_CSTATE_FAKE_END",
    };

    MB_FAKE_OUTPUT("state %s -> %s\n", CSTATE_NAME_STRING[mbf_work->c_comm_state],
                   CSTATE_NAME_STRING[state]);

    enabled = OS_DisableInterrupts();
    mbf_work->c_comm_state = state;
    (void)OS_RestoreInterrupts(enabled);
    MBFi_SendCallback(state, arg);
}


/*---------------------------------------------------------------------------*
  Name:         MBFi_SendCallback

  Description:  �R�[���o�b�N�𑗐M���܂��B

  Arguments:    state   �t�F�C�N�q�@�̏��
                arg     �R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBFi_SendCallback(u16 state, void *arg)
{
    if (mbf_work->stateCallback == NULL)
    {
        return;
    }
    mbf_work->stateCallback(state, arg);
}

/*---------------------------------------------------------------------------*
  Name:         MBFi_ErrorCallback

  Description:  �G���[�������R�[���o�b�N�Œʒm���܂��B

  Arguments:    apiid   �G���[�̌����ƂȂ���API���R�[���o�b�N�Œʒm���܂��B
                errcode  �G���[�̌����R�[�h���R�[���o�b�N�Œʒm���܂��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBFi_ErrorCallback(u16 apiid, u16 errcode, BOOL isApiError)
{
    MBErrorStatus cb;
    u16     error_type;

    if (mbf_work->stateCallback == NULL)
    {
        return;
    }

    if (isApiError)
    {
        // API�֐��Ăяo�����̃G���[
        switch (errcode)
        {
        case WM_ERRCODE_INVALID_PARAM:
        case WM_ERRCODE_FAILED:
        case WM_ERRCODE_WM_DISABLE:
        case WM_ERRCODE_NO_DATASET:
        case WM_ERRCODE_FIFO_ERROR:
        case WM_ERRCODE_TIMEOUT:
            error_type = MB_ERRCODE_FATAL;
            break;
        case WM_ERRCODE_OPERATING:
        case WM_ERRCODE_ILLEGAL_STATE:
        case WM_ERRCODE_NO_CHILD:
        case WM_ERRCODE_OVER_MAX_ENTRY:
        case WM_ERRCODE_NO_ENTRY:
        case WM_ERRCODE_INVALID_POLLBITMAP:
        case WM_ERRCODE_NO_DATA:
        case WM_ERRCODE_SEND_QUEUE_FULL:
        case WM_ERRCODE_SEND_FAILED:
        default:
            error_type = MB_ERRCODE_WM_FAILURE;
            break;
        }
    }
    else
    {
        // �R�[���o�b�N���̃G���[
        switch (apiid)
        {
        case WM_APIID_INITIALIZE:
        case WM_APIID_SET_LIFETIME:
        case WM_APIID_SET_P_PARAM:
        case WM_APIID_SET_BEACON_IND:
        case WM_APIID_START_PARENT:
        case WM_APIID_START_MP:
        case WM_APIID_SET_MP_DATA:
        case WM_APIID_START_DCF:
        case WM_APIID_SET_DCF_DATA:
        case WM_APIID_DISCONNECT:
        case WM_APIID_START_KS:
            /* �ȏ�̃G���[�́AWM�ŏ������̕K�v�ȃG���[ */
            error_type = MB_ERRCODE_FATAL;
            break;
        case WM_APIID_RESET:
        case WM_APIID_END:
        default:
            /* ���̑��̃G���[�ɂ��ẮA�R�[���o�b�N�G���[�Ƃ��ĕԂ� */
            error_type = MB_ERRCODE_WM_FAILURE;
            break;
        }
    }

    MB_FAKE_OUTPUT("MBFi_ErrorCallback apiid = 0x%x, errcode = 0x%x\n", apiid, errcode);
    cb.errcode = error_type;
    mbf_work->stateCallback(MB_COMM_CSTATE_ERROR, &cb);
}


//============================================================================
// �X�L�����p�֐�
//============================================================================

void MB_FakeSetVerboseScanCallback(WMCallbackFunc callback)
{
    OSIntrMode enabled = OS_DisableInterrupts();

    mbf_work->verboseScanCallback = callback;

    (void)OS_RestoreInterrupts(enabled);
}

/*---------------------------------------------------------------------------*
  Name:         MB_FakeSetCompareGGIDCallback

  Description:  MB�e�@��GGID��r�R�[���o�b�N��ݒ肵�܂��B

  Arguments:    callback    GGID��r�R�[���o�b�N�֐�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MB_FakeSetCompareGGIDCallback(MBFakeCompareGGIDCallbackFunc callback)
{
    OSIntrMode enabled = OS_DisableInterrupts();

    mbf_work->compareGGIDCallback = callback;

    (void)OS_RestoreInterrupts(enabled);
}

/*---------------------------------------------------------------------------*
  Name:         MB_FakeStartScanParent

  Description:  �}���`�u�[�g�e�@�̃X�L�������J�n���܂��B

  Arguments:    callback    �e�@�X�L�����̃R�[���o�b�N��ʒm����֐�
                ggid        ��������MB�e�@��GGID�BGGID���قȂ�Δ�������܂���B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MB_FakeStartScanParent(MBFakeScanCallbackFunc callback, u32 ggid)
{
    MB_FAKE_OUTPUT("%s\n", __func__);

    mbf_work->scanCallback = callback;
    mbf_work->ggid = ggid;
    mbf_work->scanParam.channel = 0;
    mbf_work->scanParam.scanBuf = &mbf_work->bssDescBuf;
    mbf_work->scanning = TRUE;
    mbf_work->locking = FALSE;
    mbf_work->beaconRecvStatus.nowScanTargetFlag = FALSE;
    MBFi_StateInStartScan();
}

/*---------------------------------------------------------------------------*
  Name:         MBFi_StateInStartScan

  Description:  �X�L�����R�}���h�𔭍s���܂��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_StateInStartScan(void)
{
    WMErrCode result;

    // WM_StartScan()���s
    if (mbf_work->locking)
    {
        mbf_work->scanParam.maxChannelTime = MB_SCAN_TIME_LOCKING;
    }
    else
    {
        static const u8 ANY_PARENT[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        WM_CopyBssid(ANY_PARENT, mbf_work->scanParam.bssid);

        mbf_work->scanParam.maxChannelTime = MB_SCAN_TIME_NORMAL;
        // ���b�N���łȂ���΃`�����l���̐ݒ������
        if (!RotateChannel())
        {
            // �������g���Ȃ����
            MBFi_ScanErrorCallback(WM_APIID_MEASURE_CHANNEL, 0);
            return;
        }
    }

    if (mbf_work->scanParam.channel == 0)
    {
        mbf_work->scanParam.channel = 1;
    }
    result = WM_StartScan(MBFi_StateOutStartScanParent, &mbf_work->scanParam);
    if (result != WM_ERRCODE_OPERATING)
    {
        MBFi_ScanErrorCallback(WM_APIID_START_SCAN, result);
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBFi_StateOutStartScanParent

  Description:  WM_StartScan�̃R�[���o�b�N�֐�

  Arguments:    arg     WM_StartScan�̃R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_StateOutStartScanParent(void *arg)
{
    WMStartScanCallback *cb = (WMStartScanCallback *)arg;

    if (WM_ERRCODE_SUCCESS != cb->errcode)
    {
        MBFi_ScanErrorCallback(WM_APIID_START_SCAN, cb->errcode);
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_SCAN_START:
        break;

    case WM_STATECODE_PARENT_FOUND:
        // Scan�Ŏ擾�����e�@���̊i�[
        {
            /*
             * �w�肳�ꂽGGID�ƈ�v���邩, �܂���
             * ���[�U��`�̔�r������TRUE��Ԃ���
             * �r�[�R����͑ΏۂƂ���.
             */
            BOOL    matched = (mbf_work->compareGGIDCallback == NULL) ?
                (cb->gameInfo.ggid == mbf_work->ggid) :
                (*mbf_work->compareGGIDCallback)(cb, mbf_work->ggid);
            if (mbf_work->verboseScanCallback)
            {
                mbf_work->verboseScanCallback(arg);
            }

            if (matched)
            {
                DC_InvalidateRange(&mbf_work->bssDescBuf, WM_BSS_DESC_SIZE);
                (void)MB_RecvGameInfoBeacon(MBFi_CommBeaconRecvCallback, cb->linkLevel,
                                            &mbf_work->bssDescBuf);
                MB_CountGameInfoLifetime(MBFi_CommBeaconRecvCallback, TRUE);
            }
        }
        // don't break;

    case WM_STATECODE_PARENT_NOT_FOUND:
        /* �e�@�Q�[�����̎����J�E���g */
        MB_CountGameInfoLifetime(MBFi_CommBeaconRecvCallback, FALSE);

        if (mbf_work->scanning)
        {
            MBFi_StateInStartScan();
        }
        else
        {
            MBFi_StateInEndScan();
        }
        break;

    default:
        MBFi_ScanErrorCallback(WM_APIID_START_SCAN, WM_ERRCODE_FAILED);
        break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBFi_ScanCallback

  Description:  ���[�U�v���O�����ɑ΂��āA�e�@�̃X�L���������R�[���o�b�N��
                �ʒm���܂��B

  Arguments:    state   �ʒm������
                gInfop  ���������e�@���ւ̃|�C���^
                index   �����ŊǗ����Ă���e�@�̊Ǘ��ԍ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_ScanCallback(u16 state, MBGameInfoRecvList * gInfop, int index)
{
    MBFakeScanCallback cb;

    if (mbf_work->scanCallback == NULL)
    {
        return;
    }

    cb.index = (u16)index;
    if (gInfop != NULL)
    {
        cb.gameInfo = &gInfop->gameInfo;
        cb.bssDesc = &gInfop->bssDesc;
    }
    else
    {
        cb.gameInfo = NULL;
        cb.bssDesc = NULL;
    }
    mbf_work->scanCallback(state, &cb);
}

/*---------------------------------------------------------------------------*
  Name:         MBFi_ScanErrorCallback

  Description:  �X�L�������̃G���[�������R�[���o�b�N�Œʒm���܂��B

  Arguments:    apiid    �G���[�̌����ƂȂ���APIID
                errcode  �G���[�̌����R�[�h

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_ScanErrorCallback(u16 apiid, u16 errcode)
{
    MBFakeScanErrorCallback cb;

    if (mbf_work->scanCallback == NULL)
    {
        return;
    }

    cb.apiid = apiid;
    cb.errcode = errcode;
    mbf_work->scanCallback(MB_FAKESCAN_API_ERROR, &cb);
}

/*---------------------------------------------------------------------------*
  Name:         MBFi_ScanLock

  Description:  �X�L��������e�@�����b�N

  Arguments:    macAddr  : ���b�N����e�@��MAC�A�h���X

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_ScanLock(u8 *macAddr)
{
    MB_FAKE_OUTPUT("Scan Locked\n");
    mbf_work->locking = TRUE;
    WM_CopyBssid(macAddr, mbf_work->scanParam.bssid);
}

/*---------------------------------------------------------------------------*
  Name:         MBFi_ScanUnlock

  Description:  �X�L��������e�@�̃��b�N������

  Arguments:    None.
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_ScanUnlock(void)
{
    MB_FAKE_OUTPUT("Scan Unlocked\n");
    mbf_work->locking = FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         MBi_CommBeaconRecvCallback

  Description:  �q�@�̃r�[�R����M�R�[���o�b�N

  Arguments:    msg    : �r�[�R����M���b�Z�[�W
                gInfop : �e�@�Q�[�����
                index  : �r�[�R���C���f�N�X

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_CommBeaconRecvCallback(MbBeaconMsg msg, MBGameInfoRecvList * gInfop, int index)
{
    switch (msg)
    {
    case MB_BC_MSG_GINFO_VALIDATED:
        MBFi_ScanCallback(MB_FAKESCAN_PARENT_FOUND, (void *)gInfop, index);
        MB_FAKE_OUTPUT("Parent Info validated\n");
        break;
    case MB_BC_MSG_GINFO_INVALIDATED:
        MB_FAKE_OUTPUT("Parent Info invalidate\n");
        break;
    case MB_BC_MSG_GINFO_LOST:
        MBFi_ScanCallback(MB_FAKESCAN_PARENT_LOST, (void *)gInfop, index);
        MB_FAKE_OUTPUT("Parent Info Lost\n");
        break;
    case MB_BC_MSG_GINFO_LIST_FULL:
        MB_FAKE_OUTPUT("Parent List Full\n");
        break;
    case MB_BC_MSG_GINFO_BEACON:
        /* ���m�̐e�@�̃r�[�R�������o���邽�тɒʒm */
        MBFi_ScanCallback(MB_FAKESCAN_PARENT_BEACON, (void *)gInfop, index);
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         RotateChannel

  Description:  �`�����l���̃��[�e�[�V����

  Arguments:    None.

  Returns:      �L���ȃ`�����l����������Ȃ����FALSE��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL RotateChannel(void)
{
    u16     allowedChannel = WM_GetAllowedChannel();
    if (allowedChannel == 0x8000 || allowedChannel == 0)
    {
        return FALSE;
    }

    // �`�����l�������[�e�[�V����
    mbf_work->scanParam.channel++;
    while (TRUE)
    {
        mbf_work->scanParam.channel++;
        if (mbf_work->scanParam.channel > 16)
        {
            mbf_work->scanParam.channel = 1;
        }
        if (allowedChannel & (0x0001 << (mbf_work->scanParam.channel - 1)))
        {
            break;
        }
    }
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MB_FakeEndScan

  Description:  �e�@�̃X�L�����I��

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MB_FakeEndScan(void)
{
    mbf_work->scanning = FALSE;
    mbf_work->endScanBusy = TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MBFi_StateInEndScan

  Description:  �e�@�̃X�L�����I���R�}���h�𔭍s

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_StateInEndScan(void)
{
    WMErrCode result;

    result = WM_EndScan(MBFi_StateOutEndScan);
    if (result != WM_ERRCODE_OPERATING)
    {
        MBFi_ScanErrorCallback(WM_APIID_END_SCAN, result);
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBFi_StateOutEndScan

  Description:  �e�@�̃X�L�����I���R�[���o�b�N

  Arguments:    arg     WM_EndScan�̃R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_StateOutEndScan(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        MBFi_ScanErrorCallback(WM_APIID_END_SCAN, cb->errcode);
        return;
    }
    mbf_work->endScanBusy = FALSE;

    if (mbf_work->endFlag)
    {
        MBFi_EndComplete();
        return;
    }

    MBFi_ScanCallback(MB_FAKESCAN_END_SCAN, NULL, 0);

}



//============================================================================
// �e�@�ւ̃G���g���[����
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         MB_FakeEntryToParent

  Description:  �e�@�ւ̃G���g���[���J�n

  Arguments:    index   �G���g���[����e�@�̓����Ǘ��ԍ��������Ƃ��ė^���܂��B
                        MB_FakeStartScanParent�̃R�[���o�b�N�Ŏ擾����index
                        ��^���Ă��������B

  Returns:      ������index���L���Ȃ��̂ł���� TRUE.
                �L���łȂ���� FALSE.
 *---------------------------------------------------------------------------*/
BOOL MB_FakeEntryToParent(u16 index)
{
    MBGameInfoRecvList *info;

    info = MB_GetGameInfoRecvList(index);

    if (info == NULL)
    {
        return FALSE;
    }
    mbf_work->targetGgid = info->gameInfo.ggid;
    mbf_work->targetFileNo = info->gameInfo.fileNo;

    MBi_WMSetBuffer(&mbf_work->wmWork);
    MBi_WMSetCallback(MBFi_WMCallback);
    MBFi_CommChangeChildState(MB_COMM_CSTATE_INIT_COMPLETE, NULL);
    MI_CpuCopy8(&info->bssDesc, &mbf_work->bssDescBuf, sizeof(WMBssDesc));
    MBi_WMStartConnect(&mbf_work->bssDescBuf);

    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MBFi_WMCallback

  Description:  MP�ʐM�̃R�[���o�b�N�֐�

  Arguments:    type    �R�[���o�b�N�^�C�v
                arg     �R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_WMCallback(u16 type, void *arg)
{
#pragma unused( arg )

    switch (type)
    {
        //--------------------------------------------
        // �e�@�ւ̐ڑ����s�ʒm
    case MB_CALLBACK_CONNECT_FAILED:
        MBFi_CommChangeChildState(MB_COMM_CSTATE_CONNECT_FAILED, NULL);
        break;
        //--------------------------------------------
        // �e�@�ւ̐ڑ������ʒm
    case MB_CALLBACK_CONNECTED_TO_PARENT:
        MB_FAKE_OUTPUT("connect to parent\n");
        MBFi_CommChangeChildState(MB_COMM_CSTATE_CONNECT, arg);
        MBi_ChildStartMP((u16 *)mbf_work->sendBuf, (u16 *)mbf_work->recvBuf);
        break;
        //--------------------------------------------
        // �e�@����̐ؒf�ʒm
    case MB_CALLBACK_DISCONNECTED_FROM_PARENT:
        MBFi_CommChangeChildState(MB_COMM_CSTATE_DISCONNECTED_BY_PARENT, arg);
        break;
        //--------------------------------------------
        // MP�ʐM�J�n�ʒm
    case MB_CALLBACK_MP_STARTED:
        break;
        //-----------------------------
        // �f�[�^���M�\�ʒm
    case MB_CALLBACK_MP_SEND_ENABLE:
        {
            // ���M�f�[�^�̃Z�b�g����
            MBFi_CommChildSendData();
        }
        break;
        //-----------------------------
        // �f�[�^��M�\�ʒm
    case MB_CALLBACK_MP_CHILD_RECV:
        {
            MBFi_CommChildRecvData((WMPortRecvCallback *)arg);
        }
        break;
        //-----------------------------
        // �q�@������̐ؒf�����ʒm
    case MB_CALLBACK_DISCONNECT_COMPLETE:
        if (mbf_work->c_comm_state == MB_COMM_CSTATE_BOOTREQ_ACCEPTED)
        {
            // �u�[�g���N�G�X�g��M��Ȃ�u�[�g������Ԃ֑J��
            MBFi_CommChangeChildState(MB_COMM_CSTATE_BOOT_READY, NULL);
        }
        else
        {
            // �u�[�g���N�G�X�g��M�O�Ȃ�L�����Z����Ԃ֑J��
            MBFi_CommChangeChildState(MB_COMM_CSTATE_CANCELLED, NULL);
        }
        if (mbf_work->endFlag)
        {
            MBFi_EndComplete();
        }
        break;
        //------------------------------
        // MP�ʐM�ʒm
    case MB_CALLBACK_MP_CHILD_SENT:
    case MB_CALLBACK_MP_CHILD_SENT_TIMEOUT:
    case MB_CALLBACK_MP_CHILD_SENT_ERR:
        // �P�Ȃ�MP���x���̒ʐM�󋵂̒ʒm�Ȃ̂œ��ɏ����͕K�v�Ȃ�
        break;
        //------------------------------
        // �G���[�ւ̑Ώ�
    case MB_CALLBACK_API_ERROR:
        {
            MBErrorCallback *cb = (MBErrorCallback *) arg;

            MBFi_ErrorCallback(cb->apiid, cb->errcode, TRUE);
        }
        break;
    case MB_CALLBACK_ERROR:
        {
            MBErrorCallback *cb = (MBErrorCallback *) arg;

            MBFi_ErrorCallback(cb->apiid, cb->errcode, FALSE);
        }
        break;
    default:
        break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBFi_CommChildRecvData

  Description:  �e�@����̃f�[�^��M����

  Arguments:    cb  �|�[�g�R�[���o�b�N�̃R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_CommChildRecvData(WMPortRecvCallback *cb)
{
    MBCommParentBlockHeader hd;

    // �w�b�_���
    (void)MBi_SetRecvBufferFromParent(&hd, (u8 *)cb->data);

//    MBFi_PrintMBCommCallbacyType( hd.type );

    switch (hd.type)                   // ��MBlock�^�C�v�ɂ���ԑJ��
    {
    case MB_COMM_TYPE_PARENT_SENDSTART:
        // �e�@����̑��M�J�n���b�Z�[�W
        if (mbf_work->c_comm_state == MB_COMM_CSTATE_CONNECT)
        {
            MB_FAKE_OUTPUT("Allowd to request file from parent!\n");
            MBFi_CommChangeChildState(MB_COMM_CSTATE_REQ_ENABLE, NULL);
        }
        break;

    case MB_COMM_TYPE_PARENT_KICKREQ: // �e�@�����Kick���b�Z�[�W
        if (mbf_work->c_comm_state == MB_COMM_CSTATE_REQ_ENABLE)
        {
            MBFi_CommChangeChildState(MB_COMM_CSTATE_REQ_REFUSED, NULL);
        }
        break;

    case MB_COMM_TYPE_PARENT_MEMBER_FULL:      // �e�@����̃����o�[���߃��b�Z�[�W
        if (mbf_work->c_comm_state == MB_COMM_CSTATE_REQ_ENABLE)
        {
            MBFi_CommChangeChildState(MB_COMM_CSTATE_MEMBER_FULL, NULL);
        }
        break;

    case MB_COMM_TYPE_PARENT_DL_FILEINFO:
        // MbDownloadFileInfoHeader�̎�M
        if (mbf_work->c_comm_state == MB_COMM_CSTATE_REQ_ENABLE)
        {
            /* ��M����MbDownloadFileInfo�������Ƃ��ēn���B */
            MBFi_CommChangeChildState(MB_COMM_CSTATE_DLINFO_ACCEPTED, NULL);
        }

        break;

    case MB_COMM_TYPE_PARENT_DATA:
        /* �u���b�N�f�[�^�̎�M */
        if (mbf_work->c_comm_state == MB_COMM_CSTATE_DLINFO_ACCEPTED)
        {
            MBFi_CommChangeChildState(MB_COMM_CSTATE_RECV_PROCEED, NULL);
        }

        if (mbf_work->c_comm_state == MB_COMM_CSTATE_RECV_PROCEED)
        {
            MBFi_CommChangeChildState(MB_COMM_CSTATE_RECV_COMPLETE, NULL);      // ��M����
        }
        break;

    case MB_COMM_TYPE_PARENT_BOOTREQ:
        if (mbf_work->c_comm_state == MB_COMM_CSTATE_RECV_COMPLETE)
        {
            MBFi_CommChangeChildState(MB_COMM_CSTATE_BOOTREQ_ACCEPTED, NULL);
        }
        else if (mbf_work->c_comm_state == MB_COMM_CSTATE_BOOTREQ_ACCEPTED)
        {
            MBi_WMDisconnect();        // �ʐM���I��������
        }
        break;
    default:
        break;
    }

    return;
}



/*---------------------------------------------------------------------------*
  Name:         MBi_CommChildSendData

  Description:  �q�@�f�[�^���M

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/

static void MBFi_CommChildSendData(void)
{
    MBCommChildBlockHeader hd;
    WMErrCode errcode = WM_ERRCODE_SUCCESS;
    u16     pollbmp = 0xffff;

    switch (mbf_work->c_comm_state)
    {
    case MB_COMM_CSTATE_REQ_ENABLE:
        {
            MBCommRequestData req_data;
            u8     *databuf;

            req_data.ggid = mbf_work->targetGgid;
            req_data.version = MB_IPL_VERSION;
            req_data.fileid = (u8)mbf_work->targetFileNo;
            MI_CpuCopy8(&mbf_work->userInfo, &req_data.userinfo, sizeof(MBUserInfo));

            hd.type = MB_COMM_TYPE_CHILD_FILEREQ;
            hd.req_data.piece = MBi_SendRequestDataPiece(hd.req_data.data, &req_data);
            databuf = MBi_MakeChildSendBuffer(&hd, (u8 *)mbf_work->sendDataBuf);
            if (databuf == NULL)
            {
                return;
            }
            errcode = MBi_MPSendToParent(MB_COMM_CHILD_HEADER_SIZE, pollbmp, mbf_work->sendDataBuf);
        }
        break;

    case MB_COMM_CSTATE_DLINFO_ACCEPTED:
        // DownloadInfo��̃��b�Z�[�W
        hd.type = MB_COMM_TYPE_CHILD_ACCEPT_FILEINFO;
        (void)MBi_MakeChildSendBuffer(&hd, (u8 *)mbf_work->sendDataBuf);
        errcode = MBi_MPSendToParent(MB_COMM_CHILD_HEADER_SIZE, pollbmp, mbf_work->sendDataBuf);
        break;

    case MB_COMM_CSTATE_RECV_PROCEED:
        break;

    case MB_COMM_CSTATE_RECV_COMPLETE:
        // �u���b�N�]����~���b�Z�[�W(�e����BOOTREQ������܂ő���Â���)
        hd.type = MB_COMM_TYPE_CHILD_STOPREQ;
        (void)MBi_MakeChildSendBuffer(&hd, (u8 *)mbf_work->sendDataBuf);
        errcode = MBi_MPSendToParent(MB_COMM_CHILD_HEADER_SIZE, pollbmp, mbf_work->sendDataBuf);
        break;

    case MB_COMM_CSTATE_BOOTREQ_ACCEPTED:
        hd.type = MB_COMM_TYPE_CHILD_BOOTREQ_ACCEPTED;
        (void)MBi_MakeChildSendBuffer(&hd, (u8 *)mbf_work->sendDataBuf);
        errcode = MBi_MPSendToParent(MB_COMM_CHILD_HEADER_SIZE, pollbmp, mbf_work->sendDataBuf);
        break;
    default:
        // MP�ʐM�m���̂��߁ADUMMY MP�𑗐M
        hd.type = MB_COMM_TYPE_DUMMY;
        (void)MBi_MakeChildSendBuffer(&hd, (u8 *)mbf_work->sendDataBuf);
        errcode = MBi_MPSendToParent(MB_COMM_CHILD_HEADER_SIZE, pollbmp, mbf_work->sendDataBuf);
        break;

    }
}


/*---------------------------------------------------------------------------*
  Name:         MB_FakeGetParentGameInfo

  Description:  �e�@���X�g����e�@�̃Q�[�������擾����B

  Arguments:    index     �e�@�C���f�b�N�X
                pGameInfo �e�@�����擾����̈�ւ̃|�C���^

  Returns:      �L���Ȑe�@�̃C���f�b�N�X�Ȃ�� TRUE
                �����Ȑe�@�̃C���f�b�N�X�Ȃ�� FALSE;
 *---------------------------------------------------------------------------*/
BOOL MB_FakeGetParentGameInfo(u16 index, MBGameInfo *pGameInfo)
{
    MBGameInfoRecvList *parentInfo;
    OSIntrMode enabled;

    if (index >= MB_GAME_INFO_RECV_LIST_NUM)
    {
        return FALSE;
    }

    enabled = OS_DisableInterrupts();

    parentInfo = MB_GetGameInfoRecvList(index);
    if (parentInfo == NULL)
    {
        (void)OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    MI_CpuCopy8(&parentInfo->gameInfo, pGameInfo, sizeof(MBGameInfo));
    (void)OS_RestoreInterrupts(enabled);
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MB_FakeGetParentBssDesc

  Description:  �e�@���X�g����e�@��BssDesc���擾����B

  Arguments:    index �e�@�C���f�b�N�X
                pBssDesc �e�@�����擾����̈�ւ̃|�C���^

  Returns:      �L���Ȑe�@�̃C���f�b�N�X�Ȃ�� TRUE
                �����Ȑe�@�̃C���f�b�N�X�Ȃ�� FALSE;
 *---------------------------------------------------------------------------*/
BOOL MB_FakeGetParentBssDesc(u16 index, WMBssDesc *pBssDesc)
{
    MBGameInfoRecvList *parentInfo;
    OSIntrMode enabled;

    if (index >= MB_GAME_INFO_RECV_LIST_NUM)
    {
        return FALSE;
    }

    enabled = OS_DisableInterrupts();

    parentInfo = MB_GetGameInfoRecvList(index);
    if (parentInfo == NULL)
    {
        (void)OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    MI_CpuCopy8(&parentInfo->bssDesc, pBssDesc, sizeof(WMBssDesc));
    (void)OS_RestoreInterrupts(enabled);
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MB_FakeReadParentBssDesc

  Description:  �e�@���X�g�̐e�@BssDesc�����Ƃ�WM_StartConnect�֐��̂��߂̏����擾���܂��B

  Arguments:    index           �e�@�C���f�b�N�X
                pBssDesc        �e�@�����擾����̈�ւ̃|�C���^
                parent_max_size MP�e�@�̍ő呗�M�T�C�Y
                child_max_size  MP�q�@�̍ő呗�M�T�C�Y
                ks_flag         �L�[�V�F�A�����O�ݒ�t���O
                cs_flag         �A���]���ݒ�t���O

  Returns:      �L���Ȑe�@�̃C���f�b�N�X�Ȃ�� TRUE
                �����Ȑe�@�̃C���f�b�N�X�Ȃ�� FALSE;
 *---------------------------------------------------------------------------*/
BOOL MB_FakeReadParentBssDesc(u16 index, WMBssDesc *pBssDesc, u16 parent_max_size,
                              u16 child_max_size, BOOL ks_flag, BOOL cs_flag)
{
    BOOL    result;

    SDK_NULL_ASSERT(pBssDesc);

    result = MB_FakeGetParentBssDesc(index, pBssDesc);

    if (!result)
    {
        return FALSE;
    }

    pBssDesc->gameInfoLength = 0x10;
    pBssDesc->gameInfo.userGameInfoLength = 0;
    pBssDesc->gameInfo.parentMaxSize = parent_max_size;
    pBssDesc->gameInfo.childMaxSize = child_max_size;
    pBssDesc->gameInfo.attribute = (u8)((ks_flag ? WM_ATTR_FLAG_KS : 0) |
                                        (cs_flag ? WM_ATTR_FLAG_CS : 0) | WM_ATTR_FLAG_ENTRY);
    return TRUE;
}



#if (MB_FAKE_PRINT == 1)

/*---------------------------------------------------------------------------*
  Name:         MBFi_PrintMBCallbackType

  Description:  MB_WM����̃R�[���o�b�N�^�C�v���f�o�b�O�o�͂��܂��B

  Arguments:    type  �R�[���o�b�N�^�C�v

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_PrintMBCallbackType(u16 type)
{
#pragma unused( type )
    static const char *const CALLBACK_NAME[] = {
        "MB_CALLBACK_CHILD_CONNECTED",
        "MB_CALLBACK_CHILD_DISCONNECTED",
        "MB_CALLBACK_MP_PARENT_SENT",
        "MB_CALLBACK_MP_PARENT_RECV",
        "MB_CALLBACK_PARENT_FOUND",
        "MB_CALLBACK_PARENT_NOT_FOUND",
        "MB_CALLBACK_CONNECTED_TO_PARENT",
        "MB_CALLBACK_DISCONNECTED",
        "MB_CALLBACK_MP_CHILD_SENT",
        "MB_CALLBACK_MP_CHILD_RECV",
        "MB_CALLBACK_DISCONNECTED_FROM_PARENT",
        "MB_CALLBACK_CONNECT_FAILED",
        "MB_CALLBACK_DCF_CHILD_SENT",
        "MB_CALLBACK_DCF_CHILD_SENT_ERR",
        "MB_CALLBACK_DCF_CHILD_RECV",
        "MB_CALLBACK_DISCONNECT_COMPLETE",
        "MB_CALLBACK_DISCONNECT_FAILED",
        "MB_CALLBACK_END_COMPLETE",
        "MB_CALLBACK_MP_CHILD_SENT_ERR",
        "MB_CALLBACK_MP_PARENT_SENT_ERR",
        "MB_CALLBACK_MP_STARTED",
        "MB_CALLBACK_INIT_COMPLETE",
        "MB_CALLBACK_END_MP_COMPLETE",
        "MB_CALLBACK_SET_GAMEINFO_COMPLETE",
        "MB_CALLBACK_SET_GAMEINFO_FAILED",
        "MB_CALLBACK_MP_SEND_ENABLE",
        "MB_CALLBACK_PARENT_STARTED",
        "MB_CALLBACK_BEACON_LOST",
        "MB_CALLBACK_BEACON_SENT",
        "MB_CALLBACK_BEACON_RECV",
        "MB_CALLBACK_MP_SEND_DISABLE",
        "MB_CALLBACK_DISASSOCIATE",
        "MB_CALLBACK_REASSOCIATE",
        "MB_CALLBACK_AUTHENTICATE",
        "MB_CALLBACK_SET_LIFETIME",
        "MB_CALLBACK_DCF_STARTED",
        "MB_CALLBACK_DCF_SENT",
        "MB_CALLBACK_DCF_SENT_ERR",
        "MB_CALLBACK_DCF_RECV",
        "MB_CALLBACK_DCF_END",
        "MB_CALLBACK_MPACK_IND",
        "MB_CALLBACK_MP_CHILD_SENT_TIMEOUT",
        "MB_CALLBACK_SEND_QUEUE_FULL_ERR",
        "MB_CALLBACK_API_ERROR",
        "MB_CALLBACK_ERROR",
    };

    MB_FAKE_OUTPUT("RecvCallback %s\n", CALLBACK_NAME[type]);
}

/*---------------------------------------------------------------------------*
  Name:         MBFi_PrintMBCommCallbacyType

  Description:  �e�@����̃p�P�b�g�̃��b�Z�[�W�^�C�v���f�o�b�O�o�͂��܂��B

  Arguments:    type ���b�Z�[�W�^�C�v

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBFi_PrintMBCommCallbacyType(u16 type)
{
#pragma unused( type )
    static const char *const MB_TYPE_STRING[] = {
        "MB_COMM_TYPE_DUMMY",          //      0
        "MB_COMM_TYPE_PARENT_SENDSTART",        //      1
        "MB_COMM_TYPE_PARENT_KICKREQ", //      2
        "MB_COMM_TYPE_PARENT_DL_FILEINFO",      //      3
        "MB_COMM_TYPE_PARENT_DATA",    //      4
        "MB_COMM_TYPE_PARENT_BOOTREQ", //      5
        "MB_COMM_TYPE_PARENT_MEMBER_FULL",      //      6
        "MB_COMM_TYPE_CHILD_FILEREQ",  //      7
        "MB_COMM_TYPE_CHILD_ACCEPT_FILEINFO",   //      8
        "MB_COMM_TYPE_CHILD_CONTINUE", //      9
        "MB_COMM_TYPE_CHILD_STOPREQ",  //      10
        "MB_COMM_TYPE_CHILD_BOOTREQ_ACCEPTED",  //      11
    };

    MB_FAKE_OUTPUT("RECV %s\n", MB_TYPE_STRING[type]);
}

#endif

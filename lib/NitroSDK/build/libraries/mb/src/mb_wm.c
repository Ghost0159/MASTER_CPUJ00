/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_wm.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_wm.c,v $
  Revision 1.8  2005/10/28 11:23:41  seiki_masashi
  WM_STATECODE_DISCONNECTED_FROM_MYSELF �̒ǉ��ɑΉ�
  WM_STATECODE_PORT_INIT �̒ǉ��ɑΉ�

  Revision 1.7  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.6  2005/02/21 00:39:34  yosizaki
  replace prefix MBw to MBi.

  Revision 1.5  2005/01/11 07:41:13  takano_makoto
  fix copyright header.

  Revision 1.4  2005/01/07 02:56:25  takano_makoto
  WM_StartMP�̃R�[���o�b�N�Ŗ��̂Ȃ��G���[�ʒm�𖳎�����悤�ɏC���B

  Revision 1.3  2004/11/24 13:00:02  takano_makoto
  �G���[������ǉ�

  Revision 1.2  2004/11/23 23:50:54  takano_makoto
  Warning������

  Revision 1.1  2004/11/22 12:38:30  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "mb_common.h"
#include "mb_wm.h"
#include "mb_child.h"
#include "mb_wm_base.h"
#include "mb_block.h"

//===========================================================================
// �v���g�^�C�v�錾
//===========================================================================

static BOOL IsSendEnabled(void);
static void MBi_WMStateOutStartConnect(void *arg);
static void ChildStateOutStartMP(void *arg);
static void ChildPortCallback(void *arg);
static void StateOutMPSendToParent(void *arg);

static void MBi_WMStateOutStartConnect(void *arg);

static void MBi_WMStateOutEndMP(void *arg);

static void MBi_WMStateOutDisconnect(void *arg);
static void MBi_WMStateInDisconnect(void);

static void MBi_WMStateOutReset(void *arg);

static void MBi_WMSendCallback(u16 type, void *arg);
static void MBi_WMErrorCallback(u16 apiid, u16 error_code);
static void MBi_WMApiErrorCallback(u16 apiid, u16 error_code);


//===========================================================================
// �ϐ��錾
//===========================================================================

static MBWMWork *wmWork = NULL;


//===========================================================================
// �֐���`
//===========================================================================

/*---------------------------------------------------------------------------*
  Name:         MBi_WMSetBuffer

  Description:  MB_WM��MP�ʐM�̂��߂Ɏg�p����o�b�t�@��ݒ肵�܂��B
                MBWMWork�̃T�C�Y�����^����K�v������܂��B

  Arguments:    buf     ���[�N�o�b�t�@�Ƃ��Ďg�p����̈�ւ̃|�C���^

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_WMSetBuffer(void *buf)
{
    SDK_NULL_ASSERT(buf);
    SDK_ASSERT(((u32)buf & 0x1f) == 0); // 32�o�C�g�A���C������Ă��邩�ǂ���

    wmWork = (MBWMWork *) buf;
    wmWork->start_mp_busy = 0;         // �e�@��StartMP���d�Ăяo���h�~
    wmWork->mpStarted = 0;
    wmWork->child_bitmap = 0;
    wmWork->mpBusy = 0;
    wmWork->endReq = 0;
    wmWork->sendBuf = NULL;
    wmWork->recvBuf = NULL;
    wmWork->mpCallback = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMSetCallback

  Description:  �R�[���o�b�N��ݒ肵�܂��B

  Arguments:    callback    �R�[���o�b�N�֐�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_WMSetCallback(MBWMCallbackFunc callback)
{
    OSIntrMode enabled = OS_DisableInterrupts();

    wmWork->mpCallback = callback;

    (void)OS_RestoreInterrupts(enabled);
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMStartConnect

  Description:  �e�@�ւ̐ڑ��J�n

  Arguments:    bssDesc  �ڑ�����e�@��BssDesc

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_WMStartConnect(WMBssDesc *bssDesc)
{
    WMErrCode result;

    wmWork->mpStarted = 0;
    wmWork->endReq = 0;

    wmWork->sendBufSize = (u16)WM_SIZE_MP_CHILD_SEND_BUFFER(bssDesc->gameInfo.childMaxSize, FALSE);
    wmWork->recvBufSize =
        (u16)WM_SIZE_MP_CHILD_RECEIVE_BUFFER(bssDesc->gameInfo.parentMaxSize, FALSE);
    wmWork->pSendLen = bssDesc->gameInfo.parentMaxSize;
    wmWork->pRecvLen = bssDesc->gameInfo.childMaxSize;
    wmWork->blockSizeMax = (u16)MB_COMM_CALC_BLOCK_SIZE(wmWork->pSendLen);
    MBi_SetChildMPMaxSize(wmWork->pRecvLen);

    result = WM_StartConnect(MBi_WMStateOutStartConnect, bssDesc, NULL);

    if (result != WM_ERRCODE_OPERATING)
    {
        MBi_WMSendCallback(MB_CALLBACK_CONNECT_FAILED, NULL);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMStateOutStartConnect

  Description:  �e�@�ւ̐ڑ��R�[���o�b�N

  Arguments:    arg     WM_StartConnect�̃R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBi_WMStateOutStartConnect(void *arg)
{
    WMStartConnectCallback *cb = (WMStartConnectCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        MBi_WMSendCallback(MB_CALLBACK_CONNECT_FAILED, arg);
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_BEACON_LOST:
        break;
    case WM_STATECODE_CONNECT_START:
        break;
    case WM_STATECODE_DISCONNECTED:
        MBi_WMSendCallback(MB_CALLBACK_DISCONNECTED_FROM_PARENT, NULL);
        break;
    case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
        // ����ؒf�����ꍇ�͏������Ȃ�
        break;
    case WM_STATECODE_CONNECTED:
        // �F�؏I�����B
        MBi_WMSendCallback(MB_CALLBACK_CONNECTED_TO_PARENT, arg);
        break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBi_ChildStartMP

  Description:  MP�ʐM�J�n

  Arguments:    sendBuf     ���M�o�b�t�@�Ƃ��Đݒ肷��̈�ւ̃|�C���^
                recvBuf     ��M�o�b�t�@�Ƃ��Đݒ肷��̈�ւ̃|�C���^

  Returns:      �G���[�R�[�h�A���펞��WM_ERRCODE_OPERATING.
 *---------------------------------------------------------------------------*/
void MBi_ChildStartMP(u16 *sendBuf, u16 *recvBuf)
{
    WMErrCode result;

    wmWork->sendBuf = (u32 *)sendBuf;
    wmWork->recvBuf = (u32 *)recvBuf;

    result = WM_SetPortCallback(WM_PORT_BT, ChildPortCallback, NULL);
    if (result != WM_ERRCODE_SUCCESS)
    {
        MBi_WMApiErrorCallback(WM_APIID_START_MP, result);
        return;
    }

    result = WM_StartMPEx(ChildStateOutStartMP, recvBuf, wmWork->recvBufSize, sendBuf, wmWork->sendBufSize, 1,  // mpFreq
                          0,           // defaultRetryCount
                          FALSE,       // minPollBmpMode
                          FALSE,       // singlePacketMode
                          TRUE,        // fixFrameMode
                          TRUE);       // ignoreFatalError

    if (result != WM_ERRCODE_OPERATING)
    {
        MBi_WMApiErrorCallback(WM_APIID_START_MP, result);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ChildStateOutStartMP

  Description:  �q�@ WM_StartMPEx�R�[���o�b�N�֐�

  Arguments:    arg     WM_StartMP�̃R�[���o�b�N����

  Returns:      �G���[�R�[�h�A���펞��WM_ERRCODE_OPERATING.
 *---------------------------------------------------------------------------*/
static void ChildStateOutStartMP(void *arg)
{
    WMStartMPCallback *cb = (WMStartMPCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        // �G���[�����̕K�v�̖����G���[�ʒm�̏ꍇ�͏I��
        if (cb->errcode == WM_ERRCODE_SEND_FAILED)
        {
            return;
        }
        else if (cb->errcode == WM_ERRCODE_TIMEOUT)
        {
            return;
        }
        else if (cb->errcode == WM_ERRCODE_INVALID_POLLBITMAP)
        {
            return;
        }

        MBi_WMErrorCallback(cb->apiid, cb->errcode);
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_MP_START:
        wmWork->mpStarted = 1;         // MP�J�n�ς݃t���O���Z�b�g
        wmWork->mpBusy = 0;
        wmWork->child_bitmap = 0;
        MBi_WMSendCallback(MB_CALLBACK_MP_STARTED, NULL);
        {
            // MP���M���R�[���o�b�N
            MBi_WMSendCallback(MB_CALLBACK_MP_SEND_ENABLE, NULL);
        }
        break;

    case WM_STATECODE_MP_IND:
        // None.
        break;

    case WM_STATECODE_MPACK_IND:
        // None.
        break;

    case WM_STATECODE_MPEND_IND:      // �e�@�݂̂ɔ���
    default:
        MBi_WMErrorCallback(cb->apiid, WM_ERRCODE_FAILED);
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBi_WMDisconnect

  Description:  �q�@��MP�ؒf�����BWM_EndMP������ɐe�@����Disconnect���܂��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_WMDisconnect(void)
{
    WMErrCode result;

    wmWork->endReq = 1;

    result = WM_EndMP(MBi_WMStateOutEndMP);

    if (result != WM_ERRCODE_OPERATING)
    {
        wmWork->endReq = 0;
        MBi_WMApiErrorCallback(WM_APIID_END_MP, result);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMStateOutEndMP

  Description:  WM_EndMP�̃R�[���o�b�N�֐�

  Arguments:    arg     WM_EndMP�̃R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBi_WMStateOutEndMP(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        wmWork->endReq = 0;
        MBi_WMErrorCallback(cb->apiid, cb->errcode);
        return;
    }

    wmWork->mpStarted = 0;
    MBi_WMStateInDisconnect();
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMStateInDisconnect

  Description:  �q�@��e�@����ؒf���AIDLE��Ԃ֑J�ڂ����܂��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBi_WMStateInDisconnect(void)
{
    WMErrCode result;

    result = WM_Disconnect(MBi_WMStateOutDisconnect, 0);

    if (result != WM_ERRCODE_OPERATING)
    {
        wmWork->endReq = 0;
        MBi_WMApiErrorCallback(WM_APIID_DISCONNECT, result);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMStateInDisconnect

  Description:  WM_Disconnect�̃R�[���o�b�N����

  Arguments:    arg  WM_Disconnect�̃R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBi_WMStateOutDisconnect(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    wmWork->endReq = 0;
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        MBi_WMErrorCallback(cb->apiid, cb->errcode);
        return;
    }
    MBi_WMSendCallback(MB_CALLBACK_DISCONNECT_COMPLETE, NULL);
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMReset

  Description:  �q�@�̂����Z�b�g���AIDLE�X�e�[�g�ɑJ�ڂ����܂��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_WMReset(void)
{
    WMErrCode result;

    result = WM_Reset(MBi_WMStateOutReset);
    if (result != WM_ERRCODE_OPERATING)
    {
        MBi_WMApiErrorCallback(WM_APIID_RESET, result);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMStateOutReset

  Description:  �q�@�̂����Z�b�g���AIDLE�X�e�[�g�ɑJ�ڂ����܂��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBi_WMStateOutReset(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        MBi_WMErrorCallback(cb->apiid, cb->errcode);
        return;
    }
    // Reset �͎��̏�Ԃ��J�n�����A�A�C�h�����O�i�ҋ@���j��Ԃɂ��܂��B
    MBi_WMSendCallback(MB_CALLBACK_DISCONNECT_COMPLETE, NULL);
}


/*
 * MP���M���`�F�b�N
   
   SetMP��A�R�[���o�b�N���Ԃ�O�ɁA�Ă�MP���Z�b�g���Ă��܂�Ȃ��悤�ɁA
   SetMP���s���ɗ��Ă�AmpBusy�̃t���O�𔻒�v�f�ɒǉ��B
 
 */
/*---------------------------------------------------------------------------*
  Name:         IsSendEnabled

  Description:  ���݁A�V����MP�f�[�^���Z�b�g���Ă��ǂ����ǂ����𔻒肷��֐��ł��B
                SetMP��A�R�[���o�b�N���Ԃ�O�ɁA�Ă�MP���Z�b�g���Ă��܂�Ȃ��悤�ɁA
                SetMP���s���ɗ��Ă�AmpBusy�̃t���O�𔻒�v�f�ɒǉ����܂����B

  Arguments:    None.

  Returns:      �V�����f�[�^���Z�b�g���Ă��\��Ȃ��Ȃ�� TRUE
                �����łȂ��ꍇ�� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL IsSendEnabled(void)
{
    return (wmWork->mpStarted == 1) && (wmWork->mpBusy == 0) && (wmWork->endReq == 0);
}


/*---------------------------------------------------------------------------*
  Name:         ChildPortCallback

  Description:  �q�@MP�|�[�g�R�[���o�b�N�֐�

  Arguments:    arg     MP�ʐM�̃|�[�g�R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ChildPortCallback(void *arg)
{
    WMPortRecvCallback *cb = (WMPortRecvCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_PORT_RECV:
        // �f�[�^��M��ʒm
        MBi_WMSendCallback(MB_CALLBACK_MP_CHILD_RECV, cb);
        break;
    case WM_STATECODE_CONNECTED:
        // �ڑ��ʒm
        break;
    case WM_STATECODE_PORT_INIT:
    case WM_STATECODE_DISCONNECTED:
    case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
        break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBi_MPSendToParent

  Description:  �o�b�t�@���e��e�@�֑��M���܂��B

  Arguments:    body_len �f�[�^�T�C�Y
                pollbmp  ���M����̃|�[���r�b�g�}�b�v(�q�@�̏ꍇ�ɂ͊֌W����܂���)
                sendbuf  ���M�f�[�^�ւ̃|�C���^
  Returns:      ���M�����̊J�n�ɐ��������Ȃ�΁AWM_ERRCODE_OPERATING�A
                ���s������Ȃ�΁A����ȊO�̃R�[�h���Ԃ��Ă��܂��B
 *---------------------------------------------------------------------------*/
WMErrCode MBi_MPSendToParent(u32 body_len, u16 pollbmp, u32 *sendbuf)
{
    WMErrCode result;

    // 32�o�C�g�A���C���`�F�b�N
    SDK_ASSERT(((u32)sendbuf & 0x1F) == 0);

    DC_FlushRange(sendbuf, sizeof(body_len));
    DC_WaitWriteBufferEmpty();

    if (!IsSendEnabled())
    {
        return WM_ERRCODE_FAILED;
    }

    result = WM_SetMPDataToPort(StateOutMPSendToParent,
                                (u16 *)sendbuf,
                                (u16)body_len, pollbmp, WM_PORT_BT, WM_PRIORITY_LOW);
    if (result != WM_ERRCODE_OPERATING)
    {
        return result;
    }

    wmWork->mpBusy = 1;
    return WM_ERRCODE_OPERATING;
}


/*---------------------------------------------------------------------------*
  Name:         StateOutMPSendToParent

  Description:  MP���M�����ʒm�R�[���o�b�N

  Arguments:    arg     WM_SetMPDataToPort�̃R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void StateOutMPSendToParent(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    wmWork->mpBusy = 0;
    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        MBi_WMSendCallback(MB_CALLBACK_MP_CHILD_SENT, arg);
    }
    else if (cb->errcode == WM_ERRCODE_TIMEOUT)
    {
        MBi_WMSendCallback(MB_CALLBACK_MP_CHILD_SENT_TIMEOUT, arg);
    }
    else
    {
        MBi_WMSendCallback(MB_CALLBACK_MP_CHILD_SENT_ERR, arg);
    }
    // ���̑��M������
    MBi_WMSendCallback(MB_CALLBACK_MP_SEND_ENABLE, NULL);
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMSendCallback

  Description:  WM���C���[�ł̃R�[���o�b�N�ʒm���s�Ȃ��܂��B

  Arguments:    type        �R�[���o�b�N�^�C�v
                arg         �R�[���o�b�N����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBi_WMSendCallback(u16 type, void *arg)
{
    if (wmWork->mpCallback == NULL)
    {
        return;
    }
    wmWork->mpCallback(type, arg);
}

/*---------------------------------------------------------------------------*
  Name:         MBi_WMErrorCallback

  Description:  WM���C���[�ł̃G���[�ʒm���s�Ȃ��܂��B

  Arguments:    apiid       �����ƂȂ���WM_APIID
                error_code  �G���[�R�[�h

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBi_WMErrorCallback(u16 apiid, u16 error_code)
{
    MBErrorCallback arg;

    if (wmWork->mpCallback == NULL)
    {
        return;
    }

    arg.apiid = apiid;
    arg.errcode = error_code;

    wmWork->mpCallback(MB_CALLBACK_ERROR, &arg);
}

/*---------------------------------------------------------------------------*
  Name:         MBi_WMApiErrorCallback

  Description:  WM��API�R�[���̖߂�l�ŃG���[�����������ꍇ�̒ʒm���s�Ȃ��܂��B

  Arguments:    apiid       �����ƂȂ���WM_APIID
                error_code  �G���[�R�[�h

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBi_WMApiErrorCallback(u16 apiid, u16 error_code)
{
    MBErrorCallback arg;

    if (wmWork->mpCallback == NULL)
    {
        return;
    }

    arg.apiid = apiid;
    arg.errcode = error_code;

    wmWork->mpCallback(MB_CALLBACK_API_ERROR, &arg);
}


/*---------------------------------------------------------------------------*
  Name:         MBi_WMApiErrorCallback

  Description:  WM��API�R�[���̖߂�l�ŃG���[�����������ꍇ�̒ʒm���s�Ȃ��܂��B

  Arguments:    apiid       �����ƂȂ���WM_APIID
                error_code  �G���[�R�[�h

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_WMClearCallback(void)
{
    (void)WM_SetPortCallback(WM_PORT_BT, NULL, NULL);
}

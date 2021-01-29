/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_close.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_close.c,v $
  Revision 1.22  2006/05/18 11:35:44  yasu
  ���荞�݋֎~��Ԃ̏C��

  Revision 1.21  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.20  2005/10/14 06:48:19  yasu
  ����p�C�v�ւ� close �ʒm���� response �L���[���N���A���Ă��Ȃ������̂��C��

  Revision 1.19  2005/10/14 03:48:57  yasu
  warning �΍�

  Revision 1.18  2005/10/14 01:56:53  yasu
  �R�}���h�L���[����t�������Ƃ��� Close �������s���S�ȏ�ԂɂȂ邱�Ƃ��C��

  Revision 1.17  2005/09/29 03:05:50  yasu
  Unregister/Register �֘A�̃o�O FIX

  Revision 1.16  2005/09/28 11:44:37  yasu
  �R�[�h�𐮗�
  SOCL_CalmDown() ��ǉ�
  CPS ���C�u�����̐����Ή����܂��Ȃ̂� SOC ���ŏ������J�o�[���Ă���

  Revision 1.15  2005/09/28 04:11:56  kitase_hirotake
  Write �̏I����҂悤�ɏC��

  Revision 1.14  2005/09/27 14:18:09  yasu
  SOC_Close �̔񓯊�����T�|�[�g

  Revision 1.13  2005/09/19 15:11:37  yasu
  Closing ��Ԃ�ǉ����������̔�����

  Revision 1.12  2005/09/19 14:08:29  yasu
  Closing ��Ԃ�ǉ�����

  Revision 1.11  2005/09/01 06:42:55  yasu
  �\�P�b�g�������Ă��邩�ǂ����̔�����ꌳ��

  Revision 1.10  2005/09/01 06:12:20  yasu
  �\�P�b�g�������Ă��邩�ǂ����̔�����ꌳ��

  Revision 1.9  2005/08/25 08:27:38  yasu
  UDP ��M�ɂ�����o�b�t�@�����O�����ǉ�

  Revision 1.8  2005/08/24 09:25:13  yasu
  SOCL_SocketIsInvalid �ǉ�

  Revision 1.7  2005/08/19 05:06:16  yasu
  DEBUG �r���h���ɕ����\�P�b�g���̃f�[�^��h��Ԃ��Ă���

  Revision 1.6  2005/08/18 13:52:11  yasu
  close �������� send �p�C�v�Ƃ̓��������

  Revision 1.5  2005/08/18 13:18:49  yasu
  �\�P�b�g�����N���X�g�ɂ�� cleanup �����̒ǉ�

  Revision 1.4  2005/08/18 08:53:36  yasu
  CPS �֐��ł� Close ���ɖ����I�� Shutdown ���Ăяo���K�v������_�ɑΉ�

  Revision 1.3  2005/07/30 22:30:14  yasu
  �f���������悤�ɏC��

  Revision 1.2  2005/07/30 15:30:52  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.1  2005/07/22 12:44:56  yasu
  �񓯊�����������

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>

static int  SOCLi_CloseCallBack(void* arg);
static void SOCLi_FreeCommandPipe(SOCLiSocketCommandPipe* pipe);

/*---------------------------------------------------------------------------*
  Name:         SOCL_IsClosed

  Description:  �\�P�b�g�� Close ���ꂽ���ǂ������肷��
                SOCL_Close �͔񓯊��I�ɃN���[�Y�������s�Ȃ��D
                ���̏����������������ǂ������̊֐��Ŕ���ł���D
  
  Arguments:    s		�\�P�b�g
  
  Returns:      TRUE : �N���[�Y���ꂽ
                FALSE: �s���ȃ\�P�b�g�܂��̓N���[�Y����Ă��Ȃ�
 *---------------------------------------------------------------------------*/
int SOCL_IsClosed(int s)
{
    SOCLSocket*     socket = (SOCLSocket*)s;

    // �K����̃n���h���l�ł���
    // �\�P�b�g���L���ȃG���g���łȂ��������p���G���g���ɂ��Ȃ��Ȃ�N���[�Y�����Ƃ݂Ȃ�
    if ((int)socket >= 0 && SOCL_SocketIsInvalid(socket) && !SOCL_SocketIsInTrash(socket))
    {
        return TRUE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_Close

  Description:  �\�P�b�g�� Close �������s�Ȃ��D
  
  Arguments:    s		�\�P�b�g
  
  Returns:      0  : ���� (��x�ڂ� Close �Ăяo���� Close �������� 0 ���Ԃ�)
                �� : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                  SOCL_EINPROGRESS �N���[�Y�������p����
 *---------------------------------------------------------------------------*/
int SOCL_Close(int s)
{
    SOCLSocket*         socket = (SOCLSocket*)s;
    SOCLiCommandPacket*     command;

    // �K��O�̒l�Ȃ�G���[
    if ((int)socket <= 0)
    {
        return SOCL_EINVAL;
    }

    // �\�P�b�g���p���G���g���ɂ���Ȃ�N���[�Y���Ƃ݂Ȃ�
    if (SOCL_SocketIsInTrash(socket))
    {
        return SOCL_EINPROGRESS;
    }

    // �\�P�b�g���G���g���ɂȂ��Ȃ���ɃN���[�Y���ꂽ�Ƃ݂Ȃ�
    if (SOCL_SocketIsInvalid(socket))
    {
        return SOCL_ESUCCESS;
    }

    if (!SOCL_SocketIsCreated(socket))
    {
        return SOCL_ENETRESET;  // ����������Ă��Ȃ�
    }

    // �\�P�b�g�̃t���O�� WAIT_CLOSE �Ȃ�N���[�Y��
    if (SOCL_SocketIsWaitingClose(socket))
    {
        return SOCL_EINPROGRESS;
    }

    socket->state |= (SOCL_STATUS_CLOSING | SOCL_STATUS_WAIT_CLOSE);

    // TCP �Ȃ瑗�M�X���b�h�� NULL �R�}���h�𓊂��A�S�Ẵf�[�^�𑗐M��A���M�X���b�h���I��������
    // NULL �R�}���h�� BLOCK �œ�������
    if (SOCL_SocketIsTCP(socket))
    {
        (void)SOCLi_SendCommandPacket(&socket->send_pipe->h, NULL);
    }

    // ����X���b�h�ɃR�}���h�𓊂��A�R�[���o�b�N�֐������� CPS_Close ��
    // �N������D�R�}���h�̑��M�Ɋւ��Ă� BLOCK �ōs�Ȃ��� CLOSE �������̂̏I���͑҂��Ȃ�
    command = SOCLi_CreateCommandPacket(SOCLi_CloseCallBack, socket, SOCL_FLAGBLOCK_BLOCK);
    command->h.response = NULL; // Close �����I���ʒm�͕K�v�Ȃ�
    (void)SOCLi_SendCommandPacketToCtrlPipe(socket, command);

    // ��x�ڂ̃N���[�Y�� 0 ��Ԃ��D
    return SOCL_ESUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_CloseCallBack

  Description:  Close �����̃R�[���o�b�N
  
  Arguments:    arg  �R�}���h�u���b�N�ւ̃|�C���^
  
  Returns:      SOCLi_ExecCommand* �ɓn�����l(BLOCK���[�h�̂Ƃ�)
 *---------------------------------------------------------------------------*/
static int SOCLi_CloseCallBack(void* arg)
{
    SOCLiCommandClose*  cpacket = (SOCLiCommandClose*)arg;
    SOCLSocket*         socket = (SOCLSocket*)cpacket->h.socket;
    OSIntrMode          enable;

    if (SOCL_SocketIsTCP(socket))
    {
        // send pipe ���̃f�[�^�����M�����܂ő҂�
        OS_JoinThread(&socket->send_pipe->h.thread);

        // �\�P�b�g�̃N���[�Y����
        CPS_TcpShutdown();
        CPS_TcpClose();
        CPS_SocRelease();
    }

    CPS_SocUnRegister();

    socket->state &= ~(SOCL_STATUS_CONNECTING | SOCL_STATUS_CONNECTED);

    // ���̃X���b�h�ɑ΂��ďI���ʒm���s�Ȃ��D���̎��_�ł̓L���[���󂢂Ă���̂Ńf�b�h���b�N�͋N����Ȃ�
    (void)SOCLi_SendCommandPacket(SOCL_SocketIsUDPSend(socket) ? &socket->send_pipe->h : &socket->recv_pipe->h, NULL);

    // �X���b�h��p�����X�g�ɍڂ���
    enable = OS_DisableInterrupts();
    SOCLi_SocketUnregister(socket);
    SOCLi_SocketRegisterTrash(socket);
    (void)OS_RestoreInterrupts(enable);

    socket->state |= SOCL_STATUS_WAIT_RELEASE;

    return SOCL_ESUCCESS;   // ����
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_CleanupSocket

  Description:  �\�P�b�g�p�̗̈���J������
                �ȉ��̗̈�����ɊJ�����Ă���
  
                �̈�
                --------------------------------------------------------
                ���M�p�X���b�h�̃o�b�t�@
                ��M�p�X���b�h�̃o�b�t�@
                CPS  �\�P�b�g�p�̃o�b�t�@
                SOCL �\�P�b�g�̈�

  Arguments:    socket  �\�P�b�g�p�����[�^
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOCLi_CleanupSocket(SOCLSocket* socket)
{
    OSIntrMode  enable;

    if (socket)
    {
        socket->state = 0;

        // �e�p�[�c�����ɊJ������
        if (SOCL_SocketIsTCP(socket))
        {
            SOCLi_FreeCommandPipe(&socket->send_pipe->h);
            SOCLi_FreeCommandPipe(&socket->recv_pipe->h);
        }
        else if (SOCL_SocketIsUDP(socket))
        {
            // UDP ��M�f�[�^���J�����Ă���
            SOCLiSocketUdpData*     udpdata = socket->recv_pipe->udpdata.out;
            SOCLiSocketUdpData*     udpdata_next;

            while (udpdata)
            {
                udpdata_next = udpdata->next;
                SOCLi_Free(udpdata);
                udpdata = udpdata_next;
            }

            socket->recv_pipe->udpdata.size = 0;
            socket->recv_pipe->udpdata.in = NULL;
            socket->recv_pipe->udpdata.out = NULL;
            OS_WakeupThread(&socket->recv_pipe->udpdata.waiting);

            SOCLi_FreeCommandPipe(&socket->recv_pipe->h);
        }
        else if (SOCL_SocketIsUDPSend(socket))
        {
            SOCLi_FreeCommandPipe(&socket->send_pipe->h);
        }

        enable = OS_DisableInterrupts();
        SOCLi_SocketUnregister(socket);
        SOCLi_SocketUnregisterTrash(socket);

#ifdef SDK_DEBUG
        MI_CpuFill8(socket, 0xaa, sizeof(*socket));
#endif

        SOCLi_Free(socket);
        (void)OS_RestoreInterrupts(enable);
    }

    return;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_FreeCommandPipe

  Description:  �\�P�b�g�̃R�}���h�p�C�v�̈���J������

  Arguments:    pipe  �R�}���h�p�C�v
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/

//
// SOCKET THREAD

//
static void SOCLi_FreeCommandPipe(SOCLiSocketCommandPipe* pipe)
{
    SOCLiCommandPacket*     command;
    OSIntrMode  enable;

    if (pipe == NULL)
        return;

    SDK_ASSERT(OS_GetCurrentThread() != &pipe->thread);

    //
    // �\�P�b�g�X���b�h�̏I�����܂�
    // ���ɏI�����Ă���Ƃ��͉������������ɕԂ��Ă���
    //
    OS_JoinThread(&pipe->thread);

    //
    // �\�P�b�g�X���b�h�̏����҂��ɂȂ��Ă��鑼�̃X���b�h���N��������
    //     - �R�}���h�����҂��̃X���b�h�� SOCL_ECANCELED ���b�Z�[�W�𑗐M
    //     - �R�}���h�p�P�b�g�����
    // ���̊֘A�����S�Ă��܂Ƃ߂ăA�g�~�b�N�ɂ��Ă���D
    // �����̃��b�Z�[�W�֘A�����S�Ă� NOBLOCK �łȂ��ƃn���O����\��
    // ������̂Œ��ӁD
    //

    // �X���b�h�؂�ւ����~�߂�
    enable = OS_DisableInterrupts();
    (void)OS_DisableScheduler();
    
    while (OS_ReceiveMessage(&pipe->queue, (void*) &command, OS_MESSAGE_NOBLOCK))
    {
        if (command)
        {
            if (command->h.response)
            {
                (void)OS_SendMessage(command->h.response, (void*)SOCL_ECANCELED, OS_MESSAGE_NOBLOCK);
            }

            SOCLi_FreeCommandPacket(command);   // NOBLOCK
        }
    }
    
    // ��L�̃��b�Z�[�W���M�����ɂ���ċN���������̗D��x�̍���
    // �X���b�h�Ɏ��s�����Ϗ�����
    (void)OS_EnableScheduler();
    OS_RescheduleThread();
    (void)OS_RestoreInterrupts(enable);
    
    return;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_TrashSocket

  Description:  �p�����X�g�ɓo�^����Ă���\�P�b�g�̗̈���J������
                CPS ���C�u�����������I�ɌĂяo�����
  
  Arguments:    �Ȃ�
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOCLi_TrashSocket(void)
{
    OSIntrMode  enable = OS_DisableInterrupts();

    while (SOCLiSocketListTrash != NULL)
    {
        SOCLi_CleanupSocket(SOCLiSocketListTrash);
    }
    (void)OS_RestoreInterrupts(enable);
}

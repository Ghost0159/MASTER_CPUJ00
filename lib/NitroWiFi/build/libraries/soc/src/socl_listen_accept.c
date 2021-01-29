/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_listen_accept.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_listen_accept.c,v $
  Revision 1.7  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.6  2006/01/13 07:26:50  yasu
  SOCL �֐��̃p�����[�^����

  Revision 1.5  2005/08/24 09:25:13  yasu
  SOCL_SocketIsInvalid �ǉ�

  Revision 1.4  2005/08/01 13:23:00  yasu
  SOCL_Listen/Accept �̒ǉ�

  Revision 1.3  2005/07/30 15:30:43  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.2  2005/07/27 12:20:04  yasu
  �f�[�^�o�͂̍ۂɃl�b�g���[�N�o�C�g�I�[�_�[��I���\�ɂ���

  Revision 1.1  2005/07/22 13:55:35  yasu
  listen accept �̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>
#include <nitroWiFi/soc.h>

static int  SOCL_ExecListenAcceptCommand(int s, vu16* remote_port_ptr, volatile SOCLInAddr* remote_ip_ptr);
static int  SOCLi_ListenAcceptCallBack(void* arg);

/*---------------------------------------------------------------------------*
  Name:         SOCL_Listen

  Description:  TCP �̃p�b�V�u�R�l�N�V�������m�����鐔���w�肵�A
                �҂������̏���������. ���݂͉������Ȃ�
  
  Arguments:    s               �\�P�b�g
                backlog         �҂��󂯉\�Ȑڑ��̐�
  
  Returns:      ��or 0: ���� (accept ���ꂽ
 *---------------------------------------------------------------------------*/
int SOCL_Listen(int s, int backlog)
{
    SOCLSocket*     socket = (SOCLSocket*)s;

    if (SOCL_SocketIsInvalid(socket))
    {
        return SOCL_EINVAL;         // �K��O�̒l
    }

    if (!SOCL_SocketIsCreated(socket))
    {
        return SOCL_ENETRESET;      // ����������Ă��Ȃ�
    }

    if (SOCL_SocketIsConnecting(socket))
    {
        return SOCL_EINVAL;         // �ڑ��������J�n���Ă���
    }

    if (!SOCL_SocketIsTCP(socket))
    {
        return SOCL_EINVAL;         // TCP �\�P�b�g�łȂ�
    }

    if (!SOCL_SocketIsBlock(socket))
    {
        return SOCL_EWOULDBLOCK;    // BLOCK �̎����̂�
    }

    // ����ł͉������Ȃ�
    // �񓯊��� Listen/Accept ���T�|�[�g����ꍇ�A������ǉ�����K�v������D
    (void)backlog;
    (void)socket;

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_Accept

  Description:  TCP �̃p�b�V�u�R�l�N�V�������m������
                CPS �̎d�l�ɂ��킹�� Listen �� Accept �������ɍs�Ȃ���
                �\�P�b�g��V�����쐬����
  
  Arguments:    s               �\�P�b�g
                remote_port     �����[�g�̃|�[�g�ԍ��ւ̃|�C���^
                remote_ip       �����[�g�� IP �A�h���X�ւ̃|�C���^
  
  Returns:      ��or 0: ���� (accept ���ꂽ
                ��    : �G���[
                ����T�|�[�g���Ă���G���[�l�͈ȉ�
  
                �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOCL_Accept(int s_listen, vu16* remote_port_ptr, volatile SOCLInAddr* remote_ip_ptr)
{
    SOCLSocket*     socket_listen = (SOCLSocket*)s_listen;
    int s_accept;
    s32 result;

    if (SOCL_SocketIsInvalid(socket_listen))
    {
        return SOCL_EINVAL;         // �K��O�̒l
    }

    if (!SOCL_SocketIsCreated(socket_listen))
    {
        return SOCL_ENETRESET;      // ����������Ă��Ȃ�
    }

    if (SOCL_SocketIsConnecting(socket_listen))
    {
        return SOCL_EINVAL;         // �ڑ��������J�n���Ă���
    }

    if (!SOCL_SocketIsTCP(socket_listen))
    {
        return SOCL_EINVAL;         // TCP �\�P�b�g�łȂ�
    }

    if (!SOCL_SocketIsBlock(socket_listen))
    {
        return SOCL_EWOULDBLOCK;    // BLOCK �̎����̂�
    }

    s_accept = SOCL_TcpSocket();

    if (s_accept < 0)
    {
        return s_accept;

        // SOCL_ENOMEM    : �������[�s��
    }

    result = SOCL_Bind(s_accept, socket_listen->local_port);

    if (result < 0)
    {
        return result;

        // SOCL_Bind �̕Ԃ�l
        //   SOCL_EINVAL;      �K��O�̒l             -> �����ł͂��肦�Ȃ�
        //   SOCL_ENETRESET;   ����������Ă��Ȃ�     -> �����ł͂��肦�Ȃ�
        //   SOCL_EALREADY;    �ڑ��������J�n���Ă��� -> �����ł͂��肦�Ȃ�
        //   SOCL_EMFILE;      �\�P�b�g�R�}���h�p�P�b�g���m�ۂł��Ȃ�����
        //   SOCL_ETIMEDOUT;   �ڑ��^�C���A�E�g
    }

    result = SOCL_ListenAccept(s_accept, remote_port_ptr, remote_ip_ptr);

    if (result < 0)
    {
        return result;

        // SOCL_ListenAccept �̕Ԃ�l
        //   SOCL_EINVAL;       �K��O�̒l             -> �����ł͂��肦�Ȃ�
        //   SOCL_ENETRESET;    ����������Ă��Ȃ�     -> �����ł͂��肦�Ȃ�
        //   SOCL_EINVAL;       �ڑ��������J�n���Ă��� -> �����ł͂��肦�Ȃ�
        //   SOCL_EINVAL;       TCP �\�P�b�g�łȂ�     -> �����ł͂��肦�Ȃ�
        //   SOCL_EWOULDBLOCK;  BLOCK ���[�h�łȂ�     -> �����ł͂��肦�Ȃ�
        //   SOCL_EINVAL;       Listen �p�̃|�[�g���ݒ肳��Ă��Ȃ� -> �����ł͂��肦�Ȃ�
    }

    return s_accept;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_ListenAccept

  Description:  TCP �̃p�b�V�u�R�l�N�V�������m������
                CPS �̎d�l�ɂ��킹�� Listen �� Accept �������ɍs�Ȃ���
                �����Ŏw�肳�ꂽ�\�P�b�g�� accept �p�Ƃ��Ďg��
  
  Arguments:    s               �\�P�b�g
                remote_port     �����[�g�̃|�[�g�ԍ��ւ̃|�C���^
                remote_ip       �����[�g�� IP �A�h���X�ւ̃|�C���^
                net_byte_order  �o�C�g�I�[�_�[ TRUE �Ȃ� NetwrokByteOrder
  
  Returns:      ��or 0: ����
                ��    : �G���[
                ����T�|�[�g���Ă���G���[�l�͈ȉ�
  
                �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOCL_ListenAccept(int s, vu16* remote_port_ptr, volatile SOCLInAddr* remote_ip_ptr)
{
    SOCLSocket*         socket = (SOCLSocket*)s;
    SOCLiCommandPacket*     command;
    s32 result;

    if (SOCL_SocketIsInvalid(socket))
    {
        return SOCL_EINVAL;         // �K��O�̒l
    }

    if (!SOCL_SocketIsCreated(socket))
    {
        return SOCL_ENETRESET;      // ����������Ă��Ȃ�
    }

    if (SOCL_SocketIsConnecting(socket))
    {
        return SOCL_EINVAL;         // �ڑ��������J�n���Ă���
    }

    if (!SOCL_SocketIsTCP(socket))
    {
        return SOCL_EINVAL;         // TCP �\�P�b�g�łȂ�
    }

    if (!SOCL_SocketIsBlock(socket))
    {
        return SOCL_EWOULDBLOCK;    // BLOCK ���[�h�łȂ�
    }

    if (socket->local_port == 0)
    {
        return SOCL_EINVAL;         // Listen �p�̃|�[�g���ݒ肳��Ă��Ȃ�
    }

    // ��M�X���b�h�ɃR�}���h�𓊂��A�R�[���o�b�N�֐������� CPS_Listen
    // ����� CPS_SocWho ���N������D
    // ���݁A�\�P�b�g�^�C�v NOBLOCK �̓T�|�[�g���Ă��Ȃ�
    command = SOCLi_CreateCommandPacket(SOCLi_ListenAcceptCallBack, socket, socket->flag_block);

    SDK_ASSERT(command);

    // listen_accept �p�̃p�����[�^�ݒ�
    command->listen_accept.local_port = socket->local_port;
    command->listen_accept.remote_port_ptr = remote_port_ptr;
    command->listen_accept.remote_ip_ptr = remote_ip_ptr;
    socket->state |= SOCL_STATUS_CONNECTING;

    // �R�}���h���s
    result = SOCLi_ExecCommandPacketInRecvPipe(socket, command);

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ListenAcceptCallBack

  Description:  Listen & Accept �����̃R�[���o�b�N
  
  Arguments:    arg  �R�}���h�u���b�N�ւ̃|�C���^
  
  Returns:      SOCLi_ExecCommand* �ɓn�����l(BLOCK���[�h�̂Ƃ�)
 *---------------------------------------------------------------------------*/
static int SOCLi_ListenAcceptCallBack(void* arg)
{
    SOCLiCommandListenAccept*   cpacket = (SOCLiCommandListenAccept*)arg;
    SOCLSocket*     socket = (SOCLSocket*)cpacket->h.socket;
    SOCLiSocketRecvCommandPipe*     recv_pipe;
    u16         remote_port;
    CPSInAddr   remote_ip;
    CPSInAddr   local_ip;

    SDK_ASSERT(socket);
    recv_pipe = socket->recv_pipe;
    SDK_ASSERT(recv_pipe);

    OS_LockMutex(&recv_pipe->h.in_use); // BLOCKED
    {
        // ���[�J���|�[�g�҂��󂯐ݒ�  �����[�g�� any �ݒ�
#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_SocBind(%d,0,0)\n", cpacket->local_port);
#endif
        CPS_SocBind(cpacket->local_port, 0, 0);

        // ���̃\�P�b�g����ڑ�����������܂ő҂�
#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_TcpListen()\n");
#endif
        CPS_TcpListen();

        recv_pipe->consumed = 0;

        // �ڑ���̎擾
#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_SocWho()\n");
#endif
        remote_ip = CPS_SocWho(&remote_port, &local_ip);

        *cpacket->remote_port_ptr = remote_port;
        *cpacket->remote_ip_ptr = remote_ip;

        socket->state |= SOCL_STATUS_CONNECTED;
    }

    OS_UnlockMutex(&recv_pipe->h.in_use);

    return SOCL_ESUCCESS;               // ����
}

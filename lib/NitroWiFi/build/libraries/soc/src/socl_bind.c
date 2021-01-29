/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_bind.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_bind.c,v $
  Revision 1.16  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.15  2006/01/13 07:26:50  yasu
  SOCL �֐��̃p�����[�^����

  Revision 1.14  2005/09/19 14:08:29  yasu
  Closing ��Ԃ�ǉ�����

  Revision 1.13  2005/09/13 07:27:06  yada
  applied for connect() on UDP

  Revision 1.12  2005/08/24 09:25:13  yasu
  SOCL_SocketIsInvalid �ǉ�

  Revision 1.11  2005/08/18 08:54:18  yasu
  NOBLOCK connect �ɂ�����Ԓl�̏C��

  Revision 1.10  2005/08/10 09:30:35  seiki_masashi
  SSL �̃T�|�[�g��ǉ�

  Revision 1.9  2005/08/08 14:26:11  yasu
  NoBlock ���[�h���̊��ɐڑ����Ă���Ƃ��� result �l�� 0 �ɕύX

  Revision 1.8  2005/08/07 13:04:05  yasu
  NOBLOCK ���̏����𒲐�

  Revision 1.7  2005/08/01 13:23:42  yasu
  Bind ���� Mutex �J���ʒu�̕ύX

  Revision 1.6  2005/07/30 22:30:14  yasu
  �f���������悤�ɏC��

  Revision 1.5  2005/07/30 15:30:52  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.4  2005/07/23 14:16:49  yasu
  Debug ���b�Z�[�W���B��

  Revision 1.3  2005/07/22 12:44:56  yasu
  �񓯊�����������

  Revision 1.2  2005/07/19 14:06:33  yasu
  SOCL_Read/Write �ǉ�

  Revision 1.1  2005/07/18 13:16:28  yasu
  ���M��M�X���b�h���쐬����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>

static int  SOCLi_BindCallBack(void* arg);
static int  SOCLi_ExecBindCommand(SOCLSocket* socket);

/*---------------------------------------------------------------------------*
  Name:         SOCL_Bind

  Description:  �\�P�b�g�̃��[�J���|�[�g���̐ݒ�(�o�C���h)���s�Ȃ�
                CPS �֐��ł͐ڑ���̃|�[�g�ƃA�h���X�������ɐݒ肵��
                �ڑ�����̂ł����ł̓\�P�b�g�\���̓����ɐݒ�l��ۑ�
                ����̂݁D

  Arguments:    s		�\�P�b�g
                localport	���[�J���̃|�[�g�ԍ�

  Returns:      ��or 0: ����
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOCL_Bind(int s, u16 local_port)
{
    SOCLSocket*     socket = (SOCLSocket*)s;

    if (SOCL_SocketIsInvalid(socket))
    {
        return SOCL_EINVAL;     // �K��O�̒l
    }

    if (!SOCL_SocketIsCreated(socket))
    {
        return SOCL_ENETRESET;  // ����������Ă��Ȃ�
    }

    if (SOCL_SocketIsConnecting(socket))
    {
        return SOCL_EALREADY;   // �ڑ��������J�n���Ă���
    }

    socket->local_port = local_port;

    return SOCL_SocketIsUDP(socket) ? SOCLi_ExecBindCommand(socket) : 0;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_Connect

  Description:  �\�P�b�g�̃����[�g�|�[�g���̐ݒ�(�o�C���h)���s�Ȃ�
                CPS �֐��ł͐ڑ���̃|�[�g�ƃA�h���X�������ɐݒ肵��
                �ڑ�����̂ł����ł� SOC_Bind �Ń\�P�b�g�\���̓����ɕۑ�
                ���ꂽ�ݒ�l���g�p����D

  Arguments:    s		�\�P�b�g
                remoteport	�����[�g�̃|�[�g�ԍ�
                remoteIP	�����[�g�� IP �A�h���X

  Returns:      ��or 0: ����
                ��    : �G���[
                ����T�|�[�g���Ă���G���[�l�͈ȉ�
                  SOC_EINPROGRESS  : �ڑ����܂��������Ă��܂���B
                  SOC_EINVAL       : �����ȏ����B
                  SOC_EISCONN      : �\�P�b�g�����łɐڑ�����Ă��܂��B
                  SOC_EMFILE       : �\�P�b�g�L�q�q������ȏ���܂���B
                  SOC_ENETRESET    : �\�P�b�g������������Ă��܂���B
                  SOC_ENOBUFS      : ���\�[�X�s���B
                  SOC_ETIMEDOUT    : TCP�̃R�l�N�g�^�C���A�E�g�B

                �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOCLiResultCodeInConnecting = SOCL_EINPROGRESS; //�܂��� SOCL_EALREADY;
int SOCL_Connect(int s, u16 remote_port, SOCLInAddr remote_ip)
{
    SOCLSocket*     socket = (SOCLSocket*)s;
    int result;

    if (SOCL_SocketIsInvalid(socket) || SOCL_SocketIsClosing(socket))
    {
        return SOCL_EINVAL;                 // �K��O�̒l���邢�͐ؒf��
    }

    if (!SOCL_SocketIsCreated(socket))
    {
        return SOCL_ENETRESET;              // ����������Ă��Ȃ�
    }

    // TCP �̏ꍇ
    if (SOCL_SocketIsTCP(socket))
    {
        if (SOCL_SocketIsConnected(socket)) // ���łɐڑ�����Ă��邩
        {
            return SOCL_SocketIsBlock(socket) ? SOCL_EISCONN : 0;   // ���ɐڑ����Ă���
        }

        if (SOCL_SocketIsConnecting(socket))
        {
            if (SOCL_SocketIsError(socket))
            {
                return socket->result;

                // SOCL_ETIMEDOUT : �ڑ��^�C���A�E�g
            }
            else
            {
                // ���ɐڑ��J�n���D
                // ���^�[���l����ɂ��邩�́A�݊����̖��D
                // �A�v���P�[�V�����̂���� SOCL_EINPROGRESS �����҂��Ă�����̂�
                // SOCL_EALREADY �����҂��Ă�����̂Ƃ�����炵���D-> �T�|�[�g���
                // ���[�U���ύX�\�ɂ��邽�߂ɃO���[�o���ϐ��ɂ��Ă���
                return SOCLiResultCodeInConnecting;
            }
        }

        socket->remote_port = remote_port;
        socket->remote_ip = remote_ip;

        result = SOCLi_ExecBindCommand(socket);

        return SOCL_SocketIsBlock(socket) ? result : SOCL_EINPROGRESS;
    }
    else
    {
        // UDP�̏ꍇ�͊��ɐڑ����ĂĂ��悢(bind ��� connect�ɑΉ��B�o���邾��)
        socket->remote_port = remote_port;
        socket->remote_ip = remote_ip;
        return 0;
    }
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_ExecBindCommand

  Description:  �\�P�b�g�̃����[�g�|�[�g���̐ݒ�(�o�C���h)���s�Ȃ�
                CPS �֐��ł͐ڑ���̃|�[�g�ƃA�h���X�������ɐݒ肵��
                �ڑ�����̂ł����ł� SOC_Bind �Ń\�P�b�g�\���̓����ɕۑ�
                ���ꂽ�ݒ�l���g�p����D

  Arguments:    s		�\�P�b�g
                remoteport	�����[�g�̃|�[�g�ԍ�
                remoteIP	�����[�g�� IP �A�h���X

  Returns:      ��or 0: ����
                ��    : �G���[
                ����T�|�[�g���Ă���G���[�l�͈ȉ�
                  SOCL_EMFILE    : �\�P�b�g�R�}���h�p�P�b�g���m�ۂł��Ȃ�����
                �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
static int SOCLi_ExecBindCommand(SOCLSocket* socket)
{
    SOCLiCommandPacket*     command;
    s32 result;

    // recv_pipe �ɃR�}���h�𓊂��A�R�[���o�b�N�֐������� CPS_Bind ��
    // �N������D�\�P�b�g�^�C�v�� NOBLOCK �ɂȂ��Ă���Ȃ�R�}���h���m��
    // �ł��Ȃ��ꍇ�ɃG���[�ƂȂ�
    command = SOCLi_CreateCommandPacket(SOCLi_BindCallBack, socket, socket->flag_block);
    if (NULL == command)
    {
        return SOCL_EMFILE;
    }

    // bind �p�̃p�����[�^�ݒ�
    command->bind.local_port = socket->local_port;
    command->bind.remote_port = socket->remote_port;
    command->bind.remote_ip = socket->remote_ip;
    socket->state |= SOCL_STATUS_CONNECTING;

    // �R�}���h���s
    result = SOCLi_ExecCommandPacketInRecvPipe(socket, command);

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_BindCallBack

  Description:  Bind/Connect �����̃R�[���o�b�N

  Arguments:    arg  �R�}���h�u���b�N�ւ̃|�C���^

  Returns:      SOCLi_ExecCommand* �ɓn�����l(BLOCK���[�h�̂Ƃ�)
 *---------------------------------------------------------------------------*/
static int SOCLi_BindCallBack(void* arg)
{
    SOCLiCommandBind*   cpacket = (SOCLiCommandBind*)arg;
    SOCLSocket*         socket = cpacket->h.socket;
    SOCLiSocketRecvCommandPipe*     recv_pipe;
    u32 retcode = 0;

    SDK_ASSERT(socket);
    recv_pipe = socket->recv_pipe;
    SDK_ASSERT(recv_pipe);

    OS_LockMutex(&recv_pipe->h.in_use); // BLOCKED
    {
#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_SocBind(\n");
        OS_TPrintf("   local  port=%d,\n", cpacket->local_port);
        OS_TPrintf("   remote_port=%d,\n", cpacket->remote_port);
        OS_TPrintf("   remote_ip  =%3d. %3d. %3d. %3d\n", CPS_CV_IPv4(cpacket->remote_ip));
#endif
        CPS_SocBind(cpacket->local_port, cpacket->remote_port, cpacket->remote_ip);
        recv_pipe->consumed = 0;

        // TCP �Ȃ�R�l�N�g�������s�Ȃ��D
        // ���s������^�C���A�E�g�G���[�l��Ԃ�
        if (cpacket->h.flag_mode == SOCL_FLAGMODE_TCP || cpacket->h.flag_mode == SOCL_FLAGMODE_SSL)
        {
#ifdef SDK_MY_DEBUG
            OS_TPrintf("CPS_TcpConnect\n");
#endif
            retcode = CPS_TcpConnect();
#ifdef SDK_MY_DEBUG
            OS_TPrintf("CPS_TcpConnect.retcode=%d\n", retcode);
#endif
        }
    }

    OS_UnlockMutex(&recv_pipe->h.in_use);

    if (retcode)
    {
        socket->state |= SOCL_STATUS_ERROR;
        return SOCL_ETIMEDOUT;
    }

    socket->state |= SOCL_STATUS_CONNECTED;
    return SOCL_ESUCCESS;               // ����
}

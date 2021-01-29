/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_shutdown.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_shutdown.c,v $
  Revision 1.6  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.5  2005/09/19 15:11:37  yasu
  Closing ��Ԃ�ǉ����������̔�����

  Revision 1.4  2005/08/24 09:25:13  yasu
  SOCL_SocketIsInvalid �ǉ�

  Revision 1.3  2005/08/14 10:58:35  yasu
  �ڑ����t���O�̊m�F�̊ԈႢ���C��

  Revision 1.2  2005/07/30 15:30:52  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.1  2005/07/22 12:44:56  yasu
  �񓯊�����������

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>

static int  SOCLi_ShutdownCallBack(void* arg);

/*---------------------------------------------------------------------------*
  Name:         SOCL_Shutdown

  Description:  �\�P�b�g�� Shutdown �������s�Ȃ��D
  
  Arguments:    s		�\�P�b�g
  
  Returns:      ��or 0: ����
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :  
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOCL_Shutdown(int s)
{
    SOCLSocket*     socket = (SOCLSocket*)s;
    SOCLiSocketSendCommandPipe*     send_pipe;
    SOCLiCommandPacket*         command;
    s32 result;

    if (SOCL_SocketIsInvalid(socket))
    {
        return SOCL_EINVAL;     // �K��O�̒l
    }

    if (!SOCL_SocketIsCreated(socket))
    {
        return SOCL_ENETRESET;  // ����������Ă��Ȃ�
    }

    if (!SOCL_SocketIsConnected(socket) || SOCL_SocketIsClosing(socket))
    {
        return SOCL_ENOTCONN;   // �ڑ�����Ă��Ȃ�
    }

    socket->state |= SOCL_STATUS_CLOSING;

    // ���M�X���b�h�ɃR�}���h�𓊂��A�R�[���o�b�N�֐������� CPS_Shutdown ��
    // �N������D�\�P�b�g�^�C�v�� NOBLOCK �ɂȂ��Ă���Ȃ�R�}���h���m��
    // �ł��Ȃ��ꍇ�ɃG���[�ƂȂ�
    //
    // ���݂̎����͎��Ԃ̖��̂��߂��̂悤�ɂ��Ă��邪�Adestructor ���g�p��
    // �Ė����Ƀ\�P�b�g��ؒf����Ƃ��������ɕύX����K�v������Ǝv����
    send_pipe = socket->send_pipe;

    if (send_pipe && send_pipe->exe_socket)
    {
        command = SOCLi_CreateCommandPacket(SOCLi_ShutdownCallBack, send_pipe->exe_socket, socket->flag_block);
        if (NULL == command)
        {
            return SOCL_EMFILE;
        }

        // �R�}���h���s
        result = SOCLi_ExecCommandPacketInSendPipe(send_pipe->exe_socket, command);
    }
    else
    {
        result = 0;
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ShutdownCallBack

  Description:  Shutdown �����̃R�[���o�b�N
  
  Arguments:    arg  �R�}���h�u���b�N�ւ̃|�C���^
  
  Returns:      SOCLi_ExecCommand* �ɓn�����l(BLOCK���[�h�̂Ƃ�)
 *---------------------------------------------------------------------------*/
static int SOCLi_ShutdownCallBack(void* arg)
{
    SOCLiCommandShutdown*   cpacket = (SOCLiCommandShutdown*)arg;
    SOCLSocket*         socket = (SOCLSocket*)cpacket->h.socket;

    if (SOCL_SocketIsTCP(socket))
    {
        CPS_TcpShutdown();
    }

    return SOCL_ESUCCESS;   // ����
}

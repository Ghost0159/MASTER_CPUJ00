/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_poll.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_poll.c,v $
  Revision 1.12  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.11  2005/10/04 10:31:40  yasu
  SOC_GetStatus �ɂ����ă����[�g���� FIN ���󂯎�����Ƃ��̎�M�o�b�t�@�Ɏc���Ă���
  �f�[�^�����邤���́AREADABLE �Ƃ݂Ȃ��悤�ɏC��

  Revision 1.10  2005/09/01 06:42:55  yasu
  �\�P�b�g�������Ă��邩�ǂ����̔�����ꌳ��

  Revision 1.9  2005/08/26 04:02:25  yasu
  �x���̗}��

  Revision 1.8  2005/08/26 03:45:25  yasu
  SOCL_GetStatus �� UDP ��M�V�X�e���ύX�ɂƂ��Ȃ��C��

  Revision 1.7  2005/08/24 09:25:13  yasu
  SOCL_SocketIsInvalid �ǉ�

  Revision 1.6  2005/08/18 04:27:15  yasu
  �s���� '\' �̃P�A

  Revision 1.5  2005/08/17 04:10:28  yasu
  SOC_Poll �̋@�\�ǉ�

  Revision 1.4  2005/08/12 09:51:10  yasu
  IsWritable �֐��̕Ԓl�����i�� BOOL �ɐ���

  Revision 1.3  2005/08/01 07:34:28  yasu
  IsWritable �� connected ���ǂ����̃`�F�b�N��ǉ�

  Revision 1.2  2005/07/29 01:49:02  yasu
  �p��ŃR���p�C���΍�

  Revision 1.1  2005/07/28 11:15:17  yasu
  SOC_Poll �̎���

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>
#include <nitroWiFi/soc.h>

/*---------------------------------------------------------------------------*
  Name:         SOCL_GetStatus

  Description:  �w�肵���\�P�b�g�̏�Ԃ𒲂ׂ܂��D

  Arguments:    s       �\�P�b�g�L�q�q
  
  Returns:      �ȉ��̂��̂� OR ���ꂽ���̂ƂȂ�
                SOC_POLLNVAL		0x80	�\�P�b�g������
                SOC_POLLHUP		0x40	�ڑ����������邢�͐ڑ����łȂ�
                SOC_POLLERR		0x20	�G���[����
                SOC_POLLWRNORM		0x08	�������݉\.
                SOC_POLLRDNORM		0x01	�ǂݍ��݉\.
 *---------------------------------------------------------------------------*/
int SOCL_GetStatus(int s)
{
    SOCLSocket*     socket = (SOCLSocket*)s;
    int         result = 0;
    OSIntrMode  enable;

    if (SOCL_SocketIsInvalid(socket))
    {
        result |= SOC_POLLNVAL;
    }
    else
    {
        if (SOCL_SocketIsError(socket))
        {
            result |= SOC_POLLERR;
        }

        if (SOCL_SocketIsUDP(socket) || SOCL_SocketIsConnected(socket))
        {
            enable = OS_DisableInterrupts();
            if (SOCLi_GetReadBufferOccpiedSize(socket) > 0)
            {
                result |= SOC_POLLRDNORM;
            }

            if (SOCLi_GetWriteBufferFreeSize(socket) > 0)
            {
                result |= SOC_POLLWRNORM;
            }
            (void)OS_RestoreInterrupts(enable);
        }

        if (SOCL_SocketIsTCP(socket))
        {
            // �ڑ���Ԃł��邩�ǂ����m�F���t���O�������e�i���X����
            // READ �\�ȏ�ԂȂ�ؒf��Ԃɂ͂��Ȃ�
            if (SOCL_SocketIsConnected(socket) && socket->cps_socket.state != CPS_STT_ESTABLISHED &&
                !(result & SOC_POLLRDNORM))
            {
                socket->state &= ~(SOCL_STATUS_CONNECTING | SOCL_STATUS_CONNECTED);
            }

            if (!SOCL_SocketIsConnecting(socket) && !SOCL_SocketIsConnected(socket))
            {
                result |= SOC_POLLHUP;
            }
        }

    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_GetReadBufferOccpiedSize

  Description:  �w�肵���\�P�b�g�̒��M���Ă���f�[�^�̃T�C�Y�𒲂ׂ܂��D

  Arguments:    socket   �\�P�b�g
  
  Returns:      �T�C�Y
 *---------------------------------------------------------------------------*/
s32 SOCLi_GetReadBufferOccpiedSize(SOCLSocket* socket)
{
    SOCLiSocketRecvCommandPipe*     recv_pipe = socket->recv_pipe;
    s32 size = 0;

    if (recv_pipe)
    {
        if (SOCL_SocketIsUDP(socket))
        {
            SOCLiSocketUdpData*     udpout = recv_pipe->udpdata.out;
            size = (s32) (udpout ? udpout->size : 0);
        }
        else if (SOCL_SocketIsTCP(socket))
        {
            size = (s32) socket->cps_socket.rcvbufp - (s32) recv_pipe->consumed;
        }
    }

    return size;
}

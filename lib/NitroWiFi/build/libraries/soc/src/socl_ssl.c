/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_ssl.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_ssl.c,v $
  Revision 1.4  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.3  2005/08/24 09:25:13  yasu
  SOCL_SocketIsInvalid �ǉ�

  Revision 1.2  2005/08/10 09:32:00  seiki_masashi
  SSL �̃T�|�[�g��ǉ�

  Revision 1.1  2005/08/06 11:55:46  seiki_masashi
  SOCL_EnableSsl ��ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>

static int  SOCLi_EnableSslCallBack(void* arg);
static int  SOCLi_ExecEnableSslCommand(SOCLSocket* socket, SOCLSslConnection* con);

/*---------------------------------------------------------------------------*
  Name:         SOCL_EnableSsl

  Description:  �\�P�b�g�� SSL ���[�h�ɂ���B
                bind �O�ɌĂяo���˂΂Ȃ�Ȃ��B
  
  Arguments:    s    �\�P�b�g
                con  SOCLSslConnection �\���̂ւ̃|�C���^
                     NULL �� SSL ���[�h����ʏ�ɖ߂�
  
  Returns:      �� or 0: ����
                ��     : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :  
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOCL_EnableSsl(int s, SOCLSslConnection* con)
{
    SOCLSocket*     socket = (SOCLSocket*)s;

    if (SOCL_SocketIsInvalid(socket))
    {
        return SOCL_EINVAL;     // �K��O�̒l
    }

    if (!SOCL_SocketIsTCP(socket))
    {
        return SOCL_EINVAL;     // SSL �� TCP �ł����g���Ȃ�
    }

    if (!SOCL_SocketIsCreated(socket))
    {
        return SOCL_ENETRESET;  // ����������Ă��Ȃ�
    }

    if (SOCL_SocketIsConnecting(socket))
    {
        return SOCL_EINVAL;     // �ڑ��������J�n���Ă���
    }

    return SOCLi_ExecEnableSslCommand(socket, con);
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ExecEnableSslCommand

  Description:  �\�P�b�g�� SSL ���[�h�ɂ���
  
  Arguments:    s    �\�P�b�g
                con  SOCLSslConnection �\���̂ւ̃|�C���^
                     NULL �� SSL ���[�h����ʏ�ɖ߂�
  
  Returns:      ��or 0: ����
                ��    : �G���[
                ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :  
                �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
static int SOCLi_ExecEnableSslCommand(SOCLSocket* socket, SOCLSslConnection* con)
{
    SOCLiCommandPacket*     command;
    s32 result;

    // recv_pipe �ɃR�}���h�𓊂��A�R�[���o�b�N�֐������� CPS_SetSsl ��
    // ���s����D�R�}���h���m�ۂł��Ȃ��ꍇ�ɃG���[�ƂȂ�
    command = SOCLi_CreateCommandPacket(SOCLi_EnableSslCallBack, socket, OS_MESSAGE_BLOCK);
    if (NULL == command)
    {
        return SOCL_EMFILE;
    }

    // enable ssl �p�̃p�����[�^�ݒ�
    command->enable_ssl.connection = con;

    // �R�}���h���s
    result = SOCLi_ExecCommandPacketInRecvPipe(socket, command);

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_EnableSslCallBack

  Description:  EnableSsl �̃R�[���o�b�N
  
  Arguments:    arg  �R�}���h�u���b�N�ւ̃|�C���^
  
  Returns:      SOCLi_ExecCommand* �ɓn�����l(BLOCK���[�h�̂Ƃ�)
 *---------------------------------------------------------------------------*/
static int SOCLi_EnableSslCallBack(void* arg)
{
    SOCLiCommandEnableSsl*          cpacket = (SOCLiCommandEnableSsl*)arg;
    SOCLSocket*         socket = cpacket->h.socket;
    SOCLiSocketRecvCommandPipe*     recv_pipe;
    CPSSoc*     cpsoc;

    SDK_ASSERT(socket);
    recv_pipe = socket->recv_pipe;
    SDK_ASSERT(recv_pipe);
    cpsoc = (CPSSoc*)OSi_GetSpecificData(&recv_pipe->h.thread, OSi_SPECIFIC_CPS);
    SDK_ASSERT(cpsoc);

    OS_LockMutex(&recv_pipe->h.in_use); // BLOCKED
    {
#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_SetSsl(%d)\n", (cpacket->connection != NULL) ? 1 : 0);
#endif

        if (cpacket->connection != NULL)
        {
            cpsoc->con = cpacket->connection;
            CPS_SetSsl(1);
            socket->flag_mode = SOCL_FLAGMODE_SSL;
        }
        else
        {
            socket->flag_mode = SOCL_FLAGMODE_TCP;
            CPS_SetSsl(0);
            cpsoc->con = NULL;
        }
    }

    OS_UnlockMutex(&recv_pipe->h.in_use);

    return SOCL_ESUCCESS;               // ����
}

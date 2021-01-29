/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_create.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_create.c,v $
  Revision 1.14  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.13  2006/01/13 07:26:50  yasu
  SOCL �֐��̃p�����[�^����

  Revision 1.12  2005/09/27 14:18:09  yasu
  SOC_Close �̔񓯊�����T�|�[�g

  Revision 1.11  2005/08/25 08:27:38  yasu
  UDP ��M�ɂ�����o�b�t�@�����O�����ǉ�

  Revision 1.10  2005/08/18 13:18:49  yasu
  �\�P�b�g�����N���X�g�ɂ�� cleanup �����̒ǉ�

  Revision 1.9  2005/08/13 08:02:39  yasu
  consumed_min ����̒ǉ�

  Revision 1.8  2005/08/10 09:30:57  seiki_masashi
  SSL �̃T�|�[�g��ǉ�

  Revision 1.7  2005/07/30 22:30:14  yasu
  �f���������悤�ɏC��

  Revision 1.6  2005/07/30 15:30:08  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.5  2005/07/27 12:20:33  yasu
  �o�b�t�@�T�C�Y 0 �̎��̓|�C���^�� NULL �Ƃ���

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

static int          SOCLi_CreateSocketCallBack(void* arg);

static SOCLSocket*  SOCLi_StartupSocket(const SOCLSocketParam* param);
static u32          SOCLi_GetSizeSocket(const SOCLSocketParam* param);
static u32          SOCLi_GetSizeCommandPipe(const SOCLSocketCommandPipeParam* param);
static u8*          SOCLi_InitSocket(SOCLSocket* socket, const SOCLSocketParam* param);
static u8*          SOCLi_InitSocketBuffer(u8* ptr, CPSSocBuf* buf, u32 size);
static u8*          SOCLi_InitCommandPipe(u8*  ptr, SOCLiSocketCommandPipe*  pipe, const SOCLSocketCommandPipeParam*  param);

/*---------------------------------------------------------------------------*
  Name:         SOCL_CreateSocket

  Description:  �\�P�b�g���쐬����D
                �Ǝ��̃C���^�[�t�F�C�X���g�p���Ă���D
                ���������p�Ɋւ���ׂ��Ȑݒ肪�\�D

  Arguments:    param           �ڍׂȐݒ�p�����[�^
                                SDK �ŗp�ӂ��Ă���f�t�H���g�� 
                                TCP �̏ꍇ: &SOCLSocketParamTCP
                                UDP �̏ꍇ: &SOCLSocketParamUDP*
  
  Returns:      ���̒l: �\�P�b�g�ւ̃|�C���^
                ��    : �G���[
                ����T�|�[�g���Ă���G���[�l�͈ȉ�
                  SOCL_ENOMEM    : �������[�s��
                �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOCL_CreateSocket(const SOCLSocketParam* param)
{
    SOCLSocket*         socket;
    SOCLiCommandPacket*     command;
    s32 result;

    SDK_ASSERT(param != NULL);

    // �\�P�b�g�̗̈���m�ۂ��A�\�P�b�g�������s�Ȃ����߂̃X���b�h�𑖂点��
    // SOCLSocket ���̒l�𑼂̃X���b�h������������̂́A�\�P�b�g�쐬�Ɣp����
    // �݂̂Ƃ���D
    socket = SOCLi_StartupSocket(param);
    if (socket == NULL)
    {
        return SOCL_ENOMEM;
    }

    // �\�P�b�g�������R�}���h�𑗂�R�[���o�b�N���N�������A
    // ����������҂Dcommand �̈�͊������ɂ͓����ŊJ������Ă���D
    command = SOCLi_CreateCommandPacket(SOCLi_CreateSocketCallBack, socket, OS_MESSAGE_BLOCK);

    // �쐬�Ɣj���̏ꍇ�� Ctrl Pipe �փR�}���h�𑗂�
    // �ʏ�� Recv Pipe �� Ctrl Pipe �ɐݒ肳��Ă���(UDP Send �̂Ƃ��͗�O)
    // SOCLi_CreateSocketCallBack �͕K�� SUCCESS �ŕԂ�
    result = SOCLi_ExecCommandPacketInCtrlPipe(socket, command);
    SDK_ASSERT(result >= 0);

    return (int)socket;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_CreateSocketCallBack

  Description:  Create �����̃R�[���o�b�N
  
  Arguments:    arg  �R�}���h�u���b�N�ւ̃|�C���^
  
  Returns:      SOCLi_ExecCommand* �ɓn�����l
 *---------------------------------------------------------------------------*/
static int SOCLi_CreateSocketCallBack(void* arg)
{
    SOCLiCommandCreateSocket*   cpacket = (SOCLiCommandCreateSocket*)arg;
    SOCLSocket*     socket = cpacket->h.socket;
    SOCLiSocketSendCommandPipe*     send_pipe;
    SOCLiSocketRecvCommandPipe*     recv_pipe;

    SDK_ASSERT(socket);
#ifdef SDK_MY_DEBUG
    OS_TPrintf("CPS_SocRegister\n");
#endif
    CPS_SocRegister(&socket->cps_socket);

    send_pipe = socket->send_pipe;
    recv_pipe = socket->recv_pipe;

    switch (socket->flag_mode)
    {
    case SOCL_FLAGMODE_TCP:
    case SOCL_FLAGMODE_SSL:
#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_SocDup\n");
        OS_TPrintf("CPS_SocUse\n");
#endif
        CPS_SocDup(&send_pipe->h.thread);
        CPS_SocUse();
        break;

    case SOCL_FLAGMODE_UDP:
        CPS_SocUse();
        CPS_SocDatagramMode();
        CPS_SetUdpCallback(SOCLi_UdpRecvCallback);
#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_SocUse\n");
        OS_TPrintf("CPS_SocDatagramMode\n");
        OS_TPrintf("CPS_SetUdpCallback\n");
#endif
        break;

    case SOCL_FLAGMODE_UDPSEND:
#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_SocDatagramMode\n");
#endif
        CPS_SocDatagramMode();
        break;

    case SOCL_FLAGMODE_ICMP:
    default:
        SDK_ASSERT(0);
        break;
    }

    socket->state = SOCL_STATUS_CREATE;

    return SOCL_ESUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_StartupSocket

  Description:  �\�P�b�g�p�̗̈���擾����
                �ȉ��̗̈��A�������������}�b�v��Ɋm�ۂ���
  
                �̈�
                --------------------------------------------------------
                SOCL �\�P�b�g�̈�
                CPS  �\�P�b�g�p�̃o�b�t�@
                ��M�p�X���b�h�̃o�b�t�@
                ���M�p�X���b�h�̃o�b�t�@

  Arguments:    param  �p�����[�^
  
  Returns:      �m�ۂ����\�P�b�g���̈�̃|�C���^
 *---------------------------------------------------------------------------*/
static SOCLSocket* SOCLi_StartupSocket(const SOCLSocketParam* param)
{
    SOCLSocket*     socket;
    u32         size;
    OSIntrMode  enable;

    // �K�v�ȃ������T�C�Y�����߂�
    size = SOCLi_GetSizeSocket(param);

    enable = OS_DisableInterrupts();
    socket = SOCLi_Alloc(size);

    if (socket)
    {
        // ��U 0 �N���A����
        MI_CpuClear8(socket, size);

        // �����������Ɋ���U��
        (void)SOCLi_InitSocket(socket, param);

        // �����N�̃��X�g�ւ̓o�^
        SOCLi_SocketRegister(socket);
    }

    (void)OS_RestoreInterrupts(enable);

    return socket;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_GetSizeSocket
  
  Description:  SOCKET �ɕK�v�ȃ������T�C�Y��Ԃ�
  
  Arguments:    param  �\�P�b�g�����̃p�����[�^
  
  Returns:      SOCKET �ɕK�v�ȗ̈�̃T�C�Y
 *---------------------------------------------------------------------------*/
static u32 SOCLi_GetSizeSocket(const SOCLSocketParam* param)
{
    u32 size;

    size = SOCLi_ROUNDUP4(sizeof(SOCLSocket));

    // ��M�@�\�Ɋւ��ĕK�v�ȃ�����
    if (param->buffer.rcvbuf_size)
    {
        size += SOCLi_ROUNDUP4(sizeof(SOCLiSocketRecvCommandPipe));
        size += SOCLi_RoundUp4(param->buffer.rcvbuf_size);
        size += SOCLi_RoundUp4(param->buffer.linbuf_size);
        size += SOCLi_GetSizeCommandPipe(&param->recv_pipe);
    }

    // ���M�@�\�Ɋւ��ĕK�v�ȃ�����
    if (param->buffer.sndbuf_size)
    {
        size += SOCLi_ROUNDUP4(sizeof(SOCLiSocketSendCommandPipe));
        size += SOCLi_RoundUp4(param->buffer.sndbuf_size);
        size += SOCLi_RoundUp4(param->buffer.outbuf_size);
        size += SOCLi_RoundUp4(param->buffer.wrtbuf_size);
        size += SOCLi_GetSizeCommandPipe(&param->send_pipe);
    }

    return size;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_GetSizeCommandPipe
  
  Description:  Command Pipe �ɕK�v�ȃ������T�C�Y��Ԃ�
  
  Arguments:    param  Command Pipe�����̃p�����[�^
  
  Returns:      Command Pipe �ɕK�v�ȗ̈�̃T�C�Y
 *---------------------------------------------------------------------------*/
static u32 SOCLi_GetSizeCommandPipe(const SOCLSocketCommandPipeParam* param)
{
    u32 size;

    size = SOCLi_RoundUp4(param->queue_max * sizeof(OSMessage));
    size += SOCLi_RoundUp4(param->stack_size);

    return size;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_InitSocket
  
  Description:  SOCKET ������������

  Arguments:    socket  �Ώۂ̃\�P�b�g�̗̈�(0 �N���A����Ă��邱��)
                param   �������p�����[�^
  
  Returns:      SOCKET �̗̈�̎��̃|�C���^
 *---------------------------------------------------------------------------*/
static u8* SOCLi_InitSocket(SOCLSocket* socket, const SOCLSocketParam* param)
{
    u8*         ptr;
    CPSSoc*     cps = &socket->cps_socket;

    socket->flag_mode = param->flag_mode;
    socket->flag_block = param->flag_block;

    ptr = (u8*)socket;
    ptr = ptr + SOCLi_ROUNDUP4(sizeof(SOCLSocket));

    // ��M�@�\�Ɋւ��鏉����
    if (param->buffer.rcvbuf_size)
    {
        SOCLiSocketRecvCommandPipe*     pipe = (SOCLiSocketRecvCommandPipe*)ptr;

        // Recv Command Pipe
        socket->recv_pipe = pipe;
        pipe->consumed_min = param->buffer.rcvbuf_consume_min;
        ptr += SOCLi_ROUNDUP4(sizeof(SOCLiSocketRecvCommandPipe));
        ptr = SOCLi_InitCommandPipe(ptr, &pipe->h, &param->recv_pipe);

        // Buffers
        ptr = SOCLi_InitSocketBuffer(ptr, &cps->rcvbuf, param->buffer.rcvbuf_size);
        ptr = SOCLi_InitSocketBuffer(ptr, &cps->linbuf, param->buffer.linbuf_size);

        pipe->udpdata.size_max = param->buffer.udpbuf_size;
        OS_InitThreadQueue(&pipe->udpdata.waiting);
    }

    // ���M�@�\�Ɋւ��鏉����
    if (param->buffer.sndbuf_size)
    {
        SOCLiSocketSendCommandPipe*     pipe = (SOCLiSocketSendCommandPipe*)ptr;

        // Send Command Pipe
        socket->send_pipe = pipe;   // �\�P�b�g�ƃp�C�v�̑��݃����N
        pipe->exe_socket = socket;  //   Send Pipe ���L���ɗL��
        ptr += SOCLi_ROUNDUP4(sizeof(SOCLiSocketSendCommandPipe));
        ptr = SOCLi_InitCommandPipe(ptr, &pipe->h, &param->send_pipe);

        // Buffers
        ptr = SOCLi_InitSocketBuffer(ptr, &cps->sndbuf, param->buffer.sndbuf_size);
        ptr = SOCLi_InitSocketBuffer(ptr, &cps->outbuf, param->buffer.outbuf_size);
        ptr = SOCLi_InitSocketBuffer(ptr, &pipe->buffer.area, param->buffer.wrtbuf_size);
        OS_InitThreadQueue(&pipe->buffer.waiting);
    }
    else
    {
        // Send Pipe �����L����
        socket->send_pipe = ((SOCLSocket*)SOCLiUDPSendSocket)->send_pipe;
    }

    return ptr;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_InitSocketBuffer
  
  Description:  SOCKET ���̃o�b�t�@������������

  Arguments:    ptr     �o�b�t�@�p�Ɋm�ۂ��ꂽ������
                buf     �o�b�t�@�\����
                size    �o�b�t�@�T�C�Y
  
  Returns:      �o�b�t�@�̗̈�̎��ɗ��p�\�ȃ|�C���^
 *---------------------------------------------------------------------------*/
static u8* SOCLi_InitSocketBuffer(u8* ptr, CPSSocBuf* buf, u32 size)
{
    buf->data = size ? ptr : NULL;  // 0 �Ȃ� NULL ��ݒ�
    buf->size = size;
    return ptr + SOCLi_RoundUp4(size);
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_InitCommandPipe
  
  Description:  �R�}���h�p�C�v������������

  Arguments:    ptr     �R�}���h�p�C�v���̃o�b�t�@�p�Ɋm�ۂ��ꂽ������
                buf     �o�b�t�@�\����
                size    �o�b�t�@�T�C�Y
  
  Returns:      ptr �̎��ɗ��p�\�ȃ|�C���^
 *---------------------------------------------------------------------------*/
static u8* SOCLi_InitCommandPipe(u8* ptr, SOCLiSocketCommandPipe* pipe, const SOCLSocketCommandPipeParam* param)
{
    OSMessage*  messages;
    u8*         stack_bottom;

    // �������̊���U��
    messages = (OSMessage*)ptr;
    stack_bottom = ptr + SOCLi_GetSizeCommandPipe(param);

    // �e�̈�̏�����
    OS_InitMessageQueue(&pipe->queue, messages, param->queue_max);

    OS_InitMutex(&pipe->in_use);

    OS_CreateThread(&pipe->thread, SOCLi_CommandPacketHandler, pipe, stack_bottom, param->stack_size, param->priority);
    OS_WakeupThreadDirect(&pipe->thread);

    return stack_bottom;
}

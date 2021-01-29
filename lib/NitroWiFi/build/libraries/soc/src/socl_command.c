/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_command.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_command.c,v $
  Revision 1.10  2006/05/17 08:37:11  yasu
  SOCLi_CommandPacketHandler ���łɂ�����X���b�h�̍ăX�P�W���[���������폜�B
  �X���b�h�؂�ւ��� OS_ReadMessage ���ň�̉��B

  Revision 1.9  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.8  2005/12/06 11:53:13  yasu
  TCP BLOCK Write �ɂ�����s��̏C��
  BLOCK ���[�h����p�̃}�N���̓���

  Revision 1.7  2005/10/14 06:51:35  yasu
  �R�}���h�p�P�b�g�쐬���� response �����o�ɏ����l��ݒ肵�Ă��Ȃ������̂��C��

  Revision 1.6  2005/10/14 01:56:53  yasu
  �R�}���h�L���[����t�������Ƃ��� Close �������s���S�ȏ�ԂɂȂ邱�Ƃ��C��

  Revision 1.5  2005/09/27 14:18:09  yasu
  SOC_Close �̔񓯊�����T�|�[�g

  Revision 1.4  2005/08/11 07:02:00  yasu
  �O��̏C����������

  Revision 1.3  2005/08/11 05:08:21  yasu
  �Ԓl���s��ƂȂ�֐��̏C��

  Revision 1.2  2005/07/30 22:30:14  yasu
  �f���������悤�ɏC��

  Revision 1.1  2005/07/30 15:29:33  yasu
  �R�}���h�p�C�v�Ɋւ���ϐ����\���̂ɕ���

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/soc.h>

//---------------------------------------------------------------------------*
// �R�}���h�p�P�b�g�L���[
//---------------------------------------------------------------------------*
static OSMessage*       SOCLiCommandPackets;
static OSMessageQueue   SOCLiCommandPacketQueue;

/*---------------------------------------------------------------------------*
  Name:         SOCLi_StartupCommandPacketQueue

  Description:  �R�}���h�p�P�b�g�L���[������

  Arguments:    cp_max_count �p�P�b�g�̐�
  
  Returns:       0 ����
                -1 �������m�ێ��s
 *---------------------------------------------------------------------------*/
int SOCLi_StartupCommandPacketQueue(s32 cp_max_count)
{
    SOCLiCommandPacket*     cp_array;
    u32         cp_array_size;
    OSMessage*  cp_message_array;
    u32         cp_message_size;

    SDK_ASSERT(cp_max_count > 0);

    cp_message_size = MATH_ROUNDUP(cp_max_count * sizeof(OSMessage), 4);
    cp_array_size = MATH_ROUNDUP(cp_max_count * sizeof(SOCLiCommandPacket), 4);
    cp_message_array = (OSMessage*)SOCLi_Alloc(cp_array_size + cp_message_size);

    if (NULL == cp_message_array)
    {
        return -1;
    }

    // MessageQueue �쐬�����
    // Message �Ƃ��ăR�}���h�p�P�b�g�̃A�h���X�� Queue �ɓo�^
    OS_InitMessageQueue(&SOCLiCommandPacketQueue, cp_message_array, cp_max_count);

    cp_array = (SOCLiCommandPacket *) ((u32) cp_message_array + cp_message_size);

    while (cp_max_count > 0)
    {
        SOCLi_FreeCommandPacket(cp_array);

        cp_array++;
        cp_max_count--;
    }

    SOCLiCommandPackets = cp_message_array;
    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_CleanupCommandPacketQueue

  Description:  �R�}���h�p�P�b�g�L���[�J��

  Arguments:    �Ȃ�

  Returns:       0 ����I��
                -1 �S�ẴR�}���h�p�P�b�g�̈悪�������Ă��Ȃ�
 *---------------------------------------------------------------------------*/
int SOCLi_CleanupCommandPacketQueue(void)
{
    // �L���[���t���ɂȂ��Ă��Ȃ��Ȃ疢����̃p�P�b�g������
    if (!OS_IsMessageQueueFull(&SOCLiCommandPacketQueue))
    {
        return -1;
    }

    SOCLi_Free(SOCLiCommandPackets);
    SOCLiCommandPackets = NULL;
    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_AllocCommandPacket

  Description:  �R�}���h�p�P�b�g�̊m��

  Arguments:    flag  �R�}���h����̃u���b�N���[�h
                      OS_MESSAGE_NOBLOCK or OS_MESSAGE_BLOCK

  Returns:      �R�}���h�p�P�b�g�ւ̃|�C���^
                �u���b�N���[�h�� NOBLOCK �ŃR�}���h�p�P�b�g�̈悪�Ȃ��Ȃ� NULL
                �u���b�N���[�h�� BLOCK   �ŃR�}���h�p�P�b�g�̈悪�Ȃ��Ȃ�
                �󂫗̈悪�ł���܂Ńu���b�N���܂��D
 *---------------------------------------------------------------------------*/
SOCLiCommandPacket* SOCLi_AllocCommandPacket(s32 flag)
{
    SOCLiCommandPacket*     cp;

    return(OS_ReceiveMessage(&SOCLiCommandPacketQueue, (void*) &cp, flag) ? cp : NULL);
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_CreateCommandPacket

  Description:  �R�}���h�p�P�b�g�̍쐬

  Arguments:    handler   �R�}���h�̃n���h��
                socket    �Ώۃ\�P�b�g
                flag      �R�}���h����̃u���b�N���[�h
                          OS_MESSAGE_NOBLOCK or OS_MESSAGE_BLOCK
                          �\�P�b�g�ɐݒ肳��Ă郂�[�h�����̂܂܎g����Ƃ�
                          ����Ȃ��D�����܂ō���̃R�}���h�̓��샂�[�h�D
   
  Returns:      �R�}���h�p�P�b�g�ւ̃|�C���^
                �u���b�N���[�h�� NOBLOCK �ŃR�}���h�p�P�b�g�̈悪�Ȃ��Ȃ� NULL
                �u���b�N���[�h�� BLOCK   �ŃR�}���h�p�P�b�g�̈悪�Ȃ��Ȃ�
                �󂫗̈悪�ł���܂Ńu���b�N���܂��D
 *---------------------------------------------------------------------------*/
SOCLiCommandPacket* SOCLi_CreateCommandPacket(SOCLiCommandHandler handler, SOCLSocket* socket, s32 flag)
{
    SOCLiCommandPacket*     cp;

    cp = SOCLi_AllocCommandPacket(flag);

    if (cp)
    {
        cp->h.handler = handler;
        cp->h.socket = socket;
        cp->h.response = NULL;
        cp->h.flag_mode = socket->flag_mode;
        cp->h.flag_block = (s8) flag;
    }

    return cp;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_FreeCommandPacket

  Description:  �R�}���h�p�P�b�g�̊J��

  Arguments:    command   �R�}���h�p�P�b�g�ւ̃|�C���^

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOCLi_FreeCommandPacket(SOCLiCommandPacket* command)
{
    if (command)
    {
        (void)OS_SendMessage(&SOCLiCommandPacketQueue, command, OS_MESSAGE_NOBLOCK);
    }
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_GetCtrlPipe
  
  Description:  �R�}���h����p�C�v�̎擾

  Arguments:    socket		�\�P�b�g
  
  Returns:      �\�P�b�g����p�̃R�}���h�p�C�v�ւ̃|�C���^
 *---------------------------------------------------------------------------*/
SOCLiSocketCommandPipe* SOCLi_GetCtrlPipe(SOCLSocket* socket)
{
    // �ʏ�� Recv Pipe �����}�X�^�[�Ƃ���
    // UDPSend �̂Ƃ��͗�O�� Send Pipe ���}�X�^�[�ƂȂ�f�[�^���󂯂�.
    return(socket->recv_pipe) ? &socket->recv_pipe->h : &socket->send_pipe->h;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_SendCommandPacket
  
  Description:  �R�}���h�p�P�b�g�̑��M

  Arguments:    pipe		���M��̃R�}���h�p�C�v
                command         �R�}���h�̃p�P�b�g
  
  Returns:      command->flag_block �� NOBLOCK �̂Ƃ�
                     ���M�����Ȃ� SOCL_ESUCCESS=0
                         ���s�Ȃ� SOCL_ENOBUFS<0
                         ���s������R�}���h���������
                command->flag_block �� BLOCK �̂Ƃ�
                     ��� 0
 *---------------------------------------------------------------------------*/
int SOCLi_SendCommandPacket(SOCLiSocketCommandPipe* pipe, SOCLiCommandPacket* command)
{
    s32 result;
    s32 flag_block;

    SDK_ASSERT(pipe);

    flag_block = (command && !SOCL_FLAGISBLOCK(command->h.flag_block)) ? OS_MESSAGE_NOBLOCK : OS_MESSAGE_BLOCK;
    result = OS_SendMessage(&pipe->queue, command, flag_block);

    if (!result)
    {
        SOCLi_FreeCommandPacket(command);
    }

    return result ? SOCL_ESUCCESS : SOCL_ENOBUFS;
}

int SOCLi_SendCommandPacketToCtrlPipe(SOCLSocket* socket, SOCLiCommandPacket* command)
{
    SOCLiSocketCommandPipe*     pipe = SOCLi_GetCtrlPipe(socket);

    SDK_ASSERT(pipe);
    return SOCLi_SendCommandPacket(pipe, command);
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ExecCommandPacket
  
  Description:  �R�}���h�p�P�b�g�̎��s(���M�������҂�)

  Arguments:    pipe		���M��̃R�}���h�p�C�v
                command         �R�}���h�̃p�P�b�g
  
  Returns:      command->flag_block �� NOBLOCK �̂Ƃ�
                     ���M�����Ȃ� SOCL_ESUCCESS=0
                         ���s�Ȃ� SOCL_ENOBUFS<0
                         ���s������R�}���h���������
                command->flag_block �� BLOCK �̂Ƃ�
                     �R�}���h�̃��^�[���R�[�h���Ԃ�
 *---------------------------------------------------------------------------*/
int SOCLi_ExecCommandPacket(SOCLiSocketCommandPipe* pipe, SOCLiCommandPacket* command)
{
    s32 result;

    SDK_ASSERT(pipe);
    SDK_ASSERT(command);

    if (command->h.flag_block == OS_MESSAGE_BLOCK)
    {
        OSMessage       response;
        OSMessageQueue  response_queue;

        OS_InitMessageQueue(&response_queue, &response, 1);
        command->h.response = &response_queue;

        (void)SOCLi_SendCommandPacket(pipe, command);
        (void)OS_ReceiveMessage(&response_queue, (void**) &result, OS_MESSAGE_BLOCK);
    }
    else
    {
        // command->h.flag_block == OS_MESSAGE_NOBLOCK/NORESULT
        command->h.response = NULL;
        result = SOCLi_SendCommandPacket(pipe, command);
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ExecCommandPacketInRecvPipe
                SOCLi_ExecCommandPacketInSendPipe
                SOCLi_ExecCommandPacketInCtrlPipe
  
  Description:  �R�}���h�p�P�b�g�� RecvPipe/SendPipe �Ŏ��s(���M�������҂�)

  Arguments:    socket          �\�P�b�g
                command         �R�}���h�̃p�P�b�g
  
  Returns:      command->flag_block �� NOBLOCK �̂Ƃ�
                     ���M�����Ȃ� SOCL_ESUCCESS=0
                         ���s�Ȃ� SOCL_ENOBUFS<0
                         ���s������R�}���h���������
                command->flag_block �� BLOCK �̂Ƃ�
                     ��� 0
 *---------------------------------------------------------------------------*/
int SOCLi_ExecCommandPacketInRecvPipe(SOCLSocket* socket, SOCLiCommandPacket* command)
{
    SOCLiSocketRecvCommandPipe*     pipe = socket->recv_pipe;

    SDK_ASSERT(pipe);
    return SOCLi_ExecCommandPacket(&pipe->h, command);
}

int SOCLi_ExecCommandPacketInSendPipe(SOCLSocket* socket, SOCLiCommandPacket* command)
{
    SOCLiSocketSendCommandPipe*     pipe = socket->send_pipe;

    SDK_ASSERT(pipe);
    return SOCLi_ExecCommandPacket(&pipe->h, command);
}

int SOCLi_ExecCommandPacketInCtrlPipe(SOCLSocket* socket, SOCLiCommandPacket* command)
{
    SOCLiSocketCommandPipe*     pipe = SOCLi_GetCtrlPipe(socket);

    SDK_ASSERT(pipe);
    return SOCLi_ExecCommandPacket(pipe, command);
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_CommandPacketHandler
  
  Description:  �R�}���h�p�P�b�g���������邽�߂̃X���b�h�̃G���g��
                �\�P�b�g�\���̂̃����o�[�͊��Ɏ��� SOCL_* �֐��ɂ���ĕύX����
                �Ă���\�������邽�߁A���̃X���b�h����\�P�b�g�\���̓�����
                �l�������I�ɂ͎Q�Ƃ��Ă͂Ȃ�Ȃ��D�K�v�Ȓl�̓R�}���h���b�Z�[�W
                �ɐς�œn�����ƁD�������A���̓�����l�����č��ꂽ�����̏ꍇ
                ���̌���ł͂Ȃ��D
                �܂����̃X���b�h�̗D�揇�ʂ� SOCL �֐����Ăяo������������
                �Ȃ���΂Ȃ�Ȃ��D
  
  Arguments:    arg             �҂��󂯃L���[
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOCLi_CommandPacketHandler(void* arg)
{
    SOCLiSocketCommandPipe*     cpipe = (SOCLiSocketCommandPipe*)arg;
    SOCLiCommandPacket*         command;
    OSIntrMode  enable;
    s32         result;

    //
    //  �R�}���h�����b�Z�[�W�Ƃ��Ď擾���A�w�肳�ꂽ�R�}���h�����s����
    //  ���b�Z�[�W�Ƃ��� NULL ��������ƏI��
    //
    while (1)
    {
        // �����̓r���� OS_KillThread �ŏ����X���b�h����~����\�������邽��
        // �������b�Z�[�W�𑗂�܂ŃR�}���h�����b�Z�[�W�L���[����O���Ȃ��D
        (void)OS_ReadMessage(&cpipe->queue, (void*) &command, OS_MESSAGE_BLOCK);

        if (command)
        {
            // �R�[���o�b�N�Ăяo���D
            result = command->h.handler(command);

            // �����I���ɂƂ��Ȃ����b�Z�[�W�̕��z�D
            //   - �L���[����R�}���h����폜
            //   - ���ʂ��\�P�b�g�\���̂֏����߂�
            //   - �K�v�Ȃ犮�����b�Z�[�W�𓊂���
            //   - �R�}���h�p�P�b�g�L���[�փR�}���h��ԋp����
            // ���̊֘A�����S�Ă��܂Ƃ߂ăA�g�~�b�N�ɂ��Ă���D
            // �����̃��b�Z�[�W�֘A�����S�Ă� NOBLOCK �łȂ��ƃn���O����\��
            // ������̂Œ��ӁD
            enable = OS_DisableInterrupts();
            (void)OS_DisableScheduler();
            (void)OS_ReceiveMessage(&cpipe->queue, (void*)NULL, OS_MESSAGE_NOBLOCK);

            if (command->h.socket)
            {
                command->h.socket->result = result;
            }

            if (command->h.response)
            {
                (void)OS_SendMessage(command->h.response, (void*)result, OS_MESSAGE_NOBLOCK);
            }

            SOCLi_FreeCommandPacket(command);   // NOBLOCK
            (void)OS_EnableScheduler();
            (void)OS_RestoreInterrupts(enable);

            // ����� OS_ReadMessage() ���ŏ�L�̃��b�Z�[�W���M�����ɂ����
            // �N���������̗D��x�̍����X���b�h�Ɏ��s�����Ϗ������
        }
        else
        {
            break;
        }
    }

    return;
}

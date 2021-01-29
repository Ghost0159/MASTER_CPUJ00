/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_write.c

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_write.c,v $
  Revision 1.35  2007/06/20 09:04:26  yasu
  �R�l�N�V�����̕s�ӂ̒f��̑Ή�

  Revision 1.34  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.33  2006/01/17 07:26:22  yasu
  ����N�x 2005->2006 �ύX

  Revision 1.32  2006/01/17 07:24:36  yasu
  TCP BLOCK Write �ɂ�����s��̏C���̍Ēǉ�
  BLOCK ���[�h����p�̃}�N���̓���

  Revision 1.31  2005/10/20 09:08:14  yasu
  ���M���s���ɂ����� Write �o�b�t�@���J�����Ă��Ȃ��s��C��

  Revision 1.30  2005/10/18 06:12:44  terui
  TCP���M�X���[�v�b�g����Ɋ֘A����ύX

  Revision 1.29  2005/09/19 15:11:37  yasu
  Closing ��Ԃ�ǉ����������̔�����

  Revision 1.28  2005/09/19 14:08:29  yasu
  Closing ��Ԃ�ǉ�����

  Revision 1.27  2005/09/19 04:51:49  seiki_masashi
  small fix

  Revision 1.26  2005/09/17 11:54:52  yasu
  typo �C��

  Revision 1.25  2005/09/17 10:14:00  yasu
  SocWrite �̃G���[���̃n���h�����N������������

  Revision 1.24  2005/09/13 07:27:06  yada
  applied for connect() on UDP

  Revision 1.23  2005/09/08 05:35:34  yasu
  �\�P�b�g�f�X�N���v�^���L���l�ł��邩�ǂ����̔���� SOCL ���ōs�Ȃ����Ƃɂ���

  Revision 1.22  2005/08/24 09:25:13  yasu
  SOCL_SocketIsInvalid �ǉ�

  Revision 1.21  2005/08/11 10:06:09  yasu
  write �ł� UDP �o�C���h���ʂ� CPS �w�ɔ��f������

  Revision 1.20  2005/08/11 07:56:32  yasu
  UDP �� Write ���ɖ����I�� E�|�[�g���擾����

  Revision 1.19  2005/08/11 07:30:03  yasu
  Write �R�}���h�̃p�C�v���M���̃G���[�n���h�����O�̏C��

  Revision 1.18  2005/08/11 05:08:51  yasu
  �����O�o�b�t�@�Ǘ��̏C��

  Revision 1.17  2005/08/10 12:34:04  yasu
  �o�b�t�@�̑���ύX

  Revision 1.16  2005/08/10 04:51:12  yasu
  �����O�o�b�t�@ full ���̏����C��

  Revision 1.15  2005/08/09 12:27:12  yasu
  �����O�o�b�t�@����̃f�[�^�R�s�[�����̏C��

  Revision 1.14  2005/08/09 07:43:29  yasu
  LANG=en �ł̃R���p�C���Ή�

  Revision 1.13  2005/08/09 04:31:10  yasu
  ASSERT �����̐��U����̏C��

  Revision 1.12  2005/08/09 03:20:39  yasu
  �������Ă����f�o�b�N���b�Z�[�W�폜

  Revision 1.11  2005/08/09 01:21:02  yasu
  sndbuf �T�C�Y�̐��������̏C��

  Revision 1.10  2005/08/08 14:16:36  yasu
  TCP �� send �����̑Ή�

  Revision 1.9  2005/08/01 07:02:50  seiki_masashi
  non-blocking ���Ƀo�b�t�@�̋󂫂��Ȃ��ꍇ�� EWOULDBLOCK ��Ԃ��悤�ɏC��

  Revision 1.8  2005/08/01 04:55:25  yasu
  SOCL_Write �̕Ԓl�̏C��

  Revision 1.7  2005/07/30 22:30:14  yasu
  �f���������悤�ɏC��

  Revision 1.6  2005/07/30 15:30:52  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.5  2005/07/28 11:16:12  yasu
  ���L�\�ȃR�[�h��؂�o���Ċ֐���

  Revision 1.4  2005/07/27 12:19:00  yasu
  sendto �̒ǉ�

  Revision 1.3  2005/07/25 10:15:01  yasu
  SOC ���C�u�����̒ǉ�

  Revision 1.2  2005/07/22 12:44:56  yasu
  �񓯊�����������

  Revision 1.1  2005/07/19 14:06:33  yasu
  SOCL_Read/Write �ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>

static int  SOCLi_WriteBuffer(SOCLSocket* , const u8* , s32, u16, SOCLInAddr, s32);
static int  SOCLi_ExecWriteCommand(SOCLSocket* , const u8* , s32, s32, u16, SOCLInAddr, s32);
static int  SOCLi_WriteCallBack(void* );
static s32  SOCLi_AllocWriteBuffer(SOCLSocket* , s32, s32, s32* , s32);
static int  SOCLi_MemCpy(u8* , int, SOCLiCommandWrite* );

#if SOCL_OPTIMIZE_TCP_SEND
static int  SOCLi_GetOptimumSendBufLen(int);
#endif

/*---------------------------------------------------------------------------*
  Name:         SOCL_Write

  Description:  �\�P�b�g����f�[�^�𑗂�܂��D

  Arguments:    socket          �\�P�b�g
                buffer          ���M�f�[�^
                buffer_len      ���M�f�[�^�̒���
                remote_port     ���M��z�X�g port
                remote_ip       ���M��z�X�g ip

                remote_port/remote_ip �� Connect ���s�Ȃ��Ă��Ȃ� UDP �̏ꍇ
                �̂ݗL��. TCP �� Connect ���s�Ȃ��� UDP �̏ꍇ�͒l�͖��������

  Returns:      ���̒l: ��M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
int SOCL_Write(int s, const void* buffer, int buffer_len, u16 remote_port, SOCLInAddr remote_ip)
{
    return SOCL_WriteTo(s, buffer, buffer_len, remote_port, remote_ip, 0);
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_WriteNoBlock

  Description:  �\�P�b�g���� NOBLOCK ���[�h�Ńf�[�^�𑗂�܂��D

  Arguments:    socket          �\�P�b�g
                buffer          ���M�f�[�^
                buffer_len      ���M�f�[�^�̒���
                remote_port     ���M��z�X�g port (UDP �̂ݗL��)
                remote_ip       ���M��z�X�g ip   (UDP �̂ݗL��)

  Returns:      ���̒l: ���M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
int SOCL_WriteNoBlock(int s, const void* buffer, int buffer_len, u16 remote_port, SOCLInAddr remote_ip)
{
    return SOCL_WriteTo(s, buffer, buffer_len, remote_port, remote_ip, SOCL_MSG_DONTWAIT);
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_WriteBlock

  Description:  �\�P�b�g���� BLOCK ���[�h�Ńf�[�^�𑗂�܂��D

  Arguments:    socket          �\�P�b�g
                buffer          ���M�f�[�^
                buffer_len      ���M�f�[�^�̒���
                remote_port     ���M��z�X�g port (UDP �̂ݗL��)
                remote_ip       ���M��z�X�g ip   (UDP �̂ݗL��)

  Returns:      ���̒l: ���M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
int SOCL_WriteBlock(int s, const void* buffer, int buffer_len, u16 remote_port, SOCLInAddr remote_ip)
{
    return SOCL_WriteTo(s, buffer, buffer_len, remote_port, remote_ip, 0);
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_WriteTo

  Description:  �\�P�b�g����f�[�^�𑗂�܂��D

  Arguments:    socket          �\�P�b�g
                buffer          ���M�f�[�^
                buffer_len      ���M�f�[�^�̒���
                remote_port     ���M��z�X�g port
                remote_ip       ���M��z�X�g ip
                flag            ���M�t���O

                remote_port/remote_ip �� Connect ���s�Ȃ��Ă��Ȃ� UDP �̏ꍇ
                �̂ݗL��. TCP �� Connect ���s�Ȃ��� UDP �̏ꍇ�͒l�͖��������

  Returns:      ���̒l: ��M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
int SOCL_WriteTo(int s, const void* buffer, int buffer_len, u16 remote_port, SOCLInAddr remote_ip, int flags)
{
    SOCLSocket*     socket = (SOCLSocket*)s;
    SOCLiSocketSendCommandPipe*     send_pipe;
    int flag_block;
    int result;

    if (SOCL_SocketIsInvalid(socket))
    {
        return SOCL_EINVAL;     // �K��O�̒l
    }

    if (!SOCL_SocketIsCreated(socket))
    {
        return SOCL_ENETRESET;  // ����������Ă��Ȃ�
    }

    if (SOCL_SocketIsTCP(socket) && (!SOCL_SocketIsConnected(socket) || SOCL_SocketIsClosing(socket)))
    {
        return SOCL_ENOTCONN;   // �ڑ����Ă��Ȃ�
    }

    SDK_ASSERT(buffer && buffer_len);
    send_pipe = socket->send_pipe;
    SDK_ASSERT(send_pipe);      // �������̂��������������

    //
    // Lock �������đ��̃X���b�h����̃A�N�Z�X��r������D
    // ���Ƀo�b�t�@�̓����ɃA�N�Z�X���鏈���͒��Ӑ[���r����������K�v����
    //
    if ((flags & SOCL_MSG_DONTWAIT) || SOCL_SocketIsNoBlock(socket))
    {
        if (!OS_TryLockMutex(&send_pipe->h.in_use))
        {
            return SOCL_EWOULDBLOCK;
        }

        flag_block = SOCL_FLAGBLOCK_NOBLOCK;
    }
    else
    {
        OS_LockMutex(&send_pipe->h.in_use);
        flag_block = SOCL_FLAGBLOCK_BLOCK;
    }

    result = SOCLi_WriteBuffer(socket, buffer, buffer_len, remote_port, remote_ip, flag_block);
    OS_UnlockMutex(&send_pipe->h.in_use);

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_WriteBuffer

  Description:  WRITE �������J�n���܂��D�������r������͍s�Ȃ��Ă��܂���D

  Arguments:    socket          �\�P�b�g
                buffer          �R�s�[�f�[�^
                buffer_len      �R�s�[�f�[�^�̒���
                remote_port     ���M��z�X�g port (UDP �̂ݗL��)
                remote_ip       ���M��z�X�g ip   (UDP �̂ݗL��)
                flag_block      BLOCK ���邩�ǂ����̃t���O

  Returns:      �R�s�[�ł����f�[�^�� (0 �Ȃ玸�s)
 *---------------------------------------------------------------------------*/
static int SOCLi_WriteBuffer(SOCLSocket*  socket, const u8*  buffer, s32 buffer_len, u16 remote_port,
                             SOCLInAddr remote_ip, s32 flag_block)
{
    SOCLiSocketSendCommandPipe*     send_pipe = socket->send_pipe;
    s32 sndbuf_len;
    s32 wrtbuf_in;
    s32 write_len;
    s32 buffer_min;
    s32 copied_len = 0;
#ifdef SDK_DEBUG
    s32 wrtbuf_size = (s32) send_pipe->buffer.area.size - 1;
#endif
    int result;

    SDK_ASSERT(send_pipe);              // �������̂��������������
    SDK_ASSERT(send_pipe->exe_socket);  // �������̂��������������

    // ���M�X���b�g(sndbuf)�̃o�b�t�@�T�C�Y���擾����
    sndbuf_len = (s32) send_pipe->exe_socket->cps_socket.sndbuf.size;

    if (SOCL_SocketIsUDP(socket))
    {
        sndbuf_len -= SOCL_UDP_SNDBUF_NOCOPY_OFFSET;

        // UDP �̏ꍇ�Asndbuf/wrtbuf �̃T�C�Y�����]���T�C�Y���傫���ꍇ��
        // �]���s��
        // SEND PIPE �ɐڑ�����Ă��� CPS �\�P�b�g�� sndbuf �T�C�Y���m�F����
        SDK_ASSERT(sndbuf_len >= 0);

        if (buffer_len > sndbuf_len)
        {
            return SOCL_EMSGSIZE;
        }

        // UDP �̓]���͈�x�ōs�Ȃ��邽�߃f�[�^���m�ۂł���\���ȑ傫����
        // �o�b�t�@���K�v
        buffer_min = buffer_len;
    }
    else
    {
        sndbuf_len -= SOCL_TCP_SNDBUF_NOCOPY_OFFSET;

        // TCP �̓]���̏ꍇ�̓o�b�t�@�����Ȃ���Ε�������
        // 2�̃|���V�[������
#ifdef SDK_TCPSEND_DOIT_SOON
        // NOBLOCK ���ɏ����ł��o�b�t�@���󂯂΋l�ߍ���
        buffer_min = (!SOCL_FLAGISBLOCK(flag_block)) ? 1 : MATH_MIN(buffer_len, sndbuf_len);
#else
        // NOBLOCK ���ł��l�ߍ��܂Ȃ�
        buffer_min = MATH_MIN(buffer_len, sndbuf_len);
#endif
    }

    SDK_ASSERT(wrtbuf_size >= sndbuf_len);

    while (buffer_len > 0)
    {
        //
        // �o�b�t�@���m�ۂ���
        //   BLOCK   ���͍Œ�ł� buffer_min �����̑傫�����Ԃ�܂� BLOCK ����
        //   NOBLOCK ���� buffer_min �����̑傫�����擾�ł��Ȃ���� 0 �ŕԂ�
        write_len = SOCLi_AllocWriteBuffer(socket, buffer_len, buffer_min, &wrtbuf_in, flag_block);

        //
        // �m�ۂł�����R�}���h�����J�n
        //
        if (write_len > 0)
        {
            result = SOCLi_ExecWriteCommand(socket, buffer, write_len, wrtbuf_in, remote_port, remote_ip, flag_block);

            if (result <= 0)
            {
                // SOCLi_ExecWriteCommand �� ERROR �ɂȂ�̂� NOBLOCK ���̂�
                return SOCL_EWOULDBLOCK;
            }

            buffer += write_len;
            buffer_len -= write_len;
            copied_len += write_len;
        }

        //
        // NoBlock �Ȃ烋�[�v���ŏI��
        //
        if (!SOCL_FLAGISBLOCK(flag_block))
        {
            if (write_len <= 0)
            {
                // �o�b�t�@�ɋ󂫂��Ȃ����Ƃ�ʒm
                return SOCL_EWOULDBLOCK;
            }
            break;
        }
    }

    return (int)copied_len;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_AllocWriteBuffer

  Description:  �]���p�o�b�t�@�Ƃ��Ċm�ۉ\�ȃT�C�Y���擾���܂��D

                �v�����������̃o�b�t�@���m�ۉ\�ɂȂ�܂� BLOCK ���܂��D
                0 ���w�肵���ꍇ�� NOBLOCK �̈����ɂȂ�܂��D

  Arguments:    socket               �\�P�b�g
                max_size             �擾����o�b�t�@�̍ő�
                min_size             �擾����o�b�t�@�̍ŏ�
                wrtbuf_in_ptr        �o�b�t�@�̓��͐擪
                flag_block           �擾�ł���܂� BLOCK ����H

  Returns:      �m�ۉ\�ȃo�b�t�@��
 *---------------------------------------------------------------------------*/
static s32 SOCLi_AllocWriteBuffer(SOCLSocket* socket, s32 max_size, s32 min_size, s32* wrtbuf_in_ptr, s32 flag_block)
{
    SOCLiSocketSendCommandPipe*     send_pipe = socket->send_pipe;
#ifdef SDK_DEBUG
    s32         wrtbuf_size = (s32) send_pipe->buffer.area.size - 1;
#endif
    s32         free_size;
    OSIntrMode  enable;

    SDK_ASSERT(send_pipe);
    SDK_ASSERT(wrtbuf_size >= min_size);

    //
    // �����ő�T�C�Y�ƍŏ��T�C�Y���������Ă�����ő�T�C�Y��D��
    // �ő�T�C�Y�ȏ�̃f�[�^�͐�΂ɕK�v�łȂ�
    // �ŏ��T�C�Y�͓]�����������シ�邽�߂̐ݒ�ł��邽��
    //
    if (min_size > max_size)
    {
        min_size = max_size;
    }

    //
    // �����O�o�b�t�@�̃|�C���^���擾
    //   wrtbuf_in �����͑�/wrtbuf_out ���o�͑�
    //
    enable = OS_DisableInterrupts();
    while (1)
    {
        free_size = SOCLi_GetWriteBufferFreeSize(socket);

        //
        // �o�b�t�@�����\���ɂ���ꍇ�͏I��
        // max_size �𒴂��Ȃ��悤�ɏ����ݒ肷��
        //
        if (free_size >= min_size)
        {
            if (free_size >= max_size)
            {
                free_size = max_size;
            }

            *wrtbuf_in_ptr = (s32) send_pipe->buffer.in;
            break;
        }

        //
        // NOBLOCK �Ȃ� BLOCK �����Ɏ擾�T�C�Y 0 �ŏI��
        //
        if (!SOCL_FLAGISBLOCK(flag_block))
        {
            free_size = 0;
            break;
        }

        //
        // �o�b�t�@���\���ɓ����Ȃ��Ƃ��̓o�b�t�@���󂭂܂ő҂�
        //   �T�C�Y������Ȃ����Ƃ̔��f�̊J�n���� SleepThread ����
        //   �o�b�t�@�󂫑҂��ɓ���܂ł� WriteCallBack ������Ȃ�
        //   �悤�ɂ��Ȃ���΂Ȃ�Ȃ�
        //
        OS_SleepThread(&send_pipe->buffer.waiting);

    }
    (void)OS_RestoreInterrupts(enable);

    // �����O�o�b�t�@�̋󂫗̈��Ԃ�
    return free_size;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_GetWriteBufferFreeSize

  Description:  �󂫃o�b�t�@�ʂ�Ԃ��܂�.
                ���̊֐��͊��荞�݋֎~��ԂŌĂ΂�Ȃ���΂����܂���D

  Arguments:    socket                �\�P�b�g

  Returns:      �󂫃o�b�t�@�T�C�Y
 *---------------------------------------------------------------------------*/
s32 SOCLi_GetWriteBufferFreeSize(SOCLSocket* socket)
{
    SOCLiSocketSendCommandPipe*     send_pipe = socket->send_pipe;
    s32 wrtbuf_size;
    s32 wrtbuf_in;
    s32 wrtbuf_out;
    s32 available_len;

    SDK_ASSERT(send_pipe);

    wrtbuf_size = (s32) send_pipe->buffer.area.size;
    wrtbuf_in = (s32) send_pipe->buffer.in;
    wrtbuf_out = (s32) send_pipe->buffer.out;
    available_len = wrtbuf_out - wrtbuf_in - 1;

    if (available_len < 0)
    {
        available_len += wrtbuf_size;
    }

    return available_len;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ExecWriteCommand

  Description:  WRITE �������J�n���܂��D�������r������͍s�Ȃ��Ă��܂���D
                �܂����O�Ƀ����O�o�b�t�@�փf�[�^���R�s�[�ł��邱�Ƃ��m�F����
                �����K�v������܂��D

  Arguments:    socket                �\�P�b�g
                buffer                �R�s�[�f�[�^
                buffer_len            �R�s�[�f�[�^�̒���
                wrtbuf_in             �����O�o�b�t�@�ւ̓����ʒu
                remote_port           ���M��z�X�g port (UDP �̂ݗL��)
                remote_ip             ���M��z�X�g ip   (UDP �̂ݗL��)

  Returns:      �R�s�[�ł����f�[�^�� (0 �Ȃ玸�s)
 *---------------------------------------------------------------------------*/
static int SOCLi_ExecWriteCommand(SOCLSocket*  socket, const u8*  buffer, s32 buffer_len, s32 wrtbuf_in, u16 remote_port,
                                  SOCLInAddr remote_ip, s32 flag_block)
{
    SOCLiSocketSendCommandPipe*     send_pipe = socket->send_pipe;
    SOCLiCommandPacket*         command;
    u8*     wrtbuf;
    s32     wrtbuf_size;
    int     result;
    u16     saved_send_buffer_in;

#ifdef SDK_MY_DEBUG
    OS_TPrintf("socket=%08x exe_socket=%08x\n", socket, send_pipe->exe_socket);
#endif

    // ���M�X���b�h�ɃR�}���h�𓊂��A�R�[���o�b�N�֐������� CPS_Write ��
    // �N������D
    //    �R�}���h�p�P�b�g���擾����̂� flag_block �Ɋ�Â������[�h��
    //    �������邪�Awrite �̏����Ɋւ��Ă� NORESULT �ōs�Ȃ��D
    //
    command = SOCLi_CreateCommandPacket(SOCLi_WriteCallBack, send_pipe->exe_socket, flag_block);
    if (NULL == command)
    {
        return SOCL_EMFILE;
    }

    if (SOCL_FLAGISBLOCK(flag_block) && !SOCL_SocketIsUDP(socket))
    {
        command->h.flag_block = SOCL_FLAGBLOCK_NORESULT;
    }
    else
    {
        command->h.flag_block = SOCL_FLAGBLOCK_NOBLOCK;
    }

    wrtbuf = (u8*)send_pipe->buffer.area.data;
    wrtbuf_size = (s32) send_pipe->buffer.area.size;

    // �f�[�^������ WriteBuffer �֓]������
    //    �P�`�����N�ő����ꍇ�ƂQ�`�����N�ɕ����Ȃ��Ƃ����Ȃ��ꍇ��
    //    �Ή�����D
    //
    if (wrtbuf_in + buffer_len < wrtbuf_size)
    {
        command->write.buffer1 = wrtbuf + wrtbuf_in;
        command->write.buffer1_len = buffer_len;
        command->write.buffer2 = NULL;
        command->write.buffer2_len = 0;
        wrtbuf_in += buffer_len;
    }
    else
    {
        const u8*   buffer2;

        command->write.buffer1 = wrtbuf + wrtbuf_in;
        command->write.buffer1_len = wrtbuf_size - wrtbuf_in;
        command->write.buffer2 = wrtbuf;
        command->write.buffer2_len = buffer_len - command->write.buffer1_len;
        wrtbuf_in = command->write.buffer2_len;
        buffer2 = buffer + command->write.buffer1_len;
        MI_CpuCopy8(buffer2, command->write.buffer2, (u32) command->write.buffer2_len);
    }

    MI_CpuCopy8(buffer, command->write.buffer1, (u32) command->write.buffer1_len);

    // �����O�o�b�t�@�̃|�C���^���ړ�������
    saved_send_buffer_in = send_pipe->buffer.in;
    send_pipe->buffer.in = command->write.wrtbuf_after = (u16) wrtbuf_in;

    //
    // Connect ����Ă��Ȃ� UDP �ʐM�ł���A����悪 write �̈�����
    // �w�肳��Ă���ꍇ�́A���葊�����R�}���h�p�P�b�g�Ɏw�肷��
    //
    // UDP ��
    if (SOCL_SocketIsUDP(socket))
    {
        if (socket->local_port == 0)
        {
            socket->local_port = CPS_SocGetEport(); // bind ����ĂȂ��Ȃ�o�C���h����
            socket->cps_socket.local_port = socket->local_port;
        }

        command->write.local_port = socket->local_port;

        // connect ����Ă��Ȃ� UDP
        if (socket->remote_ip == 0 || remote_ip != 0)
        {
            command->write.remote_ip = remote_ip;
            command->write.remote_port = remote_port;
        }

        // connect ����Ă��� UDP
        else
        {
            command->write.remote_ip = socket->remote_ip;
            command->write.remote_port = socket->remote_port;
        }
    }
    else
    {
        command->write.remote_ip = 0;               // Bind �Ȃ�
    }

    result = SOCLi_ExecCommandPacketInSendPipe(send_pipe->exe_socket, command);

    if (result != SOCL_ESUCCESS)
    {
        // ���s����̂� NOBLOCK �̂Ƃ�
        // �Ԓl�� 0 �ɂ��A�o�b�t�@���J������
        send_pipe->buffer.in = saved_send_buffer_in;
        buffer_len = 0;
    }

    return (int)buffer_len;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_WriteCallBack

  Description:  WRITE �����̃R�[���o�b�N�D

  Arguments:    arg   �R�}���h�p�P�b�g�ւ̃|�C���^

  Returns:      ���̒l: ���M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
static int SOCLi_WriteCallBack(void* arg)
{
    SOCLiCommandWrite*  cpacket = (SOCLiCommandWrite*)arg;
    SOCLSocket*         socket = cpacket->h.socket;
    SOCLiSocketSendCommandPipe*     send_pipe = socket->send_pipe;
    u8*     sndbuf;
    int     sndbuf_len;
    int     sndbuf_offset;
    int     copied = 0;
    int     result;

    SDK_ASSERT(send_pipe);

    if ((!SOCL_SocketIsTCP(socket)) || SOCL_SocketIsConnected(socket))  // Closing �̏ꍇ�͏������s����
    {
        // remote_ip �Ɏw�肪�������Ƃ��́ABind �������s�Ȃ�
        //   ����� UDP �ڑ��̏ꍇ�Ɏg�p����
        if (cpacket->remote_ip)
        {
#ifdef SDK_MY_DEBUG
            OS_TPrintf("WriteCallBack-BIND:%d %d %08x\n", cpacket->local_port, cpacket->remote_port, cpacket->remote_ip);
#endif
            CPS_SocBind(cpacket->local_port, cpacket->remote_port, cpacket->remote_ip);
        }

        //
        // �����O�o�b�t�@����̃f�[�^�̎��o���D
        // ���͂� sndbuf �ɃR�s�[���Ă��� SocWrite �֐����Ăяo���D
        // message queue �ɂ���ăV���A���C�Y����Ă��邽�߁Amutex �Ȃ��ł� sndbuf ��
        // �r�����Ȃ���Ă���
        //
#ifdef SDK_MY_DEBUG
        OS_TPrintf("WriteCallBack-WRITE 1: %08x %d\n", cpacket->buffer1, cpacket->buffer1_len);
        OS_TPrintf("                    2: %08x %d\n", cpacket->buffer2, cpacket->buffer2_len);
        OS_DumpThreadList();
        OS_TPrintf("  CPS->rcvbuf %08x%5d\n", socket->cps_socket.rcvbuf.data, socket->cps_socket.rcvbuf.size);
        OS_TPrintf("  CPS->sndbuf %08x%5d\n", socket->cps_socket.sndbuf.data, socket->cps_socket.sndbuf.size);
        OS_TPrintf("  CPS->linbuf %08x%5d\n", socket->cps_socket.linbuf.data, socket->cps_socket.linbuf.size);
        OS_TPrintf("  CPS->outbuf %08x%5d\n", socket->cps_socket.outbuf.data, socket->cps_socket.outbuf.size);
#endif

        sndbuf_offset = SOCL_SocketIsTCP(socket) ? SOCL_TCP_SNDBUF_NOCOPY_OFFSET : SOCL_UDP_SNDBUF_NOCOPY_OFFSET;
        sndbuf = socket->cps_socket.sndbuf.data + sndbuf_offset;
#if SOCL_OPTIMIZE_TCP_SEND
        if (SOCL_SocketIsTCP(socket))
        {
            /* ���M�X���[�v�b�g�����コ���邽�߁A��x�ɑ��M�����݂�f�[�^�T�C�Y���œK�� */
            sndbuf_len = SOCLi_GetOptimumSendBufLen((int)socket->cps_socket.sndbuf.size - sndbuf_offset);
        }
        else
        {
            sndbuf_len = (int)socket->cps_socket.sndbuf.size - sndbuf_offset;
        }

#else
        sndbuf_len = (int)socket->cps_socket.sndbuf.size - sndbuf_offset;
#endif

        for (;;)
        {
            result = SOCLi_MemCpy(sndbuf, sndbuf_len, cpacket);
            if (result <= 0)
            {
                break;
            }

#ifdef SDK_MY_DEBUG
            OS_TPrintf("CPS_SocWrite.sendbuf      =%d\n", sndbuf);
            OS_TPrintf("            .sendlen      =%d\n", result);
#endif
            result = (int)CPS_SocWrite(sndbuf, (u32) result);

#ifdef SDK_MY_DEBUG
            OS_TPrintf("CPS_SocWrite.result       =%d\n", result);
#endif

            if (result <= 0)
            {
                // �ڑ����؂ꂽ�ꍇ
                if (SOCL_SocketIsTCP(socket))
                {
                    // ������x�R�l�N�g���珈��������K�v������D
                    socket->state &= ~(SOCL_STATUS_CONNECTING | SOCL_STATUS_CONNECTED | SOCL_STATUS_CLOSING);
                    socket->state |=  SOCL_STATUS_RESET;  // �ڑ��؂�� READ ���ɒm�点�邽�߂̃t���O
                    
                    // �u���b�N����
                    CPS_TcpClose();
                }

                copied = SOCL_ETIMEDOUT;
                break;
            }

            copied += result;
        }
    }
    else
    {
        copied = SOCL_ETIMEDOUT;    // �r���Őڑ����؂�Ă���
    }

#ifdef SDK_MY_DEBUG
    OS_TPrintf("WriteCallBack-WRITE.copied=%d\n", copied);
#endif

    // �����O�o�b�t�@���J�����A�J���҂��ɂȂ��Ă���X���b�h��������N����
    // ���̃��b�Z�[�W���󂯎�邽�߂ɂ́A�r�������ɋC�����邱��
    // �G���[���N���Ă��o�b�t�@���󂯂�
    send_pipe->buffer.out = cpacket->wrtbuf_after;
    OS_WakeupThread(&send_pipe->buffer.waiting);

    return copied;
}

#if SOCL_OPTIMIZE_TCP_SEND

/*---------------------------------------------------------------------------*
  Name:         SOCLi_GetOptimumSendBufLen

  Description:  TCP���M���A�œK�ȃX���[�v�b�g�𓾂邽�߂�CPS_SocWrite�ɗ^����
                ���M�f�[�^�T�C�Y���擾����B

  Arguments:    len - ����T�C�Y���w�肷��B

  Returns:      int - �œK�ȑ��M�f�[�^�T�C�Y��Ԃ��B
 *---------------------------------------------------------------------------*/
static int SOCLi_GetOptimumSendBufLen(int len)
{
    CPSSoc*     soc = CPS_CURSOC;
    int         temp = 0;
    OSIntrMode  enabled = OS_DisableInterrupts();

    if (soc != NULL)
    {
        if ((soc->remote_mss > 0) && (soc->remote_win > 0))
        {
            temp = 2 * MATH_IMin(MATH_IMin(soc->remote_mss, soc->remote_win), (int)SOCLiCPSConfig.mymss);
        }
        else
        {
            temp = 2 * (int)SOCLiCPSConfig.mymss;
        }
    }
    else
    {
        temp = 2 * (int)SOCLiCPSConfig.mymss;
    }
    (void)OS_RestoreInterrupts(enabled);

    if (temp > 0)
    {
        int count = len / temp;

        if (count > 0)
        {
            return temp * count;
        }
    }

    return len;
}
#endif

/*---------------------------------------------------------------------------*
  Name:         SOCLi_MemCpy

  Description:  2 �̃o�b�t�@�ɂ܂������Ă���f�[�^�� COPY ����

  Arguments:    socket  �\�P�b�g
                cpacket �o�b�t�@�f�[�^�̃p�����[�^

  Returns:      �� or 0: ���M�����f�[�^�̃o�C�g��
 *---------------------------------------------------------------------------*/
static int SOCLi_MemCpy(u8* sndbuf_ptr, int sndbuf_len, SOCLiCommandWrite* cpacket)
{
    s32 buffer1_len = cpacket->buffer1_len;
    s32 buffer2_len = cpacket->buffer2_len;

    if (buffer1_len > sndbuf_len)
    {
        // buffer1 �̈ꕔ�ő����ꍇ
        buffer1_len = sndbuf_len;
        buffer2_len = 0;
    }
    else if (buffer2_len > sndbuf_len - buffer1_len)
    {
        // buffer1 �S���� buffer2 �̈ꕔ�ő����ꍇ
        buffer2_len = sndbuf_len - buffer1_len;
    }
    else
    {
        // buffer1 �S���� buffer2 �S���ł�����Ȃ��ꍇ
        // ���̂܂� buffer1_len, buffer2_len �łn�j
    }

    if (buffer1_len > 0)
    {
        MI_CpuCopy8(cpacket->buffer1, sndbuf_ptr, (u32) buffer1_len);
        cpacket->buffer1 += buffer1_len;
        cpacket->buffer1_len -= buffer1_len;
    }

    if (buffer2_len > 0)
    {
        MI_CpuCopy8(cpacket->buffer2, sndbuf_ptr + buffer1_len, (u32) buffer2_len);
        cpacket->buffer2 += buffer2_len;
        cpacket->buffer2_len -= buffer2_len;
    }

    return buffer1_len + buffer2_len;
}

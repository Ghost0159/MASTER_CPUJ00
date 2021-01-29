/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_read.c

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_read.c,v $
  Revision 1.41  2007/06/20 09:04:26  yasu
  �R�l�N�V�����̕s�ӂ̒f��̑Ή�

  Revision 1.40  2006/07/24 01:35:37  adachi_hiroaki
  UDP �o�b�t�@�����ӂ�Ă��Ă��A���̃p�P�b�g�̎�M�����𐳂����s���悤�ɏC�� [nitro-ip:0170]

  Revision 1.39  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.38  2005/12/06 11:53:13  yasu
  TCP BLOCK Write �ɂ�����s��̏C��
  BLOCK ���[�h����p�̃}�N���̓���

  Revision 1.37  2005/09/19 14:08:29  yasu
  Closing ��Ԃ�ǉ�����

  Revision 1.36  2005/09/16 12:13:50  yasu
  �C�x���g�J�E���^�� FinalRom �ł��L���ɂ���

  Revision 1.35  2005/09/14 06:00:19  yasu
  SOCLi_CheckCount �̃T�|�[�g

  Revision 1.34  2005/09/09 07:38:57  yasu
  Block �󋵂̔�����C��

  Revision 1.33  2005/09/08 05:35:34  yasu
  �\�P�b�g�f�X�N���v�^���L���l�ł��邩�ǂ����̔���� SOCL ���ōs�Ȃ����Ƃɂ���

  Revision 1.32  2005/09/01 06:42:55  yasu
  �\�P�b�g�������Ă��邩�ǂ����̔�����ꌳ��

  Revision 1.31  2005/08/25 08:27:38  yasu
  UDP ��M�ɂ�����o�b�t�@�����O�����ǉ�

  Revision 1.30  2005/08/24 09:25:13  yasu
  SOCL_SocketIsInvalid �ǉ�

  Revision 1.29  2005/08/14 10:59:34  yasu
  consumed_min ����̒ǉ�

  Revision 1.28  2005/08/13 08:20:27  yasu
  consumed_min ����̒ǉ�

  Revision 1.27  2005/08/12 09:50:23  yasu
  EOF �ɒB�����Ƃ��̐ڑ��X�e�[�^�X�̐ݒ��ǉ�

  Revision 1.26  2005/08/11 12:02:50  yasu
  consume ��� UDP �\�P�b�g�̐ڑ����Z�b�g�����̊Â��������������̂��C��

  Revision 1.25  2005/08/11 10:07:15  yasu
  read ���̃��[�J���|�[�g�� UDP �o�C���h���ʂ�ۑ�����

  Revision 1.24  2005/08/11 07:02:34  yasu
  �f�o�b�O�o�͂̍폜

  Revision 1.23  2005/08/10 12:34:53  yasu
  �o�b�t�@�̑���ύX

  Revision 1.22  2005/08/10 09:31:44  seiki_masashi
  SSL �̃T�|�[�g��ǉ�

  Revision 1.21  2005/08/10 04:49:13  yasu
  ��M���̕Ԃ�l��ύX

  Revision 1.20  2005/08/09 12:26:33  yasu
  consume �����̏C��

  Revision 1.19  2005/08/09 03:20:14  yasu
  TCP/UDP �� consume �����̕ύX

  Revision 1.18  2005/08/08 14:15:18  yasu
  read �̕Ԓl��ύX

  Revision 1.17  2005/08/07 13:04:49  yasu
  Read �̕Ԓl�̍ďC��

  Revision 1.16  2005/08/04 04:15:56  yasu
  �������o�O�C��

  Revision 1.15  2005/08/04 04:08:28  yasu
  UDP read �̕Ԃ�l�̏C��

  Revision 1.14  2005/08/02 13:12:32  seiki_masashi
  EOF ����p�萔�� enum �l�ɒu������

  Revision 1.13  2005/08/02 10:50:09  yasu
  NoBlock �� EOF ���菈���̎b��Ή�

  Revision 1.12  2005/08/02 09:40:11  yasu
  BLCOK READ ���[�h���̏����C���ǉ�

  Revision 1.11  2005/08/02 08:13:12  yasu
  READ �� EOF �̎b������D
  BLCOK READ ���[�h���̏����C��

  Revision 1.10  2005/08/02 07:00:58  yasu
  NOBLOCK ���[�h�ɂ����� EOF �Ή�

  Revision 1.9  2005/08/01 07:04:27  seiki_masashi
  non-blocking ���Ƀf�[�^���Ȃ��ꍇ�� EWOULDBLOCK ��Ԃ��悤�ɏC��
  blocking ���� SOCLi_ExecReadCommand �ɗ^����������C��

  Revision 1.8  2005/08/01 06:26:36  yasu
  SOCLi_ReadCallBack�@�ɂ�����f�[�^���Ȃ��Ƃ��̕Ԓl�̕ύX

  Revision 1.7  2005/08/01 05:55:26  yasu
  recv_pipe->consumed �̌��Z�^�C�~���O���C��

  Revision 1.6  2005/07/30 22:30:14  yasu
  �f���������悤�ɏC��

  Revision 1.5  2005/07/30 15:30:29  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.4  2005/07/25 10:15:01  yasu
  SOC ���C�u�����̒ǉ�

  Revision 1.3  2005/07/22 12:44:56  yasu
  �񓯊�����������

  Revision 1.2  2005/07/19 14:06:33  yasu
  SOCL_Read/Write �ǉ�

  Revision 1.1  2005/07/15 13:33:11  yasu
  soc ���C�u�����̃\�[�X����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>

static int  SOCLi_ReadAndConsumeBuffer(SOCLSocket* , void* , int, u16* , SOCLInAddr* , s32, s32);
static int  SOCLi_ReadBuffer(SOCLSocket* , void* , int, u16* , SOCLInAddr* , s32);
static int  SOCLi_ReadUdpBuffer(SOCLSocket* , void* , int, u16* , SOCLInAddr* , s32);
static int  SOCLi_CopyCPSBuffer(SOCLSocket* , void* , int, u16* , SOCLInAddr* );
static u8*  SOCLi_ReadCPSBuffer(SOCLSocket* , s32* , u16* , u16* , SOCLInAddr* );
static int  SOCLi_ExecReadCommand(SOCLSocket* , void* , int, u16* , SOCLInAddr* );
static int  SOCLi_ReadCallBack(void* );
static int  SOCLi_ConsumeBuffer(SOCLSocket* );
static int  SOCLi_ConsumeCallBack(void* );
static int  SOCLi_ConsumeCPSBuffer(SOCLSocket* );

u32         SOCLi_CheckCount[SOCL_CHECKCOUNT_MAX] = { 0};

/*---------------------------------------------------------------------------*
  Name:         SOCL_Read

  Description:  �\�P�b�g����f�[�^���󂯎��܂�

  Arguments:    socket          �\�P�b�g
                buffer          ��M�f�[�^
                buffer_len      ��M�f�[�^���i�[�o�b�t�@�̒���
                remote_port     ���M���z�X�g port
                remote_ip       ���M���z�X�g ip

                remote_port/remote_ip �� Connect ���s�Ȃ��Ă��Ȃ� UDP �̏ꍇ
                �̂ݗL��. TCP �� Connect ���s�Ȃ��� UDP �̏ꍇ�͒l�͖��������

  Returns:      ���̒l: ��M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
int SOCL_Read(int s, void* buffer, int buffer_len, u16* remote_port, SOCLInAddr* remote_ip)
{
    return SOCL_ReadFrom(s, buffer, buffer_len, remote_port, remote_ip, 0);
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_ReadNoBlock

  Description:  �\�P�b�g���� NOBLOCK ���[�h�Ńf�[�^���󂯎��܂��D

  Arguments:    socket          �\�P�b�g
                buffer          ��M�f�[�^
                buffer_len      ��M�f�[�^�̒���
                remote_port     ���M���z�X�g port (UDP �̂ݗL��)
                remote_ip       ���M���z�X�g ip   (UDP �̂ݗL��)

  Returns:      ���̒l: ��M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
int SOCL_ReadNoBlock(int s, void* buffer, int buffer_len, u16* remote_port, SOCLInAddr* remote_ip)
{
    return SOCL_ReadFrom(s, buffer, buffer_len, remote_port, remote_ip, SOCL_MSG_DONTWAIT);
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_ReadBlock

  Description:  �\�P�b�g���� BLOCK ���[�h�Ńf�[�^���󂯎��܂��D

  Arguments:    socket          �\�P�b�g
                buffer          ��M�f�[�^
                buffer_len      ��M�f�[�^�̒���
                remote_port     ���M���z�X�g port (UDP �̂ݗL��)
                remote_ip       ���M���z�X�g ip   (UDP �̂ݗL��)

  Returns:      ���̒l: ��M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
int SOCL_ReadBlock(int s, void* buffer, int buffer_len, u16* remote_port, SOCLInAddr* remote_ip)
{
    return SOCL_ReadFrom(s, buffer, buffer_len, remote_port, remote_ip, 0);
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_ReadFrom

  Description:  �\�P�b�g����f�[�^���󂯎��܂�

  Arguments:    socket          �\�P�b�g
                buffer          ��M�f�[�^
                buffer_len      ��M�f�[�^���i�[�o�b�t�@�̒���
                remote_port     ���M���z�X�g port
                remote_ip       ���M���z�X�g ip
                flag            ��M�t���O

                remote_port/remote_ip �� Connect ���s�Ȃ��Ă��Ȃ� UDP �̏ꍇ
                �̂ݗL��. TCP �� Connect ���s�Ȃ��� UDP �̏ꍇ�͒l�͖��������

  Returns:      ���̒l: ��M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
int SOCL_ReadFrom(int s, void* buffer, int buffer_len, u16* remote_port, SOCLInAddr* remote_ip, int flags)
{
    SOCLSocket*     socket = (SOCLSocket*)s;
    SOCLiSocketRecvCommandPipe*     recv_pipe;
    int flag_block;
    int result;

    if (SOCL_SocketIsInvalid(socket))
    {
        return SOCL_EINVAL;     // �K��O�̒l
    }

    if ((flags & SOCL_MSG_DONTWAIT) || SOCL_SocketIsNoBlock(socket))
    {
        if (SOCL_SocketIsSSL(socket))
        {
            return SOCL_EINVAL; // SSL �̓u���b�L���O�Ăяo�������T�|�[�g���Ȃ�
        }

        flag_block = SOCL_FLAGBLOCK_NOBLOCK;
    }
    else
    {
        if (OS_GetProcMode() == OS_PROCMODE_IRQ)
        {
            return SOCL_EINVAL; // �u���b�L���O�Ăяo���͊��荞�݃n���h�����ŌĂяo���Ȃ�
        }

        flag_block = SOCL_FLAGBLOCK_BLOCK;
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
    recv_pipe = socket->recv_pipe;
    SDK_ASSERT(recv_pipe);      // �������̂��������������
    if (!SOCL_FLAGISBLOCK(flag_block))
    {
        if (!OS_TryLockMutex(&recv_pipe->h.in_use))
        {
            return SOCL_EWOULDBLOCK;
        }
    }
    else
    {
        OS_LockMutex(&recv_pipe->h.in_use);
    }

    result = SOCLi_ReadAndConsumeBuffer(socket, buffer, buffer_len, remote_port, remote_ip, flag_block, flags);

    OS_UnlockMutex(&recv_pipe->h.in_use);
    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ReadAndConsumeBuffer

  Description:  READ ����� Consume �������J�n���܂��D
                �������r������͍s�Ȃ��Ă��܂���D

  Arguments:    socket          �\�P�b�g
                buffer          �R�s�[�f�[�^
                buffer_len      �R�s�[�f�[�^�̒���
                remote_port     ���M���z�X�g port (UDP �̂ݗL��)
                remote_ip       ���M���z�X�g ip   (UDP �̂ݗL��)
                flag_block      BLOCK ���邩�ǂ����̃t���O

  Returns:      �R�s�[�ł����f�[�^�� (0 �Ȃ玸�s)
 *---------------------------------------------------------------------------*/
static int SOCLi_ReadAndConsumeBuffer(SOCLSocket*  socket, void*  buffer, int buffer_len, u16*  remote_port,
                                      SOCLInAddr*  remote_ip, s32 flag_block, s32 flags)
{
    SOCLiSocketRecvCommandPipe*     recv_pipe = socket->recv_pipe;
    BOOL    is_noconsumed = ((flags & SOCL_MSG_PEEK) && recv_pipe);
    int     result;
    s8      flag_noconsume;

    if (is_noconsumed)
    {
        // �ꎞ�I�� noconsume �t���O�𗧂Ă�
        flag_noconsume = recv_pipe->flag_noconsume;
        recv_pipe->flag_noconsume = TRUE;
    }

    if (SOCL_SocketIsUDP(socket))
    {
        result = SOCLi_ReadUdpBuffer(socket, buffer, buffer_len, remote_port, remote_ip, flag_block);
    }
    else
    {
        result = SOCLi_ReadBuffer(socket, buffer, buffer_len, remote_port, remote_ip, flag_block);

        if (result >= 0)
        {
            (void)SOCLi_ConsumeBuffer(socket);
        }
    }

    if (is_noconsumed)
    {
        // noconsume �t���O�����ɖ߂�
        recv_pipe->flag_noconsume = flag_noconsume;
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ReadBuffer

  Description:  READ �������J�n���܂��D�������r������͍s�Ȃ��Ă��܂���D

  Arguments:    socket          �\�P�b�g
                buffer          �R�s�[�f�[�^
                buffer_len      �R�s�[�f�[�^�̒���
                remote_port     ���M���z�X�g port (UDP �̂ݗL��)
                remote_ip       ���M���z�X�g ip   (UDP �̂ݗL��)
                flag_block      BLOCK ���邩�ǂ����̃t���O
                                BLOCK �Ȃ�R�}���h���쐬���Ď��s���܂��D

  Returns:      �R�s�[�ł����f�[�^�� (0 �Ȃ玸�s)
 *---------------------------------------------------------------------------*/
static int SOCLi_ReadBuffer(SOCLSocket*  socket, void*  buffer, int buffer_len, u16*  remote_port, SOCLInAddr*  remote_ip,
                            s32 flag_block)
{
    int result;

#ifdef SDK_MY_DEBUG
    OS_TPrintf("\n===\nReadBuffer.buffer                 =%d-%d %d\n", buffer, (u32) buffer + buffer_len, buffer_len);
#endif

    // SSL �̏ꍇ�� CPS �̎�M�o�b�t�@��`�����邱�Ƃ��ł��Ȃ����߁A
    // �K����M�X���b�h�ɃR�}���h�𔭍s����
    if (SOCL_SocketIsSSL(socket))
    {
        result = SOCLi_ExecReadCommand(socket, buffer, buffer_len, remote_port, remote_ip);
        return result;
    }

    //
    // �܂��ABLOCK/NOBLOCK �Ɋ�炸�� NOBLOCK ���[�h�𔭍s����D
    // ���� BLOCK ���[�h�̏ꍇ EWOULDBLOCK ���Ԃ��Ă����ꍇ�A���̂����f�[�^
    // ���������邱�Ƃ����҂��� recv_pipe �� READ �R�}���h�𔭍s����
    //
    result = SOCLi_CopyCPSBuffer(socket, buffer, buffer_len, remote_port, remote_ip);

    if (result == SOCL_EWOULDBLOCK && SOCL_FLAGISBLOCK(flag_block))
    {
        result = SOCLi_ExecReadCommand(socket, buffer, buffer_len, remote_port, remote_ip);
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_CopyCPSBuffer

  Description:  CPS ���C�u�����̎�M�o�b�t�@����f�[�^���R�s�[����
                NOBLOCK ���[�h�����݂�Ƃ����`���ɂȂ��Ă���

  Arguments:    socket          �\�P�b�g
                buffer          �R�s�[���M�o�b�t�@�^
                buffer_len      �R�s�[���M�o�b�t�@�̒���
                remote_port     ��M����̃|�[�g�ւ̃|�C���^
                remote_ip       ��M����� IP �ւ̃|�C���^

  Returns:      ���̒l: TCP �o�b�t�@�փR�s�[�����T�C�Y
                        UDP �o�b�t�@�֑����Ă��Ă����T�C�Y
                0     : EOF
                ���̒l: �G���[
                        SOCL_EWOULDBLOCK: �f�[�^�������Ă��Ă��Ȃ�
                        SOCL_EINVAL     : ������Ԃ����������Ȃ��Ă���
 *---------------------------------------------------------------------------*/
static int SOCLi_CopyCPSBuffer(SOCLSocket*  socket, void*  buffer, int buffer_len, u16*  remote_port,
                               SOCLInAddr*  remote_ip)
{
    OSIntrMode  enable;
    SOCLiSocketRecvCommandPipe*     recv_pipe;
    u8*         cpsbuffer;
    s32         cpsbuffer_len;
    s32         copy_len;
    s32         consume_len;
    u16         my_port;
    u16         his_port;
    SOCLInAddr  his_ip;
    int         result = 0;

    SDK_ASSERT(buffer && buffer_len > 0);

    //
    // �f�[�^�̃R�s�[���� Consume ���|���邱�Ƃ�h�~����
    // Mutex �ɂ�����@�����邪�����ł͊��荞�݂ŏ������s�Ȃ��D
    //
    enable = OS_DisableInterrupts();
    {
        //
        // ��M�o�b�t�@�ɒ����Ă���f�[�^���m�F���A�f�[�^������Ȃ�o�b�t�@��
        // �R�s�[����D�����łȂ��ABLOCK ���[�h�ł���Ȃ� READ �R�}���h�𔭍s��
        // recv_pipe �œ]�����s�Ȃ�
        //
        cpsbuffer = SOCLi_ReadCPSBuffer(socket, &cpsbuffer_len, &my_port, &his_port, &his_ip);

        if (cpsbuffer)
        {
            if (cpsbuffer_len == 0)
            {
                // �|�C���^�̓Z�b�g����Ă��ăT�C�Y�� 0 �̂Ƃ���
                // �f�[�^���͂��Ă��Ȃ�
                result = SOCL_EWOULDBLOCK;
            }
            else
            {
                // �f�[�^�̃R�s�[�T�C�Y�A����T�C�Y���v�Z����
                //   �������Ă���f�[�^�T�C�Y���o�b�t�@�T�C�Y�܂ŃR�s�[����
                //   TCP �̓R�s�[�������� UDP �͓������Ă���f�[�^�S�Ă�
                //   �̂Ă�
                copy_len = MATH_MIN(buffer_len, cpsbuffer_len);
                consume_len = SOCL_SocketIsTCP(socket) ? copy_len : cpsbuffer_len;

                // �R�s�[���p��
                // PEEK �ǂ݂̏ꍇ�͔p�����Ȃ�
                MI_CpuCopy8(cpsbuffer, buffer, (u32) copy_len); // �R�s�[
#ifdef SDK_MY_DEBUG
                OS_TPrintf("MI_CPUCOPY    .buffer             =%d-%d %d\n", buffer, (u32) buffer + copy_len, copy_len);
                OS_TPrintf("              .copy_len           =%d\n", copy_len);
#endif
                recv_pipe = socket->recv_pipe;
                SDK_ASSERT(recv_pipe);
                if (!recv_pipe->flag_noconsume)
                {
                    recv_pipe->consumed += consume_len;         // �p��
                }

#ifdef SDK_MY_DEBUG
                OS_TPrintf("              .recv_pipe->consumed=%d\n", recv_pipe->consumed);
#endif
                result = (int)consume_len;
            }
        }
        else
        {
            // cpsbuffer �̃|�C���^���Ԃ炸�ɃT�C�Y�� 0 �Ȃ� EOF
            // 0 ��Ԃ�
            if (cpsbuffer_len == 0)
            {
                result = 0;
            }
            else
            {
                result = SOCL_EINVAL;   // System error
            }

            socket->state &= ~(SOCL_STATUS_CONNECTING | SOCL_STATUS_CONNECTED);
        }

        if (result >= 0)
        {
            //
            // ���� IP/Port �̃R�s�[
            // ���[�J���|�[�g�̊m��
            //
            if (remote_port && remote_ip)
            {
                *remote_port = his_port;
                *remote_ip = his_ip;
            }

            if (socket->local_port == 0)
            {
                socket->local_port = my_port;
            }
        }
    }
    (void)OS_RestoreInterrupts(enable);

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ReadCPSBuffer

  Description:  CPS ���C�u�����̎�M�o�b�t�@�̏�Ԃ𒲂ׂ�

  Arguments:    socket          �\�P�b�g
                buffer_len      ��M�o�b�t�@�̒����ւ̃|�C���^
                local_port      ���z�X�g�̃|�[�g�ւ̃|�C���^
                remote_port     ��M����̃|�[�g�ւ̃|�C���^
                remote_ip       ��M����� IP �ւ̃|�C���^

  Returns:      �� 0: ��M�o�b�t�@�̐擪�A�h���X
                        buffer_len �ɃT�C�Y���Ԃ�
                        0 �Ȃ�f�[�^�������̂�҂��Ă�����
                   0: �G���[ or EOF
                        ���̂Ƃ� buffer_len �ɃG���[�R�[�h���Ԃ�
                        0 �Ȃ� EOF  ���̒l�Ȃ�V�X�e���G���[
 *---------------------------------------------------------------------------*/
static u8* SOCLi_ReadCPSBuffer(SOCLSocket*  socket, s32*  buffer_len, u16*  local_port, u16*  remote_port,
                               SOCLInAddr*  remote_ip)
{
    SOCLiSocketRecvCommandPipe*     recv_pipe;
    CPSSoc*     cpsoc;
    s32         consumed;
    s32         len;

    recv_pipe = socket->recv_pipe;
    cpsoc = (CPSSoc*)OSi_GetSpecificData(&recv_pipe->h.thread, OSi_SPECIFIC_CPS);

    consumed = recv_pipe->consumed;
    len = (s32) cpsoc->rcvbufp - consumed;

    if (len >= 0)
    {
        (*local_port) = cpsoc->local_port;
        (*remote_port) = cpsoc->remote_port;
        (*remote_ip) = (SOCLInAddr) cpsoc->remote_ip;
        (*buffer_len) = len;

        // EOF ����
        if (len == 0 && cpsoc->state != CPS_STT_ESTABLISHED)
        {
            return NULL;
        }
    }
    else
    {
        (*buffer_len) = -1;
        return NULL;    // Error
    }

    return((u8*)cpsoc->rcvbuf.data) + consumed;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ExecReadCommand

  Description:  BLOCK READ ���s�R�}���h

  Arguments:    s                 �\�P�b�g
                user_buffer       READ ��
                user_buffer_len   READ �o�b�t�@�T�C�Y

  Returns:      ���̒l: ��M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
static int SOCLi_ExecReadCommand(SOCLSocket*  socket, void*  buffer, int buffer_len, u16*  remote_port,
                                 SOCLInAddr*  remote_ip)
{
    SOCLiCommandPacket*     command;
    s32 result;

    // recv_pipe �ɃR�}���h�𓊂��A�R�[���o�b�N�֐�������
    // CPS_Read/CPS_SocConsume ���N������D
    // �����I�� BLOCK ���[�h�Ȃ̂Ő�������܂ő҂�
    command = SOCLi_CreateCommandPacket(SOCLi_ReadCallBack, socket, SOCL_FLAGBLOCK_BLOCK);
    SDK_ASSERT(command);

    command->read.buffer = buffer;
    command->read.buffer_len = buffer_len;
    command->read.remote_port = remote_port;
    command->read.remote_ip = remote_ip;

    // �R�}���h���s
    result = SOCLi_ExecCommandPacketInRecvPipe(socket, command);

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ReadCallBack

  Description:  READ �����̃R�[���o�b�N�D
                BLOCK ���[�h�̂Ƃ��̂�

  Arguments:    arg   �R�}���h�p�P�b�g�ւ̃|�C���^

  Returns:      ���̒l: ��M�������b�Z�[�W�̃o�C�g��
                     0: EOF
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
static int SOCLi_ReadCallBack(void* arg)
{
    SOCLiCommandRead*   cpacket = (SOCLiCommandRead*)arg;
    SOCLSocket*         socket = cpacket->h.socket;
    SOCLiSocketRecvCommandPipe*     recv_pipe = socket->recv_pipe;
    u8*         buffer = cpacket->buffer;
    s32         buffer_len = cpacket->buffer_len;
    u16*        remote_port = cpacket->remote_port;
    SOCLInAddr*     remote_ip = cpacket->remote_ip;
    u8*         cpsbuffer;
    u32 cpsbuffer_len;
    s32 consumed;
    int result;

    // �V�����f�[�^���ǉ������܂� BLOCK ���[�h�ő҂�.
    // ����ł� SOCL ���C���[�ɂ����Ė{�@�\���������邽�߂� OS_Sleep ��
    // �g�����|�[�����O���s�Ȃ��Ă��邪�ACPS ���C���[�̓����̎������C��
    // ���ĐV�K�f�[�^�������� BLOCK �������s�Ȃ��ׂ��D
    consumed = recv_pipe->consumed;
    for (;;)
    {
        cpsbuffer = CPS_SocRead(&cpsbuffer_len);
        if (cpsbuffer == NULL || (s32) cpsbuffer_len - consumed > 0)
        {
            break;
        }

        if (SOCL_SocketIsTCP(socket) && socket->cps_socket.state != CPS_STT_ESTABLISHED)
        {
            cpsbuffer = NULL;   // EOF
            break;
        }

        OS_Sleep(10);           // 10msec �̑҂�
    }

    // �ؒf���ꂽ�Ȃ�G���[
    if (SOCL_SocketIsReset(socket))
    {
        return SOCL_ECONNRESET;
    }
    
    // SSL �̏ꍇ�͎�M�o�b�t�@��`������Ȃ����߂�
    // �P���� cpsbuffer �����M�f�[�^���R�s�[����
    // SSL �͕K���u���b�L���O�ŌĂяo����邽�߁A�����͒P���ƂȂ�
    if (SOCL_SocketIsSSL(socket))
    {
        u32 len;

        if (cpsbuffer == NULL)
        {
            return 0;
        }

        len = MATH_MIN(buffer_len, cpsbuffer_len);
        MI_CpuCopy8(cpsbuffer, buffer, len);
        CPS_SocConsume(len);
        return (int)len;
    }

    // CPS_SocRead �Ŋ��Ƀo�b�t�@�Ƀf�[�^���ǉ�����Ă��邱�Ƃ����҂ł���
    // ���߁A���̌Ăяo���͒����ɏI������Dcpsbuffer �� 0 �̎��� EOF �ƂȂ�
    if (cpsbuffer)
    {
        result = SOCLi_CopyCPSBuffer(socket, buffer, buffer_len, remote_port, remote_ip);
    }
    else
    {
        result = 0 /*EOF*/ ;
    }

    // �G���[�܂��� EOF �Ȃ炻�̕Ԓl�ŏI��
    if (result <= 0)
    {
        return result;
    }

    //
    // consumed �� consumed �̍ŏ����ȏ�Ȃ� Consume �������s�Ȃ�
    //
    SDK_ASSERT(recv_pipe->consumed_min > 0);
    if (recv_pipe->consumed >= (s32) recv_pipe->consumed_min)
    {
        (void)SOCLi_ConsumeCPSBuffer(socket);
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ConsumeBuffer

  Description:  Consume �������s�Ȃ�

  Arguments:    s                 �\�P�b�g

  Returns:      ���̒l: ��M�������b�Z�[�W�̃o�C�g��
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
static int SOCLi_ConsumeBuffer(SOCLSocket* socket)
{
    SOCLiSocketRecvCommandPipe*     recv_pipe = socket->recv_pipe;
    SOCLiCommandPacket*         command;
    s32 result;

    //
    // consume �� consume �̍ŏ��������Ȃ牽�������ɏI������
    //
    SDK_ASSERT(recv_pipe->consumed_min > 0);
    if (recv_pipe->consumed < (s32) recv_pipe->consumed_min)
    {
        return SOCL_ESUCCESS;
    }

    // recv_pipe �ɃR�}���h�𓊂��A�R�[���o�b�N�֐�������
    // CPS_SocConsume ���N������DNOBLOCK �ŏ�������
    command = SOCLi_CreateCommandPacket(SOCLi_ConsumeCallBack, socket, SOCL_FLAGBLOCK_NOBLOCK);
    if (NULL == command)
    {
        return SOCL_EMFILE;
    }

    // �R�}���h���s
    result = SOCLi_ExecCommandPacketInRecvPipe(socket, command);

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ConsumeCallBack

  Description:  Consume �����̃R�[���o�b�N�D

  Arguments:    arg   �R�}���h�p�P�b�g�ւ̃|�C���^

  Returns:      consume �����f�[�^�̃o�C�g��
 *---------------------------------------------------------------------------*/
static int SOCLi_ConsumeCallBack(void* arg)
{
    SOCLiCommandConsume*    cpacket = (SOCLiCommandConsume*)arg;

    return SOCLi_ConsumeCPSBuffer(cpacket->h.socket);
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ConsumeCPSBuffer

  Description:  Consume �������s�Ȃ�

  Arguments:    recv_pipe ��M�p�C�v

  Returns:      consume �����f�[�^�̃o�C�g��
 *---------------------------------------------------------------------------*/
static int SOCLi_ConsumeCPSBuffer(SOCLSocket* socket)
{
    SOCLiSocketRecvCommandPipe*     recv_pipe = socket->recv_pipe;
    OSIntrMode  enable;
    s32         consumed;

    SDK_ASSERT(recv_pipe);

    // consumed �p�����[�^�𑀍삷��̂Ŋ��荞�݂��~�����A
    // consumed �� CPS_SocConsume ���ł̃p�����[�^�Ƃ̐���𓯒�������
    enable = OS_DisableInterrupts();
    consumed = recv_pipe->consumed;

    if (consumed)
    {
#ifdef SDK_MY_DEBUG
        OS_TPrintf("\nCPS_SocConsume(%d)\n", consumed);
#endif

        //
        // CPS_SocConsume �̓����ŁA�����o�b�t�@���J�������^�C�~���O��
        // BLOCKing �����O���ォ�� recv_pipe->consumed ��������^�C�~���O��
        // �ύX����K�v������D���̕����� CPS_SocConsume �̎����ˑ��Ȃ̂Œ���
        //
#if 1   // �����o�b�t�@�J���� BLOCKing ����ꍇ�͐�� consumed ��������(����)
        recv_pipe->consumed = 0;
        CPS_SocConsume((u32) consumed); // BLOCKED
#else // �����o�b�t�@�J���O BLOCKing ����ꍇ�͌�� consumed ��������
        CPS_SocConsume((u32) consumed); // BLOCKED
        recv_pipe->consumed -= consumed;
#endif

#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_SocConsume.cpssoc.state      =%d\n", socket->cps_socket.state);
        OS_TPrintf("              .cpssoc.rcvbufp    =%d\n", socket->cps_socket.rcvbufp);
        OS_TPrintf("              .recv_pipe.consumed=%d\n", consumed);
#endif
    }
    (void)OS_RestoreInterrupts(enable);

    return (int)consumed;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_ReadUdpBuffer

  Description:  READ �������J�n���܂��D�������r������͍s�Ȃ��Ă��܂���D

  Arguments:    socket          �\�P�b�g
                buffer          �R�s�[�f�[�^
                buffer_len      �R�s�[�f�[�^�̒���
                remote_port     ���M���z�X�g port (UDP �̂ݗL��)
                remote_ip       ���M���z�X�g ip   (UDP �̂ݗL��)
                flag_block      BLOCK ���邩�ǂ����̃t���O
                                BLOCK �Ȃ�R�}���h���쐬���Ď��s���܂��D

  Returns:      �R�s�[�ł����f�[�^�� (0 �Ȃ玸�s)
 *---------------------------------------------------------------------------*/
static int SOCLi_ReadUdpBuffer(SOCLSocket*  socket, void*  buffer, int buffer_len, u16*  remote_port,
                               SOCLInAddr*  remote_ip, s32 flag_block)
{
    SOCLiSocketRecvCommandPipe*     pipe = socket->recv_pipe;
    SOCLiSocketUdpData*         udpdata = pipe->udpdata.out;
    OSIntrMode  enable;
    int         result;

    enable = OS_DisableInterrupts();
    while (NULL == (udpdata = pipe->udpdata.out))
    {
        if (!SOCL_FLAGISBLOCK(flag_block))
        {
            result = SOCL_EWOULDBLOCK;
            break;
        }

        OS_SleepThread(&pipe->udpdata.waiting);

        if (SOCL_SocketIsInvalid(socket) || !SOCL_SocketIsCreated(socket))
        {
            result = SOCL_ENOTCONN;
            break;  // Sleep ���Ƀ\�P�b�g�� close ���ꂽ
        }
    }

    if (udpdata)
    {
        MI_CpuCopy8((u8*) &udpdata[1], buffer, (u32) MATH_MIN(buffer_len, udpdata->size));

        if (remote_port)
        {
            (*remote_port) = udpdata->remote_port;
        }

        if (remote_ip)
        {
            (*remote_ip) = udpdata->remote_ip;
        }

        result = udpdata->size;

        if (!pipe->flag_noconsume)
        {
            pipe->udpdata.out = udpdata->next;
            if (udpdata->next == NULL)
            {
                pipe->udpdata.in = NULL;
            }

            SOCLi_Free(udpdata);
            pipe->udpdata.size -= result;
        }
    }
    (void)OS_RestoreInterrupts(enable);

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_UdpRecvCallback

  Description:  UDP �p�P�b�g���M���̃R�[���o�b�N

  Arguments:    data  �f�[�^�ւ̃|�C���^
                len   �f�[�^��
                soc   CPSSoc�\���̂ւ̃|�C���^

  Returns:      1 : �o�b�t�@��j������
 *---------------------------------------------------------------------------*/
int SOCLi_UdpRecvCallback(u8* data, u32 len, CPSSoc* soc)
{
    SOCLSocket*     socket = (SOCLSocket*)soc;
    SOCLiSocketRecvCommandPipe*     pipe = socket->recv_pipe;
    SOCLiSocketUdpData*         udpheader;
    OSIntrMode  enable = OS_DisableInterrupts();

    if (pipe->udpdata.size_max >= pipe->udpdata.size + len)
    {
        // ��M�o�b�t�@�ւ̃R�s�[
        udpheader = SOCLi_Alloc(sizeof(SOCLiSocketUdpData) + len);

        if (udpheader)
        {
            pipe->udpdata.size += len;
            udpheader->next = NULL;
            udpheader->size = (u16) len;
            udpheader->remote_port = soc->remote_port;
            udpheader->remote_ip = soc->remote_ip;
            MI_CpuCopy8(data, (void*) &udpheader[1], len);

            if (pipe->udpdata.in)
            {
                pipe->udpdata.in->next = udpheader;
            }

            pipe->udpdata.in = udpheader;

            if (pipe->udpdata.out == NULL)
            {
                pipe->udpdata.out = udpheader;
            }
        }
        else
        {
            SOCLi_CheckCount[SOCL_CHECKCOUNT_UDPRCVCB_1]++;
        }
    }
    else
    {
        SOCLi_CheckCount[SOCL_CHECKCOUNT_UDPRCVCB_2]++;
    }
    
    // ���[�J���|�[�g�m��
    if (socket->local_port == 0)
    {
        socket->local_port = soc->local_port;
    }

    // ���̃f�[�^�̎�M����
    soc->remote_port = soc->remote_port_bound;
    soc->remote_ip = soc->remote_ip_bound;

    // �҂��Ă���X���b�h������Ȃ�N����
    OS_WakeupThread(&pipe->udpdata.waiting);

    (void)OS_RestoreInterrupts(enable);

    return 1;   // �f�[�^�j��
}

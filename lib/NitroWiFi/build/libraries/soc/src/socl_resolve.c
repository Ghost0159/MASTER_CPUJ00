/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_resolve.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_resolve.c,v $
  Revision 1.9  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.8  2005/09/19 14:08:29  yasu
  Closing ��Ԃ�ǉ�����

  Revision 1.7  2005/09/15 12:51:01  yasu
  DHCP Requested IP �T�|�[�g

  Revision 1.6  2005/08/24 09:25:13  yasu
  SOCL_SocketIsInvalid �ǉ�

  Revision 1.5  2005/08/17 11:48:13  yasu
  SOC_InetAtoN �C��

  Revision 1.4  2005/08/02 06:19:03  yasu
  SOCL_GetHostID �̊֐���

  Revision 1.3  2005/07/30 22:30:14  yasu
  �f���������悤�ɏC��

  Revision 1.2  2005/07/30 15:31:36  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.1  2005/07/25 10:15:01  yasu
  SOC ���C�u�����̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>

#define CPS_RCVBUF          SOCL_TCP_SOCKET_CPS_RCVBUF_SIZE
#define CPS_SNDBUF          SOCL_TCP_SOCKET_CPS_SNDBUF_SIZE
#define CPS_RCVBUF_ALIGNED  ((CPS_RCVBUF + 3) &~3)
#define CPS_SNDBUF_ALIGNED  ((CPS_SNDBUF + 3) &~3)

/*---------------------------------------------------------------------------*
  Name:         SOCL_Resolve

  Description:  DNS �T�[�o�� IP �A�h���X��₢���킹��D
  
  Arguments:    hostname        �z�X�g��
  
  Returns:      IP �A�h���X  0 �Ȃ�G���[
 *---------------------------------------------------------------------------*/
SOCLInAddr SOCL_Resolve(const char* hostname)
{
    u8*         buffers;
    CPSSoc      soc;
    SOCLInAddr  hostip;

    if (!hostname)
    {
        return 0;
    }

    buffers = SOCLi_Alloc(CPS_RCVBUF_ALIGNED + CPS_SNDBUF_ALIGNED);

    if (buffers == NULL)
    {
        return 0;
    }

    MI_CpuClear8(&soc, sizeof(CPSSoc)); // ��U 0 �N���A����
    soc.rcvbuf.data = buffers;
    soc.rcvbuf.size = CPS_RCVBUF;
    soc.sndbuf.data = buffers + CPS_RCVBUF_ALIGNED;
    soc.sndbuf.size = CPS_SNDBUF;

    CPS_SocRegister(&soc);
    hostip = CPS_Resolve((char*)hostname);
    CPS_SocUnRegister();

    SOCLi_Free(buffers);

    return hostip;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_InetAtoH

  Description:  �����\�L���ꂽ IP �A�h���X�𐔎��ɕϊ�����
  
  Arguments:    hostname        �����\�L���ꂽ IP �A�h���X
  
  Returns:      IP �A�h���X  0 �Ȃ�G���[
 *---------------------------------------------------------------------------*/
SOCLInAddr SOCL_InetAtoH(const char* hostname)
{
    SOCLInAddr  hostip;
    OSIntrMode  enable;
    CPSInAddr   dns0, dns1;

    // DNS �T�[�o�������ꎞ�I�ɒ�~�����A�������琔�l�ւ̕ϊ����s�Ȃ��D
    // �R�[�h�T�C�Y�̍팸�̂��߁ACPS ���̃��[�`�����g�p����D
    // �{���� CPS �����̕ϊ����[�`���𒼐ڌĂт����悤�ɂ���ׂ�
    enable = OS_DisableInterrupts();
    dns0 = CPSDnsIp[0];
    dns1 = CPSDnsIp[1];
    CPSDnsIp[0] = 0;
    CPSDnsIp[1] = 0;
    hostip = CPS_Resolve((char*)hostname);
    CPSDnsIp[0] = dns0;
    CPSDnsIp[1] = dns1;
    (void)OS_RestoreInterrupts(enable);

    return hostip;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_GetResolver/SOCL_SetResolver

  Description:  ���݂� DNS �T�[�o�� host ip �A�h���X���擾������ݒ肵���肷��
                �܂��l�b�g���[�N�ڑ����ł��Ă��ȂȂ畉�̒l�� SOCL_ENETRESET
                ��Ԃ��D
 *---------------------------------------------------------------------------*/
int SOCL_GetResolver(SOCLInAddr* dns1, SOCLInAddr* dns2)
{
    SDK_ASSERT(dns1);
    SDK_ASSERT(dns2);

    if (SOCL_GetHostID() == 0)
    {
        return SOCL_ENETRESET;
    }

    *dns1 = (SOCLInAddr) CPSDnsIp[0];
    *dns2 = (SOCLInAddr) CPSDnsIp[1];
    return 0;
}

int SOCL_SetResolver(const SOCLInAddr dns1, const SOCLInAddr dns2)
{
    if (SOCL_GetHostID() == 0)
    {
        return SOCL_ENETRESET;
    }

    CPSDnsIp[0] = (CPSInAddr) dns1;
    CPSDnsIp[1] = (CPSInAddr) dns2;
    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_GetRemote

  Description:  �\�P�b�g�̐ڑ���𒲂ׂ�
  
  Arguments:    s		�\�P�b�g
                port		�����[�g�z�X�g�̃|�[�g�ԍ�
                ip		�����[�g�z�X�g�� IP �A�h���X
  
  Returns:      ��or 0: ����
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :  
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOCL_GetRemote(int s, u16* port, SOCLInAddr* ip)
{
    SOCLSocket*     socket = (SOCLSocket*)s;

    if (!SOCL_SocketIsCreated(socket))
    {
        return SOCL_ENETRESET;  // ����������Ă��Ȃ�
    }

    if (SOCL_SocketIsTCP(socket) && (!SOCL_SocketIsConnected(socket) || SOCL_SocketIsClosing(socket)))
    {
        return SOCL_ENOTCONN;   // �ڑ����Ă��Ȃ�
    }

    *port = socket->cps_socket.remote_port;
    *ip = socket->cps_socket.remote_ip;

    return SOCL_ESUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_GetHostID

  Description:	���z�X�g�� host ip �A�h���X���擾����
                �܂��擾�ł��Ă��ȂȂ� 0 �ƂȂ�D
  
  Arguments:    �Ȃ�
  
  Returns:      �z�X�g�� IPv4 �� IP �A�h���X(HostByteOrder)
 *---------------------------------------------------------------------------*/
SOCLInAddr SOCL_GetHostID(void)
{
    // �����N���m�����Ă��Ȃ��Ă��A
    // ���荞�ݒ��łȂ��ADHCP ���N�G�X�g�Œ��Ȃ�X���[�v����
    if (CPSMyIp == 0)
    {
        if ((SOCLiDhcpState & (SOCL_DHCP_REQUEST | SOCL_DHCP_CALLBACK)) == SOCL_DHCP_REQUEST)
        {
            if (OS_GetProcMode() != OS_PROCMODE_IRQ)
            {
                OS_Sleep(10);   // 10ms �����𖾂��n��
            }
        }
    }
    else if (SOCLiRequestedIP == 0)
    {
        // IP �A�h���X���ۑ�����Ă��Ȃ��Ȃ�ۑ����Ă���
        SOCLiRequestedIP = CPSMyIp;
    }

    return (volatile SOCLInAddr)CPSMyIp;
}

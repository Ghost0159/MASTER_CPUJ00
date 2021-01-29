/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     soc.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: soc.c,v $
  Revision 1.31  2006/07/18 02:37:09  okubata_ryoma
  NitroSDK3.2PR�ȑO��STD_CopyLString�ƌ݊�����ۂ��߂̕ύX

  Revision 1.30  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.29  2005/09/17 11:51:58  yasu
  ConfigError ���̔���� Link �؂��ǉ�����

  Revision 1.28  2005/09/17 09:05:27  yasu
  �����N�؂�̃n���h�����O�ɂƂ��Ȃ� DHCP����̏C��

  Revision 1.27  2005/09/15 12:50:48  yasu
  Alloc/Free ����̏C��

  Revision 1.26  2005/09/08 05:35:34  yasu
  �\�P�b�g�f�X�N���v�^���L���l�ł��邩�ǂ����̔���� SOCL ���ōs�Ȃ����Ƃɂ���

  Revision 1.25  2005/09/02 08:00:13  yasu
  ���[�J���z�X�g�� IP �̃A�T�[�g���폜

  Revision 1.24  2005/08/19 12:49:49  yada
  SOC_GetHostByName() and SOC_GetHostByAddr() �Ńz�X�g����Ԃ��悤�ɂȂ���

  Revision 1.23  2005/08/19 05:05:23  yasu
  mtu/rwin �Ή�

  Revision 1.22  2005/08/17 11:48:13  yasu
  SOC_InetAtoN �C��

  Revision 1.21  2005/08/17 04:10:27  yasu
  SOC_Poll �̋@�\�ǉ�

  Revision 1.20  2005/08/12 14:20:58  yasu
  SOC_InetNtoP�@�̕Ԃ�������A�h���X�̃t�H�[�}�b�g���̏C��

  Revision 1.19  2005/08/12 09:53:07  yasu
  Recv/Send �ɂ���������̎w��� socket �\���̓��� block/noblock �ݒ�̒��a

  Revision 1.18  2005/08/09 02:01:04  yasu
  SOC_Poll �̃o�O�t�B�N�X

  Revision 1.17  2005/08/04 04:15:37  yasu
  SOC_MSG_PEEK ���Ή�

  Revision 1.16  2005/08/02 03:14:00  yasu
  Free �����̃��b�p�[������ɓ��삵�Ă��Ȃ��̂��C��

  Revision 1.15  2005/08/01 13:25:21  yasu
  SOC_Listen/Accept �̓������� socket �炵������

  Revision 1.14  2005/08/01 07:40:28  yasu
  �R�����g�ǉ�

  Revision 1.13  2005/07/30 15:34:08  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.12  2005/07/28 14:14:22  seiki_masashi
  SOC_InetNtoP, SOC_InetPtoN �̒ǉ�

  Revision 1.11  2005/07/28 11:26:31  yasu
  typo �o�O�C��

  Revision 1.10  2005/07/28 11:15:23  yasu
  SOC_Poll �̎���

  Revision 1.9  2005/07/27 13:18:01  yasu
  SOC_GetHostByAddr�@�̒ǉ�

  Revision 1.8  2005/07/27 12:21:44  yasu
  IP_AtoN �n�̖��߂̍��

  Revision 1.7  2005/07/27 12:01:10  yasu
  sendto �̒ǉ�

  Revision 1.6  2005/07/25 14:22:24  yasu
  SOC_Startup �� SOCL_Startup �̘A��

  Revision 1.5  2005/07/25 10:15:01  yasu
  SOC ���C�u�����̒ǉ�

  Revision 1.4  2005/07/22 14:11:11  yasu
  �o�O�C��

  Revision 1.3  2005/07/22 13:55:35  yasu
  listen accept �̒ǉ�

  Revision 1.2  2005/07/22 12:44:56  yasu
  �񓯊�����������

  Revision 1.1  2005/07/18 13:16:27  yasu
  ���M��M�X���b�h���쐬����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/soc.h>
#include <nitroWiFi/socl.h>

const u8    SOCAddrAny[SOC_ALEN] = {   0,  0,  0,  0};
const u8    SOCAddrLoopback[SOC_ALEN] = { 127,  0,  0,  1};
const u8    SOCAddrLimited[SOC_ALEN] = { 255,255,255,255};

/*---------------------------------------------------------------------------*
  Name:         SOC_Socket

  Description:  �\�P�b�g���쐬����
                ���������p�p�����[�^�̓f�t�H���g�ݒ���g�p

  Arguments:    pf              �v���g�R���t�@�~���[
                                ���݂� SOC_PF_INET ��ݒ肷�邱��
                type            �\�P�b�g�̎��
                                SOC_SOCK_STREAM : TCP �\�P�b�g
                                SOC_SOCK_DGRAM  : UDP �\�P�b�g
                protocol        �v���g�R���w��
                                ���݂� 0 ��ݒ肷�邱��

  Returns:      ��or 0: �\�P�b�g�L�q�q
                  ����ł� NDS �̃A�h���X��Ԃ� int �\���Ő��̒l�ł��邱�Ƃ�
                  ���p���ă|�C���^�̒l�����̂܂ܕԂ��Ă���D
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - SOC_ENOMEM:  �������[�s��
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOC_Socket(int pf, int type, int protocol)
{
    (void)pf;   // unused
    (void)protocol;

    SDK_ASSERT(pf == SOC_PF_INET);
    SDK_ASSERT(type == SOC_SOCK_STREAM || type == SOC_SOCK_DGRAM);
    SDK_ASSERT(protocol == 0);

    return(type == SOC_SOCK_STREAM) ? SOCL_TcpSocket() : SOCL_UdpSocket();
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Bind

  Description:  �\�P�b�g�̃��[�J���|�[�g���̐ݒ�(�o�C���h)���s�Ȃ�
                CPS �֐��ł͐ڑ���̃|�[�g�ƃA�h���X�������ɐݒ肵��
                �ڑ�����̂ł����ł̓\�P�b�g�\���̓����Ƀ��[�J���|�[�g��
                �ݒ�l��ۑ�����̂݁D

  Arguments:    s               �\�P�b�g�L�q�q
                sockAddr        ���[�J���̃|�[�g�ԍ��E�A�h���X
                    SOCSockAddr �^�� SOCSockAddrIn �^
                    �ȉ��͐ݒ��

                    SOCSockAddrIn  a;
                    MI_CpuClear8(&a, sizeof(a));
                    a.len    = sizeof(SOCSockAddrIn);
                    a.family = SOC_PF_INET;
                    a.port   = SOC_HtoNs(local_port)
                    a.addr   = SOC_HtoNl(SOC_INADDR_ANY);
                    SOC_Bind(socket, &a);

  Returns:      ��or 0: ����
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOC_Bind(int s, const void* sockAddr)
{
    const SOCSockAddrIn*    addr = (const SOCSockAddrIn*)sockAddr;

    SDK_ASSERT(addr != NULL);
    SDK_ASSERT(addr->len == sizeof(SOCSockAddrIn)); // not used
    SDK_ASSERT(addr->family == SOC_PF_INET);        // not used
    return SOCL_Bind(s, SOC_NtoHs(addr->port));
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Connect

  Description:  �\�P�b�g�̃����[�g�|�[�g���̐ݒ�(�o�C���h)���s�Ȃ�
                CPS �֐��ł͐ڑ���̃|�[�g�ƃA�h���X�������ɐݒ肵��
                �ڑ�����̂ł����ł� SOC_Bind �Ń\�P�b�g�\���̓����ɕۑ�
                ���ꂽ�ݒ�l���g�p����D

  Arguments:    s               �\�P�b�g�L�q�q
                sockAddr        �����[�g�̃|�[�g�ԍ��E�A�h���X
                    SOCSockAddr �^�� SOCSockAddrIn �^
                    �ȉ��͐ݒ��

                    SOCSockAddrIn  a;
                    MI_CpuClear8(&a, sizeof(a));
                    a.len    = sizeof(SOCSockAddrIn);
                    a.family = SOC_PF_INET;
                    a.port   = SOC_HtoNs(remote_port)
                    a.addr   = SOC_HtoNl(remote_IP_address);
                    SOC_Connect(socket, &a);

  Returns:      ��or 0: ����
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOC_Connect(int s, const void* sockAddr)
{
    const SOCSockAddrIn*    addr = (const SOCSockAddrIn*)sockAddr;

    SDK_ASSERT(addr != NULL);
    SDK_ASSERT(addr->len == sizeof(SOCSockAddrIn)); // not used
    SDK_ASSERT(addr->family == SOC_PF_INET);        // not used
    return SOCL_Connect(s, SOC_NtoHs(addr->port), SOC_NtoHl(addr->addr.addr));
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Read/SOC_Recv/SOC_RecvFrom

  Description:  �\�P�b�g����f�[�^��ǂݍ��ށD

  Arguments:    s               �\�P�b�g�L�q�q
                buf             �ǂݍ��݃o�b�t�@
                len             �ǂݍ��݃o�b�t�@�T�C�Y
                flag            �ǂݍ��ݎ��̐ݒ�t���O
                                ���݂� SOC_MSG_DONTWAIT �̂݃T�|�[�g
                sockFrom        �ǂݍ��݃f�[�^�𑗂��Ă����z�X�g���

  Returns:      ��or 0: ����
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOC_Read(int s, void* buf, int len)
{
    return SOCL_ReadFrom(s, buf, len, NULL, NULL, 0);
}

int SOC_Recv(int s, void* buf, int len, int flags)
{
    return SOCL_ReadFrom(s, buf, len, NULL, NULL, flags);
}

int SOC_RecvFrom(int s, void* buf, int len, int flags, void* sockFrom)
{
    u16         port;
    SOCLInAddr  ip;
    int         result;

    result = SOCL_ReadFrom(s, buf, len, &port, &ip, flags);

    if (result >= 0 && sockFrom)
    {
        SOCSockAddrIn*  addr = (SOCSockAddrIn*)sockFrom;
        addr->port = SOC_HtoNs(port);
        addr->addr.addr = SOC_HtoNl(ip);
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Write/SOC_Send

  Description:  �\�P�b�g�փf�[�^���������ށD

  Arguments:    s               �\�P�b�g�L�q�q
                buf             �������݃o�b�t�@
                len             �������݃o�b�t�@�T�C�Y
                flag            �ǂݍ��ݎ��̐ݒ�t���O
                                ���݂� SOC_MSG_DONTWAIT �̂݃T�|�[�g

  Returns:      ��or 0: ����
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOC_Write(int s, const void* buf, int len)
{
    return SOCL_WriteTo(s, buf, len, 0, (SOCLInAddr) 0, 0);
}

int SOC_Send(int s, const void* buf, int len, int flags)
{
    return SOCL_WriteTo(s, buf, len, 0, (SOCLInAddr) 0, flags);
}

int SOC_SendTo(int s, const void* buf, int len, int flags, const void* sockTo)
{
    SOCSockAddrIn*  addr;
    u16         port;
    SOCLInAddr  ip;

    if (sockTo)
    {
        addr = (SOCSockAddrIn*)sockTo;
        port = SOC_NtoHs(addr->port);
        ip = SOC_NtoHl(addr->addr.addr);
    }
    else
    {
        port = 0;
        ip = 0;
    }

    return SOCL_WriteTo(s, buf, len, port, ip, flags);
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Shutdown

  Description:  �\�P�b�g�� shutdown ����D

  Arguments:    s               �\�P�b�g�L�q�q
                how             �V���b�g�_�E�������̎��
                                SO_SHUT_RDWR ����M����

                                ���݁ACPS �֐��̐����ɂ��
                                SOC_SHUT_RDWR �݂̂̃T�|�[�g

  Returns:      ��or 0: ����
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOC_Shutdown(int s, int how)
{
    SDK_ASSERT(how == SOC_SHUT_RDWR);
    (void)how;  // unused
    return SOCL_Shutdown(s);
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Close

  Description:  �\�P�b�g���N���[�Y����D

  Arguments:    s               �\�P�b�g�L�q�q

  Returns:      ��or 0: ����
                ��    : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
int SOC_Close(int s)
{
    return SOCL_Close(s);
}

/*---------------------------------------------------------------------------*
  Name:         SOC_GetHostByName

  Description:  �z�X�g������z�X�g�G���g�����擾����
                CPS �̐�������擾�ł���̂� IP �A�h���X�݂̂ł���D

  Arguments:    name      �z�X�g��

  Returns:      hostent   �z�X�g�̃G���g�����\���̂ւ̃|�C���^
 *---------------------------------------------------------------------------*/
SOCHostEnt* SOC_GetHostByName(const char* name)
{
    static SOCHostEnt   hostent;
    static u8*          hostAddrList[2];
    static SOCLInAddr   hostip;
    static char         entName[SOC_MAXDNAME + 1];
    u32 h;

    h = (u32) SOCL_Resolve(name);
    if (h == 0)
    {
        return NULL;
    }
    
    // NitroSDK3.2PR�ȑO��STD_CopyLString�ƌ݊�����ۂ���
    MI_CpuClear8(entName, sizeof(entName));
    //---- �z�X�g���R�s�[
    (void)STD_CopyLString(&entName[0], name, SOC_MAXDNAME + 1);

    hostent.name = &entName[0];
    hostent.aliases = NULL;
    hostent.addrType = SOC_PF_INET;
    hostent.length = sizeof(SOCLInAddr);
    hostent.addrList = hostAddrList;
    hostAddrList[0] = (u8*) &hostip;
    hostAddrList[1] = NULL;
    hostip = SOC_HtoNl(h);

    return &hostent;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_GetHostByAddr

  Description:  �A�h���X����z�X�g�G���g�����擾����
                ���ۂ̓A�h���X���z�X�g�G���g���\���̓����Ɋi�[����݂̂ł���D

  Arguments:    s               �\�P�b�g�L�q�q

  Returns:      hostent   �z�X�g�̃G���g�����\���̂ւ̃|�C���^
 *---------------------------------------------------------------------------*/
SOCHostEnt* SOC_GetHostByAddr(const void* addr, int len, int type)
{
    static SOCHostEnt   hostent;
    static u8*          hostAddrList[2];
    static SOCLInAddr   hostip;
    static char         entName[SOC_MAXDNAME + 1];

    SDK_ASSERT(type == SOC_PF_INET);
    SDK_ASSERT(len == sizeof(SOCLInAddr));

    //---- �A�h���X�𕶎��\�L�ɕϊ�
    (void)SOC_InetNtoP(SOC_AF_INET, addr, &entName[0], SOC_MAXDNAME + 1);

    hostent.name = &entName[0];
    hostent.aliases = NULL;
    hostent.addrType = (s16) type;
    hostent.length = (s16) len;
    hostent.addrList = hostAddrList;
    hostAddrList[0] = (u8*) &hostip;
    hostAddrList[1] = NULL;
    hostip = *(SOCLInAddr*)addr;

    return &hostent;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_GetSockName

  Description:  �\�P�b�g�̃o�C���h����Ă��郍�[�J���A�h���X/�|�[�g���擾����

  Arguments:    s         �\�P�b�g
                sockAddr  �\�P�b�g�̃o�C���h��

                SOCSockAddrIn  a;
                a.len    = sizeof(SOCSockAddrIn);
                a.family = SOC_PF_INET;
                a.port   = SOC_HtoNs(local_port)
                a.addr   = SOC_HtoNl(local_ip);

  Returns:      ���� 0
                ���s ���̒l
 *---------------------------------------------------------------------------*/
int SOC_GetSockName(int s, void* sockAddr)
{
    SOCSockAddrIn*  addr = (SOCSockAddrIn*)sockAddr;
    u32 local_ip;
    u32 local_port;

    if (!s)
    {
        return SOC_ENETRESET;
    }

    local_ip = (u32) SOCL_GetHostID();
    local_port = (u32) SOCL_GetHostPort(s);

    if (local_ip == 0)
    {
        local_port = 0;
    }

    addr->len = sizeof(SOCSockAddrIn);
    addr->family = SOC_PF_INET;
    addr->port = SOC_HtoNs(local_port);
    addr->addr.addr = SOC_HtoNl(local_ip);

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_GetHostID

  Description:  �z�X�g�� IP �A�h���X���擾����D

  Arguments:    �Ȃ�

  Returns:      �z�X�g�� IPv4 �� IP �A�h���X(NetworkByteOrder)
 *---------------------------------------------------------------------------*/
long SOC_GetHostID(void)
{
    u32 hostid = (u32) SOCL_GetHostID();

    return (long)SOC_HtoNl(hostid);
}

/*---------------------------------------------------------------------------*
  Name:         SOC_GetResolver/SOC_SetResolver

  Description:  ���݂� DNS �T�[�o�� host ip �A�h���X���擾������ݒ肵���肷��
                �܂��l�b�g���[�N�ڑ����ł��Ă��ȂȂ畉�̒l�� SOCL_ENETRESET
                ��Ԃ��D
 *---------------------------------------------------------------------------*/
int SOC_SetResolver(const SOCInAddr* dns1, const SOCInAddr* dns2)
{
    return SOCL_SetResolver(SOC_NtoHl(dns1->addr), SOC_NtoHl(dns2->addr));
}

int SOC_GetResolver(SOCInAddr* dns1, SOCInAddr* dns2)
{
    u32 dns1a, dns2a;
    int result;

    result = SOCL_GetResolver(&dns1a, &dns2a);
    if (result >= 0)
    {
        dns1->addr = SOC_HtoNl(dns1a);
        dns2->addr = SOC_HtoNl(dns2a);
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Listen

  Description:  �\�P�b�g����̐ڑ��҂����s�Ȃ�
                CPS �̎d�l��茻�݂͉������Ȃ�

  Arguments:    s               �\�P�b�g�L�q�q

  Returns:      0
 *---------------------------------------------------------------------------*/
int SOC_Listen(int s, int backlog)
{
    return SOCL_Listen(s, backlog);
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Accept

  Description:  �\�P�b�g����̐ڑ��҂����s�Ȃ�
                ���� NOBLOCK ���[�h�ł͓��삵�Ȃ�

  Arguments:    s               �\�P�b�g�L�q�q
                sockAddr        �ڑ�����̃A�h���X

  Returns:      ��or 0: ����(Accept �\�P�b�g�̋L�q�q)
                ��    : �G���[
 *---------------------------------------------------------------------------*/
int SOC_Accept(int s, void* sockAddr)
{
    SOCSockAddrIn*  addr = (SOCSockAddrIn*)sockAddr;
    u16         remote_port;
    SOCLInAddr  remote_ip;
    int         result;

    result = SOCL_Accept(s, &remote_port, &remote_ip);

    if (result >= 0)
    {
        addr->port = SOC_HtoNs(remote_port);
        addr->addr.addr = SOC_HtoNl(remote_ip);
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Fcntl

  Description:  �\�P�b�g�̐ݒ��ύX����
                ����ɂ����Ă� SOC_O_NONBLOCK �����ݒ�ł��Ȃ�

  Arguments:    s               �\�P�b�g�L�q�q
                cmd             �R�}���h
                mode            �ݒ�/�ǎ悵�����l

  Returns:      ��or 0: ����
                ��    : �G���[
 *---------------------------------------------------------------------------*/
int SOC_Fcntl(int s, int cmd, int mode)
{
    int t;

    if (!s)
    {
        return -1;
    }

    switch (cmd)
    {
    case SOC_F_GETFL:
        t = SOCL_IsBlock(s) ? 0 : SOC_O_NONBLOCK;
        return (int)t;

    case SOC_F_SETFL:
        if (mode & SOC_O_NONBLOCK)
        {
            SOCL_SetNoBlock(s);
        }
        else
        {
            SOCL_SetBlock(s);
        }
        break;

    default:
        SDK_ASSERT(0);
        break;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Startup

  Description:  �\�P�b�g�̃p�����[�^�̐ݒ���s�Ȃ�

  Arguments:    config          �ݒ�f�[�^�̍\����

  Returns:      ��or 0: ����
                ��    : �G���[
 *---------------------------------------------------------------------------*/
static void*  (*AllocFunc_SOC) (u32 name, s32 size);
static void* AllocFunc_SOCL(u32 size)
{
    s32     msize = (s32) (size + sizeof(s32));
    s32*    m = AllocFunc_SOC(0U, msize);

    // �Ăяo���ɕK�v�ȃp�����[�^��̈�̐擪�ɉB���Ă���
    if (m)
    {
        m[0] = (s32) msize;
        m++;
    }

    return (void*)m;
}

static void (*FreeFunc_SOC) (u32 name, void *ptr, s32 size);
static void FreeFunc_SOCL(void* ptr)
{
    s32*    m = (s32*)ptr;

    // �Ăяo���ɕK�v�ȃp�����[�^��̈�̐擪������o��
    if (m)
    {
        m--;
        FreeFunc_SOC(0U, (void*)m, m[0]);
    }
}

int SOC_Startup(const SOCConfig* config)
{
    static SOCLConfig   soclConfig;

    soclConfig.use_dhcp = (config->flag == SOC_FLAG_DHCP) ? TRUE : FALSE;
    soclConfig.host_ip.my_ip = SOC_NtoHl(config->addr.addr);
    soclConfig.host_ip.net_mask = SOC_NtoHl(config->netmask.addr);
    soclConfig.host_ip.gateway_ip = SOC_NtoHl(config->router.addr);
    soclConfig.host_ip.dns_ip[0] = SOC_NtoHl(config->dns1.addr);
    soclConfig.host_ip.dns_ip[1] = SOC_NtoHl(config->dns2.addr);
    soclConfig.alloc = AllocFunc_SOCL;
    soclConfig.free = FreeFunc_SOCL;
    AllocFunc_SOC = config->alloc;
    FreeFunc_SOC = config->free;
    soclConfig.cmd_packet_max = SOCL_CMDPACKET_MAX;
    soclConfig.mtu = config->mtu;
    soclConfig.rwin = config->rwin;

    return SOCL_Startup(&soclConfig);
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Cleanup

  Description:  �\�P�b�g�̃p�����[�^�̊J�����s�Ȃ�

  Arguments:

  Returns:      ��or 0: ����
                ��    : �G���[
 *---------------------------------------------------------------------------*/
int SOC_Cleanup(void)
{
    return SOCL_Cleanup();
}

/*---------------------------------------------------------------------------*
  Name:         SOC_InetNtoA

  Description:  IP �A�h���X�𕶎��\�L�֕ϊ�����

  Arguments:    in  �A�h���X

  Returns:      �����\�L
 *---------------------------------------------------------------------------*/
char* SOC_InetNtoA(SOCInAddr in)
{
    static char buffer[SOC_INET_ADDRSTRLEN];

    (void)SOC_InetNtoP(SOC_AF_INET, &in.addr, buffer, sizeof(buffer));

    return buffer;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_InetAtoN

  Description:  �����\�L���� IP �A�h���X�ɂ֕ϊ�����

  Arguments:    cp  �����\�L
                inp IP �A�h���X

  Returns:      TRUE ����   FALSE ���s
 *---------------------------------------------------------------------------*/
int SOC_InetAtoN(const char* cp, SOCInAddr* inp)
{
    u32 h = (u32) SOCL_InetAtoH(cp);

    if (h == 0)
    {
        return FALSE;
    }

    inp->addr = SOC_HtoNl(h);
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_InetNtoP

  Description:  IP �A�h���X�𕶎��\�L�֕ϊ�����

  Arguments:    in  �A�h���X

  Returns:      �����\�L
 *---------------------------------------------------------------------------*/
const char* SOC_InetNtoP(int af, const void* src, char* dst, unsigned len)
{
    u32 addr32;
    u8  addr[4];

    if (af != SOC_AF_INET)
    {
        return NULL;
    }

    if (len < SOC_INET_ADDRSTRLEN)
    {
        return NULL;
    }

    MI_CpuCopy8(src, &addr32, sizeof(addr32));

    SOC_U32to4U8(addr32, addr);
    (void)OS_SNPrintf(dst, SOC_INET_ADDRSTRLEN, "%d.%d.%d.%d", addr[3], addr[2], addr[1], addr[0]);

    return (const char*)dst;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_InetPtoN

  Description:  �����\�L���� IP �A�h���X�ɂ֕ϊ�����

  Arguments:    af  �A�h���X�t�@�~���[
                src �n���ꂽ������ւ̃|�C���^
                dst ���l�`���̃A�h���X��Ԃ��o�b�t�@�ւ̃|�C���^

  Returns:      0:���� 1:���s SOC_EAFNOSUPPORT:af��SOC_AF_INET�łȂ�
 *---------------------------------------------------------------------------*/
int SOC_InetPtoN(int af, const char* src, void* dst)
{
    u32 h;
    u32 addr;

    if (af != SOC_AF_INET)
    {
        return SOC_EAFNOSUPPORT;
    }

    h = (u32) SOCL_InetAtoH(src);

    if (h == 0)
    {
        return 1;
    }

    addr = SOC_HtoNl(h);
    MI_CpuCopy8(&addr, dst, sizeof(addr));

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_U32to4U8

  Description:  U32 �̒l�� u8 x 4 �̔z��ɑ������

  Arguments:    adr_u32         u32 �̒l
                adr_4u8         �o�͐�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOC_U32to4U8(u32 adr_u32, u8* adr_4u8)
{
    adr_4u8[0] = (u8) ((adr_u32 >> 24) & 0xff);
    adr_4u8[1] = (u8) ((adr_u32 >> 16) & 0xff);
    adr_4u8[2] = (u8) ((adr_u32 >> 8) & 0xff);
    adr_4u8[3] = (u8) ((adr_u32 >> 0) & 0xff);
}

/*---------------------------------------------------------------------------*
  Name:         SOC_Poll

  Description:  �w�肵���\�P�b�g�̂Ȃ��œǂݍ��݂⏑�����ݏ������\�ɂȂ���
                ����L�q�q���Ȃ����ǂ������ׂ܂��D

  Arguments:    fds             SOCPollFD �̔z��
                nfds            SOCPollFD �̐�
                timeout         �^�C���A�E�g
                                (0 �Ȃ炾�߂Ȃ瑦���I��/
                                 ���̒l�Ȃ�^�C���A�E�g�Ȃ�)

  Returns:      ���̒l: �����Ɉ�v�����\�P�b�g�̐�
                     0: �^�C���A�E�g
                ���̒l: �G���[
 *---------------------------------------------------------------------------*/
#define SOC_POLLING_INTERVAL    1   // �P�� ms
int SOC_Poll(SOCPollFD* fds, unsigned int nfds, OSTick timeout)
{
    SOCPollFD*  fp;
    BOOL        is_enable_timeout;
    s64         time;
    int         i, num_good;
    int         status, status_mask;

    is_enable_timeout = (timeout != (OSTick) SOC_INFTIM);
    time = (s64) timeout;

    for (;;)
    {
        fp = fds;
        num_good = 0;

        for (i = 0; i < nfds; i++)
        {
            status_mask = fp->events | SOC_POLLERR | SOC_POLLHUP | SOC_POLLNVAL;
            status = SOCL_GetStatus(fp->fd) & status_mask;
            if (status)
            {
                num_good++;
            }

            fp->revents = (s16) status;
            fp++;
        }

        if (num_good > 0 || (is_enable_timeout && time <= 0))
        {
            break;
        }

        OS_Sleep(SOC_POLLING_INTERVAL);
        time -= (s64) OS_MilliSecondsToTicks(SOC_POLLING_INTERVAL);
    }

    return num_good;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_GetLinkState

  Description:  �����N���m�����Ă����� TRUE ��������

  Arguments:    interface   �g�p���܂���
 *status     �����N���
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOC_GetLinkState(SOCInterface* interface, BOOL* status)
{
    if (WCM_GetPhase() == WCM_PHASE_DCF)            // �����N���m��������n�j
    {
        *status = TRUE;
    }
    else
    {
        *status = FALSE;

        if (OS_GetProcMode() != OS_PROCMODE_IRQ)    // ���荞�ݒ��łȂ��Ȃ�
        {
            OS_Sleep(1);    // 1ms �����𖾂��n��
        }
    }
    (void)interface;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_GetConfigError

  Description:  Config �������ɃG���[�������������ǂ����𒲍�����

  Arguments:    interface       �g�p���܂���

  Returns:      �G���[�v��: 0 �Ȃ�G���[�Ȃ�
 *---------------------------------------------------------------------------*/
s32 SOC_GetConfigError(SOCInterface* interface)
{
    s32 result = 0;

    if (WCM_GetPhase() == WCM_PHASE_DCF)
    {
        if ((SOCLiDhcpState & SOCL_DHCP_ERROR) == SOCL_DHCP_ERROR)
        {
            // ���݂� WCM �̎d�l�ł̓����N�؂�����o�ł���̂�
            // ���ۂ̃����N���؂�Ă��牽�b���o���Ă���Ȃ̂�
            // DHCP �擾�̃^�C���A�E�g�̕�����ɐ������Ă��܂��ꍇ������D
            // ����� DHCP �Ɏ��s�����Ƃ��͕K�������N�؂ꂪ���������Ƃ݂Ȃ���
            // �Đڑ����ɂ̓����N�m������͂��߂Ă��炤�������S�D
            result = SOC_IP_ERR_DHCP_TIMEOUT;
        }
    }
    else
    {
        result = SOC_IP_ERR_LINK_DOWN;
    }

#ifdef SDK_MY_DEBUG
    switch (CPSNoIpReason)
    {
    case CPS_NOIP_REASON_DHCPDISCOVERY:
        OS_TPrintf("CPS_NOIP_REASON_DHCPDISCOVERY\n");
        break;

    case CPS_NOIP_REASON_LINKOFF:
        OS_TPrintf("CPS_NOIP_REASON_LINKOFF\n");
        break;

    case CPS_NOIP_REASON_LEASETIMEOUT:
        OS_TPrintf("CPS_NOIP_REASON_LEASETIMEOUT\n");
        break;

    case CPS_NOIP_REASON_COLLISION:
        OS_TPrintf("CPS_NOIP_REASON_COLLISION\n");
        break;

    default:
        OS_TPrintf("DEFAULT???\n");
    }
#endif

    (void)interface;
    return result;
}

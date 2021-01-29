/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - include - soc
  File:     soc.h

  Copyright 2002,2003,2005,2006 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law. They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: soc.h,v $
  Revision 1.24  2006/03/10 09:20:22  kitase_hirotake
  INDENT SOURCE

  Revision 1.23  2005/12/21 13:55:24  seiki_masashi
  SSL �֘A�̒�`�� SOCL ����R�s�[

  Revision 1.22  2005/12/20 04:05:07  adachi_hiroaki
  SSL�֘A�̏�����SOC����Ăׂ�悤�ɂ���

  Revision 1.21  2005/09/17 09:05:27  yasu
  �����N�؂�̃n���h�����O�ɂƂ��Ȃ� DHCP����̏C��

  Revision 1.20  2005/09/08 05:35:45  yasu
  �\�P�b�g�f�X�N���v�^���L���l�ł��邩�ǂ����̔���� SOCL ���ōs�Ȃ����Ƃɂ���

  Revision 1.19  2005/08/19 12:56:07  yada
  add SOC_MAXDNAME

  Revision 1.18  2005/08/19 05:06:37  yasu
  mtu/rwin �Ή�

  Revision 1.17  2005/08/17 04:10:39  yasu
  SOC_Poll �̋@�\�ǉ�

  Revision 1.16  2005/08/13 09:38:38  yasu
  SOC_GetSockOpt �ŕԒl�� 0 �𖄂߂�悤�ɏC��

  Revision 1.15  2005/08/10 01:07:36  yasu
  SOC_GetMtu�@�̒ǉ�

  Revision 1.14  2005/08/02 06:18:19  yasu
  SOC_GetHostID ���� SOCL_GetHostID ���Ăяo���悤�ɏC��

  Revision 1.13  2005/08/02 05:42:58  yasu
  SOC_GetAddr �ł� sleep ������ǉ�

  Revision 1.12  2005/07/30 15:35:18  yasu
  �y�����@�C��

  Revision 1.11  2005/07/28 14:14:40  seiki_masashi
  SOC_InetNtoP, SOC_InetPtoN �̒ǉ�

  Revision 1.10  2005/07/28 13:52:25  seiki_masashi
  IP_ERR_* �萔�̒ǉ�

  Revision 1.9  2005/07/28 13:32:44  seiki_masashi
  IP_GetConfigError, IP_ClearConfigError ��ǉ�

  Revision 1.8  2005/07/28 11:15:17  yasu
  SOC_Poll �̎���

  Revision 1.7  2005/07/27 14:05:03  yasu
  gctest ���Ŏg�p���Ă���ϐ��̒ǉ�

  Revision 1.6  2005/07/27 13:18:22  yasu
  SOC_GetHostByAddr�@�̒ǉ�

  Revision 1.5  2005/07/27 12:22:38  yasu
  IP_AtoN �n�̒�`���C��

  Revision 1.4  2005/07/27 11:59:48  yasu
  SOC �֐��ǉ�

  Revision 1.3  2005/07/26 00:08:01  yasu
  SOC_Startup �� SOCL_Startup �̘A��

  Revision 1.2  2005/07/25 10:15:01  yasu
  SOC ���C�u�����̒ǉ�

  Revision 1.1  2005/07/15 13:32:51  yasu
  soc ���C�u�����̃w�b�_�𒲐�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITROWIFI_SOC_H_
#define NITROWIFI_SOC_H_

#include <nitro/types.h>
#include <nitro/os.h>
#include <nitroWiFi/wcm.h>
#include <nitroWiFi/socl.h>
#include <nitroWiFi/soc_errcode.h>

#ifdef __cplusplus

extern "C" {
#endif

#define SOC_VENDOR_NINTENDO         0x0000              // Nintendo TCP/IP stack
#define SOC_VERSION                 0x0100              // Version 1.0
#define SOC_PF_INET                 2                   // ARPA Internet protocols
#define SOC_AF_INET                 SOC_PF_INET
#define SOC_SOCK_STREAM             1                   // stream socket
#define SOC_SOCK_DGRAM              2                   // datagram socket
#define SOC_FLAG_DHCP               0x0001              // Use DHCP
#define SOC_SOL_SOCKET              0xffff              // options for socket level
#define SOC_REUSEADDR               0x00000004          // BOOL
#define SOC_MSG_PEEK                SOCL_MSG_PEEK       //0x02 take data but leave it
#define SOC_MSG_DONTWAIT            SOCL_MSG_DONTWAIT   //0x04 non-block operation
#define SOC_INET_ADDRSTRLEN         16

#define SOC_INADDR_ANY              ((u32) 0x00000000)  // 0.0.0.0
#define SOC_INADDR_BROADCAST        ((u32) 0xffffffff)  // 255.255.255.255
#define SOC_INADDR_LOOPBACK         ((u32) 0x7f000001)  // 127.0.0.1
#define SOC_INADDR_UNSPEC_GROUP     ((u32) 0xe0000000)  // 224.0.0.0
#define SOC_INADDR_ALLHOSTS_GROUP   ((u32) 0xe0000001)  // 224.0.0.1
#define SOC_INADDR_MAX_LOCAL_GROUP  ((u32) 0xe00000ff)  // 224.0.0.255

// SO_Shutdown()
#define SOC_SHUT_RD     0                       // Not supported. Always RDWR
#define SOC_SHUT_WR     1                       // Not supported. Always RDWR
#define SOC_SHUT_RDWR   2

// SO_Fcntl()
#define SOC_F_GETFL     3
#define SOC_F_SETFL     4
#define SOC_O_NONBLOCK  0x04

// SO_GetSockOpt() / SO_SetSockOpt()
#define SOC_SOL_SOCKET      0xffff              // options for socket level
#define SOC_SO_SNDBUF       0x00001001          // int
#define SOC_SO_RCVBUF       0x00001002          // int
#define SOC_SO_REUSEADDR    0x00000004          // BOOL

// DHCP_GetStatus()
#define SOC_DHCP_STATE_INIT         0
#define SOC_DHCP_STATE_SELECTING    1
#define SOC_DHCP_STATE_REQUESTING   2
#define SOC_DHCP_STATE_BOUND        3
#define SOC_DHCP_STATE_RENEWING     4
#define SOC_DHCP_STATE_REBINDING    5
#define SOC_DHCP_STATE_INIT_REBOOT  6
#define SOC_DHCP_STATE_REBOOTING    7

//
#define SOC_IP_ERR_NONE                 0       // Ok
#define SOC_IP_ERR_BUSY                 (-1)    // Busy
#define SOC_IP_ERR_UNREACHABLE          (-2)    // Networks is unreachable
#define SOC_IP_ERR_RESET                (-3)    // Connection reset
#define SOC_IP_ERR_NOT_EXIST            (-4)    // Connection does not exist
#define SOC_IP_ERR_EXIST                (-5)    // Connection already exists
#define SOC_IP_ERR_SOCKET_UNSPECIFIED   (-6)    // Foreign socket unspecified
#define SOC_IP_ERR_INS_RESOURCES        (-7)    // Insufficient resources
#define SOC_IP_ERR_CLOSING              (-8)    // Connection closing
#define SOC_IP_ERR_AGAIN                (-9)    // Would block
#define SOC_IP_ERR_TIMEOUT              (-10)   // Timeout
#define SOC_IP_ERR_REFUSED              (-11)   // Connection refused
#define SOC_IP_ERR_INVALID              (-12)
#define SOC_IP_ERR_INV_SOCKET           (-13)
#define SOC_IP_ERR_INV_OPTION           (-14)

//
#define SOC_IP_ERR_CANCELED         (-16)
#define SOC_IP_ERR_DATASIZE         (-17)
#define SOC_IP_ERR_SOURCE_QUENCH    (-18)       // IPv4 only
#define SOC_IP_ERR_NETDOWN          (-19)       // local interface down

// IPGetConfigError()
#define SOC_IP_ERR_DHCP_TIMEOUT         (-100)  // Could not find any DHCP server.
#define SOC_IP_ERR_DHCP_EXPIRED         (-101)
#define SOC_IP_ERR_DHCP_NAK             (-102)
#define SOC_IP_ERR_PPPoE_TIMEOUT        (-103)  // Could not find any access concentrator.
#define SOC_IP_ERR_PPPoE_SERVICE_NAME   (-104)  // The requested Service-Name request could not be honored.
#define SOC_IP_ERR_PPPoE_AC_SYSTEM      (-105)  // The access concentrator experienced some error.
#define SOC_IP_ERR_PPPoE_GENERIC        (-106)  // An unrecoverable error occured.
#define SOC_IP_ERR_LCP                  (-107)  // LCP negotiation error
#define SOC_IP_ERR_AUTH                 (-108)  // Authentication error
#define SOC_IP_ERR_IPCP                 (-109)  // IPCP negotiation error
#define SOC_IP_ERR_PPP_TERMINATED       (-110)  // The access concentrator terminated the PPPoE session.
#define SOC_IP_ERR_ADDR_COLLISION       (-111)  // Duplicate IP address
#define SOC_IP_ERR_LINK_DOWN            (-112)  // Link down

// DNS Error
#define SOC_IP_ERR_DNS_MAX      (-200)
#define SOC_IP_ERR_DNS_MIN      (SOC_IP_ERR_DNS_MAX - 15)

#define SOC_DNS_ERROR_MASK      0x000f
#define SOC_DNS_ERROR_NONE      0x0000          // No error
#define SOC_DNS_ERROR_FORMAT    0x0001          // Format error
#define SOC_DNS_ERROR_SERVER    0x0002          // Server failure
#define SOC_DNS_ERROR_NAME      0x0003          // Name Error
#define SOC_DNS_ERROR_NOTIMP    0x0004          // Not Implemented
#define SOC_DNS_ERROR_REFUSED   0x0005          // Refused
#define SOC_IP_ERR_DNS_FORMAT   (SOC_IP_ERR_DNS_MAX - SOC_DNS_ERROR_FORMAT)     // -201
#define SOC_IP_ERR_DNS_SERVER   (SOC_IP_ERR_DNS_MAX - SOC_DNS_ERROR_SERVER)     // -202
#define SOC_IP_ERR_DNS_NAME     (SOC_IP_ERR_DNS_MAX - SOC_DNS_ERROR_NAME)       // -203
#define SOC_IP_ERR_DNS_NOTIMP   (SOC_IP_ERR_DNS_MAX - SOC_DNS_ERROR_NOTIMP)     // -204
#define SOC_IP_ERR_DNS_REFUSED  (SOC_IP_ERR_DNS_MAX - SOC_DNS_ERROR_REFUSED)    // -205
#define SOC_DNS_NAME_MAX        255
#define SOC_MAXDNAME            (SOC_DNS_NAME_MAX + 1)

#define SOC_ALEN                4
#define SOC_MAC_ALEN            6           // MAC address length
typedef s64     SOCTime;

extern const u8 SOCAddrAny[SOC_ALEN];       // 0.  0.  0.  0
extern const u8 IPAddrLoopback[SOC_ALEN];   // 127.0.  0.  1
extern const u8 IPAddrLimited[SOC_ALEN];    // 255.255.255.255
typedef struct SOCSockAddr
{
    u8  len;
    u8  family;
    u8  data[6];
} SOCSockAddr;

typedef struct SOCInAddr
{
    u32 addr;
} SOCInAddr;

typedef struct SOCSockAddrIn
{
    u8          len;                        // size of socket address structure
    u8          family;                     // the address family
    u16         port;                       // the port number
    SOCInAddr   addr;                       // the Internet address
} SOCSockAddrIn;

typedef struct SOCPollFD
{
    int     fd;
    short   events;                 // input event flags
    short   revents;                // output event flags
} SOCPollFD;

typedef struct SOCHostEnt
{
    char*   name;                   // official name of host
    char **  aliases;               // alias list (zero-terminated)
    s16     addrType;               // always SO_PF_INET
    s16     length;                 // length of address
    u8 **    addrList;              // list of addresses
} SOCHostEnt;

typedef struct SOCIPSocket
{
    u8  len;                        // size of socket address structure
    u8  family;                     // the address family
    u16 port;                       // the port number
    u8  addr[SOC_ALEN];             // the Internet address
} SOCIPSocket;

typedef struct SOCConfig
{
    u16 vendor;                     // SOC_VENDOR_NINTENDO
    u16 version;                    // SOC_VERSION

    //
    // vendor specific section
    //
    void*  (*alloc) (u32 name, s32 size);
    void (*free) (u32 name, void *ptr, s32 size);

    u32         flag;               // ORed SO_FLAG_*
    SOCInAddr   addr;
    SOCInAddr   netmask;
    SOCInAddr   router;
    SOCInAddr   dns1;
    SOCInAddr   dns2;

    s32         timeWaitBuffer;     // time wait buffer size
    s32         reassemblyBuffer;   // reassembly buffer size
    s32         mtu;                // maximum transmission unit size

    // TCP
    s32         rwin;               // default TCP receive window size (default 2 x MSS)
    SOCTime     r2;                 // default TCP total retransmit timeout value (default 100 sec)

    // PPP
    const char*     peerid;
    const char*     passwd;

    // PPPoE
    const char*     serviceName;    // UTF-8 string

    // DHCP
    const char*     hostName;       // DHCP host name
    s32 rdhcp;                      // DHCP retransmit times (default 4)

    // UDP
    s32 udpSendBuff;                // default UDP send buffer size (default 1472)
    s32 udpRecvBuff;                // defualt UDP receive buffer size (default 4416)
} SOCConfig;

typedef struct SOCAddrInfo  SOCAddrInfo;
struct SOCAddrInfo
{
    int         flags;              // input flags
    int         family;             // address family of socket
    int         sockType;           // socket type
    int         protocol;           // protocol of socket
    unsigned    addrLen;            // length of socket address
    char*       canonName;          // canonical name of service location
    void*       addr;               // socket address of socket
    SOCAddrInfo*    next;           // pointer to next in list
};

typedef SOCLSslConnection   SOCSslConnection;
typedef SOCLCaInfo          SOCCaInfo;
typedef int (*SOCSslAuthHandler) (int result, SOCSslConnection*con, int level);

//      Authentication error codes
#define SOC_CERT_OUTOFDATE                  SOCL_CERT_OUTOFDATE
#define SOC_CERT_BADSERVER                  SOCL_CERT_BADSERVER

#define SOC_CERT_ERRMASK                    SOCL_CERT_ERRMASK

#define SOC_CERT_NOROOTCA                   SOCL_CERT_NOROOTCA
#define SOC_CERT_BADSIGNATURE               SOCL_CERT_BADSIGNATURE
#define SOC_CERT_UNKNOWN_SIGALGORITHM       SOCL_CERT_UNKNOWN_SIGALGORITHM
#define SOC_CERT_UNKNOWN_PUBKEYALGORITHM    SOCL_CERT_UNKNOWN_PUBKEYALGORITHM

u32 (SOC_NtoHl) (u32 netlong);
u16 (SOC_NtoHs) (u16 netshort);
u32 (SOC_HtoNl) (u32 hostlong);
u16 (SOC_HtoNs) (u16 hostshort);

#ifdef SDK_BIG_ENDIAN
#define SOC_NtoHl(netlong)      ((u32) (netlong))
#define SOC_NtoHs(netshort)     ((u16) (netshort))
#define SOC_HtoNl(hostlong)     ((u32) (hostlong))
#define SOC_HtoNs(hostshort)    ((u16) (hostshort))

#else //SDK_LITTLE_ENDIAN
#define SOC_NtoHl(netlong) \
        ( \
            (u32) \
                ( \
                    (((u32) (netlong) >> 24) & 0x000000ff) | (((u32) (netlong) >> 8) & 0x0000ff00) | \
                        (((u32) (netlong) << 8) & 0x00ff0000) | (((u32) (netlong) << 24) & 0xff000000) \
                ) \
        )

#define SOC_NtoHs(netshort) ((u16) ((((u16) (netshort) >> 8) & 0x00ff) | (((u16) (netshort) << 8) & 0xff00)))

#define SOC_HtoNl(hostlong) \
        ( \
            (u32) \
                ( \
                    (((u32) (hostlong) >> 24) & 0x000000ff) | (((u32) (hostlong) >> 8) & 0x0000ff00) | \
                        (((u32) (hostlong) << 8) & 0x00ff0000) | (((u32) (hostlong) << 24) & 0xff000000) \
                ) \
        )

#define SOC_HtoNs(hostshort)    ((u16) ((((u16) (hostshort) >> 8) & 0x00ff) | (((u16) (hostshort) << 8) & 0xff00)))
#endif

int SOC_Socket(int pf, int type, int protocol);
int SOC_Bind(int s, const void* sockAddr);
int SOC_Connect(int s, const void* sockAddr);
int SOC_Read(int s, void* buf, int len);
int SOC_Recv(int s, void* buf, int len, int flags);
int SOC_RecvFrom(int s, void* buf, int len, int flags, void* sockFrom);
int SOC_Write(int s, const void* buf, int len);
int SOC_Send(int s, const void* buf, int len, int flags);
int SOC_SendTo(int s, const void* buf, int len, int flags, const void* sockTo);
int SOC_Close(int s);
int SOC_Shutdown(int s, int how);

SOCHostEnt*     SOC_GetHostByAddr(const void* addr, int len, int type);
SOCHostEnt*     SOC_GetHostByName(const char* name);
int     SOC_GetSockName(int s, void* sockAddr);
long    SOC_GetHostID(void);

int     SOC_Startup(const SOCConfig* config);
int     SOC_Cleanup(void);

int     SOC_SetResolver(const SOCInAddr* dns1, const SOCInAddr* dns2);
int     SOC_GetResolver(SOCInAddr* dns1, SOCInAddr* dns2);

int     SOC_Accept(int s, void* sockAddr);
int     SOC_Listen(int s, int backlog);

int     SOC_Fcntl(int s, int cmd, int mode);

int     SOC_InetAtoN(const char* cp, SOCInAddr* inp);
char*   SOC_InetNtoA(SOCInAddr in);
int     SOC_InetPtoN(int af, const char* src, void* dst);
const char*     SOC_InetNtoP(int af, const void* src, char* dst, unsigned len);

int SOC_Poll(SOCPollFD fds[], unsigned nfds, OSTick timeout);

/*---------------------------------------------------------------------------*
  Name:         SOC_EnableSsl

  Description:  �\�P�b�g�� SSL ���[�h�ɂ���B
                bind �O�ɌĂяo���˂΂Ȃ�Ȃ��B
  
  Arguments:    s    �\�P�b�g
                con  SOCSslConnection �\���̂ւ̃|�C���^
                     NULL �� SSL ���[�h����ʏ�ɖ߂�
  
  Returns:      �� or 0: ����
                ��     : �G���[
                  ����T�|�[�g���Ă���G���[�l�͈ȉ�
                    - :  
                  �G���[�l�͒ǉ������ꍇ�����邽�߁A���̕��̒l���S�Ĕėp
                  �G���[�Ƃ��Ĉ������ƁD
 *---------------------------------------------------------------------------*/
static inline int SOC_EnableSsl(int s, SOCSslConnection* con)
{
    return SOCL_EnableSsl(s, (SOCLSslConnection*)con);
}

/*---------------------------------------------------------------------------*
  Name:         SOC_AddRandomSeed

  Description:  SSL �Ŏg�p���闐���p�̎�𝘝a����B
                OS_GetLowEntropyData �֐��̕Ԃ�l�����I�ɗ^����ׂ��ł���B
                SOCL �̏������O�ł��Ăяo���\�B
  
  Arguments:    seed �������a�̂��߂̎�ւ̃|�C���^
                len  seed �Ŏw�肵����̒���
  
  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
static inline void SOC_AddRandomSeed(void* seed, u32 len)
{
    SOCL_AddRandomSeed(seed, len);
}

//int   SOC_GetPeerName(int s, void* sockAddr);
//int   SOC_SockAtMark (int s);
//
// NO SUPPORT FUNCTIONS

//
static inline int SOC_GetSockOpt(int s, int level, int optname, void* optval, int* optlen)
{
    (void)s;
    (void)level;
    (void)optname;
    MI_CpuClear8(optval, (u32) * optlen);

    return 0;
}

static inline int SOC_SetSockOpt(int s, int level, int optname, const void* optval, int optlen)
{
    (void)s;
    (void)level;
    (void)optname;
    (void)optval;
    (void)optlen;

    return 0;
}

static inline void SOC_Init(void)
{
}

static inline int SOC_Reset(int final)
{
    (void)final;
    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         SOC_GetMacAddr/SOC_GetAddr/SOC_GetGateway/SOC_GetNetmask

  Description:  HOST �̐ݒ���擾����
 *---------------------------------------------------------------------------*/
void        SOC_U32to4U8(u32 adr_u32, u8* adr_4u8);

typedef struct SOCInterface
{
    u32 _dummy_;
} SOCInterface;

static inline void SOC_GetMacAddr(SOCInterface* interface, u8* macAddress)
{
    (void)interface;
    OS_GetMacAddress(macAddress);
}

static inline void SOC_GetAddr(SOCInterface* interface, u8* addr)
{
    u32 hostip = (u32) SOCL_GetHostID();    // with BLOCK
    SOC_U32to4U8(hostip, addr);

    (void)interface;
}

static inline void SOC_GetAlias(SOCInterface* interface, u8* addr)
{
    SOC_GetAddr(interface, addr);
}

static inline void SOC_GetGateway(SOCInterface* interface, u8* addr)
{
    SOC_U32to4U8((u32) CPSGatewayIp, addr);
    (void)interface;
}

static inline void SOC_GetNetmask(SOCInterface* interface, u8* addr)
{
    SOC_U32to4U8((u32) CPSNetMask, addr);
    (void)interface;
}

static inline void SOC_GetBroadcastAddr(SOCInterface* interface, u8* addr)
{
    SOC_U32to4U8((u32) SOC_INADDR_BROADCAST, addr);
    (void)interface;
}

static inline void SOC_GetMtu(SOCInterface* interface, s32* mtu)
{
    (*mtu) = SOCLiConfigPtr ? (s32) SOCLiConfigPtr->mtu : 0;
    (void)interface;
}

static inline s32 SOC_ClearConfigError(SOCInterface* interface)
{
    (void)interface;
    SOCLiDhcpState = 0;
    return 0;
}

void    SOC_GetLinkState(SOCInterface* interface, BOOL* status);
s32     SOC_GetConfigError(SOCInterface* interface);

/*---------------------------------------------------------------------------*
  Name:         SOC_DHCPGetStatus

  Description:  DHCP �̏�Ԃ��擾����
                �����ł͎��z�X�g�� IP �A�h���X���ݒ肳��Ă��邩�ǂ���������
                �l���ݒ肳��Ă����� SOC_DHCP_STATE_BOUND �������łȂ��Ȃ�
                SOC_DHCP_STATE_REQUESTING ��Ԃ�
 *---------------------------------------------------------------------------*/
typedef struct
{
    u32 dummy;
} SOCDHCPInfo;

static inline int SOC_DHCPGetStatus(SOCDHCPInfo* info)
{
    (void)info;
    return SOC_GetHostID() ? SOC_DHCP_STATE_BOUND : SOC_DHCP_STATE_REQUESTING;
}

#ifdef __cplusplus

}
#endif

#endif // NITROWIFI_SOC_H_

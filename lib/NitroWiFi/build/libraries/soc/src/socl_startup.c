/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     soc_startup.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_startup.c,v $
  Revision 1.18  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.17  2005/10/05 11:15:45  yasu
  �o�[�W�����X�g�����O�̖��ߍ���

  Revision 1.16  2005/09/28 11:44:37  yasu
  �R�[�h�𐮗�
  SOCL_CalmDown() ��ǉ�
  CPS ���C�u�����̐����Ή����܂��Ȃ̂� SOC ���ŏ������J�o�[���Ă���

  Revision 1.15  2005/09/21 11:19:40  yasu
  SOCL_SetYieldWait() �̒ǉ�

  Revision 1.14  2005/09/15 13:14:46  yasu
  DHCP �^�C���A�E�g�T�|�[�g

  Revision 1.13  2005/09/15 12:51:01  yasu
  DHCP Requested IP �T�|�[�g

  Revision 1.12  2005/09/02 08:47:05  yasu
  SOCLConfig ���� IP �A�h���X�̐ݒ��ύX�ł��� API ��p��

  Revision 1.11  2005/08/22 04:33:02  yasu
  UDP �̎�M�o�b�t�@�T�C�Y���Œ�ɂ���

  Revision 1.10  2005/08/19 05:05:36  yasu
  mtu/rwin �Ή�

  Revision 1.9  2005/08/18 13:17:59  yasu
  cleanup API ��ʃt�@�C���ֈړ�

  Revision 1.8  2005/08/08 14:15:38  yasu
  CPS �X���b�h�̗D��x�ݒ�

  Revision 1.7  2005/07/30 22:30:14  yasu
  �f���������悤�ɏC��

  Revision 1.6  2005/07/30 15:33:45  yasu
  �R�}���h�p�C�v�����ɂƂ��Ȃ��C��

  Revision 1.5  2005/07/25 14:22:24  yasu
  SOC_Startup �� SOCL_Startup �̘A��

  Revision 1.4  2005/07/23 14:16:49  yasu
  Debug ���b�Z�[�W���B��

  Revision 1.3  2005/07/22 12:44:56  yasu
  �񓯊�����������

  Revision 1.2  2005/07/19 14:06:33  yasu
  SOCL_Read/Write �ǉ�

  Revision 1.1  2005/07/18 13:16:28  yasu
  ���M��M�X���b�h���쐬����

  Revision 1.1  2005/07/15 13:33:11  yasu
  soc ���C�u�����̃\�[�X����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>
#include "../wcm/include/wcm_cpsif.h"

//---------------------------------------------------------------------------*
//
//  SOCL �� Config �l�ւ̃|�C���^�̕ۑ�
//  SOCL �֐��̌Ăяo���O�ɂ����ɒl���ݒ肷��K�v������
//
CPSConfig       SOCLiCPSConfig;
const SOCLConfig*   SOCLiConfigPtr = NULL;
int         SOCLiUDPSendSocket = 0;
int         SOCLiDhcpState = 0;
SOCLInAddr  SOCLiRequestedIP = 0;
u32         SOCLiYieldWait = 0;

//---------------------------------------------------------------------------*
static void SOCLi_StartupCPS(void);
static int  SOCLi_StartupSOCL(void);
static void SOCLi_SetMyIP(void);
BOOL        SOCL_LinkIsOn(void);

#include <nitroWiFi/version.h>
#include <nitro/version_begin.h>
#define MODULE  "WiFi"SDK_WIFI_VERSION_STRING
static char id_string[] = SDK_MIDDLEWARE_STRING("NINTENDO", MODULE);
#include <nitro/version_end.h>

/*---------------------------------------------------------------------------*
  Name:         SOCL_Startup

  Description:  ���C�u���������������܂��B
                �֐����Ăяo���O�� SOCLConfig �p�����[�^�����C�u�����d�l��
                ���킹�ď��������Ă�������.

  Arguments:    socl_config     �\����SOCLConfig�ւ̃|�C���^
                socl_hostip     IP �A�h���X�̐ݒ�l
                                NULL �Ȃ� DHCP �Ŏ擾����

  Returns:      0  �Ȃ琳��I��
                -1 �Ȃ烁�����s��
 *---------------------------------------------------------------------------*/
int SOCL_Startup(const SOCLConfig* socl_config)
{
    SDK_USING_MIDDLEWARE(id_string);
    SDK_ASSERT(socl_config);

    if (SOCLiConfigPtr)
    {
        return 0;
    }

    SOCLiConfigPtr = socl_config;
    SOCLi_StartupCPS();                     // CPS     �T�[�r�X�N��
    return SOCLi_StartupSOCL();             // SOCL    �T�[�r�X�N��
}

static int SOCLi_StartupSOCL(void)
{
    int r;

    // Command �T�[�r�X�N��
    r = SOCLi_StartupCommandPacketQueue((int)SOCLiConfigPtr->cmd_packet_max);

    // UDP Send �\�P�b�g�̍쐬
    if (r >= 0)
    {
        SOCLiUDPSendSocket = SOCL_UdpSendSocket();
    }

    return r;
}

static void SOCLi_StartupCPS(void)
{
    //
    //  CPS �֐����g�p���邽�߂̐ݒ�p�����[�^
    //
    CPSConfig*          cps_config = &SOCLiCPSConfig;
    const SOCLConfig*   socl_config = SOCLiConfigPtr;

    SDK_ASSERT(socl_config->alloc);
    SDK_ASSERT(socl_config->free);

    MI_CpuClear8(cps_config, sizeof(CPSConfig));
    cps_config->alloc = socl_config->alloc;
    cps_config->free = socl_config->free;
    cps_config->link_is_on = SOCL_LinkIsOn; // ���������N�̏�Ԃ�Ԃ��֐�
    cps_config->random_seed = 0;            // 0 �Ȃ痐���̎�� OS_Tick ����擾
    cps_config->yield_wait = SOCLiYieldWait;    // CPS ���C�u������ yield ���� sleep ����

    // socl_config->lan_buffer_size �� 0    �Ȃ� default �l��ݒ�
    // socl_config->lan_buffer      �� NULL �Ȃ� alloc �Ŏ��͊m�ۂ���
    if (socl_config->lan_buffer_size)
    {
        cps_config->lan_buflen = socl_config->lan_buffer_size;
    }
    else
    {
        cps_config->lan_buflen = SOCL_LAN_BUFFER_SIZE_DEFAULT;
    }

    if (socl_config->lan_buffer)
    {
        cps_config->lan_buf = socl_config->lan_buffer;
    }
    else
    {
        cps_config->lan_buf = SOCLi_Alloc(cps_config->lan_buflen);
    }

    // MTU/MSS �̐ݒ�
    //  socl_config->mtu �� 0 �Ȃ� default �l��ݒ�
    {
        s32 mtu, mss, rwin;

        mtu = socl_config->mtu ? socl_config->mtu : SOCL_MTU_SIZE_DEFAULT;
        rwin = socl_config->rwin ? socl_config->rwin : SOCL_RWIN_SIZE_DEFAULT;
        mss = SOCLi_MTUtoMSS(mtu);

        cps_config->mymss = (u32) mss;
        SOCLSocketParamTCP.buffer.rcvbuf_size = (u16) rwin;
        SOCLSocketParamTCP.buffer.rcvbuf_consume_min = (u16) (rwin / 2);
    }

    // HostIP �̐ݒ� hostip �� 0 �Ȃ� DHCP ���g�p����
    // DHCP �������Ȃ��ꍇ�ASOCLiDhcpState �Ŕ���ł���
    CPSMyIp = 0x00000000;

    if (socl_config->use_dhcp)
    {
        SOCLiDhcpState = SOCL_DHCP_REQUEST;
        cps_config->mode = 0;
        cps_config->dhcp_callback = SOCLi_DhcpTimeout;
        cps_config->requested_ip = SOCLiRequestedIP;
    }
    else
    {
        SOCLiDhcpState = 0;
        cps_config->mode = CPS_DONOTUSE_DHCP;
        cps_config->dhcp_callback = SOCLi_SetMyIP;
    }

    // CPS �X���b�h�̗D��x
    CPS_SetThreadPriority(socl_config->cps_thread_prio ? socl_config->cps_thread_prio : (u32) SOCL_CPS_SOCKET_THREAD_PRIORITY);

    WCM_SetRecvDCFCallback(CPSi_RecvCallbackFunc);

    {   // �񓯊� Cleanup �ɑ΂���ꎞ�I�ȑ΍�
        extern void CPS_SetScavengerCallback(void (*f) (void));
        CPS_SetScavengerCallback(SOCLi_TrashSocket);
    }

#ifdef SDK_MY_DEBUG
    OS_TPrintf("CPS_Startup\n");
#endif
    CPS_Startup(cps_config);

    return;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_SetMyIPinConfig

  Description:  SOCLConfig ���ɓo�^����Ă��� IP �A�h���X�t�B�[���h�̏�������

  Arguments:    �Ȃ�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
SDK_WEAK_SYMBOL void SOCLi_SetMyIPinConfig(SOCLInAddr ip, SOCLInAddr mask, SOCLInAddr gateway)
{
    SOCLConfig*     config = (SOCLConfig*)SOCLiConfigPtr;

    if (config)
    {
        config->host_ip.my_ip = ip;
        config->host_ip.net_mask = mask;
        config->host_ip.gateway_ip = gateway;
    }

    return;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_SetMyIP

  Description:  IP �A�h���X�̎蓮�ݒ�

  Arguments:    �Ȃ�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
SDK_WEAK_SYMBOL void SOCLi_SetMyIP(void)
{
    const SOCLConfig*   config = SOCLiConfigPtr;

    SDK_ASSERT(config);
    CPSMyIp = SOCL2CPSInAddr(config->host_ip.my_ip);
    CPSNetMask = SOCL2CPSInAddr(config->host_ip.net_mask);
    CPSGatewayIp = SOCL2CPSInAddr(config->host_ip.gateway_ip);
    CPSDnsIp[0] = SOCL2CPSInAddr(config->host_ip.dns_ip[0]);
    CPSDnsIp[1] = SOCL2CPSInAddr(config->host_ip.dns_ip[1]);
    SOCLiDhcpState |= SOCL_DHCP_CALLBACK;
    return;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_DhcpTimeout

  Description:  DHCP �� Timeout �̃R�[���o�b�N

  Arguments:    �Ȃ�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
SDK_WEAK_SYMBOL void SOCLi_DhcpTimeout(void)
{
    SOCLiDhcpState |= (SOCL_DHCP_CALLBACK | SOCL_DHCP_ERROR);
    return;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_LinkIsOn

  Description:  ���������N�̐ڑ�����

  Arguments:    �Ȃ�

  Returns:      �ڑ����Ă���Ȃ� TRUE
 *---------------------------------------------------------------------------*/
SDK_WEAK_SYMBOL BOOL SOCL_LinkIsOn(void)
{
    return WCM_GetApMacAddress() != NULL;
}

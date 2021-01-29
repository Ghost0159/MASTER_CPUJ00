/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_consts.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_const.c,v $
  Revision 1.9  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.8  2005/08/25 08:27:38  yasu
  UDP ��M�ɂ�����o�b�t�@�����O�����ǉ�

  Revision 1.7  2005/08/22 04:33:02  yasu
  UDP �̎�M�o�b�t�@�T�C�Y���Œ�ɂ���

  Revision 1.6  2005/08/19 05:05:23  yasu
  mtu/rwin �Ή�

  Revision 1.5  2005/08/13 08:01:23  yasu
  consumed_min �̒ǉ�

  Revision 1.4  2005/07/30 22:30:14  yasu
  �f���������悤�ɏC��

  Revision 1.3  2005/07/30 15:28:29  yasu
  UDP ���M��M��2�d��

  Revision 1.2  2005/07/22 12:44:56  yasu
  �񓯊�����������

  Revision 1.1  2005/07/18 13:16:28  yasu
  ���M��M�X���b�h���쐬����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>

/*---------------------------------------------------------------------------*
  Name:         SOCLSocketParamTCP/SOCLSocketParamUDP

  Description:  �\�P�b�g�ɂ�����f�t�H���g�̃��������p�p�����[�^�D
                �ア�V���{���Ƃ��Ē�`���Ă���̂ŃA�v���P�[�V��������
                ���������p�Ɋւ���ׂ��Ȑݒ�̂��߂ɃI�[�o�[���C�h�\�D
                TCP �ł� UDP �ł�����
 *---------------------------------------------------------------------------*/

// TCP
SDK_WEAK_SYMBOL SOCLSocketParam SOCLSocketParamTCP =
{
    SOCL_FLAGMODE_TCP,                      // TCP
    SOCL_FLAGBLOCK_BLOCK,                   // BLOCK
    {                                       // cps
        0,                                  //SOCL_TCP_SOCKET_CPS_RCVBUF_SIZE,		// rcvbuf_size

        //****Set in SOCL_Startup()
        0,                                  //SOCL_TCP_SOCKET_CPS_RCVBUF_CONSUME_MIN,	// rcvbuf_consume_min

        //****Set in SOCL_Startup()
        SOCL_TCP_SOCKET_CPS_SNDBUF_SIZE,    // sndbuf_size
        SOCL_TCP_SOCKET_CPS_LINBUF_SIZE,    // linbuf_size
        SOCL_TCP_SOCKET_CPS_OUTBUF_SIZE,    // outbuf_size
        SOCL_TCP_SOCKET_SEND_WRTBUF_SIZE,   // wrtbuf_size
        0,  // udpbuf_size
    },

    {       // recv_thread
        SOCL_TCP_SOCKET_RECV_THREAD_STACK_SIZE, // stack_size
        SOCL_TCP_SOCKET_RECV_THREAD_PRIORITY,   // priority
        SOCL_TCP_SOCKET_RECV_THREAD_QUEUE_MAX,  // queue_max
    },

    {   // send_thread
        SOCL_TCP_SOCKET_SEND_THREAD_STACK_SIZE, // stack_size
        SOCL_TCP_SOCKET_SEND_THREAD_PRIORITY,   // priority
        SOCL_TCP_SOCKET_SEND_THREAD_QUEUE_MAX,  // queue_max
    },
};

//----------------------------------------------------------------------------
// UDP
//
SDK_WEAK_SYMBOL SOCLSocketParam SOCLSocketParamUDP =
{
    SOCL_FLAGMODE_UDP,                      // UDP
    SOCL_FLAGBLOCK_BLOCK,                   // BLOCK
    {                                       // cps
        SOCL_UDP_SOCKET_CPS_RCVBUF_SIZE,    // rcvbuf_size
        1,  // rcvbuf_consume_min
        0,  // sndbuf_size
        SOCL_UDP_SOCKET_CPS_LINBUF_SIZE,    // linbuf_size
        0,  // outbuf_size
        0,  // wrtbuf_size
        SOCL_UDP_SOCKET_RECV_UDPBUF_SIZE,   // udpbuf_size
    },

    {   // recv_thread
        SOCL_UDP_SOCKET_RECV_THREAD_STACK_SIZE, // stack_size
        SOCL_UDP_SOCKET_RECV_THREAD_PRIORITY,   // priority
        SOCL_UDP_SOCKET_RECV_THREAD_QUEUE_MAX,  // queue_max
    },

    {                       // send_thread
        0,                  // stack_size
        0,                  // priority
        0,                  // queue_max
    },
};

//----------------------------------------------------------------------------
// UDP for Send
//
SDK_WEAK_SYMBOL SOCLSocketParam SOCLSocketParamUDPSend =
{
    SOCL_FLAGMODE_UDPSEND,  // UDPSEND
    SOCL_FLAGBLOCK_NOBLOCK, // BLOCK
    {                       // cps
        0,                  // rcvbuf_size
        0,                  // rcvbuf_consume_min
        SOCL_UDP_SOCKET_CPS_SNDBUF_SIZE,    // sndbuf_size
        0,  // linbuf_size
        SOCL_UDP_SOCKET_CPS_OUTBUF_SIZE,    // outbuf_size
        SOCL_UDP_SOCKET_SEND_WRTBUF_SIZE,   // wrtbuf_size
        0,  // udpbuf_size
    },

    {       // recv_thread
        0,  // stack_size
        0,  // priority
        0,  // queue_max
    },

    {       // send_thread
        SOCL_UDP_SOCKET_SEND_THREAD_STACK_SIZE, // stack_size
        SOCL_UDP_SOCKET_SEND_THREAD_PRIORITY,   // priority
        SOCL_UDP_SOCKET_SEND_THREAD_QUEUE_MAX,  // queue_max
    },
};

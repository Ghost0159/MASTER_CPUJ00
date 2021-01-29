/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - include
  File:     wcm_private.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_private.h,v $
  Revision 1.9  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.8  2005/10/05 13:44:40  yasu
  OS_SecondsToTicks() �̃o�O�̑΍�

  Revision 1.7  2005/10/05 09:40:38  terui
  �L�[�v�A���C�u�pNull�p�P�b�g���M�Ԋu��60�b��70�b�ɕύX

  Revision 1.6  2005/10/05 08:53:32  terui
  �d�g���x�~�ρA�y�ю擾�֐��̃v���g�^�C�v�錾��ǉ�

  Revision 1.5  2005/09/12 10:01:13  terui
  WCM_CONNECT_STATUSCODE_ILLEGAL_RATESET�萔��ǉ�

  Revision 1.4  2005/09/01 13:11:03  terui
  Keep Alive �p�P�b�g���M�Ԋu��萔��`
  �A���[���p�ϐ������[�N�\���̂ɒǉ�
  �R�l�N�g���s���� wlStatus ��ێ�����ϐ������[�N�\���̂ɒǉ�

  Revision 1.3  2005/08/09 07:58:30  terui
  WEP�L�[��ޔ���������o�b�t�@��32�o�C�g�A���C�������ʒu�ɒ���

  Revision 1.2  2005/07/11 12:05:31  terui
  WCMWork �\���̂ɃL���b�V�����C�������킹��ׂ̃_�~�[�����o��ǉ��B

  Revision 1.1  2005/07/07 10:45:37  terui
  �V�K�ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITROWIFI_WCM_PRIVATE_H_
#define NITROWIFI_WCM_PRIVATE_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include <nitroWiFi/wcm.h>
#include <nitro/os.h>

/*---------------------------------------------------------------------------*
    �萔 ��`
 *---------------------------------------------------------------------------*/

// ���Z�b�g�d���Ăяo���Ǘ��t���O�̏��
#define WCM_RESETTING_OFF   0
#define WCM_RESETTING_ON    1

// AP ���ێ����X�g���̊e�u���b�N�̎g�p���
#define WCM_APLIST_BLOCK_EMPTY  0
#define WCM_APLIST_BLOCK_OCCUPY 1

// Keep Alive �p NULL �p�P�b�g���M�Ԋu(�b)
#define WCM_KEEP_ALIVE_SPAN ((OSTick) 70)

// �f�o�b�O�\���L���ݒ�
#ifdef SDK_FINALROM
#define WCM_DEBUG   0
#else
#define WCM_DEBUG   1
#endif

// �ڑ����̎��s�R�[�h��`
#define WCM_CONNECT_STATUSCODE_ILLEGAL_RATESET  18

#if WCM_DEBUG
#define WCMi_Printf     OS_TPrintf("[WCM] %s(%d) Report: ", __FILE__, __LINE__); \
    OS_TPrintf
#define WCMi_Warning    OS_TPrintf("[WCM] %s(%d) Warning: ", __FILE__, __LINE__); \
    OS_TPrintf
#else
#define WCMi_Printf(...)    ((void)0)
#define WCMi_Warning(...)   ((void)0)
#endif

/*---------------------------------------------------------------------------*
    �\���� ��`
 *---------------------------------------------------------------------------*/

// WCM �������[�N�ϐ��Q���܂Ƃ߂��\����
typedef struct WCMWork
{
    u8          wmWork[WCM_ROUNDUP32( WM_SYSTEM_BUF_SIZE ) ];
    u8          sendBuf[WCM_DCF_SEND_BUF_SIZE];
    u8          recvBuf[WCM_DCF_RECV_BUF_SIZE * 2];
    WMBssDesc   bssDesc;
    struct
    {
        u8  key[WM_SIZE_WEPKEY];
        u8  mode;
        u8  keyId;
        u8  reserved[WCM_ROUNDUP32( WM_SIZE_WEPKEY + 2 ) - ( WM_SIZE_WEPKEY + 2 ) ];
    } wepDesc;
    u32         phase;
    u32         option;
    u16         maxScanTime;
    u8          apListLock;
    u8          resetting;
    WCMConfig   config;
    WCMNotify   notify;
    s16         notifyId;
    u16         authId;
    u32         scanCount;
    WMScanExParam   scanExParam;
    OSAlarm         keepAliveAlarm;
    u16 wlStatusOnConnectFail;
    u8  reserved[6];

} WCMWork;

// AP ���ێ����X�g���̃u���b�N�\����
typedef struct WCMApList
{
    u8      state;
    u8      reserved;
    u16     linkLevel;
    u32     index;
    void*   previous;
    void*   next;
    u32     data[WCM_ROUNDUP4( WCM_APLIST_SIZE ) / sizeof(u32) ];
} WCMApList;

// AP ���ێ����X�g�̊Ǘ��p�w�b�_�\����
typedef struct WCMApListHeader
{
    u32         count;
    WCMApList*  head;
    WCMApList*  tail;

} WCMApListHeader;

/*---------------------------------------------------------------------------*
    �֐� ��`
 *---------------------------------------------------------------------------*/

// WCM ���C�u�����̓����Ǘ��\���̂ւ̃|�C���^���擾
WCMWork*    WCMi_GetSystemWork(void);

// AP ��񔭌����ɌĂяo����� �ێ����X�g�ւ̃G���g���[�ǉ�(�X�V)�v��
void        WCMi_EntryApList(WMBssDesc* bssDesc, u16 linkLevel);

// WCM_Init �֐�����Ăяo����� CPS �C���^�[�t�F�[�X�������֐�
void        WCMi_InitCpsif(void);

// DCF �f�[�^��M���ɌĂяo����� CPS �C���^�[�t�F�[�X�ւ̃R�[���o�b�N�֐�
void        WCMi_CpsifRecvCallback(WMDcfRecvBuf* recv);

// Keep Alive �p NULL �p�P�b�g�𑗐M����֐�
void        WCMi_CpsifSendNullPacket(void);

// Keep Alive �p�A���[�������Z�b�g����֐�
void        WCMi_ResetKeepAliveAlarm(void);

// DCF �f�[�^��M���Ɏ�M�d�g���x����U�ޔ�����֐�
void        WCMi_ShelterRssi(u8 rssi);
u8          WCMi_GetRssiAverage(void);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* NITROWIFI_WCM_PRIVATE_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

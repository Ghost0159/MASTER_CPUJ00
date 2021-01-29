/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - dwc_connectinet
  File:     dwc_connectinet.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_connectinet.h,v $
  Revision 1.24  2007/03/02 06:26:59  nakata
  DWC_APINFO_AREA_KOR��ǉ��B

  Revision 1.23  2006/11/17 06:30:07  nakata
  TestInet�֘A�̒萔�̖��̕ύX

  Revision 1.22  2006/11/07 04:59:30  nakata
  DWC_TestInet�n�̒ǉ�

  Revision 1.21  2006/06/27 06:43:56  nakata
  FriendsMatch���C�u�����Ȃ��p��DCF�Ď��֐���ǉ�

  Revision 1.20  2006/05/25 23:54:11  uemura
  DWC_CheckWiFiStation�֐���ǉ�

  Revision 1.19  2006/04/17 07:33:34  nakata
  �u���E�U�p�z�b�g�X�|�b�g�ݒ�֐�DWC_EnableHotSpot��ǉ�

  Revision 1.18  2006/03/09 08:05:52  uemura
  DWC_SetConnectApType�̈�����int����DWCApInfoType�֕ύX

  Revision 1.17  2006/03/09 07:35:13  nakata
  DWCApInfo����spotinfo���������A�֘A������ύX

  Revision 1.16  2006/03/09 06:07:41  uemura
  DWC_DebugConnectInetAsync�̈���WEP��16�i�O�l�������Ă���Ɛ���������ł��Ȃ��o�O���C��
  ������WEPMODE��n���悤�ɕύX���܂���

  Revision 1.15  2006/03/09 02:50:37  uemura
  �R�����g�C��

  Revision 1.14  2006/03/09 02:27:17  uemura
  DWC_SetConnectApType,DWC_DebugConnectInetAsync�̒ǉ�
  ����2�֐���dwc_connect_inet_sample�փR���p�C���X�C�b�`�Ŏg�p�ł���悤�ɒǉ�����

  Revision 1.13  2006/02/23 08:21:47  nakata
  DWCApInfo����̒萔������ύX�B

  Revision 1.12  2006/02/23 06:58:04  nakata
  DWCApinfo.bssid�̌^��char����unsigned char�ɕύX�B

  Revision 1.11  2006/02/23 02:55:40  nakata
  DWC_GetApInfo�֐��֘A�̃}�j���A����ǉ��B

  Revision 1.10  2006/02/23 00:53:54  nakata
  AP���擾�֐�DWC_GetApInfo��dwc_connectinet.c�ɒǉ�
  dwc_sample�̕s�K�v�ȃf�o�b�O���b�Z�[�W�\����}��

  Revision 1.9  2005/10/06 07:39:38  sasakit
  SSL�̑��{���ݏ�v�Z�X���b�h�̗D�揇�ʂ�������@�\���O�����璲���ł���悤�ɂ����B

  Revision 1.8  2005/10/01 10:57:00  sasakit
  DWC_CleanupInetAsync()���ɁADWC_ProcessInet()���Ă�ł�DISCONNECTED�ɂȂ�Ȃ��悤�ɂ����B

  Revision 1.7  2005/09/17 00:31:18  sasakit
  UDP�p�P�b�g�𗎂Ƃ��������擾����֐��̒ǉ��B

  Revision 1.6  2005/09/14 01:55:31  sasakit
  AP����̐ؒf�����𐮗������B
  DWC_GetLinkLevel()�֐��̒ǉ��B

  Revision 1.5  2005/09/07 07:13:55  sasakit
  DWC_AC�֐��Q�������ɌĂяo���悤�ɃX�e�[�g�}�V������ꂽ�B

  Revision 1.4  2005/09/06 09:12:38  sasakit
  ConnectInet���̃G���[�̏�����ǉ��B

  Revision 1.3  2005/08/31 04:13:43  sasakit
  �T�[�o�̐؂�ւ��@�\�ǉ��B

  Revision 1.2  2005/08/20 07:01:19  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_CONNECTINET_H_
#define DWC_CONNECTINET_H_

#include <ac/dwc_ac.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DWCstInetControl
{
    volatile int ac_state;
    u16  state;
    u16  online;
    u16  dmaNo;
    u16  powerMode;

    u8            wontExecuteWDS;  // nintendo spot�ւ̐ڑ������݂�t�F�[�Y�𒆎~���邩�ǂ����̃t���O [masuda]
    DWCWDSState   processWDSstate; // WDS�̏����X�e�[�g
    DWCWDSData    nspotInfo;       // WDS�Ŏ擾����ssid,wep,wepmode,apnum���i�[���邽�߂̕ϐ�

#ifndef SDK_FINALROM
	int  connectType;
#endif	// SDK_FINALROM
} DWCInetControl;

typedef enum
{
    DWC_CONNECTINET_STATE_NOT_INITIALIZED = 0,
    DWC_CONNECTINET_STATE_IDLE,
    DWC_CONNECTINET_STATE_OPERATING,
    DWC_CONNECTINET_STATE_OPERATED,
    DWC_CONNECTINET_STATE_CONNECTED,
    DWC_CONNECTINET_STATE_DISCONNECTING,
    DWC_CONNECTINET_STATE_DISCONNECTED,
    DWC_CONNECTINET_STATE_ERROR,
    DWC_CONNECTINET_STATE_FATAL_ERROR,

    DWC_CONNECTINET_STATE_LAST
} DWCInetResult;

typedef enum
{
    DWC_CONNECTINET_AUTH_TEST,
    DWC_CONNECTINET_AUTH_DEVELOP,
    DWC_CONNECTINET_AUTH_RELEASE,
    DWC_CONNECTINET_AUTH_LAST
} DWCInetAuthType;

// [nakata] �������AP���擾�֘A�̊g������

// �G���A�R�[�h
typedef enum {
	DWC_APINFO_AREA_JPN = 0,
	DWC_APINFO_AREA_USA,
	DWC_APINFO_AREA_EUR,
	DWC_APINFO_AREA_AUS,
    DWC_APINFO_AREA_KOR,
	DWC_APINFO_AREA_UNKNOWN = 0xff
} DWCApInfoArea;

// AP�^�C�v
typedef enum {
	DWC_APINFO_TYPE_USER0 = 0,
	DWC_APINFO_TYPE_USER1,
	DWC_APINFO_TYPE_USER2,
	DWC_APINFO_TYPE_USB,
	DWC_APINFO_TYPE_SHOP,
	DWC_APINFO_TYPE_FREESPOT,
	DWC_APINFO_TYPE_WAYPORT,
	DWC_APINFO_TYPE_OTHER,          /* ����` */
	DWC_APINFO_TYPE_NINTENDOWFC = DWC_APINFO_TYPE_OTHER,    /* �\�� */
	DWC_APINFO_TYPE_NINTENDOSPOT,
	DWC_APINFO_TYPE_UNKNOWN = 99    /* �s�� */
} DWCApInfoType;

#define DWC_SIZE_SPOTINFO 9

typedef struct DWCstApInfo
{
	DWCApInfoType		aptype;
	DWCApInfoArea		area;
	char				spotinfo[DWC_SIZE_SPOTINFO+1];
	char				essid[WM_SIZE_SSID+1];
	u8 					bssid[WM_SIZE_BSSID];
} DWCApInfo;

BOOL DWC_GetApInfo(DWCApInfo *apinfo);
// [nakata] �����܂�AP���擾�֘A�̊g������

// [nakata] �������u���E�U�Ή��̊g������
void DWC_EnableHotspot(void);
// [nakata] �����܂Ńu���E�U�Ή��̊g������

// [uemura] 2006/05/24�ǉ�
BOOL DWC_CheckWiFiStation( const void *ssid, u16 len );
// [uemura] �����܂�

typedef void* (* DWCACAlloc )( u32 name, s32 size );				// alloc�֐��ւ̃|�C���^
typedef void  (* DWCACFree  )( u32 name, void *ptr, s32 size );		// free�֐��ւ̃|�C���^

extern void  DWC_InitInetEx( DWCInetControl* inetCntl, u16 dmaNo, u16 powerMode, u16 ssl_priority );
extern void  DWC_InitInet( DWCInetControl* inetCntl );
extern void  DWC_SetAuthServer( DWCInetAuthType type );

extern void  DWC_ConnectInetAsync( void );
extern void  DWC_DebugConnectInetAsync( const void *ssid, const void *wep, int wepMode );
extern void  DWC_DebugConnectInetExAsync( const void *ssid, const void *wep, int wepMode, const char *apSpotInfo, int overrideType );

extern BOOL  DWC_CheckInet( void );

extern void  DWC_ProcessInet( void );

extern DWCInetResult DWC_GetInetStatus( void );

extern void  DWC_CleanupInet( void );
extern BOOL  DWC_CleanupInetAsync( void );

extern BOOL  DWCi_CheckDisconnected( void );
extern WMLinkLevel DWC_GetLinkLevel( void );
extern int   DWC_GetUdpPacketDropNum( void );

// [nakata] 06/06/27 dwc_main.c����ړ�
extern void DWC_ForceShutdown( void );
extern BOOL DWC_UpdateConnection( void );
// [nakata] �����܂�

// [nakata] 06/11/06 �C���^�[�l�b�g�ڑ��m�F���������b�v����֐���ǉ�
typedef enum
{
    DWC_TESTINET_NOT_INITIALIZED = 0,
    DWC_TESTINET_OPERATING,
    DWC_TESTINET_CONNECTED,
    DWC_TESTINET_DISCONNECTED
} DWCTestInetResult;

BOOL DWC_TestInetAsync(int timeout);
DWCTestInetResult DWC_TestInetProcess(void);
// [nakata] �����܂�

#ifndef SDK_FINALROM
void         DWC_SetConnectApType( DWCApInfoType type );
#else
#define      DWC_SetConnectApType( type )       ((void)0)
#endif

#ifdef __cplusplus
}
#endif


#endif // DWC_CONNECTINET_H_

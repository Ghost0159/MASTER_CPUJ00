/*---------------------------------------------------------------------------*
  Project:  NitroDWC - ac - dwc_ac
  File:     dwc_ac.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  Revision 1.5  2005/08/20 07:01:18  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_AC_H_
#define DWC_AC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
 *	Project		: Wi-Fi Connection
 *	Description	: �l�b�g���[�N�ւ̎����ڑ����C�u�����p�w�b�_
 *	File		: dwc_ac.h
 *	Version		: 0.0
 *	Auther		: Uemura
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *	��`
 *---------------------------------------------------------------------------*/
// �����ڑ����̏��( DWC_AC_GetStatus�̖߂�l )
enum{
	// -10000 �ȏ�̃G���[�̓R�����g�̕\�����K�v�ɂȂ�܂�
	// -2xxxx NetCheck���̃G���[�R�[�h�A��ɔF�،n�̃G���[
	// -5xxxx AC��NetCheck���̃G���[�R�[�h�A�ڑ����s���̃G���[�i�ڍׂ͉��L�j
	// -5xxxx��xxxx�ɂ͍ł��C���^�[�l�b�g�ڑ��ɋ߂Â����ڑ���̃^�C�v�ƃG���[�ԍ�������܂��B
	// ��񌅂ɂ̓G���[�ԍ�������A���񌅂͐ڑ���^�C�v�������܂��B
	// ���  ���� �G���[���e
	// 00      XX     �ʐM���ؒf���ꂽ�B
	// 00      99     �`�o�̃r�[�R�����S���݂���Ȃ��B
	// 10      99     �`�o�̃r�[�R���͌����������ASSID����v������̂��Ȃ��B
	// 11      XX     WEP�L�[�ԈႢ�A���[�h�Ⴂ
	// 12      XX     AP�̐ڑ����������I�[�o�[�����B
	// 13      XX     �`�o�ɐڑ��ł��Ȃ��B
	// 20      XX     DHCP�G���[�B
	// 21      XX     ���O�����ł��Ȃ��BDNS�G���[�B
	// 22      XX     �C���^�[�l�b�g�ɐڑ��ł��Ȃ�1 Test Blacklist Site ����HTTPExcluding 200/302 or Timeout)
	// 23      XX     �C���^�[�l�b�g�ɐڑ��ł��Ȃ�2 (call Account Creates ���� HTTP Excluding 200 or Timeout or  CAinvalid)
	// 30      XX     �C���^�[�l�b�g�ɐڑ��ł��Ȃ�3 (hot spot error : Post203HTML and SSID to NAS�@���Ɂ@HTTPExcluding200 or Timeout or  CAinvalid)
	// 31      XX     �C���^�[�l�b�g�ɐڑ��ł��Ȃ�4 (hot spot error : Postto Hotspot Auth Server (HTTPS) ����Timeout  CAinvalid)
	// 32      XX     �C���^�[�l�b�g�ɐڑ��ł��Ȃ�5 (hot spot error : �Q�x�ڂ�Test Blacklist site ���Ɂ@HTTP302)

	// �ȉ��̓v���O�����I�ȃG���[�Q
	DWC_AC_STATE_ERROR_FATAL            = -10,				// �G���[ �����ʐM�ɒv���I�ȃG���[������
	DWC_AC_STATE_ERROR_START_UP,							// �G���[ �ڑ��J�n�iWCM_StartupAsync�j���s
	DWC_AC_STATE_ERROR_SOCKET_START,						// �G���[ �\�P�b�g�J�n�iSOC_Startup�j���s
	DWC_AC_STATE_ERROR_NETCHECK_CREATE,						// �G���[ �ڑ����e�X�g�J�n�iDWC_Netcheck_Create�j���s
	DWC_AC_STATE_ERROR_IRREGULAR,							// �G���[ WCM_GetPhase�̖߂�l���uWCM_PHASE_IRREGULAR�v�ɂȂ����ꍇ

	// �ȉ���DWC_AC_Process���̏��
	DWC_AC_STATE_NULL                   = 0,				// �������
	DWC_AC_STATE_SEARCH,									// ������
	DWC_AC_STATE_CONNECT,									// �`�o�֐ڑ���
	DWC_AC_STATE_TEST,										// �C���^�[�l�b�g�ւ̐ڑ��m�F��
	DWC_AC_STATE_RETRY,										// �ʂ̂`�o�֍Đڑ���
	DWC_AC_STATE_COMPLETE									// �ڑ�����
};

// �A�N�Z�X�|�C���g�̎��( DWC_AC_GetApType�̖߂�l )
enum{
	DWC_AC_AP_TYPE_USER1,									// ���[�U�[�ݒ�P
	DWC_AC_AP_TYPE_USER2,									// ���[�U�[�ݒ�Q
	DWC_AC_AP_TYPE_USER3,									// ���[�U�[�ݒ�R
	DWC_AC_AP_TYPE_USB,										// �j���e���h�[Wi-Fi USB�R�l�N�^
	DWC_AC_AP_TYPE_SHOP,									// �j���e���h�[Wi-Fi �X�e�[�V����
	DWC_AC_AP_TYPE_FREESPOT,								// FreeSpot
	DWC_AC_AP_TYPE_WAYPORT,									// WayPort
	DWC_AC_AP_TYPE_NINTENDOWFC,								// NINTENDOWFC
	DWC_AC_AP_TYPE_NINTENDOSPOT,							// nintendo spot
	DWC_AC_AP_TYPE_UNKNOWN = 99,							// �s��(�߂�l�Ƃ��Ă͕Ԃ���܂���)
	DWC_AC_AP_TYPE_FALSE = 0xff								// �擾���s
};

// WDS�p�o�b�t�@�T�C�Y
#define	DWC_WDS_SSID_BUF_SIZE				32
#define DWC_WDS_WEPKEY_BUF_SIZE             32

// WDS�p�X�e�[�g
typedef enum
{
    DWC_WDS_STATE_IDLE      = 0,
    DWC_WDS_STATE_PROCESS   = 1,
    DWC_WDS_STATE_COMPLETED = 2, // AP�̃f�[�^���擾�ł����B
    DWC_WDS_STATE_FAILED    = 3, // �擾�ł��Ȃ������B
    DWC_WDS_STATE_ERROR     = 4
} DWCWDSState;


/*---------------------------------------------------------------------------*
 *	�\���̒�`
 *---------------------------------------------------------------------------*/
typedef struct tagDWCACOption{								// AC�p�I�v�V�����ݒ�
	u8          connectType;								// �ڑ���^�C�v�̎w��i�O�F�w�薳���@�P�`�W�F�ڑ���w��j
	u8          skipNetCheck;								// �l�b�g�`�F�b�N�̃X�L�b�v�i�P�F�`�F�b�N���@�O�F�`�F�b�N�L�j
} DWCACOption;

typedef struct tagDWCACConfig{								// DWC_AC�֐��Q�p�R���t�B�O
	void*       (* alloc )( u32 name, s32 size );			// alloc�֐��ւ̃|�C���^
	void        (* free  )( u32 name, void *ptr, s32 size );// free �֐��ւ̃|�C���^
	u8          dmaNo;										// �O�`�R
	u8          powerMode;									// �P�FACTIVE�@�O�FSAVE
	DWCACOption option;										// �I�v�V�����ݒ�
} DWCACConfig;

typedef struct tagDWCWDSData{
    u8          ssid[ DWC_WDS_SSID_BUF_SIZE ];
    u8          wep[ DWC_WDS_WEPKEY_BUF_SIZE ];
    int         wepMode;
    char        apnum[10];
} DWCWDSData;

/*---------------------------------------------------------------------------*
 *	�֐���`
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_Create
 * Description	: �������֐��@�������̊m�ہADWC_AC�AWCM�̏��������s���܂�
 * Arguments	: *config - DWC_AC�p�R���t�B�O�f�[�^
 * Returns		: BOOL    - TRUE  �F ����
 *				:           FALSE �F ���s
 *---------------------------------------------------------------------------*/
BOOL DWC_AC_Create( DWCACConfig *config );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_Process
 * Description	: �����ڑ��̃��C���֐��ł�
 *				: �O�ȊO���߂�܂Ŗ��t���[���R�[�����Ă�������
 * Arguments	: None.
 * Returns		: int -  0 : ������
 *				:        1 : �ڑ�����
 *				:        2 : �ڑ������A������������SSID�𕡐�����(���[�U�[�ݒ�AP�Ɍ���)
 *				:       -1 : �G���[����
 *---------------------------------------------------------------------------*/
int  DWC_AC_Process( void );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_GetStatus
 * Description	: �����ڑ�����DWC_AC���C�u������Ԃ��擾���܂�
 * Arguments	: None.
 * Returns		: int - ���̒l : ���݂̏������e
 *				:       �s�̒l : �G���[�ڍ�
 *---------------------------------------------------------------------------*/
int  DWC_AC_GetStatus( void );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_GetApType
 * Description	: �ڑ�����AP�̃^�C�v���擾���܂�
 *				: DWC_AC_Process������������łȂ���Ύ擾�͐������܂���
 * Arguments	: None.
 * Returns		: u8 - 0 �` 7 �F �ڑ�����AP�̃^�C�v
 *				:        0xff �F �擾���s
 *---------------------------------------------------------------------------*/
u8   DWC_AC_GetApType( void );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_GetApSpotInfo
 * Description	: �ڑ�����AP�̏ꏊ�����擾���܂�
 *				: DWC_AC_Process������������łȂ���Ύ擾�͐������܂���
 * Arguments	: *apSpotInfo - �����i�[����o�b�t�@(10�o�C�g�K�v�ł�)
 * Returns		: BOOL    - TRUE  �F ����
 *				:           FALSE �F ���s
 *---------------------------------------------------------------------------*/
BOOL DWC_AC_GetApSpotInfo( u8 *apSpotInfo );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_Destroy
 * Description	: �I���֐��A�ؒf�����A�o�b�t�@�̊J�����s���܂�
 *				: TRUE�ɂȂ�܂Ŗ��t���[���R�[�����Ă�������
 * Arguments	: None.
 * Returns		: BOOL - TRUE  �F ����
 *				:        FALSE �F ���s
 *---------------------------------------------------------------------------*/
BOOL DWC_AC_Destroy( void );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_SetSpecifyAp
 * Description	: �w�肳�ꂽAP�ւ̐ڑ��ݒ���s��
 * Arguments	: *ssid   - �ڑ�����AP��SSID
 *              : *wep    - �ڑ�����AP��Wep�L�[
 *              : wepMode - Wep�L�[�̃��[�h
 * Returns		: None.
 *---------------------------------------------------------------------------*/
void DWC_AC_SetSpecifyAp( const void *ssid, const void *wep, int wepMode );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_SetSpecifyApEx
 * Description	: �w�肳�ꂽAP�ւ̐ڑ��ݒ���s��(�ꏊ���t)
 * Arguments	: *ssid        - �ڑ�����AP��SSID
 *              : *wep         - �ڑ�����AP��Wep�L�[
 *              : wepMode      - Wep�L�[�̃��[�h
 *              : apSpotInfo   - 10������AP�ꏊ���(�s�v�Ȃ�NULL���w��)
 *              :                10�o�C�g�Œ�œǂݏo���܂�
 *              : overrideType - �㏑������ڑ���^�C�v
 * Returns		: None.
 *---------------------------------------------------------------------------*/
void DWC_AC_SetSpecifyApEx( const void *ssid, const void *wep, int wepMode, const char *apSpotInfo, int overrideType );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_CheckWiFiStation
 * Description	: AP��SSID����Wi-Fi�X�e�[�V�������ǂ������m�F����
 * Arguments	: *ssid - ��������AP��SSID
 *              : len   - SSID�̒���
 * Returns		: BOOL  - TRUE  �F ����
 *				:         FALSE �F ���s
 *---------------------------------------------------------------------------*/
BOOL DWC_AC_CheckWiFiStation( const void *ssid, u16 len );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_StartupGetWDSInfo
 * Description	: nintendo spot�̐ڑ����邽�߂̏����擾����@������
 * Arguments	: DWCWDSData
 *              : 
 * Returns		: BOOL  - TRUE  �F ����
 *				:         FALSE �F ���s
 *---------------------------------------------------------------------------*/
BOOL DWC_AC_StartupGetWDSInfo( DWCWDSData* nspotInfo );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_ProcessGetWDSInfo
 * Description	: nintendo spot�ւ̐ڑ����邽�߂̏����擾����@������i�߂�
 * Arguments	: 
 *              : 
 * Returns		: DWCWDSResult
 *---------------------------------------------------------------------------*/
DWCWDSState DWC_AC_ProcessGetWDSInfo( void );

/*---------------------------------------------------------------------------*
 * Name			: DWC_AC_CleanupGetWDSInfo
 * Description	: nintendo spot�ւ̐ڑ����邽�߂̏����擾����@�J��
 * Arguments	: 
 *              : 
 * Returns		: None.
 *---------------------------------------------------------------------------*/
void DWC_AC_CleanupGetWDSInfo( void );



#ifdef __cplusplus
}
#endif


#endif // DWC_AC_H_

/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wmDEMOlib
  File:     wm_lib.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm_lib.h,v $
  Revision 1.48  2006/03/07 01:01:04  ooe
  wm_lib_add_parent_listEx()��ǉ��B

  Revision 1.47  2006/01/18 02:12:40  kitase_hirotake
  do-indent

  Revision 1.46  2005/06/07 05:45:56  seiki_masashi
  Key Sharing �Ɋւ�����ʏ�����ጸ���邽�߂̕ύX

  Revision 1.45  2005/03/03 10:20:13  ooe
  small fix

  Revision 1.44  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.43  2005/02/17 08:08:54  ooe
  wm_lib_set_ggidFilter��ǉ��B

  Revision 1.42  2005/02/17 00:16:06  ooe
  ggidFilter��ǉ��B

  Revision 1.41  2005/02/08 09:36:57  ooe
  copyright �N�\�L�C��.

  Revision 1.40  2005/02/08 06:57:39  ooe
  StartScanEx, SetWEPKey�ȂǂɑΉ��B

  Revision 1.39  2004/10/26 08:25:32  ooe
  WM_StartScanEx�ɑΉ��B
  scanExFlag��save_parentInfoEx()��ǉ��B

  Revision 1.38  2004/10/05 09:45:28  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.37  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.36  2004/09/14 09:51:34  ooe
  mpBusy�t���O��ǉ�

  Revision 1.35  2004/09/10 12:08:10  seiki_masashi
  �L�[�V�F�A�����O�̑��M�o�C�g�������C�u�������ŉB������悤�ɕύX

  Revision 1.34  2004/08/31 09:14:16  ooe
  WM_lib_param�ɁAno_retry�t���O��ǉ��BMP�̍đ����s�Ȃ�Ȃ��Ƃ��Ɏg�p����B

  Revision 1.33  2004/08/30 05:36:16  sato_masaki
  add wm_lib_set_gameinfo function

  Revision 1.32  2004/08/19 23:52:49  ikedae
  mpSending �폜

  Revision 1.31  2004/08/18 04:22:49  ooe
  WM_TYPE_API_ERROR ��ǉ�

  Revision 1.30  2004/08/18 00:40:28  seiki_masashi
  small fix

  Revision 1.29  2004/08/18 00:26:07  seiki_masashi
  recvBuf �� u16* �ɕύX

  Revision 1.28  2004/08/17 13:13:54  seiki_masashi
  WM_StartMP �̎d�l�ύX�ɑ΂���b��Ή�

  Revision 1.27  2004/08/16 10:11:02  ooe
  WM_lib_CalcRSSI��ǉ�

  Revision 1.26  2004/08/16 09:40:45  ooe
  WM_lib_param��currentTgid��ǉ�

  Revision 1.25  2004/08/10 05:45:39  ooe
  find_counter��ǉ�(Scan�����p)

  Revision 1.24  2004/08/07 09:52:23  ooe
  MP�֌W�̑啝�ύX�ɑΉ�

  Revision 1.23  2004/08/05 04:05:41  miya
  add functions

  Revision 1.22  2004/08/03 12:00:12  sato_masaki
  WMbssDesc�\���̌o�R�ŁA�ڑ��������s���֐��Bwm_lib_connect_parent_via_bssdesc��ǉ��B

  Revision 1.21  2004/08/02 05:37:47  ooe
  WM_TYPE_MPACK_IND��ǉ�

  Revision 1.20  2004/07/31 04:40:00  yosiokat
  WM_lib_get_max_scan_time�̒ǉ��B

  Revision 1.19  2004/07/30 09:22:33  miya
  scan time �ύX

  Revision 1.18  2004/07/30 03:59:07  miya
  DCF����̂���

  Revision 1.17  2004/07/29 07:40:48  miya
  modified callback table size

  Revision 1.16  2004/07/29 01:28:44  yosiokat
  void WM_lib_set_scan_channel_seek(BOOL flag)�̊֐��v���g�^�C�v�ǉ��B

  Revision 1.15  2004/07/29 00:15:06  miya
  bssid(MAC�A�h���X)�w��r�[�R���X�L�����ɑΉ�

  Revision 1.14  2004/07/28 09:03:43  miya
  samll fix

  Revision 1.13  2004/07/28 08:53:02  miya
  samll fix

  Revision 1.12  2004/07/28 08:34:36  miya
  add comment

  Revision 1.11  2004/07/28 04:25:53  miya
  add functions about ggid & tgid

  Revision 1.9  2004/07/28 03:53:28  ikedae
  WM_DMA_NO���w�b�_�Ɉړ�

  Revision 1.8  2004/07/28 02:46:47  terui
  Fix padding of structure

  Revision 1.7  2004/07/28 02:23:59  miya
  add functions about UserGameInfo

  Revision 1.6  2004/07/27 11:54:33  miya
  bssid�ݒ�ǉ�

  Revision 1.5  2004/07/27 11:31:23  miya
  event �ǉ�

  Revision 1.4  2004/07/27 09:23:10  ooe
  WM_TYPE_MP_SEND_DISABLE��ǉ�

  Revision 1.3  2004/07/27 08:07:25  yasu
  Add #pragma  warn_padding off

  Revision 1.2  2004/07/27 02:39:56  ikedae
  pollbitmap�Ή��� WM_lib_SetMPDataEX �ǉ�

  Revision 1.1  2004/07/23 15:13:50  terui
  �������ĐV�K�ǉ�

  Revision 1.5  2004/07/11 03:02:33  ooe
  WM_SetBeaconIndication�֌W�̒ǉ��Ȃ�

  Revision 1.4  2004/07/08 10:04:22  terui
  WM7�����J�n�ɔ����ύX

  Revision 1.19  2004/07/06 01:50:57  Ooe
  ����M�o�b�t�@�T�C�Y�v�Z�}�N����ǉ��E�C��

  Revision 1.18  2004/07/05 13:00:25  Miya
  keysharing, contSend�e�ݒ�ǉ�

  Revision 1.17  2004/07/05 06:03:15  Ikedae
  WM_TYPE_PARENT_STARTED�ǉ�

  Revision 1.16  2004/07/05 01:12:57  Sato
  add functions wm_lib_get_lib_param and wm_get_parent_bssdesc

  Revision 1.15  2004/07/03 07:29:47  Ooe
  WM_lib_GetKeySet()��ǉ�

  Revision 1.14  2004/07/03 06:55:04  Miya
  small change

  Revision 1.13  2004/07/03 01:48:56  Miya
  change checkMacAddress function to internal

  Revision 1.12  2004/07/02 04:51:46  Miya
  �L���b�V������T�C�Y�̏C��

  Revision 1.11  2004/07/01 03:38:11  Miya
  wm_tool.h wm_lib.h ����

  Revision 1.10  2004/06/29 06:58:44  Miya
  WM_CalcChildRecvBufSize�̏C��

  Revision 1.9  2004/06/29 06:43:54  Ooe
  �o�b�t�@�T�C�Y�v�Z�}�N�����C��

  Revision 1.8  2004/06/29 06:10:46  Ooe
  ���M���R�[���o�b�N�pTYPE(WM_TYPE_MP_SEND_ENABLE)��ǉ��B

  Revision 1.7  2004/06/24 11:21:42  Ooe
  WM_SetGameInfo()�̃G���[������ǉ��B

  Revision 1.6  2004/06/24 10:02:47  Ooe
  WM_SetGameInfo()��ǉ�

  Revision 1.5  2004/06/22 05:19:15  Ikedae
  WM_lib_Main�폜�֘A�̕ύX

  Revision 1.4  2004/06/21 06:39:48  Ooe
  (none)

  Revision 1.3  2004/06/19 05:56:58  Miya
  modified header file reference

  Revision 1.3  2004/06/19 04:03:14  miya
  ver. up Marionea-1.48.00

  Revision 1.2  2004/06/02 04:19:04  miya
  ver. up

  Revision 1.1  2004/05/26 08:47:30  terui
  �����g�ݍ��݂ɔ����ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef WMDEMOLIB_WM_LIB_H_
#define WMDEMOLIB_WM_LIB_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

/* wm_lib.h */
#include <nitro.h>
#include <nitro/wm/common/wm.h>

#define WM_MODE_MP_PARENT       1
#define WM_MODE_MP_CHILD        2
#define WM_MODE_INFRASTRUCTURE  3

#define WM_DMA_NO 2

#define WM_PORT_KEYSHARING      15

#define WM_TYPE_CHILD_CONNECTED             0
#define WM_TYPE_CHILD_DISCONNECTED          1
#define WM_TYPE_MP_PARENT_SENT              2
#define WM_TYPE_MP_PARENT_RECV              3
#define WM_TYPE_PARENT_FOUND                4
#define WM_TYPE_PARENT_NOT_FOUND            5
#define WM_TYPE_CONNECTED_TO_PARENT         6
#define WM_TYPE_DISCONNECTED                7
#define WM_TYPE_MP_CHILD_SENT               8
#define WM_TYPE_MP_CHILD_RECV               9
#define WM_TYPE_DISCONNECTED_FROM_PARENT    10
#define WM_TYPE_CONNECT_FAILED              11
#define WM_TYPE_DCF_CHILD_SENT              12
#define WM_TYPE_DCF_CHILD_SENT_ERR          13
#define WM_TYPE_DCF_CHILD_RECV              14
#define WM_TYPE_DISCONNECT_COMPLETE         15
#define WM_TYPE_DISCONNECT_FAILED           16
#define WM_TYPE_END_COMPLETE                17
#define WM_TYPE_MP_CHILD_SENT_ERR           18
#define WM_TYPE_MP_PARENT_SENT_ERR          19
#define WM_TYPE_MP_STARTED                  20
#define WM_TYPE_INIT_COMPLETE               21
#define WM_TYPE_END_MP_COMPLETE             22
#define WM_TYPE_SET_GAMEINFO_COMPLETE       23
#define WM_TYPE_SET_GAMEINFO_FAILED         24
#define WM_TYPE_MP_SEND_ENABLE              25
#define WM_TYPE_PARENT_STARTED              26
#define WM_TYPE_BEACON_LOST                 27
#define WM_TYPE_BEACON_SENT                 28
#define WM_TYPE_BEACON_RECV                 29
#define WM_TYPE_MP_SEND_DISABLE             30
#define WM_TYPE_DISASSOCIATE                31
#define WM_TYPE_REASSOCIATE                 32
#define WM_TYPE_AUTHENTICATE                33
#define WM_TYPE_SET_LIFETIME                34
#define WM_TYPE_DCF_STARTED                 35
#define WM_TYPE_DCF_SENT                    36
#define WM_TYPE_DCF_SENT_ERR                37
#define WM_TYPE_DCF_RECV                    38
#define WM_TYPE_DCF_END                     39
#define WM_TYPE_MPACK_IND                   40
#define WM_TYPE_MP_CHILD_SENT_TIMEOUT       41
#define WM_TYPE_SEND_QUEUE_FULL_ERR         42

#define WM_TYPE_API_ERROR                   255 // API�R�[���̖߂�l�ŃG���[
#define WM_TYPE_ERROR                       256 // �R�[���o�b�N�ł̃G���[


#define WL_HEADER_LENGTH_P  48         // �e�@��M����WL�w�b�_��
#define WM_HEADER_LENGTH    (2+4)      // WMHeader��
#define WL_HEADER_LENGTH_T  8          // �S�̂�WL�w�b�_��
#define WL_HEADER_LENGTH_C  8          // ���ꂼ��̎q�@�f�[�^��WL�w�b�_��

// �}�N�� �q�@��M�o�b�t�@�T�C�Y�v�Z
#define WM_CalcChildSendBufSize(_pInfo_)        (u16)(WM_SIZE_MP_CHILD_SEND_BUFFER(((WMgameInfo *)(&(((WMbssDesc*)(_pInfo_))->gameInfo)))->childMaxSize, TRUE))
#define WM_CalcChildRecvBufSize(_pInfo_)        (u16)(WM_SIZE_MP_CHILD_RECEIVE_BUFFER(((WMgameInfo *)(&(((WMbssDesc*)(_pInfo_))->gameInfo)))->parentMaxSize, TRUE))

// �}�N�� �e�@����M�o�b�t�@�T�C�Y�v�Z
#define WM_CalcParentSendBufSize(_libParam_)    (u16)(WM_SIZE_MP_PARENT_SEND_BUFFER(((WM_lib_param*)(&(_libParam_)))->parentParam->parentMaxSize, TRUE))
#define WM_CalcParentRecvBufSize(_libParam_)    (u16)(WM_SIZE_MP_PARENT_RECEIVE_BUFFER(((WM_lib_param*)(&(_libParam_)))->parentParam->childMaxSize, WM_NUM_MAX_CHILD, TRUE))


typedef void (*WMcallbackFunc2) (u16 type, void *arg);  // WM�̃R�[���o�b�N�^(���̂Q)

// wm_lib �p�����[�^�\����
typedef struct
{
    void   *wmBuf;                     // WM�V�X�e���o�b�t�@
    WMpparam *parentParam;             // �e�@���ݒ�(�e�@���g�p)
    WMbssDesc *parentInfoBuf;          // �e�@���擾�o�b�t�@(�q�@���g�p)
    u16    *sendBuf;                   // ���M�o�b�t�@
    u16    *recvBuf;                   // MP��M�o�b�t�@
    WMdcfRecvBuf *dcfBuf;              // DCF��M�o�b�t�@
    WMstatus *statusBuf;               // �X�e�[�^�X�擾�p�o�b�t�@(�ł���΍폜������)
    WMcallbackFunc2 callback;          // WM_lib�p�R�[���o�b�N
    WMbssDesc *pInfo;                  // �ڑ���e�@���|�C���^(�q�@�Ŏg�p)
    u16     sendBufSize;               // ���M�o�b�t�@�̃T�C�Y
    u16     recvBufSize;               // MP��M�o�b�t�@�̃T�C�Y
    u16     dcfBufSize;                // DCF��M�o�b�t�@�̃T�C�Y
    u16     mode;                      // WM_MODE_***
    u16     endReq;
    u16     mpStarted;                 // MP�J�n�ς݃t���O
    u16     mpBusy;                    // MP���M�v�����t���O
    u16     child_bitmap;              // �q�@�ڑ���
    u16     parentInfoBufSize;         // �e�@���擾�o�b�t�@�̃T�C�Y(StartScanEx�Ŏg�p)

    // KeySharing�֘A
    u16     keySharing;                // KeySharing���s�t���O(1:KS�����s, 0:KS�����s���Ȃ�)
    WMkeySetBuf *ksBuf;                // KeySharing�p�o�b�t�@

    // �A�����M�֘A
    u16     contSend;                  // �A�����M���s�t���O(1:�A�����M, 0:1�t���[�����̒ʐM)

    // wep�֘A
    u16     wepMode;                   // �ڑ��Ɏg�p����WEPmode
    u16     wepKeyId;                  // �ڑ��Ɏg�p����WEPKey��ID
    u8      wepKey[80];                // �ڑ��Ɏg�p����WEPkey

    // �ڑ��F�؊֘A
    u16     authMode;                  // WM_AUTHMODE_OPEN_SYSTEM or WM_AUTHMODE_SHARED_KEY

    // gameinfo�֘A
    u8     *uname;                     // UserName
    u8     *gname;                     // GameName
    u16     currentTgid;               // �ڑ���e�@��TGID(BeaconRecv.Ind�Ń`�F�b�N����)

    // user gameinfo�֘A
    u16     userGameInfoLength;        // ���[�U�[�̈�̒���
    u16    *userGameInfo;              // ���[�U�[�̈�o�b�t�@�̃|�C���^

    // MP�đ��֘A
    BOOL    no_retry;                  // �đ��������s�t���O(FALSE:�đ������s, TRUE:�đ�����)

    // ScanEx�֘A
    BOOL    scanExFlag;                // ScanEx���g�p����ꍇ��TRUE�ɂ���

    u32     ggidFilter;                // GGID�t�B���^�����O(0xffffffff�Ȃ�S���ʉ�)

//    u16             rsv1;               // for 4byte padding
}
WM_lib_param;

//-----------------------------------
// WM_lib_Init()
int     WM_lib_Init(WM_lib_param * para);       // WM_lib�̏�����

//-----------------------------------
// WM_lib_SetMPData()
int     WM_lib_SetMPData(u16 *data, u16 size);  // MP�f�[�^�̑��M

//-----------------------------------
// WM_lib_SetMPDataEX()
int     WM_lib_SetMPDataEX(u16 *data, u16 size, u16 pollbitmap);
                                                // MP�f�[�^�̑��M(pollbitmap�Ή��j

//-----------------------------------
// WM_lib_End()
int     WM_lib_End(void);              // �e�@���[�h �I���v��

//-----------------------------------
// WM_lib_ConnectToParent()
int     WM_lib_ConnectToParent(WMbssDesc *pInfo);       // �e�@�ւ̐ڑ��v��

//-----------------------------------
// WM_lib_SetDCFData()
int     WM_lib_SetDCFData(const u8 *destAdr, u16 *data, u16 size);      // DCF�f�[�^�̑��M

//-----------------------------------
// WM_lib_SetGameInfo()
int     WM_lib_SetGameInfo(u16 *userGameInfo, u16 size, u32 ggid, u16 tgid);

//-----------------------------------
// WM_lib_Disconnect()
int     WM_lib_Disconnect(u16 aid);    // �e�@����q�@(aid)��ؒf����


//-----------------------------------
// WM_lib_CheckMPSend()
BOOL    WM_lib_CheckMPSend(void);      // MP���M���`�F�b�N


//-----------------------------------
// WM_lib_GetKeySet()
int     WM_lib_GetKeySet(WMkeySet *keySet);

//-----------------------------------
// WM_lib_set_auto()
void    WM_lib_set_auto(BOOL flag);    // �e�q�@�Ƃ������o�^

//-----------------------------------
// WM_lib_set_bssid()
void    WM_lib_set_bssid(u16 bssid0, u16 bssid1, u16 bssid2);   // BssId�ݒ�

//-----------------------------------
// WM_lib_CurrentScanChannel()

void    WM_lib_set_mp_dcf_parallel(BOOL flag);
void    WM_lib_set_no_retry_flag(BOOL flag);

//-----------------------------------
// Scan�p�p�����[�^�ݒ�E�擾�֐�
void    WM_lib_set_max_scan_time(u16 time);     // Scan�p�����Ԑݒ�
u16     WM_lib_get_max_scan_time(void); // Scan�p�����Ԏ擾
void    WM_lib_set_scan_channel_seek(BOOL flag);        // �`�����l���V�[�N�ݒ�
void    WM_lib_set_channel(u16 channel);        // �`�����l���ݒ�
u16     WM_lib_CurrentScanChannel(void);        // ���݃X�L�������̃`�����l����Ԃ�
void    WM_lib_set_scanBssid(u8 *bssid);        // BSSID�t�B���^�����O

//-----------------------------------
// ScanEx�p�p�����[�^�ݒ�E�擾�֐�
void    WM_lib_set_scanEx_maxChannelTime(u16 time);     // maxChannelTime�ݒ�
u16     WM_lib_get_scanEx_maxChannelTime(void); // maxChannelTime�擾
void    WM_lib_set_scanEx_channelList(u16 channel);     // channelList(bitmap)�ݒ�
u16     WM_lib_get_scanEx_channelList(void);    // channelList(bitmap)�擾
void    WM_lib_set_scanEx_scanType(u16 scanType);       // ScanType�ݒ� WM_SCANTYPE_ACTIVE(0), WM_SCANTYPE_PASSIVE(1)
u16     WM_lib_get_scanEx_scanType(void);       // ScanType�擾 WM_SCANTYPE_ACTIVE(0), WM_SCANTYPE_PASSIVE(1)
void    WM_lib_set_scanEx_ssidLength(u16 ssidLength);   // SSID�t�B���^�����O�pSSID���ݒ�
void    WM_lib_set_scanEx_ssid(u8 *ssid);       // SSID�t�B���^�����O�pSSID�ݒ�
void    WM_lib_set_scanEx_bssid(u8 *bssid);     // BSSID�t�B���^�����O�pBSSID�ݒ�

//-----------------------------------
// WM_lib_CalcRSSI()
u16     WM_lib_CalcRSSI(u16 rate_rssi, u16 aid);        // rate_rssi��RSSI�l�ɕϊ�


/************ wm_tool.h �������Ƃ� �J�n **********************/


#define WM_NUM_PARENT_INFORMATIONS 16
#define WM_NUM_CHILD_INFORMATIONS 15


typedef struct
{
    u16     found_parent_count;        // �����ςݐe�@��
    u16     my_aid;                    // ���g�Ɋ��蓖�Ă�ꂽAID;
    WMmpRecvBuf *recvBuf;              // WM_StartMP()�ň����n���AMP�o�b�t�@
    u16    *sendBuf;                   // WM_SetMPData()�ň����n���AMP���M�f�[�^�o�b�t�@
    u8      MacAdrList[WM_NUM_PARENT_INFORMATIONS][6];  // �e�@MacAddress�ۑ��p(4�䕪)
    WMbssDesc *parentInfo[WM_NUM_PARENT_INFORMATIONS];  // �e�@���i�[�p(4�䕪) �ڑ����Ɏg�p����BssDesc
    u16     channelList[WM_NUM_PARENT_INFORMATIONS];    // �e�@channel�ۑ��p(4�䕪)
    u16     ssidLength[WM_NUM_PARENT_INFORMATIONS];     // �e�@SSID�̒���
    u8      ssidList[WM_NUM_PARENT_INFORMATIONS][32];   // �e�@SSID�ۑ��p(4�䕪)
    u16     gameInfoLength[WM_NUM_PARENT_INFORMATIONS]; // �e�@GameInfo�̒���

    u16     find_counter[WM_NUM_PARENT_INFORMATIONS];   // �e�@������

    u16     rsv[10];
    WMgameInfo gameInfoList[WM_NUM_PARENT_INFORMATIONS] ATTRIBUTE_ALIGN(32);    // �e�@GameInfo�ۑ��p(4�䕪)
}
childInfo;


typedef struct
{
    u8      addr[6];
    u8      reserved[2];               // for 4 byte padding
}
MACADDRESS;

typedef struct
{
    int     valid;
    MACADDRESS macaddr;
}
CHILD_INFO;

typedef void (*WM_LIB_CALLBACK) (u16 type, void *arg);


int     wm_lib_get_last_found_parent_no(void);
void    save_parentInfo(WMstartScanCallback *buf, WM_lib_param * param, childInfo * info);
void    save_parentInfoEx(WMstartScanExCallback *buf, WM_lib_param * param, childInfo * info);


BOOL    wm_lib_get_own_macaddress(MACADDRESS * ma);





/* �������p�֐� */



/* �����\��̊֐� */
void    wm_lib_add_child_list(WMstartParentCallback *arg);
void    wm_lib_delete_child_list(WMstartParentCallback *arg);
void    wm_lib_add_parent_list(WMstartScanCallback *arg);
void    wm_lib_add_parent_listEx(WMStartScanExCallback *arg);
void    wm_lib_delete_parent_list(WMstartScanCallback *arg);
void    wm_lib_parent_found_count_reset(void);
void    wm_lib_set_my_aid(int aid);

/* �e�@�q�@���p�֐� */
void    wm_lib_comm_init(void);
int     wm_lib_start(void);
const char *wm_lib_get_wlib_version(void);
void    wm_lib_set_mode(u16 mode);
void    wm_lib_set_recvbuffer(u8 *buf);
void    wm_lib_set_sendbuffer(u8 *buf);
void    wm_lib_set_callback(WM_LIB_CALLBACK callback);
int     wm_lib_get_recvbuffer_size(void);
int     wm_lib_get_sendbuffer_size(void);
void    wm_lib_set_keySharing_mode(int flag);
void    wm_lib_set_contSend_mode(int flag);
void    wm_lib_set_multiboot_mode(int flag);
void    wm_lib_set_ni_mode(int flag);
void    wm_lib_set_ggid(u32 ggid);
u32     wm_lib_get_ggid(void);
void    wm_lib_set_tgid(u16 tgid);
u16     wm_lib_get_tgid(void);
void    wm_lib_set_scan_bssid(u16 bssid0, u16 bssid1, u16 bssid2);


/* �e�@�p�֐� */
void    wm_lib_set_gameinfo_gamename(char *str);
void    wm_lib_set_gameinfo_username(char *user_name);
u32     wm_lib_get_parent_gameinfo_ggid(int id);
u16     wm_lib_get_parent_gameinfo_tgid(int id);
u16    *wm_lib_get_parent_gameinfo_usergameinfo(int id);
u16     wm_lib_get_parent_gameinfo_usergameinfosize(int id);

void    wm_lib_set_parent_send_size(u16 size);
void    wm_lib_set_parent_channel(u16 ch);
void    wm_lib_set_beacon_period(u16 period_ms);


void    wm_lib_set_parent_recv_size_per_child(u16 size);
BOOL    wm_lib_get_child_macaddress(int aid, MACADDRESS * ma);
BOOL    wm_lib_is_child_valid(int aid);
int     wm_lib_set_gameinfo(void);

/* �q�@�p�֐� */

BOOL    wm_lib_is_parent_gameinfo_valid(int id);
int     wm_lib_get_parent_gameinfo_channel(int id);
int     wm_lib_get_parent_gameinfo_parent_sendmaxsize(int id);
int     wm_lib_get_parent_gameinfo_child_sendbufsize(int id);
int     wm_lib_get_parent_gameinfo_child_recvbufsize(int id);
const char *wm_lib_get_parent_gameinfo_username(int id);
const char *wm_lib_get_parent_gameinfo_gamename(int id);


void   *WM_lib_get_mp_parent_callback_ptr(void);
void   *WM_lib_get_mp_child_callback_ptr(void);


int     wm_lib_get_parent_found_count(void);
int     wm_lib_connect_parent(int parent_no);
BOOL    wm_lib_get_parent_macaddress(int id, MACADDRESS * ma);
int     wm_lib_get_my_aid(void);

/*  get Keysharing Keyset   */
int     wm_lib_get_keyset(WMkeySet *keyset);
int     wm_lib_connect_parent_via_bssdesc(WMbssDesc *bssDesc);

void    wm_lib_set_ggidFilter(u32 ggidFilter);

/*
    get pointer to  WMbssDesc structure
*/
WMbssDesc *wm_get_parent_bssdesc(int id, WMbssDesc *bssDescp);

int     wm_lib_connect_parent_via_bssdesc(WMbssDesc *bssDescp);

/************ wm_tool.h �������Ƃ� �I�� **********************/

/****************************************************/


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* WMDEMOLIB_WM_LIB_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

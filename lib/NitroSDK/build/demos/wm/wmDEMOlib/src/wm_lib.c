/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - libraries
  File:     wm_lib.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm_lib.c,v $
  Revision 1.74  2007/02/20 00:28:08  kitase_hirotake
  indent source

  Revision 1.73  2006/03/07 01:02:20  ooe
  StartScanEx�̃R�[���o�b�N���̏������C���B

  Revision 1.72  2006/02/20 08:03:48  seiki_masashi
  WM_IsValidGameInfo �֐����g�p����悤�ɕύX

  Revision 1.71  2006/02/20 02:39:35  seiki_masashi
  WMGameInfo.version ��p�~���AmagicNumber �� ver ��ǉ�
  WM_GAMEINFO_TYPE_OLD �̃T�|�[�g��p�~

  Revision 1.70  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.69  2005/11/01 00:34:38  seiki_masashi
  WM_STATECODE_DISCONNECTED_FROM_MYSELF �̒ǉ��ɑΉ�

  Revision 1.68  2005/06/07 05:45:56  seiki_masashi
  Key Sharing �Ɋւ�����ʏ�����ጸ���邽�߂̕ύX

  Revision 1.67  2005/06/02 09:05:28  terui
  WM_SetBeaconPeriod ���g�p���镔�����폜

  Revision 1.66  2005/03/17 07:16:52  ooe
  ScanEx�ɑ΂���C����DCF���[�h�ɂ��Ή��B

  Revision 1.65  2005/03/16 08:46:27  ooe
  WM_StartScanEx�Œʒm�����APIID��SCAN_EX�ɂ���C���ɑΉ��B

  Revision 1.64  2005/03/02 08:28:42  ooe
  �f�o�b�O�o�͒ǉ��Ȃ�

  Revision 1.63  2005/03/01 10:37:36  terui
  �R�����gFIX

  Revision 1.62  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.61  2005/02/15 00:09:42  ooe
  WMi_StartScanEx -> WM_StartScanEx
  ignoreFatalError�t���O��contSend�t���O�ƘA������悤�ɏC���B

  Revision 1.60  2005/02/14 23:54:05  terui
  WMi_StartScanEx -> WM_StartScanEx

  Revision 1.59  2005/02/08 06:53:38  ooe
  WM_StartScanEx�̎d�l�ύX�ɔ����啝�C���B

  Revision 1.58  2005/01/18 09:54:31  yosizaki
  copyright �N�\�L�C��.

  Revision 1.57  2005/01/17 01:09:32  ooe
  WM_StartConnectEx�ɑΉ��B

  Revision 1.56  2004/11/09 02:51:00  seiki_masashi
  Null �����ɂ��^�C���A�E�g������

  Revision 1.55  2004/10/29 04:38:02  terui
  WM_StartScanEx�֐�����U����J�Ƃ������ƂɑΉ��B

  Revision 1.54  2004/10/26 08:27:08  ooe
  WM_StartScanEx�ɑΉ��B

  Revision 1.53  2004/10/18 04:17:46  terui
  �r�[�R���Ԋu�A�X�L�����Ԋu���΂������d�l��ǉ��B

  Revision 1.52  2004/10/05 09:45:28  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.51  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.50  2004/10/03 08:15:26  ooe
  small fix

  Revision 1.49  2004/10/03 08:11:50  ooe
  StartMPEx�̎d�l�ύX�ɑΉ��B

  Revision 1.48  2004/09/20 10:20:41  ooe
  WM_lib_End��endReq������C���B
  WM_SetBeaconPeriod�ǉ��ɑΉ��B

  Revision 1.47  2004/09/18 10:05:52  sato_masaki
  WM_lib_End() ��������d�ɌĂяo���ꂽ�ꍇ�ւ̑΍�B

  Revision 1.46  2004/09/15 05:42:48  ooe
  beaconRecv.Ind�̃R�[���o�b�N�����ύX�ɔ����Atgid���蕔���C���B

  Revision 1.45  2004/09/14 09:51:15  ooe
  mpBusy�t���O��ǉ�

  Revision 1.44  2004/09/13 09:25:06  ooe
  LIFE_TIME�L��ɖ߂���

  Revision 1.43  2004/09/03 10:13:15  ooe
  Scan����seek�`�����l����1,7,13�ɕύX

  Revision 1.42  2004/09/01 09:15:42  ooe
  WM_ERRCODE_INVALID_POLLBITMAP�̏�����ǉ�

  Revision 1.41  2004/08/31 09:13:18  ooe
  WM_StartMP��Ex�ɕύX���A�đ��������[�h�ɑΉ������B

  Revision 1.40  2004/08/30 01:57:30  seiki_masashi
  WM_STATECODE_CHILD_CONNECTED �� WM_STATECODE_CONNECTED �ɕύX

  Revision 1.39  2004/08/26 09:49:06  ooe
  BeaconRecv���̃X�e�[�g�`�F�b�N��ǉ�

  Revision 1.38  2004/08/23 04:40:35  ooe
  WM_StartConnect()�̎d�l�ύX�ɑΉ��B

  Revision 1.37  2004/08/19 15:27:01  miya
  modified SetGameInfo function.

  Revision 1.36  2004/08/19 11:17:41  miya
  add GAMEINFO_VERSION

  Revision 1.35  2004/08/19 02:02:38  miya
  modified WMGameInfo format

  Revision 1.34  2004/08/18 04:22:25  ooe
  CHK_ERRCODE��check_WMAPI_errcode()�ɕύX

  Revision 1.33  2004/08/18 00:26:25  seiki_masashi
  small fix

  Revision 1.32  2004/08/17 13:13:27  seiki_masashi
  WM_StartMP �̎d�l�ύX�ɑ΂���b��Ή�

  Revision 1.31  2004/08/16 10:10:46  ooe
  WM_lib_CalcRSSI��ǉ�

  Revision 1.30  2004/08/16 09:39:56  ooe
  BeaconRecvInd����TGID�`�F�b�N������ǉ�

  Revision 1.29  2004/08/12 04:02:28  ooe
  scan���Ԃ̃����_�����������R�����g�A�E�g

  Revision 1.28  2004/08/11 08:35:19  ooe
  Scan���Ԃ̃����_�����e�X�g��g�ݍ���

  Revision 1.27  2004/08/10 05:47:08  ooe
  WM_lib_set_channel()��ǉ�(Scan�����p)

  Revision 1.26  2004/08/07 09:52:16  ooe
  MP�֌W�̑啝�ύX�ɑΉ�

  Revision 1.25  2004/08/05 04:05:36  miya
  add functions

  Revision 1.24  2004/08/05 01:20:56  ikedae
  mpSending����ύX

  Revision 1.23  2004/08/04 09:30:37  ikedae
  �q�@�̎�M����bitmap��0�̎� SEND_ENABLE�𔭍s���Ȃ��悤�ɕύX

  Revision 1.22  2004/08/02 05:37:24  ooe
  MPACK.Ind�ɑΉ�

  Revision 1.21  2004/07/31 08:46:17  yosiokat
  WM_lib_set_max_scan_time��wm_lib_start�̑O�ŃR�[������ƁAWM_APIID_SET_LIFETIME�R�[���o�b�N���Ƀf�t�H���g�ݒ�ɖ߂���Ă��܂��̂��C���B

  Revision 1.20  2004/07/31 04:40:09  yosiokat
  WM_lib_get_max_scan_time�̒ǉ��B

  Revision 1.19  2004/07/30 09:22:42  miya
  scan time �ύX

  Revision 1.18  2004/07/30 03:59:14  miya
  DCF����̂���

  Revision 1.17  2004/07/29 12:27:11  miya
  ���C�t�^�C����ݒ�

  Revision 1.16  2004/07/29 07:41:02  miya
  modified callback table size

  Revision 1.15  2004/07/29 01:49:49  miya
  �����r�[�R���X�L�����������߂�

  Revision 1.14  2004/07/29 00:45:12  miya
  �X�L�������̃`�����l���ύX���~�ł���悤�ɕύX

  Revision 1.13  2004/07/29 00:15:13  miya
  bssid(MAC�A�h���X)�w��r�[�R���X�L�����ɑΉ�

  Revision 1.12  2004/07/28 12:03:54  ikedae
  mpSending����ύX

  Revision 1.11  2004/07/28 11:00:30  ooe
  DCF���[�h��Scan���C��

  Revision 1.10  2004/07/28 09:03:48  miya
  samll fix

  Revision 1.9  2004/07/28 04:28:53  miya
  add functions about ggid & tgid

  Revision 1.8  2004/07/28 03:53:28  ikedae
  WM_DMA_NO���w�b�_�Ɉړ�

  Revision 1.7  2004/07/27 11:54:22  miya
  bssid�ݒ�ǉ�

  Revision 1.6  2004/07/27 11:31:37  miya
  event �ǉ�

  Revision 1.5  2004/07/27 09:00:59  ooe
  WM_APIID_INDICATION�̏���(�������Ȃ�)��ǉ�

  Revision 1.4  2004/07/27 06:41:51  miya
  hook IndicationCallback and error event

  Revision 1.3  2004/07/27 02:52:02  ooe
  ScanChannel�ύX�֐���ǉ�

  Revision 1.2  2004/07/27 02:39:56  ikedae
  pollbitmap�Ή��� WM_lib_SetMPDataEX �ǉ�

  Revision 1.1  2004/07/23 15:29:17  terui
  �������ĐV�K�ǉ�

  Revision 1.4  2004/07/23 14:41:47  terui
  DMA�ԍ�3��GX�Ƃ��������̂ŁA2�ɕύX�B
  �C���N���[�h�w�b�_���ꕔ�ύX

  Revision 1.3  2004/07/22 08:30:48  ikedae
  scanParam �� 32�o�C�g�A���C���ɕύX

  Revision 1.2  2004/07/20 12:46:15  terui
  �C���N���[�h����w�b�_��ǉ��B

  Revision 1.1  2004/07/20 11:26:09  terui
  wm_lib�؂藣���ɔ�������

  Revision 1.10  2004/07/20 01:16:49  terui
  WM_SetGameInfo�̎d�l�ύX�ɑΉ��B

  Revision 1.9  2004/07/18 11:29:44  seiki_masashi
  add WM_Initialize wait loop

  Revision 1.8  2004/07/15 01:09:33  ooe
  �q�@StartKeySharing()�̃R�[���o�b�N�w����C��

  Revision 1.7  2004/07/15 00:16:20  ooe
  scan�p���t���O���쐬

  Revision 1.6  2004/07/14 09:41:47  ooe
  StartScan�̈����ύX�ɔ����Ƃ肠�����̏C��

  Revision 1.5  2004/07/11 03:03:21  ooe
  WM_SetBeaconIndication�֌W�̒ǉ��Ȃ�

  Revision 1.4  2004/07/08 10:07:46  terui
  WM7�����J�n�ɔ����ύX

  Revision 1.18  2004/07/05 06:03:27  Ikedae
  WM_TYPE_PARENT_STARTED�ǉ�

  Revision 1.17  2004/07/03 07:31:17  Ooe
  WM_lib_GetKeySet()��ǉ�

  Revision 1.16  2004/07/02 04:51:37  Miya
  �L���b�V������T�C�Y�̏C��

  Revision 1.15  2004/07/01 03:38:28  Miya
  wm_tool.h wm_lib.h ����

  Revision 1.14  2004/06/29 06:45:55  Miya
  �f�[�^�T�C�Y�̃n�[�h�R�[�f�B���O���ł��邾���}�N����

  Revision 1.13  2004/06/29 06:20:48  Ooe
  ���M���R�[���o�b�N(WM_TYPE_MP_SEND_ENABLE)��ǉ��B

  Revision 1.12  2004/06/29 06:08:37  Miya
  WMCallback *buf ���ĂƂ���̂���Ȃ����S�ď���

  Revision 1.11  2004/06/25 06:18:10  Ooe
  WM_SetMPData()�̈����ǉ� (pollbmp)

  Revision 1.10  2004/06/24 11:21:59  Ooe
  WM_SetGameInfo()�̃G���[������ǉ��B

  Revision 1.9  2004/06/24 10:04:10  Ooe
  WM_SetGameInfo()��ǉ�

  Revision 1.8  2004/06/23 06:34:31  Ikedae
  WM_lib�֐��͑S�Đ�������WM_ERRCODE_SUCCESS��Ԃ��悤�ɕύX

  Revision 1.7  2004/06/22 06:22:15  Ooe
  �e�@����̐ؒf�����ŁA�q�@���̍X�V������ǉ��B

  Revision 1.6  2004/06/22 05:19:17  Ikedae
  WM_lib_Main�폜�֘A�̕ύX

  Revision 1.5  2004/06/21 06:40:40  Ooe
  (none)

  Revision 1.4  2004/06/19 05:56:29  Miya
  modified header file reference

  Revision 1.3  2004/06/19 04:05:10  miya
  ver. up Marionea-1.48.00

  Revision 1.2  2004/06/02 02:29:40  miya
  ver. up

  Revision 1.1  2004/05/26 08:55:07  terui
  �����g�ݍ��݂ɔ����ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

// 2005/05/26 update by terui@nintendo
#include <nitro/wm.h>
#include "wm_lib.h"
#include <stdlib.h>
#include <string.h>

void    mprintf(const char *fmt, ...);

#undef WMLIB_DEBUG                     // �f�o�b�O�o��

#define CAM_LIFE_TIME       40
#define FRAME_LIFE_TIME     5
#define TABLE_NO          0xffff

#undef  NO_LIFETIME

#define STARTCONNECT_EX_TEST	1      // StartConnectEx����

static BOOL WM_lib_auto_flag = FALSE;
static BOOL WM_lib_mp_dcf_parallel = FALSE;


static WM_lib_param *param = NULL;     // �p�����[�^�\���̂ւ̃|�C���^

static void WM_lib_mp_parent_callback(void *arg);       // MP �e�@���[�h �R�[���o�b�N
static void WM_lib_mp_child_callback(void *arg);        // MP �q�@���[�h �R�[���o�b�N
static void WM_lib_dcf_child_callback(void *arg);       // DCF �q�@���[�h �R�[���o�b�N
static void WM_lib_dummy(void *arg);   // �_�~�[�R�[���o�b�N
static void changeScanChannel(WMscanParam *param);      // Scan�`�����l���ύX�֐�

static void (*callback_ptr) (void *arg);

static const u8 test_ssid[WM_SIZE_CHILD_SSID] = "TEST SSID\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";      // 24Byte

#if 0
#define CHK_ERRCODE(x,y)    if ( x != WM_ERRCODE_OPERATING &&           \
                                    x != WM_ERRCODE_SUCCESS)            \
                            {                                           \
                                param->callback( WM_TYPE_ERROR, y); \
                            }
#else
static void check_WMAPI_errcode(u16 apiid, int errcode);
#endif

static inline int conv_errcode(int errcode)
{
    return (errcode == WM_ERRCODE_OPERATING) ? WM_ERRCODE_SUCCESS : errcode;
}

static WMscanParam scanParam ATTRIBUTE_ALIGN(32);
static WMscanExParam scanExParam ATTRIBUTE_ALIGN(32);
static u16 scanning_flag = 0;

static BOOL WM_lib_change_scan_channel_flag = TRUE;

static u16 maxChannelTime_org;

//--------------------------------------------------------
// WM�̎����e�@�q�@�o�^�@�\�t���O �n�m
void WM_lib_set_auto(BOOL flag)
{
    WM_lib_auto_flag = flag;
}
void WM_lib_set_mp_dcf_parallel(BOOL flag)
{
    WM_lib_mp_dcf_parallel = flag;
}

// MaxScanTime�ݒ�
void WM_lib_set_max_scan_time(u16 time)
{
    scanParam.maxChannelTime = time;
    maxChannelTime_org = time;
}

// MaxScanTime�擾
u16 WM_lib_get_max_scan_time(void)
{
    return scanParam.maxChannelTime;
}

// ScanChannel�ݒ�
void WM_lib_set_channel(u16 channel)
{
    if (channel == 0)
    {
        WM_lib_set_scan_channel_seek(TRUE);
        scanParam.channel = 1;
    }
    else
    {
        WM_lib_set_scan_channel_seek(FALSE);
        scanParam.channel = channel;
    }
}

// Scan����MacAddress(BSSID)�t�B���^�����O�ɗp����MacAddress�ݒ�
void WM_lib_set_scanBssid(u8 *bssid)
{
    MI_CpuCopy8(bssid, scanParam.bssid, WM_SIZE_MACADDR);
}

//-------------------------------------------------------------
// StartScanEx�p�p�����[�^�ݒ�֐�

// ScanEx�p maxChannelTime�ݒ�
void WM_lib_set_scanEx_maxChannelTime(u16 time)
{
    scanExParam.maxChannelTime = time;
    maxChannelTime_org = time;
}

// ScanEx�p maxChannelTime�擾
u16 WM_lib_get_scanEx_maxChannelTime(void)
{
    return scanExParam.maxChannelTime;
}

// ScanEx�p Channel�ݒ�
void WM_lib_set_scanEx_channelList(u16 channelList)
{
    // channel �́A�r�b�g�}�b�v�ł��邱�Ƃɒ���
    // 0x1041�Ȃ�A1,7,13ch
    WM_lib_set_scan_channel_seek(FALSE);
    scanExParam.channelList = channelList;
}

// ScanEx�p Channel�擾
u16 WM_lib_get_scanEx_channelList(void)
{
    return scanExParam.channelList;
}

// ScanEx�p ScanType(passive or active)�ݒ�
void WM_lib_set_scanEx_scanType(u16 scanType)
{
    scanExParam.scanType = scanType;
}

// ScanEx�p ScanType(passive or active)�擾
// WM_SCANTYPE_ACTIVE(0), WM_SCANTYPE_PASSIVE(1)
u16 WM_lib_get_scanEx_scanType(void)
{
    return scanExParam.scanType;
}

// ScanEx�p SSID�t�B���^�����O(SSID��)�ݒ�
void WM_lib_set_scanEx_ssidLength(u16 ssidLength)
{
    scanExParam.ssidLength = ssidLength;
}

// ScanEx�p SSID�t�B���^�����O�ɗp����SSID�ݒ�
void WM_lib_set_scanEx_ssid(u8 *ssid)
{
    if (ssid == NULL)
    {
        MI_CpuClear8(scanExParam.ssid, WM_SIZE_SSID);
    }
    else
    {
        MI_CpuCopy8(ssid, scanExParam.ssid, WM_SIZE_SSID);
    }
}

// ScanEx�p MacAddress(BSSID)�t�B���^�����O�ɗp����MacAddress�ݒ�
void WM_lib_set_scanEx_bssid(u8 *bssid)
{
    MI_CpuCopy8(bssid, scanExParam.bssid, WM_SIZE_MACADDR);
}


//--------------------------------------------------------
// WM�̏���������
int WM_lib_Init(WM_lib_param * para)
{
    int     errcode;
    param = para;

    param->mpStarted = 0;
    param->mpBusy = 0;
    param->child_bitmap = 0;
    param->currentTgid = 0;

    param->endReq = 0;
    WM_lib_mp_dcf_parallel = FALSE;

    switch (param->mode)
    {
    case WM_MODE_MP_PARENT:
        callback_ptr = WM_lib_mp_parent_callback;
        break;
    case WM_MODE_MP_CHILD:
        callback_ptr = WM_lib_mp_child_callback;
        break;
    case WM_MODE_INFRASTRUCTURE:
        callback_ptr = WM_lib_dcf_child_callback;
        break;
    default:
        callback_ptr = WM_lib_dummy;
        return WM_ERRCODE_FAILED;
    }

    do
    {
        errcode = WM_Initialize(param->wmBuf, callback_ptr, WM_DMA_NO);
    }
    while (errcode == WM_ERRCODE_WM_DISABLE);
    if (errcode != WM_ERRCODE_OPERATING)
    {
        return WM_ERRCODE_FAILED;
    }
    else
    {
        return WM_SetIndCallback(callback_ptr);
    }
}

//=================================================================
// �_�~�[�R�[���o�b�N
static void WM_lib_dummy(void *arg)
{
    WMCallback *buf = (WMCallback *)arg;
}

//=================================================================
// MP�e�@�̃R�[���o�b�N
void WM_lib_mp_parent_callback(void *arg)
{
    int     errcode;
    WMCallback *buf = (WMCallback *)arg;

    switch (buf->apiid)
    {
    case WM_APIID_START_PARENT:
        {
            WMstartParentCallback *callback = (WMstartParentCallback *)arg;

#ifdef	WMLIB_DEBUG
            OS_Printf("StartParent callback\n");
            OS_Printf("      state:%d\n", callback->state);
#endif

            if (WM_ERRCODE_SUCCESS == callback->errcode)
            {
                switch (callback->state)
                {
                case WM_STATECODE_PARENT_START:
                    param->callback(WM_TYPE_PARENT_STARTED, arg);
                    break;

                case WM_STATECODE_CONNECTED:
                    errcode = WM_ReadStatus(param->statusBuf);
                    check_WMAPI_errcode(WM_APIID_READ_STATUS, errcode);

                    // �X�e�[�^�X�ǂݏo��
                    param->child_bitmap = param->statusBuf->child_bitmap;

                    if (WM_lib_auto_flag == TRUE)
                    {
                        wm_lib_add_child_list(callback);
                    }

                    param->callback(WM_TYPE_CHILD_CONNECTED, arg);

                    if (param->statusBuf->mp_flag == 0) // MP�J�n�O�Ȃ�MP�J�n
                    {
                        errcode = WM_StartMPEx(WM_lib_mp_parent_callback, param->recvBuf, param->recvBufSize, param->sendBuf, param->sendBufSize, (u16)(param->contSend ? 0 : 1), 0, FALSE, FALSE, param->no_retry,     // �f�t�H���gFALSE
                                               param->contSend ? TRUE : FALSE   // ignoreFatalError
                            );

                        check_WMAPI_errcode(WM_APIID_START_MP, errcode);
                    }
                    // MP���M���R�[���o�b�N
                    if (TRUE == WM_lib_CheckMPSend())
                    {
                        param->callback(WM_TYPE_MP_SEND_ENABLE, NULL);
                    }
                    break;

                case WM_STATECODE_DISCONNECTED:
                    errcode = WM_ReadStatus(param->statusBuf);  // �X�e�[�^�X�ǂݏo��
                    check_WMAPI_errcode(WM_APIID_READ_STATUS, errcode);

                    param->child_bitmap = param->statusBuf->child_bitmap;

                    if (WM_lib_auto_flag == TRUE)
                    {
                        wm_lib_delete_child_list(callback);
                    }
                    param->callback(WM_TYPE_CHILD_DISCONNECTED, arg);
                    break;

                case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
                    // ����ؒf�����ꍇ�͏������s���܂���
                    break;

                case WM_STATECODE_BEACON_SENT: // �r�[�R�����M�����ʒm
                    param->callback(WM_TYPE_BEACON_SENT, arg);
                    break;

                default:
                    param->callback(WM_TYPE_ERROR, arg);
                    break;
                }
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

    case WM_APIID_SET_LIFETIME:
        {
            if (WM_ERRCODE_SUCCESS == buf->errcode)
            {
                param->callback(WM_TYPE_SET_LIFETIME, arg);

                // API�̔��s
                errcode = WM_SetParentParameter(WM_lib_mp_parent_callback, param->parentParam);
                check_WMAPI_errcode(WM_APIID_SET_P_PARAM, errcode);
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

    case WM_APIID_START_MP:
        {
            // �����ł̓G���[�R�[�h�͕Ԃ��Ă��Ȃ�
            WMstartMPCallback *callback = (WMstartMPCallback *)arg;

#ifdef	WMLIB_DEBUG
            OS_Printf("StartMP callback\n");
            OS_Printf("     state:%d\n", callback->state);
#endif
            switch (callback->state)
            {
            case WM_STATECODE_MP_START:
                param->mpStarted = 1;  // MP�J�n�ς݃t���O���Z�b�g
                param->callback(WM_TYPE_MP_STARTED, arg);

                if (param->keySharing == 1)
                {
                    // KeySharing�J�n
                    errcode = WM_StartKeySharing(param->ksBuf, WM_PORT_KEYSHARING);
                    check_WMAPI_errcode(WM_APIID_START_KS, errcode);
                    if (TRUE == WM_lib_CheckMPSend())
                        param->callback(WM_TYPE_MP_SEND_ENABLE, NULL);
                }
                else
                {
                    // MP���M���R�[���o�b�N
                    if (TRUE == WM_lib_CheckMPSend())
                    {
                        param->callback(WM_TYPE_MP_SEND_ENABLE, NULL);
                        if (WM_lib_mp_dcf_parallel == TRUE)
                        {
                            errcode =
                                WM_StartDCF(WM_lib_mp_parent_callback, param->dcfBuf,
                                            param->dcfBufSize);
                            check_WMAPI_errcode(WM_APIID_START_DCF, errcode);
                        }
                    }
                }
                break;

            case WM_STATECODE_MPEND_IND:
                // MP��M�R�[���o�b�N
                param->callback(WM_TYPE_MP_PARENT_RECV, (void *)(callback->recvBuf));
                break;

            default:
                param->callback(WM_TYPE_ERROR, arg);
                break;
            }
            break;
        }
        break;

    case WM_APIID_START_DCF:
        if (WM_ERRCODE_SUCCESS == buf->errcode)
        {
#if 0
#define WM_TYPE_DCF_STARTED
#define WM_TYPE_DCF_SENT
#define WM_TYPE_DCF_SENT_ERR
#define WM_TYPE_DCF_RECV
#define WM_TYPE_DCF_END
#endif
            param->callback(WM_TYPE_DCF_STARTED, arg);
        }
        else
        {
            param->callback(WM_TYPE_ERROR, arg);
        }
        break;

    case WM_APIID_SET_MP_DATA:
        {
            param->mpBusy = 0;

            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                param->callback(WM_TYPE_MP_PARENT_SENT, arg);
                param->callback(WM_TYPE_MP_SEND_ENABLE, NULL);  // ���̑��M������
            }
            else if (buf->errcode == WM_ERRCODE_SEND_QUEUE_FULL)
            {
                param->callback(WM_TYPE_SEND_QUEUE_FULL_ERR, arg);
            }
            else
            {
                param->callback(WM_TYPE_MP_PARENT_SENT_ERR, arg);
                param->callback(WM_TYPE_MP_SEND_ENABLE, NULL);  // ���̑��M������
            }
        }
        break;

    case WM_APIID_INITIALIZE:
        {
            if (WM_ERRCODE_SUCCESS == buf->errcode)
            {
                errcode = WM_ReadStatus(param->statusBuf);
                check_WMAPI_errcode(WM_APIID_READ_STATUS, errcode);
                // �X�e�[�^�X�ǂݏo��
                // API�̔��s
                errcode = WM_SetIndCallback(WM_lib_mp_parent_callback);
                check_WMAPI_errcode(WM_APIID_INDICATION, errcode);
#ifdef NO_LIFETIME
                errcode =
                    WM_SetLifeTime(WM_lib_mp_parent_callback, TABLE_NO, 0xffff, 0xffff, 0xffff);
#else
                errcode =
                    WM_SetLifeTime(WM_lib_mp_parent_callback, TABLE_NO, CAM_LIFE_TIME,
                                   FRAME_LIFE_TIME, CAM_LIFE_TIME);
#endif
                check_WMAPI_errcode(WM_APIID_SET_LIFETIME, errcode);
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

#if 1
    case WM_APIID_SET_P_PARAM:
        {
            param->callback(WM_TYPE_INIT_COMPLETE, arg);
            // �����ł̓G���[�R�[�h�͕Ԃ��Ă��Ȃ�
            errcode = WM_SetBeaconIndication(WM_lib_mp_parent_callback, 1 /* 1:ON, 0:OFF */ );
            check_WMAPI_errcode(WM_APIID_SET_BEACON_IND, errcode);
        }
        break;
        // Beacon Send/Recv Indication�𔭐������邩�ǂ���
    case WM_APIID_SET_BEACON_IND:
        {
            if (WM_ERRCODE_SUCCESS == buf->errcode)
            {
                errcode = WM_StartParent(WM_lib_mp_parent_callback);
                check_WMAPI_errcode(WM_APIID_START_PARENT, errcode);
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;
#else
    case WM_APIID_SET_P_PARAM:
        {
            param->callback(WM_TYPE_INIT_COMPLETE, arg);
            // �����ł̓G���[�R�[�h�͕Ԃ��Ă��Ȃ�
            errcode = WM_StartParent(WM_lib_mp_parent_callback);
            check_WMAPI_errcode(WM_APIID_START_PARENT, errcode);
        }
        break;
#endif

    case WM_APIID_RESET:
        {
#ifdef	WMLIB_DEBUG
            OS_Printf("Reset callback\n");
            OS_Printf("      errcode:%d\n", buf->errcode);
#endif
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                errcode = WM_End(WM_lib_mp_parent_callback);
                check_WMAPI_errcode(WM_APIID_END, errcode);
            }
            else
            {
                param->endReq = 0;
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

    case WM_APIID_END:
        {
            param->endReq = 0;
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                param->callback(WM_TYPE_END_COMPLETE, arg);
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

    case WM_APIID_DISCONNECT:
        {
            WMDisconnectCallback *cb = (WMDisconnectCallback *)buf;
#ifdef WMLIB_DEBUG
            OS_Printf("Disconnect callback\n");
            OS_Printf("    cb->errcode            = %d\n", cb->errcode);
            OS_Printf("    cb->wlCmdID            = %d\n", cb->wlCmdID);
            OS_Printf("    cb->wlResult           = %d\n", cb->wlResult);
            OS_Printf("    cb->tryBitmap          = %d\n", cb->tryBitmap);
            OS_Printf("    cb->disconnectedBitmap = %d\n", cb->disconnectedBitmap);
#endif
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                // �q�@�����X�V
                errcode = WM_ReadStatus(param->statusBuf);
                check_WMAPI_errcode(WM_APIID_READ_STATUS, errcode);
                param->child_bitmap = param->statusBuf->child_bitmap;

                if (WM_lib_auto_flag == TRUE)
                {
                    wm_lib_delete_child_list((WMstartParentCallback *)arg);
                }
                param->callback(WM_TYPE_DISCONNECT_COMPLETE, arg);
            }
            else
            {
                param->callback(WM_TYPE_DISCONNECT_FAILED, arg);
            }
        }
        break;

    case WM_APIID_SET_GAMEINFO:
        {
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                param->callback(WM_TYPE_SET_GAMEINFO_COMPLETE, arg);
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

    case WM_APIID_END_MP:
        {
            param->callback(WM_TYPE_END_MP_COMPLETE, arg);
        }
        break;

    case WM_APIID_INDICATION:
        {
            WMindCallback *cb = (WMindCallback *)arg;
            switch (cb->state)
            {
            case WM_STATECODE_BEACON_RECV:     // Beacon��Mindicate
                param->callback(WM_TYPE_BEACON_RECV, arg);
                break;
            case WM_STATECODE_DISASSOCIATE:    // �ڑ��ؒfindicate
                param->callback(WM_TYPE_DISASSOCIATE, arg);
                break;
            case WM_STATECODE_REASSOCIATE:     // �Đڑ�indicate
                param->callback(WM_TYPE_REASSOCIATE, arg);
                break;
            case WM_STATECODE_AUTHENTICATE:    // �F�؊m�Findicate
                param->callback(WM_TYPE_AUTHENTICATE, arg);
                break;
            }
        }
        break;

    default:
        param->callback(WM_TYPE_ERROR, arg);
        break;

    }
}

//=================================================================
// MP�q�@�̃R�[���o�b�N
void WM_lib_mp_child_callback(void *arg)
{
    WMCallback *buf = (WMCallback *)arg;
    int     errcode;

    switch (buf->apiid)
    {
        //---------------------------------------------------------------
    case WM_APIID_INITIALIZE:
        {
            if (WM_ERRCODE_SUCCESS == buf->errcode)
            {
#ifdef WMLIB_DEBUG
                OS_TPrintf("mp C: WM_Initialize callback\n");
#endif
                param->callback(WM_TYPE_INIT_COMPLETE, arg);
                {
                    errcode = WM_ReadStatus(param->statusBuf);  // �X�e�[�^�X�ǂݏo��
                    check_WMAPI_errcode(WM_APIID_READ_STATUS, errcode);

                    // API�̔��s
                    errcode = WM_SetIndCallback(WM_lib_mp_child_callback);
                    check_WMAPI_errcode(WM_APIID_INDICATION, errcode);
#ifdef NO_LIFETIME
                    errcode =
                        WM_SetLifeTime(WM_lib_mp_child_callback, TABLE_NO, 0xffff, 0xffff, 0xffff);
#else
                    errcode =
                        WM_SetLifeTime(WM_lib_mp_child_callback, TABLE_NO, CAM_LIFE_TIME,
                                       FRAME_LIFE_TIME, CAM_LIFE_TIME);
#endif
                    check_WMAPI_errcode(WM_APIID_SET_LIFETIME, errcode);
                }
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_SET_LIFETIME:
        {
#ifdef WMLIB_DEBUG
            OS_TPrintf("mp C: WM_SetLifeTime callback\n");
#endif
            if (WM_ERRCODE_SUCCESS == buf->errcode)
            {
                param->callback(WM_TYPE_SET_LIFETIME, arg);

                // WM_StartScan()���s
                if (param->scanExFlag == TRUE)
                {
                    // WM_StartScanEx�g�p��
                    scanExParam.scanBuf = param->parentInfoBuf;
                    scanExParam.scanBufSize = param->parentInfoBufSize;

                    if (scanExParam.maxChannelTime == 0)
                        WM_lib_set_scanEx_maxChannelTime(200);

                    scanExParam.bssid[0] = 0xff;
                    scanExParam.bssid[1] = 0xff;
                    scanExParam.bssid[2] = 0xff;
                    scanExParam.bssid[3] = 0xff;
                    scanExParam.bssid[4] = 0xff;
                    scanExParam.bssid[5] = 0xff;

                    scanning_flag = 1;
                    errcode = WM_StartScanEx(WM_lib_mp_child_callback, &scanExParam);
                    check_WMAPI_errcode(WM_APIID_START_SCAN_EX, errcode);
                }
                else
                {
                    // WM_StartScan�g�p��
                    scanParam.scanBuf = param->parentInfoBuf;

                    if (scanParam.channel == 0)
                        scanParam.channel = 1;
                    if (scanParam.maxChannelTime == 0)
                        WM_lib_set_max_scan_time(200);

                    scanParam.bssid[0] = 0xff;
                    scanParam.bssid[1] = 0xff;
                    scanParam.bssid[2] = 0xff;
                    scanParam.bssid[3] = 0xff;
                    scanParam.bssid[4] = 0xff;
                    scanParam.bssid[5] = 0xff;

                    scanning_flag = 1;
                    errcode = WM_StartScan(WM_lib_mp_child_callback, &scanParam);
                    check_WMAPI_errcode(WM_APIID_START_SCAN, errcode);
                }
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_START_SCAN:
        {
            WMstartScanCallback *callback = (WMstartScanCallback *)arg;

#ifdef WMLIB_DEBUG
            OS_TPrintf("mp C: WM_StartScan callback\n");
#endif
            if (WM_ERRCODE_SUCCESS == callback->errcode)
            {
                switch (callback->state)
                {
                case WM_STATECODE_SCAN_START:
                    break;

                case WM_STATECODE_PARENT_FOUND:
                    if (WM_lib_auto_flag)
                    {                  // wm_tool.c�̎��������g���ꍇ
                        if (WM_IsValidGameInfo
                            (&((WMstartScanCallback *)arg)->gameInfo,
                             ((WMstartScanCallback *)arg)->gameInfoLength))
                        {
                            wm_lib_add_parent_list((WMstartScanCallback *)arg);
                        }
                    }

                    param->callback(WM_TYPE_PARENT_FOUND, arg);
                    if (scanning_flag == 1)
                    {
                        if (WM_lib_change_scan_channel_flag == TRUE)
                            changeScanChannel(&scanParam);

                        errcode = WM_StartScan(WM_lib_mp_child_callback, &scanParam);
                        check_WMAPI_errcode(WM_APIID_START_SCAN, errcode);
                    }
                    break;

                case WM_STATECODE_PARENT_NOT_FOUND:
                    if (WM_lib_auto_flag)
                    {                  // wm_tool.c�̎��������g���ꍇ
                        wm_lib_delete_parent_list((WMstartScanCallback *)arg);
                    }
                    param->callback(WM_TYPE_PARENT_NOT_FOUND, arg);
                    if (scanning_flag == 1)
                    {
                        if (WM_lib_change_scan_channel_flag == TRUE)
                            changeScanChannel(&scanParam);

                        errcode = WM_StartScan(WM_lib_mp_child_callback, &scanParam);
                        check_WMAPI_errcode(WM_APIID_START_SCAN, errcode);
                    }
                    break;

                default:
                    param->callback(WM_TYPE_ERROR, arg);
                    break;
                }
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_START_SCAN_EX:
        {
            WMstartScanExCallback *callback = (WMstartScanExCallback *)arg;

#ifdef WMLIB_DEBUG
            OS_TPrintf("mp C: WM_StartScanEx callback\n");
#endif
            if (WM_ERRCODE_SUCCESS == callback->errcode)
            {
                switch (callback->state)
                {
                case WM_STATECODE_SCAN_START:
                    break;

                case WM_STATECODE_PARENT_FOUND:
                    if (WM_lib_auto_flag)
                    {                  // wm_tool.c�̎��������g���ꍇ
                        WMBssDesc *p_bssDesc;
                        u16     bssDescCount;
                        int     i;

                        bssDescCount = ((WMStartScanExCallback *)arg)->bssDescCount;
                        for (i = 0; i < bssDescCount; ++i)
                        {
                            p_bssDesc = ((WMStartScanExCallback *)arg)->bssDesc[i];

                            if (WM_IsValidGameInfo(&p_bssDesc->gameInfo, p_bssDesc->gameInfoLength))
                            {
                                wm_lib_add_parent_listEx((WMstartScanExCallback *)arg);
                            }
                        }
                    }

                    param->callback(WM_TYPE_PARENT_FOUND, arg);
                    if (scanning_flag == 1)
                    {
                        errcode = WM_StartScanEx(WM_lib_mp_child_callback, &scanExParam);
                        check_WMAPI_errcode(WM_APIID_START_SCAN_EX, errcode);
                    }
                    break;

                case WM_STATECODE_PARENT_NOT_FOUND:
                    param->callback(WM_TYPE_PARENT_NOT_FOUND, arg);
                    if (scanning_flag == 1)
                    {
                        errcode = WM_StartScanEx(WM_lib_mp_child_callback, &scanExParam);
                        check_WMAPI_errcode(WM_APIID_START_SCAN_EX, errcode);
                    }
                    break;

                default:
                    param->callback(WM_TYPE_ERROR, arg);
                    break;
                }
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_END_SCAN:
        {
            if (WM_ERRCODE_SUCCESS == buf->errcode)
            {
                errcode = WM_StartConnect(WM_lib_mp_child_callback, param->pInfo, test_ssid);
                //errcode = WM_StartConnect( WM_lib_mp_child_callback, param->pInfo, NULL );
                check_WMAPI_errcode(WM_APIID_START_CONNECT, errcode);
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_START_CONNECT:
        {
            WMstartConnectCallback *callback = (WMstartConnectCallback *)arg;

            if (callback->errcode == WM_ERRCODE_SUCCESS)
            {
                switch (callback->state)
                {
                case WM_STATECODE_CONNECT_START:
                    break;

                case WM_STATECODE_CONNECTED:
                    if (WM_lib_auto_flag == TRUE)
                    {
                        wm_lib_set_my_aid(callback->aid);
                    }
                    param->callback(WM_TYPE_CONNECTED_TO_PARENT, arg);
                    param->child_bitmap = 1;
#if 0
                    OS_Printf("WM_GetMPReceiveBufferSize:%d\n", WM_GetMPReceiveBufferSize());

                    OS_Printf("WM_GetMPSendBufferSize   :%d\n", WM_GetMPSendBufferSize());
#endif
                    errcode = WM_StartMPEx(WM_lib_mp_child_callback, param->recvBuf, param->recvBufSize, param->sendBuf, param->sendBufSize, (u16)(param->contSend ? 0 : 1), 0, FALSE, FALSE, param->no_retry,  // �f�t�H���gFALSE
                                           param->contSend ? TRUE : FALSE       // ignoreFatalError
                        );


                    check_WMAPI_errcode(WM_APIID_START_MP, errcode);
                    break;

                case WM_STATECODE_DISCONNECTED:
                    param->callback(WM_TYPE_DISCONNECTED_FROM_PARENT, arg);
                    param->child_bitmap = 0;
                    break;

                case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
                    // ����ؒf�����ꍇ�͏������s���܂���
                    break;

                case WM_STATECODE_BEACON_LOST:
                    param->callback(WM_TYPE_BEACON_LOST, arg);
                    break;

                default:
                    param->callback(WM_TYPE_ERROR, arg);
                    break;
                }
            }
            else
            {
                // �R�[���o�b�N�ŃG���[�ʒm
                if (WM_lib_auto_flag == TRUE)
                {
                    wm_lib_parent_found_count_reset();
                }
                param->callback(WM_TYPE_CONNECT_FAILED, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_START_MP:
        {
            WMstartMPCallback *callback = (WMstartMPCallback *)arg;

            switch (callback->state)
            {
            case WM_STATECODE_MP_START:
                param->mpStarted = 1;  // MP�J�n�ς݃t���O���Z�b�g
                param->callback(WM_TYPE_MP_STARTED, arg);

                if (param->keySharing == 1)
                {
                    // KeySharing�J�n
                    errcode = WM_StartKeySharing(param->ksBuf, WM_PORT_KEYSHARING);
                    check_WMAPI_errcode(WM_APIID_START_KS, errcode);
                    if (TRUE == WM_lib_CheckMPSend())
                        param->callback(WM_TYPE_MP_SEND_ENABLE, NULL);
                }
                else
                {
                    // MP���M���R�[���o�b�N
                    if (TRUE == WM_lib_CheckMPSend())
                    {
                        param->callback(WM_TYPE_MP_SEND_ENABLE, NULL);
                        if (WM_lib_mp_dcf_parallel == TRUE)
                        {
                            errcode =
                                WM_StartDCF(WM_lib_mp_child_callback, param->dcfBuf,
                                            param->dcfBufSize);
                            check_WMAPI_errcode(WM_APIID_START_DCF, errcode);
                        }
                    }
                }
                break;

            case WM_STATECODE_MP_IND:
                if (callback->errcode == WM_ERRCODE_INVALID_POLLBITMAP)
                {
                    // �����̓|�[������Ă��Ȃ����AMP�t���[���͎�M���Ă���̂ł��̂܂ܒʒm
                    param->callback(WM_TYPE_MP_CHILD_RECV, (void *)(callback->recvBuf));
                }
                else
                {
                    param->callback(WM_TYPE_MP_CHILD_RECV, (void *)(callback->recvBuf));
                }
                break;

            case WM_STATECODE_MPACK_IND:
                break;

            default:
                param->callback(WM_TYPE_ERROR, arg);
                break;
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_START_DCF:
        if (WM_ERRCODE_SUCCESS == buf->errcode)
        {
            param->callback(WM_TYPE_DCF_STARTED, arg);
        }
        else
        {
            param->callback(WM_TYPE_ERROR, arg);
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_SET_MP_DATA:
        {
            param->mpBusy = 0;

            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                param->callback(WM_TYPE_MP_CHILD_SENT, arg);
            }
            else if (buf->errcode == WM_ERRCODE_TIMEOUT)
            {
                param->callback(WM_TYPE_MP_CHILD_SENT_TIMEOUT, arg);
            }
            else
            {
                param->callback(WM_TYPE_MP_CHILD_SENT_ERR, arg);
            }

            // ���̑��M������
            param->callback(WM_TYPE_MP_SEND_ENABLE, NULL);
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_RESET:
        {
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                errcode = WM_End(WM_lib_mp_child_callback);
                check_WMAPI_errcode(WM_APIID_END, errcode);
            }
            else
            {
                param->endReq = 0;
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_END:
        {
            param->endReq = 0;
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                param->callback(WM_TYPE_END_COMPLETE, arg);
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_INDICATION:
        {
            WMindCallback *cb = (WMindCallback *)arg;
            switch (cb->state)
            {
            case WM_STATECODE_BEACON_RECV:     // Beacon��Mindicate
                {
                    static u16 flag = 0;        // 2�񑱂��Ă����ɗ����Ƃ��AWM_lib_End�̓�d���s��h��
                    WMBeaconRecvIndCallback *callback = (WMBeaconRecvIndCallback *)arg;
                    if ((param->currentTgid != 0) && (callback->gameInfo.tgid != param->currentTgid)
                        && (callback->wmstate == WM_STATE_MP_CHILD))
                    {
                        if (flag == 0)
                        {
                            int     errcode;
                            flag = 1;
                            errcode = WM_lib_End();
                            check_WMAPI_errcode(WM_APIID_END, errcode);
                        }
                    }
                    else
                    {
                        flag = 0;
                    }
                    param->callback(WM_TYPE_BEACON_RECV, arg);
                }
                break;
            case WM_STATECODE_DISASSOCIATE:    // �ڑ��ؒfindicate
                param->callback(WM_TYPE_DISASSOCIATE, arg);
                break;
            case WM_STATECODE_REASSOCIATE:     // �Đڑ�indicate
                param->callback(WM_TYPE_REASSOCIATE, arg);
                break;
            case WM_STATECODE_AUTHENTICATE:    // �F�؊m�Findicate
                param->callback(WM_TYPE_AUTHENTICATE, arg);
                break;
            }
        }
        break;

    default:
        param->callback(WM_TYPE_ERROR, arg);
        break;
    }
}

//========================================================================================

//=================================================================
// DCF�q�@�̃R�[���o�b�N
void WM_lib_dcf_child_callback(void *arg)
{
    WMCallback *buf = (WMCallback *)arg;
    int     errcode;

    switch (buf->apiid)
    {
        //---------------------------------------------------------------
    case WM_APIID_INITIALIZE:
        {
            if (WM_ERRCODE_SUCCESS == buf->errcode)
            {
#ifdef WMLIB_DEBUG
                OS_TPrintf("dcf: WM_Initialize callback\n");
#endif
                param->callback(WM_TYPE_INIT_COMPLETE, arg);

                errcode =
                    WM_SetLifeTime(WM_lib_dcf_child_callback, TABLE_NO, 0xffff, 0xffff, 0xffff);
                check_WMAPI_errcode(WM_APIID_SET_LIFETIME, errcode);
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_SET_LIFETIME:
        {
            if (WM_ERRCODE_SUCCESS == buf->errcode)
            {
#ifdef WMLIB_DEBUG
                OS_TPrintf("dcf: WM_SetLifeTime callback\n");
#endif

                errcode = WM_SetWEPKeyEx(WM_lib_dcf_child_callback, param->wepMode,
                                         param->wepKeyId, param->wepKey);
                check_WMAPI_errcode(WM_APIID_SET_WEPKEY_EX, errcode);
            }
            else
            {
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_SET_WEPKEY:
    case WM_APIID_SET_WEPKEY_EX:
        {
#ifdef WMLIB_DEBUG
            OS_TPrintf("dcf: WM_SetWEPKey(Ex) callback\n");
#endif
            if (WM_ERRCODE_SUCCESS == buf->errcode)
            {
                // �X�e�[�^�X�ǂݏo��
                errcode = WM_ReadStatus(param->statusBuf);
                check_WMAPI_errcode(WM_APIID_READ_STATUS, errcode);

                // WM_SetIncCallback���s
                errcode = WM_SetIndCallback(WM_lib_dcf_child_callback);
                check_WMAPI_errcode(WM_APIID_INDICATION, errcode);

                // Scan�J�n
                if (param->scanExFlag == TRUE)
                {
                    // WM_StartScanEx�g�p��
                    scanExParam.scanBuf = param->parentInfoBuf;
                    scanExParam.scanBufSize = param->parentInfoBufSize;

                    if (scanExParam.maxChannelTime == 0)
                        WM_lib_set_scanEx_maxChannelTime(200);

                    scanExParam.bssid[0] = 0xff;
                    scanExParam.bssid[1] = 0xff;
                    scanExParam.bssid[2] = 0xff;
                    scanExParam.bssid[3] = 0xff;
                    scanExParam.bssid[4] = 0xff;
                    scanExParam.bssid[5] = 0xff;

                    scanning_flag = 1;

#ifdef WMLIB_DEBUG
                    OS_TPrintf("dcf: WM_StartScanEx\n");
#endif
                    // WM_StartScanEx���s
                    errcode = WM_StartScanEx(WM_lib_dcf_child_callback, &scanExParam);
                    check_WMAPI_errcode(WM_APIID_START_SCAN_EX, errcode);
                }
                else
                {
                    // WM_StartScan�g�p��
                    scanParam.scanBuf = param->parentInfoBuf;

                    if (scanParam.channel == 0)
                        scanParam.channel = 1;
                    if (scanParam.maxChannelTime == 0)
                        WM_lib_set_max_scan_time(200);

                    scanExParam.bssid[0] = 0xff;
                    scanExParam.bssid[1] = 0xff;
                    scanExParam.bssid[2] = 0xff;
                    scanExParam.bssid[3] = 0xff;
                    scanExParam.bssid[4] = 0xff;
                    scanExParam.bssid[5] = 0xff;

                    scanning_flag = 1;

#ifdef WMLIB_DEBUG
                    OS_TPrintf("dcf: WMi_StartScan\n");
#endif
                    // WM_StartScan���s
                    errcode = WM_StartScan(WM_lib_dcf_child_callback, &scanParam);
                    check_WMAPI_errcode(WM_APIID_START_SCAN, errcode);
                }
            }
        }
        break;


        //---------------------------------------------------------------
    case WM_APIID_START_SCAN:
        {
            WMstartScanCallback *callback = (WMstartScanCallback *)arg;

#ifdef WMLIB_DEBUG
            OS_TPrintf("dcf: WMi_StartScan callback ");
#endif
            if (WM_ERRCODE_SUCCESS == callback->errcode)
            {
                switch (callback->state)
                {
                case WM_STATECODE_SCAN_START:
#ifdef WMLIB_DEBUG
                    OS_TPrintf("start\n");
#endif
                    break;

                case WM_STATECODE_PARENT_FOUND:
#ifdef WMLIB_DEBUG
                    OS_TPrintf("found\n");
#endif
                    param->callback(WM_TYPE_PARENT_FOUND, arg);
                    if (scanning_flag == 1)
                    {
                        if (WM_lib_change_scan_channel_flag == TRUE)
                            changeScanChannel(&scanParam);

                        errcode = WM_StartScan(WM_lib_dcf_child_callback, &scanParam);

                        check_WMAPI_errcode(WM_APIID_START_SCAN, errcode);
                    }
                    break;

                case WM_STATECODE_PARENT_NOT_FOUND:
#ifdef WMLIB_DEBUG
                    OS_TPrintf("not found\n");
#endif
                    param->callback(WM_TYPE_PARENT_NOT_FOUND, arg);
                    if (scanning_flag == 1)
                    {
                        if (WM_lib_change_scan_channel_flag == TRUE)
                            changeScanChannel(&scanParam);

                        errcode = WM_StartScan(WM_lib_dcf_child_callback, &scanParam);

                        check_WMAPI_errcode(WM_APIID_START_SCAN, errcode);
                    }
                    break;

                default:
#ifdef WMLIB_DEBUG
                    OS_TPrintf("error\n");
#endif
                    param->callback(WM_TYPE_ERROR, arg);
                    break;
                }
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_START_SCAN_EX:
        {
            WMstartScanExCallback *callback = (WMstartScanExCallback *)arg;

#ifdef WMLIB_DEBUG
            OS_TPrintf("dcf: WMi_StartScanEx callback ");
#endif
            if (WM_ERRCODE_SUCCESS == callback->errcode)
            {
                switch (callback->state)
                {
                case WM_STATECODE_SCAN_START:
#ifdef WMLIB_DEBUG
                    OS_TPrintf("start\n");
#endif
                    break;

                case WM_STATECODE_PARENT_FOUND:
#ifdef WMLIB_DEBUG
                    OS_TPrintf("found\n");
#endif
                    param->callback(WM_TYPE_PARENT_FOUND, arg);
                    if (scanning_flag == 1)
                    {
                        errcode = WM_StartScanEx(WM_lib_dcf_child_callback, &scanExParam);
                        check_WMAPI_errcode(WM_APIID_START_SCAN_EX, errcode);
                    }
                    break;

                case WM_STATECODE_PARENT_NOT_FOUND:
#ifdef WMLIB_DEBUG
                    OS_TPrintf("not found\n");
#endif
                    param->callback(WM_TYPE_PARENT_NOT_FOUND, arg);
                    if (scanning_flag == 1)
                    {
                        errcode = WM_StartScanEx(WM_lib_dcf_child_callback, &scanExParam);
                        check_WMAPI_errcode(WM_APIID_START_SCAN_EX, errcode);
                    }
                    break;

                default:
#ifdef WMLIB_DEBUG
                    OS_TPrintf("error\n");
#endif
                    param->callback(WM_TYPE_ERROR, arg);
                    break;
                }
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_END_SCAN:
        {
            switch (buf->errcode)
            {
            case WM_ERRCODE_SUCCESS:
#ifdef STARTCONNECT_EX_TEST
                errcode = WM_StartConnectEx(WM_lib_dcf_child_callback, param->pInfo, NULL,
                                            FALSE, param->authMode);
#else
                errcode = WM_StartConnect(WM_lib_dcf_child_callback, param->pInfo, NULL);
#endif
                check_WMAPI_errcode(WM_APIID_START_CONNECT, errcode);
                break;

            default:
                param->callback(WM_TYPE_ERROR, arg);
                break;
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_START_CONNECT:
        {
            WMstartConnectCallback *callback = (WMstartConnectCallback *)arg;

            if (callback->errcode == WM_ERRCODE_SUCCESS)
            {
                switch (callback->state)
                {
                case WM_STATECODE_CONNECT_START:
                    break;

                case WM_STATECODE_CONNECTED:
                    param->callback(WM_TYPE_CONNECTED_TO_PARENT, arg);
                    errcode =
                        WM_StartDCF(WM_lib_dcf_child_callback, param->dcfBuf, param->dcfBufSize);
                    check_WMAPI_errcode(WM_APIID_START_DCF, errcode);
                    break;

                case WM_STATECODE_DISCONNECTED:
                    param->callback(WM_TYPE_DISCONNECTED_FROM_PARENT, arg);
                    break;

                case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
                    // ����ؒf�����ꍇ�͏������s���܂���
                    break;

                default:
                    param->callback(WM_TYPE_ERROR, arg);
                    break;
                }
            }
            else
            {
                // �R�[���o�b�N�ŃG���[�ʒm
                param->callback(WM_TYPE_CONNECT_FAILED, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_START_DCF:
        {
            WMstartDCFCallback *callback = (WMstartDCFCallback *)arg;

            if (callback->errcode == WM_ERRCODE_SUCCESS)
            {
                switch (callback->state)
                {
                case WM_STATECODE_DCF_START:
                    break;

                case WM_STATECODE_DCF_IND:
                    param->callback(WM_TYPE_DCF_CHILD_RECV, callback->recvBuf);
                    break;

                default:
                    param->callback(WM_TYPE_ERROR, arg);
                    break;
                }
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_RESET:
        {
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                errcode = WM_End(WM_lib_dcf_child_callback);
                check_WMAPI_errcode(WM_APIID_END, errcode);
            }
            else
            {
                param->endReq = 0;
                param->callback(WM_TYPE_ERROR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_END:
        {
            param->endReq = 0;
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                param->callback(WM_TYPE_END_COMPLETE, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_SET_DCF_DATA:
        {
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                param->callback(WM_TYPE_DCF_CHILD_SENT, arg);
            }
            else
            {
                param->callback(WM_TYPE_DCF_CHILD_SENT_ERR, arg);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_INDICATION:
        {
        }
        break;

    default:
        param->callback(WM_TYPE_ERROR, arg);
        break;
    }
}




//========================================================================================

//--------------------------------------------------------
// WM_lib �I���v��
int WM_lib_End(void)
{
    int     errcode;

    if (param == NULL)
        return WM_ERRCODE_FAILED;

    /* End�v�������ɏo����Ă���ꍇ�́A�������Ȃ��D */
    if (param->endReq == 0)
    {
        scanning_flag = 0;
        param->endReq = 1;
        errcode = WM_Reset(callback_ptr);
        return conv_errcode(errcode);
    }

    return WM_ERRCODE_FAILED;

}

//--------------------------------------------------------
// WM_lib MP�f�[�^���M�v��
int WM_lib_SetMPDataEX(u16 *data, u16 size, u16 pollbitmap)
{
    int     errcode;

    if (param == NULL)
        return WM_ERRCODE_FAILED;

    // MP�J�n�O�y�сAMP���M�v�����y�сA�I���v�����s���MP�f�[�^�̑��M�͂ł��Ȃ�
    if ((param->mpStarted == 0) || (param->mpBusy == 1) || (param->endReq == 1))
        return WM_ERRCODE_FAILED;

    // MP�f�[�^���M�v�����t���O�𗧂Ă�
    param->mpBusy = 1;

    switch (param->mode)
    {
    case WM_MODE_MP_PARENT:
        if (0 == param->contSend)
        {
            errcode = WM_SetMPData(WM_lib_mp_parent_callback, data, size, 1000, pollbitmap);
        }
        else
        {
            errcode = WM_SetMPData(WM_lib_mp_parent_callback, data, size, 0, pollbitmap);
        }
        if (errcode == WM_ERRCODE_OPERATING)
        {
        }

        if (errcode != WM_ERRCODE_SUCCESS)
            param->mpBusy = 0;
        return conv_errcode(errcode);

    case WM_MODE_MP_CHILD:
        errcode = WM_SetMPData(WM_lib_mp_child_callback, data, size, 0, pollbitmap);
        if (errcode == WM_ERRCODE_OPERATING)
        {
        }
        if (errcode != WM_ERRCODE_SUCCESS)
            param->mpBusy = 0;
        return conv_errcode(errcode);

    default:
        return WM_ERRCODE_FAILED;
    }
}

int WM_lib_SetMPData(u16 *data, u16 size)
{
    return WM_lib_SetMPDataEX(data, size, 0xffff);
}


//--------------------------------------------------------
// WM_lib DCF�f�[�^���M�v��
int WM_lib_SetDCFData(const u8 *destAdr, u16 *data, u16 size)
{
    int     errcode;
    errcode = WM_SetDCFData(WM_lib_dcf_child_callback, destAdr, data, size);
    return conv_errcode(errcode);
}



//--------------------------------------------------------
// WM_lib �ڑ��v��
int WM_lib_ConnectToParent(WMbssDesc *pInfo)
{
    int     errcode;
    WMGameInfo *pGameInfo = (WMGameInfo *)&(pInfo->gameInfo);

    if (param == NULL)
        return WM_ERRCODE_FAILED;

    param->pInfo = pInfo;
    param->currentTgid = pGameInfo->tgid;

    scanning_flag = 0;

    errcode = WM_EndScan(callback_ptr);
    return conv_errcode(errcode);
}

//--------------------------------------------------------
// WM_lib �ؒf�v��
int WM_lib_Disconnect(u16 aid)
{
    int     errcode;

    if (param == NULL)
        return WM_ERRCODE_FAILED;

    errcode = WM_Disconnect(callback_ptr, aid);
    if (errcode != WM_ERRCODE_OPERATING)
        param->callback(WM_TYPE_DISCONNECT_FAILED, 0);
    return conv_errcode(errcode);
}

//--------------------------------------------------------
// WM_lib GameInfo�ݒ�v��
int WM_lib_SetGameInfo(u16 *userGameInfo, u16 size, u32 ggid, u16 tgid)
{
    int     errcode;
    u8      attribute;
    if (param == NULL)
        return WM_ERRCODE_FAILED;

    param->parentParam->userGameInfo = userGameInfo;
    param->parentParam->userGameInfoLength = size;
    param->parentParam->ggid = ggid;
    param->parentParam->tgid = tgid;

    attribute = (u8)(((param->parentParam->entryFlag) ? WM_ATTR_FLAG_ENTRY : 0) |       // entryFlag �ŉ��ʃr�b�g
                     ((param->parentParam->multiBootFlag) ? WM_ATTR_FLAG_MB : 0) |      // multiBootFlag ��2�r�b�g
                     ((param->parentParam->KS_Flag) ? WM_ATTR_FLAG_KS : 0) |    // KS_Flag ��3�r�b�g
                     ((param->parentParam->CS_Flag) ? WM_ATTR_FLAG_CS : 0)      // CS_Flag ��4�r�b�g
        );


    // ���́AuserGameInfo�����łȂ��AparentParam���̂��X�V���Ă���
//    errcode = WM_SetGameInfo( callback_ptr, param->parentParam );    
    errcode = WM_SetGameInfo(callback_ptr,
                             param->parentParam->userGameInfo,
                             param->parentParam->userGameInfoLength,
                             param->parentParam->ggid, param->parentParam->tgid, attribute);

    if (errcode != WM_ERRCODE_OPERATING)
        param->callback(WM_TYPE_SET_GAMEINFO_FAILED, 0);
    return conv_errcode(errcode);
}


//--------------------------------------------------------
// WM_lib MP���M���̃`�F�b�N
BOOL WM_lib_CheckMPSend(void)
{
    if (param == NULL)
        return WM_ERRCODE_FAILED;

    if ((param->mpStarted == 1) && (param->mpBusy == 0) &&
        (param->endReq == 0) && (param->child_bitmap != 0))
        return TRUE;
    else
        return FALSE;
}

//--------------------------------------------------------
// WM_lib �L�[�Z�b�g�擾�֐�
int WM_lib_GetKeySet(WMkeySet *keySet)
{
    if (TRUE == WM_lib_CheckMPSend())
        return WM_GetKeySet(param->ksBuf, keySet);
    else
        return 0xffff;
}

//--------------------------------------------------------
u16 WM_lib_CurrentScanChannel(void)
{
    return scanParam.channel;
}


//--------------------------------------------------------
// ScanChannel�ύX�֐�
static void changeScanChannel(WMscanParam *param)
{
    switch (param->channel)
    {
    case 1:
        param->channel = 7;
        break;

    case 7:
        param->channel = 13;
        break;

    case 13:
        param->channel = 1;
        break;

    default:
        param->channel = 1;
        break;
    }
}


void WM_lib_set_scan_channel_seek(BOOL flag)
{
    WM_lib_change_scan_channel_flag = flag;
}


void WM_lib_set_bssid(u16 bssid0, u16 bssid1, u16 bssid2)
{
    /* �����̕ύX���� */
    scanParam.bssid[0] = (u8)(bssid0 & 0x00ff);
    scanParam.bssid[1] = (u8)(bssid0 >> 8);
    scanParam.bssid[2] = (u8)(bssid1 & 0x00ff);
    scanParam.bssid[3] = (u8)(bssid1 >> 8);
    scanParam.bssid[4] = (u8)(bssid2 & 0x00ff);
    scanParam.bssid[5] = (u8)(bssid2 >> 8);
}

void   *WM_lib_get_mp_parent_callback_ptr(void)
{
    return &WM_lib_mp_parent_callback;
}

void   *WM_lib_get_mp_child_callback_ptr(void)
{
    return &WM_lib_mp_child_callback;
}

//-----------------------------------------------------------------------------------
// rate_rssi��4�i�K��RSSI�l�ɕϊ�����
u16 WM_lib_CalcRSSI(u16 rate_rssi, u16 aid)
{
    u16     rssi;
    u16     i;
    static u16 bufCounter[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static u16 rssiBuf[16][16] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    rssi = (u16)(rate_rssi >> 8);
//��������������������������������������������������
//�� RSSI(���6bit)                   ��LNA ��rsv ��
//��������������������������������������������������
    rssi &= ~0x0001;                   // rsv��O�̂��߃N���A
    if (rssi & 0x0002)
    {
        rssi >>= 2;
    }
    else
    {
        rssi >>= 2;
        rssi += 25;
    }

    // �ŋ�16��̕��ϒl���v�Z
    rssiBuf[aid][bufCounter[aid]] = rssi;
    if (++bufCounter[aid] == 16)
        bufCounter[aid] = 0;

    rssi = 0;
    for (i = 0; i < 16; ++i)
        rssi += rssiBuf[aid][i];

    rssi >>= 4;

    // RSSI�l��4�i�K�ɂ��ĕԂ�
    if (rssi < 8)
        return 0;
    else if (rssi < 20)
        return 1;
    else if (rssi < 36)
        return 2;
    else
        return 3;
}


// WM_API�̖߂�l�`�F�b�N
static void check_WMAPI_errcode(u16 apiid, int errcode)
{
    u16     arg[2];

    if (errcode != WM_ERRCODE_OPERATING && errcode != WM_ERRCODE_SUCCESS)
    {
        arg[0] = apiid;
        arg[1] = (u16)errcode;
        param->callback(WM_TYPE_API_ERROR, arg);
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

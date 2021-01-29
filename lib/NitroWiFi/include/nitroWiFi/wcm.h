/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - include
  File:     wcm.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm.h,v $
  Revision 1.13  2006/03/10 09:20:22  kitase_hirotake
  INDENT SOURCE

  Revision 1.12  2005/09/17 10:08:19  terui
  WCM_CAM_LIFETIME�萔��ǉ�

  Revision 1.11  2005/09/12 10:00:27  terui
  WCM_CAMOUFLAGE_RATESET�萔��ǉ�

  Revision 1.10  2005/09/10 04:09:21  terui
  ���[�e�B���e�B�֐��Q��inline������֐��ɕύX���A�ʃt�@�C���ɐ؂�o��

  Revision 1.9  2005/09/09 00:55:39  terui
  �����l�b�g���[�N�̓����𒲍����� inline �֐���ǉ�

  Revision 1.8  2005/09/01 13:03:01  terui
  Keep Alive �p�ɕK�v�ȃo�b�t�@��ǉ��������Ƃɔ����AWCM_WORK_SIZE ���g��

  Revision 1.7  2005/08/08 06:13:42  terui
  AP�ɐڑ�����ۂ̃T�|�[�g���[�g�Z�b�g��1M/2M���Œ�Œǉ�����悤����

  Revision 1.6  2005/07/19 13:04:08  terui
  WCM_APLIST_SIZE �� 64 ����@192 �Ɋg��

  Revision 1.5  2005/07/18 02:27:55  terui
  Fix comment.

  Revision 1.4  2005/07/15 11:33:04  terui
  WCM_BeginSearchAsync �֐��AWCM_EndSearchAsync �֐���`�̒ǉ�

  Revision 1.3  2005/07/12 06:25:52  terui
  WCM_SetChannelScanTime �֐���ǉ�

  Revision 1.2  2005/07/11 12:04:48  terui
  WCMWepDesc �\���̒�`���̖��p�ȃ����o���폜�B

  Revision 1.1  2005/07/07 10:40:44  terui
  �V�K�ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITROWIFI_WCM_H_
#define NITROWIFI_WCM_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include <nitro/types.h>
#include <nitro/wm.h>

/*---------------------------------------------------------------------------*
    �萔 ��`
 *---------------------------------------------------------------------------*/

// WCM ���C�u�������K�v�Ƃ��郏�[�N�o�b�t�@�̃T�C�Y
#define WCM_WORK_SIZE   8960

// �������ʃR�[�h
#define WCM_RESULT_SUCCESS          0               // ����
#define WCM_RESULT_FAILURE          1               // ���s
#define WCM_RESULT_PROGRESS         2               // ���ɓ����񓯊����������s��
#define WCM_RESULT_ACCEPT           3               // �񓯊������v�������( �K���R�[���o�b�N������ )
#define WCM_RESULT_REJECT           4               // �񓯊������v��������( ARM7 �ւ̗v���`�B�Ɏ��s )
#define WCM_RESULT_WMDISABLE        5               // ARM7 ���� WM ���C�u���������삵�Ă��Ȃ�
#define WCM_RESULT_NOT_ENOUGH_MEM   6               // ���[�N�o�b�t�@�̃T�C�Y������Ȃ�
#define WCM_RESULT_FATAL_ERROR      7               // �����s�\�ȃG���[

// ���C�u���������t�F�[�Y
#define WCM_PHASE_NULL              0               // �������O
#define WCM_PHASE_WAIT              1               // ����������̏��( �v���҂� )
#define WCM_PHASE_WAIT_TO_IDLE      2               // ����������̏�� ���� �����@�\�̋N���V�[�P���X��
#define WCM_PHASE_IDLE              3               // �����@�\�A�C�h�����
#define WCM_PHASE_IDLE_TO_WAIT      4               // �A�C�h����� ���� �����@�\�̒�~�V�[�P���X��
#define WCM_PHASE_IDLE_TO_SEARCH    5               // �A�C�h����� ���� AP �����T����Ԃւ̈ڍs�V�[�P���X��
#define WCM_PHASE_SEARCH            6               // AP �����T�����
#define WCM_PHASE_SEARCH_TO_IDLE    7               // AP �����T����� ���� �A�C�h����Ԃւ̈ڍs�V�[�P���X��
#define WCM_PHASE_IDLE_TO_DCF       8               // �A�C�h����� ���� AP �ւ̖����ڑ��V�[�P���X��
#define WCM_PHASE_DCF               9               // AP �Ɩ����ڑ����ꂽ DCF �ʐM�\���
#define WCM_PHASE_DCF_TO_IDLE       10              // DCF �ʐM��� ���� �����ڑ���ؒf����V�[�P���X��
#define WCM_PHASE_FATAL_ERROR       11              // �����s�\�ȃG���[���������A�S�Ă̏������󂯕t�����Ȃ����
#define WCM_PHASE_IRREGULAR         12              // ��ԑJ�ڃV�[�P���X�̓r���Ŗ�肪�����������
#define WCM_PHASE_TERMINATING       13              // WCM ���C�u�����̋�����~�V�[�P���X��

// �ʒm���
#define WCM_NOTIFY_COMMON           0               // �f�t�H���g�̒ʒm��ʁB
#define WCM_NOTIFY_STARTUP          1               // WCM_StartupAsync �֐��̌��ʒʒm���
#define WCM_NOTIFY_CLEANUP          2               // WCM_CleanupAsync �֐��̌��ʒʒm���
#define WCM_NOTIFY_BEGIN_SEARCH     3               // WCM_SearchAsync �֐��AWCM_BeginSearchAsync �֐��ɂ��T���J�n�����̌��ʒʒm���
#define WCM_NOTIFY_END_SEARCH       4               // WCM_SearchAsync �֐��AWCM_EndSearchAsync �֐��ɂ��T����~�����̌��ʒʒm���
#define WCM_NOTIFY_CONNECT          5               // WCM_ConnectAsync �֐��̌��ʒʒm���
#define WCM_NOTIFY_DISCONNECT       6               // WCM_DisconnectAsync �֐��̌��ʒʒm��ʁEAP ����̐ؒf�ʒm���̎��
#define WCM_NOTIFY_FOUND_AP         7               // �����T���������� AP �������̒ʒm���
#define WCM_NOTIFY_SEARCH_AROUND    8               // �����T���������ꏄ�������̒ʒm��� ( �I�v�V�����ݒ莞 )
#define WCM_NOTIFY_TERMINATE        9               // WCM_TerminateAsync �֐��̌��ʒʒm���

// AP �����T�����ʕێ��o�b�t�@�̃��b�N���
#define WCM_APLIST_UNLOCK   0                       // �J�� - �������ꂽ AP �������Ɏ����I�Ƀ��X�g���ҏW�����
#define WCM_APLIST_LOCK     1                       // �{�� - �������ꂽ AP ���̓��X�g�ɔ��f����Ȃ�

// AP �����T�����ʕێ��o�b�t�@�̖��t��������
#define WCM_APLIST_MODE_IGNORE      0               // ���� - �������ꂽ AP ���͔j�������
#define WCM_APLIST_MODE_EXCHANGE    1               // ���� - �ł��Â� AP ���Ɠ���ւ�����

// WEP �Í������[�h���
#define WCM_WEPMODE_NONE    0                       // �Í����Ȃ�
#define WCM_WEPMODE_40      1                       // RC4 (  40 �r�b�g ) �Í������[�h
#define WCM_WEPMODE_104     2                       // RC4 ( 104 �r�b�g ) �Í������[�h
#define WCM_WEPMODE_128     3                       // RC4 ( 128 �r�b�g ) �Í������[�h

// �e�퐧��p�萔
#define WCM_CAM_LIFETIME            80              // CAM ���C�t�^�C��( 8 �b )
#define WCM_BSSID_SIZE              WM_SIZE_BSSID   // BSS-ID �̃T�C�Y ( 6 �o�C�g )
#define WCM_ESSID_SIZE              WM_SIZE_SSID    // ESS-ID �̍ő�T�C�Y ( 32 �o�C�g )
#define WCM_APLIST_SIZE             192             // AP ���u���b�N�̃f�[�^�̈�T�C�Y
#define WCM_APLIST_EX_SIZE          16              // AP ���u���b�N�̐���̈�T�C�Y
#define WCM_APLIST_BLOCK_SIZE       (WCM_APLIST_EX_SIZE + WCM_APLIST_SIZE)
#define WCM_WEP_SIZE                WM_SIZE_WEPKEY  // 80 �o�C�g
#define WCM_WEP_EX_SIZE             (1 + 1 + 14)    // WCMWepDesc.mode + .keyId + .reserved
#define WCM_WEP_STRUCT_SIZE         (WCM_WEP_SIZE + WCM_WEP_EX_SIZE)    // == sizeof( WCMWepDesc )
#define WCM_DCF_RECV_EXCESS_SIZE    (sizeof(WMDcfRecvBuf) - 4)          // 44 �o�C�g
#define WCM_DCF_SEND_EXCESS_SIZE    0   // ���M�o�b�t�@�Ƀw�b�_�i�[�p�̈�͕s�v
#define WCM_DCF_RECV_SIZE           (WM_DCF_MAX_SIZE + WCM_DCF_RECV_EXCESS_SIZE)
#define WCM_DCF_RECV_BUF_SIZE       (WCM_ROUNDUP32(WCM_DCF_RECV_SIZE))  // ���� DCF ��M�ɕK�v�ȃo�b�t�@�T�C�Y
#define WCM_DCF_SEND_BUF_SIZE       (WCM_ROUNDUP32(WM_DCF_MAX_SIZE))    // ���� DCF ���M�ɕK�v�ȃo�b�t�@�T�C�Y
#define WCM_ETC_BUF_SIZE            108     // �e�퐧��p�ϐ��Q�̃T�C�Y
#define WCM_ADDITIONAL_RATESET      0x0003  // �T�|�[�g���[�g�Z�b�g�ɌŒ�Œǉ�( 1M or 2M )
#define WCM_CAMOUFLAGE_RATESET      0x0024  // �T�|�[�g���[�g�Z�b�g�ɋU���ǉ�( 5.5M and 11M )

/*
                                     * �T�|�[�g���[�g�Z�b�g�� 16 �r�b�g�̃r�b�g�t�B�[���h�Ŏw�肷��`�ł���B
                                     * �e�r�b�g�ƃ��[�g�̑Ή��A�y�ю��ۂɃG�������g�Ɋi�[����鐔�l�̑Ή��͈ȉ��̒ʂ�B
                                     *
                                     *   1M   2M 5.5M   6M   9M  11M  12M  18M  24M  36M  48M  54M
                                     * {  2,   4,  11,  12,  18,  22,  24,  36,  48,  72,  96, 108, 0, 0, 0, 0 }
                                     */
#define WCM_AID_MIN 1                       // �ŏ��� Auth-ID
#define WCM_AID_MAX 2007                    // �ő�� Auth-ID
extern const u8 WCM_Bssid_Any[WCM_BSSID_SIZE];
extern const u8 WCM_Essid_Any[WCM_ESSID_SIZE];

// �C�ӂ� BSS-ID �������萔
#define WCM_BSSID_ANY   ((void*)WCM_Bssid_Any)

// �C�ӂ� ESS-ID �������萔
#define WCM_ESSID_ANY   ((void*)WCM_Essid_Any)

/*-------- �I�v�V�����p�r�b�g�z�u��` --------*/

// �X�L�������̃`�����l��
#define WCM_OPTION_TEST_CHANNEL     0x00008000
#define WCM_OPTION_FILTER_CHANNEL   0x00003ffe
#define WCM_OPTION_MASK_CHANNEL     (WCM_OPTION_TEST_CHANNEL | WCM_OPTION_FILTER_CHANNEL)
#define WCM_OPTION_CHANNEL_1        (WCM_OPTION_TEST_CHANNEL | 0x00000002)
#define WCM_OPTION_CHANNEL_2        (WCM_OPTION_TEST_CHANNEL | 0x00000004)
#define WCM_OPTION_CHANNEL_3        (WCM_OPTION_TEST_CHANNEL | 0x00000008)
#define WCM_OPTION_CHANNEL_4        (WCM_OPTION_TEST_CHANNEL | 0x00000010)
#define WCM_OPTION_CHANNEL_5        (WCM_OPTION_TEST_CHANNEL | 0x00000020)
#define WCM_OPTION_CHANNEL_6        (WCM_OPTION_TEST_CHANNEL | 0x00000040)
#define WCM_OPTION_CHANNEL_7        (WCM_OPTION_TEST_CHANNEL | 0x00000080)
#define WCM_OPTION_CHANNEL_8        (WCM_OPTION_TEST_CHANNEL | 0x00000100)
#define WCM_OPTION_CHANNEL_9        (WCM_OPTION_TEST_CHANNEL | 0x00000200)
#define WCM_OPTION_CHANNEL_10       (WCM_OPTION_TEST_CHANNEL | 0x00000400)
#define WCM_OPTION_CHANNEL_11       (WCM_OPTION_TEST_CHANNEL | 0x00000800)
#define WCM_OPTION_CHANNEL_12       (WCM_OPTION_TEST_CHANNEL | 0x00001000)
#define WCM_OPTION_CHANNEL_13       (WCM_OPTION_TEST_CHANNEL | 0x00002000)
#define WCM_OPTION_CHANNEL_ALL      (WCM_OPTION_TEST_CHANNEL | WCM_OPTION_FILTER_CHANNEL)
#define WCM_OPTION_CHANNEL_RDC      (WCM_OPTION_CHANNEL_1 | WCM_OPTION_CHANNEL_7 | WCM_OPTION_CHANNEL_13)

// DCF �ʐM���̃p���[�Z�[�u�ݒ�
#define WCM_OPTION_TEST_POWER   0x00020000
#define WCM_OPTION_FILTER_POWER 0x00010000
#define WCM_OPTION_MASK_POWER   (WCM_OPTION_TEST_POWER | WCM_OPTION_FILTER_POWER)
#define WCM_OPTION_POWER_SAVE   (WCM_OPTION_TEST_POWER | 0x00000000)
#define WCM_OPTION_POWER_ACTIVE (WCM_OPTION_TEST_POWER | 0x00010000)

// AP �Ƃ̔F�؃��[�h
#define WCM_OPTION_TEST_AUTH        0x00080000
#define WCM_OPTION_FILTER_AUTH      0x00040000
#define WCM_OPTION_MASK_AUTH        (WCM_OPTION_TEST_AUTH | WCM_OPTION_FILTER_AUTH)
#define WCM_OPTION_AUTH_OPENSYSTEM  (WCM_OPTION_TEST_AUTH | 0x00000000)
#define WCM_OPTION_AUTH_SHAREDKEY   (WCM_OPTION_TEST_AUTH | 0x00040000)

// �X�L�������
#define WCM_OPTION_TEST_SCANTYPE    0x00200000
#define WCM_OPTION_FILTER_SCANTYPE  0x00100000
#define WCM_OPTION_MASK_SCANTYPE    (WCM_OPTION_TEST_SCANTYPE | WCM_OPTION_FILTER_SCANTYPE)
#define WCM_OPTION_SCANTYPE_PASSIVE (WCM_OPTION_TEST_SCANTYPE | 0x00000000)
#define WCM_OPTION_SCANTYPE_ACTIVE  (WCM_OPTION_TEST_SCANTYPE | 0x00100000)

// �X�L�����ꏄ���̒ʒm�ېݒ�
#define WCM_OPTION_TEST_ROUNDSCAN   0x00800000
#define WCM_OPTION_FILTER_ROUNDSCAN 0x00400000
#define WCM_OPTION_MASK_ROUNDSCAN   (WCM_OPTION_TEST_ROUNDSCAN | WCM_OPTION_FILTER_ROUNDSCAN)
#define WCM_OPTION_ROUNDSCAN_IGNORE (WCM_OPTION_TEST_ROUNDSCAN | 0x00000000)
#define WCM_OPTION_ROUNDSCAN_NOTIFY (WCM_OPTION_TEST_ROUNDSCAN | 0x00400000)

/*---------------------------------------------------------------------------*
    �}�N�� ��`
 *---------------------------------------------------------------------------*/
#define WCM_ROUNDUP32(_x_)      (((_x_) + 0x01f) &~(0x01f))
#define WCM_ROUNDDOWN32(_x_)    ((_x_) &~(0x01f))
#define WCM_ROUNDUP4(_x_)       (((_x_) + 0x03) &~(0x03))
#define WCM_ROUNDDOWN4(_x_)     ((_x_) &~(0x03))

/*---------------------------------------------------------------------------*
    �\���� ��`
 *---------------------------------------------------------------------------*/

// WCM ���C�u��������ݒ�
typedef struct WCMConfig
{
    s32     dmano;      // WM �����p���� DMA �ԍ� ( 0 �` 3 )
    void*   pbdbuffer;  // AP ���ێ��̈�A�h���X
    s32     nbdbuffer;  // AP ���ێ��̈�T�C�Y ( �o�C�g�P�� )
    s32     nbdmode;    // AP ���ێ����@ ( WCM_APLIST_MODE_* )
} WCMConfig;

// WCM ���C�u�����ʒm�� �ڍ׃p�����[�^���p��
typedef union WCMNoticeParameter
{
    s32     n;          // �����^
    void*   p;          // �|�C���^�^
} WCMNoticeParameter;

// WCM ���C�u�����ʒm�R�[���o�b�N�ɓn�����\����
typedef struct WCMNotice
{
    s16 notify; // �ʒm��� ( WCM_NOTIFY_* )
    s16 result; // �������� ( WCM_RESULT_* )
    WCMNoticeParameter  parameter[3];   // �e��ڍ׃p�����[�^
} WCMNotice;

// WCM �ʒm�R�[���o�b�N�֐� �^��`
typedef void (*WCMNotify) (WCMNotice * arg);

// WEP�L�[�ݒ�\����
typedef struct WCMWepDesc
{
    u8  mode;                   // WEP �Í������[�h ( WCM_WEPMODE_* )
    u8  keyId;                  // WEP �L�[ ID ( 0 �` 3 )
    u8  key[WM_SIZE_WEPKEY];    // WEP �L�[�f�[�^��[ 20 * 4 �o�C�g ]
} WCMWepDesc;

/*---------------------------------------------------------------------------*
    �֐� ��`
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         WCM_Init

  Description:  WCM ���C�u����������������B�����֐��B

  Arguments:    buf     -   WCM ���C�u�����������I�Ɏg�p���郏�[�N�o�b�t�@�ւ�
                            �|�C���^���w�肷��B������������́AWCM_Finish �֐�
                            �ɂ���� WCM ���C�u�������I������܂ŕʂ̗p�r�ɗp��
                            �Ă͂����Ȃ��B
                len     -   ���[�N�o�b�t�@�̃T�C�Y���w�肷��BWCM_WORK_SIZE ���
                            �������T�C�Y���w�肷��Ƃ��̊֐��͎��s����B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̏������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_NOT_ENOUGH_MEM
 *---------------------------------------------------------------------------*/
s32         WCM_Init(void* buf, s32 len);

/*---------------------------------------------------------------------------*
  Name:         WCM_Finish

  Description:  WCM���C�u�������I������B�����֐��BWCM_Init�֐��ɂ���Ďw�肵��
                ���[�N�p�o�b�t�@�͉�������B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̏������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE
 *---------------------------------------------------------------------------*/
s32         WCM_Finish(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_StartupAsync

  Description:  �����@�\�̋N���V�[�P���X���J�n����B
                �����I�ɂ� WAIT ���� IDLE �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� notify �Ŏw�肵���R�[���o�b�N�֐��ɂ��񓯊��I��
                �������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_STARTUP �ƂȂ�B

  Arguments:    config  -   WCM �̓���ݒ�ƂȂ�\���̂ւ̃|�C���^���w��B
                notify  -   �񓯊��I�ȏ������ʂ�ʒm����R�[���o�b�N�֐����w��B
                            ���̃R�[���o�b�N�֐��͈Ȍ�̔񓯊��I�Ȋ֐��̌���
                            �ʒm�ɋ��ʂ��Ďg�p�����B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_WMDISABLE ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32         WCM_StartupAsync(WCMConfig* config, WCMNotify notify);

/*---------------------------------------------------------------------------*
  Name:         WCM_CleanupAsync

  Description:  �����@�\�̒�~�V�[�P���X���J�n����B
                �����I�ɂ� IDLE ���� WAIT �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_CLEANUP �ƂȂ�B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32         WCM_CleanupAsync(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_SearchAsync

  Description:  AP �����T���J�n�V�[�P���X�A�������͒�~�V�[�P���X���J�n����B
                �p�����[�^�ɂ���� WCM_BeginSearchAsync �֐����Ăяo�����A
                WCM_EndSearchAsync �֐����Ăяo�����U�蕪���邾���̊֐��B

  Arguments:    bssid   -   �T������ AP �� BSSID ���w�肷��BNULL ���w�肵��
                            �ꍇ�ɂ́A�T���̒�~���w���������̂Ƃ݂Ȃ����B
                essid   -   �T������ AP �� ESSID ���w�肷��BNULL ���w�肵��
                            �ꍇ�ɂ́A�T���̒�~���w���������̂Ƃ݂Ȃ����B
                option  -   �I�v�V�����ݒ�t�H�[�}�b�g�ɏ]���� 32 �r�b�g��
                            �I�v�V�����ύX�p�ϐ����w�肷��B0 ���w�肵���ꍇ��
                            �ύX�͍s���Ȃ��B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32         WCM_SearchAsync(void* bssid, void* essid, u32 option);

/*---------------------------------------------------------------------------*
  Name:         WCM_BeginSearchAsync

  Description:  AP �����T���J�n�V�[�P���X���J�n����B���Ɏ����T����Ԃł�����
                �ꍇ�ɁA�T�������̕ύX�̂ݍs�����Ƃ��\�B
                �����I�ɂ� IDLE ���� SEARCH �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_BEGIN_SEARCH �ƂȂ�B

  Arguments:    bssid   -   �T������ AP �� BSSID ���w�肷��BNULL ��������
                            WCM_BSSID_ANY ���w�肵���ꍇ�͔C�ӂ� BSSID ������
                            AP ��T������B
                essid   -   �T������ AP �� ESSID ���w�肷��BNULL ��������
                            WCM_ESSID_ANY ���w�肵���ꍇ�͔C�ӂ� ESSID ������
                            AP ��T������B
                option  -   �I�v�V�����ݒ�t�H�[�}�b�g�ɏ]���� 32 �r�b�g��
                            �I�v�V�����ύX�p�ϐ����w�肷��B0 ���w�肵���ꍇ��
                            �ύX�͍s��Ȃ��B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32         WCM_BeginSearchAsync(void* bssid, void* essid, u32 option);

/*---------------------------------------------------------------------------*
  Name:         WCM_EndSearchAsync

  Description:  AP �T����~�V�[�P���X���J�n����B
                �����I�ɂ� SEARCH ���� IDLE �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A���@�I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_END_SEARCH �ƂȂ�B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32         WCM_EndSearchAsync(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_ConnectAsync

  Description:  AP �Ƃ̖����ڑ��V�[�P���X���J�n����B
                �����I�ɂ� IDLE ���� DCF �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_CONNECT �ƂȂ�B

  Arguments:    bssDesc -   �ڑ����� AP �̖����l�b�g���[�N�����w�肷��B
                            WCM_SearchAsync �ɂ���ē���ꂽ��񂪂��̂܂܎w��
                            ����邱�Ƃ�z��B
                wepDesc -   WCMWepDesc �^�� WEP �L�[�ɂ��Í����ݒ�����w��
                            ����BNULL �̏ꍇ�́AWEP �Í����Ȃ��Ƃ����ݒ�ɂȂ�B
                option  -   �I�v�V�����ݒ�t�H�[�}�b�g�ɏ]���� 32 �r�b�g��
                            �I�v�V�����ύX�p�ϐ����w�肷��B0 ���w�肵���ꍇ��
                            �ύX�͍s���Ȃ��B

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32         WCM_ConnectAsync(void* bssDesc, void* wepDesc, u32 option);

/*---------------------------------------------------------------------------*
  Name:         WCM_DisconnectAsync

  Description:  AP �Ƃ̖����ڑ���ؒf����V�[�P���X���J�n����B
                �����I�ɂ� DCF ���� IDLE �փt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_StartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_DISCONNECT �ƂȂ�B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32         WCM_DisconnectAsync(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_TerminateAsync

  Description:  �����@�\�̋����I���V�[�P���X���J�n����B
                �ʂ̔񓯊����������s����Ă��Ȃ�����͂ǂ̃t�F�[�Y����ł����s
                �\�ł���A�����I�ɂ� WAIT �ւƃt�F�[�Y�̈ڍs�������s����B
                �񓯊��֐��ł���A�����I�ȕԂ�l�� WCM_RESULT_ACCEPT �ł�����
                �ꍇ�ɂ� WCM_SartupAsync �֐��Ŏw�肵���R�[���o�b�N�֐��ɂ��
                �񓯊��I�ȏ������ʂ��ʒm�����B
                �񓯊��������ʂ̒ʒm��ʂ� WCM_NOTIFY_TERMINATE �ƂȂ�B

  Arguments:    None.

  Returns:      s32     -   �ȉ��̓��̂����ꂩ�̓����I�ȏ������ʂ��Ԃ����B
                            WCM_RESULT_SUCCESS , WCM_RESULT_FAILURE ,
                            WCM_RESULT_PROGRESS , WCM_RESULT_REJECT ,
                            WCM_RESULT_FATAL_ERROR , WCM_RESULT_ACCEPT
 *---------------------------------------------------------------------------*/
s32         WCM_TerminateAsync(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_GetPhase

  Description:  WCM ���C�u�����̓������ ( �t�F�[�Y ) ���擾����B�����֐��B

  Arguments:    None.

  Returns:      s32     -   ���݂� WCM ���C�u�����̃t�F�[�Y��Ԃ��B
 *---------------------------------------------------------------------------*/
s32         WCM_GetPhase(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_UpdateOption

  Description:  WCM ���C�u�����̃I�v�V�����ݒ���X�V����B

  Arguments:    option  -   �I�v�V�����ݒ�t�H�[�}�b�g�ɏ]���� 32 �r�b�g��
                            �I�v�V�����ύX�p�ϐ����w�肷��B
                            0 ���w�肵���ꍇ�͉����X�V���s��Ȃ�����ɂȂ�B

  Returns:      u32     -   �ύX���s���O�̃I�v�V�����ϐ���Ԃ��B
 *---------------------------------------------------------------------------*/
u32         WCM_UpdateOption(u32 option);

/*---------------------------------------------------------------------------*
  Name:         WCM_SetChannelScanTime

  Description:  AP �̎����T�����ɂP�̃`�����l�����X�L�������鎞�Ԃ�ݒ肷��B

  Arguments:    msec    -   �P�`�����l�����X�L�������鎞�Ԃ� ms �P�ʂŎw��B
                            10 �` 1000 �܂ł̊ԂŐݒ�\�����A���͈̔͊O�̒l��
                            �w�肷��Ǝ����T���̓f�t�H���g�̐ݒ莞�ԂŒT�����s��
                            �悤�ɂȂ�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void        WCM_SetChannelScanTime(u16 msec);

/*---------------------------------------------------------------------------*
  Name:         WCM_LockApList

  Description:  AP ���ێ����X�g�̃��b�N��Ԃ�ύX����B
    NOTICE:     ���b�N���͊��荞�݂ɂ���� AP �𔭌������ꍇ�ł� AP ���ێ�
                ���X�g�̓��e�͕ύX���ꂸ�A�������� AP ���͔j�������_�ɒ��ӁB

  Arguments:    lock    -   WCM_APLIST_LOCK     : ���b�N����B
                            WCM_APLIST_UNLOCK   : ���b�N���������B
                            ���̑�              : WCM_APLIST_LOCK �Ƃ݂Ȃ��B

  Returns:      s32     -   �Ăяo���O�̃��b�N��Ԃ�Ԃ��B
                            WCM_APLIST_LOCK     : ���b�N����Ă����B
                            WCM_APLIST_UNLOCK   : ���b�N�͉������Ă����B
 *---------------------------------------------------------------------------*/
s32         WCM_LockApList(s32 lock);

/*---------------------------------------------------------------------------*
  Name:         WCM_ClearApList

  Description:  AP ���ێ����X�g�̓��e���������ď�����Ԃɖ߂��B
                ���b�N��Ԕ@���Ɋւ�炸���ێ��̈���N���A����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void        WCM_ClearApList(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_CountApList

  Description:  AP ���ێ����X�g���ŕێ�����Ă��� AP ���Ǘ��u���b�N����
                �擾����B
    NOTICE:     WCM_LockApList�֐��ɂă��b�N����Ă��Ȃ��ꍇ�ɂ͊֐��ďo�����
                ���荞�݂ɂ���ău���b�N���͑�������\��������_�ɒ��ӁB

  Arguments:    None.

  Returns:      s32     -   AP ���Ǘ��u���b�N����Ԃ��B
                            ����Ƀu���b�N�����擾�ł��Ȃ��ꍇ�ɂ� 0 ��Ԃ��B
 *---------------------------------------------------------------------------*/
s32         WCM_CountApList(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_PointApList

  Description:  AP ���ێ����X�g������A�w�肳�ꂽ index �����蓖�Ă��Ă���
                AP ���𒊏o����B
    NOTICE:     WCM_LockApList�֐��ɂ���ă��b�N����Ă��Ȃ��ꍇ�ɂ͊֐��ďo��
                ��Ɋ��荞�݂ɂ���Ď擾�����|�C���^�������o�b�t�@�̓��e������
                �����\��������_�ɒ��ӁB

  Arguments:    index       -   index ���w�肷��BWCM_CountApList�֐��ɂ����
                                �����鐔 ������ index ���w�肷��K�v������B

  Returns:      WMBssDesc*  -   ���o���� AP ���ւ̃|�C���^��Ԃ��B
                                ���o�Ɏ��s�����ꍇ�� NULL ��Ԃ��B
 *---------------------------------------------------------------------------*/
WMBssDesc*  WCM_PointApList(s32 index);

/*---------------------------------------------------------------------------*
  Name:         WCM_PointApListLinkLevel

  Description:  AP ���ێ����X�g������A�w�肳�ꂽ index �����蓖�Ă��Ă���
                AP ��񂪓o�^���ꂽ�ۂ̓d�g���x�𒊏o����B

  Arguments:    index       -   index ���w�肷��BWCM_CountApList�֐��ɂ����
                                �����鐔 ������ index ���w�肷��K�v������B

  Returns:      WMLinkLevel -   ���o���� AP ���o�^���̓d�g���x��Ԃ��B
                                ���o�Ɏ��s�����ꍇ�� WM_LINK_LEVEL_0 ��Ԃ��B
 *---------------------------------------------------------------------------*/
WMLinkLevel WCM_PointApListLinkLevel(s32 index);

/*---------------------------------------------------------------------------*
  Name:         WCM_CompareBssID

  Description:  �Q�� BSSID ���r����B�����֐��B

  Arguments:    a       -   ��r�Ώۂ� BSSID �ւ̃|�C���^���w�肷��B
                b       -   ��r�Ώۂ� BSSID �ւ̃|�C���^���w�肷��B

  Returns:      BOOL    -   �Q�� BSSID �������Ȃ�� TRUE ��Ԃ��B
                            �قȂ� BSSID �ł���ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL        WCM_CompareBssID(u8* a, u8* b);

/*---------------------------------------------------------------------------*
  Name:         WCM_GetLinkLevel

  Description:  DCF �ʐM���̃����N���x���擾����B�����֐��B

  Arguments:    None.

  Returns:      WMLinkLevel -   4�i�K�ɕ]�����������N���x��Ԃ��B
 *---------------------------------------------------------------------------*/
WMLinkLevel WCM_GetLinkLevel(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_IsInfrastructureNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��񂪃C���t���X�g���N�`���l�b�g���[�N
                �̂��̂ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   �C���t���X�g���N�`���l�b�g���[�N�ł���ꍇ�� TRUE ���A
                            �����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL        WCM_IsInfrastructureNetwork(WMBssDesc* bssDesc);

/*---------------------------------------------------------------------------*
  Name:         WCM_IsAdhocNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��񂪃A�h�z�b�N�l�b�g���[�N�̂���
                �ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   �A�h�z�b�N�l�b�g���[�N�ł���ꍇ�� TRUE ���A
                            �����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL        WCM_IsAdhocNetwork(WMBssDesc* bssDesc);

/*---------------------------------------------------------------------------*
  Name:         WCM_IsPrivacyNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��� WEP �Z�L�����e�B�̓K�p���ꂽ
                �l�b�g���[�N�̂��̂ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   WEP �Z�L�����e�B�̓K�p���ꂽ�l�b�g���[�N�ł���ꍇ��
                            TRUE ���A�����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL        WCM_IsPrivacyNetwork(WMBssDesc* bssDesc);

/*---------------------------------------------------------------------------*
  Name:         WCM_IsShortPreambleNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��񂪃V���[�g�v���A���u�����g�p����
                �l�b�g���[�N�̂��̂ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   �V���[�g�v���A���u�����g�p�����l�b�g���[�N�ł���ꍇ��
                            TRUE ���A�����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL        WCM_IsShortPreambleNetwork(WMBssDesc* bssDesc);

/*---------------------------------------------------------------------------*
  Name:         WCM_IsLongPreambleNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��񂪃����O�v���A���u�����g�p����
                �l�b�g���[�N�̂��̂ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   �����O�v���A���u�����g�p�����l�b�g���[�N�ł���ꍇ��
                            TRUE ���A�����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL        WCM_IsLongPreambleNetwork(WMBssDesc* bssDesc);

/*---------------------------------------------------------------------------*
  Name:         WCM_IsEssidHidden

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��� ESS-ID ���B�����Ă��邩�ǂ���
                ���ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   ESS-ID ���B������Ă���ꍇ�� TRUE ��Ԃ��B
                            �L���� ESS-ID �����J����Ă���ꍇ�ɂ� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL        WCM_IsEssidHidden(WMBssDesc* bssDesc);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* NITROWIFI_WCM_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

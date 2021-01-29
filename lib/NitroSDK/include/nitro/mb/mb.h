/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - include
  File:     mb.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb.h,v $
  Revision 1.13  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.12  2005/08/17 01:13:34  yosizaki
  add MB_GetMultiBootDownloadParameter().

  Revision 1.11  2005/05/20 03:11:07  yosizaki
  add MBGameRegistry::userParam.

  Revision 1.10  2005/04/11 03:37:16  yosizaki
  fix comment.

  Revision 1.9  2005/04/11 01:46:10  yosizaki
  add MB_SetLifeTime().
  add MB_SetPowerSaveMode().

  Revision 1.8  2005/03/14 07:09:33  sato_masaki
  MB_GetGameEntryBitmap()�֐���ǉ��B

  Revision 1.7  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.6  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.5  2004/11/24 05:14:43  yosizaki
  rename MB_StartParentEx/MB_EndEx..

  Revision 1.4  2004/11/22 12:58:17  takano_makoto
  �R�����g�ǉ�

  Revision 1.3  2004/11/18 07:34:05  takano_makoto
  MB_SetParentCommSize, MB_SetParentCommParam�Ŏq�@���M�T�C�Y�̐ݒ������p�~

  Revision 1.2  2004/11/11 11:54:44  yosizaki
  add MB_SEGMENT_BUFFER_MIN.
  increase MB_SYSTEM_BUF_SIZE for task-system. (+0x1000)

  Revision 1.1  2004/11/10 13:12:47  takano_makoto
  �t�@�C���̈ʒu���ړ�

  Revision 1.54  2004/10/26 08:10:34  takano_makoto
  MB_DisconnectChild��ǉ�

  Revision 1.53  2004/10/18 11:46:12  yosizaki
  add MB_StartParentEx, MB_EndEx.

  Revision 1.52  2004/10/05 14:42:50  yosizaki
  export some MB_COMM_P_* range consts.

  Revision 1.51  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.50  2004/10/01 07:39:11  sato_masaki
  MB_COMM_PSTATE_WAIT_TO_SEND �X�e�[�g��ǉ��B

  Revision 1.49  2004/09/22 12:17:56  sato_masaki
  add function MB_SetParentCommParam()

  Revision 1.48  2004/09/21 01:39:31  sato_masaki
  WM�ɋN������G���[���AMB_ERRCODE_WM_FAILURE�ɓ���BMB_ERRCODE_MP_SENT_FAILURE��p�~�B

  Revision 1.47  2004/09/20 13:16:07  sato_masaki
  - MB_MAX_FILE��16�ɕύX
  - MB_ERROR��`���폜
  - MBErrorStatus�\���̂�level�����o���폜(�G���[���x���̔p�~)
  - �G���[�̒�`��MBError�񋓎q����AMBErrCode�񋓎q�Ɉڍs�B

  Revision 1.46  2004/09/17 05:08:10  sato_masaki
  small fix

  Revision 1.45  2004/09/17 04:43:09  sato_masaki
  add function MB_ReadMultiBootParentBssDesc()

  Revision 1.44  2004/09/16 12:46:56  sato_masaki
  MBi_***�Œ�`���Ă����֐����AMB_***�ɕύX�B

  Revision 1.43  2004/09/15 06:27:34  sato_masaki
  add function MBi_SetParentCommSize, MBi_GetTgid

  Revision 1.42  2004/09/15 05:08:20  miya
  �G���[�R�[�h�ǉ�

  Revision 1.41  2004/09/14 14:23:19  yosiokat
  �Emb_gameinfo.c���ł����g��Ȃ��萔��`��mb_gameinfo.c�Ɉړ��B

  Revision 1.40  2004/09/14 13:20:13  sato_masaki
  ���N�G�X�g�f�[�^��f�Љ����đ���悤�ɕύX�B

  Revision 1.39  2004/09/14 06:06:24  sato_masaki
  MB_CommBootRequest**, MB_CommStartSending**���C�����C�����B

  Revision 1.38  2004/09/13 13:03:32  yosiokat
  �EMB_GAME_NAME_LENGTH��"48"�ɕύX�B
  �EMB_GAME_INTRO_LENGTH��"96"�ɕύX�B

  Revision 1.37  2004/09/13 08:18:54  sato_masaki
  - add MB_COMM_PSTATE_ERROR to MBCommPState
  - add MBiErrorStatus, MBiError

  Revision 1.36  2004/09/13 03:01:14  sato_masaki
  MBi_Init()��ǉ��B(���d�l�Ƃ̌݊��̂���)
  MB_GetParentSystemBufSize �� MBi_GetParentSystemBufSize �ɕύX�B

  Revision 1.35  2004/09/11 11:23:57  sato_masaki
  - MB_SYSTEM_BUF_SIZE �l�ύX
  - MBCommResponseRequestType�^�Ƀ����o�ǉ��B
  - MB_GetParentSystemBufSize()�ǉ��B

  Revision 1.34  2004/09/10 07:40:51  yosizaki
  add MBCommRequestData.

  Revision 1.33  2004/09/10 06:30:29  sato_masaki
  comment�ǉ��B

  Revision 1.32  2004/09/10 02:42:10  sato_masaki
  MBUserInfo�\���̂�playerNo�Ɋւ���ԍ���`��ύX�B
  (��:�e15�A�q0-14 �� �V:�e0�A�q1-15)

  Revision 1.31  2004/09/08 04:39:54  sato_masaki
  comment�C���B

  Revision 1.30  2004/09/08 04:39:13  sato_masaki
  MB_CommBootRequestAll()�֐���ǉ��B�ύX�O��MB_CommBootRequest()�ƌ݊��̋@�\�����֐��ł��B

  Revision 1.29  2004/09/07 11:56:05  sato_masaki
  - MB_CommBootRequest()��AID�w��̊֐��ɕύX�B
  - MB_COMM_PSTATE_CANCEL��MB_COMM_PSTATE_END�ɖ��̕ύX�B
    MB���C�u�����̏I���R�[���o�b�N�́AMB_COMM_PSTATE_END�݂̂Ƃ���B

  Revision 1.28  2004/09/07 04:41:14  sato_masaki
  MBCommPState�񋓎q��MB_COMM_PSTATE_WM_EVENT��ǉ��B

  Revision 1.27  2004/09/06 12:26:43  sato_masaki
  comment�C��

  Revision 1.26  2004/09/06 12:02:33  sato_masaki
  MB_Init()�֐��̈����ɁAtgid��ǉ��B

  Revision 1.25  2004/09/04 09:51:35  sato_masaki
  �L�q�C���B�q�@API����ь^�Ɋւ����`���c���Ă������߁A�폜����B

  Revision 1.24  2004/09/04 06:46:49  sato_masaki
  ������ߎ��̃R�[���o�b�N�X�e�[�g(MB_COMM_*STATE_MEMBER_FULL)��݂���B

  Revision 1.23  2004/09/03 07:11:15  sato_masaki
  �q�@����pAPI�L�q���폜�B

  Revision 1.22  2004/09/02 11:55:58  sato_masaki
  MB_CommStartSending()�Ɏq�@AID�������Ƃ��Ēǉ��B
  �G���g���[���Ă����q�@�Ɉꊇ���đ��M����ꍇ�́AMB_CommStartSendingAll()
  ���g�p����悤�ɕύX�B

  Revision 1.21  2004/09/02 09:59:44  sato_masaki
  MB_DeleteFile()��MB_UnregisterFile()�ɖ��O�ύX�B

  Revision 1.20  2004/09/02 09:45:56  sato_masaki
  �t�@�C���o�^�폜�̊֐��AMB_DeleteFile()��ǉ��B

  Revision 1.19  2004/08/27 04:04:43  sato_masaki
  MBCommCState��MB_COMM_CSTATE_AUTHENTICATION_FAILED�ǉ��B

  Revision 1.18  2004/08/26 09:59:54  sato_masaki
  MB_COMM_PSTATE_REQUESTED��ǉ��B
  MB_CommResponseRequest�֐���ǉ��B

  Revision 1.17  2004/08/25 03:15:18  sato_masaki
  MB_CommStartSending�֐���ǉ��B

  Revision 1.16  2004/08/19 14:54:44  yosiokat
  MB_BEACON_FIXED_DATA_SIZE���\���̂̕ύX�ɍ��킹�ďC���B

  Revision 1.15  2004/08/19 07:42:26  sato_masaki
  MB_ROM_HEADER_ADDRESS�̒�`��HW_ROM_HEADER_BUF�ɕύX

  Revision 1.14  2004/08/19 02:42:14  yosizaki
  change type of MBUserInfo.

  Revision 1.13  2004/08/16 13:08:02  sato_masaki
  RomHeader�i�[�A�h���X�AMB_ROM_HEADER_ADDRESS��ǉ�

  Revision 1.12  2004/08/14 12:19:56  sato_masaki
  MBParentBssDesc����AgameInfoLength�����o���폜

  Revision 1.11  2004/08/13 13:16:10  sato_masaki
  MB_COMM_PSTATE_DISCONNECTED��ǉ�

  Revision 1.10  2004/08/12 12:05:18  sato_masaki
  MBCommCState�̕��я��ύX

  Revision 1.9  2004/08/12 09:56:28  yosiokat
  MBGameRegistry�̃A�C�R���f�[�^�w����t�@�C���p�X�ւ̃|�C���^�ɕύX�B

  Revision 1.8  2004/08/12 09:14:11  yosiokat
  �EMB_SCAN_LOCK_SEC�̒ǉ��B
  �EMBCommCState��MB_COMM_CSTATE_CONNECT_FAILED��MB_COMM_CSTATE_DISCONNECTED_BY_PARENT��ǉ��B
  �E�����g�p�֐�MB_DeleteRecvGameInfo�̒ǉ��B

  Revision 1.7  2004/08/12 09:11:11  yosiokat
  �EMB_BSSDESC_SIZE��`��sizeof(MBParentBssDesc)�ɕύX�B
  �EMB_BSSDESC_ADDRESS�̒�`��0x023fe800�ɕύX�B

  Revision 1.6  2004/08/10 12:07:55  sato_masaki
  �e�@�A�q�@�X�e�[�g��`�̕ύX�A����сAMB_CommBootRequest, MB_CommIsBootable�֐��̒ǉ��B

  Revision 1.5  2004/08/09 14:56:09  yosizaki
  format all the symbol for API

  Revision 1.4  2004/08/09 07:08:49  sato_masaki
  ��` MB_BSSDESC_ADDRESS �̒l��ύX

  Revision 1.3  2004/08/08 23:42:14  yosizaki
  change some names.

  Revision 1.2  2004/08/07 15:03:12  yosizaki
  merge all the diff. (-2004/08/07)

  Revision 1.1  2004/08/07 07:03:27  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef NITRO_MB_MB_H_
#define NITRO_MB_MB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>
#include <nitro/misc.h>
#include <nitro/fs/file.h>
#include <nitro/wm.h>


/* ---------------------------------------------------------------------

        �萔

   ---------------------------------------------------------------------*/

#define MB_MAX_CHILD                    WM_NUM_MAX_CHILD

/* �t�@�C�����̕����� */
#define MB_FILENAME_SIZE_MAX            (10)

/* �e�@�̃t�@�C���ő吔 */
#define MB_MAX_FILE                     (16)

/* �}���`�u�[�g���[�N�̃T�C�Y */
#define MB_SYSTEM_BUF_SIZE              (0xC000)

/* �e�@�̒ʐM�T�C�Y�̐ݒ�\�͈� */
#define MB_COMM_PARENT_SEND_MIN         256
#define MB_COMM_PARENT_SEND_MAX         510

#define MB_COMM_PARENT_RECV_MIN         8
#define MB_COMM_PARENT_RECV_MAX         16

/* MB_ReadSegment() �ɕK�v�ȃo�b�t�@�T�C�Y�̍ŏ��l */
#define MB_SEGMENT_BUFFER_MIN           0x10000

/* MB API ���ʌ^ */
#define MB_SUCCESS                      MB_ERRCODE_SUCCESS      // ���݊�

/* MB_Init() �ɑ΂��� TGID �������Őݒ肷��悤�w�肷���`�l */
#define	MB_TGID_AUTO	0x10000

/*
 * �}���`�u�[�g�p�e�@�Q�[�����
 */

#define MB_ICON_COLOR_NUM               16      // �A�C�R���F��
#define MB_ICON_PALETTE_SIZE            (MB_ICON_COLOR_NUM * 2) // �A�C�R���f�[�^�T�C�Y (16bit�J���[�~�F��)
#define MB_ICON_DATA_SIZE               512     // �A�C�R���f�[�^�T�C�Y (32�~32dot 16�F)
#define MB_GAME_NAME_LENGTH             48      // �Q�[���l�[����       (2byte�P��)         ���ő�ō��L�������ł��A����185�h�b�g�ȓ��Ɏ��܂�͈͂Ŏw��B
#define MB_GAME_INTRO_LENGTH            96      // �Q�[��������         (2byte�P��)         ���ő�ō��L�������ł��A����199�h�b�g�~2�Ɏ��܂�͈͂Ŏw��B
#define MB_USER_NAME_LENGTH             10      // ���[�U�[�l�[����     (2byte�P��)
#define MB_MEMBER_MAX_NUM               15      // �ʐM�����o�[�ő吔
#define MB_FILE_NO_MAX_NUM              64      // MbGameInfo.fileNo�̓r�[�R���ɏ悹�鎞��6bit�ɂȂ�̂ŁA�ő��64��
#define MB_GAME_INFO_RECV_LIST_NUM      16      // �q�@���ł̃Q�[������M���X�g��

/* �u�[�g�^�C�v */
#define MB_TYPE_ILLEGAL                 0       /* �s���ȏ�� */
#define MB_TYPE_NORMAL                  1       /* ROM ����N�� */
#define MB_TYPE_MULTIBOOT               2       /* �}���`�u�[�g�q�@ */

#define MB_BSSDESC_SIZE                 (sizeof(MBParentBssDesc))
#define MB_DOWNLOAD_PARAMETER_SIZE      HW_DOWNLOAD_PARAMETER_SIZE

/* �e�@�_�E�����[�h�̏�� */
typedef enum
{
    MB_COMM_PSTATE_NONE,
    MB_COMM_PSTATE_INIT_COMPLETE,
    MB_COMM_PSTATE_CONNECTED,
    MB_COMM_PSTATE_DISCONNECTED,
    MB_COMM_PSTATE_KICKED,
    MB_COMM_PSTATE_REQ_ACCEPTED,
    MB_COMM_PSTATE_SEND_PROCEED,
    MB_COMM_PSTATE_SEND_COMPLETE,
    MB_COMM_PSTATE_BOOT_REQUEST,
    MB_COMM_PSTATE_BOOT_STARTABLE,
    MB_COMM_PSTATE_REQUESTED,
    MB_COMM_PSTATE_MEMBER_FULL,
    MB_COMM_PSTATE_END,
    MB_COMM_PSTATE_ERROR,
    MB_COMM_PSTATE_WAIT_TO_SEND,

    /*  �����g�p�̗񋓒l�ł��B
       ���̒l�̏�Ԃɂ͑J�ڂ��܂���B */
    MB_COMM_PSTATE_WM_EVENT = 0x80000000
}
MBCommPState;


/* �q�@����̐ڑ����N�G�X�g�ɑ΂��鉞�� */
typedef enum
{
    MB_COMM_RESPONSE_REQUEST_KICK,     /* �q�@����̃G���g���[�v�������ۂ���D */
    MB_COMM_RESPONSE_REQUEST_ACCEPT,   /* �q�@����̃G���g���[�v�����󂯓����D */
    MB_COMM_RESPONSE_REQUEST_DOWNLOAD, /* �q�@�փ_�E�����[�h�J�n��ʒm����D */
    MB_COMM_RESPONSE_REQUEST_BOOT      /* �q�@�փu�[�g�J�n��ʒm����D */
}
MBCommResponseRequestType;


/*  �e�@�C�x���g�ʒm�R�[���o�b�N�^ */
typedef void (*MBCommPStateCallback) (u16 child_aid, u32 status, void *arg);


typedef struct
{
    u16     errcode;
}
MBErrorStatus;

typedef enum
{
    /* �����������l */
    MB_ERRCODE_SUCCESS = 0,

    MB_ERRCODE_INVALID_PARAM,          /* �����G���[ */
    MB_ERRCODE_INVALID_STATE,          /* �Ăяo����Ԃ̏����s���� */

    /* �ȉ��̃G���[�͎q�@��p�D */
    MB_ERRCODE_INVALID_DLFILEINFO,     /* �_�E�����[�h��񂪕s�� */
    MB_ERRCODE_INVALID_BLOCK_NO,       /* ��M�����u���b�N�ԍ����s�� */
    MB_ERRCODE_INVALID_BLOCK_NUM,      /* ��M�t�@�C���̃u���b�N�����s�� */
    MB_ERRCODE_INVALID_FILE,           /* �v�����Ă��Ȃ��t�@�C���̃u���b�N����M���� */
    MB_ERRCODE_INVALID_RECV_ADDR,      /* ��M�A�h���X���s�� */

    /* �ȉ��̃G���[��WM�̃G���[�ɋN���D */
    MB_ERRCODE_WM_FAILURE,             /* WM �̃R�[���o�b�N�ɂ�����G���[ */

    /** �ȍ~�͒ʐM�p���s�Ȃ��� (WM �̍ď��������K�v) **/
    MB_ERRCODE_FATAL,

    MB_ERRCODE_MAX
}
MBErrCode;

/* ---------------------------------------------------------------------

        �\����

   ---------------------------------------------------------------------*/

/*
 * �}���`�u�[�g�Q�[���o�^���\����.
 */
typedef struct
{
    /* �v���O�����o�C�i���̃t�@�C���p�X */
    const char *romFilePathp;
    /* �Q�[�����̕�������w���|�C���^ */
    u16    *gameNamep;
    /* �Q�[�����e�̐�������������w���|�C���^ */
    u16    *gameIntroductionp;
    /* �A�C�R���L�����f�[�^�̃t�@�C���p�X */
    const char *iconCharPathp;
    /* �A�C�R���p���b�g�f�[�^�̃t�@�C���p�X */
    const char *iconPalettePathp;
    /* �Q�[���O���[�vID */
    u32     ggid;
    /* �ő�v���C�l�� */
    u8      maxPlayerNum;
    u8      pad[3];
    /* ���[�U��`�̊g���p�����[�^ (32BYTE) */
    u8      userParam[MB_DOWNLOAD_PARAMETER_SIZE];
}
MBGameRegistry;


/*
 * �A�C�R���f�[�^�\����.
 * �A�C�R���� 16�F�p���b�g + 32dot * 32dot.
 */
typedef struct
{
    /* �p���b�g�f�[�^ (32 BYTE) */
    u16     palette[MB_ICON_COLOR_NUM];
    u16     data[MB_ICON_DATA_SIZE / 2];
    /* �L�����N�^�f�[�^ (512 BUYTE) */
}
MBIconInfo;                            /* 544byte */


/*
 * ���[�U�[���\����.
 */
typedef struct
{
    /* �D���ȐF�f�[�^ (�F�Z�b�g�ԍ�) */
    u8      favoriteColor:4;
    /* �v���C���[�ԍ��i�e�@��No.0�A�q�@��No.1-15�j */
    u8      playerNo:4;
    /* �j�b�N�l�[���� */
    u8      nameLength;
    /* �j�b�N�l�[�� */
    u16     name[MB_USER_NAME_LENGTH];
}
MBUserInfo;                            /* 22byte */


/*
 * �}���`�u�[�g�e�@���.
 * (WMbssDesc����gameInfo[128]������������)
 */
typedef struct
{
    u16     length;                    // 2
    u16     rssi;                      // 4
    u16     bssid[3];                  // 10
    u16     ssidLength;                // 12
    u8      ssid[32];                  // 44
    u16     capaInfo;                  // 46
    struct
    {
        u16     basic;                 // 48
        u16     support;               // 50
    }
    rateSet;
    u16     beaconPeriod;              // 52
    u16     dtimPeriod;                // 54
    u16     channel;                   // 56
    u16     cfpPeriod;                 // 58
    u16     cfpMaxDuration;            // 60
}
MBParentBssDesc;


/* �}���`�u�[�g�q�@�ɂƂ��Ă̐e�@����ێ�����\���� */
typedef struct
{
    u16     boot_type;
    MBParentBssDesc parent_bss_desc;
}
MBParam;


/* �q�@����̃G���g���[���N�G�X�g���Ɏ�M����f�[�^�^ */
typedef struct
{
    u32     ggid;                      //  4B
    MBUserInfo userinfo;               // 22B
    u16     version;                   //  2B
    u8      fileid;                    //  1B
    u8      pad[3];                    // �v 29B
}
MBCommRequestData;


/* ---------------------------------------------------------------------

        �}���`�u�[�g���C�u����(MB)API - �e�@�p

   ---------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         MB_Init

  Description:  MB ���C�u����������

  Arguments:    work   - MB�̓�����ԊǗ��̂��߂Ɋ��蓖�Ă郁�����ւ̃|�C���^
                         ���̃T�C�Y�� MB_SYSTEM_BUF_SIZE �o�C�g�ȏ�ł���K�v
                         ������܂��B�܂��A���I�m�ۂ��s���ꍇ�A
                         MB_GetParentSystemBufSize�֐��ɂāA�T�C�Y���擾���邱�Ƃ�
                         �ł��܂��B
                         ���蓖�Ă���������MB_End�֐����ĂԂ܂œ����Ŏg�p����
                         �܂��B

                user   - ���[�U�f�[�^���i�[���ꂽ�\���̂̃|�C���^
                         ���̃|�C���^���w�����e��MB_Init�֐��̒��ł̂ݎQ�Ƃ���
                         �܂��B

                ggid   - GGID���w�肵�܂��B

                tgid   - TGID�̒l (�l��WM�̃K�C�h���C���ɑ��������ߕ��Ő�������
                         ���̂��g�p���Ă��������B)
                         MB_TGID_AUTO ���w�肷��ƁA�����Ŏ����I�ɓK�؂Ȓl��
                         �ݒ肳��܂��B

                dma    - MB�̓��������̂��߂Ɋ��蓖�Ă�DMA�`�����l��
                         ����DMA�`�����l����MB_End�֐����ĂԂ܂œ����Ŏg�p����
                         �܂��B 
  
  Returns:      ����������������������� MB_SUCCESS ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

int     MB_Init(void *work, const MBUserInfo *user, u32 ggid, u32 tgid, u32 dma);


/*---------------------------------------------------------------------------*
  Name:         MB_GetParentSystemBufSize

  Description:  MB�Ŏg�p���郏�[�N�������̃T�C�Y���擾

  Arguments:    None.
  
  Returns:      MB�Ŏg�p���郏�[�N�������̃T�C�Y
 *---------------------------------------------------------------------------*/
int     MB_GetParentSystemBufSize(void);


/*---------------------------------------------------------------------------*
  Name:         MB_SetParentCommSize

  Description:  MB �ʐM�Ɏg�p����e�@�̒ʐM�f�[�^�T�C�Y���Z�b�g�B
                MB_Init �����s���A MB_StartParent ���R�[������O�̃^�C�~���O��
                �g�p���Ă��������B�ʐM�J�n��́A�o�b�t�@�T�C�Y�̕ύX�͂ł��܂���B

  Arguments:    sendSize - �e�@����e�q�@�ɑ���A���M�f�[�^�T�C�Y
  
  Returns:      ����M�f�[�^�T�C�Y�̕ύX�����������TRUE�A�����łȂ��ꍇ��
                FALSE��Ԃ��܂��D
 *---------------------------------------------------------------------------*/

BOOL    MB_SetParentCommSize(u16 sendSize);

/*---------------------------------------------------------------------------*
  Name:         MB_SetParentCommParam

  Description:  MB �ʐM�Ɏg�p����e�@�̒ʐM�f�[�^�T�C�Y�A�ő�q�@�ڑ������Z�b�g�B
                MB_Init �����s���A MB_StartParent ���R�[������O�̃^�C�~���O��
                �g�p���Ă��������B�ʐM�J�n��́A�o�b�t�@�T�C�Y�̕ύX�͂ł��܂���B

  Arguments:    sendSize    - �e�@����e�q�@�ɑ���A���M�f�[�^�T�C�Y
                maxChildren - �ő�q�@�ڑ���
  
  Returns:      ����M�f�[�^�T�C�Y�̕ύX�����������TRUE�A�����łȂ��ꍇ��
                FALSE��Ԃ��܂��D
 *---------------------------------------------------------------------------*/

BOOL    MB_SetParentCommParam(u16 sendSize, u16 maxChildren);

/*---------------------------------------------------------------------------*
  Name:         MB_GetTgid

  Description:  MB ���C�u�����ɂ����āAWM�Ŏg�p���Ă���TGID���擾

  Arguments:    None.
  
  Returns:      WM�Ŏg�p���Ă���TGID��Ԃ��܂��D
 *---------------------------------------------------------------------------*/

u16     MB_GetTgid(void);


/*---------------------------------------------------------------------------*
  Name:         MB_End

  Description:  MB���C�u�������I��

  Arguments:    None.
  
  Returns:      None.
 *---------------------------------------------------------------------------*/

void    MB_End(void);


/*---------------------------------------------------------------------------*
  Name:         MB_EndToIdle

  Description:  MB���C�u�������I��.
                ������WM���C�u������ IDLE �X�e�[�g�Ɉێ�����܂�.

  Arguments:    None.
  
  Returns:      None.
 *---------------------------------------------------------------------------*/

void    MB_EndToIdle(void);

/*---------------------------------------------------------------------------*
  Name:         MB_DisconnectChild

  Description:  �q�@�̐ؒf

  Arguments:    aid - �ؒf����q�@��aid 
  
  Returns:      �J�n��������������������� MB_SUCCESS ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

void    MB_DisconnectChild(u16 aid);


/*---------------------------------------------------------------------------*
  Name:         MB_StartParent

  Description:  �e�@�p�����[�^�ݒ� & �X�^�[�g

  Arguments:    channel - �e�@�Ƃ��ĒʐM���s���`�����l�� 
  
  Returns:      �J�n��������������������� MB_SUCCESS ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

int     MB_StartParent(int channel);


/*---------------------------------------------------------------------------*
  Name:         MB_StartParentFromIdle

  Description:  �e�@�p�����[�^�ݒ� & �X�^�[�g.
                ������WM���C�u������ IDLE �X�e�[�g�ł��邱�Ƃ��K�v�ł�.

  Arguments:    channel - �e�@�Ƃ��ĒʐM���s���`�����l�� 
  
  Returns:      �J�n��������������������� MB_SUCCESS ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

int     MB_StartParentFromIdle(int channel);

/*---------------------------------------------------------------------------*
  Name:         MB_CommGetChildrenNumber

  Description:  ���݃G���g���[���Ă���q�̑������擾

  Arguments:    None.

  Returns:      ���ݐe�@���F�����Ă���q�@�̑�����Ԃ��܂��B
 *---------------------------------------------------------------------------*/

u8      MB_CommGetChildrenNumber(void);


/*---------------------------------------------------------------------------*
  Name:         MB_CommGetChildUser

  Description:  �w�� aid �̎q�̏����擾

  Arguments:    child_aid - ��Ԃ��擾����q�@���w�� aid 
  
  Returns:      �J�n��������������������� MB_SUCCESS ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

const MBUserInfo *MB_CommGetChildUser(u16 child_aid);


/*---------------------------------------------------------------------------*
  Name:         MB_CommGetParentState

  Description:  �w�� aid �̎q�@�ɑ΂���e�@��Ԃ��擾

  Arguments:    child_aid - ��Ԃ��擾����q�@���w�� aid 
  
  Returns:      MBCommPState�Ŏ�����邢���ꂩ�̏�Ԃ�Ԃ��܂��B
 *---------------------------------------------------------------------------*/

int     MB_CommGetParentState(u16 child);


/*---------------------------------------------------------------------------*
  Name:         MB_CommSetParentStateCallback

  Description:  �e�@��Ԓʒm�R�[���o�b�N��ݒ�

  Arguments:    callback - �e�@��Ԃ�ʒm������R�[���o�b�N�֐��ւ̃|�C���^
  
  Returns:      None.
 *---------------------------------------------------------------------------*/

void    MB_CommSetParentStateCallback(MBCommPStateCallback callback);


/*---------------------------------------------------------------------------*
  Name:         MB_GetSegmentLength

  Description:  �w�肳�ꂽ�o�C�i���t�@�C���ɕK�v�ȃZ�O�����g�o�b�t�@�����擾

  Arguments:    file - �o�C�i���t�@�C�����w�� FSFile �\����
  
  Returns:      ����ɃT�C�Y���擾�ł���ΐ��̒l��, �����łȂ���� 0
 *---------------------------------------------------------------------------*/

u32     MB_GetSegmentLength(FSFile *file);


/*---------------------------------------------------------------------------*
  Name:         MB_ReadSegment

  Description:  �w�肳�ꂽ�o�C�i���t�@�C������K�v�ȃZ�O�����g�f�[�^��ǂݏo��.

  Arguments:    file - �o�C�i���t�@�C�����w�� FSFile �\����
                buf  - �Z�O�����g�f�[�^��ǂݏo���o�b�t�@
                len  - buf �̃T�C�Y
  
  Returns:      ����ɃZ�O�����g�f�[�^��ǂݏo����� TRUE, �����łȂ���� FALSE
 *---------------------------------------------------------------------------*/

BOOL    MB_ReadSegment(FSFile *file, void *buf, u32 len);


/*---------------------------------------------------------------------------*
  Name:         MB_RegisterFile

  Description:  �w�肵���t�@�C�����_�E�����[�h���X�g�֓o�^

  Arguments:    game_reg - �o�^����v���O�����̏�񂪊i�[���ꂽ
                           MBGameRegistry�\���̂ւ̃|�C���^ 
                buf      - ���o�����Z�O�����g��񂪊i�[���ꂽ�������ւ̃|�C���^
                           ���̃Z�O�����g����MB_ReadSegment�֐��Ŏ擾���܂��B
  
  Returns:      �t�@�C���𐳂����o�^�ł����ꍇ��TRUE�A
                �����łȂ��ꍇ��FALSE��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

BOOL    MB_RegisterFile(const MBGameRegistry *game_reg, const void *buf);


/*---------------------------------------------------------------------------*
  Name:         MB_UnregisterFile

  Description:  �w�肵���t�@�C�����_�E�����[�h���X�g����폜

  Arguments:    game_reg - �폜����v���O�����̏�񂪊i�[���ꂽ
                           MBGameRegistry�\���̂ւ̃|�C���^ 

  Returns:      �t�@�C���𐳂����폜�ł����ꍇ��TRUE�A
                �����łȂ��ꍇ��FALSE��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

void   *MB_UnregisterFile(const MBGameRegistry *game_reg);


/*---------------------------------------------------------------------------*
  Name:         MB_CommResponseRequest

  Description:  �q�@����̐ڑ����N�G�X�g�ɑ΂��鉞�����w��

  Arguments:    child_aid - �����𑗐M����q�@���w�� aid
                ack       - �q�@�ւ̉������ʂ��w�肷��
                            MBCommResponseRequestType�^�̗񋓒l

  Returns:      �w�肵���q�@���ڑ������҂���Ԃł���
                �w�肵��������ނ������Ȃ��̂ł����TRUE���A
                �����łȂ��ꍇ��FALSE��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

BOOL    MB_CommResponseRequest(u16 child_aid, MBCommResponseRequestType ack);


/*---------------------------------------------------------------------------*
  Name:         MB_CommStartSending

  Description:  �G���g���[�ς݂̎w�� aid �̎q�@�ցA�u�[�g�C���[�W���M���J�n����

  Arguments:    child_aid - �u�[�g�C���[�W���M���J�n����q�@���w�� aid

  Returns:      �w��q�@���G���g���[���Ă����TRUE���A
                �����łȂ��Ȃ�FALSE��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

static inline BOOL MB_CommStartSending(u16 child_aid)
{
    return MB_CommResponseRequest(child_aid, MB_COMM_RESPONSE_REQUEST_DOWNLOAD);
}


/*---------------------------------------------------------------------------*
  Name:         MB_CommStartSendingAll

  Description:  �G���g���[���Ă����q�@�ցA�u�[�g�C���[�W���M���J�n����

  Arguments:    None.

  Returns:      �G���g���[���Ă���q�@�����݂��Ă����TRUE���A
                �����łȂ��Ȃ�FALSE��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

static inline BOOL MB_CommStartSendingAll(void)
{
    u8      child, num;

    for (num = 0, child = 1; child <= WM_NUM_MAX_CHILD; child++)
    {
        if (TRUE == MB_CommStartSending(child))
        {
            num++;
        }
    }

    if (num == 0)
        return FALSE;

    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MB_CommIsBootable

  Description:  �u�[�g�\���𔻒�

  Arguments:    child_aid - �Ώێq�@��AID

  Returns:      yes - TRUE  no - FALSE
 *---------------------------------------------------------------------------*/

BOOL    MB_CommIsBootable(u16 child_aid);


/*---------------------------------------------------------------------------*
  Name:         MB_CommBootRequest

  Description:  �_�E�����[�h�ς݂̎w�肵���q�@�ɑ΂��ău�[�g�v���𑗐M����

  Arguments:    child_aid - �Ώێq�@��AID

  Returns:      success - TRUE  failed - FALSE
 *---------------------------------------------------------------------------*/

static inline BOOL MB_CommBootRequest(u16 child_aid)
{
    return MB_CommResponseRequest(child_aid, MB_COMM_RESPONSE_REQUEST_BOOT);
}


/*---------------------------------------------------------------------------*
  Name:         MB_CommBootRequestAll

  Description:  �_�E�����[�h�ςݎq�@�ɑ΂��ău�[�g�v���𑗐M����

  Arguments:    None.

  Returns:      success - TRUE  failed - FALSE
 *---------------------------------------------------------------------------*/

static inline BOOL MB_CommBootRequestAll(void)
{
    u8      child, num;

    for (num = 0, child = 1; child <= WM_NUM_MAX_CHILD; child++)
    {
        if (TRUE == MB_CommBootRequest(child))
        {
            num++;
        }
    }

    if (num == 0)
        return FALSE;

    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MB_GetGameEntryBitmap

  Description:  �z�M����GameRegistry�ɑ΂��ăG���g���[���Ă���
                AID�r�b�g�}�b�v���擾����B

  Arguments:    game_req      - �ΏۂƂȂ�GameRegistry�ւ̃|�C���^

  Returns:      �w�肵��GameRegistry�ɃG���g���[���Ă���AID�r�b�g�}�b�v
                (�e�@AID:0�A�q�@AID:1-15)
                �Q�[�����z�M���ł͂Ȃ��ꍇ�A�Ԃ�l��0�ƂȂ�B
                �i�Q�[�����z�M���̏ꍇ�A�K���e�@AID:0���G���g���[�����o�[��
                  �܂܂�Ă���B�j
 *---------------------------------------------------------------------------*/

u16     MB_GetGameEntryBitmap(const MBGameRegistry *game_reg);


/* ---------------------------------------------------------------------

        �}���`�u�[�g���C�u����(MB)API - �u�[�g��q�@�p

   ---------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         MB_GetMultiBootParam

  Description:  �}���`�u�[�g�q�@�ɂƂ��Ă̐e�@��񂪕ێ����ꂽ�|�C���^���擾

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/

static inline const MBParam *MB_GetMultiBootParam(void)
{
    return (const MBParam *)HW_WM_BOOT_BUF;
}


/*---------------------------------------------------------------------------*
  Name:         MB_IsMultiBootChild

  Description:  �}���`�u�[�g�q�@���ǂ�������
                TRUE �ł���� MB_GetMultiBootParentBssDesc() ���L��

  Arguments:    None.

  Returns:      �}���`�u�[�g�q�@�ł���� TRUE
 *---------------------------------------------------------------------------*/

static inline BOOL MB_IsMultiBootChild(void)
{
    return MB_GetMultiBootParam()->boot_type == MB_TYPE_MULTIBOOT;
}


/*---------------------------------------------------------------------------*
  Name:         MB_GetMultiBootParentBssDesc

  Description:  �}���`�u�[�g�q�@�ł���΁A�e�@���ւ̃|�C���^��Ԃ�

  Arguments:    None.

  Returns:      �}���`�u�[�g�q�@�ł���ΗL���ȃ|�C���^�A�����łȂ���� NULL
 *---------------------------------------------------------------------------*/

static inline const MBParentBssDesc *MB_GetMultiBootParentBssDesc(void)
{
    return MB_IsMultiBootChild()? &MB_GetMultiBootParam()->parent_bss_desc : NULL;
}

/*---------------------------------------------------------------------------*
  Name:         MB_GetMultiBootDownloadParameter

  Description:  �}���`�u�[�g�q�@�ł���΁A�_�E�����[�h���Ɏw�肳�ꂽ
                ���[�U��`�̊g���p�����[�^��Ԃ�

  Arguments:    None.

  Returns:      �}���`�u�[�g�q�@�ł���ΗL���ȃ|�C���^�A�����łȂ���� NULL
 *---------------------------------------------------------------------------*/

static inline const u8 *MB_GetMultiBootDownloadParameter(void)
{
    return MB_IsMultiBootChild()? (const u8 *)HW_DOWNLOAD_PARAMETER : NULL;
}

/*---------------------------------------------------------------------------*
  Name:         MB_ReadMultiBootParentBssDesc

  Description:  �}���`�u�[�g�e�@��������p�����������ƂɁA
                WM_StartConnect �֐��ɗp���邽�߂́AWMBssDesc �\���̂̏���
                �Z�b�g����

  Arguments:    p_desc            pointer to destination WMBssDesc
                                    (must be aligned 32-bytes)
                parent_max_size   max packet length of parent in MP-protocol.
                                    (must be equal to parent's WMParentParam!)
                child_max_size    max packet length of child in MP-protocol.
                                    (must be equal to parent's WMParentParam!)
                ks_flag           if use key-sharing mode, TRUE.
                                    (must be equal to parent's WMParentParam!)
                cs_flag           if use continuous mode, TRUE.
                                    (must be equal to parent's WMParentParam!)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MB_ReadMultiBootParentBssDesc(WMBssDesc *p_desc,
                                      u16 parent_max_size, u16 child_max_size, BOOL ks_flag,
                                      BOOL cs_flag);

/*---------------------------------------------------------------------------*
  Name:         MB_SetLifeTime

  Description:  MB �̃��C�����X�쓮�ɑ΂��ă��C�t�^�C���𖾎��I�Ɏw�肵�܂�.
                �f�t�H���g�ł� ( 0xFFFF, 40, 0xFFFF, 40 ) �ɂȂ��Ă��܂�.

  Arguments:    tableNumber     ���C�t�^�C����ݒ肷��CAM�e�[�u���ԍ�
                camLifeTime     CAM�e�[�u���̃��C�t�^�C��
                frameLifeTime   �t���[���P�ʂł̃��C�t�^�C��
                mpLifeTime      MP�ʐM�̃��C�t�^�C��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MB_SetLifeTime(u16 tableNumber, u16 camLifeTime, u16 frameLifeTime, u16 mpLifeTime);

/*---------------------------------------------------------------------------*
  Name:         MB_SetPowerSaveMode

  Description:  �ȓd�̓��[�h��ݒ肵�܂�.
                �ȓd�̓��[�h��, �f�t�H���g�ŗL���ƂȂ��Ă��܂�.
                ���̊֐��͓d�͏�����l�����Ȃ��Ă悢��ʂň���ɋ쓮����
                ���߂̃I�v�V�����ł���, �d���ɂȂ���Ă��邱�Ƃ�ۏ�
                ���ꂽ������łȂ�����, �ʏ�̃Q�[���A�v���P�[�V������
                ������g�p���ׂ��ł͂���܂���.

  Arguments:    enable           �L���ɂ���Ȃ�� TRUE, �����ɂ���Ȃ�� FALSE.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MB_SetPowerSaveMode(BOOL enable);



/*****************************************************************************/
/* obsolete interfaces */

#define MB_StartParentEx(channel)    SDK_ERR("MB_StartParentEx() is discontinued. please use MB_StartParentFromIdle()\n")
#define MB_EndEx()    SDK_ERR("MB_EndEx() is discontinued. please use MB_EndToIdle()\n")



#ifdef __cplusplus
} /* extern "C" */
#endif


#endif // NITRO_MB_MB_H_

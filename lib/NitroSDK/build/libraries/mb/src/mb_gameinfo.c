/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_gameinfo.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

    $Log: mb_gameinfo.c,v $
    Revision 1.47  2007/08/09 08:27:44  takano_makoto
    MB_BC_MSG_GINFO_LOST�̃C�x���g�R�[���o�b�N�ł��AbssDesc��gameInfo�̏���L���Ƃ���

    Revision 1.46  2007/02/20 00:28:10  kitase_hirotake
    indent source

    Revision 1.45  2006/03/13 06:33:33  yosizaki
    add MB_FAKESCAN_PARENT_BEACON.

    Revision 1.44  2006/02/20 02:34:17  seiki_masashi
    WMGameInfo.version ��p�~���AmagicNumber �� ver ��ǉ�
    WMGameInfo.gameNameCount_attribute �� attribute �ɖ��O�ύX
    WM_GAMEINFO_TYPE_OLD �̃T�|�[�g��p�~

    Revision 1.43  2006/01/18 02:11:30  kitase_hirotake
    do-indent

    Revision 1.42  2005/11/18 05:31:12  yosizaki
    fix MBi_ReadyBeaconSendStatus().

    Revision 1.41  2005/11/14 05:37:28  yosizaki
    fix about WM_ATTR_FLAG_ENTRY flag.

    Revision 1.40  2005/03/04 12:35:05  yosiokat
    MB_DeleteGameInfo�ŁA���݃r�[�R���ŃQ�[�����z�M���̃Q�[�����폜���ꂽ�ꍇ�A
    ���̌�̃Q�[���z�M������ɓ��삵�Ȃ��Ȃ�s����C���B

    Revision 1.39  2005/02/28 05:26:24  yosizaki
    do-indent.

    Revision 1.38  2005/02/21 00:39:34  yosizaki
    replace prefix MBw to MBi.

    Revision 1.37  2005/02/18 11:56:03  yosizaki
    fix around hidden warnings.

    Revision 1.36  2005/01/24 23:48:25  takano_makoto
    ASSERT�̏C��

    Revision 1.35  2005/01/18 05:31:03  takano_makoto
    gameIntroduction��NUL�����ȍ~�̃f�[�^�����M����悤�ɏC��

    Revision 1.33  2005/01/17 09:29:06  takano_makoto
    MB_XXXUserBeaconData �� MB_XXXUserVolatData�֕ύX�BMB_SetSendVolatCallback�֐���ǉ��B

    Revision 1.32  2005/01/17 06:26:34  takano_makoto
    MB_SetUserBeaconData�ɂ����āA�f�[�^�Z�b�g���̊��荞�݂��֎~

    Revision 1.31  2005/01/17 05:54:29  takano_makoto
    MB_SetUserBeaconData, MB_GetUserBeaconData��ǉ�

    Revision 1.30  2005/01/11 07:41:13  takano_makoto
    fix copyright header.

    Revision 1.29  2004/11/22 12:43:44  takano_makoto
    MBw�֐����g�p���Ȃ��悤�ɕύX�BMB_GetGameInfoRecvList��ǉ��B

    Revision 1.28  2004/11/10 07:40:03  takano_makoto
    mb_gameinfo.c�̒�����MBw�֐���r�����A�X�L�����p�����[�^�̐ݒ��؂�o��

    Revision 1.27  2004/11/10 03:17:52  takano_makoto
    mbrs�Ŋm�ۂ��Ă����o�b�t�@���O����n���Ă��炦��悤�ɏC��

    Revision 1.26  2004/11/02 18:17:58  takano_makoto
    fix DEBUG Print.

    Revision 1.24  2004/10/28 11:01:42  yada
    just fix comment

    Revision 1.23  2004/10/05 09:45:29  terui
    MAC�A�h���X��u8�^�̔z��ɓ���B

    Revision 1.22  2004/10/04 13:40:25  terui
    �Q�[���O���[�vID��u32�^�ɓ���B

    Revision 1.21  2004/09/29 05:12:28  sato_masaki
    BssDesc�ɁA�e�Q�[����GGID��n���Ă��Ȃ������s��C���B
    
    Revision 1.20  2004/09/28 12:49:48  yosiokat
    IPL�u�����`1.14.2.7�ƃ}�[�W�B
    
    Revision 1.14.2.7  2004/09/28 10:53:13  yosiokat
    �EMBi_SendVolatBeacon����beaconNo�̃C���N�������g���s���Ă��Ȃ������s����C���B
    �EMB_SAME_BEACON_RECV_MAX_COUNT��"3"�ɕύX�B
    
    Revision 1.14.2.6  2004/09/28 02:42:11  yosiokat
    MBi_UnlockScanTarget�ŁA�A�����b�N���s���Ɏ��̃��b�N�^�[�Q�b�g�𒲂ׂă��b�N���鏈�����폜�B
    
    Revision 1.14.2.5  2004/09/28 02:40:25  yosiokat
    �E�e�@�̃r�[�R���ɑ��M���ƂɃC���N�������g����beaconNo��ǉ�����B
    �E�q�@��beaconNo�������Ă��Ȃ��e�@�̃r�[�R����A���Ŏ�M�����ꍇ�A���̐e�@���͖����Ƃ��鏈����ǉ��B
    
    Revision 1.14.2.4  2004/09/21 08:31:05  yosiokat
    �e�@�ŁA�Q�[������ǉ����폜���ǉ��������ɂ��������Ȃ�s����C���B
    
    Revision 1.19  2004/09/22 09:41:42  sato_masaki
    IPL_branch 1.14.2.4�ƃ}�[�W
    (�e�@�ŁA�Q�[������ǉ����폜���ǉ��������ɂ��������Ȃ�s����C���B)
    
    Revision 1.18  2004/09/20 10:45:41  yosiokat
    IPL�u�����`Rev.1.14.2.3�Ƃ̃}�[�W�B
    
    Revision 1.14.2.3  2004/09/20 10:15:49  yosiokat
    �E__UGI_IN_GGID���폜�B�r�[�R�����ɂ́A���ggid�������Ă���`���ɂ���B
    �EMBi_ClearFixedInfo���폜�B
    �EMB_RecvGameInfoBeacon���ł�SSID�̐ݒ��MBi_SetSSIDToBssDesc�֐��ɕ����B
    �EMBi_GetStoreElement���ōs���Ă���TGID�`�F�b�N��MBi_CheckTGID�Ƃ��āAMBi_AnalyzeBeacon���ōs���悤�ɂ���B
    �EMBi_CheckSeqNoFixed���̃V�[�P���X�ԍ��`�F�b�N���C���B�i�ꕔ��MBi_RecvFixedBeacon�Ɉړ��j
    �EMBi_CheckSeqNoFixed�AMBi_CheckSeqNoVolat�̕Ԃ�l��void�ɁB
    �EMBGameInfoRecvList����seqNoFixed, seqNoVolat���폜���āA��gameInfo���ɂ���seqNoFixed, seqNoVolat���g�p����悤�C���B
    
    Revision 1.14.2.2  2004/09/18 08:41:04  miya
    HEAD�ƃ}�[�W
    
    Revision 1.17  2004/09/17 04:00:38  miya
    �Q�ڈȍ~�̃Q�[���z�M��񂪎󂯎��Ȃ��o�O���C��
    
    Revision 1.16  2004/09/16 12:20:42  miya
    �t�@�C���X�V�p�̉���
    
    Revision 1.15  2004/09/16 11:22:22  miya
    �t�@�C���X�V�p�����o�ǉ�
    
    Revision 1.14  2004/09/15 12:42:42  yosiokat
    �v���C�����o�[���폜���ꂽ�ۂ̃r�[�R����M�����܂������Ă��Ȃ������̂��C���B
    
    Revision 1.13  2004/09/15 09:22:27  yosiokat
    mbss.seqNoVolat���AMBi_ReadyBeaconSendStatus���ŃQ�[����؂�ւ��鎞�ɃZ�b�g����Ă��Ȃ������s����C���B
    
    Revision 1.12  2004/09/15 07:03:10  yosiokat
    �EusefulGameInfoFlag��݂��āA��U�e�@�f�[�^�ꎮ����M������A�e�@�f�[�^�̎���������܂ŗ����Â���悤�ɂ���B
    �E���r�[�R����seqNoFixed��seqNoVolat�̗��������āA�X�L�������b�N�̔�����s���悤�ύX�B
    
    Revision 1.11  2004/09/14 14:40:14  yosiokat
    small fix.
    
    Revision 1.10  2004/09/14 14:22:19  yosiokat
    �EMbBeacon���Ƀ`�F�b�N�T����}�����A�r�[�R����M���Ƀ`�F�b�N�T���m�F�ɒʂ�Ȃ��r�[�R���͎̂Ă�悤�ύX�B
    �Emb_gameinfo.c���ł����g��Ȃ��萔��mb.h����ړ��B
    �EMB_SEND_MEMBER_MAX_NUM�̒�`���C���B
    
    Revision 1.9  2004/09/13 09:28:16  yosiokat
    �E�X�L�������b�N���Ƀ��b�N�Ώۂ��S��������Ȃ���Ԃ̑΍�̂��߁AMB_CountGameInfoLifetime�̈�����wmTypeEvent��ǉ��B
    �@�X�L�������b�N���쎞�Ƀ��b�N�Ώۂ��A���łS�񌩂���Ȃ�������A���b�N��������悤�ɂ���B
    
    Revision 1.8  2004/09/11 12:47:47  yosiokat
    �E�X�L�������b�N���ɃX�L�������Ԃ�ύX���鏈���ɁA���b�N���ԁA�e�@�������J�E���g�������Ή��ł��Ă��Ȃ������̂��C���B
    
    Revision 1.7  2004/09/10 01:31:06  sato_masaki
    PlayerFlag�̃t�H�[�}�b�g�ύX�B�i��Ɗ����j
    
    Revision 1.6  2004/09/09 14:01:47  sato_masaki
    mb_gameinfo.c�ɂ�����APlayerFlag�̃t�H�[�}�b�g�ύX�B�i��ƒ��j
    
    Revision 1.5  2004/09/09 10:04:17  sato_masaki
    small fix
    
    Revision 1.4  2004/09/09 09:22:40  sato_masaki
    GGID��TGID��SSID�ɃZ�b�g������@��ύX�B
    
    Revision 1.3  2004/09/08 13:01:30  sato_masaki
    �X�L�����Ώۂ̃��b�N���A���b�N�������ɁAMBi_SetMaxScanTime()�֐��ɂāA
    Scan���Ԃ��w�肷�鏈����ǉ��B
    
    Revision 1.2  2004/09/06 12:00:38  sato_masaki
    -GameInfo�ւ�fileNo�̐ݒ���AMB_RegisterFile()(mb_fileinfo.c)���ōs���悤�ɕύX�������߁A
     MB_AddGameInfo()����폜�B
    -�e�@�v���O������GGID�̓A�v���P�[�V�����ɂ���ĈقȂ�(���������ɐݒ肳���)���߁A
     �q�@�̕��ł́A�e�@GGID���������Ƃ��ł��Ȃ��Ȃ�̂ŁA
     MBi_CheckMBParent()���̐e�@GGID�`�F�b�N���폜�B
    -SetGameInfo���s���Ƃ���GGID��WM���������ɐݒ肵���e�@�ŗL��GGID�ɕύX�B
    -MB_RecvGameInfoBeacon()�ɂāA�_�E�����[�h������Q�[����GGID�ƁA�e�@��TGID��p����
     �q�@�ɓn��SSID�𐶐�����悤�ɂ����B
    
    Revision 1.1  2004/09/03 07:04:36  sato_masaki
    �t�@�C�����@�\�ʂɕ����B
    
    Revision 1.15  2004/08/26 09:53:00  sato_masaki
    tgid��1�ɌŒ�iSSID�`�F�b�N�̑΍�j
    
    Revision 1.14  2004/08/26 04:54:08  sato_masaki
    OS_Printf is changed into MB_DEBUG_OUTPUT
    
    Revision 1.13  2004/08/19 22:55:25  yosiokat
    MB_Init�ł̃}���`�u�[�g�t���O�AGGID�̃Z�b�g���~�߂āA�r�[�R�����M����WM_SetGameInfo�ŃZ�b�g����悤�ɂ���B
    
    Revision 1.12  2004/08/19 16:00:53  miya
    modified WM_SetGameInfo function
    
    Revision 1.11  2004/08/19 14:32:12  yosiokat
    �E__UGI_IN_GGID���I�t�ɁB
    �EMbBeacon�\���̂�fixed�v�f��flagmentNo, flagmentMaxNum, size��u8�w��ɕύX�B
    �EMBi_ExUTF16_LEtoSJIS_BE�ɉ��s�R�[�h0x0d,0x0a�̕ϊ���ǉ��B
    
    Revision 1.10  2004/08/18 23:57:14  yosizaki
    fix warning (p_game on SDK_FINALROM)
    
    Revision 1.9  2004/08/18 13:52:04  yosizaki
    add conversion routine for Unicode<->SJIS.
    
    Revision 1.8  2004/08/18 09:54:23  miya

    
    Revision 1.7  2004/08/18 09:30:47  yosiokat
    �EbsendBuff�̃A���C�������g��32byte�ɁB
    
    Revision 1.6  2004/08/18 09:08:32  miya
    modified WMGameInfo format
    
    Revision 1.5  2004/08/12 09:54:50  yosiokat
    MBi_MakeGameInfo�̃A�C�R���f�[�^�擾���t�@�C������ǂݏo���悤�ύX�B
    
    Revision 1.4  2004/08/12 09:17:53  yosiokat
    �E�X�L�������b�N�Ɋ�����݂���悤�ύX�B
    �EMB_DeleteRecvGameInfo�̒ǉ��B
    
    Revision 1.3  2004/08/09 14:56:43  yosizaki
    format all the symbol for API
    
    Revision 1.2  2004/08/07 15:02:37  yosizaki
    merge all the diff. (-2004/08/07)
    
    Revision 1.1  2004/08/07 07:14:51  yosizaki
    (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include "mb_gameinfo.h"
#include "mb_common.h"
#include "mb_private.h"                // for MB_SCAN_TIME_NORMAL & MB_SCAN_TIME_LOKING & MB_OUTPUT, MB_DEBUG_OUTPUT


/*

�@���������������ӎ�����������������
�@�E�X�L�������b�N���́A�X�L�������Ԃ�e�@�̃r�[�R���Ԋu�ȏ�ɂ������������Ƃ̂��ƁB�i200ms�ȏ�j
�@�@�A���A���������ꍇ�A���I�ɃX�L�������Ԃ�ς��邱�ƂɂȂ�̂ŁALifetimeCount��LockTimeCount�̍ő�l
    ����ɃJ�E���g����Ă���l���ǂ����邩�H

*/


/*
 * �g�p���Ă��� wmDEMOlib �֐�:
 *  �E����
 *
 * �g�p���Ă��� WM API:
 *  �EWM_SetGameInfo (MBi_SendFixedBeacon, MBi_SendVolatBeacon)
 *
 *
 *
 *
 */

// define data---------------------------------------------

#define MB_GAME_INFO_LIFETIME_SEC       60      // ��M�����Q�[�����̎����i����sec�l�̊ԃr�[�R����M���Ȃ���΍폜�����j
#define MB_SCAN_LOCK_SEC                8       // ����e�@�����b�N���ăX�L��������ő厞��
#define MB_SAME_BEACON_RECV_MAX_COUNT   3       // �e�@�������r�[�R�����o�������Ă����ꍇ�A���̐e�@�̃r�[�R������M��������ő�J�E���g���B

#define MB_BEACON_DATA_SIZE         (WM_SIZE_USER_GAMEINFO - 8)
#define MB_BEACON_FIXED_DATA_SIZE   (MB_BEACON_DATA_SIZE   - 6) // �r�[�R�����̐e�@�Q�[�����Œ�̈�̃f�[�^�T�C�Y
#define MB_BEACON_VOLAT_DATA_SIZE   (MB_BEACON_DATA_SIZE   - 8) // �r�[�R�����̐e�@�Q�[����񗬓��̈�̃f�[�^�T�C�Y
#define MB_SEND_MEMBER_MAX_NUM      (MB_BEACON_VOLAT_DATA_SIZE / sizeof(MBUserInfo))    // �P��̃r�[�R���ő��M�ł��郁���o�[���̍ő吔

#define FIXED_NORMAL_SIZE            sizeof(MBGameInfoFixed)    // �Q�[�����Œ�̈�̃m�[�}���T�C�Y
#define FIXED_NO_ICON_SIZE          (sizeof(MBGameInfoFixed) - sizeof(MBIconInfo))      // �Q�[�����Œ�̈�̃A�C�R���Ȃ��T�C�Y
#define FIXED_FLAGMENT_MAX(size)    ( ( size / MB_BEACON_FIXED_DATA_SIZE) + ( size % MB_BEACON_FIXED_DATA_SIZE ? 1 : 0 ) )
                                                                                    // �Q�[�����Œ�̈���r�[�R��������

#define MB_LIFETIME_MAX_COUNT       ( MB_GAME_INFO_LIFETIME_SEC * 1000 / MB_SCAN_TIME_NORMAL + 1 )
#define MB_LOCKTIME_MAX_COUNT       ( MB_SCAN_LOCK_SEC          * 1000 / MB_SCAN_TIME_NORMAL + 1 )
                                                                                    // �e�@�Q�[����񐶑����Ԃ�ScanTime�P�ʂɊ��Z��������
                                                                                    // �X�L�������b�N���ԁ@�@��ScanTime�P�ʂɊ��Z��������
#define MB_SCAN_COUNT_UNIT_NORMAL   ( 1 )
#define MB_SCAN_COUNT_UNIT_LOCKING  ( MB_SCAN_TIME_LOCKING / MB_SCAN_TIME_NORMAL )



typedef enum MbBeaconState
{
    MB_BEACON_STATE_STOP = 0,
    MB_BEACON_STATE_READY,
    MB_BEACON_STATE_FIXED_START,
    MB_BEACON_STATE_FIXED,
    MB_BEACON_STATE_VOLAT_START,
    MB_BEACON_STATE_VOLAT,
    MB_BEACON_STATE_NEXT_GAME
}
MbBeaconState;


/* �r�[�R���t�H�[�}�b�g�\���� */
typedef struct MbBeacon
{
    u32     ggid;                      // GGID
    u8      dataAttr:2;                // �f�[�^�����iMbBeaconDataAttr�Ŏw��j
    u8      fileNo:6;                  // �t�@�C��No.
    u8      seqNoFixed;                // Fixed�f�[�^�V�[�P���X�ԍ��i�f�[�^���e�ɍX�V���������ꍇ�ɉ��Z�����B�j
    u8      seqNoVolat;                // Volat�f�[�^�V�[�P���X�ԍ��i�@�V�@�j
    u8      beaconNo;                  // �r�[�R���i���o�[�i�r�[�R�����M���ƂɃC���N�������g�j
    /* �� MbBeaconData */
    union
    {
        struct
        {                              // MBGameInfoFixed���M��
            u16     sum;               // 16bit checksum
            u8      flagmentNo;        // �f�Љ��f�[�^�̌��ݔԍ�
            u8      flagmentMaxNum;    // �f�Љ��f�[�^�̍ő吔
            u8      size;              // �f�[�^�T�C�Y
            u8      rsv;
            u8      data[MB_BEACON_FIXED_DATA_SIZE];    // �f�[�^����
        }
        fixed;
        struct
        {                              // MBGameInfoVolatile���M��
            u16     sum;               // 16bit checksum
            u8      nowPlayerNum;      // ���݂̃v���C���[��
            u8      pad[1];
            u16     nowPlayerFlag;     // ���݂̑S�v���C���[�̃v���C���[�ԍ����r�b�g�Ŏ����B
            u16     changePlayerFlag;  // ����̍X�V�ŕύX���ꂽ�v���C���[���̔ԍ����r�b�g�Ŏ����B�i�V�[�P���X�ԍ����ς�����u�Ԃɂ̂ݔ���Ɏg�p�j
            MBUserInfo member[MB_SEND_MEMBER_MAX_NUM];  // �e�q�@�̃��[�U�[����MB_SEND_MEMBER_MAX_NUM�����M�iPlayerNo == 15�i�e�@�j �Ȃ�I�[�B�j
            u8      userVolatData[MB_USER_VOLAT_DATA_SIZE];     // ���[�U���Z�b�g�ł���f�[�^
        }
        volat;
    }
    data;

}
MbBeacon;

/* �e�@���r�[�R�����M�X�e�[�^�X�\���� */
typedef struct MbBeaconSendStatus
{
    MBGameInfo *gameInfoListTop;       // �Q�[����񃊃X�g�擪�ւ̃|�C���^�i�Е������X�g�j
    MBGameInfo *nowGameInfop;          // ���ݑ��M���̃Q�[�����ւ̃|�C���^
    u8     *srcp;                      // ���݂̑��M���̃Q�[�����\�[�X�ւ̃|�C���^
    u8      state;                     // �r�[�R�����M�X�e�[�g�i�Œ�f�[�^���A�����f�[�^�����M�B�e�f�[�^��S���M������X�e�[�g�ύX�B�j
    u8      seqNoFixed;                // �Œ�̈�̃V�[�P���X�ԍ�
    u8      seqNoVolat;                // �����̈�̃V�[�P���X�ԍ�
    u8      flagmentNo;                // �@�@�V�@�@�@�@�@�@�̃t���O�����g�ԍ��i�Œ�̈�̏ꍇ�j
    u8      flagmentMaxNum;            // �@�@�V�@�@�@�@�@�@�̃t���O�����g���@�i�V�j
    u8      beaconNo;
    u8      pad[2];
}
MbBeaconSendStatus;



// function's prototype------------------------------------
static BOOL MBi_ReadIconInfo(const char *filePathp, MBIconInfo *iconp, BOOL char_flag);
static void MBi_ClearSendStatus(void);
static BOOL MBi_ReadyBeaconSendStatus(void);
static void MBi_InitSendFixedBeacon(void);
static void MBi_SendFixedBeacon(u32 ggid, u16 tgid, u8 attribute);
static void MBi_InitSendVolatBeacon(void);
static void MBi_SendVolatBeacon(u32 ggid, u16 tgid, u8 attribute);

static BOOL MBi_CheckMBParent(WMBssDesc *bssDescp);
static void MBi_SetSSIDToBssDesc(WMBssDesc *bssDescp, u32 ggid);
static int MBi_GetStoreElement(WMBssDesc *bssDescp, MBBeaconMsgCallback Callbackp);
static void MBi_CheckCompleteGameInfoFlagments(int index, MBBeaconMsgCallback Callbackp);
static void MBi_AnalyzeBeacon(WMBssDesc *bssDescp, int index, u16 linkLevel);
static void MBi_CheckTGID(WMBssDesc *bssDescp, int inex);
static void MBi_CheckSeqNoFixed(int index);
static void MBi_CheckSeqNoVolat(int index);
static void MBi_InvalidateGameInfoBssID(u8 *bssidp);
static void MBi_RecvFixedBeacon(int index);
static void MBi_RecvVolatBeacon(int index);

static void MBi_LockScanTarget(int index);
static void MBi_UnlockScanTarget(void);
static int mystrlen(u16 *str);


// const data----------------------------------------------

// global variables----------------------------------------

static MbBeaconSendStatus mbss;        // �r�[�R�����M�X�e�[�^�X

static MbBeaconRecvStatus mbrs;        // �r�[�R����M�X�e�[�^�X
static MbBeaconRecvStatus *mbrsp = NULL;        // �r�[�R����M�X�e�[�^�X

// static variables----------------------------------------
static MbScanLockFunc sLockFunc = NULL; // �X�L�������b�N�ݒ�֐��|�C���^
static MbScanUnlockFunc sUnlockFunc = NULL;     // �X�L�������b�N�����p�֐��|�C���^

static MbBeacon bsendBuff ATTRIBUTE_ALIGN(32);  // �r�[�R�����M�o�b�t�@
static MbBeacon *brecvBuffp;           // �r�[�R����M�o�b�t�@
static WMBssDesc bssDescbuf ATTRIBUTE_ALIGN(32);        // BssDesc�̃e���|�����o�b�t�@

static MBSendVolatCallbackFunc sSendVolatCallback = NULL;       // ���[�U�f�[�^���M�R�[���o�b�N
static u32 sSendVolatCallbackTimming;  // ���M�R�[���o�b�N�����^�C�~���O

// function's description-----------------------------------------------


//=========================================================
// �r�[�R����M���[�N�o�b�t�@����
//=========================================================
// �ݒ肳��Ă��郏�[�N�o�b�t�@���擾
const MbBeaconRecvStatus *MB_GetBeaconRecvStatus(void)
{
    return mbrsp;
}

// static�ϐ����烏�[�N�o�b�t�@��ݒ�(���݊��p)
void MBi_SetBeaconRecvStatusBufferDefault(void)
{
    mbrsp = &mbrs;
}

// �r�[�R����M���[�N�o�b�t�@�̐ݒ�
void MBi_SetBeaconRecvStatusBuffer(MbBeaconRecvStatus * buf)
{
    mbrsp = buf;
}


// �X�L�������b�N�p�֐���ݒ�
void MBi_SetScanLockFunc(MbScanLockFunc lock, MbScanUnlockFunc unlock)
{
    sLockFunc = lock;
    sUnlockFunc = unlock;
}



//=========================================================
// �e�@�ɂ��Q�[�����̃r�[�R�����M
//=========================================================

// �r�[�R���ő��M����e�@�Q�[�����̐���
void MBi_MakeGameInfo(MBGameInfo *gameInfop,
                      const MBGameRegistry *mbGameRegp, const MBUserInfo *parent)
{
    BOOL    icon_disable;

    // �Ƃ肠�����S�N���A
    MI_CpuClear16(gameInfop, sizeof(MBGameInfo));

    // �A�C�R���f�[�^�̓o�^
    gameInfop->dataAttr = MB_BEACON_DATA_ATTR_FIXED_NORMAL;
    icon_disable = !MBi_ReadIconInfo(mbGameRegp->iconCharPathp, &gameInfop->fixed.icon, TRUE);
    icon_disable |= !MBi_ReadIconInfo(mbGameRegp->iconPalettePathp, &gameInfop->fixed.icon, FALSE);

    if (icon_disable)
    {
        gameInfop->dataAttr = MB_BEACON_DATA_ATTR_FIXED_NO_ICON;
        MI_CpuClearFast(&gameInfop->fixed.icon, sizeof(MBIconInfo));
    }

    // GGID
    gameInfop->ggid = mbGameRegp->ggid;

    // �e�̃��[�U�[���o�^
    if (parent != NULL)
    {
        MI_CpuCopy16(parent, &gameInfop->fixed.parent, sizeof(MBUserInfo));
    }

    // �ő�v���C�l���o�^
    gameInfop->fixed.maxPlayerNum = mbGameRegp->maxPlayerNum;

    // �Q�[�������Q�[�����e�����o�^
    {
        int     len;

#if defined(MB_CHANGE_TO_UNICODE)
#define COPY_GAME_INFO_STRING   MBi_ExSJIS_BEtoUTF16_LE
#else  /* defined(MB_CHANGE_TO_UNICODE) */
#define COPY_GAME_INFO_STRING   MI_CpuCopy16
#endif /* defined(MB_CHANGE_TO_UNICODE) */

        len = mystrlen(mbGameRegp->gameNamep) << 1;
        COPY_GAME_INFO_STRING((u8 *)mbGameRegp->gameNamep, gameInfop->fixed.gameName, (u16)len);
        // �Q�[�����e�����̌�Ƀf�[�^�𖄂ߍ��߂�悤�ɁANUL�����ȍ~�̃f�[�^�����M����B
        len = MB_GAME_INTRO_LENGTH * 2;
        COPY_GAME_INFO_STRING((u8 *)mbGameRegp->gameIntroductionp,
                              gameInfop->fixed.gameIntroduction, (u16)len);
    }

    // �e�@���̃v���C���[����o�^
    gameInfop->volat.nowPlayerNum = 1;
    gameInfop->volat.nowPlayerFlag = 0x0001;    // �e�@�̃v���C���[�ԍ���0
    gameInfop->broadcastedPlayerFlag = 0x0001;

    // ���}���`�u�[�g�J�n���ɂ̓v���C�����o�[�͂��Ȃ��̂ŁA�o�^�͂Ȃ��B
}


// �A�C�R���f�[�^�̃��[�h
static BOOL MBi_ReadIconInfo(const char *filePathp, MBIconInfo *iconp, BOOL char_flag)
{
    FSFile  file;
    s32     size = char_flag ? MB_ICON_DATA_SIZE : MB_ICON_PALETTE_SIZE;
    u16    *dstp = char_flag ? iconp->data : iconp->palette;

    if (filePathp == NULL)
    {                                  // �t�@�C���w�肪�Ȃ����FALSE���^�[��
        return FALSE;
    }

    FS_InitFile(&file);

    if (FS_OpenFile(&file, filePathp) == FALSE)
    {                                  // �t�@�C�����J���Ȃ�������FALSE���^�[��
        MB_DEBUG_OUTPUT("\t%s : file open error.\n", filePathp);
        return FALSE;
    }
    else if ((u32)size != FS_GetLength(&file))
    {
        MB_DEBUG_OUTPUT("\t%s : different file size.\n", filePathp);
        (void)FS_CloseFile(&file);
        return FALSE;
    }

    (void)FS_ReadFile(&file, dstp, size);
    (void)FS_CloseFile(&file);
    return TRUE;
}


// �Q�[�����̗����������X�V����B
void MB_UpdateGameInfoMember(MBGameInfo *gameInfop,
                             const MBUserInfo *member, u16 nowPlayerFlag, u16 changePlayerFlag)
{
    int     i;
    u8      playerNum = 1;

    MI_CpuCopy16(member, &gameInfop->volat.member[0], sizeof(MBUserInfo) * MB_MEMBER_MAX_NUM);
    /* �q�@�����J�E���g */
    for (i = 0; i < MB_MEMBER_MAX_NUM; i++)
    {
        if (nowPlayerFlag & (0x0002 << i))
        {
            playerNum++;
        }
    }
    gameInfop->volat.nowPlayerNum = playerNum;
    gameInfop->volat.nowPlayerFlag = (u16)(nowPlayerFlag | 0x0001);
    gameInfop->volat.changePlayerFlag = changePlayerFlag;
    gameInfop->seqNoVolat++;
}


// �����񒷂̎Z�o
static int mystrlen(u16 *str)
{
    int     len = 0;
    while (*str++)
        len++;
    return len;
}


// �Q�[�����𑗐M���X�g�ɒǉ�
void MB_AddGameInfo(MBGameInfo *newGameInfop)
{
    MBGameInfo *gInfop = mbss.gameInfoListTop;

    if (!gInfop)
    {                                  // mbss�̐擪��NULL�Ȃ�擪�ɓo�^�B
        mbss.gameInfoListTop = newGameInfop;
    }
    else
    {                                  // �����Ȃ��΁A���X�g��H���čs���A�Ō�ɒǉ��B
        while (gInfop->nextp != NULL)
        {
            gInfop = gInfop->nextp;
        }
        gInfop->nextp = newGameInfop;
    }
    newGameInfop->nextp = NULL;        // �ǉ������Q�[������next���I�[�Ƃ���B
}


// �Q�[�����𑗐M���X�g����폜
BOOL MB_DeleteGameInfo(MBGameInfo *gameInfop)
{
    MBGameInfo *gInfop = mbss.gameInfoListTop;
    MBGameInfo *before;

    while (gInfop != NULL)
    {
        if (gInfop != gameInfop)
        {
            before = gInfop;
            gInfop = gInfop->nextp;
            continue;
        }

        // ��v�����̂Ń��X�g����폜
        if ((u32)gInfop == (u32)mbss.gameInfoListTop)
        {                              // ���X�g�擪���폜����ꍇ
            mbss.gameInfoListTop = mbss.gameInfoListTop->nextp;
        }
        else
        {
            before->nextp = gInfop->nextp;
        }

        if ((u32)gameInfop == (u32)mbss.nowGameInfop)   // �폜����Q�[����񂪌��ݑ��M���̏ꍇ�A���ݑ��M����
        {                              // �Q�[�������č\�z���ꂽ���X�g����Đݒ肷��B
            mbss.nowGameInfop = NULL;
            if (!MBi_ReadyBeaconSendStatus())
            {
                mbss.state = MB_BEACON_STATE_READY;     // �Q�[����񂪑S�č폜����Ă����Ȃ�΁A�X�e�[�^�X��READY�ɁB
            }
        }
        return TRUE;
    }

    // �Q�[����񂪂Ȃ��ꍇ
    return FALSE;
}


// �Q�[����񑗐M�ݒ�̏�����
void MB_InitSendGameInfoStatus(void)
{
    mbss.gameInfoListTop = NULL;       // �Q�[����񃊃X�g��S�폜�B
    mbss.nowGameInfop = NULL;          // �����M�Q�[�����폜�B
    mbss.state = MB_BEACON_STATE_READY;
    sSendVolatCallback = NULL;
    MBi_ClearSendStatus();             // ���M�X�e�[�^�X���N���A�B
}


// ���M�X�e�[�^�X�̃N���A
static void MBi_ClearSendStatus(void)
{
    mbss.seqNoFixed = 0;
    mbss.seqNoVolat = 0;
    mbss.flagmentNo = 0;
    mbss.flagmentMaxNum = 0;
    mbss.beaconNo = 0;
}


// �r�[�R�����M
void MB_SendGameInfoBeacon(u32 ggid, u16 tgid, u8 attribute)
{
    while (1)
    {
        switch (mbss.state)
        {
        case MB_BEACON_STATE_STOP:
        case MB_BEACON_STATE_READY:
            if (!MBi_ReadyBeaconSendStatus())
            {                          // �Q�[�����̑��M����
                return;
            }
            break;
        case MB_BEACON_STATE_FIXED_START:
            MBi_InitSendFixedBeacon();
            break;
        case MB_BEACON_STATE_FIXED:
            MBi_SendFixedBeacon(ggid, tgid, attribute);
            return;
        case MB_BEACON_STATE_VOLAT_START:
            MBi_InitSendVolatBeacon();
            break;
        case MB_BEACON_STATE_VOLAT:
            MBi_SendVolatBeacon(ggid, tgid, attribute);
            return;
        case MB_BEACON_STATE_NEXT_GAME:
            break;
        }
    }
}


// �r�[�R�����M�X�e�[�^�X�𑗐M�\�ɏ�������B
static BOOL MBi_ReadyBeaconSendStatus(void)
{
    MBGameInfo *nextGameInfop;

    // �Q�[����񂪓o�^����Ă��Ȃ��Ȃ�A�G���[���^�[���B
    if (!mbss.gameInfoListTop)
    {
        /* MB �t���O�� ENTRY �t���O�͂����ŗ��Ƃ� */
        (void)WM_SetGameInfo(NULL, (u16 *)&bsendBuff, WM_SIZE_USER_GAMEINFO, MBi_GetGgid(),
                             MBi_GetTgid(), WM_ATTR_FLAG_CS);
        return FALSE;
    }

    // ���ɑ��M����GameInfo�̑I��
    if (!mbss.nowGameInfop || !mbss.nowGameInfop->nextp)
    {
        // �܂����M���̃Q�[����񂪂Ȃ����A���X�g�̍Ō�̏ꍇ�́A���X�g�̐擪�𑗐M����悤�����B
        nextGameInfop = mbss.gameInfoListTop;
    }
    else
    {
        // �����Ȃ��΁A���X�g�̎��̃Q�[���𑗐M����悤�����B
        nextGameInfop = mbss.nowGameInfop->nextp;
    }

    mbss.nowGameInfop = nextGameInfop;

    MBi_ClearSendStatus();
    mbss.seqNoVolat = mbss.nowGameInfop->seqNoVolat;

    mbss.state = MB_BEACON_STATE_FIXED_START;

    return TRUE;
}


// �Q�[�����̌Œ�f�[�^���̑��M������
static void MBi_InitSendFixedBeacon(void)
{
    if (mbss.state != MB_BEACON_STATE_FIXED_START)
    {
        return;
    }

    if (mbss.nowGameInfop->dataAttr == MB_BEACON_DATA_ATTR_FIXED_NORMAL)
    {
        mbss.flagmentMaxNum = FIXED_FLAGMENT_MAX(FIXED_NORMAL_SIZE);
        mbss.srcp = (u8 *)&mbss.nowGameInfop->fixed;
    }
    else
    {
        mbss.flagmentMaxNum = FIXED_FLAGMENT_MAX(FIXED_NO_ICON_SIZE);
        mbss.srcp = (u8 *)&mbss.nowGameInfop->fixed.parent;
    }
    mbss.state = MB_BEACON_STATE_FIXED;
}


// �Q�[�����̌Œ�f�[�^���𕪊����ăr�[�R�����M
static void MBi_SendFixedBeacon(u32 ggid, u16 tgid, u8 attribute)
{
    u32     lastAddr = (u32)mbss.nowGameInfop + sizeof(MBGameInfoFixed);

    if ((u32)mbss.srcp + MB_BEACON_FIXED_DATA_SIZE <= lastAddr)
    {
        bsendBuff.data.fixed.size = MB_BEACON_FIXED_DATA_SIZE;
    }
    else
    {
        bsendBuff.data.fixed.size = (u8)(lastAddr - (u32)mbss.srcp);
        MI_CpuClear16((void *)((u8 *)bsendBuff.data.fixed.data + bsendBuff.data.fixed.size),
                      (u32)(MB_BEACON_FIXED_DATA_SIZE - bsendBuff.data.fixed.size));
    }

    // �r�[�R�����M�o�b�t�@�ɃZ�b�g
    MB_DEBUG_OUTPUT("send flagment= %2d  adr = 0x%x\n", mbss.flagmentNo, mbss.srcp);
    MI_CpuCopy16(mbss.srcp, bsendBuff.data.fixed.data, bsendBuff.data.fixed.size);
    bsendBuff.data.fixed.flagmentNo = mbss.flagmentNo;
    bsendBuff.data.fixed.flagmentMaxNum = mbss.flagmentMaxNum;
    bsendBuff.dataAttr = mbss.nowGameInfop->dataAttr;
    bsendBuff.seqNoFixed = mbss.nowGameInfop->seqNoFixed;
    bsendBuff.seqNoVolat = mbss.seqNoVolat;
    bsendBuff.ggid = mbss.nowGameInfop->ggid;
    bsendBuff.fileNo = mbss.nowGameInfop->fileNo;
    bsendBuff.beaconNo = mbss.beaconNo++;
    bsendBuff.data.fixed.sum = 0;
    bsendBuff.data.fixed.sum = MBi_calc_cksum((u16 *)&bsendBuff.data, MB_BEACON_DATA_SIZE);

    // ���M�X�e�[�^�X�̍X�V
    mbss.flagmentNo++;
    if (mbss.flagmentNo < mbss.flagmentMaxNum)
    {
        mbss.srcp += MB_BEACON_FIXED_DATA_SIZE;
    }
    else
    {
        mbss.state = MB_BEACON_STATE_VOLAT_START;
    }

    /* �e�@�v���O�������g�̂�GGID��o�^���� */
    (void)WM_SetGameInfo(NULL, (u16 *)&bsendBuff, WM_SIZE_USER_GAMEINFO, ggid, tgid,
                         (u8)(attribute | WM_ATTR_FLAG_MB | WM_ATTR_FLAG_ENTRY));
    // �}���`�u�[�g�t���O�͂����ŗ��Ă�B
}


// �Q�[�����̗����f�[�^���̑��M������
static void MBi_InitSendVolatBeacon(void)
{
    mbss.nowGameInfop->broadcastedPlayerFlag = 0x0001;
    mbss.seqNoVolat = mbss.nowGameInfop->seqNoVolat;
    mbss.state = MB_BEACON_STATE_VOLAT;
}


// �Q�[�����̗����f�[�^���𕪊����ăr�[�R�����M
static void MBi_SendVolatBeacon(u32 ggid, u16 tgid, u8 attribute)
{
    int     i;
    int     setPlayerNum;
    u16     remainPlayerFlag;

    if (mbss.seqNoVolat != mbss.nowGameInfop->seqNoVolat)
    {                                  // ���M���Ƀf�[�^�X�V���������瑗�蒼���B
        MBi_InitSendVolatBeacon();
    }

    // �r�[�R�������Z�b�g
    bsendBuff.dataAttr = MB_BEACON_DATA_ATTR_VOLAT;
    bsendBuff.seqNoFixed = mbss.nowGameInfop->seqNoFixed;
    bsendBuff.seqNoVolat = mbss.seqNoVolat;
    bsendBuff.ggid = mbss.nowGameInfop->ggid;
    bsendBuff.fileNo = mbss.nowGameInfop->fileNo;
    bsendBuff.beaconNo = mbss.beaconNo++;

    // ���݂̃v���C���[���̏�Ԃ��Z�b�g
    bsendBuff.data.volat.nowPlayerNum = mbss.nowGameInfop->volat.nowPlayerNum;
    bsendBuff.data.volat.nowPlayerFlag = mbss.nowGameInfop->volat.nowPlayerFlag;
    bsendBuff.data.volat.changePlayerFlag = mbss.nowGameInfop->volat.changePlayerFlag;

    // �A�v���ݒ�f�[�^���Z�b�g
    if (sSendVolatCallbackTimming == MB_SEND_VOLAT_CALLBACK_TIMMING_BEFORE
        && sSendVolatCallback != NULL)
    {
        sSendVolatCallback(mbss.nowGameInfop->ggid);
    }

    for (i = 0; i < MB_USER_VOLAT_DATA_SIZE; i++)
    {
        bsendBuff.data.volat.userVolatData[i] = mbss.nowGameInfop->volat.userVolatData[i];
    }

    MB_DEBUG_OUTPUT("send PlayerFlag = %x\n", mbss.nowGameInfop->volat.nowPlayerFlag);

    // ���񑗐M����v���C���[���̃Z�b�g
    MI_CpuClear16(&bsendBuff.data.volat.member[0], sizeof(MBUserInfo) * MB_SEND_MEMBER_MAX_NUM);
    setPlayerNum = 0;
    remainPlayerFlag =
        (u16)(mbss.nowGameInfop->broadcastedPlayerFlag ^ mbss.nowGameInfop->volat.nowPlayerFlag);
    for (i = 0; i < MB_MEMBER_MAX_NUM; i++)
    {
        if ((remainPlayerFlag & (0x0002 << i)) == 0)
        {
            continue;
        }

        MB_DEBUG_OUTPUT("  member %d set.\n", i);

        MI_CpuCopy16(&mbss.nowGameInfop->volat.member[i],
                     &bsendBuff.data.volat.member[setPlayerNum], sizeof(MBUserInfo));
        mbss.nowGameInfop->broadcastedPlayerFlag |= 0x0002 << i;
        if (++setPlayerNum == MB_SEND_MEMBER_MAX_NUM)
        {
            break;
        }
    }
    if (setPlayerNum < MB_SEND_MEMBER_MAX_NUM)
    {                                  // �ő呗�M���ɒB���Ȃ��������ɂ͏I�[����B
        bsendBuff.data.volat.member[setPlayerNum].playerNo = 0;
    }

    // �`�F�b�N�T���̃Z�b�g
    bsendBuff.data.volat.sum = 0;
    bsendBuff.data.volat.sum = MBi_calc_cksum((u16 *)&bsendBuff.data, MB_BEACON_DATA_SIZE);


    // ���M�I���`�F�b�N
    if (mbss.nowGameInfop->broadcastedPlayerFlag == mbss.nowGameInfop->volat.nowPlayerFlag)
    {
        mbss.state = MB_BEACON_STATE_READY;     // �S���𑗐M���I�������A���̃Q�[�����𑗐M���邽�߂̏����X�e�[�g�ցB
    }

    /* �e�@�v���O�������g�̂�GGID��o�^���� */

    (void)WM_SetGameInfo(NULL, (u16 *)&bsendBuff, WM_SIZE_USER_GAMEINFO, ggid, tgid,
                         (u8)(attribute | WM_ATTR_FLAG_MB | WM_ATTR_FLAG_ENTRY));
    // �}���`�u�[�g�t���O�͂����ŗ��Ă�B

    if (sSendVolatCallbackTimming == MB_SEND_VOLAT_CALLBACK_TIMMING_AFTER
        && sSendVolatCallback != NULL)
    {
        sSendVolatCallback(mbss.nowGameInfop->ggid);
    }

}


//=========================================================
// �q�@�ɂ��Q�[�����̃r�[�R����M
//=========================================================

// �Q�[������M�X�e�[�^�X�̏�����
void MB_InitRecvGameInfoStatus(void)
{
    MI_CpuClearFast(mbrsp, sizeof(MbBeaconRecvStatus));

    mbrsp->scanCountUnit = MB_SCAN_COUNT_UNIT_NORMAL;
}


// �r�[�R����M
BOOL MB_RecvGameInfoBeacon(MBBeaconMsgCallback Callbackp, u16 linkLevel, WMBssDesc *bssDescp)
{
    int     index;

    // ����擾�����r�[�R�����A�}���`�u�[�g�e�@���ǂ����𔻒�B
    if (!MBi_CheckMBParent(bssDescp))
    {
        return FALSE;
    }

    /* �擾����bssDesc���e���|�����o�b�t�@�փR�s�[ */
    MI_CpuCopy16(bssDescp, &bssDescbuf, sizeof(WMBssDesc));

    /* bssDescp�����[�J���o�b�t�@�ɐ؂�ւ���B */
    bssDescp = &bssDescbuf;

    brecvBuffp = (MbBeacon *) bssDescp->gameInfo.userGameInfo;

    // �`�F�b�N�T���m�F�B
    if (MBi_calc_cksum((u16 *)&brecvBuffp->data, MB_BEACON_DATA_SIZE))
    {
        MB_DEBUG_OUTPUT("Beacon checksum error!\n");
        return FALSE;
    }

    // �e�@�Q�[�����̊i�[�ꏊ�𔻒�i���ɓ���e�@�̃f�[�^����M���Ă���Ȃ�A�����ɑ����Ċi�[����j
    index = MBi_GetStoreElement(bssDescp, Callbackp);
    if (index < 0)
    {
        return FALSE;                  // �i�[�ꏊ���Ȃ��̂ŃG���[���^�[��
    }
    MB_DEBUG_OUTPUT("GameInfo Index:%6d\n", index);

    // �r�[�R�����
    MBi_AnalyzeBeacon(bssDescp, index, linkLevel);

    // �e�@�Q�[�����̑S�Ă̒f�Ђ������������肵�A�R�[���o�b�N�Œʒm
    MBi_CheckCompleteGameInfoFlagments(index, Callbackp);

    return TRUE;
}


// ����擾�����r�[�R�����}���`�u�[�g�e�@���ǂ����𔻒肷��B
static BOOL MBi_CheckMBParent(WMBssDesc *bssDescp)
{
    // �}���`�u�[�g�e�@���ǂ����𔻒肷��B
    if ((bssDescp->gameInfo.magicNumber != WM_GAMEINFO_MAGIC_NUMBER)
        || !(bssDescp->gameInfo.attribute & WM_ATTR_FLAG_MB))
    {
        MB_DEBUG_OUTPUT("not MB parent : %x%x\n",
                        *(u16 *)(&bssDescp->bssid[4]), *(u32 *)bssDescp->bssid);
        return FALSE;
    }
    else
    {

        MB_DEBUG_OUTPUT("MB parent     : %x%x",
                        *(u16 *)(&bssDescp->bssid[4]), *(u32 *)bssDescp->bssid);
        return TRUE;
    }
}


// bssDesc��SSID���Z�b�g����B
static void MBi_SetSSIDToBssDesc(WMBssDesc *bssDescp, u32 ggid)
{
    /* 
       SSID�̐ݒ� 

       �_�E�����[�h�A�v���ŗL��GGID�ƁA�}���`�u�[�g�e�@��TGID����
       SSID�𐶐�����B
       �q�@�́A����SSID��p���Đe�@�A�v���P�[�V�����ƍĐڑ����s���B
     */
    bssDescp->ssidLength = 32;
    ((u16 *)bssDescp->ssid)[0] = (u16)(ggid & 0x0000ffff);
    ((u16 *)bssDescp->ssid)[1] = (u16)((ggid & 0xffff0000) >> 16);
    ((u16 *)bssDescp->ssid)[2] = bssDescp->gameInfo.tgid;
}


// bssDesc�����ƂɎ�M���X�g�v�f�̂ǂ��Ɋi�[���邩���擾����B
static int MBi_GetStoreElement(WMBssDesc *bssDescp, MBBeaconMsgCallback Callbackp)
{
    int     i;

    // ���ɂ��̐e�@�̓���Q�[��������M���Ă��邩�ǂ����𔻒�i "GGID", "BSSID", "fileNo"�̂R����v����Ȃ�A����Q�[�����Ɣ��f����j
    for (i = 0; i < MB_GAME_INFO_RECV_LIST_NUM; i++)
    {
        MBGameInfoRecvList *info = &mbrsp->list[i];

        if ((mbrsp->usingGameInfoFlag & (0x01 << i)) == 0)
        {
            continue;
        }
        // GGID����v���邩�H
        if (info->gameInfo.ggid != brecvBuffp->ggid)
        {
            continue;
        }
        // MAC�A�h���X����v���邩�H
        if (!WM_IsBssidEqual(info->bssDesc.bssid, bssDescp->bssid))
        {
            continue;
        }
        // �t�@�C��No.����v���邩�H
        if (mbrsp->list[i].gameInfo.fileNo != brecvBuffp->fileNo)
        {
            continue;
        }

        // =========================================
        // ���̐e�@���ɑ΂��āA���Ɏ�M�ꏊ���m�ۂ���Ă���Ɣ��f�B
        // =========================================
        if (!(mbrsp->validGameInfoFlag & (0x01 << i)))
        {
            MBi_LockScanTarget(i);     // �܂��Y���e�@�̏�񂪑����Ă��Ȃ���΁A�X�L�������b�N����B
        }
        return i;
    }

    // �܂���M���Ă��Ȃ��̂ŁA���X�g��NULL�ʒu��T���āA�������i�[�ꏊ�ɂ���B
    for (i = 0; i < MB_GAME_INFO_RECV_LIST_NUM; i++)
    {
        if (mbrsp->usingGameInfoFlag & (0x01 << i))
        {
            continue;
        }

        MI_CpuCopy16(bssDescp, &mbrsp->list[i].bssDesc, sizeof(WMBssDesc));
        // BssDesc���R�s�[
        mbrsp->list[i].gameInfo.seqNoFixed = brecvBuffp->seqNoFixed;
        mbrsp->usingGameInfoFlag |= (u16)(0x01 << i);

        MB_DEBUG_OUTPUT("\n");
        // ���̐e�@�݂̂�Scan�ΏۂɃ��b�N����B
        MBi_LockScanTarget(i);
        return i;
    }

    // �i�[�ꏊ���S�Ė��܂��Ă����ꍇ�̓R�[���o�b�N�Œʒm���ăG���[���^�[��
    if (Callbackp != NULL)
    {
        Callbackp(MB_BC_MSG_GINFO_LIST_FULL, NULL, 0);
    }
    return -1;
}


// �e�@�Q�[�����̑S�Ă̒f�Ђ������������肵�A�R�[���o�b�N�Œʒm
static void MBi_CheckCompleteGameInfoFlagments(int index, MBBeaconMsgCallback Callbackp)
{
    MBGameInfoRecvList *info = &mbrsp->list[index];

    /* ������ɂ���A�v���P�[�V�����֐e�@������ʒm���� */
    if (Callbackp != NULL)
    {
        Callbackp(MB_BC_MSG_GINFO_BEACON, info, index);
    }
    if ((info->getFlagmentFlag == info->allFlagmentFlag) && (info->getFlagmentFlag)     // �V�����e�@�Q�[����񂪑������ꍇ
        && (info->gameInfo.volat.nowPlayerFlag)
        && (info->getPlayerFlag == info->gameInfo.volat.nowPlayerFlag))
    {
        if (mbrsp->validGameInfoFlag & (0x01 << index))
        {
            return;
        }
        mbrsp->validGameInfoFlag |= 0x01 << index;
        mbrsp->usefulGameInfoFlag |= 0x01 << index;
        MBi_UnlockScanTarget();        // Scan�Ώۂ̃��b�N����������B
        MB_DEBUG_OUTPUT("validated ParentInfo = %d\n", index);
        if (Callbackp != NULL)
        {
            Callbackp(MB_BC_MSG_GINFO_VALIDATED, info, index);
        }
    }
    else
    {                                  // ���Ɏ擾���Ă����e�@�Q�[����񂪍X�V����āA�s���S�ȏ�ԂɂȂ����ꍇ
        if ((mbrsp->validGameInfoFlag & (0x01 << index)) == 0)
        {
            return;
        }
        mbrsp->validGameInfoFlag ^= (0x01 << index);
        MB_DEBUG_OUTPUT("Invaldated ParentInfo = %d\n", index);
        if (Callbackp != NULL)
        {
            Callbackp(MB_BC_MSG_GINFO_INVALIDATED, info, index);
        }
    }
}


// �e�@�̃Q�[�����̎����J�E���g�i���łɃX�L�������b�N���Ԃ̃J�E���g���s���j
void MB_CountGameInfoLifetime(MBBeaconMsgCallback Callbackp, BOOL found_parent)
{
    int     i;
    BOOL    unlock = FALSE;

    // �Q�[������M���X�g�̎�������@���@�V�K���b�N�Ώۃ^�[�Q�b�g�̗L�����m�F
    for (i = 0; i < MB_GAME_INFO_RECV_LIST_NUM; i++)
    {
        MBGameInfoRecvList *info = &mbrsp->list[i];
        u16     mask = (u16)(0x0001 << i);
        if ((mbrsp->usingGameInfoFlag & mask) == 0)
        {
            continue;
        }
        // ��������
        info->lifetimeCount -= mbrsp->scanCountUnit;
        if (info->lifetimeCount >= 0)
        {
            continue;
        }
        info->lifetimeCount = 0;
        if (mbrsp->validGameInfoFlag & mask)
        {
            if (Callbackp != NULL)
            {
                Callbackp(MB_BC_MSG_GINFO_LOST, info, i);
            }
        }
        if (mbrsp->nowScanTargetFlag & mask)
        {
            unlock = TRUE;
        }
        mbrsp->usingGameInfoFlag &= ~mask;
        MB_DeleteRecvGameInfo(i);      // �R�[���o�b�N�ʒm��ɃQ�[�������폜����B
        MB_DEBUG_OUTPUT("gameInfo %2d : lifetime end.\n", i);
    }

    // �X�L�������b�N���Ԃ̃J�E���g
    if (mbrsp->nowScanTargetFlag && mbrsp->nowLockTimeCount > 0)
    {
        mbrsp->nowLockTimeCount -= mbrsp->scanCountUnit;        // �X�L�������b�N���Ԃ��^�C���A�E�g������A���b�N���������Ď��̃��b�N�Ώۂ�T���B
        if (mbrsp->nowLockTimeCount < 0)
        {
            MB_DEBUG_OUTPUT("scan lock time up!\n");
            unlock = TRUE;
        }
        else if (!found_parent)
        {
            if (++mbrsp->notFoundLockTargetCount > 4)
            {
                MB_DEBUG_OUTPUT("scan lock target not found!\n");
                unlock = TRUE;
            }
        }
        else
        {
            mbrsp->notFoundLockTargetCount = 0;
        }
    }

    // �X�L�����A�����b�N�����B
    if (unlock)
    {
        mbrsp->nowLockTimeCount = 0;
        MBi_UnlockScanTarget();
    }
}


// �r�[�R�����
static void MBi_AnalyzeBeacon(WMBssDesc *bssDescp, int index, u16 linkLevel)
{
    MBi_CheckTGID(bssDescp, index);    // TGID�̃`�F�b�N
    MBi_CheckSeqNoFixed(index);        // seqNoFixed�̃`�F�b�N
    MBi_CheckSeqNoVolat(index);        // seqNoVolat�̃`�F�b�N

    // ��M�r�[�R���̋��ʕ����̃f�[�^�擾
    {
        MBGameInfoRecvList *info = &mbrsp->list[index];

        // �����r�[�R�����o��������ُ�ȏ�Ԃ̐e�@�����O����B
        if (info->beaconNo == brecvBuffp->beaconNo)
        {
            if (++info->sameBeaconRecvCount > MB_SAME_BEACON_RECV_MAX_COUNT)
            {
                info->lifetimeCount = 0;        // �e�@���̎������[���ɂ��āA�폜������B
                MB_OUTPUT("The parent broadcast same beacon.: %d\n", index);
                MBi_InvalidateGameInfoBssID(&info->bssDesc.bssid[0]);
                return;                // ���̐e�@�����ɂ��Q�[������z�M���Ă�����A������폜������B
            }
        }
        else
        {
            info->sameBeaconRecvCount = 0;
        }
        // ����ȏ�Ԃ̐e�@�̃f�[�^��M
        info->beaconNo = brecvBuffp->beaconNo;
        info->lifetimeCount = MB_LIFETIME_MAX_COUNT;    // �e�@���̉����B
        info->gameInfo.ggid = brecvBuffp->ggid; // ggid    �̎擾�B
        info->gameInfo.fileNo = brecvBuffp->fileNo;     // fileNo  �̎擾�B
        info->linkLevel = linkLevel;   // �d�g���x�̎擾�B
        // bssDesc��SSID���Z�b�g����B
        MBi_SetSSIDToBssDesc(&info->bssDesc, info->gameInfo.ggid);
    }

    // ��M�r�[�R���̃f�[�^��ޖ��̃f�[�^�擾
    if (brecvBuffp->dataAttr == MB_BEACON_DATA_ATTR_VOLAT)
    {
        MBi_RecvVolatBeacon(index);
    }
    else
    {
        MBi_RecvFixedBeacon(index);
    }
}


// tgid�̃`�F�b�N
static void MBi_CheckTGID(WMBssDesc *bssDescp, int index)
{
    if (mbrsp->list[index].bssDesc.gameInfo.tgid == bssDescp->gameInfo.tgid)
    {
        return;
    }

    // tgid���A�b�v���Ă�����A���̐e�@�͗����グ������Ă���Ƃ݂Ȃ��āA�S�f�[�^���N���A���Ď�蒼���B
    MB_DEBUG_OUTPUT("\ntgid updated! : %x%x", *(u16 *)(&bssDescp->bssid[4]),
                    *(u32 *)bssDescp->bssid);
    MB_DeleteRecvGameInfoWithoutBssdesc(index);
    MI_CpuCopy16(bssDescp, &mbrsp->list[index].bssDesc, sizeof(WMBssDesc));
    // tgid�X�V�̏ꍇ�́A�V����bssDesc���R�s�[����B
    MBi_LockScanTarget(index);         // �Y���e�@���X�L�������b�N����B
}


// Fixed�f�[�^��SeqNo�`�F�b�N
static void MBi_CheckSeqNoFixed(int index)
{
    // �V�[�P���X�ԍ����X�V����Ă����ꍇ�̑Ή�
    if (mbrsp->list[index].gameInfo.seqNoFixed == brecvBuffp->seqNoFixed)
    {
        return;
    }
    // �V�[�P���X�ԍ����X�V����Ă�����A����܂łɎ�M�����f�[�^���N���A����B
    MB_DEBUG_OUTPUT("\n seqNoFixed updated!");
    MB_DeleteRecvGameInfoWithoutBssdesc(index);
    MBi_LockScanTarget(index);         // �Y���e�@���X�L�������b�N����B
    mbrsp->list[index].gameInfo.seqNoFixed = brecvBuffp->seqNoFixed;
}


// Volat�f�[�^��SeqNo�`�F�b�N
static void MBi_CheckSeqNoVolat(int index)
{
    MBGameInfoRecvList *grecvp = &mbrsp->list[index];

    // �V�[�P���X�ԍ����X�V����Ă����ꍇ�̑Ή�
    if (mbrsp->list[index].gameInfo.seqNoVolat != brecvBuffp->seqNoVolat)
    {
        MB_DEBUG_OUTPUT("\n seqNoVolat updated!");
        MBi_LockScanTarget(index);     // �X�L�������b�N�\�����ׂă��b�N����B
    }
}


// �Ώ�BSSID�̐e�@�Q�[������S�Ė����ɂ���B
static void MBi_InvalidateGameInfoBssID(u8 *bssidp)
{
    int     i;
    for (i = 0; i < MB_GAME_INFO_RECV_LIST_NUM; i++)
    {
        if ((mbrsp->usingGameInfoFlag & (0x01 << i)) == 0)
        {
            continue;
        }

        if (!WM_IsBssidEqual(bssidp, mbrsp->list[i].bssDesc.bssid))
        {
            continue;
        }

        // �폜�Ώۂ̃f�[�^�Ɣ���B
        mbrsp->list[i].lifetimeCount = 0;       // �e�@���̎������[���ɂ��āA�폜������B
        MB_OUTPUT("The parent broadcast same beacon.: %d\n", i);
    }
}


// �Q�[�����̌Œ�f�[�^���𕪊����ăr�[�R����M
static void MBi_RecvFixedBeacon(int index)
{
    MBGameInfoRecvList *grecvp = &mbrsp->list[index];
    u32     lastAddr = (u32)&grecvp->gameInfo + sizeof(MBGameInfoFixed);
    u8     *dstp;

    // �V�[�P���X�ԍ����X�V����Ă��炸�A�����Ɏ擾�ς݂̃r�[�R���Ȃ��M���Ȃ��B
    if (grecvp->gameInfo.seqNoFixed == brecvBuffp->seqNoFixed)
    {
        if (grecvp->getFlagmentFlag & (0x01 << brecvBuffp->data.fixed.flagmentNo))
        {
            return;
        }
    }

    // ��M�r�[�R������M�o�b�t�@���I�[�o�[���Ȃ����`�F�b�N�B
    if (brecvBuffp->dataAttr == MB_BEACON_DATA_ATTR_FIXED_NORMAL)
    {
        dstp = (u8 *)&grecvp->gameInfo.fixed;
    }
    else
    {
        dstp = (u8 *)&grecvp->gameInfo.fixed.parent;
    }
    dstp += MB_BEACON_FIXED_DATA_SIZE * brecvBuffp->data.fixed.flagmentNo;
    // �Q�[�����o�b�t�@�̎�M�A�h���X���Z�o�B

    if ((u32)dstp + brecvBuffp->data.fixed.size > lastAddr)
    {
        MB_DEBUG_OUTPUT("recv beacon gInfoFixed Buffer over!\n");
        // �o�b�t�@���I�[�o�[����r�[�R���f�[�^�͖�������B
        return;
    }

    // ��M�r�[�R����ΏۃQ�[�����o�b�t�@�ɃZ�b�g
    MB_DEBUG_OUTPUT("recv flagment= %2d  adr = 0x%x", brecvBuffp->data.fixed.flagmentNo, dstp);
    MI_CpuCopy16(brecvBuffp->data.fixed.data, dstp, brecvBuffp->data.fixed.size);
    grecvp->gameInfo.dataAttr = brecvBuffp->dataAttr;
    grecvp->getFlagmentFlag |= 0x01 << brecvBuffp->data.fixed.flagmentNo;
    grecvp->allFlagmentFlag = (u32)((0x01 << brecvBuffp->data.fixed.flagmentMaxNum) - 1);
    MB_DEBUG_OUTPUT("\t now flagment = 0x%x \t all flagment = 0x%x\n",
                    grecvp->getFlagmentFlag, grecvp->allFlagmentFlag);
}


// �Q�[�����̗����f�[�^���𕪊����ăr�[�R����M
static void MBi_RecvVolatBeacon(int index)
{
    int     i;
    MBGameInfoRecvList *grecvp = &mbrsp->list[index];

    /* ���[�U��`�f�[�^�͏�Ɏ�M���� */
    for (i = 0; i < MB_USER_VOLAT_DATA_SIZE; i++)
    {
        grecvp->gameInfo.volat.userVolatData[i] = brecvBuffp->data.volat.userVolatData[i];
    }
    MI_CpuCopy16(brecvBuffp, &grecvp->bssDesc.gameInfo.userGameInfo, WM_SIZE_USER_GAMEINFO);

    // �����o�[���X�V�����o�����ۂ̏���
    if (grecvp->gameInfo.seqNoVolat != brecvBuffp->seqNoVolat)
    {
        if ((u8)(grecvp->gameInfo.seqNoVolat + 1) == brecvBuffp->seqNoVolat)
        {                              // seqNoVolat���P����Ȃ�A�ύX����Ă��Ȃ������o�[���̈��p��
            for (i = 0; i < MB_MEMBER_MAX_NUM; i++)
            {
                if (brecvBuffp->data.volat.changePlayerFlag & (0x02 << i))
                {
                    MI_CpuClear16(&grecvp->gameInfo.volat.member[i], sizeof(MBUserInfo));
                }
            }
            grecvp->getPlayerFlag &= ~brecvBuffp->data.volat.changePlayerFlag;
            mbrsp->validGameInfoFlag &= ~(0x0001 << index);
        }
        else
        {                              // seqNoVolat�������Ƃ���Ă�����A����܂Ŏ�M�����S�����o�[�����N���A
            MI_CpuClear16(&grecvp->gameInfo.volat.member[0],
                          sizeof(MBUserInfo) * MB_MEMBER_MAX_NUM);
            grecvp->getPlayerFlag = 0;
            mbrsp->validGameInfoFlag &= ~(0x0001 << index);
        }
        grecvp->gameInfo.seqNoVolat = brecvBuffp->seqNoVolat;
    }
    else if (grecvp->getPlayerFlag == brecvBuffp->data.volat.nowPlayerFlag)
    {
        return;                        // �V�[�P���X�ԍ����X�V����Ă��炸�A�����Ɏ擾�ς݂̃r�[�R���Ȃ��M���Ȃ��B
    }

    // �v���C���[���̓ǂݏo��
    grecvp->gameInfo.volat.nowPlayerNum = brecvBuffp->data.volat.nowPlayerNum;
    grecvp->gameInfo.volat.nowPlayerFlag = brecvBuffp->data.volat.nowPlayerFlag;
    grecvp->gameInfo.volat.changePlayerFlag = brecvBuffp->data.volat.changePlayerFlag;
    grecvp->getPlayerFlag |= 0x0001;

    // �e�����o�[�̃��[�U�[���̓ǂݏo��
    for (i = 0; i < MB_SEND_MEMBER_MAX_NUM; i++)
    {
        int     playerNo = (int)brecvBuffp->data.volat.member[i].playerNo;
        if (playerNo == 0)
        {
            continue;
        }
        MB_DEBUG_OUTPUT("member %d recv.\n", playerNo);
        MI_CpuCopy16(&brecvBuffp->data.volat.member[i],
                     &grecvp->gameInfo.volat.member[playerNo - 1], sizeof(MBUserInfo));
        grecvp->getPlayerFlag |= 0x01 << playerNo;
    }
}


// �X�L�����Ώۂ�P��e�@�Ƀ��b�N����
static void MBi_LockScanTarget(int index)
{
    /* ���Ƀ��b�N���̃^�[�Q�b�g������Ȃ疳�� */
    if (mbrsp->nowScanTargetFlag)
    {
        return;
    }

    if (sLockFunc != NULL)
    {
        sLockFunc(mbrsp->list[index].bssDesc.bssid);
    }
    mbrsp->scanCountUnit = MB_SCAN_COUNT_UNIT_LOCKING;

    mbrsp->nowScanTargetFlag = (u16)(0x01 << index);    // �V�����X�L�������b�N�Ώۂ��r�b�g�Ŏ����B
    mbrsp->nowLockTimeCount = MB_LOCKTIME_MAX_COUNT;    // �X�L�������b�N���Ԃ�������
    MB_DEBUG_OUTPUT("scan target locked. : %x %x %x %x %x %x\n",
                    mbrsp->list[index].bssDesc.bssid[0],
                    mbrsp->list[index].bssDesc.bssid[1],
                    mbrsp->list[index].bssDesc.bssid[2],
                    mbrsp->list[index].bssDesc.bssid[3],
                    mbrsp->list[index].bssDesc.bssid[4], mbrsp->list[index].bssDesc.bssid[5]);
}


// �X�L�����Ώۂ̃��b�N����������B
static void MBi_UnlockScanTarget(void)
{
    if (mbrsp->nowScanTargetFlag == 0)
    {
        return;
    }

    if (sUnlockFunc != NULL)
    {
        sUnlockFunc();
    }
    mbrsp->scanCountUnit = MB_SCAN_COUNT_UNIT_NORMAL;
    mbrsp->nowScanTargetFlag = 0;
    mbrsp->notFoundLockTargetCount = 0;

    MB_DEBUG_OUTPUT(" unlock target\n");
}


// ��M�����Q�[���������S�ɍ폜�i�Q�[�����L���t���O���폜�j
void MB_DeleteRecvGameInfo(int index)
{
    mbrsp->usefulGameInfoFlag &= ~(0x0001 << index);
    mbrsp->validGameInfoFlag &= ~(0x0001 << index);
    MI_CpuClear16(&mbrsp->list[index], sizeof(MBGameInfoRecvList));
}


// ��M�����Q�[������bssDesc�������č폜����B
void MB_DeleteRecvGameInfoWithoutBssdesc(int index)
{
    mbrsp->usefulGameInfoFlag &= ~(0x0001 << index);
    mbrsp->validGameInfoFlag &= ~(0x0001 << index);
    mbrsp->list[index].getFlagmentFlag = 0;
    mbrsp->list[index].allFlagmentFlag = 0;
    mbrsp->list[index].getPlayerFlag = 0;
    mbrsp->list[index].linkLevel = 0;
    MI_CpuClear16(&(mbrsp->list[index].gameInfo), sizeof(MBGameInfo));
}

// ��M�����e�@���\���̂ւ̃|�C���^���擾����
MBGameInfoRecvList *MB_GetGameInfoRecvList(int index)
{
    // �L���ȃf�[�^���Ȃ����NULL��Ԃ�
    if ((mbrsp->usefulGameInfoFlag & (0x01 << index)) == 0)
    {
        return NULL;
    }

    return &mbrsp->list[index];
}


//=========================================================
// ���[�UVolatile�f�[�^�̐ݒ�
//=========================================================

/*---------------------------------------------------------------------------*
  Name:         MB_SetSendVolatileCallback

  Description:  �}���`�u�[�g�̃r�[�R�����M�R�[���o�b�N��ݒ肵�܂��B
  
  Arguments:    callback    ���M�����̃R�[���o�b�N�֐��ł��B
                            �f�[�^�����M�����x�ɃR�[���o�b�N���Ăяo����܂��B
                timming     �R�[���o�b�N�����^�C�~���O�B
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void MB_SetSendVolatCallback(MBSendVolatCallbackFunc callback, u32 timming)
{
    OSIntrMode enabled = OS_DisableInterrupts();

    sSendVolatCallback = callback;
    sSendVolatCallbackTimming = timming;

    (void)OS_RestoreInterrupts(enabled);
}



/*---------------------------------------------------------------------------*
  Name:         MB_SetUserVolatData

  Description:  �}���`�u�[�g�̃r�[�R�����̋󂫗̈�Ƀ��[�U�f�[�^��ݒ肵�܂��B
  
  Arguments:    ggid        MB_RegisterFile�Őݒ肵���v���O������ggid���w�肵�āA
                            ���̃t�@�C���̃r�[�R���Ƀ��[�U�f�[�^��t�����܂��B
                userData    �ݒ肷�郆�[�U�f�[�^�ւ̃|�C���^�B
                size        �ݒ肷�郆�[�U�f�[�^�̃T�C�Y (�ő�8�o�C�g�܂�)
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void MB_SetUserVolatData(u32 ggid, const u8 *userData, u32 size)
{
    MBGameInfo *gameInfo;

    SDK_ASSERT(size <= MB_USER_VOLAT_DATA_SIZE);
    SDK_NULL_ASSERT(userData);

    gameInfo = mbss.gameInfoListTop;
    if (gameInfo == NULL)
    {
        return;
    }

    while (gameInfo->ggid != ggid)
    {
        if (gameInfo == NULL)
        {
            return;
        }
        gameInfo = gameInfo->nextp;
    }

    {
        u32     i;

        OSIntrMode enabled = OS_DisableInterrupts();

        for (i = 0; i < size; i++)
        {
            gameInfo->volat.userVolatData[i] = userData[i];
        }

        (void)OS_RestoreInterrupts(enabled);
    }
}

/*---------------------------------------------------------------------------*
  Name:         MB_GetUserVolatData

  Description:  �r�[�R�����̋󂫗̈�ɂ��郆�[�U�f�[�^���擾���܂��B
  
  Arguments:    gameInfo    Scan���Ɏ擾����gameInfo�p�����[�^�ւ̃|�C���^�B
  
  Returns:      ���[�U�f�[�^�ւ̃|�C���^.
 *---------------------------------------------------------------------------*/
void   *MB_GetUserVolatData(const WMGameInfo *gameInfo)
{
    MbBeacon *beacon;

    SDK_NULL_ASSERT(gameInfo);

    if (!(gameInfo->attribute & WM_ATTR_FLAG_MB))
    {
        return NULL;
    }

    beacon = (MbBeacon *) (gameInfo->userGameInfo);

    if (beacon->dataAttr != MB_BEACON_DATA_ATTR_VOLAT)
    {
        return NULL;
    }

    return beacon->data.volat.userVolatData;
}








#if defined(MB_CHANGE_TO_UNICODE)

/******************************************************************************/
/* Unicode - SJIS */

/* ��`�����������̂ŉ��}�ŗp�� */
#define SJIS_HIGHER_CODE1_MIN   0x81
#define SJIS_HIGHER_CODE1_MAX   0x9F
#define SJIS_HIGHER_CODE2_MIN   0xE0
#define SJIS_HIGHER_CODE2_MAX   0xFC

// define data------------------------------------------
#define TBL8140_ELEM_NUM                0xbd    // 8140tbl�̗v�f��
#define TBL849f_ELEM_NUM                0x20    // 849ftbl�̗v�f��

typedef struct SjisUtf16Pare
{
    u16     sjis;
    u16     unicode;
}
SjisUtf16Pare;

// extern data------------------------------------------

// function's prototype declaration---------------------
static u16 SearchUnicodeTable(u16 unicode, SjisUtf16Pare * tblp, int elem_num);

// global variable -------------------------------------

// static variable -------------------------------------

// const data  -----------------------------------------
static const SjisUtf16Pare tblSJIS_UTF16_8140[TBL8140_ELEM_NUM];
static const SjisUtf16Pare tblSJIS_UTF16_849f[TBL849f_ELEM_NUM];


//======================================================
// SJIS-BE����UTF16-LE�ւ̕ϊ�
//======================================================
void MBi_ExSJIS_BEtoUTF16_LE(u8 *sjisp, u16 *unip, u16 length)
{
    u16     code;

    while ((*sjisp) && (length-- > 0))
    {
        if (((*sjisp >= SJIS_HIGHER_CODE1_MIN) && (*sjisp <= SJIS_HIGHER_CODE1_MAX))
            || ((*sjisp >= SJIS_HIGHER_CODE2_MIN) && (*sjisp <= SJIS_HIGHER_CODE2_MAX)))
        {                              // SJIS���H
            code = (u16)((u16)*sjisp++ << 8);
            code |= (u16)*sjisp++;
        }
        else
        {                              // ASCII
            code = (u16)*sjisp++;
        }

        if (code == 0x005c)
        {
            *unip = 0x00a5;            // \

        }
        else if (code == 0x007e)
        {
            *unip = 0x203e;            // ~

        }
        else if ((code == 0x000d) || (code == 0x000a))
        {
            *unip = code;              // ���s�R�[�h

        }
        else if ((code >= 0x0020) && (code < 0x007e))   // ' ' �` }
        {
            *unip = code;

        }
        else if ((code >= 0x00a1) && (code <= 0x00df))  // � �` �
        {
            *unip = (u16)((code - 0x00a1) + 0xff61);

        }
        else if ((code >= 0x8140) && (code <= 0x81fc))  // '�@' �` ��
        {
            // �o���o���Ȃ̂ŁA�e�[�u������
            *unip = tblSJIS_UTF16_8140[code - 0x8140].unicode;

        }
        else if ((code >= 0x824f) && (code <= 0x8258))  // �O �` �X
        {
            *unip = (u16)((code - 0x824f) + 0xff10);

        }
        else if ((code >= 0x8260) && (code <= 0x8279))  // �` �` �y
        {
            *unip = (u16)((code - 0x8260) + 0xff21);

        }
        else if ((code >= 0x8281) && (code <= 0x829a))  // �� �` ��
        {
            *unip = (u16)((code - 0x8281) + 0xff41);

        }
        else if ((code >= 0x829f) && (code <= 0x82f1))  // �� �` ��
        {
            *unip = (u16)((code - 0x829f) + 0x3041);

        }
        else if ((code >= 0x8340) && (code <= 0x8396))  // �@ �` ��  ��0x837f�͔���
        {
            *unip = (u16)((code - 0x8340) + 0x30a1);
            if (code == 0x837f)
            {
                *unip = 0x3000;
            }
            else if (code > 0x837f)
            {
                (*unip)--;
            }

        }
        else if ((code >= 0x839f) && (code <= 0x83b6))  // �� �` ��
        {
            *unip = (u16)((code - 0x839f) + 0x0391);
            if (code >= 0x83b0)
                (*unip)++;

        }
        else if ((code >= 0x83bf) && (code <= 0x83d6))  // �� �` ��
        {
            *unip = (u16)((code - 0x83bf) + 0x03b1);
            if (code >= 0x83d0)
                (*unip)++;

        }
        else if ((code >= 0x8440) && (code <= 0x8460))  // �@ �` �`
        {
            *unip = (u16)((code - 0x8440) + 0x0410);
            if (code == 0x8446)
            {
                *unip = 0x0401;
            }
            else if (code > 0x8446)
            {
                (*unip)--;
            }

        }
        else if ((code >= 0x8470) && (code <= 0x8491))  // �p �` �� ��0x847f�͔���
        {
            *unip = (u16)((code - 0x8470) + 0x0430);
            if (code == 0x8476)
            {
                *unip = 0x0451;
            }
            else if (code == 0x847f)
            {
                *unip = 0x3000;
            }
            else if (code > 0x8476)
            {
                (*unip)--;
                if (code > 0x847f)
                {
                    (*unip)--;
                }
            }

        }
        else if ((code >= 0x849f) && (code <= 0x84be))  // �� �` ��
        {
            // �o���o���Ȃ̂ŁA�e�[�u������
            *unip = tblSJIS_UTF16_849f[code - 0x849f].unicode;
        }
        else
        {
            *unip = 0x3000;
        }

        unip++;
    }
}


//======================================================
// UTF16-LE����SJIS-BE�ւ̕ϊ�
//======================================================
void MBi_ExUTF16_LEtoSJIS_BE(u8 *sjisp, u16 *unip, u16 length)
{
    u16     code, sjis_le;

    while ((*unip) && (length-- > 0))
    {
        code = *unip++;
        sjis_le = 0;

        if (code == 0x00a5)
        {
            sjis_le = 0x005c;          // \

        }
        else if (code == 0x005c)
        {
            sjis_le = 0x815f;          // �_

        }
        else if (code == 0x203e)
        {
            sjis_le = 0x007e;          // ~

        }
        else if ((code == 0x000d) || (code == 0x000a))
        {
            sjis_le = code;            // ���s�R�[�h

        }
        else if ((code >= 0x0020) && (code < 0x007e))   // ' ' �` }
        {
            sjis_le = code;

        }
        else if ((code >= 0x00a2) && (code <= 0x00f7))
        {                              // Unicode = 0x00a2 - 0x00f7 �́A SJIS = 0x814c - 0x81f7�ɔz�u
            sjis_le =
                SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[0xc],
                                   0x81f7 - 0x814c);

        }
        else if ((code >= 0xff61) && (code <= 0xff9f))  // � �` �
        {
            sjis_le = (u16)((code - 0xff61) + 0x00a1);

        }
        else if (code == 0x4edd)
        {
            sjis_le = 0x8157;

        }
        else if ((code >= 0xff01) && (code <= 0xffe5))
        {
            if ((code >= 0xff10) && (code <= 0xff19))   // �O �` �X
            {
                sjis_le = (u16)((code - 0xff10) + 0x824f);

            }
            else if ((code >= 0xff21) && (code <= 0xff3a))      // �` �` �y
            {
                sjis_le = (u16)((code - 0xff21) + 0x8260);

            }
            else if ((code >= 0xff41) && (code <= 0xff5a))      // �� �` ��
            {
                sjis_le = (u16)((code - 0xff41) + 0x8281);
            }
            else
            {                          // Unicode = 0xff01 - 0xffe5 �́A SJIS = 0x8143 - 0x8197�ɔz�u
                sjis_le =
                    SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[3],
                                       0x8197 - 0x8143);
            }

        }
        else if ((code >= 0x3000) && (code <= 0x30fe))
        {
            if ((code >= 0x3041) && (code <= 0x3093))   // �� �` ��
            {
                sjis_le = (u16)((code - 0x3041) + 0x829f);

            }
            else if ((code >= 0x30a1) && (code <= 0x30f6))      // �@ �` ��  ��0x837f�͔���
            {
                sjis_le = (u16)((code - 0x30a1) + 0x8340);
                if (code >= 0x30e0)
                {
                    (sjis_le)++;
                }
            }
            else
            {                          // Unicode = 0x3000 - 0x30fe �́A SJIS = 0x8140 - 0x81ac�ɔz�u
                sjis_le =
                    SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[0],
                                       0x81ac - 0x8140);
            }

        }
        else if ((code >= 0x0391) && (code <= 0x03a9))  // �� �` ��
        {
            sjis_le = (u16)((code - 0x0391) + 0x839f);
            if (code >= 0x03a3)
                (sjis_le)--;

        }
        else if ((code >= 0x03b1) && (code <= 0x03c9))  // �� �` ��
        {
            sjis_le = (u16)((code - 0x03b1) + 0x83bf);
            if (code >= 0x03c3)
                (sjis_le)--;

        }
        else if (code == 0x0401)
        {
            sjis_le = 0x8446;

        }
        else if ((code >= 0x0410) && (code <= 0x042f))  // �@ �` �`
        {
            sjis_le = (u16)((code - 0x0410) + 0x8440);
            if (code >= 0x0416)
            {
                (sjis_le)++;
            }

        }
        else if ((code >= 0x0430) && (code <= 0x044f))  // �p �` �� ��0x847f�͔���
        {
            sjis_le = (u16)((code - 0x0430) + 0x8470);
            if (code >= 0x0436)
            {
                (sjis_le)++;
                if (code >= 0x043e)
                {
                    (sjis_le)++;
                }
            }

        }
        else if (code == 0x0451)
        {
            sjis_le = 0x8476;

        }
        else if ((code >= 0x2500) && (code <= 0x254b))  // �� �` ��
        {
            sjis_le =
                SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_849f, TBL849f_ELEM_NUM);

        }
        else if (((code >= 0x2010) && (code <= 0x2312)) || ((code >= 0x25a0) && (code <= 0x266f)))
        {                              // ��L�R�[�h�́A SJIS = 0x815c - 0x81fc�ɔz�u
            sjis_le =
                SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[0x815c - 0x8140],
                                   0x81fc - 0x815c);

        }
        else
        {
            sjis_le = 0x8140;
        }

        // �ϊ�����SJIS�R�[�h���o�b�t�@�Ɋi�[
        if (sjis_le & 0xff00)
        {                              // ASCII�R�[�h�łȂ���΁A�r�b�O�G���f�B�A���`���Ŋi�[�B
            *sjisp++ = (u8)(sjis_le >> 8);
        }
        *sjisp++ = (u8)(sjis_le);
    }
}


// Unicode -> SJIS�ւ̃e�[�u������
static u16 SearchUnicodeTable(u16 unicode, SjisUtf16Pare * tblp, int elem_num)
{
    elem_num++;
    while (elem_num--)
    {
        if (tblp->unicode == unicode)
        {
            return tblp->sjis;
        }
        tblp++;
    }
    return 0x8140;
}


//======================================================
// SJIS-BE <-> UTF16-LE�ϊ��̃`�F�b�N
//======================================================
void MBi_CheckSJIS_BEtoUTF16_LE(void)
{
    u16     sjis, sjis_be, rev_sjis, sjis_le;
    u16     unicode;

    // ASCII�R�[�h�̃`�F�b�N
    for (sjis = 0; sjis < 0x00ff; sjis++)
    {
        unicode = 0;
        rev_sjis = 0;
        MBi_ExSJIS_BEtoUTF16_LE((u8 *)&sjis, &unicode, 1);
        MBi_ExUTF16_LEtoSJIS_BE((u8 *)&rev_sjis, &unicode, 1);
        sjis_le = (u16)((rev_sjis >> 8) | (rev_sjis << 8));
        MB_DEBUG_OUTPUT("0x%x\t-> 0x%x\t-> 0x%x\n", sjis, unicode, sjis_le);
    }

    // SJIS�R�[�h�̃`�F�b�N
    for (sjis = 0x8140; sjis < 0x84ff; sjis++)
    {
        unicode = 0;
        rev_sjis = 0;
        sjis_be = (u16)((sjis >> 8) | (sjis << 8));
        MBi_ExSJIS_BEtoUTF16_LE((u8 *)&sjis_be, &unicode, 1);
        MBi_ExUTF16_LEtoSJIS_BE((u8 *)&rev_sjis, &unicode, 1);
        sjis_le = (u16)((rev_sjis >> 8) | (rev_sjis << 8));
        MB_DEBUG_OUTPUT("0x%x\t-> 0x%x\t-> 0x%x\n", sjis, unicode, sjis_le);
    }
}


//======================================================
// SJIS�R�[�h->Unicode�e�[�u��
//======================================================

// 0x8140�`
static const SjisUtf16Pare tblSJIS_UTF16_8140[TBL8140_ELEM_NUM] = {
    {0x8140, 0x3000},                  // �@ 
    {0x8141, 0x3001},                  // �A 
    {0x8142, 0x3002},                  // �B 
    {0x8143, 0xFF0C},                  // �C 
    {0x8144, 0xFF0E},                  // �D 
    {0x8145, 0x30FB},                  // �E 
    {0x8146, 0xFF1A},                  // �F 
    {0x8147, 0xFF1B},                  // �G 
    {0x8148, 0xFF1F},                  // �H 
    {0x8149, 0xFF01},                  // �I 
    {0x814A, 0x309B},                  // �J 
    {0x814B, 0x309C},                  // �K 
    {0x814C, 0x00B4},                  // �L 
    {0x814D, 0xFF40},                  // �M 
    {0x814E, 0x00A8},                  // �N 
    {0x814F, 0xFF3E},                  // �O 
    {0x8150, 0xFFE3},                  // �P 
    {0x8151, 0xFF3F},                  // �Q 
    {0x8152, 0x30FD},                  // �R 
    {0x8153, 0x30FE},                  // �S 
    {0x8154, 0x309D},                  // �T 
    {0x8155, 0x309E},                  // �U 
    {0x8156, 0x3003},                  // �V 
    {0x8157, 0x4EDD},                  // �W 
    {0x8158, 0x3005},                  // �X 
    {0x8159, 0x3006},                  // �Y 
    {0x815A, 0x3007},                  // �Z 
    {0x815B, 0x30FC},                  // �[ 
    {0x815C, 0x2015},                  // �\ 
    {0x815D, 0x2010},                  // �] 
    {0x815E, 0xFF0F},                  // �^ 
    {0x815F, 0x005C},                  // �_ 
    {0x8160, 0x301C},                  // �` 
    {0x8161, 0x2016},                  // �a 
    {0x8162, 0xFF5C},                  // �b 
    {0x8163, 0x2026},                  // �c 
    {0x8164, 0x2025},                  // �d 
    {0x8165, 0x2018},                  // �e 
    {0x8166, 0x2019},                  // �f 
    {0x8167, 0x201C},                  // �g 
    {0x8168, 0x201D},                  // �h 
    {0x8169, 0xFF08},                  // �i 
    {0x816A, 0xFF09},                  // �j 
    {0x816B, 0x3014},                  // �k 
    {0x816C, 0x3015},                  // �l 
    {0x816D, 0xFF3B},                  // �m 
    {0x816E, 0xFF3D},                  // �n 
    {0x816F, 0xFF5B},                  // �o 
    {0x8170, 0xFF5D},                  // �p 
    {0x8171, 0x3008},                  // �q 
    {0x8172, 0x3009},                  // �r 
    {0x8173, 0x300A},                  // �s 
    {0x8174, 0x300B},                  // �t 
    {0x8175, 0x300C},                  // �u 
    {0x8176, 0x300D},                  // �v 
    {0x8177, 0x300E},                  // �w 
    {0x8178, 0x300F},                  // �x 
    {0x8179, 0x3010},                  // �y 
    {0x817A, 0x3011},                  // �z 
    {0x817B, 0xFF0B},                  // �{ 
    {0x817C, 0x2212},                  // �| 
    {0x817D, 0x00B1},                  // �} 
    {0x817E, 0x00D7},                  // �~ 
    {0x817F, 0x3000},                  //  
    {0x8180, 0x00F7},                  // �� 
    {0x8181, 0xFF1D},                  // �� 
    {0x8182, 0x2260},                  // �� 
    {0x8183, 0xFF1C},                  // �� 
    {0x8184, 0xFF1E},                  // �� 
    {0x8185, 0x2266},                  // �� 
    {0x8186, 0x2267},                  // �� 
    {0x8187, 0x221E},                  // �� 
    {0x8188, 0x2234},                  // �� 
    {0x8189, 0x2642},                  // �� 
    {0x818A, 0x2640},                  // �� 
    {0x818B, 0x00B0},                  // �� 
    {0x818C, 0x2032},                  // �� 
    {0x818D, 0x2033},                  // �� 
    {0x818E, 0x2103},                  // �� 
    {0x818F, 0xFFE5},                  // �� 
    {0x8190, 0xFF04},                  // �� 
    {0x8191, 0x00A2},                  // �� 
    {0x8192, 0x00A3},                  // �� 
    {0x8193, 0xFF05},                  // �� 
    {0x8194, 0xFF03},                  // �� 
    {0x8195, 0xFF06},                  // �� 
    {0x8196, 0xFF0A},                  // �� 
    {0x8197, 0xFF20},                  // �� 
    {0x8198, 0x00A7},                  // �� 
    {0x8199, 0x2606},                  // �� 
    {0x819A, 0x2605},                  // �� 
    {0x819B, 0x25CB},                  // �� 
    {0x819C, 0x25CF},                  // �� 
    {0x819D, 0x25CE},                  // �� 
    {0x819E, 0x25C7},                  // �� 
    {0x819F, 0x25C6},                  // �� 
    {0x81A0, 0x25A1},                  // �� 
    {0x81A1, 0x25A0},                  // �� 
    {0x81A2, 0x25B3},                  // �� 
    {0x81A3, 0x25B2},                  // �� 
    {0x81A4, 0x25BD},                  // �� 
    {0x81A5, 0x25BC},                  // �� 
    {0x81A6, 0x203B},                  // �� 
    {0x81A7, 0x3012},                  // �� 
    {0x81A8, 0x2192},                  // �� 
    {0x81A9, 0x2190},                  // �� 
    {0x81AA, 0x2191},                  // �� 
    {0x81AB, 0x2193},                  // �� 
    {0x81AC, 0x3013},                  // �� 
    {0x81AD, 0x3000},                  //  
    {0x81AE, 0x3000},                  //  
    {0x81AF, 0x3000},                  //  
    {0x81B0, 0x3000},                  //  
    {0x81B1, 0x3000},                  //  
    {0x81B2, 0x3000},                  //  
    {0x81B3, 0x3000},                  //  
    {0x81B4, 0x3000},                  //  
    {0x81B5, 0x3000},                  //  
    {0x81B6, 0x3000},                  //  
    {0x81B7, 0x3000},                  //  
    {0x81B8, 0x2208},                  // �� 
    {0x81B9, 0x220B},                  // �� 
    {0x81BA, 0x2286},                  // �� 
    {0x81BB, 0x2287},                  // �� 
    {0x81BC, 0x2282},                  // �� 
    {0x81BD, 0x2283},                  // �� 
    {0x81BE, 0x222A},                  // �� 
    {0x81BF, 0x2229},                  // �� 
    {0x81C0, 0x3000},                  //  
    {0x81C1, 0x3000},                  //  
    {0x81C2, 0x3000},                  //  
    {0x81C3, 0x3000},                  //  
    {0x81C4, 0x3000},                  //  
    {0x81C5, 0x3000},                  //  
    {0x81C6, 0x3000},                  //  
    {0x81C7, 0x3000},                  //  
    {0x81C8, 0x2227},                  // �� 
    {0x81C9, 0x2228},                  // �� 
    {0x81CA, 0x00AC},                  // �� 
    {0x81CB, 0x21D2},                  // �� 
    {0x81CC, 0x21D4},                  // �� 
    {0x81CD, 0x2200},                  // �� 
    {0x81CE, 0x2203},                  // �� 
    {0x81CF, 0x3000},                  //  
    {0x81D0, 0x3000},                  //  
    {0x81D1, 0x3000},                  //  
    {0x81D2, 0x3000},                  //  
    {0x81D3, 0x3000},                  //  
    {0x81D4, 0x3000},                  //  
    {0x81D5, 0x3000},                  //  
    {0x81D6, 0x3000},                  //  
    {0x81D7, 0x3000},                  //  
    {0x81D8, 0x3000},                  //  
    {0x81D9, 0x3000},                  //  
    {0x81DA, 0x2220},                  // �� 
    {0x81DB, 0x22A5},                  // �� 
    {0x81DC, 0x2312},                  // �� 
    {0x81DD, 0x2202},                  // �� 
    {0x81DE, 0x2207},                  // �� 
    {0x81DF, 0x2261},                  // �� 
    {0x81E0, 0x2252},                  // �� 
    {0x81E1, 0x226A},                  // �� 
    {0x81E2, 0x226B},                  // �� 
    {0x81E3, 0x221A},                  // �� 
    {0x81E4, 0x223D},                  // �� 
    {0x81E5, 0x221D},                  // �� 
    {0x81E6, 0x2235},                  // �� 
    {0x81E7, 0x222B},                  // �� 
    {0x81E8, 0x222C},                  // �� 
    {0x81E9, 0x3000},                  //  
    {0x81EA, 0x3000},                  //  
    {0x81EB, 0x3000},                  //  
    {0x81EC, 0x3000},                  //  
    {0x81EE, 0x3000},                  //  
    {0x81EE, 0x3000},                  //  
    {0x81EF, 0x3000},                  //  
    {0x81F0, 0x212B},                  // �� 
    {0x81F1, 0x2030},                  // �� 
    {0x81F2, 0x266F},                  // �� 
    {0x81F3, 0x266D},                  // �� 
    {0x81F4, 0x266A},                  // �� 
    {0x81F5, 0x2020},                  // �� 
    {0x81F6, 0x2021},                  // �� 
    {0x81F7, 0x00B6},                  // �� 
    {0x81F8, 0x3000},                  //  
    {0x81F9, 0x3000},                  //  
    {0x81FA, 0x3000},                  //  
    {0x81FB, 0x3000},                  //  
    {0x81FC, 0x25EF},                  // �� 
};


// 0x849f�`
// Unicode�ł́A0x2500-0x254b�̊�
static const SjisUtf16Pare tblSJIS_UTF16_849f[TBL849f_ELEM_NUM] = {
    {0x849F, 0x2500},                  // �� 
    {0x84A0, 0x2502},                  // �� 
    {0x84A1, 0x250C},                  // �� 
    {0x84A2, 0x2510},                  // �� 
    {0x84A3, 0x2518},                  // �� 
    {0x84A4, 0x2514},                  // �� 
    {0x84A5, 0x251C},                  // �� 
    {0x84A6, 0x252C},                  // �� 
    {0x84A7, 0x2524},                  // �� 
    {0x84A8, 0x2534},                  // �� 
    {0x84A9, 0x253C},                  // �� 
    {0x84AA, 0x2501},                  // �� 
    {0x84AB, 0x2503},                  // �� 
    {0x84AC, 0x250F},                  // �� 
    {0x84AD, 0x2513},                  // �� 
    {0x84AE, 0x251B},                  // �� 
    {0x84AF, 0x2517},                  // �� 
    {0x84B0, 0x2523},                  // �� 
    {0x84B1, 0x2533},                  // �� 
    {0x84B2, 0x252B},                  // �� 
    {0x84B3, 0x253B},                  // �� 
    {0x84B4, 0x254B},                  // �� 
    {0x84B5, 0x2520},                  // �� 
    {0x84B6, 0x252F},                  // �� 
    {0x84B7, 0x2528},                  // �� 
    {0x84B8, 0x2537},                  // �� 
    {0x84B9, 0x253F},                  // �� 
    {0x84BA, 0x251D},                  // �� 
    {0x84BB, 0x2530},                  // �� 
    {0x84BC, 0x2525},                  // �� 
    {0x84BD, 0x2538},                  // �� 
    {0x84BE, 0x2542},                  // �� 
};

#endif /* defined(MB_CHANGE_TO_UNICODE) */

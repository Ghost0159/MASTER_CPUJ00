/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - include
  File:     MbGameInfo.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_gameinfo.h,v $
  Revision 1.12  2006/03/13 06:33:33  yosizaki
  add MB_FAKESCAN_PARENT_BEACON.

  Revision 1.11  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.10  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.9  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.8  2004/11/22 12:50:31  takano_makoto
  MBGameInfoRecvList, MBGameInfoRecvStatus��mb_child.h����ړ�

  Revision 1.7  2004/11/10 07:42:51  takano_makoto
  mb_gameinfo.c�̒��̎q�@���̏�������MBw�֐���r��

  Revision 1.5  2004/09/14 23:55:48  yosizaki
  disable MB_CHANGE_TO_UNICODE
      (change MBGameregistry's member to Unicode)

  Revision 1.4  2004/09/13 09:24:39  yosiokat
  �E�X�L�������b�N���Ƀ��b�N�Ώۂ��S��������Ȃ���Ԃ̑΍�̂��߁AMB_CountGameInfoLifetime�̈�����wmTypeEvent��ǉ��B

  Revision 1.3  2004/09/09 14:02:23  sato_masaki
  mb_gameinfo.c�ɂ�����APlayerFlag�̃t�H�[�}�b�g�ύX�B�i��ƒ��j

  Revision 1.2  2004/09/08 04:34:47  sato_masaki
  small fix

  Revision 1.1  2004/09/03 07:06:28  sato_masaki
  �t�@�C�����@�\�ʂɕ����B

  Revision 1.6  2004/09/01 09:40:51  sato_masaki
  MB_AUTHCODE_SIZE�̒l�ύX�B

  Revision 1.5  2004/08/30 06:31:38  sato_masaki
  MBDownloadFileInfo�ɔF�ؗp�R�[�hauth_code��ǉ��B

  Revision 1.4  2004/08/19 07:43:11  sato_masaki
  GameInfo�̃t���O�Ɋւ���}�N����`��ǉ�

  Revision 1.3  2004/08/12 09:15:52  yosiokat
  �EMbBeaconRecvStatus�ɃX�L�������b�N�����J�E���g�pnowLockTimeCount��lockTimeMaxCount��ǉ��B

  Revision 1.2  2004/08/09 14:55:22  yosizaki
  format all the symbol for API

  Revision 1.1  2004/08/07 07:03:27  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


/*
 * ���̃w�b�_�t�@�C���͓��������y�юq�@���ł̂ݎg�p���܂�.
 * �ʏ�̃}���`�u�[�g�e�@���쐬�����ł͕K�v����܂���.
 */


#ifndef MB_GAME_INFO_H_
#define MB_GAME_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <nitro/types.h>
#include <nitro/wm.h>
#include <nitro/mb/mb_gameinfo.h>


//=============================================================================
// 
// �f�[�^�^�@��`
//
//=============================================================================

#define MB_GAMEINFO_PARENT_FLAG                     (0x0001)
#define MB_GAMEINFO_CHILD_FLAG( __child_aid__ )     ( 1 << ( __child_aid__ ) )

//---------------------------------------------------------
// �e�@����q�@�Ƀr�[�R���Ŕz�M����Q�[�����
//---------------------------------------------------------

/*
 * �q�@���Q�[������M���X�g�\����.
 */
typedef struct MBGameInfoRecvList
{
    MBGameInfo gameInfo;               // �e�@�Q�[�����
    WMBssDesc bssDesc;                 // �e�@�ڑ��p���
    u32     getFlagmentFlag;           // ���ݎ�M�����f�Ѓr�[�R�����r�b�g�Ŏ����B
    u32     allFlagmentFlag;           // flagmentMaxNum���r�b�g�Ɋ��Z�����l
    u16     getPlayerFlag;             // ���ݎ�M�ς݂̃v���C���[�t���O���r�b�g�Ŏ����B
    s16     lifetimeCount;             // ���̏��ɑ΂�������J�E���^�i���̐e�@�̃r�[�R������M������A���������т�j
    u16     linkLevel;                 /* �e�@�����Beacon��M���x�������A4�i�K�̒l. */
    u8      beaconNo;                  // �Ō�Ɏ�M�����r�[�R���ԍ�
    u8      sameBeaconRecvCount;       // �����r�[�R���ԍ���A�����Ď�M������
}
MBGameInfoRecvList, MbGameInfoRecvList;


/*
 * �q�@���r�[�R����M�X�e�[�^�X�\����
 */
typedef struct MbBeaconRecvStatus
{
    u16     usingGameInfoFlag;         // �Q�[�����̎�M�Ɏg�p����gameInfo�z��v�f���r�b�g�Ŏ����B
    u16     usefulGameInfoFlag;        // ��U�͑S�Ẵr�[�R������M���āAvalidGameInfoFlag���������Q�[�����������B
    // �i�ʐM�����o�[�̍X�V���ŁAvalidGameInfoFlag�͈ꎞ�I�ɗ�����ꍇ������̂ŁA�\����ڑ��ɂ͂�����̃t���O���g�p���Ĕ��f����B�j
    u16     validGameInfoFlag;         // �Q�[���������S�Ɏ�M���Ă���gameInfo�z��v�f���r�b�g�Ŏ����B
    u16     nowScanTargetFlag;         // ���݂�Scan�^�[�Q�b�g���r�b�g�Ŏ����B
    s16     nowLockTimeCount;          // ���݂�Scan�^�[�Q�b�g�̎c�胍�b�N���ԁB
    s16     notFoundLockTargetCount;   // ���݂�ScanTarget���A���Ō�����Ȃ�������
    u16     scanCountUnit;             // ���݂̃X�L�������Ԃ��J�E���g���Ɋ��Z�����l
    u8      pad[2];
    MBGameInfoRecvList list[MB_GAME_INFO_RECV_LIST_NUM];        // �Q�[������M���X�g
}
MbBeaconRecvStatus;
/*
 * ���q�@���́A
 * getFlagmentFlag == allFlagmentFlag�@���@
 * getPlayerFlag   == gameInfo.volat.nowPlayerFlag
 * �ŁA�e�@���擾�����ƂȂ�B
 */


/*
 * MB_RecvGameInfoBeacon, MB_CountGameInfoLifetime ��
 * �R�[���o�b�N�֐��ŕԂ����msg.
 */
typedef enum MbBeaconMsg
{
    MB_BC_MSG_GINFO_VALIDATED = 1,
    MB_BC_MSG_GINFO_INVALIDATED,
    MB_BC_MSG_GINFO_LOST,
    MB_BC_MSG_GINFO_LIST_FULL,
    MB_BC_MSG_GINFO_BEACON
}
MbBeaconMsg;

typedef void (*MBBeaconMsgCallback) (MbBeaconMsg msg, MBGameInfoRecvList * gInfop, int index);

typedef void (*MbScanLockFunc) (u8 *macAddress);
typedef void (*MbScanUnlockFunc) (void);

/******************************************************************************/
/* �ȉ��͓����g�p */


//------------------
// �e�@���֐�
//------------------

    // ���M�X�e�[�^�X�̏������B
void    MB_InitSendGameInfoStatus(void);

    // MbGameRegistry����MbGameInfo���쐬����B
void    MBi_MakeGameInfo(MBGameInfo *gameInfop,
                         const MBGameRegistry *mbGameRegp, const MBUserInfo *parent);

    // MBGameInfo�̎q�@�����o�[�����X�V����B
void    MB_UpdateGameInfoMember(MBGameInfo *gameInfop,
                                const MBUserInfo *member, u16 nowPlayerFlag, u16 changePlayerFlag);

    // ��������MBGameInfo���r�[�R���Ŕ��M����悤���M���X�g�ɒǉ�����B
void    MB_AddGameInfo(MBGameInfo *newGameInfop);

    // ���M���X�g�ɒǉ����Ă���MBGameInfo���폜����B
BOOL    MB_DeleteGameInfo(MBGameInfo *gameInfop);

    // ���M���X�g�ɓo�^����Ă���MBGameInfo���r�[�R���ɏ悹�Ĕ��M����B
void    MB_SendGameInfoBeacon(u32 ggid, u16 tgid, u8 attribute);


//------------------
// �q�@���֐�
//------------------
    // �r�[�R����M�X�e�[�^�X�o�b�t�@��static�Ɋm��
void    MBi_SetBeaconRecvStatusBufferDefault(void);
    // �r�[�R����M�X�e�[�^�X�o�b�t�@��ݒ�
void    MBi_SetBeaconRecvStatusBuffer(MbBeaconRecvStatus * buf);

    // ��M�X�e�[�^�X�̏�����
void    MB_InitRecvGameInfoStatus(void);

    // ��M�����r�[�R������MBGameInfo�����o���B
BOOL    MB_RecvGameInfoBeacon(MBBeaconMsgCallback Callbackp, u16 linkLevel, WMBssDesc *bssDescp);

    // �e�@��񃊃X�g�̎����J�E���g
void    MB_CountGameInfoLifetime(MBBeaconMsgCallback Callbackp, BOOL found_parent);

    // �X�L�������b�N�֐���ݒ�
void    MBi_SetScanLockFunc(MbScanLockFunc lockFunc, MbScanUnlockFunc unlockFunc);

    // ��M�����e�@���\���̂ւ̃|�C���^���擾����
MBGameInfoRecvList *MB_GetGameInfoRecvList(int index);

/* �r�[�R����M��Ԃ��擾 */
const MbBeaconRecvStatus *MB_GetBeaconRecvStatus(void);

/* �w�肳�ꂽ�Q�[�����̍폜 */
void    MB_DeleteRecvGameInfo(int index);
void    MB_DeleteRecvGameInfoWithoutBssdesc(int index);

//------------------
// Unicode �ڍs�p�V���{��.
//------------------
//#define   MB_CHANGE_TO_UNICODE

#if defined(MB_CHANGE_TO_UNICODE)

void    MBi_ExSJIS_BEtoUTF16_LE(u8 *sjisp, u16 *unip, u16 length);
void    MBi_ExUTF16_LEtoSJIS_BE(u8 *sjisp, u16 *unip, u16 length);
void    MBi_CheckSJIS_BEtoUTF16_LE(void);

#endif /* defined(MB_CHANGE_TO_UNICODE) */

#ifdef __cplusplus
}
#endif

#endif // MB_GAME_INFO_H_

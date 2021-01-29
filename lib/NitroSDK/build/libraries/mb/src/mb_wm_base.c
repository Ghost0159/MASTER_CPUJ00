/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_wm_base.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_wm_base.c,v $
  Revision 1.18  2007/10/03 06:32:42  yosizaki
  fix to disable Beacon-indication on MB_EndToIdle().

  Revision 1.17  2007/07/30 03:32:58  yosizaki
  fix about MB_End

  Revision 1.16  2007/01/19 04:34:37  yosizaki
  copyright���X�V.

  Revision 1.15  2007/01/18 05:58:03  yosizaki
  fix about finalization.

  Revision 1.14  2006/07/19 04:41:30  yosizaki
  change default lifetime parameters.

  Revision 1.13  2006/02/20 02:35:20  seiki_masashi
  WM_GAMEINFO_TYPE_OLD �̃T�|�[�g��p�~

  Revision 1.12  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.11  2005/12/13 10:41:29  yosizaki
  remove temporary hook for SEND_FAILED

  Revision 1.10  2005/12/09 08:16:10  yosizaki
  add hook for SEND_FAILED (temporary)

  Revision 1.9  2005/11/14 05:37:28  yosizaki
  fix about WM_ATTR_FLAG_ENTRY flag.

  Revision 1.8  2005/11/07 01:21:50  okubata_ryoma
  SDK_STATIC_ASSERT����SDK_COMPILER_ASSERT�ɕύX

  Revision 1.7  2005/10/28 11:23:41  seiki_masashi
  WM_STATECODE_DISCONNECTED_FROM_MYSELF �̒ǉ��ɑΉ�
  WM_STATECODE_PORT_INIT �̒ǉ��ɑΉ�

  Revision 1.6  2005/07/29 03:45:01  seiki_masashi
  WM_STATECODE_INFORMATION �̒ǉ�

  Revision 1.5  2005/04/14 04:40:20  yosizaki
  fix timing of calling WM_GetNextTgid().

  Revision 1.4  2005/04/11 08:51:56  yosizaki
  move MBi_CreateTgid() to wm_sync.c as WM_GetNextTgid().

  Revision 1.3  2005/04/11 01:45:48  yosizaki
  add MB_SetLifeTime().
  add MB_SetPowerSaveMode().
  add MBi_CreateTgid().

  Revision 1.2  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.1  2005/02/21 00:40:08  yosizaki
  replace filename from tmp_lib_stub.


  Revision 1.62  2005/02/18 11:56:03  yosizaki
  fix around hidden warnings.

  Revision 1.61  2005/01/27 07:07:49  takano_makoto
  ichneumon�g�p���ɃE�F�C�g������悤�ɏC��

  Revision 1.60  2004/12/28 09:32:28  pair2
  fix around MB_EndToIdle. (check endReq == 1 or pWork == NULL )

  Revision 1.59  2004/11/29 04:47:41  yosizaki
  allocate sendBuf. (avoid to share with SetMPData's buffer)

  Revision 1.58  2004/11/24 05:14:33  yosizaki
  rename MB_StartParentEx/MB_EndEx..

  Revision 1.57  2004/11/22 12:47:26  takano_makoto
  �������N�G�X�g���M�̏�����mb_block�Ƃ��ĕ���

  Revision 1.56  2004/11/18 07:28:46  takano_makoto
  MB_SetParentCommSize, MB_SetParentCommParam�Ŏq�@���M�T�C�Y�̐ݒ������p�~

  Revision 1.55  2004/11/11 11:46:56  yosizaki
  change WM_End/Ex to finish task-thread before WM_Reset.

  Revision 1.54  2004/11/10 07:40:03  takano_makoto
  mb_gameinfo.c�̒�����MBw�֐���r�����A�X�L�����p�����[�^�̐ݒ��؂�o��

  Revision 1.53  2004/11/10 03:17:02  takano_makoto
  MBi_SetBeaconRecvStatusBufferDefault��ǉ�

  Revision 1.52  2004/11/09 12:02:23  yosizaki
  fix allocation of wmBuf.

  Revision 1.51  2004/11/09 01:05:39  seiki_masashi
  Null �����ɂ��^�C���A�E�g������

  Revision 1.50  2004/10/26 08:09:54  takano_makoto
  MB_DisconnectChild��ǉ�

  Revision 1.49  2004/10/19 00:55:31  sato_masaki
  WM_API�̕Ԃ�l�G���[��MB_CALLBACK_API_ERROR�ŕԂ���Ă��Ȃ������ӏ�������̂��C���B

  Revision 1.48  2004/10/18 11:44:36  yosizaki
  add MB_StartParentEx, MB_EndEx.

  Revision 1.47  2004/10/15 08:50:44  yosizaki
  add start_mp_busy.

  Revision 1.46  2004/10/08 10:32:49  takano_makoto
  IPL�N����ʂ�nameLength�̕ύX�����f����Ȃ��s����C��

  Revision 1.45  2004/10/08 03:04:17  takano_makoto
  1.44�̏C���ɒǉ�����NUL�������`�F�b�N����悤�ɏC��

  Revision 1.44  2004/10/08 02:44:46  takano_makoto
  MB_Init��MBUserInfo��nameLength������悤�ɏC��

  Revision 1.43  2004/10/05 09:45:29  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.42  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.41  2004/10/03 08:17:31  ooe
  StartMPEx�̎d�l�ύX�ɑΉ��BignoreFatalError�B

  Revision 1.40  2004/09/28 08:55:30  sato_masaki
  - Indication callback�ŁAWM_STATECODE_FIFO_ERROR���o���Ƃ��́AOS_Panic���o���悤�ɕύX�B
  - MBi_IsCommSizeValid�ɂāA�T�C�Y�͈͂̔����ǉ��B

  Revision 1.39  2004/09/25 05:51:47  sato_masaki
  NitroSDK_IPL_branch 1.27.2.7 �ƃ}�[�W

  Revision 1.38  2004/09/25 05:43:49  sato_masaki
  ���C�����[�v�ɂ����ČĂ΂꓾��API���s���ɁA���荞�݋֎~�����鏈����ǉ��B

  Revision 1.37  2004/09/22 09:41:18  sato_masaki
  IPL_branch 1.27.2.5�ƃ}�[�W

  Revision 1.36  2004/09/22 02:50:31  yosizaki
  optimize for code-size around MBi_ChildCallback().

  Revision 1.35  2004/09/22 02:31:57  yosizaki
  optimize for code-size around MB_Init().

  Revision 1.34  2004/09/21 01:39:15  sato_masaki
  WM�ɋN������G���[���AMB_ERRCODE_WM_FAILURE�ɓ���BMB_ERRCODE_MP_SENT_FAILURE��p�~�B

  Revision 1.33  2004/09/20 12:56:17  sato_masaki
  MB_Init()�ɂ����āA�������ς݂ł��邩�ۂ��̔����ǉ��B

  Revision 1.32  2004/09/18 09:16:26  sato_masaki
  p_mbi_param->endReq�t���O�������Ă���ꍇ�́AMB_End()�͉������Ȃ��悤�ɕύX�B

  Revision 1.31  2004/09/18 03:11:25  sato_masaki
  small fix

  Revision 1.30  2004/09/18 02:58:50  sato_masaki
  bug fix

  Revision 1.29  2004/09/17 11:36:30  sato_masaki
  - add function MBi_IsStarted()
  - change MP size check (it always returns 'TRUE')

  Revision 1.28  2004/09/16 12:48:35  sato_masaki
  - mb.h�ɂāAMBi_***�Œ�`���Ă����֐����AMB_***�ɕύX�B
  - ����M�f�[�^�T�C�Y�̐������]��������悤�ɂ����B

  Revision 1.27  2004/09/15 06:51:59  sato_masaki
  WBT��`�ɓZ��鏈�����폜�B

  Revision 1.26  2004/09/15 06:17:47  sato_masaki
  add MBi_SetParentCommSize, MBi_GetTgid functions

  Revision 1.25  2004/09/14 12:07:15  terui
  rand�֐����폜�AMAC�A�h���X�ɂ��r�[�R���Ԋu���΂������@�\�ɍ��ւ��B

  Revision 1.24  2004/09/14 02:55:29  sato_masaki
  comment �C��

  Revision 1.23  2004/09/14 02:41:38  sato_masaki
  MBiParam �� mpBusy �ǉ��B

  Revision 1.21  2004/09/13 13:08:26  sato_masaki
  - MBi_ChildPortCallback()��ǉ��B
  - WM_STATECODE_CHILD_CONNECTED�̂Ƃ��́AMP�Z�b�g�����Ȃ��悤�ɕύX�B

  Revision 1.20  2004/09/13 02:56:23  sato_masaki
  ���d�l�Ƃ̌݊��̂��߁Atgid�������Ɏ�����MBi_Init()��V�݁B

  Revision 1.19  2004/09/11 12:49:06  yosiokat
  �EMBi_StartCommon���ł̃X�L�������Ԑݒ��萔10����MB_SCAN_TIME_NORMAL�ɕύX�B

  Revision 1.18  2004/09/11 11:16:10  sato_masaki
  - �e�q���ꂼ��ɂ��āASDK_STATIC_ASSERT�ɂă��[�N�T�C�Y�̐������𔻒�B
  - MB_CommWork�\���̂�MB_CommPWork, MB_CommCWork, MB_CommCommonWork�ɕ���

  Revision 1.17  2004/09/11 07:02:08  yosiokat
  �ESDK_SMALL_BUILD��`�ɂ��e�q�R�[�h�������~�߁A���ʂɃr���h���ĕ����ł���悤�ύX����B
  �@MBi_StartCore�̏����̋��ʉ��ł��镔����MBi_StartCommon�Ƃ��A����ȊO��MB_StartParent��MB_StartChild�Ɉڂ��B
  �@MBi_SendMP���ł�MODE_PARENT���̃R�[���o�b�N�w���MBi_ParentCallback����p_mbi_param->callback_ptr�ɕύX�B

  Revision 1.16  2004/09/11 05:01:26  terui
  ����M�o�b�t�@�T�C�Y�v�Z�}�N���̎b��΍�����ɖ߂����B

  Revision 1.15  2004/09/11 04:00:21  sato_masaki
  WM_Calc*******()�}�N�����́A�L�[�V�F�A�����O�g�p������BOOL�l��TRUE�ɂ���B
  (WM_StartMPEx()�̃G���[����̂��߂̈ꎞ�I�Ή���B)

  Revision 1.14  2004/09/10 12:32:47  seiki_masashi
  �L�[�V�F�A�����O�̑��M�o�C�g�������C�u�������ŉB������悤�ɕύX

  Revision 1.13  2004/09/10 07:50:15  sato_masaki
  changeScanChannel()�֐��ɂāAWM_GetAllowedChannel()����擾����
  �g�p�\channel�̒�����AScanChannel��I�肷��悤�ɕύX�B
  �܂��A�X�L�����\��Channel�����݂��Ȃ��ꍇ�AMBi_CommEnd()���ĂԂ悤�ɕύX�B

  Revision 1.12  2004/09/10 02:32:23  sato_masaki
  WM_StartScan()���s����scanParam.channel�l��ύX�B

  Revision 1.11  2004/09/09 06:49:08  sato_masaki
  bug fix about changeScanChannel()

  Revision 1.10  2004/09/09 06:29:49  yosizaki
  change WM_SetMPData to use ToPort(WM_PORT_BT, WM_PRIORITY_LOW).

  Revision 1.9  2004/09/09 02:46:53  sato_masaki
  CAM_LIFE_TIME��40�ɖ߂��B

  Revision 1.8  2004/09/09 01:33:18  sato_masaki
  FRAME_LIFE_TIME��5�ɖ߂��ACAM_LIFE_TIME��2*40�ɐݒ�B

  Revision 1.7  2004/09/09 00:50:03  sato_masaki
  FRAME_LIFE_TIME��10�ɐݒ�B

  Revision 1.6  2004/09/08 13:10:09  sato_masaki
  WM_ERRCODE_INVALID_POLLBITMAP�̎���MB_CALLBACK_MP_CHILD_RECV�R�[���o�b�N��Ԃ��悤�ɂ����B

  Revision 1.5  2004/09/08 10:58:54  sato_masaki
  - LIFETIME��L���ɐݒ�B
  - Scan����seek�`�����l����1,7,13�ɕύX
  - WM_StartMP��Ex�ɕύX���A�đ��������[�h�ɐݒ�B

  Revision 1.4  2004/09/06 12:05:24  sato_masaki
  StartConnect�Ɋւ���Comment���폜�B

  Revision 1.3  2004/09/06 11:43:21  sato_masaki
  MB_Init()�֐��̈����ɁAtgid��ǉ��B
  MBi_GetTgid()�֐��̒ǉ��B

  Revision 1.2  2004/09/04 09:29:03  yosiokat
  SDK_SMALL_BUILD��`���ɐe�@���[�h�̃R�[�h�����悤�ύX�B

  Revision 1.1  2004/09/03 07:04:36  sato_masaki
  �t�@�C�����@�\�ʂɕ����B

  Revision 1.25  2004/09/02 09:46:49  sato_masaki
  mbc->fileinfo��0�N���A��ǉ��B

  Revision 1.24  2004/08/30 06:32:46  sato_masaki
  ��`WMBT��WBT�ɕύX�B

  Revision 1.23  2004/08/26 10:08:57  sato_masaki
  BeaconRecv���̃X�e�[�g�`�F�b�N��ǉ�

  Revision 1.22  2004/08/26 09:54:59  sato_masaki
  small fix

  Revision 1.21  2004/08/24 12:20:06  sato_masaki
  SSID�ƍ��΍�(�ꎞ����)

  Revision 1.20  2004/08/23 04:45:17  ooe
  WM_StartConnect()�̎d�l�ύX�ɑΉ��B

  Revision 1.19  2004/08/19 22:55:17  yosiokat
  MB_Init�ł̃}���`�u�[�g�t���O�AGGID�̃Z�b�g���~�߂āA�r�[�R�����M����WM_SetGameInfo�ŃZ�b�g����悤�ɂ���B

  Revision 1.18  2004/08/19 16:00:09  miya
  modified WM_SetGameInfo function

  Revision 1.17  2004/08/19 15:11:47  yosiokat
  �O��C���̃R�����g�ǉ��BMB_Init��p_mbi_param->parentParam->multiBootFlag= 1�ɕύX�B

  Revision 1.16  2004/08/19 15:07:37  yosiokat
  WM_GAMEINFO_TYPE_OLD��`�̂�p_mbi_param->parentParam->userName�AgameName���Z�b�g����悤�C���B

  Revision 1.15  2004/08/19 12:27:20  sato_masaki
  Beacon Indicate�ł́A�ʐM�I���������R�����g�A�E�g

  Revision 1.14  2004/08/19 08:00:53  ikedae
  #include <string.h>�@�폜

  Revision 1.13  2004/08/19 07:51:40  sato_masaki
  WM�̕ύX�d�l�ɑΉ��B

  Revision 1.12  2004/08/19 07:34:16  ooe
  beaconPeriod�̃����_����(�b�� �����p)

  Revision 1.11  2004/08/19 02:42:36  yosizaki
  change type of MBUserInfo.

  Revision 1.10  2004/08/18 01:25:09  miya
  small fix

  Revision 1.9  2004/08/18 00:05:14  sato_masaki
  WM_StartMP�̈����ύX�ɔ����X�V

  Revision 1.8  2004/08/16 13:02:40  sato_masaki
  MBi_GetSendBufSize�֐���ǉ�

  Revision 1.7  2004/08/12 09:20:57  yosiokat
  �EMBi_RestartScan�AMBi_SetMaxScanTime�AMBi_GetScanChannel�֐��̒ǉ��B
  �E�X�L��������PARENT_NOT_FOUND�������ꍇ�ɂ��AMB_Comm���C���ɃR�[���o�b�N��Ԃ��悤�ύX�B

  Revision 1.6  2004/08/10 12:11:55  sato_masaki
  MBi_GetAid�֐��̒ǉ��B

  Revision 1.5  2004/08/09 14:56:43  yosizaki
  format all the symbol for API

  $NoKeywords: $
 *---------------------------------------------------------------------------*/



#include "mb_private.h"


/*
 * �v�����Ă���T�C�Y�ŗe�ʂ�������������.
 */
SDK_COMPILER_ASSERT(32 + sizeof(MBiParam) + 32 + sizeof(MB_CommPWork) + 32 + WM_SYSTEM_BUF_SIZE <=
                    MB_SYSTEM_BUF_SIZE);
SDK_COMPILER_ASSERT(32 + sizeof(MBiParam) + 32 + sizeof(MB_CommCWork) + 32 + WM_SYSTEM_BUF_SIZE <=
                    MB_CHILD_SYSTEM_BUF_SIZE);


/*
 * �e/�q ����M�o�b�t�@�T�C�Y�v�Z�}�N��
 * ��M�o�b�t�@  parentMaxSize + WLHeader(48B) + WMHeader(2B)
 * ���M�o�b�t�@ (parentMaxSize + WMHeader(2B) + (KeySet(36B) KS���̂�) + 31) & 0xffffffe0 (�A���C�����g�C��)
 * ��M�o�b�t�@ (childMaxSize + WLHeader(8B) + WMHeader(2B) + (KeyData(2B)(KeySharing���̂�)) ��15�䕪 �{ �S�̂�Header(8B) + 31) & 0xffffffe0 (�A���C�����g�C��)
 */
#define WL_HEADER_LENGTH_P  48         /* �e�@��M����WL�w�b�_�� */
#define WM_HEADER_LENGTH    2          /* WMHeader�� */
#define WL_HEADER_LENGTH_T  8          /* �S�̂�WL�w�b�_�� */
#define WL_HEADER_LENGTH_C  8          /* ���ꂼ��̎q�@�f�[�^��WL�w�b�_�� */


// �}�N�� �q�@��M�o�b�t�@�T�C�Y�v�Z

#define WM_CalcChildSendBufSize(_pInfo_)        (u16)(WM_SIZE_MP_CHILD_SEND_BUFFER(((WMGameInfo *)(&(((WMBssDesc*)(_pInfo_))->gameInfo)))->childMaxSize, FALSE))
#define WM_CalcChildRecvBufSize(_pInfo_)        (u16)(WM_SIZE_MP_CHILD_RECEIVE_BUFFER(((WMGameInfo *)(&(((WMBssDesc*)(_pInfo_))->gameInfo)))->parentMaxSize, FALSE))

// �}�N�� �e�@����M�o�b�t�@�T�C�Y�v�Z
#define WM_CalcParentSendBufSize(_libParam_)    (u16)(WM_SIZE_MP_PARENT_SEND_BUFFER(((MBiParam*)(_libParam_))->parentParam.parentMaxSize, FALSE))
#define WM_CalcParentRecvBufSize(_libParam_)    (u16)(WM_SIZE_MP_PARENT_RECEIVE_BUFFER(((MBiParam*)(_libParam_))->parentParam.childMaxSize, WM_NUM_MAX_CHILD, FALSE))


/* Beacon�Ԋu(ms)�f�t�H���g�l */
#define MB_BEACON_PERIOD_DEFAULT    (200)

/*
 * LifeTime �̐؂�ւ��f�o�b�O�X�C�b�`.
 */
#define  NO_LIFETIME    0
#if (NO_LIFETIME == 1)
#define FRAME_LIFE_TIME 0xFFFF
#define CAM_LIFE_TIME   0xFFFF
#define MP_LIFE_TIME    0xFFFF
#else
/* WM���C�u�������s����ł����������ɍs���Ă����΍� */
/*
#define FRAME_LIFE_TIME 0xFFFF
*/
#define FRAME_LIFE_TIME 5
#define CAM_LIFE_TIME   40
#define MP_LIFE_TIME    40
#endif

#define TABLE_NO    0xFFFF


static u16 mbi_life_table_no = TABLE_NO;
static u16 mbi_life_frame = FRAME_LIFE_TIME;
static u16 mbi_life_cam = CAM_LIFE_TIME;
static u16 mbi_life_mp = MP_LIFE_TIME;
static BOOL mbi_power_save_mode = TRUE;


//===========================================================
// �֐��v���g�^�C�v�錾
//===========================================================
static void MBi_ScanLock(u8 *macAddr); // �X�L�������b�N�ݒ�p�֐�
static void MBi_ScanUnlock(void);      // �X�L�������b�N�����p�֐�

/*
 * WM ���ʒl���`�F�b�N���ăG���[�Ȃ�R�[���o�b�N
 */
static void MBi_CheckWmErrcode(u16 apiid, int errcode);
static void MBi_ParentCallback(void *arg);



/******************************************************************************/
/* �ϐ� */

static MBiParam *p_mbi_param;
static u16 WM_DMA_NO;

/*
 * WM�V�X�e���o�b�t�@.
 * ���� MBiParam �����o��������,
 * MB_StartParentFromIdle() �̂��߂ɓƗ�.
 */
static u8 *wmBuf;


MB_CommCommonWork *mbc = NULL;


/* �q�@�p�ϐ� */
static WMscanParam scanParam ATTRIBUTE_ALIGN(32);


/******************************************************************************/
/* �֐� */


/* ScanChannel ������ύX���Ă��� */
static BOOL changeScanChannel(WMscanParam *p)
{
    u16     channel_bmp, channel, i;

    /* channel bitmap �擾 */
    channel_bmp = WM_GetAllowedChannel();

    /* �g�p�\��channel�������ꍇ�́AFALSE��Ԃ��D */
    if (channel_bmp == 0)
    {
        OS_TWarning("No Available Scan channel\n");
        return FALSE;
    }

    /* �g�p�\��channel�����݂����ꍇ�D */
    for (i = 0, channel = p->channel;
         i < 16; i++, channel = (u16)((channel == 16) ? 1 : channel + 1))
    {
        if ((channel_bmp & (1 << (channel - 1))) == 0)
        {
            continue;
        }

        /* ���o���ꂽchannel���O�̂��̂Ɠ����������ꍇ�͕ʂ�channel���������� */
        if (p->channel != channel)
        {
            p->channel = channel;
            break;
        }
    }

    return TRUE;

}


/*---------------------------------------------------------------------------*
  Name:         MB_SetLifeTime

  Description:  MB �̃��C�����X�쓮�ɑ΂��ă��C�t�^�C���𖾎��I�Ɏw��.
                �f�t�H���g�ł� ( 0xFFFF, 40, 0xFFFF, 40 ) �ɂȂ��Ă���.

  Arguments:    tgid            �w�肳��� TGID

  Returns:      ���������̂܂܂�, �܂��͓K�؂� TGID.
 *---------------------------------------------------------------------------*/
void MB_SetLifeTime(u16 tableNumber, u16 camLifeTime, u16 frameLifeTime, u16 mpLifeTime)
{
    mbi_life_table_no = tableNumber;
    mbi_life_cam = camLifeTime;
    mbi_life_frame = frameLifeTime;
    mbi_life_mp = mpLifeTime;
}

/*---------------------------------------------------------------------------*
  Name:         MB_SetPowerSaveMode

  Description:  �펞�ʓd���[�h��ݒ肷��.
                ����͓d�͏�����l�����Ȃ��ėǂ���ʂň���ɋ쓮���邽�߂�
                �I�v�V�����ł���f�t�H���g�ł͖����ƂȂ��Ă���.
                �d���ɂȂ���Ă��邱�Ƃ�ۏ؂��ꂽ������łȂ�����,
                �ʏ�̃Q�[���A�v���P�[�V������������g�p���ׂ��ł͂Ȃ�.

  Arguments:    enable           �L���ɂ���Ȃ�� TRUE, �����ɂ���Ȃ�� FALSE.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MB_SetPowerSaveMode(BOOL enable)
{
    mbi_power_save_mode = enable;
}


/*
 * �������𐬌��Ƃ݂Ȃ��ĕϊ����� WM ���ʒl��Ԃ�
 */
static inline int conv_errcode(int errcode)
{
    return (errcode == WM_ERRCODE_OPERATING) ? WM_ERRCODE_SUCCESS : errcode;
}


/*
 * MP���M���`�F�b�N
   
   SetMP��A�R�[���o�b�N���Ԃ�O�ɁA�Ă�MP���Z�b�g���Ă��܂�Ȃ��悤�ɁA
   SetMP���s���ɗ��Ă�AmpBusy�̃t���O�𔻒�v�f�ɒǉ��B
 
 */
static BOOL MBi_IsSendEnabled(void)
{
    return (p_mbi_param->mpStarted == 1) &&
        (p_mbi_param->mpBusy == 0) &&
        (p_mbi_param->endReq == 0) && (p_mbi_param->child_bitmap != 0);
}

static void MBi_OnInitializeDone(void)
{
    int     errcode;
    /* API�̔��s */
    errcode = WM_SetIndCallback(MBi_ParentCallback);
    MBi_CheckWmErrcode(WM_APIID_INDICATION, errcode);
    errcode = WM_SetLifeTime(MBi_ParentCallback, mbi_life_table_no,
                             mbi_life_cam, mbi_life_frame, mbi_life_mp);
    MBi_CheckWmErrcode(WM_APIID_SET_LIFETIME, errcode);
}


/* 
 * MB�e�@��ichneumon���g�p����VRAM�Ŗ����𓮍삳���Ă���ꍇ�A
 * �q�@�䐔�����Ȃ��Ɛe�@��MP�f�[�^�ݒ肪�����ɏ�������߂��Ă��܂��A
 * IPL���e�@��MP�ʐM�ɒǂ����Ȃ��B
 * ���̑΍�̂��߁A��莞�Ԃ̃E�F�C�g������B
 */
static inline void MbWaitForWvr(u32 cycles)
{
    u32     child_cnt = 0;
    u32     i;

    for (i = 0; i < MB_MAX_CHILD; i++)
    {
        if (pPwork->p_comm_state[i] != MB_COMM_PSTATE_NONE)
        {
            if (++child_cnt >= 2)
                break;
        }
    }
    // �q�@����䂾���̏ꍇ�ɃE�F�C�g����������B
    if (child_cnt == 1)
    {
        OS_SpinWait(cycles);
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBi_EndCommon

  Description:  ���ʂ�MB�I������.

  Arguments:    arg               ���[�U�ւ̏I���R�[���o�b�N����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBi_EndCommon(void *arg)
{
    p_mbi_param->mbIsStarted = 0;
    mbc->isMbInitialized = 0;
    if (p_mbi_param->callback)
    {
        p_mbi_param->callback(MB_CALLBACK_END_COMPLETE, arg);
    }
}

/*
 * �e�@�R�[���o�b�N
 */
static void MBi_ParentCallback(void *arg)
{
    int     errcode;
    WMCallback *buf = (WMCallback *)arg;

    switch (buf->apiid)
    {
    case WM_APIID_INITIALIZE:
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }
            MBi_OnInitializeDone();
        }
        break;

    case WM_APIID_SET_LIFETIME:
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }

            // API�̔��s
            errcode = WM_SetParentParameter(MBi_ParentCallback, &p_mbi_param->parentParam);
            MBi_CheckWmErrcode(WM_APIID_SET_P_PARAM, errcode);
        }
        break;

    case WM_APIID_SET_P_PARAM:
        {
            p_mbi_param->callback(MB_CALLBACK_INIT_COMPLETE, arg);
            // �����ł̓G���[�R�[�h�͕Ԃ��Ă��Ȃ�
            errcode = WM_SetBeaconIndication(MBi_ParentCallback, 1 /* 1:ON, 0:OFF */ );
            MBi_CheckWmErrcode(WM_APIID_SET_BEACON_IND, errcode);
        }
        break;

        // Beacon Send/Recv Indication�𔭐������邩�ǂ���
    case WM_APIID_SET_BEACON_IND:
        if (!p_mbi_param->endReq)
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }

            errcode = WMi_StartParentEx(MBi_ParentCallback, mbi_power_save_mode);
            MBi_CheckWmErrcode(WM_APIID_START_PARENT, errcode);
        }
        else
        {
            // �r�[�R���ʒm�𖳌������A�I����������
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_mbi_param->endReq = 0;
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
            }
            else
            {
                MBi_EndCommon(arg);
            }
        }
        break;

    case WM_APIID_START_PARENT:
        {
            WMstartParentCallback *callback = (WMstartParentCallback *)arg;

            if (callback->errcode != WM_ERRCODE_SUCCESS)
            {
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }

            switch (callback->state)
            {
            case WM_STATECODE_PARENT_START:
                p_mbi_param->child_bitmap = 0;
                p_mbi_param->mpStarted = 0;
                break;
            case WM_STATECODE_CHILD_CONNECTED:

                /* MB_End()�R�[����͋����I��break */
                if (p_mbi_param->endReq == 1)
                    break;

                p_mbi_param->child_bitmap |= (0x1 << callback->aid);
                p_mbi_param->callback(MB_CALLBACK_CHILD_CONNECTED, arg);

                // MP�J�n�O�Ȃ�MP�J�n
                if ((p_mbi_param->mpStarted == 0) && (!mbc->start_mp_busy))
                {
                    mbc->start_mp_busy = TRUE;
                    errcode = WM_StartMPEx(MBi_ParentCallback, (u16 *)p_mbi_param->recvBuf, p_mbi_param->recvBufSize, (u16 *)p_mbi_param->sendBuf, p_mbi_param->sendBufSize, (u16)(p_mbi_param->contSend ? 0 : 1), 0,   /* defaultRetryCount */
                                           FALSE, FALSE, 1,     /* fixFreqMode */
                                           TRUE /* ignoreFatalError */
                        );
                    MBi_CheckWmErrcode(WM_APIID_START_MP, errcode);

                }
                else
                {
                    // MP���M���R�[���o�b�N
                    if (MBi_IsSendEnabled())
                    {
                        p_mbi_param->callback(MB_CALLBACK_MP_SEND_ENABLE, NULL);
                    }
                }
                break;

            case WM_STATECODE_DISCONNECTED:
                p_mbi_param->child_bitmap &= ~(0x1 << callback->aid);
                p_mbi_param->callback(MB_CALLBACK_CHILD_DISCONNECTED, arg);
                break;

            case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
                // ����ؒf�����ꍇ�͏������Ȃ�
                break;

            case WM_STATECODE_BEACON_SENT:
                /* MB_End()�R�[����͋����I��break */
                if (p_mbi_param->endReq == 1)
                    break;

                // �r�[�R�����M�����ʒm
                p_mbi_param->callback(MB_CALLBACK_BEACON_SENT, arg);
                break;

            default:
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
                break;
            }
        }
        break;

    case WM_APIID_START_MP:
        {
            // �����ł̓G���[�R�[�h�͕Ԃ��Ă��Ȃ�
            WMstartMPCallback *callback = (WMstartMPCallback *)arg;
            mbc->start_mp_busy = FALSE;
            switch (callback->state)
            {
            case WM_STATECODE_MP_START:
                // MP�J�n�ς݃t���O���Z�b�g
                p_mbi_param->mpStarted = 1;
                {
                    // MP���M���R�[���o�b�N
                    if (p_mbi_param->endReq == 0)
                    {
                        p_mbi_param->callback(MB_CALLBACK_MP_SEND_ENABLE, NULL);
                    }
                }
                break;

            case WM_STATECODE_MPEND_IND:
                // MP��M�R�[���o�b�N
                p_mbi_param->callback(MB_CALLBACK_MP_PARENT_RECV, (void *)(callback->recvBuf));
                break;

            default:
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
                break;
            }
            break;
        }
        break;

    case WM_APIID_SET_MP_DATA:
        {
            /* MB�e�@��ichneumon���g�p���Ă���ꍇ�ɃE�F�C�g������ */
            if (pPwork->useWvrFlag)
            {
                MbWaitForWvr(13000);   // ARM9��IRQ���荞�ݒ��ɖ�3���C���E�F�C�g������
            }
            p_mbi_param->mpBusy = 0;
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                p_mbi_param->callback(MB_CALLBACK_MP_PARENT_SENT, arg);
                if (p_mbi_param->endReq == 0)
                {
                    p_mbi_param->callback(MB_CALLBACK_MP_SEND_ENABLE, NULL);        // ���̑��M������
                }
            }
            else if (buf->errcode == WM_ERRCODE_SEND_QUEUE_FULL)
            {
                p_mbi_param->callback(MB_CALLBACK_SEND_QUEUE_FULL_ERR, arg);
            }
            else
            {
                p_mbi_param->callback(MB_CALLBACK_MP_PARENT_SENT_ERR, arg);
                if (p_mbi_param->endReq == 0)
                {
                    p_mbi_param->callback(MB_CALLBACK_MP_SEND_ENABLE, NULL);        // ���̑��M������
                }
            }
        }
        break;

    case WM_APIID_RESET:
        if (!mbc->is_started_ex)
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_mbi_param->endReq = 0;
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
            }
            else
            {
                p_mbi_param->child_bitmap = 0;
                p_mbi_param->mpStarted = 0;
                errcode = WM_End(MBi_ParentCallback);
                MBi_CheckWmErrcode(WM_APIID_END, errcode);
            }
        }
        else
        {
            (void)WM_SetPortCallback(WM_PORT_BT, NULL, NULL);
            (void)WM_SetIndCallback(NULL);
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_mbi_param->endReq = 0;
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
            }
            else
            {
                errcode = WM_SetBeaconIndication(MBi_ParentCallback, 0);
                MBi_CheckWmErrcode(WM_APIID_SET_BEACON_IND, errcode);
            }
        }
        break;
    case WM_APIID_END:
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_mbi_param->endReq = 0;
                p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
            }
            else
            {
                MBi_EndCommon(arg);
            }
        }
        break;

    case WM_APIID_DISCONNECT:
        {
            WMDisconnectCallback *callback = (WMDisconnectCallback *)arg;

            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                return;
            }

            // �q�@���X�V
            p_mbi_param->child_bitmap &= ~(callback->disconnectedBitmap);
        }
        break;

    case WM_APIID_INDICATION:
        {
            WMindCallback *cb = (WMindCallback *)arg;
            switch (cb->state)
            {
            case WM_STATECODE_BEACON_RECV:     // Beacon��Mindicate
                p_mbi_param->callback(MB_CALLBACK_BEACON_RECV, arg);
                break;
            case WM_STATECODE_DISASSOCIATE:    // �ڑ��ؒfindicate
                p_mbi_param->callback(MB_CALLBACK_DISASSOCIATE, arg);
                break;
            case WM_STATECODE_REASSOCIATE:     // �Đڑ�indicate
                p_mbi_param->callback(MB_CALLBACK_REASSOCIATE, arg);
                break;
            case WM_STATECODE_AUTHENTICATE:    // �F�؊m�Findicate
                p_mbi_param->callback(MB_CALLBACK_AUTHENTICATE, arg);
                break;

            case WM_STATECODE_FIFO_ERROR:
                OS_TPanic("FIFO Error\n");
                break;
            case WM_STATECODE_INFORMATION:
                // �������Ȃ�
                break;
            }
        }
        break;

    default:
        p_mbi_param->callback(MB_CALLBACK_ERROR, arg);
        break;
    }
}

/*
 * �q�@�R�[���o�b�N
 */
static void MBi_ChildPortCallback(void *arg)
{
    WMPortRecvCallback *cb = (WMPortRecvCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_PORT_RECV:
        // �f�[�^��M��ʒm
        p_mbi_param->callback(MB_CALLBACK_MP_CHILD_RECV, (void *)(arg));
        break;
    case WM_STATECODE_CONNECTED:
        // �ڑ��ʒm
        break;
    case WM_STATECODE_PORT_INIT:
    case WM_STATECODE_DISCONNECTED:
    case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
        break;
    }
}


static void MBi_ChildCallback(void *arg)
{
    WMCallback *buf = (WMCallback *)arg;
    MBiParam *const p_param = p_mbi_param;
    int     errcode;

    switch (buf->apiid)
    {
        //---------------------------------------------------------------
    case WM_APIID_INITIALIZE:
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }

            p_param->callback(MB_CALLBACK_INIT_COMPLETE, arg);

            // API�̔��s
            errcode =
                WM_SetLifeTime(MBi_ChildCallback, mbi_life_table_no, mbi_life_cam, mbi_life_frame,
                               mbi_life_mp);
            MBi_CheckWmErrcode(WM_APIID_SET_LIFETIME, errcode);
        }
        break;

    case WM_APIID_SET_LIFETIME:
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }

            // WM_StartScan()���s
            scanParam.scanBuf = &p_param->parentInfoBuf;
            if (scanParam.channel == 0)
                scanParam.channel = 1;
            if (scanParam.maxChannelTime == 0)
            {
                scanParam.maxChannelTime = 200;
            }
            scanParam.bssid[0] = 0xff;
            scanParam.bssid[1] = 0xff;
            scanParam.bssid[2] = 0xff;
            scanParam.bssid[3] = 0xff;
            scanParam.bssid[4] = 0xff;
            scanParam.bssid[5] = 0xff;
            p_param->scanning_flag = TRUE;
            p_param->scan_channel_flag = TRUE;
            errcode = WM_StartScan(MBi_ChildCallback, &scanParam);
            MBi_CheckWmErrcode(WM_APIID_START_SCAN, errcode);
        }
        break;

    case WM_APIID_START_SCAN:
        {
            WMstartScanCallback *callback = (WMstartScanCallback *)arg;

            if (callback->errcode != WM_ERRCODE_SUCCESS)
            {
                p_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }

            switch (callback->state)
            {
            case WM_STATECODE_SCAN_START:
                break;

            case WM_STATECODE_PARENT_FOUND:
                // Scan�Ŏ擾�����e�@���̊i�[
                {
                    WMstartScanCallback *buf = (WMstartScanCallback *)arg;
                    ParentInfo *p = &p_param->parent_info[0];

                    int     i;
                    for (i = 0; i < p_param->found_parent_count; ++i)
                    {

                        if (WM_IsBssidEqual(buf->macAddress, p[i].scan_data.macAddress))
                        {
                            p[i].scan_data.gameInfoLength = buf->gameInfoLength;
                            p[i].scan_data.gameInfo = buf->gameInfo;
                            DC_InvalidateRange(p_param->parent_info[i].parentInfo,
                                               WM_BSS_DESC_SIZE);
                            MI_DmaCopy16(WM_DMA_NO, &p_param->parentInfoBuf,
                                         p_param->parent_info[i].parentInfo, WM_BSS_DESC_SIZE);
                            p_param->last_found_parent_no = i;
                            goto scan_end;
                        }
                    }

                    if (i < MB_NUM_PARENT_INFORMATIONS)
                    {
                        p_param->found_parent_count = (u16)(i + 1);
                        // �V�K�ɔ��������e�@�Ȃ̂ŁA�f�[�^���i�[����
                        MI_CpuCopy16(buf, &p[i].scan_data, sizeof(p[i].scan_data));
                        DC_InvalidateRange(p_param->parent_info[i].parentInfo, WM_BSS_DESC_SIZE);
                        MI_DmaCopy16(WM_DMA_NO, &p_param->parentInfoBuf,
                                     p_param->parent_info[i].parentInfo, WM_BSS_DESC_SIZE);
                        p_param->last_found_parent_no = i;
                    }
                }
              scan_end:
                p_param->callback(MB_CALLBACK_PARENT_FOUND, arg);
                if (!p_param->scanning_flag)
                {
                    return;
                }

                if (p_param->scan_channel_flag)
                {
                    if (FALSE == changeScanChannel(&scanParam))
                    {
                        (void)MBi_CommEnd();
                    }
                }
                errcode = WM_StartScan(MBi_ChildCallback, &scanParam);
                MBi_CheckWmErrcode(WM_APIID_START_SCAN, errcode);
                break;

            case WM_STATECODE_PARENT_NOT_FOUND:
                p_param->callback(MB_CALLBACK_PARENT_NOT_FOUND, arg);   // �����̃R�[���o�b�N��ParentInfoLifeTimeCount��
                if (!p_param->scanning_flag)
                {
                    return;
                }

                if (p_param->scan_channel_flag)
                {
                    if (FALSE == changeScanChannel(&scanParam))
                    {
                        (void)MBi_CommEnd();
                    }
                }
                errcode = WM_StartScan(MBi_ChildCallback, &scanParam);
                MBi_CheckWmErrcode(WM_APIID_START_SCAN, errcode);
                break;

            default:
                p_param->callback(MB_CALLBACK_ERROR, arg);
                break;
            }
        }
        break;

    case WM_APIID_END_SCAN:
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }

            errcode = WM_StartConnect(MBi_ChildCallback, p_param->pInfo, NULL);
            MBi_CheckWmErrcode(WM_APIID_START_CONNECT, errcode);
        }
        break;

    case WM_APIID_START_CONNECT:
        {
            WMstartConnectCallback *callback = (WMstartConnectCallback *)arg;

            if (callback->errcode != WM_ERRCODE_SUCCESS)
            {
                /* �e�̐������Z�b�g */
                p_param->found_parent_count = 0;
                // �R�[���o�b�N�ŃG���[�ʒm
                p_param->callback(MB_CALLBACK_CONNECT_FAILED, arg);
                return;
            }

            switch (callback->state)
            {
            case WM_STATECODE_CONNECT_START:
                p_param->child_bitmap = 0;
                p_param->mpStarted = 1;
                break;

            case WM_STATECODE_CONNECTED:
                p_param->my_aid = (u16)callback->aid;
                p_param->callback(MB_CALLBACK_CONNECTED_TO_PARENT, arg);
                p_param->child_bitmap = 1;

                errcode = WM_SetPortCallback(WM_PORT_BT, MBi_ChildPortCallback, NULL);

                if (errcode != WM_ERRCODE_SUCCESS)
                {
                    break;
                }

                errcode = WM_StartMPEx(MBi_ChildCallback, (u16 *)p_param->recvBuf, p_param->recvBufSize, (u16 *)p_param->sendBuf, p_param->sendBufSize, (u16)(p_param->contSend ? 0 : 1), 0,    /* defaultRetryCount */
                                       FALSE, FALSE, 1, /* fixFreqMode */
                                       TRUE     /* ignoreFatalError */
                    );
                MBi_CheckWmErrcode(WM_APIID_START_MP, errcode);
                break;

            case WM_STATECODE_DISCONNECTED:
                p_param->callback(MB_CALLBACK_DISCONNECTED_FROM_PARENT, arg);
                p_param->child_bitmap = 0;
                p_param->mpStarted = 0;
                break;

            case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
                // ����ؒf�����ꍇ�͏������Ȃ�
                break;

            default:
                p_param->callback(MB_CALLBACK_ERROR, arg);
                break;
            }
        }
        break;

    case WM_APIID_START_MP:
        {
            WMstartMPCallback *callback = (WMstartMPCallback *)arg;

            switch (callback->state)
            {
            case WM_STATECODE_MP_START:
                p_param->mpStarted = 1; // MP�J�n�ς݃t���O���Z�b�g
                {
                    // MP���M���R�[���o�b�N
                    if (MBi_IsSendEnabled())
                    {
                        p_param->callback(MB_CALLBACK_MP_SEND_ENABLE, NULL);
                    }
                }
                break;

            case WM_STATECODE_MP_IND:
                if (callback->errcode == WM_ERRCODE_INVALID_POLLBITMAP)
                {
//                  p_param->callback( MB_CALLBACK_MP_CHILD_RECV, (void*)(callback->recvBuf) );
                }
                else
                {
//                    p_param->callback( MB_CALLBACK_MP_CHILD_RECV, (void*)(callback->recvBuf) );
                }
                break;

            case WM_STATECODE_MPACK_IND:
                {
                    //p_param->callback( MB_CALLBACK_MPACK_IND, NULL );
                }
                break;

            default:
                p_param->callback(MB_CALLBACK_ERROR, arg);
                break;
            }
        }
        break;

    case WM_APIID_SET_MP_DATA:
        {
            p_param->mpBusy = 0;
            if (buf->errcode == WM_ERRCODE_SUCCESS)
            {
                p_param->callback(MB_CALLBACK_MP_CHILD_SENT, arg);
            }
            else if (buf->errcode == WM_ERRCODE_TIMEOUT)
            {
                p_param->callback(MB_CALLBACK_MP_CHILD_SENT_TIMEOUT, arg);
            }
            else
            {
                p_param->callback(MB_CALLBACK_MP_CHILD_SENT_ERR, arg);
            }
            // ���̑��M������
            if (p_mbi_param->endReq == 0)
            {
                p_param->callback(MB_CALLBACK_MP_SEND_ENABLE, NULL);
            }
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_RESET:
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_param->endReq = 0;
                p_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }
            p_mbi_param->child_bitmap = 0;
            p_mbi_param->mpStarted = 0;

            errcode = WM_End(MBi_ChildCallback);
            MBi_CheckWmErrcode(WM_APIID_END, errcode);
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_END:
        {
            if (buf->errcode != WM_ERRCODE_SUCCESS)
            {
                p_param->endReq = 0;
                p_param->callback(MB_CALLBACK_ERROR, arg);
                return;
            }
            MBi_EndCommon(arg);
        }
        break;

        //---------------------------------------------------------------
    case WM_APIID_START_KS:
        {
            // MP���M���R�[���o�b�N
            if (MBi_IsSendEnabled())
            {
                p_param->callback(MB_CALLBACK_MP_SEND_ENABLE, NULL);
            }
        }
        break;

        //---------------------------------------------------------------        
    case WM_APIID_INDICATION:
        {
            WMindCallback *cb = (WMindCallback *)arg;
            switch (cb->state)
            {
            case WM_STATECODE_FIFO_ERROR:
                OS_TPanic("FIFO Error\n");
                break;
            case WM_STATECODE_INFORMATION:
                // �������Ȃ�
                break;
            }
        }
        break;
    default:
        p_param->callback(MB_CALLBACK_ERROR, arg);
        break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBi_GetBeaconPeriodDispersion

  Description:  �@���MAC�A�h���X���ƂɓK�x�ɂ΂�����r�[�R���Ԋu�֕t������
                �l���擾����B

  Arguments:    None.

  Returns:      u32 -   �@�킲�ƂɓK�x�ɂ΂���� 0 �` 19 �̒l��Ԃ��B
 *---------------------------------------------------------------------------*/
static u32 MBi_GetBeaconPeriodDispersion(void)
{
    u8      mac[6];
    u32     ret;
    s32     i;

    OS_GetMacAddress(mac);
    for (i = 0, ret = 0; i < 6; i++)
    {
        ret += mac[i];
    }
    ret += OS_GetVBlankCount();
    ret *= 7;
    return (ret % 20);
}


/******************************************************************************/
/* API */

/* ������ */
int MB_Init(void *work, const MBUserInfo *user, u32 ggid, u32 tgid, u32 dma)
{

    SDK_ASSERT(user != NULL);
    SDK_ASSERT(work != NULL);
    SDK_ASSERT((dma < 4));
    SDK_ASSERT(user->nameLength <= MB_USER_NAME_LENGTH);

    if (mbc && mbc->isMbInitialized)
    {
        return MB_ERRCODE_INVALID_STATE;
    }
    else
    {
        MBiParam *const p_parm = (MBiParam *) ((((u32)work) + 0x1F) & ~0x1F);
        MB_CommCommonWork *const p_com =
            (MB_CommCommonWork *) ((((u32)p_parm) + sizeof(*p_parm) + 0x1F) & ~0x1F);
        OSIntrMode enabled;

        /*
         * TGID �����w��̏ꍇ�͎����擾����.
         * (������ RTC ���g�p����̂Ŋ��荞�݋֎~�̑O�Ɏ擾)
         */
        if (tgid == MB_TGID_AUTO)
        {
            tgid = WM_GetNextTgid();
        }

        enabled = OS_DisableInterrupts();

        /* ���C�t�^�C���Əȓd�̓��[�h�ݒ�l�̏����� */
        mbi_life_table_no = TABLE_NO;
        mbi_life_frame = FRAME_LIFE_TIME;
        mbi_life_cam = CAM_LIFE_TIME;
        mbi_life_mp = MP_LIFE_TIME;
        mbi_power_save_mode = TRUE;

        /* DMA �`�����l��, ���[�N�̈�̏����� */
        WM_DMA_NO = (u16)dma;
        p_mbi_param = p_parm;
        mbc = p_com;
        MI_CpuClear32(p_parm, sizeof(*p_parm));
        MI_CpuClear16(p_com, sizeof(*p_com));

        {                              /* ���[�U��, �Q�[������ۑ� */
            int     i;
            static const u16 *game = L"multiboot";
            u16    *c;
            c = (u16 *)p_parm->uname;
            for (i = 0; i < user->nameLength; ++i)
            {
                *c++ = user->name[i];
            }
            c = (u16 *)p_parm->gname;
            for (i = 0; i < WM_SIZE_GAMENAME; ++i)
            {
                if (*game == 0)
                {
                    break;
                }
                *c++ = *game++;
            }
            MI_CpuCopy8(user, &p_com->user, sizeof(MBUserInfo));
            if (user->nameLength < MB_USER_NAME_LENGTH)
            {
                p_com->user.name[user->nameLength] = 0;
            }
        }

        {
            p_parm->p_sendlen = MB_COMM_P_SENDLEN_DEFAULT;
            p_parm->p_recvlen = MB_COMM_P_RECVLEN_DEFAULT;

            /* �e�@���(�q�@��Scan���Ɏ擾������)���Z�b�g */
            p_parm->sendBufSize = 0;
            p_parm->recvBufSize = 0;

            /* �s�v? */
            p_parm->contSend = 1;

            p_parm->recvBuf = (WMmpRecvBuf *)p_com->recvbuf;

            {
                WMParentParam *const p_parent = &p_parm->parentParam;
                /*
                 * ����r�[�R���� entry/mb �Ƃ��ɖ���.
                 * MbBeacon��SetGameInfo��ON�ɂ���.
                 */
                p_parent->entryFlag = 0;
                p_parent->multiBootFlag = 0;
                p_parent->CS_Flag = 1;
                p_parent->KS_Flag = 0;
                /* ���@�ŗL��GGID TGID���Z�b�g */
                p_parent->ggid = ggid;
                p_parent->tgid = (u16)tgid;
                p_parent->beaconPeriod =
                    (u16)(MB_BEACON_PERIOD_DEFAULT + MBi_GetBeaconPeriodDispersion());
                p_parent->maxEntry = 15;
            }
        }

        p_parm->mpBusy = 0;
        p_parm->mbIsStarted = 0;
        p_com->isMbInitialized = 1;

        p_com->start_mp_busy = FALSE;

        (void)OS_RestoreInterrupts(enabled);
    }

    return MB_ERRCODE_SUCCESS;
}

#define MP_USEC_TIME_LIMIT  (5600)

static BOOL MBi_IsCommSizeValid(u16 sendSize, u16 recvSize, u16 entry_num)
{
    int     usec;

    SDK_ASSERT(entry_num > 0 && entry_num <= 15);

    /* sendSize���K��͈͊O�̏ꍇ�A�����ȃT�C�Y�ݒ�Ɣ��� */
    if (sendSize > MB_COMM_P_SENDLEN_MAX || sendSize < MB_COMM_P_SENDLEN_MIN)
    {
        OS_TWarning("MB Parent send buffer size is out of the range.[%3d - %3d Bytes]\n",
                    MB_COMM_P_SENDLEN_MIN, MB_COMM_P_SENDLEN_MAX);
        return FALSE;
    }

    /* recvSize���K��͈͊O�̏ꍇ�A�����ȃT�C�Y�ݒ�Ɣ��� */
    if (recvSize > MB_COMM_P_RECVLEN_MAX || recvSize < MB_COMM_P_RECVLEN_MIN)
    {
        OS_TWarning
            ("MB Parent receive buffer size per child is out of the range.[%3d - %3d Bytes]\n",
             MB_COMM_P_RECVLEN_MIN, MB_COMM_P_RECVLEN_MAX);
        return FALSE;
    }

    /* 1���MP�ʐM�ɂ����鎞�Ԃɑ΂���]�� */
    usec = 330 + 4 * (sendSize + 38) + entry_num * (112 + 4 * (recvSize + 32));

    /* ���v���Ԃ�5600us�𒴉߂����ꍇ�A�����ȃT�C�Y�ݒ�Ɣ��肳���B */
    if (usec >= MP_USEC_TIME_LIMIT)
    {
        OS_TWarning("These send receive sizes require lower than %4dusec\n"
                    "it exceeds %4d usec\n", MP_USEC_TIME_LIMIT, usec);
        return FALSE;
    }

    return TRUE;
}


BOOL MB_SetParentCommParam(u16 sendSize, u16 maxChildren)
{
    OSIntrMode enabled = OS_DisableInterrupts();

    /* WM���N�����Ă�����A�ύX�s�� */
    if (p_mbi_param->mbIsStarted)
    {
        OS_TWarning("MB has Already started\n");

        (void)OS_RestoreInterrupts(enabled);
        return FALSE;
    }

    /* ����M�f�[�^�T�C�Y�̐�������]�� */
    if (FALSE == MBi_IsCommSizeValid(sendSize, MB_COMM_P_RECVLEN_DEFAULT, maxChildren))
    {
        OS_TWarning("MP data sizes have not changed\n");
        // 
        (void)OS_RestoreInterrupts(enabled);
        return FALSE;
    }

    /* �ő�ڑ��l���̃Z�b�g */
    p_mbi_param->parentParam.maxEntry = maxChildren;

    /* ����M�f�[�^�T�C�Y�̃Z�b�g */
    p_mbi_param->p_sendlen = sendSize;
    p_mbi_param->p_recvlen = MB_COMM_P_RECVLEN_DEFAULT;

    (void)OS_RestoreInterrupts(enabled);

    return TRUE;
}


BOOL MB_SetParentCommSize(u16 sendSize)
{
    OSIntrMode enabled = OS_DisableInterrupts();
    BOOL    ret;

    /* �ő�ڑ��l���́A���݂̐ݒ�������p�� */
    ret = MB_SetParentCommParam(sendSize, p_mbi_param->parentParam.maxEntry);

    (void)OS_RestoreInterrupts(enabled);

    return ret;
}

u16 MB_GetTgid(void)
{
    return p_mbi_param->parentParam.tgid;
}

/* �������i�e�q���ʕ����j */
static int MBi_StartCommon(void)
{
    int     errcode;

    p_mbi_param->mpStarted = 0;
    p_mbi_param->child_bitmap = 0;
    p_mbi_param->endReq = 0;
    p_mbi_param->currentTgid = 0;
    MBi_SetMaxScanTime(MB_SCAN_TIME_NORMAL);

    if (!mbc->is_started_ex)
    {
        do
        {
            errcode = WM_Initialize(wmBuf, p_mbi_param->callback_ptr, WM_DMA_NO);
        }
        while (errcode == WM_ERRCODE_WM_DISABLE);
        if (errcode != WM_ERRCODE_OPERATING)
        {
            OS_TWarning("WM_Initialize failed!\n");
            return MB_ERRCODE_WM_FAILURE;
        }
        else
        {
            (void)WM_SetIndCallback(p_mbi_param->callback_ptr);
            p_mbi_param->mbIsStarted = 1;
            return MB_ERRCODE_SUCCESS;
        }
    }
    else
    {
        (void)WM_SetIndCallback(p_mbi_param->callback_ptr);
        p_mbi_param->mbIsStarted = 1;
        MBi_OnInitializeDone();
        return MB_ERRCODE_SUCCESS;
    }
}


/* �e�@�p�����[�^�ݒ� & �X�^�[�g ���ʏ��� */
static int MBi_StartParentCore(int channel)
{
    int     i, ret;
    MBCommPStateCallback cb_tmp;
    OSIntrMode enabled;

    enabled = OS_DisableInterrupts();

    p_mbi_param->parentParam.channel = (u16)channel;
    wmBuf = (u8 *)((((u32)mbc) + sizeof(MB_CommPWork) + 31) & ~31);

    /* ����Callback���Z�b�g����Ă����ꍇ�̂��߂ɁA�e���|�����ɑޔ� */
    cb_tmp = pPwork->parent_callback;

    /* �e�@�ŗL�̃��[�N�̈���N���A */
    MI_CpuClear16((void *)((u32)pPwork + sizeof(MB_CommCommonWork)),
                  sizeof(MB_CommPWork) - sizeof(MB_CommCommonWork));
    MB_CommSetParentStateCallback(cb_tmp);

    /* ����M�T�C�Y�ˑ��̃p�����[�^�Z�o */
    mbc->block_size_max = MB_COMM_CALC_BLOCK_SIZE(p_mbi_param->p_sendlen);

    MBi_SetChildMPMaxSize(p_mbi_param->p_recvlen);
    MBi_SetParentPieceBuffer(&pPwork->req_data_buf);

    for (i = 0; i < MB_MAX_CHILD; i++)
    {
        pPwork->p_comm_state[i] = MB_COMM_PSTATE_NONE;
        pPwork->fileid_of_child[i] = -1;        /* �q�@����̃��N�G�X�gFileID�������� */
    }
    pPwork->file_num = 0;

    MI_CpuClear16(&pPwork->fileinfo[0], sizeof(pPwork->fileinfo));
    MI_CpuClear8(&pPwork->req2child[0], sizeof(pPwork->req2child));

    p_mbi_param->mode = MB_MODE_PARENT;
    p_mbi_param->callback = MBi_CommParentCallback;
    p_mbi_param->callback_ptr = MBi_ParentCallback;

    /* �e�@�ő呗�M�T�C�Y(Byte) */
    p_mbi_param->parentParam.parentMaxSize = p_mbi_param->p_sendlen;
    p_mbi_param->sendBufSize = WM_CalcParentSendBufSize(p_mbi_param);
    /* �q�@�ő呗�M�T�C�Y(Byte) */
    p_mbi_param->parentParam.childMaxSize = p_mbi_param->p_recvlen;
    p_mbi_param->recvBufSize = WM_CalcParentRecvBufSize(p_mbi_param);

    OS_TPrintf("Parent sendSize : %4d\n", p_mbi_param->parentParam.parentMaxSize);
    OS_TPrintf("Parent recvSize : %4d\n", p_mbi_param->parentParam.childMaxSize);
    OS_TPrintf("Parent sendBufSize : %4d\n", p_mbi_param->sendBufSize);
    OS_TPrintf("Parent recvBufSize : %4d\n", p_mbi_param->recvBufSize);

    MB_InitSendGameInfoStatus();

    ret = MBi_StartCommon();

    (void)OS_RestoreInterrupts(enabled);

    pPwork->useWvrFlag = PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM7);

    return ret;
}

/* �e�@�p�����[�^�ݒ� & �X�^�[�g (WM ����������) */
int MB_StartParent(int channel)
{
    mbc->is_started_ex = FALSE;
    return MBi_StartParentCore(channel);
}

/* �e�@�p�����[�^�ݒ� & �X�^�[�g (WM �O���N����) */
int MB_StartParentFromIdle(int channel)
{
    mbc->is_started_ex = TRUE;
    return MBi_StartParentCore(channel);
}

/* �q�@�p�����[�^�ݒ� & �X�^�[�g */
int MB_StartChild(void)
{
    int     ret;
    MBCommCStateCallbackFunc cb_tmp;
    OSIntrMode enabled;

    enabled = OS_DisableInterrupts();

    mbc->is_started_ex = FALSE;

    wmBuf = (u8 *)((((u32)mbc) + sizeof(MB_CommCWork) + 31) & ~31);

    /* ����Callback���Z�b�g����Ă����ꍇ�̂��߂ɁA�e���|�����ɑޔ� */
    cb_tmp = pCwork->child_callback;

    /* �q�@�ŗL�̃��[�N�̈���N���A */
    MI_CpuClear16((void *)((u32)pCwork + sizeof(MB_CommCommonWork)),
                  sizeof(MB_CommCWork) - sizeof(MB_CommCommonWork));
    MB_CommSetChildStateCallback(cb_tmp);

    pCwork->c_comm_state = MB_COMM_CSTATE_NONE;

    p_mbi_param->mode = MB_MODE_CHILD;
    p_mbi_param->callback = MBi_CommChildCallback;
    p_mbi_param->callback_ptr = MBi_ChildCallback;

    p_mbi_param->scanning_flag = FALSE;
    p_mbi_param->scan_channel_flag = TRUE;
    p_mbi_param->last_found_parent_no = -1;

    MBi_SetBeaconRecvStatusBufferDefault();
    MBi_SetScanLockFunc(MBi_ScanLock, MBi_ScanUnlock);
    MB_InitRecvGameInfoStatus();

    ret = MBi_StartCommon();

    (void)OS_RestoreInterrupts(enabled);

    return ret;
}


/* �^�X�N�X���b�h���I���������܂��͌��X�N�����Ă��Ȃ������ꍇ�̖����̏I������ */
static int MBi_CallReset(void)
{
    int     errcode;
    errcode = WM_Reset(p_mbi_param->callback_ptr);
    MBi_CheckWmErrcode(WM_APIID_RESET, errcode);
    return conv_errcode(errcode);
}

static void MBi_OnReset(MBiTaskInfo * p_task)
{
    (void)p_task;
    (void)MBi_CallReset();
}

/* �ʐM���I�� */
int MBi_CommEnd(void)
{
    int     ret = WM_ERRCODE_FAILED;
    OSIntrMode enabled = OS_DisableInterrupts();

    /* �܂�Start���Ăяo���Ă��Ȃ���΂��̏�ł������ɏI�� */
    if (!p_mbi_param->mbIsStarted)
    {
        MBi_EndCommon(NULL);
    }
    else if (p_mbi_param->endReq == 0)
    {
        p_mbi_param->scanning_flag = FALSE;
        p_mbi_param->endReq = 1;
        /* �^�X�N�X���b�h�������Ă���΂�����~������Ƀ��Z�b�g */
        if (MBi_IsTaskAvailable())
        {
            MBi_EndTaskThread(MBi_OnReset);
            ret = WM_ERRCODE_SUCCESS;
        }
        /* �����łȂ���΂��̏�Ń��Z�b�g */
        else
        {
            ret = MBi_CallReset();
        }
    }

    (void)OS_RestoreInterrupts(enabled);

    return ret;
}

void MB_End(void)
{
    OSIntrMode enabled = OS_DisableInterrupts();

    if (mbc->is_started_ex)
    {
        OS_TPanic("MB_End called after MB_StartParentFromIdle! (please call MB_EndToIdle)");
    }

    (void)MBi_CommEnd();

    (void)OS_RestoreInterrupts(enabled);
}

void MB_EndToIdle(void)
{
    OSIntrMode enabled = OS_DisableInterrupts();

    if (!mbc->is_started_ex)
    {
        OS_TPanic("MB_EndToIdle called after MB_StartParent! (please call MB_End)");
    }

    (void)MBi_CommEnd();

    (void)OS_RestoreInterrupts(enabled);
}

//-------------------------------------
// �q�@��ؒf
//-------------------------------------
void MB_DisconnectChild(u16 aid)
{
    SDK_NULL_ASSERT(pPwork);
    SDK_ASSERT(p_mbi_param->endReq != 1);

    if (WM_Disconnect(MBi_ParentCallback, aid) != WM_ERRCODE_OPERATING)
    {
        OS_TWarning("MB_DisconnectChild failed disconnect child %d\n", aid);
    }

    if (aid == 0 || aid >= 16)
    {
        OS_TWarning("Disconnected Illegal AID No. ---> %2d\n", aid);
        return;
    }

    /*  �q�@��������
       (�ؒf���N�������ꍇ�A����AID�Ɋւ���q�@���͏�������) */
    pPwork->childversion[aid - 1] = 0;
    MI_CpuClear8(&pPwork->childggid[aid - 1], sizeof(u32));
    MI_CpuClear8(&pPwork->childUser[aid - 1], sizeof(MBUserInfo));

    /* ��M�o�b�t�@���N���A */
    MBi_ClearParentPieceBuffer(aid);

    pPwork->req2child[aid - 1] = MB_COMM_USER_REQ_NONE;

    /* �v��FileID���Z�b�g����Ă����ꍇ�BID��-1�ɃN���A����B */
    if (pPwork->fileid_of_child[aid - 1] != -1)
    {
        u8      fileID = (u8)pPwork->fileid_of_child[aid - 1];
        u16     nowChildFlag = pPwork->fileinfo[fileID].gameinfo_child_bmp;
        u16     child = aid;

        pPwork->fileinfo[fileID].gameinfo_child_bmp &= ~(MB_GAMEINFO_CHILD_FLAG(child));
        pPwork->fileinfo[fileID].gameinfo_changed_bmp |= MB_GAMEINFO_CHILD_FLAG(child);
        pPwork->fileid_of_child[child - 1] = -1;
        pPwork->fileinfo[fileID].pollbmp &= ~(0x0001 << child);

        MB_DEBUG_OUTPUT("Update Member (AID:%2d)\n", child);
    }

    /* �ڑ������N���A */
    if (pPwork->child_entry_bmp & (0x0001 << aid))
    {
        pPwork->child_num--;
        pPwork->child_entry_bmp &= ~(0x0001 << aid);
    }
    pPwork->p_comm_state[aid - 1] = MB_COMM_PSTATE_NONE;
}


/* mb_child.c �ł̂ݎg�p ****************************************************/

int MBi_GetLastFountParent(void)
{
    return p_mbi_param->last_found_parent_no;
}

WMBssDesc *MBi_GetParentBssDesc(int id)
{
    return p_mbi_param->parent_info[id].parentInfo;
}

static void MBi_ScanLock(u8 *macAddr)
{
    scanParam.maxChannelTime = MB_SCAN_TIME_LOCKING;
    p_mbi_param->scan_channel_flag = FALSE;
    WM_CopyBssid(macAddr, scanParam.bssid);
}

static void MBi_ScanUnlock(void)
{
    static const u8 bss_fill[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    scanParam.maxChannelTime = MB_SCAN_TIME_NORMAL;
    p_mbi_param->scan_channel_flag = TRUE;
    WM_CopyBssid(bss_fill, scanParam.bssid);
}

/* mb_comm �ł̂ݎg�p *********************************************************/

// �X�L�������Ԃ̐ݒ�
void MBi_SetMaxScanTime(u16 time)
{
    scanParam.maxChannelTime = time;
}

static int
MBi_SetMPData(WMCallbackFunc callback, const u16 *sendData, u16 sendDataSize, u16 tmptt,
              u16 pollbmp)
{
#pragma unused( tmptt )
    int     errcode;
    errcode =
        WM_SetMPDataToPort(callback, sendData, sendDataSize, pollbmp, WM_PORT_BT, WM_PRIORITY_LOW);
    MBi_CheckWmErrcode(WM_APIID_SET_MP_DATA, errcode);
    return errcode;
}


int MBi_SendMP(const void *buf, int len, int pollbmp)
{
    int     errcode;

    u16     size = (u16)len;
    u16     pollbitmap = (u16)pollbmp;

    // MP�J�n�O�y�сA�I���v�����s���MP�f�[�^�̑��M�͂ł��Ȃ�
    if ((p_mbi_param->mpStarted == 0) || (p_mbi_param->endReq == 1))
    {
        return WM_ERRCODE_FAILED;
    }

    switch (p_mbi_param->mode)
    {
    case MB_MODE_PARENT:
        errcode = MBi_SetMPData(p_mbi_param->callback_ptr, (u16 *)buf, size,    // ��MBi_ParentCallback���g����mb_child�ł�ParentCallback�������N����Ă��܂��̂ŁAcallback_ptr�ő�p�B
                                (u16)((!p_mbi_param->contSend) ? 1000 : 0), pollbitmap);

        if (errcode == WM_ERRCODE_OPERATING)
        {
            p_mbi_param->mpBusy = 1;
        }

        return conv_errcode(errcode);

    case MB_MODE_CHILD:
        errcode = MBi_SetMPData(MBi_ChildCallback, (u16 *)buf, size, 0, pollbitmap);

        if (errcode == WM_ERRCODE_OPERATING)
        {
            p_mbi_param->mpBusy = 1;
        }

        return conv_errcode(errcode);

    default:
        return WM_ERRCODE_FAILED;
    }
}

int MBi_GetSendBufSize(void)
{
    return (int)p_mbi_param->sendBufSize;
}

int MBi_GetRecvBufSize(void)
{
    return (int)p_mbi_param->recvBufSize;
}

int MBi_CommConnectToParent(const WMBssDesc *bssDescp)
{
    WMgameInfo *gameInfo;
    int     errcode;
    SDK_ASSERT(bssDescp != 0);

    gameInfo = (WMgameInfo *)(&(bssDescp->gameInfo));
    p_mbi_param->p_sendlen = gameInfo->parentMaxSize;
    p_mbi_param->p_recvlen = gameInfo->childMaxSize;
    /* ����M�T�C�Y�ˑ��̃p�����[�^�Z�o */
    mbc->block_size_max = MB_COMM_CALC_BLOCK_SIZE(p_mbi_param->p_sendlen);

    MBi_SetChildMPMaxSize(p_mbi_param->p_recvlen);

    OS_TPrintf("\trecv size : %d\n", p_mbi_param->p_sendlen);
    OS_TPrintf("\tsend size : %d\n", p_mbi_param->p_recvlen);
    OS_TPrintf("\tblock size: %d\n", mbc->block_size_max);

    p_mbi_param->recvBufSize = (u16)WM_CalcChildRecvBufSize(bssDescp);
    p_mbi_param->sendBufSize = (u16)WM_CalcChildSendBufSize(bssDescp);
    p_mbi_param->pInfo = bssDescp;
    p_mbi_param->currentTgid = ((WMGameInfo *)&(bssDescp->gameInfo))->tgid;
    p_mbi_param->scanning_flag = FALSE;

    errcode = WM_EndScan(p_mbi_param->callback_ptr);
    MBi_CheckWmErrcode(WM_APIID_END_SCAN, errcode);
    return conv_errcode(errcode);
}

u32 MBi_GetGgid(void)
{
    return p_mbi_param->parentParam.ggid;
}

u16 MBi_GetTgid(void)
{
    return (p_mbi_param->parentParam.tgid);
}

u8 MBi_GetAttribute(void)
{
    return ((u8)(((p_mbi_param->parentParam.entryFlag) ? WM_ATTR_FLAG_ENTRY : 0) |      // entryFlag �ŉ��ʃr�b�g
                 ((p_mbi_param->parentParam.multiBootFlag) ? WM_ATTR_FLAG_MB : 0) |     // multiBootFlag ��2�r�b�g
                 ((p_mbi_param->parentParam.KS_Flag) ? WM_ATTR_FLAG_KS : 0) |   // KS_Flag ��3�r�b�g
                 ((p_mbi_param->parentParam.CS_Flag) ? WM_ATTR_FLAG_CS : 0)     // CS_Flag ��4�r�b�g
            ));
}


// �X�L�����̍ĊJ
int MBi_RestartScan(void)
{
    int     errcode;

    p_mbi_param->scanning_flag = TRUE;

    if (p_mbi_param->scan_channel_flag)
    {
        if (FALSE == changeScanChannel(&scanParam))
        {
            (void)MBi_CommEnd();
        }
    }

    errcode = WM_StartScan(MBi_ChildCallback, &scanParam);
    MBi_CheckWmErrcode(WM_APIID_START_SCAN, errcode);
    return conv_errcode(errcode);
}


// �X�L�����`�����l���̎擾
int MBi_GetScanChannel(void)
{
    return scanParam.channel;
}

// ���g��AID�擾
u16 MBi_GetAid(void)
{
    return p_mbi_param->my_aid;
}

BOOL MBi_IsStarted(void)
{
    return (p_mbi_param->mbIsStarted == 1) ? TRUE : FALSE;
}

// WM_API�̖߂�l�`�F�b�N
static void MBi_CheckWmErrcode(u16 apiid, int errcode)
{
    u16     arg[2];

    if (errcode != WM_ERRCODE_OPERATING && errcode != WM_ERRCODE_SUCCESS)
    {
        arg[0] = apiid;
        arg[1] = (u16)errcode;
        p_mbi_param->callback(MB_CALLBACK_API_ERROR, arg);
    }
}

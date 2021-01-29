/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - include
  File:     wm_api.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm_api.h,v $
  Revision 1.76  2007/11/09 13:08:09  seiki_masashi
  �d�g�̎�M�̂݉\�ȏ�Ԃɖ���������������֐���ǉ�

  Revision 1.75  2007/02/20 00:28:08  kitase_hirotake
  indent source

  Revision 1.74  2006/07/06 11:45:31  okubata_ryoma
  undo

  Revision 1.72  2006/02/20 07:42:43  seiki_masashi
  WM_SetGameInfo �֐��̈�������ύX

  Revision 1.71  2006/02/20 06:48:19  seiki_masashi
  WM_IsValidGameInfo �֐��� WM_IsValidGameBeacon �֐��̒ǉ�

  Revision 1.70  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.69  2005/11/25 00:46:09  seiki_masashi
  mask �� WMMPParam �\���̓��Ɉړ�

  Revision 1.68  2005/11/24 03:02:42  seiki_masashi
  WM_SetMPParameter �֐��Ɗ֘A�����`��ǉ�

  Revision 1.67  2005/11/02 01:32:23  ooe
  WM_StartTestRxMode, WM_StopTestRxMode��ǉ��B

  Revision 1.66  2005/11/01 08:59:44  seiki_masashi
  WM_IsMP, WM_GetMPReadyAIDs �̊e�֐������J�֐��ɕύX

  Revision 1.65  2005/10/28 11:18:55  seiki_masashi
  WMStatus �\���̂ւ̃A�N�Z�T�̎���

  Revision 1.64  2005/07/27 06:52:42  ooe
  WM_SetPowerSaveMode��ǉ��B

  Revision 1.63  2005/06/07 05:44:48  seiki_masashi
  Key Sharing �Ɋւ�����ʏ�����ጸ���邽�߂̕ύX

  Revision 1.62  2005/04/11 08:51:18  yosizaki
  add WM_GetNextTgid().

  Revision 1.61  2005/04/11 01:16:32  seiki_masashi
  WMi_StartParentEx �֐���ǉ�

  Revision 1.60  2005/03/17 05:09:13  ooe
  WM_GetOtherElements��ǉ��B

  Revision 1.59  2005/03/03 14:44:57  seiki_masashi
  WM_DISABLE_KEYSHARING �X�C�b�`�̒ǉ�

  Revision 1.58  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.57  2005/02/14 11:48:18  terui
  WM_StartScan[Ex]�Ɋւ���R�����gFIX�B

  Revision 1.56  2005/02/14 06:35:08  terui
  WMi_CheckMpPacketTimeRequired�֐���ǉ��B

  Revision 1.55  2005/02/08 06:43:42  ooe
  WM_StartScanEx�̈����̌^��ύX(WMScanExParam)

  Revision 1.54  2005/02/03 11:42:51  ooe
  WM_SetWEPKeyEx��ǉ��B

  Revision 1.53  2005/02/02 07:46:20  takano_makoto
  WM_CopyBssid16, WM_IsBssidEqual16��ǉ�

  Revision 1.52  2005/01/27 12:03:27  takano_makoto
  typo�C��

  Revision 1.51  2005/01/27 09:11:49  takano_makoto
  WM_CopyBssId, WM_IsBssidEqual��u8�P�ʂŏ�������悤�ɏC���B

  Revision 1.50  2005/01/27 08:06:21  takano_makoto
  ASSERT��ǉ�

  Revision 1.49  2005/01/17 01:04:09  ooe
  WM_StartConnectEx��ǉ����AWM_StartConnect���C�����C�����B
  DCF���M�o�b�t�@�T�C�Y�v�Z�p�ɁAWM_SIZE_DCF_CHILD_SEND_BUFFER��ǉ��B

  Revision 1.48  2005/01/11 07:54:48  takano_makoto
  fix copyright header.

  Revision 1.47  2005/01/06 02:05:10  seiki_masashi
  WM_StartMP, WM_SetMPData, WM_SetMPDataToPort �� inline ��

  Revision 1.46  2004/12/22 02:29:53  terui
  WM_IsExistAllowedChannel��ǉ��B

  Revision 1.45  2004/11/17 07:31:04  terui
  Only fix comment.

  Revision 1.44  2004/11/10 05:41:49  takano_makoto
  WM_CopyBssid, WM_IsBssidEqual�̈�����const�|�C���^�ɏC��

  Revision 1.43  2004/11/10 00:59:12  takano_makoto
  WM_IsBssidEqual, WM_CopyBssid��ǉ�

  Revision 1.42  2004/11/09 00:03:59  seiki_masashi
  Null �����ɂ��^�C���A�E�g������

  Revision 1.41  2004/11/02 07:27:38  terui
  �R�����g�C���B

  Revision 1.40  2004/10/29 02:53:37  terui
  WM_StartScanEx���ꎞ�I�ɔ���J�֐��ɕύX�B

  Revision 1.39  2004/10/26 08:22:09  ooe
  WM_StartScanEx��ǉ�

  Revision 1.38  2004/10/22 04:36:47  terui
  WMErrCode�Ƃ���int�^��Ԃ��Ă����֐��ɂ��āAWMErrCode�񋓌^��Ԃ��悤�ɕύX�B

  Revision 1.37  2004/10/18 11:08:54  terui
  WM_SetBeaconPeriod�֐�����U����J�ɕύX�B

  Revision 1.36  2004/10/18 04:21:00  terui
  Fix comment only.

  Revision 1.35  2004/10/18 04:17:46  terui
  �r�[�R���Ԋu�A�X�L�����Ԋu���΂������d�l��ǉ��B

  Revision 1.34  2004/10/15 14:09:16  terui
  Fix comment.

  Revision 1.33  2004/10/05 09:45:28  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.32  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.31  2004/10/03 07:51:10  ooe
  ipl_branch�ƃ}�[�W�B ignoreFatalError�t���O�ɑΉ��B

  Revision 1.30  2004/10/01 04:09:26  terui
  WM_ENABLE_TESTMODE�X�C�b�`�ɂ��e�X�g���[�h����ɂ��Ă̋@�\�𐧌��B

  Revision 1.29  2004/09/20 10:30:21  ooe
  WM_SetBeaconPeriod��ǉ��B

  Revision 1.28  2004/09/16 02:59:28  seiki_masashi
  WM_GetMPReceiveBufferSize �̕s����C��

  Revision 1.27  2004/09/13 04:09:12  seiki_masashi
  WM_SetMPFrequency �̒ǉ�

  Revision 1.26  2004/09/11 04:09:38  terui
  Only fix comment.

  Revision 1.25  2004/09/10 12:04:37  seiki_masashi
  �L�[�V�F�A�����O�̑��M�o�C�g�������C�u�������ŉB������悤�ɕύX

  Revision 1.24  2004/09/10 04:56:16  seiki_masashi
  WM_SIZE_MP_*_BUFFER �𐳊m�Ȓl�ɕύX

  Revision 1.23  2004/09/09 07:24:11  seiki_masashi
  WM_SetMPDataToPortEx �̒ǉ�

  Revision 1.22  2004/09/09 01:58:20  terui
  �������E�I�������̍ו����ɔ����A�v���g�^�C�v�錾��ǉ��B

  Revision 1.21  2004/09/02 09:26:49  seiki_masashi
  fix typo.

  Revision 1.20  2004/09/02 09:15:33  terui
  WMi_GetStatusAddress�֐���ǉ��B

  Revision 1.19  2004/08/30 00:53:09  terui
  Move definition about LinkLevel to wm.h

  Revision 1.18  2004/08/27 06:44:20  seiki_masashi
  WM_StartMPEx �̈����� defaultRetryCount, fixFreqMode ��ǉ�

  Revision 1.17  2004/08/27 02:16:00  terui
  Only fix comment.

  Revision 1.16  2004/08/25 08:20:38  seiki_masashi
  WM_StatMPEx �̒ǉ�

  Revision 1.15  2004/08/25 07:59:06  terui
  Add WM_ConvGgid*.

  Revision 1.14  2004/08/24 13:20:04  terui
  �����N���x�擾�֐��y�ы��x�������񋓌^��ǉ��B

  Revision 1.13  2004/08/23 04:39:10  ooe
  WM_StartConnect()�̎d�l�ύX�B������ssid��ǉ��B

  Revision 1.12  2004/08/20 11:54:21  terui
  Childs -> Children

  Revision 1.11  2004/08/20 06:47:39  terui
  WM_DisconnectChilds��ǉ��B

  Revision 1.10  2004/08/19 15:25:25  miya
  modified SetGameInfo function.

  Revision 1.9  2004/08/18 12:54:30  seiki_masashi
  WM_SIZE_MP_*_*_BUFFER �֖��̕ύX

  Revision 1.8  2004/08/18 06:47:28  terui
  WM_GetAllowedChannel�֐���ǉ��B
  WM_SetEntry�֐���ǉ��B

  Revision 1.7  2004/08/18 00:24:47  seiki_masashi
  WM_StartMP �̈����̔�����

  Revision 1.6  2004/08/17 12:49:45  seiki_masashi
  WM_StartMP �̈����ɑ��M�o�b�t�@���w�肷��悤�ɕύX

  Revision 1.5  2004/08/11 04:27:09  seiki_masashi
  StartDataSharing �̈����� doubleMode ��ǉ�

  Revision 1.4  2004/08/10 13:17:04  terui
  Fix comment.

  Revision 1.1  2004/08/10 09:10:17  terui
  Initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_WM_ARM9_WM_API_H_
#define NITRO_WM_ARM9_WM_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     WM_DEFAULT_BEACON_PERIOD    200     // ms
#define     WM_DEFAULT_SCAN_PERIOD      30      // ms
#define     WM_MAX_MP_PACKET_TIME       5600    // us


/*---------------------------------------------------------------------------*
    �֐���`
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         WM_Init

  Description:  WM���C�u�����̏������������s���B
                ARM9���̏������݂̂��s�������֐��B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                                �o�b�t�@�̃T�C�Y��WM_SYSTEM_BUF_SIZE�����K�v�B
                dmaNo       -   WM���g�p����DMA�ԍ��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Init(void *wmSysBuf, u16 dmaNo);

/*---------------------------------------------------------------------------*
  Name:         WM_Finish

  Description:  WM���C�u�����̏I���������s���B�����֐��B
                WM_Init�֐����ĂԑO�̏�Ԃɖ߂�B

  Arguments:    None.

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Finish(void);

/*---------------------------------------------------------------------------*
  Name:         WM_Enable

  Description:  �����n�[�h�E�F�A���g�p�\�ȏ�Ԃɂ���B
                ������Ԃ�READY��Ԃ���STOP��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Enable(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_EnableForListening

  Description:  �����n�[�h�E�F�A���g�p�\�ȏ�Ԃɂ���B
                ������Ԃ�READY��Ԃ���STOP��ԂɑJ�ڂ����B
                �d�g�𑗏o����I�y���[�V�����͂ł��Ȃ��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                blink       -   LED ��_�ł����邩

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EnableForListening(WMCallbackFunc callback, BOOL blink);

/*---------------------------------------------------------------------------*
  Name:         WMi_EnableEx

  Description:  �����n�[�h�E�F�A���g�p�\�ȏ�Ԃɂ���B
                ������Ԃ�READY��Ԃ���STOP��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                miscFlags   -   ���������̃t���O

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_EnableEx(WMCallbackFunc callback, u32 miscFlags);

/*---------------------------------------------------------------------------*
  Name:         WM_Disable

  Description:  �����n�[�h�E�F�A���g�p�֎~��Ԃɂ���B
                ������Ԃ�STOP��Ԃ���READY��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Disable(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_PowerOn

  Description:  �����n�[�h�E�F�A���N������B
                ������Ԃ�STOP��Ԃ���IDLE��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_PowerOn(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_PowerOff

  Description:  �����n�[�h�E�F�A���V���b�g�_�E������B
                ������Ԃ�IDLE��Ԃ���STOP��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_PowerOff(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_Initialize

  Description:  WM�������������s���B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                                �o�b�t�@�̃T�C�Y��WM_SYSTEM_BUF_SIZE�����K�v�B
                callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                dmaNo       -   WM���g�p����DMA�ԍ��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Initialize(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo);

/*---------------------------------------------------------------------------*
  Name:         WM_InitializeForListening

  Description:  WM�������������s���B
                �X�L�����ȊO�̒ʐM�͂ł��Ȃ��B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                                �o�b�t�@�̃T�C�Y��WM_SYSTEM_BUF_SIZE�����K�v�B
                callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                dmaNo       -   WM���g�p����DMA�ԍ��B
                blink       -   LED ��_�ł����邩�ǂ���

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_InitializeForListening(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo, BOOL blink);

/*---------------------------------------------------------------------------*
  Name:         WMi_InitializeEx

  Description:  WM�������������s���B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                                �o�b�t�@�̃T�C�Y��WM_SYSTEM_BUF_SIZE�����K�v�B
                callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                dmaNo       -   WM���g�p����DMA�ԍ��B
                miscFlags   -   ���������̃t���O

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_InitializeEx(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo, u32 miscFlags);

/*---------------------------------------------------------------------------*
  Name:         WM_SetIndCallback

  Description:  WM7����̏󋵒ʒm�ɑ΂��ČĂт������֐���ݒ肷��B
                ���̊֐����͓̂����֐��B

  Arguments:    callback    -   �����t�@�[���E�F�A����̏󋵒ʒm���ɌĂяo�����
                                �R�[���o�b�N�֐����w��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetIndCallback(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_SetPortCallback

  Description:  WM7����̒ʐM�t���[����M�ʒm�ɑ΂��ČĂт������֐���ݒ肷��B

  Arguments:    port        -   �|�[�g�ԍ��B
                callback    -   ��M�ʒm���ɌĂяo�����R�[���o�b�N�֐��B
                arg         -   �R�[���o�b�N�֐��� WMPortRecvCallback.arg �Ƃ���
                                �n���������B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetPortCallback(u16 port, WMCallbackFunc callback, void *arg);

/*---------------------------------------------------------------------------*
  Name:         WM_Reset

  Description:  �������C�u���������Z�b�g���A����������̏�Ԃɖ߂��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Reset(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_End

  Description:  �������C�u�������I������B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_End(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_ReadStatus

  Description:  �������C�u������Ԃ������\���̂��擾����B�����֐��B

  Arguments:    statusBuf   -   ��Ԃ��擾����ϐ��ւ̃|�C���^�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_ReadStatus(WMStatus *statusBuf);

/*---------------------------------------------------------------------------*
  Name:         WM_SetParentParameter

  Description:  �e�@����ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                pparaBuf    -   �e�@���������\���̂ւ̃|�C���^�B
                                pparaBuf�y��pparaBuf->userGameInfo�̎��̂͋����I��
                                �L���b�V���X�g�A�����_�ɒ��ӁB

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetParentParameter(WMCallbackFunc callback, const WMParentParam *pparaBuf);

/*---------------------------------------------------------------------------*
  Name:         WMi_StartParent

  Description:  �e�@�Ƃ��ĒʐM���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                powerSave   -   �ȓd�̓��[�h���g�p����ꍇ��TRUE�A���Ȃ��ꍇ��FALSE�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_StartParentEx(WMCallbackFunc callback, BOOL powerSave);

/*---------------------------------------------------------------------------*
  Name:         WM_StartParent

  Description:  �e�@�Ƃ��ĒʐM���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartParent(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_EndParent

  Description:  �e�@�Ƃ��Ă̒ʐM���~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndParent(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_StartScan

  Description:  �q�@�Ƃ��Đe�@�̃X�L�������J�n����B
                ��x�̌Ăяo���ň��̐e�@�����擾����B
                WM_EndScan���Ăяo�����ɘA�����ČĂяo���Ă��ǂ��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                param       -   �X�L�������������\���̂ւ̃|�C���^�B
                                �X�L�������ʂ̏���param->scanBuf��ARM7�����ڏ����o��
                                �̂ŁA�L���b�V�����C���ɍ��킹�Ă����K�v������B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartScan(WMCallbackFunc callback, const WMScanParam *param);

/*---------------------------------------------------------------------------*
  Name:         WM_StartScanEx

  Description:  �q�@�Ƃ��Đe�@�̃X�L�������J�n����B
                ��x�̌Ăяo���ŕ�����̐e�@�����擾����B
                WM_EndScan���Ăяo�����ɘA�����ČĂяo���Ă��ǂ��B

  Arguments:    callback - �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                param    - �X�L�������������\���̂ւ̃|�C���^�B
                           �X�L�������ʂ̏���param->scanBuf��ARM7�����ڏ����o��
                           �̂ŁA�L���b�V�����C���ɍ��킹�Ă����K�v������B

  Returns:      int      - WM_ERRCODE_*�^�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartScanEx(WMCallbackFunc callback, const WMScanExParam *param);

/*---------------------------------------------------------------------------*
  Name:         WM_EndScan

  Description:  �q�@�Ƃ��ẴX�L�����������~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndScan(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_StartConnectEx

  Description:  �q�@�Ƃ��Đe�@�ւ̐ڑ����J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                pInfo       -   �ڑ�����e�@�̏��B
                                WM_StartScan�ɂĎ擾�����\���̂��w�肷��B
                                ���̍\���̂̎��̂͋����I�ɃL���b�V���X�g�A
                                �����_�ɒ��ӁB
                ssid        -   �e�@�ɒʒm����q�@���(24Byte(WM_SIZE_CHILD_SSID)�Œ�T�C�Y)
                powerSave   -   �ȓd�̓��[�h���g�p����ꍇ��TRUE�A���Ȃ��ꍇ��FALSE�B
                authMode    -   Authentication�̃��[�h�I���B
                                  WM_AUTHMODE_OPEN_SYSTEM : OPEN SYSTEM���[�h
                                  WM_AUTHMODE_SHARED_KEY  : SHARED KEY ���[�h

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartConnectEx(WMCallbackFunc callback, const WMBssDesc *pInfo, const u8 *ssid,
                            BOOL powerSave, u16 authMode);

/*---------------------------------------------------------------------------*
  Name:         WM_StartConnect

  Description:  �q�@�Ƃ��Đe�@�ւ̐ڑ����J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                pInfo       -   �ڑ�����e�@�̏��BWM_StartScan�ɂĎ擾�����\���̂�
                                �w�肷��B���̍\���̂̎��̂͋����I�ɃL���b�V���X�g�A
                                �����_�ɒ��ӁB
                ssid        -   �e�@�ɒʒm����q�@���(24Byte(WM_SIZE_CHILD_SSID)�Œ�T�C�Y)

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
static inline WMErrCode
WM_StartConnect(WMCallbackFunc callback, const WMBssDesc *pInfo, const u8 *ssid)
{
    return WM_StartConnectEx(callback, pInfo, ssid, TRUE, WM_AUTHMODE_OPEN_SYSTEM);
}

/*---------------------------------------------------------------------------*
  Name:         WM_Disconnect

  Description:  �m������Ă���ڑ���ؒf����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                aid         -   �ؒf����ʐM�����AID�B
                                �e�@�̏ꍇ�AID��1�`15�̎q�@���ʂɐؒf�B
                                �q�@�̏ꍇ�AID��0�̐e�@�Ƃ̒ʐM���I���B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Disconnect(WMCallbackFunc callback, u16 aid);

/*---------------------------------------------------------------------------*
  Name:         WM_DisconnectChildren

  Description:  �ڑ��m������Ă���q�@�����ꂼ��ؒf����B�e�@��p�֐��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                aidBitmap   -   �ؒf����q�@��AID�r�b�g�t�B�[���h�B
                                �ŉ��ʃr�b�g�͖�������Abit 1�`15 ��AID 1�`15 �̎q�@
                                �����ꂼ�ꎦ���B
                                �ڑ�����Ă��Ȃ��q�@�������r�b�g�͖��������̂ŁA
                                �ڑ��󋵂Ɋւ�炸�S�q�@��ؒf����ꍇ��0xFFFF���w��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_DisconnectChildren(WMCallbackFunc callback, u16 aidBitmap);

/*---------------------------------------------------------------------------*
  Name:         WM_GetMPSendBufferSize

  Description:  MP �ʐM�̑��M�o�b�t�@�̃T�C�Y�����݂� WM �̏�Ԃ���v�Z����B

  Arguments:    None.

  Returns:      int     -   WM_StartMP �ɓn���K�v�����鑗�M�o�b�t�@�̃T�C�Y�B
 *---------------------------------------------------------------------------*/
int     WM_GetMPSendBufferSize(void);

/*---------------------------------------------------------------------------*
  Name:         WM_GetMPReceiveBufferSize

  Description:  MP �ʐM�̎�M�o�b�t�@�̃T�C�Y�����݂� WM �̏�Ԃ���v�Z����B

  Arguments:    None.

  Returns:      int     -   WM_StartMP �ɓn���K�v�������M�o�b�t�@�̃T�C�Y�B
 *---------------------------------------------------------------------------*/
int     WM_GetMPReceiveBufferSize(void);

/*---------------------------------------------------------------------------*
  Name:         WM_StartMPEx

  Description:  MP�ʐM���J�n����B�ڍׂȓ��샂�[�h���w��\�B�e�@�E�q�@���ʊ֐��B

  Arguments:    callback        -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                recvBuf         -   ��M�f�[�^�i�[�o�b�t�@�B
                                    ARM7�����ڃf�[�^�������o���̂ŁA�L���b�V���ɒ��ӁB
                recvBufSize     -   ��M�f�[�^�i�[�o�b�t�@�̃T�C�Y�B
                                    WM_GetReceiveBufferSize() �̕Ԃ�l�����傫���K�v������B
                sendBuf         -   ���M�f�[�^�i�[�o�b�t�@�B
                                    ARM7�����ڃf�[�^�������o���̂ŁA�L���b�V���ɒ��ӁB
                sendBufSize     -   ���M�f�[�^�i�[�o�b�t�@�̃T�C�Y�B
                                    WM_GetSendBufferSize() �̕Ԃ�l�����傫���K�v������B
                mpFreq          -   1�t���[���ɉ��� MP �ʐM�����邩�B
                                    0 �ŘA�����M���[�h�B�e�@�݈̂Ӗ������B
                defaultRetryCount - 0�`7�ԃ|�[�g�̒ʐM�̍ۂɁA���M�Ɏ��s�����ꍇ��
                                    ���g���C���s���W���̉񐔁B0 �Ń��g���C���s��Ȃ��B
                minPollBmpMode  -   MP �ʐM���� pollBitmap �𑗐M���� packet �̈���ɉ�����
                                    �Œ���̏W���ɗ}���铮�샂�[�h�B
                singlePacketMode -  1 ��� MP �ɂ��Ĉ�� packet ��������Ȃ��Ȃ�
                                    ����ȓ��샂�[�h�B
                fixFreqMode     -   ���g���C�ɂ�� MP �ʐM�̑������֎~�������ȓ��샂�[�h�B
                                    1�t���[������ MP �ʐM�̉񐔂����傤�� mpFreq ��ɌŒ�B

                ignoreFatalError -  FatalError��������AutoDisconnect���s�Ȃ�Ȃ��Ȃ�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartMPEx(WMCallbackFunc callback,
                       u16 *recvBuf,
                       u16 recvBufSize,
                       u16 *sendBuf,
                       u16 sendBufSize,
                       u16 mpFreq,
                       u16 defaultRetryCount,
                       BOOL minPollBmpMode,
                       BOOL singlePacketMode, BOOL fixFreqMode, BOOL ignoreFatalError);

/*---------------------------------------------------------------------------*
  Name:         WM_StartMP

  Description:  MP�ʐM���J�n����B�e�@�E�q�@���ʊ֐��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                recvBuf     -   ��M�f�[�^�i�[�o�b�t�@�B
                                ARM7�����ڃf�[�^�������o���̂ŁA�L���b�V���ɒ��ӁB
                recvBufSize -   ��M�f�[�^�i�[�o�b�t�@�̃T�C�Y�B
                                WM_GetReceiveBufferSize() �̕Ԃ�l�����傫���K�v������B
                sendBuf     -   ���M�f�[�^�i�[�o�b�t�@�B
                                ARM7�����ڃf�[�^�������o���̂ŁA�L���b�V���ɒ��ӁB
                sendBufSize -   ���M�f�[�^�i�[�o�b�t�@�̃T�C�Y�B
                                WM_GetSendBufferSize() �̕Ԃ�l�����傫���K�v������B
                mpFreq      -   1�t���[���ɉ��� MP �ʐM�����邩�B
                                0 �ŘA�����M���[�h�B�e�@�݈̂Ӗ������B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartMP(WMCallbackFunc callback,
                     u16 *recvBuf, u16 recvBufSize, u16 *sendBuf, u16 sendBufSize, u16 mpFreq);

/*---------------------------------------------------------------------------*
  Name:         WM_SetMPParameter

  Description:  MP �ʐM�p�̊e��p�����[�^��ݒ肷��

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                param       -   MP �ʐM�p�̃p�����[�^���i�[�����\���̂ւ̃|�C���^

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetMPParameter(WMCallbackFunc callback, const WMMPParam * param);

/*---------------------------------------------------------------------------*
  Name:         WM_SetMPChildSize

  Description:  1���MP�ʐM�ł̎q�@�̑��M�o�C�g����ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                childSize   -   �q�@�̑��M�o�C�g���B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetMPChildSize(WMCallbackFunc callback, u16 childSize);

/*---------------------------------------------------------------------------*
  Name:         WM_SetMPParentSize

  Description:  1���MP�ʐM�ł̐e�@�̑��M�o�C�g����ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                childSize   -   �e�@�̑��M�o�C�g���B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetMPParentSize(WMCallbackFunc callback, u16 parentSize);

/*---------------------------------------------------------------------------*
  Name:         WM_SetMPFrequency

  Description:  1�t���[���ɉ��� MP �ʐM���邩��؂�ւ���B�e�@�p�֐��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                mpFreq      -   1�t���[���ɉ��� MP �ʐM�����邩�B
                                0 �ŘA�����M���[�h�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetMPFrequency(WMCallbackFunc callback, u16 mpFreq);

/*---------------------------------------------------------------------------*
  Name:         WM_SetMPInterval

  Description:  1�t���[�����ɘA������ MP �ʐM���s���ꍇ�̊Ԋu��ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                parentInterval - 1�t���[�����ɘA�����čs���ꍇ�̐e�@�� MP �ʐM�Ԋu (ms)
                childInterval  - 1�t���[�����ɘA�����čs���ꍇ�̎q�@�� MP �ʐM����������Ԋu (ms)

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetMPInterval(WMCallbackFunc callback, u16 parentInterval, u16 childInterval);

/*---------------------------------------------------------------------------*
  Name:         WM_SetMPTiming

  Description:  �t���[�������ʐM���[�h���� MP �ʐM�̏������J�n����^�C�~���O��ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                parentVCount -  �t���[�������ʐM���̐e�@�����̊J�n V Count
                childVCount  -  �t���[�������ʐM���̎q�@�����̊J�n V Count

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetMPTiming(WMCallbackFunc callback, u16 parentVCount, u16 childVCount);

/*---------------------------------------------------------------------------*
  Name:         WM_SetMPDataToPortEx

  Description:  MP�ʐM�ɂđ��M����f�[�^��\�񂷂�B�e�@�E�q�@���ʊ֐��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                arg         -   callback �ɓn��������
                sendData    -   ���M�\�񂷂�f�[�^�ւ̃|�C���^�B
                                ���̃f�[�^�̎��̂͋����I�ɃL���b�V���X�g�A�����_�ɒ��ӁB
                sendDataSize -  ���M�\�񂷂�f�[�^�̃T�C�Y�B
                destBitmap  -   ���M��q�@������ aid �� bitmap ���w��B
                port        -   ���M����|�[�g�ԍ��B
                prio        -   �D��x (0:�ō��`3:�Œ�)

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetMPDataToPortEx(WMCallbackFunc callback,
                               void *arg,
                               const u16 *sendData,
                               u16 sendDataSize, u16 destBitmap, u16 port, u16 prio);

/*---------------------------------------------------------------------------*
  Name:         WM_SetMPDataToPort

  Description:  MP�ʐM�ɂđ��M����f�[�^��\�񂷂�B�e�@�E�q�@���ʊ֐��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                sendData    -   ���M�\�񂷂�f�[�^�ւ̃|�C���^�B
                                ���̃f�[�^�̎��̂͋����I�ɃL���b�V���X�g�A�����_�ɒ��ӁB
                sendDataSize -  ���M�\�񂷂�f�[�^�̃T�C�Y�B
                destBitmap  -   ���M��q�@������ aid �� bitmap ���w��B
                port        -   ���M����|�[�g�ԍ��B
                prio        -   �D��x (0:�ō��`3:�Œ�)

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
static inline WMErrCode
WM_SetMPDataToPort(WMCallbackFunc callback, const u16 *sendData, u16 sendDataSize, u16 destBitmap,
                   u16 port, u16 prio)
{
    return WM_SetMPDataToPortEx(callback, NULL, sendData, sendDataSize, destBitmap, port, prio);
}

/*---------------------------------------------------------------------------*
  Name:         WM_SetMPData

  Description:  MP�ʐM�ɂđ��M����f�[�^��\�񂷂�B�e�@�E�q�@���ʊ֐��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                sendData    -   ���M�\�񂷂�f�[�^�ւ̃|�C���^�B
                                ���̃f�[�^�̎��̂͋����I�ɃL���b�V���X�g�A�����_�ɒ��ӁB
                sendDataSize -  ���M�\�񂷂�f�[�^�̃T�C�Y�B
                tmptt       -   �A�����M���[�h�̏ꍇ�AMP�p�P�b�g���M���Ԃ�ݒ�B
                                (�e�@�̂ݗL���B) (2004/07/26 ���݁E����)
                pollbmp     -   �A�����M���[�h�̏ꍇ�A���M��q�@������Poll�}�b�v���w��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
static inline WMErrCode
WM_SetMPData(WMCallbackFunc callback, const u16 *sendData, u16 sendDataSize, u16 tmptt, u16 pollbmp)
{
#pragma unused( tmptt )
    return WM_SetMPDataToPortEx(callback, NULL, sendData, sendDataSize, pollbmp, WM_PORT_RAWDATA,
                                WM_PRIORITY_NORMAL);
}

/*---------------------------------------------------------------------------*
  Name:         WM_EndMP

  Description:  MP�ʐM���~����B�e�@�E�q�@���ʊ֐��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndMP(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_StartDCF

  Description:  �C���t���X�g���N�`�����[�h�ł̒ʐM���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                recvBuf     -   �f�[�^��M�o�b�t�@�ւ̃|�C���^�B
                                ARM7�����ڃf�[�^�������o���̂ŁA�L���b�V���ɒ��ӁB
                recvBufSize -   �f�[�^��M�o�b�t�@�̃T�C�Y�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartDCF(WMCallbackFunc callback, WMDcfRecvBuf *recvBuf, u16 recvBufSize);

/*---------------------------------------------------------------------------*
  Name:         WM_SetDCFData

  Description:  �C���t���X�g���N�`�����[�h�ʐM�ɂđ��M����f�[�^��\�񂷂�

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                destAdr     -   �ʐM�����MAC�A�h���X�������o�b�t�@�ւ̃|�C���^�B
                sendData    -   ���M�\�񂷂�f�[�^�ւ̃|�C���^�B
                                ���M�\�񂷂�f�[�^�̎��̂͋����I�ɃL���b�V���X�g�A
                                �����_�ɒ��ӁB
                sendDataSize -  ���M�\�񂷂�f�[�^�̃T�C�Y�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetDCFData(WMCallbackFunc callback,
                        const u8 *destAdr, const u16 *sendData, u16 sendDataSize);

/*---------------------------------------------------------------------------*
  Name:         WM_EndDCF

  Description:  �C���t���X�g���N�`�����[�h�ł̒ʐM���~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndDCF(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_SetWEPKey

  Description:  �Í��@�\�A�Í��L�[��ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                wepmode     -   0: �Í��@�\�Ȃ�
                                1: RC4(40bit)�Í����[�h
                                2: RC4(104bit)�Í����[�h
                                3: RC4(128bit)�Í����[�h
                wepkey      -   �Í��L�[�f�[�^( 80�o�C�g )�ւ̃|�C���^�B
                                �L�[�f�[�^��4�̃f�[�^�ō\������A���ꂼ��20�o�C�g�B
                                �e20�o�C�g�̂����A
                                 40�r�b�g���[�h�ł�  5 �o�C�g
                                104�r�b�g���[�h�ł� 13 �o�C�g
                                128�r�b�g���[�h�ł� 16 �o�C�g
                                �̃f�[�^���g�p�����B
                                �܂��A���̃f�[�^�̎��̂͋����I�ɃL���b�V���X�g�A�����B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetWEPKey(WMCallbackFunc callback, u16 wepmode, const u16 *wepkey);

/*---------------------------------------------------------------------------*
  Name:         WM_SetWEPKeyEx

  Description:  �Í��@�\�A�Í��L�[�A�Í��L�[ID��ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                wepmode     -   0: �Í��@�\�Ȃ�
                                1: RC4(40bit)�Í����[�h
                                2: RC4(104bit)�Í����[�h
                                3: RC4(128bit)�Í����[�h
                wepkeyid    -   4�w�肵��wepkey�̂ǂ���g�p���邩��I�����܂��B
                                0�`3�Ŏw�肵�܂��B
                wepkey      -   �Í��L�[�f�[�^( 80�o�C�g )�ւ̃|�C���^�B
                                �L�[�f�[�^��4�̃f�[�^�ō\������A���ꂼ��20�o�C�g�B
                                �e20�o�C�g�̂����A
                                 40�r�b�g���[�h�ł�  5 �o�C�g
                                104�r�b�g���[�h�ł� 13 �o�C�g
                                128�r�b�g���[�h�ł� 16 �o�C�g
                                �̃f�[�^���g�p�����B
                                �܂��A���̃f�[�^�̎��̂͋����I�ɃL���b�V���X�g�A�����B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetWEPKeyEx(WMCallbackFunc callback, u16 wepmode, u16 wepkeyid, const u8 *wepkey);

/*---------------------------------------------------------------------------*
  Name:         WM_ReadMPData

  Description:  ��M�����f�[�^�S�̂���w��q�@�̃f�[�^�����𒊏o����B�����֐��B

  Arguments:    header      -   ��M�f�[�^�S�̂������|�C���^�B
                aid         -   �f�[�^�𒊏o����q�@��AID�B

  Returns:      WMMpRecvData* - �Y���q�@����̎�M�f�[�^�ւ̃|�C���^��Ԃ��B
                                ���o�Ɏ��s�����ꍇ�ANULL��Ԃ��B
 *---------------------------------------------------------------------------*/
WMMpRecvData *WM_ReadMPData(const WMMpRecvHeader *header, u16 aid);

/*---------------------------------------------------------------------------*
  Name:         WM_StartDataSharing

  Description:  �f�[�^�V�F�A�����O�@�\��L���ɂ���B�����֐��B
                �@�\��L���ɂ�����MP�ʐM���s�����ƂŁAMP�ʐM�ɕt������
                �f�[�^�V�F�A�����O�ʐM���s����B

  Arguments:    dsInfo      -   WMDataSharingInfo �\���́B
                port        -   �g�p���� port �ԍ��B
                aidBitmap   -   �f�[�^�V�F�A���s������� AID�B
                dataLength  -   ���L�������̃f�[�^���B (�����o�C�g���w�肷�邱��)
                doubleMode  -   ���t���[�� WM_StepDataSharing ����ꍇ�� TRUE ���w��

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartDataSharing(WMDataSharingInfo *dsInfo,
                              u16 port, u16 aidBitmap, u16 dataLength, BOOL doubleMode);

/*---------------------------------------------------------------------------*
  Name:         WM_EndDataSharing

  Description:  �f�[�^�V�F�A�����O�@�\�𖳌��ɂ���B�����֐��B

  Arguments:    dsInfo      -   WMDataSharingInfo �\���́B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndDataSharing(WMDataSharingInfo *dsInfo);

/*---------------------------------------------------------------------------*
  Name:         WM_StepDataSharing

  Description:  �f�[�^�V�F�A�����O�̓�������i�߂�B�����֐��B

  Arguments:    dsInfo      -   WMDataSharingInfo �\���́B
                sendData    -   ���L���������M�f�[�^�B
                receiveData -   ��M�������L�f�[�^�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
                         *_SUCCESS      : ���L�f�[�^����M����
                         *_NO_DATASET   : �܂����L�f�[�^�����Ă��Ȃ�
 *---------------------------------------------------------------------------*/
WMErrCode WM_StepDataSharing(WMDataSharingInfo *dsInfo,
                             const u16 *sendData, WMDataSet *receiveData);

/*---------------------------------------------------------------------------*
  Name:         WM_GetSharedDataAddress

  Description:  �f�[�^�V�F�A�����O�̎�M�f�[�^���̓���� AID �̃A�h���X�𓾂�B
                �����֐��B

  Arguments:    dsInfo      -   WMDataSharingInfo �\���́B
                receiveData -   ��M�������L�f�[�^�B
                aid         -   AID�B

  Returns:      u16*        -   ��M�f�[�^�̃A�h���X�B���݂��Ȃ���� NULL ��Ԃ��B
 *---------------------------------------------------------------------------*/
u16    *WM_GetSharedDataAddress(WMDataSharingInfo *dsInfo, WMDataSet *receiveData, u16 aid);

/*---------------------------------------------------------------------------*
  Name:         WM_StartKeySharing

  Description:  �L�[�V�F�A�����O�@�\��L���ɂ���B
                �@�\��L���ɂ�����MP�ʐM���s�����ƂŁAMP�ʐM�ɕt������
                �L�[�V�F�A�����O�ʐM���s����B

  Arguments:    buf         -   �L�[�����i�[����o�b�t�@�ւ̃|�C���^�B
                                ���̂� WMDataSharingInfo �\���̂ւ̃|�C���^�B
                port        -   �g�p���� port �ԍ�

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartKeySharing(WMKeySetBuf *buf, u16 port);

/*---------------------------------------------------------------------------*
  Name:         WM_EndKeySharing

  Description:  �L�[�V�F�A�����O�@�\�𖳌��ɂ���B

  Arguments:    buf         -   �L�[�����i�[����o�b�t�@�ւ̃|�C���^�B
                                ���̂� WMDataSharingInfo �\���̂ւ̃|�C���^�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndKeySharing(WMKeySetBuf *buf);

/*---------------------------------------------------------------------------*
  Name:         WM_GetKeySet

  Description:  �L�[�V�F�A�����O���ꂽ�L�[�Z�b�g�f�[�^���ЂƂǂݏo���B

  Arguments:    buf         -   �L�[�����i�[����o�b�t�@�ւ̃|�C���^�B
                                ���̂� WMDataSharingInfo �\���̂ւ̃|�C���^�B
                keySet      -   �L�[�Z�b�g��ǂݏo���o�b�t�@�ւ̃|�C���^�B
                                WM_StartKeySharing�ɂė^�����o�b�t�@�Ƃ�
                                �ʂ̃o�b�t�@���w�肷��B

  Returns:      MWErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_GetKeySet(WMKeySetBuf *buf, WMKeySet *keySet);

/*---------------------------------------------------------------------------*
  Name:         WM_SetGameInfo

  Description:  �Q�[������ݒ肷��BWM_SetParentParameter�ɂ�菉���l�̓Z�b�g
                �����̂ŁA�����ύX����ꍇ�ɗp����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                userGameInfo     - ���[�U�[�Q�[�����ւ̃|�C���^�B
                userGameInfoSize - ���[�U�[�Q�[�����̃T�C�Y�B
                ggid        -   �Q�[���O���[�vID
                tgid        -   �e���|�����O���[�vID
                attr        -   �t���O�Q�B�ȉ��̃t���O�̘_���a��ݒ�B
                                    WM_ATTR_FLAG_ENTRY - �G���g���[����
                                    WM_ATTR_FLAG_MB    - �}���`�u�[�g�󂯕t��
                                    WM_ATTR_FLAG_KS    - �L�[�V�F�A�����O
                                    WM_ATTR_FLAG_CS    - �A���]�����[�h
  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetGameInfo(WMCallbackFunc callback,
                         const u16 *userGameInfo, u16 userGameInfoSize,
                         u32 ggid, u16 tgid, u8 attr);

/*---------------------------------------------------------------------------*
  Name:         WM_SetBeaconIndication

  Description:  �r�[�R������Mindicate�̗L��/������؂�ւ���B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                flag        -   0: ����
                                1: �L��

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetBeaconIndication(WMCallbackFunc callback, u16 flag);

#ifdef  WM_ENABLE_TESTMODE
/*---------------------------------------------------------------------------*
  Name:         WM_StartTestMode

  Description:  �e�X�g���[�h�ł̒ʐM���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                signal      -   0: ���ϒ�(data=0)
                                1: ���ϒ�(data=1)
                                2: PN15�i,
                                3: 01�p�^�[��(�X�N�����u������)
                                4: 01�p�^�[��(�X�N�����u���Ȃ�)
                rate        -   1: 1Mbps
                                2: 2Mbps
                channel     -   �f�[�^�𑗐M����`�����l�����w��(1�`14)�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartTestMode(WMCallbackFunc callback, u16 signal, u16 rate, u16 channel);

/*---------------------------------------------------------------------------*
  Name:         WM_StopTestMode

  Description:  �e�X�g���[�h�ł̒ʐM���~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StopTestMode(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_StartTestRxMode

  Description:  �e�X�g���[�h�ł̎�M���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                channel     -   �f�[�^����M����`�����l�����w��(1�`14)�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartTestRxMode(WMCallbackFunc callback, u16 channel);

/*---------------------------------------------------------------------------*
  Name:         WM_StopTestRxMode

  Description:  �e�X�g���[�h�ł̎�M���~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StopTestRxMode(WMCallbackFunc callback);

#endif

/*---------------------------------------------------------------------------*
  Name:         WM_SetLifeTime

  Description:  ���C�t�^�C����ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                tableNumber -   ���C�t�^�C����ݒ肷��CAM�e�[�u���ԍ��B
                                ( 0xFFFF�ł��ׂẴe�[�u�� )
                camLifeTime -   CAM�̃��C�t�^�C���B( 100ms�P�ʁF0xFFFF�Ŗ��� )
                frameLifeTime - �ݒ肷��t���[���̃��C�t�^�C���̃r�[�R���Ԋu�B
                                ( 100ms�P�ʁF0xFFFF�Ŗ��� )
                camLifeTime -   MP �ʐM�̃��C�t�^�C���B( 100ms�P�ʁF0xFFFF�Ŗ��� )

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetLifeTime(WMCallbackFunc callback,
                         u16 tableNumber, u16 camLifeTime, u16 frameLifeTime, u16 mpLifeTime);

/*---------------------------------------------------------------------------*
  Name:         WM_MeasureChannel

  Description:  �`�����l���̎g�p�󋵂𑪒肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                ccaMode     -   CCA���샂�[�h�B
                                0: �L�����A�Z���X�̂݁BED臒l�͖����B
                                1: ED臒l�̂ݗL���B
                                2: �L�����A�Z���X��ED臒l�̘_���ρB
                                3: �L�����A�Z���X��ED臒l�̘_���a�B
                EDThreshold -   ED臒l�B(0�`61) -60dBm �` -80dBm
                channel     -   ��������`�����l���B
                                ( 1���MeasureChannel��1�̃`�����l���̂� )
                measureTime -   �������鎞�ԁB

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_MeasureChannel(WMCallbackFunc callback,
                            u16 ccaMode, u16 edThreshold, u16 channel, u16 measureTime);

/*---------------------------------------------------------------------------*
  Name:         WM_InitWirelessCounter

  Description:  WirelessCounter������������B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_InitWirelessCounter(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_GetWirelessCounter

  Description:  Wireless NIC�̑���M�t���[�����A����M�G���[�t���[�������擾����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_GetWirelessCounter(WMCallbackFunc callback);

/*---------------------------------------------------------------------------*
  Name:         WM_GetAllowedChannel

  Description:  �ʐM���Ɏg�p�������ꂽ�`�����l�����擾����B�����֐��B

  Arguments:    None.

  Returns:      u16 -   �g�p���`�����l���̃r�b�g�t�B�[���h��Ԃ��B�ŉ��ʃr�b�g��
                        1�`�����l���A�ŏ�ʃr�b�g��16�`�����l���������B�r�b�g��1��
                        �`�����l�����g�p���A�r�b�g��0�̃`�����l���͎g�p�֎~�B
                        �ʏ�� 1�`13 �`�����l���̓��������̃r�b�g��1�ɂȂ����l��
                        �Ԃ����B0x0000���Ԃ��ꂽ�ꍇ�͎g�p�������ꂽ�`�����l����
                        ���݂��Ȃ����߁A�����@�\���̂��̂��g�p�֎~�ł���B
                        �܂��A�����������ȂǊ֐��Ɏ��s�����ꍇ��0x8000���Ԃ����B
 *---------------------------------------------------------------------------*/
u16     WM_GetAllowedChannel(void);

#ifdef  WM_PRECALC_ALLOWEDCHANNEL
/*---------------------------------------------------------------------------*
  Name:         WM_IsExistAllowedChannel

  Description:  �ʐM���Ɏg�p�������ꂽ�`�����l�������݂��邩�ǂ����m�F����B
                WM���C�u�����𖢏������ł����Ă�����ɔ���\�B

  Arguments:    None.

  Returns:      BOOL    -   �g�p�����ꂽ�`�����l�������݂���ꍇ��TRUE���A
                            ���݂��Ȃ��ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL    WM_IsExistAllowedChannel(void);
#endif

/*---------------------------------------------------------------------------*
  Name:         WM_SetEntry

  Description:  �e�@�Ƃ��āA�q�@����̐ڑ��󂯕t���ۂ�؂�ւ���B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                enabled     -   �G���g���[����/�s���t���O�BTRUE:���AFALSE:�s���B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetEntry(WMCallbackFunc callback, BOOL enabled);

/*---------------------------------------------------------------------------*
  Name:         WM_GetLinkLevel

  Description:  �ʐM���̃����N���x���擾����B�����֐��B

  Arguments:    None.

  Returns:      WMLinkLevel -   4�i�K�ɕ]�����������N���x��Ԃ��B
 *---------------------------------------------------------------------------*/
WMLinkLevel WM_GetLinkLevel(void);


/*---------------------------------------------------------------------------*
  Name:         WMi_SetBeaconPeriod

  Description:  Beacon�̊Ԋu��ύX����

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                beaconPeriod -  Beacon�Ԋu(10�`1000 TU(1024��s))

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_SetBeaconPeriod(WMCallbackFunc callback, u16 beaconPeriod);
/* 2004/10/18���݁A�ʐM���Ƀr�[�R���Ԋu��ύX���邱�Ƃɂ���肪���o����
   ���邽�߁A��U�g�p�֎~�֐��Ƃ���ׂɔ���J�Ƃ��܂��B */


/*---------------------------------------------------------------------------*
  Name:         WM_GetDispersionBeaconPeriod

  Description:  �e�@�Ƃ��Ċ�������ۂɐݒ肷�ׂ��r�[�R���Ԋu�l���擾����B
                �����֐��B

  Arguments:    None.

  Returns:      u16 -   �ݒ肷�ׂ��r�[�R���Ԋu�l(ms)�B
 *---------------------------------------------------------------------------*/
u16     WM_GetDispersionBeaconPeriod(void);

/*---------------------------------------------------------------------------*
  Name:         WM_GetDispersionScanPeriod

  Description:  �q�@�Ƃ��Đe�@��T������ۂɐݒ肷�ׂ��T�����x���Ԃ��擾����B
                �����֐��B

  Arguments:    None.

  Returns:      u16 -   �ݒ肷�ׂ��T�����x����(ms)�B
 *---------------------------------------------------------------------------*/
u16     WM_GetDispersionScanPeriod(void);

/*---------------------------------------------------------------------------*
  Name:         WM_GetOtherElements

  Description:  �r�[�R�����̊g���G�������g���擾����B
                �����֐��B

  Arguments:    bssDesc - �e�@���\���́B
                          WM_StartScan(Ex)�ɂĎ擾�����\���̂��w�肷��B

  Returns:      WMOtherElements - �g���G�������g�\���́B
 *---------------------------------------------------------------------------*/
WMOtherElements WM_GetOtherElements(WMBssDesc *bssDesc);

/*---------------------------------------------------------------------------*
  Name:         WM_GetNextTgid

  Description:  �����������ꂽ��ӂ� TGID �l���擾����B
                �����֐��B
                ���̊֐��͓����� RTC_Init() ���Ăяo���B

  Arguments:    None.

  Returns:      ����Ăяo������ RTC �����Ƃɐ������ꂽ TGID���A
                ����ȍ~�͑O��̕Ԃ�l�� 1 �������Z�����l��Ԃ��B
 *---------------------------------------------------------------------------*/
u16     WM_GetNextTgid(void);

/*---------------------------------------------------------------------------*
  Name:         WM_SetPowerSaveMode

  Description:  PowerSaveMode��ύX����

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                powerSave   -   �ȓd�̓��[�h���g�p����ꍇ��TRUE�A���Ȃ��ꍇ��FALSE�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetPowerSaveMode(WMCallbackFunc callback, BOOL powerSave);

/*---------------------------------------------------------------------------*
  Name:         WMi_IsMP

  Description:  ���݂� MP �ʐM��Ԃ��擾����B

  Arguments:    None.

  Returns:      MP �ʐM��Ԃł���� TRUE
 *---------------------------------------------------------------------------*/
BOOL    WMi_IsMP(void);

/*---------------------------------------------------------------------------*
  Name:         WM_GetAID

  Description:  ���݂� AID ���擾����B
                ��Ԃ� PARENT, MP_PARENT, CHILD, MP_CHILD �̂����ꂩ�̎��̂�
                �L���Ȓl��Ԃ��B

  Arguments:    None.

  Returns:      AID
 *---------------------------------------------------------------------------*/
u16     WM_GetAID(void);

/*---------------------------------------------------------------------------*
  Name:         WM_GetConnectedAIDs

  Description:  ���݂̐ڑ�������r�b�g�}�b�v�̌`�Ŏ擾����B
                ��Ԃ� PARENT, MP_PARENT, CHILD, MP_CHILD �̂����ꂩ�̎��̂�
                �L���Ȓl��Ԃ��B
                �q�@�̏ꍇ�͐e�@�Ɛڑ����� 0x0001 ��Ԃ��B

  Arguments:    None.

  Returns:      �ڑ����Ă��鑊��� AID �̃r�b�g�}�b�v
 *---------------------------------------------------------------------------*/
u16     WM_GetConnectedAIDs(void);

/*---------------------------------------------------------------------------*
  Name:         WMi_GetMPReadyAIDs

  Description:  ���݂̐ڑ�����̂����AMP ����M�ł��鑊��� AID �̈ꗗ��
                �r�b�g�}�b�v�̌`�Ŏ擾����B
                ��Ԃ� PARENT, MP_PARENT, CHILD, MP_CHILD �̂����ꂩ�̎��̂�
                �L���Ȓl��Ԃ��B
                �q�@�̏ꍇ�͐e�@�Ɛڑ����� 0x0001 ��Ԃ��B

  Arguments:    None.

  Returns:      MP ���J�n���Ă��鑊��� AID �̃r�b�g�}�b�v
 *---------------------------------------------------------------------------*/
u16     WMi_GetMPReadyAIDs(void);


// for debug
void    WMi_DebugPrintSendQueue(WMPortSendQueue *queue);
void    WMi_DebugPrintAllSendQueue(void);
const WMStatus *WMi_GetStatusAddress(void);
BOOL    WMi_CheckMpPacketTimeRequired(u16 parentSize, u16 childSize, u8 childs);


/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         WM_SIZE_MP_PARENT_RECEIVE_BUFFER

  Description:  �e�@�̎�M�o�b�t�@�̃T�C�Y���v�Z����B

  Arguments:    childMaxSize - �q�@�����M�������f�[�^�̍ő�o�C�g��
                maxEntry     - �ڑ��������q�@��
                ksFlag       - Key Sharing ���s�����ǂ����̃u�[���l

  Returns:      int      - WM_StartMP �ɓn���Ȃ��Ƃ����Ȃ���M�o�b�t�@�̃T�C�Y
 *---------------------------------------------------------------------------*/
#define WM_SIZE_MP_PARENT_RECEIVE_BUFFER( childMaxSize, maxEntry, ksFlag ) \
            (((sizeof( WMMpRecvHeader ) - sizeof( WMMpRecvData ) + \
            ( ( sizeof( WMMpRecvData ) + (childMaxSize) + WM_HEADER_CHILD_MAX_SIZE - 2 + 2/*MACBUG*/ + ((ksFlag) ? WM_SIZE_KS_CHILD_DATA + WM_SIZE_MP_CHILD_PADDING : 0) ) * (maxEntry) ) \
             + 31) & ~0x1f) * 2)

/*---------------------------------------------------------------------------*
  Name:         WM_SIZE_MP_CHILD_RECEIVE_BUFFER

  Description:  �q�@�̎�M�o�b�t�@�̃T�C�Y���v�Z����B

  Arguments:    parentMaxSize - �e�@�����M�������f�[�^�̍ő�o�C�g��
                ksFlag       - Key Sharing ���s�����ǂ����̃u�[���l

  Returns:      int      - WM_StartMP �ɓn���Ȃ��Ƃ����Ȃ���M�o�b�t�@�̃T�C�Y
 *---------------------------------------------------------------------------*/
#define WM_SIZE_MP_CHILD_RECEIVE_BUFFER( parentMaxSize, ksFlag ) \
            (((sizeof( WMMpRecvBuf ) + (parentMaxSize) + WM_HEADER_PARENT_MAX_SIZE - 4 + ((ksFlag) ? WM_SIZE_KS_PARENT_DATA + WM_SIZE_MP_PARENT_PADDING : 0) + 31) & ~0x1f) * 2)

/*---------------------------------------------------------------------------*
  Name:         WM_SIZE_MP_PARENT_SEND_BUFFER

  Description:  �e�@�̑��M�o�b�t�@�̃T�C�Y���v�Z����B

  Arguments:    parentMaxSize - ���M�������f�[�^�̍ő�o�C�g��
                ksFlag       - Key Sharing ���s�����ǂ����̃u�[���l

  Returns:      int      - WM_StartMP �ɓn���Ȃ��Ƃ����Ȃ����M�o�b�t�@�̃T�C�Y
 *---------------------------------------------------------------------------*/
#define WM_SIZE_MP_PARENT_SEND_BUFFER( parentMaxSize, ksFlag ) \
            (((parentMaxSize) + WM_HEADER_PARENT_MAX_SIZE + ((ksFlag) ? WM_SIZE_KS_PARENT_DATA + WM_SIZE_MP_PARENT_PADDING : 0) + 31) & ~0x1f)

/*---------------------------------------------------------------------------*
  Name:         WM_SIZE_MP_CHILD_SEND_BUFFER

  Description:  �q�@�̑��M�o�b�t�@�̃T�C�Y���v�Z����B

  Arguments:    childMaxSize - ���M�������f�[�^�̍ő�o�C�g��
                ksFlag       - Key Sharing ���s�����ǂ����̃u�[���l

  Returns:      int      - WM_StartMP �ɓn���Ȃ��Ƃ����Ȃ����M�o�b�t�@�̃T�C�Y
 *---------------------------------------------------------------------------*/
#define WM_SIZE_MP_CHILD_SEND_BUFFER( childMaxSize, ksFlag ) \
            (((childMaxSize) + WM_HEADER_CHILD_MAX_SIZE + ((ksFlag) ? WM_SIZE_KS_CHILD_DATA + WM_SIZE_MP_CHILD_PADDING : 0) + 31) & ~0x1f)

/*---------------------------------------------------------------------------*
  Name:         WM_SIZE_DCF_CHILD_SEND_BUFFER

  Description:  DCF�q�@�̑��M�o�b�t�@�̃T�C�Y���v�Z����B

  Arguments:    childMaxSize - ���M�������f�[�^�̍ő�o�C�g��

  Returns:      int      - WM_StartDCF �ɓn���Ȃ��Ƃ����Ȃ����M�o�b�t�@�̃T�C�Y
 *---------------------------------------------------------------------------*/
#define WM_SIZE_DCF_CHILD_SEND_BUFFER( childMaxSize ) \
            (((childMaxSize) + WM_SIZE_MADATA_HEADER + 31) & ~0x1f)

#define WM_SIZE_CHILD_SEND_BUFFER WM_SIZE_MP_CHILD_SEND_BUFFER
#define WM_SIZE_CHILD_RECEIVE_BUFFER WM_SIZE_MP_CHILD_RECEIVE_BUFFER
#define WM_SIZE_PARENT_SEND_BUFFER WM_SIZE_MP_PARENT_SEND_BUFFER
#define WM_SIZE_PARENT_RECEIVE_BUFFER WM_SIZE_MP_PARENT_RECEIVE_BUFFER


/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         WM_ConvGgid32to16

  Description:  �Q�[���O���[�vID��"32�r�b�g�̌`"����"16�r�b�g�~2�̌`"�ɕϊ�����B

  Arguments:    src     -   32�r�b�g�ŕ\���ꂽ�ϊ���GGID�ւ̃|�C���^�B
                dest    -   �ϊ����GGID���i�[����o�b�t�@�ւ̃|�C���^�B
                            ���̃A�h���X��擪��4�o�C�g�f�[�^���������܂�܂��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void WM_ConvGgid32to16(u32 *src, u16 *dst)
{
    dst[0] = (u16)(*src & 0x0000ffff);
    dst[1] = (u16)(*src >> 16);
}

/*---------------------------------------------------------------------------*
  Name:         WM_ConvGgid16to32

  Description:  �Q�[���O���[�vID��"16�r�b�g�~2�̌`"����"32�r�b�g�̌`"�ɕϊ�����B

  Arguments:    src     -   16�r�b�g�z��ŕ\���ꂽ�ϊ���GGID�ւ̃|�C���^�B
                dest    -   �ϊ����GGID���i�[����o�b�t�@�ւ̃|�C���^�B
                            ���̃A�h���X��擪��4�o�C�g�f�[�^���������܂�܂��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void WM_ConvGgid16to32(u16 *src, u32 *dst)
{
    *dst = ((u32)src[1] << 16) | src[0];
}

/*---------------------------------------------------------------------------*
  Name:         WM_IsBssidEqual

  Description:  ���BSSID�����������ǂ������`�F�b�N����B

  Arguments:    idp1     -   ��r����BSSID�ւ̃|�C���^�B
                idp2     -   ��r����BSSID�ւ̃|�C���^�B

  Returns:      ��������� TRUE�A
                �قȂ��Ă���� FALSE�B
 *---------------------------------------------------------------------------*/
static inline BOOL WM_IsBssidEqual(const u8 *idp1, const u8 *idp2)
{
    return ((*idp1 == *idp2) &&
            (*(idp1 + 1) == *(idp2 + 1)) &&
            (*(idp1 + 2) == *(idp2 + 2)) &&
            (*(idp1 + 3) == *(idp2 + 3)) &&
            (*(idp1 + 4) == *(idp2 + 4)) && (*(idp1 + 5) == *(idp2 + 5)));
}


/*---------------------------------------------------------------------------*
  Name:         WM_IsBssidEqual16

  Description:  ���BSSID�����������ǂ�����2�o�C�g�P�ʂŃ`�F�b�N����B

  Arguments:    idp1     -   ��r����BSSID�ւ̃|�C���^�B2�o�C�g�A���C�����K�v�B
                idp2     -   ��r����BSSID�ւ̃|�C���^�B2�o�C�g�A���C�����K�v�B

  Returns:      ��������� TRUE�A
                �قȂ��Ă���� FALSE�B
 *---------------------------------------------------------------------------*/
static inline BOOL WM_IsBssidEqual16(const u8 *idp1, const u8 *idp2)
{
    SDK_ALIGN2_ASSERT(idp1);
    SDK_ALIGN2_ASSERT(idp2);

    return ((*(u16 *)idp1 == *(u16 *)idp2) &&
            (*(u16 *)(idp1 + 2) == *(u16 *)(idp2 + 2)) &&
            (*(u16 *)(idp1 + 4) == *(u16 *)(idp2 + 4)));
}

/*---------------------------------------------------------------------------*
  Name:         WM_CopyBssid

  Description:  BSSID���R�s�[����B

  Arguments:    src     -   �R�s�[��BSSID�ւ̃|�C���^�B
                dest    -   �R�s�[��BSSID�ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void WM_CopyBssid(const u8 *src, u8 *dst)
{
    *dst = *src;
    *(dst + 1) = *(src + 1);
    *(dst + 2) = *(src + 2);
    *(dst + 3) = *(src + 3);
    *(dst + 4) = *(src + 4);
    *(dst + 5) = *(src + 5);
}

/*---------------------------------------------------------------------------*
  Name:         WM_CopyBssid16

  Description:  BSSID��2�o�C�g�P�ʂŃR�s�[����B

  Arguments:    src     -   �R�s�[��BSSID�ւ̃|�C���^�B2�o�C�g�A���C�����K�v�B
                dest    -   �R�s�[��BSSID�ւ̃|�C���^�B2�o�C�g�A���C�����K�v�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void WM_CopyBssid16(const u8 *src, u8 *dst)
{
    SDK_ALIGN2_ASSERT(src);
    SDK_ALIGN2_ASSERT(dst);

    *(u16 *)dst = *(u16 *)src;
    *(u16 *)(dst + 2) = *(u16 *)(src + 2);
    *(u16 *)(dst + 4) = *(u16 *)(src + 4);
}

/*---------------------------------------------------------------------------*
  Name:         WM_IsValidGameInfo

  Description:  GameInfo ���Ή����Ă�����̂ł��邩�𒲂ׂ�

  Arguments:    gameInfo - WMGameInfo �\���̂ւ̃|�C���^
                gameInfoLength - gameInfo �̎w����̃f�[�^�̒���

  Returns:      �Ή����Ă��� GameInfo �ł������ꍇ�� TRUE.
 *---------------------------------------------------------------------------*/
static inline BOOL WM_IsValidGameInfo(const WMGameInfo *gameInfo, u16 gameInfoLength)
{
    return (gameInfoLength >= WM_GAMEINFO_LENGTH_MIN
            && gameInfo->magicNumber == WM_GAMEINFO_MAGIC_NUMBER);
}

/*---------------------------------------------------------------------------*
  Name:         WM_IsValidGameBeacon

  Description:  �r�[�R����DS���C�����X�v���C�p�̐e�@����̂��̂ł��邩�𒲂ׂ�

  Arguments:    bssDesc - �r�[�R����񂪓����Ă��� WMBssDesc �\���̂ւ̃|�C���^

  Returns:      DS���C�����X�v���C�p�e�@����̃r�[�R���ł������ꍇ�� TRUE.
 *---------------------------------------------------------------------------*/
static inline BOOL WM_IsValidGameBeacon(const WMBssDesc *bssDesc)
{
    return WM_IsValidGameInfo(&bssDesc->gameInfo, bssDesc->gameInfoLength);
}

/*---------------------------------------------------------------------------*
  Name:         WMi_IsDisconnectReasonFromMyself

  Description:  WM_STATECODE_DISCONNECTED �� reason �l����������̐ؒf����
                ���肷��

  Arguments:    reason - WMStartParentCallback �� WMPortRecvCallback �� reason �����o

  Returns:      ������ WM �̊֐����Ăяo���Đؒf�����ꍇ�� TRUE,
                ���肩��؂�ꂽ�A�܂��͒ʐM�G���[�Ő؂ꂽ�ꍇ�� FALSE
 *---------------------------------------------------------------------------*/
static inline BOOL WMi_IsDisconnectReasonFromMyself(u16 reason)
{
    return (reason >= WM_DISCONNECT_REASON_FROM_MYSELF);
}


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* NITRO_WM_ARM9_WM_API_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

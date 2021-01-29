/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - libraries
  File:     wm.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm.c,v $
  Revision 1.92  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.91  2004/09/13 00:20:35  terui
  �ʃt�@�C���Q�ւ̕��������ɔ����A�X�V�����݂̂��c���ăR�[�h���폜�B

  Revision 1.90  2004/09/10 12:05:40  seiki_masashi
  �L�[�V�F�A�����O�̑��M�o�C�g�������C�u�������ŉB������悤�ɕύX

  Revision 1.89  2004/09/10 04:55:36  seiki_masashi
  WM_GetReceiveBufferSize �𐳊m�Ȓl�ɕύX

  Revision 1.88  2004/09/09 05:54:00  seiki_masashi
  SetMPData �� callback �Ɉ�����n����悤�ɕύX

  Revision 1.87  2004/09/09 01:56:11  terui
  wm.c�t�@�C���̕�������ɔ����ꕔ�֐���ʃt�@�C���ֈړ��B
  �ʃt�@�C������Q�Ƃ��鋤�ʊ֐����������ǉ��B
  static�ϐ��Astatic�֐��ɂ��āA�����K��Ɋ�Â������̂�Rename�B
  WM_Init�AWM_Finish�֐��̒ǉ��B

  Revision 1.86  2004/09/03 07:15:24  terui
  �f�o�b�O�p��CheckParentParameter�֐���ǉ��B

  Revision 1.85  2004/09/03 05:05:30  seiki_masashi
  SetMPDataToPort �� NO_CHILD ��������� child_bitmap==0 �ɕύX

  Revision 1.84  2004/09/03 04:46:49  seiki_masashi
  WMStatus.mp_readyBitmap �𗘗p����悤�ɕύX

  Revision 1.83  2004/09/02 09:27:00  seiki_masashi
  fix typo.

  Revision 1.82  2004/09/02 09:24:42  seiki_masashi
  MPEND_IND ��� port ��M�R�[���o�b�N�������Ȃ������Ƃɔ����A�L���b�V�������̒ǉ�(��)

  Revision 1.81  2004/09/02 09:15:13  terui
  WMi_GetStatusAddress�֐���ǉ��B

  Revision 1.78  2004/08/30 01:59:46  seiki_masashi
  WM_STATECODE_CHILD_CONNECTED �� WM_STATECODE_CONNECTED �ɓ���

  Revision 1.77  2004/08/30 01:58:23  ooe
  small fix

  Revision 1.76  2004/08/30 00:58:51  terui
  �����N���x��ARM7���ňꌳ�Ǘ�����悤�ɏC���B

  Revision 1.75  2004/08/27 08:14:14  seiki_masashi
  small fix.

  Revision 1.74  2004/08/27 06:43:50  seiki_masashi
  WM_StartMPEx �̈����� defaultRetryCount, fixFreqMode ��ǉ�

  Revision 1.73  2004/08/27 04:42:20  terui
  WM_Disconnect�̃p�����[�^�`�F�b�N���C���B

  Revision 1.72  2004/08/27 04:25:37  terui
  WM_Disconnect�ɂ����āA�q�@�ł���ꍇ�̃p�����[�^�`�F�b�N���C���B

  Revision 1.71  2004/08/27 02:16:27  terui
  Only fix comment.

  Revision 1.70  2004/08/25 08:17:32  seiki_masashi
  WM_StatMPEx �̒ǉ�

  Revision 1.69  2004/08/25 05:52:44  terui
  Rename parentRssi to lastRssi.

  Revision 1.68  2004/08/24 13:21:32  terui
  �����N���x�擾�֐���ǉ��B

  Revision 1.67  2004/08/23 04:42:45  ooe
  WM_StartConnect()�̎d�l�ύX�ɑΉ��B

  Revision 1.66  2004/08/21 06:56:50  ooe
  WMStartConnectReq�^���g�p����悤�ɕύX

  Revision 1.65  2004/08/20 11:55:28  terui
  Childs -> Children

  Revision 1.64  2004/08/20 06:48:17  terui
  WM_DisconnectChilds��ǉ��B

  Revision 1.63  2004/08/19 15:25:07  miya
  modified SetGameInfo function.

  Revision 1.62  2004/08/19 05:55:21  miya
  WM_SetGameInfo

  Revision 1.61  2004/08/19 02:53:48  terui
  WM_SetEntry�֐���ǉ��B

  Revision 1.60  2004/08/18 12:39:49  yosiokat
  WM_SetGameInfo��gameInfo�T�C�Y�`�F�b�N��64����WM_SIZE_USER_GAMEINFO�ɏC���B

  Revision 1.59  2004/08/18 06:45:09  terui
  WM_GetAllowedChannel�֐���ǉ��B

  Revision 1.58  2004/08/18 00:22:30  seiki_masashi
  WM_StartMP �̈����̔�����

  Revision 1.57  2004/08/17 12:43:13  seiki_masashi
  WM_StartMP �̈����ɑ��M�o�b�t�@���w�肷��悤�ɕύX
  WM_ERRCODE_FRAME_GAP �̔p�~�ɔ����C��

  Revision 1.56  2004/08/16 06:00:21  seiki_masashi
  DataSharing �̓��I�ڑ��E�ؒf�Ή��̏C��

  Revision 1.55  2004/08/12 14:22:37  seiki_masashi
  DataSharing �̓�������ւ̑Ή�

  Revision 1.54  2004/08/11 10:26:55  seiki_masashi
  ��M�o�b�t�@�̃L���b�V����ARM7 �ɓn���O�ɏ����o���悤�ɏC��

  Revision 1.53  2004/08/11 10:03:03  seiki_masashi
  DataSharing �J�n���̏������C��

  Revision 1.52  2004/08/11 07:26:00  seiki_masashi
  DataSharing �J�n���̏������C��

  Revision 1.51  2004/08/11 04:26:13  seiki_masashi
  StartDataSharing �̈����� doubleMode ��ǉ�
  DataSharing �J�n���̏����̈��艻

  Revision 1.50  2004/08/10 14:16:52  seiki_masashi
  �q�@����� Data �� 2 �܂Ńo�b�t�@�����O����悤�ɂ���̂��ꎞ�����

  Revision 1.49  2004/08/10 12:08:12  seiki_masashi
  WmDataSharingReceiveData ���� WmDataSharingSendDataSet �̕���
  �q�@����� Data �� 2 �܂Ńo�b�t�@�����O����悤��

  Revision 1.48  2004/08/10 10:12:33  terui
  �R�����g�ǉ��A�\�[�X���`�̂݁B

  Revision 1.47  2004/08/10 04:14:35  seiki_masashi
  port ��M�ʒm�� callback �� CONNECT, DISCONNECT ���ʒm����悤�ɕύX
  DataSharing ���ɓ��I�Ɏq�@�������Ă�������p������悤�ɕύX

  Revision 1.46  2004/08/10 01:40:34  seiki_masashi
  �q�@�����Ȃ���Ԃ� StepDataSharing ���Ă�ł����퓮�삷��悤�ɏC��

  Revision 1.45  2004/08/09 10:29:33  seiki_masashi
  DataSharing ���� aidBitmap ���������łȂ� DataSet �͎󂯎��Ȃ��悤�ɕύX

  Revision 1.44  2004/08/07 13:39:39  terui
  DmaCopy32->CpuCopyFast in WM_ReadStatus.

  Revision 1.43  2004/08/07 13:37:25  terui
  Undefine WM_USE_TEMP_STATUS_BUF switch

  Revision 1.42  2004/08/07 10:23:19  seiki_masashi
  ��M�o�b�t�@�̃L���b�V���������� MPEND.ind, MP.ind �Ŏ����I�ɍs���悤�ɕύX
  WMDataSharingInfo �� DataSet �� seq �ԍ���ۑ�

  Revision 1.41  2004/08/07 08:43:16  seiki_masashi
  DataSharing �̑��M�f�[�^�� 32-byte align ��

  Revision 1.40  2004/08/07 07:04:25  seiki_masashi
  WM_StepDataSharing �̈�����ύX

  Revision 1.39  2004/08/07 06:51:18  seiki_masashi
  7 ����̎�M�f�[�^�̃L���b�V���������s��

  Revision 1.38  2004/08/07 05:16:20  seiki_masashi
  small fix

  Revision 1.37  2004/08/07 04:09:02  seiki_masashi
  DataSharing ���� receivedBitmap, aidBitmap �����M����悤�ɕύX
  DataSharing �̑��M�o�b�t�@�̏������Y����C��

  Revision 1.36  2004/08/07 02:58:28  terui
  �R�[���o�b�N�r������t���O����shared�̈�Ɉړ�
  WM_USE_BUF_REMAIN�X�C�b�`���폜

  Revision 1.35  2004/08/07 00:19:40  seiki_masashi
  dataSharing �̎���

  Revision 1.34  2004/08/05 10:01:59  seiki_masashi
  SetMPDataToPort() �� tmptt ���󂯂Ƃ�Ȃ��悤�ɕύX

  Revision 1.33  2004/08/05 08:25:54  seiki_masashi
  sendQueue �̎����ɔ����ύX

  Revision 1.32  2004/08/04 07:12:39  seiki_masashi
  WMi_DebugPrintAllSendQueue �̒ǉ�

  Revision 1.31  2004/08/04 06:20:50  ooe
  InitWirelessCounter�y��GetWirelessCounter��ǉ�

  Revision 1.30  2004/08/04 05:16:29  seiki_masashi
  small fix

  Revision 1.29  2004/08/04 01:45:09  seiki_masashi
  port �����̑O����
  WM_SetMPDataToPort, WM_SetPortCallback �̒ǉ�

  Revision 1.28  2004/08/03 12:25:08  ooe
  WL��TestMode�R�}���h�ύX�ɑΉ�

  Revision 1.27  2004/08/02 06:29:47  terui
  WMbssDesc -> WMBssDesc�ɓ���
  WMstatus -> WMStatus�ɓ���

  Revision 1.26  2004/08/02 02:52:51  terui
  �\���̖��𖽖��K��Ɋ�Â�����

  Revision 1.25  2004/07/30 05:20:17  ooe
  MeasureChannel�R�}���h�𔭍s�ł���悤�ɂ���

  Revision 1.24  2004/07/29 12:09:59  ooe
  WM_MeasureChannel��ǉ�

  Revision 1.23  2004/07/29 07:37:59  miya
  add error message

  Revision 1.22  2004/07/29 06:10:44  miya
  bug fix

  Revision 1.21  2004/07/29 05:13:46  miya
  add comment

  Revision 1.20  2004/07/29 04:20:50  miya
  add SetLifeTime function

  Revision 1.19  2004/07/28 04:42:27  miya
  add comment

  Revision 1.18  2004/07/28 04:21:18  miya
  add functions about ggid & tgid

  Revision 1.17  2004/07/27 00:06:26  terui
  API�Ăяo�����̃X�e�[�g�`�F�b�N������

  Revision 1.16  2004/07/23 14:43:48  terui
  WM_ReadStatus�֐��ɂ�����DMA�R�s�[�T�C�Y��ύX�B

  Revision 1.15  2004/07/20 01:52:52  terui
  small fix

  Revision 1.14  2004/07/20 01:16:20  terui
  �֐������R�����g��啝�ɒǉ��B
  src�Ƃ��Ẵ|�C���^������const�ɓ���B
  ���C�������������ARM7�ƃf�[�^����肷�镔���ł̃L���b�V������̓O��B
  �s�v�Ȋ֐��폜�B
  WM_SetGameInfo�̎d�l�����[�U�[GameInfo�̂ݍX�V�\�Ȏd�l�ɕύX�B
  ���̑��e���C���B

  Revision 1.13  2004/07/16 11:26:09  terui
  Add CheckStateEx.
  Update comments.

  Revision 1.12  2004/07/15 00:32:57  yasu
  fix a small bug

  Revision 1.11  2004/07/14 09:41:24  ooe
  StartScan�̈�����ύX

  Revision 1.10  2004/07/14 05:47:23  seiki_masashi
  fix a bug.

  Revision 1.9  2004/07/13 08:34:24  yada
  sorry, fix a little

  Revision 1.8  2004/07/13 08:32:06  yada
  let WM_sp_init() return WM_ERRCODE_WM_DISABLE when WMsp is not ready

  Revision 1.7  2004/07/12 10:37:26  terui
  Scan�J�n���̃X�e�[�g�`�F�b�N��ύX

  Revision 1.6  2004/07/12 01:31:25  ooe
  small fix

  Revision 1.5  2004/07/11 03:03:33  ooe
  WM_SetBeaconIndication�֌W�̒ǉ��Ȃ�

  Revision 1.4  2004/07/08 10:07:50  terui
  WM7�����J�n�ɔ����ύX

  Revision 1.15  2004/07/06 08:37:46  Ooe
  �g�p���Ă��Ȃ��ϐ����폜

  Revision 1.14  2004/07/06 01:53:33  Ooe
  fifoFlag�̏������������C��

  Revision 1.13  2004/07/05 13:01:22  Ooe
  �L���b�V��������C��

  Revision 1.12  2004/07/02 04:53:01  Miya
  �L���b�V������̏C��

  Revision 1.11  2004/06/30 03:08:53  ikedae
  fifoBuf�@�̃A���C�������g�C��

  Revision 1.10  2004/06/25 09:18:27  Ooe
  Marionea-1.52.00�ɑΉ�

  Revision 1.9  2004/06/25 06:18:06  Ooe
  WM_SetMPData()�̈����ǉ� (pollbmp)

  Revision 1.8  2004/06/24 11:22:03  Ooe
  WM_SetGameInfo()�̃G���[������ǉ��B

  Revision 1.7  2004/06/24 10:04:00  Ooe
  WM_SetGameInfo()��ǉ�

  Revision 1.6  2004/06/22 09:28:06  Ikedae
  (none)

  Revision 1.5  2004/06/22 05:19:16  Ikedae
  WM_lib_Main�폜�֘A�̕ύX

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
/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/

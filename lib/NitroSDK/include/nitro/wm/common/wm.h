/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - include
  File:     wm.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm.h,v $
  Revision 1.172  2007/11/09 13:08:09  seiki_masashi
  �d�g�̎�M�̂݉\�ȏ�Ԃɖ���������������֐���ǉ�

  Revision 1.171  2007/05/02 08:42:57  seiki_masashi
  WM_StartMPEx �֐��ł̒ǉ��ݒ�p�����[�^�̉e�����ꎞ�I�ɂȂ�悤�ύX

  Revision 1.170  2007/02/20 00:28:08  kitase_hirotake
  indent source

  Revision 1.169  2006/07/06 12:53:18  seiki_masashi
  WMGameInfo �\���̂� platform ��ǉ����Aver �� 2 �֕ύX

  Revision 1.168  2006/07/06 11:46:30  okubata_ryoma
  undo

  Revision 1.166  2006/02/20 06:47:38  seiki_masashi
  WM_GAMEINFO_LENGTH_MIN �̒ǉ�

  Revision 1.165  2006/02/20 02:24:45  seiki_masashi
  WMGameInfo.version ��p�~���AmagicNumber �� ver ��ǉ�
  WMGameInfo.gameNameCount_attribute �� attribute �ɖ��O�ύX
  WM_GAMEINFO_TYPE_OLD �̃T�|�[�g��p�~

  Revision 1.164  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.163  2005/12/01 15:06:09  seiki_masashi
  WMPortSend/RecvCallback �\���̂� maxSend/RecvDataSize �t�B�[���h��ǉ�
  WMStartParent/ConenctCallback �\���̂� parent/childSize �t�B�[���h��ǉ�

  Revision 1.162  2005/12/01 13:43:37  seiki_masashi
  WMMPParam �\���̂� maxFrequency, ignoreSizePrecheckMode ��ǉ�

  Revision 1.161  2005/11/25 13:04:32  seiki_masashi
  WMStatus.mp_parentInterval, mp_childInterval, mp_parentSize, mp_childSize �̒ǉ�

  Revision 1.160  2005/11/25 03:01:58  seiki_masashi
  mp_ackAlarm �̒�`�ꏊ�� WMStatus ���� WMSPWork �\���̓��Ɉړ�

  Revision 1.159  2005/11/25 02:34:26  seiki_masashi
  VAlarm �\���̂̒�`�ꏊ�� WMArm7Buf ���� WMSPWork �\���̓��Ɉړ�

  Revision 1.158  2005/11/25 00:46:31  seiki_masashi
  mask �� WMMPParam �\���̓��Ɉړ�

  Revision 1.157  2005/11/24 13:04:17  seiki_masashi
  parentVCount, childVCount �͈̔͐�����ǉ�

  Revision 1.156  2005/11/24 12:21:23  seiki_masashi
  WM_MP_PARAM_MUST_SET_BEFORE_MP �̓��e��ύX

  Revision 1.155  2005/11/24 03:58:15  seiki_masashi
  WMStatus.mp_parentVCount, mp_childVCount �t�B�[���h��ǉ�

  Revision 1.154  2005/11/24 03:02:19  seiki_masashi
  WM_SetMPParameter �֐��Ɗ֘A�����`��ǉ�

  Revision 1.153  2005/11/11 04:23:45  terui
  Fix comment.

  Revision 1.152  2005/11/02 01:31:37  ooe
  WM_StartTestRxMode, WM_StopTestRxMode�֘A�ŁA
  APIID(WM_APIID_START_TESTRXMODE,WM_APIID_STOP_TESTRXMODE)��ǉ��B
  �X�e�[�g�R�[�h(WM_STATE_TESTMODE_RX)��ǉ��B
  ���N�G�X�g�̈����\����(WMStartTestRxModeReq)��ǉ��B
  �R�[���o�b�N�̈����\����(WMStopTestRxModeCallback)��ǉ��B

  Revision 1.151  2005/11/01 08:59:06  seiki_masashi
  WMPortRecvCallback.connectedAidBitmap �̒ǉ�
  WMArm9Buf.myAid, connectedAidBitmap �̒ǉ�
  WM_INDBUF_SIZE �̍폜

  Revision 1.150  2005/10/31 07:01:45  ooe
  WMStartTestModeCallback��ǉ��B

  Revision 1.149  2005/10/28 11:21:07  seiki_masashi
  �萔 WM_STATECODE_DISCONNECTED_FROM_MYSELF �̒ǉ�
  �萔 WM_STATECODE_PORT_INIT �̒ǉ�
  WMDisconnectReason �񋓌^�̒ǉ�
  WMStartConnectCallback �\���̂� macAddress �����o��ǉ�
  WMPortRecvCallback �\���̂̃����o�� aidBitmap ���p�̂� reason ��ǉ�

  Revision 1.148  2005/09/08 02:54:59  terui
  WMScanExParam,WMStartScanExReq �\���̓��̃����o�̕��я��A�^�𒲐�

  Revision 1.147  2005/09/08 00:42:07  terui
  WM_SCANTYPE_*_CUSTOM �萔��ǉ�

  Revision 1.146  2005/09/08 00:38:06  terui
  WMStartScanExReq�\���̂�ssidMatchLength�����o��ǉ�

  Revision 1.145  2005/09/08 00:36:18  terui
  WMScanExParam�\���̂̉������R���p�C���X�C�b�`�Ő��䂷��悤�ɏC��

  Revision 1.144  2005/09/08 00:21:31  terui
  WMScanExParam�\���̂�ssidMatchLength�����o��ǉ�

  Revision 1.143  2005/09/01 01:15:29  ooe
  WMStartConnectCallback�\���̂ɁAwlStatus��ǉ��B

  Revision 1.142  2005/07/29 04:00:48  seiki_masashi
  small fix.

  Revision 1.141  2005/07/27 07:46:41  seiki_masashi
  WM_STATECODE_INFORMATION �̒ǉ�

  Revision 1.140  2005/07/27 06:53:28  ooe
  WM_APIID_SET_PS_MODE��ǉ��B

  Revision 1.139  2005/06/08 08:40:29  seiki_masashi
  WMStatus.mp_limitCount ��ǉ�

  Revision 1.138  2005/06/07 05:44:19  seiki_masashi
  Key Sharing �Ɋւ�����ʈ�����ጸ

  Revision 1.137  2005/03/18 01:39:22  terui
  WM_NUM_MAX_AP_AID �萔��ǉ�

  Revision 1.136  2005/03/17 05:08:48  ooe
  otherElement�ő吔(WM_SCAN_OTHER_ELEMENT_MAX)���`�B
  WMBssDesc�\���̂ɁAotherElementCount�����o��ǉ��B
  WMBssDescWMOtherElements�\���̂�ǉ��B

  Revision 1.135  2005/03/08 06:44:18  terui
  WMStatus�\���̂�pwrMgtMode�����o��ǉ��B

  Revision 1.134  2005/03/04 08:51:19  seiki_masashi
  WMPortSendCallback �� size �t�B�[���h�� length �ł��Q�Ƃł���悤�ɕύX

  Revision 1.133  2005/03/03 14:44:37  seiki_masashi
  ���M�L���[�̒i���� 64 ���� 32 �֕ύX
  WMPortSendQueueData �\���̂� restBitmap ����ǉ�
  WM_DISABLE_KEYSHARING �X�C�b�`�̒ǉ�
  WMStatus �Ƀ��W���[���֘A�� mp_resumeFlag ����ǉ�
  WMPortSendCallback �\���̂� destBitmap ����ǉ�

  Revision 1.132  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.131  2005/02/24 14:25:29  seiki_masashi
  �萔 WMi_DISCONNECT_REASON_* ��ǉ�

  Revision 1.130  2005/02/14 13:39:42  seiki_masashi
  WM_STATECODE_UNKNOWN ��ǉ�

  Revision 1.129  2005/02/08 06:45:05  ooe
  WM_StartScanEx�̎d�l�ύX�ɔ����C���B

  Revision 1.128  2005/02/07 06:52:57  terui
  WMStatus�\���̂�beaconIndicateFlag�����o��ǉ��B
  WMStatus�\���̂�wepKeyId�����o�̔z�u��ύX�B

  Revision 1.127  2005/02/03 11:41:25  ooe
  WM_SetWEPKeyEx��ǉ��ɔ����ύX�B

  Revision 1.126  2005/01/26 05:31:58  takano_makoto
  WMArm9Buf����32�o�C�g���̖��ʂȃp�f�B���O�������Ă����̂��폜

  Revision 1.125  2005/01/26 01:36:15  seiki_masashi
  WMARM9Buf.scanOnlyFlag ��V��

  Revision 1.124  2005/01/18 13:00:26  seiki_masashi
  WM_PACKED_PACKET_MAX �̒ǉ�

  Revision 1.123  2005/01/17 01:07:32  ooe
  WM_StartConnectEx�̈����p�ɁAAUTH���[�h�̒�`��ǉ��B
  MaData�̓]���T�C�Y�w��p�萔��ǉ��B
  API�����\����WMStartConnectReq�ɁApowerSave��authMode��ǉ��B

  Revision 1.122  2005/01/11 07:54:58  takano_makoto
  fix copyright header.

  Revision 1.121  2005/01/07 11:04:47  terui
  ARM7�p�o�b�t�@�T�C�Y���g�����A�����I�Ƀ��N�G�X�g��񓯊��I�ɔ��s����ׂ̃o�b�t�@���m�ہB

  Revision 1.120  2004/12/27 09:06:21  ooe
  ���ۂɂ͑��݂��Ȃ�����"WM_WEPMODE_232BIT"���폜�B

  Revision 1.119  2004/12/27 05:30:41  terui
  WM_API_REQUEST_ACCEPTED��`��ǉ��B

  Revision 1.118  2004/11/10 13:02:51  takano_makoto
  include�t�@�C�����C��

  Revision 1.117  2004/11/09 02:45:23  seiki_masashi
  Null �����ɂ��^�C���A�E�g������
  �����̎��s�L���[�ł� VAlarm �����҂��������ɕ������܂�Ȃ��悤�ɕύX

  Revision 1.116  2004/11/08 04:16:46  terui
  MWStartScanExCallback�\���̂�linkLevel�����o��ǉ��B

  Revision 1.115  2004/11/04 12:14:28  terui
  WMStartScanExCallback��channel�����o��ǉ��B

  Revision 1.114  2004/11/02 09:12:56  seiki_masashi
  �X�C�b�`�p�̃}�N���萔 WM_DS_DATA_SIZE_252 ���g�p����悤�ɕύX

  Revision 1.113  2004/11/02 07:28:42  terui
  �R�����g�C���B

  Revision 1.112  2004/10/30 04:31:02  yasu
  comment out '#undef WM_ENABLE_TESTMODE'

  Revision 1.111  2004/10/27 04:29:57  ooe
  StartScanEx�̃R�[���o�b�N������ύX�B

  Revision 1.110  2004/10/26 08:23:39  ooe
  WM_StartScanEx��ǉ��BWM_SIZE_SCAN_EX_BUF���`�B

  Revision 1.109  2004/10/26 00:53:57  terui
  WMDisconnectCallback�\���̂�V�K�ǉ��B

  Revision 1.108  2004/10/22 09:31:58  seiki_masashi
  Resume �p�ϐ��EAPIID�̒ǉ�

  Revision 1.107  2004/10/22 01:54:42  seiki_masashi
  �f�[�^�V�F�A�����O�̍ő�T�C�Y�� 252 �o�C�g���� 508 �o�C�g�Ɋg��

  Revision 1.106  2004/10/05 09:45:29  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.105  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.104  2004/10/04 11:43:54  terui
  WM_SIZE_MP_DATA_MAX��`���X�V�B

  Revision 1.103  2004/10/04 00:51:47  terui
  WM_SIZE_MP_DATA_MAX�� 510 -> 508 �֕ύX�B(514-header(2)-footer(4))

  Revision 1.102  2004/10/03 07:52:54  ooe
  ipl_branch�ƃ}�[�W�B ignoreFatalError�t���O�y�сAFlash�G���[�ɑΉ��B

  Revision 1.101  2004/10/01 13:22:26  seiki_masashi
  LinkLevel �̌v�Z������ύX

  Revision 1.100  2004/10/01 04:11:23  terui
  WM_ENABLE_TESTMODE�X�C�b�`��V�݁B�f�t�H���g��undef�ɁB
  �����g�p���`�����l�����Ɍv�Z����@�\��؂�ւ���X�C�b�`��ǉ��B

  Revision 1.99  2004/09/24 08:22:31  seiki_masashi
  fix comment

  Revision 1.98  2004/09/24 04:52:16  seiki_masashi
  WM_STATECODE_FIFO_ERROR �̒ǉ�
  WMIndCallback �� reason ��ǉ�

  Revision 1.97  2004/09/24 01:22:42  ooe
  WMMpRecvHeader�ɁAerrBitmap��ǉ�(WL-2.05.00�ȍ~)

  Revision 1.96  2004/09/23 12:17:05  ooe
  AutoDisconnect��ǉ��B
  ����ɕK�v��WMStatus�̃����o"curr_tgid"���ǉ��B

  Revision 1.95  2004/09/23 09:40:39  seiki_masashi
  WMDataSharingInfo �� state �t�B�[���h��ǉ�

  Revision 1.94  2004/09/20 10:30:39  ooe
  WM_APIID_SET_BEACON_PERIOD��ǉ��B

  Revision 1.93  2004/09/17 06:36:40  seiki_masashi
  WMPortRecvCallback �� ssid ����� myAid �t�B�[���h��ǉ�

  Revision 1.92  2004/09/15 07:33:50  seiki_masashi
  WMStatus.mp_bufferEmptyFlag �̒ǉ�

  Revision 1.91  2004/09/15 06:38:31  seiki_masashi
  WMStatus.mp_ping* �̒ǉ�

  Revision 1.90  2004/09/15 05:40:27  ooe
  WMBeaconRecvIndCallback�̃����o�ɁA
  gameInfo��gameInfoLength��ǉ��B

  Revision 1.89  2004/09/13 04:22:06  seiki_masashi
  small fix.

  Revision 1.88  2004/09/13 04:09:16  seiki_masashi
  WM_SetMPFrequency �̒ǉ�

  Revision 1.87  2004/09/10 12:04:37  seiki_masashi
  �L�[�V�F�A�����O�̑��M�o�C�g�������C�u�������ŉB������悤�ɕύX

  Revision 1.86  2004/09/09 12:39:45  seiki_masashi
  WM_HEADER_SIZE �̒ǉ�

  Revision 1.85  2004/09/09 07:24:11  seiki_masashi
  WM_SetMPDataToPortEx �̒ǉ�

  Revision 1.84  2004/09/09 02:00:57  terui
  �g�p���Ă��Ȃ�Indication�p�o�b�t�@�̍폜�B
  APIID�̖��̂��ꕔ�ύX�B
  �����X�e�[�g��WM_STATE_READY(WM_Enable�O)��ǉ��B

  Revision 1.83  2004/09/08 04:25:54  terui
  WMApiid�̒�`��ύX�AEnable�ADisable�AIdlize�AStop�p��ID��ǉ��B
  WMArm9Buf���̃p�f�B���O�T�C�Y��ύX�B

  Revision 1.82  2004/09/03 04:36:41  seiki_masashi
  WM_ERRCODE_SEND_FAILED �̒ǉ�
  WMStatus.mp_readyBitmap �̒ǉ�

  Revision 1.81  2004/09/02 08:39:28  terui
  WM_SIZE_MP_DATA_MAX��`��ǉ��B

  Revision 1.80  2004/09/01 07:00:36  terui
  WMState�񋓌^��WM_STATE_MAX��ǉ��B

  Revision 1.79  2004/08/31 09:26:07  seiki_masashi
  WMStatus.mp_setDataFlag, mp_sentDataFlag, WM_ERRCODE_NO_DATA �̒ǉ�

  Revision 1.78  2004/08/30 08:37:39  seiki_masashi
  WMStatus.mp_dataSettingFlag, mp_isPolledFlag �̒ǉ�
  WM_ERRCODE_INVALID_POLLBITMAP �̒ǉ�

  Revision 1.77  2004/08/30 01:58:59  seiki_masashi
  WM_STATECODE_CHILD_CONNECTED �� WM_STATECODE_CONNECTED �ɓ���

  Revision 1.76  2004/08/30 00:53:51  terui
  Add definition about LinkLevel

  Revision 1.75  2004/08/27 08:17:18  terui
  Change pass to version_wl.h

  Revision 1.74  2004/08/27 06:46:33  seiki_masashi
  WMStatus.mp_newFrameFlag, mp_fixFreqMode, mp_defaultRetryCount �̒ǉ�
  WMSendQueueData.retryCount �̒ǉ�

  Revision 1.73  2004/08/26 09:42:35  ooe
  BeaconRecv�̃R�[���o�b�N�Ɉ����ǉ�

  Revision 1.72  2004/08/26 07:02:41  terui
  Include NITRO_WL_VERSION.

  Revision 1.71  2004/08/25 08:18:24  seiki_masashi
  status->mp_minPollBmpMode �� mp_singlePacketMode �̒ǉ�

  Revision 1.70  2004/08/25 05:51:25  terui
  Rename parentRssi to lastRssi.

  Revision 1.69  2004/08/24 13:19:32  terui
  WMStatus��parentRssi�����o��ǉ��B

  Revision 1.68  2004/08/23 04:39:43  ooe
  WM_StartConnect()�̎d�l�ύX�B

  Revision 1.67  2004/08/21 11:08:41  ooe
  WL-1.71.00�Ή�

  Revision 1.66  2004/08/21 06:56:01  ooe
  WMStartConnectReq�^���쐬

  Revision 1.65  2004/08/21 06:14:52  ooe
  WMStartParentCallback�̈�����ssid��ssidLength��ǉ��B

  Revision 1.64  2004/08/20 04:29:17  terui
  �G���[�R�[�h��WM_ERRCODE_OVER_MAX_ENTRY��ǉ��B

  Revision 1.63  2004/08/19 16:24:19  miya
  switch GAMEINFO_TYPE

  Revision 1.62  2004/08/19 14:53:04  miya
  bug fix.

  Revision 1.61  2004/08/19 11:14:14  miya
  add GAMEINFO_VERSION

  Revision 1.60  2004/08/19 06:37:34  seiki_masashi
  Key Sharing �p�� DataSharingInfo �\���̂� 32-Byte align �ɍ����悤�C��

  Revision 1.59  2004/08/19 02:41:36  terui
  Entry�ۂɂ�鎩���ؒf�pAPIID��ǉ��B
  �q�@��Entry�󂯕t�����ۂ��ꂽ�ꍇ�̃G���[�R�[�h��ǉ��B

  Revision 1.58  2004/08/18 13:12:01  yosiokat
  WM_GAMEINFO_TYPE_OLD���`�B

  Revision 1.57  2004/08/18 09:01:38  miya
  modified WMGameInfo format

  Revision 1.56  2004/08/18 06:34:34  terui
  WM_SetEntry�pAPIID��ǉ��B
  WMStatus�\���̂�allowedChannel�����o��ǉ�(�p�f�B���O�ƍ��ւ�)�B

  Revision 1.55  2004/08/18 00:24:15  seiki_masashi
  �ꕔ�萔�� wm_private.h ����ړ�

  Revision 1.54  2004/08/17 12:50:45  seiki_masashi
  ���M�o�b�t�@�����[�U�v���O�����w��̂��̂��g�p����悤�ɕύX
  status->maxReceiveSize �̒ǉ�

  Revision 1.53  2004/08/16 06:50:50  ooe
  WMBeaconRecvIndCallback��ǉ�

  Revision 1.52  2004/08/12 14:28:11  seiki_masashi
  WM_ERRCODE_FRAME_GAP �̒ǉ���

  Revision 1.51  2004/08/11 04:26:45  seiki_masashi
  WMDataSharingInfo �� doubleMode �t�B�[���h��ǉ�

  Revision 1.50  2004/08/11 00:19:42  terui
  �|�[�g�ɗ\���ǉ�

  Revision 1.49  2004/08/10 09:10:57  terui
  ARM9��p�̊֐��Q��`��wm_api.h�ɐ؂�o��

  Revision 1.48  2004/08/10 04:12:13  seiki_masashi
  WMPortRecvCallback �ɐڑ��E�ؒf�ʒm�p�� macAddress �t�B�[���h��ǉ�

  Revision 1.47  2004/08/09 10:19:47  seiki_masashi
  ���M���I���܂� senqQueue �̍Đݒ���ł��Ȃ��悤�ɔr�����������

  Revision 1.46  2004/08/07 14:15:44  seiki_masashi
  �A������ WMSP_SendMaMP, SendMaKeyData �̋N�������֐���ǉ�

  Revision 1.45  2004/08/07 10:19:46  seiki_masashi
  WMDataSharingInfo �� DataSet �� seq �ԍ���ۑ�

  Revision 1.44  2004/08/07 08:45:49  seiki_masashi
  DataSharing �̑��M�f�[�^�� 32-byte align ��

  Revision 1.43  2004/08/07 07:04:40  seiki_masashi
  WM_StepDataSharing �̈�����ύX

  Revision 1.42  2004/08/07 04:06:44  seiki_masashi
  WMDataSet �� receivedBitmap �t�B�[���h��ǉ�

  Revision 1.41  2004/08/07 02:55:56  terui
  �R�[���o�b�N�r������t���O����shared�̈�Ɉړ�

  Revision 1.40  2004/08/07 01:22:47  seiki_masashi
  WMStatus.sendQueueMutex �̒ǉ�

  Revision 1.39  2004/08/07 00:23:10  seiki_masashi
  dataSharing �̒ǉ�

  Revision 1.38  2004/08/06 05:10:44  seiki_masashi
  small fix

  Revision 1.37  2004/08/05 11:59:16  ooe
  GetWirelessCounter�̍��ڐ������ɑΉ�

  Revision 1.36  2004/08/05 10:03:13  seiki_masashi
  SetMPDataToPort() �� tmptt ���󂯂Ƃ�Ȃ��悤�ɕύX

  Revision 1.35  2004/08/05 08:26:14  seiki_masashi
  sendQueue �̎����ɔ����ύX

  Revision 1.34  2004/08/04 06:57:03  seiki_masashi
  WMStatus �� portSeqNo ��ǉ�

  Revision 1.33  2004/08/04 06:48:04  seiki_masashi
  ���M queue �����̏���
  WM_STATUS_BUF_SIZE �� 512 ���� 2048 ��

  Revision 1.32  2004/08/04 06:21:33  ooe
  InitWirelessCounter�y��GetWirelessCounter��ǉ�

  Revision 1.31  2004/08/04 02:29:39  seiki_masashi
  WMPortSendQueue �\���̂̒�`�̒ǉ�

  Revision 1.30  2004/08/04 01:44:30  seiki_masashi
  port �����̑O����

  Revision 1.29  2004/08/03 12:22:28  ooe
  WL��TestMode�R�}���h�ύX�ɑΉ�

  Revision 1.28  2004/08/03 00:52:58  yosiokat
  WM_SIZE_SYSTEM_GAMEINFO��"40"�ɏC���B�i���̏ꂵ�̂��j

  Revision 1.27  2004/08/02 06:29:15  terui
  WMbssDesc -> WMBssDesc�ɓ���
  WMstatus -> WMStatus�ɓ���

  Revision 1.26  2004/08/02 05:42:46  ooe
  small fix

  Revision 1.25  2004/08/02 05:34:49  ooe
  WMstartMPCallback�\���̂ɁAMPACK�p�̃����o��ǉ�

  Revision 1.24  2004/08/02 02:52:32  terui
  �\���̖��𖽖��K��Ɋ�Â�����

  Revision 1.23  2004/07/31 09:03:41  sato_masaki
  WMgameInfo�\���̒���userGameInfo�����o��4byte�A���C�������g�Ŋm�ۂ��邽��

  Revision 1.22  2004/07/30 05:20:57  ooe
  MeasureChannel�֌W�̒ǉ��ύX

  Revision 1.21  2004/07/29 12:11:00  ooe
  WM_MeasureChannel��ǉ�

  Revision 1.20  2004/07/29 07:54:19  ikedae
  �ؒf����reasonCode�ǉ�

  Revision 1.19  2004/07/29 07:38:46  miya
  modified callback table size

  Revision 1.18  2004/07/29 06:50:08  miya
  modified WMarm9Buf size

  Revision 1.17  2004/07/29 05:43:00  miya
  modified callback table size

  Revision 1.16  2004/07/29 04:21:38  miya
  add SetLifeTime function

  Revision 1.15  2004/07/28 07:35:20  terui
  WM_EndConnect�֐��̒�`���폜

  Revision 1.14  2004/07/28 04:23:18  miya
  add functions about ggid & tgid

  Revision 1.13  2004/07/28 02:54:16  terui
  �\���̂̃p�f�B���O�C���B
  enum�Ƀ^�C�v����ǉ��B
  WM_APIID_*��enum�^�ɕύX�B
  WMState��`��wm_private.h����ړ��B

  Revision 1.12  2004/07/27 07:32:24  yasu
  Add #pragma  warn_padding off

  Revision 1.11  2004/07/27 07:17:41  yasu
  Fix alignment 32 on WMpparam

  Revision 1.10  2004/07/20 01:24:34  terui
  indicate�pSTATECODE�̒ǉ��B
  indicate�p�R�[���o�b�N�x�N�g���ޔ�ϐ���ǉ��B
  �e�@���ɐڑ��q�@�ő吔�����o��ǉ��B
  scan_continue�Ascanning_ch�����o���폜�B
  WM_getIndBuf�֐����폜�B
  WM_SetGameInfo�֐��̎d�l��ύX�B
  src�Ƃ��ă|�C���^�������ɂƂ�֐���const�ɓ���B

  Revision 1.9  2004/07/14 09:42:50  ooe
  StartScan�̈����ύX�ɔ����C��

  Revision 1.8  2004/07/11 02:59:30  ooe
  WM_SetBeaconIndication()�̒ǉ�

  Revision 1.7  2004/07/10 12:52:39  miya
  �r�[�R���n�ǉ�

  Revision 1.6  2004/07/10 12:45:08  terui
  Modify comments.

  Revision 1.5  2004/07/10 09:30:30  ooe
  #define WM_APIID_VALARM_MP 23 ��ǉ�

  Revision 1.4  2004/07/08 09:50:35  terui
  WM7�̉����J�n�ɔ����ύX

  Revision 1.17  2004/07/06 08:04:27  Ooe
  WL-1.56.00�ւ̑Ή�

  Revision 1.16  2004/07/06 05:49:05  Ooe
  FIFO�o�b�t�@�̃T�C�Y��256Byte�ɖ߂���

  Revision 1.15  2004/07/06 02:08:24  Ooe
  WM_ATTR_FLAG_CS �̒�`��ǉ�

  Revision 1.14  2004/07/06 01:51:28  Ooe
  FIFO�o�b�t�@�T�C�Y��ύX

  Revision 1.13  2004/07/05 13:01:47  Ooe
  SYSTEM_BUF_SIZE�ɃJ�b�R��ǉ�

  Revision 1.12  2004/07/03 07:28:43  Ooe
  �T�C�Y�̒�`�Ȃǂ��X�V

  Revision 1.11  2004/07/02 04:51:46  Miya
  �L���b�V������T�C�Y�̏C��

  Revision 1.10  2004/06/29 06:50:27  Ooe
  BssDesc�\���̂��C��

  Revision 1.9  2004/06/29 06:41:43  Miya
  �f�[�^�T�C�Y�̃n�[�h�R�[�f�B���O���ł��邾���}�N����

  Revision 1.8  2004/06/25 06:18:27  Ooe
  WM_SetMPData()�̈����ǉ� (pollbmp)

  Revision 1.7  2004/06/25 02:15:27  Ooe
  WM_SetGameInfo()�̎����ɂ��I������DMA����݂̃o�O���C���B

  Revision 1.6  2004/06/24 10:02:44  Ooe
  WM_SetGameInfo()��ǉ�

  Revision 1.5  2004/06/21 06:39:42  Ooe
  (none)

  Revision 1.4  2004/06/19 05:56:58  Miya
  modified header file reference

  Revision 1.3  2004/06/19 04:03:08  miya
  ver. up Marionea-1.48.00

  Revision 1.2  2004/06/02 04:18:40  miya
  (none)

  Revision 1.1  2004/05/26 08:47:17  terui
  �����g�ݍ��݂ɔ����ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITRO_WM_COMMON_WM_H_
#define NITRO_WM_COMMON_WM_H_

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include <nitro/types.h>
#include <nitro/os.h>
#include <nitro_wl/common/version_wl.h>


/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/

#undef WM_GAMEINFO_TYPE_OLD            // WMGameInfo �̋��`���ւ̑Ή��͔p�~����܂���

// Data Sharing �̍ő�T�C�Y�� 252 �o�C�g�ɂ��� WMDataSharingInfo �\���̂��X�����ɂ���X�C�b�`
//#define WM_DS_DATA_SIZE_252

// TEST���[�h�L�����X�C�b�`
//#define WM_ENABLE_TESTMODE

// �g�p�\�`�����l�����Ɍv�Z���Ă����R���p�C���X�C�b�`
#if ( SDK_VERSION_WL >= 15600 )
#ifdef  SDK_TEG
#undef  WM_PRECALC_ALLOWEDCHANNEL
#else
#define WM_PRECALC_ALLOWEDCHANNEL
#endif
#else
#undef  WM_PRECALC_ALLOWEDCHANNEL
#endif

#define WM_SSID_MASK_CUSTOMIZE  1


// WM API�֘A ===========================================================================

// �eAPI��ID
typedef enum WMApiid
{
    WM_APIID_INITIALIZE = 0,           //  0 : WM_Initialize()
    WM_APIID_RESET,                    //  1 : WM_Reset()
    WM_APIID_END,                      //  2 : WM_End()

    WM_APIID_ENABLE,                   //  3 : WM_Enable()
    WM_APIID_DISABLE,                  //  4 : WM_Disable()
    WM_APIID_POWER_ON,                 //  5 : WM_PowerOn()
    WM_APIID_POWER_OFF,                //  6 : WM_PowerOff()

    WM_APIID_SET_P_PARAM,              //  7 : WM_SetParentParameter()
    WM_APIID_START_PARENT,             //  8 : WM_StartParent()
    WM_APIID_END_PARENT,               //  9 : WM_EndParent()
    WM_APIID_START_SCAN,               // 10 : WM_StartScan()
    WM_APIID_END_SCAN,                 // 11 : WM_EndScan()
    WM_APIID_START_CONNECT,            // 12 : WM_StartConnect()
    WM_APIID_DISCONNECT,               // 13 : WM_Disconnect()
    WM_APIID_START_MP,                 // 14 : WM_StartMP()
    WM_APIID_SET_MP_DATA,              // 15 : WM_SetMPData()
    WM_APIID_END_MP,                   // 16 : WM_EndMP()
    WM_APIID_START_DCF,                // 17 : WM_StartDCF()
    WM_APIID_SET_DCF_DATA,             // 18 : WM_SetDCFData()
    WM_APIID_END_DCF,                  // 19 : WM_EndDCF()
    WM_APIID_SET_WEPKEY,               // 20 : WM_SetWEPKey()
    WM_APIID_START_KS,                 // 21 : WM_StartKeySharing() ���ʌ݊��̂��ߎc���Ă��܂�
    WM_APIID_END_KS,                   // 22 : WM_EndKeySharing()   ���ʌ݊��̂��ߎc���Ă��܂�
    WM_APIID_GET_KEYSET,               // 23 : WM_GetKeySet()
    WM_APIID_SET_GAMEINFO,             // 24 : WM_SetGameInfo()
    WM_APIID_SET_BEACON_IND,           // 25 : WM_SetBeaconIndication()
    WM_APIID_START_TESTMODE,           // 26 : WM_StartTestMode()
    WM_APIID_STOP_TESTMODE,            // 27 : WM_StopTestMode()
    WM_APIID_VALARM_MP,                // 28 : ARM7����VAlarm�����MP�N���p(����)
    WM_APIID_SET_LIFETIME,             // 29 : WM_SetLifeTime()
    WM_APIID_MEASURE_CHANNEL,          // 30 : WM_MeasureChannel()
    WM_APIID_INIT_W_COUNTER,           // 31 : WM_InitWirelessCounter()
    WM_APIID_GET_W_COUNTER,            // 32 : WM_GetWirelessCounter()
    WM_APIID_SET_ENTRY,                // 33 : WM_SetEntry()
    WM_APIID_AUTO_DEAUTH,              // 34 : ARM7����indicate����̎����ؒf�N���p(����)
    WM_APIID_SET_MP_PARAMETER,         // 35 : WM_SetMPParameter()
    WM_APIID_SET_BEACON_PERIOD,        // 36 : WM_SetBeaconPeriod()
    WM_APIID_AUTO_DISCONNECT,          // 37 : ARM7����indicate����̎����ؒf�N���p(����)
    WM_APIID_START_SCAN_EX,            // 38 : WM_StartScanEx()
    WM_APIID_SET_WEPKEY_EX,            // 39 : WM_SetWEPKeyEx()
    WM_APIID_SET_PS_MODE,              // 40 : WM_SetPowerSaveMode()
    WM_APIID_START_TESTRXMODE,         // 41 : WM_StartTestRxMode()
    WM_APIID_STOP_TESTRXMODE,          // 42 : WM_StopTestRxMode()

    WM_APIID_KICK_MP_PARENT,           // 43 : MP�N���p(ARM7�����g�p)
    WM_APIID_KICK_MP_CHILD,            // 44 : MP�N���p(ARM7�����g�p)
    WM_APIID_KICK_MP_RESUME,           // 45 : MP�N���p(ARM7�����g�p)

    WM_APIID_ASYNC_KIND_MAX,           // 46 : �񓯊������̎��

    WM_APIID_INDICATION = 128,         // 128 : Indication�R�[���o�b�N�p
    WM_APIID_PORT_SEND,                // 129 : port �ւ̃f�[�^���M�ʒm
    WM_APIID_PORT_RECV,                // 130 : port �ւ̃f�[�^��M�ʒm
    WM_APIID_READ_STATUS,              // 131 : WM_ReadStatus()

    WM_APIID_UNKNOWN = 255             // 255 : �s���̃R�}���h�ԍ�����ARM7����Ԃ��l
}
WMApiid;

// for backward compatibility
//#define WM_APIID_SET_MP_FREQ    WM_APIID_SET_MP_PARAMETER

// ARM7���ł�API�v����t�����t���O
#define WM_API_REQUEST_ACCEPTED 0x8000

// WEP���[�h
#define WM_WEPMODE_NO           0
#define WM_WEPMODE_40BIT        1
#define WM_WEPMODE_104BIT       2
#define WM_WEPMODE_128BIT       3

// AUTH���[�h (WM_StartConnectEx�Ŏg�p����)
#define WM_AUTHMODE_OPEN_SYSTEM 0
#define WM_AUTHMODE_SHARED_KEY  1

// ScanType
#define WM_SCANTYPE_ACTIVE      0
#define WM_SCANTYPE_PASSIVE     1

#if WM_SSID_MASK_CUSTOMIZE
#define WM_SCANTYPE_ACTIVE_CUSTOM   2
#define WM_SCANTYPE_PASSIVE_CUSTOM  3
#endif

// BSS�ЂƂɂ��ڑ��\�Ȏq�@�ő吔
#define WM_NUM_MAX_CHILD        15     // NITRO �e�@����^����ꂤ�� AssociationID �̍ő�l
#define WM_NUM_MAX_AP_AID       2007   // �������[�^����^����ꂤ�� AssociationID �̍ő�l

// port ��
#define WM_NUM_OF_PORT          16     // port �̌�
#define WM_NUM_OF_SEQ_PORT      8      // sequential �ȏ������s�� port �̌�
typedef enum WMPort
{
    // Sequential �Ȑ�����s��Ȃ��|�[�g�Q
    WM_PORT_RAWDATA = 0,               // ���ʌ݊��p�̃|�[�g
    WM_PORT_BT = 1,                    // �u���b�N�]���p�|�[�g
    WM_PORT_RESERVE_02 = 2,            // �\��
    WM_PORT_RESERVE_03 = 3,            // �\��

    // Sequential �Ȑ�����s���|�[�g�Q
    WM_PORT_RESERVE_10 = 8,            // �\��
    WM_PORT_RESERVE_11 = 9,            // �\��
    WM_PORT_RESERVE_12 = 10,           // �\��
    WM_PORT_RESERVE_13 = 11            // �\��
}
WMPort;

// ���M queue �֘A
#define WM_SEND_QUEUE_NUM       32     // ���v 32 �i�� queue list
#define WM_SEND_QUEUE_END       ((u16)0xffff)   // tail ��\���l
#define WM_PRIORITY_LEVEL       4      // �D��x�� 4 �i�K
typedef enum WMPriorityLevel
{
    WM_PRIORITY_URGENT = 0,
    WM_PRIORITY_HIGH,
    WM_PRIORITY_NORMAL,
    WM_PRIORITY_LOW
}
WMPriorityLevel;

// ARM7 ���� MP ���M�������n�܂�f�t�H���g�� V Count �l
#define WM_VALARM_COUNT_CHILD_MP  240  // �q�@MP����
#define WM_VALARM_COUNT_PARENT_MP 260  // �e�@MP����
#define WM_VALARM_COUNT_RANGE_TOP    220        // parentVCount, childVCount �̐ݒ�\��� (220�`262, 0�`190)
#define WM_VALARM_COUNT_RANGE_BOTTOM 190        // parentVCount, childVCount �̐ݒ�\����

// ���M�Ԋu
#define WM_MP_FREQ_CONT         16     // �A�����M�ɂȂ鑗�M�p�x
#define WM_MP_COUNT_LIMIT       256    // �c�著�M�񐔃J�E���^�̏���l
#define WM_DEFAULT_MP_FREQ_LIMIT 6     // �f�t�H���g�� MP �p�x���
#define WM_DEFAULT_MP_PARENT_INTERVAL 1000      // �f�t�H���g�̐e�@�� MP �Ԋu (us)
#define WM_DEFAULT_MP_CHILD_INTERVAL  0 // �f�t�H���g�̎q�@�� MP �Ԋu (us)

// �e��f�[�^�T�C�Y(�T�C�Y�̒P�ʂ̓o�C�g)
#define WM_SIZE_BSSID           6
#define WM_SIZE_SSID            32

#define WM_SIZE_GAMEINFO        128    // �ő�T�C�Y

#define WM_SIZE_SCAN_EX_BUF     1024   // StartScanEx�p�o�b�t�@�T�C�Y

#define WM_GAMEINFO_LENGTH_MIN  16     // �L���� GameInfo �̍Œ�T�C�Y
#define WM_GAMEINFO_MAGIC_NUMBER 0x0001 // DS �p GameInfo �̎��ʗp magic number
#define WM_GAMEINFO_VERSION_NUMBER 1   // GameInfo �̃o�[�W�����ԍ�
#define WM_GAMEINFO_PLATFORM_ID_NITRO       0   // �e�@�� DS �ł���
#define WM_GAMEINFO_PLATFORM_ID_REVOLUTION  8   // �e�@�� Wii �ł���
#define WM_SIZE_SYSTEM_GAMEINFO 16     // userGameInfoLength�܂ł̃T�C�Y
#define WM_SIZE_USER_GAMEINFO   112    // GameInfo�̃��[�U�[�̈�̍ő�T�C�Y

#define WM_SIZE_SCAN_PARAM      32

#define WM_PARENT_PARAM_SIZE    64
#define WM_SIZE_USERNAME        8
#define WM_SIZE_GAMENAME        16
#define WM_SIZE_GGID            4

#define WM_SIZE_WL_VERSION      8
#define WM_SIZE_BBP_VERSION     4
#define WM_SIZE_MACADDR         6
#define WM_SIZE_WEPKEY          80

#define WM_SIZE_CHILD_SSID      24

#if ( SDK_VERSION_WL >= 21100 )
#define WM_SIZE_MP_DATA_MAX     512
#else
#define WM_SIZE_MP_DATA_MAX     508
#endif

#define WM_SIZE_MADATA_HEADER   44

// �e�@�������̃p�P�b�g��1���MP�ʐM�ɓ����ۂɃp�P�b�g�ԂɕK�v�ȃo�C�g��
#define WM_SIZE_MP_PARENT_PADDING   (WM_HEADER_SIZE + WM_HEADER_PARENT_MAX_SIZE)
// �q�@�������̃p�P�b�g��1���MP�ʐM�ɓ����ۂɃp�P�b�g�ԂɕK�v�ȃo�C�g��
#define WM_SIZE_MP_CHILD_PADDING    (WM_HEADER_SIZE + WM_HEADER_CHILD_MAX_SIZE)

#define WM_SIZE_DS_PARENT_HEADER    4
#define WM_SIZE_KS_PARENT_DATA  (2*16+WM_SIZE_DS_PARENT_HEADER)
#define WM_SIZE_KS_CHILD_DATA   2

// �A�g���r���[�g�t���O

#define WM_ATTR_ENTRY_SHIFT     0
#define WM_ATTR_MB_SHIFT        1
#define WM_ATTR_KS_SHIFT        2
#define WM_ATTR_CS_SHIFT        3

#define WM_ATTR_FLAG_ENTRY      (1 << WM_ATTR_ENTRY_SHIFT)
#define WM_ATTR_FLAG_MB         (1 << WM_ATTR_MB_SHIFT)
#define WM_ATTR_FLAG_KS         (1 << WM_ATTR_KS_SHIFT)
#define WM_ATTR_FLAG_CS         (1 << WM_ATTR_CS_SHIFT)

// �R�[���o�b�N�r������p
#define WM_EXCEPTION_CB_MASK    0x0001

// wmHeader
#define WM_HEADER_SIZE          2      // �e�q���� wmHeader �̃o�C�g��

//#define WM_HEADER_MP        0x0001
#define WM_HEADER_KS        0x4000
#define WM_HEADER_VSYNC     0x8000

// packetHeader
#define WM_HEADER_PARENT_MAX_SIZE   4  // �w�b�_�Ƃ��čő�ŕt�������o�C�g�� (wmHeader 2byte ���͏���)
                                        // ���݂� Sequence Number �� Destination Bitmap ���t�������ꍇ������

#define WM_HEADER_CHILD_MAX_SIZE    2  // �w�b�_�Ƃ��čő�ŕt�������o�C�g�� (wmHeader 2byte ���͏���)
                                        // ���݂� Sequence Number ���t�������ꍇ������

#define WM_HEADER_PORT_MASK     0x0f00
#define WM_HEADER_PORT_SHIFT    8
#define WM_HEADER_SEQ_FLAG      0x0800
#define WM_HEADER_DEST_BITMAP   0x1000
#define WM_HEADER_LENGTH_MASK   0x00ff
#define WM_HEADER_LENGTH_SCALE  2

#define WM_SEQ_PORT_FLAG        0x0008
#define WM_SEQ_PORT_MASK        0x0007

// packetHeader Seq No Field ( 2byte�� )
#define WM_HEADER_SEQ_RETRY     0x8000
#define WM_HEADER_SEQ_NUM_MASK  0x7fff

// 1�̃G���x���[�u�Ƀp�b�N�\�ȍő�p�P�b�g��(>= WM_SIZE_MP_DATA_MAX/WM_SIZE_MP_PARENT_PADDING + 1)
#define WM_PACKED_PACKET_MAX    128


// API�̃��U���g�R�[�h
typedef enum WMErrCode
{
    WM_ERRCODE_SUCCESS = 0,
    WM_ERRCODE_FAILED = 1,
    WM_ERRCODE_OPERATING = 2,
    WM_ERRCODE_ILLEGAL_STATE = 3,
    WM_ERRCODE_WM_DISABLE = 4,
    WM_ERRCODE_NO_KEYSET = 5,
    WM_ERRCODE_NO_DATASET = 5,         // NO_KEYSET �Ɠ����l
    WM_ERRCODE_INVALID_PARAM = 6,
    WM_ERRCODE_NO_CHILD = 7,
    WM_ERRCODE_FIFO_ERROR = 8,
    WM_ERRCODE_TIMEOUT = 9,
    WM_ERRCODE_SEND_QUEUE_FULL = 10,
    WM_ERRCODE_NO_ENTRY = 11,
    WM_ERRCODE_OVER_MAX_ENTRY = 12,
    WM_ERRCODE_INVALID_POLLBITMAP = 13,
    WM_ERRCODE_NO_DATA = 14,
    WM_ERRCODE_SEND_FAILED = 15,

    WM_ERRCODE_DCF_TEST,               // �f�o�b�O�p
    WM_ERRCODE_WL_INVALID_PARAM,       // �f�o�b�O�p (�폜��)
    WM_ERRCODE_WL_LENGTH_ERR,          // �f�o�b�O�p (�폜��)

    WM_ERRCODE_FLASH_ERROR,            // WL�̃t���b�V���p�����[�^�G���[(�v���I�Ȃ̂Ńt���[�Y����)
    WM_ERRCODE_MAX
}
WMErrCode;

// API�̃R�[���o�b�N�ŕԂ��X�e�[�g�R�[�h
typedef enum WMStateCode
{
    WM_STATECODE_PARENT_START = 0,     // StartParent�p �e�@����J�n
    WM_STATECODE_BEACON_SENT = 2,      // StartParent�p Beacon���M����

    WM_STATECODE_SCAN_START = 3,       // StartScan�p Scan����J�n
    WM_STATECODE_PARENT_NOT_FOUND = 4, // StartScan�p �e�@�𔭌��ł��Ȃ�
    WM_STATECODE_PARENT_FOUND = 5,     // StartScan�p �e�@�𔭌�����

    WM_STATECODE_CONNECT_START = 6,    // StartConnect�p �ڑ������J�n
    WM_STATECODE_BEACON_LOST = 8,      // StartConnect�p �e�@�̃r�[�R������������

    WM_STATECODE_CONNECTED = 7,        // StartParent�y��StartConnect, port �p �ڑ�����
    WM_STATECODE_CHILD_CONNECTED = 7,  // WM_STATECODE_CONNECTED �Ɠ����l
    WM_STATECODE_DISCONNECTED = 9,     // StartParent�y��StartConnect, port �p �ؒf�ʒm
    WM_STATECODE_DISCONNECTED_FROM_MYSELF = 26, // StartParent�y��StartConnect, port �p ��������̐ؒf�ʒm

    WM_STATECODE_MP_START = 10,        // StartMP�p MP�ʐM���[�h�J�n
    WM_STATECODE_MPEND_IND = 11,       // StartMP�p �e�@MP�V�[�P���X����(��M)
    WM_STATECODE_MP_IND = 12,          // StartMP�p �q�@MP��M
    WM_STATECODE_MPACK_IND = 13,       // StartMP�p �q�@MPACK��M

    WM_STATECODE_DCF_START = 14,       // StartDCF�p DCF�ʐM���[�h�J�n
    WM_STATECODE_DCF_IND = 15,         // StartDCF�p DCF�f�[�^��M

    WM_STATECODE_BEACON_RECV = 16,     // Beacon��Mindicate
    WM_STATECODE_DISASSOCIATE = 17,    // �ڑ��ؒfindicate
    WM_STATECODE_REASSOCIATE = 18,     // �Đڑ�indicate
    WM_STATECODE_AUTHENTICATE = 19,    // �F�؊m�Findicate

    WM_STATECODE_PORT_INIT = 25,       // port ������
    WM_STATECODE_PORT_SEND = 20,       // port �փf�[�^���M
    WM_STATECODE_PORT_RECV = 21,       // port �փf�[�^��M

    WM_STATECODE_FIFO_ERROR = 22,      // ARM7��FIFO�G���[�����ʒm
    WM_STATECODE_INFORMATION = 23,     // ���ʒm
    WM_STATECODE_UNKNOWN = 24,         // �s����̒ʒm

    WM_STATECODE_MAX = 27
}
WMStateCode;

// WM�̃X�e�[�g�R�[�h
typedef enum WMState
{
    WM_STATE_READY = 0,                // READY�X�e�[�g(Init�O�ALED�ʏ�_�����)
    WM_STATE_STOP,                     // STOP�X�e�[�g (Initialize�O�ALED�_�ŏ��)
    WM_STATE_IDLE,                     // IDLE�X�e�[�g (Initialize��, Reset��Ȃ�)
    WM_STATE_CLASS1,                   // CLASS1�X�e�[�g
    WM_STATE_TESTMODE,                 // TESTMODE(�\���p)
    WM_STATE_SCAN,                     // StartScan��
    WM_STATE_CONNECT,                  // StartConnect��
    WM_STATE_PARENT,                   // StartParent��
    WM_STATE_CHILD,                    // �ڑ�������̎q�@
    WM_STATE_MP_PARENT,                // StartMP��̐e�@
    WM_STATE_MP_CHILD,                 // StartMP��̎q�@
    WM_STATE_DCF_CHILD,                // StartDCF��̎q�@
    WM_STATE_TESTMODE_RX,              // TESTMODE_RX
    WM_STATE_MAX
}
WMState;

// �����N���x
typedef enum WMLinkLevel
{
    WM_LINK_LEVEL_0 = 0,
    WM_LINK_LEVEL_1,
    WM_LINK_LEVEL_2,
    WM_LINK_LEVEL_3,
    WM_LINK_LEVEL_MAX
}
WMLinkLevel;

// Data Sharing �̓������
typedef enum WMDataSharingState
{
    WM_DS_STATE_READY = 0,             // WM_StartDataSharing �O
    WM_DS_STATE_START,                 // WM_StepDataSharing ��
    WM_DS_STATE_PAUSING,               // Pause ��Ԃւ̑J�ڒ�
    WM_DS_STATE_PAUSED,                // WM_StartDataSharing ����܂��� Pause ��
    WM_DS_STATE_RETRY_SEND,            // SEND_QUEUE_FULL �������̂ōđ����K�v
    WM_DS_STATE_ERROR                  // Error ����
}
WMDataSharingState;

// �ؒf���� reason �R�[�h
typedef enum WMDisconnectReason
{
    // �O���v���ɂ��ؒf�Ɋւ��� reason �l
    WM_DISCONNECT_REASON_RESERVED = 0, // �\��
    WM_DISCONNECT_REASON_UNSPECIFIED = 1,       // ����ł��Ȃ��G���[
    WM_DISCONNECT_REASON_PREV_AUTH_INVALID = 2, // ���O�̔F�؂͂��͂�L���ł���܂���
    WM_DISCONNECT_REASON_DEAUTH_LEAVING = 3,    // BSS ���痣�ꂽ���ߔF�؂��������܂���
    WM_DISCONNECT_REASON_INACTIVE = 4, // �s�����̂��ߐڑ����������܂���
    WM_DISCONNECT_REASON_UNABLE_HANDLE = 5,     // AP �ɏ\���Ȏ������Ȃ����ߐڑ����������܂���
    WM_DISCONNECT_REASON_RX_CLASS2_FROM_NONAUTH_STA = 6,        // �F�؂���Ă��Ȃ� STA ���� Class2 �̃t���[������M���܂���
    WM_DISCONNECT_REASON_RX_CLASS3_FROM_NONASSOC_STA = 7,       // �ڑ�����Ă��Ȃ� STA ���� Class3 �̃t���[������M���܂���
    WM_DISCONNECT_REASON_DISASSOC_LEAVING = 8,  // BSS ���痣�ꂽ���߃A�\�V�G�[�V�������������܂���
    WM_DISCONNECT_REASON_ASSOC_STA_NOTAUTHED = 9,       // �ڑ��v������ STA �͂܂��F�؂��󂯂Ă��܂���
    // DS �̃��[�J���ʐM�ŗL�� reason �l
    WM_DISCONNECT_REASON_NO_ENTRY = 19, // DS �̐e�@�͌��݃G���g���[���󂯕t���Ă��܂���
    // ���C�u������������̎����ؒf�Ɋւ��� reason �l
    WM_DISCONNECT_REASON_MP_LIFETIME = 0x8001,  // MP �ʐM���C�t�^�C�����؂�܂���
    WM_DISCONNECT_REASON_TGID_CHANGED = 0x8002, // ��M beacon ���� TGID ���ω����܂���
    WM_DISCONNECT_REASON_FATAL_ERROR = 0x8003,  // ���C�u�������ŉ񕜕s�\�ȃG���[���������܂���
    // ���C�u�����ɂ��ؒf�Ɋւ��� reason �l
    WM_DISCONNECT_REASON_FROM_MYSELF = 0xf001   // WM �� API ���g�p���A��������ؒf���܂���
}
WMDisconnectReason;

// �e��ʒm
typedef enum WMInfoCode
{
    WM_INFOCODE_NONE = 0,
    WM_INFOCODE_FATAL_ERROR            // Fatal Error �����ʒm
}
WMInfoCode;

// WM_SetMPParameter �֐��� mask �p�萔
#define WM_MP_PARAM_MIN_FREQUENCY              0x0001
#define WM_MP_PARAM_FREQUENCY                  0x0002
#define WM_MP_PARAM_MAX_FREQUENCY              0x0004
#define WM_MP_PARAM_PARENT_SIZE                0x0008
#define WM_MP_PARAM_CHILD_SIZE                 0x0010
#define WM_MP_PARAM_PARENT_INTERVAL            0x0020
#define WM_MP_PARAM_CHILD_INTERVAL             0x0040
#define WM_MP_PARAM_PARENT_VCOUNT              0x0080
#define WM_MP_PARAM_CHILD_VCOUNT               0x0100
#define WM_MP_PARAM_DEFAULT_RETRY_COUNT        0x0200
#define WM_MP_PARAM_MIN_POLL_BMP_MODE          0x0400
#define WM_MP_PARAM_SINGLE_PACKET_MODE         0x0800
#define WM_MP_PARAM_IGNORE_FATAL_ERROR_MODE    0x1000
#define WM_MP_PARAM_IGNORE_SIZE_PRECHECK_MODE  0x2000

// MP �ʐM���ɂ͐ݒ�ł��Ȃ��p�����[�^�̃��X�g
#define WM_MP_PARAM_MUST_SET_BEFORE_MP      (WM_MP_PARAM_MIN_POLL_BMP_MODE|WM_MP_PARAM_SINGLE_PACKET_MODE|WM_MP_PARAM_IGNORE_SIZE_PRECHECK_MODE)

// WMMPTmpParam.mask �p�萔
#define WM_MP_TMP_PARAM_MIN_FREQUENCY              0x0001
#define WM_MP_TMP_PARAM_FREQUENCY                  0x0002
#define WM_MP_TMP_PARAM_MAX_FREQUENCY              0x0004
#define WM_MP_TMP_PARAM_DEFAULT_RETRY_COUNT        0x0200
#define WM_MP_TMP_PARAM_MIN_POLL_BMP_MODE          0x0400
#define WM_MP_TMP_PARAM_SINGLE_PACKET_MODE         0x0800
#define WM_MP_TMP_PARAM_IGNORE_FATAL_ERROR_MODE    0x1000

// �p�����[�^�̌��E�l
#define WM_MP_PARAM_INTERVAL_MAX 10000 // interval �ɐݒ�\�Ȓl�� 10000us �ȉ�

// miscFlags �p�萔
#define WM_MISC_FLAG_LISTEN_ONLY               0x0001
#define WM_MISC_FLAG_NO_BLINK                  0x0002

// WM API�̃R�[���o�b�N�̌^
typedef void (*WMcallbackFunc) (void *arg);     // WM API�̃R�[���o�b�N�^
typedef void (*WMCallbackFunc) (void *arg);     // WM API�̃R�[���o�b�N�^

// �o�b�t�@�T�C�Y =======================================================================

//������������������������
//�� WM9 variable       �� 512
//������������������������
//�� WM7 variable       �� 256 + 512
//������������������������
//�� WM status          �� 2048
//������������������������
//�� FIFO buf (9 �� 7)  �� 256
//������������������������
//�� FIFO buf (9 �� 7)  �� 256
//������������������������

// WM�Ŏg�p����o�b�t�@�̃T�C�Y -------------------

#define WM_ARM9WM_BUF_SIZE      512    // ARM9��WM�̕ϐ��̈�̃T�C�Y
#define WM_ARM7WM_BUF_SIZE      ( 256 + 512 )   // ARM7��WM�̕ϐ��̈�̃T�C�Y
#define WM_STATUS_BUF_SIZE      2048   // WM�̃X�e�[�^�X�̈�̃T�C�Y
#define WM_FIFO_BUF_SIZE        256    // ARM9,7�Ԃ�FIFO�R�}���h�o�b�t�@�T�C�Y

// ���[�U�[���m�ۂ���o�b�t�@�̃T�C�Y -------------
//   WM�V�X�e���Ŏg�p����S�o�b�t�@�̃T�C�Y 512 + 256 + 512 + 2048 + 256 + 256 = 0xf00 (FIFO_BIF * 2 �ł��邱�Ƃɒ���)
#define WM_SYSTEM_BUF_SIZE      (WM_ARM9WM_BUF_SIZE + WM_ARM7WM_BUF_SIZE + WM_STATUS_BUF_SIZE + WM_FIFO_BUF_SIZE + WM_FIFO_BUF_SIZE)
#define WM_BSS_DESC_SIZE        192    // WM_StartScan()�ň����n���A�e�@���i�[�p�o�b�t�@�̃T�C�Y

#define WM_DCF_MAX_SIZE         1508   // DCF�Ŏ�M�ł���ő�T�C�Y
#define WM_KEYSET_SIZE          36     // KeySet�̃T�C�Y
#define WM_KEYDATA_SIZE         2      // KeyData�̃T�C�Y

#define WM_DS_HEADER_SIZE       4

#ifdef WM_DS_DATA_SIZE_252             // WM_DS_DATA_SIZE == 252 �ō\��Ȃ��ꍇ
#define WM_DS_DATA_SIZE         252
#else  // WM_DS_DATA_SIZE == 508 �̏ꍇ�i�f�t�H���g�j
#define WM_DS_DATA_SIZE         508    // 1�� WMDataSet �̍ő�f�[�^�T�C�Y(512-4)
                                       // WM_DS_DATA_SIZE+WM_DS_HEADER_SIZE �� 32 �̔{���̕K�v�L
#endif

#define WM_DS_DATASET_NUM       4      // WMDataSetBuf �ɂ����� WMDataSet ��ێ����邩(�Œ�l)
#define WM_DS_INFO_SIZE         (sizeof(WMDataSharingInfo))     // DataSharing�p�o�b�t�@�T�C�Y

#define WM_SCAN_EX_PARENT_MAX   16     // 1���ScanEx�Ŕ����\�ȍő�e�@�䐔
#define WM_SCAN_OTHER_ELEMENT_MAX 16   // �ʒm�\��otherElement�̍ő吔

/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/

// �o�b�t�@�\���� =======================================================================

// �f�[�^�Z�b�g�\����
typedef struct WMDataSet
{
//    u16 seqNum;                 // �f�[�^�Z�b�g�̃V�[�P���V�����i���o�[
    u16     aidBitmap;
    u16     receivedBitmap;            // ��M��Ԃ̃r�b�g�}�b�v
    u16     data[WM_DS_DATA_SIZE / sizeof(u16)];        // ���L�f�[�^

}
WMDataSet;

// DataSharing ���\����
typedef struct WMDataSharingInfo
{
    WMDataSet ds[WM_DS_DATASET_NUM];
    u16     seqNum[WM_DS_DATASET_NUM]; // Sequential Number
    u16     writeIndex;                // ���ɏ������݂��s�� Index
    u16     sendIndex;                 // ���ݑ��M���� Index
    u16     readIndex;                 // ���ɓǂݏo����� Index
    u16     aidBitmap;                 // �f�[�^���L�̒��Ԃ� bitmap�i�����܂ށj
    u16     dataLength;                // 1��ӂ�̋��L�f�[�^��
    u16     stationNumber;             // �f�[�^���L�ɎQ�����Ă���䐔 (aidBitmap �̗����Ă���r�b�g�̐�)
    u16     dataSetLength;             // dataLength * stationNumber
    u16     port;                      // �g�p���Ă��� port (8�Ԉȏ�)
    u16     doubleMode;                // ���t���[������M�\�Ȍ��ݒʐM���[�h���ǂ���
    u16     currentSeqNum;             // ���O�ɓǂݏo���� DataSet �� SeqNum
    u16     state;                     // ���݂� DataSharing �̏��(WMDataSharingState)
    u16     reserved[1];               // for 32Byte Alignment
}
WMDataSharingInfo;

// �L�[�Z�b�g�\����
typedef struct WMKeySet
{
    u16     seqNum;                    // �L�[�Z�b�g�̃V�[�P���V�����i���o�[
    u16     rsv;
    u16     key[16];                   // �L�[�f�[�^

}
WMKeySet, WMkeySet;

// �L�[�Z�b�g�ۑ��p�����O�o�b�t�@
typedef WMDataSharingInfo WMKeySetBuf, WMkeySetBuf;

//---------------------------------------
// �e�q���� MP port ���M�L���[�\����
typedef struct
{
    u16     next;
    u16     port;
    u16     destBitmap;
    u16     restBitmap;
    u16     sentBitmap;
    u16     sendingBitmap;
    u16     padding;
    u16     size;
    u16     seqNo;
    u16     retryCount;
    const u16 *data;
    WMCallbackFunc callback;
    void   *arg;
}
WMPortSendQueueData;

typedef struct
{
    u16     head;
    u16     tail;
}
WMPortSendQueue;

//---------------------------------------
// �q�@ MP��M�o�b�t�@�\����
typedef struct WMMpRecvBuf
{
    u16     rsv1[3];
    u16     length;

    u16     rsv2[1];
    u16     ackTimeStamp;
    u16     timeStamp;
    u16     rate_rssi;
    u16     rsv3[2];

    u16     rsv4[2];
    u8      destAdrs[6];
    u8      srcAdrs[6];
    u16     rsv5[3];
    u16     seqCtrl;

    u16     txop;
    u16     bitmap;
    u16     wmHeader;
    u16     data[2];

}
WMMpRecvBuf, WMmpRecvBuf;

//---------------------------------------
// �e�@ MP��M�o�b�t�@�\����
typedef struct WMMpRecvData
{
    u16     length;
    u16     rate_rssi;
    u16     aid;
    u16     noResponse;
    u16     wmHeader;
    u16     cdata[1];

}
WMMpRecvData, WMmpRecvData;

typedef struct WMMpRecvHeader
{
    u16     bitmap;
#if SDK_VERSION_WL >= 20500
    u16     errBitmap;
#endif
    u16     count;
    u16     length;
    u16     txCount;
    WMMpRecvData data[1];

}
WMMpRecvHeader, WMmpRecvHeader;

//---------------------------------------
// �q�@ DCF��M�o�b�t�@�\����
typedef struct WMDcfRecvBuf
{
    u16     frameID;
    u16     rsv1[2];
    u16     length;

    u16     rsv2[3];
    u16     rate_rssi;
    u16     rsv3[4];
    u8      destAdrs[6];
    u8      srcAdrs[6];
    u16     rsv4[4];

    u16     data[2];

}
WMDcfRecvBuf, WMdcfRecvBuf;

//---------------------------------------
// WM �e�@�p�����[�^�\����
typedef struct WMParentParam
{
    u16    *userGameInfo;              // ���[�U�[�̈�ɃZ�b�g����f�[�^�̃|�C���^
    u16     userGameInfoLength;        // ���[�U�[�̈�̒���
    u16     padding;

    u32     ggid;
    u16     tgid;                      // �e���|������GameID(���[�U�[������e�@�ɂȂ�Ƃ��ɐ�������)
    u16     entryFlag;                 // 1:�ڑ���, 0:�ڑ��s��
    u16     maxEntry;                  // �ڑ��\�q�@�䐔
    u16     multiBootFlag;             // 1:�}���`�u�[�g��, 0:�}���`�u�[�g�s��
    u16     KS_Flag;                   // 1:KeySharing����, 0:KeySharing����
    u16     CS_Flag;                   // 1:�A�����M, 0:1�t���[�����̑��M
    u16     beaconPeriod;              // Beacon�Ԋu(ms)
    u16     rsv1[WM_SIZE_USERNAME / sizeof(u16)];       //
    u16     rsv2[WM_SIZE_GAMENAME / sizeof(u16)];       //
    u16     channel;                   // �`�����l��
    u16     parentMaxSize;             // �e�@�ő呗�M�T�C�Y
    u16     childMaxSize;              // �q�@�ő呗�M�T�C�Y

    u16     rsv[4];                    // 32�o�C�g�A���C�����g�̂���

}
WMParentParam, WMpparam;

//---------------------------------------
// WM GameInfo�\����

typedef struct WMGameInfo
{
    u16     magicNumber;               // == 0x0001
    u8      ver;                       // GameInfo Version 1
    u8      platform;                  // DS: 0, Wii: 8
    u32     ggid;
    u16     tgid;
    u8      userGameInfoLength;        // ���[�U�[�̈�̒���
    union
    {
        // (CS_Flag):1, (KS_Flag):1, multibootFlag:1, entryFlag:1
        u8      gameNameCount_attribute;        // �݊��̂��߂̋���
        u8      attribute;
    };
    u16     parentMaxSize;             //�e�@�ő呗�M�T�C�Y
    u16     childMaxSize;              //�q�@�ő呗�M�T�C�Y
    union
    {
        u16     userGameInfo[WM_SIZE_USER_GAMEINFO / sizeof(u16)];      // ���[�U�[�̈�(�ő�112�o�C�g (�b��))
        struct
        {
            u16     userName[WM_SIZE_USERNAME / sizeof(u16)];   // WM_SIZE_USERNAME -> 8
            u16     gameName[WM_SIZE_GAMENAME / sizeof(u16)];   // WM_SIZE_GAMENAME -> 16
            u16     padd1[44];         // pad -> 88 
        }
        old_type;                      // �v 112�o�C�g
    };
}
WMGameInfo, WMgameInfo;

//---------------------------------------
// BSSDescription �e�@���\����(Scan�Ŋl�����AConnect�ł̎w��Ɏg�p����)
#if SDK_VERSION_WL < 17100
typedef struct WMBssDesc
{
    u16     length;                    // 2
    u16     rssi;                      // 4
    u8      bssid[WM_SIZE_BSSID];      // 10
    u16     ssidLength;                // 12
    u8      ssid[WM_SIZE_SSID];        // 44
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
    u16     gameInfoLength;            // 62
    WMGameInfo gameInfo;               // 190
    u16     rsv;                       // 192

}
WMBssDesc, WMbssDesc;
#else
typedef struct WMBssDesc
{
    u16     length;                    // 2
    u16     rssi;                      // 4
    u8      bssid[WM_SIZE_BSSID];      // 10
    u16     ssidLength;                // 12
    u8      ssid[WM_SIZE_SSID];        // 44
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
    u16     gameInfoLength;            // 62
    u16     otherElementCount;         // 64
    WMGameInfo gameInfo;               // 192

}
WMBssDesc, WMbssDesc;
#endif

//---------------------------------------
// WM �r�[�R�����g���G�������g�\����
typedef struct WMOtherElements
{
    u8      count;
    u8      rsv[3];
    struct
    {
        u8      id;
        u8      length;
        u8      rsv[2];
        u8     *body;
    }
    element[WM_SCAN_OTHER_ELEMENT_MAX];
}
WMOtherElements;

//---------------------------------------
// WM Scan�p�����[�^�\����
typedef struct WMScanParam
{
    WMBssDesc *scanBuf;                // �e�@�����i�[���邽�߂̃o�b�t�@
    u16     channel;                   // Scan���s��channel
    u16     maxChannelTime;            // �ő�Scan����
    u8      bssid[WM_SIZE_BSSID];      // Scan�Ώېe�@MacAddress(0xff�Ȃ�S�e�@�Ώ�)
    u16     rsv[9];

}
WMScanParam, WMscanParam;

//---------------------------------------
// WM ScanEx�p�����[�^�\����
typedef struct WMScanExParam
{
    WMBssDesc *scanBuf;                // �e�@�����i�[���邽�߂̃o�b�t�@
    u16     scanBufSize;               // scanBuf�̃T�C�Y
    u16     channelList;               // Scan���s��channel���X�g(�����w��� WM_GetAllowedChannel�̖߂�l�Ɠ��l�̌`��)
    u16     maxChannelTime;            // �ő�Scan����
    u8      bssid[WM_SIZE_BSSID];      // Scan�Ώېe�@MacAddress(0xff�Ȃ�S�e�@�Ώ�)
    u16     scanType;                  // �ʏ��WM_SCANTYPE_PASSIVE�BActiveScan����WM_SCANTYPE_ACTIVE�B
    u16     ssidLength;                // �Ώېe�@��SSID��(0�̏ꍇ�͑S�e�@���ΏۂɂȂ�)
    u8      ssid[WM_SIZE_SSID];        // Scan�Ώېe�@��SSID
#if WM_SSID_MASK_CUSTOMIZE
    u16     ssidMatchLength;
    u16     rsv2[7];
#else
    u16     rsv2[8];
#endif

}
WMScanExParam, WMscanExParam;

//---------------------------------------
// WM SetMPParameter �p�����[�^�\����
typedef struct WMMPParam
{
    u32     mask;                      // �ȉ��̂ǂ̃t�B�[���h���L�������r�b�g�Ŏw�肷��

    u16     minFrequency;              // 1�s�N�`���t���[��������� MP ��(�ŏ��l)(������)
    u16     frequency;                 // 1�s�N�`���t���[��������� MP ��
    u16     maxFrequency;              // 1�s�N�`���t���[��������� MP ��(����l)
    u16     parentSize;                // ���݂̐e�@���M�T�C�Y
    u16     childSize;                 // ���݂̎q�@���M�T�C�Y
    u16     parentInterval;            // 1�t���[�����ɘA�����čs���ꍇ�̐e�@�� MP �ʐM�Ԋu (us)
    u16     childInterval;             // 1�t���[�����ɘA�����čs���ꍇ�̎q�@�� MP �ʐM����������Ԋu (us)
    u16     parentVCount;              // �t���[�������ʐM���̐e�@�����̊J�n V Count
    u16     childVCount;               // �t���[�������ʐM���̎q�@�����̊J�n V Count
    u16     defaultRetryCount;         // Raw �ʐM port �ł̒ʐM�̃f�t�H���g���g���C��
    u8      minPollBmpMode;            // PollBitmap �𑗐M����p�P�b�g�̑��M��ɍ��킹�čŏ��ɂ��郂�[�h
    u8      singlePacketMode;          // 1MP �� 1packet ��������Ȃ��Ȃ郂�[�h
    u8      ignoreFatalErrorMode;      // Fatal Error �𖳎����郂�[�h
    u8      ignoreSizePrecheckMode;    // �ʐM�J�n���̑���M�T�C�Y�̎��O�`�F�b�N�𖳎����郂�[�h
}
WMMPParam;

//---------------------------------------
// WM StartMP �p�ꎞ�p�����[�^�\����
typedef struct WMMPTmpParam
{
    u32     mask;                      // �ȉ��̂ǂ̃t�B�[���h���L�������r�b�g�Ŏw�肷��

    u16     minFrequency;              // 1�s�N�`���t���[��������� MP ��(�ŏ��l)(������)
    u16     frequency;                 // 1�s�N�`���t���[��������� MP ��
    u16     maxFrequency;              // 1�s�N�`���t���[��������� MP ��(����l)
    u16     defaultRetryCount;         // Raw �ʐM port �ł̒ʐM�̃f�t�H���g���g���C��
    u8      minPollBmpMode;            // PollBitmap �𑗐M����p�P�b�g�̑��M��ɍ��킹�čŏ��ɂ��郂�[�h
    u8      singlePacketMode;          // 1MP �� 1packet ��������Ȃ��Ȃ郂�[�h
    u8      ignoreFatalErrorMode;      // Fatal Error �𖳎����郂�[�h
    u8      reserved[1];
}
WMMPTmpParam;

//---------------------------------------
// WM �X�e�[�^�X�o�b�t�@�\���� (����������ARM7�ōs���BARM9�͓ǂݏo���̂݉�)
typedef struct WMStatus
{
    // �X�e�[�g��� (4Byte)
    u16     state;                     // WM�̃X�e�[�g���
    u16     BusyApiid;                 // ���s��APIID

    // �t���O (32Byte) 36
    BOOL    apiBusy;                   // API���s���t���O(FIFO�ɂ��API���s�֎~�t���O)
    BOOL    scan_continue;             // Scan�p���t���O(2004/07/19���݁A���g�p)
    BOOL    mp_flag;                   // MP���s���t���O
    BOOL    dcf_flag;                  // DCF���s���t���O
    BOOL    ks_flag;                   // KeySharing���s���t���O
    BOOL    dcf_sendFlag;              // DCF���M�҂��t���O
    BOOL    VSyncFlag;                 // VBlank����OK�t���O

    // �o�[�W������� (16Byte) 52
    u8      wlVersion[WM_SIZE_WL_VERSION];      // WL�̃o�[�W����
    u16     macVersion;                // MAC�̃o�[�W����
    u16     rfVersion;                 // RF�̃o�[�W����
    u16     bbpVersion[WM_SIZE_BBP_VERSION / sizeof(u16)];      // BBP�̃o�[�W����

    // MP�֌W (106Byte) 158
    u16     mp_parentSize;             // �e�@��1��� MP �ʐM�ł̑��M�T�C�Y
    u16     mp_childSize;              // �q�@��1��� MP �ʐM�ł̑��M�T�C�Y
    u16     mp_parentMaxSize;          // �e�@��1��� MP �ʐM�ł̑��M�T�C�Y�̍ő�l
    u16     mp_childMaxSize;           // �q�@��1��� MP �ʐM�ł̑��M�T�C�Y�̍ő�l
    u16     mp_sendSize;               // ���݂̐ڑ��ł̍ő呗�M�T�C�Y
    u16     mp_recvSize;               // ���݂̐ڑ��ł̍ő��M�T�C�Y
    u16     mp_maxSendSize;            // ���݂̐ڑ��ł̍ő呗�M�T�C�Y (parentMaxSize+4 or childMaxSize+2)
    u16     mp_maxRecvSize;            // ���݂̐ڑ��ł̍ő��M�T�C�Y (childMaxSize+2 or parentMaxSize+4)
    u16     mp_parentVCount;           // �e�@���� MP �����J�n VCount
    u16     mp_childVCount;            // �q�@���� MP �����J�n VCount
    u16     mp_parentInterval;         // �e�@���g�p���� MP �ʐM�Ԋu (us)
    u16     mp_childInterval;          // �q�@���g�p���� MP �ʐM�Ԋu (us)

    OSTick  mp_parentIntervalTick;     // �e�@���g�p���� MP �ʐM�Ԋu (tick)
    OSTick  mp_childIntervalTick;      // �q�@���g�p���� MP �ʐM�Ԋu (tick)

    u16     mp_minFreq;                // 1�s�N�`���t���[��������� MP �񐔁i�ŏ��l�j(������)
    u16     mp_freq;                   // 1�s�N�`���t���[��������� MP �񐔁i�W���l�j
    u16     mp_maxFreq;                // 1�s�N�`���t���[��������� MP �񐔁i�ő�l�j

    u16     mp_vsyncOrderedFlag;       // ���� MP �� vsync �Ɠ������čs���悤�e�@����w�����󂯂���
    u16     mp_vsyncFlag;              // ���� MP �� vsync �Ɠ������čs����̂�
    s16     mp_count;                  // ���݂̃s�N�`���t���[���ł��Ɖ��� MP ������̂�
    s16     mp_limitCount;             // ���݂̃s�N�`���t���[���ōđ����܂߂Ă��Ɖ��� MP �����Ă������̂�
    u16     mp_resumeFlag;             // Resume ���s���t���O
    u16     mp_prevPollBitmap;         // ���O�� MP �ł� PollBitmap
    u16     mp_prevWmHeader;           // ���O�� MP �ł� wmHeader
    u16     mp_prevTxop;               // ���O�� MP �ł� txop
    u16     mp_prevDataLength;         // ���O�� MP �ł� dataLength
    u16     mp_recvBufSel;             // MP��M�o�b�t�@�̑I���t���O
    u16     mp_recvBufSize;            // MP��M�o�b�t�@�̃T�C�Y
    WMMpRecvBuf *mp_recvBuf[2];        // MP��M�o�b�t�@�̃|�C���^
    u32    *mp_sendBuf;                // MP���M�o�b�t�@�̃|�C���^
    u16     mp_sendBufSize;            // MP���M�o�b�t�@�̃T�C�Y

    u16     mp_ackTime;                // MPACK �̓����\�莞��
    u16     mp_waitAckFlag;            // MPACK ��҂��Ă���

    u16     mp_readyBitmap;            // MP �̏����������Ă���q�@�� bitmap (�e�@�p)

    u16     mp_newFrameFlag;           // �V�����s�N�`���t���[��������� TRUE �ɂȂ�t���O
    u16     mp_setDataFlag;            // �ԐM�f�[�^�Z�b�g�ς݃t���O�i�q�@�p�j
    u16     mp_sentDataFlag;           // �ԐM�f�[�^���M�ς݃t���O�i�q�@�p�j
    u16     mp_bufferEmptyFlag;        // �ԐM�f�[�^�p�o�b�t�@��t���O�i�q�@�p�j
    u16     mp_isPolledFlag;           // ���O�� MP �͎����� poll ����Ă������i�q�@�p�j

    u16     mp_minPollBmpMode;         // PollBitmap �𑗐M����p�P�b�g�̑��M��ɍ��킹�čŏ��ɂ��郂�[�h
    u16     mp_singlePacketMode;       // 1MP �� 1packet ��������Ȃ��Ȃ郂�[�h
    u8      reserved_c[2];
    u16     mp_defaultRetryCount;      // Seq ��������Ȃ��|�[�g�ł̒ʐM�̃f�t�H���g���g���C��
    u16     mp_ignoreFatalErrorMode;   // FatalError�𖳎����邩�ǂ���
    u16     mp_ignoreSizePrecheckMode; // �ʐM�J�n���̑���M�T�C�Y�̎��O�`�F�b�N�𖳌��ɂ���

    u16     mp_pingFlag;               // ping ��ł^�C�~���O�ŗ��t���O
    u16     mp_pingCounter;            // ping �܂ł̎c��t���[�����̃J�E���^

    // DCF�֌W (26Byte) 184
    u8      dcf_destAdr[WM_SIZE_MACADDR];       // DCF���M��MAC�A�h���X
    u16    *dcf_sendData;              // DCF���M�f�[�^�̃|�C���^
    u16     dcf_sendSize;              // DCF���M�f�[�^�T�C�Y
    u16     dcf_recvBufSel;            // DCF��M�o�b�t�@�̑I���t���O
    WMDcfRecvBuf *dcf_recvBuf[2];      // DCF��M�o�b�t�@�̃|�C���^
    u16     dcf_recvBufSize;           // DCF��M�o�b�t�@�̃T�C�Y

    u16     curr_tgid;                 // ���ݐڑ����Ă���e�@��TGID(BeaconRecv.Ind�Ŏg�p)

    // RSSI�֌W (4Byte) 206
    u16     linkLevel;
    u16     minRssi;
    u16     rssiCounter;

    // �ǉ��̃p�����[�^�Q
    u16     beaconIndicateFlag;        // �r�[�R���֘A�ʒm���t���O
    u16     wepKeyId;                  // WEP key ID (0�`3)
    u16     pwrMgtMode;                // �p���[�}�l�W�����g���[�h
    u32     miscFlags;                 // �ׁX�Ƃ����t���O

    u16     VSyncBitmap;               // V-Blank���������Ă���q�@�̃r�b�g�}�b�v
    u16     valarm_queuedFlag;         // V-Alarm���������s�L���[�ɐς܂�Ă���

    // V-Blank�����֌W (14Byte) 218
    u32     v_tsf;                     // V-Blank�����p V_TSF�l
    u32     v_tsf_bak;                 // V-Blank�����p �O��擾����V_TSF�l
    u32     v_remain;                  // V-Blank�����p �����c��
    u16     valarm_counter;            // V-Alarm���荞�݂̔����񐔃J�E���g�p

    // �ǂݏo���҂��t���O (2Byte) 220
//    u16             indbuf_read_wait;       // Indication�o�b�t�@�ǂݏo���҂��t���O

    u8      reserved_e[2];

    // ���g�̃X�e�[�^�X (8Byte) 228
    u8      MacAddress[WM_SIZE_MACADDR];
    u16     mode;                      // �e�@:WL_CMDLABEL_MODE_PARENT �q�@:WL_CMDLABEL_MODE_CHILD

    // �e�@�̂Ƃ��Ɏg�p������ ( 64 + 90 + 2 = 156Byte) 384
    WMParentParam pparam;              // �e�@�p�����[�^
    u8      childMacAddress[WM_NUM_MAX_CHILD][WM_SIZE_MACADDR];
    u16     child_bitmap;              // �q�@�ڑ���(poll bitmap�Ƃ��Ďg�p����)

    // �q�@�̂Ƃ��Ɏg�p������ (14Byte) 398
    WMBssDesc *pInfoBuf;               // Scan�����e�@���i�[�p�o�b�t�@�̃|�C���^(ARM9API�̈����ł��炤)
    u16     aid;                       // ���݂�AID
    u8      parentMacAddress[WM_SIZE_MACADDR];
    u16     scan_channel;              // ����Scan���̃`�����l��

    u8      reserved_f[4];

    // WEP �֌W(86Byte) 488
    u16     wepMode;                   // WEP mode (���r�b�g��WEP��)
    BOOL    wep_flag;                  // ���肪WEP��v�����Ă��邩�ǂ���������
    u16     wepKey[WM_SIZE_WEPKEY / sizeof(u16)];       // WEP key (20Byte * 4)
    /* wepKeyId�ɂ��Ă�40�s�قǏ�ɔz�u�������܂���(2005/02/07 terui) */

    // ���̑�(4Byte) 492
    u16     rate;                      // �g�prate(WL_CMDLABEL_RATE_AUTO, 1M, 2M)
    u16     preamble;                  // Long:0, Short:1

    // �e��p�����[�^ (4Byte) 496
    u16     tmptt;
    u16     retryLimit;

    // ���p�\�`�����l�� (2Byte) 498
    u16     enableChannel;

    // ���p���`�����l��(2Byte) 500
    u16     allowedChannel;

    // port �֌W ( 1316+256+20+4+256Byte ) 1816+256+20+4+256
    u16     portSeqNo[WM_NUM_MAX_CHILD + 1][WM_NUM_OF_SEQ_PORT];        // 16*8*2=256Byte

    WMPortSendQueueData sendQueueData[WM_SEND_QUEUE_NUM];       // 32*32=1024Byte
    WMPortSendQueue sendQueueFreeList; // 4Byte
    WMPortSendQueue sendQueue[WM_PRIORITY_LEVEL];       // 16Byte
    WMPortSendQueue readyQueue[WM_PRIORITY_LEVEL];      // 16Byte
    OSMutex sendQueueMutex;            // 20 byte
    BOOL    sendQueueInUse;            // MP���M�҂��t���O

    // Null Ack �^�C���A�E�g ( 128 + 8 Byte )
    OSTick  mp_lastRecvTick[1 + WM_NUM_MAX_CHILD];
    OSTick  mp_lifeTimeTick;

    // ���ݗL���� MP �ݒ�l
    // WM_StartMPEx �֐��Ŏw�肵���ǉ��p�����[�^��
    // �i�������������Ȃ��悤�ɂ��邽�߂ɁA�����̐ݒ�l��
    // WM_SetMPParameter �֐��Őݒ肳���i���I�Ȓl��
    // WM_StartMPEx �֐��Őݒ肳���ꎞ�I�Ȓl�ƁA��d�Ɏ�����Ă���
    u16     mp_current_minFreq;                // 1�s�N�`���t���[��������� MP �񐔁i�ŏ��l�j(������)
    u16     mp_current_freq;                   // 1�s�N�`���t���[��������� MP �񐔁i�W���l�j
    u16     mp_current_maxFreq;                // 1�s�N�`���t���[��������� MP �񐔁i�ő�l�j
    u16     mp_current_minPollBmpMode;         // PollBitmap �𑗐M����p�P�b�g�̑��M��ɍ��킹�čŏ��ɂ��郂�[�h
    u16     mp_current_singlePacketMode;       // 1MP �� 1packet ��������Ȃ��Ȃ郂�[�h
    u16     mp_current_defaultRetryCount;      // Seq ��������Ȃ��|�[�g�ł̒ʐM�̃f�t�H���g���g���C��
    u16     mp_current_ignoreFatalErrorMode;   // FatalError�𖳎����邩�ǂ���
    u8      reserved_g[2];
}
WMStatus, WMstatus;

//---------------------------------------
// WM ARM7���̕ϐ��̈�p�o�b�t�@�\����
typedef struct WMArm7Buf
{
    WMStatus *status;                  // WM�̃X�e�[�^�X�o�b�t�@
//    u32         *indbuf;        // Indication�o�b�t�@�̃|�C���^
    u8      reserved_a[4];
    u32    *fifo7to9;                  // FIFO(ARM7��9)�p�o�b�t�@�̃|�C���^
//    BOOL        *fifoFlag;      // ARM7��ARM9��FIFO���M���t���O
    u8      reserved_b[4];

    WMBssDesc connectPInfo;            // �ڑ���e�@���(190byte)

    // ARM7�������甭�s���郊�N�G�X�g�p�o�b�t�@
    u32     requestBuf[512 / sizeof(u32)];

}
WMArm7Buf, WMarm7Buf;

//---------------------------------------
// CallbackTable�̗v�f��
#define WM_NUM_OF_CALLBACK      ( WM_APIID_ASYNC_KIND_MAX - 2 )
// WM ARM9���̕ϐ��̈�p�o�b�t�@�\����
typedef struct WMArm9Buf
{
    // �e��|�C���^(20Byte)
    WMArm7Buf *WM7;                    // ARM7��WM�̕ϐ��̈� (ARM9����̓A�N�Z�X���Ȃ�)
    WMStatus *status;                  // WM�̃X�e�[�^�X�o�b�t�@(ARM9����̓��[�h�I�����[)
    u32    *indbuf;                    // Indication�o�b�t�@
    u32    *fifo9to7;                  // FIFO(ARM9��7)�p�o�b�t�@
    u32    *fifo7to9;                  // FIFO(ARM7��9)�p�o�b�t�@

    // ���̑�(4Byte)
    u16     dmaNo;                     // WM���g�p����DMA�ԍ�
    u16     scanOnlyFlag;              // Scan �������Ȃ����샂�[�h�t���O

    // Callback �֌W(172Byte)
    WMCallbackFunc CallbackTable[WM_NUM_OF_CALLBACK];   // �R�[���o�b�N�e�[�u��(42*4 = 168Byte)
    WMCallbackFunc indCallback;

    // port �֌W(134Byte)
    WMCallbackFunc portCallbackTable[WM_NUM_OF_PORT];   // 16*4 = 64Byte
    void   *portCallbackArgument[WM_NUM_OF_PORT];       // 16*4 = 64Byte
    u32     connectedAidBitmap;        // �ڑ���ꗗ(�r�b�g���삪�������� u32 �Ŋm��)
    u16     myAid;                     // ���݂� AID

    /* �v 20+4+172+134 Byte */

    u8      reserved1[WM_ARM9WM_BUF_SIZE -
                      (20 + 4 + 4 * WM_NUM_OF_CALLBACK + 4 + (4 + 4) * WM_NUM_OF_PORT + 6)];

}
WMArm9Buf, WMarm9Buf;

//==========================================================================================

// API�̈���
typedef struct WMStartScanReq
{
    u16     apiid;
    u16     channel;
    WMBssDesc *scanBuf;
    u16     maxChannelTime;
    u8      bssid[WM_SIZE_BSSID];

}
WMStartScanReq, WMstartScanReq;

typedef struct WMStartScanExReq
{
    u16     apiid;
    u16     channelList;
    WMBssDesc *scanBuf;
    u16     scanBufSize;
    u16     maxChannelTime;
    u8      bssid[WM_SIZE_BSSID];
    u16     scanType;
    u16     ssidLength;
    u8      ssid[WM_SIZE_SSID];
#if WM_SSID_MASK_CUSTOMIZE
    u16     ssidMatchLength;
    u16     rsv[2];
#else
    u16     rsv[3];
#endif
}
WMStartScanExReq, WMstartScanExReq;

typedef struct WMStartConnectReq
{
    u16     apiid;
    u16     reserved;
    WMBssDesc *pInfo;
    u8      ssid[WM_SIZE_CHILD_SSID];
    BOOL    powerSave;
    u16     reserved2;
    u16     authMode;

}
WMStartConnectReq, WMstartConnectReq;

typedef struct WMMeasureChannelReq
{
    u16     apiid;
    u16     ccaMode;
    u16     edThreshold;
    u16     channel;
    u16     measureTime;

}
WMMeasureChannelReq, WMmeasureChannelReq;

typedef struct WMSetMPParameterReq
{
    u16     apiid;
    u16     reserved;

    WMMPParam param;
}
WMSetMPParameterReq;

typedef struct WMStartMPReq
{
    u16     apiid;
    u16     rsv1;
    u32    *recvBuf;
    u32     recvBufSize;
    u32    *sendBuf;
    u32     sendBufSize;

    WMMPParam param; // unused
    WMMPTmpParam tmpParam;
}
WMStartMPReq;

typedef struct WMStartTestModeReq
{
    u16     apiid;
    u16     control;
    u16     signal;
    u16     rate;
    u16     channel;
}
WMStartTestModeReq, WMstartTestModeReq;

typedef struct WMStartTestRxModeReq
{
    u16     apiid;
    u16     channel;
}
WMStartTestRxModeReq, WMstartTestRxModeReq;


//==========================================================================================

// �ʏ�̃R�[���o�b�N�̈���
typedef struct WMCallback
{
    u16     apiid;
    u16     errcode;
    u16     wlCmdID;
    u16     wlResult;

}
WMCallback;

// WM_StartParent()�̃R�[���o�b�N�̈���
typedef struct WMStartParentCallback
{
    u16     apiid;
    u16     errcode;
    u16     wlCmdID;
    u16     wlResult;
    u16     state;                     // PARENT_START, BEACON_SENT, CHILD_CONNECTED, DISCONNECTED
    u8      macAddress[WM_SIZE_MACADDR];        // �ڑ������e�q�@��MAC�A�h���X
    u16     aid;
    u16     reason;                    // �ؒf����reasonCode
    u8      ssid[WM_SIZE_CHILD_SSID];  // SSID (�q�@���)
    u16     parentSize;
    u16     childSize;

}
WMStartParentCallback, WMstartParentCallback;

// WM_StartScan()�̃R�[���o�b�N�̈���
typedef struct WMStartScanCallback
{
    u16     apiid;
    u16     errcode;
    u16     wlCmdID;
    u16     wlResult;
    u16     state;                     // SCAN_START, PARENT_NOT_FOUND, PARENT_FOUND
    u8      macAddress[WM_SIZE_MACADDR];        // ���������e�@��MAC�A�h���X
    u16     channel;
    u16     linkLevel;
    u16     ssidLength;
    u16     ssid[WM_SIZE_SSID / sizeof(u16)];
    u16     gameInfoLength;
    WMGameInfo gameInfo;

}
WMStartScanCallback, WMstartScanCallback;

// WM_StartScanEx()�̃R�[���o�b�N�̈���
typedef struct WMStartScanExCallback
{
    u16     apiid;
    u16     errcode;
    u16     wlCmdID;
    u16     wlResult;
    u16     state;                     // SCAN_START, PARENT_NOT_FOUND, PARENT_FOUND
    u16     channelList;               // �����E�������Ɋւ�炸�A�X�L���������`�����l�����X�g
    u8      reserved[2];               // padding
    u16     bssDescCount;              // �������ꂽ�e�@�̐�
    WMBssDesc *bssDesc[WM_SCAN_EX_PARENT_MAX];  // �e�@���̐擪�A�h���X
    u16     linkLevel[WM_SCAN_EX_PARENT_MAX];   // ��M�d�g���x

}
WMStartScanExCallback, WMstartScanExCallback;

// WM_StartConnect()�̃R�[���o�b�N�̈���
typedef struct WMStartConnectCallback
{
    u16     apiid;
    u16     errcode;                   // PARENT_NOT_FOUND(�^�C���A�E�g)
    u16     wlCmdID;
    u16     wlResult;
    u16     state;                     // CONNECT_START, BEACON_LOST, CONNECTED, DISCONNECTED
    u16     aid;                       // CONNECTED�̂Ƃ��̂݁A���g�Ɋ��蓖�Ă�ꂽAID
    u16     reason;                    // �ؒf����reasonCode
    u16     wlStatus;
    u8      macAddress[WM_SIZE_MACADDR];
    u16     parentSize;
    u16     childSize;

}
WMStartConnectCallback, WMstartConnectCallback;

// WM_Disconnect()�y��WM_DisconnectChildren�̃R�[���o�b�N
typedef struct WMDisconnectCallback
{
    u16     apiid;
    u16     errcode;
    u16     wlCmdID;
    u16     wlResult;
    u16     tryBitmap;
    u16     disconnectedBitmap;
}
WMDisconnectCallback;

// WM_SetMPParameter()�̃R�[���o�b�N�̈���
typedef struct WMSetMPParameterCallback
{
    u16     apiid;
    u16     errcode;
    u32     mask;
    WMMPParam oldParam;
}
WMSetMPParameterCallback;

// WM_StartMP()�̃R�[���o�b�N�̈���
typedef struct WMStartMPCallback
{
    u16     apiid;
    u16     errcode;
    u16     state;
    u8      reserved[2];               // for padding 4byte align
    WMMpRecvBuf *recvBuf;              // ��M�o�b�t�@

    // �ȉ��AMPACK.Ind�̂Ƃ��̂ݎg�p
    u16     timeStamp;
    u16     rate_rssi;
    u8      destAdrs[6];
    u8      srcAdrs[6];
    u16     seqNum;
    u16     tmptt;
    u16     pollbmp;
    u16     reserved2;
}
WMStartMPCallback, WMstartMPCallback;

// WM_StartDCF()�̃R�[���o�b�N�̈���
typedef struct WMStartDCFCallback
{
    u16     apiid;
    u16     errcode;
    u16     state;
    u8      reserved[2];               // for padding 4byte align
    WMDcfRecvBuf *recvBuf;             // ��M�o�b�t�@

}
WMStartDCFCallback, WMstartDCFCallback;

// WM_MeasureChannel()�̃R�[���o�b�N�̈���
typedef struct WMMeasureChannelCallback
{
    u16     apiid;
    u16     errcode;
    u16     wlCmdID;
    u16     wlResult;
    u16     channel;
    u16     ccaBusyRatio;
}
WMMeasureChannelCallback, WMmeasureChannelCallback;

// WM_GetWirelessCounter()�̃R�[���o�b�N�̈���
typedef struct WMGetWirelessCounterCallback
{
    u16     apiid;
    u16     errcode;
    u16     wlCmdID;
    u16     wlResult;
    u32     TX_Success;                // ����ɑ��M���s������
    u32     TX_Failed;                 // ����ɑ��M���s���Ȃ�������
    u32     TX_Retry;                  // �đ����s������
    u32     TX_AckError;               // ��]����ACK�t���[������M�ł��Ȃ�������
    u32     TX_Unicast;                // ����ɑ��M���s����Unicast���t���[���̐�
    u32     TX_Multicast;              // ����ɑ��M���s����Multicast���t���[���̐�
    u32     TX_WEP;                    // ����ɑ��M���s�����Í��t���[���̐�
    u32     TX_Beacon;                 // ����ɑ��M���s����Beacon�t���[���̐�
    u32     RX_RTS;                    // (��) RTS����M����CTS�̉������s������
    u32     RX_Fragment;               // �t���O�����g���ꂽ�t���[������M������
    u32     RX_Unicast;                // Unicast���̃t���[������M������
    u32     RX_Multicast;              // Multicast���̃t���[������M������
    u32     RX_WEP;                    // (��) ����ɕ������s�����Í��t���[���̎�M��
    u32     RX_Beacon;                 // Beacon�t���[������M������
    u32     RX_FCSError;               // (��) FCS�G���[������������
    u32     RX_DuplicateError;         // (��) �d���G���[������������
    u32     RX_MPDuplicateError;       // (��) MP�t���[���̏d���G���[������������
    u32     RX_ICVError;               // (��) ICV�G���[������������
    u32     RX_FrameCtrlError;         // (��) �s����FrameCtrl�����t���[������M������
    u32     RX_LengthError;            // (��) �K��̒������Z�����邢�͒����t���[������M������
    u32     RX_PLCPError;              // (��) PLCP��CRC�G���[������������
    u32     RX_BufferOverflowError;    // (��) ��M�o�b�t�@���I�[�o�[�t���[������
    u32     RX_PathError;              // (��) ��M�u���b�N�����̃G���[������������
    u32     RX_RateError;              // (��) ��M���[�g���s���ȃt���[������M������
    u32     RX_FCSOK;                  // (��) �����FCS�����t���[������M������
    u32     TX_MP;                     // MP�t���[���𑗐M������
    u32     TX_KeyData;                // MP-KeyData�t���[���𑗐M������
    u32     TX_NullKey;                // MP-NullKey�t���[���𑗐M������
    u32     RX_MP;                     // �����MP�t���[������M������
    u32     RX_MPACK;                  // �����MPACK�t���[������M������
    u32     MPKeyResponseError[15];    // Key�v���ɑ΂��Đ���ɉ������Ȃ�������
    /*
     * (��) �����n�[�h�E�F�A���̃��W�X�^���s���ɃN���A����邱�Ƃ����邽�߁A
     * �n�[�h�E�F�A�̃J�E���g���W�X�^�����Z���Ă����\�t�g�J�E���^�͎��ۂ̍��v�l��
     * �����Ȃ��ꍇ������B(��)�̂��Ă���J�E���^�ɂ̂݉e��������B
     */
}
WMGetWirelessCounterCallback, WMgetWirelessCounterCallback;

// Indication�R�[���o�b�N�̈���
typedef struct WMIndCallback
{
    u16     apiid;
    u16     errcode;
    u16     state;
    u16     reason;

}
WMIndCallback, WMindCallback;

// MP port �ʐM���M�R�[���o�b�N�̈���
typedef struct WMPortSendCallback
{
    u16     apiid;
    u16     errcode;
    u16     wlCmdID;
    u16     wlResult;
    u16     state;
    u16     port;
    u16     destBitmap;
    u16     restBitmap;
    u16     sentBitmap;
    u16     rsv;
    const u16 *data;
    union
    {
        u16     size;
        u16     length;
    };
    u16     seqNo;
    WMCallbackFunc callback;
    void   *arg;
    u16     maxSendDataSize;
    u16     maxRecvDataSize;

}
WMPortSendCallback;

// MP port �ʐM��M�R�[���o�b�N�̈���
typedef struct WMPortRecvCallback
{
    u16     apiid;
    u16     errcode;
    u16     state;
    u16     port;                      // �|�[�g�ԍ�
    WMMpRecvBuf *recvBuf;              // ��M�o�b�t�@ // WMStartMPCallback �Ƃ̌݊����̂���
    u16    *data;                      // ��M�f�[�^ (port ���Ă̎�M�f�[�^�͂�������Q�Ƃ��邱��)
    u16     length;                    // ��M�f�[�^��
    u16     aid;                       // ���M�� AID
    u8      macAddress[WM_SIZE_MACADDR];
    u16     seqNo;                     // seqNo
    void   *arg;                       // callback �ɓn������ (WM9 ���ŒǋL)
    u16     myAid;                     // ������ AID
    u16     connectedAidBitmap;        // �ڑ��� AID Bitmap
    u8      ssid[WM_SIZE_CHILD_SSID];  // SSID (�q�@���)
    u16     reason;                    // �ؒf����reasonCode
    u16     rsv;                       // reserved
    u16     maxSendDataSize;           // ���݂̑��M�\�T�C�Y
    u16     maxRecvDataSize;           // ���݂̎�M�\�T�C�Y

}
WMPortRecvCallback;

// BeaconRecv.Ind�R�[���o�b�N�̈���
typedef struct WMBeaconRecvIndCallback
{
    u16     apiid;
    u16     errcode;
    u16     state;
    u16     tgid;
    u16     wmstate;
    u16     gameInfoLength;
    WMGameInfo gameInfo;
}
WMBeaconRecvIndCallback;

// WM_StartTestMode()�̃R�[���o�b�N�̈���
typedef struct WMStartTestModeCallback
{
    u16     apiid;
    u16     errcode;
    u32     RFadr5;
    u32     RFadr6;
    u16     PllLockCheck;
    u16     RFMDflag;

}
WMStartTestModeCallback;

// WM_StopTestRxMode()�̃R�[���o�b�N�̈���
typedef struct WMStopTestRxModeCallback
{
    u16     apiid;
    u16     errcode;
    u32     fcsOk;
    u32     fcsErr;
}
WMStopTestRxModeCallback;

/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* NITRO_WM_COMMON_WM_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/

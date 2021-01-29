/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - dwc_match
  File:     dwc_match.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_match.h,v $
  Revision 1.62  2007/08/24 10:12:42  takayama
  NNThink������Ăяo����Ă���DWCi_NNCompletedCallback����
  NNFreeNegotiateList���Ăяo���Ă����o�O���C���B

  Revision 1.61  2006/12/15 07:06:26  nakata
  ServerBrowserLimitUpdate�̃^�C���A�E�g���Ԃ����΂���
  ���̂܂܂��ƕ��ʂɃ^�C���A�E�g�Ƀq�b�g����ꍇ�����邽��

  Revision 1.60  2006/09/26 11:22:42  nakata
  ServerBrowserLimitUpdate�̃^�C���A�E�g���o�s�\�Ȗ����C��

  Revision 1.59  2006/02/07 00:16:36  arakit
  �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o���ߐ؂�֐�DWC_StopGameServer()���쐬�����B

  Revision 1.58  2006/01/26 05:33:15  arakit
  �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o���ߐ؂�֐�DWC_StopGameServer()���쐬�����B

  Revision 1.57  2006/01/25 07:52:14  arakit
  �E�}�b�`���C�N�����������ɐe�@���ؒf����ƁA�q�@���͂܂��ԂɂȂ��Ă��܂��s����C��
  �E�F�B�w��s�A�}�b�`���C�N�ɂ����āAdistantFriend = FALSE �̎��͐�΂Ɏw��̗F�B
  �@�ȊO�Ƃ͐ڑ����Ȃ��悤�ɏC�������B

  Revision 1.56  2006/01/23 04:50:43  arakit
  ����p��ɍ��킹�ăR�����g���C��

  Revision 1.55  2005/11/29 09:14:19  arakit
  ServerBrowserLimitUpdate()�Ŏ擾����ő�T�[�o����6�Ɍ��炵���B

  Revision 1.54  2005/11/04 11:37:00  arakit
  �֐������R�����g���C�������B

  Revision 1.53  2005/11/02 02:49:32  arakit
  �t�@�C���̃C���N���[�h�֌W�𒲐������B

  Revision 1.52  2005/10/28 09:09:58  arakit
  �EDWCi_GetCurrentTime()�̍폜�ɔ����A���Ԃ�S��OSTick�Ŏ��悤�ɕύX�����B
  �E[todo]�̃R�����g�������폜�A�������͒��������B

  Revision 1.51  2005/10/05 08:28:50  arakit
  �T�[�o�N���C�A���g�}�b�`���O���ɁA�}�b�`���O������A�}�b�`���O���u���b�N���A�u���b�N���ꂽ
  �N���C�A���g�ɁA��p�̃G���[��Ԃ��}�b�`���O�I�v�V������ǉ������B

  Revision 1.50  2005/10/05 06:50:52  arakit
  �F�B�w��}�b�`���O�ŁA�o�f�B�����ς݂̃o�f�B�����Ȃ��Ă��A��������ł�
  �F�B�s���̃G���[���o���Ȃ��悤�ɂ����B

  Revision 1.49  2005/10/04 09:02:14  arakit
  �}�b�`���O���������������ɃN���[�Y�A�d��OFF����Ă����������Ƃ��悤�ύX�����B

  Revision 1.48  2005/10/04 06:42:33  sasakit
  SBUpdate�p�����[�^�̃`���[�j���O

  Revision 1.47  2005/10/04 06:28:00  sasakit
  SBUpdate����^�C�~���O�̒����B

  Revision 1.46  2005/10/03 12:21:19  arakit
  �T�[�o�N���C�A���g�^�}�b�`���O�̃N���C�A���g�\�񎞂ɁA�T�[�o����̖������^�C���A�E�g��
  �������Ă��A���񐔃��g���C����悤�ɂ����B

  Revision 1.45  2005/10/03 05:06:11  arakit
  QR2�N���֐��̖��O��DWC_RegisterMatchingStatus�ɕύX�����B

  Revision 1.43  2005/10/01 14:25:45  arakit
  �^�C���A�E�g�����̕s����C�������B

  Revision 1.42  2005/10/01 10:32:24  sasakit
  ���Timeout��msec�I�[�_�[�ɏC���B

  Revision 1.41  2005/10/01 07:16:09  arakit
  �T�[�o�N���C�A���g�^�}�b�`���O���̃L�����Z�����������������쐬�����B

  Revision 1.40  2005/09/30 02:50:07  arakit
  �X�C�b�`�萔DWC_CLOSE_CONNECTION_NEW��p�~�����B

  Revision 1.39  2005/09/28 12:42:20  arakit
  �E�P��̃}�b�`���O�łT��NAT�l�S�V�G�[�V�����Ɏ��s������G���[�ɂȂ�悤�ɂ����B
  �ENAT�l�S�V�G�[�V�����̃N�b�L�[���A���16bit�������_���l�A����16bit���v���t�@�C��ID��
  �@����16bit�ɕύX�����B

  Revision 1.38  2005/09/27 13:02:15  arakit
  �q�@���m�̐ڑ������҂����Ԃ́A�Œ�ł�NN���g���C�������͑҂悤�ɉ��΂����B

  Revision 1.37  2005/09/27 05:14:25  arakit
  �}�b�`���O���������ʐM�̃f�[�^�T�C�Y��萔�ɂ����B

  Revision 1.36  2005/09/26 15:16:09  sasakit
  NAT Negotiation�̃��g���C�񐔂����炵���B

  Revision 1.35  2005/09/26 11:06:26  arakit
  QR2�L�[��numplayers��253�Ȃǂُ̈�Ȓl�ɂȂ��Ă��܂��s����C�������B

  Revision 1.34  2005/09/26 05:06:08  arakit
  �ESB�A�b�v�f�[�g�p��addFilter��������A���C�u���������ɃR�s�[���Ď��悤�ɕύX�����B
  �EDWC_AddMatchKey*()��key���̕�������A���C�u���������ɃR�s�[���Ď��悤�ɕύX�����B

  Revision 1.33  2005/09/24 12:56:30  arakit
  �T�[�o�N���C�A���g�^�}�b�`���O���ɁA�}�b�`���O���������z�X�g�݂̂�L����AID�Ƃ���
  �����悤�AAID��R�l�N�V�������ւ̃A�N�Z�X�֐����C�������B

  Revision 1.32  2005/09/23 04:30:09  arakit
  ���������z�X�g�̃^�C���A�E�g�������C���A�ǉ������B

  Revision 1.31  2005/09/21 07:13:14  arakit
  gt2Connect�҂��̃^�C���A�E�g��ݒ肵���B

  Revision 1.30  2005/09/21 06:06:45  arakit
  gt2Connect�̃��g���C�񐔃I�[�o���Ƀ}�b�`���O����蒼���悤�ɂ����B

  Revision 1.29  2005/09/21 05:21:26  arakit
  �E�L�����Z�����܂߂��N���[�Y�����̃V�X�e����傫���ύX�����B
  �E�R�}���h���M�̃��g���C�A�^�C���A�E�g�������쐬�����B
  �ENAT�l�S�V�G�[�V�������s���̃}�b�`���O��蒼��������ǉ������B
  �Egt2�̃\�t�g�N���[�Y��S�ăn�[�h�N���[�Y�ɒu���������B
  �@����ɔ���DWCi_CloseConnectionsAsync()���g��Ȃ��悤�ɂ����B
  �i���j�T�[�o�N���C�A���g�^�}�b�`���O�͐��퓮�삵�Ȃ�

  Revision 1.28  2005/09/19 13:51:24  sasakit
  RESV_WAIT��M���̍đ�������ύX�����B

  Revision 1.27  2005/09/15 13:02:06  arakit
  �F�B�w��}�b�`���O�̌Â��N���C�A���gQR2�f�[�^�������ă��b�Z�[�W�𑗂��Ă��܂��̂���������B

  Revision 1.26  2005/09/15 09:44:32  arakit
  �ESB���b�Z�[�W�R�}���h�đ��������ꕔ���������B
  �E�T�[�o�ɏオ����������QR2�f�[�^����萳�m�ɔ��肷��悤�ɂ����B

  Revision 1.25  2005/09/15 06:07:41  arakit
  �w��l���ȉ��}�b�`���O�����I�v�V�����̏�Ԃ��擾�ł���֐�
  DWC_GetMOMinCompState(u64* time)��ǉ������B

  Revision 1.24  2005/09/14 10:07:49  arakit
  �w��l���ȉ��ł̃}�b�`���O�����I�v�V�����ݒ�@�\��ǉ������B

  Revision 1.23  2005/09/13 11:11:30  arakit
  �v���O��������

  Revision 1.22  2005/09/10 09:02:57  arakit
  �f�o�b�O�p�̃}�b�`���O������ԕ\���֐�DWC_GetMatchingState()��ǉ������B

  Revision 1.21  2005/09/09 09:54:09  arakit
  �o�[�W�����Ⴂ�̃��C�u�������m�Ń}�b�`���O���Ȃ��悤�ɕύX�����B

  Revision 1.20  2005/09/08 14:41:26  arakit
  �E�F�B�w��}�b�`���O�ŁA�F�B��1�l���}�b�`���O�J�n���Ă��Ȃ��ꍇ�́A�\��R�}���h
  �@�đ��Ԋu��Z�������B
  �E�x��Ď�M�����\��R�}���h�ɔ������Ă��܂��s����C�������B

  Revision 1.19  2005/09/08 08:50:24  arakit
  �E���O�C������gt2Socket��QR2���N������悤�ɕύX
  �Eaid���d�����Ĕ��s�����s����C��

  Revision 1.18  2005/09/08 02:48:34  arakit
  �ڑ��ςݎq�@�����T�[�o�́A���T�[�o�����i�������͗\��j�J�n��x�点�A
  �ł��邾�����N���C�A���g����̐ڑ���҂悤�ɕύX�B

  Revision 1.17  2005/09/07 12:43:34  arakit
  �F�B���w��}�b�`���O�̎d�l��ύX���A�}�b�`���O�������������B

  Revision 1.16  2005/09/05 11:44:29  arakit
  �EDWCConnectionClosedCallback�̈���isServer�́A�������T�[�o�N���C�A���g�^
  �@�}�b�`���O�̃N���C�A���g�ŁA�T�[�o���N���[�Y�������̂�TRUE��Ԃ��悤��
  �@���Ă������A����킵���̂ŁA�������T�[�o�Ŏ������N���[�Y���Ă�TRUE��
  �@�Ԃ��悤�ɕύX�����B
  �E�T�[�o�N���C�A���g�^�}�b�`���O�ŁA�N���C�A���g�����Ȃ��Ȃ�������
  �@�T�[�o�̏����ɕs����������̂ŁA�C�������B
  �E�T�[�o�N���C�A���g�^�ŁA�N���[�Y�A�L�����Z�����qr2_shutdown()���Ă΂��
  �@���Ȃ������̂��C�������B
  �E�Ō�̃}�b�`���O�^�C�v�擾�֐�DWC_GetLastMatchingType()��ǉ������B

  Revision 1.15  2005/09/03 13:03:53  arakit
  GameSpy�v���t�@�C��ID�������ɂƂ�悤�ɂȂ��Ă����R�[���o�b�N�A
  DWCMatchedSCCallback, DWCNewClientCallback, DWCConnectionClosedCallback
  �ɂ��āA�F�B���X�g�C���f�b�N�X��Ԃ��悤�ɕύX�����B

  Revision 1.14  2005/09/01 12:15:00  arakit
  �}�b�`���O�������̓�������������ǉ������B

  Revision 1.13  2005/08/31 02:10:07  arakit
  �E�F�B�w��}�b�`���O�ŁA�F�B�̗F�B�������邩�ǂ�����I���ł���悤�ɂ����B
  �E�F�B���w��}�b�`���O�̏ꍇ�A�N���C�A���g��Ԃ������_�����Ԉێ�����悤�ɂ����B

  Revision 1.12  2005/08/29 12:28:15  arakit
  �E�F�B���w��}�b�`���O�ŁA�T�[�o�ɂȂ����z�X�g���A�^�C���A�E�g�ł܂��N���C�A���g�ɖ߂�A
  �T�[�o�̌�������蒼���悤�ύX�����B
  �E�F�B���w��}�b�`���O���ɁA���ɑ��̃T�[�o�ɐڑ������N���C�A���g��NN�\���
  ���M���Ă��܂��s����C�������B

  Revision 1.11  2005/08/29 06:37:17  arakit
  �E�F�B�w��}�b�`���O�ŁASB�̃^�C���A�E�g����������s����C�������B
  �E�F�B�w��}�b�`���O�Őڑ��l���w�肪���f����Ă��Ȃ������̂��C�������B
  �E�T�[�o�N���C�A���g�^�}�b�`���O�ŁA�ő�ڑ��l���A���ݐڑ��l����gpStatus��
  locationString�ɃZ�b�g����悤�ɂ��A���̒l���Q�Ƃ��邽�߂̊֐���p�ӂ����B

  Revision 1.10  2005/08/26 08:15:23  arakit
  �EASSERT�ƃ��W���[������ϐ��̃`�F�b�N�𐮗������B
  �E�O���֐�DWC_ShutdownFriendsMatch()�̈������Ƃ�Ȃ��悤�ɕύX�����B
  �܂��A���֐�����e���W���[���̐���ϐ���NULL�N���A����悤�ɂ����B
  �E�G���[�����֐���ύX���A�G���[�Z�b�g�A�G���[�`�F�b�N��K�v�ȉӏ��ɓ��ꂽ�B

  Revision 1.9  2005/08/24 07:12:07  arakit
  qr2�R�[���o�b�N������qr2_shutdown()���Ă΂Ȃ��悤�C�������B

  Revision 1.8  2005/08/23 13:54:52  arakit
  �EGP�o�f�B���b�Z�[�W�A�y��SB���b�Z�[�W�ɂ��ʐM�R�}���h�đ��Ԋu���A�R�O�Q�[���t���[��
  ����A�U�O�Q�[���t���[���ɕύX�����B
  �E�}�b�`���O�����E�L�����Z����A�Ăу}�b�`���O���s������QR2�G���[����������o�O���C�������B

  Revision 1.7  2005/08/20 10:28:07  sasakit
  �s����\���Ă���s�̑΍�B

  Revision 1.6  2005/08/20 07:01:20  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_MATCH_H_
#define DWC_MATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
// �T�[�o�N���C�A���g�}�b�`���C�N�ȊO�ł̓}�b�`���C�N���I�������QR2���I������d�l
#define DWC_QR2_ALIVE_DURING_MATCHING

// �F�B���X�g�ɗF�B�����Ȃ��Ă��ꍇ�ɂ���Ă̓}�b�`���C�N�J�n��OK����
#define DWC_MATCH_ACCEPT_NO_FRIEND

// �F�B����̗F�B�w��s�A�}�b�`���C�N��L���ɂ���
#define DWC_LIMIT_FRIENDS_MATCH_VALID

// �T�[�o�N���C�A���g�}�b�`���C�N�̓r�����ߐ؂��L���ɂ���
#define DWC_STOP_SC_SERVER
    
// �}�b�`���C�N�̃v���g�R���o�[�W�����B�o�[�W�����̈Ⴄ�}�b�`���C�N�R�}���h��
// �T�[�o�Ƃ͒ʐM���s��Ȃ��悤�ɂ��邽�߂Ɏg���B
#define DWC_MATCHING_VERSION 3U
    
// gt2�̕W���ɏ����iAID�r�b�g�}�b�v�̊֌W�����32�ȏ�ɂ͂ł��Ȃ��j
#define DWC_MAX_CONNECTIONS 32

// QR2�J�X�^���L�[�̃C���f�b�N�X�iNUM_RESERVED_KEYS = 50�j
// 50�`99��DWC�̗\��L�[�A100�`253���Q�[���Ŏg�p�ł���L�[�Ƃ���
#define DWC_QR2_PID_KEY            NUM_RESERVED_KEYS
#define DWC_QR2_MATCH_TYPE_KEY     (NUM_RESERVED_KEYS+1)
#define DWC_QR2_MATCH_RESV_KEY     (NUM_RESERVED_KEYS+2)
#define DWC_QR2_MATCH_VER_KEY      (NUM_RESERVED_KEYS+3)
#define DWC_QR2_MATCH_EVAL_KEY     (NUM_RESERVED_KEYS+4)

// �e�L�[�̕�����
#define DWC_QR2_PID_KEY_STR        "dwc_pid"
#define DWC_QR2_MATCH_TYPE_KEY_STR "dwc_mtype"
#define DWC_QR2_MATCH_RESV_KEY_STR "dwc_mresv"
#define DWC_QR2_MATCH_VER_KEY_STR  "dwc_mver"
#define DWC_QR2_MATCH_EVAL_KEY_STR "dwc_eval"

// DWC�̗\��L�[���i50�j
#define DWC_QR2_RESERVED_KEYS  (100-NUM_RESERVED_KEYS)
    
// �Q�[���ōő�g�p�ł���L�[�̐��i153�j
#define DWC_QR2_GAME_RESERVED_KEYS (MAX_REGISTERED_KEYS-NUM_RESERVED_KEYS-DWC_QR2_RESERVED_KEYS)
    
// �Q�[����`�L�[�̊J�n�l
#define DWC_QR2_GAME_KEY_START 100


// SB���b�Z�[�W�A�y��GP�o�f�B���b�Z�[�W�̃}�b�`���C�N�p�R�}���h
#define DWC_SB_COMMAND_STRING "SBCM"  // SB���b�Z�[�W�R�}���h���ʕ�����
    
#define DWC_MATCH_COMMAND_RESERVATION    0x01  // NAT�l�S�V�G�[�V�����\��v��
#define DWC_MATCH_COMMAND_RESV_OK        0x02  // NAT�l�S�V�G�[�V�����\�񊮗�
#define DWC_MATCH_COMMAND_RESV_DENY      0x03  // NAT�l�S�V�G�[�V�����\�񋑔�
#define DWC_MATCH_COMMAND_RESV_WAIT      0x04  // NAT�l�S�V�G�[�V�����\��҂��v��
#define DWC_MATCH_COMMAND_RESV_CANCEL    0x05  // NAT�l�S�V�G�[�V�����\������v��
#define DWC_MATCH_COMMAND_TELL_ADDR      0x06  // NN�J�n���A�h���X�ʒm�iNAT�l�S�V�G�[�V�����s�v�j
#define DWC_MATCH_COMMAND_NEW_PID_AID    0x07  // �V�K�ڑ��N���C�A���g�̃v���t�@�C��ID��AID�ʒm
#define DWC_MATCH_COMMAND_LINK_CLS_REQ   0x08  // �N���C�A���g���m�̐ڑ��v��
#define DWC_MATCH_COMMAND_LINK_CLS_SUC   0x09  // �N���C�A���g���m�̐ڑ������ʒm
#define DWC_MATCH_COMMAND_CLOSE_LINK     0x0a  // �ڑ��ς݂̃R�l�N�V�����N���[�Y�ʒm
#define DWC_MATCH_COMMAND_RESV_PRIOR     0x0b  // NAT�l�S�V�G�[�V�����D��\��v��
#define DWC_MATCH_COMMAND_CANCEL         0x0c  // �}�b�`���C�N�L�����Z���R�}���h
#define DWC_MATCH_COMMAND_CANCEL_SYN     0x0d  // �}�b�`���C�N�L�����Z�������pSYN
#define DWC_MATCH_COMMAND_CANCEL_SYN_ACK 0x0e  // �}�b�`���C�N�L�����Z�������pSYN-ACK
#define DWC_MATCH_COMMAND_CANCEL_ACK     0x0f  // �}�b�`���C�N�L�����Z�������pACK
#define DWC_MATCH_COMMAND_SC_CLOSE_CL    0x10  // �ڑ��ς݃N���C�A���g�̈Ӑ}���Ȃ��ؒf�ʒm
#define DWC_MATCH_COMMAND_POLL_TIMEOUT   0x11  // OPTION_MIN_COMPLETE�̃^�C���A�E�g�|�[�����O
#define DWC_MATCH_COMMAND_POLL_TO_ACK    0x12  // OPTION_MIN_COMPLETE�̃^�C���A�E�g�|�[�����O�ւ�ACK
#define DWC_MATCH_COMMAND_SC_CONN_BLOCK  0x13  // OPTION_SC_CONNECT_BLOCK�̃u���b�N�ɂ��\�񋑔�
#ifdef DWC_LIMIT_FRIENDS_MATCH_VALID
#define DWC_MATCH_COMMAND_FRIEND_ACCEPT  0x20  // �N���C�A���g����̐V�K�ڑ��F�B����
#endif
#define DWC_MATCH_COMMAND_CL_WAIT_POLL   0x40  // �N���C�A���g�҂���ԃ^�C���A�E�g�|�[�����O
#define DWC_MATCH_COMMAND_SV_KA_TO_CL    0x41  // ��L�|�[�����O�ɑ΂���T�[�o�̃L�[�v�A���C�u
#define DWC_MATCH_COMMAND_DUMMY          0xff  // �_�~�[�R�}���h

#if 0
#define DWC_MATCH_CMD_RESEND_INTERVAL     60    // �}�b�`���C�N�R�}���h�đ��Ԋu
#define DWC_MATCH_CMD_RESEND_INTERVAL_ADD 150   // �}�b�`���C�N�R�}���h�đ��Ԋu�ǉ����ԃx�[�X
#define DWC_MATCH_CMD_RESV_TIMEOUT        480   // NN�\��R�}���h�����^�C���A�E�g
#define DWC_SB_UPDATE_INTERVAL            60    // SB�T�[�o�A�b�v�f�[�g�Ԋu
#define DWC_SB_UPDATE_INTERVAL_ADD        180   // SB�T�[�o�A�b�v�f�[�g�Ԋu�ǉ����ԃx�[�X
#define DWC_QR2_KEEP_RESV_TIMEOUT         600   // QR2�\��ێ��^�C���A�E�g����
#else
#define DWC_MATCH_CMD_RESEND_INTERVAL_MSEC     3000   // �}�b�`���C�N�R�}���h�đ��Ԋu
#define DWC_MATCH_CMD_RESEND_INTERVAL_ADD_MSEC 3000   // �}�b�`���C�N�R�}���h�đ��Ԋu�ǉ����ԃx�[�X
#define DWC_MATCH_CMD_RESV_TIMEOUT_MSEC        6000   // NN�\��R�}���h�����^�C���A�E�g
#define DWC_SB_UPDATE_INTERVAL_MSEC            3000   // SB�T�[�o�A�b�v�f�[�g�Ԋu
#define DWC_SB_UPDATE_INTERVAL_SHORT_MSEC      1000   // SB�T�[�o�A�b�v�f�[�g�Ԋu
#define DWC_SB_UPDATE_INTERVAL_ADD_MSEC        3000   // SB�T�[�o�A�b�v�f�[�g�Ԋu�ǉ����ԃx�[�X
#define DWC_SB_UPDATE_INTERVAL_SHORT           1
#define DWC_SB_UPDATE_INTERVAL_LONG            2
#define DWC_SB_UPDATE_TIMEOUT                  30000  // SB�T�[�o�A�b�v�f�[�g�������^�C���A�E�g����
#endif

// �R�}���h�𑗂��Ă����ɏ������Ă܂��R�}���h���Ԃ���鏈���̏ꍇ��
// �ėp�I�Ɏg���A�R�}���h�����^�C���A�E�g����
#define DWC_MATCH_CMD_RTT_TIMEOUT          6000

// SB�T�[�o�A�b�v�f�[�g���s���Ă���A�ړI�̃T�[�o�f�[�^��������܂ł̏�����
// �ėp�I�Ɏg���^�C���A�E�g���ԁi�����Q�񕪂�z��j
#define DWC_MATCH_SB_UPDATE_TIMEOUT        13000

// �}�b�`���C�N�R�}���h�̕W���I�ȍő�đ���
#define DWC_MATCH_CMD_RETRY_MAX            5

// �}�b�`���C�N�������������p�^�C���A�E�g����
#define DWC_MATCH_SYN_ACK_WAIT_TIME        DWC_MATCH_CMD_RTT_TIMEOUT

// �T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�����������p�^�C���A�E�g����
#define DWC_MATCH_CANCEL_SYN_ACK_WAIT_TIME DWC_MATCH_CMD_RTT_TIMEOUT

// �F�B���w��s�A�}�b�`���C�N�̗\��ێ��^�C���A�E�g����
#define DWC_MATCH_RESV_KEEP_TIME_ANYBODY   DWC_MATCH_CMD_RTT_TIMEOUT
// �F�B�w��A�T�[�o�N���C�A���g�}�b�`���C�N�̗\��ێ��^�C���A�E�g����
// �i�\��OK�R�}���h���M�{SB�T�[�o�A�b�v�f�[�g�{NN�J�n���Ԃ��l���j
#define DWC_MATCH_RESV_KEEP_TIME_FRIEND    (DWC_MATCH_CMD_RTT_TIMEOUT+DWC_MATCH_SB_UPDATE_TIMEOUT)

#define DWC_MAX_MATCH_NN_RETRY  1   // NAT�l�S�V�G�[�V�������g���C�ő��
#define DWC_MATCH_NN_FAILURE_MAX 5  // NAT�l�S�V�G�[�V�������s���e�ő�񐔁i������z����ƃG���[�I���j

// NAT�l�S�V�G�[�V�������s�ナ�g���C�҂��^�C���A�E�g����
#define DWC_WAIT_NN_RETRY_TIMEOUT          10000

// gt2Connect�̃^�C���A�E�g����
#define DWC_GT2_CONNECT_TIMEOUT            5000

// NAT�l�S�V�G�[�V����������Agt2Connect��M�҂��^�C���A�E�g����
#define DWC_WAIT_GT2_CONNECT_TIMEOUT       (DWC_GT2_CONNECT_TIMEOUT*DWC_MATCH_CMD_RETRY_MAX)

// �N���C�A���g���m��1��1�̐ڑ������^�C���A�E�g���ԁi�T�[�o���g���j
// ��CLS_REQ�R�}���h�̍đ��͍s�킸�A�^�C���A�E�g���ԂɂȂ����瑦�^�C���A�E�g
// �������s���悤�ɂȂ����̂ŁA
// PARTNER_WAIT_TIME�i10msec�j�~DWC_WAIT_NN_RETRY_TIMEOUT�{���̎��Ԃ�
// �҂���NN�̃��g���C��҂K�v������B
#define DWC_MATCH_LINK_CLS_TIMEOUT         30000

// �N���C�A���g�̑҂���Ԏ��ɃT�[�o����̃f�[�^��M���ȉ��̎��ԂȂ���΁A
// �^�C���A�E�g�Ɣ��f���ăT�[�o�ւ̃|�[�����O���s��
#define DWC_MATCH_CL_WAIT_TIMEOUT          30000

// RESV_WAIT��M��\��R�}���h�ő�đ��񐔁i�ŏ��̂P��͏������đ��݂̂̉񐔁j
#define DWC_RESV_COMMAND_RETRY_MAX 16

// OPTION_MIN_COMPLETE�ŁA�^�C���A�E�g�|�[�����O�����g���C�����
// �i�ŏ��̂P��͏������đ��݂̂̉񐔁j
#define DWC_MATCH_OPT_MIN_COMP_POLL_RETRY_MAX 5

// ServerBrowserLimitUpdate()�ɓn���A�����T�[�o�ő吔
#define DWC_SB_UPDATE_MAX_SERVERS 6

// �}�b�`���C�N���������p�P�b�g�́AReliable���M�f�[�^�{�̃T�C�Y
#define DWC_MATCH_SYN_DATA_BODY_SIZE 4

// [todo]
// �}�b�`���C�N�v���F�B�C���f�b�N�X���X�g���B
// ���ɒ萔�Őݒ肷�邪�A�{���̓Q�[��������w�肳�ꂽ�傫�������g�p����
#define DWC_MAX_MATCH_IDX_LIST 64

    
//----------------------------------------------------------------------------
// enum
//----------------------------------------------------------------------------
// �}�b�`���C�N�I�v�V�����^�C�v
typedef enum {
    DWC_MATCH_OPTION_MIN_COMPLETE = 0,  // �F�B�w��E���w��s�A�}�b�`���C�N�ŁA
                                        // �w��l���ȉ��ł��}�b�`���C�N����������
    DWC_MATCH_OPTION_SC_CONNECT_BLOCK,  // �T�[�o�N���C�A���g�}�b�`���C�N�ŁA
                                        // �P��}�b�`���C�N������ɑ��̐ڑ����󂯕t���Ȃ�����
    DWC_MATCH_OPTION_NUM
} DWCMatchOptType;

// DWC_SetMatchingOption()�̌��ʌ^
enum {
    DWC_SET_MATCH_OPT_RESULT_SUCCESS = 0,  // �G���[�Ȃ�
    DWC_SET_MATCH_OPT_RESULT_E_BAD_STATE,  // FriendsMatch���C�u�����񓮍쒆
    DWC_SET_MATCH_OPT_RESULT_E_INVALID,    // �����ȃI�v�V�����w��
    DWC_SET_MATCH_OPT_RESULT_E_PARAM,      // �p�����[�^�G���[
    DWC_SET_MATCH_OPT_RESULT_E_ALLOC,      // Alloc�G���[
    DWC_SET_MATCH_OPT_RESULT_NUM
};


// �}�b�`���C�N��ԗ񋓎q
// [todo]
// �Q�[���������C�u�������Q�Ƃ��Ȃ�enum��dwc_match.c�Ɉڂ��i���define���j
typedef enum {
    DWC_MATCH_STATE_INIT = 0,           // �������

    // �N���C�A���g���̏��
    DWC_MATCH_STATE_CL_WAITING,         // �҂����
    DWC_MATCH_STATE_CL_SEARCH_OWN,      // ���z�X�g��񌟍���
    DWC_MATCH_STATE_CL_SEARCH_HOST,     // �󂫃z�X�g������ԁi�F�B���w��̎��̂݁j
    DWC_MATCH_STATE_CL_WAIT_RESV,       // �\��ɑ΂���T�[�o����̕ԓ��҂�
    DWC_MATCH_STATE_CL_SEARCH_NN_HOST,  // �\�񂪊�����������z�X�g��������
    DWC_MATCH_STATE_CL_NN,              // NAT�l�S�V�G�[�V������
    DWC_MATCH_STATE_CL_GT2,             // GT2�R�l�N�V�����m����
    DWC_MATCH_STATE_CL_CANCEL_SYN,      // �T�[�o�N���C�A���g�Ń}�b�`���C�N�L�����Z������������
    DWC_MATCH_STATE_CL_SYN,             // �}�b�`���C�N��������������
        
    // �T�[�o���̏��
    DWC_MATCH_STATE_SV_WAITING,         // �҂����
    DWC_MATCH_STATE_SV_OWN_NN,          // �N���C�A���g�Ƃ�NAT�l�S�V�G�[�V������
    DWC_MATCH_STATE_SV_OWN_GT2,         // �N���C�A���g�Ƃ�GT2�R�l�N�V�����m����
    DWC_MATCH_STATE_SV_WAIT_CL_LINK,    // �N���C�A���g���m�̐ڑ������҂�
    DWC_MATCH_STATE_SV_CANCEL_SYN,      // �T�[�o�N���C�A���g�Ń}�b�`���C�N�L�����Z����������SYN-ACK�҂�
    DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT, // �T�[�o�N���C�A���g�Ń}�b�`���C�N�L�����Z�����������I���҂�
    DWC_MATCH_STATE_SV_SYN,             // �}�b�`���C�N�I����������SYN-ACK�҂�
    DWC_MATCH_STATE_SV_SYN_WAIT,        // �}�b�`���C�N�I�����������I���҂�

    // ���ʏ��
    DWC_MATCH_STATE_WAIT_CLOSE,         // �R�l�N�V�����N���[�Y�����҂�

    // �}�b�`���C�N�I�v�V�����g�p���݂̂̏��
    DWC_MATCH_STATE_SV_POLL_TIMEOUT,    // �T�[�o��OPTION_MIN_COMPLETE�̃^�C���A�E�g���|�[�����O��

    DWC_MATCH_STATE_NUM
} DWCMatchState;

// �}�b�`���C�N�^�C�v�񋓎q
enum {
    DWC_MATCH_TYPE_ANYBODY = 0,  // �F�B���w��s�A�}�b�`���C�N
    DWC_MATCH_TYPE_FRIEND,       // �F�B�w��s�A�Ԃ��������C�N
    DWC_MATCH_TYPE_SC_SV,        // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o��
    DWC_MATCH_TYPE_SC_CL,        // �T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g��
    DWC_MATCH_TYPE_NUM
};


//----------------------------------------------------------------------------
// typedef - function
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �F�B�w��A�F�B���w��s�A�}�b�`���C�N�����R�[���o�b�N�^
  �����@�Ferror  DWC�G���[���
          cancel TRUE:�L�����Z���Ń}�b�`���C�N�I���AFALSE:�L�����Z������Ă��Ȃ�
          param  �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �K��l���̃R�l�N�V�������m�����ꂽ���Ɉ�x�����Ă΂��
 *---------------------------------------------------------------------------*/
typedef void (*DWCMatchedCallback)(DWCError error, BOOL cancel, void* param);

/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�����R�[���o�b�N�^
  �����@�Ferror    DWC�G���[���
          cancel   TRUE:�L�����Z���Ń}�b�`���C�N�I���AFALSE:�L�����Z���ł͂Ȃ�
          self     TRUE: �������T�[�o�ւ̐ڑ��ɐ����A��������
                         �ڑ����L�����Z�������B
                   FALSE:���̃z�X�g���T�[�o�ւ̐ڑ��ɐ����A
                         �������͐ڑ����L�����Z�������B
          isServer TRUE: self = FALSE �̎��ɏ�L�̓�������������z�X�g��
                         �T�[�o�ł���A
                   FALSE:self = FALSE �̎��ɏ�L�̓�������������z�X�g��
                         �N���C�A���g�ł��邩�Aself = TRUE
          index    self = FALSE �̎��ɁA��L�̓�������������z�X�g��
                   �F�B���X�g�C���f�b�N�X�B
                   �z�X�g���F�B�łȂ����Aself = TRUE �̎���-1�ƂȂ�B
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �P�̃R�l�N�V�������m�������i���s�A�L�����Z�����j�x�ɌĂ΂��B
  �G���[�������́Aparam�ȊO�̊e�����̒l�͕s��B
 *---------------------------------------------------------------------------*/
typedef void (*DWCMatchedSCCallback)(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int index, void* param);

/*---------------------------------------------------------------------------*
  �V�K�ڑ��N���C�A���g�ڑ��J�n�ʒm�R�[���o�b�N�^
  �����@�Findex �V�K�ڑ��N���C�A���g�̗F�B���X�g�C���f�b�N�X�B
                �V�K�ڑ��N���C�A���g���F�B�łȂ����-1�ƂȂ�B
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �T�[�o�N���C�A���g�}�b�`���C�N�ŁA���ł��Ă���l�b�g���[�N�ɐV����
  �ʂ̃N���C�A���g���ڑ����J�n�������ɌĂ΂��
 *---------------------------------------------------------------------------*/
typedef void (*DWCNewClientCallback)(int index, void* param);

/*---------------------------------------------------------------------------*
  �v���C���[�]���R�[���o�b�N�^
  �����@�Findex �]���Ώۃv���C���[�̃C���f�b�N�X�B�}�b�`���C�N�L�[�l��
                �擾����֐��ɓn���K�v������B
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�F�v���C���[�̕]���l�B0�ȉ��Ȃ�}�b�`���C�N�̑Ώۂ���O�����B
  �F�B�w��E�F�B���w��s�A�}�b�`���C�N���ɁA�}�b�`���C�N�Ώۃv���C���[��
  ������x�ɌĂ΂��B
  �����Őݒ肵���߂�l�̑傫���v���C���[�قǐڑ���Ƃ��đI�΂��\���������Ȃ�B
 *---------------------------------------------------------------------------*/
typedef int (*DWCEvalPlayerCallback)(int index, void* param);

/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N���ߐ؂�R�[���o�b�N�^
  �����@�Fparam �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�ɂ����āA�}�b�`���C�N�̒��ߐ؂��
  �s�����ꍇ�ɁA���ߐ؂菈���������������ɌĂ΂��B
 *---------------------------------------------------------------------------*/
typedef void (*DWCStopSCCallback)(void* param);


//----------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------
// �}�b�`���C�N�I�v�V���� DWC_MATCH_OPTION_MIN_COMPLETE �̎��Ɏg�p����
// �I�v�V�����l�\����
typedef struct DWCstMatchOptMinComplete {
    u8  valid;       // 1:�ݒ��L���ɂ���A0:�ݒ�𖳌��ɂ���
    u8  minEntry;    // �Œ�}�b�`���C�N�����l���i�������܂ށj
    u8  pad[2];      // �p�f�B���O
    u32 timeout;     // �}�b�`���C�N�����Ƃ���^�C���A�E�g���ԁi�P�ʁFmsec�j
} DWCMatchOptMinComplete;


// NAT�l�S�V�G�[�V�����R�[���o�b�N�p�����[�^�p���\����
typedef struct DWCstNNInfo
{
    u8  isQR2;       // 1:QR2���Ȃ̂Ń��g���C�K�v�Ȃ��A0:SB���Ȃ̂Ń��g���C�K�v
    u8  retryCount;  // ���g���C�񐔌v���J�E���^
    u16 port;        // NAT�l�S�V�G�[�V������̃|�[�g�ԍ�
    u32 ip;          // NAT�l�S�V�G�[�V�������IP
    int cookie;      // NAT�l�S�V�G�[�V�����̃N�b�L�[�l�B0��NN���łȂ����Ƃ�\��
} DWCNNInfo;

// �}�b�`���C�N�p�R�}���h����\����
// [todo]
// data�̃T�C�Y��DWCSBMessage�Ƌ��Ɍ������K�v����
typedef struct DWCstMatchCommandControl {
    u8  command;      // ���M�R�}���h
    u8  count;        // ���g���C��
    u16 port;         // ���M��QR2�p�p�u���b�N�|�[�g�ԍ�
    u32 ip;           // ���M��QR2�pIP
    u32 data[32];     // �ǉ����M�f�[�^
    int profileID;    // ���M��v���t�@�C��ID
    int len;          // �ǉ����M�f�[�^�v�f��
    OSTick sendTime;  // ���M����
} DWCMatchCommandControl;

// �}�b�`���C�N����\����
typedef struct DWCstMatchControl
{
    GPConnection* pGpObj;  // gp�R�l�N�V�����\���̃|�C���^�ւ̃|�C���^
        
    GT2Socket* pGt2Socket; // GT2�I�u�W�F�N�g�|�C���^�ւ̃|�C���^
    GT2ConnectionCallbacks* gt2Callbacks;  // GT2�R�[���o�b�N�W�ւ̃|�C���^
    u8  gt2ConnectCount;   // GT2���g���C�J�E���^
    u8  gt2NumConnection;  // ��������GT2�R�l�N�V�����̐�
    u8  gt2NumValidConn;   // �S���Ɛڑ����������L����GT2�R�l�N�V�����̐�
    u8  pad1;
        
    qr2_t qr2Obj;          // QR2�I�u�W�F�N�g�ւ̃|�C���^
    vu8  qr2NNFinishCount; // NAT�l�S�V�G�[�V���������J�E���^
    vu8  qr2MatchType;     // �}�b�`���C�N�̃^�C�v�BDWC_MATCH_TYPE_*�񋓎q
    vu8  qr2NumEntry;      // �����ȊO�ɍő剽�l�W�߂�����
    vu8  qr2IsReserved;    // 1:�����ւ�NN�\��ς݁A0:���\��
#ifdef DWC_QR2_ALIVE_DURING_MATCHING
    u8   qr2ShutdownFlag;  // 1:QR2�V���b�g�_�E����\�񒆁A0:��������
    u8   pad2;
#else
    u16  pad2;
#endif
    u16  qr2Port;          // ������QR2�p�p�u���b�N�|�[�g�ԍ�
    u32  qr2IP;            // ������QR2�p�p�u���b�NIP
    volatile int qr2Reservation;    // �����ւ�NN�\��z�X�g�̃v���t�@�C��ID
    u32 qr2IPList[DWC_MAX_CONNECTIONS];    // �ڑ��z�X�g��QR2�pIP�A�h���X���X�g
    u16 qr2PortList[DWC_MAX_CONNECTIONS];  // �ڑ��z�X�g��QR2�p�|�[�g�ԍ����X�g

    ServerBrowser sbObj;   // SB�I�u�W�F�N�g�ւ̃|�C���^
//    u32 sbUpdateCount;     // SB�T�[�o�A�b�v�f�[�g�Ăяo���Ԋu�J�E���^
    BOOL   sbUpdateFlag;   // SB�T�[�o�A�b�v�f�[�g�Ăяo���t���O
    OSTick sbUpdateTick;   // SB�T�[�o�A�b�v�f�[�g�Ăяo���Ԋu����
    int sbPidList[DWC_MAX_CONNECTIONS];  // �ڑ��z�X�g�̃v���t�@�C��ID���X�g
	OSTick sbUpdateRequestTick; // SB�T�[�o���~�b�g�A�b�v�f�[�g�֐��Ăяo������

    u8  nnRecvCount;       // ����NAT�l�S�V�G�[�V������M��
    u8  nnFailureCount;    // NN���s�J�E���g
    u16 nnCookieRand;      // NAT�l�S�V�G�[�V�����Ɏg���N�b�L�[�p�����l
    int nnLastCookie;      // �O���M����NN�p�N�b�L�[
    OSTick nnFailedTime;   // NAT�l�S�V�G�[�V�������s���̎���
    OSTick nnFinishTime;   // NAT�l�S�V�G�[�V�����I�����̎���
    DWCNNInfo nnInfo;      // NAT�l�S�V�G�[�V�������\����

    DWCMatchState state;   // �}�b�`���C�N�i�s���
    u8  clLinkProgress;    // �N���C�A���g���m�̐ڑ��m���i�x
    u8  friendCount;       // �F�B�w��s�A�}�b�`���C�N�̗F�B���X�g�����J�E���^
    u8  distantFriend;     // 1:�F�B�̗F�B�Ƃ̐ڑ��������A2:�����Ȃ�
    u8  resvWaitCount;     // �\��R�}���h�đ��� 
    u8  closeState;        // �R�l�N�V�����N���[�Y��ԁBDWC_MATCH_CLOSE_STATE_*�񋓎q�Ŏw��
    u8  cancelState;       // �}�b�`���C�N�L�����Z����ԁBDWC_MATCH_CANCEL_STATE_*�񋓎q�Ŏw��
    u8  scResvRetryCount;  // �T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g�̗\�񃊃g���C��
    u8  synResendCount;    // �}�b�`���C�N����������SYN�p�P�b�g�đ���
    u8  cancelSynResendCount;  // �}�b�`���C�N�L�����Z������������SYN�p�P�b�g�đ���
    u8  clWaitTimeoutCount;    // �N���C�A���g�҂���ԃ^�C���A�E�g��
#ifdef DWC_STOP_SC_SERVER
    u8  stopSCFlag;        // �T�[�o�N���C�A���g�}�b�`���C�N���ߐ؂�t���O
    u8  pad3;
#endif
    u16 baseLatency;       // �N���C�A���g�Ȃ�T�[�o�Ƃ̃��C�e���V�A�T�[�o�Ȃ�N���C�A���g�̂����̍ő�̃��C�e���V
    u16 cancelBaseLatency; // �T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�������Ɏg��GP�T�[�o�o�R�̃z�X�g�ԍŒZ���C�e���V
    u16 searchPort;        // �����T�[�oQR2�p�u���b�N�|�[�g�ԍ�
#ifdef DWC_STOP_SC_SERVER
    u16 pad4;
#endif
    u32 searchIP;          // �����T�[�oQR2�p�u���b�NIP
#if 0
    u32 cmdResendCount;    // �}�b�`���C�N�R�}���h�đ��J�E���^
    u32 cmdTimeoutCount;   // �}�b�`���C�N�R�}���h���X�|���X�^�C���A�E�g�J�E���^
#else
    BOOL   cmdResendFlag;  // �}�b�`���C�N�R�}���h�đ��t���O
    OSTick cmdResendTick;  // �}�b�`���C�N�R�}���h�đ�����
    u32 cmdTimeoutTime;    // �}�b�`���C�N�R�}���h���X�|���X�^�C���A�E�g�J�E���^
    OSTick cmdTimeoutStartTick;  // �}�b�`���C�N�R�}���h���X�|���X�^�C���A�E�g�J�E���^
#endif
    u32 synAckBit;         // �}�b�`���C�N����������SYN-ACK�̎�MAID�r�b�g�}�b�v
    u32 cancelSynAckBit;   // �}�b�`���C�N�L�����Z��������SYN-ACK�̎�MAID�r�b�g�}�b�v
#ifdef DWC_LIMIT_FRIENDS_MATCH_VALID
    u32 friendAcceptBit;   // �V�K�ڑ��F�B���R�}���h��M�r�b�g�}�b�v
#endif
    OSTick lastSynSent;          // �Ō�Ƀ}�b�`���C�N���������M���𑗐M��������
    OSTick lastCancelSynSent;    // �Ō�Ƀ}�b�`���C�N�L�����Z�������M���𑗐M��������
    OSTick closedTime;           // ���z�X�g����N���[�Y���ꂽ����
    OSTick clWaitTime;           // �N���C�A���g�҂���ԊJ�n����
    volatile int profileID;      // �����̃v���t�@�C��ID
    int reqProfileID;      // �F�B�w��s�A�}�b�`���C�N��NN�v���𑗂�������̃v���t�@�C��ID
    int priorProfileID;    // �D��NN�v���𑗐M��������T�[�o�̃v���t�@�C��ID
    int cbEventPid;        // ��ɃT�[�o�N���C�A���g�s�A�}�b�`���C�N����
                           // �R�[���o�b�N�ɓn���F�B�̃v���t�@�C��ID�B
                           // �L�����Z�����z�X�g�̃v���t�@�C��ID�̕ۑ��ɂ��p����B
                               
    u32 ipList[DWC_MAX_CONNECTIONS];    // �ڑ��z�X�g�̃Q�[��IP�A�h���X���X�g
    u16 portList[DWC_MAX_CONNECTIONS];  // �ڑ��z�X�g�̃Q�[���|�[�g�ԍ����X�g
    u8  aidList[DWC_MAX_CONNECTIONS];   // �ڑ��z�X�g�ŗL��ID���X�g
    u32 validAidBitmap;                 // �S���Ɛڑ����������L����AID�̃r�b�g�}�b�v�B�}�b�`���C�N���̂��̂���ʂ���ׂɎg���B
    
    const char* gameName;  // GameSpy����^������Q�[����
    const char* secretKey; // GameSpy����^������V�[�N���b�g�L�[

    const DWCFriendData* friendList;  // �F�B���X�g�ւ̃|�C���^
    int friendListLen;                // �F�B���X�g��
    u8  friendIdxList[DWC_MAX_MATCH_IDX_LIST];  // �}�b�`���C�N�F�B�C���f�b�N�X���X�g
    int friendIdxListLen;  // �}�b�`���C�N�F�B�C���f�b�N�X���X�g��
    u32 svDataBak[DWC_MAX_CONNECTIONS+1];    // �T�[�o�ڑ��z�X�g�f�[�^�o�b�N�A�b�v or �L�����Z�����f�[�^�o�b�N�A�b�v
                                             // [todo]
                                             // �g�����̏��Ȃ��f�[�^�Ȃ̂œ��I�Ɏ��悤�ɂ���

    DWCMatchCommandControl cmdCnt;           // �}�b�`���C�N�p�R�}���h����\����

    DWCMatchedSCCallback matchedCallback;    // dwc_main.c�̃}�b�`���C�N�����R�[���o�b�N
    void* matchedParam;                      // ��L�R�[���o�b�N�p�p�����[�^
    DWCNewClientCallback newClientCallback;  // �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N
    void* newClientParam;                    // ��L�R�[���o�b�N�p�p�����[�^
    DWCEvalPlayerCallback evalCallback;      // �v���C���[�]���R�[���o�b�N
    void* evalParam;                         // ��L�R�[���o�b�N�p�p�����[�^
#ifdef DWC_STOP_SC_SERVER
    DWCStopSCCallback stopSCCallback;        // �T�[�o�N���C�A���g�}�b�`���C�N���ߐ؂芮���R�[���o�b�N
    void* stopSCParam;                       // ��L�R�[���o�b�N�p�p�����[�^
#endif
} DWCMatchControl;

// SB���b�Z�[�W�w�b�_�\����
typedef struct DWCstSBMessageHeader {
    char identifier[4];  // ���ʎq
    u32 version;         // �}�b�`���C�N�v���g�R���o�[�W����
    u8  command;         // �R�}���h��ʁi�萔DWC_SB_COMMAND_*�j
    u8  size;            // �R�}���h�ɑ΂���f�[�^�T�C�Y�i�w�b�_�T�C�Y�͊܂܂Ȃ��j
    u16 qr2Port;         // ������QR2�p�p�u���b�N�|�[�g�ԍ�
    u32 qr2IP;           // ������QR2�p�p�u���b�NIP
    int profileID;       // �����̃v���t�@�C��ID 
} DWCSBMessageHeader;

// SB���b�Z�[�W�\����
// �����̃��b�Z�[�W�𑱂��āA�P�̃f�[�^�Ƃ��đ��M���邱�Ƃ��\�B
// [todo]
// data�̃T�C�Y�͉��炩�̏����GP�o�f�B���b�Z�[�W�R�}���h�Ƌ��ʂ�
// �݂��Ȃ���΂Ȃ�Ȃ�
typedef struct DWCstSBMessage {
    DWCSBMessageHeader header;      // SB���b�Z�[�W�w�b�_
    u32 data[DWC_MAX_CONNECTIONS];  // ���M�f�[�^�{��
} DWCSBMessage;

// �Q�[����`�̃}�b�`���C�N�pQR2�L�[�f�[�^�\����
typedef struct DWCstGameMatchKeyData {
    u8  keyID;          // �L�[ID
    u8  isStr;          // 0:�o�����[��int�A1:�o�����[��������
    u16 pad;
    char* keyStr;       // �L�[������ւ̃|�C���^
    const void* value;  // �o�����[�ւ̃|�C���^
} DWCGameMatchKeyData;

// �}�b�`���C�N�I�v�V���� DWC_MATCH_OPTION_MIN_COMPLETE �̎��Ɏg�p����
// �I�v�V�����l�����\���� 
typedef struct DWCstMatchOptMinCompleteIn {
    u8  valid;            // 1:�ݒ��L���ɂ���A0:�ݒ�𖳌��ɂ���
    u8  minEntry;         // �Œ�}�b�`���C�N�����l���i�������܂ށj
    u8  retry;            // 1:�ŏ��̃|�[�����O�ȍ~�A0:�ŏ��̃|�[�����O�܂�
    u8  pad;
    u32 timeout;          // �}�b�`���C�N�����Ƃ���^�C���A�E�g���ԁi�P�ʁFmsec�j
    u32 recvBit;          // �|�[�����O�ɑ΂���N���C�A���g�̕ԐM�󋵃r�b�g
    u32 timeoutBit;       // �N���C�A���g�̃^�C���A�E�g�󋵃r�b�g
    OSTick startTime;     // �}�b�`���C�N�J�n����
    OSTick lastPollTime;  // �Ō�Ƀ|�[�����O���s��������
} DWCMatchOptMinCompleteIn;

// �}�b�`���C�N�I�v�V����DWC_MATCH_OPTION_SC_CONNECT_BLOCK �̎��Ɏg�p����
// �I�v�V�����l�����\����
typedef struct DWCstMatchOptSCBlock {
    u8  valid;         // 1:�ݒ��L���ɂ���A0:�ݒ�𖳌��ɂ���
    u8  lock;          // 1:�ڑ�������V�K�ڑ����ے��A0:�ʏ펞
    u16 pad;
} DWCMatchOptSCBlock;


//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------
#ifdef DWC_QR2_ALIVE_DURING_MATCHING
/*---------------------------------------------------------------------------*
  �}�b�`���C�N�X�e�[�^�X�̃}�X�^�[�T�[�o�o�^�֐��i�F�B�w��A���w��s�A�}�b�`���C�N�p�j
  �����@�F�Ȃ�
  �߂�l�FTRUE :�}�X�^�[�T�[�o�Ƀ}�b�`���C�N�X�e�[�^�X���o�^���ꂽ�A
          FALSE:���O�C���O�A�������̓G���[�̂��ߓo�^����Ȃ�����
  �p�r�@�F���z�X�g�̃}�b�`���C�N�X�e�[�^�X���}�X�^�[�T�[�o�ɓo�^����B
          ���̊֐����Ă΂�Ă���ŒZ15�b���ŁA���z�X�g���玩�z�X�g�̏�Ԃ�
          ������悤�ɂȂ�A�}�b�`���C�N��i�߂���悤�ɂȂ�B
          �T�[�o�N���C�A���g�}�b�`���C�N�ȊO�ł́A�}�b�`���C�N���I��������_��
          �}�X�^�[�T�[�o���炱�̃}�b�`���C�N�X�e�[�^�X�����������B
          �}�b�`���C�N�J�n�֐����Ăׂ΁A���C�u�������ł��̊֐����Ă΂�邪�A
          �}�b�`���C�N�̏I���𑁂߂�ׂɁA�\�߃}�b�`���C�N�X�e�[�^�X��
          �}�X�^�[�T�[�o�ɓo�^���Ă��������ꍇ�́A���̊֐����Ăяo���΂悢�B
          �܂��A�}�b�`���C�N�X�e�[�^�X��DWC_ShutdownFriendsMatch()���Ă񂾎��ɂ�
          ���������B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_RegisterMatchingStatus(void);
#endif


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�����L�����Z���֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:�L�����Z�����������s�����AFALSE:�}�b�`���C�N���łȂ�
  �p�r�@�F�i�s���̃}�b�`���C�N�������L�����Z������B
          �L�����Z�������͖{�֐����Ŋ������A�}�b�`���C�N�����R�[���o�b�N��
          �Ăяo�����B
 *---------------------------------------------------------------------------*/
BOOL DWC_CancelMatching(void);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�����L�����Z���֐� �񓯊��� (obsolete function)
  �����@�F�Ȃ�
  �߂�l�FTRUE:�L�����Z�������̎��s���J�n�����AFALSE:�}�b�`���C�N���łȂ�
  �p�r�@�F�i�s���̃}�b�`���C�N�������L�����Z������B
          �K�v�ȃL�����Z���������s������A�}�b�`���C�N�����R�[���o�b�N��
          �Ăяo�����B
          ��DWC_CancelMatching()���g���ĉ�����
 *---------------------------------------------------------------------------*/
extern BOOL DWC_CancelMatchingAsync(void);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�L�����Z���L���m�F�֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:�L�����Z���\�AFALSE:�}�b�`���C�N���łȂ�������A���f�ł��Ȃ�
          �󋵂ŃL�����Z���s�\�B
  �p�r�@�F�i�s���̃}�b�`���C�N�������L�����Z���ł��邩�ǂ����𒲂ׂ�
 *---------------------------------------------------------------------------*/
extern BOOL DWC_IsValidCancelMatching(void);


#ifdef DWC_STOP_SC_SERVER
/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N �T�[�o���ߐ؂�֐�
  �����@�Fcallback ���ߐ؂芮���R�[���o�b�N
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�}�b�`���C�N�̒��ߐ؂�J�n�B����������R�[���o�b�N���Ԃ��Ă���B
          FALSE:�G���[�����AFriendsMatch���C�u�����񓮍쒆�Ȃǂ̗��R�Œ��ߐ؂莸�s�B
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N�̃}�b�`���C�N����ߐ؂�A�Ȍ�V�K��
          �ڑ����󂯕t���Ȃ��悤�ɂ���B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_StopSCMatchingAsync(DWCStopSCCallback callback, void* param);
#endif


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�w�W�L�[�ǉ��֐� int��
  �����@�FkeyID        �L�[ID���w�肵�ăZ�b�g�������ꍇ�͂����ɃL�[ID��n���B
                       �V�K��keyID���擾��������� DWC_QR2_GAME_KEY_START
                       �ȉ��̒l�i0�Ȃǁj���Z�b�g����Ηǂ��B
                       ���Ɏg�p���̒ǉ��L�[ID���w�肵���ꍇ�́A�g�p���̂��̂�
                       ����keyString��n����valueSrc���Z�b�g���������A
                       �ႤkeyString��n�����ꍇ�͉������Ȃ��B
          keyString    �L�[���ʂ̂��߂̕�����ւ̃|�C���^�B
                       ������̓R�s�[���ă��C�u�������ŕێ����܂��B
          valueSrc     �L�[�ɑΉ�����l�ւ̃|�C���^�ŁA�}�X�^�[�T�[�o�����
                       �|�[�����O������ƁA���̃|�C���^����l���Q�Ƃ���
                       �}�X�^�[�T�[�o�ɒl���A�b�v����B
  �߂�l�F�ǉ������L�[�̃L�[ID�B�p�����[�^�G���[�A�������̓L�[�̐ݒ�����
          �z�������ߒǉ��ł��Ȃ������ꍇ��0��Ԃ��B
          DWC_InitFriendsMatch()���Ă΂����O�ɌĂяo���Ă�0��Ԃ��B
  �p�r�@�F�}�b�`���C�N�̎w�W�ƂȂ�L�[�iint�^�j��ǉ�����B
 *---------------------------------------------------------------------------*/
extern u8  DWC_AddMatchKeyInt(u8 keyID,
                              const char* keyString,
                              const int* valueSrc);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�w�W�L�[�ǉ��֐� string��
  �����@�FkeyID        �L�[ID���w�肵�ăZ�b�g�������ꍇ�͂����ɃL�[ID��n���B
                       �V�K��keyID���擾��������� DWC_QR2_GAME_KEY_START
                       �ȉ��̒l�i0�Ȃǁj���Z�b�g����Ηǂ��B
                       ���Ɏg�p���̒ǉ��L�[ID���w�肵���ꍇ�́A�g�p���̂��̂�
                       ����keyString��n����valueSrc���Z�b�g���������A
                       �ႤkeyString��n�����ꍇ�͉������Ȃ��B
          keyString    �L�[���ʂ̂��߂̕�����ւ̃|�C���^�B
                       ������̓R�s�[���ă��C�u�������ŕێ����܂��B
          valueSrc     �L�[�ɑΉ����镶����ւ̃|�C���^�ŁA�}�X�^�[�T�[�o�����
                       �|�[�����O������ƁA���̃|�C���^���當������Q�Ƃ���
                       �}�X�^�[�T�[�o�ɕ�������A�b�v����B
  �߂�l�F�ǉ������L�[�̃L�[ID�B�p�����[�^�G���[�A�������̓L�[�̐ݒ�����
          �z�������ߒǉ��ł��Ȃ������ꍇ��0��Ԃ��B
          DWC_InitFriendsMatch()���Ă΂����O�ɌĂяo���Ă�0��Ԃ��B
  �p�r�@�F�}�b�`���C�N�̎w�W�ƂȂ�L�[�ichar*�^�j��ǉ�����B
 *---------------------------------------------------------------------------*/
extern u8  DWC_AddMatchKeyString(u8 keyID,
                                 const char* keyString,
                                 const char* valueSrc);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�w�W�L�[�擾�֐� int��
  �����@�Findex     �}�b�`���C�N���v���C���[�̃C���f�b�N�X�B�]���R�[���o�b�N��
                    ���� index �����̂܂ܓn���B
          keyString �L�[���ʂ̂��߂̕�����ւ̃|�C���^
          idefault  �w�肵���L�[�������Ă��Ȃ��v���C���[�������ꍇ��
                    �f�t�H���g�l
  �߂�l�F�}�b�`���C�N���v���C���[�����A�w�肵���L�[�ɑΉ�����l�B
          �L�[���Ȃ��ꍇ�͈��� idefault �ɓn�����l���Ԃ��Ă���B
  �p�r�@�F�}�b�`���C�N�w�W�L�[�ɑΉ�����l���擾����B
          �v���C���[�]���R�[���o�b�N���ł̎g�p�̂ݕۏ؂��Ă���A
          ����ȊO�̏ꏊ�ŌĂяo�����ꍇ�̖߂�l�͕s��B
 *---------------------------------------------------------------------------*/
extern int DWC_GetMatchIntValue(int index, const char* keyString, int idefault);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�w�W�L�[�擾�֐� string��
  �����@�Findex     �}�b�`���C�N���v���C���[�̃C���f�b�N�X�B�]���R�[���o�b�N��
                    ���� index �����̂܂ܓn���B
          keyString �L�[���ʂ̂��߂̕�����ւ̃|�C���^
          sdefault  �w�肵���L�[�������Ă��Ȃ��v���C���[�������ꍇ��
                    �f�t�H���g������
  �߂�l�F�}�b�`���C�N���v���C���[�����A�w�肵���L�[�ɑΉ����镶����B
          �L�[���Ȃ��ꍇ�͈��� sdefault �ɓn���������񂪕Ԃ��Ă���B
  �p�r�@�F�}�b�`���C�N�w�W�L�[�ɑΉ����镶������擾����B
          �v���C���[�]���R�[���o�b�N���ł̎g�p�̂ݕۏ؂��Ă���A
          ����ȊO�̏ꏊ�ŌĂяo�����ꍇ�̖߂�l�͕s��B
 *---------------------------------------------------------------------------*/
extern const char* DWC_GetMatchStringValue(int index, const char* keyString, const char* sdefault);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�^�C�v�擾�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ō�ɍs�����}�b�`���C�N��ʁBDWC_MATCH_TYPE_*�Œ�`�����B
          �I�t���C����ԂȂ�-1��Ԃ��B
  �p�r�@�F�Ō�ɍs�����}�b�`���C�N��ʂ��擾����B
 *---------------------------------------------------------------------------*/
extern int DWC_GetLastMatchingType(void);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�I�v�V�����Z�b�g�֐�
  �����@�Fopttype �}�b�`���C�N�I�v�V�����̌^
          optval  �}�b�`���C�N�I�v�V�����̒l�ւ̃|�C���^�B
                  opttype�ɑΉ������^�̃|�C���^��n���B
          optlen  �^���z��̏ꍇ�̂݁A���̔z��̃T�C�Y��n���K�v������B
  �߂�l�FDWC_SET_MATCH_OPT_RESULT_*�^�̗񋓎q
  �p�r�@�F�}�b�`���C�N�ɂ��Ă̗l�X�ȃI�v�V������ݒ肷��B
 *---------------------------------------------------------------------------*/
extern int DWC_SetMatchingOption(DWCMatchOptType opttype, const void* optval, int optlen);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�I�v�V�����擾�֐�
  �����@�Fopttype �}�b�`���C�N�I�v�V�����̌^
          optval  �}�b�`���C�N�I�v�V�����̒l�i�[��|�C���^�B
                  opttype�ɑΉ������^�̃|�C���^��n���B
          optlen  �擾�����I�v�V�����̒l�̃T�C�Y�i�[��
  �߂�l�FDWC_SET_MATCH_OPT_RESULT_*�^�̗񋓎q
  �p�r�@�F�}�b�`���C�N�ɂ��Ă̐ݒ蒆�̃I�v�V�������擾����B
          �w�肵���I�v�V�������ݒ肳��Ă��Ȃ��ꍇ�́A���������ɁA
          ����optlen��0���i�[���ĕԂ��B
 *---------------------------------------------------------------------------*/
extern int DWC_GetMatchingOption(DWCMatchOptType opttype, void* optval, int* optlen);


/*---------------------------------------------------------------------------*
  �w��l���ȉ��}�b�`���C�N�����I�v�V�����E��Ԏ擾�֐�
  �����@�Ftime �}�b�`���C�N�J�n����̌o�ߎ��ԁi�P�ʁFmsec�j�i�[��|�C���^�B
               ���Ԃ��K�v�Ȃ����NULL��n���Ηǂ��B
  �߂�l�F0 :�^�C���A�E�g���ԓ�
          1 :�^�C���A�E�g���Ԃ��z���Ă���A�w��l���ȉ��Ń}�b�`���C�N��������\������
          -1:���̃}�b�`���C�N�I�v�V�������ݒ肳��Ă��Ȃ��A��������OFF�ɂȂ��Ă���
  �p�r�@�F�w��l���ȉ��}�b�`���C�N�����I�v�V�����̎��Ԍo�ߏ󋵂��擾����B
 *---------------------------------------------------------------------------*/
extern int DWC_GetMOMinCompState(u64* time);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�u���b�N�I�v�V������Ԏ擾�֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE :�V�K�ڑ��N���C�A���g���u���b�N���A
          FALSE:�u���b�N���łȂ����A�I�v�V�������ݒ肳��Ă��Ȃ�
  �p�r�@�F�}�b�`���C�N�u���b�N�I�v�V�����̃u���b�N��Ԃ��擾����
 *---------------------------------------------------------------------------*/
extern BOOL DWC_GetMOSCConnectBlockState(void);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�u���b�N�����֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�u���b�N�I�v�V�����̃u���b�N��Ԃ���������
 *---------------------------------------------------------------------------*/
extern void DWC_ClearMOSCConnectBlock(void);


/*---------------------------------------------------------------------------*
  �}�b�`���C�N������Ԏ擾�֐�
  �����@�F�Ȃ�
  �߂�l�F���݂̃}�b�`���C�N�������
  �p�r�@�F�}�b�`���C�N���̓�����Ԃ��擾����B�f�o�b�O���Ƀ}�b�`���C�N�̓�����Ԃ�
          �m�肽���ꍇ�ɂ̂ݎg�p����B
 *---------------------------------------------------------------------------*/
extern DWCMatchState DWC_GetMatchingState(void);


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
extern void DWCi_MatchInit(DWCMatchControl* matchcnt,
                           GPConnection pGpObj,
                           GT2Socket* pGt2Socket,
                           GT2ConnectionCallbacks* gt2Callbacks,
                           const char* gameName,
                           const char* secretKey,
                           const DWCFriendData friendList[],
                           int  friendListLen);


extern qr2_error_t DWCi_QR2Startup(int profileID);


extern void DWCi_ConnectToAnybodyAsync(u8  numEntry,
                                       const char* addFilter,
                                       DWCMatchedSCCallback matchedCallback,
                                       void* matchedParam,
                                       DWCEvalPlayerCallback evalCallback,
                                       void* evalParam);


extern void DWCi_ConnectToFriendsAsync(const u8 friendIdxList[],
                                       int friendIdxListLen,
                                       u8  numEntry,
                                       BOOL distantFriend,
                                       DWCMatchedSCCallback matchedCallback,
                                       void* matchedParam,
                                       DWCEvalPlayerCallback evalCallback,
                                       void* evalParam);


// [arakit] main 051027
extern void DWCi_SetupGameServer(u8  maxEntry,
                                 DWCMatchedSCCallback matchedCallback,
                                 void* matchedParam,
                                 DWCNewClientCallback newClientCallback,
                                 void* newClientParam);


extern void DWCi_ConnectToGameServerAsync(int serverPid,
                                          DWCMatchedSCCallback matchedCallback,
                                          void* matchedParam,
                                          DWCNewClientCallback newClientCallback,
                                          void* newClientParam);


extern void DWCi_MatchProcess(BOOL fullSpec);


extern void DWCi_GT2ConnectAttemptCallback(GT2Socket socket,
                                           GT2Connection connection,
                                           unsigned int ip, unsigned
                                           short port, int latency,
                                           GT2Byte* message, int len);


extern GT2Bool DWCi_GT2UnrecognizedMessageCallback(GT2Socket socket,
                                                   unsigned int ip,
                                                   unsigned short port,
                                                   GT2Byte* message,
                                                   int len);


extern void DWCi_GT2ConnectedCallback(GT2Connection connection,
                                      GT2Result result,
                                      GT2Byte* message,
                                      int len);


extern void DWCi_MatchGPRecvBuddyMsgCallback(GPConnection* pconnection,
                                             int profileID,
                                             char* message);


extern void DWCi_StopMatching(DWCError error, int errorCode);


extern void DWCi_ClearQR2Key(void);


extern void DWCi_ProcessMatchSynPacket(u8 aid, u16 type, u8* data);


extern BOOL DWCi_ProcessMatchClosing(DWCError error, int errorCode, int profileID);


extern void DWCi_ProcessMatchSCClosing(int clientPid);


extern BOOL DWCi_DeleteHostByProfileID(int profileID, int numHost);


extern int  DWCi_DeleteHostByIndex(int index, int numHost);


extern int  DWCi_GetNumAllConnection(void);


extern int  DWCi_GetNumValidConnection(void);


extern void DWCi_SetNumValidConnection(void);


extern int  DWCi_GetAllAIDList(u8** aidList);


extern int  DWCi_GetValidAIDList(u8** aidList);


extern GPResult DWCi_GPSetServerStatus(void);


extern void DWCi_ShutdownMatch(void);


extern BOOL DWCi_IsShutdownMatch(void);


extern int  DWCi_GetNumValidConnection(void);


extern void DWCi_NNFreeNegotiateList(void);


#ifdef __cplusplus
}
#endif

#endif // DWC_MATCH_H_

������������������������������������������������������������������������������
��                                                                          ��
��  Readme.txt                                                              ��
��                                                                          ��
��  �|�P�����v���`�i Wi-Fi ���r�[���C�u����                                 ��
��                                                                          ��
������������������������������������������������������������������������������

1. �͂��߂�
==============================================================================

�@���̃��C�u�����́AGameSpy��IRC�T�[�o����đ��̃v���C���[�ƒʐM������̂ł��B
���C�u�����̏ڍׂ�man�t�H���_���̊֐����t�@�����X���Q�Ƃ��Ă��������B


2. �f�B���N�g���\��
==============================================================================

+build----+-demos      �f��
          +-libraries  ���C�u�����\�[�X�t�@�C��
+include               �O���C���N���[�h�t�@�C��
+lib                   ���C�u�����t�@�C��
+man                   �֐����t�@�����X
+tools                 �Ǘ��c�[��


3. �e�X�g��
==============================================================================

�@�{�p�b�P�[�W�́A�ȉ��̊��ō\�z�A����m�F����܂����B

 NitroSDK-4_2-patch-plus-080118
 NITRO-System-Library-071126-patch1
 NitroDWC-2_2plus5-080612
 NitroWiFi-2_1-patch-plus3-070920
 CodeWarrior-2.0-sp2-patch2


4. �C���X�g�[��
==============================================================================

�@�p�b�P�[�W�Ɋ܂܂�邷�ׂẴt�@�C����������NitroDWC�̃p�b�P�[�W�ɏ㏑������
���������B
�r���h�ς݂̃��C�u�����t�@�C���͕t�����Ă��܂����A�\�[�X�R�[�h����������Ă���
���̂Ńr���h���邱�Ƃ��\�ł��B
���C�u�������r���h����ꍇ�͑O��㏑���������̂��N���[����A�r���h���s���Ă���
�����B
$(NITRODWC_ROOT)/include/ppwlobby/ppw_lobby.h���C���N���[�h���A
$(NITRODWC_ROOT)/lib�ȉ���libdwcppwlobby.a��libdwcilobby.a��libdwcenc.a�������N
���Ă��������B

5. �f��
==============================================================================

�@���r�[���C�u���������������A���b�Z�[�W���M�Ȃǂ��s���f���ł��B
�������������X�{�^���������ƃ��r�[���C�u�����̊֐������ɌĂяo���܂��B
A�{�^���������ƃ~�j�Q�[���̕�W����P2P�ʐM�܂ł��s���܂��B

6. �Ǘ��p�c�[��
==============================================================================

�@tools�ȉ���Wi-Fi�Ђ�΂̊Ǘ��c�[�������^����Ă��܂��B�ڂ�����
"tools/PpwAdminTool/Readme.txt"���Q�Ƃ��Ă��������B


�y�X�V�����z

�� 2008/07/07 1_0_0

�E20�l�W�܂����u�Ԃɐl��������ƃ��b�N����Ȃ��l���o�Ă���s����C�����܂����B

�� 2008/07/02 1_0_0 beta 20

�E�ڑ�����h���C����peerchat.gamespy.com�������̂�
  <gamename>.gs.peerchat.nintendowifi.net�ɏC�����܂����B

�� 2008/06/23 1_0_0 beta 19

�E���������Ƀ��C�u�������V���b�g�_�E������ƃ��C�u�������s���ȏ�ԂƂȂ萳����
�V���b�g�_�E������Ȃ��Ȃ�o�O���C�����܂����B
�EPPW_LobbyShutdownAsync�֐��̎d�l��ύX���܂����BDWC_InitFriendsMatch�֐��Ƌ�
�Ƀ��r�[���C�u�������g�p���Ă���Ƃ��́A���r�[���C�u�����̏I�����m�F�������
DWC_ShutdownFriendsMatch�֐����Ăяo���Ă��������B  

�� 2008/06/20 1_0_0 beta 18

�EPPW_LobbySystemProfileUpdatedCallback�R�[���o�b�N��
  PPW_LobbyPlayerConnectedCallback�R�[���o�b�N�̑O�y��
  PPW_LobbyPlayerDisconnectedCallback�R�[���o�b�N�̌�ɌĂ΂��\��������܂�
  ���̂ŌĂ΂�Ȃ��悤�ɏC�����܂����B

�� 2008/06/18 1_0_0 beta 17

�E���C���`�����l���ɐڑ�����ۂ�3��܂ōĎ��s����悤�ɂ��܂����B
�E�Ǘ��c�[����ver1_0_0 beta7�ɍX�V���܂����B

�� 2008/06/16 1_0_0 beta 16

�E�A���P�[�g���ɃX�y�V�����E�B�[�N���ǂ����̃t���O��ǉ����܂����B
�E�Ǘ��c�[����ver1_0_0 beta6�ɍX�V���܂����B

�� 2008/06/13 1_0_0 beta 15

�E�A���P�[�g�@�\�ɑΉ����܂����B
�E�A���P�[�g���o����PPW_LobbySubmitQuestionnaire�֐���ǉ����܂����B
�E�擾�����A���P�[�g����ʒm����PPW_LobbyGetQuestionnaireCallback�R�[���o�b
�@�N��ǉ����܂����B
�E�A���P�[�g�̒�o�����������Ƃ��ɌĂяo�����
�@PPW_LobbySubmitQuestionnaireCallback�R�[���o�b�N��ǉ����܂����B
�E�G���[�̏�Ԃ���G���[�R�[�h���擾����PPW_LobbyToErrorCode�֐���ǉ����܂����B
�E�Ǘ��c�[����ver1_0_0 beta5�ɍX�V���܂����B

�� 2008/05/27 1_0_0 beta 14

�E�^�C�v�\���ɑΉ����܂����B
�E�`���b�g�T�[�o�ւ̐ڑ��ƃX�P�W���[���Ȃǂ̃f�[�^�擾�𓯎��ɍs�����Ƃɂ��A
  ���������Ԃ��኱�Z�k���܂����B
�E�Ǘ��c�[����ver1_0_0 beta4�ɍX�V���܂����B

�� 2008/05/08 1_0_0 beta 13

�EPPW_LobbyCheckProfileCallback�R�[���o�b�N�̕Ԃ�l��void�ɏC�����܂����B

�� 2008/05/07 1_0_0 beta 12

�E�s���`�F�b�N���s���悤�ɂ��܂����B
�E�s���`�F�b�N�̌��ʂ�ʒm����APPW_LobbyCheckProfileCallback�R�[���o�b�N��ǉ�
  ���܂����B 
�E�X�P�W���[����VIP�ݒ�̎擾��(�����[�X/�f�o�b�O)��PPW_LobbyInitializeAsync��
  ����prefix�ɂ�茈�肳���悤�ɂ��܂����BDWC_LOBBY_CHANNEL_PREFIX_RELEASE��
  �����[�X�T�[�o����A����ȊO�ł̓f�o�b�O�T�[�o����f�[�^���擾���܂��B�������A
  ���݃����[�X�T�[�o�̗p�ӂ͂ł��Ă��܂���B
�E�Ǘ��c�[����ver1_0_0 beta3�ɍX�V���܂����B

�� 2008/04/16 1_0_0 beta 11

�E�Ǘ��c�[����ver1_0_0 beta2�ɍX�V���܂����B

�� 2008/04/14 1_0_0 beta 10

�E�T�[�o����X�P�W���[����VIP�����擾����悤�ɂ��܂����B
�E�擾�����X�P�W���[����ʒm����APPW_LobbyNotifyScheduleCallback�R�[���o�b�N���
  �����܂����B
�E�擾����VIP����ʒm����A PPW_LobbyGetVipCallback�R�[���o�b�N��ǉ����܂����B
�E�T�[�o�Ƃ̒ʐM�����������Ԃ�ʒm����APPW_LobbyExcessFloodCallback�R�[���o�b
  �N��ǉ����܂����B
�EVIP�����Q�Ƃ���APPW_LobbyGetVip�֐���ǉ����܂����B
�E�X�P�W���[����VIP�̐ݒ���s����Ǘ��c�[����ǉ����܂����B

�� 2008/02/07 1_0_0 beta 9

�E�f���̃~�j�Q�[����W���@���C�����܂����B
�E�T�[�o����ؒf���ꂽ�Ƃ��Ƀ��C�u�������I���ł��Ȃ��Ȃ�o�O���C�����܂����B
�E�`�����l���̐l�������������Ȃ��o�O���C�����܂����B
�E�V�X�e����`�v���t�B�[�����ω����Ă��Ȃ��̂�
  PPW_LobbySystemProfileUpdatedCallback�R�[���o�b�N���Ă΂��o�O���C�����܂����B
�EPPW_LobbySchedule�\���̂�roomFlag�����o��ǉ����A�A���Z�E�X�t���O���Z�b�g����
  ��悤�ɂ��܂����B

�� 2008/01/22 1_0_0 beta 8

�E�C�x���g(PPW_LOBBY_TIME_EVENT)�̎�ނ�����PlaWifi080117�ɍ��킹�܂����B
�E�������ɃC�x���g�����������ہAPPW_LobbyScheduleProgressCallback�R�[���o�b�N��
  ����t���[�����ɔ�������悤�ɂ��܂����B
�E�Ή����C�u�������A�b�v�f�[�g���܂����B

�� 2008/01/10 1_0_0 beta 7

�E�����̃��C�u������libdwcilobby.a�֕������܂����B
�EPPW_IsConnectedUserId�֐���dwcFriendMatchCtrl�p�����[�^��const�ɂ��܂����B
�EPPW_LobbyBinaryMassageReceivedCallback�R�[���o�b�N��data�p�����[�^��const��
  ���܂����B
�E��l�ȏ�`�����l���ɎQ�������Ƃ��ɐ�����READY��ԂɂȂ�Ȃ��Ȃ�o�O���C����
  �܂����B(beta5����)
�E���[�U���Đڑ������Ƃ��ɂ��̃��[�U�ɑ΂���
  PPW_LobbyPlayerDisconnectedCallback�R�[���o�b�N���Ă񂾌�A
  PPW_LobbyPlayerConnectedCallback�R�[���o�b�N���ĂԂ悤�ɏC�����܂����B
�E�����l�ł̊J�����ɓ���̃`�����l���ɓ���Ȃ��悤�ɂ��邽�߂�
  DWC_LOBBY_CHANNEL_PREFIX_DEBUG��10�ɑ��₵�܂����B
�EPPW_LobbyEnumUserId�֐������������ʂ�Ԃ��Ȃ��o�O���C�����܂����B
�E�G���[���b�Z�[�W�̕\���ɂ��Ċ֐����t�@�����X�ɒǋL���܂����B

�� 2007/12/20 1_0_0 beta 6

�E�`�����l�����̂ɃZ�b�g�����`�����l���f�[�^����M�ł��Ȃ��o�O���C�����܂����B
�Ebeta5��PPW_LobbyGetChannelDataCallback��param�p�����[�^�����NULL�ƂȂ�o�O
  ������܂����̂ŏC�����܂����B
�E�֐����t�@�����X�ŁAPPW_LobbySystemProfile�\���́APPW_LobbyRecruitInfo�\���́A
  PPW_LobbyTimeInfo�\���̂̃����o�Ɋւ������������Ă����蔲���Ă����肵�Ă�
  ���̂��C�����܂����B

�� 2007/12/19 1_0_0 beta 5

�EPPW_LobbySendPlayer*Message�֐��ő��M���ꂽ���b�Z�[�W����M����Ǝ~�܂�o�O
  ���C�����܂����B
�E�������g�����C���`�����l���ɓ������Ƃ��ɂ�PPW_LobbyPlayerConnectedCallback��
  �Ă΂��悤�ɂ��܂����B

�� 2007/12/14 1_0_0 beta 4

�EPPW_LobbyStartRecruit�֐���������matchMakingString���Z�b�g���Ȃ��o�O���C����
  �܂����B
�E�w�肵�����[�UID�̐l��P2P�ڑ����Ă��邩�𒲂ׂ�APPW_IsConnectedUserId�֐���
  �ǉ����܂����B
�E�~�j�Q�[���̕�W���I�������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�A
  PPW_LobbyRecruitStoppedCallback��ǉ����܂����B
�E�������m�ۂɎ��s�����Ƃ���Panic�ŉ��������Ă����̂�Panic���Ȃ��悤�C�����܂�
  ���B
�E�f���Ń~�j�Q�[���̕�W����P2P�ʐM�܂ł��s���悤�ɂ��܂����B

�� 2007/12/05 1_0_0 beta 3

�E�������g�p�ʍ팸�̂���RTTI���I�t�ɂ��܂����B

�� 2007/12/04 1_0_0 beta 2

�E�������g�p�ʍ팸�̂��ߗ�O�@�\���I�t�ɂ��܂����B

�� 2007/11/30 1_0_0 beta 1

�E���񃊃��[�X�B

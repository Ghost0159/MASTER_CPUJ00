/*---------------------------------------------------------------------------*
  Project:  NitroDWC -  - 
  File:     dwc_main.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_main.h,v $
  Revision 1.31  2006/04/27 07:06:09  takayama
  ingamesn��10��������25�����Ɋg���B

  Revision 1.30  2006/01/23 04:50:43  arakit
  ����p��ɍ��킹�ăR�����g���C��

  Revision 1.29  2006/01/17 07:29:17  arakit
  �EDWC_SetConnectionClosedCallback()�̖߂�l��void����BOOL�ɕύX���A
  �@����ϐ��m�ۑO�͉���������FALSE��Ԃ��悤�ɂ����B

  Revision 1.28  2005/12/21 03:08:54  arakit
  ���O�C���̍ۂɃT�[�o�ɃQ�[�����X�N���[���l�[����n����悤�ɂ����B

  Revision 1.27  2005/11/04 05:57:17  arakit
  �֐������̃R�����g��ǉ������B

  Revision 1.26  2005/11/02 02:49:33  arakit
  �t�@�C���̃C���N���[�h�֌W�𒲐������B

  Revision 1.25  2005/10/28 09:10:20  arakit
  �R�����g���C�������B

  Revision 1.24  2005/10/26 09:06:35  arakit
  DWC_LoginAsync(), DWC_ConnectToAnybodyAsync(), DWC_ConnectToFriendsAsync(),
  DWC_SetupGameServerAsync(), DWC_ConnectToGameServerAsync() �ɖ߂�l��^�����B

  Revision 1.23  2005/10/05 05:19:27  arakit
  �EDWC_UpdateServersAsync()�̃R�[���o�b�N���ŁA�K��State��LOGIN�ɏ㏑�����Ă��܂�
  �@�s����C�������B

  Revision 1.22  2005/09/26 05:06:08  arakit
  �ESB�A�b�v�f�[�g�p��addFilter��������A���C�u���������ɃR�s�[���Ď��悤�ɕύX�����B
  �EDWC_AddMatchKey*()��key���̕�������A���C�u���������ɃR�s�[���Ď��悤�ɕύX�����B

  Revision 1.21  2005/09/24 12:56:30  arakit
  �T�[�o�N���C�A���g�^�}�b�`���O���ɁA�}�b�`���O���������z�X�g�݂̂�L����AID�Ƃ���
  �����悤�AAID��R�l�N�V�������ւ̃A�N�Z�X�֐����C�������B

  Revision 1.20  2005/09/21 05:21:26  arakit
  �E�L�����Z�����܂߂��N���[�Y�����̃V�X�e����傫���ύX�����B
  �E�R�}���h���M�̃��g���C�A�^�C���A�E�g�������쐬�����B
  �ENAT�l�S�V�G�[�V�������s���̃}�b�`���O��蒼��������ǉ������B
  �Egt2�̃\�t�g�N���[�Y��S�ăn�[�h�N���[�Y�ɒu���������B
  �@����ɔ���DWCi_CloseConnectionsAsync()���g��Ȃ��悤�ɂ����B
  �i���j�T�[�o�N���C�A���g�^�}�b�`���O�͐��퓮�삵�Ȃ�

  Revision 1.19  2005/09/15 00:22:06  arakit
  �EDWC_CloseConnectionsAsync()�ɖ߂�l��ǉ������B
  �E�R�l�N�V���������ؒf�֐�DWC_CloseConnectionHard(u8 aid)��
  �@int  DWC_CloseConnectionHardBitmap(u8 aid) ��ǉ������B

  Revision 1.18  2005/09/13 11:10:56  arakit
  �E���ɃN���[�Y���ꂽaid�ɑ΂��đ��M���悤�Ƃ�����f�[�^�A�N�Z�X��O��
  �@�~�܂��Ă��܂��s����C�������B
  �EAID�r�b�g�}�b�v�擾�֐�   u32 DWC_GetAIDBitmap( void )�A
  �@AID�w��Reliable���M�֐�   u32 DWC_SendReliableBitmap( u32 bitmap, const void* buffer, int size )�A
  �@AID�w��Unreliable���M�֐� u32 DWC_SendUnreliableBitmap( u32 bitmap, const void* buffer, int size )
  �@��p�ӂ����B

  Revision 1.17  2005/09/09 10:43:59  arakit
  SB��Persistent��DNS�G���[���N�������ۂɃ��g���C����悤�ɂ����B

  Revision 1.16  2005/09/09 09:54:30  arakit
  �o�[�W�����Ⴂ�̃��C�u�������m�Ń}�b�`���O���Ȃ��悤�ɕύX�����B

  Revision 1.15  2005/09/08 08:50:24  arakit
  �E���O�C������gt2Socket��QR2���N������悤�ɕύX
  �Eaid���d�����Ĕ��s�����s����C��

  Revision 1.14  2005/09/05 11:44:16  arakit
  �EDWCConnectionClosedCallback�̈���isServer�́A�������T�[�o�N���C�A���g�^
  �@�}�b�`���O�̃N���C�A���g�ŁA�T�[�o���N���[�Y�������̂�TRUE��Ԃ��悤��
  �@���Ă������A����킵���̂ŁA�������T�[�o�Ŏ������N���[�Y���Ă�TRUE��
  �@�Ԃ��悤�ɕύX�����B
  �E�T�[�o�N���C�A���g�^�}�b�`���O�ŁA�N���C�A���g�����Ȃ��Ȃ�������
  �@�T�[�o�̏����ɕs����������̂ŁA�C�������B
  �E�T�[�o�N���C�A���g�^�ŁA�N���[�Y�A�L�����Z�����qr2_shutdown()���Ă΂��
  �@���Ȃ������̂��C�������B

  Revision 1.13  2005/09/03 13:03:53  arakit
  GameSpy�v���t�@�C��ID�������ɂƂ�悤�ɂȂ��Ă����R�[���o�b�N�A
  DWCMatchedSCCallback, DWCNewClientCallback, DWCConnectionClosedCallback
  �ɂ��āA�F�B���X�g�C���f�b�N�X��Ԃ��悤�ɕύX�����B

  Revision 1.12  2005/08/31 10:29:46  arakit
  DWC����\���̂̌^����DWC_FriendsMatchControl�ɕύX�����B

  Revision 1.11  2005/08/31 06:04:52  arakit
  �\�P�b�g�G���[���擾����֐�DWC_GetLastSocketError()��ǉ������B

  Revision 1.10  2005/08/31 02:10:35  arakit
  �F�B�w��}�b�`���O�ŁA�F�B�̗F�B�������邩�ǂ�����I���ł���悤�ɂ����B

  Revision 1.9  2005/08/26 08:15:23  arakit
  �EASSERT�ƃ��W���[������ϐ��̃`�F�b�N�𐮗������B
  �E�O���֐�DWC_ShutdownFriendsMatch()�̈������Ƃ�Ȃ��悤�ɕύX�����B
  �܂��A���֐�����e���W���[���̐���ϐ���NULL�N���A����悤�ɂ����B
  �E�G���[�����֐���ύX���A�G���[�Z�b�g�A�G���[�`�F�b�N��K�v�ȉӏ��ɓ��ꂽ�B

  Revision 1.8  2005/08/23 13:53:46  arakit
  �����_���֐���MATH_Rand*���g���悤�ɂ����B

  Revision 1.7  2005/08/20 07:01:20  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_MAIN_H_
#define DWC_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
// �o�^�ς݃o�f�B�ւ̃o�f�B�o�^�v���G���[�𖳎����邽�߂̃X�C�b�`�B
#define DWC_IGNORE_GP_ERROR_ALREADY_BUDDY
    
// GP�o�f�B���b�Z�[�W�̃R�}���h�萔
//#define DWC_GP_COMMAND_STRING       "DWC_GPCM"  // GP���b�Z�[�W�R�}���h���ʕ�����
#define DWC_GP_COMMAND_STRING       "GPCM"  // GP���b�Z�[�W�R�}���h���ʕ�����
#define DWC_GP_COMMAND_MATCH_STRING "MAT"   // �}�b�`���C�N�p���b�Z�[�W���ʕ�����

#define DWC_DNS_ERROR_RETRY_MAX 5  // DNS�G���[�̃��g���C�����

// �_�~�[�̃����[�g�F�؏������s�����߂̃X�C�b�`
//#define DWC_USE_DUMMY_REMOTE_AUTH

// �v���C���[���i�Q�[�����X�N���[���l�[���j�̍ő咷�i���C�h����25�����{"\0\0"�j
#define DWC_MAX_PLAYER_NAME 26


//----------------------------------------------------------------------------
// enum
//----------------------------------------------------------------------------
// DWC�l�b�g���[�N��ԗ񋓎q
typedef enum {
    DWC_STATE_INIT = 0,         // �������
    DWC_STATE_AVAILABLE_CHECK,  // �Q�[���̗��p�\�󋵂��`�F�b�N��
    DWC_STATE_LOGIN,            // ���O�C��������
    DWC_STATE_ONLINE,           // ���O�C��������I�����C�����
    DWC_STATE_UPDATE_SERVERS,   // �F�B���X�g����������
    DWC_STATE_MATCHING,         // �}�b�`���C�N������
    DWC_STATE_CONNECTED,        // �}�b�`���C�N������A���b�V���^�l�b�g���[�N�\�z�ςݏ��
    DWC_STATE_NUM
} DWCState;


//----------------------------------------------------------------------------
// typedef - function
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �R�l�N�V�����N���[�Y�R�[���o�b�N�^
  �����@�Ferror    DWC�G���[���
          isLocal  TRUE:�������N���[�Y�����AFALSE:���̒N�����N���[�Y�����B
                   �}�b�`���C�N���ɉ����̂Ȃ��z�X�g��ؒf���鎞��TRUE�ɂȂ�B
          isServer TRUE: �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o���N���[�Y�����B
                         �������T�[�o��isLoacal = TRUE �̎���TRUE�ɂȂ�B
                   FALSE:����ȊO�B�T�[�o�N���C�A���g�}�b�`���C�N�ȊO�ł�
                         �K��FALSE�ƂȂ�B
          aid      �N���[�Y�����v���C���[��aid
          index    �N���[�Y�����v���C���[�̗F�B���X�g�C���f�b�N�X�B
                   �N���[�Y�����v���C���[���F�B�łȂ����-1�ƂȂ�B
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
 *---------------------------------------------------------------------------*/
typedef void (*DWCConnectionClosedCallback)(DWCError error, BOOL isLocal, BOOL isServer, u8 aid, int index, void* param);


//----------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------
// FriendsMatch����\����
typedef struct DWCstControl
{
    GT2Socket gt2Socket;   // ���z�X�g��GT2�\�P�b�g�\���̂ւ̃|�C���^
    GT2ConnectionCallbacks gt2Callbacks;  // gt2�R�l�N�V�����R�[���o�b�N�W
    int  gt2SendBufSize;   // GT2�\�P�b�g�̑��M�o�b�t�@�T�C�Y
    int  gt2RecvBufSize;   // GT2�\�P�b�g�̎�M�o�b�t�@�T�C�Y

    GPConnection gpObj;    // gp�R�l�N�V�����\���̂ւ̃|�C���^
    DWCUserData* userdata; // DWCUserData�\���̂ւ̃|�C���^

    DWCState state;        // �l�b�g���[�N���
    DWCState lastState;    // �O��̃l�b�g���[�N���
    u8   aid;              // ������AID�i���b�V���^�l�b�g���[�N���ł̌ŗLID�j
    u8   ownCloseFlag;     // TRUE:�Q�[�����������ŃN���[�Y�����AFALSE:����ȊO�̃N���[�Y
    u16  playerName[DWC_MAX_PLAYER_NAME];  // ���̃��[�U���Q�Ƃ��鎩���̃v���C���[��
    int  profileID;        // ���[�U�v���t�@�C��ID
    const char* gameName;  // GameSpy����^������Q�[�����ւ̃|�C���^
    const char* secretKey; // GameSpy����^������V�[�N���b�g�L�[�ւ̃|�C���^

    DWCLoginCallback         loginCallback;          // ���O�C�������R�[���o�b�N
    void *loginParam;          // ��L�R�[���o�b�N�p�p�����[�^
    DWCUpdateServersCallback updateServersCallback;  // �F�B���X�g�������������R�[���o�b�N
    void* updateServersParam;  // ��L�R�[���o�b�N�p�p�����[�^
    DWCMatchedCallback       matchedCallback;        // �s�A�}�b�`���C�N�����R�[���o�b�N
    void* matchedParam;        // ��L�R�[���o�b�N�p�p�����[�^
    DWCMatchedSCCallback     matchedSCCallback;      // �T�[�o�N���C�A���g�}�b�`���C�N�����R�[���o�b�N
    void* matchedSCParam;      // ��L�R�[���o�b�N�p�p�����[�^
    DWCConnectionClosedCallback closedCallback;      // �R�l�N�V�����N���[�Y�R�[���o�b�N
    void* closedParam;         // ��L�R�[���o�b�N�p�p�����[�^

    DWCLoginControl logcnt;       // ���O�C������I�u�W�F�N�g
    DWCFriendControl friendcnt;   // �F�B�Ǘ��I�u�W�F�N�g
    DWCMatchControl matchcnt;     // �}�b�`���C�N����I�u�W�F�N�g
    DWCTransportInfo transinfo;   // �g�����X�|�[�g����I�u�W�F�N�g
} DWCFriendsMatchControl;

// GT2�R�l�N�V�������\����
typedef struct DWCstConnectionInfo
{
    u8  index;    // �i�[����Ă���R�l�N�V�������X�g�̃C���f�b�N�X
    u8  aid;      // �z�X�g�ŗLID
    u16 reserve;  // �\��
    void* param;  // ���[�U�f�[�^
} DWCConnectionInfo;


//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  FriendsMatch���C�u�����������֐�
  �����@�Fdwccnt        FriendsMatch����I�u�W�F�N�g�ւ̃|�C���^
          userdata      ���[�U�f�[�^�I�u�W�F�N�g�ւ̃|�C���^
          productID     GameSpy����^������v���_�N�gID
          gameName      GameSpy����^����ꂽ�Q�[�����iNULL�I�[�K�v�j
          secretKey     GameSpy����^����ꂽ�V�[�N���b�g�L�[�iNULL�I�[�K�v�j
          sendBufSize   DWC_Transport���g�����M�o�b�t�@�T�C�Y�B0�Ȃ�f�t�H���g8KByte���g�p����B
          recvBufSize   DWC_Transport���g����M�o�b�t�@�T�C�Y�B0�Ȃ�f�t�H���g8KByte���g�p����B
          friendList    �F�B���X�g�i�Q�[���Ŏg�p���Ȃ��ꍇ��NULL�ł��ǂ��j
          friendListLen �F�B���X�g�̍ő咷�i�v�f���j
  �߂�l�F�Ȃ�
  �p�r�@�FFriendsMatch���C�u����������������
 *---------------------------------------------------------------------------*/
extern void DWC_InitFriendsMatch(DWCFriendsMatchControl* dwccnt,
                                 DWCUserData* userdata,
                                 int productID,
                                 const char* gameName,
                                 const char* secretKey,
                                 int sendBufSize,
                                 int recvBufSize,
                                 DWCFriendData friendList[],
                                 int friendListLen);

/*---------------------------------------------------------------------------*
  FriendsMatch���C�u�����I���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FFriendsMatch���C�u�������I�����AGameSpySDK�̃q�[�v�̈���J������
 *---------------------------------------------------------------------------*/
extern void DWC_ShutdownFriendsMatch(void);


/*---------------------------------------------------------------------------*
  FriendsMatch���C�u�����ʐM�����X�V�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F���Q�[���t���[���Ăяo���AFriendsMatch���C�u�����̒ʐM�������X�V����
 *---------------------------------------------------------------------------*/
extern void DWC_ProcessFriendsMatch(void);


/*---------------------------------------------------------------------------*
  Wi-Fi�R�l�N�V�������O�C���֐�
  �����@�Fingamesn �Q�[�����X�N���[���l�[��
          reserved �ߋ��̎d�l�BNULL��n���B
          callback ���O�C�������ʒm�p�R�[���o�b�N�֐�
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B���������s�ŃR�[���o�b�N���Ԃ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�Q�[�������p�\�ł��邩���ׁA�����[�g�F�؁AGP�T�[�o�ւ̐ڑ����s��
 *---------------------------------------------------------------------------*/
// [arakit] main 051025
extern BOOL DWC_LoginAsync(const u16*  ingamesn,
                           const char* reserved,
                           DWCLoginCallback callback,
                           void* param);


/*---------------------------------------------------------------------------*
  �F�B���X�g���������֐�
  �����@�FplayerName     ���̃��[�U���Q�Ƃ��鎩���̃v���C���[��
                         ���ߋ��̎d�l�ł��B���݂̓Z�b�g���ꂽ�l�𖳎����Ă��܂��B
          updateCallback �F�B���X�g�������������R�[���o�b�N
          updateParam    ��L�R�[���o�b�N�p�p�����[�^
          statusCallback �F�B��ԕω��ʒm�R�[���o�b�N
          statusParam    ��L�R�[���o�b�N�p�p�����[�^
          deleteCallback �F�B���X�g�폜�R�[���o�b�N
          deleteParam    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B���������s�ŃR�[���o�b�N���Ԃ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F���O�C��������AGP�T�[�o��̗F�B���X�g�i�o�f�B���X�g�j��
          ���[�J���̗F�B���X�g�̓����������s��
 *---------------------------------------------------------------------------*/
extern BOOL DWC_UpdateServersAsync(const char* playerName,
                                   DWCUpdateServersCallback updateCallback,
                                   void* updateParam,
                                   DWCFriendStatusCallback statusCallback,
                                   void* statusParam,
                                   DWCDeleteFriendListCallback deleteCallback,
                                   void* deleteParam);


/*---------------------------------------------------------------------------*
  �F�B���w��s�A�}�b�`���C�N�J�n�֐�
  �����@�FnumEntry        �v������l�b�g���[�N�\���l���i�������܂ށj
          addFilter       �Q�[���Œǉ��������}�b�`���C�N����������B
                          ������ǉ����Ȃ��ꍇ��NULL��n���B
                          �����̓X�^���_�[�h��SQL�̏����ŏ������Ƃ��ł��܂��B
                          ���̂Ƃ���ݒ�ł��镶���񒷂͍ő�127�����ŁA
                          �f�o�b�O�r���h�ł͕������I�[�o�`�F�b�N�����Ă��܂��B
                          ������̓R�s�[���ă��C�u�������ŕێ����܂��B
          matchedCallback �}�b�`���C�N�����R�[���o�b�N
          matehedParam    ��L�R�[���o�b�N�p�p�����[�^
          evalCallback    �v���C���[�]���R�[���o�b�N
          evalParam       ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B�}�b�`���C�N�̌��ʂ��R�[���o�b�N�ŕԂ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�F�B���w�肹���ɁA�l���w��Ń��b�V���^�l�b�g���[�N���쐬����
 *---------------------------------------------------------------------------*/
// [arakit] main 051025
extern BOOL DWC_ConnectToAnybodyAsync(u8  numEntry,
                                      const char* addFilter,
                                      DWCMatchedCallback matchedCallback,
                                      void* matchedParam,
                                      DWCEvalPlayerCallback evalCallback,
                                      void* evalParam);


/*---------------------------------------------------------------------------*
  �F�B�w��s�A�}�b�`���C�N�J�n�֐�
  �����@�FfriendIdxList    �ڑ��v���F�B�C���f�b�N�X���X�g�B
                           NULL�Ȃ�F�B���X�g�S�Ă�ڑ��v���ΏۂƂ���B
          friendIdxListLen �ڑ��v���F�B�C���f�b�N�X���X�g��
          numEntry         �v������l�b�g���[�N�\���l���i�������܂ށj
          distantFriend    TRUE:�F�B�̗F�B�Ƃ̐ڑ��������AFALSE:�����Ȃ�
          matchedCallback  �}�b�`���C�N�����R�[���o�b�N
          matchedParam     ��L�R�[���o�b�N�p�p�����[�^
          evalCallback     �v���C���[�]���R�[���o�b�N
          evalParam        ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B�}�b�`���C�N�̌��ʂ��R�[���o�b�N�ŕԂ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�F�B���w�肵�Đڑ����A���b�V���^�l�b�g���[�N���쐬����
 *---------------------------------------------------------------------------*/
// [arakit] main 051025
extern BOOL DWC_ConnectToFriendsAsync(const u8 friendIdxList[],
                                      int friendIdxListLen,
                                      u8  numEntry,
                                      BOOL distantFriend,
                                      DWCMatchedCallback matchedCallback,
                                      void* matchedParam,
                                      DWCEvalPlayerCallback evalCallback,
                                      void* evalParam);


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�N���֐�
  �����@�FmaxEntry          �ő�ڑ��l���i�������܂ށj
          matchedCallback   �ڑ������R�[���o�b�N�B�P�l�ڑ����邲�ƂɌĂ΂��B
          matchedParam      ��L�R�[���o�b�N�p�p�����[�^
          newClientCallback �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N
          newClientParam    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B�}�b�`���C�N�̌��ʂ��R�[���o�b�N�ŕԂ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�T�[�o�𗧂ĂăN���C�A���g����̗v��������΁A�R�l�N�V������
          �ڑ��E�ؒf�������s���B
 *---------------------------------------------------------------------------*/
// [arakit] main 051025 051027
extern BOOL DWC_SetupGameServer(u8  maxEntry,
                                DWCMatchedSCCallback matchedCallback,
                                void* matchedParam,
                                DWCNewClientCallback newClientCallback,
                                void* newClientParam);

// ���e�𐳂����\���֐����ɕύX���܂������A����DWC_SetupGameServersAsync()���g���܂�
#define DWC_SetupGameServerAsync DWC_SetupGameServer
// [arakit] main 051027


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g�N���֐�
  �����@�FserverIndex       �ڑ���T�[�o�̗F�B���X�g�C���f�b�N�X
          matchedCallback   �ڑ������R�[���o�b�N�B�P�l�ڑ����邲�ƂɌĂ΂��B
          matchedParam      ��L�R�[���o�b�N�p�p�����[�^
          newClientCallback �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N
          newClientParam    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B�}�b�`���C�N�̌��ʂ��R�[���o�b�N�ŕԂ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�F�B���X�g�̃C���f�b�N�X�Őڑ�����T�[�o���w�肵�A�����֐ڑ�����B
          �܂��A�T�[�o�֐V���ɐڑ������N���C�A���g��
          ����΁A���̃N���C�A���g�Ƃ̐ڑ��������s���B
 *---------------------------------------------------------------------------*/
// [arakit] main 051025
extern BOOL DWC_ConnectToGameServerAsync(int serverIndex,
                                         DWCMatchedSCCallback matchedCallback,
                                         void* matchedParam,
                                         DWCNewClientCallback newClientCallback,
                                         void* newClientParam);


/*---------------------------------------------------------------------------*
  �R�l�N�V�����N���[�Y�R�[���o�b�N�ݒ�֐�
  �����@�Fcallback �R�l�N�V�����P���N���[�Y�����x�ɌĂяo�����R�[���o�b�N
          param    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�o�^����
          FALSE:FriendsMatch���C�u�����񓮍쒆�œo�^���s
  �p�r�@�F�R�l�N�V�����N���[�Y�R�[���o�b�N��ݒ肷��
 *---------------------------------------------------------------------------*/
extern BOOL DWC_SetConnectionClosedCallback(DWCConnectionClosedCallback callback, void* param);


/*---------------------------------------------------------------------------*
  �S�R�l�N�V���������N���[�Y�֐�
  �����@�F�Ȃ�
  �߂�l�F0�ȏ�Ȃ琬���A���̐��Ȃ玸�s�B
          0 :�N���[�Y���s�B
          1 :�ڑ��z�X�g��0�Ȃ̂ŁA�N���[�Y�����͍s�������A�R�[���o�b�N�͌Ă΂�Ȃ��B
             �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ�̂݁B
          -1:�ڑ�������łȂ��A�������̓G���[�������Ȃ̂ŉ������Ȃ������B
  �p�r�@�F�ڑ����̃R�l�N�V������S�ăN���[�Y����B
          �N���[�Y�����͖{�֐����Ŋ������A�{�֐��𔲂���O�ɁA
          �P�̃R�l�N�V�������N���[�Y���邲�ƂɁA
          DWC_SetConnectionClosedCallback()�Őݒ肵���R�[���o�b�N�֐���
          �Ăяo�����BWi-Fi�R�l�N�V�����ɂ̓��O�C�������܂܂ƂȂ�B
          ���̃N���[�Y�͑���z�X�g�ɂ��ʒm�����B
          �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�ɂ����āA���ɐڑ����̃z�X�g��
          �Ȃ��ꍇ�́A�I���������s�������ŁA�R�[���o�b�N�͌Ă΂�Ȃ��B
 *---------------------------------------------------------------------------*/
extern int  DWC_CloseAllConnectionsHard(void);


/*---------------------------------------------------------------------------*
  �R�l�N�V���������N���[�Y�֐�
  �����@�Faid �N���[�Y�������z�X�g��AID
  �߂�l�F0 :�N���[�Y���s�B
          -1:�ڑ�������łȂ��A�������̓G���[�������Ȃ̂ŉ������Ȃ������B
          -2:���ɃN���[�Y�ς݂�AID���w�肳�ꂽ�̂ŉ������Ȃ������B
  �p�r�@�F�w���AID�Ƃ̃R�l�N�V�����������N���[�Y����B
          �N���[�Y�����͖{�֐����Ŋ������A�{�֐��𔲂���O��
          DWC_SetConnectionClosedCallback()�Őݒ肵���R�[���o�b�N�֐���
          �Ăяo�����BWi-Fi�R�l�N�V�����ɂ̓��O�C�������܂܂ƂȂ�B
          ���̃N���[�Y�͑���z�X�g�ɂ��ʒm�����B
          �d����؂�Ȃǂ̗��R�ŒʐM�s�\�ɂȂ����z�X�g�ɑ΂��A�R�l�N�V������
          �N���[�Y����Ƃ����悤�ȁA�ُ��ԏ����̗p�r�ł݂̂��g�p�������B
 *---------------------------------------------------------------------------*/
extern int  DWC_CloseConnectionHard(u8 aid);


/*---------------------------------------------------------------------------*
  �r�b�g�}�b�v�w��R�l�N�V���������N���[�Y�֐�
  �����@�Fbitmap �N���[�Y�������z�X�g��AID�r�b�g�}�b�v�ւ̃|�C���^�B
                 ���ۂɃN���[�Y�ɐ�������AID�̃r�b�g�݂̂𗧂ĂĕԂ��B
                 �����̃r�b�g�͗����Ă��Ă��K�����낷�B
  �߂�l�F0 :�N���[�Y�J�n�B�N���[�Y��������������x�ɃR�[���o�b�N���Ă΂��B
          -1:�ڑ�������łȂ��A�������̓G���[�������Ȃ̂ŉ������Ȃ������B
          -2:�S�Ă�AID�����ɃN���[�Y�ς݂������̂ŉ������Ȃ������B
  �p�r�@�F�w���AID�Ƃ̃R�l�N�V�����������N���[�Y����B
          �N���[�Y�����͖{�֐����Ŋ������A�{�֐��𔲂���O��
          DWC_SetConnectionClosedCallback()�Őݒ肵���R�[���o�b�N�֐���
          �Ăяo�����BWi-Fi�R�l�N�V�����ɂ̓��O�C�������܂܂ƂȂ�B
          ���̃N���[�Y�͑���z�X�g�ɂ��ʒm�����B
          �d����؂�Ȃǂ̗��R�ŒʐM�s�\�ɂȂ����z�X�g�ɑ΂��A�R�l�N�V������
          �N���[�Y����Ƃ����悤�ȁA�ُ��ԏ����̗p�r�ł݂̂��g�p�������B
 *---------------------------------------------------------------------------*/
extern int  DWC_CloseConnectionHardBitmap(u32* bitmap);


/*---------------------------------------------------------------------------*
  �ڑ��z�X�g���擾�֐�
  �����@�F�Ȃ�
  �߂�l�F�l�b�g���[�N���\�����Ă���z�X�g���i�������܂ށj�B
          �I�t���C������0��Ԃ��B
  �p�r�@�F���b�V���^�l�b�g���[�N�\���z�X�g����Ԃ�
 *---------------------------------------------------------------------------*/
extern int DWC_GetNumConnectionHost(void);


/*---------------------------------------------------------------------------*
  ������AID�擾�֐�
  �����@�F�Ȃ�
  �߂�l�F������AID
  �p�r�@�F������AID�i���b�V���^�l�b�g���[�N���ŌŗL�̃z�X�gID�j��Ԃ�
 *---------------------------------------------------------------------------*/
extern u8  DWC_GetMyAID(void);


/*---------------------------------------------------------------------------*
  �ڑ����z�X�g��AID���X�g�擾�֐�
  �����@�FaidList AID���X�g�ւ̃|�C���^
  �߂�l�FAID���X�g���i�������܂ށj
  �p�r�@�FAID���X�g�ւ̃|�C���^���擾����i�R�s�[�͂��Ȃ��j�B
          AID���X�g�́A���ݗL����AID���O���珇�ɋl�߂�ꂽ���X�g�ŁA
          ���̗v�f���͎������܂߂��ڑ����z�X�g���ɂȂ�B
 *---------------------------------------------------------------------------*/
extern int DWC_GetAIDList(u8** aidList);


/*---------------------------------------------------------------------------*
  �ڑ����z�X�g��AID�r�b�g�}�b�v�擾�֐�
  �����@�F�Ȃ�
  �߂�l�FAID�r�b�g�}�b�v�B�I�t���C������0��Ԃ��B
  �p�r�@�F�ڑ����z�X�g��AID�r�b�g�}�b�v���擾����B
 *---------------------------------------------------------------------------*/
extern u32 DWC_GetAIDBitmap(void);


/*---------------------------------------------------------------------------*
  AID�L������֐�
  �����@�Faid AID
  �߂�l�FTRUE:AID���L���AFALSE:AID������
  �p�r�@�F�w�肳�ꂽAID���L�����ǂ����i�����ɐڑ�����Ă��邩�ǂ����j�𒲂ׂ�B
          ������AID���w�肵���ꍇ��FALSE��Ԃ��B
          �}�b�`���C�N���̐V�K�ڑ��N���C�A���g��AID�͖����Ɣ��肷��B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_IsValidAID(u8 aid);


/*---------------------------------------------------------------------------*
  FriendsMatch���C�u������Ԏ擾�֐�
  �����@�F�Ȃ�
  �߂�l�FDWCState�^�񋓎q
  �p�r�@�FFriendsMatch���C�u�����̏�����Ԃ��擾����
 *---------------------------------------------------------------------------*/
extern DWCState DWC_GetState(void);


/*---------------------------------------------------------------------------*
  �\�P�b�g�G���[�擾�֐�
  �����@�F�Ȃ�
  �߂�l�FGT2�\�P�b�g�G���[���
  �p�r�@�FGameSpy gt2 SDK �̃\�P�b�g�G���[��ʂ��擾����
 *---------------------------------------------------------------------------*/
extern int DWC_GetLastSocketError(void);


/*---------------------------------------------------------------------------*
  �S�R�l�N�V�����N���[�Y�֐�  (obsolete function)
  �����@�F�Ȃ�
  �߂�l�F0�ȏ�Ȃ琬���A���̐��Ȃ玸�s�B
          0 :�N���[�Y�J�n�B�N���[�Y����������������R�[���o�b�N���Ă΂��B
          1 :�ڑ��z�X�g��0�Ȃ̂ŁA�N���[�Y�����͍s�������A�R�[���o�b�N�͌Ă΂�Ȃ��B
             �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ�̂݁B
          -1:�ڑ�������łȂ��A�������̓G���[�������Ȃ̂ŉ������Ȃ������B
  �p�r�@�F�ڑ����̃R�l�N�V������S�ăN���[�Y����B
          �P�̃R�l�N�V�������N���[�Y���邲�ƂɁA
          DWC_SetConnectionClosedCallback()�Őݒ肵���R�[���o�b�N�֐���
          �Ăяo�����BWi-Fi�R�l�N�V�����ɂ̓��O�C�������܂܂ƂȂ�B
          ���̃N���[�Y�͑���z�X�g�ɂ��ʒm�����B
          �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�ɂ����āA���ɐڑ����̃z�X�g��
          �Ȃ��ꍇ�́A�I���������s�������ŁA�R�[���o�b�N�͌Ă΂�Ȃ��B
          ��DWC_CloseAllConnectionsHard()���g���ĉ�����
 *---------------------------------------------------------------------------*/
extern int DWC_CloseConnectionsAsync(void);


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
extern GT2Result DWCi_GT2Startup(void);


extern GT2Connection DWCi_GetGT2Connection(u8 aid);


extern u8  DWCi_GetConnectionAID(GT2Connection connection);


extern u8  DWCi_GetConnectionIndex(GT2Connection connection);


extern void* DWCi_GetConnectionUserData(GT2Connection connection);


extern int  DWCi_GT2GetConnectionListIdx(void);


extern void DWCi_ClearGT2ConnectionList(void);


extern GT2Connection* DWCi_GetGT2ConnectionByIdx(int index);


extern GT2Connection* DWCi_GetGT2ConnectionByProfileID(int profileID, int numHost);


extern DWCConnectionInfo* DWCi_GetConnectionInfoByIdx(int index);


extern BOOL DWCi_IsValidAID(u8 aid);


#ifdef __cplusplus
}
#endif


#endif // DWC_MAIN_H_

/*---------------------------------------------------------------------------*
  Project:  NitroDWC -  - 
  File:     dwc_friend.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_friend.h,v $
  Revision 1.32  2006/02/27 02:19:17  nakata
  Base64�֘A�̃��b�p�֐��̈����^���C���B

  Revision 1.31  2006/01/25 07:50:50  arakit
  �f�[�^�X�g���[�W�T�[�o�E���[�h�֐�DWC_LoadOwnPublicDataAsync(),
  DWC_LoadOwnPrivateDataAsync()��ǉ������B

  Revision 1.30  2006/01/23 04:50:43  arakit
  ����p��ɍ��킹�ăR�����g���C��

  Revision 1.29  2006/01/17 07:17:25  arakit
  �EDWCFriendStatusCallback�̐ݒ�݂̂��s���֐� DWC_SetFriendStatusCallback()
  �@���쐬�����B
  �EDWC_SetBuddyFriendCallback(), DWC_SetStorageServerCallback() �̖߂�l��
  �@void����BOOL�ɕύX���A����ϐ��m�ۑO�͉���������FALSE��Ԃ��悤�ɂ����B
  �EBase64�G���R�[�h�E�f�R�[�h�֐��iAuth�̃��b�v�֐��jDWC_Base64Encode(),
  �@DWC_Base64Decode()���쐬�����B

  Revision 1.28  2005/12/21 03:08:32  arakit
  ����\���̂̃����oplayerName�̃t�H�[�}�b�g�̕ύX�ɑΉ������B

  Revision 1.27  2005/11/07 02:54:59  sasakit
  GsStatus->OwnStatus�ɕύX�B

  Revision 1.26  2005/11/04 11:36:44  arakit
  DWCLoadFromServerCallback�̈���profileID���A�F�B���X�g�C���f�b�N�X�iindex�j�ɕύX�����B

  Revision 1.25  2005/11/04 05:44:59  arakit
  �X�y���~�X���C�������B

  Revision 1.24  2005/11/04 05:07:18  arakit
  Persistent�T�[�o�ւ̃��O�C���A���O�A�E�g�֐���ǉ����APersistent�T�[�o�ւ�
  �A�N�Z�X���\�ɂ����B

  Revision 1.23  2005/11/02 02:49:33  arakit
  �t�@�C���̃C���N���[�h�֌W�𒲐������B

  Revision 1.22  2005/10/28 09:11:05  arakit
  �֐������́u�`�ʐM���ɓˑR�d����؂����ꍇ�Ȃǂ�10�b�߂��O�̏�Ԃ��c��܂��v
  �Ƃ����R�����g�́A�����\���Ă��Ȃ��̂ŏC�������B

  Revision 1.21  2005/10/06 07:00:54  arakit
  �֐��̐��������኱�C��

  Revision 1.20  2005/10/03 04:05:04  arakit
  gp��TCP�ڑ����؂��Ȃ��悤�ɃL�[�v�A���C�u����̊֐��Ăяo����ǉ������B

  Revision 1.19  2005/10/01 07:14:07  arakit
  �F�B���X�g�ҏW�\��Ԏ擾�֐� BOOL DWC_CanChangeFriendList(void) ��ǉ������B

  Revision 1.18  2005/09/28 05:17:28  arakit
  �F�B�f�[�^���������A�o�f�B�֌W����������֐�DWC_DeleteBuddyFriendData()���쐬�����B

  Revision 1.17  2005/09/27 13:01:42  arakit
  �g��Ȃ��Ȃ����萔���폜�����B

  Revision 1.16  2005/09/20 10:03:40  sasakit
  gpSendBuddyRequest()�̕ԓ�������dwc_friend.c�Ɉڂ����B

  Revision 1.15  2005/09/16 07:26:40  arakit
  �R�[���o�b�N�̈�������ύX

  Revision 1.14  2005/09/13 11:12:22  arakit
  �EMP�F�B�f�[�^����ProfileSearch���s�������ɁA���肪������Ȃ������ꍇ��
  �@�s����C�������B
  �EDWC_UpdateServersAsync()�������ȊO�Ƀo�f�B����������Ԃ��R�[���o�b�N��
  �@�ݒ�ł���悤�ɂ����B

  Revision 1.13  2005/09/12 07:50:25  arakit
  �Egp��Alloc�΍�Ƃ��āAProfileSearch���P�l���s���悤�ɂ����B
  �E�I�����C�����Ƀo�f�B�֌W�̐�������������悤�A�C�������B
  �E�F�B���X�g�����R�[���o�b�N�̈����ɁA�����ꂽ���̂Ɠ����Ɣ��肳�ꂽ�F�B�f�[�^�́A
  �@�F�B���X�g�C���f�b�N�X���n���悤�ɂ����B

  Revision 1.12  2005/09/09 09:54:09  arakit
  �o�[�W�����Ⴂ�̃��C�u�������m�Ń}�b�`���O���Ȃ��悤�ɕύX�����B

  Revision 1.11  2005/09/05 11:43:31  arakit
  �EPersistent�T�[�o���[�h�֐�DWC_LoadOthersDataAsync()�̈���profaileID��
  �@�F�B���X�g�C���f�b�N�X�ɕύX�����B
  �EgpStatus������̂����A���C�u������locationString�A�Q�[����statusString��
  �@�g�p����悤�ɂ��Ă������AstatusString�̓f�t�H���g��"Offline"�̕�����
  �@�����Ă��܂��̂ŁA���C�u������statusString�A�Q�[����locationString��
  �@�g�p����悤�ɓ���ւ����B
  �E�o�C�i���f�[�^���e�L�X�g�G���R�[�h����gpStatus��locationString��
  �@�Z�b�g����֐�DWC_SetGsStatusData()�A���̃f�[�^���G���R�[�h���ēǂފ֐�
  �@DWC_GetGsStatusData()�ƁA�F�B��Status�o�C�i���f�[�^��ǂފ֐�
  �@DWC_GetFriendStatusData(), DWC_GetFriendStatusDataSC()��ǉ������B

  Revision 1.10  2005/09/03 13:03:35  arakit
  �EfriendList����n�O���֐� DWC_SetFriendDataToList(), DWC_SetGsProfileIDToList(),
  �@DWC_DeleteFriendFromList()�����������B
  �EGameSpy�v���t�@�C��ID�𒼐ڈ����֐� DWC_GetProfileIDFromList(),
  �@DWC_GetFriendListIndex() ������֐��ɕύX�����B
  �EDWC_GetFriendStatus(), DWC_GetFriendStatusSC() �̈������A�u�F�B���X�g�ւ�
  �@�|�C���^�{�F�B���X�g���v����A�u�F�B�f�[�^�ւ̃|�C���^�v�ɕύX�����B
  �E�F�B���X�g�����X�V���ɗF�B�f�[�^����������Ă��A���X�g��O�l�߂��Ȃ��悤��
  �@�ύX�����B����ɔ����A�O�l�߂�O��Ƃ��Ă����������C�������B
  �EMP�����F�B�f�[�^����F�B��GameSpy�v���t�@�C��ID���擾����ۂɁA
  �@�F�B���܂���x�����O�C�����Ă��Ȃ��������ɕs����������Ă����̂��C�������B
  �E�o�f�B���N�G�X�g��M���̏��F�������A�ŏI�d�l�ɍ��킹���B

  Revision 1.9  2005/08/29 06:35:56  arakit
  �EgpStatus��statusString���Q�[�����ŁAlocationString�����C�u��������
  �ǂݏ����ł���悤�ɂ����B
  �E�T�[�o�N���C�A���g�^�}�b�`���O�ŁA�ő�ڑ��l���A���ݐڑ��l����gpStatus��
  locationString�ɃZ�b�g����悤�ɂ��A���̒l���Q�Ƃ��邽�߂̊֐���p�ӂ����B

  Revision 1.8  2005/08/26 08:15:22  arakit
  �EASSERT�ƃ��W���[������ϐ��̃`�F�b�N�𐮗������B
  �E�O���֐�DWC_ShutdownFriendsMatch()�̈������Ƃ�Ȃ��悤�ɕύX�����B
  �܂��A���֐�����e���W���[���̐���ϐ���NULL�N���A����悤�ɂ����B
  �E�G���[�����֐���ύX���A�G���[�Z�b�g�A�G���[�`�F�b�N��K�v�ȉӏ��ɓ��ꂽ�B

  Revision 1.7  2005/08/23 13:39:20  arakit
  �E�O���֐�DWC_HasProflieIDInList(), DWC_HasOnlyMPFriendKeyInList()�����������B
  �E�O���֐�DWC_GetProfileIDFromfList()�ƈꕔ�����֐����A���[�U�f�[�^��������
  ���悤�ɕύX�����B

  Revision 1.6  2005/08/20 07:01:19  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_FRIEND_H_
#define DWC_FRIEND_H_


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
// GameSpy Status������ő咷�iNULL�I�[�܂ށj�B256�����B
#define DWC_GS_STATUS_STRING_LEN GP_LOCATION_STRING_LEN

// DWC���䉺��Persistet�f�[�^�̃Z�[�u�E���[�h�ł��邱�Ƃ�\���t���O
#define DWC_PERS_KEY_ID_MASK         0x80000000
#define DWC_PERS_KEY_ID_PLAYER_NAME  (DWC_PERS_KEY_ID_MASK | 0x01)

// gpProcess()�Ăяo���Ԋu�i�P�ʁFmsec�j
#define DWC_GP_PROCESS_INTERVAL      300

// �F�B���X�g�����������J�n����܂ł�gpProcess()�Ăяo����
#define DWC_FRIEND_UPDATE_WAIT_COUNT 7    // 300�~7 = 2.1sec

// GP�L�[�v�A���C�u���MgpProcess()�Ăяo����
#define DWC_GP_KEEP_ALIVE_COUNT      256  // 300�~256 = 76.8sec

// gpStatus��statusString�ɐݒ肷��Akey������i�K���R�����j
#define DWC_GP_SSTR_KEY_MATCH_VERSION    "VER"  // �}�b�`���C�N�̃v���g�R���o�[�W����
#define DWC_GP_SSTR_KEY_MATCH_FRIEND_NUM "FME"  // �F�B�w��s�A�}�b�`���C�N�̊�]�ڑ��l��
#define DWC_GP_SSTR_KEY_DISTANT_FRIEND   "MDF"  // �F�B�w��s�A�}�b�`���C�N�ŗF�B�̗F�B���������ǂ���
#define DWC_GP_SSTR_KEY_MATCH_SC_MAX     "SCM"  // �T�[�o�N���C�A���g�}�b�`���C�N�̍ő�ڑ��l��
#define DWC_GP_SSTR_KEY_MATCH_SC_NUM     "SCN"  // �T�[�o�N���C�A���g�}�b�`���C�N�̌��ݐڑ��l��

// gpStatus��status��ύX���Ȃ����Ƃ�\��status�l
#define DWC_GP_STATUS_NO_CHANGE -1

    
//----------------------------------------------------------------------------
// enum
//----------------------------------------------------------------------------
// �F�B�̒ʐM��ԁiGPEnum��Status�{�����Ē�`���Ďg�p�j
enum {
    DWC_STATUS_OFFLINE = 0,    // �I�t���C��
    DWC_STATUS_ONLINE,         // �I�����C���iGP�T�[�o�Ƀ��O�C���ς݁j
    DWC_STATUS_PLAYING,        // �Q�[���v���C��
    DWC_STATUS_MATCH_ANYBODY,  // �F�B���w��s�A�}�b�`���C�N��
    DWC_STATUS_MATCH_FRIEND,   // �F�B�w��s�A�}�b�`���C�N��
    DWC_STATUS_MATCH_SC_CL,    // �T�[�o�N���C�A���g�}�b�`���C�N���̃N���C�A���g
    DWC_STATUS_MATCH_SC_SV,    // �T�[�o�N���C�A���g�}�b�`���C�N���̃T�[�o
    DWC_STATUS_NUM
};
    
// �F�B�Ǘ��������
typedef enum {
    DWC_FRIEND_STATE_INIT = 0,      // �������
    DWC_FRIEND_STATE_PERS_LOGIN,    // Persistent�T�[�o���O�C�����F�B���X�g����������
    DWC_FRIEND_STATE_LOGON,         // Persistent�T�[�o���O�C����
    DWC_FRIEND_STATE_NUM
} DWCFriendState;

// ���[�J���F�B���X�g��GP�o�f�B���X�g�Ƃ̓����������
enum {
    DWC_BUDDY_UPDATE_STATE_WAIT = 0,  // �o�f�B���X�g�_�E�����[�h�����҂�
    DWC_BUDDY_UPDATE_STATE_CHECK,     // �o�f�B���X�g�`�F�b�N��
    DWC_BUDDY_UPDATE_STATE_PSEARCH,   // �v���t�@�C���T�[�`��
    DWC_BUDDY_UPDATE_STATE_COMPLETE,  // �o�f�B���X�g������������
    DWC_BUDDY_UPDATE_STATE_NUM
};

// Persistent�T�[�o���O�C�����
enum {
    DWC_PERS_STATE_INIT = 0,   // �������
    DWC_PERS_STATE_LOGIN,      // ���O�C����
    DWC_PERS_STATE_CONNECTED,  // �ڑ����
    DWC_PERS_STATE_NUM
};


//----------------------------------------------------------------------------
// typedef - function
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �F�B���X�g���������R�[���o�b�N�^
  �����@�Ferror     DWC�G���[���
          isChanged TRUE :�����������ɗF�B���X�g������������ꂽ�A
                    FALSE:�F�B���X�g�̏��������Ȃ�
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
 *---------------------------------------------------------------------------*/
typedef void (*DWCUpdateServersCallback)(DWCError error, BOOL isChanged, void* param);

/*---------------------------------------------------------------------------*
  �F�B��ԕω��ʒm�R�[���o�b�N�^
  �����@�Findex        ��Ԃ��ω������F�B�̗F�B���X�g�C���f�b�N�X
          status       �F�B�̏�ԁBDWC_STATUS_* �̗񋓎q�B
          statusString �Q�[���Œ�`����GameSpy status������B
                       ������͍ő�255�����{NULL�I�[�B
          param  �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
 *---------------------------------------------------------------------------*/
typedef void (*DWCFriendStatusCallback)(int index, u8 status, const char* statusString, void* param);

/*---------------------------------------------------------------------------*
  �F�B���X�g�폜�R�[���o�b�N�^
  �����@�FdeletedIndex �폜���ꂽ�F�B�̗F�B���X�g�C���f�b�N�X
          srcIndex     �����F�B���Ɣ��肳�ꂽ�F�B���X�g�C���f�b�N�X
          param        �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  DWC_UpdateServersAsync()�̏������ɁA�F�B���X�g���ɓ����F�B���������A
  ���C�u�������ŏ����������ɌĂ΂��
 *---------------------------------------------------------------------------*/
typedef void (*DWCDeleteFriendListCallback)(int deletedIndex, int srcIndex, void* param);

/*---------------------------------------------------------------------------*
  �o�f�B�����R�[���o�b�N�^
  �����@�Findex GP�T�[�o��Ńo�f�B�֌W�����������F�B�́A�F�B���X�g�C���f�b�N�X
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
 *---------------------------------------------------------------------------*/
typedef void (*DWCBuddyFriendCallback)(int index, void* param);

/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o�E���O�C���R�[���o�b�N�^
  �����@�Ferror DWC�G���[���
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
 *---------------------------------------------------------------------------*/
typedef void (*DWCStorageLoginCallback)(DWCError error, void* param);

/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o�E�Z�[�u�����R�[���o�b�N�^
  �����@�Fsuccess  TRUE:�Z�[�u�����AFALSE:�Z�[�u���s
          isPublic TRUE:�Z�[�u�����f�[�^���p�u���b�N�f�[�^�AFALSE:�v���C�x�[�g�f�[�^
          param  �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
 *---------------------------------------------------------------------------*/
typedef void (*DWCSaveToServerCallback)(BOOL success, BOOL isPublic, void* param);

/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o�E���[�h�����R�[���o�b�N�^
  �����@�Fsuccess   TRUE:���[�h�����AFALSE:���[�h���s
          index     ���[�h���f�[�^�ێ��҂̗F�B���X�g�C���f�b�N�X�B
                    �����̏ꍇ�ƁA���[�h�����O�ɗF�B�łȂ��Ȃ����ꍇ��-1�ɂȂ�B
          data      ���[�h�����f�[�^�ikey/value�g�̕�����j
          len       ���[�h�f�[�^��
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  ���[�h�֐��Ŏw�肵���L�[�����݂��Ȃ������ꍇ��A���l��DWC_SavePrivateDataAsync()
  �ŃZ�[�u�����L�[��ǂ����Ƃ����ꍇ�� success = FALSE �ɂȂ�B
 *---------------------------------------------------------------------------*/
typedef void (*DWCLoadFromServerCallback)(BOOL success, int index, char* data, int len, void* param);


//----------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------
// �F�B���X�g�p���p��
#define DWC_FRIEND_DATA_SIZE 16  // �F�B���X�g�T�C�Y�i�P�ʁF�o�C�g�j�i���͎g���Ă��Ȃ��j
typedef union DWCunFriendDataOld
{
    u8  mpFriendKey[DWC_FRIEND_DATA_SIZE];  // MP�F�B������
    struct {
        int id;                             // �v���t�@�C��ID�i��Í�����ԁj
        u8  pad[DWC_FRIEND_DATA_SIZE-sizeof(int)];  // �p�f�B���O
    } profile;
    u8  flags[DWC_FRIEND_DATA_SIZE];  // �t���O�Q�Ɨp�i[15]�̏��2bit���t���O�j
} DWCFriendDataOld;

typedef struct DWCstFriendControl
{
    DWCFriendState state;    // �F�B�Ǘ��������
        
    GPConnection* pGpObj;    // gp�R�l�N�V�����\���̃|�C���^�ւ̃|�C���^
    u32 gpProcessCount;      // gpProcess�Ăяo����
    OSTick lastGpProcess;    // gpProcess���Ō�ɌĂяo��������
        
    int friendListLen;       // �F�B���X�g�ő咷
    DWCFriendData* friendList;  // �F�B���X�g

    u8  buddyUpdateIdx;      // �F�B���X�g�����������C���f�b�N�X
    u8  friendListChanged;   // 1:�t�����h���X�g�ɕύX���������A0:�Ȃ�����
    u8  buddyUpdateState;    // �F�B���X�g����������ԁBDWC_BUDDY_UPDATE_STATE_*�񋓎q�Œ�`�B
    vu8 svUpdateComplete;    // �F�B���X�g�������������t���O�B2�œ�����������
    u32 persCallbackLevel;   // Persistent�f�[�^�Z�[�u�E���[�h�R�[���o�b�N�҂�����
    int profileID;           // �����̃v���t�@�C��ID
    const u16* playerName;   // ���̃��[�U���Q�Ƃ��鎩���̃v���C���[���ւ̃|�C���^

    DWCUpdateServersCallback updateCallback;     // �F�B���X�g�������������R�[���o�b�N
    void* updateParam;       // ��L�R�[���o�b�N�p�p�����[�^
    DWCFriendStatusCallback statusCallback;      // �F�B��ԕω��ʒm�R�[���o�b�N
    void* statusParam;       // ��L�R�[���o�b�N�p�p�����[�^
    DWCDeleteFriendListCallback deleteCallback;  // �F�B���X�g�폜�R�[���o�b�N
    void* deleteParam;       // ��L�R�[���o�b�N�p�p�����[�^
    DWCBuddyFriendCallback buddyCallback;        // �o�f�B�֌W�����R�[���o�b�N
    void* buddyParam;        // ��L�R�[���o�b�N�p�p�����[�^
    DWCStorageLoginCallback persLoginCallback;   // Persistent�T�[�o���O�C�������ʒm�R�[���o�b�N
    void* persLoginParam;    // ��L�R�[���o�b�N�p�p�����[�^
    DWCSaveToServerCallback saveCallback;        // Persistent�T�[�o�Z�[�u�R�[���o�b�N
    DWCLoadFromServerCallback loadCallback;      // Persistent�T�[�o���[�h�R�[���o�b�N
} DWCFriendControl;


//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �F�B�ʐM��Ԏ擾�֐�
  �����@�FfriendData   �F�B���ւ̃|�C���^
          statusString �Q�[���Œ�`����GameSpy status������i�[��|�C���^�B
                       NULL��n���Ε�����̃R�s�[�͍s��Ȃ��B
                       ������͍ő�255�����{NULL�I�[�B
  �߂�l�F�ʐM���enum�l�idwc_friend.h ��DWC_STATUS_*�j�B
          �������I�t���C���̎��ȂǂɌĂяo����DWC_STATUS_OFFLINE�����Ԃ��Ȃ��B
          �܂����肪�܂�GP�̃o�f�B���X�g�ɓo�^����Ă��Ȃ�����
          DWC_STATUS_OFFLINE�Ԃ��B
  �p�r�@�F���[�J���ɃL���b�V�����ꂽ�A�w��̗F�B�̒ʐM��Ԃ��擾����B
  �@�@�@�@�ʐM�͍s��Ȃ��B
          �F�B�̒ʐM��Ԃ́A�ω�������x��GP�T�[�o����ʒm�����B
          �������A�ʐM���ɓˑR�d����؂����ꍇ�Ȃǂ͐����Ԃ͑O�̏�Ԃ��c��B
 *---------------------------------------------------------------------------*/
extern u8   DWC_GetFriendStatus(const DWCFriendData* friendData, char* statusString);


/*---------------------------------------------------------------------------*
  �F�B�ʐM��Ԏ擾�֐��i�T�[�o�N���C�A���g�}�b�`���C�N�Q�[���p�j
  �����@�FfriendData   �F�B���ւ̃|�C���^
          maxEntry     �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂Ă�
                       ����ꍇ�̍ő�ڑ��l���B
                       �i�[��|�C���^�BNULL��n���Ύ擾���Ȃ��B
          numEntry     �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂Ă�
                       ����ꍇ�̌��ݐڑ��l���B
                       �i�[��|�C���^�BNULL��n���Ύ擾���Ȃ��B
          statusString �Q�[���Œ�`����GameSpy status������i�[��|�C���^�B
                       NULL��n���Ε�����̃R�s�[�͍s��Ȃ��B
                       ������͍ő�255�����{NULL�I�[�B
  �߂�l�F�ʐM���enum�l�idwc_friend.h ��DWC_STATUS_*�j�B
          �������I�t���C���̎��ȂǂɌĂяo����DWC_STATUS_OFFLINE�����Ԃ��Ȃ��B
          �܂����肪�܂�GP�̃o�f�B���X�g�ɓo�^����Ă��Ȃ�����
          DWC_STATUS_OFFLINE�Ԃ��B
  �p�r�@�F���[�J���ɃL���b�V�����ꂽ�A�w��̗F�B�̒ʐM��Ԃ��擾����B
  �@�@�@�@�ʐM�͍s��Ȃ��B
          �F�B�̒ʐM��Ԃ́A�ω�������x��GP�T�[�o����ʒm�����B
          �������A�ʐM���ɓˑR�d����؂����ꍇ�Ȃǂ͐����Ԃ͑O�̏�Ԃ��c��B
 *---------------------------------------------------------------------------*/
extern u8   DWC_GetFriendStatusSC(const DWCFriendData* friendData, u8* maxEntry, u8* numEntry, char* statusString);


/*---------------------------------------------------------------------------*
  �F�B�ʐM��Ԏ擾�֐��istatusData�Łj
  �����@�FfriendData �F�B���ւ̃|�C���^
          statusData �Q�[���Œ�`����GameSpy status�f�[�^�i�[��|�C���^�B
                     NULL��n���΃f�[�^�̃R�s�[�͍s��Ȃ��B
          size       �Q�[���Œ�`����GameSpy status�f�[�^���̊i�[��|�C���^�B
                     �����ȃf�[�^�̏ꍇ��-1���i�[�����B
  �߂�l�F�ʐM���enum�l�idwc_friend.h ��DWC_STATUS_*�j�B
          �������I�t���C���̎��ȂǂɌĂяo����DWC_STATUS_OFFLINE�����Ԃ��Ȃ��B
          �܂����肪�܂�GP�̃o�f�B���X�g�ɓo�^����Ă��Ȃ�����
          DWC_STATUS_OFFLINE�Ԃ��B
  �p�r�@�F���[�J���ɃL���b�V�����ꂽ�A�w��̗F�B�̒ʐM��Ԃ��擾����B
  �@�@�@�@�ʐM�͍s��Ȃ��B
          �F�B�̒ʐM��Ԃ́A�ω�������x��GP�T�[�o����ʒm�����B
          �������A�ʐM���ɓˑR�d����؂����ꍇ�Ȃǂ͐����Ԃ͑O�̏�Ԃ��c��B
          �߂�l��DWC_STATUS_OFFLINE�̏ꍇ�́A*size = -1 ���Z�b�g����ȊO��
          �l���Z�b�g�����ɕԂ��܂��B
 *---------------------------------------------------------------------------*/
extern u8 DWC_GetFriendStatusData(const DWCFriendData* friendData, char* statusData, int* size);


/*---------------------------------------------------------------------------*
  �F�B�ʐM��Ԏ擾�֐��i�T�[�o�N���C�A���g�}�b�`���C�N�Q�[���pstatusData�Łj
  �����@�FfriendData �F�B���ւ̃|�C���^
          maxEntry   �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂Ă�
                     ����ꍇ�̍ő�ڑ��l���B
                     �i�[��|�C���^�BNULL��n���Ύ擾���Ȃ��B
          numEntry   �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂Ă�
                     ����ꍇ�̌��ݐڑ��l���B
                     �i�[��|�C���^�BNULL��n���Ύ擾���Ȃ��B
          statusData �Q�[���Œ�`����GameSpy status������i�[��|�C���^�B
                     NULL��n���Ε�����̃R�s�[�͍s��Ȃ��B
          size       �Q�[���Œ�`����GameSpy status�f�[�^���̊i�[��|�C���^�B
                     �����ȃf�[�^�̏ꍇ��-1���i�[�����B
  �߂�l�F�ʐM���enum�l�idwc_friend.h ��DWC_STATUS_*�j�B
          �������I�t���C���̎��ȂǂɌĂяo����DWC_STATUS_OFFLINE�����Ԃ��Ȃ��B
          �܂����肪�܂�GP�̃o�f�B���X�g�ɓo�^����Ă��Ȃ�����
          DWC_STATUS_OFFLINE�Ԃ��B
  �p�r�@�F���[�J���ɃL���b�V�����ꂽ�A�w��̗F�B�̒ʐM��Ԃ��擾����B
  �@�@�@�@�ʐM�͍s��Ȃ��B
          �F�B�̒ʐM��Ԃ́A�ω�������x��GP�T�[�o����ʒm�����B
          �������A�ʐM���ɓˑR�d����؂����ꍇ�Ȃǂ͐����Ԃ͑O�̏�Ԃ��c��B
          �߂�l��DWC_STATUS_OFFLINE�̏ꍇ�́A*size = -1 ���Z�b�g����ȊO��
          �l���Z�b�g�����ɕԂ��܂��B
 *---------------------------------------------------------------------------*/
extern u8 DWC_GetFriendStatusDataSC(const DWCFriendData* friendData, u8* maxEntry, u8* numEntry, char* statusData, int* size);


/*---------------------------------------------------------------------------*
  �F�B���擾�֐�
  �����@�FfriendList    �F�B���X�g
          friendListLen �F�B���X�g�̍ő咷�i�v�f���j
  �߂�l�F�F�B���X�g�ɓo�^����Ă���F�B�̐�
  �p�r�@�F���[�J���̗F�B���X�g���Q�Ƃ��āA�F�B�o�^����Ԃ��B�ʐM�͍s��Ȃ��B
 *---------------------------------------------------------------------------*/
extern int  DWC_GetNumFriend(const DWCFriendData friendList[], int friendListLen);


/*---------------------------------------------------------------------------*
  GameSpy Status������Z�b�g�֐�
  �����@�FstatusString GameSpy��Status�ɃZ�b�g���镶����B255�����{NULL�I�[�܂ŁB
  �߂�l�FTRUE :�Z�b�g����
          FALSE:���O�C���O�ȂǂŃZ�b�g���s
  �p�r�@�F������GameSpy Status�ɕ�������Z�b�g����B
          �Z�b�g���ꂽ������́A���̃z�X�g�����DWC_GetFriendStatus()��
          �擾���邱�Ƃ��ł���B
          '/'��'\\'�͎��ʕ����Ƃ��ă��C�u�������g���̂ŁA�����񒆂�
          �g�p���Ȃ��ŉ������B
          �Z�b�g���ꂽ������́ADWC_ShutdownFriendsMatch()���Ă΂���
          �N���A����܂��B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_SetOwnStatusString(const char* statusString);


/*---------------------------------------------------------------------------*
  GameSpy Status������擾�֐�
  �����@�FstatusString GameSpy��Status������i�[��|�C���^�B255�����{NULL�I�[�܂ŁB
  �߂�l�FTRUE :�擾�����AFALSE:���O�C���O�ȂǂŎ擾���s
  �p�r�@�F���݂̎�����GameSpy Status��������擾����B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_GetOwnStatusString(char* statusString);


/*---------------------------------------------------------------------------*
  GameSpy Status�f�[�^�Z�b�g�֐�
  �����@�FstatusData GameSpy��Status�ɃZ�b�g����o�C�i���f�[�^�ւ̃|�C���^�B
          size       �o�C�i���f�[�^�̃T�C�Y�B189Byte�܂ŁB
  �߂�l�FTRUE :�Z�b�g����
          FALSE:���O�C���O�A�e�ʃI�[�o�[�ȂǂŃZ�b�g���s
  �p�r�@�F������GameSpy Status�Ƀo�C�i���f�[�^��Base64�Ńe�L�X�g������
          �Z�b�g����B
          �Z�b�g���ꂽ�f�[�^�́A���̃z�X�g����DWC_GetFriendStatusData()��
          �擾���邱�Ƃ��ł���B
          �Z�b�g���ꂽ�f�[�^�́ADWC_ShutdownFriendsMatch()���Ă΂���
          �N���A����܂��B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_SetOwnStatusData(const char* statusData, u32 size);


/*---------------------------------------------------------------------------*
  GameSpy Status�f�[�^�擾�֐�
  �����@�FstatusData GameSpy��Status�f�[�^�i�[��|�C���^
  �߂�l�F-1:���O�C���O�ȂǂŎ擾���s�A
          �񕉐���:�擾�����o�C�i���f�[�^�̃T�C�Y
  �p�r�@�F���݂̎�����GameSpy Status�f�[�^���擾����B
          ����statusData��NULL��n���ƁA�o�C�i���f�[�^�̃T�C�Y�̂ݎ擾����B
 *---------------------------------------------------------------------------*/
extern int DWC_GetOwnStatusData(char* statusData);


/*---------------------------------------------------------------------------*
  [obsolute functions]
  �݊����̂��߂ɁA�Â��֐����C�����C���Ŏc���Ă����B
 *---------------------------------------------------------------------------*/
inline BOOL DWC_SetGsStatusString(const char* statusString)
{
    return DWC_SetOwnStatusString( statusString );
}
inline BOOL DWC_GetGsStatusString(char* statusString)
{
    return DWC_GetOwnStatusString( statusString );
}
inline BOOL DWC_SetGsStatusData(const char* statusData, u32 size)
{
    return DWC_SetOwnStatusData( statusData, size );
}
inline int DWC_GetGsStatusData(char* statusData)
{
    return DWC_GetOwnStatusData( statusData );
}

/*---------------------------------------------------------------------------*
  �F�B���X�g�ҏW�\��Ԏ擾�֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:�F�B���X�g��ҏW���ėǂ��AFALSE:�F�B���X�g��ҏW���Ă͂����Ȃ�
  �p�r�@�F�F�B���X�g��ҏW���Ă��ǂ��^�C�~���O���ǂ����𒲂ׂ�B
          �����ł̖߂�l�́A����DWC_ProcessFriendsMatch()���Ă΂��܂ŗL���B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_CanChangeFriendList(void);


/*---------------------------------------------------------------------------*
  �F�B��񊮑S�����֐�
  �����@�FfriendData �F�B���ւ̃|�C���^
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B�����[���N���A���AGP�T�[�o��̃o�f�B�֌W����������B
 *---------------------------------------------------------------------------*/
extern void DWC_DeleteBuddyFriendData(DWCFriendData* friendData);


/*---------------------------------------------------------------------------*
  �o�f�B�����R�[���o�b�N�o�^�֐�
  �����@�Fcallback �o�f�B�����R�[���o�b�N
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�o�^����
          FALSE:FriendsMatch���C�u�����񓮍쒆�œo�^���s
  �p�r�@�F���[�J���̗F�B���X�g�ɓo�^���ꂽ�F�B���ADWC_UpdateServersAsync()
          �ďo�����ȊO�̃^�C�~���O�ŁAGP�T�[�o��ł��o�f�B�Ƃ���
          �F�߂�ꂽ���ɌĂяo�����R�[���o�b�N��o�^����
 *---------------------------------------------------------------------------*/
extern BOOL DWC_SetBuddyFriendCallback(DWCBuddyFriendCallback callback, void* param);


/*---------------------------------------------------------------------------*
  �F�B��ԕω��ʒm�R�[���o�b�N�o�^�֐�
  �����@�Fcallback �F�B��ԕω��ʒm�R�[���o�b�N
          param    �R�[���o�b�N�p�p�����[�^�BNULL��n���Ή����Z�b�g���Ȃ��B
                   �R�[���o�b�N�p�p�����[�^�̃f�t�H���g�l��NULL�B
  �߂�l�FTRUE :�o�^����
          FALSE:FriendsMatch���C�u�����񓮍쒆�œo�^���s
  �p�r�@�F�F�B�̒ʐM��Ԃ��ω��������ɒʒm�����R�[���o�b�N��o�^����B
          �ʏ��DWC_UpdateServersAsync()�œo�^�ł��邪�A
          DWC_UpdateServersAsync()���g��Ȃ����F�B�̏�Ԃ͎擾�������Ƃ���
          �ꍇ�ɖ{�֐���p����B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_SetFriendStatusCallback(DWCFriendStatusCallback callback, void* param);


/*---------------------------------------------------------------------------*
  Base64�G���R�[�h�֐�
  �����@�Fsrc    �ϊ����f�[�^�ւ̃|�C���^
          srclen �ϊ����f�[�^��
          dst    �ϊ���f�[�^�i�[�p�o�b�t�@�ւ̃|�C���^�B
                 NULL���w�肷��Εϊ���ɕK�v�ȍő�o�b�t�@�T�C�Y��Ԃ������B
          dstlen �ϊ���f�[�^�i�[�p�o�b�t�@�̃T�C�Y�i�����񒷁j
  �߂�l�F-1      :�ϊ����s�B�K�v�ȕϊ���o�b�t�@�T�C�Y��dstlen���z���Ă���B
          ����ȊO:�ϊ����ʂ̕�����
  �p�r�@�F�^����ꂽ�f�[�^��Base64�ŃG���R�[�h���Achar�^�̕�����Ƃ��ďo�͂���
 *---------------------------------------------------------------------------*/
extern int DWC_Base64Encode(const char *src, const u32 srclen, char *dst, const u32 dstlen);


/*---------------------------------------------------------------------------*
  Base64�f�R�[�h�֐�
  �����@�Fsrc    Base64�f�[�^�ւ̃|�C���^
          srclen Base64�f�[�^���i�����񒷁j
          dst    �ϊ���f�[�^�i�[�p�o�b�t�@�ւ̃|�C���^�B
                 NULL���w�肷��Εϊ���ɕK�v�ȍő�o�b�t�@�T�C�Y��Ԃ������B
          dstlen �ϊ���f�[�^�i�[�p�o�b�t�@�̃T�C�Y
  �߂�l�F-1      :�ϊ����s�B�K�v�ȕϊ���o�b�t�@�T�C�Y��dstlen���z���Ă���B
          ����ȊO:�ϊ����ʂ̃f�[�^�T�C�Y
  �p�r�@�F�^����ꂽBase64�f�[�^���f�R�[�h���Achar�^�̃f�[�^�Ƃ��ďo�͂���
 *---------------------------------------------------------------------------*/
extern int DWC_Base64Decode(const char *src, const u32 srclen, char *dst, const u32 dstlen);


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o�ւ̃��O�C���֐�
  �����@�Fcallback ���O�C�������ʒm�R�[���o�b�N
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :���O�C���J�n�B���������s�ŃR�[���o�b�N���Ԃ��Ă���B
          FALSE:DWC_LoginAsync()�����O�A���Ƀ��O�C���ς݂ȂǂŃ��O�C����
                �J�n���Ȃ������B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�FGameSpy�f�[�^�X�g���[�W�T�[�o�ւ̃��O�C�����s�Ȃ��B
          ���O�C�����Ȃ��ƃf�[�^�X�g���[�W�T�[�o�ւ̃Z�[�u�E���[�h�͂ł��Ȃ��B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_LoginToStorageServerAsync(DWCStorageLoginCallback callback, void* param);


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o����̃��O�A�E�g�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FGameSpy�f�[�^�X�g���[�W�T�[�o���烍�O�A�E�g���A
          ����I�u�W�F�N�g���������B
          ���̊֐����Ă΂Ȃ��Ă��ADWC_ShutdownFriendsMatch()���Ăׂ΁A
          ���O�A�E�g�������s�Ȃ���B
 *---------------------------------------------------------------------------*/
extern void DWC_LogoutFromStorageServer(void);


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o �f�[�^�Z�[�u�E���[�h�����ʒm�R�[���o�b�N�o�^�֐�
  �����@�FsaveCallback �Z�[�u�����ʒm�R�[���o�b�N
          loadCallback ���[�h�����ʒm�R�[���o�b�N
  �߂�l�FTRUE :�o�^����
          FALSE:FriendsMatch���C�u�����񓮍쒆�œo�^���s
  �p�r�@�F�f�[�^�X�g���[�W�ւ̃Z�[�u�E���[�h�������������ɌĂяo�����
          �R�[���o�b�N��o�^����
 *---------------------------------------------------------------------------*/
extern BOOL DWC_SetStorageServerCallback(DWCSaveToServerCallback saveCallback,
                                         DWCLoadFromServerCallback loadCallback);


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�oPublic�f�[�^�Z�[�u�֐�
  �����@�Fkeyvalues �f�[�^�x�[�X������ikey/value�l�j�ւ̃|�C���^
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:�Z�[�u�J�n�AFALSE�F�I�t���C����Ԃ��ʐM�G���[���ŃZ�[�u�ł��Ȃ�
  �p�r�@�F�N�ł��Q�Ɖ\�ȃf�[�^���f�[�^�X�g���[�W�T�[�o�ɃZ�[�u����B
          �Z�[�u����f�[�^��NULL�I�[�̕�����ŁAkey/value�̑g�ł���K�v������B
          �i"\\name\\mario\\stage\\3"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key/value�g����x�ɃZ�[�u���邱�Ƃ��ł���j
          �f�[�^�̃Z�b�g������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_SavePublicDataAsync(char* keyvalues, void* param);


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�oPrivate�f�[�^�Z�[�u�֐�
  �����@�Fkeyvalues �Z�[�u�������f�[�^�x�[�X������ikey/value�l�j�ւ̃|�C���^
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:�Z�[�u�J�n�AFALSE�F�I�t���C����Ԃ��ʐM�G���[���ŃZ�[�u�ł��Ȃ�
  �p�r�@�F�{�l�̂ݎQ�Ɖ\�ȃf�[�^���f�[�^�X�g���[�W�T�[�o�ɃZ�[�u����B
          �Z�[�u����f�[�^��NULL�I�[�̕�����ŁAkey/value�̑g�ł���K�v������B
          �i"\\name\\mario\\stage\\3"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key/value�g����x�ɃZ�[�u���邱�Ƃ��ł���j
          �f�[�^�̃Z�b�g������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_SavePrivateDataAsync(char* keyvalues, void* param);


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o ������Public�f�[�^���[�h�֐�
  �����@�Fkeys  ���[�h�������f�[�^��key������ւ̃|�C���^
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:���[�h�J�n�AFALSE�F�I�t���C����Ԃ��ʐM�G���[���Ń��[�h�ł��Ȃ�
  �p�r�@�F�f�[�^�X�g���[�W�T�[�o����Public�f�[�^�����[�h����B
          ���[�h����f�[�^��key�Ŏw�肵�ANULL�I�[�̕�����ł���K�v������B
          �i"\\name\\stage"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key����x�Ƀ��[�h���邱�Ƃ��ł���j
          �f�[�^�̃��[�h������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_LoadOwnPublicDataAsync(char* keys, void* param);


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o ������Private�f�[�^���[�h�֐�
  �����@�Fkeys  ���[�h�������f�[�^��key������ւ̃|�C���^
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:���[�h�J�n�AFALSE�F�I�t���C����Ԃ��ʐM�G���[���Ń��[�h�ł��Ȃ�
  �p�r�@�F�f�[�^�X�g���[�W�T�[�o����Private�f�[�^�����[�h����B
          ���[�h����f�[�^��key�Ŏw�肵�ANULL�I�[�̕�����ł���K�v������B
          �i"\\name\\stage"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key����x�Ƀ��[�h���邱�Ƃ��ł���j
          �f�[�^�̃��[�h������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_LoadOwnPrivateDataAsync(char* keys, void* param);


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o �����̃f�[�^���[�h�֐�  (obsolete function)
  �����@�Fkeys  ���[�h�������f�[�^��key������ւ̃|�C���^
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:���[�h�J�n�AFALSE�F�I�t���C����Ԃ��ʐM�G���[���Ń��[�h�ł��Ȃ�
  �p�r�@�F�f�[�^�X�g���[�W�T�[�o����f�[�^�����[�h����B
          ���[�h����f�[�^��key�Ŏw�肵�ANULL�I�[�̕�����ł���K�v������B
          �i"\\name\\stage"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key����x�Ƀ��[�h���邱�Ƃ��ł���j
          �f�[�^�̃��[�h������������R�[���o�b�N���Ă΂��B
          ���������e�̊֐���DWC_LoadOwnPrivateDataAsync()�ɉ������܂����B
 *---------------------------------------------------------------------------*/
#define DWC_LoadOwnDataAsync DWC_LoadOwnPrivateDataAsync


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o ���l��Public�f�[�^���[�h�֐�
  �����@�Fkeys  ���[�h�������f�[�^��key������ւ̃|�C���^
          index ���[�h���������[�U�̗F�B���X�g�C���f�b�N�X
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:���[�h�J�n�A
          FALSE�F�������I�t���C����Ԃ����肪�F�B�łȂ��A�ʐM�G���[���Ń��[�h�ł��Ȃ�
  �p�r�@�F�f�[�^�X�g���[�W�T�[�o����Public�f�[�^�����[�h����B
          ���[�h����f�[�^��key�Ŏw�肵�ANULL�I�[�̕�����ł���K�v������B
          �i"\\name\\stage"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key����x�Ƀ��[�h���邱�Ƃ��ł���j
          �f�[�^�̃��[�h������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
extern BOOL DWC_LoadOthersDataAsync(char* keys, int index, void* param);


//----------------------------------------------------------------------------
// function - internal
//---------------------------------------------------------------------------
extern void DWCi_FriendInit(DWCFriendControl* friendcnt,
                            GPConnection* pGpObj,
                            const u16* playerName,
                            DWCFriendData* friendList,
                            int friendListLen);


extern void DWCi_FriendProcess(void);


extern void DWCi_UpdateServersAsync(const char* authToken,
                                    const char* partnerChallenge,
                                    DWCUpdateServersCallback updateCallback,
                                    void* updateParam,
                                    DWCFriendStatusCallback statusCallback,
                                    void* statusParam,
                                    DWCDeleteFriendListCallback deleteCallback,
                                    void* deleteParam);


extern void DWCi_StopFriendProcess(DWCError error, int errorCode);


extern void DWCi_GPGetInfoCallback(GPConnection* connection,
                                   GPGetInfoResponseArg* arg,
                                   void* param);

extern void DWCi_GPRecvBuddyRequestCallback(GPConnection* connection,
                                            GPRecvBuddyRequestArg* arg,
                                            void* param);

extern BOOL DWCi_GPRecvBuddyAuthCallback(GPConnection* connection,
                                         GPRecvBuddyMessageArg* arg,
                                         void* param );


extern void DWCi_GPRecvBuddyStatusCallback(GPConnection* connection,
                                           GPRecvBuddyStatusArg* arg,
                                           void* param);


extern DWCFriendData* DWCi_GetFriendList(void);


extern int  DWCi_GetFriendListLen(void);


extern int  DWCi_GetProfileIDFromList(int index);


extern int DWCi_GetFriendListIndex(int profileID);


extern void DWCi_InitGPProcessCount(void);


extern GPResult DWCi_SetGPStatus(int status, const char* statusString, const char* locationString);


extern BOOL DWCi_GetGPStatus(int* status, char* statusString, char* locationString);


extern void DWCi_CallBuddyFriendCallback(int index);


extern void DWCi_ShutdownFriend(void);


#ifdef __cplusplus
}
#endif


#endif // DWC_FRIEND_H_

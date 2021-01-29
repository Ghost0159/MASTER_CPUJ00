//=============================================================================
/**
 * @file	comm_player_local.h
 * @brief	�v���C���[�f�[�^�̃��[�J����`
 * @author	Katsumi Ohno
 * @date    2008.01.24
 */
//=============================================================================

#ifndef __COMM_PLAYER_LOCAL_H__
#define __COMM_PLAYER_LOCAL_H__

//==============================================================================
// ��`
//==============================================================================


/// �����ȃt�B�[���h�}�b�v�A�N�^�[�ԍ�
#define _NULL_FIELD_MAP_ACT_ID (-1)
/// �l�b�g�v���[���[�̓^�X�N�͌���ł���
#define _TASK_PRI_NET_PLAYER  (100)
/// �l�b�g�v���[���[�����͂���Ɍ�
#define _TASK_PRI_NET_PLAYER_CREATE  (_TASK_PRI_NET_PLAYER+100)
/// BLACT_HEADER work�́@�e�q�@ID�����Ă����ꏊ
/// �O�ƂP�͖{���̃q�[���[�A�j���Ŏg�p���Ă���̂łQ�ԂɂȂ��Ă���
#define _WORK_MACHINE_ID (2)

#define _NAME_DISP (0)   ///< �v���[���[�ɖ��O��\������

#define _NO_PLACE_INITIALIZE (0xff)  ///< �ړ��̏��������I����Ă��Ȃ��ꍇ

#define _PLACEDATA_SEND_SIZE (5)  ///< �ꏊ�f�[�^�𑗐M����ꍇ�̃T�C�Y
#define _IDPLACEDATA_SEND_SIZE (4)  ///< ID���ꏊ�f�[�^�𑗐M����ꍇ�̃T�C�Y

//#define _BLOW_STOP_COUNT (15)  // �ŏ��ɓ����Ȃ���
#define _BLOW_COUNTER_NUM_MAX   (27)  // �ӂ���΂����t���[����
#define _BIGBLOW_COUNTER_NUM_MAX   (45)  // �ӂ���΂����t���[����
#define _BLOW_COUNTER_STOP   (0xff)  // �~�߂Ă�����Ԃ̒l

#define _TALK_TARGET_INDEX_WORK (0)  ///< ��b�f�[�^�@�b�����ID�̈ʒu
#define _TALK_MY_INDEX_WORK (1)  ///< ��b�f�[�^�@�b���Ă���ID�̈ʒu

#define _FLAG_TVTOPIC_NUM (5)  ///TV�ɗ����ꍇ�̊��̐�

// ���j���[���E�C���h�E���^�X�N�ɓo�^���Ă����ۂ̃v���C�I���e�B�[
enum menuTaskPri_e {
    _TALK_WINDOW_PRI = 10001,
};


enum _player_speed {
    _SPEED_2,
    _SPEED_4,
    _SPEED_8,
    _SPEED_16,
    _SPEED_HITWALL,
    _SPEED_MAX,
};


enum hedFieldEffect {
    _HFE_NONE,
    _HFE_FLAG,
    _HFE_EXCLAMATION,
    _HFE_OK,
};


#define _LOCAL_WIN_PX (2)   // GF_BGL_BmpAdd�Ŏg�p���� �����E�C���h�E��PX�l
#define _LOCAL_WIN_PY (19)   // GF_BGL_BmpAdd�Ŏg�p���� �����E�C���h�E��PY�l
#define _LOCAL_WIN_SX (28)   // GF_BGL_BmpAdd�Ŏg�p���� �����E�C���h�E��SX�l
#define _LOCAL_WIN_SY (4)   // GF_BGL_BmpAdd�Ŏg�p���� �����E�C���h�E��SY�l

#define _DASH_BIT (0x80)
#define _DEBUG_DASH_BIT (0x40)

#define _MSG_MAX  (50*GLOBAL_MSGLEN)    ///<

#define _KEEP_FLAG_MAX (5)  // �L�����Ă���ID

//==============================================================================
//	�^�錾
//==============================================================================

typedef struct{
    int timer;
    int targetID;
} _FREEZE_PLAYER;



typedef struct{
    u8 mystatus[COMM_SEND_MYSTATUS_SIZE];
    u16 netID;
} _SEND_FLAG;


typedef struct{
    u16 xpos;
    u16 zpos;
    s8 dir;
    u8 speed;
    u8 speedKey;
    u8 bHit;
} _PlayerPlace;

typedef struct{
    u16 xpos;
    u16 zpos;
} _Grid;

typedef struct{
    u8 netID:4;      // �b���Ă���l
    u8 targetID:4;   // �b���������Ă���l
    u8 bItemMenu:1;      //  �A�C�e�����j���[���J���ɂ����Ă���
    u8 bPersonalInformation:1;      // �l�����J���Ă���
} _talkMenuInfo;

typedef struct{
    u32 holdBit;
    PLAYER_CONDITION_MGR_PTR pCondMgr;
    PLAYER_STATE_PTR pPlayer[COMM_MACHINE_MAX];   // �v���[���[�̃|�C���^
    EOA_PTR pMark[COMM_MACHINE_MAX];  //�܂��
    u8 bActive[COMM_MACHINE_MAX];     // �v���[���[�����݂��邩�ǂ���
    TCB_PTR pTask;          // �����^�X�N�̃|�C���^
    FIELDSYS_WORK* pFSys;
    _talkMenuInfo talkMenuInfo;   // �������ǂ�������b��Ԃɂ��邩�ǂ����L�� �T�[�o�ɂ͖���
    u8 handCount[COMM_MACHINE_MAX];
    _PlayerPlace sPlayerPlaceServer[ COMM_MACHINE_MAX ];  // �T�[�o�[�̃v���[���[�̈ʒu���
    _PlayerPlace sPlayerPlace[ COMM_MACHINE_MAX ];  // �v���[���[�̈ʒu���
    u8 bMoveControl[ COMM_MACHINE_MAX ]; ///< �@�̃R���g���[�����
    u8 bMoveControlFlag[ COMM_MACHINE_MAX ]; ///< �@�̃R���g���[����� �n�^��p
    u8 bMoveControlVS[ COMM_MACHINE_MAX ];
    u8 hedFE[ COMM_MACHINE_MAX ]; ///< ���̏�̃t�B�[���h�G�t�F�N�g�Ǘ�
    s8 blowDir[COMM_MACHINE_MAX];
    u8 blowCounterS[COMM_MACHINE_MAX];   // ������΂��J�E���^
    u8 blowDirS[COMM_MACHINE_MAX];
    u8 giddyCounter[COMM_MACHINE_MAX];
    u8 countHole[COMM_MACHINE_MAX];
    u8 bBlowBigS[COMM_MACHINE_MAX];
    u8 bMoveChange[COMM_MACHINE_MAX]; // �ړ������̂ő��M���邽�߂̃t���O
    u8 walkCount[COMM_MACHINE_MAX];   // �������x
    u8 walkCountClient[COMM_MACHINE_MAX];   // �������x �N���C�A���g
    _SEND_FLAG sendFlagData[COMM_MACHINE_MAX+1];
  //  u8 sendFlagDataID[COMM_MACHINE_MAX];
    MYSTATUS* pKeepFlag[_KEEP_FLAG_MAX];  // 5�����肱�̎��ɏd�����Ȃ���΋L�^
    MYSTATUS* pNowFlag[COMM_MACHINE_MAX];  //�݂�Ȃ��瑗�M���ꂽ���̂͂����ɓ���
//    MYSTATUS* pMyFlag;   //�����������Ă���͂� ����𑗐M������
    u16 oldPad;
    u16 myDeliveryFlagNum;
    u8 bStartLineChk[4]; //
    u8 bTalkMenuDisp;   ///<< ��b�E�C���h�E�\����TRUE
//    u8 msgIndex;      ///< ���b�Z�[�W�C���f�b�N�X ���b�Z�[�W�I���҂��Ɏg�p
    u8 bFlagDataRecv;
    u8 moveSync;     ///< �ړ��ʒu����
    u8 bPosAllSend;  ///< �S���̈ʒu���đ��M����t���O
    u8 bMyMoveControl; ///< ���@�̃R���g���[��
    u8 bResetting;   ///< ���Z�b�g���̏ꍇ����ɓ����Ȃ��悤��
    u8 bUnder;    // �n�����ǂ���
    u8 bFirstMoveEnable;  // �e�@�����Ȃ��Ȃ����ꍇ�A�e�@�ɐڑ������ꍇ �J�n�𑗐M���邽�߂�FLG
    u8 forceDirFlg;  //�����p�x����
    u8 bMoveControlVSRoom; //�ΐ핔���ł̈ړ�
    u8 bDataSendFlag; // ���̓�������
  u8 bNotRecvMoveData;  //�����̈ړ��f�[�^���T�[�o����󂯎��Ȃ�
} CommPlayerWork;


extern CommPlayerWork* CommPlayerGetInstance(void);

#endif //__COMM_PLAYER_LOCAL_H__


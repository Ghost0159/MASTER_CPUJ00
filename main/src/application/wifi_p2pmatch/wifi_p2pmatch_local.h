//=============================================================================
/**
 * @file	wifi_note_local.h
 * @brief	WIFI�蒠���[�J����`
 * @author	k.ohno
 * @date    2006.4.5
 */
//=============================================================================


#ifndef __WIFI_P2PMATCH_LOCAL_H__
#define __WIFI_P2PMATCH_LOCAL_H__

#include "savedata/wifilist.h"
#include "wifi_p2pmatchroom.h"
#include "gflib/button_man.h"
#include "system/fontoam.h"
#include "application/connect_anm_types.h"

#include "system/bmp_menu.h"
#include "system/bmp_list.h"
#include "system/wordset.h"
#include "system/procsys.h"

#include "savedata/mystatus.h"

#define WIFIP2PMATCH_MEMBER_MAX  (WIFILIST_FRIEND_MAX)
#define WIFIP2PMATCH_DISP_MEMBER_MAX  (5)

// �@�\�{�^���p��`
#define FUNCBUTTON_NUM	( 7 ) 		// �@�\�{�^���̐�
#define START_WORDPANEL ( 0 )		// �ŏ��̕������̓p�l���̔ԍ��i�O���Ђ炪�ȁj

// CLACT�Œ�`���Ă���Z�����傫�����ăT�u��ʂɉe�����łĂ��܂��̂ŗ����Ă݂�
#define NAMEIN_SUB_ACTOR_DISTANCE 	(256*FX32_ONE)

// CellActor�ɏ��������郊�\�[�X�}�l�[�W���̎�ނ̐��i���}���`�Z���E�}���`�Z���A�j���͎g�p���Ȃ��j
#define CLACT_RESOURCE_NUM		(  4 )
#define _OAM_NUM			( 5 )

#define FRIENDCODE_MAXLEN     (12)

// �}�b�`���O���[���ɂ͂��Ă���l�̐��i����������āj
#define MATCHROOM_IN_NPCNUM	(32)
#define MATCHROOM_IN_OBJNUM	(MATCHROOM_IN_NPCNUM+1)

// ���b�Z�[�W�\�����WAIT
#define WIFIP2PMATCH_CORNER_MESSAGE_END_WAIT	( 60 )


// �����p�l���̑J�ڗp
enum{
  WIFIP2PMATCH_MODE_INIT  = 0,
  WIFIP2PMATCH_NORMALCONNECT_YESNO,
  WIFIP2PMATCH_NORMALCONNECT_WAIT,
  WIFIP2PMATCH_DIFFER_MACHINE_INIT,
  WIFIP2PMATCH_DIFFER_MACHINE_NEXT,
  WIFIP2PMATCH_DIFFER_MACHINE_ONEMORE,
  WIFIP2PMATCH_FIRST_YESNO,
  WIFIP2PMATCH_POWEROFF_INIT,
  WIFIP2PMATCH_POWEROFF_YESNO,
  WIFIP2PMATCH_POWEROFF_WAIT,
  WIFIP2PMATCH_RETRY_INIT,        //10
  WIFIP2PMATCH_RETRY_YESNO,
  WIFIP2PMATCH_RETRY_WAIT,
  WIFIP2PMATCH_RETRY,
  WIFIP2PMATCH_CONNECTING_INIT,
  WIFIP2PMATCH_CONNECTING,
  WIFIP2PMATCH_FIRST_ENDMSG,
  WIFIP2PMATCH_FIRST_ENDMSG_WAIT,
  WIFIP2PMATCH_FRIENDLIST_INIT,
  WIFIP2PMATCH_MODE_FRIENDLIST,
  WIFIP2PMATCH_VCHATWIN_WAIT,           //20
  WIFIP2PMATCH_MODE_VCT_CONNECT_INIT2,
  WIFIP2PMATCH_MODE_VCT_CONNECT_INIT,
  WIFIP2PMATCH_MODE_VCT_CONNECT_WAIT,
  WIFIP2PMATCH_MODE_VCT_CONNECT,
  WIFIP2PMATCH_MODE_VCT_CONNECTEND_YESNO,
  WIFIP2PMATCH_MODE_VCT_CONNECTEND_WAIT,
  WIFIP2PMATCH_MODE_VCT_DISCONNECT,
  WIFIP2PMATCH_MODE_BATTLE_DISCONNECT,
  WIFIP2PMATCH_MODE_DISCONNECT,
  WIFIP2PMATCH_MODE_BATTLE_CONNECT_INIT,  //30
  WIFIP2PMATCH_MODE_BATTLE_CONNECT_WAIT,
  WIFIP2PMATCH_MODE_BATTLE_CONNECT,
  WIFIP2PMATCH_MODE_MAIN_MENU,
  WIFIP2PMATCH_MODE_END_WAIT,
  WIFIP2PMATCH_MODE_CHECK_AND_END,
  WIFIP2PMATCH_MODE_SELECT_INIT,
  WIFIP2PMATCH_MODE_SELECT_WAIT,
  WIFIP2PMATCH_MODE_SUBBATTLE_WAIT,
  WIFIP2PMATCH_MODE_SELECT_REL_INIT,
  WIFIP2PMATCH_MODE_SELECT_REL_YESNO,       //40
  WIFIP2PMATCH_MODE_SELECT_REL_WAIT,
  WIFIP2PMATCH_MODE_MATCH_INIT,
  WIFIP2PMATCH_MODE_MATCH_INIT2,
  WIFIP2PMATCH_MODE_MATCH_WAIT,
  WIFIP2PMATCH_MODE_MATCH_LOOP,
  WIFIP2PMATCH_MODE_BCANCEL_YESNO,
  WIFIP2PMATCH_MODE_BCANCEL_WAIT,
  WIFIP2PMATCH_MODE_CALL_INIT,
  WIFIP2PMATCH_MODE_CALL_YESNO,
  WIFIP2PMATCH_MODE_CALL_SEND,
  WIFIP2PMATCH_MODE_CALL_CHECK,             //50
  WIFIP2PMATCH_MODE_MYSTATUS_WAIT,
  WIFIP2PMATCH_MODE_CALL_WAIT,
  WIFIP2PMATCH_MODE_PERSONAL_INIT,
  WIFIP2PMATCH_MODE_PERSONAL_WAIT,
  WIFIP2PMATCH_MODE_PERSONAL_END,
  WIFIP2PMATCH_MODE_EXIT_YESNO,
  WIFIP2PMATCH_MODE_EXIT_WAIT,
  WIFIP2PMATCH_MODE_EXITING,
  WIFIP2PMATCH_MODE_EXIT_END,
  WIFIP2PMATCH_NEXTBATTLE_YESNO,         //60
  WIFIP2PMATCH_NEXTBATTLE_WAIT,
  WIFIP2PMATCH_MODE_VCHAT_NEGO,
  WIFIP2PMATCH_MODE_VCHAT_NEGO_WAIT,
  WIFIP2PMATCH_RECONECTING_WAIT,
  WIFIP2PMATCH_MODE_BCANCEL_YESNO_VCT,
  WIFIP2PMATCH_MODE_BCANCEL_WAIT_VCT,
  WIFIP2PMATCH_PARENT_RESTART,
  WIFIP2PMATCH_FIRST_SAVING,
  WIFIP2PMATCH_MODE_CANCEL_ENABLE_WAIT,
  WIFIP2PMATCH_FIRST_SAVING2,

};


enum{
	WIFIP2PMATCH_SYNCHRONIZE_END=201,
};

// �㉺��ʎw���`
#define BOTH_LCD	( 2 )
#define MAIN_LCD	( GF_BGL_MAIN_DISP )	// �v�͂O��
#define SUB_LCD		( GF_BGL_SUB_DISP )		// �P�Ȃ�ł����B

// BMPWIN�w��
enum{
	BMP_NAME1_S_BG0,
	BMP_NAME2_S_BG0,
	BMP_NAME3_S_BG0,
	BMP_NAME4_S_BG0,
	BMP_NAME5_S_BG0,
	BMP_WIFIP2PMATCH_MAX,
};


//---------------WIFISTATUS�f�[�^

typedef enum{
  WIFI_STATUS_NONE,   // ��������	NONE�̂Ƃ��͏o�������܂���
  WIFI_STATUS_VCT,      // VCT��
  WIFI_STATUS_SBATTLE50,      // �V���O���ΐ풆
  WIFI_STATUS_SBATTLE100,      // �V���O���ΐ풆
  WIFI_STATUS_SBATTLE_FREE,      // �V���O���ΐ풆
  WIFI_STATUS_DBATTLE50,      // �_�u���ΐ풆
  WIFI_STATUS_DBATTLE100,      // �_�u���ΐ풆
  WIFI_STATUS_DBATTLE_FREE,      // �_�u���ΐ풆
  WIFI_STATUS_TRADE,          // ������
  WIFI_STATUS_SBATTLE50_WAIT,   // �V���O��Lv50�ΐ��W��
  WIFI_STATUS_SBATTLE100_WAIT,   // �V���O��Lv100�ΐ��W��
  WIFI_STATUS_SBATTLE_FREE_WAIT,   // �V���O��Lv100�ΐ��W��
  WIFI_STATUS_DBATTLE50_WAIT,   // �_�u��Lv50�ΐ��W��
  WIFI_STATUS_DBATTLE100_WAIT,   // �_�u��Lv100�ΐ��W��
  WIFI_STATUS_DBATTLE_FREE_WAIT,   // �_�u��Lv100�ΐ��W��
  WIFI_STATUS_TRADE_WAIT,    // ������W��
  WIFI_STATUS_LOGIN_WAIT,    // �ҋ@���@���O�C������͂���
  
  WIFI_STATUS_DP_UNK,        // DP��UNKNOWN
  
  // �v���`�i�Œǉ�
  WIFI_STATUS_POFIN,          // �|�t�B��������
  WIFI_STATUS_POFIN_WAIT,    // �|�t�B����W��
  WIFI_STATUS_FRONTIER,          // �t�����e�B�A��
  WIFI_STATUS_FRONTIER_WAIT,    // �t�����e�B�A��W��

  WIFI_STATUS_BUCKET,				// �o�P�b�g�Q�[��
  WIFI_STATUS_BUCKET_WAIT,			// �o�P�b�g�Q�[����W��
  WIFI_STATUS_BALANCEBALL,		    // �ʏ��Q�[��
  WIFI_STATUS_BALANCEBALL_WAIT,		// �ʏ��Q�[����W��
  WIFI_STATUS_BALLOON,				// �΂�[��Q�[��
  WIFI_STATUS_BALLOON_WAIT,			// �΂�[��[����W��

#ifdef WFP2P_DEBUG_EXON
  WIFI_STATUS_BATTLEROOM,     // �o�g�����[����
  WIFI_STATUS_BATTLEROOM_WAIT,// �o�g�����[����W��
  WIFI_STATUS_MBATTLE_FREE,     // �}���`�o�g����
  WIFI_STATUS_MBATTLE_FREE_WAIT,// �}���`�o�g����W��
#endif

  WIFI_STATUS_PLAY_OTHER,	// WiFi�N���u�ɈȊO�ŗV�ђ�
  WIFI_STATUS_UNKNOWN,   // �V���ɍ�����炱�̔ԍ��ȏ�ɂȂ�
};

typedef enum{
  _REGULATION_BATTLE_TOWER     // �o�g���^���[�ΐ����
};


//�g�[�^��189�o�C�g���M�ł��邪�Ƃ肠�������ʃv���O�����͔͈͕���������
#define _POKEMON_NUM   (6)

#define _CANCELENABLE_TIMER (60*30)   // �L�����Z���ɂȂ�ׂ̃^�C�}�[60min


enum{
	WF_VIEW_STATUS,
	WF_VIEW_VCHAT,
	WF_VIEW_STATUS_NUM,
};

enum{
	WF_USERDISPTYPE_NRML,	// �ʏ�
	WF_USERDISPTYPE_MINI,	// �~�j�Q�[��
	WF_USERDISPTYPE_BLTW,	// �o�g���^���[
	WF_USERDISPTYPE_BLFT,	// �o�g���t�����e�B�A
	WF_USERDISPTYPE_BLKS,	// �o�g���L���b�X��
	WF_USERDISPTYPE_BTST,	// �o�g���X�e�[�W
	WF_USERDISPTYPE_BTRT,	// �o�g�����[���b�g
	WF_USERDISPTYPE_NUM,
};

// ���[�U�\���{�^����
enum{
	MCV_USERD_BTTN_LEFT = 0,
	MCV_USERD_BTTN_BACK,
	MCV_USERD_BTTN_RIGHT,
	MCV_USERD_BTTN_NUM,
};

typedef struct _WIFI_MACH_STATUS_tag{
  u16 pokemonType[_POKEMON_NUM];
  u16 hasItemType[_POKEMON_NUM];
  u8 version;
  u8 regionCode;
  u8 pokemonZukan;
  u8 status;
  u8 regulation;
  u8 trainer_view;
  u8 sex;
  u8 nation;
  u8 area;
  u8 vchat;
  u8 vchat_org;
} _WIFI_MACH_STATUS;

#define _WIFI_STATUS_MAX_SIZE  (sizeof(_WIFI_MACH_STATUS))		// ������ς����DP�Ƃ̌݊������Ȃ��Ȃ�

//============================================================================================
//	�\���̒�`
//============================================================================================

typedef struct {
  _WIFI_MACH_STATUS myMatchStatus;   // �����̃}�b�`���O��ԃf�[�^
  _WIFI_MACH_STATUS friendMatchStatus[WIFIP2PMATCH_MEMBER_MAX]; // ����̃}�b�`���O��ԃf�[�^
} TEST_MATCH_WORK;

//-------------------------------------
///	Icon�O���t�B�b�N
//=====================================
typedef struct {
	void* p_buff;
	NNSG2dScreenData* p_scrn;

	void* p_charbuff;
	NNSG2dCharacterData* p_char;
} WIFIP2PMATCH_ICON;


//-------------------------------------
///	�f�[�^�r���[�A�[�f�[�^
//=====================================
typedef struct {

	// ���[�h�Z�b�g
	WORDSET*	p_wordset;			// ���b�Z�[�W�W�J�p���[�N�}�l�[�W���[
	
	// �{�^���O���t�B�b�N
	void* p_bttnbuff;
	NNSG2dScreenData* p_bttnscrn;

	void* p_userbuff[WF_USERDISPTYPE_NUM];
	NNSG2dScreenData* p_userscrn[WF_USERDISPTYPE_NUM];

	void* p_useretcbuff;
	NNSG2dScreenData* p_useretcscrn;
	
	BUTTON_MAN* p_bttnman;	// �{�^���Ǘ��V�X�e��
	u8 bttnfriendNo[ MATCHROOM_IN_NPCNUM ];
	u8 frame_no;	// ���\�����Ă��鏰�̃i���o�[
	u8 touch_friendNo;	// �G��Ă���F�B�ԍ�+1
	u8 touch_frame;	// �t���[����
	u8 user_disp;		// ���[�U�[�f�[�^�\��ONOFF
	s8 user_dispno;	// ���[�U�[DISP���e
	u8 button_on;		// �{�^���\���X�V
	u8 button_count;	// �{�^���t���[���J�E���^
	u8 bttn_chg_friendNo;	// �{�^�����X�V���Ăق����F�B�ԍ�
	BOOL bttn_allchg;	// �{�^�����ׂĂ��X�V���邩 

	// �\���r�b�g�}�b�v��
	GF_BGL_BMPWIN	  nameWin[ WCR_MAPDATA_1BLOCKOBJNUM ];
	GF_BGL_BMPWIN	  statusWin[ WCR_MAPDATA_1BLOCKOBJNUM ][ WF_VIEW_STATUS_NUM ];
	GF_BGL_BMPWIN	  userWin;

	// �{�^��
	CLACT_U_RES_OBJ_PTR	button_res[ 4 ];
	CLACT_WORK_PTR button_act[MCV_USERD_BTTN_NUM];
	CHAR_MANAGER_ALLOCDATA back_fontoam_cg;
	FONTOAM_OBJ_PTR back_fontoam;	// ���ǂ�pFONTOAM
	u32 buttonact_on;			// �{�^�����샂�[�h
	u32 touch_button;
	u32 touch_button_event;
	BUTTON_MAN* p_oambttnman;	// �{�^���Ǘ��V�X�e��
} WIFIP2PMATCH_VIEW;



struct _WIFIP2PMATCH_WORK{
  WIFI_LIST* pList;				// �Z�[�u�f�[�^���̃��[�U�[�ƃt�����h�f�[�^
  TEST_MATCH_WORK* pMatch;		// �T�[�o�[�Ƒ���M���鎩���ƗF�B�̏��
  int friendMatchReadCount;		// �r�[�R������M�����F�B��
  u8 index2No[WIFIP2PMATCH_MEMBER_MAX];	// �t�����h�i���o�[�z��
  u8 index2NoBackUp[WIFIP2PMATCH_MEMBER_MAX];	// �t�����h�i���o�[�z��o�b�N�A�b�v
  int matchStatusBackup[WIFIP2PMATCH_MEMBER_MAX];// �F�B�r�[�R���f�[�^���ς�����������p
  int matchVchatBackup[WIFIP2PMATCH_MEMBER_MAX];// �F�B�r�[�R���f�[�^���ς�����������p
//  NAMEIN_PARAM*		nameinParam;
  
  // ���C�����X�g�p���[�N
  BMPLIST_DATA*   menulist;
  BMPLIST_WORK* lw;		// BMP���j���[���[�N
  
  BMPLIST_DATA*   submenulist;
  BMPLIST_WORK* sublw;		// BMP���j���[���[�N
  GF_BGL_INI		*bgl;									// GF_BGL_INI
  SAVEDATA*  pSaveData;
  WORDSET			*WordSet;								// ���b�Z�[�W�W�J�p���[�N�}�l�[�W���[
  MSGDATA_MANAGER *MsgManager;							// ���O���̓��b�Z�[�W�f�[�^�}�l�[�W���[
  MSGDATA_MANAGER *SysMsgManager;  //
 // STRBUF			*TrainerName[WIFIP2PMATCH_MEMBER_MAX];		// ���O
//  STRBUF			*MsgString;								// ���b�Z�[�W
  STRBUF*         pExpStrBuf;
  STRBUF			*TalkString;							// ��b���b�Z�[�W�p
  STRBUF			*TitleString;							// �^�C�g�����b�Z�[�W�p
//  STRBUF			*MenuString[4];							// ���j���[���b�Z�[�W�p
  STRBUF*         pTemp;        // ���͓o�^���̈ꎞ�o�b�t�@
  
  int				MsgIndex;								// �I�����o�p���[�N
  BMPMENU_WORK* pYesNoWork;
  void* timeWaitWork;			// �^�C���E�G�C�g�A�C�R�����[�N
  CLACT_SET_PTR 			clactSet;								// �Z���A�N�^�[�Z�b�g
  CLACT_U_EASYRENDER_DATA	renddata;								// �ȈՃ����_�[�f�[�^
  CLACT_U_RES_MANAGER_PTR	resMan[CLACT_RESOURCE_NUM];				// ���\�[�X�}�l�[�W��
  FONTOAM_SYS_PTR			fontoam;								// �t�H���gOAM�V�X�e��
/*
  CLACT_U_RES_OBJ_PTR 	resObjTbl[BOTH_LCD][CLACT_RESOURCE_NUM];// ���\�[�X�I�u�W�F�e�[�u��
  CLACT_HEADER			clActHeader_m;							// �Z���A�N�^�[�w�b�_�[
  CLACT_HEADER			clActHeader_s;							// �Z���A�N�^�[�w�b�_�[
  CLACT_WORK_PTR			MainActWork[_OAM_NUM];				// �Z���A�N�^�[���[�N�|�C���^�z��
	//CLACT_WORK_PTR			SubActWork[_OAM_NUM];				// �Z���A�N�^�[���[�N�|�C���^�z��
//*/

  GF_BGL_BMPWIN			MsgWin;									// ��b�E�C���h�E
  //   GF_BGL_BMPWIN           MenuWin[4];
  GF_BGL_BMPWIN			MyInfoWin;								// �����̏�ԕ\��
  GF_BGL_BMPWIN			MyInfoWinBack;								// �^�C�g��
  GF_BGL_BMPWIN			SysMsgWin;								// �V�X�e���E�B���h�E�ŕ`�悷�����	����[��ADWC���[���̃��b�Z�[�W
  GF_BGL_BMPWIN			ListWin;									// �t�����h���X�g
  GF_BGL_BMPWIN			SubListWin;									// ��W����^�C�v�Ȃǂ�`�悷�郊�X�g
  GF_BGL_BMPWIN			MyWin;									// �F�B�̌l���\��

   int cancelEnableTimer;   // �L�����Z�����ɂȂ�ׂ̃^�C�}�[
  int localTime;
  int seq;									// ���݂̕������͏�ԁi����OK/�A�j�����j�Ȃ�
  int       endSeq;
  int						nextseq;
  int initSeq;
  int						mode;									// ���ݍőO�ʂ̕����p�l��
  int						timer;			// �҂����Ԃ�A�t���O�ȂǂɎg�p
  MYSTATUS                *pMyStatus;		// �����̃X�e�[�^�X
  u8     mainCur;
  u8     subCur;
  BOOL bInitMessage;		// ����ڑ���
  PROC*		subProc;
  int preConnect;			// �V���������F�B(-1=�Ȃ�)
  u16 battleCur;			// �o�g���^�C�v�I�����j���[�J�[�\��
  u16 singleCur[3];			// �o�g���̏ڍו����̃��j���[�J�[�\��
  u16 bSingle;				// SINGLE�o�g���@�_�u���o�g���X�C�b�`
  u16 keepStatus;			// �ڑ����悤�Ƃ����瑊��̃X�e�[�^�X���ς�����Ƃ��������p
  u16 keepVChat;			// �ڑ����悤�Ƃ�����{�C�X�`���b�g��Ԃ��ς�����Ƃ��������p
  u16 friendNo;			// ���Ȃ����Ă���F�B�i���o�[
  BOOL bRetryBattle;
   int vctEnc;
	WIFI_MATCHROOM matchroom;	// �}�b�`���O���[�����[�N
	MCR_MOVEOBJ*	p_matchMoveObj[ MATCHROOM_IN_OBJNUM ];
	WIFIP2PMATCH_ICON icon;	// �A�C�R���O���t�B�b�N
	WIFIP2PMATCH_VIEW view;	// �F�B�f�[�^�r���[�A�[

	BOOL friend_request_wait;	// �F�B��W����TRUE�ɂȂ�t���O
	
	CONNECT_BG_PALANM cbp;		// Wifi�ڑ���ʂ�BG�p���b�g�A�j������\����

	u16 brightness_reset;	// _retry�Ń}�X�^�[�P�x��VBlank�Ń��Z�b�g���邽��
	u16 friend_num;			// P2Pmatch��ʏ������^�C�~���O�̗F�B��
};




#endif  //__WIFI_P2PMATCH_LOCAL_H__


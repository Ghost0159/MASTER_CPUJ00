//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_system.c
 *	@brief		WiFi���r�[���ʏ����V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.09.14
 *
 *	WiFi���r�[����ɓ��삵�Ă���V�X�e���͂����œ������B
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "gflib/strbuf_family.h"

#include "system/msgdata.h"
#include "system/fontproc.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_wifi_hiroba.h"

#include "savedata/mystatus.h"
#include "savedata/zukanwork.h"
#include "savedata/wifihistory.h"
#include "savedata/gametime.h"
#include "savedata/fnote_mem.h"
#include "savedata/record.h"
#include "savedata/wifi_hiroba_save.h"

#include "field/fieldobj_code.h"

#include "poketool/pokeparty.h"

#include "wifi/dwc_lobbylib.h"

#include "application/wifi_country.h"

#include "wflby_system.h"
#include "worldtimer_place.h"
#include "wflby_3dobj.h"
#include "wflby_3dmatrix.h"

#include "wflby_snd.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------


#ifdef PM_DEBUG
//#define WFLBY_DEBUG_NPC_IN
#define WFLBY_DEBUG_ALL_VIP
//#define WFLBY_DEBUG_PROFILE_DRAW
#endif

#ifdef WFLBY_DEBUG_ALL_VIP
extern BOOL D_Tomoya_WiFiLobby_ALLVip;
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	��b�f�[�^
//=====================================
enum {
	WFLBY_TALK_TYPE_NONE,	// �������ĂȂ�
	WFLBY_TALK_TYPE_A,		// �b����������
	WFLBY_TALK_TYPE_B,		// �b��������ꂽ��
} ;

//-------------------------------------
///	�t���[�g�J�E���g�l	
//�@�p���[�h�͖�144�b������
//=====================================
#define WFLBY_FLOAT_COUNT_MAX			( 144*30 )	// �S�̃J�E���g�l
#define WFLBY_FLOAT_COUNT_KANKAKU		( 12*30 )	// �P�����o��Ԋu
#define WFLBY_FLOAT_COUNT_ONE			( 40*30 )	// �P�̃t���[�g�������Ă����b��



//-------------------------------------
///	BGM�̃t�F�[�h�A�E�g�^�C�~���O
//=====================================
#define WFLBY_END_BGM_FADEOUT_FRAME		( 127 )



//-------------------------------------
///	�K�W�F�b�g�I�����[�g�V�X�e��
//=====================================
#define WFLBY_GADGETRATE_DEF		( 100 )		// �������[�g
#define WFLBY_GADGETRATE_DIV		( 2 )		// �K�W�F�b�g���I�΂ꂽ���Ƃɒl������l


//-------------------------------------
///	�����tA�`D�l�̃C���f�b�N�X
//=====================================
enum{
	WFLBY_AIKOTOBA_KEY_A,
	WFLBY_AIKOTOBA_KEY_B,
	WFLBY_AIKOTOBA_KEY_C,
	WFLBY_AIKOTOBA_KEY_D,
	WFLBY_AIKOTOBA_KEY_NUM,
};


//-------------------------------------
///	�ԉΒ萔
//=====================================
#define WFLBY_FIRE_MOVE_PAR_MAX	( 256 )	// 


//-------------------------------------
///	�L���[�o�b�t�@��
//=====================================
#define WFLBY_LASTACT_Q_BUFFNUM	( WFLBY_LASTACT_BUFFNUM+1 )



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------


//-------------------------------------
///	���[�U��{���
//=====================================
typedef struct _WFLBY_USER_PROFILE{
	s32				userid;						//  ���r�[�����[�UID
	u32				trainerid;					// �g���[�i�[ID					
	STRCODE			name[PERSON_NAME_SIZE + EOM_SIZE];	// ���[�U��
	WFLBY_TIME		intime;						// ��������
	WFLBY_TIME		wldtime;					// �����̍���GMT����
	u16				monsno[ TEMOTI_POKEMAX ];	// �莝���|�P����
	u8				formid[ TEMOTI_POKEMAX ];	// �|�P�����̃t�H�����f�[�^
	u8				tamago[ TEMOTI_POKEMAX ];	// ���t���O	
	u8				sex;						// ����
	u8				region_code;				// ����R�[�h LANG_JAPAN�ȂǂȂ�
	u16				tr_type;					// �g���[�i�̌�����
	u16				nation;						// ���R�[�h
	u8				area;						// �n��R�[�h
	u8				zukan_zenkoku;				// �S���}�ӕێ��t���O
	u8				game_clear;					// �Q�[���N���A�t���O
	u8				item;						// �^�b�`�g�C
	u8				rom_code;					// �����o�[�W����	VERSION_PLATINUM�Ƃ�
	u8				status;						// �v���C���[�X�e�[�^�X
	s64				start_sec;					// �`�����n�߂�����
	u8				last_action[WFLBY_LASTACT_BUFFNUM];	// �Ō�ɍs�������Ɓ@WFLBY_LASTACTION_TYPE������܂��B
	s32				last_action_userid[WFLBY_LASTACT_BUFFNUM];	// �Ō�ɍs�������Ɓ@�l���Ƃ̐ڐG����userid���ݒ肳���o�b�t�@
	u16				waza_type[WFLBY_SELECT_TYPENUM];	// �I���������[�U�̑����^�C�v
	WFLBY_ANKETO	anketo;						// �A���P�[�g�I���f�[�^

	// �v���`�i�Ȍ�̃V�X�e���͂����ɒǉ����Ă���
} WFLBY_USER_PROFILE;

//-------------------------------------
///	�����̊�{���
//=====================================
typedef struct {
	STRCODE			def_name[PERSON_NAME_SIZE + EOM_SIZE];	// ���[�U��
	STRCODE			comm_name[PERSON_NAME_SIZE + EOM_SIZE];	// �ʐM���[�U��
	WFLBY_USER_PROFILE profile;
	u32	crc_check;	// crc�`�F�b�N
} WFLBY_USER_MYPROFILE;



//-------------------------------------
///	WiFi�L��V�񂾓��e����
//=====================================
typedef struct {
	u8	play[ WFLBY_PLAYED_NUM ];	// �V�񂾂��ǂ���
} WFLBY_PLAYED_DATA;



//-------------------------------------
///	�t���[�g���
//=====================================
typedef struct {
	BOOL	start;							// ����J�n�t���O
	s32		all_count;						// �p���[�h�S�̃J�E���^
	s32		count[ WFLBY_FLOAT_MAX ];		// ���ꂼ�ꓮ��J�E���^
	u32		move[ WFLBY_FLOAT_MAX ];		// ���쒆��
	u32		reserve[ WFLBY_FLOAT_MAX ][ WFLBY_FLOAT_ON_NUM ];	// �\����
	u32		reserve_lock[ WFLBY_FLOAT_MAX ][ WFLBY_FLOAT_ON_NUM ];// �\�񃍃b�N
	BOOL	anm[ WFLBY_FLOAT_MAX ][ WFLBY_FLOAT_ON_NUM ];		// �A�j�����N�G�X�g
} WFLBY_FLOAT_DATA;


//-------------------------------------
///	�ԉΏ��
//=====================================
typedef struct {
	BOOL	start;
	s64		start_time;
	s32		count;
	s32		count_max;

	// ����^�C�v�ƍ��̐i�s�p�[�Z���e�[�W
	u32		move_type;
	u32		move_count;	// ����J�E��Count
} WFLBY_FIRE_DATA;


//-------------------------------------
///	�C�x���g�t���O
//=====================================
typedef struct {
	u32 user_in;
	u32 user_out;
	u32 event_on;		//  �����C�x���g�̃r�b�g������
	u32 profile_up;
} WFLBY_SYSTEM_FLAG;

//-------------------------------------
///	�C�x���g��ԃt���O
//=====================================
typedef struct {
	u8  lock:1;			// �����̃��b�N	TRUEorFALSE
	u8	end:1;			// �I��			TRUEorFALSE
	u8	hababi:2;		// �ԉΒ萔		WFLBY_EVENT_HANABI_ST 
	u8	parade:1;		// PARADE		TRUEorFALSE
	u8	mini:2;			// �~�j�Q�[��OFFTRUEorFALSE
	u8	endcm:1;			// �~�j�Q�[��OFFTRUEorFALSE
	u8	neon_room;		// �l�I�����	PPW_LOBBY_TIME_EVENT_NEON_A0�`PPW_LOBBY_TIME_EVENT_NEON_A5 
	u8	neon_floor;		// �t���A���	PPW_LOBBY_TIME_EVENT_NEON_B0�`PPW_LOBBY_TIME_EVENT_NEON_B3
	u8	neon_monu;		// ���j�������g��ԁ@PPW_LOBBY_TIME_EVENT_NEON_C0�`PPW_LOBBY_TIME_EVENT_NEON_C3

	s16	end_count;		// �I�����Ԃ܂ł̃J�E���^
	s16 neon_count;		// �l�I���J�n�܂ł̃J�E���^
	s16 hanabi_count;	// �ԉΊJ�n�܂ł̃J�E���^
	s16 parade_count;	// �p���[�h�J�n�܂ł̃J�E���^

	u8	bgm_lock;	// �p���[�h�̂Ƃ���BGM�̕ύX�����Ă悢��
	u8	bgm_vl_down;// �{�����[�����Ƃ���ԃt���O�@�����ɂ���͕̂ςł����E�E�E
	u8	bgm_fadeout;// BGM�t�F�[�h�A�E�g���FTRUE
	u8	anketo_input:4;// �A���P�[�g�ɓ�������
	u8	anketo_output:4;// �A���P�[�g���P����������
	
} WFLBY_SYSTEM_EVENT;


//-------------------------------------
///	�A�v���f�[�^
//=====================================
typedef struct {
	NEWS_DATA*			p_lobbynews;		// ���r�[�j���[�X�f�[�^
	BOOL				topic_event_start;	// �^�C���C�x���g�g�s�b�N�J�n�t���O
	WFLBY_WLDTIMER		worldtimer;		// ���E���v�f�[�^
	WFLBY_TIME			worldtime;		// ���E����
	s64					worldtime_s64;	// ���E���ԁi��{�j
	s64					locktime_s64;	// ���b�N���ԁi��{�j
	MYSTATUS*			p_mystatus[NEWS_TOPICNAME_NUM];
} WFLBY_SYSTEM_GLB;


//-------------------------------------
///	����M�f�[�^
//=====================================
typedef struct {
	u16 data;			// �f�[�^
	u16 seq;			// ���̃V�[�P���X�i�f�[�^�̈Ӗ��ɂ��Ȃ���j
} WFLBY_SYSTEM_TALK_DATA;

//-------------------------------------
///	���A�f�[�^
//=====================================
typedef struct {
	u32 talk_msk;						// ���̐l�Ƙb�������Ƃ����邩
	u16 talk_idx;						// ���b���Ă�l�̃C���f�b�N�X
	u8  talk_seq;						// ��b�V�[�P���XWFLBY_TALK_SEQ
	u8	talk_b_start;					// ��bB�̉�b�J�n�t���O
	u16 recv_data;						// ��M�������b�Z�[�W
	u8 talk_type;						// ��b�^�C�v
	u8 talk_first;						// ���߂Ẳ�b�Ȃ� TRUE
	u8 talk_count[WFLBY_PLAYER_MAX];	// ��b�񐔃J�E���^
	WFLBY_SYSTEM_TALK_DATA send_data;	// ����f�[�^
	u16 recv_wait_on;					// ��M�҂��J�n
	s16	recv_wait;						// ��M�҂�����
} WFLBY_SYSTEM_TALK;

//-------------------------------------
///	�K�W�F�b�g�f�[�^
//=====================================
typedef struct {
	u8 gadget[ WFLBY_PLAYER_MAX ];	// ��M�����K�W�F�b�g
} WFLBY_SYSTEM_GADGET;


//-------------------------------------
///	�A�v���P�[�V�����t���O�t���O
//=====================================
typedef struct {
	s16 mg_lock_count;	// �~�j�Q�[�����b�N
	u8	apl_force_end;	// �A�v���P�[�V���������I��
	u8	pad[2];
} WFLBY_SYSTEM_APLFLAG;

//-------------------------------------
///	�K�W�F�b�g�����_���I���@���[�N
//=====================================
typedef struct {
	u8	rate[ WFLBY_ITEM_GROUPNUM ];	// �K�W�F�b�g�̑I��RATE
	u8	pad[3];
	u8	gadget_recv[ WFLBY_PLAYER_MAX ];// �K�W�F�b�g����M���ă��[�g�ɔ��f�����̂�
} WFLBY_SYSTEM_GADGETRATE;


//-------------------------------------
///	�����t�o�b�t�@
//=====================================
typedef struct {
	u8	aikotoba[ WFLBY_PLAYER_MAX ];		// 20byte
	WFLBY_AIKOTOBA_DATA aikotoba_data[ WFLBY_PLAYER_MAX ];
	PMSW_AIKOTOBA_TBL*	p_wordtbl;			// �ȈՉ�b�P��e�[�u��
} WFLBY_SYSTEM_AIKOTOBA;
typedef union {
	u32 number;
	u8  key[WFLBY_AIKOTOBA_KEY_NUM];
} WFLBY_SYSTEM_AIKOTOBA_KEY;


//-------------------------------------
///	�������Ɨ����L���[�Ǘ����
//=====================================
typedef struct {
	s32 cue_userid[ WFLBY_LASTACT_Q_BUFFNUM ];// userid�L���[
	u8	cue[ WFLBY_LASTACT_Q_BUFFNUM ];	// buffnum 13
	u8	top;		// �擪
	u8	tail;		// ����
	u8	pad;


	// CRC�`�F�b�N�p�p�����[�^�͉����ɓ���Ƃ�����
	//WFLBY_LASTACTION_CUE_CRC_DATASIZE���炱�̃T�C�Y�͔r������
	const SAVEDATA* cp_save;
	u16 crc_check;
	u16 error;
} WFLBY_LASTACTION_CUE;
// CRC�`�F�b�N�Ń`�F�b�N����Ƃ��̃f�[�^�T�C�Y
#define WFLBY_LASTACTION_CUE_CRC_DATASIZE	( sizeof(WFLBY_LASTACTION_CUE) - 8 )


//-------------------------------------
///	�~�j�Q�[�����
//=====================================
typedef struct {
	MYSTATUS* p_mystatus;
	BOOL		error_end;	// ���f�I�������Ƃ�TRUE
} WFLBY_SYSTEM_MG;


//-------------------------------------
///	�L����̃A���P�[�g����
//=====================================
typedef struct {
	ANKETO_QUESTION_RESULT data;
	BOOL	set_flag;
} WFLBY_SYSTEM_ANKETO;




//-------------------------------------
///	WiFi���r�[���ʃV�X�e�����[�N
//=====================================
typedef struct _WFLBY_SYSTEM{
	SAVEDATA*				p_save;			// �Z�[�u�f�[�^
	WFLBY_SYSTEM_GLB		glbdata;		// �O���[�o���f�[�^
	WFLBY_SYSTEM_FLAG		flag;			// �t���O�S
	WFLBY_USER_MYPROFILE	myprofile;		// �����̃��[�J���v���t�B�[��
	WFLBY_SYSTEM_TALK		talk;			// ��b�f�[�^
	WFLBY_SYSTEM_GADGET		gadget;			// �K�W�F�b�g�N���f�[�^
	WFLBY_SYSTEM_EVENT		event;			// �C�x���g��ԃf�[�^
	WFLBY_FLOAT_DATA		floatdata;		// �t���[�g�f�[�^
	WFLBY_SYSTEM_APLFLAG	aplflag;		// �~�j�Q�[���t���O
	WFLBY_PLAYED_DATA		played;			// �V�񂾂��Ƃ̂�����̃f�[�^
	WFLBY_SYSTEM_GADGETRATE gadgetrate;		// �K�W�F�b�g�I�����[�g
	WFLBY_VIPFLAG			vipflag;		// VIP�t���O
	WFLBY_MINIGAME_PLIDX	mg_plidx;		// �~�j�Q�[���v���C���[�C���f�b�N�X
	WFLBY_SYSTEM_AIKOTOBA	aikotoba;		// �����t�f�[�^
	WFLBY_FIRE_DATA			fire_data;		// �ԉ΃f�[�^
	_WIFI_MACH_STATUS*		p_wificlubbuff;	// Wi-Fi�N���u�̏�Ԃ�NONE�ɂ��邽�߂̃o�b�t�@
	WFLBY_LASTACTION_CUE	lastactin_q;	// �Ō�ɂ������ƃL���[
	WFLBY_SYSTEM_MG			mg_data;		// �~�j�Q�[�����
	WFLBY_SYSTEM_ANKETO		anketo_result;	// �L��̃A���P�[�g���ʕۑ���
	u32						heapID;			// �q�[�vID
	u32						error;			// �V�X�e����error
}WFLBY_SYSTEM;



//-----------------------------------------------------------------------------
/**
 *	�t���[�g����C�x���g�f�[�^
 */
//-----------------------------------------------------------------------------
static const s32 sc_WFLBY_FLOAT_TIMING[ WFLBY_FLOAT_MAX ] = {
	0,
	WFLBY_FLOAT_COUNT_KANKAKU,
	WFLBY_FLOAT_COUNT_KANKAKU*2,

	(40 * 30),
	(40 * 30)+WFLBY_FLOAT_COUNT_KANKAKU,
	(40 * 30)+WFLBY_FLOAT_COUNT_KANKAKU*2,

	(80 * 30),
	(80 * 30)+WFLBY_FLOAT_COUNT_KANKAKU,
	(80 * 30)+WFLBY_FLOAT_COUNT_KANKAKU*2,
};



//-------------------------------------
///	�g���[�i�^�C�v���ʃf�[�^
//=====================================
typedef struct {
	u16		tr_type;
	u16		sex;
} WFLBY_SYSTEM_TRTYPE_SEX;

#define WFLBY_SYSTEMUNICHAR_NUM		(16)	// ���j�I���L�����N�^�̐�
static const WFLBY_SYSTEM_TRTYPE_SEX sc_WFLBY_SYSTEM_TRTYPE_SEX[WFLBY_SYSTEMUNICHAR_NUM] = {
	// ���j�I���L�����N�^16
	{
		BOY1,
		PM_MALE,
	},
	{
		BOY3,
		PM_MALE,
	},
	{
		MAN3,
		PM_MALE,
	},
	{
		BADMAN,
		PM_MALE,
	},
	{
		EXPLORE,
		PM_MALE,
	},
	{
		FIGHTER,
		PM_MALE,
	},
	{
		GORGGEOUSM,
		PM_MALE,
	},
	{
		MYSTERY,
		PM_MALE,
	},
	{
		GIRL1,
		PM_FEMALE,
	},
	{
		GIRL2,
		PM_FEMALE,
	},
	{
		WOMAN2,
		PM_FEMALE,
	},
	{
		WOMAN3,
		PM_FEMALE,
	},
	{
		IDOL,
		PM_FEMALE,
	},
	{
		LADY,	
		PM_FEMALE,
	},
	{
		COWGIRL,
		PM_FEMALE,
	},
	{
		GORGGEOUSW,
		PM_FEMALE,
	},
};




//-------------------------------------
///	�ԉ΍\���f�[�^
//=====================================
typedef struct {
	u16	move_type;
	u16	move_par_num;
} WFLBY_FIRE_MOVE_DATA;
// �\���f�[�^
// �p�[�Z���e�[�W�̍��v��WFLBY_FIRE_MOVE_PAR_MAX�ɂ��Ă�������
static const  WFLBY_FIRE_MOVE_DATA sc_WFLBY_FIRE_MOVE_DATA[] = {
//*
	{ WFLBY_FIRE_NORMAL,	80 },
	{ WFLBY_FIRE_NONE,		2 },	// ��������Ƃ߂�
	{ WFLBY_FIRE_KOUGO,		142 },
	{ WFLBY_FIRE_NONE,		2 },	// ��������Ƃ߂�
	{ WFLBY_FIRE_NAMI,		30 },
//*/
/*
	{ WFLBY_FIRE_NAMI,		20 },
	{ WFLBY_FIRE_NAMI,		2 },	// ��������Ƃ߂�
	{ WFLBY_FIRE_NAMI,		128 },
	{ WFLBY_FIRE_NAMI,		2 },	// ��������Ƃ߂�
	{ WFLBY_FIRE_NAMI,		104 },
//*/
};



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

// EOM�������R�[�h�̍Ō�ɂ��邩���`�F�b�N����
static BOOL WFLBY_SYSTEM_CheckStrEOMCode( const STRCODE* cp_str, u32 len );


// WiFi���r�[���C�u�����R�[���o�b�N
static void WFLBY_SYSTEM_CallbackUserIn( s32 userid, const void* cp_profile, void* p_work, BOOL mydata );
static void WFLBY_SYSTEM_CallbackUserOut( s32 userid, void* p_work );
static void WFLBY_SYSTEM_CallbackUserProfileUpdate( s32 userid, const void* cp_profile, void* p_work );
static void WFLBY_SYSTEM_CallbackEvent( PPW_LOBBY_TIME_EVENT event, void* p_work );
static void WFLBY_SYSTEM_CallbackCheckProfile( const void* cp_profile, u32 profile_size, void* p_work );


// ���E���v�f�[�^�̍쐬
static void WFLBY_SYSTEM_SetWldTimerData( WFLBY_USER_MYPROFILE* p_myprofile, WFLBY_WLDTIMER* p_wk );


// �C�x���g��񂪗��Ă��Ȃ��Ƃ��̎��Ԑ����`�F�b�N
static void WFLBY_SYSTEM_ContEndTime( WFLBY_SYSTEM* p_wk );

// �v���t�B�[���f�[�^������
static void WFLBY_SYSTEM_InitProfile( WFLBY_USER_MYPROFILE* p_myprofile, SAVEDATA* p_save, u32 heapID );
static void WFLBY_SYSTEM_DWC_SetMyProfile( WFLBY_SYSTEM* p_wk );
static void WFLBY_SYSTEM_MyProfile_SetCrc( WFLBY_USER_MYPROFILE* p_myprofile, const SAVEDATA* cp_save );
static BOOL WFLBY_SYSTEM_MyProfile_CheckCrc( const WFLBY_USER_MYPROFILE* cp_myprofile, const SAVEDATA* cp_save );
static void WFLBY_SYSTEM_MyProfileCopy_CheckData( WFLBY_USER_MYPROFILE* p_myprofile, const WFLBY_USER_PROFILE* cp_userdata, u32 profile_size );

static void WFLBY_SYSTEM_OSPrint_Profile( const WFLBY_USER_PROFILE* cp_profile );



// �C�x���g�t���O�f�[�^
static void WFLBY_SYSTEM_FLAG_ClearEvent( WFLBY_SYSTEM_FLAG* p_wk );
static void WFLBY_SYSTEM_EVENT_Count( WFLBY_SYSTEM_EVENT* p_wk, WFLBY_SYSTEM_FLAG* p_flag, WFLBY_FLOAT_DATA* p_float, WFLBY_FIRE_DATA* p_fire_data );
static void WFLBY_SYSTEM_EVENT_Init( WFLBY_SYSTEM_EVENT* p_wk );
static void WFLBY_SYSTEM_EVENT_PlayBgm( const WFLBY_SYSTEM_EVENT* cp_wk,  u32 bgm );
static void WFLBY_SYSTEM_EVENT_SetBgmVolume( const WFLBY_SYSTEM_EVENT* cp_wk );

// �~�j�Q�[���t���O
static void WFLBY_SYSTEM_MgFlag_SetLock( WFLBY_SYSTEM_APLFLAG* p_wk );
static BOOL WFLBY_SYSTEM_MgFlag_GetLock( const WFLBY_SYSTEM_APLFLAG* cp_wk );
static void WFLBY_SYSTEM_MgFlag_Cont( WFLBY_SYSTEM_APLFLAG* p_wk );

// �~�j�Q�[�����
static void WFLBY_SYSTEM_MG_Init( WFLBY_SYSTEM_MG* p_wk, u32 heapID );
static void WFLBY_SYSTEM_MG_Exit( WFLBY_SYSTEM_MG* p_wk );
static void WFLBY_SYSTEM_MG_SetMyStatus( WFLBY_SYSTEM_MG* p_wk, const WFLBY_USER_MYPROFILE* cp_profile, u32  heapID );

// �A�v�������I���t���O
static void WFLBY_SYSTEM_AplFlag_SetForceEnd( WFLBY_SYSTEM_APLFLAG* p_wk );
static BOOL WFLBY_SYSTEM_AplFlag_GetForceEnd( const WFLBY_SYSTEM_APLFLAG* cp_wk );

// �K�W�F�b�g�f�[�^
static void WFLBY_SYSTEM_GadGetClear( WFLBY_SYSTEM_GADGET* p_wk );

//  ��b���
static void WFLBY_SYSTEM_TALK_Init( WFLBY_SYSTEM_TALK* p_wk );
static void WFLBY_SYSTEM_TALK_EndClear( WFLBY_SYSTEM_TALK* p_wk );
static void WFLBY_SYSTEM_TALK_ExEndClear( WFLBY_SYSTEM_TALK* p_wk );
static BOOL WFLBY_SYSTEM_TALK_CheckMsk( const WFLBY_SYSTEM_TALK* cp_wk, u32 idx );
static void WFLBY_SYSTEM_TALK_SetMsk( WFLBY_SYSTEM_TALK* p_wk, u32 idx );
static void WFLBY_SYSTEM_TALK_ClearMsk( WFLBY_SYSTEM_TALK* p_wk, u32 idx );
static void WFLBY_SYSTEM_TALK_AddCount( WFLBY_SYSTEM_TALK* p_wk, u32 idx );
static void WFLBY_SYSTEM_TALK_ClearCount( WFLBY_SYSTEM_TALK* p_wk, u32 idx );
static void WFLBY_SYSTEM_TALK_SetSendDataNowSeq( WFLBY_SYSTEM_TALK* p_wk, u16 data );
static void WFLBY_SYSTEM_TALK_SendTalkAns( WFLBY_SYSTEM* p_wk, s32 userid, BOOL ans );
static void WFLBY_SYSTEM_TALK_SendTalkExEnd( WFLBY_SYSTEM* p_wk, s32 userid );
static void WFLBY_SYSTEM_TALK_StartRecvWait( WFLBY_SYSTEM_TALK* p_wk );
static void WFLBY_SYSTEM_TALK_StopRecvWait( WFLBY_SYSTEM_TALK* p_wk );
static void WFLBY_SYSTEM_TALK_UpdateRecvWait( WFLBY_SYSTEM_TALK* p_wk );
static BOOL WFLBY_SYSTEM_TALK_CheckRecvWait( const WFLBY_SYSTEM_TALK* cp_wk );
static s16	WFLBY_SYSTEM_TALK_GetRacvWaitTime( const WFLBY_SYSTEM_TALK* cp_wk );


// �g�s�b�N�f�[�^��ݒ肷��
static void WFLBY_SYSTEM_TOPIC_SetConnect( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic );
static void WFLBY_SYSTEM_TOPIC_SetItem( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic );
static void WFLBY_SYSTEM_TOPIC_SetMinigame( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic );
static void WFLBY_SYSTEM_TOPIC_SetFootBoard( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic );
static void WFLBY_SYSTEM_TOPIC_SetWldTimer( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic );
static void WFLBY_SYSTEM_TOPIC_SetNews( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic );
static void WFLBY_SYSTEM_TOPIC_SetMgResult( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic );
static void WFLBY_SYSTEM_TOPIC_SetDummy( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic );
static void WFLBY_SYSTEM_TOPIC_SetEvent( WFLBY_SYSTEM* p_wk, WFLBY_EVENTTYPE event );


// �t���[�g�f�[�^
static void WFLBY_SYSTEM_FLOAT_Init( WFLBY_FLOAT_DATA* p_float );
static void WFLBY_SYSTEM_FLOAT_Start( WFLBY_FLOAT_DATA* p_float );
static void WFLBY_SYSTEM_FLOAT_Main( WFLBY_FLOAT_DATA* p_float );
static inline void WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( u32 idx, u32* p_num, u32* p_ofs );
static u32 WFLBY_SYSTEM_FLOAT_SetReserve( WFLBY_FLOAT_DATA* p_float, u32 plidx, WFLBY_FLOAT_STATION_TYPE station );
static void WFLBY_SYSTEM_FLOAT_ResetReserve( WFLBY_FLOAT_DATA* p_float, u32 plidx );
static u32 WFLBY_SYSTEM_FLOAT_GetReserve( const WFLBY_FLOAT_DATA* cp_float, u32 idx );
static WFLBY_FLOAT_MOVE_STATE WFLBY_SYSTEM_FLOAT_GetMove( const WFLBY_FLOAT_DATA* cp_float, u32 floatidx );
static u32 WFLBY_SYSTEM_FLOAT_GetCount( const WFLBY_FLOAT_DATA* cp_float, u32 floatidx );
static void WFLBY_SYSTEM_FLOAT_SetAnm( WFLBY_FLOAT_DATA* p_float, u32 plidx );
static BOOL WFLBY_SYSTEM_FLOAT_GetAnm( const WFLBY_FLOAT_DATA* cp_float, u32 idx );
static void WFLBY_SYSTEM_FLOAT_CleanAnm( WFLBY_FLOAT_DATA* p_float, u32 idx );
static u32 WFLBY_SYSTEM_FLOAT_GetIdx( const WFLBY_FLOAT_DATA* cp_float, u32 plidx );
static void WFLBY_SYSTEM_FLOAT_SetReserveLock( WFLBY_FLOAT_DATA* p_float, u32 idx, WFLBY_FLOAT_STATION_TYPE station );
static BOOL WFLBY_SYSTEM_FLOAT_CheckReserveLock( const WFLBY_FLOAT_DATA* cp_float, u32 idx, WFLBY_FLOAT_STATION_TYPE station );
static WFLBY_FLOAT_STATION_TYPE WFLBY_SYSTEM_FLOAT_GetReserveLock( const WFLBY_FLOAT_DATA* cp_float, u32 idx );


// �ʐM��M�R�[���o�b�N
static void WFLBY_SYSTEM_COMMCMD_TALK_Req( s32 userid, const void* cp_data, u32 size, void* p_work );
static void WFLBY_SYSTEM_COMMCMD_TALK_Ans( s32 userid, const void* cp_data, u32 size, void* p_work );
static void WFLBY_SYSTEM_COMMCMD_TALK_Data( s32 userid, const void* cp_data, u32 size, void* p_work );
static void WFLBY_SYSTEM_COMMCMD_TALK_End( s32 userid, const void* cp_data, u32 size, void* p_work );
static void WFLBY_SYSTEM_COMMCMD_TOPIC_Data( s32 userid, const void* cp_data, u32 size, void* p_work );
static void WFLBY_SYSTEM_COMMCMD_GADGET_Data( s32 userid, const void* cp_data, u32 size, void* p_work );
static void WFLBY_SYSTEM_COMMCMD_FLOAT_On( s32 userid, const void* cp_data, u32 size, void* p_work );
static void WFLBY_SYSTEM_COMMCMD_FLOAT_Anm( s32 userid, const void* cp_data, u32 size, void* p_work );


// �v���t�B�[������
static BOOL WFLBY_SYSTEM_PROFILE_CheckLastTmpStatus( WFLBY_STATUS_TYPE status );

// �K�W�F�b�g�I�����[�g
static void WFLBY_SYSTEM_GADGETRATE_Init( WFLBY_SYSTEM_GADGETRATE* p_wk );
static void WFLBY_SYSTEM_GADGETRATE_CalcRate( WFLBY_SYSTEM_GADGETRATE* p_wk, WFLBY_ITEMTYPE item, u32 plno );
static WFLBY_ITEMTYPE WFLBY_SYSTEM_GADGETRATE_GetRandItem( const WFLBY_SYSTEM_GADGETRATE* cp_wk );


// VIP�t���O
static void WFLBY_SYSTEM_VIPFLAG_Set( WFLBY_VIPFLAG* p_wk, u32 idx, BOOL vip );
static BOOL WFLBY_SYSTEM_VIPFLAG_Get( const WFLBY_VIPFLAG* cp_wk, u32 idx );

// �����t�o�b�t�@
static void WFLBY_SYSTEM_AIKOTOBABUFF_Init( WFLBY_SYSTEM_AIKOTOBA* p_wk, u32 heapID );
static void WFLBY_SYSTEM_AIKOTOBABUFF_Exit( WFLBY_SYSTEM_AIKOTOBA* p_wk );
static void WFLBY_SYSTEM_AIKOTOBABUFF_SetData( WFLBY_SYSTEM_AIKOTOBA* p_wk, u32 idx, BOOL aikotoba, u32 number );
static BOOL WFLBY_SYSTEM_AIKOTOBABUFF_GetFlag( const WFLBY_SYSTEM_AIKOTOBA* cp_wk, u32 idx );
static const WFLBY_AIKOTOBA_DATA* WFLBY_SYSTEM_AIKOTOBABUFF_GetData( const WFLBY_SYSTEM_AIKOTOBA* cp_wk, u32 idx );
static void WFLBY_SYSTEM_AIKOTOBA_Make( PMSW_AIKOTOBA_TBL* p_wordtbl, u32 number, WFLBY_AIKOTOBA_DATA* p_aikotoba );


// �ԉ΃f�[�^
static void WFLBY_SYSTEM_FIREDATA_Init( WFLBY_FIRE_DATA* p_wk, u32 fire_start, u32 fire_end );
static void WFLBY_SYSTEM_FIREDATA_Start( WFLBY_FIRE_DATA* p_wk );
static void WFLBY_SYSTEM_FIREDATA_Main( WFLBY_FIRE_DATA* p_wk );
static u32 WFLBY_SYSTEM_FIREDATA_GetCount( const WFLBY_FIRE_DATA* cp_wk );
static WFLBY_FIRE_TYPE WFLBY_SYSTEM_FIREDATA_GetType( const WFLBY_FIRE_DATA* cp_wk );


// �Ō�ɂ������ƃL���[
static void WFLBY_SYSTEM_LASTACTQ_Init( WFLBY_LASTACTION_CUE* p_wk, const SAVEDATA* cp_save );
static void WFLBY_SYSTEM_LASTACTQ_Push( WFLBY_LASTACTION_CUE* p_wk, WFLBY_LASTACTION_TYPE data, s32 userid );
static BOOL WFLBY_SYSTEM_LASTACTQ_ResetUserID( WFLBY_LASTACTION_CUE* p_wk, s32 before_id, s32 after_id );
static WFLBY_LASTACTION_TYPE WFLBY_SYSTEM_LASTACTQ_Pop( WFLBY_LASTACTION_CUE* p_wk );
static WFLBY_LASTACTION_TYPE WFLBY_SYSTEM_LASTACTQ_GetData( WFLBY_LASTACTION_CUE* p_wk, u32 idx );
static s32 WFLBY_SYSTEM_LASTACTQ_GetUserID( WFLBY_LASTACTION_CUE* p_wk, u32 idx );
static void WFLBY_SYSTEM_LASTACTQ_SetCrc( WFLBY_LASTACTION_CUE* p_wk );
static void WFLBY_SYSTEM_LASTACTQ_CheckCrc( WFLBY_LASTACTION_CUE* p_wk );
static BOOL WFLBY_SYSTEM_LASTACTQ_GetCrcError( const WFLBY_LASTACTION_CUE* cp_wk );

// tr_type���烆�j�I���L�����N�^�������ł̃g���[�i�[�ԍ����擾����
static u32 WFLBY_TRTYPE_UNIONCHARNO_Get( u32 tr_type );
static u32 WFLBY_UNIONCHARNO_TRTYPE_Get( u32 union_char_no );

//-----------------------------------------------------------------------------
/**
 *		�ʐM�f�[�^
 */
//-----------------------------------------------------------------------------
static const DWC_LOBBY_MSGCOMMAND	sc_WFLBY_SYSTEM_COMMCMD[ WFLBY_SYSTEM_MAIN_COMMCMD_NUM ] = {

	// ��b
	{ WFLBY_SYSTEM_COMMCMD_TALK_Req,		sizeof(WFLBY_SYSTEM_TALK_DATA) },
	{ WFLBY_SYSTEM_COMMCMD_TALK_Ans,		sizeof(WFLBY_SYSTEM_TALK_DATA) },
	{ WFLBY_SYSTEM_COMMCMD_TALK_Data,		sizeof(WFLBY_SYSTEM_TALK_DATA) },
	{ WFLBY_SYSTEM_COMMCMD_TALK_End,		sizeof(WFLBY_SYSTEM_TALK_DATA) },

	// �g�s�b�N
	{ WFLBY_SYSTEM_COMMCMD_TOPIC_Data,		sizeof(WFLBY_TOPIC) },

	// �K�W�F�b�g
	{ WFLBY_SYSTEM_COMMCMD_GADGET_Data,		sizeof(u32) },

	// �t���[�g
	{ WFLBY_SYSTEM_COMMCMD_FLOAT_On,		sizeof(u32)	},
	{ WFLBY_SYSTEM_COMMCMD_FLOAT_Anm,		sizeof(u32)	},
};






//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[���ʏ����V�X�e��
 *
 *	@param	p_save	�Z�[�u�f�[�^
 *	@param	heapID	�q�[�vID
 *
 *	@return	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
WFLBY_SYSTEM* WFLBY_SYSTEM_Init( SAVEDATA* p_save, u32 heapID )
{
	WFLBY_SYSTEM* p_wk;

	p_wk = sys_AllocMemory( heapID, sizeof(WFLBY_SYSTEM) );
	memset( p_wk, 0, sizeof(WFLBY_SYSTEM) );

	// �Z�[�u�f�[�^�ۑ�
	p_wk->p_save = p_save;

	// �q�[�v
	p_wk->heapID = heapID;

	// WiFi���r�[���C�u����������
	{
		DWC_LOBBY_CALLBACK callback;
		callback.p_user_in				= WFLBY_SYSTEM_CallbackUserIn;
		callback.p_user_out				= WFLBY_SYSTEM_CallbackUserOut;
		callback.p_profile_update		= WFLBY_SYSTEM_CallbackUserProfileUpdate;
		callback.p_event				= WFLBY_SYSTEM_CallbackEvent;
		callback.p_check_profile		= WFLBY_SYSTEM_CallbackCheckProfile;

		DWC_LOBBY_Init( heapID, p_wk->p_save,
				sizeof(WFLBY_USER_PROFILE),	// �v���t�B�[���T�C�Y
				&callback,					// �R�[���o�b�N
				p_wk );						// ���[�N
	}
	
	
	
	// �O���[�o���f�[�^�쐬
	{
		int i;
		
		p_wk->glbdata.p_lobbynews = NEWS_DSET_Init( heapID );

		for( i=0; i<NEWS_TOPICNAME_NUM; i++ ){
			p_wk->glbdata.p_mystatus[i] = MyStatus_AllocWork( heapID );
		}
		
	}

	// �����̃v���t�B�[���쐬
	WFLBY_SYSTEM_InitProfile( &p_wk->myprofile, p_wk->p_save, heapID );

	// ��b�f�[�^������
	WFLBY_SYSTEM_TALK_Init( &p_wk->talk );

	// �K�W�F�b�g
	WFLBY_SYSTEM_GadGetClear( &p_wk->gadget );

	// �ʐM�R�}���h�ݒ�
	DWC_LOBBY_SetMsgCmd( sc_WFLBY_SYSTEM_COMMCMD, WFLBY_SYSTEM_MAIN_COMMCMD_NUM, p_wk );

	// �C�x���g�t���O�ݒ�
	WFLBY_SYSTEM_EVENT_Init( &p_wk->event );

	// �t���[�g�f�[�^�N���A
	WFLBY_SYSTEM_FLOAT_Init( &p_wk->floatdata );

	// �K�W�F�b�g���[�g������
	WFLBY_SYSTEM_GADGETRATE_Init( &p_wk->gadgetrate );

	// �����t�o�b�t�@�̏�����
	WFLBY_SYSTEM_AIKOTOBABUFF_Init( &p_wk->aikotoba, heapID );

	// �����L���[������
	WFLBY_SYSTEM_LASTACTQ_Init( &p_wk->lastactin_q, p_wk->p_save );

	// �~�j�Q�[����񃏁[�N������
	WFLBY_SYSTEM_MG_Init( &p_wk->mg_data, heapID );
	
	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�̔j��
 *
 *	@param	p_wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_Exit( WFLBY_SYSTEM* p_wk )
{
	// �L����̃Z�[�u
	{
		WFLBY_SAVEDATA* p_hiroba;
		WFLBY_ANKETO	anketo_data;

		WFLBY_SYSTEM_GetProfileAnketoData( &p_wk->myprofile.profile, &anketo_data );

		p_hiroba = SAVEDATA_WFLBY_GetWork( p_wk->p_save );
		SAVEDATA_WFLBY_SetQuestionNumber( p_hiroba, anketo_data.anketo_no );
		SAVEDATA_WFLBY_SetMyAnswer( p_hiroba, anketo_data.select );
	}

	// �~�j�Q�[����񃏁[�N�j��
	WFLBY_SYSTEM_MG_Exit( &p_wk->mg_data );

	// �����t�o�b�t�@�̔j��
	WFLBY_SYSTEM_AIKOTOBABUFF_Exit( &p_wk->aikotoba );
	
	// �ʐM�R�}���h�j��
	{
		DWC_LOBBY_CleanMsgCmd();
	}
	
	// �O���[�o���f�[�^�j��
	{
		int i;

		NEWS_DSET_Exit( p_wk->glbdata.p_lobbynews );

		for( i=0; i<NEWS_TOPICNAME_NUM; i++ ){
			sys_FreeMemoryEz( p_wk->glbdata.p_mystatus[i] );
		}
	}

	// WiFi���r�[���C�u�����j��
	DWC_LOBBY_Exit();
	
	sys_FreeMemoryEz( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C������
 *
 *	@param	p_wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_Main( WFLBY_SYSTEM* p_wk )
{

	
#ifdef WFLBY_DEBUG_NPC_IN
	
	if( sys.trg & PAD_BUTTON_L ){
		WFLBY_USER_PROFILE profile;
		int i;
		static const u32 tr_type[] = {
			0,	   
			1,     
			2,     
			3,   
			4,  
			5,  
			6,
			7,  
			8,  
			9,    
			10,    
			11,   
			12,   
			13,     
			14,     
			15,  
		};

		//�����̃v���t�B�[������{�ɂ���
		profile = p_wk->myprofile.profile;
		
		profile.status		= WFLBY_STATUS_LOGIN;		// �v���C���[�X�e�[�^�X
		for( i=1; i<20; i++ ){
			profile.userid		= i;						//  ���r�[�����[�UID
			profile.sex			= gf_mtRand() % PM_NEUTRAL;	// ����
			profile.tr_type		= gf_mtRand() % NELEMS(tr_type);	// �g���[�i�̌�����
			DWC_LOBBY_DEBUG_PlayerIN( &profile, i );
		}

		if( sys.trg & PAD_BUTTON_R ){
			int i;
			WFLBY_USER_PROFILE* p_profile;
			// �S������C�ɃT�u�`�����l���̂ق��ɂ������Č���
			for( i=1; i<20; i++ ){
				p_profile = (WFLBY_USER_PROFILE*)WFLBY_SYSTEM_GetUserProfile( p_wk, i );
				p_profile->status = WFLBY_STATUS_WORLDTIMER;
				
				DWC_LOBBY_DEBUG_SetProfile( p_profile, i );
			}
		}
	}
#endif

	// �g�s�b�N���C��
	NEWS_DSET_Main( p_wk->glbdata.p_lobbynews );

	// ���E���Ԃ̍X�V
	{
		s64 time;
		DWC_LOBBY_GetTime( &time );
		p_wk->glbdata.worldtime_s64 = time;
		WFLBY_TIME_Set( &p_wk->glbdata.worldtime, &time );
	}

	// 080705	tomoya 
	// BTS�ʐM789�ւ̑Ώ�
	{
		// �o�ߎ��Ԃ��疾�炩�ɃI�����̎��ԂɂȂ�����
		// �����I�ɏI��点��
		WFLBY_SYSTEM_ContEndTime( p_wk );
	}

	// ��b��M�҂��J�E���g
	{
		WFLBY_SYSTEM_TALK_UpdateRecvWait( &p_wk->talk );
	}

	// �K�W�F�b�g�f�[�^�N���A
	WFLBY_SYSTEM_GadGetClear( &p_wk->gadget );

	// �C�x���g�t���O�N���A
	WFLBY_SYSTEM_FLAG_ClearEvent( &p_wk->flag );

	// �^�C���C�x���g�J�E���g
	WFLBY_SYSTEM_EVENT_Count( &p_wk->event, &p_wk->flag, 
			&p_wk->floatdata, &p_wk->fire_data );

	// �~�j�Q�[���t���O�Ǘ�
	WFLBY_SYSTEM_MgFlag_Cont( &p_wk->aplflag );

	// �t���[�g�f�[�^���C��
	WFLBY_SYSTEM_FLOAT_Main( &p_wk->floatdata );

	// �ԉ΃f�[�^���C��
	WFLBY_SYSTEM_FIREDATA_Main( &p_wk->fire_data );

	// ����Q��CRC�`�F�b�N�G���[���V�X�e�����G���[�ɔ��f
	{
		BOOL crc_error;
		crc_error = WFLBY_SYSTEM_LASTACTQ_GetCrcError( &p_wk->lastactin_q );
		if( crc_error == TRUE ){
			p_wk->error = WFLBY_SYSTEM_ERR_CRC;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	VBLANK	����
 *
 *	@param	p_wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_VBlank( WFLBY_SYSTEM* p_wk )
{

}

//----------------------------------------------------------------------------
/**
 *	@brief	Wi-Fi�Ђ�΃V�X�e��	�G���[��Ԏ擾
 *
 *	@param	cp_wk	�V�X�e�����[�N
 *
 *	@retval	WFLBY_SYSTEM_ERR_NONE,
 *	@retval	WFLBY_SYSTEM_ERR_CRC,		// CRC�`�F�b�N�ŃG���[���N����
 */
//-----------------------------------------------------------------------------
WFLBY_SYSTEM_ERR_TYPE WFLBY_SYSTEM_GetError( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->error;
}

//----------------------------------------------------------------------------
/**
 *	@brief	Wi-Fi�Ђ�ΒʐM�ߏ��Ԃ̎擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�ʐM�ߏ���
 *	@retval	FALSE	�ʐM�ʏ���
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetExcessFoodError( const WFLBY_SYSTEM* cp_wk )
{
	// �ʐM�ߏ��Ԃ��擾����
	return DWC_LOBBY_CheckExcessFlood();
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�j���[�X�����擾����
 *
 *	@param	p_wk	���[�N
 *
 *	@return	���r�[�f�[�^
 */
//-----------------------------------------------------------------------------
NEWS_DATA* WFLBY_SYSTEM_GetLobbyData( WFLBY_SYSTEM* p_wk )
{
	return p_wk->glbdata.p_lobbynews;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���Ԏ擾	���݂̏�Ԃ��쐬���āA���̃f�[�^��Ԃ��܂��B
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
const WFLBY_WLDTIMER* WFLBY_SYSTEM_GetWldTimer( WFLBY_SYSTEM* p_wk )
{
	// ���̏�Ԃ����[�N�ɐݒ�
	WFLBY_SYSTEM_SetWldTimerData( &p_wk->myprofile, &p_wk->glbdata.worldtimer );
	
	return &p_wk->glbdata.worldtimer;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���݂̎��Ԃ��擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	����
 */
//-----------------------------------------------------------------------------
const WFLBY_TIME* WFLBY_SYSTEM_GetWldTime( const WFLBY_SYSTEM* cp_wk )
{
	return &cp_wk->glbdata.worldtime;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V���������Ă����l���`�F�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�`�F�b�N����l�̃C���f�b�N�X
 *
 *	@retval	TRUE	�V���������Ă���
 *	@retval	FALSE	�����Ă��ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_FLAG_GetUserIn( WFLBY_SYSTEM* p_wk, u32 idx )
{
	if( p_wk->flag.user_in & (1<<idx) ){
		p_wk->flag.user_in &= ~(1<<idx);
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�����ގ��҂����邩�`�F�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�`�F�b�N����C���f�b�N�X
 *
 *	@retval	TRUE		�ގ�����
 *	@retval	FALSE		�ގ����ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_FLAG_GetUserOut( WFLBY_SYSTEM* p_wk, u32 idx )
{
	if( p_wk->flag.user_out & (1<<idx) ){
		p_wk->flag.user_out &= ~(1<<idx);
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����C�x���g�������������`�F�b�N
 *
 *	@param	p_wk	���[�N
 *	@param	type	�C�x���g�^�C�v
 *
 *	@retval	TRUE	�����C�x���g����������
 *	@retval	FALSE	�����J�n���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_FLAG_GetEventStart( WFLBY_SYSTEM* p_wk, WFLBY_EVENT_CHG_TYPE type )
{
	if( p_wk->flag.event_on & (1 << type) ){
		return TRUE;
	}
	return FALSE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�v���t�B�[�����X�V���ꂽ���`�F�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	idx			���[�UIDX
 *
 *	@retval	TRUE	���̃��[�U�̃v���t�B�[�����X�V���ꂽ
 *	@retval	FALSE	�X�V����Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_FLAG_GetUserProfileUpdate( WFLBY_SYSTEM* p_wk, u32 idx )
{
	if( p_wk->flag.profile_up & (1<<idx) ){
		p_wk->flag.profile_up &= ~(1<<idx);
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z�[�u�f�[�^�擾
 *
 *	@param	p_wk	���[�N
 *
 *	@return	�Z�[�u�f�[�^
 */
//-----------------------------------------------------------------------------
SAVEDATA* WFLBY_SYSTEM_GetSaveData( WFLBY_SYSTEM* p_wk )
{
	return p_wk->p_save;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���Z�E�X�\�����t���O���擾����
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_FLAG_GetArceus( const WFLBY_SYSTEM* cp_wk )
{
	GF_ASSERT( cp_wk );	// �{����SYSTEM����Ȃ����ǕK�v�Ȃ悤�ɂȂ邩������Ȃ�����
	return DWC_LOBBY_GetRoomData( DWC_LOBBY_ROOMDATA_ARCEUS );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�𓚂����̂��`�F�b�N����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	������
 *	@retval	FALSE	�����Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_FLAG_GetAnketoInput( const WFLBY_SYSTEM* cp_wk )
{
	GF_ASSERT( cp_wk );

	return cp_wk->event.anketo_input;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g���ʂ������̂��ǂ���
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�݂�
 *	@retval	FALSE	�݂ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_FLAG_GetAnketoView( const WFLBY_SYSTEM* cp_wk )
{
	GF_ASSERT( cp_wk );

	return cp_wk->event.anketo_output;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�̌��ʂ������̂���ݒ�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_FLAG_SetAnketoView( WFLBY_SYSTEM* p_wk )
{
	p_wk->event.anketo_output = TRUE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���	���b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	���b�N
 *	@retval	FALSE	���b�N�O
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_Event_GetLock( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->event.lock;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�����m���Ԃ��`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�I�����m����
 *	@retval	FALSE	�I�����m���ԑO
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_Event_GetEndCM( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->event.endcm;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���	�����I��
 *
 *	@param	cp_wk 
 *
 *	@retval	TRUE	�����̏I��
 *	@retval	FALSE	�����܂��I������Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_Event_GetEnd( const WFLBY_SYSTEM* cp_wk )
{
	if( (cp_wk->event.end == TRUE) && (cp_wk->event.end_count <= 0) ){
		return TRUE;
	}else{
		return FALSE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���	�ԉ�
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	WFLBY_EVENT_HANABI_BEFORE,	// �ԉΑO
 *	@retval	WFLBY_EVENT_HANABI_PLAY,	// �ԉΒ�
 *	@retval	WFLBY_EVENT_HANABI_AFTER,	// �ԉΌ�
 */
//-----------------------------------------------------------------------------
WFLBY_EVENT_HANABI_ST WFLBY_SYSTEM_Event_GetHanabi( const WFLBY_SYSTEM* cp_wk )
{
	if( (cp_wk->event.hanabi_count <= 0) ){
		return cp_wk->event.hababi;
	}
	return WFLBY_EVENT_HANABI_BEFORE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���	�p���[�h
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE		�p���[�h��
 *	@retval	FALSE		�p���[�h���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_Event_GetParade( const WFLBY_SYSTEM* cp_wk )
{
	if( (cp_wk->event.parade == TRUE) && (cp_wk->event.parade_count <= 0) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���	�~�j�Q�[����~
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�~�j�Q�[����~
 *	@retval	FALSE	�~�j�Q�[�����s
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_Event_GetMiniGameStop( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->event.mini;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���	�l�I���C�x���g
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�l�I���C�x���g�J�n��
 *	@retval	FALSE	�l�I���C�x���g�J�n�O
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_Event_GetNeon( const WFLBY_SYSTEM* cp_wk )
{
	if( (cp_wk->glbdata.topic_event_start == TRUE) && (cp_wk->event.neon_count <= 0) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���	�����l�I��
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	PPW_LOBBY_TIME_EVENT_NEON_A0�`PPW_LOBBY_TIME_EVENT_NEON_A5
 */
//-----------------------------------------------------------------------------
PPW_LOBBY_TIME_EVENT WFLBY_SYSTEM_Event_GetNeonRoom( const WFLBY_SYSTEM* cp_wk )
{
	if( (cp_wk->event.neon_count <= 0) ){
		return cp_wk->event.neon_room;
	}
	return PPW_LOBBY_TIME_EVENT_NEON_A0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���	���l�I��
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	PPW_LOBBY_TIME_EVENT_NEON_B0�`PPW_LOBBY_TIME_EVENT_NEON_B3
 */
//-----------------------------------------------------------------------------
PPW_LOBBY_TIME_EVENT WFLBY_SYSTEM_Event_GetNeonFloor( const WFLBY_SYSTEM* cp_wk )
{
	if( (cp_wk->event.neon_count <= 0) ){
		return cp_wk->event.neon_floor;
	}
	return PPW_LOBBY_TIME_EVENT_NEON_B0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���	���j�������g�l�I��
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	PPW_LOBBY_TIME_EVENT_NEON_C0�`PPW_LOBBY_TIME_EVENT_NEON_C3
 */
//--------------------PPW_LOBBY_TIME_EVENT_FIRE_WORKS_START---------------------------------------------------------
PPW_LOBBY_TIME_EVENT WFLBY_SYSTEM_Event_GetNeonMonu( const WFLBY_SYSTEM* cp_wk )
{
	if( (cp_wk->event.neon_count <= 0) ){
		return cp_wk->event.neon_monu;
	}
	return PPW_LOBBY_TIME_EVENT_NEON_C0;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���t���O	���b�N
 *		�e��BCANCEL�������Ƀ��b�N��������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_MGFLAG_SetLock( WFLBY_SYSTEM* p_wk )
{
	WFLBY_SYSTEM_MgFlag_SetLock( &p_wk->aplflag );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���t���O	���b�N�����擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	���b�N��
 *	@retval	FALSE	���b�N���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_MGFLAG_GetLock( const WFLBY_SYSTEM* cp_wk )
{
	return WFLBY_SYSTEM_MgFlag_GetLock( &cp_wk->aplflag );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���G���[�I��	�`�F�b�N�֐�
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�G���[�I������
 *	@retval	FALSE	�ʏ�I������
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_MG_CheckErrorEnd( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->mg_data.error_end;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���G���[�I��	�ݒ�֐�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_MG_SetErrorEnd( WFLBY_SYSTEM* p_wk, BOOL flag )
{
	p_wk->mg_data.error_end = flag;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���ʐM����̃��r�[���C���f�b�N�X	�o�b�t�@�N���A
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_PLIDX_Clear( WFLBY_SYSTEM* p_wk )
{
	MI_CpuClear32( &p_wk->mg_plidx, sizeof(WFLBY_MINIGAME_PLIDX) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���ʐM����̃��r�[�Ȃ��C���f�b�N�X	�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	plidx		���r�[���C���f�b�N�X
 *	@param	netid		�ʐMID
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_PLIDX_Set( WFLBY_SYSTEM* p_wk, s32 userid, u32 netid )
{
	u32 plidx;
	
	GF_ASSERT( netid < WFLBY_MINIGAME_MAX );

	// userid����plidx�����߂�
	plidx = DWC_LOBBY_GetUserIDIdx( userid );
	GF_ASSERT( plidx != DWC_LOBBY_USERIDTBL_IDX_NONE );

//	OS_TPrintf( "idx %d, netid %d\n", plidx, netid );
	
	p_wk->mg_plidx.plidx[ netid ] = plidx;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���ʐM����̃��r�[�Ȃ��C���f�b�N�X	�����̃f�[�^��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	netid		�l�b�gID
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_PLIDX_SetMyData( WFLBY_SYSTEM* p_wk, u32 netid )
{
	s32 myuserid;

	myuserid = DWC_LOBBY_GetMyUserID();
	WFLBY_SYSTEM_PLIDX_Set( p_wk, myuserid, netid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���ʐM����̃��r�[���C���f�b�N�X	�擾
 *
 *	@param	cp_wk		���[�N
 *	@param	p_buff		�f�[�^�i�[��
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_PLIDX_Get( const WFLBY_SYSTEM* cp_wk, WFLBY_MINIGAME_PLIDX* p_buff )
{
	MI_CpuCopy8( &cp_wk->mg_plidx, p_buff, sizeof(WFLBY_MINIGAME_PLIDX) );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A�v���������I�ɏI���������̂���ݒ�
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_APLFLAG_SetForceEnd( WFLBY_SYSTEM* p_wk )
{
	WFLBY_SYSTEM_AplFlag_SetForceEnd( &p_wk->aplflag );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����I���t���O���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�����I��
 *	@retval	FALSE	�����I������Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_APLFLAG_GetForceEnd( const WFLBY_SYSTEM* cp_wk )
{
	return WFLBY_SYSTEM_AplFlag_GetForceEnd( &cp_wk->aplflag );
}



//----------------------------------------------------------------------------
/**
 *	@brief	BGM���p���[�h�ɕς��邱�Ƃ����b�N���̓��b�N��������
 *
 *	@param	p_wk		���[�N
 *	@param	flag		TRUE�F���b�N	FALSE�F���b�N����
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetLockBGMChange( WFLBY_SYSTEM* p_wk, BOOL flag )
{
	p_wk->event.bgm_lock = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������ׂ�BGM�𗬂�
 *
 *	@param	cp_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetNowBGM( const WFLBY_SYSTEM* cp_wk )
{
	if( (cp_wk->event.parade == TRUE) && (cp_wk->event.parade_count <= 0) ){

		Snd_DataSetByScene( SND_SCENE_WIFI_LOBBY_HIROBA, SEQ_PL_WIFIPARADE, 0 );	// �p���[�h

		// �{�����[���ݒ�
		WFLBY_SYSTEM_EVENT_SetBgmVolume( &cp_wk->event );
	}else{

		// BGM�`�F���W
		Snd_DataSetByScene( SND_SCENE_WIFI_LOBBY_HIROBA, SEQ_PL_WIFIUNION, 0 );	//wifi���r�[�Đ�

		// �{�����[���ݒ�
		WFLBY_SYSTEM_EVENT_SetBgmVolume( &cp_wk->event );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�����[�����Ƃ���Ԑݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�t���O	TRUE�F���Ƃ�	FALSE�F�ʏ�
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetBGMVolumeDown( WFLBY_SYSTEM* p_wk, BOOL flag )
{
	if( (flag != p_wk->event.bgm_vl_down) ){

		p_wk->event.bgm_vl_down = flag;

		// ���Ɖ������Ȃ牺���Ă���
		WFLBY_SYSTEM_EVENT_SetBgmVolume( &p_wk->event );
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�����̃o�b�t�@�C���f�b�N�X���擾����
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetMyIdx( const WFLBY_SYSTEM* cp_wk )
{
	s32 myuserid = DWC_LOBBY_GetMyUserID();
	return DWC_LOBBY_GetUserIDIdx( myuserid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃��[�UID���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	���[�UID	�i���r�[���C�u�����Ǘ�ID�j
 */
//-----------------------------------------------------------------------------
s32 WFLBY_SYSTEM_GetMyUserID( const WFLBY_SYSTEM* cp_wk )
{
	return DWC_LOBBY_GetMyUserID();
}

//----------------------------------------------------------------------------
/**
 *	@brief	idx�̃v���C���[�������ɓ����Ă��邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE	�����ɓ����Ă�
 *	@retval	FALSE	�����ɓ����ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetUserRoomIn( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	DWC_LOBBY_CHANNEL_USERID data;

	// ���[�UID�e�[�u���擾
	DWC_LOBBY_GetUserIDTbl( &data );
	
	if( data.cp_tbl[ idx ] != DWC_LOBBY_INVALID_USER_ID ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�v���t�B�[�����擾����
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 *
 *	@return	���[�U�v���t�B�[��
 */
//-----------------------------------------------------------------------------
const WFLBY_USER_PROFILE* WFLBY_SYSTEM_GetUserProfile( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	s32 user;
	DWC_LOBBY_CHANNEL_USERID usertbl;
	const WFLBY_USER_PROFILE* cp_data;

	GF_ASSERT( idx < WFLBY_PLAYER_MAX );

	// ���[�UID�e�[�u���擾
	DWC_LOBBY_GetUserIDTbl( &usertbl );


	// ���[�UID�̃v���t�B�[���擾
	user = usertbl.cp_tbl[ idx ];

	// ����INDEX�̐l�����邩�`�F�b�N
	if( user == DWC_LOBBY_INVALID_USER_ID ){
		return NULL;
	}

	// �����̃f�[�^�Ȃ烍�[�J����̎����̃f�[�^���擾����
	if( user == DWC_LOBBY_GetMyUserID() ){
		cp_data = &cp_wk->myprofile.profile;
	}else{
		cp_data = (const WFLBY_USER_PROFILE*)DWC_LOBBY_GetUserProfile( user );
	}

	return cp_data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃v���t�B�[�����擾����	���C�u�����̂��̂𒼐�
 *
 *	@param	cp_wk		���[�N
 *
 *	@return �v���t�B�[��
 */
//-----------------------------------------------------------------------------

const WFLBY_USER_PROFILE* WFLBY_SYSTEM_GetMyProfile( const WFLBY_SYSTEM* cp_wk )
{
	const WFLBY_USER_PROFILE* cp_data;

	cp_data = (const WFLBY_USER_PROFILE*)DWC_LOBBY_GetMyProfile();

	return cp_data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃v���t�B�[���擾	�V�X�e�����̂��̂��i���ڏ������ނ悤�j
 *
 *	@param	p_wk	���[�N
 *
 *	@return	�v���t�B�[��
 *
 *
 *	*�������񂾂�K��WFLBY_SYSTEM_SetMyProfile�����Ă��������B
 */
//-----------------------------------------------------------------------------
WFLBY_USER_PROFILE* WFLBY_SYSTEM_GetMyProfileLocal( WFLBY_SYSTEM* p_wk )
{
	// CRC�`�F�b�N
	if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
		p_wk->error = WFLBY_SYSTEM_ERR_CRC;
	}
	return &p_wk->myprofile.profile;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�e�[�^�X�̕ύX��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	status		�X�e�[�^�X
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetMyStatus( WFLBY_SYSTEM* p_wk, WFLBY_STATUS_TYPE status )
{
	s32 userid;
	
	GF_ASSERT( status < WFLBY_STATUS_NUM );

	// CRC�`�F�b�N
	if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
		p_wk->error = WFLBY_SYSTEM_ERR_CRC;
		return ;
	}

	// �ω����邩�`�F�b�N
	if( p_wk->myprofile.profile.status == status ){
		return ;
	}

	// ��b���N�G�X�g���ɉ�b�ȊO�̏�ԂɑJ�ڂ���Ƃ��A
	// ��b���N�G�X�g��j������
	if( status != WFLBY_STATUS_TALK ){
		if( WFLBY_SYSTEM_CheckTalk( p_wk ) == TRUE ){

			// �e�Ȃ狭���I��
			if( WFLBY_SYSTEM_CheckTalkReq( p_wk ) == FALSE ){
				// ��b�I��
				userid = DWC_LOBBY_GetUserIdxID( p_wk->talk.talk_idx );
				WFLBY_SYSTEM_TALK_SendTalkExEnd( p_wk, p_wk->talk.talk_idx );	// ������ɂނ�Ƃ���
			}
			WFLBY_SYSTEM_TALK_ExEndClear( &p_wk->talk );
		}
	}

	// �����A�~�j�Q�[���Ȃǂ̃X�e�[�^�X��ύX����̂ł���΁A
	// �Ō�ɂ������Ƃ��X�V����
	switch( p_wk->myprofile.profile.status ){
	case WFLBY_STATUS_BALLSLOW:		// �ʂ���
		WFLBY_SYSTEM_LASTACTQ_Push( &p_wk->lastactin_q, WFLBY_LASTACTION_BALLSLOW, DWC_LOBBY_INVALID_USER_ID );
		break;
	case WFLBY_STATUS_BALANCEBALL:	// �ʏ��
		WFLBY_SYSTEM_LASTACTQ_Push( &p_wk->lastactin_q, WFLBY_LASTACTION_BALANCEBALL, DWC_LOBBY_INVALID_USER_ID );
		break;
	case WFLBY_STATUS_BALLOON:		// ���D���
		WFLBY_SYSTEM_LASTACTQ_Push( &p_wk->lastactin_q, WFLBY_LASTACTION_BALLOON, DWC_LOBBY_INVALID_USER_ID );
		break;
	case WFLBY_STATUS_FOOTBOAD00:	// ���Ճ{�[�h
		WFLBY_SYSTEM_LASTACTQ_Push( &p_wk->lastactin_q, WFLBY_LASTACTION_FOOTWHITE, DWC_LOBBY_INVALID_USER_ID );
		break;
	case WFLBY_STATUS_FOOTBOAD01:	// ���Ճ{�[�h
		WFLBY_SYSTEM_LASTACTQ_Push( &p_wk->lastactin_q, WFLBY_LASTACTION_FOOTBLACK, DWC_LOBBY_INVALID_USER_ID );
		break;
	case WFLBY_STATUS_WORLDTIMER:	// ���E���v
		WFLBY_SYSTEM_LASTACTQ_Push( &p_wk->lastactin_q, WFLBY_LASTACTION_WLDTIMER, DWC_LOBBY_INVALID_USER_ID );
		break;
	case WFLBY_STATUS_TOPIC:		// �j���[�X
		WFLBY_SYSTEM_LASTACTQ_Push( &p_wk->lastactin_q, WFLBY_LASTACTION_NEWS, DWC_LOBBY_INVALID_USER_ID );
		break;

	// tomoya 080618
	// �A���P�[�g�̓C�x���g�̂ق��ł��Ă�
//	case WFLBY_STATUS_ANKETO:		// �A���P�[�g
//		WFLBY_SYSTEM_LASTACTQ_Push( &p_wk->lastactin_q, WFLBY_LASTACTION_ANKETO, DWC_LOBBY_INVALID_USER_ID );
//		break;
	default:
		break;
	}

	p_wk->myprofile.profile.status = status;
	WFLBY_SYSTEM_MyProfile_SetCrc( &p_wk->myprofile, p_wk->p_save );
	WFLBY_SYSTEM_DWC_SetMyProfile( p_wk );	// �X�V
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ō�ɂ������Ɛݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	action		��������
 *	@param	userid		���[�UID
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetLastAction( WFLBY_SYSTEM* p_wk, WFLBY_LASTACTION_TYPE action, s32 userid )
{
	GF_ASSERT( action < WFLBY_LASTACTION_MAX );
	
	WFLBY_SYSTEM_LASTACTQ_Push( &p_wk->lastactin_q, action, userid );
	WFLBY_SYSTEM_DWC_SetMyProfile( p_wk );	// �X�V
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�e����ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	item	�A�C�e��
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetMyItem( WFLBY_SYSTEM* p_wk, WFLBY_ITEMTYPE item )
{
	GF_ASSERT( item < WFLBY_ITEM_NUM );

	// CRC�`�F�b�N
	if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
		p_wk->error = WFLBY_SYSTEM_ERR_CRC;
		return ;
	}


	p_wk->myprofile.profile.item = item;

	WFLBY_SYSTEM_MyProfile_SetCrc( &p_wk->myprofile, p_wk->p_save );
	WFLBY_SYSTEM_DWC_SetMyProfile( p_wk );	// �X�V
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�Z�^�C�v��ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	type1	�Z�^�C�v�P
 *	@param	type2	�Z�^�C�v�Q
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetSelectWazaType( WFLBY_SYSTEM* p_wk, WFLBY_POKEWAZA_TYPE type1, WFLBY_POKEWAZA_TYPE type2 )
{
	GF_ASSERT( type1 < WFLBY_POKEWAZA_TYPE_NUM );
	GF_ASSERT( type2 < WFLBY_POKEWAZA_TYPE_NUM );

	// CRC�`�F�b�N
	if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
		p_wk->error = WFLBY_SYSTEM_ERR_CRC;
		return ;
	}

	// �s���f�[�^�`�F�b�N
	if( type1 >= WFLBY_POKEWAZA_TYPE_NUM ){
		return ;
	}
	if( type2 >= WFLBY_POKEWAZA_TYPE_NUM ){
		return ;
	}

	// ��ɂ߂Ċi�[����
	// type1��NONE�Ȃ�type2����ɓ����
	if( type1 == WFLBY_POKEWAZA_TYPE_NONE ){
		p_wk->myprofile.profile.waza_type[0] = type2;
		p_wk->myprofile.profile.waza_type[1] = WFLBY_POKEWAZA_TYPE_NONE;
	}else{
		p_wk->myprofile.profile.waza_type[0] = type1;
		p_wk->myprofile.profile.waza_type[1] = type2;
	}

	WFLBY_SYSTEM_MyProfile_SetCrc( &p_wk->myprofile, p_wk->p_save );
	WFLBY_SYSTEM_DWC_SetMyProfile( p_wk );	// �X�V
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�I������ݒ肷��
 *
 *	@param	p_wk			���[�N
 *	@param	anketo_no		�A���P�[�g�ʂ��i���o�[
 *	@param	select			�I���i���o�[
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetAnketoData( WFLBY_SYSTEM* p_wk, u32 anketo_no, u32 select )
{
	GF_ASSERT( select < ANKETO_ANSWER_NUM );

	// CRC�`�F�b�N
	if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
		p_wk->error = WFLBY_SYSTEM_ERR_CRC;
		return ;
	}

	if( select >= ANKETO_ANSWER_NUM ){
		return ;
	}

	p_wk->myprofile.profile.anketo.anketo_no	= anketo_no;
	p_wk->myprofile.profile.anketo.select		= select;

	// �A���P�[�g�ɓ������t���O�ݒ�
	p_wk->event.anketo_input	 = TRUE;

	// �A���P�[�g�V��
	WFLBY_SYSTEM_PLAYED_SetPlay( p_wk, WFLBY_PLAYED_ANKETO );
	

	WFLBY_SYSTEM_MyProfile_SetCrc( &p_wk->myprofile, p_wk->p_save );
	WFLBY_SYSTEM_DWC_SetMyProfile( p_wk );	// �X�V
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[������l�x�r�s�`�s�t�r�𐶐�
 *
 *	@param	cp_profile		�v���t�B�[��
 *	@param	p_buff			�i�[��o�b�t�@
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_GetProfileMyStatus( const WFLBY_USER_PROFILE* cp_profile, MYSTATUS* p_buff, u32 heapID )
{
	BOOL result;
	BOOL set_dummy;
	
	// ���O��EOM�����邩�`�F�b�N���āA
	// �������炻�̂܂�MYSTATUS�ɐݒ肷��
	// �Ȃ�������_�~�[�������ݒ肷��
	result = WFLBY_SYSTEM_CheckStrEOMCode( cp_profile->name, PERSON_NAME_SIZE + EOM_SIZE );	

	set_dummy = FALSE;
	
	if( result == TRUE ){
		// ���O
		MyStatus_SetMyName( p_buff, cp_profile->name );

		// �s���ȕ����R�[�h�`�F�b�N
		{
			STRBUF* p_name;
			STRBUF* p_name_tmp;

			p_name_tmp	= STRBUF_Create( (PERSON_NAME_SIZE + EOM_SIZE)*4, heapID );
			p_name		= STRBUF_Create( (PERSON_NAME_SIZE + EOM_SIZE)*4, heapID );
			MyStatus_CopyNameString( p_buff, p_name );
			result = FontProc_ErrorStrCheck( FONT_SYSTEM, p_name, p_name_tmp );
			if( result == FALSE ){
				set_dummy = TRUE;
			}
			
			STRBUF_Delete( p_name_tmp );
			STRBUF_Delete( p_name );
		}
		
	}else{
		// �_�~�[�̖��O�ݒ�
		set_dummy = TRUE;
	}


	// �_�~�[�̖��O��ݒ�
	if( set_dummy ){
		MSGDATA_MANAGER*  p_msgman;
		STRBUF* p_str;
		p_msgman	= MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_wifi_hiroba_dat, heapID );
		p_str		= MSGMAN_AllocString( p_msgman, msg_hiroba_profile_15 );
		MyStatus_SetMyNameFromString( p_buff, p_str );
		STRBUF_Delete( p_str );
		MSGMAN_Delete( p_msgman );
	}

	
	// ID
	MyStatus_SetID( p_buff, WFLBY_SYSTEM_GetProfileTrID( cp_profile ) );
	// ����
	MyStatus_SetMySex( p_buff, WFLBY_SYSTEM_GetProfileSex( cp_profile ) );
	// ������
	MyStatus_SetTrainerView( p_buff, WFLBY_SYSTEM_GetProfileTrType( cp_profile ) );
	// ���[�W����
	MyStatus_SetRegionCode( p_buff, WFLBY_SYSTEM_GetProfileRagionCode( cp_profile ) );
	// �Q�[���N���A
	MyStatus_SetDpClearFlag( p_buff );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[����status�����o���擾
 *
 *	@param	cp_profile		�v���t�B�[��
 *
 *	@retval		�X�e�[�^�X
 *	@retval		WFLBY_STATUS_UNKNOWN	�Ȃ��̏��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetProfileStatus( const WFLBY_USER_PROFILE* cp_profile )
{
	if( cp_profile->status >= WFLBY_STATUS_UNKNOWN ){
		return WFLBY_STATUS_UNKNOWN;
	}
	return cp_profile->status;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[����lastaction�����o���擾
 *
 *	@param	cp_profile		�v���t�B�[�� 
 *	@param	idx				�C���f�b�N�X
 *
 *	@retval		�Ō�ɂ����X�e�[�^�X
 *	@retval		WFLBY_LASTACTION_MAX�Ȃ��̂���
 */
//-----------------------------------------------------------------------------
WFLBY_LASTACTION_TYPE WFLBY_SYSTEM_GetProfileLastAction( const WFLBY_USER_PROFILE* cp_profile, u32 idx )
{
	int i;
	int count;
	
	GF_ASSERT( idx < WFLBY_LASTACT_BUFFNUM );

	// �s���ȃf�[�^�ȊO�̃f�[�^��idx�Ԗڂ̂��̂�
	// �Ԃ�
	count = 0;
	for( i=0; i<WFLBY_LASTACT_BUFFNUM; i++ ){

		if( cp_profile->last_action[i] < WFLBY_LASTACTION_MAX ){
			count ++;
		}

		if( (count-1) == idx ){
			return cp_profile->last_action[i];
		}
	}

	return WFLBY_LASTACTION_MAX;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[����lastaction_user�����o���擾
 *
 *	@param	cp_profile		�v���t�B�[�� 
 *	@param	idx				�C���f�b�N�X
 *
 *	@retval	DWC_LOBBY_INVALID_USER_ID	�ЂƂ���Ȃ�
 *	@retval	���̑�							���[�UIDX
 */
//-----------------------------------------------------------------------------
s32 WFLBY_SYSTEM_GetProfileLastActionUserID( const WFLBY_USER_PROFILE* cp_profile, u32 idx )
{
	int i;
	int count;
	
	GF_ASSERT( idx < WFLBY_LASTACT_BUFFNUM );

	// �s���ȃf�[�^�ȊO�̃f�[�^��idx�Ԗڂ̂��̂�
	// �Ԃ�
	count = 0;
	for( i=0; i<WFLBY_LASTACT_BUFFNUM; i++ ){

		if( cp_profile->last_action[i] < WFLBY_LASTACTION_MAX ){
			count ++;
		}

		if( (count-1) == idx ){
			return cp_profile->last_action_userid[i];
		}
	}

	return DWC_LOBBY_INVALID_USER_ID;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U���r�[���h�c���擾
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
s32 WFLBY_SYSTEM_GetProfileUserID( const WFLBY_USER_PROFILE* cp_profile )
{
	return cp_profile->userid;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�h�c���擾
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetProfileTrID( const WFLBY_USER_PROFILE* cp_profile )
{
	return cp_profile->trainerid;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�^�C�v�ɑΉ������Ō�ɂ�������NO��Ԃ�
 *
 *	@param	cp_profile	�v���t�B�[��
 *
 *	@return	WFLBY_LASTACTION_TYPE	�Ή�����Ō�ɂ�������NO
 */
//-----------------------------------------------------------------------------
WFLBY_LASTACTION_TYPE WFLBY_SYSTEM_GetProfileTrTypetoLastActionNo( const WFLBY_USER_PROFILE* cp_profile )
{
	u32 tr_type;
	int i;
	
	tr_type = WFLBY_SYSTEM_GetProfileTrType( cp_profile );
	for( i=0; i<WFLBY_SYSTEMUNICHAR_NUM; i++ ){
		if( sc_WFLBY_SYSTEM_TRTYPE_SEX[ i ].tr_type == tr_type ){
			return i;
		}
	}

	// �Ȃ��̂�MAX��Ԃ�
	return WFLBY_LASTACTION_MAX;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������Ԃ��擾
 *	
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
WFLBY_TIME WFLBY_SYSTEM_GetProfileInTime( const WFLBY_USER_PROFILE* cp_profile )
{
	return cp_profile->intime;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���Ԃ��擾
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
WFLBY_TIME WFLBY_SYSTEM_GetProfileWldTime( const WFLBY_USER_PROFILE* cp_profile )
{
	return cp_profile->wldtime;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ʂ��擾
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetProfileSex( const WFLBY_USER_PROFILE* cp_profile )
{
	u32 sex;
	int i;
	u32 tr_type;
	
	// �m��Ȃ����ʂ���Ȃ����`�F�b�N
	if( cp_profile->sex >= PM_NEUTRAL ){

		// �m��Ȃ����ʂȂ̂Ńg���[�i�^�C�v�ɍ��킹��
		// �\���ł���g���[�i�^�C�v���`�F�b�N
		tr_type = WFLBY_UNIONCHARNO_TRTYPE_Get( cp_profile->tr_type );
		if( tr_type != OBJCODEMAX ){
			for( i=0; i<NELEMS(sc_WFLBY_SYSTEM_TRTYPE_SEX); i++ ){
				if( sc_WFLBY_SYSTEM_TRTYPE_SEX[i].tr_type == cp_profile->tr_type ){
					sex = sc_WFLBY_SYSTEM_TRTYPE_SEX[i].tr_type;
				}
			}
		}else{
		
			// �g���[�i�^�C�v���s���Ȃ̂ŏ�
			sex = PM_FEMALE;
		}
			
	}else{
		sex = cp_profile->sex;
	}
	return sex;
}

//----------------------------------------------------------------------------
/**
 *	@brief	����R�[�h���擾����
 *
 *	@param	cp_profile	�v���t�B�[��
 *
 *	@retval	����R�[�h
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetProfileRagionCode( const WFLBY_USER_PROFILE* cp_profile )
{
	u32 region_code;

	if( WFLBY_SYSTEM_CheckProfileRagionCode( cp_profile ) == TRUE ){
		region_code = cp_profile->region_code;
	}else{
		region_code = WFLBY_REGIONCODE_ERR;	// (�p��)
	}
	return region_code;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L���ȃ��[�W�����R�[�h���`�F�b�N����
 *
 *	@param	cp_profile	�v���t�B�[��
 *
 *	@retval	TRUE	�L��
 *	@retval	FALSE	����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_CheckProfileRagionCode( const WFLBY_USER_PROFILE* cp_profile )
{
	// �m��Ȃ�����R�[�h����Ȃ����`�F�b�N
	switch( cp_profile->region_code ){
	case LANG_JAPAN:
	case LANG_ENGLISH:
	case LANG_FRANCE:
	case LANG_ITALY:
	case LANG_GERMANY:
	case LANG_SPAIN:
		return TRUE;

	// �؍�����\���ł��Ȃ��̂ŁA�p��ɂ���
	default:
		break;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���R�[�h�����̂܂܎擾����
 *
 *	@param	cp_profile	�v���t�B�[��
 *
 *	@return	���̂܂܂̍��R�[�h
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetProfileRagionCodeOrg( const WFLBY_USER_PROFILE* cp_profile )
{
	return cp_profile->region_code;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�^�C�v�擾
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetProfileTrType( const WFLBY_USER_PROFILE* cp_profile )
{
	u32 trtype;
	
//	OS_TPrintf( "union_char_no %d\n", cp_profile->tr_type );

	trtype = WFLBY_UNIONCHARNO_TRTYPE_Get( cp_profile->tr_type );
	
	// �\���ł���g���[�i�^�C�v���`�F�b�N
	if( trtype != OBJCODEMAX ){
		return trtype;
	}
	
	// �\���ł��Ȃ��̂ŁA�o������̂ɒu��������
	if( WFLBY_SYSTEM_GetProfileSex( cp_profile ) == PM_MALE ){
		return BOY1;
	}
	return GIRL1;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���R�[�h�擾
 *
 *	@param	cp_profile	�v���t�B�[��
 *
 *	@retval	WFLBY_WLDDATA_NATION_NONE	�s���ȍ�
 *	@retval	���̑�						�\���ł��鍑
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetProfileNation( const WFLBY_USER_PROFILE* cp_profile )
{
	u32 nation;
	
	// ���̍������邩�`�F�b�N
	if( cp_profile->nation >= WIFI_COUNTRY_MAX ){
		nation = WFLBY_WLDDATA_NATION_NONE;
	}else{

		// �n�悪�����Ă��邩�`�F�b�N
		if( WFLBY_AREA_Check( cp_profile->nation, cp_profile->area ) == FALSE ){
			return WFLBY_WLDDATA_NATION_NONE;
		}
		
		nation = cp_profile->nation;
	}
	return nation;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n��R�[�h���擾����
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetProfileArea( const WFLBY_USER_PROFILE* cp_profile )
{
	u32 area;
	
	// ���̍������邩�`�F�b�N
	if( WFLBY_AREA_Check( cp_profile->nation, cp_profile->area ) == FALSE ){
		area = 0;
	}else{
		area = cp_profile->area;
	}
	return area;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�S���}�ӂ������Ă��邩�`�F�b�N
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetProfileZukanZenkoku( const WFLBY_USER_PROFILE* cp_profile )
{
	return cp_profile->zukan_zenkoku;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���N���A���`�F�b�N
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetProfileGameClear( const WFLBY_USER_PROFILE* cp_profile )
{
	return cp_profile->game_clear;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����Ă���A�C�e�����擾
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
WFLBY_ITEMTYPE WFLBY_SYSTEM_GetProfileItem( const WFLBY_USER_PROFILE* cp_profile )
{
	u32 item;

	// �܂��^�b�`�g�C��������Ă��Ȃ��l�ɃA�N�Z�X���܂����B
	if( cp_profile->item == WFLBY_ITEM_INIT ){
//		OS_TPrintf( "item error %d\n", cp_profile->item );
		return WFLBY_ITEM_BELL00;
	}
	
	// �s���A�C�e���łȂ����`�F�b�N
	if( cp_profile->item >= WFLBY_ITEM_NUM ){
		item = WFLBY_ITEM_BELL00;
	}else{
		item = cp_profile->item;
	}
	return item;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�b�`�g�C��������Ă��邩�`�F�b�N����
 *
 *	@param	cp_profile	�v���t�B�[��
 *
 *	@retval	TRUE	�^�b�`�g�C��������Ă���
 *	@retval	FALSE	�^�b�`�g�C��������Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_CheckGetItem( const WFLBY_USER_PROFILE* cp_profile )
{
	if( cp_profile->item == WFLBY_ITEM_INIT ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���̊J�n���Ԃ��`�F�b�N����
 *
 *	@param	cp_profile	�v���t�B�[��
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�s��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_CheckProfileGameStartTime( const WFLBY_USER_PROFILE* cp_profile )
{
	if( cp_profile->start_sec > GMTIME_SEC_MAX ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�������Z�^�C�v���擾����
 *
 *	@param	cp_profile	�v���t�B�[��
 *	@param	idx			�I����e�C���f�b�N�X
 *
 *	@return	�Z�^�C�v
 */
//-----------------------------------------------------------------------------
WFLBY_POKEWAZA_TYPE WFLBY_SYSTEM_GetProfileWazaType( const WFLBY_USER_PROFILE* cp_profile, u32 idx )
{
	GF_ASSERT( idx < WFLBY_SELECT_TYPENUM );

	if( cp_profile->waza_type[ idx ] >= WFLBY_POKEWAZA_TYPE_NUM ){
		return WFLBY_POKEWAZA_TYPE_NONE;
	}

	return cp_profile->waza_type[ idx ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�f�[�^�擾
 *
 *	@param	cp_profile		���[�N
 *	@param	p_data			�A���P�[�g�f�[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_GetProfileAnketoData( const WFLBY_USER_PROFILE* cp_profile, WFLBY_ANKETO* p_data )
{
	// �A���P�[�g�f�[�^�擾
	*p_data = cp_profile->anketo;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���̊J�n���Ԃ��擾
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
s64 WFLBY_SYSTEM_GetProfileGameStartTime( const WFLBY_USER_PROFILE* cp_profile )
{
	if( cp_profile->start_sec > GMTIME_SEC_MAX ){
		return GMTIME_SEC_MAX;
	}
	return cp_profile->start_sec;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����pMYSTATUS���擾����i�s�����O��u�������f�[�^��MYSTATUS�j
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	MYSTATUS
 */
//-----------------------------------------------------------------------------
MYSTATUS* WFLBY_SYSTEM_GetMgMyStatus( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->mg_data.p_mystatus;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���[�U��VIP�t���O���擾����
 *
 *	@param	cp_wk	���[�N
 *	@param	idx		���[�U�C���f�b�N�X
 *
 *	@retval	TRUE	VIP���[�U
 *	@retval	FALSE	���ʃ��[�U
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetUserVipFlag( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	return WFLBY_SYSTEM_VIPFLAG_Get( &cp_wk->vipflag, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP�t���O�o�b�t�@���̎擾
 *
 *	@param	cp_wk	���[�N
 *	@param	p_buff	�f�[�^�i�[��
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_GetVipFlagWk( const WFLBY_SYSTEM* cp_wk, WFLBY_VIPFLAG* p_buff )
{
	WFLBY_VIPFLAG* p_vip;

	MI_CpuCopy8( &cp_wk->vipflag, p_buff, sizeof(WFLBY_VIPFLAG) );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�̍����t�L���t���O���擾
 *		
 *	@param	cp_wk	���[�N
 *	@param	idx		���[�U�C���f�b�N�X
 *
 *	@retval	TRUE	�������Ƃ΂�����
 *	@retval	FALSE	�������Ƃ΂��Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetUserAikotobaFlag( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	return WFLBY_SYSTEM_AIKOTOBABUFF_GetFlag( &cp_wk->aikotoba, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�̍����t���擾
 *
 *	@param	cp_wk	���[�N
 *	@param	idx		���[�U�C���f�b�N�X
 *
 *	@retval	NULL				�����t�������ĂȂ�
 *	@retval	�����t�f�[�^
 */
//-----------------------------------------------------------------------------
const WFLBY_AIKOTOBA_DATA* WFLBY_SYSTEM_GetUserAikotobaStr( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	return WFLBY_SYSTEM_AIKOTOBABUFF_GetData( &cp_wk->aikotoba, idx );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�����������Ă���T�u�`�����l���ɂ���v���C���[�����擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�����������Ă���T�u�`�����l���ɂ���v���C���[��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetSubChanPlNum( const WFLBY_SYSTEM* cp_wk )
{
	DWC_LOBBY_CHANNEL_USERID usertbl;

	DWC_LOBBY_SUBCHAN_GetUserIDTbl( &usertbl );

	return usertbl.num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����������Ă���T�u�`�����l���ɂ��郆�[�U�h�c���擾
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			�f�[�^�h�c
 *
 *	@retval	DWC_LOBBY_USERIDTBL_IDX_NONE�ȊO	���[�U�h�c
 *	@retval	DWC_LOBBY_USERIDTBL_IDX_NONE		���̃T�u�`�����l���h�c�w�Ƀ��[�U�͂��Ȃ�
 *
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetSubChanEnum( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	u32 userid;

	userid = DWC_LOBBY_SUBCHAN_GetUserIdxID( idx );
	if( userid == DWC_LOBBY_INVALID_USER_ID ){
		return DWC_LOBBY_USERIDTBL_IDX_NONE;
	}
	return DWC_LOBBY_GetUserIDIdx( userid );
}



//----------------------------------------------------------------------------
/**
 *	@brief	���A�g�s�b�N�̑��M
 *
 *	@param	p_wk		���[�N
 *	@param	user_a		���[�U�`	�b����������
 *	@param	user_b		���[�U�a	�b��������ꂽ��
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_TOPIC_SendConnect( WFLBY_SYSTEM* p_wk, u32 user_a, u32 user_b )
{
	WFLBY_TOPIC topic = {0};

	topic.topic_type	= NEWS_TOPICTYPE_CONNECT;
	topic.lobby_id[0]	= DWC_LOBBY_GetUserIdxID( user_a );
	topic.lobby_id[1]	= DWC_LOBBY_GetUserIdxID( user_b );
	topic.num			= 2;
	DWC_LOBBY_SendChannelMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TOPIC_DATA, &topic, sizeof(WFLBY_TOPIC) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�e���������b�Z�[�W�̑��M
 *
 *	@param	p_wk		���[�N
 *	@param	user_a		���[�U�`	�����������
 *	@param	user_b		���[�U�a	��������
 *	@param	item		�A�C�e��		
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_TOPIC_SendItem( WFLBY_SYSTEM* p_wk, u32 user_a, u32 user_b, WFLBY_ITEMTYPE item )
{
	WFLBY_TOPIC topic = {0};

	topic.topic_type	= NEWS_TOPICTYPE_ITEM;
	topic.lobby_id[0]	= DWC_LOBBY_GetUserIdxID( user_b );
	topic.lobby_id[1]	= DWC_LOBBY_GetUserIdxID( user_a );
	topic.num			= 2;
	topic.item			= item;
	DWC_LOBBY_SendChannelMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TOPIC_DATA, &topic, sizeof(WFLBY_TOPIC) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���n�̃g�s�b�N�𑗐M
 *
 *	@param	p_wk			���[�N
 *	@param	minigame		�~�j�Q�[���^�C�v
 *	@param	num				�Q���l���i�����܂߁j
 *	@param	user_0			���[�U�O	�e������Ȃ�e�͂���
 *	@param	user_1			�P
 *	@param	user_2			�Q
 *	@param	user_3			�R
 *	@param	play			�v���C����
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_TOPIC_SendMiniGame( WFLBY_SYSTEM* p_wk, WFLBY_GAMETYPE minigame, u32 num, u32 user_0, u32 user_1, u32 user_2, u32 user_3, BOOL play )
{
	WFLBY_TOPIC topic = {0};
	u32 topic_type;

	switch( minigame ){
	case WFLBY_GAME_BALLSLOW:		// �ʓ���
	case WFLBY_GAME_BALANCEBALL:	// �ʏ��
	case WFLBY_GAME_BALLOON:		// �ӂ�������
		topic_type = NEWS_TOPICTYPE_MINIGAME;
		break;
		
	case WFLBY_GAME_FOOTWHITE:		// ���Ճ{�[�h�@��
	case WFLBY_GAME_FOOTBLACK:		// ���Ճ{�[�h�@��
		topic_type = NEWS_TOPICTYPE_FOOTBOARD;
		break;
		
	case WFLBY_GAME_WLDTIMER:		// ���E���v
		topic_type = NEWS_TOPICTYPE_WORLDTIMER;
		break;

	case WFLBY_GAME_NEWS:			// ���r�[�j���[�X
		topic_type = NEWS_TOPICTYPE_LOBBYNEWS;
		break;

	default:
		return ;	// ����Ȃ�
	}

	topic.topic_type	= topic_type;
	topic.lobby_id[0]	= DWC_LOBBY_GetUserIdxID( user_0 );
	topic.lobby_id[1]	= DWC_LOBBY_GetUserIdxID( user_1 );
	topic.lobby_id[2]	= DWC_LOBBY_GetUserIdxID( user_2 );
	topic.lobby_id[3]	= DWC_LOBBY_GetUserIdxID( user_3 );
	topic.num			= num;
	topic.minigame		= minigame;
	topic.play			= play;
	DWC_LOBBY_SendChannelMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TOPIC_DATA, &topic, sizeof(WFLBY_TOPIC) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���D����	�g�s�b�N���M
 *
 *	@param	p_wk		���[�N
 *	@param	minigame	�~�j�Q�[���^�C�v	
 *	@param	num			�D���Ґ�
 *	@param	user_0		�D����A
 *	@param	user_1		�D����B
 *	@param	user_2		�D����C
 *	@param	user_3		�D����D 
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_TOPIC_SendMiniGameTopResult( WFLBY_SYSTEM* p_wk, WFLBY_GAMETYPE minigame, u32 num, u32 user_0, u32 user_1, u32 user_2, u32 user_3 )
{
	WFLBY_TOPIC topic = {0};
	u32 topic_type;

	switch( minigame ){
	case WFLBY_GAME_BALLSLOW:		// �ʓ���
	case WFLBY_GAME_BALANCEBALL:	// �ʏ��
	case WFLBY_GAME_BALLOON:		// �ӂ�������
		topic_type = NEWS_TOPICTYPE_MGRESULT;
		break;
		
	default:
		return ;	// ����Ȃ�
	}

	topic.topic_type	= topic_type;
	topic.lobby_id[0]	= DWC_LOBBY_GetUserIdxID( user_0 );
	topic.lobby_id[1]	= DWC_LOBBY_GetUserIdxID( user_1 );
	topic.lobby_id[2]	= DWC_LOBBY_GetUserIdxID( user_2 );
	topic.lobby_id[3]	= DWC_LOBBY_GetUserIdxID( user_3 );
	topic.num			= num;
	topic.minigame		= minigame;
	topic.play			= FALSE;
	DWC_LOBBY_SendChannelMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TOPIC_DATA, &topic, sizeof(WFLBY_TOPIC) );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�����̃v���t�B�[�����X�V����
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetMyProfile( WFLBY_SYSTEM* p_wk, const WFLBY_USER_PROFILE* cp_data )
{
	// CRC�`�F�b�N
	if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
		p_wk->error = WFLBY_SYSTEM_ERR_CRC;
		return ;
	}

	p_wk->myprofile.profile = *cp_data;
	WFLBY_SYSTEM_MyProfile_SetCrc( &p_wk->myprofile, p_wk->p_save );
	WFLBY_SYSTEM_DWC_SetMyProfile( p_wk );	// �X�V
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�[�Y�������擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�V�[�Y��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetSeason( const WFLBY_SYSTEM* cp_wk )
{
	u32 season;
	season = DWC_LOBBY_GetRoomData( DWC_LOBBY_ROOMDATA_SEASON );
	if( season >= WFLBY_SEASON_NUM ){
		season = WFLBY_SEASON_NONE;
	}
	return season;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����^�C�v���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�����^�C�v
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetRoomType( const WFLBY_SYSTEM* cp_wk )
{
	u32 roomtype;
	roomtype = DWC_LOBBY_GetRoomData( DWC_LOBBY_ROOMDATA_ROOMTYPE );
	if( roomtype >= WFLBY_ROOM_NUM ){	// �s��l�`�F�b�N
		roomtype = WFLBY_ROOM_FIRE;
	}
	return roomtype;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃��b�N���Ԃ��擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�����̃��b�N����	�b�P��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetLockTime( const WFLBY_SYSTEM* cp_wk )
{
	return DWC_LOBBY_GetRoomData( DWC_LOBBY_ROOMDATA_LOCKTIME );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃N���[�Y���Ԃ��擾����	�i���b�N���������Ă���N���[�Y�܂ł̎��ԁj
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�����̃N���[�Y����	�b�P��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetCloseTime( const WFLBY_SYSTEM* cp_wk )
{
	u32 closetime;

	// �N���[�Y���Ԃ̎擾�@���������A�N���[�Y�̓C�x���g�͔�����WFLBY_END_OVERTIME�b��
	closetime = DWC_LOBBY_GetRoomData( DWC_LOBBY_ROOMDATA_CLOSETIME );
	closetime += (WFLBY_END_OVERTIME/30);	// �`��t���[���P�ʂ�b�P�ʂɂ��Ă��瑫��

	return closetime;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g���o�����`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			�v���C���[INDEX
 *
 *	@retval	FALSE		�o���Ȃ��Ă���
 *	@retval	TRUE		�o��	
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetGadGetFlag( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );
	return cp_wk->gadget.gadget[idx];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g���b�Z�[�W�̑��M
 *
 *	@param	p_wk		���[�N
 *	@param	gadget		�K�W�F�b�g
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SendGadGetData( WFLBY_SYSTEM* p_wk, WFLBY_ITEMTYPE gadget )
{
	u32 data;

	data = gadget;
	
	DWC_LOBBY_SendChannelMsg( WFLBY_SYSTEM_MAIN_COMMCMD_GADGET_DATA, 
			&data, sizeof(u32) );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�����쒆���`�F�b�N����
 *
 *	@param	cp_wk	���[�N
 *	@param	idx		�t���[�g�C���f�b�N�X
 *
 *	@retval	WFLBY_FLOAT_STATE_BEFORE,	// ����O
 *	@retval	WFLBY_FLOAT_STATE_MOVE,		// ���쒆
 *	@retval	WFLBY_FLOAT_STATE_AFTER,	// �����
 */
//-----------------------------------------------------------------------------
WFLBY_FLOAT_MOVE_STATE WFLBY_SYSTEM_GetFloatMove( const WFLBY_SYSTEM* cp_wk, u32 floatidx )
{
	return WFLBY_SYSTEM_FLOAT_GetMove( &cp_wk->floatdata, floatidx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g����J�E���^�擾
 *
 *	@param	cp_wk		���[�N
 *	@param	floatidx	�t���[�g�C���f�b�N�X
 *
 *	@return	�J�E���^
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetFloatCount( const WFLBY_SYSTEM* cp_wk, u32 floatidx )
{
	return WFLBY_SYSTEM_FLOAT_GetCount( &cp_wk->floatdata, floatidx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g����̍ő�l���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�J�E���g�̍ő�l
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetFloatCountMax( const WFLBY_SYSTEM* cp_wk )
{
	return WFLBY_FLOAT_COUNT_ONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�\������擾
 *
 *	@param	cp_wk	���[�N
 *	@param	idx		�t���[�g�C���f�b�N�X
 *
 *	@return	���̃t���[�g�C���f�b�N�X�ɍڂ��Ă���l�̃v���C���[�h�c�w
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetFloatReserve( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	return WFLBY_SYSTEM_FLOAT_GetReserve( &cp_wk->floatdata, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�h�c�w�̗\�񂵂Ă�����ȃC���f�b�N�X���A��
 *
 *	@param	cp_wk		���[�N
 *	@param	plidx		�v���C���[�C���f�b�N�X
 *
 *	@retval	���ȃC���f�b�N�X
 *	@retval	DWC_LOBBY_USERIDTBL_IDX_NONE	�\�񂵂ĂȂ�
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetFloatPlIdxReserve( const WFLBY_SYSTEM* cp_wk, u32 plidx )
{
	return WFLBY_SYSTEM_FLOAT_GetIdx( &cp_wk->floatdata, plidx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�\�����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	plidx		�v���C���[�C���f�b�N�X
 *	@param	station		�w�萔
 *
 *	@retval	DWC_LOBBY_USERIDTBL_IDX_NONE	�\�񎸔s
 *	@retval	���̑�							�\�񐬌�
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_SetFloatReserve( WFLBY_SYSTEM* p_wk, u32 plidx, WFLBY_FLOAT_STATION_TYPE station )
{
	return WFLBY_SYSTEM_FLOAT_SetReserve( &p_wk->floatdata, plidx, station );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\����̔j��
 *
 *	@param	p_wk		���[�N
 *	@param	plidx		�v���C���[�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_ResetFloatReserve( WFLBY_SYSTEM* p_wk, u32 plidx )
{
	WFLBY_SYSTEM_FLOAT_ResetReserve( &p_wk->floatdata, plidx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�A�j�����N�G�X�g��ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�v���C���[�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetFloatAnm( WFLBY_SYSTEM* p_wk, u32 idx )
{
	WFLBY_SYSTEM_FLOAT_SetAnm( &p_wk->floatdata, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�A�j�����N�G�X�g���擾
 *
 *	@param	cp_wk	���[�N
 *	@param	idx		�t���[�g�A�j���C���f�b�N�X
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetFloatAnm( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	return WFLBY_SYSTEM_FLOAT_GetAnm( &cp_wk->floatdata, idx );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�A�j�����N�G�X�g�̃N���A
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�t���[�g�A�j���C���f�b�N�X
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_CleanFloatAnm( WFLBY_SYSTEM* p_wk, u32 idx )
{
	WFLBY_SYSTEM_FLOAT_CleanAnm( &p_wk->floatdata, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g���ȃC���f�b�N�X����t���[�g�C���f�b�N�X�ƃI�t�Z�b�g���擾
 *
 *	@param	idx				���ȃC���f�b�N�X
 *	@param	p_floatidx		�t���[�g�C���f�b�N�X
 *	@param	p_floatofs		�t���[�g�I�t�Z�b�g
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_GetFloatIdxOfs( u32 idx, u32* p_floatidx, u32* p_floatofs )
{
	WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( idx, p_floatidx, p_floatofs );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�\�񃍃b�N
 *
 *	@param	p_wk	���[�N
 *	@param	idx		���ȃC���f�b�N�X
 *	@param	station	�w�萔
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetFloatReserveLock( WFLBY_SYSTEM* p_wk, u32 idx, WFLBY_FLOAT_STATION_TYPE station )
{
	WFLBY_SYSTEM_FLOAT_SetReserveLock( &p_wk->floatdata, idx, station );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g���b�N	�܂��\��ł���w�̎擾
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			���ȃC���f�b�N�X
 *
 *	@return	�w�萔
 */
//-----------------------------------------------------------------------------
WFLBY_FLOAT_STATION_TYPE WFLBY_SYSTEM_GetFloatReserveLock( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	return WFLBY_SYSTEM_FLOAT_GetReserveLock( &cp_wk->floatdata, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�ɏ�������Ƃ𑗐M
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SendFloatOn( WFLBY_SYSTEM* p_wk )
{
	u32 dummy;
	DWC_LOBBY_SendChannelMsg( WFLBY_SYSTEM_MAIN_COMMCMD_FLOAT_ON, &dummy, sizeof(u32) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�A�j�����g�p�������Ƃ𑗐M
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SendFloatAnm( WFLBY_SYSTEM* p_wk )
{
	u32 dummy;
	DWC_LOBBY_SendChannelMsg( WFLBY_SYSTEM_MAIN_COMMCMD_FLOAT_ANM, &dummy, sizeof(u32) );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���S�ɃN���[���ȏ�Ԃɂ���
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_EndTalk( WFLBY_SYSTEM* p_wk )
{
	WFLBY_SYSTEM_TALK_EndClear( &p_wk->talk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b���N�G�X�g
 *
 *	@param	p_wk	���[�N
 *	@param	idx		���[�UIDX 
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	���s	�i���������Z������ԁj
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_SendTalkReq( WFLBY_SYSTEM* p_wk, u32 idx )
{
	s32 userid;

	// ��b���H
//	GF_ASSERT( p_wk->talk.talk_type == WFLBY_TALK_TYPE_NONE );
//	�����ɗ��邱�Ƃ�����悤�Ȃ̂Ł��̂悤�ɏC��
	// ���̏u�ԁA�b��������ꂽ��Ԃ̂Ƃ�
	if( p_wk->talk.talk_type != WFLBY_TALK_TYPE_NONE ){
		return FALSE;
	}
	
	// idx�̐l���Z������Ԃ���Ȃ����`�F�b�N
	{
		const WFLBY_USER_PROFILE* cp_profile = WFLBY_SYSTEM_GetUserProfile( p_wk, idx );
		u32 status = WFLBY_SYSTEM_GetProfileStatus( cp_profile );
//		OS_TPrintf( "talk req user status %d  useridx=%d\n", status, idx );
		if( status != WFLBY_STATUS_LOGIN ){
			return FALSE;
		}
	}

	// ��b��Ԑݒ�
	p_wk->talk.talk_idx		= idx;
	p_wk->talk.talk_seq		= WFLBY_TALK_SEQ_B_ANS;
	p_wk->talk.talk_type	= WFLBY_TALK_TYPE_A;

	// �f�[�^�ݒ�
	WFLBY_SYSTEM_TALK_SetSendDataNowSeq( &p_wk->talk, 0 );
	
	// ��b�J�n
	userid = DWC_LOBBY_GetUserIdxID( idx );
	DWC_LOBBY_SendPlayerMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TALK_REQ, userid, &p_wk->talk.send_data, sizeof(WFLBY_SYSTEM_TALK_DATA) );

	// ��M�҂��ݒ�
	WFLBY_SYSTEM_TALK_StartRecvWait( &p_wk->talk );

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�f�[�^�𑗂�
 *
 *	@param	p_wk		���[�N
 *	@param	data		�f�[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SendTalkData( WFLBY_SYSTEM* p_wk, u16 data )
{
	BOOL exend = FALSE;
	s32 userid;
	
	// �V�[�P���X�̐������`�F�b�N
	if( (p_wk->talk.talk_type == WFLBY_TALK_TYPE_A) ){
		if( p_wk->talk.talk_seq != WFLBY_TALK_SEQ_A_SEL ){
			// �^�C�vA�Ȃ̂�A�̑I�𒆂���Ȃ�
			// ��b�����܂��s���ĂȂ��̂Őؒf
			exend = TRUE;
		}
	}else{
		if( (p_wk->talk.talk_type == WFLBY_TALK_TYPE_B) ){
			if( p_wk->talk.talk_seq != WFLBY_TALK_SEQ_B_SEL ){
				// �^�C�vB�Ȃ̂�B�̑I�𒆂���Ȃ�
				// ��b�����܂��s���ĂȂ��̂Őؒf
				exend = TRUE;
			}
		}
	}
	// �ً}�I��
	if( exend ){
		userid = DWC_LOBBY_GetUserIdxID( p_wk->talk.talk_idx );
		WFLBY_SYSTEM_TALK_SendTalkExEnd( p_wk, userid );
		return ;
	}


	
	// ��b��Ԑݒ�
	switch( p_wk->talk.talk_type ){
	case WFLBY_TALK_TYPE_A:
		p_wk->talk.talk_seq		= WFLBY_TALK_SEQ_B_SEL;
		break;

	case WFLBY_TALK_TYPE_B:
		p_wk->talk.talk_seq		= WFLBY_TALK_SEQ_A_SEL;
		break;

	default:
//		GF_ASSERT( 0 );	// �Θb�^�C�v����������

		userid = DWC_LOBBY_GetUserIdxID( p_wk->talk.talk_idx );
		WFLBY_SYSTEM_TALK_SendTalkExEnd( p_wk, userid );
		return;
	}

	// �f�[�^�ݒ�
	WFLBY_SYSTEM_TALK_SetSendDataNowSeq( &p_wk->talk, data );
	
	// ��b�J�n
	userid = DWC_LOBBY_GetUserIdxID( p_wk->talk.talk_idx );
	DWC_LOBBY_SendPlayerMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TALK_DATA, userid, &p_wk->talk.send_data, sizeof(WFLBY_SYSTEM_TALK_DATA) );

	// ��M�҂��ݒ�
	WFLBY_SYSTEM_TALK_StartRecvWait( &p_wk->talk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b���Őe�Ȃ�I���𑗂�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SendTalkEnd( WFLBY_SYSTEM* p_wk )
{
	s32 userid;
	
	if( p_wk->talk.talk_seq == WFLBY_TALK_SEQ_NONE ){
		return ;
	}
	if( p_wk->talk.talk_type != WFLBY_TALK_TYPE_A ){
		return ;
	}

	// ���M
	p_wk->talk.send_data.seq = WFLBY_TALK_SEQ_NONE;

	userid = DWC_LOBBY_GetUserIdxID( p_wk->talk.talk_idx );
	DWC_LOBBY_SendPlayerMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TALK_END, userid, &p_wk->talk.send_data, sizeof(WFLBY_SYSTEM_TALK_DATA) );

	// ��b�I��
	WFLBY_SYSTEM_TALK_EndClear( &p_wk->talk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����I���������Ƃ�������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SendTalkExEnd( WFLBY_SYSTEM* p_wk )
{
	s32 userid;

	if( p_wk->talk.talk_seq == WFLBY_TALK_SEQ_NONE ){
		return ;
	}

	userid = DWC_LOBBY_GetUserIdxID( p_wk->talk.talk_idx );

	p_wk->talk.talk_seq		= WFLBY_TALK_SEQ_EXEND;
	p_wk->talk.send_data.seq = WFLBY_TALK_SEQ_EXEND;
	DWC_LOBBY_SendPlayerMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TALK_DATA, userid, &p_wk->talk.send_data, sizeof(WFLBY_SYSTEM_TALK_DATA) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�����`�F�b�N����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	��b��
 *	@retval	FALSE	��b���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_CheckTalk( const WFLBY_SYSTEM* cp_wk )
{
	if( cp_wk->talk.talk_type != WFLBY_TALK_TYPE_NONE ){
		return  TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�N�������b���N�G�X�g���������`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE		����
 *	@retval	FALSE		���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_CheckTalkReq( const WFLBY_SYSTEM* cp_wk )
{
	if( cp_wk->talk.talk_type == WFLBY_TALK_TYPE_B ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�������Ƃ����邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetTalkMsk( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	return WFLBY_SYSTEM_TALK_CheckMsk( &cp_wk->talk, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�������M����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	��b����
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_GetTalkIdx( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->talk.talk_idx;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̉�b�����߂Ă��擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	���߂�
 *	@retval	FALSE	�͂��߂Ă���Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetTalkFirst( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->talk.talk_first;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�V�[�P���X���擾����
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	WFLBY_TALK_SEQ��b�V�[�P���X
 */
//-----------------------------------------------------------------------------
WFLBY_TALK_SEQ WFLBY_SYSTEM_GetTalkSeq( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->talk.talk_seq;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��M�f�[�^���擾����	��ɑI���������ڂȂ�
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	��M�f�[�^
 */
//-----------------------------------------------------------------------------
u16 WFLBY_SYSTEM_GetTalkRecv( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->talk.recv_data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�����������炻�̐l�Ƙb���邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			���[�U�C���f�b�N�X
 *	
 *	@retval	TRUE		�b������OK
 *	@retval	FALSE		�b����������
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_CheckTalkCount( const WFLBY_SYSTEM* cp_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );
	
	if( cp_wk->talk.talk_count[ idx ] >= WFLBY_TALK_MAX ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�G���[��ԃ`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�G���[���
 *	@retval	FALSE	�G���[��Ԃ���Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_CheckTalkError( const WFLBY_SYSTEM* cp_wk )
{
	if( cp_wk->talk.talk_seq == WFLBY_TALK_SEQ_EXEND ){
		return TRUE;
	}

	// �^�C���E�G�C�g�`�F�b�N
	if( WFLBY_SYSTEM_TALK_CheckRecvWait( &cp_wk->talk ) == FALSE ){
		return TRUE;
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b��M�҂����Ԃ��擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�܂�����
 */
//-----------------------------------------------------------------------------
s16	WFLBY_SYSTEM_GetRecvWaitTime( const WFLBY_SYSTEM* cp_wk )
{
	return WFLBY_SYSTEM_TALK_GetRacvWaitTime( &cp_wk->talk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b���������������΂ɂ����̂Ŋ|����ꂽ���͉�b�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetTalkBStart( WFLBY_SYSTEM* p_wk )
{
	p_wk->talk.talk_b_start = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b��������ꂽ�ꍇ�A�b���������肪�����̂��΂ɗ������擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�b��������ꂽ����b�J�n
 *	@retval	FALSE	��b�܂�����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_CheckTalkBStart( const WFLBY_SYSTEM* cp_wk )
{
	return cp_wk->talk.talk_b_start;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b���������񐔂��J�E���g
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_AddTalkCount( WFLBY_SYSTEM* p_wk )
{
	// ��b�����`�F�b�N
	if( WFLBY_SYSTEM_CheckTalk( p_wk ) == TRUE ){

		// ��b���������J�E���g
		WFLBY_SYSTEM_TALK_AddCount( &p_wk->talk, p_wk->talk.talk_idx );
	}
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�E���g����w�肵�ăJ�E���g
 *
 *	@param	p_wk	���[�N
 *	@param	idx		���[�U�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_AddTalkCountEx( WFLBY_SYSTEM* p_wk, u32 idx )
{
	// ��b���������J�E���g
	WFLBY_SYSTEM_TALK_AddCount( &p_wk->talk, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�������Ƃ���}�X�N��ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetTalkMsk( WFLBY_SYSTEM* p_wk, u32 idx )
{
	WFLBY_SYSTEM_TALK_SetMsk( &p_wk->talk, idx );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�`���m�[�g�f�[�^��ݒ�	��b����
 *
 *	@param	p_wk		���[�N
 *	@param	plidx		��b�����l
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_FNOTE_SetTalk( WFLBY_SYSTEM* p_wk, u32 plidx )
{
	WFLBY_USER_PROFILE* p_profile;
	MYSTATUS* p_mystatus;
	void* p_buf;
	FNOTE_DATA* p_fnote;

	p_fnote		= SaveData_GetFNote( p_wk->p_save );
	p_profile	= (WFLBY_USER_PROFILE*)WFLBY_SYSTEM_GetUserProfile( p_wk, plidx );
	
	if( p_profile != NULL ){

		p_mystatus = MyStatus_AllocWork( HEAPID_WFLBY_ROOM );
		WFLBY_SYSTEM_GetProfileMyStatus( p_profile, p_mystatus, HEAPID_WFLBY_ROOM );

		p_buf = FNOTE_Sio_ID_Name_DataMake( MyStatus_GetMyName( p_mystatus ), p_profile->sex, 
				HEAPID_WFLBY_ROOM, FNOTE_ID_PL_LOBBY_CHAT );
		FNOTE_DataSave( p_fnote, p_buf, FNOTE_TYPE_SIO );

		sys_FreeMemoryEz( p_mystatus );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`���m�[�g�f�[�^��ݒ�	�K�W�F�b�g���擾
 *
 *	@param	p_wk		���[�N
 *	@param	plidx		�����������
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_FNOTE_SetGetGadget( WFLBY_SYSTEM* p_wk, u32 plidx )
{
	WFLBY_USER_PROFILE* p_profile;
	MYSTATUS* p_mystatus;
	void* p_buf;
	FNOTE_DATA* p_fnote;

	p_fnote		= SaveData_GetFNote( p_wk->p_save );
	p_profile	= (WFLBY_USER_PROFILE*)WFLBY_SYSTEM_GetUserProfile( p_wk, plidx );
	
	if( p_profile != NULL ){

		p_mystatus = MyStatus_AllocWork( HEAPID_WFLBY_ROOM );
		WFLBY_SYSTEM_GetProfileMyStatus( p_profile, p_mystatus, HEAPID_WFLBY_ROOM );

		p_buf = FNOTE_Sio_ID_Name_DataMake( MyStatus_GetMyName( p_mystatus ), p_profile->sex,
				HEAPID_WFLBY_ROOM, FNOTE_ID_PL_LOBBY_TOY_GET );
		FNOTE_DataSave( p_fnote, p_buf, FNOTE_TYPE_SIO );

		sys_FreeMemoryEz( p_mystatus );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`���m�[�g�f�[�^��ݒ�	�~�j�Q�[����V��
 *
 *	@param	p_wk			���[�N
 *	@param	minigame		�~�j�Q�[���f�[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_FNOTE_SetPlayMinigame( WFLBY_SYSTEM* p_wk, WFLBY_GAMETYPE minigame )
{
	void* p_buf;
	FNOTE_DATA* p_fnote;

	p_fnote		= SaveData_GetFNote( p_wk->p_save );

	switch( minigame ){
	// �~�j�Q�[��
	case WFLBY_GAME_BALLSLOW:	// �ʓ���
	case WFLBY_GAME_BALANCEBALL:	// �ʏ��
	case WFLBY_GAME_BALLOON:		// �ӂ�������
		p_buf = FNOTE_SioLobbyMinigameDataMake( minigame,
				HEAPID_WFLBY_ROOM, FNOTE_ID_PL_LOBBY_MINIGAME );
		break;
		
	// ���Ճ{�[�h
	case WFLBY_GAME_FOOTWHITE:	// ���Ճ{�[�h�@����
	case WFLBY_GAME_FOOTBLACK:	// ���Ճ{�[�h�@��
		p_buf =  FNOTE_SioIDOnlyDataMake( HEAPID_WFLBY_ROOM, FNOTE_ID_PL_LOBBY_FOOT_BOARD );
		break;
		
	// ���E���v
	case WFLBY_GAME_WLDTIMER:	
		p_buf =  FNOTE_SioIDOnlyDataMake( HEAPID_WFLBY_ROOM, FNOTE_ID_PL_LOBBY_WORLDTIMER );
		break;
		
	// ���r�[�j���[�X
	case WFLBY_GAME_NEWS:		
		p_buf =  FNOTE_SioIDOnlyDataMake( HEAPID_WFLBY_ROOM, FNOTE_ID_PL_LOBBY_NEWS );
		break;

	default:
		p_buf = NULL;
		break;
	}

	if( p_buf ){
		FNOTE_DataSave( p_fnote, p_buf, FNOTE_TYPE_SIO );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`���m�[�g�f�[�^��ݒ�	�t���[�g�ɏ����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_FNOTE_SetParade( WFLBY_SYSTEM* p_wk )
{
	void* p_buf;
	FNOTE_DATA* p_fnote;

	p_fnote		= SaveData_GetFNote( p_wk->p_save );
	p_buf = FNOTE_SioIDOnlyDataMake( HEAPID_WFLBY_ROOM, FNOTE_ID_PL_LOBBY_PARADE );
	FNOTE_DataSave( p_fnote, p_buf, FNOTE_TYPE_SIO );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���R�[�h�f�[�^		�b����������	�J�E���g
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_RECORD_AddTalk( WFLBY_SYSTEM* p_wk )
{
	RECORD* p_rec;

	p_rec = SaveData_GetRecord( p_wk->p_save );
	RECORD_Inc( p_rec, RECID_LOBBY_CHAT );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���R�[�h�f�[�^		�~�j�Q�[���V�񂾉�	�J�E���g
 *
 *	@param	p_wk	���[�N 
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_RECORD_AddMinigame( WFLBY_SYSTEM* p_wk )
{
	RECORD* p_rec;

	p_rec = SaveData_GetRecord( p_wk->p_save );
	RECORD_Inc( p_rec, RECID_LOBBY_GAME_COUNT );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�񂾏��o�b�t�@	�V�񂾂��Ƃ�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	type		�V�т�������
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_PLAYED_SetPlay( WFLBY_SYSTEM* p_wk, WFLBY_PLAYED_TYPE type )
{
	GF_ASSERT( type < WFLBY_PLAYED_NUM );
	p_wk->played.play[type] = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�񂾏��o�b�t�@	�����ߓx����ԍ����܂��V�񂾂��Ƃ̂Ȃ��V�т�Ԃ�
 *	
 *	@param	cp_wk	���[�N
 *
 *	@retval	WFLBY_PLAYED_NUM	�����V�񂾂��Ƃ̂Ȃ��V�т͂Ȃ�
 *	@retval	���̂�				�����ߗV��
 */
//-----------------------------------------------------------------------------
WFLBY_PLAYED_TYPE WFLBY_SYSTEM_PLAYED_GetOsusumePlayType( const WFLBY_SYSTEM* cp_wk )
{
	int i;
	BOOL play_ok;

	for( i=0; i<WFLBY_PLAYED_NUM; i++ ){
		if( cp_wk->played.play[i] == FALSE ){
			// ���̗V�т͍��ł��V�ׂ邩�H
			switch( i ){
			// �~�j�Q�[��
			case WFLBY_PLAYED_BALLTHROW:	// �^�}����
			case WFLBY_PLAYED_BALANCEBALL:	// Balance�{�[��
			case WFLBY_PLAYED_BALLOON:		// �ӂ�������
				if( WFLBY_SYSTEM_Event_GetMiniGameStop( cp_wk ) == TRUE ){
					play_ok = FALSE;
				}else{
					play_ok = TRUE;
				}
				break;
				
			case WFLBY_PLAYED_WORLDTIMER:	// ���E���v	
			case WFLBY_PLAYED_TOPIC:		// �L��j���[�X
			case WFLBY_PLAYED_FOOTBOARD:	// ����BOARD
				if( WFLBY_SYSTEM_Event_GetEndCM( cp_wk ) == TRUE ){
					play_ok = FALSE;
				}else{
					play_ok = TRUE;
				}
				break;

			case WFLBY_PLAYED_ANKETO:		// �A���P�[�g
				if( WFLBY_SYSTEM_Event_GetNeon( cp_wk ) == TRUE ){
					play_ok = FALSE;
				}else{
					play_ok = TRUE;
				}
				break;
				
			default:
			case WFLBY_PLAYED_TALK:			// �b������
				play_ok = TRUE;
				break;
			}

			// �V�ׂ�Ȃ�Ԃ�
			if( play_ok == TRUE ){
				return i;
			}
		}
	}
	return WFLBY_PLAYED_NUM;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����_���K�W�F�b�g�̎擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�����_���őI�����ꂽ�K�W�F�b�g
 */
//-----------------------------------------------------------------------------
WFLBY_ITEMTYPE WFLBY_SYSTEM_RandItemGet( const WFLBY_SYSTEM* cp_wk )
{
	WFLBY_ITEMTYPE	item;
	item = WFLBY_SYSTEM_GADGETRATE_GetRandItem( &cp_wk->gadgetrate );

	// ������VIP�Ȃ��R�i�K�ɕύX����
	if( WFLBY_SYSTEM_GetUserVipFlag( cp_wk, WFLBY_SYSTEM_GetMyIdx( cp_wk ) ) == TRUE ){
		item += (WFLBY_ITEM_DANKAI-1);
	}
	
	return item;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΍\���f�[�^	���̉ԉΕ\���^�C�v���擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	WFLBY_FIRE_NONE,	// �ԉ΂Ȃ�
 *	@retval	WFLBY_FIRE_NORMAL,	// �ʏ�
 *	@retval	WFLBY_FIRE_KOUGO,	// ���ݔ���
 *	@retval	WFLBY_FIRE_NAMI,	// �g�󔭎�
 *	@retval	WFLBY_FIRE_CLIMAX,	// �Ō�
 *	@retval	WFLBY_FIRE_END,		// �I����
 */
//-----------------------------------------------------------------------------
WFLBY_FIRE_TYPE WFLBY_SYSTEM_FIRE_GetType( const WFLBY_SYSTEM* cp_wk )
{
	return WFLBY_SYSTEM_FIREDATA_GetType( &cp_wk->fire_data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΍\���f�[�^	����J�E���^
 *	
 *	@param	cp_wk	���[�N
 *
 *	@return	�ԉ΃^�C�v�ɂȂ��Ă��炽�������ԁi�`��t���[���P�ʁj
 */
//-----------------------------------------------------------------------------
u32 WFLBY_SYSTEM_FIRE_GetTypeMoveCount( const WFLBY_SYSTEM* cp_wk )
{
	return WFLBY_SYSTEM_FIREDATA_GetCount( &cp_wk->fire_data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g���ʂ�ۑ�����
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�A���P�[�g����
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_SetAnketoResult( WFLBY_SYSTEM* p_wk, const ANKETO_QUESTION_RESULT* cp_data )
{
	p_wk->anketo_result.set_flag	= TRUE;
	p_wk->anketo_result.data		= *cp_data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g���ʂ��擾����
 *
 *	@param	cp_wk		���[�N
 *	@param	p_data		�A���P�[�g���ʕۑ���
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_SYSTEM_GetAnketoResult( const WFLBY_SYSTEM* cp_wk, ANKETO_QUESTION_RESULT* p_data )
{
	if( cp_wk->anketo_result.set_flag == TRUE ){
		*p_data = cp_wk->anketo_result.data;
		return TRUE;
	}
	return FALSE;
}




//----------------------------------------------------------------------------
/**
 *	@brief	WiFi�N���u�p�ɑ�����_�~�[���̃o�b�t�@�ݒ肷��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_WiFiClubBuff_Init( WFLBY_SYSTEM* p_wk )
{
	MYSTATUS* p_mystatus;
	
	GF_ASSERT( p_wk->p_wificlubbuff == NULL );

	p_mystatus = SaveData_GetMyStatus( p_wk->p_save );
	
	// WiFi�N���u�ŁA
	p_wk->p_wificlubbuff = WifiP2PMatch_CleanStatus_Set( p_mystatus, p_wk->heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi�N���u�p�ɑ�����_�~�[����j������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_SYSTEM_WiFiClubBuff_Exit( WFLBY_SYSTEM* p_wk )
{
//	GF_ASSERT( p_wk->p_wificlubbuff != NULL );
	// WiFi�N���u�ŁA
	if( p_wk->p_wificlubbuff != NULL ){
		// �������m�ۂ���Ă�����A�j������
		WifiP2PMatch_CleanStatus_Delete( p_wk->p_wificlubbuff );
		p_wk->p_wificlubbuff = NULL;
	}
}
	

#ifdef PM_DEBUG
void WFLBY_SYSTEM_DEBUG_SetItem( WFLBY_SYSTEM* p_wk, u32 item )
{
	p_wk->myprofile.profile.item = item;

	WFLBY_SYSTEM_MyProfile_SetCrc( &p_wk->myprofile, p_wk->p_save );
}

void WFLBY_SYSTEM_DEBUG_SetLangCode( WFLBY_USER_PROFILE* p_profile, u32 code )
{
	p_profile->region_code = code;
}
#endif









//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�����񒆂�EOM_�R�[�h�����邩�`�F�b�N����
 *
 *	@param	cp_str		������R�[�h
 *	@param	len			������
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_CheckStrEOMCode( const STRCODE* cp_str, u32 len )
{
	int i;

	for( i=0; i<len; i++ ){
		if( cp_str[i] == EOM_ ){
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�����R�[���o�b�N
 *
 *	@param	userid		���[�UID
 *	@param	cp_profile	�v���t�B�[��
 *	@param	p_work		���[�N
 *	@param	mydata		�����̃f�[�^��
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_CallbackUserIn( s32 userid, const void* cp_profile, void* p_work, BOOL mydata )
{
	WFLBY_SYSTEM* p_wk;
	u32 idx;
	const WFLBY_USER_PROFILE* cp_userdata;
	s32 old_user;
	u32 flag;
	
	p_wk = p_work;
	

	// �����̃f�[�^���`�F�b�N
	if( mydata ){

		// cp_userdata�̂ق�����~�j�Q�[���p��MYSTATUS���쐬����
		WFLBY_SYSTEM_MG_SetMyStatus( &p_wk->mg_data, &p_wk->myprofile, p_wk->heapID );

		// CRC�`�F�b�N
		if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
			p_wk->error = WFLBY_SYSTEM_ERR_CRC;
		}

		idx = DWC_LOBBY_GetUserIDIdx( userid );
		NEWS_DSET_SetRoomIn( p_wk->glbdata.p_lobbynews, idx, p_wk->myprofile.profile.sex, NEWS_ROOMSP_MYDATA|NEWS_ROOMSP_OLD );

		// �����̃��[�UID��ݒ肷��
		p_wk->myprofile.profile.userid = userid;


//		OS_TPrintf( "nation area %d %d\n", p_wk->myprofile.profile.nation, p_wk->myprofile.profile.area );
		

		// �������Ԃ�ݒ肷��
		{
			s64 time;
			DWC_LOBBY_GetLoginTime( userid, &time );
			WFLBY_TIME_Set( &p_wk->myprofile.profile.intime, &time );
			OS_TPrintf( "in time hour=%d min=%d  sec=%d\n", p_wk->myprofile.profile.intime.hour, p_wk->myprofile.profile.intime.minute, p_wk->myprofile.profile.intime.second );
		}

		// GMT���Ԃ��玩���̒n��̎��Ԃ��擾����
		{
			u32 nation, area;
			BOOL reagion_code;

			nation = WFLBY_SYSTEM_GetProfileNation( &p_wk->myprofile.profile );
			area = WFLBY_SYSTEM_GetProfileArea( &p_wk->myprofile.profile );
			reagion_code = WFLBY_SYSTEM_CheckProfileRagionCode( &p_wk->myprofile.profile );	// ���[�W�������������`�F�b�N

			// �������邩�`�F�b�N	����Ɍ���R�[�h���L�����m�F
			if( (nation != WFLBY_WLDDATA_NATION_NONE) && (reagion_code == TRUE) ){
				p_wk->myprofile.profile.wldtime = WFLBY_WORLDTIMER_PLACE_GetPlaceTime( 
						nation, 
						area, p_wk->myprofile.profile.intime, p_wk->heapID );
				OS_TPrintf( "wld time hour=%d min=%d  sec=%d\n", p_wk->myprofile.profile.wldtime.hour, p_wk->myprofile.profile.wldtime.minute, p_wk->myprofile.profile.wldtime.second );
			}else{
				p_wk->myprofile.profile.wldtime.hour   = 12;
				p_wk->myprofile.profile.wldtime.minute = 0;
				p_wk->myprofile.profile.wldtime.second = 0;
				OS_TPrintf( "wld time hour=%d min=%d  sec=%d\n", p_wk->myprofile.profile.wldtime.hour, p_wk->myprofile.profile.wldtime.minute, p_wk->myprofile.profile.wldtime.second );
			}
		}



		// �v���t�B�[�����X�V
		WFLBY_SYSTEM_MyProfile_SetCrc( &p_wk->myprofile, p_wk->p_save );
		WFLBY_SYSTEM_DWC_SetMyProfile( p_wk );
		
	}else{
		// �v���t�B�[���ݒ�
		cp_userdata = (const WFLBY_USER_PROFILE*)cp_profile;

//		OS_TPrintf( "user profile lang_code %d\n", cp_userdata->region_code );

		// ��ԌÂ����炢��l���Ɏ擾
		old_user = DWC_LOBBY_GetOldUser();

		// �C���f�b�N�X���擾���āA���̐l�𒆂ɓ����
		// �����̕����Â����炢��̂ŁA���̐l����ԌÂ��킯���Ȃ�
		idx = DWC_LOBBY_GetUserIDIdx( userid );
		NEWS_DSET_SetRoomIn( p_wk->glbdata.p_lobbynews, idx, 
				WFLBY_SYSTEM_GetProfileSex( cp_userdata ), NEWS_ROOMSP_NONE );

		// ��������ԌÂ��������ԌÂ��f�[�^�ɐݒ�
		if( old_user == userid ){
			NEWS_DSET_SetRoomOld( p_wk->glbdata.p_lobbynews, idx );
		}


		// 080702�ǉ�
		// �����Ă����l�̗�������������A�m��Ȃ��l�ɂȂ�悤�ɏ�������
		if( WFLBY_SYSTEM_LASTACTQ_ResetUserID( &p_wk->lastactin_q, userid, DWC_LOBBY_INVALID_USER_ID ) ){
			WFLBY_SYSTEM_DWC_SetMyProfile( p_wk );	// �X�V
		}
	}


	// �N�������Ă���
	idx  = DWC_LOBBY_GetUserIDIdx( userid );
	p_wk->flag.user_in |= 1 << idx;

#ifdef WFLBY_DEBUG_ALL_VIP
	if( D_Tomoya_WiFiLobby_ALLVip == TRUE ){
		WFLBY_SYSTEM_VIPFLAG_Set( &p_wk->vipflag, idx, TRUE );
		{
//			WFLBY_SYSTEM_AIKOTOBABUFF_SetData( &p_wk->aikotoba, idx, TRUE, 1849019343  );
			WFLBY_SYSTEM_AIKOTOBABUFF_SetData( &p_wk->aikotoba, idx, TRUE, gf_mtRand() );
		}

		//VIP�������Ă����g�s�b�N����
		{
			NEWS_DATA_SET_VIPIN news_data;
			news_data.vip_plidx = idx;
			NEWS_DSET_SetVipIn( p_wk->glbdata.p_lobbynews, &news_data, &p_wk->vipflag );
		}
	}else{

		// VIP�`�F�b�N
		if( DWC_LOBBY_VIP_CheckVip( userid ) == TRUE ){
			u32 aikotoba_key;

			// VIP�t���O�ݒ�
			WFLBY_SYSTEM_VIPFLAG_Set( &p_wk->vipflag, idx, TRUE );

			//VIP�������Ă����g�s�b�N����
			{
				NEWS_DATA_SET_VIPIN news_data;
				news_data.vip_plidx = idx;
				NEWS_DSET_SetVipIn( p_wk->glbdata.p_lobbynews, &news_data, &p_wk->vipflag );
			}
			
			// �������Ƃ΁@�`�F�b�N
			aikotoba_key = DWC_LOBBY_VIP_GetAikotobaKey( userid );
			if( aikotoba_key != DWC_LOBBY_VIP_KEYNONE ){
				WFLBY_SYSTEM_AIKOTOBABUFF_SetData( &p_wk->aikotoba, idx, TRUE, aikotoba_key );
			}else{
				WFLBY_SYSTEM_AIKOTOBABUFF_SetData( &p_wk->aikotoba, idx, FALSE, aikotoba_key );
			}
		}else{
			// ����������
			// VIP�t���OOFF
			WFLBY_SYSTEM_VIPFLAG_Set( &p_wk->vipflag, idx, FALSE );
			WFLBY_SYSTEM_AIKOTOBABUFF_SetData( &p_wk->aikotoba, idx, FALSE, 0 );
		}
		
	}
#else

	// VIP�`�F�b�N
	if( DWC_LOBBY_VIP_CheckVip( userid ) == TRUE ){
		u32 aikotoba_key;

		// VIP�t���O�ݒ�
		WFLBY_SYSTEM_VIPFLAG_Set( &p_wk->vipflag, idx, TRUE );

		//VIP�������Ă����g�s�b�N����
		{
			NEWS_DATA_SET_VIPIN news_data;
			news_data.vip_plidx = idx;
			NEWS_DSET_SetVipIn( p_wk->glbdata.p_lobbynews, &news_data, &p_wk->vipflag );
		}
		
		// �������Ƃ΁@�`�F�b�N
		aikotoba_key = DWC_LOBBY_VIP_GetAikotobaKey( userid );
		if( aikotoba_key != DWC_LOBBY_VIP_KEYNONE ){
			WFLBY_SYSTEM_AIKOTOBABUFF_SetData( &p_wk->aikotoba, idx, TRUE, aikotoba_key );
		}else{
			WFLBY_SYSTEM_AIKOTOBABUFF_SetData( &p_wk->aikotoba, idx, FALSE, aikotoba_key );
		}
	}else{
		// ����������
		// VIP�t���OOFF
		WFLBY_SYSTEM_VIPFLAG_Set( &p_wk->vipflag, idx, FALSE );
		WFLBY_SYSTEM_AIKOTOBABUFF_SetData( &p_wk->aikotoba, idx, FALSE, 0 );
	}
#endif


	OS_TPrintf( "in useridx = %d my=%d\n", idx, mydata );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�ގ��R�[���o�b�N
 *
 *	@param	userid		���[�UID
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_CallbackUserOut( s32 userid, void* p_work )
{
	WFLBY_SYSTEM* p_wk;
	u32 idx, old_idx;
	BOOL old_set;
	s32	old_user;
	
	p_wk = p_work;


	// �����̃��O�A�E�g�̏ꍇ�́A�G���[������
	if( userid == DWC_LOBBY_GetMyUserID() ){
		return ;
	}

	// �C���f�b�N�X�擾
	idx = DWC_LOBBY_GetUserIDIdx( userid );

	// ��b���̐l�Ȃ狭���I��
	if( WFLBY_SYSTEM_CheckTalk( p_wk ) == TRUE ){
		// ��b���̐l���H
		if( WFLBY_SYSTEM_GetTalkIdx( p_wk ) == idx ){
			// ��b�̋����I��
			WFLBY_SYSTEM_TALK_ExEndClear( &p_wk->talk );
		}
	}

	// ���̐l��ސȂ�����
	old_set = NEWS_DSET_SetRoomOut( p_wk->glbdata.p_lobbynews, idx );
	if( old_set ){
		old_user = DWC_LOBBY_GetOldUser_UserDesc( userid );
		old_idx = DWC_LOBBY_GetUserIDIdx( old_user );
//		OS_TPrintf( "old user chg =%d\n", old_idx );
		NEWS_DSET_SetRoomOld( p_wk->glbdata.p_lobbynews, old_idx );
	}

	// VIP�t���OOFF
	WFLBY_SYSTEM_VIPFLAG_Set( &p_wk->vipflag, idx, FALSE );

	// ��b�J�E���gOFF
	WFLBY_SYSTEM_TALK_ClearCount( &p_wk->talk, idx );

	// ��b�������Ƃ��邩�t���O
	WFLBY_SYSTEM_TALK_ClearMsk( &p_wk->talk, idx );

	// 080702�ǉ�
	// �o�čs�����l�̗����̃��[�UID����������
	if( WFLBY_SYSTEM_LASTACTQ_ResetUserID( &p_wk->lastactin_q, userid, DWC_LOBBY_INVALID_USER_ID ) ){
		WFLBY_SYSTEM_DWC_SetMyProfile( p_wk );	// �X�V
	}
	

	// �����t
	WFLBY_SYSTEM_AIKOTOBABUFF_SetData( &p_wk->aikotoba, idx, FALSE, 0 );

	// �N���łĂ�����
	idx  = DWC_LOBBY_GetUserIDIdx( userid );
	p_wk->flag.user_out |= 1 << idx;

}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[���A�b�v�f�[�^
 *	
 *	@param	userid		���[�UID
 *	@param	cp_profile	�v���t�B�[��
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_CallbackUserProfileUpdate( s32 userid, const void* cp_profile, void* p_work )
{
	WFLBY_SYSTEM* p_wk;
	u32 idx;
	const WFLBY_USER_PROFILE* cp_data;
	u32 status;

	p_wk = p_work;

	idx = DWC_LOBBY_GetUserIDIdx( userid );
	
	p_wk->flag.profile_up |= 1 << idx;

	// �v���t�B�[���擾
	cp_data = cp_profile;

	status = WFLBY_SYSTEM_GetProfileStatus( cp_data );

	//  ���̐l�̃X�e�[�^�X����b�ȊO�ɂȂ�^�C�~���O�ŁA
	//  ��b�J�E���^�����Z�b�g
	// ��Ԃ��ς��`�ŁALOGIN�X�e�[�g�ɖ߂�Ƃ��A��b�񐔂����Z�b�g
	switch( status ){
	case WFLBY_STATUS_BALLSLOW:		// �ʂ���
	case WFLBY_STATUS_BALANCEBALL:	// �ʏ��
	case WFLBY_STATUS_BALLOON:		// ���D���
	case WFLBY_STATUS_FOOTBOAD00:	// ���Ճ{�[�h
	case WFLBY_STATUS_FOOTBOAD01:	// ���Ճ{�[�h
	case WFLBY_STATUS_WORLDTIMER:	// ���E���v
	case WFLBY_STATUS_TOPIC:			// �j���[�X
		WFLBY_SYSTEM_TALK_ClearCount( &p_wk->talk, idx );
		break;

	default:
		break;
	}

	// �K�W�F�b�g���[�g�ݒ�
	if( WFLBY_SYSTEM_CheckGetItem( cp_data ) == TRUE ){
		WFLBY_SYSTEM_GADGETRATE_CalcRate( &p_wk->gadgetrate, WFLBY_SYSTEM_GetProfileItem( cp_data ), idx );
	}

	// �s�������̂킩��Ȃ��f�[�^������j������
	


//	OS_TPrintf( "user profile up user=%d status=%d\n", userid, status );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�J�n�R�[���o�b�N
 *
 *	@param	event		�C�x���g
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_CallbackEvent( PPW_LOBBY_TIME_EVENT event, void* p_work )
{
	WFLBY_SYSTEM* p_wk;
	BOOL topic_make = FALSE;
	WFLBY_EVENTTYPE wflby_event;

	p_wk = p_work;


	// 080707
	// tomoya takahashi
	// ���b�N�M���M���œ����Ă���ƁA
	// ���C����ʂ�O�ɁA�����Ŏ��Ԃ�ݒ肷��\��������̂ŁA
	// ���Ԃ��擾����
	{
		PPW_LobbyTimeInfo time;

		PPW_LobbyGetTimeInfo( &time );
		WFLBY_TIME_Set( &p_wk->glbdata.worldtime, &time.currentTime );
		p_wk->glbdata.worldtime_s64 = time.currentTime;
	}

	// �g�s�b�N�ݒ�
	switch( event ){
	case PPW_LOBBY_TIME_EVENT_LOCK:              ///< �����̃��b�N�B
		NEWS_DSET_SetLockTime( p_wk->glbdata.p_lobbynews, &p_wk->glbdata.worldtime );
		p_wk->glbdata.locktime_s64 = p_wk->glbdata.worldtime_s64;
		p_wk->event.lock	= TRUE;
		p_wk->flag.event_on |= 1 << WFLBY_EVENT_CHG_LOCK;
		break;

	// �l�I���Ɖԉ΂��n�߂܂�
	case PPW_LOBBY_TIME_EVENT_NEON_A1:           ///< �����l�I���~��1�B
	case PPW_LOBBY_TIME_EVENT_NEON_A2:           ///< �����l�I���~��2�B
	case PPW_LOBBY_TIME_EVENT_NEON_A3:           ///< �����l�I���~��3�B
	case PPW_LOBBY_TIME_EVENT_NEON_A4:           ///< �����l�I���~��4�B
	case PPW_LOBBY_TIME_EVENT_NEON_A5:           ///< �����l�I���~��5�B
		// �ŏ��̂P��ڂ̂Ƃ��̂݃g�s�b�N���쐬
		if( p_wk->glbdata.topic_event_start == FALSE ){
			topic_make = TRUE;
			wflby_event = WFLBY_EVENT_NEON;

			p_wk->glbdata.topic_event_start = TRUE;

			// �l�I���J�n�J�E���g
			p_wk->event.neon_count	= WFLBY_EVENT_STARTTIME;
		}
	case PPW_LOBBY_TIME_EVENT_NEON_A0:           ///< �����l�I���~��0�B

		p_wk->event.neon_room	= event;
		p_wk->flag.event_on |= 1 << WFLBY_EVENT_CHG_NEON_ROOM;
		break;
		
	case PPW_LOBBY_TIME_EVENT_NEON_B1:           ///< ���l�I���~��1�B
	case PPW_LOBBY_TIME_EVENT_NEON_B2:           ///< ���l�I���~��2�B
	case PPW_LOBBY_TIME_EVENT_NEON_B3:           ///< ���l�I���~��3�B
		// �ŏ��̂P��ڂ̂Ƃ��̂݃g�s�b�N���쐬
		if( p_wk->glbdata.topic_event_start == FALSE ){
			topic_make = TRUE;
			wflby_event = WFLBY_EVENT_NEON;

			p_wk->glbdata.topic_event_start = TRUE;

			// �l�I���J�n�J�E���g
			p_wk->event.neon_count	= WFLBY_EVENT_STARTTIME;
		}
	case PPW_LOBBY_TIME_EVENT_NEON_B0:           ///< ���l�I���~��0�B

		p_wk->event.neon_floor	= event;
		p_wk->flag.event_on		|= 1 << WFLBY_EVENT_CHG_NEON_FLOOR;
		break;

	case PPW_LOBBY_TIME_EVENT_NEON_C1:           ///< ���j�������g�~��1�B
	case PPW_LOBBY_TIME_EVENT_NEON_C2:           ///< ���j�������g�~��2�B
	case PPW_LOBBY_TIME_EVENT_NEON_C3:           ///< ���j�������g�~��3�B
		// �ŏ��̂P��ڂ̂Ƃ��̂݃g�s�b�N���쐬
		if( p_wk->glbdata.topic_event_start == FALSE ){
			topic_make = TRUE;
			wflby_event = WFLBY_EVENT_NEON;

			p_wk->glbdata.topic_event_start = TRUE;

			// �l�I���J�n�J�E���g
			p_wk->event.neon_count	= WFLBY_EVENT_STARTTIME;
		}
	case PPW_LOBBY_TIME_EVENT_NEON_C0:           ///< ���j�������g�~��0�B

		p_wk->event.neon_monu	= event;
		p_wk->flag.event_on		|= 1 << WFLBY_EVENT_CHG_NEON_MONU;
		break;
		
	case PPW_LOBBY_TIME_EVENT_FIRE_WORKS_START:  ///< �t�@�C�A�[���[�N�X�J�n�B
		topic_make = TRUE;
		wflby_event = WFLBY_EVENT_FIRE;


		// �ԉ΃C�x���g�̒�������ԉΏ���������
		{
			u32 fire_start,  fire_end;

			fire_start	= DWC_LOBBY_GetRoomData( DWC_LOBBY_ROOMDATA_FIRESTART );
			fire_end	= DWC_LOBBY_GetRoomData( DWC_LOBBY_ROOMDATA_FIREEND );

			WFLBY_SYSTEM_FIREDATA_Init( &p_wk->fire_data, fire_start, fire_end );
		}

		// �ԉΊJ�n�J�E���g�J�n
		p_wk->event.hanabi_count	= WFLBY_EVENT_STARTTIME;
		break;

	case PPW_LOBBY_TIME_EVENT_FIRE_WORKS_END:    ///< �t�@�C�A�[���[�N�X�I���B
		p_wk->event.hababi		= WFLBY_EVENT_HANABI_AFTER;
		p_wk->flag.event_on		|= 1 << WFLBY_EVENT_CHG_FIRE;
		break;

	case PPW_LOBBY_TIME_EVENT_PARADE:            ///< �p���[�h�B
		topic_make = TRUE;
		wflby_event = WFLBY_EVENT_PARADE;


		// �p���[�h�J�n�J�E���g�J�n
		p_wk->event.parade_count	= WFLBY_EVENT_STARTTIME;
		break;

	case PPW_LOBBY_TIME_EVENT_CLOSE:             ///< �I���B
		topic_make = TRUE;
		wflby_event = WFLBY_EVENT_CLOSE;

		p_wk->event.endcm		= TRUE;			// �I��CM�J�n
		// �I���J�E���g�J�n
		p_wk->event.end_count	= WFLBY_END_OVERTIME;
		break;
		
	case PPW_LOBBY_TIME_EVENT_MINIGAME_END:      ///< ���j�������g�~��4�B

		p_wk->event.mini		= TRUE;
		p_wk->flag.event_on		|= 1 << WFLBY_EVENT_CHG_MINIGAME;
		break;

		
	// �������Ȃ�����
	default:
		break;
	}

	if( topic_make ){
		WFLBY_SYSTEM_TOPIC_SetEvent( p_wk, wflby_event );
	}

}


//----------------------------------------------------------------------------
/**
 *	@brief	�������s���`�F�b�N��̃v���t�B�[����M�R�[���o�b�N
 *
 *	@param	cp_profile		�v���t�B�[��
 *	@param	profile_size	�v���t�B�[���T�C�Y(�s���`�F�b�N��server���`�F�b�N�����Ƃ��̃T�C�Y)
 *	@param	p_work			���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_CallbackCheckProfile( const void* cp_profile, u32 profile_size, void* p_work )
{
	WFLBY_SYSTEM* p_wk;
	const WFLBY_USER_PROFILE* cp_userdata;

	p_wk			= p_work;
	cp_userdata		= cp_profile;


	// CRC�`�F�b�N
	if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
		p_wk->error = WFLBY_SYSTEM_ERR_CRC;
		return ;
	}

	// �u����̃v���t�B�[����ݒ�
	WFLBY_SYSTEM_MyProfileCopy_CheckData( &p_wk->myprofile, cp_userdata, profile_size );

#ifdef WFLBY_DEBUG_PROFILE_DRAW
	WFLBY_SYSTEM_OSPrint_Profile( &p_wk->myprofile.profile );
#endif

	// �v���t�B�[�����X�V
	WFLBY_SYSTEM_MyProfile_SetCrc( &p_wk->myprofile, p_wk->p_save );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���̏�Ԃ𐢊E���v�ɐݒ�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_SetWldTimerData( WFLBY_USER_MYPROFILE* p_myprofile, WFLBY_WLDTIMER* p_wk )
{
	int i;
	DWC_LOBBY_CHANNEL_USERID usertbl;
	const WFLBY_USER_PROFILE* cp_profile;
	WFLBY_WLDTIMER* p_data;
	u8 nation;
	u8 area;

	// �������N���A
	memset( p_wk, 0, sizeof(WFLBY_WLDTIMER) );

	// ���[�U�e�[�u���擾
	DWC_LOBBY_GetUserIDTbl( &usertbl );

	// �����̃f�[�^���ɒǉ�
	cp_profile = &p_myprofile->profile;
	WFLBY_WLDTIMER_SetData( p_wk, 
			WFLBY_SYSTEM_GetProfileNation( cp_profile ), 
			WFLBY_SYSTEM_GetProfileArea( cp_profile ), FALSE );
	
	
	// �܂����r�[�ɂ���l�̃f�[�^�Ńo�b�t�@���쐬
	for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
		if( usertbl.cp_tbl[i] != DWC_LOBBY_INVALID_USER_ID ){
			cp_profile = (const WFLBY_USER_PROFILE*)DWC_LOBBY_GetUserProfile( usertbl.cp_tbl[ i ] );
			WFLBY_WLDTIMER_SetData( p_wk, 
					WFLBY_SYSTEM_GetProfileNation( cp_profile ), 
					WFLBY_SYSTEM_GetProfileArea( cp_profile ), FALSE );
		}
	}

	// �ގ��҃f�[�^
	for( i=0; i<DWC_WLDDATA_MAX; i++ ){
		nation	= DWC_LOBBY_WLDDATA_GetNation( i );
		area	= DWC_LOBBY_WLDDATA_GetArea( i );
		WFLBY_WLDTIMER_SetData( p_wk, nation, area, TRUE );
	}
}




//----------------------------------------------------------------------------
/**
 *	@brief	�I�����ԊǗ�	�iBTS�ʐM789�̑Ώ��j
 *
 *	@param	p_wk	���[�N	
 *
 *	�C�x���g�����������A�I���̎��ԂɂȂ����狭���I�ɏI���ɂ���
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_ContEndTime( WFLBY_SYSTEM* p_wk )
{
	s64	closetime;
	s64	now_time;

	// ���b�N���Ȃ�o�ߎ��Ԃ��擾
	if( (p_wk->event.lock == TRUE) && 
		(p_wk->event.endcm == FALSE) && 
		(DWC_LOBBY_GetState() == DWC_LOBBY_CHANNEL_STATE_CONNECT) ){

		// �I�����Ԏ擾
		closetime = WFLBY_SYSTEM_GetCloseTime( p_wk );
		closetime += p_wk->glbdata.locktime_s64;

		// ���̎���
		now_time = p_wk->glbdata.worldtime_s64;


//		OS_TPrintf( "closetime %d  worldtime %d\n", (u32)(closetime), (u32)(now_time) );

		// �N���[�YCM���Ԃ����Ă���͂�
		if( now_time > closetime ){

			// �����ŏI���ɂ��Ă��܂�
			p_wk->event.endcm		= TRUE;			// �I��CM�J�n
			// �I���J�E���g�J�n
			p_wk->event.end_count	= WFLBY_END_OVERTIME;
		}
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�Z�[�u�f�[�^����v���t�B�[����������������
 *
 *	@param	p_myprofile		�v���t�B�[���f�[�^
 *	@param	p_save			�Z�[�u�f�[�^
 *	@param	heapID			�q�[�v�h�c
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_InitProfile( WFLBY_USER_MYPROFILE* p_myprofile, SAVEDATA* p_save, u32 heapID )
{
	MYSTATUS* p_mystatus;
	POKEPARTY* p_pokeparty;
	WIFI_HISTORY* p_history;
	ZUKAN_WORK* p_zukan;
	GMTIME* p_time;

	// �Z�[�u�f�[�^�擾
	{
		p_mystatus		= SaveData_GetMyStatus( p_save );
		p_pokeparty		= SaveData_GetTemotiPokemon( p_save );
		p_zukan			= SaveData_GetZukanWork( p_save );
		p_history		= SaveData_GetWifiHistory( p_save );
		p_time			= SaveData_GetGameTime( p_save );
	}

	
	{// ���O��ݒ�
		STRBUF* p_name;
		p_name = MyStatus_CreateNameString( p_mystatus, heapID );
		// ����M����v���t�B�[���̖��O
		STRBUF_GetStringCode( p_name, p_myprofile->profile.name, PERSON_NAME_SIZE + EOM_SIZE );
		STRBUF_GetStringCode( p_name, p_myprofile->def_name, PERSON_NAME_SIZE + EOM_SIZE );
		STRBUF_Delete( p_name );
	}
	p_myprofile->profile.userid		= DWC_LOBBY_INVALID_USER_ID;
	p_myprofile->profile.trainerid	= MyStatus_GetID( p_mystatus );
	{	// �|�P�����֘A
		POKEMON_PARAM* p_pokemon;
		int pokemax;
		int i;

		pokemax	= PokeParty_GetPokeCount( p_pokeparty );
		for( i=0; i<TEMOTI_POKEMAX; i++ ){
			if( i< pokemax ){
				p_pokemon				= PokeParty_GetMemberPointer( p_pokeparty, i );
				p_myprofile->profile.monsno[i]	= PokeParaGet( p_pokemon, ID_PARA_monsno, NULL );
				p_myprofile->profile.formid[i]	= PokeParaGet( p_pokemon, ID_PARA_form_no, NULL );
				p_myprofile->profile.tamago[i]	= PokeParaGet( p_pokemon, ID_PARA_tamago_flag, NULL );
			}else{
				p_myprofile->profile.monsno[i]	= MONSNO_MAX;
			}
		}
	}
	p_myprofile->profile.sex			= MyStatus_GetMySex( p_mystatus );
	p_myprofile->profile.region_code	= MyStatus_GetRegionCode( p_mystatus );
	p_myprofile->profile.tr_type		= MyStatus_GetTrainerView( p_mystatus );
	p_myprofile->profile.tr_type		= WFLBY_TRTYPE_UNIONCHARNO_Get( p_myprofile->profile.tr_type );	// FIELDOBJ_CODE���烆�j�I���L�����N�^���̃C���f�b�N�X�l�ɕύX
	p_myprofile->profile.nation			= WIFIHISTORY_GetMyNation( p_history );
	p_myprofile->profile.area			= WIFIHISTORY_GetMyArea( p_history );

	p_myprofile->profile.zukan_zenkoku	= ZukanWork_GetZenkokuZukanFlag( p_zukan );
	p_myprofile->profile.game_clear		= MyStatus_GetDpClearFlag( p_mystatus );
	p_myprofile->profile.item			= WFLBY_ITEM_INIT;
	p_myprofile->profile.status			= WFLBY_STATUS_NONE;
	p_myprofile->profile.rom_code		= PM_VERSION;		// ���WiFi�L��̂��߂�Version�̓Q�[���ɂ���Ă����邱��
	p_myprofile->profile.start_sec		= p_time->start_sec;
	{
		int i;
		for( i=0; i<WFLBY_LASTACT_BUFFNUM; i++ ){
			p_myprofile->profile.last_action[i]			= WFLBY_LASTACTION_MAX;
			p_myprofile->profile.last_action_userid[i]	= DWC_LOBBY_INVALID_USER_ID;
		}
	}
	{
		int i;
		for( i=0; i<WFLBY_SELECT_TYPENUM; i++ ){
			p_myprofile->profile.waza_type[i]			= WFLBY_POKEWAZA_TYPE_NONE;
		}
	}

	p_myprofile->profile.anketo.anketo_no = WFLBY_SAVEDATA_QUESTION_NONE;
	p_myprofile->profile.anketo.select	  = ANKETO_ANSWER_NUM;

#ifdef WFLBY_DEBUG_PROFILE_DRAW
	WFLBY_SYSTEM_OSPrint_Profile( &p_myprofile->profile );
#endif
	
	//  CRC�v�Z
	WFLBY_SYSTEM_MyProfile_SetCrc( p_myprofile, p_save );

//	OS_TPrintf( "lang code %d\n", p_myprofile->region_code );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�����̃v���t�B�[����ݒ肷��
 *
 *	@param	p_wk			���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_DWC_SetMyProfile( WFLBY_SYSTEM* p_wk )
{
	int i;
	const WFLBY_USER_PROFILE* cp_lib_data;

	// CRC�`�F�b�N
	if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
		p_wk->error = WFLBY_SYSTEM_ERR_CRC;
		return ;
	}
	
	// �s��������ݒ�
	for( i=0; i<WFLBY_LASTACT_BUFFNUM; i++ ){
		p_wk->myprofile.profile.last_action[i]			= WFLBY_SYSTEM_LASTACTQ_GetData( &p_wk->lastactin_q, i );
		p_wk->myprofile.profile.last_action_userid[i]	= WFLBY_SYSTEM_LASTACTQ_GetUserID( &p_wk->lastactin_q, i );
	}

	// ���O�����C�u�����̃f�[�^�ɂ��镨�ɏ��������Ă��瑗��
	MI_CpuCopy8( p_wk->myprofile.comm_name, p_wk->myprofile.profile.name, sizeof(STRCODE)*(PERSON_NAME_SIZE + EOM_SIZE) );
	
	DWC_LOBBY_SetMyProfile( &p_wk->myprofile.profile );	// �X�V

	// �������������O�����ɖ߂�
	MI_CpuCopy8( p_wk->myprofile.def_name, p_wk->myprofile.profile.name, sizeof(STRCODE)*(PERSON_NAME_SIZE + EOM_SIZE) );

	//  CRC�v�Z
	WFLBY_SYSTEM_MyProfile_SetCrc( &p_wk->myprofile, p_wk->p_save );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃v���t�B�[��CRC�n�b�V���l���蓖��
 *
 *	@param	p_myprofile		�����̃v���t�B�[��
 *	@param	cp_save			�Z�[�u�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_MyProfile_SetCrc( WFLBY_USER_MYPROFILE* p_myprofile, const SAVEDATA* cp_save )
{
	p_myprofile->crc_check = SaveData_CalcCRC( cp_save, &p_myprofile->profile, sizeof(WFLBY_USER_PROFILE) );
//	OS_TPrintf( "crc set %d\n", p_myprofile->crc_check );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃v���t�B�[��CRC�n�b�V���`�F�b�N
 *
 *	@param	cp_myprofile		�����̃v���t�B�[��
 *	@param	cp_save			�Z�[�u�f�[�^
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�s���f�[�^
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_MyProfile_CheckCrc( const WFLBY_USER_MYPROFILE* cp_myprofile, const SAVEDATA* cp_save )
{
	u32 check_crc;

	// CRC�`�F�b�N
	check_crc = SaveData_CalcCRC( cp_save, &cp_myprofile->profile, sizeof(WFLBY_USER_PROFILE) );
//	OS_TPrintf( "crc check %d==%d\n", check_crc, cp_myprofile->crc_check );
	if( check_crc != cp_myprofile->crc_check ){
		GF_ASSERT_MSG( 0, "lobby crc error\n" );
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�s���`�F�b�N��̃v���t�B�[����MY�v���t�B�[���ɐݒ�
 *
 *	@param	p_myprofile		MY�v���t�B�[��
 *	@param	cp_userdata		�s���`�F�b�N��̃v���t�B�[��
 *	@param	profile_size	�s���`�F�b�N��̃v���t�B�[���T�C�Y
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_MyProfileCopy_CheckData( WFLBY_USER_MYPROFILE* p_myprofile, const WFLBY_USER_PROFILE* cp_userdata, u32 profile_size )
{
	p_myprofile->profile = *cp_userdata;

	// ���[�J�����̃v���t�B�[���T�C�Y�ƁA�u����̃v���t�B�[���T�C�Y���������������Ƃ�
	if( sizeof(WFLBY_USER_PROFILE) >= profile_size ){
		MI_CpuCopy8( cp_userdata, &p_myprofile->profile, profile_size );
	}else{
		// �u�����̃v���t�B�[���̂ق����ł����Ƃ�
		MI_CpuCopy8( cp_userdata, &p_myprofile->profile, sizeof(WFLBY_USER_PROFILE) );
	}
	

	// �ʐM�p�̎����̖��O���擾����
	MI_CpuCopy8( cp_userdata->name, p_myprofile->comm_name, sizeof(STRCODE)*(PERSON_NAME_SIZE + EOM_SIZE) );

	//  ���O�͎����̖��O
	MI_CpuCopy8( p_myprofile->def_name, p_myprofile->profile.name, sizeof(STRCODE)*(PERSON_NAME_SIZE + EOM_SIZE) );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[���̕\��
 *
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_OSPrint_Profile( const WFLBY_USER_PROFILE* cp_profile )
{
	int i;

	OS_TPrintf( "user profile\n" );
	OS_TPrintf( "userid=%d\n", cp_profile->userid );
	OS_TPrintf( "trainerid=%d\n", cp_profile->trainerid );
	OS_TPrintf( "name={" );
	for( i=0; i<PERSON_NAME_SIZE + EOM_SIZE; i++ ){
		OS_TPrintf( " %d,", cp_profile->name[i] );
	}
	OS_TPrintf( "}\n" );
	OS_TPrintf( "intime=%d\n", cp_profile->intime.time );
	OS_TPrintf( "wldtime=%d\n", cp_profile->wldtime.time );
	OS_TPrintf( "monsno={" );
	for( i=0; i<TEMOTI_POKEMAX; i++ ){
		OS_TPrintf( " %d,", cp_profile->monsno[i] );
	}
	OS_TPrintf( "}\n" );
	OS_TPrintf( "formid={" );
	for( i=0; i<TEMOTI_POKEMAX; i++ ){
		OS_TPrintf( " %d,", cp_profile->formid[i] );
	}
	OS_TPrintf( "}\n" );
	OS_TPrintf( "tamago={" );
	for( i=0; i<TEMOTI_POKEMAX; i++ ){
		OS_TPrintf( " %d,", cp_profile->tamago[i] );
	}
	OS_TPrintf( "}\n" );
	OS_TPrintf( "sex=%d\n", cp_profile->sex );
	OS_TPrintf( "region_code=%d\n", cp_profile->region_code );
	OS_TPrintf( "tr_type=%d\n", cp_profile->tr_type );
	OS_TPrintf( "nation=%d\n", cp_profile->nation );
	OS_TPrintf( "area=%d\n", cp_profile->area );
	OS_TPrintf( "zukan_zenkoku=%d\n", cp_profile->zukan_zenkoku );
	OS_TPrintf( "game_clear=%d\n", cp_profile->game_clear );
	OS_TPrintf( "item=%d\n", cp_profile->item );
	OS_TPrintf( "rom_code=%d\n", cp_profile->rom_code );
	OS_TPrintf( "status=%d\n", cp_profile->status );
	OS_TPrintf( "start_sec=%d\n", cp_profile->start_sec );
	OS_TPrintf( "last_action={" );
	for( i=0; i<WFLBY_LASTACT_BUFFNUM; i++ ){
		OS_TPrintf( " %d,", cp_profile->last_action[i] );
	}
	OS_TPrintf( "}\n" );
	OS_TPrintf( "last_action_userid={" );
	for( i=0; i<WFLBY_LASTACT_BUFFNUM; i++ ){
		OS_TPrintf( " %d,", cp_profile->last_action_userid[i] );
	}
	OS_TPrintf( "}\n" );
}






//----------------------------------------------------------------------------
/**
 *	@brief	�t���O���[�N	�C�x���g�t���O��������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FLAG_ClearEvent( WFLBY_SYSTEM_FLAG* p_wk )
{
	p_wk->event_on = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I���^�C���I�[�o�[�J�E���g
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_EVENT_Count( WFLBY_SYSTEM_EVENT* p_wk, WFLBY_SYSTEM_FLAG* p_flag, WFLBY_FLOAT_DATA* p_float, WFLBY_FIRE_DATA* p_fire_data )
{
	// �J�E���g���I�������J�n�C�x���g�𔭓�������
	
	if( (p_wk->end_count-1) >= 0 ){
		p_wk->end_count --;

		if( p_wk->end_count == WFLBY_END_BGMFADEOUT ){
			//�t�F�[�h�A�E�g�J�n
			Snd_BgmFadeOut( BGM_VOL_MIN, WFLBY_END_BGM_FADEOUT_FRAME );
			p_wk->bgm_fadeout = TRUE;	// �t�F�[�h�A�E�g��
		}
	}else if( p_wk->end_count == 0 ){
		p_wk->end		= TRUE;
		p_flag->event_on	|= 1 << WFLBY_EVENT_CHG_CLOSE;
		p_wk->end_count--;
	}

	if( (p_wk->neon_count-1) >= 0 ){
		p_wk->neon_count --;
	}else if( p_wk->neon_count == 0 ){
		p_flag->event_on	|= 1 << WFLBY_EVENT_CHG_NEON_FLOOR;
		p_flag->event_on	|= 1 << WFLBY_EVENT_CHG_NEON_MONU;
		p_flag->event_on	|= 1 << WFLBY_EVENT_CHG_NEON_ROOM;
		p_wk->neon_count --;
	}

	if( (p_wk->hanabi_count-1) >= 0 ){
		p_wk->hanabi_count --;
	}else if( p_wk->hanabi_count == 0 ){
		p_wk->hababi		= WFLBY_EVENT_HANABI_PLAY;
		p_flag->event_on	|= 1 << WFLBY_EVENT_CHG_FIRE;
		p_wk->hanabi_count--;

		// �ԉΓ���J�n
		WFLBY_SYSTEM_FIREDATA_Start( p_fire_data );
	}

	if( (p_wk->parade_count-1) >= 0 ){
		p_wk->parade_count --;
	}else if( p_wk->parade_count == 0 ){

		// �t���[�g����J�n
		WFLBY_SYSTEM_FLOAT_Start( p_float );

		// BGM�ύX
		if( p_wk->bgm_lock == FALSE ){
			Snd_BgmPlay( SEQ_PL_WIFIPARADE );	// �T�E���h�f�[�^���[�h
		}
		p_wk->parade		= TRUE;
		p_flag->event_on	|= 1 << WFLBY_EVENT_CHG_PARADE;
		p_wk->parade_count--;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�f�[�^������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_EVENT_Init( WFLBY_SYSTEM_EVENT* p_wk )
{
	p_wk->lock			= FALSE;
	p_wk->end			= FALSE;
	p_wk->hababi		= WFLBY_EVENT_HANABI_BEFORE;
	p_wk->parade		= FALSE;
	p_wk->mini			= FALSE;
	p_wk->endcm			= FALSE;
	p_wk->neon_room		= PPW_LOBBY_TIME_EVENT_NEON_A0;
	p_wk->neon_floor	= PPW_LOBBY_TIME_EVENT_NEON_B0;
	p_wk->neon_monu		= PPW_LOBBY_TIME_EVENT_NEON_C0;

	p_wk->end_count		= -1;		// �I�����Ԃ܂ł̃J�E���^
	p_wk->neon_count	= -1;		// �l�I���J�n�܂ł̃J�E���^
	p_wk->hanabi_count	= -1;		// �ԉΊJ�n�܂ł̃J�E���^
	p_wk->parade_count	= -1;		// �p���[�h�J�n�܂ł̃J�E���^
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���gBGM�̍Đ�
 *
 *	@param	cp_wk	���[�N
 *	@param	bgm		BGM
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_EVENT_PlayBgm( const WFLBY_SYSTEM_EVENT* cp_wk,  u32 bgm )
{
	if( cp_wk->bgm_lock == FALSE ){
		Snd_BgmPlay( bgm );	// �T�E���h�f�[�^���[�h

		// ���Ɖ������Ȃ牺���Ă���
		WFLBY_SYSTEM_EVENT_SetBgmVolume( cp_wk );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���O��Ԃ�BGM�{�����[���ݒ�
 *
 *	@param	cp_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_EVENT_SetBgmVolume( const WFLBY_SYSTEM_EVENT* cp_wk )
{
	// �t�F�[�h�A�E�g���͉������Ȃ�
	if( cp_wk->bgm_fadeout == FALSE ){
		
		if( cp_wk->bgm_vl_down == TRUE ){
			Snd_PlayerSetPlayerVolume( PLAYER_BGM, BGM_VOL_HIROBA_APP );
		}else{
			Snd_PlayerSetPlayerVolume( PLAYER_BGM, BGM_VOL_MAX );
		}
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���t���O	���b�N�Z�b�g
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_MgFlag_SetLock( WFLBY_SYSTEM_APLFLAG* p_wk )
{
	p_wk->mg_lock_count = WFLBY_MINIGAME_LOCK_TIME;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���t���O	���b�N�����擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	���b�N��
 *	@retval	FALSE	���b�N���ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_MgFlag_GetLock( const WFLBY_SYSTEM_APLFLAG* cp_wk )
{
	if( cp_wk->mg_lock_count > 0 ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���t���O	�Ǘ�
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_MgFlag_Cont( WFLBY_SYSTEM_APLFLAG* p_wk )
{
	if( p_wk->mg_lock_count > 0 ){
		p_wk->mg_lock_count --;
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[������������
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_MG_Init( WFLBY_SYSTEM_MG* p_wk, u32 heapID )
{
	p_wk->p_mystatus = MyStatus_AllocWork( heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[������j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_MG_Exit( WFLBY_SYSTEM_MG* p_wk )
{
	sys_FreeMemoryEz( p_wk->p_mystatus );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[������MYSTATUS��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_profile	�v���t�B�[��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_MG_SetMyStatus( WFLBY_SYSTEM_MG* p_wk, const WFLBY_USER_MYPROFILE* cp_profile, u32  heapID )
{
	WFLBY_USER_PROFILE* p_buff;

	// �o�b�t�@�쐬
	p_buff = sys_AllocMemory( heapID,  sizeof(WFLBY_USER_PROFILE) );
	MI_CpuCopyFast( &cp_profile->profile, p_buff, sizeof(WFLBY_USER_PROFILE) );

	// ���O�����C�u�����̃f�[�^�ɂ��镨�ɏ��������Ă���MYSTATUS���擾����
	MI_CpuCopy8( cp_profile->comm_name, p_buff->name, sizeof(STRCODE)*(PERSON_NAME_SIZE + EOM_SIZE) );
	
	// MYSTATUS���擾
	WFLBY_SYSTEM_GetProfileMyStatus( p_buff, p_wk->p_mystatus, heapID );

	// �o�b�t�@�j��
	sys_FreeMemoryEz( p_buff );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�����I���t���O��ݒ肷��
 *	
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_AplFlag_SetForceEnd( WFLBY_SYSTEM_APLFLAG* p_wk )
{
	p_wk->apl_force_end = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����I���t���O���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�����I��
 *	@retval	FALSE	�����I������Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_AplFlag_GetForceEnd( const WFLBY_SYSTEM_APLFLAG* cp_wk )
{
	return cp_wk->apl_force_end;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�f�[�^�N���A
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_GadGetClear( WFLBY_SYSTEM_GADGET* p_wk )
{
	memset( p_wk, 0, sizeof(WFLBY_SYSTEM_GADGET) );
}




//----------------------------------------------------------------------------
/**
 *	@brief	��b���[�N��������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_Init( WFLBY_SYSTEM_TALK* p_wk )
{
	memset( p_wk, 0, sizeof(WFLBY_SYSTEM_TALK) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�I�����̃N���A
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_EndClear( WFLBY_SYSTEM_TALK* p_wk )
{
	p_wk->talk_idx	= 0;
	p_wk->talk_seq	= WFLBY_TALK_SEQ_NONE;
	p_wk->talk_type	= WFLBY_TALK_TYPE_NONE;

	// ��M�҂���~
	WFLBY_SYSTEM_TALK_StopRecvWait( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ً}�I�����̃N���A
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_ExEndClear( WFLBY_SYSTEM_TALK* p_wk )
{
	p_wk->talk_idx	= 0;
	p_wk->talk_seq	= WFLBY_TALK_SEQ_EXEND;

	// ��M�҂���~
	WFLBY_SYSTEM_TALK_StopRecvWait( p_wk );
	p_wk->talk_type	= WFLBY_TALK_TYPE_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̐l�Ɖ�b�������Ƃ����邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE	��b�������Ƃ�����
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_TALK_CheckMsk( const WFLBY_SYSTEM_TALK* cp_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );
	if( cp_wk->talk_msk & (1<<idx) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�������Ƃ����邱�Ƃ�ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_SetMsk( WFLBY_SYSTEM_TALK* p_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );

	p_wk->talk_msk |= (1<<idx);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�X�N�N���A
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_ClearMsk( WFLBY_SYSTEM_TALK* p_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );

	p_wk->talk_msk &= ~(1<<idx);
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�����J�E���g
 *	
 *	@param	p_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_AddCount( WFLBY_SYSTEM_TALK* p_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );

	// ��b�����񐔂𐔂���
	if( (p_wk->talk_count[idx] + 1) <= WFLBY_TALK_MAX ){

		p_wk->talk_count[idx]++;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�J�E���g�N���A
 *
 *	@param	p_wk
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_ClearCount( WFLBY_SYSTEM_TALK* p_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );
	p_wk->talk_count[ idx ] = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���M�f�[�^�ݒ�		���̃V�[�P���X�𓯎��ɐݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_SetSendDataNowSeq( WFLBY_SYSTEM_TALK* p_wk, u16 data )
{
	p_wk->send_data.seq		= p_wk->talk_seq;
	p_wk->send_data.data	= data;
}


//----------------------------------------------------------------------------
/**
 *	@brief	��b����
 *
 *	@param	p_wk		���[�N
 *	@param	userid		���[�UID
 *	@param	ans			��b����FTRUE	���Ȃ��FFALSE
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_SendTalkAns( WFLBY_SYSTEM* p_wk, s32 userid, BOOL ans )
{
	p_wk->talk.send_data.seq		= WFLBY_TALK_SEQ_A_SEL;
	p_wk->talk.send_data.data	= ans;
	DWC_LOBBY_SendPlayerMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TALK_ANS, userid, &p_wk->talk.send_data, sizeof(WFLBY_SYSTEM_TALK_DATA) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�f�[�^�G���[�ɂ��ؒf����
 *
 *	@param	p_wk		���[�N
 *	@param	userid		���[�UID
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_SendTalkExEnd( WFLBY_SYSTEM* p_wk, s32 userid )
{
	p_wk->talk.talk_seq		= WFLBY_TALK_SEQ_EXEND;
	p_wk->talk.send_data.seq = WFLBY_TALK_SEQ_EXEND;
	DWC_LOBBY_SendPlayerMsg( WFLBY_SYSTEM_MAIN_COMMCMD_TALK_DATA, userid, &p_wk->talk.send_data, sizeof(WFLBY_SYSTEM_TALK_DATA) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��M�҂����J�n����
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_StartRecvWait( WFLBY_SYSTEM_TALK* p_wk )
{
	p_wk->recv_wait_on	= TRUE;
	p_wk->recv_wait		= 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��M�҂����I������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_StopRecvWait( WFLBY_SYSTEM_TALK* p_wk )
{
	p_wk->recv_wait_on	= FALSE;
	p_wk->recv_wait		= 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��M�҂��A�b�v�f�[�g
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TALK_UpdateRecvWait( WFLBY_SYSTEM_TALK* p_wk )
{
	if( p_wk->recv_wait_on == TRUE ){
		if( (p_wk->recv_wait + 1) <= WFLBY_TALK_RECV_WAIT ){
			p_wk->recv_wait ++;
		} 
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��M�҂����Ԃ��`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�܂���M�҂�����OK
 *	@retval	FALSE	�����ؒf�����Ⴄ
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_TALK_CheckRecvWait( const WFLBY_SYSTEM_TALK* cp_wk )
{
	if( cp_wk->recv_wait_on == FALSE ){
		return TRUE;
	}
	if( cp_wk->recv_wait >= WFLBY_TALK_RECV_WAIT ){
		return FALSE;	
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b��M�҂����Ԃ��擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	��M�҂�����
 */
//-----------------------------------------------------------------------------
static s16	WFLBY_SYSTEM_TALK_GetRacvWaitTime( const WFLBY_SYSTEM_TALK* cp_wk )
{
	return cp_wk->recv_wait;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���A�g�s�b�N��M
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TOPIC_SetConnect( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic )
{
	const WFLBY_USER_PROFILE* cp_profile;
	int i;
	u32 idx;
	NEWS_DATA_SET_CONNECT data;

	if( cp_topic->num != 2 ){
		return ;
	}

	for( i=0; i<2; i++ ){
		idx = DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[i] );
		if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
			cp_profile = WFLBY_SYSTEM_GetUserProfile( p_wk, idx );
			WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_wk->glbdata.p_mystatus[i], p_wk->heapID );
		}else{
			return ;
		}
	}
	
	data.cp_p1	= p_wk->glbdata.p_mystatus[0];
	data.cp_p2	= p_wk->glbdata.p_mystatus[1];
	data.idx_p1	= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[0] );
	data.idx_p2	= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[1] );
	
	NEWS_DSET_SetConnect( p_wk->glbdata.p_lobbynews,
			&data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�e�������g�s�b�N��M
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TOPIC_SetItem( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic )
{
	const WFLBY_USER_PROFILE* cp_profile;
	int i;
	u32 idx;
	NEWS_DATA_SET_ITEM data;

	if( cp_topic->num != 2 ){
		return ;
	}


	for( i=0; i<2; i++ ){
		idx = DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[i] );
		if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
			cp_profile = WFLBY_SYSTEM_GetUserProfile( p_wk, idx );
			WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_wk->glbdata.p_mystatus[i], p_wk->heapID );
		}else{
			return ;
		}
	}

	data.cp_p1	= p_wk->glbdata.p_mystatus[0];
	data.cp_p2	= p_wk->glbdata.p_mystatus[1];
	data.idx_p1	= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[0] );
	data.idx_p2	= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[1] );
	data.item	= cp_topic->item;
	
	NEWS_DSET_SetItem( p_wk->glbdata.p_lobbynews,
			&data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���g�s�b�N��M
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TOPIC_SetMinigame( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic )
{
	const WFLBY_USER_PROFILE* cp_profile;
	int i;
	MYSTATUS* p_mystatus[ WFLBY_MINIGAME_MAX ];
	u32 idx;
	NEWS_DATA_SET_MINIGAME data;


	if( (cp_topic->num > WFLBY_MINIGAME_MAX) || (cp_topic->num <= 0) ){
		return ;
	}

	if( cp_topic->play == TRUE ){
		for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
			if( i<cp_topic->num ){
				idx = DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[i] );
				if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
					cp_profile = WFLBY_SYSTEM_GetUserProfile( p_wk, idx );
					WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_wk->glbdata.p_mystatus[i], p_wk->heapID );
					p_mystatus[i] = p_wk->glbdata.p_mystatus[i];
				}else{
					return ;
				}
			}else{
				p_mystatus[i] = NULL;
			}
		}
	}else{
		// ��W���̂Ƃ��͐e�̖��O�̂�
		for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
			if( i==0 ){
				u32 idx = DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[i] );
				if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
					cp_profile = WFLBY_SYSTEM_GetUserProfile( p_wk, idx );
					WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_wk->glbdata.p_mystatus[i], p_wk->heapID );
					p_mystatus[i] = p_wk->glbdata.p_mystatus[i];
				}else{
					return ;
				}
			}else{
				p_mystatus[i] = NULL;
			}
		}
	}

	data.minigame	= cp_topic->minigame;
	data.num		= cp_topic->num;
	data.cp_p1		= p_mystatus[0];
	data.cp_p2		= p_mystatus[1];
	data.cp_p3		= p_mystatus[2];
	data.cp_p4		= p_mystatus[3];
	data.idx_p1		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[0] );
	data.idx_p2		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[1] );
	data.idx_p3		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[2] );
	data.idx_p4		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[3] );
	data.play		= cp_topic->play;
	
	NEWS_DSET_SetMiniGame( p_wk->glbdata.p_lobbynews,
			&data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ճ{�[�h�g�s�b�N��M
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TOPIC_SetFootBoard( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic )
{
	const WFLBY_USER_PROFILE* cp_profile;
	int i;
	u32 idx;
	NEWS_DATA_SET_FOOTBOARD data;

	if( (cp_topic->num > WFLBY_FOOT_MAX) || (cp_topic->num <= 0) ){
		return ;
	}

	idx = DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[0] );
	if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
		cp_profile = WFLBY_SYSTEM_GetUserProfile( p_wk, idx );
		WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_wk->glbdata.p_mystatus[0], p_wk->heapID );
	}else{
		return ;
	}

	data.board	= cp_topic->minigame;
	data.num	= cp_topic->num;
	data.cp_p1	= p_wk->glbdata.p_mystatus[0];
	data.idx_p1	= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[0] );
	
	NEWS_DSET_SetFootBoard( p_wk->glbdata.p_lobbynews,
			&data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���v�g�s�b�N��M
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TOPIC_SetWldTimer( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic )
{
	const WFLBY_USER_PROFILE* cp_profile;
	int i;
	MYSTATUS* p_mystatus[ WFLBY_MINIGAME_MAX ];
	u32 idx;
	NEWS_DATA_SET_WORLDTIMER data;

	if( (cp_topic->num > WFLBY_MINIGAME_MAX) || (cp_topic->num <= 0) ){
		return ;
	}

	for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
		if( i<cp_topic->num ){
			idx = DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[i] );
			if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
				cp_profile = WFLBY_SYSTEM_GetUserProfile( p_wk, idx );
				WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_wk->glbdata.p_mystatus[i], p_wk->heapID );
				p_mystatus[i] = p_wk->glbdata.p_mystatus[i];
			}else{
				return ;
			}
		}else{
			p_mystatus[i] = NULL;
		}
	}

	data.num		= cp_topic->num;
	data.cp_p1		= p_mystatus[0];
	data.cp_p2		= p_mystatus[1];
	data.cp_p3		= p_mystatus[2];
	data.cp_p4		= p_mystatus[3];
	data.idx_p1		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[0] );
	data.idx_p2		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[1] );
	data.idx_p3		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[2] );
	data.idx_p4		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[3] );
	
	NEWS_DSET_SetWorldTimer( p_wk->glbdata.p_lobbynews,
			&data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�j���[�X�g�s�b�N��M
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TOPIC_SetNews( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic )
{
	const WFLBY_USER_PROFILE* cp_profile;
	int i;
	MYSTATUS* p_mystatus[ WFLBY_MINIGAME_MAX ];
	u32 idx;
	NEWS_DATA_SET_LOBBYNEWS data;

	if( (cp_topic->num > WFLBY_MINIGAME_MAX) || (cp_topic->num <= 0) ){
		return ;
	}

	for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
		if( i<cp_topic->num ){
			idx = DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[i] );
			if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
				cp_profile = WFLBY_SYSTEM_GetUserProfile( p_wk, idx );
				WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_wk->glbdata.p_mystatus[i], p_wk->heapID );
				p_mystatus[i] = p_wk->glbdata.p_mystatus[i];
			}else{
				return ;
			}
		}else{
			p_mystatus[i] = NULL;
		}
	}

	data.num		= cp_topic->num;
	data.cp_p1		= p_mystatus[0];
	data.cp_p2		= p_mystatus[1];
	data.cp_p3		= p_mystatus[2];
	data.cp_p4		= p_mystatus[3];
	data.idx_p1		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[0] );
	data.idx_p2		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[1] );
	data.idx_p3		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[2] );
	data.idx_p4		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[3] );
	
	NEWS_DSET_SetLobbyNews( p_wk->glbdata.p_lobbynews,
			&data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[�����ʉ��	�g�s�b�N�f�[�^�ݒ�
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TOPIC_SetMgResult( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic )
{
	const WFLBY_USER_PROFILE* cp_profile;
	int i;
	MYSTATUS* p_mystatus[ WFLBY_MINIGAME_MAX ];
	u32 idx;
	NEWS_DATA_SET_MGRESULT	data;


	if( (cp_topic->num > WFLBY_MINIGAME_MAX) || (cp_topic->num <= 0) ){
		return ;
	}

	for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
		if( i<cp_topic->num ){
			idx = DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[i] );
			if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
				cp_profile = WFLBY_SYSTEM_GetUserProfile( p_wk, idx );
				WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_wk->glbdata.p_mystatus[i], p_wk->heapID );
				p_mystatus[i] = p_wk->glbdata.p_mystatus[i];
			}else{
				return ;
			}
		}else{
			p_mystatus[i] = NULL;
		}
	}

	data.minigame	= cp_topic->minigame;
	data.num		= cp_topic->num;
	data.cp_p1		= p_mystatus[0];
	data.cp_p2		= p_mystatus[1];
	data.cp_p3		= p_mystatus[2];
	data.cp_p4		= p_mystatus[3];
	data.idx_p1		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[0] );
	data.idx_p2		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[1] );
	data.idx_p3		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[2] );
	data.idx_p4		= DWC_LOBBY_GetUserIDIdx( cp_topic->lobby_id[3] );
	
	NEWS_DSET_SetMgResult( p_wk->glbdata.p_lobbynews,
			&data );
}

static void WFLBY_SYSTEM_TOPIC_SetDummy( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�g�s�b�N��M
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_TOPIC_SetEvent( WFLBY_SYSTEM* p_wk, WFLBY_EVENTTYPE event )
{
	NEWS_DATA_SET_TIMEEVENT data;

	data.cp_time	= &p_wk->glbdata.worldtime;
	data.event_no	= event;
	
	NEWS_DSET_SetTimeEvent( p_wk->glbdata.p_lobbynews, &data );
}





//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�f�[�^�Ǘ�	������
 *
 *	@param	p_float		�t���[�g�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FLOAT_Init( WFLBY_FLOAT_DATA* p_float )
{
	memset( p_float, 0, sizeof(WFLBY_FLOAT_DATA) );
	MI_CpuFill32( p_float->reserve, DWC_LOBBY_USERIDTBL_IDX_NONE, sizeof(u32)*(WFLBY_FLOAT_MAX*WFLBY_FLOAT_ON_NUM) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g����J�n
 *
 *	@param	p_float		�t���[�g�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FLOAT_Start( WFLBY_FLOAT_DATA* p_float )
{
	p_float->start = TRUE;

	//  ���̒i�K�ŗ\�񂪓����Ă���\�����\������
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g����
 *
 *	@param	p_float		�t���[�g�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FLOAT_Main( WFLBY_FLOAT_DATA* p_float )
{
	int i;

	if( p_float->start == FALSE ){
		return ;
	}

	for( i=0; i<WFLBY_FLOAT_MAX; i++ ){
		// ����J�n�`�F�b�N
		if( p_float->all_count == sc_WFLBY_FLOAT_TIMING[i] ){
			// ���s�J�n
			p_float->move[i] = WFLBY_FLOAT_STATE_MOVE;
		}

		// �t���[�g����J�E���g
		if( p_float->move[i] == WFLBY_FLOAT_STATE_MOVE ){
			if( (p_float->count[i] + 1) <= WFLBY_FLOAT_COUNT_ONE ){
				p_float->count[i] ++;
			}else{
				// �����~
				p_float->move[i] = WFLBY_FLOAT_STATE_AFTER;
			}
		}
	}

	if( (p_float->all_count + 1) <= WFLBY_FLOAT_COUNT_MAX ){
		p_float->all_count ++;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�C���f�b�N�X����i���o�[�ƃI�t�Z�b�g���擾
 *
 *	@param	idx
 *	@param	p_num
 *	@param	p_ofs 
 */
//-----------------------------------------------------------------------------
static inline void WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( u32 idx, u32* p_num, u32* p_ofs )
{
	*p_num = idx / WFLBY_FLOAT_ON_NUM;
	*p_ofs = idx % WFLBY_FLOAT_ON_NUM;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\����̐ݒ�
 *
 *	@param	p_float		�t���[�g�f�[�^
 *	@param	plidx		�v���C���[IDX
 *	@param	station		�w�萔
 *
 *	@retval	DWC_LOBBY_USERIDTBL_IDX_NONE	�\�񎸔s
 *	@retval	���̑�							�\�񐬌�
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_SYSTEM_FLOAT_SetReserve( WFLBY_FLOAT_DATA* p_float, u32 plidx, WFLBY_FLOAT_STATION_TYPE station )
{
	u32 idx;
	int i, j;
	BOOL result;
	u32 on_plidx;


	// �����͂��łɏ���ĂȂ����`�F�b�N
	idx = WFLBY_SYSTEM_FLOAT_GetIdx( p_float, plidx );
	if( idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){

		// ���b�N���������Ă���Ƃ���ȉ��ŁA����O�����쒆�ŁA
		// �󂢂Ă���Ƃ��낪�Ȃ����`�F�b�N
		for( i=0; i<WFLBY_FLOAT_MAX; i++ ){
			if( WFLBY_SYSTEM_FLOAT_GetMove( p_float, i ) != WFLBY_FLOAT_STATE_AFTER ){
				for( j=0; j<WFLBY_FLOAT_ON_NUM; j++ ){
					idx = (i*WFLBY_FLOAT_ON_NUM)+j;

					result = WFLBY_SYSTEM_FLOAT_CheckReserveLock( p_float, idx, station );
					if( result == FALSE ){
						// �����󂢂Ă���H
						on_plidx = WFLBY_SYSTEM_FLOAT_GetReserve( p_float, idx );
						if( on_plidx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
							// �����ɏ悹��
							p_float->reserve[ i ][ j ] = plidx;
//							OS_TPrintf( "set reserve[%d][%d]=%d\n", i, j, p_float->reserve[ i ][ j ] );
							return idx;
						}
					}

//					OS_TPrintf( "reserve[%d][%d]=%d\n", i, j, p_float->reserve[ i ][ j ] );
				}
			}
		}
	}

	// �̂�Ȃ�
	return DWC_LOBBY_USERIDTBL_IDX_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\��j������
 *
 *	@param	p_float		�t���[�g
 *	@param	plidx		�v���C���[�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FLOAT_ResetReserve( WFLBY_FLOAT_DATA* p_float, u32 plidx )
{
	u32 idx;
	u32 float_num, float_ofs;

	idx = WFLBY_SYSTEM_FLOAT_GetIdx( p_float, plidx );
	if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
		WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( idx, &float_num, &float_ofs );
		p_float->reserve[float_num][float_ofs] = DWC_LOBBY_USERIDTBL_IDX_NONE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\������擾
 *
 *	@param	cp_float	�t���[�g�f�[�^
 *	@param	idx			�f�[�^�C���f�b�N�X
 *
 *	@retval	DWC_LOBBY_USERIDTBL_IDX_NONE	�̂��ĂȂ�
 *	@retval	���̑�	����Ă�
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_SYSTEM_FLOAT_GetReserve( const WFLBY_FLOAT_DATA* cp_float, u32 idx )
{
	u32 float_num;
	u32 float_ofs;

	GF_ASSERT( idx < WFLBY_FLOAT_MAX*WFLBY_FLOAT_ON_NUM );

	WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( idx, &float_num, &float_ofs );

	return cp_float->reserve[ float_num ][ float_ofs ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�����삵�Ă��邩�`�F�b�N
 *
 *	@param	cp_float		�t���[�g���[�N
 *	@param	floatidx		�t���[�g�C���f�b�N�X
 *
 *	@retval	WFLBY_FLOAT_STATE_BEFORE,	// ����O
 *	@retval	WFLBY_FLOAT_STATE_MOVE,		// ���쒆
 *	@retval	WFLBY_FLOAT_STATE_AFTER,	// �����
 */
//-----------------------------------------------------------------------------
static WFLBY_FLOAT_MOVE_STATE WFLBY_SYSTEM_FLOAT_GetMove( const WFLBY_FLOAT_DATA* cp_float, u32 floatidx )
{
	GF_ASSERT( floatidx < WFLBY_FLOAT_MAX );
	return cp_float->move[ floatidx ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�E���g�l�̎擾
 *
 *	@param	cp_float		�t���[�g�f�[�^
 *	@param	idx				�C���f�b�N�X
 *
 *	@return	�J�E���g�l
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_SYSTEM_FLOAT_GetCount( const WFLBY_FLOAT_DATA* cp_float, u32 floatidx )
{
	GF_ASSERT( floatidx < WFLBY_FLOAT_MAX );

	return cp_float->count[ floatidx ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���̎n����ݒ�
 *
 *	@param	cp_float	�t���[�g�f�[�^
 *	@param	plidx		�v���C���[�C���f�b�N�X
 */	
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FLOAT_SetAnm( WFLBY_FLOAT_DATA* p_float, u32 plidx )
{
	u32 float_num;
	u32 float_ofs;
	u32 idx;

	idx = WFLBY_SYSTEM_FLOAT_GetIdx( p_float, plidx );
	if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
		WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( idx, &float_num, &float_ofs );
		p_float->anm[ float_num ][ float_ofs ] = TRUE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�A�j�����Đ����邩�`�F�b�N
 *
 *	@param	cp_float	�t���[�g�f�[�^
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE	�t���[�g�A�j���Đ�
 *	@retval	FALSE	�t���[�g�A�j���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_FLOAT_GetAnm( const WFLBY_FLOAT_DATA* cp_float, u32 idx )
{
	u32 float_num;
	u32 float_ofs;

	GF_ASSERT( idx < WFLBY_FLOAT_MAX*WFLBY_FLOAT_ON_NUM );

	WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( idx, &float_num, &float_ofs );
	return cp_float->anm[ float_num ][ float_ofs ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�A�j���Đ��f�[�^�N���[��
 *
 *	@param	p_float		�t���[�g�f�[�^
 *	@param	idx			�t���[�g�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FLOAT_CleanAnm( WFLBY_FLOAT_DATA* p_float, u32 idx )
{
	u32 float_num;
	u32 float_ofs;

	GF_ASSERT( idx < WFLBY_FLOAT_MAX*WFLBY_FLOAT_ON_NUM );

	WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( idx, &float_num, &float_ofs );
	p_float->anm[ float_num ][ float_ofs ] = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�f�[�^	�v���C���[�h�c�w�������Ă���t���[�g�C���f�b�N�X���擾
 *
 *	@param	cp_float	�t���[�g�f�[�^
 *	@param	plidx		�v���C���[�C���f�b�N�X
 *
 *	@retval		�t���[�g�C���f�b�N�X
 *	@retval		DWC_LOBBY_USERIDTBL_IDX_NONE	���Ȃ�
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_SYSTEM_FLOAT_GetIdx( const WFLBY_FLOAT_DATA* cp_float, u32 plidx )
{
	int i, j;

	for( i=0; i<WFLBY_FLOAT_MAX; i++ ){
		
		for( j=0; j<WFLBY_FLOAT_ON_NUM; j++ ){
			if( cp_float->reserve[i][j] == plidx ){
				return i*WFLBY_FLOAT_ON_NUM+j;
			}
		}
	}

	return DWC_LOBBY_USERIDTBL_IDX_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\�񃍃b�N��������
 *
 *	@param	cp_float	�t���[�g
 *	@param	idx			���ȃC���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FLOAT_SetReserveLock( WFLBY_FLOAT_DATA* p_float, u32 idx, WFLBY_FLOAT_STATION_TYPE station )
{
	u32 float_num;
	u32 float_ofs;

	GF_ASSERT( idx < WFLBY_FLOAT_MAX*WFLBY_FLOAT_ON_NUM );

	WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( idx, &float_num, &float_ofs );
	p_float->reserve_lock[ float_num ][ float_ofs ] = station+1;	// ���̉w�ȏ�̉w����\�񂵂Ȃ��Ɠ���Ȃ�
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̍��Ȃɗ\��̃��b�N���������Ă��邩�`�F�b�N
 *
 *	@param	cp_float	�t���[�g
 *	@param	idx			���ȃC���f�b�N�X
 *	@param	station		�w�萔
 *
 *	@retval	TRUE	�\�񃍃b�N���������Ă���
 *	@retval	FALSE	�\�񃍃b�N���������Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_FLOAT_CheckReserveLock( const WFLBY_FLOAT_DATA* cp_float, u32 idx, WFLBY_FLOAT_STATION_TYPE station )
{
	u32 float_num;
	u32 float_ofs;

	GF_ASSERT( idx < WFLBY_FLOAT_MAX*WFLBY_FLOAT_ON_NUM );

	WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( idx, &float_num, &float_ofs );

	// �����Ă���l�ȏ�Ȃ�\��ł�����
	if( cp_float->reserve_lock[float_num][float_ofs] <= station ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ȃ̉w���b�N�f�[�^���擾
 *
 *	@param	cp_float	�t���[�g�f�[�^
 *	@param	idx			���ȃC���f�b�N�X
 *
 *	@return	�w�萔
 */
//-----------------------------------------------------------------------------
static WFLBY_FLOAT_STATION_TYPE WFLBY_SYSTEM_FLOAT_GetReserveLock( const WFLBY_FLOAT_DATA* cp_float, u32 idx )
{
	u32 float_num;
	u32 float_ofs;

	GF_ASSERT( idx < WFLBY_FLOAT_MAX*WFLBY_FLOAT_ON_NUM );

	WFLBY_SYSTEM_FLOAT_GetFloatNumOffs( idx, &float_num, &float_ofs );

	// �����Ă���l���傫��������\��ł�����
	return cp_float->reserve_lock[ float_num ][ float_ofs ];
}	
		










// �ʐM�R�[���o�b�N
//----------------------------------------------------------------------------
/**
 *	@brief	��b���N�G�X�g
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_COMMCMD_TALK_Req( s32 userid, const void* cp_data, u32 size, void* p_work )
{
	WFLBY_SYSTEM* p_wk = p_work;
	u32 idx;
	const WFLBY_SYSTEM_TALK_DATA* cp_talkdata;
	u32 status;
	BOOL now_talk;

	// �f�[�^�擾
	cp_talkdata = cp_data;

	// ���̐l��IDX���擾
	idx = DWC_LOBBY_GetUserIDIdx( userid );

	// CRC�`�F�b�N
	if( WFLBY_SYSTEM_MyProfile_CheckCrc( &p_wk->myprofile, p_wk->p_save ) == FALSE ){
		p_wk->error = WFLBY_SYSTEM_ERR_CRC;
	}
	
	status = WFLBY_SYSTEM_GetProfileStatus( &p_wk->myprofile.profile );

	now_talk = WFLBY_SYSTEM_CheckTalk( p_wk );
	
	// ����b�ł����Ԃ��`�F�b�N
	// ��Ԃ��Z�������A�m��Ȃ��l�Ȃ炲�߂�Ȃ���
	// ���̐l���牽�x���b���������Ă���ꍇ
	if( (status != WFLBY_STATUS_LOGIN) ||
		(idx == DWC_LOBBY_USERIDTBL_IDX_NONE) ||
		(cp_talkdata->seq != WFLBY_TALK_SEQ_B_ANS) ||
		(now_talk == TRUE)	||
		(WFLBY_SYSTEM_CheckTalkCount( p_wk, idx ) == FALSE)){

	
		// ��b�ł��Ȃ����Ƃ𑗐M
//		OS_TPrintf( "talk don't  user=%d\n", userid );
//
//		OS_TPrintf( "status %d\n", status );
//		OS_TPrintf( "idx %d\n", idx );
//		OS_TPrintf( "cp_talkdata->seq %d\n", WFLBY_TALK_SEQ_B_ANS );
//		OS_TPrintf( "WFLBY_SYSTEM_CheckTalkCount %d\n", WFLBY_SYSTEM_CheckTalkCount( p_wk, idx ) );
		WFLBY_SYSTEM_TALK_SendTalkAns( p_wk, userid, FALSE );
		return ;
	}

	// ��b�ł���悤�Ȃ̂ŁA��������
	WFLBY_SYSTEM_TALK_SendTalkAns( p_wk, userid, TRUE );

	// ���߂Ă��`�F�b�N
	if( WFLBY_SYSTEM_TALK_CheckMsk( &p_wk->talk, idx ) == FALSE ){
		p_wk->talk.talk_first = TRUE;
	}else{
		p_wk->talk.talk_first = FALSE;
	}

	// ��Ԃ���b�ɂ���
//	WFLBY_SYSTEM_TALK_SetMsk( &p_wk->talk, idx );
	p_wk->talk.talk_idx		= idx;
	p_wk->talk.talk_seq		= WFLBY_TALK_SEQ_A_SEL;
	p_wk->talk.talk_type	= WFLBY_TALK_TYPE_B;
	p_wk->talk.talk_b_start	= FALSE;

	// ��b�����҂��J�n
	WFLBY_SYSTEM_TALK_StartRecvWait( &p_wk->talk );

//	OS_TPrintf( "talk match  user=%d\n", userid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b���N�G�X�g�ւ̕ԓ�
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_COMMCMD_TALK_Ans( s32 userid, const void* cp_data, u32 size, void* p_work )
{
	WFLBY_SYSTEM*  p_wk = p_work;
	u32 idx;
	const WFLBY_SYSTEM_TALK_DATA* cp_talkdata;

	// �f�[�^�擾
	cp_talkdata = cp_data;

	// �C���f�b�N�X�擾
	idx = DWC_LOBBY_GetUserIDIdx( userid );

	// ��b�����`�F�b�N
	if( p_wk->talk.talk_type == WFLBY_TALK_TYPE_NONE ){
		// BTS:�ʐM No.206	
		// �����ɗ��邱�Ƃ�����悤�ł����A
		// �E�����͉�b��ԂłȂ����߁A��M�������𖳎�
		// ����B
		// �E�f�[�^�𑗐M��������́A�ԓ����A���Ă��Ȃ���
		// �ŁA�^�C���A�E�g�����֑J�ڂ���B
		// �ƁA�Ȃ��Ă���̂ŁA�A�T�[�g�łƂ߂�K�v�͂Ȃ�
		// 
//		GF_ASSERT(0);
		return ;
	}
	
	// ��b���̐l���`�F�b�N
	if( p_wk->talk.talk_idx == idx ){


		// ��b���m�����ꂽ���`�F�b�N
		if( cp_talkdata->data == TRUE ){

			// ���߂Ă��`�F�b�N
			if( WFLBY_SYSTEM_TALK_CheckMsk( &p_wk->talk, idx ) == FALSE ){
				p_wk->talk.talk_first = TRUE;
			}else{
				p_wk->talk.talk_first = FALSE;
			}
			// �����̑I����
//			WFLBY_SYSTEM_TALK_SetMsk( &p_wk->talk, idx );
			p_wk->talk.talk_seq = WFLBY_TALK_SEQ_A_SEL;
		}else{
			// �ؒf��
			WFLBY_SYSTEM_TALK_EndClear( &p_wk->talk );
		}

		// ��M�҂���~
		WFLBY_SYSTEM_TALK_StopRecvWait( &p_wk->talk );
	}else{
//		OS_TPrintf( "talkans idx diff talkidx=%d  recvidx=%d\n", p_wk->talk.talk_idx, idx );
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�f�[�^�̎�M
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_COMMCMD_TALK_Data( s32 userid, const void* cp_data, u32 size, void* p_work )
{
	WFLBY_SYSTEM*  p_wk = p_work;
	u32 idx;
	const WFLBY_SYSTEM_TALK_DATA* cp_talkdata;

	// ��b�����`�F�b�N
	if( p_wk->talk.talk_type == WFLBY_TALK_TYPE_NONE ){
		return ;
	}

	// �f�[�^�擾
	cp_talkdata = cp_data;

	// �C���f�b�N�X�擾
	idx = DWC_LOBBY_GetUserIDIdx( userid );

	// ���肪�����Ă��邩�`�F�b�N
	if( p_wk->talk.talk_idx == idx ){

		// ��M�����V�[�P���X�ɕύX
		if( (cp_talkdata->seq == WFLBY_TALK_SEQ_A_SEL) ||
			(cp_talkdata->seq == WFLBY_TALK_SEQ_B_SEL) ){
			p_wk->talk.talk_seq = cp_talkdata->seq;	// �ؒf�ɂȂ�\��������

			// ���肩��̎�M����M�o�b�t�@�ɐݒ�
			p_wk->talk.recv_data = cp_talkdata->data;

			// ��M�҂���~
			WFLBY_SYSTEM_TALK_StopRecvWait( &p_wk->talk );
		}else{

			// �ً}��~
			WFLBY_SYSTEM_TALK_ExEndClear( &p_wk->talk );
		}

	}else{

		// ���肪���Ȃ��Ȃ��ĂȂ����`�F�b�N
		if( idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
			// �ً}�ؒf��
			WFLBY_SYSTEM_TALK_ExEndClear( &p_wk->talk );
		}

//		OS_TPrintf( "talkdata idx diff talkidx=%d  recvidx=%d\n", p_wk->talk.talk_idx, idx );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�I��
 *
 *	@param	userid		���[�U
 *	@param	cp_data		�f�[�^
 *	@param	size		�T�C�Y
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_COMMCMD_TALK_End( s32 userid, const void* cp_data, u32 size, void* p_work )
{
	WFLBY_SYSTEM*  p_wk = p_work;
	u32 idx;

	// ��b����
	if( p_wk->talk.talk_seq == WFLBY_TALK_SEQ_NONE ){
		return ;
	}

	// ��b���Ă�l�ƈꏏ��
	idx = DWC_LOBBY_GetUserIDIdx( userid );
	if( p_wk->talk.talk_idx == idx ){
//		OS_TPrintf( "talk end\n" );
		// ��b�I��
		WFLBY_SYSTEM_TALK_EndClear( &p_wk->talk );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�f�[�^	��M
 *
 *	@param	userid		���[�UID
 *	@param	cp_data		�f�[�^
 *	@param	size		�T�C�Y
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_COMMCMD_TOPIC_Data( s32 userid, const void* cp_data, u32 size, void* p_work )
{
	WFLBY_SYSTEM*		p_wk		= p_work;
	const WFLBY_TOPIC*	cp_topic	= cp_data;
	static void (* const  pFunc[])( WFLBY_SYSTEM* p_wk, const WFLBY_TOPIC* cp_topic ) = {
		WFLBY_SYSTEM_TOPIC_SetConnect,
		WFLBY_SYSTEM_TOPIC_SetItem,
		WFLBY_SYSTEM_TOPIC_SetMinigame,
		WFLBY_SYSTEM_TOPIC_SetFootBoard,
		WFLBY_SYSTEM_TOPIC_SetWldTimer,
		WFLBY_SYSTEM_TOPIC_SetNews,
		WFLBY_SYSTEM_TOPIC_SetDummy,
		WFLBY_SYSTEM_TOPIC_SetDummy,
		WFLBY_SYSTEM_TOPIC_SetMgResult,
	};

	// �g�s�b�N�̒ǉ�
	pFunc[ cp_topic->topic_type ]( p_wk, cp_topic );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�f�[�^��M
 *
 *	@param	userid		���[�UID
 *	@param	cp_data		�f�[�^
 *	@param	size		�T�C�Y
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_COMMCMD_GADGET_Data( s32 userid, const void* cp_data, u32 size, void* p_work )
{
	WFLBY_SYSTEM* p_wk		= p_work;
	u32 idx;

	// ���[�U��IDX����M
	idx = DWC_LOBBY_GetUserIDIdx( userid );
	if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
		
		p_wk->gadget.gadget[idx] = TRUE;
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g���	��M
 *
 *	@param	userid	���[�U�h�c
 *	@param	cp_data	�f�[�^
 *	@param	size	�T�C�Y
 *	@param	p_work	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_COMMCMD_FLOAT_On( s32 userid, const void* cp_data, u32 size, void* p_work )
{
	WFLBY_SYSTEM* p_wk		= p_work;
	u32 idx;
	u32 result;
	int i;

	// ���[�U��IDX����M
	idx = DWC_LOBBY_GetUserIDIdx( userid );
	if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
		// �ǂ����ɐ�΂̂���
		for( i=0; i<WFLBY_FLOAT_STATION_NUM; i++ ){
			result = WFLBY_SYSTEM_FLOAT_SetReserve( &p_wk->floatdata, idx, i );

			if( result != DWC_LOBBY_USERIDTBL_IDX_NONE ){
				break;	// �o�^����
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�A�j��	��M
 *
 *	@param	userid	���[�U�h�c
 *	@param	cp_data	�f�[�^
 *	@param	size	�T�C�Y
 *	@param	p_work	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_COMMCMD_FLOAT_Anm( s32 userid, const void* cp_data, u32 size, void* p_work )
{
	WFLBY_SYSTEM* p_wk		= p_work;
	u32 idx;

	// ���[�U��IDX����M
	idx = DWC_LOBBY_GetUserIDIdx( userid );
	if( idx != DWC_LOBBY_USERIDTBL_IDX_NONE ){
		WFLBY_SYSTEM_FLOAT_SetAnm( &p_wk->floatdata, idx );
	}
}






//----------------------------------------------------------------------------
/**
 *	@brief	�Ō�ɂ������ƂɂƂ��Ă����X�e�[�^�X���`�F�b�N
 *
 *	@param	status	���
 *
 *	@retval		TRUE	�Ƃ��Ă���
 *	@retval		FALSE	�Ƃ��Ă����Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_PROFILE_CheckLastTmpStatus( WFLBY_STATUS_TYPE status )
{
	BOOL result;
	
	switch( status ){

	case WFLBY_STATUS_BALLSLOW:		// �ʂ���
	case WFLBY_STATUS_BALANCEBALL:	// �ʏ��
	case WFLBY_STATUS_BALLOON:		// ���D���
	case WFLBY_STATUS_FOOTBOAD00:	// ���Ճ{�[�h
	case WFLBY_STATUS_FOOTBOAD01:	// ���Ճ{�[�h
	case WFLBY_STATUS_WORLDTIMER:	// ���E���v
	case WFLBY_STATUS_TOPIC:			// �j���[�X
	case WFLBY_STATUS_TALK:			// ��b
	case WFLBY_STATUS_FLOAT:			// �t���[�g
		result = TRUE;
		break;

	default:
		result =FALSE;
		break;
	}

	return result;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�I���V�X�e��	������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_GADGETRATE_Init( WFLBY_SYSTEM_GADGETRATE* p_wk )
{
	int i;
	
	memset( p_wk, 0, sizeof(WFLBY_SYSTEM_GADGETRATE) );
	
	// �������[�g��ݒ�
	for( i=0; i<WFLBY_ITEM_GROUPNUM; i++ ){
		p_wk->rate[i] = WFLBY_GADGETRATE_DEF;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�I���V�X�e��	���[�g�v�Z
 *
 *	@param	p_wk	���[�N
 *	@param	item	�N���������Ă���K�W�F�b�g
 *	@param	plno	���̃K�W�F�b�g����������l
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_GADGETRATE_CalcRate( WFLBY_SYSTEM_GADGETRATE* p_wk, WFLBY_ITEMTYPE item, u32 plno )
{
	u32 item_group;
	int i;
	GF_ASSERT( plno < WFLBY_PLAYER_MAX );
	GF_ASSERT( item < WFLBY_ITEM_NUM );
	
	
	// �܂����̐l�̃K�W�F�b�g�f�[�^�����[�g�ɔ��f����Ă��Ȃ���΁A
	// ���[�g�v�Z����
	if( p_wk->gadget_recv[plno] == FALSE ){
		item_group = item / WFLBY_ITEM_DANKAI;
		p_wk->rate[ item_group ] /= WFLBY_GADGETRATE_DIV;
		p_wk->gadget_recv[plno] = TRUE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�I���V�X�e��	���[�g���g���ă����_���ɑI�������A�C�e���擾
 *
 *	@param	cp_wk	���[�N
 *	
 *	@return	�K�W�F�b�g
 */
//-----------------------------------------------------------------------------
static WFLBY_ITEMTYPE WFLBY_SYSTEM_GADGETRATE_GetRandItem( const WFLBY_SYSTEM_GADGETRATE* cp_wk )
{
	int i;
	u32 rate_max;
	u32 rate_start;
	u32 rand;
	
	// ���̃��[�g�̒l�����v�����l���擾
	rate_max = 0;
	for( i=0; i<WFLBY_ITEM_GROUPNUM; i++ ){
		rate_max += cp_wk->rate[i];
	}

	rand = gf_mtRand() % rate_max;
	
	// �����̒l���K�W�F�b�g���[�g�̃O���[�v�̒l�͈͓̔��Ȃ�
	// ���̃K�W�F�b�g�̃��x���P�̒l��Ԃ�
	rate_start = 0;
	for( i=0; i<WFLBY_ITEM_GROUPNUM; i++ ){
		if( (rate_start <= rand) && ((rate_start+cp_wk->rate[i]) > rand) ){
			return i*WFLBY_ITEM_DANKAI;
		}
		rate_start += cp_wk->rate[i];
	}

	// �����ɗ��邱�Ƃ͂Ȃ�
	GF_ASSERT(0);
	return WFLBY_ITEM_BELL00;
}



//----------------------------------------------------------------------------
/**
 *	@brief	VIP�t���O	�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 *	@param	vip			VIP�t���O
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_VIPFLAG_Set( WFLBY_VIPFLAG* p_wk, u32 idx, BOOL vip )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );
	if( idx < WFLBY_PLAYER_MAX ){
		p_wk->vip[idx] = vip;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP�t���O	�擾
 *
 *	@param	cp_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 *
 *	@retval	TRUE	VIP��
 *	@retval	FALSE	VIP����Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_VIPFLAG_Get( const WFLBY_VIPFLAG* cp_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );
	if( idx < WFLBY_PLAYER_MAX ){
		return cp_wk->vip[idx];
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�����t�o�b�t�@�̏�����
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_AIKOTOBABUFF_Init( WFLBY_SYSTEM_AIKOTOBA* p_wk, u32 heapID )
{
	MI_CpuClear8( p_wk, sizeof(WFLBY_SYSTEM_AIKOTOBA) );

	// �ȈՉ�b�P��e�[�u���擾
	p_wk->p_wordtbl = PMSW_AIKOTOBATBL_Init( heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����t�o�b�t�@�̔j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_AIKOTOBABUFF_Exit( WFLBY_SYSTEM_AIKOTOBA* p_wk )
{
	// �ȈՉ�b�P��e�[�u���擾
	PMSW_AIKOTOBATBL_Delete( p_wk->p_wordtbl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����t�o�b�t�@�ɏ���ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 *	@param	aikotoba�����t�L���t���O
 *	@param	number	�����t�����p�L�[�l
 *
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_AIKOTOBABUFF_SetData( WFLBY_SYSTEM_AIKOTOBA* p_wk, u32 idx, BOOL aikotoba, u32 number )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );

	if( idx < WFLBY_PLAYER_MAX ){
		p_wk->aikotoba[idx] = aikotoba;

		if( aikotoba == TRUE ){
			WFLBY_SYSTEM_AIKOTOBA_Make( p_wk->p_wordtbl, number, 
					&p_wk->aikotoba_data[idx] );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����t�o�b�t�@���獇���t�����邩�`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_AIKOTOBABUFF_GetFlag( const WFLBY_SYSTEM_AIKOTOBA* cp_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );
	if( idx < WFLBY_PLAYER_MAX ){
		return cp_wk->aikotoba[idx];
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����t�o�b�t�@���獇���t�擾
 *
 *	@param	cp_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 *
 *	@return	�����t�f�[�^
 */
//-----------------------------------------------------------------------------
static const WFLBY_AIKOTOBA_DATA* WFLBY_SYSTEM_AIKOTOBABUFF_GetData( const WFLBY_SYSTEM_AIKOTOBA* cp_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );
	if( idx < WFLBY_PLAYER_MAX ){
		if( cp_wk->aikotoba[idx] ){
			return &cp_wk->aikotoba_data[idx];
		}
	}
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������Ƃ�	����
 *
 *	@param	p_wordtbl		�P��e�[�u��
 *	@param	number			�L�[�l
 *	@param	p_aikotoba		�����t�i�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_AIKOTOBA_Make( PMSW_AIKOTOBA_TBL* p_wordtbl, u32 number, WFLBY_AIKOTOBA_DATA* p_aikotoba )
{
	WFLBY_SYSTEM_AIKOTOBA_KEY keydata;
	u32 tblnum;
	u32 tblidx;

	// �P��e�[�u�������擾
	tblnum = PMSW_AIKOTOBATBL_GetTblNum( p_wordtbl );

	// �L�[�̒l����
	// A�lB�lC�lD�l���쐬
	keydata.number = number;


	{
		OS_TPrintf( "�e�[�u������:%d\n", tblnum );
		OS_TPrintf( "key number:0x%x\n", number );
			
		OS_TPrintf( "A�l:%d\n", keydata.key[ WFLBY_AIKOTOBA_KEY_A ] );
		OS_TPrintf( "B�l:%d\n", keydata.key[ WFLBY_AIKOTOBA_KEY_B ] );
		OS_TPrintf( "C�l:%d\n", keydata.key[ WFLBY_AIKOTOBA_KEY_C ] );
		OS_TPrintf( "D�l:%d\n", keydata.key[ WFLBY_AIKOTOBA_KEY_D ] );


		OS_TPrintf( "�P��A:%d\n", (keydata.key[WFLBY_AIKOTOBA_KEY_D] + keydata.key[WFLBY_AIKOTOBA_KEY_A]) % tblnum );
		OS_TPrintf( "�P��B:%d\n", (keydata.key[WFLBY_AIKOTOBA_KEY_A] + keydata.key[WFLBY_AIKOTOBA_KEY_B]) % tblnum );
		OS_TPrintf( "�P��C:%d\n", (keydata.key[WFLBY_AIKOTOBA_KEY_B] + keydata.key[WFLBY_AIKOTOBA_KEY_C]) % tblnum );
		OS_TPrintf( "�P��D:%d\n", (keydata.key[WFLBY_AIKOTOBA_KEY_C] + keydata.key[WFLBY_AIKOTOBA_KEY_D]) % tblnum );

		OS_TPrintf( "\n" );
	}

	// �e���[�h��I��
	tblidx				= (keydata.key[WFLBY_AIKOTOBA_KEY_D] + keydata.key[WFLBY_AIKOTOBA_KEY_A]) % tblnum;
	p_aikotoba->word[0] = PMSW_AIKOTOBATBL_GetTblData( p_wordtbl, tblidx );

	tblidx				= (keydata.key[WFLBY_AIKOTOBA_KEY_A] + keydata.key[WFLBY_AIKOTOBA_KEY_B]) % tblnum;
	p_aikotoba->word[1] = PMSW_AIKOTOBATBL_GetTblData( p_wordtbl, tblidx );

	tblidx				= (keydata.key[WFLBY_AIKOTOBA_KEY_B] + keydata.key[WFLBY_AIKOTOBA_KEY_C]) % tblnum;
	p_aikotoba->word[2] = PMSW_AIKOTOBATBL_GetTblData( p_wordtbl, tblidx );

	tblidx				= (keydata.key[WFLBY_AIKOTOBA_KEY_C] + keydata.key[WFLBY_AIKOTOBA_KEY_D]) % tblnum;
	p_aikotoba->word[3] = PMSW_AIKOTOBATBL_GetTblData( p_wordtbl, tblidx );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΃f�[�^���[�N	������
 *
 *	@param	p_wk			���[�N
 *	@param	fire_start		�ԉΊJ�n
 *	@param	fire_end		�ԉΏI��
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FIREDATA_Init( WFLBY_FIRE_DATA* p_wk, u32 fire_start, u32 fire_end )
{
	p_wk->start		= FALSE;
	p_wk->count		= 0;
	p_wk->count_max	= (fire_end - fire_start) * 30;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΃f�[�^���[�N	����J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FIREDATA_Start( WFLBY_FIRE_DATA* p_wk )
{
	p_wk->start		= TRUE;
	p_wk->count		= 0;

	// �ԉΊJ�n���Ԏ擾
	DWC_LOBBY_GetTime( &p_wk->start_time );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΃f�[�^���[�N	���상�C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_FIREDATA_Main( WFLBY_FIRE_DATA* p_wk )
{
	s64 now_time;
	s64 diff_time;
	
	if( p_wk->start ){
		
		// ���ۂ̎��Ԃł̌o�ߊԊu���count�̌o�߂��������Ƃ���
		// ���Ԃ̂ق��ɍ��킹��
		DWC_LOBBY_GetTime( &now_time );
		diff_time = now_time - p_wk->start_time;
		if( p_wk->count < (diff_time*30) ){
			p_wk->count = (diff_time*30);
		}

		if( p_wk->count < p_wk->count_max ){
			p_wk->count ++;

			// �J�E���^�̒l���瓮��^�C�v��ύX
			{
				u32 par;
				u32 par_num;
				int i;
				
				par = (p_wk->count * WFLBY_FIRE_MOVE_PAR_MAX) / p_wk->count_max;
				par_num = 0;
				for( i=0; i<NELEMS(sc_WFLBY_FIRE_MOVE_DATA); i++ ){
					par_num += sc_WFLBY_FIRE_MOVE_DATA[i].move_par_num;
					if( par_num >= par ){
						if( p_wk->move_type != sc_WFLBY_FIRE_MOVE_DATA[i].move_type ){
							p_wk->move_type		= sc_WFLBY_FIRE_MOVE_DATA[i].move_type;	
							p_wk->move_count	= 0;
//							OS_TPrintf( "par_num %d  par %d\n", par_num, par );
//							OS_TPrintf( "movetype %d\n", p_wk->move_type );
						}
						break;
					}
				}
			}

		}else{
			
			// �����܂ŗ����炸����CLIMAX
			if( p_wk->move_type != WFLBY_FIRE_CLIMAX ){
				p_wk->move_type		= WFLBY_FIRE_CLIMAX;
				p_wk->move_count	= 0;
			}

			// CLIMAX�̌o�ߎ��Ԃ�WFLBY_FIRE_END_COUNT�ɂȂ����炨���
			if( (p_wk->move_count >= WFLBY_FIRE_END_COUNT) ){
				p_wk->move_type	= WFLBY_FIRE_END;
				p_wk->start		= FALSE;
			}
		}

		p_wk->move_count ++;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	movetype�ɖ��Ă���̌o�ߎ��Ԃ��擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�o�ߎ���
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_SYSTEM_FIREDATA_GetCount( const WFLBY_FIRE_DATA* cp_wk )
{
	return cp_wk->move_count;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̉ԉΓ���^�C�v���擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	����^�C�v
 */
//-----------------------------------------------------------------------------
static WFLBY_FIRE_TYPE WFLBY_SYSTEM_FIREDATA_GetType( const WFLBY_FIRE_DATA* cp_wk )
{
	return cp_wk->move_type;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�Ō�ɂ������ƃL���[		������
 *
 *	@param	p_wk	���[�N
 *	@param	cp_save	�Z�[�u�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_LASTACTQ_Init( WFLBY_LASTACTION_CUE* p_wk, const SAVEDATA* cp_save )
{
	int i;

	for( i=0; i<WFLBY_LASTACT_Q_BUFFNUM; i++ ){
		p_wk->cue[i] = WFLBY_LASTACTION_MAX;
		p_wk->cue_userid[i] = DWC_LOBBY_INVALID_USER_ID;
	}
	p_wk->top	= 0;
	p_wk->tail	= 0;

	p_wk->cp_save = cp_save;

	WFLBY_SYSTEM_LASTACTQ_SetCrc( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ō�ɂ������ƃL���[	�v�b�V��
 *
 *	@param	p_wk	���[�N
 *	@param	data	�f�[�^
 *	@param	userid	���[�UID
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_LASTACTQ_Push( WFLBY_LASTACTION_CUE* p_wk, WFLBY_LASTACTION_TYPE data, s32 userid )
{
	// �`�F�b�N
	WFLBY_SYSTEM_LASTACTQ_CheckCrc( p_wk );
	
	// ����+1���擪�̎����t
	if( ((p_wk->tail + 1)%WFLBY_LASTACT_Q_BUFFNUM) == p_wk->top ){
		// ���t
		// 1�f�[�^���|�b�v���ăf�[�^�i�[
		WFLBY_SYSTEM_LASTACTQ_Pop( p_wk );
	}

	// �f�[�^�ݒ�
	p_wk->cue[ p_wk->tail ]			= data;
	p_wk->cue_userid[ p_wk->tail ]	= userid;

	// �����ʒu�𓮂���
	p_wk->tail = (p_wk->tail + 1)%WFLBY_LASTACT_Q_BUFFNUM;

	// �ݒ�
	WFLBY_SYSTEM_LASTACTQ_SetCrc( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID��u��������
 *
 *	@param	p_wk			���[�N
 *	@param	before_id		�O
 *	@param	after_id		��
 *
 *	@retval	TRUE	�u��������
 *	@retval	FALSE	�u�������ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_LASTACTQ_ResetUserID( WFLBY_LASTACTION_CUE* p_wk, s32 before_id, s32 after_id )
{
	int i;
	BOOL ret = FALSE;
	
	// �`�F�b�N
	WFLBY_SYSTEM_LASTACTQ_CheckCrc( p_wk );


	// ���[�UID�̕ύX
	for( i=0; i<WFLBY_LASTACT_Q_BUFFNUM; i++ ){
		if( p_wk->cue_userid[i] == before_id ){
			p_wk->cue_userid[i] = after_id;
			ret = TRUE;
		}
	}

	// �ݒ�
	WFLBY_SYSTEM_LASTACTQ_SetCrc( p_wk );

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ō�ɂ������ƃL���[	�|�b�v
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static WFLBY_LASTACTION_TYPE WFLBY_SYSTEM_LASTACTQ_Pop( WFLBY_LASTACTION_CUE* p_wk )
{
	u8 data;

	// �`�F�b�N
	WFLBY_SYSTEM_LASTACTQ_CheckCrc( p_wk );
	
	// �擪=����	�f�[�^���Ȃ�
	if( p_wk->tail == p_wk->top ){
		return WFLBY_LASTACTION_MAX;
	}

	data = p_wk->cue[ p_wk->top ];
	p_wk->cue[ p_wk->top ] = WFLBY_LASTACTION_MAX;	// �f�[�^�j��
	p_wk->cue_userid[ p_wk->tail ]	= DWC_LOBBY_INVALID_USER_ID;

	// �擪�ʒu�𓮂���
	p_wk->top = (p_wk->top + 1)%WFLBY_LASTACT_Q_BUFFNUM;

	// �ݒ�
	WFLBY_SYSTEM_LASTACTQ_SetCrc( p_wk );

	return data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ō�ɂ������ƃL���[	�f�[�^�擾	�ŐV�̃f�[�^�[���Â��f�[�^��
 *
 *	@param	p_wk		���[�N	
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static WFLBY_LASTACTION_TYPE WFLBY_SYSTEM_LASTACTQ_GetData( WFLBY_LASTACTION_CUE* p_wk, u32 idx )
{
	s32 dataidx;
	GF_ASSERT( idx < WFLBY_LASTACT_BUFFNUM );

	// �`�F�b�N
	WFLBY_SYSTEM_LASTACTQ_CheckCrc( p_wk );

	// 1�𑫂�
	idx ++;
	
	dataidx = p_wk->tail - idx;
	if( dataidx < 0 ){
		dataidx += WFLBY_LASTACT_Q_BUFFNUM;
	}
	
	return p_wk->cue[ dataidx ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ō�ɂ������ƃL���[	userid�擾	�ŐV�̃f�[�^�[���Â��f�[�^��
 *
 *	@param	p_wk		���[�N	
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static s32 WFLBY_SYSTEM_LASTACTQ_GetUserID( WFLBY_LASTACTION_CUE* p_wk, u32 idx )
{
	s32 dataidx;
	GF_ASSERT( idx < WFLBY_LASTACT_BUFFNUM );

	// CRC�`�F�b�N
	WFLBY_SYSTEM_LASTACTQ_CheckCrc( p_wk );

	// 1�𑫂�
	idx ++;
	
	dataidx = p_wk->tail - idx;
	if( dataidx < 0 ){
		dataidx += WFLBY_LASTACT_Q_BUFFNUM;
	}
	
	return p_wk->cue_userid[ dataidx ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	CRC�ݒ�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_LASTACTQ_SetCrc( WFLBY_LASTACTION_CUE* p_wk )
{
	p_wk->crc_check = SaveData_CalcCRC( p_wk->cp_save, p_wk, WFLBY_LASTACTION_CUE_CRC_DATASIZE );
//	OS_TPrintf( "lastactQ crc set %d\n", p_wk->crc_check );

}

//----------------------------------------------------------------------------
/**
 *	@brief	CRC�`�F�b�N
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_LASTACTQ_CheckCrc( WFLBY_LASTACTION_CUE* p_wk )
{
	u32 check_crc;

	// CRC�`�F�b�N
	check_crc = SaveData_CalcCRC( p_wk->cp_save, p_wk, WFLBY_LASTACTION_CUE_CRC_DATASIZE );
//	OS_TPrintf( "lastactQ crc check %d==%d\n", check_crc, p_wk->crc_check );
	if( check_crc != p_wk->crc_check ){
		GF_ASSERT_MSG( 0, "lobby lastactQ crc error\n" );
		p_wk->error = TRUE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	CRC�̃G���[��Ԃ��擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�G���[���
 *	@retval	FLASE	����
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_SYSTEM_LASTACTQ_GetCrcError( const WFLBY_LASTACTION_CUE* cp_wk )
{
	return cp_wk->error;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�^�C�v���烆�j�I���L�����N�^�i���o�[���擾
 *
 *	@param	tr_type		�g���[�i�^�C�v
 *
 *	@retval	0�`15	���j�I���L�����N�^�i���o�[
 *	@retval	WFLBY_SYSTEMUNICHAR_NUM	�s���ȃL�����N�^�i���o�[
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_TRTYPE_UNIONCHARNO_Get( u32 tr_type )
{
	int i;
	
	for( i=0; i<WFLBY_SYSTEMUNICHAR_NUM; i++ ){
		if( sc_WFLBY_SYSTEM_TRTYPE_SEX[i].tr_type == tr_type ){
			return i;
		}
	}

	return WFLBY_SYSTEMUNICHAR_NUM;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�I���L�����N�^�i���o�[����g���[�i�^�C�v���擾
 *
 *	@param	union_char_no ���j�I���L�����N�^�i���o�[
 *
 *	@retval	OBJCODEMAX		�s���ȃf�[�^
 *	@retval	���̑�			�g���[�iID
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_UNIONCHARNO_TRTYPE_Get( u32 union_char_no )
{
	if( union_char_no < WFLBY_SYSTEMUNICHAR_NUM ){
		return sc_WFLBY_SYSTEM_TRTYPE_SEX[ union_char_no ].tr_type;
	}
	return OBJCODEMAX;
}

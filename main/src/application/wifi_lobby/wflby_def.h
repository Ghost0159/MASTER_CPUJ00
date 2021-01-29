//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_def.h
 *	@brief		WiFi���r�[	�ʐM�\����
 *	@author		tomoya	takahashi	
 *	@data		2007.09.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_DEF_H__
#define __WFLBY_DEF_H__

#include <nitro.h>
#include "system/buflen.h"
#include "system/pms_word.h"

#include "battle/battle_common.h"

#include "msgdata/msg_wifi_place_msg_world.h"


//-----------------------------------------------------------------------------
/**
 *			�S�̃f�o�b�N
 */
//-----------------------------------------------------------------------------
#ifdef PM_DEBUG
//#define WFLBY_DEBUG_ROOM_WLDTIMER_AUTO	// �I�[�g�Ő��E���v�ɓ�������o����
//#define WFLBY_DEBUG_ROOM_MINIGAME_AUTO	// �I�[�g�Ń~�j�Q�[���o�����������
#endif

#ifdef WFLBY_DEBUG_ROOM_WLDTIMER_AUTO
extern BOOL WFLBY_DEBUG_ROOM_WFLBY_TIMER_AUTO;
#endif

#ifdef WFLBY_DEBUG_ROOM_MINIGAME_AUTO
extern BOOL WFLBY_DEBUG_ROOM_MINIGAME_AUTO_FLAG;
#endif


//-----------------------------------------------------------------------------
/**
 *				�S����
 */
//-----------------------------------------------------------------------------

//-------------------------------------
/// �����I���C�x���g���畔�����I��������܂ł̎���
//=====================================
#define WFLBY_END_OVERTIME		( 30*30 )	// 30�b
#define WFLBY_END_BGMFADEOUT	( 28*30 )	// �̂���28�b�ɂȂ�����BGM���t�F�[�h�A�E�g������

//-------------------------------------
/// �ʏ�̃C�x���g����M���Ă��甽�f������܂ł̎���
//=====================================
#define WFLBY_EVENT_STARTTIME	( 10*30 )	// 10�b

//-------------------------------------
///	1�����̐l��
//=====================================
#define WFLBY_PLAYER_MAX	(20)

//-------------------------------------
///	�}�b�v�z�u�l���̃v���C���[NO
//=====================================
#define WFLBY_MAPPERSON_ID			( 0xff )	// �J�E���^�[�ɂ���l
#define WFLBY_MAPPERSON_PIERROT_ID	( 0xfe )	// �s�G���i�s�G���͐l�Ɠ����悤�ɘb�������邱�Ƃ��o����̂�playid�̃`�F�b�N�����Ă��������j

//-------------------------------------
///	�~�j�Q�[���l��
//=====================================
#define WFLBY_MINIGAME_MAX	(4)

//-------------------------------------
///	�~�j�Q�[���eBCANCEL	�Q�����b�N����
//=====================================
#define WFLBY_MINIGAME_LOCK_TIME	(30*30)


//-------------------------------------
///	���Ճ{�[�h�l��
//=====================================
#define WFLBY_FOOT_MAX	(8)

//-------------------------------------
///	1�����ɏo���t���[�g�̍ő吔
//=====================================
#define WFLBY_FLOAT_MAX		(9)

//-------------------------------------
///	1�t���[�g�̒��
//=====================================
#define WFLBY_FLOAT_ON_NUM		(3)

//-------------------------------------
///		���E�f�[�^ ������ID
//=====================================
#define WFLBY_WLDDATA_NATION_NONE		( country000 )	// �����ȍ�ID

//-------------------------------------
///	1�l�Ɖ�b�ł���ő吔
//=====================================
#define WFLBY_TALK_MAX		( 6 )

//-------------------------------------
///	��b�����҂�����
//=====================================
#define WFLBY_TALK_RECV_WAIT		( 900 )		// 30�b
#define WFLBY_TALK_RECV_HALF_WAIT	( 450 )		// 15�b	���ԃ��b�Z�[�W�̕\��

//-------------------------------------
///	�s���Ȍ���R�[�h�̂Ƃ�
//=====================================
#define WFLBY_REGIONCODE_ERR	( LANG_ENGLISH )	// �p���̐l�Ƃ��Ĉ����܂�

//-------------------------------------
///	�s�������Ƃ̗����@�ێ���
//=====================================
#define WFLBY_LASTACT_BUFFNUM	( 12 )

//-------------------------------------
///	���[�U���v���t�B�[���p�ɑI�����鑮���^�C�v
//=====================================
#define WFLBY_SELECT_TYPENUM		(2)								// �I��
typedef enum{
	WFLBY_POKEWAZA_TYPE_NONE,	// �I�΂Ȃ�
	WFLBY_POKEWAZA_TYPE_NORMAL,
	WFLBY_POKEWAZA_TYPE_BATTLE,
	WFLBY_POKEWAZA_TYPE_HIKOU,
	WFLBY_POKEWAZA_TYPE_POISON,
	WFLBY_POKEWAZA_TYPE_JIMEN,
	WFLBY_POKEWAZA_TYPE_IWA,
	WFLBY_POKEWAZA_TYPE_MUSHI,
	WFLBY_POKEWAZA_TYPE_GHOST,
	WFLBY_POKEWAZA_TYPE_METAL,
	WFLBY_POKEWAZA_TYPE_FIRE,
	WFLBY_POKEWAZA_TYPE_WATER,
	WFLBY_POKEWAZA_TYPE_KUSA,
	WFLBY_POKEWAZA_TYPE_ELECTRIC,
	WFLBY_POKEWAZA_TYPE_SP,
	WFLBY_POKEWAZA_TYPE_KOORI,
	WFLBY_POKEWAZA_TYPE_DRAGON,
	WFLBY_POKEWAZA_TYPE_AKU,
	WFLBY_POKEWAZA_TYPE_NUM,
} WFLBY_POKEWAZA_TYPE;


//-------------------------------------
///	�~�j�Q�[���^�C�v
//=====================================
typedef enum{
	WFLBY_GAME_BALLSLOW,	// �ʓ���
	WFLBY_GAME_BALANCEBALL,	// �ʏ��
	WFLBY_GAME_BALLOON,		// �ӂ�������
	WFLBY_GAME_FOOTWHITE,	// ���Ճ{�[�h�@��
	WFLBY_GAME_FOOTBLACK,	// ���Ճ{�[�h�@��
	WFLBY_GAME_WLDTIMER,	// ���E���v
	WFLBY_GAME_NEWS,		// ���r�[�j���[�X
	WFLBY_GAME_TALK,		// ������ׂ�
	WFLBY_GAME_ITEM,		// �K�W�F�b�g
	WFLBY_GAME_ITEMCHG,		// �K�W�F�b�g����

	WFLBY_GAME_UNKNOWN,		// �킩��Ȃ�

	WFLBY_GAME_NUM,			// ��

	// P2P�ʐM����~�j�Q�[���̐�
	WFLBY_GAME_P2PGAME_NUM = WFLBY_GAME_BALLOON + 1,
} WFLBY_GAMETYPE;

//-------------------------------------
///	���ԃC�x���g�^�C�v
//=====================================
typedef enum{
	WFLBY_EVENT_NEON,		// �l�I��
	WFLBY_EVENT_FIRE,		// �t�@�C�A�[
	WFLBY_EVENT_PARADE,		// �p���[�h
	WFLBY_EVENT_LIGHT_OFF,	// ���C�g�I�t
	WFLBY_EVENT_CLOSE,		// �N���[�Y

	WFLBY_EVENT_UNKNOWN,	// �킩��Ȃ�
	
	WFLBY_EVENT_NUM,		// ��
} WFLBY_EVENTTYPE;

//-------------------------------------
///	�^�C���C�x���g	���b�Z�[�W�萔
//=====================================
typedef enum{
	WFLBY_EVENT_GMM_NEON,		// �l�I��
	WFLBY_EVENT_GMM_FIRE,		// �t�@�C�A�[
	WFLBY_EVENT_GMM_PARADE,		// �p���[�h
	
	WFLBY_EVENT_GMM_NUM,		// ��
} WFLBY_EVENTGMM_TYPE;

//-------------------------------------
///	�A�C�e���^�C�v
//=====================================
enum{
	WFLBY_ITEM_BELL00,		// �x����
	WFLBY_ITEM_BELL01,		// �x����
	WFLBY_ITEM_BELL02,		// �x����
	WFLBY_ITEM_DRAM00,		// �h������
	WFLBY_ITEM_DRAM01,		// �h������
	WFLBY_ITEM_DRAM02,		// �h������
	WFLBY_ITEM_CYMBALS00,	// �V���o����
	WFLBY_ITEM_CYMBALS01,	// �V���o����
	WFLBY_ITEM_CYMBALS02,	// �V���o����
	WFLBY_ITEM_RIPPRU00,	// ���b�v����
	WFLBY_ITEM_RIPPRU01,	// ���b�v����
	WFLBY_ITEM_RIPPRU02,	// ���b�v����
	WFLBY_ITEM_SIGNAL00,	// �V�O�i����
	WFLBY_ITEM_SIGNAL01,	// �V�O�i����
	WFLBY_ITEM_SIGNAL02,	// �V�O�i����
	WFLBY_ITEM_WINK00,		// �E�B���N��
	WFLBY_ITEM_WINK01,		// �E�B���N��
	WFLBY_ITEM_WINK02,		// �E�B���N��
	WFLBY_ITEM_CRACKER00,	// �N���b�J�[��
	WFLBY_ITEM_CRACKER01,	// �N���b�J�[��
	WFLBY_ITEM_CRACKER02,	// �N���b�J�[��
	WFLBY_ITEM_SPARK00,		// �X�p�[�N����
	WFLBY_ITEM_SPARK01,		// �X�p�[�N����
	WFLBY_ITEM_SPARK02,		// �X�p�[�N����
	WFLBY_ITEM_BALLOON00,	// �o���[����
	WFLBY_ITEM_BALLOON01,	// �o���[����
	WFLBY_ITEM_BALLOON02,	// �o���[����
	WFLBY_ITEM_NUM,		// ��

	// �K�W�F�b�g�����l	�i�܂���������������s���ȂƂ��j
	WFLBY_ITEM_INIT = 0xff,

	// �ЂƂ̎�ނ̃A�C�e���ɂ���i�K��
	WFLBY_ITEM_DANKAI = 3,
} ;
// �K�W�F�b�g��GROUP�̎��
#define WFLBY_ITEM_GROUPNUM (WFLBY_ITEM_NUM/WFLBY_ITEM_DANKAI) 
typedef u32 WFLBY_ITEMTYPE;	// �A�C�e���p�^�錾

//-------------------------------------
///	�g�s�b�N�X�f�[�^
//=====================================
typedef enum {	// �g�s�b�N�X�^�C�v
	NEWS_TOPICTYPE_CONNECT,			// ���A
	NEWS_TOPICTYPE_ITEM,			// �K�W�F�b�g�̌���
	NEWS_TOPICTYPE_MINIGAME,		// �~�j�Q�[��
	NEWS_TOPICTYPE_FOOTBOARD,		// �������ƃ{�[�h
	NEWS_TOPICTYPE_WORLDTIMER,		// ���E���v
	NEWS_TOPICTYPE_LOBBYNEWS,		// ���r�[�j���[�X
	NEWS_TOPICTYPE_TIMEEVENT,		// ���ԃC�x���g
	NEWS_TOPICTYPE_VIPIN,			// VIP�����������Ƃ��̃j���[�X
	NEWS_TOPICTYPE_MGRESULT,		// �~�j�Q�[������

	NEWS_TOPICTYPE_NUM,		// �g�s�b�N�^�C�v��
} NEWS_TOPICTYPE;

//-------------------------------------
///	�����^�C�v
//=====================================
typedef enum{
	WFLBY_ROOM_FIRE,			// ��
	WFLBY_ROOM_WARTER,			// ��
	WFLBY_ROOM_ELEC,			// �d�C
	WFLBY_ROOM_GRASS,			// ��
	WFLBY_ROOM_SPECIAL,			// ����
	WFLBY_ROOM_NUM,				// ������
} WFLBY_ROOM_TYPE;

//-------------------------------------
///	�V�[�Y���^�C�v
//=====================================
typedef enum {
	WFLBY_SEASON_NONE,
	WFLBY_SEASON_SPRING,
	WFLBY_SEASON_SUMMER,
	WFLBY_SEASON_FALL,
	WFLBY_SEASON_WINTER,
	WFLBY_SEASON_NUM,
} WFLBY_SEASON_TYPE;

//-------------------------------------
///	�X�e�[�^�X
//=====================================
typedef enum {
	WFLBY_STATUS_NONE,
	WFLBY_STATUS_LOGIN,			// ���O�C�����(�ʏ�)
	WFLBY_STATUS_BALLSLOW,		// �ʂ���
	WFLBY_STATUS_BALANCEBALL,	// �ʏ��
	WFLBY_STATUS_BALLOON,		// ���D���
	WFLBY_STATUS_FOOTBOAD00,	// ���Ճ{�[�h
	WFLBY_STATUS_FOOTBOAD01,	// ���Ճ{�[�h
	WFLBY_STATUS_WORLDTIMER,	// ���E���v
	WFLBY_STATUS_TOPIC,			// �j���[�X
	WFLBY_STATUS_TALK,			// ��b
	WFLBY_STATUS_FLOAT,			// �t���[�g
	WFLBY_STATUS_LOGOUT,		// �ގ�
	WFLBY_STATUS_MATCHING,		// �~�j�Q�[���̃}�b�`���O��
	WFLBY_STATUS_ANKETO,		// �A���P�[�g��
	

	WFLBY_STATUS_UNKNOWN,		// �s��
	
	WFLBY_STATUS_NUM,
} WFLBY_STATUS_TYPE;

//-------------------------------------
///	���A���ԑ�
//=====================================
typedef enum {
	WFLBY_AISATSU_MORNING,
	WFLBY_AISATSU_NOON,
	WFLBY_AISATSU_NIGHT,
} WFLBY_AISATSU_TIMEZONE;



//-------------------------------------
///	���C�g�f�[�^
//=====================================
typedef enum {
	WFLBY_DRAW_LIGHT_ROOM,	// �������C�g
	WFLBY_DRAW_LIGHT_LAMP,	// �����v���C�g
	WFLBY_DRAW_LIGHT_CLOFF,	// �N���[�Y������郉�C�g
	WFLBY_DRAW_LIGHT_FLOAT,	// �t���[�g�p���C�g
} WFLBY_DRAW_LIGHT_TYPE;




//-------------------------------------
///	�����l�I���~��
//=====================================
typedef enum {
	WFLBY_LIGHT_NEON_ROOM0,
	WFLBY_LIGHT_NEON_ROOM1,
	WFLBY_LIGHT_NEON_ROOM2,
	WFLBY_LIGHT_NEON_ROOM3,
	WFLBY_LIGHT_NEON_ROOM4,
	WFLBY_LIGHT_NEON_ROOM5,
	WFLBY_LIGHT_NEON_ROOMNUM,
} WFLBY_LIGHT_NEON_ROOMTYPE;

//-------------------------------------
///	���l�I���~��
//=====================================
typedef enum {
	WFLBY_LIGHT_NEON_FLOOR0,
	WFLBY_LIGHT_NEON_FLOOR1,
	WFLBY_LIGHT_NEON_FLOOR2,
	WFLBY_LIGHT_NEON_FLOOR3,
	WFLBY_LIGHT_NEON_FLOORNUM,
} WFLBY_LIGHT_NEON_FLOORTYPE;

//-------------------------------------
///	���j�������g�l�I���~��
//=====================================
typedef enum {
	WFLBY_LIGHT_NEON_MONU0,
	WFLBY_LIGHT_NEON_MONU1,
	WFLBY_LIGHT_NEON_MONU2,
	WFLBY_LIGHT_NEON_MONU3,
	WFLBY_LIGHT_NEON_MONUNUM,
} WFLBY_LIGHT_NEON_MONUTYPE;


//-------------------------------------
///	����萔
//=====================================
typedef enum {
	WFLBY_LASTACTION_BOY1,			// �l���Ƃ̐ڐG
	WFLBY_LASTACTION_BOY3,
	WFLBY_LASTACTION_MAN3,
	WFLBY_LASTACTION_BADMAN,
	WFLBY_LASTACTION_EXPLORE,
	WFLBY_LASTACTION_FIGHTER,
	WFLBY_LASTACTION_GORGGEOUSM,
	WFLBY_LASTACTION_MYSTERY,
	WFLBY_LASTACTION_GIRL1,
	WFLBY_LASTACTION_GIRL2,
	WFLBY_LASTACTION_WOMAN2,
	WFLBY_LASTACTION_WOMAN3,
	WFLBY_LASTACTION_IDOL,
	WFLBY_LASTACTION_LADY,
	WFLBY_LASTACTION_COWGIRL,
	WFLBY_LASTACTION_GORGGEOUSW,

	WFLBY_LASTACTION_BALLSLOW,		// �ʓ���
	WFLBY_LASTACTION_BALANCEBALL,	// �ʏ��
	WFLBY_LASTACTION_BALLOON,		// �ӂ�������
	WFLBY_LASTACTION_FOOTWHITE,		// ���Ճ{�[�h�@��
	WFLBY_LASTACTION_FOOTBLACK,		// ���Ճ{�[�h�@��
	WFLBY_LASTACTION_WLDTIMER,		// ���E���v
	WFLBY_LASTACTION_NEWS,			// ���r�[�j���[�X
	WFLBY_LASTACTION_ANKETO,		// �A���P�[�g

	// �v���`�i��͂���ȍ~�ɒǉ�

	WFLBY_LASTACTION_MAX,
} WFLBY_LASTACTION_TYPE;



//-------------------------------------
///	�A���P�[�g��{���
//=====================================
#define ANKETO_NORMAL_Q_NUM		( 50 )
#define ANKETO_SPECIAL_Q_NUM	( 10 )
enum{
	ANKETO_ANSWER_A,
	ANKETO_ANSWER_B,
	ANKETO_ANSWER_C,
	ANKETO_ANSWER_NUM,
};



//-------------------------------------
///	����		4byte
//=====================================
typedef struct {
	union{
		u32 time;
		struct{
			u8		hour;
			u8		minute;
			u8		second;
			u8		dummy;
		};
	};
} WFLBY_TIME;
// ����֐�
// cp_time cp_add p_anm�@���ׂē������̂ł����v�ł�
extern void WFLBY_TIME_Set( WFLBY_TIME* p_time, const s64* cp_settime );
extern void WFLBY_TIME_Add( const WFLBY_TIME* cp_time, const WFLBY_TIME* cp_add, WFLBY_TIME* p_ans );
extern void WFLBY_TIME_Sub( const WFLBY_TIME* cp_time, const WFLBY_TIME* cp_sub, WFLBY_TIME* p_ans );


//-----------------------------------------------------------------------------
/**
 *				���r�[�j���[�X�@�g�s�b�N�f�[�^
 */
//-----------------------------------------------------------------------------
#define NEWS_TOPICNAME_NUM	( 4 )	// �\�������l�̍ő吔

//-------------------------------------
///	���r�[�j���[�X�@�g�s�b�N���M�f�[�^
//	20byte
//=====================================
typedef struct{
	s32	lobby_id[NEWS_TOPICNAME_NUM];	// �L���ID	32byte
	u8	num;							// �Q����
	u8	topic_type;						// �g�s�b�N�^�C�v		NEWS_TOPICTYPE
	u8	item;							// �A�C�e���i���o�[		WFLBY_ITEMTYPE
	u8	minigame:7;						// �~�j�Q�[���i���o�[	WFLBY_GAMETYPE
	u8	play:1;							// �~�j�Q�[����V��ł����Ԃ�
} WFLBY_TOPIC;


//-----------------------------------------------------------------------------
/**
 *				���E�n�}�f�[�^
 */
//-----------------------------------------------------------------------------
#define WFLBY_WLDTIMER_DATA_MAX		(50)	// �ő�i�[�n�搔
//-------------------------------------
///	1�n��f�[�^
//	4byte
//=====================================
typedef struct {
	u16 nation;			///<��
	u8  area;			///<�n��
	u8  outside:4;		///<�ގ��t���O
	u8  flag:4;			///<�f�[�^�L��	�i�f�[�^���i�[����Ă���̂��ǂ����j
} WFLBY_COUNTRY;
//-------------------------------------
///	���E�n�}�n��f�[�^
//	8*WFLBY_WLDTIMER_DATA_MAX	byte
//=====================================
typedef struct _WFLBY_WLDTIMER{
	WFLBY_COUNTRY	data[WFLBY_WLDTIMER_DATA_MAX];
} WFLBY_WLDTIMER;
//-------------------------------------
///	�A�N�Z�X�֐�
//=====================================
extern void WFLBY_WLDTIMER_SetData( WFLBY_WLDTIMER* p_data, u16 nation, u8 area, BOOL outside );
extern u16 WFLBY_WLDTIMER_GetNation( const WFLBY_WLDTIMER* cp_data, u8 index );
extern u8 WFLBY_WLDTIMER_GetArea( const WFLBY_WLDTIMER* cp_data, u8 index );
extern BOOL WFLBY_WLDTIMER_GetOutSide( const WFLBY_WLDTIMER* cp_data, u8 index );
extern BOOL WFLBY_WLDTIMER_GetDataFlag( const WFLBY_WLDTIMER* cp_data, u8 index );





//-----------------------------------------------------------------------------
/**
 *			���Ճf�[�^
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	1���Ճf�[�^
//	12byte
//=====================================
typedef struct {
	u16		monsno;		///< �����X�^�[�i���o�[
	u16		col;		///< ���Ղ̐F
	s16		x;			///< �����W
	s16		y;			///< �����W
	u8		formno;		///< �t�H�����i���o�[
	u8		move;		///< ����^�C�v

	u16		pad;
} WFLBY_FOOT;



#if 0
#define WFLBY_FTBRD_DATA_MAX	( 600 )	// �ő匏��
//-------------------------------------
///	1���Ճf�[�^
//	8byte
//=====================================
typedef struct {
	u16		monsno;		///< �����X�^�[�i���o�[
	u8		formno;		///< �t�H�����i���o�[
	u8		sex;		///< ����
	u16		col;		///< ���Ղ̐F
	u8		flag;		///< �f�[�^�L��	
	u8		dummy;		///< 
} WFLBY_FOOT;

//-------------------------------------
///	���Ճf�[�^
//	8*WFLBY_FTBRD_DATA_MAX	byte
//=====================================
typedef struct _WFLBY_FTBRD{
	WFLBY_FOOT	data[WFLBY_FTBRD_DATA_MAX];
} WFLBY_FTBRD;
#endif

//-----------------------------------------------------------------------------
/**
 *			���C�g���\�[�X����
 *				���C�g�̉e�����󂯂郂�f���͂�����ʂ�
 */
//-----------------------------------------------------------------------------
extern void WFLBY_LIGHT_SetUpMdl( void* p_file );





//-----------------------------------------------------------------------------
/**
 *			�������Ƃ�	�f�[�^
 */
//-----------------------------------------------------------------------------
// ���[�h��
#define WFLBY_AIKOTOBA_WORD_NUM	( 4 )	

//-------------------------------------
///	�������Ƃ΃o�b�t�@
//=====================================
typedef struct { 
	PMS_WORD word[ WFLBY_AIKOTOBA_WORD_NUM ];
} WFLBY_AIKOTOBA_DATA;




//-----------------------------------------------------------------------------
/**
 *			P2P�~�j�Q�[���ȂǂɎg�p���郏�[�N
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	VIP�t���O�o�b�t�@
//=====================================
typedef struct {
	u8  vip[ WFLBY_PLAYER_MAX ];	// �S�v���C���[����VIP�t���O
} WFLBY_VIPFLAG;

//-------------------------------------
///	�~�j�Q�[���pPLIDX�o�b�t�@
//=====================================
typedef struct {
	u8  plidx[ WFLBY_MINIGAME_MAX ];	// �S�v���C���[���̃��r�[���l��IDX
} WFLBY_MINIGAME_PLIDX;


//-------------------------------------
///	�A���P�[�g�ێ����[�N
//=====================================
typedef struct {
	u32	anketo_no;		// �A���P�[�g�ʂ��i���o�[
	u32	select;			// �I���i���o�[
} WFLBY_ANKETO;


//-------------------------------------
///	�A���P�[�g���ʏ��
//=====================================
typedef struct {
	u32	ans[ ANKETO_ANSWER_NUM ];	
	u64 ans_all;
} ANKETO_QUESTION_RESULT;




//-----------------------------------------------------------------------------
/**
 *			�n�悪����̂��`�F�b�N
 */
//-----------------------------------------------------------------------------
extern BOOL WFLBY_AREA_Check( u16 nation, u16 area );


//-----------------------------------------------------------------------------
/**
 *			�L����Z�^�C�v�萔����퓬�p�Z�^�C�v�萔�����߂�֐�
 */
//-----------------------------------------------------------------------------
extern u32 WFLBY_BattleWazaType_Get( WFLBY_POKEWAZA_TYPE type );




//-----------------------------------------------------------------------------
/**
 *			�G���[����
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	���̌�ǂ�����ׂ����̒萔
//=====================================
typedef enum {
	WFLBY_ERR_TYPE_RETRY,	// �Đڑ�
	WFLBY_ERR_TYPE_END,		// �I����
} WFLBY_ERR_TYPE;


extern int WFLBY_ERR_GetStrID( int errno,int errtype );
extern WFLBY_ERR_TYPE WFLBY_ERR_GetErrType( int errno,int errtype );

// �G���[���������Ȃ̂����`�F�b�N
extern BOOL WFLBY_ERR_CheckError( void );



#endif		// __WFLBY_DEF_H__


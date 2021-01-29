//==============================================================================
/**
 * @file	guru2_comm.h
 * @brief	���邮������@�ʐM����
 * @author	kagaya
 * @data	05.07.13
 */
//==============================================================================
#ifndef GURU2_COMM_FILE
#define GURU2_COMM_FILE

#include "communication/comm_command.h"
#include "../../field/comm_command_field.h"

#include "communication/communication.h"
#include "communication/comm_state.h"
#include "communication/comm_save.h"
#include "communication/wm_icon.h"
#include "field/comm_union_beacon.h"
#include "field/comm_union_view_common.h"

//==============================================================================
//	define
//==============================================================================
#define COMM_GURU2_TIMINGSYNC_NO (202)

//--------------------------------------------------------------
///	�ʐM�l��
//--------------------------------------------------------------
enum
{
	G2MEMBER_0,
	G2MEMBER_1,
	G2MEMBER_2,
	G2MEMBER_3,
	G2MEMBER_4,
	G2MEMBER_MAX,
};

#define GURU2_FRIEND_MAX (G2MEMBER_MAX-1)	///<����ő�

//--------------------------------------------------------------
///	�ʐM�R�}���h
//--------------------------------------------------------------
enum
{
	G2COMM_RC_STOP,			///<�����ɂ���~
	G2COMM_RC_RESTART,		///<�����I���A�ēx�J�n
	G2COMM_RC_END_CHILD,	///<�q�@���E
	G2COMM_RC_END,			///<�I��
	G2COMM_RC_CHILD_JOIN,	///<�q�@�����J�n
	G2COMM_RC_START,		///<�J�n
	G2COMM_RC_DATA,			///<�f�[�^�]��
	G2COMM_RC_BAN,			///<���E�֎~�E����
	
	G2COMM_GM_SIGNAL,		///<�Q�[���@�V�O�i��
	G2COMM_GM_JOIN,			///<�Q�[���@�Q��
	G2COMM_GM_BTN,			///<�Q�[���@�{�^�����
	G2COMM_GM_GAMEDATA,		///<�Q�[���@���
	G2COMM_GM_PLAYNO,		///<�Q�[���@�v���C�ԍ�
	G2COMM_GM_PLAYMAX,		///<�Q�[���@�v���C�ő吔
	G2COMM_GM_TRADE_POS,	///<�Q�[���@�����ʒu
	G2COMM_GM_GAMERESULT,	///<�Q�[���@����
	G2COMM_GM_EGG_DATA_NG,	///<�Q�[���@���f�[�^�ُ�
	G2COMM_GM_EGG_DATA_OK,	///<�Q�[���@���f�[�^����
	
	G2COMM_MAX,				///<�ő�
};

//--------------------------------------------------------------
///	�ʐM���[�N
//--------------------------------------------------------------
#define GURU2_WIDEUSE_SENDWORK_SIZE (24)	///�ėp���M�f�[�^�ő�

//--------------------------------------------------------------
///	��t
//--------------------------------------------------------------
enum
{
	CREC_REQ_RIDATU_CHECK,		///<���E�m�F
	CREC_REQ_RIDATU_EXE,		///<���E���s
};

//--------------------------------------------------------------
///	�|�P�����Z���N�g�@�e�I���t���O
//--------------------------------------------------------------
enum
{
	G2C_OYA_END_FLAG_NON = 0,
	G2C_OYA_END_FLAG_EXE,
	G2C_OYA_END_FLAG_END,
};

//--------------------------------------------------------------
///	�Q�[���V�O�i��
//--------------------------------------------------------------
#define G2COMM_GMSBIT_JOIN_CLOSE	(1<<0) ///<�Q�[���Q�����ߐ؂�
#define G2COMM_GMSBIT_CANCEL		(1<<1) ///<�Q�[���L�����Z��
#define G2COMM_GMSBIT_GAME_START	(1<<2) ///<�Q�[���J�n
#define G2COMM_GMSBIT_EGG_ADD_START (1<<3) ///<�^�}�S�ǉ��J�n

#define G2COMM_GAMEBIT_DISC_STOP	(1<<0) ///<�Q�[���@�M��~
#define G2COMM_GAMEBIT_BTN_STOP		(1<<1) ///<�Q�[���@�{�^�����͋֎~

#define GURU2COMM_BAN_NONE		(0)
#define GURU2COMM_BAN_ON		(1)
#define GURU2COMM_BAN_MOST_ON	(2)

#define GURU2_COMM_SYNCRO_0 (201)	///<�ʐM���� 0

#define GURU2COMM_RC_SEND_DATABUF_SIZE	(3000)		///<���M�f�[�^�T�C�Y

// �|�P�����p�[�e�B�[�𑗐M����Ƃ��̃f�[�^��
#define POKEPARTY_SEND_ONCE_SIZE (236*6+4*2)	// sizeof(POKEPARTY)

//==============================================================================
//	typedef
//==============================================================================
//--------------------------------------------------------------
//	�Q�Ɨp
//--------------------------------------------------------------
typedef struct _TAG_GURU2PROC_WORK GURU2PROC_WORK;

//--------------------------------------------------------------
///	GURU2COMM_END_CHILD_WORK
///	���̍\���̂̒��g��ւ�����A�K���ʐM�֐��e�[�u����
///	�g�p���Ă���ӏ��̑���M�T�C�Y��ύX���邱��
//--------------------------------------------------------------
typedef struct
{
	u8 ridatu_id;			///<���E�҂�ID
	u8 oya_share_num;		///<�e�̎���shareNum
	u8 request;				///<���߃R�[�h
	u8 ridatu_kyoka;		///<TRUE:���EOK�AFALSE:���ENG
}GURU2COMM_END_CHILD_WORK;

//--------------------------------------------------------------
///	GURU2COMM_RC_SENDDATA
//--------------------------------------------------------------
typedef struct
{
	u8  data[GURU2COMM_RC_SEND_DATABUF_SIZE];	//���R�[�h�����f�[�^
	u32 _xor;						//�f�[�^�������m�F�pXOR
	u32 seed;	//�e���痈�����̐��l�����ɗ����𔭐������ăf�[�^�����ւ���
}GURU2COMM_RC_SENDDATA;

//--------------------------------------------------------------
///	GURU2COMM_GAMEDATA
//--------------------------------------------------------------
typedef struct
{
	u16 oya_flag;
	u16 game_time;
	s16 disc_speed;
	u16 disc_angle;
}GURU2COMM_GAMEDATA;

//--------------------------------------------------------------
///	GURU2COMM_GAMERESULT
//--------------------------------------------------------------
typedef struct
{
	u16 disc_angle;					//�M�p�x
	u16 omake_bit;					//���܂��G���A�q�b�g�r�b�g
	u16 egg_angle[G2MEMBER_MAX];	//���p�x
	u16 dmy;
}GURU2COMM_GAMERESULT;

//--------------------------------------------------------------
///	GURU2COMM_PLAYNO
//--------------------------------------------------------------
typedef struct
{
	u16 play_no;
	u16 comm_id;
}GURU2COMM_PLAYNO;

//--------------------------------------------------------------
///	PSEL_OYAEND_WORK
//--------------------------------------------------------------
typedef struct
{
	int seq_no;
}GURU2COMM_PSEL_OYAEND_TCB_WORK;

//--------------------------------------------------------------
///	GURU2COMM_KO_CANCEL_WORK
//--------------------------------------------------------------
typedef struct
{
	u8 cancel_id;
	u8 cancel_flag;
	u16 dmy;
}GURU2COMM_CANCEL_ANSWER;

//--------------------------------------------------------------
///	GURU2COMM_WORK
//--------------------------------------------------------------
typedef struct _TAG_GURU2COMM_WORK
{
	GURU2PROC_WORK *g2p;
	u8 wideuse_sendwork[GURU2_WIDEUSE_SENDWORK_SIZE]; //�]���p���[�N
	
	int send_stop_flag;		//���M��~
	int recv_count;
	int record_execute;
	int banFlag;			//����֎~�t���O
	int	shareNum;
	u32	ridatu_bit;			//���E���悤�Ƃ��Ă���q��bit
	int send_num;
	u16	oya_share_num;		//�e�������Ă���shareNum�����炤
	
	int comm_psel_oya_end_flag;		//�|�P�����Z���N�g���̐e�I���t���O
	u16 comm_game_signal_bit;		//�Q�[���V�O�i��
	u16 comm_game_join_bit;			//�Q�[���Q���r�b�g
	u16 comm_game_cancel_bit;		//�Q�[���L�����Z��
	s16 comm_game_egg_recv_bit;		//�^�}�S�f�[�^��M�r�b�g
	u16 comm_game_egg_check_error_bit;
	u16 comm_game_egg_check_ok_bit;
	u8 *recv_poke_party_buf;//[G2MEMBER_MAX][POKEPARTY_SEND_ONCE_SIZE]
	u8 *friend_poke_party_buf;//[G2MEMBER_MAX][POKEPARTY_SEND_ONCE_SIZE]
	
	GURU2COMM_PSEL_OYAEND_TCB_WORK pselend_work;
}GURU2COMM_WORK;

//==============================================================================
//	extern
//==============================================================================
extern GURU2COMM_WORK * Guru2Comm_WorkInit(GURU2PROC_WORK *g2p, u32 heap_id);
extern void Guru2Comm_WorkDelete( GURU2COMM_WORK *g2c );
extern void Guru2Comm_CommandInit( GURU2COMM_WORK *g2c );
extern void Guru2Comm_CommUnionRoomChange( GURU2COMM_WORK *g2c );

extern void * Guru2Comm_WideUseSendWorkGet( GURU2COMM_WORK *g2c, u32 size );

extern BOOL Guru2Comm_SendData(
	GURU2COMM_WORK *g2c, u32 code, const void *data, u32 size );

extern int Guru2Comm_OnlineNumGet( void );
extern void * Guru2Comm_FriendPokePartyGet( GURU2COMM_WORK *g2c, int id );

#endif //GURU2_COMM_FILE

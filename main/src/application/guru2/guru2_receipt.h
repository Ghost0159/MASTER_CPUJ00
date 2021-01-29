//=============================================================================
/**
 * @file	guru2_receipt.h
 * @brief	���邮������@��t�@�i���R�[�h�R�[�i�[�̃p�N��
 * @author	kagaya
 * @date    2006.02.14
 */
//=============================================================================
#ifndef GURU2_RECEIPT_H
#define GURU2_RECEIPT_H

#include "guru2_local.h"

//�@�\�{�^���p��`
#define FUNCBUTTON_NUM	( 7 ) 		// �@�\�{�^���̐�
#define START_WORDPANEL ( 0 )		// �ŏ��̕������̓p�l���̔ԍ��i�O���Ђ炪�ȁj

// �ڑ��l��MAX�͂T�l
#define RECORD_CORNER_MEMBER_MAX	( 5 )

// CLACT�Œ�`���Ă���Z�����傫�����ăT�u��ʂɉe�����łĂ��܂��̂ŗ����Ă݂�
#define NAMEIN_SUB_ACTOR_DISTANCE 	(256*FX32_ONE)

// CellActor�ɏ��������郊�\�[�X�}�l�[�W���̎�ނ̐��i���}���`�Z���E�}���`�Z���A�j���͎g�p���Ȃ��j
#define CLACT_RESOURCE_NUM		(  4 )
#define NAMEIN_OAM_NUM			( 14 )


// ���b�Z�[�W�\�����WAIT
#define RECORD_CORNER_MESSAGE_END_WAIT	( 60 )

#define ERRNUM_TIMEOUT  (10*30)  //PL:0632 err_num���ω����Ȃ����߃V�[�P���X���i�܂Ȃ��o�O�p�b�`�C��

// �����p�l���̑J�ڗp
enum{
	RECORD_MODE_INIT  = 0, 

	RECORD_MODE_NEWMEMBER,
	RECORD_MODE_NEWMEMBER_END,

	RECORD_MODE,

	RECORD_MODE_END_SELECT,
	RECORD_MODE_END_SELECT_WAIT,
	RECORD_MODE_END_SELECT_ANSWER_WAIT,
	RECORD_MODE_END_SELECT_ANSWER_OK,
	RECORD_MODE_END_SELECT_ANSWER_NG,
	
    RECORD_MODE_END_CHILD,
    RECORD_MODE_END_CHILD_WAIT,

	RECORD_MODE_END_SELECT_PARENT,
    RECORD_MODE_END_SELECT_PARENT_WAIT,

	RECORD_MODE_FORCE_END,
	RECORD_MODE_FORCE_END_MES_WAIT,
    RECORD_MODE_FORCE_END_WAIT,
    RECORD_MODE_FORCE_END_SYNCHRONIZE,

	RECORD_MODE_END_PARENT_ONLY,
	RECORD_MODE_END_PARENT_ONLY_WAIT,

	RECORD_MODE_LOGOUT_CHILD,
    RECORD_MODE_LOGOUT_CHILD_WAIT,
    RECORD_MODE_LOGOUT_CHILD_CLOSE,

	RECORD_MODE_START_SELECT,
	RECORD_MODE_START_SELECT_WAIT,

	RECORD_MODE_START_RECORD_COMMAND,
	RECORD_MODE_RECORD_SEND_DATA,

	RECORD_MODE_RECORD_MIX_DATA,

	RECORD_MODE_COMM_SAVE_START,
    RECORD_MODE_COMM_SAVE,
    RECORD_MODE_END_MES_WAIT,


    RECORD_MODE_MESSAGE_WAIT,
	
	RECORD_MODE_GURU2_POKESEL_START,
};

#if 0
enum{
	RECORD_SYNCHRONIZE_END=201,
};
#endif

// �㉺��ʎw���`
#define RES_NUM	( 3 )
#define MAIN_LCD	( GF_BGL_MAIN_DISP )	// �v�͂O��
#define SUB_LCD		( GF_BGL_SUB_DISP )		// �P�Ȃ�ł����B
#define CHARA_RES	( 2 )

// BMPWIN�w��
enum{
	BMP_NAME1_S_BG0,
	BMP_NAME2_S_BG0,
	BMP_NAME3_S_BG0,
	BMP_NAME4_S_BG0,
	BMP_NAME5_S_BG0,
	BMP_RECORD_MAX,
};

enum{
	RECORD_EXIST_NO=0,
	RECORD_EXIST_APPEAR_REQ,
	RECORD_EXIST_APPEAR,
	RECORD_EXIST_BYE_REQ,
};

typedef struct{
	int sw;
	int seq;
	int wait;
	void *paldata;
	NNSG2dPaletteData *palbuf;
}VTRANS_PAL_WORK;

//-------------------------------------
///	080626 tomoya
//  �g���[�iID�ۑ��̈�
//=====================================
typedef union {
	u64		check;
	struct{
		u32		tr_id;
		u32		in;
	}data;
} TR_ID_DATA;

//============================================================================================
//	�\���̒�`
//============================================================================================
struct _TAG_GURU2RC_WORK{
	int proc_seq;
	int seq_no;												//�V�[�P���X
	int end_next_flag;
	GURU2PROC_WORK *g2p;
	GURU2COMM_WORK *g2c;
	
	GF_BGL_INI			*bgl;								// GF_BGL_INI
	BOOL				wipe_end;							// ���C�v�����҂�
	VTRANS_PAL_WORK		palwork;							// �p���b�g�A�j���p���[�N
	TCB_PTR				trans_tcb;
	
	WORDSET			*WordSet;								// ���b�Z�[�W�W�J�p���[�N�}�l�[�W���[
	MSGDATA_MANAGER *MsgManager;							// ���O���̓��b�Z�[�W�f�[�^�}�l�[�W���[
	STRBUF			*TrainerName[RECORD_CORNER_MEMBER_MAX];		// ���O
	STRBUF			*EndString;								// ������u��߂�v
	STRBUF			*TalkString;							// ��b���b�Z�[�W�p
	STRBUF			*TitleString;							// �^�C�g�����b�Z�[�W�p
	int				MsgIndex;								// �I�����o�p���[�N
	
	CLACT_SET_PTR 			clactSet;								// �Z���A�N�^�[�Z�b�g
	CLACT_U_EASYRENDER_DATA	renddata;								// �ȈՃ����_�[�f�[�^
	CLACT_U_RES_MANAGER_PTR	resMan[CLACT_RESOURCE_NUM];				// ���\�[�X�}�l�[�W��
	CLACT_U_RES_OBJ_PTR 	resObjTbl[RES_NUM][CLACT_RESOURCE_NUM];// ���\�[�X�I�u�W�F�e�[�u��
	CLACT_HEADER			clActHeader_m;							// �Z���A�N�^�[�w�b�_�[
	CLACT_HEADER			clActHeader_s;							// �Z���A�N�^�[�w�b�_�[
	CLACT_HEADER			clActHeader_friend;							// �Z���A�N�^�[�w�b�_�[
	CLACT_WORK_PTR			MainActWork[NAMEIN_OAM_NUM];				// �Z���A�N�^�[���[�N�|�C���^�z��
	CLACT_WORK_PTR			SubActWork[NAMEIN_OAM_NUM];				// �Z���A�N�^�[���[�N�|�C���^�z��

	GF_BGL_BMPWIN 			TrainerNameWin[BMP_RECORD_MAX];			// ���G������ʗpBMP�E�C���h�E
	GF_BGL_BMPWIN			MsgWin;									// ��b�E�C���h�E
	GF_BGL_BMPWIN			EndWin;									// ��߂�
	GF_BGL_BMPWIN			TitleWin;								// �u���R�[�h�R�[�i�[�@�ڂ��イ���イ�I�v�Ȃ�
	GF_BGL_BMPWIN			*YesNoWin[2];							// �͂��E�������E�C���h�E�̃|�C���^
	BMPMENU_WORK			*YesNoMenuWork;

	void*					FieldObjCharaBuf[2];					// �l��OBJ�L�����t�@�C���f�[�^
	NNSG2dCharacterData*	FieldObjCharaData[2];					// �l��OBJ�L�����f�[�^�̎��|�C���^				
	void*					FieldObjPalBuf[2];						// �l��OBJ�p���b�g�@�C���f�[�^
	NNSG2dPaletteData*		FieldObjPalData[2];						// �l��OBJ�p���b�g�t�@�C���f�[�^

	int						seq;									// ���݂̕������͏�ԁi����OK/�A�j�����j�Ȃ�
	int						nextseq;
	int						mode;									// ���ݍőO�ʂ̕����p�l��
	int						wait;
	u8						ConnectCheck[8][2];
	MYSTATUS				*TrainerStatus[RECORD_CORNER_MEMBER_MAX][2];
	int						TrainerReq[RECORD_CORNER_MEMBER_MAX];

	int						saveseq_work;							// �Z�[�u�V�[�P���X�Ǘ��p���[�N

	u16						CursorPal;
	u16						*ObjPaletteTable;
	u8						brush_color;
	u8						brush;

	GURU2COMM_RC_SENDDATA	send_data;
	GURU2COMM_RC_SENDDATA	recv_data[RECORD_CORNER_MEMBER_MAX];
	u8						record_execute;							// TRUE:���R�[�h������
	
	// ----�e�������K�v�ȃ��[�N
	volatile int			connectBackup;							// �ڑ��l���Ď��p
	int						beacon_flag;
	u32						ridatu_bit;								// ���E���悤�Ƃ��Ă���q��Bit
	u8						status_end;								// TRUE:�I���V�[�P���X�ֈڍs
	s8						limit_mode;								// �ڑ��������
	u8						start_num;								// ���R�[�h�����J�n�l��
	u8						record_send;							// TRUE:���R�[�h���M����
	
	// �q�@���K�v�ȃ��[�N
    int err_num_timeout;   ///PL:0632 err_num���ω����Ȃ����߃V�[�P���X���i�܂Ȃ��o�O�p�b�`�C��
	s32 					err_num;								// �ʐM�I�����݂邽�߂̃��[�N
	s16						ridatu_wait;			// ���E�p�̃E�F�C�g

  //------080626	tomoya �C�� �g���[�i�̓o��Ƒޏ�`�F�b�N�p�̃g���[�iID�ۑ��̈�
	TR_ID_DATA				trainer_id[RECORD_CORNER_MEMBER_MAX][2];

#ifdef PM_DEBUG
	int 					frame;									//
	int						framenum[9][2];							//
#endif
};

//==============================================================================
//	extern
//==============================================================================
extern void Guru2Rc_MainSeqForceChange( GURU2RC_WORK *wk, int seq, u8 id  );
extern void Guru2Rc_MainSeqCheckChange( GURU2RC_WORK *wk, int seq, u8 id  );

#if 0
extern void CommCommandRecordInitialize(void* pWork);
extern void RecordCorner_MainSeqCheckChange( RECORD_WORK *wk, int seq, u8 id );
extern void RecordCorner_MainSeqForceChange( RECORD_WORK *wk, int seq, u8 id  );
extern void RecordCornerTitleChange( RECORD_WORK *wk );
extern int RecordCorner_MyStatusGetNum(void);

extern void MakeSendData(SAVEDATA * sv, RECORD_DATA * send_rec);
extern void MixReceiveData(SAVEDATA * sv, const RECORD_DATA * record);
#endif

#endif

//-----------------------------------------------------------------------------
/**
 *			Wi-Fi�L����ꋖ�m�F�f�[�^�\����
 */
//-----------------------------------------------------------------------------


//-------------------------------------
/// �l���̖��O�̒����i�������܂ށj
//=====================================
#define PERSON_NAME_SIZE	(7)	

//-------------------------------------
/// �I���R�[�h�̒���
//=====================================
#define	EOM_SIZE			(1)	

//-------------------------------------
///	�莝���|�P�����̍ő吔
//=====================================
#define TEMOTI_POKEMAX	(6)


//-------------------------------------
///	�s�������Ƃ̗����@�ێ���
//=====================================
#define WFLBY_LASTACT_BUFFNUM	( 12 )


//-------------------------------------
///	���[�U���v���t�B�[���p�ɑI�����鑮���^�C�v
//=====================================
#define WFLBY_SELECT_TYPENUM		(2)								// �I��



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

//-------------------------------------
///	�����R�[�h		2byte
//=====================================
typedef u16		STRCODE;


//-------------------------------------
///	�A���P�[�g�ێ����[�N
//=====================================
typedef struct {
	u32	anketo_no;		// �A���P�[�g�ʂ��i���o�[
	u32	select;			// �I���i���o�[
} WFLBY_ANKETO;


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


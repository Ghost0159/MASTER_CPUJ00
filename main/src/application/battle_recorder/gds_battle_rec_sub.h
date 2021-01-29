//==============================================================================
/**
 *
 * �퓬�^��̃T�u�\���̂�萔�̒�`�Ȃ�
 *
 */
//==============================================================================
#ifndef __GT_GDS_BATTLE_REC_SUB_H__
#define __GT_GDS_BATTLE_REC_SUB_H__


#include "gds_define.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�^��f�[�^�̑��ݗL��
#define GT_REC_OCC_MAGIC_KEY		(0xe281)

///�^��w�b�_�Ɋi�[����|�P�����ԍ��̍ő�v�f��
#define GT_HEADER_MONSNO_MAX		(12)

#define	GT_WAZA_TEMOTI_MAX		(4)		///<1�̂̃|�P���������Ă�Z�̍ő�l

#define GT_PERSON_NAME_SIZE	7	// �l���̖��O�̒����i�������܂ށj
#define GT_MONS_NAME_SIZE		10	// �|�P�������̒���(�o�b�t�@�T�C�Y EOM_�܂܂�)
#define	GT_EOM_SIZE			1	// �I���R�[�h�̒���

#define	GT_CLIENT_MAX			(4)		///<�����ɋN������N���C�A���g�̍ő吔

#define GT_TEMOTI_POKEMAX		6	///< �莝���|�P�����ő吔

#define	GT_REC_BUFFER_SIZE	(0x0400)	///<�ΐ�^��o�b�t�@�T�C�Y

#define GT_REGULATION_NAME_SIZE   (11)      // ���[�����̒��� 11����22�o�C�g +EOM2byte

///�o�g�����[�h
typedef enum{
	//--------------------------------------------------------------
	//	�R���V�A��
	//--------------------------------------------------------------
	GT_BATTLE_MODE_COLOSSEUM_SINGLE,			//�V���O���o�g��(�~�b�N�X�܂�)�F�����Ȃ�
	GT_BATTLE_MODE_COLOSSEUM_SINGLE_STANDARD,	//�V���O���o�g��(�~�b�N�X�܂�)�F�X�^���_�[�h�J�b�v
	GT_BATTLE_MODE_COLOSSEUM_SINGLE_FANCY,		//�V���O���o�g��(�~�b�N�X�܂�)�F�t�@���V�[�J�b�v
	GT_BATTLE_MODE_COLOSSEUM_SINGLE_LITTLE,		//�V���O���o�g��(�~�b�N�X�܂�)�F���g���J�b�v
	GT_BATTLE_MODE_COLOSSEUM_SINGLE_LIGHT,		//�V���O���o�g��(�~�b�N�X�܂�)�F���C�g�J�b�v
	GT_BATTLE_MODE_COLOSSEUM_SINGLE_DOUBLE,		//�V���O���o�g��(�~�b�N�X�܂�)�F�_�u���J�b�v
	GT_BATTLE_MODE_COLOSSEUM_SINGLE_ETC,		//�V���O���o�g��(�~�b�N�X�܂�)�F���̑��̃J�b�v
	GT_BATTLE_MODE_COLOSSEUM_DOUBLE,			//�_�u���o�g���F�����Ȃ�
	GT_BATTLE_MODE_COLOSSEUM_DOUBLE_STANDARD,	//�_�u���o�g���F�X�^���_�[�h�J�b�v
	GT_BATTLE_MODE_COLOSSEUM_DOUBLE_FANCY,		//�_�u���o�g���F�t�@���V�[�J�b�v
	GT_BATTLE_MODE_COLOSSEUM_DOUBLE_LITTLE,		//�_�u���o�g���F���g���J�b�v
	GT_BATTLE_MODE_COLOSSEUM_DOUBLE_LIGHT,		//�_�u���o�g���F���C�g�J�b�v
	GT_BATTLE_MODE_COLOSSEUM_DOUBLE_DOUBLE,		//�_�u���o�g���F�_�u���J�b�v
	GT_BATTLE_MODE_COLOSSEUM_DOUBLE_ETC,		//�_�u���o�g���F���̑��̃J�b�v
	GT_BATTLE_MODE_COLOSSEUM_MULTI,				//�}���`�o�g��

	//--------------------------------------------------------------
	//	�t�����e�B�A
	//--------------------------------------------------------------
	//�^���[
	GT_BATTLE_MODE_TOWER_SINGLE,		//WIFI_DL�܂�
	GT_BATTLE_MODE_TOWER_DOUBLE,
	GT_BATTLE_MODE_TOWER_MULTI,		//NPC, COMM, WIFI
	//�t�@�N�g���[�pINDEX�J�n
	GT_BATTLE_MODE_FACTORY50_SINGLE,
	GT_BATTLE_MODE_FACTORY50_DOUBLE,
	GT_BATTLE_MODE_FACTORY50_MULTI,	//COMM, WIFI
	GT_BATTLE_MODE_FACTORY100_SINGLE,
	GT_BATTLE_MODE_FACTORY100_DOUBLE,
	GT_BATTLE_MODE_FACTORY100_MULTI,	//COMM, WIFI
	//�X�e�[�W�pINDEX�J�n
	GT_BATTLE_MODE_STAGE_SINGLE,
	GT_BATTLE_MODE_STAGE_DOUBLE,
	GT_BATTLE_MODE_STAGE_MULTI,		//COMM, WIFI
	//�L���b�X���pINDEX�J�n
	GT_BATTLE_MODE_CASTLE_SINGLE,
	GT_BATTLE_MODE_CASTLE_DOUBLE,
	GT_BATTLE_MODE_CASTLE_MULTI,		//COMM, WIFI
	//���[���b�g�pINDEX�J�n
	GT_BATTLE_MODE_ROULETTE_SINGLE,
	GT_BATTLE_MODE_ROULETTE_DOUBLE,
	GT_BATTLE_MODE_ROULETTE_MULTI,		//COMM, WIFI

	//--------------------------------------------------------------
	//	�w��Ȃ�
	//--------------------------------------------------------------
	GT_BATTLE_MODE_COLOSSEUM_SINGLE_NO_REGULATION = 0xfa,	//�R���V�A��/�V���O�� �����Ȃ�
	GT_BATTLE_MODE_COLOSSEUM_SINGLE_REGULATION = 0xfb,		//�R���V�A��/�V���O�� �J�b�v��
	GT_BATTLE_MODE_COLOSSEUM_DOUBLE_NO_REGULATION = 0xfc,	//�R���V�A��/�_�u�� �����Ȃ�
	GT_BATTLE_MODE_COLOSSEUM_DOUBLE_REGULATION = 0xfd,		//�R���V�A��/�_�u�� �J�b�v��
	GT_BATTLE_MODE_EXCLUSION_FRONTIER = 0xfe,				//�t�����e�B�A�{�݂����O
	GT_BATTLE_MODE_NONE = 0xff,			//�w��Ȃ�
}GT_BATTLE_MODE;

//--------------------------------------------------------------
//	���������h�w��Ȃ��h�̒�`
//--------------------------------------------------------------
///�|�P�����w�薳��
#define GT_BATTLE_REC_SEARCH_MONSNO_NONE			(0xffff)
///�o�g�����[�h�w�薳��
#define GT_BATTLE_REC_SEARCH_BATTLE_MODE_NONE		(0xff)
///���R�[�h�w�薳��
#define GT_BATTLE_REC_SEARCH_COUNTRY_CODE_NONE		(0xff)
///�n���R�[�h�w�薳��
#define GT_BATTLE_REC_SEARCH_LOCAL_CODE_NONE		(0xff)

//==============================================================================
//	�^��`
//==============================================================================
///�f�[�^�i���o�[�̌^
typedef u64 GT_DATA_NUMBER;


//----------------------------------------------------------
/**
 * @brief	������ԃf�[�^�^��`
 */
//----------------------------------------------------------
typedef struct {
	GT_STRCODE name[GT_PERSON_NAME_SIZE + GT_EOM_SIZE];		// 16
	u32 id;											// 20
	u32 gold;										// 24

	u8 sex;	
	u8 region_code;									//26

	u8 badge;										
	u8 trainer_view;	// ���j�I�����[�����ł̌����ڃt���O //28
	u8 rom_code;		// 0:�_�C��  1:�p�[��		// 29

	u8 dp_clear:1;		// DP�N���A���Ă��邩�t���O
	u8 dp_zenkoku:1;	// DP�S���}�ӎ�ɓ���Ă��邩�t���O
	u8 dummy1:6;									// 30
	u8 dummy2;										// 31
	u8 dummy3;
}GT_MYSTATUS;


//--------------------------------------------------------------
/**
 * �ΐ�^��p�ɃJ�X�^�}�C�Y���ꂽ�|�P�����p�����[�^
 * 112byte
 */
//--------------------------------------------------------------
typedef struct{
	u32	personal_rnd;							//04h	������
	u16	pp_fast_mode		:1;					//06h	�Í��^�����^�`�F�b�N�T����������񂵂ɂ��āA���������������[�h
	u16	ppp_fast_mode		:1;					//06h	�Í��^�����^�`�F�b�N�T����������񂵂ɂ��āA���������������[�h
	u16	fusei_tamago_flag	:1;					//06h	�_���^�}�S�t���O
	u16						:13;				//06h
	
	u16	monsno;								//02h	�����X�^�[�i���o�[		
	u16 item;								//04h	�����A�C�e���i���o�[	
	u32	id_no;								//08h	IDNo
	u32	exp;								//0ch	�o���l
	u8	friendshipness;						//0dh	�Ȃ��x
	u8	speabino;							//0eh	����\��
	u8	hp_exp;								//11h	HP�w�͒l
	u8	pow_exp;							//12h	�U���͓w�͒l
	u8	def_exp;							//13h	�h��͓w�͒l
	u8	agi_exp;							//14h	�f�����w�͒l
	u8	spepow_exp;							//15h	���U�w�͒l
	u8	spedef_exp;							//16h	���h�w�͒l

	u16	waza[GT_WAZA_TEMOTI_MAX];				//08h	�����Z
	u8	pp[GT_WAZA_TEMOTI_MAX];				//0ch	�����ZPP
	u8	pp_count[GT_WAZA_TEMOTI_MAX];			//10h	�����ZPP_COUNT
	u32	hp_rnd			:5;					//		HP����
	u32	pow_rnd			:5;					//		�U���͗���
	u32	def_rnd			:5;					//		�h��͗���
	u32	agi_rnd			:5;					//		�f��������
	u32	spepow_rnd		:5;					//		���U����
	u32	spedef_rnd		:5;					//		���h����
	u32	tamago_flag		:1;					//		�^�}�S�t���O�i0:�^�}�S����Ȃ��@1:�^�}�S����j
	u32	nickname_flag	:1;					//14h	�j�b�N�l�[�����������ǂ����t���O
	u8	event_get_flag	:1;					//		�C�x���g�Ŕz�z�������Ƃ������t���O
	u8	sex				:2;					//   	�|�P�����̐���
	u8	form_no			:5;					//19h	�`��i���o�[
	
	GT_STRCODE	nickname[GT_MONS_NAME_SIZE+GT_EOM_SIZE];	//16h	�j�b�N�l�[��
	
	GT_STRCODE	oyaname[GT_PERSON_NAME_SIZE+GT_EOM_SIZE];	//10h	�e�̖��O
	u8	get_ball;								//1ch	�߂܂����{�[��
	u8	language;								// ����R�[�h
	
	u32			condition;							//04h	�R���f�B�V����
	u8			level;								//05h	���x��
	u8			cb_id;								//06h	�J�X�^���{�[��ID
	u16			hp;									//08h	HP
	u16			hpmax;								//0ah	HPMAX
	u16			pow;								//0ch	�U����
	u16			def;								//0eh	�h���
	u16			agi;								//10h	�f����
	u16			spepow;								//12h	���U
	u16			spedef;								//14h	���h
	
} GT_REC_POKEPARA;

//--------------------------------------------------------------
/**
 *	�퓬�^��p�ɃJ�X�^�}�C�Y���ꂽPOKEPARTY
 *	676 byte
 */
//--------------------------------------------------------------
typedef struct{
	///	�ێ��ł���|�P�������̍ő�
	u16 PokeCountMax;
	///	���ݕێ����Ă���|�P������
	u16 PokeCount;
	///	�|�P�����f�[�^
	GT_REC_POKEPARA member[GT_TEMOTI_POKEMAX];
}GT_REC_POKEPARTY;

///<�ΐ�^��p�f�[�^�\���̐錾
typedef u8 GT_REC_DATA;

///<�ΐ�^��p���[�N�\���̐錾
//	4096byte
typedef struct
{
	GT_REC_DATA	rec_buffer[GT_CLIENT_MAX][GT_REC_BUFFER_SIZE];
}GT_RECORD_PARAM;


//---------------------------------------------------------------------------
/**
 * @brief	�ݒ�f�[�^�\���̒�`
 *			2byte
 */
//---------------------------------------------------------------------------
typedef struct {
	u16 msg_speed:4;			///<MSGSPEED		�l�r�f����̑��x
	u16 sound_mode:2;			///<SOUNDMODE		�T�E���h�o��
	u16 battle_rule:1;			///<BATTLERULE		�퓬���[��
	u16 wazaeff_mode:1;			///<WAZAEFF_MODE	�킴�G�t�F�N�g
	u16 input_mode:2;			///<INPUTMODE		���̓��[�h
	u16 window_type:5;			///<WINTYPE			�E�B���h�E�^�C�v
	u16 dummy:1;
}GT_CONFIG;

//--------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�f�[�^�p�����[�^
 *			52byte
 */
//--------------------------------------------------------------
typedef struct{
	u8			data_type;					//�f�[�^�^�C�v
	u8			tr_type;					//�g���[�i�[����
	u8			tr_gra;						//�g���[�i�[�O���t�B�b�N
	u8			poke_count;					//�����|�P������

	u16			use_item[4];				//�g�p����

	u32			aibit;						//AI�p�^�[��
	u32			fight_type;					//�퓬�^�C�v�i1vs1or2vs2�j

	GT_STRCODE		name[GT_PERSON_NAME_SIZE+GT_EOM_SIZE];	//�g���[�i�[��
	GT_PMS_DATA	win_word;
	GT_PMS_DATA	lose_word;
}GT_TRAINER_DATA;

//--------------------------------------------------------------
/**
 * @brief   �퓬�J�n�p�����[�^
 *			336 byte
 */
//--------------------------------------------------------------
typedef struct{
	u32					fight_type;					///<�퓬��ʃt���O
	int					win_lose_flag;				///<���������t���O
	int					trainer_id[GT_CLIENT_MAX];		///<�g���[�i�[��ID
	GT_TRAINER_DATA		trainer_data[GT_CLIENT_MAX];	///<�g���[�i�[�f�[�^
	int					bg_id;						///<�w�iID
	int					ground_id;					///<�n�`ID
	int					place_id;					///<�n�於ID
	int					zone_id;					///<�]�[��ID
	int					time_zone;					///<���ԑђ�`
	int					shinka_place_mode;			///<�ꏊ�i�����iSHINKA_PLACE�`�j
	int					contest_see_flag;			///<�R���e�X�g���������ǂ����̃t���O
	int					mizuki_flag;				///<�~�Y�L�ɂ��������ǂ����̃t���O
	int					get_pokemon_client;			///<�ߊl�����|�P������ClientNo�iPOKEPARTY_ENEMY��POKEPARTY_ENEMY_PAIR�j
	int					weather;					///<�V��
	int					level_up_flag;				///<���x���A�b�v�����|�P�����t���O�i�i���`�F�b�N������j
	u32					server_version[GT_CLIENT_MAX];	///<�퓬�T�[�o�v���O�����̃o�[�W�����i�ʐM�ΐ펞�Ɏg�p�j
	u32					battle_status_flag;			///<�퓬������p�t���O
	int					safari_ball;				///<�T�t�@���{�[���̐�
	u32					regulation_flag;			///<���M�����[�V�����t���O
	u32					rand;						///<�퓬��p�����̎�
	int					comm_stand_no[GT_CLIENT_MAX];	///<�ʐM�ΐ핔���ł̎����̗����ʒu
	u16					comm_id;					///<�����̒ʐMID
	u16					dummy;						///<�p�f�B���O
	int					total_turn;					///<�퓬�ɂ��������^�[����
	u8					voice_waza_param[GT_CLIENT_MAX];	///<�؃��b�v�{�C�X�̂�����ׂ莞�̃p�����[�^�i�^��f�[�^�Đ����Ɏg�p�j
}GT_REC_BATTLE_PARAM;

//----------------------------------------------------------
/**
 * @brief	�o�g�����M�����[�V�����f�[�^�^��`		32byte
 */
//----------------------------------------------------------
typedef struct {
	GT_STRCODE cupName[GT_REGULATION_NAME_SIZE + GT_EOM_SIZE];	//24
	u16 totalLevel;
	u8 num;
	u8 level;
    s8 height;
    s8 weight;
    u8 evolution:1;    //  
    u8 bLegend:1;
    u8 bBothMonster:1;
    u8 bBothItem:1;
    u8 bFixDamage:1;
    u8 padding;
}GT_REGULATION;


#endif	//__GT_GDS_BATTLE_REC_SUB_H__


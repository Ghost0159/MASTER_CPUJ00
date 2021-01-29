//==============================================================================
/**
 * @file	balloon_types.h
 * @brief	���D�~�j�Q�[���̔ėp��`
 * @author	matsuda
 * @date	2007.11.27(��)
 */
//==============================================================================
#ifndef __BALLOON_GAME_TYPES_H__
#define __BALLOON_GAME_TYPES_H__


//==============================================================================
//	�萔��`
//==============================================================================
enum{
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP,
	DIR_RIGHT,
};

///�����ɏo�����C�A�N�^�[�ő吔(�S�v���C���[�̍��v)
#define PLAYER_AIR_PARAM_MAX			(40)

///�����ɏo���镗�D�A�C�R���A�N�^�[�ő吔
#define ICON_BALLOON_PARAM_MAX			(5)

///�j��G�t�F�N�g��\�����Ă��鎞��
#define EXPLODED_ANM_TIME				(30)

///�J�E���^�[�̌���
enum{
	BALLOON_COUNTER_0,
	BALLOON_COUNTER_1,
	BALLOON_COUNTER_MAX,
};
///�J�E���^�[�̌���
#define BALLOON_COUNTER_KETA_MAX		(6)


///��������1�Ώۖ�
#define BLD_PLANE_1		(GX_BLEND_PLANEMASK_BG3)
///��������2�Ώۖ�
#define BLD_PLANE_2		(GX_BLEND_BGALL | GX_BLEND_PLANEMASK_OBJ)
///��1�Ώۖʂɑ΂��郿�u�����f�B���O�W��
#define BLD_ALPHA_1		13//(0x1b)	//(9)
///��2�Ώۖʂɑ΂��郿�u�����f�B���O�W��
#define BLD_ALPHA_2		(3)

//--------------------------------------------------------------
//	BMP�E�B���h�E
//--------------------------------------------------------------
enum{
	BALLOON_BMPWIN_TALK,		///<��b�E�B���h�E

	BALLOON_BMPWIN_NAME_1,		///<�v���C���[��
	BALLOON_BMPWIN_NAME_2,		///<�v���C���[��
	BALLOON_BMPWIN_NAME_3,		///<�v���C���[��
	
	BALLOON_BMPWIN_SUB_TALK,	///<�T�u��ʗp��b�E�B���h�E

	BALLOON_BMPWIN_MAX,
};

//--------------------------------------------------------------
//	�A�N�^�[�\�t�g�v���C�I���e�B
//--------------------------------------------------------------
enum{
	BALLOON_SOFTPRI_START = 10,
	
	BALLOON_SOFTPRI_TOUCH_PEN,
	
	BALLOON_SOFTPRI_COUNTER,
	BALLOON_SOFTPRI_COUNTER_WIN,
	
	BALLOON_SOFTPRI_AIM,
};

//--------------------------------------------------------------
//	�T�u��ʂ̃A�N�^�[�\�t�g�v���C�I���e�B
//--------------------------------------------------------------
enum{
	BALLOONSUB_SOFTPRI_START = 10,
	
	BALLOONSUB_SOFTPRI_ICON_BALLOON,
	BALLOONSUB_SOFTPRI_EXPLODED_STORM,
	BALLOONSUB_SOFTPRI_EXPLODED_CHIP,
	BALLOONSUB_SOFTPRI_EXPLODED_SMOKE,

	BALLOONSUB_SOFTPRI_BOOSTER_HIT,
	BALLOONSUB_SOFTPRI_BOOSTER,
	BALLOONSUB_SOFTPRI_BOOSTER_LAND_SMOKE,
	BALLOONSUB_SOFTPRI_BOOSTER_SHADOW,
	BALLOONSUB_SOFTPRI_JOINT,
	BALLOONSUB_SOFTPRI_AIR,
};

//--------------------------------------------------------------
//	BG�v���C�I���e�B
//--------------------------------------------------------------
///�^�b�`�y���A�N�^�[��BG�v���C�I���e�B
#define BALLOON_BGPRI_TOUCH_PEN			(0)

///�J�E���^�[�A�N�^�[��BG�v���C�I���e�B
#define BALLOON_BGPRI_COUNTER			(3)
///�J�E���^�[�A�N�^�[WindowOFF���̃_�~�[�A�N�^�[BG�v���C�I���e�B
#define BALLOON_BGPRI_DUMMY_COUNTER		(1)

///�Ə��A�N�^�[��BG�v���C�I���e�B
#define BALLOON_BGPRI_AIM				(1)

//--------------------------------------------------------------
//	�T�u��ʂ�BG�v���C�I���e�B
//--------------------------------------------------------------
///��C�A�N�^�[��BG�v���C�I���e�B
#define BALLOONSUB_BGPRI_AIR			(3)
///���D�j��A�N�^�[��BG�v���C�I���e�B
#define BALLOONSUB_BGPRI_EXPLODED		(1)
///���D�A�C�R���A�N�^�[��BG�v���C�I���e�B
#define BALLOONSUB_BGPRI_ICON_BALLOON	(1)
///�W���C���g�A�N�^�[��BG�v���C�I���e�B
#define BALLOONSUB_BGPRI_JOINT			(3)
///�u�[�X�^�[�A�N�^�[��BG�v���C�I���e�B
#define BALLOONSUB_BGPRI_BOOSTER		(3)


//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///�����ɏo�����C�̍ő吔
#define PIPE_AIR_AIR_MAX		(8)

///��C���f���̃^�C�v
enum{
	PIPE_3D_AIR_TYPE_SMALL,
	PIPE_3D_AIR_TYPE_MIDDLE,
	PIPE_3D_AIR_TYPE_BIG,
	PIPE_3D_AIR_TYPE_SPECIAL,	///<�\�[�i���X���ł����Ȃ����Ƃ��ɉ������񂾂Ƃ�
	
	PIPE_3D_AIR_TYPE_MAX,
};

//--------------------------------------------------------------
//	���D�A�C�R��
//--------------------------------------------------------------
///���D�A�C�R���̏��
enum{
	ICON_BALLOON_STATUS_NULL,			///<��������Ă��Ȃ�
	ICON_BALLOON_STATUS_NORMAL,			///<�ʏ���
	ICON_BALLOON_STATUS_PINCH,			///<���D���j�􂵂����ȏ��
	ICON_BALLOON_STATUS_EXPLODED,		///<������
};

//--------------------------------------------------------------
//	�J�E���g�_�E��
//--------------------------------------------------------------
enum{
	BALLOON_COUNTDOWN_NULL,				///<�������
	BALLOON_COUNTDOWN_TOUCHPEN_DEMO_INIT,	///<�^�b�`�y���f��
	BALLOON_COUNTDOWN_TOUCHPEN_DEMO,	///<�^�b�`�y���f��
	BALLOON_COUNTDOWN_START,			///<�J�E���g�_�E���J�n
	BALLOON_COUNTDOWN_START_WAIT,		///<�I���҂����
	BALLOON_COUNTDOWN_START_END,		///<�J�n�J�E���g�_�E���I��
	BALLOON_COUNTDOWN_TIMEUP,			///<�^�C���A�b�v�J�n
	BALLOON_COUNTDOWN_TIMEUP_WAIT,		///<�^�C���A�b�v�҂����
	BALLOON_COUNTDOWN_TIMEUP_END,		///<�^�C���A�b�v�I��
};

//--------------------------------------------------------------
//	�W���C���g�ԍ�
//--------------------------------------------------------------
///�W���C���g�̃A�N�^�[�ԍ�
enum{
	JOINT_ACTOR_D,		//��
	JOINT_ACTOR_U,		//��
	JOINT_ACTOR_L,		//��
	JOINT_ACTOR_R,		//�E
	
	JOINT_ACTOR_MAX,
};

//--------------------------------------------------------------
//	�u�[�X�^�[�ԍ�
//--------------------------------------------------------------
enum{
	BOOSTER_ACTOR_NORMAL,	//�ʏ�
	BOOSTER_ACTOR_WEAK,		//�����u�[�X�^�[
	BOOSTER_ACTOR_HARD,		//3�{�u�[�X�^�[
	
	BOOSTER_ACTOR_MAX,
};

///�����ɏo����SIO�u�[�X�^�[�̐�
#define SIO_BOOSTER_ACTOR_MAX		(3)

//--------------------------------------------------------------
//	�u�[�X�^�[�^�C�v
//--------------------------------------------------------------
enum{
	BOOSTER_TYPE_NONE,		//�Ȃ�
	BOOSTER_TYPE_NORMAL,	//�ʏ�(2�{)
	BOOSTER_TYPE_WEAK,		//�����u�[�X�^�[
	BOOSTER_TYPE_HARD,		//3�{�u�[�X�^�[
};

//--------------------------------------------------------------
//	�u�[�X�^�[���n�̉�
//--------------------------------------------------------------
///�u�[�X�^�[���n�ŏo�鉌�̐�
#define BOOSTER_LAND_SMOKE_NUM				(2)
///�u�[�X�^�[���n�ŏo�鉌�̎���
#define BOOSTER_LAND_SMOKE_DELETE_TIME		(8)
///�u�[�X�^�[���n���̃A�j�����
#define BOOSTER_LAND_SMOKE_ANM_PATERN		(2)

//--------------------------------------------------------------
//	����
//--------------------------------------------------------------
///�����̎��ӂԂ��Ŏg�p����A�N�^�[��
#define EXPLODED_STORM_ACTOR_MAX		(36)	//(40)
///�����̉��Ŏg�p����A�N�^�[��
#define EXPLODED_SMOKE_ACTOR_MAX		(8)//(12)
///�����̕��D�̔j�ЂŎg�p����A�N�^�[��
#define EXPLODED_CHIP_ACTOR_MAX			(3)//(4)

//--------------------------------------------------------------
//	�J�E���^�[
//--------------------------------------------------------------
///�J�E���^�[�̃t�H���g�J���[�ݒ�
#define COUNTER_FONT_COLOR		(GF_PRINTCOLOR_MAKE(0xe,0xf,0))
///�J�E���^�[X���W��l
#define COUNTER_BASE_X			(8)
///�J�E���^�[X���W�̔z�u�Ԋu
#define COUNTER_X_SPACE			(8)
///�J�E���^�[Y���W
#define COUNTER_Y				(168)

//==============================================================================
//	�^��`
//==============================================================================
///�\�[�i���X���쐧��V�X�e�����[�N�̕s��`�|�C���^
typedef struct _SONANS_SYS * SONANS_SYS_PTR;


//==============================================================================
//	�\���̒�`
//==============================================================================
///��C�A�N�^�[�̈ʒu���̃f�[�^
typedef struct{
	s16 x;				///<��C�o�����WX
	s16 y;				///<��C�o�����WY
	u16 anmseq;			///<�A�j���V�[�P���X�ԍ�
	u8 pal_ofs;			///<�p���b�g�I�t�Z�b�g
	u8 move_dir;		///<�ړ�����
	
	u8 dummy[2];
}AIR_POSITION_DATA;

///�v���C���[���ꂼ�ꂪ������������C�p�����[�^
typedef struct{
	BOOL occ;			///<TRUE:�L���@FALSE:����
	s16 net_id;
	s16 balloon_no;
	s32 air;
	
	s16 wait;
	u8 booster_type;	///<BOOSTER_TYPE_???
	u8 dummy[1];
	
	CATS_ACT_PTR cap;
	const AIR_POSITION_DATA *apd;
}PLAYER_AIR_PARAM; 

///���ӂԂ����[�N
typedef struct{
	CATS_ACT_PTR cap;
	fx32 furihaba;
	fx32 add_furihaba;
	fx32 sec;
	fx32 add_sec;
	s16 delete_wait;
}STORM_WORK;

///�j��A�N�^�[�p�����[�^
typedef struct{
	BOOL occ;			///<TRUE:�����A�j����
	STORM_WORK storm[EXPLODED_STORM_ACTOR_MAX];
	STORM_WORK smoke[EXPLODED_SMOKE_ACTOR_MAX];
	STORM_WORK chip[EXPLODED_CHIP_ACTOR_MAX];
	
	u8 dummy[2];
}EXPLODED_PARAM; 

///���D�A�C�R���A�N�^�[�p�����[�^
typedef struct{
	CATS_ACT_PTR cap;
	u8 type;			///<�A�C�R���̎��(BALLOON_LEVEL_???)
	u8 status;			///<���݂̏��(ICON_BALLOON_STATUS_???)
	u8 pos;				///<�\���ʒu(��ԉE�[����̃I�t�Z�b�g�ԍ�)
	u8 dummy;
	
	s16 balloon_no;		///<�S�����镗�D�ԍ�
	s16 dummy_2;
}ICONBALLOON_PARAM;

///�W���C���g���쐧��
typedef struct{
	CATS_ACT_PTR cap[JOINT_ACTOR_MAX];
}JOINT_WORK;

///�u�[�X�^�[���n���̉��A�N�^�[���쐧��\����
typedef struct{
	CATS_ACT_PTR cap[BOOSTER_LAND_SMOKE_NUM];
	fx32 add_x[BOOSTER_LAND_SMOKE_NUM];
	fx32 add_y[BOOSTER_LAND_SMOKE_NUM];
	u8 seq;
	u8 time;
}BOOSTER_LAND_SMOKE;

///�u�[�X�^�[�̓��샏�[�N
typedef struct{
	CATS_ACT_PTR cap;
	CATS_ACT_PTR hit_cap;		///<�q�b�g�������ɏo���G�t�F�N�g�A�N�^�[
	CATS_ACT_PTR shadow_cap;	///<�e�A�N�^�[
	fx32 start_theta;		///<�X�^�[�g�p�x
	u8 mode;				///<BOOSTER_MODE_???
	u8 seq;
	u8 local_frame;
	u8 local_wait;
	u8 booster_type;		///<�u�[�X�^�[�^�C�v
	u8 hit_ok;				///<TRUE:��C�Ƃ̓����蔻�蔭���\
	s32 local_work;
	s16 local_x;
	s16 local_y;
	fx32 local_add_x;
	fx32 local_add_y;
	fx32 local_fx_x;
	fx32 local_fx_y;
	
	BOOSTER_LAND_SMOKE land_smoke;	///<�u�[�X�^�[���n���̉��A�N�^�[���쐧�䃏�[�N
}BOOSTER_MOVE;

///�u�[�X�^�[���쐧��\����
typedef struct{
	BOOSTER_MOVE move[BOOSTER_ACTOR_MAX];			///<�u�[�X�^�[���샏�[�N
	fx32 theta;				///<���݂̕b�j�p�x
	fx32 add_theta;			///<�b�j�ɉ��Z����p�x
	u32 world_frame;		///<�u�[�X�^�[�����삵�Ă���Œ��A��ɃJ�E���^�A�b�v
	u8 byousin_pos;			///<�b�j�����݂ǂ��������Ă��邩
	u8 stop;				///<TRUE:�u�[�X�^�[�S�Ă̓�����~����
	u8 wait;				///<�E�F�C�g
	u8 frame;				///<�b�j�Ԃ̈ړ��t���[��
	u8 lap;					///<���݂̎���
}BOOSTER_WORK;

///SIO�u�[�X�^�[�̓��샏�[�N
typedef struct{
	CATS_ACT_PTR cap;
	CATS_ACT_PTR hit_cap;	///<�q�b�g�������ɏo���G�t�F�N�g�A�N�^�[
	fx32 local_fx_x;
	fx32 local_fx_y;
	s16 end_y;
	u8 booster_type;
	u8 seq;
	u8 local_frame;
	u8 occ;
	s16 start_wait;
}SIO_BOOSTER_MOVE;

///SIO�u�[�X�^�[���쐧��\����
typedef struct{
	SIO_BOOSTER_MOVE move[SIO_BOOSTER_ACTOR_MAX];			///<�u�[�X�^�[���샏�[�N
}SIO_BOOSTER_WORK;

///�t�H���gOAM�p�̍\���̒�`
typedef struct{
	FONTOAM_OBJ_PTR fontoam;		///<�t�H���gOAM
	CHAR_MANAGER_ALLOCDATA cma;		///<�t�H���gOAM�L�����̈�f�[�^
	u16 len;						///<�t�H���g�̃h�b�g��
}BALLOON_FONTACT;

///�J�E���^�[����\���̒�`
typedef struct{
	CATS_ACT_PTR win_cap;			///<�E�B���h�E�A�N�^�[�̃|�C���^
	BALLOON_FONTACT fontact[BALLOON_COUNTER_KETA_MAX][BALLOON_COUNTER_MAX];	///<�t�H���g�A�N�^�[(�J�E���^�[)
	BALLOON_FONTACT fontact_cc;		///<�t�H���g�A�N�^�[(CC)
	BALLOON_FONTACT fontact_dummy[BALLOON_COUNTER_KETA_MAX];	///<�t�H���g�A�N�^�[(�J�E���^�[�_�~�[)
	
	u8 number[BALLOON_COUNTER_KETA_MAX];		///<�X�N���[���h�b�g�P��(���ݒl)
	u8 last_number[BALLOON_COUNTER_KETA_MAX];	///<�X�N���[���h�b�g�P��(��~�l)
	u8 next_number[BALLOON_COUNTER_KETA_MAX];	///<�X�N���[���h�b�g�P��(���N�G�X�g�l)
	u8 rotate_count[BALLOON_COUNTER_KETA_MAX];	///<�����������J�E���g
	s8 move_wait[BALLOON_COUNTER_KETA_MAX];		///<����J�n�O�̃E�F�C�g
	s8 wait;
	u8 move_keta;								///<�����܂ŉ�]���삵�Ă��邩
	u8 seq;
}BALLOON_COUNTER;

///�^�b�`�y������\���̒�`
typedef struct{
	CATS_ACT_PTR cap;				///<�^�b�`�y���A�N�^�[�̃|�C���^
	s16 seq;
	s16 wait;
	s16 x;
	s16 y;
	int tp_x;						///<�^���^�b�`�y�����W
	int tp_y;
	int tp_cont;
	int tp_trg;
	BOOL demo_end;					///<TRUE:�f���I��
}BALLOON_PEN;

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�Ɍq�����Ă���p�C�v�Ƌ�C�̓��쐧��\����
 */
//--------------------------------------------------------------
typedef struct{
	D3DOBJ_MDL	pipe_mdl;		///<�p�C�v���f��
   	D3DOBJ		pipe_obj;		///<�p�C�vOBJ
   	
	D3DOBJ_MDL	air_mdl[PIPE_3D_AIR_TYPE_MAX];	///<��C���f��
	struct{
		int occ;				///<TRUE:�f�[�^����������Ă���
		D3DOBJ obj;				///<��COBJ
		D3DOBJ_ANM	anm;		///<��C�A�j��
	}air[PIPE_AIR_AIR_MAX][PIPE_3D_AIR_TYPE_MAX];
}PIPE_AIR_WORK;

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�Ɍq�����Ă������̓��쐧��\����
 */
//--------------------------------------------------------------
typedef struct{
	D3DOBJ_MDL	mdl;		///<������f��
   	D3DOBJ		obj;		///<���OBJ
}DAIZA_WORK;

//--------------------------------------------------------------
/**
 * @brief   �T�[�o�[�p�̃V�X�e�����[�N
 */
//--------------------------------------------------------------
typedef struct{
	int seq;
	int work;
	int wait;

	int local_seq;
	int local_work;
	int local_wait;
	
	u8 req_timing_no;		///<�������悤�ɑS���ɔ��s���������ԍ�
	u8 dummy[3];
}BALLOON_SERVER_WORK;

//--------------------------------------------------------------
/**
 * @brief   ���D����Q�[�����䃁�C���\����
 */
//--------------------------------------------------------------
typedef struct _BALLOON_GAME_WORK{
	BALLOON_SYSTEM_WORK *bsw;		///<���D����V�X�e�����[�N�ւ̃|�C���^
	int server_netid;				///<�T�[�o�[�S���̃l�b�gID
	u32 recv_server_version_cnt;	///<�T�[�o�[�o�[�W�����̎�M����
	u32 recv_server_version[BALLOON_PLAYER_MAX];	///��M�����T�[�o�[�o�[�W����
	
	MNGM_COUNTWK *mgcount;			///<�~�j�Q�[�����ʃJ�E���g�_�E���G�t�F�N�g�V�X�e���ւ̃|�C���^
	int countdown_eff;				///<�J�E���g�_�E���G�t�F�N�g�̌��݂̏��
	
	CATS_SYS_PTR		csp;
	CATS_RES_PTR		crp;
	GF_BGL_INI *bgl;
	GF_BGL_BMPWIN win[BALLOON_BMPWIN_MAX];
	MSGDATA_MANAGER *msgman;		///<���b�Z�[�W�f�[�^�}�l�[�W���̃|�C���^
	WORDSET *wordset;				///<Alloc�������b�Z�[�W�p�P��o�b�t�@�ւ̃|�C���^
	STRBUF *msg_buf;				///<Alloc����������o�b�t�@�ւ̃|�C���^
	PALETTE_FADE_PTR pfd;			///<�p���b�g�t�F�[�h�V�X�e���ւ̃|�C���^
	FONTOAM_SYS_PTR fontoam_sys;	///<FONTOAM�V�X�e��

	TCB_PTR update_tcb;
	GF_G3DMAN *g3Dman;

	GF_CAMERA_PTR camera;			///<�J�����ւ̃|�C���^
	GF_CAMERA_PTR camera_3d;		///<3D���f���p�J�����ւ̃|�C���^
	
	PTC_PTR ptc;					///<�p�[�e�B�N���V�X�e���ւ̃|�C���^

    // �A���P�[�^
    NNSFndAllocator allocator;
	
	BALLOON_SERVER_WORK server_work;	///<�T�[�o�[�p�̃V�X�e�����[�N
	
	SONANS_SYS_PTR sns;				///<�\�[�i���X���쐧��V�X�e�����[�N�ւ̃|�C���^
	
	PIPE_AIR_WORK pipe_air;			///<�\�[�i���X�Ɍq����p�C�v�Ƌ�C3D����\����
	DAIZA_WORK daiza;				///<�������\����
	
	BALLOON_STATUS bst;				///<���D�X�e�[�^�X

	JOINT_WORK joint;				///<�W���C���g���䃏�[�N
	BOOSTER_WORK booster;				///<�u�[�X�^�[���쐧�䃏�[�N
	SIO_BOOSTER_WORK sio_booster;		///<SIO�u�[�X�^�[���쐧�䃏�[�N
	
	BALLOON_COUNTER counter;		///<�J�E���^�[���쐧�䃏�[�N
	BALLOON_PEN pen;				///<�^�b�`�y�����쐧�䃏�[�N
	
	BALLOON_SIO_BUFFER send_buf;	///<���M�o�b�t�@
	BALLOON_SIO_BUFFER recv_buf[BALLOON_PLAYER_MAX];	///<��M�o�b�t�@
	BALLOON_SIO_PLAY_WORK send_important;		///<�d�v�f�[�^��p���M�o�b�t�@(�e�݂̂��g�p)
	BALLOON_SIO_PLAY_WORK recv_important;		///<�d�v�f�[�^��p��M�o�b�t�@
	int recv_write_ring_no[BALLOON_PLAYER_MAX];	///<��M�p�����O�o�b�t�@�̏������݈ʒu
	int recv_read_ring_no[BALLOON_PLAYER_MAX];	///<��M�p�����O�o�b�t�@�̓ǂݍ��݈ʒu
	int send_write_ring_no;						///<���M�p�����O�o�b�t�@�̏������݈ʒu
	int send_read_ring_no;						///<���M�p�����O�o�b�t�@�̓ǂݍ��݈ʒu
	BALLOON_AIR_DATA send_airdata_stack[PIPE_AIR_AIR_MAX];	///<��C�f�[�^�X�^�b�N
	int send_airdata_read_no;					///<��C�f�[�^�X�^�b�N�Q�Ɣԍ�(�ǂݏo��)
	int send_airdata_write_no;					///<��C�f�[�^�X�^�b�N�Q�Ɣԍ�(��������)
	
	u32 time;					///<�Q�[���v���C�^�C��
	int game_start;				///<TRUE:�Q�[���J�n
	int game_finish;			///<TRUE:�Q�[���I��(�I�����o�ֈڍs)
	int game_end;				///<TRUE:�Q�[���I��(��ʈÓ]�ֈڍs)
	int sio_wait;				///<Wifi���ב΍��p�Ɉ�x���M����ƈ����ԑ��M�o���Ȃ��悤�ɂ���
	int balloon_no;				///<���݂̕��D�ԍ�
	int balloon_occ;			///<TRUE�F���D����������Ă���
	int exploded_count;			///<���D��j�􂳂�����
	
	int timing_req;				///<������胊�N�G�X�g�ԍ�
	int timing_no;				///<�����ԍ�
	
	PLAYER_AIR_PARAM air_param[PLAYER_AIR_PARAM_MAX];	///<��C�A�N�^�[�p�����[�^
	EXPLODED_PARAM exploded_param;		///<�j��A�N�^�[�p�����[�^
	ICONBALLOON_PARAM iconballoon_param[ICON_BALLOON_PARAM_MAX];	///<���D�A�C�R���p�����[�^
	
	s32 my_total_air;			///<���������ꂽ��C�ʂ̍��v�l
	u32 main_frame;				///<�Q�[���J�n����̃t���[����
}BALLOON_GAME_WORK;


#endif	//__BALLOON_GAME_TYPES_H__

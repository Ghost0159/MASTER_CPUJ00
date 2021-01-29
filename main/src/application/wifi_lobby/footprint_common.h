//==============================================================================
/**
 * @file	footprint_common.h
 * @brief	���Ճ{�[�h���ʃw�b�_
 * @author	matsuda
 * @date	2008.01.18(��)
 */
//==============================================================================
#ifndef __FOOTPRINT_COMMON_H__
#define __FOOTPRINT_COMMON_H__


//==============================================================================
//	�萔��`
//==============================================================================

///���݂̃Q�[���̏��
enum{
	FOOTPRINT_GAME_STATUS_INIT,			///<������������
	FOOTPRINT_GAME_STATUS_NORMAL,			///<�ʏ�
	FOOTPRINT_GAME_STATUS_EXIT_SELECT,	///<�I���I��
	FOOTPRINT_GAME_STATUS_FINISH,			///<�I��������
};

//--------------------------------------------------------------
//	BG�t���[��
//--------------------------------------------------------------
///�t���[���w��
enum{
	//���C�����
	FOOT_FRAME_WIN = GF_BGL_FRAME1_M,			//�E�B���h�E��
	FOOT_FRAME_PANEL = GF_BGL_FRAME2_M,			//�p�l��
	FOOT_FRAME_BG = GF_BGL_FRAME3_M,			//�w�i
	
	//�T�u���
	FOOT_SUBFRAME_WIN = GF_BGL_FRAME1_S,		//�E�B���h�E��
	FOOT_SUBFRAME_PLATE = GF_BGL_FRAME2_S,			//�v���[�g
	FOOT_SUBFRAME_BG = GF_BGL_FRAME3_S,			//�w�i
};

//--------------------------------------------------------------
//	BG�v���C�I���e�B
//--------------------------------------------------------------
enum{
	FOOT_BGPRI_3D = 2,		//3D��BG�v���C�I���e�B
	
	FOOT_BGPRI_WIN = 0,		//�p�l����BG�v���C�I���e�B
	FOOT_BGPRI_PANEL = 0,		//�p�l����BG�v���C�I���e�B
	FOOT_BGPRI_BG = 3,			//�w�i��BG�v���C�I���e�B

	FOOT_SUBBGPRI_WIN = 2,		//�p�l����BG�v���C�I���e�B
	FOOT_SUBBGPRI_PLATE = 3,	//�v���[�g��BG�v���C�I���e�B
	FOOT_SUBBGPRI_BG = 3,		//�w�i��BG�v���C�I���e�B
};

//--------------------------------------------------------------
//	�A�N�^�[BG�v���C�I���e�B
//--------------------------------------------------------------
enum{
	ACTBGPRI_INK = 1,				///<�C���N
	ACTBGPRI_INK_FOOT = 1,			///<�C���N�̏�ɔz�u���鑫��
	ACTBGPRI_INK_FOUNDATION = 1,	///<�C���N�̉��n
	
	ACTBGPRI_TOUCH_EFF = 0,			///<�C���N���^�b�`�������ɏo���G�t�F�N�g
	ACTBGPRI_EXIT_FONT = 0,			///<�u��߂�v
	
	//-- �T�u��� --//
	ACTBGPRI_SUB_NAME_FRAME = 1,	///<���O���͂ޘg
	ACTBGPRI_SUB_NAME_FOOT = 1,		///<���O�̉��ɔz�u���鑫��
};

//--------------------------------------------------------------
//	�A�N�^�[�\�t�g�v���C�I���e�B
//--------------------------------------------------------------
enum{
	SOFTPRI_TOUCH_EFF = 50,
	SOFTPRI_EXIT_FONT,
	SOFTPRI_INK_FOOT,
	SOFTPRI_INK,
	SOFTPRI_INK_FOUNDATION,
	
	//-- �T�u��� --//
	SOFTPRI_SUB_NAME_FRAME = 50,
	SOFTPRI_SUB_NAME_FOOT,
};

//--------------------------------------------------------------
//	�p���b�g�W�J�ʒu
//--------------------------------------------------------------
///�{�^���́u�͂��E�������v�p���b�g�W�J�ʒu(���C�����)
#define FOOT_MAINBG_BUTTON_YESNO_PAL	(11)	//2�{�g�p
///��b�t�H���g�p���b�g�W�J�ʒu(���C�����)
#define FOOT_MAINBG_TALKFONT_PAL	 (13)
///��b�E�B���h�E�p���b�g�W�J�ʒu(���C�����)
#define FOOT_MAINBG_TALKWIN_PAL		(14)

///��b�t�H���g�p���b�g�W�J�ʒu(�T�u���)
#define FOOT_SUBBG_TALKFONT_PAL		 (13)

//--------------------------------------------------------------
//	���b�Z�[�WCGX�W�J�ʒu
//--------------------------------------------------------------
///��b�E�B���h�E�̃L�����N�^�W�J�J�n�ʒu
#define WINCGX_TALKWIN_START	(1)
///�{�^���́u�͂��E�������v�W�J�J�n�ʒu
#define WINCGX_BUTTON_YESNO_START	(WINCGX_TALKWIN_START + TALK_WIN_CGX_SIZ)
///��b�E�B���h�E���̃��b�Z�[�W�W�J�J�n�ʒu
#define WINCGX_MESSAGE_START	(WINCGX_BUTTON_YESNO_START + TOUCH_SW_USE_CHAR_NUM)

//--------------------------------------------------------------
//	�T�u��ʁF���b�Z�[�WCGX�W�J�ʒu
//--------------------------------------------------------------
///���O�\�����b�Z�[�W�̃L�����N�^�W�J�J�n�ʒu
#define WINCGX_SUB_NAME_START		(0x4000 / 0x20)
///���O�\��BMPWIN�̃T�C�YX(�L�����P��)
#define WINCGX_SUB_NAME_SIZE_X		(8)
///���O�\��BMPWIN�̃T�C�YY(�L�����P��)
#define WINCGX_SUB_NAME_SIZE_Y		(2)
///���O�\����BMPWIN��
#define FOOTPRINT_BMPWIN_NAME_MAX		(8)

//--------------------------------------------------------------
//	�A�N�^�[
//--------------------------------------------------------------
///�T�u�T�[�t�F�[�XY(fx32)
#define FOOTPRINT_SUB_ACTOR_DISTANCE		((192 + 80) << FX32_SHIFT)
///�T�u�T�[�t�F�[�XY(int)
#define FOOTPRINT_SUB_ACTOR_DISTANCE_INTEGER		(FOOTPRINT_SUB_ACTOR_DISTANCE >> FX32_SHIFT)

//--------------------------------------------------------------
//	�V�X�e��
//--------------------------------------------------------------
///���Ճ{�[�h�ɎQ���ł���ő�l��
#define FOOTPRINT_ENTRY_MAX		(8)

///�X�^���v�̏o����ő吔
#define STAMP_MAX		(128)

///�X�^���v�e�N�X�`����VWait�]�����N�G�X�g�o�b�t�@�̍ő吔
#define LOAD_TEX_BUFFER_MAX		(32)


//==============================================================================
//	�^��`
//==============================================================================
///FOOTPRINT_SYS�̕s��`�|�C���^
typedef struct _FOOTPRINT_SYS * FOOTPRINT_SYS_PTR;

///STAMP_MOVE_WORK�̕s��`�|�C���^
typedef struct _STAMP_MOVE_WORK * STAMP_MOVE_PTR;


//==============================================================================
//	�\���̒�`
//==============================================================================
//--------------------------------------------------------------
//	�}�C�f�[�^
//--------------------------------------------------------------
///�����̒ʐM�X�e�[�^�X
typedef struct{
	BOOL ready;							///<TRUE:�ʐM��M��t�������o���Ă�����
	s32 user_id;						///<���[�U�[ID
}FOOTPRINT_MY_COMM_STATUS;

//--------------------------------------------------------------
//	���ՃX�^���v
//--------------------------------------------------------------
///���ՃX�^���v�̃p�����[�^(���ꂪ�ʐM�ő���M�����)
typedef struct{
	u16 color;				///<�X�^���v�̃J���[�R�[�h
	u16 monsno;				///<�|�P�����ԍ�
	u32 personal_rnd;		///<������(���ʁA���i)
	u8 form_no;				///<�t�H�����ԍ�
	u8 x;					///<���Ս��WX(3D���W�ł͂Ȃ��^�b�`�p�l����̍��W)
	u8 y;					///<���Ս��WY(3D���W�ł͂Ȃ��^�b�`�p�l����̍��W)
	
	u8 padding;
}STAMP_PARAM;

//--------------------------------------------------------------
//	�X�^���v�̘A���񐔂��Ǘ�����\����
//--------------------------------------------------------------
///�X�^���v�̘A���񐔂��Ǘ�����\����
typedef struct{
	u8 chain;			///<�`�F�C����
	u8 stamp_num;		///<�����̃X�^���v�����̃��[�N���Q�Ƃ��Ă��邩
	
	u16 padding;
}STAMP_CHAIN_WORK;

//--------------------------------------------------------------
//	�X�y�V�����G�t�F�N�g����\����
//--------------------------------------------------------------
///�X�y�V�����G�t�F�N�g�F�ɂ���
typedef struct{
	fx32 default_len;
	s16 wait;
	u8 seq;			///<�V�[�P���X�ԍ�
	
	u8 padding;
}SPECIAL_EFF_NIJIMI;

///�X�y�V�����G�t�F�N�g�F�͂���
typedef struct{
	VecFx32 up_vec;		///<�J�����̏����
	u8 seq;
	u8 count;
	u8 loop;
	
	u8 padding;
}SPECIAL_EFF_HAJIKE;

///�X�y�V�����G�t�F�N�g�F�W�O�U�O
typedef struct{
	CAMERA_ANGLE default_angle;
	s32 calc_angle_y;	///<���ݒn�̌v�Z�ɕ֗��Ȃ悤�Ƀ}�C�i�X��������A���O����p�ӂ��Ă���
	s16 wait;
	u8 seq;
	
	u8 padding;
}SPECIAL_EFF_ZIGZAG;

///�X�y�V�����G�t�F�N�g�F�֍s
typedef struct{
	CAMERA_ANGLE default_angle;
	s32 calc_angle_y;	///<���ݒn�̌v�Z�ɕ֗��Ȃ悤�Ƀ}�C�i�X��������A���O����p�ӂ��Ă���
	s16 wait;
	u8 seq;
	
	u8 padding;
}SPECIAL_EFF_DAKOU;

///�X�y�V�����G�t�F�N�g�F�O��
typedef struct{
	CAMERA_ANGLE default_angle;
	s32 calc_angle_x;	///<���ݒn�̌v�Z�ɕ֗��Ȃ悤�Ƀ}�C�i�X��������A���O����p�ӂ��Ă���
	s16 wait;
	u8 seq;
	
	u8 padding;
}SPECIAL_EFF_KISEKI;

///�X�y�V�����G�t�F�N�g�F�h��
typedef struct{
	CAMERA_ANGLE default_angle;
	fx32 theta;
	u8 loop;
	u8 seq;
	
	u8 padding[2];
}SPECIAL_EFF_YURE;

///�X�y�V�����G�t�F�N�g�F�g��
typedef struct{
	fx32 default_len;
	s16 wait;
	u8 seq;			///<�V�[�P���X�ԍ�
	
	u8 padding;
}SPECIAL_EFF_KAKUDAI;

///�X�y�V�����G�t�F�N�g�F�u���[X
typedef struct{
	VecFx32 default_target;
	VecFx32 default_pos;
	fx32 theta;
	u8 loop;
	u8 seq;
	
	u8 padding[2];
}SPECIAL_EFF_BRAR_X;

///�X�y�V�����G�t�F�N�g�F�u���[Y
typedef struct{
	VecFx32 default_target;
	VecFx32 default_pos;
	fx32 theta;
	u8 loop;
	u8 seq;
	
	u8 padding[2];
}SPECIAL_EFF_BRAR_Y;

///�X�y�V�����G�t�F�N�g�F����
typedef struct{
	CAMERA_ANGLE default_angle;
	s32 calc_angle_x;	///<���ݒn�̌v�Z�ɕ֗��Ȃ悤�Ƀ}�C�i�X��������A���O����p�ӂ��Ă���
	s16 wait;
	u8 seq;
	
	u8 padding;
}SPECIAL_EFF_TARE;

///�X�^���v�̃X�y�V�����G�t�F�N�g����\����
typedef struct{
	SPECIAL_EFF_NIJIMI sp_nijimi;
	SPECIAL_EFF_HAJIKE sp_hajike;
	SPECIAL_EFF_ZIGZAG sp_zigzag;
	SPECIAL_EFF_DAKOU sp_dakou;
	SPECIAL_EFF_KISEKI sp_kiseki;
	SPECIAL_EFF_YURE sp_yure;
	SPECIAL_EFF_KAKUDAI sp_kakudai;
	SPECIAL_EFF_BRAR_X sp_brar_x;
	SPECIAL_EFF_BRAR_Y sp_brar_y;
	SPECIAL_EFF_TARE sp_tare;
}STAMP_SPECIAL_WORK;

///�X�y�V�����G�t�F�N�g�����O�̃t���b�V���G�t�F�N�g����\����
typedef struct{
	u8 seq;			///<�V�[�P���X�ԍ�
	u8 loop;		///<���[�v�J�E���^
	s16 evy;		///<EVY�l(����8�r�b�g����)
}SPECIAL_FLASH_WORK;

//--------------------------------------------------------------
/**
 * @brief   ���ՃX�^���v�S�̂��Ǘ�����V�X�e�����[�N
 */
//--------------------------------------------------------------
typedef struct{
	STAMP_SPECIAL_WORK special_work;			///<�X�y�V�����G�t�F�N�g���샏�[�N
	SPECIAL_FLASH_WORK flash_work;				///<�X�y�V���������O�̃t���b�V�����샏�[�N
	STAMP_MOVE_PTR move[STAMP_MAX];
	STAMP_MOVE_PTR hitcheck_move[STAMP_MAX];	///�q�b�g�`�F�b�N�Ώۂ̃X�^���v�ւ̃|�C���^
	NNSG3dResTex *load_tex[LOAD_TEX_BUFFER_MAX];	///<�e�N�X�`��Vwait�]���o�b�t�@
	u32 polygon_id_manage[2];		///<�|���S��ID�Ǘ��r�b�g
	STAMP_CHAIN_WORK chain_work[STAMP_MAX];		///<�X�^���v�̘A���Ǘ����[�N
	u8 special_effect_type;				///<�������̃X�y�V�����G�t�F�N�g�^�C�v
	u8 special_effect_req;				///<�X�y�V�����G�t�F�N�g�^�C�v�̔������N�G�X�g
	u8 special_effect_start_effect_end;			///<TRUE:�X�y�V�����O�̃t���b�V���G�t�F�N�g�I��
	u8 player_max;								///<���݂̎Q���l��
	u8 v_wipe_req;								///<V�u�����N�ł�WIPE�]�����N�G�X�g
	s16 v_wipe_no;								///<V�u�����N�ł�WIPE�]�����̒l
}STAMP_SYSTEM_WORK;

///�R���o�[�^����o�͂����f�[�^�^
typedef struct{
	u8 disp;			///<���Ղ̗L���B(TRUE=�L��)
	u8 hit_size;		///<�q�b�g�͈͂̑傫��
	
	u16 padding;
}FOOTPRINT_CONV_DATA;

//==============================================================================
//	�O���֐��錾
//==============================================================================
extern FOOTPRINT_MY_COMM_STATUS * Footprint_MyCommStatusGet(FOOTPRINT_SYS_PTR fps);
extern BOOL Footprint_StampAdd(FOOTPRINT_SYS_PTR fps, const STAMP_PARAM *param, s32 user_id);
extern void Footprint_NameWrite(FOOTPRINT_SYS_PTR fps, s32 user_id);
extern void Footprint_NameErase(FOOTPRINT_SYS_PTR fps, u32 user_index);
extern void Footprint_WorldWidthHeightGet(FOOTPRINT_SYS_PTR fps, fx32 *width, fx32 *height);


#endif	//__FOOTPRINT_COMMON_H__

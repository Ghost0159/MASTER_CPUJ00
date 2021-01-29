//==============================================================================
/**
 * @file	ending_common.h
 * @brief	�G���f�B���O���ʃw�b�_
 * @author	matsuda
 * @date	2008.04.07(��)
 */
//==============================================================================
#ifndef __ENDING_COMMON_H__
#define __ENDING_COMMON_H__


//==============================================================================
//	�f�o�b�O�p��`
//==============================================================================
#ifdef PM_DEBUG
///�L���ɂ��Ă���ƃV�[������Y�{�^���ŃX�L�b�v�\
//#define DEBUG_ENDING_SCENE_SKIP
///�L���ɂ��Ă���ƃJ������z�u�����������
//#define DEBUG_ENDING_CAMERA
#endif


//--------------------------------------------------------------
//	�V�X�e��
//--------------------------------------------------------------
///�V�X�e���t�H���g�p���b�g�̓W�J�ʒu
#define SYSTEM_FONT_PAL			(15)

///�u���C�g�l�X�t�F�[�h�Ώۖ�(�X�^�b�t���[���ʂ̂ݏ���)
#define ENDING_FADE_PLANE		(PLANEMASK_BG0 | PLANEMASK_BG2 | PLANEMASK_BG3 | PLANEMASK_OBJ | PLANEMASK_OBJ | PLANEMASK_BD)

///�V�[���؂�ւ��̃u���C�g�l�X�t�F�[�h���x
#define ENDING_FADE_SYNC_SCENE		(24)
///�L���v�`���摜�؂�ւ��̃u���C�g�l�X�t�F�[�h���x
#define ENDING_FADE_SYNC_CAPTURE	(32)

///BG�t���[���ݒ�
#define FRAME_M_STAFFROLL		(GF_BGL_FRAME1_M)	//�X�^�b�t���[����
#define FRAME_M_BG				(GF_BGL_FRAME2_M)	//�w�i
#define FRAME_M_CAPTURE			(GF_BGL_FRAME3_M)	//�L���v�`��
#define FRAME_S_STAFFROLL		(GF_BGL_FRAME1_S)	//�X�^�b�t���[��
#define FRAME_S_EFF				(GF_BGL_FRAME2_S)	//�G�t�F�N�g(���g�p)
#define FRAME_S_BG				(GF_BGL_FRAME3_S)	//�w�i

///BG�t���[���ݒ�(�t�B�i�[���ȍ~)
#define FRAME_FIN_M_BG			(GF_BGL_FRAME1_M)	//�w�i
#define FRAME_FIN_M_CAP_A		(GF_BGL_FRAME2_M)	//�L���v�`��A
#define FRAME_FIN_M_CAP_B		(GF_BGL_FRAME3_M)	//�L���v�`��B
#define FRAME_FIN_S_BG			(GF_BGL_FRAME3_S)	//�w�i

///BG�v���C�I���e�B
enum{
	FRAME_BGPRI_3D = 1,
	
	FRAME_BGPRI_M_STAFFROLL = 0,
	FRAME_BGPRI_M_BG = 3,
	FRAME_BGPRI_M_CAPTURE = 2,
	
	FRAME_BGPRI_S_STAFFROLL = 0,
	FRAME_BGPRI_S_EFF = 1,
	FRAME_BGPRI_S_BG = 3,
	
	FRAME_BGPRI_FIN_M_CAPTURE_TOP = 2,
	FRAME_BGPRI_FIN_M_CAPTURE_BOTTOM = 3,
};

///�G���f�B���O�̃V�[����
enum{
	ENDING_SCENE_MAP1,
	ENDING_SCENE_CAPTURE1,
	ENDING_SCENE_MAP2,
	ENDING_SCENE_CAPTURE2,
	ENDING_SCENE_MAP3,
	ENDING_SCENE_FIELD,
	ENDING_SCENE_COPYRIGHT,

	ENDING_SCENE_MAX,
};

///�G���f�B���O��ʑJ�ڃt���[���l	���G���f�B���O�� 4��37�b(8310�t���[��)
enum{
	ENDING_COUNTER_SCENE1_FUWANTE_0 = 30 * 30,
	ENDING_COUNTER_SCENE1_FUWANTE_1 = 45 * 30,
	ENDING_COUNTER_SCENE1_FUWANTE_2 = 53 * 30,
	ENDING_COUNTER_SCENE1_END = 61 * 30,
	
	ENDING_COUNTER_CAP_SCENE1_END = 160 * 30 + 15,
	
	//-- �t�B�[���h�F�V�[���Q --//
	ENDING_COUNTER_SCENE2_KYAMOME_ESCAPE = 199 * 30,
	ENDING_COUNTER_SCENE2_END = 200 * 30,
	
	ENDING_COUNTER_CAP_SCENE2_END = 232 * 30,
	
	//-- �t�B�[���h�F�V�[���R --//
	ENDING_COUNTER_SCENE3_STAR_0 = 240 * 30,
	ENDING_COUNTER_SCENE3_STAR_1 = 246 * 30,
	ENDING_COUNTER_SCENE3_STAR_2 = 248 * 30,
	ENDING_COUNTER_SCENE3_JIBA = 248 * 30,
	ENDING_COUNTER_SCENE3_PALANM_START = 254 * 30,
	ENDING_COUNTER_SCENE3_END = 266 * 30,
	
	ENDING_COUNTER_FIELD_END = ENDING_COUNTER_SCENE3_END + 30*10,
	ENDING_COUNTER_COPYRIGHT_END = ENDING_COUNTER_FIELD_END + 60*30,
};

///�}�b�v�V�[���Q�Ŏg�p����p���b�g�A�j���̖{��
#define ENDING_SCENE2_PALANM_NUM		(4)

///�}�b�v�V�[���R�Ŏg�p����p���b�g�A�j���̖{��
#define ENDING_SCENE3_PALANM_NUM		(8)
///�}�b�v�V�[���R�̃p���b�g�A�j���W�J�ʒu
#define ENDING_SCENE3_PALANM_PAL_POS	(1)

//--------------------------------------------------------------
//	�A�N�^�[
//--------------------------------------------------------------
///�T�u�T�[�t�F�[�XY(fx32)
#define ENDING_SUB_ACTOR_DISTANCE		((192 + 80) << FX32_SHIFT)
///�T�u�T�[�t�F�[�XY(int)
#define ENDING_SUB_ACTOR_DISTANCE_INTEGER		(ENDING_SUB_ACTOR_DISTANCE >> FX32_SHIFT)

///�A�N�^�[�C���f�b�N�X
enum{
	//�V�[���}�b�v�P
	ACT_INDEX_SCENE1_HERO = 0,
	ACT_INDEX_SCENE1_HERO_SCARF,
	ACT_INDEX_SCENE1_FUWANTE_0,
	ACT_INDEX_SCENE1_FUWANTE_1,
	ACT_INDEX_SCENE1_FUWANTE_2,
	
	//�V�[���}�b�v�Q
	ACT_INDEX_SCENE2_HERO = 0,
	ACT_INDEX_SCENE2_HERO_SCARF,
	ACT_INDEX_SCENE2_KYAMOME_0,
	ACT_INDEX_SCENE2_KYAMOME_1,

	//�V�[���}�b�v�R
	ACT_INDEX_SCENE3_HERO = 0,
	ACT_INDEX_SCENE3_HERO_SCARF,
	ACT_INDEX_SCENE3_JIBA_0,
	ACT_INDEX_SCENE3_STAR_MAIN,
	ACT_INDEX_SCENE3_STAR_SUB,
	
	ACT_INDEX_MAX = ACT_INDEX_SCENE1_FUWANTE_2 + 1,		//�e�V�[���̒��ň�ԑ傫�����̂��w��
};

//--------------------------------------------------------------
//	�A�N�^�[BG�v���C�I���e�B
//--------------------------------------------------------------
enum{
	ACTBGPRI_HERO = 1,			///<��l��
	ACTBGPRI_STAR = 3,			///<���ꐯ
	
	//-- �T�u��� --//
	ACTBGPRI_SUB_FUWANTE = 2,		///<�t�����e
	ACTBGPRI_SUB_KYAMOME = 2,		///<�L������
	ACTBGPRI_SUB_JIBA = 2,			///<�W�o�R�C��
	ACTBGPRI_SUB_STAR = 3,			///<���ꐯ
};

//--------------------------------------------------------------
//	�A�N�^�[�\�t�g�v���C�I���e�B
//--------------------------------------------------------------
enum{
	SOFTPRI_HERO = 51,			///<��l��
	SOFTPRI_HERO_SCARF = 60,	///<��l���̃X�J�[�t
	
	SOFTPRI_STAR = 200,			///<���ꐯ
	
	//-- �T�u��� --//
	SOFTPRI_SUB_FUWANTE = 150,		///<�t�����e
	SOFTPRI_SUB_KYAMOME = 150,		///<�L������
	SOFTPRI_SUB_JIBA = 150,			///<�W�o�R�C��
	SOFTPRI_SUB_STAR = 200,			///<���ꐯ
};

//--------------------------------------------------------------
//	3D���f��
//--------------------------------------------------------------
///���f���ő�g�p��
#define ENDING_MODEL_MAX		(10)

///���f���f�[�^Index
enum{
	//�V�[���P
	ENDING_SCENE1_MDL_TREE_A = 0,
	ENDING_SCENE1_MDL_TREE_B,
	
	//�V�[���Q
	ENDING_SCENE2_MDL_LIGHT = 0,
	
	//�V�[���R
	ENDING_SCENE3_MDL_TREE_A = 0,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_B,
	ENDING_SCENE3_MDL_LIGHT,
	
	
	ENDING_SCENE_MDL_MAX = ENDING_SCENE3_MDL_LIGHT + 1,	//�e�V�[���̒��ň�ԑ傫���̂��w��
};

///�e�V�[���Ŏg�p����3DOBJ�̍ő吔(���C���P��)
#define ENDING_SCENE_LINE_3DOBJ_MAX		(16)

///���C����
enum{
	MODEL_LINE_0,
	MODEL_LINE_1,
	MODEL_LINE_MAX,
};

//==============================================================================
//	�\���̒�`
//==============================================================================
///�X�^�b�t���[������\���̂ւ̕s��`�|�C���^
typedef struct _PL_ENDING_LIST_WORK	PL_ENDING_LIST_WORK;

///�z�u���̃��C���z�u�f�[�^
typedef struct{
	s16 mdl_num;		///<START��END�̊Ԃɂ������f����u����
	s16 move_frame;		///<���t���[����START����END�܂ňړ������邩
#if 0
	fx32 start_x;
	fx32 end_x;
	fx32 start_y;
	fx32 end_y;
	fx32 start_z;
	fx32 end_z;
#else
	fx32 end_x;
	fx32 space_x;
	fx32 end_y;
	fx32 space_y;
	fx32 end_z;
	fx32 space_z;
#endif
}MDL_LINE_ARRANGE_DATA;

///�z�u���̔z�u�f�[�^
typedef struct{
	MDL_LINE_ARRANGE_DATA arrange[MODEL_LINE_MAX];
}MDL_ARRANGE_DATA;

///�t�����e���샏�[�N
typedef struct{
	fx32 x;
	fx32 y;
	fx32 theta;
	s32 start_wait;
	s16 scale_count;
	s8 scale_dir;
}FUWANTE_WORK;

///�L���������샏�[�N
typedef struct{
	fx32 x;
	fx32 y;
	fx32 add_x;
	fx32 add_y;
	fx32 theta;
	fx32 theta_x;
	fx32 theta_y;
	fx32 add_theta;
	fx32 add_theta_x;
	fx32 add_theta_y;
	fx32 offset_x;
	fx32 offset_y;
	s32 move_frame;
	s16 wait;
	s16 rotate_count;
	s8 rotate_dir;
	u8 seq;
}KYAMOME_WORK;

///�W�o�R�C�����샏�[�N
typedef struct{
	fx32 x;
	fx32 y;
	fx32 theta;
	s16 rotate_count;
	s8 rotate_dir;
}JIBA_WORK;

///�V�[���F�}�b�v1�̃��[�N
typedef struct{
	fx32 bg_scr_x;
	fx32 bg_scr_y;
	FUWANTE_WORK fuwante_work[ACT_INDEX_SCENE1_FUWANTE_2 - ACT_INDEX_SCENE1_FUWANTE_0 + 1];
}SCENE_WORK_MAP1;

///�V�[���F�}�b�v2�̃��[�N
typedef struct{
	fx32 bg_scr_x;
	fx32 bg_scr_y;
	u16 palanm_buf[ENDING_SCENE2_PALANM_NUM][16];
	int anm_pos;
	int evy;							///<EVY�l(����8�r�b�g����)
	int evy_dir;						///<EVY�̉����Z�̕���
	KYAMOME_WORK kyamome_work[ACT_INDEX_SCENE2_KYAMOME_1 - ACT_INDEX_SCENE2_KYAMOME_0 + 1];
}SCENE_WORK_MAP2;

///�V�[���F�}�b�v3�̃��[�N
typedef struct{
	fx32 bg_scr_x;
	fx32 bg_scr_y;
	u16 palanm_buf[ENDING_SCENE3_PALANM_NUM][16];
	int anm_pos;
	int evy;							///<EVY�l(����8�r�b�g����)
	int evy_dir;						///<EVY�̉����Z�̕���
	int anm_seq;
	u32 anm_frame;

	u8 star_move;			///<TRUE:�X�^�[���쒆
	u8 star_ud;				///<0:���ʂ̃X�^�[����A�@1:����ʂ̃X�^�[����
	s8 ev1;					///<�������W��
	s8 ev2;					///<�������W��
	
	s32 mdl_sort_tblno;		///<�\�����f���Q�Ɛ�
	
	JIBA_WORK jiba_work[ACT_INDEX_SCENE3_JIBA_0 - ACT_INDEX_SCENE3_JIBA_0 + 1];
}SCENE_WORK_MAP3;

///�V�[���F�L���v�`��1�̃��[�N
typedef struct{
	u32 change_frame;			///<�L���v�`���摜��؂�ւ���^�C�~���O���J�E���g
}SCENE_WORK_CAP1;

///�V�[���F�t�B�[���h1�̃��[�N
typedef struct{
	u32 change_frame;			///<�L���v�`���摜��؂�ւ���^�C�~���O���J�E���g
	u8 change_num;				///<�L���v�`���摜��؂�ւ����񐔂��J�E���g
	s8 ev1;
	s8 ev2;
	u32 plane1;
}SCENE_WORK_FIELD1;

///�V�[���F�R�s�[���C�g�̃��[�N
typedef struct{
	u32 work;
}SCENE_WORK_COPYRIGHT;

///�e�V�[���̃��C���Ŏ��R�Ɏg���郏�[�N
typedef struct{
	int seq;
	s16 wait;
	s16 work;
	union{
		SCENE_WORK_MAP1 sw_map1;
		SCENE_WORK_MAP2 sw_map2;
		SCENE_WORK_MAP3 sw_map3;
		SCENE_WORK_CAP1 sw_cap1;
		SCENE_WORK_FIELD1 sw_field1;
		SCENE_WORK_COPYRIGHT sw_cp;
	};
}ENDING_SCENE_WORK;

///��l���A�j���p���[�N
typedef struct{
	s16 mabataki_frame;
	s16 mabataki_anmno;
	s16 run_anm_count;		///<����A�j�������Ă����
	s16 mabataki_count;
}ENDING_HERO_ANM_WORK;


//--------------------------------------------------------------
/**
 *	�G���f�B���O����\����
 */
//--------------------------------------------------------------
typedef struct{
	ENDING_PARENT_PARAM *parent_work;
	BOOL skipFlag;			///<TRUE�F�X�L�b�v���s��

	GF_BGL_INI		*bgl;				///<BG�V�X�e���ւ̃|�C���^
	PALETTE_FADE_PTR pfd;				///<�p���b�g�V�X�e��
	GF_G3DMAN *g3Dman;
	TCB_PTR update_tcb;					///<Update�pTCB�ւ̃|�C���^
	CATS_SYS_PTR		csp;
	CATS_RES_PTR		crp;
	
	// �`��܂��̃��[�N�i���BMP�p�̕��������j
	MSGDATA_MANAGER *msgman;						// ���O���̓��b�Z�[�W�f�[�^�}�l�[�W���[

	// BMPWIN�`�����
	GF_BGL_BMPWIN		*win_fin;		///�u�����v��BMPWIN

	GF_CAMERA_PTR camera;				///<�J�����ւ̃|�C���^
	
	//3D���f��
	D3DOBJ_MDL mdl[ENDING_SCENE_MDL_MAX];		///<�e�V�[���Ŏg�p���郂�f���̍ő吔
	D3DOBJ     obj[MODEL_LINE_MAX][ENDING_SCENE_LINE_3DOBJ_MAX];///<�e�V�[���Ŏg�p����3DOBJ�ő吔
	MDL_ARRANGE_DATA mdl_arrange;				///<�z�u�f�[�^
	
	ENDING_SCENE_WORK scene_work;		///<�V�[�����[�N
	CATS_ACT_PTR cap[ACT_INDEX_MAX];	///<�A�N�^�[�|�C���^
	u8 *hero_eye_charbuf;				///<��l���̖ڃL�����N�^�o�b�t�@
	
	ARCHANDLE *ending_hdl;				///<�O���t�B�b�N�t�@�C���ւ̃n���h��
	u32 main_counter;					///<�G���f�B���O�J�n���Ă���̃t���[�������J�E���g
	u8 skip_seq;						///<�X�L�b�v���쎞�̃V�[�P���X
	u8 scene_no;						///<���݂̃V�[���ԍ�
	u8 scene_init_ok;					///<TRUE:�V�[���̏�����������ʂ��Ă���
	u8 draw_3d;							///<TRUE:3D�`��OK
	u8 mdl_line_num;					///<3D���f���̎g�p���C����
	u8 cap_no;							///<�L���v�`���摜�����Ԗڂ܂ŕ\��������
	
	PL_ENDING_LIST_WORK *list_work;		///<�X�^�b�t���[������
	PL_ENDING_LIST_WORK *list_work_sub;	///<�T�u��ʗp�X�^�b�t���[������
	BOOL list_end_flag;					///<�X�^�b�t���[���I���t���O

	ENDING_HERO_ANM_WORK hero_anm_work;	///<��l���A�j���p���[�N
	
#ifdef PM_DEBUG
	int debug_mode;
#endif
}ENDING_MAIN_WORK;



#endif	//__ENDING_COMMON_H__

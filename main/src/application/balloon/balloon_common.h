//==============================================================================
/**
 * @file	balloon_common.h
 * @brief	���D����F�S��ʋ��ʃw�b�_
 * @author	matsuda
 * @date	2007.11.06(��)
 */
//==============================================================================
#ifndef __BALLOON_COMMON_H__
#define __BALLOON_COMMON_H__

#include "application/wifi_lobby/minigame_tool.h"


//==============================================================================
//	�萔��`
//==============================================================================

///���D�~�j�Q�[���̃T�[�o�[�o�[�W����
#define BALLOON_SERVER_VERSION			(100)

///���D����̃��C�����[�h
enum{
	BALLOON_MODE_ENTRY,			///<�G���g���[��ʒ�
	BALLOON_MODE_GAME,			///<�Q�[����ʒ�
	BALLOON_MODE_RESULT,		///<���ʔ��\��ʒ�
};

///���D����~�j�Q�[���̎Q���ő�l��
#define BALLOON_PLAYER_MAX			(WFLBY_MINIGAME_MAX)

///Wifi���ב΍�ׁ̈A���M�f�[�^��1�񑗂邲�ƂɎ��̑��M�܂ł̃E�F�C�g������
#define BALLOON_SIO_SEND_WAIT		(13)


///�T�u�T�[�t�F�[�XY(fx32)
#define BALLOON_SUB_ACTOR_DISTANCE		((192 + 160) << FX32_SHIFT)
///�T�u�T�[�t�F�[�XY(int)
#define BALLOON_SUB_ACTOR_DISTANCE_INTEGER		(BALLOON_SUB_ACTOR_DISTANCE >> FX32_SHIFT)

//--------------------------------------------------------------
//	���C����ʂ�BG�t���[���w��A�v���C�I���e�B��`
//--------------------------------------------------------------
#define BALLOON_3DBG_PRIORITY			(2)		///<3D�ʂ�BG�v���C�I���e�B
///�E�B���h�E�ʂ�BG�v���C�I���e�B
#define BALLOON_BGPRI_WINDOW			(0)
///�G�t�F�N�g�ʂ�BG�v���C�I���e�B
#define BALLOON_BGPRI_EFFECT			(1)
///�w�i�ʂ�BG�v���C�I���e�B
#define BALLOON_BGPRI_BACKGROUND		(2)

///�E�B���h�E�E�p�l���ʂ̃t���[��
#define BALLOON_FRAME_WIN				(GF_BGL_FRAME1_M)
///�G�t�F�N�g�ʂ̃t���[��
#define BALLOON_FRAME_EFF				(GF_BGL_FRAME2_M)
///�ϋq(�w�i)�ʂ̃t���[��
#define BALLOON_FRAME_BACK				(GF_BGL_FRAME3_M)

#define BALLOON_BGNO_WINDOW			(1)					///<BG�ԍ��F�E�B���h�E
#define BALLOON_BGNO_EFFECT			(2)					///<BG�ԍ��F�G�t�F�N�g
#define BALLOON_BGNO_BACKGROUND		(3)					///<BG�ԍ��F�w�i

//--------------------------------------------------------------
//	�T�u��ʂ�BG�t���[���w��A�v���C�I���e�B��`
//--------------------------------------------------------------
///�T�u��ʁF�E�B���h�EBG�ʂ�BG�v���C�I���e�B
#define BALLOON_SUBBG_WIN_PRI			(1)
///�T�u��ʁF�p�C�vBG�ʂ�BG�v���C�I���e�B
#define BALLOON_SUBBG_PIPE_PRI			(3)
///�T�u��ʁF�w�iBG�ʂ�BG�v���C�I���e�B
#define BALLOON_SUBBG_BACK_PRI			(3)
///�T�u��ʁF���DBG�ʂ�BG�v���C�I���e�B
#define BALLOON_SUBBG_BALLOON_PRI		(0)

///�E�B���h�E�ʂ̃t���[��
#define BALLOON_SUBFRAME_WIN			(GF_BGL_FRAME0_S)
///�p�C�vBG�ʂ̃t���[��
#define BALLOON_SUBFRAME_PIPE			(GF_BGL_FRAME1_S)
///�w�i�ʂ̃t���[��
#define BALLOON_SUBFRAME_BACK			(GF_BGL_FRAME2_S)
///���DBG�̃t���[��
#define BALLOON_SUBFRAME_BALLOON		(GF_BGL_FRAME3_S)

//--------------------------------------------------------------
//	�X�N���[���N���A�R�[�h
//--------------------------------------------------------------
///�p�C�v�ʃN���A�R�[�h
#define PIPE_BG_CLEAR_CODE		(0)
///�E�B���h�E�ʃN���A�R�[�h
#define WIN_BG_CLEAR_CODE		(512-1)

//--------------------------------------------------------------
//	���D�^�C�vID
//--------------------------------------------------------------
///���D���x��ID
enum{
	BALLOON_LEVEL_1,
	BALLOON_LEVEL_2,
	BALLOON_LEVEL_3,
	
	BALLOON_LEVEL_MAX,
};

//==============================================================================
//	�\���̒�`
//==============================================================================
///���D����V�X�e�����[�N�\���̒�`
typedef struct{
	MNGM_ENRES_PARAM entry_param;		///<�G���g���[��ʌĂяo���p�����[�^
	MNGM_RESULT_PARAM result_param;		///<���ʔ��\��ʌĂяo���p�����[�^

	PROC *sub_proc;						///<���s���̃T�u�v���Z�X

	u8 player_netid[WFLBY_MINIGAME_MAX];	///<�Q�����Ă���v���C���[��netID
	u8 player_max;							///<�Q���l��
	
	u8 mode;							///<���[�h(BALLOON_MODE_???)
	u16 balloon_break_num;				///<���������D�̐�

	MNGM_RAREGAME_TYPE raregame_type;	///<���A�Q�[���̎��
	
	//�O������Z�b�g�����̈�
	BOOL replay;						///<�ēx���킷�邩�ǂ���(���ʔ��\��ʂ���󂯎��)

	u8 vchat;							///<�{�C�X�`���b�g���[�h

	u8 dis_error;						///< �ؒf�G���[���m	080527 tomoya 
	u8 dis_error_seq;					///< �ؒf�G���[���m	080625 tomoya 

#ifdef PM_DEBUG
	int debug_offline;		///<TRUE:�f�o�b�O�p�̃I�t���C�����[�h
#endif
}BALLOON_SYSTEM_WORK;



//==============================================================================
//	�\���̒�`�F�~�j�Q�[�����
//==============================================================================
///���D�X�e�[�^�X
typedef struct{
	int occ;			///<�L�������t���O(TRUE:�L���B���D�����݂��Ă���)
	
	int level;			///<���D���x��
	s32 air;			///<���݂̋�C��
	s32 max_air;		///<�ő�ϋv��
	s32 air_stack;		///<��C�ʃX�^�b�N�G���A
	s32 add_air;		///<�X�^�b�N�ɒ��܂��Ă����C��1�t���[���œ�����
	s32 player_air[WFLBY_MINIGAME_MAX];	///<�v���C���[���̒���������C��[player_pos�w��]
	
	int shake_flag;
	
	u8 bg_on_req;		///<TRUE:BG�\�����N�G�X�g
	u8 bg_off_req;		///<TRUE:BG��\�����N�G�X�g
	u8 padding[2];
}BALLOON_STATUS;


///s16��POINT�^
typedef struct{
	s16 x;
	s16 y;
}POINT_S16;

#endif	//__BALLOON_COMMON_H__


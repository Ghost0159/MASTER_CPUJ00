//==============================================================================
/**
 * @file	frontier_scr.h
 * @brief	�t�����e�B�A�p�X�N���v�g����̃w�b�_
 * @author	matsuda
 * @date	2007.03.30(��)
 */
//==============================================================================
#ifndef __FRONTIER_SCR_H__
#define __FRONTIER_SCR_H__

#include "gflib/msg_print.h"						//STRCODE
#include "system\msgdata.h"							//MSGMAN_TYPE_DIRECT
#include "system\bmp_menu.h"
#include "system/bmp_list.h"
#include "system/pm_str.h"
#include "system/msgdata_util.h"


#include "system/wordset.h"							//WORDSET_Create
#include "system/clact_tool.h"

#include "frontier_types.h"
#include "fss_task.h"



//==============================================================================
//	�\���̒�`
//==============================================================================
///���z�}�V���Ԃŋ��L���郏�[�N�̍\����
typedef struct{
	u16 work[FSW_WORK_MAX - FSW_PARAM_START];	///<�e���z�}�V�������L���Ďg�p���郏�[�N
}FS_PARAM_WORK;

//�X�N���v�g���䃏�[�N�\����
struct _FS_SYSTEM{
	FMAIN_PTR fmain;

	FSS_TASK *fss_task[FSS_TASK_MAX];	///<�X�N���v�g����^�X�N�ւ̃|�C���^
	FS_PARAM_WORK param_work;			///<�e���z�}�V�������L���Ďg�p���郏�[�N
	
	int heap_id;						///�X�N���v�g�Ŏg�p����q�[�vID
	u8 seq;								//

	MSGDATA_MANAGER* def_msgman;	// �W�����b�Z�[�W�}�l�[�W���[
	FSS_CODE * def_script;			// ���[�h�����W���X�N���v�g

	WORDSET* wordset;				//�P��Z�b�g
	STRBUF* msg_buf;				//���b�Z�[�W�o�b�t�@�|�C���^
	STRBUF* tmp_buf;				//�e���|�����o�b�t�@�|�C���^
	u8 msg_talk_index;				///<��b���b�Z�[�W�̕����`�惋�[�`���̃C���f�b�N�X

	s8 fss_task_count;			//�ǉ��������z�}�V���̐�

	u16 def_scene_id;				//���C���V�[��ID

	int player_dir;					//�C�x���g�N�����̎�l���̌���

//	u32 magic_no;					//�C�x���g�̃��[�N���X�N���v�g���䃏�[�N���𔻕�
	u8 MsgIndex;					//���b�Z�[�W�C���f�b�N�X
	u8 anm_count;					//�A�j���[�V�������Ă��鐔
//	u8 common_scr_flag;				//���[�J���E���ʃX�N���v�g�؂�ւ��t���O(0=���[�J���A1=����)
	u8 win_open_flag;				//��b�E�B���h�E���J�������t���O(0=�J���Ă��Ȃ��A1=�J����)
	BOOL win_flag;					///<�퓬���ʕێ��p���[�N

	//�C�x���g�E�B���h�E
	FSEVWIN_PTR ev_win;				//�C�x���g�E�B���h�E���[�N�ւ̃|�C���^

	//��b�E�B���h�E
	GF_BGL_BMPWIN bmpwin_talk;		//�r�b�g�}�b�v�E�B���h�E(��b���b�Z�[�W�p)
	BMPMENU_WORK* mw;				//�r�b�g�}�b�v���j���[���[�N

//	TARGET_OBJ_PTR target_obj;		//�b�������Ώۂ�OBJ�̃|�C���^
//	TARGET_OBJ_PTR dummy_obj;		//�����_�~�[OBJ�̃|�C���^

	u16* ret_script_wk;				//�X�N���v�g���ʂ������郏�[�N�̃|�C���^

	void * waiticon;				///<�ҋ@�A�C�R���̃|�C���^
	
//	u16 work[EVSCR_WORK_MAX];		//���[�N(ANSWORK,TMPWORK�Ȃǂ̑���)

//	fsysFunc next_func;				//�X�N���v�g�I�����ɌĂяo�����֐�

	void * subproc_work;			//�T�u�v���Z�X�Ƃ̂��Ƃ�Ɏg�p���郏�[�N�ւ̃|�C���^
	void* pWork;					//���[�N�ւ̔ėp�|�C���^

//	TCB_PTR player_tcb;				//���@�`�ԃ��|�[�gTCB

	GF_BGL_BMPWIN CoinWinDat;		//�r�b�g�}�b�v�E�B���h�E�f�[�^
	GF_BGL_BMPWIN GoldWinDat;		//�r�b�g�}�b�v�E�B���h�E�f�[�^

	GF_BGL_BMPWIN* pParentNameCPWin;//�r�b�g�}�b�v�E�B���h�E�f�[�^
	GF_BGL_BMPWIN* pChildNameCPWin;	//�r�b�g�}�b�v�E�B���h�E�f�[�^

//	REPORT_INFO * riw;				///<���|�[�g���p�E�B���h�E���䃏�[�N

	u16 save_seq0;
	u16 save_seq1;
	u16 save_rec_mode;
	u16 save_rensyou;
	u16 *save_ret_wk;
};

//==============================================================================
//	�O���֐��錾
//==============================================================================
extern FSS_PTR FSS_SystemCreate(FMAIN_PTR fmain, int heap_id, int scene_id);
extern BOOL FSS_ScriptMain(FSS_PTR fss);
extern void FSS_SystemFree(FSS_PTR fss);
extern void FSS_ScrTaskAdd(FSS_PTR fss, int scene_id, int event_id);
extern void FSS_MsgManageSceneChange(FSS_PTR fss, int new_scene_id, int heap_id);
extern FS_PARAM_WORK *FSS_ParamWorkBackup(FSS_PTR fss, int heap_id);
extern void FSS_ParamWorkRecover(FSS_PTR fss, FS_PARAM_WORK *pw);

//--------------------------------------------------------------
//	�X�N���v�g�V�X�e���n���g�p����悤�ȃc�[��
//--------------------------------------------------------------
extern u16 * FSS_ParamWorkAdrsGet(FSS_PTR fss, int work_no);
extern FMAP_PTR FSS_GetFMapAdrs(FSS_PTR fss);



#endif	//__FRONTIER_SCR_H__

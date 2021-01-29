//==============================================================================
/**
 * @file	fs_usescript.h
 * @brief	�t�����e�B�A�p�X�N���v�g�Ŏg�p�����`�Ȃ�
 * @author	matsuda
 * @date	2007.03.30(��)
 *
 * �A�Z���u���t�@�C������include����ꍇ�́A�O���Ő��__ASM_ONLY_INC_��define��`���Ă�������
 */
//==============================================================================
#ifndef __FS_USESCRIPT_H__
#define __FS_USESCRIPT_H__

#ifdef __ASM_ONLY_INC_
#define	__ASM_NO_DEF_
#define ASM_CPP							//pm_version.h
#endif	//__ASM_ONLY_INC_

///�A�Z���u���f�[�^�̏I�[�R�[�h�Ɏg�p
#define FSS_DATA_END_CODE	(0xfd13)	//�K���Ȓl

///OBJ�R�[�h�F�������g
#define FSS_CODE_MYSELF		(0xeeee)
///OBJ�R�[�h�F�ʐM�v���C���[
#define FSS_CODE_SIO_PLAYER	(0xeeef)

#define ON					(1)
#define OFF					(0)

//include\gflib\camera.h���甲��(���c����m�F)
#define FSS_CAMERA_PERSPECTIV	(0)		// �����ˉe
#define FSS_CAMERA_ORTHO		(1)		// ���ˉe

//--------------------------------------------------------------
//	���[�NID��`
//--------------------------------------------------------------
///���̃X�N���v�g�݂̂Ŏg�p���郏�[�N
#define FSW_LOCAL_START		(0x8000)
#define FSW_LOCAL0			(FSW_LOCAL_START + 0)
#define FSW_LOCAL1			(FSW_LOCAL_START + 1)
#define FSW_LOCAL2			(FSW_LOCAL_START + 2)
#define FSW_LOCAL3			(FSW_LOCAL_START + 3)
#define FSW_LOCAL4			(FSW_LOCAL_START + 4)
#define FSW_LOCAL5			(FSW_LOCAL_START + 5)
#define FSW_LOCAL6			(FSW_LOCAL_START + 6)
#define FSW_LOCAL7			(FSW_LOCAL_START + 7)
#define FSW_LOCAL_MAX		(FSW_LOCAL7 + 1)

///�X�N���v�g�Ԃŋ��L���郏�[�N
#define FSW_PARAM_START		(FSW_LOCAL_MAX)
#define	FSW_ANSWER			(FSW_PARAM_START + 0)
#define	FSW_PARAM0			(FSW_PARAM_START + 1)
#define	FSW_PARAM1			(FSW_PARAM_START + 2)
#define	FSW_PARAM2			(FSW_PARAM_START + 3)
#define	FSW_PARAM3			(FSW_PARAM_START + 4)
#define	FSW_PARAM4			(FSW_PARAM_START + 5)
#define	FSW_PARAM5			(FSW_PARAM_START + 6)
#define	FSW_PARAM6			(FSW_PARAM_START + 7)
#define FSW_WORK_MAX		(FSW_PARAM6 + 1)

///���W�X�^
#define FSW_REG_START		(FSW_WORK_MAX)
#define	FSW_REG0			(FSW_REG_START + 0)
#define	FSW_REG1			(FSW_REG_START + 1)
#define	FSW_REG2			(FSW_REG_START + 2)
#define	FSW_REG3			(FSW_REG_START + 3)
#define FSW_REG_MAX			(FSW_REG3 + 1)
#define FSW_REG_WORK_MAX	(FSW_REG_MAX - FSW_REG_START)

//--------------------------------------------------------------
//	�A�N�^�[�|�C���^�L���p�o�b�t�@��ID
//--------------------------------------------------------------
#define ACTWORK_0			(0)
#define ACTWORK_1			(1)
#define ACTWORK_2			(2)
#define ACTWORK_3			(3)
#define ACTWORK_4			(4)
#define ACTWORK_5			(5)
#define ACTWORK_6			(6)
#define ACTWORK_7			(7)
#define ACTWORK_MAX			(8)

#define ACTWORK_RESOURCE_MAX	(ACTWORK_MAX)

//--------------------------------------------------------------
//	�\���p�|�P�����Ǘ�ID
//--------------------------------------------------------------
#define POKE_DISP_MANAGER_ID_START	(50000)
#define POKE_DISP_MANAGER_ID_0		(POKE_DISP_MANAGER_ID_START + 0)
#define POKE_DISP_MANAGER_ID_1		(POKE_DISP_MANAGER_ID_START + 1)
#define POKE_DISP_MANAGER_ID_2		(POKE_DISP_MANAGER_ID_START + 2)
#define POKE_DISP_MANAGER_ID_3		(POKE_DISP_MANAGER_ID_START + 3)
#define POKE_DISP_MANAGER_ID_NUM	(POKE_DISP_MANAGER_ID_3 - POKE_DISP_MANAGER_ID_START + 1)

//--------------------------------------------------------------
//	�p�[�e�B�N����SPA�Ǘ��ԍ�	��PARTICLE_GLOBAL_MAX�𒴂��Ȃ��悤�ɂ��邱��
//	
//	SPAWORK_?�̓X�N���v�g�Ŏg�p����
//	SPAWORK_EFF_?�̓^�X�N��Ȃ�C�t�@�C������g�p����
//	�G���J�E���g�G�t�F�N�g�ȂǁA�^�X�N�ォ�琶���������̂�SPAWORK_EFF_?���g�p���Ă����
//	�X�N���v�g�Ŏg�p���Ă���Ǘ��ԍ��Ƃ͔��Ȃ�
//	�ꍇ�ɂ���Ă�SPAWORK_EFF_?�̕��̓}�l�[�W�����ɂ��邩��
//--------------------------------------------------------------
#define SPAWORK_0			(0)		//�X�N���v�g��Ŏg�p
#define SPAWORK_1			(1)
#define SPAWORK_2			(2)
#define SPAWORK_3			(3)
#define SPAWORK_EFF_0		(SPAWORK_3 + 1)			//C�t�@�C������g�p
#define SPAWORK_EFF_1		(SPAWORK_EFF_0 + 1)
#define SPAWORK_EFF_2		(SPAWORK_EFF_0 + 2)
#define SPAWORK_EFF_3		(SPAWORK_EFF_0 + 3)
#define SPAWORK_MAX			(SPAWORK_EFF_3 + 1)


//==============================================================================
//	
//==============================================================================
#define	LT				0				/* <	*/
#define	EQ				1				/* ==	*/
#define	GT				2				/* >	*/
#define	LE				3				/* <=	*/
#define	GE				4				/* >=	*/
#define	NE				5				/* !=	*/
#define	EQUAL			EQ
#define	LITTLER			LT
#define	GREATER			GT
#define	LT_EQ			LE
#define	GT_EQ			GE

#define FSEV_WIN_B_CANCEL		(0xfffe)		//B�L�����Z��
#define FSEV_WIN_NOTHING		(0xeeee)		//�������̒l
#define FSEV_WIN_COMPULSION_DEL	(0xeedd)		//�����폜�̒l
#define	FSEV_WIN_LIST_LABEL		(0xfa)			//���X�g�g�p���Ƀ��x�����w�肷���`(�X�N���v�g�p)
#define	FSEV_WIN_TALK_MSG_NONE	(0xff)			//���X�g�g�p���̉�b���b�Z�[�W�Ȃ�

//���C�v�t�F�[�h�̊�{�̒l
#define SCR_WIPE_DIV				(WIPE_DEF_DIV)
#define SCR_WIPE_SYNC				(WIPE_DEF_SYNC)


//==============================================================================
//	�A�j���[�V�����R�[�h	��AnmCmdTbl�ƕ��т𓯂��ɂ��Ă�������
//==============================================================================
#define FC_DIR_U				0		///<��Ɍ���
#define FC_DIR_D				1		///<���Ɍ���
#define FC_DIR_L				2		///<���Ɍ���
#define FC_DIR_R				3		///<�E�Ɍ���
#define FC_WALK_U_8F			4		///<��Ɉړ��@8�t���[��
#define FC_WALK_D_8F			5		///<���Ɉړ��@8�t���[��
#define FC_WALK_L_8F			6		///<���Ɉړ��@8�t���[��
#define FC_WALK_R_8F			7		///<�E�Ɉړ��@8�t���[��
#define FC_WALK_U_16F			8		///<��Ɉړ��@16�t���[��
#define FC_WALK_D_16F			9		///<���Ɉړ��@16�t���[��
#define FC_WALK_L_16F			10		///<���Ɉړ��@16�t���[��
#define FC_WALK_R_16F			11		///<�E�Ɉړ��@16�t���[��
#define FC_WALK_U_4F			12		///<��Ɉړ��@4�t���[��
#define FC_WALK_D_4F			13		///<���Ɉړ��@4�t���[��
#define FC_WALK_L_4F			14		///<���Ɉړ��@4�t���[��
#define FC_WALK_R_4F			15		///<�E�Ɉړ��@4�t���[��
#define FC_STAY_WALK_U_8F		16		///<���̏�����@������@8�t���[��
#define FC_STAY_WALK_D_8F		17		///<���̏�����@�������@8�t���[��
#define FC_STAY_WALK_L_8F		18		///<���̏�����@�������@8�t���[��
#define FC_STAY_WALK_R_8F		19		///<���̏�����@�E�����@8�t���[��
#define FC_STAY_WALK_U_16F		20		///<���̏�����@������@16�t���[��
#define FC_STAY_WALK_D_16F		21      ///<���̏�����@�������@16�t���[��
#define FC_STAY_WALK_L_16F		22      ///<���̏�����@�������@16�t���[��
#define FC_STAY_WALK_R_16F		23      ///<���̏�����@�E�����@16�t���[��
#define FC_STAY_WALK_U_2F		24		///<���̏�����@������@8�t���[��
#define FC_STAY_WALK_D_2F		25		///<���̏�����@�������@8�t���[��
#define FC_STAY_WALK_L_2F		26		///<���̏�����@�������@8�t���[��
#define FC_STAY_WALK_R_2F		27		///<���̏�����@�E�����@8�t���[��
#define FC_STAY_WALK_U_4F		28		///<���̏�����@������@8�t���[��
#define FC_STAY_WALK_D_4F		29		///<���̏�����@�������@8�t���[��
#define FC_STAY_WALK_L_4F		30		///<���̏�����@�������@8�t���[��
#define FC_STAY_WALK_R_4F		31		///<���̏�����@�E�����@8�t���[��
#define FC_ANMCMD_END			32	///<AnmCmdTbl�ɒ�`���Ă��閽�߂����܂ŁB���L����̓E�F�C�g�̂�
#define FC_WAIT_1F				(FC_ANMCMD_END + 0)		///<1�t���[���҂�
#define FC_WAIT_2F				(FC_ANMCMD_END + 1)		///<2�t���[���҂�
#define FC_WAIT_4F				(FC_ANMCMD_END + 2)		///<4�t���[���҂�
#define FC_WAIT_8F				(FC_ANMCMD_END + 3)		///<8�t���[���҂�
#define FC_WAIT_15F				(FC_ANMCMD_END + 4)		///<15�t���[���҂�
#define FC_WAIT_16F				(FC_ANMCMD_END + 5)		///<16�t���[���҂�
#define FC_WAIT_32F				(FC_ANMCMD_END + 6)		///<32�t���[���҂�
#define FC_SYSCMD_START			(FC_ANMCMD_END + 7)	//�V�X�e���R�}���h�J�n
#define FC_SYSCMD_VISIBLE_ON	(FC_SYSCMD_START + 0)	///<OBJ��\��������
#define FC_SYSCMD_VISIBLE_OFF	(FC_SYSCMD_START + 1)	///<OBJ���\���ɂ���
#define FC_SYSCMD_END			(FC_SYSCMD_START + 2)//�V�X�e���R�}���h�I��



//==============================================================================
//	�A�Z���u���t�@�C���݂̂�include
//==============================================================================
#ifdef __ASM_ONLY_INC_

#include "../../../include/pm_version.h"
#include "../../../include/system/wipe.h"			//���C�v
#include "../../../include/system/snd_def.h"
#include "../../field/fieldobj_code.h"
#include "../../field/comm_direct_counter_def.h"
#include "../../../include/application/wifi_2dmap/wifi_2dcharcon.h"
#include "../../../include/application/wifi_2dmap/wf2dmap_common_c.h"
#include "../../../include/application/wifi_2dmap/wf2dmap_objst.h"
#include "../fss_scene.h"	//�V�[��ID
#include "../frontier_act_id.h"
#include "../frontier_def.h"
#include "../../../include/savedata/battle_rec.h"
#include "../../../include/savedata/record.h"
#include "../../../include/savedata/score_def.h"

#endif	//__ASM_ONLY_INC_


#endif	//__FS_USESCRIPT_H__

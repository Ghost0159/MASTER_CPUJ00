//==============================================================================
/**
 * @file	frontier_tcb_pri.h
 * @brief	�t�����e�B�A�̃V�X�e���֘ATCB�v���C�I���e�B��`
 * @author	matsuda
 * @date	2007.04.16(��)
 */
//==============================================================================
#ifndef __FRONTIER_TCB_PRI_H__
#define __FRONTIER_TCB_PRI_H__


//--------------------------------------------------------------
//	�V�X�e���n
//--------------------------------------------------------------
///�}�b�v�F�I�u�W�F�N�g���C�����s����
#define TCBPRI_OBJSYS_UPDATE		(60000)
///�}�b�v�F�I�u�W�F�N�g�R�}���h���f����
#define TCBPRI_CMDJUDGE_UPDATE		(61000)
///�}�b�v�F���C�����[�v�̍Ō�ɍs���V�X�e���֘A�̍X�V����
#define TCBPRI_MAP_UPDATE			(80000)

///�t�B�[���hOBJ�A�j���[�V�������sTCB�v���C�I���e�B
#define TCBPRI_FIELDOBJ_ANIME		(TCBPRI_OBJSYS_UPDATE + 100)

///�X�e�[�W�FBG�A�j������^�X�N
#define TCBPRI_STAGE_BGANIME_CONTROL		(TCBPRI_MAP_UPDATE - 500)
///�X�e�[�W�F�p���b�g�A�j������^�X�N
#define TCBPRI_STAGE_PALANIME_CONTROL		(TCBPRI_STAGE_BGANIME_CONTROL + 1)
///�X�e�[�W�F���X�^�[�A�j������^�X�N
#define TCBPRI_STAGE_LASTER_CONTROL			(TCBPRI_STAGE_PALANIME_CONTROL + 1)

///�n�k����^�X�N
#define TCBPRI_SHAKE				(70000)
///Window����^�X�N
#define TCBPRI_WND					(300)

//--------------------------------------------------------------
//	�G�t�F�N�g�n
//--------------------------------------------------------------
///�t�����e�B�A�u���[����̃J�b�g�C���G�t�F�N�g
#define TCBPRI_ENC_CUTIN			(1000)
///�J�b�g�C���G�t�F�N�g�̃W�O�U�OBG�̃p���b�g�A�j��
#define TCBPRI_ENC_CUTIN_BGPALANM	(1100)


#endif	//__FRONTIER_TCB_PRI_H__

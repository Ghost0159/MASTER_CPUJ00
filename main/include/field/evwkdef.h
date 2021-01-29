//==============================================================================================
/**
 * @file	evwkdef.c
 * @brief	�t���O�A���[�N�̈��`
 * @author	Satoshi Nohara
 * @date	2005.11.07
 *
 * 2007.06.04	�v���`�i�p�ɃI�t�Z�b�g��ύX
 */
//==============================================================================================
#ifndef __EVWKDEF_H__
#define __EVWKDEF_H__

//----------------------------------------------------------------------------------------------
//
//	�t���O�֘A��`
//
//----------------------------------------------------------------------------------------------
//script/saveflag.h
#define USRFLAG_START		0
#define USRFLAG_MAX			2400						//�X�N���v�g��`�̃t���O�ő吔
#define USRFLAG_AREA_MAX	(USRFLAG_MAX/8)

//field/sysflag.h
#define SYSFLAG_START		(USRFLAG_MAX)
#define	SYSFLAG_MAX			320							//�V�X�e����`�̃t���O�ő吔
#define	SYSFLAG_AREA_MAX	(SYSFLAG_MAX/8)

#define TIMEFLAG_START		(USRFLAG_MAX+SYSFLAG_MAX)
#define TIMEFLAG_MAX		192							//���Ԍo�߃C�x���g�p�t���O�ő吔
#define	TIMEFLAG_AREA_MAX	(TIMEFLAG_MAX/8)

//�t���O�ő吔
#define	EVENT_FLAG_MAX		(USRFLAG_MAX+SYSFLAG_MAX+TIMEFLAG_MAX)
#define	EVENT_FLAG_AREA_MAX	(EVENT_FLAG_MAX/8)

//�t���O�I�t�Z�b�g
#define	SVFLG_START			0x0000						//�Z�[�u�t���O�X�^�[�g�i���o�[
#define	SCFLG_START			0x4000						//����t���O�X�^�[�g�i���o�[

//�g���[�i�[�t���O
#define TR_FLAG_START		1360						//�g���[�i�[ID�Ή��t���O�J�n�ʒu
#define TR_FLAG_MAX			(USRFLAG_MAX-TR_FLAG_START)	//�g���[�i�[ID�Ή��t���O�ő吔

//�Z�[�u���Ȃ��t���O�֘A��`
#define	CTRLFLAG_MAX		64							//����t���O�ő吔
#define	CTRLFLAG_AREA_MAX	(CTRLFLAG_MAX/8)

//�t���O�X�^�[�g�i���o�[
#define FE_FLAG_START		(100)						//�C�x���g�i�s����t���O
//#define FV_FLAG_START		(400)						//�o�j�b�V���t���O
#define FV_FLAG_START		(370)						//�o�j�b�V���t���O
#define FH_FLAG_START		(730)						//�B���A�C�e���t���O(256+28��)
//#define FB_FLAG_START		(FH_FLAG_START+256)			//�t�B�[���h��̃A�C�e���t���O(256��)
#define FB_FLAG_START		(FH_FLAG_START+256+28)		//�t�B�[���h��̃A�C�e���t���O(256+72��)
//730+256+28=1014
//1014+256+72=1342
//1360�`�g���[�i�[

#define LOCAL_FLAG_MAX		(64)						//���[�J���t���O��(8bit*8=64)
#define LOCAL_FLAG_AREA_MAX	(LOCAL_FLAG_MAX/8)

//----------------------------------------------------------------------------------------------
//
//	���[�N�֘A��`
//
//----------------------------------------------------------------------------------------------
//script/savework.h
#define EVENT_WORK_AREA_MAX	288

#define	SVWK_START			0x4000								//�Z�[�u���[�N��`�X�^�[�g�i���o�[

//���[�N�I�t�Z�b�g
#define LOCAL_WORK_START	(0+SVWK_START)						//���[�J�����[�N�J�n
#define LOCAL_WORK_MAX		(32)								//���[�J�����[�N��
#define OBJCHR_WORK_START	(LOCAL_WORK_START+LOCAL_WORK_MAX)	//�ύX�\��OBJ�L�����w�胏�[�N�J�n
#define OBJCHR_WORK_MAX		(16)								//�ύX�\��OBJ�L�����w�胏�[�N��
#define OTHERS_WORK_START	(OBJCHR_WORK_START+OBJCHR_WORK_MAX)	//���̑��̃��[�N�J�n
#define OTHERS_WORK_MAX		(64)								//���̑��̃��[�N��
#define SYSWORK_MAX			(LOCAL_WORK_MAX+OBJCHR_WORK_MAX+OTHERS_WORK_MAX)	//

#define	SVSCRWK_START		(SVWK_START+SYSWORK_MAX)			//script/savework.h(�X�N���v�g�p)
#define	SCWK_START			0x8000						//�f�[�^�����E�ėp���[�N��`�X�^�[�g�i���o�[

#if 0
//----------------------------------------------------------------------------------------------
//
//	�G�������h
//
//----------------------------------------------------------------------------------------------
//�t���O�� = 2400
//���肬��܂Ŏg�p���Ă���

//�g���[�i�[�t���O�� = 864
//854�g�p���Ă���

//���[�N�� = 256
//�����]�T���銴���H������
#endif

#if 0
//----------------------------------------------------------------------------------------------
//
//	�_�C���E�p�[��
//
//----------------------------------------------------------------------------------------------
�g���[�i�[�t���O
�J�n	1360
�ő吔	1040
2400 - 1360 = 1040

���[�J���t���O
0-63

���g�p
63-99

�C�x���g�t���O
100-399

�o�j�b�V���t���O
400-729

�B���A�C�e���t���O
730-985(255)

�t�B�[���h��̃A�C�e��
986-1241

���g�p
1242-1359
#endif

#endif


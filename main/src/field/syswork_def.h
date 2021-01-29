//============================================================================================
/**
 * @file	syswork_def.h
 * @bfief	�V�X�e�����[�N��`
 * @author	Satoshi Nohara
 * @date	06.04.14
 */
//============================================================================================
#ifndef	__SYSWORK_DEF_H__
#define	__SYSWORK_DEF_H__


//============================================================================================
//	�t���O��`
//
//	�X�N���v�g			��`�𒼐ڎQ�Ƃ��Ă悢�I
//
//	�v���O����			syswork.h�ɂ���A�N�Z�X�֐����g���I
//
//============================================================================================
#define		SYS_WORK_FIRST_POKE_NO		(0+OTHERS_WORK_START)	//�ŏ��̃|�P�����i���o�[
//#define	SYS_WORK_POISON_STEP_COUNT	(1+OTHERS_WORK_START)	//�ŕ����J�E���^�[
#define		SYS_WORK_INTERVIEW_NO		(2+OTHERS_WORK_START)	//15-18�̃C���^�r���[�i���o�[
#define		SYS_WORK_BTL_SEARCHER_BATTERY	(3+OTHERS_WORK_START)	//�o�g���T�[�`���[�o�b�e���[
#define		SYS_WORK_BTL_SEARCHER_CLEAR	(4+OTHERS_WORK_START)	//�o�g���T�[�`���[�N���A
#define		SYS_WORK_OOKISA_RECORD		(5+OTHERS_WORK_START)	//�傫����חp���R�[�h�i�ő̒l�j
#define		SYS_WORK_HIDEMAP_01			(6+OTHERS_WORK_START)	//�B���}�b�v����p���[�N���̂P
#define		SYS_WORK_HIDEMAP_02			(7+OTHERS_WORK_START)	//�B���}�b�v����p���[�N���̂Q
#define		SYS_WORK_HIDEMAP_03			(8+OTHERS_WORK_START)	//�B���}�b�v����p���[�N���̂R
#define		SYS_WORK_HIDEMAP_04			(9+OTHERS_WORK_START)	//�B���}�b�v����p���[�N���̂S
#define		SYS_WORK_PARK_WALK_COUNT	(10+OTHERS_WORK_START)	//���R�����ł̕����J�E���g
#define		SYS_WORK_NEWS_DAYS_COUNT	(11+OTHERS_WORK_START)	//�V���В��ߐ؂�J�E���^
#define		SYS_WORK_POKELOT_RND1		(12+OTHERS_WORK_START)	//�|�P�����N�W�p32bit
#define		SYS_WORK_POKELOT_RND2		(13+OTHERS_WORK_START)	//�|�P�����N�W�p32bit
#define		SYS_WORK_UG_TALK_COUNT		(14+OTHERS_WORK_START)	//�n����b�J�E���g�p(�~�J�Q�p�j
#define		SYS_WORK_PAIR_TRAINER_ID	(15+OTHERS_WORK_START)	//�A������g���[�i�[ID
#define		SYS_WORK_RENSYOU_COUNT		(16+OTHERS_WORK_START)	//�X���b�g�A���ő吔
#define		SYS_WORK_POKE_LEVEL_NO		(17+OTHERS_WORK_START)	//�|�P�������x���ŃA�C�e������
#define		SYS_WORK_DEPART_COUNT		(18+OTHERS_WORK_START)	//�f�p�[�g�ł̍w����
#define		SYS_WORK_HAIHU_EVENT01		(19+OTHERS_WORK_START)	//�z�z�C�x���g����p���[�N���̂P
#define		SYS_WORK_HAIHU_EVENT02		(20+OTHERS_WORK_START)	//�z�z�C�x���g����p���[�N���̂Q
#define		SYS_WORK_HAIHU_EVENT03		(21+OTHERS_WORK_START)	//�z�z�C�x���g����p���[�N���̂R
#define		SYS_WORK_HAIHU_EVENT04		(22+OTHERS_WORK_START)	//�z�z�C�x���g����p���[�N���̂S
#define		SYS_WORK_UG_KASEKI_DIG_COUNT	(23+OTHERS_WORK_START)	//�n�� ���������@������
#define		SYS_WORK_UG_TRAP_HIT_COUNT	(24+OTHERS_WORK_START)	//�n�� �g���b�v�ɂ�������
#define		SYS_WORK_UG_TALK_COUNT2		(25+OTHERS_WORK_START)	//�n����b�J�E���g�p
#define		SYS_WORK_FRIENDLY_COUNT		(26+OTHERS_WORK_START)	//�Ȃ��x�p�����J�E���g
#define		SYS_WORK_PL_GRASS_GYM_TIME	(27+OTHERS_WORK_START)	//PL���W���@���ԃJ�E���g
#define		SYS_WORK_MAID_TURN			(28+OTHERS_WORK_START)	//���C�h���������^�[����(�ڕW)
#define		SYS_WORK_MAID_WIN_TURN		(29+OTHERS_WORK_START)	//���C�h���������^�[����(�����^�[��)
#define		SYS_WORK_EVENT_T07R0201		(30+OTHERS_WORK_START)	//�ʑ��C�x���g�i���o�[
#define		SYS_WORK_MEMORY_PRINT_TOWER		(31+OTHERS_WORK_START)	//�L�O�v�����g(�^���[)
#define		SYS_WORK_MEMORY_PRINT_FACTORY	(32+OTHERS_WORK_START)	//�L�O�v�����g(�t�@�N�g���[)
#define		SYS_WORK_MEMORY_PRINT_STAGE		(33+OTHERS_WORK_START)	//�L�O�v�����g(�X�e�[�W)
#define		SYS_WORK_MEMORY_PRINT_CASTLE	(34+OTHERS_WORK_START)	//�L�O�v�����g(�L���b�X��)
#define		SYS_WORK_MEMORY_PRINT_ROULETTE	(35+OTHERS_WORK_START)	//�L�O�v�����g(���[���b�g)
#define		SYS_WORK_UG_TOOL_GIVE_COUNT	(36+OTHERS_WORK_START)//�n�� �������������(���ʒu�ς���)
#define		SYS_WORK_TW_EVSEQ			(37+OTHERS_WORK_START)	//�j�ꂽ���E�C�x���g�i�s��
#define		SYS_WORK_ARUSEUSU			(38+OTHERS_WORK_START)	//�A���Z�E�X�C�x���g
#define		SYS_WORK_SYEIMI				(39+OTHERS_WORK_START)	//�V�F�C�~�C�x���g
#define		SYS_WORK_MUUBASU_GET		(40+OTHERS_WORK_START)	//���[�o�X�ߊl����
#define		SYS_WORK_AI_GET				(41+OTHERS_WORK_START)	//�A�C�ߊl����
#define		SYS_WORK_T07R0201_TALK		(42+OTHERS_WORK_START)	//�ʑ���b�i���o�[
#define		SYS_WORK_TW_AKAGI_APPEAR		(43+OTHERS_WORK_START) //�j�ꂽ���E�A�J�M�o���Ǘ� ������0,1�̃t���O�Ǘ��̂�
#define		SYS_WORK_PC_UG_ZONE_ID		(44+OTHERS_WORK_START)	//�ǂ��̃|�P�Z���n�����ۑ�
#define		SYS_WORK_PC_UG_OBJ_ID		(45+OTHERS_WORK_START)	//�|�P�Z���n����OBJID�ۑ�
#define		SYS_WORK_FAIYAA_GET			(46+OTHERS_WORK_START)	//�t�@�C���[�ߊl����
#define		SYS_WORK_SANDAA_GET			(47+OTHERS_WORK_START)	//�T���_�[�ߊl����
#define		SYS_WORK_HURIIZAA_GET		(48+OTHERS_WORK_START)	//�t���[�U�[�ߊl����
#define		SYS_WORK_T06R0101_TALK		(49+OTHERS_WORK_START)	//�Đ�{�݃����_����b�̂ǂ��炩
#define		SYS_WORK_STAGE_TOTAL_LV		(50+OTHERS_WORK_START)	//�X�e�[�W�g�[�^�����R�[�hLV
#define		SYS_WORK_STAGE_OUEN			(51+OTHERS_WORK_START)	//�X�e�[�W�����L�����R�[�h
#define		SYS_WORK_WIFI_FR_CLEAR_FLAG	(52+OTHERS_WORK_START)	//WIFI�t�����e�B�A��TEMP�N���A
#define		SYS_WORK_T06R0101_OBJ1		(53+OTHERS_WORK_START)	//�Đ�{��OBJ�R�[�h1
#define		SYS_WORK_T06R0101_OBJ2		(54+OTHERS_WORK_START)	//�Đ�{��OBJ�R�[�h2
#define		SYS_WORK_T06R0101_OBJ3		(55+OTHERS_WORK_START)	//�Đ�{��OBJ�R�[�h3
#define		SYS_WORK_T06R0101_OBJ4		(56+OTHERS_WORK_START)	//�Đ�{��OBJ�R�[�h4
#define		SYS_WORK_D35R0102_REZI		(57+OTHERS_WORK_START)	//���W�X�`���C�x���g
#define		SYS_WORK_D35R0104_REZI		(58+OTHERS_WORK_START)	//���W�A�C�X�C�x���g
#define		SYS_WORK_D35R0106_REZI		(59+OTHERS_WORK_START)	//���W���b�N�C�x���g

//���[�N�̍ő吔�ȂǊm�F����I
//0 - 63

//============================================================================================
//
//		�z�z�C�x���g�p��`
//
//============================================================================================
#define		HAIHUEVENT_ID_D30		0		///<�z�z�C�x���g�F�_�[���C�i�_�[�N�j
#define		HAIHUEVENT_ID_D18		1		///<�z�z�C�x���g�F�~�[�V�G�i�G�E���X�j
#define		HAIHUEVENT_ID_D05		2		///<�z�z�C�x���g�F�A���Z�E�X�i�A�E�X�j
#define		HAIHUEVENT_ID_C04		3		///<�z�z�C�x���g�F�G�E���X�i���g���j

//============================================================================================
//
//		�B���}�b�v���ʒ�`
//
//============================================================================================
#define		HIDEMAP_ID_D15	(0)				///<�B���}�b�v�FD15
#define		HIDEMAP_ID_D30	(1)				///<�z�z�}�b�v�FD30
#define		HIDEMAP_ID_L04	(2)				///<�B���}�b�v�FL04
#define		HIDEMAP_ID_D18	(3)				///<�z�z�}�b�v�FD18
#endif	//__SYSWORK_DEF_H__




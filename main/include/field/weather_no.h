//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *@file		weatheru_no.h
 *@brief	���V�C	�i���o�[
 *@author	tomoya takahashi
 *@data		2006.06.08
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]



#ifndef __WEATHER_NO_H__
#define	__WEATHER_NO_H__

//-------------------------------------
//	�V�C�f�[�^��	(no)
//
//	���@�V��i���o�[��
//=====================================

#define		WEATHER_SYS_SUNNY			(0)				// ����
#define		WEATHER_SYS_CLOUDINESS		(1)				// �܂�		BG	
#define		WEATHER_SYS_RAIN			(2)				// �J		OAM	FOG
#define		WEATHER_SYS_STRAIN			(3)				// ��J		OAM FOG 
#define		WEATHER_SYS_SPARK			(4)				// ��J		OAM FOG BG
#define		WEATHER_SYS_SNOW			(5)				// ��		OAM FOG
#define		WEATHER_SYS_SNOWSTORM		(6)				// ����		OAM FOG
#define		WEATHER_SYS_SNOWSTORM_H		(7)				// �Ґ���	OAM FOG BG
#define		WEATHER_SYS_FOG				(8)				// ����		FOG	
#define		WEATHER_SYS_VOLCANO			(9)				// �ΎR�D	OAM FOG BG
#define		WEATHER_SYS_SANDSTORM		(10)			// BG�g�p����	OAM FOG BG
#define		WEATHER_SYS_DIAMONDDUST		(11)			// �X�m�[�_�X�g	OAM FOG
#define		WEATHER_SYS_SPIRIT			(12)			// �C��		OAM
#define		WEATHER_SYS_MYSTIC			(13)			// �_��		OAM FOG
#define		WEATHER_SYS_MIST1			(14)			// �������̖�	FOG	BG
#define		WEATHER_SYS_MIST2			(15)			// �������̖�	FOG	BG
#define		WEATHER_SYS_FLASH			(16)			// �t���b�V��		BG

#define		WEATHER_SYS_SPARK_EFF		(17)			// ��		BG
#define		WEATHER_SYS_FOGS			(18)			// ��		FOG
#define		WEATHER_SYS_FOGM			(19)			// ����		FOG

#define		WEATHER_SYS_RAINBOW			(20)			// ��		BG
#define		WEATHER_SYS_SNOW_STORM_BG	(21)			// BG�g�p����	OAM FOG BG
#define		WEATHER_SYS_STORM			(22)			// ����		OAM FOG

#define		WEATHER_SYS_KOGOREBI		(23)			// �Â��	BG
#define		WEATHER_SYS_DOUKUTU00		(24)			// ���A	FOG
#define		WEATHER_SYS_DOUKUTU01		(25)			// ���A	FOG
#define		WEATHER_SYS_GINGA00			(26)			// ��͒c�����Ĕ��Â�
#define		WEATHER_SYS_LOSTTOWER00		(27)			// LostTower���Â�
#define		WEATHER_SYS_HARDMOUNTAIN	(28)			// �n�[�h�}�E���e��
#define		WEATHER_SYS_DOUKUTU02		(29)			// ���A�@FOG�@��
#define		WEATHER_SYS_USUGURAI		(30)			// ���Â��@FOG

#define		WEATHER_SYS_NUM				(31)			// �V�C��

	// �퓬�t�B�[���h��Ԃ��Ă��悤
	// �o�g���t�@�N�g���[�̃��[���b�g�{�݂�
	// ���ۂ̃t�B�[���h������V�C�ȊO�̏�Ԃ�
	// �o�g���ɓn���Ȃ��Ă͂����Ȃ��Ȃ������ߍ쐬�B
	//
	// PL��GS�̒ʐM�ł��g�p�����萔�Ȃ̂ŁA
	// �l�͈��ɂ��Ă��������B
#define		WEATHER_SYS_BTONLYSTART (1000)	// �o�g���I�����[�̏�ԊJ�n�萔
#define		WEATHER_SYS_HIGHSUNNY	(1001)	// ���{����
#define		WEATHER_SYS_TRICKROOM	(1002)	// �g���b�N���[��



#if  0
enum{
	WEATHER_SYS_SUNNY,			// ����
	WEATHER_SYS_CLOUDINESS,		// �܂�		BG	
	WEATHER_SYS_RAIN,			// �J		OAM	FOG
	WEATHER_SYS_STRAIN,			// ��J		OAM FOG 
	WEATHER_SYS_SPARK,			// ��J		OAM FOG BG
	WEATHER_SYS_SNOW,			// ��		OAM FOG
	WEATHER_SYS_SNOWSTORM,		// ����		OAM FOG
	WEATHER_SYS_SNOWSTORM_H,	// �Ґ���	OAM FOG BG
	WEATHER_SYS_FOG,			// ����		FOG	
	WEATHER_SYS_VOLCANO,		// �ΎR�D	OAM FOG BG
	WEATHER_SYS_SANDSTORM,		// BG�g�p����	OAM FOG BG
	WEATHER_SYS_DIAMONDDUST,	// �X�m�[�_�X�g	OAM FOG
	WEATHER_SYS_SPIRIT,			// �C��		OAM
	WEATHER_SYS_MYSTIC,			// �_��		OAM FOG
	WEATHER_SYS_MIST1,			// �������̖�	FOG	BG
	WEATHER_SYS_MIST2,			// �������̖�	FOG	BG
	WEATHER_SYS_FLASH,			// �t���b�V��		BG

	WEATHER_SYS_SPARK_EFF,		// ��		BG
	WEATHER_SYS_FOGS,			// ��		FOG
	WEATHER_SYS_FOGM,		// ����		FOG

	WEATHER_SYS_RAINBOW,	// ��		BG
	WEATHER_SYS_SNOW_STORM_BG,// BG�g�p����	OAM FOG BG
	WEATHER_SYS_STORM,		// ����		OAM FOG
	WEATHER_SYS_KOGOREBI,		// �Â��	BG
	WEATHER_SYS_DOUKUTU00,		// ���A	FOG
	WEATHER_SYS_DOUKUTU01,		// ���A	FOG
	WEATHER_SYS_GINGA00,		// ��͒c�����Ĕ��Â�
	WEATHER_SYS_LOSTTOWER00,	// LostTower���Â�
	WEATHER_SYS_HARDMOUNTAIN,	// �n�[�h�}�E���e��
	WEATHER_SYS_DOUKUTU02,		// ���A�@FOG�@��
	WEATHER_SYS_USUGURAI,		// ���Â��@FOG

	WEATHER_SYS_NUM,// �V�C��

	// �퓬�t�B�[���h��Ԃ��Ă��悤
	// �o�g���t�@�N�g���[�̃��[���b�g�{�݂�
	// ���ۂ̃t�B�[���h������V�C�ȊO�̏�Ԃ�
	// �o�g���ɓn���Ȃ��Ă͂����Ȃ��Ȃ������ߍ쐬�B
	//
	// PL��GS�̒ʐM�ł��g�p�����萔�Ȃ̂ŁA
	// �l�͈��ɂ��Ă��������B
	WEATHER_SYS_BTONLYSTART = 1000,	// �o�g���I�����[�̏�ԊJ�n�萔
	WEATHER_SYS_HIGHSUNNY,	// ���{����
	WEATHER_SYS_TRICKROOM,	// �g���b�N���[��
};
#endif


#endif // __WEATHER_NO_H__

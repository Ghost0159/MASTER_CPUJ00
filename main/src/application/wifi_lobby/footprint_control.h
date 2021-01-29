//==============================================================================
/**
 * @file	footprint_control.h
 * @brief	���Ճ{�[�h�F�p�ɂɒ����������Ȃ��̂��܂Ƃ߂�
 * @author	matsuda
 * @date	2008.01.26(�y)
 */
//==============================================================================
#ifndef __FOOTPRINT_CONTROL_H__
#define __FOOTPRINT_CONTROL_H__


//==============================================================================
//	�萔��`
//==============================================================================
///�C���N�̉��n��EVY�l(���̐F����ǂ��܂ŐF�𗎂Ƃ����B0 �` 16)
#define INK_FOUNDATION_EVY		(12)
///�C���N�̉��n�̖ڎw���J���[
#define INK_FOUNDATION_COLOR	(0x0000)

///�C���N�p���b�g�ɒu����Ă��鎩���̑��Ճ}�[�N�̐F�F�z���C�g�{�[�h
#define INKPAL_FOOT_COLOR_CODE_WHITE		(0x0000)
///�C���N�p���b�g�ɒu����Ă��鎩���̑��Ճ}�[�N�̐F�F�u���b�N�{�[�h
#define INKPAL_FOOT_COLOR_CODE_BLACK		(0x7fff)

///������Ă�C���N�p���b�g��ύX���鎞��EVY
#define TOUCH_EFF_EVY			(6)
///������Ă�C���N�p���b�g��ύX���鎞�̃J���[�R�[�h
#define TOUCH_EFF_COLOR			(0x0000)


//--------------------------------------------------------------
//	�C���N�Q�[�W
//--------------------------------------------------------------
///�C���N�Q�[�W�̒���(�h�b�g�P��)
#define INK_GAUGE_LEN				(32)	//���ҏW�֎~
///�C���N�Q�[�W�̍ő�e��
#define INK_GAUGE_TANK_MAX			(INK_GAUGE_LEN << 8)	//���ҏW�֎~
///MAX�̏�Ԃ��牽��X�^���v��ł�����Q�[�W������ۂɂ��邩
#define INK_GAUGE_MAX_STAMP			(6)
///1�X�^���v���������Ƃɏ�����
#define INK_GAUGE_CONSUME_STAMP	(INK_GAUGE_TANK_MAX / INK_GAUGE_MAX_STAMP)	//���ҏW�֎~
///�C���N������������A�Q�[�W�����t���[�������āA�����̈ʒu�܂ňړ������邩
#define INK_GAUGE_SUB_FRAME			(4)
///����ۂ̏�Ԃ��牽�t���[���ŃC���N���ő�܂ŉ񕜂����邩
#define INK_GAUGE_RESTORE_FRAME		(60)
///1�t���[���ŉ񕜂�����C���N��
#define INK_GAUGE_RESTORE_NUM		(INK_GAUGE_TANK_MAX / INK_GAUGE_RESTORE_FRAME)	//���ҏW�֎~



#endif	//__FOOTPRINT_CONTROL_H__

//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_snd.h
 *	@brief		�L�ꉹ�y�w�b�_
 *	@author		tomoya takahashi
 *	@data		2008.01.09
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_SND_H__
#define __WFLBY_SND_H__

#include "system/snd_tool.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

#define WFLBY_SND_JUMP		( SEQ_SE_PL_DANSA5 )		// ��蕨�ɏ�鉹

#define WFLBY_SND_MINIGAME	( SEQ_SE_DP_DECIDE )		// �~�j�Q�[���J�E���^�[�ɓ�������
//#define WFLBY_SND_MINIGAME	( SEQ_SE_PL_TIMER02 )		// �~�j�Q�[���J�E���^�[�ɓ�������

#define WFLBY_SND_CURSOR	( SEQ_SE_DP_SELECT )		// ���X�g�̃J�[�\����

#define WFLBY_SND_STAFF		( SEQ_SE_DP_SELECT )		// �X�^�b�t�ɘb���������Ƃ�

#define WFLBY_SND_PLIN		( SEQ_SE_DP_TELE2 )			// �l���~��Ă��鉹
#define WFLBY_SND_PLOUT		( SEQ_SE_DP_TELE2 )			// �l����鉹

#define WFLBY_SND_DOUZOU	( SEQ_SE_PL_EFF02 )			// �����ɘb������(�������Ⴊ������)
#define WFLBY_SND_DOUZOU2	( SEQ_SE_PL_EFF04 )			// �����ɘb������(�������Ⴊ������)�~���E

//���g�p
//#define WFLBY_SND_NEWSIN	( SEQ_SE_PL_BREC16 )		// ���r�[�j���[�X�ɓ�������
//#define WFLBY_SND_WLDTMIN	( SEQ_SE_PL_BREC16 )		// ���E���v�ɓ�������
//#define WFLBY_SND_FOOTIN	( SEQ_SE_PL_BREC16 )		// ���Ճ{�[�h�ɓ�������
#define WFLBY_SND_NEWSIN	( SEQ_SE_DP_SELECT )		// ���r�[�j���[�X�ɓ�������
#define WFLBY_SND_WLDTMIN	( SEQ_SE_DP_SELECT )		// ���E���v�ɓ�������
#define WFLBY_SND_FOOTIN	( SEQ_SE_DP_SELECT )		// ���Ճ{�[�h�ɓ�������

#define WFLBY_SND_TALK		( SEQ_SE_DP_BUTTON9 )		// �b��������

#define WFLBY_SND_PROFILE	( SEQ_SE_DP_SELECT )		// X�{�^���Ŏ����̃v���t�B�[�����J��

//-----------------------------------------------------------------------------
//
//	�t���[�g�֘A			
//
//	�����{�G�t�F�N�g�{���[�^�[���Ȃ邩���H
//
//	��蕨�������{�^���͋���
//	��_PV = �����Ȃ炷�{�^��(���������{SE���Đ�)
//	��_EX = ���ꓮ���{�^��
//-----------------------------------------------------------------------------
#define WFLBY_SND_RIDE_INVALIDITY			(0xfffe)	// SE���Đ����Ȃ���`

//#define WFLBY_SND_RIDE_BUTTON01	( SEQ_SE_PL_FLOAT03 )// �t���[�g���掞�ɉ��{�^��(�敨�𓮂���)
#define WFLBY_SND_RIDE_BUTTON01	( SEQ_SE_PL_FLOAT13 )	// �t���[�g���掞�ɉ��{�^��(�敨�𓮂���)
//#define WFLBY_SND_RIDE_BUTTON02	( SEQ_SE_PL_FLOAT03 )// �t���[�g���掞�ɉ��{�^��(���C�g������)
//#define WFLBY_SND_RIDE_BUTTON03	( SEQ_SE_PL_FLOAT03 )// �t���[�g���掞�ɉ��{�^��(�����Ȃ炷)
//��������Snd_PMVoicePlay( monsno, 0 );�ł��肢���܂�

//01(�A�`����)
//�g�T�J���h���
#define WFLBY_SND_RIDE_BUTTON_ATYAMO_PV		( WFLBY_SND_RIDE_INVALIDITY )	//�ڂ̕\��ς��
#define WFLBY_SND_RIDE_BUTTON_ATYAMO_EX		( SEQ_SE_PL_FLOAT10 )	//�g�T�J���h���

//02(���U�[�h��)
#define WFLBY_SND_RIDE_BUTTON_RIZAADON_PV	( SEQ_SE_PL_FIRE )		//�����o��
//#define WFLBY_SND_RIDE_BUTTON_RIZAADON_PV	( WFLBY_SND_RIDE_INVALIDITY )		//�����o��
#define WFLBY_SND_RIDE_BUTTON_RIZAADON_EX	( SEQ_SE_PL_FLOAT03 )	//�H���Ђ炭

//03(�t�V�M�_�l)
#define WFLBY_SND_RIDE_BUTTON_HUSIGIDANE_PV	( SEQ_SE_PL_WHIP )		//��̕ڂ��o��
#define WFLBY_SND_RIDE_BUTTON_HUSIGIDANE_EX	( SEQ_SE_PL_FLOAT11 )	//�w���̂ڂ݂��͂܂��Ĕ�яo��

//04(�}�X�L�b�p)
#define WFLBY_SND_RIDE_BUTTON_HAEZIGOKU_PV	( SEQ_SE_PL_FLOAT03 )	//��������
#define WFLBY_SND_RIDE_BUTTON_HAEZIGOKU_EX	( SEQ_SE_PL_FLOAT10 )	//�肪����

//05(�|�b�`���})
#define WFLBY_SND_RIDE_BUTTON_POTTYAMA_PV	( SEQ_SE_PL_FLOAT10 )	//��������
//#define WFLBY_SND_RIDE_BUTTON_POTTYAMA_EX	( SEQ_SE_PL_FLOAT10 )	//�肪���
#define WFLBY_SND_RIDE_BUTTON_POTTYAMA_EX	( SEQ_SE_PL_FLOAT12 )	//�肪���

//06(�M�����h�X)
#define WFLBY_SND_RIDE_BUTTON_GYARADOSU_PV	( SEQ_SE_PL_WATER )			//�����o��
//#define WFLBY_SND_RIDE_BUTTON_GYARADOSU_PV	( WFLBY_SND_RIDE_INVALIDITY )	//�����o��
#define WFLBY_SND_RIDE_BUTTON_GYARADOSU_EX	( SEQ_SE_PL_FLOAT10 )	//�K��������

//07(�s�J�`���E)
//#define WFLBY_SND_RIDE_BUTTON_PIKATYUU_PV	( SEQ_SE_PL_FLOAT03 )	//��̕\��ς��
#define WFLBY_SND_RIDE_BUTTON_PIKATYUU_PV	( WFLBY_SND_RIDE_INVALIDITY )	//��̕\��ς��
#define WFLBY_SND_RIDE_BUTTON_PIKATYUU_EX	( SEQ_SE_PL_FLOAT10 )	//�K��������

//08(�W�o�R�C��)
//#define WFLBY_SND_RIDE_BUTTON_DEKAKOIRU_PV	( SEQ_SE_PL_FLOAT03 )		//�ڂ�����
#define WFLBY_SND_RIDE_BUTTON_DEKAKOIRU_PV	( WFLBY_SND_RIDE_INVALIDITY )	//�ڂ�����
#define WFLBY_SND_RIDE_BUTTON_DEKAKOIRU_EX	( SEQ_SE_PL_ELECTRO )	//�d�C���o��

//09(�~���E)
//#define WFLBY_SND_RIDE_BUTTON_MYUU_PV		( SEQ_SE_PL_FLOAT03 )	//��������
#define WFLBY_SND_RIDE_BUTTON_MYUU_PV		( WFLBY_SND_RIDE_INVALIDITY )	//��������
#define WFLBY_SND_RIDE_BUTTON_MYUU_EX		( SEQ_SE_PL_FLOAT10 )	//�K��������
#define WFLBY_SND_RIDE_BUTTON_MYUU_PV_2		( SEQ_SE_PL_KIRAKIRA4 )	//�L���L���G�t�F�N�g(3��ڂ̂�)


//-----------------------------------------------------------------------------
//
//	�^�b�`�g�C�֘A
//
//-----------------------------------------------------------------------------
#if 0	//2ch
#define WFLBY_SND_TOUCH_TOY01_1	( SEQ_SE_PL_HIROBA01 )	// �x��
#define WFLBY_SND_TOUCH_TOY01_2	( SEQ_SE_PL_HIROBA01_2 )// �x��
#define WFLBY_SND_TOUCH_TOY01_3	( SEQ_SE_PL_HIROBA01_3 )// �x��
#else	//1ch
#define WFLBY_SND_TOUCH_TOY01_1	( SEQ_SE_PL_HIROBA01_4 )// �x��
#define WFLBY_SND_TOUCH_TOY01_2	( SEQ_SE_PL_HIROBA01_5 )// �x��
#define WFLBY_SND_TOUCH_TOY01_3	( SEQ_SE_PL_HIROBA01_6 )// �x��
#endif

#if 1	//2ch(��������1ch���ƕ������Ȃ�����)
#define WFLBY_SND_TOUCH_TOY02_1	( SEQ_SE_PL_HIROBA03 )	// �h����
#define WFLBY_SND_TOUCH_TOY02_2	( SEQ_SE_PL_HIROBA03_2 )// �h����
#define WFLBY_SND_TOUCH_TOY02_3	( SEQ_SE_PL_HIROBA03_3 )// �h����
#else
#define WFLBY_SND_TOUCH_TOY02_1	( SEQ_SE_PL_HIROBA03_4 )// �h����
#define WFLBY_SND_TOUCH_TOY02_2	( SEQ_SE_PL_HIROBA03_5 )// �h����
#define WFLBY_SND_TOUCH_TOY02_3	( SEQ_SE_PL_HIROBA03_6 )// �h����
#endif	//1ch

#if 0	//2ch
#define WFLBY_SND_TOUCH_TOY03_1	( SEQ_SE_PL_HIROBA02 )	// �V���o��
#define WFLBY_SND_TOUCH_TOY03_2	( SEQ_SE_PL_HIROBA02_2 )// �V���o��
#define WFLBY_SND_TOUCH_TOY03_3	( SEQ_SE_PL_HIROBA02_3 )// �V���o��
#else	//1ch
#define WFLBY_SND_TOUCH_TOY03_1	( SEQ_SE_PL_HIROBA02_4 )// �V���o��
#define WFLBY_SND_TOUCH_TOY03_2	( SEQ_SE_PL_HIROBA02_5 )// �V���o��
#define WFLBY_SND_TOUCH_TOY03_3	( SEQ_SE_PL_HIROBA02_6 )// �V���o��
#endif

#define WFLBY_SND_TOUCH_TOY04_1	( SEQ_SE_PL_HIROBA70 )	// ���b�v��(�͂���)
#define WFLBY_SND_TOUCH_TOY04_2	( SEQ_SE_PL_HIROBA70_2 )// ���b�v��(�͂���)
#define WFLBY_SND_TOUCH_TOY04_3	( SEQ_SE_PL_HIROBA70_3 )// ���b�v��(�͂���)
#define WFLBY_SND_TOUCH_TOY05_1	( SEQ_SE_PL_HIROBA100 )	// �V�O�i��
#define WFLBY_SND_TOUCH_TOY05_2	( SEQ_SE_PL_HIROBA100_2)// �V�O�i��
#define WFLBY_SND_TOUCH_TOY05_3	( SEQ_SE_PL_HIROBA100_3)// �V�O�i��
#define WFLBY_SND_TOUCH_TOY06_1	( SEQ_SE_PL_HIROBA05 )	// �X�C���O
#define WFLBY_SND_TOUCH_TOY06_2	( SEQ_SE_PL_HIROBA05_2 )// �X�C���O
#define WFLBY_SND_TOUCH_TOY06_3	( SEQ_SE_PL_HIROBA05_3 )// �X�C���O
#define WFLBY_SND_TOUCH_TOY07_1	( SEQ_SE_PL_W030 )		// �N���b�J�[(��)
#define WFLBY_SND_TOUCH_TOY07_2	( SEQ_SE_PL_W030_2 )	// �N���b�J�[
#define WFLBY_SND_TOUCH_TOY07_3	( SEQ_SE_PL_W030_3 )	// �N���b�J�[(��)
#define WFLBY_SND_TOUCH_TOY08_1	( SEQ_SE_PL_W019 )		// ������
#define WFLBY_SND_TOUCH_TOY08_2	( SEQ_SE_PL_W019_2 )	// ������
#define WFLBY_SND_TOUCH_TOY08_3	( SEQ_SE_PL_W019_3 )	// ������
#define WFLBY_SND_TOUCH_TOY09_1	( SEQ_SE_PL_HIROBA30 )	// ���D
#define WFLBY_SND_TOUCH_TOY09_2	( SEQ_SE_PL_HIROBA30 )	// ���D
#define WFLBY_SND_TOUCH_TOY09_3	( SEQ_SE_PL_HIROBA30 )	// ���D

//�^�b�`�g�C�̕��D
#define WFLBY_SND_BALLOON_01	( SEQ_SE_PL_HIROBA20 )	// �����
#define WFLBY_SND_BALLOON_02	( SEQ_SE_PL_SUTYA )		// ��l�����n
//������鉹�Ɠ����Ȃ̂ł���Ȃ������H

//�ԉ�
#define WFLBY_SND_FIREWORKS01	( SEQ_SE_PL_140_3 )		// �ݒu�ԉ�(�������ł��グ������)
//�����[�v���Ȃ̂ŁA�I������Snd_SeStopBySeqNo( WFLBY_SND_FIREWORKS01, 0 );���Ă�ł�������
//#define WFLBY_SND_FIREWORKS02	( SEQ_SE_PL_140 )		// �ŏ�ԉ�(�Ō�ɑł�������ԉ�)
#define WFLBY_SND_FIREWORKS02	( SEQ_SE_PL_140_2 )		// �ŏ�ԉ�(�Ō�ɑł�������ԉ�)

//�^�b�`�g�C����������
//#define WFLBY_SND_TOUCH_TOY_CHG	( SEQ_SE_PL_EFF03 )		// �����SE�Ŏ���
#define WFLBY_SND_TOUCH_TOY_CHG	( SEQ_SE_PL_EFF03_4 )		// �����SE�Ŏ���



// �v���t�B�[���Ƀ^�C�v�����鉹
#define WFLBY_SND_TYPE_SET	( SEQ_SE_DP_BUTTON3 )


//-----------------------------------------------------------------------------
/**
 *			�A���P�[�g�֘A
 */
//-----------------------------------------------------------------------------
#define ANKETO_SND_RECV_WAIT	( SEQ_SE_PL_BREC80 )	// ��M��
#define ANKETO_SND_RECV			( SEQ_SE_DP_BUTTON9 )	// ��M
#define ANKETO_SND_SEND_WAIT	( SEQ_SE_PL_BREC80 )	// ���M��
#define ANKETO_SND_SEND			( SEQ_SE_DP_BUTTON9 )	// ���M
#define ANKETO_SND_CURSOR		( SEQ_SE_DP_SELECT78 )	// cursor��
//#define ANKETO_SND_SELECT		( SEQ_SE_DP_PIRORIRO2 )	// �I��
#define ANKETO_SND_SELECT		( SEQ_SE_DP_SELECT )	// �I��(�������M�ɂ����̂ŒZ����)
//#define ANKETO_SND_BAR		( SEQ_SE_PL_CON_034 )	// �o�[�\��
#define ANKETO_SND_BAR			( SEQ_SE_PL_PINPON2 )	// �o�[�\��


#endif		// __WFLBY_SND_H__



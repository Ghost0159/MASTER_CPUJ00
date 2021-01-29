
#ifndef __BR_SND_DEF_H__
#define __BR_SND_DEF_H__

#include "system/snd_tool.h"

/// Snd_SePlay( );

enum {
	
	//eBR_SND_NG		= SEQ_SE_DP_CUSTOM06,		//�^�b�`�FNG
	//eBR_SND_NG			= SEQ_SE_PL_BREC57,			//�^�b�`�FNG
	eBR_SND_NG			= SEQ_SE_PL_BREC58,			//�^�b�`�FNG

	//eBR_SND_TOUCH		= SEQ_SE_DP_BUTTON9,		//�^�b�`�FOK
	eBR_SND_TOUCH		= SEQ_SE_PL_BREC57,			//�^�b�`�FOK
	//eBR_SND_TOUCH		= SEQ_SE_PL_BREC58,			//�^�b�`�FOK
	//eBR_SND_TOUCH		= SEQ_SE_PL_BREC59,			//�^�b�`�FOK

	//eBR_SND_SEND_OK	= SEQ_SE_DP_TARARARAN,		//�f�[�^���M"����"
	eBR_SND_SEND_OK		= SEQ_SE_PL_BREC12,			//�f�[�^���M"����"

	//eBR_SND_RECV_OK	= SEQ_SE_DP_TARARARAN,		//�f�[�^��M"����"
	eBR_SND_RECV_OK		= SEQ_SE_PL_BREC12,			//�f�[�^��M"����"

	eBR_SND_FRONTIER_IN	= SEQ_SE_PL_BREC20,			//�u�t�����e�B�A�̂��낭�v�Ăяo��
	eBR_SND_FRONTIER_OUT= SEQ_SE_PL_BREC21,			//�u�t�����e�B�A�̂��낭�v������

	/////////////////////////////////////////////////////////////////////////////////////
	//08.03.19 �ǉ�
	eBR_SND_SEARCH		= SEQ_SE_PL_BREC80,			//�f�[�^�������A���M���A��M��(���[�v��)

	//�����[�v�ɂ��邩����
	eBR_SND_SCROLL		= SEQ_SE_PL_BUTTON,			//�^�b�`�F�X�N���[���o�[�𓮂���

	eBR_SND_COL_CHG		= eBR_SND_TOUCH,			//�^�b�`�F��ʂ̐F��ύX

	//eBR_SND_TOUCH_HERE= SEQ_SE_DP_SELECT,			//��ʂɓH�H����������(TOUCH HERE)
	//eBR_SND_TOUCH_HERE	= SEQ_SE_PL_BREC11,			//��ʂɓH�H����������(TOUCH HERE)
	eBR_SND_TOUCH_HERE	= SEQ_SE_PL_BREC11_2,			//��ʂɓH�H����������(TOUCH HERE)

	//��br_start.c��SE�Ăяo����`��u�������ė~�����ł��B
	eBR_SND_START	= SEQ_SE_PL_BREC03,			//�N��
	eBR_SND_START2	= SEQ_SE_PL_BREC03,				//�N��
	eBR_SND_START3	= SEQ_SE_PL_BREC03,				//�N��

	//�ق��������邩�ȁH
};


#endif

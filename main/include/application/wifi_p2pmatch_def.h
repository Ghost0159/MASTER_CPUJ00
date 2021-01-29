//============================================================================================
/**
 * @file	wifi_p2pmatch_def.h
 * @bfief	WIFI P2P�ڑ��̃}�b�`���O�Ɋւ����`  �X�N���v�g�Ŏg�p
 * @author	k.ohno
 * @date	06.04.07
 */
//============================================================================================
#ifndef __WIFI_P2PMATCH_DEF_H__
#define __WIFI_P2PMATCH_DEF_H__


// �e�f�o�b�N�p��`
#ifdef PM_DEBUG
#define WFP2P_DEBUG	// �f�o�b�N�@�\��ON

#ifdef WFP2P_DEBUG
//#define WFP2P_DEBUG_EXON	// �قڕK�v�Ȃ����X�g���\������
//#define WFP2P_DEBUG_PLON_PC	// �v���C���[�̏����ʒu�p�\�R���O
#endif

#endif

// P2PMATCH �ւ̎w��
#define WIFI_P2PMATCH_DPW     (1)   // ���̌�DPW�ֈڍs����
#define WIFI_P2PMATCH_P2P     (2)   // ���̂܂�P2P�����Ɉڍs



// P2PMATCH ����̖߂�l
#define WIFI_P2PMATCH_SBATTLE50     (1)   // �ʐM�ΐ�Ăяo��
#define WIFI_P2PMATCH_SBATTLE100     (2)   // �ʐM�ΐ�Ăяo��
#define WIFI_P2PMATCH_SBATTLE_FREE     (3)   // �ʐM�ΐ�Ăяo��
#define WIFI_P2PMATCH_DBATTLE50     (4)   // �ʐM�ΐ�Ăяo��
#define WIFI_P2PMATCH_DBATTLE100     (5)   // �ʐM�ΐ�Ăяo��
#define WIFI_P2PMATCH_DBATTLE_FREE     (6)   // �ʐM�ΐ�Ăяo��
#define WIFI_P2PMATCH_TRADE      (7)   // �|�P�����g���[�h�Ăяo��
#define WIFI_P2PMATCH_END        (8)   // �ʐM�ؒf���Ă܂��B�I�����܂��B
#define WIFI_P2PMATCH_UTIL       (9)   // �ڑ��ݒ���Ăяo���܂��B�߂�܂���
#define WIFI_P2PMATCH_DPW_END    (10)   // ���E�ΐ�p�ݒ�I��
#define WIFI_P2PMATCH_POFIN      (11)   // �|�t�B�������Ăяo��
#define WIFI_P2PMATCH_FRONTIER   (12)   // �t�����e�B�A�Ăяo��
#define WIFI_P2PMATCH_BUCKET	(13)   // �o�P�b�g�Q�[���Ăяo��
#define WIFI_P2PMATCH_BALANCEBALL	(14)   // �ʏ��Q�[���Ăяo��
#define WIFI_P2PMATCH_BALLOON	(15)   // �΂�[��Q�[���Ăяo��

#ifdef WFP2P_DEBUG_EXON	// 
#define WIFI_P2PMATCH_BATTLEROOM	(16)   // �o�g�����[���Ăяo��
#define WIFI_P2PMATCH_MBATTLE_FREE	(17)   // �ʐM�ΐ�}���`�o�g���Ăяo��
#endif	// WFP2P_DEBUG_EXON


#endif //__WIFI_P2PMATCH_DEF_H__


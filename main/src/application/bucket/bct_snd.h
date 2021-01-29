//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		bct_snd.h
 *	@brief		���w�b�_
 *	@author		tomoya takahashi
 *	@data		2007.09.28
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __BCT_SND_H__
#define __BCT_SND_H__

#include "system/snd_tool.h"

#define BCT_SND_MARUIN				( SEQ_SE_DP_ELEBETA )		// �}���m�[���o��
//#define BCT_SND_MARUIN2			( SEQ_SE_DP_JIHANKI )		// �}���m�[���o��K�V����
//#define BCT_SND_MARUIN2			( SEQ_SE_DP_HAMARU )		// �}���m�[���o��K�V����
#define BCT_SND_MARUIN2				( SEQ_SE_DP_HAMARU )		// �}���m�[���o���������
#define BCT_SND_MARUIN3				( SEQ_SE_DP_KI_GASYAN )		// �}���m�[���o��͂܂�

#define BCT_SND_COUNT				( SEQ_SE_DP_DECIDE )		// �R�E�Q�E�P

//#define BCT_SND_START				( SEQ_SE_DP_DECIDE )		// �X�^�[�g
#define BCT_SND_START				( SEQ_SE_DP_CON_016 )		// �X�^�[�g
//#define BCT_SND_START				( SEQ_SE_DP_PIRORIRO2 )		// �X�^�[�g

#define BCT_SND_SLOW				( SEQ_SE_DP_FW104 )			// �����鉹
//#define BCT_SND_SLOW				( SEQ_SE_DP_DANSA5 )		// �����鉹

#define BCT_SND_EAT					( SEQ_SE_PL_NOMI02 )		// �H�ׂ鉹

//////////////////////////////////////////////////////////////////////////////
//#define BCT_SND_EAT100			( SEQ_SE_PL_NOMI02 )		// �H�ׂ鉹
//#define BCT_SND_EAT100				( SEQ_SE_DP_DENSI16 )		// �H�ׂ鉹
//#define BCT_SND_EAT100			( SEQ_SE_DP_KIRAKIRA3 )		// �H�ׂ鉹
#define BCT_SND_EAT100				( SEQ_SE_PL_POINT1 )		// �H�ׂ鉹

//////////////////////////////////////////////////////////////////////////////
//#define BCT_SND_EAT200			( SEQ_SE_PL_NOMI02 )		// �H�ׂ鉹
//#define BCT_SND_EAT200				( SEQ_SE_DP_DENSI16 )		// �H�ׂ鉹
//#define BCT_SND_EAT200			( SEQ_SE_DP_KIRAKIRA )		// �H�ׂ鉹
#define BCT_SND_EAT200				( SEQ_SE_PL_POINT2 )		// �H�ׂ鉹

//////////////////////////////////////////////////////////////////////////////
//#define BCT_SND_EAT300			( SEQ_SE_PL_NOMI02 )		// �H�ׂ鉹
//#define BCT_SND_EAT300				( SEQ_SE_DP_DENSI16 )		// �H�ׂ鉹
//#define BCT_SND_EAT300			( SEQ_SE_DP_DENSI06 )		// �H�ׂ鉹(��Ȃ�)
//#define BCT_SND_EAT300			( SEQ_SE_DP_KIRAKIRA )		// �H�ׂ鉹(��Ȃ�)
//#define BCT_SND_EAT300			( SEQ_SE_DP_KIRAKIRA3 )		// �H�ׂ鉹(��Ȃ�)
//#define BCT_SND_EAT300			( SEQ_SE_DP_KIRAKIRA4 )		// �H�ׂ鉹(��Ȃ�)
#define BCT_SND_EAT300				( SEQ_SE_PL_POINT3 )		// �H�ׂ鉹

//////////////////////////////////////////////////////////////////////////////
//#define BCT_SND_BOUND				( SEQ_SE_DP_DANSA4 )		// �͂�����
//#define BCT_SND_BOUND				( SEQ_SE_DP_CARD10 )		// �͂�����
//#define BCT_SND_BOUND				( SEQ_SE_DP_CON_001 )		// �͂�����
#define BCT_SND_BOUND				( SEQ_SE_DP_023 )			// �͂�����

#define BCT_SND_TIMEUP				( SEQ_SE_DP_PINPON )		// �^�C���A�b�v
//#define BCT_SND_TIMEUP				( SEQ_SE_DP_MAZYO )		// �^�C���A�b�v(��Ȃ�)

#define BCT_SND_FEVER_CHIME			( SEQ_SE_PL_PINPON )		// �t�B�[�o�[�O�̃`���C��
//#define BCT_SND_FEVER_CHIME			( SEQ_SE_DP_SELECT )		// �t�B�[�o�[�O�̃`���C��

//#define BCT_SND_FEVER_MOVE		( SEQ_SE_PL_BALLOON02 )		// �}���m�[���̃t�B�[�o�[����
//#define BCT_SND_FEVER_MOVE		( SEQ_SE_PL_BALLOON05 )		// �}���m�[���̃t�B�[�o�[����
#define BCT_SND_FEVER_MOVE			( SEQ_SE_PL_BALLOON05_2 )	// �}���m�[���̃t�B�[�o�[����

#endif		// __BCT_SND_H__


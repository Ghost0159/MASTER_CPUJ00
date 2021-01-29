//==============================================================================
/**
 * @file	bb_snd.h
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2007.10.01(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================

#ifndef __BB_SND_H__
#define __BB_SND_H__

#define BB_SND_COUNT		( SEQ_SE_DP_DECIDE )
#define BB_SND_START		( SEQ_SE_DP_DECIDE )

////////////////////////////////////////////////////////////////////////////////
//�ȉ��𓖂Ăĉ������B��낵���ł��B

#define BB_SND_KIRAKIRA		( SEQ_SE_PL_KIRAKIRA )	//�{�[���𓮂����ăL���L��

//���[�v�Ȃ̂Œ�~�������Ă�ł��������BSnd_SeStopBySeqNo(no,0);
#define BB_SND_HAND			( SEQ_SE_PL_HAND )		//����(���A��)����

#define BB_SND_KANSEI		( SEQ_SE_DP_DENDOU )	//����(�����̊J�n���ɍĐ�)

#define BB_SND_JUMP_IN		( SEQ_SE_DP_DANSA )		//�}�l�l���{�[���ɏ��
#define BB_SND_JUMP_OUT		( SEQ_SE_DP_DANSA )		//�}�l�l���{�[�����痎����
#define BB_SND_LANDING		( SEQ_SE_DP_SUTYA )		//�}�l�l���{�[�����痎���Ē��n
#define BB_SND_NEJI			( SEQ_SE_PL_KIN )		//�l�W�K�Ƃꂽ

//�������ŁA����͔������B
//#define BB_SND_HANABI		( SEQ_SE_PL_FW120 )		//�ԉ΂��o�邽�тɍĐ����Ă�������
#define BB_SND_HANABI		( SEQ_SE_PL_140_2_2 )	//�ԉ΂��o�邽�тɍĐ����Ă�������

#define BB_SND_LV_UP		( SEQ_SE_PL_LV_UP )		//���x���A�b�v

#endif

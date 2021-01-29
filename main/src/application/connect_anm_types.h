//==============================================================================
/**
 * @file	connect_anm_types.h
 * @brief	Wifi�ڑ���ʂ�BG�A�j���̍\���̒�`�Ȃ�
 * @author	matsuda
 * @date	2007.12.26(��)
 */
//==============================================================================
#ifndef __CONNECT_ANM_TYPES_H__
#define __CONNECT_ANM_TYPES_H__


//==============================================================================
//	�萔��`
//==============================================================================
///ncl�t�@�C����̃p���b�g�A�j�����J�n������p���b�g�J�n�ʒu
#define CBP_PAL_START_NUMBER	(0)
///�p���b�g�A�j���Ώۃp���b�g�{��
#define CBP_PAL_NUM			(4)
///�A�j���]���p���b�g�{��
#define CBP_TRANS_PAL_NUM	(1)

///�p���b�g�A�j���Ώۃp���b�g�̒��Ɋ܂܂�Ă�����ۂɃA�j���Ώۂ̃J���[�J�n�ʒu
#define CBP_PAL_COLOR_START	(1)	//CBP_PAL_COLOR_START �` (CBP_PAL_COLOR_START + CBP_PAL_COLOR_NUM)
///�p���b�g�A�j���Ώۃp���b�g�̒��Ɋ܂܂�Ă�����ۂɃA�j���Ώۂ̃J���[��
#define CBP_PAL_COLOR_NUM	(15)

///EVY���Z�l(����8�r�b�g����)
#define CBP_ADD_EVY			(0x0300)

///EVY�p�^�[����
#define CBP_EVY_ANM_NUM		((16<<8) / CBP_ADD_EVY + 2)	// +1 = �[���� +1=0�̕�
///EVY�p�^�[����(�S�A�j���p�^�[����)
#define CBP_EVY_TBL_ALL		(CBP_EVY_ANM_NUM * (CBP_PAL_NUM - 1))


//==============================================================================
//	�\���̒�`
//==============================================================================
///Wifi�ڑ�BG�̃p���b�g�A�j������\����
typedef struct{
	TCB_PTR tcb;
	BOOL occ;							///<TRUE:�L���@FALSE:����
	u16 src_color[CBP_PAL_NUM][16];		///<���p���b�g
	u16 dest_color[CBP_EVY_TBL_ALL][16];	///<�]���p���b�g
	
	s16 trans_pos;
	s8 trans_dir;
	u8 intr_count;
}CONNECT_BG_PALANM;


#endif	//__CONNECT_ANM_TYPES_H__

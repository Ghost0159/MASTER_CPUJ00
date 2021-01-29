//==============================================================================
/**
 * @file	fss_test.h
 * @brief	�ȒP�Ȑ���������
 * @author	matsuda
 * @date	2007.04.04(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#ifndef __FSS_TEST_H__
#define __FSS_TEST_H__


//==============================================================================
//	�萔��`
//==============================================================================
//--------------------------------------------------------------
//	�t���[���ԍ�
//--------------------------------------------------------------
///�E�B���h�E�E���b�Z�[�W�ʂ̃t���[��
#define FSSTEST_FRAME_WIN				(GF_BGL_FRAME1_M)
///�G�t�F�N�g�ʂ̃t���[��
#define FSSTEST_FRAME_EFF				(GF_BGL_FRAME2_M)
///�w�i�ʂ̃t���[��
#define FSSTEST_FRAME_BACKGROUND			(GF_BGL_FRAME3_M)

///�T�u��ʁF�ϋq�ʂ̃t���[��
#define FSSTEST_FRAME_SUB_AUDIENCE		(GF_BGL_FRAME0_S)

///BG�ԍ��F�E�B���h�E
#define FSSTEST_BGNO_WIN			(1)
///BG�ԍ��F�G�t�F�N�g
#define FSSTEST_BGNO_EFF			(2)
///BG�ԍ��F�w�i
#define FSSTEST_BGNO_BACKGROUND	(3)

///GX_WND�ԍ��F�G�t�F�N�g
#define FSSTEST_GX_WND_EFF			(GX_WND_PLANEMASK_BG2)

///<3D�ʂ�BG�v���C�I���e�B
#define FSSTEST_3DBG_PRIORITY		(1)
///�E�B���h�E�ʂ�BG�v���C�I���e�B
#define FSSTEST_BGPRI_WIN			(2)
///�G�t�F�N�g�ʂ�BG�v���C�I���e�B
#define FSSTEST_BGPRI_EFF			(1)
///�w�i�ʂ�BG�v���C�I���e�B
#define FSSTEST_BGPRI_BACKGROUND		(3)

///<�T�u��ʁF�w�i�ʂ�BG�v���C�I���e�B
///�ϋq�ʂ�BG�v���C�I���e�B
#define FSSTEST_BGPRI_SUB_AUDIENCE	(3)



#endif	//__FSS_TEST_H__

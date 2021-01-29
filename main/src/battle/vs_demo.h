//============================================================================================
/**
 * @file	vs_demo.h
 * @brief	�ʐM�ΐ�f��
 * @author	Hiroyuki Nakamura
 * @date	06.04.26
 */
//============================================================================================
#ifndef VS_DEMO_H
#define VS_DEMO_H
#undef GLOBAL
#ifdef VS_DEMO_H_GLOBAL
#define GLOBAL	/*	*/
#else
#define GLOBAL	extern
#endif


//============================================================================================
//	�萔��`
//============================================================================================

// �\���ʒu
enum {
	VSD_PARTY_LEFT1 = 0,	// �����p�[�e�B�̃v���C���[�P
	VSD_PARTY_RIGHT1,		// �E���p�[�e�B�̃v���C���[�Q
	VSD_PARTY_LEFT2,		// �����p�[�e�B�̃v���C���[�P
	VSD_PARTY_RIGHT2,		// �E���p�[�e�B�̃v���C���[�Q
	VSD_PARTY_MAX
};

// �O���쐬�f�[�^
typedef struct {
	BATTLE_PARAM * bp;				// �퓬�p�����[�^
	POKEPARTY * pp[VSD_PARTY_MAX];	// �|�P�����f�[�^
	STRBUF * name[VSD_PARTY_MAX];	// �v���[���[��
	u32	heap;		// �q�[�vID
	u8	mode;		// �f�����[�h
	u8	type;		// �f���^�C�v
	u8	result;		// �ΐ팋��
	u8	end_flg;	// �I���Ď��t���O
	
	u8 rec_mode;	// �^�惂�[�h
	u8 dummy[3];
}VS_DEMO_DATA;

// �\�����[�h
enum {
	VSD_MODE_MULTI = 0,		// �Q���|�P�����m�F
	VSD_MODE_IN,			// �ΐ�J�n
	VSD_MODE_END			// �ΐ팋��
};

// �f���^�C�v
enum {
	VSD_TYPE_NORMAL = 0,	// �ΐ�i�ʏ�j
	VSD_TYPE_MULTI,			// �ΐ�i�}���`�j

	VSD_TYPE_LEFT = 0,		// �Q���m�F�E���i�����j
	VSD_TYPE_RIGHT			// �Q���m�F�E�E�i�����j
};

// ����
enum {
	VSD_RESULT_WIN = 1,		// ����
	VSD_RESULT_LOSE,		// ����
	VSD_RESULT_DRAW,		// ��������
};


//============================================================================================
//	�v���g�^�C�v�錾
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �^�X�N�ǉ�
 *
 * @param	dat		�o�b�O�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
extern void VSDemo_TaskAdd( VS_DEMO_DATA * dat );


#undef GLOBAL
#endif	/* VS_DEMO_H */

//============================================================================================
/**
 * @file	nuts_tag.h
 * @brief	�؂̎��^�O��ʏ���
 * @author	Hiroyuki Nakamura
 * @date	06.01.25
 */
//============================================================================================
#ifndef NUTS_TAG_H
#define NUTS_TAG_H
#undef GLOBAL
#ifdef NUTS_TAG_H_GLOBAL
#define GLOBAL	/*	*/
#else
#define GLOBAL	extern
#endif


//============================================================================================
//	�萔��`
//============================================================================================
typedef struct _NTAG_DATA	NTAG_DATA;

enum {
	TAG_OFF = 0,
	TAG_ON
};


//============================================================================================
//	�O���[�o���ϐ�
//============================================================================================


//============================================================================================
//	�v���g�^�C�v�錾
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �؂̎��^�O��ʂ̃f�[�^�̈�m��
 *
 * @param	heap	�q�[�v�h�c
 *
 * @return	�m�ۂ����̈�̃A�h���X
 */
//--------------------------------------------------------------------------------------------
GLOBAL NTAG_DATA * NutsTagSystemDataAlloc( u8 heap );
GLOBAL u32 NutsTagSystemDataSizeGet(void);


//--------------------------------------------------------------------------------------------
/**
 * ���݂̕\���ʒu�擾
 *
 * @param	dat		�f�[�^�ݒ�ꏊ
 *
 * @return	�\���ʒu
 */
//--------------------------------------------------------------------------------------------
GLOBAL void TagPosGet( NTAG_DATA * dat, u8 * scr, u8 * cur );

//--------------------------------------------------------------------------------------------
/**
 * �؂̎��^�O��ʂ̕\���^�O�ݒ�
 *
 * @param	dat		�f�[�^�ݒ�ꏊ
 * @param	item	�A�C�e���ԍ�
 * @param	flg		�����^�O�ݒ�t���O
 *
 * @return	none
 *
 * @li	flg = TAG_ON : �ŏ��ɕ\�������^�O�ɐݒ�
 */
//--------------------------------------------------------------------------------------------
GLOBAL void NutsTagPutDataSet( NTAG_DATA * dat, u16 item, u16 flg );

GLOBAL void NutsTagBagCursorSet( NTAG_DATA * dat, u8 scr, u8 cur, u8 max );

//--------------------------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F������
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//--------------------------------------------------------------------------------------------
GLOBAL PROC_RESULT NutsTagProc_Init( PROC * proc, int * seq );

//--------------------------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F���C��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//--------------------------------------------------------------------------------------------
GLOBAL PROC_RESULT NutsTagProc_Main( PROC * proc, int * seq );

//--------------------------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F�I��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//--------------------------------------------------------------------------------------------
GLOBAL PROC_RESULT NutsTagProc_End( PROC * proc, int * seq );

//--------------------------------------------------------------------------------------------
/**
 * �؂̎��̊G�̃L�����̃A�[�J�C�u�C���f�b�N�X�擾
 *
 * @param	id		�؂̎��ԍ�
 *
 * @return	�L�����̃A�[�J�C�u�C���f�b�N�X
 */
//--------------------------------------------------------------------------------------------
GLOBAL u32 NutsCgxArcGet( u32 id );

//--------------------------------------------------------------------------------------------
/**
 * �؂̎��̊G�̃L�����̃A�[�J�C�u�C���f�b�N�X�擾�i�A�C�e���ԍ��j
 *
 * @param	item	�A�C�e���ԍ�
 *
 * @return	�L�����̃A�[�J�C�u�C���f�b�N�X
 */
//--------------------------------------------------------------------------------------------
GLOBAL u32 NutsCgxArcGet_Item( u32 item );

//--------------------------------------------------------------------------------------------
/**
 * �؂̎��̊G�̃p���b�g�̃A�[�J�C�u�C���f�b�N�X�擾
 *
 * @param	id		�؂̎��ԍ�
 *
 * @return	�p���b�g�̃A�[�J�C�u�C���f�b�N�X
 */
//--------------------------------------------------------------------------------------------
GLOBAL u32 NutsPalArcGet( u32 id );

//--------------------------------------------------------------------------------------------
/**
 * �؂̎��̊G�̃p���b�g�̃A�[�J�C�u�C���f�b�N�X�擾�i�A�C�e���ԍ��j
 *
 * @param	item	�A�C�e���ԍ�
 *
 * @return	�p���b�g�̃A�[�J�C�u�C���f�b�N�X
 */
//--------------------------------------------------------------------------------------------
GLOBAL u32 NutsPalArcGet_Item( u32 item );


#undef GLOBAL
#endif	/* NUTS_TAG_H */
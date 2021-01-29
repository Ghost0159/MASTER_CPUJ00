//============================================================================================
/**
 * @file	bag_obj.h
 * @brief	�o�b�O���OBJ����
 * @author	Hiroyuki Nakamura
 * @date	05.10.31
 */
//============================================================================================
#ifndef BAG_OBJ_H
#define BAG_OBJ_H
#undef GLOBAL
#ifdef BAG_OBJ_H_GLOBAL
#define GLOBAL	/*	*/
#else
#define GLOBAL	extern
#endif


//============================================================================================
//	�萔��`
//============================================================================================
#define	CURSOR_ACT_PX	( 177 )		// �A�C�e���I���J�[�\���\��X���W
#define	CURSOR_ACT_PY	( 24 )		// �A�C�e���I���J�[�\���\��Y���W

enum {
	ACT_OFF = 0,	// �A�N�^�[��\��
	ACT_ON			// �A�N�^�[�\��
};


//============================================================================================
//	�v���g�^�C�v�錾
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �Z���A�N�^�[������
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void BagCellActorInit( BAG_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * �Z���A�N�^�[���
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void BAGOBJ_ClactFree( BAG_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * �A�C�e���A�C�R���ύX
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 * @param	item	�A�C�e���ԍ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void BagItemIconChange( BAG_WORK * wk, u16 item );

//--------------------------------------------------------------------------------------------
/**
 * �A�C�e���I���J�[�\���̃p���b�g�ύX
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void BagObj_CursorPalChg( BAG_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * �A�j��
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void Bag_ObjAnmMain( BAG_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * �|�P�b�g�J�[�\������`�F�b�N
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 *
 * @retval	"TRUE = ��~��"
 * @retval	"FALSE = ���쒆"
 */
//--------------------------------------------------------------------------------------------
GLOBAL u8 BagPocketCursorMoveCheck( BAG_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * �|�P�b�g�J�[�\�����쏉����
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void BagPocketCursorMoveInit( BAG_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * �|�P�b�g�J�[�\�����상�C��
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void BagPocketCursorMoveMain( BAG_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * ���I����Z�b�g
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 * @param	mode	�g�p���[�h
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void Bag_NumArrowSet( BAG_WORK * wk, u8 mode );

//--------------------------------------------------------------------------------------------
/**
 * ���I����\���ؑ�
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 * @param	flg		�\���t���O
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void Bag_NumArrowPut( BAG_WORK * wk, u8 flg );

//--------------------------------------------------------------------------------------------
/**
 * �Z�}�V���̃A�C�R���Z�b�g
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 * @param	item	�A�C�e���ԍ�
 * @param	flg		�\���t���O
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void BagObj_WazaIconPut( BAG_WORK * wk, u16 item, u8 flg );

//--------------------------------------------------------------------------------------------
/**
 * �T�u�{�^���G�t�F�N�g������
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 * @param	x		�\��X���W
 * @param	y		�\��Y���W
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void BagObj_SubButtonEffInit( BAG_WORK * wk, s16 x, s16 y );

//--------------------------------------------------------------------------------------------
/**
 * �T�u�{�^���G�t�F�N�g���C��
 *
 * @param	wk		�o�b�O��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void BagObj_SubButtonEffMain( BAG_WORK * wk );



#undef GLOBAL
#endif	/* BAG_OBJ_H */
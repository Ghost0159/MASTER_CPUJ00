//==============================================================================
/**
 *		���ʒ�`
 */
//==============================================================================
#ifndef __GT_GDS_DEFINE_H__
#define __GT_GDS_DEFINE_H__


//==============================================================================
//	�萔��`
//==============================================================================
#define  GT_PMS_WORD_MAX		(2)			///< �����Ɋ܂܂��P��̍ő吔

//==============================================================================
//	�\���̒�`
//==============================================================================

//--------------------------------------------------------------
/**
 *	���b�Z�[�W�^��`
 */
//--------------------------------------------------------------
typedef u16		GT_STRCODE;

//------------------------------------------------------
/**
 *  ���͌^��`
 */
//------------------------------------------------------
typedef struct {
	u16				sentence_type;			///< ���̓^�C�v
	u16				sentence_id;			///< �^�C�v��ID
	u16				word[GT_PMS_WORD_MAX];		///< �P��ID
}GT_PMS_DATA;

//--------------------------------------------------------------
/**
 *	CRC�^��`
 */
//--------------------------------------------------------------
typedef struct {
	u16				crc16ccitt_hash;		///< CRC-16/CCITT �̃n�b�V���l
	u16				dummy;
}GT_GDS_CRC;

///GT_GDS_CRC�̃T�C�Y
#define GT_GDS_CRC_SIZE			(sizeof(GT_GDS_CRC))


#endif	//__GT_GDS_DEFINE_H__
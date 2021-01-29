//============================================================================================
/**
 * @file	placename.h
 * @brief	�|�P�����ߊl�ꏊ�f�[�^���
 * @author	taya
 * @date	2006.05.20
 *
 */
//============================================================================================
#ifndef __PLACENAME_H__
#define __PLACENAME_H__

//------------------------------------------------------
/**
 *	�ߊl�ꏊ�^�C�v
 */
//------------------------------------------------------
typedef enum {
	PLACENAME_TYPE_MAP = 0,		///< �}�b�v�ŃQ�[�����n���Ƃ��Ďg���Ă���
	PLACENAME_TYPE_GAME_EVENT,	///< �Q�[�����ł̃C�x���g�B��Ă�v�w�A�ʐM�����ȂǁB
	PLACENAME_TYPE_EXT_EVENT,	///< �O���z�z�C�x���g�B�|�P�����t�F�X�^�ȂǁB

	PLACENAME_TYPE_MAX,
}PLACENAME_TYPE;


///DP�ɑ��݂��Ă���n���͈́F�m�[�}������
#define PLACE_RANGE_DP_NORMAL_LOW		(1)
///DP�ɑ��݂��Ă���n���͈́F�m�[�}�����
#define PLACE_RANGE_DP_NORMAL_HIGH		(111)
///DP�ɑ��݂��Ă���n���͈́F���ꉺ��
#define PLACE_RANGE_DP_TOKUSYU_LOW		(2000)
///DP�ɑ��݂��Ă���n���͈́F������
#define PLACE_RANGE_DP_TOKUSYU_HIGH		(2010)
///DP�ɑ��݂��Ă���n���͈́F�z�z����
#define PLACE_RANGE_DP_HAIFU_LOW		(3000)
///DP�ɑ��݂��Ă���n���͈́F�z�z���
#define PLACE_RANGE_DP_HAIFU_HIGH		(3076)


//------------------------------------------------------------------
/**
 * �ߊl�ꏊ�i���o�[����ߊl�ꏊ�^�C�v���擾
 *
 * @param   number		�ߊl�ꏊ�i���o�[�i�|�P�����p�����[�^�Ƃ��ăZ�b�g����Ă���l�j
 *
 * @retval  PLACENAME_TYPE		�ߊl�ꏊ�^�C�v
 */
//------------------------------------------------------------------
extern PLACENAME_TYPE  PlaceName_GetType( u32 number );


//------------------------------------------------------------------
/**
 * �ߊl�ꏊ�i���o�[����A�e�ߊl�ꏊ�^�C�v���Ƃ̃C���f�b�N�X�l��Ԃ�
 * ���߂�l��gmm���當����擾����ۂ̃C���f�b�N�X�Ƃ��Ďg����
 *
 * @param   number		�ߊl�ꏊ�i���o�[�i�|�P�����p�����[�^�Ƃ��ăZ�b�g����Ă���l�j
 *
 * @retval  int		�ߊl�ꏊ�^�C�v���Ƃ̃C���f�b�N�X
 */
//------------------------------------------------------------------
extern int PlaceName_GetIndex( u32 number );


//------------------------------------------------------------------
/**
 * �ߊl�ꏊ�^�C�v���C���f�b�N�X����A�|�P�����p�����[�^�Ƃ��ăZ�b�g���邽�߂̒l���쐬
 *
 * @param   type		�ߊl�ꏊ�^�C�v
 * @param   index		�^�C�v���Ƃ̃C���f�b�N�X
 *
 * @retval  int			�|�P�����p�����[�^�Ƃ��ăZ�b�g���邽�߂̒l
 */
//------------------------------------------------------------------
extern int PlaceName_IndexToParamNumber( PLACENAME_TYPE type, int index );

//--------------------------------------------------------------
/**
 * @brief   �ߊl�ꏊ��DP�̃G���A�������ׂ�
 *
 * @param   place_id		�ߊl�ꏊ�i���o�[�i�|�P�����p�����[�^�Ƃ��ăZ�b�g����Ă���l�j
 *
 * @retval  TRUE:DP�ő��݂��Ă���ߊl�ꏊ
 * @retval  FALSE:DP�ł͑��݂��Ă��Ȃ�
 */
//--------------------------------------------------------------
extern BOOL PlaceName_RangeCheckDP(u16 number);


#endif /* #ifndef __PLACENAME_H__ */

//============================================================================================
/**
 * @file	regulation.h
 * @brief	�o�g�����M�����[�V�����f�[�^�A�N�Z�X�p�w�b�_
 * @author	k.ohno
 * @date	2006.5.24
 */
//============================================================================================
#ifndef __REGULATION_DATA_H__
#define __REGULATION_DATA_H__

#include "regulation.h"


//==============================================================================
//	�萔��`
//==============================================================================
///���M�����[�V����No
enum{
	REGULATION_NO_STANDARD,		///<�X�^���_�[�h�J�b�v
	REGULATION_NO_FANCY,		///<�t�@���V�[�J�b�v
	REGULATION_NO_LITTLE,		///<���g���J�b�v
	REGULATION_NO_LIGHT,		///<���C�g�J�b�v
	REGULATION_NO_DOUBLE,		///<�_�u���J�b�v
	REGULATION_NO_ETC,			///<�J�X�^���J�b�v(���̑�)
	
	REGULATION_NO_NULL = 0xff,	///<���M�����[�V��������
};


//==============================================================================
//	�O���֐��錾
//==============================================================================
// * @brief	���M�����[�V�����f�[�^�ւ̃|�C���^�擾
extern const REGULATION* Data_GetRegulation(SAVEDATA* pSave, int regNo);
// * @brief	���M�����[�V�����f�[�^�̖��O�擾
extern void Data_GetRegulationName(SAVEDATA* pSave, int regNo, STRBUF* pStrBuff, int HeapID);
//���������̃��M�����[�V�����f�[�^�ւ̃|�C���^�擾
extern const REGULATION* Data_GetNoLimitRegulation(void);
//���M�����[�V�����f�[�^���烌�M�����[�V����NO���擾
extern int Data_GetRegulationNo(const REGULATION *reg);


#endif //__REGULATION_DATA_H__

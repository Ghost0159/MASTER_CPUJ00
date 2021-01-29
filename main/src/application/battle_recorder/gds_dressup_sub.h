//==============================================================================
/**
 *
 * GDS�h���X�A�b�v�̃T�u�\���̂�萔��`
 *
 */
//==============================================================================
#ifndef __GT_DRESSUP_SUB_H__
#define __GT_DRESSUP_SUB_H__

#include "gds_define.h"

//==============================================================================
//	�萔��`
//==============================================================================
#define GT_IMC_SAVEDATA_NICKNAME_STR_NUM	(11)	///<�j�b�N�l�[���̕�����
#define GT_IMC_SAVEDATA_OYANAME_STR_NUM 	(8)		///<�e�l�[���̕�����
#define GT_IMC_SAVEDATA_STRBUF_NUM	  		(12)	///<STRBUFF�̕K�v�v�f��
#define	GT_IMC_RBOX_OBJ_MAX					(11)	// �E�{�b�N�X�̃I�u�W�F�N�g�o�^�ő吔
													// �A�N�Z�T��10 + �|�P����1
#define GT_IMC_SAVEDATA_TELEVISION_ACCE_NUM	( GT_IMC_RBOX_OBJ_MAX - 1 )		///<�e���r�ǃA�N�Z�T���o�^��


//==============================================================================
//	�\���̒�`
//==============================================================================
//-------------------------------------
//	�h���X�A�b�v�p�|�P�����f�[�^
//=====================================
typedef struct {
	u32 personal_rnd;				///<�|�P�����̌�����
	u32 idno;						///<IDNo	���A�|�P�����p
	u16	monsno;						///<�����X�^�[�i���o�[
	u16 oyaname[ GT_IMC_SAVEDATA_OYANAME_STR_NUM ];	///<�j�b�N�l�[��
	s8 poke_pri;					///<�|�P�����D�揇��
	u8 poke_x;						///<�|�P���������W
	u8 poke_y;						///<�|�P���������W
	u8 form_id;					///<�`��A�g���r���[�g
}GT_IMC_SAVEDATA_POKEMON;

//-------------------------------------
//	�h���X�A�b�v�p�A�N�Z�T���f�[�^�\����
//=====================================
typedef struct {
	u8 accessory_no;	///<�A�N�Z�T���i���o�[
	u8 accessory_x;		///<�A�N�Z�T�������W
	u8 accessory_y;		///<�A�N�Z�T�������W
	s8 accessory_pri;	///<�A�N�Z�T���\���D��
}GT_IMC_SAVEDATA_ACCESSORIE;



#endif	//__GT_DRESSUP_SUB_H__

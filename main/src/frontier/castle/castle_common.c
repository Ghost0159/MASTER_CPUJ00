//==============================================================================================
/**
 * @file	castle_common.c
 * @brief	�u�o�g���L���b�X���v���ʏ���
 * @author	Satoshi Nohara
 * @date	07.10.22
 */
//==============================================================================================
#include "system/main.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "system/snd_tool.h"
#include "system/pm_str.h"
#include "system/buflen.h"
#include "system/wordset.h"
#include "system/bmp_list.h"
#include "poketool/pokeparty.h"
#include "savedata/frontier_savedata.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_info.h"
#include "communication/comm_def.h"

#include "gflib/blact.h"
#include "../../field/fieldobj.h"
#include "../../field/scr_tool.h"

#include "castle_common.h"

#include "system/pm_overlay.h"
FS_EXTERN_OVERLAY(frontier_common);


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
u8 GetCsrPokePos( u8 h_max, u8 csr_pos );
u8 GetCsrSelType( u8 h_max, u8 csr_pos );
u8 GetCommSelCsrPos( u8 type_offset, u8 decide_type );
u8 Castle_GetRank( SAVEDATA* sv, u8 type, u8 rank_type );
void Castle_ListSeCall( u32 param, u16 no );
void Castle_SetPairName( WORDSET* wordset, u32 id );

//--------------------------------------------------------------
/**
 * @brief	�G���g���[�|�P�����̉��C�ڂ��擾
 *
 * @param	h_max	���̐�
 * @param	csr_pos	�J�[�\���ʒu
 *
 * @return	"���C�ڂ�"
 */
//--------------------------------------------------------------
u8 GetCsrPokePos( u8 h_max, u8 csr_pos )
{
	//return (csr_pos % h_max);
	return csr_pos;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ʒu����u�����ӂ��v�u�ǂ����v�u�悳�v�u�킴�v�̂ǂ��I���������擾
 * @brief	�J�[�\���ʒu����u�Ă����v�u���x���v�u�悳�v�u�킴�v�̂ǂ��I���������擾
 * @brief	�J�[�\���ʒu����u�����ӂ��v�u�ǂ����v�u���傤�ق��v�̂ǂ��I���������擾
 *
 * @param	h_max	���̐�
 * @param	csr_pos	�J�[�\���ʒu
 *
 * @return	"���"
 */
//--------------------------------------------------------------
u8 GetCsrSelType( u8 h_max, u8 csr_pos )
{
	return (csr_pos / h_max);
}

//--------------------------------------------------------------
/**
 * @brief	�ʐM�̑I�񂾃J�[�\���ʒu���擾
 *
 * @param	hv_max	�c���̐�
 * @param	csr_pos	�J�[�\���ʒu
 *
 * @return	"�J�[�\���ʒu"
 */
//--------------------------------------------------------------
u8 GetCommSelCsrPos( u8 type_offset, u8 decide_type )
{
	//�I�t�Z�b�g���O��
	if( decide_type < type_offset ){
		return decide_type;
	}

	return (decide_type - type_offset);
}

//--------------------------------------------------------------
/**
 * @brief	���݂̃����N�擾
 *
 * @param	score_sv	CASTLESCORE�^�̃|�C���^
 * @param	type		�o�g���^�C�v
 * @param	rank_type	�ǂ̃����N���擾���邩(�����ӂ��A�����^���A���傤�ق�)
 *
 * @return	"�����N"
 */
//--------------------------------------------------------------
u8 Castle_GetRank( SAVEDATA* sv, u8 type, u8 rank_type )
{
	return FrontierRecord_Get(	SaveData_GetFrontier(sv), 
						CastleScr_GetRankRecordID(type,rank_type),
						Frontier_GetFriendIndex(CastleScr_GetRankRecordID(type,rank_type)) );
}

//--------------------------------------------------------------
/**
 * @brief	BMPLIST_NULL����Ȃ����͋��ʂ̌��ʉ���炷
 *
 * @param	param		
 * @param	no			SE�i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
void Castle_ListSeCall( u32 param, u16 no )
{
	if( param != BMPLIST_NULL ){
		Snd_SePlay( no );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p�[�g�i�[�����Z�b�g
 *
 * @param	wordset		WORDSET�^�̃|�C���^
 * @param	id			bufID
 *
 * @return	none
 */
//--------------------------------------------------------------
void Castle_SetPairName( WORDSET* wordset, u32 id )
{
	MYSTATUS* my;
	u32 col;

	//�p�[�g�i�[��MyStatus�擾
	my = CommInfoGetMyStatus( (CommGetCurrentID() ^ 1) );

	WORDSET_RegisterPlayerName( wordset, id, my );
	return;
}



//============================================================================================
/**
 * @file	scr_tool.h
 * @bfief	�X�N���v�g�Ŏg�p����v���O����
 * @author	Satoshi Nohara
 * @date	05.11.16
 */
//============================================================================================
#ifndef __SCR_TOOL_H__
#define __SCR_TOOL_H__

#include "fieldobj.h"

//============================================================================================
//
//	extern�錾
//
//============================================================================================
#ifdef PM_DEBUG
extern u8 debug_frontier_key_set;
#endif

//============================================================================================
//
//	���̑�
//
//============================================================================================

//--------------------------------------------------------------
/**
 * �n���ꂽ�l�̌������擾
 *
 * @param   num			�l
 *
 * @retval  "����"
 */
//--------------------------------------------------------------
extern u16 GetNumKeta(u32 num);

//--------------------------------------------------------------
/**
 * �Z�}�V���̃A�C�e���i���o�[���`�F�b�N
 *
 * @param   itemno		�A�C�e���i���o�[
 *
 * @retval  TRUE		�Z�}�V��
 * @retval  FALSE		����ȊO
 */
//--------------------------------------------------------------
extern u16 WazaMachineItemNoCheck( u16 itemno );


//============================================================================================
//
//	�}�ӕ]��
//
//============================================================================================

//--------------------------------------------------------------
/**
 * �V���I�E�}�ӕ]�����b�Z�[�WID�擾
 *
 * @param   num		�B����������������
 * @param	c04_arrive	c04�n�N�^�C�V�e�B�̓����t���O
 *
 * @retval  "���b�Z�[�WID"
 */
 //--------------------------------------------------------------
extern u16 GetShinouZukanHyoukaMsgID( u16 num ,u16 c04_arrive);

//--------------------------------------------------------------
/**
 * �]�����b�Z�[�WID�擾
 *
 * @param   num		�B�����������߂܂�����
 * @param	sex		��l���̐���
 *
 * @retval  "���b�Z�[�WID"
 */
//--------------------------------------------------------------
extern u16 GetZenkokuZukanHyoukaMsgID( u16 num ,u16 sex);


//==============================================================================================
//
//	�S�Ŋ֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�Q�[���I�[�o�[��ʌĂяo��
 *
 * @param	fsys	FIELDSYS_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
extern void GameOverCall( FIELDSYS_WORK* fsys, GMEVENT_CONTROL* event );



//--------------------------------------------------------------
/**
 * �擪�|�P�����Z�o
 *
 * @param   sv		�|�P�����Z�[�u���\����
 *
 * @retval  �ԍ�
 */
//--------------------------------------------------------------
extern u16 GetFrontPokemon(SAVEDATA * sv);
//--------------------------------------------------------------
/**
 *	@brief	���W�L���O�C�x���g�@�L���O����ł��邩�`�F�b�N
 *
 *	�莝���ɃA�C�X�E���b�N�E�X�`�������邩�ǂ����H
 */
//--------------------------------------------------------------
extern BOOL EventCheck_IsReziUnseal(SAVEDATA* sv);

//--------------------------------------------------------------
/**
 * @brief	�t�B�[���h�I�u�W�F��h�炷�C�x���g 
 * @param	ev	GMEVENT_CONTROL *
 * @param	obj	FIELD_OBJ_PTR	�t�B�[���h�I�u�W�F�ւ̃|�C���^
 * @param	time	�h�炷��
 * @param	spd		�h�炷�X�s�[�h(360������؂�鐔�����悢)
 * @param	ofsx	�h�炷��X	
 * @param	ofsz	�h�炷��Z	
 * @retval	BOOL	TRUE=�C�x���g�I��
 */
//--------------------------------------------------------------
extern void EventCmd_ObjShakeAnm( GMEVENT_CONTROL *event,FIELD_OBJ_PTR obj,
		u16 time,u16 spd,u16 ofsx,u16 ofsz);

//--------------------------------------------------------------
/**
 * @brief	�t�B�[���h�I�u�W�F��Blink������A�j���C�x���g 
 * @param	ev		GMEVENT_CONTROL *
 * @param	obj		FIELD_OBJ_PTR	�t�B�[���h�I�u�W�F�ւ̃|�C���^
 * @param	count	Blink�������
 * @param	time	onoff��؂�ւ���C���^�[�o��
 */
//--------------------------------------------------------------
extern void EventCmd_ObjBlinkAnm(GMEVENT_CONTROL *event,FIELD_OBJ_PTR obj,u16 count,u16 time);


//============================================================================================
//
//	�t�@�N�g���[�Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
extern int FactoryScr_GetWinRecordID( u8 level, u8 type );
extern int FactoryScr_GetMaxWinRecordID( u8 level, u8 type );
extern int FactoryScr_GetTradeRecordID( u8 level, u8 type );
extern int FactoryScr_GetMaxTradeRecordID( u8 level, u8 type );


//============================================================================================
//
//	�o�g���X�e�[�W�Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
extern int StageScr_GetWinRecordID( u8 type );
extern int StageScr_GetMaxWinRecordID( u8 type );
extern int StageScr_GetMonsNoRecordID( u8 type );
extern int StageScr_GetExMaxWinRecordID( u8 type );
extern int StageScr_GetTypeLevelRecordID( u8 type, u8 csr_pos );

//�^�C�v���x���̃��R�[�h���擾����
extern u16 StageScr_TypeLevelRecordGet( SAVEDATA* sv, u8 type, u8 csr_pos, u16* l_num, u16* h_num );

//�^�C�v���x���̃��R�[�h���Z�b�g����
extern void StageScr_TypeLevelRecordSet( SAVEDATA* sv, u8 type, u8 csr_pos, u8 num );


//============================================================================================
//
//	�o�g���L���b�X���Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
extern int CastleScr_GetRankRecordID( u8 type, u8 id );
extern int CastleScr_GetWinRecordID( u8 type );
extern int CastleScr_GetMaxWinRecordID( u8 type );
extern int CastleScr_GetCPRecordID( u8 type );
extern int CastleScr_GetUsedCPRecordID( u8 type );
extern int CastleScr_GetRemainderCPRecordID( u8 type );


//============================================================================================
//
//	�o�g���^���[�Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
//extern int TowerScr_CheckEntryPokeSub( u8* pos_tbl, u8 max, SAVEDATA* savedata );


//============================================================================================
//
//	�o�g�����[���b�g�Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
extern int RouletteScr_GetWinRecordID( u8 type );
extern int RouletteScr_GetMaxWinRecordID( u8 type );


//============================================================================================
//
//	�^���[�Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
extern int TowerScr_GetWinRecordID( u8 type );
extern int TowerScr_GetMaxWinRecordID( u8 type );


//
extern int Frontier_GetFriendIndex( u32 record_no );
extern u8 Frontier_GetPairRomCode( void );
extern u8 Frontier_CheckDPRomCode( SAVEDATA* sv );

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
extern u16 FrontierScrTools_CheckEntryPokeNum( u16 num, SAVEDATA* savedata );


#endif



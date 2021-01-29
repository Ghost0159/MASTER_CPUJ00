//============================================================================================
/**
 * @file	scr_stage_sub.c
 * @bfief	�X�N���v�g�R�}���h�F�o�g���X�e�[�W(��t�܂��)
 * @author	Satoshi Nohara
 * @date	06.07.19
 *
 * �X�N���v�g�֘A�Ƃ͐؂藣������������u��
 */
//============================================================================================
#include "common.h"

#include <nitro/code16.h> 
#include "system/pm_str.h"
#include "system\msgdata.h"			//MSGMAN_GetString
#include "system\wordset.h"			//WordSet_RegistPlayerName
#include "gflib/strbuf_family.h"	//����

//�ʐM
#include "communication/comm_def.h"
#include "communication/comm_tool.h"
#include "communication/comm_system.h"
#include "../frontier/comm_command_frontier.h"

#include "savedata/frontier_savedata.h"
#include "scr_stage_sub.h"
#include "scr_tool.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
void CommFldStageRecvMonsNo(int id_no,int size,void *pData,void *work);
void BattleStageSetNewChallenge( SAVEDATA* sv, STAGESCORE* wk, u8 type );


//============================================================================================
//
//	�ʐM
//
//============================================================================================

//--------------------------------------------------------------
/**
 * @brief   ��{��� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 *
 * �o�g���X�e�[�W���[�N���m�ی�A�ŏ��Ɏ󂯎��f�[�^
 */
//--------------------------------------------------------------
void CommFldStageRecvMonsNo(int id_no,int size,void *pData,void *work)
{
	int i,num;
	FLDSCR_STAGE_COMM* wk = work;
	//const u16* recv_buf = pData;
	const FLDSCR_STAGE_COMM* recv_buf = pData;

	OS_Printf( "******�t�B�[���h�X�e�[�W****** �����X�^�[�i���o�[��M\n" );
	OS_Printf( "id_no = %d\n", id_no );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "size = %d\n", size );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//����̎莝���̃|�P�����i���o�[���擾(�y�A���[�N�ɑ��)
	wk->pair_monsno = recv_buf->mine_monsno;
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief		�A�����t���O�N���A�A�A�����R�[�h�N���A�A�^�C�v�J�E���g�N���A
 * @param		core
 */
//--------------------------------------------------------------------------------------------
void BattleStageSetNewChallenge( SAVEDATA* sv, STAGESCORE* wk, u8 type )
{
	int i;
	u8 buf8[4];
	u16 l_num,h_num;

	//"10�A��(�N���A)�������t���O"�̃N���A�������o��
	buf8[0] = 0;
	STAGESCORE_PutScoreData( wk, STAGESCORE_ID_CLEAR_FLAG, type, 0, buf8 );

	//WIFI�̂ݓ���
	if( type == STAGE_TYPE_WIFI_MULTI ){
		FrontierRecord_Set(	SaveData_GetFrontier(sv), 
							FRID_STAGE_MULTI_WIFI_CLEAR_BIT,
							Frontier_GetFriendIndex(FRID_STAGE_MULTI_WIFI_CLEAR_BIT), 0 );
	}

	//"���݂̘A����"��0�ɂ���
	FrontierRecord_Set(	SaveData_GetFrontier(sv), 
						StageScr_GetWinRecordID(type),
						Frontier_GetFriendIndex(StageScr_GetWinRecordID(type)), 0 );

	//fssc_stage_sub.c�Ɉړ�
	//���݂̃o�g���^�C�v�̃^�C�v���x�����N���A����
	//for( i=0; i < STAGE_TR_TYPE_MAX ;i++ ){
	//	StageScr_TypeLevelRecordSet( sv, type, i, 0 );
	//}

	return;
}


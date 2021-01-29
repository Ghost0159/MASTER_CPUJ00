//============================================================================================
/**
 * @file	scr_roulette_sub.c
 * @bfief	�X�N���v�g�R�}���h�F�o�g�����[���b�g(��t�܂��)
 * @author	Satoshi Nohara
 * @date	07.09.06
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

#include "scr_roulette_sub.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
void CommFldRouletteRecvMonsNo(int id_no,int size,void *pData,void *work);


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
void CommFldRouletteRecvMonsNo(int id_no,int size,void *pData,void *work)
{
	int i,num;
	FLDSCR_ROULETTE_COMM* wk = work;
	//const u16* recv_buf = pData;
	const FLDSCR_ROULETTE_COMM* recv_buf = pData;

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
	wk->pair_monsno[0] = recv_buf->mine_monsno[0];
	wk->pair_monsno[1] = recv_buf->mine_monsno[1];
	return;
}



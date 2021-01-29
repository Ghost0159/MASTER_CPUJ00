//============================================================================================
/**
 * @file	fssc_sound.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�F�T�E���h
 * @author	Satoshi Nohara
 * @date	07.04.10
 */
//============================================================================================
#include "common.h"
#include "system/lib_pack.h"
#include "system/snd_tool.h"

#include "frontier_types.h"
#include "frontier_main.h"
#include "frontier_scr.h"
#include "frontier_tool.h"
#include "frontier_scrcmd.h"
#include "fss_task.h"


//============================================================================================
//
//	�v�����g�^�C�v�錾
//
//============================================================================================
static BOOL EvWaitSe(FSS_TASK * core);
static BOOL EvWaitMe( FSS_TASK * core );

BOOL FSSC_SePlay( FSS_TASK * core );
BOOL FSSC_SeStop( FSS_TASK * core );
BOOL FSSC_SeWait(FSS_TASK * core);
BOOL FSSC_MePlay( FSS_TASK * core );
BOOL FSSC_MeWait( FSS_TASK * core );
BOOL FSSC_BgmPlay( FSS_TASK * core );
BOOL FSSC_BgmStop( FSS_TASK * core );


//============================================================================================
//
//	�R�}���h
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �r�d��炷
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_SePlay( FSS_TASK * core )
{
	Snd_SePlay( FSSTGetWorkValue(core) );
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * �r�d���~�߂�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_SeStop( FSS_TASK * core )
{
	Snd_SeStopBySeqNo( FSSTGetWorkValue(core), 0 );
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * �r�d�I���҂�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	���1
 *
 * @li		�J�Ȃǂ̃��[�v���ŁA�������[�v�ɂȂ��Ă��܂��̂ŁASE�i���o�[�w�肵�āA������`�F�b�N�I
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_SeWait(FSS_TASK * core)
{
	//���z�}�V���̔ėp���W�X�^��SE�i���o�[���i�[
	core->reg[0] = FSSTGetWorkValue(core);
	FSST_SetWait( core, EvWaitSe );
	return 1;
}

//return 1 = �I��
static BOOL EvWaitSe(FSS_TASK * core)
{
	//if( Snd_SePlayCheckAll() == 0 ){
	if( Snd_SePlayCheck(core->reg[0]) == 0 ){
		return TRUE;
	}

	return FALSE;
};

//--------------------------------------------------------------------------------------------
/**
 * ME�Đ�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_MePlay( FSS_TASK * core )
{
	Snd_MePlay( FSSTGetU16(core) );
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * ME�I���҂�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_MeWait( FSS_TASK * core )
{
	FSST_SetWait( core, EvWaitMe );
	return 1;
}

//return 1 = �I��
static BOOL EvWaitMe( FSS_TASK * core )
{
	//�I���҂��̂�
	//if( Snd_MePlayCheck() == 0 ){
	
	//�I���҂��{�E�F�C�g���[�N�҂�
	if( Snd_MePlayCheckBgmPlay() == 0 ){ 
		return TRUE;
	}

	return FALSE;
};

//--------------------------------------------------------------------------------------------
/**
 * BGM�Đ�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_BgmPlay( FSS_TASK * core )
{
	Snd_BgmPlay( FSSTGetU16(core) );
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * BGM���~�߂�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_BgmStop( FSS_TASK * core )
{
	u16 music = FSSTGetU16(core);	//"���g�p"
	//Snd_BgmStop( music, 0 );
	Snd_BgmStop( Snd_NowBgmNoGet(), 0 );
	return 0;
}



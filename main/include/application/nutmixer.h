/**
 *	@file	nutmixer.h
 *	@brief	�i�b�c�~�L�T�[�@�O�����J�w�b�_
 *	@author	Miyuki Iwasawa
 *	@date	06.04.06
 */

#ifndef __H_NUTMIXER_H__
#define __H_NUTMIXER_H__

#include "savedata/savedata_def.h"
#include "savedata/poruto_util.h"
#include "savedata/mystatus.h"
#include "system/gamedata.h"
#include "itemtool/myitem.h"
#include "poketool/pokeparty.h"
#include "application/p_status.h"
#include "application/pokelist.h"
#include "savedata/fnote_mem.h"

//���[�J���f�[�^���p���\����
typedef struct _NUTMIXER_PARAM{
	int	ret_val;	///<�Ԃ�l
	int	sys_mode;	///<�V�X�e�����샂�[�h	�ʐM���P�l�p��

	PORUTO_BLOCK* poruSave;	///<�|���g�Z�[�u�f�[�^
	
	SAVEDATA*	SaveData;	///<�Z�[�u�f�[�^
	
	MYSTATUS	*mySave;	///<�X�e�[�^�X
	MYITEM		*itemSave;	///<�A�C�e��
	CONFIG		*configSave;	///<�R���t�B�O
	FNOTE_DATA * fnote;		///<�`���m�[�g

	BOOL wifi_pofin;	///< wifi�|�t�B����
	BOOL wifi_vchat;	///< wifiVCHAT�t���O
}NUTMIXER_PARAM;

///�I�[�o�[���C�v���Z�X
extern PROC_RESULT NutMixerProc_Init( PROC *proc,int *seq);
extern PROC_RESULT NutMixerProc_Main( PROC *proc,int *seq);
extern PROC_RESULT NutMixerProc_End( PROC *proc,int *seq);

#endif	//__H_NUTMIXER_H__
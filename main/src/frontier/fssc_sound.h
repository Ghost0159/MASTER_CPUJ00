//============================================================================================
/**
 * @file	fssc_sound.h
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�F�T�E���h
 * @author	Satoshi Nohara
 * @date	07.04.10
 */
//============================================================================================
#ifndef	__FSSC_SOUND_H__
#define	__FSSC_SOUND_H__


//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern BOOL FSSC_SePlay( FSS_TASK * core );
extern BOOL FSSC_SeStop( FSS_TASK * core );
extern BOOL FSSC_SeWait(FSS_TASK * core);
extern BOOL FSSC_MePlay( FSS_TASK * core );
extern BOOL FSSC_MeWait( FSS_TASK * core );
extern BOOL FSSC_BgmPlay( FSS_TASK * core );
extern BOOL FSSC_BgmStop( FSS_TASK * core );


#endif	//__FSSC_SOUND_H__



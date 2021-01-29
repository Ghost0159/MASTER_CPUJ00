//==============================================================================
/**
 * @file	balloon_sonans.h
 * @brief	���D����F�\�[�i���X���쐧��̃w�b�_
 * @author	matsuda
 * @date	2007.11.14(��)
 */
//==============================================================================
#ifndef __BALLOON_SONANS_H__
#define __BALLOON_SONANS_H__


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern SONANS_SYS_PTR Sonas_Init(BALLOON_GAME_PTR game);
extern void Sonans_Exit(BALLOON_GAME_PTR game, SONANS_SYS_PTR sns);
extern void Sonans_Update(BALLOON_GAME_PTR game, SONANS_SYS_PTR sns);
extern void Sonans_VBlank(BALLOON_GAME_PTR game, SONANS_SYS_PTR sns);


#endif	//__BALLOON_SONANS_H__

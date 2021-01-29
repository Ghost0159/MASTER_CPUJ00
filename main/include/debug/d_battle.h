
//==============================================================================================
/**
 * @file	d_battle.h
 * @brief	�퓬�p�f�o�b�O�c�[��
 * @author	sogabe
 * @date	2006.04.14
 */
//==============================================================================================

#ifndef	__D_BATTLE_H__
#define	__D_BATTLE_H__

extern	void	DebugBattleInit(BATTLE_WORK *bw);
extern	BOOL	DebugBattleMain(BATTLE_WORK *bw,BOOL *flag);
extern	BOOL	DebugBattlePokemonReshuffle(BATTLE_WORK *bw,int client_no,int *pos);

extern	void	DebugSoftSpritePosPut(BATTLE_WORK *bw);

extern	u32		DebugBattleFlag;
extern	u32		debug_battle_work;

//======================================================
//	DebugBattleFlag�p��`
//======================================================
#define	DBF_SPEEDUP	(0x00000001)	///<�퓬�������i�f�o�b�O�o�g���p�j

//======================================================
//	debug_battle_work�p��`
//======================================================
#define	DBW_RESTART	(0x00000001)	///<�퓬���ŏ������蒼��

#endif	__D_BATTLE_H__

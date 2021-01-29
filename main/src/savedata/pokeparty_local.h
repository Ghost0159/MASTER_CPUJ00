
//============================================================================================
/**
 * @file	pokeparty_local.h
 * @author	sogabe
 * @date	2007.03.14
 * @brief	�|�P�����莝���f�[�^�^
 *
 * �Z�[�u�f�[�^�֘A�̋��L�w�b�_�B�O�����J�͂��Ȃ��B
 */
//============================================================================================
#ifndef	__POKEPARTY_LOCAL_H__
#define	__POKEPARTY_LOCAL_H__

#include	"poketool/poke_tool_def.h"
#include	"system/gamedata.h"

//----------------------------------------------------------
/**
 * @brief	�|�P�����p�[�e�B�[�\���̂̒�`
 *
 * �ő��6�̂܂ł̃|�P�������܂Ƃ߂ĕێ��ł���悤�ɂȂ��Ă���B
 */
//----------------------------------------------------------
struct _POKEPARTY {
	///	�ێ��ł���|�P�������̍ő�
	int PokeCountMax;
	///	���ݕێ����Ă���|�P������
	int PokeCount;
	///	�|�P�����f�[�^
	struct pokemon_param member[TEMOTI_POKEMAX];
};

#endif	/* __POKEPARTY_LOCAL_H__ */

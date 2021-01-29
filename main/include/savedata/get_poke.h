//============================================================================================
/**
 * @file	get_poke.h
 * @brief	ポケモン入手時のセーブデータ処理
 * @author	taya
 * @date	2006.07.26
 */
//============================================================================================
#ifndef __GET_POKE_H__
#define __GET_POKE_H__

#include "savedata/savedata_def.h"

//------------------------------------------------------------------
/**
 * 図鑑の手に入れたフラグ、ポケッチのポケモンヒストリーなど、
 * 必要なセーブデータ関連処理をまとめて呼び出す
 *
 * @param   sv		セーブデータポインタ
 * @param   pp		ポケモンパラメータ
 */
//------------------------------------------------------------------
extern void SaveData_GetPokeRegister( SAVEDATA* sv, const POKEMON_PARAM* pp );


#endif /* __GET_POKE_H__ */


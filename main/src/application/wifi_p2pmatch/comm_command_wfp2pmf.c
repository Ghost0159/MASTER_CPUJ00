//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		comm_command_wfp2pmf.c
 *	@brief		２〜４人専用待合室	通信コマンド
 *	@author		tomoya takahashi
 *	@data		2007.05.24
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "communication/communication.h"
#include "comm_command_wfp2pmf_func.h"

#define __COMM_COMMAND_WFP2PMF_H_GLOBAL
#include "comm_command_wfp2pmf.h"

//-----------------------------------------------------------------------------
/**
 *					コーディング規約
 *		●関数名
 *				１文字目は大文字それ以降は小文字にする
 *		●変数名
 *				・変数共通
 *						constには c_ を付ける
 *						staticには s_ を付ける
 *						ポインタには p_ を付ける
 *						全て合わさると csp_ となる
 *				・グローバル変数
 *						１文字目は大文字
 *				・関数内変数
 *						小文字と”＿”と数字を使用する 関数の引数もこれと同じ
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					定数宣言
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					構造体宣言
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					プロトタイプ宣言
*/
//-----------------------------------------------------------------------------


// 対応するコールバック関数
static const CommPacketTbl _CommNutWFP2PMFPacketTbl[] = {
	//-------------------------------------
	//　ゲーム用
	//=====================================
	{ CommWFP2PMFGameResult, CommWFP2PMFGetWFP2PMF_COMM_RESULTSize, NULL },			///< 通信OK			親ー＞子
	{ CommWFP2PMFGameStart, CommWFP2PMFGetZeroSize, NULL },			///< ゲーム開始			親ー＞子
	{ CommWFP2PMFGameVchat, CommWFP2PMFGetWFP2PMF_COMM_VCHATSize, NULL },	///< VCHATデータ	親ー＞子
};

//----------------------------------------------------------------------------
/**
 *	@brief		コマンドテーブルを取得
 */
//-----------------------------------------------------------------------------
const CommPacketTbl* WFP2PMF_CommCommandTclGet( void )
{
	return _CommNutWFP2PMFPacketTbl;
}

//----------------------------------------------------------------------------
/**
 *	@brief	コマンドテーブル要素数を取得
 */
//-----------------------------------------------------------------------------
int WFP2PMF_CommCommandTblNumGet( void )
{
    return sizeof(_CommNutWFP2PMFPacketTbl)/sizeof(CommPacketTbl);
}

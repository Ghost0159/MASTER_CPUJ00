//============================================================================================
/**
 * @file	plist_item.h
 * @brief	ポケモンリスト画面アイテム使用処理
 * @author	Hiroyuki Nakamura
 * @date	05.12.12
 */
//============================================================================================
#ifndef PLIST_ITEM_H
#define PLIST_ITEM_H
#undef GLOBAL
#ifdef PLIST_ITEM_H_GLOBAL
#define GLOBAL	/*	*/
#else
#define GLOBAL	extern
#endif


//============================================================================================
//	プロトタイプ宣言
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * アイテム使用メイン
 *
 * @param	wk		ポケモンリストのワーク
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_ItemUseMain( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * アイテム使用メッセージウェイト
 *
 * @param	work		ポケモンリストのワーク
 *
 * @return	次のシーケンス
 */
//--------------------------------------------------------------------------------------------
GLOBAL int PokeList_ItemUseMsgWait( void * work );

//--------------------------------------------------------------------------------------------
/**
 * 全体瀕死回復アイテムチェック
 *
 * @param	item	アイテム番号
 *
 * @retval	"TRUE = 全体瀕死回復アイテム"
 * @retval	"FALSE = それ以外"
 */
//--------------------------------------------------------------------------------------------
GLOBAL BOOL PL_ALLDeathRcvItemCheck( u16 item );

//--------------------------------------------------------------------------------------------
/**
 * 全体瀕死回復メイン
 *
 * @param	work		ポケモンリストのワーク
 *
 * @return	次のシーケンス
 */
//--------------------------------------------------------------------------------------------
GLOBAL int PokeList_AllDeathRcvMain( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * レベルアップ技覚え
 *
 * @param	wk		ポケモンリストのワーク
 *
 * @return	次のシーケンス
 */
//--------------------------------------------------------------------------------------------
GLOBAL int PokeList_LvUpWazaSetRetInit( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * 技マシン使用チェック
 *
 * @param	wk		ポケモンリストのワーク
 * @param	pp		POKEMON_PARAM
 *
 * @retval	"0-3 = 技位置"
 * @retval	"0xfd = すでに覚えている"
 * @retval	"0xfe = 技がいっぱい"
 * @retval	"0xff = 相性が悪い"
 */
//--------------------------------------------------------------------------------------------
GLOBAL u8 PokeList_WazaOboeCheck( PLIST_WORK * wk, POKEMON_PARAM * pp );

//--------------------------------------------------------------------------------------------
/**
 * 技マシン
 *
 * @param	wk		ポケモンリストのワーク
 *
 * @return	次のシーケンス
 */
//--------------------------------------------------------------------------------------------
GLOBAL int PokeList_WazaOboeInit( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * 技忘れ->覚え
 *
 * @param	wk		ポケモンリストのワーク
 *
 * @return	次のシーケンス
 */
//--------------------------------------------------------------------------------------------
GLOBAL int PokeList_WazaSetRetInit( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * 技覚えメッセージ終了待ち
 *
 * @param	wk		ポケモンリストのワーク
 *
 * @return	次のシーケンス
 */
//--------------------------------------------------------------------------------------------
GLOBAL int PokeList_WazaSetRetSet( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * 技選択メニュー作成
 *
 * @param	wk		ポケモンリストのワーク
 * @param	type	0 = PP Up, 1 = PP Rcv
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_WazaMenuInit( PLIST_WORK * wk, u8 type );

//--------------------------------------------------------------------------------------------
/**
 * 技選択メニューメイン
 *
 * @param	wk		ポケモンリストのワーク
 *
 * @return	次のシーケンス
 */
//--------------------------------------------------------------------------------------------
GLOBAL int PokeList_WazaMenuMain( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * メールボックスからメールを持たせる
 *
 * @param	wk		ポケモンリストのワーク
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_MailBoxAdd( PLIST_WORK * wk );



#undef GLOBAL
#endif

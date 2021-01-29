//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		lobby_news_data.h
 *	@brief		���r�[�j���[�X	�f�[�^�i�[����
 *	@author		tomoya takahashi
 *	@data		2007.10.09
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __LOBBY_NEWS_DATA_H__
#define __LOBBY_NEWS_DATA_H__

#include "gflib/strbuf.h"

#include "savedata/mystatus.h"

#include "wflby_def.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	ROOM�\���@������
//=====================================
typedef enum {
	NEWS_ROOMSP_NONE	= 0,
	NEWS_ROOMSP_MYDATA	= 1 << 0,
	NEWS_ROOMSP_OLD		= 1 << 1,
} NEWS_ROOMSP;

//-------------------------------------
///	ROOM�C�x���g
//=====================================
typedef enum {
	NEWS_ROOMEV_NONE,
	NEWS_ROOMEV_IN,		// �N�������Ă���
	NEWS_ROOMEV_OUT,	// �N���o�čs����
} NEWS_ROOMEV;



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	WiFi���r�[�j���[�X�f�[�^�x�[�X
//=====================================
typedef struct _NEWS_DATA NEWS_DATA;





//-------------------------------------
///	���r�[�g�s�b�N�ݒ�\���̌S
//=====================================
// ��b�f�[�^
typedef struct {
	const MYSTATUS* cp_p1;
	const MYSTATUS* cp_p2;
	u16				idx_p1;
	u16				idx_p2;
} NEWS_DATA_SET_CONNECT;

// �K�W�F�b�g�����f�[�^
typedef struct {
	const MYSTATUS* cp_p1;
	const MYSTATUS* cp_p2;
	u16				idx_p1;
	u16				idx_p2;
	WFLBY_ITEMTYPE	item;
} NEWS_DATA_SET_ITEM;

// �~�j�Q�[���f�[�^
typedef struct {
	WFLBY_GAMETYPE minigame;
	u32 num;
	const MYSTATUS* cp_p1;
	const MYSTATUS* cp_p2;
	const MYSTATUS* cp_p3;
	const MYSTATUS* cp_p4;
	u16				idx_p1;
	u16				idx_p2;
	u16				idx_p3;
	u16				idx_p4;
	BOOL play;
} NEWS_DATA_SET_MINIGAME;

// ���Ճ{�[�h
typedef struct {
	WFLBY_GAMETYPE board;
	u32 num;
	const MYSTATUS* cp_p1;
	u16				idx_p1;
} NEWS_DATA_SET_FOOTBOARD;

// ���E���v
typedef struct {
	u32 num;
	const MYSTATUS* cp_p1;
	const MYSTATUS* cp_p2;
	const MYSTATUS* cp_p3;
	const MYSTATUS* cp_p4;
	u16				idx_p1;
	u16				idx_p2;
	u16				idx_p3;
	u16				idx_p4;
} NEWS_DATA_SET_WORLDTIMER;

// ���r�[�j���[�X
typedef struct {
	u32 num;
	const MYSTATUS* cp_p1;
	const MYSTATUS* cp_p2;
	const MYSTATUS* cp_p3;
	const MYSTATUS* cp_p4;
	u16				idx_p1;
	u16				idx_p2;
	u16				idx_p3;
	u16				idx_p4;
} NEWS_DATA_SET_LOBBYNEWS;

// �C�x���g
typedef struct {
	const WFLBY_TIME*	cp_time;
	WFLBY_EVENTTYPE		event_no;
} NEWS_DATA_SET_TIMEEVENT;

// VIP����
typedef struct {
	u32	vip_plidx;
} NEWS_DATA_SET_VIPIN;

// �~�j�Q�[������
typedef struct {
	WFLBY_GAMETYPE minigame;
	u32 num;
	const MYSTATUS* cp_p1;
	const MYSTATUS* cp_p2;
	const MYSTATUS* cp_p3;
	const MYSTATUS* cp_p4;
	u16				idx_p1;
	u16				idx_p2;
	u16				idx_p3;
	u16				idx_p4;
} NEWS_DATA_SET_MGRESULT;



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
extern NEWS_DATA* NEWS_DSET_Init( u32 heapID );
extern void NEWS_DSET_Exit( NEWS_DATA* p_data );
extern void NEWS_DSET_Main( NEWS_DATA* p_data );	// �g�s�b�N�̔j�������Ȃǂ��s��
	// NEWS_DSET_Main�Ɗe�f�[�^�ݒ�̃^�C�~���O�ɒ��ӂ��Ă��������B
	// Main�֐����ŁA�����C�x���g�t���O�̔j���Ȃǂ��s����
	// �`��ɔ��f����Ȃ��Ȃ�\��������܂��B
	//
	// �f�[�^�ݒ��P�V���NMain���Ă΂�Ȃ��悤�ɂ��Ă����K�v������܂��B
	//
	// Main
	// �b
	// �f�[�^�ݒ�
	// �b
	// �`�揈��
	//
	// ���̗��ꂪ���z�ł��B

// ���ԃf�[�^�ݒ�
extern void NEWS_DSET_SetLockTime( NEWS_DATA* p_data, const WFLBY_TIME* cp_time );

// ���ގ��ݒ�
extern void NEWS_DSET_SetRoomIn( NEWS_DATA* p_data, u32 player,  u32 sex, NEWS_ROOMSP special_msk );
extern BOOL NEWS_DSET_SetRoomOut( NEWS_DATA* p_data, u32 player );
extern void NEWS_DSET_SetRoomOld( NEWS_DATA* p_data, u32 player );

// �g�s�b�N�X�ݒ�
extern void NEWS_DSET_SetConnect( NEWS_DATA* p_data,  const NEWS_DATA_SET_CONNECT* cp_data );
extern void NEWS_DSET_SetItem( NEWS_DATA* p_data, const NEWS_DATA_SET_ITEM* cp_data );
extern void NEWS_DSET_SetMiniGame( NEWS_DATA* p_data, const NEWS_DATA_SET_MINIGAME* cp_data );
extern void NEWS_DSET_SetFootBoard( NEWS_DATA* p_data, const NEWS_DATA_SET_FOOTBOARD* cp_data );
extern void NEWS_DSET_SetWorldTimer( NEWS_DATA* p_data, const NEWS_DATA_SET_WORLDTIMER* cp_data );
extern void NEWS_DSET_SetLobbyNews( NEWS_DATA* p_data, const NEWS_DATA_SET_LOBBYNEWS* cp_data );
extern void NEWS_DSET_SetTimeEvent( NEWS_DATA* p_data, const NEWS_DATA_SET_TIMEEVENT* cp_data );
extern void NEWS_DSET_SetVipIn( NEWS_DATA* p_data, const NEWS_DATA_SET_VIPIN* cp_data, const WFLBY_VIPFLAG* cp_vip );
extern void NEWS_DSET_SetMgResult( NEWS_DATA* p_data, const NEWS_DATA_SET_MGRESULT* cp_data );

// �g�s�b�N�X�̔j��
extern void NEWS_DSET_RemoveTopTopic( NEWS_DATA* p_data );

// LOCK���Ԏ擾
extern BOOL NEWS_DSET_GetLockTime( const NEWS_DATA* cp_data, WFLBY_TIME* p_time );

// �v���C���[�f�[�^�擾
extern BOOL NEWS_DSET_CheckPlayerInside( const NEWS_DATA* cp_wk, u32 player );
extern u32 NEWS_DSET_GetPlayerSex( const NEWS_DATA* cp_wk, u32 player );
extern BOOL NEWS_DSET_GetPlayerSpecialMy( const NEWS_DATA* cp_wk, u32 player );
extern BOOL NEWS_DSET_GetPlayerSpecialOld( const NEWS_DATA* cp_wk, u32 player );
extern BOOL NEWS_DSET_GetPlayerNewPlayer( const NEWS_DATA* cp_wk, u32 player );
extern NEWS_ROOMEV NEWS_DSET_GetPlayerEvent( const NEWS_DATA* cp_wk );

// �g�s�b�N�f�[�^�擾
// �g�s�b�N�f�[�^�͕�����̌`�ɂȂ��Ď擾�����
extern BOOL NEWS_DSET_CheckToppic( const NEWS_DATA* cp_wk );
extern NEWS_TOPICTYPE NEWS_DSET_GetTopicType( const NEWS_DATA* cp_wk ); 
extern BOOL NEWS_DSET_GetTopicData( const NEWS_DATA* cp_wk, const WFLBY_VIPFLAG* cp_vip, NEWS_TOPICTYPE type, STRBUF* p_str, u32 heapID );
extern u32 NEWS_DSET_GetTopicUserIdx( const NEWS_DATA* cp_wk, NEWS_TOPICTYPE type, u32 count );

#endif		// __LOBBY_NEWS_DATA_H__


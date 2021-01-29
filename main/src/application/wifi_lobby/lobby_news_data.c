//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		lobby_news_data.c
 *	@brief		���r�[�j���[�X	�f�[�^�i�[����
 *	@author		tomoya takahashi
 *	@data		2007.10.09
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "gflib/strbuf.h"

#include "system/buflen.h"
#include "system/wordset.h"
#include "system/msgdata.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_wflby_news.h"

#include "lobby_news_data.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�g�s�b�N�֘A
//=====================================
#define NEWS_TOPIC_MAX		( 8 )							// �g�s�b�N�ő吔
#define NEWS_TOPICNAME_LEN	( PERSON_NAME_SIZE+EOM_SIZE )	// �g���[�i������T�C�Y

enum{		// �D�揇��
	NEWS_TOPIC_PRI_NONE,
	NEWS_TOPIC_PRI_VIPIN,
	NEWS_TOPIC_PRI_MINIGAME_02,
	NEWS_TOPIC_PRI_MINIGAME_01,
	NEWS_TOPIC_PRI_MINIGAME_00,
	NEWS_TOPIC_PRI_FOOTBOARD_00,
	NEWS_TOPIC_PRI_FOOTBOARD_01,
	NEWS_TOPIC_PRI_ITEM,
	NEWS_TOPIC_PRI_CONNECT,
	NEWS_TOPIC_PRI_MGRESULT02,	// ����	�ǂ�ǂ�\�[�i���X
	NEWS_TOPIC_PRI_MGRESULT01,	// ����	�}�l�l
	NEWS_TOPIC_PRI_MGRESULT00,	// ���� �}���m�[��
	NEWS_TOPIC_PRI_WORLDTIMER,
	NEWS_TOPIC_PRI_LOBBYNEWS,
	NEWS_TOPIC_PRI_TIMEEVENT,
};
enum{		// �e�g�s�b�N�̃��[�N�g�p���@

	NEWS_TOPIC_WK_NUM		= 3,	// ���[�N�̑���

	// ���A

	// �A�C�e���V�F�A
	NEWS_TOPIC_WK_ITEMNO		= 0,

	// �~�j�Q�[��
	NEWS_TOPIC_WK_GAME_NO		= 0,
	NEWS_TOPIC_WK_GAME_PLAYNUM,
	NEWS_TOPIC_WK_GAME_PLAYFLAG,

	// ��������
	NEWS_TOPIC_WK_FOOT_NO		= 0,
	NEWS_TOPIC_WK_FOOT_PLAYNUM,

	// ���E���v
	NEWS_TOPIC_WK_WLDT_PLAYNUM	= 0,

	// ���r�[�j���[�X
	NEWS_TOPIC_WK_LBYN_PLAYNUM	= 0,

	// ���ԃC�x���g
	NEWS_TOPIC_WK_TIEV_TIME		= 0,
	NEWS_TOPIC_WK_TIEV_NO,

	// VIP����
	NEWS_TOPIC_WK_VIPIN_IDX		= 0,

	// �~�j�Q�[������
	NEWS_TOPIC_WK_MGRESULT_NO		= 0,
	NEWS_TOPIC_WK_MGRESULT_PLAYNUM,
};
enum{		// �e�g�s�b�N�̔j���J�E���g
	// �~�j�Q�[��
	NEWS_TOPIC_DEST_VIPIN		= 60 * 30,
	NEWS_TOPIC_DEST_GAME		= 30 * 30,
	NEWS_TOPIC_DEST_FOOT		= 30 * 30,
	NEWS_TOPIC_DEST_ITEM		= 20 * 30,
	NEWS_TOPIC_DEST_CONNECT		= 20 * 30,
	NEWS_TOPIC_DEST_MGRESULT	= 15 * 30,
	NEWS_TOPIC_DEST_WLDTIME		= 15 * 30,
	NEWS_TOPIC_DEST_LBYNEWS		= 15 * 30,
	NEWS_TOPIC_DEST_TIEV		= 30 * 30,
};


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�ڑ��v���C���[�f�[�^
//=====================================
//1�v���C���[�f�[�^
typedef struct {
	u16 sex;
	u16 special;
} NEWS_ROOMPLAYER_ONE;
// �v���C���[�S�f�[�^
typedef struct {
	NEWS_ROOMPLAYER_ONE	player[WFLBY_PLAYER_MAX];
	u16					event;				// �v���C���[�C�x���g
	u16					new_player;			// �V���������Ă����v���C���[
} NEWS_ROOMPLAYER;

//-------------------------------------
///	�g�s�b�N�X�f�[�^
//=====================================
// 1�g�s�b�N�f�[�^
typedef struct _NEWS_TOPIC{
	u8		type;							// �f�[�^�^�C�v
	u32		work[NEWS_TOPIC_WK_NUM];		// �ėp���[�N
	STRBUF* p_name[ NEWS_TOPICNAME_NUM ];	// �l�̖��O�p�o�b�t�@
	u16		pl_idx[ NEWS_TOPICNAME_NUM ];	// �l�̎��ʃC���f�b�N�X
	u16		pri;							// �D�揇��
	s16		dest;							// �j���J�E���^

	// ���X�g�\��
	struct _NEWS_TOPIC* p_next;
	struct _NEWS_TOPIC* p_last;
} NEWS_TOPIC;
// �g�s�b�N�S�f�[�^
typedef struct {
	NEWS_TOPIC		topic_buf[ NEWS_TOPIC_MAX ];// �g�s�b�N�i�[�o�b�t�@
	NEWS_TOPIC		topic_top;					// �g�s�b�N�擪�f�[�^
} NEWS_TOPICBUF;



//-------------------------------------
///	WiFi���r�[�j���[�X�f�[�^�x�[�X
//=====================================
typedef struct _NEWS_DATA {
	WFLBY_TIME		lock_time;					// LOCK�^�C��
	BOOL			lock_flag;					// LOCK����Ă��邩
	NEWS_ROOMPLAYER	player;						// �ڑ��v���C���[�o�b�t�@
	NEWS_TOPICBUF	topic;						// �g�s�b�N�o�b�t�@

} NEWS_DATA;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�ڑ��v���C���[	�֘A
//=====================================
static void NEWS_ROOMPLAYER_Init( NEWS_ROOMPLAYER* p_wk );
static void NEWS_ROOMPLAYER_SetData( NEWS_ROOMPLAYER* p_wk, u32 player, u32 sex, NEWS_ROOMSP special );
static void NEWS_ROOMPLAYER_SetIn( NEWS_ROOMPLAYER* p_wk, u32 player, u32 sex, NEWS_ROOMSP special );
static void NEWS_ROOMPLAYER_SetOut( NEWS_ROOMPLAYER* p_wk, u32 player );
static void NEWS_ROOMPLAYER_SetSpecial( NEWS_ROOMPLAYER* p_wk, u32 player, NEWS_ROOMSP special );
static void NEWS_ROOMPLAYER_SetEvent( NEWS_ROOMPLAYER* p_wk, NEWS_ROOMEV event );
static void NEWS_ROOMPLAYER_SetNewPlayer( NEWS_ROOMPLAYER* p_wk, u32 player );
static BOOL NEWS_ROOMPLAYER_CheckInside( const NEWS_ROOMPLAYER* cp_wk, u32 player );
static u32 NEWS_ROOMPLAYER_GetSex( const NEWS_ROOMPLAYER* cp_wk, u32 player );
static BOOL NEWS_ROOMPLAYER_GetSpecialMy( const NEWS_ROOMPLAYER* cp_wk, u32 player );
static BOOL NEWS_ROOMPLAYER_GetSpecialOld( const NEWS_ROOMPLAYER* cp_wk, u32 player );
static NEWS_ROOMSP NEWS_ROOMPLAYER_GetSpecial( const NEWS_ROOMPLAYER* cp_wk, u32 player );
static NEWS_ROOMEV NEWS_ROOMPLAYER_GetEvent( const NEWS_ROOMPLAYER* cp_wk );
static BOOL NEWS_ROOMPLAYER_GetNewPlayer( const NEWS_ROOMPLAYER* cp_wk, u32 player );


//-------------------------------------
///	�g�s�b�N�X	�֘A
//=====================================
static void NEWS_TOPIC_Init( NEWS_TOPICBUF* p_wk, u32 heapID );
static void NEWS_TOPIC_Exit( NEWS_TOPICBUF* p_wk );
static void NEWS_TOPIC_Main( NEWS_TOPICBUF* p_wk );
static void NEWS_TOPIC_RemoveTop( NEWS_TOPICBUF* p_wk );
static NEWS_TOPIC* NEWS_TOPIC_GetCleanWork( NEWS_TOPICBUF* p_wk, u16 pri );
static void NEWS_TOPIC_AddList( NEWS_TOPIC* p_last, NEWS_TOPIC* p_obj );
static void NEWS_TOPIC_Add( NEWS_TOPICBUF* p_wk, NEWS_TOPIC* p_obj );
static void NEWS_TOPIC_Remove( NEWS_TOPICBUF* p_wk, NEWS_TOPIC* p_obj );
static void NEWS_TOPIC_SetData( NEWS_TOPIC* p_obj, u32 data0, u32 data1, u32 data2, const MYSTATUS* cp_p1, const MYSTATUS* cp_p2, const MYSTATUS* cp_p3, const MYSTATUS* cp_p4, u16 idx_p1, u16 idx_p2, u16 idx_p3, u16 idx_p4, u32 dest, u32 pri, u32 type );

static BOOL NEWS_TOPIC_GetSTRConnect( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID );
static BOOL NEWS_TOPIC_GetSTRItem( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID );
static BOOL NEWS_TOPIC_GetSTRMiniGame( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID );
static BOOL NEWS_TOPIC_GetSTRFoot( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID );
static BOOL NEWS_TOPIC_GetSTRWldTimer( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID );
static BOOL NEWS_TOPIC_GetSTREvent( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID );
static BOOL NEWS_TOPIC_GetSTRVipIn( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID );
static BOOL NEWS_TOPIC_GetSTRMgResultIn( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID );

static u32 NEWS_TOPIC_GetUserIdxConnect( const NEWS_TOPIC* cp_obj, u32 count );
static u32 NEWS_TOPIC_GetUserIdxItem( const NEWS_TOPIC* cp_obj, u32 count );
static u32 NEWS_TOPIC_GetUserIdxMiniGame( const NEWS_TOPIC* cp_obj, u32 count );
static u32 NEWS_TOPIC_GetUserIdxFoot( const NEWS_TOPIC* cp_obj, u32 count );
static u32 NEWS_TOPIC_GetUserIdxWldTimer( const NEWS_TOPIC* cp_obj, u32 count );
static u32 NEWS_TOPIC_GetUserIdxEvent( const NEWS_TOPIC* cp_obj, u32 count );
static u32 NEWS_TOPIC_GetUserIdxVipIn( const NEWS_TOPIC* cp_obj, u32 count );
static u32 NEWS_TOPIC_GetUserIdxMgResult( const NEWS_TOPIC* cp_obj, u32 count );

static BOOL NEWS_TOPIC_MGRESULT_CheckPlayerNum( WFLBY_GAMETYPE minigame, u32 num );


//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�j���[�X	�o�b�t�@�쐬
 *
 *	@param	heapID	�q�[�v
 *
 *	@return	���r�[�j���[�X�o�b�t�@
 */
//-----------------------------------------------------------------------------
NEWS_DATA* NEWS_DSET_Init( u32 heapID )
{
	NEWS_DATA* p_data;

	p_data = sys_AllocMemory( heapID, sizeof( NEWS_DATA ) );
	memset( p_data, 0, sizeof(NEWS_DATA) );

	// �ڑ��v���C���[�@������
	NEWS_ROOMPLAYER_Init( &p_data->player );

	// �g�s�b�N�o�b�t�@������
	NEWS_TOPIC_Init( &p_data->topic, heapID );


	return p_data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�j���[�X	�o�b�t�@�j��
 *
 *	@param	p_data	���[�N
 */
//-----------------------------------------------------------------------------
void NEWS_DSET_Exit( NEWS_DATA* p_data )
{
	
	// �g�s�b�N�o�b�t�@�j��
	NEWS_TOPIC_Exit( &p_data->topic );
	
	sys_FreeMemoryEz( p_data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�j���[�X	���C������	�i�g�s�b�N�X�����Ԍo�߂ɂ��j�����鏈���Ȃǁj
 *
 *	@param	p_data	���[�N
 */
//-----------------------------------------------------------------------------
void NEWS_DSET_Main( NEWS_DATA* p_data )
{
	// �g�s�b�N�X
	NEWS_TOPIC_Main( &p_data->topic );

	// �ݒ肵���C�x���g��j��
	// �V���������Ă����l�f�[�^(player.new_player)���j���������Ƃ��낾���A
	// lobby_news�ŕ\������Ƃ��ɂ����ƂƂ��Ă������ق���
	// �s�����悢�̂Ŕj�����Ȃ�
	NEWS_ROOMPLAYER_SetEvent( &p_data->player, NEWS_ROOMEV_NONE );
	
}


//-------------------------------------
///	�f�[�^�i�[����
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	LOCK���Ԃ̐ݒ�
 *
 *	@param	p_data	���[�N
 *	@param	cp_time	���ԃf�[�^
 */
//-----------------------------------------------------------------------------
void NEWS_DSET_SetLockTime( NEWS_DATA* p_data, const WFLBY_TIME* cp_time )
{
	p_data->lock_time = *cp_time;
	p_data->lock_flag = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ɓ������f�[�^�ݒ�
 *
 *	@param	p_data		���[�N
 *	@param	player		�������v���C���[NO
 *	@param	sex			�������v���C���[�̐���
 *	@param	special_msk	�������v���C���[�̓����ԃ}�X�N
 */
//-----------------------------------------------------------------------------
void NEWS_DSET_SetRoomIn( NEWS_DATA* p_data, u32 player,  u32 sex, NEWS_ROOMSP special_msk )
{
	NEWS_ROOMPLAYER_SetIn( &p_data->player, player, sex, special_msk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������o���f�[�^�ݒ�
 *
 *	@param	p_data		���[�N
 *	@param	player		�o���v���C���[NO
 *
 *	@retval	TRUE	��ԌÂ��l�������ꍇ
 *	@retval	FALSE	��ԌÂ��Ȃ��l�������ꍇ
 */
//-----------------------------------------------------------------------------
BOOL NEWS_DSET_SetRoomOut( NEWS_DATA* p_data, u32 player )
{
	BOOL result;
	
	result = NEWS_ROOMPLAYER_GetSpecialOld( &p_data->player, player );
	
	NEWS_ROOMPLAYER_SetOut( &p_data->player, player );

	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ԌÂ��l�ݒ���s��
 *
 *	@param	p_data		���[�N
 *	@param	player		�v���C���[
 */	
//-----------------------------------------------------------------------------
void NEWS_DSET_SetRoomOld( NEWS_DATA* p_data, u32 player )
{
	u32 special;
	int i;

	// ���܂ň�ԌÂ��f�[�^�������l��������t���O�𗎂Ƃ�
	for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
		special = NEWS_ROOMPLAYER_GetSpecial( &p_data->player, i );
		if( special & NEWS_ROOMSP_OLD ){
			special &= ~NEWS_ROOMSP_OLD;
			NEWS_ROOMPLAYER_SetSpecial( &p_data->player, i, special );
		}
	}

	special = NEWS_ROOMPLAYER_GetSpecial( &p_data->player, player );
	NEWS_ROOMPLAYER_SetSpecial( &p_data->player, player, special | NEWS_ROOMSP_OLD );
}

//-----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�̐ݒ�
 *
 *	@param	p_data		���[�N
 *	�e�ݒ�f�[�^
 */
//-----------------------------------------------------------------------------
void NEWS_DSET_SetConnect( NEWS_DATA* p_data,  const NEWS_DATA_SET_CONNECT* cp_data )
{
	NEWS_TOPIC* p_obj;
	p_obj = NEWS_TOPIC_GetCleanWork( &p_data->topic, NEWS_TOPIC_PRI_CONNECT );
	if( p_obj == NULL ){
		return ;
	}
	NEWS_TOPIC_SetData( p_obj, 
			0, 0, 0,
			cp_data->cp_p1, cp_data->cp_p2,
			NULL, NULL,
			cp_data->idx_p1, cp_data->idx_p2,
			0, 0,
			NEWS_TOPIC_DEST_CONNECT, NEWS_TOPIC_PRI_CONNECT, 
			NEWS_TOPICTYPE_CONNECT );
	NEWS_TOPIC_Add( &p_data->topic, p_obj );
}

void NEWS_DSET_SetItem( NEWS_DATA* p_data, const NEWS_DATA_SET_ITEM* cp_data )
{
	NEWS_TOPIC* p_obj;

	// �A�C�e���^�C�v���s���Ȃ�ݒ肵�Ȃ�
	if( cp_data->item >= WFLBY_ITEM_NUM  ){
		return ;
	}
	
	p_obj = NEWS_TOPIC_GetCleanWork( &p_data->topic, NEWS_TOPIC_PRI_ITEM );
	if( p_obj == NULL ){
		return ;
	}
	NEWS_TOPIC_SetData( p_obj, 
			cp_data->item, 0, 0,
			cp_data->cp_p1, cp_data->cp_p2,
			NULL, NULL,
			cp_data->idx_p1, cp_data->idx_p2,
			0, 0,
			NEWS_TOPIC_DEST_ITEM, NEWS_TOPIC_PRI_ITEM, 
			NEWS_TOPICTYPE_ITEM );
	NEWS_TOPIC_Add( &p_data->topic, p_obj );
}

void NEWS_DSET_SetMiniGame( NEWS_DATA* p_data, const NEWS_DATA_SET_MINIGAME* cp_data )
{
	NEWS_TOPIC* p_obj;
	u32 minigame;
	static const sc_NEWS_DSET_MINIGAME_PRI[ WFLBY_GAME_P2PGAME_NUM ] = {
		NEWS_TOPIC_PRI_MINIGAME_00,
		NEWS_TOPIC_PRI_MINIGAME_01,
		NEWS_TOPIC_PRI_MINIGAME_02,
	};

	// �~�j�Q�[���^�C�v���s���Ȃ�ݒ肵�Ȃ�
	if( (cp_data->minigame != WFLBY_GAME_BALLSLOW) && 
		(cp_data->minigame != WFLBY_GAME_BALANCEBALL) && 
		(cp_data->minigame != WFLBY_GAME_BALLOON) ){
		return ;
	}

	// �ڑ��l����Play=TRUE�Ȃ�Q�ȏ�ł���K�v������
	// �S�ȉ��ł���K�v������
	if( cp_data->play == TRUE ){
		if( (cp_data->num < 2) || (cp_data->num > 4) ){
			return ;
		}
	}else{
		// �ڑ��O�Ȃ�1�ȏ�ł���K�v������
		if( (cp_data->num < 1) || (cp_data->num > 4) ){
			return ;
		}
	}

	// �~�j�Q�[�����ł̐����ɕϊ�
	minigame = cp_data->minigame - WFLBY_GAME_BALLSLOW;
	
	p_obj = NEWS_TOPIC_GetCleanWork( &p_data->topic, sc_NEWS_DSET_MINIGAME_PRI[minigame] );
	if( p_obj == NULL ){
		return ;
	}
	NEWS_TOPIC_SetData( p_obj, 
			minigame, cp_data->num, cp_data->play,
			cp_data->cp_p1, cp_data->cp_p2,
			cp_data->cp_p3, cp_data->cp_p4,
			cp_data->idx_p1, cp_data->idx_p2,
			cp_data->idx_p3, cp_data->idx_p4,
			NEWS_TOPIC_DEST_GAME, sc_NEWS_DSET_MINIGAME_PRI[minigame], 
			NEWS_TOPICTYPE_MINIGAME );
	NEWS_TOPIC_Add( &p_data->topic, p_obj );
}

void NEWS_DSET_SetFootBoard( NEWS_DATA* p_data, const NEWS_DATA_SET_FOOTBOARD* cp_data )
{
	NEWS_TOPIC* p_obj;

	// ���Ճ{�[�h�^�C�v���s���Ȃ�ݒ肵�Ȃ�
	if( (cp_data->board != WFLBY_GAME_FOOTWHITE) && 
		(cp_data->board != WFLBY_GAME_FOOTBLACK) ){
		return ;
	}

	p_obj = NEWS_TOPIC_GetCleanWork( &p_data->topic, NEWS_TOPIC_PRI_FOOTBOARD_00+cp_data->board );
	if( p_obj == NULL ){
		return ;
	}
	NEWS_TOPIC_SetData( p_obj, 
			cp_data->board, cp_data->num, 0,
			cp_data->cp_p1, NULL,
			NULL, NULL,
			cp_data->idx_p1, 0,
			0, 0,
			NEWS_TOPIC_DEST_FOOT, NEWS_TOPIC_PRI_FOOTBOARD_00+cp_data->board, 
			NEWS_TOPICTYPE_FOOTBOARD );
	NEWS_TOPIC_Add( &p_data->topic, p_obj );
}

void NEWS_DSET_SetWorldTimer( NEWS_DATA* p_data, const NEWS_DATA_SET_WORLDTIMER* cp_data )
{
	NEWS_TOPIC* p_obj;

	p_obj = NEWS_TOPIC_GetCleanWork( &p_data->topic, NEWS_TOPIC_PRI_WORLDTIMER );
	if( p_obj == NULL ){
		return ;
	}
	NEWS_TOPIC_SetData( p_obj, 
			cp_data->num, 0, 0,
			cp_data->cp_p1, cp_data->cp_p2,
			cp_data->cp_p3, cp_data->cp_p4,
			cp_data->idx_p1, cp_data->idx_p2,
			cp_data->idx_p3, cp_data->idx_p4,
			NEWS_TOPIC_DEST_WLDTIME, NEWS_TOPIC_PRI_WORLDTIMER, 
			NEWS_TOPICTYPE_WORLDTIMER );
	NEWS_TOPIC_Add( &p_data->topic, p_obj );
}

void NEWS_DSET_SetLobbyNews( NEWS_DATA* p_data, const NEWS_DATA_SET_LOBBYNEWS* cp_data )
{
	NEWS_TOPIC* p_obj;

	p_obj = NEWS_TOPIC_GetCleanWork( &p_data->topic, NEWS_TOPIC_PRI_LOBBYNEWS );
	if( p_obj == NULL ){
		return ;
	}
	NEWS_TOPIC_SetData( p_obj, 
			cp_data->num, 0, 0,
			cp_data->cp_p1, cp_data->cp_p2,
			cp_data->cp_p3, cp_data->cp_p4,
			cp_data->idx_p1, cp_data->idx_p2,
			cp_data->idx_p3, cp_data->idx_p4,
			NEWS_TOPIC_DEST_LBYNEWS, NEWS_TOPIC_PRI_LOBBYNEWS, 
			NEWS_TOPICTYPE_LOBBYNEWS );
	NEWS_TOPIC_Add( &p_data->topic, p_obj );
}

void NEWS_DSET_SetTimeEvent( NEWS_DATA* p_data, const NEWS_DATA_SET_TIMEEVENT* cp_data )
{
	NEWS_TOPIC* p_obj;

	// �C�x���g���s���Ȃ�ݒ肵�Ȃ�
	switch( cp_data->event_no ){
	
	// �\������C�x���g
	case WFLBY_EVENT_NEON:		// �l�I��
	case WFLBY_EVENT_FIRE:		// �ԉ�
	case WFLBY_EVENT_PARADE:		// �p���[�h
	case WFLBY_EVENT_CLOSE:		// �N���[�Y
		break;
	
	// �\�����Ȃ��C�x���g
	default:
		return;
	}
	
	p_obj = NEWS_TOPIC_GetCleanWork( &p_data->topic, NEWS_TOPIC_PRI_TIMEEVENT );
	if( p_obj == NULL ){
		return ;
	}
	NEWS_TOPIC_SetData( p_obj, 
			cp_data->cp_time->time, cp_data->event_no, 0,
			NULL, NULL,	NULL, NULL,
			0, 0, 0, 0,
			NEWS_TOPIC_DEST_TIEV, NEWS_TOPIC_PRI_TIMEEVENT, 
			NEWS_TOPICTYPE_TIMEEVENT );
	NEWS_TOPIC_Add( &p_data->topic, p_obj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP�̓���
 */
//-----------------------------------------------------------------------------
void NEWS_DSET_SetVipIn( NEWS_DATA* p_data, const NEWS_DATA_SET_VIPIN* cp_data, const WFLBY_VIPFLAG* cp_vip )
{
	NEWS_TOPIC* p_obj;
	int i;

	// ���̃��[�UIDX�͐��������`�F�b�N
	if( cp_data->vip_plidx >= WFLBY_PLAYER_MAX ){
		return ;
	}

	// ���̐l��VIP�Ȃ̂��`�F�b�N����
	if( cp_vip->vip[ cp_data->vip_plidx ] == FALSE ){
		return ;	// VIP����Ȃ��̂Œǉ����Ȃ�
	}

	p_obj = NEWS_TOPIC_GetCleanWork( &p_data->topic, NEWS_TOPIC_PRI_VIPIN );
	if( p_obj == NULL ){
		return ;
	}
	NEWS_TOPIC_SetData( p_obj, 
			cp_data->vip_plidx, 0, 0,
			NULL, NULL,	NULL, NULL,
			0, 0, 0, 0,
			NEWS_TOPIC_DEST_VIPIN, NEWS_TOPIC_PRI_VIPIN, 
			NEWS_TOPICTYPE_VIPIN );
	NEWS_TOPIC_Add( &p_data->topic, p_obj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[������
 */
//-----------------------------------------------------------------------------
void NEWS_DSET_SetMgResult( NEWS_DATA* p_data, const NEWS_DATA_SET_MGRESULT* cp_data )
{
	NEWS_TOPIC* p_obj;
	u32 minigame;
	static const u8 sc_NEWS_DSET_MGRESULT_PRI[ 3 ] = {
		NEWS_TOPIC_PRI_MGRESULT00,
		NEWS_TOPIC_PRI_MGRESULT01,
		NEWS_TOPIC_PRI_MGRESULT02,
	};


	// �~�j�Q�[���^�C�v���s���Ȃ�ݒ肵�Ȃ�
	if( (cp_data->minigame != WFLBY_GAME_BALLSLOW) && 
		(cp_data->minigame != WFLBY_GAME_BALANCEBALL) && 
		(cp_data->minigame != WFLBY_GAME_BALLOON) ){
		return ;
	}

	// �D���l���`�F�b�N
	if( NEWS_TOPIC_MGRESULT_CheckPlayerNum( cp_data->minigame, cp_data->num ) == FALSE ){
		return ;
	}

	// �~�j�Q�[�����ł̐����ɕϊ�
	minigame = cp_data->minigame - WFLBY_GAME_BALLSLOW;
	
	p_obj = NEWS_TOPIC_GetCleanWork( &p_data->topic, sc_NEWS_DSET_MGRESULT_PRI[minigame] );
	if( p_obj == NULL ){
		return ;
	}
	NEWS_TOPIC_SetData( p_obj, 
			minigame, cp_data->num, 0,
			cp_data->cp_p1, cp_data->cp_p2,
			cp_data->cp_p3, cp_data->cp_p4,
			cp_data->idx_p1, cp_data->idx_p2,
			cp_data->idx_p3, cp_data->idx_p4,
			NEWS_TOPIC_DEST_MGRESULT, sc_NEWS_DSET_MGRESULT_PRI[minigame], 
			NEWS_TOPICTYPE_MGRESULT );
	NEWS_TOPIC_Add( &p_data->topic, p_obj );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�擪�g�s�b�N��j������
 *
 *	@param	p_data	���[�N
 */
//-----------------------------------------------------------------------------
void NEWS_DSET_RemoveTopTopic( NEWS_DATA* p_data )
{
	NEWS_TOPIC_RemoveTop( &p_data->topic );
}


//-------------------------------------
///	�f�[�^�擾����
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	LOCK���Ԃ��擾����
 *
 *	@param	cp_data		���[�N
 *	@param	p_time		���Ԋi�[��
 *
 *	@retval	TRUE	LOCK����Ă���
 *	@retval	FALSE	LOCK����Ă��Ȃ����玞�Ԃ�����
 */
//-----------------------------------------------------------------------------
BOOL NEWS_DSET_GetLockTime( const NEWS_DATA* cp_data, WFLBY_TIME* p_time )
{
	*p_time = cp_data->lock_time;
	return cp_data->lock_flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�������̒��ɓ����Ă��邩�`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *	@param	player	�v���C���[NO
 *
 *	@retval	TRUE	�����Ă���
 *	@retval	FALSE	�����Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL NEWS_DSET_CheckPlayerInside( const NEWS_DATA* cp_wk, u32 player )
{
	return NEWS_ROOMPLAYER_CheckInside( &cp_wk->player, player );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�̐��ʂ��擾
 *
 *	@param	cp_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@return	����
 */
//-----------------------------------------------------------------------------
u32 NEWS_DSET_GetPlayerSex( const NEWS_DATA* cp_wk, u32 player )
{
	return NEWS_ROOMPLAYER_GetSex( &cp_wk->player, player );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[���������`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	������
 */
//-----------------------------------------------------------------------------
BOOL NEWS_DSET_GetPlayerSpecialMy( const NEWS_DATA* cp_wk, u32 player )
{
	return NEWS_ROOMPLAYER_GetSpecialMy( &cp_wk->player, player );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[����Ԑ̂��炢�邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@retval	TRUE	�ӂ邢
 *	@retval	FALSE	�ӂ邭�Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL NEWS_DSET_GetPlayerSpecialOld( const NEWS_DATA* cp_wk, u32 player )
{
	return NEWS_ROOMPLAYER_GetSpecialOld( &cp_wk->player, player );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̐l���V���������Ă����l���`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@retval	TRUE	�V���������Ă����l
 *	@retval	FALSE	�Ⴄ�l
 */
//-----------------------------------------------------------------------------
BOOL NEWS_DSET_GetPlayerNewPlayer( const NEWS_DATA* cp_wk, u32 player )
{
	return NEWS_ROOMPLAYER_GetNewPlayer( &cp_wk->player, player );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�C�x���g�擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	NEWS_ROOMEV_NONE,
 *	@retval	NEWS_ROOMEV_IN,		// �N�������Ă���
 *	@retval	NEWS_ROOMEV_OUT,	// �N���o�čs����
 */
//-----------------------------------------------------------------------------
NEWS_ROOMEV NEWS_DSET_GetPlayerEvent( const NEWS_DATA* cp_wk )
{
	return NEWS_ROOMPLAYER_GetEvent( &cp_wk->player );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�����邩�`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�g�s�b�N������
 *	@retval	FALSE	�g�s�b�N���Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL NEWS_DSET_CheckToppic( const NEWS_DATA* cp_wk )
{
	if( cp_wk->topic.topic_top.p_next != &cp_wk->topic.topic_top ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�擪�̃g�s�b�N�X�^�C�v�̎擾
 *
 *	@param	cp_wk		���[�N 
 *
 *	@retval	NEWS_TOPICTYPE_CONNECT,			// ���A
 *	@retval	NEWS_TOPICTYPE_ITEM,			// �K�W�F�b�g�̌���
 *	@retval	NEWS_TOPICTYPE_MINIGAME,		// �~�j�Q�[��
 *	@retval	NEWS_TOPICTYPE_FOOTBOARD,		// �������ƃ{�[�h
 *	@retval	NEWS_TOPICTYPE_WORLDTIMER,		// ���E���v
 *	@retval	NEWS_TOPICTYPE_LOBBYNEWS,		// ���r�[�j���[�X
 *	@retval	NEWS_TOPICTYPE_TIMEEVENT,		// ���ԃC�x���g
 */
//-----------------------------------------------------------------------------
NEWS_TOPICTYPE NEWS_DSET_GetTopicType( const NEWS_DATA* cp_wk )
{
	// �f�[�^�����邩�`�F�b�N
	GF_ASSERT( NEWS_DSET_CheckToppic( cp_wk ) );
	
	return cp_wk->topic.topic_top.p_next->type;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�擪�f�[�^�̃g�s�b�N�𕶎���ɕϊ����ĕԂ�
 *
 *	@param	cp_wk		���[�N
 *	@param	cp_vip		VIP�t���O
 *	@param	type		�g�s�b�N�^�C�v
 *	@param	p_str		������i�[��
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	���������ꂽ
 *	@retval	FALSE	�����������Ȃ�����
 */
//-----------------------------------------------------------------------------
BOOL NEWS_DSET_GetTopicData( const NEWS_DATA* cp_wk, const WFLBY_VIPFLAG* cp_vip, NEWS_TOPICTYPE type, STRBUF* p_str, u32 heapID )
{
	WORDSET*			p_wordset;
	MSGDATA_MANAGER*	p_msgman;
	NEWS_TOPIC*			p_obj;
	BOOL				result;
	static BOOL (* const p_func[])( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID ) = {
		NEWS_TOPIC_GetSTRConnect,
		NEWS_TOPIC_GetSTRItem,
		NEWS_TOPIC_GetSTRMiniGame,
		NEWS_TOPIC_GetSTRFoot,
		NEWS_TOPIC_GetSTRWldTimer,
		NEWS_TOPIC_GetSTRWldTimer,
		NEWS_TOPIC_GetSTREvent,
		NEWS_TOPIC_GetSTRVipIn,
		NEWS_TOPIC_GetSTRMgResultIn,
	};

	GF_ASSERT( NEWS_DSET_CheckToppic( cp_wk ) );	// �g�s�b�N�����邩�`�F�b�N
	
	//  �擪�g�s�b�N�擾
	p_obj = cp_wk->topic.topic_top.p_next;

	// ���b�Z�[�W�f�[�^������
	p_msgman	= MSGMAN_Create(MSGMAN_TYPE_DIRECT,ARC_MSG,NARC_msg_wflby_news_dat,heapID );
	p_wordset	= WORDSET_Create( heapID );

	// ������쐬
	if( p_obj->type < NEWS_TOPICTYPE_NUM ){
		result = p_func[ p_obj->type ]( p_obj, cp_vip, p_wordset, p_msgman, p_str, heapID );
	}else{
		result = FALSE;
	}
	
	MSGMAN_Delete( p_msgman );
	WORDSET_Delete( p_wordset );

	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�擪�f�[�^�̃g�s�b�N�𕶎���ɕϊ����ĕԂ�
 *
 *	@param	cp_wk		���[�N
 *	@param	type		�g�s�b�N�^�C�v
 *	@param	count		�g�s�b�N�f�[�^���̃g���[�i�̃C���f�b�N�X
 *
 *	@retval	WFLBY_PLAYER_MAX	���̃C���f�b�N�X�ɂ���l�͂��Ȃ���
 *	@retval	���̂�				���̃C���f�b�N�X�̃v���C���[�̃��[�r�[���C���f�b�N�X
 */
//-----------------------------------------------------------------------------
u32 NEWS_DSET_GetTopicUserIdx( const NEWS_DATA* cp_wk, NEWS_TOPICTYPE type, u32 count )
{
	NEWS_TOPIC*			p_obj;
	u32 ret;
	static u32 (* const p_func[])( const NEWS_TOPIC* cp_obj, u32 count ) = {
		NEWS_TOPIC_GetUserIdxConnect,
		NEWS_TOPIC_GetUserIdxItem,
		NEWS_TOPIC_GetUserIdxMiniGame,
		NEWS_TOPIC_GetUserIdxFoot,
		NEWS_TOPIC_GetUserIdxWldTimer,
		NEWS_TOPIC_GetUserIdxWldTimer,
		NEWS_TOPIC_GetUserIdxEvent,
		NEWS_TOPIC_GetUserIdxVipIn,
		NEWS_TOPIC_GetUserIdxMgResult,
	};

	GF_ASSERT( NEWS_DSET_CheckToppic( cp_wk ) );	// �g�s�b�N�����邩�`�F�b�N
	
	//  �擪�g�s�b�N�擾
	p_obj = cp_wk->topic.topic_top.p_next;

	// ������쐬
	if( p_obj->type < NEWS_TOPICTYPE_NUM ){
		ret = p_func[ p_obj->type ]( p_obj, count );
	}else{
		GF_ASSERT( 0 );	// �g�s�b�N�^�C�v�G���[
		ret = WFLBY_PLAYER_MAX;
	}

	return ret;
}






//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�ڑ��v���C���[	�֘A
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	����������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void NEWS_ROOMPLAYER_Init( NEWS_ROOMPLAYER* p_wk )
{
	int i;
	
	for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
		NEWS_ROOMPLAYER_SetData( p_wk, i, PM_NEUTRAL, 0 );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�f�[�^�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	player		�ݒ�v���C���[NO
 *	@param	sex			����
 *	@param	special		������
 */
//-----------------------------------------------------------------------------
static void NEWS_ROOMPLAYER_SetData( NEWS_ROOMPLAYER* p_wk, u32 player, u32 sex, NEWS_ROOMSP special )
{
	GF_ASSERT( player < WFLBY_PLAYER_MAX );
	p_wk->player[ player ].sex		= sex;
	p_wk->player[ player ].special	= special;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	player		�v���C���[NO
 *	@param	sex			����
 *	@param	special		������
 */
//-----------------------------------------------------------------------------
static void NEWS_ROOMPLAYER_SetIn( NEWS_ROOMPLAYER* p_wk, u32 player, u32 sex, NEWS_ROOMSP special )
{
	NEWS_ROOMPLAYER_SetData( p_wk, player, sex, special );
	NEWS_ROOMPLAYER_SetEvent( p_wk, NEWS_ROOMEV_IN );
	NEWS_ROOMPLAYER_SetNewPlayer( p_wk, player );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ގ��ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	player	�v���C���[NO
 */
//-----------------------------------------------------------------------------
static void NEWS_ROOMPLAYER_SetOut( NEWS_ROOMPLAYER* p_wk, u32 player )
{
	NEWS_ROOMPLAYER_SetData( p_wk, player, PM_NEUTRAL, 0 );
	NEWS_ROOMPLAYER_SetEvent( p_wk, NEWS_ROOMEV_OUT );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�y�V�����f�[�^�����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	player		�v���C���[
 *	@param	special		�X�y�V�����f�[�^
 */
//-----------------------------------------------------------------------------
static void NEWS_ROOMPLAYER_SetSpecial( NEWS_ROOMPLAYER* p_wk, u32 player, NEWS_ROOMSP special )
{
	GF_ASSERT( player < WFLBY_PLAYER_MAX );
	p_wk->player[ player ].special	= special;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�f�[�^��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	event		�C�x���g
 */
//-----------------------------------------------------------------------------
static void NEWS_ROOMPLAYER_SetEvent( NEWS_ROOMPLAYER* p_wk, NEWS_ROOMEV event )
{
	GF_ASSERT( event <= NEWS_ROOMSP_OLD );
	p_wk->event = event;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V���������Ă����l��ۑ�����
 *
 *	@param	p_wk		���[�N
 *	@param	player		�v���C���[
 */
//-----------------------------------------------------------------------------
static void NEWS_ROOMPLAYER_SetNewPlayer( NEWS_ROOMPLAYER* p_wk, u32 player )
{
	GF_ASSERT( player < WFLBY_PLAYER_MAX );
	p_wk->new_player = player;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�������ɓ����Ă��邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@retval	TRUE	�����Ă���
 *	@retval	FALSE	�����Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_ROOMPLAYER_CheckInside( const NEWS_ROOMPLAYER* cp_wk, u32 player )
{
	u32 sex;

	sex = NEWS_ROOMPLAYER_GetSex( cp_wk, player );
	if( sex == PM_NEUTRAL ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�̐��ʂ��擾
 *
 *	@param	cp_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@return	����
 */
//-----------------------------------------------------------------------------
static u32 NEWS_ROOMPLAYER_GetSex( const NEWS_ROOMPLAYER* cp_wk, u32 player )
{
	GF_ASSERT( player < WFLBY_PLAYER_MAX );
	return cp_wk->player[ player ].sex;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[���������`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@retval	TRUE	�����̃f�[�^
 *	@retval	FALSE	���l�̃f�[�^
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_ROOMPLAYER_GetSpecialMy( const NEWS_ROOMPLAYER* cp_wk, u32 player )
{
	GF_ASSERT( player < WFLBY_PLAYER_MAX );
	if( cp_wk->player[ player ].special & NEWS_ROOMSP_MYDATA ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�������Ƃ��Â��l���`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@retval	TRUE		�����Ƃ��Â��l
 *	@retval	FALSE		�����Ƃ��Â��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_ROOMPLAYER_GetSpecialOld( const NEWS_ROOMPLAYER* cp_wk, u32 player )
{
	GF_ASSERT( player < WFLBY_PLAYER_MAX );
	if( cp_wk->player[ player ].special & NEWS_ROOMSP_OLD ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�̓����ԃ}�X�N���擾����
 *
 *	@param	p_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@return	�}�X�N
 */
//-----------------------------------------------------------------------------
static NEWS_ROOMSP NEWS_ROOMPLAYER_GetSpecial( const NEWS_ROOMPLAYER* cp_wk, u32 player )
{
	GF_ASSERT( player < WFLBY_PLAYER_MAX );
	return cp_wk->player[ player ].special;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�̎擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	NEWS_ROOMEV_IN,		// �N�������Ă���
 *	@retval NEWS_ROOMEV_OUT,	// �N���o�čs����
 */
//-----------------------------------------------------------------------------
static NEWS_ROOMEV NEWS_ROOMPLAYER_GetEvent( const NEWS_ROOMPLAYER* cp_wk )
{
	return cp_wk->event;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̐l���V���������Ă����l���`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	player		�v���C���[
 *
 *	@retval	TRUE	�V���������Ă����l
 *	@retval	FALSE	�Ⴄ�l
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_ROOMPLAYER_GetNewPlayer( const NEWS_ROOMPLAYER* cp_wk, u32 player )
{
	GF_ASSERT( player < WFLBY_PLAYER_MAX );
	
	if( cp_wk->new_player == player ){
		return TRUE;
	}
	return FALSE;
}




//-------------------------------------
///	�g�s�b�N�X	�֘A
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�X�V�X�e��	������
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void NEWS_TOPIC_Init( NEWS_TOPICBUF* p_wk, u32 heapID )
{
	int i, j;

	memset( p_wk, 0, sizeof(NEWS_TOPICBUF) );

	// �g�s�b�N�X�i�[�o�b�t�@������
	for( i=0; i<NEWS_TOPIC_MAX; i++ ){
		for( j=0; j<NEWS_TOPICNAME_NUM; j++ ){
			p_wk->topic_buf[ i ].p_name[j] = STRBUF_Create( NEWS_TOPICNAME_LEN, heapID );
		}
	}

	// �g�s�b�N���X�g�擪�_�~�[�f�[�^
	p_wk->topic_top.p_next = &p_wk->topic_top;
	p_wk->topic_top.p_last = &p_wk->topic_top;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�X�V�X�e��	�j������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void NEWS_TOPIC_Exit( NEWS_TOPICBUF* p_wk )
{
	int i, j;

	// �g�s�b�N�X�i�[�o�b�t�@�j��
	for( i=0; i<NEWS_TOPIC_MAX; i++ ){
		for( j=0; j<NEWS_TOPICNAME_NUM; j++ ){
			STRBUF_Delete( p_wk->topic_buf[ i ].p_name[j] );
		}
	}

	memset( p_wk, 0, sizeof(NEWS_TOPICBUF) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N	�g�s�b�N�Ǘ�����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void NEWS_TOPIC_Main( NEWS_TOPICBUF* p_wk )
{
	NEWS_TOPIC* p_next;
	NEWS_TOPIC* p_now;

	// �j���J�E���^�`�F�b�N
	// �D�揇�ʊǗ�
	p_now = p_wk->topic_top.p_next;
	while( p_now != &p_wk->topic_top ){
		p_next = p_now->p_next;

		// �D�揇�ʐݒ���Ȃ���
		p_now->pri = NEWS_TOPIC_PRI_NONE;

		// �j���J�E���g
		if( (p_now->dest - 1) > 0 ){
			p_now->dest --;
		}else{
			// �����͔j��
			NEWS_TOPIC_Remove( p_wk, p_now );
		}

		p_now = p_next;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�擪�f�[�^��j������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void NEWS_TOPIC_RemoveTop( NEWS_TOPICBUF* p_wk )
{
	NEWS_TOPIC_Remove( p_wk, p_wk->topic_top.p_next );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�󂢂Ă��郏�[�N���擾����
 *
 *	@param	p_wk	�g�s�b�N�V�X�e�����[�N
 *	@param	pri		�D�揇��
 *
 *	@return	�󂢂Ă郏�[�N
 */
//-----------------------------------------------------------------------------
static NEWS_TOPIC* NEWS_TOPIC_GetCleanWork( NEWS_TOPICBUF* p_wk, u16 pri )
{
	int i;
	NEWS_TOPIC* p_ret;

	p_ret = NULL;
	for( i=0; i<NEWS_TOPIC_MAX; i++ ){
		if( p_wk->topic_buf[i].p_next == NULL ){
			p_ret = &p_wk->topic_buf[i];
			break;
		}
	}

	if( p_ret == NULL ){
		// �D�揇�ʂ������ȏ�ȃg�s�b�N�������āA�󂢂Ă��郏�[�N�ɂ���
		for( i=0; i<NEWS_TOPIC_MAX; i++ ){
			if( p_wk->topic_buf[i].pri >= pri ){
				// �����N��j��
				NEWS_TOPIC_Remove( p_wk, &p_wk->topic_buf[i] );
				p_ret = &p_wk->topic_buf[i];
				break;
			}
		}
	}
	return p_ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�A���������Ȃ�
 *
 *	@param	p_last		�O�̃f�[�^
 *	@param	p_obj		����
 */
//-----------------------------------------------------------------------------
static void NEWS_TOPIC_AddList( NEWS_TOPIC* p_last, NEWS_TOPIC* p_obj )
{
	p_obj->p_next			= p_last->p_next;
	p_obj->p_last			= p_last;
	p_last->p_next			= p_obj;
	p_obj->p_next->p_last	= p_obj;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�X�����X�g�ɒǉ�����
 *
 *	@param	p_wk		���[�N
 *	@param	p_obj		�ǉ�����g�s�b�N
 */
//-----------------------------------------------------------------------------
static void NEWS_TOPIC_Add( NEWS_TOPICBUF* p_wk, NEWS_TOPIC* p_obj )
{
	// �D�揇�ʂ̏��������Ԃɏ����Œǉ����Ă���
	// ������T�[�`���Ă���

	// ������D�揇�ʂɏ]���Ēǉ�����
	{
		NEWS_TOPIC* p_last;

		p_last = p_wk->topic_top.p_last;
		while( p_last != &p_wk->topic_top ){

			// �D�揇�ʂ��傫���Ȃ����甲����
			if( p_last->pri < p_obj->pri ){
				break;
			}

			p_last = p_last->p_last;
		}

		NEWS_TOPIC_AddList( p_last, p_obj );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�X��j������
 *
 *	@param	p_wk		���[�N
 *	@param	p_obj		�j������g�s�b�N�X
 */
//-----------------------------------------------------------------------------
static void NEWS_TOPIC_Remove( NEWS_TOPICBUF* p_wk, NEWS_TOPIC* p_obj )
{
	p_obj->p_last->p_next = p_obj->p_next;
	p_obj->p_next->p_last = p_obj->p_last;

	//  �f�[�^�̔j��
	p_obj->p_last = NULL;
    p_obj->p_next = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�f�[�^�̐ݒ�
 *
 *	@param	p_obj		���[�N
 *	@param	data0		���[�N�f�[�^�P
 *	@param	data1		���[�N�f�[�^�Q
 *	@param	data2		���[�N�f�[�^�R
 *	@param	cp_p1		�v���C���[���P
 *	@param	cp_p2		�v���C���[���Q
 *	@param	cp_p3		�v���C���[���R
 *	@param	cp_p4		�v���C���[���S
 *	@param	idx_p1		�v���C���[�C���f�b�N�X�P
 *	@param	idx_p2		�v���C���[�C���f�b�N�X�Q
 *	@param	idx_p3		�v���C���[�C���f�b�N�X�R
 *	@param	idx_p4		�v���C���[�C���f�b�N�X�S
 *	@param	dest		�j���J�E���g�l
 *	@param	pri			�D�揇��
 *	@param	type		�^�C�v
 */
//-----------------------------------------------------------------------------
static void NEWS_TOPIC_SetData( NEWS_TOPIC* p_obj, u32 data0, u32 data1, u32 data2, const MYSTATUS* cp_p1, const MYSTATUS* cp_p2, const MYSTATUS* cp_p3, const MYSTATUS* cp_p4, u16 idx_p1, u16 idx_p2, u16 idx_p3, u16 idx_p4, u32 dest, u32 pri, u32 type )
{
	GF_ASSERT( type < NEWS_TOPICTYPE_NUM );
	p_obj->type		= type;
	p_obj->dest		= dest;
	p_obj->pri		= pri;
	p_obj->work[0]	= data0;
	p_obj->work[1]	= data1;
	p_obj->work[2]	= data2;
	p_obj->pl_idx[0]= idx_p1;
	p_obj->pl_idx[1]= idx_p2;
	p_obj->pl_idx[2]= idx_p3;
	p_obj->pl_idx[3]= idx_p4;
	if( cp_p1 ){
		MyStatus_CopyNameString( cp_p1, p_obj->p_name[0] );
	}
	if( cp_p2 ){
		MyStatus_CopyNameString( cp_p2, p_obj->p_name[1] );
	}
	if( cp_p3 ){
		MyStatus_CopyNameString( cp_p3, p_obj->p_name[2] );
	}
	if( cp_p4 ){
		MyStatus_CopyNameString( cp_p4, p_obj->p_name[3] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O���i�[����
 *
 *	@param	p_obj		���[�N
 *	@param	no			�i�[����o�b�t�@�i���o�[
 *	@param	cp_player	�v���C���[�X�e�[�^�X
 */
//-----------------------------------------------------------------------------
static void NEWS_TOPIC_SetName( NEWS_TOPIC* p_obj, u32 no, const MYSTATUS* cp_player )
{
	MyStatus_CopyNameString( cp_player, p_obj->p_name[no] );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�ڑ�
 *
 *	@param	p_obj			�g�s�b�N�I�u�W�F
 *	@param	cp_vip			VIP�t���O
 *	@param	p_wordset		���[�h�Z�b�g
 *	@param	p_msgman		���b�Z�[�W�}�l�[�W��
 *	@param	p_str			������i�[��
 *	@param	heapID			�q�[�v�h�c
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_TOPIC_GetSTRConnect( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID )
{
	STRBUF*				p_tmpstr;

	WORDSET_RegisterWord( p_wordset, 0,  p_obj->p_name[0], 0, TRUE, CasetteLanguage );	// ���O
	WORDSET_RegisterWord( p_wordset, 1,  p_obj->p_name[1], 0, TRUE, CasetteLanguage );	// ���O

	// �W�J���镶����쐬
	p_tmpstr = MSGMAN_AllocString( p_msgman, wflby_topic_03 );

	// �W�J
	WORDSET_ExpandStr( p_wordset, p_str, p_tmpstr );

	// ���b�Z�[�W�f�[�^�j��
	STRBUF_Delete( p_tmpstr );

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�e������
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_TOPIC_GetSTRItem( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID )
{
	STRBUF*				p_tmpstr;

	WORDSET_RegisterWord( p_wordset, 0,  p_obj->p_name[0], 0, TRUE, CasetteLanguage );	// ���O
	WORDSET_RegisterWord( p_wordset, 1,  p_obj->p_name[1], 0, TRUE, CasetteLanguage );	// ���O
	WORDSET_RegisterWiFiLobbyItemName( p_wordset, 2, p_obj->work[ NEWS_TOPIC_WK_ITEMNO ] );

	// �W�J���镶����쐬
	p_tmpstr = MSGMAN_AllocString( p_msgman, wflby_topic_04 );

	// �W�J
	WORDSET_ExpandStr( p_wordset, p_str, p_tmpstr );

	// ���b�Z�[�W�f�[�^�j��
	STRBUF_Delete( p_tmpstr );


	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����W
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_TOPIC_GetSTRMiniGame( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID )
{
	STRBUF*				p_tmpstr;


	// �~�j�Q�[����
	WORDSET_RegisterWiFiLobbyGameName( p_wordset, 0, p_obj->work[NEWS_TOPIC_WK_GAME_NO] );
	
	// �l���ŕ������ύX
	if( p_obj->work[NEWS_TOPIC_WK_GAME_PLAYFLAG] == TRUE ){
		u32 msgidx;

		// �W�J���镶����쐬
		switch( p_obj->work[ NEWS_TOPIC_WK_GAME_PLAYNUM ] ){
		case 3:
			WORDSET_RegisterWord( p_wordset, 1,  p_obj->p_name[0], 0, TRUE, CasetteLanguage );	// ���O
			WORDSET_RegisterWord( p_wordset, 2,  p_obj->p_name[1], 0, TRUE, CasetteLanguage );	// ���O
			WORDSET_RegisterWord( p_wordset, 3,  p_obj->p_name[2], 0, TRUE, CasetteLanguage );	// ���O
			msgidx = wflby_topic_14;
			break;

		case 2:
			WORDSET_RegisterWord( p_wordset, 1,  p_obj->p_name[0], 0, TRUE, CasetteLanguage );	// ���O
			WORDSET_RegisterWord( p_wordset, 2,  p_obj->p_name[1], 0, TRUE, CasetteLanguage );	// ���O
			msgidx = wflby_topic_15;
			break;

		case 4:
			WORDSET_RegisterWord( p_wordset, 1,  p_obj->p_name[0], 0, TRUE, CasetteLanguage );	// ���O
			WORDSET_RegisterWord( p_wordset, 2,  p_obj->p_name[1], 0, TRUE, CasetteLanguage );	// ���O
			WORDSET_RegisterWord( p_wordset, 3,  p_obj->p_name[2], 0, TRUE, CasetteLanguage );	// ���O
			WORDSET_RegisterWord( p_wordset, 4,  p_obj->p_name[3], 0, TRUE, CasetteLanguage );	// ���O
			msgidx = wflby_topic_02;
			break;
		
		default:
			return FALSE;	// ������쐬�s�\
		}
		p_tmpstr = MSGMAN_AllocString( p_msgman, msgidx );
	}else{

		WORDSET_RegisterWord( p_wordset, 1,  p_obj->p_name[0], 0, TRUE, CasetteLanguage );	// ���O
		WORDSET_RegisterNumber( p_wordset, 2, WFLBY_MINIGAME_MAX - p_obj->work[ NEWS_TOPIC_WK_GAME_PLAYNUM ],
				1, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );

		// �W�J���镶����쐬
		p_tmpstr = MSGMAN_AllocString( p_msgman, wflby_topic_01 );
	}

	// �W�J
	WORDSET_ExpandStr( p_wordset, p_str, p_tmpstr );

	// ���b�Z�[�W�f�[�^�j��
	STRBUF_Delete( p_tmpstr );


	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief		���Ճ{�[�h
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_TOPIC_GetSTRFoot( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID )
{
	STRBUF*				p_tmpstr;

	WORDSET_RegisterWiFiLobbyGameName( p_wordset, 0, p_obj->work[NEWS_TOPIC_WK_FOOT_NO] );
	WORDSET_RegisterWord( p_wordset, 1,  p_obj->p_name[0], 0, TRUE, CasetteLanguage );	// ���O
	WORDSET_RegisterNumber( p_wordset, 2, WFLBY_FOOT_MAX - p_obj->work[ NEWS_TOPIC_WK_FOOT_PLAYNUM ],
			1, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );

	// �W�J���镶����쐬
	p_tmpstr = MSGMAN_AllocString( p_msgman, wflby_topic_05 );

	// �W�J
	WORDSET_ExpandStr( p_wordset, p_str, p_tmpstr );

	// ���b�Z�[�W�f�[�^�j��
	STRBUF_Delete( p_tmpstr );


	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���v
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_TOPIC_GetSTRWldTimer( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID )
{
	STRBUF*				p_tmpstr;
	int i;
	u32 stridx;

	// ���O�ݒ�
	for( i=0; i<p_obj->work[NEWS_TOPIC_WK_WLDT_PLAYNUM]; i++ ){
		WORDSET_RegisterWord( p_wordset, i,  p_obj->p_name[i], 0, TRUE, CasetteLanguage );	// ���O
	}
	if( p_obj->type == NEWS_TOPICTYPE_WORLDTIMER ){
		WORDSET_RegisterWiFiLobbyGameName( p_wordset, 
				p_obj->work[NEWS_TOPIC_WK_WLDT_PLAYNUM], WFLBY_GAME_WLDTIMER );
	}else{
		WORDSET_RegisterWiFiLobbyGameName( p_wordset, 
				p_obj->work[NEWS_TOPIC_WK_WLDT_PLAYNUM], WFLBY_GAME_NEWS );
	}

	// �W�J���镶����쐬
	p_tmpstr = MSGMAN_AllocString( p_msgman, wflby_topic_06+(WFLBY_MINIGAME_MAX - p_obj->work[NEWS_TOPIC_WK_WLDT_PLAYNUM]) );

	// �W�J
	WORDSET_ExpandStr( p_wordset, p_str, p_tmpstr );

	// ���b�Z�[�W�f�[�^�j��
	STRBUF_Delete( p_tmpstr );


	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�ݒ�
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_TOPIC_GetSTREvent( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID )
{
	STRBUF*				p_tmpstr;
	u32 stridx;

	switch( p_obj->work[NEWS_TOPIC_WK_TIEV_NO] ){
	case WFLBY_EVENT_NEON:
		WORDSET_RegisterWiFiLobbyEventName( p_wordset, 0, WFLBY_EVENT_GMM_NEON );
		stridx = wflby_topic_11;
		break;
	case WFLBY_EVENT_FIRE:
		WORDSET_RegisterWiFiLobbyEventName( p_wordset, 0, WFLBY_EVENT_GMM_FIRE );
		stridx = wflby_topic_16;
		break;
	case WFLBY_EVENT_PARADE:
		WORDSET_RegisterWiFiLobbyEventName( p_wordset, 0, WFLBY_EVENT_GMM_PARADE );
		stridx = wflby_topic_12;
		break;

	case WFLBY_EVENT_CLOSE:		// �N���[�Y
		stridx = wflby_topic_13;
		break;

	default:
		return FALSE;
	}

	// �W�J���镶����쐬
	p_tmpstr = MSGMAN_AllocString( p_msgman, stridx );

	// �W�J
	WORDSET_ExpandStr( p_wordset, p_str, p_tmpstr );

	// ���b�Z�[�W�f�[�^�j��
	STRBUF_Delete( p_tmpstr );


	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP�������Ă����ݒ�
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_TOPIC_GetSTRVipIn( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID )
{
	// ���[�UID���M�p�ł��邩�`�F�b�N
	if( p_obj->work[NEWS_TOPIC_WK_VIPIN_IDX] >= WFLBY_PLAYER_MAX ){
		return FALSE;
	}
	
	// ���̐l��VIP���ǂ����`�F�b�N����
	if( cp_vip->vip[ p_obj->work[NEWS_TOPIC_WK_VIPIN_IDX] ] == FALSE ){
		return FALSE;
	}
	
	MSGMAN_GetString( p_msgman, wflby_topic_17, p_str );

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[������
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_TOPIC_GetSTRMgResultIn( NEWS_TOPIC* p_obj, const WFLBY_VIPFLAG* cp_vip, WORDSET* p_wordset, MSGDATA_MANAGER* p_msgman, STRBUF* p_str, u32 heapID )
{
	STRBUF*				p_tmpstr;
	u32 msgidx;
	int i;
	static const u16 sc_NEWS_TOPIC_BALLOON_MSG[ WFLBY_MINIGAME_MAX ] = {
		0,
		wflby_topic_23,
		wflby_topic_24,
		wflby_topic_19,
	};
	static const u16 sc_NEWS_TOPIC_BALLSLOWBALANCE_MSG[ WFLBY_MINIGAME_MAX ] = {
		wflby_topic_18,
		wflby_topic_20,
		wflby_topic_21,
		wflby_topic_22,
	};

	// �l������������������g�s�b�N�쐬�s�\
	if( NEWS_TOPIC_MGRESULT_CheckPlayerNum( p_obj->work[NEWS_TOPIC_WK_MGRESULT_NO], 
				p_obj->work[NEWS_TOPIC_WK_MGRESULT_PLAYNUM] ) == FALSE ){
		return FALSE;
	}

	// �~�j�Q�[����
	WORDSET_RegisterWiFiLobbyGameName( p_wordset, 0, p_obj->work[NEWS_TOPIC_WK_MGRESULT_NO] );
	
	// �l���ŕ������ύX
	// �W�J���镶����쐬
	for( i=0; i<p_obj->work[ NEWS_TOPIC_WK_MGRESULT_PLAYNUM ]; i++ ){
		WORDSET_RegisterWord( p_wordset, i+1,  p_obj->p_name[i], 0, TRUE, CasetteLanguage );	// ���O�ݒ�
	}
	if( (p_obj->work[NEWS_TOPIC_WK_MGRESULT_NO] == WFLBY_GAME_BALLSLOW) ||
		(p_obj->work[NEWS_TOPIC_WK_MGRESULT_NO] == WFLBY_GAME_BALANCEBALL) ){
		p_tmpstr = MSGMAN_AllocString( p_msgman, 
				sc_NEWS_TOPIC_BALLSLOWBALANCE_MSG[ p_obj->work[NEWS_TOPIC_WK_MGRESULT_PLAYNUM]-1] );
	}else{
		p_tmpstr = MSGMAN_AllocString( p_msgman, 
				sc_NEWS_TOPIC_BALLOON_MSG[ p_obj->work[NEWS_TOPIC_WK_MGRESULT_PLAYNUM]-1 ] );
	}

	// �W�J
	WORDSET_ExpandStr( p_wordset, p_str, p_tmpstr );

	// ���b�Z�[�W�f�[�^�j��
	STRBUF_Delete( p_tmpstr );

	return TRUE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	���ꂼ��̃g�s�b�N���[�U����IDX���擾����
 *
 *	@param	cp_obj		�I�u�W�F
 *	@param	count		�J�E���g
 *
 *	@retval	WFLBY_PLAYER_MAX	���̃C���f�b�N�X�ɂ���l�͂��Ȃ���
 *	@retval	���̂�				���̃C���f�b�N�X�̃v���C���[�̃��[�r�[���C���f�b�N�X
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	��b
 */
//-----------------------------------------------------------------------------
static u32 NEWS_TOPIC_GetUserIdxConnect( const NEWS_TOPIC* cp_obj, u32 count )
{

	// 2�ȏ�͂Ȃ�
	if( count >= 2 ){
		return WFLBY_PLAYER_MAX;
	}
	return cp_obj->pl_idx[count];
}
	
//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�e������
 */
//-----------------------------------------------------------------------------
static u32 NEWS_TOPIC_GetUserIdxItem( const NEWS_TOPIC* cp_obj, u32 count )
{
	// 2�ȏ�͂Ȃ�
	if( count >= 2 ){
		return WFLBY_PLAYER_MAX;
	}
	return cp_obj->pl_idx[count];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[��
 */
//-----------------------------------------------------------------------------
static u32 NEWS_TOPIC_GetUserIdxMiniGame( const NEWS_TOPIC* cp_obj, u32 count )
{
	if( cp_obj->work[ NEWS_TOPIC_WK_GAME_PLAYNUM ] <= count ){
		return WFLBY_PLAYER_MAX;
	}
	return cp_obj->pl_idx[ count ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ճ{�[�h
 */
//-----------------------------------------------------------------------------
static u32 NEWS_TOPIC_GetUserIdxFoot( const NEWS_TOPIC* cp_obj, u32 count )
{
	if( count >= 1 ){
		return WFLBY_PLAYER_MAX;
	}
	return cp_obj->pl_idx[count];
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���v
 */
//-----------------------------------------------------------------------------
static u32 NEWS_TOPIC_GetUserIdxWldTimer( const NEWS_TOPIC* cp_obj, u32 count )
{
	if( cp_obj->work[NEWS_TOPIC_WK_WLDT_PLAYNUM] <= count ){
		return WFLBY_PLAYER_MAX;
	}
	return cp_obj->pl_idx[count];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g	
 */
//-----------------------------------------------------------------------------
static u32 NEWS_TOPIC_GetUserIdxEvent( const NEWS_TOPIC* cp_obj, u32 count )
{
	return WFLBY_PLAYER_MAX;
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP����
 */
//-----------------------------------------------------------------------------
static u32 NEWS_TOPIC_GetUserIdxVipIn( const NEWS_TOPIC* cp_obj, u32 count )
{
	return WFLBY_PLAYER_MAX;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[������
 */
//-----------------------------------------------------------------------------
static u32 NEWS_TOPIC_GetUserIdxMgResult( const NEWS_TOPIC* cp_obj, u32 count )
{
	if( cp_obj->work[ NEWS_TOPIC_WK_MGRESULT_PLAYNUM ] <= count ){
		return WFLBY_PLAYER_MAX;
	}
	return cp_obj->pl_idx[ count ];
}



//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[�����ʃg�s�b�N	�l���`�F�b�N
 *
 *	@param	minigame		�~�j�Q�[���^�C�v
 *	@param	num				�l��
 *
 *	@retval	TRUE	���v
 *	@retval	FALSE	�s��
 */
//-----------------------------------------------------------------------------
static BOOL NEWS_TOPIC_MGRESULT_CheckPlayerNum( WFLBY_GAMETYPE minigame, u32 num )
{
	if( (minigame == WFLBY_GAME_BALLSLOW) || (minigame == WFLBY_GAME_BALANCEBALL) ){
		if( (num < 1) || (num > WFLBY_MINIGAME_MAX) ){
			return FALSE;
		}
	}else{
		if( (num < 2) || (num > WFLBY_MINIGAME_MAX) ){
			return FALSE;
		}
	}
	return TRUE;
}


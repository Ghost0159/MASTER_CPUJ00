//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_system_def.h
 *	@brief		WiFi���r�[�O���[�o���f�[�^
 *	@author		tomoya takahashi
 *	@data		2007.11.27
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_SYSTEM_DEF_H__
#define __WFLBY_SYSTEM_DEF_H__

#include "system/gamedata.h"

#include "lobby_news_data.h"
#include "wflby_def.h"

#include "wifi/dwc_lobbylib.h"

#include "application/wifi_p2pmatch_tool.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------


//-------------------------------------
///	�Ђ�΁@�V�X�e���@�G���[
//	�ʐM�G���[�Ƃ͈Ⴄ�L����̏�Ԃł̃G���[
//=====================================
typedef enum {
	WFLBY_SYSTEM_ERR_NONE,
	WFLBY_SYSTEM_ERR_CRC,		// CRC�`�F�b�N�ŃG���[���N����
} WFLBY_SYSTEM_ERR_TYPE;



//-------------------------------------
///	���C���`�����l���ʐM�R�}���h
//=====================================
enum {

	// ���A�p
	WFLBY_SYSTEM_MAIN_COMMCMD_TALK_REQ,		// ��b���N�G�X�g
	WFLBY_SYSTEM_MAIN_COMMCMD_TALK_ANS,		// ��b����
	WFLBY_SYSTEM_MAIN_COMMCMD_TALK_DATA,	// ��b�f�[�^
	WFLBY_SYSTEM_MAIN_COMMCMD_TALK_END,		// ��b�I�����N�G�X�g

	// �g�s�b�N�p
	WFLBY_SYSTEM_MAIN_COMMCMD_TOPIC_DATA,	// �g�s�b�N�f�[�^

	// �K�W�F�b�g�p
	WFLBY_SYSTEM_MAIN_COMMCMD_GADGET_DATA,	// �K�W�F�b�g�f�[�^

	// �t���[�g�p
	WFLBY_SYSTEM_MAIN_COMMCMD_FLOAT_ON,		// �t���[�g�ɂ̂����I
	WFLBY_SYSTEM_MAIN_COMMCMD_FLOAT_ANM,	// �t���[�g�A�j��

	
	// GS�ȍ~�͂�������ǉ�

	// ��
	WFLBY_SYSTEM_MAIN_COMMCMD_NUM,
} ;


//-------------------------------------
///	��b�V�[�P���X
//=====================================
typedef enum {
	WFLBY_TALK_SEQ_NONE,	// �����Ȃ�	��b���I�������Ă�������
	WFLBY_TALK_SEQ_B_ANS,	// ������ꑤ	����
	WFLBY_TALK_SEQ_A_SEL,	// �b��������	�I��
	WFLBY_TALK_SEQ_B_SEL,	// ������ꑤ	�I��
	WFLBY_TALK_SEQ_EXEND,	// �ً}�I��	��b���I�������Ă�������
} WFLBY_TALK_SEQ;


//-------------------------------------
///	�^�C���C�x���g�`�F�b�N�p�萔
//=====================================
typedef enum {
	WFLBY_EVENT_CHG_LOCK,
	WFLBY_EVENT_CHG_NEON_ROOM,
	WFLBY_EVENT_CHG_NEON_FLOOR,
	WFLBY_EVENT_CHG_NEON_MONU,
	WFLBY_EVENT_CHG_MINIGAME,
	WFLBY_EVENT_CHG_FIRE,
	WFLBY_EVENT_CHG_PARADE,
	WFLBY_EVENT_CHG_CLOSE,
} WFLBY_EVENT_CHG_TYPE;

//-------------------------------------
///	�^�C���C�x���g�`�F�b�N�ԉ΂̏��
//=====================================
typedef enum {
	WFLBY_EVENT_HANABI_BEFORE,	// �ԉΑO
	WFLBY_EVENT_HANABI_PLAY,	// �ԉΒ�
	WFLBY_EVENT_HANABI_AFTER,	// �ԉΌ�
} WFLBY_EVENT_HANABI_ST;


//-------------------------------------
///	�t���[�g�X�e�[�V����
//=====================================
typedef enum {
	WFLBY_FLOAT_STATION_RIGHT,		// �E
	WFLBY_FLOAT_STATION_CENTER,		// ��
	WFLBY_FLOAT_STATION_LEFT,		// ��
	WFLBY_FLOAT_STATION_NUM,		// �w��(�\�񃍃b�N�����̒l�ɂȂ�����N�����Ȃ�)
} WFLBY_FLOAT_STATION_TYPE;

//-------------------------------------
///	�t���[�g�����
//=====================================
typedef enum {
	WFLBY_FLOAT_STATE_BEFORE,	// ����O
	WFLBY_FLOAT_STATE_MOVE,		// ���쒆
	WFLBY_FLOAT_STATE_AFTER,	// �����
} WFLBY_FLOAT_MOVE_STATE;



//-------------------------------------
///	�V�񂾏��ۑ��o�b�t�@
//	�V�у^�C�v
//
//	��ɂ����قǁA�Љ��D�揇�ʂ�����
//=====================================
typedef enum {
	WFLBY_PLAYED_BALLTHROW,		// �^�}����
	WFLBY_PLAYED_BALANCEBALL,	// Balance�{�[��
	WFLBY_PLAYED_BALLOON,		// �ӂ�������
	WFLBY_PLAYED_WORLDTIMER,	// ���E���v	
	WFLBY_PLAYED_TOPIC,			// �L��j���[�X
	WFLBY_PLAYED_FOOTBOARD,		// ����BOARD
	WFLBY_PLAYED_ANKETO,		// �A���P�[�g
	WFLBY_PLAYED_TALK,			// �b������
	
	WFLBY_PLAYED_NUM,			// �ۑ�����V�у^�C�v�̐�
} WFLBY_PLAYED_TYPE;



//-------------------------------------
///	�ԉ΍\���f�[�^
//=====================================
typedef enum {
	WFLBY_FIRE_END,		// �I����
	WFLBY_FIRE_NONE,	// �ԉ΂Ȃ�
	WFLBY_FIRE_NORMAL,	// �ʏ�
	WFLBY_FIRE_KOUGO,	// ���ݔ���
	WFLBY_FIRE_NAMI,	// �g�󔭎�
	WFLBY_FIRE_CLIMAX,	// �Ō�
	WFLBY_FIRE_NUM,		// ����^�C�v��
} WFLBY_FIRE_TYPE;
#define WFLBY_FIRE_END_COUNT	( 120 )		// �ԉΏI�����̓����̃J�E���g�l






//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	WiFi���r�[���ʃV�X�e�����[�N
//=====================================
typedef struct _WFLBY_SYSTEM	WFLBY_SYSTEM;




//-------------------------------------
///	���[�U��{���
//=====================================
typedef struct _WFLBY_USER_PROFILE WFLBY_USER_PROFILE;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

// �Ђ�΃V�X�e�����G���[
extern WFLBY_SYSTEM_ERR_TYPE WFLBY_SYSTEM_GetError( const WFLBY_SYSTEM* cp_wk );

//  �L��ʐM�ߏ��ԃG���[
extern BOOL WFLBY_SYSTEM_GetExcessFoodError( const WFLBY_SYSTEM* cp_wk );


// ���̎擾
extern NEWS_DATA* WFLBY_SYSTEM_GetLobbyData( WFLBY_SYSTEM* p_wk );	// ���r�[�j���[�X
extern const WFLBY_WLDTIMER* WFLBY_SYSTEM_GetWldTimer( WFLBY_SYSTEM* p_wk );// ���E���v
extern const WFLBY_TIME* WFLBY_SYSTEM_GetWldTime( const WFLBY_SYSTEM* cp_wk );	// ����


//  �t���O�֌W
extern BOOL WFLBY_SYSTEM_FLAG_GetUserIn( WFLBY_SYSTEM* p_wk, u32 idx );	// �V�������[�U�������Ă�����
extern BOOL WFLBY_SYSTEM_FLAG_GetUserOut( WFLBY_SYSTEM* p_wk, u32 idx );// ���[�U���ސȂ�����
extern BOOL WFLBY_SYSTEM_FLAG_GetEventStart( WFLBY_SYSTEM* p_wk, WFLBY_EVENT_CHG_TYPE type );// �C�x���g���J�n������
extern BOOL WFLBY_SYSTEM_FLAG_GetUserProfileUpdate( WFLBY_SYSTEM* p_wk, u32 idx );// ���[�U�v���t�B�[�����X�V������
extern SAVEDATA* WFLBY_SYSTEM_GetSaveData( WFLBY_SYSTEM* p_wk );	// �Z�[�u�f�[�^�擾
extern BOOL WFLBY_SYSTEM_FLAG_GetArceus( const WFLBY_SYSTEM* cp_wk );	// �A���Z�E�X�\�����t���O	TRUE�F����
extern BOOL WFLBY_SYSTEM_FLAG_GetAnketoInput( const WFLBY_SYSTEM* cp_wk );	// �A���P�[�g�ɓ��������ǂ���
extern BOOL WFLBY_SYSTEM_FLAG_GetAnketoView( const WFLBY_SYSTEM* cp_wk );	// �A���P�[�g�̌��ʂ��������ǂ���
extern void WFLBY_SYSTEM_FLAG_SetAnketoView( WFLBY_SYSTEM* p_wk );	// �A���P�[�g�̌��ʂ��������ǂ���


// �C�x���g��Ԏ擾
extern BOOL WFLBY_SYSTEM_Event_GetLock( const WFLBY_SYSTEM* cp_wk );
extern BOOL WFLBY_SYSTEM_Event_GetEndCM( const WFLBY_SYSTEM* cp_wk );
extern BOOL WFLBY_SYSTEM_Event_GetEnd( const WFLBY_SYSTEM* cp_wk );
extern WFLBY_EVENT_HANABI_ST WFLBY_SYSTEM_Event_GetHanabi( const WFLBY_SYSTEM* cp_wk );
extern BOOL WFLBY_SYSTEM_Event_GetParade( const WFLBY_SYSTEM* cp_wk );
extern BOOL WFLBY_SYSTEM_Event_GetMiniGameStop( const WFLBY_SYSTEM* cp_wk );
extern BOOL WFLBY_SYSTEM_Event_GetNeon( const WFLBY_SYSTEM* cp_wk );
extern PPW_LOBBY_TIME_EVENT WFLBY_SYSTEM_Event_GetNeonRoom( const WFLBY_SYSTEM* cp_wk );
extern PPW_LOBBY_TIME_EVENT WFLBY_SYSTEM_Event_GetNeonFloor( const WFLBY_SYSTEM* cp_wk );
extern PPW_LOBBY_TIME_EVENT WFLBY_SYSTEM_Event_GetNeonMonu( const WFLBY_SYSTEM* cp_wk );


// �~�j�Q�[���t���O
extern void WFLBY_SYSTEM_MGFLAG_SetLock( WFLBY_SYSTEM* p_wk );
extern BOOL WFLBY_SYSTEM_MGFLAG_GetLock( const WFLBY_SYSTEM* cp_wk );


extern BOOL WFLBY_SYSTEM_MG_CheckErrorEnd( const WFLBY_SYSTEM* cp_wk );
extern void WFLBY_SYSTEM_MG_SetErrorEnd( WFLBY_SYSTEM* p_wk, BOOL flag );


// �~�j�Q�[���ʐM����̃v���C���[IDX
extern void WFLBY_SYSTEM_PLIDX_Clear( WFLBY_SYSTEM* p_wk );
extern void WFLBY_SYSTEM_PLIDX_Set( WFLBY_SYSTEM* p_wk, s32 userid, u32 netid );
extern void WFLBY_SYSTEM_PLIDX_SetMyData( WFLBY_SYSTEM* p_wk, u32 netid );
extern void WFLBY_SYSTEM_PLIDX_Get( const WFLBY_SYSTEM* cp_wk, WFLBY_MINIGAME_PLIDX* p_buff );


// �A�v����ԃt���O
// �A�v���������I���������̂���ݒ肷��
extern void WFLBY_SYSTEM_APLFLAG_SetForceEnd( WFLBY_SYSTEM* p_wk );
extern BOOL WFLBY_SYSTEM_APLFLAG_GetForceEnd( const WFLBY_SYSTEM* cp_wk );


// BGM���Đ�
extern void WFLBY_SYSTEM_SetLockBGMChange( WFLBY_SYSTEM* p_wk, BOOL flag );
extern void WFLBY_SYSTEM_SetNowBGM( const WFLBY_SYSTEM* cp_wk );
extern void WFLBY_SYSTEM_SetBGMVolumeDown( WFLBY_SYSTEM* p_wk, BOOL flag );


// ���[�U�v���t�B�[��
// ���[�U�v���t�B�[���̃|�C���^�����[�J�����[�N�ɕۑ����Ȃ����ƁA
// �v���t�B�[���̒��g���Q�Ƃ���Ƃ��́A�Q�Ƃ��邲�Ƃ�WFLBY_SYSTEM_GetUserProfile�֐��Ȃǂ�
// �v���t�B�[���|�C���^���擾���Ă��������B
// �������Ȃ��Ƃ����ގ������l�̃��������N���A���ꂽ�v���t�B�[�����Q�Ƃ���\�����o�邽�߂ł��B
extern u32 WFLBY_SYSTEM_GetMyIdx( const WFLBY_SYSTEM* cp_wk );	// �����̃o�b�t�@�C���f�b�N�X
extern s32 WFLBY_SYSTEM_GetMyUserID( const WFLBY_SYSTEM* cp_wk );
extern BOOL WFLBY_SYSTEM_GetUserRoomIn( const WFLBY_SYSTEM* cp_wk, u32 idx );	// ���݂��Ă��邩�`�F�b�N
extern const WFLBY_USER_PROFILE* WFLBY_SYSTEM_GetUserProfile( const WFLBY_SYSTEM* cp_wk, u32 idx );
extern const WFLBY_USER_PROFILE* WFLBY_SYSTEM_GetMyProfile( const WFLBY_SYSTEM* cp_wk );		// DWCLobby���C�u���������璼�ڎ擾(���C�u��������X�V��������)
extern WFLBY_USER_PROFILE* WFLBY_SYSTEM_GetMyProfileLocal( WFLBY_SYSTEM* p_wk );				// WFLBY_SYSLTEM���ɂƂ��Ă��镨���擾�i�ŐV�j
extern void WFLBY_SYSTEM_SetMyProfile( WFLBY_SYSTEM* p_wk, const WFLBY_USER_PROFILE* cp_data );
extern void WFLBY_SYSTEM_SetMyStatus( WFLBY_SYSTEM* p_wk, WFLBY_STATUS_TYPE status );
extern void WFLBY_SYSTEM_SetLastAction( WFLBY_SYSTEM* p_wk, WFLBY_LASTACTION_TYPE action, s32 userid );
extern void WFLBY_SYSTEM_SetMyItem( WFLBY_SYSTEM* p_wk, WFLBY_ITEMTYPE item );
extern void WFLBY_SYSTEM_SetSelectWazaType( WFLBY_SYSTEM* p_wk, WFLBY_POKEWAZA_TYPE type1, WFLBY_POKEWAZA_TYPE type2 );
extern void WFLBY_SYSTEM_SetAnketoData( WFLBY_SYSTEM* p_wk, u32 anketo_no, u32 select );
extern void WFLBY_SYSTEM_GetProfileMyStatus( const WFLBY_USER_PROFILE* cp_profile, MYSTATUS* p_buff, u32 heapID );
extern u32 WFLBY_SYSTEM_GetProfileStatus( const WFLBY_USER_PROFILE* cp_profile );
extern WFLBY_LASTACTION_TYPE WFLBY_SYSTEM_GetProfileLastAction( const WFLBY_USER_PROFILE* cp_profile, u32 idx );
extern s32 WFLBY_SYSTEM_GetProfileLastActionUserID( const WFLBY_USER_PROFILE* cp_profile, u32 idx );
extern s32 WFLBY_SYSTEM_GetProfileUserID( const WFLBY_USER_PROFILE* cp_profile );
extern u32 WFLBY_SYSTEM_GetProfileTrID( const WFLBY_USER_PROFILE* cp_profile );
extern WFLBY_TIME WFLBY_SYSTEM_GetProfileInTime( const WFLBY_USER_PROFILE* cp_profile );
extern WFLBY_TIME WFLBY_SYSTEM_GetProfileWldTime( const WFLBY_USER_PROFILE* cp_profile );
extern u32 WFLBY_SYSTEM_GetProfileSex( const WFLBY_USER_PROFILE* cp_profile );
extern u32 WFLBY_SYSTEM_GetProfileRagionCode( const WFLBY_USER_PROFILE* cp_profile );
extern BOOL WFLBY_SYSTEM_CheckProfileRagionCode( const WFLBY_USER_PROFILE* cp_profile );
extern u32 WFLBY_SYSTEM_GetProfileRagionCodeOrg( const WFLBY_USER_PROFILE* cp_profile );	// �g�p�����ӁI�I�I�I�@�����o�l�����̂܂܎擾
 
extern u32 WFLBY_SYSTEM_GetProfileTrType( const WFLBY_USER_PROFILE* cp_profile );
extern WFLBY_LASTACTION_TYPE WFLBY_SYSTEM_GetProfileTrTypetoLastActionNo( const WFLBY_USER_PROFILE* cp_profile );
extern u32 WFLBY_SYSTEM_GetProfileNation( const WFLBY_USER_PROFILE* cp_profile );
extern u32 WFLBY_SYSTEM_GetProfileArea( const WFLBY_USER_PROFILE* cp_profile );
extern BOOL WFLBY_SYSTEM_GetProfileZukanZenkoku( const WFLBY_USER_PROFILE* cp_profile );
extern BOOL WFLBY_SYSTEM_GetProfileGameClear( const WFLBY_USER_PROFILE* cp_profile );
extern BOOL WFLBY_SYSTEM_CheckGetItem( const WFLBY_USER_PROFILE* cp_profile );
extern WFLBY_ITEMTYPE WFLBY_SYSTEM_GetProfileItem( const WFLBY_USER_PROFILE* cp_profile );
extern s64 WFLBY_SYSTEM_GetProfileGameStartTime( const WFLBY_USER_PROFILE* cp_profile );
extern BOOL WFLBY_SYSTEM_CheckProfileGameStartTime( const WFLBY_USER_PROFILE* cp_profile );
extern WFLBY_POKEWAZA_TYPE WFLBY_SYSTEM_GetProfileWazaType( const WFLBY_USER_PROFILE* cp_profile, u32 idx );
extern void WFLBY_SYSTEM_GetProfileAnketoData( const WFLBY_USER_PROFILE* cp_profile, WFLBY_ANKETO* p_data );


// �~�j�Q�[����pMYSTATUS�f�[�^
extern MYSTATUS* WFLBY_SYSTEM_GetMgMyStatus( const WFLBY_SYSTEM* cp_wk );


// VIP�t���O�̎擾
extern BOOL WFLBY_SYSTEM_GetUserVipFlag( const WFLBY_SYSTEM* cp_wk, u32 idx );
extern void WFLBY_SYSTEM_GetVipFlagWk( const WFLBY_SYSTEM* cp_wk, WFLBY_VIPFLAG* p_buff );

// �������Ƃ΃t���O�̎擾
extern BOOL WFLBY_SYSTEM_GetUserAikotobaFlag( const WFLBY_SYSTEM* cp_wk, u32 idx );

// �������Ƃ΃��b�Z�[�W�̎擾
extern const WFLBY_AIKOTOBA_DATA* WFLBY_SYSTEM_GetUserAikotobaStr( const WFLBY_SYSTEM* cp_wk, u32 idx );


// �T�u�`�����l���ւ̎Q�����쐬
extern u32 WFLBY_SYSTEM_GetSubChanPlNum( const WFLBY_SYSTEM* cp_wk );
extern u32 WFLBY_SYSTEM_GetSubChanEnum( const WFLBY_SYSTEM* cp_wk, u32 idx );


// �g�s�b�N���M
extern void WFLBY_SYSTEM_TOPIC_SendConnect( WFLBY_SYSTEM* p_wk, u32 user_a, u32 user_b );
extern void WFLBY_SYSTEM_TOPIC_SendItem( WFLBY_SYSTEM* p_wk, u32 user_a, u32 user_b, WFLBY_ITEMTYPE item );
extern void WFLBY_SYSTEM_TOPIC_SendMiniGame( WFLBY_SYSTEM* p_wk, WFLBY_GAMETYPE minigame, u32 num, u32 user_0, u32 user_1, u32 user_2, u32 user_3, BOOL play );
extern void WFLBY_SYSTEM_TOPIC_SendMiniGameTopResult( WFLBY_SYSTEM* p_wk, WFLBY_GAMETYPE minigame, u32 num, u32 user_0, u32 user_1, u32 user_2, u32 user_3 );


// �Q�[���f�[�^
extern u32 WFLBY_SYSTEM_GetSeason( const WFLBY_SYSTEM* cp_wk );
extern u32 WFLBY_SYSTEM_GetRoomType( const WFLBY_SYSTEM* cp_wk );
extern u32 WFLBY_SYSTEM_GetLockTime( const WFLBY_SYSTEM* cp_wk );	// ���b�N����
extern u32 WFLBY_SYSTEM_GetCloseTime( const WFLBY_SYSTEM* cp_wk );	// ���b�N���������Ă���N���[�Y�܂ł̎���


// �K�W�F�b�g
extern BOOL WFLBY_SYSTEM_GetGadGetFlag( const WFLBY_SYSTEM* cp_wk, u32 idx ); 
extern void WFLBY_SYSTEM_SendGadGetData( WFLBY_SYSTEM* p_wk, WFLBY_ITEMTYPE gadget );


// �t���[�g�f�[�^
// idx�̓t�H���[�g���Ȃ̃C���f�b�N�X
// plidx�̓v���C���[�C���f�b�N�X
// float�̓v���C���[�C���f�b�N�X
extern WFLBY_FLOAT_MOVE_STATE WFLBY_SYSTEM_GetFloatMove( const WFLBY_SYSTEM* cp_wk, u32 floatidx );
extern u32 WFLBY_SYSTEM_GetFloatCount( const WFLBY_SYSTEM* cp_wk, u32 floatidx );
extern u32 WFLBY_SYSTEM_GetFloatCountMax( const WFLBY_SYSTEM* cp_wk );
extern u32 WFLBY_SYSTEM_GetFloatReserve( const WFLBY_SYSTEM* cp_wk, u32 idx );
extern u32 WFLBY_SYSTEM_GetFloatPlIdxReserve( const WFLBY_SYSTEM* cp_wk, u32 plidx );
extern u32 WFLBY_SYSTEM_SetFloatReserve( WFLBY_SYSTEM* p_wk, u32 plidx, WFLBY_FLOAT_STATION_TYPE station );
extern void WFLBY_SYSTEM_ResetFloatReserve( WFLBY_SYSTEM* p_wk, u32 plidx );
extern void WFLBY_SYSTEM_SetFloatAnm( WFLBY_SYSTEM* p_wk, u32 plidx );
extern BOOL WFLBY_SYSTEM_GetFloatAnm( const WFLBY_SYSTEM* cp_wk, u32 idx );
extern void WFLBY_SYSTEM_CleanFloatAnm( WFLBY_SYSTEM* p_wk, u32 idx );
extern void WFLBY_SYSTEM_GetFloatIdxOfs( u32 idx, u32* p_floatidx, u32* p_floatofs );
extern void WFLBY_SYSTEM_SetFloatReserveLock( WFLBY_SYSTEM* p_wk, u32 idx, WFLBY_FLOAT_STATION_TYPE station );
extern WFLBY_FLOAT_STATION_TYPE WFLBY_SYSTEM_GetFloatReserveLock( const WFLBY_SYSTEM* cp_wk, u32 idx );
extern void WFLBY_SYSTEM_SendFloatOn( WFLBY_SYSTEM* p_wk );
extern void WFLBY_SYSTEM_SendFloatAnm( WFLBY_SYSTEM* p_wk );

// ��b�f�[�^
// ���S�ɃN���[���ȏ�Ԃɂ���
extern void WFLBY_SYSTEM_EndTalk( WFLBY_SYSTEM* p_wk );	// 
// ���M
extern BOOL WFLBY_SYSTEM_SendTalkReq( WFLBY_SYSTEM* p_wk, u32 idx );			// ��b���N�G�X�g
extern void WFLBY_SYSTEM_SendTalkData( WFLBY_SYSTEM* p_wk, u16 data );	// ��b�f�[�^�𑗂�
extern void WFLBY_SYSTEM_SendTalkEnd( WFLBY_SYSTEM* p_wk );			// ��b�I��
extern void WFLBY_SYSTEM_SendTalkExEnd( WFLBY_SYSTEM* p_wk );		// �����I���������Ƃ�������

// �擾
extern BOOL WFLBY_SYSTEM_CheckTalk( const WFLBY_SYSTEM* cp_wk );
extern BOOL WFLBY_SYSTEM_CheckTalkReq( const WFLBY_SYSTEM* cp_wk );			// �b��������ꂽ��
extern BOOL WFLBY_SYSTEM_GetTalkMsk( const WFLBY_SYSTEM* cp_wk, u32 idx );	// �b�����������Ƃ����邩�`�F�b�N
extern u32 WFLBY_SYSTEM_GetTalkIdx( const WFLBY_SYSTEM* cp_wk );			// ��b����
extern BOOL WFLBY_SYSTEM_GetTalkFirst( const WFLBY_SYSTEM* cp_wk );
extern WFLBY_TALK_SEQ WFLBY_SYSTEM_GetTalkSeq( const WFLBY_SYSTEM* cp_wk );	// ��b�V�[�P���X
extern u16 WFLBY_SYSTEM_GetTalkRecv( const WFLBY_SYSTEM* cp_wk );			// ��b�I�����擾
extern BOOL WFLBY_SYSTEM_CheckTalkCount( const WFLBY_SYSTEM* cp_wk, u32 idx );	// ��b�ł��邩�擾
extern BOOL WFLBY_SYSTEM_CheckTalkError( const WFLBY_SYSTEM* cp_wk );	// �G���[��ԃ`�F�b�N	�G���[�ɂȂ������b�I��������
extern s16	WFLBY_SYSTEM_GetRecvWaitTime( const WFLBY_SYSTEM* cp_wk );	// ��M�҂����Ԃ��擾����
extern void WFLBY_SYSTEM_SetTalkBStart( WFLBY_SYSTEM* p_wk );			// �b���������������΂ɂ����̂Ŋ|����ꂽ���͉�b�J�n
extern BOOL WFLBY_SYSTEM_CheckTalkBStart( const WFLBY_SYSTEM* cp_wk );	// �b��������ꂽ�ꍇ�A�b���������肪�����̂��΂ɗ������擾
//  ��b�񐔂̃J�E���g
extern void WFLBY_SYSTEM_AddTalkCount( WFLBY_SYSTEM* p_wk );	// ��b���������񐔂��J�E���g
extern void WFLBY_SYSTEM_AddTalkCountEx( WFLBY_SYSTEM* p_wk, u32 idx );	// ��b���������񐔂��J�E���g
// ��b�������Ƃ��邩�}�X�N
extern void WFLBY_SYSTEM_SetTalkMsk( WFLBY_SYSTEM* p_wk, u32 idx );	// ��b�������Ƃ���̐ݒ������

// �`���m�[�g�f�[�^�̐ݒ�
extern void WFLBY_SYSTEM_FNOTE_SetTalk( WFLBY_SYSTEM* p_wk, u32 plidx );
extern void WFLBY_SYSTEM_FNOTE_SetGetGadget( WFLBY_SYSTEM* p_wk, u32 plidx );
extern void WFLBY_SYSTEM_FNOTE_SetPlayMinigame( WFLBY_SYSTEM* p_wk, WFLBY_GAMETYPE minigame );
extern void WFLBY_SYSTEM_FNOTE_SetParade( WFLBY_SYSTEM* p_wk );

// ���R�[�h�f�[�^�̐ݒ�
extern void WFLBY_SYSTEM_RECORD_AddTalk( WFLBY_SYSTEM* p_wk );
extern void WFLBY_SYSTEM_RECORD_AddMinigame( WFLBY_SYSTEM* p_wk );


// �V�񂾃f�[�^�̕ۑ��Ǝ擾
extern void WFLBY_SYSTEM_PLAYED_SetPlay( WFLBY_SYSTEM* p_wk, WFLBY_PLAYED_TYPE type );
extern WFLBY_PLAYED_TYPE WFLBY_SYSTEM_PLAYED_GetOsusumePlayType( const WFLBY_SYSTEM* cp_wk );

// �K�W�F�b�g�擾
extern WFLBY_ITEMTYPE WFLBY_SYSTEM_RandItemGet( const WFLBY_SYSTEM* cp_wk );


// �ԉΏ��
extern WFLBY_FIRE_TYPE WFLBY_SYSTEM_FIRE_GetType( const WFLBY_SYSTEM* cp_wk );
extern u32 WFLBY_SYSTEM_FIRE_GetTypeMoveCount( const WFLBY_SYSTEM* cp_wk );

// �L����̃A���P�[�g���ʂ�ۑ�����
extern void WFLBY_SYSTEM_SetAnketoResult( WFLBY_SYSTEM* p_wk, const ANKETO_QUESTION_RESULT* cp_data );
extern BOOL WFLBY_SYSTEM_GetAnketoResult( const WFLBY_SYSTEM* cp_wk, ANKETO_QUESTION_RESULT* p_data );



// Wi-Fi�N���u�̏�Ԃ�NONE�ɂ��邽�߂̃o�b�t�@
extern void WFLBY_SYSTEM_WiFiClubBuff_Init( WFLBY_SYSTEM* p_wk );
extern void WFLBY_SYSTEM_WiFiClubBuff_Exit( WFLBY_SYSTEM* p_wk );

#ifdef PM_DEBUG
extern void WFLBY_SYSTEM_DEBUG_SetItem( WFLBY_SYSTEM* p_wk, u32 item );
extern void WFLBY_SYSTEM_DEBUG_SetLangCode( WFLBY_USER_PROFILE* p_profile, u32 code );
#endif

#endif		// __WFLBY_SYSTEM_DEF_H__


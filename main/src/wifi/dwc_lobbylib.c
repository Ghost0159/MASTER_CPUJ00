//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		dwc_lobbylib.c
 *	@brief		���r�[�T�[�o�Ǘ��֐�
 *	@author		tomoya takahashi
 *	@data		2007.12.06
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "savedata/wifilist.h"
#include "savedata/wifihistory.h"

#include "msgdata/msg_wifi_place_msg_world.h"

#include "wifi/dwc_lobbylib.h"
#include "wifi/dwc_rap.h"

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



//-------------------------------------
///	WiFi���r�[�Q�[�����
//	�����DP�Ɠ��l
//=====================================
#define DWC_LOBBY_GAMENAME			("pokemonplatds")			// �Q�[����
#define DWC_LOBBY_SECRET_KEY		("IIup73")					// �V�[�N���b�g�L�[



#ifdef  PM_DEBUG
#define DWC_LOBBY_PRINT(...)		OS_TPrintf(__VA_ARGS__)
//#define DWC_LOBBY_USERENUM			// USERENUM�\��
//#define DWC_LOBBY_ANKETO_DRAW			// �A���P�[�g���̃f�o�b�N�\��


// �f�o�b�N�J�n���
extern BOOL D_Tomoya_WiFiLobby_DebugStart;
extern BOOL D_Tomoya_WiFiLobby_LockTime_Long;

/*
#if defined(DEBUG_ONLY_FOR_ohno)	// ��삳���p
#define DWC_LOBBY_USE_PREFIX		( PPW_LOBBY_CHANNEL_PREFIX_DEBUG1 )	// �f�o�b�N�Ŏg�p����PREFIX
#endif

#if defined(DEBUG_ONLY_FOR_matsuda)	// ���c�����p
#define DWC_LOBBY_USE_PREFIX		( PPW_LOBBY_CHANNEL_PREFIX_DEBUG3 )	// �f�o�b�N�Ŏg�p����PREFIX
#endif

#if defined(lee_hyunjung)	// �肳���p
#define DWC_LOBBY_USE_PREFIX		( PPW_LOBBY_CHANNEL_PREFIX_DEBUG4 )	// �f�o�b�N�Ŏg�p����PREFIX
#endif

#if defined(DEBUG_ONLY_FOR_nohara)	// �쌴�����p
#define DWC_LOBBY_USE_PREFIX		( PPW_LOBBY_CHANNEL_PREFIX_DEBUG5 )	// �f�o�b�N�Ŏg�p����PREFIX
#endif

#if defined(DEBUG_ONLY_FOR_tomoya_takahashi)	// ������p
#define DWC_LOBBY_USE_PREFIX		( PPW_LOBBY_CHANNEL_PREFIX_DEBUG2 )	// �f�o�b�N�Ŏg�p����PREFIX
#endif
//*/

#ifndef DWC_LOBBY_USE_PREFIX
#define DWC_LOBBY_USE_PREFIX		( GF_DWC_LOBBY_CHANNEL_PREFIX )
#endif

#else
// ���i��
#define DWC_LOBBY_USE_PREFIX		( GF_DWC_LOBBY_CHANNEL_PREFIX )		// ���i�łŎg�p����PREFIX
#define DWC_LOBBY_PRINT(...)		((void)0);
#endif



//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�`�����l���^�C�v
//=====================================
enum {
	DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN,	// ���C���̃`�����l��
	DWC_LOBBY_LOCALCHANNEL_TYPE_SUB,	// �T�u�`�����l���i���Ճ{�[�h�Ȃǁj
	DWC_LOBBY_LOCALCHANNEL_TYPE_NUM,	// �`�����l����
} ;



//-------------------------------------
///	�`�����l���f�[�^	KEY
//=====================================
enum{
	DWC_LOBBY_CHANNEL_KEY_WLDDATA,	//  �ގ��ҍ��A�n��f�[�^
	DWC_LOBBY_CHANNEL_KEY_NUM,	//  �ގ��ҍ��A�n��f�[�^
};
static const char* sc_DWC_LOBBY_CHANNEL_KEY_NAME[ DWC_LOBBY_CHANNEL_KEY_NUM ] = {
	"b_lby_wlddata",
};


//-------------------------------------
///	�ގ���	���E�f�[�^
//=====================================
#define DWC_LOBBY_WLDDATA_NATION_NONE		( country000 )	// �����ȍ�ID


//-------------------------------------
///	�X�P�W���[���o�b�t�@�T�C�Y
//=====================================
// �X�P�W���[����126������邱�Ƃ��o����T�C�Y�Ȃ̂ő��v
// ����X�P�W���[���̌�����18���x
#define DWC_LOBBY_SCHEDULE_SIZE				( 1024 )	


//-------------------------------------
///	�~�j�Q�[����W���
//	��W���
//=====================================
enum{
	DWC_LOBBY_MG_ST_RECRUIT,	// ��W��
	DWC_LOBBY_MG_ST_GAMEING,	// �V�ђ�
};
#define DWC_LOBBY_MG_TIMELIMIT_SEC	( 30 )	//1�b�̒P��


//-------------------------------------
///	�W�v����t���O�̃o�b�t�@��
//=====================================
#define DWC_LOBBY_ANKETO_LANG_SUMMARIZE_FLAG_BUFF_NUM	( 12 )


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------



//-------------------------------------
///	�ގ��҂̐��E�f�[�^
//	4byte
//=====================================
typedef struct {
	u16	nation;		///<��
	u8	area;		///<�n��
	u8	flag;		///<�f�[�^�L��	�i�f�[�^���i�[����Ă���̂��ǂ����j
} DWC_WLDDATA;

//-------------------------------------
///	���[�U��
//=====================================
typedef struct {
	s16			num;			// ���݂̓o�^��
	u16			tblnum;			// �e�[�u����
	s32*		p_tbl;			// ���[�UID�e�[�u��
} DWC_LOBBY_USERIDTBL;


//-------------------------------------
///	���b�Z�[�W�R�}���h
//=====================================
typedef struct {
	const DWC_LOBBY_MSGCOMMAND*	cp_msgtbl;
	u32							msgtblnum;
	void*						p_work;
} DWC_LOBBY_MSGCMD;


//-------------------------------------
///	�~�j�Q�[����W�f�[�^
//=====================================
typedef struct {
	PPW_LobbyRecruitInfo	mg_data;
	u16						mg_flag;
	u16						mg_timelimit;
	s32						mg_parent;
} DWC_LOBBY_MGDATA;


//-------------------------------------
///	VIP���
//=====================================
typedef struct {
	PPW_LobbyVipRecord* p_vipbuff;
	u16					num;	// �o�b�t�@����
	u16					datanum;// ���ۂɃf�[�^�������Ă��鐔
} DWC_LOBBY_VIP;


//-------------------------------------
///	�A���P�[�g���
//=====================================
typedef struct {
	PPW_LobbyQuestionnaire	question;
	u32						submit_state;
} DWC_ANKETO_DATA;



//-------------------------------------
///	Wi-Fi���r�[���C�u�������[�N
//=====================================
typedef struct {
	//  �Z�[�u�f�[�^
	SAVEDATA*		p_save;	
	WIFI_LIST*		p_wifilist;
	WIFI_HISTORY*	p_wifihistory;

	// �R�[���o�b�N�f�[�^
	DWC_LOBBY_CALLBACK	callback;
	void*				p_callbackwork;	// �R�[���o�b�N�p���[�U���[�N

	// ���[�U���
	u8*				p_profile;		// �v���t�B�[���o�b�t�@	size��profilesize*PPW_LOBBY_MAX_PLAYER_NUM_MAIN
	u32				profilesize;	// �v���t�B�[���T�C�Y

	// �S�����̃V�X�e�����v���t�B�[��
	PPW_LobbySystemProfile	sysprofile[PPW_LOBBY_MAX_PLAYER_NUM_MAIN];

	// ���[�UID��
	DWC_LOBBY_USERIDTBL	useridtbl[ DWC_LOBBY_LOCALCHANNEL_TYPE_NUM ];

	// �t���O�S
	u8				status;			// �X�e�[�^�X
	u8				subchan_type;	// �T�u�`�����l���^�C�vDWC_LOBBY_SUBCHAN_TYPE 
	u8				subchan_status;	// �T�u�`�����l���X�e�[�^�XDWC_LOBBY_CHANNEL_STATE
	u8				subchan_login_success;	// �T�u�`�����l���Ƀ��O�C������������
	u8				wlddata_send;	// ���f�[�^���M���t���O
	u8				excess_flood;	// �ʐM�ߏ�G���[
	u8				download_error;	// �T�[�o�f�[�^download�G���[			
	u8				profile_error;	// �v���t�B�[���s���G���[			

	// �ގ��Ґ��E�f�[�^
	DWC_WLDDATA		wfldata[ DWC_WLDDATA_MAX ];

	// �ʐM���b�Z�[�W�R�}���h
	DWC_LOBBY_MSGCMD	msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_NUM ];

	// LOBBY����
	PPW_LobbyTimeInfo	time;

	// �X�P�W���[���f�[�^
	u8			schedulebuff[DWC_LOBBY_SCHEDULE_SIZE];
	u32			schedulesize;

	// ��W�f�[�^
	DWC_LOBBY_MGDATA		mg_data[ DWC_LOBBY_MG_NUM ];
	u16						mg_myentry;		// �G���g���[���Ă�~�j�Q�[���^�C�v
	u8						mg_myparent;	// �������e�Ȃ�TRUE
	u8						mg_myend;		// �e�ŕ�W���I�����Ȃ��Ă͂����Ȃ��Ƃ�TRUE

	// VIP���
	DWC_LOBBY_VIP	vipdata;

	// �A���P�[�g���
	DWC_ANKETO_DATA anketo;
	
	//
	// ����̃|�P�����o�[�W�����ł͉��ɒǉ����Ă���
	//
	
} DWC_LOBBYLIB_WK;



//-----------------------------------------------------------------------------
/**
 *					���r�[���C�u�������[�N
 */
//-----------------------------------------------------------------------------
static DWC_LOBBYLIB_WK* p_DWC_LOBBYLIB_WK = NULL;




//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	��{�G���[�`�F�b�N
//=====================================
static BOOL DWC_LOBBY_CheckCommonErr( PPW_LOBBY_RESULT result );


//-------------------------------------
///	�G���[��Ԃ̃N���A
//=====================================
static void DWC_LOBBY_ClearSystemErr( void );


//-------------------------------------
///	PPW�̃`�����l���^�C�v��DWC_LOBBY�̃`�����l���^�C�v�ɕύX����
//	PPW_LOBBY_CHANNEL_KIND_MAIN	�̂Ƃ���DWC_LOBBY_SUBCHAN_NONE��Ԃ��܂��@
//=====================================
static DWC_LOBBY_SUBCHAN_TYPE DWC_LOBBY_GetSubChannelType( PPW_LOBBY_CHANNEL_KIND channel );
static PPW_LOBBY_CHANNEL_KIND DWC_LOBBY_GetChannelKind( DWC_LOBBY_SUBCHAN_TYPE subchan_type );


//-------------------------------------
///	�ގ��ҍ��R�[�h�Ɏ����̃f�[�^��ݒ肷��
//=====================================
static BOOL DWC_LOBBY_WLDDATA_SetMyData( DWC_LOBBYLIB_WK* p_sys );


//-------------------------------------
///	�v���t�B�[���o�b�t�@�A�N�Z�X
//=====================================
static void DWC_LOBBY_Profile_Init( DWC_LOBBYLIB_WK* p_sys, u32 size, u32 heapID );
static void DWC_LOBBY_Profile_Exit( DWC_LOBBYLIB_WK* p_sys );
static void DWC_LOBBY_Profile_SetData( DWC_LOBBYLIB_WK* p_sys, s32 userid, const void* cp_data );
static void DWC_LOBBY_Profile_SetDataEx( DWC_LOBBYLIB_WK* p_sys, s32 userid, const void* cp_data, u32 datasize );
static const void* DWC_LOBBY_Profile_GetData( const DWC_LOBBYLIB_WK* cp_sys, s32 userid );
static void DWC_LOBBY_Profile_CleanData( DWC_LOBBYLIB_WK* p_sys, s32 userid );

//-------------------------------------
///	�V�X�e���v���t�B�[���o�b�t�@�A�N�Z�X
//=====================================
static void DWC_LOBBY_SysProfile_SetData( DWC_LOBBYLIB_WK* p_sys, s32 userid, const PPW_LobbySystemProfile* cp_data );
static const PPW_LobbySystemProfile* DWC_LOBBY_SysProfile_GetData( const DWC_LOBBYLIB_WK* cp_sys, s32 userid );
static void DWC_LOBBY_SysProfile_CleanData( DWC_LOBBYLIB_WK* p_sys, s32 userid );




//-------------------------------------
///	���[�U�񋓃V�X�e��
//=====================================
static void DWC_LOBBY_UserIDTbl_Init( DWC_LOBBYLIB_WK* p_sys, u32 maintblnum, u32 subtblnum, u32 heapID );
static void DWC_LOBBY_UserIDTbl_Exit( DWC_LOBBYLIB_WK* p_sys );
static void DWC_LOBBY_UserIDTbl_Clear( DWC_LOBBYLIB_WK* p_sys, u32 channeltype );
static void DWC_LOBBY_UserIDTbl_AddId( DWC_LOBBYLIB_WK* p_sys, s32 userid, u32 channeltype );
static void DWC_LOBBY_UserIDTbl_DelId( DWC_LOBBYLIB_WK* p_sys, s32 userid, u32 channeltype );
static s32 DWC_LOBBY_UserIDTbl_GetIdx( const DWC_LOBBYLIB_WK* cp_sys, s32 userid, u32 channeltype );
static s32 DWC_LOBBY_UserIDTbl_GetUserid( const DWC_LOBBYLIB_WK* cp_sys, s32 idx, u32 channeltype );
static void DWC_LOBBY_UserIDTbl_SUBCHAN_SetSysProfileData( DWC_LOBBYLIB_WK* p_sys, DWC_LOBBY_SUBCHAN_TYPE subchan_type );
static BOOL DWC_LOBBY_UserIDTbl_SUBCHAN_CheckSysProfileLogin( const DWC_LOBBYLIB_WK* cp_sys, DWC_LOBBY_SUBCHAN_TYPE subchan_type );

//-------------------------------------
///	�R�[���o�b�N�S
//=====================================
static void DWC_LOBBY_CallBack_JoinChannel( BOOL success, PPW_LOBBY_CHANNEL_KIND channel );
static void DWC_LOBBY_CallBack_Connect( s32 userid, const PPW_LobbySystemProfile* cp_sysprofile, const u8* cp_userprofile, u32 user_profilesize );
static void DWC_LOBBY_CallBack_DesconnectedChannel( s32 userid );
static void DWC_LOBBY_CallBack_StringMessageRecv( s32 userid, PPW_LOBBY_CHANNEL_KIND channel, s32 type, const char* cp_message );
static void DWC_LOBBY_CallBack_BynaryMessageRecv( s32 userid, PPW_LOBBY_CHANNEL_KIND channel, s32 type, const u8* cp_data, u32 datasize );
static void DWC_LOBBY_CallBack_ChannelDataRecv( BOOL success, BOOL broadcast, PPW_LOBBY_CHANNEL_KIND channel, s32 userid, const char* p_key, const u8* cp_data, u32 datasize, void* p_param );
static void DWC_LOBBY_CallBack_SystemProfileUpdate( s32 userid, const PPW_LobbySystemProfile* cp_profile );
static void DWC_LOBBY_CallBack_UserProfileUpdate( s32 userid, const u8* cp_profile, u32 size );
static void DWC_LOBBY_CallBack_Recruit( s32 userid, const PPW_LobbyRecruitInfo* cp_info );
static void DWC_LOBBY_CallBack_RecruitStop( s32 userid, const PPW_LobbyRecruitInfo* cp_info );
static void DWC_LOBBY_CallBack_Schedule( PPW_LOBBY_TIME_EVENT event );
static void DWC_LOBBY_CallBack_CheckProfile( PPW_LOBBY_STATS_RESULT result, const u8 *userProfile, u32 userProfileSize );
static BOOL DWC_LOBBY_CallBack_NotifySchedule( PPW_LOBBY_STATS_RESULT result, const PPW_LobbySchedule* p_schedule );
static void DWC_LOBBY_CallBack_Vip( PPW_LOBBY_STATS_RESULT result, const PPW_LobbyVipRecord* cp_vipRecords, u32 num );
static void DWC_LOBBY_CallBack_Anketo( PPW_LOBBY_STATS_RESULT result, const PPW_LobbyQuestionnaire* cp_questionnaire );
static void DWC_LOBBY_CallBack_AnketoSubmit( PPW_LOBBY_STATS_RESULT result );
static void DWC_LOBBY_CallBack_ExcessFlood( u32 floodWeight );


//-------------------------------------
///	�~�j�Q�[����W
//=====================================
static void DWC_LOBBY_MG_Init( DWC_LOBBYLIB_WK* p_sys );
static void DWC_LOBBY_MG_Exit( DWC_LOBBYLIB_WK* p_sys );
static void DWC_LOBBY_MG_SetData( DWC_LOBBYLIB_WK* p_sys, DWC_LOBBY_MG_TYPE type, s32 userid, const PPW_LobbyRecruitInfo* cp_mg_data );
static BOOL DWC_LOBBY_MG_ResetData( DWC_LOBBYLIB_WK* p_sys, DWC_LOBBY_MG_TYPE type, s32 userid );
static BOOL DWC_LOBBY_MG_Check( const DWC_LOBBYLIB_WK* cp_sys, DWC_LOBBY_MG_TYPE type );
static u16 DWC_LOBBY_MG_CalcTimeLimit( const DWC_LOBBYLIB_WK* cp_sys, DWC_LOBBY_MG_TYPE type );
static const PPW_LobbyRecruitInfo* DWC_LOBBY_MG_GetData( const DWC_LOBBYLIB_WK* cp_sys, DWC_LOBBY_MG_TYPE type );
static void DWC_LOBBY_MG_Update( DWC_LOBBYLIB_WK* p_sys );
static void DWC_LOBBY_MG_SetRecruitStartTime( PPW_LobbyRecruitInfo* p_info, const s64* cp_time );
static void DWC_LOBBY_MG_GetRecruitStartTime( const PPW_LobbyRecruitInfo* cp_info, s64* p_time );
static void DWC_LOBBY_MG_ConnectCallBack( u16 aid, void* p_work );
static void DWC_LOBBY_MG_DisConnectCallBack( u16 aid, void* p_work );


//-------------------------------------
///	�X�P�W���[���f�[�^�A�N�Z�X�֐�
//=====================================
static u32 DWC_LOBBY_SCHEDULE_GetEventTime( const PPW_LobbySchedule* cp_schedule, PPW_LOBBY_TIME_EVENT event );



//-------------------------------------
///	VIP���@�֐�
//=====================================
static void DWC_LOBBY_VIPDATA_Init( DWC_LOBBY_VIP* p_vip, u32 buffnum, u32 heapID );
static void DWC_LOBBY_VIPDATA_Exit( DWC_LOBBY_VIP* p_vip );
static void DWC_LOBBY_VIPDATA_SetData( DWC_LOBBY_VIP* p_vip, const PPW_LobbyVipRecord* cp_vipRecords, u32 num );
static BOOL DWC_LOBBY_VIPDATA_CheckVip( const DWC_LOBBY_VIP* cp_vip, s32 profileid );
static s32 DWC_LOBBY_VIPDATA_GetAikotoba( const DWC_LOBBY_VIP* cp_vip, s32 profileid );


//-------------------------------------
///	�A���P�[�g���	�֐�
//=====================================
static void DWC_LOBBY_ANKETO_Set( DWC_ANKETO_DATA* p_wk, const PPW_LobbyQuestionnaire* cp_question );
static void DWC_LOBBY_DEBUG_Printf( const PPW_LobbyQuestionnaireRecord* cp_data );


//----------------------------------------------------------------------------
/**
 *	@brief	DWC	���r�[���C�u�����Ǘ��V�X�e��	������
 *
 *	@param	heapID			�q�[�vID
 *	@param	p_save			�Z�[�u�f�[�^
 *	@param	profilesize		�v���t�B�[���P�l���̃��[�N�T�C�Y
 *	@param	cp_callback		�R�[���o�b�N���
 *	@param	p_callbackwork	�R�[���o�b�N���[�N
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_Init( u32 heapID, SAVEDATA* p_save, u32 profilesize, const DWC_LOBBY_CALLBACK* cp_callback,  void* p_callbackwork )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK == NULL );
	GF_ASSERT( profilesize < PPW_LOBBY_MAX_BINARY_SIZE );	// �v���t�B�[���T�C�Y�ő吔�`�F�b�N

	// ���[�N�m��
	p_DWC_LOBBYLIB_WK = sys_AllocMemory( heapID, sizeof(DWC_LOBBYLIB_WK) );
	memset( p_DWC_LOBBYLIB_WK, 0, sizeof(DWC_LOBBYLIB_WK) );
	
	// �Z�[�u�f�[�^�ۑ�
	p_DWC_LOBBYLIB_WK->p_save			= p_save;
	p_DWC_LOBBYLIB_WK->p_wifilist		= SaveData_GetWifiListData( p_save );
	p_DWC_LOBBYLIB_WK->p_wifihistory	= SaveData_GetWifiHistory( p_save );

	// �R�[���o�b�N�f�[�^
	p_DWC_LOBBYLIB_WK->callback			= *cp_callback;
	p_DWC_LOBBYLIB_WK->p_callbackwork	= p_callbackwork;

	//  ���[�U���	�v���t�B�[���o�b�t�@�쐬
	DWC_LOBBY_Profile_Init( p_DWC_LOBBYLIB_WK, profilesize, heapID );
	
	// ���[�UID��	������
	DWC_LOBBY_UserIDTbl_Init( p_DWC_LOBBYLIB_WK, PPW_LOBBY_MAX_PLAYER_NUM_MAIN, PPW_LOBBY_MAX_PLAYER_NUM_FOOT, heapID );

	// �~�j�Q�[���}�b�`���O�f�[�^������
	DWC_LOBBY_MG_Init( p_DWC_LOBBYLIB_WK );

	// VIP���o�b�t�@������
	DWC_LOBBY_VIPDATA_Init( &p_DWC_LOBBYLIB_WK->vipdata, DWC_LOBBY_VIP_BUFFNUM, heapID );	
}

//----------------------------------------------------------------------------
/**
 *	@brief	DWC	���r�[���C�u�����Ǘ��V�X�e��	�j��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_Exit( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// VIP���o�b�t�@ �j��
	DWC_LOBBY_VIPDATA_Exit( &p_DWC_LOBBYLIB_WK->vipdata );	

	// ���[�UID��	�j��
	DWC_LOBBY_UserIDTbl_Exit( p_DWC_LOBBYLIB_WK );
	
	// ���[�U���	�v���t�B�[���o�b�t�@�j��
	DWC_LOBBY_Profile_Exit( p_DWC_LOBBYLIB_WK );

	// ��{��j��
	sys_FreeMemoryEz( p_DWC_LOBBYLIB_WK );
	p_DWC_LOBBYLIB_WK = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	DWC ���r�[	�X�V
 *
 *	// ���C���`�����l�����
 *	@retval	DWC_LOBBY_CHANNEL_STATE_NONE,           // �`�����l���ɓ����Ă��Ȃ��B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_LOGINWAIT,		// �`�����l���ɓ������B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_CONNECT,		// �`�����l���ɓ����ς݁B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_LOGOUTWAIT,     // �`�����l���ɑގ����B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_ERROR           // �`�����l����Ԃ��擾�ł��܂���ł����B
 */
//-----------------------------------------------------------------------------
DWC_LOBBY_CHANNEL_STATE DWC_LOBBY_Update( void )
{
	PPW_LOBBY_STATE	ppw_state;

	// ���r�[���쒆���`�F�b�N
	if( p_DWC_LOBBYLIB_WK == NULL ){ return DWC_LOBBY_CHANNEL_STATE_NONE; }


	// �T�u�`�����l���̓����񋓂�\��
#if 0
	{
		int i;

		OS_TPrintf( "sub chan enum\n" );
		for( i=0; i<p_DWC_LOBBYLIB_WK->useridtbl[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ].tblnum; i++ ){
			OS_TPrintf( " %d\n", p_DWC_LOBBYLIB_WK->useridtbl[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ].p_tbl[i] );
		}
	}
#endif
	
	// ���r�[�X�V
	ppw_state = PPW_LobbyProcess();
	
//	DWC_LOBBY_PRINT( "ppw_state %d\n", ppw_state );
	switch( ppw_state ){
	// �����s���Ă��Ȃ����
    case PPW_LOBBY_STATE_NOTINITIALIZED:         ///< ���C�u����������������Ă��Ȃ��B
		p_DWC_LOBBYLIB_WK->status = DWC_LOBBY_CHANNEL_STATE_NONE;
		break;

	// ���O�C����
    case PPW_LOBBY_STATE_CONNECTING:             ///< �`���b�g�T�[�o�ɐڑ����B
    case PPW_LOBBY_STATE_CONNECTED:              ///< �`���b�g�T�[�o�ɐڑ������B
    case PPW_LOBBY_STATE_DOWNLOAD:               ///< �T�[�o����ݒ���_�E�����[�h�J�n�B
    case PPW_LOBBY_STATE_DOWNLOAD_WAIT:          ///< �T�[�o����ݒ���_�E�����[�h���B
    case PPW_LOBBY_STATE_SEARCHINGCHANNEL:       ///< ���C���`�����l�����������B
    case PPW_LOBBY_STATE_ENTERMAINCHANNEL:       ///< ���C���`�����l���ɓ����J�n�B
    case PPW_LOBBY_STATE_PREPARINGMAINCHANNEL:   ///< ���C���`�����l���ɓ����y�я������B
		p_DWC_LOBBYLIB_WK->status = DWC_LOBBY_CHANNEL_STATE_LOGINWAIT;
		break;

	// �ڑ���
    case PPW_LOBBY_STATE_READY:                  ///< ���C���`�����l���ɓ��������B
		p_DWC_LOBBYLIB_WK->status = DWC_LOBBY_CHANNEL_STATE_CONNECT;
		break;

	// ���O�A�E�g��
    case PPW_LOBBY_STATE_CLOSING:                ///< ���C�u�����I���������B
		p_DWC_LOBBYLIB_WK->status = DWC_LOBBY_CHANNEL_STATE_LOGOUTWAIT;
		break;

	// �G���[��
	case PPW_LOBBY_STATE_ERROR:                   ///< ���C�u�����ɒv���I�ȃG���[���������B::PPW_LobbyGetLastError�֐��ŃG���[���擾��V���b�g�_�E�����Ă��������B
		p_DWC_LOBBYLIB_WK->status = DWC_LOBBY_CHANNEL_STATE_ERROR;
		break;
	}

#ifdef DWC_LOBBY_USERENUM
	{
		if( sys.trg & PAD_BUTTON_L ){
			u32 num;
			static s32 buff[20];
			s32* p_buff;
			int i;
			PPW_LOBBY_RESULT lbresult;

			num = 20;
			p_buff = buff;
			lbresult = PPW_LobbyEnumUserId(p_buff, &num);
			if( lbresult == PPW_LOBBY_RESULT_SUCCESS ){

				for( i=0; i<num; i++ ){
					OS_TPrintf( "userid %d[%d]\n", i, p_buff[i] );
				}
			}else{
				OS_TPrintf( "userid result err\n" );
			}

			OS_TPrintf( "my id=%d\n", PPW_LobbyGetMyUserId() );
		}
	}
#endif
	

	// �ڑ����̏���
	if( p_DWC_LOBBYLIB_WK->status == DWC_LOBBY_CHANNEL_STATE_CONNECT ){


		// P2P�ڑ����C��
		{
			DWC_LOBBY_MG_Update( p_DWC_LOBBYLIB_WK );
		}
		
		// ���r�[�̎��Ԃ��擾
		{
			PPW_LobbyGetTimeInfo( &p_DWC_LOBBYLIB_WK->time );
		}

		
		// �T�u�`�����l���������Ȃ�T�u�`�����l���̏�Ԃ��`�F�b�N
		{
			PPW_LOBBY_CHANNEL_STATE	subchan_state;

			subchan_state = PPW_LobbyGetSubChannelState();
			switch( subchan_state ){
			// �`�����l���ɓ����Ă��Ȃ��B
			case PPW_LOBBY_CHANNEL_STATE_NONE:           
				p_DWC_LOBBYLIB_WK->subchan_status	= DWC_LOBBY_CHANNEL_STATE_NONE;
				p_DWC_LOBBYLIB_WK->subchan_type		= DWC_LOBBY_SUBCHAN_NONE;
				break;
			// �`�����l���ɓ����������B
			case PPW_LOBBY_CHANNEL_STATE_UNAVAILABLE:    
				p_DWC_LOBBYLIB_WK->subchan_status = DWC_LOBBY_CHANNEL_STATE_LOGINWAIT;
				break;
			// �`�����l���ɓ����ς݁B
			case PPW_LOBBY_CHANNEL_STATE_AVAILABLE:

				//  �X�e�[�^�X��LOGOUT�����Ȃ��̂ŁA�V�X�e�����Ŏ蓮��LOGOUT��Ԃɂ��܂��B�@STATE��LOGOUT�̂Ƃ��͂�����CONNECT�ɏ��������邱�Ƃ͂��܂���B
				if( p_DWC_LOBBYLIB_WK->subchan_status != DWC_LOBBY_CHANNEL_STATE_LOGOUTWAIT ){
					p_DWC_LOBBYLIB_WK->subchan_status = DWC_LOBBY_CHANNEL_STATE_CONNECT;
				}
				break;
			// �`�����l����Ԃ��擾�ł��܂���ł����B
			case PPW_LOBBY_CHANNEL_STATE_ERROR:           
				p_DWC_LOBBYLIB_WK->subchan_status = DWC_LOBBY_CHANNEL_STATE_ERROR;
				break;

			}
		}
	}


	return DWC_LOBBY_GetState();
}

//----------------------------------------------------------------------------
/**
 *	@brief		���݋N�����Ă���G���[���擾����
 *	
 *	@retval	PPW_LOBBY_ERROR_NONE,                   ///< �G���[�����B
 *	@retval	PPW_LOBBY_ERROR_UNKNOWN,                ///< �s���ȃG���[�B
 *	@retval	PPW_LOBBY_ERROR_ALLOC,                  ///< �������m�ۂɎ��s�B
 *	@retval	PPW_LOBBY_ERROR_SESSION                 ///< �v���I�ȒʐM�G���[�B
 *	@retval	PPW_LOBBY_ERROR_STATS_SESSION			///< �v���I�ȒʐM�G���[(���[���ݒ�T�[�o)�B  
 */
//-----------------------------------------------------------------------------
PPW_LOBBY_ERROR DWC_LOBBY_GetErr( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	return PPW_LobbyGetLastError();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���[�萔����\������i���o�[���擾
 *
 *	@param	error	
 *
 *	@return	�\�����鐔��
 */
//-----------------------------------------------------------------------------
s32 DWC_LOBBY_GetErrNo( PPW_LOBBY_ERROR error )
{
	return PPW_LobbyToErrorCode( error );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�T�[�o�Ƀ��O�C��
 *
 *	@param	cp_loginprofile			���O�C�����̃v���t�B�[��subChannelKind
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	���s		���s�����ꍇ�G���[�^�C�v���擾����Logout���Ă�������
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_Login( const void* cp_loginprofile )
{
	return DWC_LOBBY_LoginEx( cp_loginprofile, DWC_LOBBY_USE_PREFIX );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�T�[�o�Ƀ��O�C��
 *
 *	@param	cp_loginprofile			���O�C�����̃v���t�B�[��subChannelKind
 *	@param	prefix					�ڑ���̃`�����l�����
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	���s		���s�����ꍇ�G���[�^�C�v���擾����Logout���Ă�������
 *
 *
 *	prefix	
 *		PPW_LOBBY_CHANNEL_PREFIX_RELEASE
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG1
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG2
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG3
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG4
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG5
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG6
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG7
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG8
 *		PPW_LOBBY_CHANNEL_PREFIX_DEBUG9
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_LoginEx( const void* cp_loginprofile, u32 prefix )
{
	PPW_LobbyCallbacks	callbacks;
	PPW_LOBBY_RESULT	result;
	
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// ���r�[�R�[���o�b�N�e�[�u���쐬
	{
		callbacks.lobbyJoinChannelCallback				= DWC_LOBBY_CallBack_JoinChannel;
		callbacks.lobbyPlayerConnectedCallback			= DWC_LOBBY_CallBack_Connect;
		callbacks.lobbyPlayerDisconnectedCallback		= DWC_LOBBY_CallBack_DesconnectedChannel;
		callbacks.lobbyStringMassageReceivedCallback	= DWC_LOBBY_CallBack_StringMessageRecv;
		callbacks.lobbyBinaryMassageReceivedCallback	= DWC_LOBBY_CallBack_BynaryMessageRecv;
		callbacks.lobbyGetChannelDataCallback			= DWC_LOBBY_CallBack_ChannelDataRecv;
		callbacks.lobbySystemProfileUpdatedCallback		= DWC_LOBBY_CallBack_SystemProfileUpdate;
		callbacks.lobbyUserProfileUpdatedCallback		= DWC_LOBBY_CallBack_UserProfileUpdate;
		callbacks.lobbyRecruitCallback					= DWC_LOBBY_CallBack_Recruit;
		callbacks.lobbyRecruitStoppedCallback			= DWC_LOBBY_CallBack_RecruitStop;
		callbacks.lobbyScheduleProgressCallback			= DWC_LOBBY_CallBack_Schedule;
		callbacks.lobbyCheckProfileCallback				= DWC_LOBBY_CallBack_CheckProfile;
		callbacks.lobbyNotifyScheduleCallback			= DWC_LOBBY_CallBack_NotifySchedule;
		callbacks.lobbyGetVipCallback					= DWC_LOBBY_CallBack_Vip;
		callbacks.lobbyGetQuestionnaireCallback			= DWC_LOBBY_CallBack_Anketo;
		callbacks.lobbySubmitQuestionnaireCallback		= DWC_LOBBY_CallBack_AnketoSubmit;
		callbacks.lobbyExcessFloodCallback				= DWC_LOBBY_CallBack_ExcessFlood;
	}

	// ���r�[�Ƀ��O�C��
	{
		DWCUserData*		p_dwcuser;
		
		p_dwcuser = WifiList_GetMyUserInfo( p_DWC_LOBBYLIB_WK->p_wifilist );
		result = PPW_LobbyInitializeAsync( DWC_LOBBY_GAMENAME,
				DWC_LOBBY_SECRET_KEY, 
				prefix,
				&callbacks, 
				p_dwcuser, 
				cp_loginprofile, 
				p_DWC_LOBBYLIB_WK->profilesize );

	}

	// �G���[�`�F�b�N
	return DWC_LOBBY_CheckCommonErr( result );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�C�������҂�
 *
 *	@retval	TRUE	���O�C������
 *	@retval	FALSE	���O�C����or���O�C���ȊO�̏��
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_LoginWait( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	if( p_DWC_LOBBYLIB_WK->status == DWC_LOBBY_CHANNEL_STATE_CONNECT ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�T�[�o���烍�O�A�E�g
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_Logout( void )
{
	PPW_LOBBY_RESULT	result;
	BOOL retval;
		
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// �G���[��ԉ��
	DWC_LOBBY_ClearSystemErr();

	// ���C�u�����I��
	result = PPW_LobbyShutdownAsync();

	// �G���[�`�F�b�N
	retval =  DWC_LOBBY_CheckCommonErr( result );
	GF_ASSERT( retval );
}

//----------------------------------------------------------------------------
/**
 *	@brief		���O�A�E�g�����҂�
 *
 *	@retval		TRUE	����
 *	@retval		FALSE	���O�A�E�g��or���O�A�E�g��ԈȊO�̏��
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_LogoutWait( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	if( p_DWC_LOBBYLIB_WK->status == DWC_LOBBY_CHANNEL_STATE_NONE ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[���C���`�����l���̏�Ԃ�Ԃ�
 *
 *	@retval	DWC_LOBBY_CHANNEL_STATE_NONE,           // �`�����l���ɓ����Ă��Ȃ��B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_LOGINWAIT,		// �`�����l���ɓ������B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_CONNECT,		// �`�����l���ɓ����ς݁B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_LOGOUTWAIT,     // �`�����l���ɑގ����B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_ERROR           // �`�����l����Ԃ��擾�ł��܂���ł����B�G���[��ނ��擾���ăV���b�g�_�E�����Ă�������
 */
//-----------------------------------------------------------------------------
DWC_LOBBY_CHANNEL_STATE DWC_LOBBY_GetState( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// �T�u�`�����l�����O�C�����ŃG���[�ɂȂ����Ƃ����G���[��ԂɕύX
	if( p_DWC_LOBBYLIB_WK->subchan_type	!= DWC_LOBBY_SUBCHAN_NONE ){
		if( p_DWC_LOBBYLIB_WK->subchan_status == DWC_LOBBY_CHANNEL_STATE_ERROR ){
			return DWC_LOBBY_CHANNEL_STATE_ERROR;
		}
	}

	// �v���t�B�[�����G���[
	if( p_DWC_LOBBYLIB_WK->profile_error ){
		return DWC_LOBBY_CHANNEL_STATE_ERROR;
	}

	// VIP�f�[�^��X�P�W���[����download�Ɏ��s������G���[
	if( p_DWC_LOBBYLIB_WK->download_error ){
		return DWC_LOBBY_CHANNEL_STATE_ERROR;
	}

	return p_DWC_LOBBYLIB_WK->status;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�ʐM�ߏ��Ԃ��擾����
 *
 *	@retval	TRUE	�ʐM�ߏ���
 *	@retval	FALSE	�ʏ���
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_CheckExcessFlood( void )
{
	BOOL flag;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	flag = p_DWC_LOBBYLIB_WK->excess_flood;
	p_DWC_LOBBYLIB_WK->excess_flood = FALSE;
	return flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[���s���`�F�b�N
 *
 *	@retval	TRUE		�s��
 *	@retval	FALSE		�s������Ȃ�
 *
 *	��DWC_LOBBY_GetState()�̌��ʂ��G���[�ɂȂ�����Ă�ł�������
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_CheckProfileError( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	if( p_DWC_LOBBYLIB_WK->profile_error ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID���擾����
 *
 *	@retval	DWC_LOBBY_INVALID_USER_ID	�s���ȃ��[�UID 
 *	@retval	���[�UID
 */
//-----------------------------------------------------------------------------
s32 DWC_LOBBY_GetMyUserID( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	return PPW_LobbyGetMyUserId();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃v���t�B�[�����X�V����
 *
 *	@param	cp_data			�v���t�B�[���f�[�^
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_SetMyProfile( const void* cp_data )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	DWC_LOBBY_Profile_SetData( p_DWC_LOBBYLIB_WK, PPW_LobbyGetMyUserId(), cp_data );
	PPW_LobbyUpdateMyProfile( cp_data, p_DWC_LOBBYLIB_WK->profilesize, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃v���t�B�[�����擾����
 *
 *	@return	�����̃v���t�B�[��
 */
//-----------------------------------------------------------------------------
const void* DWC_LOBBY_GetMyProfile( void )
{
	s32 userid;
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	userid = DWC_LOBBY_GetMyUserID();
	return DWC_LOBBY_Profile_GetData( p_DWC_LOBBYLIB_WK, userid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID�̃v���t�B�[�����擾
 *	
 *	@param	userid	���[�UID
 */
//-----------------------------------------------------------------------------
const void* DWC_LOBBY_GetUserProfile( s32 userid )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	return DWC_LOBBY_Profile_GetData( p_DWC_LOBBYLIB_WK, userid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�C���������Ԃ��擾
 *
 *	@param	userid		���[�UID
 *	@param	p_time		���Ԋi�[��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_GetLoginTime( s32 userid, s64* p_time )
{
	const PPW_LobbySystemProfile* cp_sysprofile;
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	cp_sysprofile = DWC_LOBBY_SysProfile_GetData( p_DWC_LOBBYLIB_WK, userid );
	*p_time = cp_sysprofile->enterTime;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��Ԑ̂��炢��v���C���[���擾
 *
 *	@retval	DWC_LOBBY_INVALID_USER_ID	��������Ȃ��Ƃ�
 *	@retval	��ԌÂ����炢��v���C���[
 */
//-----------------------------------------------------------------------------
s32	DWC_LOBBY_GetOldUser( void )
{
	int i;
	s32 old_userid = DWC_LOBBY_INVALID_USER_ID;
	s64 old_time = 0x0fffffffffffffff;		// ���肦�Ȃ�����

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// ���[�UID�̎��Ԃ��擾���Ĉ�ԍŏ����炢��l�����
	for( i=0; i<PPW_LOBBY_MAX_PLAYER_NUM_MAIN; i++ ){
		{
			s32 userid;
			s64 time;

			userid = DWC_LOBBY_UserIDTbl_GetUserid( p_DWC_LOBBYLIB_WK, 
					i, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
			if( userid != DWC_LOBBY_INVALID_USER_ID ){
				// ���Ԃ��擾
				DWC_LOBBY_GetLoginTime( userid, &time );
				if( old_time > time ){
					old_time = time;
					old_userid = userid;
				}
			}
		}
	}

	return old_userid;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��Ԑ̂��炢��v���C���[���擾
 *
 *	@param	desc_user		���ؒf����l
 * 
 *	@retval	DWC_LOBBY_INVALID_USER_ID	��������Ȃ��Ƃ�
 *	@retval	��ԌÂ����炢��v���C���[
 */
//-----------------------------------------------------------------------------
s32	DWC_LOBBY_GetOldUser_UserDesc( s32 desc_user )
{
	int i;
	s32 old_userid = DWC_LOBBY_INVALID_USER_ID;
	s64 old_time = 0x0fffffffffffffff;		// ���肦�Ȃ�����

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// ���[�UID�̎��Ԃ��擾���Ĉ�ԍŏ����炢��l�����
	for( i=0; i<PPW_LOBBY_MAX_PLAYER_NUM_MAIN; i++ ){
		{
			s32 userid;
			s64 time;

			userid = DWC_LOBBY_UserIDTbl_GetUserid( p_DWC_LOBBYLIB_WK, 
					i, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
			if( (userid != DWC_LOBBY_INVALID_USER_ID) && 
				(userid != desc_user) ){
				// ���Ԃ��擾
				DWC_LOBBY_GetLoginTime( userid, &time );
				if( old_time > time ){
					old_time = time;
					old_userid = userid;
				}
			}
		}
	}

	return old_userid;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l�����N�G�X�g
 *
 *	@param	channel			�`�����l��
 *
 *	@retval	TRUE			����
 *	@retval	FALSE			���s		�G���[���擾���ăG���[������
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_SUBCHAN_Login( DWC_LOBBY_SUBCHAN_TYPE channel )
{
	BOOL	retval;
	PPW_LOBBY_RESULT	result;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	
	DWC_LOBBY_PRINT( "dwc subchan login %d\n", DWC_LOBBY_GetChannelKind( channel ) );
	result = PPW_LobbyJoinSubChannelAsync( DWC_LOBBY_GetChannelKind( channel ) );
	// �G���[�`�F�b�N
	retval = DWC_LOBBY_CheckCommonErr( result );

	if( retval == TRUE ){
		p_DWC_LOBBYLIB_WK->subchan_type		= channel;

		// �T�u�`�����l�������`�F�b�N
		p_DWC_LOBBYLIB_WK->subchan_login_success = DWC_LOBBY_SUBCHAN_LOGIN_WAIT;
	}

	return retval;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l�����O�C�������҂�
 *
 *	@retval	DWC_LOBBY_SUBCHAN_LOGIN_WAIT,			// �T�u�`�����l���Ƀ��O�C���ҋ@��
 *	@retval	DWC_LOBBY_SUBCHAN_LOGIN_OK,				// �T�u�`�����l���Ƀ��O�C������
 *	@retval	DWC_LOBBY_SUBCHAN_LOGIN_NG,				// �T�u�`�����l���Ƀ��O�C�����s
 */
//-----------------------------------------------------------------------------
DWC_LOBBY_SUBCHAN_LOGIN_RESULT DWC_LOBBY_SUBCHAN_LoginWait( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	
	return p_DWC_LOBBYLIB_WK->subchan_login_success;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l�����O�A�E�g���N�G�X�g
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	���s		�G���[���擾���ăG���[������
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_SUBCHAN_Logout( void )
{
	PPW_LOBBY_RESULT	result;
	BOOL	retval;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	
	result = PPW_LobbyLeaveSubChannel();
	// �G���[�`�F�b�N
	retval = DWC_LOBBY_CheckCommonErr( result );

	if( retval == TRUE ){
		// ��Ԃ����O�A�E�g�����҂��ɂ���
		p_DWC_LOBBYLIB_WK->subchan_status = DWC_LOBBY_CHANNEL_STATE_LOGOUTWAIT;
	}
	return retval;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l�����O�A�E�g�����҂�
 *
 *	@retval	TRUE	���O�A�E�g����
 *	@retval	FALSE	���O�A�E�g�����̓��O�A�E�g�ȊO�̏�����
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_SUBCHAN_LogoutWait( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	
	if( p_DWC_LOBBYLIB_WK->subchan_status == DWC_LOBBY_CHANNEL_STATE_NONE ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l����Ԃ��擾����
 *
 *	@retval	DWC_LOBBY_CHANNEL_STATE_NONE,           // �`�����l���ɓ����Ă��Ȃ��B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_LOGINWAIT,		// �`�����l���ɓ������B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_CONNECT,		// �`�����l���ɓ����ς݁B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_LOGOUTWAIT,     // �`�����l���ɑގ����B
 *	@retval	DWC_LOBBY_CHANNEL_STATE_ERROR           // �`�����l����Ԃ��擾�ł��܂���ł����B
 */
//-----------------------------------------------------------------------------
DWC_LOBBY_CHANNEL_STATE DWC_LOBBY_SUBCHAN_GetState( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	return p_DWC_LOBBYLIB_WK->subchan_status;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l���ɓ����Ă���l�̐l���𒲂ׂē�����Ԃ��`�F�b�N
 *
 *	@param	channel		�`�����l���i���o�[
 *
 *	@retval	TRUE	�����
 *	@retval	FALSE	����Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_SUBCHAN_LoginCheck( DWC_LOBBY_SUBCHAN_TYPE channel )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	return DWC_LOBBY_UserIDTbl_SUBCHAN_CheckSysProfileLogin( p_DWC_LOBBYLIB_WK, channel );
}



//----------------------------------------------------------------------------
/**
 *	@brief	���C���`�����l���̃��[�UID�e�[�u�����擾����
 *
 *	@param	p_data	�f�[�^�i�[��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_GetUserIDTbl( DWC_LOBBY_CHANNEL_USERID* p_data )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	p_data->num		= p_DWC_LOBBYLIB_WK->useridtbl[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].num;
	p_data->cp_tbl	= p_DWC_LOBBYLIB_WK->useridtbl[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].p_tbl;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l���̃��[�UID�e�[�u�����擾����
 *
 *	@param	p_data	�f�[�^�i�[��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_SUBCHAN_GetUserIDTbl( DWC_LOBBY_CHANNEL_USERID* p_data )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	p_data->num		= p_DWC_LOBBYLIB_WK->useridtbl[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ].num;
	p_data->cp_tbl	= p_DWC_LOBBYLIB_WK->useridtbl[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ].p_tbl;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C���`�����l���Ȃ��̃��[�UID����񋓓���INDEX�����߂�
 *
 *	@param	userid	���[�UID
 *
 *	@retval	DWC_LOBBY_USERIDTBL_IDX_NONE	���Ȃ�
 *	@retval	����ȊO						���̐l��INDEX
 */
//-----------------------------------------------------------------------------
u32	DWC_LOBBY_GetUserIDIdx( s32 userid )
{
	return DWC_LOBBY_UserIDTbl_GetIdx( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l���Ȃ��̃��[�UID����񋓓���INDEX�����߂�
 *
 *	@param	userid	���[�UID
 *
 *	@retval	DWC_LOBBY_USERIDTBL_IDX_NONE	���Ȃ�
 *	@retval	����ȊO						���̐l��INDEX
 */
//-----------------------------------------------------------------------------
u32	DWC_LOBBY_SUBCHAN_GetUserIDIdx( s32 userid )
{
	return DWC_LOBBY_UserIDTbl_GetIdx( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_SUB );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C���`�����l���̃��[�UID���C���f�b�N�X����擾
 *
 *	@param	idx		�C���f�b�N�X
 *
 *	@retval	DWC_LOBBY_INVALID_USER_ID	���̃C���f�b�N�X�ɂ͒N�����Ȃ� 	
 *	@retval	���̑�						���[�UID
 */
//-----------------------------------------------------------------------------
s32	DWC_LOBBY_GetUserIdxID( u32 idx )
{
	return DWC_LOBBY_UserIDTbl_GetUserid( p_DWC_LOBBYLIB_WK, idx, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l���̃��[�UID���C���f�b�N����擾
 *
 *	@param	idx		�C���f�b�N�X
 *
 *	@retval	DWC_LOBBY_INVALID_USER_ID	���̃C���f�b�N�X�ɂ͒N�����Ȃ� 	
 *	@retval	���̑�						���[�UID
 */
//-----------------------------------------------------------------------------
s32	DWC_LOBBY_SUBCHAN_GetUserIdxID( u32 idx )
{
	return DWC_LOBBY_UserIDTbl_GetUserid( p_DWC_LOBBYLIB_WK, idx, DWC_LOBBY_LOCALCHANNEL_TYPE_SUB );
}

//----------------------------------------------------------------------------
/**
 *	@brief	GMT���Ԃ��擾����
 *
 *	@param	p_time	���Ԋi�[��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_GetTime( s64* p_time )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	*p_time = p_DWC_LOBBYLIB_WK->time.currentTime;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������쐬�������Ԃ��擾����
 *
 *	@param	p_time	���Ԋi�[��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_GetOpenTime( s64* p_time )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	*p_time = p_DWC_LOBBYLIB_WK->time.openedTime;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���������b�N���ꂽ���Ԃ��擾
 *
 *	@param	p_time	���Ԋi�[��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_GetLockTime( s64* p_time )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	*p_time = p_DWC_LOBBYLIB_WK->time.lockedTime;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃f�[�^���擾
 *
 *	@param	type	�擾�^�C�v
 *
 *	@return	�擾�^�C�v�ɑΉ������f�[�^
 *
 *	type
 *		DWC_LOBBY_ROOMDATA_LOCKTIME,		// �`�����l�������b�N���鎞��
 *		DWC_LOBBY_ROOMDATA_RANDOM,			// �T�[�o���琶������郉���_���l
 *		DWC_LOBBY_ROOMDATA_ROOMTYPE,		// �����̃^�C�v
 *		DWC_LOBBY_ROOMDATA_SEASON,			// �G�ߔԍ�
 *		DWC_LOBBY_ROOMDATA_ARCEUS,			// �A���Z�E�X�̕\���������邩 
 *		DWC_LOBBY_ROOMDATA_CLOSETIME,		// �����̃��b�N����N���[�Y�܂ł̎���
 *		DWC_LOBBY_ROOMDATA_FIRESTART,		// �����̃��b�N����ԉΊJ�n�܂ł̎���
 *		DWC_LOBBY_ROOMDATA_FIREEND,			// �����̃��b�N����ԉΏI���܂ł̎���
 *
 */
//-----------------------------------------------------------------------------
u32 DWC_LOBBY_GetRoomData( DWC_LOBBY_ROOMDATA_TYPE type )
{
	u32					data;
	PPW_LobbySchedule*	p_schedule;
	int i;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( (p_DWC_LOBBYLIB_WK->status == DWC_LOBBY_CHANNEL_STATE_CONNECT) || (p_DWC_LOBBYLIB_WK->status == DWC_LOBBY_CHANNEL_STATE_ERROR) );
	GF_ASSERT( type < DWC_LOBBY_ROOMDATA_MAX );

	// �X�P�W���[���o�b�t�@�擾
	p_schedule = (PPW_LobbySchedule*)p_DWC_LOBBYLIB_WK->schedulebuff;

	switch( type ){
	case DWC_LOBBY_ROOMDATA_LOCKTIME:		// �`�����l�������b�N���鎞��
		data = p_schedule->lockTime;
		break;
		
	case DWC_LOBBY_ROOMDATA_RANDOM:			// �T�[�o���琶������郉���_���l
		data = p_schedule->random;
		break;
		
	case DWC_LOBBY_ROOMDATA_ROOMTYPE:		// �����̃^�C�v
		data = p_schedule->roomType;
		break;
		
	case DWC_LOBBY_ROOMDATA_SEASON:			// �G�ߔԍ�
		data = p_schedule->season;
		break;

	case DWC_LOBBY_ROOMDATA_ARCEUS:			// �A���Z�E�X�\�����t���O
		if( p_schedule->roomFlag & PPW_LOBBY_ROOM_FLAG_ARCEUS ){
			data = TRUE;
		}else{
			data = FALSE;
		}
		break;

	case DWC_LOBBY_ROOMDATA_CLOSETIME:	// �����̃��b�N����N���[�Y�܂ł̎���
		data = DWC_LOBBY_SCHEDULE_GetEventTime( p_schedule, PPW_LOBBY_TIME_EVENT_CLOSE );
		break;

	case DWC_LOBBY_ROOMDATA_FIRESTART:		// �����̃��b�N����ԉΊJ�n�܂ł̎���
		data = DWC_LOBBY_SCHEDULE_GetEventTime( p_schedule, PPW_LOBBY_TIME_EVENT_FIRE_WORKS_START );
		break;

	case DWC_LOBBY_ROOMDATA_FIREEND:		// �����̃��b�N����ԉΏI���܂ł̎���
		data = DWC_LOBBY_SCHEDULE_GetEventTime( p_schedule, PPW_LOBBY_TIME_EVENT_FIRE_WORKS_END );
		break;
	}

	return data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���f�[�^���T�[�o�ɃA�b�v
 *
 *	@retval	TRUE	�T�[�o�ɃA�b�v
 *	@retval	FALSE	�T�[�o�ɃA�b�v���Ȃ�����
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_WLDDATA_Send( void )
{
	BOOL result;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	
	// �ގ�OK�Ȃ̂ŁA�ގ��ҍ��f�[�^��o�^����
	result = DWC_LOBBY_WLDDATA_SetMyData( p_DWC_LOBBYLIB_WK );
	if( result ){
		p_DWC_LOBBYLIB_WK->wlddata_send = TRUE;
	}
	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���f�[�^�̑��M�����҂�
 *
 *	@retval	TRUE	���M����
 *	@retval	FALSE	���M�����傤���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_WLDDATA_SendWait( void )
{
	if( p_DWC_LOBBYLIB_WK->wlddata_send == FALSE ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ގ���	��ID�擾
 *
 *	@param	index	�C���f�b�N�X
 *	
 *	@return	��ID
 */
//-----------------------------------------------------------------------------
u16 DWC_LOBBY_WLDDATA_GetNation( u8 index )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( index < DWC_WLDDATA_MAX );

	return p_DWC_LOBBYLIB_WK->wfldata[ index ].nation;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ގ���	�n��ID�擾
 *
 *	@param	index	�C���f�b�N�X
 *	
 *	@return	�n��ID
 */
//-----------------------------------------------------------------------------
u8 DWC_LOBBY_WLDDATA_GetArea( u8 index )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( index < DWC_WLDDATA_MAX );

	return p_DWC_LOBBYLIB_WK->wfldata[ index ].area;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ގ��ҍ��f�[�^��index�̃o�b�t�@�ɂ��邩�`�F�b�N
 *
 *	@param	index	�C���f�b�N�X
 *
 *	@retval	TRUE	�f�[�^������
 *	@retval	FALSE	�f�[�^���Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_WLDDATA_GetDataFlag( u8 index )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( index < DWC_WLDDATA_MAX );

	return p_DWC_LOBBYLIB_WK->wfldata[ index ].flag;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���C���`�����l��	���b�Z�[�W�R�}���h��ݒ�
 *
 *	@param	cp_tbl		�R�}���h�e�[�u��
 *	@param	tblnum		�e�[�u����
 *	@param	p_work		�R�[���o�b�N�ɓn�����[�N
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_SetMsgCmd( const DWC_LOBBY_MSGCOMMAND* cp_tbl, u32 tblnum, void* p_work )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].cp_msgtbl = cp_tbl;
	p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].msgtblnum = tblnum;
	p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].p_work	= p_work;
}
//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l��	���b�Z�[�W�R�}���h��ݒ�
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_SUBCHAN_SetMsgCmd( const DWC_LOBBY_MSGCOMMAND* cp_tbl, u32 tblnum, void* p_work )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ].cp_msgtbl = cp_tbl;
	p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ].msgtblnum = tblnum;
	p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ].p_work	= p_work;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�R�}���h���N���A
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_CleanMsgCmd( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	memset( &p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ], 0, sizeof(DWC_LOBBY_MSGCMD) );
}
void DWC_LOBBY_SUBCHAN_CleanMsgCmd( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	memset( &p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ], 0, sizeof(DWC_LOBBY_MSGCMD) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C���`�����l��	�S�̂Ƀ��b�Z�[�W�̑��M
 *
 *	@param	type		���b�Z�[�W�^�C�v
 *	@param	cp_data		�f�[�^
 *	@param	size		�T�C�Y
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_SendChannelMsg( u32 type, const void* cp_data, u32 size )
{
	PPW_LOBBY_RESULT result;
	BOOL retval;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	
	// type�`�F�b�N
	GF_ASSERT( p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].msgtblnum > type );
	// �T�C�Y�`�F�b�N
	GF_ASSERT( p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].cp_msgtbl[ type ].size == size );

	// ���M
	result = PPW_LobbySendChannelBinaryMessage( PPW_LOBBY_CHANNEL_KIND_MAIN, type, cp_data, size );

	retval = DWC_LOBBY_CheckCommonErr( result );
	GF_ASSERT( retval == TRUE );	// ���s���������
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C���`�����l��	�v���C���[�Ƀ��b�Z�[�W�̑��M
 *
 *	@param	type		���b�Z�[�W�^�C�v
 *	@param	userid		���[�UID
 *	@param	cp_data		�f�[�^
 *	@param	size		�T�C�Y
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_SendPlayerMsg( u32 type, s32 userid, const void* cp_data, u32 size )
{
	PPW_LOBBY_RESULT result;
	BOOL retval;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	
	// type�`�F�b�N
	GF_ASSERT( p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].msgtblnum > type );
	// �T�C�Y�`�F�b�N
	GF_ASSERT( p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].cp_msgtbl[ type ].size == size );

	// ���M
	result = PPW_LobbySendPlayerBinaryMessage( userid, type, cp_data, size );

	retval = DWC_LOBBY_CheckCommonErr( result );
	GF_ASSERT( retval == TRUE );	// ���s���������
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l��		�S�̂Ƀ��b�Z�[�W���M
 *
 *	@param	type		���b�Z�[�W�^�C�v
 *	@param	cp_data		�f�[�^
 *	@param	size		�T�C�Y
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_SUBCHAN_SendMsg( u32 type, const void* cp_data, u32 size )
{
	PPW_LOBBY_RESULT result;
	BOOL retval;
	u32 channel_kind;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	
	// type�`�F�b�N
	GF_ASSERT( p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ].msgtblnum > type );
	// �T�C�Y�`�F�b�N
	GF_ASSERT( p_DWC_LOBBYLIB_WK->msgcmd[ DWC_LOBBY_LOCALCHANNEL_TYPE_SUB ].cp_msgtbl[ type ].size == size );
	// �T�u�`�����l���ɑ����Ă��邩
	GF_ASSERT( p_DWC_LOBBYLIB_WK->subchan_type != DWC_LOBBY_SUBCHAN_NONE );

	// ���M
	channel_kind = DWC_LOBBY_GetChannelKind( p_DWC_LOBBYLIB_WK->subchan_type );
	result = PPW_LobbySendChannelBinaryMessage( channel_kind, type, cp_data, size );

	retval = DWC_LOBBY_CheckCommonErr( result );
	GF_ASSERT( retval == TRUE );	// ���s���������
}


//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����W�̊J�n
 *
 *	@param	type		�~�j�Q�[���^�C�v
 *	@param	maxnum		�ő�ڑ���
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	���łɒN������W���Ă�E���������łɉ����̕�W�ɎQ�����Ă�
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_MG_StartRecruit( DWC_LOBBY_MG_TYPE type, u32 maxnum )
{
	PPW_LOBBY_RESULT result;
	BOOL check;
	
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// ���̃~�j�Q�[������W���łȂ����`�F�b�N
	if( DWC_LOBBY_MG_Check( p_DWC_LOBBYLIB_WK, type ) == TRUE ){
		return FALSE;
	}

	// ���łɃG���g���[���Ă��Ȃ����`�F�b�N
	if( p_DWC_LOBBYLIB_WK->mg_myentry != DWC_LOBBY_MG_NUM ){
		return FALSE;
	}

	// ��W�J�n
	p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.gameKind		= type;
	p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.maxNum		= maxnum;
	p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.currentNum	= 1;
	p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.paramVal		= DWC_LOBBY_MG_ST_RECRUIT;
	DWC_LOBBY_MG_SetRecruitStartTime( &p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data, 
			&p_DWC_LOBBYLIB_WK->time.currentTime );

	result = PPW_LobbyStartRecruit( &p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data );

	check = DWC_LOBBY_CheckCommonErr( result );
	GF_ASSERT( check );	// ��W�Ɏ��s

	// ��W�J�n
	p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_flag		= TRUE;
	p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_parent	= PPW_LobbyGetMyUserId();
	p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_timelimit	= DWC_LOBBY_MG_TIMELIMIT * DWC_LOBBY_MG_TIMELIMIT_SEC;
	p_DWC_LOBBYLIB_WK->mg_myentry					= type;
	p_DWC_LOBBYLIB_WK->mg_myparent					= TRUE;
	p_DWC_LOBBYLIB_WK->mg_myend						= FALSE;


//----#if TESTOHNO
    DWC_LOBBY_PRINT("match start %s\ntimelimit = %d\n",p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.matchMakingString, p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_timelimit / DWC_LOBBY_MG_TIMELIMIT_SEC);

    
	// �}�b�`���O�w�W���擾���ă}�b�`���O�J�n
    if(mydwc_startmatch( (u8*)p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.matchMakingString, 
				maxnum, TRUE, 
				(p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_timelimit / DWC_LOBBY_MG_TIMELIMIT_SEC) )){

    }

	mydwc_setConnectCallback( DWC_LOBBY_MG_ConnectCallBack, p_DWC_LOBBYLIB_WK );
	mydwc_setDisconnectCallback( DWC_LOBBY_MG_DisConnectCallBack, p_DWC_LOBBYLIB_WK );
//----#endif	// TESTOHNO

	return TRUE;
}  

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����W�̏I��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_MG_EndRecruit( void )
{
	PPW_LOBBY_RESULT result;
	BOOL check;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// �����ɃG���g���[���Ă邩�`�F�b�N
	if( p_DWC_LOBBYLIB_WK->mg_myentry == DWC_LOBBY_MG_NUM ){
		return ;
	}

	DWC_LOBBY_PRINT( "dwc lobby end recruit\n" );

	// ��������W�����������`�F�b�N
	if( p_DWC_LOBBYLIB_WK->mg_myparent == FALSE ){
		return ;
	}

	// �~�j�Q�[����W�̏I��
	result = PPW_LobbyStopRecruit();
	check = DWC_LOBBY_CheckCommonErr( result );
	GF_ASSERT( check );	// ��W�Ɏ��s

	// ��W�������Z�b�g
	check = DWC_LOBBY_MG_ResetData( p_DWC_LOBBYLIB_WK, p_DWC_LOBBYLIB_WK->mg_myentry, PPW_LobbyGetMyUserId() );
	GF_ASSERT( check );

	// �e�ł͂Ȃ��Ȃ����̂ŁA�e�łȂ����Ƃɂ���
	p_DWC_LOBBYLIB_WK->mg_myparent = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�Ȃ��W���Ă���~�j�Q�[�����J�n����
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_MG_StartGame( void )
{
	PPW_LOBBY_RESULT result;
	BOOL check;

	GF_ASSERT( p_DWC_LOBBYLIB_WK );
	
	// �e���`�F�b�N
	if( p_DWC_LOBBYLIB_WK->mg_myparent == FALSE ){
		return ;
	}

	// �~�j�Q�[���̕�W�����Q�[���J�n�ɂ���
	p_DWC_LOBBYLIB_WK->mg_data[ p_DWC_LOBBYLIB_WK->mg_myentry ].mg_data.paramVal = DWC_LOBBY_MG_ST_GAMEING;

	result = PPW_LobbyUpdateRecruitInfo( &p_DWC_LOBBYLIB_WK->mg_data[ p_DWC_LOBBYLIB_WK->mg_myentry ].mg_data );
	check = DWC_LOBBY_CheckCommonErr( result );
	GF_ASSERT( check );	// �A�b�v�f�[�g�Ɏ��s
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�����Ă���~�j�Q�[�����J�n���邩�`�F�b�N
 *
 *	@retval	TRUE	�Q�����Ă���~�j�Q�[�����J�n����
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_MG_CheckStartGame( void )
{

	GF_ASSERT( p_DWC_LOBBYLIB_WK );
	
	// �����������ɎQ�������H
	if( p_DWC_LOBBYLIB_WK->mg_myentry == DWC_LOBBY_MG_NUM ){
		return FALSE;
	}
	
	// �Q�[���J�n�ɂȂ������`�F�b�N
	if( p_DWC_LOBBYLIB_WK->mg_data[ p_DWC_LOBBYLIB_WK->mg_myentry ].mg_data.paramVal == DWC_LOBBY_MG_ST_GAMEING ){
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���̕�W���I������K�v�����邩�`�F�b�N����
 *
 *	@retval	TRUE	��W���I������K�v������
 *	@retval	FALSE	��W���I������K�v�͂Ȃ�
 *
 *	��W�Ɏ��s�����Ƃ��@TRUE�ɂȂ�܂�
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_MG_ForceEnd( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	return p_DWC_LOBBYLIB_WK->mg_myend;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[������W�����`�F�b�N
 *
 *	@param	type	�~�j�Q�[���^�C�v
 *
 *	@retval	TRUE	type�̃~�j�Q�[������W��
 *	@retval	FALSE	type�̃~�j�Q�[���͕�W���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_MG_CheckRecruit( DWC_LOBBY_MG_TYPE type )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	return DWC_LOBBY_MG_Check( p_DWC_LOBBYLIB_WK, type );
}

//----------------------------------------------------------------------------
/**
 *	@brief	type�̃~�j�Q�[���ɃG���g���[�ł��邩�`�F�b�N
 *
 *	@param	type	�~�j�Q�[���^�C�v
 *
 *	@retval	TRUE	�G���g���[�\
 *	@retval	FALSE	�G���g���[�s�\
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_MG_CheckEntryOk( DWC_LOBBY_MG_TYPE type )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK );
	
	// ��W���������Ă��邩�H
	if( DWC_LOBBY_MG_CheckRecruit( type ) == FALSE ){
		return FALSE;
	}
	// ��W��Ԃ�RECRUIT���H
	if( p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.paramVal == DWC_LOBBY_MG_ST_RECRUIT ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̃~�j�Q�[���ɂ��Ɖ��l�Q���ł��邩�`�F�b�N
 *
 *	@param	type	�~�j�Q�[���^�C�v
 *
 *	@return	�l��
 */
//-----------------------------------------------------------------------------
u32 DWC_LOBBY_MG_GetRest( DWC_LOBBY_MG_TYPE type )
{
	const PPW_LobbyRecruitInfo* cp_info; 
	
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( type < DWC_LOBBY_MG_NUM );
	
	// ��W���Ă�l�����Ȃ��Ƃ��́A�O��Ԃ�
	if( DWC_LOBBY_MG_Check( p_DWC_LOBBYLIB_WK, type ) == FALSE ){
		return 0;
	}

	cp_info = DWC_LOBBY_MG_GetData( p_DWC_LOBBYLIB_WK, type );
	return ( cp_info->maxNum - cp_info->currentNum );
}

//----------------------------------------------------------------------------
/**
 *	@brief	type�̕�W�ɉ��l�Q�����Ă��邩�`�F�b�N
 *
 *	@param	type	�^�C�v
 *	
 *	@return	��W�ɎQ�����Ă���l��
 */
//-----------------------------------------------------------------------------
u32 DWC_LOBBY_MG_GetEntryNum( DWC_LOBBY_MG_TYPE type )
{
	const PPW_LobbyRecruitInfo* cp_info; 
	
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( type < DWC_LOBBY_MG_NUM );
	
	// ��W���Ă�l�����Ȃ��Ƃ��́A�O��Ԃ�
	if( DWC_LOBBY_MG_Check( p_DWC_LOBBYLIB_WK, type ) == FALSE ){
		return 0;
	}

	cp_info = DWC_LOBBY_MG_GetData( p_DWC_LOBBYLIB_WK, type );
	return cp_info->currentNum;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���̕�W�ɂ��񂩂���
 *
 *	@param	type	�~�j�Q�[���^�C�v
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	��W���Ă�l�����Ȃ��E���������łɉ����ɎQ�����Ă�
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_MG_Entry( DWC_LOBBY_MG_TYPE type )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// ��W�҂����邩�`�F�b�N
	if( DWC_LOBBY_MG_Check( p_DWC_LOBBYLIB_WK, type ) == FALSE ){
		return FALSE;
	}
	// ���łɉ����ɎQ�����ĂȂ����`�F�b�N
	if( p_DWC_LOBBYLIB_WK->mg_myentry != DWC_LOBBY_MG_NUM ){
		return FALSE;
	}
	// ���łɂ��̕�W�́A�Q�[�������ɂ͂����Ă��Ȃ����H
	if( p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.paramVal == DWC_LOBBY_MG_ST_GAMEING ){
		return FALSE;
	}
	
	// ��W�ɎQ��
	p_DWC_LOBBYLIB_WK->mg_myentry = type;

	// �}�b�`���O�w�W����ʐM�J�n
//----#if TESTOHNO
		
    DWC_LOBBY_PRINT("match entry %s\ntimelimit = %d\n",p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.matchMakingString, p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_timelimit / DWC_LOBBY_MG_TIMELIMIT_SEC);
    if(mydwc_startmatch( (u8*)p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.matchMakingString,
                         p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_data.maxNum, FALSE,
						 (p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_timelimit / DWC_LOBBY_MG_TIMELIMIT_SEC))){
    }

	mydwc_setConnectCallback( DWC_LOBBY_MG_ConnectCallBack, p_DWC_LOBBYLIB_WK );
	mydwc_setDisconnectCallback( DWC_LOBBY_MG_DisConnectCallBack, p_DWC_LOBBYLIB_WK );
//----#endif //TESTOHNO

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���J�n�܂ł̃^�C�����~�b�g���擾
 *
 *	@param	type		�~�j�Q�[���^�C�v
 *
 *	@return	�^�C�����~�b�g�i�R�O�V���N�P�ʁj	�܂��N����W���ĂȂ��ꍇ�͍ő�l
 */
//-----------------------------------------------------------------------------
s32 DWC_LOBBY_MG_GetTimeLimit( DWC_LOBBY_MG_TYPE type )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( type < DWC_LOBBY_MG_NUM );
	
	// ��W���Ă�l�����Ȃ��Ƃ��́ADWC_LOBBY_MG_TIMELIMIT��Ԃ�
	if( DWC_LOBBY_MG_Check( p_DWC_LOBBYLIB_WK, type ) == FALSE ){
		return DWC_LOBBY_MG_TIMELIMIT * DWC_LOBBY_MG_TIMELIMIT_SEC;
	}

	return p_DWC_LOBBYLIB_WK->mg_data[ type ].mg_timelimit;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[���G���g���[�҂�
 *
 *	@retval	DWC_LOBBY_MG_ENTRYNONE,			// �������Ă��Ȃ�
 *	@retval	DWC_LOBBY_MG_ENTRYWAIT,			// �G���g���[��
 *	@retval	DWC_LOBBY_MG_ENTRYOK,			// �G���g���[����
 *	@retval	DWC_LOBBY_MG_ENTRYNG,			// �G���g���[���s
 */
//-----------------------------------------------------------------------------
DWC_LOBBY_MG_ENTRYRET DWC_LOBBY_MG_EntryWait( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// �G���g���[�����`�F�b�N
	if( p_DWC_LOBBYLIB_WK->mg_myentry == DWC_LOBBY_MG_NUM ){
		return DWC_LOBBY_MG_ENTRYNONE;
	}

	// �G���g���[������҂�
    if(mydwc_getaid() == MYDWC_NONE_AID){
        return DWC_LOBBY_MG_ENTRYWAIT;
    }
	
	return DWC_LOBBY_MG_ENTRYOK;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ݐڑ����̃~�j�Q�[�����擾
 *
 *	@retval	DWC_LOBBY_MG_BALLSLOW,			// �ʂ���
 *	@retval	DWC_LOBBY_MG_BALANCEBALL,		// �ʏ��
 *	@retval	DWC_LOBBY_MG_BALLOON,			// ���D���
 *	@retval	DWC_LOBBY_MG_NUM,				// �Q�����ĂȂ�
 */
//-----------------------------------------------------------------------------
DWC_LOBBY_MG_TYPE DWC_LOBBY_MG_GetConnectType( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	return p_DWC_LOBBYLIB_WK->mg_myentry;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ڑ����̃~�j�Q�[���̐e���[�UID
 *
 *	@retval DWC_LOBBY_INVALID_USER_ID	�ڑ����ĂȂ�
 *	@retval ����ȊO					�e�̃��[�UID
 */
//-----------------------------------------------------------------------------
s32 DWC_LOBBY_MG_GetParentUserID( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	if( p_DWC_LOBBYLIB_WK->mg_myentry == DWC_LOBBY_MG_NUM ){
		return DWC_LOBBY_INVALID_USER_ID;
	}
	return p_DWC_LOBBYLIB_WK->mg_data[ p_DWC_LOBBYLIB_WK->mg_myentry ].mg_parent;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������e���`�F�b�N
 *
 *	@retval	TRUE	�e
 *	@retval	FALSE	�q
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_MG_MyParent( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	return p_DWC_LOBBYLIB_WK->mg_myparent;
}

//----------------------------------------------------------------------------
/**
 *	@brief	����P2P�ڑ������`�F�b�N
 *
 *	@retval	TRUE	�ڑ���
 *	@retval	FALSE	�ڑ����ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_MG_CheckConnect( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	// Lobby�V�X�e���Ȃ��Őڑ������`�F�b�N
	if( p_DWC_LOBBYLIB_WK->mg_myentry == DWC_LOBBY_MG_NUM ){
		return FALSE;
	}

	// �ڑ������`�F�b�N
    if(mydwc_getaid() != MYDWC_NONE_AID){
        return TRUE;
    }
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP���ǂ������ׂ�
 *
 *	@param	userid	���[�UID
 *
 *	@retval	TRUE	VIP
 *	@retval	FALSE	�ʏ�̐l
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_VIP_CheckVip( s32 userid )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	return DWC_LOBBY_VIPDATA_CheckVip( &p_DWC_LOBBYLIB_WK->vipdata, userid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP�����t�L�[�擾
 *
 *	@param	userid	���[�UID
 *
 *	@retval	DWC_LOBBY_VIP_KEYNONE	�������Ƃ΁@�Ȃ�
 *	@retval	���̑�					�������Ƃ΁@�L�[�l
 */
//-----------------------------------------------------------------------------
s32 DWC_LOBBY_VIP_GetAikotobaKey( s32 userid )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	return DWC_LOBBY_VIPDATA_GetAikotoba( &p_DWC_LOBBYLIB_WK->vipdata, userid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g���[�f�[�^���M
 *
 *	@param	select	�I��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_ANKETO_SubMit( s32 select )
{
	u32 result;
	BOOL retval;
	
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( select < PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM );
	GF_ASSERT( p_DWC_LOBBYLIB_WK->anketo.submit_state != DWC_LOBBY_ANKETO_STATE_SENDING );


	result = PPW_LobbySubmitQuestionnaire( select );
	retval = DWC_LOBBY_CheckCommonErr( result );
	GF_ASSERT( retval == TRUE );	// ���s���������

	p_DWC_LOBBYLIB_WK->anketo.submit_state = DWC_LOBBY_ANKETO_STATE_SENDING;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g���[�f�[�^���M����
 *
 *	@retval	DWC_LOBBY_ANKETO_STATE_NONE,	// ���M�O	
 *	@retval	DWC_LOBBY_ANKETO_STATE_SENDING,	// ���M��
 *	@retval	DWC_LOBBY_ANKETO_STATE_SENDOK,	// ���M����
 *	@retval	DWC_LOBBY_ANKETO_STATE_SENDNG,	// ���M���s
 */
//-----------------------------------------------------------------------------
DWC_LOBBY_ANKETO_STATE DWC_LOBBY_ANKETO_WaitSubMit( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	return p_DWC_LOBBYLIB_WK->anketo.submit_state;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�f�[�^�擾
 *
 *	@param	type	�^�C�v
 *
 *	@return	�f�[�^
 *
 *	type
 *		DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_SERIAL,	// ���̎���ʂ��ԍ�
 *		DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_NO,		// ���̎���ԍ�
 *		DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_SPECIAL,	// ���̎��₪�X�y�V������
 *		DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_NINI,	// ���̎��₪�C�ӎ��₩
 *		DWC_LOBBY_ANKETO_DATA_LAST_QUESTION_SERIAL,	// �O��̎���ʂ��ԍ�
 *		DWC_LOBBY_ANKETO_DATA_LAST_QUESTION_NO,		// �O��̎���ԍ�
 *		DWC_LOBBY_ANKETO_DATA_LAST_QUESTION_SPECIAL,// �O��̎��₪�X�y�V������
 *		DWC_LOBBY_ANKETO_DATA_LAST_QUESTION_NINI,	// ���̎��₪�C�ӎ��₩
 *		DWC_LOBBY_ANKETO_DATA_LAST_RESULT_A,	// �O��̎��₪����A
 *		DWC_LOBBY_ANKETO_DATA_LAST_RESULT_B,	// �O��̎��₪����B
 *		DWC_LOBBY_ANKETO_DATA_LAST_RESULT_C,	// �O��̎��₪����C
 */
//-----------------------------------------------------------------------------
s32 DWC_LOBBY_ANKETO_GetData( DWC_LOBBY_ANKETO_DATA type )
{
	s32 data;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( type < DWC_LOBBY_ANKETO_DATA_MAX );

	switch( type ){
	// ���̎���ʂ��ԍ�
	case DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_SERIAL:	
		data = p_DWC_LOBBYLIB_WK->anketo.question.currentQuestionnaireRecord.questionSerialNo;
		break;
	// ���̎���ԍ�
	case DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_NO:		
		data = p_DWC_LOBBYLIB_WK->anketo.question.currentQuestionnaireRecord.questionNo;
		break;
	// ���̎��₪�X�y�V������
	case DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_SPECIAL:
		data = p_DWC_LOBBYLIB_WK->anketo.question.currentQuestionnaireRecord.isSpecialWeek;
		break;
	// ���̎��₪�C�ӎ��₩
	case DWC_LOBBY_ANKETO_DATA_NOW_QUESTION_NINI:
		if( p_DWC_LOBBYLIB_WK->anketo.question.currentQuestionnaireRecord.questionNo >= PPW_LOBBY_FREE_QUESTION_START_NO ){
			data = TRUE;
		}else{
			data = FALSE;
		}
		break;

	// �O��̎���ʂ��ԍ�
	case DWC_LOBBY_ANKETO_DATA_LAST_QUESTION_SERIAL:
		data = p_DWC_LOBBYLIB_WK->anketo.question.lastQuestionnaireRecord.questionSerialNo;
		break;
	// �O��̎���ԍ�
	case DWC_LOBBY_ANKETO_DATA_LAST_QUESTION_NO:	
		data = p_DWC_LOBBYLIB_WK->anketo.question.lastQuestionnaireRecord.questionNo;
		break;
	// �O��̎��₪�X�y�V������
	case DWC_LOBBY_ANKETO_DATA_LAST_QUESTION_SPECIAL:
		data = p_DWC_LOBBYLIB_WK->anketo.question.lastQuestionnaireRecord.isSpecialWeek;
		break;
	// �O��̎��₪�C�ӎ��₩
	case DWC_LOBBY_ANKETO_DATA_LAST_QUESTION_NINI:
		if( p_DWC_LOBBYLIB_WK->anketo.question.lastQuestionnaireRecord.questionNo >= PPW_LOBBY_FREE_QUESTION_START_NO ){
			data = TRUE;
		}else{
			data = FALSE;
		}
		break;

	// �O��̎��₪����A
	// �O��̎��₪����B
	// �O��̎��₪����C
	case DWC_LOBBY_ANKETO_DATA_LAST_RESULT_A:
	case DWC_LOBBY_ANKETO_DATA_LAST_RESULT_B:	
	case DWC_LOBBY_ANKETO_DATA_LAST_RESULT_C:	
		data = p_DWC_LOBBYLIB_WK->anketo.question.lastMultiResult[ type - DWC_LOBBY_ANKETO_DATA_LAST_RESULT_A ];
		break;
		
	default:
		GF_ASSERT(0);
		data = 0;
		break;
	}
	
	return data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g���b�Z�[�W�擾
 *
 *	@param	type	�^�C�v
 *	
 *	@retval		���b�Z�[�W�f�[�^	�iSTRCODE�j
 *	@retval		NULL		�Ȃ�
 *
 *	type
 *		DWC_LOBBY_ANKETO_MESSAGE_NOW_QUESTION,	// ���̖��
 *		DWC_LOBBY_ANKETO_MESSAGE_NOW_ANSWER_00,	// ���̉�A
 *		DWC_LOBBY_ANKETO_MESSAGE_NOW_ANSWER_01,	// ���̉�B
 *		DWC_LOBBY_ANKETO_MESSAGE_NOW_ANSWER_02,	// ���̉�C
 *		DWC_LOBBY_ANKETO_MESSAGE_LAST_QUESTION,		// �O��̖��
 *		DWC_LOBBY_ANKETO_MESSAGE_LAST_ANSWER_00,	// �O��̉�A
 *		DWC_LOBBY_ANKETO_MESSAGE_LAST_ANSWER_01,	// �O��̉�B
 *		DWC_LOBBY_ANKETO_MESSAGE_LAST_ANSWER_02,	// �O��̉�C
 */
//-----------------------------------------------------------------------------
u16* DWC_LOBBY_ANKETO_GetMessage( DWC_LOBBY_ANKETO_MESSAGE type )
{
	u16* p_str = NULL;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( type < DWC_LOBBY_ANKETO_MESSAGE_MAX );

	switch( type ){
	// ���̖��
	case DWC_LOBBY_ANKETO_MESSAGE_NOW_QUESTION:	
		p_str = p_DWC_LOBBYLIB_WK->anketo.question.currentQuestionnaireRecord.questionSentence;
		break;
	// ���̉�A
	// ���̉�B
	// ���̉�C
	case DWC_LOBBY_ANKETO_MESSAGE_NOW_ANSWER_00:	
	case DWC_LOBBY_ANKETO_MESSAGE_NOW_ANSWER_01:	
	case DWC_LOBBY_ANKETO_MESSAGE_NOW_ANSWER_02:	
		p_str = p_DWC_LOBBYLIB_WK->anketo.question.currentQuestionnaireRecord.answer[ type - DWC_LOBBY_ANKETO_MESSAGE_NOW_ANSWER_00 ];
		break;

	// �O��̖��
	case DWC_LOBBY_ANKETO_MESSAGE_LAST_QUESTION:		
		p_str = p_DWC_LOBBYLIB_WK->anketo.question.lastQuestionnaireRecord.questionSentence;
		break;
	// �O��̉�A
	// �O��̉�B
	// �O��̉�C
	case DWC_LOBBY_ANKETO_MESSAGE_LAST_ANSWER_00:	
	case DWC_LOBBY_ANKETO_MESSAGE_LAST_ANSWER_01:	
	case DWC_LOBBY_ANKETO_MESSAGE_LAST_ANSWER_02:	
		p_str = p_DWC_LOBBYLIB_WK->anketo.question.lastQuestionnaireRecord.answer[ type - DWC_LOBBY_ANKETO_MESSAGE_LAST_ANSWER_00 ];
		break;

	default:
		GF_ASSERT(0);
		p_str = NULL;
		break;
	}

	return p_str;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����W�v�����`�F�b�N
 *
 *	@param	type		����^�C�v
 *	@param	lang		����^�C�v
 *
 *	@retval	TRUE		�����̌���Ɠ����W�v
 *	@retval	FALSE		�����̌���Ɠ����W�v����Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL DWC_LOBBY_ANKETO_GetLanguageSummarize( DWC_LOBBY_ANKETO_LANGUAGE_DATA type, u32 lang )
{
	BOOL ret;
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );
	GF_ASSERT( type < DWC_LOBBY_ANKETO_LANGUAGE_MAX );
	GF_ASSERT( lang < DWC_LOBBY_ANKETO_LANG_SUMMARIZE_FLAG_BUFF_NUM );

	if( type >= DWC_LOBBY_ANKETO_LANGUAGE_MAX ){
		return FALSE;
	}

	if( lang >= DWC_LOBBY_ANKETO_LANG_SUMMARIZE_FLAG_BUFF_NUM ){
		return FALSE;
	}

	if( type == DWC_LOBBY_ANKETO_LANGUAGE_NOW ){
		ret = p_DWC_LOBBYLIB_WK->anketo.question.currentQuestionnaireRecord.multiLanguageSummarizeFlags[ lang ];
	}else{
		ret = p_DWC_LOBBYLIB_WK->anketo.question.lastQuestionnaireRecord.multiLanguageSummarizeFlags[ lang ];
	}

	return ret;
}



//----------------------------------------------------------------------------
/**
 *	@brief	P2P�ڑ��I��
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_MG_EndConnect( void )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	mydwc_setConnectCallback( NULL, NULL );
	mydwc_setDisconnectCallback( NULL, NULL );

	// Lobby����Ԃ��ؒf��
	p_DWC_LOBBYLIB_WK->mg_myentry	= DWC_LOBBY_MG_NUM;
	p_DWC_LOBBYLIB_WK->mg_myparent	= FALSE;
	p_DWC_LOBBYLIB_WK->mg_myend		= FALSE;
}


#ifdef PM_DEBUG
// �f�o�b�N�p�X�P�W���[���ݒ�
void DWC_LOBBY_DEBUG_SetRoomData( u32 locktime, u32 random, u8 roomtype, u8 season )
{
	PPW_LobbySchedule* p_schedule = (PPW_LobbySchedule*)p_DWC_LOBBYLIB_WK->schedulebuff;
	PPW_LOBBY_RESULT result;

	GF_ASSERT( p_DWC_LOBBYLIB_WK != NULL );

	if( D_Tomoya_WiFiLobby_DebugStart == FALSE ){
		return ;
	}
	
	p_schedule->roomType = roomtype;
	p_schedule->season = season;
	p_schedule->random = random;

	OS_TPrintf( "event fast\n" );

	
	if( D_Tomoya_WiFiLobby_LockTime_Long == TRUE ){
		p_schedule->lockTime = 0xffffffff;	// ���b�N����
	}else{
		p_schedule->lockTime = 30;
	}


	p_schedule->scheduleRecordNum = 18;


	p_schedule->scheduleRecords[0].time = 0;
	p_schedule->scheduleRecords[0].event = PPW_LOBBY_TIME_EVENT_LOCK;
	p_schedule->scheduleRecords[1].time = 1*10;
	p_schedule->scheduleRecords[1].event = PPW_LOBBY_TIME_EVENT_NEON_A0;
	p_schedule->scheduleRecords[2].time = 1*10;
	p_schedule->scheduleRecords[2].event = PPW_LOBBY_TIME_EVENT_NEON_B0;
	p_schedule->scheduleRecords[3].time = 2 * 10;
	p_schedule->scheduleRecords[3].event = PPW_LOBBY_TIME_EVENT_NEON_A1;
	p_schedule->scheduleRecords[4].time = 3 * 10;
	p_schedule->scheduleRecords[4].event = PPW_LOBBY_TIME_EVENT_NEON_B1;
	p_schedule->scheduleRecords[5].time = 3 * 10;
	p_schedule->scheduleRecords[5].event = PPW_LOBBY_TIME_EVENT_NEON_A2;
	p_schedule->scheduleRecords[6].time = 4 * 10;
	p_schedule->scheduleRecords[6].event = PPW_LOBBY_TIME_EVENT_NEON_B2;
	p_schedule->scheduleRecords[7].time = 4 * 10;
	p_schedule->scheduleRecords[7].event = PPW_LOBBY_TIME_EVENT_NEON_A3;
	p_schedule->scheduleRecords[8].time = 5 * 10;
	p_schedule->scheduleRecords[8].event = PPW_LOBBY_TIME_EVENT_NEON_B1;
	p_schedule->scheduleRecords[9].time = 5 * 10;
	p_schedule->scheduleRecords[9].event = PPW_LOBBY_TIME_EVENT_NEON_A4;
	p_schedule->scheduleRecords[10].time = 6 * 10;
	p_schedule->scheduleRecords[10].event = PPW_LOBBY_TIME_EVENT_NEON_A5;
	p_schedule->scheduleRecords[11].time = 6 * 10;
	p_schedule->scheduleRecords[11].event = PPW_LOBBY_TIME_EVENT_NEON_B2;

	p_schedule->scheduleRecords[12].time = 7 * 60;
	p_schedule->scheduleRecords[12].event = PPW_LOBBY_TIME_EVENT_NEON_B3;

	p_schedule->scheduleRecords[13].time = 2 * 60;



	p_schedule->scheduleRecords[13].event = PPW_LOBBY_TIME_EVENT_FIRE_WORKS_START;
	p_schedule->scheduleRecords[14].time = 3 * 60;
	p_schedule->scheduleRecords[14].event = PPW_LOBBY_TIME_EVENT_FIRE_WORKS_END;
	p_schedule->scheduleRecords[15].time = 3 * 60;
//	p_schedule->scheduleRecords[15].time = 0;
	p_schedule->scheduleRecords[15].event = PPW_LOBBY_TIME_EVENT_PARADE;
	p_schedule->scheduleRecords[16].time = (6 * 60) + 10;
	p_schedule->scheduleRecords[16].event = PPW_LOBBY_TIME_EVENT_CLOSE;
	p_schedule->scheduleRecords[17].time = 2 * 60;
	p_schedule->scheduleRecords[17].event = PPW_LOBBY_TIME_EVENT_MINIGAME_END;

	result = PPW_LobbySetSchedule( p_schedule, 
			sizeof(PPW_LobbySchedule) + (sizeof(PPW_LobbyScheduleRecord) * (p_schedule->scheduleRecordNum - 1)) );
	GF_ASSERT( DWC_LOBBY_CheckCommonErr( result ) );
}




//----------------------------------------------------------------------------
/**
 *	@brief	���̐l�����邱�Ƃɂ���
 *
 *	@param	cp_data
 *	@param	userid 
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_DEBUG_PlayerIN( const void* cp_data, s32 userid )
{
	PPW_LobbySystemProfile system;

	system.subChannelKind	= PPW_LOBBY_CHANNEL_KIND_INVALID;
	system.enterTime		= p_DWC_LOBBYLIB_WK->time.currentTime;

	// ���邱�Ƃɂ��Ă݂�
	DWC_LOBBY_CallBack_Connect( userid, &system, cp_data, p_DWC_LOBBYLIB_WK->profilesize );
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̐l�̃v���t�B�[����������
 *
 *	@param	cp_data
 *	@param	useridx 
 */
//-----------------------------------------------------------------------------
void DWC_LOBBY_DEBUG_SetProfile( const void* cp_data, u32 userid )
{
	DWC_LOBBY_CallBack_UserProfileUpdate( userid, cp_data, p_DWC_LOBBYLIB_WK->profilesize );
}
#endif




//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	���ʃG���[�`�F�b�N
 *
 *	@param	result	�G���[�^�C�v
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�G���[���
 */
//-----------------------------------------------------------------------------
static BOOL DWC_LOBBY_CheckCommonErr( PPW_LOBBY_RESULT result )
{
	BOOL ret = TRUE;
	
	switch( result ){
	// ����
	case PPW_LOBBY_RESULT_SUCCESS:               ///< �����B
		break;

	// ���s
	case PPW_LOBBY_RESULT_ERROR_ALLOC:           ///< �������m�ۂɎ��s�B
	case PPW_LOBBY_RESULT_ERROR_SESSION:         ///< �v���I�ȒʐM�G���[�B
	case PPW_LOBBY_RESULT_ERROR_CHANNEL:         ///< �w�肵���`�����l���ɓ����Ă��Ȃ��B
	case PPW_LOBBY_RESULT_ERROR_STATE:           ///< �Ăяo���Ă͂����Ȃ���ԂŊ֐����Ăяo�����B

		DWC_LOBBY_PRINT("dwc err check %d\n",result);
		
		ret = FALSE;
		break;
		
	// �p�����[�^���s��
	case PPW_LOBBY_RESULT_ERROR_PARAM:           ///< �w�肵���������s���B
	case PPW_LOBBY_RESULT_ERROR_NODATA:          ///< �w�肵�����͑��݂��Ȃ��B
		GF_ASSERT_MSG( 0, "func param err = %d\n", result );
		ret = FALSE;
		break;

	// ���łɃp�j�b�N���
	case PPW_LOBBY_RESULT_ERROR_CONDITION:       ///< �v���I�G���[���������Ȃ̂ł��̊֐����Ăяo���Ȃ��B
		ret = FALSE;
		break;

	default:
		GF_ASSERT(0);	// ���肦�Ȃ�
		ret = FALSE;
		break;
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̃V�X�e���Ȃ��Őݒ肳�ꂽ�G���[��Ԃ��N���A
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_ClearSystemErr( void )
{
	p_DWC_LOBBYLIB_WK->profile_error	= FALSE;
	p_DWC_LOBBYLIB_WK->download_error	= FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	PPW�̃`�����l���^�C�v��n����DWC���p�̃T�u�`�����l���^�C�v���擾����
 *
 *	@param	channel		PPW�`�����l���^�C�v
 *
 *	@retval	DWC_LOBBY_SUBCHAN_FOOT1,			// ���Ճ{�[�h�P
 *	@retval	DWC_LOBBY_SUBCHAN_FOOT2,			// ���Ճ{�[�h�Q
 *	@retval	DWC_LOBBY_SUBCHAN_CLOCK,			// ���E���v
 *	@retval	DWC_LOBBY_SUBCHAN_NEWS,				// ���r�[�j���[�X
 *	@retval	DWC_LOBBY_SUBCHAN_NONE,				// �V�X�e�����Ŏg�p���܂�
 */
//-----------------------------------------------------------------------------
static DWC_LOBBY_SUBCHAN_TYPE DWC_LOBBY_GetSubChannelType( PPW_LOBBY_CHANNEL_KIND channel )
{
	s32 subchan;

	subchan = channel - 1;
	if( subchan < 0 ){
		subchan = DWC_LOBBY_SUBCHAN_NONE;
	}
	return subchan;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�`�����l���^�C�v��PPW�̃`�����l���^�C�v�ɂ���
 *
 *	@param	subchan_type	�T�u�`�����l���^�C�v
 *
 *	@return	PPW�̃`�����l���^�C�v
 */
//-----------------------------------------------------------------------------
static PPW_LOBBY_CHANNEL_KIND DWC_LOBBY_GetChannelKind( DWC_LOBBY_SUBCHAN_TYPE subchan_type )
{
	return subchan_type + 1;
}




//----------------------------------------------------------------------------
/**
 *	@brief	�����̍��f�[�^��ގ��ҍ��f�[�^�o�b�t�@�ɐݒ�
 *
 *	@param	p_sys	�V�X�e�����[�N
 *
 *	@retval	TRUE	�ݒ肵��
 *	@retval	FALSE	�ݒ肵�Ȃ�����
 */
//-----------------------------------------------------------------------------
static BOOL DWC_LOBBY_WLDDATA_SetMyData( DWC_LOBBYLIB_WK* p_sys )
{
	u32 nation;
	u32 area;
	int i;
	PPW_LOBBY_RESULT result;

	// ���ƒn��ID���擾
	nation	= WIFIHISTORY_GetMyNation( p_sys->p_wifihistory );
	area	= WIFIHISTORY_GetMyArea( p_sys->p_wifihistory );

	// �����Ȃ��Ȃ炻�̂܂ܔ�����
	if( nation == DWC_LOBBY_WLDDATA_NATION_NONE ){
		return FALSE;
	}

	// ���ƒn��œ����f�[�^���������牽�������ɔ�����
	for( i=0; i<DWC_WLDDATA_MAX; i++ ){
		if( p_sys->wfldata[i].flag == FALSE ){
			// �����Ƀf�[�^�����Ĕ�����
			
			break;
		}else{

			// ��r����
			if( (p_sys->wfldata[i].nation == nation) && 
				(p_sys->wfldata[i].area == area) ){
				return FALSE;	// �ꏏ�̃f�[�^���������̂ŉ�������������
			}
		}
	}

	// MAX�ɂȂ�����o�b�t�@�I�[�o�[
	if( i >= DWC_WLDDATA_MAX ){
		GF_ASSERT( 0 );
		return FALSE;
	}

	p_sys->wfldata[i].nation	= nation;
	p_sys->wfldata[i].area		= area;
	p_sys->wfldata[i].flag		= TRUE;

	// �f�[�^���ݒ肳�ꂽ�̂Ń`�����l���f�[�^���X�V
	result = PPW_LobbySetChannelData( 
			PPW_LOBBY_INVALID_USER_ID,	// �`�����l�����̂ɐݒ�
			PPW_LOBBY_CHANNEL_KIND_MAIN, sc_DWC_LOBBY_CHANNEL_KEY_NAME[DWC_LOBBY_CHANNEL_KEY_WLDDATA],
			(const u8*)p_sys->wfldata, sizeof(DWC_WLDDATA)*DWC_WLDDATA_MAX );

	OS_TPrintf( "wlddata send\n" );
	// �G���[�`�F�b�N
	return DWC_LOBBY_CheckCommonErr( result );
}






//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[��������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	size		�v���t�B�[���P�f�[�^�T�C�Y
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_Profile_Init( DWC_LOBBYLIB_WK* p_sys, u32 size, u32 heapID )
{
	p_DWC_LOBBYLIB_WK->p_profile	= sys_AllocMemory( heapID, size*PPW_LOBBY_MAX_PLAYER_NUM_MAIN );
	memset( p_DWC_LOBBYLIB_WK->p_profile, 0, size*PPW_LOBBY_MAX_PLAYER_NUM_MAIN );
	p_DWC_LOBBYLIB_WK->profilesize	= size;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[���j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_Profile_Exit( DWC_LOBBYLIB_WK* p_sys )
{
	sys_FreeMemoryEz( p_DWC_LOBBYLIB_WK->p_profile );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[��	�f�[�^�ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	userid		�ݒ��
 *	@param	cp_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_Profile_SetData( DWC_LOBBYLIB_WK* p_sys, s32 userid, const void* cp_data )
{
	DWC_LOBBY_Profile_SetDataEx( p_sys, userid, cp_data, p_sys->profilesize );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[��	�f�[�^�ݒ�	�f�[�^�T�C�Y�������s��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	userid		�ݒ��
 *	@param	cp_data		�f�[�^
 *	@param	datasize	�v���t�B�[���f�[�^�T�C�Y
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_Profile_SetDataEx( DWC_LOBBYLIB_WK* p_sys, s32 userid, const void* cp_data, u32 datasize )
{
	s32 idx;
	u8* p_buff;
	u32 copy_size;

	// userid����o�b�t�@�C���f�b�N�X���擾
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_sys, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	GF_ASSERT( idx != DWC_LOBBY_USERIDTBL_IDX_NONE );

	// �C���f�b�N�X��̃o�b�t�@�ɃR�s�[
	p_buff = p_sys->p_profile;
	p_buff = &p_buff[ p_sys->profilesize*idx ];

	// �R�s�[�T�C�Y
	// �v���t�B�[���T�C�Y���傫���Ƃ��i�����o�[�W�����j
	// �����̃v���t�B�[���T�C�Y�ŃR�s�[����
	//
	// �v���t�B�[���T�C�Y���������Ƃ��i�O�̃o�[�W�����j
	// ���̃T�C�Y�ŃR�s�[���違�R�s�[����O�ɂO�N���A
	if( p_sys->profilesize > datasize ){
		copy_size = datasize;
		memset( p_buff, 0, p_sys->profilesize );
	}else{
		copy_size = p_sys->profilesize;
	}
	memcpy( p_buff, cp_data, copy_size );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[�����擾
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	userid		���[�UID
 *
 *	@return	�v���t�B�[���f�[�^
 */
//-----------------------------------------------------------------------------
static const void* DWC_LOBBY_Profile_GetData( const DWC_LOBBYLIB_WK* cp_sys, s32 userid )
{
	s32 idx;
	const u8* cp_buff;

	// userid����o�b�t�@�C���f�b�N�X���擾
	idx = DWC_LOBBY_UserIDTbl_GetIdx( cp_sys, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	GF_ASSERT( idx != DWC_LOBBY_USERIDTBL_IDX_NONE );

	// �C���f�b�N�X��̃o�b�t�@�ɃR�s�[
	cp_buff = cp_sys->p_profile;
	cp_buff = &cp_buff[ cp_sys->profilesize*idx ];
	return cp_buff;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���t�B�[���N���[���f�[�^
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	userid		userID
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_Profile_CleanData( DWC_LOBBYLIB_WK* p_sys, s32 userid )
{
	s32 idx;
	u8* p_buff;

	// userid����o�b�t�@�C���f�b�N�X���擾
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_sys, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	GF_ASSERT( idx != DWC_LOBBY_USERIDTBL_IDX_NONE );

	// �C���f�b�N�X��̃o�b�t�@���N���A
	p_buff = p_sys->p_profile;
	p_buff = &p_buff[ p_sys->profilesize*idx ];
	memset( p_buff, 0, p_sys->profilesize );
}




//----------------------------------------------------------------------------
/**
 *	@brief	�V�X�e���v���t�B�[����ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	userid		���[�UID
 *	@param	cp_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_SysProfile_SetData( DWC_LOBBYLIB_WK* p_sys, s32 userid, const PPW_LobbySystemProfile* cp_data )
{
	s32 idx;

	// userid����o�b�t�@�C���f�b�N�X���擾
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_sys, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	GF_ASSERT( idx != DWC_LOBBY_USERIDTBL_IDX_NONE );
	// �C���f�b�N�X��̃o�b�t�@�ɃR�s�[
	p_sys->sysprofile[ idx ] = *cp_data;

	DWC_LOBBY_PRINT( "sys profile channel=%d\n", cp_data->subChannelKind );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�X�e���v���t�B�[�����擾
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	userid		���[�UID
 */
//-----------------------------------------------------------------------------
static const PPW_LobbySystemProfile* DWC_LOBBY_SysProfile_GetData( const DWC_LOBBYLIB_WK* cp_sys, s32 userid )
{
	s32 idx;

	// userid����o�b�t�@�C���f�b�N�X���擾
	idx = DWC_LOBBY_UserIDTbl_GetIdx( cp_sys, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	GF_ASSERT( idx != DWC_LOBBY_USERIDTBL_IDX_NONE );
	// �C���f�b�N�X��̃o�b�t�@��Ԃ�
	return &cp_sys->sysprofile[ idx ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�X�e���v���t�B�[���̃N���A
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	userid		userID
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_SysProfile_CleanData( DWC_LOBBYLIB_WK* p_sys, s32 userid )
{
	s32 idx;

	// userid����o�b�t�@�C���f�b�N�X���擾
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_sys, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	GF_ASSERT( idx != DWC_LOBBY_USERIDTBL_IDX_NONE );

	// �C���f�b�N�X��̃o�b�t�@�ɃR�s�[
	memset( &p_sys->sysprofile[ idx ], 0, sizeof(PPW_LobbySystemProfile) );
}





//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID��	������
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	maintblnum		���C�����[�U�ő吔
 *	@param	subtblnum		�T�u���[�U�ő吔
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_UserIDTbl_Init( DWC_LOBBYLIB_WK* p_sys, u32 maintblnum, u32 subtblnum, u32 heapID )
{
	int i;
	int tblnum;
	for( i=0; i<DWC_LOBBY_LOCALCHANNEL_TYPE_NUM; i++ ){
		GF_ASSERT( p_sys->useridtbl[ i ].p_tbl == NULL );
		if( i == DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ){
			tblnum = maintblnum;
		}else{
			tblnum = subtblnum;
		}
		p_sys->useridtbl[ i ].num			= 0;
		p_sys->useridtbl[ i ].p_tbl			= sys_AllocMemory( heapID, sizeof(s32)*tblnum );
		p_sys->useridtbl[ i ].tblnum		= tblnum;

		DWC_LOBBY_UserIDTbl_Clear( p_sys, i );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID��		�j��
 *	
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_UserIDTbl_Exit( DWC_LOBBYLIB_WK* p_sys )
{
	int i;

	for( i=0; i<DWC_LOBBY_LOCALCHANNEL_TYPE_NUM; i++ ){
		GF_ASSERT( p_sys->useridtbl[ i ].p_tbl != NULL );
		sys_FreeMemoryEz( p_sys->useridtbl[ i ].p_tbl );
		p_sys->useridtbl[ i ].p_tbl = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID�񋓃f�[�^�N���A
 *
 *	@param	p_sys				�V�X�e�����[�N
 *	@param	channeltype			�`�����l���^�C�v
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_UserIDTbl_Clear( DWC_LOBBYLIB_WK* p_sys, u32 channeltype )
{
	int i;

	p_sys->useridtbl[ channeltype ].num = 0;
	for( i=0; i<p_sys->useridtbl[ channeltype ].tblnum; i++ ){
		p_sys->useridtbl[ channeltype ].p_tbl[ i ] = DWC_LOBBY_INVALID_USER_ID;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID��	���[�UID�o�^
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	userid			���[�UID
 *	@param	channeltype		�`�����l���^�C�v
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_UserIDTbl_AddId( DWC_LOBBYLIB_WK* p_sys, s32 userid, u32 channeltype )
{
	u32 idx;
	int i;
	
	// ���łɓo�^�ς݂łȂ����`�F�b�N
	if( DWC_LOBBY_UserIDTbl_GetIdx( p_sys, userid, channeltype ) != DWC_LOBBY_USERIDTBL_IDX_NONE ){
		return ;
	} 

	// �܂��o�^�ł��邩�`�F�b�N
	GF_ASSERT( p_sys->useridtbl[ channeltype ].num < p_sys->useridtbl[ channeltype ].tblnum );
	if( p_sys->useridtbl[ channeltype ].num >= p_sys->useridtbl[ channeltype ].tblnum ){
		return ;
	}

	// �o�^�C���f�b�N�X�����߂�
	for( i=0; i<p_sys->useridtbl[ channeltype ].tblnum; i++ ){
		if( p_sys->useridtbl[ channeltype ].p_tbl[ i ] == DWC_LOBBY_INVALID_USER_ID ){
			idx = i;
			break;
		}
	}
	
	// �o�^
	p_sys->useridtbl[ channeltype ].p_tbl[ idx ] = userid;
	p_sys->useridtbl[ channeltype ].num ++;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID��	���[�UID�j��
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	userid			���[�UID
 *	@param	channeltype		�`�����l���^�C�v
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_UserIDTbl_DelId( DWC_LOBBYLIB_WK* p_sys, s32 userid, u32 channeltype )
{
	u32 idx;

	// �o�^IDX�擾
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_sys, userid, channeltype );
	
	// �j���ς݂���Ȃ����`�F�b�N
	if( idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
		return ;
	} 

	// �j��
	p_sys->useridtbl[ channeltype ].p_tbl[ idx ] = DWC_LOBBY_INVALID_USER_ID;
	p_sys->useridtbl[ channeltype ].num --;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID��		���[�UID����e�[�u���C���f�b�N�X���擾
 *
 *	@param	cp_sys			�V�X�e�����[�N
 *	@param	userid			���[�UID
 *	@param	channeltype		�`�����l���^�C�v
 *
 *	@retval	DWC_LOBBY_USERIDTBL_IDX_NONE	���̃��[�UID�̓o�^����Ă��Ȃ�
 *	@retval	����ȊO						���[�UID�̃e�[�u���C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static s32 DWC_LOBBY_UserIDTbl_GetIdx( const DWC_LOBBYLIB_WK* cp_sys, s32 userid, u32 channeltype )
{
	int i;

	for( i=0; i<cp_sys->useridtbl[ channeltype ].tblnum; i++ ){
		if( cp_sys->useridtbl[ channeltype ].p_tbl[ i ] == userid ){
			return i;
		}
	}
	return DWC_LOBBY_USERIDTBL_IDX_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�UID��		�C���f�b�N�X�ʒu�̃��[�UID���擾
 *
 *	@param	cp_sys			�V�X�e�����[�N
 *	@param	idx				�C���f�b�N�X
 *	@param	channeltype		�`�����l���^�C�v
 *
 *	@retval	DWC_LOBBY_INVALID_USER_ID	���̃C���f�b�N�X�ɂ͒N�����Ȃ� 	
 *	@retval	���̑�						���[�UID
 */
//-----------------------------------------------------------------------------
static s32 DWC_LOBBY_UserIDTbl_GetUserid( const DWC_LOBBYLIB_WK* cp_sys, s32 idx, u32 channeltype )
{
	GF_ASSERT( idx < cp_sys->useridtbl[ channeltype ].tblnum );
	return cp_sys->useridtbl[ channeltype ].p_tbl[ idx ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	SystemProfile���g�p���āA���̃v���C���[�̃`�����l���o�^�󋵂��쐬
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	subchan_type	�ݒ肷��`�����l��
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_UserIDTbl_SUBCHAN_SetSysProfileData( DWC_LOBBYLIB_WK* p_sys, DWC_LOBBY_SUBCHAN_TYPE subchan_type )
{
	int i;
	s32 userid;
	u32 channel_kind;

	// �T������PPW�`�����l���^�C�v�ɕϊ�����
	channel_kind = DWC_LOBBY_GetChannelKind( subchan_type );

	// 1�x�N���[��
	DWC_LOBBY_UserIDTbl_Clear( p_sys, DWC_LOBBY_LOCALCHANNEL_TYPE_SUB );

	// ���̏󋵂�ݒ肷��
	for( i=0; i<p_sys->useridtbl[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].tblnum; i++ ){
		userid = DWC_LOBBY_UserIDTbl_GetUserid( p_sys, i, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
		if( userid != DWC_LOBBY_USERIDTBL_IDX_NONE ){
			if( p_sys->sysprofile[ i ].subChannelKind == channel_kind ){
				DWC_LOBBY_UserIDTbl_AddId( p_sys,  userid, DWC_LOBBY_LOCALCHANNEL_TYPE_SUB );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	SystemProfile���g�p���āA������v���C���[�̎w��`�����l���ւ̐ڑ������`�F�b�N���A���O�C�����Ă����v���`�F�b�N
 *
 *	@param	cp_sys				�V�X�e��
 *	@param	subchan_type		�T�u�`�����l���^�C�v
 *
 *	@retval	TRUE		LOGIN�@OK
 *	@retval	FALSE		LOGIN�@NG
 */
//-----------------------------------------------------------------------------
static BOOL DWC_LOBBY_UserIDTbl_SUBCHAN_CheckSysProfileLogin( const DWC_LOBBYLIB_WK* cp_sys, DWC_LOBBY_SUBCHAN_TYPE subchan_type )
{
	int i;
	int num;
	s32 userid;
	u32 channel_kind;
	static const u8 sc_ChannelMaxNum[ DWC_LOBBY_SUBCHAN_NONE ] = {
		PPW_LOBBY_MAX_PLAYER_NUM_FOOT,
		PPW_LOBBY_MAX_PLAYER_NUM_FOOT,
		PPW_LOBBY_MAX_PLAYER_NUM_CLOCK,
		PPW_LOBBY_MAX_PLAYER_NUM_NEWS,
	};

	GF_ASSERT( subchan_type < DWC_LOBBY_SUBCHAN_NONE );

	// �T������PPW�`�����l���^�C�v�ɕϊ�����
	channel_kind = DWC_LOBBY_GetChannelKind( subchan_type );

	// �ڑ����𐔂���
	num = 0;
	for( i=0; i<cp_sys->useridtbl[ DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ].tblnum; i++ ){
		userid = DWC_LOBBY_UserIDTbl_GetUserid( cp_sys, i, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
		if( userid != DWC_LOBBY_USERIDTBL_IDX_NONE ){
			if( cp_sys->sysprofile[ i ].subChannelKind == channel_kind ){
				num ++;
			}
		}
	}

	//  �ڑ��l���I�[�΁[�`�F�b�N
	if( sc_ChannelMaxNum[ subchan_type ] > num ){
		return TRUE;
	}
	return FALSE;
}






//----------------------------------------------------------------------------
/**
 *	@brief		�R�[���o�b�N�S
 */
//-----------------------------------------------------------------------------
// �����̓�������
static void DWC_LOBBY_CallBack_JoinChannel( BOOL success, PPW_LOBBY_CHANNEL_KIND channel )
{
	PPW_LOBBY_RESULT result;

	if( channel == PPW_LOBBY_CHANNEL_KIND_MAIN ){
		// �X�P�W���[���f�[�^���擾
		p_DWC_LOBBYLIB_WK->schedulesize = DWC_LOBBY_SCHEDULE_SIZE;
		result = PPW_LobbyGetSchedule( (PPW_LobbySchedule*)p_DWC_LOBBYLIB_WK->schedulebuff, &p_DWC_LOBBYLIB_WK->schedulesize );
		GF_ASSERT( DWC_LOBBY_CheckCommonErr( result ) );	// �o�b�t�@�T�C�Y�G���[

		{
			PPW_LobbySchedule* p_schedule;
			p_schedule = (PPW_LobbySchedule*)p_DWC_LOBBYLIB_WK->schedulebuff;
			DWC_LOBBY_PRINT( "callback  join  shcedulesize = 0x%x buffsize = 0x%x\n", p_DWC_LOBBYLIB_WK->schedulesize, DWC_LOBBY_SCHEDULE_SIZE );
			DWC_LOBBY_PRINT( "room = %d		season = %d\n", p_schedule->roomType, p_schedule->season );
		}

		// ���E���v�f�[�^�̎擾
		PPW_LobbyGetChannelDataAsync(PPW_LOBBY_INVALID_USER_ID, PPW_LOBBY_CHANNEL_KIND_MAIN, 
				sc_DWC_LOBBY_CHANNEL_KEY_NAME[DWC_LOBBY_CHANNEL_KEY_WLDDATA], NULL );
	}else{

		if( p_DWC_LOBBYLIB_WK->subchan_type == DWC_LOBBY_GetSubChannelType( channel ) ){
			if( success == TRUE ){
				p_DWC_LOBBYLIB_WK->subchan_login_success = DWC_LOBBY_SUBCHAN_LOGIN_OK;

				// ���̃v���C���[��subchan_type�ւ̓����󋵂�UserIDTbl�ɏ�������
				DWC_LOBBY_UserIDTbl_SUBCHAN_SetSysProfileData( p_DWC_LOBBYLIB_WK, p_DWC_LOBBYLIB_WK->subchan_type );

				// �����̒ǉ�
				DWC_LOBBY_UserIDTbl_AddId( p_DWC_LOBBYLIB_WK, PPW_LobbyGetMyUserId(), DWC_LOBBY_LOCALCHANNEL_TYPE_SUB );
				
			}else{
				p_DWC_LOBBYLIB_WK->subchan_login_success = DWC_LOBBY_SUBCHAN_LOGIN_NG;
			}
			DWC_LOBBY_PRINT( "callback  subchan join  chantype = %d success = %d\n", channel, success );
		}else{

			GF_ASSERT( 0 );
			//  ���̃`�����l���ɂ̓A�N�Z�X���Ă��Ȃ��̂ŁA�ގ�������
			DWC_LOBBY_SUBCHAN_Logout();
		}
	}
}

// ���r�[�����R�[���o�b�N
static void DWC_LOBBY_CallBack_Connect( s32 userid, const PPW_LobbySystemProfile* cp_sysprofile, const u8* cp_userprofile, u32 user_profilesize )
{
	BOOL myuser;
	
	DWC_LOBBY_PRINT( "callback  connect userid = %d\n", userid );

	// �������`�F�b�N
	if( userid == PPW_LobbyGetMyUserId() ){

		// �ŐV�̃��r�[�̎��Ԃ��擾
		{
			PPW_LobbyGetTimeInfo( &p_DWC_LOBBYLIB_WK->time );
		}

		myuser = TRUE;

	}else{
		myuser = FALSE;
	}

	//  ���̃��[�U���C���f�b�N�X�e�[�u���ɒǉ�
	{
		DWC_LOBBY_UserIDTbl_AddId( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	}

	// ���̃��[�U�̃v���t�B�[�����R�s�[
	{
		DWC_LOBBY_Profile_SetDataEx( p_DWC_LOBBYLIB_WK, userid, cp_userprofile, user_profilesize );
	}

	// ���̃��[�U�̃V�X�e���v���t�B�[�����R�s�[
	{
		DWC_LOBBY_SysProfile_SetData( p_DWC_LOBBYLIB_WK, userid, cp_sysprofile );
	}

	// �����R�[���o�b�N
	{
		p_DWC_LOBBYLIB_WK->callback.p_user_in( userid, 
				DWC_LOBBY_Profile_GetData( p_DWC_LOBBYLIB_WK, userid ), 
				p_DWC_LOBBYLIB_WK->p_callbackwork, myuser );
	}
} 

//----------------------------------------------------------------------------
/**
 *	@brief	�ؒf�R�[���o�b�N
 *
 *	@param	userid	���[�UID
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_DesconnectedChannel( s32 userid )
{
	DWC_LOBBY_PRINT( "callback  desconnect userid = %d\n", userid );

	// ���̐l��m���Ă��邩�`�F�b�N
	if( DWC_LOBBY_UserIDTbl_GetIdx( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN ) == DWC_LOBBY_USERIDTBL_IDX_NONE ){
		return ;	// ���łɂ��Ȃ�
	}

	// ��W�҂Ȃ��W�f�[�^�j��
	{
		int i;
		for( i=0; i<DWC_LOBBY_MG_NUM; i++ ){
			if( p_DWC_LOBBYLIB_WK->mg_data[ i ].mg_parent == userid ){
				// ���̕�W���I��������Ƃɂ���
				DWC_LOBBY_CallBack_RecruitStop( userid, &p_DWC_LOBBYLIB_WK->mg_data[ i ].mg_data );
			}
		}
	}
	

	// �ގ��R�[���o�b�N
	{
		p_DWC_LOBBYLIB_WK->callback.p_user_out( userid, p_DWC_LOBBYLIB_WK->p_callbackwork );
	}

	// �V�X�e���v���t�B�[���o�b�t�@�ƃv���t�B�[���o�b�t�@���N���A
	{
		DWC_LOBBY_SysProfile_CleanData( p_DWC_LOBBYLIB_WK, userid );
		DWC_LOBBY_Profile_CleanData( p_DWC_LOBBYLIB_WK, userid );
	}

	//  ���̃��[�U���C���f�b�N�X�e�[�u������폜
	{
		DWC_LOBBY_UserIDTbl_DelId( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
		DWC_LOBBY_UserIDTbl_DelId( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_SUB );
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�����񃁃b�Z�[�W��M
 *
 *	@param	userid			userID
 *	@param	channel			�`�����l��
 *	@param	type			�^�C�v
 *	@param	cp_message		���b�Z�[�W�f�[�^
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_StringMessageRecv( s32 userid, PPW_LOBBY_CHANNEL_KIND channel, s32 type, const char* cp_message )
{
	DWC_LOBBY_PRINT( "-----------err string messege recv-------------\n" );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�C�i�����b�Z�[�W�̎�M
 *
 *	@param	userid		���[�UID
 *	@param	channel		�`�����l��
 *	@param	type		�^�C�v
 *	@param	cp_data		�f�[�^
 *	@param	datasize	�f�[�^�T�C�Y
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_BynaryMessageRecv( s32 userid, PPW_LOBBY_CHANNEL_KIND channel, s32 type, const u8* cp_data, u32 datasize )
{
	u32 chan_type;
	u32 subchan_type;
	s32 idx;
	
	DWC_LOBBY_PRINT( "bin mess recv  user=%d chan=%d type=%d\n", userid, channel, type );

	// ���̃��[�UID�������Ă��邩�`�F�b�N
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	if( idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
		return ;	// ���̂ЂƂ���Ȃ�
	}
	
	
	switch( channel ){
    case PPW_LOBBY_CHANNEL_KIND_MAIN:            ///< ���C���`�����l���B
		chan_type = DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN;
		break;

    case PPW_LOBBY_CHANNEL_KIND_FOOT1:           ///< ���Ճ{�[�h1�B
    case PPW_LOBBY_CHANNEL_KIND_FOOT2:           ///< ���Ճ{�[�h2�B
    case PPW_LOBBY_CHANNEL_KIND_CLOCK:           ///< ���E���v�B
    case PPW_LOBBY_CHANNEL_KIND_NEWS:            ///< ���r�[�j���[�X�B
		subchan_type = DWC_LOBBY_GetSubChannelType( channel );	// MAIN�`�����l����������
		if( p_DWC_LOBBYLIB_WK->subchan_type == subchan_type ){
			chan_type = DWC_LOBBY_LOCALCHANNEL_TYPE_SUB;
		}else{
			// �`�����l���^�C�v���Ⴄ�̂ŉ������Ȃ�
			return;
		}
		break;

	case  PPW_LOBBY_CHANNEL_KIND_INVALID:
		chan_type = DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN;
		break;
	
	default:
		return;
	}

	// �^�C�v�����킩�`�F�b�N
	if( p_DWC_LOBBYLIB_WK->msgcmd[ chan_type ].msgtblnum > type ){

		// �A�b�p�[�o�[�W�����ŃT�C�Y���傫���Ȃ�\��������̂ŁA�T�C�Y���傫���Ȃ��Ă���̂͋���
		GF_ASSERT( p_DWC_LOBBYLIB_WK->msgcmd[ chan_type ].cp_msgtbl[ type ].size <= datasize );

		// �R�[���o�b�N�Ăяo��
		p_DWC_LOBBYLIB_WK->msgcmd[ chan_type ].cp_msgtbl[ type ].p_func( 
				userid, cp_data, 
				p_DWC_LOBBYLIB_WK->msgcmd[ chan_type ].cp_msgtbl[ type ].size, 
				p_DWC_LOBBYLIB_WK->msgcmd[ chan_type ].p_work );
	}else{
		DWC_LOBBY_PRINT( "err	binmsg type err\n" );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�����l���f�[�^��M����
 *
 *	@param	success			��M�ɐ���������
 *	@param	broadcast		�u���[�h�L���X�g�t���O
 *	@param	channel			���M�`�����l���i�`�����l�������M�����Ƃ��j
 *	@param	userid			���M���[�UID�i���[�U�`�����l���f�[�^�����M�����Ƃ��j
 *	@param	p_key			�`�����l���f�[�^�̃L�[
 *	@param	cp_data			�f�[�^
 *	@param	datasize		�f�[�^�T�C�Y	
 *	@param	param			�֐��Ɏw�肵���p�����[�^
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_ChannelDataRecv( BOOL success, BOOL broadcast, PPW_LOBBY_CHANNEL_KIND channel, s32 userid, const char* p_key, const u8* cp_data, u32 datasize, void* p_param )
{
	s32 myuserid;
	DWC_LOBBY_PRINT( "channel data recv %s\n", p_key );

	// ��M�`�F�b�N
	if( strcmp( p_key, sc_DWC_LOBBY_CHANNEL_KEY_NAME[DWC_LOBBY_CHANNEL_KEY_WLDDATA] ) == 0 ){
		memcpy( p_DWC_LOBBYLIB_WK->wfldata, cp_data, sizeof(DWC_WLDDATA)*DWC_WLDDATA_MAX );

		// ���M�����҂����Ȃ�t���O��������
		if( p_DWC_LOBBYLIB_WK->wlddata_send ){
			p_DWC_LOBBYLIB_WK->wlddata_send = FALSE;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�X�e���v���t�B�[�����X�V���ꂽ
 *	
 *	@param	userid			���[�UID
 *	@param	cp_profile		�V�X�e���v���t�B�[��
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_SystemProfileUpdate( s32 userid, const PPW_LobbySystemProfile* cp_profile )
{
	u32 after_channel, before_channel, sys_channel;
	const PPW_LobbySystemProfile* cp_before;
	s32 idx;

	DWC_LOBBY_PRINT( "sys profile update %d�@�@channelno %d\n", userid, cp_profile->subChannelKind );

	// ���̃��[�UID�������Ă��邩�`�F�b�N
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	if( idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
		return ;	// ���̂ЂƂ���Ȃ�
	}

	//-------------------------------------
	// �������T�u�`�����l���ɂ���Ƃ��́A�T�u�`�����l���ɗ����l���m�F����
	//=====================================
	if( p_DWC_LOBBYLIB_WK->subchan_type != DWC_LOBBY_SUBCHAN_NONE ){
		// �`�����l�������C���`�����l������T�u�`�����l���ɂȂ��ĂȂ����A
		// �T�u�`�����l�����烁�C���`�����l���ɂȂ��Ă��Ȃ����m�F����
		cp_before = DWC_LOBBY_SysProfile_GetData( p_DWC_LOBBYLIB_WK, userid );
		before_channel	= cp_before->subChannelKind;		//  PPW�O�̏��
		after_channel	= cp_profile->subChannelKind;		//	�X�V�����Ƃ��̏��
		sys_channel		= DWC_LOBBY_GetChannelKind( p_DWC_LOBBYLIB_WK->subchan_type );	// �����̏��
		if( (before_channel == PPW_LOBBY_CHANNEL_KIND_INVALID) && (after_channel == sys_channel) ){
			// ���̃��[�U�������Ɠ����`�����l���ɂ���Ă���
			DWC_LOBBY_UserIDTbl_AddId( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_SUB );
		}else if( (before_channel == sys_channel) && (after_channel != sys_channel) ){	// �����������y������
//		}else if( (before_channel == sys_channel) && (after_channel == PPW_LOBBY_CHANNEL_KIND_INVALID) ){
			// ���̃��[�U�͎����Ɠ����`�����l������ގ�����
			DWC_LOBBY_UserIDTbl_DelId( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_SUB );
		}

	}
	

	// �V�X�e���v���t�B�[���X�V
	DWC_LOBBY_SysProfile_SetData( p_DWC_LOBBYLIB_WK, userid, cp_profile );	
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�v���t�B�[���X�V
 *
 *	@param	userid		���[�UID
 *	@param	cp_profile	�v���t�B�[��
 *	@param	size		�T�C�Y
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_UserProfileUpdate( s32 userid, const u8* cp_profile, u32 size )
{
	s32 idx;

	DWC_LOBBY_PRINT( "profile update %d\n", userid );

	// ���̃��[�UID�������Ă��邩�`�F�b�N
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	if( idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
		return ;	// ���̂ЂƂ���Ȃ�
	}
		
	// �V�����v���t�B�[����ݒ�
	DWC_LOBBY_Profile_SetData( p_DWC_LOBBYLIB_WK, userid, cp_profile );	

	// �v���t�B�[���X�V�R�[���o�b�N
	p_DWC_LOBBYLIB_WK->callback.p_profile_update( userid, cp_profile, 
			p_DWC_LOBBYLIB_WK->p_callbackwork );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��W�v�����ς����
 *
 *	@param	userid		���[�UID
 *	@param	cp_info		��W�f�[�^
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_Recruit( s32 userid, const PPW_LobbyRecruitInfo* cp_info )
{
	u32 myuserid;
	s64 mytime;
	s64 othertime;
	s32 idx;
	
	DWC_LOBBY_PRINT( "Recruit Recv userid=%d gametype = %d entrynum = %d\n", userid, cp_info->gameKind, cp_info->currentNum );

	// ���̃��[�UID�������Ă��邩�`�F�b�N
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	if( idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
		return ;	// ���̂ЂƂ���Ȃ�
	}


	myuserid = DWC_LOBBY_GetMyUserID();

	// �����ȊO�������̍쐬������W�v���Ɠ����~�j�Q�[���̕�W�v�����o�����疳������
	if( myuserid != userid ){
		if( p_DWC_LOBBYLIB_WK->mg_myparent ){
			if( p_DWC_LOBBYLIB_WK->mg_myentry == cp_info->gameKind ){
				// ���̕�W���n�܂������ԂƎ����̎��Ԃ��r���āA��ɗ������̂��c��
				DWC_LOBBY_MG_GetRecruitStartTime( &p_DWC_LOBBYLIB_WK->mg_data[ p_DWC_LOBBYLIB_WK->mg_myentry ].mg_data, &mytime );
				DWC_LOBBY_MG_GetRecruitStartTime( cp_info, &othertime );
				if( mytime >= othertime ){	// �������x�������玩�����I������
					// �����̏I����������
					p_DWC_LOBBYLIB_WK->mg_myend = TRUE;

					// ��W���I��������
					DWC_LOBBY_MG_EndRecruit();
					DWC_LOBBY_PRINT( "!recruit err end\n" );
				}else{
					// ����̂ق����x���̂ŁA�f�[�^�𖳎�����
					return ;
				}
			}
		}
	}
	
	// ��W����ۑ�
	DWC_LOBBY_MG_SetData( p_DWC_LOBBYLIB_WK, cp_info->gameKind, userid, cp_info );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U����W���I�������Ƃ�
 *
 *	@param	userid		���[�UID
 *	@param	cp_info		��W�f�[�^
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_RecruitStop( s32 userid, const PPW_LobbyRecruitInfo* cp_info )
{
	BOOL result;
	s32 idx;
	
	DWC_LOBBY_PRINT( "Recruit stop Recv userid=%d  game=%d\n", userid, cp_info->gameKind );

	// ���̃��[�UID�������Ă��邩�`�F�b�N
	idx = DWC_LOBBY_UserIDTbl_GetIdx( p_DWC_LOBBYLIB_WK, userid, DWC_LOBBY_LOCALCHANNEL_TYPE_MAIN );
	if( idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
		return ;	// ���̂ЂƂ���Ȃ�
	}

	
	
	// ��W����ۑ�
	// �߂�l�ŁA�f�[�^�����Z�b�g�������擾�ł���
	// ���Z�b�g���Ȃ������Ƃ������Ƃ́A��������M�������̂Ɛe���Ⴄ�Ƃ�������
	result = DWC_LOBBY_MG_ResetData( p_DWC_LOBBYLIB_WK, cp_info->gameKind, userid );

	// �����̑����Ă����W���I�������狭���I���t���O�𗧂Ă�
	if( (p_DWC_LOBBYLIB_WK->mg_myentry == cp_info->gameKind) && (result == TRUE) ){
		p_DWC_LOBBYLIB_WK->mg_myend = TRUE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�P�W���[���̋N��
 *
 *	@param	event	�C�x���g�i���o�[
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_Schedule( PPW_LOBBY_TIME_EVENT event )
{
	DWC_LOBBY_PRINT( "schedule Recv event=%d\n", event );
	
	// �R�[���o�b�N���Ă�
	p_DWC_LOBBYLIB_WK->callback.p_event( event, p_DWC_LOBBYLIB_WK->p_callbackwork );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o���烆�[�U��{���̕s���`�F�b�N���ʂ��擾����
 *
 *	@param	result			�ʐM����
 *	@param	userProfile		���[�U��{���
 *	@param	userProfileSize	��{���T�C�Y
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_CheckProfile( PPW_LOBBY_STATS_RESULT result, const u8 *userProfile, u32 userProfileSize )
{
	DWC_LOBBY_PRINT( "profile ��M�@����\n" );
	
	if( result != PPW_LOBBY_STATS_RESULT_SUCCESS ){

		p_DWC_LOBBYLIB_WK->profile_error = TRUE;
		
		// ���̑��̏ꍇ�̓V���b�g�_�E����
		DWC_LOBBY_PRINT( "profile �s���ȃ��[�U\n" );
		
	}else{


		// �u����̃v���t�B�[����ʒB
		{
			p_DWC_LOBBYLIB_WK->callback.p_check_profile( userProfile, 
					userProfileSize,
					p_DWC_LOBBYLIB_WK->p_callbackwork );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o����X�P�W���[�����_�E�����[�h�����ۂɌĂ΂��R�[���o�b�N
 *
 *	@param	result			�ʐM����
 *	@param	p_schedule		�X�P�W���[���f�[�^
 */
//-----------------------------------------------------------------------------
static BOOL DWC_LOBBY_CallBack_NotifySchedule( PPW_LOBBY_STATS_RESULT result, const PPW_LobbySchedule* p_schedule )
{
	if( result == PPW_LOBBY_STATS_RESULT_SUCCESS ){
		// �_�E�����[�h����
		DWC_LOBBY_PRINT( "Schedule �_�E�����[�h�@����\n" );
#ifdef PM_DEBUG
		// �f�o�b�N�J�n�̏ꍇ�Ńo�b�N�ݒ肳�ꂽ�p�����[�^���g�p����
		if( D_Tomoya_WiFiLobby_DebugStart == TRUE ){
			return FALSE;
		}
		return TRUE;
#else
		return TRUE;
#endif	
	}

	p_DWC_LOBBYLIB_WK->download_error = TRUE;
	
	// ���̑��̏ꍇ�̓V���b�g�_�E����
	DWC_LOBBY_PRINT( "Schedule �s���ȃ��[�U\n" );
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP�����_�E�����[�h�����ۂɌĂ΂��R�[���o�b�N
 *	
 *	@param	result			�ʐM����
 *	@param	cp_vipRecords	��M�f�[�^
 *	@param	num				VIP�f�[�^��
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_Vip( PPW_LOBBY_STATS_RESULT result, const PPW_LobbyVipRecord* cp_vipRecords, u32 num )
{
	// �_�E�����[�h����
	if( result == PPW_LOBBY_STATS_RESULT_SUCCESS ){
		DWC_LOBBY_PRINT( "VIP �_�E�����[�h�@����\n" );
		DWC_LOBBY_VIPDATA_SetData( &p_DWC_LOBBYLIB_WK->vipdata, cp_vipRecords, num );
	}else{

		p_DWC_LOBBYLIB_WK->download_error = TRUE;
		// ���s�����̂ŃV���b�g�_�E��
		DWC_LOBBY_PRINT( "VIP �_�E�����[�h�@���s\n" );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�����_�E�����[�h�����ۂɌĂ΂��R�[���o�b�N
 *
 *	@param	result				�ʐM����
 *	@param	cp_questionnaire	��M�f�[�^
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_Anketo( PPW_LOBBY_STATS_RESULT result, const PPW_LobbyQuestionnaire* cp_questionnaire )
{
	// �_�E�����[�h����
	if( result == PPW_LOBBY_STATS_RESULT_SUCCESS ){
		DWC_LOBBY_PRINT( "�A���P�[�g �_�E�����[�h�@����\n" );
		DWC_LOBBY_ANKETO_Set( &p_DWC_LOBBYLIB_WK->anketo, cp_questionnaire );
	}else{

		p_DWC_LOBBYLIB_WK->download_error = TRUE;
		// ���s�����̂ŃV���b�g�_�E��
		DWC_LOBBY_PRINT( "�A���P�[�g �_�E�����[�h�@���s\n" );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�̓��[����
 *
 *	@param	result	����������
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_AnketoSubmit( PPW_LOBBY_STATS_RESULT result )
{
	GF_ASSERT( p_DWC_LOBBYLIB_WK->anketo.submit_state == DWC_LOBBY_ANKETO_STATE_SENDING );
	
	if( result == PPW_LOBBY_STATS_RESULT_SUCCESS ){
		DWC_LOBBY_PRINT( "�Ƃ��Ђ傤�@����\n" );
		p_DWC_LOBBYLIB_WK->anketo.submit_state = DWC_LOBBY_ANKETO_STATE_SENDOK;
	}else{
		DWC_LOBBY_PRINT( "�Ƃ��Ђ傤�@���s\n" );
//		p_DWC_LOBBYLIB_WK->anketo.submit_state = DWC_LOBBY_ANKETO_STATE_SENDNG;
		p_DWC_LOBBYLIB_WK->anketo.submit_state = DWC_LOBBY_ANKETO_STATE_SENDOK;	// ���s��OK�Ȏ��ɂ����Ⴄ 080616 tomoya
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�Ƃ̒ʐM���ߏ�ɍs��ꂽ�Ƃ��̃R�[���o�b�N
 *
 *	@param	floodWeight		�����������d��	300�Ń`�����l���f�[�^���擾�ł��Ȃ��Ȃ�A600�ŃT�[�o����ؒf�����
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_CallBack_ExcessFlood( u32 floodWeight )
{
	DWC_LOBBY_PRINT( "ExcessFlood  floodWeight=%d\n", floodWeight );
//	GF_ASSERT_MSG( 0, "ExcessFlood  floodWeight=%d\n", floodWeight );

	p_DWC_LOBBYLIB_WK->excess_flood = TRUE;
}




//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����W�p�����[�^������
 *
 *	@param	p_sys		�V�X�e��
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_MG_Init( DWC_LOBBYLIB_WK* p_sys )
{
	memset( p_sys->mg_data, 0, sizeof(DWC_LOBBY_MGDATA)*DWC_LOBBY_MG_NUM );
	p_sys->mg_myentry = DWC_LOBBY_MG_NUM;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����W�f�[�^�ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	type		�^�C�v
 *	@param	userid		���[�UID
 *	@param	cp_mg_data	�f�[�^
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_MG_SetData( DWC_LOBBYLIB_WK* p_sys, DWC_LOBBY_MG_TYPE type, s32 userid, const PPW_LobbyRecruitInfo* cp_mg_data )
{
	// �^�C�v�������Ă邩�`�F�b�N
	GF_ASSERT( type < DWC_LOBBY_MG_NUM );
	
	// ���łɐݒ�ς݂ł͂Ȃ����`�F�b�N
	if( DWC_LOBBY_MG_Check( p_sys, type ) == TRUE ){
		// �����l���`�F�b�N
		if( p_sys->mg_data[ type ].mg_parent != userid ){
			// �X�V���Ȃ�
			DWC_LOBBY_PRINT( "MG Recruit userid err parent=%d, now=%d\n", p_sys->mg_data[ type ].mg_parent,  userid );
			return ;
		}
	}

	p_sys->mg_data[ type ].mg_data		= *cp_mg_data;

	if( p_sys->mg_data[ type ].mg_flag == FALSE ){
		p_sys->mg_data[ type ].mg_flag		= TRUE;
		p_sys->mg_data[ type ].mg_parent	= userid;

		// �e����W�����������Ԃ��獡�Ԃł̃t���[�������������^�C�����~�b�g���쐬����
		p_sys->mg_data[ type ].mg_timelimit	= DWC_LOBBY_MG_CalcTimeLimit( p_sys, type );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�f�[�^���Z�b�g
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	type		�^�C�v
 *	@param	userid		���[�UID
 *
 *	@retval	TRUE	�f�[�^�����Z�b�g����
 *	@retval	FALSE	�f�[�^�����Z�b�g���Ȃ�����
 */
//-----------------------------------------------------------------------------
static BOOL DWC_LOBBY_MG_ResetData( DWC_LOBBYLIB_WK* p_sys, DWC_LOBBY_MG_TYPE type, s32 userid )
{
	// �^�C�v�������Ă邩�`�F�b�N
	GF_ASSERT( type < DWC_LOBBY_MG_NUM );

	DWC_LOBBY_PRINT( "MG_ResetData userid=%d  param userid=%d	", p_sys->mg_data[ type ].mg_parent, userid );
	
	// �f�[�^�����邩�`�F�b�N
	if( DWC_LOBBY_MG_Check( p_sys, type ) == TRUE ){
		// ���[�UID���ꏏ���`�F�b�N
		if( p_sys->mg_data[ type ].mg_parent == userid ){
			DWC_LOBBY_PRINT( "data clear\n" );
			// �ꏏ�Ȃ烊�Z�b�g
			p_sys->mg_data[ type ].mg_flag = FALSE;
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��W�����`�F�b�N
 *
 *	@param	cp_sys	�V�X�e�����[�N
 *	@param	type	�~�j�Q�[���^�C�v
 *
 *	@retval	TRUE	��W��
 *	@retval	FALSE	��W�Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL DWC_LOBBY_MG_Check( const DWC_LOBBYLIB_WK* cp_sys, DWC_LOBBY_MG_TYPE type )
{
	// �^�C�v�������Ă邩�`�F�b�N
	GF_ASSERT( type < DWC_LOBBY_MG_NUM );
	return cp_sys->mg_data[ type ].mg_flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�����~�b�g�̌v�Z���s��
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	type		�~�j�Q�[���^�C�v
 */
//-----------------------------------------------------------------------------
static u16 DWC_LOBBY_MG_CalcTimeLimit( const DWC_LOBBYLIB_WK* cp_sys, DWC_LOBBY_MG_TYPE type )
{
	s64 start_time;
	s64 now_time;
	const PPW_LobbyRecruitInfo* cp_info;

	// ��W��񂪂��邩�`�F�b�N
	if( DWC_LOBBY_MG_Check( cp_sys, type ) == FALSE ){
		return 0;
	}
		
	// ��W�J�n���Ԏ擾
	cp_info = DWC_LOBBY_MG_GetData( cp_sys, type );
	DWC_LOBBY_MG_GetRecruitStartTime( cp_info, &start_time );

	// �S�̃^�C�����~�b�g����o�߂������Ԃ�����
	now_time = cp_sys->time.currentTime - start_time;	// �b�P��
	if( now_time > DWC_LOBBY_MG_TIMELIMIT_SEC ){	// �傫��������҂����Ԃ��ő�l�ɂ���
		now_time = DWC_LOBBY_MG_TIMELIMIT_SEC;
	}else if( now_time < 0 ){
		now_time = 0;
	}
	return ((DWC_LOBBY_MG_TIMELIMIT-now_time) * DWC_LOBBY_MG_TIMELIMIT_SEC);
}

//----------------------------------------------------------------------------
/**
 *	@brief	��W�����擾����
 *
 *	@param	cp_sys		�����Ăރ��[�N
 *	@param	type		�~�j�Q�[���^�C�v
 *		
 *	@return	��W���
 */
//-----------------------------------------------------------------------------
static const PPW_LobbyRecruitInfo* DWC_LOBBY_MG_GetData( const DWC_LOBBYLIB_WK* cp_sys, DWC_LOBBY_MG_TYPE type )
{
	// �^�C�v�������Ă邩�`�F�b�N
	GF_ASSERT( type < DWC_LOBBY_MG_NUM );
	// ���łɐݒ�ς݂ł͂Ȃ����`�F�b�N
	GF_ASSERT( cp_sys->mg_data[ type ].mg_flag == TRUE );

	return &cp_sys->mg_data[ type ].mg_data;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����W	�A�b�v�f�[�g	P2P�ڑ����ɍs�������������ɏ���
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_MG_Update( DWC_LOBBYLIB_WK* p_sys )
{
	int i;
	u16 gmt_timelimit;
	
	// �������e�̏ꍇ
	if( p_DWC_LOBBYLIB_WK->mg_myparent == TRUE ){
		DWC_LOBBY_MGDATA* p_mgdata;
		u32 connect_num;

		// �ʐM�l�����擾
		connect_num = mydwc_AnybodyEvalNum();

		GF_ASSERT( connect_num <= 4 );
		
		// �V�K�ڑ��҂������̂ŕ�W�l����ύX���ăf�[�^��]��
		p_mgdata = &p_DWC_LOBBYLIB_WK->mg_data[ p_DWC_LOBBYLIB_WK->mg_myentry ];	// �~�j�Q�[���f�[�^�擾

		if( p_mgdata->mg_data.currentNum != connect_num ){
			p_mgdata->mg_data.currentNum = connect_num;
			// �X�V
			PPW_LobbyUpdateRecruitInfo( &p_mgdata->mg_data );
			DWC_LOBBY_PRINT( "dwc lobby p2p connect chg %d\n", connect_num );
		}
	}

	//  �^�C�����~�b�g�v�Z
	for( i=0; i<DWC_LOBBY_MG_NUM; i++ ){

		// ����GMT���Ԃł̕�W�J�n����̎��ԁ{�o�߃V���N�����̂��čׂ����J�E���g�_�E������
		// GMT���Ԃ��g�p�������̂ł͎��Ԃ̌o�߂̎擾���ׂ����Ȃ��̂ŃV���N�����g�p����
		// �����V���N���������Ə��������̉e�����󂯂�̂ŁA
		// GMT���Ԃ̕������������炻����̎��Ԃ��g�p����
		if( p_DWC_LOBBYLIB_WK->mg_data[ i ].mg_flag == TRUE ){

			if( p_DWC_LOBBYLIB_WK->mg_data[ i ].mg_timelimit > 0 ){
				p_DWC_LOBBYLIB_WK->mg_data[ i ].mg_timelimit --;
			}

			gmt_timelimit = DWC_LOBBY_MG_CalcTimeLimit( p_DWC_LOBBYLIB_WK, i );

			// GMT�̎��Ԃ̂ق����������Ȃ�������
			if( gmt_timelimit < p_DWC_LOBBYLIB_WK->mg_data[ i ].mg_timelimit ){
				p_DWC_LOBBYLIB_WK->mg_data[ i ].mg_timelimit = gmt_timelimit;
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��W�J�n���Ԃ�ݒ肷��
 *	
 *	@param	p_info		��W�f�[�^
 *	@param	cp_time		����
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_MG_SetRecruitStartTime( PPW_LobbyRecruitInfo* p_info, const s64* cp_time )
{
	s64* p_timebuf;
	p_timebuf = (s64*)p_info->paramBuf;
	*p_timebuf = *cp_time;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��W�J�n���Ԃ��擾����
 *	
 *	@param	cp_info		��W�f�[�^
 *	@param	cp_time		����
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_MG_GetRecruitStartTime( const PPW_LobbyRecruitInfo* cp_info, s64* p_time )
{
	const s64* cp_timebuf;
	cp_timebuf = (const s64*)cp_info->paramBuf;
	*p_time = *cp_timebuf;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[��P2P�V�K�ڑ��R�[���o�b�N
 *
 *	@param	aid			AID
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_MG_ConnectCallBack( u16 aid, void* p_work )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[��P2P�ؒf�R�[���o�b�N
 *
 *	@param	aid		AID
 *	@param	p_work	���[�N
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_MG_DisConnectCallBack( u16 aid, void* p_work )
{
	DWC_LOBBYLIB_WK* p_wk;
	p_wk = p_work;

	DWC_LOBBY_PRINT( "dwc lobby p2p disconnect  %d\n", aid );
	
	// �N�����ؒf�����̂ŁA��W�I��
	if( p_wk->mg_myparent == TRUE ){
		DWC_LOBBY_MG_EndRecruit();		// 
	}else{

		// �e�̐ؒf�Ȃ狭���I��
		if( aid == 0 ){
			p_DWC_LOBBYLIB_WK->mg_myend = TRUE;
		}
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�X�P�W���[���̎��Ԃ��擾����
 *
 *	@param	cp_schedule	�X�P�W���[��
 *	@param	event		�C�x���g�ԍ�
 *
 *	@return	��ԍŏ��̊J�n����
 */	
//-----------------------------------------------------------------------------
static u32 DWC_LOBBY_SCHEDULE_GetEventTime( const PPW_LobbySchedule* cp_schedule, PPW_LOBBY_TIME_EVENT event )
{
	u32 data;
	int i;

	data = 0;	// �Ȃ������炨���������A�Ƃ肠�����O������
	for( i=0; i<cp_schedule->scheduleRecordNum; i++ ){
		if( cp_schedule->scheduleRecords[i].event == event ){
			data = cp_schedule->scheduleRecords[i].time;
		}
	}

	return data;
}


//----------------------------------------------------------------------------
/**
 *	@brief	VIP���o�b�t�@	������
 *
 *	@param	p_vip		VIP���[�N
 *	@param	buffnum		�o�b�t�@��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_VIPDATA_Init( DWC_LOBBY_VIP* p_vip, u32 buffnum, u32 heapID )
{
	p_vip->p_vipbuff = sys_AllocMemory( heapID, sizeof(PPW_LobbyVipRecord)*buffnum );
	p_vip->num		 = buffnum;
	p_vip->datanum	 = 0;
}


//----------------------------------------------------------------------------
/**
 *	@brief	VIP���o�b�t�@	�j��
 *
 *	@param	p_vip		VIP���[�N
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_VIPDATA_Exit( DWC_LOBBY_VIP* p_vip )
{
	sys_FreeMemoryEz( p_vip->p_vipbuff );
	p_vip->p_vipbuff = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP���o�b�t�@	�f�[�^��ݒ�
 *
 *	@param	p_vip			VIP���[�N
 *	@param	cp_vipRecords	�f�[�^
 *	@param	num				�f�[�^��
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_VIPDATA_SetData( DWC_LOBBY_VIP* p_vip, const PPW_LobbyVipRecord* cp_vipRecords, u32 num )
{
	u32 datanum;
	
	GF_ASSERT( num < p_vip->num );

	// �p�ӂ����o�b�t�@�����_�E�����[�h�f�[�^�������ꍇ
	// �i�[�ł��邾���i�[����
	if( num < p_vip->num ){
		datanum = num;
	}else{
		datanum = p_vip->num;
	}

	MI_CpuCopy32( cp_vipRecords, p_vip->p_vipbuff, sizeof(PPW_LobbyVipRecord)*datanum );
	p_vip->datanum = datanum;
}


//----------------------------------------------------------------------------
/**
 *	@brief	VIP���o�b�t�@	VIP�`�F�b�N
 *
 *	@param	cp_vip		VIP���[�N
 *	@param	profileid	�v���t�@�C��ID
 *
 *	@retval	TRUE	VIP
 *	@retval	FALSE	�ʏ�̂Ђ�
 */
//-----------------------------------------------------------------------------
static BOOL DWC_LOBBY_VIPDATA_CheckVip( const DWC_LOBBY_VIP* cp_vip, s32 profileid )
{
	int i;

	for( i=0; i<cp_vip->datanum; i++ ){
		if( cp_vip->p_vipbuff[i].profileId == profileid ){
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	VIP���o�b�t�@	�������Ƃ΃L�[�擾
 *
 *	@param	cp_vip		VIP���[�N
 *	@param	profileid	�v���t�@�C��ID
 *
 *	@return	�������ƂΗp�L�[
 */
//-----------------------------------------------------------------------------
static s32 DWC_LOBBY_VIPDATA_GetAikotoba( const DWC_LOBBY_VIP* cp_vip, s32 profileid )
{
	int i;

	for( i=0; i<cp_vip->datanum; i++ ){
		if( cp_vip->p_vipbuff[i].profileId == profileid ){
			return cp_vip->p_vipbuff[i].key;
		}
	}

	return DWC_LOBBY_VIP_KEYNONE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g���		�f�[�^�ݒ�
 *
 *	@param	p_wk			���[�N
 *	@param	cp_question		�ݒ�f�[�^
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_ANKETO_Set( DWC_ANKETO_DATA* p_wk, const PPW_LobbyQuestionnaire* cp_question )
{
	memcpy( &p_wk->question, cp_question, sizeof(PPW_LobbyQuestionnaire) );

#ifdef DWC_LOBBY_ANKETO_DRAW
	DWC_LOBBY_PRINT( "nowdata \n" );
	DWC_LOBBY_DEBUG_Printf( &p_wk->question.currentQuestionnaireRecord );

	DWC_LOBBY_PRINT( "lastdata \n" );
	DWC_LOBBY_DEBUG_Printf( &p_wk->question.lastQuestionnaireRecord );

	DWC_LOBBY_PRINT( "lastresult[" );
	{
		int i;

		for( i=0; i<PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM; i++ ){
			DWC_LOBBY_PRINT( " %d", p_wk->question.lastMultiResult[ i ] );
		}
	}
	DWC_LOBBY_PRINT( "]\n" );
#endif
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g���@�f�o�b�N�\��
 *
 *	@param	cp_data		���[�N
 */
//-----------------------------------------------------------------------------
static void DWC_LOBBY_DEBUG_Printf( const PPW_LobbyQuestionnaireRecord* cp_data )
{
	int i, j;
	
	OS_TPrintf( "questionSerialNo = %d\n", cp_data->questionSerialNo );
	OS_TPrintf( "questionNo = %d\n", cp_data->questionNo );

	OS_TPrintf( "questionSentence[" );
	for( i=0; i<PPW_LOBBY_MAX_QUESTION_SENTENCE_LENGTH; i++ ){
		OS_TPrintf( " %d", cp_data->questionSentence[i] );
	}
	OS_TPrintf( "]\n" );
	for( i=0; i<PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM; i++ ){
		OS_TPrintf( "answer[%d][", i );
		for( i=0; i<PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM; i++ ){
			OS_TPrintf( " %d", cp_data->answer[i][j] );
		}
		OS_TPrintf( "]\n" );
	}
	OS_TPrintf( "multiLanguageSummarizeFlags[" );
	for( i=0; i<12; i++ ){
		OS_TPrintf( " %d", cp_data->multiLanguageSummarizeFlags[i] );
	}
	OS_TPrintf( "]\n" );
}

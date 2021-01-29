//==============================================================================
/**
 * @file	email_enter.c
 * @brief	E���[���o�^���������
 * @author	matsuda
 * @date	2007.10.17(��)
 */
//==============================================================================
#include "common.h"
#include <dwc.h>
#include "libdpw/dpw_tr.h"
#include "system/procsys.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "system/wipe.h"
#include "system/fontproc.h"
#include "system/lib_pack.h"
#include "system/lib_pack.h"
#include "system/fontoam.h"
#include "system/window.h"
#include "gflib/touchpanel.h"
#include "system/bmp_menu.h"
#include "system/snd_tool.h"
#include "gflib/strbuf_family.h"
#include "savedata/wifilist.h"
#include "savedata/zukanwork.h"
#include "communication/communication.h"


//#include "application/worldtrade.h"
//#include "worldtrade_local.h"

#include "msgdata/msg_wifi_lobby.h"
#include "msgdata/msg_wifi_gtc.h"
#include "msgdata/msg_wifi_system.h"



//#include "worldtrade.naix"			// �O���t�B�b�N�A�[�J�C�u��`
#include "../wifi_p2pmatch/wifip2pmatch.naix"			// �O���t�B�b�N�A�[�J�C�u��`

#include "application/email_main.h"
#include "msgdata/msg_email.h"
#include "system/bmp_list.h"
#include "system/pmfprint.h"
#include "savedata/config.h"
#include "savedata\system_data.h"
#include "savedata\email_savedata.h"
#include "email_snd_def.h"

#include "application/connect_anm.h"

#include "system/bmp_cursor.h"
#include "email_gra.naix"

//==============================================================================
//	�萔��`
//==============================================================================
///���E�����Ɠ����ɂ��Ă���(WORLDTRADE_WORDSET_BUFLEN)
#define EMAIL_WORDSET_BUFLEN	( 64 )
// ��b�E�C���h�E������o�b�t�@��
#define TALK_MESSAGE_BUF_NUM	( 90*2 )
#define DWC_ERROR_BUF_NUM		(16*8*2)

#define EMAIL_BG_CUR_PAL	( 9 )
#define EMAIL_MESFRAME_PAL	 ( 10 )
#define EMAIL_MENUFRAME_PAL ( 11 )
#define EMAIL_MESFRAME_CHR	 (  1 )
#define EMAIL_MENUFRAME_CHR ( EMAIL_MESFRAME_CHR + TALK_WIN_CGX_SIZ )
#define EMAIL_TALKFONT_PAL	 ( 13 )

// �P�b�҂p�̒�`
#define WAIT_ONE_SECONDE_NUM	( 30 )

//���b�Z�[�W�ꊇ�`��̏ꍇ�ɃZ�b�g���郁�b�Z�[�Windex
#define EMAIL_MSG_NO_WAIT		(0xff)


///E���[���F�؃G���[�����������ۂ̃G���[���b�Z�[�W�R�[�h
enum{
	//�l���o�^���̃G���[
	EMAIL_ENTRY_ERROR_SEND = -5000,
	EMAIL_ENTRY_ERROR_SENDFAILURE = -5001,
	EMAIL_ENTRY_ERROR_SUCCESS = -5002,
	EMAIL_ENTRY_ERROR_FAILURE = -5003,
	EMAIL_ENTRY_ERROR_INVALIDPARAM = -5004,
	EMAIL_ENTRY_ERROR_SERVERSTATE = -5005,
	
	//�o�^�R�[�h�F�؎��̃G���[
	EMAIL_PASS_ERROR_SEND = -5006,
	EMAIL_PASS_ERROR_SENDFAILURE = -5007,
	EMAIL_PASS_ERROR_SUCCESS = -5008,
	EMAIL_PASS_ERROR_FAILURE = -5009,
	EMAIL_PASS_ERROR_INVALIDPARAM = -5010,
	EMAIL_PASS_ERROR_SERVERSTATE = -5011,
};

///�����^�C���A�E�g�܂ł̎���
#define TIMEOUT_TIME			(30*60*2)	//2��


//==============================================================================
//	�\���̒�`
//==============================================================================
typedef struct{
	EMAIL_SYSWORK *esys;

	GF_BGL_INI		*bgl;								// GF_BGL_INI
	
	int				subprocess_seq;						// �T�u�v���O�����V�[�P���XNO
	int				subprocess_nextseq;					// �T�u�v���O����NEXT�V�[�P���XNO

	int				ConnectErrorNo;						// DWC�E�܂��̓T�[�o�[����̃G���[
	int				ErrorRet;
	int				ErrorCode;
	int				ErrorType;

	// �`��܂��̃��[�N�i���BMP�p�̕��������j
	WORDSET			*WordSet;							// ���b�Z�[�W�W�J�p���[�N�}�l�[�W���[
	MSGDATA_MANAGER *MsgManager;						// ���O���̓��b�Z�[�W�f�[�^�}�l�[�W���[
	MSGDATA_MANAGER *MonsNameManager;					// �|�P���������b�Z�[�W�f�[�^�}�l�[�W���[
	MSGDATA_MANAGER *LobbyMsgManager;					// ���O���̓��b�Z�[�W�f�[�^�}�l�[�W���[
	MSGDATA_MANAGER *SystemMsgManager;					// Wifi�V�X�e�����b�Z�[�W�f�[�^
	MSGDATA_MANAGER *EmailMsgManager;					// E���[�����b�Z�[�W�f�[�^�}�l�[�W���[
	STRBUF			*TalkString;						// ��b���b�Z�[�W�p
	STRBUF			*TitleString;						// �^�C�g�����b�Z�[�W�p
	STRBUF			*ErrorString;
	int				MsgIndex;							// �I�����o�p���[�N


	// BMPWIN�`�����
	GF_BGL_BMPWIN			MsgWin;					// ��b�E�C���h�E
	GF_BGL_BMPWIN			TitleWin;				// �u���R�[�h�R�[�i�[�@�ڂ��イ���イ�I�v�Ȃ�
	GF_BGL_BMPWIN			SubWin;					// �u���R�[�h�R�[�i�[�@�ڂ��イ���イ�I�v�Ȃ�
	GF_BGL_BMPWIN			list_bmpwin;			///<���j���[���X�g�쐬�pBMPWIN

	BMPMENU_WORK			*YesNoMenuWork;
	void*					timeWaitWork;			// ��b�E�C���h�E���A�C�R�����[�N

	int						wait;

	
	int local_seq;
	int local_wait;
	int local_work;
	BMP_MENULIST_DATA *listmenu;					///<E���[�����j���[
	BMPLIST_WORK *lw;
	
	// ���p�K��p
	int 				sub_seq;
	int					info_pos;
	int					info_cur_pos;
	int					info_end;
	int					info_param;
	int					info_wait;
	int					info_count;
	GF_BGL_BMPWIN		info_win;
	GF_BGL_BMPWIN		info_win2;
	BMPCURSOR*			info_cur;
	
	s32 timeout_count;				///<�^�C���A�E�g�J�E���^
	
	//WIFI�ڑ�BG�p���b�g�A�j������\���̂ւ̃|�C���^
	CONNECT_BG_PALANM cbp;
}EMAIL_MENU_WORK;


//============================================================================================
//	�v���g�^�C�v�錾
//============================================================================================
PROC_RESULT EmailMenu_Enter_Init( PROC * proc, int * seq );
PROC_RESULT EmailMenu_Enter_Main( PROC * proc, int * seq );
PROC_RESULT EmailMenu_Enter_End(PROC *proc, int *seq);
static void VBlankFunc( void * work );
static void EmailMenu_VramBankSet(GF_BGL_INI *bgl);
static void BgExit( GF_BGL_INI * ini );
static void BgGraphicSet( EMAIL_MENU_WORK * wk );
static void BmpWinInit( EMAIL_MENU_WORK *wk );
static void BmpWinDelete( EMAIL_MENU_WORK *wk );
static void Email_SetNextSeq( EMAIL_MENU_WORK *wk, int to_seq, int next_seq );
static void Email_TimeIconAdd( EMAIL_MENU_WORK *wk );
static void Email_TimeIconDel( EMAIL_MENU_WORK *wk );
static BMPMENU_WORK *Email_BmpWinYesNoMake( GF_BGL_INI *bgl, int y, int yesno_bmp_cgx );
static int Enter_MenuList( EMAIL_MENU_WORK *wk );
static int Enter_AddressCheckProcChange(EMAIL_MENU_WORK *wk);
static int Enter_RecvSelectYesNo(EMAIL_MENU_WORK *wk);
static int Enter_EmailDataInitializeYesNo(EMAIL_MENU_WORK *wk);
static int Enter_AddressEntryStart(EMAIL_MENU_WORK *wk);
static int Enter_AddressInputProcChange(EMAIL_MENU_WORK *wk);
static int Enter_AddressReturn(EMAIL_MENU_WORK *wk);
static int Enter_Start( EMAIL_MENU_WORK *wk);
static int Enter_ConnectYesNoSelect( EMAIL_MENU_WORK *wk );
static int Enter_ForceEndStart( EMAIL_MENU_WORK *wk ) ;
static int Enter_ForceEnd( EMAIL_MENU_WORK *wk );
static int Enter_ForceEndMessage( EMAIL_MENU_WORK *wk );
static int Enter_InternetConnect( EMAIL_MENU_WORK *wk );
static int Enter_InternetConnectWait( EMAIL_MENU_WORK *wk );
static int Enter_WifiConnectionLogin( EMAIL_MENU_WORK *wk );
static int Enter_WifiConnectionLoginWait( EMAIL_MENU_WORK *wk );
static int Enter_DpwTrInit( EMAIL_MENU_WORK *wk );
static int Enter_ServerStart( EMAIL_MENU_WORK *wk );
static int Enter_ServerResult( EMAIL_MENU_WORK *wk );
static int Enter_AuthMailWait(EMAIL_MENU_WORK *wk);
static int Enter_AuthInputProcChange(EMAIL_MENU_WORK *wk);
static int Enter_AuthMailEndYesNo(EMAIL_MENU_WORK *wk);
static int Enter_AuthenticateReturn(EMAIL_MENU_WORK *wk);
static int Enter_PasswordSetup(EMAIL_MENU_WORK *wk);
static int Enter_PasswordSetupCheck(EMAIL_MENU_WORK *wk);
static int Enter_Save(EMAIL_MENU_WORK *wk);
static int Enter_CleanupInet( EMAIL_MENU_WORK *wk );
static int Enter_DwcErrorPrint( EMAIL_MENU_WORK *wk );
static int Enter_ErrorPadWait( EMAIL_MENU_WORK *wk );
static int Enter_End( EMAIL_MENU_WORK *wk);
static int Enter_YesNo( EMAIL_MENU_WORK *wk);
static int Enter_ServerServiceError( EMAIL_MENU_WORK *wk );
static int Enter_ServerServiceEnd( EMAIL_MENU_WORK *wk );
static int Enter_MessageWait( EMAIL_MENU_WORK *wk );
static int Enter_MessageWait1Second( EMAIL_MENU_WORK *wk );
static int Enter_MessageWaitYesNoStart(EMAIL_MENU_WORK *wk);
static void Enter_MessagePrint( EMAIL_MENU_WORK *wk, MSGDATA_MANAGER *msgman, int msgno, int wait, u16 dat );
static int Enter_MessagePrintEndCheck(int msg_index);
static int printCommonFunc( GF_BGL_BMPWIN *win, STRBUF *strbuf, int x, int flag, GF_PRINTCOLOR color, int font );
void Email_TalkPrint( GF_BGL_BMPWIN *win, STRBUF *strbuf, int x, int y, int flag, GF_PRINTCOLOR color );
static void _systemMessagePrint( EMAIL_MENU_WORK *wk, int msgno );
static void errorDisp(EMAIL_MENU_WORK* wk, int type, int code);
static void EmailMenuListAllHeader_CursorCallback(BMPLIST_WORK * wk, u32 param, u8 mode);
static void EmailMenuListNoDataHeader_CursorCallback(BMPLIST_WORK * wk, u32 param, u8 mode);


enum{
	ENTER_MENU_LIST,
	ENTER_ADDRESS_CHECK_PROC_CHANGE,
	ENTER_RECV_SELECT_YESNO,
	ENTER_EMAIL_DATA_INITIALIZE_YESNO,
	ENTER_ADDRESS_ENTRY_START,
	ENTER_ADDRESS_INPUT_PROC_CHANGE,
	ENTER_ADDRESS_RETURN,
	ENTER_START,
	ENTER_CONNECT_YESNO_SELECT,
	ENTER_FORCE_END_START,
	ENTER_FORCE_END,
	ENTER_FORCE_END_MESSAGE,
	ENTER_INTERNET_CONNECT,
	ENTER_INTERNET_CONNECT_WAIT,
	ENTER_WIFI_CONNECTION_LOGIN,
	ENTER_WIFI_CONNECTION_LOGIN_WAIT,
	ENTER_DPWTR_INIT,
	ENTER_SERVER_START,
	ENTER_SERVER_RESULT,
	ENTER_AUTH_MAIL_WAIT,
	ENTER_AUTH_INPUT_PROC_CHANGE,
	ENTER_AUTH_MAIL_END_YESNO,
	ENTER_AUTHENTICATE_RETURN,
	ENTER_PASSWORD_SETUP,
	ENTER_PASSWORD_SETUP_CHECK,
	ENTER_SAVE,
	ENTER_CLEANUP_INET,
	ENTER_DWC_ERROR_PRINT,
	ENTER_ERROR_PAD_WAIT,
	ENTER_END,
	ENTER_CONNECT_END,		//�ʐM�����܂܏I��
	ENTER_YESNO,
	ENTER_SERVER_SERVICE_ERROR,
	ENTER_SERVER_SERVICE_END,
	ENTER_MES_WAIT,
	ENTER_MES_WAIT_1_SECOND,
	ENTER_MES_WAIT_YESNO_START,
};

static int (*Functable[])( EMAIL_MENU_WORK *wk ) = {
	Enter_MenuList,						//ENTER_MENU_LIST,
	Enter_AddressCheckProcChange,		//ENTER_ADDRESS_CHECK_PROC_CHANGE
	Enter_RecvSelectYesNo,				//ENTER_RECV_SELECT_YESNO
	Enter_EmailDataInitializeYesNo,		//ENTER_EMAIL_DATA_INITIALIZE_YESNO
	Enter_AddressEntryStart,			//ENTER_ADDRESS_ENTRY_START
	Enter_AddressInputProcChange,		//ENTER_ADDRESS_INPUT_PROC_CHANGE
	Enter_AddressReturn,				//ENTER_ADDRESS_RETURN
	Enter_Start,                        //ENTER_START,
	Enter_ConnectYesNoSelect,           //ENTER_CONNECT_YESNO_SELECT,
	Enter_ForceEndStart,                //ENTER_FORCE_END_START,
	Enter_ForceEnd,                     //ENTER_FORCE_END,
	Enter_ForceEndMessage,              //ENTER_FORCE_END_MESSAGE,
	Enter_InternetConnect,              //ENTER_INTERNET_CONNECT,
	Enter_InternetConnectWait,          //ENTER_INTERNET_CONNECT_WAIT,
	Enter_WifiConnectionLogin,          //ENTER_WIFI_CONNECTION_LOGIN,
	Enter_WifiConnectionLoginWait,      //ENTER_WIFI_CONNECTION_LOGIN_WAIT,
	Enter_DpwTrInit,                    //ENTER_DPWTR_INIT,
	Enter_ServerStart,                  //ENTER_SERVER_START,
	Enter_ServerResult,                 //ENTER_SERVER_RESULT,
	Enter_AuthMailWait,                 //ENTER_AUTH_MAIL_WAIT,
	Enter_AuthInputProcChange,          //ENTER_AUTH_INPUT_PROC_CHANGE,
	Enter_AuthMailEndYesNo,             //ENTER_AUTH_MAIL_END_YESNO,
	Enter_AuthenticateReturn,           //ENTER_AUTHENTICATE_RETURN,
	Enter_PasswordSetup,                //ENTER_PASSWORD_SETUP,
	Enter_PasswordSetupCheck,           //ENTER_PASSWORD_SETUP_CHECK,
	Enter_Save,                         //ENTER_SAVE,
	Enter_CleanupInet,                  //ENTER_CLEANUP_INET,
	Enter_DwcErrorPrint,                //ENTER_DWC_ERROR_PRINT,
	Enter_ErrorPadWait,                 //ENTER_ERROR_PAD_WAIT,
	Enter_End,                          //ENTER_END,
	Enter_End,                          //ENTER_CONNECT_END,
	Enter_YesNo,                        //ENTER_YESNO,
	Enter_ServerServiceError,           //ENTER_SERVER_SERVICE_ERROR,
	Enter_ServerServiceEnd,             //ENTER_SERVER_SERVICE_END,
	Enter_MessageWait,                  //ENTER_MES_WAIT,
	Enter_MessageWait1Second,           //ENTER_MES_WAIT_1_SECOND,
	Enter_MessageWaitYesNoStart,        //ENTER_MES_WAIT_YESNO_START,
};


//--------------------------------------------------------------
//	���j���[���X�g�F�S�I�[�v��
//--------------------------------------------------------------
typedef struct{
	u32 str_id;
	u32 param;
}EMAIL_BMPMENULIST;

///E���[���̃g�b�v���j���[�̍���
static const EMAIL_BMPMENULIST EmailMenuList_All[] = {
	{ msg_email_list_001, ENTER_ADDRESS_CHECK_PROC_CHANGE },		//�m�F
//	{ msg_email_list_002, ENTER_ADDRESS_ENTRY_START },		//�A�h���X�ݒ�
	{ msg_email_list_003, ENTER_RECV_SELECT_YESNO },		//��M�ݒ�
	{ msg_email_list_004, ENTER_EMAIL_DATA_INITIALIZE_YESNO },		//����
	{ msg_email_list_005, ENTER_END },			//���ǂ�
};

#define LIST_MENU_ALL_MAX		(NELEMS(EmailMenuList_All))

///E���[����ʁF�g�b�v���j���[�̃��X�g
static const BMPLIST_HEADER EmailMenuListAllHeader = {
	NULL,			// �\�������f�[�^�|�C���^
	EmailMenuListAllHeader_CursorCallback,		// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					// ���\�����Ƃ̃R�[���o�b�N�֐�
	NULL,
	LIST_MENU_ALL_MAX,	// ���X�g���ڐ�
	LIST_MENU_ALL_MAX,						// �\���ő區�ڐ�
	0,						// ���x���\���w���W
	8,						// ���ڕ\���w���W
	0,						// �J�[�\���\���w���W
	0,						// �\���x���W
	FBMP_COL_BLACK,			// �����F
	FBMP_COL_WHITE,			// �w�i�F
	FBMP_COL_BLK_SDW,		// �����e�F
	0,						// �����Ԋu�w
	16,						// �����Ԋu�x
	BMPLIST_NO_SKIP,		// �y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,				// �����w��
	0,						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};

static const u8 EmailMenuBmpSize[4] = {
	13, 7, 18, 10-2,	//X, Y, SX, SY
};

//--------------------------------------------------------------
//	���j���[���X�g�F�A�h���X�o�^���Ă��Ȃ��ꍇ
//--------------------------------------------------------------
///E���[���̃g�b�v���j���[�̍���
static const EMAIL_BMPMENULIST EmailMenuList_NoData[] = {
	{ msg_email_list_002, ENTER_ADDRESS_ENTRY_START },		//�A�h���X�ݒ�
	{ msg_email_list_005, ENTER_END },			//���ǂ�
};

#define LIST_MENU_NODATA_MAX		(NELEMS(EmailMenuList_NoData))

///E���[����ʁF�g�b�v���j���[�̃��X�g
static const BMPLIST_HEADER EmailMenuListNoDataHeader = {
	NULL,			// �\�������f�[�^�|�C���^
	EmailMenuListNoDataHeader_CursorCallback,	// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					// ���\�����Ƃ̃R�[���o�b�N�֐�
	NULL,
	LIST_MENU_NODATA_MAX,	// ���X�g���ڐ�
	LIST_MENU_NODATA_MAX,						// �\���ő區�ڐ�
	0,						// ���x���\���w���W
	8,						// ���ڕ\���w���W
	0,						// �J�[�\���\���w���W
	0,						// �\���x���W
	FBMP_COL_BLACK,			// �����F
	FBMP_COL_WHITE,			// �w�i�F
	FBMP_COL_BLK_SDW,		// �����e�F
	0,						// �����Ԋu�w
	16,						// �����Ԋu�x
	BMPLIST_NO_SKIP,		// �y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,				// �����w��
	0,						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};

static const u8 EmailMenuNoDataBmpSize[4] = {
	13, 13, 18, 4,	//X, Y, SX, SY
};

//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
//	�͂��E�������@�E�B���h�E
//--------------------------------------------------------------
// �͂��E������
#define	BMP_YESNO_PX	( 23 )
#define	BMP_YESNO_PY	( 13 )
#define	BMP_YESNO_SX	( 7 )
#define	BMP_YESNO_SY	( 4 )
#define	BMP_YESNO_PAL	( 13 )

// �͂��E������(�E�C���h�E�p�j
static const BMPWIN_DAT YesNoBmpWin = {
	GF_BGL_FRAME0_M, BMP_YESNO_PX, BMP_YESNO_PY,
	BMP_YESNO_SX, BMP_YESNO_SY, BMP_YESNO_PAL, 
	0, //��Ŏw�肷��
};

// �͂��E�������E�C���h�E��Y���W
#define	EMAIL_YESNO_PY2	( 13 )		// ��b�E�C���h�E���Q�s�̎�
#define	EMAIL_YESNO_PY1	( 15 )		// ��b�E�C���h�E���P�s�̎�


//==============================================================================
//	�萔��`
//==============================================================================
///�T�u�V�[�P���X�̖߂�l
enum{
	SUBSEQ_CONTINUE,	///<�p��
	SUBSEQ_END,			///<�I��
};


//============================================================================================
//	�v���Z�X�֐�
//============================================================================================
//==============================================================================
/**
 * $brief   ���E�����������ʏ�����
 *
 * @param   wk		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailMenu_Enter_Init( PROC * proc, int * seq )
{
	EMAIL_MENU_WORK *wk;

	sys_VBlankFuncChange(NULL, NULL);	// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	GX_SetVisiblePlane(0);
	GXS_SetVisiblePlane(0);
	GX_SetVisibleWnd(GX_WNDMASK_NONE);
	GXS_SetVisibleWnd(GX_WNDMASK_NONE);
	G2_BlendNone();
	G2S_BlendNone();

	//E���[����ʗp�q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_EMAIL, 0x70000 );
	
	wk = PROC_AllocWork(proc, sizeof(EMAIL_MENU_WORK), HEAPID_EMAIL );
	MI_CpuClear8(wk, sizeof(EMAIL_MENU_WORK));

	wk->esys = PROC_GetParentWork(proc);

	wk->bgl = GF_BGL_BglIniAlloc( HEAPID_EMAIL );

	initVramTransferManagerHeap(64, HEAPID_EMAIL);

	sys_KeyRepeatSpeedSet( SYS_KEYREPEAT_SPEED_DEF, SYS_KEYREPEAT_WAIT_DEF );

	//VRAM���蓖�Đݒ�
	EmailMenu_VramBankSet(wk->bgl);

	// �^�b�`�p�l���V�X�e��������
	InitTPSystem();
	InitTPNoBuff(4);
	
	//���b�Z�[�W�}�l�[�W���쐬
	wk->WordSet    		 = WORDSET_CreateEx( 11, EMAIL_WORDSET_BUFLEN, HEAPID_EMAIL );
	wk->MsgManager       = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wifi_gtc_dat, HEAPID_EMAIL );
	wk->LobbyMsgManager  = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wifi_lobby_dat, HEAPID_EMAIL );
	wk->SystemMsgManager = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wifi_system_dat, HEAPID_EMAIL );
	wk->MonsNameManager  = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_monsname_dat, HEAPID_EMAIL );
	wk->EmailMsgManager  = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_email_dat, HEAPID_EMAIL );

	// ������o�b�t�@�쐬
	wk->TalkString  = STRBUF_Create( TALK_MESSAGE_BUF_NUM, HEAPID_EMAIL );
	wk->ErrorString = STRBUF_Create( DWC_ERROR_BUF_NUM,    HEAPID_EMAIL );
	wk->TitleString = MSGMAN_AllocString( wk->MsgManager, msg_gtc_01_032 );

	// BG�O���t�B�b�N�]��
	BgGraphicSet( wk );

	// BMPWIN�m��
	BmpWinInit( wk );

	switch(Email_RecoveryMenuModeGet(wk->esys)){
	case ENTER_INTERNET_CONNECT:	//GSID���擾���Ă���
		if(!DWC_CheckInet() && mydwc_checkMyGSID(wk->esys->savedata) == TRUE){
			// ����wifi�ڑ��̍ۂ͖������Őڑ���
			//if(mydwc_checkMyGSID(wk->esys->savedata) == FALSE){
			// WIFI���������J�n
			//Enter_MessagePrint( wk, wk->LobbyMsgManager, msg_wifilobby_002, 1, 0x0f0f );
			//Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_INTERNET_CONNECT );
			//Email_TimeIconAdd(wk);
			wk->subprocess_seq = ENTER_INTERNET_CONNECT;
			// �ʐM�G���[�Ǘ��̂��߂ɒʐM���[�`����ON
			CommStateWifiEMailStart( wk->esys->savedata );	// �G���[��^�C�g���ɖ߂�悤�ɐ�p�̊֐����쐬 080603tomoya 
//			CommStateWifiDPWStart( wk->esys->savedata );
			// Wifi�ʐM�A�C�R��
		    WirelessIconEasy();
		}else{
			wk->subprocess_seq = ENTER_MENU_LIST;
		}
		break;
	case ENTER_AUTHENTICATE_RETURN:
		// Wifi�ʐM�A�C�R��
		wk->timeout_count = 0;
	    WirelessIconEasy();
	    wk->subprocess_seq = Email_RecoveryMenuModeGet(wk->esys);
		break;
	default:
		wk->subprocess_seq = Email_RecoveryMenuModeGet(wk->esys);
		break;
	}
	Email_RecoveryMenuModeSet(wk->esys, 0);	//�l�N���A

	// ���C�v�t�F�[�h�J�n
	WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK, 
		WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_EMAIL );

	// BG�ʕ\��ON
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG0, VISIBLE_ON );
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG1, VISIBLE_ON );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_OBJ, VISIBLE_ON );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );

	//���C����ʐݒ�
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();

	MsgPrintSkipFlagSet(MSG_SKIP_ON);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);

	sys_VBlankFuncChange(VBlankFunc, wk);

	return PROC_RES_FINISH;
}


//==============================================================================
/**
 * $brief   ���E�����������ʃ��C��
 *
 * @param   wk		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailMenu_Enter_Main( PROC * proc, int * seq )
{
	EMAIL_MENU_WORK * wk  = PROC_GetWork( proc );
	int ret, temp_subprocess_seq;
	enum{
		SEQ_INIT,
		SEQ_MAIN,
		SEQ_END,
	};
	
	switch(*seq){
	case SEQ_INIT:
		if(WIPE_SYS_EndCheck() == TRUE){
			*seq = SEQ_MAIN;
		}
		break;
	case SEQ_MAIN:
		// �V�[�P���X�J�ڂŎ��s
		temp_subprocess_seq = wk->subprocess_seq;
		ret = (*Functable[wk->subprocess_seq])( wk );
		if(temp_subprocess_seq != wk->subprocess_seq){
			wk->local_seq = 0;
			wk->local_work = 0;
			wk->local_wait = 0;
		}
		if(ret == SUBSEQ_END){
			*seq = SEQ_END;
		}
		break;
	case SEQ_END:
		if(WIPE_SYS_EndCheck() == TRUE){
			return PROC_RES_FINISH;
		}
		break;
	}
	
	ConnectBGPalAnm_Main(&wk->cbp);
	
	return PROC_RES_CONTINUE;
}


//==============================================================================
/**
 * $brief   ���E�����������ʏI��
 *
 * @param   wk		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailMenu_Enter_End(PROC *proc, int *seq)
{
	EMAIL_MENU_WORK * wk  = PROC_GetWork( proc );

	ConnectBGPalAnm_End(&wk->cbp);
	
	// ���b�Z�[�W�}�l�[�W���[�E���[�h�Z�b�g�}�l�[�W���[���
	MSGMAN_Delete( wk->EmailMsgManager );
	MSGMAN_Delete( wk->MonsNameManager );
	MSGMAN_Delete( wk->SystemMsgManager );
	MSGMAN_Delete( wk->LobbyMsgManager );
	MSGMAN_Delete( wk->MsgManager );
	WORDSET_Delete( wk->WordSet );

	STRBUF_Delete( wk->TitleString ); 
	STRBUF_Delete( wk->ErrorString ); 
	STRBUF_Delete( wk->TalkString ); 
	
	BmpWinDelete( wk );
	
	// BG_SYSTEM���
	sys_FreeMemoryEz( wk->bgl );
	BgExit( wk->bgl );

	sys_VBlankFuncChange( NULL, NULL );		// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	//Vram�]���}�l�[�W���[�폜
	DellVramTransferManager();

	StopTP();		//�^�b�`�p�l���̏I��

	MsgPrintSkipFlagSet(MSG_SKIP_OFF);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);

	WirelessIconEasyEnd();

	PROC_FreeWork( proc );				// PROC���[�N�J��
	sys_DeleteHeap( HEAPID_EMAIL );

	return PROC_RES_FINISH;
}

//--------------------------------------------------------------------------------------------
/**
 * VBlank�֐�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VBlankFunc( void * work )
{
	EMAIL_MENU_WORK *wk = work;

	// �Z���A�N�^�[Vram�]���}�l�[�W���[���s
	DoVramTransferManager();

	// �����_�����LOAM�}�l�[�W��Vram�]��
	REND_OAMTrans();
	
	GF_BGL_VBlankFunc(wk->bgl);
	
	ConnectBGPalAnm_VBlank(&wk->cbp);

	OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}

//--------------------------------------------------------------
/**
 * @brief   Vram�o���N�ݒ���s��
 *
 * @param   bgl		BGL�f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void EmailMenu_VramBankSet(GF_BGL_INI *bgl)
{
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	
	//VRAM�ݒ�
	{
		GF_BGL_DISPVRAM vramSetTable = {
			GX_VRAM_BG_128_A,				// ���C��2D�G���W����BG
			GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g

			GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
			GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g

			GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
			GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g

			GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
			GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g

			GX_VRAM_TEX_0_B,				// �e�N�X�`���C���[�W�X���b�g
			GX_VRAM_TEXPLTT_01_FG			// �e�N�X�`���p���b�g�X���b�g
		};
		GF_Disp_SetBank( &vramSetTable );

		//VRAM�N���A
		MI_CpuClear32((void*)HW_BG_VRAM, HW_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
	}

	// BG SYSTEM
	{
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	//���C����ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME0_M	�e�L�X�g��
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
				2, 0, 0, FALSE
			},
			///<FRAME1_M	�w�i
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
				3, 0, 0, FALSE
			},
			///<FRAME2_M	���ӕ���
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
				1, 0, 0, FALSE
			},
			///<FRAME3_M	�J�[�\��
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x18000, GX_BG_EXTPLTT_01,
				0, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, GF_BGL_FRAME0_M, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, GF_BGL_FRAME0_M );
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME0_M, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME0_M, GF_BGL_SCROLL_Y_SET, 0);
		
		GF_BGL_BGControlSet(bgl, GF_BGL_FRAME1_M, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, GF_BGL_FRAME1_M );
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME1_M, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME1_M, GF_BGL_SCROLL_Y_SET, 0);

		GF_BGL_BGControlSet(bgl, GF_BGL_FRAME2_M, &TextBgCntDat[2], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, GF_BGL_FRAME2_M );
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME2_M, GF_BGL_SCROLL_Y_SET, 0);
		
		GF_BGL_BGControlSet(bgl, GF_BGL_FRAME3_M, &TextBgCntDat[3], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, GF_BGL_FRAME3_M );
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME3_M, GF_BGL_SCROLL_Y_SET, 0);
	}
	//�T�u��ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME0_S	�e�L�X�g��
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
				0, 0, 0, FALSE
			},
			///<FRAME1_S	�w�i
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
				2, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, GF_BGL_FRAME0_S, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, GF_BGL_FRAME0_S );
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME0_S, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME0_S, GF_BGL_SCROLL_Y_SET, 0);
		
		GF_BGL_BGControlSet(bgl, GF_BGL_FRAME1_S, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, GF_BGL_FRAME1_S );
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME1_S, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME1_S, GF_BGL_SCROLL_Y_SET, 0);
	}

	GF_BGL_ClearCharSet( GF_BGL_FRAME0_M, 32, 0, HEAPID_EMAIL );
	GF_BGL_ClearCharSet( GF_BGL_FRAME0_S, 32, 0, HEAPID_EMAIL );
}

//--------------------------------------------------------------------------------------------
/**
 * BG���
 *
 * @param	ini		BGL�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgExit( GF_BGL_INI * ini )
{
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME1_S );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME0_S );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME3_M );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME2_M );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME1_M );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME0_M );

}


//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^�Z�b�g
 *
 * @param	wk		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicSet( EMAIL_MENU_WORK * wk )
{
	GF_BGL_INI *bgl = wk->bgl;
	ARCHANDLE* p_handle;

	p_handle = ArchiveDataHandleOpen( ARC_WIFIP2PMATCH_GRA, HEAPID_EMAIL );

	// �㉺��ʂa�f�p���b�g�]��
	ArcUtil_HDL_PalSet(    p_handle, NARC_wifip2pmatch_conect_NCLR, PALTYPE_MAIN_BG, 0, 0,  HEAPID_EMAIL);
	ArcUtil_HDL_PalSet(    p_handle, NARC_wifip2pmatch_conect_NCLR, PALTYPE_SUB_BG,  0, 0,  HEAPID_EMAIL);
	
	// ��b�t�H���g�p���b�g�]��
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, EMAIL_TALKFONT_PAL*0x20, HEAPID_EMAIL );
	TalkFontPaletteLoad( PALTYPE_SUB_BG,  EMAIL_TALKFONT_PAL*0x20, HEAPID_EMAIL );

	// ��b�E�C���h�E�O���t�B�b�N�]��
	TalkWinGraphicSet(	bgl, GF_BGL_FRAME0_M, EMAIL_MESFRAME_CHR, 
						EMAIL_MESFRAME_PAL,  CONFIG_GetWindowType(wk->esys->config), HEAPID_EMAIL );

	MenuWinGraphicSet(	bgl, GF_BGL_FRAME0_M, EMAIL_MENUFRAME_CHR,
						EMAIL_MENUFRAME_PAL, 0, HEAPID_EMAIL );
						
	MenuWinGraphicSet(	bgl, GF_BGL_FRAME2_M, EMAIL_MENUFRAME_CHR,
						EMAIL_MENUFRAME_PAL, 0, HEAPID_EMAIL );




	// ���C�����BG1�L�����]��
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_conect_NCGR, bgl, GF_BGL_FRAME1_M, 0, 0, 0, HEAPID_EMAIL);

	// ���C�����BG1�X�N���[���]��
	ArcUtil_HDL_ScrnSet(   p_handle, NARC_wifip2pmatch_conect_01_NSCR, bgl, GF_BGL_FRAME1_M, 0, 32*24*2, 0, HEAPID_EMAIL);



	// �T�u���BG1�L�����]��
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_conect_sub_NCGR, bgl, GF_BGL_FRAME1_S, 0, 0, 0, HEAPID_EMAIL);

	// �T�u���BG1�X�N���[���]��
	ArcUtil_HDL_ScrnSet(   p_handle, NARC_wifip2pmatch_conect_sub_NSCR, bgl, GF_BGL_FRAME1_S, 0, 32*24*2, 0, HEAPID_EMAIL);

	GF_BGL_BackGroundColorSet( GF_BGL_FRAME0_M, 0 );
	GF_BGL_BackGroundColorSet( GF_BGL_FRAME0_S, 0 );
	
	//Wifi�ڑ�BG�p���b�g�A�j���V�X�e��������
	ConnectBGPalAnm_Init(&wk->cbp, p_handle, NARC_wifip2pmatch_conect_anm_NCLR, HEAPID_EMAIL);
	
	ArchiveDataHandleClose( p_handle );
		
	// �J�[�\���pBG
	{
		p_handle = ArchiveDataHandleOpen( ARC_EMAIL_GRA, HEAPID_EMAIL );		

		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );

		ArcUtil_HDL_BgCharSet( p_handle, NARC_email_gra_mail_cur_NCGR, bgl, GF_BGL_FRAME3_M, 0, 0, 0, HEAPID_EMAIL );
		ArcUtil_HDL_ScrnSet( p_handle, NARC_email_gra_mail_cur_NSCR, bgl, GF_BGL_FRAME3_M,  0, 0, 0, HEAPID_EMAIL );
		ArcUtil_HDL_PalSet( p_handle, NARC_email_gra_mail_cur_NCLR, PALTYPE_MAIN_BG, EMAIL_BG_CUR_PAL*0x20, 0x20, HEAPID_EMAIL);

		ArchiveDataHandleClose( p_handle );
	}
}

#define SUB_TEXT_X		(  4 )
#define SUB_TEXT_Y		(  4 )
#define SUB_TEXT_SX		( 23 )
#define SUB_TEXT_SY		( 16 )

#define CONNECT_TEXT_X	(  4 )
#define CONNECT_TEXT_Y	(  1 )
#define CONNECT_TEXT_SX	( 24 )
#define CONNECT_TEXT_SY	(  2 )

// ��b�E�C���h�E�\���ʒu��`
#define TALK_WIN_X		(  2 )
#define TALK_WIN_Y		( 19 )
#define	TALK_WIN_SX		( 27 )
#define	TALK_WIN_SY		(  4 )

#define TALK_MESSAGE_OFFSET	 ( EMAIL_MENUFRAME_CHR + MENU_WIN_CGX_SIZ )
#define ERROR_MESSAGE_OFFSET ( TALK_MESSAGE_OFFSET   + TALK_WIN_SX*TALK_WIN_SY )
#define TITLE_MESSAGE_OFFSET ( ERROR_MESSAGE_OFFSET  + SUB_TEXT_SX*SUB_TEXT_SY )
#define YESNO_OFFSET 		 ( TITLE_MESSAGE_OFFSET  + CONNECT_TEXT_SX*CONNECT_TEXT_SY )
#define MENULIST_MESSAGE_OFFSET	(ERROR_MESSAGE_OFFSET)	//�G���[���b�Z�[�W�ƈꏏ�ɂ͏o�Ȃ��̂� ��check YESNO_OFFSET�̒l�𒲂ׂāA�[���ȋ󂫂�����Ȃ�A���̌��ɂ���

//------------------------------------------------------------------
/**
 * BMPWIN�����i�����p�l���Ƀt�H���g�`��j
 *
 * @param   wk		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void BmpWinInit( EMAIL_MENU_WORK *wk )
{
	// ---------- ���C����� ------------------

	// BG0��BMPWIN(�G���[����)�E�C���h�E�m�ہE�`��
	GF_BGL_BmpWinAdd(wk->bgl, &wk->SubWin, GF_BGL_FRAME0_M,
	SUB_TEXT_X, SUB_TEXT_Y, SUB_TEXT_SX, SUB_TEXT_SY, EMAIL_TALKFONT_PAL,  ERROR_MESSAGE_OFFSET );

	GF_BGL_BmpWinDataFill( &wk->SubWin, 0x0000 );

	// BG0��BMPWIN(�^�C�g��)�E�C���h�E�m�ہE�`��
	GF_BGL_BmpWinAdd(wk->bgl, &wk->TitleWin, GF_BGL_FRAME0_M,
	CONNECT_TEXT_X, CONNECT_TEXT_Y, CONNECT_TEXT_SX, CONNECT_TEXT_SY, EMAIL_TALKFONT_PAL, TITLE_MESSAGE_OFFSET );

	GF_BGL_BmpWinDataFill( &wk->TitleWin, 0x0000 );
	Email_TalkPrint( &wk->TitleWin, wk->TitleString, 0, 1, 1, GF_PRINTCOLOR_MAKE(15,14,0) );

	// BG0��BMP�i��b�E�C���h�E�j�m��
	GF_BGL_BmpWinAdd(wk->bgl, &wk->MsgWin, GF_BGL_FRAME0_M,
		TALK_WIN_X, 
		TALK_WIN_Y, 
		TALK_WIN_SX, 
		TALK_WIN_SY, EMAIL_TALKFONT_PAL,  TALK_MESSAGE_OFFSET );
	GF_BGL_BmpWinDataFill( &wk->MsgWin, 0x0000 );
}

//------------------------------------------------------------------
/**
 * $brief   �m�ۂ���BMPWIN�����
 *
 * @param   wk		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void BmpWinDelete( EMAIL_MENU_WORK *wk )
{
	GF_BGL_BmpWinDel( &wk->MsgWin );
	GF_BGL_BmpWinDel( &wk->TitleWin );
	GF_BGL_BmpWinDel( &wk->SubWin );
}


//------------------------------------------------------------------
/**
 * $brief   
 *
 * @param   wk		
 * @param   to_seq		
 * @param   next_seq		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void Email_SetNextSeq( EMAIL_MENU_WORK *wk, int to_seq, int next_seq )
{
	wk->subprocess_seq      = to_seq;
	wk->subprocess_nextseq  = next_seq;
}

//------------------------------------------------------------------
/**
 * @brief   ���ԃA�C�R���ǉ�
 *
 * @param   wk		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void Email_TimeIconAdd( EMAIL_MENU_WORK *wk )
{
	if(wk->timeWaitWork == NULL){
		wk->timeWaitWork = TimeWaitIconAdd( &wk->MsgWin, EMAIL_MESFRAME_CHR );
	}
}

//------------------------------------------------------------------
/**
 * @brief   ���ԃA�C�R�������iNULL�`�F�b�N����j
 *
 * @param   wk		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void Email_TimeIconDel( EMAIL_MENU_WORK *wk )
{
	if(wk->timeWaitWork!=NULL){
		TimeWaitIconDel(wk->timeWaitWork);
		wk->timeWaitWork = NULL;
	}
}


//==============================================================================
/**
 * $brief   �͂��E�������E�C���h�E�o�^
 *
 * @param   bgl		
 * @param   menuframe		
 * @param   y		
 * @param   yesno_bmp_cgx		
 *
 * @retval  BMPMENU_WORK *		
 */
//==============================================================================
static BMPMENU_WORK *Email_BmpWinYesNoMake( GF_BGL_INI *bgl, int y, int yesno_bmp_cgx )
{
	BMPWIN_DAT yesnowin;

	yesnowin        = YesNoBmpWin;
	yesnowin.pos_y  = y;
	yesnowin.chrnum = yesno_bmp_cgx;

	return BmpYesNoSelectInit( bgl, &yesnowin, EMAIL_MENUFRAME_CHR, EMAIL_MENUFRAME_PAL, HEAPID_EMAIL );
}

static BMPMENU_WORK *Email_BmpWinYesNoMakeEx( GF_BGL_INI *bgl, int y, int yesno_bmp_cgx, int pos )
{
	BMPWIN_DAT yesnowin;

	yesnowin        = YesNoBmpWin;
	yesnowin.pos_y  = y;
	yesnowin.chrnum = yesno_bmp_cgx;

	return BmpYesNoSelectInitEx( bgl, &yesnowin, EMAIL_MENUFRAME_CHR, EMAIL_MENUFRAME_PAL, pos, HEAPID_EMAIL );
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
static int Enter_MenuList( EMAIL_MENU_WORK *wk )
{
	u32	ret;

	switch(wk->local_seq){
	case 0:
		{
			int i;
			BMPLIST_HEADER list_h;
			const u8 *bmp_size;
			const EMAIL_BMPMENULIST *bmp_menulist;
			int menu_max;
			
			if(EMAILSAVE_UseCheck(wk->esys->savedata) == TRUE){
				bmp_size = EmailMenuBmpSize;
				bmp_menulist = EmailMenuList_All;
				menu_max = LIST_MENU_ALL_MAX;
				list_h = EmailMenuListAllHeader;
			}
			else{
				bmp_size = EmailMenuNoDataBmpSize;
				bmp_menulist = EmailMenuList_NoData;
				menu_max = LIST_MENU_NODATA_MAX;
				list_h = EmailMenuListNoDataHeader;
			}

			//���j���[���X�g�p��BMP�E�B���h�E����
			GF_BGL_BmpWinAdd(wk->bgl, &wk->list_bmpwin, 
				GF_BGL_FRAME0_M, bmp_size[0], bmp_size[1], bmp_size[2], bmp_size[3], 
				EMAIL_TALKFONT_PAL, MENULIST_MESSAGE_OFFSET );

			wk->listmenu = BMP_MENULIST_Create(menu_max, HEAPID_EMAIL);
			for(i = 0; i < menu_max; i++){
				BMP_MENULIST_AddArchiveString(wk->listmenu, wk->EmailMsgManager, 
					bmp_menulist[i].str_id, bmp_menulist[i].param);
			}

			list_h.win = &wk->list_bmpwin;
			list_h.list = wk->listmenu;
			wk->lw = BmpListSet(&list_h, 0, 0, HEAPID_EMAIL);
			
			BmpMenuWinWrite(&wk->list_bmpwin, WINDOW_TRANS_OFF, 
				EMAIL_MENUFRAME_CHR, EMAIL_MENUFRAME_PAL);

			BmpTalkWinClear( &wk->MsgWin, WINDOW_TRANS_OFF );
			
			GF_BGL_BmpWinOn(&wk->list_bmpwin);
		}
		
		wk->local_seq++;
		break;
	case 1:
		ret = BmpListMain(wk->lw);
		switch(ret){
		case BMPLIST_NULL:
			break;
		case BMPLIST_CANCEL:
			Snd_SePlay(EMAIL_SE_CANCEL);
			Email_SubProcessChange( wk->esys, EMAIL_SUBPROC_END, 0 );
			wk->local_work  = ENTER_END;
			wk->local_seq++;
			break;
		default:
			Snd_SePlay(EMAIL_SE_DECIDE);
			wk->local_work = ret;
			wk->local_seq++;
			break;
		}
		break;
		
	default:
		BMP_MENULIST_Delete(wk->listmenu);
		BmpListExit(wk->lw, NULL, NULL);
		BmpMenuWinClear( &wk->list_bmpwin, WINDOW_TRANS_OFF);
		GF_BGL_BmpWinOff( &wk->list_bmpwin );
		GF_BGL_BmpWinDel( &wk->list_bmpwin );
		
		wk->subprocess_seq = wk->local_work;
		break;
	}
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   E���[���m�F�T�uPROC�Ăяo��
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_AddressCheckProcChange(EMAIL_MENU_WORK *wk)
{
	//�o�^�R�[�h���͉�ʂֈڍs
	Email_SubProcessChange( wk->esys, 
			EMAIL_SUBPROC_ADDRESS_CHECK, EMAIL_MODE_INPUT_EMAIL_CHECK);
	Email_RecoveryMenuModeSet( wk->esys, ENTER_MENU_LIST);
	wk->subprocess_seq = ENTER_END;
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   E���[�����󂯎�邩���߂�
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_RecvSelectYesNo(EMAIL_MENU_WORK *wk)
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==TRUE){
		return SUBSEQ_CONTINUE;
	}

	switch(wk->local_seq){
	case 0:
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_111, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 1:	//�͂��E�������E�B���h�E�\��
		wk->YesNoMenuWork = Email_BmpWinYesNoMake(wk->bgl, EMAIL_YESNO_PY2, YESNO_OFFSET );
		wk->local_seq++;
		break;
	case 2:
		{
			int ret = BmpYesNoSelectMain( wk->YesNoMenuWork, HEAPID_EMAIL );

			if(ret!=BMPMENU_NULL){
				if(ret==BMPMENU_CANCEL){	//�󂯎��Ȃ�
					Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_113, 1, 0x0f0f );
					EMAILSAVE_ParamSet(wk->esys->savedata, EMAIL_PARAM_RECV_FLAG, 0);
				}
				else{	//�󂯎��
					Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_112, 1, 0x0f0f );
					EMAILSAVE_ParamSet(wk->esys->savedata, EMAIL_PARAM_RECV_FLAG, 
						DPW_PROFILE_MAILRECVFLAG_EXCHANGE);
				}
				wk->local_seq++;
			}
		}
		break;
	case 3:
		Email_SetNextSeq( wk, ENTER_SAVE, ENTER_MENU_LIST );
		break;
	}
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   E���[�������������܂����H
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_EmailDataInitializeYesNo(EMAIL_MENU_WORK *wk)
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==TRUE){
		return SUBSEQ_CONTINUE;
	}

	switch(wk->local_seq){
	case 0:
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_114, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 1:	//�͂��E�������E�B���h�E�\��
		wk->YesNoMenuWork = Email_BmpWinYesNoMakeEx(wk->bgl, EMAIL_YESNO_PY2, YESNO_OFFSET, 1 );
		wk->local_seq++;
		break;
	case 2:
		{
			int ret = BmpYesNoSelectMain( wk->YesNoMenuWork, HEAPID_EMAIL );

			if(ret!=BMPMENU_NULL){
				if(ret==BMPMENU_CANCEL){
					wk->subprocess_seq = ENTER_MENU_LIST;
				}
				else{
					Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_115, 1, 0x0f0f );
					EMAILSAVE_DataInitialize(wk->esys->savedata);
					wk->local_seq++;
				}
			}
		}
		break;
	case 3:
		Email_SetNextSeq( wk, ENTER_SAVE, ENTER_MENU_LIST );
		break;
	}
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   E���[���ݒ�F�J�n�m�F
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static const int Agreement_Str[] = {
	msg_email_info_000,
	msg_email_info_spc,
	msg_email_info_001,
	msg_email_info_002,
	msg_email_info_003,
	msg_email_info_004,
	msg_email_info_spc,
	msg_email_info_005,
	msg_email_info_006,
	msg_email_info_spc,
	msg_email_info_007,
//	msg_email_info_008,
//	msg_email_info_009,
//	msg_email_info_spc,
//	msg_email_info_010,
//	msg_email_info_011,
};
#define INFO_MESSAGE_LINE	( 6 )

static u32 AgreePos[][ 2 ] = {
	{   4, 8 },
	{ 128, 8 },
};

static void AgreeCurPut( EMAIL_MENU_WORK* wk )
{
	STRBUF* str;
	GF_BGL_BmpWinDataFill( &wk->info_win2,  0x0f0f );
	
	str = MSGMAN_AllocString( wk->EmailMsgManager, msg_email_907 );
	GF_STR_PrintSimple( &wk->info_win2, FONT_SYSTEM, str, AgreePos[ 0 ][ 0 ] + 12, AgreePos[ 0 ][ 1 ], MSG_NO_PUT, NULL );
	STRBUF_Delete( str );
	
	str = MSGMAN_AllocString( wk->EmailMsgManager, msg_email_908 );
	GF_STR_PrintSimple( &wk->info_win2, FONT_SYSTEM, str, AgreePos[ 1 ][ 0 ] + 12, AgreePos[ 1 ][ 1 ], MSG_NO_PUT, NULL );
	STRBUF_Delete( str );

	BMPCURSOR_Print( wk->info_cur, &wk->info_win2, AgreePos[ wk->info_cur_pos ][ 0 ], AgreePos[ wk->info_cur_pos ][ 1 ] );
	
	GF_BGL_BmpWinOn( &wk->info_win2 );
}

static void AgreeCurClear( EMAIL_MENU_WORK* wk )
{
	GF_BGL_BmpWinDataFill( &wk->info_win2,  0x0f0f );	
	GF_BGL_BmpWinOn( &wk->info_win2 );
}

static void AgreeListCurMove( EMAIL_MENU_WORK* wk )
{
	if ( (++wk->info_wait) == 8 ){
		wk->info_count ^= 1;
		wk->info_wait = 0;		
		///< ��
		if ( wk->info_pos + INFO_MESSAGE_LINE != wk->info_end ){
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 1 + ( wk->info_count * 20 ), 14, 17, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 2 + ( wk->info_count * 20 ), 15, 17, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 3 + ( wk->info_count * 20 ), 16, 17, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 4 + ( wk->info_count * 20 ), 17, 17, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M,11 + ( wk->info_count * 20 ), 14, 18, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M,12 + ( wk->info_count * 20 ), 15, 18, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M,13 + ( wk->info_count * 20 ), 16, 18, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M,14 + ( wk->info_count * 20 ), 17, 18, 1, 1, 9 );
		}
		else {
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 0, 14, 17, 4, 2, 9 );
		}

		///< ��
		if ( wk->info_pos != 0 ){
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 5 + ( wk->info_count * 20 ), 14,  3, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 6 + ( wk->info_count * 20 ), 15,  3, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 7 + ( wk->info_count * 20 ), 16,  3, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 8 + ( wk->info_count * 20 ), 17,  3, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M,15 + ( wk->info_count * 20 ), 14,  4, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M,16 + ( wk->info_count * 20 ), 15,  4, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M,17 + ( wk->info_count * 20 ), 16,  4, 1, 1, 9 );
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M,18 + ( wk->info_count * 20 ), 17,  4, 1, 1, 9 );
		}
		else {
			GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME3_M, 0, 14, 3, 4, 2, 9 );
		}
		GF_BGL_LoadScreenV_Req( wk->bgl, GF_BGL_FRAME3_M );
	}
}

static void Agreement_TxtRW( EMAIL_MENU_WORK* wk )
{
	int i;
	int no = 0;
						
	GF_BGL_BmpWinDataFill( &wk->info_win,  0x0f0f );
	for ( i = wk->info_pos; i < wk->info_pos + INFO_MESSAGE_LINE; i++ ){
		STRBUF* str = MSGMAN_AllocString( wk->EmailMsgManager, Agreement_Str[ i ] );
		GF_STR_PrintSimple( &wk->info_win, FONT_SYSTEM, str, 4, no * 16, MSG_NO_PUT, NULL );
		STRBUF_Delete( str );
		no++;
	}
	GF_BGL_BmpWinOn( &wk->info_win );
}

static int Enter_Agreement( EMAIL_MENU_WORK *wk )
{
	switch ( wk->sub_seq ){
	case 0:
		{
			int i;
			int list_max = NELEMS( Agreement_Str );
			BMPLIST_HEADER list_h;
			
			wk->info_pos = 0;
			wk->info_cur_pos = 0;
			wk->info_param = 0;
			wk->info_end = NELEMS( Agreement_Str );
			GF_BGL_BmpWinAdd( wk->bgl, &wk->info_win,  GF_BGL_FRAME2_M, 1,  5, 30, 12, EMAIL_TALKFONT_PAL, MENULIST_MESSAGE_OFFSET );
			GF_BGL_BmpWinAdd( wk->bgl, &wk->info_win2, GF_BGL_FRAME2_M, 1, 19, 30,  4, EMAIL_TALKFONT_PAL, MENULIST_MESSAGE_OFFSET + ( 30 * 12 ) );

			GF_BGL_BmpWinDataFill( &wk->info_win,  0x0f0f );
			for ( i = 0; i < INFO_MESSAGE_LINE; i++ ){
				STRBUF* str = MSGMAN_AllocString( wk->EmailMsgManager, Agreement_Str[ i ] );
				GF_STR_PrintSimple( &wk->info_win, FONT_SYSTEM, str, 4, i * 16, MSG_NO_PUT, NULL );
				STRBUF_Delete( str );
			}
			BmpMenuWinWrite( &wk->info_win, WINDOW_TRANS_OFF, EMAIL_MENUFRAME_CHR, EMAIL_MENUFRAME_PAL );			
			GF_BGL_BmpWinOn( &wk->info_win );

			GF_BGL_BmpWinDataFill( &wk->info_win2,  0x0f0f );
			BmpMenuWinWrite( &wk->info_win2, WINDOW_TRANS_OFF, EMAIL_MENUFRAME_CHR, EMAIL_MENUFRAME_PAL );			
			GF_BGL_BmpWinOn( &wk->info_win2 );
			wk->info_cur = BMPCURSOR_Create( HEAPID_EMAIL );
			
			GF_BGL_BmpWinDataFill( &wk->MsgWin,  0x0f0f );
			BmpTalkWinClear( &wk->MsgWin, WINDOW_TRANS_OFF );
			GF_BGL_BmpWinOff( &wk->MsgWin );
			
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_ON );
			
			wk->info_wait  = 0;
			wk->info_count = 0;
			
			wk->sub_seq++;
		}
		break;

	case 1:
		{
			int i;
			int old = wk->info_pos;

			if ( sys.repeat & PAD_KEY_UP ){
				if ( wk->info_pos != 0 ){
					wk->info_pos--;
					Snd_SePlay( SEQ_SE_DP_SELECT );
				}
			}
			else if ( sys.repeat & PAD_KEY_DOWN ){
				if ( wk->info_pos + INFO_MESSAGE_LINE < wk->info_end ){
					wk->info_pos++;
					Snd_SePlay( SEQ_SE_DP_SELECT );
				}	
				if ( wk->info_pos + INFO_MESSAGE_LINE == wk->info_end ){	///< �Ō�܂ōs�����̂œ��Ӄ`�F�b�N
					AgreeCurPut( wk );
					wk->sub_seq++;
					Snd_SePlay( SEQ_SE_DP_SELECT );
				}
			}
			else if ( sys.trg & PAD_BUTTON_B ){
				Snd_SePlay( SEQ_SE_DP_SELECT );
				wk->info_param = 2;
				wk->sub_seq = 0xFF;
			}
			if ( old != wk->info_pos ){
				Agreement_TxtRW( wk );
			}
		}
		break;

	case 2:
		{
			if ( sys.trg & PAD_KEY_RIGHT || sys.trg & PAD_KEY_LEFT ){
				wk->info_cur_pos ^= 1;
				Snd_SePlay( SEQ_SE_DP_SELECT );
				AgreeCurPut( wk );
			}
			else if ( sys.trg & PAD_BUTTON_A ){
				wk->info_param = 2 - wk->info_cur_pos;
				Snd_SePlay( SEQ_SE_DP_SELECT );
				wk->sub_seq++;
			}
			else if ( sys.trg & PAD_BUTTON_B ){
				wk->info_param = 2;
				Snd_SePlay( SEQ_SE_DP_SELECT );
				wk->sub_seq++;
			}
			else if ( sys.trg & PAD_KEY_UP ){
				AgreeCurClear( wk );
				Snd_SePlay( SEQ_SE_DP_SELECT );
				wk->sub_seq--;
				wk->info_pos--;
				Agreement_TxtRW( wk );
			}		
		}
		break;

	default:
		{
			BmpMenuWinClear( &wk->info_win, WINDOW_TRANS_ON );
			GF_BGL_BmpWinOff( &wk->info_win );
			GF_BGL_BmpWinDel( &wk->info_win );
			
			BmpMenuWinClear( &wk->info_win2, WINDOW_TRANS_ON );
			GF_BGL_BmpWinOff( &wk->info_win2 );
			GF_BGL_BmpWinDel( &wk->info_win2 );
			
			BMPCURSOR_Delete( wk->info_cur );
			
			GF_BGL_ScrClear( wk->bgl, GF_BGL_FRAME3_M );
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
			
			wk->sub_seq = 0;
		}	
		return wk->info_param;
	}	
	AgreeListCurMove( wk );

	return 0;
}

static int Enter_AddressEntryStart(EMAIL_MENU_WORK *wk)
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==TRUE){
		return SUBSEQ_CONTINUE;
	}

	switch(wk->local_seq){
	case 0: //20�Έȏ�H
	//	Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_101, 1, 0x0f0f );
		wk->local_seq = 3;
		break;
	case 1:	//�͂��E�������E�B���h�E�\��
	//	wk->YesNoMenuWork = Email_BmpWinYesNoMakeEx(wk->bgl, EMAIL_YESNO_PY2, YESNO_OFFSET, 1 );
		wk->local_seq++;
		break;
	
	case 2:
		{
			int ret = BmpYesNoSelectMain( wk->YesNoMenuWork, HEAPID_EMAIL );
			if(ret!=BMPMENU_NULL){
				if(ret==BMPMENU_CANCEL){
					wk->local_seq++;
				}
				else{
					///< wi-fi�Ȃ���[�H
					Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_101_01, 1, 0x0f0f );
					wk->local_seq = 4;
				}
			}
		}
		break;
		
	case 3:
		{
			int ret = Enter_Agreement( wk );
			
			if ( ret == 1 ){
				Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_101_01, 1, 0x0f0f );
				wk->local_seq++;
			}
			else if ( ret == 2 ){
				Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_MENU_LIST );
			}
		}
		break;
	
	case 4:	///< �Ȃ����[
		wk->YesNoMenuWork = Email_BmpWinYesNoMake(wk->bgl, EMAIL_YESNO_PY2, YESNO_OFFSET );
		wk->local_seq++;
		break;
		
	default:
		{
			int ret = BmpYesNoSelectMain( wk->YesNoMenuWork, HEAPID_EMAIL );

			if(ret!=BMPMENU_NULL){
				if(ret==BMPMENU_CANCEL){
					Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_102, 1, 0x0f0f );
					Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_MENU_LIST );
				}
				else{
				//	Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_102, 1, 0x0f0f );
				//	Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_ADDRESS_INPUT_PROC_CHANGE );
					wk->subprocess_seq = ENTER_ADDRESS_INPUT_PROC_CHANGE;
				}
			}
		}
		break;
	}
	
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �o�^�R�[�h���͉�ʌĂяo��
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_AddressInputProcChange(EMAIL_MENU_WORK *wk)
{
	//�o�^�R�[�h���͉�ʂֈڍs
	Email_SubProcessChange( wk->esys, 
			EMAIL_SUBPROC_ADDRESS_INPUT, EMAIL_MODE_INPUT_EMAIL );
	Email_RecoveryMenuModeSet( wk->esys, ENTER_ADDRESS_RETURN);
	wk->subprocess_seq = ENTER_END;
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �A�h���X���͉�ʂ���߂��Ă����Ƃ��ɋN������V�[�P���X
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_AddressReturn(EMAIL_MENU_WORK *wk)
{
	STRCODE *code;
	
	if(Email_AddressReturnFlagGet(wk->esys) == EMAIL_ADDRESS_RET_CANCEL){
		wk->subprocess_seq = ENTER_MENU_LIST;
		return SUBSEQ_CONTINUE;
	}
	
	switch(wk->local_seq){
	case 0:
		wk->subprocess_seq = ENTER_START;
		break;
	}
	return SUBSEQ_CONTINUE;
}

//------------------------------------------------------------------
/**
 * $brief   �T�u�v���Z�X�V�[�P���X�X�^�[�g����
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_Start( EMAIL_MENU_WORK *wk)
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==TRUE){
		return SUBSEQ_CONTINUE;
	}

	switch(wk->local_seq){
	case 0:
		OS_TPrintf("Enter �J�n\n");

		wk->local_seq++;
		break;
	case 1:
		//�ʐM���܂��B�����͂����ł����H
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_103, 1, 0x0f0f );
		wk->local_seq++;
		break;

	case 2:	//�͂��E�������E�B���h�E�\��
		wk->YesNoMenuWork = Email_BmpWinYesNoMake(wk->bgl, EMAIL_YESNO_PY2, YESNO_OFFSET );
		wk->local_seq++;
		break;
	case 3:
		{
			int ret = BmpYesNoSelectMain( wk->YesNoMenuWork, HEAPID_EMAIL );

			if(ret!=BMPMENU_NULL){
				if(ret==BMPMENU_CANCEL){
					Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_MENU_LIST );
				}
				else{
					wk->local_seq++;
				}
			}
		}
		break;
	
	case 4:	//WiFi�ڑ��J�n
		if(mydwc_checkMyGSID(wk->esys->savedata) == FALSE){
			//������GS�v���t�@�C��ID���擾���Ă��Ȃ��̂ŁA�擾�v���Z�X�Ɉڍs
			Email_SubProcessChange( wk->esys, EMAIL_SUBPROC_GSPROFILE_GET, 0 );
			Email_RecoveryMenuModeSet(wk->esys, ENTER_INTERNET_CONNECT);
			wk->subprocess_seq = ENTER_END;
			return SUBSEQ_CONTINUE;
		}
		
		Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_INTERNET_CONNECT );
		break;
	}
	
	return SUBSEQ_CONTINUE;
}



//------------------------------------------------------------------
/**
 * $brief   �ڑ����J�n���܂����H
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_ConnectYesNoSelect( EMAIL_MENU_WORK *wk )
{
	int ret = BmpYesNoSelectMain( wk->YesNoMenuWork, HEAPID_EMAIL );

	if(ret!=BMPMENU_NULL){
		if(ret==BMPMENU_CANCEL){
			// �ڑ����I�����܂����H
//			Enter_MessagePrint( wk, wk->MsgManager, msg_gtc_01_008, 1, 0, 0x0f0f );
//			Email_SetNextSeq( wk, ENTER_MES_WAIT_YESNO_START, ENTER_END_YESNO_SELECT );
//			wk->subprocess_seq = ENTER_END_START;

			// �I��
			CommStateWifiEMailEnd();	// �ʐM�G���[��^�C�g���ɖ߂�悤�ɐ�p�̊֐����쐬 080603 tomoya
//			CommStateWifiDPWEnd();
			Email_SubProcessChange( wk->esys, EMAIL_SUBPROC_END, 0 );
			wk->subprocess_seq  = ENTER_END;

		}else{
			// WIFI���������J�n
			Enter_MessagePrint( wk, wk->LobbyMsgManager, msg_wifilobby_002, 1, 0x0f0f );
			Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_INTERNET_CONNECT );
			Email_TimeIconAdd(wk);

		}
	}

	return SUBSEQ_CONTINUE;
	
}


//------------------------------------------------------------------
/**
 * $brief   ���ɑI���͏I����Ă���̂�WIFI����ڑ�����
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_ForceEndStart( EMAIL_MENU_WORK *wk ) 
{
	// �ڑ����I�����܂�
	Enter_MessagePrint( wk, wk->SystemMsgManager, dwc_message_0011, 1, 0x0f0f );
	Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_FORCE_END );

	return SUBSEQ_CONTINUE;
}

//------------------------------------------------------------------
/**
 * $brief   �ڑ��I��
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_ForceEnd( EMAIL_MENU_WORK *wk )
{
    // �ʐM�G���[�Ǘ��̂��߂ɒʐM���[�`����OFF
	CommStateWifiEMailEnd();	// �ʐM�G���[��^�C�g���ɖ߂�悤�ɐ�p�̊֐����쐬 080603 tomoya
//    CommStateWifiDPWEnd();
	// WIFI���������I��
    DWC_CleanupInet();
    WirelessIconEasyEnd();
	sys_SleepOK(SLEEPTYPE_COMM);

    //��ʂ𔲂�����2�x�A���ŁuE���[���ݒ�v������Wifi�Ɍq���悤�Ƃ����
    //"dpw_tr.c:150 Panic:dpw tr is already initialized."
    //�̃G���[���o��̂ł�����Ƃ��̏I���֐����ĂԂ悤�ɂ��� 2007.10.26(��) matsuda
	if(wk->esys->dpw_tr_init == TRUE){
	    Dpw_Tr_End();
	    wk->esys->dpw_tr_init = 0;
	}

//	Email_SubProcessChange( wk, WORLDTRADE_ENTER, 0 );
	wk->subprocess_seq  = ENTER_FORCE_END_MESSAGE;

	return SUBSEQ_CONTINUE;
	
}

//------------------------------------------------------------------
/**
 * @brief   �ڑ��I�����b�Z�[�W
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_ForceEndMessage( EMAIL_MENU_WORK *wk )
{
	Enter_MessagePrint( wk, wk->SystemMsgManager, dwc_message_0012, 1, 0x0f0f );
	Email_SetNextSeq( wk, ENTER_MES_WAIT_1_SECOND, ENTER_END );
		
	return SUBSEQ_CONTINUE;
}


//------------------------------------------------------------------
/**
 * $brief   �C���^�[�l�b�g�ڑ��J�n
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_InternetConnect( EMAIL_MENU_WORK *wk )
{
	switch(wk->local_seq){
	case 0:
		// �ʐM�G���[�Ǘ��̂��߂ɒʐM���[�`����ON
		CommStateWifiEMailStart( wk->esys->savedata );	// �G���[��^�C�g���ɖ߂�悤�ɐ�p�̊֐����쐬 080603tomoya 
//		CommStateWifiDPWStart( wk->esys->savedata );
		// Wifi�ʐM�A�C�R��
	    WirelessIconEasy();
		// WIFI���������J�n
		Enter_MessagePrint( wk, wk->LobbyMsgManager, msg_wifilobby_002, 1, 0x0f0f );
		Email_TimeIconAdd(wk);
		wk->local_seq++;
		break;
	case 1:
		if( Enter_MessagePrintEndCheck( wk->MsgIndex )==FALSE){
			wk->local_seq++;
		}
		break;
	case 2:
		sys_SleepNG(SLEEPTYPE_COMM);
		DWC_InitInetEx(&wk->esys->stConnCtrl,COMM_DMA_NO,COMM_POWERMODE,COMM_SSL_PRIORITY);
	    DWC_SetAuthServer(GF_DWC_CONNECTINET_AUTH_TYPE);
		DWC_ConnectInetAsync();
		
		wk->subprocess_seq = ENTER_INTERNET_CONNECT_WAIT;
		OS_TPrintf("InternetConnet Start\n");
		break;
	}
	return SUBSEQ_CONTINUE;
}

//------------------------------------------------------------------
/**
 * $brief   �l�b�g�ڑ��҂�
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_InternetConnectWait( EMAIL_MENU_WORK *wk )
{
	DWC_ProcessInet();

	if (DWC_CheckInet())
	{
		switch (DWC_GetInetStatus())
		{
		case DWC_CONNECTINET_STATE_ERROR:
			{
				// �G���[�\��
				DWCError err;
				int errcode;
                DWCErrorType errtype;
				err = DWC_GetLastErrorEx(&errcode,&errtype);
				wk->ErrorRet  = err;
				wk->ErrorCode = errcode;
				wk->ErrorType = errtype;

				OS_TPrintf("   Error occurred %d %d.\n", err, errcode);
			}
			DWC_ClearError();
			DWC_CleanupInet();
		    WirelessIconEasyEnd();
		    CommStateWifiEMailEnd();
			sys_SleepOK(SLEEPTYPE_COMM);

		    //��ʂ𔲂�����2�x�A���ŁuE���[���ݒ�v������Wifi�Ɍq���悤�Ƃ����
		    //"dpw_tr.c:150 Panic:dpw tr is already initialized."
		    //�̃G���[���o��̂ł�����Ƃ��̏I���֐����ĂԂ悤�ɂ��� 2007.10.26(��) matsuda
			if(wk->esys->dpw_tr_init == TRUE){
			    Dpw_Tr_End();
			    wk->esys->dpw_tr_init = 0;
			}

			Email_TimeIconDel( wk );
			wk->subprocess_seq = ENTER_DWC_ERROR_PRINT;

			break;
		case DWC_CONNECTINET_STATE_NOT_INITIALIZED:
		case DWC_CONNECTINET_STATE_IDLE:
		case DWC_CONNECTINET_STATE_OPERATING:
		case DWC_CONNECTINET_STATE_OPERATED:
		case DWC_CONNECTINET_STATE_DISCONNECTING:
		case DWC_CONNECTINET_STATE_DISCONNECTED:
		default:
			OS_TPrintf("DWC_CONNECTINET_STATE_DISCONNECTED!\n");
			//break;
		case DWC_CONNECTINET_STATE_FATAL_ERROR:
			{
				// �G���[�\��
				DWCError err;
				int errcode;
				err = DWC_GetLastError(&errcode);

				OS_TPrintf("   Error occurred %d %d.\n", err, errcode);
				Email_TimeIconDel(wk);
				wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
				wk->ConnectErrorNo = DPW_TR_ERROR_SERVER_TIMEOUT;
			}
			break;

		case DWC_CONNECTINET_STATE_CONNECTED:
	        {	// �ڑ����\������B�X�܂̏ꍇ�͓X�܏����\������B
				DWCApInfo apinfo;
	
				DWC_GetApInfo(&apinfo);
	
	            OS_TPrintf("   Connected to AP type %d.\n", apinfo.aptype);

	            if (apinfo.aptype == DWC_APINFO_TYPE_SHOP)
	            {
					OS_TPrintf("<Shop AP Data>\n");
	                OS_TPrintf("area code: %d.\n", apinfo.area);
	                OS_TPrintf("spotinfo : %s.\n", apinfo.spotinfo);
	            }
	        }
	        // �R�l�N�g�����H
			wk->subprocess_seq = ENTER_WIFI_CONNECTION_LOGIN;
			break;
		}
		
		// ���ԃA�C�R������

	}
	
	return SUBSEQ_CONTINUE;
}

//------------------------------------------------------------------
/**
 * @brief   GameSpy�T�[�o�[���O�C���J�n
 *
 * @param   wk		
 *
 * @retval  int
 */
//------------------------------------------------------------------
static int Enter_WifiConnectionLogin( EMAIL_MENU_WORK *wk )
{
	DWC_NASLoginAsync();
	wk->subprocess_seq = ENTER_WIFI_CONNECTION_LOGIN_WAIT;
	OS_Printf("GameSpy�T�[�o�[���O�C���J�n\n");

	return SUBSEQ_CONTINUE;
}
//------------------------------------------------------------------
/**
 * @brief   GameSpy�T�[�o�[���O�C�������҂�
 *
 * @param   wk
 *
 * @retval  int
 */
//------------------------------------------------------------------
static int Enter_WifiConnectionLoginWait( EMAIL_MENU_WORK *wk )
{
	switch(DWC_NASLoginProcess()){
	case DWC_NASLOGIN_STATE_SUCCESS:
		OS_Printf("GameSpy�T�[�o�[���O�C������\n");
		wk->subprocess_seq = ENTER_DPWTR_INIT;
		break;
	case DWC_NASLOGIN_STATE_ERROR:
	case DWC_NASLOGIN_STATE_CANCELED:
	case DWC_NASLOGIN_STATE_DIRTY:
		Email_TimeIconDel(wk);
		OS_Printf("GameSpy�T�[�o�[���O�C�����s\n");
		{
			int errCode;
			DWCErrorType errType;
			DWCError dwcError;
			dwcError = DWC_GetLastErrorEx( &errCode, &errType );
			wk->ErrorRet  = dwcError;
			wk->ErrorCode = errCode;

			DWC_ClearError();
			DWC_CleanupInet();
		    WirelessIconEasyEnd();
		    CommStateWifiEMailEnd();
			sys_SleepOK(SLEEPTYPE_COMM);

		    //��ʂ𔲂�����2�x�A���ŁuE���[���ݒ�v������Wifi�Ɍq���悤�Ƃ����
		    //"dpw_tr.c:150 Panic:dpw tr is already initialized."
		    //�̃G���[���o��̂ł�����Ƃ��̏I���֐����ĂԂ悤�ɂ��� 2007.10.26(��) matsuda
			if(wk->esys->dpw_tr_init == TRUE){
			    Dpw_Tr_End();
			    wk->esys->dpw_tr_init = 0;
			}

			//���肦�Ȃ��͂������A�ǂ̃G���[�ɂ�����������Ȃ��\�����l�����A�����l�Ƃ��Ď��̃V�[�P���X���ɐݒ肵�Ă���
			wk->subprocess_seq = ENTER_DWC_ERROR_PRINT;

			switch(errType){
			case DWC_ETYPE_LIGHT:
			case DWC_ETYPE_SHOW_ERROR:
				wk->subprocess_seq = ENTER_DWC_ERROR_PRINT;
				break;
			case DWC_ETYPE_SHUTDOWN_GHTTP:
				DWC_ShutdownGHTTP();
				wk->subprocess_seq = ENTER_DWC_ERROR_PRINT;
				break;
			case DWC_ETYPE_DISCONNECT:
				wk->subprocess_seq = ENTER_DWC_ERROR_PRINT;
				break;
			case DWC_ETYPE_SHUTDOWN_FM:
				OS_TPrintf("DWC_ETYPE_SHUTDOWN_FM!\n");
				DWC_ShutdownFriendsMatch();
				wk->subprocess_seq = ENTER_DWC_ERROR_PRINT;
				break;
			case DWC_ETYPE_SHUTDOWN_ND:	//���̃V�[�P���X�ł͂��肦�Ȃ��̂ňꉞ�����ӂ��Ƃ΂��ɂ���
				OS_TPrintf("DWC_ETYPE_SHUTDOWN_ND!\n");
				//break;
			case DWC_ETYPE_FATAL:
				// �����ӂ��Ƃ΂�
				CommFatalErrorFunc_NoNumber();
				break;
			}

			// 20000�ԑ���L���b�`������errType�����ł��낤�ƃ��Z�b�g�G���[��
			if(errCode<-20000 && errCode >=-29999){
//				CommSetErrorReset(COMM_ERROR_RESET_TITLE);
				OS_Printf("dwcError = %d  errCode = %d, errType = %d\n", dwcError, errCode, errType);
				wk->subprocess_seq = ENTER_DWC_ERROR_PRINT;
			}
		}
		break;
	}
	
	
	return SUBSEQ_CONTINUE;
}



//------------------------------------------------------------------
/**
 * $brief   ���E�����T�[�o�[�ڑ����C�u����������
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_DpwTrInit( EMAIL_MENU_WORK *wk )
{
	// ���E�����ڑ�������
	DWCUserData		*MyUserData;		// �F�؍ς݂�DWCUSER�f�[�^�������Ȃ��͂�
	s32 profileId;
	SYSTEMDATA *systemdata;
	WIFI_LIST *wifilist;
	
	wifilist = SaveData_GetWifiListData(wk->esys->savedata);
	systemdata = SaveData_GetSystemData(wk->esys->savedata);

	// DWCUser�\���̎擾
	MyUserData = WifiList_GetMyUserInfo(wifilist);

	// ����FriendKey�̓v���C���[���n�߂Ď擾�������̂��H
	profileId = SYSTEMDATA_GetDpwInfo( systemdata );
	if( profileId==0 ){
		OS_TPrintf("����擾profileId�Ȃ̂�DpwInfo�Ƃ��ēo�^���� %08x \n", mydwc_getMyGSID(SaveData_GetWifiListData(wk->esys->savedata)));

		// ����擾FriendKey�Ȃ̂ŁADpwId�Ƃ��ĕۑ�����
		SYSTEMDATA_SetDpwInfo( systemdata, mydwc_getMyGSID(wifilist) );
	}

	
	// �����ȃf�[�^���擾
	profileId = SYSTEMDATA_GetDpwInfo( systemdata );
	OS_Printf("Dpw�T�[�o�[���O�C����� profileId=%08x\n", profileId);

	// DPW_TR������
	Dpw_Tr_Init( profileId, DWC_CreateFriendKey( MyUserData ) );
	wk->esys->dpw_tr_init = TRUE;

	OS_TPrintf("Dpw Trade ������\n");

	wk->subprocess_seq = ENTER_SERVER_START;
	
	return SUBSEQ_CONTINUE;
}

//------------------------------------------------------------------
/**
 * $brief   E���[���F�؂̃T�[�o�[�Ƃ̂����J�n
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_ServerStart( EMAIL_MENU_WORK *wk )
{
	Email_DCProfileCreate(wk->esys);
	Email_DCProfileSet_Address(wk->esys);
	Dpw_Tr_SetProfileAsync(&wk->esys->dc_profile, &wk->esys->dc_profile_result);
//	Dpw_Tr_GetServerStateAsync();

	OS_TPrintf("�v���t�B�[�����M\n");

	// �T�[�o�[��Ԋm�F�҂���
	wk->subprocess_seq = ENTER_SERVER_RESULT;
	wk->timeout_count = 0;

	return SUBSEQ_CONTINUE;
}

//------------------------------------------------------------------
/**
 * $brief   �T�[�o�[��Ԋm�F�҂�
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_ServerResult( EMAIL_MENU_WORK *wk )
{
	if (Dpw_Tr_IsAsyncEnd()){
		s32 result = Dpw_Tr_GetAsyncResult();
		wk->timeout_count = 0;
		switch (result){
		case DPW_TR_STATUS_SERVER_OK:		// ����ɓ��삵�Ă���
			OS_TPrintf(" profile is up!\n");

			//�v���t�B�[���̌��ʃ��[�N���m�F
			switch(wk->esys->dc_profile_result.code){
			case DPW_PROFILE_RESULTCODE_SUCCESS:	//���̓o�^�ɐ���
				switch(wk->esys->dc_profile_result.mailAddrAuthResult){
				case DPW_PROFILE_AUTHRESULT_SEND:	//�F�؃��[�����M����
					wk->subprocess_seq = ENTER_AUTH_MAIL_WAIT;
					break;
				case DPW_PROFILE_AUTHRESULT_SENDFAILURE:	//�F�؃��[���̑��M�Ɏ��s
					OS_TPrintf(" �F�؃��[�����M���s\n");
					Email_TimeIconDel(wk);
					wk->ConnectErrorNo = EMAIL_ENTRY_ERROR_SENDFAILURE;
					wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
					break;
				//�ȉ��̃G���[�����͂��̃V�[���ł͑z�肵�Ă��Ȃ����[���F�؂̌��ʂ��Ԃ����ꍇ
				// (�����̗F�B�R�[�h���ω������Ƃ��ɈȑO�Ɠ������[���A�h���X�ƃp�X���[�h��
				// �M�����Ƃ��ɂ��̂悤�ɂȂ�\��������܂��B�ʏ�͂��蓾�܂���B)�}�j���A�����p
				case DPW_PROFILE_AUTHRESULT_SUCCESS:	//�F�ؐ���
					OS_TPrintf(" mail service error\n");
					Email_TimeIconDel(wk);
					wk->ConnectErrorNo = EMAIL_ENTRY_ERROR_SUCCESS;
					wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
					break;
				case DPW_PROFILE_AUTHRESULT_FAILURE:	//�F�؂Ɏ��s
					OS_TPrintf(" mail service error\n");
					Email_TimeIconDel(wk);
					wk->ConnectErrorNo = EMAIL_ENTRY_ERROR_FAILURE;
					wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
					break;
				default:	//���肦�Ȃ����ǈꉞ�B�����ӂ��Ƃ΂�
					CommFatalErrorFunc_NoNumber();
					break;
				}
				break;
			case DPW_PROFILE_RESULTCODE_ERROR_INVALIDPARAM:	//�v���t�B�[���̑��M�p�����[�^�s��
				OS_TPrintf(" server stop service.\n");
				Email_TimeIconDel(wk);
				wk->ConnectErrorNo = EMAIL_ENTRY_ERROR_INVALIDPARAM;
				wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
				break;
			case DPW_PROFILE_RESULTCODE_ERROR_SERVERSTATE:	//�T�[�o�����e�i���Xor�ꎞ��~��
				OS_TPrintf(" server stop service.\n");
				Email_TimeIconDel(wk);
				wk->ConnectErrorNo = EMAIL_ENTRY_ERROR_SERVERSTATE;
				wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
				break;
			default:
				// ���ӂ��Ƃ΂�
				OS_TPrintf("default error!\n");
				Email_TimeIconDel(wk);
				CommFatalErrorFunc_NoNumber();
				break;
			}
			break;
		case DPW_TR_STATUS_SERVER_STOP_SERVICE:	// �T�[�r�X��~��
			OS_TPrintf(" server stop service.\n");
			Email_TimeIconDel(wk);
			wk->ConnectErrorNo = result;
			wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
			break;
		case DPW_TR_STATUS_SERVER_FULL:			// �T�[�o�[�����t
		case DPW_TR_ERROR_SERVER_FULL:
			OS_TPrintf(" server full.\n");
			Email_TimeIconDel(wk);
			wk->ConnectErrorNo = result;
			wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
			break;

		case DPW_TR_ERROR_CANCEL :
		case DPW_TR_ERROR_FAILURE :
			// �uGTS�̂����ɂ�ɂ����ς����܂����v���^�C�g����
			Email_TimeIconDel(wk);
			wk->ConnectErrorNo = result;
			wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
			break;

		case DPW_TR_ERROR_SERVER_TIMEOUT :
		case DPW_TR_ERROR_DISCONNECTED:	
			// �T�[�o�[�ƒʐM�ł��܂��񁨏I��
			OS_TPrintf(" upload error. %d \n", result);
			Email_TimeIconDel(wk);
			wk->ConnectErrorNo = result;
			wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
			break;
		case DPW_TR_ERROR_FATAL:			//!< �ʐM�v���I�G���[�B�d���̍ē������K�v�ł�
		default:
			// ���ӂ��Ƃ΂�
			Email_TimeIconDel(wk);
			CommFatalErrorFunc_NoNumber();
			break;

		}
		
	}
	else{
		wk->timeout_count++;
		if(wk->timeout_count == TIMEOUT_TIME){
			CommFatalErrorFunc_NoNumber();	//�����ӂ��Ƃ΂�
		}
	}
//	Email_TimeIconDel(wk);
	return SUBSEQ_CONTINUE;
}


//--------------------------------------------------------------
/**
 * @brief   �v���C���[���g�̔F�؃��[����M�҂�
 *
 * @param   wk		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static int Enter_AuthMailWait(EMAIL_MENU_WORK *wk)
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==TRUE){
		return SUBSEQ_CONTINUE;
	}

	switch(wk->local_seq){
	case 0:
		Email_TimeIconDel(wk);
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_105, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 1:	//�͂��E�������E�B���h�E�\��
		wk->YesNoMenuWork = Email_BmpWinYesNoMake(wk->bgl, EMAIL_YESNO_PY2, YESNO_OFFSET );
		wk->local_seq++;
		break;
	case 2:
		{
			int ret = BmpYesNoSelectMain( wk->YesNoMenuWork, HEAPID_EMAIL );

			if(ret!=BMPMENU_NULL){
				if(ret==BMPMENU_CANCEL){
					//E���[���o�^�𒆒f���܂����H
					wk->subprocess_seq = ENTER_AUTH_MAIL_END_YESNO;
				}
				else{
					wk->local_seq++;
				}
			}
		}
		break;
	case 3:
		//����4���̓o�^�R�[�h����͂��Ă�������
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_105_04, 1, 0x0f0f );
		Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_AUTH_INPUT_PROC_CHANGE );
		break;
	}
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �o�^�R�[�h���͉�ʌĂяo��
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_AuthInputProcChange(EMAIL_MENU_WORK *wk)
{
	//�o�^�R�[�h���͉�ʂֈڍs
	Email_SubProcessChange( wk->esys, 
			EMAIL_SUBPROC_AUTHENTICATE_INPUT, EMAIL_MODE_INPUT_AUTHENTICATE );
	Email_RecoveryMenuModeSet( wk->esys, ENTER_AUTHENTICATE_RETURN);
	wk->subprocess_seq = ENTER_CONNECT_END;
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �F�؃��[����M��́A�uE���[���o�^�𒆒f���܂����H�v�I��
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_AuthMailEndYesNo(EMAIL_MENU_WORK *wk)
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==TRUE){
		return SUBSEQ_CONTINUE;
	}
	
	switch(wk->local_seq){
	case 0:
		//E���[���o�^�𒆒f���܂����H
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_105_01, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 1:
		wk->YesNoMenuWork = Email_BmpWinYesNoMake(wk->bgl, EMAIL_YESNO_PY2, YESNO_OFFSET );
		wk->local_seq++;
		break;
	case 2:
		{
			int ret = BmpYesNoSelectMain( wk->YesNoMenuWork, HEAPID_EMAIL );

			if(ret!=BMPMENU_NULL){
				if(ret==BMPMENU_CANCEL){
					wk->subprocess_seq = ENTER_AUTH_MAIL_WAIT;
				}
				else{
					Email_SetNextSeq( wk, ENTER_CLEANUP_INET, ENTER_MENU_LIST );
				}
			}
		}
		break;
	}
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �o�^�R�[�h���͉�ʂ���߂��Ă����Ƃ��ɋN������V�[�P���X
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_AuthenticateReturn(EMAIL_MENU_WORK *wk)
{
	if(Email_AuthenticateCodeGet(wk->esys) == EMAIL_AUTHENTICATE_CODE_CANCEL){
		wk->subprocess_seq = ENTER_AUTH_MAIL_END_YESNO;
		return SUBSEQ_CONTINUE;
	}
	
	switch(wk->local_seq){
	case 0:
		//�o�^�R�[�h�̊m�F���ł�
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_105_05, MSG_ALLPUT, 0x0f0f );
		Email_TimeIconAdd(wk);
		
		//Email_DCProfileCreate(wk->esys, &wk->dc_profile, wk->esys->savedata);
		Email_DCProfileSet_Authenticate(wk->esys);
		//�o�^�R�[�h���Z�b�g�����v���t�B�[���𑗐M
		Dpw_Tr_SetProfileAsync(&wk->esys->dc_profile, &wk->esys->dc_profile_result);
		OS_TPrintf("���M�����F�؃R�[�h = %d\n", wk->esys->dc_profile.mailAddrAuthPass);
		wk->local_seq++;
		break;
	case 1:
		if(Dpw_Tr_IsAsyncEnd()){
			s32 result = Dpw_Tr_GetAsyncResult();

			wk->timeout_count = 0;
			Email_TimeIconDel(wk);
			switch (result){
			case DPW_TR_STATUS_SERVER_OK:		// ����ɓ��삵�Ă���
				OS_TPrintf(" profile is up!\n");
				//�v���t�B�[���̌��ʃ��[�N���m�F
				switch(wk->esys->dc_profile_result.code){
				case DPW_PROFILE_RESULTCODE_SUCCESS:	//���̓o�^�ɐ���
					OS_TPrintf("mailAddrAuthResult = %d\n", wk->esys->dc_profile_result.mailAddrAuthResult);
					
					switch(wk->esys->dc_profile_result.mailAddrAuthResult){
					case DPW_PROFILE_AUTHRESULT_SUCCESS:	//�F�ؐ���
						OS_TPrintf("�F�ؐ���\n");
						wk->local_seq++;
						break;
					case DPW_PROFILE_AUTHRESULT_FAILURE:	//�F�؂Ɏ��s
						OS_TPrintf(" �F�؎��s\n");
						//�ē��͂�
						Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_105_06, 1, 0x0f0f );
						Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_AUTH_INPUT_PROC_CHANGE );
						break;
					//�ȉ��̃G���[�����͂��̃V�[���ł͑z�肵�Ă��Ȃ����[���F�؂̌��ʂ��Ԃ����ꍇ
					// (�����̗F�B�R�[�h���ω������Ƃ��ɈȑO�Ɠ������[���A�h���X�ƃp�X���[�h��
					// �M�����Ƃ��ɂ��̂悤�ɂȂ�\��������܂��B�ʏ�͂��蓾�܂���B)
					// �}�j���A�����p
					case DPW_PROFILE_AUTHRESULT_SEND:	//�F�؃��[�����M����
						OS_TPrintf(" mail service error\n");
						wk->ConnectErrorNo = EMAIL_PASS_ERROR_SEND;
						wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
						break;
					case DPW_PROFILE_AUTHRESULT_SENDFAILURE:	//�F�؃��[���̑��M�Ɏ��s
						OS_TPrintf(" mail service error\n");
						wk->ConnectErrorNo = EMAIL_PASS_ERROR_SENDFAILURE;
						wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
						break;
					default:	//���肦�Ȃ����ǈꉞ�B�����ӂ��Ƃ΂�
						CommFatalErrorFunc_NoNumber();
						break;
					}
					break;
				case DPW_PROFILE_RESULTCODE_ERROR_INVALIDPARAM:	//�v���t�B�[���̑��M�p�����[�^�s��
					OS_TPrintf(" server stop service.\n");
					wk->ConnectErrorNo = EMAIL_PASS_ERROR_INVALIDPARAM;
					wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
					break;
				case DPW_PROFILE_RESULTCODE_ERROR_SERVERSTATE:	//�T�[�o�����e�i���Xor�ꎞ��~��
					OS_TPrintf(" server stop service.\n");
					wk->ConnectErrorNo = EMAIL_PASS_ERROR_SERVERSTATE;
					wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
					break;
				default:
					GF_ASSERT(0);
					CommFatalErrorFunc_NoNumber();
					break;
				}
				break;
			case DPW_TR_STATUS_SERVER_STOP_SERVICE:	// �T�[�r�X��~��
				OS_TPrintf(" server stop service.\n");
				wk->ConnectErrorNo = result;
				wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
				break;
			case DPW_TR_STATUS_SERVER_FULL:			// �T�[�o�[�����t
			case DPW_TR_ERROR_SERVER_FULL:
				OS_TPrintf(" server full.\n");
				wk->ConnectErrorNo = result;
				wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
				break;

			case DPW_TR_ERROR_CANCEL :
			case DPW_TR_ERROR_FAILURE :
				// �uGTS�̂����ɂ�ɂ����ς����܂����v���^�C�g����
				wk->ConnectErrorNo = result;
				wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
				break;

			case DPW_TR_ERROR_SERVER_TIMEOUT :
			case DPW_TR_ERROR_DISCONNECTED:	
				// �T�[�o�[�ƒʐM�ł��܂��񁨏I��
				OS_TPrintf(" upload error. %d \n", result);
				wk->ConnectErrorNo = result;
				wk->subprocess_seq = ENTER_SERVER_SERVICE_ERROR;
				break;
			case DPW_TR_ERROR_FATAL:			//!< �ʐM�v���I�G���[�B�d���̍ē������K�v�ł�
			default:
				// ���ӂ��Ƃ΂�
				CommFatalErrorFunc_NoNumber();
				break;

			}
		}
		else{
			wk->timeout_count++;
			if(wk->timeout_count == TIMEOUT_TIME){
				CommFatalErrorFunc_NoNumber();	//�����ӂ��Ƃ΂�
			}
		}
		break;
	case 2:	//�F�ؐ�����̏���
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_106, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 3:
		if( Enter_MessagePrintEndCheck( wk->MsgIndex )==FALSE){
			Email_SetNextSeq( wk, ENTER_CLEANUP_INET, ENTER_PASSWORD_SETUP );
		}
		break;
	}
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �p�X���[�h�o�^��ʌĂяo��
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_PasswordSetup(EMAIL_MENU_WORK *wk)
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==TRUE){
		return SUBSEQ_CONTINUE;
	}

	switch(wk->local_seq){
	case 0:
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_107, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 1:
		Email_SubProcessChange( wk->esys, EMAIL_SUBPROC_PASSWORD_INPUT, EMAIL_MODE_INPUT_PASSWORD);
		Email_RecoveryMenuModeSet(wk->esys, ENTER_PASSWORD_SETUP_CHECK);
		wk->subprocess_seq = ENTER_CONNECT_END;
		break;
	}
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �p�X���[�h�o�^��ʌĂяo��
 *
 * @param   wk		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Enter_PasswordSetupCheck(EMAIL_MENU_WORK *wk)
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==TRUE){
		return SUBSEQ_CONTINUE;
	}

	switch(wk->local_seq){
	case 0:
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_110, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 1:
		Email_AddressSaveWorkSet(wk->esys);
		Email_AuthenticateCodeSaveWorkSet(wk->esys);
		Email_PasswordSaveWorkSet(wk->esys);
		Email_SetNextSeq( wk, ENTER_SAVE, ENTER_MENU_LIST );
		break;
	}
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �Z�[�u���s
 *
 * @param   wk		
 *
 * @retval  
 *
 * �E�Z�[�u��̔�ѐ��Email_SetNextSeq�Őݒ肵�Ă����Ă�������
 */
//--------------------------------------------------------------
static int Enter_Save(EMAIL_MENU_WORK *wk)
{
	switch(wk->local_seq){
	case 0:
		//�Z�[�u���Ă��܂��B�d���؂�Ȃ��ł�������
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_300, MSG_ALLPUT, 0x0f0f );
		Email_TimeIconAdd(wk);
		wk->local_seq++;
		break;
	case 1:
		{
			SAVE_RESULT result;
			
			result = SaveData_Save(wk->esys->savedata);
			if(result == SAVE_RESULT_OK){
				WORDSET_RegisterPlayerName(wk->WordSet,0,SaveData_GetMyStatus(wk->esys->savedata));
				Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_301, MSG_ALLPUT, 0x0f0f );
				Snd_SePlay(SEQ_SE_DP_SAVE);
			}
			else{
				Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_302, MSG_ALLPUT, 0x0f0f );
			}
			Email_TimeIconDel(wk);
			wk->local_seq++;
		}
		break;
	case 2:
		if( Enter_MessagePrintEndCheck( wk->MsgIndex )==FALSE){
			wk->local_seq++;
		}
		break;
	case 3:
		wk->local_wait++;
		if(wk->local_wait > 60){
			wk->local_wait = 0;
			wk->subprocess_seq = wk->subprocess_nextseq;
		}
		break;
	}
	return SUBSEQ_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   Wi-Fi�R�l�N�V��������ؒf
 *
 * @param   wk		
 *
 * @retval  
 *
 * �E�ؒf���b�Z�[�W��\�����܂��B
 * �E�ؒf��̔�ѐ��Email_SetNextSeq�Őݒ肵�Ă����Ă�������
 */
//--------------------------------------------------------------
static int Enter_CleanupInet( EMAIL_MENU_WORK *wk )
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==TRUE){
		return SUBSEQ_CONTINUE;
	}

	switch(wk->local_seq){
	case 0:
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_105_02, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 1:
		// WIFI���������I��
//		if(DWC_CheckInet()){
		    DWC_CleanupInet();
		    WirelessIconEasyEnd();
//		}
	    // �ʐM�G���[�Ǘ��̂��߂ɒʐM���[�`����OFF
	    CommStateWifiEMailEnd();	// �ʐM�G���[��^�C�g���ɖ߂�悤�ɐ�p�̊֐����쐬 080603 tomoya
//	    CommStateWifiDPWEnd();
		sys_SleepOK(SLEEPTYPE_COMM);
	    
	    //��ʂ𔲂�����2�x�A���ŁuE���[���ݒ�v������Wifi�Ɍq���悤�Ƃ����
	    //"dpw_tr.c:150 Panic:dpw tr is already initialized."
	    //�̃G���[���o��̂ł�����Ƃ��̏I���֐����ĂԂ悤�ɂ��� 2007.10.26(��) matsuda
		if(wk->esys->dpw_tr_init == TRUE){
		    Dpw_Tr_End();
		    wk->esys->dpw_tr_init = 0;
		}
	    wk->local_seq++;
	    break;
	case 2:
		if(Enter_MessagePrintEndCheck( wk->MsgIndex )==FALSE){
			Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_105_03, 1, 0x0f0f );
			wk->local_seq++;
		}
		break;
	case 3:
		if(Enter_MessagePrintEndCheck( wk->MsgIndex )==FALSE){
			wk->local_seq++;
		}
		break;
	case 4:
		wk->wait++;
		if(wk->wait > WAIT_ONE_SECONDE_NUM){
			wk->subprocess_seq  = wk->subprocess_nextseq;
		}
		break;
	}
	return SUBSEQ_CONTINUE;
}

//------------------------------------------------------------------
/**
 * @brief   Wifi�ڑ��G���[��\��
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_DwcErrorPrint( EMAIL_MENU_WORK *wk )
{
	int type;
	
    type =  mydwc_errorType(-wk->ErrorCode,wk->ErrorType);


	OS_Printf("error code = %d, type = %d\n", wk->ErrorCode, type);

    errorDisp(wk, type, -wk->ErrorCode);


	wk->subprocess_seq = ENTER_ERROR_PAD_WAIT;
	
	return SUBSEQ_CONTINUE;
}

//------------------------------------------------------------------
/**
 * @brief   �v�����g��L�[�҂�
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_ErrorPadWait( EMAIL_MENU_WORK *wk )
{
	if(sys.trg & PAD_BUTTON_DECIDE || sys.trg & PAD_BUTTON_CANCEL){
		BmpMenuWinClear( &wk->SubWin, WINDOW_TRANS_ON );
		wk->subprocess_seq = ENTER_MENU_LIST;//ENTER_START;
	}
	return SUBSEQ_CONTINUE;
}


//------------------------------------------------------------------
/**
 * $brief   �T�u�v���Z�X�V�[�P���X�I������
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_End( EMAIL_MENU_WORK *wk)
{
	if(wk->subprocess_seq != ENTER_CONNECT_END){
	    CommStateWifiEMailEnd();	// �ʐM�G���[��^�C�g���ɖ߂�悤�ɐ�p�̊֐����쐬 080603 tomoya
//		CommStateWifiDPWEnd();
	}

    WirelessIconEasyEnd();

	// ���ԃA�C�R�������Q�d����ɂȂ�Ȃ��悤��NULL�`�F�b�N����
	Email_TimeIconDel( wk );

	
	WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK, 
		WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_EMAIL );
	
	wk->subprocess_seq = 0;
//	wk->sub_out_flg = 1;
	
	return SUBSEQ_END;
}

//------------------------------------------------------------------
/**
 * $brief   �͂��E������
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_YesNo( EMAIL_MENU_WORK *wk)
{
	wk->YesNoMenuWork = Email_BmpWinYesNoMake(wk->bgl, EMAIL_YESNO_PY2, YESNO_OFFSET );
	wk->subprocess_seq = wk->subprocess_nextseq;

	return SUBSEQ_CONTINUE;
	
}


//==============================================================================
/**
 * $brief   �l�b�g�ɂ͌q���������ǃT�[�o�[�G���[�������\��
 *
 * @param   wk		
 *
 * @retval  none		
 */
//==============================================================================
static int Enter_ServerServiceError( EMAIL_MENU_WORK *wk )
{
	int msgno =0;
	int msgman_select = 0;
	
	switch(wk->ConnectErrorNo){
	case DPW_TR_STATUS_SERVER_STOP_SERVICE:
		msgno = msg_email_error_001;
		break;
	case DPW_TR_STATUS_SERVER_FULL:
	case DPW_TR_ERROR_SERVER_FULL:
		msgno = msg_email_error_002;
		break;
	case DPW_TR_ERROR_SERVER_TIMEOUT:
	case DPW_TR_ERROR_DISCONNECTED:
		// �f�s�r�Ƃ̂�����������܂����B
		msgno = msg_email_error_006;
		break;
	case DPW_TR_ERROR_CANCEL  :
	case DPW_TR_ERROR_FAILURE :
	case DPW_TR_ERROR_NO_DATA:
	case DPW_TR_ERROR_ILLIGAL_REQUEST :
	default:
		//�@������G���[���������܂����B
		msgno = msg_email_error_005;
		break;
	case EMAIL_ENTRY_ERROR_SENDFAILURE:
		msgno = msg_gtc_email_error_000;
		msgman_select++;
		break;
	case EMAIL_ENTRY_ERROR_SUCCESS:
		msgno = msg_gtc_email_error_001;
		msgman_select++;
		break;
	case EMAIL_ENTRY_ERROR_FAILURE:
		msgno = msg_gtc_email_error_001;
		msgman_select++;
		break;
	case EMAIL_ENTRY_ERROR_INVALIDPARAM:
		msgno = msg_gtc_email_error_002;
		msgman_select++;
		break;
	case EMAIL_ENTRY_ERROR_SERVERSTATE:
		msgno = msg_gtc_email_error_003;
		msgman_select++;
		break;
	case EMAIL_PASS_ERROR_SEND:
		msgno = msg_gtc_email_error_001;
		msgman_select++;
		break;
	case EMAIL_PASS_ERROR_SENDFAILURE:
		msgno = msg_gtc_email_error_001;
		msgman_select++;
		break;
	case EMAIL_PASS_ERROR_INVALIDPARAM:
		msgno = msg_gtc_email_error_002;
		msgman_select++;
		break;
	case EMAIL_PASS_ERROR_SERVERSTATE:
		msgno = msg_gtc_email_error_003;
		msgman_select++;
		break;
	}
	// �G���[�\��
	if(msgman_select == 0){
		Enter_MessagePrint( wk, wk->EmailMsgManager, msgno, 1, 0x0f0f );
	}
	else{
		Enter_MessagePrint( wk, wk->MsgManager, msgno, 1, 0x0f0f );
	}
	Email_SetNextSeq( wk, ENTER_MES_WAIT, ENTER_SERVER_SERVICE_END );

	OS_TPrintf("Error����\n");

	return SUBSEQ_CONTINUE;
}

//==============================================================================
/**
 * $brief   �T�[�o�[�T�[�r�X�̖��ŏI��
 *
 * @param   wk		
 *
 * @retval  none		
 */
//==============================================================================
static int Enter_ServerServiceEnd( EMAIL_MENU_WORK *wk )
{
	switch(wk->local_seq){
	case 0:
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_105_02, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 1:
		if(Enter_MessagePrintEndCheck(wk->MsgIndex) == FALSE){
		    // �ʐM�G���[�Ǘ��̂��߂ɒʐM���[�`����OFF
			CommStateWifiEMailEnd();	// �ʐM�G���[��^�C�g���ɖ߂�悤�ɐ�p�̊֐����쐬 080603 tomoya
		//    CommStateWifiDPWEnd();
		    DWC_CleanupInet();
		    WirelessIconEasyEnd();
			sys_SleepOK(SLEEPTYPE_COMM);

		    //��ʂ𔲂�����2�x�A���ŁuE���[���ݒ�v������Wifi�Ɍq���悤�Ƃ����
		    //"dpw_tr.c:150 Panic:dpw tr is already initialized."
		    //�̃G���[���o��̂ł�����Ƃ��̏I���֐����ĂԂ悤�ɂ��� 2007.10.26(��) matsuda
			if(wk->esys->dpw_tr_init == TRUE){
			    Dpw_Tr_End();
			    wk->esys->dpw_tr_init = 0;
			}
			wk->local_seq++;
		}
		break;
	case 2:
		Enter_MessagePrint( wk, wk->EmailMsgManager, msg_email_105_03, 1, 0x0f0f );
		wk->local_seq++;
		break;
	case 3:
		if(Enter_MessagePrintEndCheck( wk->MsgIndex )==FALSE){
			wk->local_seq++;
		}
		break;
	default:
		wk->wait++;
		if(wk->wait > WAIT_ONE_SECONDE_NUM){
			Email_SubProcessEndSet(wk->esys);
			wk->subprocess_seq  = ENTER_END;
		}
		break;
	}
	
	return SUBSEQ_CONTINUE;
}

//------------------------------------------------------------------
/**
 * $brief   ��b�I����҂��Ď��̃V�[�P���X��
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_MessageWait( EMAIL_MENU_WORK *wk )
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==FALSE){
		wk->subprocess_seq = wk->subprocess_nextseq;
	}
	return SUBSEQ_CONTINUE;

}

//------------------------------------------------------------------
/**
 * @brief   ��b�\����1�b�҂�
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_MessageWait1Second( EMAIL_MENU_WORK *wk )
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==FALSE){
		if(wk->wait > WAIT_ONE_SECONDE_NUM){
			wk->subprocess_seq = wk->subprocess_nextseq;
		}
		wk->wait++;
	}
	return SUBSEQ_CONTINUE;
	
}

//------------------------------------------------------------------
/**
 * $brief   ��b�\����҂�����Łu�͂��E�������v���J�n����
 *
 * @param   wk		
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int Enter_MessageWaitYesNoStart(EMAIL_MENU_WORK *wk)
{
	if( Enter_MessagePrintEndCheck( wk->MsgIndex )==FALSE){
		wk->YesNoMenuWork = Email_BmpWinYesNoMake(wk->bgl, EMAIL_YESNO_PY2, YESNO_OFFSET );
		wk->subprocess_seq = wk->subprocess_nextseq;
	}
	return SUBSEQ_CONTINUE;
	
}

//------------------------------------------------------------------
/**
 * $brief   ��b�E�C���h�E�\��
 *
 * @param   wk		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void Enter_MessagePrint( EMAIL_MENU_WORK *wk, MSGDATA_MANAGER *msgman, int msgno, int wait, u16 dat )
{
	// ������擾
	STRBUF *tempbuf;

	// ������擾
	tempbuf = MSGMAN_AllocString(  msgman, msgno );

	// WORDSET�W�J
	WORDSET_ExpandStr( wk->WordSet, wk->TalkString, tempbuf );

	STRBUF_Delete(tempbuf);

	// ��b�E�C���h�E�g�`��
	GF_BGL_BmpWinDataFill( &wk->MsgWin,  0x0f0f );
	BmpTalkWinWrite( &wk->MsgWin, WINDOW_TRANS_ON, EMAIL_MESFRAME_CHR, EMAIL_MESFRAME_PAL );

	// ������`��J�n
	wk->MsgIndex = GF_STR_PrintSimple( &wk->MsgWin, FONT_TALK, wk->TalkString, 0, 0, wait, NULL);

	wk->wait = 0;
	
	if(wait == MSG_NO_PUT || wait == MSG_ALLPUT){
		wk->MsgIndex = EMAIL_MSG_NO_WAIT;
	}
}

//--------------------------------------------------------------
/**
 * @brief   ���b�Z�[�W�̏I�������`�F�b�N
 *
 * @param   msg_index		���b�Z�[�WIndex
 *
 * @retval  TRUE:�������B�@�@FALSE:�I�����Ă���
 */
//--------------------------------------------------------------
static int Enter_MessagePrintEndCheck(int msg_index)
{
	if(msg_index == EMAIL_MSG_NO_WAIT || GF_MSG_PrintEndCheck( msg_index )==0){
		return FALSE;	//�`��I�����Ă���
	}
	return TRUE;	//���b�Z�[�W�������s��
}




//------------------------------------------------------------------
/**
 * @brief   
 *
 * @param   win		
 * @param   strbuf		
 * @param   flag		1���ƃZ���^�����O�A�Q���ƉE�悹
 * @param   color		
 * @param   font		�t�H���g�w��iFONT_TALK��FONT_SYSTEM
 *
 * @retval  int		
 */
//------------------------------------------------------------------
static int printCommonFunc( GF_BGL_BMPWIN *win, STRBUF *strbuf, int x, int flag, GF_PRINTCOLOR color, int font )
{
	int length=0,ground;
	switch(flag){
	// �Z���^�����O
	case 1:
		length = FontProc_GetPrintStrWidth( font, strbuf, 0 );
		x          = ((win->sizx*8)-length)/2;
		break;

	// �E��
	case 2:
		length = FontProc_GetPrintStrWidth( font, strbuf, 0 );
		x          = (win->sizx*8)-length;
		break;
	}
	return x;
}


//------------------------------------------------------------------
/**
 * $brief   BMPWIN���̕\���ʒu���w�肵��FONT_TALK�Ńv�����g(�`��̂݁j
 *
 * @param   win		GF_BGL_BMPWIN
 * @param   strbuf	
 * @param   x		X���W���炷�l
 * @param   y		Y���W���炷�l
 * @param   flag	0���ƍ��񂹁A1���ƃZ���^�����O�A2���ƉE��
 * @param   color	�����F�w��i�w�i�F��BMP��h��Ԃ��܂��j
 *
 * @retval  none
 */
//------------------------------------------------------------------
void Email_TalkPrint( GF_BGL_BMPWIN *win, STRBUF *strbuf, int x, int y, int flag, GF_PRINTCOLOR color )
{
	x = printCommonFunc( win, strbuf, x, flag, color, FONT_TALK );

	GF_STR_PrintColor( win, FONT_TALK, strbuf, x, y, MSG_ALLPUT, color,NULL);
	
}

//------------------------------------------------------------------
/**
 * @brief   ���������G���[�\��
 *
 * @param   wk		
 * @param   msgno		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void _systemMessagePrint( EMAIL_MENU_WORK *wk, int msgno )
{
	STRBUF *tmpString = STRBUF_Create( DWC_ERROR_BUF_NUM, HEAPID_EMAIL );
    MSGMAN_GetString(  wk->SystemMsgManager, msgno, tmpString );
    WORDSET_ExpandStr( wk->WordSet, wk->ErrorString, tmpString );

    // ��b�E�C���h�E�g�`��
    GF_BGL_BmpWinDataFill(&wk->SubWin, 15 );
    BmpMenuWinWrite(&wk->SubWin, WINDOW_TRANS_OFF, EMAIL_MENUFRAME_CHR, EMAIL_MENUFRAME_PAL );
    // ������`��J�n
    wk->MsgIndex = GF_STR_PrintSimple( &wk->SubWin, FONT_TALK,
                                       wk->ErrorString, 0, 0, MSG_ALLPUT, NULL);
	wk->MsgIndex = EMAIL_MSG_NO_WAIT;	//�ꊇ�`��Ȃ̂�
	
	STRBUF_Delete(tmpString);
}

//------------------------------------------------------------------
/**
 * @brief   Wifi�R�l�N�V�����G���[�̕\��
 *
 * @param   wk		
 * @param   type	
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void errorDisp(EMAIL_MENU_WORK* wk, int type, int code)
{
    int msgno;

    if(type != -1){
        msgno = dwc_error_0001 + type;
    }
    else{
        msgno = dwc_error_0012;
    }
//    EndMessageWindowOff(wk);
    WORDSET_RegisterNumber(wk->WordSet, 0, code,
                           5, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);

	BmpTalkWinClear( &wk->MsgWin, WINDOW_TRANS_OFF );
    _systemMessagePrint(wk, msgno);

#if 0
    switch(type){
      case 1:
      case 4:
      case 5:
        wk->seq = WIFIP2PMATCH_RETRY_INIT;  // �Đڑ����t�B�[���h��
        break;
      case 6:
      case 7:
      case 8:
      case 9:
        wk->seq = WIFIP2PMATCH_RETRY_INIT;//WIFIP2PMATCH_POWEROFF_INIT;  // �d����؂邩�t�B�[���h
        break;
      case 10:
        wk->seq = WIFIP2PMATCH_RETRY_INIT;  // ���j���[�ꗗ��
        break;
      case 0:
      case 2:
      case 3:
      case 11:
      default:
        wk->seq = WIFIP2PMATCH_MODE_CHECK_AND_END;  // �t�B�[���h
        break;
    }
#endif
}




//--------------------------------------------------------------
/**
 * @brief   E���[����ʁF�g�b�v���j���[�̃��X�g�̃J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
 *
 * @param   wk		
 * @param   param		
 * @param   mode		
 */
//--------------------------------------------------------------
static void EmailMenuListAllHeader_CursorCallback(BMPLIST_WORK * wk, u32 param, u8 mode)
{
	if(mode == 0){	//����������SE��炳�Ȃ�
		Snd_SePlay(EMAIL_SE_MOVE);
	}
}

//--------------------------------------------------------------
/**
 * @brief   E���[����ʁF�g�b�v���j���[�̃��X�g�̃J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
 *
 * @param   wk		
 * @param   param		
 * @param   mode		
 */
//--------------------------------------------------------------
static void EmailMenuListNoDataHeader_CursorCallback(BMPLIST_WORK * wk, u32 param, u8 mode)
{
	if(mode == 0){	//����������SE��炳�Ȃ�
		Snd_SePlay(EMAIL_SE_MOVE);
	}
}

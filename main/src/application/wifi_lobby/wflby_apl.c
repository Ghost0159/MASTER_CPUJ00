//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_apl.c
 *	@brief		WiFi���r�[�A�v���P�[�V�����Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.09.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "system/procsys.h"
#include "system/pm_overlay.h"

#include "application/bucket.h"
#include "application/balloon.h"
#include "application/balance_ball.h"
#include "application/footprint_main.h"
#include "application/wifi_lobby/wflby_anketo.h"

#include "wflby_apl.h"

// ���삳����v���b�N
#include "wflby_room.h"
#include "worldtimer.h"
#include "lobby_news.h"
#include "wflby_connect.h"

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
///	�A�v���œ������V�X�e��
//=====================================
enum {
	WFLBY_APL_ROOM,
	WFLBY_APL_WORLDTIMER,
	WFLBY_APL_TOPIC,
	WFLBY_APL_FOOT1,
	WFLBY_APL_FOOT2,
	WFLBY_APL_LOGIN,
	WFLBY_APL_BALLSLOW,
	WFLBY_APL_BALANCEBALL,
	WFLBY_APL_BALLOON,
	WFLBY_APL_LOGOUT,
	WFLBY_APL_TIMEOUTLOGOUT,
	WFLBY_APL_ANKETO_INPUT,
	WFLBY_APL_ANKETO_OUTPUT,
	WFLBY_APL_NUM,
} ;



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	1�A�v���f�[�^
//=====================================
typedef struct {
	void* p_param;
} WFLBY_APLDATA;



//-------------------------------------
///	WiFi���r�[�@�A�v���Ǘ��V�X�e��
//=====================================
typedef struct _WFLBY_APL{
	PROC*			p_proc;					// �A�v���v���b�N
	WFLBY_APLDATA	apldata[WFLBY_APL_NUM];	// ����A�v�����̃p�����[�^
	u8				aplno;					// ���s���̃A�v���i���o�[
	u8				check_skip;				// �ڑ���ʐڑ��m�F���X�L�b�v���邩
	u16				heapID;					// �q�[�vID

	// �f�[�^�֘A
	WFLBY_SYSTEM*		p_system;
	SAVEDATA*			p_savedata;
	WFLBY_COUNTER_TIME* p_wflby_counter;
}WFLBY_APL;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WFLBY_APL_PROC_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data, const PROC_DATA* cp_pdata );

// �A�v���f�[�^�֘A
static void WFLBY_APLDATA_Init( WFLBY_APL* p_sys, u32 idx,  u32 heapID );
static void WFLBY_APLDATA_Exit( WFLBY_APL* p_sys, u32 idx );
static void WFLBY_APLDATA_Start( WFLBY_APL* p_sys, u32 idx );
static WFLBY_APL_RET WFLBY_APLDATA_End( WFLBY_APL* p_sys, u32 idx );


// �~�j�Q�[�����ʃ��[�N�쐬
static void WFLBY_APL_MINIGAME_AllocWk( WFLBY_MINIGAME_WK* p_wk, u32 heapID );
static void WFLBY_APL_MINIGAME_FreeWk( WFLBY_MINIGAME_WK* p_wk );
static void WFLBY_APL_MINIGAME_InitWk( WFLBY_MINIGAME_WK* p_wk, WFLBY_SYSTEM* p_system, u32 heapID );

// �e�A�v���̊֐�
// ����
static void WFLBY_APLDATA_ROOM_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_ROOM_Exit( WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_ROOM_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_ROOM_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );

// ���v
static void WFLBY_APLDATA_WORLDTIMER_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_WORLDTIMER_Exit( WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_WORLDTIMER_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_WORLDTIMER_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );

// �g�s�b�N
static void WFLBY_APLDATA_TOPIC_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_TOPIC_Exit( WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_TOPIC_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_TOPIC_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );

// ���O�C��
static void WFLBY_APLDATA_LOGIN_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_LOGIN_Exit( WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_LOGIN_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_LOGIN_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );

// �ʓ���
static void WFLBY_APLDATA_BS_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_BS_Exit( WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_BS_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_BS_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );

// �ʏ��
static void WFLBY_APLDATA_BB_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_BB_Exit( WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_BB_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_BB_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );

// ���犄
static void WFLBY_APLDATA_BL_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_BL_Exit( WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_BL_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_BL_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );

// ���Ճ{�[�h
static void WFLBY_APLDATA_FOOT_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_FOOT_Exit( WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_FOOT_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_FOOT1_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_FOOT2_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );

// ���O�A�E�g
static void WFLBY_APLDATA_LOGOUT_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_LOGOUT_Exit( WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_LOGOUT_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_LOGOUT_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_TIMEOUTLOGOUT_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_Logout_StartDef( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data, BOOL timeout );

// �A���P�[�g
static void WFLBY_APLDATA_ANKETO_InitInput( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_ANKETO_InitOutput( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID );
static void WFLBY_APLDATA_ANKETO_Exit( WFLBY_APLDATA* p_data );
static void WFLBY_APLDATA_ANKETO_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );
static WFLBY_APL_RET WFLBY_APLDATA_ANKETO_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data );

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[�A�v���Ǘ��V�X�e���쐬
 *
 *	@param	check_skip		�ڑ��m�F���X�L�b�v���邩
 *	@param	p_save			�Z�[�u�f�[�^
 *	@param	p_wflby_counter	WiFi�Ђ�Γ������Ԋi�[��
 *	@param	p_system		�V�X�e�����[�N
 *	@param	heapID			�q�[�vID
 *
 *	@return	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
WFLBY_APL* WFLBY_APL_Init( BOOL check_skip, SAVEDATA* p_save, WFLBY_COUNTER_TIME* p_wflby_counter, WFLBY_SYSTEM* p_system, u32 heapID )
{
	WFLBY_APL* p_wk;

	p_wk = sys_AllocMemory( heapID, sizeof(WFLBY_APL) );
	memset( p_wk, 0, sizeof(WFLBY_APL) );

	// �f�[�^�ۑ�
	p_wk->p_system			= p_system;
	p_wk->p_savedata		= p_save;
	p_wk->p_wflby_counter	= p_wflby_counter;

	// �ڑ��m�F�t���O
	p_wk->check_skip	= check_skip;

	// �q�[�vID�ۑ�
	p_wk->heapID		= heapID;

	// �A�v���f�[�^������
	{
		int i;
		for( i=0; i<WFLBY_APL_NUM; i++ ){
			WFLBY_APLDATA_Init( p_wk, i, heapID );
		}
	}
	

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[�A�v���Ǘ��V�X�e���j��
 *
 *	@param	p_wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_APL_Exit( WFLBY_APL* p_wk )
{
	// �A�v���f�[�^�j��
	{
		int i;
		for( i=0; i<WFLBY_APL_NUM; i++ ){
			WFLBY_APLDATA_Exit( p_wk, i );
		}
	}
	
	sys_FreeMemoryEz( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�v���P�[�V�����̊J�n
 *
 *	@param	p_wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_APL_Start( WFLBY_APL* p_wk )
{
	WFLBY_APLDATA_Start( p_wk, WFLBY_APL_LOGIN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[�A�v���Ǘ��V�X�e��	���C��
 *
 *	@param	p_wk	�V�X�e�����[�N
 *
 *	@return	�������
 */
//-----------------------------------------------------------------------------
WFLBY_APL_RET WFLBY_APL_Main( WFLBY_APL* p_wk )
{
	BOOL result;
	u32 ret = WFLBY_APL_RET_NONE;
	
	if( p_wk->p_proc ){
		result = ProcMain( p_wk->p_proc );
		if( result ){
			PROC_Delete( p_wk->p_proc );
			p_wk->p_proc = NULL;
			
			//  ���̃A�v���̏I������
			ret = WFLBY_APLDATA_End( p_wk, p_wk->aplno );
		}
	}
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[�A�v���Ǘ��V�X�e��	VBlank����
 *
 *	@param	p_wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_APL_VBlank( WFLBY_APL* p_wk )
{
}


//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	�v���b�N�J�n���ʊ֐�
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�A�v���f�[�^
 *	@param	cp_pdata	�v���b�N�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_APL_PROC_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data, const PROC_DATA* cp_pdata )
{
	GF_ASSERT( p_sys->p_proc == NULL );
	p_sys->p_proc = PROC_Create( cp_pdata, p_data->p_param, p_sys->heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�v�����[�N�̏�����
 *
 *	@param	p_sys	�A�v���V�X�e��
 *	@param	idx		�C���f�b�N�X
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_Init( WFLBY_APL* p_sys, u32 idx,  u32 heapID )
{
	static void (*pFunc[])( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data, u32 heapID ) = {
		WFLBY_APLDATA_ROOM_Init,
		WFLBY_APLDATA_WORLDTIMER_Init,
		WFLBY_APLDATA_TOPIC_Init,
		WFLBY_APLDATA_FOOT_Init,
		WFLBY_APLDATA_FOOT_Init,
		WFLBY_APLDATA_LOGIN_Init,
		WFLBY_APLDATA_BS_Init,
		WFLBY_APLDATA_BB_Init,
		WFLBY_APLDATA_BL_Init,
		WFLBY_APLDATA_LOGOUT_Init,
		WFLBY_APLDATA_LOGOUT_Init,
		WFLBY_APLDATA_ANKETO_InitInput,
		WFLBY_APLDATA_ANKETO_InitOutput,
	};
	GF_ASSERT( idx < WFLBY_APL_NUM );
	pFunc[ idx ]( p_sys, &p_sys->apldata[idx], heapID );
} 

//----------------------------------------------------------------------------
/**
 *	@brief	�A�v�����[�N�̔j��
 *
 *	@param	p_sys	�A�v���V�X�e��
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_Exit( WFLBY_APL* p_sys, u32 idx )
{
	static void (*pFunc[])( WFLBY_APLDATA* p_data ) = {
		WFLBY_APLDATA_ROOM_Exit,
		WFLBY_APLDATA_WORLDTIMER_Exit,
		WFLBY_APLDATA_TOPIC_Exit,
		WFLBY_APLDATA_FOOT_Exit,
		WFLBY_APLDATA_FOOT_Exit,
		WFLBY_APLDATA_LOGIN_Exit,
		WFLBY_APLDATA_BS_Exit,
		WFLBY_APLDATA_BB_Exit,
		WFLBY_APLDATA_BL_Exit,
		WFLBY_APLDATA_LOGOUT_Exit,
		WFLBY_APLDATA_LOGOUT_Exit,
		WFLBY_APLDATA_ANKETO_Exit,
		WFLBY_APLDATA_ANKETO_Exit,
	};
	GF_ASSERT( idx < WFLBY_APL_NUM );
	pFunc[ idx ]( &p_sys->apldata[idx] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�v���J�n
 *
 *	@param	p_sys	�A�v���V�X�e��
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_Start( WFLBY_APL* p_sys, u32 idx )
{
	static void (*pFunc[])( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data ) = {
		WFLBY_APLDATA_ROOM_Start,
		WFLBY_APLDATA_WORLDTIMER_Start,
		WFLBY_APLDATA_TOPIC_Start,
		WFLBY_APLDATA_FOOT1_Start,
		WFLBY_APLDATA_FOOT2_Start,
		WFLBY_APLDATA_LOGIN_Start,
		WFLBY_APLDATA_BS_Start,
		WFLBY_APLDATA_BB_Start,
		WFLBY_APLDATA_BL_Start,
		WFLBY_APLDATA_LOGOUT_Start,
		WFLBY_APLDATA_TIMEOUTLOGOUT_Start,
		WFLBY_APLDATA_ANKETO_Start,
		WFLBY_APLDATA_ANKETO_Start,
	};
	GF_ASSERT( idx < WFLBY_APL_NUM );
	pFunc[ idx ]( p_sys, &p_sys->apldata[idx] );

	// �J�n�A�v���i���o�[�ۑ�
	p_sys->aplno = idx;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�v���I��
 *
 *	@param	p_sys	�A�v���V�X�e��
 *	@param	idx		�C���f�b�N�X
 *
 *	@return	�A�v���߂�l
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_End( WFLBY_APL* p_sys, u32 idx )
{
	static WFLBY_APL_RET (*pFunc[])( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data ) = {
		WFLBY_APLDATA_ROOM_End,
		WFLBY_APLDATA_WORLDTIMER_End,
		WFLBY_APLDATA_TOPIC_End,
		WFLBY_APLDATA_FOOT_End,
		WFLBY_APLDATA_FOOT_End,
		WFLBY_APLDATA_LOGIN_End,
		WFLBY_APLDATA_BS_End,
		WFLBY_APLDATA_BB_End,
		WFLBY_APLDATA_BL_End,
		WFLBY_APLDATA_LOGOUT_End,
		WFLBY_APLDATA_LOGOUT_End,
		WFLBY_APLDATA_ANKETO_End,
		WFLBY_APLDATA_ANKETO_End,
	};
	GF_ASSERT( idx < WFLBY_APL_NUM );
	return pFunc[ idx ]( p_sys, &p_sys->apldata[idx] );
}





//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[�����ʃ��[�N	�������m��
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APL_MINIGAME_AllocWk( WFLBY_MINIGAME_WK* p_wk, u32 heapID )
{
	int i;
	
	for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
		p_wk->p_mystate[i] = MyStatus_AllocWork( heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[�����ʃ��[�N	�������j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APL_MINIGAME_FreeWk( WFLBY_MINIGAME_WK* p_wk )
{
	int i;
	
	for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
		sys_FreeMemoryEz( p_wk->p_mystate[i] );
		p_wk->p_mystate[i] = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[�����ʃ��[�N�̏�����
 *
 *	@param	p_wk		���[�N
 *	@param	p_system	�V�X�e�����[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APL_MINIGAME_InitWk( WFLBY_MINIGAME_WK* p_wk, WFLBY_SYSTEM* p_system, u32 heapID )
{
	const WFLBY_USER_PROFILE* cp_profile;
	const WFLBY_USER_PROFILE* cp_status_profile;
	int i;

	// �~�j�Q�[�����ʃ��[�N���쐬
	cp_profile		= WFLBY_SYSTEM_GetMyProfileLocal( p_system );
	p_wk->gadget	= WFLBY_SYSTEM_GetProfileItem( cp_profile );
	WFLBY_SYSTEM_GetVipFlagWk( p_system, &p_wk->vipflag );
	WFLBY_SYSTEM_PLIDX_Get( p_system, &p_wk->plidx );
	p_wk->p_wflbysystem	= p_system;
	p_wk->error_end	= FALSE;

	// ���ꂼ��̐l��MYSTATUS���[�N�擾
	for( i=0; i<WFLBY_MINIGAME_MAX; i++ ){
		cp_status_profile = WFLBY_SYSTEM_GetUserProfile( p_system, p_wk->plidx.plidx[i] );
		if( cp_status_profile != NULL ){
			WFLBY_SYSTEM_GetProfileMyStatus( cp_status_profile, p_wk->p_mystate[i], heapID );
		}
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�����p�����[�^������
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_ROOM_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	WFLBY_ROOM_PARAM* p_param;
	
	p_data->p_param = sys_AllocMemory( heapID, sizeof(WFLBY_ROOM_PARAM) );
	memset( p_data->p_param, 0, sizeof(WFLBY_ROOM_PARAM) );

	p_param = p_data->p_param;

	p_param->p_save		= p_sys->p_savedata;
	p_param->p_system	= p_sys->p_system;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����p�����[�^�j��
 *
 *	@param	p_data	�V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_ROOM_Exit( WFLBY_APLDATA* p_data )
{
	GF_ASSERT(p_data->p_param);
	sys_FreeMemoryEz( p_data->p_param );
	p_data->p_param = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����v���b�N�J�n
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_ROOM_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	WFLBY_ROOM_PARAM* p_param;

	// �I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY(wifilobby_room);

	// �}�Ӄv���b�N�f�[�^
	static const PROC_DATA Proc = {	
		WFLBY_ROOM_Init,
		WFLBY_ROOM_Main,
		WFLBY_ROOM_Exit,
		FS_OVERLAY_ID(wifilobby_room)
	};
	p_param				= p_data->p_param;
	p_param->season		= WFLBY_SYSTEM_GetSeason( p_sys->p_system );
	p_param->room		= WFLBY_SYSTEM_GetRoomType( p_sys->p_system );
	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����v���b�N�I������
 *
 *	@param	p_sys	�V�X�e��
 *	@param	p_data	�A�v���f�[�^
 *
 *	@return	�A�v���V�X�e���̖߂�l
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_ROOM_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	WFLBY_ROOM_PARAM* p_param;
	p_param = p_data->p_param;
	
	switch( p_param->ret ){
	// �o������o���̂ŏI��
	case WFLBY_ROOM_RET_ROOM_OUT:			
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_LOGOUT );
		break;
	// ���E���v��
	case WFLBY_ROOM_RET_ROOM_WORLDTIMER:
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_WORLDTIMER );
		break;
	// �g�s�b�N��
	case WFLBY_ROOM_RET_ROOM_TOPIC:	
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_TOPIC );
		break;
	// ���Ճ{�[�h��
	case WFLBY_ROOM_RET_ROOM_FOOT1:	
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_FOOT1 );
		break;
	// ���Ճ{�[�h��
	case WFLBY_ROOM_RET_ROOM_FOOT2:	
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_FOOT2 );
		break;
	// �ʓ���
	case WFLBY_ROOM_RET_ROOM_BALLSLOW:		
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_BALLSLOW );
		break;
	// �ʏ��
	case WFLBY_ROOM_RET_ROOM_BALANCE:		
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_BALANCEBALL );
		break;
	// ���D����
	case WFLBY_ROOM_RET_ROOM_BALLOON:		
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_BALLOON );
		break;
	// �^�C���A�E�g�I��
	case WFLBY_ROOM_RET_ROOM_TIMEOUT:
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_TIMEOUTLOGOUT );
		break;

	// �A���P�[�g�@����
	case WFLBY_ROOM_RET_ROOM_ANKETO_INPUT:	
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ANKETO_INPUT );
		break;

	// �A���P�[�g�@�o��	
	case WFLBY_ROOM_RET_ROOM_ANKETO_OUTPUT:	
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ANKETO_OUTPUT );
		break;

#ifdef PM_DEBUG
	case WFLBY_ROOM_RET_ROOM_RETURN:			// �����ɂ�����x�߂�
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ROOM);
		break;
#endif

	default:
		GF_ASSERT(0);
		break;
	}
	return WFLBY_APL_RET_NONE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���E���v�V�X�e��	������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_data		�A�v���f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_WORLDTIMER_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	WLDTIMER_PARAM* p_param;

	p_data->p_param = sys_AllocMemory( heapID, sizeof(WLDTIMER_PARAM) );
	memset( p_data->p_param, 0, sizeof(WLDTIMER_PARAM) );

	p_param = p_data->p_param;

	// �f�[�^�ݒ�
	p_param->p_save		= p_sys->p_savedata;
	p_param->p_system	= p_sys->p_system;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���v�V�X�e��	�j��
 *
 *	@param	p_data		�A�v���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_WORLDTIMER_Exit( WFLBY_APLDATA* p_data )
{
	sys_FreeMemoryEz( p_data->p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���v�J�n
 *
 *	@param	p_sys	�A�v���V�X�e��
 *	@param	p_data	�A�v���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_WORLDTIMER_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	WLDTIMER_PARAM* p_param;

	// �I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY(worldtimer);

	// �}�Ӄv���b�N�f�[�^
	static const PROC_DATA Proc = {	
		WLDTIMER_Init,
		WLDTIMER_Main,
		WLDTIMER_Exit,
		FS_OVERLAY_ID(worldtimer)
	};

	p_param = p_data->p_param;

	p_param->cp_data	= WFLBY_SYSTEM_GetWldTimer( p_sys->p_system );
	p_param->worldtime	= *(WFLBY_SYSTEM_GetWldTime( p_sys->p_system ));

	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���E���v�I��
 *	
 *	@param	p_sys	�A�v���V�X�e��
 *	@param	p_data	�A�v���f�[�^
 *
 *	@return	�˂�NONE
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_WORLDTIMER_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �������I����Ă��Ȃ����`�F�b�N
	if( WFLBY_SYSTEM_Event_GetEnd( p_sys->p_system ) ){
		// �^�C���A�E�g�I��
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_TIMEOUTLOGOUT );
	}else{
		// �����ɖ߂�
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ROOM );
	}
	
	return WFLBY_APL_RET_NONE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�p�����[�^������
 *
 *	@param	p_sys		�A�v���V�X�e��
 *	@param	p_data		�A�v���f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_TOPIC_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	NEWS_DRAW_DATA* p_param;

	p_data->p_param = sys_AllocMemory( heapID, sizeof(NEWS_DRAW_DATA) );
	memset( p_data->p_param, 0, sizeof(NEWS_DRAW_DATA) );
	
	p_param = p_data->p_param;

	// �f�[�^�ݒ�
	p_param->p_data			= WFLBY_SYSTEM_GetLobbyData( p_sys->p_system );
	p_param->cp_nowtime		= WFLBY_SYSTEM_GetWldTime( p_sys->p_system );
	p_param->p_system		= p_sys->p_system;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�p�����[�^�j��
 *
 *	@param	p_data		�A�v���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_TOPIC_Exit( WFLBY_APLDATA* p_data )
{
	sys_FreeMemoryEz( p_data->p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�J�n
 *
 *	@param	p_sys		�A�v���V�X�e��
 *	@param	p_data		�A�v���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_TOPIC_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY(lobbynews);

	// �}�Ӄv���b�N�f�[�^
	static const PROC_DATA Proc = {	
		NEWS_DRAW_Init,
		NEWS_DRAW_Main,
		NEWS_DRAW_Exit,
		FS_OVERLAY_ID(lobbynews)
	};

	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�s�b�N�v���b�N�I��
 *
 *	@param	p_sys		�A�v���V�X�e��
 *	@param	p_data		�A�v���f�[�^
 *
 *	@return	���NONE
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_TOPIC_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �������I����Ă��Ȃ����`�F�b�N
	if( WFLBY_SYSTEM_Event_GetEnd( p_sys->p_system ) ){
		// �^�C���A�E�g�I��
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_TIMEOUTLOGOUT );
	}else{
		// �����ɖ߂�
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ROOM );
	}
	return WFLBY_APL_RET_NONE;
}


//----------------------------------------------------------------------------
/**
 *	@brief		���O�C���v���b�N�J�n
 *
 *	@param	p_sys		�A�v���V�X�e��
 *	@param	p_data		�A�v���f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_LOGIN_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	WFLBY_CONNECT_PARAM* p_param;

	p_data->p_param = sys_AllocMemory( heapID, sizeof(WFLBY_CONNECT_PARAM) );
	memset( p_data->p_param, 0, sizeof(WFLBY_CONNECT_PARAM) );
	
	p_param = p_data->p_param;

	// �f�[�^�ݒ�
	p_param->p_save				= p_sys->p_savedata;
	p_param->p_system			= p_sys->p_system;
	p_param->check_skip			= p_sys->check_skip;
	p_param->p_wflby_counter	= p_sys->p_wflby_counter;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�C���v���b�N�I��
 *
 *	@param	p_data	�A�v���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_LOGIN_Exit( WFLBY_APLDATA* p_data )
{
	WFLBY_CONNECT_PARAM* p_param;
	p_param = p_data->p_param;
	sys_FreeMemoryEz( p_data->p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�C���v���b�N	�J�n
 *
 *	@param	p_sys	�A�v���V�X�e��
 *	@param	p_data	�A�v���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_LOGIN_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY(wifilobby_connect);

	// �}�Ӄv���b�N�f�[�^
	static const PROC_DATA Proc = {	
		WFLBY_CONNECT_Init,
		WFLBY_CONNECT_Main,
		WFLBY_CONNECT_Exit,
		FS_OVERLAY_ID(wifilobby_connect)
	};

	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�C������	�I��
 *
 *	@param	p_sys	�A�v���V�X�e��
 *	@param	p_data	�A�v���f�[�^
 *
 *	@return	���NONE
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_LOGIN_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	WFLBY_CONNECT_PARAM* p_param;

	
	p_param = p_data->p_param;


	// ���ꂽ���`�F�b�N
	if( p_param->enter == FALSE ){
		// �����
		return WFLBY_APL_RET_END;
	}

	// �����ɓ���
	WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ROOM );
	return WFLBY_APL_RET_NONE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�{�[������	�p�����[�^������
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_BS_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	BUCKET_PROC_WORK* p_param;

	p_data->p_param = sys_AllocMemory( heapID, sizeof(BUCKET_PROC_WORK) );
	memset( p_data->p_param, 0, sizeof(BUCKET_PROC_WORK) );
	
	p_param = p_data->p_param;

	p_param->vchat		= FALSE;
	p_param->wifi_lobby	= TRUE;
	p_param->p_save		= p_sys->p_savedata;

	// �~�j�Q�[�����ʃ��[�N�������m��
	WFLBY_APL_MINIGAME_AllocWk( &p_param->lobby_wk, heapID );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�[�������p�����[�^�j��
 *
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_BS_Exit( WFLBY_APLDATA* p_data )
{
	BUCKET_PROC_WORK* p_param;


	// �~�j�Q�[�����ʃ��[�N�������j��
	p_param = p_data->p_param;
	WFLBY_APL_MINIGAME_FreeWk( &p_param->lobby_wk );
	
	sys_FreeMemoryEz( p_data->p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�[�������A�v���J�n
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_BS_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	FS_EXTERN_OVERLAY(bucket);
	FS_EXTERN_OVERLAY(minigame_common);
	// �v���Z�X��`�f�[�^
	static const PROC_DATA Proc = {
		BucketProc_Init,
		BucketProc_Main,
		BucketProc_End,
		FS_OVERLAY_ID(bucket),
	};

	// �~�j�Q�[�����ʃI�[�o�[���C��ǂݍ���
	Overlay_Load( FS_OVERLAY_ID(minigame_common), OVERLAY_LOAD_NOT_SYNCHRONIZE);

	{
		BUCKET_PROC_WORK* p_param;

		p_param = p_data->p_param;
		WFLBY_APL_MINIGAME_InitWk( &p_param->lobby_wk, p_sys->p_system, p_sys->heapID );
	}
	
	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʓ���	�v���b�N�I��
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�A�v���f�[�^
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_BS_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{

	// �~�j�Q�[�����ʃI�[�o�[���C�j��
	FS_EXTERN_OVERLAY(minigame_common);
	Overlay_UnloadID( FS_OVERLAY_ID(minigame_common) );


	// �K�W�F�b�g���A�b�v�f�[�g����Ă���\��������̂Ń`�F�b�N
	// �G���[�I�����������ݒ�
	{
		BUCKET_PROC_WORK* p_param;
		p_param = p_data->p_param;
		WFLBY_SYSTEM_MG_SetErrorEnd( p_sys->p_system, p_param->lobby_wk.error_end );
		WFLBY_SYSTEM_SetMyItem( p_sys->p_system, p_param->lobby_wk.gadget );
	}
	
	// �����ɖ߂�
	WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ROOM );
	
	return WFLBY_APL_RET_NONE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	BALANCE�{�[���p�����[�^������
 *
 *	@param	p_sys		�A�v���V�X�e��
 *	@param	p_data		�A�v���f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_BB_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	BB_PROC_WORK* p_param;
	
	p_data->p_param = sys_AllocMemory( heapID, sizeof(BB_PROC_WORK) );
	memset( p_data->p_param, 0, sizeof(BB_PROC_WORK) );

	p_param = p_data->p_param;
	
	p_param->vchat		= FALSE;
	p_param->wifi_lobby	= TRUE;
	p_param->p_save		= p_sys->p_savedata;

	// �~�j�Q�[�����ʃ��[�N�������m��
	WFLBY_APL_MINIGAME_AllocWk( &p_param->lobby_wk, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BALANCE�{�[���p�����[�^�j��
 *
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_BB_Exit( WFLBY_APLDATA* p_data )
{
	BB_PROC_WORK* p_param;

	// �~�j�Q�[�����ʃ��[�N�������j��
	p_param = p_data->p_param;
	WFLBY_APL_MINIGAME_FreeWk( &p_param->lobby_wk );

	sys_FreeMemoryEz( p_data->p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BALANCE�{�[���A�v���J�n
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_BB_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	FS_EXTERN_OVERLAY(balance_ball);
	FS_EXTERN_OVERLAY(minigame_common);
	// �v���Z�X��`�f�[�^
	static const PROC_DATA Proc = {
		BalanceBallProc_Init,
		BalanceBallProc_Main,
		BalanceBallProc_Exit,
		FS_OVERLAY_ID(balance_ball),
	};

	// �~�j�Q�[�����ʃI�[�o�[���C��ǂݍ���
	Overlay_Load( FS_OVERLAY_ID(minigame_common), OVERLAY_LOAD_NOT_SYNCHRONIZE);

	{
		BB_PROC_WORK* p_param;
		p_param = p_data->p_param;
		WFLBY_APL_MINIGAME_InitWk( &p_param->lobby_wk, p_sys->p_system, p_sys->heapID );
	}
	
	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BALANCE�{�[��	�v���b�N�I��
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�A�v���f�[�^
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_BB_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �~�j�Q�[�����ʃI�[�o�[���C�j��
	FS_EXTERN_OVERLAY(minigame_common);
	Overlay_UnloadID( FS_OVERLAY_ID(minigame_common) );

	// �K�W�F�b�g���A�b�v�f�[�g����Ă���\��������̂Ń`�F�b�N
	{
		BB_PROC_WORK* p_param;
		p_param = p_data->p_param;
		WFLBY_SYSTEM_MG_SetErrorEnd( p_sys->p_system, p_param->lobby_wk.error_end );
		WFLBY_SYSTEM_SetMyItem( p_sys->p_system, p_param->lobby_wk.gadget );
	}
	
	// �����ɖ߂�
	WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ROOM );
	
	return WFLBY_APL_RET_NONE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���犄	�p�����[�^������
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_BL_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	BALLOON_PROC_WORK* p_param;

	p_data->p_param = sys_AllocMemory( heapID, sizeof(BALLOON_PROC_WORK) );
	memset( p_data->p_param, 0, sizeof(BALLOON_PROC_WORK) );
	
	p_param = p_data->p_param;

	p_param->vchat		= FALSE;
	p_param->wifi_lobby	= TRUE;
	p_param->p_save		= p_sys->p_savedata;

	// �~�j�Q�[�����ʃ��[�N�������m��
	WFLBY_APL_MINIGAME_AllocWk( &p_param->lobby_wk, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���犄�p�����[�^�j��
 *
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_BL_Exit( WFLBY_APLDATA* p_data )
{
	BALLOON_PROC_WORK* p_param;
	
	// �~�j�Q�[�����ʃ��[�N�������j��
	p_param = p_data->p_param;
	WFLBY_APL_MINIGAME_FreeWk( &p_param->lobby_wk );
	
	sys_FreeMemoryEz( p_data->p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���犄	�A�v���J�n
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_BL_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	FS_EXTERN_OVERLAY(balloon);
	FS_EXTERN_OVERLAY(minigame_common);
	// �v���Z�X��`�f�[�^
	static const PROC_DATA Proc = {
		BalloonProc_Init,
		BalloonProc_Main,
		BalloonProc_End,
		FS_OVERLAY_ID(balloon),
	};

	// �~�j�Q�[�����ʃI�[�o�[���C��ǂݍ���
	Overlay_Load( FS_OVERLAY_ID(minigame_common), OVERLAY_LOAD_NOT_SYNCHRONIZE);

	{
		BALLOON_PROC_WORK* p_param;
		p_param = p_data->p_param;
		WFLBY_APL_MINIGAME_InitWk( &p_param->lobby_wk, p_sys->p_system, p_sys->heapID );
		
	}
	
	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BALANCE�{�[��	�v���b�N�I��
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�A�v���f�[�^
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_BL_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �~�j�Q�[�����ʃI�[�o�[���C�j��
	FS_EXTERN_OVERLAY(minigame_common);
	Overlay_UnloadID( FS_OVERLAY_ID(minigame_common) );

	// �K�W�F�b�g���A�b�v�f�[�g����Ă���\��������̂Ń`�F�b�N
	{
		BALLOON_PROC_WORK* p_param;
		p_param = p_data->p_param;
		WFLBY_SYSTEM_MG_SetErrorEnd( p_sys->p_system, p_param->lobby_wk.error_end );
		WFLBY_SYSTEM_SetMyItem( p_sys->p_system, p_param->lobby_wk.gadget );
	}
	
	
	// �����ɖ߂�
	WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ROOM );
	
	return WFLBY_APL_RET_NONE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	���Ճ{�[�h�@�p�����[�^������
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_FOOT_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	FOOTPRINT_PARAM* p_param;
	
	p_data->p_param = sys_AllocMemory( heapID, sizeof(FOOTPRINT_PARAM) );
	memset( p_data->p_param, 0, sizeof(FOOTPRINT_PARAM) );

	p_param = p_data->p_param;

	p_param->wflby_sys		= p_sys->p_system;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ճ{�[�h	�p�����[�^�j��
 *
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_FOOT_Exit( WFLBY_APLDATA* p_data )
{
	sys_FreeMemoryEz( p_data->p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ճ{�[�h�I��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_FOOT_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �������I����Ă��Ȃ����`�F�b�N
	if( WFLBY_SYSTEM_Event_GetEnd( p_sys->p_system ) ){
		// �^�C���A�E�g�I��
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_TIMEOUTLOGOUT );
	}else{
		// �����ɖ߂�
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ROOM );
	}
	
	return WFLBY_APL_RET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ճ{�[�h���J�n
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_FOOT1_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	FOOTPRINT_PARAM* p_param;

	FS_EXTERN_OVERLAY(footprint_board);
	// �v���Z�X��`�f�[�^
	static const PROC_DATA Proc = {
		FootPrintProc_Init,
		FootPrintProc_Main,
		FootPrintProc_End,
		FS_OVERLAY_ID(footprint_board),
	};

	p_param = p_data->p_param;
	p_param->board_type		= FOOTPRINT_BOARD_TYPE_WHITE;

	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ճ{�[�h���J�n
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_FOOT2_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	FOOTPRINT_PARAM* p_param;

	FS_EXTERN_OVERLAY(footprint_board);
	// �v���Z�X��`�f�[�^
	static const PROC_DATA Proc = {
		FootPrintProc_Init,
		FootPrintProc_Main,
		FootPrintProc_End,
		FS_OVERLAY_ID(footprint_board),
	};

	p_param = p_data->p_param;
	p_param->board_type		= FOOTPRINT_BOARD_TYPE_BLACK;

	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}
	

//----------------------------------------------------------------------------
/**
 *	@brief	���O�A�E�g�����p�����[�^�������쐬
 *
 *	@param	p_sys		�A�v���V�X�e��
 *	@param	p_data		�A�v���f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_LOGOUT_Init( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	WFLBY_DISCONNECT_PARAM* p_param;

	p_data->p_param = sys_AllocMemory( heapID, sizeof(WFLBY_DISCONNECT_PARAM) );
	memset( p_data->p_param, 0, sizeof(WFLBY_DISCONNECT_PARAM) );

	p_param = p_data->p_param;

	// �f�[�^�ݒ�
	p_param->p_save			= p_sys->p_savedata;
	p_param->p_system		= p_sys->p_system;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�A�E�g�����p�����[�^�j��
 *
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_LOGOUT_Exit( WFLBY_APLDATA* p_data )
{
	sys_FreeMemoryEz( p_data->p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�A�E�g�A�v���J�n
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_LOGOUT_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	WFLBY_APLDATA_Logout_StartDef( p_sys, p_data, FALSE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���A�E�g���O�A�E�g�A�v���J�n
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_TIMEOUTLOGOUT_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	WFLBY_APLDATA_Logout_StartDef( p_sys, p_data, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�A�E�g�J�n	���ʕ���
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 *	@param	timeout		�^�C���A�E�g
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_Logout_StartDef( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data, BOOL timeout )
{
	// �I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY(wifilobby_connect);

	// �}�Ӄv���b�N�f�[�^
	static const PROC_DATA Proc = {	
		WFLBY_DISCONNECT_Init,
		WFLBY_DISCONNECT_Main,
		WFLBY_DISCONNECT_Exit,
		FS_OVERLAY_ID(wifilobby_connect)
	};

	{
		WFLBY_DISCONNECT_PARAM* p_param;
		p_param = p_data->p_param;
		p_param->timeout = timeout;
	}

	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�A�E�g�A�v���I��
 *	
 *	@param	p_sys		�V�X�e��
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_LOGOUT_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �I���
	return WFLBY_APL_RET_END;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g	������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_data		�f�[�^���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_ANKETO_InitInput( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	ANKETO_PARAM* p_param;

	p_data->p_param = sys_AllocMemory( heapID, sizeof(ANKETO_PARAM) );
	memset( p_data->p_param, 0, sizeof(ANKETO_PARAM) );
	
	p_param = p_data->p_param;

	// �f�[�^�ݒ�
	p_param->p_save			= p_sys->p_savedata;
	p_param->p_system		= p_sys->p_system;
	p_param->move_type		= ANKETO_MOVE_TYPE_INPUT;
}
static void WFLBY_APLDATA_ANKETO_InitOutput( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data,  u32 heapID )
{
	ANKETO_PARAM* p_param;

	p_data->p_param = sys_AllocMemory( heapID, sizeof(ANKETO_PARAM) );
	memset( p_data->p_param, 0, sizeof(ANKETO_PARAM) );
	
	p_param = p_data->p_param;

	// �f�[�^�ݒ�
	p_param->p_save			= p_sys->p_savedata;
	p_param->p_system		= p_sys->p_system;
	p_param->move_type		= ANKETO_MOVE_TYPE_VIEW;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g	�j��
 *
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_ANKETO_Exit( WFLBY_APLDATA* p_data )
{
	sys_FreeMemoryEz( p_data->p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g	�J�n
 *	
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_data		�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_APLDATA_ANKETO_Start( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY(wifilobby_anketo);

	// �}�Ӄv���b�N�f�[�^
	static const PROC_DATA Proc = {	
		ANKETO_Init,
		ANKETO_Main,
		ANKETO_Exit,
		FS_OVERLAY_ID(wifilobby_anketo)
	};

	WFLBY_APL_PROC_Start( p_sys, p_data, &Proc );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A���P�[�g�I��
 *
 *	@param	p_sys		���[�N
 *	@param	p_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static WFLBY_APL_RET WFLBY_APLDATA_ANKETO_End( WFLBY_APL* p_sys, WFLBY_APLDATA* p_data )
{
	// �������I����Ă��Ȃ����`�F�b�N
	if( WFLBY_SYSTEM_Event_GetEnd( p_sys->p_system ) ){
		// �^�C���A�E�g�I��
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_TIMEOUTLOGOUT );
	}else{
		// �����ɖ߂�
		WFLBY_APLDATA_Start( p_sys, WFLBY_APL_ROOM );
	}
	return WFLBY_APL_RET_NONE;
}


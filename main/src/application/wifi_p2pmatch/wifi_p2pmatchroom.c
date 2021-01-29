//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_matchroom.c
 *	@brief		wifi	�}�b�`���O���[��
 *	@author		tomoya takahashi
 *	@data		2007.01.31
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "system/procsys.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "system/wipe.h"
#include "system/lib_pack.h"
#include "system/snd_tool.h"

#include "wifi/dwc_rap.h"
#include "wifi_p2pmatch_se.h"
#include "wifi/dwc_rapfriend.h"

#include "communication/communication.h"
#include "communication/comm_state.h"
#include "system/snd_tool.h"  //sndTOOL

#include "wifip2pmatch.naix"			// �O���t�B�b�N�A�[�J�C�u��`

#include "include/system/pm_debug_wifi.h"

#include "application/wifi_p2pmatch_def.h"

#define __WIFI_P2PMATCHROOM_H_GLOBAL
#include "wifi_p2pmatchroom.h"


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
#ifdef PM_DEBUG
//#define MCR_DEBUG_2CCHAR_CHECK	// 2DMAPSYSTEM�̕\���`�F�b�N�p
#endif

#ifdef MCR_DEBUG_2CCHAR_CHECK
static u32 s_MCR_DEBUG_2CCHAR_VIEW_ID;
static u32 s_MCR_DEBUG_2CCHAR_VIEW_COUNT = 0;
#endif



//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
// �o�^�ʒu�f�[�^
#define MCR_MOVEOBJ_ADD_POSNUM	(52)

// ���N�G�X�g�R�}���h�L���[
#define MCR_REQCMDQ_NUM	( 128 )


#define MCR_MAP_GRID_SIZE	(WF2DMAP_GRID_SIZ)	// �}�b�v�P�O���b�h�T�C�Y


// ������
enum{
	MCR_MOVEOBJ_ST_PL_KEYWAIT,		// �v���C���[�ʏ퓮��
	MCR_MOVEOBJ_ST_KURUKURU,		// �����鏈��
	MCR_MOVEOBJ_ST_KURUKURU_DEL,	// ��鏈��
	MCR_MOVEOBJ_ST_NPC,				// NPC����
	MCR_MOVEOBJ_ST_NPC_JUMP,		// NPC��W��
	MCR_MOVEOBJ_ST_NPC_PAUSE,		// NPC�����~
	MCR_MOVEOBJ_ST_NUM,
};


enum{	// ����
	MCR_MOVEOBJ_WAY_TOP = WF2DMAP_WAY_UP,
	MCR_MOVEOBJ_WAY_BOTTOM = WF2DMAP_WAY_DOWN,
	MCR_MOVEOBJ_WAY_LEFT = WF2DMAP_WAY_LEFT,
	MCR_MOVEOBJ_WAY_RIGHT = WF2DMAP_WAY_RIGHT,
	MCR_MOVEOBJ_WAY_NUM
};
#define MCR_MOVEOBJ_BG_PRI	( 3 )
#define MCR_MOVEOBJ_PLAYER_FRIENDNO	(0)

#define MCR_MOVEOBJ_WALK_COUNT		(8)	// �����V���N��
#define MCR_MOVEOBJ_KURU_COUNT		(45)// ���邭��V���N��
#define MCR_MOVEOBJ_NPC_RAND_S		(256)// NPC�����ς���^�C�~���O�ŏ�
#define MCR_MOVEOBJ_NPC_RAND_M		(1024)// NPC�����ς���^�C�~���O�ő�

#define MCR_MOVEOBJ_WALK_DIST		( MCR_MAP_GRID_SIZE/MCR_MOVEOBJ_WALK_COUNT )	// 1�V���N�ɓ������W
#define MCR_MOVEOBJ_KURU_DIST		( -192 )
#define MCR_MOVEOBJ_NPC_JUMP_EFFCOUNT	( 6 )// �W�����v���Ă��鎞��
#define MCR_MOVEOBJ_NPC_JUMP_WAITCOUNT	( 16 )// ���̃W�����v�܂ł̃E�G�C�g
#define MCR_MOVEOBJ_NPC_JUMP_COUNT		( (MCR_MOVEOBJ_NPC_JUMP_EFFCOUNT*2)+MCR_MOVEOBJ_NPC_JUMP_WAITCOUNT )// �P�W�����v�J�E���g
#define MCR_MOVEOBJ_NPC_JUMP_DIS	( 4 )// �W�����v�̍���
#define MCR_MOVEOBJ_NPC_JUMP_RMAX	(180)// ��]��MAX


// �G�t�F�N�g���\�[�X
#define MCR_EFFECTRES_BGPRI		(2)	// BG�D�揇��
#define MCR_EFFECTRES_SOFTPRI	(0)// BG�D�揇��
#define MCR_EFFECTRES_OFS_Y		(32)// Y���W�␳�n
#define MCR_EFFECTRES_OFS_X		(8)// Y���W�␳�n
#define MCR_EFFECTWAKURES_OFS_Y		(0)// Y���W�␳�n
#define MCR_EFFECTWAKURES_OFS_X		(8)// Y���W�␳�n

// ���\�[�X�Ǘ�ID
#define MCR_EFFECTRES_CONTID	( 50 )	// �G�t�F�N�g���\�[�X�Ǘ�ID


// PC�A�j���[�V����
enum{
	// �ҋ@���̃A�j��
	MCR_PCANM_ALL_SEQ_OFF,
	MCR_PCANM_ALL_SEQ_ON,
	MCR_PCANM_ALL_SEQ_NUM,

	// ���s���̃A�j��
	MCR_PCANM_USE_SEQ_NONE = 0,
	MCR_PCANM_USE_SEQ_ON00,
	MCR_PCANM_USE_SEQ_OFF01,
	MCR_PCANM_USE_SEQ_ON01,
	MCR_PCANM_USE_SEQ_WAIT,
	MCR_PCANM_USE_SEQ_OFF02,
	MCR_PCANM_USE_SEQ_ON02,
	MCR_PCANM_USE_SEQ_OFF03,
	MCR_PCANM_USE_SEQ_NUM,

	// �J���[�^�C�v
	MCR_PCANM_COL_ON = 0,
	MCR_PCANM_COL_OFF,
	MCR_PCANM_COL_WAIT,
};
#define MCR_PCANM_BGPLTT	( 7 )	// �A�j��������p���b�g
#define MCR_PCANM_BGPLTTOFS	(1)		// �A�j��������p���b�g�̊J�n�I�t�Z�b�g
#define MCR_PCANM_PCNUM			(4)	// PC�̐�
#define MCR_PCANM_DESTPL(x)	( (MCR_PCANM_BGPLTT*32) + (((x)+MCR_PCANM_BGPLTTOFS)*2) )	// �p���b�g�]����A�h���X�擾

//-------------------------------------
///	�A�j���f�[�^
//=====================================
typedef struct {
	u8 flag;		// �J�E���g�L��
	u8 count_num;	// �J�E���g�l
	u8 trans;		// �]���L��
	u8 trans_flag;	// �]���t���O
} MCR_PCANM_DATA;
static const MCR_PCANM_DATA AnmData[ MCR_PCANM_USE_SEQ_NUM ] = {
	{ FALSE,	0,	FALSE,	MCR_PCANM_COL_OFF },
	{ TRUE,		2, TRUE,	MCR_PCANM_COL_ON },
	{ TRUE,		4, TRUE,	MCR_PCANM_COL_OFF },
	{ TRUE,		2, TRUE,	MCR_PCANM_COL_ON },
	{ FALSE,	0,	FALSE,	MCR_PCANM_COL_OFF },
	{ TRUE,		2, TRUE,	MCR_PCANM_COL_OFF },
	{ TRUE,		4, TRUE,	MCR_PCANM_COL_ON },
	{ TRUE,		3, TRUE,	MCR_PCANM_COL_OFF },
};

static const u16 AllAnmData[ MCR_PCANM_ALL_SEQ_NUM ] = {
	3,
	2,
};



//-----------------------------------------------------------------------------
/**
 *		�}�N��
 */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	����I�u�W�F�N�g�o�^�ʒu
//=====================================
typedef struct {
	s16	x;
	s16	y;
} MCR_MOVEOBJ_ONPOS;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

static void WcrMoveObjAllDel( WIFI_MATCHROOM* p_mcr );

static BOOL WcrMoveObjKeyInputCheck( const WIFI_MATCHROOM* cp_mcr );
static BOOL WcrMoveObjPCSelectCheck( const WIFI_MATCHROOM* cp_mcr );
static void WcrMoveObjCmdJudgeAndCmdMove( WIFI_MATCHROOM* p_mcr );
static void WcrMoveObjCmdReq( WIFI_MATCHROOM* p_mcr );
static void WcrMoveObjDraw( WIFI_MATCHROOM* p_mcr );
static const MCR_MOVEOBJ* WcrMoveObjGetHitCheck( const WIFI_MATCHROOM* cp_mcr, const MCR_MOVEOBJ* cp_obj, WF2DMAP_WAY way );


static void WcrObjDrawInit( WIFI_MATCHROOM* p_mcr, u32 hero_view, u32 heapID );
static void WcrObjDrawExit( WIFI_MATCHROOM* p_mcr );

static void WcrScrnDrawInit( WIFI_MATCHROOM* p_mcr, u32 heapID, ARCHANDLE* p_handle, u32 map_no );
static void WcrScrnDrawExit( WIFI_MATCHROOM* p_mcr );

static void WcrClactInit( MCR_CLACT* p_clact, u32 heapID, ARCHANDLE* p_handle );
static void WcrClactDest( MCR_CLACT* p_clact );
static void WcrClactResLoad( MCR_CLACT* p_clact, u32 heapID, ARCHANDLE* p_handle );
static void WcrClactResRelease( MCR_CLACT* p_clact );
static void WcrClactAdd( MCR_CLACT* p_clact, u32 heapID );
static void WcrClactDel( MCR_CLACT* p_clact );
static void WcrBgContInit( GF_BGL_INI* p_bgl, u32 heapID );
static void WcrBgContDest( GF_BGL_INI* p_bgl );
static void WcrBgSet( GF_BGL_INI* p_bgl, u32 heapID, ARCHANDLE* p_handle );

static BOOL WcrExitCheck( WIFI_MATCHROOM* p_mcr );

static void WcrClactResEffectLoad( MCR_CLACT* p_clact, u32 heapID, ARCHANDLE* p_handle );
static void WcrClactResEffectRelease( MCR_CLACT* p_clact );
static void WcrClactResEffectAdd( MCR_CLACT* p_clact, u32 heapID );
static void WcrClactResEffectDel( MCR_CLACT* p_clact );
static void WcrClactResEffectExitDrawOn( WIFI_MATCHROOM* p_mcr );
static void WcrClactResEffectExitDrawOff( WIFI_MATCHROOM* p_mcr );
static void WcrClactResEffectCursorDrawOn( WIFI_MATCHROOM* p_mcr, WF2DMAP_POS pos, u32 pri );
static void WcrClactResEffectCursorDrawOff( WIFI_MATCHROOM* p_mcr );


static void WcrMapGetNpcSetPos( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ_ONPOS* p_pos, u32 friendNo );
static void WcrMapGetPlayerSetPos( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ_ONPOS* p_pos );

static MCR_MOVEOBJ* WcrMoveObj_GetClean( WIFI_MATCHROOM* p_mcr );
static void WcrMoveObj_SetUpGraphic( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, BOOL hero );
static void WcrMoveObj_SetUpMove( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, const MCR_MOVEOBJ_ONPOS* cp_pos, u32 friendNo, u16 charaid, u8 way, WF2DMAP_OBJST status );
static BOOL WcrMoveObj_CheckAccess( const MCR_MOVEOBJ* cp_obj );

static void WcrMoveObj_SetMoveFuncPlayer( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static void WcrMoveObj_SetMoveFuncKuruKuruInit( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static void WcrMoveObj_SetMoveFuncNpc( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static void WcrMoveObj_SetMoveFuncNpcJump( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static u32 WcrMoveObj_GetRetWay( u32 way );

static void WcrMoveObj_DrawFuncDefault( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static void WcrMoveObj_DrawFuncKuruKuru( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static void WcrMoveObj_DrawFuncNpcJump( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );

static BOOL WcrMoveObj_MoveFuncPlayerKeyWait( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static BOOL WcrMoveObj_MoveFuncKuruKuru( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static BOOL WcrMoveObj_MoveFuncKuruKuruDel( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static BOOL WcrMoveObj_MoveFuncNpc( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static BOOL WcrMoveObj_MoveFuncNpcJump( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
static BOOL WcrMoveObj_MoveFuncNpcPause( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );

static void WcrMoveObj_ReqCmdSet( WIFI_MATCHROOM* p_mcr, s32 cmd, s32 way, s32 playid );
static void WcrMoveObj_ReqCmdSetEasy( WIFI_MATCHROOM* p_mcr, s32 cmd, const MCR_MOVEOBJ* cp_obj );

static BOOL WcrMoveObj_MoveSetOkCheck( const MCR_MOVEOBJ* cp_obj );
static void WcrMoveObj_MoveSet( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, MCR_NPC_MOVETYPE moveID );


static void WcrPCANM_Init( WIFI_MATCHROOM* p_sys, MCR_PCANM* p_wk, ARCHANDLE* p_handle );
static void WcrPCANM_Delete( WIFI_MATCHROOM* p_sys, MCR_PCANM* p_wk );
static void WcrPCANM_Main( WIFI_MATCHROOM* p_sys, MCR_PCANM* p_wk );
static void WcrPCANM_AllMain( MCR_PCANM* p_wk );
static void WcrPCANM_UseMain( MCR_PCANM* p_wk );
static void* WcrPCANM_GetAnmSrc( MCR_PCANM* p_wk, u32 on_off );
static void WcrPCANM_UseAnmEnd( MCR_PCANM* p_wk );
static void WcrPCANM_UseAnmNext( MCR_PCANM* p_wk );
static void WcrPCANM_UseStart( MCR_PCANM* p_wk, u8 pc_no );
static void WcrPCANM_UseEndReq( MCR_PCANM* p_wk );

//----------------------------------------------------------------------------
/**
 *	@brief	WIFI�@�}�b�`���O���[���@������	���[�N�����\�[�X�ǂݍ��݂��s��
 *
 *	@param	p_mcr		���[�N
 *	@param	heapID		�q�[�vID
 *	@param	p_handle	�A�[�J�C�u�n���h��
 *	@param	p_bgl		BGL
 *	@param	hero_view	��l���̐���
 *	@param	friendNum	�F�B�̑���
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_Init( WIFI_MATCHROOM* p_mcr, u32 heapID, ARCHANDLE* p_handle, GF_BGL_INI* p_bgl, u32 hero_view, u32 friendNum )
{
	WF2DMAP_POS map_siz;
	u32 map_no;
	
	memset( p_mcr, 0, sizeof(WIFI_MATCHROOM) );

	map_no = (friendNum-1) / WCR_MAPDATA_1BLOCKOBJNUM;

	p_mcr->use_heap = heapID;
	p_mcr->p_bgl	= p_bgl;
	
	// CLACT INIT
	WcrClactInit( &p_mcr->clact, p_mcr->use_heap, p_handle );

	// BGL�@������
	WcrBgContInit( p_mcr->p_bgl, heapID );

	// �}�b�v�f�[�^������
	map_siz = WcrMapGridSizGet( map_no );
	p_mcr->p_mapsys = WF2DMAP_MAPSysInit( map_siz.x, map_siz.y, heapID );
	WF2DMAP_MAPSysDataSet( p_mcr->p_mapsys, WcrMapDataGet( map_no ) );

	// �I�u�W�F�N�g�Ǘ��V�X�e���쐬
	p_mcr->p_objsys = WF2DMAP_OBJSysInit( MCR_MOVEOBJNUM, heapID );

	// �I�u�W�F�N�g�\���V�X�e������
	WcrObjDrawInit( p_mcr, hero_view, heapID );

	// �X�N���[���f�[�^������
	WF2DMAP_SCROLLSysDataInit( &p_mcr->scroll );

	// �X�N���[���`��V�X�e��������
	WcrScrnDrawInit( p_mcr, heapID, p_handle, map_no );
	WcrBgSet( p_mcr->p_bgl, heapID, p_handle );

	// PC���ɂߏ�����
	WcrPCANM_Init( p_mcr, &p_mcr->pc_anm, p_handle );

	// ���N�G�X�g�R�}���h�L���[����
	p_mcr->p_reqcmdQ = WF2DMAP_REQCMDQSysInit( MCR_REQCMDQ_NUM, heapID );
	
	// ����������
	p_mcr->init = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WIFI	�}�b�`���O���[���@�j���@���[�N�����\�[�X�j��
 *
 *	@param	p_mcr	���[�N
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_Dest( WIFI_MATCHROOM* p_mcr )
{
	// ���N�G�X�g�R�}���h�L���[�j��
	WF2DMAP_REQCMDQSysExit( p_mcr->p_reqcmdQ );
	
	// �X�N���[���`��V�X�e���͂�
	WcrScrnDrawExit( p_mcr );

	// PC���ɂߔj��
	WcrPCANM_Delete( p_mcr, &p_mcr->pc_anm );

	// �S����I�u�W�F�N�g�j��
	WcrMoveObjAllDel( p_mcr );

	// �I�u�W�F�N�g�\���V�X�e���j��
	WcrObjDrawExit( p_mcr );

	// �I�u�W�F�N�g�V�X�e���͂�
	WF2DMAP_OBJSysExit( p_mcr->p_objsys );

	// �}�b�v�f�[�^�͂�
	WF2DMAP_MAPSysExit( p_mcr->p_mapsys );

	// �Z���A�N�^�[�j��
	WcrClactDest( &p_mcr->clact );

	// BGL�j��
	WcrBgContDest( p_mcr->p_bgl );

	memset( p_mcr, 0, sizeof(WIFI_MATCHROOM) );
}

//----------------------------------------------------------------------------
/**
 *	@brief
 *
 *	@param	const WIFI_MATCHROOM* cp_mcr 
 *
 *	@return
 */
//-----------------------------------------------------------------------------
BOOL WIFI_MCR_GetInitFlag( const WIFI_MATCHROOM* cp_mcr )
{
	return cp_mcr->init;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���C������
 *
 *	@param	p_mcr	���[�N
 *
 *	@retval	MCR_RET_NONE,		// �Ȃ�
 *	@retval	MCR_RET_CANCEL,		// �L�����Z��
 *	@retval	MCR_RET_SELECT,		// �I��
 *	@retval MCR_RET_MYSELECT,	// �����I��
 */
//-----------------------------------------------------------------------------
u32 WIFI_MCR_Main( WIFI_MATCHROOM* p_mcr )
{
	BOOL result;

	// �I�u�W�F�N�g����
	WF2DMAP_OBJSysMain( p_mcr->p_objsys );

	// �R�}���h���s����
	WcrMoveObjCmdReq( p_mcr );

	// �R�}���h���f�������R�}���h���s����
	WcrMoveObjCmdJudgeAndCmdMove( p_mcr );
	
	// �X�N���[������
	WF2DMAP_SCRContSysMain( &p_mcr->scroll, p_mcr->p_player->p_obj );	
	
	// �X�N���[���\������
	WF2DMAP_SCRDrawSysMain( p_mcr->p_scrdraw, &p_mcr->scroll );	

	// �I�u�W�F�N�g�\���f�[�^�X�V����
	WF2DMAP_OBJDrawSysUpdata( p_mcr->p_objdraw );

	// �C�x���g�I�u�W�F�N�g�\���f�[�^�X�V����
	WcrMoveObjDraw( p_mcr );

	// �`�F�b�N�O�ɏo��������
	WcrClactResEffectExitDrawOff( p_mcr );

	// �L�[���͏���
	if( WcrMoveObjKeyInputCheck( p_mcr ) == TRUE ){

		// �o���}�b�g�̏�ɗ����Ă��邩�`�F�b�N
		result = WcrExitCheck( p_mcr );

		if( result == TRUE ){

			// �}�b�g�̏�ŉ��������Ă���̂ŁA�����o��
			WcrClactResEffectExitDrawOn( p_mcr );

			// ����ɉ�����������A�������o��
			if( sys.cont & PAD_KEY_DOWN ){
				return MCR_RET_CANCEL;
			}
		}
		if( sys.trg & PAD_BUTTON_DECIDE ){
			// ���l��I���������`�F�b�N
			if( WIFI_MCR_PlayerSelect( p_mcr ) > 0 ){
				return MCR_RET_SELECT;

			}else if( WcrMoveObjPCSelectCheck( p_mcr ) == TRUE ){

				// �ڂ̑O�Ȃ�PC�Ȃ烊�X�g���o��
				return MCR_RET_MYSELECT;
			}
		}
	}
	
	return MCR_RET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\��
 *
 *	@param	p_mcr	���[�N
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_Draw( WIFI_MATCHROOM* p_mcr )
{
	if( p_mcr->init ){
		CLACT_Draw( p_mcr->clact.clactSet );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�𒆂̗F�B�R�[�h���擾����
 *
 *	@param	cp_mcr	���[�N
 */
//-----------------------------------------------------------------------------
u8 WIFI_MCR_PlayerSelect( const WIFI_MATCHROOM* cp_mcr )
{
	GF_ASSERT( cp_mcr->p_player != NULL );	// ��l��������̂��H
	return cp_mcr->p_player->accesFriend;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l���̏���Ă���}�b�v�f�[�^�擾
 *
 *	@param	cp_mcr	���[�N
 *
 *	@return	�}�b�v�p�����[�^
 */
//-----------------------------------------------------------------------------
u32 WIFI_MCR_GetPlayerOnMapParam( const WIFI_MATCHROOM* cp_mcr )
{
	WF2DMAP_POS pos;
	
	GF_ASSERT( cp_mcr->p_player != NULL );	// ��l��������̂��H

	pos = WF2DMAP_OBJWkMatrixGet( cp_mcr->p_player->p_obj );
	return WF2DMAP_MAPSysParamGet( cp_mcr->p_mapsys, WF2DMAP_POS2GRID(pos.x), WF2DMAP_POS2GRID(pos.y) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l���̏���Ă��鉺�̃}�b�v�f�[�^�擾
 *
 *	@param	cp_mcr	���[�N
 *
 *	@return	�}�b�v�p�����[�^
 */
//-----------------------------------------------------------------------------
u32 WIFI_MCR_GetPlayerOnUnderMapParam( const WIFI_MATCHROOM* cp_mcr )
{
	WF2DMAP_POS pos;
	
	GF_ASSERT( cp_mcr->p_player != NULL );	// ��l��������̂��H

	pos = WF2DMAP_OBJWkMatrixGet( cp_mcr->p_player->p_obj );
	return WF2DMAP_MAPSysParamGet( cp_mcr->p_mapsys, WF2DMAP_POS2GRID(pos.x), WF2DMAP_POS2GRID(pos.y)+1 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�[�\���\��
 *
 *	@param	p_mcr			�V�X�e�����[�N
 *	@param	cp_obj			�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_CursorOn( WIFI_MATCHROOM* p_mcr, const MCR_MOVEOBJ* cp_obj )
{
	WF2DMAP_POS pos;
	u32 pri;
	pos = WF2DMAP_OBJWkMatrixGet( cp_obj->p_obj );
	pri = WF2DMAP_OBJDrawWkDrawPriGet( cp_obj->p_draw );
	pri --;
	WcrClactResEffectCursorDrawOn( p_mcr, pos, pri-1 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�[�\����\��
 *
 *	@param	p_mcr 
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_CursorOff( WIFI_MATCHROOM* p_mcr )
{
	WcrClactResEffectCursorDrawOff( p_mcr );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�̓�����~�ݒ肷��
 *
 *	@param	p_mcr		�}�b�`���O���[���f�[�^
 *	@param	flag 
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_PlayerMovePause( WIFI_MATCHROOM* p_mcr, BOOL flag )
{
	p_mcr->player_pause = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�̓����~�t���O���擾
 *
 *	@param	cp_mcr		�}�b�`���O���[���f�[�^
 *
 *	@retval	TRUE	��~��
 *	@retval	FALSE	���s��
 */
//-----------------------------------------------------------------------------
BOOL WIFI_MCR_PlayerMovePauseGet( const WIFI_MATCHROOM* cp_mcr )
{
	return cp_mcr->player_pause;
}


//----------------------------------------------------------------------------
/**
 *	@brief	��l���o�^
 *
 *	@param	p_mcr		���[�N
 *	@param	view		�p
 *
 *	@retval	�o�^�����I�u�W�F�N�g�|�C���^
 *	@retval	NULL �����o�^�ł��Ȃ�
 */
//-----------------------------------------------------------------------------
MCR_MOVEOBJ* WIFI_MCR_SetPlayer( WIFI_MATCHROOM* p_mcr, u32 view )
{
	MCR_MOVEOBJ* p_obj;
	MCR_MOVEOBJ_ONPOS pos;

	// �󂢂Ă���I�u�W�F�擾
	p_obj = WcrMoveObj_GetClean( p_mcr );
	
	// ��l���͎���Ă���
	p_mcr->p_player = p_obj;

	WcrMapGetPlayerSetPos( p_mcr, &pos );

#ifdef MCR_DEBUG_2CCHAR_CHECK
	// ����ݒ�
	WcrMoveObj_SetUpMove( p_mcr, p_obj, &pos, 
			MCR_MOVEOBJ_PLAYER_FRIENDNO, AMBRELLA, MCR_MOVEOBJ_WAY_TOP, WF2DMAP_OBJST_NONE );
	s_MCR_DEBUG_2CCHAR_VIEW_ID = AMBRELLA;
#else
	// ����ݒ�
	WcrMoveObj_SetUpMove( p_mcr, p_obj, &pos, 
			MCR_MOVEOBJ_PLAYER_FRIENDNO, view, MCR_MOVEOBJ_WAY_TOP, WF2DMAP_OBJST_NONE );
#endif

	// �v���C���[����ݒ�
	WcrMoveObj_SetMoveFuncPlayer( p_mcr, p_obj );

	// �O���t�B�b�N�ݒ�
	WcrMoveObj_SetUpGraphic( p_mcr, p_obj, TRUE );

	// �v���C���[���o�^���ꂽ���x�X�N���[�������킹��
	// �X�N���[������
	WF2DMAP_SCRContSysMain( &p_mcr->scroll, p_mcr->p_player->p_obj );	
	// �X�N���[���\������
	WF2DMAP_SCRDrawSysMain( p_mcr->p_scrdraw, &p_mcr->scroll );	
	
	return p_obj;
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC�I�u�W�F�N�g�@�o�^
 *
 *	@param	p_mcr		���[�N
 *	@param	view		�p
 *	@param	friendNo	�F�B�ԍ�
 *
 *	@retval	�I�u�W�F�N�g���[�N
 *	@retval	NULL				�o�^���s
 */
//-----------------------------------------------------------------------------
MCR_MOVEOBJ* WIFI_MCR_SetNpc( WIFI_MATCHROOM* p_mcr, u32 view, u8 friendNo )
{
	MCR_MOVEOBJ* p_obj;
	MCR_MOVEOBJ_ONPOS pos;
	WF2DMAP_POS hero_pos;

	// �󂢂Ă���I�u�W�F�擾
	p_obj = WcrMoveObj_GetClean( p_mcr );
	
	// �o�^�ꏊ��T��
	// -1���Ă���͎̂�l�����O�ԂƂ����F�B�ԍ��\���ɂȂ��Ă��邽��
	WcrMapGetNpcSetPos( p_mcr, &pos, friendNo - 1 );

	// ���̏ꏊ�Ɏ�l�������Ȃ����`�F�b�N
	if( p_mcr->p_player ){
		hero_pos = WF2DMAP_OBJWkMatrixGet( p_mcr->p_player->p_obj );
		if( (hero_pos.x == pos.x) &&
			(hero_pos.y == pos.y) ){
			return NULL;
		}
	}

	// ����ݒ�
	WcrMoveObj_SetUpMove( p_mcr, p_obj, &pos, 
			friendNo, view, MCR_MOVEOBJ_WAY_BOTTOM, WF2DMAP_OBJST_BUSY );

	// ����֐�
	WcrMoveObj_SetMoveFuncKuruKuruInit( p_mcr, p_obj );


	// �O���t�B�b�N�ݒ�
	WcrMoveObj_SetUpGraphic( p_mcr, p_obj, FALSE );
	
	return p_obj;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�j�����N�G�X�g
 *
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_DelPeopleReq( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	s32 way;

	way = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_WAY );
	
	p_obj->pMove = WcrMoveObj_MoveFuncKuruKuruDel;
	p_obj->pDraw = WcrMoveObj_DrawFuncKuruKuru;
	p_obj->move_st	= MCR_MOVEOBJ_ST_KURUKURU_DEL;
	p_obj->move_count = 0;

	// �R�}���h���sOFF
	WF2DMAP_OBJDrawWkUpdataFlagSet( p_obj->p_draw, FALSE );
	  
	// ���邭��A�j���X�^�[�g
	WF2DMAP_OBJDrawWkKuruAnimeStart( p_obj->p_draw );

	// �Z������ԂɕύX����
	WcrMoveObj_ReqCmdSetEasy( p_mcr, WF2DMAP_OBJST_BUSY, p_obj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����I�ɔj��
 *
 *	@param	p_obj	���[�N
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_DelPeople( MCR_MOVEOBJ* p_obj )
{
	// 2D�L�����N�^�j��
	WF2DMAP_OBJDrawWkDel( p_obj->p_draw );

	// �I�u�W�F�N�g�Ǘ��j��
	WF2DMAP_OBJWkDel( p_obj->p_obj );

	memset( p_obj, 0, sizeof(MCR_MOVEOBJ) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�ԍ����擾����
 *
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 *
 *	@return	�F�B�ԍ�
 */
//-----------------------------------------------------------------------------
u8	WIFI_MCR_GetFriendNo( const MCR_MOVEOBJ* cp_obj )
{
	return WF2DMAP_OBJWkDataGet( cp_obj->p_obj, WF2DMAP_OBJPM_PLID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ڂ��擾
 *
 *	@param	cp_obj	���[�N
 *
 *	@return	������
 */
//-----------------------------------------------------------------------------
u32	WIFI_MCR_GetView( const MCR_MOVEOBJ* cp_obj )
{
	return WF2DMAP_OBJWkDataGet( cp_obj->p_obj, WF2DMAP_OBJPM_CHARA );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����Ă���t�������擾
 *
 *	@param	cp_obj	�I�u�W�F�N�g���[�N
 *
 *	@return	�t����
 */
//-----------------------------------------------------------------------------
WF2DMAP_WAY	WIFI_MCR_GetRetWay( const MCR_MOVEOBJ* cp_obj )
{
	WF2DMAP_WAY way;

	way = WF2DMAP_OBJWkDataGet( cp_obj->p_obj, WF2DMAP_OBJPM_WAY );
	return WF2DMPA_OBJToolRetWayGet( way );
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC�̓����~�@	�����Œ�
 *
 *	@param	p_mcr	�V�X�e�����[�N
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_NpcPauseOn( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, WF2DMAP_WAY way )
{
	u32 playid;
	WF2DMAP_POS mat;

	// ���̓���֐���ۑ����Ă���
	p_obj->pMoveTmp = p_obj->pMove;
	p_obj->pMove = WcrMoveObj_MoveFuncNpcPause;

	p_obj->move_st_tmp	= p_obj->move_st;
	p_obj->move_st		= MCR_MOVEOBJ_ST_NPC_PAUSE;

	playid = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_PLID );
	WcrMoveObj_ReqCmdSet( p_mcr, WF2DMAP_CMD_NONE, way, playid );

	// �\������������ɕύX
	// �܂��X�V��~
	WF2DMAP_OBJDrawWkUpdataFlagSet( p_obj->p_draw, FALSE );
	WF2DMAP_OBJDrawWkWaySet( p_obj->p_draw, way );

	// �Ȃ񂩂��Ă���Ƃ�������̂�
	// ���W�����ɖ߂�
	mat.x = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_X );
	mat.y = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_Y );
	WF2DMAP_OBJDrawWkMatrixSet( p_obj->p_draw, mat );
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC�̓���J�n
 *
 *	@param	p_mcr	�V�X�e�����[�N
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_NpcPauseOff( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	// 080708	tomoya
	// NULL�Ȃ�Ȃ������
	if( p_obj == NULL ){
		return ;
	}
	if( p_obj->pMoveTmp == NULL ){
		return ;	// �����|�[�Y��Ԃ�������Ă���
	}
	p_obj->pMove = p_obj->pMoveTmp;
	p_obj->pMoveTmp = NULL;
	p_obj->move_st	= p_obj->move_st_tmp;
	WF2DMAP_OBJDrawWkUpdataFlagSet( p_obj->p_draw, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC�̓���^�C�v�ύX
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	moveID		����^�C�v
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_NpcMoveSet( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, MCR_NPC_MOVETYPE moveID )
{
	BOOL result;
	
	// ���邭�铮�슮�����Ă��邩�`�F�b�N
	result = WcrMoveObj_MoveSetOkCheck( p_obj );
	if( result == FALSE ){
		// �t���O�����ݒ肵�Ă���
		p_obj->moveID = moveID;
	}else{
		// ���ݒ肵�Ă����v
		WcrMoveObj_MoveSet( p_mcr, p_obj, moveID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�p��PC�A�j���J�n
 *
 *	@param	p_mcr	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_PCAnmStart( WIFI_MATCHROOM* p_mcr )
{
	s8 map_param;
	// PC�̑O�ɂ��邩�H
	if( WcrMoveObjPCSelectCheck( p_mcr ) == TRUE ){
		// ����PC���H
		map_param = WIFI_MCR_GetPlayerOnUnderMapParam( p_mcr );
		map_param -= MCR_MAPPM_MAP00;
		GF_ASSERT( (map_param >= 0) && (map_param < 4) );

		// �J�n�I
		WcrPCANM_UseStart( &p_mcr->pc_anm, map_param );
		return;
	}

	GF_ASSERT( 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j�����I��������
 *
 *	@param	p_mcr	���[�N
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_PCAnmOff( WIFI_MATCHROOM* p_mcr )
{
	WcrPCANM_UseEndReq( &p_mcr->pc_anm );
}

//----------------------------------------------------------------------------
/**
 *	@brief	PC�A�j�����C������
 *
 *	@param	p_mcr	���[�N
 */
//-----------------------------------------------------------------------------
void WIFI_MCR_PCAnmMain( WIFI_MATCHROOM* p_mcr )
{
	// �p�\�R���A�j��
	WcrPCANM_Main( p_mcr, &p_mcr->pc_anm );
}


//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	�S���j��
 *
 *	@param	p_mcr	���[�N
 */
//-----------------------------------------------------------------------------
static void WcrMoveObjAllDel( WIFI_MATCHROOM* p_mcr )
{
	int i;

	for( i=0; i<MCR_MOVEOBJNUM; i++ ){
		if( p_mcr->moveObj[i].p_obj != NULL ){
			WIFI_MCR_DelPeople( &p_mcr->moveObj[i] );
		}
	}
}	

//----------------------------------------------------------------------------
/**
 *	@brief	�L�[���͂��Ă悢���`�F�b�N
 *
 *	@param	cp_mcr	�V�X�e�����[�N
 *
 *	@retval	TRUE	Key���͂��Ă��悢
 *	@retval	FALSE	Key���͂��Ă͂����Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WcrMoveObjKeyInputCheck( const WIFI_MATCHROOM* cp_mcr )
{
	s32 status;

	status = WF2DMAP_OBJWkDataGet( cp_mcr->p_player->p_obj, WF2DMAP_OBJPM_ST );
	if( status == WF2DMAP_OBJST_NONE ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l����PC�̑O�ɂ���̂��`�F�b�N
 *
 *	@param	cp_mcr		�V�X�e�����[�N
 *
 *	@retval	TRUE	PC�I��
 *	@retval	FALSE	PC��I��
 */
//-----------------------------------------------------------------------------
static BOOL WcrMoveObjPCSelectCheck( const WIFI_MATCHROOM* cp_mcr )
{
	u32 param;
	WF2DMAP_POS pos;
	WF2DMAP_WAY way;

	pos = WF2DMAP_OBJWkMatrixGet( cp_mcr->p_player->p_obj );
	way = WF2DMAP_OBJWkDataGet( cp_mcr->p_player->p_obj, WF2DMAP_OBJPM_WAY );
	pos = WF2DMAP_OBJToolWayPosGet( pos, way );
	param = WF2DMAP_MAPSysParamGet( cp_mcr->p_mapsys, WF2DMAP_POS2GRID(pos.x), WF2DMAP_POS2GRID(pos.y) );

	// ��������Ă���K�v������
	if( way == WF2DMAP_WAY_UP ){
		if( param == MCR_MAPPM_PC ){
			return TRUE;
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�}���h���f���R�}���h���s����
 *
 *	@param	p_mcr	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WcrMoveObjCmdJudgeAndCmdMove( WIFI_MATCHROOM* p_mcr )
{
	WF2DMAP_REQCMD req;
	WF2DMAP_ACTCMD act;
	BOOL result;

	// �R�}���h���ԃR�}���h�����s
	while( WF2DMAP_REQCMDQSysCmdPop( p_mcr->p_reqcmdQ, &req ) == TRUE ){
		result = WF2DMAP_JUDGESysCmdJudge( p_mcr->p_mapsys, p_mcr->p_objsys, &req, &act );
		if( result == TRUE ){
			WF2DMAP_OBJSysCmdSet( p_mcr->p_objsys, &act );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�}���h���N�G�X�g����
 *
 *	@param	p_mcr	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WcrMoveObjCmdReq( WIFI_MATCHROOM* p_mcr )
{
	// ����֐����܂킷
	int i;
	BOOL ret;

	for( i=0; i<MCR_MOVEOBJNUM; i++ ){
		if( p_mcr->moveObj[i].p_obj != NULL ){
			
			ret = p_mcr->moveObj[i].pMove( p_mcr, &p_mcr->moveObj[i] );
			if( ret == TRUE ){
				// �j��
				WIFI_MCR_DelPeople( &p_mcr->moveObj[i] );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�C�x���g�\������
 *
 *	@param	p_mcr	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WcrMoveObjDraw( WIFI_MATCHROOM* p_mcr )
{
	int i;

	for( i=0; i<MCR_MOVEOBJNUM; i++ ){
		if( p_mcr->moveObj[i].p_obj != NULL ){
			p_mcr->moveObj[i].pDraw( p_mcr, &p_mcr->moveObj[i] );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�ʂ��̓����蔻��
 *
 *	@param	cp_mcr		�V�X�e�����[�N
 *	@param	cp_obj		�I�u�W�F���[�N
 *	@param	way			�������i�ޕ���
 */
//-----------------------------------------------------------------------------
static const MCR_MOVEOBJ* WcrMoveObjGetHitCheck( const WIFI_MATCHROOM* cp_mcr, const MCR_MOVEOBJ* cp_obj, WF2DMAP_WAY way )
{
	const WF2DMAP_OBJWK* cp_wk;
	int i;
	

	cp_wk = WF2DMAP_OBJSysHitCheck( cp_obj->p_obj, cp_mcr->p_objsys, way );
	
	if( cp_wk != NULL ){
		// ���̃I�u�W�F�������Ă�I�u�W�F��T��
		for( i=0; i<MCR_MOVEOBJNUM; i++ ){
			if( cp_mcr->moveObj[i].p_obj == cp_wk ){
				return &cp_mcr->moveObj[i];
			}
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^�\���V�X�e������
 *
 *	@param	p_mcr		���[�N
 *	@param	heapID		�g�p�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WcrObjDrawInit( WIFI_MATCHROOM* p_mcr, u32 hero_view, u32 heapID )
{
	int i;
	
#ifdef MCR_DEBUG_2CCHAR_CHECK
	p_mcr->p_objdraw = WF2DMAP_OBJDrawSysInit( p_mcr->clact.clactSet, NULL, MCR_MOVEOBJNUM, 
			NNS_G2D_VRAM_TYPE_2DMAIN, heapID );


	WF2DMAP_OBJDrawSysResSet( p_mcr->p_objdraw, AMBRELLA, WF_2DC_MOVENORMAL, heapID );
#else
	p_mcr->p_objdraw = WF2DMAP_OBJDrawSysInit_Shadow( p_mcr->clact.clactSet, NULL, MCR_MOVEOBJNUM, 
			hero_view, WF_2DC_MOVERUN, NNS_G2D_VRAM_TYPE_2DMAIN, heapID );

	// �L�����N�^�f�[�^�̓o�^
	// UNION�L�����N�^
	WF2DMAP_OBJDrawSysUniResSet( p_mcr->p_objdraw, WF_2DC_MOVETURN, heapID );	
#endif


}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^�\���V�X�e���j��
 *
 *	@param	p_mcr		���[�N
 */
//-----------------------------------------------------------------------------
static void WcrObjDrawExit( WIFI_MATCHROOM* p_mcr )
{
	WF2DMAP_OBJDrawSysExit( p_mcr->p_objdraw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���\���V�X�e��������
 *
 *	@param	p_mcr		���[�N
 *	@param	heapID		�q�[�vID
 *	@param	p_handle	�n���h��
 */
//-----------------------------------------------------------------------------
static void WcrScrnDrawInit( WIFI_MATCHROOM* p_mcr, u32 heapID, ARCHANDLE* p_handle, u32 map_no )
{
	WF2DMAP_SCRDRAWINIT init = {
		CLACT_U_EASYRENDER_SURFACE_MAIN,
		GF_BGL_FRAME0_M,
		GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe000,
		GX_BG_CHARBASE_0x00000,
		GX_BG_EXTPLTT_01,
		MCR_MOVEOBJ_BG_PRI,
		FALSE,
		ARC_WIFIP2PMATCH_GRA,
		NARC_wifip2pmatch_wf_match_top_room_1_NSCR,
		FALSE
	};

	// �O���t�B�b�N�f�[�^��ݒ�
	init.dataid_scrn += map_no;

	p_mcr->p_scrdraw = WF2DMAP_SCRDrawSysInit( 
			&p_mcr->clact.renddata, p_mcr->p_bgl, &init, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���\���V�X�e���j��
 *
 *	@param	p_mcr		���[�N
 */	
//-----------------------------------------------------------------------------
static void WcrScrnDrawExit( WIFI_MATCHROOM* p_mcr )
{
	WF2DMAP_SCRDrawSysExit( p_mcr->p_scrdraw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�@������
 *
 *	@param	p_clact		�Z���A�N�^�[���[�N
 *	@param	heapID		�q�[�v
 *	@param	p_handle	�A�[�J�C�u�n���h��
 */
//-----------------------------------------------------------------------------
static void WcrClactInit( MCR_CLACT* p_clact, u32 heapID, ARCHANDLE* p_handle )
{
	int i;

	// �Z���A�N�^�[�Z�b�g�쐬
	p_clact->clactSet = CLACT_U_SetEasyInit( MCR_CLACT_OBJNUM, &p_clact->renddata, heapID );
    CLACT_U_SetSubSurfaceMatrix( &p_clact->renddata, 0, MCR_CLACTSUBSURFACE_Y );

	// �L�����ƃp���b�g�̃��\�[�X�}�l�[�W���쐬
	for( i=0; i<MCR_CLACT_RESNUM; i++ ){
		p_clact->resMan[i] = CLACT_U_ResManagerInit(MCR_CLACT_LOADRESNUM, i, heapID);
	}

	// �l�����\�[�X�ǂݍ��݂ƃL�����N�^�p���b�g�̓]��
	// �G�t�F�N�g���\�[�X�ǂݍ��݂ƃL�����N�^�p���b�g�̓]��
	WcrClactResLoad( p_clact, heapID, p_handle );

	// �A�N�^�[�̓o�^
	WcrClactAdd( p_clact, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�f�[�^�j��
 *	
 *	@param	p_clact		�Z���A�N�^�[���[�N
 */
//-----------------------------------------------------------------------------
static void WcrClactDest( MCR_CLACT* p_clact )
{
	int i;

	// �A�N�^�[�̔j��
	WcrClactDel( p_clact );
	
	// ���\�[�X�j��
	WcrClactResRelease( p_clact );
	
	// ���\�[�X�}�l�[�W���j��
	for( i=0; i<MCR_CLACT_RESNUM; i++ ){
		CLACT_U_ResManagerDelete( p_clact->resMan[i] );
	}

	// �Z���A�N�^�[�Z�b�g�j��
	CLACT_DestSet( p_clact->clactSet );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���A�G�t�F�N�g�̃��\�[�X�ǂݍ��݁��]��
 *
 *	@param	p_clact		�Z���A�N�^�[���[�N
 *	@param	heapID		�q�[�vID
 *	@param	p_handle	�A�[�J�C�u�n���h��
 */
//-----------------------------------------------------------------------------
static void WcrClactResLoad( MCR_CLACT* p_clact, u32 heapID, ARCHANDLE* p_handle )
{
	//�@�G�t�F�N�g���\�[�X�Ǎ���
	WcrClactResEffectLoad( p_clact, heapID, p_handle );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[���\�[�X�j��
 *
 *	@param	p_clact		�Z���A�N�^�[���[�N
 */
//-----------------------------------------------------------------------------
static void WcrClactResRelease( MCR_CLACT* p_clact )
{
	// �G�t�F�N�g���\�[�X�j��
	WcrClactResEffectRelease( p_clact );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[���[�N�o�^
 *
 *	@param	p_clact		�Z���A�N�^�[�f�[�^���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WcrClactAdd( MCR_CLACT* p_clact, u32 heapID )
{
	WcrClactResEffectAdd( p_clact, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[���[�N�j��
 *	
 *	@param	p_clact		�Z���A�N�^�[�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WcrClactDel( MCR_CLACT* p_clact )
{
	WcrClactResEffectDel( p_clact );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�R���g���[���ݒ�
 *
 *	@param	p_bgl	BGL
 *	@param	heapID	�g�p�q�[�v
 */
//-----------------------------------------------------------------------------
static void WcrBgContInit( GF_BGL_INI* p_bgl, u32 heapID )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�p����BGCONT��j������
 *
 *	@param	p_bgl	BGL
 */
//-----------------------------------------------------------------------------
static void WcrBgContDest( GF_BGL_INI* p_bgl )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG��ݒ�
 *
 *	@param	p_bgl		BGL
 *	@param	heapID		�q�[�v
 *	@param	p_handle	�n���h��
 */
//-----------------------------------------------------------------------------
static void WcrBgSet( GF_BGL_INI* p_bgl, u32 heapID, ARCHANDLE* p_handle )
{
	// �p���b�g�]��
	// �g�p���Ă悢�p���b�g��MAX�@8
	ArcUtil_HDL_PalSet( p_handle, NARC_wifip2pmatch_wf_match_top_room_NCLR, PALTYPE_MAIN_BG, 0, 8*32, heapID );	
	// �o�b�N�O���E���h�J���[�͕ς��Ȃ�
	GF_BGL_BackGroundColorSet( GF_BGL_FRAME0_M, 0 );
	
    // ���C�����BG2�L�����]��
    ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_wf_match_top_room_NCGR, p_bgl,
                       GF_BGL_FRAME0_M, 0, 0, 0, heapID);
/*
    // ���C�����BG2�X�N���[���]��
    ArcUtil_HDL_ScrnSet(   p_handle, NARC_wifip2pmatch_wf_match_top_room_1_NSCR+map_no, p_bgl,
                       GF_BGL_FRAME0_M, 0, 0, 0, heapID);
//*/
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G�t�F�N�g�̃��\�[�X�Ǎ���
 *
 *	@param	p_clact		�Z���A�N�^�[���[�N
 *	@param	heapID		�q�[�v
 *	@param	p_handle	�n���h��
 */
//-----------------------------------------------------------------------------
static void WcrClactResEffectLoad( MCR_CLACT* p_clact, u32 heapID, ARCHANDLE* p_handle )
{
	BOOL result;
	
	p_clact->effect.resobj[0] = 
		CLACT_U_ResManagerResAddArcChar_ArcHandle( p_clact->resMan[0], 
				p_handle, NARC_wifip2pmatch_wf_match_top_room_obj_NCGR,
				FALSE, MCR_EFFECTRES_CONTID, NNS_G2D_VRAM_TYPE_2DMAIN, heapID );

	p_clact->effect.resobj[1] = 
		CLACT_U_ResManagerResAddArcPltt_ArcHandle( p_clact->resMan[1], 
				p_handle, NARC_wifip2pmatch_wf_match_top_room_obj_NCLR,
				FALSE, MCR_EFFECTRES_CONTID, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapID );

	p_clact->effect.resobj[2] = 
		CLACT_U_ResManagerResAddArcKindCell_ArcHandle( p_clact->resMan[2], 
				p_handle, NARC_wifip2pmatch_wf_match_top_room_obj_NCER,
				FALSE, MCR_EFFECTRES_CONTID, CLACT_U_CELL_RES, heapID );

	p_clact->effect.resobj[3] = 
		CLACT_U_ResManagerResAddArcKindCell_ArcHandle( p_clact->resMan[3], 
				p_handle, NARC_wifip2pmatch_wf_match_top_room_obj_NANR,
				FALSE, MCR_EFFECTRES_CONTID, CLACT_U_CELLANM_RES, heapID );

	// Vram�]��
	result = CLACT_U_CharManagerSetAreaCont( p_clact->effect.resobj[0] );
	GF_ASSERT( result == TRUE );
	result = CLACT_U_PlttManagerSetCleanArea( p_clact->effect.resobj[1] );
	GF_ASSERT( result == TRUE );
	
	// ���������烊�\�[�X��j��
	CLACT_U_ResManagerResOnlyDelete( p_clact->effect.resobj[0] );
	CLACT_U_ResManagerResOnlyDelete( p_clact->effect.resobj[1] );

	// �w�b�_�[�쐬
	CLACT_U_MakeHeader( &p_clact->effect.header, 
			 MCR_EFFECTRES_CONTID, MCR_EFFECTRES_CONTID, 
			 MCR_EFFECTRES_CONTID, MCR_EFFECTRES_CONTID,
			 CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
			 0, MCR_EFFECTRES_BGPRI, 
			 p_clact->resMan[0], p_clact->resMan[1],
			 p_clact->resMan[2], p_clact->resMan[3],
			 NULL, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G�t�F�N�g�̃��\�[�X�j��
 *
 *	@param	p_clact		�Z���A�N�^�[���[�N
 */
//-----------------------------------------------------------------------------
static void WcrClactResEffectRelease( MCR_CLACT* p_clact )
{
	int i;

	CLACT_U_CharManagerDelete( p_clact->effect.resobj[0] );
	CLACT_U_PlttManagerDelete( p_clact->effect.resobj[1] );
	
	for( i=0; i<4; i++ ){
		CLACT_U_ResManagerResDelete( p_clact->resMan[i], p_clact->effect.resobj[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G�t�F�N�g�A�N�^�[�o�^
 *
 *	@param	p_clact		�Z���A�N�^�[�f�[�^���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WcrClactResEffectAdd( MCR_CLACT* p_clact, u32 heapID )
{
	CLACT_ADD add;

	// �O�N���A
	memset( &add, 0, sizeof(CLACT_ADD) );
	
	add.ClActSet = p_clact->clactSet;
	add.ClActHeader = &p_clact->effect.header;
	add.DrawArea = NNS_G2D_VRAM_TYPE_2DMAIN;
	add.sca.x = FX32_ONE;
	add.sca.y = FX32_ONE;
	add.pri = MCR_EFFECTRES_SOFTPRI;
	add.heap  = heapID;

	p_clact->effect.exit_cursor = CLACT_Add( &add );
	p_clact->effect.obj_waku = CLACT_Add( &add );

	// �\��OFF
	CLACT_SetDrawFlag( p_clact->effect.exit_cursor, FALSE );
	CLACT_SetDrawFlag( p_clact->effect.obj_waku, FALSE );

	// �J�[�\���̓I�[�g�A�j��
	CLACT_SetAnmFlag( p_clact->effect.exit_cursor, TRUE );

	// �g�̓A�j���P
	CLACT_AnmChg( p_clact->effect.obj_waku, 1 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G�t�F�N�g�j��
 *
 *	@param	p_clact		�Z���A�N�^�[�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WcrClactResEffectDel( MCR_CLACT* p_clact )
{
	CLACT_Delete( p_clact->effect.exit_cursor );
	CLACT_Delete( p_clact->effect.obj_waku );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���`��ON
 *
 *	@param	p_mcr		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WcrClactResEffectExitDrawOn( WIFI_MATCHROOM* p_mcr )
{
	MCR_MOVEOBJ_ONPOS pos;
	VecFx32 mat;
	
	// �o�����W���擾 �␳���ăA�N�^�[�ɐݒ�
	WcrMapGetPlayerSetPos( p_mcr, &pos );

	pos.x += MCR_EFFECTRES_OFS_X;
	pos.y += MCR_EFFECTRES_OFS_Y;
	mat.x = pos.x << FX32_SHIFT;
	mat.y = pos.y << FX32_SHIFT;
	CLACT_SetMatrix( p_mcr->clact.effect.exit_cursor, &mat );
	
	
	// ���̍��W�ŕ\��ON
	CLACT_SetDrawFlag( p_mcr->clact.effect.exit_cursor, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���`��OFF
 *
 *	@param	p_mcr		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WcrClactResEffectExitDrawOff( WIFI_MATCHROOM* p_mcr )
{
	CLACT_SetDrawFlag( p_mcr->clact.effect.exit_cursor, FALSE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�[�\��ON
 *
 *	@param	p_mcr		�V�X�e�����[�N
 *	@param	pos			�J�[�\�����W
 *	@param	pri			�\���D�揇��
 */	
//-----------------------------------------------------------------------------
static void WcrClactResEffectCursorDrawOn( WIFI_MATCHROOM* p_mcr, WF2DMAP_POS pos, u32 pri )
{
	VecFx32 mat;

	pos.x += MCR_EFFECTWAKURES_OFS_X;
	pos.y += MCR_EFFECTWAKURES_OFS_Y;
	mat.x = pos.x << FX32_SHIFT;
	mat.y = pos.y << FX32_SHIFT;
	CLACT_SetMatrix( p_mcr->clact.effect.obj_waku, &mat );

	CLACT_DrawPriorityChg( p_mcr->clact.effect.obj_waku, pri );
	
	// ���̍��W�ŕ\��ON
	CLACT_SetDrawFlag( p_mcr->clact.effect.obj_waku, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�[�\��OFF
 *
 *	@param	p_mcr 
 */
//-----------------------------------------------------------------------------
static void WcrClactResEffectCursorDrawOff( WIFI_MATCHROOM* p_mcr )
{
	CLACT_SetDrawFlag( p_mcr->clact.effect.obj_waku, FALSE );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�o���`�F�b�N�̗L��
 *
 *	@param	p_mcr	���[�N
 *
 *	@retval	TRUE	�o���ɂ̂���
 *	@retval	FALSE	�o���ɂ̂��Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WcrExitCheck( WIFI_MATCHROOM* p_mcr )
{
	WF2DMAP_POS pos;
	u32 map_param;
	WF2DMAP_WAY	way;
	
	// ��l�������邩�`�F�b�N
	if( p_mcr->p_player == NULL ){
		return FALSE;
	}

	// ���W���擾
	pos = WF2DMAP_OBJWkMatrixGet( p_mcr->p_player->p_obj );

	// �������擾
	way = WF2DMAP_OBJWkDataGet( p_mcr->p_player->p_obj, WF2DMAP_OBJPM_WAY );

	// �����̃}�b�v�f�[�^�擾
	map_param = WF2DMAP_MAPSysParamGet( p_mcr->p_mapsys, WF2DMAP_POS2GRID(pos.x), WF2DMAP_POS2GRID(pos.y) );

	// �o���̏�ŉ��������Ă�����I��
	if( (map_param == MCR_MAPPM_EXIT) && (way == WF2DMAP_WAY_DOWN) ){
		return TRUE;
	}

	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	NPC�o�^���W���擾
 *
 *	@param	p_mcr		���[�N
 *	@param	p_pos		���W�ݒ��
 *	@param	friendNo	�t�����h�i���o�[
 */
//-----------------------------------------------------------------------------
static void WcrMapGetNpcSetPos( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ_ONPOS* p_pos, u32 friendNo )
{
	u32 block_num;
	u32 block_idx;
	int i, j;
	u32 map_grid_x, map_grid_y;
	u32 param;
	u32 block_no;

	block_num = friendNo / WCR_MAPDATA_1BLOCKOBJNUM;
	block_idx = friendNo % WCR_MAPDATA_1BLOCKOBJNUM;

	map_grid_x = WF2DMAP_MAPSysGridXGet( p_mcr->p_mapsys );
	map_grid_y = WF2DMAP_MAPSysGridYGet( p_mcr->p_mapsys );
	
	// block_num��block_idx�̃}�b�v�ʒu���擾����
	for( i=0; i<map_grid_y; i++ ){
		for( j=0; j<map_grid_x; j++ ){
			param = WF2DMAP_MAPSysParamGet( p_mcr->p_mapsys, j, i );
			if( param == MCR_MAPPM_OBJ00+block_idx ){
				// ���̉��Ƀu���b�NNo�������Ă���̂Ń`�F�b�N
				block_no = WF2DMAP_MAPSysParamGet( p_mcr->p_mapsys, j, i+1 );
				if( block_no == block_num+MCR_MAPPM_MAP00 ){
					// �݂������I
					p_pos->x =  WF2DMAP_GRID2POS( j );
					p_pos->y =  WF2DMAP_GRID2POS( i );
					return ;
				}
			}
		}
	}

	// �Ȃ�������
	GF_ASSERT_MSG( 0, "frinedno=%d\n", friendNo );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�o�^���W���擾
 *	
 *	@param	p_mcr	�V�X�e�����[�N
 *	@param	p_pos	���W�i�[��
 */
//-----------------------------------------------------------------------------
static void WcrMapGetPlayerSetPos( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ_ONPOS* p_pos )
{
	int i, j;
	u32 map_grid_x, map_grid_y;
	u32 param;

	map_grid_x = WF2DMAP_MAPSysGridXGet( p_mcr->p_mapsys );
	map_grid_y = WF2DMAP_MAPSysGridYGet( p_mcr->p_mapsys );
	
	// block_num��block_idx�̃}�b�v�ʒu���擾����
	for( i=0; i<map_grid_y; i++ ){
		for( j=0; j<map_grid_x; j++ ){
			param = WF2DMAP_MAPSysParamGet( p_mcr->p_mapsys, j, i );
#ifdef WFP2P_DEBUG_PLON_PC
			if( param == MCR_MAPPM_PC ){
				// �݂������I
				p_pos->x =  WF2DMAP_GRID2POS( j );
				p_pos->y =  WF2DMAP_GRID2POS( i+1 );
				return ;
			}
#else
			if( param == MCR_MAPPM_EXIT ){
				// �݂������I
				p_pos->x =  WF2DMAP_GRID2POS( j );
				p_pos->y =  WF2DMAP_GRID2POS( i );
				return ;
			}
#endif
		}
	}

	// �Ȃ�������
	GF_ASSERT(0);
}


//----------------------------------------------------------------------------
/**
 *	@brief	�󂢂Ă���I�u�W�F�N�g���擾
 *
 *	@param	p_mcr	���[�N
 *
 *	@retval	�󂢂Ă���I�u�W�F�N�g
 */
//-----------------------------------------------------------------------------
static MCR_MOVEOBJ* WcrMoveObj_GetClean( WIFI_MATCHROOM* p_mcr )
{
	int i;

	for( i=0; i<MCR_MOVEOBJNUM; i++ ){
		if( p_mcr->moveObj[i].p_obj == NULL ){
			return &p_mcr->moveObj[i];
		}
	}
	GF_ASSERT(0);
	return NULL;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�Z�b�g�A�b�v
 *
 *	@param	p_mcr		���[�N
 *	@param	p_obj		����I�u�W�F�N�g
 *	@param	hero		��l�����ǂ���
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_SetUpGraphic( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, BOOL hero )
{
	// �\���f�[�^�̓o�^
	p_obj->p_draw = WF2DMAP_OBJDrawWkNew( p_mcr->p_objdraw, p_obj->p_obj, hero, p_mcr->use_heap );

	if( hero == FALSE ){
		// �܂��X�V��~
		WF2DMAP_OBJDrawWkUpdataFlagSet( p_obj->p_draw, FALSE );

		// ���邭��A�j���X�^�[�g
		WF2DMAP_OBJDrawWkKuruAnimeStart( p_obj->p_draw );

		// OBJ�\��OFF
		WF2DMAP_OBJDrawWkDrawFlagSet( p_obj->p_draw, FALSE );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	����I�u�W�F�̓��암���̏�����
 *
 *	@param	p_mcr		���[�N
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	cp_pos		�o�^�|�W�V����
 *	@param	friendNo	�F�B�i���o�[
 *	@param	charid		�L�����N�^ID
 *	@param	way			�����������
 *	@param	status		�������
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_SetUpMove( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, const MCR_MOVEOBJ_ONPOS* cp_pos, u32 friendNo, u16 charaid, u8 way, WF2DMAP_OBJST status )
{
	WF2DMAP_OBJDATA add;

	// �I�u�W�F�N�g������
	add.x = cp_pos->x;
	add.y = cp_pos->y;
	add.playid = friendNo;
	add.status = status;
	add.way = way;
	add.charaid = charaid;
	p_obj->p_obj = WF2DMAP_OBJWkNew( p_mcr->p_objsys, &add );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̐l�ɍ��b�������đ��v���`�F�b�N
 *
 *	@param	cp_obj	���[�N
 *
 *	@retval	TRUE	���v
 *	@retval	FALSE	����
 */
//-----------------------------------------------------------------------------
static BOOL WcrMoveObj_CheckAccess( const MCR_MOVEOBJ* cp_obj )
{
	switch( cp_obj->move_st ){
	// OK
	case MCR_MOVEOBJ_ST_PL_KEYWAIT:		// �v���C���[�ʏ퓮��
	case MCR_MOVEOBJ_ST_NPC:				// NPC����
	case MCR_MOVEOBJ_ST_NPC_JUMP:		// NPC��W��
		return TRUE;
		
	// NG
	case MCR_MOVEOBJ_ST_KURUKURU:		// �����鏈��
	case MCR_MOVEOBJ_ST_KURUKURU_DEL:	// ��鏈��
	case MCR_MOVEOBJ_ST_NPC_PAUSE:		// NPC�����~
		return FALSE;
		
	default:
		break;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[����J�n
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_SetMoveFuncPlayer( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	p_obj->pMove = WcrMoveObj_MoveFuncPlayerKeyWait;
	p_obj->pDraw = WcrMoveObj_DrawFuncDefault;

	p_obj->move_st	= MCR_MOVEOBJ_ST_PL_KEYWAIT;

	// ��Ԃ�ʏ�ɖ߂�
	WcrMoveObj_ReqCmdSetEasy( p_mcr, WF2DMAP_OBJST_NONE, p_obj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���邭�鏉�����ݒ�
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_SetMoveFuncKuruKuruInit( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	p_obj->pMove = WcrMoveObj_MoveFuncKuruKuru;
	p_obj->pDraw = WcrMoveObj_DrawFuncKuruKuru;
	p_obj->move_count = MCR_MOVEOBJ_KURU_COUNT;

	p_obj->move_st	= MCR_MOVEOBJ_ST_KURUKURU;
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC����ݒ�
 *
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_SetMoveFuncNpc( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	p_obj->pMove = WcrMoveObj_MoveFuncNpc;
	p_obj->pDraw = WcrMoveObj_DrawFuncDefault;
	p_obj->move_count = MCR_MOVEOBJ_NPC_RAND_S + (gf_mtRand() % MCR_MOVEOBJ_NPC_RAND_M);

	p_obj->move_st	= MCR_MOVEOBJ_ST_NPC;

	// ��Ԃ�ʏ�ɖ߂�
	WcrMoveObj_ReqCmdSetEasy( p_mcr, WF2DMAP_OBJST_NONE, p_obj );

	// �`��X�V�L��
	WF2DMAP_OBJDrawWkUpdataFlagSet( p_obj->p_draw, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC�W�����v����ݒ�
 *
 *	@param	p_mcr	�V�X�e�����[�N
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_SetMoveFuncNpcJump( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	p_obj->pMove = WcrMoveObj_MoveFuncNpcJump;
	p_obj->pDraw = WcrMoveObj_DrawFuncNpcJump;
	p_obj->move_count = MCR_MOVEOBJ_NPC_JUMP_COUNT;

	p_obj->move_st	= MCR_MOVEOBJ_ST_NPC_JUMP;


	// ��Ԃ�ʏ�ɖ߂�
	WcrMoveObj_ReqCmdSetEasy( p_mcr, WF2DMAP_OBJST_BUSY, p_obj );

	// �`��X�V����
	WF2DMAP_OBJDrawWkUpdataFlagSet( p_obj->p_draw, FALSE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̋t���擾
 *		
 *	@param	way		����
 */
//-----------------------------------------------------------------------------
static u32 WcrMoveObj_GetRetWay( u32 way )
{
	static const u32 RetWay[] = {
		MCR_MOVEOBJ_WAY_BOTTOM,
		MCR_MOVEOBJ_WAY_TOP,
		MCR_MOVEOBJ_WAY_RIGHT,
		MCR_MOVEOBJ_WAY_LEFT,
	};
	return RetWay[ way ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��֐��f�t�H���g
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_DrawFuncDefault( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	���邭��
 *
 *	@param	p_obj ���[�N
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_DrawFuncKuruKuru( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	WF2DMAP_POS mat, shadow_mat;

	WF2DMAP_OBJDrawWkKuruAnimeMain( p_obj->p_draw );

	// ��\���Ȃ�\���ɂ���
	if( WF2DMAP_OBJDrawWkDrawFlagGet( p_obj->p_draw ) == FALSE ){
		WF2DMAP_OBJDrawWkDrawFlagSet( p_obj->p_draw, TRUE );
	}

	// �ʒu���ォ�痎�Ƃ��Ă���
	mat.x = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_X );
	mat.y = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_Y );
	shadow_mat = mat;
	mat.y += FX_Mul( _Sin360( p_obj->move_count ), MCR_MOVEOBJ_KURU_DIST*FX32_ONE ) >> FX32_SHIFT;
	WF2DMAP_OBJDrawWkMatrixSet( p_obj->p_draw, mat );

	// �A�͒n�ʂ̈ʒu�ɏo�����ςȂ�
	WF2DMAP_OBJDrawWkShadowMatrixSet( p_obj->p_draw, shadow_mat );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�W�����v�`��
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_DrawFuncNpcJump( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	u32 r;
	WF2DMAP_POS mat, shadow_mat;
	s16 count;

	mat.x = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_X );
	mat.y = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_Y );
	shadow_mat = mat;

	// �J�E���g�l�擾
	count = p_obj->move_count - MCR_MOVEOBJ_NPC_JUMP_WAITCOUNT;

	// count > 0�@���ƃW�����v��	����ȊO�͎��̃W�����v�܂ł̃E�G�C�g
	if( count > 0 ){

		count = count % MCR_MOVEOBJ_NPC_JUMP_EFFCOUNT;

		// SIN�J�[�u�̊p�x���擾
		r = (MCR_MOVEOBJ_NPC_JUMP_RMAX*count) / MCR_MOVEOBJ_NPC_JUMP_EFFCOUNT;

		// �W�����v
		mat.y -= FX_Mul( Sin360(r), MCR_MOVEOBJ_NPC_JUMP_DIS*FX32_ONE ) >> FX32_SHIFT;
	}

	WF2DMAP_OBJDrawWkMatrixSet( p_obj->p_draw, mat );

	// �A�͒n�ʂ̈ʒu�ɏo�����ςȂ�
	WF2DMAP_OBJDrawWkShadowMatrixSet( p_obj->p_draw, shadow_mat );

	// ������������
	WF2DMAP_OBJDrawWkWaySet( p_obj->p_draw, WF2DMAP_WAY_DOWN );
}


//----------------------------------------------------------------------------
/**
 *	@brief	����֐�
 *
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�ʏ�
 */
//-----------------------------------------------------------------------------
// ��l���p
static BOOL WcrMoveObj_MoveFuncPlayerKeyWait( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	s32 way;
	s32 playid;
	s32 status;
	s32 cmd;
	const MCR_MOVEOBJ* cp_wk;

	way = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_WAY );
	playid = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_PLID );

	// �ҋ@��ԂłȂ���΂Ȃ�Ȃ�
	status = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_ST );
	if( status != WF2DMAP_OBJST_NONE ){
		return FALSE;
	}


	// �L�����N�^�����ւ���f�o�b�N�@�\
#ifdef MCR_DEBUG_2CCHAR_CHECK
	if( sys.trg & PAD_BUTTON_START ){
		static const u16 sc_DEBUG_VIEW_TBL[] = {
			HERO,
			BOY1,
			BOY3,
			MAN3,
			BADMAN,
			EXPLORE,
			FIGHTER,
			GORGGEOUSM,
			MYSTERY, 
			HEROINE,
			GIRL1, 
			GIRL2, 
			WOMAN2,
			WOMAN3,
			IDOL,  
			LADY,  
			COWGIRL,
			GORGGEOUSW,
			BOY2,			
			GIRL3,			
			CAMPBOY,		
			PICNICGIRL,	
			BABYGIRL1,		
			MIDDLEMAN1,	
			MIDDLEWOMAN1,	
			WAITER,		
			WAITRESS,		
			MAN1,			
			WOMAN1,		
			CAMERAMAN,		
			REPORTER,		
			FARMER,		
			CYCLEM,		
			CYCLEW,		
			OLDMAN1,		
			MAN5,			
			WOMAN5,		
			BABYBOY1,		
			SPORTSMAN,		
			FISHING,		
			SEAMAN,		
			MOUNT,			
			MAN2,			
			BIGMAN,		
			ASSISTANTM,	
			GENTLEMAN,		
			WORKMAN,		
			CLOWN,			
			POLICEMAN,		
			AMBRELLA,		
			PIKACHU,		
			SEVEN1,		
			SEVEN2,		
			SEVEN3,		
			SEVEN4,		
			SEVEN5,		
			TOWERBOSS,		
			ACHAMO,		
			ARTIST,		
			ASSISTANTW,	
			BABY,			
			BAG,			
			BALLOON,		
			BEACHBOY,		
			BEACHGIRL,		
			BIGFOUR1,		
			BIGFOUR2,		
			BIGFOUR3,		
			BIGFOUR4,		
			BIRD,			
			BOY4,			
			CHAMPION,		
			DOCTOR,		
			DSBOY,			
			ENECO,			
			FREEZES,		
			GINGABOSS,		
			GINGAM,		
			GINGAW,		
			GIRL4,			
			GKANBU1,		
			GKANBU2,		
			GKANBU3,		
			KINOCOCO,		
			KODUCK,		
			KOIKING,		
			LEADER1,		
			LEADER2,		
			LEADER3,		
			LEADER4,		
			LEADER5,		
			LEADER6,		
			LEADER7,		
			LEADER8,		
			MAID,			
			MAMA,			
			MIDDLEMAN2,	
			MIDDLEWOMAN2,	
			MIKAN,			
			MIMITUTO,		
			MINUN,			
			MOSS,			
			OLDMAN2,		
			OLDWOMAN1,		
			OLDWOMAN2,		
			OOKIDO,		
			PATIRITUSU,	
			PCWOMAN1,		
			BRAINS2,		
			BRAINS3,		
			BRAINS4,		
			PRINCESS,		
			BFSM,			
			BFSW1,			
			BFSW2,			
			WIFISM,		
			WIFISW,		
			DPHERO,		
			DPHEROINE,		
		};
		
		WF2DMAP_OBJDrawWkDel( p_obj->p_draw );
		WF2DMAP_OBJDrawSysResDel( p_mcr->p_objdraw, s_MCR_DEBUG_2CCHAR_VIEW_ID );

		s_MCR_DEBUG_2CCHAR_VIEW_ID = sc_DEBUG_VIEW_TBL[s_MCR_DEBUG_2CCHAR_VIEW_COUNT];
		s_MCR_DEBUG_2CCHAR_VIEW_COUNT = (s_MCR_DEBUG_2CCHAR_VIEW_COUNT + 1) % NELEMS(sc_DEBUG_VIEW_TBL);

		WF2DMAP_OBJDrawSysResSet( p_mcr->p_objdraw, s_MCR_DEBUG_2CCHAR_VIEW_ID, WF_2DC_MOVENORMAL, p_mcr->use_heap );

		WF2DMAP_OBJWkDataSet( p_obj->p_obj, WF2DMAP_OBJPM_CHARA, s_MCR_DEBUG_2CCHAR_VIEW_ID );
		p_obj->p_draw = WF2DMAP_OBJDrawWkNew( p_mcr->p_objdraw, p_obj->p_obj, TRUE, p_mcr->use_heap );
	}
#endif

	// �I���֌W
	if( sys.trg & PAD_BUTTON_DECIDE ){

		// �ڂ̑O�ɐl�����邩�`�F�b�N
		cp_wk = WcrMoveObjGetHitCheck( p_mcr, p_obj, way );
		
		if( cp_wk != NULL ){
			// ���̐l�͖Z�����Ȃ����`�F�b�N
			if( WcrMoveObj_CheckAccess( cp_wk ) == TRUE ){
				// ����friendNO��ݒ�
				p_obj->accesFriend = WF2DMAP_OBJWkDataGet( cp_wk->p_obj, WF2DMAP_OBJPM_PLID );
			}else{
				// �ʐM�o�O602�Ώ�	tomoya
				p_obj->accesFriend = 0;
			}
		}else{
			p_obj->accesFriend = 0;
		}
	}

	// �ړ���~��Ԃ��`�F�b�N
	if( p_mcr->player_pause == TRUE ){	
		return FALSE;
	}

	// ���������邩
	if( sys.cont & PAD_BUTTON_B ){
		cmd = WF2DMAP_OBJST_RUN;
	}else{
		cmd = WF2DMAP_OBJST_WALK;
	}
	
	// �ړ��֌W
	if( sys.cont & PAD_KEY_UP ){
		if( way == MCR_MOVEOBJ_WAY_TOP ){
			WcrMoveObj_ReqCmdSet( p_mcr, cmd, way, playid );
		}else{
			WcrMoveObj_ReqCmdSet( p_mcr, WF2DMAP_OBJST_TURN, MCR_MOVEOBJ_WAY_TOP, playid );
		}
	}else if( sys.cont & PAD_KEY_DOWN ){
		if( way == MCR_MOVEOBJ_WAY_BOTTOM ){
			WcrMoveObj_ReqCmdSet( p_mcr, cmd, way, playid );
		}else{
			WcrMoveObj_ReqCmdSet( p_mcr, WF2DMAP_OBJST_TURN, MCR_MOVEOBJ_WAY_BOTTOM, playid );
		}
	}else if( sys.cont & PAD_KEY_LEFT ){
		if( way == MCR_MOVEOBJ_WAY_LEFT ){
			WcrMoveObj_ReqCmdSet( p_mcr, cmd, way, playid );
		}else{
			WcrMoveObj_ReqCmdSet( p_mcr, WF2DMAP_OBJST_TURN, MCR_MOVEOBJ_WAY_LEFT, playid );
		}
	}else if( sys.cont & PAD_KEY_RIGHT ){
		if( way == MCR_MOVEOBJ_WAY_RIGHT ){
			WcrMoveObj_ReqCmdSet( p_mcr, cmd, way, playid );
		}else{
			WcrMoveObj_ReqCmdSet( p_mcr, WF2DMAP_OBJST_TURN, MCR_MOVEOBJ_WAY_RIGHT, playid );
		}
	}
	
	return FALSE;
}

// ���邭��
static BOOL WcrMoveObj_MoveFuncKuruKuru( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	s32 way;
	u32 playid, myplayid;
	
	p_obj->move_count --;
	if( p_obj->move_count <= 0 ){

		// ���邭��A�j���I��
		WF2DMAP_OBJDrawWkKuruAnimeEnd( p_obj->p_draw );

		// �A�b�v�f�[�g�J�n
		WF2DMAP_OBJDrawWkUpdataFlagSet( p_obj->p_draw, TRUE );

		playid = WF2DMAP_OBJWkDataGet( p_mcr->p_player->p_obj, WF2DMAP_OBJPM_PLID );
		myplayid = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_PLID );
		
		// ����������
		if( myplayid == playid ){

			// �v���C���[
			WcrMoveObj_SetMoveFuncPlayer( p_mcr, p_obj );
		}else{
			// NPC
			WcrMoveObj_MoveSet( p_mcr, p_obj, p_obj->moveID );
		}
	}
	return FALSE;
}

static BOOL WcrMoveObj_MoveFuncKuruKuruDel( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	p_obj->move_count ++;
	if( p_obj->move_count > MCR_MOVEOBJ_KURU_COUNT ){
		return TRUE;
	}
	return FALSE;
}

// NPC����
static BOOL WcrMoveObj_MoveFuncNpc( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	u32 way;
	u32 playid;
	
	p_obj->move_count --;
	if( p_obj->move_count <= 0 ){

		p_obj->move_count = MCR_MOVEOBJ_NPC_RAND_S + (gf_mtRand() % MCR_MOVEOBJ_NPC_RAND_M);
	
		// �����_���ŕ�����ς���
		way = gf_mtRand() % MCR_MOVEOBJ_WAY_NUM;
			
		// ���N�G�X�g�R�}���h�ݒ�
		// ���̕����ɕύX
		playid = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_PLID );
		WcrMoveObj_ReqCmdSet( p_mcr, WF2DMAP_CMD_NONE, way, playid );
	}
	return FALSE;
}

// NPC����@�W�����v
static BOOL WcrMoveObj_MoveFuncNpcJump( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	p_obj->move_count --;
	if( p_obj->move_count < 0 ){
		p_obj->move_count = MCR_MOVEOBJ_NPC_JUMP_COUNT;
	}
	return FALSE;
}

// NPC�����~
static BOOL WcrMoveObj_MoveFuncNpcPause( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj )
{
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���N�G�X�g�R�}���h��ݒ肷��
 *
 *	@param	p_mcr		�V�X�e�����[�N
 *	@param	cmd			�R�}���h
 *	@param	way			����
 *	@param	playid		�v���C���[ID
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_ReqCmdSet( WIFI_MATCHROOM* p_mcr, s32 cmd, s32 way, s32 playid )
{
	WF2DMAP_REQCMD reqcmd;
	
	reqcmd.cmd = cmd;
	reqcmd.way = way;
	reqcmd.playid = playid;
	WF2DMAP_REQCMDQSysCmdPush( p_mcr->p_reqcmdQ, &reqcmd );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ȈՃR�}���h�ݒ�
 *
 *	@param	p_mcr	�V�X�e�����[�N
 *	@param	cmd		�ݒ�R�}���h
 *	@param	cp_obj	�ݒ�I�u�W�F�N�g
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_ReqCmdSetEasy( WIFI_MATCHROOM* p_mcr, s32 cmd, const MCR_MOVEOBJ* cp_obj )
{
	s32 way;
	s32 playid;

	way = WF2DMAP_OBJWkDataGet( cp_obj->p_obj, WF2DMAP_OBJPM_WAY );
	playid = WF2DMAP_OBJWkDataGet( cp_obj->p_obj, WF2DMAP_OBJPM_PLID );

	WcrMoveObj_ReqCmdSet( p_mcr, cmd, way, playid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC�̓���^�C�v��ύX���Ă����v���`�F�b�N
 *
 *	@param	cp_obj	�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	����ݒ�@OK
 *	@retval	FALSE	����ύX�@NG
 */
//-----------------------------------------------------------------------------
static BOOL WcrMoveObj_MoveSetOkCheck( const MCR_MOVEOBJ* cp_obj )
{
	// ���邭��`�撆�͂���
	if( (u32)cp_obj->pDraw == (u32)WcrMoveObj_DrawFuncKuruKuru ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	����ݒ�
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	moveID		����^�C�v
 */
//-----------------------------------------------------------------------------
static void WcrMoveObj_MoveSet( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, MCR_NPC_MOVETYPE moveID )
{	
	// �`��֐���ύX
	switch( moveID ){
	case MCR_NPC_MOVE_NORMAL:
		WcrMoveObj_SetMoveFuncNpc( p_mcr, p_obj );
		break;
		
	case MCR_NPC_MOVE_JUMP:
		WcrMoveObj_SetMoveFuncNpcJump( p_mcr, p_obj );
		break;

	default:
		// ����Ȃ���Ȃ�
		GF_ASSERT( 0 );
		break;
	}
	p_obj->moveID = moveID;
}

//----------------------------------------------------------------------------
/**
 *	@brief	PC�A�j���V�X�e���@������
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_wk		���[�N
 *	@param	p_handle	�A�[�J�C�u�n���h��
 */
//-----------------------------------------------------------------------------
static void WcrPCANM_Init( WIFI_MATCHROOM* p_sys, MCR_PCANM* p_wk, ARCHANDLE* p_handle )
{
	// ���[�N�̃N���A
	memset( p_wk, 0, sizeof(MCR_PCANM) );
	
	// ���\�[�X�Ǎ���
	p_wk->p_plbuff = ArcUtil_HDL_PalDataGet( p_handle, 
			NARC_wifip2pmatch_wf_match_top_room_pc_NCLR, &p_wk->p_pltt, p_sys->use_heap );

	// �SPC�A�j���J�n
	p_wk->all_pcbitmap = (1<<0) | (1<<1) | (1<<2) | (1<<3);
	p_wk->all_seq = MCR_PCANM_ALL_SEQ_OFF;
	p_wk->all_count = AllAnmData[p_wk->all_seq];
}

//----------------------------------------------------------------------------
/**
 *	@brief	PC�A�j���V�X�e���@�j��
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WcrPCANM_Delete( WIFI_MATCHROOM* p_sys, MCR_PCANM* p_wk )
{
	// ���[�N�̔j��
	sys_FreeMemoryEz( p_wk->p_plbuff );
	memset( p_wk, 0, sizeof(MCR_PCANM) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	PC�A�j���@���C��
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WcrPCANM_Main( WIFI_MATCHROOM* p_sys, MCR_PCANM* p_wk )
{
	// �ҋ@�A�j�����C��
	WcrPCANM_AllMain( p_wk );	

	// �g�p���A�j�����C��
	WcrPCANM_UseMain( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ҋ@�A�j�����C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WcrPCANM_AllMain( MCR_PCANM* p_wk )
{
	int i;
	u32 on_off;

	// �ҋ@�A�j��
	p_wk->all_count --;
	if( p_wk->all_count <= 0 ){
		p_wk->all_seq = (p_wk->all_seq + 1) % MCR_PCANM_ALL_SEQ_NUM;
		p_wk->all_count = AllAnmData[p_wk->all_seq];

		// �p���b�g�]��
		if( p_wk->all_seq == MCR_PCANM_ALL_SEQ_ON ){
			on_off = MCR_PCANM_COL_WAIT;
		}else{
			on_off = MCR_PCANM_COL_OFF;
		}
		for( i=0; i<MCR_PCANM_PCNUM; i++ ){
			if( p_wk->all_pcbitmap & (1<<i) ){
				AddVramTransferManager( NNS_GFD_DST_2D_BG_PLTT_MAIN, 
						MCR_PCANM_DESTPL(i), WcrPCANM_GetAnmSrc(p_wk, on_off ), 2 );

//				TOMOYA_PRINT( "�]������ %d dest[0x%x] src[0x%x] \n", on_off, MCR_PCANM_DESTPL(i), WcrPCANM_GetAnmSrc(p_wk, on_off ) );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�p���A�j�����C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WcrPCANM_UseMain( MCR_PCANM* p_wk )
{
	BOOL trans = FALSE;
	u32 on_off = FALSE;
	BOOL next = FALSE;


	// ��{�I�ɂ�AnmData�̃J�E���g���i�ނƎ��̃A�j���f�[�^�ɐi��ł����܂��B
	// �J�E���g�������f�[�^�̎��́Ause_pc_next�t���O���ݒ肳�ꂽ�玟�ɐi�݂܂��B
	// �p���b�g�̓]����AnmData��trans�����o��trans_flag�����o�����Č��߂܂�

	// �J�E���g�V�X�e��
	if( AnmData[ p_wk->use_pc_seq ].flag == TRUE ){
		p_wk->use_pc_count --;
		if( p_wk->use_pc_count <= 0 ){
			next = TRUE;
		}
	}else{
		// ���ɐi�ރt���O�������Ă����玟��
		if( p_wk->use_pc_next ){
			p_wk->use_pc_next = FALSE;
			next = TRUE;
		}
	}

	// ���̃V�[�P���X��
	if( next ){
		p_wk->use_pc_seq ++;

		// �I���`�F�b�N
		if( p_wk->use_pc_seq >= MCR_PCANM_USE_SEQ_NUM ){
			WcrPCANM_UseAnmEnd( p_wk );
			return ;
		}

		// �J�E���g���Ȃǐݒ�
		p_wk->use_pc_count = AnmData[ p_wk->use_pc_seq ].count_num;
		trans = AnmData[ p_wk->use_pc_seq ].trans;
		on_off = AnmData[ p_wk->use_pc_seq ].trans_flag;
	}
	

	// �]������
	if( trans ){
		AddVramTransferManager( NNS_GFD_DST_2D_BG_PLTT_MAIN, 
				MCR_PCANM_DESTPL(p_wk->use_pc), WcrPCANM_GetAnmSrc(p_wk, on_off ), 2 );
//		TOMOYA_PRINT( "�]������ %d dest[0x%x] src[0x%x] \n", on_off, MCR_PCANM_DESTPL(p_wk->use_pc), WcrPCANM_GetAnmSrc(p_wk, on_off ) );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���̃p���b�g�f�[�^���擾����
 *
 *	@param	p_wk		���[�N
 *	@param	on_off		TRUE�FON		FALSE�FOFF
 */
//-----------------------------------------------------------------------------
static void* WcrPCANM_GetAnmSrc( MCR_PCANM* p_wk, u32 on_off )
{
	u16* p_data;

	// �f�[�^�擾
	p_data = (u16*)p_wk->p_pltt->pRawData;

	return &p_data[ on_off+MCR_PCANM_BGPLTTOFS ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�p���A�j���N���[��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WcrPCANM_UseAnmEnd( MCR_PCANM* p_wk )
{
	// �ҋ@�A�j���ɍ���������
	p_wk->all_pcbitmap |= 1<<p_wk->use_pc;
	
	p_wk->use_pc = 0;
	p_wk->use_pc_seq = MCR_PCANM_USE_SEQ_NONE;
	p_wk->use_pc_count = 0;
	p_wk->use_pc_next = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�p���A�j�������ɐi�߂鏈��
 *
 *	@param	p_wk ���[�N
 */
//-----------------------------------------------------------------------------
static void WcrPCANM_UseAnmNext( MCR_PCANM* p_wk )
{
	p_wk->use_pc_next = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	PC�A�j���J�n
 *
 *	@param	p_wk	���[�N
 *	@param	pc_no	�p���b�gNO
 */
//-----------------------------------------------------------------------------
static void WcrPCANM_UseStart( MCR_PCANM* p_wk, u8 pc_no )
{
	// ���r���̂��̂����邩
	if( p_wk->use_pc_seq != MCR_PCANM_USE_SEQ_NONE ){
		// �����s���̂��̂��I��点��
		WcrPCANM_UseAnmEnd( p_wk );
	}

	// �J�n
	p_wk->all_pcbitmap ^= 1<<pc_no;
	p_wk->use_pc = pc_no;
	WcrPCANM_UseAnmNext( p_wk );

//	TOMOYA_PRINT( "use anm start\n" );
}

//----------------------------------------------------------------------------
/**
 *	@brief	PC�A�j���I��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WcrPCANM_UseEndReq( MCR_PCANM* p_wk )
{
	if( p_wk->use_pc_seq != MCR_PCANM_USE_SEQ_NONE ){
		WcrPCANM_UseAnmNext( p_wk );
//		TOMOYA_PRINT( "use anm end req\n" );
	}
}

//==============================================================================
/**
 * @file	br_manager.c
 * @brief	�o�g�����R�[�_�[
 * @author	goto
 * @date	2007.07.26(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#include <dwc.h>

#include "common.h"
#include "system/procsys.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/pm_str.h"
#include "system/clact_tool.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "system/brightness.h"
#include "system/fontproc.h"
#include "system/lib_pack.h"
#include "gflib/msg_print.h"
#include "gflib/touchpanel.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeicon.h"
#include "poketool/boxdata.h"
#include "system/snd_tool.h"
#include "gflib/strbuf_family.h"
#include "system/pm_overlay.h"
#include "system/wipe.h"

#include "src/field/syswork.h"
#include "src/field/sysflag.h"

#include "br_private.h"


//--------------------------------------------------------------
//	�I�[�o�[���CID
//--------------------------------------------------------------
FS_EXTERN_OVERLAY(gds_comm);
FS_EXTERN_OVERLAY( battle_recorder );


typedef struct {
	
	int				seq;
	int				state;			// ���
	PROC*			p_subproc;		// �T�u�v���Z�X
	
	SAVEDATA*		save;
	BATTLE_PARAM*	bp;
	

	BR_WORK*		br_work;		// �o�g�����R�[�_�[�p���[�N
	
	POST_MESSAGE	message;		// �A��
	
	FIELDSYS_WORK*	fsys;
	
} BR_MANAGER_SYS;


enum {
	
	eBRM_BATTLE_RECORDER	= 0,		///< �o�g�����R�[�_�[
	eBRM_BATTLE_REC_PLAYER,				///< �^��Đ�
};

//==============================================================
// Prototype
//==============================================================
static void ManagerInit_Common( PROC * proc, int mode );

static PROC_RESULT BR_Manager_Proc_Init( PROC * proc, int * seq );
static PROC_RESULT BR_Manager_Proc_Init_GDS( PROC * proc, int * seq );
static PROC_RESULT BR_Manager_Proc_Init_GDS_BV( PROC * proc, int * seq );
static PROC_RESULT BR_Manager_Proc_Init_GDS_BV_RANK( PROC * proc, int * seq );
static PROC_RESULT BR_Manager_Proc_Init_GDS_RANK( PROC * proc, int * seq );
static PROC_RESULT BR_Manager_Proc_Init_GDS_DRESS( PROC * proc, int * seq );
static PROC_RESULT BR_Manager_Proc_Init_GDS_BOX( PROC * proc, int * seq );

static PROC_RESULT BR_Manager_Proc_Main( PROC * proc, int * seq );
static PROC_RESULT BR_Manager_Proc_Exit( PROC * proc, int * seq );
static BOOL BRM_BattleRecorder( BR_MANAGER_SYS* wk, int heap );
static BOOL BRM_BattleRecPlayer( BR_MANAGER_SYS* wk, int heap );
static BOOL BR_ExBattleCheck( int type );


//--------------------------------------------------------------
/**
 * @brief	�q�v���b�N����擾�����̂ŁA�Ԉ���Ă�manager_proc��n���Ȃ���
 *
 * @param	proc	
 *
 * @retval	BR_WORK*	
 *
 */
//--------------------------------------------------------------
BR_WORK* BR_PROC_GetWork( PROC * proc )
{
	BR_WORK* ret_wk;
	
	BR_MANAGER_SYS* mana_wk = PROC_GetParentWork( proc );
	
	return mana_wk->br_work;
}


//--------------------------------------------------------------
/**
 * @brief	�e�ւ̘A��
 *
 * @param	proc	
 * @param	bFlag	
 * @param	state	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_PostMessage( POST_MESSAGE* message, BOOL bFlag, int state )
{
	message->bFlag = bFlag;
	message->state = state;	
}

//--------------------------------------------------------------
/**
 * @brief	�m�[�g�̐ݒ�
 *
 * @param	save	
 * @param	heap	
 * @param	id	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void GDS_NoteDataSet( SAVEDATA* save, int heap, u32 id )
{
	FNOTE_DATA *f_note = SaveData_GetFNote( save );
	void *buf = FNOTE_SioIDOnlyDataMake( heap, id );	
	FNOTE_DataSave( f_note, buf, FNOTE_TYPE_SIO );	
}

//--------------------------------------------------------------
/**
 * @brief	���������ʕ���
 *
 * @param	proc	
 * @param	mode	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void ManagerInit_Common( PROC * proc, int mode )
{
	BR_MANAGER_SYS* wk;
	
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_BR_MANAGER, 0x10000 );
	
	wk = PROC_AllocWork( proc, sizeof( BR_MANAGER_SYS ), HEAPID_BR_MANAGER );
	MI_CpuFill8( wk, 0, sizeof( BR_MANAGER_SYS ) );	
	
	wk->fsys		= PROC_GetParentWork( proc );
	wk->save		= wk->fsys->savedata;
	wk->br_work		= sys_AllocMemory( HEAPID_BR_MANAGER, sizeof( BR_WORK ) );
	MI_CpuFill8( wk->br_work, 0, sizeof( BR_WORK ) );
	
	wk->br_work->message	= &wk->message;
	wk->br_work->save		= wk->save;
//	wk->br_work->fsys		= wk->fsys;
	
	wk->br_work->mode		= mode;
	wk->br_work->menu_data_mem[ wk->br_work->tag_man.stack_s ] = BattleRecorder_MenuDataGetEx( wk->br_work, wk->br_work->mode );
	
	{
		int id;
		switch( mode ){
		case BR_MODE_GDS_BV:
			id = FNOTE_ID_PL_GDS_VIDEO;
			break;
		case BR_MODE_GDS_BV_RANK:
			id = FNOTE_ID_PL_GDS_RANKING;
			break;
		case BR_MODE_GDS_RANK:
			id = FNOTE_ID_PL_GDS_RANKING;
			break;
		case BR_MODE_GDS_DRESS:
			id = FNOTE_ID_PL_GDS_DRESS;
			break;
		case BR_MODE_GDS_BOX:
			id = FNOTE_ID_PL_GDS_BOX;
			break;
		default:
			return;
		}		
		GDS_NoteDataSet( wk->save, HEAPID_BR_MANAGER, id );
	}
}

//--------------------------------------------------------------
/**
 * @brief	�v���b�N������
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT BR_Manager_Proc_Init( PROC * proc, int * seq )
{
	ManagerInit_Common( proc, BR_MODE_BROWSE );
	
	return PROC_RES_FINISH;
}

static PROC_RESULT BR_Manager_Proc_Init_GDS( PROC * proc, int * seq )
{
	ManagerInit_Common( proc, BR_MODE_GDS );
		
	return PROC_RES_FINISH;
}

static PROC_RESULT BR_Manager_Proc_Init_GDS_BV( PROC * proc, int * seq )
{
	ManagerInit_Common( proc, BR_MODE_GDS_BV );
		
	return PROC_RES_FINISH;
}

static PROC_RESULT BR_Manager_Proc_Init_GDS_BV_RANK( PROC * proc, int * seq )
{
	ManagerInit_Common( proc, BR_MODE_GDS_BV_RANK );
		
	return PROC_RES_FINISH;
}

static PROC_RESULT BR_Manager_Proc_Init_GDS_RANK( PROC * proc, int * seq )
{
	ManagerInit_Common( proc, BR_MODE_GDS_RANK );
		
	return PROC_RES_FINISH;
}

static PROC_RESULT BR_Manager_Proc_Init_GDS_DRESS( PROC * proc, int * seq )
{
	ManagerInit_Common( proc, BR_MODE_GDS_DRESS );
		
	return PROC_RES_FINISH;
}

static PROC_RESULT BR_Manager_Proc_Init_GDS_BOX( PROC * proc, int * seq )
{
	ManagerInit_Common( proc, BR_MODE_GDS_BOX );
		
	return PROC_RES_FINISH;
}


//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT BR_Manager_Proc_Main( PROC * proc, int * seq )
{
	BOOL bEnd;
	BR_MANAGER_SYS* wk = PROC_GetWork( proc );
	
	switch ( *seq ){
	case eBRM_BATTLE_RECORDER:
	
		bEnd = BRM_BattleRecorder( wk, HEAPID_BR_MANAGER );
	
		if ( bEnd ){			
			if ( wk->message.bFlag == TRUE ){
				*seq = eBRM_BATTLE_REC_PLAYER;
				wk->seq = 0;
			}
			else {
				return PROC_RES_FINISH;
			}
		}
		break;
		
	case eBRM_BATTLE_REC_PLAYER:
	
		bEnd = BRM_BattleRecPlayer( wk, HEAPID_BR_MANAGER );
		
		if ( bEnd ){
			*seq = eBRM_BATTLE_RECORDER;
			wk->seq = 0;
		}	
		break;
	}
	
	return PROC_RES_CONTINUE;
}


//--------------------------------------------------------------
/**
 * @brief	�I��
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT BR_Manager_Proc_Exit( PROC * proc, int * seq )
{
	BR_MANAGER_SYS* wk = PROC_GetWork( proc );

	///< �^��f�[�^�̉�� ���ɖ��Ȃ���΁A�m�ۂ����ςȂ��Ő퓬���[�`�����܂킵���肷�邽��
	if ( BattleRec_DataExistCheck() == TRUE ){
		BattleRec_Exit();
		OS_Printf( " --- �o�g�����R�[�_�[�I���܂� brs ���J���Ă���\n" );
	}
	
	sys_FreeMemoryEz( wk->br_work );
//	sys_FreeMemoryEz( wk );
	PROC_FreeWork( proc );
	
	
	Snd_PlayerSetPlayerVolume( PLAYER_FIELD, BGM_VOL_MAX );
	
	sys_DeleteHeap( HEAPID_BR_MANAGER );
	
	return PROC_RES_FINISH;
}


//--------------------------------------------------------------
/**
 * @brief	�o�g�����R�[�_�[�N��
 *
 * @param	wk	
 * @param	heap	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BRM_BattleRecorder( BR_MANAGER_SYS* wk, int heap )
{
	switch ( wk->seq ){
	case 0:
		if ( wk->br_work->mode == BR_MODE_BROWSE ){
			wk->p_subproc = PROC_Create( &BattleRecorder_BrowseProcData, wk, heap );
		}
		else {
			wk->p_subproc = PROC_Create( &BattleRecorder_GDSProcData, wk, heap );
		}
		wk->seq++;
		break;
	default:
		if( ProcMain( wk->p_subproc ) == FALSE ){
			break;
		}
		PROC_Delete( wk->p_subproc );
		
		return TRUE;
	}
	
	return FALSE;
}


//--------------------------------------------------------------
/**
 * @brief	�o�g���^��v���C
 *
 * @param	wk	
 * @param	heap	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BRM_BattleRecPlayer( BR_MANAGER_SYS* wk, int heap )
{
//	if ( wk->br_work->mode != BR_MODE_BROWSE ){	DWC_ProcessInet(); }	///< wi-fi�ڑ��ێ��̂���(DWC_ProcessInet();)
	
	switch ( wk->seq ){
	case 0:
		if ( wk->br_work->mode != BR_MODE_BROWSE ){
			//�퓬��ʂɓ˓�����ׁA�I�[�o�[���C������Ă���gds_comm�͈ꎞ�������
			Overlay_UnloadID(FS_OVERLAY_ID(gds_comm));	//2008.02.23(�y) matsuda
			DpwCommonOverlayEnd();
		}
		wk->seq++;
		break;
	
	case 1:
		{
			LOAD_RESULT ret;
			//�ΐ�^��f�[�^�̃��[�h
			
			wk->bp = BattleParam_Create( heap, FIGHT_TYPE_1vs1 );
			
			if ( BattleRec_DataExistCheck() == FALSE ){
				BattleRec_Load( wk->save, heap, &ret, wk->bp, wk->br_work->ex_param1 );
			}
			else {
				BattleRec_BattleParamCreate( wk->bp, wk->save );
				ret = LOAD_RESULT_OK;
			}

			wk->bp->bag_cursor	= MyItem_BagCursorAlloc( heap );
			wk->bp->record		= SaveData_GetRecord( wk->save );
			
			//�s�������`�F�b�N
			if(Overlay_Load(FS_OVERLAY_ID(battle_recorder), OVERLAY_LOAD_NOT_SYNCHRONIZE) == TRUE){
				BattleRecTool_ErrorStrCheck(BattleRec_WorkAdrsGet(), wk->bp, heap);
				Overlay_UnloadID(FS_OVERLAY_ID(battle_recorder));
			}
			
			wk->br_work->ex_param3 = TRUE;
			OS_Printf( "ex_param3 ( rec_stop_flag ) = %d\n", wk->br_work->ex_param3 );
			if ( ret != LOAD_RESULT_OK ){
				sys_FreeMemoryEz( wk->bp->bag_cursor );
				BattleParam_Delete( wk->bp );		//BATTLE_PARAM�̊J��
				wk->seq = 0;
				return TRUE;
			}
			else {
				wk->seq++;
			}
		}
		break;
		
	case 2:
		///< �g���[�i�[�h�c�ŋȂ𕪂���
		{			
			Snd_PlayerSetPlayerVolume( PLAYER_FIELD, BGM_VOL_MAX );
		
			Snd_BattleRecFlag( 1 );			//�^��Đ��p�̃T�E���h�t���Oon

			if ( BR_ExBattleCheck( wk->bp->trainer_data[ 1 ].tr_type ) == TRUE ){
				Snd_DataSetByScene( SND_SCENE_BATTLE, SEQ_PL_BA_BRAIN, 1 );	//�o�g���ȍĐ�
			}
			else {
				Snd_DataSetByScene( SND_SCENE_BATTLE, SEQ_BA_TRAIN, 1 );	//�o�g���ȍĐ�
			}
		}		
		wk->p_subproc = PROC_Create( &TestBattleProcData, wk->bp, heap );
		wk->seq++;
		break;
		
	default:
		if( ProcMain( wk->p_subproc ) == FALSE ){
			break;
		}
		wk->br_work->ex_param3 = wk->bp->battle_rec_stop_flag;
		
		if(wk->br_work->mode != BR_MODE_BROWSE){
			///< �I�����C���Đ��̏ꍇ
			if ( wk->br_work->ex_param3 == FALSE ){
				*wk->br_work->secure_addrs = TRUE;
			}
		}
		OS_Printf( "ex_param3 ( rec_stop_flag ) = %d\n", wk->br_work->ex_param3 );
		sys_FreeMemoryEz( wk->bp->bag_cursor );
		BattleParam_Delete( wk->bp );		//BATTLE_PARAM�̊J��
//		BattleRec_Exit();					//������Ȃ��B�ǂݍ��񂾂��̂����̂܂܎g�������̂ŁB
		PROC_Delete( wk->p_subproc );
		{
			u16 bgm_no;

			Snd_BattleRecFlag( 0 );			//�^��Đ��p�̃T�E���h�t���Ooff

			//�T�E���h�f�[�^�Z�b�g(�V�[�����ύX����Ȃ����͉������Ȃ�)
			Snd_SceneSet( SND_SCENE_DUMMY );
			bgm_no = Snd_FieldBgmNoGet( wk->fsys, wk->fsys->location->zone_id );
			Snd_ZoneBgmSet( Snd_FieldBgmNoGetNonBasicBank( wk->fsys, wk->fsys->location->zone_id ) );//zone set

			Snd_DataSetByScene( SND_SCENE_FIELD, bgm_no, 1 );
		}
		wk->seq = 0;
		
		if(wk->br_work->mode != BR_MODE_BROWSE){
			//gds_comm���A
			DpwCommonOverlayStart();
			Overlay_Load(FS_OVERLAY_ID(gds_comm), OVERLAY_LOAD_NOT_SYNCHRONIZE);
		}
		return TRUE;
	}
	
	return FALSE;
}


// -----------------------------------------
//
//	�� �v���b�N
//
// -----------------------------------------
const PROC_DATA BattleRecorder_ManagerProcData = {
	BR_Manager_Proc_Init,
	BR_Manager_Proc_Main,
	BR_Manager_Proc_Exit,
	NO_OVERLAY_ID,	
};

const PROC_DATA BattleRecorder_ManagerProcDataGDS = {
	BR_Manager_Proc_Init_GDS,
	BR_Manager_Proc_Main,
	BR_Manager_Proc_Exit,	
	NO_OVERLAY_ID,	
};

const PROC_DATA BattleRecorder_ManagerProcDataGDS_BV = {
	BR_Manager_Proc_Init_GDS_BV,
	BR_Manager_Proc_Main,
	BR_Manager_Proc_Exit,	
	NO_OVERLAY_ID,	
};

const PROC_DATA BattleRecorder_ManagerProcDataGDS_BV_RANK = {
	BR_Manager_Proc_Init_GDS_BV_RANK,
	BR_Manager_Proc_Main,
	BR_Manager_Proc_Exit,	
	NO_OVERLAY_ID,	
};

const PROC_DATA BattleRecorder_ManagerProcDataGDS_RANK = {
	BR_Manager_Proc_Init_GDS_RANK,
	BR_Manager_Proc_Main,
	BR_Manager_Proc_Exit,	
	NO_OVERLAY_ID,	
};

const PROC_DATA BattleRecorder_ManagerProcDataGDS_DRESS = {
	BR_Manager_Proc_Init_GDS_DRESS,
	BR_Manager_Proc_Main,
	BR_Manager_Proc_Exit,	
	NO_OVERLAY_ID,	
};

const PROC_DATA BattleRecorder_ManagerProcDataGDS_BOX = {
	BR_Manager_Proc_Init_GDS_BOX,
	BR_Manager_Proc_Main,
	BR_Manager_Proc_Exit,	
	NO_OVERLAY_ID,	
};


///< �v���b�N�e�[�u��
static const PROC_DATA* BR_ProcData_Table[] = {	
	&BattleRecorder_ManagerProcData,
	&BattleRecorder_ManagerProcDataGDS,
	&BattleRecorder_ManagerProcDataGDS_BV,
	&BattleRecorder_ManagerProcDataGDS_BV_RANK,
	&BattleRecorder_ManagerProcDataGDS_RANK,
	&BattleRecorder_ManagerProcDataGDS_DRESS,
	&BattleRecorder_ManagerProcDataGDS_BOX,
};

//--------------------------------------------------------------
/**
 * @brief	���[�h���̃v���b�N�f�[�^�̎擾
 *
 * @param	mode	
 *
 * @retval	const PROC_DATA*	
 *
 */
//--------------------------------------------------------------
const PROC_DATA* BattleRecoder_ProcDataGet( int mode )
{
	const PROC_DATA* pData;
	
	pData = BR_ProcData_Table[ mode ];
	
	return pData;
}


//--------------------------------------------------------------
/**
 * @brief	�Q�[���N���A�������H
 *
 * @param	wk	
 *
 * @retval	BOOL	TRUE = �N���A
 *
 */
//--------------------------------------------------------------
BOOL BR_IsGameClear( BR_WORK* wk )
{
	EVENTWORK* ev;
	
	ev = SaveData_GetEventWork( wk->save );
	
	return SysFlag_ArriveGet( ev, FLAG_ARRIVE_D32R0101 );
#if 0
	
	return SysFlag_GameClearCheck( ev );
#endif
}

//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	type	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BR_ExBattleCheck( int type )
{	
	int i;
	const int trtype[] = {
		TRTYPE_KUROTUGU,
		TRTYPE_FBOSS1,
		TRTYPE_FBOSS2,
		TRTYPE_FBOSS3,
		TRTYPE_FBOSS4,
	};
	
	for ( i = 0; i < NELEMS( trtype ); i++ ){		
		if ( type == trtype[ i ] ){
			return TRUE;
		}
	}
	
	return FALSE;
}

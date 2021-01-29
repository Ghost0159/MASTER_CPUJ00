//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_3dobjcont.c
 *	@brief		�o�ꂷ��l�������ׂĊǗ�����V�X�e��	
 *				�l���ɑ΂��郊�N�G�X�g�����ׂĂ����𓖂��čs��
 *	@author		tomoya takahashi
 *	@data		2007.11.13
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wf2dmap_cmdq.h"
#include "application/wifi_2dmap/wf2dmap_judge.h"

#include "wflby_def.h"
#include "wflby_3dobjcont.h"
#include "wflby_3dobj.h"
#include "wflby_3dmatrix.h"

#include "wflby_snd.h"

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
#define WFLBY_3DOBJCONT_QBUF_NUM	( 32 )	// �R�}���h�L���[�o�b�t�@��



//-------------------------------------
///	NPC����萔
//=====================================
enum{
	WFLBY_3DOBJCONT_NPC_SEQ_WAITSET,
	WFLBY_3DOBJCONT_NPC_SEQ_WAIT,
	WFLBY_3DOBJCONT_NPC_SEQ_WAYSET,
	WFLBY_3DOBJCONT_NPC_SEQ_WAYMOVEWALKTURN,
	WFLBY_3DOBJCONT_NPC_SEQ_WAYMOVEWAIT,
};
static const u8 sc_WFLBY_3DOBJCONT_NPC_WAIT[] = {
	32, 64, 128, 144
};
static const u8 sc_WFLBY_3DOBJCONT_NPC_WAY[] = {
	WF2DMAP_WAY_UP, WF2DMAP_WAY_DOWN, WF2DMAP_WAY_LEFT, WF2DMAP_WAY_RIGHT
};


//-------------------------------------
///	�̂ڂ�@������@����
//=====================================
// ������
enum{
	WFLBY_3DOBJCONT_FLYDOWN_SEQ_INIT,
	WFLBY_3DOBJCONT_FLYDOWN_SEQ_MAIN,
	WFLBY_3DOBJCONT_FLYDOWN_SEQ_ONEROTA,	// ����ɂP��]
	WFLBY_3DOBJCONT_FLYDOWN_SEQ_END,
};
// ���
enum{
	WFLBY_3DOBJCONT_FLYUP_SEQ_INIT,
	WFLBY_3DOBJCONT_FLYUP_SEQ_ONEROTA,	// �P��]
	WFLBY_3DOBJCONT_FLYUP_SEQ_MAIN,
	WFLBY_3DOBJCONT_FLYUP_SEQ_END,
};
#define WFLBY_3DOBJCONT_FLY_Y			( FX32_CONST(500) )	// ���ł�������
#define WFLBY_3DOBJCONT_FLY_COUNT		( 24 )		// ����V���N��
#define WFLBY_3DOBJCONT_ROTA_ONE_COUNT	( 32 )

//-------------------------------------
///	��]������ѓ���
//=====================================
enum {
	WFLBY_3DOBJCONT_ROTA_SEQ_INIT,
	WFLBY_3DOBJCONT_ROTA_SEQ_MAIN,
	WFLBY_3DOBJCONT_ROTA_SEQ_ONEROTA,	// ����ɂP��]
	WFLBY_3DOBJCONT_ROTA_SEQ_END,
} ;
#define WFLBY_3DOBJCONT_ROTA_DIST	( WFLBY_3DMATRIX_GRID_GRIDSIZ )	// �ړ�����l
#define WFLBY_3DOBJCONT_ROTA_COUNT	( 4 )							// �ړ�����V���N��
#define WFLBY_3DOBJCONT_ROTA_ONEROTA_COUNT	( 4 )
#define WFLBY_3DOBJCONT_ROTA_ONESIDEROTA	( 2 )			// ��]�P�Z���ɕK�v�ȃV���N��
#define WFLBY_3DOBJCONT_ROTA_ANMSPEED		( 2 )			// ��]�A�j���X�s�[�h


//-------------------------------------
///	�W�����v����
//=====================================
enum {
	WFLBY_3DOBJCONT_JUMP_SEQ_INIT,
	WFLBY_3DOBJCONT_JUMP_SEQ_MAIN,
} ;

#define WFLBY_3DOBJCONT_MOVEOBJ_JUMP_EFFCOUNT	( 6 )// �W�����v���Ă��鎞��
#define WFLBY_3DOBJCONT_MOVEOBJ_JUMP_WAITCOUNT	( 16 )// ���̃W�����v�܂ł̃E�G�C�g
#define WFLBY_3DOBJCONT_MOVEOBJ_JUMP_COUNT		( (WFLBY_3DOBJCONT_MOVEOBJ_JUMP_EFFCOUNT*2)+WFLBY_3DOBJCONT_MOVEOBJ_JUMP_WAITCOUNT )// �P�W�����v�J�E���g
#define WFLBY_3DOBJCONT_MOVEOBJ_JUMP_DIS	( 10 )// �W�����v�̍���
#define WFLBY_3DOBJCONT_MOVEOBJ_JUMP_ZDIS	( -2 )// �W�����v�̍���
#define WFLBY_3DOBJCONT_MOVEOBJ_JUMP_RMAX	(180)// ��]��MAX



//-------------------------------------
///	�e�̐F��ύX����V�X�e��
//=====================================
#define WFLBY_3DOBJCONT_SHADOW_ALHA_FADE_COUNT	( 60 )


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	���`���샏�[�N
//=====================================
typedef struct {
	fx32 x;
	fx32 s_x;
	fx32 dis_x;
	u32  count_max;
} WFLBY_3DOBJCONT_MOVE_WK;


//-------------------------------------
///	�e�̃A���t�@�l�ϓ��V�X�e��
//=====================================
typedef struct {
	u8	move;		// ���쒆��
	u8	now;		// ���̒l
	s8  start;		// �J�n�̃A���t�@�l
	s8	diff;		// �ω���
	s16 count;		// �J�E���g�l
	s16 count_max;	// �ő�J�E���g�l
} WFLBY_SHADOW_ALPHA;


//-------------------------------------
///	���샏�[�N�\����
//=====================================
typedef union{
	// MOVENPC���[�N
	struct{
		s16 wait;
		s16 way;
	}npc;

	// MOVEFLY�n���[�N
	struct{
		WFLBY_3DOBJCONT_MOVE_WK	y;
		u32						count;
	}fly;

	// rota�n���[�N
	struct{
		WFLBY_3DOBJCONT_MOVE_WK	dist;
		u32						count;
		fx32					def;
	}rota;

	// jump�n���[�N
	struct{
		s32						count;
		VecFx32					def_mat;
	}jump;
} WFLBY_3DPARSON_MOVEWK;

//-------------------------------------
///	���[�N�\����
//=====================================
typedef struct _WFLBY_3DPERSON{
	WF2DMAP_OBJWK* p_obj;
	WFLBY_3DOBJWK* p_draw;

	// ����֐����[�N
	u16						seq;
	u8						moveend;
	u8						movetype;
	WFLBY_3DPARSON_MOVEWK	work;

	// ��{����֐�
	void (*p_func)( struct _WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
}WFLBY_3DPERSON;
typedef void (*pWFLBY_3DPARSON_FUNC)( struct _WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );

//-------------------------------------
///	�V�X�e���\����
//=====================================
typedef struct _WFLBY_3DOBJCONT{
	WF2DMAP_OBJSYS*		p_objsys;
	WFLBY_3DOBJSYS*		p_drawsys;
	WFLBY_3DPERSON*		p_objbuf;
	u16					objnum;
	u16					hero_sex;
	WF2DMAP_REQCMDQ*	p_reqQ;
	const WFLBY_MAPCONT* cp_mapsys;
	WFLBY_SHADOW_ALPHA	shadow_alpha;

	WFLBY_3DPERSON*		p_player;
	WFLBY_3DPERSON*		p_pierrot;
}WFLBY_3DOBJCONT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�}�b�v�z�u�l��������
//=====================================
static void WFLBY_3DOBJCONT_MAPOBJ_Init( WFLBY_3DOBJCONT* p_sys );


//-------------------------------------
///	�R�}���h�ݒ�
//=====================================
static void WFLBY_3DOBJCONT_REQCMD_Set( WFLBY_3DOBJCONT* p_sys, s32 cmd, s32 way, s32 playid );


//-------------------------------------
///	���[�e�B���e�B�֐�
//=====================================
// �����Ńe�[�u���̒��̂P�̗v�f�̒l��Ԃ�
static u8 WFLBY_3DOBJCONT_GetRndTbl( const u8* cp_tbl, u32 tblnum );	
// �l����way�����̃O���b�h��OBJID���`�F�b�N����
static BOOL WFLBY_3DOBJCONT_CheckWayObjID( const WFLBY_3DOBJCONT* cp_sys, const WFLBY_3DPERSON* cp_obj, WF2DMAP_WAY way, u32 objid );
// ���`����
static void WFLBY_3DOBJCONT_MOVEWK_Init( WFLBY_3DOBJCONT_MOVE_WK* p_wk, fx32 s_x, fx32 e_x, u32 count_max );
static BOOL WFLBY_3DOBJCONT_MOVEWK_Main( WFLBY_3DOBJCONT_MOVE_WK* p_wk, u32 count );
static fx32 WFLBY_3DOBJCONT_MOVEWK_GetNum( const WFLBY_3DOBJCONT_MOVE_WK* cp_wk );



//-------------------------------------
///	�l�����[�N����
//=====================================
static WFLBY_3DPERSON* WFLBY_3DOBJCONT_GetCleanWk( WFLBY_3DOBJCONT* p_sys );
static BOOL WFLBY_3DOBJCONT_CheckCleanWk( const WFLBY_3DPERSON* cp_wk );
// �l������֐�
static void WFLBY_3DOBJCONT_MoveNone(  WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_PlayerMove(  WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_NpcMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_FlyUpMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_FlyDownMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_RotaLeftMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_RotaRightMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_RotaUpMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_RotaDownMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_RotaMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys, WF2DMAP_WAY way );
static void WFLBY_3DOBJCONT_Jump( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );
static void WFLBY_3DOBJCONT_JumpEnd( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys );


//-------------------------------------
///	�e�̃A���t�@�l�ύX�V�X�e��
//=====================================
static void WFLBY_3DOBJCONT_ShadowAlpha_Init( WFLBY_SHADOW_ALPHA* p_wk, WFLBY_3DOBJSYS* p_sys, u32 alpha );
static void WFLBY_3DOBJCONT_ShadowAlpha_Start( WFLBY_SHADOW_ALPHA* p_wk, u32 alpha );
static BOOL WFLBY_3DOBJCONT_ShadowAlpha_Main( WFLBY_SHADOW_ALPHA* p_wk, WFLBY_3DOBJSYS* p_sys );




//-----------------------------------------------------------------------------
/**
 *			�}�b�v�z�u�l���f�[�^
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	
//=====================================
typedef struct {
	u8	mapobjid;
	u8	way;
	u8 trtype;
	u8	playid;
} WFLBY_MAPOBJDATA;
static const WFLBY_MAPOBJDATA sc_WFLBY_3DPARSON_MAPOBJ[] = {
	{ WFLBY_MAPOBJID_NPC_SW_IN,		WF2DMAP_WAY_LEFT,	WIFISW, WFLBY_MAPPERSON_ID },
	{ WFLBY_MAPOBJID_NPC_SW_TOPIC,	WF2DMAP_WAY_DOWN,	WIFISW, WFLBY_MAPPERSON_ID },
	{ WFLBY_MAPOBJID_NPC_SW_TOPIC2,	WF2DMAP_WAY_DOWN,	WIFISM, WFLBY_MAPPERSON_ID },
	{ WFLBY_MAPOBJID_ANKETO_MAN,	WF2DMAP_WAY_DOWN,	WIFISM, WFLBY_MAPPERSON_PIERROT_ID },
};


//-----------------------------------------------------------------------------
/**
 *				����e�[�u��
 */
//-----------------------------------------------------------------------------
static const pWFLBY_3DPARSON_FUNC sc_WFLBY_3DPARSON_FUNC[WFLBY_3DOBJCONT_MOVENUM] = {
	WFLBY_3DOBJCONT_MoveNone,
	WFLBY_3DOBJCONT_PlayerMove,
	WFLBY_3DOBJCONT_NpcMove,
	WFLBY_3DOBJCONT_FlyUpMove,
	WFLBY_3DOBJCONT_FlyDownMove,
	WFLBY_3DOBJCONT_RotaUpMove,
	WFLBY_3DOBJCONT_RotaDownMove,
	WFLBY_3DOBJCONT_RotaLeftMove,
	WFLBY_3DOBJCONT_RotaRightMove,
	WFLBY_3DOBJCONT_Jump,
};

static const pWFLBY_3DPARSON_FUNC sc_WFLBY_3DPARSON_END_FUNC[WFLBY_3DOBJCONT_MOVENUM] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	WFLBY_3DOBJCONT_JumpEnd,
};




//-----------------------------------------------------------------------------
/**
 *		�e�̃A���t�@�l�e�[�u��
 */
//-----------------------------------------------------------------------------
static const u32 sc_WFLBY_3DOBJCONT_SHADOW_ALPHA[ WFLBY_LIGHT_NEON_ROOMNUM ] = {
	// WFLBY_LIGHT_NEON_ROOM0
	{ 16, },

	// WFLBY_LIGHT_NEON_ROOM1
	{ 12, },

	// WFLBY_LIGHT_NEON_ROOM2
	{ 8, },

	// WFLBY_LIGHT_NEON_ROOM3
	{ 6, },

	// WFLBY_LIGHT_NEON_ROOM4
	{ 0, },

	// WFLBY_LIGHT_NEON_ROOM5
	{ 0, },
};



// �V�X�e���Ǘ�
//----------------------------------------------------------------------------
/**
 *	@brief	�l���Ǘ��V�X�e��	������
 *
 *	@param	objnum		�I�u�W�F�N�g��
 *	@param	hero_sex	����
 *	@param	cp_map		�}�b�v�V�X�e��
 *	@param	heapID		�q�[�vID
 *	@param	gheapID		�O���t�B�b�N�q�[�vID
 *	
 *	@return	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
WFLBY_3DOBJCONT* WFLBY_3DOBJCONT_Init( u32 objnum, u32 hero_sex, const WFLBY_MAPCONT* cp_map, u32 heapID, u32 gheapID )
{
	WFLBY_3DOBJCONT* p_sys;

	p_sys = sys_AllocMemory( heapID, sizeof(WFLBY_3DOBJCONT) );
	memset( p_sys, 0, sizeof(WFLBY_3DOBJCONT) );

	// �V�X�e���쐬
	p_sys->p_objsys		= WF2DMAP_OBJSysInit( objnum, heapID );
	p_sys->p_drawsys	= WFLBY_3DOBJSYS_Init( objnum, hero_sex, heapID, gheapID );
	p_sys->p_reqQ		= WF2DMAP_REQCMDQSysInit( WFLBY_3DOBJCONT_QBUF_NUM, heapID );

	// �I�u�W�F�N�g���[�N�쐬
	p_sys->objnum		= objnum;
	p_sys->p_objbuf		= sys_AllocMemory( heapID, sizeof(WFLBY_3DPERSON)*objnum );
	memset( p_sys->p_objbuf, 0, sizeof(WFLBY_3DPERSON)*objnum );

	// �}�b�v�V�X�e����ۑ����Ă���
	p_sys->cp_mapsys = cp_map;

	// ��l���̐��ʂ�ۑ����Ă���
	p_sys->hero_sex = hero_sex;

	// ��t�̂��˂�����Ȃǂ�ǉ�
	WFLBY_3DOBJCONT_MAPOBJ_Init( p_sys );
	
	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���Ǘ��V�X�e��	�j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_Exit( WFLBY_3DOBJCONT* p_sys )
{
	
	//  �I�u�W�F�N�g���[�N�j��
	sys_FreeMemoryEz( p_sys->p_objbuf );
	
	// �V�X�e���j��
	WF2DMAP_REQCMDQSysExit( p_sys->p_reqQ );
	WFLBY_3DOBJSYS_Exit( p_sys->p_drawsys );
	WF2DMAP_OBJSysExit( p_sys->p_objsys );

	// �V�X�e�����̂�j��
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���V�X�e��	���쏈��
 *		
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_Move( WFLBY_3DOBJCONT* p_sys )
{
	// �I�u�W�F�N�g�V�X�e��
	WF2DMAP_OBJSysMain( p_sys->p_objsys );

	// �e�̐F��ύX
	WFLBY_3DOBJCONT_ShadowAlpha_Main( &p_sys->shadow_alpha, p_sys->p_drawsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���V�X�e��	���N�G�X�g���f����
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_ReqMove( WFLBY_3DOBJCONT* p_sys )
{

	// �v���C���[����
	{
		int i;
		for( i=0; i<p_sys->objnum; i++ ){
			if( WFLBY_3DOBJCONT_CheckCleanWk( &p_sys->p_objbuf[i] ) == FALSE ){
				p_sys->p_objbuf[i].p_func( &p_sys->p_objbuf[i], p_sys );
			}
		}
	}
	
	// ����`�F�b�N����
	{
		WF2DMAP_REQCMD req;
		WF2DMAP_ACTCMD act;
		BOOL result;
		const WF2DMAP_MAPSYS* cp_map_sys;

		// �}�b�v�R�A�V�X�e��
		cp_map_sys = WFLBY_MAPCONT_GetMapCoreSys( p_sys->cp_mapsys );

		// �R�}���h���ԃR�}���h�����s
		while( WF2DMAP_REQCMDQSysCmdPop( p_sys->p_reqQ, &req ) == TRUE ){
			result = WF2DMAP_JUDGESysCmdJudge( cp_map_sys, p_sys->p_objsys, &req, &act );
			if( result == TRUE ){
				WF2DMAP_OBJSysCmdSet( p_sys->p_objsys, &act );
			}
		}
	}

	// �`��A�b�v�f�[�g
	WFLBY_3DOBJSYS_Updata( p_sys->p_drawsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���Ǘ��V�X�e��	�`�揈��
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_Draw( WFLBY_3DOBJCONT* p_sys )
{
	WFLBY_3DOBJSYS_Draw( p_sys->p_drawsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���Ǘ��V�X�e��	VBlank����
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_VBlank( WFLBY_3DOBJCONT* p_sys )
{
	WFLBY_3DOBJSYS_VBlank( p_sys->p_drawsys );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�e�̐F������������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	roomtype	�����̖��邳�^�C�v
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_InitShadowAlpha( WFLBY_3DOBJCONT* p_sys, WFLBY_LIGHT_NEON_ROOMTYPE roomtype )
{
	WFLBY_3DOBJCONT_ShadowAlpha_Init( &p_sys->shadow_alpha, p_sys->p_drawsys, sc_WFLBY_3DOBJCONT_SHADOW_ALPHA[ roomtype ] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���̉e�̐F��ݒ肷��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	roomtype	�����^�C�v
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_SetShadowAlpha( WFLBY_3DOBJCONT* p_sys, WFLBY_LIGHT_NEON_ROOMTYPE roomtype )
{
	WFLBY_3DOBJCONT_ShadowAlpha_Start( &p_sys->shadow_alpha, sc_WFLBY_3DOBJCONT_SHADOW_ALPHA[ roomtype ] );
}


// ���N�G�X�g�R�}���h�ݒ�
//----------------------------------------------------------------------------
/**
 *	@brief	���N�G�X�g�R�}���h��ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_cmd		�R�}���h�f�[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_SetReqCmd( WFLBY_3DOBJCONT* p_sys, const WF2DMAP_REQCMD* cp_cmd )
{
	WF2DMAP_REQCMDQSysCmdPush( p_sys->p_reqQ, cp_cmd );
}

//  ���[�N����
//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[��o�^
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	plid		�v���C���[ID
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
WFLBY_3DPERSON* WFLBY_3DOBJCONT_AddPlayer( WFLBY_3DOBJCONT* p_sys, u32 plid )
{
	u16 addx, addy;
	BOOL result;

	// �o�^�ʒu�����߂�
	result = WFLBY_MAPCONT_SarchObjID( p_sys->cp_mapsys, 
			WFLBY_MAPOBJID_PLAYER_IN, &addx, &addy, 0 );
	GF_ASSERT( result );	// �o�^�ʒu���Ȃ�
	return WFLBY_3DOBJCONT_AddPlayerEx( p_sys, plid, addx, addy );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l���̓o�^	�o���ʒu�w��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	plid		�v���C���[ID
 *	@param	gridx		�O���b�hX
 *	@param	gridy		�O���b�hY
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
WFLBY_3DPERSON* WFLBY_3DOBJCONT_AddPlayerEx( WFLBY_3DOBJCONT* p_sys, u32 plid, u32 gridx,  u32 gridy )
{
	WFLBY_3DPERSON* p_wk;
	
	// ��̃��[�N���擾
	p_wk = WFLBY_3DOBJCONT_GetCleanWk( p_sys );
	
	// OBJ�̓o�^
	{
		WF2DMAP_OBJDATA	objdata;
		objdata.x		= WF2DMAP_GRID2POS(gridx);
		objdata.y		= WF2DMAP_GRID2POS(gridy);
		objdata.playid	= plid;
		objdata.status	= WF2DMAP_OBJST_NONE;
		objdata.way		= WF2DMAP_WAY_UP;
		if( p_sys->hero_sex == PM_MALE ){
			objdata.charaid	= HERO;
		}else{
			objdata.charaid	= HEROINE;
		}
		p_wk->p_obj = WF2DMAP_OBJWkNew( p_sys->p_objsys, &objdata );
	}

	// drawobj�̓o�^
	{
		p_wk->p_draw = WFLBY_3DOBJWK_New( p_sys->p_drawsys, p_wk->p_obj );
	}

	// �v���C���[�f�[�^�͕ۑ����Ă���
	p_sys->p_player = p_wk;

	// ����֐��ݒ�
	WFLBY_3DOBJCONT_SetWkMove( p_sys, p_wk, WFLBY_3DOBJCONT_MOVENONE );

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC��o�^
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	plid	�v���C���[ID
 *	@param	trtype	trainer�^�C�v
 *
 *	@return	���[�N	NULL	���͓o�^�ł��܂���
 */
//-----------------------------------------------------------------------------
WFLBY_3DPERSON* WFLBY_3DOBJCONT_AddNpc( WFLBY_3DOBJCONT* p_sys, u32 plid, u32 trtype )
{
	WFLBY_3DPERSON* p_wk;
	
	// ��̃��[�N���擾
	p_wk = WFLBY_3DOBJCONT_GetCleanWk( p_sys );
	
	// OBJ�̓o�^
	{
		WF2DMAP_OBJDATA	objdata;
		u16 addx, addy, point_idx;
		BOOL result;
		BOOL add_ok = FALSE;
		WFLBY_3DPERSON* p_player;

		// �o�^�ʒu�����߂�
		// ��l�������Ȃ��āA���̐l���o�邱�Ƃ̏o����ꏊ
		point_idx = 0;
		do{
			result = WFLBY_MAPCONT_SarchObjID( p_sys->cp_mapsys, 
					WFLBY_MAPOBJID_NPC_00+plid, &addx, &addy, point_idx );
			
			if( result == FALSE ){
				OS_TPrintf( "npc �o�^�s�\\n" );
				return NULL;	// ���ݓo�^�s�\
			}else{
				// ���̈ʒu�Ɏ�l�������Ȃ����`�F�b�N
				p_player = WFLBY_3DOBJCONT_GetPlayer( p_sys );
				if( p_player == NULL ){
					add_ok = TRUE;
				}else{
					if( WFLBY_3DOBJCONT_CheckGridHit( p_player, addx, addy ) == FALSE ){
						add_ok = TRUE;
					}
				}
			}
			
			// ���̃|�C���g���`�F�b�N
			point_idx ++;
		}while( add_ok == FALSE );

		objdata.x		= WF2DMAP_GRID2POS(addx);
		objdata.y		= WF2DMAP_GRID2POS(addy);
		objdata.playid	= plid;
		objdata.status	= WF2DMAP_OBJST_NONE;
		objdata.way		= WF2DMAP_WAY_DOWN;
		objdata.charaid	= trtype;
		p_wk->p_obj = WF2DMAP_OBJWkNew( p_sys->p_objsys, &objdata );
	}

	// drawobj�̓o�^
	{
		p_wk->p_draw = WFLBY_3DOBJWK_New( p_sys->p_drawsys, p_wk->p_obj );
	}

	// ����֐��ݒ�
	WFLBY_3DOBJCONT_SetWkMove( p_sys, p_wk, WFLBY_3DOBJCONT_MOVENONE );

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l����j��
 *	
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_Delete( WFLBY_3DPERSON* p_wk )
{
	WFLBY_3DOBJWK_Del( p_wk->p_draw );
	WF2DMAP_OBJWkDel( p_wk->p_obj );

	memset( p_wk, 0, sizeof(WFLBY_3DPERSON) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[ID�Ől�����擾
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	plid	�v���C���[ID
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
WFLBY_3DPERSON* WFLBY_3DOBJCONT_GetPlIDWk( WFLBY_3DOBJCONT* p_sys, u32 plid )
{
	int i;
	u32 obj_plid;

	for( i=0; i<p_sys->objnum; i++ ){
		if( p_sys->p_objbuf[i].p_obj ){
			obj_plid = WF2DMAP_OBJWkDataGet( p_sys->p_objbuf[i].p_obj, WF2DMAP_OBJPM_PLID );
			if( obj_plid == plid ){
				return &p_sys->p_objbuf[i];
			}
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l�����擾����
 *
 *	@param	p_sys	���[�N
 *
 *	@return	��l��
 */
//-----------------------------------------------------------------------------
WFLBY_3DPERSON* WFLBY_3DOBJCONT_GetPlayer( WFLBY_3DOBJCONT* p_sys )
{
	return p_sys->p_player;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�s�G�����擾
 *
 *	@param	p_sys	���[�N
 *
 *	@return	�s�G���I�u�W�F�f�[�^
 */
//-----------------------------------------------------------------------------
WFLBY_3DPERSON* WFLBY_3DOBJCONT_GetPierrot( WFLBY_3DOBJCONT* p_sys )
{
	return p_sys->p_pierrot;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�̐ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	pos			�ʒu
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_SetWkPos( WFLBY_3DPERSON* p_wk, WF2DMAP_POS pos )
{
	// ���݈ʒu�A�ߋ��ʒu�����ɓ����
	GF_ASSERT( p_wk->p_obj );
	WF2DMAP_OBJWkMatrixSet( p_wk->p_obj, pos );
	WF2DMAP_OBJWkLastMatrixSet( p_wk->p_obj, pos );
	WF2DMAP_OBJWkDataSet( p_wk->p_obj, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_NONE );
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�ݒ�{����
 *
 *	@param	p_wk	���[�N
 *	@param	pos		�ʒu
 *	@param	way		����
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_SetWkPosAndWay( WFLBY_3DPERSON* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	WFLBY_3DOBJCONT_SetWkPos( p_wk, pos );
	WF2DMAP_OBJWkDataSet( p_wk->p_obj, WF2DMAP_OBJPM_WAY, way );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�W�V�������擾����
 *
 *	@param	cp_wk	���[�N
 */
//-----------------------------------------------------------------------------
WF2DMAP_POS WFLBY_3DOBJCONT_GetWkPos( const WFLBY_3DPERSON* cp_wk )
{
	GF_ASSERT( cp_wk->p_obj );
	return WF2DMAP_OBJWkFrameMatrixGet( cp_wk->p_obj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	wf2dmap_obj�̃f�[�^���擾����
 *
 *	@param	cp_wk	���[�N
 *	@param	pm		�擾����f�[�^�̒萔
 *
 *	@return	�f�[�^
 */
//-----------------------------------------------------------------------------
u32 WFLBY_3DOBJCONT_GetWkObjData( const WFLBY_3DPERSON* cp_wk, WF2DMAP_OBJPARAM pm )
{
	GF_ASSERT( cp_wk->p_obj );
	return WF2DMAP_OBJWkDataGet( cp_wk->p_obj, pm );
}

//----------------------------------------------------------------------------
/**
 *	@brief	wf2dmap_obj�̃��[�N���擾����
 *
 *	@param	p_wk	���[�N
 *
 *	@return	WF2DMAP_OBJWK�|�C���^
 */
//-----------------------------------------------------------------------------
WF2DMAP_OBJWK* WFLBY_3DOBJCONT_GetWkObjWk( WFLBY_3DPERSON* p_wk )
{
	GF_ASSERT( p_wk->p_obj );
	return p_wk->p_obj;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���̃��N�G�X�g�R�}���h�𔭍s����
 *
 *	@param	p_sys		�I�u�W�F�Ǘ��V�X�e��
 *	@param	cp_wk		�l�����[�N
 *	@param	cmd			�R�}���h
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_SetWkReqCmd( WFLBY_3DOBJCONT* p_sys, const WFLBY_3DPERSON* cp_wk, WF2DMAP_CMD cmd, WF2DMAP_WAY way )
{
	u32 status;
	u32 playid;

	status = WF2DMAP_OBJWkDataGet( cp_wk->p_obj, WF2DMAP_OBJPM_ST );
	GF_ASSERT_MSG( status == WF2DMAP_OBJST_NONE, "cmd req status not none\n" );
	playid = WF2DMAP_OBJWkDataGet( cp_wk->p_obj, WF2DMAP_OBJPM_PLID );
	WFLBY_3DOBJCONT_REQCMD_Set( p_sys, cmd, way, playid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�N�V�����R�}���h�ŋ����I�ɓ�����
 *
 *	@param	p_sys		�I�u�W�F�Ǘ��V�X�e��
 *	@param	cp_wk		�l�����[�N
 *	@param	cmd			�R�}���h
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_SetWkActCmd( WFLBY_3DOBJCONT* p_sys, const WFLBY_3DPERSON* cp_wk, WF2DMAP_CMD cmd, WF2DMAP_WAY way )
{
	WF2DMAP_ACTCMD act;
	act.pos		= WF2DMAP_OBJWkMatrixGet( cp_wk->p_obj );
	act.cmd		= cmd;
	act.way		= way;
	act.playid	= WF2DMAP_OBJWkDataGet( cp_wk->p_obj, WF2DMAP_OBJPM_PLID );
	WF2DMAP_OBJSysCmdSet( p_sys->p_objsys, &act );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���̓����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	movetype	����^�C�v
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_SetWkMove( WFLBY_3DOBJCONT* p_sys, WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT_MOVETYPE movetype )
{
	GF_ASSERT( movetype < WFLBY_3DOBJCONT_MOVENUM );

	// ����̏I������
	if( sc_WFLBY_3DPARSON_END_FUNC[ p_wk->movetype ] != NULL ){
		sc_WFLBY_3DPARSON_END_FUNC[ p_wk->movetype ]( p_wk, p_sys );
	}
	
	p_wk->seq		= 0;
	p_wk->moveend	= FALSE;
	p_wk->movetype	= movetype;
	memset( &p_wk->work, 0, sizeof(WFLBY_3DPARSON_MOVEWK) );
	p_wk->p_func	= sc_WFLBY_3DPARSON_FUNC[ movetype ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[���삪�I�����Ă��邩�`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJCONT_CheckWkMoveEnd( const WFLBY_3DPERSON* cp_wk )
{
	return cp_wk->moveend;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[����^�C�v���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	����^�C�v
 */
//-----------------------------------------------------------------------------
WFLBY_3DOBJCONT_MOVETYPE WFLBY_3DOBJCONT_GetWkMove( const WFLBY_3DPERSON* cp_wk )
{
	return cp_wk->movetype;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���̖ڂ̑O�̐l�����擾����
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_wk		���[�N
 *
 *	@retval			�ڂ̑O�̐l��
 *	@retval	NULL	�ڂ̑O�ɂ͒N�����Ȃ�
 */
//-----------------------------------------------------------------------------
WFLBY_3DPERSON* WFLBY_3DOBJCONT_GetFrontPerson( WFLBY_3DOBJCONT* p_sys, const WFLBY_3DPERSON* cp_wk )
{
	const WF2DMAP_OBJWK* cp_hitobj;
	u32 way;
	u32 hitobj_userid;

	way = WF2DMAP_OBJWkDataGet( cp_wk->p_obj, WF2DMAP_OBJPM_WAY );
	cp_hitobj = WF2DMAP_OBJSysHitCheck( cp_wk->p_obj, p_sys->p_objsys, way );

	// �������H
	if( cp_hitobj == NULL ){
		return NULL;
	}

	hitobj_userid = WF2DMAP_OBJWkDataGet( cp_hitobj, WF2DMAP_OBJPM_PLID );
	return WFLBY_3DOBJCONT_GetPlIDWk( p_sys, hitobj_userid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����O�t���O�ݒ�
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�J�����O���Ĕ�\����
 *	@retval	FALSE	�J�����O���ĂȂ��\����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJCONT_GetCullingFlag( const WFLBY_3DPERSON* cp_wk )
{
	return WFLBY_3DOBJWK_GetCullingFlag( cp_wk->p_draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�w�肵���O���b�h�ɐl�������邩�`�F�b�N
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	gridx		�O���b�h��
 *	@param	gridy		�O���b�h��
 *
 *	@retval	NULL�ȊO	������������
 *	@retval	NULL		�l���͂��Ȃ�
 */
//-----------------------------------------------------------------------------
const WFLBY_3DPERSON* WFLBY_3DOBJCONT_CheckSysGridHit( const WFLBY_3DOBJCONT* cp_sys, u16 gridx, u16 gridy )
{
	const WF2DMAP_OBJWK* cp_obj;
	int i;
	WF2DMAP_POS pos;

	pos.x = WF2DMAP_GRID2POS( gridx );
	pos.y = WF2DMAP_GRID2POS( gridy );
	
	cp_obj = WF2DMAP_OBJSysPosHitCheck( cp_sys->p_objsys, pos );

	if( cp_obj == NULL ){	// ���Ȃ�����
		return NULL;
	}

	// ���̃I�u�W�F�̎������f�[�^��Ԃ�
	for( i=0; i<cp_sys->objnum; i++ ){

		if( cp_sys->p_objbuf[i].p_obj == cp_obj ){
			return &cp_sys->p_objbuf[i];
		}
	}

	GF_ASSERT( 0 );	// ��������
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���b�h�ʒu�ɂ��̃I�u�W�F�����邩�`�F�b�N����
 *
 *	@param	cp_wk		���[�N
 *	@param	gridx		�O���b�h�����W
 *	@param	gridy		�O���b�h�����W
 *
 *	@retval	TRUE	���̈ʒu�ɂ���
 *	@retval	FALSE	���̈ʒu�ɂ��Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJCONT_CheckGridHit( const WFLBY_3DPERSON* cp_wk, u16 gridx, u16 gridy )
{
	WF2DMAP_POS pos;
	WF2DMAP_POS last_pos;
	
	pos			= WF2DMAP_OBJWkMatrixGet( cp_wk->p_obj );
	last_pos	= WF2DMAP_OBJWkLastMatrixGet( cp_wk->p_obj );

	if( (pos.x == WF2DMAP_GRID2POS(gridx)) && (pos.y == WF2DMAP_GRID2POS(gridy)) ){
		return TRUE;
	}
	if( (last_pos.x == WF2DMAP_GRID2POS(gridx)) && (last_pos.y == WF2DMAP_GRID2POS(gridy)) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���O�S�����ɋ󂢂Ă���ꏊ�����邩�`�F�b�N����
 *
 *	@param	p_sys		�V�X�e��
 *	@param	cp_wk		���[�N
 *	@param	p_way		����
 *	@param	p_pos		�ʒu
 *
 *	@retval	TRUE	�󂢂Ă���Ƃ��낪������
 *	@retval	FALSE	�󂢂Ă���Ƃ��낪�Ȃ�
 *
 *	�D�揇�ʂ́A�E������������
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJCONT_GetOpenGird4Way( WFLBY_3DOBJCONT* p_sys, const WFLBY_3DPERSON* cp_wk, u32* p_way, WF2DMAP_POS* p_pos )
{
	static const u8 sc_WAY[ WF2DMAP_WAY_NUM ] = {
		WF2DMAP_WAY_RIGHT,	
		WF2DMAP_WAY_LEFT,	
		WF2DMAP_WAY_DOWN,	
		WF2DMAP_WAY_UP,	
	};
	int i;
	WF2DMAP_POS pos;
	WF2DMAP_POS waypos;
	BOOL hit;
	u32 eventid;
	const WF2DMAP_OBJWK* cp_hitobj;

	// �l���̍��W���擾
	pos = WF2DMAP_OBJWkMatrixGet( cp_wk->p_obj );

	
	for( i=0; i<WF2DMAP_WAY_NUM; i++ ){
		waypos = WF2DMAP_OBJToolWayPosGet( pos, sc_WAY[i] );	

		// �n�`�Ɣ���
		// �����蔻��
		hit = WFLBY_MAPCONT_HitGet( p_sys->cp_mapsys, 
				WF2DMAP_POS2GRID(waypos.x), WF2DMAP_POS2GRID(waypos.y) );
		if( hit == TRUE ){
			continue;	// ����
		}
		// ���̈ʒu�ɉ����̃C�x���g���Ȃ���
		eventid = WFLBY_MAPCONT_EventGet( p_sys->cp_mapsys, 
				WF2DMAP_POS2GRID(waypos.x), WF2DMAP_POS2GRID(waypos.y) );
		if( (eventid != 0) && (eventid != WFLBY_MAPEVID_EV_ROOF_MAT) ){
			continue;	// ����
		}

		// �����������Ȃ����`�F�b�N
		cp_hitobj = WF2DMAP_OBJSysPosHitCheck( p_sys->p_objsys, waypos );
		if( cp_hitobj != NULL ){
			continue;	// ����
		}

		// ���̈ʒu�󂢂Ă܂�
		*p_way = sc_WAY[i];
		*p_pos = waypos;
		return TRUE;
	}
	return FALSE;
}

// �O������\�������̂ݕύX����
//----------------------------------------------------------------------------
/**
 *	@brief	�`��X�V�t���O��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	updata		�A�b�v�f�[�g�t���O
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_SetUpdata( WFLBY_3DPERSON* p_wk, BOOL updata )
{
	WFLBY_3DOBJWK_SetUpdata( p_wk->p_draw, updata );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��A�b�v�f�[�g�t���O���擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�A�b�v�f�[�g��
 *	@retval	FALSE	�A�b�v�f�[�g���Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJCONT_DRAW_GetUpdata( const WFLBY_3DPERSON* cp_wk )
{
	return WFLBY_3DOBJWK_GetUpdata( cp_wk->p_draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_pos		�ʒu
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_SetMatrix( WFLBY_3DPERSON* p_wk, const WF2DMAP_POS* cp_pos )
{
	WFLBY_3DOBJWK_SetMatrix( p_wk->p_draw, cp_pos );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD���W��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_vec		�x�N�g��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_Set3DMatrix( WFLBY_3DPERSON* p_wk, const VecFx32* cp_vec )
{
	WFLBY_3DOBJWK_Set3DMatrix( p_wk->p_draw, cp_vec );
}

//----------------------------------------------------------------------------
/**
 *	@brief	3D���W���擾����
 *
 *	@param	cp_wk	���[�N
 *	@param	p_vec	�x�N�g���i�[��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_Get3DMatrix( const WFLBY_3DPERSON* cp_wk, VecFx32* p_vec )
{
	WFLBY_3DOBJWK_Get3DMatrix( cp_wk->p_draw, p_vec );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�悷�������ύX
 *
 *	@param	p_wk	���[�N
 *	@param	way		����
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_SetWay( WFLBY_3DPERSON* p_wk, WF2DMAP_WAY way )
{
	WFLBY_3DOBJWK_SetWay( p_wk->p_draw, way );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��	�W�����v�V�[���̕`��
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�s�q�t�d�F�Đ�	�e�`�k�r�d�F�I����
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_SetAnmJump( WFLBY_3DPERSON* p_wk, BOOL flag )
{
	if( flag ){
		WFLBY_3DOBJWK_StartAnm( p_wk->p_draw, WFLBY_3DOBJ_ANM_JUMP );
	}else{
		WFLBY_3DOBJWK_EndAnm( p_wk->p_draw );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��	��]�A�j���`��
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�s�q�t�d�F�Đ�	�e�`�k�r�d�F�I����
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_SetAnmRota( WFLBY_3DPERSON* p_wk, BOOL flag )
{
	if( flag ){
		WFLBY_3DOBJWK_StartAnm( p_wk->p_draw, WFLBY_3DOBJ_ANM_ROTA );
	}else{
		WFLBY_3DOBJWK_EndAnm( p_wk->p_draw );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��	�o�^�o�^�A�j���`��
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�s�q�t�d�F�Đ�	�e�`�k�r�d�F�I����
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_SetAnmBata( WFLBY_3DPERSON* p_wk, BOOL flag )
{
	if( flag ){
		WFLBY_3DOBJWK_StartAnm( p_wk->p_draw, WFLBY_3DOBJ_ANM_BATABATA );
	}else{
		WFLBY_3DOBJWK_EndAnm( p_wk->p_draw );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��	���C�g�t���O��ON
 *
 *	@param	p_wk		���[�N
 *	@param	light_msk	���C�g�}�X�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_SetLight( WFLBY_3DPERSON* p_wk, u32 light_msk )
{
	WFLBY_3DOBJWK_SetLight( p_wk->p_draw, light_msk );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�`��	�`��t���O��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�t���O
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJCONT_DRAW_SetDraw( WFLBY_3DPERSON* p_wk,  BOOL flag )
{
	WFLBY_3DOBJWK_SetDrawFlag( p_wk->p_draw, flag );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��	�`��t���O���擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�`�撆
 *	@retval	FALSE	��\����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJCONT_DRAW_GetDraw( const WFLBY_3DPERSON* cp_wk )
{
	return WFLBY_3DOBJWK_GetDrawFlag( cp_wk->p_draw );
}








//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�z�u�l���̔z�u
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_MAPOBJ_Init( WFLBY_3DOBJCONT* p_sys )
{
	int i;
	WFLBY_3DPERSON* p_wk;
	u16 addx, addy;
	BOOL result;
	u16 idx;
	
	// �}�b�v�̒��ɔz�u�I�u�W�F�����Ȃ����`�F�b�N
	for( i=0; i<NELEMS(sc_WFLBY_3DPARSON_MAPOBJ); i++ ){
		idx = 0;
		while( WFLBY_MAPCONT_SarchObjID( p_sys->cp_mapsys, 
					sc_WFLBY_3DPARSON_MAPOBJ[i].mapobjid, &addx, &addy, idx ) == TRUE ){

			// ��̃��[�N���擾
			p_wk = WFLBY_3DOBJCONT_GetCleanWk( p_sys );

			// �o�^
			{
				WF2DMAP_OBJDATA	objdata;

				objdata.x		= WF2DMAP_GRID2POS(addx);
				objdata.y		= WF2DMAP_GRID2POS(addy);
				objdata.playid	= sc_WFLBY_3DPARSON_MAPOBJ[i].playid;
				objdata.status	= WF2DMAP_OBJST_NONE;
				objdata.way		= sc_WFLBY_3DPARSON_MAPOBJ[i].way;
				objdata.charaid	= sc_WFLBY_3DPARSON_MAPOBJ[i].trtype;
				p_wk->p_obj = WF2DMAP_OBJWkNew( p_sys->p_objsys, &objdata );
			}

			// drawobj�̓o�^
			{
				p_wk->p_draw = WFLBY_3DOBJWK_New( p_sys->p_drawsys, p_wk->p_obj );
			}

			// ����֐��ݒ�
			WFLBY_3DOBJCONT_SetWkMove( p_sys, p_wk, WFLBY_3DOBJCONT_MOVENONE );

			// �s�G�������ۑ�����
			if( sc_WFLBY_3DPARSON_MAPOBJ[i].mapobjid == WFLBY_MAPOBJID_ANKETO_MAN ){
				p_sys->p_pierrot = p_wk;
			}

			idx ++;
		}
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�L���[�ɃR�}���h��ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cmd			�R�}���h
 *	@param	way			����
 *	@param	playid		�v���C���[�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_REQCMD_Set( WFLBY_3DOBJCONT* p_sys, s32 cmd, s32 way, s32 playid )
{
	WF2DMAP_REQCMD reqcmd;
	
	reqcmd.cmd = cmd;
	reqcmd.way = way;
	reqcmd.playid = playid;
	WFLBY_3DOBJCONT_SetReqCmd( p_sys, &reqcmd );
}





//----------------------------------------------------------------------------
/**
 *	@brief	�e�[�u���̒�����D���ȃf�[�^�𗐐��Ŏ擾����
 *
 *	@param	cp_tbl		�e�[�u��
 *	@param	tblnum		�e�[�u����
 *
 *	@return	�I�����ꂽ����
 */
//-----------------------------------------------------------------------------
static u8 WFLBY_3DOBJCONT_GetRndTbl( const u8* cp_tbl, u32 tblnum )
{
	int idx;
	idx = gf_mtRand() % tblnum;
	return cp_tbl[idx];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l����way�̕����ɂ���̂�objid�̃}�b�v���`�F�b�N
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	cp_obj		�l�����[�N
 *	@param	way			����
 *	@param	objid		�I�u�W�FID
 *
 *	@retval	TRUE	way�̕����ɂ���̂�objid
 *	@retval	FALSE	way�̕����ɂ���̂�objid�ł͂Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DOBJCONT_CheckWayObjID( const WFLBY_3DOBJCONT* cp_sys, const WFLBY_3DPERSON* cp_obj, WF2DMAP_WAY way, u32 objid )
{
	WF2DMAP_POS pos;
	u32 mapobjid;

	// way�����̍��W�����߂�
	pos = WF2DMAP_OBJWkMatrixGet( cp_obj->p_obj );
	pos = WF2DMAP_OBJToolWayPosGet( pos, way );

	// ������OBJID���擾
	mapobjid = WFLBY_MAPCONT_ObjIDGet( cp_sys->cp_mapsys, WF2DMAP_POS2GRID(pos.x), WF2DMAP_POS2GRID(pos.y) );
	
	if( mapobjid == objid ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���`���쏉����
 *
 *	@param	p_wk		���[�N
 *	@param	s_x			�J�n���W
 *	@param	e_x			�I�����W
 *	@param	count_max	�J�E���g�ő�l
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_MOVEWK_Init( WFLBY_3DOBJCONT_MOVE_WK* p_wk, fx32 s_x, fx32 e_x, u32 count_max )
{
	p_wk->x			= s_x;
	p_wk->s_x		= s_x;
	p_wk->dis_x		= e_x - s_x;
	p_wk->count_max	= count_max;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���`���상�C��
 *
 *	@param	p_wk		���[�N
 *	@param	count		�J�E���^
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DOBJCONT_MOVEWK_Main( WFLBY_3DOBJCONT_MOVE_WK* p_wk, u32 count )
{
	fx32 w_x;

	// ���ݍ��W�擾
	w_x = FX_Mul( p_wk->dis_x, FX32_CONST(count) );
	w_x = FX_Div( w_x, FX32_CONST(p_wk->count_max) );

	
	p_wk->x = w_x + p_wk->s_x;

	if( count >= p_wk->count_max ){
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�l
 */
//-----------------------------------------------------------------------------
static fx32 WFLBY_3DOBJCONT_MOVEWK_GetNum( const WFLBY_3DOBJCONT_MOVE_WK* cp_wk )
{
	return cp_wk->x;
}




//----------------------------------------------------------------------------
/**
 *	@brief	�󂢂Ă��郏�[�N���擾����
 *
 *	@param	p_sys		�V�X�e�� 
 *
 *	@return	�󂢂Ă��郏�[�N
 */
//-----------------------------------------------------------------------------
static WFLBY_3DPERSON* WFLBY_3DOBJCONT_GetCleanWk( WFLBY_3DOBJCONT* p_sys )
{
	int i;

	for( i=0; i<p_sys->objnum; i++ ){
		if( WFLBY_3DOBJCONT_CheckCleanWk( &p_sys->p_objbuf[i] ) ){
			return &p_sys->p_objbuf[i];
		}
	}

	GF_ASSERT(0);
	return &p_sys->p_objbuf[i];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�󂢂Ă��郏�[�N���`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�󂢂Ă�
 *	@retval	FALSE	�󂢂ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DOBJCONT_CheckCleanWk( const WFLBY_3DPERSON* cp_wk )
{
	if( cp_wk->p_obj == NULL ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	����Ȃ�
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_MoveNone(  WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l���̓���
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_PlayerMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys  )
{
	s32 way;
	s32 playid;
	s32 status;
	s32 cmd;
	const WF2DMAP_OBJWK* cp_wk;

	way = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_WAY );
	playid = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_PLID );

	// �ҋ@��ԂłȂ���΂Ȃ�Ȃ�
	status = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_ST );
	if( status != WF2DMAP_OBJST_NONE ){
		return;
	}


	// ���������邩
	if( sys.cont & PAD_BUTTON_B ){
		cmd = WF2DMAP_OBJST_RUN;
	}else{
		cmd = WF2DMAP_OBJST_WALK;
	}
	
	// �ړ��֌W
	if( sys.cont & PAD_KEY_UP ){
		if( way == WF2DMAP_WAY_UP ){
			WFLBY_3DOBJCONT_REQCMD_Set( p_sys, cmd, way, playid );
		}else{
			WFLBY_3DOBJCONT_REQCMD_Set( p_sys, WF2DMAP_OBJST_TURN, WF2DMAP_WAY_UP, playid );
		}
	}else if( sys.cont & PAD_KEY_DOWN ){
		if( way == WF2DMAP_WAY_DOWN ){
			WFLBY_3DOBJCONT_REQCMD_Set( p_sys, cmd, way, playid );
		}else{
			WFLBY_3DOBJCONT_REQCMD_Set( p_sys, WF2DMAP_OBJST_TURN, WF2DMAP_WAY_DOWN, playid );
		}
	}else if( sys.cont & PAD_KEY_LEFT ){
		if( way == WF2DMAP_WAY_LEFT ){
			WFLBY_3DOBJCONT_REQCMD_Set( p_sys, cmd, way, playid );
		}else{
			WFLBY_3DOBJCONT_REQCMD_Set( p_sys, WF2DMAP_OBJST_TURN, WF2DMAP_WAY_LEFT, playid );
		}
	}else if( sys.cont & PAD_KEY_RIGHT ){
		if( way == WF2DMAP_WAY_RIGHT ){
			WFLBY_3DOBJCONT_REQCMD_Set( p_sys, cmd, way, playid );
		}else{
			WFLBY_3DOBJCONT_REQCMD_Set( p_sys, WF2DMAP_OBJST_TURN, WF2DMAP_WAY_RIGHT, playid );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC�̓���
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_NpcMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
	switch( p_wk->seq ){
	// �E�G�C�g�𗐐��Ō���
	case WFLBY_3DOBJCONT_NPC_SEQ_WAITSET:
		p_wk->work.npc.wait = WFLBY_3DOBJCONT_GetRndTbl( sc_WFLBY_3DOBJCONT_NPC_WAIT, NELEMS( sc_WFLBY_3DOBJCONT_NPC_WAIT ) );
		p_wk->seq++;
		break;
	// �E�G�C�g
	case WFLBY_3DOBJCONT_NPC_SEQ_WAIT:
		p_wk->work.npc.wait --;
		if( p_wk->work.npc.wait == 0 ){
			p_wk->seq++;
		}
		break;
	// �������͐U����������𗐐��Ō���
	case WFLBY_3DOBJCONT_NPC_SEQ_WAYSET:
		{
			u32 plno;
			u32 way;

			// ���[�UNO�ƕ������擾
			plno = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_PLID );
			way	 = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_WAY );
			
			// ����������������߂�
			p_wk->work.npc.way = WFLBY_3DOBJCONT_GetRndTbl( sc_WFLBY_3DOBJCONT_NPC_WAY, NELEMS( sc_WFLBY_3DOBJCONT_NPC_WAY ) );

			// ���̕����ɐi��ő��v���`�F�b�N
			// ���v�[������
			// ���߁@�|���U���������
			if( WFLBY_3DOBJCONT_CheckWayObjID( p_sys, p_wk, p_wk->work.npc.way, WFLBY_MAPOBJID_NPC_00+plno ) == TRUE ){

				// �������̕����������Ă���̂��H
				if( way == p_wk->work.npc.way ){
					WFLBY_3DOBJCONT_SetWkReqCmd( p_sys, p_wk, WF2DMAP_CMD_WALK, p_wk->work.npc.way );
					p_wk->seq = WFLBY_3DOBJCONT_NPC_SEQ_WAYMOVEWAIT;
				}else{
					WFLBY_3DOBJCONT_SetWkReqCmd( p_sys, p_wk, WF2DMAP_CMD_TURN, p_wk->work.npc.way );
					p_wk->seq = WFLBY_3DOBJCONT_NPC_SEQ_WAYMOVEWALKTURN;
				}
			}else{
				WFLBY_3DOBJCONT_SetWkReqCmd( p_sys, p_wk, WF2DMAP_CMD_TURN, p_wk->work.npc.way );
				p_wk->seq = WFLBY_3DOBJCONT_NPC_SEQ_WAYMOVEWAIT;
			}
			
		}
		break;
	// ��������ŐU������K�v������ꍇ
	case WFLBY_3DOBJCONT_NPC_SEQ_WAYMOVEWALKTURN:
		{
			u32 st;
			st = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_ST );
			if( st == WF2DMAP_OBJST_NONE ){
				WFLBY_3DOBJCONT_SetWkReqCmd( p_sys, p_wk, WF2DMAP_CMD_WALK, p_wk->work.npc.way );
				p_wk->seq ++;
			}
		}
		break;
	// �����E�U���������̏I���҂�
	case WFLBY_3DOBJCONT_NPC_SEQ_WAYMOVEWAIT:
		{
			u32 st;
			st = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_ST );
			if( st == WF2DMAP_OBJST_NONE ){
				p_wk->seq = WFLBY_3DOBJCONT_NPC_SEQ_WAITSET;
			}
		}
		break;
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�^��ɔ��ł���
 *
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_FlyUpMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
	switch( p_wk->seq ){
	// �p�����[�^������	���̂܂�MAIN��
	case WFLBY_3DOBJCONT_FLYUP_SEQ_INIT:
		// �O���t�B�b�N�X�V��OFF
		WFLBY_3DOBJWK_SetUpdata( p_wk->p_draw, FALSE );
		WFLBY_3DOBJWK_StartAnm( p_wk->p_draw, WFLBY_3DOBJ_ANM_ROTA );
		
		// ���`���쏉����
		WFLBY_3DOBJCONT_MOVEWK_Init( &p_wk->work.fly.y, 
				WFLBY_3DMATRIX_FLOOR_MAT, WFLBY_3DOBJCONT_FLY_Y, WFLBY_3DOBJCONT_FLY_COUNT );
		p_wk->work.fly.count = WFLBY_3DOBJCONT_ROTA_ONE_COUNT;
		p_wk->seq ++;

	// ����ɂP��]
	case WFLBY_3DOBJCONT_FLYUP_SEQ_ONEROTA:
		p_wk->work.fly.count--;
		if( p_wk->work.fly.count == 0 ){
			p_wk->seq ++;

			p_wk->work.fly.count = 0;


			// ����Ă�����
			Snd_SePlay( WFLBY_SND_PLOUT );

		}
		break;
		
	// ���상�C��
	case WFLBY_3DOBJCONT_FLYUP_SEQ_MAIN:
		{
			BOOL result;
			VecFx32 vec;

			result = WFLBY_3DOBJCONT_MOVEWK_Main( &p_wk->work.fly.y, p_wk->work.fly.count );

			p_wk->work.fly.count ++;

			// ���W��ݒ�
			WFLBY_3DOBJWK_Get3DMatrix( p_wk->p_draw, &vec );
			vec.y = WFLBY_3DOBJCONT_MOVEWK_GetNum( &p_wk->work.fly.y );
			WFLBY_3DOBJWK_Set3DMatrix( p_wk->p_draw, &vec );

			if( result == TRUE ){
				p_wk->seq ++;

				WFLBY_3DOBJWK_EndAnm( p_wk->p_draw );
				p_wk->moveend = TRUE;
			}
		}
		break;
	// �I���҂�
	case WFLBY_3DOBJCONT_FLYDOWN_SEQ_END:
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ɉ���Ă���
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_FlyDownMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
	switch( p_wk->seq ){
	// �p�����[�^������	���̂܂�MAIN��
	case WFLBY_3DOBJCONT_FLYDOWN_SEQ_INIT:
		// �O���t�B�b�N�X�V��OFF
		WFLBY_3DOBJWK_SetUpdata( p_wk->p_draw, FALSE );
		WFLBY_3DOBJWK_StartAnm( p_wk->p_draw, WFLBY_3DOBJ_ANM_ROTA );
		
		// ���`���쏉����
		WFLBY_3DOBJCONT_MOVEWK_Init( &p_wk->work.fly.y, 
				WFLBY_3DOBJCONT_FLY_Y, WFLBY_3DMATRIX_FLOOR_MAT, WFLBY_3DOBJCONT_FLY_COUNT );
		p_wk->work.fly.count = 0;
		p_wk->seq ++;

		// �����Ă��鉹
		Snd_SePlay( WFLBY_SND_PLOUT );

	// ���상�C��
	case WFLBY_3DOBJCONT_FLYDOWN_SEQ_MAIN:
		{
			BOOL result;
			VecFx32 vec;

			result = WFLBY_3DOBJCONT_MOVEWK_Main( &p_wk->work.fly.y, p_wk->work.fly.count );

			p_wk->work.fly.count ++;

			// ���W��ݒ�
			WFLBY_3DOBJWK_Get3DMatrix( p_wk->p_draw, &vec );
			vec.y = WFLBY_3DOBJCONT_MOVEWK_GetNum( &p_wk->work.fly.y );
			WFLBY_3DOBJWK_Set3DMatrix( p_wk->p_draw, &vec );

			if( result == TRUE ){
				p_wk->work.fly.count = WFLBY_3DOBJCONT_ROTA_ONE_COUNT;
				p_wk->seq ++;
			}
		}
		break;
	// ����ɂP��]
	case WFLBY_3DOBJCONT_FLYDOWN_SEQ_ONEROTA:
		p_wk->work.fly.count--;
		if( p_wk->work.fly.count == 0 ){
			p_wk->seq ++;

			// �`��X�V��ON
			WFLBY_3DOBJWK_EndAnm( p_wk->p_draw );
			WFLBY_3DOBJWK_SetUpdata( p_wk->p_draw, TRUE );
			p_wk->moveend = TRUE;
		}
		break;
	// �I���҂�
	case WFLBY_3DOBJCONT_FLYDOWN_SEQ_END:
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��������ɉ�]���Ȃ���P�O���b�h�������
 *
 *	@param	p_wk	���[�N
 *	@param	p_sys	�V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_RotaLeftMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
	WFLBY_3DOBJCONT_RotaMove( p_wk, p_sys, WF2DMAP_WAY_LEFT );
}
static void WFLBY_3DOBJCONT_RotaRightMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
	WFLBY_3DOBJCONT_RotaMove( p_wk, p_sys, WF2DMAP_WAY_RIGHT );
}

static void WFLBY_3DOBJCONT_RotaUpMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
	WFLBY_3DOBJCONT_RotaMove( p_wk, p_sys, WF2DMAP_WAY_UP );
}
static void WFLBY_3DOBJCONT_RotaDownMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
	WFLBY_3DOBJCONT_RotaMove( p_wk, p_sys, WF2DMAP_WAY_DOWN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��������ɉ�]���Ȃ��琁�����
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_RotaMove( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys, WF2DMAP_WAY way )
{
	switch( p_wk->seq ){
	// ���쏉����
	case WFLBY_3DOBJCONT_ROTA_SEQ_INIT:
		// �O���t�B�b�N�X�V��OFF
		WFLBY_3DOBJWK_SetUpdata( p_wk->p_draw, FALSE );
		WFLBY_3DOBJWK_StartAnm( p_wk->p_draw, WFLBY_3DOBJ_ANM_ROTA );
		WFLBY_3DOBJWK_SetAnmSpeed( p_wk->p_draw, WFLBY_3DOBJCONT_ROTA_ANMSPEED );

		// ���`���쏉����
		WFLBY_3DOBJCONT_MOVEWK_Init( &p_wk->work.rota.dist, 
				0, WFLBY_3DOBJCONT_ROTA_DIST, WFLBY_3DOBJCONT_ROTA_COUNT );

		// ��{�ƂȂ邻�̈ʒu���擾
		{
			VecFx32 vec;
			WFLBY_3DOBJWK_Get3DMatrix( p_wk->p_draw, &vec );
			switch( way ){
			case WF2DMAP_WAY_UP:
			case WF2DMAP_WAY_DOWN:
				p_wk->work.rota.def = vec.z;
				break;

			case WF2DMAP_WAY_LEFT:
			case WF2DMAP_WAY_RIGHT:
				p_wk->work.rota.def = vec.x;
				break;
			}
		}
		p_wk->work.rota.count = 0;
		p_wk->seq ++;
		
	// ���상�C��
	case WFLBY_3DOBJCONT_ROTA_SEQ_MAIN:
		{
			BOOL result;
			VecFx32 vec;

			result = WFLBY_3DOBJCONT_MOVEWK_Main( &p_wk->work.rota.dist, p_wk->work.rota.count );

			p_wk->work.rota.count ++;

			// ���W��ݒ�
			WFLBY_3DOBJWK_Get3DMatrix( p_wk->p_draw, &vec );
			switch( way ){
			case WF2DMAP_WAY_UP:
				vec.z = p_wk->work.rota.def - WFLBY_3DOBJCONT_MOVEWK_GetNum( &p_wk->work.rota.dist );
				break;
				
			case WF2DMAP_WAY_DOWN:
				vec.z = p_wk->work.rota.def + WFLBY_3DOBJCONT_MOVEWK_GetNum( &p_wk->work.rota.dist );
				break;

			case WF2DMAP_WAY_LEFT:
				vec.x = p_wk->work.rota.def - WFLBY_3DOBJCONT_MOVEWK_GetNum( &p_wk->work.rota.dist );
				break;
				
			case WF2DMAP_WAY_RIGHT:
				vec.x = p_wk->work.rota.def + WFLBY_3DOBJCONT_MOVEWK_GetNum( &p_wk->work.rota.dist );
			
				break;
			}
			WFLBY_3DOBJWK_Set3DMatrix( p_wk->p_draw, &vec );

			if( result == TRUE ){
				u32 retway;
				retway	= WF2DMPA_OBJToolRetWayGet( way );
				// �����ɂ���ĂP��]�̎��Ԃ�ύX����
				p_wk->work.rota.count = WFLBY_3DOBJCONT_ROTA_ONEROTA_COUNT;
				switch( retway ){
				case WF2DMAP_WAY_RIGHT:
					p_wk->work.rota.count += WFLBY_3DOBJCONT_ROTA_ONESIDEROTA;
				case WF2DMAP_WAY_UP:
					p_wk->work.rota.count += WFLBY_3DOBJCONT_ROTA_ONESIDEROTA;
				case WF2DMAP_WAY_LEFT:
					p_wk->work.rota.count += WFLBY_3DOBJCONT_ROTA_ONESIDEROTA;
				case WF2DMAP_WAY_DOWN:
					break;
				}
				p_wk->seq ++;
			}
		}
		break;
		
	case WFLBY_3DOBJCONT_ROTA_SEQ_ONEROTA:	// ����ɂP��]
		p_wk->work.rota.count--;
		if( p_wk->work.rota.count == 0 ){
			p_wk->seq ++;

			// ���W�𐁂���񂾐�ɐݒ�
			{
				WF2DMAP_POS pos;
				u32			retway;
				pos		= WF2DMAP_OBJWkMatrixGet( p_wk->p_obj );
				pos		= WF2DMAP_OBJToolWayPosGet( pos, way );
				retway	= WF2DMPA_OBJToolRetWayGet( way );
				WFLBY_3DOBJCONT_SetWkPosAndWay( p_wk, pos, retway );
			}
			
			// �`��X�V��ON
			WFLBY_3DOBJWK_EndAnm( p_wk->p_draw );
			WFLBY_3DOBJWK_SetUpdata( p_wk->p_draw, TRUE );
			p_wk->moveend = TRUE;
		}
		break;
		
	case WFLBY_3DOBJCONT_ROTA_SEQ_END:

		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�W�����v����
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_Jump( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
	switch( p_wk->seq ){
	case WFLBY_3DOBJCONT_JUMP_SEQ_INIT:
		WFLBY_3DOBJWK_SetUpdata( p_wk->p_draw, FALSE );
		WFLBY_3DOBJWK_Get3DMatrix( p_wk->p_draw, &p_wk->work.jump.def_mat );
		p_wk->work.jump.count = WFLBY_3DOBJCONT_MOVEOBJ_JUMP_COUNT;
		p_wk->seq ++;
		break;

	case WFLBY_3DOBJCONT_JUMP_SEQ_MAIN:

		// �J�E���g����
		p_wk->work.jump.count --;
		if( p_wk->work.jump.count < 0 ){
			p_wk->work.jump.count = WFLBY_3DOBJCONT_MOVEOBJ_JUMP_COUNT;
		}
		
		
		// �`�攽�f����
		{
			u32 r;
			VecFx32 matrix;
			s16 count;

			// ���W�擾
			matrix = p_wk->work.jump.def_mat;

			// �J�E���g�l�擾
			count = p_wk->work.jump.count - WFLBY_3DOBJCONT_MOVEOBJ_JUMP_WAITCOUNT;

			// count > 0�@���ƃW�����v��	����ȊO�͎��̃W�����v�܂ł̃E�G�C�g
			if( count > 0 ){

				count = count % WFLBY_3DOBJCONT_MOVEOBJ_JUMP_EFFCOUNT;

				// SIN�J�[�u�̊p�x���擾
				r = (WFLBY_3DOBJCONT_MOVEOBJ_JUMP_RMAX*count) / WFLBY_3DOBJCONT_MOVEOBJ_JUMP_EFFCOUNT;

				// �W�����v
				matrix.z += FX_Mul( Sin360(r), WFLBY_3DOBJCONT_MOVEOBJ_JUMP_ZDIS*FX32_ONE );
				matrix.y += FX_Mul( Sin360(r), WFLBY_3DOBJCONT_MOVEOBJ_JUMP_DIS*FX32_ONE );
			}

			WFLBY_3DOBJWK_Set3DMatrix( p_wk->p_draw, &matrix );

			// ������������
			WFLBY_3DOBJWK_SetWay( p_wk->p_draw, WF2DMAP_WAY_DOWN );
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�W�����v�I������
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_JumpEnd( WFLBY_3DPERSON* p_wk, WFLBY_3DOBJCONT* p_sys )
{
	VecFx32 matrix;
	
	WFLBY_3DOBJWK_SetUpdata( p_wk->p_draw, TRUE );

	// ���W�擾
	WFLBY_3DOBJWK_Get3DMatrix( p_wk->p_draw, &matrix );
	matrix.y = WFLBY_3DMATRIX_FLOOR_MAT;
	WFLBY_3DOBJWK_Set3DMatrix( p_wk->p_draw, &matrix );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�e�A���t�@�l�ύX�V�X�e��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�RD�I�u�W�F�`��V�X�e��
 *	@param	alpha		�A���t�@�l
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_ShadowAlpha_Init( WFLBY_SHADOW_ALPHA* p_wk, WFLBY_3DOBJSYS* p_sys, u32 alpha )
{
	p_wk->now		= alpha;
	p_wk->start		= alpha;
	p_wk->diff		= 0;
	p_wk->count		= 0;
	p_wk->count_max	= 0;
	p_wk->move		= FALSE;
	WFLBY_3DOBJSYS_SetShadowAlpha( p_sys, alpha );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�A���t�@�l�ύX�V�X�e��	�J�n
 *
 *	@param	p_wk		���[�N
 *	@param	alpha		�A���t�@�l
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJCONT_ShadowAlpha_Start( WFLBY_SHADOW_ALPHA* p_wk, u32 alpha )
{
	p_wk->move		= TRUE;
	p_wk->start		= p_wk->now;
	p_wk->diff		= alpha - p_wk->now;
	p_wk->count		= 0;
	p_wk->count_max	= WFLBY_3DOBJCONT_SHADOW_ALHA_FADE_COUNT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�A���t�@�l�ύX�V�X�e��	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�RD�I�u�W�F�`��V�X�e��
 *
 *	@retval	TRUE	�A���t�@�ύX����
 *	@retval	FALSE	�A���t�@�ύX��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DOBJCONT_ShadowAlpha_Main( WFLBY_SHADOW_ALPHA* p_wk, WFLBY_3DOBJSYS* p_sys )
{
	if( p_wk->move == FALSE ){
		return TRUE;
	}

	p_wk->count ++;
	if( p_wk->count >= p_wk->count_max ){
		p_wk->move = FALSE;
	}

	p_wk->now = (p_wk->count * p_wk->diff) / p_wk->count_max;
	p_wk->now += p_wk->start;

	WFLBY_3DOBJSYS_SetShadowAlpha( p_sys, p_wk->now );

	return FALSE;
}


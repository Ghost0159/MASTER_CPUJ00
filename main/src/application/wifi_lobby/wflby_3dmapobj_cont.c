//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_3dmapobj_cont.c
 *	@brief		�}�b�v�I�u�W�F�z�u�Ǘ�
 *	@author		tomoya takahashi
 *	@data		2007.11.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "wflby_3dmapobj_cont.h"
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
//-------------------------------------
///	�z�u�I�u�W�F�o�^�ő吔
//=====================================
#define WFLBY_3DMAPOBJ_CONT_MAPOBJWK_MAX		( 128 )


//-------------------------------------
///	�z�u�I�u�W�F	�A�j��
//=====================================
#define WFLBY_3DMAPOBJ_CONT_ANM_MG_NUM	(3)		// �~�j�Q�[���̐�
// �Ŕ�
enum{
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_NONE,
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_REC_0,
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_REC_1,
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_REC_2,
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_REC_3,
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_PLAY_0,
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_PLAY_1,
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_PLAY_2,
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_PLAY_3,
	WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_NUM,
};
#define WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_ONEFRAME	( 4 )

// ����
#define WFLBY_3DMAPOBJ_CONT_DOUZOU_ANM_RESTART_FRAME	( 0x2000 )
//static int WFLBY_3DMAPOBJ_CONT_DOUZOU_ANM_RESTART_FRAME	= ( 0x2000 );
#define WFLBY_3DMAPOBJ_CONT_DOUZOU_BIGFIRE_POS_OFS		( 8 )
#define WFLBY_3DMAPOBJ_CONT_DOUZOU_BIGFIRE_POS_OFS_Z	( 20 )

// �����v�ԉ�
#define WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_NUM		( 24 )	// �ԉ΃I�u�W�F�̍ő吔
enum{
	WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT,
	WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONSTART,
	WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ON,
	WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART,
	WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF,
};
#define WFLBY_3DMAPOBJ_CONT_HANABI_SPEED_END	( 0 )	// �ŏI�A�j���X�s�[�h
#define WFLBY_3DMAPOBJ_CONT_HANABI_ALPHA_DEF	( 31 )	// ��{�ԉ΃A���t�@�l
#define WFLBY_3DMAPOBJ_CONT_HANABI_COUNT_MAX	( 10 )	// �ԉ΂������Ă���J�E���g�l
#define WFLBY_3DMAPOBJ_CONT_HANABI_SCALE_DIF	( FX32_CONST( 0.180f ) )// �傫�������������銄��

//  �Ŕ�
#define WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_MAX		( 28 )
#define WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_HALF		( WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_MAX/2 )


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	���ԉ�	���[�N
//=====================================
typedef struct {
	WFLBY_3DMAPOBJ_WK* p_hanabi;
	u16	status;
	s16	count;
	fx32 start_speed;	// �J�n�A�j���X�s�[�h
} WFLBY_3DMAPOBJ_HANABIWK;


//-------------------------------------
///	�A�j���̕ύX�Ȃǂő��삷�����
//=====================================
typedef struct {
	WFLBY_3DMAPOBJ_WK* p_mgmachine[WFLBY_3DMAPOBJ_CONT_ANM_MG_NUM];
	WFLBY_3DMAPOBJ_WK* p_mgkanban[WFLBY_3DMAPOBJ_CONT_ANM_MG_NUM];
	BOOL ramp_cont_yure;

	WFLBY_3DMAPOBJ_HANABIWK		hanabi[WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_NUM];
	WFLBY_3DMAPOBJ_HANABIWK		hanabibig[WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_NUM];
	WFLBY_3DMAPOBJ_WK*			p_brhanabi[WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_NUM];
	WFLBY_3DMAPOBJ_HANABIWK		brhanabibig[WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_NUM];
	u8	hanabi_count;
	u8	hanabibig_count;
	u8	brhanabi_count;
	u8	brhanabibig_count;

	u32 kanban_count;
} WFLBY_3DMAPOBJ_ANMCONT;



//-------------------------------------
///	�V�X�e���\����
//=====================================
typedef struct _WFLBY_3DMAPOBJ_CONT{
	const WFLBY_MAPCONT*	cp_mapdata;		// �}�b�v�f�[�^
	WFLBY_3DMAPOBJ*			p_mapobjsys;	// �z�u�I�u�W�F�V�X�e��
	WFLBY_3DMAPOBJ_WK*		p_mapwkobj[ WFLBY_3DMAPOBJ_CONT_MAPOBJWK_MAX ];	// �z�u�I�u�W�F�z��
	WFLBY_3DMAPOBJ_FLOAT*	p_mapfloat[ WFLBY_3DMAPOBJ_CONT_MAPOBJFLOAT_MAX ];	// �t���[�g�I�u�W�F
	WFLBY_3DMAPOBJ_ANMCONT	anmcont;
	u8						season;			// �V�[�Y��
	u8						room;			// ����
	u8						objaddnum;		// �z�u�I�u�W�F�o�^��
	u8						floataddnum;	// �t���[�g�o�^��

}WFLBY_3DMAPOBJ_CONT;


//-------------------------------------
///	�z�u���f���@�}�b�vID�ƃ��f��ID�̑Ή��\
//=====================================
typedef struct {
	u16 mapid;
	u16 mdlid;
} WFLBY_3DMAPOBJ_MAPDATA;


//-------------------------------------
///	�t���[�g�\���f�[�^
//=====================================
typedef struct {
	u16 floatnum;
	u16 col;
} WFLBY_3DMAPOBJ_FLOAT_DATA;



//-----------------------------------------------------------------------------
/**
 *			�f�[�^�֘A
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�z�u�I�u�W�F�f�[�^
//=====================================
static const WFLBY_3DMAPOBJ_MAPDATA sc_WFLBY_3DMAPOBJ_CONT_WKDATA[] = {
	{ WFLBY_MAPOBJID_DOUZOU00, WFLBY_3DMAPOBJ_BIGBRONZE },
	{ WFLBY_MAPOBJID_DOUZOU0100, WFLBY_3DMAPOBJ_SMALLBRONZE00 },
	{ WFLBY_MAPOBJID_DOUZOU0101, WFLBY_3DMAPOBJ_SMALLBRONZE01 },
	{ WFLBY_MAPOBJID_DOUZOU0102, WFLBY_3DMAPOBJ_SMALLBRONZE02 },
	{ WFLBY_MAPOBJID_DOUZOU0102, WFLBY_3DMAPOBJ_SMALLBRONZE02 },
	{ WFLBY_MAPOBJID_DOUZOU0103, WFLBY_3DMAPOBJ_SMALLBRONZE03 },
	{ WFLBY_MAPOBJID_TREE00, WFLBY_3DMAPOBJ_TREE },
	{ WFLBY_MAPOBJID_MG00, WFLBY_3DMAPOBJ_MG_00 },
	{ WFLBY_MAPOBJID_MG01, WFLBY_3DMAPOBJ_MG_01 },
	{ WFLBY_MAPOBJID_MG02, WFLBY_3DMAPOBJ_MG_02 },
	{ WFLBY_MAPOBJID_TABLE, WFLBY_3DMAPOBJ_TABLE },
	{ WFLBY_MAPOBJID_KANBAN00, WFLBY_3DMAPOBJ_KANBAN },
	{ WFLBY_MAPOBJID_KANBAN01, WFLBY_3DMAPOBJ_KANBAN },
	{ WFLBY_MAPOBJID_KANBAN02, WFLBY_3DMAPOBJ_KANBAN },
	{ WFLBY_MAPOBJID_LAMP00, WFLBY_3DMAPOBJ_LAMP00 },
	{ WFLBY_MAPOBJID_LAMP01, WFLBY_3DMAPOBJ_LAMP01 },
	{ WFLBY_MAPOBJID_LAMP00, WFLBY_3DMAPOBJ_HANABI },
	{ WFLBY_MAPOBJID_LAMP01, WFLBY_3DMAPOBJ_HANABI },
	{ WFLBY_MAPOBJID_LAMP00, WFLBY_3DMAPOBJ_HANABIBIG },
	{ WFLBY_MAPOBJID_LAMP01, WFLBY_3DMAPOBJ_HANABIBIG },
	{ WFLBY_MAPOBJID_DOUZOU0100, WFLBY_3DMAPOBJ_BRHANABI },
	{ WFLBY_MAPOBJID_DOUZOU0101, WFLBY_3DMAPOBJ_BRHANABI },
	{ WFLBY_MAPOBJID_DOUZOU0102, WFLBY_3DMAPOBJ_BRHANABI },
	{ WFLBY_MAPOBJID_DOUZOU0102, WFLBY_3DMAPOBJ_BRHANABI },
	{ WFLBY_MAPOBJID_DOUZOU0103, WFLBY_3DMAPOBJ_BRHANABI },
	{ WFLBY_MAPOBJID_DOUZOU0100, WFLBY_3DMAPOBJ_BRHANABIBIG },
	{ WFLBY_MAPOBJID_DOUZOU0101, WFLBY_3DMAPOBJ_BRHANABIBIG },
	{ WFLBY_MAPOBJID_DOUZOU0102, WFLBY_3DMAPOBJ_BRHANABIBIG },
	{ WFLBY_MAPOBJID_DOUZOU0102, WFLBY_3DMAPOBJ_BRHANABIBIG },
	{ WFLBY_MAPOBJID_DOUZOU0103, WFLBY_3DMAPOBJ_BRHANABIBIG },
	{ WFLBY_MAPOBJID_DOUZOU00FIRE00, WFLBY_3DMAPOBJ_HANABI },
	{ WFLBY_MAPOBJID_DOUZOU00FIRE01, WFLBY_3DMAPOBJ_HANABI },
	{ WFLBY_MAPOBJID_DOUZOU00FIRE00, WFLBY_3DMAPOBJ_HANABIBIG },
	{ WFLBY_MAPOBJID_DOUZOU00FIRE01, WFLBY_3DMAPOBJ_HANABIBIG },
	{ WFLBY_MAPOBJID_ANKETO_BOARD,	WFLBY_3DMAPOBJ_ANKETO },
};

// �傫�������p�ԉ΂̃I�t�Z�b�g�ݒ�
#define WFLBY_3DMAPOBJ_CONT_DOUZOU00_FIRE_NUM	(2)
static const s32 sc_WFLBY_3DMAPOBJ_CONT_DOUZOU00_FIRE_OFFS_X[ WFLBY_3DMAPOBJ_CONT_DOUZOU00_FIRE_NUM ] = {
	// ����
	-11,

	// �E��
	12,
};
#define WFLBY_3DMAPOBJ_CONT_DOUZOU00_FIRE_OFFS_Y	(-10)


//-------------------------------------
///	�t���[�g���Ȉʒu�f�[�^
//=====================================
static const WF2DMAP_POS sc_WFLBY_3DMAPOBJ_CONT_FLOATSHEET_POS[ WFLBY_3DMAPOBJ_CONT_MAPOBJFLOAT_SHEET_MAX ] = {
	{ -2,	10 },
	{ 14,	10 },
	{ 30,	10 },
};
#define WFLBY_3DMAPOBJ_CONT_FLOAT_Y	( FX32_CONST( 40 ) )	// �t���[�g����Y���Ђ傤
#define WFLBY_3DMAPOBJ_CONT_FLOAT_X		( 0 )	// �y���W
#define WFLBY_3DMAPOBJ_CONT_FLOAT_Z		( FX32_CONST( 40 ) )	// �y���W

//-------------------------------------
///	�t���[�g�\���f�[�^
//=====================================
static const WFLBY_3DMAPOBJ_FLOAT_DATA sc_WFLBY_3DMAPOBJJ_CONT_FLOAT_DATA[ WFLBY_FLOAT_MAX ] = {
	{ WFLBY_3DMAPOBJ_FLOAT_00, WFLBY_3DMAPOBJ_FLOAT_COL_00 },
	{ WFLBY_3DMAPOBJ_FLOAT_00, WFLBY_3DMAPOBJ_FLOAT_COL_00 },
	{ WFLBY_3DMAPOBJ_FLOAT_01, WFLBY_3DMAPOBJ_FLOAT_COL_00 },

	{ WFLBY_3DMAPOBJ_FLOAT_00, WFLBY_3DMAPOBJ_FLOAT_COL_01 },
	{ WFLBY_3DMAPOBJ_FLOAT_00, WFLBY_3DMAPOBJ_FLOAT_COL_01 },
	{ WFLBY_3DMAPOBJ_FLOAT_01, WFLBY_3DMAPOBJ_FLOAT_COL_01 },

	{ WFLBY_3DMAPOBJ_FLOAT_00, WFLBY_3DMAPOBJ_FLOAT_COL_02 },
	{ WFLBY_3DMAPOBJ_FLOAT_00, WFLBY_3DMAPOBJ_FLOAT_COL_02 },
	{ WFLBY_3DMAPOBJ_FLOAT_01, WFLBY_3DMAPOBJ_FLOAT_COL_02 },
};


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DOBJCONT_MAPOBJWK_CheckPosGrid( const WFLBY_3DMAPOBJ_WK* cp_wk, u8 gridx, u8 gridy );
static WFLBY_3DMAPOBJ_WK* WFLBY_3DOBJCONT_MAPOBJWK_GetGridWk( WFLBY_3DMAPOBJ_CONT* p_sys, u8 gridx, u8 gridy );

static void WFLBY_3DMAPOBJ_LAMP_YURE_SetCallBack( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk );
static void WFLBY_3DMAPOBJ_LAMP_NONE_SetCallBack( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk );

static void WFLBY_3DMAPOBJ_LAMP_DrawOff_SetCallBack( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk );


static void WFLBY_3DMAPOBJ_DouzouAnm_PlayNormal( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_3DMAPOBJ_WK* p_wk );
static void WFLBY_3DMAPOBJ_DouzouAnm_PlaySpecial( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_3DMAPOBJ_WK* p_wk );
static void WFLBY_3DMAPOBJ_DouzouAnm_PlayAnm( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_3DMAPOBJ_WK* p_wk );


// ���ԉ΃��[�N
static void WFLBY_3DMAPOBJ_HANABIWK_Init( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ_WK* p_hanabi );
static void WFLBY_3DMAPOBJ_HANABIWK_Main( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys );
static void WFLBY_3DMAPOBJ_HANABIWK_On( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys, fx32 speed );
static void WFLBY_3DMAPOBJ_HANABIWK_Off( WFLBY_3DMAPOBJ_HANABIWK* p_wk );
static void WFLBY_3DMAPOBJ_HANABIWK_OnDraw( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys );
static void WFLBY_3DMAPOBJ_HANABIWK_OffDraw( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys );
static void WFLBY_3DMAPOBJ_HANABIWK_OffMoveStart( WFLBY_3DMAPOBJ_HANABIWK* p_wk );
static BOOL WFLBY_3DMAPOBJ_HANABIWK_OffMove( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys );

// �傫���ԉΗp
static void WFLBY_3DMAPOBJ_HANABIWK_OnBig( WFLBY_3DMAPOBJ_HANABIWK* p_wk, fx32 speed );
static void WFLBY_3DMAPOBJ_HANABIWK_MainBig( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys );
static void WFLBY_3DMAPOBJ_HANABIWK_OnBigDraw( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys );

// �����傫���ԉΗp�i�J�n��OnBig���g�p�j
static void WFLBY_3DMAPOBJ_HANABIWK_MainBrBig( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys );
static void WFLBY_3DMAPOBJ_HANABIWK_OnBrBigDraw( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys );

// �傫�������p�ԉ΍��W�ݒ�֐�
static void WFLBY_3DMAPOBJ_DOUZOU00_HANABI_SetPos( WFLBY_3DMAPOBJ_WK* p_hanabi, u32 mapid );

//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�z�u�I�u�W�F�N�g�Ǘ��V�X�e��	������
 *
 *	@param	season	�V�[�Y��	
 *	@param	room	�����^�C�v
 *	@param	cp_map	�}�b�v�f�[�^
 *	@param	heapID	�q�[�vID
 *	@param	gheapID	�O���t�B�b�N�q�[�vID
 *
 *	@return	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
WFLBY_3DMAPOBJ_CONT* WFLBY_3DMAPOBJCONT_Init( WFLBY_SEASON_TYPE season, WFLBY_ROOM_TYPE room, const WFLBY_MAPCONT* cp_map, u32 heapID, u32 gheapID )
{
	WFLBY_3DMAPOBJ_CONT* p_sys;
	int i;

	p_sys = sys_AllocMemory( heapID, sizeof(WFLBY_3DMAPOBJ_CONT) );
	memset( p_sys, 0, sizeof(WFLBY_3DMAPOBJ_CONT) );

	// �}�b�v�f�[�^�ۑ�
	p_sys->cp_mapdata = cp_map;

	// �V�[�Y�������f�[�^�ۑ�
	p_sys->season	= season;
	p_sys->room		= room;

	// �z�u�I�u�W�F�V�X�e���쐬
	p_sys->p_mapobjsys = WFLBY_3DMAPOBJ_Init( WFLBY_FLOAT_MAX, WFLBY_3DMAPOBJ_CONT_MAPOBJWK_MAX, heapID, gheapID );

	// �z�u�I�u�W�F���\�[�X�ǂݍ���
	WFLBY_3DMAPOBJ_ResLoad( p_sys->p_mapobjsys, room, season, heapID, gheapID );

	// �z�u�I�u�W�F�N�g��o�^
	{
		int i;
		u32 count;
		u16 gridx, gridy;

		for( i=0; i<NELEMS(sc_WFLBY_3DMAPOBJ_CONT_WKDATA); i++ ){
			count = 0;
			while( WFLBY_MAPCONT_SarchObjID( p_sys->cp_mapdata, sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mapid, &gridx, &gridy, count ) == TRUE ){


				// �O���b�h�ʒu�ɓo�^
				p_sys->p_mapwkobj[ p_sys->objaddnum ] = WFLBY_3DMAPOBJ_WK_Add( 
						p_sys->p_mapobjsys, sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mdlid, gridx, gridy );

				// �e���f���̏���������
				switch( sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mdlid ){
				// �~�j�Q�[��
				case WFLBY_3DMAPOBJ_MG_00:
				case WFLBY_3DMAPOBJ_MG_01:
				case WFLBY_3DMAPOBJ_MG_02:
					WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
							p_sys->p_mapwkobj[ p_sys->objaddnum ], 
							WFLBY_3DMAPOBJ_MG_ANM_ALL, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	

					// �~�j�Q�[���^�C�v�̂��ƂɐU�蕪����
					p_sys->anmcont.p_mgmachine[ sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mdlid-WFLBY_3DMAPOBJ_MG_00 ] = p_sys->p_mapwkobj[ p_sys->objaddnum ];
					break;

				case WFLBY_3DMAPOBJ_BIGBRONZE:
					WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
							p_sys->p_mapwkobj[ p_sys->objaddnum ], 
							WFLBY_3DMAPOBJ_BIGBRONZEZ_ANM_ALL, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	
					WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
							p_sys->p_mapwkobj[ p_sys->objaddnum ], 
							WFLBY_3DMAPOBJ_BIGBRONZEZ_ANM_ALL01, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	
					WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
							p_sys->p_mapwkobj[ p_sys->objaddnum ], 
							WFLBY_3DMAPOBJ_BIGBRONZEZ_ANM_ALL02, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	
					break;
				
				// �Ŕ�
				case WFLBY_3DMAPOBJ_KANBAN:
					// �Ƃ肠�������쒆�A�j����
					WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
							p_sys->p_mapwkobj[ p_sys->objaddnum ], 
							WFLBY_3DMAPOBJ_KANBAN_ANM_LAMP, WFLBY_3DMAPOBJ_WK_ANM_NOAUTO );	
					// �~�j�Q�[���^�C�v�̂��ƂɐU�蕪����
					p_sys->anmcont.p_mgkanban[ sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mapid-WFLBY_MAPOBJID_KANBAN00 ] = p_sys->p_mapwkobj[ p_sys->objaddnum ];
					break;

				case WFLBY_3DMAPOBJ_LAMP00:
				case WFLBY_3DMAPOBJ_LAMP01:
					WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
							p_sys->p_mapwkobj[ p_sys->objaddnum ], 
							WFLBY_3DMAPOBJ_LAMP_ANM_ON, WFLBY_3DMAPOBJ_WK_ANM_NOAUTO );	
					break;

	
				// �ԉΗ�
				case WFLBY_3DMAPOBJ_HANABI:
					WFLBY_3DMAPOBJ_HANABIWK_Init( 
							&p_sys->anmcont.hanabi[ p_sys->anmcont.hanabi_count ],
							p_sys->p_mapwkobj[ p_sys->objaddnum ]	);
					p_sys->anmcont.hanabi_count ++;
					GF_ASSERT( p_sys->anmcont.hanabi_count <= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_NUM );
					WFLBY_3DMAPOBJ_WK_SetDraw( p_sys->p_mapwkobj[ p_sys->objaddnum ], FALSE );

					// �傫�������p�̉ԉ΂͈ʒu�𒲐�����
					if( (sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mapid == WFLBY_MAPOBJID_DOUZOU00FIRE00) ||
						(sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mapid == WFLBY_MAPOBJID_DOUZOU00FIRE01) ){

						WFLBY_3DMAPOBJ_DOUZOU00_HANABI_SetPos( p_sys->p_mapwkobj[ p_sys->objaddnum ], sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mapid );
							
					}
					
					break;

				case WFLBY_3DMAPOBJ_HANABIBIG:
					WFLBY_3DMAPOBJ_HANABIWK_Init( 
							&p_sys->anmcont.hanabibig[ p_sys->anmcont.hanabibig_count ],
							p_sys->p_mapwkobj[ p_sys->objaddnum ]	);
					p_sys->anmcont.hanabibig_count ++;
					GF_ASSERT( p_sys->anmcont.hanabi_count <= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_NUM );
					WFLBY_3DMAPOBJ_WK_SetDraw( p_sys->p_mapwkobj[ p_sys->objaddnum ], FALSE );

					// �傫�������p�̉ԉ΂͈ʒu�𒲐�����
					if( (sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mapid == WFLBY_MAPOBJID_DOUZOU00FIRE00) ||
						(sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mapid == WFLBY_MAPOBJID_DOUZOU00FIRE01) ){

						WFLBY_3DMAPOBJ_DOUZOU00_HANABI_SetPos( p_sys->p_mapwkobj[ p_sys->objaddnum ], sc_WFLBY_3DMAPOBJ_CONT_WKDATA[i].mapid );
							
					}
					break;
					
				case WFLBY_3DMAPOBJ_BRHANABI:
					p_sys->anmcont.p_brhanabi[ p_sys->anmcont.brhanabi_count ] = p_sys->p_mapwkobj[ p_sys->objaddnum ];
					p_sys->anmcont.brhanabi_count ++;
					GF_ASSERT( p_sys->anmcont.brhanabi_count <= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_NUM );
					WFLBY_3DMAPOBJ_WK_SetDraw( p_sys->p_mapwkobj[ p_sys->objaddnum ], FALSE );
					break;
					
				case WFLBY_3DMAPOBJ_BRHANABIBIG:
					WFLBY_3DMAPOBJ_HANABIWK_Init( 
							&p_sys->anmcont.brhanabibig[ p_sys->anmcont.brhanabibig_count ],
							p_sys->p_mapwkobj[ p_sys->objaddnum ]	);
					p_sys->anmcont.brhanabibig_count ++;
					GF_ASSERT( p_sys->anmcont.brhanabibig_count <= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_NUM );
					WFLBY_3DMAPOBJ_WK_SetDraw( p_sys->p_mapwkobj[ p_sys->objaddnum ], FALSE );

					// �ʒu�𒲐�
					{
						WF2DMAP_POS pos;
						pos = WFLBY_3DMAPOBJ_WK_GetPos( p_sys->p_mapwkobj[ p_sys->objaddnum ] );
						pos.x += WFLBY_3DMAPOBJ_CONT_DOUZOU_BIGFIRE_POS_OFS;
						pos.y += WFLBY_3DMAPOBJ_CONT_DOUZOU_BIGFIRE_POS_OFS_Z;
						WFLBY_3DMAPOBJ_WK_SetPos( p_sys->p_mapwkobj[ p_sys->objaddnum ], pos );
					}
					break;

				}

				p_sys->objaddnum ++;

				// ���Ԗڂ�OBJID��T����
				count ++;

			}
		}
	}


	//  �t���[�g�̍쐬
	{	
		VecFx32 matrix;
		matrix.x	= WFLBY_3DMAPOBJ_CONT_FLOAT_X;
		matrix.z	= WFLBY_3DMAPOBJ_CONT_FLOAT_Z;
		matrix.y	= WFLBY_3DMATRIX_FLOOR_MAT;
		for( i=0; i<WFLBY_FLOAT_MAX; i++ ){
			p_sys->p_mapfloat[ i ] = WFLBY_3DMAPOBJ_FLOAT_Add( 
					p_sys->p_mapobjsys, 
					sc_WFLBY_3DMAPOBJJ_CONT_FLOAT_DATA[i].floatnum, sc_WFLBY_3DMAPOBJJ_CONT_FLOAT_DATA[i].col, &matrix );

			// ��\��
			WFLBY_3DMAPOBJ_FLOAT_SetDraw( p_sys->p_mapfloat[ i ], FALSE );
		}
		p_sys->floataddnum = WFLBY_FLOAT_MAX;
	}

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�I�u�W�F�Ǘ��V�X�e���j��
 *
 *	@param	p_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_Exit( WFLBY_3DMAPOBJ_CONT* p_sys )
{
	// �S�t���[�g�j��
	{
		int i;

		for( i=0; i<p_sys->floataddnum; i++ ){
			WFLBY_3DMAPOBJ_FLOAT_Del( p_sys->p_mapfloat[ i ] );
		}
	}
	
	// �S�I�u�W�F��j��
	{
		int i;
		for( i=0; i<p_sys->objaddnum; i++ ){
			WFLBY_3DMAPOBJ_WK_Del( p_sys->p_mapobjsys, p_sys->p_mapwkobj[i] );
		}
	}

	// �z�u�I�u�W�F���\�[�X�j��
	WFLBY_3DMAPOBJ_ResRelease( p_sys->p_mapobjsys );	

	// �z�u�I�u�W�F�V�X�e���j��
	WFLBY_3DMAPOBJ_Exit( p_sys->p_mapobjsys );

	// �������g��j��
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C������
 *
 *	@param	p_sys		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_Main( WFLBY_3DMAPOBJ_CONT* p_sys )
{
	int i;

#ifndef WFLBY_3DMAPOBJ_CONT_DOUZOU_ANM_RESTART_FRAME
	if( sys.trg & PAD_BUTTON_L ){
		WFLBY_3DMAPOBJ_CONT_DOUZOU_ANM_RESTART_FRAME += FX32_HALF;
		OS_TPrintf( "frame 0x%x\n", WFLBY_3DMAPOBJ_CONT_DOUZOU_ANM_RESTART_FRAME );
	}
	if( sys.trg & PAD_BUTTON_R ){
		WFLBY_3DMAPOBJ_CONT_DOUZOU_ANM_RESTART_FRAME -= FX32_HALF;
		OS_TPrintf( "frame 0x%x\n", WFLBY_3DMAPOBJ_CONT_DOUZOU_ANM_RESTART_FRAME );
	}
#endif

	// ���ԉ΃��[�N���C��
	for( i=0; i<p_sys->anmcont.hanabi_count; i++ ){
		WFLBY_3DMAPOBJ_HANABIWK_Main( &p_sys->anmcont.hanabi[i], p_sys->p_mapobjsys );
	}

	// ���ł��ԉ΃��[�N���C��
	for( i=0; i<p_sys->anmcont.hanabibig_count; i++ ){
		WFLBY_3DMAPOBJ_HANABIWK_MainBig( &p_sys->anmcont.hanabibig[i], p_sys->p_mapobjsys );
	}

	// �����ł��ԉ΃��[�N���C��
	for( i=0; i<p_sys->anmcont.brhanabibig_count; i++ ){
		WFLBY_3DMAPOBJ_HANABIWK_MainBrBig( &p_sys->anmcont.brhanabibig[i], p_sys->p_mapobjsys );
	}
	
	// �z�u�I�u�W�F�V�X�e�����C��
	WFLBY_3DMAPOBJ_Main( p_sys->p_mapobjsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�揈��
 *
 *	@param	p_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_Draw( WFLBY_3DMAPOBJ_CONT* p_sys )
{
	// �z�u�I�u�W�F�V�X�e���`��
	WFLBY_3DMAPOBJ_Draw( p_sys->p_mapobjsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank����
 *
 *	@param	p_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_VBlank( WFLBY_3DMAPOBJ_CONT* p_sys )
{
	// �z�u�I�u�W�F�V�X�e��VBlank
	WFLBY_3DMAPOBJ_VBlank( p_sys->p_mapobjsys );
}




//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�̈ʒu���擾
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	idx			�t���[�g�i���o�[
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_FLOAT_GetPos( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx, VecFx32* p_vec )
{
	GF_ASSERT( cp_sys->floataddnum > idx );
	WFLBY_3DMAPOBJ_FLOAT_GetPos( cp_sys->p_mapfloat[ idx ],  p_vec );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�̈ʒu��ݒ�
 *
 *	@param	p_sys		�V�X�e��
 *	@param	idx			�t���[�g�i���o�[
 *	@param	x			x�ʒu
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_FLOAT_SetPos( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, fx32 x )
{
	VecFx32 matrix;
	
	GF_ASSERT( p_sys->floataddnum > idx );

	matrix.x = x;
	matrix.z = WFLBY_3DMAPOBJ_CONT_FLOAT_Z;
	matrix.y = WFLBY_3DMATRIX_FLOOR_MAT;
	WFLBY_3DMAPOBJ_FLOAT_SetPos( p_sys->p_mapfloat[ idx ], &matrix );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�I�t�Z�b�g���W��ݒ肵�܂��B	�G�t�F�N�g�ȂǂɎg�p���Ă�������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	idx			�t���[�g�C���f�b�N�X
 *	@param	cp_vec		���W
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_FLOAT_SetOfsPos( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, const VecFx32* cp_vec )
{
	GF_ASSERT( p_sys->floataddnum > idx );
	WFLBY_3DMAPOBJ_FLOAT_SetOfsPos( p_sys->p_mapfloat[ idx ], cp_vec );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�̍��Ȃ̈ʒu���擾
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	idx			�t���[�g�i���o�[
 *	@param	no			���ȃi���o�[
 *	@param	p_vec		�ʒu�i�[��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_FLOAT_GetSheetPos( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx, u32 no, VecFx32* p_vec )
{
	GF_ASSERT( cp_sys->floataddnum > idx );
	GF_ASSERT( no < WFLBY_3DMAPOBJ_CONT_MAPOBJFLOAT_SHEET_MAX );
	
	WFLBY_3DMAPOBJ_FLOAT_GetPos( cp_sys->p_mapfloat[ idx ], p_vec );
	p_vec->x += (sc_WFLBY_3DMAPOBJ_CONT_FLOATSHEET_POS[ no ].x * WFLBY_3DMATRIX_GRID_SIZ);
	p_vec->z += (sc_WFLBY_3DMAPOBJ_CONT_FLOATSHEET_POS[ no ].y * WFLBY_3DMATRIX_GRID_SIZ);
	p_vec->y = WFLBY_3DMAPOBJ_CONT_FLOAT_Y;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g	Sound���A�j��	�J�n
 *
 *	@param	p_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE		�A�j���ݒ�ł���
 *	@retval	FALSE		�A�j���ݒ�ł��Ȃ�����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DMAPOBJCONT_FLOAT_SetAnmSound( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx )
{
	GF_ASSERT( p_sys->floataddnum > idx );
	return WFLBY_3DMAPOBJ_FLOAT_SetAnmSound( p_sys->p_mapfloat[ idx ] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g	Body�A�j��	�J�n
 *
 *	@param	p_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE		�A�j���ݒ�ł���
 *	@retval	FALSE		�A�j���ݒ�ł��Ȃ�����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DMAPOBJCONT_FLOAT_SetAnmBody( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx )
{
	GF_ASSERT( p_sys->floataddnum > idx );
	return WFLBY_3DMAPOBJ_FLOAT_SetAnmBody( p_sys->p_mapfloat[ idx ] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�̕`��ݒ�
 *
 *	@param	p_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 *	@param	flag		�t���O
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_FLOAT_SetDraw( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, BOOL flag )
{
	GF_ASSERT( p_sys->floataddnum > idx );
	WFLBY_3DMAPOBJ_FLOAT_SetDraw( p_sys->p_mapfloat[ idx ], flag );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�̕`��擾
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE	�`�撆
 *	@retval	FALSE	�`��O
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DMAPOBJCONT_FLOAT_GetDraw( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx )
{
	GF_ASSERT( cp_sys->floataddnum > idx );
	return WFLBY_3DMAPOBJ_FLOAT_GetDraw( cp_sys->p_mapfloat[ idx ] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�p�x��ݒ肷��
 *
 *	@param	p_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 *	@param	x			������]�p�x
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_FLOAT_SetRot( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, u16 x )
{
	GF_ASSERT( p_sys->floataddnum > idx );
	WFLBY_3DMAPOBJ_FLOAT_SetRotaX( p_sys->p_mapfloat[ idx ], x );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�����A�j�������`�F�b�N
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE	�����A�j��	�Đ���
 *	@retval	FALSE	�����A�j��	�Đ����ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DMAPOBJCONT_FLOAT_CheckAnmSound( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx )
{
	GF_ASSERT( cp_sys->floataddnum > idx );
	return WFLBY_3DMAPOBJ_FLOAT_CheckAnmSound( cp_sys->p_mapfloat[ idx ] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g���̃A�j��	�Đ����`�F�b�N
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE	���̃A�j��	�Đ���
 *	@retval	FALSE	���̃A�j��	��~��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DMAPOBJCONT_FLOAT_CheckAnmBody( const WFLBY_3DMAPOBJ_CONT* cp_sys, u32 idx )
{
	GF_ASSERT( cp_sys->floataddnum > idx );
	return WFLBY_3DMAPOBJ_FLOAT_CheckAnmBody( cp_sys->p_mapfloat[ idx ] );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�|�[���\��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_OnPoll( WFLBY_3DMAPOBJ_CONT* p_sys )
{
	WFLBY_3DMAPOBJ_MAP_OnPoll( p_sys->p_mapobjsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�|�[���\���A�j�����Ȃ�����
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_OffPoll( WFLBY_3DMAPOBJ_CONT* p_sys )
{
	WFLBY_3DMAPOBJ_MAP_OffPoll( p_sys->p_mapobjsys );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�����A�j���J�n
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	gridx	�O���b�h��
 *	@param	gridy	�O���b�h��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_StartDouzouAnm( WFLBY_3DMAPOBJ_CONT* p_sys, u8 gridx, u8 gridy )
{
	int i;
	BOOL result[2];
	u32  mdltype;

	
	// �����̃��f���̈ʒu�𒲂ׁA�ʒu�̈�v���铺�����A�j��������
	for( i=0; i<p_sys->objaddnum; i++ ){
		result[0] = WFLBY_3DOBJCONT_MAPOBJWK_CheckPosGrid( p_sys->p_mapwkobj[i], gridx-1, gridy-1 );
		result[1] = WFLBY_3DOBJCONT_MAPOBJWK_CheckPosGrid( p_sys->p_mapwkobj[i], gridx, gridy-1 );
		if( (result[0] == TRUE) || (result[1] == TRUE) ){
			// �������������`�F�b�N
			mdltype = WFLBY_3DMAPOBJ_WK_GetType( p_sys->p_mapwkobj[i] );
			switch( mdltype ){
			case WFLBY_3DMAPOBJ_SMALLBRONZE00:	// ���ꂼ��S�p�^�[��
			case WFLBY_3DMAPOBJ_SMALLBRONZE01:	// 
			case WFLBY_3DMAPOBJ_SMALLBRONZE02:	// 
			case WFLBY_3DMAPOBJ_SMALLBRONZE03:	// 
				if( p_sys->room == WFLBY_ROOM_SPECIAL ){
					WFLBY_3DMAPOBJ_DouzouAnm_PlaySpecial( p_sys, p_sys->p_mapwkobj[i] );
				}else{
					WFLBY_3DMAPOBJ_DouzouAnm_PlayNormal( p_sys, p_sys->p_mapwkobj[i] );
				}
				break;
				
			default:
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ԉΐݒ�
 *
 *	@param	p_sys		�V�X�e��	
 *	@param	flag		�t���O
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_SetDouzouFire( WFLBY_3DMAPOBJ_CONT* p_sys, BOOL flag )
{
	int i;
	u32  mdltype;

	
	// �����̃��f���̈ʒu�𒲂ׁA�ʒu�̈�v���铺�����A�j��������
	for( i=0; i<p_sys->objaddnum; i++ ){
		// �������������`�F�b�N
		mdltype = WFLBY_3DMAPOBJ_WK_GetType( p_sys->p_mapwkobj[i] );
		switch( mdltype ){
		// �����̑���
		case WFLBY_3DMAPOBJ_SMALLBRONZE00:	// ���ꂼ��S�p�^�[��
		case WFLBY_3DMAPOBJ_SMALLBRONZE01:	// 
		case WFLBY_3DMAPOBJ_SMALLBRONZE02:	// 
		case WFLBY_3DMAPOBJ_SMALLBRONZE03:	// 
			if( flag==TRUE ){
				WFLBY_3DMAPOBJ_WK_SetLightFlag( p_sys->p_mapobjsys, p_sys->p_mapwkobj[i], 1<<WFLBY_DRAW_LIGHT_ROOM );
			}
			break;


		// �ԉ΂̑���
		case WFLBY_3DMAPOBJ_BRHANABI:	
			//  �ԉ΍Đ�
			if( flag==TRUE ){

				WFLBY_3DMAPOBJ_WK_SetAnmSpeed( p_sys->p_mapwkobj[i], FX32_HALF );

				WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[i], 
						WFLBY_3DMAPOBJ_BRHANABI_ANM_NICA, WFLBY_3DMAPOBJ_WK_ANM_RANDSTART );	

				WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[i], 
						WFLBY_3DMAPOBJ_BRHANABI_ANM_NITA, WFLBY_3DMAPOBJ_WK_ANM_RANDSTART );	

				WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[i], 
						WFLBY_3DMAPOBJ_BRHANABI_ANM_NITP, WFLBY_3DMAPOBJ_WK_ANM_RANDSTART );	

				WFLBY_3DMAPOBJ_WK_SetDraw( p_sys->p_mapwkobj[i], TRUE );

			}else{
				WFLBY_3DMAPOBJ_WK_DelAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[i], 
						WFLBY_3DMAPOBJ_BRHANABI_ANM_NICA );	

				WFLBY_3DMAPOBJ_WK_DelAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[i], 
						WFLBY_3DMAPOBJ_BRHANABI_ANM_NITA );	

				WFLBY_3DMAPOBJ_WK_DelAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[i], 
						WFLBY_3DMAPOBJ_BRHANABI_ANM_NITP );	

				WFLBY_3DMAPOBJ_WK_SetDraw( p_sys->p_mapwkobj[i], FALSE );
			}
			break;
			
		default:
			break;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����傫���ԉ΍Đ�
 *
 *	@param	p_sys	���[�N
 *	@param	speed	�Đ��X�s�[�h
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_SetDouzouBigFire( WFLBY_3DMAPOBJ_CONT* p_sys, fx32 speed )
{
	int i;
	u32  mdltype;

	
	// �����̃��f���̈ʒu�𒲂ׁA�ʒu�̈�v���铺�����A�j��������
	for( i=0; i<p_sys->anmcont.brhanabibig_count; i++ ){

		WFLBY_3DMAPOBJ_HANABIWK_OnBig( &p_sys->anmcont.brhanabibig[i], speed );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃��C�g��OFF�ɂ���
 *
 *	@param	p_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_OffDouzouLight( WFLBY_3DMAPOBJ_CONT* p_sys )
{
	int i;
	u32  mdltype;

	
	// �����̃��f���̈ʒu�𒲂ׁA�ʒu�̈�v���铺�����A�j��������
	for( i=0; i<p_sys->objaddnum; i++ ){
		// �������������`�F�b�N
		mdltype = WFLBY_3DMAPOBJ_WK_GetType( p_sys->p_mapwkobj[i] );
		switch( mdltype ){
		// �����̑���
		case WFLBY_3DMAPOBJ_SMALLBRONZE00:	// ���ꂼ��S�p�^�[��
		case WFLBY_3DMAPOBJ_SMALLBRONZE01:	// 
		case WFLBY_3DMAPOBJ_SMALLBRONZE02:	// 
		case WFLBY_3DMAPOBJ_SMALLBRONZE03:	// 
			WFLBY_3DMAPOBJ_WK_SetLightFlag( p_sys->p_mapobjsys, p_sys->p_mapwkobj[i], 1<<WFLBY_DRAW_LIGHT_ROOM );
			break;
			
		default:
			break;
		}
	}
}


// �����C�g
//----------------------------------------------------------------------------
/**
 *	@brief	�����C�g�ݒ�
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	anm		�A�j���f�[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_SetFloorLight( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_3DMAPOBJ_CONT_LAMP_ANM anm )
{
	int i;
	u32  mdltype;

	
	// �����v�̃��f���̈ʒu�𒲂ׁA�ʒu�̈�v���郉���v���A�j��������
	for( i=0; i<p_sys->objaddnum; i++ ){
		// �����������v���`�F�b�N
		mdltype = WFLBY_3DMAPOBJ_WK_GetType( p_sys->p_mapwkobj[i] );
		switch( mdltype ){
		case WFLBY_3DMAPOBJ_LAMP00:
		case WFLBY_3DMAPOBJ_LAMP01:

			switch( anm ){
			// �����o�Ă��Ȃ����
			case WFLBY_3DMAPOBJ_CONT_LAMP_NONE:	
				WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[ i ], 
						WFLBY_3DMAPOBJ_LAMP_ANM_ON, WFLBY_3DMAPOBJ_WK_ANM_NOAUTO );	
				p_sys->anmcont.ramp_cont_yure = FALSE;	// ���Ȃ��߂��Ȃ�
				break;
			// NONE��������o����Ԃɂ���		���̌���A�j���ɂȂ�܂�
			case WFLBY_3DMAPOBJ_CONT_LAMP_ON:	
				WFLBY_3DMAPOBJ_WK_AddAnmAnmCallBack( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[ i ], 
						WFLBY_3DMAPOBJ_LAMP_ANM_ON, WFLBY_3DMAPOBJ_WK_ANM_NOLOOP,
						WFLBY_3DMAPOBJ_LAMP_YURE_SetCallBack );	
				p_sys->anmcont.ramp_cont_yure = TRUE;	// ����
				break;
			// �����o�Ă����Ԃ�����������B	���̌�NONE��ԂɂȂ�܂��B
			case WFLBY_3DMAPOBJ_CONT_LAMP_OFF:	
				WFLBY_3DMAPOBJ_WK_AddAnmAnmCallBack( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[ i ], 
						WFLBY_3DMAPOBJ_LAMP_ANM_ON, WFLBY_3DMAPOBJ_WK_ANM_BACKNOLOOP,
						WFLBY_3DMAPOBJ_LAMP_NONE_SetCallBack );	

				// ���A�j���͔j��
				WFLBY_3DMAPOBJ_WK_DelAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[ i ], 
						WFLBY_3DMAPOBJ_LAMP_ANM_YURE );	
				p_sys->anmcont.ramp_cont_yure = FALSE;	// ���Ȃ�
				break;
			// ���A�j��
			case WFLBY_3DMAPOBJ_CONT_LAMP_YURE:	
				// ���A�j���ݒ�
				WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[ i ], 
						WFLBY_3DMAPOBJ_LAMP_ANM_YURE, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	

				// ���A�j���͔j��
				WFLBY_3DMAPOBJ_WK_DelAnm( p_sys->p_mapobjsys, 
						p_sys->p_mapwkobj[ i ], 
						WFLBY_3DMAPOBJ_LAMP_ANM_ON );	
				p_sys->anmcont.ramp_cont_yure = TRUE;	// ����
				break;
			default:
				break;
			}
			break;
			
		default:
			break;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���A�j�������`�F�b�N
 *
 *	@param	cp_sys	�Ǘ��V�X�e��
 *
 *	@retval	TRUE	���A�j����
 *	@retval	FALSE	���A�j�����ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DMAPOBJCONT_MAP_CheckFloorLightYure( const WFLBY_3DMAPOBJ_CONT* cp_sys )
{
	return cp_sys->anmcont.ramp_cont_yure;	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����C�g�ԉΐݒ�
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	flag	���邩������
 *	@param	speed	�X�s�[�h
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFire( WFLBY_3DMAPOBJ_CONT* p_sys, BOOL  flag, fx32 speed )
{
	int i;

	
	// �����̃��f���̈ʒu�𒲂ׁA�ʒu�̈�v���铺�����A�j��������
	for( i=0; i<p_sys->anmcont.hanabi_count; i++ ){
		
		WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFireIdx( p_sys, i, flag, speed );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉ�	�����ɔ���
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	idx			�C���f�b�N�X
 *	@param	flag		�t���O
 *	@param	speed		�X�s�[�h
 *
 *	@retval	TRUE	�ݒ�ł���
 *	@retval	FALSE	���̃C���f�b�N�X�̉ԉ΂͂Ȃ��i����ȍ~�̉ԉ΂͂Ȃ��j
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFireIdx( WFLBY_3DMAPOBJ_CONT* p_sys, u32 idx, BOOL  flag,  fx32 speed )
{

	int i;
	WFLBY_3DMAPOBJ_HANABIWK* p_hanabi;

	if( idx < p_sys->anmcont.hanabi_count ){

		p_hanabi = &p_sys->anmcont.hanabi[idx];

		if( flag ){
			WFLBY_3DMAPOBJ_HANABIWK_On( p_hanabi, p_sys->p_mapobjsys, speed );
		}else{
			WFLBY_3DMAPOBJ_HANABIWK_Off( p_hanabi );
		}

		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉ�	�ł������̔���
 *
 *	@param	p_sys	�V�X�e��
 *	@param	speed	�Đ��X�s�[�h
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_SetFloorLightBigFire( WFLBY_3DMAPOBJ_CONT* p_sys, fx32 speed )
{
	int i;
	u32  mdltype;

	
	// �����̃��f���̈ʒu�𒲂ׁA�ʒu�̈�v���铺�����A�j��������
	for( i=0; i<p_sys->anmcont.hanabibig_count; i++ ){
		WFLBY_3DMAPOBJ_HANABIWK_OnBig( &p_sys->anmcont.hanabibig[i], speed );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ŔA�j���̐ݒ�
 *
 *	@param	p_sys		�V�X�e��
 *	@param	game		�Q�[���^�C�v
 *	@param	num			�ڑ��l��
 *	@param	recruit		���N���[�g
 *	@param	play		�V�ђ���
 *	@param	light		��������
 */
//-----------------------------------------------------------------------------
void WFLBY_3DMAPOBJCONT_MAP_SetMGAnm( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_GAMETYPE game, u32 num, BOOL recruit, BOOL play, BOOL light )
{
	BOOL result;
	WFLBY_3DMAPOBJ_WK* p_kanban;
	WFLBY_3DMAPOBJ_WK* p_machine;
	u32 idx;

	
	// �Q�[���^�C�v����~�j�Q�[���C���f�b�N�X�����߂�
	idx = game - WFLBY_GAME_BALLSLOW;

	p_machine	= p_sys->anmcont.p_mgmachine[ idx ];
	p_kanban	= p_sys->anmcont.p_mgkanban[ idx ];


	// ��W���V�ђ�
	if( (play == TRUE) || (recruit == TRUE) ){
		// �~�j�Q�[���@�𓮍삳����
		if( WFLBY_3DMAPOBJ_WK_GetAnmFlag( p_machine, WFLBY_3DMAPOBJ_MG_ANM_MOVE ) == FALSE ){
			// �Ƃ肠�������쒆�A�j����
			WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, 
					p_machine, 
					WFLBY_3DMAPOBJ_MG_ANM_MOVE, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	
		}

		// �Ŕ�
		// �l���̃t���[������ݒ�
		if( play == TRUE ){
			WFLBY_3DMAPOBJ_WK_SetAnmFrame( p_sys->p_mapobjsys, p_kanban, 
					WFLBY_3DMAPOBJ_KANBAN_ANM_LAMP, 
					FX32_CONST( (WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_PLAY_0+num-1)*WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_ONEFRAME ) );
		}else{
			// ��W���͓_�ł�����
			p_sys->anmcont.kanban_count = (p_sys->anmcont.kanban_count + 1) % WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_MAX;
			if( p_sys->anmcont.kanban_count < WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_HALF ){
				WFLBY_3DMAPOBJ_WK_SetAnmFrame( p_sys->p_mapobjsys, p_kanban, 
						WFLBY_3DMAPOBJ_KANBAN_ANM_LAMP, 
						FX32_CONST( (WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_REC_0+num-1)*WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_ONEFRAME ) );
			}else{
				WFLBY_3DMAPOBJ_WK_SetAnmFrame( p_sys->p_mapobjsys, p_kanban, 
						WFLBY_3DMAPOBJ_KANBAN_ANM_LAMP, 
						FX32_CONST( WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_NONE*WFLBY_3DMAPOBJ_CONT_KANBAN_ANM_ONEFRAME ) );
			}
		}
	}else{

		// ���ꂼ��̃A�j����ݒ肷��
		// �d��OFF��
		if( light ){
			WFLBY_3DMAPOBJ_WK_DelAnmAll( p_sys->p_mapobjsys, p_machine );
			WFLBY_3DMAPOBJ_WK_DelAnmAll( p_sys->p_mapobjsys, p_kanban );
			// ���C�g�t���O��OFF�ɂ���
			WFLBY_3DMAPOBJ_WK_SetLightFlag( p_sys->p_mapobjsys, p_machine, 1<<WFLBY_DRAW_LIGHT_ROOM );
		}else{
			// �~�j�Q�[���̓I�u�W�F���Ƃ߂�
			WFLBY_3DMAPOBJ_WK_DelAnm( p_sys->p_mapobjsys, 
					p_machine, WFLBY_3DMAPOBJ_MG_ANM_MOVE );	

			// �Ŕ̓t���[���O�ɂ���
			WFLBY_3DMAPOBJ_WK_SetAnmFrame( p_sys->p_mapobjsys, p_kanban, 
						WFLBY_3DMAPOBJ_KANBAN_ANM_LAMP, 0 );
		}
	}
}









//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�O���b�h�ƃI�u�W�F�̈ʒu����v���邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	gridx		�O���b�h��
 *	@param	gridy		�O���b�h��
 *
 *	@retval	TRUE	���W�ƃO���b�h����v����
 *	@retval	FALSE	��v���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DOBJCONT_MAPOBJWK_CheckPosGrid( const WFLBY_3DMAPOBJ_WK* cp_wk, u8 gridx, u8 gridy )
{
	WF2DMAP_POS pos;
	WF2DMAP_POS chk_pos;

	// �O���b�h�����W�ɕϊ�
	chk_pos.x = WF2DMAP_GRID2POS(gridx);
	chk_pos.y = WF2DMAP_GRID2POS(gridy);
	
	pos = WFLBY_3DMAPOBJ_WK_GetPos( cp_wk );
	if( (pos.x == chk_pos.x) &&
		(pos.y == chk_pos.y) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���b�h�ʒu�ɂ���z�u�I�u�W�F���擾����
 *
 *	@param	p_sys		�V�X�e��
 *	@param	gridx		�O���b�h
 *	@param	gridy 
 *
 *	@retval	NULL	���Ȃ�
 *	@retval	���̃O���b�h�ɂ��郏�[�N
 */
//-----------------------------------------------------------------------------
static WFLBY_3DMAPOBJ_WK* WFLBY_3DOBJCONT_MAPOBJWK_GetGridWk( WFLBY_3DMAPOBJ_CONT* p_sys, u8 gridx, u8 gridy )
{
	int i;
	BOOL result;

	
	// �����̃��f���̈ʒu�𒲂ׁA�ʒu�̈�v���铺�����A�j��������
	for( i=0; i<p_sys->objaddnum; i++ ){
		result = WFLBY_3DOBJCONT_MAPOBJWK_CheckPosGrid( p_sys->p_mapwkobj[i], gridx, gridy );
		if( result == TRUE ){
			return p_sys->p_mapwkobj[i];
		}
	}
	return NULL;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�����v�@���ݒ�R�[���o�b�N
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_LAMP_YURE_SetCallBack( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk )
{
	u32 mdltype;
	
	// �I�u�W�F�^�C�v�������ƃ����v���`�F�b�N
	mdltype = WFLBY_3DMAPOBJ_WK_GetType( p_wk );
	GF_ASSERT( (mdltype == WFLBY_3DMAPOBJ_LAMP00) || (mdltype == WFLBY_3DMAPOBJ_LAMP01) );
	if( (mdltype == WFLBY_3DMAPOBJ_LAMP00) || (mdltype == WFLBY_3DMAPOBJ_LAMP01) ){
		WFLBY_3DMAPOBJ_WK_AddAnm( p_sys, 
				p_wk, 
				WFLBY_3DMAPOBJ_LAMP_ANM_YURE, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����v�Ȃ��A�j���ݒ�
 *
 *	@param	p_sys	�V�X�e��
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_LAMP_NONE_SetCallBack( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk )
{
	u32 mdltype;
	
	// �I�u�W�F�^�C�v�������ƃ����v���`�F�b�N
	mdltype = WFLBY_3DMAPOBJ_WK_GetType( p_wk );
	GF_ASSERT( (mdltype == WFLBY_3DMAPOBJ_LAMP00) || (mdltype == WFLBY_3DMAPOBJ_LAMP01) );
	if( (mdltype == WFLBY_3DMAPOBJ_LAMP00) || (mdltype == WFLBY_3DMAPOBJ_LAMP01) ){
		WFLBY_3DMAPOBJ_WK_AddAnm( p_sys, p_wk, 
				WFLBY_3DMAPOBJ_LAMP_ANM_ON, WFLBY_3DMAPOBJ_WK_ANM_NOAUTO );	
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\�����n�e�e����R�[���o�b�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_LAMP_DrawOff_SetCallBack( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk )
{
	WFLBY_3DMAPOBJ_WK_SetDraw( p_wk, FALSE );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�����A�j���̍Đ�	�ʏ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_DouzouAnm_PlayNormal( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_3DMAPOBJ_WK* p_wk )
{
	fx32 anm_frame;
	BOOL anm_start;
	int i;

	// �A�j���J�n
	anm_start = FALSE;	

	// �A�j�����J�n�����`�F�b�N
	if( WFLBY_3DMAPOBJ_WK_CheckAnm( p_sys->p_mapobjsys, p_wk, WFLBY_3DMAPOBJ_SMALLBRONZE_ANM_TOUCH ) == FALSE ){
		anm_start = TRUE;	// ��΂ɃA�j���J�n
	}else{
		// �t���[�������K��̈ʒu�܂ōs���ĂȂ���΃A�j���J�n�����Ȃ�
		anm_frame = WFLBY_3DMAPOBJ_WK_GetAnmFrame( p_wk, WFLBY_3DMAPOBJ_SMALLBRONZE_ANM_TOUCH );
		if( anm_frame > WFLBY_3DMAPOBJ_CONT_DOUZOU_ANM_RESTART_FRAME ){
			anm_start = TRUE;
		}
	}
	
	// anm_start��TRUE�Ȃ�A�j���Đ�
	if( anm_start == TRUE ){
		
		// �����A�j���Đ�
		WFLBY_3DMAPOBJ_DouzouAnm_PlayAnm( p_sys, p_wk );	


		// �������쉹�ݒ�
		Snd_SePlay( WFLBY_SND_DOUZOU );
	}
}
// ���ꕔ���p
static void WFLBY_3DMAPOBJ_DouzouAnm_PlaySpecial( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_3DMAPOBJ_WK* p_wk )
{
	BOOL anm_start;

	// �A�j���J�n
	anm_start = FALSE;	

	// �A�j�����J�n�����`�F�b�N
	if( WFLBY_3DMAPOBJ_WK_CheckAnm( p_sys->p_mapobjsys, p_wk, WFLBY_3DMAPOBJ_SMALLBRONZE_ANM_TOUCH ) == FALSE ){
		anm_start = TRUE;	// ��΂ɃA�j���J�n
	}
	
	// anm_start��TRUE�Ȃ�A�j���Đ�
	if( anm_start == TRUE ){

		// �����A�j���Đ�
		WFLBY_3DMAPOBJ_DouzouAnm_PlayAnm( p_sys, p_wk );	

		// �������쉹�ݒ�
		Snd_SePlay( WFLBY_SND_DOUZOU2 );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ۂɓ����A�j�����Đ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		�`�惏�[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_DouzouAnm_PlayAnm( WFLBY_3DMAPOBJ_CONT* p_sys, WFLBY_3DMAPOBJ_WK* p_wk )
{
	int i;

	for( i=WFLBY_3DMAPOBJ_SMALLBRONZE_ANM_TOUCH; i<WFLBY_3DMAPOBJ_SMALLBRONZE_ANM_NUM; i++ ){

		WFLBY_3DMAPOBJ_WK_AddAnm( p_sys->p_mapobjsys, p_wk,
				i, WFLBY_3DMAPOBJ_WK_ANM_NOLOOP );
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_hanabi	�ԉ΃��[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_Init( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ_WK* p_hanabi )
{
	p_wk->p_hanabi	= p_hanabi;
	p_wk->status	= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	���C��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_Main( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys )
{
	switch( p_wk->status ){
	// �\���J�n�҂�
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT:
		break;

	// �\���J�n
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONSTART:
		WFLBY_3DMAPOBJ_HANABIWK_OnDraw( p_wk, p_mapobjsys );

		p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ON;
		break;
		
	// �\����
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ON:
		break;

	// �\��OFF�J�n
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART:
		WFLBY_3DMAPOBJ_HANABIWK_OffMoveStart( p_wk );
		p_wk->status		= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF;
		break;
		
	// �\��OFF
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF:
		if( WFLBY_3DMAPOBJ_HANABIWK_OffMove( p_wk, p_mapobjsys ) == TRUE ){

			// �I��
			WFLBY_3DMAPOBJ_HANABIWK_OffDraw( p_wk, p_mapobjsys );

			// �\���J�n�҂�������
			p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT;
		}
		break;

	default:
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	�ԉΕ\���J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_mapobjsys	�z�u�I�u�W�F�`��V�X�e��
 *	@param	speed		�A�j���X�s�[�h
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_On( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys, fx32 speed )
{
	// �����\�����Ȃ牽�����Ȃ�
	if( (p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONSTART) ||
		(p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ON) ){
		return ;
	}

	// �j�����̏ꍇ�A�����I�ɔj������
	if( (p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART) ||
		(p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF) ){
		WFLBY_3DMAPOBJ_HANABIWK_OffDraw( p_wk, p_mapobjsys );
	}

	// �\���J�n
	p_wk->status		= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONSTART;
	p_wk->start_speed	= speed;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	�ԉΕ\���I��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_Off( WFLBY_3DMAPOBJ_HANABIWK* p_wk )
{
	// �j�����Ȃ牽�����Ȃ�
	if( (p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART) ||
		(p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF) ){
		return ;
	}

	// �`�悵�ĂȂ��Ȃ牽�����Ȃ�
	if( (p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONSTART) ||
		(p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT) ){
		p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT;
		return ;
	}
	
	// �j��
	p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	���ۂɉԉ΂�\������
 *	
 *	@param	p_wk		���[�N
 *	@param	p_mapobjsys	�z�u�I�u�W�F�`��Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_OnDraw( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys )
{
	WFLBY_3DMAPOBJ_WK_SetAnmSpeed( p_wk->p_hanabi, p_wk->start_speed );
	
	WFLBY_3DMAPOBJ_WK_AddAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_HANABI_ANM_NICA, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	
	WFLBY_3DMAPOBJ_WK_AddAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_HANABI_ANM_NITA, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	
	WFLBY_3DMAPOBJ_WK_AddAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_HANABI_ANM_NITP, WFLBY_3DMAPOBJ_WK_ANM_LOOP );	

	WFLBY_3DMAPOBJ_WK_SetDraw( p_wk->p_hanabi, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	���ۂɉԉ΂�\������߂�
 *	
 *	@param	p_wk		���[�N
 *	@param	p_mapobjsys	�z�u�I�u�W�F�`��Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_OffDraw( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys )
{
	WFLBY_3DMAPOBJ_WK_DelAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_HANABI_ANM_NICA );	
	WFLBY_3DMAPOBJ_WK_DelAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_HANABI_ANM_NITA );	
	WFLBY_3DMAPOBJ_WK_DelAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_HANABI_ANM_NITP );	

	WFLBY_3DMAPOBJ_WK_SetDraw( p_wk->p_hanabi, FALSE );

	WFLBY_3DMAPOBJ_WK_ResetAlpha( p_wk->p_hanabi );

	WFLBY_3DMAPOBJ_WK_SetScale( p_wk->p_hanabi, FX32_ONE, FX32_ONE, FX32_ONE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	�ԉΏI������	������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_OffMoveStart( WFLBY_3DMAPOBJ_HANABIWK* p_wk )
{
	p_wk->count = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	�ԉΏI������	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_mapobjsys	�z�u�I�u�W�F�Ǘ��V�X�e��
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DMAPOBJ_HANABIWK_OffMove( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys )
{
	if( p_wk->count < WFLBY_3DMAPOBJ_CONT_HANABI_COUNT_MAX ){
		p_wk->count ++;

		// �Đ��t���[���ݒ�
		{
			fx32 dif_speed;
			fx32 now_speed;
			dif_speed = p_wk->start_speed - WFLBY_3DMAPOBJ_CONT_HANABI_SPEED_END;
			now_speed = FX_Mul( p_wk->count<<FX32_SHIFT, dif_speed );
			now_speed = FX_Div( now_speed, WFLBY_3DMAPOBJ_CONT_HANABI_COUNT_MAX<<FX32_SHIFT );
			now_speed += p_wk->start_speed;
			WFLBY_3DMAPOBJ_WK_SetAnmSpeed( p_wk->p_hanabi, now_speed );
		}

		// �A���t�@�ݒ�
		{
			u32 alpha;

			alpha = (p_wk->count * WFLBY_3DMAPOBJ_CONT_HANABI_ALPHA_DEF) / WFLBY_3DMAPOBJ_CONT_HANABI_COUNT_MAX;
			alpha = WFLBY_3DMAPOBJ_CONT_HANABI_ALPHA_DEF - alpha;
			WFLBY_3DMAPOBJ_WK_SetAlpha( p_wk->p_hanabi, alpha, WFLBY_3DMAPOBJ_CONT_HANABI_ALPHA_DEF );
		}

		// �X�P�[��
		{
			fx32 scale;

			scale = FX_Mul( p_wk->count<<FX32_SHIFT, WFLBY_3DMAPOBJ_CONT_HANABI_SCALE_DIF );
			scale = FX_Div( scale, WFLBY_3DMAPOBJ_CONT_HANABI_COUNT_MAX<<FX32_SHIFT );
			scale = FX32_ONE + scale;
			
			WFLBY_3DMAPOBJ_WK_SetScale( p_wk->p_hanabi, scale, scale, scale );
		}

		return FALSE;
	}

	return TRUE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	�ԉ΂�\���J�n	�ł����ԉ�
 *
 *	@param	p_wk			���[�N
 *	@param	speed			�A�j���X�s�[�h
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_OnBig( WFLBY_3DMAPOBJ_HANABIWK* p_wk, fx32 speed )
{
	// �����\�����Ȃ牽�����Ȃ�
	if( (p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONSTART) ||
		(p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ON) ){
		return ;
	}

	// �j�����̏ꍇ�͂��̂܂ܐV�����o�����Ⴄ
/*	if( (p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART) ||
		(p_wk->status == WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF) ){
		return ;
	}//*/

	// �\���J�n
	p_wk->status		= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONSTART;
	p_wk->start_speed	= speed;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_mapobjsys �z�u�I�u�W�F�`��Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_MainBig( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys )
{
	switch( p_wk->status ){
	// �\���J�n�҂�
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT:
		break;

	// �\���J�n
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONSTART:
		WFLBY_3DMAPOBJ_HANABIWK_OnBigDraw( p_wk, p_mapobjsys );

		p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ON;
		break;
		
	// �\����
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ON:
		// �A�j���̏I���҂�
		if( WFLBY_3DMAPOBJ_WK_CheckAnm( p_mapobjsys, p_wk->p_hanabi, 0 ) == FALSE ){

			// �J�n�҂���
			p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART;
		}
		break;

	// �\��OFF�J�n
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART:
		WFLBY_3DMAPOBJ_HANABIWK_OffMoveStart( p_wk );
		p_wk->status		= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF;
		break;
		
	// �\��OFF
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF:
		if( WFLBY_3DMAPOBJ_HANABIWK_OffMove( p_wk, p_mapobjsys ) == TRUE ){

			// �\��OFF�I��
			WFLBY_3DMAPOBJ_WK_SetDraw( p_wk->p_hanabi, FALSE );

			WFLBY_3DMAPOBJ_WK_ResetAlpha( p_wk->p_hanabi );

			WFLBY_3DMAPOBJ_WK_SetScale( p_wk->p_hanabi, FX32_ONE, FX32_ONE, FX32_ONE );

			// �\���J�n�҂�������
			p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT;
		}
		break;

	default:
		GF_ASSERT( 0 );
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	���ۂɉԉ΂�\���J�n	�ł����ԉ�
 *	
 *	@param	p_wk		���[�N
 *	@param	p_mapobjsys	�z�u�I�u�W�F�`��Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_OnBigDraw( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys )
{
	WFLBY_3DMAPOBJ_WK_SetAnmSpeed( p_wk->p_hanabi, p_wk->start_speed );

	WFLBY_3DMAPOBJ_WK_AddAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_HANABIBIG_ANM_NICA, WFLBY_3DMAPOBJ_WK_ANM_NOLOOP );	
	WFLBY_3DMAPOBJ_WK_AddAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_HANABIBIG_ANM_NITP, WFLBY_3DMAPOBJ_WK_ANM_NOLOOP );	

	WFLBY_3DMAPOBJ_WK_SetDraw( p_wk->p_hanabi, TRUE );
}



//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	���C��		�����ł��ԉΗp
 *
 *	@param	p_wk		���[�N
 *	@param	p_mapobjsys �z�u�I�u�W�F�`��Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_MainBrBig( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys )
{
	switch( p_wk->status ){
	// �\���J�n�҂�
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT:
		break;

	// �\���J�n
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONSTART:
		WFLBY_3DMAPOBJ_HANABIWK_OnBrBigDraw( p_wk, p_mapobjsys );

		p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ON;
		break;
		
	// �\����
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ON:
		// �A�j���̏I���҂�
		if( WFLBY_3DMAPOBJ_WK_CheckAnm( p_mapobjsys, p_wk->p_hanabi, 0 ) == FALSE ){

			// �J�n�҂���
			p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART;
		}
		break;

	// �\��OFF�J�n
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFFSTART:
		WFLBY_3DMAPOBJ_HANABIWK_OffMoveStart( p_wk );
		p_wk->status		= WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF;
		break;
		
	// �\��OFF
	case WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_OFF:
		if( WFLBY_3DMAPOBJ_HANABIWK_OffMove( p_wk, p_mapobjsys ) == TRUE ){

			// �\��OFF�I��
			WFLBY_3DMAPOBJ_WK_SetDraw( p_wk->p_hanabi, FALSE );

			WFLBY_3DMAPOBJ_WK_ResetAlpha( p_wk->p_hanabi );

			WFLBY_3DMAPOBJ_WK_SetScale( p_wk->p_hanabi, FX32_ONE, FX32_ONE, FX32_ONE );

			// �\���J�n�҂�������
			p_wk->status = WFLBY_3DMAPOBJ_CONT_HANABI_OBJ_ST_ONWAIT;
		}
		break;

	default:
		GF_ASSERT( 0 );
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԉΊǗ��V�X�e��	���ۂɉԉ΂�\���J�n	�����ł����ԉ�
 *	
 *	@param	p_wk		���[�N
 *	@param	p_mapobjsys	�z�u�I�u�W�F�`��Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_HANABIWK_OnBrBigDraw( WFLBY_3DMAPOBJ_HANABIWK* p_wk, WFLBY_3DMAPOBJ* p_mapobjsys )
{
	WFLBY_3DMAPOBJ_WK_SetAnmSpeed( p_wk->p_hanabi, p_wk->start_speed );
	
	//  �ԉ΍Đ�
	WFLBY_3DMAPOBJ_WK_AddAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_BRHANABIBIG_ANM_NICA, WFLBY_3DMAPOBJ_WK_ANM_NOLOOP );	

	WFLBY_3DMAPOBJ_WK_AddAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_BRHANABIBIG_ANM_NITA, WFLBY_3DMAPOBJ_WK_ANM_NOLOOP );	

	WFLBY_3DMAPOBJ_WK_AddAnm( p_mapobjsys, 
			p_wk->p_hanabi, 
			WFLBY_3DMAPOBJ_BRHANABIBIG_ANM_NITP, WFLBY_3DMAPOBJ_WK_ANM_NOLOOP );	

	WFLBY_3DMAPOBJ_WK_SetDraw( p_wk->p_hanabi, TRUE );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�傫�������p�ԉ΍��W��ݒ肷��
 *
 *	@param	p_hanabi	�ԉ΃��[�N
 *	@param	mapid		�z�u�I�u�W�F�Ǘ�ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMAPOBJ_DOUZOU00_HANABI_SetPos( WFLBY_3DMAPOBJ_WK* p_hanabi, u32 mapid )
{
	WF2DMAP_POS pos;
	u32 fire_offs_idx;

	GF_ASSERT( (mapid == WFLBY_MAPOBJID_DOUZOU00FIRE00) || (mapid == WFLBY_MAPOBJID_DOUZOU00FIRE01) );

	fire_offs_idx = mapid - WFLBY_MAPOBJID_DOUZOU00FIRE00;
	pos = WFLBY_3DMAPOBJ_WK_GetPos( p_hanabi );
	pos.x += sc_WFLBY_3DMAPOBJ_CONT_DOUZOU00_FIRE_OFFS_X[fire_offs_idx];
	pos.y += WFLBY_3DMAPOBJ_CONT_DOUZOU00_FIRE_OFFS_Y;
	WFLBY_3DMAPOBJ_WK_SetPos( p_hanabi, pos );
}

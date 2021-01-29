//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_float.c
 *	@brief		�t���[�g�Ǘ��V�X�e��	(SYSTEM�̃t���[�g�f�[�^��`��ɔ��f������V�X�e��)
 *	@author		tomoya takahashi
 *	@data		2008.01.29
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include  "poketool/monsno.h"

#include "wflby_snd.h"

#include "wflby_float.h"
#include "wflby_3dmatrix.h"

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
//#define WFLBY_DEBUG_FLOAT_ANM_SELECT	// L�{�^���ŃA�j����I������
#endif

// L�{�^���ŃA�j����I������
#ifdef WFLBY_DEBUG_FLOAT_ANM_SELECT
static u32 s_WFLBY_DEBUG_FLOAT_ANM_SELECT_COUNT = WFLBY_FLOAT_ANM_SOUND; 
#endif



//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�}�b�v�̈ړ�����
//=====================================
#define WFLBY_FLOAT_CONT_START_X_OFS	( 6 )	// �}�b�v�̑傫���{���̈ʒu����t���[�g���n�܂�
#define WFLBY_FLOAT_CONT_END_X			( -8 )	// �I��������W
#define WFLBY_FLOAT_CONT_LOCK_X_OFS		( WF2DMAP_GRID_SIZ )	// �\�񃍃b�N��������w�ʒu����̃I�t�Z�b�g�ʒu



//-------------------------------------
///	�t���[�g���A�j�����
//=====================================
enum {
	WFLBY_FLOAT_SHAKE_ANM_NORMAL,
	WFLBY_FLOAT_SHAKE_ANM_NORMAL_BIG,
	WFLBY_FLOAT_SHAKE_ANM_NUM,
} ;

//-------------------------------------
///	WFLBY_FLOAT_SHAKE_ANM_NORMAL�萔
//=====================================
#define WFLBY_FLOAT_SHAKE_NRM_COUNT_MAX		( 8 )
#define WFLBY_FLOAT_SHAKE_NRM_COUNT_MOVEY	( 2 )
#define WFLBY_FLOAT_SHAKE_NRM_COUNT_ROT		( RotKeyR( 5 ) )
#define WFLBY_FLOAT_SHAKE_NRM_COUNT_ROOP	( 4 )

//-------------------------------------
///	WFLBY_FLOAT_SHAKE_ANM_NORMALBIG�萔
//=====================================
#define WFLBY_FLOAT_SHAKE_NRMBIG_COUNT_MAX		( 16 )
#define WFLBY_FLOAT_SHAKE_NRMBIG_COUNT_MOVEY	( 4 )
#define WFLBY_FLOAT_SHAKE_NRMBIG_COUNT_ROT		( RotKeyR( 10 ) )
#define WFLBY_FLOAT_SHAKE_NRMBIG_COUNT_ROOP		( 2 )



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	���A�j���f�[�^
//=====================================
typedef struct {
	u16 anm;		// BOOL�^
	u16	anm_type;	// WFLBY_FLOAT_SHAKE_ANM�`
	union{
		// X�����
		struct{
			s16 count;
			s16 count_max;
			u16 rot_num;
			s16 move_y;
			s16 roop;
		} normal;
	} data;
} WFLBY_FLOAT_SHAKE;


//-------------------------------------
///	�t���[�g�Ǘ����[�N
//=====================================
typedef struct _WFLBY_FLOAT_CONT{
	WFLBY_ROOMWK* p_rmwk;
	WFLBY_SYSTEM* p_system;
	WFLBY_3DMAPOBJ_CONT* p_mapobjcont;
	WFLBY_MAPCONT* p_mapcont;

	// �A�j���Đ����
	u8	anm_play[WFLBY_FLOAT_MAX][WFLBY_FLOAT_ANM_NUM];		// 3*9	27
	u8	pad;												// 28

	// �t���[�g�h��A�j��
	WFLBY_FLOAT_SHAKE	shake[ WFLBY_FLOAT_MAX ];

	// �w���
	WF2DMAP_POS		station_pos[ WFLBY_FLOAT_STATION_NUM ];

	// �����̍Đ������A�j���f�[�^
	u16	float_idx;
	u16 anm_no;

	// �t���[�g���싗��
	fx32	float_move_start;
	fx32	float_move_dis;
} WFLBY_FLOAT_CONT;


//-------------------------------------
///	�t���[�gdata
//	�ւ␔���̃t���[�g�f�[�^
//=====================================
typedef struct {
	u16	monsno_sml;
	u16	monsno_big;
	u16	monsno_sml_snd_pv;
	u16	monsno_sml_snd_ex;
	u16	monsno_big_snd_pv;
	u16	monsno_big_snd_ex;
} WFLBY_FLOAT_DATA;



//-----------------------------------------------------------------------------
/**
 *		�t���[�g�f�[�^
 */
//-----------------------------------------------------------------------------
static const WFLBY_FLOAT_DATA sc_WFLBY_FLOAT_DATA[WFLBY_ROOM_NUM] = {
	// ��
	{ 
		MONSNO_ATYAMO, MONSNO_RIZAADON, 
		WFLBY_SND_RIDE_BUTTON_ATYAMO_PV, 
		WFLBY_SND_RIDE_BUTTON_ATYAMO_EX,
		WFLBY_SND_RIDE_BUTTON_RIZAADON_PV, 
		WFLBY_SND_RIDE_BUTTON_RIZAADON_EX,
	},

	// ��
	{ 
		MONSNO_POTTYAMA, MONSNO_GYARADOSU,
		WFLBY_SND_RIDE_BUTTON_POTTYAMA_PV,
		WFLBY_SND_RIDE_BUTTON_POTTYAMA_EX,
		WFLBY_SND_RIDE_BUTTON_GYARADOSU_PV,
		WFLBY_SND_RIDE_BUTTON_GYARADOSU_EX,
	},

	// �d�C
	{ 
		MONSNO_PIKATYUU, MONSNO_DEKAKOIRU,
		WFLBY_SND_RIDE_BUTTON_PIKATYUU_PV,
		WFLBY_SND_RIDE_BUTTON_PIKATYUU_EX,
		WFLBY_SND_RIDE_BUTTON_DEKAKOIRU_PV,
		WFLBY_SND_RIDE_BUTTON_DEKAKOIRU_EX,
	},

	// ��
	{ 
		MONSNO_HUSIGIDANE, MONSNO_HAEZIGOKU,
		WFLBY_SND_RIDE_BUTTON_HUSIGIDANE_PV,
		WFLBY_SND_RIDE_BUTTON_HUSIGIDANE_EX,
		WFLBY_SND_RIDE_BUTTON_HAEZIGOKU_PV,
		WFLBY_SND_RIDE_BUTTON_HAEZIGOKU_EX,
	},

	// ����
	{ 
		MONSNO_MYUU, MONSNO_MYUU,
		WFLBY_SND_RIDE_BUTTON_MYUU_PV,
		WFLBY_SND_RIDE_BUTTON_MYUU_EX,
		WFLBY_SND_RIDE_BUTTON_MYUU_PV_2,
		WFLBY_SND_RIDE_BUTTON_MYUU_EX,
	},
};

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WFLBY_FLOAT_CONT_SetMatrix( WFLBY_FLOAT_CONT* p_sys, u32 idx );
static void WFLBY_FLOAT_CONT_SetReserveLock( WFLBY_FLOAT_CONT* p_sys, u32 idx );
static void WFLBY_FLOAT_CONT_SetAnm( WFLBY_FLOAT_CONT* p_sys, u32 idx, BOOL my_float_on, BOOL my_float, u32 my_float_offs );

static void WFLBY_FLOAT_CONT_PlaySe( BOOL my_float_on, BOOL my_float, BOOL my_offs, u32 se_idx );
static void WFLBY_FLOAT_CONT_PlayMonsVoice( BOOL my_float_on, BOOL my_float, BOOL my_offs, u32 monsno );

static BOOL WFLBY_FLOAT_CONT_InitShake( WFLBY_FLOAT_SHAKE* p_wk, u32 anm_type, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx );
static BOOL WFLBY_FLOAT_CONT_MainShake( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx );
static void WFLBY_FLOAT_CONT_InitShakeNormal( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx );
static void WFLBY_FLOAT_CONT_InitShakeNormalBig( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx );
static BOOL WFLBY_FLOAT_CONT_MainShakeNormal( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx );

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�Ǘ��V�X�e��	������
 *
 *	@param	p_rmwk		�������[�N
 *	@param	heapID		�q�[�v�h�c
 *
 *	@return	���[�N
 */
//-----------------------------------------------------------------------------
WFLBY_FLOAT_CONT* WFLBY_FLOAT_CONT_Init( WFLBY_ROOMWK* p_rmwk, u32 heapID )
{
	WFLBY_FLOAT_CONT* p_sys;
	int i;
	u16 gridx, gridy;
	static const u8 sc_STATION_OBJID[ WFLBY_FLOAT_STATION_NUM ] = {
		WFLBY_MAPOBJID_FLOAT02_IN,
		WFLBY_MAPOBJID_FLOAT01_IN,
		WFLBY_MAPOBJID_FLOAT00_IN,
	};

	p_sys = sys_AllocMemory( heapID, sizeof(WFLBY_FLOAT_CONT) );
	memset( p_sys, 0, sizeof(WFLBY_FLOAT_CONT) );

	p_sys->p_rmwk		= p_rmwk;
	p_sys->p_system		= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_sys->p_mapobjcont	= WFLBY_ROOM_GetMapObjCont( p_rmwk );
	p_sys->p_mapcont	= WFLBY_ROOM_GetMapCont( p_rmwk );

	// �t���[�g��������擾
	p_sys->float_move_start	= WFLBY_MAPCONT_GridXGet( p_sys->p_mapcont ) + WFLBY_FLOAT_CONT_START_X_OFS;
	p_sys->float_move_dis	= WFLBY_FLOAT_CONT_END_X - p_sys->float_move_start;
	// �O���b�h���W�Ȃ̂ŁA�R�c���W�ɂ���
	p_sys->float_move_start = WF2DMAP_GRID2POS( p_sys->float_move_start ) * WFLBY_3DMATRIX_GRID_SIZ;
	p_sys->float_move_dis	= WF2DMAP_GRID2POS( p_sys->float_move_dis ) * WFLBY_3DMATRIX_GRID_SIZ;


	// �w�̈ʒu���擾����
	for( i=0; i<WFLBY_FLOAT_STATION_NUM; i++ ){
		WFLBY_MAPCONT_SarchObjID( p_sys->p_mapcont, sc_STATION_OBJID[i], 
				&gridx, &gridy, 0 );
		p_sys->station_pos[i].x	= WF2DMAP_GRID2POS(gridx);
		p_sys->station_pos[i].y	= WF2DMAP_GRID2POS(gridy);
	}

	// �t���[�g��[�Ɉړ�������
	for( i=0; i<WFLBY_FLOAT_MAX; i++ ){
		WFLBY_3DMAPOBJCONT_FLOAT_SetPos( p_sys->p_mapobjcont, i, p_sys->float_move_start );

		// ���ݍ��W���t���[�g�ɐݒ肵�A�\�񃍃b�N�󋵂�ݒ肷��
		if( WFLBY_SYSTEM_GetFloatMove( p_sys->p_system, i ) == WFLBY_FLOAT_STATE_MOVE ){
			// ���W
			WFLBY_FLOAT_CONT_SetMatrix( p_sys, i );

			// �\�񃍃b�N�ݒ�
			WFLBY_FLOAT_CONT_SetReserveLock( p_sys, i );
		}
	}
	

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�Ǘ��V�X�e��	�j��
 *
 *	@param	p_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_FLOAT_CONT_Exit( WFLBY_FLOAT_CONT* p_sys )
{
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�Ǘ��V�X�e��	���C��
 *
 *	@param	p_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_FLOAT_CONT_Main( WFLBY_FLOAT_CONT* p_sys )
{
	int i;
	BOOL result;
	u32 my_float_idx;
	u32 my_float_offs;
	u32 myidx;
	BOOL my_float_on;
	BOOL my_float;

#ifdef WFLBY_DEBUG_FLOAT_ANM_SELECT
	if( sys.trg & PAD_BUTTON_L ){
		s_WFLBY_DEBUG_FLOAT_ANM_SELECT_COUNT = (s_WFLBY_DEBUG_FLOAT_ANM_SELECT_COUNT+1) % WFLBY_FLOAT_ANM_NUM;
		OS_TPrintf( "FLOAT_ANM change %d\n", s_WFLBY_DEBUG_FLOAT_ANM_SELECT_COUNT );
	}
#endif
	
	// �����̍��ȏ��擾
	myidx			= WFLBY_SYSTEM_GetMyIdx( p_sys->p_system );
	my_float_idx	= WFLBY_SYSTEM_GetFloatPlIdxReserve( p_sys->p_system, myidx );

	if( my_float_idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
		my_float_on = FALSE;		// �t���[�g�ɂ͏���ĂȂ�
	}else{
		// ����������Ă���t���[�g���`�F�b�N
		WFLBY_SYSTEM_GetFloatIdxOfs( my_float_idx, &my_float_idx, &my_float_offs );
		my_float_on = TRUE;		// ����������Ă���t���[�g������
	}

	
	for( i=0; i<WFLBY_FLOAT_MAX; i++ ){
		if( WFLBY_SYSTEM_GetFloatMove( p_sys->p_system, i ) == WFLBY_FLOAT_STATE_MOVE ){

			// �\���̂n�m�@�n�e�e
			WFLBY_3DMAPOBJCONT_FLOAT_SetDraw( p_sys->p_mapobjcont, i, TRUE );

			// ���W
			WFLBY_FLOAT_CONT_SetMatrix( p_sys, i );

			// �\�񃍃b�N�ݒ�
			WFLBY_FLOAT_CONT_SetReserveLock( p_sys, i );

			// ����������Ă���t���[�g���H
			if( my_float_on ){
				if( my_float_idx == i ){
					my_float = TRUE;
				}else{
					my_float = FALSE;
				}
			}
			
			// �A�j��
			WFLBY_FLOAT_CONT_SetAnm( p_sys, i, my_float_on, my_float, my_float_offs );
		}else{

			// �\���̂n�m�@�n�e�e
			WFLBY_3DMAPOBJCONT_FLOAT_SetDraw( p_sys->p_mapobjcont, i, FALSE );
		}
	}

	// �A�j������
	for( i=0; i<WFLBY_FLOAT_MAX; i++ ){


		// ��
		result = WFLBY_3DMAPOBJCONT_FLOAT_CheckAnmSound( p_sys->p_mapobjcont, i );
		p_sys->anm_play[i][WFLBY_FLOAT_ANM_SOUND] = result;

		// ���̗h�炵
		result = WFLBY_FLOAT_CONT_MainShake( &p_sys->shake[i], p_sys->p_mapobjcont, i );
		p_sys->anm_play[i][WFLBY_FLOAT_ANM_SHAKE] = result;

		// ����
		result = WFLBY_3DMAPOBJCONT_FLOAT_CheckAnmBody( p_sys->p_mapobjcont, i );
		p_sys->anm_play[i][WFLBY_FLOAT_ANM_BODY] = result;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j�������`�F�b�N����
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	floatidx	�t���[�g�C���f�b�N�X
 *	@param	anmidx		�A�j���C���f�b�N�X
 *
 *	@retval	TRUE		�A�j����	
 *	@retval	FALSE		�A�j����~
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_FLOAT_CONT_CheckAnm( const WFLBY_FLOAT_CONT* cp_sys, u32 floatidx, WFLBY_FLOAT_ANM_TYPE anmidx )
{
	GF_ASSERT( floatidx < WFLBY_FLOAT_MAX );
	GF_ASSERT( anmidx < WFLBY_FLOAT_ANM_NUM );
	return cp_sys->anm_play[floatidx][anmidx];
}





//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	���W�̐ݒ�
 *
 *	@param	p_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_FLOAT_CONT_SetMatrix( WFLBY_FLOAT_CONT* p_sys, u32 idx )
{
	s32 count;
	s32 count_max;
	fx32 x;
	fx32 dis;

	// �J�E���g�l�ƍő吔���獡�̕\���ʒu���擾
	count		= WFLBY_SYSTEM_GetFloatCount( p_sys->p_system, idx );
	count_max	= WFLBY_SYSTEM_GetFloatCountMax( p_sys->p_system );

	// �v�Z����ɂ͒l���傫������̂ŁA�ړ������𒲐�����
	dis = FX_Div( p_sys->float_move_dis, FX32_CONST(2) );
	x	= FX_Div( FX_Mul(FX32_CONST( count ), dis), FX32_CONST(count_max) );
	x	= FX_Mul( x, FX32_CONST(2) );
	x	+= p_sys->float_move_start;

//	OS_TPrintf( "idx=%d  x=%d\n", idx, x >> FX32_SHIFT );

	// ���W��ݒ�
	WFLBY_3DMAPOBJCONT_FLOAT_SetPos( p_sys->p_mapobjcont, idx, x );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�̃A�j����ݒ肷��
 *
 *	@param	p_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 *	@param	my_float_on		�����̓t�g�[�g�ɏ���Ă��邩�H
 *	@param	my_float		����������Ă���t���[�g�Ȃ̂�
 *	@param	my_float_offs	����������Ă�����Ȃ̈ʒu
 */
//-----------------------------------------------------------------------------
static void WFLBY_FLOAT_CONT_SetAnm( WFLBY_FLOAT_CONT* p_sys, u32 idx, BOOL my_float_on, BOOL my_float, u32 my_float_offs )
{
	int i;
	BOOL anm_on;
	BOOL anm_result = FALSE;
	u32 season;
	u32 room;
	BOOL my_float_offs_chk;

	room	= WFLBY_SYSTEM_GetRoomType( p_sys->p_system );
	season	= WFLBY_SYSTEM_GetSeason( p_sys->p_system );


	
	for( i=0; i<WFLBY_FLOAT_ON_NUM; i++ ){
		// �A�j���̍Đ��`�F�b�N
		anm_on = WFLBY_SYSTEM_GetFloatAnm( p_sys->p_system, (idx*WFLBY_FLOAT_ON_NUM)+i );

		// �����̃t���[�g�̂Ƃ��͎����̍��Ȃ��`�F�b�N����
		my_float_offs_chk = FALSE;
		if( my_float == TRUE ){
			if( i == my_float_offs ){
				my_float_offs_chk = TRUE;
			}
		}
		
		if( anm_on == TRUE ){
#ifdef WFLBY_DEBUG_FLOAT_ANM_SELECT
			switch( s_WFLBY_DEBUG_FLOAT_ANM_SELECT_COUNT ){
#else
			switch( i ){
#endif
			// ������
			case WFLBY_FLOAT_ANM_SOUND:
				// �A�j���ݒ�
				anm_result = WFLBY_3DMAPOBJCONT_FLOAT_SetAnmSound( p_sys->p_mapobjcont, idx );
				// ���Əo��
				if( anm_result == TRUE ){
					// ��������SE��炷
					if( (idx % WFLBY_FLOAT_ON_NUM) == 2 ){
						// �ł����t���[�g
						WFLBY_FLOAT_CONT_PlayMonsVoice( my_float_on, my_float, my_float_offs_chk, sc_WFLBY_FLOAT_DATA[ room ].monsno_big );

						if( sc_WFLBY_FLOAT_DATA[ room ].monsno_big_snd_pv != WFLBY_SND_RIDE_INVALIDITY ){
							WFLBY_FLOAT_CONT_PlaySe( my_float_on, my_float, my_float_offs_chk, sc_WFLBY_FLOAT_DATA[ room ].monsno_big_snd_pv );
						}
					}else{
						// ���������t���[�g
						WFLBY_FLOAT_CONT_PlayMonsVoice( my_float_on, my_float, my_float_offs_chk, sc_WFLBY_FLOAT_DATA[ room ].monsno_sml );

						if( sc_WFLBY_FLOAT_DATA[ room ].monsno_sml_snd_pv != WFLBY_SND_RIDE_INVALIDITY ){
							WFLBY_FLOAT_CONT_PlaySe( my_float_on, my_float, my_float_offs_chk, sc_WFLBY_FLOAT_DATA[ room ].monsno_sml_snd_pv );
						}
					}
					
					WFLBY_SYSTEM_CleanFloatAnm( p_sys->p_system, (idx*WFLBY_FLOAT_ON_NUM)+i );	// �A�j���Đ������̂Ńt���O�𗎂Ƃ�
				}
				break;
			// �t���[�g�h�炵�A�j��
			case WFLBY_FLOAT_ANM_SHAKE:
				{
					u32 anm;

					if( (idx % WFLBY_FLOAT_ON_NUM) == 2 ){
						anm = WFLBY_FLOAT_SHAKE_ANM_NORMAL_BIG;
					}else{
						anm = WFLBY_FLOAT_SHAKE_ANM_NORMAL;
					}
					anm_result = WFLBY_FLOAT_CONT_InitShake( &p_sys->shake[ idx ], anm, p_sys->p_mapobjcont, idx );
					if( anm_result == TRUE ){
						WFLBY_SYSTEM_CleanFloatAnm( p_sys->p_system, (idx*WFLBY_FLOAT_ON_NUM)+i );	// �A�j���Đ������̂Ńt���O�𗎂Ƃ�

						WFLBY_FLOAT_CONT_PlaySe( my_float_on, my_float, my_float_offs_chk, WFLBY_SND_RIDE_BUTTON01 );
						
					}
				}
				break;

			// �{�f�B�A�j��
			case WFLBY_FLOAT_ANM_BODY:
				anm_result = WFLBY_3DMAPOBJCONT_FLOAT_SetAnmBody( p_sys->p_mapobjcont, idx );
				if( anm_result == TRUE ){
					WFLBY_SYSTEM_CleanFloatAnm( p_sys->p_system, (idx*WFLBY_FLOAT_ON_NUM)+i );	// �A�j���Đ������̂Ńt���O�𗎂Ƃ�
					
					// ��������SE��炷
					if( (idx % WFLBY_FLOAT_ON_NUM) == 2 ){
						// �ł����t���[�g
						WFLBY_FLOAT_CONT_PlaySe( my_float_on, my_float, my_float_offs_chk, sc_WFLBY_FLOAT_DATA[ room ].monsno_big_snd_ex );
					}else{
						// ���������t���[�g
						WFLBY_FLOAT_CONT_PlaySe( my_float_on, my_float, my_float_offs_chk, sc_WFLBY_FLOAT_DATA[ room ].monsno_sml_snd_ex );
					}
				}
				break;
			}
		}
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�r�d�̍Đ�
 *
 *	@param	my_float_on		�����̓t���[�g�ɏ���Ă邩�H
 *	@param	my_float		����������Ă���t���[�g���H
 *	@param	my_offs			����������Ă�����Ȃ��H
 *	@param	se_idx			�r�d�m��
 */
//-----------------------------------------------------------------------------
static void WFLBY_FLOAT_CONT_PlaySe( BOOL my_float_on, BOOL my_float, BOOL my_offs, u32 se_idx )
{
	if( my_float_on ){
		// ����Ă���Ƃ��́A����������Ă���t���[�Ƃ����r�d��炷
		if( my_float ){

			// ����Ɏ����̍��Ȃ̂r�d�Ȃ玩����p�v���C���[�Ŗ炷
			if( my_offs ){
				Snd_SePlayEx( se_idx, SND_PLAYER_NO_WIFI_HIROBA );
			}else{
				Snd_SePlay( se_idx );
			}
		}
	}else{

		// ����Ă��Ȃ��Ƃ��͑S��Snd_SePlay�ŏ㏑��
		Snd_SePlay( se_idx );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�P�����{�C�X�p
 */
//-----------------------------------------------------------------------------
static void WFLBY_FLOAT_CONT_PlayMonsVoice( BOOL my_float_on, BOOL my_float, BOOL my_offs, u32 monsno )
{
	if( my_float_on ){
		// ����Ă���Ƃ��́A����������Ă���t���[�Ƃ����r�d��炷
		if( my_float ){

			// ����Ɏ����̍��Ȃ̂r�d�Ȃ玩����p�v���C���[�Ŗ炷
			Snd_PMVoicePlay( monsno, 0 );
		}
	}else{

		// ����Ă��Ȃ��Ƃ��͑S���㏑��
		Snd_PMVoicePlay( monsno, 0 );
	}
}




//----------------------------------------------------------------------------
/**
 *	@brief	���A�j��	������
 *
 *	@param	p_wk			���[�N
 *	@param	anm_type		�A�j���^�C�v
 *	@param	p_mapobjcont	�}�b�v�I�u�W�F�Ǘ��V�X�e��
 *	@param	idx				�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_FLOAT_CONT_InitShake( WFLBY_FLOAT_SHAKE* p_wk, u32 anm_type, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx )
{
	static void (* const pFunc[])( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx ) = {
		WFLBY_FLOAT_CONT_InitShakeNormal,
		WFLBY_FLOAT_CONT_InitShakeNormalBig,
	};
	GF_ASSERT( anm_type < WFLBY_FLOAT_SHAKE_ANM_NUM );

	// ���쒆�͓������Ȃ�
	if( p_wk->anm == TRUE ){
		return FALSE;
	}

	p_wk->anm_type	= anm_type;
	p_wk->anm		= TRUE;
	pFunc[ anm_type ]( p_wk, p_mapobjcont, idx );

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̗h�炵�A�j�����C��
 *
 *	@param	p_wk			���[�N
 *	@param	p_mapobjcont	�}�b�v�I�u�W�F�Ǘ��V�X�e��
 *	@param	idx				�C���f�b�N�X
 *
 *	@retval	TRUE	�h�炵��
 *	@retval	FALSE	�h�炵�I��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_FLOAT_CONT_MainShake( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx )
{
	BOOL result;
	static BOOL (* const pFunc[])( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx ) = {
		WFLBY_FLOAT_CONT_MainShakeNormal,
		WFLBY_FLOAT_CONT_MainShakeNormal,
	};

	if( p_wk->anm ){
		result = pFunc[ p_wk->anm_type ]( p_wk, p_mapobjcont, idx );
		if( result == TRUE ){
			// �I��
			memset( p_wk, 0, sizeof(WFLBY_FLOAT_SHAKE) );
		}
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʏ���A�j��	�J�n
 *
 *	@param	p_wk			���[�N
 *	@param	p_mapobjcont	�}�b�v�I�u�W�F�Ǘ��V�X�e��
 *	@param	idx				�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_FLOAT_CONT_InitShakeNormal( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx )
{
	p_wk->data.normal.count			= 0;
	p_wk->data.normal.count_max		= WFLBY_FLOAT_SHAKE_NRM_COUNT_MAX;
	p_wk->data.normal.rot_num		= WFLBY_FLOAT_SHAKE_NRM_COUNT_ROT;
	p_wk->data.normal.move_y		= WFLBY_FLOAT_SHAKE_NRM_COUNT_MOVEY;
	p_wk->data.normal.roop			= WFLBY_FLOAT_SHAKE_NRM_COUNT_ROOP;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʏ���傫���A�j��	�J�n
 *
 *	@param	p_wk			���[�N
 *	@param	p_mapobjcont	�}�b�v�I�u�W�F�Ǘ��V�X�e��
 *	@param	idx				�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_FLOAT_CONT_InitShakeNormalBig( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx )
{
	p_wk->data.normal.count			= 0;
	p_wk->data.normal.count_max		= WFLBY_FLOAT_SHAKE_NRMBIG_COUNT_MAX;
	p_wk->data.normal.rot_num		= WFLBY_FLOAT_SHAKE_NRMBIG_COUNT_ROT;
	p_wk->data.normal.move_y		= WFLBY_FLOAT_SHAKE_NRMBIG_COUNT_MOVEY;
	p_wk->data.normal.roop			= WFLBY_FLOAT_SHAKE_NRMBIG_COUNT_ROOP;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʏ���A�j��
 *
 *	@param	p_wk			���[�N
 *	@param	p_mapobjcont	�}�b�v�I�u�W�F�Ǘ��V�X�e��
 *	@param	idx				�C���f�b�N�X
 *
 *	@retva	TRUE	�A�j������
 *	@retva	FALSE	�A�j����
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_FLOAT_CONT_MainShakeNormal( WFLBY_FLOAT_SHAKE* p_wk, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 idx )
{
	VecFx32 ofs_pos = {0};	// ���W
	u16	rotx;
	u16 eff_rot;
	BOOL ret = FALSE;

	// �J�E���g����
	if( (p_wk->data.normal.count+1) < p_wk->data.normal.count_max ){
		p_wk->data.normal.count ++;
	}else{
		p_wk->data.normal.count = 0;
		if( (p_wk->data.normal.roop - 1) > 0 ){
			p_wk->data.normal.roop --;
		}else{
			ret = TRUE;		// �I��
		}
	}

	// �J�E���g�l�����]�x�����擾����
	eff_rot = (p_wk->data.normal.count * 0xffff) / p_wk->data.normal.count_max;

	// ���̓x�����炘����]�p�x���v�Z
	rotx = FX_Mul( FX_SinIdx( eff_rot ), FX32_CONST( p_wk->data.normal.rot_num ) ) >> FX32_SHIFT;
	
	// �x���ړ��ʒu�v�Z
	ofs_pos.y =  FX_Mul( FX_SinIdx( eff_rot ), FX32_CONST( p_wk->data.normal.move_y ) );

	// ���̃f�[�^��ݒ�
	WFLBY_3DMAPOBJCONT_FLOAT_SetRot( p_mapobjcont, idx, rotx );
	WFLBY_3DMAPOBJCONT_FLOAT_SetOfsPos( p_mapobjcont, idx, &ofs_pos );


	return ret;		// �r��
}


//----------------------------------------------------------------------------
/**
 *	@brief	�\�񃍃b�N�ݒ�
 *
 *	@param	p_sys		�V�X�e��
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_FLOAT_CONT_SetReserveLock( WFLBY_FLOAT_CONT* p_sys, u32 idx )
{
	int i, j;
	u32 posidx;
	u32 now_station;
	VecFx32 matrix;
	fx32 lock_x;

	//�@���Ȃ̍��W���w��X���W����菬�����Ȃ�����\�񃍃b�N��������
	for( i=0; i<WFLBY_FLOAT_ON_NUM; i++ ){
		posidx = (idx * WFLBY_FLOAT_ON_NUM) + i;
		now_station = WFLBY_SYSTEM_GetFloatReserveLock( p_sys->p_system, posidx );

		// ���W���擾
		WFLBY_3DMAPOBJCONT_FLOAT_GetSheetPos( p_sys->p_mapobjcont, idx, i, &matrix );

		// ���b�N����w��ݒ�
		for( j=now_station; j<WFLBY_FLOAT_STATION_NUM; j++ ){
		
			// �w���b�N�`�F�b�N
			lock_x = ((p_sys->station_pos[j].x+WFLBY_FLOAT_CONT_LOCK_X_OFS)*WFLBY_3DMATRIX_GRID_SIZ);
			if( lock_x > matrix.x ){
				// ���b�N������
				WFLBY_SYSTEM_SetFloatReserveLock( p_sys->p_system, posidx, j );
			}
		}
	}
}

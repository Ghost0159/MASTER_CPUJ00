//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_fire.c
 *	@brief		WiFi�L��ԉΏ���
 *	@author		tomoya takahashi
 *	@data		2008.03.18
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]


#include "common.h"

#include "wflby_fire.h"

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
///	���ݓ���̒萔
//=====================================
#define WFLBY_FIRE_CONT_KOUGO_ONEANM_FRAME	( 14 )
#define WFLBY_FIRE_CONT_KOUGO_ANM_SPEED		( FX32_ONE )
#define WFLBY_FIRE_CONT_KOUGO_ANM_SEQ_NUM	( 4 )
enum{
	WFLBY_FIRE_CONT_KOUGO_ON,
	WFLBY_FIRE_CONT_KOUGO_OFF,
	WFLBY_FIRE_CONT_KOUGO_FLAG_NUM,
};

// ���ݗp�ԉ΃C���f�b�N�X
// ���݉ԉ΂���Ԃ��ꂢ�Ɍ�����悤�ɒ��������C���f�b�N�X
// �}�b�v��񂪍X�V���ꂽ�琮�������ς��̂Œ��ӂ��K�v�ł��B
// �ėp���̂܂����������e�[�u���ł��B
//
// [�ԉΔz�u�}]
//
//    1			2		0
//	9	 10
//
//	3
// 4	  
//		    15   16
//  11					12
//		5 13 6	7 14 8
//
//			�����
static const u8 sc_WFLBY_FIRE_CONT_KOUGO_FIRE_IDX[] = {
	6, 13, 5, 11,
	7, 14, 8, 12,
	1,  0, 2, 10,
	16, 9, 4, 3, 
	15, 
};



//-------------------------------------
///	�g�󔭎˒萔
//=====================================
#define WFLBY_FIRE_CONT_NAMI_ONEANM_FRAME	( 6 )
#define WFLBY_FIRE_CONT_NAMI_SEQ_NUM		( 7 )
enum{
	WFLBY_FIRE_CONT_NAMI_ON,
	WFLBY_FIRE_CONT_NAMI_OFF,
	WFLBY_FIRE_CONT_NAMI_FLAG_NUM,
};
// �g�p�ԉΔ��˃f�[�^
// [�ԉΔz�u�}]�Q��
static const u8 sc_WFLBY_FIRE_CONT_NAMI_00_IDX[] = {
	5,  13, 6, 7, 14, 8,
};
static const u8 sc_WFLBY_FIRE_CONT_NAMI_01_IDX[] = {
	11, 12,
};
static const u8 sc_WFLBY_FIRE_CONT_NAMI_02_IDX[] = {
	15, 16,	
};
static const u8 sc_WFLBY_FIRE_CONT_NAMI_03_IDX[] = {
	4,	
};
static const u8 sc_WFLBY_FIRE_CONT_NAMI_04_IDX[] = {
	3,
};
static const u8 sc_WFLBY_FIRE_CONT_NAMI_05_IDX[] = {
	9, 10,
};
static const u8 sc_WFLBY_FIRE_CONT_NAMI_06_IDX[] = {
	1, 2, 0,
};
// �g�V�[�P���X���Ƃ́@���ˁ@��~�@�t���O
static const u8 sc_WFLBY_FIRE_CONT_NAMI_ONOFF_FLAG[ WFLBY_FIRE_CONT_NAMI_SEQ_NUM ][ WFLBY_FIRE_CONT_NAMI_FLAG_NUM ] = {
	{ 0, 5 },
	{ 1, 6 },
	{ 2, 0 },
	{ 3, 1 },
	{ 4, 2 },
	{ 5, 3 },
	{ 6, 4 },
};




//-------------------------------------
///	�N���C�}�b�N�X
//=====================================
// �Đ��f�[�^
typedef struct {
	u32		start_count;
	fx32	anm_speed;
} WFLBY_FIRECONT_CLIMAX_ANMDATA;

#define WFLBY_FIRECONT_CLIMAX_ANMNUM	(3)
static const WFLBY_FIRECONT_CLIMAX_ANMDATA sc_WFLBY_FIRECONT_CLIMAX_ANMDATA[WFLBY_FIRECONT_CLIMAX_ANMNUM] = {
	{ 10, FX32_ONE },
	{ 45, FX32_ONE },
	{ 80, FX32_HALF },
};

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�e����^�C�v�̃��[�N
//=====================================
typedef union {
	struct{
		u32 seq;
	} kougo;

	struct{
		u32 seq;
	} nami;

	struct{
		u32 anmidx;
	} climax;
} WFLBY_FIRECONT_WK;

//-------------------------------------
///	�g�e�[�u���f�[�^
//=====================================
typedef struct {
	const u8* cp_tbl;
	u32	tblnum;
} WFLBY_FIRECONT_NAMI_TBL;



//-------------------------------------
///	�ԉΊǗ��V�X�e��
//=====================================
typedef struct _WFLBY_FIRE_CONT {
	const WFLBY_SYSTEM*		cp_system;
	WFLBY_3DMAPOBJ_CONT*	p_mapobjcont;

	u8	now_move_type;
	u8	fire_se;
	u16 last_hanabi_st;

	WFLBY_FIRECONT_WK move_wk;	// ���샏�[�N
} WFLBY_FIRE_CONT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WFLBY_FIRE_CONT_Move( WFLBY_FIRE_CONT* p_sys, u32 move_type, u32 count );
static void WFLBY_FIRE_CONT_MoveNone( WFLBY_FIRE_CONT* p_sys, u32 count );
static void WFLBY_FIRE_CONT_MoveNormal( WFLBY_FIRE_CONT* p_sys, u32 count );
static void WFLBY_FIRE_CONT_MoveKougo( WFLBY_FIRE_CONT* p_sys, u32 count );
static void WFLBY_FIRE_CONT_MoveNami( WFLBY_FIRE_CONT* p_sys, u32 count );
static void WFLBY_FIRE_CONT_MoveClimax( WFLBY_FIRE_CONT* p_sys, u32 count );

static void WFLBY_FIRE_CONT_Init( WFLBY_FIRE_CONT* p_sys, u32 move_type, u32 count );
static void WFLBY_FIRE_CONT_InitNone( WFLBY_FIRE_CONT* p_sys, u32 count );
static void WFLBY_FIRE_CONT_InitNormal( WFLBY_FIRE_CONT* p_sys, u32 count );
static void WFLBY_FIRE_CONT_InitKougo( WFLBY_FIRE_CONT* p_sys, u32 count );
static void WFLBY_FIRE_CONT_InitNami( WFLBY_FIRE_CONT* p_sys, u32 count );
static void WFLBY_FIRE_CONT_InitClimax( WFLBY_FIRE_CONT* p_sys, u32 count );



//-------------------------------------
///	�g�󔭎�
//=====================================
static void WFLBY_FIRE_CONT_Nami_GetTblData( u32 idx, WFLBY_FIRECONT_NAMI_TBL* p_buff );


//-------------------------------------
///	�ԉ΂̉��Ǘ�
//=====================================
static void WFLBY_FIRE_CONT_StartFireSe( WFLBY_FIRE_CONT* p_wk );
static void WFLBY_FIRE_CONT_StopFireSe( WFLBY_FIRE_CONT* p_wk );


//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΃C�x���g�Ǘ��V�X�e��		������
 *
 *	@param	cp_system		�V�X�e��	
 *	@param	p_mapobjcont	�z�u�I�u�W�F�Ǘ��V�X�e��
 *	@param	heapID			�q�[�vID
 *
 *	@return	�쐬�����V�X�e��
 */
//-----------------------------------------------------------------------------
WFLBY_FIRE_CONT* WFLBY_FIRECONT_Init( const WFLBY_SYSTEM* cp_system, WFLBY_3DMAPOBJ_CONT* p_mapobjcont, u32 heapID )
{
	WFLBY_FIRE_CONT* p_wk;
	u32 move_type;
	
	p_wk = sys_AllocMemory( heapID, sizeof(WFLBY_FIRE_CONT) );
	memset( p_wk, 0, sizeof(WFLBY_FIRE_CONT) );

	p_wk->cp_system		= cp_system;
	p_wk->p_mapobjcont	= p_mapobjcont;

	// �ԉΒ��̂Ƃ�����������
	move_type				= WFLBY_SYSTEM_FIRE_GetType( cp_system );
	p_wk->last_hanabi_st	= WFLBY_SYSTEM_Event_GetHanabi( cp_system );
	if( (WFLBY_EVENT_HANABI_BEFORE != p_wk->last_hanabi_st) && (move_type != WFLBY_FIRE_END) ){

		u32 count;

		// ��������ԉ΂��o��
		WFLBY_3DMAPOBJCONT_MAP_SetDouzouFire( p_mapobjcont, TRUE );

		//SE
		WFLBY_FIRE_CONT_StartFireSe( p_wk );

		// ���̏�Ԃ����āA����������
		count		= WFLBY_SYSTEM_FIRE_GetTypeMoveCount( cp_system );
		WFLBY_FIRE_CONT_Init( p_wk, move_type, count );
	}

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΃C�x���g�Ǘ��V�X�e��		�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_FIRECONT_Exit( WFLBY_FIRE_CONT* p_wk )
{
	WFLBY_FIRE_CONT_StopFireSe( p_wk );
	sys_FreeMemoryEz( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΃C�x���g�Ǘ��V�X�e��	���C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_FIRECONT_Main( WFLBY_FIRE_CONT* p_wk )
{
	u32 hanabi_st;
	u32 move_type;
	u32 count;

	move_type	= WFLBY_SYSTEM_FIRE_GetType( p_wk->cp_system );
	count		= WFLBY_SYSTEM_FIRE_GetTypeMoveCount( p_wk->cp_system );
	hanabi_st	= WFLBY_SYSTEM_Event_GetHanabi( p_wk->cp_system );

	// �ԉΏ�Ԃ��ύX�������`�F�b�N
	if( p_wk->last_hanabi_st != hanabi_st ){
		p_wk->last_hanabi_st = hanabi_st;

		if( hanabi_st == WFLBY_EVENT_HANABI_PLAY ){

			// �ԉ΂��J�n���ꂽ

			// ��������ԉ΂��o��
			WFLBY_3DMAPOBJCONT_MAP_SetDouzouFire( p_wk->p_mapobjcont, TRUE );

			// ���쏉����
			WFLBY_FIRE_CONT_Init( p_wk, move_type, count );

			//SE
			WFLBY_FIRE_CONT_StartFireSe( p_wk );
		}
	}



	// �ԉΑO�ȊO�̏�Ԃ̏ꍇ����
	// ���C�����삳����
	// move_type���I���ɂȂ����珟��ɏI���
	if( hanabi_st != WFLBY_EVENT_HANABI_BEFORE ){

		// ���삪�ς�����珉����
		if( p_wk->now_move_type != move_type ){
			// ���쏉����
			WFLBY_FIRE_CONT_Init( p_wk, move_type, count );
		}

		// ����
		WFLBY_FIRE_CONT_Move( p_wk, move_type, count );
	}
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΃C�x���g�Ǘ�	SE��~
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_FIRECONT_StopFireSe( WFLBY_FIRE_CONT* p_wk )
{
	WFLBY_FIRE_CONT_StopFireSe( p_wk );
}






//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΂̓���
 *
 *	@param	p_sys		���[�N
 *	@param	move_type	����^�C�v
 *	@param	count		�J�E���^
 */
//-----------------------------------------------------------------------------
static void WFLBY_FIRE_CONT_Move( WFLBY_FIRE_CONT* p_sys, u32 move_type, u32 count )
{
	static void (* const pFunc[])( WFLBY_FIRE_CONT* p_sys, u32 count ) = {
		WFLBY_FIRE_CONT_MoveNone,
		WFLBY_FIRE_CONT_MoveNone,
		WFLBY_FIRE_CONT_MoveNormal,
		WFLBY_FIRE_CONT_MoveKougo,
		WFLBY_FIRE_CONT_MoveNami,
		WFLBY_FIRE_CONT_MoveClimax,
	};

	GF_ASSERT( move_type < WFLBY_FIRE_NUM );
	if( move_type < WFLBY_FIRE_NUM ){
		pFunc[ move_type ]( p_sys, count );
	}
}
//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΃^�C�v�ɑΉ���������֐�
 *
 *	@param	p_sys			���[�N
 *	@param	count			���̓���J�E���^
 */
//-----------------------------------------------------------------------------
// �����Ȃ�
static void WFLBY_FIRE_CONT_MoveNone( WFLBY_FIRE_CONT* p_sys, u32 count )
{
}
// �ʏ킠�ɂ�
static void WFLBY_FIRE_CONT_MoveNormal( WFLBY_FIRE_CONT* p_sys, u32 count )
{
}
//  ���ݔ���
static void WFLBY_FIRE_CONT_MoveKougo( WFLBY_FIRE_CONT* p_sys, u32 count )
{
	BOOL result;
	u32 idx;
	BOOL flag;
	BOOL set_fire_flag;
	u32 kougo_idx;
	static const u32 sc_Flag[ WFLBY_FIRE_CONT_KOUGO_ANM_SEQ_NUM ][ WFLBY_FIRE_CONT_KOUGO_FLAG_NUM ] = {
		{ 0, 2 },
		{ 1, 3 },
		{ 2, 0 },
		{ 3, 1 },
	};

#if 0
	// �w�肵���ʒu�̉ԉ΂�����
	if( sys.trg & PAD_BUTTON_R ){
		static s32 hanabi_no;
		static s32 last_hanabi_no;

		OS_TPrintf( "hanabi_no %d\n", hanabi_no );

		WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFireIdx( p_sys->p_mapobjcont, 
				last_hanabi_no, FALSE, WFLBY_FIRE_CONT_KOUGO_ANM_SPEED );

		result = WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFireIdx( p_sys->p_mapobjcont, 
				hanabi_no, TRUE, WFLBY_FIRE_CONT_KOUGO_ANM_SPEED );

		last_hanabi_no = hanabi_no;
		
		if( result == FALSE ){
			hanabi_no = 0;
		}else{
			hanabi_no ++;
		}
	}
#endif

#if 1
	if( (count % WFLBY_FIRE_CONT_KOUGO_ONEANM_FRAME) == 0 ){


		// �A�j���ݒ�
		idx = 0;
		do{
			
			// �ԉ΂̑�����s����
			set_fire_flag = FALSE;

			// seq�� ���ˁA���̂܂܁A�Ƃ߂� ��؂�ւ���
			if( (idx % WFLBY_FIRE_CONT_KOUGO_ANM_SEQ_NUM) == sc_Flag[ p_sys->move_wk.kougo.seq ][ WFLBY_FIRE_CONT_KOUGO_ON ] ){
				flag = TRUE;
				set_fire_flag = TRUE;
			}
			else if( (idx % WFLBY_FIRE_CONT_KOUGO_ANM_SEQ_NUM) == sc_Flag[ p_sys->move_wk.kougo.seq ][ WFLBY_FIRE_CONT_KOUGO_OFF ] ){
				flag = FALSE;
				set_fire_flag = TRUE;
			}
			
			if( set_fire_flag == TRUE ){

				// �C���f�b�N�X�����݉ԉ΍\���C���f�b�N�X�ɕύX
				if( idx < NELEMS(sc_WFLBY_FIRE_CONT_KOUGO_FIRE_IDX) ){
					kougo_idx = sc_WFLBY_FIRE_CONT_KOUGO_FIRE_IDX[idx];
					
					result = WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFireIdx( p_sys->p_mapobjcont, 
							kougo_idx, flag, WFLBY_FIRE_CONT_KOUGO_ANM_SPEED );
					GF_ASSERT( result == TRUE );	// �ԉ΍\���f�[�^�������Ă��Ȃ�
				}else{
					result = FALSE;
				}
			}else{
				result = TRUE;
			}

			idx ++;
		}while( result == TRUE );

		p_sys->move_wk.kougo.seq = (p_sys->move_wk.kougo.seq + 1) % WFLBY_FIRE_CONT_KOUGO_ANM_SEQ_NUM;
	}
#endif
}
// �g�󔭎�
static void WFLBY_FIRE_CONT_MoveNami( WFLBY_FIRE_CONT* p_sys, u32 count )
{
	int i;
	u32	off, on;
	WFLBY_FIRECONT_NAMI_TBL nami_tbl;	// �g�e�[�u���f�[�^
	BOOL result;
	
	if( (count % WFLBY_FIRE_CONT_NAMI_ONEANM_FRAME) == 0 ){


		// ON/OFF���擾
		off = sc_WFLBY_FIRE_CONT_NAMI_ONOFF_FLAG[ p_sys->move_wk.nami.seq ][ WFLBY_FIRE_CONT_NAMI_OFF ];
		on	= sc_WFLBY_FIRE_CONT_NAMI_ONOFF_FLAG[ p_sys->move_wk.nami.seq ][ WFLBY_FIRE_CONT_NAMI_ON ];

		
		// �A�j���ݒ�
		// �A�j��OFF
		WFLBY_FIRE_CONT_Nami_GetTblData( off, &nami_tbl );
		for( i=0; i<nami_tbl.tblnum; i++ ){
			result = WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFireIdx( p_sys->p_mapobjcont, 
					nami_tbl.cp_tbl[i], FALSE, WFLBY_FIRE_CONT_KOUGO_ANM_SPEED );
			GF_ASSERT( result == TRUE );	// �ԉ΍\���f�[�^�������Ă��Ȃ�
		}

		// �A�j��ON
		WFLBY_FIRE_CONT_Nami_GetTblData( on, &nami_tbl );
		for( i=0; i<nami_tbl.tblnum; i++ ){
			result = WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFireIdx( p_sys->p_mapobjcont, 
					nami_tbl.cp_tbl[i], TRUE, WFLBY_FIRE_CONT_KOUGO_ANM_SPEED );
			GF_ASSERT( result == TRUE );	// �ԉ΍\���f�[�^�������Ă��Ȃ�
		}
		
		p_sys->move_wk.nami.seq = (p_sys->move_wk.nami.seq + 1) % WFLBY_FIRE_CONT_NAMI_SEQ_NUM;

	}
}
// �N���C�}�b�N�X
static void WFLBY_FIRE_CONT_MoveClimax( WFLBY_FIRE_CONT* p_sys, u32 count )
{
	if( p_sys->move_wk.climax.anmidx < WFLBY_FIRECONT_CLIMAX_ANMNUM ){
		
		if( sc_WFLBY_FIRECONT_CLIMAX_ANMDATA[p_sys->move_wk.climax.anmidx].start_count < count ){
			// ���ˁI
			WFLBY_3DMAPOBJCONT_MAP_SetFloorLightBigFire( p_sys->p_mapobjcont, 
					sc_WFLBY_FIRECONT_CLIMAX_ANMDATA[p_sys->move_wk.climax.anmidx].anm_speed );

			WFLBY_3DMAPOBJCONT_MAP_SetDouzouBigFire( p_sys->p_mapobjcont, 
					sc_WFLBY_FIRECONT_CLIMAX_ANMDATA[p_sys->move_wk.climax.anmidx].anm_speed );

			Snd_SePlay( WFLBY_SND_FIREWORKS02 );

			// ���̃A�j����
			p_sys->move_wk.climax.anmidx ++;
		}
		
	}
	
}


//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ΂̏�����
 *
 *	@param	p_sys		���[�N		
 *	@param	move_type	����^�C�v
 *	@param	count		�J�E���^
 */
//-----------------------------------------------------------------------------
static void WFLBY_FIRE_CONT_Init( WFLBY_FIRE_CONT* p_sys, u32 move_type, u32 count )
{
	static void (* const pFunc[])( WFLBY_FIRE_CONT* p_sys, u32 count ) = {
		WFLBY_FIRE_CONT_InitNone,
		WFLBY_FIRE_CONT_InitNone,
		WFLBY_FIRE_CONT_InitNormal,
		WFLBY_FIRE_CONT_InitKougo,
		WFLBY_FIRE_CONT_InitNami,
		WFLBY_FIRE_CONT_InitClimax,
	};

	GF_ASSERT( move_type < WFLBY_FIRE_NUM );
	if( move_type < WFLBY_FIRE_NUM ){

		// ���[�N�̃N���A
		memset( &p_sys->move_wk, 0, sizeof(WFLBY_FIRECONT_WK) );
		
		pFunc[ move_type ]( p_sys, count );
		p_sys->now_move_type = move_type;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ԉΓ���^�C�v�ɑΉ����������̏�����
 *
 *	@param	p_sys		�V�X�e��
 *	@param	count		�J�E���g�l
 */
//-----------------------------------------------------------------------------
static void WFLBY_FIRE_CONT_InitNone( WFLBY_FIRE_CONT* p_sys, u32 count )
{
	// �S�A�j�����Ƃ߂�
	WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFire( p_sys->p_mapobjcont, FALSE, FX32_ONE );
}
// �ʏ�A�j��
static void WFLBY_FIRE_CONT_InitNormal( WFLBY_FIRE_CONT* p_sys, u32 count )
{
	// �S�A�j������
	WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFire( p_sys->p_mapobjcont, TRUE, FX32_ONE );
}
// ���ݔ���
static void WFLBY_FIRE_CONT_InitKougo( WFLBY_FIRE_CONT* p_sys, u32 count )
{
	// ���̂������˂����͂��Ȃ̂ō��͑S�ԉ΂�OFF
	WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFire( p_sys->p_mapobjcont, FALSE, FX32_ONE );
}
// �g��
static void WFLBY_FIRE_CONT_InitNami( WFLBY_FIRE_CONT* p_sys, u32 count )
{
	// ���̂������˂����͂��Ȃ̂ō��͑S�ԉ΂�OFF
	WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFire( p_sys->p_mapobjcont, FALSE, FX32_ONE );
}
// �N���C�}�b�N�X
static void WFLBY_FIRE_CONT_InitClimax( WFLBY_FIRE_CONT* p_sys, u32 count )
{
	//SE
	WFLBY_FIRE_CONT_StopFireSe( p_sys );
	
	// ���̂������˂����͂��Ȃ̂ō��͑S�ԉ΂�OFF
	WFLBY_3DMAPOBJCONT_MAP_SetFloorLightFire( p_sys->p_mapobjcont, FALSE, FX32_ONE );
	WFLBY_3DMAPOBJCONT_MAP_SetDouzouFire( p_sys->p_mapobjcont, FALSE );
}




//----------------------------------------------------------------------------
/**
 *	@brief	�g�e�[�u���f�[�^�擾�֐�
 *
 *	@param	idx		�g�C���f�b�N�X
 *	@param	p_buff	�i�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_FIRE_CONT_Nami_GetTblData( u32 idx, WFLBY_FIRECONT_NAMI_TBL* p_buff )
{
	switch( idx ){
	case 0:
		p_buff->tblnum	= NELEMS(sc_WFLBY_FIRE_CONT_NAMI_00_IDX);
		p_buff->cp_tbl	= sc_WFLBY_FIRE_CONT_NAMI_00_IDX;
		break;
		
	case 1:
		p_buff->tblnum	= NELEMS(sc_WFLBY_FIRE_CONT_NAMI_01_IDX);
		p_buff->cp_tbl	= sc_WFLBY_FIRE_CONT_NAMI_01_IDX;
		break;
		
	case 2:
		p_buff->tblnum	= NELEMS(sc_WFLBY_FIRE_CONT_NAMI_02_IDX);
		p_buff->cp_tbl	= sc_WFLBY_FIRE_CONT_NAMI_02_IDX;
		break;
		
	case 3:
		p_buff->tblnum	= NELEMS(sc_WFLBY_FIRE_CONT_NAMI_03_IDX);
		p_buff->cp_tbl	= sc_WFLBY_FIRE_CONT_NAMI_03_IDX;
		break;
		
	case 4:
		p_buff->tblnum	= NELEMS(sc_WFLBY_FIRE_CONT_NAMI_04_IDX);
		p_buff->cp_tbl	= sc_WFLBY_FIRE_CONT_NAMI_04_IDX;
		break;
		
	case 5:
		p_buff->tblnum	= NELEMS(sc_WFLBY_FIRE_CONT_NAMI_05_IDX);
		p_buff->cp_tbl	= sc_WFLBY_FIRE_CONT_NAMI_05_IDX;
		break;

	case 6:
		p_buff->tblnum	= NELEMS(sc_WFLBY_FIRE_CONT_NAMI_06_IDX);
		p_buff->cp_tbl	= sc_WFLBY_FIRE_CONT_NAMI_06_IDX;
		break;

	default:
		GF_ASSERT(0);
		break;
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ�SE�Đ�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_FIRE_CONT_StartFireSe( WFLBY_FIRE_CONT* p_wk )
{
	if( p_wk->fire_se == FALSE ){
		Snd_SePlay( WFLBY_SND_FIREWORKS01 );
		p_wk->fire_se = TRUE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ�SE��~
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_FIRE_CONT_StopFireSe( WFLBY_FIRE_CONT* p_wk )
{
	if( p_wk->fire_se ){
		Snd_SeStopBySeqNo( WFLBY_SND_FIREWORKS01, 0 );
		p_wk->fire_se = FALSE;
	}
}

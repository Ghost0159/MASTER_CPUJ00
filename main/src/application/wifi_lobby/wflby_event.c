//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_event.c
 *	@brief		�C�x���g�Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.11.26
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "wflby_def.h"
#include "wflby_event.h"

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
#define WFLBY_DEBUG_EVENT_PRINT		// �C�x���g�v�����g
#endif

// �f�o�b�N�v�����g�ݒ�
#ifdef WFLBY_DEBUG_EVENT_PRINT
#define WFLBY_EVENT_DEBUG_PRINT(...)		OS_TPrintf(__VA_ARGS__)
#else
#define WFLBY_EVENT_DEBUG_PRINT(...)		((void)0);
#endif


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�C�x���g�^�C�~���O�萔
//=====================================
enum {
	WFLBY_EVENT_FUNC_BEFORE,
	WFLBY_EVENT_FUNC_AFTER,
} ;


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�C�x���g�f�[�^
//=====================================
typedef struct {
	const WFLBY_EVENT_DATA*	cp_data;	// �C�x���g�f�[�^
	void*					p_wk;		// ���[�J�����[�N
	void*					p_param;	// �p�����[�^
	u32						seq;		// �V�[�P���X
} WFLBY_EVENTWK_LOCAL;

//-------------------------------------
///	�C�x���g���[�N
//=====================================
typedef struct _WFLBY_EVENTWK{
	// �q�[�vID
	u16				heapID;

	// �C�x���g����t���O
	u8				evmove;

	// �C�x���gNO
	u8				evno;

	// ���s�f�[�^
	WFLBY_EVENTWK_LOCAL data;

	// �Δ��
	WFLBY_EVENTWK_LOCAL tmp;
} WFLBY_EVENTWK;



//-------------------------------------
///	�C�x���g�Ǘ��V�X�e��
//=====================================
typedef struct _WFLBY_EVENT {
	// �q�[�vID
	u32				heapID;

	// ���r�[�f�[�^
	WFLBY_ROOMWK*	p_rmwk;

	// �ʃC�x���g�f�[�^
	WFLBY_EVENTWK	private[ WFLBY_PLAYER_MAX ];

	// �S�̃C�x���g�f�[�^
	WFLBY_EVENTWK	global;
}WFLBY_EVENT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�V�X�e���Ǘ��֐�
//=====================================
static void WFLBY_EVENT_MainLocal( WFLBY_EVENT* p_sys, u32 timing );



//-------------------------------------
///	�C�x���g�Ǘ��V�X�e���֐�
//=====================================
static void WFLBY_EVENTWK_Init( WFLBY_EVENTWK* p_wk, u32 evno, u32 heapID );
static void WFLBY_EVENTWK_Exit( WFLBY_EVENTWK* p_wk );
static void WFLBY_EVENTWK_Main( WFLBY_EVENTWK* p_wk, WFLBY_EVENT* p_sys, u32 timing, u32 plno );
static void WFLBY_EVENTWK_Set( WFLBY_EVENTWK* p_wk, const WFLBY_EVENT_DATA* cp_data, void* p_param );
static void WFLBY_EVENTWK_SetMove( WFLBY_EVENTWK* p_wk, BOOL move );
static void WFLBY_EVENTWK_Push( WFLBY_EVENTWK* p_wk, const WFLBY_EVENT_DATA* cp_data, void* p_param );
static void WFLBY_EVENTWK_Pop( WFLBY_EVENTWK* p_wk );
static BOOL WFLBY_EVENTWK_CheckIntrEnd( const WFLBY_EVENTWK* cp_wk );
static BOOL WFLBY_EVENTWK_CheckEventDo( const WFLBY_EVENTWK* cp_wk );
static void WFLBY_EVENTWKCL_SetData( WFLBY_EVENTWK_LOCAL* p_wk, const WFLBY_EVENT_DATA* cp_data, void* p_param, u32 seq, void* p_evwk );
static void WFLBY_EVENTWKCL_Clean( WFLBY_EVENTWK_LOCAL* p_wk );








//-------------------------------------
// �C�x���g�V�X�e���Ǘ�
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�V�X�e��	������	
 *
 *	@param	p_rmwk		ROOM���[�N
 *	@param	heapID		�q�[�vID
 *
 *	@return	�C�x���g�V�X�e��
 */
//-----------------------------------------------------------------------------
WFLBY_EVENT* WFLBY_EVENT_Init( WFLBY_ROOMWK* p_rmwk, u32 heapID )
{
	WFLBY_EVENT* p_sys;

	p_sys = sys_AllocMemory( heapID, sizeof(WFLBY_EVENT) );
	memset( p_sys, 0, sizeof(WFLBY_EVENT) );

	// �q�[�v�ۑ�
	p_sys->heapID = heapID;

	// �����f�[�^��ۑ�
	p_sys->p_rmwk = p_rmwk;

	// �ʃC�x���g�����ݒ�
	{
		int i;
		for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
			WFLBY_EVENTWK_Init( &p_sys->private[i], i, heapID );
		}
	}

	// �S�̃C�x���g�����ݒ�
	WFLBY_EVENTWK_Init( &p_sys->global, 0, heapID );

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�V�X�e��	�j��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENT_Exit( WFLBY_EVENT* p_sys )
{
	// �ʃC�x���g�j��
	{
		int i;
		for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
			WFLBY_EVENTWK_Exit( &p_sys->private[i] );
		}
	}

	// �S�̃C�x���g�f�[�^�j��
	WFLBY_EVENTWK_Exit( &p_sys->global );


	// �V�X�e���j��
	sys_FreeMemoryEz( p_sys );
}

//-------------------------------------
// �e���s�֐�
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	����I�u�W�F�������s�֐�
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENT_MainBefore( WFLBY_EVENT* p_sys )
{
	WFLBY_EVENT_MainLocal( p_sys, WFLBY_EVENT_FUNC_BEFORE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	����I�u�W�F���N�G�X�g�ݒ����s�֐�
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENT_MainAfter( WFLBY_EVENT* p_sys )
{
	WFLBY_EVENT_MainLocal( p_sys, WFLBY_EVENT_FUNC_AFTER );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�p�����[�^���擾����
 *
 *	@param	p_wk	���[�N
 *
 *	@return	�p�����[�^
 */
//-----------------------------------------------------------------------------
void* WFLBY_EVENTWK_GetParam( WFLBY_EVENTWK* p_wk )
{
	return p_wk->data.p_param;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g��Ɨ̈���쐬����
 *
 *	@param	p_wk		���[�N
 *	@param	size		�T�C�Y
 */
//-----------------------------------------------------------------------------
void* WFLBY_EVENTWK_AllocWk( WFLBY_EVENTWK* p_wk, u32 size )
{
	GF_ASSERT( p_wk->data.p_wk == NULL );
	p_wk->data.p_wk = sys_AllocMemory( p_wk->heapID, size );
	memset( p_wk->data.p_wk, 0, size );
	return p_wk->data.p_wk;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g��Ɨ̈��j������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENTWK_DeleteWk( WFLBY_EVENTWK* p_wk )
{
	GF_ASSERT( p_wk->data.p_wk != NULL );
	sys_FreeMemoryEz( p_wk->data.p_wk );
	p_wk->data.p_wk = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g��Ɨ̈���擾����
 *
 *	@param	p_wk		���[�N
 *
 *	@return	��Ɨ̈�
 */
//-----------------------------------------------------------------------------
void* WFLBY_EVENTWK_GetWk( WFLBY_EVENTWK* p_wk )
{
	return p_wk->data.p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�[�P���X���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�V�[�P���X
 */
//-----------------------------------------------------------------------------
u32 WFLBY_EVENTWK_GetSeq( const WFLBY_EVENTWK* cp_wk )
{
	return  cp_wk->data.seq;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�[�P���X��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	seq			�V�[�P���X
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENTWK_SetSeq( WFLBY_EVENTWK* p_wk, u32 seq )
{
	
	p_wk->data.seq = seq;
	WFLBY_EVENT_DEBUG_PRINT( "event no = %d seq = %d\n", p_wk->evno, seq );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�[�������P�i�߂�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENTWK_AddSeq( WFLBY_EVENTWK* p_wk )
{
	p_wk->data.seq ++;
	WFLBY_EVENT_DEBUG_PRINT( "event no = %d seq = %d\n", p_wk->evno, p_wk->data.seq );
}

//-------------------------------------
// �ʃC�x���g�Ǘ�
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�풓�C�x���g��ݒ肷��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	plno		�v���C���[ID
 *	@param	cp_data		�C�x���g�f�[�^
 *	@param	p_param	�p�����[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENT_SetPrivateEvent( WFLBY_EVENT* p_sys, u32 plno, const WFLBY_EVENT_DATA* cp_data, void* p_param )
{
	GF_ASSERT( p_sys );
	GF_ASSERT( plno < WFLBY_PLAYER_MAX );

	// �풓�C�x���g�f�[�^�ݒ�
	WFLBY_EVENTWK_Set( &p_sys->private[ plno ], cp_data, p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���荞�݃C�x���g�f�[�^�ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	plno		�v���C���[NO
 *	@param	cp_data		�f�[�^
 *	@param	p_param	�p�����[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENT_SetPrivateIntrEvent( WFLBY_EVENT* p_sys, u32 plno, const WFLBY_EVENT_DATA* cp_data, void* p_param )
{
	GF_ASSERT( p_sys );
	GF_ASSERT( plno < WFLBY_PLAYER_MAX );

	// ���荞�݃C�x���g�ݒ�
	WFLBY_EVENTWK_Push( &p_sys->private[ plno ], cp_data, p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���荞�݃C�x���g�I���`�F�b�N
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	plno		�v���C���[NO
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	���荞�ݒ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_EVENT_WaitPrivateIntrEvent( const WFLBY_EVENT* cp_sys, u32 plno )
{
	GF_ASSERT( cp_sys );
	GF_ASSERT( plno < WFLBY_PLAYER_MAX );

	return WFLBY_EVENTWK_CheckIntrEnd( &cp_sys->private[ plno ] );
}


#ifdef PM_DEBUG
// �f�o�b�N
void WFLBY_EVENT_SetPrivateEvent_DEBUG( WFLBY_EVENT* p_sys, u32 plno, const WFLBY_EVENT_DATA* cp_data, void* p_param, u32 line )
{
	WFLBY_EVENT_DEBUG_PRINT( "event set plno=%d line=%d\n", plno, line );
	WFLBY_EVENT_SetPrivateEvent( p_sys, plno, cp_data, p_param );
}

void WFLBY_EVENT_SetPrivateIntrEvent_DEBUG( WFLBY_EVENT* p_sys, u32 plno, const WFLBY_EVENT_DATA* cp_data, void* p_param, u32 line )
{
	WFLBY_EVENT_DEBUG_PRINT( "event set intr plno=%d line=%d\n", plno, line );
	WFLBY_EVENT_SetPrivateIntrEvent( p_sys, plno, cp_data, p_param );
}

#endif


//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�̓���ݒ�
 *
 *	@param	p_sys		�V�X�e��
 *	@param	plno		���삷��C�x���g��PLNO
 *	@param	move		����t���O	TRUE�F����		FALSE�F�񓮍�
 *
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENT_SetPrivateEventMove( WFLBY_EVENT* p_sys, u32 plno, BOOL move )
{
	GF_ASSERT( p_sys );
	GF_ASSERT( plno < WFLBY_PLAYER_MAX );

	WFLBY_EVENTWK_SetMove( &p_sys->private[ plno ], move );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�S�̃C�x���g���J�n
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_data		�C�x���g�f�[�^ 
 *	@param	p_param	�p�����[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_EVENT_SetGlobalEvent( WFLBY_EVENT* p_sys, const WFLBY_EVENT_DATA* cp_data, void* p_param )
{
	GF_ASSERT( p_sys );

	// ���荞�݃C�x���g�ݒ�
	WFLBY_EVENTWK_Push( &p_sys->global, cp_data, p_param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���[�o���C�x���g���I���������`�F�b�N
 *
 *	@param	cp_sys	�V�X�e�����[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_EVENT_WaitGlobalEvent( const WFLBY_EVENT* cp_sys )
{
	GF_ASSERT( cp_sys );
	return WFLBY_EVENTWK_CheckIntrEnd( &cp_sys->global );
}




//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENT_MainLocal( WFLBY_EVENT* p_sys, u32 timing )
{

	// �S�̃C�x���g���s���ŗD��
	if( WFLBY_EVENTWK_CheckEventDo( &p_sys->global ) == TRUE ){
		WFLBY_EVENTWK_Main( &p_sys->global, p_sys, timing, 0 );
		return ;
	}

	// �v���C�x�[�g�C�x���g���s
	{
		int i;

		for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
			if( WFLBY_EVENTWK_CheckEventDo( &p_sys->private[i] ) == TRUE ){
				WFLBY_EVENTWK_Main( &p_sys->private[i], p_sys, timing, i );
			}
		}
	}
}




//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���[�N������
 *
 *	@param	p_wk		���[�N
 *	@param	evno		�C�x���g�i���o�[
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENTWK_Init( WFLBY_EVENTWK* p_wk, u32 evno, u32 heapID )
{
	p_wk->heapID = heapID;
	p_wk->evmove = TRUE;
	p_wk->evno	 = evno;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���[�N�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENTWK_Exit( WFLBY_EVENTWK* p_wk )
{
	//  ���s�f�[�^�j��
	if( p_wk->data.p_wk != NULL ){
		sys_FreeMemoryEz( p_wk->data.p_wk );
	}
	if( p_wk->tmp.p_wk != NULL ){
		sys_FreeMemoryEz( p_wk->tmp.p_wk );
	}
	memset( p_wk, 0, sizeof(WFLBY_EVENTWK) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C�����s
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e��
 *	@param	timing		���s�^�C�~���O
 *	@param	plno		�v���C���[NO
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENTWK_Main( WFLBY_EVENTWK* p_wk, WFLBY_EVENT* p_sys, u32 timing, u32 plno )
{
	BOOL result;
	pEVENT_FUNC p_func;

	// �C�x���g����`�F�b�N
	if( p_wk->evmove == FALSE ){
		return ;
	}

	// �^�C�~���O�ɂ���ēǂݑւ���
	switch( timing ){
	case WFLBY_EVENT_FUNC_BEFORE:
		p_func = p_wk->data.cp_data->p_before;
		break;
	case WFLBY_EVENT_FUNC_AFTER:
		p_func = p_wk->data.cp_data->p_after;
		break;
	default:
		GF_ASSERT(0);	// ���肦�Ȃ�
		break;
	}
	if( p_func != NULL ){
		result = p_func( p_wk,
				p_sys->p_rmwk, plno );

		// �I�����A���Ă�����
		if( result == TRUE ){
			// �ޔ��f�[�^���풓�ɖ߂�
			WFLBY_EVENTWK_Pop( p_wk );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�f�[�^��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 *	@param	p_param	�p�����[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENTWK_Set( WFLBY_EVENTWK* p_wk, const WFLBY_EVENT_DATA* cp_data, void* p_param )
{
	// ���荞�ݒ��łȂ����`�F�b�N
	GF_ASSERT_MSG( WFLBY_EVENTWK_CheckIntrEnd( p_wk ) == TRUE, "intr event\n" );

	// �f�[�^���c���ĂȂ����`�F�b�N
	GF_ASSERT_MSG( p_wk->data.p_wk == NULL, "rest wk\n" );

	WFLBY_EVENTWKCL_SetData( &p_wk->data, cp_data, p_param, 0, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	����t���O��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	move		����t���O
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENTWK_SetMove( WFLBY_EVENTWK* p_wk, BOOL move )
{
	p_wk->evmove = move;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���[�N	���荞�݃C�x���g��ݒ�	�풓�C�x���g��ޔ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�ݒ肷��
 *	@param	p_param	�p�����[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENTWK_Push( WFLBY_EVENTWK* p_wk, const WFLBY_EVENT_DATA* cp_data, void* p_param )
{
	// ���荞�݋N�����Ɋ��荞�ݐݒ�ł��Ȃ�
	GF_ASSERT_MSG( WFLBY_EVENTWK_CheckIntrEnd( p_wk ) == TRUE, "event tmp over\n" );

	// �ޔ�
	p_wk->tmp	= p_wk->data;

	// �ݒ�
	WFLBY_EVENTWKCL_SetData( &p_wk->data, cp_data, p_param, 0, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g���[�N	�풓�C�x���g�����ɖ߂�
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENTWK_Pop( WFLBY_EVENTWK* p_wk )
{
	// �ޔ�悪���邩�`�F�b�N
//	GF_ASSERT( p_wk->cp_tmp != NULL, "event tmp none\n" );	// ����ς�ޔ�悪�Ȃ��Ă��悢
	// �f�[�^���c���ĂȂ����`�F�b�N
	GF_ASSERT( p_wk->data.p_wk == NULL );

	// �ޔ���ݒ�
	p_wk->data		= p_wk->tmp;

	// �ޔ��̃N���A
	WFLBY_EVENTWKCL_Clean( &p_wk->tmp );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���荞�݃C�x���g���I�������̂��`�F�b�N����
 *
 *	@param	cp_wk	�C�x���g���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EVENTWK_CheckIntrEnd( const WFLBY_EVENTWK* cp_wk )
{
	if( cp_wk->tmp.cp_data == NULL ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���s����C�x���g������̂��`�F�b�N
 *
 *	@param	cp_wk	 �C�x���g���[�N
 *
 *	@retval	TRUE	���s����
 *	@retval	FALSE	���s���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_EVENTWK_CheckEventDo( const WFLBY_EVENTWK* cp_wk )
{
	if( cp_wk->data.cp_data == NULL ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�f�[�^�Ƀf�[�^��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 *	@param	p_param		�p�����[�^
 *	@param	seq			�V�[�P���X
 *	@param	p_evwk		�C�x���g���[�J�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENTWKCL_SetData( WFLBY_EVENTWK_LOCAL* p_wk, const WFLBY_EVENT_DATA* cp_data, void* p_param, u32 seq, void* p_evwk )
{
	p_wk->cp_data	= cp_data;
	p_wk->seq		= seq;
	p_wk->p_wk		= p_evwk;
	p_wk->p_param	= p_param;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�f�[�^�̃f�[�^���N���[������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_EVENTWKCL_Clean( WFLBY_EVENTWK_LOCAL* p_wk )
{
	p_wk->cp_data	= NULL;
	p_wk->seq		= 0;
	p_wk->p_wk		= NULL;
	p_wk->p_param	= NULL;
}


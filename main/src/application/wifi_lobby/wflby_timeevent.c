//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_timeevent.c
 *	@brief		�^�C���C�x���g
 *	@author		tomoya takahshi
 *	@data		2008.01.26
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "wflby_timeevent.h"
#include "wflby_light.h"
#include "wflby_float.h"
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

#ifdef PM_DEBUG
//#define WFLBY_DEBUG_LIGHT_CONT
#endif

#ifdef WFLBY_DEBUG_LIGHT_CONT
static s32 s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT;
static s32 s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT;
#endif


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�^�C���C�x���g���[�N
//=====================================
typedef struct _WFLBY_TIMEEVENT{
	WFLBY_ROOMWK*		p_rmwk;
	WFLBY_SYSTEM*		p_system;
	WFLBY_3DMAPOBJ_CONT*p_mapobjcont;
	WFLBY_3DOBJCONT*	p_3dobjcont;
	WFLBY_LIGHTWK*		p_light;
	WFLBY_FLOAT_CONT*	p_floatcont;
	WFLBY_FIRE_CONT*	p_firecont;
}WFLBY_TIMEEVENT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static u32 WFLBY_TIMEEVENT_GetNeonRoom( PPW_LOBBY_TIME_EVENT event );
static u32 WFLBY_TIMEEVENT_GetNeonFloor( PPW_LOBBY_TIME_EVENT event );
static u32 WFLBY_TIMEEVENT_GetNeonMonu( PPW_LOBBY_TIME_EVENT event );

static void WFLBY_TIMEEVENT_AplLightOff( WFLBY_TIMEEVENT* p_wk );

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���C�x���g�Ǘ��V�X�e��	������
 *
 *	@param	heapID		�q�[�v
 *	@param	p_rmwk		�������[�N
 */
//-----------------------------------------------------------------------------
WFLBY_TIMEEVENT* WFLBY_TIMEEVENT_Init( u32 heapID, WFLBY_ROOMWK* p_rmwk )
{
	WFLBY_TIMEEVENT* p_wk;
	u32 room, floor, monu;
	u32 hanabi, lock, parade, neon, close, minigame;
	

	p_wk = sys_AllocMemory( heapID, sizeof(WFLBY_TIMEEVENT) );
	memset( p_wk, 0, sizeof(WFLBY_TIMEEVENT) );

	p_wk->p_rmwk		= p_rmwk;
	p_wk->p_system		= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_wk->p_mapobjcont	= WFLBY_ROOM_GetMapObjCont( p_rmwk );
	p_wk->p_3dobjcont	= WFLBY_ROOM_GetObjCont( p_rmwk );

	room	= WFLBY_TIMEEVENT_GetNeonRoom( WFLBY_SYSTEM_Event_GetNeonRoom( p_wk->p_system ) );
	floor	= WFLBY_TIMEEVENT_GetNeonFloor( WFLBY_SYSTEM_Event_GetNeonFloor( p_wk->p_system ) );
	monu	= WFLBY_TIMEEVENT_GetNeonMonu( WFLBY_SYSTEM_Event_GetNeonMonu( p_wk->p_system ) );

	//  ���C�g�Ǘ��V�X�e��������
	p_wk->p_light		= WFLBY_LIGHT_Init( heapID, room, floor, monu );

	// �t���[�g�Ǘ��V�X�e��������
	p_wk->p_floatcont	= WFLBY_FLOAT_CONT_Init( p_rmwk, heapID );

	// �ԉΊǗ��V�X�e��������
	p_wk->p_firecont	= WFLBY_FIRECONT_Init( p_wk->p_system, p_wk->p_mapobjcont, heapID );

	// �l���̉e�̐F
	WFLBY_3DOBJCONT_InitShadowAlpha( p_wk->p_3dobjcont, room );


	// �e�C�x���g��Ԏ擾
	hanabi	= WFLBY_SYSTEM_Event_GetHanabi( p_wk->p_system );
	lock	= WFLBY_SYSTEM_Event_GetLock( p_wk->p_system );
	parade	= WFLBY_SYSTEM_Event_GetParade( p_wk->p_system );
	neon	= WFLBY_SYSTEM_Event_GetNeon( p_wk->p_system );
	close	= WFLBY_SYSTEM_Event_GetEndCM( p_wk->p_system );
	minigame= WFLBY_SYSTEM_Event_GetMiniGameStop( p_wk->p_system );
	
	// �����̏��
	{
		if( hanabi == WFLBY_EVENT_HANABI_AFTER ){
			// �ԉΌ�Ȃ̂ŁA�����̖����������
			WFLBY_3DMAPOBJCONT_MAP_OffDouzouLight( p_wk->p_mapobjcont );
		}
	}

	// �����C�g�̏��
	{
		// �ԉ΂��n�܂��Ă���Ƃ�
		if(  hanabi != WFLBY_EVENT_HANABI_PLAY ){

			// �ԉΑO�Ȃ�ŏ���������o��
			if( (hanabi == WFLBY_EVENT_HANABI_BEFORE) && (neon == TRUE) ){
				WFLBY_3DMAPOBJCONT_MAP_SetFloorLight( p_wk->p_mapobjcont, WFLBY_3DMAPOBJ_CONT_LAMP_YURE  );
			}
		}
	}

	// ��̏��
	{
		// �p���[�h���n�܂��Ă���Ƃ�
		if( parade == TRUE ){
			WFLBY_3DMAPOBJCONT_MAP_OnPoll( p_wk->p_mapobjcont );
		}
	}

	// �N���[�Y
	{
		if( close == TRUE ){
			// �A�v���̖����������
			WFLBY_TIMEEVENT_AplLightOff( p_wk );
		}
	}


	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���C�x���g�Ǘ��V�X�e��	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_TIMEEVENT_Exit( WFLBY_TIMEEVENT* p_wk )
{
	// �ԉΊǗ��V�X�e���j��
	WFLBY_FIRECONT_Exit( p_wk->p_firecont );

	// �t���[�g�Ǘ��V�X�e���j��
	WFLBY_FLOAT_CONT_Exit( p_wk->p_floatcont );
	
	// ���C�g�Ǘ��V�X�e���j��
	WFLBY_LIGHT_Exit( p_wk->p_light );

	// ���[�N�j��
	sys_FreeMemoryEz( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���C�x���g�Ǘ��V�X�e��	���C��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_TIMEEVENT_Main( WFLBY_TIMEEVENT* p_wk )
{
#ifdef WFLBY_DEBUG_LIGHT_CONT
	if( sys.cont & PAD_BUTTON_A ){
		if( sys.trg & PAD_KEY_UP ){
			s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT = (s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT + 1) % WFLBY_LIGHT_NEON_ROOMNUM;
			OS_TPrintf( "room type %d\n", s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT );
			WFLBY_LIGHT_SetRoom( p_wk->p_light, s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT );
		}
		else if( sys.trg & PAD_KEY_DOWN ){
			s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT --;
			if( s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT < 0 ){
				s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT += WFLBY_LIGHT_NEON_ROOMNUM;
			}
			OS_TPrintf( "room type %d\n", s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT );
			WFLBY_LIGHT_SetRoom( p_wk->p_light, s_WFLBY_DEBUG_LIGHT_CONT_ROOM_COUNT );
		}
	}

	if( sys.cont & PAD_BUTTON_Y ){
		if( sys.trg & PAD_KEY_UP ){
			s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT = (s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT + 1) % WFLBY_LIGHT_NEON_FLOORNUM;
			OS_TPrintf( "floor type %d\n", s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT );
			WFLBY_LIGHT_SetFloor( p_wk->p_light, s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT );
		}
		else if( sys.trg & PAD_KEY_DOWN ){
			s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT --;
			if( s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT < 0 ){
				s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT += WFLBY_LIGHT_NEON_FLOORNUM;
			}
			OS_TPrintf( "floor type %d\n", s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT );
			WFLBY_LIGHT_SetFloor( p_wk->p_light, s_WFLBY_DEBUG_LIGHT_CONT_FLOOR_COUNT );
		}
	}
#endif
	
	// �C�x���g�����`�F�b�N
	// �����l�I��
	if( WFLBY_SYSTEM_FLAG_GetEventStart( p_wk->p_system, WFLBY_EVENT_CHG_NEON_ROOM ) ){
		u32 room;
		room = WFLBY_TIMEEVENT_GetNeonRoom( WFLBY_SYSTEM_Event_GetNeonRoom( p_wk->p_system ) );
		WFLBY_LIGHT_SetRoom( p_wk->p_light, room );
		// �l���̉e�̐F
		WFLBY_3DOBJCONT_SetShadowAlpha( p_wk->p_3dobjcont, room );
	}
	// ���l�I��
	if( WFLBY_SYSTEM_FLAG_GetEventStart( p_wk->p_system, WFLBY_EVENT_CHG_NEON_FLOOR ) ){
		u32 floor;
		floor = WFLBY_TIMEEVENT_GetNeonFloor( WFLBY_SYSTEM_Event_GetNeonFloor( p_wk->p_system ) );
		WFLBY_LIGHT_SetFloor( p_wk->p_light, floor );
	}
	// ���j�������g�l�I��
	if( WFLBY_SYSTEM_FLAG_GetEventStart( p_wk->p_system, WFLBY_EVENT_CHG_NEON_MONU ) ){
		u32 monu;
		monu = WFLBY_TIMEEVENT_GetNeonMonu( WFLBY_SYSTEM_Event_GetNeonMonu( p_wk->p_system ) );
		WFLBY_LIGHT_SetMonu( p_wk->p_light, monu );
	}

	// �l�I���C�x���g���n�܂��Ă��āA�ԉΒ��łȂ��Ƃ�
	if( (WFLBY_SYSTEM_Event_GetNeon( p_wk->p_system ) == TRUE) && 
		(WFLBY_SYSTEM_Event_GetHanabi( p_wk->p_system ) == WFLBY_EVENT_HANABI_BEFORE) ){
		// ���C�g�����Ă��Ȃ��Ȃ�A���C�gON
		if( WFLBY_3DMAPOBJCONT_MAP_CheckFloorLightYure( p_wk->p_mapobjcont ) == FALSE ){
			// ���C�gON
			WFLBY_3DMAPOBJCONT_MAP_SetFloorLight( p_wk->p_mapobjcont, WFLBY_3DMAPOBJ_CONT_LAMP_ON );
		}
	}

	// �ԉΊJ�n
	if( WFLBY_SYSTEM_FLAG_GetEventStart( p_wk->p_system, WFLBY_EVENT_CHG_FIRE ) ){
		if( WFLBY_SYSTEM_Event_GetHanabi( p_wk->p_system ) == WFLBY_EVENT_HANABI_PLAY ){
			// �ԉΊJ�n
			// �����̃��C�g��������OFF�ɂȂ�܂�
			WFLBY_3DMAPOBJCONT_MAP_OffDouzouLight( p_wk->p_mapobjcont );

			// �����C�gOFF
			WFLBY_3DMAPOBJCONT_MAP_SetFloorLight( p_wk->p_mapobjcont, WFLBY_3DMAPOBJ_CONT_LAMP_OFF );

		}else{
			// �ԉΒ�~
		}
	}
		

	// �p���[�h�J�n
	if( WFLBY_SYSTEM_FLAG_GetEventStart( p_wk->p_system, WFLBY_EVENT_CHG_PARADE ) ){
		if( WFLBY_SYSTEM_Event_GetParade( p_wk->p_system ) == TRUE ){

			// ����A�j���J�n
			WFLBY_3DMAPOBJCONT_MAP_OnPoll( p_wk->p_mapobjcont );
		}
	}

	// �N���[�Y�b�l�J�n
	if( WFLBY_SYSTEM_Event_GetEndCM( p_wk->p_system ) == TRUE ){
		// �g�̃A�j����~
		WFLBY_3DMAPOBJCONT_MAP_OffPoll( p_wk->p_mapobjcont );
		

		// �A�v���̖����������
		WFLBY_TIMEEVENT_AplLightOff( p_wk );
	}

	
	// ���C�g�Ǘ����C��
	WFLBY_LIGHT_Main( p_wk->p_light );

	// �t���[�g�Ǘ����C��
	WFLBY_FLOAT_CONT_Main( p_wk->p_floatcont );

	// �ԉΊǗ����C��
	WFLBY_FIRECONT_Main( p_wk->p_firecont );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g���A�j�������`�F�b�N
 *
 *	@param	cp_wk		�V�X�e��
 *	@param	floatidx	�t���[�g�C���f�b�N�X
 *	@param	anmidx		�A�j���C���f�b�N�X		WFLBY_FLOAT_ANM_NUM
 *
 *	@retval	TRUE	�A�j����
 *	@retval	FALSE	�A�j�����ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_TIMEEVENT_FLOAT_CheckAnm( const WFLBY_TIMEEVENT* cp_wk, u32 floatidx, WFLBY_FLOAT_ANM_TYPE anmidx )
{
	return WFLBY_FLOAT_CONT_CheckAnm( cp_wk->p_floatcont, floatidx, anmidx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ԉ�SE������Ă������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_TIMEEVENT_FIRE_StopSe( WFLBY_TIMEEVENT* p_wk )
{
	WFLBY_FIRECONT_StopFireSe( p_wk->p_firecont );
}





//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�����l�I���i���o�[���擾����
 *
 *	@param	event	�C�x���g
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_TIMEEVENT_GetNeonRoom( PPW_LOBBY_TIME_EVENT event )
{
	switch( event ){
    case PPW_LOBBY_TIME_EVENT_NEON_A0:           ///< �����l�I���~��0�B
    case PPW_LOBBY_TIME_EVENT_NEON_A1:           ///< �����l�I���~��1�B
    case PPW_LOBBY_TIME_EVENT_NEON_A2:           ///< �����l�I���~��2�B
    case PPW_LOBBY_TIME_EVENT_NEON_A3:           ///< �����l�I���~��3�B
    case PPW_LOBBY_TIME_EVENT_NEON_A4:           ///< �����l�I���~��4�B
    case PPW_LOBBY_TIME_EVENT_NEON_A5:           ///< �����l�I���~��5�B
		
		return event - PPW_LOBBY_TIME_EVENT_NEON_A0;

	default:
		//  �C�x���g�i���o�[����[
		GF_ASSERT( 0 );
		break;
	}
	return 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���l�I���i���o�[���擾����
 *
 *	@param	event	�C�x���g
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_TIMEEVENT_GetNeonFloor( PPW_LOBBY_TIME_EVENT event )
{
	switch( event ){
    case PPW_LOBBY_TIME_EVENT_NEON_B0:           ///< ���l�I���~��0�B
    case PPW_LOBBY_TIME_EVENT_NEON_B1:           ///< ���l�I���~��1�B
    case PPW_LOBBY_TIME_EVENT_NEON_B2:           ///< ���l�I���~��2�B
    case PPW_LOBBY_TIME_EVENT_NEON_B3:           ///< ���l�I���~��3�B
		
		return event - PPW_LOBBY_TIME_EVENT_NEON_B0;

	default:
		//  �C�x���g�i���o�[����[
		GF_ASSERT( 0 );
		break;
	}
	return 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�������g�l�I���i���o�[���擾����
 *
 *	@param	event	�C�x���g
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_TIMEEVENT_GetNeonMonu( PPW_LOBBY_TIME_EVENT event )
{
	switch( event ){
    case PPW_LOBBY_TIME_EVENT_NEON_C0:           ///< ���j�������g�~��0�B
    case PPW_LOBBY_TIME_EVENT_NEON_C1:           ///< ���j�������g�~��1�B
    case PPW_LOBBY_TIME_EVENT_NEON_C2:           ///< ���j�������g�~��2�B
    case PPW_LOBBY_TIME_EVENT_NEON_C3:           ///< ���j�������g�~��3�B
		
		return event - PPW_LOBBY_TIME_EVENT_NEON_C0;

	default:
		//  �C�x���g�i���o�[����[
		GF_ASSERT( 0 );
		break;
	}
	return 0;
}





//----------------------------------------------------------------------------
/**
 *	@brief	�A�v���P�[�V�������C�g��OFF����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_TIMEEVENT_AplLightOff( WFLBY_TIMEEVENT* p_wk )
{
	// ���C�g�Q��OFF�ɂ���
	NNS_G3dGlbLightColor( 2, GX_RGB( 0,0,0 ) );
}

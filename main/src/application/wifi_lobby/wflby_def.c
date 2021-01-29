//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_def.c
 *	@brief		WiFi���r�[	���ʒʐM���[�N�A�N�Z�X�֐�
 *	@author		tomoya takahashi
 *	@data		2007.09.18
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "wflby_def.h"

#include "savedata/wifihistory.h"

#include "msgdata/msg_wifi_system.h"

#include "application/wifi_country.h"

#include "communication/comm_state.h"
#include "wifi/dwc_rap.h"

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

#ifdef WFLBY_DEBUG_ROOM_WLDTIMER_AUTO
BOOL WFLBY_DEBUG_ROOM_WFLBY_TIMER_AUTO = FALSE;
#endif

#ifdef WFLBY_DEBUG_ROOM_MINIGAME_AUTO
BOOL WFLBY_DEBUG_ROOM_MINIGAME_AUTO_FLAG = FALSE;
#endif


//-----------------------------------------------------------------------------
/**
 *				���ԃf�[�^
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	���Ԃ�ݒ�
 *
 *	@param	p_time			�^�C��
 *	@param	cp_settime		����
 */
//-----------------------------------------------------------------------------
void WFLBY_TIME_Set( WFLBY_TIME* p_time, const s64* cp_settime )
{
	RTCDate data;
	RTCTime time;
	RTC_ConvertSecondToDateTime( &data, &time, *cp_settime );
	p_time->hour	= time.hour;
	p_time->minute	= time.minute;
	p_time->second	= time.second;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ԃ̑����Z	
 *
 *	@param	cp_time		���삷�鎞��
 *	@param	cp_add		�������ގ���
 *	@param	p_ans		����
 */
//-----------------------------------------------------------------------------
void WFLBY_TIME_Add( const WFLBY_TIME* cp_time, const WFLBY_TIME* cp_add, WFLBY_TIME* p_ans )
{
	u32 add;
	WFLBY_TIME time;
	WFLBY_TIME addtime;

	time = *cp_time;
	addtime = *cp_add;
	
	// �b�̑����Z
	time.second		+= addtime.second;
	add				= time.second / 60;
	time.minute		+= add;
	p_ans->second	= time.second	% 60;

	// ���̑����Z
	time.minute		+= addtime.minute;
	add				= time.minute / 60;
	time.hour		+= add;
	p_ans->minute	= time.minute	% 60;

	// ���̑����Z
	time.hour	+= addtime.hour;
	p_ans->hour	= time.hour	% 24;

}

//----------------------------------------------------------------------------
/**
 *	@brief	���Ԃ̈����Z	
 *
 *	@param	p_time		���삷�鎞��
 *	@param	cp_sub		��������
 *	@param	p_ans		����
 */
//-----------------------------------------------------------------------------
void WFLBY_TIME_Sub( const WFLBY_TIME* cp_time, const WFLBY_TIME* cp_sub, WFLBY_TIME* p_ans )
{
	u32 sub;
	s32 sum;
	WFLBY_TIME time;
	WFLBY_TIME subtime;

	time	= *cp_time;
	subtime	= *cp_sub;

	// �b�̈����Z
	sum = ((s8)time.second - (s8)subtime.second);
	if( sum < 0  ){
		sub = ((-sum) / 60) + 1;
		time.second += 60*sub;
		time.minute -= sub;
	}
	p_ans->second = time.second - subtime.second;


	// ���̈����Z
	sum = ((s8)time.minute - (s8)subtime.minute);
	if( sum < 0 ){
		sub = ((-sum) / 60) + 1;	// �Œ�ł��P�͈�������+1
		time.minute	+= 60*sub;
		time.hour	-= sub;
	}
	p_ans->minute = time.minute - subtime.minute;

	// ��
	sum = ((s8)time.hour - (s8)subtime.hour);
	if( sum < 0 ){
		sub = ((-sum) / 24) + 1;	// �Œ�ł��P�͈�������+1
		time.hour	+= 24*sub;
	}
	p_ans->hour = time.hour - subtime.hour;

}




//-----------------------------------------------------------------------------
/**
 *				���E�n�}�f�[�^
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�f�[�^�ݒ�
 *
 *	@param	p_data		���[�N
 *	@param	nation		��ID
 *	@param	area		�n��ID
 *	@param	outside		�ގ��҂��ǂ���
 */
//-----------------------------------------------------------------------------
void WFLBY_WLDTIMER_SetData( WFLBY_WLDTIMER* p_data, u16 nation, u8 area, BOOL outside )
{
	int i;
	BOOL set_data;
	u32 area_max;

	// ���f�[�^�Ȃ��Ȃ����Ȃ�
	if( nation == WFLBY_WLDDATA_NATION_NONE ){
		return ;
	}

	area_max = WIFI_COUNTRY_CountryCodeToPlaceIndexMax( nation );

	// �s���ȍ��Ȃ����Ȃ�
	if( area > area_max ){
		return ;
	}

	set_data = FALSE;
	for( i=0; i<WFLBY_WLDTIMER_DATA_MAX; i++ ){
		
		// �f�[�^���Ȃ��Ƃ���܂ł����炻���ɓ����
		if( p_data->data[ i ].flag == FALSE){
			set_data = TRUE;
		}else{
			// ���ƃG���A�����Ԃ��Ă���A�������ގ��҂łȂ��Ƃ������ݒ肷��
			if( (p_data->data[i].nation == nation) &&
				(p_data->data[i].area	== area)){
				if( outside == TRUE ){
					return ;
				}else{
					set_data = TRUE;
				}
			}
		}
		
		// �ݒ�
		if( set_data ){
			p_data->data[i].nation	= nation;
			p_data->data[i].area	= area;
			p_data->data[i].outside	= outside;
			p_data->data[i].flag	= TRUE;
			return ;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ID�̎擾
 */
//-----------------------------------------------------------------------------
u16 WFLBY_WLDTIMER_GetNation( const WFLBY_WLDTIMER* cp_data, u8 index )
{
	GF_ASSERT( index < WFLBY_WLDTIMER_DATA_MAX );
	GF_ASSERT( cp_data->data[index].flag == TRUE );
	
	return cp_data->data[index].nation;
}
//----------------------------------------------------------------------------
/**
 *	@brief	�n��ID�̎擾
 */
//-----------------------------------------------------------------------------
u8 WFLBY_WLDTIMER_GetArea( const WFLBY_WLDTIMER* cp_data, u8 index )
{
	GF_ASSERT( index < WFLBY_WLDTIMER_DATA_MAX );
	GF_ASSERT( cp_data->data[index].flag == TRUE );
	
	return cp_data->data[index].area;
}
//----------------------------------------------------------------------------
/**
 *	@brief	�ގ��t���O�̎擾
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_WLDTIMER_GetOutSide( const WFLBY_WLDTIMER* cp_data, u8 index )
{
	GF_ASSERT( index < WFLBY_WLDTIMER_DATA_MAX );
	GF_ASSERT( cp_data->data[index].flag == TRUE );
	
	return cp_data->data[index].outside;
}
//----------------------------------------------------------------------------
/**
 *	@brief	�f�[�^�L���t���O�̎擾
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_WLDTIMER_GetDataFlag( const WFLBY_WLDTIMER* cp_data, u8 index )
{
	GF_ASSERT( index < WFLBY_WLDTIMER_DATA_MAX );
	
	return cp_data->data[index].flag;
}








//-----------------------------------------------------------------------------
/**
 *			���C�g���\�[�X����
 *				���C�g�̉e�����󂯂郂�f���͂�����ʂ�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	���C�g���\�[�X����		���C�g�̉e�����󂯂郂�f���͂�����ʂ�
 */
//-----------------------------------------------------------------------------
void WFLBY_LIGHT_SetUpMdl( void* p_file )
{
	NNSG3dResMdlSet* p_mdlset;
	NNSG3dResMdl* p_res;

	p_mdlset = NNS_G3dGetMdlSet( p_file );
	p_res = NNS_G3dGetMdlByIdx( p_mdlset, 0 );
	
	NNS_G3dMdlUseGlbDiff( p_res );
	NNS_G3dMdlUseGlbAmb( p_res );
	NNS_G3dMdlUseGlbSpec( p_res );
	NNS_G3dMdlUseGlbEmi( p_res );
}



//-----------------------------------------------------------------------------
/**
 *			�n�悪����̂��`�F�b�N
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�n�悪���邩�`�F�b�N
 *
 *	@param	nation		���R�[�h
 *	@param	area		�n��R�[�h
 *	
 *	@retval	TRUE	����
 *	@retval	FALSE	�Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_AREA_Check( u16 nation, u16 area )
{
	u32 area_max;

	// �n��������Ă���̂��`�F�b�N
	area_max = WIFI_COUNTRY_CountryCodeToPlaceIndexMax( nation );

	// �n�搔0�̏ꍇ0�Ȃ�OK
	if( area_max == 0 ){
		if( area == 0 ){
			return TRUE;
		}
	}
	
	// ���n�搔0���傫���Ƃ���1�`���̒l�܂�OK
	if( (area >= 1) && (area <= area_max) ){
		return TRUE;
	}

	return FALSE;
}





//-----------------------------------------------------------------------------
/**
 *			�L����Z�^�C�v�萔����퓬�p�Z�^�C�v�萔�����߂�֐�
 */
//-----------------------------------------------------------------------------
u32 WFLBY_BattleWazaType_Get( WFLBY_POKEWAZA_TYPE type )
{
	static const u8 sc_BATTLE_WAZA_TYPE_DATA[ WFLBY_POKEWAZA_TYPE_NUM ] = {
		HATE_TYPE,			//	WFLBY_POKEWAZA_TYPE_NONE,	// �I�΂Ȃ�
		NORMAL_TYPE,		//	WFLBY_POKEWAZA_TYPE_NORMAL,
		BATTLE_TYPE,		//	WFLBY_POKEWAZA_TYPE_BATTLE,
		HIKOU_TYPE,			//	WFLBY_POKEWAZA_TYPE_HIKOU,
		POISON_TYPE,		//	WFLBY_POKEWAZA_TYPE_POISON,
		JIMEN_TYPE,			//	WFLBY_POKEWAZA_TYPE_JIMEN,
		IWA_TYPE,			//	WFLBY_POKEWAZA_TYPE_IWA,
		MUSHI_TYPE,			//	WFLBY_POKEWAZA_TYPE_MUSHI,
		GHOST_TYPE,			//	WFLBY_POKEWAZA_TYPE_GHOST,
		METAL_TYPE,			//	WFLBY_POKEWAZA_TYPE_METAL,
		FIRE_TYPE,			//	WFLBY_POKEWAZA_TYPE_FIRE,
		WATER_TYPE,			//	WFLBY_POKEWAZA_TYPE_WATER,
		KUSA_TYPE,			//	WFLBY_POKEWAZA_TYPE_KUSA,
		ELECTRIC_TYPE,		//	WFLBY_POKEWAZA_TYPE_ELECTRIC,
		SP_TYPE,			//	WFLBY_POKEWAZA_TYPE_SP,
		KOORI_TYPE,			//	WFLBY_POKEWAZA_TYPE_KOORI,
		DRAGON_TYPE,		//	WFLBY_POKEWAZA_TYPE_DRAGON,
		AKU_TYPE,			//	WFLBY_POKEWAZA_TYPE_AKU,
	};
	return sc_BATTLE_WAZA_TYPE_DATA[ type ];
}




//-----------------------------------------------------------------------------
/**
 *			�G���[����
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�G���[�i���o�[����G���[���b�Z�[�W���擾
 *
 *	@param	errno	�G���[�i���o�[	CommStateGetWifiErrNo
 *	
 *	@return	���b�Z�[�W�i���o�[
 */
//-----------------------------------------------------------------------------
int WFLBY_ERR_GetStrID( int errno,int errtype )
{
	int err_type;
	int msgno;

	err_type = mydwc_errorType( errno,errtype );
	
    if((err_type == 11) || (errno == ERRORCODE_0)){
        msgno = dwc_error_0015;
    }
    else if(errno == ERRORCODE_HEAP){
        msgno = dwc_error_0014;
    }
    else{
		if( err_type >= 0 ){
	        msgno = dwc_error_0001 + err_type;
		}else{
	        msgno = dwc_error_0012;
		}
    }

	return msgno;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���[�i���o�[����G���[���b�Z�[�W���擾
 *
 *	@param	errno	�G���[�i���o�[
 *
 *	@return	�G���[�^�C�v
 */
//-----------------------------------------------------------------------------
WFLBY_ERR_TYPE WFLBY_ERR_GetErrType( int errno,int errtype )
{
	int err_type;
	int ret;

	err_type = mydwc_errorType( errno, errtype );
	
    if( errno == ERRORCODE_0 ){
		err_type = 11;
    }
    else if(errno == ERRORCODE_HEAP){
		err_type = 12;
    }
    
    switch(err_type){
      case 1:
      case 4:
      case 5:
      case 11:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
		ret = WFLBY_ERR_TYPE_RETRY;
		break; 

      case 0:
      case 2:
      case 3:
      default:
		ret = WFLBY_ERR_TYPE_END;
		break; 
    }

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	Wi-Fi�G���[�`�F�b�N
 *
 *	@retval	TRUE	�G���[����
 *	@retval	FALSE	�G���[�Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ERR_CheckError( void )
{
	if( CommStateIsWifiError() || CommStateWifiLobbyError() ){
		return TRUE;
	}
	return FALSE;
}

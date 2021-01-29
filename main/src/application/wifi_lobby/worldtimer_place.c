//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		worldtimer_place.c
 *	@brief		���E���v	�n�掞�Ԏ擾����	�������������邽�߂ɂ��̏�������wflby_common�I�[�o�[���C�Ɉړ�
 *	@author		tomoya takahashi
 *	@data		2008.05.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "system/arc_util.h"

#include "application/wifi_country.h"
#include "application/wifi_earth/wifi_earth_place.naix"
#include "application/wifi_lobby/worldtimer_place.h"

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

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

static void WLDTIMER_EarthListGetNationData( ARCHANDLE* p_handle, u32 heapID, u16 nation, u8 area, fx32* p_y );


//----------------------------------------------------------------------------
/**
 *	@brief	�n��̎��Ԃ��擾����
 *
 *	@param	nation		��ID
 *	@param	area		�n��ID
 *	@param	gmt			GMT
 *	@param	heapID		�q�[�vID
 *
 *	@return	���̎���
 */
//-----------------------------------------------------------------------------
WFLBY_TIME WFLBY_WORLDTIMER_PLACE_GetPlaceTime( u16 nation, u8 area, WFLBY_TIME gmt, u32 heapID )
{
	// �n�_���X�g
	fx32 gmt_y;
	fx32 place_y;
	u16 rota;
	s32 hour_dif;
	WFLBY_TIME time;
	ARCHANDLE* p_handle;

	p_handle = ArchiveDataHandleOpen( ARC_WIFI_EARCH_PLACE, heapID );
	

	// GMT�n��̉�]�p�x�擾
	WLDTIMER_EarthListGetNationData( p_handle, heapID, WLDTIMER_GMT_NATIONID,  WLDTIMER_GMT_AREAID, &gmt_y );
	
	// �n��̉�]�p�x�擾
	WLDTIMER_EarthListGetNationData( p_handle, heapID, nation, area, &place_y );

	// �������玞�Ԃ̌덷�����߂�덷�͎��ԒP�ʂōs��
	rota = gmt_y - place_y;
	hour_dif = (rota*24)/0xffff;


	// �}�C�i�X�̒l��������A�I�[�o�[�����肵�Ă���͂��Ȃ̂Œ���
	if( hour_dif < 0 ){
		hour_dif += 24;
	}
	if( hour_dif >= 24 ){
		hour_dif %= 24;
	}

	// ���Ԃ����߂�
	memset( &time, 0, sizeof(WFLBY_TIME) );
	time.hour = hour_dif;
	WFLBY_TIME_Add( &time, &gmt, &time );

	ArchiveDataHandleClose( p_handle );

	return time;
}




//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	���鍑��Y��]�p�x�����󂯎��
 *
 *	@param	p_handle		�A�[�J�C�u�n���h��
 *	@param	heapID			�q�[�vID
 *	@param	nation			��ID
 *	@param	area			�n��ID
 *	@param	p_y				����]�i�[��
 */
//-----------------------------------------------------------------------------
static void WLDTIMER_EarthListGetNationData( ARCHANDLE* p_handle, u32 heapID, u16 nation, u8 area, fx32* p_y )
{
	BOOL set_ok = FALSE;
	
	{//�n�_�}�[�N��]�������i���f�[�^�o�C�i���f�[�^���[�h�j
		void* filep;
		EARTH_DATA_NATION* listp;
		u32	size;
		int	listcount;

		filep = ArcUtil_HDL_LoadEx( p_handle, NARC_wifi_earth_place_place_pos_wrd_dat, 
								FALSE, heapID, ALLOC_TOP, &size );

		listp = (EARTH_DATA_NATION*)filep;	//�t�@�C���ǂݍ��ݗp�ɕϊ�
		listcount = size/6;				//�n�_���擾�i�f�[�^���F�P�n�_�ɂ��U�o�C�g�j

		GF_ASSERT( nation < listcount );

		// �n�悪����̂��`�F�b�N
		if( listp[ nation ].flag != 2 ){
			*p_y = listp[ nation ].y;
			set_ok = TRUE;
		}
		
		sys_FreeMemoryEz(filep);
	}
	
	if( set_ok == FALSE ){//�n�_�}�[�N��]�������i�n��f�[�^�o�C�i���f�[�^���[�h�j
		void* filep;
		EARTH_DATA_AREA* listp;
		u32	size, data_id;
		int	index,listcount;

		index = WIFI_COUNTRY_CountryCodeToDataIndex( nation );	//1orgin

		data_id = WIFI_COUNTRY_DataIndexToPlaceDataID( index );
		filep = ArcUtil_HDL_LoadEx( p_handle, data_id, FALSE, 
								heapID, ALLOC_TOP, &size );

		listp = (EARTH_DATA_AREA*)filep;	//�t�@�C���ǂݍ��ݗp�ɕϊ�
		listcount = size/4;		//�n�_���擾�i�f�[�^���F�P�n�_�ɂ��S�o�C�g�j

		if( area < listcount ){
			*p_y = listp[ area ].y;
		}else{
			GF_ASSERT(0);
			*p_y = listp[ 0 ].y;
		}

		sys_FreeMemoryEz(filep);
	}
}


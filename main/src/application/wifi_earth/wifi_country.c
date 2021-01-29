//============================================================================================
/**
 * @file	wifi_country.c
 * @brief	WI-FI�n����ʂŎg�p���镶���񓙂̃f�[�^���O�����J���邽�߂̎d�g��
 * @author	taya
 * @date	2006.04.22
 */
//============================================================================================
#include "common.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_wifi_place_msg_world.h"
#include "wifi_earth_place.naix"

#include "application\wifi_country.h"

#include "msgdata/msg_wifi_place_msg_ARG.h"
#include "msgdata/msg_wifi_place_msg_AUS.h"
#include "msgdata/msg_wifi_place_msg_BRA.h"
#include "msgdata/msg_wifi_place_msg_CAN.h"
#include "msgdata/msg_wifi_place_msg_CHN.h"
#include "msgdata/msg_wifi_place_msg_DEU.h"
#include "msgdata/msg_wifi_place_msg_ESP.h"
#include "msgdata/msg_wifi_place_msg_FIN.h"
#include "msgdata/msg_wifi_place_msg_FRA.h"
#include "msgdata/msg_wifi_place_msg_GBR.h"
#include "msgdata/msg_wifi_place_msg_IND.h"
#include "msgdata/msg_wifi_place_msg_ITA.h"
#include "msgdata/msg_wifi_place_msg_JPN.h"
#include "msgdata/msg_wifi_place_msg_NOR.h"
#include "msgdata/msg_wifi_place_msg_POL.h"
#include "msgdata/msg_wifi_place_msg_RUS.h"
#include "msgdata/msg_wifi_place_msg_SWE.h"
#include "msgdata/msg_wifi_place_msg_USA.h"

//�f�[�^�\���́i���n��e�[�u���f�[�^�j
typedef struct EARTH_AREATABLE_tag
{
	u8		nationID;
	u8		place_dataID;
	u16		msg_arcID;
	
	u8		place_index_max;		///<�n�於�̍ő吔
	u8		padding[3];		//�p�f�B���O
}EARTH_AREATABLE;

static const EARTH_AREATABLE NationFlag_to_AreaID[] = {
	{//�_�~�[
		country000,
		NARC_wifi_earth_place_place_pos_wrd_dat,
		NARC_msg_wifi_place_msg_world_dat,
		0
	},
	{
		country009,
		NARC_wifi_earth_place_place_pos_ARG_dat,
		NARC_msg_wifi_place_msg_ARG_dat,
		ARG24
	},
	{
		country012,
		NARC_wifi_earth_place_place_pos_AUS_dat,
		NARC_msg_wifi_place_msg_AUS_dat,
		AUS07
	},
	{
		country028,
		NARC_wifi_earth_place_place_pos_BRA_dat,
		NARC_msg_wifi_place_msg_BRA_dat,
		BRA27
	},
	{
		country036,
		NARC_wifi_earth_place_place_pos_CAN_dat,
		NARC_msg_wifi_place_msg_CAN_dat,
		CAN13
	},
	{
		country043,
		NARC_wifi_earth_place_place_pos_CHN_dat,
		NARC_msg_wifi_place_msg_CHN_dat,
		CHN31
	},
	{
		country077,
		NARC_wifi_earth_place_place_pos_DEU_dat,
		NARC_msg_wifi_place_msg_DEU_dat,
		DEU16
	},
	{
		country193,
		NARC_wifi_earth_place_place_pos_ESP_dat,
		NARC_msg_wifi_place_msg_ESP_dat,
		ESP17
	},
	{
		country070,
		NARC_wifi_earth_place_place_pos_FIN_dat,
		NARC_msg_wifi_place_msg_FIN_dat,
		FIN06
	},
	{
		country071,
		NARC_wifi_earth_place_place_pos_FRA_dat,
		NARC_msg_wifi_place_msg_FRA_dat,
		FRA22
	},
	{
		country219,
		NARC_wifi_earth_place_place_pos_GBR_dat,
		NARC_msg_wifi_place_msg_GBR_dat,
		GBR12
	},
	{
		country094,
		NARC_wifi_earth_place_place_pos_IND_dat,
		NARC_msg_wifi_place_msg_IND_dat,
		IND35
	},
	{
		country101,
		NARC_wifi_earth_place_place_pos_ITA_dat,
		NARC_msg_wifi_place_msg_ITA_dat,
		ITA20
	},
	{
		country103,
		NARC_wifi_earth_place_place_pos_JPN_dat,
		NARC_msg_wifi_place_msg_JPN_dat,
		JPN50
	},
	{
		country156,
		NARC_wifi_earth_place_place_pos_NOR_dat,
		NARC_msg_wifi_place_msg_NOR_dat,
		NOR20
	},
	{
		country166,
		NARC_wifi_earth_place_place_pos_POL_dat,
		NARC_msg_wifi_place_msg_POL_dat,
		POL16
	},
	{
		country172,
		NARC_wifi_earth_place_place_pos_RUS_dat,
		NARC_msg_wifi_place_msg_RUS_dat,
		RUS07
	},
	{
		country199,
		NARC_wifi_earth_place_place_pos_SWE_dat,
		NARC_msg_wifi_place_msg_SWE_dat,
		SWE24
	},
	{
		country220,
		NARC_wifi_earth_place_place_pos_USA_dat,
		NARC_msg_wifi_place_msg_USA_dat,
		USA51
	},
};




//------------------------------------------------------------------
/**
 * �f�[�^�����擾
 *
 * @retval  u32		
 */
//------------------------------------------------------------------
u32 WIFI_COUNTRY_GetDataLen( void )
{
	return NELEMS(NationFlag_to_AreaID);
}

//------------------------------------------------------------------
/**
 * ���R�[�h���f�[�^�C���f�b�N�X�ɕϊ�
 *
 * �f�[�^�C���f�b�N�X�́A�e��t���f�[�^���������邽�߂Ɏg�p����B
 * �f�[�^�C���f�b�N�X���O�̎��A���̍��̃��[�J���n��f�[�^�͑��݂��Ȃ��B
 *
 * @param   countryCode		���R�[�h
 *
 * @retval  u32		�����f�[�^�C���f�b�N�X
 */
//------------------------------------------------------------------
u32 WIFI_COUNTRY_CountryCodeToDataIndex( u32 countryCode )
{
	u32 i;

	for(i=0; i<NELEMS(NationFlag_to_AreaID); i++)
	{
		if(NationFlag_to_AreaID[i].nationID == countryCode)
		{
			return i;
		}
	}

	return 0;

}

//------------------------------------------------------------------
/**
 * ���R�[�h����n�搔���擾
 *
 * @param   countryCode		���R�[�h
 *
 * @retval  �n�搔(�n�悪���݂��Ȃ����̏ꍇ��0)
 */
//------------------------------------------------------------------
u32 WIFI_COUNTRY_CountryCodeToPlaceIndexMax( u32 countryCode )
{
	u32 i;

	for(i=0; i<NELEMS(NationFlag_to_AreaID); i++)
	{
		if(NationFlag_to_AreaID[i].nationID == countryCode)
		{
			return NationFlag_to_AreaID[i].place_index_max;
		}
	}

	return 0;

}

//------------------------------------------------------------------
/**
 * ���R�[�h���烍�[�J���n�於������̃��b�Z�[�W�f�[�^ID���擾
 *
 * @param   countryCode		���R�[�h
 *
 * @retval  u32		�n�於������̃��b�Z�[�W�f�[�^ID�iARC_MSG���j
 */
//------------------------------------------------------------------
u32 WIFI_COUNTRY_CountryCodeToPlaceMsgDataID( u32 countryCode )
{
	u32 dataIndex = WIFI_COUNTRY_CountryCodeToDataIndex( countryCode );
	return WIFI_COUNTRY_DataIndexToPlaceMsgDataID( dataIndex );
}


//------------------------------------------------------------------
/**
 * �f�[�^�C���f�b�N�X���烍�[�J���n�於������̃��b�Z�[�W�f�[�^ID���擾
 *
 * @param   datID	�f�[�^�C���f�b�N�X
 *
 * @retval  u32		�n�於������̃��b�Z�[�W�f�[�^ID�iARC_MSG���j
 */
//------------------------------------------------------------------
u32 WIFI_COUNTRY_DataIndexToPlaceMsgDataID( u32 dataIndex )
{
	GF_ASSERT(dataIndex < NELEMS(NationFlag_to_AreaID));

	return NationFlag_to_AreaID[dataIndex].msg_arcID;
}

//------------------------------------------------------------------
/**
 * �f�[�^�C���f�b�N�X���獑�R�[�h���擾
 *
 * @param   dataIndex		�f�[�^�C���f�b�N�X
 *
 * @retval  u32		���R�[�h
 */
//------------------------------------------------------------------
u32 WIFI_COUNTRY_DataIndexToCountryCode( u32 dataIndex )
{
	GF_ASSERT(dataIndex < NELEMS(NationFlag_to_AreaID));

	return NationFlag_to_AreaID[dataIndex].nationID;
}


//------------------------------------------------------------------
/**
 * �f�[�^�C���f�b�N�X����n��f�[�^ID���擾
 *
 * @param   dataIndex		�f�[�^�C���f�b�N�X
 *
 * @retval  u32		�n��f�[�^ID
 */
//------------------------------------------------------------------
u32 WIFI_COUNTRY_DataIndexToPlaceDataID( u32 dataIndex )
{
	GF_ASSERT(dataIndex < NELEMS(NationFlag_to_AreaID));

	return NationFlag_to_AreaID[dataIndex].place_dataID;
}


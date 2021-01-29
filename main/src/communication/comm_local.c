//=============================================================================
/**
 * @file	comm_local.c
 * @brief	�ʐM�̒჌�x���Ȕėp�֐����W�߂�����
 * @author	Katsumi Ohno
 * @date    2005.09.12
 */
//=============================================================================


#include "common.h"
#include "wh.h"
#include "communication/communication.h"
#include "comm_local.h"
#include "comm_ring_buff.h"

//==============================================================================
/**
 * �T�[�r�X�ԍ��ɑΉ������q�@�ڑ�MAX�l�����擾
 * �T�[�r�X�ԍ��� include/communication/comm_def.h�ɂ���܂�
 * @param   serviceNo �T�[�r�X�ԍ�
 * @retval  �q�@�䐔
 */
//==============================================================================

u16 CommLocalGetServiceMaxEntry(u16 serviceNo)
{
    u8 maxBuff[]={
        COMM_MODE_TRADE_NUM_MAX,
        COMM_MODE_BATTLE_SINGLE_NUM_MAX,
        COMM_MODE_BATTLE_DOUBLE_NUM_MAX,
        COMM_MODE_BATTLE_MIX_1ON1_NUM_MAX,
        COMM_MODE_BATTLE_MULTI_NUM_MAX,
        COMM_MODE_BATTLE_MIX_2ON2_NUM_MAX,
        COMM_MODE_NUT_CRASH_NUM_MAX,
        COMM_MODE_RECORD_NUM_MAX,
        COMM_MODE_CONTEST_NUM_MAX,
        COMM_MODE_UNION_NUM_MAX,
        COMM_MODE_UNDERGROUND_NUM_MAX,
        COMM_MODE_POLLOCK4_NUM_MAX,
        COMM_MODE_POLLOCK16_NUM_MAX,
        COMM_MODE_PICTURE_NUM_MAX,
        COMM_MODE_POKETCH_NUM_MAX,
        COMM_MODE_MYSTERY_NUM_MAX,
        COMM_MODE_TOWER_MULTI_NUM_MAX,
        COMM_MODE_PARTY_NUM_MAX,
        COMM_MODE_UNION_APP_NUM_MAX,
        COMM_MODE_BATTLE_SINGLE_WIFI_NUM_MAX,
        COMM_MODE_BATTLE_DOUBLE_WIFI_NUM_MAX,
        COMM_MODE_TRADE_WIFI_NUM_MAX,
        COMM_MODE_VCHAT_WIFI_NUM_MAX,
        COMM_MODE_LOGIN_WIFI_NUM_MAX,
        COMM_MODE_DPW_WIFI_NUM_MAX,
        COMM_MODE_FUSIGI_WIFI_NUM_MAX,
		COMM_MODE_GURU2_NUM_MAX,
		COMM_MODE_FACTORY_MULTI_LV50_NUM_MAX,
		COMM_MODE_FACTORY_MULTI_OPEN_NUM_MAX,
		COMM_MODE_POFIN_WIFI_NUM_MAX,
		COMM_MODE_STAGE_MULTI_NUM_MAX,
		COMM_MODE_CASTLE_MULTI_NUM_MAX,
		COMM_MODE_ROULETTE_MULTI_NUM_MAX,
		COMM_MODE_LOBBY_WIFI_NUM_MAX,
        COMM_MODE_WIFI_FRONTIER_COMMON_NUM_MAX,
		COMM_MODE_CLUB_WIFI_NUM_MAX,
        COMM_MODE_EMAIL_WIFI_NUM_MAX,
    };
    GF_ASSERT_RETURN(serviceNo < sizeof(maxBuff), COMM_MODE_TRADE_NUM_MAX);
    return maxBuff[serviceNo];
}


//==============================================================================
/**
 * �T�[�r�X�ԍ��ɑΉ������q�@�ڑ�MAX�l�����擾
 * �T�[�r�X�ԍ��� include/communication/comm_def.h�ɂ���܂�
 * @param   serviceNo �T�[�r�X�ԍ�
 * @retval  �q�@�䐔
 */
//==============================================================================

u16 CommLocalGetServiceMinEntry(u16 serviceNo)
{
    u8 minBuff[]={
        COMM_MODE_TRADE_NUM_MIN,
        COMM_MODE_BATTLE_SINGLE_NUM_MIN,
        COMM_MODE_BATTLE_DOUBLE_NUM_MIN,
        COMM_MODE_BATTLE_MIX_1ON1_NUM_MIN,
        COMM_MODE_BATTLE_MULTI_NUM_MIN,
        COMM_MODE_BATTLE_MIX_2ON2_NUM_MIN,
        COMM_MODE_NUT_CRASH_NUM_MIN,
        COMM_MODE_RECORD_NUM_MIN,
        COMM_MODE_CONTEST_NUM_MIN,
        COMM_MODE_UNION_NUM_MIN,
        COMM_MODE_UNDERGROUND_NUM_MIN,
        COMM_MODE_POLLOCK4_NUM_MIN,
        COMM_MODE_POLLOCK16_NUM_MIN,
        COMM_MODE_PICTURE_NUM_MIN,
        COMM_MODE_POKETCH_NUM_MIN,
        COMM_MODE_MYSTERY_NUM_MIN,
        COMM_MODE_TOWER_MULTI_NUM_MIN,
        COMM_MODE_PARTY_NUM_MIN,
        COMM_MODE_UNION_APP_NUM_MIN,
        COMM_MODE_BATTLE_SINGLE_WIFI_NUM_MIN,
        COMM_MODE_BATTLE_DOUBLE_WIFI_NUM_MIN,
        COMM_MODE_TRADE_WIFI_NUM_MIN,
        COMM_MODE_VCHAT_WIFI_NUM_MIN,
        COMM_MODE_LOGIN_WIFI_NUM_MIN,
        COMM_MODE_DPW_WIFI_NUM_MIN,
        COMM_MODE_FUSIGI_WIFI_NUM_MIN,
		COMM_MODE_GURU2_NUM_MIN,
		COMM_MODE_FACTORY_MULTI_LV50_NUM_MIN,
		COMM_MODE_FACTORY_MULTI_OPEN_NUM_MIN,
		COMM_MODE_POFIN_WIFI_NUM_MIN,
		COMM_MODE_STAGE_MULTI_NUM_MIN,
		COMM_MODE_CASTLE_MULTI_NUM_MIN,
		COMM_MODE_ROULETTE_MULTI_NUM_MIN,
		COMM_MODE_LOBBY_WIFI_NUM_MIN,
        COMM_MODE_WIFI_FRONTIER_COMMON_NUM_MIN,
		COMM_MODE_CLUB_WIFI_NUM_MIN,
		COMM_MODE_EMAIL_WIFI_NUM_MIN,
    };
    GF_ASSERT_RETURN(serviceNo < sizeof(minBuff), COMM_MODE_TRADE_NUM_MIN);
    return minBuff[serviceNo];
}

//==============================================================================
/**
 * ���j�I���O���[�v���ʊ֐�
 * @param   serviceNo �T�[�r�X�ԍ�
 * @retval  yes=union
 */
//==============================================================================

BOOL CommLocalIsUnionGroup(int serviceNo)
{
    switch(serviceNo){
      case COMM_MODE_UNION_APP:
      case COMM_MODE_UNION:
      case COMM_MODE_PICTURE:
      case COMM_MODE_RECORD:
      case COMM_MODE_GURU2:
        return TRUE;
    }
    return FALSE;
}

//==============================================================================
/**
 * @brief   WiFi�O���[�v���ʊ֐�
 * @param   serviceNo �T�[�r�X�ԍ�
 * @retval  yes=union
 */
//==============================================================================

BOOL CommLocalIsWiFiGroup(int serviceNo)
{
    switch(serviceNo){
      case COMM_MODE_BATTLE_SINGLE_WIFI:
      case COMM_MODE_BATTLE_DOUBLE_WIFI:
      case COMM_MODE_TRADE_WIFI:
      case COMM_MODE_VCHAT_WIFI:
      case COMM_MODE_LOGIN_WIFI:
      case COMM_MODE_DPW_WIFI:
      case COMM_MODE_FUSIGI_WIFI:
      case COMM_MODE_WIFI_POFIN:
      case COMM_MODE_LOBBY_WIFI:
      case COMM_MODE_WIFI_FRONTIER_COMMON:
      case COMM_MODE_CLUB_WIFI:
      case COMM_MODE_EMAIL_WIFI:
        return TRUE;
    }
    return FALSE;
}

//==============================================================================
/**
 * @brief   WiFi4�l�ڑ��O���[�v���ʊ֐�
 * @param   serviceNo �T�[�r�X�ԍ�
 * @retval  yes=union
 */
//==============================================================================

BOOL CommLocalIsWiFiQuartetGroup(int serviceNo)
{
    switch(serviceNo){
      case COMM_MODE_WIFI_POFIN:   // WIFI	POFIN
      case COMM_MODE_LOBBY_WIFI:   // WIFI���r�[
      case COMM_MODE_CLUB_WIFI:   // WIFI�N���u
        return TRUE;
    }
    return FALSE;
}

//==============================================================================
/**
 * @brief   WiFi�F�B�ڑ��̃O���[�v���ʊ֐�
 * @param   serviceNo �T�[�r�X�ԍ�
 * @retval  yes=union
 */
//==============================================================================

BOOL CommLocalIsWiFiFriendGroup(int serviceNo)
{
    switch(serviceNo){
      case COMM_MODE_BATTLE_SINGLE_WIFI:
      case COMM_MODE_BATTLE_DOUBLE_WIFI:
      case COMM_MODE_TRADE_WIFI:
      case COMM_MODE_VCHAT_WIFI:
      case COMM_MODE_LOGIN_WIFI:
      case COMM_MODE_WIFI_FRONTIER_COMMON:
        return TRUE;
    }
    return FALSE;
}


//==============================================================================
/**
 * @brief   �_�C���N�g�R�[�i�[���ʊ֐�
 * @param   serviceNo �T�[�r�X�ԍ�
 * @retval  yes=union
 */
//==============================================================================

BOOL CommLocalIsDirectCounterGroup(int serviceNo)
{
    switch(serviceNo){
      case COMM_MODE_BATTLE_SINGLE:    // 1vs1 �o�g��
      case COMM_MODE_BATTLE_DOUBLE:    // 1vs1 �_�u���o�g��
      case COMM_MODE_BATTLE_MIX_1ON1:  // 1vs1 �~�b�N�X�o�g��
      case COMM_MODE_BATTLE_MULTI:     // 2vs2 �}���`�o�g��
      case COMM_MODE_BATTLE_MIX_2ON2:  // 2vs2 �~�b�N�X�o�g��
      case COMM_MODE_CONTEST:           // 2-4  �R���e�X�g
        return TRUE;
    }
    return FALSE;
}

//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wifi_p2pmatch_check.c
 *	@brief		Wi-Fi�N���u	���[�h�`�F�b�N
 *	@author		tomoya takahashi
 *	@data		2008.06.25
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "communication/comm_state.h"

#include "wifi_p2pmatch_local.h"
#include "wifi_p2pmatch_check.h"

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

//--------------------------------

//------------------------------------------------------------------
/**
 * $brief   �}�b�`���O���Ă悢���ǂ����̔���
 * @param   friendIndex���肷��F�l��index
 * @retval  �}�b�`���Ook�Ȃ�TRUE
 */
//------------------------------------------------------------------

BOOL WIFIP2PModeCheck( int friendIndex )
{
    int mySt;
    int targetSt;
    TEST_MATCH_WORK* pMatch = CommStateGetMatchWork();

    mySt = pMatch->myMatchStatus.status;
    targetSt = pMatch->friendMatchStatus[ friendIndex ].status;
    
    if((mySt == WIFI_STATUS_DBATTLE50_WAIT)&&(targetSt == WIFI_STATUS_DBATTLE50)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_DBATTLE100_WAIT)&&(targetSt == WIFI_STATUS_DBATTLE100)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_DBATTLE_FREE_WAIT)&&(targetSt == WIFI_STATUS_DBATTLE_FREE)){
        return TRUE;
    }
#ifdef WFP2P_DEBUG_EXON 
    else if((mySt == WIFI_STATUS_MBATTLE_FREE_WAIT)&&(targetSt == WIFI_STATUS_MBATTLE_FREE)){
        return TRUE;
    }
#endif
    else if((mySt == WIFI_STATUS_SBATTLE50_WAIT)&&(targetSt == WIFI_STATUS_SBATTLE50)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_SBATTLE100_WAIT)&&(targetSt == WIFI_STATUS_SBATTLE100)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_SBATTLE_FREE_WAIT)&&(targetSt == WIFI_STATUS_SBATTLE_FREE)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_TRADE_WAIT)&&(targetSt == WIFI_STATUS_TRADE)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_POFIN_WAIT)&&(targetSt == WIFI_STATUS_POFIN)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_FRONTIER_WAIT)&&(targetSt == WIFI_STATUS_FRONTIER)){
        return TRUE;
    }
#ifdef WFP2P_DEBUG_EXON 
    else if((mySt == WIFI_STATUS_BATTLEROOM_WAIT)&&(targetSt == WIFI_STATUS_BATTLEROOM)){
        return TRUE;
    }
#endif
    else if((mySt == WIFI_STATUS_BUCKET_WAIT)&&(targetSt == WIFI_STATUS_BUCKET)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_BALANCEBALL_WAIT)&&(targetSt == WIFI_STATUS_BALANCEBALL)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_BALLOON_WAIT)&&(targetSt == WIFI_STATUS_BALLOON)){
        return TRUE;
    }
    else if((mySt == WIFI_STATUS_LOGIN_WAIT)&&(targetSt == WIFI_STATUS_VCT)){
        return TRUE;
    }
    return FALSE;
}



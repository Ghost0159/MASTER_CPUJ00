//==============================================================================
/**
 * @file	wifi_counter_tool.c
 * @brief	Wifi�J�E���^�[�֘A�c�[����
 * @author	Satoshi Nohara
 * @date	2008.01.18
 */
//==============================================================================
#include "common.h"
#include "system/procsys.h"
#include "poketool/poke_tool.h"

#include "frontier_types.h"
#include "frontier_tcb_pri.h"
#include "frontier_tool.h"
#include "frontier_def.h"
#include "frontier_map.h"

#include "wifi_counter_tool.h"

#include "savedata/frontier_savedata.h"


//==============================================================================
//
//	��`	
//
//==============================================================================
#define WIFI_COUNTER_LAP_CHAR_XSIZE	(2)							//
#define WIFI_COUNTER_LAP_CHAR_YSIZE	(2)							//
#define WIFI_COUNTER_LAP_SCRN_SIZE	(WIFI_COUNTER_LAP_CHAR_XSIZE * WIFI_COUNTER_LAP_CHAR_YSIZE)
#define WIFI_COUNTER_CHAR_START		(12)						//����������f�[�^��X�ʒu
#define WIFI_COUNTER_CHAR_START2	(14)						//����������f�[�^��Y�ʒu
#define WIFI_COUNTER_CHAR_START3	(0x10*6)					//����������f�[�^�̂��郉�C��
#define NUM_WRITE_X					(14)						//��������X
#define NUM_WRITE_Y					(2)							//��������Y


//==============================================================================
//
//	�\����
//	
//==============================================================================
//BG�A�j���[�V�����Ǘ����[�N
typedef struct{
	TCB_PTR tcb;							///<����^�X�N�ւ̃|�C���^
	GF_BGL_INI* bgl;						///<
	u16 no;
	u16 wait;
}WIFI_COUNTER_MONITOR_BG_WORK;

struct _WIFI_COUNTER_MONITOR_SYS{
	GF_BGL_INI* bgl;						///<BGL�ւ̃|�C���^
	WIFI_COUNTER_MONITOR_BG_WORK* work;
};

#define WIFI_COUNTER_MONITOR_WAIT	(2)		//���j�^�[���A�j��������E�F�C�g


//==============================================================================
//
//	���j�^�[�X�N���[����؂�ւ��ăA�j��������
//	
//==============================================================================
WIFI_COUNTER_MONITOR_SYS* WifiCounterMonitor_Init( GF_BGL_INI* bgl );
static WIFI_COUNTER_MONITOR_BG_WORK* WifiCounterMonitor_TCBAdd( GF_BGL_INI* bgl );
static void WifiCounterMonitor_AnimeControl( TCB_PTR tcb, void* work );
static void WifiCounter_WriteSub( u16* src, u8 flag );

//--------------------------------------------------------------
/**
 * @brief   ������
 *
 * @param   bgl		BGL�ւ̃|�C���^
 * @param   pfd		PFD�ւ̃|�C���^
 *
 * @retval  "���[�N�̃|�C���^"
 */
//--------------------------------------------------------------
WIFI_COUNTER_MONITOR_SYS* WifiCounterMonitor_Init( GF_BGL_INI* bgl )
{
	WIFI_COUNTER_MONITOR_SYS* p_sys;
	
	p_sys = sys_AllocMemory( HEAPID_FRONTIERMAP, sizeof(WIFI_COUNTER_MONITOR_SYS) );
	MI_CpuClear8( p_sys, sizeof(WIFI_COUNTER_MONITOR_SYS) );
	
	p_sys->work = WifiCounterMonitor_TCBAdd( bgl );
	return p_sys;
}

//--------------------------------------------------------------
/**
 * @brief   TCB�쐬
 *
 * @param   bgl				BGL�ւ̃|�C���^
 *
 * @retval  BG�A�j�����䃏�[�N�̃|�C���^
 */
//--------------------------------------------------------------
static WIFI_COUNTER_MONITOR_BG_WORK* WifiCounterMonitor_TCBAdd( GF_BGL_INI* bgl )
{
	WIFI_COUNTER_MONITOR_BG_WORK* wk;
	
	wk = sys_AllocMemory( HEAPID_FRONTIERMAP, sizeof(WIFI_COUNTER_MONITOR_BG_WORK) );
	MI_CpuClear8( wk, sizeof(WIFI_COUNTER_MONITOR_BG_WORK) );
	wk->bgl = bgl;
	wk->no  = 0;
	
	wk->tcb = TCB_Add( WifiCounterMonitor_AnimeControl, wk, TCBPRI_STAGE_BGANIME_CONTROL );
	return wk;
}

//--------------------------------------------------------------
/**
 * @brief   TCB�R���g���[��
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		WIFI_COUNTER_MONITOR_BG_WORK�\����
 */
//--------------------------------------------------------------
static void WifiCounterMonitor_AnimeControl( TCB_PTR tcb, void* work )
{
	u32 i;
	u16 src[WIFI_COUNTER_LAP_SCRN_SIZE];
	WIFI_COUNTER_MONITOR_BG_WORK* wk = work;
	
	//wifi�L�ꂾ�ƁA2,3�E�F�C�g�����݂ɂ��Ă���炵�����A�����ȈႢ�Ȃ̂ŁA2�Œ�ɂ��Ă܂�
	
	if( wk->wait < WIFI_COUNTER_MONITOR_WAIT){
		wk->wait++;
		return;
	}
	wk->wait = 0;
	wk->no ^= 1;

	WifiCounter_WriteSub( src, wk->no );

	GF_BGL_ScrWrite(wk->bgl, FRMAP_FRAME_MAP, src, NUM_WRITE_X, NUM_WRITE_Y, 
					WIFI_COUNTER_LAP_CHAR_XSIZE, WIFI_COUNTER_LAP_CHAR_YSIZE );	//scrn��������

	GF_BGL_LoadScreenV_Req( wk->bgl, FRMAP_FRAME_MAP );				//�X�N���[���f�[�^�]��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�o�b�t�@�ɏ�������
 *
 * @param	src		�������݃o�b�t�@
 * @param	flag	0,1��n���ăA�j��
 *
 * @return	none
 */
//--------------------------------------------------------------
static void WifiCounter_WriteSub( u16* src, u8 flag )
{
	u32 i,j,no,start;

	if( flag == 0 ){
		start = WIFI_COUNTER_CHAR_START;
	}else{
		start = WIFI_COUNTER_CHAR_START2;
	}

	for( i=0; i < WIFI_COUNTER_LAP_CHAR_YSIZE ;i++ ){
		for( j=0; j < WIFI_COUNTER_LAP_CHAR_XSIZE ;j++ ){

			//0x10=CHAR�t�@�C����Y�I�t�Z�b�g
			no = (WIFI_COUNTER_CHAR_START3 + i * 0x10) + (start + j);
			//OS_Printf( "no = %d\n", no );
			src[ (i * WIFI_COUNTER_LAP_CHAR_XSIZE) + j ] = no;
		}
	}

	return;
}


//==============================================================================
//
//	���j�^�[�X�N���[���̃A�j���I��
//	
//==============================================================================
void WifiCounterMonitor_End( WIFI_COUNTER_MONITOR_SYS* p_sys );
static void WifiCounterMonitorBGAnime_End( WIFI_COUNTER_MONITOR_BG_WORK* wk );

//--------------------------------------------------------------
/**
 * @brief   �I��
 *
 * @param   anmsys		�X�e�[�W�A�j���[�V�����Ǘ����[�N�̃|�C���^
 */
//--------------------------------------------------------------
void WifiCounterMonitor_End( WIFI_COUNTER_MONITOR_SYS* p_sys )
{
	WifiCounterMonitorBGAnime_End( p_sys->work );
	sys_FreeMemoryEz( p_sys );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   Wifi�J�E���^�[BG�A�j���V�X�e���I��
 *
 * @param   sbw		BG�A�j�����䃏�[�N�̃|�C���^
 */
//--------------------------------------------------------------
static void WifiCounterMonitorBGAnime_End( WIFI_COUNTER_MONITOR_BG_WORK* wk )
{
	TCB_Delete( wk->tcb );
	sys_FreeMemoryEz( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   WIFI�A�����R�[�hID�擾
 *
 * @param   "���R�[�hID"
 */
//--------------------------------------------------------------
u16 WifiCounterRensyouRecordIDGet( u8 bf_no )
{
	u16 id;

	switch( bf_no ){

	case FRONTIER_NO_FACTORY_LV50:
		id = FRID_FACTORY_MULTI_WIFI_RENSHOU_CNT;
		break;

	case FRONTIER_NO_FACTORY_LV100:
		id = FRID_FACTORY_MULTI_WIFI_RENSHOU100_CNT;
		break;

	case FRONTIER_NO_CASTLE:
		id = FRID_CASTLE_MULTI_WIFI_RENSHOU_CNT;
		break;

	case FRONTIER_NO_STAGE:
		id = FRID_STAGE_MULTI_WIFI_RENSHOU_CNT;
		break;

	case FRONTIER_NO_ROULETTE:
		id = FRID_ROULETTE_MULTI_WIFI_RENSHOU_CNT;
		break;

	case FRONTIER_NO_TOWER:
		id = FRID_TOWER_MULTI_WIFI_RENSHOU_CNT;
		break;
	};

	return id;
}

//--------------------------------------------------------------
/**
 * @brief   WIFI�N���AID�擾
 *
 * @param   "���R�[�hID"
 */
//--------------------------------------------------------------
u16 WifiCounterClearRecordIDGet( u8 bf_no )
{
	u16 id;

	switch( bf_no ){

	case FRONTIER_NO_FACTORY_LV50:
		id = FRID_FACTORY_MULTI_WIFI_CLEAR_BIT;
		break;

	case FRONTIER_NO_FACTORY_LV100:
		id = FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT;
		break;

	case FRONTIER_NO_CASTLE:
		id = FRID_CASTLE_MULTI_WIFI_CLEAR_BIT;
		break;

	case FRONTIER_NO_STAGE:
		id = FRID_STAGE_MULTI_WIFI_CLEAR_BIT;
		break;

	case FRONTIER_NO_ROULETTE:
		id = FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT;
		break;

	case FRONTIER_NO_TOWER:
		id = FRID_TOWER_MULTI_WIFI_CLEAR_BIT;
		break;
	};

	return id;
}


